#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 1516
//конечный регистр в карте памяти
#define END_ADR_REGISTER 1675

int privateSDIBigGetReg2(int adrReg);

int getSDIBigModbusRegister(int);//получить содержимое регистра
int getSDIBigModbusBit(int);//получить содержимое бита
int setSDIBigModbusRegister(int, int);//получить содержимое регистра
int setSDIBigModbusBit(int, int);//получить содержимое бита

void setSDIBigCountObject(void);//записать к-во обектов
void preSDIBigReadAction(void);//action до чтения
void preSDIBigWriteAction(void);//action до записи
int  postSDIBigWriteAction(void);//action после записи

COMPONENT_OBJ *sdibigcomponent;

/**************************************/
//подготовка компонента светоиндикаторов
/**************************************/
void constructorSDIBigComponent(COMPONENT_OBJ *sdibigcomp)
{
  sdibigcomponent = sdibigcomp;

  sdibigcomponent->getModbusRegister = getSDIBigModbusRegister;//получить содержимое регистра
  sdibigcomponent->getModbusBit      = getSDIBigModbusBit;//получить содержимое бита
  sdibigcomponent->setModbusRegister = setSDIBigModbusRegister;//получить содержимое регистра
  sdibigcomponent->setModbusBit      = setSDIBigModbusBit;//получить содержимое бита

  sdibigcomponent->preReadAction   = preSDIBigReadAction;//action до чтения
  sdibigcomponent->preWriteAction  = preSDIBigWriteAction;//action до записи
  sdibigcomponent->postWriteAction = postSDIBigWriteAction;//action после записи

  sdibigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getSDIBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateSDIBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return getRangN_BIGModbusRegister(&current_settings.ranguvannja_leds[0], 8, adrReg-BEGIN_ADR_REGISTER );
}//getSDIBigModbusRegister(int adrReg)
int getSDIBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getSDIBigModbusBit(int )
int setSDIBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateSDIBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(sdibigcomponent->isActiveActualData)
    {
      edition_settings = current_settings;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
//  sdibigcomponent->isActiveActualData = 0;
  superClearActiveActualData();

  superSetOperativMarker(sdibigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер
  //проверка на допустимость
  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setSDIBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preSDIBigReadAction(void) {
//action до чтения
  sdibigcomponent->isActiveActualData = 1;
}//
void preSDIBigWriteAction(void) {
//action до записи
  sdibigcomponent->operativMarker[0] = -1;
  sdibigcomponent->operativMarker[1] = -1;//оперативный маркер
  sdibigcomponent->isActiveActualData = 1;
}//
int postSDIBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = sdibigcomponent->operativMarker[0];
  int endAdr   = sdibigcomponent->operativMarker[1];
  if(beginAdr<0) return 0;//не было записи
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_leds[0], 8, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
     upravlSchematic = 1;//флаг Rang

//      current_settings = edition_settings;//утвердить изменения

      //Відбулася зміна настройки
//      _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
      //Виставляємо признак, що на екрані треба обновити інформацію
  //    new_state_keyboard |= (1<<BIT_REWRITE);
    //  fix_change_settings_m(1, 2);//1 - запис ранжування 2 - USB
      //restart_timeout_idle_new_settings = true;

  return 0;
}//

int privateSDIBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
