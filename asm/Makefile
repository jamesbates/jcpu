asm: asm.tab.o asm.yy.o parsenode.o asmerror.o directive.o symbols.o instruction.o
	$(CC) -o $@ asm.tab.o asm.yy.o parsenode.o asmerror.o directive.o symbols.o instruction.o

.c.o:
	$(CC) -c -O2 -o $@ $<

asm.yy.c: asm.l asm.tab.c
	flex -o $@ asm.l

asm.tab.c: asm.y
	bison -d asm.y

clean:
	rm -f asm asm.yy.o asm.yy.c asm.tab.o asm.tab.c asm.tab.h parsenode.o asmerror.o directive.o symbols.o instruction.o

.PHONY: clean

