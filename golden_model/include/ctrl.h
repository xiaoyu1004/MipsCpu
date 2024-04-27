#ifndef CTRL_H
#define CTRL_H

#include <bitset>

constexpr unsigned START_PC = 0x00000000U;
constexpr unsigned WORD_LEN = 32u;

enum class AluOp { ADD = 0 };

enum class AluOp1Sel { OP1_X = 0, OP1_RS1 = 1 };

enum class AluOp2Sel { OP2_X = 0, OP2_RS2 = 1 };

struct CtrlSignals {
  // reg
  AluOp1Sel alu_op1_sel;
  AluOp2Sel alu_op2_sel;

  AluOp alu_op;
  bool rf_wen;
};

#endif