#ifndef __GLOBAL_EXTERNAL_M_H
#define __GLOBAL_EXTERNAL_M_H

#include "componentdef.h"
#include "componentenum.h"
#include "prototyps.h"

extern  COMPONENT_OBJ config_array[];
extern  short tempReadArray[];
extern  short tempWriteArray[];
extern  int   indexTW;//индекс буфера записи
extern  int   pointInterface;//метка интерфейса 0-USB 1-RS485

//закоментируйте для боевого применения
//#define TESTZBIRKA_VERSII_PZ

#endif
