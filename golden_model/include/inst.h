#ifndef INST_H
#define INST_H

#include <map>

enum class InstType {
  DEFAULT,
  INST_ADDU,
};

extern std::map<InstType, std::string> InstsMap;

#endif