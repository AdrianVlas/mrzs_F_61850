
#include "header.h"
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

int privatePKV5validRecordLen(int recordNumber, int recordLen);
int privatePKV5SelUstRecordNumber(int recordNumber);
int getFilePKV5Register20(int recordNumber, int registerNumber, int recordLen);
int setFilePKV5Register21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFilePKV5Register21(int recordNumber, int recordLen, unsigned short *dataPacket);

int privatePKV5SelUstRecordNumber(int recordNumber)
{
  if(recordNumber == 2) return 1;
  return 0;
}//privatePKV5SelUstRecordNumber

int privatePKV5validRecordLen(int recordNumber, int recordLen)
{
//��������� recordLen
  if(recordNumber == 2) if(recordLen > 6) return MARKER_ERRORPERIMETR;
  return 0;
}//privatePKV5validRecordLen

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFilePKV5Register20(int recordNumber, int registerNumber, int recordLen)
{
//��������� recordLen
  if(privatePKV5validRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privatePKV5SelUstRecordNumber(recordNumber))
  {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    switch(registerNumber)
    {
    case 1://IP ����� ����������
      return edition_settings.IP4[0]&0xff | (((edition_settings.IP4[1]&0xff)<<8)&0xFF00);
    case 2://IP ����� ����������
      return edition_settings.IP4[2]&0xff | (((edition_settings.IP4[3]&0xff)<<8)&0xFF00);
    case 3://����� �������
      return edition_settings.mask&0xff;
    case 4://���� ��-���������
      return edition_settings.gateway[0]&0xff | (((edition_settings.gateway[1]&0xff)<<8)&0xFF00);
    case 5://���� ��-���������
      return edition_settings.gateway[2]&0xff | (((edition_settings.gateway[3]&0xff)<<8)&0xFF00);
    }//switch
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  }//if

  return MARKER_ERRORPERIMETR;
}//getFilePKV5Register20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFilePKV5Register21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  UNUSED(dataPacket);
//��������� recordLen
  if(privatePKV5validRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  return 0;
}//setFilePKV5Register21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFilePKV5Register21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSetting;
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privatePKV5SelUstRecordNumber(recordNumber))
    {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
      switch(registerNumber)
      {
      case 1://IP ����� ����������
        edition_settings.IP4[0] = value & 0xff;
        edition_settings.IP4[1] = (value>>8) & 0xff;
        upravlSetting = 1;//���� Setting
        break;
      case 2://IP ����� ����������
        edition_settings.IP4[2] = value & 0xff;
        edition_settings.IP4[3] = (value>>8) & 0xff;
        upravlSetting = 1;//���� Setting
        break;

      case 3://����� �������
        edition_settings.mask = value & 0xff;
        upravlSetting = 1;//���� Setting
        break;

      case 4://���� ��-���������
        edition_settings.gateway[0] = value & 0xff;
        edition_settings.gateway[1] = (value>>8) & 0xff;
        upravlSetting = 1;//���� Setting
        break;
      case 5://���� ��-���������
        edition_settings.gateway[2] = value & 0xff;
        edition_settings.gateway[3] = (value>>8) & 0xff;
        upravlSetting = 1;//���� Setting
        break;
      }//switch
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    }//if

  }//for

  return 0;
}//postFilePKV5Register21

#endif

