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
                pa = &(*pa)->izq;
            else if(rc > 0)
                pa = &(*pa)->der;
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



int cargar_desde_set_ordenado_a_arbol(t_arbol *pa, void *set_datos, int li, int ls, unsigned (*leer)(void **,void*,unsigned, void *params),
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

    if( (r = cargar_desde_set_ordenado_a_arbol(&(*pa)->izq,set_datos,li,m-1,leer,params)))
    {
        return r;
    }
    return cargar_desde_set_ordenado_a_arbol(&(*pa)->der,set_datos,m+1,ls,leer,params);
}


unsigned leer_desde_arch_bin(void **info_nodo, void *pf, unsigned pos, void *params) //params es el tam_registro
{
    unsigned tam = *((int*)params);
    *info_nodo = malloc(tam);
    if(!*info_nodo)
        return 0;

    fseek((FILE*)pf, pos *tam, SEEK_SET);

    return fread(*info_nodo,tam,1,(FILE*)pf);
}

int cargar_arch_bin_ord (t_arbol *pa, const char *path, unsigned tam_dato)
{
    int cant_reg, r;
    FILE *pf;
    if(*pa)
        return 0;
    if(!(pf=fopen(path,"rb")))
        return 0;
    fseek(pf,0L, SEEK_END);
    cant_reg=ftell(pf)/tam_dato;
    r= cargar_desde_set_ordenado_a_arbol(pa, pf, 0, cant_reg-1, leer_desde_arch_bin, &tam_dato);
    fclose(pf);
    return r;
}

void recorrer_en_orden(t_arbol *pa, unsigned n, void *params, void (*accion)(const void *, unsigned, void *))
{
    if(!*pa)
        return;

    recorrer_en_orden(&(*pa)->izq, n+1, params, accion);


    accion((*pa)->info, (*pa)->tam_info, params);

    recorrer_en_orden(&(*pa)->der, n+1, params, accion);
}




int elim_elem_arbol (t_arbol *pa, void *dato, unsigned tam_dato, int(*cmp)(const void*, const void*))
{
    if(!(pa=buscar_nodo_arbol (pa,dato,cmp)))
       return 0;
    memcpy(dato, (*pa)->info, MIN(tam_dato, (*pa)->tam_info));
    return eliminar_raiz_arbol(pa);
}

int buscar_elem_arbol (const t_arbol *pa, void *dato, unsigned tam_dato, int(*cmp)(const void*,const void*))
{
    if(!(pa=buscar_nodo_arbol(pa,dato,cmp)))
        return 0;
    memcpy(dato,(*pa)->info,MIN(tam_dato,(*pa)->tam_info));
    return 1;
}

int eliminar_raiz_arbol(t_arbol *pa)
{
    t_nodo_arbol **remp, *elim;
    if(!*pa)
        return 0;
    free((*pa)->info);
    if(!(*pa)->izq && !(*pa)->der)
    {
        free(*pa);
        *pa=NULL;
        return 1;
    }
    remp= altura_arbol(&(*pa)->izq)> altura_arbol(&(*pa)->der)?
            mayor_nodo_arbol(&(*pa)->izq): menor_nodo_arbol(&(*pa)->der);
    elim=*remp;
    (*pa)->info=elim->info;
    (*pa)->tam_info=elim->tam_info;
    *remp=elim->izq? elim->izq : elim ->der;
    free(elim);
    return 1;
}

unsigned altura_arbol (const t_arbol *pa)
{
    int hi,hd;
    if(!(*pa))
        return 0;
    hi=altura_arbol(&(*pa)->izq);
    hd=altura_arbol(&(*pa)->der);
    return (hi>hd? hi:hd)+1;
}

t_nodo_arbol ** mayor_nodo_arbol (const t_arbol *pa)
{
    if(!(*pa))
        return NULL;
    while ((*pa)->der)
        pa=&(*pa)->der;
    return (t_nodo_arbol**)pa;
}

t_nodo_arbol ** menor_nodo_arbol (const t_arbol *pa)
{
    if(!(*pa))
        return NULL;
    while ((*pa)->izq)
        pa=&(*pa)->izq;
    return (t_nodo_arbol**)pa;
}


t_nodo_arbol ** buscar_nodo_arbol (const t_arbol *pa, const void *dato,
                                   int(*cmp)(const void*, const void*))
{
    int rc;
    while(*pa && (rc=cmp(dato,(*pa)->info)))
    {
        if(rc<0)
            pa=&(*pa)->izq;
        else
            pa=&(*pa)->der;
    }
    if(!*pa)
        return NULL;
    return (t_nodo_arbol **)pa;
}


// Función recursiva en post-orden para liberar los nodos del árbol y sus mallocs internos
void vaciar_arbol_nodos(t_arbol *pa)
{
    if (!*pa) return;

    vaciar_arbol_nodos(&(*pa)->izq);
    vaciar_arbol_nodos(&(*pa)->der);

    t_reg_indice *reg = (t_reg_indice *)(*pa)->info;
    if (reg)
    {
        if (reg->clave) free(reg->clave); // Liberamos el malloc del DNI
        free(reg);                        // Liberamos el malloc del t_reg_indice
    }
    free(*pa);                            // Liberamos el nodo del árbol
    *pa = NULL;
}

void ind_vaciar (t_indice* ind)
{
    if (!ind) return;
    vaciar_arbol_nodos(&ind->arbol);
}


