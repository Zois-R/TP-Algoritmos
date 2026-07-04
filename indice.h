#ifndef INDICE_H_INCLUDED
#define INDICE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbol.h"

#define ERR_IND_INSERCION 0

#define TO_LOWER(x) x

typedef struct {
    t_arbol arbol;
    size_t tam_clave;                       // Usamos size_t porque así viene por parámetro
    int (*cmp)(const void *, const void*);
} t_indice;


typedef struct {
    void *clave;            // Puntero genérico a la clave (el DNI)
    unsigned nro_registro;  // La posición física en el archivo .dat
} t_reg_indice;


void ind_crear (t_indice* ind, size_t tam_clave, int (*cmp)(const void*, const void*));


int ind_insertar (t_indice* ind, void *clave, unsigned nro_reg);
int ind_grabar (const t_indice* ind, const char* path);

int ind_eliminar(t_indice* ind, void *clave, unsigned *nro_reg);
int ind_buscar (const t_indice* ind, void *clave, unsigned *nro_reg);
int ind_recorrer (const t_indice* ind, void (*accion)(const void *, unsigned, void *), void* param);
#endif // INDICE_H_INCLUDED
