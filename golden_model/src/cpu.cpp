#include "cpu.h"
#include "inst.h"

#include <cstring>

extern std::map<InstType, CtrlSignals> CtrlsigsMap;

MipsCpu::MipsCpu(size_t iram_size, size_t dram_size) : exec_cycles_(0) {
  reg_ptr_ = new RegFile;

  iram_ptr_ = new RAM(iram_size);
  dram_ptr_ = new RAM(dram_size);

  trace_.open("/home/xiaoyu/workspace/MipsCpu/build/trace.txt");
}

MipsCpu::~MipsCpu() {
  // delete reg_ptr_;
  // delete iram_ptr_;
  // delete dram_ptr_;

  trace_.close();
}

void MipsCpu::load_inst(int8_t* iptr, size_t size) {
  if (!iptr) {
    fatal_msg("invalid inst ptr");
  }
  inst_size_ = size;
  for (size_t i = 0; i < size; ++i) {
    iram_ptr_->set_word(i, iptr[i]);
  }
}

void MipsCpu::load_data(int8_t* dptr, size_t size) {
  if (!dptr) {
    fatal_msg("invalid data ptr");
  }
  data_size_ = size;
  for (size_t i = 0; i < size; ++i) {
    dram_ptr_->set_word(i, dptr[i]);
  }
}

int MipsCpu::run() {
  while (true) {
    ++exec_cycles_;
    inst_wb();
    if (mem2wb_pipe_reg_.pc >= (inst_size_ - 4)) {
      std::cout << "cpu run finish, run " << exec_cycles_ << " cycle" << std::endl;
      break;
    }

    inst_mem();
    inst_execute();
    inst_decode();
    inst_fetch();
  }

  return 0;
}

void MipsCpu::inst_fetch() {
  static unsigned pc = START_PC;

  if (pc % 4 != 0) {
    fatal_msg("pc must be multiple of 4");
  }

  unsigned next_pc;
  if (br_delay_slot_) {
    next_pc = br_delay_slot_pc_;
  } else {
    switch (if_stage_input_.pc_sel) {
      case PcSel::PC_0:
        next_pc = pc;
        break;
      case PcSel::PC_4:
        next_pc = pc + 4;
        break;
      default:
        fatal_msg("invalid pc_sel");
        break;
    }
  }

  if (if_stage_input_.stall) return;

  pc = next_pc;

  if2id_pipe_reg_.if2id_valid = true;
  if2id_pipe_reg_.pc          = pc;
  unsigned ori_inst           = static_cast<unsigned>(iram_ptr_->get_word(pc));
  if2id_pipe_reg_.inst        = ((ori_inst >> 24) & 0xff) | ((ori_inst >> 8) & 0xff00) |
      ((ori_inst << 8) & 0xff0000) | ((ori_inst << 24) & 0xff000000);

  if (if_stage_input_.br_en) {
    br_delay_slot_    = true;
    br_delay_slot_pc_ = if_stage_input_.br_addr;
  } else {
    br_delay_slot_ = false;
  }
}

void MipsCpu::inst_decode() {
  uint8_t rs_addr = if2id_pipe_reg_.inst << 6 >> 27;
  uint8_t rt_addr = if2id_pipe_reg_.inst << 11 >> 27;

  // rs forwarding
  bool rs_ex_fw = id_stage_input_.ex2id_fw.valid && id_stage_input_.ex2id_fw.rf_waddr == rs_addr &&
      rs_addr != 0;
  bool rs_mem_fw = id_stage_input_.mem2id_fw.valid &&
      id_stage_input_.mem2id_fw.rf_waddr == rs_addr && rs_addr != 0;
  bool rs_wb_fw = id_stage_input_.wb2id_fw.valid && id_stage_input_.wb2id_fw.rf_waddr == rs_addr &&
      rs_addr != 0;

  int rs_data;
  if (rs_ex_fw) {
    rs_data = id_stage_input_.ex2id_fw.alu_out;
  } else if (rs_mem_fw) {
    rs_data = id_stage_input_.mem2id_fw.mem_data;
  } else if (rs_wb_fw) {
    rs_data = id_stage_input_.wb2id_fw.rf_wdata;
  } else {
    rs_data = reg_ptr_->get(rs_addr);
  }

  // rt forwarding
  bool rt_ex_fw = id_stage_input_.ex2id_fw.valid && id_stage_input_.ex2id_fw.rf_waddr == rt_addr &&
      rt_addr != 0;
  bool rt_mem_fw = id_stage_input_.mem2id_fw.valid &&
      id_stage_input_.mem2id_fw.rf_waddr == rt_addr && rt_addr != 0;
  bool rt_wb_fw = id_stage_input_.wb2id_fw.valid && id_stage_input_.wb2id_fw.rf_waddr == rt_addr &&
      rt_addr != 0;

  int rt_data;
  if (rt_ex_fw) {
    rt_data = id_stage_input_.ex2id_fw.alu_out;
  } else if (rt_mem_fw) {
    rt_data = id_stage_input_.mem2id_fw.mem_data;
  } else if (rt_wb_fw) {
    rt_data = id_stage_input_.wb2id_fw.rf_wdata;
  } else {
    rt_data = reg_ptr_->get(rt_addr);
  }

  uint8_t rd_addr = if2id_pipe_reg_.inst << 16 >> 27;

  CtrlSignals ctrlsigs = get_ctrl_sigs(if2id_pipe_reg_.inst);

  bool load_hazard = (rs_ex_fw || rt_ex_fw) && id_stage_input_.ex2id_fw.is_load;
  bool stall       = load_hazard;

  // imm
  unsigned usext_imm_data = if2id_pipe_reg_.inst << 16 >> 16;
  bool imm_lt_0           = (usext_imm_data & 0x00008000) != 0;
  int sext_imm_data       = imm_lt_0 ? (usext_imm_data | 0xffff0000) : usext_imm_data;

  int imm_data;
  if (ctrlsigs.imm_sel == ImmSel::IMM_S) {
    imm_data = sext_imm_data;
  } else if (ctrlsigs.imm_sel == ImmSel::IMM_U) {
    imm_data = usext_imm_data;
  } else if (ctrlsigs.imm_sel == ImmSel::IMM_SLL_2_S) {
    int imm_sll_data = usext_imm_data << 2;
    int s_bit        = imm_sll_data & 0x00008000;
    imm_data         = s_bit ? (imm_sll_data | 0xffff0000) : (imm_sll_data & 0x0000ffff);
  }

  // op1
  int op1_data;
  switch (ctrlsigs.op1_sel) {
    case Op1Sel::OP1_RS:
      op1_data = rs_data;
      break;
    case Op1Sel::OP1_RT:
      op1_data = rt_data;
      break;
  }

  // op2
  int op2_data;
  switch (ctrlsigs.op2_sel) {
    case Op2Sel::OP2_RT:
      op2_data = rt_data;
      break;
    case Op2Sel::OP2_SA: {
      int sa   = if2id_pipe_reg_.inst << 21 >> 27;
      op2_data = sa;
      break;
    }
    case Op2Sel::OP2_IMM: {
      op2_data = imm_data;
      break;
    }
  }

  // branch
  bool br_en = false;
  if (ctrlsigs.br_type == BrType::BR_EQ) {
    br_en = (op1_data == op2_data);
  } else if (ctrlsigs.br_type == BrType::BR_NE) {
    br_en = (op1_data != op2_data);
  }

  // rf_waddr
  uint8_t rf_waddr;
  switch (ctrlsigs.rf_waddr_sel) {
    case WbAddrSel::WB_ADDR_RT:
      rf_waddr = rt_addr;
      break;
    case WbAddrSel::WB_ADDR_RD:
      rf_waddr = rd_addr;
      break;
  }

  id2ex_pipe_reg_.id2ex_valid  = if2id_pipe_reg_.if2id_valid && !stall;
  id2ex_pipe_reg_.pc           = if2id_pipe_reg_.pc;
  id2ex_pipe_reg_.alu_op       = ctrlsigs.alu_op;
  id2ex_pipe_reg_.alu_op1_data = op1_data;
  id2ex_pipe_reg_.alu_op2_data = op2_data;
  id2ex_pipe_reg_.ld_type      = ctrlsigs.ld_type;
  id2ex_pipe_reg_.st_type      = ctrlsigs.st_type;
  id2ex_pipe_reg_.st_data      = rt_data;
  id2ex_pipe_reg_.rf_wdata_sel = ctrlsigs.rf_wdata_sel;
  id2ex_pipe_reg_.rf_waddr     = rf_waddr;
  id2ex_pipe_reg_.rf_wen       = ctrlsigs.rf_wen;

  if (id2ex_pipe_reg_.id2ex_valid) {
    if_stage_input_.pc_sel = ctrlsigs.pc_sel;
  }
  if_stage_input_.stall   = stall;
  if_stage_input_.br_en   = br_en;
  if_stage_input_.br_addr = if2id_pipe_reg_.pc + 4 + imm_data;
}

void MipsCpu::inst_execute() {
  int op1_data = id2ex_pipe_reg_.alu_op1_data;
  int op2_data = id2ex_pipe_reg_.alu_op2_data;

  int alu_out;
  switch (id2ex_pipe_reg_.alu_op) {
    case AluOp::ALU_ADD:
      alu_out = op1_data + op2_data;
      break;
    case AluOp::ALU_SLL:
      alu_out = op1_data << op2_data;
      break;
    case AluOp::ALU_SUB:
      alu_out = op1_data - op2_data;
      break;
    default:
      fatal_msg("invalid alu op");
      break;
  }

  // to mem
  ex2mem_pipe_reg_.ex2mem_valid = id2ex_pipe_reg_.id2ex_valid;
  ex2mem_pipe_reg_.pc           = id2ex_pipe_reg_.pc;
  ex2mem_pipe_reg_.alu_out      = alu_out;
  ex2mem_pipe_reg_.ld_type      = id2ex_pipe_reg_.ld_type;
  ex2mem_pipe_reg_.st_type      = id2ex_pipe_reg_.st_type;
  ex2mem_pipe_reg_.st_data      = id2ex_pipe_reg_.st_data;
  ex2mem_pipe_reg_.rf_wdata_sel = id2ex_pipe_reg_.rf_wdata_sel;
  ex2mem_pipe_reg_.rf_waddr     = id2ex_pipe_reg_.rf_waddr;
  ex2mem_pipe_reg_.rf_wen       = id2ex_pipe_reg_.rf_wen;

  // forwarding, to id
  id_stage_input_.ex2id_fw.valid    = id2ex_pipe_reg_.id2ex_valid;
  id_stage_input_.ex2id_fw.rf_waddr = id2ex_pipe_reg_.rf_waddr;
  id_stage_input_.ex2id_fw.alu_out  = alu_out;
  id_stage_input_.ex2id_fw.is_load  = id2ex_pipe_reg_.ld_type != LoadType::LD_X;
}

void MipsCpu::inst_mem() {
  int mem_data;
  if (ex2mem_pipe_reg_.ex2mem_valid) {
    switch (ex2mem_pipe_reg_.ld_type) {
      case LoadType::LD_W:
        mem_data = dram_ptr_->get_word(ex2mem_pipe_reg_.alu_out);
        break;
    }

    switch (ex2mem_pipe_reg_.st_type) {
      case StoreType::ST_W:
        dram_ptr_->set_word(ex2mem_pipe_reg_.alu_out, ex2mem_pipe_reg_.st_data);
        break;
    }
  }

  // to wb
  mem2wb_pipe_reg_.mem2wb_valid = ex2mem_pipe_reg_.ex2mem_valid;
  mem2wb_pipe_reg_.pc           = ex2mem_pipe_reg_.pc;
  mem2wb_pipe_reg_.alu_out      = ex2mem_pipe_reg_.alu_out;
  mem2wb_pipe_reg_.mem_data     = mem_data;
  mem2wb_pipe_reg_.rf_wdata_sel = ex2mem_pipe_reg_.rf_wdata_sel;
  mem2wb_pipe_reg_.rf_waddr     = ex2mem_pipe_reg_.rf_waddr;
  mem2wb_pipe_reg_.rf_wen       = ex2mem_pipe_reg_.rf_wen;

  // forwarding, to id
  id_stage_input_.mem2id_fw.valid    = ex2mem_pipe_reg_.ex2mem_valid;
  id_stage_input_.mem2id_fw.rf_waddr = ex2mem_pipe_reg_.rf_waddr;
  id_stage_input_.mem2id_fw.mem_data = mem_data;
}

void MipsCpu::inst_wb() {
  // wb data
  int rf_wdata;
  switch (mem2wb_pipe_reg_.rf_wdata_sel) {
    case WbDataSel::WB_DATA_ALU:
      rf_wdata = mem2wb_pipe_reg_.alu_out;
      break;
    case WbDataSel::WB_DATA_MEM:
      rf_wdata = mem2wb_pipe_reg_.mem_data;
      break;
  }

  // wb rnum
  int rf_wnum = mem2wb_pipe_reg_.rf_waddr;

  bool wb_enable = mem2wb_pipe_reg_.rf_wen && mem2wb_pipe_reg_.mem2wb_valid;
  if (wb_enable) {
    reg_ptr_->set(rf_wnum, rf_wdata);
    // trace log
    trace_ << std::hex << mem2wb_pipe_reg_.pc << " " << std::dec << rf_wnum << " " << std::hex
           << rf_wdata << std::endl;
  }

  // forwarding, to id
  id_stage_input_.wb2id_fw.valid    = wb_enable;
  id_stage_input_.wb2id_fw.rf_waddr = rf_wnum;
  id_stage_input_.wb2id_fw.rf_wdata = rf_wdata;
}