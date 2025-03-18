%{
    /* Proyecto Analizador Léxico Compiladores
    /* Alumnos: Enzo Valdés Zavala
    /* Téllez Cruz Brayan
    * Objetivo: Elaborar un analizador léxico en lex/flex que reconozca los componentes léxicos
    * pertenecientes a las clases descritas y definidas en clase.
    * Este recibira un archivo de entrada, el cual se indica desde la linea de comandos al momento
    * de ejecución. Delimitador de un componente lexico será varios espacios, tabuladores o saltos
    * de linea, asi como el inicio de otro componente léxico. Considerar orden de las acciones en 
    * el programa flex.
    */
    #include <stdio.h>
    #include <stdlib.h>
    #include "token.h"

    void yyerror(const char*);

    /*
    * Creamos las tablas sin inicializar.
    */
    Tabla *reservadas, *especiales, *identificadores, *aritmeticos, *relacionales, *asignacion, *cadenas, *enteros, *reales;
    Tabla *tokens;

    FILE *salida;

    int state;
%}

%option yylineno

/* Definimos los componentes que usaremos */
letM	[A-Z] 
letm	[a-z]
Dig	    [1-9]
vocA	[áéíóú]
VocA	[ÁÉÍÓÚ]
PalRes	("Bool"|"Cade"|"Continuar"|"Devo"|"Ent"|"Fals"|"Flota"|"Global"|"Haz"|"Mientras"|"Nulo"|"Para"|"Parar"|"Si"|"Sino"|"Ver")
SimEsp	(<|>|<<|>>|#|#!|\*|"|"|"°")
Ident	"@"({letM}|{letm}|{Dig}|0|{vocA}|{VocA}|"ñ"|"Ñ")+"_"
CCad	"--".+"--"
CEnt	[+\-]?(0|{Dig})+("p"|"P"|"g"|"G")?
CRel	({Dig}*("'")({Dig}|0)+)|({CEnt}("r"|"R"))
OpAri   ("sum"|"rest"|"mult"|"div"|"mod"|"inc"|"dec"|"exp"|"dive")
OpRel	("h"|"m"|"e"|"c"|"he"|"me")
OpAsi   ("->"|"+->"|"-->"|"*->"|"/->"|"%->"|">>->"|"<<->"|"^->"|"&->")
Comen   \[.*\]

%%
{Ident}		{
    printf("Un identificador: %s\n", yytext);
    tabla_insertar(tokens, nuevo_token(2,
        tabla_insertar(identificadores, nuevo_identificador(yytext, SIN_TIPO))));
}
{CCad}		{
    printf("Una cadena: %s\n", yytext);
    tabla_insertar(tokens, nuevo_token(6,
        tabla_insertar(cadenas, nueva_cadena(yytext))));
}
{CRel}		{
    printf("Una constante real: %s\n", yytext);
    tabla_insertar(tokens, nuevo_token(7,
        tabla_insertar(reales, nuevo_real(yytext))));
}
{CEnt}		{
    printf("Una constante entera: %s\n", yytext);
    tabla_insertar(tokens, nuevo_token(8,
        tabla_insertar(enteros, nuevo_entero(yytext))));
}
{PalRes}    {
    printf("Una palabra reservada: %s\n", yytext);
    tabla_insertar(tokens, nuevo_token(0, tabla_buscar(*reservadas, yytext)));
}
{SimEsp}    {
    printf("Un simbolo especial: %s\n", yytext);
    tabla_insertar(tokens, nuevo_token(0, tabla_buscar(*especiales, yytext)));
}
{OpAri}     {
    printf("Un operador aritmetico: %s\n", yytext);
    tabla_insertar(tokens, nuevo_token(0, tabla_buscar(*aritmeticos, yytext)));
}
{OpRel}     {
    printf("Un operador relacional: %s\n", yytext);
    tabla_insertar(tokens, nuevo_token(0, tabla_buscar(*relacionales, yytext)));
}
{OpAsi}     {
    printf("Un operador de asignacion: %s\n", yytext);
    tabla_insertar(tokens, nuevo_token(0, tabla_buscar(*asignacion, yytext)));
}
{Comen}     {
    printf("Un comentario: %s\n", yytext);
}
(\n|\t|\s)  {}
.           { yyerror(yytext); }
%%

void yyerror(const char *error) {
    fprintf(salida, "[Error Linea %d]: %s\n", yylineno, error);
    fprintf(stdout, "[Error Linea %d]: %s\n", yylineno, error);
}

int yywrap() {
    return 1;
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        perror("Es necesario proveer al menos un archivo.");
        exit(EXIT_FAILURE);
    }

    // Tablas estáticas
    /*
        Definimos cada una de nuestras tablas estáticas, rellenandolas de
        manera automática.
    */
    reservadas = nueva_tabla("Reservada", RESERVADA);
    tabla_llenar(reservadas, 16, "Bool", "Cade", "Continuar", "Devo", "Ent", "Fals", "Flota", "Global", "Haz", "Mientras", "Nulo", "Para", "Parar", "Si", "Sino", "Ver");
    especiales = nueva_tabla("Simbolo especial", SIMBOLO_ESPECIAL);
    tabla_llenar(especiales, 9, "<", ">", "<<", ">>", "#", "#!", "*", "|", "°");
    aritmeticos = nueva_tabla("Aritmetico", ARITMETICO);
    tabla_llenar(aritmeticos, 9, "sum", "rest", "mult", "div", "mod", "inc", "dec", "exp", "dive");
    relacionales = nueva_tabla("Relacional", RELACIONAL);
    tabla_llenar(relacionales, 6, "h", "m", "e", "c", "he", "me");
    asignacion = nueva_tabla("Asignacion", ASIGNACION);
    tabla_llenar(asignacion, 10, "->", "+->", "-->", "*->", "/->", "%->", ">>->", "<<->", "^->", "&->");
    // Tablas dinámicas
    identificadores = nueva_tabla("Simbolos", IDENTIFICADOR);
    cadenas = nueva_tabla("Literales cadenas", LITERAL_CADENA);
    enteros = nueva_tabla("Literales enteras", LITERAL_ENTERA);
    reales = nueva_tabla("Literales reales", LITERAL_REAL);
    tokens = nueva_tabla("Tokens", TOKEN);

    int archivo = 1;
    yyin = fopen(argv[archivo], "r");
    salida = fopen("salida.txt", "w");

	yylex();

    tabla_imprimir(stdout, *reservadas);
    tabla_imprimir(stdout, *especiales);
    tabla_imprimir(stdout, *identificadores);
    tabla_imprimir(stdout, *aritmeticos);
    tabla_imprimir(stdout, *relacionales);
    tabla_imprimir(stdout, *asignacion);
    tabla_imprimir(stdout, *cadenas);
    tabla_imprimir(stdout, *enteros);
    tabla_imprimir(stdout, *reales);
    tabla_imprimir(stdout, *tokens);
    
    tabla_imprimir(salida, *reservadas);
    tabla_imprimir(salida, *especiales);
    tabla_imprimir(salida, *identificadores);
    tabla_imprimir(salida, *aritmeticos);
    tabla_imprimir(salida, *relacionales);
    tabla_imprimir(salida, *asignacion);
    tabla_imprimir(salida, *cadenas);
    tabla_imprimir(salida, *enteros);
    tabla_imprimir(salida, *reales);
    tabla_imprimir(salida, *tokens);
    return EXIT_SUCCESS;
}
