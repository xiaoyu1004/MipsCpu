#include "cpu.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

static void ReadBin(std::string& fname, std::vector<int>& vec) {
  std::ifstream fs(fname);
  if (!fs.is_open()) {
    fatal_msg("test asm file inst open fail");
  }

  std::cout << "load inst success: " << fname << std::endl;

  std::string line;
  while (std::getline(fs, line)) {
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    unsigned hex;
    sscanf(line.c_str(), "%x", &hex);
    unsigned reverse_hex = ((hex >> 24) & 0xff) | ((hex >> 8) & 0xff00) | ((hex << 8) & 0xff0000) |
        ((hex << 24) & 0xff000000);
    vec.push_back(reverse_hex);
  }

  fs.close();
}

static void InitInstRam(std::string& inst_fname, std::vector<int>& inst_vec) {
  ReadBin(inst_fname, inst_vec);
}

static void InitDataRam(std::string& data_fname, std::vector<int>& data_vec) {
  ReadBin(data_fname, data_vec);
}

int main(int argc, const char* argv[]) {
  if (argc != 3) {
    fatal_msg("argc must be 3");
  }

  // init inst ram
  std::vector<int> inst_vec;
  std::string inst_fname(argv[1]);
  InitInstRam(inst_fname, inst_vec);

  // init data ram
  std::vector<int> data_vec;
  std::string data_fname(argv[2]);
  InitDataRam(data_fname, data_vec);

  // run
  MipsCpu* mipscpu = new MipsCpu();

  mipscpu->load_inst(reinterpret_cast<uint8_t*>(inst_vec.data()), inst_vec.size() * sizeof(int));
  mipscpu->load_data(reinterpret_cast<uint8_t*>(data_vec.data()), data_vec.size() * sizeof(int));
  mipscpu->run();

  delete mipscpu;
}