
#include "header.h"
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

int privateIECLANvalidRecordLen(int recordNumber, int recordLen);
int privateIECLANRangRecordNumber(int recordNumber);
int getFileIECLANRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileIECLANRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileIECLANRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_LAN 8
//�-�� ������ ������ ������ LAN
#define PART_REGISTERS_LAN 8

int privateIECLANvalidRecordLen(int recordNumber, int recordLen)
{
//��������� recordNumber
  if(recordNumber > N_OUT_LAN*PART_REGISTERS_LAN) return MARKER_ERRORPERIMETR;
//��������� recordLen
  if(recordLen > REGISTERS_LAN+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateIECLANvalidRecordLen

int privateIECLANRangRecordNumber(int recordNumber)
{
  if(recordNumber > 0) return 1;
  return 0;
}//privateIECLANRangRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileIECLANRegister20(int recordNumber, int registerNumber, int recordLen)
{
//��������� recordLen
  if(privateIECLANvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privateIECLANRangRecordNumber(recordNumber))
  {
    //������������
    int item     = (recordNumber - 1)/PART_REGISTERS_LAN;
    int itemPart = (recordNumber - 1)%PART_REGISTERS_LAN;
//����� ��������� ����
    uint16_t *ranguvannja_lan = current_settings_interfaces.ranguvannja_Out_LAN[item]
                                [itemPart]; //����������

    int bitInOut  = ranguvannja_lan[registerNumber-1];//offset%REGISTERS_LAN];
    int adr = -1;

    if(bitInOut!=0)
    {
      adr = decoderN_BIGACMD(bitInOut-1);
    }//if(bitInOut!=0)

    return adr<0? 0: adr+getACMDSmallBeginAdr();//������������ ������� ���� � ����� modbus  ��� rele
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileIECLANRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileIECLANRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//��������� recordLen
  if(privateIECLANvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateIECLANRangRecordNumber(recordNumber))
    {
      if(validN_BIGACMD(value) != 0) return MARKER_ERRORPERIMETR;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileIECLANRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileIECLANRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang

  if(privateIECLANRangRecordNumber(recordNumber))
  {
    int item     = (recordNumber - 1)/PART_REGISTERS_LAN;
    int itemPart = (recordNumber - 1)%PART_REGISTERS_LAN;

    //�������� �����
    for(int i=0; i<recordLen-1; i++)
    {
      unsigned int adr = (unsigned short) swapByteInShort(dataPacket[i]);//����� ��������

      //����� ��������� ����
      uint16_t *ranguvannja_lan = edition_settings.ranguvannja_Out_LAN[item]
                                  [itemPart]; //����������

      ranguvannja_lan[i] = (encoderN_BIGACMD(adr-getACMDSmallBeginAdr())+1);//���������� ������ modbus � ������ ���� ��� ����
    }//for

    //��������� ������� - ���������
    upravlSchematic = 1;//���� Rang
  }//if

  return 0;
}//postFileIECLANRegister21
#endif
