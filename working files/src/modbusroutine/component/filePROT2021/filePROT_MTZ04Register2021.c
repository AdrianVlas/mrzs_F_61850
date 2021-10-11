
#include "header.h"

int privateMTZ04validRecordLen(int recordNumber, int recordLen);
int privateMTZ04SelUstRecordNumber(int recordNumber);
int privateMTZ04SelCountGroupBitRecordNumber(int recordNumber);
int getFileMTZ04Register20(int recordNumber, int registerNumber, int recordLen);
int setFileMTZ04Register21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileMTZ04Register21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateMTZ04SelUstRecordNumber(int recordNumber)
{
  if((recordNumber >= 5 && recordNumber <= 6)
      || (recordNumber >= 105 && recordNumber <= 108)
      || (recordNumber == 102)) return 1;
  return 0;
}//privateMTZ04validRecordNumber

int privateMTZ04validRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  switch(recordNumber)
  {
  case 0:
  case 1:
  case 101:
  case 102:
    if(recordLen>2) return MARKER_ERRORPERIMETR;
    break;
  default:
    if(recordLen>5) return MARKER_ERRORPERIMETR;
  }//switch
  return 0;
}//privateMTZ04validRecordLen

int privateMTZ04SelCountGroupBitRecordNumber(int recordNumber)
{
  if(recordNumber == 1)
    return 1;//к-сть Group біт
  if(recordNumber == 101)
    return 3;//к-сть Group біт
  return -1;
}//privateMTZ04SelCountGroupBitRecordNumber
////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileMTZ04Register20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateMTZ04validRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateMTZ04SelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPROT_MTZ04REGISTER2021);

  else if(countGroupBit > 0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPROT_MTZ04REGISTER2021);
  }//if(countGroupBit>0)
  else if(privateMTZ04SelUstRecordNumber(recordNumber))
    return GET_UST_REGISTER000FOR20(FILEPROT_MTZ04REGISTER2021);

  return MARKER_ERRORPERIMETR;
}//getFileMTZ04Register20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileMTZ04Register21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateMTZ04validRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit = privateMTZ04SelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPROT_MTZ04REGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(countGroupBit > 0)
    {
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPROT_MTZ04REGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else if(privateMTZ04SelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPROT_MTZ04REGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileMTZ04Register21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileMTZ04Register21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit = privateMTZ04SelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPROT_MTZ04REGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

    else if(countGroupBit>0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPROT_MTZ04REGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)

    else if(privateMTZ04SelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPROT_MTZ04REGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
  }//for


  return 0;
}//postFileMTZ04Register21
