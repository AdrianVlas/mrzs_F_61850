
#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 100
//��������� bit � ����� ������
#define BEGIN_ADR_BIT 100

#if (                                \
     (MODYFIKACIA_VERSII_PZ == 0) || \
     (MODYFIKACIA_VERSII_PZ == 1) || \
     (MODYFIKACIA_VERSII_PZ == 3) || \
     (MODYFIKACIA_VERSII_PZ == 10)|| \
     (MODYFIKACIA_VERSII_PZ == 11)   \
    )
//�������� ������� � ����� ������
#define END_ADR_REGISTER 100
//�������� bit � ����� ������
#define END_ADR_BIT 115
#endif

#if (                                \
     (MODYFIKACIA_VERSII_PZ == 2) || \
     (MODYFIKACIA_VERSII_PZ == 4)    \
    )   
//�������� ������� � ����� ������
#define END_ADR_REGISTER 100
//�������� bit � ����� ������
#define END_ADR_BIT 108
#endif

#if (                                \
     (MODYFIKACIA_VERSII_PZ == 5) || \
     (MODYFIKACIA_VERSII_PZ == 15)   \
    )   
//�������� ������� � ����� ������
#define END_ADR_REGISTER 101
//�������� bit � ����� ������
#define END_ADR_BIT 119
#endif

int privateDOUTSmallGetReg2(int adrReg);
int privateDOUTSmallGetBit2(int adrBit);

int getDOUTSmallModbusRegister(int);//�������� ���������� ��������
int getDOUTSmallModbusBit(int);//�������� ���������� ����
int setDOUTSmallModbusRegister(int, int);//�������� �������
int setDOUTSmallModbusBit(int, int);//�������� ���

void setDOUTSmallCountObject(void);//�������� �-�� �������

void preDOUTSmallReadAction(void);//action �� ������
void preDOUTSmallWriteAction(void);//action �� ������
int  postDOUTSmallWriteAction(void);//action ����� ������

COMPONENT_OBJ *doutsmallcomponent;

/**************************************/
//���������� ���������� ��
/**************************************/
void constructorDOUTSmallComponent(COMPONENT_OBJ *doutcomp)
{
  doutsmallcomponent = doutcomp;

  doutsmallcomponent->getModbusRegister = getDOUTSmallModbusRegister;//�������� ���������� ��������
  doutsmallcomponent->getModbusBit      = getDOUTSmallModbusBit;//�������� ���������� ����
  doutsmallcomponent->setModbusRegister = setDOUTSmallModbusRegister;// ��������
  doutsmallcomponent->setModbusBit      = setDOUTSmallModbusBit;// ����

  doutsmallcomponent->preReadAction   = preDOUTSmallReadAction;//action �� ������
  doutsmallcomponent->preWriteAction  = preDOUTSmallWriteAction;//action �� ������
  doutsmallcomponent->postWriteAction = postDOUTSmallWriteAction;//action ����� ������
 
  doutsmallcomponent->isActiveActualData = 0;
}//constructorDOUTSmallComponent(COMPONENT_OBJ *doutcomp)

int getDOUTSmallModbusRegister(int adrReg) {
  //�������� ���������� ��������
  if(privateDOUTSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

//  return  (state_outputs) &0xFFFF;
  switch(adrReg-BEGIN_ADR_REGISTER)
  {
    case 0:
    return (state_outputs) &0xFFFF;
    case 1:
    return (state_outputs>>16)&0xFFFF;
  }//switch

  return 0;

}//getDOUTModbusRegister(int adrReg)
int getDOUTSmallModbusBit(int adrBit) {
  //�������� ���������� bit
  if(privateDOUTSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  short tmp   = state_outputs;
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDOUTModbusBit(int adrReg)
int setDOUTSmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTModbusRegister(int adrReg)
int setDOUTSmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTModbusRegister(int adrReg)

void preDOUTSmallReadAction(void) {
//action �� ������
  doutsmallcomponent->isActiveActualData = 1;
}//
void preDOUTSmallWriteAction(void) {
//action �� ������
  doutsmallcomponent->operativMarker[0] = -1;
  doutsmallcomponent->operativMarker[1] = -1;//����������� ������
  doutsmallcomponent->isActiveActualData = 1;
}//
int postDOUTSmallWriteAction(void) {
//action ����� ������
  return 0;
}//

int privateDOUTSmallGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateDOUTSmallGetBit2(int adrBit)
{
  //��������� ������� ��������
 return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)
