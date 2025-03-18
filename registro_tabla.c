#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

RegistroTabla *nuevo_registro_tabla(int indice, TablaTipo tipo, Valor valor) {
    RegistroTabla *nuevoRegistro = (RegistroTabla*) malloc( sizeof(RegistroTabla) );

    nuevoRegistro->indice = indice;
    nuevoRegistro->tipo = tipo;
    nuevoRegistro->valor.v_identificador = valor;

    return nuevoRegistro;
}

bool registro_tabla_comparar(Valor registro, Valor valor) {
    switch (((RegistroTabla*)registro)->tipo) {
        case TOKEN:
            break;
        case LITERAL_CADENA:
            break;
        case LITERAL_ENTERA:
            break;
        case LITERAL_REAL:
            break;
        case IDENTIFICADOR:
            break;
        default:
            return strcmp(((RegistroTabla*)registro)->estatico, (char*)valor) == 0;
    }
    return true;
}

void registro_tabla_eliminar(RegistroTabla *registro, TablaTipo tipo) {
    if(tipo == TOKEN) {
        token_eliminar(registro->token);
    } else {
        valor_tabla_eliminar(&registro->valor, tipo);
    }
    registro->indice = 0;
    registro->token = NULL;
    free(registro);
}

char *registro_a_cadena(RegistroTabla *valor, TablaTipo tipo) {
    char *cadena = (char*) malloc( REGISTRO_CADENA_LONGITUD );

    switch (tipo) {
        case TOKEN:
            sprintf(cadena, "| %10d | %10d |", valor->token->clase, valor->token->posicion);
            break;
        case LITERAL_CADENA:
            sprintf(cadena, "| %10d | %50s |", valor->indice, valor->valor.v_cadena);
            break;
        case LITERAL_ENTERA:
            sprintf(cadena, "| %10d | %10d |", valor->indice, *(valor->valor.v_entera));
            break;
        case LITERAL_REAL:
            sprintf(cadena, "| %10d | %5.4f |", valor->indice, *(valor->valor.v_real));
            break;
        case IDENTIFICADOR:;
            TipoDato t = valor->valor.v_identificador->tipo;
            sprintf(cadena, "| %10d | %30s | %10s |",
                valor->indice,
                valor->valor.v_identificador->identificador,
                t == BOLEANO ? "Bool" : t == CADENA ? "Cade" : t == ENTERO ? "Ent" : t == REAL ? "Flota" : "Sin tipo"
            );
            break;
        default:
            sprintf(cadena, "| %10d | %10s |", valor->indice, valor->estatico);
    }

    return cadena;
}