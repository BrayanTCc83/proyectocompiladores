#include <stdio.h>
#include <stdlib.h>
#include "atomos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

CadenaAtomos *nueva_cadena_atomos() {
    CadenaAtomos *cadena = (CadenaAtomos*) malloc(sizeof(CadenaAtomos));
    cadena->cadena[0] = '\0';
    cadena->indice = 0;
    cadena->size = 0;
    cadena->consumidos = 0;
    return cadena;
}

// Inserta un átomo (carácter o secuencia wchar_t) como UTF-8 en la cadena
void cadena_atomos_insertar(CadenaAtomos *cadena, const wchar_t *atomo) {
    #ifdef DETAILS
    printf("Insertando: %ls.\n", atomo);
    #endif
    // Convertir wchar_t a multibyte (UTF-8)
    char buffer[MB_CUR_MAX * 8];  // permite hasta varios wchar_t
    int bytes = wcsrtombs(buffer, &atomo, sizeof(buffer), NULL);
    if (bytes > 0) {
        memcpy(cadena->cadena + cadena->size, buffer, bytes);
        cadena->size += bytes;
        cadena->cadena[cadena->size] = '\0'; // null-terminate
    }

    #ifdef DETAILS
    printf("Cadena atomos: %s\n", cadena->cadena);
    #endif
}

// Devuelve el siguiente átomo (wchar_t) desde el índice actual
wchar_t cadena_atomos_siguiente(CadenaAtomos *cadena) {
    wchar_t wc;
    mbstate_t st;
    memset(&st, 0, sizeof(st));

    size_t len = mbrtowc(&wc, cadena->cadena + cadena->indice, cadena->size - cadena->indice, &st);
    if (len == (size_t)-1 || len == (size_t)-2 || len == 0) {
        return L'\0';  // error o fin
    }

    #ifdef DETAILS
    printf("Siguiente en cadena: %lc\n", wc);
    #endif
    return wc;
}

// Avanza al siguiente carácter UTF-8 en la cadena
void cadena_atomos_consumir(CadenaAtomos *cadena) {
    mbstate_t st;
    memset(&st, 0, sizeof(st));
    wchar_t wc;
    size_t len = mbrtowc(&wc, cadena->cadena + cadena->indice, cadena->size - cadena->indice, &st);
    if (len != (size_t)-1 && len != (size_t)-2 && len > 0) {
        cadena->indice += len;
        cadena->consumidos++;  // Aumenta contador de átomos leídos
    }
    #ifdef DETAILS
    printf("Consumiendo token de %d byte(s).\n", len);
    #endif
}

void cadena_atomos_eliminar(CadenaAtomos *cadena) {
    free(cadena);
}