#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 2444
//конечный регистр в карте памяти
#define END_ADR_REGISTER 2499

int privateORBigGetReg2(int adrReg);

int getORBigModbusRegister(int);//получить содержимое регистра
int getORBigModbusBit(int);//получить содержимое бита
int setORBigModbusRegister(int, int);//получить содержимое регистра
int setORBigModbusBit(int, int);//получить содержимое бита

void setORBigCountObject(void);//записать к-во обектов
void preORBigReadAction(void);//action до чтения
void preORBigWriteAction(void);//action до записи
int  postORBigWriteAction(void);//action после записи

COMPONENT_OBJ *orbigcomponent;

/**************************************/
//подготовка компонента OR
/**************************************/
void constructorORBigComponent(COMPONENT_OBJ *orbigcomp)
{
  orbigcomponent = orbigcomp;

  orbigcomponent->getModbusRegister = getORBigModbusRegister;//получить содержимое регистра
  orbigcomponent->getModbusBit      = getORBigModbusBit;//получить содержимое бита
  orbigcomponent->setModbusRegister = setORBigModbusRegister;//получить содержимое регистра
  orbigcomponent->setModbusBit      = setORBigModbusBit;//получить содержимое бита

  orbigcomponent->preReadAction   = preORBigReadAction;//action до чтения
  orbigcomponent->preWriteAction  = preORBigWriteAction;//action до записи
  orbigcomponent->postWriteAction = postORBigWriteAction;//action после записи

  orbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getORBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return getRangN_BIGModbusRegister(&current_settings.ranguvannja_d_or[0], 8, adrReg-BEGIN_ADR_REGISTER );
}//getDOUTBigModbusRegister(int adrReg)
int getORBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setORBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateORBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(orbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
//  orbigcomponent->isActiveActualData = 0;
  superClearActiveActualData();

  superSetOperativMarker(orbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  //проверка на допустимость
  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setORBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preORBigReadAction(void) {
//action до чтения
  orbigcomponent->isActiveActualData = 1;
}//
void preORBigWriteAction(void) {
//action до записи
  orbigcomponent->operativMarker[0] = -1;
  orbigcomponent->operativMarker[1] = -1;//оперативный маркер
  orbigcomponent->isActiveActualData = 1;
}//
int postORBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = orbigcomponent->operativMarker[0];
  int endAdr   = orbigcomponent->operativMarker[1];
  if(beginAdr<0) return 0;//не было записи
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_d_or[0], 8, beginAdr,
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

int privateORBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
