#ifndef CPU_H
#define CPU_H

#include "utils.h"
#include "regfile.h"
#include "ram.h"
#include "ctrl.h"

#include <cstddef>

class MipsCpu {
 public:
  MipsCpu(size_t iram_size, size_t dram_size) : pc_(START_PC) {
    reg_ptr_ = new RegFile;
    iram_ptr_ = new RAM(iram_size);
    dram_ptr_ = new RAM(dram_size);
  }

  ~MipsCpu() {
    delete[] reg_ptr_;
    delete[] iram_ptr_;
    delete[] dram_ptr_;
  }

 public:
  int run();

 private:
  void inst_fetch();
  void inst_decode();
  void inst_execute();
  void inst_mem();
  void inst_wb();

 private:
  RegFile* reg_ptr_;
  RAM* iram_ptr_;
  RAM* dram_ptr_;

 private:
  CtrlSignals ctrlsigs_;
  unsigned inst_bit_;
  unsigned pc_;
  int rs1_data_;
  int rs2_data_;
  int alu_op1_data_;
  int alu_op2_data_;
  int alu_out_;
  uint8_t rd_;
};

#endif