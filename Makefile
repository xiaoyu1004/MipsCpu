all:
	make -C golden_model

clean:
	make clean -C golden_model

.PHONY: all clean