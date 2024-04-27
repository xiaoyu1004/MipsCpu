#include "inst.h"

#include <cstring>
#include <map>

std::map<InstOp, std::string> Instructions {
    {InstOp::ADD, "000000????????????????????100000"}
 };

CtrlSignals get_ctrl_sigs(unsigned inst_bit) {
  CtrlSignals ctrl;
  unsigned opcode = inst_bit >> 26;
  unsigned func = inst_bit << 26 >> 26;
  return ctrl;
}