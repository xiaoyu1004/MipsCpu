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

  std::vector<int> data_vec{8, 7, 6, 5, 4, 3, 2, 1};

  MipsCpu* mipscpu = new MipsCpu;
  mipscpu->load_inst(reinterpret_cast<int8_t*>(inst_vec.data()), inst_vec.size() * sizeof(int));
  mipscpu->load_data(reinterpret_cast<int8_t*>(data_vec.data()), data_vec.size() * sizeof(int));
  mipscpu->run();
  delete mipscpu;
}