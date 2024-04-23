#include "cpu.h"

int MipsCpu::run()
{
    inst_fetch();
    inst_decode();
    inst_execute();
    inst_mem();
    inst_wb();
}

void MipsCpu::inst_fetch() {}
void MipsCpu::inst_decode() {}
void MipsCpu::inst_execute() {}
void MipsCpu::inst_mem() {}
void MipsCpu::inst_wb() {}