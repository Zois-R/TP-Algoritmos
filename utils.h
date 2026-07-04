#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "indice.h"


#define ERR_ARCH 0

// Estructura auxiliar para manejar las fechas
typedef struct {
    int dia;
    int mes;
    int anio;
} t_fecha;


typedef struct {
    long dni;                       // 10.000 < DNI < 100.000.000
    char apellidos[61];             // 60 caracteres + 1 para el '\0'
    char nombres[61];               // 60 caracteres + 1 para el '\0'
    t_fecha fecha_nacimiento;
    char sexo;                      // 'F', 'M' u 'O'
    t_fecha fecha_afiliacion;
    char categoria[11];             // 10 caracteres + 1 para el '\0' ('VITALICIO', 'MENOR', etc.)
    t_fecha fecha_ultima_cuota;
    char estado;                    // 'A' (Activo) o 'I' (Inactivo)
    t_fecha fecha_baja;             // Si el estado es 'A', esta fecha debería estar "vacía" o en cero
} t_socio;


///PRIMER APLICACIÓN
void pasar_archivo_csv_binario(const char *nombre_arch);
void leer_archivo_binario(const char *nombre_arch);


void mostrar_menu(t_indice *pi, const char* pathSocio);
int cmp_clave(const void *a, const void *b);

void mostrar_clave(const void *info_nodo, unsigned tam_info, void *params);

int crear_indice_socios_desde_arch_maestro(t_indice *pi,const char *path);


#endif // UTILS_H_INCLUDED
