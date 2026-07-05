#include "indice.h"

/** \brief
Inicializa la estructura a índice vacío y almacena en la estructura de índice
el tamaño de la clave genérica a utilizar y la función de comparación
 *
 * \param void* const
 * \param void* const
 * \return void ind_crear (t_indice* ind, size_t tam_clave, int
 *
 */
void ind_crear (t_indice* ind, size_t tam_clave, int (*cmp)(const void*, const void*))
{
    crear_arbol(&ind->arbol);
    ind->tam_clave = tam_clave;
    ind->cmp = cmp;

    // NUEVO: Reservamos la memoria contigua para nuestro buffer.
    // Tamaño total = (los bytes del DNI) + (los bytes del Número de Registro)
    ind->reg_indice = malloc(tam_clave + sizeof(unsigned));
}



/** \brief  Inserta en orden el registro
según la clave
 *
 * \param t_indice *
 * \param void*
 * \param unsigned
 * \return
 *
 */
int ind_insertar (t_indice* ind, void *clave, unsigned nro_reg)
{
    // 1. Pegamos la clave (el DNI) al principio de nuestro buffer
    memcpy(ind->reg_indice, clave, ind->tam_clave);

    // 2. Pegamos el número de registro justo a la derecha del DNI
    // ¿Cómo nos movemos a la derecha? Casteamos a char* (para movernos de a 1 byte)
    // y le sumamos el tamaño de la clave.
    memcpy((char*)ind->reg_indice + ind->tam_clave, &nro_reg, sizeof(unsigned));

    // 3. Calculamos el tamaño total del paquete
    int tam_total = ind->tam_clave + sizeof(unsigned);

    // 4. Mandamos el paquete entero al árbol genérico
    return insentar_en_arbol(&ind->arbol, ind->reg_indice, tam_total, ind->cmp);
}



/** \brief
 Graba un archivo binario ordenado
(típicamente de extensión ‘.idx’) con el contenido del índice con la estructura clave-nro_reg.

 *
 * \param const t_indice* ind
 * \param const char *path
 * \return int
 *
 */
// Ya no necesitamos la estructura extraña t_ctx_grabar_indice.
// Pasamos el FILE* directamente por params.


int ind_grabar(const t_indice* ind, const char* path)
{
    FILE *fp = fopen(path, "wb");
    if (!fp) return 0;

    // Recorremos y le pasamos el archivo (fp) como parámetro (mochila)
    recorrer_en_orden((t_arbol *)&ind->arbol, 0, fp, accion_grabar_nodo_idx);

    fclose(fp);
    return 1;
}


void accion_grabar_nodo_idx(const void *info_nodo, unsigned tam_info, void *params)
{
    FILE *fp = (FILE *)params;

    // ¡Una sola línea! Volcamos el paquete contiguo [DNI | NRO_REG] directo al disco
    fwrite(info_nodo, tam_info, 1, fp);
}



/** \brief
 * Recorre el índice en orden y llama a acción para cada registro del mismo.
  Devuelve 1 (uno) si la operación fue exitosa y 0 (cero) en caso contrario.
 * \param  const void*
 * \param unsigned
 * \param  void*
 * \return int ind_recorrer (const t_indice* ind, void
 *
 */
int ind_recorrer (const t_indice* ind, void (*accion)(const void *, unsigned, void *), void* param)
{
    if(!ind)
        return 0;

    recorrer_en_orden((t_arbol *)&ind->arbol, 0, param, accion);

    return TODO_OK;
}



int ind_buscar(const t_indice* ind, void *clave, unsigned *nro_reg)
{
    // 1. Pegamos el DNI buscado al principio de nuestro buffer de trabajo
    memcpy(ind->reg_indice, clave, ind->tam_clave);
    int tam_total = ind->tam_clave + sizeof(unsigned);

    // 2. Le pedimos al árbol que busque ese buffer
    if(buscar_en_arbol(&ind->arbol, ind->reg_indice, tam_total, ind->cmp) == TODO_OK)
    {
        // 3. Si lo encontró, el árbol nos devolvió el bloque lleno.
        // Nos movemos hacia la derecha (saltando el DNI) para agarrar el nro_reg.
        *nro_reg = *(unsigned*)((char*)ind->reg_indice + ind->tam_clave);
        return 1;
    }
    return 0; // No existe
}


int ind_eliminar(t_indice* ind, void *clave, unsigned *nro_reg)
{
    // 1. Preparamos el buffer igual que en la búsqueda
    memcpy(ind->reg_indice, clave, ind->tam_clave);
    int tam_total = ind->tam_clave + sizeof(unsigned);

    // 2. Le pedimos al árbol que lo elimine
    if(eliminar_de_arbol(&ind->arbol, ind->reg_indice, tam_total, ind->cmp) == TODO_OK)
    {
        // 3. Si lo eliminó con éxito, extraemos qué nro_reg tenía asociado
        // para devolvérselo al Menú, así el Menú va al archivo y lo pone en estado 'B'.
        *nro_reg = *(unsigned*)((char*)ind->reg_indice + ind->tam_clave);
        return 1;
    }
    return 0;
}



void ind_vaciar(t_indice* ind)
{
    if(!ind) return;

    // Vaciamos todos los nodos del árbol
    vaciar_arbol(&ind->arbol);

    // Destruimos el buffer de trabajo que creamos en ind_crear
    if(ind->reg_indice) {
        free(ind->reg_indice);
        ind->reg_indice = NULL;
    }
}



int ind_cargar(t_indice *ind, const char *path)
{
    FILE *pf = fopen(path, "rb");
    if (!pf) return 0; // Control de seguridad por si falla la apertura

    // 1. Calculamos el tamaño del bloque contiguo (DNI + Nro_Reg)
    int tam_registro = ind->tam_clave + sizeof(unsigned);

    // 2. Calculamos cuántos registros hay en total viajando al final del archivo
    fseek(pf, 0, SEEK_END);
    long bytes_totales = ftell(pf);
    int cant_registros = bytes_totales / tam_registro;

    if (cant_registros == 0) {
        fclose(pf);
        return 0;
    }

    // 3. Invocamos la función del profesor que arma el árbol balanceado.
    // Pasamos el Límite Inferior (0) y el Límite Superior (cant_registros - 1).
    int resultado = cargar_desde_set_ordenado_a_arbol(
                        &ind->arbol,
                        pf,
                        0,
                        cant_registros - 1,
                        (unsigned (*)(void *, void *, unsigned int, void *))leer_desde_arch_bin,
                        &tam_registro
                    );

    fclose(pf);
    return resultado == TODO_OK ? 1 : 0;
}
