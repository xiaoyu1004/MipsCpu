BASE_DIR = $(abspath .)

MIPS_GCC 	= mipsel-linux-gnu-gcc
MIPS_LD 	= mipsel-linux-gnu-ld
MIPS_OBJDUMP = mipsel-linux-gnu-objdump
MIPS_OBJCOPY = mipsel-linux-gnu-objcopy

INC_DIR		 = $(BASE_DIR)/include
MIPS_SFLAGS  = -fno-pic -mno-abicalls -nostdinc -nostdlib -fno-reorder-blocks -fno-reorder-functions -EL -fno-builtin -c -I$(INC_DIR)
MIPS_LDFLAGS = -T $(BASE_DIR)/link32.ld

TEST_DIRS 			    := $(shell find $(BASE_DIR) -maxdepth 1 -type d)
MIPS32_ASM_S 			= $(foreach d, $(TEST_DIRS), $(wildcard $d/*.S))
MIPS32_ASM_DUMPS 		= $(foreach f, $(MIPS32_ASM_S), $(patsubst %.S, %.dump, $f))
MIPS32_ASM_INST 		= $(foreach f, $(MIPS32_ASM_S), $(patsubst %.S, %.inst, $f))
MIPS32_ASM_DATA 		= $(foreach f, $(MIPS32_ASM_S), $(patsubst %.S, %.data, $f))

GOLDEN_TRACE   			= $(foreach f, $(MIPS32_ASM_S), $(patsubst %.S, %.trace, $f))
VERIFY_TRACE   			= $(foreach f, $(MIPS32_ASM_S), $(patsubst %.S, %.verify_trace, $f))
RUN_GOLDEN				= $(BASE_DIR)/../build/run_gdmodel
RUN_RTLSIM_VERIFY	    = $(BASE_DIR)/../generated-src/VTop

all: $(MIPS32_ASM_DUMPS) $(MIPS32_ASM_INST) $(MIPS32_ASM_DATA)

test: $(GOLDEN_TRACE) $(VERIFY_TRACE)

%.o: %.S
	$(MIPS_GCC) $(MIPS_SFLAGS) $< -o $@

%.elf: %.o
	$(MIPS_LD) $(MIPS_LDFLAGS) $< -o $@

%.dump: %.elf
	$(MIPS_OBJDUMP) --disassemble-all --disassemble-zeroes --section=.text --section=.text.startup --section=.data $< > $@

%.inst_bin: %.elf
	$(MIPS_OBJCOPY) -O binary -j .text $< $@

%.inst: %.inst_bin
	od -An -tx1 -w4 -v $< > $@

%.data_bin: %.elf
	$(MIPS_OBJCOPY) -O binary -j .data $< $@

%.data: %.data_bin
	od -An -tx1 -w4 -v $< > $@

%.trace: %.inst %.data
	$(RUN_GOLDEN) $^

%.verify_trace: %.inst %.data
	$(RUN_RTLSIM_VERIFY) $^ > $@

clean:
	rm -rf $(MIPS32_ASM_DUMPS) $(MIPS32_ASM_INST) $(MIPS32_ASM_DATA) $(GOLDEN_TRACE) $(VERIFY_TRACE)

.PHONY: clean all