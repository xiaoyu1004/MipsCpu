base_dir = $(abspath ..)

GOLDEN_MODEL_TESTS_DIR = $(base_dir)/tests
GOLDEN_MODEL_OBJS_DIR = $(base_dir)/objs
INCLUDES = $(base_dir)/include

GOLDEN_MODEL_TESTS = main.cpp
GOLDEN_MODEL_TESTS_OBJS = $(foreach f, $(GOLDEN_MODEL_TESTS), $(patsubst %.cpp, $(GOLDEN_MODEL_OBJS_DIR)/%.o, $f))

GOLDEN_MODEL_TESTS_TARGET = $(GOLDEN_MODEL_OBJS_DIR)/run_golden_model_tests
all: $(GOLDEN_MODEL_TESTS_TARGET)

CC = /usr/bin/g++
CCFLAGS = -std=c++11 -Werror -I$(INCLUDES)

$(GOLDEN_MODEL_OBJS_DIR)/%.o: %.cpp
	$(CC) $(CCFLAGS) -c -o $@ $<

$(GOLDEN_MODEL_TESTS_TARGET): $(GOLDEN_MODEL_TESTS_OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

# mips
MIPS_GCC = mipsel-linux-gnu-gcc
MIPS_CXXFLAGS = 

clean:
	rm -rf $(GOLDEN_MODEL_TESTS_OBJS)

.PHONY: clean all