#ifndef CPU_H
#define CPU_H

#include "utils.h"
#include "regfile.h"
#include "ram.h"
#include "ctrl.h"

#include <cstddef>

constexpr unsigned XLEN = 32U;

constexpr size_t IRAM_SIZE = 16384u;
constexpr size_t DRAM_SIZE = 32768u;

constexpr unsigned NOP = 0x00000000;

struct IF2IDPipeReg {
  unsigned pc   = START_PC;
  unsigned inst = NOP;
};

struct ID2EXPipeReg {
  unsigned pc       = START_PC;
  AluOp alu_op      = AluOp::ALU_ADD;
  int alu_op1_data  = 0;
  int alu_op2_data  = 0;
  LoadType ld_type  = LoadType::LD_X;
  StoreType st_type = StoreType::ST_X;
  WbSel wb_sel      = WbSel::WB_X;
  uint8_t rf_waddr  = 0;
  bool wb_en        = false;
};

struct EX2MEMPipeReg {
  unsigned pc       = START_PC;
  int alu_out       = 0;
  LoadType ld_type  = LoadType::LD_X;
  StoreType st_type = StoreType::ST_X;
  WbSel wb_sel      = WbSel::WB_X;
  uint8_t rf_waddr  = 0;
  bool wb_en        = false;
};

struct MEM2WBPipeReg {
  unsigned pc      = START_PC;
  int alu_out      = 0;
  WbSel wb_sel     = WbSel::WB_X;
  uint8_t rf_waddr = 0;
  bool wb_en       = false;
};

struct IFInput {
  PcSel pc_sel;
};

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
  IF2IDPipeReg if2id_pipe_reg_;
  ID2EXPipeReg id2ex_pipe_reg_;
  EX2MEMPipeReg ex2mem_pipe_reg_;
  MEM2WBPipeReg mem2wb_pipe_reg_;

  IFInput if_input_;
};

#endif