
#include "header.h"

int privateUROVvalidRecordLen(int recordNumber, int recordLen);
int privateUROVSelUstRecordNumber(int recordNumber);
int privateUROVSelCountGroupBitRecordNumberRegisterNumber(int recordNumber, int registerNumber);
int getFileUROVRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileUROVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileUROVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateUROVSelUstRecordNumber(int recordNumber)
{
  if((recordNumber >= 5 && recordNumber <= 7)) return 1;
  return 0;
}//privateUROVvalidRecordNumber

int privateUROVvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  switch(recordNumber)
  {
  case 0:
  case 1:
    if(recordLen>4) return MARKER_ERRORPERIMETR;
    break;
  default:
    if(recordLen>5) return MARKER_ERRORPERIMETR;
  }//switch
  return 0;
}//privateUROVvalidRecordLen

int privateUROVSelCountGroupBitRecordNumberRegisterNumber(int recordNumber, int registerNumber)
{
  if(recordNumber != 1) return -1;
  if((registerNumber == 1) || (registerNumber == 2))
    return 16;//к-сть Group біт
  if(registerNumber == 3)
    return 4;//к-сть Group біт
  return -1;
}//privateUROVSelCountGroupBitRecordNumber
////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileUROVRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateUROVvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateUROVSelCountGroupBitRecordNumberRegisterNumber(recordNumber, registerNumber);//к-сть Group біт

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPROT_UROVREGISTER2021);

  else if(countGroupBit > 0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPROT_UROVREGISTER2021);
  }//if(countGroupBit>0)
  else if(privateUROVSelUstRecordNumber(recordNumber))
    return GET_UST_REGISTER000FOR20(FILEPROT_UROVREGISTER2021);

  return MARKER_ERRORPERIMETR;
}//getFileUROVRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileUROVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateUROVvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit =
      privateUROVSelCountGroupBitRecordNumberRegisterNumber(recordNumber, registerNumber);//к-сть Group біт

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPROT_UROVREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(countGroupBit > 0)
    {
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPROT_UROVREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else if(privateUROVSelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPROT_UROVREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileUROVRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileUROVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit =
      privateUROVSelCountGroupBitRecordNumberRegisterNumber(recordNumber, registerNumber);//к-сть Group біт

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPROT_UROVREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

    else if(countGroupBit>0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPROT_UROVREGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)

    else if(privateUROVSelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPROT_UROVREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
  }//for


  return 0;
}//postFileUROVRegister21
