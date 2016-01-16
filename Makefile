SRCS = boot kernels util

all: $(SRCS) mvfiles	

boot: bootloader
	make -C bootloader

kernels: kernel
	make -C kernel
	
mvfiles: bootloader kernel 
	./imagemaker bootloader/bootloader.bin kernel/kernel32/Kernel32.bin kernel/kernel64/Kernel64.bin 
	chmod 777 Disk.img

util: utility
	make -C utility
	cp utility/imagemaker .

clean: 
	rm Disk.img
	make -C utility clean
	rm imagemaker
	make -C bootloader clean
	make -C kernel clean
