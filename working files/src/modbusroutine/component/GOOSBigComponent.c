#include "header.h"
#if (MODYFIKACIA_VERSII_PZ >= 10)

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 2636
//конечный регистр в карте памяти
#define END_ADR_REGISTER 3659

//к-во регистров внутри одной части GOOS
#define REGISTERS_GOOS 8
//к-во частей внутри одного GOOS
#define PART_REGISTERS_GOOS 8

int privateGOOSBigGetReg2(int adrReg);

int getGOOSBigModbusRegister(int);//получить содержимое регистра
int getGOOSBigModbusBit(int);//получить содержимое бита
int setGOOSBigModbusRegister(int, int);//получить содержимое регистра
int setGOOSBigModbusBit(int, int);//получить содержимое бита

void setGOOSBigCountObject(void);//записать к-во обектов
void preGOOSBigReadAction(void);//action до чтения
void preGOOSBigWriteAction(void);//action до записи
int  postGOOSBigWriteAction(void);//action после записи

COMPONENT_OBJ *goosbigcomponent;

/**************************************/
//подготовка компонента GOOS
/**************************************/
void constructorGOOSBigComponent(COMPONENT_OBJ *goosbigcomp)
{
  goosbigcomponent = goosbigcomp;

  goosbigcomponent->getModbusRegister = getGOOSBigModbusRegister;//получить содержимое регистра
  goosbigcomponent->getModbusBit      = getGOOSBigModbusBit;//получить содержимое бита
  goosbigcomponent->setModbusRegister = setGOOSBigModbusRegister;//получить содержимое регистра
  goosbigcomponent->setModbusBit      = setGOOSBigModbusBit;//получить содержимое бита

  goosbigcomponent->preReadAction   = preGOOSBigReadAction;//action до чтения
  goosbigcomponent->preWriteAction  = preGOOSBigWriteAction;//action до записи
  goosbigcomponent->postWriteAction = postGOOSBigWriteAction;//action после записи

  goosbigcomponent->isActiveActualData = 0;
}//

int getGOOSBigModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateGOOSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  int offset = adrReg-BEGIN_ADR_REGISTER;
  int idxObj = offset/(REGISTERS_GOOS*PART_REGISTERS_GOOS);

//поиск активного бита
  unsigned int *ranguvannja_goos = &current_settings_interfaces.ranguvannja_In_GOOSE[idxObj]
                                    [(offset%(REGISTERS_GOOS*PART_REGISTERS_GOOS))/REGISTERS_GOOS][0]; //Ранжування 
  int bit = getSequenceN_SMALLIndexActiveBit(offset%REGISTERS_GOOS, ranguvannja_goos);//индекс активного бита
  if(bit!=-1)
    {
      int adr = decoderN_SMALLACMD(bit);
      return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus  для dv
    }//if(bit!=N_BIG*32)
  return 0;
}//getGOOSBigModbusRegister(int adrReg)
int getGOOSBigModbusBit(int x)
{
  //получить содержимое регистра
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setGOOSBigModbusRegister(int adrReg, int dataReg)
{
  //записать содержимое регистра
  if(privateGOOSBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(goosbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//делаем копию
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(goosbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//записать в буфер

  if(validN_SMALLACMD(dataReg)==MARKER_ERRORPERIMETR) return MARKER_ERRORPERIMETR;

  switch((unsigned short)dataReg)
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
}//
int setGOOSBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //получить содержимое регистра
  return MARKER_OUTPERIMETR;
}//

void preGOOSBigReadAction(void) {
//action до чтения
  goosbigcomponent->isActiveActualData = 1;
}//
void preGOOSBigWriteAction(void) {
//action до записи
  goosbigcomponent->operativMarker[0] = -1;
  goosbigcomponent->operativMarker[1] = -1;//оперативный маркер
  goosbigcomponent->isActiveActualData = 1;
}//
int postGOOSBigWriteAction(void) {
extern int upravlSchematic;//флаг Rang
//action после записи
  int beginAdr = goosbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = goosbigcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray

  unsigned int tmp[N_SMALL];//для сохр ранжирования
  int idxObjOld=-1;//индекс суб обекта old
  //убрать старое
  for(int i=0; i<countAdr; i++)
    {
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/(REGISTERS_GOOS*PART_REGISTERS_GOOS);
      //поиск активного бита
      unsigned int *ranguvannja_goos = &edition_settings.ranguvannja_In_GOOSE[idxObj]
                                    [(offset%(REGISTERS_GOOS*PART_REGISTERS_GOOS))/REGISTERS_GOOS][0]; //Ранжування 

      if((offset/REGISTERS_GOOS)!=idxObjOld) {
       idxObjOld = offset/REGISTERS_GOOS;
       for(int dx=0; dx<N_SMALL; dx++) tmp[dx]=ranguvannja_goos[dx];//сохр старое ранж
      }//if

      int bitOld = getSequenceN_SMALLIndexActiveBit(offset%REGISTERS_GOOS, tmp);//индекс активного бита
      if(bitOld!=-1) {
        ranguvannja_goos[bitOld/32] &= ~(1<<(bitOld%32));
      }//if
    }//for
  //добавить новое
  for(int i=0; i<countAdr; i++)
    {
      unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//новое значение
      if(adr==0) continue;
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/(REGISTERS_GOOS*PART_REGISTERS_GOOS);
      //поиск активного бита
       unsigned int *ranguvannja_goos = &edition_settings.ranguvannja_In_GOOSE[idxObj]
                                    [(offset%(REGISTERS_GOOS*PART_REGISTERS_GOOS))/REGISTERS_GOOS][0]; //Ранжування 

      int bit = encoderN_SMALLACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для dv
      if(bit!=-1) {
        ranguvannja_goos[bit/32] |= (1<<(bit%32));
      }//if
    }//for

      //ВАЛИДАЦИЯ УСПЕШНА - УСТАНОВКА
     upravlSchematic = 1;//флаг Rang

  return 0;
}//

int privateGOOSBigGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGOOSBigGetReg2(int adrReg)
#endif
