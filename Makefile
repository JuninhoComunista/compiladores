etapa2: lex.yy.o main.o hash.o y.tab
	gcc -Wall -o etapa2 y.tab.c lex.yy.o main.o hash.o 
main.o: main.c
	gcc -c main.c
hash.o: hash.c
	gcc -c hash.c
lex.yy.o: lex.yy.c
	gcc -c lex.yy.c
lex.yy.c: scanner.l y.tab
	flex --header-file=lex.yy.h scanner.l 
y.tab: parser.y
	yacc -d parser.y

clean:
	rm *.o *.d lex.yy.* y.tab.* etapa2