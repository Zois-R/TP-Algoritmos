#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int main()
{
    char path[256];

    printf("Ingrese el path (ruta de acceso) del archivo CSV de socios sin comillas ni nada: ");


    if (fgets(path, sizeof(path), stdin) != NULL)
    {

        path[strcspn(path, "\n")] = '\0';


        pasar_archivo_csv_binario(path);

        printf("Lectura finalizada. Se genero el archivo socios.dat (si la ruta era correcta).\n");
    }
    leer_archivo_binario("socios.dat");

    /**
    Construir, además, un índice donde la clave sea el DNI del Socio, y el número de registro (unsigned
nro_reg) que le corresponde a ese socio en el archivo.
Antes de finalizar el programa, guardar el índice generado en el archivo “socios.idx”.


    */

}
