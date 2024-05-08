BASE_DIR = $(abspath ..)
ROOT_DIR = $(BASE_DIR)/..

GOLDEN_MODEL_TESTS_DIR = $(BASE_DIR)/tests
GOLDEN_MODEL_OBJS_DIR = $(ROOT_DIR)/build
INCLUDES = $(BASE_DIR)/include

GOLDEN_MODEL_TEST_SRCS = run_gdmodel_tests.cpp

GOLDEN_MODEL_TESTS_TARGET = $(GOLDEN_MODEL_OBJS_DIR)/run_gdmodel_tests

CC = /usr/bin/g++
CCFLAGS = -g -std=c++11 -Werror -I$(INCLUDES) -lmips32_cmodel -L$(GOLDEN_MODEL_OBJS_DIR) -Wl,-rpath,$(GOLDEN_MODEL_OBJS_DIR)

all: $(GOLDEN_MODEL_TESTS_TARGET)
	make -C insts

$(GOLDEN_MODEL_TESTS_TARGET): $(GOLDEN_MODEL_TEST_SRCS)
	$(CC) $^ $(CCFLAGS) -o $@ 

clean:
	rm -rf $(GOLDEN_MODEL_TESTS_TARGET)
	make clean -C insts

.PHONY: clean all