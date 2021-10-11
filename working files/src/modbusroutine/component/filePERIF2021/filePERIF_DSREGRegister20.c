
#include "header.h"

int dataRegistratorRDS(int registerNumber, int recordNumber, int recordLen);
int getFileDSREGRegister20(int recordNumber, int registerNumber, int recordLen);

////-GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET--GET-

int getFileDSREGRegister20(int recordNumber, int registerNumber, int recordLen)
{
  //Данные Регистратор дискретных сигналов
 return dataRegistratorRDS(registerNumber, recordNumber, recordLen);
}//getFileDSREGRegister20


