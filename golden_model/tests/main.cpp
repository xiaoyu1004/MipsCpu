#include "cpu.h"

#include <iostream>

int main(int argc, const char* argv[]) {
  if (argc <= 1) {
    fatal_msg("argc must be gt 1");
  }

  for (int i = 1; i < argc; ++i) {
    const char* fname = argv[i];
    printf("fname: %s \n", fname);
  }
}