#include "ctrl.h"
#include "inst.h"
#include "utils.h"
#include "cpu.h"

#include <cstring>
#include <map>
#include <algorithm>

// clang-format off
std::map<InstType, CtrlSignals> CtrlsigsMap = {
  {InstType::DEFAULT, {.pc_sel  = PcSel::PC_0, .op1_sel = Op1Sel::OP1_X, .op2_sel = Op2Sel::OP2_X, .alu_op  = AluOp::ALU_ADD, .imm_sel = ImmSel::IMM_X, .br_type = BrType::BR_X, .st_type = StoreType::ST_X, .ld_type = LoadType::LD_X, .wb_sel  = WbSel::WB_X, .wb_en  = false}},
  {InstType::INST_ADDU, {.pc_sel  = PcSel::PC_4, .op1_sel = Op1Sel::OP1_RS1, .op2_sel = Op2Sel::OP2_RS2, .alu_op  = AluOp::ALU_ADD, .imm_sel = ImmSel::IMM_X, .br_type = BrType::BR_X, .st_type = StoreType::ST_X, .ld_type = LoadType::LD_X, .wb_sel  = WbSel::WB_ALU, .wb_en  = true}}
};
// clang-format on

CtrlSignals get_default_ctrl_sigs(){
  return CtrlsigsMap[InstType::DEFAULT];
}

CtrlSignals get_ctrl_sigs(unsigned inst_bit) {
  auto inst_it = std::find_if(InstsMap.begin(), InstsMap.end(), [&](auto& m) {
    if (m.second.size() != XLEN) {
      fatal_msg("invalid inst bit str");
    }
    std::bitset<XLEN> inst_bit_{inst_bit};
    bool flag = true;
    for (size_t i = 0; i < XLEN; ++i) {
      if (m.second[i] == '?') {
        continue;
      }
      flag &= !(static_cast<int>(m.second[i]) ^ inst_bit_[i]);
    }
    return flag;
  });
  if (inst_it == std::end(InstsMap)) {
    fatal_msg("invalid inst");
  }
  auto ctrl_it = std::find_if(CtrlsigsMap.begin(), CtrlsigsMap.end(),
                              [&](auto& m) { return m.first == inst_it->first; });
  if (ctrl_it == std::end(CtrlsigsMap)) {
    fatal_msg("invalid inst");
  }
  return ctrl_it->second;
}