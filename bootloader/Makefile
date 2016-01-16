SRCS =  main.s
NASM = /usr/bin/nasm

all: bootloader

bootloader: $(SRCS)
	$(NASM) $(SRCS) -o $@.bin
	
clean: bootloader.bin
	rm bootloader.bin
