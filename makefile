.PHONY: run

run:
	win_flex AnalizadorLex.l
	gcc lex.yy.c lista.c nodo.c token.c registro_tabla.c tabla.c -L lista.h -L token.h -o programa
	./programa entrada.txt