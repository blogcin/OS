SRCS = boot kernels util

all: $(SRCS) mvfiles	

boot: bootloader
	make -C bootloader

kernels: kernel
	make -C kernel
	
mvfiles: bootloader kernel 
	cat bootloader/bootloader.bin kernel/kernel32/Kernel32.bin > build.bin

util: utility
	make -C utility
	cp utility/imagemaker .
clean: 
	make -C utility
	rm imagemaker
	make -C bootloader clean
	make -C kernel clean
	rm build.bin
