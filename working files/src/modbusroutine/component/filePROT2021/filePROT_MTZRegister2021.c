
#include "header.h"

int privateArmCosF32(float temp_value_f);
int privateArmSinF32(float temp_value_f);
int privateMTZvalidRecordLen(int recordNumber, int recordLen);
int privateMTZSelUstRecordNumber(int recordNumber);
int privateMTZSelCountGroupBitRecordNumber(int recordNumber);
int getFileMTZRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileMTZRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileMTZRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateMTZSelUstRecordNumber(int recordNumber)
{
  if((recordNumber >= 5 && recordNumber <= 14)
      || (recordNumber == 2)
      || (recordNumber >= 105 && recordNumber <= 119)
      || (recordNumber == 102)
      || (recordNumber >= 205 && recordNumber <= 214)
      || (recordNumber == 202)
      || (recordNumber >= 305 && recordNumber <= 314)
      || (recordNumber == 302) ) return 1;
  return 0;
}//privateMTZvalidRecordNumber

int privateMTZvalidRecordLen(int recordNumber, int recordLen)
{
//валидаци€ recordLen
  switch(recordNumber)
  {
  case 0:
  case 1:
  case 2:
  case 101:
  case 102:
  case 201:
  case 202:
  case 301:
  case 302:
    if(recordLen>2) return MARKER_ERRORPERIMETR;
    break;
  default:
    if(recordLen>5) return MARKER_ERRORPERIMETR;
  }//switch
  return 0;
}//privateMTZvalidRecordLen

int privateMTZSelCountGroupBitRecordNumber(int recordNumber)
{
  switch(recordNumber)
  {
  case 1:
  case 201:
    return 3;//к-сть Group б≥т
  case 101:
    return 5;//к-сть Group б≥т
  case 301:
    return 7;//к-сть Group б≥т
  }//switch
  return -1;
}//privateMTZSelCountGroupBitRecordNumber
////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileMTZRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидаци€ recordLen
  if(privateMTZvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateMTZSelCountGroupBitRecordNumber(recordNumber);//к-сть Group б≥т

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPROT_MTZREGISTER2021);

  else if(countGroupBit>0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPROT_MTZREGISTER2021);
  }//if(countGroupBit>0)
  else if(privateMTZSelUstRecordNumber(recordNumber))
  {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if(recordNumber == 102)//if(editValue == (uint32_t*)&edition_settings.type_mtz04_2)
    {
      if(edition_settings.type_mtz04_2 >= 2)
        return (edition_settings.type_mtz04_2 - 2) & 0xFFFF;
    }//if(editValue == (uint32_t*)&edition_settings.type_mtz04_2)
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    return GET_UST_REGISTER000FOR20(FILEPROT_MTZREGISTER2021);
  }
  return MARKER_ERRORPERIMETR;
}//getFileMTZRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileMTZRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидаци€ recordLen
  if(privateMTZvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit = privateMTZSelCountGroupBitRecordNumber(recordNumber);//к-сть Group б≥т

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPROT_MTZREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(countGroupBit>0)
    {
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPROT_MTZREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else if(privateMTZSelUstRecordNumber(recordNumber))
    {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if(recordNumber == 102)//if(editValue == (uint32_t*)&edition_settings.type_mtz04_2)
      {
        if(value > 3) return MARKER_ERRORDIAPAZON;
        continue;
      }//if(editValue == (uint32_t*)&edition_settings.type_mtz04_2)
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if(SET_UST_REGISTER000FOR21(FILEPROT_MTZREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileMTZRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileMTZRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSetting;//флаг Setting
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit = privateMTZSelCountGroupBitRecordNumber(recordNumber);//к-сть Group б≥т

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPROT_MTZREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

    else if(countGroupBit>0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPROT_MTZREGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)

    else if(privateMTZSelUstRecordNumber(recordNumber))
    {
////ќ—ќЅјя —Ѕќ– ј
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      int grupa_ustavok = registerNumber - 1;
      switch(recordNumber)
      {
      case 102://if(editValue == (uint32_t*)&edition_settings.type_mtz04_2)
      {
        if(value != 0) value += 2;
        edition_settings.type_mtz04_2 = value;
        upravlSetting = 1;//флаг Setting
        continue;
      }//if(editValue == (uint32_t*)&edition_settings.type_mtz04_2)
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      case 10://if(editValue == (uint32_t*)&edition_settings.setpoint_mtz_1_angle[grupa_ustavok])
        edition_settings.setpoint_mtz_1_angle_cos[grupa_ustavok] = privateArmCosF32(value);
        edition_settings.setpoint_mtz_1_angle_sin[grupa_ustavok] = privateArmSinF32(value);
        break;
      case 110://if(editValue == (uint32_t*)&edition_settings.setpoint_mtz_2_angle[grupa_ustavok])
        edition_settings.setpoint_mtz_2_angle_cos[grupa_ustavok] = privateArmCosF32(value);
        edition_settings.setpoint_mtz_2_angle_sin[grupa_ustavok] = privateArmSinF32(value);
        break;
      case 210://if(editValue == (uint32_t*)&edition_settings.setpoint_mtz_3_angle[grupa_ustavok])
        edition_settings.setpoint_mtz_3_angle_cos[grupa_ustavok] = privateArmCosF32(value);
        edition_settings.setpoint_mtz_3_angle_sin[grupa_ustavok] = privateArmSinF32(value);
        break;
      case 310://if(editValue == (uint32_t*)&edition_settings.setpoint_mtz_4_angle[grupa_ustavok])
        edition_settings.setpoint_mtz_4_angle_cos[grupa_ustavok] = privateArmCosF32(value);
        edition_settings.setpoint_mtz_4_angle_sin[grupa_ustavok] = privateArmSinF32(value);
        break;
      }//switch
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      int errorV = POST_UST_REGISTER000FOR21(FILEPROT_MTZREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
  }//for


  return 0;
}//postFileMTZRegister21
