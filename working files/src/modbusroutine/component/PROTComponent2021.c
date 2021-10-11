
#include "header.h"

//начальный файл в карте памяти
#define BEGIN_FILE_REGISTER FILEPROT_BEGCOMPONENT2021
//конечный файл в карте памяти
#define END_FILE_REGISTER FILEPROT_ENDCOMPONENT2021

int privatePROTGetPerimetr(int fileReg);
int postPROTComponentAction21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket);
int setPROTComponentRegister21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket);
int getPROTComponentRegister20(int fileNumber, int recordNumber, int registerNumber, int recordLen);

int getFileCOMMRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileCOMMRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileCOMMRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileMTZRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileMTZRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileMTZRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileMTZ04Register20(int recordNumber, int registerNumber, int recordLen);
int setFileMTZ04Register21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileMTZ04Register21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileZDZRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZDZRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZDZRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileNZZRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileNZZRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileNZZRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileTZNPRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileTZNPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileTZNPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileZOPRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZOPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZOPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileZNMINRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZNMINRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZNMINRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileZNMAXRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileZNMAXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileZNMAXRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileAPVRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileAPVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileAPVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileUROVRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileUROVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileUROVRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileAJRRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileAJRRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileAJRRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

int getFileUPRegister20(int recordNumber, int registerNumber, int recordLen);
int setFileUPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);
int postFileUPRegister21(int recordNumber, int recordLen, unsigned short *dataPacket);

COMPONENT2021_OBJ *protcomponent2021;

void constructorPROTComponent2021(COMPONENT2021_OBJ *protcomp)
{
  protcomponent2021 = protcomp;

  protcomponent2021->getModbusRegister20func = getPROTComponentRegister20;//получить содержимое регистра
  protcomponent2021->setModbusRegister21func = setPROTComponentRegister21;//записать содержимое регистра
  protcomponent2021->postWriteAction21func = postPROTComponentAction21;//запись после пакета
}//constructorPROTComponent2021

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getPROTComponentRegister20(int fileNumber, int recordNumber, int registerNumber, int recordLen)
{
 //получить содержимое регистра
 if(privatePROTGetPerimetr(fileNumber) != 0) return MARKER_OUTPERIMETR;

 if(isActiveActualData2021)
 {
   edition_settings = current_settings_interfaces;//делаем копию
   isActiveActualData2021 = 0;//action до записи
 }//if(isActiveActualData)

 switch(fileNumber)
 {
  case FILEPROT_COMMREGISTER2021:
    return getFileCOMMRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPROT_MTZREGISTER2021:
    return getFileMTZRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPROT_MTZ04REGISTER2021:
    return getFileMTZ04Register20(recordNumber, registerNumber, recordLen); 
  case FILEPROT_ZDZREGISTER2021:
    return getFileZDZRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPROT_NZZREGISTER2021:
    return getFileNZZRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPROT_TZNPREGISTER2021:
    return getFileTZNPRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPROT_ZOPREGISTER2021:
    return getFileZOPRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPROT_ZNMINREGISTER2021:
    return getFileZNMINRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPROT_ZNMAXREGISTER2021:
    return getFileZNMAXRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPROT_APVREGISTER2021:
    return getFileAPVRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPROT_UROVREGISTER2021:
    return getFileUROVRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPROT_AJRREGISTER2021:
    return getFileAJRRegister20(recordNumber, registerNumber, recordLen); 
  case FILEPROT_UPREGISTER2021:
    return getFileUPRegister20(recordNumber, registerNumber, recordLen); 
 }//switch
 return MARKER_OUTPERIMETR;
}//getPROTComponentRegister20

////-SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET--SET-

int setPROTComponentRegister21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket)
{
 if(privatePROTGetPerimetr(fileNumber) != 0) return MARKER_OUTPERIMETR;

 if(isActiveActualData2021)
 {
   edition_settings = current_settings_interfaces;//делаем копию
   isActiveActualData2021 = 0;//action до записи
 }//if(isActiveActualData)

  switch(fileNumber)
  {
   case FILEPROT_COMMREGISTER2021:
     return setFileCOMMRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_MTZREGISTER2021:
     return setFileMTZRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_MTZ04REGISTER2021:
     return setFileMTZ04Register21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_ZDZREGISTER2021:
     return setFileZDZRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_NZZREGISTER2021:
     return setFileNZZRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_TZNPREGISTER2021:
     return setFileTZNPRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_ZOPREGISTER2021:
     return setFileZOPRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_ZNMINREGISTER2021:
     return setFileZNMINRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_ZNMAXREGISTER2021:
     return setFileZNMAXRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_APVREGISTER2021:
     return setFileAPVRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_UROVREGISTER2021:
     return setFileUROVRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_AJRREGISTER2021:
     return setFileAJRRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_UPREGISTER2021:
     return setFileUPRegister21(recordNumber, recordLen, dataPacket); 
  }//switch(fileNumber)

 return MARKER_ERRORPERIMETR;
}//setPROTComponentRegister21

////-POST--POST--POST--POST--POST--POST--POST--POST--POST--POST--POST-

int postPROTComponentAction21(int fileNumber, int recordNumber, int recordLen, unsigned short *dataPacket)
{
 if(privatePROTGetPerimetr(fileNumber) != 0) return MARKER_OUTPERIMETR;

 switch(fileNumber)
 {
   case FILEPROT_COMMREGISTER2021:
     return postFileCOMMRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_MTZREGISTER2021:
     return postFileMTZRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_MTZ04REGISTER2021:
     return postFileMTZ04Register21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_ZDZREGISTER2021:
     return postFileZDZRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_NZZREGISTER2021:
     return postFileNZZRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_TZNPREGISTER2021:
     return postFileTZNPRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_ZOPREGISTER2021:
     return postFileZOPRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_ZNMINREGISTER2021:
     return postFileZNMINRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_ZNMAXREGISTER2021:
     return postFileZNMAXRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_APVREGISTER2021:
     return postFileAPVRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_UROVREGISTER2021:
     return postFileUROVRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_AJRREGISTER2021:
     return postFileAJRRegister21(recordNumber, recordLen, dataPacket); 
   case FILEPROT_UPREGISTER2021:
     return postFileUPRegister21(recordNumber, recordLen, dataPacket); 
 }//switch(fileNumber)

 return MARKER_ERRORPERIMETR;
}//postPROTComponentAction21

int privatePROTGetPerimetr(int fileNumber)
{
  //проверить внешний периметр
 return controlPerimetr(fileNumber, BEGIN_FILE_REGISTER, END_FILE_REGISTER);
}//privatePROTGetPerimetr(int fileNumber)
