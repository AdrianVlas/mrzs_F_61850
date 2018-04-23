#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 13000
//конечный регистр в карте памяти
#define END_ADR_REGISTER 13075

#define REGISTERS_REG 32

int privateREGBigGetReg2(int adrReg);

int getREGBigModbusRegister(int);//получить содержимое регистра
int getREGBigModbusBit(int);//получить содержимое бита
int setREGBigModbusRegister(int, int);//получить содержимое регистра
int setREGBigModbusBit(int, int);//получить содержимое бита

void setREGBigCountObject(void);//записать к-во обектов
void preREGBigReadAction(void);//action до чтения
void preREGBigWriteAction(void);//action до записи
int  postREGBigWriteAction(void);//action после записи
int passwordImunitetRegREGBigComponent(int adrReg);

COMPONENT_OBJ *regbigcomponent;

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
  if(offset<32) return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_analog_registrator[0],
                                                   REGISTERS_REG, offset );
//Ранжирование источников запуска дискретного регистратора
  if(offset>=36&&offset<70)
      return getRangN_BIGModbusRegister(&current_settings_interfaces.ranguvannja_digital_registrator[0],
                                                               REGISTERS_REG, offset-36 );

  switch(offset)
    {
    case 32://Время записи аналогового регистратора (доаварийный массив)
      return (current_settings_interfaces.prefault_number_periods) &0xFFFF; //В таблицю настройок записуємо не мілісекунди, а кількість періодів
    case 33://Время записи аналогового регистратора (послеаварый массив)
      return (current_settings_interfaces.postfault_number_periods) &0xFFFF; //В таблицю настройок записуємо не мілісекунди, а кількість періодів
    case 34://Количество аналоговых регистраторов
      return (info_rejestrator_ar.number_records) &0xFFFF;
#define IMUNITET_REG35 35
    case IMUNITET_REG35://Текущий аналоговый регистратор
      if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
        return (number_record_of_ar_for_USB) &0xFFFF;
      else
        return (number_record_of_ar_for_RS485) &0xFFFF;
    case 70://Количество дискретных регистраторов
      return (info_rejestrator_dr.number_records) &0xFFFF;
#define IMUNITET_REG71 71
    case IMUNITET_REG71://Текущий дискретный регистратор
      if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
        return (number_record_of_dr_for_USB) &0xFFFF;
      else
        return (number_record_of_dr_for_RS485) &0xFFFF;
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
      if(dataReg<TIMEOUT_PREFAULT_MIN/20 || dataReg>TIMEOUT_PREFAULT_MAX/20) return MARKER_ERRORDIAPAZON;
      break;
    case 33://Время записи аналогового регистратора (послеаварый массив)
      if(dataReg<TIMEOUT_POSTFAULT_MIN/20 || dataReg>TIMEOUT_POSTFAULT_MAX/20) return MARKER_ERRORDIAPAZON;
      break;
    case 34://Количество аналоговых регистраторов
      return MARKER_ERRORDIAPAZON;
    case 35://Текущий аналоговый регистратор
      if (
        ((clean_rejestrators & CLEAN_AR) != 0) ||
        (
          ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB  ) != 0)) ||
          ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485) != 0))
        )
      ) return MARKER_ERRORDIAPAZON;
      if(! ((unsigned int)dataReg < info_rejestrator_ar.number_records) &&
          ((unsigned int)dataReg < max_number_records_ar             )) return MARKER_ERRORDIAPAZON;

      break;
    case 70://Количество дискретных регистраторов
      return MARKER_ERRORDIAPAZON;
    case 71://Текущий дискретный регистратор
      if (
        ((clean_rejestrators & CLEAN_DR) != 0) ||
        (
          ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB  ) != 0)) ||
          ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485) != 0))
        )
      ) return MARKER_ERRORDIAPAZON;
      if(! ((unsigned int)dataReg < info_rejestrator_dr.number_records) &&
          (dataReg < MAX_NUMBER_RECORDS_INTO_DR             )) return MARKER_ERRORDIAPAZON;
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

void preREGBigReadAction(void)
{
//action до чтения
  regbigcomponent->isActiveActualData = 1;
}//
void preREGBigWriteAction(void)
{
//action до записи
  regbigcomponent->operativMarker[0] = -1;
  regbigcomponent->operativMarker[1] = -1;//оперативный маркер
  regbigcomponent->isActiveActualData = 1;
}//
int postREGBigWriteAction(void)
{
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
      if(offset<32&&flag1)
        {
          writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_analog_registrator[0], REGISTERS_REG, beginAdr,
                                       countReg, BEGIN_ADR_REGISTER);
          flag1=0;
          upravlSchematic = 1;//флаг Rang
        }//if

      if(offset>=36&&offset<70&&flag2)
        {
          writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_digital_registrator[0], REGISTERS_REG, beginAdr,
                                       countReg, BEGIN_ADR_REGISTER+36);
          flag2=0;
          upravlSchematic = 1;//флаг Rang
        }//if
      switch(offset)
        {
        case 32://Время записи аналогового регистратора (доаварийный массив)
          edition_settings.prefault_number_periods = tempWriteArray[offsetTempWriteArray+i];// /20; //В таблицю настройок записуємо не мілісекунди, а кількість періодів
          upravlSetting = 1;//флаг Setting
          break;
        case 33://Время записи аналогового регистратора (послеаварый массив)
          edition_settings.postfault_number_periods = tempWriteArray[offsetTempWriteArray+i];// /20; //В таблицю настройок записуємо не мілісекунди, а кількість періодів
          upravlSetting = 1;//флаг Setting
          break;
        case 35://Текущий аналоговый регистратор
        {
          unsigned int *point_to_number_record_of_ar;
          int *point_to_first_number_time_sample, *point_to_last_number_time_sample;

          if (pointInterface == USB_RECUEST)
            {
              point_to_number_record_of_ar = &number_record_of_ar_for_USB;
              point_to_first_number_time_sample = &first_number_time_sample_for_USB;
              point_to_last_number_time_sample = &last_number_time_sample_for_USB;
            }
          else
            {
              point_to_number_record_of_ar = &number_record_of_ar_for_RS485;
              point_to_first_number_time_sample = &first_number_time_sample_for_RS485;
              point_to_last_number_time_sample = &last_number_time_sample_for_RS485;
            }

          //Встановлюємо номер запису аналогового реєстратора для читання
          *point_to_number_record_of_ar = tempWriteArray[offsetTempWriteArray+i];
          //Подаємо команду читання аналогового реєстратора для  інтерфейсу

          //Виставляємо читання заголовку запису даного запису і дальше, скільки можливо, часових зрізів
          *point_to_first_number_time_sample = -1;
          int last_number_time_sample_tmp = (SIZE_PAGE_DATAFLASH_2 - sizeof(__HEADER_AR))/((NUMBER_ANALOG_CANALES + number_word_digital_part_ar)*sizeof(short int));
          int max_number_time_sample = (current_settings.prefault_number_periods + current_settings.postfault_number_periods) << VAGA_NUMBER_POINT_AR;
          if (last_number_time_sample_tmp <= max_number_time_sample)
            {
              *point_to_last_number_time_sample = last_number_time_sample_tmp - 1;//номер останнього часового зрізу ВКЛЮЧНО
            }
          else
            {
              *point_to_last_number_time_sample = max_number_time_sample - 1;
            }

          //Подаємо команду зчитати дані у бувер пам'яті
          if (pointInterface == USB_RECUEST)
            control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB;
          else
            control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485;

        }
        break;//case 35
        case 71://Текущий дискретный регистратор

          if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
            {
//int test = tempWriteArray[offsetTempWriteArray+i];
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

int passwordImunitetRegREGBigComponent(int adrReg)
{
  //имунитетные к паролю адреса регистров 0 - есть имунитет
  if(privateREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  switch(adrReg)
  {
  case BEGIN_ADR_REGISTER+IMUNITET_REG35://Текущий аналоговый регистратор
  case BEGIN_ADR_REGISTER+IMUNITET_REG71://Текущий дискретный регистратор
    return 0;//есть имунитет
  }//switch
  return MARKER_OUTPERIMETR;
}//passwordImunitetRegREGBigComponent(int adrReg)
