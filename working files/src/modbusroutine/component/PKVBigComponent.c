#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 12000
//�������� ������� � ����� ������
#define END_ADR_REGISTER 12042

int privatePKVBigGetReg2(int adrReg);

int getPKVBigModbusRegister(int);//�������� ���������� ��������
int getPKVBigModbusBit(int);//�������� ���������� ����
int setPKVBigModbusRegister(int, int);//�������� ���������� ��������
int setPKVBigModbusBit(int, int);//�������� ���������� ����

void setPKVBigCountObject(void);//�������� �-�� �������
void prePKVBigReadAction(void);//action �� ������
void prePKVBigWriteAction(void);//action �� ������
int  postPKVBigWriteAction(void);//action ����� ������
int PKVFunc000(int inOffset, int regPKV, uint32_t **editValue);

COMPONENT_OBJ *pkvbigcomponent;

int PKVFunc000(int inOffset, int regPKV, uint32_t **editValue)
{
  int diapazon = 1;

  switch(inOffset)
  {
  case 0://����� ��������� ������ ����� �������
    (*editValue) = &edition_settings.timeout_deactivation_password_interface_USB;
    if(regPKV<TIMEOUT_DEACTIVATION_PASSWORD_MIN || regPKV>TIMEOUT_DEACTIVATION_PASSWORD_MAX) diapazon=0;
    break;

//    case 1://��������/��������� ������
//      (*editValue) = &stubnull;
//      break;

  case 2://����-��� ���������� ���������
    (*editValue) = &edition_settings.timeout_idle_new_settings;
    if(regPKV<TIMEOUT_NEW_SETTINGS_MIN || regPKV>TIMEOUT_NEW_SETTINGS_MAX) diapazon=0;
    break;

  case 3://���� ����������������� ����������
    (*editValue) = (uint32_t *)&edition_settings.language;
    if(regPKV<VALUE_SETTING_LANGUAGE_MIN || regPKV>VALUE_SETTING_LANGUAGE_MAX) diapazon=0;
    break;

//    case 4:
//      (*editValue) = &stubnull;
//      break;

  case 5://�������� ����� �����
    (*editValue) = (uint32_t *)&edition_settings.speed_RS485;
    if(regPKV<VALUE_SPEED_INTERFACE_MIN || regPKV>VALUE_SPEED_INTERFACE_MAX) diapazon=0;
    break;

  case 6://���������� ����-���
    (*editValue) = (uint32_t *)&edition_settings.number_stop_bit_RS485;
    if(regPKV<1 || regPKV>MAX_NUMBER_STOP_BITS_INTERFACE) diapazon=0;
    break;

  case 7://�������
    (*editValue) = (uint32_t *)&edition_settings.pare_bit_RS485;
    if(regPKV<VALUE_PARE_INTERFACE_MIN || regPKV>VALUE_PARE_INTERFACE_MAX) diapazon=0;
    break;

  case 8://�������� �����
    (*editValue) = &edition_settings.time_out_1_RS485;
    if(regPKV<VALUE_TIME_OUT_1_INTERFACE_MIN || regPKV>VALUE_TIME_OUT_1_INTERFACE_MAX) diapazon=0;
    break;

//    case 9://�������� ����� ����� 2
//    case 10://���������� ����-��� 2
//    case 11://������� 2
//    case 12://�������� ����� 2
//      (*editValue) = &stubnull;
//      break;

  case 13://����� ���������� � ����
    (*editValue) = &edition_settings.address;
    if(regPKV<KOEF_ADDRESS_MIN || regPKV>KOEF_ADDRESS_MAX) diapazon=0;
    break;

//    case 14://������� ����� ������
//    case 15:
//      (*editValue) = &stubnull;
//      break;

//    case 16://������ 1 � 2
//      (*editValue) = &edition_settings.name_of_cell[0];
//      break;
//    case 17://������ 3 � 4
//      (*editValue) = &edition_settings.name_of_cell[2];
//      break;
//    case 18://������ 5 � 6
//      (*editValue) = &edition_settings.name_of_cell[4];
//      break;
//    case 19://������ 7 � 8
//      (*editValue) = &edition_settings.name_of_cell[6];
//      break;
//    case 20://������ 9 � 10
//      (*editValue) = &edition_settings.name_of_cell[8];
//      break;
//    case 21://������ 11 � 12
//      (*editValue) = &edition_settings.name_of_cell[10];
//      break;
//    case 22://������ 13 � 14
//      (*editValue) = &edition_settings.name_of_cell[12];
//      break;
//    case 23://������ 15 � 16
//      (*editValue) = &edition_settings.name_of_cell[14];
//      break;
  case 24://���
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x99) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 25://�����
    if(regPKV&0xff00) diapazon=0;
    if(regPKV==0) diapazon=0;
    if(regPKV>0x12) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 26://����
    if(regPKV&0xff00) diapazon=0;
    if(regPKV==0) diapazon=0;
    if(regPKV>0x31) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 27://���
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x23) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 28://������
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x59) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 29://�������
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x59) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 30://����� �������
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x99) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;

//    case 31://������� ����
//    case 32://������� �� ������/������ �����
//    case 33://����� �������� �� ������ �����
//    case 34://������ ������ �������� �� ������ �����
//    case 35://���� ������ �������� �� ������ �����
//    case 36://��� ������ �������� �� ������ �����
//    case 37://����� �������� �� ������ �����
//    case 38://������ ������ �������� �� ������ �����
//    case 39://���� ������ �������� �� ������ �����
//    case 40://��� ������ �������� �� ������ �����
//    case 41://������������� �������
//    case 42://����� ������ �������������
//      (*editValue) = &stubnull;
//      break;

  }//switch

  return diapazon;
} //PKVFunc000(int inOffset, int regPKV, uint32_t **editValue)

/**************************************/
//���������� ���������� PKV
/**************************************/
void constructorPKVBigComponent(COMPONENT_OBJ *pkvbigcomp)
{
  pkvbigcomponent = pkvbigcomp;

  pkvbigcomponent->getModbusRegister = getPKVBigModbusRegister;//�������� ���������� ��������
  pkvbigcomponent->getModbusBit      = getPKVBigModbusBit;//�������� ���������� ����
  pkvbigcomponent->setModbusRegister = setPKVBigModbusRegister;//�������� ���������� ��������
  pkvbigcomponent->setModbusBit      = setPKVBigModbusBit;//�������� ���������� ����

  pkvbigcomponent->preReadAction   = prePKVBigReadAction;//action �� ������
  pkvbigcomponent->preWriteAction  = prePKVBigWriteAction;//action �� ������
  pkvbigcomponent->postWriteAction = postPKVBigWriteAction;//action ����� ������

  pkvbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getPKVBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privatePKVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(pkvbigcomponent->isActiveActualData)
  {
    edition_settings = current_settings;//������ �����
  }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  unsigned char *label_to_time_array;
  if (copying_time == 2) label_to_time_array = time_copy;
  else label_to_time_array = time;

  uint32_t *editValue=NULL;
  PKVFunc000(adrReg-BEGIN_ADR_REGISTER, 0, &editValue);

  switch(adrReg-BEGIN_ADR_REGISTER)
  {
  case 1://��������/��������� ������
    return MARKER_ERRORPERIMETR;
  case 6://���������� ����-���
    return ((unsigned short)*editValue)+1;
  case 16://������ 1 � 2
    return edition_settings.name_of_cell[0] + ((edition_settings.name_of_cell[1]<<8)&0xFF00);
  case 17://������ 3 � 4
    return edition_settings.name_of_cell[2] + ((edition_settings.name_of_cell[3]<<8)&0xFF00);
  case 18://������ 5 � 6
    return edition_settings.name_of_cell[4] + ((edition_settings.name_of_cell[5]<<8)&0xFF00);
  case 19://������ 7 � 8
    return edition_settings.name_of_cell[6] + ((edition_settings.name_of_cell[7]<<8)&0xFF00);
  case 20://������ 9 � 10
    return edition_settings.name_of_cell[8] + ((edition_settings.name_of_cell[9]<<8)&0xFF00);
  case 21://������ 11 � 12
    return edition_settings.name_of_cell[10] + ((edition_settings.name_of_cell[11]<<8)&0xFF00);
  case 22://������ 13 � 14
    return edition_settings.name_of_cell[12] + ((edition_settings.name_of_cell[13]<<8)&0xFF00);
  case 23://������ 15 � 16
    return edition_settings.name_of_cell[14] + ((edition_settings.name_of_cell[15]<<8)&0xFF00);

  case 24://���
    return  *(label_to_time_array + 6);

  case 25://�����
    return *(label_to_time_array + 5);

  case 26://����
    return *(label_to_time_array + 4);

  case 27://���
    return *(label_to_time_array + 3);

  case 28://������
    return *(label_to_time_array + 2);

  case 29://�������
    return *(label_to_time_array + 1);

  case 30://����� �������
    return *(label_to_time_array + 0);
  }//switch

  if(editValue==NULL) return 0;
  return ((unsigned short)*editValue);
}//getDOUTBigModbusRegister(int adrReg)
int getPKVBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setPKVBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privatePKVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(pkvbigcomponent->isActiveActualData)
  {
    edition_settings = current_settings;//������ �����
  }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(pkvbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  uint32_t *editValue=NULL;
  if(!PKVFunc000(adrReg-BEGIN_ADR_REGISTER, dataReg, &editValue)) return MARKER_ERRORDIAPAZON;
//  if(editValue==NULL) return 0;

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setPKVBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void prePKVBigReadAction(void)
{
//action �� ������
  pkvbigcomponent->isActiveActualData = 1;
}//
void prePKVBigWriteAction(void)
{
//action �� ������
  pkvbigcomponent->operativMarker[0] = -1;
  pkvbigcomponent->operativMarker[1] = -1;//����������� ������
  pkvbigcomponent->isActiveActualData = 1;
}//
int postPKVBigWriteAction(void)
{
  extern int upravlSetting;//���� Setting
//action ����� ������
  int beginAdr = pkvbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//�� ���� ������
  int endAdr   = pkvbigcomponent->operativMarker[1];
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int flag_time_array = 0;

  unsigned char *label_to_time_array = time_edit;
  for(int i=0; i<7; i++) time_edit[i] = time[i];

  for(int i=0; i<countAdr; i++)
  {
    int offset = i+beginAdr-BEGIN_ADR_REGISTER;
    uint32_t *editValue=NULL;
    PKVFunc000(offset, 0, &editValue);
    switch(offset) {//������ ��������
    case 0://����� ��������� ������ ����� �������
    case 2://����-��� ���������� ���������
    case 3://���� ����������������� ����������
    case 5://�������� ����� �����
    case 7://�������
    case 8://�������� �����
    case 13://����� ���������� � ����
      *editValue = (tempWriteArray[offsetTempWriteArray+i]);
      upravlSetting = 1;//���� Setting
      break;

    case 6://���������� ����-���
      *editValue = (tempWriteArray[offsetTempWriteArray+i])-1;
      upravlSetting = 1;//���� Setting
      break;

    case 16://������ 1 � 2
      edition_settings.name_of_cell[0] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[1] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 17://������ 3 � 4
      edition_settings.name_of_cell[2] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[3] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 18://������ 5 � 6
      edition_settings.name_of_cell[4] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[5] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 19://������ 7 � 8
      edition_settings.name_of_cell[6] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[7] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 20://������ 9 � 10
      edition_settings.name_of_cell[8] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[9] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 21://������ 11 � 12
      edition_settings.name_of_cell[10] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[11] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 22://������ 13 � 14
      edition_settings.name_of_cell[12] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[13] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 23://������ 15 � 16
      edition_settings.name_of_cell[14] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[15] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;

    case 24://���
      *(label_to_time_array + 6) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 25://�����
      *(label_to_time_array + 5) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 26://����
      *(label_to_time_array + 4) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 27://���
      *(label_to_time_array + 3) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 28://������
      *(label_to_time_array + 2) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 29://�������
      *(label_to_time_array + 1) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 30://����� �������
      *(label_to_time_array + 0) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    }//switch
  }//for

  if(flag_time_array)
    if (check_data_for_data_time_menu() == 1)
    {
      //���� ���������
      _SET_BIT(control_i2c_taskes, TASK_START_WRITE_RTC_BIT);
      _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
    }//if
    else return ERROR_VALID2;

  return 0;
}//

int privatePKVBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

