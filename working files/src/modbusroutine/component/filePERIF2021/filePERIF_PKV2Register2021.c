
#include "header.h"

int privatePKV2validRecordLen(int recordNumber, int recordLen);
int privatePKV2SelUstRecordNumber(int recordNumber);
int getFilePKV2Register20(int recordNumber, int registerNumber, int recordLen);
int setFilePKV2Register21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFilePKV2Register21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privatePKV2SelUstRecordNumber(int recordNumber)
{
  if((recordNumber >= 2) && (recordNumber <= 3)) return 1;
  return 0;
}//privatePKV2SelUstRecordNumber

int privatePKV2validRecordLen(int recordNumber, int recordLen)
{
  UNUSED(recordNumber);
//валидация recordLen
  if(recordLen > 2) return MARKER_ERRORPERIMETR;
  return 0;
}//privatePKV2validRecordLen

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFilePKV2Register20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privatePKV2validRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privatePKV2SelUstRecordNumber(recordNumber))
    return GET_UST_REGISTER000FOR20(FILEPERIF_PKV2REGISTER2021);

  return MARKER_ERRORPERIMETR;
}//getFilePKV2Register20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFilePKV2Register21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privatePKV2validRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privatePKV2SelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPERIF_PKV2REGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFilePKV2Register21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFilePKV2Register21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privatePKV2SelUstRecordNumber(recordNumber))
    {
      int errorV = POST_UST_REGISTER000FOR21(FILEPERIF_PKV2REGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

  }//for

  return 0;
}//postFilePKV2Register21
