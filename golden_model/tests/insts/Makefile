BASE_DIR = $(abspath ..)

MIPS_GCC 	= mipsel-linux-gnu-gcc
MIPS_LD 	= mipsel-linux-gnu-ld
MIPS_OBJDUMP = mipsel-linux-gnu-objdump
MIPS_OBJCOPY = mipsel-linux-gnu-objcopy

MIPS_SFLAGS = -fno-builtin -mabi=32 -mno-long-calls -mno-mips16 -march=mips32 -fno-reorder-blocks -fno-reorder-functions -EL -c
MIPS_LDFLAGS = -T $(BASE_DIR)/insts/mips32.ld

MIPS32_ASM_S 			= $(wildcard $(BASE_DIR)/insts/*.S)
MIPS32_ASM_DUMPS 	= $(patsubst %.S, %.dump, $(MIPS32_ASM_S))
MIPS32_ASM_HEXS 		= $(patsubst %.S, %.hex, $(MIPS32_ASM_S))

all: $(MIPS32_ASM_DUMPS) $(MIPS32_ASM_HEXS)

%.o: %.S
	$(MIPS_GCC) $(MIPS_SFLAGS) $< -o $*.o

%.elf: %.o
	$(MIPS_LD) $(MIPS_LDFLAGS) $< -o $*.elf

%.dump: %.elf
	$(MIPS_OBJDUMP) --disassemble-all --disassemble-zeroes --section=.text --section=.text.startup --section=.data $< > $@

%.bin: %.elf
	$(MIPS_OBJCOPY) -O binary -j .text $< $*.bin 

%.hex: %.bin
	od -An -tx1 -w4 -v $< > $@

clean:
	rm -rf $(MIPS32_ASM_DUMPS) $(MIPS32_ASM_HEXS)

.PHONY: clean all