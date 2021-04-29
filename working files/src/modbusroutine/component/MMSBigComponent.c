#include "header.h"
#if (MODYFIKACIA_VERSII_PZ >= 10)

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 3660
//�������� ������� � ����� ������
#define END_ADR_REGISTER 3915

//�-�� ��������� ������ ����� ����� MMS
#define REGISTERS_MMS 8
//�-�� ������ ������ ������ MMS
#define PART_REGISTERS_MMS 8

int privateMMSBigGetReg2(int adrReg);

int getMMSBigModbusRegister(int);//�������� ���������� ��������
int getMMSBigModbusBit(int);//�������� ���������� ����
int setMMSBigModbusRegister(int, int);//�������� ���������� ��������
int setMMSBigModbusBit(int, int);//�������� ���������� ����

void setMMSBigCountObject(void);//�������� �-�� �������
void preMMSBigReadAction(void);//action �� ������
void preMMSBigWriteAction(void);//action �� ������
int  postMMSBigWriteAction(void);//action ����� ������

SRAM1 COMPONENT_OBJ *mmsbigcomponent;

/**************************************/
//���������� ���������� MMS
/**************************************/
void constructorMMSBigComponent(COMPONENT_OBJ *mmsbigcomp)
{
  mmsbigcomponent = mmsbigcomp;

  mmsbigcomponent->getModbusRegister = getMMSBigModbusRegister;//�������� ���������� ��������
  mmsbigcomponent->getModbusBit      = getMMSBigModbusBit;//�������� ���������� ����
  mmsbigcomponent->setModbusRegister = setMMSBigModbusRegister;//�������� ���������� ��������
  mmsbigcomponent->setModbusBit      = setMMSBigModbusBit;//�������� ���������� ����

  mmsbigcomponent->preReadAction   = preMMSBigReadAction;//action �� ������
  mmsbigcomponent->preWriteAction  = preMMSBigWriteAction;//action �� ������
  mmsbigcomponent->postWriteAction = postMMSBigWriteAction;//action ����� ������

  mmsbigcomponent->isActiveActualData = 0;
}//

int getMMSBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateMMSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  int offset = adrReg-BEGIN_ADR_REGISTER;
  int idxObj = offset/(REGISTERS_MMS*PART_REGISTERS_MMS);

//����� ��������� ����
  unsigned int *ranguvannja_mms = &current_settings_interfaces.ranguvannja_In_MMS[idxObj]
                                    [(offset%(REGISTERS_MMS*PART_REGISTERS_MMS))/REGISTERS_MMS][0]; //���������� 
  int bit = getSequenceN_SMALLIndexActiveBit(offset%REGISTERS_MMS, ranguvannja_mms);//������ ��������� ����
  if(bit!=-1)
    {
      int adr = decoderN_SMALLACMD(bit);
      return adr<0? 0: adr+getACMDSmallBeginAdr();//������������ ������� ���� � ����� modbus  ��� dv
    }//if(bit!=-1)
  return 0;
}//getMMSBigModbusRegister(int adrReg)
int getMMSBigModbusBit(int x)
{
  //�������� ���������� bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getMMSBigModbusBit(int adrReg)
int setMMSBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateMMSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(mmsbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//������ �����
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(mmsbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  if(dataReg==0) return 0;//������ ���������

  switch((unsigned short)dataReg)
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
}//
int setMMSBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//

void preMMSBigReadAction(void) {
//action �� ������
  mmsbigcomponent->isActiveActualData = 1;
}//
void preMMSBigWriteAction(void) {
//action �� ������
  mmsbigcomponent->operativMarker[0] = -1;
  mmsbigcomponent->operativMarker[1] = -1;//����������� ������
  mmsbigcomponent->isActiveActualData = 1;
}//
int postMMSBigWriteAction(void) {
extern int upravlSchematic;//���� Rang
//action ����� ������
  int beginAdr = mmsbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//�� ���� ������
  int endAdr   = mmsbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray

  unsigned int tmp[N_SMALL];//��� ���� ������������
  int idxObjOld=-1;//������ ��� ������ old
  //������ ������
  for(int i=0; i<countAdr; i++)
    {
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/(REGISTERS_MMS*PART_REGISTERS_MMS);
      //����� ��������� ����
      unsigned int *ranguvannja_mms = &edition_settings.ranguvannja_In_MMS[idxObj]
                                    [(offset%(REGISTERS_MMS*PART_REGISTERS_MMS))/REGISTERS_MMS][0]; //���������� 

      if((offset/REGISTERS_MMS)!=idxObjOld) {
       idxObjOld = offset/REGISTERS_MMS;
       for(int dx=0; dx<N_SMALL; dx++) tmp[dx]=ranguvannja_mms[dx];//���� ������ ����
      }//if

      int bitOld = getSequenceN_SMALLIndexActiveBit(offset%REGISTERS_MMS, tmp);//������ ��������� ����
      if(bitOld!=-1) {
        ranguvannja_mms[bitOld/32] &= ~(1<<(bitOld%32));
      }//if
    }//for
  //�������� �����
  for(int i=0; i<countAdr; i++)
    {
      unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//����� ��������
      if(adr==0) continue;
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/(REGISTERS_MMS*PART_REGISTERS_MMS);
      //����� ��������� ����
      unsigned int *ranguvannja_mms = &edition_settings.ranguvannja_In_MMS[idxObj]
                                    [(offset%(REGISTERS_MMS*PART_REGISTERS_MMS))/REGISTERS_MMS][0]; //���������� 

      int bit = encoderN_SMALLACMD(adr-getACMDSmallBeginAdr()); //���������� ������ modbus � ������ ���� ��� dv
      if(bit!=-1) {
        ranguvannja_mms[bit/32] |= (1<<(bit%32));
      }//if
    }//for

      //��������� ������� - ���������
     upravlSchematic = 1;//���� Rang

  return 0;
}//

int privateMMSBigGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateMMBigGetReg2(int adrReg)
#endif
