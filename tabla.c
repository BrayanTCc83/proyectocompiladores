#include "lista.h"
#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

Tabla *nueva_tabla(const char *nombre, TablaTipo tipo) {
    Tabla *tabla = (Tabla*) malloc( sizeof(Tabla) );

    tabla->tablaTipo = tipo;
    tabla->nombre = nueva_cadena(nombre);
    tabla->lista = nueva_lista(registro_tabla_comparar, NULL, NULL);

    return tabla;
}

void tabla_llenar(Tabla *tabla, int cantidad, ...) {
    va_list args;
    va_start(args, cantidad);

    for (int i = 0; i < cantidad; i++) {
        lista_insertar(
            tabla->lista, 
            nuevo_registro_tabla(i, tabla->tablaTipo, nueva_cadena(va_arg(args, const char *)))
        );
    }

    va_end(args);
}

int tabla_insertar(Tabla *tabla, Valor valor) {
    int indice = tabla->lista->tamano;
    lista_insertar(tabla->lista, nuevo_registro_tabla(indice, tabla->tablaTipo, valor));
    return indice;
}

int tabla_buscar(Tabla tabla, Valor valor) {
    if(tabla.lista->tamano == 0) {
        return -1;
    }
    return lista_buscar(*tabla.lista, valor);
}

void tabla_eliminar(Tabla *tabla) {
    strcpy(tabla->nombre, "");
    tabla->nombre = NULL;
    free(tabla->nombre);
    tabla->tablaTipo = 0;

    lista_eliminar(tabla->lista);
    free(tabla);
}

void tabla_imprimir(FILE *archivo, Tabla tabla) {
    size_t i = 0;
    fprintf(archivo, "\n%s:\n\n", tabla.nombre);
    switch (tabla.tablaTipo) {
        case TOKEN:
            fprintf(archivo, "| %10s | %10s |\n", "Clase", "Posicion");
            break;
        case LITERAL_CADENA:
            fprintf(archivo, "| %10s | %50s |\n", "Posicion", "Valor");
            break;
        case LITERAL_ENTERA:
            fprintf(archivo, "| %10s | %10s |\n", "Posicion", "Valor");
            break;
        case LITERAL_REAL:
            fprintf(archivo, "| %10s | %10s |\n", "Posicion", "Valor");
            break;
        case IDENTIFICADOR:;
            fprintf(archivo, "| %10s | %30s | %10s |\n", "Posicion", "Identificador", "Tipo");
            break;
        default:
            fprintf(archivo, "| %10s | %10s |\n", "Posicion", "Valor");
    }
    
    Nodo *aux = tabla.lista->inicio;
    while(aux != tabla.lista->final) {
        fprintf(archivo, "%s\n", registro_a_cadena((RegistroTabla*)(aux->valor), tabla.tablaTipo));
        aux = aux->siguiente;
    }
    if(aux == NULL) {
        return;
    }
    fprintf(archivo, "%s\n", registro_a_cadena((RegistroTabla*)(aux->valor), tabla.tablaTipo));
}