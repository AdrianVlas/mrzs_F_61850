
#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 200
//��������� bit � ����� ������
#define BEGIN_ADR_BIT 200

#if (MODYFIKACIA_VERSII_PZ == 0)
//�������� ������� � ����� ������
#define END_ADR_REGISTER 201
//�������� bit � ����� ������
#define END_ADR_BIT 219
#endif

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 1) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   
//�������� ������� � ����� ������
#define END_ADR_REGISTER 200
//�������� bit � ����� ������
#define END_ADR_BIT 215
#endif

#if MODYFIKACIA_VERSII_PZ == 2    
//�������� ������� � ����� ������
#define END_ADR_REGISTER 200
//�������� bit � ����� ������
#define END_ADR_BIT 207
#endif

int privateDVSmallGetReg2(int adrReg);
int privateDVSmallGetBit2(int adrBit);

int getDVSmallModbusRegister(int);//�������� ���������� ��������
int getDVSmallModbusBit(int);//�������� ���������� ����
int setDVSmallModbusRegister(int, int);//�������� �������
int setDVSmallModbusBit(int, int);//�������� ���

void setDVSmallCountObject(void);//�������� �-�� �������

void preDVSmallReadAction(void);//action �� ������
void preDVSmallWriteAction(void);//action �� ������
int  postDVSmallWriteAction(void);//action ����� ������

COMPONENT_OBJ *dvsmallcomponent;

/**************************************/
//���������� ���������� ��
/**************************************/
void constructorDVSmallComponent(COMPONENT_OBJ *dvcomp)
{
  dvsmallcomponent = dvcomp;

  dvsmallcomponent->getModbusRegister = getDVSmallModbusRegister;//�������� ���������� ��������
  dvsmallcomponent->getModbusBit      = getDVSmallModbusBit;//�������� ���������� ����
  dvsmallcomponent->setModbusRegister = setDVSmallModbusRegister;// ��������
  dvsmallcomponent->setModbusBit      = setDVSmallModbusBit;// ����

  dvsmallcomponent->preReadAction   = preDVSmallReadAction;//action �� ������
  dvsmallcomponent->preWriteAction  = preDVSmallWriteAction;//action �� ������
  dvsmallcomponent->postWriteAction = postDVSmallWriteAction;//action ����� ������
 
  dvsmallcomponent->isActiveActualData = 0;
}//constructorDVSmallComponent(COMPONENT_OBJ *DVcomp)

int getDVSmallModbusRegister(int adrReg) {
  //�������� ���������� ��������
  if(privateDVSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  int  input_dv = state_inputs ^ current_settings_interfaces.type_of_input;
  switch(adrReg-BEGIN_ADR_REGISTER)
  {
    case 0:
    return (input_dv) &0xFFFF;
    case 1:
    return (input_dv>>16)&0xFFFF;
  }//switch

  return 0;
}//getDVModbusRegister(int adrReg)
int getDVSmallModbusBit(int adrBit) {
  //�������� ���������� bit
  if(privateDVSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int  input_dv = state_inputs ^ current_settings_interfaces.type_of_input;
  short tmp = input_dv &0xFFFF;
  if((adrBit-BEGIN_ADR_BIT)>=16) tmp = (input_dv>>16)&0xFFFF;
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDVModbusBit(int adrReg)
int setDVSmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)
int setDVSmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDVModbusRegister(int adrReg)

void preDVSmallReadAction(void) {
//action �� ������
  dvsmallcomponent->isActiveActualData = 1;
}//
void preDVSmallWriteAction(void) {
//action �� ������
  dvsmallcomponent->operativMarker[0] = -1;
  dvsmallcomponent->operativMarker[1] = -1;//����������� ������
  dvsmallcomponent->isActiveActualData = 1;
}//
int postDVSmallWriteAction(void) {
//action ����� ������
  return 0;
}//

int privateDVSmallGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateDVSmallGetBit2(int adrBit)
{
  //��������� ������� ��������
 return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)
