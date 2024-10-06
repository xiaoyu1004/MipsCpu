#ifndef CPU_H
#define CPU_H

#include "utils.h"
#include "regfile.h"
#include "ram.h"
#include "ctrl.h"
#include "confreg.h"

#include <cstddef>
#include <fstream>

constexpr unsigned XLEN    = 32U;
constexpr size_t IRAM_SIZE = 16384u;
constexpr size_t DRAM_SIZE = 32768u;

class MipsCpu {
   public:
    MipsCpu(std::string d, size_t iram_size = IRAM_SIZE, size_t dram_size = DRAM_SIZE);
    ~MipsCpu();

   public:
    void load_inst(uint8_t* iptr, size_t size);
    void load_data(uint8_t* dptr, size_t size);
    int run();

   private:
    void inst_fetch();
    void inst_decode();
    void inst_execute();
    void inst_mem();
    void inst_wb();

   private:
    RegFile* reg_ptr_;
    RAM* iram_ptr_;
    RAM* dram_ptr_;
    Confreg* conf_ptr_;
    std::ofstream trace_;

   private:
    size_t inst_size_;
    size_t data_size_;

   private:
    CtrlSignals ctrlsigs_;
    unsigned inst_bit_;
    unsigned next_pc_;
    unsigned pc_;
    bool br_delay_slot_;
    unsigned br_delay_addr_;
    uint8_t rs_addr_;
    uint8_t rt_addr_;
    int op1_data_;
    int op2_data_;
    int alu_out_;
    int load_data_;
    int store_data_;
    uint8_t rd_addr_;
    bool int_overflow_;
};

#endif