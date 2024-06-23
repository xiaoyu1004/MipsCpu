#ifndef INST_H
#define INST_H

#include "ctrl.h"

#include <map>

enum class InstType {
  DEFAULT,
  ADDU,
  ADDIU,
  SUBU,
  LW,
  SW,
  BEQ,
  BNE,
  JAL,
  JR,
  SLT,
  SLTU,
  SLL,
  SRL,
  SRA,
  LUI,
  AND,
  OR,
  XOR,
  NOR
};

extern std::map<InstType, std::string> InstsMap;

#endif