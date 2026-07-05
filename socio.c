#include "socio.h"
#include <time.h>

void accion_listar_socio(const void *info_nodo, unsigned tam_info, void *params)
{
    const char *path_archivo = (const char *)params;
    t_socio socio;

    // Extraemos el número de registro saltando los bytes del DNI
    unsigned nro_reg = *(const unsigned*)((const char*)info_nodo + sizeof(long));

    // Usamos ese nro_reg para traer el registro físico del disco
    if (leer_socio(nro_reg, &socio, path_archivo))
    {
        printf("DNI: %-9ld | %-15s, %-15s | Cat: %-10s | Estado: %c\n",
               socio.dni, socio.apellidos, socio.nombres, socio.categoria, socio.estado);
    }
}



int leer_socio(unsigned nro_reg, t_socio *socio, const char* path)
{
    FILE *pf;

    // Abrimos en modo lectura binaria pura
    pf = fopen(path, "rb");
    if(!pf) {
        return 0; // Si falla, devolvemos 0 (falso)
    }

    // Calculamos el salto en bytes desde el principio del archivo (SEEK_SET)
    fseek(pf, nro_reg * sizeof(t_socio), SEEK_SET);

    // Leemos 1 bloque del tamaño de t_socio y lo guardamos en la estructura
    fread(socio, sizeof(t_socio), 1, pf);

    fclose(pf);

    return 1; // TODO_OK (verdadero)
}

void alta_socio(t_indice *pi, const char* path_archivo)
{
    unsigned nro_reg;
    long dni_ingresado;
    t_socio socio;
    int estado_busqueda;

    // 1. Bucle de validación del DNI (atrapamos al usuario hasta que ingrese uno válido)
    do
    {
        printf("\n--- ALTA DE NUEVO SOCIO ---\n");
        printf("Ingrese el DNI (o 0 para cancelar y volver al menu): ");
        scanf("%ld", &dni_ingresado);

        if (dni_ingresado == 0) {
            return; // El usuario se arrepintió, salimos de la función
        }

        // Buscamos si el DNI ya existe en el índice
        estado_busqueda = ind_buscar(pi, &dni_ingresado, &nro_reg);

        if (estado_busqueda == 1) { // 1 = TODO_OK / Lo encontró
            printf("Error: El DNI %ld ya se encuentra registrado y activo en el club.\n", dni_ingresado);
        }

    } while (estado_busqueda == 1);

    // 2. Si salimos del do-while, es porque el DNI es nuevo (o era una baja vieja).
    // Limpiamos la estructura por completo para que la fecha de baja quede en ceros.
    memset(&socio, 0, sizeof(t_socio));
    socio.dni = dni_ingresado;

    // Pedimos el resto de los campos
    printf("Ingrese Apellido/s: ");
    scanf(" %[^\n]", socio.apellidos);

    printf("Ingrese Nombre/s: ");
    scanf(" %[^\n]", socio.nombres);

    printf("Ingrese Fecha de Nacimiento (DD/MM/AAAA): ");
    scanf("%d/%d/%d", &socio.fecha_nacimiento.dia, &socio.fecha_nacimiento.mes, &socio.fecha_nacimiento.anio);

    printf("Ingrese Sexo (F/M/O): ");
    scanf(" %c", &socio.sexo);

    printf("Ingrese Fecha de Afiliacion (DD/MM/AAAA): ");
    scanf("%d/%d/%d", &socio.fecha_afiliacion.dia, &socio.fecha_afiliacion.mes, &socio.fecha_afiliacion.anio);

    printf("Ingrese Categoria (VITALICIO, MENOR, CADETE, ADULTO, HONORARIO, JUBILADO): ");
    scanf(" %s", socio.categoria);

    printf("Ingrese Fecha de ultima cuota paga (DD/MM/AAAA): ");
    scanf("%d/%d/%d", &socio.fecha_ultima_cuota.dia, &socio.fecha_ultima_cuota.mes, &socio.fecha_ultima_cuota.anio);

    socio.estado = 'A'; // Forzamos el estado activo

    // 3. Abrimos y guardamos físicamente en el disco
    FILE* pf;
    if (!(pf = fopen(path_archivo, "ab"))) {
        printf("Error fatal al intentar abrir la base de datos.\n");
        return;
    }

    fwrite(&socio, sizeof(t_socio), 1, pf);

    // Calculamos qué número de registro le tocó al tipo que acabamos de insertar.
    // Como los registros arrancan a contar desde 0, le restamos 1 a la cuenta total.
    nro_reg = (ftell(pf) / sizeof(t_socio)) - 1;

    fclose(pf);

    // 4. Actualizamos el índice en la memoria RAM
    ind_insertar(pi, &socio.dni, nro_reg);

    printf("\n¡Socio dado de alta exitosamente!\n");
}


void modificar_socio(t_indice *pi, const char* path_archivo)
{
    unsigned nro_reg;
    long dni_buscar;
    t_socio socio;
    int opcion;

    // 1. Validamos que el DNI exista en el índice
    do
    {
        printf("\n--- MODIFICAR SOCIO ---\n");
        printf("Ingrese el DNI del socio a modificar (o 0 para cancelar): ");
        scanf("%ld", &dni_buscar);

        if (dni_buscar == 0) {
            return; // El usuario canceló
        }

        if (ind_buscar(pi, &dni_buscar, &nro_reg) == 0) {
            printf("Error: No se encontro el socio con DNI %ld o esta dado de baja.\n", dni_buscar);
        } else {
            break; // Lo encontró, salimos del bucle
        }
    } while (1);

    // 2. Leemos el socio desde el disco rígido a la memoria RAM
    if (!leer_socio(nro_reg, &socio, path_archivo)) {
        printf("Error fatal al intentar leer el disco físico.\n");
        return;
    }

    // 3. Sub-menú de modificación con validaciones
    do
    {
        system("cls"); // Limpiamos pantalla para que quede prolijo
        printf("Socio Seleccionado: %s, %s | DNI: %ld\n", socio.apellidos, socio.nombres, socio.dni);
        printf("\n¿Qué dato desea modificar?\n");
        printf("1. Apellido/s\n");
        printf("2. Nombre/s\n");
        printf("3. Categoria\n");
        printf("4. Sexo\n");
        printf("5. Fecha de ultima cuota paga\n");
        printf("0. Guardar y Salir al Menu Principal\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch(opcion)
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

                    // Convertimos a mayúsculas para evitar errores si ingresan "menor" o "Menor"
                    for(int i = 0; socio.categoria[i]; i++) {
                        socio.categoria[i] = toupper(socio.categoria[i]);
                    }

                    if(strcmp(socio.categoria, "VITALICIO") == 0 || strcmp(socio.categoria, "MENOR") == 0 ||
                       strcmp(socio.categoria, "CADETE") == 0 || strcmp(socio.categoria, "ADULTO") == 0 ||
                       strcmp(socio.categoria, "HONORARIO") == 0 || strcmp(socio.categoria, "JUBILADO") == 0) {
                        cat_valida = 1;
                    } else {
                        printf("Error: Categoria inexistente. Reintente.\n");
                    }
                } while(cat_valida == 0);
                break;
            }

            case 4:
                do {
                    printf("Nuevo Sexo (F/M/O): ");
                    scanf(" %c", &socio.sexo);
                    socio.sexo = toupper(socio.sexo);

                    if(socio.sexo != 'F' && socio.sexo != 'M' && socio.sexo != 'O') {
                        printf("Error: Sexo invalido. Ingrese F, M u O.\n");
                    }
                } while(socio.sexo != 'F' && socio.sexo != 'M' && socio.sexo != 'O');
                break;

            case 5:
                printf("Nueva fecha de cuota (actual: %02d/%02d/%04d): ",
                        socio.fecha_ultima_cuota.dia, socio.fecha_ultima_cuota.mes, socio.fecha_ultima_cuota.anio);
                scanf("%d/%d/%d", &socio.fecha_ultima_cuota.dia, &socio.fecha_ultima_cuota.mes, &socio.fecha_ultima_cuota.anio);
                break;

            case 0:
                // 4. Cuando el usuario decide salir, pisamos el registro en el disco
                if(actualizar_socio(nro_reg, &socio, path_archivo)) {
                    printf("\n¡Cambios guardados exitosamente en la base de datos!\n");
                } else {
                    printf("\nError al intentar guardar los cambios en el disco.\n");
                }
                break;

            default:
                printf("Opcion invalida.\n");
                system("pause");
        }
    } while(opcion != 0);
}


int actualizar_socio(unsigned nro_reg, t_socio *socio, const char* path)
{
    FILE *pf = fopen(path, "r+b"); // r+b para no borrar el archivo
    if(!pf) return 0;

    fseek(pf, nro_reg * sizeof(t_socio), SEEK_SET);
    fwrite(socio, sizeof(t_socio), 1, pf);

    fclose(pf);
    return 1;
}

void baja_socio(t_indice *pi, const char* path_archivo)
{
    unsigned nro_reg;
    long dni_baja;
    t_socio socio;
    int estado_busqueda;

    // 1. Validamos que el DNI exista (no esté de baja ya)
    do {
        printf("\n--- BAJA DE SOCIO ---\n");
        printf("Ingrese el DNI a dar de baja (o 0 para cancelar): ");
        scanf("%ld", &dni_baja);

        if (dni_baja == 0) return;

        estado_busqueda = ind_buscar(pi, &dni_baja, &nro_reg);
        if (estado_busqueda == 0) {
            printf("Error: Este DNI no existe o ya fue dado de baja previamente.\n");
        }
    } while (estado_busqueda == 0);

    // 2. Modificamos el archivo físico
    FILE *pf = fopen(path_archivo, "r+b");
    if (!pf) {
        printf("Error fatal al abrir la base de datos física.\n");
        return;
    }

    // Saltamos a la posición y leemos (el fseek avanza el cabezal)
    fseek(pf, nro_reg * sizeof(t_socio), SEEK_SET);
    fread(&socio, sizeof(t_socio), 1, pf);

    // Cambiamos estado y fecha
    socio.estado = 'B';
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    socio.fecha_baja.dia = tm.tm_mday;
    socio.fecha_baja.mes = tm.tm_mon + 1;
    socio.fecha_baja.anio = tm.tm_year + 1900;

    // ¡El truco del SEEK_CUR! Retrocedemos exactamente 1 registro
    fseek(pf, (long)-sizeof(t_socio), SEEK_CUR);

    // Sobreescribimos con los datos actualizados
    fwrite(&socio, sizeof(t_socio), 1, pf);
    fclose(pf);

    // 3. Lo eliminamos del índice en RAM para que "deje de existir"
    ind_eliminar(pi, &dni_baja, &nro_reg);

    printf("\n¡Socio dado de baja exitosamente!\n");
}

void listar_socios(t_indice *pi, const char* path_archivo)
{
    system("cls");
    printf("\n===============================================================================\n");
    printf("                       LISTADO DE SOCIOS ACTIVOS                               \n");
    printf("===============================================================================\n");
    printf("| %-9s | %-15s | %-15s | %-11s | %-10s |\n", "DNI", "APELLIDOS", "NOMBRES", "CATEGORIA", "ULT. CUOTA");
    printf("-------------------------------------------------------------------------------\n");

    // Llamamos al recorrido en in-orden. Le pasamos la ruta del archivo como "mochila" (params)
    ind_recorrer(pi, accion_listar_socio, (void*)path_archivo);

    printf("===============================================================================\n");
    printf("Fin del listado.\n");
}

int compactar_y_reindexar(t_indice *pi, const char *path)
{
    FILE *pf_orig = fopen(path, "rb");
    if (!pf_orig) return 0;

    FILE *pf_tmp = fopen("socios.tmp", "wb");
    if (!pf_tmp) {
        fclose(pf_orig);
        return 0;
    }

    t_socio socio;
    while (fread(&socio, sizeof(t_socio), 1, pf_orig) == 1)
    {
        if (socio.estado != 'B') {
            fwrite(&socio, sizeof(t_socio), 1, pf_tmp);
        }
    }

    fclose(pf_orig);
    fclose(pf_tmp);

    remove(path);
    rename("socios.tmp", path);

    // Vaciamos la memoria RAM y recargamos
    ind_vaciar(pi);
    ind_crear(pi, sizeof(long), pi->cmp); // Reinicializamos el buffer
    crear_indice_socios_desde_arch_maestro(pi, path);

    return 1;
}
