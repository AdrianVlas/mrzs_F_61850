#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 1676
//конечный регистр в карте памяти
#define END_ADR_REGISTER 2059

int privateMFTBigGetReg2(int adrReg);

int getMFTBigModbusRegister(int);//получить содержимое регистра
int getMFTBigModbusBit(int);//получить содержимое бита
int setMFTBigModbusRegister(int, int);//получить содержимое регистра
int setMFTBigModbusBit(int, int);//получить содержимое бита

void setMFTBigCountObject(void);//записать к-во обектов
void preMFTBigReadAction(void);//action до чтения
void preMFTBigWriteAction(void);//action до записи
int  postMFTBigWriteAction(void);//action после записи

int validMFTN_BIGACMD(unsigned short dataReg, int actControl);
int validBazaN_BIGACMD(unsigned short dataReg, int actControl);

COMPONENT_OBJ *mftbigcomponent;

/**************************************/
//подготовка компонента МФТ
/**************************************/
void constructorMFTBigComponent(COMPONENT_OBJ *mftbigcomp)
{
  mftbigcomponent = mftbigcomp;

  mftbigcomponent->getModbusRegister = getMFTBigModbusRegister;//получить содержимое регистра
  mftbigcomponent->getModbusBit      = getMFTBigModbusBit;//получить содержимое бита
  mftbigcomponent->setModbusRegister = setMFTBigModbusRegister;//получить содержимое регистра
  mftbigcomponent->setModbusBit      = setMFTBigModbusBit;//получить содержимое бита

  mftbigcomponent->preReadAction   = preMFTBigReadAction;//action до чтения
  mftbigcomponent->preWriteAction  = preMFTBigWriteAction;//action до записи
  mftbigcomponent->postWriteAction = postMFTBigWriteAction;//action после записи

  mftbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getMFTBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateMFTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  int offset = adrReg-BEGIN_ADR_REGISTER;
  int idxObj = offset/48;
//поиск активного бита
  unsigned int *ranguvannja_mft = &current_settings.ranguvannja_df_source_plus[N_BIG*idxObj]; //Ранжування прямих
  if(((offset%48)/16)==1)
       ranguvannja_mft = &current_settings.ranguvannja_df_source_minus[N_BIG*idxObj]; //Ранжування інверсних 
  if(((offset%48)/16)==2)
       ranguvannja_mft = &current_settings.ranguvannja_df_source_blk[N_BIG*idxObj]; //Ранжування блокування 

  int bit = getSequenceN_BIGIndexActiveBit(offset%16, ranguvannja_mft);//индекс активного бита
  if(bit!=-1)
    {
      int adr = decoderN_BIGACMD(bit);
      return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus  для rele
    }//if(bit!=N_BIG*32)
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getMFTBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setMFTBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateMFTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(mftbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(mftbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  //проверка на допустимость
  return validMFTN_BIGACMD(dataReg, (adrReg-BEGIN_ADR_REGISTER)/48);
}//getDOUTBigModbusRegister(int adrReg)
int setMFTBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preMFTBigReadAction(void) {
//action до чтения
  mftbigcomponent->isActiveActualData = 1;
}//
void preMFTBigWriteAction(void) {
//action до записи
  mftbigcomponent->operativMarker[0] = -1;
  mftbigcomponent->operativMarker[1] = -1;//оперативный маркер
  mftbigcomponent->isActiveActualData = 1;
}//
int postMFTBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = mftbigcomponent->operativMarker[0];
  int endAdr   = mftbigcomponent->operativMarker[1];
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

      int idxObj = offset/48;
      //поиск активного бита
      unsigned int *ranguvannja_mft = &edition_settings.ranguvannja_df_source_plus[N_BIG*idxObj]; //Ранжування прямих
      if(((offset%48)/16)==1)
             ranguvannja_mft = &edition_settings.ranguvannja_df_source_minus[N_BIG*idxObj]; //Ранжування інверсних 
      if(((offset%48)/16)==2)
             ranguvannja_mft = &edition_settings.ranguvannja_df_source_blk[N_BIG*idxObj]; //Ранжування блокування 

      if((offset/16)!=idxObjOld) {
       idxObjOld = offset/16;
       for(int dx=0; dx<N_BIG; dx++) tmp[dx]=ranguvannja_mft[dx];//сохр старое ранж
      }//if

      int bitOld = getSequenceN_BIGIndexActiveBit(offset%16, tmp);//индекс активного бита
      if(bitOld!=-1) {
        ranguvannja_mft[bitOld/32] &= ~(1<<(bitOld%32));
      }//if
    }//for
  //добавить новое
  for(int i=0; i<countAdr; i++)
    {
      unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//новое значение
      if(adr==0) continue;
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/48;
      //поиск активного бита
      unsigned int *ranguvannja_mft = &edition_settings.ranguvannja_df_source_plus[N_BIG*idxObj]; //Ранжування прямих
      if(((offset%48)/16)==1)
             ranguvannja_mft = &edition_settings.ranguvannja_df_source_minus[N_BIG*idxObj]; //Ранжування інверсних 
      if(((offset%48)/16)==2)
             ranguvannja_mft = &edition_settings.ranguvannja_df_source_blk[N_BIG*idxObj]; //Ранжування блокування 

      int bit = encoderN_BIGACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для реле
      if(bit!=-1) {
        ranguvannja_mft[bit/32] |= (1<<(bit%32));
      }//if
    }//for

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

int privateMFTBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int validMFTN_BIGACMD(unsigned short dataReg, int actControl)
{
 return validBazaN_BIGACMD(dataReg, actControl+1);
}//validOFN_BIGACMD(unsigned short dataReg, int actControl)

