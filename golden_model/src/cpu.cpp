#include "cpu.h"
#include "inst.h"

#include <cstring>

extern std::map<InstType, CtrlSignals> CtrlsigsMap;

MipsCpu::MipsCpu(size_t iram_size, size_t dram_size) : pc_(START_PC) {
  reg_ptr_ = new RegFile;

  iram_ptr_ = new RAM(iram_size);
  dram_ptr_ = new RAM(dram_size);

  ctrlsigs_ = get_default_ctrl_sigs();
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
    if (pc_ >= (inst_size_ / 4)) {
      std::cout << "cpu run finish" << std::endl;
      break;
    }
    inst_fetch();
    inst_decode();
    inst_execute();
    inst_mem();
    inst_wb();
  }

  return 0;
}

void MipsCpu::inst_fetch() {
  if (pc_ % 4 != 0) {
    fatal_msg("pc must be multiple of 4");
  }

  unsigned next_pc;
  switch (ctrlsigs_.pc_sel) {
    case PcSel::PC_0:
      next_pc = pc_;
      break;
    case PcSel::PC_4:
      next_pc = pc_ + 4;
      break;
    default:
      fatal_msg("invalid pc_sel");
      break;
  }

  inst_bit_ = static_cast<unsigned>(iram_ptr_->get_word(next_pc));
}

void MipsCpu::inst_decode() {
  uint8_t rs1_addr = inst_bit_ << 6 >> 27;
  uint8_t rs2_addr = inst_bit_ << 11 >> 27;
  rs1_data_        = reg_ptr_->get(rs1_addr);
  rs2_data_        = reg_ptr_->get(rs2_addr);

  rd_addr_ = inst_bit_ << 16 >> 27;

  ctrlsigs_ = get_ctrl_sigs(inst_bit_);

  // op1
  switch (ctrlsigs_.op1_sel) {
    case Op1Sel::OP1_RS1:
      alu_op1_data_ = rs1_data_;
      break;
  }

  // op2
  switch (ctrlsigs_.op2_sel) {
    case Op2Sel::OP2_RS2:
      alu_op2_data_ = rs2_data_;
      break;
  }
}

void MipsCpu::inst_execute() {
  switch (ctrlsigs_.alu_op) {
    case AluOp::ALU_ADD:
      alu_out_ = alu_op1_data_ + alu_op2_data_;
    default:
      fatal_msg("invalid alu op");
      break;
  }

  // overflow
  int_overflow_ = ((alu_op1_data_ < 0 && alu_op2_data_ < 0 && alu_out_ > 0) ||
                   (alu_op1_data_ > 0 && alu_op2_data_ > 0 && alu_out_ < 0));
}

void MipsCpu::inst_mem() {}

void MipsCpu::inst_wb() {
  // wb data
  int rf_wdata;
  switch (ctrlsigs_.wb_sel) {
    case WbSel::WB_ALU:
      rf_wdata = alu_out_;
      break;
  }

  // wb rnum
  int rf_wnum = rd_addr_;

  if (ctrlsigs_.wb_en) {
    reg_ptr_->set(rf_wnum, rf_wdata);
  }
}