#ifndef INST_H
#define INST_H

#include <map>

enum class InstType { DEFAULT, ADDU, ADDIU, SLL };

extern std::map<InstType, std::string> InstsMap;

#endif