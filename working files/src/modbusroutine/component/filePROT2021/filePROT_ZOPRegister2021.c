
#include "header.h"

int privateZOPvalidRecordLen(int recordNumber, int recordLen);
int privateZOPSelUstRecordNumber(int recordNumber);
int privateZOPSelCountGroupBitRecordNumber(int recordNumber);
int getFileZOPRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZOPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZOPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateZOPSelUstRecordNumber(int recordNumber)
{
  if((recordNumber >= 5 && recordNumber <= 6)) return 1;
  return 0;
}//privateZOPvalidRecordNumber

int privateZOPvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  switch(recordNumber)
  {
  case 0:
  case 1:
    if(recordLen>2) return MARKER_ERRORPERIMETR;
    break;
  default:
    if(recordLen>5) return MARKER_ERRORPERIMETR;
  }//switch
  return 0;
}//privateZOPvalidRecordLen

int privateZOPSelCountGroupBitRecordNumber(int recordNumber)
{
  if(recordNumber == 1)
    return 1;//к-сть Group біт
  return -1;
}//privateZOPSelCountGroupBitRecordNumber
////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileZOPRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateZOPvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateZOPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPROT_ZOPREGISTER2021);

  else if(countGroupBit > 0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPROT_ZOPREGISTER2021);
  }//if(countGroupBit>0)
  else if(privateZOPSelUstRecordNumber(recordNumber))
    return GET_UST_REGISTER000FOR20(FILEPROT_ZOPREGISTER2021);

  return MARKER_ERRORPERIMETR;
}//getFileZOPRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileZOPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateZOPvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit = privateZOPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPROT_ZOPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(countGroupBit > 0)
    {
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPROT_ZOPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else if(privateZOPSelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPROT_ZOPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileZOPRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileZOPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit = privateZOPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPROT_ZOPREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

    else if(countGroupBit>0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPROT_ZOPREGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)

    else if(privateZOPSelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPROT_ZOPREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
  }//for


  return 0;
}//postFileZOPRegister21
