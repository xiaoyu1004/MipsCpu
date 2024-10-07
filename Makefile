base_dir   = $(abspath .)

all:
	make -C golden_model
	make -C rtlsim
	make -C tools
	make -C tests

test:
	make test -C tests

clean:
	make clean -C golden_model
	make clean -C tools
	make clean -C tests

.PHONY: all clean