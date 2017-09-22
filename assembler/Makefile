OBJS = assembler.tab.o assembler.yy.o parsenode.o asmerror.o directive.o symbols.o instruction.o
assembler: $(OBJS)
	$(CC) -o $@ $(OBJS)

.c.o:
	$(CC) -c -O2 -o $@ $<

assembler.yy.c: assembler.l assembler.tab.c
	flex -o $@ assembler.l

assembler.tab.c: assembler.y
	bison -d assembler.y

clean:
	rm -f assembler assembler.yy.c assembler.tab.c assembler.tab.h $(OBJS)

.PHONY: clean

