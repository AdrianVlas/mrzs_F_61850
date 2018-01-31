#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 2316
//конечный регистр в карте памяти
#define END_ADR_REGISTER 2379

int privateVVBigGetReg2(int adrReg);

int getVVBigModbusRegister(int);//получить содержимое регистра
int getVVBigModbusBit(int);//получить содержимое бита
int setVVBigModbusRegister(int, int);// регистра
int setVVBigModbusBit(int, int);// бита

void setVVBigCountObject(void);//записать к-во обектов
void preVVBigReadAction(void);//action до чтения
void preVVBigWriteAction(void);//action до записи
int  postVVBigWriteAction(void);//action после записи

COMPONENT_OBJ *vvbigcomponent;

/**************************************/
//подготовка компонента VV
/**************************************/
void constructorVVBigComponent(COMPONENT_OBJ *vvbigcomp)
{
  vvbigcomponent = vvbigcomp;

  vvbigcomponent->getModbusRegister = getVVBigModbusRegister;//получить содержимое регистра
  vvbigcomponent->getModbusBit      = getVVBigModbusBit;//получить содержимое бита
  vvbigcomponent->setModbusRegister = setVVBigModbusRegister;// регистра
  vvbigcomponent->setModbusBit      = setVVBigModbusBit;// бита

  vvbigcomponent->preReadAction   = preVVBigReadAction;//action до чтения
  vvbigcomponent->preWriteAction  = preVVBigWriteAction;//action до записи
  vvbigcomponent->postWriteAction = postVVBigWriteAction;//action после записи

  vvbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getVVBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateVVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;
//поиск активного бита
  unsigned int *ranguvannja_vv = &current_settings.ranguvannja_on_cb[0]; //Ранжування прямих
  if((offset/32)==1)
       ranguvannja_vv = &current_settings.ranguvannja_off_cb[0]; //Ранжування інверсних 

  int bit = getSequenceN_BIGIndexActiveBit(offset%32, ranguvannja_vv);//индекс активного бита
  if(bit!=-1)
    {
      int adr = decoderN_BIGACMD(bit);
      return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus  для rele
    }//if(bit!=N_BIG*32)
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getVVBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setVVBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateVVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(vvbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
//  vvbigcomponent->isActiveActualData = 0;
  superClearActiveActualData();

  superSetOperativMarker(vvbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setVVBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preVVBigReadAction(void) {
//action до чтения
  vvbigcomponent->isActiveActualData = 1;
}//
void preVVBigWriteAction(void) {
//action до записи
  vvbigcomponent->operativMarker[0] = -1;
  vvbigcomponent->operativMarker[1] = -1;//оперативный маркер
  vvbigcomponent->isActiveActualData = 1;
}//
int postVVBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = vvbigcomponent->operativMarker[0];
  int endAdr   = vvbigcomponent->operativMarker[1];
  if(beginAdr<0) return 0;//не было записи
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray

  unsigned int tmp[N_BIG];//для сохр ранжирования
  int idxObjOld=-1;//индекс суб обекта old
  //убрать старое
  for(int i=0; i<countAdr; i++)
    {
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      unsigned int *ranguvannja_vv = &edition_settings.ranguvannja_on_cb[0]; //Ранжуванн
      if((offset/32)==1)
           ranguvannja_vv = &edition_settings.ranguvannja_off_cb[0]; //Ранжування 

      if((offset/32)!=idxObjOld) {
       idxObjOld = offset/32;
       for(int dx=0; dx<N_BIG; dx++) tmp[dx]=ranguvannja_vv[dx];//сохр старое ранж
      }//if

      int bitOld = getSequenceN_BIGIndexActiveBit(offset%32, tmp);//индекс активного бита
      if(bitOld!=-1) {
        ranguvannja_vv[bitOld/32] &= ~(1<<(bitOld%32));
      }//if
    }//for
  //добавить новое
  for(int i=0; i<countAdr; i++)
    {
      unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//новое значение
      if(adr==0) continue;
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      unsigned int *ranguvannja_vv = &edition_settings.ranguvannja_on_cb[0]; //Ранжуванн
      if((offset/32)==1)
           ranguvannja_vv = &edition_settings.ranguvannja_off_cb[0]; //Ранжування 

      int bit = encoderN_BIGACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для реле
      if(bit!=-1) {
        ranguvannja_vv[bit/32] |= (1<<(bit%32));
      }//if
    }//for

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

int privateVVBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
