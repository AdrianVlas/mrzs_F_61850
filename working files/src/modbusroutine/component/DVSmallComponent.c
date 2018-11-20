
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 200
//начальный bit в карте памяти
#define BEGIN_ADR_BIT 200

#if (MODYFIKACIA_VERSII_PZ == 0)
//конечный регистр в карте памяти
#define END_ADR_REGISTER 201
//конечный bit в карте памяти
#define END_ADR_BIT 219
#endif

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 1) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   
//конечный регистр в карте памяти
#define END_ADR_REGISTER 200
//конечный bit в карте памяти
#define END_ADR_BIT 215
#endif

#if MODYFIKACIA_VERSII_PZ == 2    
//конечный регистр в карте памяти
#define END_ADR_REGISTER 200
//конечный bit в карте памяти
#define END_ADR_BIT 207
#endif

int privateDVSmallGetReg2(int adrReg);
int privateDVSmallGetBit2(int adrBit);

int getDVSmallModbusRegister(int);//получить содержимое регистра
int getDVSmallModbusBit(int);//получить содержимое бита
int setDVSmallModbusRegister(int, int);//записать регистр
int setDVSmallModbusBit(int, int);//записать бит

void setDVSmallCountObject(void);//записать к-во обектов

void preDVSmallReadAction(void);//action до чтения
void preDVSmallWriteAction(void);//action до записи
int  postDVSmallWriteAction(void);//action после записи

COMPONENT_OBJ *dvsmallcomponent;

/**************************************/
//подготовка компонента ДВ
/**************************************/
void constructorDVSmallComponent(COMPONENT_OBJ *dvcomp)
{
  dvsmallcomponent = dvcomp;

  dvsmallcomponent->getModbusRegister = getDVSmallModbusRegister;//получить содержимое регистра
  dvsmallcomponent->getModbusBit      = getDVSmallModbusBit;//получить содержимое бита
  dvsmallcomponent->setModbusRegister = setDVSmallModbusRegister;// регистра
  dvsmallcomponent->setModbusBit      = setDVSmallModbusBit;// бита

  dvsmallcomponent->preReadAction   = preDVSmallReadAction;//action до чтения
  dvsmallcomponent->preWriteAction  = preDVSmallWriteAction;//action до записи
  dvsmallcomponent->postWriteAction = postDVSmallWriteAction;//action после записи
 
  dvsmallcomponent->isActiveActualData = 0;
}//constructorDVSmallComponent(COMPONENT_OBJ *DVcomp)

int getDVSmallModbusRegister(int adrReg) {
  //получить содержимое регистра
  if(privateDVSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  int  input_dv = state_inputs ^ current_settings_interfaces.type_of_input;
  switch(adrReg-BEGIN_ADR_REGISTER)
  {
    case 0:
    return (input_dv) &0xFFFF;
    case 1:
    return (input_dv>>16)&0xFFFF;
  }//switch

  return 0;
}//getDVModbusRegister(int adrReg)
int getDVSmallModbusBit(int adrBit) {
  //получить содержимое bit
  if(privateDVSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int  input_dv = state_inputs ^ current_settings_interfaces.type_of_input;
  short tmp = input_dv &0xFFFF;
  if((adrBit-BEGIN_ADR_BIT)>=16) tmp = (input_dv>>16)&0xFFFF;
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDVModbusBit(int adrReg)
int setDVSmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)
int setDVSmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)

void preDVSmallReadAction(void) {
//action до чтения
  dvsmallcomponent->isActiveActualData = 1;
}//
void preDVSmallWriteAction(void) {
//action до записи
  dvsmallcomponent->operativMarker[0] = -1;
  dvsmallcomponent->operativMarker[1] = -1;//оперативный маркер
  dvsmallcomponent->isActiveActualData = 1;
}//
int postDVSmallWriteAction(void) {
//action после записи
  return 0;
}//

int privateDVSmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateDVSmallGetBit2(int adrBit)
{
  //проверить внешний периметр
 return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)
