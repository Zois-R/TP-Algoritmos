#include "utils.h"



void pasar_archivo_csv_binario(const char *nombre_arch)
{
    FILE *pf_csv;
    FILE *pf_bin;

    char arch_bin[] = "socios.dat";
    char linea[150];

    t_socio socio;

    pf_csv = fopen(nombre_arch, "rt");
    if(!pf_csv)
        return;

    pf_bin = fopen(arch_bin, "wb");
    if(!pf_bin)
    {
        fclose(pf_csv);
        return;
    }

    while(fgets(linea, sizeof(linea), pf_csv))
    {
        // Limpiar la basura de la memoria en cada vuelta para las fechas vacías ¿?
        memset(&socio, 0, sizeof(t_socio));

        int leidos = sscanf(linea, "%ld,%[^,],%[^,],%d/%d/%d,%c,%d/%d/%d,%[^,],%d/%d/%d,%c,%d/%d/%d",
                           &socio.dni,
                           socio.apellidos,
                           socio.nombres,
                           &socio.fecha_nacimiento.dia,
                           &socio.fecha_nacimiento.mes,
                           &socio.fecha_nacimiento.anio,
                           &socio.sexo,
                           &socio.fecha_afiliacion.dia,
                           &socio.fecha_afiliacion.mes,
                           &socio.fecha_afiliacion.anio,
                           socio.categoria,
                           &socio.fecha_ultima_cuota.dia,
                           &socio.fecha_ultima_cuota.mes,
                           &socio.fecha_ultima_cuota.anio,
                           &socio.estado,
                           &socio.fecha_baja.dia,
                           &socio.fecha_baja.mes,
                           &socio.fecha_baja.anio);

        // Si leyó por lo menos hasta el estado (15 datos), graba en el binario
        if(leidos >= 15)
        {
            fwrite(&socio, sizeof(t_socio), 1, pf_bin);
        }
    }

    fclose(pf_csv);
    fclose(pf_bin);
}


void leer_archivo_binario(const char *nombre_arch)
{
    FILE *pf_bin;
    t_socio socio;
    int contador = 0;

    pf_bin = fopen(nombre_arch, "rb");
    if(!pf_bin)
    {
        printf("Error al abrir el archivo binario %s\n", nombre_arch);
        return;
    }

    printf("--- LEYENDO ARCHIVO BINARIO ---\n");

    // Leemos de a un registro a la vez
    while(fread(&socio, sizeof(t_socio), 1, pf_bin) == 1)
    {
        contador++;
        printf("Registro %d:\n", contador);
        printf("  DNI: %ld\n", socio.dni);
        printf("  Nombre: %s, %s\n", socio.apellidos, socio.nombres);
        printf("  F. Nac: %02d/%02d/%04d\n", socio.fecha_nacimiento.dia, socio.fecha_nacimiento.mes, socio.fecha_nacimiento.anio);
        printf("  Sexo: %c\n", socio.sexo);
        printf("  Categoria: %s\n", socio.categoria);
        printf("  Estado: %c\n", socio.estado);

        // Verificamos si tiene fecha de baja válida (distinta de 0)
        if(socio.estado == 'I' || socio.fecha_baja.anio != 0) {
            printf("  F. Baja: %02d/%02d/%04d\n", socio.fecha_baja.dia, socio.fecha_baja.mes, socio.fecha_baja.anio);
        } else {
            printf("  F. Baja: -\n");
        }

        printf("-------------------------------\n");
    }

    if(contador == 0) {
        printf("El archivo esta vacio.\n");
    } else {
        printf("Se leyeron %d registros en total.\n", contador);
    }

    fclose(pf_bin);
}


int cmp_clave(const void *info_arbol1, const void *info_arbol2)
{

    // 1. El árbol nos pasa las cajas genéricas. Nosotros sabemos que son t_reg_indice.
    t_reg_indice *reg1 = (t_reg_indice *)info_arbol1;
    t_reg_indice *reg2 = (t_reg_indice *)info_arbol2;

    // 2. Adentro del registro, la clave es un puntero void*.
    // Lo casteamos a puntero long y lo desreferenciamos para obtener el número real.
    long dni1 = *(long*)(reg1->clave);
    long dni2 = *(long*)(reg2->clave);

    // 3. Comparamos (retorna >0, <0 o 0)
    return dni1 - dni2;

}


int crear_indice_socios_desde_arch_maestro(t_indice *pi,const char *path)
{
    FILE *pf;
    t_socio socio;
    unsigned nro_reg = 0;

    pf = fopen(path,"rb");



    if(!pf)
        return ERR_ARCH;

    fread(&socio,sizeof(t_socio),1,pf);
    while(!feof(pf))
        {

            ind_insertar(pi,&socio.dni,nro_reg);
            nro_reg++;
            fread(&socio,sizeof(t_socio),1,pf);
        }

    fclose(pf);
    return TODO_OK;
}


void mostrar_menu(t_indice *pi, const char* path)
{
    char opcion;
    do
    {
        system("cls");
        printf("\nMenu:\n");
        printf("(A) Alta de nuevo socio\n");
        printf("(M) Modificar socio\n");
        printf("(B) Baja de socio\n");
        printf("(L) Listar socios ordenados\n");
        printf("(C) Compactar y Reindexar\n");
        printf("(S) Salir\n");
        printf("Seleccione una opción: ");
        scanf(" %c", &opcion);

        switch(TO_LOWER(opcion))
        {
            case 'a':

                break;
            case 'm':

                break;
            case 'b':

                break;
            case 'l':

                break;
            case 'p':

                break;
            case 's':
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción inválida. Intente nuevamente.\n");
                break;
        }
        system("pause");
    } while(TO_LOWER(opcion) != 's');
}



void mostrar_clave(const void *info_nodo, unsigned tam_info, void *params)
{

    t_reg_indice *reg = (t_reg_indice *)info_nodo;


    long dni_real = *(long*)(reg->clave);


    printf("DNI: %ld | Nro de Registro: %u\n", dni_real, reg->nro_registro);
}




//
//
//int guardar_arbol_indice_archivo_idx(t_indice *pi)
