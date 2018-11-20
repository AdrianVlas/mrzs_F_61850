
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 100
//начальный bit в карте памяти
#define BEGIN_ADR_BIT 100

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 1) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   
//конечный регистр в карте памяти
#define END_ADR_REGISTER 100
//конечный bit в карте памяти
#define END_ADR_BIT 115
#endif

#if (MODYFIKACIA_VERSII_PZ == 2)
//конечный регистр в карте памяти
#define END_ADR_REGISTER 100
//конечный bit в карте памяти
#define END_ADR_BIT 108
#endif

int privateDOUTSmallGetReg2(int adrReg);
int privateDOUTSmallGetBit2(int adrBit);

int getDOUTSmallModbusRegister(int);//получить содержимое регистра
int getDOUTSmallModbusBit(int);//получить содержимое бита
int setDOUTSmallModbusRegister(int, int);//записать регистр
int setDOUTSmallModbusBit(int, int);//записать бит

void setDOUTSmallCountObject(void);//записать к-во обектов

void preDOUTSmallReadAction(void);//action до чтения
void preDOUTSmallWriteAction(void);//action до записи
int  postDOUTSmallWriteAction(void);//action после записи

COMPONENT_OBJ *doutsmallcomponent;

/**************************************/
//подготовка компонента ДВ
/**************************************/
void constructorDOUTSmallComponent(COMPONENT_OBJ *doutcomp)
{
  doutsmallcomponent = doutcomp;

  doutsmallcomponent->getModbusRegister = getDOUTSmallModbusRegister;//получить содержимое регистра
  doutsmallcomponent->getModbusBit      = getDOUTSmallModbusBit;//получить содержимое бита
  doutsmallcomponent->setModbusRegister = setDOUTSmallModbusRegister;// регистра
  doutsmallcomponent->setModbusBit      = setDOUTSmallModbusBit;// бита

  doutsmallcomponent->preReadAction   = preDOUTSmallReadAction;//action до чтения
  doutsmallcomponent->preWriteAction  = preDOUTSmallWriteAction;//action до записи
  doutsmallcomponent->postWriteAction = postDOUTSmallWriteAction;//action после записи
 
  doutsmallcomponent->isActiveActualData = 0;
}//constructorDOUTSmallComponent(COMPONENT_OBJ *doutcomp)

int getDOUTSmallModbusRegister(int adrReg) {
  //получить содержимое регистра
  if(privateDOUTSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return  (state_outputs) &0xFFFF;
}//getDOUTModbusRegister(int adrReg)
int getDOUTSmallModbusBit(int adrBit) {
  //получить содержимое bit
  if(privateDOUTSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  short tmp   = state_outputs;
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDOUTModbusBit(int adrReg)
int setDOUTSmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTModbusRegister(int adrReg)
int setDOUTSmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTModbusRegister(int adrReg)

void preDOUTSmallReadAction(void) {
//action до чтения
  doutsmallcomponent->isActiveActualData = 1;
}//
void preDOUTSmallWriteAction(void) {
//action до записи
  doutsmallcomponent->operativMarker[0] = -1;
  doutsmallcomponent->operativMarker[1] = -1;//оперативный маркер
  doutsmallcomponent->isActiveActualData = 1;
}//
int postDOUTSmallWriteAction(void) {
//action после записи
  return 0;
}//

int privateDOUTSmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateDOUTSmallGetBit2(int adrBit)
{
  //проверить внешний периметр
 return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)
