
#include "header.h"

//к-во ранжир на одном dv
#define REGISTERS_DV 8

int privateDVvalidRecordLen(int recordNumber, int recordLen);
int privateDVSelUprRegisterNumber(int registerNumber);
int privateDVSelUstRegisterNumber(int registerNumber);
int privateDVSelRangRegisterNumber(int registerNumber);
int getFileDVRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileDVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileDVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

/**************************************/
//подготовка компонента DV
/**************************************/

int privateDVvalidRecordLen(int recordNumber, int recordLen)
{
 //валидация recordNumber
  if(recordNumber == 0) return MARKER_ERRORPERIMETR;
  if(recordNumber > NUMBER_INPUTS) return MARKER_ERRORPERIMETR;
 //валидация recordLen
  if(recordLen > REGISTERS_DV+5) return MARKER_ERRORPERIMETR;
  return 0;
}//privateDVvalidRecordLen

int privateDVSelUprRegisterNumber(int registerNumber)
{
  if(registerNumber == 1 || registerNumber == 2) return 1;
  return 0;
}//privateDVSelUprRegisterNumber
int privateDVSelUstRegisterNumber(int registerNumber)
{
  if(registerNumber == 3) return 1;
  return 0;
}//privateDVSelUstRegisterNumber
int privateDVSelRangRegisterNumber(int registerNumber)
{
  if(registerNumber >= 5) return 1;
  return 0;
}//privateDVSelRangRegisterNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileDVRegister20(int recordNumber, int registerNumber, int recordLen)
{
 //валидация recordLen recordNumber
 if(privateDVvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
 if(registerNumber == 0) return 0;

 if(privateDVSelUprRegisterNumber(registerNumber))
 {
  //Тип ДВх. Вид ДВх
   return GET_UPR_REGISTER000FOR20(FILEUVV_DVREGISTER2021); 
 }//if
 else if(privateDVSelUstRegisterNumber(registerNumber))
 {
  //Допуск ДВх
   return GET_UST_REGISTER000FOR20(FILEUVV_DVREGISTER2021); 
 }//if
 else if(privateDVSelRangRegisterNumber(registerNumber))
 {
  //Ранжирование ДВх.
  return getRangN_SMALLModbusRegister2021(&current_settings_interfaces.ranguvannja_inputs[(recordNumber-1)*N_SMALL],
                                         registerNumber-5);
 }//if

 if(registerNumber < 5) return 0;
 return MARKER_ERRORPERIMETR;
}//getFileDVRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileDVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
 //валидация recordLen recordNumber
 if(privateDVvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

 for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
 {
   if(registerNumber == 0) continue;

   uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

   if(privateDVSelUprRegisterNumber(registerNumber))
   {
     //логические проверки
     if(!(value==0 || value==1)) return MARKER_ERRORDIAPAZON;
   }//if
   else if(privateDVSelUstRegisterNumber(registerNumber))
   {
    //Допуск ДВх
    if(SET_UST_REGISTER000FOR21(FILEUVV_DVREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
   }//if
   else if(privateDVSelRangRegisterNumber(registerNumber))
   {
    //Ранжирование1 ДВых.
     if(validN_SMALLACMD(value) != 0) return MARKER_ERRORDIAPAZON;
   }//if
 }//for
 return 0;
}//setFileDVRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileDVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//флаг Rang
 for(int registerNumber=0; registerNumber<recordLen; registerNumber++)//номера регистров
 {
   if(registerNumber == 0) continue;
   uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

   if(privateDVSelUprRegisterNumber(registerNumber))
   {
    //Тип ДВых.01 Тип ДВых.01 (модиф.)
     int errorV = POST_UPR_REGISTER000FOR21(FILEUVV_DVREGISTER2021);

     if(errorV == 1) continue;
     if(errorV != 0) return errorV;
   }//if
   else if(privateDVSelUstRegisterNumber(registerNumber))
   {
    //Допуск ДВх
     int errorV = POST_UST_REGISTER000FOR21(FILEUVV_DVREGISTER2021);
     if(errorV == 1) continue;
     if(errorV != 0) return errorV;
   }//if
   else return MARKER_ERRORPERIMETR;
 }//for

 if(recordLen>=5)
 {
   writeRangN_SMALLModbusRegister2021(&edition_settings.ranguvannja_inputs[(recordNumber-1)*N_SMALL], &dataPacket[5], recordLen-5);
   //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
   upravlSchematic = 1;//флаг Rang
 }//if

 return 0;
}//postFileDVRegister21
