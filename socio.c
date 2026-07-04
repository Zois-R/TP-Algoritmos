#include "socio.h"

/**
 (A) Dar de alta un nuevo socio solicitando todos los campos de la estructura socios. Verificar
previamente que el DNI ingresado no exista anteriormente en el club con estado distinto a ‘I’.
Cabe destacar que cualquier registro con estado ‘I’ para el programa no existe, y si se da de alta
un nuevo Socio que ya existía, pero como estado ‘I’, debe ser dado de alta como un nuevo socio.
Insertar al nuevo socio en el índice
*/



int dar_alta_nuevo_socio(t_indice *pi, t_socio *socio, const char* path_archivo)
{
    FILE *pf;
    unsigned nro_reg_nuevo;

    // 1. Abrimos el archivo en modo "ab" (Append Binary) para escribir al final
    pf = fopen(path_archivo, "ab");
    if (!pf)
        return 0; // Error al abrir el archivo

    // 2. Calculamos qué número de registro le toca (la posición física)
    fseek(pf, 0, SEEK_END);
    nro_reg_nuevo = ftell(pf) / sizeof(t_socio);

    // 3. Escribimos el socio físicamente en el archivo
    fwrite(socio, sizeof(t_socio), 1, pf);
    fclose(pf);

    // 4. Actualizamos el árbol en memoria RAM
    ind_insertar(pi, &socio->dni, nro_reg_nuevo);

    return TODO_OK;
}


int actualizar_socio(unsigned nro_reg, t_socio *socio, const char* path)
{
    FILE *pf;

    // 1. Abrimos el archivo en modo "r+b" (Lectura y Escritura Binaria).
    // OJO: Es vital no usar "wb" (porque borra todo el archivo)
    // ni "ab" (porque siempre escribiría al final ignorando el fseek).
    pf = fopen(path, "r+b");

    if(!pf) {
        return 0; // Falló la apertura del archivo
    }

    // 2. Calculamos el salto en bytes desde el principio del archivo (SEEK_SET)
    // Fórmula: posición_lógica * tamaño_de_cada_registro
    fseek(pf, nro_reg * sizeof(t_socio), SEEK_SET);

    // 3. Sobreescribimos el registro exacto con la estructura modificada
    fwrite(socio, sizeof(t_socio), 1, pf);

    fclose(pf);

    return 1; // TODO_OK
}



int dar_baja_socio(t_indice *pi, long dni, const char *path)
{
    unsigned nro_reg;
    t_socio socio;

    // 1. Intentamos eliminar el DNI del índice.
    // Si existe, la primitiva nos devuelve el nro_reg por puntero.
    if (ind_eliminar(pi, &dni, &nro_reg) == 0)
    {
        return 0; // No existía en el índice (o ya estaba de baja)
    }

    // 2. Si el índice lo eliminó con éxito, usamos ese nro_reg para traerlo del disco
    if (!leer_socio(nro_reg, &socio, path))
    {
        return 0; // Error físico de lectura
    }

    // 3. Modificamos el estado a 'B'
    socio.estado = 'B';

    // 4. Obtenemos la fecha de hoy del sistema usando <time.h>
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    socio.fecha_baja.dia = tm.tm_mday;
    socio.fecha_baja.mes = tm.tm_mon + 1;      // Los meses van de 0 a 11
    socio.fecha_baja.anio = tm.tm_year + 1900; // Los años cuentan desde 1900

    // 5. Sobreescribimos el socio con sus nuevos datos en su posición original
    if (!actualizar_socio(nro_reg, &socio, path))
    {
        return 0; // Error físico de escritura
    }

    return 1; // Baja exitosa
}


void accion_listar_socio(const void *info_nodo, unsigned tam_info, void *params)
{
    t_reg_indice *reg = (t_reg_indice *)info_nodo;
    const char *path_archivo = (const char *)params; // Pasamos la ruta del archivo por parámetro
    t_socio socio;

    // Usamos la función leer_socio que armamos antes para traer los datos del disco
    if (leer_socio(reg->nro_registro, &socio, path_archivo))
    {
        printf("DNI: %-9ld | %-15s, %-15s | Cat: %-10s | Estado: %c\n",
               socio.dni, socio.apellidos, socio.nombres, socio.categoria, socio.estado);
    }
}


int compactar_y_reindexar(t_indice *pi, const char *path)
{
    FILE *pf_orig = fopen(path, "rb");
    if (!pf_orig) return 0;

    FILE *pf_tmp = fopen("socios.tmp", "wb");
    if (!pf_tmp)
    {
        fclose(pf_orig);
        return 0;
    }

    t_socio socio;
    // 1. Copiamos al temporal solo los que NO están dados de baja
    while (fread(&socio, sizeof(t_socio), 1, pf_orig) == 1)
    {
        if (socio.estado != 'B')
        {
            fwrite(&socio, sizeof(t_socio), 1, pf_tmp);
        }
    }

    fclose(pf_orig);
    fclose(pf_tmp);

    // 2. Reemplazamos el archivo viejo por el compactado
    remove(path);
    rename("socios.tmp", path);

    // 3. Limpiamos el índice de la memoria RAM y lo volvemos a generar desde cero
    ind_vaciar(pi);
    crear_indice_socios_desde_arch_maestro(pi, path);

    return 1;
}


typedef struct
{
    FILE* fp;
    size_t tam_clave;
} t_ctx_grabar_indice;

void accion_grabar_nodo_idx(const void *info_nodo, unsigned tam_info, void *params)
{
    t_reg_indice *reg = (t_reg_indice *)info_nodo;
    t_ctx_grabar_indice *ctx = (t_ctx_grabar_indice *)params;

    // Grabamos secuencialmente los bytes del DNI y luego el unsigned del nro_registro
    fwrite(reg->clave, ctx->tam_clave, 1, ctx->fp);
    fwrite(&reg->nro_registro, sizeof(unsigned), 1, ctx->fp);
}

int ind_grabar (const t_indice* ind, const char* path)
{
    FILE *fp = fopen(path, "wb");
    if (!fp) return 0;

    t_ctx_grabar_indice ctx;
    ctx.fp = fp;
    ctx.tam_clave = ind->tam_clave;

    recorrer_en_orden((t_arbol *)&ind->arbol, 0, &ctx, accion_grabar_nodo_idx);

    fclose(fp);
    return 1; // TODO_OK
}


