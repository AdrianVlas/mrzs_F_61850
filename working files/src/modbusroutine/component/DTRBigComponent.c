#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 2220
//�������� ������� � ����� ������
#define END_ADR_REGISTER 2315

int privateDTRBigGetReg2(int adrReg);

int getDTRBigModbusRegister(int);//�������� ���������� ��������
int getDTRBigModbusBit(int);//�������� ���������� ����
int setDTRBigModbusRegister(int, int);// ��������
int setDTRBigModbusBit(int, int);// ����

void setDTRBigCountObject(void);//�������� �-�� �������
void preDTRBigReadAction(void);//action �� ������
void preDTRBigWriteAction(void);//action �� ������
int  postDTRBigWriteAction(void);//action ����� ������

int validDTRN_BIGACMD(unsigned short dataReg, int actControl);
int validBazaN_BIGACMD(unsigned short dataReg, int actControl);

COMPONENT_OBJ *dtrbigcomponent;

/**************************************/
//���������� ���������� �������
/**************************************/
void constructorDTRBigComponent(COMPONENT_OBJ *dtrbigcomp)
{
  dtrbigcomponent = dtrbigcomp;

  dtrbigcomponent->getModbusRegister = getDTRBigModbusRegister;//�������� ���������� ��������
  dtrbigcomponent->getModbusBit      = getDTRBigModbusBit;//�������� ���������� ����
  dtrbigcomponent->setModbusRegister = setDTRBigModbusRegister;// ��������
  dtrbigcomponent->setModbusBit      = setDTRBigModbusBit;// ����

  dtrbigcomponent->preReadAction   = preDTRBigReadAction;//action �� ������
  dtrbigcomponent->preWriteAction  = preDTRBigWriteAction;//action �� ������
  dtrbigcomponent->postWriteAction = postDTRBigWriteAction;//action ����� ������

  dtrbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getDTRBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateDTRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int offset = adrReg-BEGIN_ADR_REGISTER;
  int idxObj = offset/24;
//����� ��������� ����
  unsigned int *ranguvannja_dtr = &current_settings.ranguvannja_set_dt_source_plus[N_BIG*idxObj]; //���������� ������
  if(((offset%24)/6)==1)
       ranguvannja_dtr = &current_settings.ranguvannja_set_dt_source_minus[N_BIG*idxObj]; //���������� ��������� 
  if(((offset%24)/6)==2)
       ranguvannja_dtr = &current_settings.ranguvannja_reset_dt_source_plus[N_BIG*idxObj]; //���������� 
  if(((offset%24)/6)==3)
       ranguvannja_dtr = &current_settings.ranguvannja_reset_dt_source_minus[N_BIG*idxObj]; //���������� 

  int bit = getSequenceN_BIGIndexActiveBit(offset%6, ranguvannja_dtr);//������ ��������� ����
  if(bit!=-1)
    {
      int adr = decoderN_BIGACMD(bit);
      return adr<0? 0: adr+getACMDSmallBeginAdr();//������������ ������� ���� � ����� modbus  ��� rele
    }//if(bit!=N_BIG*32)
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int getDTRBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setDTRBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateDTRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(dtrbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings;//������ �����
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(dtrbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  //�������� �� ������������
  return validDTRN_BIGACMD(dataReg, (adrReg-BEGIN_ADR_REGISTER)/24);
}//getDOUTBigModbusRegister(int adrReg)
int setDTRBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preDTRBigReadAction(void) {
//action �� ������
  dtrbigcomponent->isActiveActualData = 1;
}//
void preDTRBigWriteAction(void) {
//action �� ������
  dtrbigcomponent->operativMarker[0] = -1;
  dtrbigcomponent->operativMarker[1] = -1;//����������� ������
  dtrbigcomponent->isActiveActualData = 1;
}//
int postDTRBigWriteAction(void) {
extern int upravlSchematic;//���� Rang
//action ����� ������
  int beginAdr = dtrbigcomponent->operativMarker[0];
  int endAdr   = dtrbigcomponent->operativMarker[1];
  if(beginAdr<0) return 0;//�� ���� ������
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray

  unsigned int tmp[N_BIG];//��� ���� ������������
  int idxObjOld=-1;//������ ��� ������ old
  //������ ������
  for(int i=0; i<countAdr; i++)
    {
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/24;

      unsigned int *ranguvannja_dtr = &edition_settings.ranguvannja_set_dt_source_plus[N_BIG*idxObj]; //���������� ������
      if(((offset%24)/6)==1)
          ranguvannja_dtr = &edition_settings.ranguvannja_set_dt_source_minus[N_BIG*idxObj]; //���������� ��������� 
      if(((offset%24)/6)==2)
          ranguvannja_dtr = &edition_settings.ranguvannja_reset_dt_source_plus[N_BIG*idxObj]; //���������� 
      if(((offset%24)/6)==3)
          ranguvannja_dtr = &edition_settings.ranguvannja_reset_dt_source_minus[N_BIG*idxObj]; //���������� 

      if((offset/6)!=idxObjOld) {
       idxObjOld = offset/6;
       for(int dx=0; dx<N_BIG; dx++) tmp[dx]=ranguvannja_dtr[dx];//���� ������ ����
      }//if

      int bitOld = getSequenceN_BIGIndexActiveBit(offset%6, tmp);//������ ��������� ����
      if(bitOld!=-1) {
        ranguvannja_dtr[bitOld/32] &= ~(1<<(bitOld%32));
      }//if
    }//for
  //�������� �����
  for(int i=0; i<countAdr; i++)
    {
      unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//����� ��������
      if(adr==0) continue;
      int offset = beginAdr-BEGIN_ADR_REGISTER+i;

      int idxObj = offset/24;

      unsigned int *ranguvannja_dtr = &edition_settings.ranguvannja_set_dt_source_plus[N_BIG*idxObj]; //���������� ������
      if(((offset%24)/6)==1)
          ranguvannja_dtr = &edition_settings.ranguvannja_set_dt_source_minus[N_BIG*idxObj]; //���������� ��������� 
      if(((offset%24)/6)==2)
          ranguvannja_dtr = &edition_settings.ranguvannja_reset_dt_source_plus[N_BIG*idxObj]; //���������� 
      if(((offset%24)/6)==3)
          ranguvannja_dtr = &edition_settings.ranguvannja_reset_dt_source_minus[N_BIG*idxObj]; //���������� 
      int bit = encoderN_BIGACMD(adr-getACMDSmallBeginAdr()); //���������� ������ modbus � ������ ���� ��� ����
      if(bit!=-1) {
        ranguvannja_dtr[bit/32] |= (1<<(bit%32));
      }//if
    }//for

      //��������� ������� - ���������
     upravlSchematic = 1;//���� Rang

//      current_settings = edition_settings;//��������� ���������

      //³������� ���� ���������
//      _SET_BIT(active_functions, RANG_SETTINGS_CHANGED);
      //����������� �������, �� �� ������ ����� �������� ����������
  //    new_state_keyboard |= (1<<BIT_REWRITE);
    //  fix_change_settings_m(1, 2);//1 - ����� ���������� 2 - USB
      //restart_timeout_idle_new_settings = true;

  return 0;
}//

int privateDTRBigGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int validDTRN_BIGACMD(unsigned short dataReg, int actControl)
{
 return validBazaN_BIGACMD(dataReg, actControl+1);
}//validDTRN_BIGACMD(unsigned short dataReg, int actControl)
