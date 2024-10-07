BASE_DIR = $(abspath .)
ROOT_DIR = $(BASE_DIR)/..

TOOLS_OBJS_DIR = $(ROOT_DIR)/build

TOOLS_SRCS = $(wildcard $(BASE_DIR)/*.cpp)
TOOLS_TARGET = $(TOOLS_OBJS_DIR)/bin2coe

all: $(TOOLS_TARGET)

CC = /usr/bin/g++
CCFLAGS = -std=c++17 -Werror

$(TOOLS_TARGET): $(TOOLS_SRCS)
	$(CC) $(CCFLAGS) -o $@ $^

clean:
	rm -rf $(TOOLS_TARGET)

.PHONY: clean all