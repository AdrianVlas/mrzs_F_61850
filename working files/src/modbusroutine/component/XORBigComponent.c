#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 2508
//конечный регистр в карте памяти
#define END_ADR_REGISTER 2523

int privateXORBigGetReg2(int adrReg);

int getXORBigModbusRegister(int);//получить содержимое регистра
int getXORBigModbusBit(int);//получить содержимое бита
int setXORBigModbusRegister(int, int);//получить содержимое регистра
int setXORBigModbusBit(int, int);//получить содержимое бита

void setXORBigCountObject(void);//записать к-во обектов
void preXORBigReadAction(void);//action до чтения
void preXORBigWriteAction(void);//action до записи
int  postXORBigWriteAction(void);//action после записи

COMPONENT_OBJ *xorbigcomponent;

/**************************************/
//подготовка компонента XOR
/**************************************/
void constructorXORBigComponent(COMPONENT_OBJ *xorbigcomp)
{
  xorbigcomponent = xorbigcomp;

  xorbigcomponent->getModbusRegister = getXORBigModbusRegister;//получить содержимое регистра
  xorbigcomponent->getModbusBit      = getXORBigModbusBit;//получить содержимое бита
  xorbigcomponent->setModbusRegister = setXORBigModbusRegister;//получить содержимое регистра
  xorbigcomponent->setModbusBit      = setXORBigModbusBit;//получить содержимое бита

  xorbigcomponent->preReadAction   = preXORBigReadAction;//action до чтения
  xorbigcomponent->preWriteAction  = preXORBigWriteAction;//action до записи
  xorbigcomponent->postWriteAction = postXORBigWriteAction;//action после записи

  xorbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getXORBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateXORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return getRangN_BIGModbusRegister(&current_settings.ranguvannja_d_xor[0], 2, adrReg-BEGIN_ADR_REGISTER );
}//getDOUTBigModbusRegister(int adrReg)
int getXORBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setXORBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateXORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(xorbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
//  xorbigcomponent->isActiveActualData = 0;
  superClearActiveActualData();

  superSetOperativMarker(xorbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  //проверка на допустимость
  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setXORBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //записать содержимое bit
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preXORBigReadAction(void) {
//action до чтения
  xorbigcomponent->isActiveActualData = 1;
}//
void preXORBigWriteAction(void) {
//action до записи
  xorbigcomponent->operativMarker[0] = -1;
  xorbigcomponent->operativMarker[1] = -1;//оперативный маркер
  xorbigcomponent->isActiveActualData = 1;
}//
int postXORBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = xorbigcomponent->operativMarker[0];
  int endAdr   = xorbigcomponent->operativMarker[1];
  if(beginAdr<0) return 0;//не было записи
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_d_xor[0], 2, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
     upravlSchematic = 1;//флаг Rang

//      current_settings = edition_settings;//утвердить изменения

      //Відбулася зміна настройки
  //    _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
      //Виставляємо признак, що на екрані треба обновити інформацію
    //  new_state_keyboard |= (1<<BIT_REWRITE);
      //fix_change_settings_m(1, 2);//1 - запис ранжування 2 - USB
      //restart_timeout_idle_new_settings = true;

  return 0;
}//

int privateXORBigGetReg2(int adrReg)
{
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateYustBigGetReg2(int adrReg)

