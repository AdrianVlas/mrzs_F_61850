
#include "header.h"
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

int privateIECGOOSEvalidRecordLen(int recordNumber, int recordLen);
int privateIECGOOSERangRecordNumber(int recordNumber);
int getFileIECGOOSERegister20(int recordNumber, int registerNumber, int recordLen);
int setFileIECGOOSERegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileIECGOOSERegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_GOOS 8
//�-�� ������ ������ ������ GOOS
#define PART_REGISTERS_GOOS 8

int privateIECGOOSEvalidRecordLen(int recordNumber, int recordLen)
{
//��������� recordNumber
  if(recordNumber > N_IN_GOOSE*PART_REGISTERS_GOOS) return MARKER_ERRORPERIMETR;
//��������� recordLen
  if(recordLen > REGISTERS_GOOS+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateIECGOOSEvalidRecordLen

int privateIECGOOSERangRecordNumber(int recordNumber)
{
  if(recordNumber > 0) return 1;
  return 0;
}//privateIECGOOSERangRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileIECGOOSERegister20(int recordNumber, int registerNumber, int recordLen)
{
//��������� recordLen
  if(privateIECGOOSEvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privateIECGOOSERangRecordNumber(recordNumber))
  {
    //������������
    int item     = (recordNumber - 1)/PART_REGISTERS_GOOS;
    int itemPart = (recordNumber - 1)%PART_REGISTERS_GOOS;
    return getRangN_SMALLModbusRegister2021(current_settings_interfaces.ranguvannja_In_GOOSE[item][itemPart],
                                            registerNumber-1);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileIECGOOSERegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileIECGOOSERegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//��������� recordLen
  if(privateIECGOOSEvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateIECGOOSERangRecordNumber(recordNumber))
    {
      if(validN_SMALLACMD(value) != 0) return MARKER_ERRORPERIMETR;
      if(value == 0) return 0;//������ ���������

      switch((unsigned short)value)
      {
      case 50000://- 1-� ��.�������    *
      case 50001://- 2-� ��.�������     *
      case 50002://- 3-� ��.�������     *
      case 50003://- 4-� ��.�������     *

//   case 50528://- ����.��      *
//   case 50529://- ����.��       *
//   case 50530://- ����.�����.��  *
//   case 50534://- ���� ��         *
//   case 50536://- �������� ����. *
//   case 50537://- �������� ����.  *
      case 50562://- ����� ���������
      case 50563://- ����� ����
      case 50569://- ̳��./�������. *
//   case 50576://- ����.�� ����.�.*
      case 50599://- �.���.���.�� ��

      case 50624://- ���������� ��.MMS 1
      case 50625://- ���������� ��.MMS 2
      case 50626://- ���������� ��.MMS 3
      case 50627://- ���������� ��.MMS 4
        return MARKER_ERRORPERIMETR; //�� ������ ���������
      }//switch
      return 0;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileGOOSERegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileIECGOOSERegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang

  if(privateIECGOOSERangRecordNumber(recordNumber))
  {
    int item     = (recordNumber - 1)/PART_REGISTERS_GOOS;
    int itemPart = (recordNumber - 1)%PART_REGISTERS_GOOS;
    writeRangN_SMALLModbusRegister2021(edition_settings.ranguvannja_In_GOOSE[item][itemPart], &dataPacket[1], recordLen-1);
    //��������� ������� - ���������
    upravlSchematic = 1;//���� Rang
  }//if

  return 0;
}//postFileIECGOOSERegister21
#endif
