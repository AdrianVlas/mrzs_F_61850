
#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 1016
//�������� ������� � ����� ������
#define END_ADR_REGISTER 1099

int privateRPRBigGetReg2(int adrReg);

int getRPRBigModbusRegister(int);//�������� ���������� ��������
int getRPRBigModbusBit(int);//�������� ���������� ����
int setRPRBigModbusRegister(int, int);//�������� ���������� ��������
int setRPRBigModbusBit(int, int);//�������� ���������� ����

void preRPRBigReadAction(void);//action �� ������
void preRPRBigWriteAction(void);//action �� ������
int  postRPRBigWriteAction(void);//action ����� ������
int getRPRSmallModbusBeginAdrRegister(void);
int getRPRSmallModbusEndAdrRegister(void);

COMPONENT_OBJ *rprbigcomponent;

/**************************************/
//���������� ���������� ������������ ���������������� ���������
/**************************************/
void constructorRPRBigComponent(COMPONENT_OBJ *rprbigcomp)
{
  rprbigcomponent = rprbigcomp;

  rprbigcomponent->getModbusRegister = getRPRBigModbusRegister;//�������� ���������� ��������
  rprbigcomponent->getModbusBit      = getRPRBigModbusBit;//�������� ���������� ����
  rprbigcomponent->setModbusRegister = setRPRBigModbusRegister;//�������� ���������� ��������
  rprbigcomponent->setModbusBit      = setRPRBigModbusBit;//�������� ���������� ����

  rprbigcomponent->preReadAction   = preRPRBigReadAction;//action �� ������
  rprbigcomponent->preWriteAction  = preRPRBigWriteAction;//action �� ������
  rprbigcomponent->postWriteAction = postRPRBigWriteAction;//action ����� ������

  rprbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getRPRBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateRPRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return (current_settings_interfaces.user_register[adrReg-BEGIN_ADR_REGISTER]) &0xFFFF;
}//getDOUTBigModbusRegister(int adrReg)
int getRPRBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setRPRBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateRPRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(rprbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//������ �����
    }//if(rprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(rprbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����
//������ ���������� ������ small ����������
  if(dataReg>=getRPRSmallModbusBeginAdrRegister() &&
      (dataReg<(getRPRSmallModbusEndAdrRegister()+1))) return MARKER_ERRORDIAPAZON;

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setRPRBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preRPRBigReadAction(void)
{
//action �� ������
  rprbigcomponent->isActiveActualData = 1;
}//
void preRPRBigWriteAction(void)
{
//action �� ������
  rprbigcomponent->operativMarker[0] = -1;
  rprbigcomponent->operativMarker[1] = -1;//����������� ������
  rprbigcomponent->isActiveActualData = 1;
}//
int postRPRBigWriteAction(void)
{
extern int upravlSetting;//���� Setting
//action ����� ������
  int beginAdr = rprbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//�� ���� ������
  int endAdr   = rprbigcomponent->operativMarker[1];
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  for(int i=0; i<countAdr; i++) {
    edition_settings.user_register[i+beginAdr-BEGIN_ADR_REGISTER] = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//����� ��������
  }//for

  //��������� ������� - ���������
     upravlSetting = 2;//���� Setting
     type_of_settings_changed |= (1 << USER_REGISTRY_CHANGED_BIT);

  return 0;
}//

int privateRPRBigGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
