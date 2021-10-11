
#include "header.h"

int privateAJRvalidRecordLen(int recordNumber, int recordLen);
int privateAJRSelUstRecordNumber(int recordNumber);
int privateAJRSelCountGroupBitRecordNumber(int recordNumber);
int getFileAJRRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileAJRRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileAJRRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateAJRSelUstRecordNumber(int recordNumber)
{
  if((recordNumber >= 5 && recordNumber <= 9)) return 1;
  if((recordNumber >= 105 && recordNumber <= 108)) return 1;
  return 0;
}//privateAJRvalidRecordNumber

int privateAJRvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  switch(recordNumber)
  {
  case 0:
  case 1:
  case 101:
    if(recordLen>2) return MARKER_ERRORPERIMETR;
    break;
  default:
    if(recordLen>5) return MARKER_ERRORPERIMETR;
  }//switch
  return 0;
}//privateAJRvalidRecordLen

int privateAJRSelCountGroupBitRecordNumber(int recordNumber)
{
  if(recordNumber == 1)
    return 3;//к-сть Group біт
  if(recordNumber == 101)
    return 2;//к-сть Group біт
  return -1;
}//privateAJRSelCountGroupBitRecordNumber
////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileAJRRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateAJRvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateAJRSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPROT_AJRREGISTER2021);

  else if(countGroupBit > 0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPROT_AJRREGISTER2021);
  }//if(countGroupBit>0)
  else if(privateAJRSelUstRecordNumber(recordNumber))
    return GET_UST_REGISTER000FOR20(FILEPROT_AJRREGISTER2021);

  return MARKER_ERRORPERIMETR;
}//getFileAJRRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileAJRRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateAJRvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit = privateAJRSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPROT_AJRREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(countGroupBit > 0)
    {
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPROT_AJRREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else if(privateAJRSelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPROT_AJRREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileAJRRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileAJRRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit = privateAJRSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPROT_AJRREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

    else if(countGroupBit > 0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPROT_AJRREGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)

    else if(privateAJRSelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPROT_AJRREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
  }//for


  return 0;
}//postFileAJRRegister21
