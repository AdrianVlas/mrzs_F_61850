#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 12000
//�������� ������� � ����� ������
#define END_ADR_REGISTER 12055

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

  case 38://������� ����
    (*editValue) = (uint32_t*)&edition_settings.time_zone;
    if(regPKV>64000 && regPKV<65536+(TIME_ZONE_MIN*60)) diapazon=0;
    if(regPKV<64000 && regPKV>(TIME_ZONE_MAX*60)) diapazon=0;
    break;
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
  if (save_time_dat_l == 3) 
  {
    time_dat_tmp = time_dat_save_l;
    time_ms_tmp = time_ms_save_l;
  }
  else
  { 
    copying_time_dat = 1;
    time_dat_tmp = time_dat_copy;
    time_ms_tmp = time_ms_copy;
    copying_time_dat = 0;
  }
  //struct tm *p;
  //struct tm *p = localtime(&time_dat_tmp);
//time_dat_tmp = 1581934622;

  uint32_t *editValue=NULL;
  PKVFunc000(adrReg-BEGIN_ADR_REGISTER, 0, &editValue);
  int offset = adrReg-BEGIN_ADR_REGISTER;

  switch(offset)
  {
  case 1://��������/��������� ������
    return MARKER_ERRORPERIMETR;
  case 6://���������� ����-���
    return (((unsigned short)*editValue)+1) &0xFFFF;

#if (MODYFIKACIA_VERSII_PZ >= 10)
    case 16://IP ����� ����������
     return edition_settings.IP4[0]&0xff | (((edition_settings.IP4[1]&0xff)<<8)&0xFF00);
    case 17://IP ����� ����������
     return edition_settings.IP4[2]&0xff | (((edition_settings.IP4[3]&0xff)<<8)&0xFF00);
    case 18://����� �������
     return edition_settings.mask&0xff;
    case 19://���� ��-���������
     return edition_settings.gateway[0]&0xff | (((edition_settings.gateway[1]&0xff)<<8)&0xFF00);
    case 20://���� ��-���������
     return edition_settings.gateway[2]&0xff | (((edition_settings.gateway[3]&0xff)<<8)&0xFF00);
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

  case 31:
  case 32:
  case 33:
  case 34:
    {
    uint16_t* tmp[4];
    tmp[0] = &((uint16_t*)&time_dat_tmp)[0];
    tmp[1] = &((uint16_t*)&time_dat_tmp)[1];
    tmp[2] = &((uint16_t*)&time_dat_tmp)[2];
    tmp[3] = &((uint16_t*)&time_dat_tmp)[3];
    return (*(tmp[offset-31]));
    }
  case 35:
  case 36:
    {
    uint16_t* tmp[2];
    tmp[0] = &((uint16_t*)&time_ms_tmp)[0];
    tmp[1] = &((uint16_t*)&time_ms_tmp)[1];
    return *(tmp[offset-35]);
    }

  case 38://������� ����
    return ((*editValue) &0xFFFF)*60;

  case 39://������� �� ������/������ �����
    return (edition_settings.dst & MASKA_FOR_BIT(N_BIT_TZ_DST)) ? 1 : 0;

  case 40://����� �������� �� ������ �����
    return (edition_settings.dst_on_rule>>POS_MM)&((int)(pow(2,SHIFT_MM)-1));
  case 41://������ ������ �������� �� ������ �����
    return (edition_settings.dst_on_rule>>POS_DOW)&((int)(pow(2,SHIFT_DOW)-1));
  case 42://���� ������ �������� �� ������ �����
    return (edition_settings.dst_on_rule>>POS_WR)&((int)(pow(2,SHIFT_WR)-1));
  case 43://��� ������ �������� �� ������ �����
    return (edition_settings.dst_on_rule>>POS_HH)&((int)(pow(2,SHIFT_HH)-1));
  case 44://����� �������� �� ������ �����
    return (edition_settings.dst_off_rule>>POS_MM)&((int)(pow(2,SHIFT_MM)-1));
  case 45://������ ������ �������� �� ������ �����
    return (edition_settings.dst_off_rule>>POS_DOW)&((int)(pow(2,SHIFT_DOW)-1));
  case 46://���� ������ �������� �� ������ �����
    return (edition_settings.dst_off_rule>>POS_WR)&((int)(pow(2,SHIFT_WR)-1));
  case 47://��� ������ �������� �� ������ �����
    return (edition_settings.dst_off_rule>>POS_HH)&((int)(pow(2,SHIFT_HH)-1));

  case 51://IP ����� ������� NTP 1
     return edition_settings.IP_time_server[0]&0xff | (((edition_settings.IP_time_server[1]&0xff)<<8)&0xFF00);
  case 52://IP ����� ������� NTP 1
     return edition_settings.IP_time_server[2]&0xff | (((edition_settings.IP_time_server[3]&0xff)<<8)&0xFF00);

  case 53://�������� ������ NTP �������
    return edition_settings.period_sync&0xFFFF;
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
  if(endAdr<0) countAdr = 1;//�-�� ���������
  int flag_time_array = 0;
  int flag_ms_array = 0;

//  struct tm *p;
  //����������� ��������� ������ ������
  switch(beginAdr)
  {
   case 12032: case 12033: case 12034: case 12036: return ERROR_VALID2;
  }//switch
  //����������� �������� ������ ������
  switch(endAdr)
  {
   case 12031: case 12032: case 12033: case 12035: return ERROR_VALID2;
  }//switch
  unsigned short time_dat_frac[4];//������� ����� (��� 64-���)
  unsigned short time_ms_frac[2];//������� �����, ������������


  for(int i=0; i<countAdr; i++)
  {
    int offset = i+beginAdr-BEGIN_ADR_REGISTER;
    uint32_t *editValue=NULL;
    PKVFunc000(offset, 0, &editValue);
    unsigned short offsetWriteRegister = tempWriteArray[offsetTempWriteArray+i];//����� �������� ��������
    switch(offset)  //������ ��������
    {
    case 0://����� ��������� ������ ����� �������
    case 2://����-��� ���������� ���������
    case 3://���� ����������������� ����������
    case 5://�������� ����� �����
    case 7://�������
    case 8://�������� �����
    case 13://����� ���������� � ����
      *editValue = offsetWriteRegister;
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
      unsigned short new_parol = offsetWriteRegister;

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
      *editValue = offsetWriteRegister;
      upravlSetting = 1;//���� Setting
      } break;

    case 6://���������� ����-���
      *editValue = offsetWriteRegister-1;
      upravlSetting = 1;//���� Setting
      break;

#if (MODYFIKACIA_VERSII_PZ >= 10)
    case 16://IP ����� ����������
     edition_settings.IP4[0] = offsetWriteRegister & 0xff;
     edition_settings.IP4[1] = (offsetWriteRegister>>8) & 0xff;
     upravlSetting = 1;//���� Setting
    break;
    case 17://IP ����� ����������
     edition_settings.IP4[2] = offsetWriteRegister & 0xff;
     edition_settings.IP4[3] = (offsetWriteRegister>>8) & 0xff;
     upravlSetting = 1;//���� Setting
    break;

    case 18://����� �������
     edition_settings.mask = offsetWriteRegister & 0xff;
     upravlSetting = 1;//���� Setting
    break;

    case 19://���� ��-���������
     edition_settings.gateway[0] = offsetWriteRegister & 0xff;
     edition_settings.gateway[1] = (offsetWriteRegister>>8) & 0xff;
     upravlSetting = 1;//���� Setting
    break;
    case 20://���� ��-���������
     edition_settings.gateway[2] = offsetWriteRegister & 0xff;
     edition_settings.gateway[3] = (offsetWriteRegister>>8) & 0xff;
     upravlSetting = 1;//���� Setting
    break;
#endif
    case 23://������ 1 � 2
      edition_settings.name_of_cell[0] = offsetWriteRegister;
      edition_settings.name_of_cell[1] = offsetWriteRegister>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 24://������ 3 � 4
      edition_settings.name_of_cell[2] = offsetWriteRegister;
      edition_settings.name_of_cell[3] = offsetWriteRegister>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 25://������ 5 � 6
      edition_settings.name_of_cell[4] = offsetWriteRegister;
      edition_settings.name_of_cell[5] = offsetWriteRegister>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 26://������ 7 � 8
      edition_settings.name_of_cell[6] = offsetWriteRegister;
      edition_settings.name_of_cell[7] = offsetWriteRegister>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 27://������ 9 � 10
      edition_settings.name_of_cell[8] = offsetWriteRegister;
      edition_settings.name_of_cell[9] = offsetWriteRegister>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 28://������ 11 � 12
      edition_settings.name_of_cell[10] = offsetWriteRegister;
      edition_settings.name_of_cell[11] = offsetWriteRegister>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 29://������ 13 � 14
      edition_settings.name_of_cell[12] = offsetWriteRegister;
      edition_settings.name_of_cell[13] = offsetWriteRegister>>8;
      upravlSetting = 1;//���� Setting
      break;
    case 30://������ 15 � 16
      edition_settings.name_of_cell[14] = offsetWriteRegister;
      edition_settings.name_of_cell[15] = offsetWriteRegister>>8;
      upravlSetting = 1;//���� Setting
      break;

    case 31://����� (��� 64-���)
    case 32://����� (��� 64-���)
    case 33://����� (��� 64-���)
    case 34://����� (��� 64-���)
      if(countAdr == 1) return ERROR_VALID2;//�-�� ���������
      time_dat_frac[offset-31] = offsetWriteRegister;//������� ����� (��� 64-���)
      flag_time_array = 1;
      break;
    case 35://�����, ������������
    case 36://�����, ������������
      if(countAdr == 1) return ERROR_VALID2;//�-�� ���������
      time_ms_frac[offset-35] = offsetWriteRegister;//������� �����, ������������
      flag_ms_array = 1;
      break;

    case 38://������� ����
      {
      int value = (short)offsetWriteRegister;
      if((value%60) >0) return ERROR_VALID2;//�������� ��������� 60
      *editValue = value/60;
      upravlSetting = 1;//���� Setting
      }
      break;

   case 39://������� �� ������/������ �����
     if(offsetWriteRegister>1) return ERROR_VALID2;
     edition_settings.dst &= (uint32_t)(~(1<<N_BIT_TZ_DST));
     edition_settings.dst |= (offsetWriteRegister<<N_BIT_TZ_DST);
     upravlSetting = 1;//���� Setting
     break;
 
   case 40://����� �������� �� ������ �����
     if(offsetWriteRegister>DST_RULE_MM_MAX) return ERROR_VALID2;
     if(offsetWriteRegister<DST_RULE_MM_MIN) return ERROR_VALID2;
     edition_settings.dst_on_rule &= (uint32_t)(~(((1 << SHIFT_MM) - 1)<<POS_MM));
     edition_settings.dst_on_rule |= (offsetWriteRegister<<POS_MM);
     upravlSetting = 1;//���� Setting
     break;
   case 41://������ ������ �������� �� ������ �����
     if(offsetWriteRegister>DST_RULE_DOW_MAX) return ERROR_VALID2;
//     if(offsetWriteRegister<DST_RULE_DOW_MIN) return ERROR_VALID2;
     edition_settings.dst_on_rule &= (uint32_t)(~(((1 << SHIFT_DOW) - 1)<<POS_DOW));
     edition_settings.dst_on_rule |= (offsetWriteRegister<<POS_DOW);
     upravlSetting = 1;//���� Setting
     break;
   case 42://����� ��� ������ ��������  �� ������ �����
     if(offsetWriteRegister>DST_RULE_DR_MAX) return ERROR_VALID2;
     if(offsetWriteRegister<DST_RULE_DR_MIN) return ERROR_VALID2;
     edition_settings.dst_on_rule &= (uint32_t)(~(((1 << SHIFT_WR) - 1)<<POS_WR));
     edition_settings.dst_on_rule |= (offsetWriteRegister<<POS_WR);
     upravlSetting = 1;//���� Setting
     break;
   case 43://��� ������ �������� �� ������ �����
     if(offsetWriteRegister>DST_RULE_HH_MAX) return ERROR_VALID2;
     //if(offsetWriteRegister<DST_RULE_HH_MIN) return ERROR_VALID2;
     edition_settings.dst_on_rule &= (uint32_t)(~(((1 << SHIFT_HH) - 1)<<POS_HH));
     edition_settings.dst_on_rule |= (offsetWriteRegister<<POS_HH);
     upravlSetting = 1;//���� Setting
     break;
   case 44://����� �������� �� ������ �����
     if(offsetWriteRegister>DST_RULE_MM_MAX) return ERROR_VALID2;
     if(offsetWriteRegister<DST_RULE_MM_MIN) return ERROR_VALID2;
     edition_settings.dst_off_rule &= (uint32_t)(~(((1 << SHIFT_MM) - 1)<<POS_MM));
     edition_settings.dst_off_rule |= (offsetWriteRegister<<POS_MM);
     upravlSetting = 1;//���� Setting
     break;
   case 45://������ ������ �������� �� ������ �����
     if(offsetWriteRegister>DST_RULE_DOW_MAX) return ERROR_VALID2;
//     if(offsetWriteRegister<DST_RULE_DOW_MIN) return ERROR_VALID2;
     edition_settings.dst_off_rule &= (uint32_t)(~(((1 << SHIFT_DOW) - 1)<<POS_DOW));
     edition_settings.dst_off_rule |= (offsetWriteRegister<<POS_DOW);
     upravlSetting = 1;//���� Setting
     break;
   case 46://����� ��� ������ �������� �� ����������� �����
     if(offsetWriteRegister>DST_RULE_DR_MAX) return ERROR_VALID2;
     if(offsetWriteRegister<DST_RULE_DR_MIN) return ERROR_VALID2;
     edition_settings.dst_off_rule &= (uint32_t)(~(((1 << SHIFT_WR) - 1)<<POS_WR));
     edition_settings.dst_off_rule |= (offsetWriteRegister<<POS_WR);
     upravlSetting = 1;//���� Setting
     break;
   case 47://��� ������ �������� �� ������ �����
     if(offsetWriteRegister>DST_RULE_HH_MAX) return ERROR_VALID2;
     //if(offsetWriteRegister<DST_RULE_HH_MIN) return ERROR_VALID2;
     edition_settings.dst_off_rule &= (uint32_t)(~(((1 << SHIFT_HH) - 1)<<POS_HH));
     edition_settings.dst_off_rule |= (offsetWriteRegister<<POS_HH);
     upravlSetting = 1;//���� Setting
     break;
 
   case 51://IP ����� ������� NTP 1
     edition_settings.IP_time_server[0] = offsetWriteRegister & 0xff;
     edition_settings.IP_time_server[1] = (offsetWriteRegister>>8) & 0xff;
     upravlSetting = 1;//���� Setting
     break;
   case 52://IP ����� ������� NTP 1
     edition_settings.IP_time_server[2] = offsetWriteRegister & 0xff;
     edition_settings.IP_time_server[3] = (offsetWriteRegister>>8) & 0xff;
     upravlSetting = 1;//���� Setting
     break;
 
   case 53://�������� ������ NTP �������
     if(offsetWriteRegister==0) return ERROR_VALID2;
     edition_settings.period_sync = offsetWriteRegister;
     upravlSetting = 1;//���� Setting
     break;

    }//switch
  }//for

  //������ ����� �������� �������
  copying_time_dat = 1;
  time_t time_dat_tmp = time_dat_copy;//time UNIX
  uint32_t time_ms_tmp = time_ms_copy;//milisec
  copying_time_dat = 0;
   uint16_t* tmp1 = &((uint16_t*)&time_dat_tmp)[0];
   uint16_t* tmp2 = &((uint16_t*)&time_dat_tmp)[1];
   uint16_t* tmp3 = &((uint16_t*)&time_dat_tmp)[2];
   uint16_t* tmp4 = &((uint16_t*)&time_dat_tmp)[3];
   uint16_t* tmp5 = &((uint16_t*)&time_ms_tmp)[0];
   uint16_t* tmp6 = &((uint16_t*)&time_ms_tmp)[1];
  if(flag_time_array)
  {
  (*tmp1) = time_dat_frac[0];
  (*tmp2) = time_dat_frac[1];
  (*tmp3) = time_dat_frac[2];
  (*tmp4) = time_dat_frac[3];
  }//if
  if(flag_ms_array)
  {
  (*tmp5) = time_ms_frac[0];
  (*tmp6) = time_ms_frac[1];
  if(time_ms_tmp>1000) return ERROR_VALID2;
  }//if

  if(flag_time_array || flag_ms_array)
  {
    time_dat_save_l = time_dat_tmp;
    time_ms_save_l = time_ms_tmp;
    save_time_dat_l = 3;
  }//if(flag_time_array || flag_ms_array)
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
