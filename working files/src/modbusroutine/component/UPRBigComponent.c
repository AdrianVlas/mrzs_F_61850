
#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 10000
//��������� bit � ����� ������
#define BEGIN_ADR_BIT 55000

//�������� ������� � ����� ������
#define END_ADR_REGISTER 10026
//�������� bit � ����� ������
#define END_ADR_BIT 55416

#define CLRVALID_DATA  0

int privateUPRBigGetReg2(int adrReg);
int privateUPRBigGetBit2(int adrReg);

int getUPRBigModbusRegister(int);//�������� ���������� ��������
int getUPRBigModbusBit(int);//�������� ���������� ����
int setUPRBigModbusRegister(int, int);//�������� ���������� ��������
int setUPRBigModbusBit(int, int);//�������� ���������� ����

void setUPRBigCountObject(void);//�������� �-�� �������
void preUPRBigReadAction(void);//action �� ������
void preUPRBigWriteAction(void);//action �� ������
int  postUPRBigWriteAction(void);//action ����� ������

int  uprFunc000(int inOffset, uint32_t *uprMaska, int validData, uint32_t **editControl);
void uprFuncRead000(int inOffset, uint32_t *uprMaska, uint32_t **editControl);
int  uprFuncValidWrite000(int inOffset, uint32_t *uprMaska, uint32_t **editControl);
int  uprFuncValid000(int inOffset, int validData);
int isValidCONFCondition(unsigned int confControl, unsigned int confMaska, uint32_t uprMaska, int validData);
int isValidEXTRACondition(unsigned int extraControl, unsigned int extraMaska, uint32_t uprMaska, int validData);
int isValidZZTYPECondition(unsigned int extraControl, unsigned int extraMaska, uint32_t uprMaska, int validData);

void loadUPRBigActualDataBit(int beginOffset, int endOffset);

COMPONENT_OBJ *uprbigcomponent;

void uprFuncRead000(int inOffset, uint32_t *uprMaska, uint32_t **editControl)
{
  uprFunc000(inOffset, uprMaska, CLRVALID_DATA, editControl);
}//uprFuncRead000(int inOffset, uint32_t *uprMaska)
int uprFuncValidWrite000(int inOffset, uint32_t *uprMaska, uint32_t **editControl)
{
  return uprFunc000(inOffset, uprMaska, CLRVALID_DATA, editControl);
}//uprFuncRead000(int inOffset, uint32_t *uprMaska)
int uprFuncValid000(int inOffset, int validData)
{
  uint32_t uprMaska=0;
  uint32_t *editControl=NULL;
  return uprFunc000(inOffset, &uprMaska, validData, &editControl);
}//uprFuncRead000(int inOffset, uint32_t *uprMaska)

int uprFunc000(int inOffset, uint32_t *uprMaska, int validData, uint32_t **editControl)
{
  int isValid = 1;
  int actControl = 0;
  if(inOffset<0)
    {
      inOffset = -inOffset;
      actControl = 1;
    }//if(inOffset<0)
  (*uprMaska) = 0xFFFFFFFF;
  switch(inOffset)
    {
//  int count_bit = 2;
    case 0:
      (*uprMaska)   = MTZ_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(!(edition_settings.configuration&(1<<MTZ_BIT_CONFIGURATION)))
            {
              //������� ���������� ������ ��� ��� ������������
              if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
            }//if
        }//if(actControl)
      break;
#define MTZ_CONFIGURATION_BEGIN 1
    case MTZ_CONFIGURATION_BEGIN:
      (*uprMaska)   = N_BIT_CTRMTZ_1;
      break;
    case 2:
      (*uprMaska)   = N_BIT_CTRMTZ_1_VPERED;
      break;
    case 3:
      (*uprMaska)   = N_BIT_CTRMTZ_1_NAZAD;
      break;

//  count_bit = 3;
    case 6:
      (*uprMaska)   = N_BIT_CTRMTZ_2;
      break;
    case 7:
      (*uprMaska)   = N_BIT_CTRMTZ_2_VPERED;
      break;
    case 8:
      (*uprMaska)   = N_BIT_CTRMTZ_2_NAZAD;
      break;
    case 9:
      (*uprMaska)   = N_BIT_CTRMTZ_2_PRYSKORENNJA;
      break;
    case 10:
      (*uprMaska)   = N_BIT_CTRMTZ_2_PRYSKORENA;
      break;

//  count_bit = 3;
    case 13:
      (*uprMaska)   = N_BIT_CTRMTZ_3;
      break;
    case 14:
      (*uprMaska)   = N_BIT_CTRMTZ_3_VPERED;
      break;
    case 15:
      (*uprMaska)   = N_BIT_CTRMTZ_3_NAZAD;
      break;

//  count_bit = 3;
    case 20:
      (*uprMaska)   = N_BIT_CTRMTZ_4;
      break;
    case 21:
      (*uprMaska)   = N_BIT_CTRMTZ_4_VPERED;
      break;
    case 22:
      (*uprMaska)   = N_BIT_CTRMTZ_4_NAZAD;
      break;

//  count_bit = 3;
#define MTZ_CONFIGURATION_END 25
    case MTZ_CONFIGURATION_END:
      (*uprMaska)   = N_BIT_CTRMTZ_NESPR_KIL_NAPR;
      break;

//  count_bit = 3;
    case 32:
      (*uprMaska)   = MTZ04_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(!(edition_settings.control_extra_settings_1&(1<<INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04))) isValid = 0;
          if(!(edition_settings.configuration&(1<<MTZ04_BIT_CONFIGURATION)))
            {
              //������� ���������� ������ ��� ��� ������������
              if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
            }//if
        }//if(actControl)
      break;
#define MTZ04_CONFIGURATION_BEGIN 33
    case 33:
      (*uprMaska)   = N_BIT_CTRMTZ04_1;
      break;
    case 34:
      (*uprMaska)   = N_BIT_CTRMTZ04_2;
      break;
    case 35:
      (*uprMaska)   = N_BIT_CTRMTZ04_2_PRYSKORENNJA;
      break;
#define MTZ04_CONFIGURATION_END 36
    case 36:
      (*uprMaska)   = N_BIT_CTRMTZ04_2_PRYSKORENA;
      break;

//  count_bit = 3;
    case 48:
      (*uprMaska)   = UP_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(!(edition_settings.configuration&(1<<UP_BIT_CONFIGURATION)))
            {
              //������� ���������� ������ ��� ��� ������������
              if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
            }//if
        }//if(actControl)
      break;

//IF ������� 49-72

//  count_bit = 3;
    case 96:
      (*uprMaska)   = ZDZ_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(!(edition_settings.configuration&(1<<ZDZ_BIT_CONFIGURATION)))
            {
              //������� ���������� ������ ��� ��� ������������
              if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
            }//if
        }//if(actControl)
      break;
#define ZDZ_CONFIGURATION_BEGIN 97
    case ZDZ_CONFIGURATION_BEGIN:
      (*uprMaska)   = CTR_ZDZ_STATE_BIT;
      break;
    case 98:
      (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_MTZ1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));//CTR_ZDZ_STARTED_FROM_MTZ1_BIT;
      if(isValidCONFCondition(**editControl, MTZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 99:
      (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_MTZ2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));//CTR_ZDZ_STARTED_FROM_MTZ2_BIT;
      if(isValidCONFCondition(**editControl, MTZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 100:
      (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_MTZ3_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));//CTR_ZDZ_STARTED_FROM_MTZ3_BIT;
      if(isValidCONFCondition(**editControl, MTZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 101:
      (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_MTZ4_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));//CTR_ZDZ_STARTED_FROM_MTZ4_BIT;
      if(isValidCONFCondition(**editControl, MTZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 102:
      (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_UMIN1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));//CTR_ZDZ_STARTED_FROM_UMIN1_BIT;
      if(isValidCONFCondition(**editControl, UMIN_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 103:
      (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_UMIN2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));//CTR_ZDZ_STARTED_FROM_UMIN2_BIT;
      if(isValidCONFCondition(**editControl, UMIN_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;

//123456
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   

    case 104:
      (*uprMaska)   = CTR_ZDZ_OVD1_STATE_BIT;
      break;
    case 105:
      (*uprMaska)   = CTR_ZDZ_OVD2_STATE_BIT;
      break;
#define ZDZ_CONFIGURATION_END 106
    case ZDZ_CONFIGURATION_END:
      (*uprMaska)   = CTR_ZDZ_OVD3_STATE_BIT;
      break;

//123456
#else
#define ZDZ_CONFIGURATION_END 103
#endif

//  count_bit = 2;
    case 112:
      (*uprMaska)   = ZZ_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(!(edition_settings.configuration&(1<<ZZ_BIT_CONFIGURATION)))
            {
              //������� ���������� ������ ��� ��� ������������
              if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
            }//if
        }//if(actControl)
      break;
#define ZZ_CONFIGURATION_BEGIN 113
    case ZZ_CONFIGURATION_BEGIN:
      (*uprMaska)   = INDEX_ML_CTR_ZZ1_3I0_STATE;
      break;

//  count_bit = 4;
    case 115:
      (*uprMaska)   = INDEX_ML_CTR_ZZ1_SECTOR;
      break;
    case 116:
      (*uprMaska)   = INDEX_ML_CTR_ZZ1_NZZ_STATE;
      if(isValidZZTYPECondition(**editControl, INDEX_ML_CTR_ZZ1_TYPE_BIT,
                                *uprMaska, validData)) isValid = 0;
      break;
    case 117:
      (*uprMaska)   = INDEX_ML_CTR_ZZ1_3U0_STATE;
      break;
#define ZZ_CONFIGURATION_END 118
    case 118:
      (*uprMaska)   = INDEX_ML_CTR_ZZ1_TYPE_BIT;
      if(actControl)
        {
          //������� ���������� ������ ��� ��� ���� ���� 33/3I0
          action_after_changing_zz1_type(&edition_settings);
        }//if(actControl)
      break;

//  count_bit = 3;
    case 128:
      (*uprMaska)   = TZNP_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if((edition_settings.control_extra_settings_1&(1<<INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04))) isValid = 0;
          if(!(edition_settings.configuration&(1<<TZNP_BIT_CONFIGURATION)))
            {
              //������� ���������� ������ ��� ��� ������������
              if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
            }//if
        }//if(actControl)
      break;
#define TZNP_CONFIGURATION_BEGIN 129
    case TZNP_CONFIGURATION_BEGIN:
      (*uprMaska)   = INDEX_ML_CTR_TZNP1;
      break;
    case 130:
      (*uprMaska)   = INDEX_ML_CTR_TZNP1_VPERED;
      break;
    case 131:
      (*uprMaska)   = INDEX_ML_CTR_TZNP1_NAZAD;
      break;
    case 132:
      (*uprMaska)   = INDEX_ML_CTR_TZNP2;
      break;
    case 133:
      (*uprMaska)   = INDEX_ML_CTR_TZNP2_VPERED;
      break;
    case 134:
      (*uprMaska)   = INDEX_ML_CTR_TZNP2_NAZAD;
      break;
    case 135:
      (*uprMaska)   = INDEX_ML_CTR_TZNP3;
      break;
    case 136:
      (*uprMaska)   = INDEX_ML_CTR_TZNP3_VPERED;
      break;
#define TZNP_CONFIGURATION_END 137
    case TZNP_CONFIGURATION_END:
      (*uprMaska)   = INDEX_ML_CTR_TZNP3_NAZAD;
      break;

//  count_bit = 3;
    case 144:
      (*uprMaska)   = ZOP_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(!(edition_settings.configuration&(1<<ZOP_BIT_CONFIGURATION)))
            {
              //������� ���������� ������ ��� ��� ������������
              if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
            }//if
        }//if(actControl)
      break;
    case 145:
      (*uprMaska)   = CTR_ZOP_STATE_BIT;
      (*editControl) = &edition_settings.control_zop;
      if(isValidCONFCondition(**editControl, ZOP_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;

//  count_bit = 1;
    case 160:
      (*uprMaska)   = ACHR_CHAPV_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(!(edition_settings.configuration&(1<<ACHR_CHAPV_BIT_CONFIGURATION)))
            {
              //������� ���������� ������ ��� ��� ������������
              if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
            }//if
        }//if(actControl)
      break;
#define ACHR_CHAPV_CONFIGURATION_BEGIN 161
    case ACHR_CHAPV_CONFIGURATION_BEGIN:
      (*uprMaska)   = INDEX_CTR_ACHR1;
      break;
    case 162:
      (*uprMaska)   = INDEX_CTR_ACHR2;
      break;
    case 163:
      (*uprMaska)   = INDEX_CTR_CHAPV1;
      break;
    case 164:
      (*uprMaska)   = INDEX_CTR_CHAPV2;
      break;
#define ACHR_CHAPV_CONFIGURATION_END 165
    case ACHR_CHAPV_CONFIGURATION_END:
      (*uprMaska)   = INDEX_CTR_CHAPV_VID_DV;
      break;

//  count_bit = 3;
    case 176:
      (*uprMaska)   = UMIN_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(!(edition_settings.configuration&(1<<UMIN_BIT_CONFIGURATION)))
            {
              //������� ���������� ������ ��� ��� ������������
              if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
            }//if
        }//if(actControl)
      break;
#define UMIN_CONFIGURATION_BEGIN 177
    case UMIN_CONFIGURATION_BEGIN:
      (*uprMaska)   = INDEX_CTR_UMIN1;
      break;
    case 178:
      (*uprMaska)   = INDEX_CTR_UMIN2;
      break;
    case 179:
      (*uprMaska)   = INDEX_CTR_PO_UMIN1_OR_AND;
      break;
    case 180:
      (*uprMaska)   = INDEX_CTR_PO_UMIN2_OR_AND;
      break;
    case 181:
      (*uprMaska)   = INDEX_CTR_UMIN1_UBLK;
      break;
    case 182:
      (*uprMaska)   = INDEX_CTR_UMIN2_UBLK;
      break;
    case 183:
      (*uprMaska)   = INDEX_CTR_UMIN1_IBLK;
      break;
#define UMIN_CONFIGURATION_END 184
    case UMIN_CONFIGURATION_END:
      (*uprMaska)   = INDEX_CTR_UMIN2_IBLK;
      break;

//  count_bit = 3;
    case 192:
      (*uprMaska)   = UMAX_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(!(edition_settings.configuration&(1<<UMAX_BIT_CONFIGURATION)))
            {
              //������� ���������� ������ ��� ��� ������������
              if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
            }//if
        }//if(actControl)
      break;
#define UMAX_CONFIGURATION_BEGIN 193
    case UMAX_CONFIGURATION_BEGIN:
      (*uprMaska)   = INDEX_CTR_UMAX1;
      break;
    case 194:
      (*uprMaska)   = INDEX_CTR_UMAX2;
      break;
    case 195:
      (*uprMaska)   = INDEX_CTR_PO_UMAX1_OR_AND;
      break;
#define UMAX_CONFIGURATION_END 196
    case UMAX_CONFIGURATION_END:
      (*uprMaska)   = INDEX_CTR_PO_UMAX2_OR_AND;
      break;

//  count_bit = 3;
    case 225:
      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE;
      (*editControl) = &edition_settings.control_extra_settings_1;
      if(actControl)
        {
          //������� ���������� ������
          if(action_after_changing_extra_settings(edition_settings.control_extra_settings_1, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;

//  count_bit = 3;
    case 227:
      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04;
      (*editControl) = &edition_settings.control_extra_settings_1;
      if(actControl)
        {
          //������� ���������� ������
          if(action_after_changing_extra_settings(edition_settings.control_extra_settings_1, &edition_settings)) isValid = 0;
        }//if(actControl)
      break;
    case 228:
      (*uprMaska)   = INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE;
      (*editControl) = &edition_settings.control_transformator;
      break;

//  count_bit = 3;
    case 288:
      (*uprMaska)   = APV_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(!(edition_settings.configuration&(1<<APV_BIT_CONFIGURATION)))
            {
              //������� ���������� ������ ��� ��� ������������
              if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
            }//if
        }//if(actControl)
      break;
#define APV_CONFIGURATION_BEGIN 289
    case APV_CONFIGURATION_BEGIN:
      (*uprMaska)   = INDEX_ML_CTRAPV_STAGE_1;
      break;
    case 290:
      (*uprMaska)   = INDEX_ML_CTRAPV_STAGE_2;
      break;
    case 291:
      (*uprMaska)   = INDEX_ML_CTRAPV_STAGE_3;
      break;
    case 292:
      (*uprMaska)   = INDEX_ML_CTRAPV_STAGE_4;
      break;
    case 293:
      (*uprMaska)   = INDEX_ML_CTRAPV_STARTED_FROM_MTZ1;
      if(isValidCONFCondition(**editControl, MTZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 294:
      (*uprMaska)   = INDEX_ML_CTRAPV_STARTED_FROM_MTZ2;
      if(isValidCONFCondition(**editControl, MTZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 295:
      (*uprMaska)   = INDEX_ML_CTRAPV_STARTED_FROM_MTZ3;
      if(isValidCONFCondition(**editControl, MTZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
#define APV_CONFIGURATION_END 296
    case APV_CONFIGURATION_END:
      (*uprMaska)   = INDEX_ML_CTRAPV_STARTED_FROM_MTZ4;
      if(isValidCONFCondition(**editControl, MTZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;


//  count_bit = 6;
    case 304:
      (*uprMaska)   = UROV_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(!(edition_settings.configuration&(1<<UROV_BIT_CONFIGURATION)))
            {
              //������� ���������� ������ ��� ��� ������������
              if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
            }//if
        }//if(actControl)
      break;
#define UROV_CONFIGURATION_BEGIN 305
    case UROV_CONFIGURATION_BEGIN:
      (*uprMaska)   = INDEX_ML_CTRUROV_STATE;
      break;
    case 306:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ1;
      if(isValidCONFCondition(**editControl, MTZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 307:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ2;
      if(isValidCONFCondition(**editControl, MTZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 308:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ3;
      if(isValidCONFCondition(**editControl, MTZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 309:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ4;
      if(isValidCONFCondition(**editControl, MTZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 310:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_1;
      if(isValidCONFCondition(**editControl, MTZ04_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 311:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_2;
      if(isValidCONFCondition(**editControl, MTZ04_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 312:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UMAX1;
      if(isValidCONFCondition(**editControl, UMAX_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 313:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UMAX2;
      if(isValidCONFCondition(**editControl, UMAX_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 314:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UMIN1;
      if(isValidCONFCondition(**editControl, UMIN_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 315:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UMIN2;
      if(isValidCONFCondition(**editControl, UMIN_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 316:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_ZOP1;
      if(isValidCONFCondition(**editControl, ZOP_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;

//  count_bit = 2;
    case 318:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_NZZ;
      if(isValidCONFCondition(**editControl, ZZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      if(isValidZZTYPECondition(**editControl, INDEX_ML_CTR_ZZ1_TYPE_BIT,
                                *uprMaska, validData)) isValid = 0;
      break;
    case 319:
      (*uprMaska)   =  INDEX_ML_CTRUROV_STARTED_FROM_3I0;
      if(isValidCONFCondition(**editControl, ZZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;

//  count_bit = 1;
    case 321:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_3U0;
      if(isValidCONFCondition(**editControl, ZZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 322:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_TZNP1;
      if(isValidCONFCondition(**editControl, TZNP_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 323:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_TZNP2;
      if(isValidCONFCondition(**editControl, TZNP_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 324:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_TZNP3;
      if(isValidCONFCondition(**editControl, TZNP_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 325:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_ZDZ;
      if(isValidCONFCondition(**editControl, ZDZ_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
    case 326:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_ACHR1;
      if(isValidCONFCondition(**editControl, ACHR_CHAPV_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;
#define UROV_CONFIGURATION_END 327
    case UROV_CONFIGURATION_END:
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_ACHR2;
      if(isValidCONFCondition(**editControl, ACHR_CHAPV_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;

//IF ������� 329-336

//  count_bit = 1;
    case 368:
      (*uprMaska)   = INDEX_ML_CTRPRYVOD_VV;
      (*editControl) = &edition_settings.control_switch;
      break;

//  count_bit = 1;
    case 371://���������� ���. �� �� ��
      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_BLK_ON_CB_MISCEVE;
      (*editControl) = &edition_settings.control_extra_settings_1;
      break;
    case 372://���������� ����. �� �� ��
      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_BLK_OFF_CB_MISCEVE;
      (*editControl) = &edition_settings.control_extra_settings_1;
      break;
    case 373://�������� ��
      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_FB_ACTIVATION;
      (*editControl) = &edition_settings.control_extra_settings_1;
      break;
    case 374://������ ��
      (*uprMaska)   = INDEX_ML_CTRRESURS_VV;
      (*editControl) = &edition_settings.control_switch;
      break;
    case 375://���������� ���������� ���������� � �� �� �����
      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_READY_TU;
      (*editControl) = &edition_settings.control_extra_settings_1;
      break;
    case 376://���� ���������� �� ���������� ��
      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_WINDOW_OFF_CB;
      (*editControl) = &edition_settings.control_extra_settings_1;
      break;

//  count_bit = 1;
    case 384:
      (*uprMaska)   = VMP_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(!(edition_settings.configuration&(1<<VMP_BIT_CONFIGURATION)))
            {
              if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
            }//if
        }//if(actControl)
      break;
    case 385:
      (*uprMaska)   = INDEX_CTR_VMP_STATE;
      (*editControl) = &edition_settings.control_vmp;
      if(isValidCONFCondition(**editControl, VMP_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      break;

//  count_bit = 1;
    case 416:
      (*uprMaska)   = EL_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
        {
          if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION)))
            {
              if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
            }//if
        }//if(actControl)
      break;

    }//switch

  if((*uprMaska)!=0xFFFFFFFF)
    {
      if(inOffset>=MTZ_CONFIGURATION_BEGIN && inOffset<=MTZ_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_mtz;
          if(isValidCONFCondition(**editControl, MTZ_BIT_CONFIGURATION, *uprMaska, validData)) isValid = 0;
        }//if

      if(inOffset>=MTZ04_CONFIGURATION_BEGIN && inOffset<=MTZ04_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_mtz04;
          if(isValidCONFCondition(**editControl, MTZ04_BIT_CONFIGURATION, *uprMaska, validData)) isValid = 0;
        }//if

      if(inOffset>=ZDZ_CONFIGURATION_BEGIN && inOffset<=ZDZ_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_zdz;
          if(isValidCONFCondition(**editControl, ZDZ_BIT_CONFIGURATION, *uprMaska, validData)) isValid = 0;
        }//if

      if(inOffset>=ZZ_CONFIGURATION_BEGIN && inOffset<=ZZ_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_zz;
          if(isValidCONFCondition(**editControl, ZZ_BIT_CONFIGURATION, *uprMaska, validData)) isValid = 0;
        }//if

      if(inOffset>=TZNP_CONFIGURATION_BEGIN && inOffset<=TZNP_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_tznp;
          if(isValidCONFCondition(**editControl, TZNP_BIT_CONFIGURATION, *uprMaska, validData)) isValid = 0;
        }//if

      if(inOffset>=ACHR_CHAPV_CONFIGURATION_BEGIN && inOffset<=ACHR_CHAPV_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_achr_chapv;
          if(isValidCONFCondition(**editControl, ACHR_CHAPV_BIT_CONFIGURATION, *uprMaska, validData)) isValid = 0;
        }//if

      if(inOffset>=UMIN_CONFIGURATION_BEGIN && inOffset<=UMIN_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_Umin;
          if(isValidCONFCondition(**editControl, UMIN_BIT_CONFIGURATION, *uprMaska, validData)) isValid = 0;
        }//if

      if(inOffset>=UMAX_CONFIGURATION_BEGIN && inOffset<=UMAX_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_Umax;
          if(isValidCONFCondition(**editControl, UMAX_BIT_CONFIGURATION, *uprMaska, validData)) isValid = 0;
        }//if

      if(inOffset>=APV_CONFIGURATION_BEGIN && inOffset<=APV_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_apv;
          if(isValidCONFCondition(**editControl, APV_BIT_CONFIGURATION, *uprMaska, validData)) isValid = 0;
        }//if

      if(inOffset>=UROV_CONFIGURATION_BEGIN && inOffset<=UROV_CONFIGURATION_END)
        {
          (*editControl) = &edition_settings.control_urov;
          if(isValidCONFCondition(**editControl, UROV_BIT_CONFIGURATION, *uprMaska, validData)) isValid = 0;
        }//if

    }//if((*uprMaska)!=-1)

  if(inOffset>=49 && inOffset<73)
    {
      (*uprMaska)   = inOffset-49;//maska;
      (*editControl) = &edition_settings.control_UP;
      if(isValidCONFCondition(**editControl, UP_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      if(actControl)
        {
          //������� ���������� ����������� ���������� ���� ���� ����������� ��
          action_after_changing_ctrl_UP(&edition_settings, edition_settings.control_UP);

        }//if(actControl)
    }//if

  if(inOffset>=329 && inOffset<337)
    {
      int inupOffset = inOffset-329;
      (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UP1+inupOffset;
      (*editControl) = &edition_settings.control_urov;
      if(isValidCONFCondition(**editControl, UROV_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
      if(isValidCONFCondition(**editControl, UP_BIT_CONFIGURATION,
                              *uprMaska, validData)) isValid = 0;
    }//if

  return isValid;
}//uprFunc000(int inOffset, uint32_t *uprMaska, uint32_t **editControl);

void loadUPRBigActualDataBit(int beginOffset, int endOffset)
{
  //ActualData
  for(int ii=0; ii<(1+(END_ADR_BIT-BEGIN_ADR_BIT+1)/16); ii++) tempReadArray[ii] = 0;
  for(int item=0; item<(END_ADR_BIT-BEGIN_ADR_BIT+1); item++)
    {
      int value =0;
      if(item>=beginOffset && item<endOffset)
        {
          uint32_t uprMaska=0;
          uint32_t *editControl=NULL;
          uprFuncRead000(item, &uprMaska, &editControl);
          if(editControl==NULL) value=0;
          else
            {
              //������ ��������
              value = (*editControl)&(1<<uprMaska);
            }//else
        }//if(item>=beginOffset && item<endOffset)

      int tsdata = 0;
      if(value) tsdata=1;
      tempReadArray[item/16] |= tsdata<<(item%16);
    }//for(int item=0; item<568; item++)
}//loadUPRBigActualDataBit(int beginOffset, int endOffset)

/**************************************/
//���������� ���������� UPR
/**************************************/
void constructorUPRBigComponent(COMPONENT_OBJ *uprbigcomp)
{
  uprbigcomponent = uprbigcomp;

  uprbigcomponent->getModbusRegister = getUPRBigModbusRegister;//�������� ���������� ��������
  uprbigcomponent->getModbusBit      = getUPRBigModbusBit;//�������� ���������� ����
  uprbigcomponent->setModbusRegister = setUPRBigModbusRegister;//�������� ���������� ��������
  uprbigcomponent->setModbusBit      = setUPRBigModbusBit;//�������� ���������� ����

  uprbigcomponent->preReadAction   = preUPRBigReadAction;//action �� ������
  uprbigcomponent->preWriteAction  = preUPRBigWriteAction;//action �� ������
  uprbigcomponent->postWriteAction = postUPRBigWriteAction;//action ����� ������

  uprbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getUPRBigModbusRegister(int adrReg)
{
  extern int globalcntReg;//�-��  ��� ������
  extern int globalbeginAdrReg;//����� ���
//�������� ���������� ��������
  if(privateUPRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  if(uprbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//������ �����
      int begin = globalbeginAdrReg-BEGIN_ADR_REGISTER;
      if(begin<0) globalcntReg += begin;

      int beginOffset = (adrReg-BEGIN_ADR_REGISTER)*16;
      int endOffset   = beginOffset +globalcntReg*16;

      loadUPRBigActualDataBit(beginOffset, endOffset); //ActualData
    }//if(acmdsmallcomponent->isActiveActualData)
  uprbigcomponent->isActiveActualData = 0;

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTBigModbusRegister(int adrReg)
int getUPRBigModbusBit(int adrBit)
{
  //�������� ���������� bit
  extern int globalcntBit;//�-�� ��� ��� ������
  extern int globalbeginAdrBit;//����� ��� ���
  //�������� ���������� bit
  if(privateUPRBigGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(uprbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//������ �����
      int begin = globalbeginAdrBit-BEGIN_ADR_BIT;
      if(begin<0) globalcntBit += begin;

      int beginOffset = adrBit-BEGIN_ADR_BIT;
      int endOffset   = adrBit-BEGIN_ADR_BIT +globalcntBit;
      loadUPRBigActualDataBit(beginOffset, endOffset); //ActualData
    }//if(acmdsmallcomponent->isActiveActualData)
  uprbigcomponent->isActiveActualData = 0;

  short tmp   = tempReadArray[(adrBit-BEGIN_ADR_BIT)/16];
  short maska = 1<<((adrBit-BEGIN_ADR_BIT)%16);
  if(tmp&maska) return 1;
  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setUPRBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateUPRBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(uprbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//������ �����
    }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(uprbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  int beginOffset = (adrReg-BEGIN_ADR_REGISTER)*16;
  for(int item=beginOffset; item<(beginOffset+16); item++)
    {
      int temp = 0;
      if(dataReg&(1<<(item-beginOffset))) temp=1;//��������� �� ���������
      if(temp)
        if(!uprFuncValid000(item, temp)) return MARKER_ERRORDIAPAZON;
    }//for

  return 0;
}//setUPRBigModbusRegister(int adrReg, int dataReg)
int setUPRBigModbusBit(int adrBit, int dataBit)
{
  //�������� ���������� bit
  if(privateUPRBigGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(uprbigcomponent->isActiveActualData)
    {
      edition_settings = current_settings_interfaces;//������ �����
    }//if(uprbigcomponent->isActiveActualData)
  uprbigcomponent->isActiveActualData = 0;

  superSetOperativMarker(uprbigcomponent, adrBit);
  superSetTempWriteArray(dataBit);//�������� � �����

  int offset = adrBit-BEGIN_ADR_BIT;
  if(dataBit)//��������� �� ���������
    if(!uprFuncValid000(offset, dataBit)) return MARKER_ERRORDIAPAZON;

  return 0;
}//getDOUTBigModbusRegister(int adrReg)

void preUPRBigReadAction(void)
{
//action �� ������
  uprbigcomponent->isActiveActualData = 1;
}//
void preUPRBigWriteAction(void)
{
//action �� ������
  uprbigcomponent->operativMarker[0] = -1;
  uprbigcomponent->operativMarker[1] = -1;//����������� ������
  uprbigcomponent->isActiveActualData = 1;
}//

int postUPRBigWriteAction(void)
{
  extern int upravlSetting;//���� Setting
  extern int upravlSchematic;//���� Rang
//action ����� ������
  int beginAdr = uprbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//�� ���� ������
  int endAdr   = uprbigcomponent->operativMarker[1];
  int flag=0;
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;
  if(beginAdr>=BEGIN_ADR_BIT)
    {
      //������ � ������
      int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_BIT);//����� �������� TempWriteArray
      for(int i=0; i<countAdr; i++)
        {
          uint32_t uprMaska=0;
          uint32_t *editControl=NULL;
          int offset = i+beginAdr-BEGIN_ADR_BIT;
          uprFuncValidWrite000(offset, &uprMaska, &editControl);
          if(editControl==NULL) continue;
          flag=1;
          uint32_t value = tempWriteArray[offsetTempWriteArray+i];

          //actControl
          if(value)//��������� �� ���������
            if(!uprFuncValidWrite000(-offset, &uprMaska, &editControl)) return ERROR_VALID3;//������ ���������
          //������ ������
          if((editControl == (uint32_t*)&edition_settings.control_extra_settings_1) ||
              (editControl == (uint32_t*)&edition_settings.configuration)
            )
            {
             if(!value)//��������� �� ����������
              if(!uprFuncValidWrite000(-offset, &uprMaska, &editControl)) return ERROR_VALID3;//������ ���������
              upravlSchematic = 1;//���� Rang
            }//if
          //��������� ���������
          (*editControl) &= ~(1<<uprMaska);
          (*editControl) |= (value<<uprMaska);
        }//for
    }//if(beginAdr>=BEGIN_ADR_BIT)
  else
    {
      //������ � ����������
      int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
      for(int i=0; i<countAdr; i++)
        {
          int offset = i+beginAdr-BEGIN_ADR_REGISTER;
          for(int bit=0; bit<16; bit++)
            {
              uint32_t uprMaska=0;
              uint32_t *editControl=NULL;
              uprFuncValidWrite000(offset*16+bit, &uprMaska, &editControl);
              if(editControl==NULL) continue;
              flag=1;
              uint32_t value = tempWriteArray[offsetTempWriteArray+i];
              int temp = 0;
              if(value&(1<<bit)) temp = 1;

              if(temp)//��������� �� ���������
                if(!uprFuncValidWrite000(-offset*16-bit, &uprMaska, &editControl)) return ERROR_VALID3;//������ ���������
              //������ ������
              if((editControl == (uint32_t*)&edition_settings.control_extra_settings_1) ||
                  (editControl == (uint32_t*)&edition_settings.configuration)
                )
                {
                 if(!temp)//��������� �� ����������
                  if(!uprFuncValidWrite000(-offset*16-bit, &uprMaska, &editControl)) return ERROR_VALID3;//������ ���������
                  upravlSchematic = 1;//���� Rang
                }//if
              //��������� ���������
              (*editControl) &= ~(1<<uprMaska);
              (*editControl) |= (temp<<uprMaska);
            }//for
        }//for
    }//else
  if(flag) upravlSetting = 1;//���� Setting

  return 0;
}//

int isValidCONFCondition(unsigned int confControl, unsigned int confMaska, uint32_t uprMaska, int validData)
{
  int temp = 0;
  if(confControl&(1<<uprMaska)) temp=1;
  if(validData!=temp)
    if(!(edition_settings.configuration&(1<<confMaska))) return 1;
  return 0;
}//isValidCONFCondition(unsigned int confControl, unsigned int confMaska, uint32_t uprMaska, int validData)
int isValidEXTRACondition(unsigned int extraControl, unsigned int extraMaska, uint32_t uprMaska, int validData)
{
  int temp = 0;
  if(extraControl&(1<<uprMaska)) temp=1;
  if(validData!=temp)
    if(edition_settings.control_extra_settings_1&(1<<extraMaska)) return 1;
  return 0;
}//isValidEXTRACondition(unsigned int extraControl, unsigned int extraMaska, uint32_t uprMaska, int validData)
int isValidZZTYPECondition(unsigned int extraControl, unsigned int extraMaska, uint32_t uprMaska, int validData)
{
  int temp = 0;
  if(extraControl&(1<<uprMaska)) temp=1;
  if(validData!=temp)
    if(edition_settings.control_zz&(1<<extraMaska)) return 1;
  return 0;
}//isValidZZTYPECondition(unsigned int extraControl, unsigned int extraMaska, uint32_t uprMaska, int validData)

int privateUPRBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateUPRBigGetBit2(int adrBit)
{
  //��������� ������� ��������
  return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)
