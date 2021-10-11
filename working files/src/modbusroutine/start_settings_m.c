
#include "const_global.h"
#include "variables_external_m.h"

/**************************************/
//Стартовая инициализация компонентов
/**************************************/
void global_component_installation(void) {
  constructorIUSmallComponent(&(config_array[IUSMALLCOMPONENT]));//подготовка компонента iu
  constructorACMDSmallComponent(&(config_array[ACMDSMALLCOMPONENT]));//подготовка компонента acmd
  constructorDOUTSmallComponent(&(config_array[DOUTSMALLCOMPONENT]));//подготовка компонента dout
  constructorDVSmallComponent(&(config_array[DVSMALLCOMPONENT]));//подготовка компонента dv
  constructorGCMDSmallComponent(&(config_array[GCMDSMALLCOMPONENT]));//подготовка компонента gcmd
  constructorPKVBigComponent(&(config_array[PKVBIGCOMPONENT]));//подготовка компонента pkv
  constructorREG1BigComponent(&(config_array[REG1BIGCOMPONENT]));//подготовка компонента reg
  constructorREG2BigComponent(&(config_array[REG2BIGCOMPONENT]));//подготовка компонента reg
  constructorPREGBigComponent(&(config_array[PREGBIGCOMPONENT]));//подготовка компонента reg
  constructorRPRSmallComponent(&(config_array[RPRSMALLCOMPONENT]));//подготовка компонента rpr
  constructorSDISmallComponent(&(config_array[SDISMALLCOMPONENT]));//подготовка компонента sdi
  constructorYustBigComponent(&(config_array[YUSTBIGCOMPONENT]));//подготовка компонента yust
  constructorAISmallComponent(&(config_array[AISMALLCOMPONENT]));//подготовка компонента aismall
  constructorQAISmallComponent(&(config_array[QAISMALLCOMPONENT]));//подготовка компонента qaismall

  constructorUVVComponent2021(&(config_array2021func[UVVCOMPONENT2021]));//подготовка компонента UVVCOMPONENT2021
  constructorPROTComponent2021(&(config_array2021func[PROTCOMPONENT2021]));//подготовка компонента PROTCOMPONENT2021
  constructorPERIFComponent2021(&(config_array2021func[PERIFCOMPONENT2021]));//подготовка компонента PERIFCOMPONENT2021
}//global_vareiables_installation_bo(void)

