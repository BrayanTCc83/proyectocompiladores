#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

Lista *nueva_lista(FuncionComparar comparar, FuncionEliminar eliminar, FuncionACadena toString) {
    Lista *nuevaLista = (Lista*) malloc( sizeof(Lista) );

    nuevaLista->comparar = comparar;
    nuevaLista->eliminar = eliminar;
    nuevaLista->toString = toString;
    nuevaLista->final = nuevaLista->inicio = NULL;
    nuevaLista->tamano = 0;

    return nuevaLista;
}

void lista_insertar(Lista *lista, Valor valor) {
    if(lista->inicio == NULL) {
        Nodo *nuevoNodo = nuevo_nodo(NULL, NULL, valor);
        nuevoNodo->anterior = nuevoNodo->siguiente = nuevoNodo;
        lista->inicio = lista->final = nuevoNodo;
        return;
    }

    Nodo *aux = lista->inicio, *final = lista->final;

    while(aux != final) {
        aux = aux->siguiente;
    }

    Nodo *nuevoNodo = nuevo_nodo(aux, lista->inicio, valor);

}

int lista_buscar(Lista lista, Valor valor) {
    Nodo *aux = lista.inicio, *final = lista.final;
    int indice = 0;

    while(aux != final & !lista.comparar(valor, aux->valor)) {
        aux = aux->siguiente;
        indice++;
    }

    if(lista.comparar(valor, aux->valor)) {
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
        printf("[%d] = ", i++);
        nodo_imprimir_detalles(aux, lista->toString);
        printf("\n");
    }
    printf("[%d] = ", i++);
    nodo_imprimir_detalles(aux, lista->toString);
    printf("\n\t}\n}");
}