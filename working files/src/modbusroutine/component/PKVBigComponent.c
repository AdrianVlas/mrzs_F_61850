#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 12000
//конечный регистр в карте памяти
#define END_ADR_REGISTER 12042

int privatePKVBigGetReg2(int adrReg);

int getPKVBigModbusRegister(int);//получить содержимое регистра
int getPKVBigModbusBit(int);//получить содержимое бита
int setPKVBigModbusRegister(int, int);//получить содержимое регистра
int setPKVBigModbusBit(int, int);//получить содержимое бита

void setPKVBigCountObject(void);//записать к-во обектов
void prePKVBigReadAction(void);//action до чтения
void prePKVBigWriteAction(void);//action до записи
int  postPKVBigWriteAction(void);//action после записи
int PKVFunc000(int inOffset, int regPKV, uint32_t **editValue);
int passwordImunitetRegPKVBigComponent(int x);

COMPONENT_OBJ *pkvbigcomponent;

int PKVFunc000(int inOffset, int regPKV, uint32_t **editValue)
{
  int diapazon = 1;

  switch(inOffset)
  {
  case 0://Время активации пароля после простоя
    if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
      (*editValue) = &edition_settings.timeout_deactivation_password_interface_USB;
    else
      (*editValue) = &edition_settings.timeout_deactivation_password_interface_RS485;
    if(regPKV<TIMEOUT_DEACTIVATION_PASSWORD_MIN || regPKV>TIMEOUT_DEACTIVATION_PASSWORD_MAX) diapazon=0;
    break;

#define IMUNITET_PKV1 1
  case IMUNITET_PKV1://Проверка/установка пароля
    if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
      (*editValue) = &edition_settings.password_interface_USB;
    else
      (*editValue) = &edition_settings.password_interface_RS485;
    break;

  case 2://Тайм-аут применения изменений
    (*editValue) = &edition_settings.timeout_idle_new_settings;
    if(regPKV<TIMEOUT_NEW_SETTINGS_MIN || regPKV>TIMEOUT_NEW_SETTINGS_MAX) diapazon=0;
    break;

  case 3://Язык пользовательского интерфейса
    (*editValue) = (uint32_t *)&edition_settings.language;
    if(regPKV<VALUE_SETTING_LANGUAGE_MIN || regPKV>VALUE_SETTING_LANGUAGE_MAX) diapazon=0;
    break;

  case 5://Скорость порта связи
    (*editValue) = (uint32_t *)&edition_settings.speed_RS485;
    if(regPKV<VALUE_SPEED_INTERFACE_MIN || regPKV>VALUE_SPEED_INTERFACE_MAX) diapazon=0;
    break;

  case 6://Количество стоп-бит
    (*editValue) = (uint32_t *)&edition_settings.number_stop_bit_RS485;
    if(regPKV<1 || regPKV>MAX_NUMBER_STOP_BITS_INTERFACE) diapazon=0;
    break;

  case 7://Паритет
    (*editValue) = (uint32_t *)&edition_settings.pare_bit_RS485;
    if(regPKV<VALUE_PARE_INTERFACE_MIN || regPKV>VALUE_PARE_INTERFACE_MAX) diapazon=0;
    break;

  case 8://Задержка приёма
    (*editValue) = &edition_settings.time_out_1_RS485;
    if(regPKV<VALUE_TIME_OUT_1_INTERFACE_MIN || regPKV>VALUE_TIME_OUT_1_INTERFACE_MAX) diapazon=0;
    break;

//    case 9://Скорость порта связи 2
//    case 10://Количество стоп-бит 2
//    case 11://Паритет 2
//    case 12://Задержка приёма 2
//      (*editValue) = &stubnull;
//      break;

  case 13://Адрес устройства в сети
    (*editValue) = &edition_settings.address;
    if(regPKV<KOEF_ADDRESS_MIN || regPKV>KOEF_ADDRESS_MAX) diapazon=0;
    break;

//    case 14://Таймаут конца фрейма
//    case 15:
//      (*editValue) = &stubnull;
//      break;

//    case 16://Символ 1 и 2
//      (*editValue) = &edition_settings.name_of_cell[0];
//      break;
//    case 17://Символ 3 и 4
//      (*editValue) = &edition_settings.name_of_cell[2];
//      break;
//    case 18://Символ 5 и 6
//      (*editValue) = &edition_settings.name_of_cell[4];
//      break;
//    case 19://Символ 7 и 8
//      (*editValue) = &edition_settings.name_of_cell[6];
//      break;
//    case 20://Символ 9 и 10
//      (*editValue) = &edition_settings.name_of_cell[8];
//      break;
//    case 21://Символ 11 и 12
//      (*editValue) = &edition_settings.name_of_cell[10];
//      break;
//    case 22://Символ 13 и 14
//      (*editValue) = &edition_settings.name_of_cell[12];
//      break;
//    case 23://Символ 15 и 16
//      (*editValue) = &edition_settings.name_of_cell[14];
//      break;
  case 24://Год
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x99) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 25://Месяц
    if(regPKV&0xff00) diapazon=0;
    if(regPKV==0) diapazon=0;
    if(regPKV>0x12) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 26://День
    if(regPKV&0xff00) diapazon=0;
    if(regPKV==0) diapazon=0;
    if(regPKV>0x31) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 27://Час
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x23) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 28://Минуты
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x59) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 29://Секунды
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x59) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;
  case 30://Сотые секунды
    if(regPKV&0xff00) diapazon=0;
    if(regPKV>0x99) diapazon=0;
    if((regPKV&0xf)>9) diapazon=0;
    break;

//    case 31://Часовой пояс
//    case 32://Переход на Зимнее/Летнее время
//    case 33://Месяц перехода на Летнее время
//    case 34://Неделя месяца перехода на Летнее время
//    case 35://День недели перехода на Летнее время
//    case 36://Час недели перехода на Летнее время
//    case 37://Месяц перехода на Зимнее время
//    case 38://Неделя месяца перехода на Зимнее время
//    case 39://День недели перехода на Зимнее время
//    case 40://Час недели перехода на Зимнее время
//    case 41://Синхронизация времени
//    case 42://Время утраты синхронизации
//      (*editValue) = &stubnull;
//      break;

  }//switch

  return diapazon;
} //PKVFunc000(int inOffset, int regPKV, uint32_t **editValue)

/**************************************/
//подготовка компонента PKV
/**************************************/
void constructorPKVBigComponent(COMPONENT_OBJ *pkvbigcomp)
{
  pkvbigcomponent = pkvbigcomp;

  pkvbigcomponent->getModbusRegister = getPKVBigModbusRegister;//получить содержимое регистра
  pkvbigcomponent->getModbusBit      = getPKVBigModbusBit;//получить содержимое бита
  pkvbigcomponent->setModbusRegister = setPKVBigModbusRegister;//получить содержимое регистра
  pkvbigcomponent->setModbusBit      = setPKVBigModbusBit;//получить содержимое бита

  pkvbigcomponent->preReadAction   = prePKVBigReadAction;//action до чтения
  pkvbigcomponent->preWriteAction  = prePKVBigWriteAction;//action до записи
  pkvbigcomponent->postWriteAction = postPKVBigWriteAction;//action после записи

  pkvbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getPKVBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privatePKVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(pkvbigcomponent->isActiveActualData)
  {
    edition_settings = current_settings_interfaces;//делаем копию
  }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  unsigned char *label_to_time_array;
  if (copying_time == 2) label_to_time_array = time_copy;
  else label_to_time_array = time;

  uint32_t *editValue=NULL;
  PKVFunc000(adrReg-BEGIN_ADR_REGISTER, 0, &editValue);

  switch(adrReg-BEGIN_ADR_REGISTER)
  {
  case 1://Проверка/установка пароля
    return MARKER_ERRORPERIMETR;
  case 6://Количество стоп-бит
    return (((unsigned short)*editValue)+1) &0xFFFF;
  case 16://Символ 1 и 2
    return (edition_settings.name_of_cell[0]&0xFF) | ((edition_settings.name_of_cell[1]<<8)&0xFF00);
  case 17://Символ 3 и 4
    return (edition_settings.name_of_cell[2]&0xFF) | ((edition_settings.name_of_cell[3]<<8)&0xFF00);
  case 18://Символ 5 и 6
    return (edition_settings.name_of_cell[4]&0xFF) | ((edition_settings.name_of_cell[5]<<8)&0xFF00);
  case 19://Символ 7 и 8
    return (edition_settings.name_of_cell[6]&0xFF) | ((edition_settings.name_of_cell[7]<<8)&0xFF00);
  case 20://Символ 9 и 10
    return (edition_settings.name_of_cell[8]&0xFF) | ((edition_settings.name_of_cell[9]<<8)&0xFF00);
  case 21://Символ 11 и 12
    return (edition_settings.name_of_cell[10]&0xFF) | ((edition_settings.name_of_cell[11]<<8)&0xFF00);
  case 22://Символ 13 и 14
    return (edition_settings.name_of_cell[12]&0xFF) | ((edition_settings.name_of_cell[13]<<8)&0xFF00);
  case 23://Символ 15 и 16
    return (edition_settings.name_of_cell[14]&0xFF) | ((edition_settings.name_of_cell[15]<<8)&0xFF00);

  case 24://Год
    return  (*(label_to_time_array + 6)) &0xFF;

  case 25://Месяц
    return (*(label_to_time_array + 5)) &0xFF;

  case 26://День
    return (*(label_to_time_array + 4)) &0xFF;

  case 27://Час
    return (*(label_to_time_array + 3)) &0xFF;

  case 28://Минуты
    return (*(label_to_time_array + 2)) &0xFF;

  case 29://Секунды
    return (*(label_to_time_array + 1)) &0xFF;

  case 30://Сотые секунды
    return (*(label_to_time_array + 0)) &0xFF;
  }//switch

  if(editValue==NULL) return 0;
  return (((unsigned short)*editValue)) &0xFFFF;
}//getDOUTBigModbusRegister(int adrReg)
int getPKVBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setPKVBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privatePKVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(pkvbigcomponent->isActiveActualData)
  {
    edition_settings = current_settings_interfaces;//делаем копию
  }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(pkvbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  uint32_t *editValue=NULL;
  if(!PKVFunc000(adrReg-BEGIN_ADR_REGISTER, dataReg, &editValue)) return MARKER_ERRORDIAPAZON;

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setPKVBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void prePKVBigReadAction(void)
{
//action до чтения
  pkvbigcomponent->isActiveActualData = 1;
}//
void prePKVBigWriteAction(void)
{
//action до записи
  pkvbigcomponent->operativMarker[0] = -1;
  pkvbigcomponent->operativMarker[1] = -1;//оперативный маркер
  pkvbigcomponent->isActiveActualData = 1;
}//
int postPKVBigWriteAction(void)
{
  extern int upravlSetting;//флаг Setting
//action после записи
  int beginAdr = pkvbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = pkvbigcomponent->operativMarker[1];
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray
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
    switch(offset)  //индекс регистра
    {
    case 0://Время активации пароля после простоя
    case 2://Тайм-аут применения изменений
    case 3://Язык пользовательского интерфейса
    case 5://Скорость порта связи
    case 7://Паритет
    case 8://Задержка приёма
    case 13://Адрес устройства в сети
      *editValue = (tempWriteArray[offsetTempWriteArray+i]);
      upravlSetting = 1;//флаг Setting
      break;

    case 1://Проверка/установка пароля
      {
      int passwordS=-1;
      if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
      {
        passwordS = password_set_USB;//Пароль установлен
      }//if
      else
      {
        passwordS = password_set_RS485;//Пароль установлен
      }
      unsigned short new_parol = tempWriteArray[offsetTempWriteArray+i];
      if(passwordS==1) //пароль не снят
      {
        //режим check parol
        if((*editValue) == new_parol)
        {
          //обнулить флажок пароля
          if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
          {
            password_set_USB=0;//Пароль установлен
          }//if
          else
          {
            password_set_RS485=0;//Пароль установлен
          }
        }//if
        else return ERROR_VALID2;//неправильный пароль
        break;
      }//if(passwordS==1) //пароль не снят
      else
      {//пароль снят
        //режим check parol
        if((*editValue) == new_parol)
        {
          if(new_parol==0) break;//повторная запись нулевого пароля не активирует флаг
          //активир флажок пароля
          if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
          {
            password_set_USB=1;//Пароль установлен
          }//if
          else
          {
            password_set_RS485=1;//Пароль установлен
          }
        }//if
      }//else
      //записать новый пароль
      *editValue = (unsigned short)(tempWriteArray[offsetTempWriteArray+i]);
      upravlSetting = 1;//флаг Setting
      } break;

    case 6://Количество стоп-бит
      *editValue = (tempWriteArray[offsetTempWriteArray+i])-1;
      upravlSetting = 1;//флаг Setting
      break;

    case 16://Символ 1 и 2
      edition_settings.name_of_cell[0] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[1] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//флаг Setting
      break;
    case 17://Символ 3 и 4
      edition_settings.name_of_cell[2] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[3] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//флаг Setting
      break;
    case 18://Символ 5 и 6
      edition_settings.name_of_cell[4] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[5] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//флаг Setting
      break;
    case 19://Символ 7 и 8
      edition_settings.name_of_cell[6] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[7] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//флаг Setting
      break;
    case 20://Символ 9 и 10
      edition_settings.name_of_cell[8] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[9] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//флаг Setting
      break;
    case 21://Символ 11 и 12
      edition_settings.name_of_cell[10] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[11] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//флаг Setting
      break;
    case 22://Символ 13 и 14
      edition_settings.name_of_cell[12] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[13] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//флаг Setting
      break;
    case 23://Символ 15 и 16
      edition_settings.name_of_cell[14] = (tempWriteArray[offsetTempWriteArray+i]);
      edition_settings.name_of_cell[15] = (tempWriteArray[offsetTempWriteArray+i])>>8;
      upravlSetting = 1;//флаг Setting
      break;

    case 24://Год
      *(label_to_time_array + 6) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 25://Месяц
      *(label_to_time_array + 5) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 26://День
      *(label_to_time_array + 4) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 27://Час
      *(label_to_time_array + 3) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 28://Минуты
      *(label_to_time_array + 2) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 29://Секунды
      *(label_to_time_array + 1) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    case 30://Сотые секунды
      *(label_to_time_array + 0) = (tempWriteArray[offsetTempWriteArray+i]);
      flag_time_array = 1;
      break;
    }//switch
  }//for

  if(flag_time_array)
  {
    if (check_data_for_data_time_menu() == 1)
    {
      //Дані достовірні
      _SET_BIT(control_i2c_taskes, TASK_START_WRITE_RTC_BIT);
      _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
    }//if
    else return ERROR_VALID2;
  }
  return 0;
}//

int privatePKVBigGetReg2(int adrReg)
{
  //проверить внешний периметр
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int passwordImunitetRegPKVBigComponent(int adrReg)
{
  //имунитетные к паролю адреса регистров 0 - есть имунитет
  if(privatePKVBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  switch(adrReg)
  {
  case BEGIN_ADR_REGISTER+IMUNITET_PKV1://Проверка/установка пароля
    return 0;//есть имунитет
  }//switch
  return MARKER_OUTPERIMETR;
}//passwordImunitetRegPKVBigComponent(int adrReg)
