
#include "const_global.h"
#include "variables_external_m.h"

/**************************************/
//��������� ������������� �����������
/**************************************/
void global_component_installation(void) {
  constructorIUSmallComponent(&(config_array[IUSMALLCOMPONENT]));//���������� ���������� iu
  constructorACMDSmallComponent(&(config_array[ACMDSMALLCOMPONENT]));//���������� ���������� acmd
  constructorDOUTSmallComponent(&(config_array[DOUTSMALLCOMPONENT]));//���������� ���������� dout
  constructorDVSmallComponent(&(config_array[DVSMALLCOMPONENT]));//���������� ���������� dv
  constructorGCMDSmallComponent(&(config_array[GCMDSMALLCOMPONENT]));//���������� ���������� gcmd
  constructorPKVBigComponent(&(config_array[PKVBIGCOMPONENT]));//���������� ���������� pkv
  constructorREG1BigComponent(&(config_array[REG1BIGCOMPONENT]));//���������� ���������� reg
  constructorREG2BigComponent(&(config_array[REG2BIGCOMPONENT]));//���������� ���������� reg
  constructorPREGBigComponent(&(config_array[PREGBIGCOMPONENT]));//���������� ���������� reg
  constructorRPRSmallComponent(&(config_array[RPRSMALLCOMPONENT]));//���������� ���������� rpr
  constructorSDISmallComponent(&(config_array[SDISMALLCOMPONENT]));//���������� ���������� sdi
  constructorYustBigComponent(&(config_array[YUSTBIGCOMPONENT]));//���������� ���������� yust
  constructorAISmallComponent(&(config_array[AISMALLCOMPONENT]));//���������� ���������� aismall
  constructorQAISmallComponent(&(config_array[QAISMALLCOMPONENT]));//���������� ���������� qaismall

  constructorUVVComponent2021(&(config_array2021func[UVVCOMPONENT2021]));//���������� ���������� UVVCOMPONENT2021
  constructorPROTComponent2021(&(config_array2021func[PROTCOMPONENT2021]));//���������� ���������� PROTCOMPONENT2021
  constructorPERIFComponent2021(&(config_array2021func[PERIFCOMPONENT2021]));//���������� ���������� PERIFCOMPONENT2021
}//global_vareiables_installation_bo(void)

