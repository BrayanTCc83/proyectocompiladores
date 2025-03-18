#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

Nodo *nuevo_nodo(Nodo *anterior, Nodo *siguiente, Valor valor) {
    Nodo *nuevoNodo = (Nodo*) malloc( sizeof(Nodo) );

    nuevoNodo->anterior = anterior;
    nuevoNodo->siguiente = siguiente;
    nuevoNodo->valor = valor;

    return nuevoNodo;
}

bool nodo_comparar_direccion(Valor a, Valor b) {
    return a == b;
}

void nodo_eliminar(Nodo *nodo, FuncionEliminar eliminar) {
    if(eliminar != NULL) {
        eliminar(nodo->valor);
    }

    nodo->anterior = NULL;
    nodo->siguiente = NULL;
    nodo->valor = NULL;

    free(nodo);
}

char *nodo_a_cadena(Nodo *nodo, FuncionACadena toString) {
    char *cadena = (char*) malloc( 200 );
    int idx = sprintf(cadena, "Nodo { this = %p, anterior = %p, siguiente = %p, ", nodo, nodo->anterior, nodo->siguiente);
    if(toString) {
        sprintf(cadena + idx, "valor = %s }", toString(nodo->valor));
    } else {
        sprintf(cadena + idx, "valor = %p }", nodo->valor);
    }
}