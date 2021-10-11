
#include "header.h"

int privateZDZvalidRecordLen(int recordNumber, int recordLen);
int privateZDZSelUstRecordNumber(int recordNumber);
int privateZDZSelCountGroupBitRecordNumber(int recordNumber);
int getFileZDZRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZDZRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZDZRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privateZDZSelUstRecordNumber(int recordNumber)
{
#ifdef  MODYFIKACIA_VERSII_OVD_ZDZ
  if((recordNumber >= 5 && recordNumber <= 7)) return 1;
#else
  if((recordNumber >= 5 && recordNumber <= 6)) return 1;
#endif

  return 0;
}//privateZDZvalidRecordNumber

int privateZDZvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordLen
  switch(recordNumber)
  {
  case 0:
  case 1:
#ifdef  MODYFIKACIA_VERSII_OVD_ZDZ
  case 7:
#endif
    if(recordLen>2) return MARKER_ERRORPERIMETR;
    break;
  default:
    if(recordLen>5) return MARKER_ERRORPERIMETR;
  }//switch
  return 0;
}//privateZDZvalidRecordLen

int privateZDZSelCountGroupBitRecordNumber(int recordNumber)
{
#ifdef  MODYFIKACIA_VERSII_OVD_ZDZ
  if(recordNumber == 1)
    return 10;//к-сть Group біт
#else

  if(recordNumber == 1)
    return 7;//к-сть Group біт
#endif
  return -1;
}//privateZDZSelCountGroupBitRecordNumber
////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileZDZRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateZDZvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int  countGroupBit = privateZDZSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

  if(recordNumber == 0) return GET_UPR_REGISTER000FOR20(FILEPROT_ZDZREGISTER2021);

  else if(countGroupBit > 0)
  {
    return GET_GROUPUPR_REGISTER000FOR20(FILEPROT_ZDZREGISTER2021);
  }//if(countGroupBit>0)
  else if(privateZDZSelUstRecordNumber(recordNumber))
  {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef  MODYFIKACIA_VERSII_OVD_ZDZ
    if(recordNumber == 7)//if(editValue == (uint32_t*)&edition_settings.zdz_ovd_porig)
      return (edition_settings.zdz_ovd_porig + 1) & 0xFFFF;
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    return GET_UST_REGISTER000FOR20(FILEPROT_ZDZREGISTER2021);
  }

  return MARKER_ERRORPERIMETR;
}//getFileZDZRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileZDZRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateZDZvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    int  countGroupBit = privateZDZSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    if(recordNumber == 0)
    {
      if(SET_UPR_REGISTER000FOR21(FILEPROT_ZDZREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if

    else if(countGroupBit > 0)
    {
      if(SET_GROUPUPR_REGISTER000FOR21(FILEPROT_ZDZREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if(countGroupBit>0)

    else if(privateZDZSelUstRecordNumber(recordNumber))
    {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef  MODYFIKACIA_VERSII_OVD_ZDZ
      if(recordNumber == 7)//if(editValue == (uint32_t*)&edition_settings.zdz_ovd_porig)
      {
        if(value < 1 || value > ZDZ_CTRL_PORIG_N) return MARKER_ERRORDIAPAZON;
        continue;
      }//if(editValue == (uint32_t*)&edition_settings.zdz_ovd_porig)
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if(SET_UST_REGISTER000FOR21(FILEPROT_ZDZREGISTER2021) != 0) return MARKER_ERRORDIAPAZON;
    }//if
    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileZDZRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileZDZRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSetting;//флаг Setting
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    int  countGroupBit = privateZDZSelCountGroupBitRecordNumber(recordNumber);//к-сть Group біт

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(recordNumber == 0)
    {
      int errorV = POST_UPR_REGISTER000FOR21(FILEPROT_ZDZREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if

    else if(countGroupBit>0)
    {
      int errorV = POST_GROUPUPR_REGISTER000FOR21(FILEPROT_ZDZREGISTER2021);
      if(errorV != 0) return errorV;
    }//if(countGroupBit>0)

    else if(privateZDZSelUstRecordNumber(recordNumber))
    {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef  MODYFIKACIA_VERSII_OVD_ZDZ
      if(recordNumber == 7)//if(editValue == (uint32_t*)&edition_settings.zdz_ovd_porig)
      {
        value -= 1;
        edition_settings.zdz_ovd_porig = value;
        upravlSetting = 1;//флаг Setting
        continue;
      }//if(editValue == (uint32_t*)&edition_settings.zdz_ovd_porig)
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

      int errorV = POST_UST_REGISTER000FOR21(FILEPROT_ZDZREGISTER2021);
      if(errorV == 1) continue;
      if(errorV != 0) return errorV;
    }//if
  }//for

////ОСОБАЯ ПРОВЕРКА
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if(recordNumber == 5) //if(editValue == (uint32_t*)&edition_settings.ctrl_zdz_type)
  {
    int temp1 = edition_settings.ctrl_zdz_type;
    switch(temp1)
    {
    case 0://0-без контроля
      break;
    case 1://1-с контролем по току
      if(!(edition_settings.configuration&(1<<MTZ_BIT_CONFIGURATION)))
      {
        upravlSetting = 0;//флаг Setting
        return ERROR_VALID2;
      }
      break;
    case 2://2-с контролем по напряжению
      if(!(edition_settings.configuration&(1<<UMIN_BIT_CONFIGURATION)))
      {
        upravlSetting = 0;//флаг Setting
        return ERROR_VALID2;
      }
      break;
    case 3://3-с контролем по току ИЛИ по напряжению
      if(!(edition_settings.configuration&(1<<MTZ_BIT_CONFIGURATION) ||
           edition_settings.configuration&(1<<UMIN_BIT_CONFIGURATION)))
      {
        upravlSetting = 0;//флаг Setting
        return ERROR_VALID2;
      }
      break;
    case 4://4-с контролем по току И по напряжению
      if(!(edition_settings.configuration&(1<<MTZ_BIT_CONFIGURATION)))
      {
        upravlSetting = 0;//флаг Setting
        return ERROR_VALID2;
      }
      if(!(edition_settings.configuration&(1<<UMIN_BIT_CONFIGURATION)))
      {
        upravlSetting = 0;//флаг Setting
        return ERROR_VALID2;
      }
      break;
    case 5://5-с контролем по 3I0
      if(!(edition_settings.configuration&(1<<ZZ_BIT_CONFIGURATION)))
      {
        upravlSetting = 0;//флаг Setting
        return ERROR_VALID2;
      }
      break;
    }//switch
  }//if(recordNumber == 5) if(editValue == (uint32_t*)&edition_settings.ctrl_zdz_type)
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return 0;
}//postFileZDZRegister21
