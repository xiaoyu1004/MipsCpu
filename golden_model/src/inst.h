#ifndef INST_H
#define INST_H

#include "ctrl.h"

enum class InstOp {
    ADD = 0,
};

CtrlSignals get_ctrl_sigs(unsigned inst_bit);

#endif