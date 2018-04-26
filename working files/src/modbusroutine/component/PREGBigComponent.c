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
      if(pointInterface==USB_RECUEST)//����� ���������� 0-USB 1-RS485
        return (number_record_of_pr_err_into_USB) &0xFFFF;
      return (number_record_of_pr_err_into_RS485) &0xFFFF;
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
        ((pointInterface==USB_RECUEST  ) && (number_record_of_pr_err_into_USB   == 0xffff)) ||
        ((pointInterface==RS485_RECUEST  ) && (number_record_of_pr_err_into_RS485 == 0xffff))
      ) return MARKER_ERRORPERIMETR;
      
      if (
        ((clean_rejestrators & CLEAN_PR_ERR) != 0) ||
        (
          ((pointInterface==USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB  ) != 0)) ||
          ((pointInterface==RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485) != 0))
        )
      ) return MARKER_ERRORPERIMETR;
      if (
        (
          (pointInterface==USB_RECUEST)//����� ���������� 0-USB 1-RS485
          &&
          (
            (number_record_of_pr_err_into_USB >= info_rejestrator_pr_err.number_records) ||
            (number_record_of_pr_err_into_USB >= MAX_NUMBER_RECORDS_INTO_PR_ERR        )
          )
        )
        ||
        (
          (pointInterface==RS485_RECUEST)//����� ���������� 0-USB 1-RS485
          &&
          (
            (number_record_of_pr_err_into_RS485 >= info_rejestrator_pr_err.number_records) ||
            (number_record_of_pr_err_into_RS485 >= MAX_NUMBER_RECORDS_INTO_PR_ERR        )
          )
        )
      )
        {
          //�������, �� ����� ������ �� ��������
          if (pointInterface==USB_RECUEST) number_record_of_pr_err_into_USB = 0xffff;
          else number_record_of_pr_err_into_RS485 = 0xffff;
          return MARKER_ERRORPERIMETR;
        }//if
      
      else
        {
          unsigned char *point_to_buffer;
          if (pointInterface==USB_RECUEST) point_to_buffer = buffer_for_USB_read_record_pr_err;
          else point_to_buffer = buffer_for_RS485_read_record_pr_err;

          if ( (*(point_to_buffer + 0)) != LABEL_START_RECORD_PR_ERR)
            {
              //����������� ���������� ���
              return MARKER_ERRORPERIMETR;
            }
          else
            {
              switch (offset-3)//temp_address)
                {
                case 0:
                {
                  return (((*(point_to_buffer + 7))  << 8) | (*(point_to_buffer + 6))) &0xFFFF;
                }
                case 1:
                {
                  return ((*(point_to_buffer + 5))  << 8) &0xFFFF;
                }
                case 2:
                {
                  return (((*(point_to_buffer + 4))  << 8) | (*(point_to_buffer + 3))) &0xFFFF;
                }
                case 3:
                {
                  return (((*(point_to_buffer + 2))  << 8) | (*(point_to_buffer + 1))) &0xFFFF;
                }
                case 4:
                {
                  return (((*(point_to_buffer + 10))  << 8) | (*(point_to_buffer + 9))) &0xFFFF;
                }
                case 5:
                {
                  return (((*(point_to_buffer + 12))  << 8) | (*(point_to_buffer + 11))) &0xFFFF;
                }
                case 6:
                {
                  return (((*(point_to_buffer + 14))  << 8) | (*(point_to_buffer + 13))) &0xFFFF;
                }
                case 7:
                {
                  return (((*(point_to_buffer + 16))  << 8) | (*(point_to_buffer + 15))) &0xFFFF;
                }
                case 8:
                {
                  return (((*(point_to_buffer + 18))  << 8) | (*(point_to_buffer + 17))) &0xFFFF;
                }
                case 9:
                {
                  return (((*(point_to_buffer + 20))  << 8) | (*(point_to_buffer + 19))) &0xFFFF;
                }
                case 10:
                {
                  return (((*(point_to_buffer + 22))  << 8) | (*(point_to_buffer + 21))) &0xFFFF;
                }
                case 11:
                {
                  return (((*(point_to_buffer + 24))  << 8) | (*(point_to_buffer + 23))) &0xFFFF;
                }
                case 12:
                {
                  return (((*(point_to_buffer + 26))  << 8) | (*(point_to_buffer + 25))) &0xFFFF;
                }
                case 13:
                {
                  return (((*(point_to_buffer + 28))  << 8) | (*(point_to_buffer + 27))) &0xFFFF;
                }
                case 14:
                {
                  return (((*(point_to_buffer + 30))  << 8) | (*(point_to_buffer + 29))) &0xFFFF;
                }
                case 15:
                {
                  return (((*(point_to_buffer + 32))  << 8) | (*(point_to_buffer + 31))) &0xFFFF;
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
            ((pointInterface == USB_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB  ) != 0)) ||
            ((pointInterface == RS485_RECUEST  ) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485) != 0))
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

                //������ ������� ������� ��� � ����� ���'�� ��� USB
                control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB;
              }
            else //if (type_interface == RS485_RECUEST)
              {
                //������������ ����� ������ ���������� ���������� ���� ��� ������� ����� ��������� RS-485
                number_record_of_pr_err_into_RS485 = (tempWriteArray[offsetTempWriteArray+i]);
                //������ ������� ������� ���������� ���������� ���� ���  ���������� RS-485

                //������ ������� ������� ��� � ����� ���'�� ��� RS-485
                control_tasks_dataflash |= TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485;
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


