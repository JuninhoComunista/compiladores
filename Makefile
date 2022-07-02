etapa1: lex.yy.o main.o hash.o
	gcc -o etapa1 lex.yy.o main.o hash.o
main.o: main.c
	gcc -c -g main.c
hash.o: hash.c
	gcc -c -g hash.c
lex.yy.o: lex.yy.c
	gcc -c -g lex.yy.c
lex.yy.c: scanner.l
	flex --header-file=lex.yy.h scanner.l 
clean:
	rm *.o *.d lex.yy.* etapa1