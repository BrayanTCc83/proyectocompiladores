.PHONY: run test

run:
	win_flex AnalizadorLex.l
	gcc lex.yy.c lista.c nodo.c token.c registro_tabla.c tabla.c atomos.c gramatica.c -L lista.h -L token.h -L atomos.h -L gramatica.h -o programa
	./programa ejemplo1.txt
	
test:
	win_flex AnalizadorLex.l
	gcc lex.yy.c lista.c nodo.c token.c registro_tabla.c tabla.c atomos.c gramatica.c -L lista.h -L token.h -L atomos.h -L gramatica.h -o programa -DDETAILS
	./programa ejemplo1.txt