#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 13000
//�������� ������� � ����� ������
#define END_ADR_REGISTER 13075

int privateREGBigGetReg2(int adrReg);

int getREGBigModbusRegister(int);//�������� ���������� ��������
int getREGBigModbusBit(int);//�������� ���������� ����
int setREGBigModbusRegister(int, int);//�������� ���������� ��������
int setREGBigModbusBit(int, int);//�������� ���������� ����

void setREGBigCountObject(void);//�������� �-�� �������
void preREGBigReadAction(void);//action �� ������
void preREGBigWriteAction(void);//action �� ������
int  postREGBigWriteAction(void);//action ����� ������

COMPONENT_OBJ *regbigcomponent;
__INFO_REJESTRATOR info_rejestrator_ar_ptr;
/**************************************/
//���������� ���������� REGistrator
/**************************************/
void constructorREGBigComponent(COMPONENT_OBJ *regbigcomp)
{
  regbigcomponent = regbigcomp;

  regbigcomponent->getModbusRegister = getREGBigModbusRegister;//�������� ���������� ��������
  regbigcomponent->getModbusBit      = getREGBigModbusBit;//�������� ���������� ����
  regbigcomponent->setModbusRegister = setREGBigModbusRegister;//�������� ���������� ��������
  regbigcomponent->setModbusBit      = setREGBigModbusBit;//�������� ���������� ����

  regbigcomponent->preReadAction   = preREGBigReadAction;//action �� ������
  regbigcomponent->preWriteAction  = preREGBigWriteAction;//action �� ������
  regbigcomponent->postWriteAction = postREGBigWriteAction;//action ����� ������

  regbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getREGBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
//  if(regbigcomponent->isActiveActualData)
//    {
//  info_rejestrator_ar_ptr = info_rejestrator_ar;
//    }//if(acmdsmallcomponent->isActiveActualData)
//  regbigcomponent->isActiveActualData = 0;

  int offset = adrReg-BEGIN_ADR_REGISTER;
//������������ ���������� ������� ����������� ������������
  if(offset<32) return getRangN_BIGModbusRegister(&current_settings.ranguvannja_analog_registrator[0], 32, offset );
//������������ ���������� ������� ����������� ������������
  if(offset>=36&&offset<70) return getRangN_BIGModbusRegister(&current_settings.ranguvannja_digital_registrator[0], 32, offset );

  switch(offset)
{
case 32://����� ������ ����������� ������������ (����������� ������)
   return current_settings.prefault_number_periods*20; //� ������� ��������� �������� �� ���������, � ������� ������
case 33://����� ������ ����������� ������������ (����������� ������)
   return current_settings.postfault_number_periods*20; //� ������� ��������� �������� �� ���������, � ������� ������
case 34://���������� ���������� �������������
   return info_rejestrator_ar.number_records;
case 35://������� ���������� �����������
   return number_record_of_ar_for_USB;
case 70://���������� ���������� �������������
    return info_rejestrator_dr.number_records;
case 71://������� ���������� �����������
    return number_record_of_dr_for_USB;
case 74://�������� ���������� �����������
return MARKER_ERRORPERIMETR;
case 75://�������� ���������� �����������
return MARKER_ERRORPERIMETR;
}//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getREGBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setREGBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateREGBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(regbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings;//������ �����
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(regbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  int offset = adrReg-BEGIN_ADR_REGISTER;
//������������ ���������� ������� ����������� ������������
//������������ ���������� ������� ����������� ������������
  if(offset<32 || (offset>=36&&offset<70)) return validN_BIGACMD(dataReg);

  switch(offset)
{
case 32://����� ������ ����������� ������������ (����������� ������)
   if(dataReg<TIMEOUT_PREFAULT_MIN || dataReg>TIMEOUT_PREFAULT_MAX) return MARKER_ERRORDIAPAZON;
    break;
case 33://����� ������ ����������� ������������ (����������� ������)
   if(dataReg<TIMEOUT_POSTFAULT_MIN || dataReg>TIMEOUT_POSTFAULT_MAX) return MARKER_ERRORDIAPAZON;
    break;
case 34://���������� ���������� �������������
   return MARKER_ERRORDIAPAZON;
case 35://������� ���������� �����������
    if(dataReg<0 ||(unsigned int)dataReg>info_rejestrator_ar.number_records) return MARKER_ERRORDIAPAZON;
    break;
case 70://���������� ���������� �������������
    return MARKER_ERRORDIAPAZON;
case 71://������� ���������� �����������
    if(dataReg<0 ||(unsigned int)dataReg>info_rejestrator_dr.number_records) return MARKER_ERRORDIAPAZON;
    break;
case 74://�������� ���������� �����������
    if(dataReg!=CMD_WORD_CLEAR_AR) return MARKER_ERRORDIAPAZON;
    break;
case 75://�������� ���������� �����������
    if(dataReg!=CMD_WORD_CLEAR_DR) return MARKER_ERRORDIAPAZON;
    break;
}//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setREGBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preREGBigReadAction(void) {
//action �� ������
  regbigcomponent->isActiveActualData = 1;
}//
void preREGBigWriteAction(void) {
//action �� ������
  regbigcomponent->operativMarker[0] = -1;
  regbigcomponent->operativMarker[1] = -1;//����������� ������
  regbigcomponent->isActiveActualData = 1;
}//
int postREGBigWriteAction(void) {
extern int upravlSchematic;//���� Rang
extern int upravlSetting;//���� Setting
//action ����� ������
  int beginAdr = regbigcomponent->operativMarker[0];
  int endAdr   = regbigcomponent->operativMarker[1];
  if(beginAdr<0) return 0;//�� ���� ������
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray

  int flag1=1;
  int flag2=1;
  int countReg=countAdr;
  for(int i=0; i<countAdr; i++)
    {
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;
  if(offset<32&&flag1){ 
//if((offset+countAdr)>=32) countReg=;
type_of_settings_changed |= (1 << RANGUVANNJA_DATA_CHANGED_BIT);
writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_analog_registrator[0], 32, beginAdr,
                                countReg, BEGIN_ADR_REGISTER);
flag1=0;
}//if

  if(offset>=36&&offset<70&&flag2){
//if((offset+countAdr)>=32) countReg=;
 type_of_settings_changed |= (1 << RANGUVANNJA_DATA_CHANGED_BIT);
 writeRangN_BIGModbusRegister(&edition_settings.ranguvannja_digital_registrator[0], 32, offset-36,
                                countReg, BEGIN_ADR_REGISTER+36);
flag2=0;
}//if
  switch(offset)
{
case 32://����� ������ ����������� ������������ (����������� ������)
  // _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
   type_of_settings_changed |= (1 << SETTINGS_DATA_CHANGED_BIT);
   edition_settings.prefault_number_periods = tempWriteArray[offsetTempWriteArray+i]/20; //� ������� ��������� �������� �� ���������, � ������� ������
   break;
case 33://����� ������ ����������� ������������ (����������� ������)
//   _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
   type_of_settings_changed |= (1 << SETTINGS_DATA_CHANGED_BIT);
   edition_settings.postfault_number_periods = tempWriteArray[offsetTempWriteArray+i]/20; //� ������� ��������� �������� �� ���������, � ������� ������
   break;
case 35://������� ���������� �����������
   number_record_of_ar_for_USB = tempWriteArray[offsetTempWriteArray+i];
   break;
case 71://������� ���������� �����������
   number_record_of_dr_for_USB = tempWriteArray[offsetTempWriteArray+i];
   break;
case 74://�������� ���������� �����������
//������ ��������
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
            ) return ERROR_VALID2;//������ ���������
     clean_rejestrators |= CLEAN_AR;
   break;
case 75://�������� ���������� �����������
//������ ��������
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
            ) return ERROR_VALID2;//������ ���������
      //�������, �� ����� �������� ����������� ����������
     clean_rejestrators |= CLEAN_DR;
   break;
}//switch
   }//for
      //��������� ������� - ���������
     upravlSetting = 2;//���� Setting
     upravlSchematic = 2;//���� Rang

//      current_settings = edition_settings;//��������� ���������

      //³������� ���� ���������
//      _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
      //����������� �������, �� �� ������ ����� �������� ����������
  //    new_state_keyboard |= (1<<BIT_REWRITE);
    //  fix_change_settings_m(1, 2);//1 - ����� ���������� 2 - USB
      //restart_timeout_idle_new_settings = true;


  return 0;
}//

int privateREGBigGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)
