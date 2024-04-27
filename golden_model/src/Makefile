base_dir = $(abspath ..)

GOLDEN_MODEL_SRC_DIR = $(base_dir)/src
GOLDEN_MODEL_OBJS_DIR = $(base_dir)/objs
INCLUDES = $(base_dir)/include

GOLDEN_MODEL_SRCS = cpu.cpp inst.cpp
GOLDEN_MODEL_OBJS = $(foreach f, $(GOLDEN_MODEL_SRCS), $(patsubst %.cpp, $(GOLDEN_MODEL_OBJS_DIR)/%.o, $f))

GOLDEN_MODEL_TARGET = $(GOLDEN_MODEL_OBJS_DIR)/libmips32_cmodel.so
all: $(GOLDEN_MODEL_TARGET)

CC = /usr/bin/g++
CCFLAGS = -std=c++11 -Werror -fPIC -shared -I$(INCLUDES)

$(GOLDEN_MODEL_OBJS_DIR)/%.o: %.cpp
	$(CC) $(CCFLAGS) -c -o $@ $<

$(GOLDEN_MODEL_TARGET): $(GOLDEN_MODEL_OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

print:
	echo $(wildcard $(GOLDEN_MODEL_OBJS_DIR))

clean:
	rm -rf $(GOLDEN_MODEL_OBJS)

.PHONY: print clean all