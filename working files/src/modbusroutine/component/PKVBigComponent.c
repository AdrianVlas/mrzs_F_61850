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
int passwordImunitetRegPKVBigComponent(int x);

COMPONENT_OBJ *pkvbigcomponent;

int PKVFunc000(int inOffset, int regPKV, uint32_t **editValue)
{
  int diapazon = 1;

  switch(inOffset)
  {
  case 0://����� ��������� ������ ����� �������
    switch(pointInterface)//����� ���������� 0-USB 1-RS485
    {
     case USB_RECUEST:
      (*editValue) = &edition_settings.timeout_deactivation_password_interface_USB;
     break;
     case RS485_RECUEST:
      (*editValue) = &edition_settings.timeout_deactivation_password_interface_RS485;
     break;
#if (MODYFIKACIA_VERSII_PZ >= 10)
     case LAN_RECUEST:
      (*editValue) = &edition_settings.timeout_deactivation_password_interface_LAN;
     break;
#endif
    }//switch
    if(regPKV<TIMEOUT_DEACTIVATION_PASSWORD_MIN || regPKV>TIMEOUT_DEACTIVATION_PASSWORD_MAX) diapazon=0;
    break;

#define IMUNITET_PKV1 1
  case IMUNITET_PKV1://��������/��������� ������
    switch(pointInterface)//����� ���������� 0-USB 1-RS485
    {
     case USB_RECUEST:
      (*editValue) = &edition_settings.password_interface_USB;
     break;
     case RS485_RECUEST:
      (*editValue) = &edition_settings.password_interface_RS485;
     break;
#if (MODYFIKACIA_VERSII_PZ >= 10)
     case LAN_RECUEST:
      (*editValue) = &edition_settings.password_interface_LAN;
     break;
#endif
    }//switch
    break;

  case 2://����-��� ���������� ���������
    (*editValue) = &edition_settings.timeout_idle_new_settings;
    if(regPKV<TIMEOUT_NEW_SETTINGS_MIN || regPKV>TIMEOUT_NEW_SETTINGS_MAX) diapazon=0;
    break;

  case 3://���� ����������������� ����������
    (*editValue) = (uint32_t *)&edition_settings.language;
    if(regPKV<VALUE_SETTING_LANGUAGE_MIN || regPKV>VALUE_SETTING_LANGUAGE_MAX) diapazon=0;
    break;

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


  case 31://���
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x99) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 32://�����
    if(regPKV&0xff00) diapazon=0;
    if(regPKV==0) diapazon=0;
    if(regPKV>0x12) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 33://����
    if(regPKV&0xff00) diapazon=0;
    if(regPKV==0) diapazon=0;
    if(regPKV>0x31) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 34://���
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x23) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 35://������
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x59) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 36://�������
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x59) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 37://����� �������
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x99) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;

//    case 38://������� ����
//    case 39://������� �� ������/������ �����
//    case 40://����� �������� �� ������ �����
//    case 41://������ ������ �������� �� ������ �����
//    case 42://���� ������ �������� �� ������ �����
//    case 43://��� ������ �������� �� ������ �����
//    case 44://����� �������� �� ������ �����
//    case 45://������ ������ �������� �� ������ �����
//    case 46://���� ������ �������� �� ������ �����
//    case 47://��� ������ �������� �� ������ �����
//    case 48://������������� �������
//    case 49://����� ������ �������������
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
    edition_settings = current_settings_interfaces;//������ �����
  }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  time_t time_dat_tmp;
  int32_t time_ms_tmp;
  if (save_time_dat_l == 3) time_dat_tmp = time_dat_save_l;
  else
  { 
    copying_time_dat = 1;
    time_dat_tmp = time_dat_copy;
    time_ms_tmp = time_ms_copy;
    copying_time_dat = 0;
  }
  struct tm *p;
  p = localtime(&time_dat_tmp);

  uint32_t *editValue=NULL;
  PKVFunc000(adrReg-BEGIN_ADR_REGISTER, 0, &editValue);

  switch(adrReg-BEGIN_ADR_REGISTER)
  {
  case 1://��������/��������� ������
    return MARKER_ERRORPERIMETR;
  case 6://���������� ����-���
    return (((unsigned short)*editValue)+1) &0xFFFF;

#if (MODYFIKACIA_VERSII_PZ >= 10)
    case 16://IP ����� ����������
     return edition_settings.IP4[0]&0xff | (((edition_settings.IP4[1]&0xff)<<8)&0xFF00);
    break;
    case 17://IP ����� ����������
     return edition_settings.IP4[2]&0xff | (((edition_settings.IP4[3]&0xff)<<8)&0xFF00);
    break;
    case 18://����� �������
     return edition_settings.mask&0xff;
    break;
    case 19://���� ��-���������
     return edition_settings.gateway[0]&0xff | (((edition_settings.gateway[1]&0xff)<<8)&0xFF00);
    break;
    case 20://���� ��-���������
     return edition_settings.gateway[2]&0xff | (((edition_settings.gateway[3]&0xff)<<8)&0xFF00);
    break;
#endif

  case 23://������ 1 � 2
    return (edition_settings.name_of_cell[0]&0xFF) | ((edition_settings.name_of_cell[1]<<8)&0xFF00);
  case 24://������ 3 � 4
    return (edition_settings.name_of_cell[2]&0xFF) | ((edition_settings.name_of_cell[3]<<8)&0xFF00);
  case 25://������ 5 � 6
    return (edition_settings.name_of_cell[4]&0xFF) | ((edition_settings.name_of_cell[5]<<8)&0xFF00);
  case 26://������ 7 � 8
    return (edition_settings.name_of_cell[6]&0xFF) | ((edition_settings.name_of_cell[7]<<8)&0xFF00);
  case 27://������ 9 � 10
    return (edition_settings.name_of_cell[8]&0xFF) | ((edition_settings.name_of_cell[9]<<8)&0xFF00);
  case 28://������ 11 � 12
    return (edition_settings.name_of_cell[10]&0xFF) | ((edition_settings.name_of_cell[11]<<8)&0xFF00);
  case 29://������ 13 � 14
    return (edition_settings.name_of_cell[12]&0xFF) | ((edition_settings.name_of_cell[13]<<8)&0xFF00);
  case 30://������ 15 � 16
    return (edition_settings.name_of_cell[14]&0xFF) | ((edition_settings.name_of_cell[15]<<8)&0xFF00);

  case 31://���
    return  INT_TO_BCD(p->tm_year - 100) &0xFF;

  case 32://�����
    return INT_TO_BCD(p->tm_mon + 1) &0xFF;

  case 33://����
    return INT_TO_BCD(p->tm_mday) &0xFF;

  case 34://���
    return INT_TO_BCD(p->tm_hour) &0xFF;

  case 35://������
    return INT_TO_BCD(p->tm_min) &0xFF;

  case 36://�������
    return INT_TO_BCD(p->tm_sec) &0xFF;

  case 37://����� �������
    return INT_TO_BCD(time_ms_tmp/10) &0xFF;
  }//switch

  if(editValue==NULL) return 0;
  return (((unsigned short)*editValue)) &0xFFFF;
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
    edition_settings = current_settings_interfaces;//������ �����
  }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(pkvbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  uint32_t *editValue=NULL;
  if(!PKVFunc000(adrReg-BEGIN_ADR_REGISTER, dataReg, &editValue)) return MARKER_ERRORDIAPAZON;

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
  //������� ������ ����� ���� � ����� ��� �����������
  copying_time_dat = 1;
  time_t time_dat_tmp = time_dat_copy;
  int32_t time_ms_tmp = time_ms_copy;
  copying_time_dat = 0;
  struct tm *p;
  p = localtime(&time_dat_tmp);
  time_edit[0] = INT_TO_BCD(time_ms_tmp/10);
  time_edit[1] = INT_TO_BCD(p->tm_sec) & 0x7F;
  time_edit[2] = INT_TO_BCD(p->tm_min) & 0x7F;
  time_edit[3] = INT_TO_BCD(p->tm_hour) & 0x3F;
  time_edit[4] = INT_TO_BCD(p->tm_mday) & 0x3F;
  time_edit[5] = INT_TO_BCD(p->tm_mon + 1) & 0x1F;
  time_edit[6] = INT_TO_BCD(p->tm_year - 100) & 0xFF;

  for(int i=0; i<countAdr; i++)
  {
    int offset = i+beginAdr-BEGIN_ADR_REGISTER;
    uint32_t *editValue=NULL;
    PKVFunc000(offset, 0, &editValue);
    switch(offset)  //������ ��������
    {
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

    case 1://��������/��������� ������
      {
      int passwordS=-1;
      switch(pointInterface)//����� ���������� 0-USB 1-RS485
      {
       case USB_RECUEST:
        passwordS = password_set_USB;//������ ����������
       break;
       case RS485_RECUEST:
        passwordS = password_set_RS485;//������ ����������
       break;
#if (MODYFIKACIA_VERSII_PZ >= 10)
       case LAN_RECUEST:
        passwordS = password_set_LAN;//������ ����������
       break;
#endif
      }//switch
      unsigned short new_parol = tempWriteArray[offsetTempWriteArray+i];

      if(passwordS==1) //������ �� ����
      {
        //����� check parol
        if((*editValue) == new_parol)
        {
          //�������� ������ ������
          switch(pointInterface)//����� ���������� 0-USB 1-RS485
          {
           case USB_RECUEST:
            password_set_USB=0;//������ ����������
           break;
           case RS485_RECUEST:
            password_set_RS485=0;//������ ����������
           break;
#if (MODYFIKACIA_VERSII_PZ >= 10)
           case LAN_RECUEST:
            password_set_LAN=0;//������ ����������
           break;
#endif
          }//switch
        }//if
        else return ERROR_VALID2;//������������ ������
        break;
      }//if(passwordS==1) //������ �� ����
      else
      {//������ ����
        //����� check parol
        if((*editValue) == new_parol)
        {
          if(new_parol==0) break;//��������� ������ �������� ������ �� ���������� ����
          //������� ������ ������
          switch(pointInterface)//����� ���������� 0-USB 1-RS485
          {
           case USB_RECUEST:
            password_set_USB=1;//������ ����������
           break;
           case RS485_RECUEST:
            password_set_RS485=1;//������ ����������
           break;
#if (MODYFIKACIA_VERSII_PZ >= 10)
           case LAN_RECUEST:
            password_set_LAN=1;//������ ����������
           break;
#endif
          }//switch
        }//if
      }//else
      //�������� ����� ������
      *editValue = (unsigned short)(tempWriteArray[offsetTempWriteArray+i]);
      upravlSetting = 1;//���� Setting
      } break;

    case 6://���������� ����-���
      *editValue = (tempWriteArray[offsetTempWriteArray+i])-1;
      upravlSetting = 1;//���� Setting
      break;

#if (MODYFIKACIA_VERSII_PZ >= 10)
    case 16://IP ����� ����������
     edition_settings.IP4[0] = tempWriteArray[offsetTempWriteArray+i] & 0xff;
     edition_settings.IP4[1] = (tempWriteArray[offsetTempWriteArray+i]>>8) & 0xff;
    break;
    case 17://IP ����� ����������
     edition_settings.IP4[2] = tempWriteArray[offsetTempWriteArray+i] & 0xff;
     edition_settings.IP4[3] = (tempWriteArray[offsetTempWriteArray+i]>>8) & 0xff;
    break;

    case 18://����� �������
     edition_settings.mask = tempWriteArray[offsetTempWriteArray+i] & 0xff;
    break;

    case 19://���� ��-���������
     edition_settings.gateway[0] = tempWriteArray[offsetTempWriteArray+i] & 0xff;
     edition_settings.gateway[1] = (tempWriteArray[offsetTempWriteArray+i]>>8) & 0xff;
    break;
    case 20://���� ��-���������
     edition_settings.gateway[2] = tempWriteArray[offsetTempWriteArray+i] & 0xff;
     edition_settings.gateway[3] = (tempWriteArray[offsetTempWriteArray+i]>>8) & 0xff;
    break;
#endif
    case 23://������ 1 � 2
      edition_settings.name_of_cell[0] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[1] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 24://������ 3 � 4
      edition_settings.name_of_cell[2] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[3] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 25://������ 5 � 6
      edition_settings.name_of_cell[4] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[5] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 26://������ 7 � 8
      edition_settings.name_of_cell[6] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[7] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 27://������ 9 � 10
      edition_settings.name_of_cell[8] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[9] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 28://������ 11 � 12
      edition_settings.name_of_cell[10] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[11] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 29://������ 13 � 14
      edition_settings.name_of_cell[12] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[13] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 30://������ 15 � 16
      edition_settings.name_of_cell[14] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[15] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//���� Setting
      break;

    case 31://���
      *(label_to_time_array + 6) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 32://�����
      *(label_to_time_array + 5) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 33://����
      *(label_to_time_array + 4) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 34://���
      *(label_to_time_array + 3) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 35://������
      *(label_to_time_array + 2) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 36://�������
      *(label_to_time_array + 1) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 37://����� �������
      *(label_to_time_array + 0) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    }//switch
  }//for

  if(flag_time_array)
  {
    if (check_data_for_data_time_menu() == 1)
    {
      //��� ��������
      time_ms_save_l = 0;
                  
      struct tm orig;
      unsigned int tmp_reg = time_edit[1];
      orig.tm_sec = 10*(tmp_reg >> 4) + (tmp_reg & 0xf);

      tmp_reg = time_edit[2];
      orig.tm_min = 10*(tmp_reg >> 4) + (tmp_reg & 0xf);
      tmp_reg = time_edit[3];
      orig.tm_hour = 10*(tmp_reg >> 4) + (tmp_reg & 0xf);

      tmp_reg = time_edit[4];
      orig.tm_mday = 10*(tmp_reg >> 4) + (tmp_reg & 0xf);

      tmp_reg = time_edit[5];
      orig.tm_mon = 10*(tmp_reg >> 4) + (tmp_reg & 0xf) - 1;

      tmp_reg = time_edit[6];
      orig.tm_year = 10*(tmp_reg >> 4) + (tmp_reg & 0xf) + 100;

      orig.tm_wday = 0;
      orig.tm_yday = 0;
      orig.tm_isdst = (current_settings.dst & MASKA_FOR_BIT(N_BIT_TZ_DST)) ? p->tm_isdst : 0;
      time_dat_save_l = mktime (&orig);
      if (current_settings.dst & MASKA_FOR_BIT(N_BIT_TZ_DST))
      {
        struct tm *p_tmp = localtime(&time_dat_save_l);
        if (p->tm_isdst != p_tmp->tm_isdst) orig.tm_isdst = p_tmp->tm_isdst;
        time_dat_save_l = mktime (&orig);
      }
      save_time_dat_l = 3;
    }//if
    else return ERROR_VALID2;
  }
  return 0;
}//

int privatePKVBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int passwordImunitetRegPKVBigComponent(int adrReg)
{
  //����������� � ������ ������ ��������� 0 - ���� ��������
  if(privatePKVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  switch(adrReg)
  {
  case BEGIN_ADR_REGISTER+IMUNITET_PKV1://��������/��������� ������
    return 0;//���� ��������
  }//switch
  return MARKER_OUTPERIMETR;
}//passwordImunitetRegPKVBigComponent(int adrReg)
