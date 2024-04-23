#ifndef REGFILE_H
#define REGFILE_H

#include "cpu.h"
#include "utils.h"

constexpr int REG_NUM = 32;

class RegFile
{
public:
    RegFile() {}
    
    int get(int num)
    {
        if (num < 0 || num >= REG_NUM)
        {
            fatal_msg("reg num must gt 0 and lt 31");
        }
        return reg_[num];
    }

    void set(int num, int val)
    {
        if (num < 0 || num >= REG_NUM)
        {
            fatal_msg("reg num must gt 0 and lt 31");
        }
        reg_[num] = val;
    }

private:
    int reg_[32] = {};
};

#endif