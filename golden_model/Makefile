BASE_DIR = $(abspath .)
ROOT_DIR = $(BASE_DIR)/..

GOLDEN_MODEL_SRC_DIR = $(BASE_DIR)/src
GOLDEN_MODEL_OBJS_DIR = $(ROOT_DIR)/build
INCLUDES = $(BASE_DIR)/include

GOLDEN_MODEL_SRCS = $(wildcard $(GOLDEN_MODEL_SRC_DIR)/*.cpp)
GOLDEN_MODEL_TARGET = $(GOLDEN_MODEL_OBJS_DIR)/run_gdmodel

all: $(GOLDEN_MODEL_TARGET)

CC = /usr/bin/g++
CCFLAGS = -g -std=c++17 -Werror -I$(INCLUDES)

$(GOLDEN_MODEL_TARGET): $(GOLDEN_MODEL_SRCS)
	$(CC) $(CCFLAGS) -o $@ $^

clean:
	rm -rf $(GOLDEN_MODEL_TARGET)

.PHONY: print clean all