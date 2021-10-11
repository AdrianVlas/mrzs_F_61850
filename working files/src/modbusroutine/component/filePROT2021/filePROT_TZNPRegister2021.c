
#include "header.h"

int privateArmCosF32(float temp_value_f);
int privateArmSinF32(float temp_value_f);
int privateTZNPvalidRecordLen(int recordNumber, int recordLen);
int privateTZNPSelUstRecordNumber(int recordNumber);
int privateTZNPSelCountGroupBitRecordNumber(int recordNumber);
int getFileTZNPRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileTZNPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileTZNPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateTZNPSelUstRecordNumber(int recordNumber)
{
  if((recordNumber >= 5 && recordNumber <= 11)) return 1;
  if((recordNumber >= 105 && recordNumber <= 111)) return 1;
  if((recordNumber >= 205 && recordNumber <= 211)) return 1;
  return 0;
}//privateTZNPvalidRecordNumber

int privateTZNPvalidRecordLen(int recordNumber, int recordLen)
{
//валидаци€ recordLen
  switch(recordNumber)
  {
  case 0:
  case 1:
  case 101:
  case 201:
    if(recordLen>2) return MARKER_ERRORPERIMETR;
    break;
  default:
    if(recordLen>5) return MARKER_ERRORPERIMETR;
  }//switch
  return 0;
}//privateTZNPvalidRecordLen

int privateTZNPSelCountGroupBitRecordNumber(int recordNumber)
{
  if((recordNumber == 1) || (recordNumber == 101) || (recordNumber == 201))
    return 3;//к-сть Group б≥т
  return -1;
}//privateTZNPSelCountGroupBitRecordNumber
////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileTZNPRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидаци€ recordLen
  if(privateTZNPvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateTZNPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group б≥т

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPROT_TZNPREGISTER2021);

  else if(countGroupBit > 0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPROT_TZNPREGISTER2021);
  }//if(countGroupBit>0)
  else if(privateTZNPSelUstRecordNumber(recordNumber))
    return GET_UST_REGISTER000FOR20(FILEPROT_TZNPREGISTER2021);

  return MARKER_ERRORPERIMETR;
}//getFileTZNPRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileTZNPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидаци€ recordLen
  if(privateTZNPvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit = privateTZNPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group б≥т

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPROT_TZNPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(countGroupBit > 0)
    {
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPROT_TZNPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else if(privateTZNPSelUstRecordNumber(recordNumber))
    {
      if(SET_UST_REGISTER000FOR21(FILEPROT_TZNPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileTZNPRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileTZNPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit = privateTZNPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group б≥т

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPROT_TZNPREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

    else if(countGroupBit>0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPROT_TZNPREGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)

    else if(privateTZNPSelUstRecordNumber(recordNumber))
    {
////ќ—ќЅјя —Ѕќ– ј
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      int grupa_ustavok = registerNumber - 1;
      switch(recordNumber)
      {
      case 11://if(editValue == (uint32_t*)&edition_settings.setpoint_tznp_1_angle[grupa_ustavok])
        edition_settings.setpoint_tznp_1_angle_cos[grupa_ustavok] = privateArmCosF32(value);
        edition_settings.setpoint_tznp_1_angle_sin[grupa_ustavok] = privateArmSinF32(value);
        break;
      case 111://if(editValue == (uint32_t*)&edition_settings.setpoint_tznp_2_angle[grupa_ustavok])
        edition_settings.setpoint_tznp_2_angle_cos[grupa_ustavok] = privateArmCosF32(value);
        edition_settings.setpoint_tznp_2_angle_sin[grupa_ustavok] = privateArmSinF32(value);
        break;
      case 211://if(editValue == (uint32_t*)&edition_settings.setpoint_tznp_3_angle[grupa_ustavok])
        edition_settings.setpoint_tznp_3_angle_cos[grupa_ustavok] = privateArmCosF32(value);
        edition_settings.setpoint_tznp_3_angle_sin[grupa_ustavok] = privateArmSinF32(value);
        break;
      }//switch
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

      int errorV = POST_UST_REGISTER000FOR21(FILEPROT_TZNPREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
  }//for


  return 0;
}//postFileTZNPRegister21
