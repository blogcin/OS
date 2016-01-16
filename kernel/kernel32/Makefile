all: prepare Kernel32.bin

NASM32 = /usr/bin/nasm
GCC32 = /usr/bin/gcc -c -m32 -ffreestanding
LD32 = /usr/bin/ld -melf_i386 -T ../elf_i386.x -nostdlib -e main -Ttext 0x10200
OBJCOPY32 = objcopy -j .text -j .data -j .rodata -j .bss -S -O binary

OBJDIR = temp
SRCDIR = SRC

prepare:
	mkdir -p $(OBJDIR)

$(OBJDIR)/EntryPoint.bin: $(SRCDIR)/EntryPoint.s
	$(NASM32) -o $@ $<

dep:
	make -C $(OBJDIR) -f ../Makefile InternalDependency

ExecuteInternalBuild: dep	
	make -C $(OBJDIR) -f ../Makefile Kernel32.elf

$(OBJDIR)/Kernel32.elf.bin: ExecuteInternalBuild
	$(OBJCOPY32) $(OBJDIR)/Kernel32.elf $@

Kernel32.bin: $(OBJDIR)/EntryPoint.bin $(OBJDIR)/Kernel32.elf.bin
	cat $^ > $@

clean:
	rm -rf $(OBJDIR)
	rm -f *.bin

ENTRYPOINT = main.o
CSRCS = $(wildcard ../$(SRCDIR)/*.c)
ASMSRCS = $(wildcard ../$(SRCDIR)/*.asm)
COBJ = $(subst main.o, ,$(notdir $(patsubst %.c,%.o,$(CSRCS))))
ASMOBJ = $(notdir $(patsubst %.asm,%.o,$(ASMSRCS)))

%.o: ../$(SRCDIR)/%.c
	$(GCC32) -c $<

%.o: ../$(SRCDIR)/%.asm
	$(NASM32) -f elf32 -o $@ $<

InternalDependency:
	$(GCC32) -MM $(CSRCS) > Dependency.dep

Kernel32.elf: $(ENTRYPOINT) $(COBJ) $(ASMOBJ)
	$(LD32) -o $@ $^

ifeq (Dependency.dep, $(wildcard Dependency.dep))
include Dependency.dep
endif
