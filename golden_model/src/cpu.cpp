#include "cpu.h"
#include "inst.h"

#include <cstring>

extern std::map<InstType, CtrlSignals> CtrlsigsMap;

MipsCpu::MipsCpu(size_t iram_size, size_t dram_size) {
  reg_ptr_ = new RegFile;

  iram_ptr_ = new RAM(iram_size);
  dram_ptr_ = new RAM(dram_size);

  // ctrlsigs_ = get_default_ctrl_sigs();
}

void MipsCpu::load_inst(int* iptr, size_t size) {
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
  memcpy(dram_ptr_, dptr, size);
}

int MipsCpu::run() {
  while (true) {
    inst_wb();
    if (mem2wb_pipe_reg_.pc >= (inst_size_ / 4 - 4)) {
      std::cout << "cpu run finish" << std::endl;
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
  switch (if_input_.pc_sel) {
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

  pc = next_pc;

  if2id_pipe_reg_.pc   = pc;
  if2id_pipe_reg_.inst = static_cast<unsigned>(iram_ptr_->get_word(pc));
}

void MipsCpu::inst_decode() {
  uint8_t rs1_addr = if2id_pipe_reg_.inst << 6 >> 27;
  uint8_t rs2_addr = if2id_pipe_reg_.inst << 11 >> 27;
  int rs1_data     = reg_ptr_->get(rs1_addr);
  int rs2_data     = reg_ptr_->get(rs2_addr);

  uint8_t rd_addr = if2id_pipe_reg_.inst << 16 >> 27;

  CtrlSignals ctrlsigs = get_ctrl_sigs(if2id_pipe_reg_.inst);

  // op1
  int op1_data;
  switch (ctrlsigs.op1_sel) {
    case Op1Sel::OP1_RS1:
      op1_data = rs1_data;
      break;
  }

  // op2
  int op2_data;
  switch (ctrlsigs.op2_sel) {
    case Op2Sel::OP2_RS2:
      op2_data = rs2_data;
      break;
  }

  id2ex_pipe_reg_.pc           = if2id_pipe_reg_.pc;
  id2ex_pipe_reg_.alu_op       = ctrlsigs.alu_op;
  id2ex_pipe_reg_.alu_op1_data = op1_data;
  id2ex_pipe_reg_.alu_op2_data = op2_data;
  id2ex_pipe_reg_.ld_type      = ctrlsigs.ld_type;
  id2ex_pipe_reg_.st_type      = ctrlsigs.st_type;
  id2ex_pipe_reg_.wb_sel       = ctrlsigs.wb_sel;
  id2ex_pipe_reg_.rf_waddr     = rd_addr;
  id2ex_pipe_reg_.wb_en        = ctrlsigs.wb_en;
}

void MipsCpu::inst_execute() {
  int op1_data = id2ex_pipe_reg_.alu_op1_data;
  int op2_data = id2ex_pipe_reg_.alu_op2_data;

  int alu_out;
  switch (id2ex_pipe_reg_.alu_op) {
    case AluOp::ALU_ADD:
      alu_out = op1_data + op2_data;
      break;
    default:
      fatal_msg("invalid alu op");
      break;
  }

  ex2mem_pipe_reg_.pc       = id2ex_pipe_reg_.pc;
  ex2mem_pipe_reg_.alu_out  = alu_out;
  ex2mem_pipe_reg_.ld_type  = id2ex_pipe_reg_.ld_type;
  ex2mem_pipe_reg_.st_type  = id2ex_pipe_reg_.st_type;
  ex2mem_pipe_reg_.wb_sel   = id2ex_pipe_reg_.wb_sel;
  ex2mem_pipe_reg_.rf_waddr = id2ex_pipe_reg_.rf_waddr;
  ex2mem_pipe_reg_.wb_en    = id2ex_pipe_reg_.wb_en;
}

void MipsCpu::inst_mem() {
  int load_data;
  switch (ex2mem_pipe_reg_.ld_type) {
    case LoadType::LD_W:
      break;
    default:
      fatal_msg("invalid load type");
      break;
  }

  mem2wb_pipe_reg_.pc       = ex2mem_pipe_reg_.pc;
  mem2wb_pipe_reg_.alu_out  = ex2mem_pipe_reg_.alu_out;
  mem2wb_pipe_reg_.wb_sel   = ex2mem_pipe_reg_.wb_sel;
  mem2wb_pipe_reg_.rf_waddr = ex2mem_pipe_reg_.rf_waddr;
  mem2wb_pipe_reg_.wb_en    = ex2mem_pipe_reg_.wb_en;
}

void MipsCpu::inst_wb() {
  // wb data
  int rf_wdata;
  switch (mem2wb_pipe_reg_.wb_sel) {
    case WbSel::WB_ALU:
      rf_wdata = mem2wb_pipe_reg_.alu_out;
      break;
  }

  // wb rnum
  int rf_wnum = mem2wb_pipe_reg_.rf_waddr;

  if (mem2wb_pipe_reg_.wb_en) {
    reg_ptr_->set(rf_wnum, rf_wdata);
  }
}