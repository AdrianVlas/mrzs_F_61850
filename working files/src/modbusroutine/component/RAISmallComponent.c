
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 409
//конечный регистр в карте памяти
#define END_ADR_REGISTER 768

int getRAISmallModbusRegister(int);//получить содержимое регистра
int getRAISmallModbusBit(int);//получить содержимое бита
int setRAISmallModbusRegister(int, int);//получить содержимое регистра
int setRAISmallModbusBit(int, int);//получить содержимое бита

void preRAISmallReadAction(void);//action до чтения
void preRAISmallWriteAction(void);//action до записи
int  postRAISmallWriteAction(void);//action после записи

int privateRAISmallGetReg2(int adrReg);

COMPONENT_OBJ *raismallcomponent;

/**************************************/
//компонент измерений в момент аварии
/**************************************/
void constructorRAISmallComponent(COMPONENT_OBJ *raismallcomp)
{
  raismallcomponent = raismallcomp;

  raismallcomponent->getModbusRegister = getRAISmallModbusRegister;//получить содержимое регистра
  raismallcomponent->getModbusBit      = getRAISmallModbusBit;//получить содержимое бита
  raismallcomponent->setModbusRegister = setRAISmallModbusRegister;//получить содержимое регистра
  raismallcomponent->setModbusBit      = setRAISmallModbusBit;//получить содержимое бита

  raismallcomponent->preReadAction   = preRAISmallReadAction;//action до чтения
  raismallcomponent->preWriteAction  = preRAISmallWriteAction;//action до записи
  raismallcomponent->postWriteAction = postRAISmallWriteAction;//action после записи

  raismallcomponent->isActiveActualData = 0;
}//constructorIUSmallComponent(COMPONENT_OBJ *iucomp)

int getRAISmallModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateRAISmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
    switch ((adrReg-BEGIN_ADR_REGISTER)%35)
    {
    case 407-BEGIN_ADR_REGISTER://IA
      {
        return 0x55;
      }
    case 408-BEGIN_ADR_REGISTER://IB
      {
        return 0x55;
      }
    case 409-BEGIN_ADR_REGISTER://IC
      {
        return 0x55;
      }
    case 411-BEGIN_ADR_REGISTER://Uab
      {
        return 0x55;
      }
    case 414-BEGIN_ADR_REGISTER://3U0
      {
        return 0x55;
      }
    case 415-BEGIN_ADR_REGISTER://I
      {
        return 0x55;
      }
    case 417-BEGIN_ADR_REGISTER://I
      {
        return 0x55;
      }
    case 418-BEGIN_ADR_REGISTER://I
      {
        return 0x55;
      }
    case 419-BEGIN_ADR_REGISTER://I
      {
        return 0x55;
      }
    case 420-BEGIN_ADR_REGISTER://I
      {
        return 0x55;
      }
    case 421-BEGIN_ADR_REGISTER://I
      {
        return 0x55;
      }
    case 422-BEGIN_ADR_REGISTER://I
      {
        return 0x55;
      }
    case 438-BEGIN_ADR_REGISTER://
      {
        return 0x55;
      }

    }//switch
    return 0;
}//getDVModbusRegister(int adrReg)
int getRAISmallModbusBit(int x) {
  //получить содержимое bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getIUModbusBit(int )
int setRAISmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//setIUModbusRegister(int, int)
int setRAISmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое bit
  return MARKER_OUTPERIMETR;
}//setIUModbusBit(int, int )

void preRAISmallReadAction(void) {
//action до чтения
  raismallcomponent->isActiveActualData = 1;
}//

void preRAISmallWriteAction(void) {
//action до записи
  raismallcomponent->operativMarker[0] = -1;
  raismallcomponent->operativMarker[1] = -1;//оперативный маркер
  raismallcomponent->isActiveActualData = 1;
}//
int postRAISmallWriteAction(void) {
//action после записи
  return 0;
}//

int privateRAISmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateRAISmallGetReg2(int adrReg)

