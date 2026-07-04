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

                long dni_ingresado;
                unsigned nro_reg_encontrado;
                t_socio socio;

                // Limpiamos la estructura por las dudas (fundamental para la fecha de baja)
                memset(&socio, 0, sizeof(t_socio));

                printf("\n--- ALTA DE NUEVO SOCIO ---\n");
                printf("Ingrese DNI: ");
                scanf("%ld", &dni_ingresado);

                // Supongamos que tu ind_buscar devuelve 1 (TODO_OK) si lo encuentra
                if(ind_buscar(pi, &dni_ingresado, &nro_reg_encontrado) == 1)
                {
                    printf("Error: El socio con DNI %ld ya existe en el sistema.\n", dni_ingresado);
                    break; // Cortamos el case 'a' y volvemos al menu
                }
                else
                {
                    // Si llegamos acá, el DNI es válido y nuevo
                    socio.dni = dni_ingresado;

                    printf("Ingrese Apellido/s: ");
                    scanf(" %[^\n]", socio.apellidos); // El espacio antes del % limpia el buffer

                    printf("Ingrese Nombre/s: ");
                    scanf(" %[^\n]", socio.nombres);

                    printf("Ingrese Fecha de Nacimiento (DD/MM/AAAA): ");
                    scanf("%d/%d/%d", &socio.fecha_nacimiento.dia, &socio.fecha_nacimiento.mes, &socio.fecha_nacimiento.anio);

                    printf("Ingrese Sexo (F/M/O): ");
                    scanf(" %c", &socio.sexo);

                    printf("Ingrese Fecha de Afiliacion (DD/MM/AAAA): ");
                    scanf("%d/%d/%d", &socio.fecha_afiliacion.dia, &socio.fecha_afiliacion.mes, &socio.fecha_afiliacion.anio);

                    // Aca podrías meter un do-while para validar que escriban bien la categoría
                    printf("Ingrese Categoria (VITALICIO, MENOR, CADETE, ADULTO, HONORARIO, JUBILADO): ");
                    scanf(" %s", socio.categoria);

                    printf("Ingrese Fecha de ultima cuota paga (DD/MM/AAAA): ");
                    scanf("%d/%d/%d", &socio.fecha_ultima_cuota.dia, &socio.fecha_ultima_cuota.mes, &socio.fecha_ultima_cuota.anio);

                    // Forzamos el estado a Activo por defecto en el alta
                    socio.estado = 'A';

                    // La fecha de baja ya está en 0 por el memset inicial

                    // Llamamos a la funcion backend para que grabe en disco y en el arbol
                    if (dar_alta_nuevo_socio(pi, &socio, path) == 1) {
                        printf("\nSocio dado de alta exitosamente!\n");
                    } else {
                        printf("\nError al intentar guardar el socio en la base de datos.\n");
                    }
                }
            break;

            case 'm':

                long dni_buscar;
                unsigned nro_reg;
                t_socio socio;
                int sub_opcion;

                printf("\n--- MODIFICAR SOCIO ---\n");
                printf("Ingrese el DNI del socio a modificar: ");
                scanf("%ld", &dni_buscar);

                // Asumo que ind_buscar devuelve 1 (TODO_OK) si lo encuentra
                if(ind_buscar(pi, &dni_buscar, &nro_reg) == 0)
                {
                    printf("Error: No se encontro el socio o fue dado de baja.\n");
                    break;
                }

                // 1. Cargamos el socio a la memoria
                if(!leer_socio(nro_reg, &socio, path))
                {
                    printf("Error al acceder a la base de datos física.\n");
                    break;
                }

                // 2. Sub-menú de modificación
                do
                {
                    printf("\nSocio: %s, %s | DNI: %ld\n", socio.apellidos, socio.nombres, socio.dni);
                    printf("¿Qué dato desea modificar?\n");
                    printf("1. Apellido\n");
                    printf("2. Nombre\n");
                    printf("3. Categoria\n");
                    printf("4. Sexo\n");
                    printf("5. Fecha ultima cuota paga\n");
                    printf("0. Guardar y Salir\n");
                    printf("Opcion: ");
                    scanf("%d", &sub_opcion);

                    switch(sub_opcion)
                    {
                        case 1:
                            printf("Nuevo Apellido (actual: %s): ", socio.apellidos);
                            scanf(" %[^\n]", socio.apellidos);
                            break;

                        case 2:
                            printf("Nuevo Nombre (actual: %s): ", socio.nombres);
                            scanf(" %[^\n]", socio.nombres);
                            break;

                        case 3:
                        {
                            int cat_valida = 0;
                            do {
                                printf("Nueva Categoria (VITALICIO, MENOR, CADETE, ADULTO, HONORARIO, JUBILADO): ");
                                scanf(" %s", socio.categoria);

                                // Pasamos todo a mayusculas para la comparacion (requiere <ctype.h>)
                                for(int i=0; socio.categoria[i]; i++) {
                                    socio.categoria[i] = toupper(socio.categoria[i]);
                                }

                                // Validacion estricta
                                if(strcmp(socio.categoria, "VITALICIO") == 0 || strcmp(socio.categoria, "MENOR") == 0 ||
                                   strcmp(socio.categoria, "CADETE") == 0 || strcmp(socio.categoria, "ADULTO") == 0 ||
                                   strcmp(socio.categoria, "HONORARIO") == 0 || strcmp(socio.categoria, "JUBILADO") == 0) {
                                    cat_valida = 1;
                                } else {
                                    printf("Categoria incorrecta. Reintente.\n");
                                }
                            } while(cat_valida == 0);
                            break;
                        }

                        case 4:
                            do {
                                printf("Nuevo Sexo (F/M/O): ");
                                scanf(" %c", &socio.sexo);
                                socio.sexo = toupper(socio.sexo); // Por si ingresan minuscula

                                if(socio.sexo != 'F' && socio.sexo != 'M' && socio.sexo != 'O') {
                                    printf("Sexo invalido. Reintente.\n");
                                }
                            } while(socio.sexo != 'F' && socio.sexo != 'M' && socio.sexo != 'O');
                            break;

                        case 5:
                            printf("Nueva fecha cuota (actual: %02d/%02d/%04d): ",
                                    socio.fecha_ultima_cuota.dia, socio.fecha_ultima_cuota.mes, socio.fecha_ultima_cuota.anio);
                            scanf("%d/%d/%d", &socio.fecha_ultima_cuota.dia, &socio.fecha_ultima_cuota.mes, &socio.fecha_ultima_cuota.anio);
                            // Opcional: Agregar funcion que valide dias segun el mes y año bisiesto
                            break;

                        case 0:
                            break;

                        default:
                            printf("Opcion invalida.\n");
                    }
                } while(sub_opcion != 0);

                // 3. Guardamos los cambios físicos en el disco rígido
                if(actualizar_socio(nro_reg, &socio, path)) {
                    printf("\n¡Socio modificado y guardado exitosamente!\n");
                } else {
                    printf("\nError al intentar sobreescribir el socio en el disco.\n");
                }

                break;
            case 'b':
                long dni_baja;

                printf("\n--- BAJA DE SOCIO ---\n");
                printf("Ingrese el DNI del socio a dar de baja: ");
                scanf("%ld", &dni_baja);

                // Ejecutamos la función lógica de baja
                if (dar_baja_socio(pi, dni_baja, path) == 1)
                {
                    printf("\nSocio con DNI %ld dado de baja correctamente en el sistema y el indice.\n", dni_baja);
                }
                else
                {
                    printf("\nError: El socio no existe o ya se encuentra dado de baja.\n");
                }
            break;

            case 'l':

                printf("\n--- LISTADO DE SOCIOS ORDENADOS POR DNI ---\n\n");
                // Le pasamos 'path' como parámetro extra para que la acción sepa de dónde leer los datos completos
                ind_recorrer(pi, accion_listar_socio, (void*)path);
                printf("\n--------------------------------------------\n");
                break;
            case 'c':

                printf("\nCompactando base de datos y regenerando indices...\n");
                if (compactar_y_reindexar(pi, path) == 1)
                {
                    printf("¡Compactacion y Reindexacion finalizada con exito!\n");
                }
                else
                {
                    printf("Error al intentar compactar el archivo.\n");
                }
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
