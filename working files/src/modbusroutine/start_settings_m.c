
#include "const_global.h"
#include "variables_external_m.h"

/**************************************/
//��������� ������������� �����������
/**************************************/
void global_component_installation(void) {
  constructorIUSmallComponent(&(config_array[IUSMALLCOMPONENT]));//���������� ���������� iu
  constructorUSTBigComponent(&(config_array[USTBIGCOMPONENT]));//���������� ���������� ust
  constructorACMDSmallComponent(&(config_array[ACMDSMALLCOMPONENT]));//���������� ���������� acmd
  constructorUPRBigComponent(&(config_array[UPRBIGCOMPONENT]));//���������� ���������� upr
  constructorANDBigComponent(&(config_array[ANDBIGCOMPONENT]));//���������� ���������� and
  constructorDOUTBigComponent(&(config_array[DOUTBIGCOMPONENT]));//���������� ���������� rele
  constructorDOUTSmallComponent(&(config_array[DOUTSMALLCOMPONENT]));//���������� ���������� dout
  constructorDTRBigComponent(&(config_array[DTRBIGCOMPONENT]));//���������� ���������� trg
  constructorDVBigComponent(&(config_array[DVBIGCOMPONENT]));//���������� ���������� dv
  constructorDVSmallComponent(&(config_array[DVSMALLCOMPONENT]));//���������� ���������� dv
  constructorGCMDSmallComponent(&(config_array[GCMDSMALLCOMPONENT]));//���������� ���������� gcmd
  constructorKEYBigComponent(&(config_array[KEYBIGCOMPONENT]));//���������� ���������� key
  constructorMFTBigComponent(&(config_array[MFTBIGCOMPONENT]));//���������� ���������� mft
  constructorNOTBigComponent(&(config_array[NOTBIGCOMPONENT]));//���������� ���������� not
  constructorORBigComponent(&(config_array[ORBIGCOMPONENT]));//���������� ���������� or
  constructorPKVBigComponent(&(config_array[PKVBIGCOMPONENT]));//���������� ���������� pkv
  constructorRAISmallComponent(&(config_array[RAISMALLCOMPONENT]));//���������� ���������� raismall
  constructorREGBigComponent(&(config_array[REGBIGCOMPONENT]));//���������� ���������� reg
  constructorPREGBigComponent(&(config_array[PREGBIGCOMPONENT]));//���������� ���������� reg
  constructorRPRBigComponent(&(config_array[RPRBIGCOMPONENT]));//���������� ���������� prpr
  constructorRPRSmallComponent(&(config_array[RPRSMALLCOMPONENT]));//���������� ���������� rpr
  constructorSDIBigComponent(&(config_array[SDIBIGCOMPONENT]));//���������� ���������� sdi
  constructorSDISmallComponent(&(config_array[SDISMALLCOMPONENT]));//���������� ���������� sdi
  constructorVVBigComponent(&(config_array[VVBIGCOMPONENT]));//���������� ���������� vv
  constructorXORBigComponent(&(config_array[XORBIGCOMPONENT]));//���������� ���������� xor
  constructorYustBigComponent(&(config_array[YUSTBIGCOMPONENT]));//���������� ���������� yust
  constructorAISmallComponent(&(config_array[AISMALLCOMPONENT]));//���������� ���������� aismall
  constructorQAISmallComponent(&(config_array[QAISMALLCOMPONENT]));//���������� ���������� qaismall
  constructorPFBigComponent(&(config_array[PFBIGCOMPONENT]));//���������� ���������� pf
#if (MODYFIKACIA_VERSII_PZ >= 10)
  constructorGOOSBigComponent(&(config_array[GOOSBIGCOMPONENT]));//���������� ���������� goos
  constructorMMSBigComponent(&(config_array[MMSBIGCOMPONENT]));//���������� ���������� mms
  constructorLANBigComponent(&(config_array[LANBIGCOMPONENT]));//���������� ���������� lan
#endif
}//global_vareiables_installation_bo(void)

