#ifndef _PROYECTO_COMPILADORES_LISTA
#define _PROYECTO_COMPILADORES_LISTA
#include <stdbool.h>

typedef struct _nodo_t Nodo;
typedef void* Valor;

typedef bool (*FuncionComparar)(Valor, Valor);
typedef void (*FuncionEliminar)(Valor);
typedef char* (*FuncionACadena)(Valor);

struct _nodo_t {
    Nodo *siguiente, *anterior;
    Valor *valor;
};

typedef struct _lista_t {
    int tamano;
    Nodo *inicio, *final;
    FuncionComparar comparar;
    FuncionEliminar eliminar;
    FuncionACadena toString;
} Lista;

Nodo *nuevo_nodo(Nodo*, Nodo*, Valor);
bool nodo_comparar_direccion(Nodo*, Nodo*);
void nodo_eliminar(Nodo*, FuncionEliminar);
void nodo_imprimir_detalles(Nodo*, FuncionACadena);

Lista *nueva_lista(FuncionComparar, FuncionEliminar, FuncionACadena);
void lista_insertar(Lista*, Valor);
int lista_buscar(Lista, Valor);
void lista_eliminar(Lista*);
void lista_imprimir_detalles(Lista*);

#endif // _PROYECTO_COMPILADORES_LISTA