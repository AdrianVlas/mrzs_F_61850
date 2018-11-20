
#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 1100
#define REGISTERS_OUTPUTS 16

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 1) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   
//�������� ������� � ����� ������
#define END_ADR_REGISTER 1355
#endif

#if (MODYFIKACIA_VERSII_PZ == 2)
//�������� ������� � ����� ������
#define END_ADR_REGISTER 1243
#endif

int privateDOUTBigGetReg2(int adrReg);

int getDOUTBigModbusRegister(int);//�������� ���������� ��������
int getDOUTBigModbusBit(int);//�������� ���������� ����
int setDOUTBigModbusRegister(int, int);// ��������
int setDOUTBigModbusBit(int, int);// ����

void preDOUTBigReadAction(void);//action �� ������
void preDOUTBigWriteAction(void);//action �� ������
int  postDOUTBigWriteAction(void);//action ����� ������

COMPONENT_OBJ *doutbigcomponent;

/**************************************/
//���������� ���������� rele
/**************************************/
void constructorDOUTBigComponent(COMPONENT_OBJ *doutcomp)
{
  doutbigcomponent = doutcomp;

  doutbigcomponent->getModbusRegister = getDOUTBigModbusRegister;//�������� ���������� ��������
  doutbigcomponent->getModbusBit      = getDOUTBigModbusBit;//�������� ���������� ����
  doutbigcomponent->setModbusRegister = setDOUTBigModbusRegister;// ��������
  doutbigcomponent->setModbusBit      = setDOUTBigModbusBit;// ����

  doutbigcomponent->preReadAction   = preDOUTBigReadAction;//action �� ������
  doutbigcomponent->preWriteAction  = preDOUTBigWriteAction;//action �� ������
  doutbigcomponent->postWriteAction = postDOUTBigWriteAction;//action ����� ������

  doutbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getDOUTBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateDOUTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_outputs[0], REGISTERS_OUTPUTS, adrReg-BEGIN_ADR_REGISTER );
}//getDOUTBigModbusRegister(int adrReg)
int getDOUTBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setDOUTBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateDOUTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(doutbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//������ �����
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(doutbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����
  //�������� �� ������������
  return validN_BIGACMD(dataReg);
}//getDOUTBigModbusRegister(int adrReg)
int setDOUTBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preDOUTBigReadAction(void)
{
//action �� ������
  doutbigcomponent->isActiveActualData = 1;
}//
void preDOUTBigWriteAction(void)
{
//action �� ������
  doutbigcomponent->operativMarker[0] = -1;
  doutbigcomponent->operativMarker[1] = -1;//����������� ������
  doutbigcomponent->isActiveActualData = 1;
}//
int postDOUTBigWriteAction(void)
{
extern int upravlSchematic;//���� Rang
//action ����� ������
  int beginAdr = doutbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//�� ���� ������
  int endAdr   = doutbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_outputs[0], REGISTERS_OUTPUTS, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //��������� ������� - ���������
     upravlSchematic = 1;//���� Rang

  return 0;
}//

int privateDOUTBigGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
