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

bool nodo_comparar_direccion(Nodo *a, Nodo *b) {
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

void nodo_imprimir_detalles(Nodo *nodo, FuncionACadena toString) {
    printf("Nodo { this = %p, anterior = %p, siguiente = %p, valor = %s }", nodo, nodo->anterior, nodo->siguiente, toString(nodo->valor));
}