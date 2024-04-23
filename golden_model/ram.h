#ifndef RAM_H
#define RAM_H

#include "utils.h"

#include <cstdint>
#include <cstddef>

class RAM
{
public:
    RAM(size_t size) : ram_size_(size)
    {
        ram_ptr_ = new int8_t[size];
    }
    ~RAM()
    {
        delete[] ram_ptr_;
    }

public:
    int get_word(size_t addr)
    {
        if (addr >= ram_size_)
        {
            fatal_msg("invalid addr, addr gt ram size");
        }
        return *reinterpret_cast<int *>(ram_ptr_ + addr);
    }

    void set_word(size_t addr, int val)
    {
        if (addr >= ram_size_)
        {
            fatal_msg("invalid addr, addr gt ram size");
        }

        *reinterpret_cast<int *>(ram_ptr_ + addr) = val;
    }

private:
    int8_t *ram_ptr_ = nullptr;
    size_t ram_size_;
};

#endif