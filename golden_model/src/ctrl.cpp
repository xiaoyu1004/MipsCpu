#include "ctrl.h"
#include "inst.h"
#include "utils.h"
#include "cpu.h"

#include <cstring>
#include <map>
#include <algorithm>

// clang-format off
std::map<InstType, CtrlSignals> CtrlsigsMap = {
  {InstType::DEFAULT, {.pc_sel  = PcSel::PC_0, .op1_sel = Op1Sel::OP1_X, .op2_sel = Op2Sel::OP2_X, .alu_op  = AluOp::ALU_ADD, .br_type = BrType::BR_X, .st_type = StoreType::ST_X, .ld_type = LoadType::LD_X, .rf_wdata_sel  = WbDataSel::WB_DATA_X, .rf_waddr_sel = WbAddrSel::WB_ADDR_X, .wb_en  = false}},
  {InstType::ADDU, {.pc_sel  = PcSel::PC_4, .op1_sel = Op1Sel::OP1_RS, .op2_sel = Op2Sel::OP2_RT, .alu_op  = AluOp::ALU_ADD, .br_type = BrType::BR_X, .st_type = StoreType::ST_X, .ld_type = LoadType::LD_X, .rf_wdata_sel  = WbDataSel::WB_DATA_ALU, .rf_waddr_sel = WbAddrSel::WB_ADDR_RD, .wb_en  = true}},
  {InstType::ADDIU, {.pc_sel  = PcSel::PC_4, .op1_sel = Op1Sel::OP1_RS, .op2_sel = Op2Sel::OP2_IMM, .alu_op  = AluOp::ALU_ADD, .br_type = BrType::BR_X, .st_type = StoreType::ST_X, .ld_type = LoadType::LD_X, .rf_wdata_sel  = WbDataSel::WB_DATA_ALU, .rf_waddr_sel = WbAddrSel::WB_ADDR_RT, .wb_en  = true}},
  {InstType::SLL, {.pc_sel  = PcSel::PC_4, .op1_sel = Op1Sel::OP1_RT, .op2_sel = Op2Sel::OP2_SA, .alu_op  = AluOp::ALU_SLL, .br_type = BrType::BR_X, .st_type = StoreType::ST_X, .ld_type = LoadType::LD_X, .rf_wdata_sel  = WbDataSel::WB_DATA_ALU, .rf_waddr_sel = WbAddrSel::WB_ADDR_RD, .wb_en  = true}},
};
// clang-format on

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
      flag &= !(static_cast<int>(m.second[i] - '0') ^ inst_bit_[XLEN - 1 - i]);
    }
    return flag;
  });
  if (inst_it == InstsMap.end()) {
    fatal_msg("invalid inst");
  }
  auto ctrl_it = std::find_if(CtrlsigsMap.begin(), CtrlsigsMap.end(),
                              [&](auto& m) { return m.first == inst_it->first; });
  if (ctrl_it == std::end(CtrlsigsMap)) {
    fatal_msg("invalid inst");
  }
  ctrl_it->second.inst_type = ctrl_it->first;
  return ctrl_it->second;
}