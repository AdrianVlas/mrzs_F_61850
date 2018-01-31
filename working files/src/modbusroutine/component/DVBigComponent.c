#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 1356
//конечный регистр в карте памяти
#define END_ADR_REGISTER 1515

int privateDVBigGetReg2(int adrReg);

int getDVBigModbusRegister(int);//получить содержимое регистра
int getDVBigModbusBit(int);//получить содержимое бита
int setDVBigModbusRegister(int, int);// регистра
int setDVBigModbusBit(int, int);// бита

void setDVBigCountObject(void);//записать к-во обектов
void preDVBigReadAction(void);//action до чтения
void preDVBigWriteAction(void);//action до записи
int  postDVBigWriteAction(void);//action после записи

COMPONENT_OBJ *dvbigcomponent;

/**************************************/
//подготовка компонента ДВ
/**************************************/
void constructorDVBigComponent(COMPONENT_OBJ *dvbigcomp)
{
  dvbigcomponent = dvbigcomp;

  dvbigcomponent->getModbusRegister = getDVBigModbusRegister;//получить содержимое регистра
  dvbigcomponent->getModbusBit      = getDVBigModbusBit;//получить содержимое бита
  dvbigcomponent->setModbusRegister = setDVBigModbusRegister;// регистра
  dvbigcomponent->setModbusBit      = setDVBigModbusBit;// бита

  dvbigcomponent->preReadAction   = preDVBigReadAction;//action до чтения
  dvbigcomponent->preWriteAction  = preDVBigWriteAction;//action до записи
  dvbigcomponent->postWriteAction = postDVBigWriteAction;//action после записи

  dvbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getDVBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateDVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return getRangN_SMALLModbusRegister(&current_settings.ranguvannja_inputs[0], 8, adrReg-BEGIN_ADR_REGISTER );
}//getDVBigModbusRegister(int adrReg)
int getDVBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setDVBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateDVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(dvbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
//  dvbigcomponent->isActiveActualData = 0;
  superClearActiveActualData();

  superSetOperativMarker(dvbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  return validN_SMALLACMD(dataReg);
}//setDVBigModbusRegister(int adrReg, int dataReg)
int setDVBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preDVBigReadAction(void) {
//action до чтения
  dvbigcomponent->isActiveActualData = 1;
}//
void preDVBigWriteAction(void) {
//action до записи
  dvbigcomponent->operativMarker[0] = -1;
  dvbigcomponent->operativMarker[1] = -1;//оперативный маркер
  dvbigcomponent->isActiveActualData = 1;
}//
int postDVBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = dvbigcomponent->operativMarker[0];
  int endAdr   = dvbigcomponent->operativMarker[1];
  if(beginAdr<0) return 0;//не было записи
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_SMALLModbusRegister(&edition_settings.ranguvannja_inputs[0], 8, beginAdr,
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

int privateDVBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
