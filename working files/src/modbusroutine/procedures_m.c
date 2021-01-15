#include "header.h"

extern unsigned char  *outputPacket;
#if (MODYFIKACIA_VERSII_PZ >= 10)
extern unsigned char  outputPacket_TCP[300];
#endif
extern unsigned char  outputPacket_USB[300];
extern unsigned char  outputPacket_RS485[300];

extern int sizeOutputPacket;
extern unsigned char *inputPacket;
extern int *received_count;
extern int globalcntBit;//�-�� ���
extern int globalcntReg;//�-�� ���������
extern int globalbeginAdrReg;//����� ��� ��������
extern int globalbeginAdrBit;//����� ��� ���

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

//��������������� ������ - 16 ASCII �������
#if (MODYFIKACIA_VERSII_PZ >= 10)
const char idetyficator_rang[MAX_NAMBER_LANGUAGE][NUMBER_TOTAL_SIGNAL_FOR_RANG + (1 - N_IN_GOOSE)  + (1 - N_IN_MMS) + (1 - N_OUT_LAN) + (3 - NUMBER_UP_SIGNAL_FOR_RANG)][16] =
#else
const char idetyficator_rang[MAX_NAMBER_LANGUAGE][NUMBER_TOTAL_SIGNAL_FOR_RANG + (3 - NUMBER_UP_SIGNAL_FOR_RANG)][16] =
#endif
{
  {NAME_RANG_RU},
  {NAME_RANG_UA},
  {NAME_RANG_EN},
  {NAME_RANG_KZ},
};


#if (MODYFIKACIA_VERSII_PZ >= 10)
/**************************************/
//������ �������� ������ TCP
/**************************************/
void inputPacketParserLAN(void)
{
//������ �������� TCP  
//#define TCP_PREFIXSIZE 6  
  pointInterface=LAN_RECUEST;//����� ���������� 0-USB 1-RS485

  received_count = &LAN_received_count;

  outputPacket = outputPacket_TCP;
  inputPacket = LAN_received;
//                +TCP_PREFIXSIZE;//������ ������� TCP
//  *received_count -= TCP_PREFIXSIZE;//������ ������� TCP
  
  if((*received_count)<0)
  {
    LAN_received_count = 0;//�������� ����
    return;//���-�� ����� �� ���
  }//if
  
  *received_count += 2;//������������ CRC
  //�������� ���������� ����
//  unsigned short CRC_sum;
//  CRC_sum = 0xffff;
//  for (int index = 0; index < (*received_count-2); index++) CRC_sum = AddCRC(*(inputPacket + index),CRC_sum);
//  if((CRC_sum & 0xff)  != *(inputPacket+*received_count-2)) return;
//  if ((CRC_sum >> 8  ) != *(inputPacket+*received_count-1)) return;

//  if(inputPacket[0]!=current_settings.address) return;

  if(inputPacketParser()==0)
  {
   LAN_received_count = 0;//�������� ����
   return;
  }//if

  sizeOutputPacket -= 2;//������ CRC

  LAN_transmiting_count = sizeOutputPacket;
//  for (int i = 0; i < TCP_PREFIXSIZE; i++) LAN_transmiting[i] = 0;//�������� ������� TCP
//  LAN_transmiting[1] = 1;//������� TCP
//  LAN_transmiting[5] = sizeOutputPacket;//������� TCP
  for (int i = 0; i < LAN_transmiting_count; i++) LAN_transmiting[i/*+ TCP_PREFIXSIZE*/] = outputPacket[i];
//  LAN_transmiting_count += TCP_PREFIXSIZE;//�������� ������� TCP
  LAN_received_count = 0;//�������� ����
  _SET_STATE (queue_mo, STATE_QUEUE_MO_SEND_MODBUS_TCP_RESP);//��������� ���������
}//inputPacketParserLAN(void)
#endif  
/**************************************/
//������ �������� ������ USB
/**************************************/
void inputPacketParserUSB(void)
{
  pointInterface=USB_RECUEST;//����� ���������� 0-USB 1-RS485

  received_count = &usb_received_count;

  inputPacket = usb_received;
  outputPacket = outputPacket_USB;
  //�������� ���������� ����
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
//������ �������� ������ RS485
/**************************************/
void inputPacketParserRS485(void)
{
  pointInterface=RS485_RECUEST;//����� ���������� 0-USB 1-RS485

  inputPacket = RxBuffer_RS485;

  received_count = &RxBuffer_RS485_count;
  outputPacket = outputPacket_RS485;

  //�������� ���������� ����
  unsigned short CRC_sum;
  CRC_sum = 0xffff;
  for (int index = 0; index < (*received_count-2); index++) CRC_sum = AddCRC(*(inputPacket + index),CRC_sum);
  if((CRC_sum & 0xff)  != *(inputPacket+*received_count-2) ||//) return;
      (CRC_sum >> 8  ) != *(inputPacket+*received_count-1))
    {
      /***
      12345
      ������� �������� (�� ������� ���������� ����)
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
      ������� �������� (�� ������� ������ 2)
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
      ������� �������� (����� ������ ������ ���������� ������)
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
//�������� ���������� 20 �������
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

  for(int item=0; item<cntitem; item++)  //����� fileNumber, recordNumber, recordLen
    {
     int tmp1 = openRegistrator(fileNumberArray[item]);//������� ������ ������������ AR DR
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
//����������� �������� 20-� �������
/**************************************/
int superReader20(int offsetRegister, int fileNumber, int recordNumber, int recordLen)
{
  switch (fileNumber)
    {
/*
    case 1://������������ ����������� ������������
      return configAnalogRegistrator(offsetRegister, recordNumber, recordLen);


    case 2://������ ����������� ������������ 1�����
    case 3://������ ����������� ������������ 2�����
    case 4://������ ����������� ������������ 3�����
      if (fileNumber == 3) recordNumber += 10000;
      else if (fileNumber == 4) recordNumber += 20000;

      return dataAnalogRegistrator(offsetRegister, recordNumber, recordLen);

    case 5://������������ ����������� ������������
      return configDiskretRegistrator(offsetRegister, recordNumber, recordLen);

    case 6://������ ����������� ������������ 1�����
      return dataDiskretRegistrator(offsetRegister, recordNumber, recordLen);
*/

    case 1://������������ ����������� ������������
      return configDiskretRegistrator(offsetRegister, recordNumber, recordLen);

    case 2://������ ����������� ������������ 1�����
    case 3://������ ����������� ������������ 2�����
      if (fileNumber == 3) recordNumber += 10000;
      return dataDiskretRegistrator(offsetRegister, recordNumber, recordLen);

    case 4://������������ ����������� ������������
      return configAnalogRegistrator(offsetRegister, recordNumber, recordLen);

    default:
      {
      //������ ����������� ������������ 
      if (fileNumber == 0) break;
      if (fileNumber > 5) recordNumber += (fileNumber - 5)*10000;
      return dataAnalogRegistrator(offsetRegister, recordNumber, recordLen);
      }//default
    }//switch

  return MARKER_ERRORPERIMETR;
}//superReader20(int fileNumber, int offsetRegister)

/***********************************************************************************/
//������� ������ �� ��������� Modbus-RTU �� �������
/***********************************************************************************/
int Error_modbus_m(unsigned int address, unsigned int function, unsigned int error, unsigned char *output_data)
{
  output_data[0] = address & 0xff;
  output_data[1] = 0x80 | (function & 0xff);
  output_data[2] = error & 0xff;
  return 3;
}

/***********************************************************************************/
//��������� ����� �� CRC ����
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
//������� ������ ������������ AR DR
/***********************************************************************************/
int openRegistrator(int number_file)
{
//����� AR DR
  if (
    (
      (number_file >= 4) &&
      (number_file <= 7) &&
      (
        ((pointInterface == USB_RECUEST  ) && (number_record_of_ar_for_USB   == 0xffff))
        ||
        ((pointInterface == RS485_RECUEST  ) && (number_record_of_ar_for_RS485 == 0xffff))
#if (MODYFIKACIA_VERSII_PZ >= 10)
        ||
        ((pointInterface == LAN_RECUEST  ) && (number_record_of_ar_for_LAN == 0xffff))
#endif
      )
    )
    ||
    (
      (number_file >= 1) &&
      (number_file <= 2) &&
      (
        ((pointInterface == USB_RECUEST  ) && (number_record_of_dr_for_USB   == 0xffff)) 
        ||
        ((pointInterface == RS485_RECUEST  ) && (number_record_of_dr_for_RS485 == 0xffff))
#if (MODYFIKACIA_VERSII_PZ >= 10)
        ||
        ((pointInterface == LAN_RECUEST  ) && (number_record_of_dr_for_LAN == 0xffff))
#endif
      )
    )
  )
    {
      //������� ����� �����, ��� �� ������ ������� ����������� ���������� ������
      return -1;
    }
  else if (
    (number_file >= 4) &&
    (number_file <= 7) &&
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
      //����� ��� �������� ������/�������� ��� ���������� ����������, ��� ���� ������� ����� (������������ ����� ��������� 20 �), ���� ������� ����������� ���������� � ��������� ����������
      //error = ERROR_SLAVE_DEVICE_BUSY;
      return -2;
      /*
      ��� �� �� ���������� ����� �� ����������� �� TASK_MAMORY_READ_DATAFLASH_FOR_AR_INTERFACE,
      ��, �������, ������ � ����� � ����� ���� ����� ��������� �� �������, ��
      ��������� ������������ ���������� �� ������� �������. ����� ��������� ������� � �����, �� �����
      ���������� � ��������� dataFlash
      */
    }
  else if (
    (number_file >= 1) &&
    (number_file <= 2) &&
    (
      (
        ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB  ) != 0)) 
        ||
        ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485) != 0))
#if (MODYFIKACIA_VERSII_PZ >= 10)
        ||
        ((pointInterface == LAN_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_LAN) != 0))
#endif
      )
      ||
      ((clean_rejestrators & CLEAN_DR) != 0)
    )
  )
    {
      //����� ��� ���������� ��� ���������� ������ ����������� ����������, ���� �� �������� � ��������� �����������
      //error = ERROR_SLAVE_DEVICE_BUSY;
      return -2;
    }//if

//������ AR
  if((number_file >= 4) && (number_file <= 7))//
    {
      //������� ���� ����������� ����������� ����� ��� ����������� ����������
      while (
        ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB  ) != 0)) 
        ||
        ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485) != 0))
#if (MODYFIKACIA_VERSII_PZ >= 10)
        ||
        ((pointInterface == LAN_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_LAN) != 0))
#endif
      )
        {
          //��� ������ ���� ������������� Watchdog, �� ��� ����� ���� ��������� ������� ������ � ���
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
//������ DR
  else
    {
      while (
        ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB  ) != 0)) 
        ||
        ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485) != 0))
#if (MODYFIKACIA_VERSII_PZ >= 10)
        ||
        ((pointInterface == LAN_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_LAN) != 0))
#endif
      )
        {
          //��� ������ ���� ������������� Watchdog, �� ��� ����� ���� ��������� ������� ������ � ���
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
//������ ����������� ������������
  //����������� ������� ������� ����
  int max_number_time_sample = (current_settings.prefault_number_periods + current_settings.postfault_number_periods) << VAGA_NUMBER_POINT_AR;
  unsigned int lenMax = (3 + NUMBER_ANALOG_CANALES + number_word_digital_part_ar);
  if ((unsigned int)recordLen > lenMax) return MARKER_ERRORPERIMETR;//���� ���� ����������

  if(recordNumber>max_number_time_sample) return MARKER_ERRORPERIMETR;//���� ���� ����������


  int *point_to_first_number_time_sample, *point_to_last_number_time_sample;

  if (pointInterface == USB_RECUEST)
    {
      point_to_first_number_time_sample = &first_number_time_sample_for_USB;
      point_to_last_number_time_sample  = &last_number_time_sample_for_USB;
    }
  else if (pointInterface == RS485_RECUEST)
    {
      point_to_first_number_time_sample = &first_number_time_sample_for_RS485;
      point_to_last_number_time_sample  = &last_number_time_sample_for_RS485;
    }
#if (MODYFIKACIA_VERSII_PZ >= 10)
  else if (pointInterface == LAN_RECUEST)
    {
      point_to_first_number_time_sample = &first_number_time_sample_for_LAN;
      point_to_last_number_time_sample  = &last_number_time_sample_for_LAN;
    }
#endif  
  else
  {
    //���������� ����� ������ �� ���� � ����
    total_error_sw_fixed(191);
  }

  //���������� �� ������� ��������� ����������� ����������
  if (
    (((int)recordNumber) < (*point_to_first_number_time_sample)) ||
    (((int)recordNumber) > (*point_to_last_number_time_sample ))
  )
    {
      //����������� ������� ��� �� � ��������

      //����������� ������� ��� ��������� ������ ������ ������ � ������������� ������ ���� � ������, ������ �������, ������� ����
      *point_to_first_number_time_sample = recordNumber;
      int last_number_time_sample_tmp = recordNumber + SIZE_PAGE_DATAFLASH_2/((NUMBER_ANALOG_CANALES + number_word_digital_part_ar)*sizeof(short int));
      if (last_number_time_sample_tmp <= max_number_time_sample)
        {
          *point_to_last_number_time_sample = last_number_time_sample_tmp - 1;//����� ���������� �������� ���� �������
        }
      else
        {
          *point_to_last_number_time_sample = max_number_time_sample - 1;
        }

      //������ ������� ������� ���� � ����� ���'��
      if (pointInterface == USB_RECUEST)
        control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB;
      else if (pointInterface == RS485_RECUEST)
        control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485;
#if (MODYFIKACIA_VERSII_PZ >= 10)
      else if (pointInterface == LAN_RECUEST)
        control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_AR_LAN;
#endif  
      else
      {
        //���������� ����� ������ �� ���� � ����
        total_error_sw_fixed(199);
      }

      //������� ���� ����������� ����������� ����� ��� ����������� ����������
      while (
        ((pointInterface == USB_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB  ) != 0)) 
        ||
        ((pointInterface == RS485_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485) != 0))
#if (MODYFIKACIA_VERSII_PZ >= 10)
        ||
        ((pointInterface == LAN_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_LAN) != 0))
#endif
      )
        {
          //���� ������ ���� ������������� Watchdog, �� ��� ����� ���� ��������� ������� ������ � ���
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

  //���� �� ���� �����, �� ������� �� ���������� ������� ������� � ����������� � ����� ������� ����������� ���������� ��� ����������

  //��������� ������ � ����� ������� � ����� ��������� �������� ��� ������� ���
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
    case 0://���������� ����� �������
      return recordNumber + 1;
    case 1://����� ������� (ms)
      return recordNumber & 0xffff;
    case 2://����� ������� (ms)
      return recordNumber >> 16;
    }//switch

  unsigned char *point_to_buffer;
  if (pointInterface == USB_RECUEST)
    point_to_buffer = buffer_for_USB_read_record_ar;
  else if (pointInterface == RS485_RECUEST)
    point_to_buffer = buffer_for_RS485_read_record_ar;
#if (MODYFIKACIA_VERSII_PZ >= 10)
  else if (pointInterface == LAN_RECUEST)
    point_to_buffer = buffer_for_LAN_read_record_ar;
#endif  
  else
  {
    //���������� ����� ������ �� ���� � ����
    total_error_sw_fixed(194);
  }

  return (*(point_to_buffer + index_time_sample +2*(offsetRegister-3))) + ((*(point_to_buffer +
         index_time_sample + 1 +2*(offsetRegister-3))) << 8);
}//dataAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen)

int configAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen)
{
//������������ ����������� ������������

  switch(recordNumber)
    {
    case 0://��� ������� recordNumber
    case 1://���������� ������� recordNumber
      return recordNumberCase0Case1(offsetRegister, recordNumber, recordLen, ANALOG_REGISTRATOR);
    }//switch

//���������� ������
  if(recordNumber>=2 && recordNumber<=(2+NUMBER_ANALOG_CANALES-1))
    {
      if(recordLen>27) return MARKER_ERRORPERIMETR;
      int subObj = recordNumber-2;//������ ������
      __HEADER_AR *header_ar_tmp;

      if (pointInterface==USB_RECUEST)//����� ���������� 0-USB 1-RS485
        header_ar_tmp = (__HEADER_AR*)buffer_for_USB_read_record_ar;
      else if (pointInterface==RS485_RECUEST)
        header_ar_tmp = (__HEADER_AR*)buffer_for_RS485_read_record_ar;
#if (MODYFIKACIA_VERSII_PZ >= 10)
      else if (pointInterface==LAN_RECUEST)
        header_ar_tmp = (__HEADER_AR*)buffer_for_LAN_read_record_ar;
#endif  
      else
      {
        //���������� ����� ������ �� ���� � ����
        total_error_sw_fixed(195);
      }

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
        case 0://���������� ����� ����������� ������ offsetRegister
          return 1+subObj;
        case 1://������������� ������0 offsetRegister
          return idetyficator[subObj][0] | (idetyficator[subObj][1]<<8);
        case 2://������������� ������1 offsetRegister
          return idetyficator[subObj][2] | (idetyficator[subObj][3]<<8);
        case 3://������������� ������2 offsetRegister
          return idetyficator[subObj][4] | (idetyficator[subObj][5]<<8);
        case 4://������������� ������3 offsetRegister
          return idetyficator[subObj][6] | (idetyficator[subObj][7]<<8);
        case 5://������������� ������4 offsetRegister
          return idetyficator[subObj][8] | (idetyficator[subObj][9]<<8);
        case 6://������������� ������5 offsetRegister
          return idetyficator[subObj][10] | (idetyficator[subObj][11]<<8);
        case 7://������������� ������6 offsetRegister
          return idetyficator[subObj][12] | (idetyficator[subObj][13]<<8);
        case 8://������������� ������7 offsetRegister
          return idetyficator[subObj][14] | (idetyficator[subObj][15]<<8);
        case 9://���� ������ offsetRegister
        {
          //���� ������ - 2 ASCII �������
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
        case 10://����������� ��������� ����0 offsetRegister
        case 11://����������� ��������� ����1 offsetRegister
        case 12://����������� ��������� ����2 offsetRegister
        case 13://����������� ��������� ����3 offsetRegister
        case 14://����������� ��������� ����4 offsetRegister
        case 15://����������� ��������� ����5 offsetRegister
        case 16://����������� ��������� ����6 offsetRegister
        case 17://����������� ��������� ����7 offsetRegister
        {
          //������������ ������� � ��� - 16 ASCII ������� - ������ ��� �� ��������, ���� � �� ���� ������� ' '
          return ((' ')<<8) | (' ');
        }//case 10-17
        case 18://������� ��������� offsetRegister
        {
          //������� ���������� - 2 ASCII �������
          const char label_meas[2][2] = {"mA","mV"};
          unsigned int index;
          if (subObj <= I_Ic)index = 0;
          else index = 1;

          return label_meas[index][0] | (label_meas[index][1]<<8);
        }//case 18
        case 19://����������� ������ offsetRegister
        {
          //���������� ������
          if      (subObj == I_3I0) return (1000*MNOGNYK_3I0) >> (VAGA_DILENNJA_3I0 + 8);
          else if (subObj <= I_Ic ) return (1000*MNOGNYK_I)   >> (VAGA_DILENNJA_I   + 4);
          else return (1000*MNOGNYK_U)   >> (VAGA_DILENNJA_U   + 4);
        }//case 19
        case 20://���������� �������� offsetRegister
          //��������� �������
          return 0;
        case 21://����� ������� ������ (�s) offsetRegister
          //���� � ���
          return 0; //��� ����� ������� ������� ����������
        case 22://������ ������ �������� offsetRegister
          //����� �������
          return -32767;
        case 23://������� ������ �������� offsetRegister
          //������ �������
          return 32767;
        case 24://��������� ����. ������. offsetRegister
        {
          //��������� ���������� �������������
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
        case 25://��������� ����. ������. offsetRegister
          //��������� ���������� �������������
          return 1;
        case 26://��������� ��� ��������� ������������� offsetRegister
          //���� ������ ������������ � �������� �������
          return 'S';
        }//switch
      return MARKER_ERRORPERIMETR;
    }//if(recordNumber>=2 && recordNumber<=(2+NUMBER_ANALOG_CANALES))

//���������� ������
  if(recordNumber>=((2+NUMBER_ANALOG_CANALES)) && recordNumber<=(((2+NUMBER_ANALOG_CANALES)) +DISKRET_TOTAL)-1)
    {
      if(recordLen>19) return MARKER_ERRORPERIMETR;
      int subObj = recordNumber-((2+NUMBER_ANALOG_CANALES));//������ ������
      return recordNumberCaseDiskret(subObj, offsetRegister);

    }//if(recordNumber>=(2+NUMBER_ANALOG_CANALES +1) && recordNumber<=(2+NUMBER_ANALOG_CANALES +1 +100))

//���������
  if(recordNumber>=(((2+NUMBER_ANALOG_CANALES)) +DISKRET_TOTAL) && recordNumber<=((((2+NUMBER_ANALOG_CANALES)) +DISKRET_TOTAL) +7)-1)
    {
      int subObj = recordNumber-(((2+NUMBER_ANALOG_CANALES)) +DISKRET_TOTAL);//������ ������
      return recordNumberCaseOther(subObj, offsetRegister, recordLen, ANALOG_REGISTRATOR);

    }//if(recordNumber>=(2+NUMBER_ANALOG_CANALES +1 +DISKRET_TOTAL +1) && recordNumber<=(2+NUMBER_ANALOG_CANALES +1 +DISKRET_TOTAL +1 +7))

  return MARKER_ERRORPERIMETR;
}//configAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen)

int configDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen)
{
//������������ ����������� ������������
  switch(recordNumber)
    {
    case 0://��� ������� recordNumber
    case 1://���������� ������� recordNumber
      return recordNumberCase0Case1(offsetRegister, recordNumber, recordLen, DISKRET_REGISTRATOR);
    }//switch

//���������� ������
  if(recordNumber>=2 && recordNumber<=(2+DISKRET_TOTAL)-1)
    {
      if(recordLen>19) return MARKER_ERRORPERIMETR;
      int subObj = recordNumber-2;//������ ������
      return recordNumberCaseDiskret(subObj, offsetRegister);

    }//if(recordNumber>=2 && recordNumber<=(2+DISKRET_TOTAL)-1)

//���������
  if(recordNumber>=((2+DISKRET_TOTAL)) && recordNumber<=(((2+DISKRET_TOTAL)) +7)-1)
    {
      int subObj = recordNumber-((2+DISKRET_TOTAL));//������ ������
      return recordNumberCaseOther(subObj, offsetRegister, recordLen, DISKRET_REGISTRATOR);

    }//if(recordNumber>=((2+DISKRET_TOTAL)) && recordNumber<=(((2+DISKRET_TOTAL)) +7)-1)

  return MARKER_ERRORPERIMETR;
}//configDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen)

int recordNumberCase0Case1(int offsetRegister, int recordNumber, int recordLen, int registrator)
{
  //���� �� ���� �����, �� ������� �� ��������� ����������� ���������� ��� ��������
  __HEADER_AR *header_ar_tmp;

  if (pointInterface==USB_RECUEST)//����� ���������� 0-USB 1-RS485
    header_ar_tmp = (__HEADER_AR*)buffer_for_USB_read_record_ar;
  else if (pointInterface==RS485_RECUEST)
    header_ar_tmp = (__HEADER_AR*)buffer_for_RS485_read_record_ar;
#if (MODYFIKACIA_VERSII_PZ >= 10)
  else if (pointInterface==LAN_RECUEST)
    header_ar_tmp = (__HEADER_AR*)buffer_for_LAN_read_record_ar;
#endif  
  else
  {
    //���������� ����� ������ �� ���� � ����
    total_error_sw_fixed(196);
  }
  
  switch(recordNumber)
    {
    case 0://��� ������� recordNumber
    {

      if(recordLen>10) return MARKER_ERRORPERIMETR;
      if(registrator==ANALOG_REGISTRATOR)
        {
          switch(offsetRegister)
            {
            case 0://��� ������� offsetRegister
              return header_ar_tmp->name_of_cell[ 0] | (header_ar_tmp->name_of_cell[ 1]<<8);
            case 1://��� ������� offsetRegister
              return header_ar_tmp->name_of_cell[ 2] | (header_ar_tmp->name_of_cell[ 3]<<8);
            case 2://��� ������� offsetRegister
              return header_ar_tmp->name_of_cell[ 4] | (header_ar_tmp->name_of_cell[ 5]<<8);
            case 3://��� ������� offsetRegister
              return header_ar_tmp->name_of_cell[ 6] | (header_ar_tmp->name_of_cell[ 7]<<8);
            case 4://��� ������� offsetRegister
              return header_ar_tmp->name_of_cell[ 8] | (header_ar_tmp->name_of_cell[ 9]<<8);
            case 5://��� ������� offsetRegister
              return header_ar_tmp->name_of_cell[ 10] | (header_ar_tmp->name_of_cell[ 11]<<8);
            case 6://��� ������� offsetRegister
              return header_ar_tmp->name_of_cell[ 12] | (header_ar_tmp->name_of_cell[ 13]<<8);
            case 7://��� ������� offsetRegister
              return header_ar_tmp->name_of_cell[ 14] | (header_ar_tmp->name_of_cell[ 15]<<8);
            case 8://�������. ����� ���������� offsetRegister
              //������� ����� �������� ������ ���� � ��� ����� (�������������� �� �����-�����������), ���� � ���� �� ������� ��� ������ � DataFlash � ����� ��������� ����������� ����������
              return serial_number_dev & 0xffff;
            case 9://��� �������� ��������� (1999) offsetRegister
              return 1999;

            }//switch
        }//if(registrator==ANALOG_REGISTRATOR)
      else
        {
          //if(registrator==DISKRET_REGISTRATOR)
          unsigned char *point_to_buffer;
          if (pointInterface==USB_RECUEST)//����� ���������� 0-USB 1-RS485
            point_to_buffer = buffer_for_USB_read_record_dr;
          else if (pointInterface==RS485_RECUEST) point_to_buffer = buffer_for_RS485_read_record_dr;
#if (MODYFIKACIA_VERSII_PZ >= 10)
          else if (pointInterface==LAN_RECUEST) point_to_buffer = buffer_for_LAN_read_record_dr;
#endif  
          else
          {
            //���������� ����� ������ �� ���� � ����
            total_error_sw_fixed(204);
          }
  
          if (offsetRegister < 8)
            {
              unsigned int index_cell;

              index_cell =  (offsetRegister - 0)<<1;
              return (*(point_to_buffer + FIRST_INDEX_NAME_OF_CELL_DR + index_cell)) | ((*(point_to_buffer + FIRST_INDEX_NAME_OF_CELL_DR + index_cell + 1))<<8);
            }
          else if (offsetRegister == 8)
            {
              //������� ����� �������� ������ ���� � ��� ����� (�������������� �� �����-�����������), ���� � ���� �� ������� ��� ������ � DataFlash � ����� ��������� ����������� ����������
              return serial_number_dev & 0xffff;
            }
          else
            {
              return 1999;
            }
        }//else if(registrator==DISKRET_REGISTRATOR)
    }//case 0
    break;

    case 1://���������� ������� recordNumber
    {
      if(recordLen>3) return MARKER_ERRORPERIMETR;
      if(registrator==ANALOG_REGISTRATOR)
        {
          switch(offsetRegister)
            {
            case 0://���������� ������� ����� offsetRegister
              return NUMBER_ANALOG_CANALES + NUMBER_TOTAL_SIGNAL_FOR_RANG;
            case 1://���������� ���������� ������� offsetRegister
              return NUMBER_ANALOG_CANALES;
            case 2://���������� ���������� ������� offsetRegister
              return NUMBER_TOTAL_SIGNAL_FOR_RANG;
            }//switch
          return MARKER_ERRORPERIMETR;
        }// if(registrator==ANALOG_REGISTRATOR)
      else
        {
          //if(registrator==DISKRET_REGISTRATOR)
          switch(offsetRegister)
            {
            case 0://���������� ������� ����� offsetRegister
              return   0 + NUMBER_TOTAL_SIGNAL_FOR_RANG;
            case 1://���������� ���������� ������� offsetRegister
              return 0;
            case 2://���������� ���������� ������� offsetRegister
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
    case 0://���������� ����� ����������� ������ offsetRegister
      return 1+subObj;
    case 1://������������� ������0 offsetRegister
    case 2://������������� ������1 offsetRegister
    case 3://������������� ������2 offsetRegister
    case 4://������������� ������3 offsetRegister
    case 5://������������� ������4 offsetRegister
    case 6://������������� ������5 offsetRegister
    case 7://������������� ������6 offsetRegister
    case 8://������������� ������7 offsetRegister
    {
//      const uint32_t index_number_UP[MAX_NAMBER_LANGUAGE][3] =
//      {
//        {11, 10, 8},
//        {11, 10, 8},
//        {12,  7, 8},
//        {11, 10, 8},
//      };
//
      int index_language = index_language_in_array(current_settings.language);

      unsigned int index_cell =  (offsetRegister - 1)<<1;
      unsigned int temp_data = 0;
      for (size_t sym = 0; sym < 2; sym++)
        {
          size_t index_row;
          if (subObj < NUMBER_GENERAL_SIGNAL_FOR_RANG) 
          {
#if (MODYFIKACIA_VERSII_PZ >= 10)
            if (subObj < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) 
            {
              index_row = subObj;
            }
            else if (subObj < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) 
            {
              index_row = RANG_BLOCK_IN_GOOSE1 + ((subObj - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) % 1);
            }
            else if (subObj < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) 
            {
              index_row = RANG_BLOCK_IN_MMS1 + (1 - N_IN_GOOSE) + ((subObj - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) % 1);
            }
            else
            {
              index_row = RANG_BLOCK_OUT_LAN1 + (1 - N_IN_GOOSE) + (1 - N_IN_MMS) + ((subObj - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) % 1);
            }
#else
            index_row = subObj;
#endif        
          }
          else if (subObj < RANG_BLOCK_UP1) 
          {
            index_row = subObj
#if (MODYFIKACIA_VERSII_PZ >= 10)
                         + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif        
                         ;
          }
          else if (subObj < (RANG_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG))
          {
            index_row = RANG_BLOCK_UP1
#if (MODYFIKACIA_VERSII_PZ >= 10)
                         + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
                         + ((subObj - RANG_BLOCK_UP1) % 3);
          }
          else
          {
            index_row = subObj 
#if (MODYFIKACIA_VERSII_PZ >= 10)
                        + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif        
                        + 3 - NUMBER_UP_SIGNAL_FOR_RANG;
          }

#if (MODYFIKACIA_VERSII_PZ >= 10)
          if (
              (subObj >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN)))  &&
              (subObj <  (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) &&
              ((index_cell + sym) >=  index_n_In_GOOSE[index_language][(subObj - RANG_BLOCK_IN_GOOSE1) % 1]) &&
              ((index_cell + sym) <= (index_n_In_GOOSE[index_language][(subObj - RANG_BLOCK_IN_GOOSE1) % 1] + 1)) 
             )   
          {
            unsigned int n = subObj - RANG_BLOCK_IN_GOOSE1;
            if ((n + 1) < 10)
            {
              if ((index_cell + sym) == index_n_In_GOOSE[index_language][n % 1])
                temp_data |= (0x30 + (n + 1)) << (8*sym);
              else
                temp_data |= (' ') << (8*sym);
            }
            else
            {
              if ((index_cell + sym) == index_n_In_GOOSE[index_language][n % 1])
                temp_data |= (0x30 + (n / 1 + 1) / 10) << (8*sym);
              else
                temp_data |= (0x30 + (n / 1 + 1) % 10) << (8*sym);
            }
          }
          else if (
                   (subObj >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN)))  &&
                   (subObj <  (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) &&
                   ((index_cell + sym) == index_n_In_MMS[index_language][(subObj - RANG_BLOCK_IN_MMS1) % 1]) 
                  )   
          {
            temp_data |= (0x30 + ((subObj - RANG_BLOCK_IN_MMS1) / 1 + 1)) << (8*sym);
          }
          else if (
                   (subObj >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN)))  &&
                   (subObj <  (NUMBER_GENERAL_SIGNAL_FOR_RANG)) &&
                   ((index_cell + sym) == index_n_Out_LAN[index_language][(subObj - RANG_BLOCK_OUT_LAN1) % 1]) 
                  )   
          {
            temp_data |= (0x30 + ((subObj - RANG_BLOCK_OUT_LAN1) / 1 + 1)) << (8*sym);
          }
          else 
#endif
          {
            if (
                (subObj >=  RANG_BLOCK_UP1)  &&
                (subObj <  (RANG_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG)) &&
                ((index_cell + sym) == index_number_UP[index_language][(subObj - RANG_BLOCK_UP1) % 3]) 
               )   
            {
              temp_data |= (0x30 + ((subObj - RANG_BLOCK_UP1) / 3 + 1)) << (8*sym);
            }
            else temp_data |= (idetyficator_rang[index_language][index_row][(index_cell + sym)]) << (8*sym);
          }
        }
      return temp_data;
    }//case 1-8
    case 9://���� ������ offsetRegister
      //���� ������ - 2 ASCII ������� - ������ �� ��������
      return (' '<<8) | ' ';
    case 10://����������� ��������� ����0 offsetRegister
    case 11://����������� ��������� ����1 offsetRegister
    case 12://����������� ��������� ����2 offsetRegister
    case 13://����������� ��������� ����3 offsetRegister
    case 14://����������� ��������� ����4 offsetRegister
    case 15://����������� ��������� ����5 offsetRegister
    case 16://����������� ��������� ����6 offsetRegister
    case 17://����������� ��������� ����7 offsetRegister
    {
      //������������ ������� � ��� - 16 ASCII �������- ������ �� ��������
      return (' '<<8) | ' ';
    }//case 10-18
    case 18://���������� ��������� offsetRegister
      //���������� ����
      return 0;
    }//switch
  return MARKER_ERRORPERIMETR;
}//recordNumberCaseDiskret(int subObj, int offsetRegister, int recordLen)

int recordNumberCaseOther(int subObj, int offsetRegister, int recordLen, int registrator)
{
  __HEADER_AR *header_ar_tmp;
  unsigned char *point_to_buffer;
  if (pointInterface == USB_RECUEST) //����� ���������� 0-USB 1-RS485
    point_to_buffer = buffer_for_USB_read_record_dr;
  else if (pointInterface == RS485_RECUEST)
    point_to_buffer = buffer_for_RS485_read_record_dr;
#if (MODYFIKACIA_VERSII_PZ >= 10)
  else if (pointInterface == LAN_RECUEST)
    point_to_buffer = buffer_for_LAN_read_record_dr;
#endif  
  else
  {
    //���������� ����� ������ �� ���� � ����
    total_error_sw_fixed(205);
  }

  int max_number_time_sample = (current_settings.prefault_number_periods + current_settings.postfault_number_periods) << VAGA_NUMBER_POINT_AR;

  if (pointInterface==USB_RECUEST) //����� ���������� 0-USB 1-RS485
    {
      header_ar_tmp = (__HEADER_AR*)buffer_for_USB_read_record_ar;
    }//if
  else if (pointInterface==RS485_RECUEST)
    {
      header_ar_tmp = (__HEADER_AR*)buffer_for_RS485_read_record_ar;
    }
#if (MODYFIKACIA_VERSII_PZ >= 10)
  else if (pointInterface==LAN_RECUEST)
    {
      header_ar_tmp = (__HEADER_AR*)buffer_for_LAN_read_record_ar;
    }
#endif  
  else
  {
    //���������� ����� ������ �� ���� � ����
    total_error_sw_fixed(197);
  }

  switch(subObj)
    {
    case 0://������� ����� (��) subObj
      if(recordLen>1) return MARKER_ERRORPERIMETR;
      return 5000;
    case 1://���-�� ������ ������������� subObj
      if(recordLen>1) return MARKER_ERRORPERIMETR;
      if(registrator==ANALOG_REGISTRATOR) return 1;
      else return 0;
    case 2://������� �������1 (��) subObj
    {
      if(recordLen>2) return MARKER_ERRORPERIMETR;
      switch(offsetRegister)
        {
        case 0://������� �������1 (��) offsetRegister
          if(registrator==ANALOG_REGISTRATOR) return ((50*1000)<<VAGA_NUMBER_POINT_AR)>>4; //��c����
          return 0;
        case 1://��������� ������� �� ���� ������� offsetRegister
          if(registrator==ANALOG_REGISTRATOR) return max_number_time_sample; //������� �������
          return *(point_to_buffer + FIRST_INDEX_NUMBER_ITEMS_DR); //������� ������� �� ����� ����� ������������� ���� ��������
        }//switch

      return MARKER_ERRORPERIMETR;
    }//case 2
    case 3://���� (���� � ����� ������ ������) subObj
    case 4://���� (���� � ����� �������) subObj
    {
      if(recordLen>7) return MARKER_ERRORPERIMETR;
      switch(offsetRegister)
        {
        case 0://���� (���� � ����� ������ ������) offsetRegister
        case 1://����� offsetRegister
        case 2://��� offsetRegister
        case 3://���� offsetRegister
        case 4://������ offsetRegister
        case 5://������� offsetRegister
        case 6://������������ offsetRegister
        {
          if(registrator==ANALOG_REGISTRATOR)
            {
              time_t time_dat_tmp = header_ar_tmp->time_dat;
              int32_t time_ms_tmp = header_ar_tmp->time_ms;

              struct tm *p;
              int temp_data;
              int max_time_milliseconds_before = (current_settings.prefault_number_periods)*20;
              int s, ds_ms;

              if(subObj==4)  goto m1;// return time_avar_analog[offsetRegister];

              //ʳ������ ������
              s = max_time_milliseconds_before / 1000;
              //ʳ������ ������� � ����� ��������
              ds_ms = max_time_milliseconds_before - s*1000;
              
              time_ms_tmp -= ds_ms;
              time_dat_tmp -= (time_t)s;

              if (time_ms_tmp < 0) 
              {
                time_ms_tmp += 1000;
                --time_dat_tmp;
              }

m1:
              p = localtime(&time_dat_tmp);
              switch(offsetRegister)
                {
                case 0:
                  {
                    temp_data = p->tm_mday;
                    break;
                  }
                case 1:
                  {
                    temp_data = p->tm_mon + 1;
                    break;
                  }
                case 2:
                  {
                    temp_data = p->tm_year + 1900;
                    break;
                  }
                case 3:
                  {
                    temp_data = p->tm_hour;
                    break;
                  }
                case 4:
                  {
                    temp_data = p->tm_min;
                    break;
                  }
                case 5:
                  {
                    temp_data = p->tm_sec*100 + time_ms_tmp/10;
                    break;
                  }
                case 6:
                  {
                    temp_data = (time_ms_tmp % 10) * 10000;
                    break;
                  }
                default:
                  temp_data = 0;
                }//switch
              return temp_data;
            }//if(registrator==ANALOG_REGISTRATOR)
          else
            {
              //if(registrator==DISKRET_REGISTRATOR)
              //���������� ������ BCD � int
              time_t time_dat_tmp;
              int32_t time_ms_tmp;

              for (unsigned int i = 0; i < sizeof(time_t); i++) *((unsigned char *)(&time_dat_tmp) + i) = point_to_buffer[FIRST_INDEX_DATA_TIME_DR + i];
              for (unsigned int i = 0; i < sizeof(int32_t); i++) *((unsigned char *)(&time_ms_tmp) + i) = point_to_buffer[FIRST_INDEX_DATA_TIME_DR + sizeof(time_t) + i];

              int temp_data;
              struct tm *p = localtime(&time_dat_tmp);
              switch(offsetRegister)
                {
                case 0:
                  {
                    temp_data = p->tm_mday;
                    break;
                  }
                case 1:
                  {
                    temp_data = p->tm_mon + 1;
                    break;
                  }
                case 2:
                  {
                    temp_data = p->tm_year + 1900;
                    break;
                  }
                case 3:
                  {
                    temp_data = p->tm_hour;
                    break;
                  }
                case 4:
                  {
                    temp_data = p->tm_min;
                    break;
                  }
                case 5:
                  {
                    temp_data = p->tm_sec*100 + time_ms_tmp/10;
                    break;
                  }
                case 6:
                  {
                    temp_data = (time_ms_tmp % 10) * 10000;
                    break;
                  }
                default:
                  temp_data = 0;
                }//switch
              return temp_data;
            } //else
        }//case 0-6
        }//switch
      return MARKER_ERRORPERIMETR;
    }//case 3 4
    case 5://��� ������ subObj
      if(recordLen>1) return MARKER_ERRORPERIMETR;
      //if(registrator==DISKRET_REGISTRATOR) 
      return 'B'; //���� - �� ������ �����
      //return (1000000 >> VAGA_NUMBER_POINT_AR)/50; //����� �������������
    case 6://����������� ��������� ����� subObj
      if(recordLen>1) return MARKER_ERRORPERIMETR;
    if(registrator==DISKRET_REGISTRATOR) return 1000;
    return (1000000 >> VAGA_NUMBER_POINT_AR)/50;
    }//switch

  return MARKER_ERRORPERIMETR;
}//

int dataDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen)
{
//������ Diskret ������������
  unsigned char *point_to_buffer;
  if (pointInterface==USB_RECUEST)//����� ���������� 0-USB 1-RS485
    point_to_buffer = buffer_for_USB_read_record_dr;
  else if (pointInterface==RS485_RECUEST)
    point_to_buffer = buffer_for_RS485_read_record_dr;
#if (MODYFIKACIA_VERSII_PZ >= 10)
  else if (pointInterface==LAN_RECUEST)
    point_to_buffer = buffer_for_LAN_read_record_dr;
#endif  
  else
  {
    //���������� ����� ������ �� ���� � ����
    total_error_sw_fixed(206);
  }

  unsigned int max_number_two_bytes = (NUMBER_TOTAL_SIGNAL_FOR_RANG >> 4);
  if ((max_number_two_bytes << 4) != NUMBER_TOTAL_SIGNAL_FOR_RANG)  max_number_two_bytes++;

  unsigned int lenMax = (3 + max_number_two_bytes);
  if ((unsigned int)recordLen > lenMax) return MARKER_ERRORPERIMETR;//���� ���� ����������

  if(recordNumber>(*(point_to_buffer + FIRST_INDEX_NUMBER_ITEMS_DR))) return MARKER_ERRORPERIMETR;//���� ���� ����������

  unsigned int offset = FIRST_INDEX_FIRST_DATA_DR + (recordNumber + 1)*38; //�� ��������� ����� ������ ��������� �������� (�� �������� ������� ������ ����������� ����������)

  switch(offsetRegister)
    {
    case 0://���������� ����� �������
      return recordNumber + 1;
    case 1://����� ������� (ms)
      return (*(point_to_buffer + offset + 0)) + ((*(point_to_buffer + offset + 1)) << 8 );
    case 2://����� ������� (ms)
      return *(point_to_buffer + offset + 2);
    }//switch

  return (*(point_to_buffer + offset + 3 + (offsetRegister - 3)*2)) + ((*(point_to_buffer + offset + 4 + (offsetRegister - 3)*2)) << 8);
}//dataDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen)
