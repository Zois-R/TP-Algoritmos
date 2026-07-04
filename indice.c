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
    t_reg_indice reg;

    // 1. Guardamos el número de registro directamente en el campo de la estructura
    reg.nro_registro = nro_reg;

    // 2. Reservamos memoria ÚNICAMENTE para el tamaño de la clave (ej: sizeof(long))
    reg.clave = malloc(ind->tam_clave);
    if (!reg.clave)
        return 0; // Falló la reserva

    // 3. Copiamos los bytes de la clave original (el DNI) hacia nuestra nueva memoria
    memcpy(reg.clave, clave, ind->tam_clave);

    // 4. Ahora sí, le mandamos nuestra estructura terminada al árbol genérico
    int estado_insercion = insentar_en_arbol(&ind->arbol, &reg, sizeof(t_reg_indice), ind->cmp);

    // Si el árbol rebotó la inserción (ej: clave duplicada), liberamos la memoria que pedimos
    if (estado_insercion == 0) {
        free(reg.clave);
    }

    return estado_insercion;
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
int ind_grabar (const t_indice* ind, const char* path)
{
   return TODO_OK;


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
