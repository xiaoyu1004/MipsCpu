all:
	make -C golden_model
	make -C sim

clean:
	make clean -C golden_model

.PHONY: all clean