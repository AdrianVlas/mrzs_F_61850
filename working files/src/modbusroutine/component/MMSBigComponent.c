#include "header.h"
#if (MODYFIKACIA_VERSII_PZ >= 10)

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 3660
//конечный регистр в карте памяти
#define END_ADR_REGISTER 3915

//к-во регистров внутри одной части MMS
#define REGISTERS_MMS 8
//к-во частей внутри одного MMS
#define PART_REGISTERS_MMS 8

int privateMMSBigGetReg2(int adrReg);

int getMMSBigModbusRegister(int);//получить содержимое регистра
int getMMSBigModbusBit(int);//получить содержимое бита
int setMMSBigModbusRegister(int, int);//получить содержимое регистра
int setMMSBigModbusBit(int, int);//получить содержимое бита

void setMMSBigCountObject(void);//записать к-во обектов
void preMMSBigReadAction(void);//action до чтения
void preMMSBigWriteAction(void);//action до записи
int  postMMSBigWriteAction(void);//action после записи

SRAM1 COMPONENT_OBJ *mmsbigcomponent;

/**************************************/
//подготовка компонента MMS
/**************************************/
void constructorMMSBigComponent(COMPONENT_OBJ *mmsbigcomp)
{
  mmsbigcomponent = mmsbigcomp;

  mmsbigcomponent->getModbusRegister = getMMSBigModbusRegister;//получить содержимое регистра
  mmsbigcomponent->getModbusBit      = getMMSBigModbusBit;//получить содержимое бита
  mmsbigcomponent->setModbusRegister = setMMSBigModbusRegister;//получить содержимое регистра
  mmsbigcomponent->setModbusBit      = setMMSBigModbusBit;//получить содержимое бита

  mmsbigcomponent->preReadAction   = preMMSBigReadAction;//action до чтения
  mmsbigcomponent->preWriteAction  = preMMSBigWriteAction;//action до записи
  mmsbigcomponent->postWriteAction = postMMSBigWriteAction;//action после записи

  mmsbigcomponent->isActiveActualData = 0;
}//

int getMMSBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateMMSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  int offset = adrReg-BEGIN_ADR_REGISTER;
  int idxObj = offset/(REGISTERS_MMS*PART_REGISTERS_MMS);

//поиск активного бита
  unsigned int *ranguvannja_mms = &current_settings_interfaces.ranguvannja_In_MMS[idxObj]
                                    [(offset%(REGISTERS_MMS*PART_REGISTERS_MMS))/REGISTERS_MMS][0]; //Ранжування 
  int bit = getSequenceN_SMALLIndexActiveBit(offset%REGISTERS_MMS, ranguvannja_mms);//индекс активного бита
  if(bit!=-1)
    {
      int adr = decoderN_SMALLACMD(bit);
      return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus  для dv
    }//if(bit!=-1)
  return 0;
}//getMMSBigModbusRegister(int adrReg)
int getMMSBigModbusBit(int x)
{
  //получить содержимое bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getMMSBigModbusBit(int adrReg)
int setMMSBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateMMSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(mmsbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(mmsbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  if(dataReg==0) return 0;//прошла валидация

  switch((unsigned short)dataReg)
  {
   case 50528://- Ввімк.ВВ      *
   case 50529://- Вимк.ВВ       *
   case 50562://- Сброс индикации
   case 50563://- Сброс реле
   case 50599://- С.блк.Гот.до ТУ

   case 50432://OF
   case 50433://OF
   case 50434://OF
   case 50435://OF
   case 50436://OF
   case 50437://OF
   case 50438://OF
   case 50439://OF
 
   case 50464://DTR
   case 50465://DTR
   case 50466://DTR
   case 50467://DTR
 
   case 50468://DTR
   case 50469://DTR
   case 50470://DTR
   case 50471://DTR

      return 0; //прошла валидация
  }//switch

  return MARKER_ERRORPERIMETR;//не прошла валидация
}//
int setMMSBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//

void preMMSBigReadAction(void) {
//action до чтения
  mmsbigcomponent->isActiveActualData = 1;
}//
void preMMSBigWriteAction(void) {
//action до записи
  mmsbigcomponent->operativMarker[0] = -1;
  mmsbigcomponent->operativMarker[1] = -1;//оперативный маркер
  mmsbigcomponent->isActiveActualData = 1;
}//
int postMMSBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = mmsbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = mmsbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray

  unsigned int tmp[N_SMALL];//для сохр ранжирования
  int idxObjOld=-1;//индекс суб обекта old
  //убрать старое
  for(int i=0; i<countAdr; i++)
    {
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/(REGISTERS_MMS*PART_REGISTERS_MMS);
      //поиск активного бита
      unsigned int *ranguvannja_mms = &edition_settings.ranguvannja_In_MMS[idxObj]
                                    [(offset%(REGISTERS_MMS*PART_REGISTERS_MMS))/REGISTERS_MMS][0]; //Ранжування 

      if((offset/REGISTERS_MMS)!=idxObjOld) {
       idxObjOld = offset/REGISTERS_MMS;
       for(int dx=0; dx<N_SMALL; dx++) tmp[dx]=ranguvannja_mms[dx];//сохр старое ранж
      }//if

      int bitOld = getSequenceN_SMALLIndexActiveBit(offset%REGISTERS_MMS, tmp);//индекс активного бита
      if(bitOld!=-1) {
        ranguvannja_mms[bitOld/32] &= ~(1<<(bitOld%32));
      }//if
    }//for
  //добавить новое
  for(int i=0; i<countAdr; i++)
    {
      unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//новое значение
      if(adr==0) continue;
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/(REGISTERS_MMS*PART_REGISTERS_MMS);
      //поиск активного бита
      unsigned int *ranguvannja_mms = &edition_settings.ranguvannja_In_MMS[idxObj]
                                    [(offset%(REGISTERS_MMS*PART_REGISTERS_MMS))/REGISTERS_MMS][0]; //Ранжування 

      int bit = encoderN_SMALLACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для dv
      if(bit!=-1) {
        ranguvannja_mms[bit/32] |= (1<<(bit%32));
      }//if
    }//for

      //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
     upravlSchematic = 1;//флаг Rang

  return 0;
}//

int privateMMSBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateMMBigGetReg2(int adrReg)
#endif
