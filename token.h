#ifndef _PROYECTO_COMPILADORES_TOKEN
#define _PROYECTO_COMPILADORES_TOKEN
#include "lista.h"
#define REGISTRO_CADENA_LONGITUD    100

typedef enum _tipo_dato_t {
    BOLEANO,
    CADENA,
    ENTERO,
    REAL,
    SIN_TIPO
} TipoDato;

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

typedef struct _identificador_t {
    char *identificador;
    TipoDato tipo;
} Identificador;

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

typedef struct _token_t {
    int clase, posicion;
} Token;

typedef struct _registro_tabla_t {
    int indice;
    TablaTipo tipo;
    union {
        ValorTabla valor;
        Token *token;
        char *estatico;
    };
} RegistroTabla;

typedef struct _tabla_t {
    char *nombre;
    TablaTipo tablaTipo;
    Lista *lista;
} Tabla;

int *nuevo_entero(const char*);
float *nuevo_real(const char*);
char *nueva_cadena(const char*);
Identificador *nuevo_identificador(const char *, TipoDato);
void valor_tabla_eliminar(ValorTabla*, TablaTipo);

Token *nuevo_token(int, int);
void token_eliminar(Token*);

RegistroTabla *nuevo_registro_tabla(int, TablaTipo, Valor);
bool registro_tabla_comparar(Valor, Valor);
void registro_tabla_eliminar(RegistroTabla*, TablaTipo);
char *registro_a_cadena(RegistroTabla*, TablaTipo);

Tabla *nueva_tabla(const char*, TablaTipo);
void tabla_llenar(Tabla*, int, ...);
int tabla_insertar(Tabla*, Valor);
int tabla_buscar(Tabla, Valor);
void tabla_eliminar(Tabla*);
void tabla_imprimir(Tabla);

#endif // _PROYECTO_COMPILADORES_TOKEN