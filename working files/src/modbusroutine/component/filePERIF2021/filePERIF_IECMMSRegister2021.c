
#include "header.h"
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

int privateIECMMSvalidRecordLen(int recordNumber, int recordLen);
int privateIECMMSRangRecordNumber(int recordNumber);
int getFileIECMMSRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileIECMMSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileIECMMSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_MMS 8
//�-�� ������ ������ ������ MMS
#define PART_REGISTERS_MMS 8

int privateIECMMSvalidRecordLen(int recordNumber, int recordLen)
{
//��������� recordNumber
  if(recordNumber > N_IN_MMS*PART_REGISTERS_MMS) return MARKER_ERRORPERIMETR;
//��������� recordLen
  if(recordLen > REGISTERS_MMS+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateIECMMSvalidRecordLen

int privateIECMMSRangRecordNumber(int recordNumber)
{
  if(recordNumber > 0) return 1;
  return 0;
}//privateIECMMSRangRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileIECMMSRegister20(int recordNumber, int registerNumber, int recordLen)
{
//��������� recordLen
  if(privateIECMMSvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privateIECMMSRangRecordNumber(recordNumber))
  {
    //������������
    int item     = (recordNumber - 1)/PART_REGISTERS_MMS;
    int itemPart = (recordNumber - 1)%PART_REGISTERS_MMS;
    return getRangN_SMALLModbusRegister2021(current_settings_interfaces.ranguvannja_In_MMS[item][itemPart],
                                            registerNumber-1);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileIECMMSRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileIECMMSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//��������� recordLen
  if(privateIECMMSvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateIECMMSRangRecordNumber(recordNumber))
    {
      if(validN_SMALLACMD(value) != 0) return MARKER_ERRORPERIMETR;
      if(value == 0) return 0;//������ ���������

      switch((unsigned short)value)
      {
      case 50528://- ����.��      *
      case 50529://- ����.��       *
      case 50562://- ����� ���������
      case 50563://- ����� ����
      case 50599://- �.���.���.�� ��

      case 50432://OF
      case 50433://OF
      case 50434://OF
      case 50435://OF
      case 50436://OF
      case 50437://OF
      case 50438://OF
      case 50439://OF

      case 50464://DTR
      case 50465://DTR
      case 50466://DTR
      case 50467://DTR

      case 50468://DTR
      case 50469://DTR
      case 50470://DTR
      case 50471://DTR

        return 0; //������ ���������
      }//switch

      return MARKER_ERRORPERIMETR;//�� ������ ���������
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileIECMMSRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileIECMMSRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang

  if(privateIECMMSRangRecordNumber(recordNumber))
  {
    int item     = (recordNumber - 1)/PART_REGISTERS_MMS;
    int itemPart = (recordNumber - 1)%PART_REGISTERS_MMS;
    writeRangN_SMALLModbusRegister2021(edition_settings.ranguvannja_In_MMS[item][itemPart], &dataPacket[1], recordLen-1);
    //��������� ������� - ���������
    upravlSchematic = 1;//���� Rang
  }//if

  return 0;
}//postFileIECMMSRegister21
#endif
