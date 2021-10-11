
#include "header.h"

int privateValidatorUstUP(int regUst, int item, int lowHiReg, int *multer);
int privateUPvalidRecordLen(int recordNumber, int recordLen);
int privateUPSelUstRecordNumber(int recordNumber);
int privateUPSelCountGroupBitRecordNumber(int recordNumber);
int getFileUPRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileUPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileUPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateUPSelUstRecordNumber(int recordNumber)
{
  if((recordNumber == 2)     ||
      (recordNumber == 102)     ||
      (recordNumber == 202)     ||
      (recordNumber == 302)     ||
      (recordNumber == 402)     ||
      (recordNumber == 502)     ||
      (recordNumber == 602)     ||
      (recordNumber == 702)
    ) return 1;
  if((recordNumber >= 5 && recordNumber <= 7)     ||
      (recordNumber >= 105 && recordNumber <= 107) ||
      (recordNumber >= 205 && recordNumber <= 207) ||
      (recordNumber >= 305 && recordNumber <= 307) ||
      (recordNumber >= 405 && recordNumber <= 407) ||
      (recordNumber >= 505 && recordNumber <= 507) ||
      (recordNumber >= 605 && recordNumber <= 607) ||
      (recordNumber >= 705 && recordNumber <= 707)) return 1;
  return 0;
}//privateUPvalidRecordNumber

int privateUPvalidRecordLen(int recordNumber, int recordLen)
{
//валидаци€ recordLen
  switch(recordNumber)
  {
  case 0:
  case 1:
  case 101:
  case 201:
  case 301:
  case 401:
  case 501:
  case 601:
  case 701:
  case 2:
  case 102:
  case 202:
  case 302:
  case 402:
  case 502:
  case 602:
  case 702:
    if(recordLen>2) return MARKER_ERRORPERIMETR;
    break;
  case 5:
  case 105:
  case 205:
  case 305:
  case 405:
  case 505:
  case 605:
  case 705:
    if(recordLen>9) return MARKER_ERRORPERIMETR;
    break;
  default:
    if(recordLen>5) return MARKER_ERRORPERIMETR;
  }//switch
  return 0;
}//privateUPvalidRecordLen

int privateUPSelCountGroupBitRecordNumber(int recordNumber)
{
  if((recordNumber == 1)   || (recordNumber == 101) || (recordNumber == 201) || (recordNumber == 301) ||
      (recordNumber == 401) || (recordNumber == 501) || (recordNumber == 601) || (recordNumber == 701))
    return 3;//к-сть Group б≥т
  return -1;
}//privateUPSelCountGroupBitRecordNumber
////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileUPRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидаци€ recordLen
  if(privateUPvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateUPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group б≥т

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPROT_UPREGISTER2021);

  else if(countGroupBit > 0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPROT_UPREGISTER2021);
  }//if(countGroupBit>0)
  else if(privateUPSelUstRecordNumber(recordNumber))
  {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if((recordNumber%100) == 5)//if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][gruppa])
    {
      int grupa_ustavok = (registerNumber - 1)/2;
      int item = recordNumber/100;
      uint32_t *editValue = (uint32_t*)&edition_settings.setpoint_UP[item][0][grupa_ustavok];

      int multer = 10;
      privateValidatorUstUP(0, item, (registerNumber-1)&1, &multer);//load multer

      if((registerNumber-1)&1)
        return (((uint32_t)(*editValue))/multer)&0xFFFF;
      else
        return (((*editValue)/multer)>>16)&0xFFFF;
    }//if((recordNumber%100) == 5)//if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][gruppa])
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    return GET_UST_REGISTER000FOR20(FILEPROT_UPREGISTER2021);
  }

  return MARKER_ERRORPERIMETR;
}//getFileUPRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileUPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидаци€ recordLen
  if(privateUPvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit = privateUPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group б≥т

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPROT_UPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(countGroupBit > 0)
    {
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPROT_UPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else if(privateUPSelUstRecordNumber(recordNumber))
    {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if((recordNumber%100) == 5)
      {
        int item = recordNumber/100;
        int multer = 10;
        if(!privateValidatorUstUP(value, item, (registerNumber-1)&1, &multer)) return MARKER_ERRORDIAPAZON;
        continue;
      }//if((recordNumber%100) == 5)
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if(SET_UST_REGISTER000FOR21(FILEPROT_UPREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileUPRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileUPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSetting;//флаг Setting
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit = privateUPSelCountGroupBitRecordNumber(recordNumber);//к-сть Group б≥т

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPROT_UPREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

    else if(countGroupBit>0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPROT_UPREGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)

    else if(privateUPSelUstRecordNumber(recordNumber))
    {
////ќ—ќЅјя —Ѕќ– ј
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if((recordNumber%100) == 5)//if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][grupa_ustavok])
      {
        int grupa_ustavok = (registerNumber - 1)/2;
        int item = recordNumber/100;
        uint32_t *editValue = (uint32_t*)&edition_settings.setpoint_UP[item][0][grupa_ustavok];

        int multer = 10;
        privateValidatorUstUP(value, item, (registerNumber-1)&1, &multer);//load multer

        if((registerNumber-1)&1)
        {
          //мл регистр
          (*editValue) = value*multer;
          upravlSetting = 1;//флаг Setting
          continue;
        }//if(offset&1)
        else
        {
          //ст регистр
          switch(edition_settings.ctrl_UP_input[item])
          {
          case UP_CTRL_P:
          case UP_CTRL_Q:
          case UP_CTRL_S:
            (*editValue)  &= 0xffff;
            (*editValue)  |= ((value & 0xffff)<<16);//
            upravlSetting = 1;//флаг Setting
            continue;
          default:
            continue;
          }//switch
        }//else
      }//if((recordNumber%100) == 5) if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][grupa_ustavok])
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      int errorV = POST_UST_REGISTER000FOR21(FILEPROT_UPREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
  }//for

////ќ—ќЅјя ѕ–ќ¬≈– ј
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if((recordNumber%100) == 5)//if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][grupa_ustavok])
  {
    for(int grupa_ustavok=0; grupa_ustavok<4; grupa_ustavok++)
    {
      int item = recordNumber/100;
      int temp1 = edition_settings.setpoint_UP[item][0][grupa_ustavok];

      switch(edition_settings.ctrl_UP_input[item])
      {
      case UP_CTRL_P:
      case UP_CTRL_Q:
        if(temp1<0) temp1 = -temp1;//модуль
        if(temp1<SETPOINT_UP_P_MIN ||
            temp1>SETPOINT_UP_P_MAX
          )
        {
          upravlSetting = 0;//флаг Setting
          return ERROR_VALID2;//ошибка валидации
        }
        break;

      case UP_CTRL_S:
        if(temp1<SETPOINT_UP_P_MIN ||
            temp1>SETPOINT_UP_P_MAX
          )
        {
          upravlSetting = 0;//флаг Setting
          return ERROR_VALID2;//ошибка валидации
        }
        break;
      }//switch
    }//for
  }//if((recordNumber%100) == 5)//if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][grupa_ustavok])
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return 0;
}//postFileUPRegister21

int privateValidatorUstUP(int regUst, int item, int lowHiReg, int *multer)
{
  switch(edition_settings.ctrl_UP_input[item])
  {
  case UP_CTRL_Ia_Ib_Ic:
  case UP_CTRL_Ia:
  case UP_CTRL_Ib:
  case UP_CTRL_Ic:
  case UP_CTRL_I1:
  case UP_CTRL_I2:
  case UP_CTRL_I04:
  case UP_CTRL_3I0_r:
    if(lowHiReg == 1)
    {
      if(regUst != 0) return 0;//нет старшего регистра
    }//if
    else if(regUst<SETPOINT_UP_I_MIN/10 || regUst>SETPOINT_UP_I_MAX/10) return 0;
    break;
  case UP_CTRL_3I0:
  case UP_CTRL_3I0_others:
    (*multer) = 1;
    if(lowHiReg == 1)
    {
      if(regUst != 0) return 0;//нет старшего регистра
    }//if
    else if(regUst<SETPOINT_UP_3I0_MIN || regUst>SETPOINT_UP_3I0_MAX) return 0;
    break;
  case UP_CTRL_Ua_Ub_Uc:
  case UP_CTRL_Ua:
  case UP_CTRL_Ub:
  case UP_CTRL_Uc:
  case UP_CTRL_U1:
  case UP_CTRL_U2:
  case UP_CTRL_3U0:
    (*multer) = 100;
    if(lowHiReg == 1)
    {
      if(regUst != 0) return 0;//нет старшего регистра
    }//if
    else if(regUst<SETPOINT_UP_U_MIN/100 || regUst>SETPOINT_UP_U_MAX/100) return 0;
    break;
  case UP_CTRL_P:
  case UP_CTRL_Q:
  case UP_CTRL_S:
    (*multer) = 1;
    break;
  }//switch
  return 1;
}//privateValidatorUstUP

