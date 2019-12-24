#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 61800
//�������� ������� � ����� ������
#define END_ADR_REGISTER 61818

int privatePREGBigGetReg2(int adrReg);

int getPREGBigModbusRegister(int);//�������� ���������� ��������
int getPREGBigModbusBit(int);//�������� ���������� ����
int setPREGBigModbusRegister(int, int);//�������� ���������� ��������
int setPREGBigModbusBit(int, int);//�������� ���������� ����

void setPREGBigCountObject(void);//�������� �-�� �������
void prePREGBigReadAction(void);//action �� ������
void prePREGBigWriteAction(void);//action �� ������
int  postPREGBigWriteAction(void);//action ����� ������

COMPONENT_OBJ *pregbigcomponent;

/**************************************/
//���������� ���������� REGistrator prog
/**************************************/
void constructorPREGBigComponent(COMPONENT_OBJ *pregbigcomp)
{
  pregbigcomponent = pregbigcomp;

  pregbigcomponent->getModbusRegister = getPREGBigModbusRegister;//�������� ���������� ��������
  pregbigcomponent->getModbusBit      = getPREGBigModbusBit;//�������� ���������� ����
  pregbigcomponent->setModbusRegister = setPREGBigModbusRegister;//�������� ���������� ��������
  pregbigcomponent->setModbusBit      = setPREGBigModbusBit;//�������� ���������� ����

  pregbigcomponent->preReadAction   = prePREGBigReadAction;//action �� ������
  pregbigcomponent->preWriteAction  = prePREGBigWriteAction;//action �� ������
  pregbigcomponent->postWriteAction = postPREGBigWriteAction;//action ����� ������

  pregbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getPREGBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privatePREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  int offset = adrReg-BEGIN_ADR_REGISTER;
  switch(offset)
    {
    case 0://�������� ����������� ����������� ������
      return MARKER_ERRORPERIMETR;
    case 1://����� ������ ��������� ��� ������
      if(pointInterface==USB_RECUEST)//����� ���������� 0-USB 1-RS485 2-LAN
        return (number_record_of_pr_err_into_USB) &0xFFFF;
      if(pointInterface==RS485_RECUEST)//����� ���������� 0-USB 1-RS485 2-LAN
        return (number_record_of_pr_err_into_RS485) &0xFFFF;
#if (MODYFIKACIA_VERSII_PZ >= 10)
      if(pointInterface==LAN_RECUEST) return (number_record_of_pr_err_into_LAN) &0xFFFF;
#endif  
      //���������� ���� ������ �� ���� � �������� �����
      total_error_sw_fixed(211);
    case 2://���������� �������
      return (info_rejestrator_pr_err.number_records) &0xFFFF;
    case 3://���� ����������� ��������
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
      
      if (
        ((pointInterface==USB_RECUEST) && (number_record_of_pr_err_into_USB   == 0xffff)) 
        ||
        ((pointInterface==RS485_RECUEST) && (number_record_of_pr_err_into_RS485 == 0xffff))
#if (MODYFIKACIA_VERSII_PZ >= 10)        
        ||
        ((pointInterface==LAN_RECUEST) && (number_record_of_pr_err_into_LAN == 0xffff))
#endif
      ) return MARKER_ERRORPERIMETR;
      
      if (
        ((clean_rejestrators & CLEAN_PR_ERR) != 0) ||
        (
          ((pointInterface==USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB  ) != 0)) 
          ||
          ((pointInterface==RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485) != 0))
#if (MODYFIKACIA_VERSII_PZ >= 10)        
          ||
          ((pointInterface==LAN_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_LAN) != 0))
#endif
        )
      ) return MARKER_ERRORPERIMETR;
      if (
        (
          (pointInterface==USB_RECUEST)//����� ���������� 0-USB 1-RS485 2-LAN
          &&
          (
            (number_record_of_pr_err_into_USB >= info_rejestrator_pr_err.number_records) ||
            (number_record_of_pr_err_into_USB >= MAX_NUMBER_RECORDS_INTO_PR_ERR        )
          )
        )
        ||
        (
          (pointInterface==RS485_RECUEST)//����� ���������� 0-USB 1-RS485 2-LAN
          &&
          (
            (number_record_of_pr_err_into_RS485 >= info_rejestrator_pr_err.number_records) ||
            (number_record_of_pr_err_into_RS485 >= MAX_NUMBER_RECORDS_INTO_PR_ERR        )
          )
        )
#if (MODYFIKACIA_VERSII_PZ >= 10)
        ||
        (
          (pointInterface==LAN_RECUEST)//����� ���������� 0-USB 1-RS485 2-LAN
          &&
          (
            (number_record_of_pr_err_into_LAN >= info_rejestrator_pr_err.number_records) ||
            (number_record_of_pr_err_into_LAN >= MAX_NUMBER_RECORDS_INTO_PR_ERR        )
          )
        )
#endif          
      )
        {
          //�������, �� ����� ������ �� ��������
          if (pointInterface==USB_RECUEST) number_record_of_pr_err_into_USB = 0xffff;
          else if (pointInterface==RS485_RECUEST) number_record_of_pr_err_into_RS485 = 0xffff;
#if (MODYFIKACIA_VERSII_PZ >= 10)
          else if (pointInterface==LAN_RECUEST) number_record_of_pr_err_into_LAN = 0xffff;
#endif  
          else
          {
            //���������� ����� ������ �� ���� � ����
            total_error_sw_fixed(214);
          }
          return MARKER_ERRORPERIMETR;
        }//if
      
      else
        {
          unsigned char *point_to_buffer;
          if (pointInterface==USB_RECUEST) point_to_buffer = buffer_for_USB_read_record_pr_err;
          else if (pointInterface==RS485_RECUEST) point_to_buffer = buffer_for_RS485_read_record_pr_err;
#if (MODYFIKACIA_VERSII_PZ >= 10)
          else if (pointInterface==LAN_RECUEST) point_to_buffer = buffer_for_LAN_read_record_pr_err;
#endif  
          else
          {
            //���������� ����� ������ �� ���� � ����
            total_error_sw_fixed(193);
          }

          if ( (*(point_to_buffer + 0)) != LABEL_START_RECORD_PR_ERR)
            {
              //����������� ����������� ����
              return MARKER_ERRORPERIMETR;
            }
          else
            {
              time_t time_dat_tmp;
              int32_t time_ms_tmp;
              for(size_t i = 0; i < sizeof(time_t); i++) *((unsigned char*)(&time_dat_tmp) + i) =  point_to_buffer[1 + i];
              for(size_t i = 0; i < sizeof(int32_t); i++) *((unsigned char*)(&time_ms_tmp) + i) = point_to_buffer[1 + sizeof(time_t) + i];
              
              struct tm *p = localtime(&time_dat_tmp);
              switch (offset-3)//temp_address)
                {
                case 0:
                {
                  int field1 = p->tm_year - 100;
                  int field2 = p->tm_mon + 1;
                  return ((INT_TO_BCD(field1) << 8) | INT_TO_BCD(field2)) &0xFFFF;
                }
                case 1:
                {
                  int field1 = p->tm_mday;
                  return (INT_TO_BCD(field1) << 8) &0xFFFF;
                }
                case 2:
                {
                  int field1 = p->tm_hour;
                  int field2 = p->tm_min;
                  return ((INT_TO_BCD(field1) << 8) | INT_TO_BCD(field2)) &0xFFFF;
                }
                case 3:
                {
                  int field1 = p->tm_sec;
                  int field2 = time_ms_tmp / 10;
                  return ((INT_TO_BCD(field1) << 8) | INT_TO_BCD(field2)) &0xFFFF;
                }
                case 4:
                {
                  return (((*(point_to_buffer + 15))  << 8) | (*(point_to_buffer + 14))) &0xFFFF;
                }
                case 5:
                {
                  return (((*(point_to_buffer + 17))  << 8) | (*(point_to_buffer + 16))) &0xFFFF;
                }
                case 6:
                {
                  return (((*(point_to_buffer + 19))  << 8) | (*(point_to_buffer + 18))) &0xFFFF;
                }
                case 7:
                {
                  return (((*(point_to_buffer + 21))  << 8) | (*(point_to_buffer + 20))) &0xFFFF;
                }
                case 8:
                {
                  return (((*(point_to_buffer + 23))  << 8) | (*(point_to_buffer + 22))) &0xFFFF;
                }
                case 9:
                {
                  return (((*(point_to_buffer + 25))  << 8) | (*(point_to_buffer + 24))) &0xFFFF;
                }
                case 10:
                {
                  return (((*(point_to_buffer + 27))  << 8) | (*(point_to_buffer + 26))) &0xFFFF;
                }
                case 11:
                {
                  return (((*(point_to_buffer + 29))  << 8) | (*(point_to_buffer + 28))) &0xFFFF;
                }
                case 12:
                {
                  return (((*(point_to_buffer + 31))  << 8) | (*(point_to_buffer + 30))) &0xFFFF;
                }
                case 13:
                {
                  return (((*(point_to_buffer + 33))  << 8) | (*(point_to_buffer + 32))) &0xFFFF;
                }
                case 14:
                {
                  return (((*(point_to_buffer + 35))  << 8) | (*(point_to_buffer + 34))) &0xFFFF;
                }
                case 15:
                {
                  return (((*(point_to_buffer + 37))  << 8) | (*(point_to_buffer + 36))) &0xFFFF;
                }
                case 16:
                {
                  return (((*(point_to_buffer + 39))  << 8) | (*(point_to_buffer + 38))) &0xFFFF;
                }
                case 17:
                {
                  return (((*(point_to_buffer + 41))  << 8) | (*(point_to_buffer + 40))) &0xFFFF;
                }
                case 18:
                {
                  return (((*(point_to_buffer + 43))  << 8) | (*(point_to_buffer + 42))) &0xFFFF;
                }
                case 19:
                {
                  return (((*(point_to_buffer + 45))  << 8) | (*(point_to_buffer + 44))) &0xFFFF;
                }
                }//switch

            }//else
            }//else
          
          break;//case 3-8

        }//switch


      return 0;
    }//getDOUTBigModbusRegister(int adrReg)
  int getPREGBigModbusBit(int x)
  {
    //�������� ���������� ��������
    UNUSED(x);
    return MARKER_OUTPERIMETR;
  }//getDOUTBigModbusRegister(int adrReg)
  int setPREGBigModbusRegister(int adrReg, int dataReg)
  {
    //�������� ���������� ��������
    if(privatePREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

    superSetOperativMarker(pregbigcomponent, adrReg);
    superSetTempWriteArray(dataReg);//�������� � �����

    int offset = adrReg-BEGIN_ADR_REGISTER;

    switch(offset)
      {
      case 0://�������� ����������� ����������� ������
        if (
          (current_ekran.current_level == EKRAN_TITLES_PR_ERR_REGISTRATOR )
          ||
          (current_ekran.current_level == EKRAN_DATA_LADEL_PR_ERR         )
          ||
          (current_ekran.current_level == EKRAN_CHANGES_DIAGNOSTICS_PR_ERR)
          ||
          (
            (control_tasks_dataflash & (
               TASK_WRITE_PR_ERR_RECORDS_INTO_DATAFLASH    |
               TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB   |
               TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485 |
#if (MODYFIKACIA_VERSII_PZ >= 10)
               TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_LAN |
#endif
               TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU
             )
            ) != 0
          )
          ||
          ((clean_rejestrators & CLEAN_PR_ERR) != 0)
        ) return MARKER_ERRORPERIMETR;
        return 0;

      case 1://����� ������ ��������� ��� ������
      {
        if (
//      ����� ��� ���������� ��� ���������� ������ ���������� ���������� ����,
//      ��� ������� ����, ���� �� �������� � ��������� �����������
          ((clean_rejestrators & CLEAN_PR_ERR) != 0) ||
          (
            ((pointInterface == USB_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB  ) != 0)) 
            ||
            ((pointInterface == RS485_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485) != 0))
#if (MODYFIKACIA_VERSII_PZ >= 10)
            ||
            ((pointInterface == LAN_RECUEST) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_LAN) != 0))
#endif
          )
        ) return MARKER_ERRORPERIMETR;

        if (
          ((unsigned int)dataReg < info_rejestrator_pr_err.number_records) &&
          (dataReg < MAX_NUMBER_RECORDS_INTO_PR_ERR        ) //��� ���������� �� ����� �� ������������� ������ ������ ������
        ) return 0;
      }//case 1
      break;
      }//switch


    return MARKER_ERRORPERIMETR;
  }//getDOUTBigModbusRegister(int adrReg)
  int setPREGBigModbusBit(int x, int y)
  {
    UNUSED(x);
    UNUSED(y);
    //�������� ���������� ��������
    return MARKER_OUTPERIMETR;
  }//getDOUTBigModbusRegister(int adrReg)

  void prePREGBigReadAction(void)
  {
//action �� ������
    pregbigcomponent->isActiveActualData = 1;
  }//
  void prePREGBigWriteAction(void)
  {
//action �� ������
    pregbigcomponent->operativMarker[0] = -1;
    pregbigcomponent->operativMarker[1] = -1;//����������� ������
    pregbigcomponent->isActiveActualData = 1;
  }//
  int postPREGBigWriteAction(void)
  {
//action ����� ������
    int beginAdr = pregbigcomponent->operativMarker[0];
    if(beginAdr<0) return 0;//�� ���� ������
    int endAdr   = pregbigcomponent->operativMarker[1];
    int countAdr = endAdr-beginAdr+1;
    if(endAdr<0) countAdr = 1;
    int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray

    for(int i=0; i<countAdr; i++)
      {
        int offset = i+beginAdr-BEGIN_ADR_REGISTER;
        switch(offset)
          {
          case 0://�������� ����������� ����������� ������
            //�������, �� ����� �������� ��������� ���������� ����
            clean_rejestrators |= CLEAN_PR_ERR;
            break;
          case 1://����� ������ ��������� ��� ������
          {
            if(pointInterface==USB_RECUEST)//����� ���������� 0-USB 1-RS485
              {
                //������������ ����� ������ ���������� ���������� ���� ��� ������� ����� ��������� USB
                number_record_of_pr_err_into_USB = (tempWriteArray[offsetTempWriteArray+i]);
                //������ ������� ������� ���������� ���������� ���� ���  ���������� USB

                //������ ������� ������� ���� � ����� ���'�� ��� USB
                control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB;
              }
            else if(pointInterface==RS485_RECUEST)//if (type_interface == RS485_RECUEST)
              {
                //������������ ����� ������ ���������� ���������� ���� ��� ������� ����� ��������� RS-485
                number_record_of_pr_err_into_RS485 = (tempWriteArray[offsetTempWriteArray+i]);
                //������ ������� ������� ���������� ���������� ���� ���  ���������� RS-485

                //������ ������� ������� ���� � ����� ���'�� ��� RS-485
                control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485;
              }
#if (MODYFIKACIA_VERSII_PZ >= 10)
            else if (pointInterface == LAN_RECUEST)
              {
                //������������ ����� ������ ���������� ���������� ���� ��� ������� ����� ��������� LAN
                number_record_of_pr_err_into_LAN = (tempWriteArray[offsetTempWriteArray+i]);
                //������ ������� ������� ���������� ���������� ���� ���  ���������� LAN

                //������ ������� ������� ���� � ����� ���'�� ��� LAN
                control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_LAN;
              }
#endif
            else
              {
                //���������� ����� ������ �� ���� � ����
                total_error_sw_fixed(189);
              }
          }//case 1
          break;
          }//switch
      }//for(int i=0; i<countAdr; i++)


    return 0;
  }//

  int privatePREGBigGetReg2(int adrReg)
  {
    //��������� ������� ��������
    return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
  }//privateGetReg2(int adrReg)


