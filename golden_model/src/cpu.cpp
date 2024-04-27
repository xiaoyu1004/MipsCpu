#include "cpu.h"
#include "inst.h"

int MipsCpu::run() {
  inst_fetch();
  inst_decode();
  inst_execute();
  inst_mem();
  inst_wb();

  return 0;
}

void MipsCpu::inst_fetch() {
  if (pc_ % 4 != 0) {
    fatal_msg("pc must be multiple of 4");
  }

  unsigned next_pc = pc_ + 4;
  // if ()

  inst_bit_ = static_cast<unsigned>(iram_ptr_->get_word(next_pc));
}

void MipsCpu::inst_decode() {
  uint8_t rs = inst_bit_ << 6 >> 27;
  uint8_t rt = inst_bit_ << 11 >> 27;
  rs1_data_  = reg_ptr_->get(rs);
  rs2_data_  = reg_ptr_->get(rt);

  rd_ = inst_bit_ << 16 >> 27;

  ctrlsigs_ = get_ctrl_sigs(inst_bit_);

  alu_op1_data_ = rs1_data_;
  alu_op2_data_ = rs2_data_;
}

void MipsCpu::inst_execute() {
  switch (ctrlsigs_.alu_op) {
    case AluOp::ALU_ADD:
      alu_out_ = alu_op1_data_ + alu_op2_data_;
    default:
      fatal_msg("invalid alu op");
      break;
  }
}

void MipsCpu::inst_mem() {}

void MipsCpu::inst_wb() {
  if (ctrlsigs_.wb_en) {
    int rf_wdata = alu_out_;
    int rf_wnum  = rd_;
    reg_ptr_->set(rf_wnum, rf_wdata);
  }
}