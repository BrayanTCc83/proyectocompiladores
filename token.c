#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int *nuevo_entero(const char *enteroCadena) {
    int *nuevoEntero = (int*) malloc( sizeof(int) );
    char *offset = NULL;

    char *c = nueva_cadena(enteroCadena);
    if((offset = strstr(c, "p")) != NULL || (offset = strstr(c, "P")) != NULL
        || (offset = strstr(c, "g")) != NULL || (offset = strstr(c, "G")) != NULL) {
        c[offset - c] = '\0';
    }
    (*nuevoEntero) = atoi(c);

    return nuevoEntero;
}

float *nuevo_real(const char *realCadena) {
    float *nuevoReal = (float*) malloc( sizeof(float) );
    char *offset = NULL;

    char *c = nueva_cadena(realCadena);
    if((offset = strstr(c, "'")) != NULL) {
        c[offset - c] = '.';
    }
    if((offset = strstr(c, "r")) != NULL || (offset = strstr(c, "R")) != NULL) {
        c[offset - c] = '\0';
    }
    *nuevoReal = atof(c);

    return nuevoReal;
}

char *nueva_cadena(const char *cadena) {
    char *nuevaCadena = (char*) malloc( strlen(cadena) * sizeof(char) + 1 );

    strcpy(nuevaCadena, cadena);

    return nuevaCadena;
}

Identificador *nuevo_identificador(const char *identificador, TipoDato tipo) {
    Identificador *nuevoIdentificador = (Identificador*) malloc( sizeof(Identificador) );

    nuevoIdentificador->identificador = nueva_cadena(identificador);
    nuevoIdentificador->tipo = tipo;

    return nuevoIdentificador;
}

void valor_tabla_eliminar(ValorTabla *valorTabla, TablaTipo tipo) {
    switch (tipo) {
        case LITERAL_CADENA:
            strcpy(valorTabla->v_cadena, "");
            break;
        case LITERAL_ENTERA:
            *(valorTabla->v_entera) = 0;
            break;
        case LITERAL_REAL:  
            *(valorTabla->v_real) = 0.0;
            break;
        case IDENTIFICADOR:
            valorTabla->v_identificador->tipo = 0;
            strcpy(valorTabla->v_identificador->identificador, "");
            free(valorTabla->v_identificador->identificador);
            valorTabla->v_identificador->identificador = NULL;
            break;
        default:
            perror("Tipo no soportado.");
    }
    free(valorTabla->v_cadena);
    valorTabla->v_cadena = NULL;

    free(valorTabla);
}

Token *nuevo_token(int clase, int posicion) {
    Token *nuevoToken = (Token*) malloc( sizeof(Token) );

    nuevoToken->clase = clase;
    nuevoToken->posicion = posicion;

    return nuevoToken;
}

void token_eliminar(Token *token) {
    token->clase = 0;
    token->posicion = 0;
    free(token);
}

Estatico *nuevo_estatico(const char *valor, const char *atomo) {
    Estatico *nuevoEstatico = (Estatico*) malloc( sizeof(Estatico) );

    nuevoEstatico->atomo = nueva_cadena(atomo);
    nuevoEstatico->valor = nueva_cadena(valor);

    return nuevoEstatico;
}

void estatico_eliminar(Estatico *estatico) {
    estatico->atomo = 0;
    
    strcpy(estatico->valor, "");
    free(estatico->valor);
    estatico->valor = NULL;

    free(estatico);
}