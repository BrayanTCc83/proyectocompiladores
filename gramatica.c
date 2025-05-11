#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "token.h"
#include "lista.h"
#include "atomos.h"
#include "gramatica.h"

extern CadenaAtomos *cadena;
#ifdef DETAILS
static Lista *pilaEjecucion;
static char *cadena_a_cadena(Valor valor) {
    return (char*) valor;
}
static bool cadena_comparar(Valor str1, Valor str2) {
    return strcmp((char*)str1, (char*)str2) == 0;
}
#endif

bool errorSintactico = false;
static wchar_t c;

static bool es_tipo_valido(wchar_t c) {
    return c == L'b' || c == L'c' || c == L'n' || c == L'f';
}

static bool es_sentencia_valida(wchar_t c) {
    return c == L'a' || c == L'd' || c == L'h' || c == L'i' || c == L'o' || c == L'p'
        || c == L't';
}

static bool es_asignacion_valida(wchar_t c) {
    return c == L'#' || c == L'ñ' || c == L'$' || c == L'v' || c == L'?' || c == L':'
        || c == L'"' || c == L'^' || c == L'&';
}

static bool es_expresion_valida(wchar_t c) {
    return c == L'(' || c == L'a' || c == L'j' || c == L'e' || c == L'r' || c == L'z' || c == L'{';
}

static bool es_expresion_general_valida(wchar_t c) {
    return c == L's' || c == L'q' || c == L'm' || c == L'u';
}

static bool es_operador_relacional_valido(wchar_t c) {
    return c == L'!' || c == L'¡' || c == L',' || c == L'.' || c == L'<' || c == L'>'
        || c == L'x' || c == L'y';
}

static bool es_operador_arimetico_valido(wchar_t c) {
    return c == L'*' || c == L'/' || c == L'%' || c == L'~' || c == L'k';
}

static bool es_operador_arimetico_basico_valido(wchar_t c) {
    return c == L'+' || c == L'-';
}

static bool es_incremental_decremental_valido(wchar_t c) {
    return c == L'z' || c == L'j';
}

static bool es_lista_dec_siguiente_valido(wchar_t c) {
    return es_sentencia_valida(c) || c == L']';
}

void Program() {
    #ifdef DETAILS
    pilaEjecucion = nueva_lista(cadena_comparar, NULL, cadena_a_cadena);
    lista_push(pilaEjecucion, nueva_cadena("Program"));
    #endif
    ListDecGlob();
    Func();
    OtraFunc();
    if(cadena->indice != cadena->size) {
        fprintf(stderr, "[Error en token %d] Se esperaba fin de programa, pero se obtuvo '%lc'.", cadena->consumidos, cadena_atomos_siguiente(cadena));
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void ListDecGlob() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("ListDecGlob"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(c == L'g'){
        DecGlobal();
        ListDecGlob();
    } else if(!es_tipo_valido(c)) {
        fprintf(stderr, "[Error en token %d]: Se esperaba 'g' o un tipo 'b', 'c', 'n', 'f', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void OtraFunc() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("OtraFunc"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_tipo_valido(c)) {
        Func();
        OtraFunc();
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void DecGlobal() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("DecGlobal"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(c != L'g') {
        fprintf(stderr, "[Error en token %d]: Se esperaba 'g' en definici%lcn global 'g <Tipo> a <List> ;' pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', c);
        errorSintactico = true;
    } else {
        cadena_atomos_consumir(cadena);
        Decl();
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void Decl() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("Decl"));
    #endif
    if(errorSintactico) {
        return;
    }
    Tipo();
    c = cadena_atomos_siguiente(cadena);
    if(c != L'a') {
        fprintf(stderr, "[Error en token %d]: Se esperaba 'a' en declaraci%lcn '<Tipo> a <List> ;' pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', c);
        errorSintactico = true;
    } else {
        cadena_atomos_consumir(cadena);
        List();
        if(c != L';') {
            fprintf(stderr, "[Error en token %d]: Se esperaba ';' en declaraci%lcn '<Tipo> a <List> ;' pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', c);
            errorSintactico = true;
        } else {
            cadena_atomos_consumir(cadena);
        }
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void Tipo() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("Tipo"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(!es_tipo_valido(c)) {
        fprintf(stderr, "[Error en token %d]: Se esperaba un tipo v%lclido 'b', 'c', 'n', 'f' pero se obtuvo '%lc'.\n", cadena->consumidos, L'á', c);
        errorSintactico = true;
    } else {
        cadena_atomos_consumir(cadena);
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void List() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("List"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(c != L',' && c != L';') {
        fprintf(stderr, "[Error en token %d]: Se esperaba ',' o ';' en la lista de declaraciones pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
    } else if(c == L',') {
        cadena_atomos_consumir(cadena);
        c = cadena_atomos_siguiente(cadena);
        if(c != L'a') {
            fprintf(stderr, "[Error en token %d]: Se esperaba 'a' en la lista de declaraciones pero se obtuvo '%lc'.\n", cadena->consumidos, c);
            errorSintactico = true;
        } else {
            cadena_atomos_consumir(cadena);
            List();
        }
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void Func() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("Func"));
    #endif
    if(errorSintactico) {
        return;
    }
    Tipo();
    c = cadena_atomos_siguiente(cadena);
    if(c != L'a') {
        fprintf(stderr, "[Error en token %d]: Se esperaba 'a' en en la definici%lcn de funci%lc <Tipo> a ( <Arg> ) [ <Cuerpo> ], pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', L'ó', c);
        errorSintactico = true;
        goto _Func;
    }

    cadena_atomos_consumir(cadena);
    c = cadena_atomos_siguiente(cadena);
    if(c != L'(') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '(' en en la definici%lcn de funci%lc <Tipo> a ( <Arg> ) [ <Cuerpo> ], pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', L'ó', c);
        errorSintactico = true;
        goto _Func;
    }

    cadena_atomos_consumir(cadena);
    Arg();
    c = cadena_atomos_siguiente(cadena);
    if(c != L')') {
        fprintf(stderr, "[Error en token %d]: Se esperaba ')' en en la definici%lcn de funci%lc <Tipo> a ( <Arg> ) [ <Cuerpo> ], pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', L'ó', c);
        errorSintactico = true;
        goto _Func;
    }

    cadena_atomos_consumir(cadena);
    c = cadena_atomos_siguiente(cadena);
    if(c != L'[') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '[' en en la definici%lcn de funci%lc <Tipo> a ( <Arg> ) [ <Cuerpo> ], pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', L'ó', c);
        errorSintactico = true;
        goto _Func;
    }

    cadena_atomos_consumir(cadena);
    Cuerpo();
    c = cadena_atomos_siguiente(cadena);
    if(c != L']') {
        fprintf(stderr, "[Error en token %d]: Se esperaba ']' en en la definici%lcn de funci%lc <Tipo> a ( <Arg> ) [ <Cuerpo> ], pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', L'ó', c);
        errorSintactico = true;
        goto _Func;
    }
    cadena_atomos_consumir(cadena);

    _Func:
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void Arg() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("Arg"));
    #endif
    if(errorSintactico) {
        return;
    }
    Tipo();
    c = cadena_atomos_siguiente(cadena);
    if(c != L'a') {
        fprintf(stderr, "[Error en token %d]: Se esperaba 'a' en argumento <Tipo> a <ListArg>, pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
    } else {
        cadena_atomos_consumir(cadena);
        ListArg();
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void ListArg() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("ListaArg"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(c != L',' && c != L')') {
        fprintf(stderr, "[Error en token %d]: Se esperaba ',' o ')' en la lista de argumentos pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
    } else if(c == L',') {
        cadena_atomos_consumir(cadena);
        Arg();
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void Cuerpo() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("Cuerpo"));
    #endif
    if(errorSintactico) {
        return;
    }
    ListaDec();
    ListaSent();
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void ListaDec() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("ListaDec"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_tipo_valido(c)){
        DecGlobal();
        ListaDec();
    } else if(!es_lista_dec_siguiente_valido(c)) {
        fprintf(stderr, "[Error en token %d]: Se esperaba un tipo 'b', 'c', 'n', 'f', una sentencia 'a', 'd', 'h', 'i', 'o', 'p', 't' o cierre ']', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void Asig() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("Asig"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(c != L'a') {
        fprintf(stderr, "[Error en token %d]: Se esperaba 'a' en asignaci%lcn a OpAsig ., pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', c);
        errorSintactico = true;
        goto _Asig;
    }

    cadena_atomos_consumir(cadena);
    OpAsig();
    c = cadena_atomos_siguiente(cadena);
    if(c != L'.') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '.' en asignaci%lcn a OpAsig ., pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', c);
        errorSintactico = true;
        goto _Asig;
    }
    cadena_atomos_consumir(cadena);

    _Asig:
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void OpAsig() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("OpAsig"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(c == L'=') {
        OpAsigSimple();
        ExprGral();
    } else if(es_asignacion_valida(c)) {
        OpAsigComp();
        E();
    } else {
        fprintf(stderr, "[Error en token %d]: Se esperaba un operador de asignaci%lcn '=', '#', '%lc', '$', 'v', '?', ':', '\"', '^' o '&' en asignaci%lcn a OpAsig ., pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', L'ñ', L'ó', c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void OpAsigSimple() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("OpAsigSimple"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(c == L'=') {
        cadena_atomos_consumir(cadena);
    } else {
        fprintf(stderr, "[Error en token %d]: Se esperaba el operador de asignaci%lcn '=' en OpAsigSimple ExprGral, pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void OpAsigComp() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("OpAsigComp"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_asignacion_valida(c)) {
        cadena_atomos_consumir(cadena);
    } else {
        fprintf(stderr, "[Error en token %d]: Se esperaba el operador de asignaci%lcn '#', '%lc', '$', 'v', '?', ':', '\"', '^' o '&' en OpAsigComp E, pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', L'ñ', c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void ExprGral() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("ExprGral"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_expresion_general_valida(c)) {
        cadena_atomos_consumir(cadena);
    } else if(es_expresion_valida(c)) {
        E();
    } else {
        fprintf(stderr, "[Error en token %d]: Se esperaba una expresi%lcn general v%lclida 's', 'q', 'm', 'u', o una expresi%lcn '(', 'a', 'j', 'e', 'r', 'z' o '{', pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', L'á', L'ó', c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void E() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("E"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_expresion_valida(c)) {
        T();
        E_();
    } else {
        fprintf(stderr, "[Error en token %d]: Se esperaba una expresi%lcn v%lclida '(', 'a', 'j', 'e', 'r', 'z' o '{', pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', L'á', c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void E_() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("E'"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_operador_arimetico_basico_valido(c)) {
        cadena_atomos_consumir(cadena);
    } else if(c == L')') {
        goto _E_;
    } else if(!es_operador_relacional_valido(c)) {
        fprintf(stderr, "[Error en token %d]: Se esperaba un operador '+', '-', un operador relacional '!', '%lc', ',', '.', '<', '>', 'x', 'y' o un ')', pero se obtuvo '%lc'.\n", cadena->consumidos, L'¡', c);
        errorSintactico = true;
        goto _E_;
    }
    c = cadena_atomos_siguiente(cadena);
    if(!es_operador_relacional_valido(c)) {
        T();
        E_();
    }
    _E_:
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void T() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("T"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_expresion_valida(c)) {
        F();
        T_();
    } else if(c == L')') {
        goto _T;
    } else if(!es_operador_arimetico_basico_valido(c) && !es_operador_relacional_valido(c)) {
        fprintf(stderr, "[Error en token %d]: Se esperaba una expresi%lcn v%lclida '(', 'a', 'j', 'e', 'r', 'z' o '{', pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', L'á', c);
        errorSintactico = true;
    }
    _T:
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void T_() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("T'"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_operador_arimetico_valido(c)) {
        cadena_atomos_consumir(cadena);
        F();
        T_();
    } else if(!es_operador_arimetico_basico_valido(c) && !es_operador_relacional_valido(c) && c != L')') {
        fprintf(stderr, "[Error en token %d]: Se esperaba un operador '+', '-', '*', '/', '%', '~', 'k', un operador relacional '!', '%lc', ',', '.', '<', '>', 'x', 'y' o un ')', pero se obtuvo '%lc'.\n", cadena->consumidos, L'¡', c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void F() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("F"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(c == L'(') {
        cadena_atomos_consumir(cadena);
        E();
        c = cadena_atomos_siguiente(cadena);
        if(c != L')') {
            fprintf(stderr, "[Error en token %d]: Se esperaba ')' en expresi%lcn ( <E> ), pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', L'ó', c);
            errorSintactico = true;
            goto _F;
        }
    } else if(c == L'{') {
        LlamaFunc();
    } else if(c == L'a') {
        cadena_atomos_consumir(cadena);
        G();
    } else if(es_incremental_decremental_valido(c)) {
        F_();
        c = cadena_atomos_siguiente(cadena);
        if(c != L'a') {
            fprintf(stderr, "[Error en token %d]: Se esperaba 'a' en expresi%lcn <F'> a, pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', c);
            errorSintactico = true;
            goto _F;
        }
    } else if(c == L'e' || c == L'r' ) {
        cadena_atomos_consumir(cadena);
    } else if(!es_operador_arimetico_valido(c) && !es_operador_arimetico_basico_valido(c) && !es_operador_relacional_valido(c) && c != L')') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '(', 'a', 'z', 'j', 'e', 'r' o '{' en <F>, pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
    }
    _F:
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void F_() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("F'"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_incremental_decremental_valido(c)) {
        cadena_atomos_consumir(cadena);
    } else {
        fprintf(stderr, "[Error en token %d]: Se esperaba 'z' o 'j' <F'> a, pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void G() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("G"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_incremental_decremental_valido(c)) {
        cadena_atomos_consumir(cadena);
    } else if(!es_operador_arimetico_basico_valido(c) && !es_operador_arimetico_valido(c) && !es_operador_relacional_valido(c) && c != ')') {
        fprintf(stderr, "[Error en token %d]:  Se esperaba un operador incremental/decremental 'j', 'z', o un operador aritm%lctico '+', '-', '*', '/', '%', '~', 'k', un operador relacional '!', '%lc', ',', '.', '<', '>', 'x', 'y' o un ')', pero se obtuvo '%lc'.\n", cadena->consumidos, L'é', L'¡', c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void ExpRel() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("ExpRel"));
    #endif
    if(errorSintactico) {
        return;
    }
    E();
    OpRel();
    E();
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void OpRel() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("OpRel"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_operador_relacional_valido(c)) {
        cadena_atomos_consumir(cadena);
    } else {
        fprintf(stderr, "[Error en token %d]: Se esperaba un operador relacional '!', '%lc', ',', '.', '<', '>', 'x', 'y', pero se obtuvo '%lc'.\n", cadena->consumidos, L'¡', c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void Sent() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("Sent"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    switch (c) {
        case L'a':
            Asig();
            break;
        case L'h':
            HazM();
            break;
        case L'i':
            Si();
            break;
        case L'o':
            Para();
            break;
        case L'd':
            Dev();
            break;
        case L't':
            cadena_atomos_consumir(cadena);
            c = cadena_atomos_siguiente(cadena);
            if(c == L'.') {
                cadena_atomos_consumir(cadena);
            } else {
                fprintf(stderr, "[Error en token %d]: Se esperaba '.', en sentencia continuar, pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', c);
                errorSintactico = true;
            }
            break;
        case L'p':
            cadena_atomos_consumir(cadena);
            c = cadena_atomos_siguiente(cadena);
            if(c == L'.') {
                cadena_atomos_consumir(cadena);
            } else {
                fprintf(stderr, "[Error en token %d]: Se esperaba '.', en sentencia parar, pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', c);
                errorSintactico = true;
            }
            break;
        default:
            fprintf(stderr, "[Error en token %d]: Se esperaba una sentencia de asignaci%lcn, ciclo mientras, condicional, devolver, continuar o parar, pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', c);
            errorSintactico = true;
            break;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void ListaSent() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("ListaSent"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_sentencia_valida(c)) {
        Sent();
        ListaSent();
    } else if(c != L']') {
        fprintf(stderr, "[Error en token %d]: Se esperaba una sentencia v%lclida 'a', 'd', 'h', 'i', 'o', 'p', 't' o un cierre ']', pero se obtuvo '%lc'.\n", cadena->consumidos, L'á', c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void HazM() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("HazM"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(c != L'h') {
        fprintf(stderr, "[Error en token %d]: Se esperaba 'h'en en la sentencia haz mientras 'h [ <ListaSent> ] w ( <ExpRel> )', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _HazM;
    }

    cadena_atomos_consumir(cadena);
    c = cadena_atomos_siguiente(cadena);
    if(c != L'[') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '[' en en la sentencia haz mientras 'h [ <ListaSent> ] w ( <ExpRel> )', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _HazM;
    }

    cadena_atomos_consumir(cadena);
    ListaSent();
    c = cadena_atomos_siguiente(cadena);
    if(c != L']') {
        fprintf(stderr, "[Error en token %d]: Se esperaba ']' en en la sentencia haz mientras 'h [ <ListaSent> ] w ( <ExpRel> )', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _HazM;
    }

    cadena_atomos_consumir(cadena);
    c = cadena_atomos_siguiente(cadena);
    if(c != L'w') {
        fprintf(stderr, "[Error en token %d]: Se esperaba 'w' en en la sentencia haz mientras 'h [ <ListaSent> ] w ( <ExpRel> )', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _HazM;
    }

    cadena_atomos_consumir(cadena);
    c = cadena_atomos_siguiente(cadena);
    if(c != L'(') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '(' en en la sentencia haz mientras 'h [ <ListaSent> ] w ( <ExpRel> )', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _HazM;
    }

    cadena_atomos_consumir(cadena);
    ExpRel();
    c = cadena_atomos_siguiente(cadena);
    if(c != L')') {
        fprintf(stderr, "[Error en token %d]: Se esperaba ')' en en la sentencia haz mientras 'h [ <ListaSent> ] w ( <ExpRel> )', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _HazM;
    }
    cadena_atomos_consumir(cadena);
    _HazM:
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void Si() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("Si"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(c != L'i') {
        fprintf(stderr, "[Error en token %d]: Se esperaba 'i' en la sentencia si 'i ( <ExpRel> ) [ <ListaSent> ]', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Si;
    }

    cadena_atomos_consumir(cadena);
    c = cadena_atomos_siguiente(cadena);
    if(c != L'(') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '(' en la sentencia si 'i ( <ExpRel> ) [ <ListaSent> ]', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Si;
    }

    cadena_atomos_consumir(cadena);
    ExpRel();
    c = cadena_atomos_siguiente(cadena);
    if(c != L')') {
        fprintf(stderr, "[Error en token %d]: Se esperaba ')' en la sentencia si 'i ( <ExpRel> ) [ <ListaSent> ]', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Si;
    }

    cadena_atomos_consumir(cadena);
    c = cadena_atomos_siguiente(cadena);
    if(c != L'[') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '[' en la sentencia si 'i ( <ExpRel> ) [ <ListaSent> ]', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Si;
    }

    cadena_atomos_consumir(cadena);
    ListaSent();
    c = cadena_atomos_siguiente(cadena);
    if(c != L']') {
        fprintf(stderr, "[Error en token %d]: Se esperaba ']' en la sentencia si 'i ( <ExpRel> ) [ <ListaSent> ]', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Si;
    }
    cadena_atomos_consumir(cadena);
    Sino();
    _Si:
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void Sino() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("Sino"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_sentencia_valida(c) || c == L']') {
        goto _Sino;
    }
    if(c != L'l') {
        fprintf(stderr, "[Error en token %d]: Se esperaba 'i' en la sentencia si-sino 'i ( <ExpRel> ) [ <ListaSent> ] l [ <ListaSent> ]', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Sino;
    }

    cadena_atomos_consumir(cadena);
    c = cadena_atomos_siguiente(cadena);
    if(c != L'[') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '[' en la sentencia si-sino 'i ( <ExpRel> ) [ <ListaSent> ] l [ <ListaSent> ]', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Sino;
    }

    cadena_atomos_consumir(cadena);
    ListaSent();
    c = cadena_atomos_siguiente(cadena);
    if(c != L']') {
        fprintf(stderr, "[Error en token %d]: Se esperaba ']' en la sentencia si-sino 'i ( <ExpRel> ) [ <ListaSent> ] l [ <ListaSent> ]', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Sino;
    }
    cadena_atomos_consumir(cadena);
    _Sino:
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void Para() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("Para"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(c != L'o') {
        fprintf(stderr, "[Error en token %d]: Se esperaba 'o' en la sentencia para 'o ( <E> ) [ <ListaSent> ]', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Para;
    }

    cadena_atomos_consumir(cadena);
    c = cadena_atomos_siguiente(cadena);
    if(c != L'(') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '(' en la sentencia para 'o ( <E> ) [ <ListaSent> ]', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Para;
    }

    cadena_atomos_consumir(cadena);
    E();
    c = cadena_atomos_siguiente(cadena);
    if(c != L')') {
        fprintf(stderr, "[Error en token %d]: Se esperaba ')' en la sentencia para 'o ( <E> ) [ <ListaSent> ]', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Para;
    }

    cadena_atomos_consumir(cadena);
    c = cadena_atomos_siguiente(cadena);
    if(c != L'[') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '[' en la sentencia para 'o ( <E> ) [ <ListaSent> ]', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Para;
    }

    cadena_atomos_consumir(cadena);
    ListaSent();
    c = cadena_atomos_siguiente(cadena);
    if(c != L']') {
        fprintf(stderr, "[Error en token %d]: Se esperaba ']' en la sentencia para 'o ( <E> ) [ <ListaSent> ]', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Para;
    }
    cadena_atomos_consumir(cadena);
    _Para:
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void Dev() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("Dev"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(c != L'd') {
        fprintf(stderr, "[Error en token %d]: Se esperaba 'i' en la sentencia devolver 'd ( <ValRet> )', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Si;
    }

    cadena_atomos_consumir(cadena);
    c = cadena_atomos_siguiente(cadena);
    if(c != L'(') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '(' en la sentencia devolver 'd ( <ValRet> )', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Si;
    }

    cadena_atomos_consumir(cadena);
    ValRet();
    c = cadena_atomos_siguiente(cadena);
    if(c != L')') {
        fprintf(stderr, "[Error en token %d]: Se esperaba ')' en la sentencia devolver 'd ( <ValRet> )', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _Si;
    }
    cadena_atomos_consumir(cadena);
    _Si:
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void ValRet() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("ValRet"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_expresion_general_valida(c) || es_expresion_valida(c)) {
        ExprGral();
    } else if(c != L')') {
        fprintf(stderr, "[Error en token %d]: Se esperaba una expresi%lcn general v%lclida 's', 'q', 'm', 'u', o una expresi%lcn '(', 'a', 'j', 'e', 'r', 'z' o '{', o ')' pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', L'á', L'ó', c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void LlamaFunc() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("LlamaFunc"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(c != L'{') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '{' en la llamada a funcion '{ a ( <ListP> ) }', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _LlamaFunc;
    }

    cadena_atomos_consumir(cadena);
    c = cadena_atomos_siguiente(cadena);
    if(c != L'a') {
        fprintf(stderr, "[Error en token %d]: Se esperaba 'a' en la llamada a funcion '{ a ( <ListP> ) }', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _LlamaFunc;
    }

    cadena_atomos_consumir(cadena);
    c = cadena_atomos_siguiente(cadena);
    if(c != L'(') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '(' en la llamada a funcion '{ a ( <ListP> ) }', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _LlamaFunc;
    }

    cadena_atomos_consumir(cadena);
    ListP();
    c = cadena_atomos_siguiente(cadena);
    if(c != L')') {
        fprintf(stderr, "[Error en token %d]: Se esperaba ')' en la llamada a funcion '{ a ( <ListP> ) }', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _LlamaFunc;
    }

    cadena_atomos_consumir(cadena);
    c = cadena_atomos_siguiente(cadena);
    if(c != L'}') {
        fprintf(stderr, "[Error en token %d]: Se esperaba '}' en la llamada a funcion '{ a ( <ListP> ) }', pero se obtuvo '%lc'.\n", cadena->consumidos, c);
        errorSintactico = true;
        goto _LlamaFunc;
    }
    cadena_atomos_consumir(cadena);
    _LlamaFunc:
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void ListP() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("ListP"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(es_expresion_general_valida(c) || es_expresion_valida(c)) {
        ExprGral();
        Param();
    } else if(c != L')') {
        fprintf(stderr, "[Error en token %d]: Se esperaba una expresi%lcn general v%lclida 's', 'q', 'm', 'u', o una expresi%lcn '(', 'a', 'j', 'e', 'r', 'z' o '{', o ')' pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', L'á', L'ó', c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}

void Param() {
    #ifdef DETAILS
    lista_push(pilaEjecucion, nueva_cadena("Param"));
    #endif
    if(errorSintactico) {
        return;
    }
    c = cadena_atomos_siguiente(cadena);
    if(c == L',') {
        cadena_atomos_consumir(cadena);
        ExprGral();
        Param();
    } else if(c != L')') {
        fprintf(stderr, "[Error en token %d]: Se esperaba una expresi%lcn general v%lclida 's', 'q', 'm', 'u', o una expresi%lcn '(', 'a', 'j', 'e', 'r', 'z' o '{', o ')' pero se obtuvo '%lc'.\n", cadena->consumidos, L'ó', L'á', L'ó', c);
        errorSintactico = true;
    }
    #ifdef DETAILS
    lista_pop(pilaEjecucion);
    #endif
}
