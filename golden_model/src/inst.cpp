#include "inst.h"

#include <cstring>
#include <map>

std::map<InstType, std::string> Instructions = {
    {InstType::INST_ADD, "000000????????????????????100000"}};

// clang-format off
std::map<InstType, CtrlSignals> ctrlsigs_map = {
  {InstType::INST_ADD, {.pc_sel  = PcSel::PC_4, .op1_sel = Op1Sel::OP1_RS1, .op2_sel = Op2Sel::OP2_RS2, .alu_op  = AluOp::ALU_ADD, .imm_sel = ImmSel::IMM_X, .br_type = BrType::BR_X, .st_type = StoreType::ST_X, .ld_type = LoadType::LD_X, .wb_sel  = WbSel::WB_ALU, .wb_en  = true}}
};
// clang-format on

CtrlSignals get_ctrl_sigs(unsigned inst_bit) {
  CtrlSignals ctrl;
  unsigned opcode = inst_bit >> 26;
  unsigned func   = inst_bit << 26 >> 26;
  return ctrl;
}