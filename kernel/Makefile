all: Kernel32bit Kernel64bit

Kernel32bit: kernel32 
	make -C kernel32

Kernel64bit: kernel64
	make -C kernel64

clean:
	make -C kernel64 clean 
	make -C kernel32 clean
