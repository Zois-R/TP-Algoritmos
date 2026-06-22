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


/**
Inserta en orden el registro
según la clave.


int ind_insertar (t_indice* ind, void *clave, unsigned nro_reg): inserta en orden el registro
según la clave.
*/

//int ind_insertar(t_indice* ind, void *clave, unsigned nro_reg)
//{
//    if(insentar_en_arbol(&ind->arbol,clave,ind->tam_clave,ind->cmp))
//        return TODO_OK;
//
//    return 0;
//}

//int ind_insertar(t_indice* ind, void* clave, unsigned nro_reg)
//{
//    t_reg_indice* reg;
//    size_t tam_reg;
//    int resultado;
//
//    if(!ind || !clave || !ind->cmp || ind->tam_clave == 0)
//        return IND_ERROR;
//
//    tam_reg = sizeof(t_reg_indice) + ind->tam_clave;
//
//    reg = (t_reg_indice*)malloc(tam_reg);
//    if(!reg)
//        return IND_ERROR;
//
//    reg->nro_reg = nro_reg;
//    memcpy(reg->clave, clave, ind->tam_clave);
//
//    cmp_clave_indice = ind->cmp;
//
//    resultado = insertarArbolBinBusq(
//                    &ind->arbol,
//                    reg,
//                    tam_reg,
//                    cmp_reg_indice
//                );
//
//    free(reg);
//
//    return resultado == TODO_BIEN ? IND_OK : IND_ERROR;
//}


/**
int ind_eliminar (t_indice* ind, void *clave, unsigned *nro_reg): elimina la entrada del índice
correspondiente a la clave y devuelve en nro_reg el número de registro asociado.

*/
//int ind_eliminar (t_indice* ind, void *clave, unsigned *nro_reg)
//{
//
//}


//
//
//#include "indice.h"
//
///*
//    Variable auxiliar para poder usar el comparador del índice
//    dentro del comparador que recibe el árbol.
//*/
//static int (*cmp_clave_indice)(const void*, const void*) = NULL;
//
//
///*
//    Compara dos registros de índice usando solamente la clave.
//*/
//static int cmp_reg_indice(const void* v1, const void* v2)
//{
//    const t_reg_indice* r1 = (const t_reg_indice*)v1;
//    const t_reg_indice* r2 = (const t_reg_indice*)v2;
//
//    return cmp_clave_indice(r1->clave, r2->clave);
//}
//
//
///*
//    Crea un registro temporal de índice.
//    Sirve para insertar, buscar o eliminar.
//*/
//static t_reg_indice* crear_reg_indice(const t_indice* ind,
//                                      const void* clave,
//                                      unsigned nro_reg)
//{
//    t_reg_indice* reg;
//    size_t tam_reg;
//
//    if(!ind || !clave || ind->tam_clave == 0)
//        return NULL;
//
//    tam_reg = sizeof(t_reg_indice) + ind->tam_clave;
//
//    reg = (t_reg_indice*)malloc(tam_reg);
//    if(!reg)
//        return NULL;
//
//    reg->nro_reg = nro_reg;
//    memcpy(reg->clave, clave, ind->tam_clave);
//
//    return reg;
//}
//
//
///*
//    Inicializa el índice vacío.
//*/
//void ind_crear(t_indice* ind, size_t tam_clave, int (*cmp)(const void*, const void*))
//{
//    if(!ind)
//        return;
//
//    crearArbolBinBusq(&ind->arbol);
//    ind->tam_clave = tam_clave;
//    ind->cmp = cmp;
//}
//
//
///*
//    Inserta una entrada en el índice:
//    clave + nro_reg.
//*/
//int ind_insertar(t_indice* ind, void *clave, unsigned nro_reg)
//{
//    t_reg_indice* reg;
//    size_t tam_reg;
//    int resultado;
//
//    if(!ind || !clave || !ind->cmp || ind->tam_clave == 0)
//        return IND_ERROR;
//
//    reg = crear_reg_indice(ind, clave, nro_reg);
//    if(!reg)
//        return IND_ERROR;
//
//    tam_reg = sizeof(t_reg_indice) + ind->tam_clave;
//
//    cmp_clave_indice = ind->cmp;
//
//    resultado = insertarArbolBinBusq(&ind->arbol,
//                                     reg,
//                                     tam_reg,
//                                     cmp_reg_indice);
//
//    free(reg);
//
//    return resultado == TODO_BIEN ? IND_OK : IND_ERROR;
//}
//
//
///*
//    Busca una clave en el índice.
//    Si la encuentra, devuelve el nro_reg asociado.
//*/
//int ind_buscar(const t_indice* ind, void *clave, unsigned *nro_reg)
//{
//    t_reg_indice* reg;
//    size_t tam_reg;
//    int resultado;
//
//    if(!ind || !clave || !nro_reg || !ind->cmp || ind->tam_clave == 0)
//        return IND_ERROR;
//
//    reg = crear_reg_indice(ind, clave, 0);
//    if(!reg)
//        return IND_ERROR;
//
//    tam_reg = sizeof(t_reg_indice) + ind->tam_clave;
//
//    cmp_clave_indice = ind->cmp;
//
//    resultado = buscarArbolBinBusq(&ind->arbol,                     ///FALTA PRIMITVA BUSCAR_CLAVE
//                                   reg,
//                                   tam_reg,
//                                   cmp_reg_indice);
//
//    if(resultado)
//    {
//        *nro_reg = reg->nro_reg;
//        free(reg);
//        return IND_OK;
//    }
//
//    free(reg);
//    return IND_ERROR;
//}
//
//
///*
//    Elimina una clave del índice.
//    Si la elimina, devuelve el nro_reg que tenía asociado.
//*/
//int ind_eliminar(t_indice* ind, void *clave, unsigned *nro_reg)
//{
//    t_reg_indice* reg;
//    size_t tam_reg;
//    int resultado;
//
//    if(!ind || !clave || !nro_reg || !ind->cmp || ind->tam_clave == 0)
//        return IND_ERROR;
//
//    reg = crear_reg_indice(ind, clave, 0);
//    if(!reg)
//        return IND_ERROR;
//
//    tam_reg = sizeof(t_reg_indice) + ind->tam_clave;
//
//    cmp_clave_indice = ind->cmp;
//
//    resultado = eliminarElemArbolBinBusq(&ind->arbol,
//                                         reg,
//                                         tam_reg,
//                                         cmp_reg_indice);
//
//    if(resultado)
//    {
//        *nro_reg = reg->nro_reg;
//        free(reg);
//        return IND_OK;
//    }
//
//    free(reg);
//    return IND_ERROR;
//}
//
//
///*
//    Estructura auxiliar para grabar el índice.
//*/
//typedef struct
//{
//    FILE* fp;
//    size_t tam_clave;
//    int error;
//} t_ctx_grabar_indice;
//
//
///*
//    Acción ejecutada por cada nodo del árbol al grabar.
//    Graba en formato:
//        clave
//        nro_reg
//*/
//static void accion_grabar_indice(void* info, /// ESTA FUNCIÓN TIENE EN CUENTA QUE EL ARCHIVO SE ABRÍO ANTES
//                                 unsigned tamInfo,
//                                 unsigned nivel,
//                                 void* params) /// PARA QUE SIRVE?
//{
//    t_reg_indice* reg = (t_reg_indice*)info;
//    t_ctx_grabar_indice* ctx = (t_ctx_grabar_indice*)params;
//
//    if(ctx->error)
//        return;
//
//    if(fwrite(reg->clave, ctx->tam_clave, 1, ctx->fp) != 1)
//    {
//        ctx->error = 1;
//        return;
//    }
//
//    if(fwrite(&reg->nro_reg, sizeof(unsigned), 1, ctx->fp) != 1)
//    {
//        ctx->error = 1;
//        return;
//    }
//}
//
//
///*
//    Graba el índice en un archivo binario ordenado.
//    El recorrido en orden del ABB permite guardar las claves ordenadas.
//*/
//int ind_grabar(const t_indice* ind, const char* path)
//{
//    FILE* fp;
//    t_ctx_grabar_indice ctx; /// DUDOSO
//
//    if(!ind || !path || ind->tam_clave == 0)
//        return IND_ERROR;
//
//    fp = fopen(path, "wb");
//    if(!fp)
//        return IND_ERROR;
//
//    ctx.fp = fp;
//    ctx.tam_clave = ind->tam_clave;
//    ctx.error = 0;
//
//    recorrerEnOrdenArbolBinBusq(&ind->arbol,
//                                &ctx,
//                                accion_grabar_indice);
//
//    fclose(fp);
//
//    return ctx.error ? IND_ERROR : IND_OK;
//}
//
//
///*
//    Carga el índice desde un archivo binario.
//    El archivo debe tener registros con formato:
//        clave
//        nro_reg
//*/
//int ind_cargar(t_indice* ind, const char* path)
//{
//    FILE* fp;
//    void* clave;
//    unsigned nro_reg;
//    int ok = IND_OK;
//
//    if(!ind || !path || !ind->cmp || ind->tam_clave == 0)
//        return IND_ERROR;
//
//    fp = fopen(path, "rb");
//    if(!fp)
//        return IND_ERROR;
//
//    clave = malloc(ind->tam_clave);
//    if(!clave)
//    {
//        fclose(fp);
//        return IND_ERROR;
//    }
//
//    ind_vaciar(ind);
//
//    while(fread(clave, ind->tam_clave, 1, fp) == 1)
//    {
//        if(fread(&nro_reg, sizeof(unsigned), 1, fp) != 1)
//        {
//            ok = IND_ERROR;
//            break;
//        }
//
//        if(!ind_insertar(ind, clave, nro_reg))
//        {
//            ok = IND_ERROR;
//            break;
//        }
//    }
//
//    free(clave);
//    fclose(fp);
//
//    return ok;
//}
//
//
///*
//    Vacía el índice en memoria.
//*/
//void ind_vaciar(t_indice* ind)
//{
//    if(!ind)
//        return;
//
//    vaciarArbolBinBusq(&ind->arbol);
//}
//
//
///*
//    Estructura auxiliar para recorrer el índice.
//*/
//typedef struct
//{
//    void (*accion)(const void *, unsigned, void *);
//    void* param;
//} t_ctx_recorrer_indice;
//
//
///*
//    Acción interna del árbol.
//    Traduce el nodo del árbol al callback público del índice.
//*/
//static void accion_recorrer_indice(void* info,
//                                   unsigned tamInfo,
//                                   unsigned nivel,
//                                   void* params)
//{
//    t_reg_indice* reg = (t_reg_indice*)info;
//    t_ctx_recorrer_indice* ctx = (t_ctx_recorrer_indice*)params;
//
//    ctx->accion(reg->clave, reg->nro_reg, ctx->param);
//}
//
//
///*
//    Recorre el índice en orden.
//*/
//int ind_recorrer(const t_indice* ind,
//                 void (*accion)(const void *, unsigned, void *),
//                 void* param)
//{
//    t_ctx_recorrer_indice ctx;
//
//    if(!ind || !accion)
//        return IND_ERROR;
//
//    ctx.accion = accion;
//    ctx.param = param;
//
//    recorrerEnOrdenArbolBinBusq(&ind->arbol,
//                                &ctx,
//                                accion_recorrer_indice);
//
//    return IND_OK;
//}

/** \brief
 * Recorre el índice en orden y llama a acción para cada registro del mismo.
  Devuelve 1 (uno) si la operación fue exitosa y 0 (cero) en caso contrario.
 * \param  const void*
 * \param unsigned
 * \param  void*
 * \return int ind_recorrer (const t_indice* ind, void
 *
 */
int ind_recorrer (const t_indice* ind, void (*accion)(const void *, unsigned, void *),
                  void*param)
{
    recorrer_en_orden(&ind->arbol,0,param,)
}

