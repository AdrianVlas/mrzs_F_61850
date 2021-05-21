#include "header.h"
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 3916
//�������� ������� � ����� ������
#define END_ADR_REGISTER 4171

//�-�� ��������� ������ ����� ����� LAN
#define REGISTERS_LAN 8
//�-�� ������ ������ ������ LAN
#define PART_REGISTERS_LAN 8

int privateLANBigGetReg2(int adrReg);

int getLANBigModbusRegister(int);//�������� ���������� ��������
int getLANBigModbusBit(int);//�������� ���������� ����
int setLANBigModbusRegister(int, int);//�������� ���������� ��������
int setLANBigModbusBit(int, int);//�������� ���������� ����

int  postLANBigWriteAction(void);//action ����� ������

COMPONENT_OBJ *lanbigcomponent;

/**************************************/
//���������� ���������� LAN
/**************************************/
void constructorLANBigComponent(COMPONENT_OBJ *lanbigcomp)
{
  lanbigcomponent = lanbigcomp;

  lanbigcomponent->getModbusRegister = getLANBigModbusRegister;//�������� ���������� ��������
  lanbigcomponent->getModbusBit      = getLANBigModbusBit;//�������� ���������� ����
  lanbigcomponent->setModbusRegister = setLANBigModbusRegister;//�������� ���������� ��������
  lanbigcomponent->setModbusBit      = setLANBigModbusBit;//�������� ���������� ����

  lanbigcomponent->postWriteAction = postLANBigWriteAction;//action ����� ������
}//

int getLANBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateLANBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  int offset = adrReg-BEGIN_ADR_REGISTER;
  int idxObj = offset/(REGISTERS_LAN*PART_REGISTERS_LAN);

//����� ��������� ����
  uint16_t *ranguvannja_lan = &current_settings_interfaces.ranguvannja_Out_LAN[idxObj]
                                    [(offset%(REGISTERS_LAN*PART_REGISTERS_LAN))/REGISTERS_LAN][0]; //���������� 

  int bitInOut  = ranguvannja_lan[offset%REGISTERS_LAN];
  int adr = -1;

  if(bitInOut!=0)
    {
      adr = decoderN_BIGACMD(bitInOut-1);
    }//if(bitInOut!=0)

  return adr<0? 0: adr+getACMDSmallBeginAdr();//������������ ������� ���� � ����� modbus  ��� rele
}//getLANBigModbusRegister(int adrReg)
int getLANBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getLANBigModbusBit(int adrReg)
int setLANBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateLANBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(lanbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//������ �����
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(lanbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

//  if(dataReg<0) return 0;
  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setLANBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setLANBigModbusBit

int postLANBigWriteAction(void) {
extern int upravlSchematic;//���� Rang
//action ����� ������
  int beginAdr = lanbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//�� ���� ������
  int endAdr   = lanbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray

  //�������� �����
  for(int i=0; i<countAdr; i++)
    {
      unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//����� ��������
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/(REGISTERS_LAN*PART_REGISTERS_LAN);
      //����� ��������� ����
      uint16_t *ranguvannja_lan = &edition_settings.ranguvannja_Out_LAN[idxObj]
                                    [(offset%(REGISTERS_LAN*PART_REGISTERS_LAN))/REGISTERS_LAN][0]; //���������� 

      ranguvannja_lan[offset%REGISTERS_LAN] = (encoderN_BIGACMD(adr-getACMDSmallBeginAdr())+1);//���������� ������ modbus � ������ ���� ��� ����
    }//for

      //��������� ������� - ���������
  upravlSchematic = 1;//���� Rang

  return 0;
}//

int privateLANBigGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateLANBigGetReg2(int adrReg)
#endif
