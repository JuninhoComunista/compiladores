etapa5: lex.yy.o main.o hash.o ast.o tac.o semantic.o decompiler.o y.tab
	gcc -Wall -o etapa5 -g y.tab.c lex.yy.o main.o hash.o ast.o tac.o decompiler.o semantic.o
main.o: main.c
	gcc -Wall -c -g main.c
decompiler.o: decompiler.c
	gcc -Wall -c -g decompiler.c
semantic.o: semantic.c
	gcc -Wall -c -g semantic.c
tac.o: tac.c
	gcc -Wall -c -g tac.c
ast.o: ast.c
	gcc -Wall -c -g ast.c
hash.o: hash.c
	gcc -Wall -c -g hash.c
lex.yy.o: lex.yy.c
	gcc -c -g lex.yy.c
lex.yy.c: scanner.l y.tab
	flex --header-file=lex.yy.h scanner.l 
y.tab: parser.y
	yacc -d parser.y

clean:
	rm lex.yy.* y.tab.* *.o etapa5 output.txt tree.txt etapa5.tgz