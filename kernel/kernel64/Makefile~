all: prepare Kernel64.bin

NASM64 = /usr/bin/nasm -f elf64
GCC64 = /usr/bin/gcc -c -m64 -ffreestanding
LD64 = /usr/bin/ld -melf_x86_64 -T ../elf_x86_64.x -nostdlib -e main -Ttext 0x200000
OBJCOPY64 = objcopy -j .text -j .data -j .rodata -j .bss -S -O binary

OBJDIR = temp
SRCDIR = SRC

prepare:
	mkdir -p $(OBJDIR)

dep:
	make -C $(OBJDIR) -f ../Makefile InternalDependency

ExecuteInternalBuild: dep	
	make -C $(OBJDIR) -f ../Makefile Kernel64.elf

Kernel64.bin: ExecuteInternalBuild
	$(OBJCOPY64) $(OBJDIR)/Kernel64.elf $@

clean:
	rm -rf $(OBJDIR)
	rm -f *.bin

ENTRYPOINTSRC = ../$(SRCDIR)/EntryPoint.s
ENTRYPOINTOBJ = EntryPoint.o

CSRCS = $(wildcard ../$(SRCDIR)/*.c)
ASMSRCS = $(wildcard ../$(SRCDIR)/*.asm)
COBJ = $(notdir $(patsubst %.c,%.o,$(CSRCS)))
ASMOBJ = $(notdir $(patsubst %.asm,%.o,$(ASMSRCS)))

$(ENTRYPOINTOBJ): $(ENTRYPOINTSRC)
	$(NASM64) -o $@ $<

%.o: ../$(SRCDIR)/%.c
	$(GCC64) -c $<

%.o: ../$(SRCDIR)/%.asm
	$(NASM64) -o $@ $<

InternalDependency:
	$(GCC64) -MM $(CSRCS) > Dependency.dep

Kernel64.elf: $(ENTRYPOINTOBJ) $(COBJ) $(ASMOBJ)
	$(LD64) -o $@ $^

ifeq (Dependency.dep, $(wildcard Dependency.dep))
include Dependency.dep
endif
