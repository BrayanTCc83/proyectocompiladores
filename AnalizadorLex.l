%{
    /* Proyecto Analizador Léxico Compiladores
    /* Alumnos: Enzo Valdés Zavala
    /* --
    * Objetivo: Elaborar un analizador léxico en lex/flex que reconozca los componentes léxicos
    * pertenecientes a las clases descritas y definidas en clase.
    * Este recibira un archivo de entrada, el cual se indica desde la linea de comandos al momento
    * de ejecución. Delimitador de un componente lexico será varios espacios, tabuladores o saltos
    * de linea, asi como el inicio de otro componente léxico. Considerar orden de las acciones en 
    * el programa flex.
    */
    #include <stdio.h>
    #include <stdlib.h>
    #include <locale.h>
    #include "token.h"
    #include "atomos.h"

    void yyerror(const char*);

    /*
    * Creamos las tablas sin inicializar.
    */
    Tabla *reservadas, *especiales, *identificadores, *aritmeticos, *relacionales, *asignacion, *cadenas, *enteros, *reales;
    Tabla *tokens;

    /*
    * Cadena de atomos
    */
    CadenaAtomos *cadena;

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
    #ifdef DETAILS
    printf("Un identificador: %s\n", yytext);
    #endif
    tabla_insertar(tokens, nuevo_token(IDENTIFICADOR,
        tabla_insertar(identificadores, nuevo_identificador(yytext, SIN_TIPO))));
    cadena_atomos_insertar(cadena, L"a");
}
{CCad}		{
    #ifdef DETAILS
    printf("Una cadena: %s\n", yytext);
    #endif
    tabla_insertar(tokens, nuevo_token(LITERAL_CADENA,
        tabla_insertar(cadenas, nueva_cadena(yytext))));
    cadena_atomos_insertar(cadena, L"s");
}
{CRel}		{
    #ifdef DETAILS
    printf("Una constante real: %s\n", yytext);
    #endif
    tabla_insertar(tokens, nuevo_token(LITERAL_REAL,
        tabla_insertar(reales, nuevo_real(yytext))));
    cadena_atomos_insertar(cadena, L"r");
}
{CEnt}		{
    #ifdef DETAILS
    printf("Una constante entera: %s\n", yytext);
    #endif
    tabla_insertar(tokens, nuevo_token(LITERAL_ENTERA,
        tabla_insertar(enteros, nuevo_entero(yytext))));
    cadena_atomos_insertar(cadena, L"e");
}
{PalRes}    {
    #ifdef DETAILS
    printf("Una palabra reservada: %s\n", yytext);
    #endif
    int i = tabla_buscar(*reservadas, yytext);
    tabla_insertar(tokens, nuevo_token(RESERVADA, i));
    cadena_atomos_insertar(cadena, (wchar_t*) tabla_obtener(*reservadas, i)->estatico.atomo);
}
{SimEsp}    {
    #ifdef DETAILS
    printf("Un simbolo especial: %s\n", yytext);
    #endif
    int i = tabla_buscar(*especiales, yytext);
    tabla_insertar(tokens, nuevo_token(SIMBOLO_ESPECIAL, i));
    cadena_atomos_insertar(cadena, (wchar_t*) tabla_obtener(*especiales, i)->estatico.atomo);
}
{OpAri}     {
    #ifdef DETAILS
    printf("Un operador aritmetico: %s\n", yytext);
    #endif
    int i = tabla_buscar(*aritmeticos, yytext);
    tabla_insertar(tokens, nuevo_token(ARITMETICO, i));
    cadena_atomos_insertar(cadena, (wchar_t*) tabla_obtener(*aritmeticos, i)->estatico.atomo);
}
{OpRel}     {
    #ifdef DETAILS
    printf("Un operador relacional: %s\n", yytext);
    #endif
    int i = tabla_buscar(*relacionales, yytext);
    tabla_insertar(tokens, nuevo_token(RELACIONAL, i));
    cadena_atomos_insertar(cadena, (wchar_t*) tabla_obtener(*relacionales, i)->estatico.atomo);
}
{OpAsi}     {
    #ifdef DETAILS
    printf("Un operador de asignacion: %s\n", yytext);
    #endif
    int i = tabla_buscar(*asignacion, yytext);
    tabla_insertar(tokens, nuevo_token(ASIGNACION, i));
    cadena_atomos_insertar(cadena, (wchar_t*) tabla_obtener(*asignacion, i)->estatico.atomo);
}
{Comen}     {
    #ifdef DETAILS
    printf("Un comentario: %s\n", yytext);
    #endif
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

void set_locale_spanish_latam() {
    const char* result = NULL;

#ifdef _WIN32
    result = setlocale(LC_ALL, "Spanish_Mexico.1252");
#else
    result = setlocale(LC_ALL, "es_MX.UTF-8");
#endif

    if (result == NULL) {
        // Fallback: usar la del sistema
        result = setlocale(LC_ALL, "");
    }

    if (result == NULL) {
        printf("No se pudo establecer la configuración regional.\n");
    } else {
        printf("Locale configurado como: %s\n", result);
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        perror("Es necesario proveer al menos un archivo.");
        exit(EXIT_FAILURE);
    }
    set_locale_spanish_latam();
    cadena = nueva_cadena_atomos();

    // Tablas estáticas
    /*
        Definimos cada una de nuestras tablas estáticas, rellenandolas de
        manera automática.
    */
    reservadas = nueva_tabla("Reservada", RESERVADA);
    tabla_llenar(reservadas, 16 * 2,
        "Bool", "b",
        "Cade", "c",
        "Continuar", "t",
        "Devo", "d",
        "Ent", "n",
        "Fals", "q",
        "Flota", "f",
        "Global", "g",
        "Haz", "h",
        "Mientras", "w",
        "Nulo", "u",
        "Para", "o",
        "Parar", "p",
        "Si", "i",
        "Sino", "l",
        "Ver", "m"
    );
    especiales = nueva_tabla("Simbolo especial", SIMBOLO_ESPECIAL);
    tabla_llenar(especiales, 9 * 2,
        "<", "(",
        ">", ")",
        "<<", "[",
        ">>", "]",
        "#", "{",
        "#!", "}",
        "*", ";",
        "|", ",",
        "\u00B0", "."
    );
    aritmeticos = nueva_tabla("Aritmetico", ARITMETICO);
    tabla_llenar(aritmeticos, 9 * 2,
        "sum", "+",
        "rest", "-",
        "mult", "*",
        "div", "/",
        "mod", "%",
        "inc", "z",
        "dec", "j",
        "exp", "k",
        "dive", "~"
    );
    relacionales = nueva_tabla("Relacional", RELACIONAL);
    tabla_llenar(relacionales, 6 * 2,
        "h", ">",
        "m", "<",
        "e", L"\u00A1",
        "c", "!",
        "he", "x",
        "me", "y"
    );
    asignacion = nueva_tabla("Asignacion", ASIGNACION);
    tabla_llenar(asignacion, 10 * 2,
        "->", "=",
        "+->", "#",
        "-->", L"\u00F1",
        "*->", "$",
        "/->", "v",
        "%->", "?",
        ">>->", ":",
        "<<->", "\"",
        "^->", "^",
        "&->", "&"
    );
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
