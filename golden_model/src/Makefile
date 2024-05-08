BASE_DIR = $(abspath ..)
ROOT_DIR = $(BASE_DIR)/..

GOLDEN_MODEL_SRC_DIR = $(BASE_DIR)/src
GOLDEN_MODEL_OBJS_DIR = $(ROOT_DIR)/build
INCLUDES = $(BASE_DIR)/include

GOLDEN_MODEL_SRCS = cpu.cpp inst.cpp ctrl.cpp
GOLDEN_MODEL_OBJS = $(foreach f, $(GOLDEN_MODEL_SRCS), $(patsubst %.cpp, $(GOLDEN_MODEL_OBJS_DIR)/%.o, $f))

GOLDEN_MODEL_TARGET = $(GOLDEN_MODEL_OBJS_DIR)/libmips32_cmodel.so

all: $(GOLDEN_MODEL_TARGET)

CC = /usr/bin/g++
CCFLAGS = -g -std=c++17 -Werror -fPIC -shared -I$(INCLUDES)

$(GOLDEN_MODEL_OBJS_DIR)/%.o: %.cpp
	$(CC) $(CCFLAGS) -c -o $@ $<

$(GOLDEN_MODEL_TARGET): $(GOLDEN_MODEL_OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

clean:
	rm -rf $(GOLDEN_MODEL_OBJS) $(GOLDEN_MODEL_TARGET)

.PHONY: print clean all