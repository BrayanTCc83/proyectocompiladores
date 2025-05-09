#include <stdio.h>
#include <stdlib.h>
#include "atomos.h"

CadenaAtomos *nueva_cadena_atomos() {
    CadenaAtomos *cadena = (CadenaAtomos*) malloc( sizeof(CadenaAtomos) );

    cadena->cadena[0] = 0;
    cadena->indice = 0;
    cadena->size = 0;

    return cadena;
}

void cadena_atomos_insertar(CadenaAtomos *cadena, wchar_t *atomo) {
    cadena->size += sprintf(cadena->cadena + cadena->size, "%ls", atomo);
    #ifdef DETAILS
    printf("Cadena atomos: %s\n", cadena->cadena);
    #endif
}

wchar_t cadena_atomos_siguiente(CadenaAtomos *cadena) {
    #ifdef DETAILS
    printf("Siguiente en cadena: %c\n", cadena->cadena[cadena->indice]);
    #endif
    return cadena->cadena[cadena->indice];
}

void cadena_atomos_consumir(CadenaAtomos *cadena) {
    cadena->indice++;
}

void cadena_atomos_eliminar(CadenaAtomos *cadena) {
    cadena->indice = 0;
    cadena->size = 0;
    free(cadena->cadena);
    free(cadena);
}
