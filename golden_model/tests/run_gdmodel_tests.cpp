#include "cpu.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    fatal_msg("argc must be 2");
  }

  std::string test_fname(argv[1]);
  std::ifstream fs(test_fname);
  if (!fs.is_open()) {
    fatal_msg("test asm file open fail");
  }

  std::cout << "open test hex success: " << test_fname << std::endl;

  std::vector<int> inst_vec;

  std::string line;
  while (std::getline(fs, line)) {
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    unsigned hex;
    sscanf(line.c_str(), "%x", &hex);
    inst_vec.push_back(hex);
  }

  fs.close();

  MipsCpu mipscpu;
  mipscpu.load_inst(inst_vec.data(), inst_vec.size());
  // mipscpu.load_data();
  mipscpu.run();
}