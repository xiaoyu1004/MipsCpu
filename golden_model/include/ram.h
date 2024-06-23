#ifndef RAM_H
#define RAM_H

#include "utils.h"

#include <cstdint>
#include <cstddef>

class RAM {
 public:
  RAM(size_t size) : ram_size_(size) { data_ptr_ = new uint8_t[size]; }

  ~RAM() { delete[] data_ptr_; }

 public:
  uint8_t get_byte(size_t addr) {
    if (addr >= ram_size_) {
      fatal_msg("invalid addr, addr gt ram size");
    }
    return data_ptr_[addr];
  }

  void set_byte(size_t addr, uint8_t val) {
    if (addr >= ram_size_) {
      fatal_msg("invalid addr, addr gt ram size");
    }
    data_ptr_[addr] = val;
  }

  unsigned get_word(size_t addr) {
    unsigned u = static_cast<unsigned>(get_byte(addr + 0));
    unsigned v = static_cast<unsigned>(get_byte(addr + 1));
    unsigned x = static_cast<unsigned>(get_byte(addr + 2));
    unsigned w = static_cast<unsigned>(get_byte(addr + 3));

    return (w << 24) | (x << 16) | (v << 8) | u;
  }

  void set_word(size_t addr, unsigned val) {
    for (int i = 0; i < 4; ++i) {
      uint8_t v = static_cast<uint8_t>((val & (0x000000FF << (i * 8))) >> (i * 8));
      set_byte(addr + i, v);
    }
  }

 private:
  uint8_t* data_ptr_ = nullptr;
  size_t ram_size_;
};

#endif