#ifndef SOCIO_H_INCLUDED
#define SOCIO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "indice.h"
#include "utils.h"



int dar_alta_nuevo_socio(t_indice *pi, t_socio *socio, const char* path_archivo);
int actualizar_socio(unsigned nro_reg, t_socio *socio, const char* path);



#endif // SOCIO_H_INCLUDED
