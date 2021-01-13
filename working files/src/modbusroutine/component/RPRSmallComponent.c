#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 16
//�������� ������� � ����� ������
#define END_ADR_REGISTER 99

int privateRPRSmallGetReg2(int adrReg);

int getRPRSmallModbusRegister(int);//�������� ���������� ��������
int getRPRSmallModbusBit(int);//�������� ���������� ����
int setRPRSmallModbusRegister(int, int);//�������� ���������� ��������
int setRPRSmallModbusBit(int, int);//�������� ���������� ����

void setRPRSmallCountObject(void);//�������� �-�� �������
void preRPRSmallReadAction(void);//action �� ������
void preRPRSmallWriteAction(void);//action �� ������
int  postRPRSmallWriteAction(void);//action ����� ������
int getRPRSmallModbusBeginAdrRegister(void);
int getRPRSmallModbusEndAdrRegister(void);

COMPONENT_OBJ *rprsmallcomponent;

int getRPRSmallModbusBeginAdrRegister(void)
{
  //�������� ����� ��� ��������
 return BEGIN_ADR_REGISTER;
}//getRPRSmallModbusBeginAdrRegister
int getRPRSmallModbusEndAdrRegister(void)
{
  //�������� ����� end ��������
 return END_ADR_REGISTER;
}//getRPRSmallModbusEndAdrRegister

/**************************************/
//���������� ���������� ������������ ���������������� ���������
/**************************************/
void constructorRPRSmallComponent(COMPONENT_OBJ *rprsmallcomp)
{
  rprsmallcomponent = rprsmallcomp;

  rprsmallcomponent->getModbusRegister = getRPRSmallModbusRegister;//�������� ���������� ��������
  rprsmallcomponent->getModbusBit      = getRPRSmallModbusBit;//�������� ���������� ����
  rprsmallcomponent->setModbusRegister = setRPRSmallModbusRegister;//�������� ���������� ��������
  rprsmallcomponent->setModbusBit      = setRPRSmallModbusBit;//�������� ���������� ����

  rprsmallcomponent->preReadAction   = preRPRSmallReadAction;//action �� ������
  rprsmallcomponent->preWriteAction  = preRPRSmallWriteAction;//action �� ������
  rprsmallcomponent->postWriteAction = postRPRSmallWriteAction;//action ����� ������

  rprsmallcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getRPRSmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateRPRSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superPreReadAction();//action �� ������
  int result = superReaderRegister(current_settings_interfaces.user_register[adrReg-BEGIN_ADR_REGISTER]);
  if(result&0xffff0000) return 0;

  return (result) &0xFFFF;
}//getDOUTModbusRegister(int adrReg)
int getRPRSmallModbusBit(int x)
{
  //�������� ���������� bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setRPRSmallModbusRegister(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setRPRSmallModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� bit
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void setRPRSmallCountObject(void) {
//�������� �-�� �������
}//
void preRPRSmallReadAction(void) {
//action �� ������
  rprsmallcomponent->isActiveActualData = 1;
}//
void preRPRSmallWriteAction(void) {
//action �� ������
  rprsmallcomponent->operativMarker[0] = -1;
  rprsmallcomponent->operativMarker[1] = -1;//����������� ������
  rprsmallcomponent->isActiveActualData = 1;
}//
int postRPRSmallWriteAction(void) {
//action ����� ������
  return 0;
}//

int privateRPRSmallGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
