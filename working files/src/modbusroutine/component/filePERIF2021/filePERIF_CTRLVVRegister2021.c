
#include "header.h"

int privateCTRLVVvalidRecordLen(int recordNumber, int recordLen);
int privateCTRLVVSelUstRecordNumber(int recordNumber);
int privateCTRLVVSelCountGroupBitRecordNumber(int recordNumber);
int privateCTRLVVSelRang51RecordNumber(int recordNumber);
int privateCTRLVVSelRang52RecordNumber(int recordNumber);
int getFileCTRLVVRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileCTRLVVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileCTRLVVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_VV 32

int privateCTRLVVSelUstRecordNumber(int recordNumber)
{
  if((recordNumber >= 5 && recordNumber <= 8)) return 1;
  if((recordNumber >= 15 && recordNumber <= 21)) return 1;
  return 0;
}//privateCTRLVVvalidRecordNumber

int privateCTRLVVvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  if((recordNumber == 51) || (recordNumber == 52))
  {
    if(recordLen > REGISTERS_VV+1) return MARKER_ERRORPERIMETR;
  }//if
  else if(recordLen > 2) return MARKER_ERRORPERIMETR;
  return 0;
}//privateCTRLVVvalidRecordLen

int privateCTRLVVSelCountGroupBitRecordNumber(int recordNumber)
{
  if(recordNumber == 1)
    return 9;//к-сть Group біт
  return -1;
}//privateCTRLVVSelCountGroupBitRecordNumber

int privateCTRLVVSelRang51RecordNumber(int recordNumber)
{
  if(recordNumber == 51) return 1;
  return 0;
}//privateCTRLVVSelRang51RecordNumber
int privateCTRLVVSelRang52RecordNumber(int recordNumber)
{
  if(recordNumber == 52) return 1;
  return 0;
}//privateCTRLVVSelRang52RecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileCTRLVVRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateCTRLVVvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateCTRLVVSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

  if(countGroupBit > 0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPERIF_CTRLVVREGISTER2021);
  }//if(countGroupBit>0)
  else if(privateCTRLVVSelUstRecordNumber(recordNumber))
    return GET_UST_REGISTER000FOR20(FILEPERIF_CTRLVVREGISTER2021);
  else if(privateCTRLVVSelRang51RecordNumber(recordNumber))
  {
    //Ранжирование VV
    return getRangN_BIGModbusRegister2021(current_settings_interfaces.ranguvannja_on_cb,
                                          registerNumber-1);
  }//if
  else if(privateCTRLVVSelRang52RecordNumber(recordNumber))
  {
    //Ранжирование VV
    return getRangN_BIGModbusRegister2021(current_settings_interfaces.ranguvannja_off_cb,
                                          registerNumber-1);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileCTRLVVRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileCTRLVVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateCTRLVVvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit = privateCTRLVVSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    if(countGroupBit > 0)
    {
      if(value&((uint32_t)(~((1<<countGroupBit)-1)))) return MARKER_ERRORDIAPAZON;
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPERIF_CTRLVVREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else if(privateCTRLVVSelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPERIF_CTRLVVREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(privateCTRLVVSelRang51RecordNumber(recordNumber) || privateCTRLVVSelRang52RecordNumber(recordNumber))
    {
      //Ранжирование VV
      if(validN_BIGACMD(value) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileCTRLVVRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileCTRLVVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang
  extern int upravlSetting;
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit = privateCTRLVVSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(countGroupBit>0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPERIF_CTRLVVREGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)

    else if(privateCTRLVVSelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPERIF_CTRLVVREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

  }//for

////ОСОБАЯ ПРОВЕРКА
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  unsigned int chastka = edition_settings.setpoint_r_kom_st_Inom/edition_settings.setpoint_r_kom_st_Inom_vymk;
  switch(recordNumber)
  {
  case 18: //if(editValue == (uint32_t*)&edition_settings.setpoint_pochatkovyj_resurs)
  {
    unsigned int temp1 = edition_settings.setpoint_pochatkovyj_resurs;
    if(
      !(((unsigned int)temp1 >= (2*chastka)) && ((unsigned int)temp1 <= edition_settings.setpoint_r_kom_st_Inom))
    )
    {
      upravlSetting = 0;
      return ERROR_VALID2;//ошибка валидации
    }
    break;
  }//case 18
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case 20:  //if(editValue == (uint32_t*)&edition_settings.setpoint_krytychnyj_resurs)
  {
    unsigned int  temp1 = edition_settings.setpoint_krytychnyj_resurs;
    if(
      !(((unsigned int)temp1 >= chastka) && ((unsigned int)temp1 <= 2*chastka))
    )
    {
      upravlSetting = 0;
      return ERROR_VALID2;//ошибка валидации
    }
    break;
  }//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case 21:  //if(editValue == (uint32_t*)&edition_settings.setpoint_pochatkova_k_vymk)
  {
    unsigned int  temp1 = edition_settings.setpoint_pochatkova_k_vymk;
    if(
      !((unsigned int)temp1 <= edition_settings.setpoint_r_kom_st_Inom)
    )
    {
      upravlSetting = 0;
      return ERROR_VALID2;//ошибка валидации
    }
    break;
  }//
  }//switch
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if(privateCTRLVVSelRang51RecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(edition_settings.ranguvannja_on_cb, &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if
  else if(privateCTRLVVSelRang52RecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(edition_settings.ranguvannja_off_cb, &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileCTRLVVRegister21
