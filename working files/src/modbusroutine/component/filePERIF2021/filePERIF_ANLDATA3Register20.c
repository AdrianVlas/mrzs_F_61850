
#include "header.h"

int getFileANLDATA3Register20(int recordNumber, int registerNumber, int recordLen);

int openRegistratorFunc20(int number_file);
int superReaderFunc20(int offsetRegister, int fileNumber, int recordNumber, int recordLen);

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileANLDATA3Register20(int recordNumber, int registerNumber, int recordLen)
{
  int tmp1 = openRegistratorFunc20(FILENUMBER(FILEPERIF_ANLDATA3REGISTER20));//открыть данные регистратора AR DR
  if (tmp1!=0) return MARKER_ERRORPERIMETR;

  int result = superReaderFunc20(registerNumber, FILENUMBER(FILEPERIF_ANLDATA3REGISTER20), recordNumber, recordLen);

  return result;
}//getFileANLDATA3Register20


