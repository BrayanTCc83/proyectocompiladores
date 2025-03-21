#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

Lista *nueva_lista(FuncionComparar comparar, FuncionEliminar eliminar, FuncionACadena toString) {
    Lista *nuevaLista = (Lista*) malloc( sizeof(Lista) );

    if(comparar) {
        nuevaLista->comparar = comparar;
    } else {
        nuevaLista->comparar = nodo_comparar_direccion;
    }
    if(eliminar) {
        nuevaLista->eliminar = eliminar;
    } else {
        nuevaLista->eliminar = NULL;
    }
    if(toString) {
        nuevaLista->toString = toString;
    } else {
        nuevaLista->toString = NULL;
    }
    nuevaLista->final = nuevaLista->inicio = NULL;
    nuevaLista->tamano = 0;

    return nuevaLista;
}

int lista_insertar(Lista *lista, Valor valor) {
    if(lista->inicio == NULL) {
        Nodo *nuevoNodo = nuevo_nodo(NULL, NULL, valor);
        nuevoNodo->anterior = nuevoNodo->siguiente = nuevoNodo;
        lista->inicio = lista->final = nuevoNodo;
        lista->tamano++;
        return 0;
    }

    int pos = lista_buscar(*lista, valor);
    if(pos != -1) {
        return pos;
    }

    Nodo *nuevoNodo = nuevo_nodo(lista->final, lista->inicio, valor);
    lista->tamano++;
    lista->inicio->anterior = nuevoNodo;
    lista->final->siguiente = nuevoNodo;
    lista->final = nuevoNodo;
    return lista->tamano - 1;
}

int lista_buscar(Lista lista, Valor valor) {
    Nodo *aux = lista.inicio, *final = lista.final;
    int indice = 0;

    while(aux != final & !lista.comparar(aux->valor, valor)) {
        aux = aux->siguiente;
        indice++;
    }

    if(lista.comparar(aux->valor, valor)) {
        return indice;
    }

    return -1;
}

void lista_eliminar(Lista *lista) {
    Nodo *aux = lista->inicio;
    while(aux != lista->final) {
        Nodo *cur = aux;
        aux = aux->siguiente;
        
        nodo_eliminar(cur, lista->eliminar);
    }
    free(lista);
}

void lista_imprimir_detalles(Lista *lista) {
    Nodo *aux = lista->inicio;
    size_t i = 0;
    printf("Lista {\n\tcomparar = %p,\n\teliminar = %p,\n\timpresion = %p\n\tnodos = {", lista->comparar, lista->eliminar, lista->toString);
    while(aux != lista->final) {
        printf("[%d] = %s\n", i++, nodo_a_cadena(aux, lista->toString));
    }
    printf("[%d] = %s\n\t}\n}", i++, nodo_a_cadena(aux, lista->toString));
}