#include "header.h"
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 2636
//�������� ������� � ����� ������
#define END_ADR_REGISTER 3659

//�-�� ��������� ������ ����� ����� GOOS
#define REGISTERS_GOOS 8
//�-�� ������ ������ ������ GOOS
#define PART_REGISTERS_GOOS 8

int privateGOOSBigGetReg2(int adrReg);

int getGOOSBigModbusRegister(int);//�������� ���������� ��������
int getGOOSBigModbusBit(int);//�������� ���������� ����
int setGOOSBigModbusRegister(int, int);//�������� ���������� ��������
int setGOOSBigModbusBit(int, int);//�������� ���������� ����

int  postGOOSBigWriteAction(void);//action ����� ������

COMPONENT_OBJ *goosbigcomponent;

/**************************************/
//���������� ���������� GOOS
/**************************************/
void constructorGOOSBigComponent(COMPONENT_OBJ *goosbigcomp)
{
  goosbigcomponent = goosbigcomp;

  goosbigcomponent->getModbusRegister = getGOOSBigModbusRegister;//�������� ���������� ��������
  goosbigcomponent->getModbusBit      = getGOOSBigModbusBit;//�������� ���������� ����
  goosbigcomponent->setModbusRegister = setGOOSBigModbusRegister;//�������� ���������� ��������
  goosbigcomponent->setModbusBit      = setGOOSBigModbusBit;//�������� ���������� ����

  goosbigcomponent->postWriteAction = postGOOSBigWriteAction;//action ����� ������
}//

int getGOOSBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateGOOSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  int offset = adrReg-BEGIN_ADR_REGISTER;
  int idxObj = offset/(REGISTERS_GOOS*PART_REGISTERS_GOOS);

//����� ��������� ����
  unsigned int *ranguvannja_goos = &current_settings_interfaces.ranguvannja_In_GOOSE[idxObj]
                                    [(offset%(REGISTERS_GOOS*PART_REGISTERS_GOOS))/REGISTERS_GOOS][0]; //���������� 
  int bit = getSequenceN_SMALLIndexActiveBit(offset%REGISTERS_GOOS, ranguvannja_goos);//������ ��������� ����
  if(bit!=-1)
    {
      int adr = decoderN_SMALLACMD(bit);
      return adr<0? 0: adr+getACMDSmallBeginAdr();//������������ ������� ���� � ����� modbus  ��� dv
    }//if(bit!=N_BIG*32)
  return 0;
}//getGOOSBigModbusRegister(int adrReg)
int getGOOSBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setGOOSBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateGOOSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(goosbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//������ �����
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(goosbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  if(validN_SMALLACMD(dataReg)==MARKER_ERRORPERIMETR) return MARKER_ERRORPERIMETR;
  if(dataReg==0) return 0;//������ ���������

  switch((unsigned short)dataReg)
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
}//
int setGOOSBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//

int postGOOSBigWriteAction(void) {
extern int upravlSchematic;//���� Rang
//action ����� ������
  int beginAdr = goosbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//�� ���� ������
  int endAdr   = goosbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray

  unsigned int tmp[N_SMALL];//��� ���� ������������
  int idxObjOld=-1;//������ ��� ������ old
  //������ ������
  for(int i=0; i<countAdr; i++)
    {
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/(REGISTERS_GOOS*PART_REGISTERS_GOOS);
      //����� ��������� ����
      unsigned int *ranguvannja_goos = &edition_settings.ranguvannja_In_GOOSE[idxObj]
                                    [(offset%(REGISTERS_GOOS*PART_REGISTERS_GOOS))/REGISTERS_GOOS][0]; //���������� 

      if((offset/REGISTERS_GOOS)!=idxObjOld) {
       idxObjOld = offset/REGISTERS_GOOS;
       for(int dx=0; dx<N_SMALL; dx++) tmp[dx]=ranguvannja_goos[dx];//���� ������ ����
      }//if

      int bitOld = getSequenceN_SMALLIndexActiveBit(offset%REGISTERS_GOOS, tmp);//������ ��������� ����
      if(bitOld!=-1) {
        ranguvannja_goos[bitOld/32] &= ~(1<<(bitOld%32));
      }//if
    }//for
  //�������� �����
  for(int i=0; i<countAdr; i++)
    {
      unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//����� ��������
      if(adr==0) continue;
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/(REGISTERS_GOOS*PART_REGISTERS_GOOS);
      //����� ��������� ����
       unsigned int *ranguvannja_goos = &edition_settings.ranguvannja_In_GOOSE[idxObj]
                                    [(offset%(REGISTERS_GOOS*PART_REGISTERS_GOOS))/REGISTERS_GOOS][0]; //���������� 

      int bit = encoderN_SMALLACMD(adr-getACMDSmallBeginAdr()); //���������� ������ modbus � ������ ���� ��� dv
      if(bit!=-1) {
        ranguvannja_goos[bit/32] |= (1<<(bit%32));
      }//if
    }//for

      //��������� ������� - ���������
     upravlSchematic = 1;//���� Rang

  return 0;
}//

int privateGOOSBigGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGOOSBigGetReg2(int adrReg)
#endif
