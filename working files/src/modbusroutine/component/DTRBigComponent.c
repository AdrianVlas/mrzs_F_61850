#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 2220
//конечный регистр в карте памяти
#define END_ADR_REGISTER 2315

#define REGISTERS_DTR 6
#define PART_REGISTERS_DTR 4

int privateDTRBigGetReg2(int adrReg);

int getDTRBigModbusRegister(int);//получить содержимое регистра
int getDTRBigModbusBit(int);//получить содержимое бита
int setDTRBigModbusRegister(int, int);// регистра
int setDTRBigModbusBit(int, int);// бита

void setDTRBigCountObject(void);//записать к-во обектов
void preDTRBigReadAction(void);//action до чтения
void preDTRBigWriteAction(void);//action до записи
int  postDTRBigWriteAction(void);//action после записи

int validDTRN_BIGACMD(unsigned short dataReg, int actControl);
int validBazaN_BIGACMD(unsigned short dataReg, int actControl);

COMPONENT_OBJ *dtrbigcomponent;

/**************************************/
//подготовка компонента триггер
/**************************************/
void constructorDTRBigComponent(COMPONENT_OBJ *dtrbigcomp)
{
  dtrbigcomponent = dtrbigcomp;

  dtrbigcomponent->getModbusRegister = getDTRBigModbusRegister;//получить содержимое регистра
  dtrbigcomponent->getModbusBit      = getDTRBigModbusBit;//получить содержимое бита
  dtrbigcomponent->setModbusRegister = setDTRBigModbusRegister;// регистра
  dtrbigcomponent->setModbusBit      = setDTRBigModbusBit;// бита

  dtrbigcomponent->preReadAction   = preDTRBigReadAction;//action до чтения
  dtrbigcomponent->preWriteAction  = preDTRBigWriteAction;//action до записи
  dtrbigcomponent->postWriteAction = postDTRBigWriteAction;//action после записи

  dtrbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getDTRBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateDTRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;
  int idxObj = offset/(PART_REGISTERS_DTR*REGISTERS_DTR);
//поиск активного бита
  unsigned int *ranguvannja_dtr = &current_settings_interfaces.ranguvannja_set_dt_source_plus[N_BIG*idxObj]; //Ранжування прямих
  if(((offset%(PART_REGISTERS_DTR*REGISTERS_DTR))/REGISTERS_DTR)==1)
       ranguvannja_dtr = &current_settings_interfaces.ranguvannja_set_dt_source_minus[N_BIG*idxObj]; //Ранжування інверсних 
  if(((offset%(PART_REGISTERS_DTR*REGISTERS_DTR))/REGISTERS_DTR)==2)
       ranguvannja_dtr = &current_settings_interfaces.ranguvannja_reset_dt_source_plus[N_BIG*idxObj]; //Ранжування 
  if(((offset%(PART_REGISTERS_DTR*REGISTERS_DTR))/REGISTERS_DTR)==3)
       ranguvannja_dtr = &current_settings_interfaces.ranguvannja_reset_dt_source_minus[N_BIG*idxObj]; //Ранжування 

  int bit = getSequenceN_BIGIndexActiveBit(offset%REGISTERS_DTR, ranguvannja_dtr);//индекс активного бита
  if(bit!=-1)
    {
      int adr = decoderN_BIGACMD(bit);
      return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus  для rele
    }//if(bit!=N_BIG*32)
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getDTRBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setDTRBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateDTRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(dtrbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(dtrbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  //проверка на конфиг
  if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;
  //проверка на допустимость
  return validDTRN_BIGACMD(dataReg, (adrReg-BEGIN_ADR_REGISTER)/(PART_REGISTERS_DTR*REGISTERS_DTR));
}//getDOUTBigModbusRegister(int adrReg)
int setDTRBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preDTRBigReadAction(void) {
//action до чтения
  dtrbigcomponent->isActiveActualData = 1;
}//
void preDTRBigWriteAction(void) {
//action до записи
  dtrbigcomponent->operativMarker[0] = -1;
  dtrbigcomponent->operativMarker[1] = -1;//оперативный маркер
  dtrbigcomponent->isActiveActualData = 1;
}//
int postDTRBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = dtrbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = dtrbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray

  unsigned int tmp[N_BIG];//для сохр ранжирования
  int idxObjOld=-1;//индекс суб обекта old
  //убрать старое
  for(int i=0; i<countAdr; i++)
    {
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/(PART_REGISTERS_DTR*REGISTERS_DTR);

      unsigned int *ranguvannja_dtr = &edition_settings.ranguvannja_set_dt_source_plus[N_BIG*idxObj]; //Ранжування прямих
      if(((offset%(PART_REGISTERS_DTR*REGISTERS_DTR))/REGISTERS_DTR)==1)
          ranguvannja_dtr = &edition_settings.ranguvannja_set_dt_source_minus[N_BIG*idxObj]; //Ранжування інверсних 
      if(((offset%(PART_REGISTERS_DTR*REGISTERS_DTR))/REGISTERS_DTR)==2)
          ranguvannja_dtr = &edition_settings.ranguvannja_reset_dt_source_plus[N_BIG*idxObj]; //Ранжування 
      if(((offset%(PART_REGISTERS_DTR*REGISTERS_DTR))/REGISTERS_DTR)==3)
          ranguvannja_dtr = &edition_settings.ranguvannja_reset_dt_source_minus[N_BIG*idxObj]; //Ранжування 

      if((offset/REGISTERS_DTR)!=idxObjOld) {
       idxObjOld = offset/REGISTERS_DTR;
       for(int dx=0; dx<N_BIG; dx++) tmp[dx]=ranguvannja_dtr[dx];//сохр старое ранж
      }//if

      int bitOld = getSequenceN_BIGIndexActiveBit(offset%REGISTERS_DTR, tmp);//индекс активного бита
      if(bitOld!=-1) {
        ranguvannja_dtr[bitOld/32] &= ~(1<<(bitOld%32));
      }//if
    }//for
  //добавить новое
  for(int i=0; i<countAdr; i++)
    {
      unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//новое значение
      if(adr==0) continue;
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/(PART_REGISTERS_DTR*REGISTERS_DTR);

      unsigned int *ranguvannja_dtr = &edition_settings.ranguvannja_set_dt_source_plus[N_BIG*idxObj]; //Ранжування прямих
      if(((offset%(PART_REGISTERS_DTR*REGISTERS_DTR))/REGISTERS_DTR)==1)
          ranguvannja_dtr = &edition_settings.ranguvannja_set_dt_source_minus[N_BIG*idxObj]; //Ранжування інверсних 
      if(((offset%(PART_REGISTERS_DTR*REGISTERS_DTR))/REGISTERS_DTR)==2)
          ranguvannja_dtr = &edition_settings.ranguvannja_reset_dt_source_plus[N_BIG*idxObj]; //Ранжування 
      if(((offset%(PART_REGISTERS_DTR*REGISTERS_DTR))/REGISTERS_DTR)==3)
          ranguvannja_dtr = &edition_settings.ranguvannja_reset_dt_source_minus[N_BIG*idxObj]; //Ранжування 
      int bit = encoderN_BIGACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для реле
      if(bit!=-1) {
        ranguvannja_dtr[bit/32] |= (1<<(bit%32));
      }//if
    }//for

      //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
     upravlSchematic = 1;//флаг Rang

  return 0;
}//

int privateDTRBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int validDTRN_BIGACMD(unsigned short dataReg, int actControl)
{
 return validBazaN_BIGACMD(dataReg, actControl+1);
}//validDTRN_BIGACMD(unsigned short dataReg, int actControl)
