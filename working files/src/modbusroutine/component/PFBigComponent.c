#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 2540
//конечный регистр в карте памяти
#define END_ADR_REGISTER 2571

int privatePFBigGetReg2(int adrReg);

int getPFBigModbusRegister(int);//получить содержимое регистра
int getPFBigModbusBit(int);//получить содержимое бита
int setPFBigModbusRegister(int, int);//получить содержимое регистра
int setPFBigModbusBit(int, int);//получить содержимое бита

void setPFBigCountObject(void);//записать к-во обектов
void prePFBigReadAction(void);//action до чтения
void prePFBigWriteAction(void);//action до записи
int  postPFBigWriteAction(void);//action после записи

COMPONENT_OBJ *pfbigcomponent;

/**************************************/
//подготовка компонента PF
/**************************************/
void constructorPFBigComponent(COMPONENT_OBJ *pfbigcomp)
{
  pfbigcomponent = pfbigcomp;

  pfbigcomponent->getModbusRegister = getPFBigModbusRegister;//получить содержимое регистра
  pfbigcomponent->getModbusBit      = getPFBigModbusBit;//получить содержимое бита
  pfbigcomponent->setModbusRegister = setPFBigModbusRegister;//получить содержимое регистра
  pfbigcomponent->setModbusBit      = setPFBigModbusBit;//получить содержимое бита

  pfbigcomponent->preReadAction   = prePFBigReadAction;//action до чтения
  pfbigcomponent->preWriteAction  = prePFBigWriteAction;//action до записи
  pfbigcomponent->postWriteAction = postPFBigWriteAction;//action после записи

  pfbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getPFBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privatePFBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;
  int bitInOut  = current_settings.ranguvannja_tf[offset/2];
  int bit = -1;
  int adr = -1;
  switch(offset%2)
  {
   case 0:
     bit = (bitInOut&0xFFFF)-1;
  if(bit!=-1)
    {
      adr = decoderN_BIGACMD(bit);
    }//if(bit!=-1)
   break;
   case 1:
     bit = ((bitInOut>>16)&0xFFFF)-1;
  if(bit!=-1)
    {
      adr = decoderN_SMALLACMD(bit);
    }//if(bit!=-1)
   break;
  }//switch

  return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus  для rele
}//getDOUTBigModbusRegister(int adrReg)
int getPFBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setPFBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privatePFBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(pfbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(pfbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  int offset = adrReg-BEGIN_ADR_REGISTER;
  switch(offset%2)
  {
   case 0:
     if(!dataReg) return 0;
     return validN_BIGACMD(dataReg);
   case 1:
     if(!dataReg) return 0;
     return validN_SMALLACMD(dataReg);
  }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setPFBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void prePFBigReadAction(void) {
//action до чтения
  pfbigcomponent->isActiveActualData = 1;
}//
void prePFBigWriteAction(void) {
//action до записи
  pfbigcomponent->operativMarker[0] = -1;
  pfbigcomponent->operativMarker[1] = -1;//оперативный маркер
  pfbigcomponent->isActiveActualData = 1;
}//
int postPFBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = pfbigcomponent->operativMarker[0];
  int endAdr   = pfbigcomponent->operativMarker[1];
  if(beginAdr<0) return 0;//не было записи
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray

  //добавить новое
  for(int i=0; i<countAdr; i++)
    {
      unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//новое значение
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;
      int bit = -1;

  switch(offset%2)
  {
   case 0:
      if(adr==0) bit = -2;
      else bit = encoderN_BIGACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для реле

      if(bit!=-1) {
         if(bit==-2) bit=-1;
         edition_settings.ranguvannja_tf[offset/2] &= (uint32_t)~0xffff;
         edition_settings.ranguvannja_tf[offset/2] |= (bit+1);
      }//if
   break;

   case 1:
      if(adr==0) bit = -2;
      else bit = encoderN_SMALLACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для dv

      if(bit!=-1) {
         if(bit==-2) bit=-1;
         edition_settings.ranguvannja_tf[offset/2] &= (uint32_t)~(0xffff<<16);
         edition_settings.ranguvannja_tf[offset/2] |= ((bit+1)<<16);//
      }//if
   break;
  }//switch

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

int privatePFBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
