
#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 200
//��������� bit � ����� ������
#define BEGIN_ADR_BIT 20000

#define TMP_OUTPUTS (tmp = 0)

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 2) ||    \
     (MODYFIKACIA_VERSII_PZ == 4) ||    \
     (MODYFIKACIA_VERSII_PZ == 12)||    \
     (MODYFIKACIA_VERSII_PZ == 14)      \
    )   
//�������� ������� � ����� ������
#define END_ADR_REGISTER 200
//�������� bit � ����� ������
#define END_ADR_BIT 20008
#endif

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 6)     \
    )   
//�������� ������� � ����� ������
#define END_ADR_REGISTER 200
//�������� bit � ����� ������
#define END_ADR_BIT 20012
#endif

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 1) ||    \
     (MODYFIKACIA_VERSII_PZ == 3) ||    \
     (MODYFIKACIA_VERSII_PZ == 10) ||    \
     (MODYFIKACIA_VERSII_PZ == 11) ||    \
     (MODYFIKACIA_VERSII_PZ == 13)     \
    )   
//�������� ������� � ����� ������
#define END_ADR_REGISTER 200
//�������� bit � ����� ������
#define END_ADR_BIT 20015
#endif

#if (                                \
     (MODYFIKACIA_VERSII_PZ == 5) || \
     (MODYFIKACIA_VERSII_PZ == 15)   \
    )   
//�������� ������� � ����� ������
#define END_ADR_REGISTER 201
//�������� bit � ����� ������
#define END_ADR_BIT 20019

#undef TMP_OUTPUTS
#define TMP_OUTPUTS (tmp = (state_outputs_raw>>16)&0xF)
#endif

#ifdef  MODYFIKACIA_VERSII_DS

//�������� ������� � ����� ������
#define END_ADR_REGISTER 203
//�������� bit � ����� ������
#define END_ADR_BIT 20051

#undef TMP_DS
#define TMP_DS (tmp = ds&((1 << NUMBER_DS) - 1))
#endif

int privateDOUTSmallGetReg2(int adrReg);
int privateDOUTSmallGetBit2(int adrBit);

int getDOUTSmallModbusRegister(int);//�������� ���������� ��������
int getDOUTSmallModbusBit(int);//�������� ���������� ����
int setDOUTSmallModbusRegister(int, int);//�������� �������
int setDOUTSmallModbusBit(int, int);//�������� ���

int  postDOUTSmallWriteAction(void);//action ����� ������

SRAM1 COMPONENT_OBJ *doutsmallcomponent;

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

  doutsmallcomponent->postWriteAction = postDOUTSmallWriteAction;//action ����� ������
}//constructorDOUTSmallComponent(COMPONENT_OBJ *doutcomp)

int getDOUTSmallModbusRegister(int adrReg) {
  //�������� ���������� ��������
  if(privateDOUTSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int tmp   = state_outputs_raw&((1<<NUMBER_SIMPLE_OUTPUTS)-1);
  switch(adrReg-BEGIN_ADR_REGISTER)
  {
   case 0: tmp &= 0xFFFF; break;
   case 1: TMP_OUTPUTS; break;
#ifdef  MODYFIKACIA_VERSII_DS
   case 3: TMP_DS; break;
#endif   
   default: tmp = 0;
  }//switch

  return tmp;
}//getDOUTModbusRegister(int adrReg)

int getDOUTSmallModbusBit(int adrBit) {
  //�������� ���������� bit
  if(privateDOUTSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int tmp = state_outputs_raw&((1<<NUMBER_SIMPLE_OUTPUTS)-1);
  int offset = adrBit-BEGIN_ADR_BIT;
  switch(offset/16)
  {
   case 0: tmp &= 0xFFFF; break;
   case 1: TMP_OUTPUTS; break;
#ifdef  MODYFIKACIA_VERSII_DS
   case 3: TMP_DS; break;
#endif   
   default: tmp = 0;
  }//switch

  int maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
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
