#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 2380
//�������� ������� � ����� ������
#define END_ADR_REGISTER 2443

#define REGISTERS_VV 32

int privateVVBigGetReg2(int adrReg);

int getVVBigModbusRegister(int);//�������� ���������� ��������
int getVVBigModbusBit(int);//�������� ���������� ����
int setVVBigModbusRegister(int, int);// ��������
int setVVBigModbusBit(int, int);// ����

int  postVVBigWriteAction(void);//action ����� ������

SRAM1 COMPONENT_OBJ *vvbigcomponent;

/**************************************/
//���������� ���������� VV
/**************************************/
void constructorVVBigComponent(COMPONENT_OBJ *vvbigcomp)
{
  vvbigcomponent = vvbigcomp;

  vvbigcomponent->getModbusRegister = getVVBigModbusRegister;//�������� ���������� ��������
  vvbigcomponent->getModbusBit      = getVVBigModbusBit;//�������� ���������� ����
  vvbigcomponent->setModbusRegister = setVVBigModbusRegister;// ��������
  vvbigcomponent->setModbusBit      = setVVBigModbusBit;// ����

  vvbigcomponent->postWriteAction = postVVBigWriteAction;//action ����� ������
}//prepareDVinConfig

int getVVBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateVVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;
//����� ��������� ����
  unsigned int *ranguvannja_vv = &current_settings_interfaces.ranguvannja_on_cb[0]; //���������� ������
  if((offset/REGISTERS_VV)==1)
       ranguvannja_vv = &current_settings_interfaces.ranguvannja_off_cb[0]; //���������� ��������� 

  int bit = getSequenceN_BIGIndexActiveBit(offset%REGISTERS_VV, ranguvannja_vv);//������ ��������� ����
  if(bit!=-1)
    {
      int adr = decoderN_BIGACMD(bit);
      return adr<0? 0: adr+getACMDSmallBeginAdr();//������������ ������� ���� � ����� modbus  ��� rele
    }//if(bit!=N_BIG*32)
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getVVBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setVVBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateVVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(vvbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//������ �����
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  //��������� //������ �� //������ �B
  if(adrReg>=(BEGIN_ADR_REGISTER+REGISTERS_VV))
  {
   //������ �� ����
   if(dataReg==50532) return MARKER_ERRORPERIMETR;//������ ��
  }
  else {
   //������ �� ���
   if(dataReg==50533) return MARKER_ERRORPERIMETR;//������ ��
  }

  superSetOperativMarker(vvbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setVVBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postVVBigWriteAction(void) {
extern int upravlSchematic;//���� Rang
//action ����� ������
  int beginAdr = vvbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//�� ���� ������
  int endAdr   = vvbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray

  unsigned int tmp[N_BIG];//��� ���� ������������
  int idxObjOld=-1;//������ ��� ������ old
  //������ ������
  for(int i=0; i<countAdr; i++)
    {
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      unsigned int *ranguvannja_vv = &edition_settings.ranguvannja_on_cb[0]; //���������
      if((offset/REGISTERS_VV)==1)
           ranguvannja_vv = &edition_settings.ranguvannja_off_cb[0]; //���������� 

      if((offset/REGISTERS_VV)!=idxObjOld) {
       idxObjOld = offset/REGISTERS_VV;
       for(int dx=0; dx<N_BIG; dx++) tmp[dx]=ranguvannja_vv[dx];//���� ������ ����
      }//if

      int bitOld = getSequenceN_BIGIndexActiveBit(offset%REGISTERS_VV, tmp);//������ ��������� ����
      if(bitOld!=-1) {
        ranguvannja_vv[bitOld/REGISTERS_VV] &= ~(1<<(bitOld%REGISTERS_VV));
      }//if
    }//for
  //�������� �����
  for(int i=0; i<countAdr; i++)
    {
      unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//����� ��������
      if(adr==0) continue;
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      unsigned int *ranguvannja_vv = &edition_settings.ranguvannja_on_cb[0]; //���������
      if((offset/REGISTERS_VV)==1)
           ranguvannja_vv = &edition_settings.ranguvannja_off_cb[0]; //���������� 

      int bit = encoderN_BIGACMD(adr-getACMDSmallBeginAdr()); //���������� ������ modbus � ������ ���� ��� ����
      if(bit!=-1) {
        ranguvannja_vv[bit/REGISTERS_VV] |= (1<<(bit%REGISTERS_VV));
      }//if
    }//for

      //��������� ������� - ���������
     upravlSchematic = 1;//���� Rang

  return 0;
}//

int privateVVBigGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
