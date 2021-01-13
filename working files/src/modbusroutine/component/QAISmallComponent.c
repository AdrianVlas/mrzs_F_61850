
#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 61904
//�������� ������� � ����� ������
#define END_ADR_REGISTER 61921

int getQAISmallModbusRegister(int);//�������� ���������� ��������
int getQAISmallModbusBit(int);//�������� ���������� ����
int setQAISmallModbusRegister(int, int);//�������� ���������� ��������
int setQAISmallModbusBit(int, int);//�������� ���������� ����

void preQAISmallReadAction(void);//action �� ������
void preQAISmallWriteAction(void);//action �� ������
int  postQAISmallWriteAction(void);//action ����� ������
void loadQAISmallActualData(void);

int privateQAISmallGetReg2(int adrReg);

COMPONENT_OBJ *qaismallcomponent;

/**************************************/
//��������� ��������� �� ������ ����� ���������
/**************************************/
void constructorQAISmallComponent(COMPONENT_OBJ *qaismallcomp)
{
  qaismallcomponent = qaismallcomp;

  qaismallcomponent->getModbusRegister = getQAISmallModbusRegister;//�������� ���������� ��������
  qaismallcomponent->getModbusBit      = getQAISmallModbusBit;//�������� ���������� ����
  qaismallcomponent->setModbusRegister = setQAISmallModbusRegister;//�������� ���������� ��������
  qaismallcomponent->setModbusBit      = setQAISmallModbusBit;//�������� ���������� ����

  qaismallcomponent->preReadAction   = preQAISmallReadAction;//action �� ������
  qaismallcomponent->preWriteAction  = preQAISmallWriteAction;//action �� ������
  qaismallcomponent->postWriteAction = postQAISmallWriteAction;//action ����� ������

  qaismallcomponent->isActiveActualData = 0;
}//constructorIUSmallComponent(COMPONENT_OBJ *iucomp)

int getQAISmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateQAISmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
    int offset = adrReg-BEGIN_ADR_REGISTER;
    switch (offset)
    {
    case 3://��� ��������� 3Io
      {
        return (measurement_low[IM_3I0_i]) &0xFFFF;
      }
    }//switch

    return 0;
}//getDVModbusRegister(int adrReg)
int getQAISmallModbusBit(int x) {
  //�������� ���������� bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getIUModbusBit(int )
int setQAISmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setIUModbusRegister(int, int)
int setQAISmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� bit
  return MARKER_OUTPERIMETR;
}//setIUModbusBit(int, int )

void preQAISmallReadAction(void) {
//action �� ������
  qaismallcomponent->isActiveActualData = 1;
}//

void preQAISmallWriteAction(void) {
//action �� ������
  qaismallcomponent->operativMarker[0] = -1;
  qaismallcomponent->operativMarker[1] = -1;//����������� ������
  qaismallcomponent->isActiveActualData = 1;
}//
int postQAISmallWriteAction(void) {
//action ����� ������
 return 0;
}//

int privateQAISmallGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

