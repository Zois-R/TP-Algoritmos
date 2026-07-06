#include "arbol.h"

void crear_arbol(t_arbol *pa)
{
    *pa =NULL;
}


int insentar_en_arbol(t_arbol *pa, const void *dato,unsigned tam_dato,int (*cmp)(const void *, const void *))
{

    int rc;
    t_nodo_arbol *nue;

    while(*pa)
        {
            if( ( rc = cmp((*pa)->info,dato) ) < 0)
                pa = &(*pa)->der;
            else if(rc > 0)
                pa = &(*pa)->izq;
            else
                return CLA_DUPLICADA;
        }

    if( (nue = (t_nodo_arbol*)malloc(sizeof(t_nodo_arbol))) == NULL || (nue->info = malloc(tam_dato)) == NULL)
        {
            free(nue);
            return SIN_MEMORIA;
        }

    memcpy(nue->info,dato,tam_dato);
    nue->tam_info = tam_dato;
    nue->izq = NULL;
    nue->der = NULL;
    *pa = nue;
    return TODO_OK;


}



int cargar_desde_set_ordenado_a_arbol(t_arbol *pa, void *set_datos, int li, int ls, unsigned (*leer)(void *,void*,unsigned, void *params),
                                      void *params )
{
    int m = (li + ls)/2, r;

    if(li > ls)
        return TODO_OK;

    (*pa) = (t_nodo_arbol*)malloc(sizeof(t_nodo_arbol));
    if(!*pa || !((*pa)->tam_info = leer(&(*pa)->info,set_datos,m,params)))
    {
        free((*pa));
        return SIN_MEMORIA;
    }
    (*pa)->izq = (*pa)->der = NULL;

    if( (r = cargar_desde_set_ordenado_a_arbol(&(*pa)->izq,set_datos,li,m-1,leer,params)) != TODO_OK)
    {
        return r;
    }
    return cargar_desde_set_ordenado_a_arbol(&(*pa)->der,set_datos,m+1,ls,leer,params);
}


unsigned leer_desde_arch_bin(void **info_nodo, void *pf, unsigned pos, void *params) // params es el tam_registro
{
    unsigned tam = *((int*)params);
    *info_nodo = malloc(tam);
    if(!*info_nodo)
        return 0;

    fseek((FILE*)pf, pos * tam, SEEK_SET);


    if (fread(*info_nodo, tam, 1, (FILE*)pf) == 1)
    {
        return tam;
    }

    free(*info_nodo);
    return 0;
}


void recorrer_en_orden(t_arbol *pa, unsigned n, void *params,
                        void (*accion)(const void *, unsigned, void *))
{
    if(!*pa)
        return;

    recorrer_en_orden(&(*pa)->izq, n + 1, params, accion);


    accion((*pa)->info, (*pa)->tam_info, params);

    recorrer_en_orden(&(*pa)->der, n + 1, params, accion);
}


void recorrer_en_pre_orden(t_arbol *pa, unsigned n, void *params,
                        void (*accion)(const void *, unsigned, void *))
{
    if(!*pa)
        return;

    accion((*pa)->info, (*pa)->tam_info, params);
    recorrer_en_pre_orden(&(*pa)->izq, n + 1, params, accion);
    recorrer_en_pre_orden(&(*pa)->der, n + 1, params, accion);

}

int eliminar_de_arbol(t_arbol *pa, void* pd, size_t tam, int (*cmp)(const void*, const void*))
{
    int comp;

    while(*pa && (comp = cmp((*pa)->info, pd)))
    {
        if(comp < 0)
            pa = &(*pa)->der;
        else
            pa = &(*pa)->izq;
    }

    if(!*pa)
        return NO_EXISTE;

    memcpy(pd, (*pa)->info, MINIMO((*pa)->tam_info, tam));
    return arbol_eliminar_raiz(pa);
}

void vaciar_arbol(t_arbol *pa)
{
    if(!*pa) return;


    vaciar_arbol(&(*pa)->izq);
    vaciar_arbol(&(*pa)->der);

    free((*pa)->info);
    free(*pa);

    *pa = NULL;
}


t_arbol* arbol_mayor_nodo(t_arbol* pa)
{
    while(*pa && (*pa)->der)
        pa = &(*pa)->der;
    return pa;
}

t_arbol* arbol_menor_nodo(t_arbol* pa)
{
    while(*pa && (*pa)->izq)
        pa = &(*pa)->izq;
    return pa;
}


int altura_arbol(const t_arbol *pa)
{
    int hi, hd;

    if(!*pa)
        return 0;

    hi = altura_arbol(&(*pa)->izq);
    hd = altura_arbol(&(*pa)->der);

    return (hi > hd ? hi : hd) + 1;
}


int arbol_eliminar_raiz(t_arbol *pa)
{
    if(!*pa)
        return NO_EXISTE;

    free((*pa)->info);

    if(!(*pa)->izq && !(*pa)->der)
    {
        free(*pa);
        *pa = NULL;
        return TODO_OK;
    }


    t_arbol *reemp;
    t_nodo_arbol *elim;


    reemp = altura_arbol(&(*pa)->izq) > altura_arbol(&(*pa)->der) ? arbol_mayor_nodo(&(*pa)->izq) : arbol_menor_nodo(&(*pa)->der);

    elim = *reemp;
    (*pa)->info = elim->info;
    (*pa)->tam_info = elim->tam_info;
    *reemp = elim->izq ? elim->izq: elim->der;
    free(elim);
    return TODO_OK;
}


int buscar_en_arbol(const t_arbol *pa, void* pd, size_t tam, int (*cmp)(const void*, const void*))
{
    int comp;

    while(*pa && (comp = cmp((*pa)->info, pd)))
    {
        if(comp < 0)
            pa = &(*pa)->der;
        else
            pa = &(*pa)->izq;
    }

    if(!*pa)
        return 0;

    memcpy(pd, (*pa)->info, MINIMO((*pa)->tam_info, tam));
    return TODO_OK;
}
