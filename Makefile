testing: scripts.tab.c lex.scr.c
	gcc -o testing scripts.tab.c lex.scr.c

scripts.tab.c: scripts.y
	bison -d scripts.y

lex.scr.c: scripts.l
	flex scripts.l

.PHONY: clean
clean:
	rm -f testing *.tab.c lex.*.c
