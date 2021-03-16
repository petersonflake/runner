testing: scripts.tab.c lex.scr.c main.c
	gcc -o testing scripts.tab.c lex.scr.c main.c string_stack.c argstack.c command.c

scripts.tab.c: scripts.y
	bison -d -Wcounterexamples scripts.y

lex.scr.c: scripts.l
	flex scripts.l

.PHONY: clean
clean:
	rm -f testing *.tab.* lex.*.c
