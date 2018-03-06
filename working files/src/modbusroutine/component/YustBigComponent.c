#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 61948
//�������� ������� � ����� ������
#define END_ADR_REGISTER 62100
extern int pointInterface;//����� ���������� 0-USB 1-RS485

int privateYustBigGetReg2(int adrReg);

int getYustBigModbusRegister(int);//�������� ���������� ��������
int getYustBigModbusBit(int);//�������� ���������� ����
int setYustBigModbusRegister(int, int);//�������� ���������� ��������
int setYustBigModbusBit(int, int);//�������� ���������� ����

void preYustBigReadAction(void);//action �� ������
void preYustBigWriteAction(void);//action �� ������
int postYustBigWriteAction(void);//action ����� ������

COMPONENT_OBJ *yustbigcomponent;

  int upravlYust=0;//���� ���������

/**************************************/
//���������� ���������� ���������
/**************************************/
void constructorYustBigComponent(COMPONENT_OBJ *yustbigcomp)
{
  yustbigcomponent = yustbigcomp;

  yustbigcomponent->getModbusRegister = getYustBigModbusRegister;//�������� ���������� ��������
  yustbigcomponent->getModbusBit      = getYustBigModbusBit;//�������� ���������� ����
  yustbigcomponent->setModbusRegister = setYustBigModbusRegister;//�������� ���������� ��������
  yustbigcomponent->setModbusBit      = setYustBigModbusBit;//�������� ���������� ����

  yustbigcomponent->preReadAction   = preYustBigReadAction;//action �� ������
  yustbigcomponent->preWriteAction  = preYustBigWriteAction;//action �� ������
  yustbigcomponent->postWriteAction = postYustBigWriteAction;//action ����� ������

  yustbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getYustBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateYustBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(yustbigcomponent, adrReg);

  int offset = adrReg-BEGIN_ADR_REGISTER;
  if(offset==0) return MARKER_ERRORPERIMETR;//61948 ������ ��������� ������ ����������
  if(offset==19) return MARKER_ERRORPERIMETR;//61967 ���������� ���������

  switch(offset) {//������ �������� �����
  case 20:
  case 21:
  case 22:
  case 23:
  case 24:
  case 25:
  case 26:
  case 27:
    return ustuvannja[offset-20];
  case 52://62000
    return current_settings_interfaces.number_iteration_el;//������������ ���������� ��������
  }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getYustBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setYustBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateYustBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(yustbigcomponent->isActiveActualData) {
    edit_serial_number_dev = serial_number_dev;
    for(int i=0; i<NUMBER_ANALOG_CANALES; i++) edit_ustuvannja[i] = ustuvannja[i];
  }//if
  yustbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(yustbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����
  if(adrReg==62000)
    if(dataReg<1 || dataReg>32) return MARKER_ERRORDIAPAZON;

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setYustBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� bit
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preYustBigReadAction(void) {
//action �� ������
  yustbigcomponent->operativMarker[0] = -1;
  yustbigcomponent->operativMarker[1] = -1;//����������� ������
  yustbigcomponent->isActiveActualData = 1;
}//

void preYustBigWriteAction(void) {
//action �� ������
  yustbigcomponent->operativMarker[0] = -1;
  yustbigcomponent->operativMarker[1] = -1;//����������� ������
  yustbigcomponent->isActiveActualData = 1;
}//

int postYustBigWriteAction(void) {
//action ����� ������
  int beginAdr = yustbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//�� ���� ������
  int endAdr   = yustbigcomponent->operativMarker[1];
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

//  int upravlYust=0;//���� ���������
  int upravlMin=0;//���� min param
  int number_iteration_el=-1;

  for(int i=0; i<countAdr; i++) {
    int offset = i+beginAdr-BEGIN_ADR_REGISTER;
    switch(offset) {//������ �������� �����
    case 0://�������� �����
      edit_serial_number_dev = tempWriteArray[offsetTempWriteArray+i];
      break;
    case 19://���������� ���������
      upravlYust = tempWriteArray[offsetTempWriteArray+i];//���������
      break;
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
      edit_ustuvannja[offset-20] = tempWriteArray[offsetTempWriteArray+i];
      break;
    case 52://62000 number_iteration_el
      number_iteration_el = tempWriteArray[offsetTempWriteArray+i];//number_iteration_el
      break;
    case 152://62100
      upravlMin = tempWriteArray[offsetTempWriteArray+i];//���� min param
      break;
    }//switch
  }//for

  if(number_iteration_el>0)
  {
    current_settings_interfaces.number_iteration_el = number_iteration_el;

    //³������� ���� ���������
    _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
    type_of_settings_changed |= (1 << SETTINGS_DATA_CHANGED_BIT);
    int typI = 2;
    if(pointInterface==1)//����� ���������� 0-USB 1-RS485
      typI = 3;
    if(set_new_settings_from_interface(typI)) return ERROR_VALID2;//2-USB
    return 0;
  }//if(number_iteration_el>0)

  if(upravlYust==0x1978) {//���� ���������

    changed_ustuvannja = CHANGED_ETAP_EXECUTION;
    serial_number_dev = edit_serial_number_dev;
    for(int i=0; i<NUMBER_ANALOG_CANALES; i++) ustuvannja[i] = edit_ustuvannja[i];

    changed_ustuvannja = CHANGED_ETAP_ENDED;
    _SET_BIT(control_i2c_taskes, TASK_START_WRITE_USTUVANNJA_EEPROM_BIT);
  }//if
  else if(upravlMin==0x1111) {

    //������� ��������� � "������� �������� ��������"
    min_settings(&current_settings_interfaces);

    //³������� ���� ���������
    _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
    restart_timeout_idle_new_settings = true;
    type_of_settings_changed = (1 << DEFAULT_SETTINGS_SET_BIT);

    int typI = 2;
    if(pointInterface==1)//����� ���������� 0-USB 1-RS485
      typI = 3;
    if(set_new_settings_from_interface(typI)) return ERROR_VALID2;
  }//if(upravlMin==0x1111)
  else return ERROR_VALID2;//���� ���������

  return 0;
}//

int privateYustBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)


