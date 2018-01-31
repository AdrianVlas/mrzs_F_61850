//#include <QtWidgets>
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 1100
//конечный регистр в карте памяти
#define END_ADR_REGISTER 1355

int privateDOUTBigGetReg2(int adrReg);

int getDOUTBigModbusRegister(int);//получить содержимое регистра
int getDOUTBigModbusBit(int);//получить содержимое бита
int setDOUTBigModbusRegister(int, int);// регистра
int setDOUTBigModbusBit(int, int);// бита

void preDOUTBigReadAction(void);//action до чтения
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

  doutbigcomponent->preReadAction   = preDOUTBigReadAction;//action до чтения
  doutbigcomponent->preWriteAction  = preDOUTBigWriteAction;//action до записи
  doutbigcomponent->postWriteAction = postDOUTBigWriteAction;//action после записи

  doutbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getDOUTBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateDOUTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  return getRangN_BIGModbusRegister(&current_settings.ranguvannja_outputs[0], 16, adrReg-BEGIN_ADR_REGISTER );
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
      edition_settings = current_settings;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
//  doutbigcomponent->isActiveActualData = 0;
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
//action до чтения
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
  int endAdr   = doutbigcomponent->operativMarker[1];
  if(beginAdr<0) return 0;//не было записи
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_outputs[0], 16, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
     upravlSchematic = 1;//флаг Rang

  //    current_settings = edition_settings;//утвердить изменения

      //Відбулася зміна настройки
//      _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
      //Виставляємо признак, що на екрані треба обновити інформацію
//      new_state_keyboard |= (1<<BIT_REWRITE);
//      fix_change_settings_m(1, 2);//1 - запис ранжування 2 - USB
//      restart_timeout_idle_new_settings = true;

  return 0;
}//

int privateDOUTBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
