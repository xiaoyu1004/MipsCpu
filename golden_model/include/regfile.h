#ifndef REGFILE_H
#define REGFILE_H

#include "utils.h"

constexpr int REG_NUM = 32;

class RegFile {
   public:
    RegFile() { reg_ptr_ = new int[REG_NUM]{}; }

    ~RegFile() { delete[] reg_ptr_; }

    int get(int num) {
        if (num < 0 || num >= REG_NUM) {
            fatal_msg("reg num must gt 0 and lt 31");
        }
        return (num == 0 ? 0 : reg_ptr_[num]);
    }

    void set(int num, int val) {
        if (num < 0 || num >= REG_NUM) {
            fatal_msg("reg num must gt 0 and lt 31");
        }
        reg_ptr_[num] = val;
    }

   private:
    int* reg_ptr_;
};

#endif