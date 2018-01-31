#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 61948
//конечный регистр в карте памяти
#define END_ADR_REGISTER 62100

int privateYustBigGetReg2(int adrReg);

int getYustBigModbusRegister(int);//получить содержимое регистра
int getYustBigModbusBit(int);//получить содержимое бита
int setYustBigModbusRegister(int, int);//получить содержимое регистра
int setYustBigModbusBit(int, int);//получить содержимое бита

void preYustBigReadAction(void);//action до чтения
void preYustBigWriteAction(void);//action до записи
int postYustBigWriteAction(void);//action после записи
//void loadYustBigActualData(void);

COMPONENT_OBJ *yustbigcomponent;

/**************************************/
//подготовка компонента Юстировки
/**************************************/
void constructorYustBigComponent(COMPONENT_OBJ *yustbigcomp)
{
  yustbigcomponent = yustbigcomp;

  yustbigcomponent->getModbusRegister = getYustBigModbusRegister;//получить содержимое регистра
  yustbigcomponent->getModbusBit      = getYustBigModbusBit;//получить содержимое бита
  yustbigcomponent->setModbusRegister = setYustBigModbusRegister;//получить содержимое регистра
  yustbigcomponent->setModbusBit      = setYustBigModbusBit;//получить содержимое бита

  yustbigcomponent->preReadAction   = preYustBigReadAction;//action до чтения
  yustbigcomponent->preWriteAction  = preYustBigWriteAction;//action до записи
  yustbigcomponent->postWriteAction = postYustBigWriteAction;//action после записи

  yustbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

//void loadYustBigActualData(void) {
  //ActualData
//  for(int i=1; i<7; i++) tempReadArray[i] = ustuvannja[i-1];
//}//loadActualData() 

int getYustBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateYustBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

//  if(yustbigcomponent->isActiveActualData) loadYustBigActualData(); //ActualData
//  yustbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(yustbigcomponent, adrReg);

  int offset = adrReg-BEGIN_ADR_REGISTER;
  if(offset==0) return MARKER_ERRORPERIMETR;//61948 Запись серийного номера устройства
  if(offset==19) return MARKER_ERRORPERIMETR;//61967 Разрешение юстировки

    switch(offset) {//индекс регистра входа
  case 20:
  case 21:
  case 22:
  case 23:
  case 24:
  case 25:
  case 26:
  case 27:
      return ustuvannja[offset-20];
  case 52://62000
      return current_settings.number_iteration_el;//Максимальное количество итераций
 }//switch

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getYustBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setYustBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateYustBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(yustbigcomponent->isActiveActualData) {
  edit_serial_number_dev = serial_number_dev;
  for(int i=0; i<NUMBER_ANALOG_CANALES; i++) edit_ustuvannja[i] = ustuvannja[i];
  }//if
  yustbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(yustbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер
  if(adrReg==62000)
     if(dataReg<1 || dataReg>32) return MARKER_ERRORDIAPAZON;

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setYustBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //записать содержимое bit
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preYustBigReadAction(void) {
//action до чтения
  yustbigcomponent->operativMarker[0] = -1;
  yustbigcomponent->operativMarker[1] = -1;//оперативный маркер
  yustbigcomponent->isActiveActualData = 1;
}//

void preYustBigWriteAction(void) {
//action до записи
  yustbigcomponent->operativMarker[0] = -1;
  yustbigcomponent->operativMarker[1] = -1;//оперативный маркер
  yustbigcomponent->isActiveActualData = 1;
}//

int postYustBigWriteAction(void) {
//action после записи
  int beginAdr = yustbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = yustbigcomponent->operativMarker[1];
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  int upravlYust=0;//флаг юстировки
  int upravlMin=0;//флаг min param
  int number_iteration_el=-1;

  for(int i=0; i<countAdr; i++) {
  int offset = i+beginAdr-BEGIN_ADR_REGISTER;
    switch(offset) {//индекс регистра входа
     case 0://Серийный номер
      edit_serial_number_dev = tempWriteArray[offsetTempWriteArray+i];
     break;
     case 19://Разрешение юстировки
      upravlYust = tempWriteArray[offsetTempWriteArray+i];//юстировки
     break;
  case 20:
  case 21:
  case 22:
  case 23:
  case 24:
  case 25:
  case 26:
  case 27:
      edit_ustuvannja[offset-20] = tempWriteArray[offsetTempWriteArray+i];
  break;
  case 52://62000 number_iteration_el
      number_iteration_el = tempWriteArray[offsetTempWriteArray+i];//number_iteration_el
  break;
  case 152://62100
      upravlMin = tempWriteArray[offsetTempWriteArray+i];//флаг min param
  break;
 }//switch
  }//for

  if(number_iteration_el>0)
  {
   current_settings_interfaces.number_iteration_el = number_iteration_el;
//      if(type_interface == USB_RECUEST)
//Якщо уставки, то " 
      //Відбулася зміна настройки
      _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
  type_of_settings_changed |= (1 << SETTINGS_DATA_CHANGED_BIT);
           if(set_new_settings_from_interface(2)) return 2;//2-USB
 return 0;
  }//if(number_iteration_el>0)

  if(upravlYust==0x1978) {//флаг юстировки

  changed_ustuvannja = CHANGED_ETAP_EXECUTION;
  serial_number_dev = edit_serial_number_dev;
  for(int i=0; i<NUMBER_ANALOG_CANALES; i++) ustuvannja[i] = edit_ustuvannja[i];

   changed_ustuvannja = CHANGED_ETAP_ENDED;
   _SET_BIT(control_i2c_taskes, TASK_START_WRITE_USTUVANNJA_EEPROM_BIT);
  }//if
  else if(upravlMin==0x1111) {
    //min param
    if (current_ekran.current_level != EKRAN_MAIN) return 2;
    semaphore_read_state_ar_record = 1;
    if (state_ar_record != STATE_AR_NO_RECORD) {
        semaphore_read_state_ar_record = 0;
        return 2;
    }//if(state_ar_record != STATE_AR_NO_RECORD) 
        state_ar_record = STATE_AR_TEMPORARY_BLOCK; 
      
        //Скидаємо настройки у "мінімальні заводські значення"
        min_settings(&current_settings_interfaces);
   //     if (method_setting != SET_DATA_IMMEDITATE) return 2;
          actions_after_changing_tiomouts_ar();

          //Розблоковуємо роботу аналогового реєстратора
          state_ar_record = STATE_AR_NO_RECORD;
        semaphore_read_state_ar_record = 0;

      //Відбулася зміна настройки
      _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
//      _SET_BIT(type_of_settings_changed, DEFAULT_SETTINGS_SET_BIT);
       type_of_settings_changed = (1 << DEFAULT_SETTINGS_SET_BIT);
//       current_settings_interfaces = current_settings;
//      if(type_interface == USB_RECUEST)
           if(set_new_settings_from_interface(2)) return 2;
  }//if(upravlMin==0x1111) 
  else return 2;//флаг юстировки

 return 0;
}//

int privateYustBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

/*
Але щоб всі процедура була звершеною треба:
I.
1) виконати цю функцію, де вхідний вказівник приймає значення
 current_settings_interfaces (якщо активіація іде функцією 6), 
 або edition_settings (якщо активація іде функцєю 16
2) у масиві active_functions виставити біт RANG_SETTINGS_CHANGED про те,
   що налаштування змінені але ще не активовані
3) У type_of_settings_changed встановити біт DEFAULT_SETTINGS_SET_BIT,
   який сигналізує про те, що іде спроба встановити "заводські" налаштування

II.a
Зрозуміло, що команда деактивації змін має все відмінити

II.b
Команда активації має викликати функцію set_new_settings_from_interface, яка вже все решта зробить сама...
*/

