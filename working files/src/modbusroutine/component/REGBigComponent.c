#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 13000
//конечный регистр в карте памяти
#define END_ADR_REGISTER 13075
extern int pointInterface;//метка интерфейса 0-USB 1-RS485

int privateREGBigGetReg2(int adrReg);

int getREGBigModbusRegister(int);//получить содержимое регистра
int getREGBigModbusBit(int);//получить содержимое бита
int setREGBigModbusRegister(int, int);//получить содержимое регистра
int setREGBigModbusBit(int, int);//получить содержимое бита

void setREGBigCountObject(void);//записать к-во обектов
void preREGBigReadAction(void);//action до чтения
void preREGBigWriteAction(void);//action до записи
int  postREGBigWriteAction(void);//action после записи

COMPONENT_OBJ *regbigcomponent;
//__INFO_REJESTRATOR info_rejestrator_ar_ptr;
/**************************************/
//подготовка компонента REGistrator
/**************************************/
void constructorREGBigComponent(COMPONENT_OBJ *regbigcomp)
{
  regbigcomponent = regbigcomp;

  regbigcomponent->getModbusRegister = getREGBigModbusRegister;//получить содержимое регистра
  regbigcomponent->getModbusBit      = getREGBigModbusBit;//получить содержимое бита
  regbigcomponent->setModbusRegister = setREGBigModbusRegister;//получить содержимое регистра
  regbigcomponent->setModbusBit      = setREGBigModbusBit;//получить содержимое бита

  regbigcomponent->preReadAction   = preREGBigReadAction;//action до чтения
  regbigcomponent->preWriteAction  = preREGBigWriteAction;//action до записи
  regbigcomponent->postWriteAction = postREGBigWriteAction;//action после записи

  regbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getREGBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;
//Ранжирование источников запуска аналогового регистратора
  if(offset<32) return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_analog_registrator[0], 32, offset );
//Ранжирование источников запуска дискретного регистратора
  if(offset>=36&&offset<70) return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_digital_registrator[0], 32, offset );

  switch(offset)
  {
  case 32://Время записи аналогового регистратора (доаварийный массив)
    return current_settings_interfaces.prefault_number_periods*20; //В таблицю настройок записуємо не мілісекунди, а кількість періодів
  case 33://Время записи аналогового регистратора (послеаварый массив)
    return current_settings_interfaces.postfault_number_periods*20; //В таблицю настройок записуємо не мілісекунди, а кількість періодів
  case 34://Количество аналоговых регистраторов
    return info_rejestrator_ar.number_records;
  case 35://Текущий аналоговый регистратор
    if(pointInterface==0)//метка интерфейса 0-USB 1-RS485
      return number_record_of_ar_for_USB;
    else
      return number_record_of_ar_for_RS485;
  case 70://Количество дискретных регистраторов
    return info_rejestrator_dr.number_records;
  case 71://Текущий дискретный регистратор
    if(pointInterface==0)//метка интерфейса 0-USB 1-RS485
      return number_record_of_dr_for_USB;
    else
      return number_record_of_dr_for_RS485;
  case 74://Очистить аналоговый регистратор
    return MARKER_ERRORPERIMETR;
  case 75://Очистить дискретный регистратор
    return MARKER_ERRORPERIMETR;
  }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getREGBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setREGBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(regbigcomponent->isActiveActualData)
  {
    edition_settings = current_settings_interfaces;//делаем копию
  }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(regbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  int offset = adrReg-BEGIN_ADR_REGISTER;
//Ранжирование источников запуска аналогового регистратора
  if(offset<32 || (offset>=36&&offset<70)) return validN_BIGACMD(dataReg);

  switch(offset)
  {
  case 32://Время записи аналогового регистратора (доаварийный массив)
    if(dataReg<TIMEOUT_PREFAULT_MIN || dataReg>TIMEOUT_PREFAULT_MAX) return MARKER_ERRORDIAPAZON;
    break;
  case 33://Время записи аналогового регистратора (послеаварый массив)
    if(dataReg<TIMEOUT_POSTFAULT_MIN || dataReg>TIMEOUT_POSTFAULT_MAX) return MARKER_ERRORDIAPAZON;
    break;
  case 34://Количество аналоговых регистраторов
    return MARKER_ERRORDIAPAZON;
  case 35://Текущий аналоговый регистратор
    if(dataReg<0 ||(unsigned int)dataReg>info_rejestrator_ar.number_records) return MARKER_ERRORDIAPAZON;
    break;
  case 70://Количество дискретных регистраторов
    return MARKER_ERRORDIAPAZON;
  case 71://Текущий дискретный регистратор
    if(dataReg<0 ||(unsigned int)dataReg>info_rejestrator_dr.number_records) return MARKER_ERRORDIAPAZON;
    break;
  case 74://Очистить аналоговый регистратор
    if(dataReg!=CMD_WORD_CLEAR_AR) return MARKER_ERRORDIAPAZON;
    break;
  case 75://Очистить дискретный регистратор
    if(dataReg!=CMD_WORD_CLEAR_DR) return MARKER_ERRORDIAPAZON;
    break;
  }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setREGBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preREGBigReadAction(void) {
//action до чтения
  regbigcomponent->isActiveActualData = 1;
}//
void preREGBigWriteAction(void) {
//action до записи
  regbigcomponent->operativMarker[0] = -1;
  regbigcomponent->operativMarker[1] = -1;//оперативный маркер
  regbigcomponent->isActiveActualData = 1;
}//
int postREGBigWriteAction(void) {
  extern int upravlSchematic;//флаг Rang
  extern int upravlSetting;//флаг Setting
//action после записи
  int beginAdr = regbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = regbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray

  int flag1=1;
  int flag2=1;
  int countReg=countAdr;
  for(int i=0; i<countAdr; i++)
  {
    int offset = beginAdr-BEGIN_ADR_REGISTER+i;
    if(offset<32&&flag1) {
      writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_analog_registrator[0], 32, beginAdr,
                                   countReg, BEGIN_ADR_REGISTER);
      flag1=0;
      upravlSchematic = 1;//флаг Rang
    }//if

    if(offset>=36&&offset<70&&flag2) {
      writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_digital_registrator[0], 32, offset-36,
                                   countReg, BEGIN_ADR_REGISTER+36);
      flag2=0;
      upravlSchematic = 1;//флаг Rang
    }//if
    switch(offset)
    {
    case 32://Время записи аналогового регистратора (доаварийный массив)
      edition_settings.prefault_number_periods = tempWriteArray[offsetTempWriteArray+i]/20; //В таблицю настройок записуємо не мілісекунди, а кількість періодів
      upravlSetting = 1;//флаг Setting
      break;
    case 33://Время записи аналогового регистратора (послеаварый массив)
      edition_settings.postfault_number_periods = tempWriteArray[offsetTempWriteArray+i]/20; //В таблицю настройок записуємо не мілісекунди, а кількість періодів
      upravlSetting = 1;//флаг Setting
      break;
    case 35://Текущий аналоговый регистратор
     if(pointInterface==0)//метка интерфейса 0-USB 1-RS485
         number_record_of_ar_for_USB = tempWriteArray[offsetTempWriteArray+i];
     else
         number_record_of_ar_for_RS485 = tempWriteArray[offsetTempWriteArray+i];
      break;
    case 71://Текущий дискретный регистратор
      if(pointInterface==0)//метка интерфейса 0-USB 1-RS485
      {
        number_record_of_dr_for_USB = tempWriteArray[offsetTempWriteArray+i];
        //Подаємо команду читання дискретного реєстратора для інтерфейсу USB

        //Виставляємо першу частину запису
        part_reading_dr_from_dataflash_for_USB = 0;
        //Подаємо команду зчитати дані у бувер пам'яті
        control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB;
      }//if
      else
      {
        number_record_of_dr_for_RS485 = tempWriteArray[offsetTempWriteArray+i];
        //Подаємо команду читання дискретного реєстратора для інтерфейсу RS-485

        //Виставляємо першу частину запису
        part_reading_dr_from_dataflash_for_RS485 = 0;
        //Подаємо команду зчитати дані у бувер пам'яті
        control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485;
      }
      break;
    case 74://Очистить аналоговый регистратор
//ОСОБАЯ ПРОВЕРКА
      if (
        (current_ekran.current_level == EKRAN_DATA_LADEL_AR)
        ||
        (state_ar_record             != STATE_AR_NO_RECORD )
        ||
        (
          (control_tasks_dataflash & (
             TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR |
             TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR      |
             TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB                         |
             TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485                       |
             TASK_MAMORY_READ_DATAFLASH_FOR_AR_MENU
           )
          ) != 0
        )
        ||
        ((clean_rejestrators & CLEAN_AR) != 0)
      ) return ERROR_VALID2;//ошибка валидации

      clean_rejestrators |= CLEAN_AR;
      break;
    case 75://Очистить дискретный регистратор
//ОСОБАЯ ПРОВЕРКА
      if (
        (current_ekran.current_level == EKRAN_TITLES_DIGITAL_REGISTRATOR)
        ||
        (current_ekran.current_level == EKRAN_DATA_LADEL_DR             )
        ||
        (current_ekran.current_level == EKRAN_CHANGES_SIGNALS_DR        )
        ||
        (current_ekran.current_level == EKRAN_TITLE_MAX_VALUES          )
        ||
        (current_ekran.current_level == EKRAN_MAX_VALUES                )
        ||
        (
          (control_tasks_dataflash & (
             TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR |
             TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB                    |
             TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485                  |
             TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU
           )
          ) != 0
        )
        ||
        ((clean_rejestrators & CLEAN_DR) != 0)
      ) return ERROR_VALID2;//ошибка валидации

      //Помічаємо, що треба очистити дискретного реєстратора
      clean_rejestrators |= CLEAN_DR;
      break;
    }//switch
  }//for

  return 0;
}//

int privateREGBigGetReg2(int adrReg)
{
  //проверить внешний периметр
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
