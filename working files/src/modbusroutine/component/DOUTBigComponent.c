
#include "header.h"

//начальный регистр в карте пам€ти
#define BEGIN_ADR_REGISTER 1100
#define REGISTERS_OUTPUTS 16

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 1) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   
//конечный регистр в карте пам€ти
#define END_ADR_REGISTER 1355
#endif

#if (MODYFIKACIA_VERSII_PZ == 2)
//конечный регистр в карте пам€ти
#define END_ADR_REGISTER 1243
#endif

int privateDOUTBigGetReg2(int adrReg);

int getDOUTBigModbusRegister(int);//получить содержимое регистра
int getDOUTBigModbusBit(int);//получить содержимое бита
int setDOUTBigModbusRegister(int, int);// регистра
int setDOUTBigModbusBit(int, int);// бита

void preDOUTBigReadAction(void);//action до чтени€
void preDOUTBigWriteAction(void);//action до записи
int  postDOUTBigWriteAction(void);//action после записи

COMPONENT_OBJ *doutbigcomponent;

/**************************************/
//подготовка компонента rele
/**************************************/
void constructorDOUTBigComponent(COMPONENT_OBJ *doutcomp)
{
  doutbigcomponent = doutcomp;

  doutbigcomponent->getModbusRegister = getDOUTBigModbusRegister;//получить содержимое регистра
  doutbigcomponent->getModbusBit      = getDOUTBigModbusBit;//получить содержимое бита
  doutbigcomponent->setModbusRegister = setDOUTBigModbusRegister;// регистра
  doutbigcomponent->setModbusBit      = setDOUTBigModbusBit;// бита

  doutbigcomponent->preReadAction   = preDOUTBigReadAction;//action до чтени€
  doutbigcomponent->preWriteAction  = preDOUTBigWriteAction;//action до записи
  doutbigcomponent->postWriteAction = postDOUTBigWriteAction;//action после записи

  doutbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getDOUTBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateDOUTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_outputs[0], REGISTERS_OUTPUTS, adrReg-BEGIN_ADR_REGISTER );
}//getDOUTBigModbusRegister(int adrReg)
int getDOUTBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setDOUTBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateDOUTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(doutbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(doutbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер
  //проверка на допустимость
  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setDOUTBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preDOUTBigReadAction(void)
{
//action до чтени€
  doutbigcomponent->isActiveActualData = 1;
}//
void preDOUTBigWriteAction(void)
{
//action до записи
  doutbigcomponent->operativMarker[0] = -1;
  doutbigcomponent->operativMarker[1] = -1;//оперативный маркер
  doutbigcomponent->isActiveActualData = 1;
}//
int postDOUTBigWriteAction(void)
{
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = doutbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = doutbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_outputs[0], REGISTERS_OUTPUTS, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //¬јЋ»ƒј÷»я ”—ѕ≈ЎЌј - ”—“јЌќ¬ ј
     upravlSchematic = 1;//флаг Rang

  return 0;
}//

int privateDOUTBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
