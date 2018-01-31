#include "header.h"

extern unsigned char  *outputPacket;
extern unsigned char  outputPacket_USB[300];
extern unsigned char  outputPacket_RS485[300];

extern int sizeOutputPacket;
extern unsigned char *inputPacket;
extern int *received_count;
extern int pointInterface;//метка интерфейса 0-USB 1-RS485
extern int globalcntBit;//к-во бит
extern int globalcntReg;//к-во регистров
extern int globalbeginAdrReg;//адрес нач регистра
extern int globalbeginAdrBit;//адрес нач бит

int outputFunc20PacketEncoder(int adrUnit, int *fileNumber, int *recordNumber, int *recordLen, int cntitem);
int inputPacketParser(void);
void inputPacketParserUSB(void);
void inputPacketParserRS485(void);
unsigned short int  AddCRC(unsigned char inpbyte, unsigned short int oldCRC);
int Error_modbus_m(unsigned int address, unsigned int function, unsigned int error, unsigned char *output_data);

int superReader20(int offsetRegister, int fileNumber, int recordNumber, int recordLen);

/**************************************/
//разбор входного пакета USB
/**************************************/
void inputPacketParserUSB(void)
{
  pointInterface=0;//метка интерфейса 0-USB 1-RS485

  received_count = &usb_received_count;

  inputPacket = usb_received;
  outputPacket = outputPacket_USB;
  //Перевірка контрольної суми
  unsigned short CRC_sum;
  CRC_sum = 0xffff;
  for (int index = 0; index < (*received_count-2); index++) CRC_sum = AddCRC(*(inputPacket + index),CRC_sum);
  if((CRC_sum & 0xff)  != *(inputPacket+*received_count-2)) return;
  if ((CRC_sum >> 8  ) != *(inputPacket+*received_count-1)) return;

//  if(inputPacket[0]==0)//глоб адрес
//  if(inputPacket[1]==16)//номер ф-ции
//  if(((unsigned int)inputPacket[3] +256*(unsigned int)inputPacket[2])==14224)
//  if(((unsigned int)inputPacket[5] +256*(unsigned int)inputPacket[4])==7) upravlBroadcast=1;//флаг общей записи времени

//  if(!(inputPacket[0]==settings_fix_prt.address||upravlBroadcast==1)) return;

  if(inputPacket[0]!=current_settings.address) return;

  if(inputPacketParser()==0) return;

//  if(upravlBroadcast) return;//флаг общей записи времени

  usb_transmiting_count = sizeOutputPacket;
  for (int i = 0; i < usb_transmiting_count; i++) usb_transmiting[i] = outputPacket[i];
  data_usb_transmiting = true;

//флаг глобального сброса
//  if(globalResetFlag) restart_device = true;

}//inputPacketParserUSB(void)

int outputFunc20PacketEncoder(int adrUnit, int *fileNumberArray, int *recordNumberArray, int *recordLenArray, int cntitem)
{
//выходной кодировщик 20 функции
//int bazaRecord = (fileNumber-5)*10000;
//int number_record_of_log = bazaRecord + recordNumber;

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
//  outputPacket[idxOutputPacket] = (unsigned char)((2+recordLen[0]*2)&0xFF);
  idxOutputPacket++;
for(int item=0; item<cntitem; item++) {
//  int fileNumber = fileNumberArray[item];

      switch (fileNumberArray[item])
      {
      case 1:
      case 2:
      case 3:
      case 4:
        {
          //*************************************
          //Заголовок для аналогового реєстратора
          //*************************************
          //*************************************
          //Дані аналогового реєстратора
          //*************************************
        //Встановлюємо номер запису дискретного реєстратора для читання через інтерфейс USB
        number_record_of_ar_for_USB = recordNumberArray[item];
        //Подаємо команду читання дискретного реєстратора для інтерфейсу USB

        //Виставляємо першу частину запису
        //part_reading_dr_from_dataflash_for_USB = 0;
        //Подаємо команду зчитати дані у бувер пам'яті
        control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB;

          //Очікуємо поки завершиться зчитуквання даних для аналогового реєстратора
          while ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB  ) != 0
//                 ((type_interface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB  ) != 0)) ||
//                 ((type_interface == RS485_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485) != 0))
                )
          {
            //Якщ часом буде спрацьовувати Watchdog, то тут треба буде поставити функцію роботи з ним
          }//while

          break;
        }        
      case 5:
      case 6:
        {
          //*************************************
          //Заголовок для дискретного реєстратора
          //*************************************
          //*************************************
          //Дані 1 для дискретного реєстратора
          //*************************************
        //Встановлюємо номер запису дискретного реєстратора для читання через інтерфейс USB
        number_record_of_dr_for_USB = recordNumberArray[item];
        //Подаємо команду читання дискретного реєстратора для інтерфейсу USB

        //Виставляємо першу частину запису
        part_reading_dr_from_dataflash_for_USB = 0;
        //Подаємо команду зчитати дані у бувер пам'яті
        control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB;

          //Очікуємо поки завершиться зчитуквання даних для аналогового реєстратора
          while ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB  ) != 0
//                 ((type_interface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB  ) != 0)) ||
//                 ((type_interface == RS485_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485) != 0))
                )
          {
            //Якщ часом буде спрацьовувати Watchdog, то тут треба буде поставити функцію роботи з ним
          }//while
          break;
        }        
      default:
        {
          //Теоретично, суди програма ніколи не мала заходити
//          error = ERROR_ILLEGAL_DATA_ADDRESS;
          break;
        }
      }//switch

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
        case MARKER_OUTPERIMETR:
        //  dataRegister[idxDataRegister] = -1;
//          dataRegister[idxDataRegister] = 0;//(short) result;
          //break;
        return -1;
//        case MARKER_ERRORPERIMETR://ошибка периметра
          //dataRegister[idxDataRegister] = -2;
//       break;
  //        return Error_modbus_m(adrUnit, // address,
    //                          outputPacket[1],//function,
      //                        10,//error,
        //                      outputPacket);//output_data
        default:
        {

          dataRegister[idxDataRegister] = (short) result;
  //        flag = 0;
        }
        }//switch
//      qDebug()<<"result= "<<dataRegister[idxDataRegister];
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
int superReader20(int /*offsetRegister*/x, int fileNumber, int /*recordNumber*/x1, int /*recordLen*/x2)
{
  UNUSED(x);
  UNUSED(x1);
  UNUSED(x2);
      switch (fileNumber)
      {
      case 1:
        {
          //*************************************
          //Заголовок для аналогового реєстратора
          //*************************************
          //Максимальна кількість часових зрівів (ця змінна ще буде потрібна у визначення номеру останньої виборки при зчитувані конфігурації аналогового реєстратора)
//          int max_number_time_sample = (current_settings.prefault_number_periods + current_settings.postfault_number_periods) << VAGA_NUMBER_POINT_AR;
//          openFleshUnit();//открыть флеш

          break;
        }        
      case 2:
      case 3:
      case 4:
        {
          //*************************************
          //Дані аналогового реєстратора
          //*************************************
          break;
        }        
      case 5:
        {
          //*************************************
          //Заголовок для дискретного реєстратора
          //*************************************
          break;
        }        
      case 6:
        {
          //*************************************
          //Дані 1 для дискретного реєстратора
          //*************************************
          break;
        }        
      default:
        {
          //Теоретично, суди програма ніколи не мала заходити
//          error = ERROR_ILLEGAL_DATA_ADDRESS;
          break;
        }
      }//switch

  return 0xAA;//просто так
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
unsigned short int  AddCRC(unsigned char inpbyte, unsigned short int oldCRC)
{
  unsigned short int v,temp;
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

/******************************************************
//Функція, яка фіксує зміни у настройках і запускає запис настройок у EEPROM
-----------------------------------------------------
setting_rang: 0 - запис уставок
              1 - запис ранжування
              2 - ігнорувати цей параметр

source:       0 - мінімальні параметри
              1 - клавіатура
              2 - USB
              3 - RS-485
---------------------------------------------------
******************************************************/
/*
void fix_change_settings_m(unsigned int setting_rang, unsigned int source)
{
  if (setting_rang < 2)
  {
    unsigned char *point_to_target;
  
    if (setting_rang == 0) point_to_target = (&current_settings)->time_setpoints;
    else point_to_target = (&current_settings)->time_ranguvannja;
    
    //Записуємо час останніх змін
    unsigned char *label_to_time_array;
    if (copying_time == 2) label_to_time_array = time_copy;
    else label_to_time_array = time;
    for (unsigned int i = 0; i < 7; i++) *(point_to_target + i) = *(label_to_time_array + i);
    *(point_to_target + 7) = (unsigned char)(source & 0xff);
  }
  
  //Запускаємо запис у EEPROM
  _SET_BIT(control_i2c_taskes, TASK_START_WRITE_SETTINGS_EEPROM_BIT);
  
}//fix_change_settings_m(unsigned int setting_rang, unsigned int source)
*/
