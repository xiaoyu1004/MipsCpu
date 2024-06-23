#include "cpu.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

static void InitInstRam(std::string& inst_fname, std::vector<int>& inst_vec) {
  std::ifstream fs(inst_fname);
  if (!fs.is_open()) {
    fatal_msg("test asm file open fail");
  }

  std::cout << "load inst success: " << inst_fname << std::endl;

  std::string line;
  while (std::getline(fs, line)) {
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    unsigned hex;
    sscanf(line.c_str(), "%x", &hex);
    inst_vec.push_back(hex);
  }

  fs.close();
}

static void InitDataRam(std::vector<int>& data_vec) {
  size_t size = data_vec.size();
  for (size_t i = 0; i < size; ++i) {
    if (i < 8) {
      data_vec[i] = 1 - 2 * i;
    } else {
      data_vec[i] = 0;
    }
  }
}

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    fatal_msg("argc must be 2");
  }

  // init inst ram
  std::vector<int> inst_vec;
  std::string inst_fname(argv[1]);
  InitInstRam(inst_fname, inst_vec);

  // init data ram
  std::vector<int> data_vec(1024);
  InitDataRam(data_vec);

  // run
  MipsCpu* mipscpu = new MipsCpu();

  mipscpu->load_inst(reinterpret_cast<uint8_t*>(inst_vec.data()), inst_vec.size() * sizeof(int));
  mipscpu->load_data(reinterpret_cast<uint8_t*>(data_vec.data()), data_vec.size() * sizeof(int));
  mipscpu->run();

  delete mipscpu;
}