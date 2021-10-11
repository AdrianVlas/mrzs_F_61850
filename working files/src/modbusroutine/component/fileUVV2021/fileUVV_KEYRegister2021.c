
#include "header.h"

//�-�� ������ �� ����� key
#define REGISTERS_KEY 8

int privateKEYvalidRecordLen(int recordNumber, int recordLen);
int privateKEYSelUstRegisterNumber(int registerNumber);
int privateKEYSelRangRegisterNumber(int registerNumber);
int getFileKEYRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileKEYRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileKEYRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

/**************************************/
//���������� ���������� KEY
/**************************************/

int privateKEYvalidRecordLen(int recordNumber, int recordLen)
{
//��������� recordNumber
  if(recordNumber == 0) return MARKER_ERRORPERIMETR;
  if(recordNumber > NUMBER_DEFINED_BUTTONS) return MARKER_ERRORPERIMETR;
//��������� recordLen
  if(recordLen > REGISTERS_KEY+5) return MARKER_ERRORPERIMETR;
  return 0;
}//privateKEYvalidRecordLen

int privateKEYSelUstRegisterNumber(int registerNumber)
{
  if(registerNumber == 1) return 1;
  return 0;
}//privateKEYSelUstRegisterNumber
int privateKEYSelRangRegisterNumber(int registerNumber)
{
  if(registerNumber >= 5) return 1;
  return 0;
}//privateKEYSelRangRegisterNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileKEYRegister20(int recordNumber, int registerNumber, int recordLen)
{
//��������� recordLen recordNumber
  if(privateKEYvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int item  = recordNumber - 1;

  if(privateKEYSelUstRegisterNumber(registerNumber))
  {
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
    if(edition_settings.buttons_mode & (1<<item)) return 1;
    return 0;
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
  }//if
  else if(privateKEYSelRangRegisterNumber(registerNumber))
  {
    //������������ KEY
    return getRangN_SMALLModbusRegister2021(&current_settings_interfaces.ranguvannja_buttons[item*N_SMALL],
                                            registerNumber-5);
  }//if

  if(registerNumber < 5) return 0;
  return MARKER_ERRORPERIMETR;
}//getFileKEYRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileKEYRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//��������� recordLen recordNumber
  if(privateKEYvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateKEYSelUstRegisterNumber(registerNumber))
    {
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
      //�������a ust
      if(value > 1) return MARKER_ERRORDIAPAZON;
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
    }//if
    else if(privateKEYSelRangRegisterNumber(registerNumber))
    {
      //������������ KEY
      uint32_t maska = ((uint32_t)edition_settings.buttons_mode)&(1<<(recordNumber-1));
      //�������� �� ������������ ����� �������
      if(!maska)
      {
        switch((unsigned short)value)
        {
        case 50000://- 1-� ��.�������    *
        case 50001://- 2-� ��.�������     *
        case 50002://- 3-� ��.�������     *
        case 50003://- 4-� ��.�������     *
        case 50016://- ����.���1          *
        case 50032://- ����.���2          *
        case 50041://- ����.�����.���2    *
        case 50048://- ����.���3          *
        case 50064://- ����.���4
        case 50080://- ����.��� 0.4�� 1
        case 50083://- ����.��� 0.4�� 2
        case 50086://- ��.�.��� 0.4�� 2
        case 50112://- ����.���
        case 50113://- ���� ��� �� ��
        case 50128://- ����.���
        case 50144://- ����.TZNP1
        case 50154://- ����.TZNP2
        case 50164://- ����.TZNP3
        case 50176://- ����.���(���)
        case 50192://- ���������� �����1
        case 50197://- ���������� �����1
        case 50202://- ���� �����1
        case 50203://- ���� �����2
        case 50208://- ���������� ������1
        case 50211://- ���������� ������2
        case 50288://- ����.����.���
        case 50304://  ���� ���� �� ��
        case 50368://- ���/����
        case 50369://- ����.���1
        case 50370://- ����.���2
        case 50384://- ����.��1
        case 50387://- ����.��2
        case 50390://- ����.��3
        case 50393://- ����.��4
        case 50396://- ����.��5
        case 50399://- ����.��6
        case 50402://- ����.��7
        case 50405://- ����.��8
        case 50528://- ����.��      *
        case 50529://- ����.��       *
        case 50530://- ����.�����.��  *
        case 50534://- ���� ��         *
        case 50536://- �������� ����. *
        case 50537://- �������� ����.  *
//      case 50562://- ����� ���������
//      case 50563://- ����� ����
        case 50569://- ̳��./�������. *
        case 50576://- ����.�� ����.�.*
//      case 50599://- �.���.���.�� ��
          return MARKER_ERRORPERIMETR; //�� ������ ���������
        }//switch
        if(((unsigned short)value)>=50608) return MARKER_ERRORPERIMETR; //�� ������ ���������
      }//if(!maska)
      else
        //�������� �� ������������ ����� �����
        switch((unsigned short)value)
        {
//   case 50000://- 1-� ��.�������    *
//   case 50001://- 2-� ��.�������     *
//   case 50002://- 3-� ��.�������     *
//   case 50003://- 4-� ��.�������     *
//   case 50016://- ����.���1          *
//   case 50032://- ����.���2          *
//   case 50041://- ����.�����.���2    *
//   case 50048://- ����.���3          *
//   case 50064://- ����.���4
//   case 50080://- ����.��� 0.4�� 1
//   case 50083://- ����.��� 0.4�� 2
//   case 50086://- ��.�.��� 0.4�� 2
//   case 50112://- ����.���
        case 50113://- ���� ��� �� ��
//   case 50128://- ����.���
//   case 50144://- ����.TZNP1
//   case 50154://- ����.TZNP2
//   case 50164://- ����.TZNP3
//   case 50176://- ����.���(���)
//   case 50192://- ���������� �����1
//   case 50197://- ���������� �����1
        case 50202://- ���� �����1
        case 50203://- ���� �����2
//   case 50208://- ���������� ������1
//   case 50211://- ���������� ������2
//   case 50288://- ����.����.���
        case 50304://  ���� ���� �� ��
        case 50368://- ���/����
//   case 50369://- ����.���1
//   case 50370://- ����.���2
//   case 50384://- ����.��1
//   case 50387://- ����.��2
//   case 50390://- ����.��3
//   case 50393://- ����.��4
//   case 50396://- ����.��5
//   case 50399://- ����.��6
//   case 50402://- ����.��7
//   case 50405://- ����.��8
        case 50528://- ����.��      *
        case 50529://- ����.��       *
//   case 50530://- ����.�����.��  *
        case 50534://- ���� ��         *
        case 50536://- �������� ����. *
        case 50537://- �������� ����.  *
        case 50562://- ����� ���������
        case 50563://- ����� ����
//   case 50569://- ̳��./�������. *
        case 50576://- ����.�� ����.�.*
        case 50599://- �.���.���.�� ��
          return MARKER_ERRORPERIMETR; //�� ������ ���������
        }//switch

    }//if
  }//for
  return 0;
}//setFileKEYRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileKEYRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//���� Rang
  extern int upravlSetting;//���� Setting

  int item  = recordNumber - 1;
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)//������ ���������
  {
    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    if(privateKEYSelUstRegisterNumber(registerNumber))
    {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if(value) edition_settings.buttons_mode |= (1<<item);
      else  edition_settings.buttons_mode &= ~(1<<item);

      action_during_changing_button_mode(&current_settings_interfaces, &edition_settings);

      for (unsigned int i = 0; i < N_SMALL; i++)
      {
        edition_settings.ranguvannja_buttons[N_SMALL*item + i] =
          current_settings_interfaces.ranguvannja_buttons[N_SMALL*item + i];
      }//for
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
      upravlSetting = 1;//���� Setting
      upravlSchematic = 1;//���� Rang
    }//if
  }//for

  if(recordLen >= 5)
  {
    writeRangN_SMALLModbusRegister2021(&edition_settings.ranguvannja_buttons[item * N_SMALL], &dataPacket[5], recordLen-5);
    //��������� ������� - ���������
    upravlSchematic = 1;//���� Rang
  }//if

  return 0;
}//postFileKEYRegister21

