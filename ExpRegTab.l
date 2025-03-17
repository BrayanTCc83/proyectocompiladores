%{ /* Proyecto Analizador Léxico Compiladores
/* Alumnos: Enzo Valdés Zavala
/* --
 * Objetivo: Elaborar un analizador léxico en lex/flex que reconozca los componentes léxicos
 * pertenecientes a las clases descritas y definidas en clase.
 * Este recibira un archivo de entrada, el cual se indica desde la linea de comandos al momento
 * de ejecución. Delimitador de un componente lexico será varios espacios, tabuladores o saltos
 * de linea, asi como el inicio de otro componente léxico. Considerar orden de las acciones en  * el programa flex.
*/
#include <stdio.h>
#include <stdlib.h>
int state;

%}

letM	[A-Z] 
letm	[a-z]
Dig	[1-9]
vocA	[áéíóú]
VocA	[ÁÉÍÓÚ]
PalRes	("Bool"|"Cade"|"Continuar"|"Devo"|"Ent"|"Fals"|"Flota"|"Global"|"Haz"|"Mientras"|"Nulo"|"Para"|"Parar"|"Si"|"Sino"|"Ver")
SimEsp	(<|>|<<|>>|#|#!|\*|"|"|º)
Ident	"@"({letM}|{letm}|{Dig}|0|{vocA}|{VocA}|"ñ"|"Ñ")+"_"
CCad	"--".+"--"
CEnt	[+\-]?(0|{Dig})+(("p"|"P")|("g"|"G"))?
CRel	[+\-]?{Dig}+("'")({Dig}|0)*("r"|"R")?
OpAri   ("sum"|"rest"|"mult"|"div"|"mod"|"inc"|"dec"|"exp"|"dive")
OpRel	({Ident}|{CEnt}|{CRel})("h"|"m"|"e"|"c"|"he"|"me")({Ident}|{CEnt}|{CRel})
OpAsi   {Ident}("->"|"+->"|"-->"|"*->"|"/->"|"%->"|">>"|"<<"|"^->"|"&->")

%%
{Ident}		printf("%s es un identificador\n", yytext);
{CCad}		printf("%s es una cadena\n", yytext);
{CEnt}		printf("%s es una constante entera\n", yytext);
{CRel}		printf("%s es una constante real\n", yytext);

%%

struct Tablas {
        char *Simbolos[20];
        int Pos[20];
};
int main () {
int i;
struct Tablas PalabrasReservadas;
for (i = 0; i < 16; i++){
        PalabrasReservadas.Pos[i] = i;
};
PalabrasReservadas.Simbolos[0] = "Bool";
PalabrasReservadas.Simbolos[1] = "Cade";
PalabrasReservadas.Simbolos[2] = "Continuar";
PalabrasReservadas.Simbolos[3] = "Devo";
PalabrasReservadas.Simbolos[4] = "Ent";
PalabrasReservadas.Simbolos[5] = "Fals";
PalabrasReservadas.Simbolos[6] = "Flota";
PalabrasReservadas.Simbolos[7] = "Global";
PalabrasReservadas.Simbolos[8] = "Haz";
PalabrasReservadas.Simbolos[9] = "Mientras";
PalabrasReservadas.Simbolos[10] = "Nulo";
PalabrasReservadas.Simbolos[11] = "Para";
PalabrasReservadas.Simbolos[12] = "Parar";
PalabrasReservadas.Simbolos[13] = "Si";
PalabrasReservadas.Simbolos[14] = "Sino";
PalabrasReservadas.Simbolos[15] = "Ver";

printf("Tabla Palabras Reservadas\n");
for (i = 0; i < 16; i ++) {
        printf("Pos = %d\t", PalabrasReservadas.Pos[i]);
        printf("Palabra = %s\n", PalabrasReservadas.Simbolos[i]);
};
struct Tablas SimboloEspecial;
for (i = 0; i < 9; i++) {
        SimboloEspecial.Pos[i] = i;
};
SimboloEspecial.Simbolos[0] = "<";
SimboloEspecial.Simbolos[1] = ">";
SimboloEspecial.Simbolos[2] = "<<";
SimboloEspecial.Simbolos[3] = ">>";
SimboloEspecial.Simbolos[4] = "#";
SimboloEspecial.Simbolos[5] = "#!";
SimboloEspecial.Simbolos[6] = "*";
SimboloEspecial.Simbolos[7] = "|";
SimboloEspecial.Simbolos[8] = "º";

printf("Tabla Simbolos Especiales\n");
for (i = 0; i < 9; i ++) {
        printf("Pos = %d\t", SimboloEspecial.Pos[i]);
        printf("Simbolo = %s\n", SimboloEspecial.Simbolos[i]);
};

struct Tablas OpAritmetico;
for (i = 0; i < 9; i++) {
        OpAritmetico.Pos[i] = i;
};
OpAritmetico.Simbolos[0] = "sum";
OpAritmetico.Simbolos[1] = "rest";
OpAritmetico.Simbolos[2] = "mult";
OpAritmetico.Simbolos[3] = "div";
OpAritmetico.Simbolos[4] = "mod";
OpAritmetico.Simbolos[5] = "inc";
OpAritmetico.Simbolos[6] = "dec";
OpAritmetico.Simbolos[7] = "exp";
OpAritmetico.Simbolos[8] = "dive";

printf("Tabla Operadores Aritmeticos\n");
for (i = 0; i < 9; i ++) {
        printf("Pos = %d\t", OpAritmetico.Pos[i]);
        printf("Simbolo = %s\n", OpAritmetico.Simbolos[i]);
};
struct Tablas OpAsignacion;
for (i = 0; i < 9; i++) {
        OpAsignacion.Pos[i] = i;
};
OpAsignacion.Simbolos[0] = "->";
OpAsignacion.Simbolos[1] = "+->";
OpAsignacion.Simbolos[2] = "-->";
OpAsignacion.Simbolos[3] = "*->";
OpAsignacion.Simbolos[4] = "/->";
OpAsignacion.Simbolos[5] = "%->";
OpAsignacion.Simbolos[6] = ">>";
OpAsignacion.Simbolos[7] = "<<";
OpAsignacion.Simbolos[8] = "^->";
OpAsignacion.Simbolos[9] = "&->";

printf("Tabla Operadores Asignacion\n");
for (i = 0; i < 10; i ++) {
        printf("Pos = %d\t", OpAsignacion.Pos[i]);
        printf("Simbolo = %s\n", OpAsignacion.Simbolos[i]);
};

struct Tablas OpRelacional;
for (i = 0; i < 6; i++) {
        OpRelacional.Pos[i] = i;
};
OpRelacional.Simbolos[0] = "h";
OpRelacional.Simbolos[1] = "m";
OpRelacional.Simbolos[2] = "e";
OpRelacional.Simbolos[3] = "c";
OpRelacional.Simbolos[4] = "he";
OpRelacional.Simbolos[5] = "me";

printf("Tabla Operadores Relacionales\n");
for (i = 0; i < 6; i ++) {
        printf("Pos = %d\t", OpRelacional.Pos[i]);
        printf("Simbolo = %s\n", OpRelacional.Simbolos[i]);
};
yylex();
}
