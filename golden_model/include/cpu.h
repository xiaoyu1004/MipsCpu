#ifndef CPU_H
#define CPU_H

#include "utils.h"
#include "regfile.h"
#include "ram.h"
#include "ctrl.h"

#include <cstddef>

constexpr unsigned XLEN    = 32U;
constexpr size_t IRAM_SIZE = 16384u;
constexpr size_t DRAM_SIZE = 32768u;

class MipsCpu {
 public:
  MipsCpu(size_t iram_size = IRAM_SIZE, size_t dram_size = DRAM_SIZE);

  ~MipsCpu() {
    delete[] reg_ptr_;
    delete[] iram_ptr_;
    delete[] dram_ptr_;
  }

 public:
  void load_inst(int* iptr, size_t size);
  void load_data(int8_t* dptr, size_t size);
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
  size_t inst_size_;
  size_t data_size_;

 private:
  CtrlSignals ctrlsigs_;
  unsigned inst_bit_;
  unsigned pc_;
  int rs1_data_;
  int rs2_data_;
  int alu_op1_data_;
  int alu_op2_data_;
  int alu_out_;
  uint8_t rd_addr_;
  bool int_overflow_;
};

#endif