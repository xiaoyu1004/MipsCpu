#ifndef CTRL_H
#define CTRL_H

#include "inst.h"

#include <bitset>

constexpr unsigned START_PC = 0x00000000U;
constexpr unsigned WORD_LEN = 32u;

// pc_sel
enum class PcSel { PC_0, PC_4 };

// op1_sel
enum class Op1Sel { OP1_X, OP1_RS, OP1_RT, OP1_PC };

// op2_sel
enum class Op2Sel { OP2_X, OP2_RT, OP2_SA, OP2_IMM };

// imm_sel
enum class ImmSel { IMM_X, IMM_S, IMM_SLL_2_S, IMM_U, IMM_Z, IMM_J };

enum class AluOp {
  ALU_X,
  ALU_ADD,
  ALU_SUB,
  ALU_SLL,
  ALU_SRL,
  ALU_SRA,
  ALU_SLT,
  ALU_SLTU,
  ALU_DIV,
  ALU_DIVU,
  ALU_MUL,
  ALU_MULU,
  ALU_LUI,
  ALU_AND,
  ALU_NOR,
  ALU_OR,
  ALU_XOR
};

// br_type
enum class BrType { BR_X, BR_EQ, BR_NE, BR_GEZ, BR_GTZ, BR_LEZ, BR_LTZ, BR_DIRECT_JUMP, BR_INDIRECT_JUMP };

bool CondBranchTrue(BrType br_type, int op1_data, int op2_data);

bool DirectJump(BrType br_type);

bool InDirectJump(BrType br_type);

// st_type
enum class StoreType { ST_X, ST_W, ST_H, ST_B };

// ld_type
enum class LoadType { LD_X, LD_W, LD_H, LD_B };

// wb_sel
enum class WbDataSel { WB_DATA_X, WB_DATA_ALU, WB_DATA_MEM, WB_DATA_PC8 };

// wb addr
enum class WbAddrSel { WB_ADDR_X, WB_ADDR_RT, WB_ADDR_RD, WB_ADDR_31 };

struct CtrlSignals {
  InstType inst_type;
  PcSel pc_sel;
  Op1Sel op1_sel;
  Op2Sel op2_sel;
  ImmSel imm_sel;
  AluOp alu_op;
  BrType br_type;
  StoreType st_type;
  LoadType ld_type;
  WbDataSel rf_wdata_sel;
  WbAddrSel rf_waddr_sel;
  bool rf_wen;
};

CtrlSignals get_ctrl_sigs(unsigned inst_bit);
CtrlSignals get_default_ctrl_sigs();

#endif