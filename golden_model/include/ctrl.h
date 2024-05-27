#ifndef CTRL_H
#define CTRL_H

#include "inst.h"

#include <bitset>

constexpr unsigned START_PC = 0x00000000U;
constexpr unsigned WORD_LEN = 32u;

// pc_sel
enum class PcSel { PC_0, PC_4, PC_ALU };

// op1_sel
enum class Op1Sel { OP1_X, OP1_RS, OP1_RT, OP1_PC };

// op2_sel
enum class Op2Sel { OP2_X, OP2_RT, OP2_SA, OP2_IMM };

enum class AluOp {
  ALU_ADD,
  ALU_ADDU,
  ALU_SUB,
  ALU_SUBU,
  ALU_SLL,
  ALU_SLT,
  ALU_SLTU,
  ALU_DIV,
  ALU_DIVU,
  ALU_MUL,
  ALU_MULU,
  ALU_AND,
  ALU_NOR,
  ALU_OR,
  ALU_XOR
};

// br_type
enum class BrType { BR_X, BR_EQ, BR_NE, BR_GEZ, BR_GTZ, BR_LEZ, BR_LTZ };

// st_type
enum class StoreType { ST_X, ST_W, ST_H, ST_B };

// ld_type
enum class LoadType { LD_X, LD_W, LD_H, LD_B };

// wb_sel
enum class WbDataSel { WB_DATA_X, WB_DATA_ALU, WB_DATA_MEM, WB_DATA_PC4 };

// wb addr
enum class WbAddrSel { WB_ADDR_X, WB_ADDR_RT, WB_ADDR_RD };

struct CtrlSignals {
  InstType inst_type;
  PcSel pc_sel;
  Op1Sel op1_sel;
  Op2Sel op2_sel;
  AluOp alu_op;
  BrType br_type;
  StoreType st_type;
  LoadType ld_type;
  WbDataSel rf_wdata_sel;
  WbAddrSel rf_waddr_sel;
  bool wb_en;
};

CtrlSignals get_ctrl_sigs(unsigned inst_bit);
CtrlSignals get_default_ctrl_sigs();

#endif