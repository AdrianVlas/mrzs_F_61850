
#include "header.h"
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)

int privateIECGOOSEvalidRecordLen(int recordNumber, int recordLen);
int privateIECGOOSERangRecordNumber(int recordNumber);
int getFileIECGOOSERegister20(int recordNumber, int registerNumber, int recordLen);
int setFileIECGOOSERegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileIECGOOSERegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

#define REGISTERS_GOOS 8
//к-во частей внутри одного GOOS
#define PART_REGISTERS_GOOS 8

int privateIECGOOSEvalidRecordLen(int recordNumber, int recordLen)
{
//валидация recordNumber
  if(recordNumber > N_IN_GOOSE*PART_REGISTERS_GOOS) return MARKER_ERRORPERIMETR;
//валидация recordLen
  if(recordLen > REGISTERS_GOOS+1) return MARKER_ERRORPERIMETR;
  return 0;
}//privateIECGOOSEvalidRecordLen

int privateIECGOOSERangRecordNumber(int recordNumber)
{
  if(recordNumber > 0) return 1;
  return 0;
}//privateIECGOOSERangRecordNumber

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileIECGOOSERegister20(int recordNumber, int registerNumber, int recordLen)
{
//валидация recordLen
  if(privateIECGOOSEvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;
  if(registerNumber == 0) return 0;

  if(privateIECGOOSERangRecordNumber(recordNumber))
  {
    //Ранжирование
    int item     = (recordNumber - 1)/PART_REGISTERS_GOOS;
    int itemPart = (recordNumber - 1)%PART_REGISTERS_GOOS;
    return getRangN_SMALLModbusRegister2021(current_settings_interfaces.ranguvannja_In_GOOSE[item][itemPart],
                                            registerNumber-1);
  }//if

  return MARKER_ERRORPERIMETR;
}//getFileIECGOOSERegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-
int setFileIECGOOSERegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
//валидация recordLen
  if(privateIECGOOSEvalidRecordLen(recordNumber, recordLen) != 0) return MARKER_ERRORPERIMETR;

  for(int registerNumber=0; registerNumber<recordLen; registerNumber++)
  {
    if(registerNumber == 0) continue;

    uint32_t value = (unsigned short) swapByteInShort(dataPacket[registerNumber]);

    if(privateIECGOOSERangRecordNumber(recordNumber))
    {
      if(validN_SMALLACMD(value) != 0) return MARKER_ERRORPERIMETR;
      if(value == 0) return 0;//прошла валидация

      switch((unsigned short)value)
      {
      case 50000://- 1-а гр.уставок    *
      case 50001://- 2-а гр.уставок     *
      case 50002://- 3-а гр.уставок     *
      case 50003://- 4-а гр.уставок     *

//   case 50528://- Ввімк.ВВ      *
//   case 50529://- Вимк.ВВ       *
//   case 50530://- Блок.ввімкн.ВВ  *
//   case 50534://- Стан ВВ         *
//   case 50536://- Контроль Ввімк. *
//   case 50537://- Контроль Вимк.  *
      case 50562://- Сброс индикации
      case 50563://- Сброс реле
      case 50569://- Місц./Дистанц. *
//   case 50576://- Вимк.від зовн.з.*
      case 50599://- С.блк.Гот.до ТУ

      case 50624://- Блокировка Вх.MMS 1
      case 50625://- Блокировка Вх.MMS 2
      case 50626://- Блокировка Вх.MMS 3
      case 50627://- Блокировка Вх.MMS 4
        return MARKER_ERRORPERIMETR; //не прошла валидация
      }//switch
      return 0;
    }//if

    else return MARKER_ERRORPERIMETR;

  }//for
  return 0;
}//setFileGOOSERegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-
int postFileIECGOOSERegister21(int recordNumber, int recordLen, unsigned short *dataPacket)
{
  extern int upravlSchematic;//Rang

  if(privateIECGOOSERangRecordNumber(recordNumber))
  {
    int item     = (recordNumber - 1)/PART_REGISTERS_GOOS;
    int itemPart = (recordNumber - 1)%PART_REGISTERS_GOOS;
    writeRangN_SMALLModbusRegister2021(edition_settings.ranguvannja_In_GOOSE[item][itemPart], &dataPacket[1], recordLen-1);
    //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
    upravlSchematic = 1;//флаг Rang
  }//if

  return 0;
}//postFileIECGOOSERegister21
#endif
