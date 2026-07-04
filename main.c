#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int main()
{
    char path[256];
    t_indice indice_socios;


    printf("Ingrese el path (ruta de acceso) del archivo CSV de socios sin comillas ni nada: ");


    if (fgets(path, sizeof(path), stdin) != NULL)
    {

        path[strcspn(path, "\n")] = '\0';


        pasar_archivo_csv_binario(path);

        printf("Lectura finalizada. Se genero el archivo socios.dat (si la ruta era correcta).\n");
    }
    leer_archivo_binario("socios.dat"); // TESTING

    ind_crear(&indice_socios,sizeof(long),cmp_clave);

    crear_indice_socios_desde_arch_maestro(&indice_socios,"socios.dat");

//    printf("\n\nMOSTRANDO ARBOL:\n\n");
//    ind_recorrer(&indice_socios,mostrar_clave,NULL); // TESTING
//

    mostrar_menu(&indice_socios,"socios.dat");
}


