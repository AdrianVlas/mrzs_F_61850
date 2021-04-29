#ifndef __GLOBAL_VARIABLES_M_H
#define __GLOBAL_VARIABLES_M_H

#include "componentdef.h"
#include "componentenum.h"
#include "prototyps.h"

SRAM1 COMPONENT_OBJ config_array[TOTAL_COMPONENT];
SRAM1 short tempReadArray[130];
SRAM1 short tempWriteArray[MAXIMUMTW];
SRAM1 int   indexTW;//индекс буфера записи
SRAM1 int   pointInterface=0;//метка интерфейса 0-USB 1-RS485

#endif
