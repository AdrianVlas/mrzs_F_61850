
#include "header.h"

#ifdef  MODYFIKACIA_VERSII_DS

int privateBLOCKDSvalidRecordLen(int recordNumber, int recordLen);
int privateBLOCKDSSelRangRecordNumber(int recordNumber);
int getFileBLOCKDSRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileBLOCKDSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileBLOCKDSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_OUTPUTS 16

int privateBLOCKDSvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordNumber
  if(recordNumber > NUMBER_DS+1) return MARKER_ERRORPERIMETR;
//валидация recordLen
  if(recordNumber == 0) if(recordLen > 2) return MARKER_ERRORPERIMETR;
  if(recordNumber > 0)  if(recordLen > REGISTERS_OUTPUTS+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateBLOCKDSvalidRecordLen

int privateBLOCKDSSelRangRecordNumber(int recordNumber)
{
  if(recordNumber > 0) return 1;
  return 0;
}//privateBLOCKDSSelRangRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileBLOCKDSRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateBLOCKDSvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPERIF_BLOCKDSREGISTER2021);
  else if(privateBLOCKDSSelRangRecordNumber(recordNumber))
  {
    //Ранжирование BLOCKDS
    return getRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_outputs[(N_BIG*NUMBER_SIMPLE_OUTPUTS)],
                                          registerNumber-1);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileBLOCKDSRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileBLOCKDSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateBLOCKDSvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPERIF_BLOCKDSREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else if(privateBLOCKDSSelRangRecordNumber(recordNumber))
    {
      //Ранжирование BLOCKDS
      if(validN_BIGACMD(value) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileBLOCKDSRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileBLOCKDSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPERIF_BLOCKDSREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
  }//for

  if(privateBLOCKDSSelRangRecordNumber(recordNumber))
  {
    writeRangN_BIGModbusRegister2021(&current_settings_interfaces.ranguvannja_outputs[(N_BIG*NUMBER_SIMPLE_OUTPUTS)],
                                     &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileBLOCKDSRegister21
#endif
