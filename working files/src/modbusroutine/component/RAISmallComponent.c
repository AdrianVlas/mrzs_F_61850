
#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 409
//�������� ������� � ����� ������
#define END_ADR_REGISTER 768

int getRAISmallModbusRegister(int);//�������� ���������� ��������
int getRAISmallModbusBit(int);//�������� ���������� ����
int setRAISmallModbusRegister(int, int);//�������� ���������� ��������
int setRAISmallModbusBit(int, int);//�������� ���������� ����

void preRAISmallReadAction(void);//action �� ������
void preRAISmallWriteAction(void);//action �� ������
int  postRAISmallWriteAction(void);//action ����� ������

int privateRAISmallGetReg2(int adrReg);

COMPONENT_OBJ *raismallcomponent;

/**************************************/
//��������� ��������� � ������ ������
/**************************************/
void constructorRAISmallComponent(COMPONENT_OBJ *raismallcomp)
{
  raismallcomponent = raismallcomp;

  raismallcomponent->getModbusRegister = getRAISmallModbusRegister;//�������� ���������� ��������
  raismallcomponent->getModbusBit      = getRAISmallModbusBit;//�������� ���������� ����
  raismallcomponent->setModbusRegister = setRAISmallModbusRegister;//�������� ���������� ��������
  raismallcomponent->setModbusBit      = setRAISmallModbusBit;//�������� ���������� ����

  raismallcomponent->preReadAction   = preRAISmallReadAction;//action �� ������
  raismallcomponent->preWriteAction  = preRAISmallWriteAction;//action �� ������
  raismallcomponent->postWriteAction = postRAISmallWriteAction;//action ����� ������

  raismallcomponent->isActiveActualData = 0;
}//constructorIUSmallComponent(COMPONENT_OBJ *iucomp)

int getRAISmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateRAISmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
    switch ((adrReg-BEGIN_ADR_REGISTER)%35)
    {
    case 407-BEGIN_ADR_REGISTER://IA
      {
        return 0x55;
      }
    case 408-BEGIN_ADR_REGISTER://IB
      {
        return 0x55;
      }
    case 409-BEGIN_ADR_REGISTER://IC
      {
        return 0x55;
      }
    case 411-BEGIN_ADR_REGISTER://Uab
      {
        return 0x55;
      }
    case 414-BEGIN_ADR_REGISTER://3U0
      {
        return 0x55;
      }
    case 415-BEGIN_ADR_REGISTER://I
      {
        return 0x55;
      }
    case 417-BEGIN_ADR_REGISTER://I
      {
        return 0x55;
      }
    case 418-BEGIN_ADR_REGISTER://I
      {
        return 0x55;
      }
    case 419-BEGIN_ADR_REGISTER://I
      {
        return 0x55;
      }
    case 420-BEGIN_ADR_REGISTER://I
      {
        return 0x55;
      }
    case 421-BEGIN_ADR_REGISTER://I
      {
        return 0x55;
      }
    case 422-BEGIN_ADR_REGISTER://I
      {
        return 0x55;
      }
    case 438-BEGIN_ADR_REGISTER://
      {
        return 0x55;
      }

    }//switch
    return 0;
}//getDVModbusRegister(int adrReg)
int getRAISmallModbusBit(int x) {
  //�������� ���������� bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getIUModbusBit(int )
int setRAISmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setIUModbusRegister(int, int)
int setRAISmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� bit
  return MARKER_OUTPERIMETR;
}//setIUModbusBit(int, int )

void preRAISmallReadAction(void) {
//action �� ������
  raismallcomponent->isActiveActualData = 1;
}//

void preRAISmallWriteAction(void) {
//action �� ������
  raismallcomponent->operativMarker[0] = -1;
  raismallcomponent->operativMarker[1] = -1;//����������� ������
  raismallcomponent->isActiveActualData = 1;
}//
int postRAISmallWriteAction(void) {
//action ����� ������
  return 0;
}//

int privateRAISmallGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateRAISmallGetReg2(int adrReg)

