#include "ctrl.h"
#include "cpu.h"
#include "inst.h"

#include <cstring>

extern std::map<InstType, CtrlSignals> CtrlsigsMap;

MipsCpu::MipsCpu(size_t iram_size, size_t dram_size) : pc_(START_PC) {
  iram_ptr_ = new RAM(iram_size);
  dram_ptr_ = new RAM(dram_size);

  ctrlsigs_ = get_default_ctrl_sigs();

  trace_.open("/home/xiaoyu/workspace/MipsCpu/build/trace.txt", std::ios_base::out);
}

MipsCpu::~MipsCpu() {
  trace_.close();

  delete iram_ptr_;
  delete dram_ptr_;
}

void MipsCpu::load_inst(uint8_t* iptr, size_t size) {
  if (!iptr) {
    fatal_msg("invalid inst ptr");
  }
  inst_size_ = size;
  for (size_t i = 0; i < size; ++i) {
    iram_ptr_->set_byte(i, iptr[i]);
  }
}

void MipsCpu::load_data(uint8_t* dptr, size_t size) {
  if (!dptr) {
    fatal_msg("invalid data ptr");
  }
  data_size_ = size;
  for (size_t i = 0; i < size; ++i) {
    dram_ptr_->set_byte(i, dptr[i]);
  }
}

int MipsCpu::run() {
  while (true) {
    inst_fetch();
    inst_decode();
    inst_execute();
    inst_mem();
    inst_wb();

    if (pc_ >= (inst_size_ - 20)) {
      std::cout << "cpu run finish" << std::endl;
      break;
    }
  }

  return 0;
}

void MipsCpu::inst_fetch() {
  if (pc_ % 4 != 0) {
    fatal_msg("pc must be multiple of 4");
  }

  pc_ = next_pc_;

  inst_bit_ = static_cast<unsigned>(iram_ptr_->get_word(pc_));

  switch (ctrlsigs_.pc_sel) {
    case PcSel::PC_0:
      next_pc_ = pc_;
      break;
    case PcSel::PC_4:
      next_pc_ = pc_ + 4;
      break;
    default:
      fatal_msg("invalid pc_sel");
      break;
  }

  if (br_delay_slot_) {
    next_pc_ = br_delay_addr_;
  }
}

void MipsCpu::inst_decode() {
  rs_addr_ = inst_bit_ << 6 >> 27;
  rt_addr_ = inst_bit_ << 11 >> 27;

  int rs_data = reg_ptr_->get(rs_addr_);
  int rt_data = reg_ptr_->get(rt_addr_);

  rd_addr_ = inst_bit_ << 16 >> 27;

  ctrlsigs_ = get_ctrl_sigs(inst_bit_);

  // imm
  unsigned usext_imm_data = inst_bit_ << 16 >> 16;
  bool imm_lt_0           = (usext_imm_data & 0x00008000) != 0;
  int sext_imm_data       = imm_lt_0 ? (usext_imm_data | 0xffff0000) : usext_imm_data;

  int imm_data;
  switch (ctrlsigs_.imm_sel) {
    case ImmSel::IMM_X:
      break;
    case ImmSel::IMM_S:
      imm_data = sext_imm_data;
      break;
    case ImmSel::IMM_U:
      imm_data = usext_imm_data;
      break;
    case ImmSel::IMM_SLL_2_S: {
      int imm_sll_data = usext_imm_data << 2;
      int s_bit        = imm_sll_data & 0x00008000;
      imm_data         = s_bit ? (imm_sll_data | 0xffff0000) : (imm_sll_data & 0x0000ffff);
      break;
    }
    case ImmSel::IMM_J:
      imm_data = inst_bit_ << 6 >> 4;
      break;
    default:
      fatal_msg("invalid imm_sel");
      break;
  }

  // op1
  switch (ctrlsigs_.op1_sel) {
    case Op1Sel::OP1_X:
      break;
    case Op1Sel::OP1_RS:
      op1_data_ = rs_data;
      break;
    case Op1Sel::OP1_RT:
      op1_data_ = rt_data;
      break;
    default:
      fatal_msg("invalid Op1Sel");
      break;
  }

  // store
  store_data_ = rt_data;

  // op2
  switch (ctrlsigs_.op2_sel) {
    case Op2Sel::OP2_X:
      break;
    case Op2Sel::OP2_RT:
      op2_data_ = rt_data;
      break;
    case Op2Sel::OP2_SA: {
      int sa    = inst_bit_ << 21 >> 27;
      op2_data_ = sa;
      break;
    }
    case Op2Sel::OP2_IMM:
      op2_data_ = imm_data;
      break;
    default:
      fatal_msg("invalid Op2Sel");
      break;
  }

  // branch
  bool cond_branch   = CondBranchTrue(ctrlsigs_.br_type, op1_data_, op2_data_);
  bool direct_jump   = DirectJump(ctrlsigs_.br_type);
  bool indirect_jump = InDirectJump(ctrlsigs_.br_type);

  br_delay_slot_ = cond_branch || direct_jump || indirect_jump;

  if (cond_branch) {
    br_delay_addr_ = imm_data + pc_ + 4;
  } else if (direct_jump) {
    br_delay_addr_ = ((pc_ + 4) & 0xF0000000) | imm_data;
  } else if (indirect_jump) {
    br_delay_addr_ = rs_data;
  }
}

void MipsCpu::inst_execute() {
  switch (ctrlsigs_.alu_op) {
    case AluOp::ALU_X:
      break;
    case AluOp::ALU_ADD:
      alu_out_ = op1_data_ + op2_data_;
      break;
    case AluOp::ALU_SUB:
      alu_out_ = op1_data_ - op2_data_;
      break;
    case AluOp::ALU_SLT:
      alu_out_ = static_cast<int>(op1_data_) < static_cast<int>(op2_data_);
      break;
    case AluOp::ALU_SLTU:
      alu_out_ = static_cast<unsigned>(op1_data_) < static_cast<unsigned>(op2_data_);
      break;
    case AluOp::ALU_SLL:
      alu_out_ = op1_data_ << op2_data_;
      break;
    case AluOp::ALU_SRL:
      alu_out_ = static_cast<unsigned>(op1_data_) >> op2_data_;
      break;
    case AluOp::ALU_SRA:
      alu_out_ = static_cast<int>(op1_data_) >> op2_data_;
      break;
    case AluOp::ALU_LUI:
      alu_out_ = op2_data_ << 16;
      break;
    case AluOp::ALU_AND:
      alu_out_ = op1_data_ & op2_data_;
      break;
    case AluOp::ALU_NOR:
      alu_out_ = ~(op1_data_ | op2_data_);
      break;
    case AluOp::ALU_OR:
      alu_out_ = op1_data_ | op2_data_;
      break;
    case AluOp::ALU_XOR:
      alu_out_ = op1_data_ ^ op2_data_;
      break;
    default:
      fatal_msg("invalid alu op");
      break;
  }
}

void MipsCpu::inst_mem() {
  switch (ctrlsigs_.ld_type) {
    case LoadType::LD_W:
      load_data_ = dram_ptr_->get_word(alu_out_);
      break;
  }

  switch (ctrlsigs_.st_type) {
    case StoreType::ST_W:
      dram_ptr_->set_word(alu_out_, store_data_);
      break;
  }
}

void MipsCpu::inst_wb() {
  // wb data
  int rf_wdata;
  switch (ctrlsigs_.rf_wdata_sel) {
    case WbDataSel::WB_DATA_X:
      break;
    case WbDataSel::WB_DATA_ALU:
      rf_wdata = alu_out_;
      break;
    case WbDataSel::WB_DATA_MEM:
      rf_wdata = load_data_;
      break;
    case WbDataSel::WB_DATA_PC8:
      rf_wdata = pc_ + 8;
      break;
    default:
      fatal_msg("invalid rf_wdata_sel");
      break;
  }

  int rf_waddr;
  switch (ctrlsigs_.rf_waddr_sel) {
    case WbAddrSel::WB_ADDR_X:
      break;
    case WbAddrSel::WB_ADDR_RT:
      rf_waddr = rt_addr_;
      break;
    case WbAddrSel::WB_ADDR_RD:
      rf_waddr = rd_addr_;
      break;
    case WbAddrSel::WB_ADDR_31:
      rf_waddr = 31;
      break;
    default:
      fatal_msg("invalid rf_waddr_sel");
      break;
  }

  if (ctrlsigs_.rf_wen) {
    reg_ptr_->set(rf_waddr, rf_wdata);
    // trace log
    trace_ << std::hex << pc_ << "\t" << std::dec << std::to_string(rf_waddr) << "\t" << std::hex
           << rf_wdata << std::endl;
  }
}