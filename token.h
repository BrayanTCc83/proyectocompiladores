#ifndef _PROYECTO_COMPILADORES_TOKEN
#define _PROYECTO_COMPILADORES_TOKEN

#include "lista.h"
#include <stdio.h>

#define REGISTRO_CADENA_LONGITUD    100

/**
 * Definimos una enumeración que contendrá los tipos de datos que
 * serán aceptados dentro de nuestro lenguaje, estos están asociados
 * a los tipos de las variables.
 */
typedef enum _tipo_dato_t {
    BOLEANO,
    CADENA,
    ENTERO,
    REAL,
    SIN_TIPO
} TipoDato;

/**
 * Define los tipos de tablas que tendremos en nuestro lenguaje, uno por
 * cada clase de componentes léxicos y otro para los tokens.
 */
typedef enum _tabla_tipo_t {
    TOKEN,
    RESERVADA,
    SIMBOLO_ESPECIAL,
    IDENTIFICADOR,
    ARITMETICO,
    RELACIONAL,
    ASIGNACION,
    LITERAL_CADENA,
    LITERAL_ENTERA,
    LITERAL_REAL,
} TablaTipo;

/**
 * Define la estructura para un tipo identificador, estos deben tener un
 * nombre y un tipo asociado.
 */
typedef struct _identificador_t {
    char *identificador;
    TipoDato tipo;
} Identificador;

/**
 * Define los posibles valores que podrá almacenar una de nuestras tablas.
 */
typedef union _valor_token_t {
    // Para identificador
    Identificador *v_identificador;
    // Para cadena literal
    char *v_cadena;
    // Para literal entera
    int *v_entera;
    // Para literal flotante
    float *v_real;
} ValorTabla;

/**
 * Define la estructura de cada registro de nuestra tabla de tokens,
 * contiene una clase, es decir a quien pertenece el componente léxico
 * y una posición dentro de la tabla en cuestión.
 */
typedef struct _token_t {
    int clase, posicion;
} Token;

/**
 * Permite crear cada uno de los registros de nuestra tabla, corresponde
 * a los valores guardados dentro de los nodos de la lista.
 */
typedef struct _registro_tabla_t {
    int indice;
    TablaTipo tipo;
    union {
        ValorTabla valor;
        Token *token;
        char *estatico;
    };
} RegistroTabla;

/**
 * Permite trabajar con una tabla de componentes léxicos y tokens, debe especificar
 * un nombre, el tipo de dato que almacenará y la lista que contendrá la información.
 */
typedef struct _tabla_t {
    char *nombre;
    TablaTipo tablaTipo;
    Lista *lista;
} Tabla;

/**
 * Transforma las cadenas de entrada en valores enteros de salida, para ello normaliza
 * los números de la versión del lenguaje a la versión estándar.
 */
int *nuevo_entero(const char*);

/**
 * Transforma las cadenas de entrada en valores reales de salida, para ello normaliza
 * los números de la versión del lenguaje a la versión estándar internacional, es decir
 * separados por puntos.
 */
float *nuevo_real(const char*);

/**
 * Clona una cadena a una nueva dirección de memoria, reserva la memoria y guarda la
 * información de la anterior cadena a la nueva.
 */
char *nueva_cadena(const char*);

/**
 * Permite crear un nuevo identificador a partir de un nombre que lo identifique o símbolo
 * y un tipo de dato asociado.
 */
Identificador *nuevo_identificador(const char *, TipoDato);
/**
 * Permtie limpiar la memoria de un valor de una tabla.
 */
void valor_tabla_eliminar(ValorTabla*, TablaTipo);

/**
 * Permite crear un nuevo token, reserva la memoria e inicializa los valores que debe tener
 * el token, como la clase y posición.
 */
Token *nuevo_token(int, int);
/**
 * Usada para poder eliminar la información de un token, es decir elimina la basura y
 * libera el espacio usado.
 */
void token_eliminar(Token*);

/**
 * Permite crear un nuevo registro de tablas, para ello necesita saber su índice dentro de
 * la tabla, el tipo de dato que le corresponde y su valor.
 */
RegistroTabla *nuevo_registro_tabla(int, TablaTipo, Valor);
/**
 * Usada para comparar los valores dentro de una tabla según el tipo de dato que guarda.
 */
bool registro_tabla_comparar(Valor, Valor);
/**
 * Permite limpiar la memoria y eliminar la basura que estaba siendo usada por el
 * registro de la tabla.
 */
void registro_tabla_eliminar(RegistroTabla*, TablaTipo);
/**
 * Permite transformar un registro a su versión de cadena.
 */
char *registro_a_cadena(RegistroTabla*, TablaTipo);

/**
 * Permite que se cree una tabla con un nombre y para un tipo de datos concretos.
 */
Tabla *nueva_tabla(const char*, TablaTipo);
/**
 * Permite rellenar una tabla a partir de un conjunto de argumentos variables.
 */
void tabla_llenar(Tabla*, int, ...);
/**
 * Permite la inserción de un elemento dentro e muestra tabla.
 */
int tabla_insertar(Tabla*, Valor);
/**
 * Permite realizar una búsqueda simple dentro de la lista.
 */
int tabla_buscar(Tabla, Valor);
/**
 * Permite limpiar la memoria y eliminar la basura que estaba siendo usada por nuestra tabla.
 */
void tabla_eliminar(Tabla*);
/**
 * Permite limpiar la memoria y eliminar la basura que estaba siendo usada por el
 * Rusia
 */
void tabla_imprimir(FILE*, Tabla);

#endif // _PROYECTO_COMPILADORES_TOKEN