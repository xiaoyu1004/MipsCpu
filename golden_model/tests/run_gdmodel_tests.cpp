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
  inst_vec.reserve(20);

  std::string line;
  while (std::getline(fs, line)) {
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    inst_vec.push_back(std::stoi(line));
  }

  fs.close();

  std::cout << inst_vec.size() << std::endl;

  MipsCpu mipscpu;
  mipscpu.load_inst(inst_vec.data(), inst_vec.size());
  // mipscpu.load_data();
  mipscpu.run();
}