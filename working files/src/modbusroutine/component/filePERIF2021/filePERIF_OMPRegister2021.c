
#include "header.h"

int privateOMPvalidRecordLen(int recordNumber, int recordLen);
int privateOMPSelUstRecordNumber(int recordNumber);
int privateOMPSelCountGroupBitRecordNumber(int recordNumber);
int getFileOMPRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileOMPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileOMPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

/**************************************/
//подготовка компонента OMP
/**************************************/

int privateOMPSelUstRecordNumber(int recordNumber)
{
  if((recordNumber >= 5 && recordNumber <= 38)) return 1;
  return 0;
}//privateOMPvalidRecordNumber

int privateOMPvalidRecordLen(int recordNumber, int recordLen)
{
  UNUSED(recordNumber);
//валидация recordLen
  if(recordLen > 2) return MARKER_ERRORPERIMETR;
  return 0;
}//privateOMPvalidRecordLen

int privateOMPSelCountGroupBitRecordNumber(int recordNumber)
{
  if(recordNumber == 1)
    return 1;//к-сть Group біт
  return -1;
}//privateOMPSelCountGroupBitRecordNumber
////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileOMPRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateOMPvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateOMPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPERIF_OMPREGISTER2021);

  else if(countGroupBit > 0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPERIF_OMPREGISTER2021);
  }//if(countGroupBit>0)
  else if(privateOMPSelUstRecordNumber(recordNumber))
    return GET_UST_REGISTER000FOR20(FILEPERIF_OMPREGISTER2021);

  return MARKER_ERRORPERIMETR;
}//getFileOMPRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileOMPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateOMPvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit = privateOMPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPERIF_OMPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(countGroupBit > 0)
    {
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPERIF_OMPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else if(privateOMPSelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPERIF_OMPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileOMPRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileOMPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit = privateOMPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPERIF_OMPREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

    else if(countGroupBit>0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPERIF_OMPREGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)

    else if(privateOMPSelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPERIF_OMPREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
  }//for


  return 0;
}//postFileOMPRegister21
