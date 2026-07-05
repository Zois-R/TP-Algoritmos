#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLA_DUPLICADA 0
#define NO_EXISTE 0
#define SIN_MEMORIA 0
#define TODO_OK 1
#define MINIMO(X,Y) ((X) < (Y) ? (X) : (Y))
typedef struct s_nodo_arbol
{
    void *info;
    unsigned tam_info;
    struct s_nodo_arbol *izq,*der;
}t_nodo_arbol;

typedef t_nodo_arbol *t_arbol;

void crear_arbol(t_arbol *pa);

int insentar_en_arbol(t_arbol *pa, const void *dato,unsigned tam_dato,int (*cmp)(const void *, const void *));

int cargar_desde_set_ordenado_a_arbol(t_arbol *pa, void *set_datos, int li, int ls, unsigned (*leer)(void *,void*,unsigned, void *params),
                                      void *params );
unsigned leer_desde_arch_bin(void **info_nodo, void *pf, unsigned m, void *params); //params es el tam_registro




void recorrer_en_orden(t_arbol *pa, unsigned n, void *params,
                        void (*accion)(const void *, unsigned, void *));

void recorrer_en_pre_orden(t_arbol *pa, unsigned n, void *params,
                        void (*accion)(const void *, unsigned, void *));


void vaciar_arbol(t_arbol *pa);
int altura_arbol(const t_arbol *pa);
t_arbol* arbol_menor_nodo(t_arbol* pa);
t_arbol* arbol_mayor_nodo(t_arbol* pa);
int eliminar_de_arbol(t_arbol *pa, void* pd, size_t tam, int (*cmp)(const void*, const void*));

int arbol_eliminar_raiz(t_arbol *pa);
int buscar_en_arbol(const t_arbol *pa, void* pd, size_t tam, int (*cmp)(const void*, const void*));


#endif // ARBOL_H_INCLUDED
