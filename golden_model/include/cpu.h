#ifndef CPU_H
#define CPU_H

#include "utils.h"
#include "regfile.h"
#include "ram.h"
#include "ctrl.h"

#include <cstddef>
#include <fstream>

constexpr unsigned XLEN = 32U;

constexpr size_t IRAM_SIZE = 16384u;
constexpr size_t DRAM_SIZE = 32768u;

constexpr unsigned NOP = 0x00000000;

struct IF2IDPipeReg {
  bool if2id_valid = false;
  unsigned pc      = START_PC;
  unsigned inst    = NOP;
};

struct ID2EXPipeReg {
  bool id2ex_valid       = false;
  unsigned pc            = START_PC;
  AluOp alu_op           = AluOp::ALU_ADD;
  int alu_op1_data       = 0;
  int alu_op2_data       = 0;
  LoadType ld_type       = LoadType::LD_X;
  StoreType st_type      = StoreType::ST_X;
  WbDataSel rf_wdata_sel = WbDataSel::WB_DATA_X;
  WbAddrSel rf_waddr_sel = WbAddrSel::WB_ADDR_X;
  uint8_t rf_waddr       = 0;
  bool wb_en             = false;
};

struct EX2MEMPipeReg {
  bool ex2mem_valid      = false;
  unsigned pc            = START_PC;
  int alu_out            = 0;
  LoadType ld_type       = LoadType::LD_X;
  StoreType st_type      = StoreType::ST_X;
  WbDataSel rf_wdata_sel = WbDataSel::WB_DATA_X;
  WbAddrSel rf_waddr_sel = WbAddrSel::WB_ADDR_X;
  uint8_t rf_waddr       = 0;
  bool wb_en             = false;
};

struct MEM2WBPipeReg {
  bool mem2wb_valid      = false;
  unsigned pc            = START_PC;
  int rf_wdata           = 0;
  WbDataSel rf_wdata_sel = WbDataSel::WB_DATA_X;
  WbAddrSel rf_waddr_sel = WbAddrSel::WB_ADDR_X;
  uint8_t rf_waddr       = 0;
  bool wb_en             = false;
};

struct IFInput {
  PcSel pc_sel = PcSel::PC_0;
};

struct EX2IDForwarding {
  bool valid;
  uint8_t rf_waddr;
  int alu_out;
};

struct MEM2IDForwarding {
  bool valid;
  uint8_t rf_waddr;
  int mem_data;
};

struct WB2IDForwarding {
  bool valid;
  uint8_t rf_waddr;
  int rf_wdata;
};

struct IDInput {
  EX2IDForwarding ex2id_fw;
  MEM2IDForwarding mem2id_fw;
  WB2IDForwarding wb2id_fw;
};

class MipsCpu {
 public:
  MipsCpu(size_t iram_size = IRAM_SIZE, size_t dram_size = DRAM_SIZE);

  ~MipsCpu();

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
  std::ofstream trace_;
  size_t exec_cycles_;

 private:
  size_t inst_size_;
  size_t data_size_;

 private:
  IF2IDPipeReg if2id_pipe_reg_;
  ID2EXPipeReg id2ex_pipe_reg_;
  EX2MEMPipeReg ex2mem_pipe_reg_;
  MEM2WBPipeReg mem2wb_pipe_reg_;

  IFInput if_stage_input_;
  IDInput id_stage_input_;
};

#endif