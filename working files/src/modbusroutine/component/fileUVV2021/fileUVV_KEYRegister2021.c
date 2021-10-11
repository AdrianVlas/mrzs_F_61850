
#include "header.h"

//к-во ранжир на одном key
#define REGISTERS_KEY 8

int privateKEYvalidRecordLen(int recordNumber, int recordLen);
int privateKEYSelUstRegisterNumber(int registerNumber);
int privateKEYSelRangRegisterNumber(int registerNumber);
int getFileKEYRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileKEYRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileKEYRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

/**************************************/
//подготовка компонента KEY
/**************************************/

int privateKEYvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordNumber
  if(recordNumber == 0) return MARKER_ERRORPERIMETR;
  if(recordNumber > NUMBER_DEFINED_BUTTONS) return MARKER_ERRORPERIMETR;
//валидация recordLen
  if(recordLen > REGISTERS_KEY+5) return MARKER_ERRORPERIMETR;
  return 0;
}//privateKEYvalidRecordLen

int privateKEYSelUstRegisterNumber(int registerNumber)
{
  if(registerNumber == 1) return 1;
  return 0;
}//privateKEYSelUstRegisterNumber
int privateKEYSelRangRegisterNumber(int registerNumber)
{
  if(registerNumber >= 5) return 1;
  return 0;
}//privateKEYSelRangRegisterNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileKEYRegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen recordNumber
  if(privateKEYvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  int item  = recordNumber - 1;

  if(privateKEYSelUstRegisterNumber(registerNumber))
  {
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
    if(edition_settings.buttons_mode & (1<<item)) return 1;
    return 0;
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
  }//if
  else if(privateKEYSelRangRegisterNumber(registerNumber))
  {
    //Ранжирование KEY
    return getRangN_SMALLModbusRegister2021(&current_settings_interfaces.ranguvannja_buttons[item*N_SMALL],
                                            registerNumber-5);
  }//if

  if(registerNumber < 5) return 0;
  return MARKER_ERRORPERIMETR;
}//getFileKEYRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileKEYRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen recordNumber
  if(privateKEYvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateKEYSelUstRegisterNumber(registerNumber))
    {
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
      //проверкa ust
      if(value > 1) return MARKER_ERRORDIAPAZON;
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
    }//if
    else if(privateKEYSelRangRegisterNumber(registerNumber))
    {
      //Ранжирование KEY
      uint32_t maska = ((uint32_t)edition_settings.buttons_mode)&(1<<(recordNumber-1));
      //проверка на допустимость Режим «Кнопка»
      if(!maska)
      {
        switch((unsigned short)value)
        {
        case 50000://- 1-а гр.уставок    *
        case 50001://- 2-а гр.уставок     *
        case 50002://- 3-а гр.уставок     *
        case 50003://- 4-а гр.уставок     *
        case 50016://- Блок.МСЗ1          *
        case 50032://- Блок.МСЗ2          *
        case 50041://- Блок.приск.МСЗ2    *
        case 50048://- Блок.МСЗ3          *
        case 50064://- Блок.МСЗ4
        case 50080://- Блок.МСЗ 0.4кВ 1
        case 50083://- Блок.МСЗ 0.4кВ 2
        case 50086://- Бл.п.МСЗ 0.4кВ 2
        case 50112://- Блок.ЗДЗ
        case 50113://- Пуск ЗДЗ від ДВ
        case 50128://- Блок.НЗЗ
        case 50144://- Блок.TZNP1
        case 50154://- Блок.TZNP2
        case 50164://- Блок.TZNP3
        case 50176://- Блок.ЗЗП(КОФ)
        case 50192://- Блокировка ЗНмин1
        case 50197://- Блокировка ЗНмин1
        case 50202://- Пуск ЗНмин1
        case 50203://- Пуск ЗНмин2
        case 50208://- Блокировка ЗНмакс1
        case 50211://- Блокировка ЗНмакс2
        case 50288://- Стат.блок.АПВ
        case 50304://  Пуск ПРВВ від ДВ
        case 50368://- АЧР/ЧАПВ
        case 50369://- Блок.АЧР1
        case 50370://- Блок.АЧР2
        case 50384://- Блок.УЗ1
        case 50387://- Блок.УЗ2
        case 50390://- Блок.УЗ3
        case 50393://- Блок.УЗ4
        case 50396://- Блок.УЗ5
        case 50399://- Блок.УЗ6
        case 50402://- Блок.УЗ7
        case 50405://- Блок.УЗ8
        case 50528://- Ввімк.ВВ      *
        case 50529://- Вимк.ВВ       *
        case 50530://- Блок.ввімкн.ВВ  *
        case 50534://- Стан ВВ         *
        case 50536://- Контроль Ввімк. *
        case 50537://- Контроль Вимк.  *
//      case 50562://- Сброс индикации
//      case 50563://- Сброс реле
        case 50569://- Місц./Дистанц. *
        case 50576://- Вимк.від зовн.з.*
//      case 50599://- С.блк.Гот.до ТУ
          return MARKER_ERRORPERIMETR; //не прошла валидация
        }//switch
        if(((unsigned short)value)>=50608) return MARKER_ERRORPERIMETR; //не прошла валидация
      }//if(!maska)
      else
        //проверка на допустимость Режим «Ключ
        switch((unsigned short)value)
        {
//   case 50000://- 1-а гр.уставок    *
//   case 50001://- 2-а гр.уставок     *
//   case 50002://- 3-а гр.уставок     *
//   case 50003://- 4-а гр.уставок     *
//   case 50016://- Блок.МСЗ1          *
//   case 50032://- Блок.МСЗ2          *
//   case 50041://- Блок.приск.МСЗ2    *
//   case 50048://- Блок.МСЗ3          *
//   case 50064://- Блок.МСЗ4
//   case 50080://- Блок.МСЗ 0.4кВ 1
//   case 50083://- Блок.МСЗ 0.4кВ 2
//   case 50086://- Бл.п.МСЗ 0.4кВ 2
//   case 50112://- Блок.ЗДЗ
        case 50113://- Пуск ЗДЗ від ДВ
//   case 50128://- Блок.НЗЗ
//   case 50144://- Блок.TZNP1
//   case 50154://- Блок.TZNP2
//   case 50164://- Блок.TZNP3
//   case 50176://- Блок.ЗЗП(КОФ)
//   case 50192://- Блокировка ЗНмин1
//   case 50197://- Блокировка ЗНмин1
        case 50202://- Пуск ЗНмин1
        case 50203://- Пуск ЗНмин2
//   case 50208://- Блокировка ЗНмакс1
//   case 50211://- Блокировка ЗНмакс2
//   case 50288://- Стат.блок.АПВ
        case 50304://  Пуск ПРВВ від ДВ
        case 50368://- АЧР/ЧАПВ
//   case 50369://- Блок.АЧР1
//   case 50370://- Блок.АЧР2
//   case 50384://- Блок.УЗ1
//   case 50387://- Блок.УЗ2
//   case 50390://- Блок.УЗ3
//   case 50393://- Блок.УЗ4
//   case 50396://- Блок.УЗ5
//   case 50399://- Блок.УЗ6
//   case 50402://- Блок.УЗ7
//   case 50405://- Блок.УЗ8
        case 50528://- Ввімк.ВВ      *
        case 50529://- Вимк.ВВ       *
//   case 50530://- Блок.ввімкн.ВВ  *
        case 50534://- Стан ВВ         *
        case 50536://- Контроль Ввімк. *
        case 50537://- Контроль Вимк.  *
        case 50562://- Сброс индикации
        case 50563://- Сброс реле
//   case 50569://- Місц./Дистанц. *
        case 50576://- Вимк.від зовн.з.*
        case 50599://- С.блк.Гот.до ТУ
          return MARKER_ERRORPERIMETR; //не прошла валидация
        }//switch

    }//if
  }//for
  return 0;
}//setFileKEYRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileKEYRegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//флаг Rang
  extern int upravlSetting;//флаг Setting

  int item  = recordNumber - 1;
  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)//номера регистров
  {
    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);
    if(privateKEYSelUstRegisterNumber(registerNumber))
    {
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      if(value) edition_settings.buttons_mode |= (1<<item);
      else  edition_settings.buttons_mode &= ~(1<<item);

      action_during_changing_button_mode(&current_settings_interfaces, &edition_settings);

      for (unsigned int i = 0; i < N_SMALL; i++)
      {
        edition_settings.ranguvannja_buttons[N_SMALL*item + i] =
          current_settings_interfaces.ranguvannja_buttons[N_SMALL*item + i];
      }//for
//- - - - - - - - - - - - - - - - - - - - - - - - - - -
      upravlSetting = 1;//флаг Setting
      upravlSchematic = 1;//флаг Rang
    }//if
  }//for

  if(recordLen >= 5)
  {
    writeRangN_SMALLModbusRegister2021(&edition_settings.ranguvannja_buttons[item * N_SMALL], &dataPacket[5], recordLen-5);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileKEYRegister21

