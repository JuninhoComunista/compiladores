etapa1: lex.yy.c
		gcc -o etapa1 hash.c lex.yy.c
lex.yy.c: scanner.l
		lex scanner.l

clean:	
		rm lex.yy.c etapa1 hash.o
