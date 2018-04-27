#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 61948
//конечный регистр в карте памяти
#define END_ADR_REGISTER 63001

int privateYustBigGetReg2(int adrReg);

int getYustBigModbusRegister(int);//получить содержимое регистра
int getYustBigModbusBit(int);//получить содержимое бита
int setYustBigModbusRegister(int, int);//получить содержимое регистра
int setYustBigModbusBit(int, int);//получить содержимое бита

void preYustBigReadAction(void);//action до чтения
void preYustBigWriteAction(void);//action до записи
int postYustBigWriteAction(void);//action после записи

COMPONENT_OBJ *yustbigcomponent;

  int upravlYust=0;//флаг юстировки

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

int getYustBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateYustBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

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
    return (ustuvannja[offset-20]) &0xFFFF;
  case 52-16+0://62000-16 +0
  case 52-16+1://62000-16 +1
  case 52-16+2://62000-16 +2
  case 52-16+3://62000-16 +3
  case 52-16+4://62000-16 +4
  case 52-16+5://62000-16 +5
  case 52-16+6://62000-16 +6
  case 52-16+7://62000-16 +7
    return (phi_ustuvannja[offset- (52-16+0)]) &0xFFFF;

  case 52://62000
    return (current_settings_interfaces.number_iteration_el) &0xFFFF;//Максимальное количество итераций
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
    for(int i=0; i<NUMBER_ANALOG_CANALES; i++) phi_edit_ustuvannja[i] = phi_ustuvannja[i];
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

//  int upravlYust=0;//флаг юстировки
  int upravlMin=0;//флаг min param
  int upravlMinEnrg=0;//флаг min Enrg
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
  case 52-16+0://62000-16 +0
  case 52-16+1://62000-16 +1
  case 52-16+2://62000-16 +2
  case 52-16+3://62000-16 +3
  case 52-16+4://62000-16 +4
  case 52-16+5://62000-16 +5
  case 52-16+6://62000-16 +6
  case 52-16+7://62000-16 +7
      {
      int index = offset- (52-16+0);
      int angle = tempWriteArray[offsetTempWriteArray+i];
      phi_edit_ustuvannja[index] = angle;
      float phi_radian_tmp = PI*((float)angle)/1800.0f;
      phi_ustuvannja_sin_cos[2*index+0] = arm_sin_f32(phi_radian_tmp);
      phi_ustuvannja_sin_cos[2*index+1] = arm_cos_f32(phi_radian_tmp);
      } break;

    case 52://62000 number_iteration_el
      number_iteration_el = tempWriteArray[offsetTempWriteArray+i];//number_iteration_el
      break;
    case 152://62100
      upravlMin = tempWriteArray[offsetTempWriteArray+i];//флаг min param
      break;
    case 1053://63001
      upravlMinEnrg = tempWriteArray[offsetTempWriteArray+i];//флаг min Enrg
      break;
    }//switch
  }//for

  if(number_iteration_el>0)
  {
    current_settings_interfaces.number_iteration_el = number_iteration_el;

    //Відбулася зміна настройки
    _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
    type_of_settings_changed |= (1 << SETTINGS_DATA_CHANGED_BIT);
    int typI = 2;
    if(pointInterface==RS485_RECUEST)//метка интерфейса 0-USB 1-RS485
      typI = 3;
    if(set_new_settings_from_interface(typI)) return ERROR_VALID2;//2-USB
    return 0;
  }//if(number_iteration_el>0)

  if(upravlYust==0x1978) {//флаг юстировки

    changed_ustuvannja = CHANGED_ETAP_EXECUTION;
    serial_number_dev = edit_serial_number_dev;
    for(int i=0; i<NUMBER_ANALOG_CANALES; i++) ustuvannja[i] = edit_ustuvannja[i];
    for(int i=0; i<NUMBER_ANALOG_CANALES; i++) phi_ustuvannja[i] = phi_edit_ustuvannja[i];

    changed_ustuvannja = CHANGED_ETAP_ENDED;
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_USTUVANNJA_EEPROM_BIT);
  }//if
  else if(upravlMin==0x1111) {

    //Скидаємо настройки у "мінімальні заводські значення"
    min_settings(&current_settings_interfaces);

    //Відбулася зміна настройки
    _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
    restart_timeout_idle_new_settings = true;
    type_of_settings_changed = (1 << DEFAULT_SETTINGS_SET_BIT);

    int typI = 2;
    if(pointInterface==RS485_RECUEST)//метка интерфейса 0-USB 1-RS485
      typI = 3;
    if(set_new_settings_from_interface(typI)) return ERROR_VALID2;
  }//if(upravlMin==0x1111)
  else if(upravlMinEnrg==0x1234) {
      //Для скидання показів лічильника треба вказати конкретне число - інакше повідомляємо, що такої адреси взагалі не існує (примітивний метод маскування від несанкціонованого дослідження карти пам'яті)
      clean_energy = 0xff;
  }//if(upravlMinEnrg==0x1234)
  else return ERROR_VALID2;//флаг юстировки

  return 0;
}//

int privateYustBigGetReg2(int adrReg)
{
  //проверить внешний периметр
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)


