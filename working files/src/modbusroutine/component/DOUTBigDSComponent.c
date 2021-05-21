
#include "header.h"

#ifdef  MODYFIKACIA_VERSII_DS
//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 1404
//�������� ������� � ����� ������
#define END_ADR_REGISTER 1419

#define REGISTERS_OUTPUTS 16

int privateDOUTBigDSGetReg2(int adrReg);

int getDOUTBigDSModbusRegister(int);//�������� ���������� ��������
int getDOUTBigDSModbusBit(int);//�������� ���������� ����
int setDOUTBigDSModbusRegister(int, int);// ��������
int setDOUTBigDSModbusBit(int, int);// ����

int  postDOUTBigDSWriteAction(void);//action ����� ������

COMPONENT_OBJ *doutbigdscomponent;

/**************************************/
//���������� ���������� rele ��
/**************************************/
void constructorDOUTBigDSComponent(COMPONENT_OBJ *doutcomp)
{
  doutbigdscomponent = doutcomp;

  doutbigdscomponent->getModbusRegister = getDOUTBigDSModbusRegister;//�������� ���������� ��������
  doutbigdscomponent->getModbusBit      = getDOUTBigDSModbusBit;//�������� ���������� ����
  doutbigdscomponent->setModbusRegister = setDOUTBigDSModbusRegister;// ��������
  doutbigdscomponent->setModbusBit      = setDOUTBigDSModbusBit;// ����

  doutbigdscomponent->postWriteAction = postDOUTBigDSWriteAction;//action ����� ������
}//prepareDVinConfig

int getDOUTBigDSModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateDOUTBigDSGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_outputs[(N_BIG*NUMBER_SIMPLE_OUTPUTS)],
                                     REGISTERS_OUTPUTS, adrReg-BEGIN_ADR_REGISTER );
}//getDOUTBigDSModbusRegister(int adrReg)
int getDOUTBigDSModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigDSModbusRegister(int adrReg)
int setDOUTBigDSModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateDOUTBigDSGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(doutbigdscomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//������ �����
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(doutbigdscomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  //�������� �� ������
  if(!(current_settings.configuration & (1 << DS_BIT_CONFIGURATION))) return MARKER_ERRORPERIMETR;//������������ ��
  //�������� �� ������������
  return validN_BIGACMD(dataReg);
}//getDOUTBigDSModbusRegister(int adrReg)
int setDOUTBigDSModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigDSModbusRegister(int adrReg)

int postDOUTBigDSWriteAction(void)
{
extern int upravlSchematic;//���� Rang
//action ����� ������
  int beginAdr = doutbigdscomponent->operativMarker[0];
  if(beginAdr<0) return 0;//�� ���� ������
  int endAdr   = doutbigdscomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_outputs[(N_BIG*NUMBER_SIMPLE_OUTPUTS)], REGISTERS_OUTPUTS, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //��������� ������� - ���������
     upravlSchematic = 1;//���� Rang

  return 0;
}//

int privateDOUTBigDSGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
#endif
