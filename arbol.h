#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLA_DUPLICADA 0
#define SIN_MEMORIA 0
#define TODO_OK 1
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

typedef struct s_nodo_arbol
{
    void *info;
    unsigned tam_info;
    struct s_nodo_arbol *izq,*der;
}t_nodo_arbol;

typedef t_nodo_arbol *t_arbol;

void crear_arbol(t_arbol *pa);

int insentar_en_arbol(t_arbol *pa, const void *dato,unsigned tam_dato,int (*cmp)(const void *, const void *));

int cargar_desde_set_ordenado_a_arbol(t_arbol *pa, void *set_datos, int li, int ls, unsigned (*leer)(void **,void*,unsigned, void *params),
                                      void *params );
unsigned leer_desde_arch_bin(void **info_nodo, void *pf, unsigned m, void *params); //params es el tam_registro
int cargar_arch_bin_ord (t_arbol *pa, const char *path, unsigned tam_dato);

int buscar_elem_arbol (const t_arbol *pa, void *dato, unsigned tam_dato, int(*cmp)(const void*,const void*));
int elim_elem_arbol (t_arbol *pa, void *dato, unsigned tam_dato, int(*cmp)(const void*, const void*));

void recorrer_en_orden(t_arbol *pa, unsigned n, void *params, void (*accion)(const void *, unsigned, void *));

int eliminar_raiz_arbol(t_arbol *pa);
unsigned altura_arbol (const t_arbol *pa);

t_nodo_arbol ** mayor_nodo_arbol (const t_arbol *pa);
t_nodo_arbol ** menor_nodo_arbol (const t_arbol *pa);
t_nodo_arbol ** buscar_nodo_arbol (const t_arbol *pa, const void *dato,
                                   int(*cmp)(const void*, const void*));
#endif // ARBOL_H_INCLUDED
