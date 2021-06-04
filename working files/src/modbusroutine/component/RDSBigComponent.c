//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 15000
//�������� ������� � ����� ������
#define END_ADR_REGISTER 15001

#include "header.h"
//#include "cmd_log.h"

int privateRDSBigGetReg2(int adrReg);

int getRDSBigModbusRegister(int);//�������� ���������� ��������
int getRDSBigModbusBit(int);//�������� ���������� ����
int setRDSBigModbusRegister(int, int);//�������� ���������� ��������
int setRDSBigModbusBit(int, int);//�������� ���������� ����

int  postRDSBigWriteAction(void);//action ����� ������

SRAM1 COMPONENT_OBJ *rdsbigcomponent;

/**************************************/
//���������� ���������� ��� ����������
/**************************************/
void constructorRDSBigComponent(COMPONENT_OBJ *rdsbigcomp)
{
  rdsbigcomponent = rdsbigcomp;

  rdsbigcomponent->getModbusRegister = getRDSBigModbusRegister;//�������� ���������� ��������
  rdsbigcomponent->getModbusBit      = getRDSBigModbusBit;//�������� ���������� ����
  rdsbigcomponent->setModbusRegister = setRDSBigModbusRegister;//�������� ���������� ��������
  rdsbigcomponent->setModbusBit      = setRDSBigModbusBit;//�������� ���������� ����

  rdsbigcomponent->postWriteAction = postRDSBigWriteAction;//action ����� ������
}//prepareDVinConfig

int getRDSBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateRDSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;

  switch(offset)
  {
    case 0://�������� ����������� rds
      return MARKER_ERRORPERIMETR;
    case 1://���������� ������� rds
      return  GetAmountFixElem()&0xFFFF;
  }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getRDSBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setRDSBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateRDSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(rdsbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  int offset = adrReg-BEGIN_ADR_REGISTER;
  switch(offset)
  {
    case 0://�������� ���������� �����������
      if(dataReg!=0x1412) return MARKER_ERRORDIAPAZON;
      break;
    case 1://���������� ������� rds
      return MARKER_ERRORDIAPAZON;
  }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setRDSBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

int postRDSBigWriteAction(void)
{
//action ����� ������
  int beginAdr = rdsbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//�� ���� ������
  int endAdr   = rdsbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  for(int i=0; i<countAdr; i++)
    {
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;
      switch(offset)
      {
        case 0://�������� ����������� rds
          if(
            ((clean_rejestrators & CLEAN_SR_ERR) != 0)
          ) return ERROR_VALID2;//������ ���������

          clean_rejestrators |= CLEAN_SR_ERR;
          break;
      }//switch
    }//for

  return 0;
}//

int privateRDSBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

