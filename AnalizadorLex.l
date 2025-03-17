%{ /* Proyecto Analizador Léxico Compiladores
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
CEnt	[+\-]?(0|{Dig})+("p"|"P")?
CRel	{Dig}+("'")({Dig}|0)*("r"|"R")?
OpAri   ("sum"|"rest"|"mult"|"div"|"mod"|"inc"|"dec"|"exp"|"dive")
OpRel	({Ident}|{CEnt}|{CRel})("h"|"m"|"e"|"c"|"he"|"me")({Ident}|{CEnt}|{CRel})
OpAsi   {Ident}("->"|"+->"|"-->"|"*->"|"/->"|"%->"|">>"|"<<"|"^->"|"&->")

%%
{Ident}		printf("%s es un identificador\n", yytext);
{CCad}		printf("%s es una cadena\n", yytext);
{CEnt}		printf("%s es una constante entera\n", yytext);
{CRel}		printf("%s es una constante real\n", yytext);

%%

int main ()
{
	yylex();
}

