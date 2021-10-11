
#include "header.h"

int privatePLINEvalidRecordLen(int recordNumber, int recordLen);
int privatePLINESelUprRecordNumber(int recordNumber);
int getFilePLINERegister20(int recordNumber, int registerNumber, int recordLen);
int setFilePLINERegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFilePLINERegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privatePLINESelUprRecordNumber(int recordNumber)
{
  if((recordNumber == 1) || (recordNumber == 3) || (recordNumber == 4)) return 1;
  return 0;
}//privatePLINESelUprRecordNumber

int privatePLINEvalidRecordLen(int recordNumber, int recordLen)
{
  UNUSED(recordNumber);
//валидация recordLen
  if(recordLen > 2) return MARKER_ERRORPERIMETR;
  return 0;
}//privatePLINEvalidRecordLen

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFilePLINERegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privatePLINEvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privatePLINESelUprRecordNumber(recordNumber))
    return GET_UPR_REGISTER000FOR20(FILEPERIF_PLINEREGISTER2021);
  
  return MARKER_ERRORPERIMETR;
}//getFilePLINERegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFilePLINERegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privatePLINEvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privatePLINESelUprRecordNumber(recordNumber))
    {
      if(SET_UPR_REGISTER000FOR21(FILEPERIF_PLINEREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
  }//for
  return 0;
}//setFilePLINERegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFilePLINERegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSetting;
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privatePLINESelUprRecordNumber(recordNumber))
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPERIF_PLINEREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
  }//for

////ОСОБАЯ ПРОВЕРКА
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if(recordNumber == 4)//editControl == (uint32_t*)&edition_settings.control_transformator)
  {
    if((edition_settings.control_transformator & (1<<INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)==0)) //0-фазные
    {
      if((edition_settings.control_extra_settings_1 & (1<<INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)!=0)) //1-линейные
      {
        upravlSetting = 0;
        return ERROR_VALID3;//ошибка валидации
      }
    }//if
  }//if(recordNumber == 4)//editControl == (uint32_t*)&edition_settings.control_transformator)
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return 0;
}//postFilePLINERegister21
