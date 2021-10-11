
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 200
//начальный bit в карте памяти
#define BEGIN_ADR_BIT 20000

#define TMP_OUTPUTS (tmp = 0)

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 2) ||    \
     (MODYFIKACIA_VERSII_PZ == 4) ||    \
     (MODYFIKACIA_VERSII_PZ == 12)||    \
     (MODYFIKACIA_VERSII_PZ == 14)      \
    )   
//конечный регистр в карте памяти
#define END_ADR_REGISTER 200
//конечный bit в карте памяти
#define END_ADR_BIT 20008
#endif

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 6)     \
    )   
//конечный регистр в карте памяти
#define END_ADR_REGISTER 200
//конечный bit в карте памяти
#define END_ADR_BIT 20012
#endif

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 1) ||    \
     (MODYFIKACIA_VERSII_PZ == 3) ||    \
     (MODYFIKACIA_VERSII_PZ == 10) ||    \
     (MODYFIKACIA_VERSII_PZ == 11) ||    \
     (MODYFIKACIA_VERSII_PZ == 13)     \
    )   
//конечный регистр в карте памяти
#define END_ADR_REGISTER 200
//конечный bit в карте памяти
#define END_ADR_BIT 20015
#endif

#if (                                \
     (MODYFIKACIA_VERSII_PZ == 5) || \
     (MODYFIKACIA_VERSII_PZ == 15)   \
    )   
//конечный регистр в карте памяти
#define END_ADR_REGISTER 201
//конечный bit в карте памяти
#define END_ADR_BIT 20019

#undef TMP_OUTPUTS
#define TMP_OUTPUTS (tmp = (state_outputs_raw>>16)&0xF)
#endif

#ifdef  MODYFIKACIA_VERSII_DS

//конечный регистр в карте памяти
#define END_ADR_REGISTER 203
//конечный bit в карте памяти
#define END_ADR_BIT 20051

#undef TMP_DS
#define TMP_DS (tmp = ds&((1 << NUMBER_DS) - 1))
#endif

int privateDOUTSmallGetReg2(int adrReg);
int privateDOUTSmallGetBit2(int adrBit);

int getDOUTSmallModbusRegister(int);//получить содержимое регистра
int getDOUTSmallModbusBit(int);//получить содержимое бита
int setDOUTSmallModbusRegister(int, int);//записать регистр
int setDOUTSmallModbusBit(int, int);//записать бит

int  postDOUTSmallWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *doutsmallcomponent;

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

  doutsmallcomponent->postWriteAction = postDOUTSmallWriteAction;//action после записи
}//constructorDOUTSmallComponent(COMPONENT_OBJ *doutcomp)

int getDOUTSmallModbusRegister(int adrReg) {
  //получить содержимое регистра
  if(privateDOUTSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int tmp   = state_outputs_raw&((1<<NUMBER_SIMPLE_OUTPUTS)-1);
  switch(adrReg-BEGIN_ADR_REGISTER)
  {
   case 0: tmp &= 0xFFFF; break;
   case 1: TMP_OUTPUTS; break;
#ifdef  MODYFIKACIA_VERSII_DS
   case 3: TMP_DS; break;
#endif   
   default: tmp = 0;
  }//switch

  return tmp;
}//getDOUTModbusRegister(int adrReg)

int getDOUTSmallModbusBit(int adrBit) {
  //получить содержимое bit
  if(privateDOUTSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int tmp = state_outputs_raw&((1<<NUMBER_SIMPLE_OUTPUTS)-1);
  int offset = adrBit-BEGIN_ADR_BIT;
  switch(offset/16)
  {
   case 0: tmp &= 0xFFFF; break;
   case 1: TMP_OUTPUTS; break;
#ifdef  MODYFIKACIA_VERSII_DS
   case 3: TMP_DS; break;
#endif   
   default: tmp = 0;
  }//switch

  int maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
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
