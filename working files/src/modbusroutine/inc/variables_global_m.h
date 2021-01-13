#ifndef __GLOBAL_VARIABLES_M_H
#define __GLOBAL_VARIABLES_M_H

#include "componentdef.h"
#include "componentenum.h"
#include "prototyps.h"

 COMPONENT_OBJ config_array[TOTAL_COMPONENT];
 short tempReadArray[130];
 short tempWriteArray[MAXIMUMTW];
 int   indexTW;//индекс буфера записи
 int   pointInterface=0;//метка интерфейса 0-USB 1-RS485

#endif
