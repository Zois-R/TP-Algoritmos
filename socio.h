#ifndef SOCIO_H_INCLUDED
#define SOCIO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "indice.h"
#include "utils.h"




void accion_listar_socio(const void *info_nodo, unsigned tam_info, void *params);
int leer_socio(unsigned nro_reg, t_socio *socio, const char* path);

void alta_socio(t_indice *pi, const char* path_archivo);
void modificar_socio(t_indice *pi, const char* path_archivo);
int actualizar_socio(unsigned nro_reg, t_socio *socio, const char* path);
void baja_socio(t_indice *pi, const char* path_archivo);

void listar_socios(t_indice *pi, const char* path_archivo);
int compactar_y_reindexar(t_indice *pi, const char *path);

#endif // SOCIO_H_INCLUDED
