
#include "header.h"

int privateCOMMvalidRecordLen(int recordNumber, int recordLen);
int getFileCOMMRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileCOMMRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileCOMMRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateCOMMvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordNumber
  if(recordNumber != 1) return MARKER_ERRORPERIMETR;
//валидация recordLen
  if(recordLen>2) return MARKER_ERRORPERIMETR;
  return 0;
}//privateCOMMvalidRecordLen

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileCOMMRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateCOMMvalidRecordLen(recordNumber, recordLen) == MARKER_ERRORPERIMETR) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  return GET_UST_REGISTER000FOR20(FILEPROT_COMMREGISTER2021);
}//getFileCOMMRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileCOMMRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateCOMMvalidRecordLen(recordNumber, recordLen) == MARKER_ERRORPERIMETR) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;
    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(SET_UST_REGISTER000FOR21(FILEPROT_COMMREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    else return MARKER_ERRORPERIMETR;
  }//for
  return 0;
}//setFileCOMMRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileCOMMRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;
    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    int errorV = POST_UST_REGISTER000FOR21(FILEPROT_COMMREGISTER2021);
    if(errorV == 1) continue;
    if(errorV != 0) return errorV;
  }//for

  return 0;
}//postFileCOMMRegister21
