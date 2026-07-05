#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int main()
{
    char path[256];
    t_indice indice_socios;

    const char *archivo_maestro = "socios.dat";

    // 1. Verificamos si la base de datos principal ya existe
    FILE *pf_dat = fopen(archivo_maestro, "rb");
    if (pf_dat != NULL)
    {
        // El archivo ya existe. Lo cerramos y evitamos pisarlo con el CSV.
        fclose(pf_dat);
        printf("-> Base de datos '%s' encontrada. Conservando registros previos.\n", archivo_maestro);
    }
    else
    {
        // El archivo NO existe. Es el primer arranque, pedimos el CSV.
        printf("-> No se encontro la base de datos principal.\n");
        printf("Por favor, ingrese el path del archivo CSV de socios para inicializar el sistema:\n> ");

        if (fgets(path, sizeof(path), stdin) != NULL)
        {
            path[strcspn(path, "\n")] = '\0';
            pasar_archivo_csv_binario(path);
            printf("-> Lectura finalizada. Se genero el archivo '%s'.\n", archivo_maestro);
        }
        else
        {
            printf("Error al leer el path. Cerrando el programa...\n");
            return 1; // Salimos con código de error
        }
    }


    //leer_archivo_binario("socios.dat"); // TESTING

    ind_crear(&indice_socios,sizeof(long),cmp_clave); //inicializar estructura indice

    // Consultamos externamente si el archivo .idx ya existe en el disco
    FILE *pf_idx = fopen("socios.idx", "rb");

    if (pf_idx != NULL)
    {
        // El archivo existe. Lo cerramos y mandamos a cargar el árbol balanceado.
        fclose(pf_idx);
        printf("-> Archivo 'socios.idx' detectado. Cargando arbol balanceado...\n");
        ind_cargar(&indice_socios, "socios.idx");
    }
    else
    {
        // El archivo no existe (es la primera vez o se borró). Leemos el maestro.
        printf("-> No se encontro 'socios.idx'. Generando indice desde 'socios.dat'...\n");
        crear_indice_socios_desde_arch_maestro(&indice_socios, "socios.dat");
    }


    //    printf("\n\nMOSTRANDO ARBOL:\n\n");
//    ind_recorrer(&indice_socios,mostrar_clave,NULL); // TESTING
    //


    getchar();
    menu(&indice_socios, "socios.dat");



    leer_archivo_binario("socios.dat");
    ind_grabar(&indice_socios,"socios.idx");
    ind_vaciar(&indice_socios);
    //test_leer_archivo_idx("socios.idx");

}


