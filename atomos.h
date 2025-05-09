#ifndef _PROYECTO_COMPILADORES_ATOMOS
#define _PROYECTO_COMPILADORES_ATOMOS
#include <stdio.h>
#include <wchar.h>

typedef struct _cadena_atomos {
    char cadena[2048];
    size_t indice;
    size_t size;
} CadenaAtomos;

CadenaAtomos *nueva_cadena_atomos();
void cadena_atomos_insertar(CadenaAtomos*, wchar_t*);
wchar_t cadena_atomos_siguiente(CadenaAtomos*);
void cadena_atomos_consumir(CadenaAtomos*);
void cadena_atomos_eliminar(CadenaAtomos*);

#endif // _PROYECTO_COMPILADORES_ATOMOS