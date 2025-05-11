#ifndef _PROYECTO_COMPILADORES_LISTA
#define _PROYECTO_COMPILADORES_LISTA
#include <stdbool.h>

/**
 * Definimos el nodo, usado para poder trabajar con la lista, se hará uso
 * de una lista doblemenete enlazada.
 */
typedef struct _nodo_t Nodo;

/**
 * Definimos un puntero genérico para nuestra lista, así nos aseguramos de
 * abstraer el comportamiento de la lista, dependerá de la implementación
 * y no de la estructura lista en sí.
 */
typedef void* Valor;

/**
 * Se define un tipo genérico para comparar valores genéricos.
 */
typedef bool (*FuncionComparar)(Valor, Valor);

/**
 * Se define un tipo de función genérico para eliminar un valor dado.
 */
typedef void (*FuncionEliminar)(Valor);

/**
 * Se define un tipo de función para transformar a cadena genérico.
 */
typedef char* (*FuncionACadena)(Valor);

/**
 * Define una estructura completa del nodo para una lista dinámica doblemente
 * ligada y genérico, es decir guarda un puntero, pero el nodo desconoce como
 * manejarlo, depende de la implementación.
 */
struct _nodo_t {
    Nodo *siguiente, *anterior;
    Valor *valor;
};

/**
 * Define una lista dinámica doblemente ligada y genérica, almacena el tamaño
 * actual de la lista, el nodo inicial y final, así como las referencias a las
 * funciones genéricas que interpretarán los nodos.
 */
typedef struct _lista_t {
    int tamano;
    Nodo *inicio, *final;
    FuncionComparar comparar;
    FuncionEliminar eliminar;
    FuncionACadena toString;
} Lista;

/**
 * Permite crear un nuevo nodo, es decir, reserva la memoria para el nodo e
 * inicializa cada una de sus propiedades.
 */
Nodo *nuevo_nodo(Nodo*, Nodo*, Valor);
/**
 * Función por defecto usada cuando no se crea una lista con una función de
 * comparación, simplemente compara las direcciones de los nodos, es preventiva.
 */
bool nodo_comparar_direccion(Valor, Valor);
/**
 * Permite limpiar la memoria para el nodo en cuestión una vez que haya terminado
 * de usarse el mismo, es útil porque evita dejar basura en memoria.
 */
void nodo_eliminar(Nodo*, FuncionEliminar);
/**
 * Se usa para convertir un nodo a una cadena, se uso principalmente para depurar
 * y probar el funcionamiento de la lista.
 */
char *nodo_a_cadena(Nodo*, FuncionACadena);

/**
 * Permite crear una nueva lista, es decir reservar la memoria e inicializar cada
 * una de las propiedades de la misma, así como sus métodos genéricos.
 */
Lista *nueva_lista(FuncionComparar, FuncionEliminar, FuncionACadena);
/**
 * Permite agregar un nuevo valor dentro de la lista, para ello hace uso de las
 * funciones de comparación designadas, en pro de evitar repeticiones (si así
 * lo indica la función de comparación).
 */
int lista_insertar(Lista*, Valor);
void lista_push(Lista*, Valor);
Valor lista_pop(Lista*);
/**
 * Busca la primera coincidencia de un valor dentro de la lista, a través de la
 * función de comparación asignada.
 */
int lista_buscar(Lista, Valor);
Valor lista_obtener(Lista, int);
/**
 * Permite eliminar la lista, evitando que se quede basura dentro de la memoria y
 * liberando el espacio para usarlo a posterior.
 */
void lista_eliminar(Lista*);
/**
 * Permite imprimir la lista de manera detallada, se usa para poder realizar las
 * pruebas del funcionamiento de las listas doblemente ligadas.
 */
void lista_imprimir_detalles(Lista*);
void lista_imprimir(Lista);

#endif // _PROYECTO_COMPILADORES_LISTA