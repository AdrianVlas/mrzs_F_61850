#include "header.h"

extern unsigned char  *outputPacket;
extern unsigned char  outputPacket_USB[300];
extern unsigned char  outputPacket_RS485[300];

extern int sizeOutputPacket;
extern unsigned char *inputPacket;
extern int *received_count;
extern int globalcntBit;//к-во бит
extern int globalcntReg;//к-во регистров
extern int globalbeginAdrReg;//адрес нач регистра
extern int globalbeginAdrBit;//адрес нач бит

#define DISKRET_TOTAL NUMBER_TOTAL_SIGNAL_FOR_RANG
// - NUMBER_UP_SIGNAL_FOR_RANG + 3)
#define DISKRET_REGISTRATOR 0
#define ANALOG_REGISTRATOR  1

int  outputFunc20PacketEncoder(int adrUnit, int *fileNumber, int *recordNumber, int *recordLen, int cntitem);
int  inputPacketParser(void);
void inputPacketParserUSB(void);
void inputPacketParserRS485(void);
unsigned short int  AddCRC(unsigned char inpbyte, unsigned short int oldCRC);
int  Error_modbus_m(unsigned int address, unsigned int function, unsigned int error, unsigned char *output_data);
int openRegistrator(int number_file);
int configAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen);
int dataAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen);
int configDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen);
int dataDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen);
int recordNumberCase0Case1(int offsetRegister, int recordNumber, int recordLen, int registrator);
int recordNumberCaseDiskret(int subObj, int offsetRegister);
int recordNumberCaseOther(int subObj, int offsetRegister, int recordLen, int registrator);

int superReader20(int offsetRegister, int fileNumber, int recordNumber, int recordLen);

//Ідентитифікатор каналу - 16 ASCII символів
const char idetyficator_rang[MAX_NAMBER_LANGUAGE][NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG + 3][16] =
{
  {NAME_RANG_RU},
  {NAME_RANG_UA},
  {NAME_RANG_EN},
  {NAME_RANG_KZ},
};

/**************************************/
//разбор входного пакета USB
/**************************************/
void inputPacketParserUSB(void)
{
  pointInterface=USB_RECUEST;//метка интерфейса 0-USB 1-RS485

  received_count = &usb_received_count;

  inputPacket = usb_received;
  outputPacket = outputPacket_USB;
  //Перевірка контрольної суми
  unsigned short CRC_sum;
  CRC_sum = 0xffff;
  for (int index = 0; index < (*received_count-2); index++) CRC_sum = AddCRC(*(inputPacket + index),CRC_sum);
  if((CRC_sum & 0xff)  != *(inputPacket+*received_count-2)) return;
  if ((CRC_sum >> 8  ) != *(inputPacket+*received_count-1)) return;

  if(inputPacket[0]!=current_settings.address) return;

  if(inputPacketParser()==0) return;

  usb_transmiting_count = sizeOutputPacket;
  for (int i = 0; i < usb_transmiting_count; i++) usb_transmiting[i] = outputPacket[i];
  data_usb_transmiting = true;
}//inputPacketParserUSB(void)

/**************************************/
//разбор входного пакета RS485
/**************************************/
void inputPacketParserRS485(void)
{
  pointInterface=RS485_RECUEST;//метка интерфейса 0-USB 1-RS485

  inputPacket = RxBuffer_RS485;

  received_count = &RxBuffer_RS485_count;
  outputPacket = outputPacket_RS485;

  //Перевірка контрольної суми
  unsigned short CRC_sum;
  CRC_sum = 0xffff;
  for (int index = 0; index < (*received_count-2); index++) CRC_sum = AddCRC(*(inputPacket + index),CRC_sum);
  if((CRC_sum & 0xff)  != *(inputPacket+*received_count-2) ||//) return;
      (CRC_sum >> 8  ) != *(inputPacket+*received_count-1))
    {
      /***
      12345
      Причина рестарту (не співпала контрольна сума)
      ***/
      //reason_of_restart_RS485 |= (1 << 5);
      /***/

      restart_monitoring_RS485();
      return;
    }

  if(!(inputPacket[0]==current_settings.address))
    {
      /***
      12345
      Причина рестарту (не співпала адреса 2)
      ***/
      //reason_of_restart_RS485 |= (1 << 6);
      /***/

      restart_monitoring_RS485();
      return;
    }

  if(inputPacketParser()==0)
    {
      /***
      12345
      Причина рестарту (запит пакету відповіді завеликого розміру)
      ***/
      // reason_of_restart_RS485 |= (1 << 7);
      /***/

      restart_monitoring_RS485();
      return;
    }

  TxBuffer_RS485_count = sizeOutputPacket;
  for (int i = 0; i < TxBuffer_RS485_count; i++) TxBuffer_RS485[i] = outputPacket[i];
  start_transmint_data_via_RS_485(TxBuffer_RS485_count);
}//inputPacketParserRS485(void)

int outputFunc20PacketEncoder(int adrUnit, int *fileNumberArray, int *recordNumberArray, int *recordLenArray, int cntitem)
{
//выходной кодировщик 20 функции
  short dataRegister[130];
  int   idxDataRegister = 0;
  int idxOutputPacket = 0;
  unsigned int respDataLength = 0;
//adrUnit
  outputPacket[idxOutputPacket] = (unsigned char)adrUnit;
  idxOutputPacket++;
//numFunc
  idxOutputPacket++;
//Resp. Data length
  idxOutputPacket++;

  for(int item=0; item<cntitem; item++)  //блоки fileNumber, recordNumber, recordLen
    {
     int tmp1 = openRegistrator(fileNumberArray[item]);//открыть данные регистратора AR DR
     if (tmp1!=0) return tmp1;

//File resp. length
      respDataLength += outputPacket[idxOutputPacket] = (unsigned char)((1+recordLenArray[item]*2)&0xFF);
      idxOutputPacket++;
//Ref. Type
      outputPacket[idxOutputPacket] = 6;
      idxOutputPacket++;

      for(; idxDataRegister<recordLenArray[item]; idxDataRegister++)
        {
          int result = superReader20(idxDataRegister, fileNumberArray[item], recordNumberArray[item], recordLenArray[item]);

          switch(result)
            {
            case MARKER_ERRORPERIMETR:
              return -1;
            default:
            {

              dataRegister[idxDataRegister] = (short) result;
              //        flag = 0;
            }
            }//switch
        }//for

      for(int i=0; i<idxDataRegister; i++)
        {
//Mdata
          outputPacket[idxOutputPacket] = (unsigned char)((dataRegister[i]>>8)&0xFF);
          idxOutputPacket++;
//Ldata
          outputPacket[idxOutputPacket] = (unsigned char)(dataRegister[i]&0xFF);
          idxOutputPacket++;
        }//for(int i=0; i<idxDataRegister; i++)
      idxDataRegister=0;
    }//for(int item=0; item<cntitem; item++)

//Resp. Data length
  outputPacket[2] = respDataLength+cntitem;
  return idxOutputPacket;
}//outputFunc20PacketEncoder(int adrUnit, int *fileNumber, int *recordNumber, int *recordLen, int cntitem)

/**************************************/
//регистровый читатель 20-й функции
/**************************************/
int superReader20(int offsetRegister, int fileNumber, int recordNumber, int recordLen)
{
  switch (fileNumber)
    {
    case 1://Конфигурация аналогового регистратора
      return configAnalogRegistrator(offsetRegister, recordNumber, recordLen);


    case 2://Данные аналогового регистратора 1часть
    case 3://Данные аналогового регистратора 2часть
    case 4://Данные аналогового регистратора 3часть
      if (fileNumber == 3) recordNumber += 10000;
      else if (fileNumber == 4) recordNumber += 20000;

      return dataAnalogRegistrator(offsetRegister, recordNumber, recordLen);

    case 5://Конфигурация дискретного регистратора
      return configDiskretRegistrator(offsetRegister, recordNumber, recordLen);

    case 6://Данные дискретного регистратора 1часть
      return dataDiskretRegistrator(offsetRegister, recordNumber, recordLen);

    }//switch

  return MARKER_ERRORPERIMETR;
}//superReader20(int fileNumber, int offsetRegister)

/***********************************************************************************/
//Функція відповіді по протоколу Modbus-RTU на помилку
/***********************************************************************************/
int Error_modbus_m(unsigned int address, unsigned int function, unsigned int error, unsigned char *output_data)
{
  output_data[0] = address & 0xff;
  output_data[1] = 0x80 | (function & 0xff);
  output_data[2] = error & 0xff;
  return 3;
}

/***********************************************************************************/
//Додавання числа до CRC суми
/***********************************************************************************/
unsigned short AddCRC(unsigned char inpbyte, unsigned short oldCRC)
{
  unsigned short v,temp;
  int i;
  unsigned char bt,m;

  bt = inpbyte;
  v  = oldCRC;

  for (i=0; i<8; i++)
    {
      temp = v;
      v = v >> 1;
      m = bt >> i;
      if (((temp ^m) & 1)== 1) v = v ^ 0xa001;
    }
  return v;
}

/***********************************************************************************/
//открыть данные регистратора AR DR
/***********************************************************************************/
int openRegistrator(int number_file)
{
//ОБЩЕЕ AR DR
  if (
    (
      (number_file >= 1) &&
      (number_file <= 4) &&
      (
        ((pointInterface == USB_RECUEST  ) && (number_record_of_ar_for_USB   == 0xffff)) ||
        ((pointInterface == RS485_RECUEST  ) && (number_record_of_ar_for_RS485 == 0xffff))
      )
    )
    ||
    (
      (number_file >= 5) &&
      (number_file <= 6) &&
      (
        ((pointInterface == USB_RECUEST  ) && (number_record_of_dr_for_USB   == 0xffff)) ||
        ((pointInterface == RS485_RECUEST  ) && (number_record_of_dr_for_RS485 == 0xffff))
      )
    )
  )
    {
      //Невірний номер файлу, або не подано команди вичитування відповідного запису
      return -1;
    }
  else if (
    (number_file >= 1) &&
    (number_file <= 4) &&
    (
      (
        (state_ar_record != STATE_AR_NO_RECORD      ) &&
        (state_ar_record != STATE_AR_TEMPORARY_BLOCK)
      )
      ||
      ((clean_rejestrators & CLEAN_AR) != 0)
      ||
      (
        (control_tasks_dataflash & (
           TASK_ERASE_DATAFLASH_2                                        |
           TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR |
           TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR
         )
        ) != 0
      )
    )
  )
    {
      //Зараз іде операція запису/стирання для аналоговго реєстратора, яка може тривати довго (післяаварійний масив становить 20 с), тому читання аналогового реєстратора є тимчасово недоступне
      //error = ERROR_SLAVE_DEVICE_BUSY;
      return -2;
      /*
      Тут ми не перевіряємо умову на виставлений біт TASK_MAMORY_READ_DATAFLASH_FOR_AR_INTERFACE,
      бо, оскільки, масиви є великі і зразу весь запис прочитати не можливо, то
      програмне забеспечення побудоване на принципі докачки. Тобто формується відповідь з даних, які зараз
      зчитуються з мікросхеми dataFlash
      */
    }
  else if (
    (number_file >= 5) &&
    (number_file <= 6) &&
    (
      (
        ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB  ) != 0)) ||
        ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485) != 0))
      )
      ||
      ((clean_rejestrators & CLEAN_DR) != 0)
    )
  )
    {
      //Зараз іде зчитування для інтерфейсу запису дискретного реєстратора, тому ця операція є тимчасово недоступною
      //error = ERROR_SLAVE_DEVICE_BUSY;
      return -2;
    }//if

//ТОЛЬКО AR
  if(number_file<5)//
    {
      //Очікуємо поки завершиться зчитуквання даних для аналогового реєстратора
      while (
        ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB  ) != 0)) ||
        ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485) != 0))
      )
        {
          //Якщ очасом буде спрацьовувати Watchdog, то тут треба буде поставити функцію роботи з ним
          if (
            (control_spi1_taskes[0] != 0) ||
            (control_spi1_taskes[1] != 0) ||
            (state_execution_spi1 > 0)
          )
            {
              mutex_spi1 = true;
              if (driver_spi_df[number_chip_dataflsh_exchange].state_execution == TRANSACTION_EXECUTING_NONE)
                {
                  main_routines_for_spi1();
                }
              mutex_spi1 = false;
            }
        }//while
    }//if(number_file<5)
//ТОЛЬКО DR
  else
    {
      while (
        ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB  ) != 0)) ||
        ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485) != 0))
      )
        {
          //Якщ очасом буде спрацьовувати Watchdog, то тут треба буде поставити функцію роботи з ним
          if (
            (control_spi1_taskes[0] != 0) ||
            (control_spi1_taskes[1] != 0) ||
            (state_execution_spi1 > 0)
          )
            {
              mutex_spi1 = true;
              if (driver_spi_df[number_chip_dataflsh_exchange].state_execution == TRANSACTION_EXECUTING_NONE)
                {
                  main_routines_for_spi1();
                }
              mutex_spi1 = false;
            }
        }//while
    }//else

  return 0;
}//openRegistrator(int number_file)

int dataAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen)
{
//данные аналогового регистратора
  //Максимальна кількість часових зрівів
  int max_number_time_sample = (current_settings.prefault_number_periods + current_settings.postfault_number_periods) << VAGA_NUMBER_POINT_AR;
  unsigned int lenMax = (3 + NUMBER_ANALOG_CANALES + number_word_digital_part_ar);
  if ((unsigned int)recordLen > lenMax) return MARKER_ERRORPERIMETR;//уйти если превышение

  if(recordNumber>max_number_time_sample) return MARKER_ERRORPERIMETR;//уйти если превышение


  int *point_to_first_number_time_sample, *point_to_last_number_time_sample;

  if (pointInterface == USB_RECUEST)
    {
      point_to_first_number_time_sample = &first_number_time_sample_for_USB;
      point_to_last_number_time_sample  = &last_number_time_sample_for_USB;
    }
  else
    {
      point_to_first_number_time_sample = &first_number_time_sample_for_RS485;
      point_to_last_number_time_sample  = &last_number_time_sample_for_RS485;
    }

  //Перевіряємо чи зчитано заголовок аналогового реєстратора
  if (
    (((int)recordNumber) < (*point_to_first_number_time_sample)) ||
    (((int)recordNumber) > (*point_to_last_number_time_sample ))
  )
    {
      //Запитуваний часовий зріз не є зчитаним

      //Виставляємо читання без заголовку запису даного запису з запитуваноого номеру зрізу і дальше, скільки можливо, часових зрізів
      *point_to_first_number_time_sample = recordNumber;
      int last_number_time_sample_tmp = recordNumber + SIZE_PAGE_DATAFLASH_2/((NUMBER_ANALOG_CANALES + number_word_digital_part_ar)*sizeof(short int));
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

      //Очікуємо поки завершиться зчитуквання даних для аналогового реєстратора
      while (
        ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB  ) != 0)) ||
        ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485) != 0))
      )
        {
          //Якщо очасом буде спрацьовувати Watchdog, то тут треба буде поставити функцію роботи з ним
          if (
            (control_spi1_taskes[0] != 0) ||
            (control_spi1_taskes[1] != 0) ||
            (state_execution_spi1 > 0)
          )
            {
              mutex_spi1 = true;
              if (driver_spi_df[number_chip_dataflsh_exchange].state_execution == TRANSACTION_EXECUTING_NONE)
                {
                  main_routines_for_spi1();
                }
              mutex_spi1 = false;
            }
        }//while
    }

  //Якщо ми сюди дійшли, то вважаємо що запитувана виборка зчитана і знаходиться у буфері читання аналогового реєстратора для інтерфейсу

  //Визначаємо індекс у буфері читання з якого розміщений потрібний нам часовий зріз
  unsigned int index_time_sample;
  if((*point_to_first_number_time_sample) == -1)
    {
      index_time_sample = sizeof(__HEADER_AR) + recordNumber*
                          (NUMBER_ANALOG_CANALES + number_word_digital_part_ar)*sizeof(short int);
    }
  else
    {
      index_time_sample = 0 + (recordNumber - (*point_to_first_number_time_sample))*
                          (NUMBER_ANALOG_CANALES + number_word_digital_part_ar)*sizeof(short int);
    }

  switch(offsetRegister)
    {
    case 0://порядковый номер выборки
      return recordNumber + 1;
    case 1://метка времени (ms)
      return recordNumber & 0xffff;
    case 2://метка времени (ms)
      return recordNumber >> 16;
    }//switch

  unsigned char *point_to_buffer;
  if (pointInterface == USB_RECUEST)
    point_to_buffer = buffer_for_USB_read_record_ar;
  else
    point_to_buffer = buffer_for_RS485_read_record_ar;

  return (*(point_to_buffer + index_time_sample +2*(offsetRegister-3))) + ((*(point_to_buffer +
         index_time_sample + 1 +2*(offsetRegister-3))) << 8);
}//dataAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen)

int configAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen)
{
//Конфигурация аналогового регистратора

  switch(recordNumber)
    {
    case 0://Имя станции recordNumber
    case 1://Количество каналов recordNumber
      return recordNumberCase0Case1(offsetRegister, recordNumber, recordLen, ANALOG_REGISTRATOR);
    }//switch

//АНАЛОГОВЫЕ КАНАЛЫ
  if(recordNumber>=2 && recordNumber<=(2+NUMBER_ANALOG_CANALES-1))
    {
      if(recordLen>27) return MARKER_ERRORPERIMETR;
      int subObj = recordNumber-2;//индекс канала
      __HEADER_AR *header_ar_tmp;

      if (pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
        header_ar_tmp = (__HEADER_AR*)buffer_for_USB_read_record_ar;
      else
        header_ar_tmp = (__HEADER_AR*)buffer_for_RS485_read_record_ar;

      char idetyficator[NUMBER_ANALOG_CANALES][16] =
      {
        "3I0             ",
        "Ia              ",
        "                ",
        "Ic              ",
        "                ",
        "                ",
        "                ",
        "3U0             "
      };
      const char idetyficator_current[2][16] =
      {
        "Ib              ",
        "I 0.4kV         "
      };

      unsigned int phase_line = header_ar_tmp->control_extra_settings_1 & CTR_EXTRA_SETTINGS_1_CTRL_PHASE_LINE;
      unsigned int Ib_I04 = header_ar_tmp->control_extra_settings_1 & CTR_EXTRA_SETTINGS_1_CTRL_IB_I04;
      for (unsigned int k = 0; k < 16; k++) idetyficator[2][k] = idetyficator_current[Ib_I04 != 0][k];

      if (phase_line == 0)
        {
          const char idetyficator_phase[3][16] =
          {
            "Ua              ",
            "Ub              ",
            "Uc              "
          };
          char *point_to_changed_name = idetyficator[I_Ua + 0];
          for (unsigned int j = 0; j < 3; j++)
            {
              for (unsigned int k = 0; k < 16; k++) *(point_to_changed_name + j*16 + k) = idetyficator_phase[j][k];
            }
        }
      else
        {
          const char idetyficator_line[3][16] =
          {
            "Uab             ",
            "Ubc             ",
            "Uca             "
          };
          char *point_to_changed_name = idetyficator[I_Ua + 0];
          for (unsigned int j = 0; j < 3; j++)
            {
              for (unsigned int k = 0; k < 16; k++) *(point_to_changed_name + j*16 + k) = idetyficator_line[j][k];
            }
        }

      switch(offsetRegister)
        {
        case 0://Порядковый номер аналогового канала offsetRegister
          return 1+subObj;
        case 1://Идентификатор канала0 offsetRegister
          return idetyficator[subObj][0] | (idetyficator[subObj][1]<<8);
        case 2://Идентификатор канала1 offsetRegister
          return idetyficator[subObj][2] | (idetyficator[subObj][3]<<8);
        case 3://Идентификатор канала2 offsetRegister
          return idetyficator[subObj][4] | (idetyficator[subObj][5]<<8);
        case 4://Идентификатор канала3 offsetRegister
          return idetyficator[subObj][6] | (idetyficator[subObj][7]<<8);
        case 5://Идентификатор канала4 offsetRegister
          return idetyficator[subObj][8] | (idetyficator[subObj][9]<<8);
        case 6://Идентификатор канала5 offsetRegister
          return idetyficator[subObj][10] | (idetyficator[subObj][11]<<8);
        case 7://Идентификатор канала6 offsetRegister
          return idetyficator[subObj][12] | (idetyficator[subObj][13]<<8);
        case 8://Идентификатор канала7 offsetRegister
          return idetyficator[subObj][14] | (idetyficator[subObj][15]<<8);
        case 9://Фаза канала offsetRegister
        {
          //Фаза каналу - 2 ASCII символів
          char phase[NUMBER_ANALOG_CANALES][2] =
          {
            "I0",
            "A ",
            "  ",
            "C ",
            "  ",
            "  ",
            "  ",
            "U0"
          };

          {
            const char phase_c[2][2] =
            {
              "B ",
              "04"
            };
            for (unsigned int k = 0; k < 2; k++) phase[I_Ib_I04][k] = phase_c[Ib_I04 != 0][k];
          }

          if (phase_line == 0)
            {
              const char phase_p[3][2] =
              {
                "A ",
                "B ",
                "C "
              };
              char *point_to_changed_name = phase[I_Ua + 0];
              for (unsigned int j = 0; j < 3; j++)
                {
                  for (unsigned int k = 0; k < 2; k++) *(point_to_changed_name + j*2 + k) = phase_p[j][k];
                }
            }
          else
            {
              const char pase_l[3][2] =
              {
                "AB",
                "BC",
                "CA"
              };
              char *point_to_changed_name = phase[I_Ua + 0];
              for (unsigned int j = 0; j < 3; j++)
                {
                  for (unsigned int k = 0; k < 2; k++) *(point_to_changed_name + j*2 + k) = pase_l[j][k];
                }
            }

          return phase[subObj][0] | (phase[subObj][1]<<8);
        }//case 9
        case 10://Наблюдаемый компонент цепи0 offsetRegister
        case 11://Наблюдаемый компонент цепи1 offsetRegister
        case 12://Наблюдаемый компонент цепи2 offsetRegister
        case 13://Наблюдаемый компонент цепи3 offsetRegister
        case 14://Наблюдаемый компонент цепи4 offsetRegister
        case 15://Наблюдаемый компонент цепи5 offsetRegister
        case 16://Наблюдаемый компонент цепи6 offsetRegister
        case 17://Наблюдаемый компонент цепи7 offsetRegister
        {
          //Спостережний елемент в колі - 16 ASCII символів - нічого тут не передаємо, тому у це поле поміщаємо ' '
          return ((' ')<<8) | (' ');
        }//case 10-17
        case 18://Единицы измерения offsetRegister
        {
          //Одиниці вимірювання - 2 ASCII символів
          const char label_meas[2][2] = {"mA","mV"};
          unsigned int index;
          if (subObj <= I_Ic)index = 0;
          else index = 1;

          return label_meas[index][0] | (label_meas[index][1]<<8);
        }//case 18
        case 19://Коэффициент канала offsetRegister
        {
          //Коефіцієнт каналу
          if      (subObj == I_3I0) return (1000*MNOGNYK_3I0) >> (VAGA_DILENNJA_3I0 + 8);
          else if (subObj <= I_Ic ) return (1000*MNOGNYK_I)   >> (VAGA_DILENNJA_I   + 4);
          else return (1000*MNOGNYK_U)   >> (VAGA_DILENNJA_U   + 4);
        }//case 19
        case 20://Добавочное смещение offsetRegister
          //Додаткове зміщення
          return 0;
        case 21://Сдвиг времени канала (µs) offsetRegister
          //Зсув у часі
          return 0; //Час зсуву відпосно початку зчитування
        case 22://Нижний предел значений offsetRegister
          //Нижня гнаниця
          return -32767;
        case 23://Верхний предел значений offsetRegister
          //Верхня границя
          return 32767;
        case 24://Первичный коэф. трансф. offsetRegister
        {
          //Первинний коефіцієнт трансформації
          if (subObj == I_3I0)
            {
              return  header_ar_tmp->T0;
            }
          else if (subObj <= I_Ic )
            {
              if (
                (subObj != I_Ib_I04 )// || (I_Ib_I04 == 0)
              )
                {
                  return   header_ar_tmp->TCurrent;
                }
              else
                {
                  return header_ar_tmp->TCurrent04;
                }
            }
          else
            {
              return header_ar_tmp->TVoltage;
            }
        }//case 24
        case 25://Вторичный коэф. трансф. offsetRegister
          //Вторинний коефіцієнт трансформації
          return 1;
        case 26://Первичный или вторичный идентификатор offsetRegister
          //Дані будуть передаватися з вторинної обмотки
          return 'S';
        }//switch
      return MARKER_ERRORPERIMETR;
    }//if(recordNumber>=2 && recordNumber<=(2+NUMBER_ANALOG_CANALES))

//ДИСКРЕТНЫЕ КАНАЛЫ
  if(recordNumber>=((2+NUMBER_ANALOG_CANALES)) && recordNumber<=(((2+NUMBER_ANALOG_CANALES)) +DISKRET_TOTAL)-1)
    {
      if(recordLen>19) return MARKER_ERRORPERIMETR;
      int subObj = recordNumber-((2+NUMBER_ANALOG_CANALES));//индекс канала
      return recordNumberCaseDiskret(subObj, offsetRegister);

    }//if(recordNumber>=(2+NUMBER_ANALOG_CANALES +1) && recordNumber<=(2+NUMBER_ANALOG_CANALES +1 +100))

//ОСТАЛЬНЫЕ
  if(recordNumber>=(((2+NUMBER_ANALOG_CANALES)) +DISKRET_TOTAL) && recordNumber<=((((2+NUMBER_ANALOG_CANALES)) +DISKRET_TOTAL) +7)-1)
    {
      int subObj = recordNumber-(((2+NUMBER_ANALOG_CANALES)) +DISKRET_TOTAL);//индекс записи
      return recordNumberCaseOther(subObj, offsetRegister, recordLen, ANALOG_REGISTRATOR);

    }//if(recordNumber>=(2+NUMBER_ANALOG_CANALES +1 +DISKRET_TOTAL +1) && recordNumber<=(2+NUMBER_ANALOG_CANALES +1 +DISKRET_TOTAL +1 +7))

  return MARKER_ERRORPERIMETR;
}//configAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen)

int configDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen)
{
//Конфигурация дискретного регистратора
  switch(recordNumber)
    {
    case 0://Имя станции recordNumber
    case 1://Количество каналов recordNumber
      return recordNumberCase0Case1(offsetRegister, recordNumber, recordLen, DISKRET_REGISTRATOR);
    }//switch

//ДИСКРЕТНЫЕ КАНАЛЫ
  if(recordNumber>=2 && recordNumber<=(2+DISKRET_TOTAL)-1)
    {
      if(recordLen>19) return MARKER_ERRORPERIMETR;
      int subObj = recordNumber-2;//индекс канала
      return recordNumberCaseDiskret(subObj, offsetRegister);

    }//if(recordNumber>=2 && recordNumber<=(2+DISKRET_TOTAL)-1)

//ОСТАЛЬНЫЕ
  if(recordNumber>=((2+DISKRET_TOTAL)) && recordNumber<=(((2+DISKRET_TOTAL)) +7)-1)
    {
      int subObj = recordNumber-((2+DISKRET_TOTAL));//индекс записи
      return recordNumberCaseOther(subObj, offsetRegister, recordLen, DISKRET_REGISTRATOR);

    }//if(recordNumber>=((2+DISKRET_TOTAL)) && recordNumber<=(((2+DISKRET_TOTAL)) +7)-1)

  return MARKER_ERRORPERIMETR;
}//configDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen)

int recordNumberCase0Case1(int offsetRegister, int recordNumber, int recordLen, int registrator)
{
  //Якщо ми сюди дійшли, то вважаємо що заголовок аналогового реєстратора вже зчитаний
  __HEADER_AR *header_ar_tmp;

  if (pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
    header_ar_tmp = (__HEADER_AR*)buffer_for_USB_read_record_ar;
  else
    header_ar_tmp = (__HEADER_AR*)buffer_for_RS485_read_record_ar;
  switch(recordNumber)
    {
    case 0://Имя станции recordNumber
    {

      if(recordLen>10) return MARKER_ERRORPERIMETR;
      if(registrator==ANALOG_REGISTRATOR)
        {
          switch(offsetRegister)
            {
            case 0://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 0] | (header_ar_tmp->name_of_cell[ 1]<<8);
            case 1://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 2] | (header_ar_tmp->name_of_cell[ 3]<<8);
            case 2://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 4] | (header_ar_tmp->name_of_cell[ 5]<<8);
            case 3://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 6] | (header_ar_tmp->name_of_cell[ 7]<<8);
            case 4://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 8] | (header_ar_tmp->name_of_cell[ 9]<<8);
            case 5://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 10] | (header_ar_tmp->name_of_cell[ 11]<<8);
            case 6://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 12] | (header_ar_tmp->name_of_cell[ 13]<<8);
            case 7://Имя станции offsetRegister
              return header_ar_tmp->name_of_cell[ 14] | (header_ar_tmp->name_of_cell[ 15]<<8);
            case 8://Идентиф. номер устройства offsetRegister
              //Серійний номер пристрою завжди один і той самий (встановлюється на заводі-виготовнику), тому я його не включив для запису у DataFlash у складі заголовку аналогового реєстратора
              return serial_number_dev & 0xffff;
            case 9://Год редакции стандарта (1999) offsetRegister
              return 1999;

            }//switch
        }//if(registrator==ANALOG_REGISTRATOR)
      else
        {
          //if(registrator==DISKRET_REGISTRATOR)
          unsigned char *point_to_buffer;
          if (pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
            point_to_buffer = buffer_for_USB_read_record_dr;
          else point_to_buffer = buffer_for_RS485_read_record_dr;
          if (offsetRegister < 8)
            {
              unsigned int index_cell;

              index_cell =  (offsetRegister - 0)<<1;
              return (*(point_to_buffer + FIRST_INDEX_NAME_OF_CELL_DR + index_cell)) | ((*(point_to_buffer + FIRST_INDEX_NAME_OF_CELL_DR + index_cell + 1))<<8);
            }
          else if (offsetRegister == 8)
            {
              //Серійний номер пристрою завжди один і той самий (встановлюється на заводі-виготовнику), тому я його не включив для запису у DataFlash у складі заголовку аналогового реєстратора
              return serial_number_dev & 0xffff;
            }
          else
            {
              return 1999;
            }
        }//else if(registrator==DISKRET_REGISTRATOR)
    }//case 0
    break;

    case 1://Количество каналов recordNumber
    {
      if(recordLen>3) return MARKER_ERRORPERIMETR;
      if(registrator==ANALOG_REGISTRATOR)
        {
          switch(offsetRegister)
            {
            case 0://Количество каналов всего offsetRegister
              return NUMBER_ANALOG_CANALES + NUMBER_TOTAL_SIGNAL_FOR_RANG;
            case 1://Количество аналоговых каналов offsetRegister
              return NUMBER_ANALOG_CANALES;
            case 2://Количество дискретных каналов offsetRegister
              return NUMBER_TOTAL_SIGNAL_FOR_RANG;
            }//switch
          return MARKER_ERRORPERIMETR;
        }// if(registrator==ANALOG_REGISTRATOR)
      else
        {
          //if(registrator==DISKRET_REGISTRATOR)
          switch(offsetRegister)
            {
            case 0://Количество каналов всего offsetRegister
              return   0 + NUMBER_TOTAL_SIGNAL_FOR_RANG;
            case 1://Количество аналоговых каналов offsetRegister
              return 0;
            case 2://Количество дискретных каналов offsetRegister
              return NUMBER_TOTAL_SIGNAL_FOR_RANG;
            }//switch

        }//else
    }
    break;

    }//switch
  return MARKER_ERRORPERIMETR;
}//recordNumberCase0Case1(int offsetRegister, int recordNumber, int recordLen)

int recordNumberCaseDiskret(int subObj, int offsetRegister)
{
  switch(offsetRegister)
    {
    case 0://Порядковый номер дискретного канала offsetRegister
      return 1+subObj;
    case 1://Идентификатор канала0 offsetRegister
    case 2://Идентификатор канала1 offsetRegister
    case 3://Идентификатор канала2 offsetRegister
    case 4://Идентификатор канала3 offsetRegister
    case 5://Идентификатор канала4 offsetRegister
    case 6://Идентификатор канала5 offsetRegister
    case 7://Идентификатор канала6 offsetRegister
    case 8://Идентификатор канала7 offsetRegister
    {
      const uint32_t index_number_UP[MAX_NAMBER_LANGUAGE][3] =
      {
        {11, 10, 8},
        {11, 10, 8},
        {12,  7, 8},
        {11, 10, 8},
      };

      int index_language = index_language_in_array(current_settings.language);

      unsigned int index_cell =  (offsetRegister - 1)<<1;
      unsigned int temp_data = 0;
      for (size_t sym = 0; sym < 2; sym++)
        {
          unsigned int index_row;
          if (subObj < (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG))
            {
              index_row = subObj;
            }
          else if (subObj < (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG))
            {
              index_row = (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG) +
                          ((subObj - (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG)) % 3);
            }
          else
            {
              index_row = subObj - NUMBER_UP_SIGNAL_FOR_RANG + 3;
            }

          if (
            (subObj >= (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG))  &&
            (subObj <  (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG)) &&
            ((index_cell + sym) == index_number_UP[index_language][(subObj - (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG)) % 3])
          )
            {
              temp_data |= (0x30 + ((subObj - (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG)) / 3 + 1)) << (8*sym);
            }
          else temp_data |= (idetyficator_rang[index_language][index_row][(index_cell + sym)]) << (8*sym);
        }
      return temp_data;
    }//case 1-8
    case 9://Фаза канала offsetRegister
      //Фаза каналу - 2 ASCII символів - нічого не передаємо
      return (' '<<8) | ' ';
    case 10://Наблюдаемый компонент цепи0 offsetRegister
    case 11://Наблюдаемый компонент цепи1 offsetRegister
    case 12://Наблюдаемый компонент цепи2 offsetRegister
    case 13://Наблюдаемый компонент цепи3 offsetRegister
    case 14://Наблюдаемый компонент цепи4 offsetRegister
    case 15://Наблюдаемый компонент цепи5 offsetRegister
    case 16://Наблюдаемый компонент цепи6 offsetRegister
    case 17://Наблюдаемый компонент цепи7 offsetRegister
    {
      //Спостережний елемент в колі - 16 ASCII символів- нічого не передаємо
      return (' '<<8) | ' ';
    }//case 10-18
    case 18://Нормальное состояние offsetRegister
      //Нормальний стан
      return 0;
    }//switch
  return MARKER_ERRORPERIMETR;
}//recordNumberCaseDiskret(int subObj, int offsetRegister, int recordLen)

int recordNumberCaseOther(int subObj, int offsetRegister, int recordLen, int registrator)
{
  __HEADER_AR *header_ar_tmp;
  unsigned char *point_to_buffer;
  if (pointInterface == USB_RECUEST) //метка интерфейса 0-USB 1-RS485
    point_to_buffer = buffer_for_USB_read_record_dr;
  else
    point_to_buffer = buffer_for_RS485_read_record_dr;

  int max_number_time_sample = (current_settings.prefault_number_periods + current_settings.postfault_number_periods) << VAGA_NUMBER_POINT_AR;

  if (pointInterface==USB_RECUEST) //метка интерфейса 0-USB 1-RS485
    {
      header_ar_tmp = (__HEADER_AR*)buffer_for_USB_read_record_ar;
    }//if
  else
    {
      header_ar_tmp = (__HEADER_AR*)buffer_for_RS485_read_record_ar;
    }

  switch(subObj)
    {
    case 0://Частота линии (Гц) subObj
      if(recordLen>1) return MARKER_ERRORPERIMETR;
      return 5000;
    case 1://Кол-во частот дискретизации subObj
      if(recordLen>1) return MARKER_ERRORPERIMETR;
      if(registrator==ANALOG_REGISTRATOR) return 1;
      else return 0;
    case 2://Частота выборки1 (Гц) subObj
    {
      if(recordLen>2) return MARKER_ERRORPERIMETR;
      switch(offsetRegister)
        {
        case 0://Частота выборки1 (Гц) offsetRegister
          if(registrator==ANALOG_REGISTRATOR) return ((50*1000)<<VAGA_NUMBER_POINT_AR)>>4; //Чаcтота
          return 0;
        case 1://Последняя выборка на этой частоте offsetRegister
          if(registrator==ANALOG_REGISTRATOR) return max_number_time_sample; //Частота виборки
          return *(point_to_buffer + FIRST_INDEX_NUMBER_ITEMS_DR); //остання виборка на даній чатоті дискретизації рівна останній
        }//switch

      return MARKER_ERRORPERIMETR;
    }//case 2
    case 3://день (Дата и время первых данных) subObj
    case 4://день (Дата и время запуска) subObj
    {
      if(recordLen>7) return MARKER_ERRORPERIMETR;
      switch(offsetRegister)
        {
        case 0://день (Дата и время первых данных) offsetRegister
        case 1://месяц offsetRegister
        case 2://год offsetRegister
        case 3://часы offsetRegister
        case 4://минуты offsetRegister
        case 5://секунды offsetRegister
        case 6://микросекунды offsetRegister
        {
          if(registrator==ANALOG_REGISTRATOR)
            {
              unsigned char time_avar_analog[7];

              //Конвертуємо формат BCD у int
              for (unsigned int i = 0; i < 7; i++)
                {
                  unsigned int val = header_ar_tmp->time[i], val_l, val_m;
                  val_l = val & 0xf;
                  val_m = (val >> 4) & 0xf;
                  time_avar_analog[i] = (unsigned char)(val_m*10 + val_l);
                }

              int temp_data;
              unsigned int max_time_milliseconds_before = (current_settings.prefault_number_periods)*2; //2 - це десяті від 20 мс, що відображає період на 50Гц
              unsigned int flag_carry = 0;
              unsigned int s, ds_ms;


              if(subObj==4)  goto m1;// return time_avar_analog[offsetRegister];
              //Кількість секунд
              s = max_time_milliseconds_before / 100;
              //Кількість десятих і сотих мілісекунд
              ds_ms = max_time_milliseconds_before - s*100;

              //Віднімаємо даесяті і соті мілісекунд
              if (time_avar_analog[0] >= ds_ms)
                {
                  time_avar_analog[0] -= ds_ms;
                  flag_carry = 0;
                }
              else
                {
                  time_avar_analog[0] = time_avar_analog[0] + 100 - ds_ms;
                  flag_carry = 1;
                }

              //Віднімаємо секунди
              if (time_avar_analog[1] >= (s + flag_carry))
                {
                  time_avar_analog[1] -= (s + flag_carry);
                  flag_carry = 0;
                }
              else
                {
                  time_avar_analog[1] = time_avar_analog[1] + 60 - (s + flag_carry);
                  flag_carry = 1;
                }

              //Дальше віднімаємо, якщо є виставлений прапорець переносу
              if (flag_carry != 0)
                {
                  //Віднімаємо хвилини
                  if (time_avar_analog[2] >=  flag_carry)
                    {
                      time_avar_analog[2] -= flag_carry;
                      flag_carry = 0;
                    }
                  else
                    {
                      time_avar_analog[2] = time_avar_analog[2] + 60 - flag_carry;
                      flag_carry = 1;
                    }

                  //Дальше віднімаємо, якщо є виставлений прапорець переносу
                  if (flag_carry != 0)
                    {
                      //Віднімаємо години
                      if (time_avar_analog[3] >=  flag_carry)
                        {
                          time_avar_analog[3] -= flag_carry;
                          flag_carry = 0;
                        }
                      else
                        {
                          time_avar_analog[3] = time_avar_analog[3] + 24 - flag_carry;
                          flag_carry = 1;
                        }

                      //Дальше віднімаємо, якщо є виставлений прапорець переносу
                      if (flag_carry != 0)
                        {
                          //Віднімаємо дні місяця
                          if (time_avar_analog[4] > flag_carry)
                            {
                              time_avar_analog[4] -= flag_carry;
                              flag_carry = 0;
                            }
                          else
                            {
                              unsigned int max_value, number_previous_mounth;

                              if (((int)(time_avar_analog[5] - 1)) > 0) number_previous_mounth = time_avar_analog[5] - 1;
                              else number_previous_mounth = 12; //Попередній місяць - Грудень

                              //Максимальну кількість днів у попередньому місяця (бо ми у місяців "позичаємо" одиничку і потім перейдемо на віднімання переносу у місяців)
                              if (number_previous_mounth == 2)
                                {
                                  //Попередній місяць - лютий
                                  //Перевірка на високосний рік
                                  if((time_avar_analog[6] & 0xfc) == 0)
                                    {
                                      //Рік високосний - кількість днів у лютому 29
                                      max_value = 29;
                                    }
                                  else
                                    {
                                      //Рік не високосний - кількість днів у лютому 28
                                      max_value = 28;
                                    }
                                }
                              else if (
                                ((number_previous_mounth <= 7) && ((number_previous_mounth & 0x01) != 0)) ||
                                ((number_previous_mounth >= 8) && ((number_previous_mounth & 0x01) == 0))
                              )
                                {
                                  //Попередній місяць має 31 день
                                  max_value = 31;
                                }
                              else
                                {
                                  //Попередній місяць має 30 днів
                                  max_value = 30;
                                }

                              time_avar_analog[4] = time_avar_analog[4] + max_value - flag_carry;
                              flag_carry = 1;
                            }

                          //Дальше віднімаємо, якщо є виставлений прапорець переносу
                          if (flag_carry != 0)
                            {
                              //Віднімаємо місяці
                              if ((int)(time_avar_analog[5] - flag_carry) > 0)
                                {
                                  time_avar_analog[5] -= flag_carry;
                                  flag_carry = 0;
                                }
                              else
                                {
                                  time_avar_analog[5] = 12;
                                  flag_carry = 1;
                                }

                              //Дальше віднімаємо, якщо є виставлений прапорець переносу
                              if (flag_carry != 0)
                                {
                                  //Віднімаємо роки
                                  if (time_avar_analog[6] > flag_carry)
                                    {
                                      time_avar_analog[6] -= flag_carry;
                                      flag_carry = 0;
                                    }
                                  else
                                    {
                                      time_avar_analog[6] = 99;
                                      flag_carry = 1;
                                    }
                                }
                            }
                        }
                    }
                }
m1:
              switch(offsetRegister)
                {
                case 0:
                case 1:
                  temp_data = time_avar_analog[4 + offsetRegister];
                  break;
                case 2:
                  temp_data = time_avar_analog[4 + offsetRegister];
                  temp_data += 2000; //Бо формат має бути чотиризначним числом
                  break;
                case 3:
                case 4:
                  temp_data = time_avar_analog[3 - (offsetRegister - 3)];
                  break;
                case 5:
                  temp_data = time_avar_analog[1]*100 + time_avar_analog[0];
                  break;
                default:
                  temp_data = 0;
                }//switch
              return temp_data;
            }//if(registrator==ANALOG_REGISTRATOR)
          else
            {
              //if(registrator==DISKRET_REGISTRATOR)
              //Конвертуємо формат BCD у int
              int temp_data;
              unsigned int time_avar_digital[7];
              for (unsigned int i = 0; i < 7; i++)
                {
                  unsigned int val = *(point_to_buffer + FIRST_INDEX_DATA_TIME_DR + i), val_l, val_m;
                  val_l = val & 0xf;
                  val_m = (val >> 4) & 0xf;
                  time_avar_digital[i] = val_m*10 + val_l;
                }//for
              switch(offsetRegister)
                {
                case 0:
                case 1:
                  temp_data = time_avar_digital[4 + offsetRegister];
                  break;
                case 2:
                  temp_data = time_avar_digital[4 + offsetRegister];
                  temp_data += 2000; //Бо формат має бути чотиризначним числом
                  break;
                case 3:
                case 4:
                  temp_data = time_avar_digital[3 - (offsetRegister - 3)];
                  break;
                case 5:
                  temp_data = time_avar_digital[1]*100 + time_avar_digital[0];
                  break;
                default:
                  temp_data = 0;
                }//switch
              return temp_data;
            } //else
        }//case 0-6
        }//switch
      return MARKER_ERRORPERIMETR;
    }//case 3 4
    case 5://Тип данных subObj
      if(recordLen>1) return MARKER_ERRORPERIMETR;
      //if(registrator==DISKRET_REGISTRATOR) 
      return 'B'; //дані - це бінарні числа
      //return (1000000 >> VAGA_NUMBER_POINT_AR)/50; //Період дискретизації
    case 6://Коэффициент временной метки subObj
      if(recordLen>1) return MARKER_ERRORPERIMETR;
    if(registrator==DISKRET_REGISTRATOR) return 1000;
    return (1000000 >> VAGA_NUMBER_POINT_AR)/50;
    }//switch

  return MARKER_ERRORPERIMETR;
}//

int dataDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen)
{
//данные Diskret регистратора
  unsigned char *point_to_buffer;
  if (pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
    point_to_buffer = buffer_for_USB_read_record_dr;
  else point_to_buffer = buffer_for_RS485_read_record_dr;

  unsigned int max_number_two_bytes = (NUMBER_TOTAL_SIGNAL_FOR_RANG >> 4);
  if ((max_number_two_bytes << 4) != NUMBER_TOTAL_SIGNAL_FOR_RANG)  max_number_two_bytes++;

  unsigned int lenMax = (3 + max_number_two_bytes);
  if ((unsigned int)recordLen > lenMax) return MARKER_ERRORPERIMETR;//уйти если превышение

  if(recordNumber>(*(point_to_buffer + FIRST_INDEX_NUMBER_ITEMS_DR))) return MARKER_ERRORPERIMETR;//уйти если превышение

  unsigned int offset = FIRST_INDEX_FIRST_DATA_DR + (recordNumber + 1)*38; //бо найперший запис містить попереднє значення (до фіксації запуску роботи дискретного реєстратора)

  switch(offsetRegister)
    {
    case 0://порядковый номер выборки
      return recordNumber + 1;
    case 1://метка времени (ms)
      return (*(point_to_buffer + offset + 0)) + ((*(point_to_buffer + offset + 1)) << 8 );
    case 2://метка времени (ms)
      return *(point_to_buffer + offset + 2);
    }//switch

  return (*(point_to_buffer + offset + 3 + (offsetRegister - 3)*2)) + ((*(point_to_buffer + offset + 4 + (offsetRegister - 3)*2)) << 8);
}//dataDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen)
