#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 2524
//конечный регистр в карте памяти
#define END_ADR_REGISTER 2538

int privateNOTBigGetReg2(int adrReg);

int getNOTBigModbusRegister(int);//получить содержимое регистра
int getNOTBigModbusBit(int);//получить содержимое бита
int setNOTBigModbusRegister(int, int);//получить содержимое регистра
int setNOTBigModbusBit(int, int);//получить содержимое бита

void setNOTBigCountObject(void);//записать к-во обектов
void preNOTBigReadAction(void);//action до чтения
void preNOTBigWriteAction(void);//action до записи
int  postNOTBigWriteAction(void);//action после записи

COMPONENT_OBJ *notbigcomponent;

/**************************************/
//подготовка компонента NOT
/**************************************/
void constructorNOTBigComponent(COMPONENT_OBJ *notbigcomp)
{
  notbigcomponent = notbigcomp;

  notbigcomponent->getModbusRegister = getNOTBigModbusRegister;//получить содержимое регистра
  notbigcomponent->getModbusBit      = getNOTBigModbusBit;//получить содержимое бита
  notbigcomponent->setModbusRegister = setNOTBigModbusRegister;//получить содержимое регистра
  notbigcomponent->setModbusBit      = setNOTBigModbusBit;//получить содержимое бита

  notbigcomponent->preReadAction   = preNOTBigReadAction;//action до чтения
  notbigcomponent->preWriteAction  = preNOTBigWriteAction;//action до записи
  notbigcomponent->postWriteAction = postNOTBigWriteAction;//action после записи

  notbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getNOTBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateNOTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  return getRangN_BIGModbusRegister(&current_settings.ranguvannja_d_not[0], 1, adrReg-BEGIN_ADR_REGISTER );
}//getDOUTBigModbusRegister(int adrReg)
int getNOTBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setNOTBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateNOTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(notbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
//  notbigcomponent->isActiveActualData = 0;
  superClearActiveActualData();

  superSetOperativMarker(notbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setNOTBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preNOTBigReadAction(void) {
//action до чтения
  notbigcomponent->isActiveActualData = 1;
}//
void preNOTBigWriteAction(void) {
//action до записи
  notbigcomponent->operativMarker[0] = -1;
  notbigcomponent->operativMarker[1] = -1;//оперативный маркер
  notbigcomponent->isActiveActualData = 1;
}//
int postNOTBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = notbigcomponent->operativMarker[0];
  int endAdr   = notbigcomponent->operativMarker[1];
  if(beginAdr<0) return 0;//не было записи
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_d_not[0], 1, beginAdr,
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

int privateNOTBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
