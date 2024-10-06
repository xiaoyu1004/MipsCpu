base_dir   = $(abspath ..)
gen_dir    = $(base_dir)/generated-src
sim_dir    = $(base_dir)/rtlsim
src_dir    = $(base_dir)/soc
lib_dir		 = $(sim_dir)/lib
threads   ?= 2

CXXFLAGS += -std=c++14 -Wall -Wno-unused-variable

# compile verilator
FILES := $(wildcard $(src_dir)/*.v) $(wildcard $(src_dir)/**/*.v) $(wildcard $(lib_dir)/*.v)

VERILATOR = verilator --cc --exe
VERILATOR_FLAGS = --assert -Wno-STMTDLY -O3 -I$(src_dir)/include --trace --threads $(threads)\
	--top-module Top -Mdir $(gen_dir)/VTop.csrc \
	-CFLAGS "$(CXXFLAGS) -include $(gen_dir)/VTop.csrc/VTop.h"

$(gen_dir)/VTop: $(FILES) $(sim_dir)/sim_main.cpp
	$(VERILATOR) $(VERILATOR_FLAGS) -o $@ $^
	$(MAKE) -C $(gen_dir)/VTop.csrc -f VTop.mk

verilator: $(gen_dir)/VTop