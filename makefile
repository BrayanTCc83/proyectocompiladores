.PHONY: run

run:
	win_flex AnalizadorLex.l
	gcc lex.yy.c lista.c nodo.c token.c registro_tabla.c tabla.c atomos.c -L lista.h -L token.h -L atomos.h -o programa -DDETAILS
	./programa entrada.txt