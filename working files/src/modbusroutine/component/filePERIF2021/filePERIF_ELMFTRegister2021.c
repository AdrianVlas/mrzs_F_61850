
#include "header.h"

int privateELMFTvalidRecordLen(int recordNumber, int recordLen);
int privateELMFTSelUprRecordNumber(int recordNumber, int registerNumber);
int privateELMFTSelUstRecordNumber(int recordNumber, int registerNumber);
int privateELMFTSelRang2RecordNumber(int recordNumber);
int privateELMFTSelRang3RecordNumber(int recordNumber);
int privateELMFTSelRang4RecordNumber(int recordNumber);
int getFileELMFTRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileELMFTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileELMFTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_ELMFT 16

int privateELMFTSelUprRecordNumber(int recordNumber, int registerNumber)
{
  if((recordNumber%4) != 1) return 0;
  if(registerNumber == 1) return 1;
  return 0;
}//privateELMFTSelUprRecordNumber
int privateELMFTSelUstRecordNumber(int recordNumber, int registerNumber)
{
  if((recordNumber%4) != 1) return 0;
  if((registerNumber == 2) || (registerNumber == 3)) return 1;
  return 0;
}//privateELMFTSelUprRecordNumber

int privateELMFTvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordNumber
  if(recordNumber > 4*NUMBER_DEFINED_FUNCTIONS) return MARKER_ERRORPERIMETR;
  if(recordNumber == 0) return MARKER_ERRORPERIMETR;
//валидация recordLen
  if((recordNumber%4) == 1)
  {
    if(recordLen > 4) return MARKER_ERRORPERIMETR;
  }//if
  else if(recordLen > REGISTERS_ELMFT+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateELMFTvalidRecordLen

int privateELMFTSelRang2RecordNumber(int recordNumber)
{
  if((recordNumber%4) == 2) return 1;
  return 0;
}//privateELMFTSelRang2RecordNumber
int privateELMFTSelRang3RecordNumber(int recordNumber)
{
  if((recordNumber%4) == 3) return 1;
  return 0;
}//privateELMFTSelRang3RecordNumber
int privateELMFTSelRang4RecordNumber(int recordNumber)
{
  if((recordNumber%4) == 0) return 1;
  return 0;
}//privateELMFTSelRang4RecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileELMFTRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateELMFTvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int item  = ((recordNumber - 2)/4)*N_BIG;
  if(privateELMFTSelUprRecordNumber(recordNumber, registerNumber))
    return GET_UPR_REGISTER000FOR20(FILEPERIF_ELMFTREGISTER2021);
  else if(privateELMFTSelUstRecordNumber(recordNumber, registerNumber))
    return GET_UST_REGISTER000FOR20(FILEPERIF_ELMFTREGISTER2021);

  else if(privateELMFTSelRang2RecordNumber(recordNumber))
  {
    //Ранжирование
    return getRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_df_source_plus[item],
                                          registerNumber-1);
  }//if
  else if(privateELMFTSelRang3RecordNumber(recordNumber))
  {
    //Ранжирование
    return getRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_df_source_minus[item],
                                          registerNumber-1);
  }//if
  else if(privateELMFTSelRang4RecordNumber(recordNumber))
  {
    //Ранжирование
    return getRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_df_source_blk[item],
                                          registerNumber-1);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileELMFTRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileELMFTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateELMFTvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateELMFTSelUprRecordNumber(recordNumber, registerNumber))
    {
      if(!(value==0 || value==1)) return MARKER_ERRORDIAPAZON;
    }//if
    else if(privateELMFTSelUstRecordNumber(recordNumber, registerNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPERIF_ELMFTREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
    else if(privateELMFTSelRang2RecordNumber(recordNumber) ||
            privateELMFTSelRang3RecordNumber(recordNumber) ||
            privateELMFTSelRang4RecordNumber(recordNumber)
           )
    {
      //проверка на конфиг
      if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;
      //Ранжирование
      if(validN_BIGACMD(value) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileELMFTRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileELMFTRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateELMFTSelUprRecordNumber(recordNumber, registerNumber))
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPERIF_ELMFTREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
    if(privateELMFTSelUstRecordNumber(recordNumber, registerNumber))
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPERIF_ELMFTREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

  }//for

  int item  = ((recordNumber - 2)/4)*N_BIG;
  if(privateELMFTSelRang2RecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(&edition_settings.ranguvannja_df_source_plus[item],
                                     &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if
  else if(privateELMFTSelRang3RecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(&edition_settings.ranguvannja_df_source_minus[item],
                                     &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if
  else if(privateELMFTSelRang4RecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(&edition_settings.ranguvannja_df_source_blk[item],
                                     &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileELMFTRegister21
