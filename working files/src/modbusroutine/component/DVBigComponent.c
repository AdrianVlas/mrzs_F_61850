#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 1356
//�������� ������� � ����� ������
#define END_ADR_REGISTER 1515

int privateDVBigGetReg2(int adrReg);

int getDVBigModbusRegister(int);//�������� ���������� ��������
int getDVBigModbusBit(int);//�������� ���������� ����
int setDVBigModbusRegister(int, int);// ��������
int setDVBigModbusBit(int, int);// ����

void setDVBigCountObject(void);//�������� �-�� �������
void preDVBigReadAction(void);//action �� ������
void preDVBigWriteAction(void);//action �� ������
int  postDVBigWriteAction(void);//action ����� ������

COMPONENT_OBJ *dvbigcomponent;

/**************************************/
//���������� ���������� ��
/**************************************/
void constructorDVBigComponent(COMPONENT_OBJ *dvbigcomp)
{
  dvbigcomponent = dvbigcomp;

  dvbigcomponent->getModbusRegister = getDVBigModbusRegister;//�������� ���������� ��������
  dvbigcomponent->getModbusBit      = getDVBigModbusBit;//�������� ���������� ����
  dvbigcomponent->setModbusRegister = setDVBigModbusRegister;// ��������
  dvbigcomponent->setModbusBit      = setDVBigModbusBit;// ����

  dvbigcomponent->preReadAction   = preDVBigReadAction;//action �� ������
  dvbigcomponent->preWriteAction  = preDVBigWriteAction;//action �� ������
  dvbigcomponent->postWriteAction = postDVBigWriteAction;//action ����� ������

  dvbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getDVBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateDVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  return getRangN_SMALLModbusRegister(&current_settings.ranguvannja_inputs[0], 8, adrReg-BEGIN_ADR_REGISTER );
}//getDVBigModbusRegister(int adrReg)
int getDVBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setDVBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateDVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(dvbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings;//������ �����
    }//if(uprbigcomponent->isActiveActualData)
//  dvbigcomponent->isActiveActualData = 0;
  superClearActiveActualData();

  superSetOperativMarker(dvbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  return validN_SMALLACMD(dataReg);
}//setDVBigModbusRegister(int adrReg, int dataReg)
int setDVBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preDVBigReadAction(void) {
//action �� ������
  dvbigcomponent->isActiveActualData = 1;
}//
void preDVBigWriteAction(void) {
//action �� ������
  dvbigcomponent->operativMarker[0] = -1;
  dvbigcomponent->operativMarker[1] = -1;//����������� ������
  dvbigcomponent->isActiveActualData = 1;
}//
int postDVBigWriteAction(void) {
extern int upravlSchematic;//���� Rang
//action ����� ������
  int beginAdr = dvbigcomponent->operativMarker[0];
  int endAdr   = dvbigcomponent->operativMarker[1];
  if(beginAdr<0) return 0;//�� ���� ������
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  writeRangN_SMALLModbusRegister(&edition_settings.ranguvannja_inputs[0], 8, beginAdr,
                                countAdr, BEGIN_ADR_REGISTER);

      //��������� ������� - ���������
     upravlSchematic = 1;//���� Rang

//      current_settings = edition_settings;//��������� ���������

      //³������� ���� ���������
  //    _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
      //����������� �������, �� �� ����� ����� �������� ����������
    //  new_state_keyboard |= (1<<BIT_REWRITE);
      //fix_change_settings_m(1, 2);//1 - ����� ���������� 2 - USB
      //restart_timeout_idle_new_settings = true;

  return 0;
}//

int privateDVBigGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
