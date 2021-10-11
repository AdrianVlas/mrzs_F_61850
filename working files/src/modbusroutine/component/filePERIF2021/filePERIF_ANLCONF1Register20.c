
#include "header.h"

int getFileANLCONF1Register20(int recordNumber, int registerNumber, int recordLen);

int openRegistratorFunc20(int number_file);
int superReaderFunc20(int offsetRegister, int fileNumber, int recordNumber, int recordLen);
int configAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen);
int dataAnalogRegistrator(int offsetRegister, int recordNumber, int recordLen);
int configDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen);
int dataDiskretRegistrator(int offsetRegister, int recordNumber, int recordLen);

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileANLCONF1Register20(int recordNumber, int registerNumber, int recordLen)
{
  int tmp1 = openRegistratorFunc20(FILENUMBER(FILEPERIF_ANLCONF1REGISTER20));//открыть данные регистратора AR DR
  if (tmp1!=0) return MARKER_ERRORPERIMETR;

  int result = superReaderFunc20(registerNumber, FILENUMBER(FILEPERIF_ANLCONF1REGISTER20), recordNumber, recordLen);

  return result;
}//getFileANLCONF1Register20

