#ifndef INST_H
#define INST_H

#include "ctrl.h"

enum class InstType {
  INST_ADD,
};

CtrlSignals get_ctrl_sigs(unsigned inst_bit);

#endif