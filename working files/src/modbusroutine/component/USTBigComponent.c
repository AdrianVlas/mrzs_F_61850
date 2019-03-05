#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 10300

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   
//�������� ������� � ����� ������
#define END_ADR_REGISTER 11900

#else
//�������� ������� � ����� ������
#define END_ADR_REGISTER 11395
#endif

int privateUSTBigGetReg2(int adrReg);

int getUSTBigModbusRegister(int);//�������� ���������� ��������
int getUSTBigModbusBit(int);//�������� ���������� ����
int setUSTBigModbusRegister(int, int);//�������� ���������� ��������
int setUSTBigModbusBit(int, int);//�������� ���������� ����

void setUSTBigCountObject(void);//�������� �-�� �������
void preUSTBigReadAction(void);//action �� ������
void preUSTBigWriteAction(void);//action �� ������
int  postUSTBigWriteAction(void);//action ����� ������
int  ustFunc000(int inOffset, int gruppa, int *multer, int regUst, uint32_t **editValue);
int grupa_ustavok_control(int  offset, int *grupa_ustavok, int *adresGruppa);

COMPONENT_OBJ *ustbigcomponent;

int ustFunc000(int inOffset, int gruppa, int *multer, int regUst, uint32_t **editValue)
{
  int diapazon = 1;
  int actControl = 0;
  if(inOffset<0)
  {
    inOffset = -inOffset;
    actControl = 1;
  }//if(inOffset<0)
  (*multer) = 10;
  switch(inOffset)
  {
  case 0:
    (*editValue) = (uint32_t*)&edition_settings.grupa_ustavok;
    (*multer) = 1;
    if(regUst<1 || regUst>4) diapazon=0;
    break;
  case 1:
    (*editValue) = (uint32_t*)&edition_settings.type_mtz1;
    (*multer) = 1;
    if(regUst<0 || regUst>2) diapazon=0;
    break;
  case 2:
    (*editValue) = (uint32_t*)&edition_settings.type_mtz2;
    (*multer) = 1;
    if(regUst<0 || regUst>7) diapazon=0;
    break;
  case 3:
    (*editValue) = (uint32_t*)&edition_settings.type_mtz3;
    (*multer) = 1;
    if(regUst<0 || regUst>7) diapazon=0;
    break;
  case 4:
    (*editValue) = (uint32_t*)&edition_settings.type_mtz4;
    (*multer) = 1;
    if(regUst<0 || regUst>2) diapazon=0;
    break;

//  int count_bit = 68;
  case 6:
    (*editValue) = (uint32_t*)&edition_settings.type_mtz04_2;
    (*multer) = 1;
    if(regUst<0 || regUst>3) diapazon=0;
    break;
//IF �������
  case 15:
    (*editValue) = &edition_settings.setpoint_mtz_1[gruppa];
    if(regUst<SETPOINT_MTZ1_MIN/10 || regUst>SETPOINT_MTZ1_MAX/10) diapazon=0;
    break;
  case 16:
    (*editValue) = &edition_settings.setpoint_mtz_1_n_vpered[gruppa];
    if(regUst<SETPOINT_MTZ1_N_VPERED_MIN/10 || regUst>SETPOINT_MTZ1_N_VPERED_MAX/10) diapazon=0;
    break;
  case 17:
    (*editValue) = &edition_settings.setpoint_mtz_1_n_nazad[gruppa];
    if(regUst<SETPOINT_MTZ1_N_NAZAD_MIN/10 || regUst>SETPOINT_MTZ1_N_NAZAD_MAX/10) diapazon=0;
    break;
  case 18:
    (*editValue) = &edition_settings.setpoint_mtz_1_po_napruzi[gruppa];
    if(regUst<SETPOINT_MTZ1_PO_NAPRUZI_MIN/10 || regUst>SETPOINT_MTZ1_PO_NAPRUZI_MAX/10) diapazon=0;
    break;
  case 19:
    (*editValue) = &edition_settings.setpoint_mtz_1_U[gruppa];
    if(regUst<SETPOINT_MTZ1_U_MIN/10 || regUst>SETPOINT_MTZ1_U_MAX/10) diapazon=0;
    break;
  case 20:
    (*multer) = 1;
    (*editValue) = &edition_settings.setpoint_mtz_1_angle[gruppa];
    if(regUst<SETPOINT_MTZ1_ANGLE_MIN || regUst>SETPOINT_MTZ1_ANGLE_MAX) diapazon=0;
    break;
  case 21:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_1[gruppa];
    if(regUst<TIMEOUT_MTZ1_MIN/10 || regUst>TIMEOUT_MTZ1_MAX/10) diapazon=0;
    break;
  case 22:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_1_n_vpered[gruppa];
    if(regUst<TIMEOUT_MTZ1_N_VPERED_MIN/10 || regUst>TIMEOUT_MTZ1_N_VPERED_MAX/10) diapazon=0;
    break;
  case 23:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_1_n_nazad[gruppa];
    if(regUst<TIMEOUT_MTZ1_N_NAZAD_MIN/10 || regUst>TIMEOUT_MTZ1_N_NAZAD_MAX/10) diapazon=0;
    break;
  case 24:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_1_po_napruzi[gruppa];
    if(regUst<TIMEOUT_MTZ1_PO_NAPRUZI_MIN/10 || regUst>TIMEOUT_MTZ1_PO_NAPRUZI_MAX/10) diapazon=0;
    break;

//  int count_bit = 68;
  case 27:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_mtz_2[gruppa];
    if(regUst<SETPOINT_MTZ2_MIN/10 || regUst>SETPOINT_MTZ2_MAX/10) diapazon=0;
    break;
  case 28:
    (*editValue) = &edition_settings.setpoint_mtz_2_n_vpered[gruppa];
    if(regUst<SETPOINT_MTZ2_N_VPERED_MIN/10 || regUst>SETPOINT_MTZ2_N_VPERED_MAX/10) diapazon=0;
    break;
  case 29:
    (*editValue) = &edition_settings.setpoint_mtz_2_n_nazad[gruppa];
    if(regUst<SETPOINT_MTZ2_N_NAZAD_MIN/10 || regUst>SETPOINT_MTZ2_N_NAZAD_MAX/10) diapazon=0;
    break;
  case 30:
    (*editValue) = &edition_settings.setpoint_mtz_2_po_napruzi[gruppa];
    if(regUst<SETPOINT_MTZ2_PO_NAPRUZI_MIN/10 || regUst>SETPOINT_MTZ2_PO_NAPRUZI_MAX/10) diapazon=0;
    break;
  case 31:
    (*editValue) = &edition_settings.setpoint_mtz_2_U[gruppa];
    if(regUst<SETPOINT_MTZ2_U_MIN/10 || regUst>SETPOINT_MTZ2_U_MAX/10) diapazon=0;
    break;
  case 32:
    (*multer) = 1;
    (*editValue) = &edition_settings.setpoint_mtz_2_angle[gruppa];
    if(regUst<SETPOINT_MTZ2_ANGLE_MIN || regUst>SETPOINT_MTZ2_ANGLE_MAX) diapazon=0;
    break;
  case 33:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2[gruppa];
    if(regUst<TIMEOUT_MTZ2_MIN/10 || regUst>TIMEOUT_MTZ2_MAX/10) diapazon=0;
    break;
  case 34:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_n_vpered[gruppa];
    if(regUst<TIMEOUT_MTZ2_N_VPERED_MIN/10 || regUst>TIMEOUT_MTZ2_N_VPERED_MAX/10) diapazon=0;
    break;
  case 35:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_n_nazad[gruppa];
    if(regUst<TIMEOUT_MTZ2_N_NAZAD_MIN/10 || regUst>TIMEOUT_MTZ2_N_NAZAD_MAX/10) diapazon=0;
    break;
  case 36:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_po_napruzi[gruppa];
    if(regUst<TIMEOUT_MTZ2_PO_NAPRUZI_MIN/10 || regUst>TIMEOUT_MTZ2_PO_NAPRUZI_MAX/10) diapazon=0;
    break;
  case 37:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_vvid_pr[gruppa];
    if(regUst<TIMEOUT_MTZ2_VVID_PR_MIN/10 || regUst>TIMEOUT_MTZ2_VVID_PR_MAX/10) diapazon=0;
    break;
  case 38:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_pr[gruppa];
    if(regUst<TIMEOUT_MTZ2_PR_MIN/10 || regUst>TIMEOUT_MTZ2_PR_MAX/10) diapazon=0;
    break;
  case 39:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_n_vpered_pr[gruppa];
    if(regUst<TIMEOUT_MTZ2_N_VPERED_PR_MIN/10 || regUst>TIMEOUT_MTZ2_N_VPERED_PR_MAX/10) diapazon=0;
    break;
  case 40:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_n_nazad_pr[gruppa];
    if(regUst<TIMEOUT_MTZ2_N_NAZAD_PR_MIN/10 || regUst>TIMEOUT_MTZ2_N_NAZAD_PR_MAX/10) diapazon=0;
    break;
  case 41:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_po_napruzi_pr[gruppa];
    if(regUst<TIMEOUT_MTZ2_PO_NAPRUZI_PR_MIN/10 || regUst>TIMEOUT_MTZ2_PO_NAPRUZI_PR_MAX/10) diapazon=0;
    break;

//  int count_bit = 68;
  case 45:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_mtz_3[gruppa];
    if(regUst<SETPOINT_MTZ3_MIN/10 || regUst>SETPOINT_MTZ3_MAX/10) diapazon=0;
    break;
  case 46:
    (*editValue) = &edition_settings.setpoint_mtz_3_n_vpered[gruppa];
    if(regUst<SETPOINT_MTZ3_N_VPERED_MIN/10 || regUst>SETPOINT_MTZ3_N_VPERED_MAX/10) diapazon=0;
    break;
  case 47:
    (*editValue) = &edition_settings.setpoint_mtz_3_n_nazad[gruppa];
    if(regUst<SETPOINT_MTZ3_N_NAZAD_MIN/10 || regUst>SETPOINT_MTZ3_N_NAZAD_MAX/10) diapazon=0;
    break;
  case 48:
    (*editValue) = &edition_settings.setpoint_mtz_3_po_napruzi[gruppa];
    if(regUst<SETPOINT_MTZ3_PO_NAPRUZI_MIN/10 || regUst>SETPOINT_MTZ3_PO_NAPRUZI_MAX/10) diapazon=0;
    break;
  case 49:
    (*editValue) = &edition_settings.setpoint_mtz_3_U[gruppa];
    if(regUst<SETPOINT_MTZ3_U_MIN/10 || regUst>SETPOINT_MTZ3_U_MAX/10) diapazon=0;
    break;
  case 50:
    (*multer) = 1;
    (*editValue) = &edition_settings.setpoint_mtz_3_angle[gruppa];
    if(regUst<SETPOINT_MTZ3_ANGLE_MIN || regUst>SETPOINT_MTZ3_ANGLE_MAX) diapazon=0;
    break;
  case 51:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_3[gruppa];
    if(regUst<TIMEOUT_MTZ3_MIN/10 || regUst>TIMEOUT_MTZ3_MAX/10) diapazon=0;
    break;
  case 52:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_3_n_vpered[gruppa];
    if(regUst<TIMEOUT_MTZ3_N_VPERED_MIN/10 || regUst>TIMEOUT_MTZ3_N_VPERED_MAX/10) diapazon=0;
    break;
  case 53:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_3_n_nazad[gruppa];
    if(regUst<TIMEOUT_MTZ3_N_NAZAD_MIN/10 || regUst>TIMEOUT_MTZ3_N_NAZAD_MAX/10) diapazon=0;
    break;
  case 54:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_3_po_napruzi[gruppa];
    if(regUst<TIMEOUT_MTZ3_PO_NAPRUZI_MIN/10 || regUst>TIMEOUT_MTZ3_PO_NAPRUZI_MAX/10) diapazon=0;
    break;

//  int count_bit = 68;
  case 63:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_mtz_4[gruppa];
    if(regUst<SETPOINT_MTZ4_MIN/10 || regUst>SETPOINT_MTZ4_MAX/10) diapazon=0;
    break;
  case 64:
    (*editValue) = &edition_settings.setpoint_mtz_4_n_vpered[gruppa];
    if(regUst<SETPOINT_MTZ4_N_VPERED_MIN/10 || regUst>SETPOINT_MTZ4_N_VPERED_MAX/10) diapazon=0;
    break;
  case 65:
    (*editValue) = &edition_settings.setpoint_mtz_4_n_nazad[gruppa];
    if(regUst<SETPOINT_MTZ4_N_NAZAD_MIN/10 || regUst>SETPOINT_MTZ4_N_NAZAD_MAX/10) diapazon=0;
    break;
  case 66:
    (*editValue) = &edition_settings.setpoint_mtz_4_po_napruzi[gruppa];
    if(regUst<SETPOINT_MTZ4_PO_NAPRUZI_MIN/10 || regUst>SETPOINT_MTZ4_PO_NAPRUZI_MAX/10) diapazon=0;
    break;
  case 67:
    (*editValue) = &edition_settings.setpoint_mtz_4_U[gruppa];
    if(regUst<SETPOINT_MTZ4_U_MIN/10 || regUst>SETPOINT_MTZ4_U_MAX/10) diapazon=0;
    break;
  case 68:
    (*multer) = 1;
    (*editValue) = &edition_settings.setpoint_mtz_4_angle[gruppa];
    if(regUst<SETPOINT_MTZ4_ANGLE_MIN || regUst>SETPOINT_MTZ4_ANGLE_MAX) diapazon=0;
    break;
  case 69:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_4[gruppa];
    if(regUst<TIMEOUT_MTZ4_MIN/10 || regUst>TIMEOUT_MTZ4_MAX/10) diapazon=0;
    break;
  case 70:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_4_n_vpered[gruppa];
    if(regUst<TIMEOUT_MTZ4_N_VPERED_MIN/10 || regUst>TIMEOUT_MTZ4_N_VPERED_MAX/10) diapazon=0;
    break;
  case 71:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_4_n_nazad[gruppa];
    if(regUst<TIMEOUT_MTZ4_N_NAZAD_MIN/10 || regUst>TIMEOUT_MTZ4_N_NAZAD_MAX/10) diapazon=0;
    break;
  case 72:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_4_po_napruzi[gruppa];
    if(regUst<TIMEOUT_MTZ4_PO_NAPRUZI_MIN/10 || regUst>TIMEOUT_MTZ4_PO_NAPRUZI_MAX/10) diapazon=0;
    break;

//  int count_bit = 68;
  case 75:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_mtz04_1[gruppa];
    if(regUst<SETPOINT_MTZ04_1_MIN/10 || regUst>SETPOINT_MTZ04_1_MAX/10) diapazon=0;
    break;
  case 76:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz04_1[gruppa];
    if(regUst<TIMEOUT_MTZ04_1_MIN/10 || regUst>TIMEOUT_MTZ04_1_MAX/10) diapazon=0;
    break;
  case 77:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_mtz04_2[gruppa];
    if(regUst<SETPOINT_MTZ04_2_MIN/10 || regUst>SETPOINT_MTZ04_2_MAX/10) diapazon=0;
    break;
  case 78:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz04_2[gruppa];
    if(regUst<TIMEOUT_MTZ04_2_MIN/10 || regUst>TIMEOUT_MTZ04_2_MAX/10) diapazon=0;
    break;
  case 79:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz04_2_vvid_pr[gruppa];
    if(regUst<TIMEOUT_MTZ04_2_VVID_PR_MIN/10 || regUst>TIMEOUT_MTZ04_2_VVID_PR_MAX/10) diapazon=0;
    break;
  case 80:
    (*editValue) = (uint32_t*)&edition_settings.timeout_mtz04_2_pr[gruppa];
    if(regUst<TIMEOUT_MTZ04_2_PR_MIN/10 || regUst>TIMEOUT_MTZ04_2_PR_MAX/10) diapazon=0;
    break;

//  count_bit = 2;
  case 82:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_zz_3I0[gruppa];
    if(regUst<SETPOINT_ZZ_3I0_MIN || regUst>SETPOINT_ZZ_3I0_MAX) diapazon=0;
    break;
  case 83:
    (*editValue) = (uint32_t*)&edition_settings.timeout_zz_3I0[gruppa];
    if(regUst<TIMEOUT_ZZ_3I0_MIN/10 || regUst>TIMEOUT_ZZ_3I0_MAX/10) diapazon=0;
    break;

//  count_bit = 2;
  case 86:
    (*editValue) = &edition_settings.setpoint_zz_3U0[gruppa];
    if(regUst<SETPOINT_ZZ_3U0_MIN/10 || regUst>SETPOINT_ZZ_3U0_MAX/10) diapazon=0;
    break;
  case 87:
    (*editValue) = (uint32_t*)&edition_settings.timeout_zz_3U0[gruppa];
    if(regUst<TIMEOUT_ZZ_3U0_MIN/10 || regUst>TIMEOUT_ZZ_3U0_MAX/10) diapazon=0;
    break;

//  count_bit = 1;
  case 89:
    (*editValue) = (uint32_t*)&edition_settings.timeout_nzz[gruppa];
    if(regUst<TIMEOUT_NZZ_MIN/10 || regUst>TIMEOUT_NZZ_MAX/10) diapazon=0;
    break;

//  count_bit = 1;
  case 92:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_1_3I0_vpered[gruppa];
    if(regUst<SETPOINT_TZNP1_3I0_VPERED_MIN/10 || regUst>SETPOINT_TZNP1_3I0_VPERED_MAX/10) diapazon=0;
    break;
  case 93:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_1_3U0_vpered[gruppa];
    if(regUst<SETPOINT_TZNP1_3U0_VPERED_MIN/10 || regUst>SETPOINT_TZNP1_3U0_VPERED_MAX/10) diapazon=0;
    break;
  case 94:
    (*editValue) = (uint32_t*)&edition_settings.timeout_tznp_1_vpered[gruppa];
    if(regUst<TIMEOUT_TZNP1_VPERED_MIN/10 || regUst>TIMEOUT_TZNP1_VPERED_MAX/10) diapazon=0;
    break;
  case 95:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_1_3I0_nazad[gruppa];
    if(regUst<SETPOINT_TZNP1_3I0_NAZAD_MIN/10 || regUst>SETPOINT_TZNP1_3I0_NAZAD_MAX/10) diapazon=0;
    break;
  case 96:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_1_3U0_nazad[gruppa];
    if(regUst<SETPOINT_TZNP1_3U0_NAZAD_MIN/10 || regUst>SETPOINT_TZNP1_3U0_NAZAD_MAX/10) diapazon=0;
    break;
  case 97:
    (*editValue) = (uint32_t*)&edition_settings.timeout_tznp_1_nazad[gruppa];
    if(regUst<TIMEOUT_TZNP1_NAZAD_MIN/10 || regUst>TIMEOUT_TZNP1_NAZAD_MAX/10) diapazon=0;
    break;
  case 98:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_1_angle[gruppa];
    if(regUst<SETPOINT_TZNP1_ANGLE_MIN || regUst>SETPOINT_TZNP1_ANGLE_MAX) diapazon=0;
    break;
  case 99:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_2_3I0_vpered[gruppa];
    if(regUst<SETPOINT_TZNP2_3I0_VPERED_MIN/10 || regUst>SETPOINT_TZNP2_3I0_VPERED_MAX/10) diapazon=0;
    break;
  case 100:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_2_3U0_vpered[gruppa];
    if(regUst<SETPOINT_TZNP2_3U0_VPERED_MIN/10 || regUst>SETPOINT_TZNP2_3U0_VPERED_MAX/10) diapazon=0;
    break;
  case 101:
    (*editValue) = (uint32_t*)&edition_settings.timeout_tznp_2_vpered[gruppa];
    if(regUst<TIMEOUT_TZNP2_VPERED_MIN/10 || regUst>TIMEOUT_TZNP2_VPERED_MAX/10) diapazon=0;
    break;
  case 102:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_2_3I0_nazad[gruppa];
    if(regUst<SETPOINT_TZNP2_3I0_NAZAD_MIN/10 || regUst>SETPOINT_TZNP2_3I0_NAZAD_MAX/10) diapazon=0;
    break;
  case 103:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_2_3U0_nazad[gruppa];
    if(regUst<SETPOINT_TZNP2_3U0_NAZAD_MIN/10 || regUst>SETPOINT_TZNP2_3U0_NAZAD_MAX/10) diapazon=0;
    break;
  case 104:
    (*editValue) = (uint32_t*)&edition_settings.timeout_tznp_2_nazad[gruppa];
    if(regUst<TIMEOUT_TZNP2_NAZAD_MIN/10 || regUst>TIMEOUT_TZNP2_NAZAD_MAX/10) diapazon=0;
    break;
  case 105:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_2_angle[gruppa];
    if(regUst<SETPOINT_TZNP2_ANGLE_MIN || regUst>SETPOINT_TZNP2_ANGLE_MAX) diapazon=0;
    break;
  case 106:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_3_3I0_vpered[gruppa];
    if(regUst<SETPOINT_TZNP3_3I0_VPERED_MIN/10 || regUst>SETPOINT_TZNP3_3I0_VPERED_MAX/10) diapazon=0;
    break;
  case 107:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_3_3U0_vpered[gruppa];
    if(regUst<SETPOINT_TZNP3_3U0_VPERED_MIN/10 || regUst>SETPOINT_TZNP3_3U0_VPERED_MAX/10) diapazon=0;
    break;
  case 108:
    (*editValue) = (uint32_t*)&edition_settings.timeout_tznp_3_vpered[gruppa];
    if(regUst<TIMEOUT_TZNP3_VPERED_MIN/10 || regUst>TIMEOUT_TZNP3_VPERED_MAX/10) diapazon=0;
    break;
  case 109:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_3_3I0_nazad[gruppa];
    if(regUst<SETPOINT_TZNP3_3I0_NAZAD_MIN/10 || regUst>SETPOINT_TZNP3_3I0_NAZAD_MAX/10) diapazon=0;
    break;
  case 110:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_3_3U0_nazad[gruppa];
    if(regUst<SETPOINT_TZNP3_3U0_NAZAD_MIN/10 || regUst>SETPOINT_TZNP3_3U0_NAZAD_MAX/10) diapazon=0;
    break;
  case 111:
    (*editValue) = (uint32_t*)&edition_settings.timeout_tznp_3_nazad[gruppa];
    if(regUst<TIMEOUT_TZNP3_NAZAD_MIN/10 || regUst>TIMEOUT_TZNP3_NAZAD_MAX/10) diapazon=0;
    break;
  case 112:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_3_angle[gruppa];
    if(regUst<SETPOINT_TZNP3_ANGLE_MIN || regUst>SETPOINT_TZNP3_ANGLE_MAX) diapazon=0;
    break;

  case 113:
    (*multer) = 1;
    (*editValue) = &edition_settings.setpoint_zop[gruppa];
    if(regUst<SETPOINT_ZOP_MIN || regUst>SETPOINT_ZOP_MAX) diapazon=0;
    break;
  case 114:
    (*editValue) = (uint32_t*)&edition_settings.timeout_zop[gruppa];
    if(regUst<TIMEOUT_ZOP_MIN/10 || regUst>TIMEOUT_ZOP_MAX/10) diapazon=0;
    break;

//  count_bit = 10;
  case 120:
    (*multer) = 100;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_Umin1[gruppa];
    if(regUst<SETPOINT_UMIN1_MIN/100 || regUst>SETPOINT_UMIN1_MAX/100) diapazon=0;
    break;
  case 121:
    (*editValue) = (uint32_t*)&edition_settings.timeout_Umin1[gruppa];
    if(regUst<TIMEOUT_UMIN1_MIN/10 || regUst>TIMEOUT_UMIN1_MAX/10) diapazon=0;
    break;
  case 122:
    (*multer) = 100;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_Umin2[gruppa];
    if(regUst<SETPOINT_UMIN2_MIN/100 || regUst>SETPOINT_UMIN2_MAX/100) diapazon=0;
    break;
  case 123:
    (*editValue) = (uint32_t*)&edition_settings.timeout_Umin2[gruppa];
    if(regUst<TIMEOUT_UMIN2_MIN/10 || regUst>TIMEOUT_UMIN2_MAX/10) diapazon=0;
    break;
  case 124:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_Umin1_Iblk[gruppa];
    if(regUst<SETPOINT_UMIN1_IBLK_MIN/10 || regUst>SETPOINT_UMIN1_IBLK_MAX/10) diapazon=0;
    break;
  case 125:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_Umin2_Iblk[gruppa];
    if(regUst<SETPOINT_UMIN2_IBLK_MIN/10 || regUst>SETPOINT_UMIN2_IBLK_MAX/10) diapazon=0;
    break;

  case 126:
    (*multer) = 100;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_Umax1[gruppa];
    if(regUst<SETPOINT_UMAX1_MIN/100 || regUst>SETPOINT_UMAX1_MAX/100) diapazon=0;
    break;
  case 127:
    (*editValue) = (uint32_t*)&edition_settings.timeout_Umax1[gruppa];
    if(regUst<TIMEOUT_UMAX1_MIN/10 || regUst>TIMEOUT_UMAX1_MAX/10) diapazon=0;
    break;
  case 128:
    (*multer) = 100;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_Umax2[gruppa];
    if(regUst<SETPOINT_UMAX2_MIN/100 || regUst>SETPOINT_UMAX2_MAX/100) diapazon=0;
    break;
  case 129:
    (*editValue) = (uint32_t*)&edition_settings.timeout_Umax2[gruppa];
    if(regUst<TIMEOUT_UMAX2_MIN/10 || regUst>TIMEOUT_UMAX2_MAX/10) diapazon=0;
    break;
  case 130:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_kp_Umax[gruppa];
    if(regUst<SETPOINT_KP_UMAX_MIN || regUst>SETPOINT_KP_UMAX_MAX) diapazon=0;
    break;

//  count_bit = 4;
  case 146:
    (*editValue) = &edition_settings.setpoint_urov[gruppa];
    if(regUst<SETPOINT_UROV_MIN/10 || regUst>SETPOINT_UROV_MAX/10) diapazon=0;
    break;
  case 147:
    (*editValue) = (uint32_t*)&edition_settings.timeout_urov_1[gruppa];
    if(regUst<TIMEOUT_UROV1_MIN/10 || regUst>TIMEOUT_UROV1_MAX/10) diapazon=0;
    break;
  case 148:
    (*editValue) = (uint32_t*)&edition_settings.timeout_urov_2[gruppa];
    if(regUst<TIMEOUT_UROV2_MIN/10 || regUst>TIMEOUT_UROV2_MAX/10) diapazon=0;
    break;

//  count_bit = 10;
  case 179:
    (*editValue) = (uint32_t*)&edition_settings.timeout_apv_block_vid_VV[gruppa];
    if(regUst<TIMEOUT_APV_BLOCK_VID_VV_MIN/10 || regUst>TIMEOUT_APV_BLOCK_VID_VV_MAX/10) diapazon=0;
    break;
  case 180:
  {
    (*editValue) = (uint32_t*)&edition_settings.timeout_apv_1[gruppa];
    if(regUst<TIMEOUT_APV1_MIN/10 || regUst>TIMEOUT_APV1_MAX/10) diapazon=0;
  }
  break;
  case 181:
    (*editValue) = (uint32_t*)&edition_settings.timeout_apv_2[gruppa];
    if(regUst<TIMEOUT_APV2_MIN/10 || regUst>TIMEOUT_APV2_MAX/10) diapazon=0;
    break;
  case 182:
    (*editValue) = (uint32_t*)&edition_settings.timeout_apv_3[gruppa];
    if(regUst<TIMEOUT_APV3_MIN/10 || regUst>TIMEOUT_APV3_MAX/10) diapazon=0;
    break;
  case 183:
    (*editValue) = (uint32_t*)&edition_settings.timeout_apv_4[gruppa];
    if(regUst<TIMEOUT_APV4_MIN/10 || regUst>TIMEOUT_APV4_MAX/10) diapazon=0;
    break;
  case 184:
    (*editValue) = (uint32_t*)&edition_settings.timeout_apv_block_vid_apv1[gruppa];
    if(regUst<TIMEOUT_APV_BLOCK_VID_APV1_MIN/10 || regUst>TIMEOUT_APV_BLOCK_VID_APV1_MAX/10) diapazon=0;
    break;
  case 185:
    (*editValue) = (uint32_t*)&edition_settings.timeout_apv_block_vid_apv2[gruppa];
    if(regUst<TIMEOUT_APV_BLOCK_VID_APV2_MIN/10 || regUst>TIMEOUT_APV_BLOCK_VID_APV2_MAX/10) diapazon=0;
    break;
  case 186:
    (*editValue) = (uint32_t*)&edition_settings.timeout_apv_block_vid_apv3[gruppa];
    if(regUst<TIMEOUT_APV_BLOCK_VID_APV3_MIN/10 || regUst>TIMEOUT_APV_BLOCK_VID_APV3_MAX/10) diapazon=0;
    break;
  case 187:
    (*editValue) = (uint32_t*)&edition_settings.timeout_apv_block_vid_apv4[gruppa];
    if(regUst<TIMEOUT_APV_BLOCK_VID_APV4_MIN/10 || regUst>TIMEOUT_APV_BLOCK_VID_APV4_MAX/10) diapazon=0;
    break;

//  count_bit = 9;
  case 195:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_achr1_f_rab[gruppa];
    if(regUst<SETPOINT_ACHR1_F_RAB_MIN/10 || regUst>SETPOINT_ACHR1_F_RAB_MAX/10) diapazon=0;
    break;
  case 196:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_achr2_f_rab[gruppa];
    if(regUst<SETPOINT_ACHR2_F_RAB_MIN/10 || regUst>SETPOINT_ACHR2_F_RAB_MAX/10) diapazon=0;
    break;
  case 197:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_chapv1_f_rab[gruppa];
    if(regUst<SETPOINT_CHAPV1_F_RAB_MIN/10 || regUst>SETPOINT_CHAPV1_F_RAB_MAX/10) diapazon=0;
    break;
  case 198:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_chapv2_f_rab[gruppa];
    if(regUst<SETPOINT_CHAPV2_F_RAB_MIN/10 || regUst>SETPOINT_CHAPV2_F_RAB_MAX/10) diapazon=0;
    break;
  case 199:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_achr_chapv_uf[gruppa];
    if(regUst<SETPOINT_ACHR_CHAPV_UF_MIN/10 || regUst>SETPOINT_ACHR_CHAPV_UF_MAX/10) diapazon=0;
    break;
  case 200:
    (*editValue) = (uint32_t*)&edition_settings.timeout_achr_1[gruppa];
    if(regUst<TIMEOUT_ACHR1_MIN/10 || regUst>TIMEOUT_ACHR1_MAX/10) diapazon=0;
    break;
  case 201:
    (*editValue) = (uint32_t*)&edition_settings.timeout_achr_2[gruppa];
    if(regUst<TIMEOUT_ACHR2_MIN/10 || regUst>TIMEOUT_ACHR2_MAX/10) diapazon=0;
    break;
  case 202:
    (*editValue) = (uint32_t*)&edition_settings.timeout_chapv_1[gruppa];
    if(regUst<TIMEOUT_CHAPV1_MIN/10 || regUst>TIMEOUT_CHAPV1_MAX/10) diapazon=0;
    break;
  case 203:
    (*editValue) = (uint32_t*)&edition_settings.timeout_chapv_2[gruppa];
    if(regUst<TIMEOUT_CHAPV2_MIN/10 || regUst>TIMEOUT_CHAPV2_MAX/10) diapazon=0;
    break;

//IF ������� 213-244

#define MARKER248  248
  case MARKER248:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.ctrl_zdz_type;
    if(regUst<0 || regUst>5) diapazon=0;
    break;
  case 249:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.timeout_zdz[gruppa];
    if(regUst<TIMEOUT_ZDZ_MIN || regUst>TIMEOUT_ZDZ_MAX) diapazon=0;
    break;

//  count_bit = 9;
  case 987:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.lines[0];
    if(regUst<NUMBER_LINES_FORWARD_MIN || regUst>NUMBER_LINES_MAX) diapazon=0;
    break;

//IF ������� 988-1003

  case 1004:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.lines[1];
    if(regUst<NUMBER_LINES_BACKWARD_MIN || regUst>NUMBER_LINES_MAX) diapazon=0;
    break;

//IF ������� 1005-1020

//  count_bit = 9;
  case 1021:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.TVoltage;
    if(regUst<KOEF_TN_MIN || regUst>KOEF_TN_MAX) diapazon=0;
    break;

//  count_bit = 9;
  case 1023:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.TCurrent;
    if(regUst<KOEF_TT_MIN || regUst>KOEF_TT_MAX) diapazon=0;
    break;
  case 1024:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.T0;
    if(regUst<KOEF_TO_MIN || regUst>KOEF_TO_MAX) diapazon=0;
    break;
  case 1025:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.TCurrent04;
    if(regUst<KOEF_TT04_MIN || regUst>KOEF_TT04_MAX) diapazon=0;
    break;

  case 1026:
    (*editValue) = (uint32_t*)&edition_settings.timeout_swch_udl_blk_on;
    if(regUst<TIMEOUT_SWCH_UDL_BLK_ON_MIN/10 || regUst>TIMEOUT_SWCH_UDL_BLK_ON_MAX/10) diapazon=0;
    break;
  case 1027:
    (*editValue) = (uint32_t*)&edition_settings.timeout_swch_on;
    if(regUst<TIMEOUT_SWCH_ON_MIN/10 || regUst>TIMEOUT_SWCH_ON_MAX/10) diapazon=0;
    break;
  case 1028:
    (*editValue) = (uint32_t*)&edition_settings.timeout_swch_off;
    if(regUst<TIMEOUT_SWCH_OFF_MIN/10 || regUst>TIMEOUT_SWCH_OFF_MAX/10) diapazon=0;
    break;
  case 1029:
    (*editValue) = (uint32_t*)&edition_settings.timeout_pryvoda_VV;
    if(regUst<TIMEOUT_PRYVODA_VV_MIN/10 || regUst>TIMEOUT_PRYVODA_VV_MAX/10) diapazon=0;
    break;

  case 1033:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_Inom;
    if(regUst<SETPOINT_Inom_MIN || regUst>SETPOINT_Inom_MAX) diapazon=0;
    break;
  case 1034:
    (*editValue) = (uint32_t*)&edition_settings.setpoint_r_kom_st_Inom;
    if(regUst<SETPOINT_RKS_Inom_MIN/10 || regUst>SETPOINT_RKS_Inom_MAX/10) diapazon=0;
    break;
  case 1035:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_Inom_vymk;
    if(regUst<SETPOINT_Inom_vymk_MIN || regUst>SETPOINT_Inom_vymk_MAX) diapazon=0;
    break;
  case 1036:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_r_kom_st_Inom_vymk;
    if(regUst<SETPOINT_RKS_Inom_vymk_MIN || regUst>SETPOINT_RKS_Inom_vymk_MAX) diapazon=0;
    break;
#define MARKER1037  1037
  case MARKER1037:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_pochatkovyj_resurs;
    break;
#define MARKER1038  1038
  case MARKER1038:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_pochatkovyj_resurs;
    break;
#define MARKER1039  1039
  case MARKER1039:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_krytychnyj_resurs;
    break;
#define MARKER1040  1040
  case MARKER1040:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_pochatkova_k_vymk;
    break;
#define MARKER1041  1041
  case MARKER1041:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.setpoint_pochatkova_k_vymk;
    break;

//  count_bit = 9;
#define MARKER1046  1046
  case MARKER1046:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.type_of_input_signal;
//      if(regUst&(~(0xFFFF))) diapazon=0;
    break;
  case 1047:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.type_of_input_signal;
    if(regUst&(~(0xF))) diapazon=0;
    break;
#define MARKER1048  1048
  case MARKER1048:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.type_of_input;
//      if(regUst&(~(0xFFFF))) diapazon=0;
    break;
  case 1049:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.type_of_input;
    if(regUst&(~(0xF))) diapazon=0;
    break;
  case 1050:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.type_of_output;
//      if(regUst&(~(0x3FF))) diapazon=0;
    break;
  case 1051:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.type_of_output_modif;
//      if(regUst&(~(0x3FF))) diapazon=0;
    break;
  case 1052:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.type_df;
    if(regUst&(~(0xFF))) diapazon=0;
    break;
#define MARKER1053  1053
  case MARKER1053:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.type_of_led;
//      if(regUst&(~(0xFFFF))) diapazon=0;
    break;
  case 1054:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.type_of_led;
    if(regUst&(~(0x1))) diapazon=0;
    break;
  case 1055:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.buttons_mode;
    if(regUst&(~(0xFFF))) diapazon=0;
    for(int bit=0; bit<NUMBER_DEFINED_BUTTONS; bit++)
    {
      if((regUst&(3<<(bit*2)))==(0x2<<(bit*2))) diapazon=0;
      if((regUst&(3<<(bit*2)))==(0x3<<(bit*2))) diapazon=0;
    }//for
    //���� ������ ���������� � ��� ������� ����� � ��������� ������ � ��������� ��
    if(actControl)
    {
      action_during_changing_button_mode(&current_settings_interfaces, &edition_settings);
      //���������� �� ��
      for (size_t number = 0; number < NUMBER_DEFINED_BUTTONS; number++)
      {
        for (unsigned int i = 0; i < N_SMALL; i++)
        {
          edition_settings.ranguvannja_buttons[N_SMALL*number + i] =
            current_settings_interfaces.ranguvannja_buttons[N_SMALL*number + i];
        }//for
      }//for
    }//if
    break;

//IF ������� 1056-1075
//IF ������� 1076-1083
//IF ������� 1086-1093

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   
  case 1600:
    (*multer) = 1;
    (*editValue) = (uint32_t*)&edition_settings.zdz_ovd_porig;
    if(regUst<0 || regUst>(ZDZ_CTRL_PORIG_N-1)) diapazon=0;
    break;
#endif

  }//switch
  if(inOffset>=7 && inOffset<15)
  {
    //��
    int item = inOffset-7;
    (*editValue) = (uint32_t*)&edition_settings.ctrl_UP_input[item];
    (*multer) = 1;
    if(regUst<0 || regUst>19) diapazon=0;
    if(actControl)
      if(regUst!=current_settings_interfaces.ctrl_UP_input[item])
      {
        uint32_t tmp = edition_settings.ctrl_UP_input[item];
        edition_settings.ctrl_UP_input[item] = -1;
        action_after_changing_input_UP(&edition_settings, item, tmp);
      }//if(actControl)
  }//if(inOffset>=7 && inOffset<15)
  if(inOffset>=213 && inOffset<245)
  {
    //��
    int item = inOffset-213;
    switch(item%4)
    {
    case 0:
    case 1:
      (*editValue) = (uint32_t*)&edition_settings.setpoint_UP[item/4][0][gruppa];
      break;
    case 2:
      (*editValue) = (uint32_t*)&edition_settings.timeout_UP[item/4][0][gruppa];
      break;
    case 3:
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_UP_KP[item/4][0][gruppa];
      break;
    }//switch

    if((item%4)==0 || (item%4)==1)
      switch(edition_settings.ctrl_UP_input[item/4])
      {
      case UP_CTRL_Ia_Ib_Ic:
      case UP_CTRL_Ia:
      case UP_CTRL_Ib:
      case UP_CTRL_Ic:
      case UP_CTRL_I1:
      case UP_CTRL_I2:
      case UP_CTRL_I04:
      case UP_CTRL_3I0_r:
        if((item%4)==1)
        {
          if(regUst!=0) diapazon=0;//��� �������� ��������
        }//if
        else if(regUst<SETPOINT_UP_I_MIN/10 || regUst>SETPOINT_UP_I_MAX/10) diapazon=0;
        break;
      case UP_CTRL_3I0:
      case UP_CTRL_3I0_others:
        (*multer) = 1;
        if((item%4)==1)
        {
          if(regUst!=0) diapazon=0;//��� �������� ��������
        }//if
        else if(regUst<SETPOINT_UP_3I0_MIN || regUst>SETPOINT_UP_3I0_MAX) diapazon=0;
        break;
      case UP_CTRL_Ua_Ub_Uc:
      case UP_CTRL_Ua:
      case UP_CTRL_Ub:
      case UP_CTRL_Uc:
      case UP_CTRL_U1:
      case UP_CTRL_U2:
      case UP_CTRL_3U0:
        (*multer) = 100;
        if((item%4)==1)
        {
          if(regUst!=0) diapazon=0;//��� �������� ��������
        }//if
        else if(regUst<SETPOINT_UP_U_MIN/100 || regUst>SETPOINT_UP_U_MAX/100) diapazon=0;
        break;
      case UP_CTRL_P:
      case UP_CTRL_Q:
      case UP_CTRL_S:
        (*multer) = 1;
        break;
      }//switch

    if((item%4)==2)
      if(regUst<TIMEOUT_UP_MIN/10 || regUst>TIMEOUT_UP_MAX/10) diapazon=0;
    if((item%4)==3)
    {
      if(edition_settings.control_UP& (1<<(2+3*(item/4))))
      {
        if(regUst<SETPOINT_UP_KP_LESS_MIN || regUst>SETPOINT_UP_KP_LESS_MAX) diapazon=0;
      }//if(edition_settings.control_UP& (1<<(2+3*item/4)))
      else  if(regUst<SETPOINT_UP_KP_MORE_MIN || regUst>SETPOINT_UP_KP_MORE_MAX) diapazon=0;
    }//if((item%4)==3)
  }//if(inOffset>=213 && inOffset<245)

//IF ������� 988-1003
  if(inOffset>=988 && inOffset<1021 && inOffset!=1004)
  {
  int item = 0;
  int direct = 0;
  if(inOffset>=988 && inOffset<1004)
  {
    (*multer) = 1;
    item = inOffset-988;
  }//if(inOffset>=988 && inOffset<1004)
  if(inOffset>=1005 && inOffset<1021)
  {
    (*multer) = 1;
    item = inOffset-1005;
    direct = 1;
  }//if(inOffset>=1005 && inOffset<1021)
  //unsigned int uRegUst = regUst&0xFFFF;
    switch(item%2)
    {
    case 0:
      (*editValue) = (uint32_t*)&edition_settings.dovgyna[direct][(item/2)%8];
      if(regUst<SETPOINT_DOVGYNA_VMP_MIN || regUst>SETPOINT_DOVGYNA_VMP_MAX) diapazon=0;
      break;
    case 1:
      (*editValue) = (uint32_t*)&edition_settings.opir[direct][(item/2)%8];
      if(regUst<SETPOINT_OPIR_VMP_MIN || regUst>SETPOINT_OPIR_VMP_MAX) diapazon=0;
      break;
    }//switch
  }//if(inOffset>=988 && inOffset<1021 && inOffset!=1004)

#if (MODYFIKACIA_VERSII_PZ == 0)
  if(inOffset>=1056 && inOffset<1076)
#endif
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 1) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   
    if(inOffset>=1056 && inOffset<1072)
#endif
#if (MODYFIKACIA_VERSII_PZ == 2)
      if(inOffset>=1056 && inOffset<1064)
#endif
      {
        int item = inOffset-1056;
        (*editValue) = (uint32_t*)&edition_settings.dopusk_dv[item];
        (*multer) = 1;
        if(regUst<0 || regUst>60) diapazon=0;
      }//if(inOffset>=1056 && inOffset<1076)

  if(inOffset>=1076 && inOffset<1084)
  {
    int item = inOffset-1076;
    (*editValue) = (uint32_t*)&edition_settings.timeout_pause_df[item];
    if(regUst<TIMEOUT_DF_PAUSE_MIN/10 || regUst>TIMEOUT_DF_PAUSE_MAX/10) diapazon=0;
  }//if(inOffset>=1076 && inOffset<1084)

  if(inOffset>=1086 && inOffset<1094)
  {
    int item = inOffset-1086;
    (*editValue) = (uint32_t*)&edition_settings.timeout_work_df[item];
    if(regUst<TIMEOUT_DF_WORK_MIN/10 || regUst>TIMEOUT_DF_WORK_MAX/10) diapazon=0;
  }//if(inOffset>=1076 && inOffset<1084)

  return diapazon;
} //ustFunc000(int inOffset, int gruppa, int *multer, int regUst, uint32_t **ustValue, uint32_t **editValue)

/**************************************/
//���������� ���������� UST
/**************************************/
void constructorUSTBigComponent(COMPONENT_OBJ *ustbigcomp)
{
  ustbigcomponent = ustbigcomp;

  ustbigcomponent->getModbusRegister = getUSTBigModbusRegister;//�������� ���������� ��������
  ustbigcomponent->getModbusBit      = getUSTBigModbusBit;//�������� ���������� ����
  ustbigcomponent->setModbusRegister = setUSTBigModbusRegister;//�������� ���������� ��������
  ustbigcomponent->setModbusBit      = setUSTBigModbusBit;//�������� ���������� ����

  ustbigcomponent->preReadAction   = preUSTBigReadAction;//action �� ������
  ustbigcomponent->preWriteAction  = preUSTBigWriteAction;//action �� ������
  ustbigcomponent->postWriteAction = postUSTBigWriteAction;//action ����� ������

  ustbigcomponent->isActiveActualData = 0;
}//prepareDVinConfig

int getUSTBigModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateUSTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(ustbigcomponent->isActiveActualData)
  {
    edition_settings = current_settings_interfaces;//������ �����
  }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  uint32_t *editValue=NULL;
  int  multer = 1;
  int  adresGruppa=0;
  int  grupa_ustavok=0;
  int  offset = adrReg-BEGIN_ADR_REGISTER;
  grupa_ustavok_control(offset, &grupa_ustavok, &adresGruppa);

  ustFunc000(offset -adresGruppa, grupa_ustavok, &multer, 0, &editValue);
  if(editValue==NULL) return 0;

  if(editValue == (uint32_t*)&edition_settings.type_mtz04_2 && (*editValue)!=0) return (*editValue)-2;
  for(int gruppa=0; gruppa<4; gruppa++)
  {
    for(int item=0; item<NUMBER_UP; item++)
    {
      if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][gruppa])
      {
        if((offset-adresGruppa)&1)
          return (((uint32_t)(*editValue))/multer)&0xFFFF;
        else
          return (((*editValue)/multer)>>16)&0xFFFF;
      }//if
    }//for(int item=0; item<NUMBER_UP; item++)
  }//for(int gruppa=0; gruppa<NUMBER_UP; gruppa++)
  if(editValue == (uint32_t*)&edition_settings.buttons_mode)
  {
    uint32_t buttonsmode = 0;
    for(int key=0; key<NUMBER_DEFINED_BUTTONS; key++)
    {
      if((*editValue) & (1<<key)) buttonsmode |= (1<<(2*key));
    }//for
    return (buttonsmode) &0xFFFF;
  }//if(editValue == (uint32_t*)&edition_settings.buttons_mode)
  if(editValue == (uint32_t*)&edition_settings.setpoint_pochatkovyj_resurs)
  {
    if(offset==MARKER1037)
    {
      return (*editValue) & (uint32_t)0xffff;
    }
    else
    {
      return ((*editValue)>>16)  & (uint32_t)0xffff;
    }//else
  }//if(editValue == (uint32_t*)&edition_settings.setpoint_pochatkovyj_resurs)
  if(editValue == (uint32_t*)&edition_settings.setpoint_pochatkova_k_vymk)
  {
    if(offset==1040)
    {
      return (*editValue) & (uint32_t)0xffff;
    }
    else
    {
      return ((*editValue)>>16)  & (uint32_t)0xffff;
    }//else
  }//if(editValue == (uint32_t*)&edition_settings.setpoint_pochatkova_k_vymk)
  if(editValue == (uint32_t*)&edition_settings.type_of_input_signal)
  {
#if MODYFIKACIA_VERSII_PZ == 0
    if(offset==MARKER1046)
    {
      return (*editValue) & (uint32_t)0xffff;
    }
    else
    {
      return ((*editValue)>>16)  & (uint32_t)0xffff;
    }//else
#else
    if(offset==MARKER1046)
    {
      return (*editValue) & (uint32_t)0xffff;
    }
    else
    {
      return 0;
    }//else

#endif
  }//if(editValue == (uint32_t*)&edition_settings.type_of_input_signal)
  if(editValue == (uint32_t*)&edition_settings.type_of_input)
  {
#if MODYFIKACIA_VERSII_PZ == 0
    if(offset==MARKER1048)
    {
      return (*editValue) & (uint32_t)0xffff;
    }
    else
    {
      return ((*editValue)>>16)  & (uint32_t)0xffff;
    }//else
#else
    if(offset==MARKER1048)
    {
      return (*editValue) & (uint32_t)0xffff;
    }
    else
    {
      return 0;
    }//else

#endif
  }//if(editValue == (uint32_t*)&edition_settings.type_of_input)
  if(editValue == (uint32_t*)&edition_settings.type_of_led)
  {
    if(offset==MARKER1053)
    {
      return (*editValue) & (uint32_t)0xffff;
    }
    else
    {
      return ((*editValue)>>16)  & (uint32_t)0xffff;
    }//else
  }//if(editValue == (uint32_t*)&edition_settings.type_of_led)

  return ((*editValue)/multer)&0xFFFF;
}//getDOUTBigModbusRegister(int adrReg)
int getUSTBigModbusBit(int x)
{
  //�������� ���������� ��������
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)
int setUSTBigModbusRegister(int adrReg, int dataReg)
{
  //�������� ���������� ��������
  if(privateUSTBigGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(ustbigcomponent->isActiveActualData)
  {
    edition_settings = current_settings_interfaces;//������ �����
  }//if(uprbigcomponent->isActiveActualData)
  superClearActiveActualData();

  superSetOperativMarker(ustbigcomponent, adrReg);
  superSetTempWriteArray(dataReg);//�������� � �����

  int  adresGruppa=0;
  int  grupa_ustavok=0;
  int  offset = adrReg-BEGIN_ADR_REGISTER;
  grupa_ustavok_control(offset, &grupa_ustavok, &adresGruppa);

  uint32_t *editValue=NULL;
  int  multer = 1;
  if(!ustFunc000(offset -adresGruppa, grupa_ustavok, &multer, dataReg, &editValue)) return MARKER_ERRORDIAPAZON;
#ifdef TESTZBIRKA_VERSII_PZ
  if(editValue==NULL) return MARKER_ERRORDIAPAZON;
#else
  if(editValue==NULL) return 0;
#endif

  return 0;
}//getDOUTBigModbusRegister(int adrReg)
int setUSTBigModbusBit(int x, int y)
{
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//getDOUTBigModbusRegister(int adrReg)

void preUSTBigReadAction(void)
{
//action �� ������
  ustbigcomponent->isActiveActualData = 1;
}//
void preUSTBigWriteAction(void)
{
//action �� ������
  ustbigcomponent->operativMarker[0] = -1;
  ustbigcomponent->operativMarker[1] = -1;//����������� ������
  ustbigcomponent->isActiveActualData = 1;
}//
int postUSTBigWriteAction(void)
{
  extern int upravlSetting;//���� Setting
//action ����� ������
  int beginAdr = ustbigcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//�� ���� ������
  int endAdr   = ustbigcomponent->operativMarker[1];
  int flag=0;
  int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//����� �������� TempWriteArray
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  int  grupa_ustavok=0;
  for(int i=0; i<countAdr; i++)
  {
    int  adresGruppa=0;
    int offset = i+beginAdr-BEGIN_ADR_REGISTER;
    uint32_t *editValue=NULL;
    int  multer = 1;
    int grupa_offset = grupa_ustavok_control(offset, &grupa_ustavok, &adresGruppa);
    ustFunc000(offset -adresGruppa, grupa_ustavok, &multer, 0, &editValue);
    if(editValue==NULL) continue;

    flag=1;

    uint32_t value = (unsigned short) tempWriteArray[offsetTempWriteArray+i];

//������ ������
    if(editValue == (uint32_t*)&edition_settings.type_of_input_signal)
    {
      if(offset==MARKER1046)
      {
        (*editValue) &= (uint32_t)~0xffff;
        (*editValue) |= (value & 0xffff);
        goto m1;
      }
      else
      {
        (*editValue)  &= (uint32_t)~(0xffff<<16);
        (*editValue)  |= ((value & 0xffff)<<16);//
        goto m1;
      }//else
    }//if(editValue == (uint32_t*)&edition_settings.type_of_input_signal)
    if(editValue == (uint32_t*)&edition_settings.type_of_input)
    {
      if(offset==MARKER1048)
      {
        (*editValue) &= (uint32_t)~0xffff;
        (*editValue) |= (value & 0xffff);
        goto m1;
      }
      else
      {
        (*editValue)  &= (uint32_t)~(0xffff<<16);
        (*editValue)  |= ((value & 0xffff)<<16);//
        goto m1;
      }//else
    }//if(editValue == (uint32_t*)&edition_settings.type_of_input)
    if(editValue == (uint32_t*)&edition_settings.type_of_led)
    {
      if(offset==MARKER1053)
      {
        (*editValue) &= (uint32_t)~0xffff;
        (*editValue) |= (value & 0xffff);
        goto m1;
      }
      else
      {
        (*editValue)  &= (uint32_t)~(0xffff<<16);
        (*editValue)  |= ((value & 0xffff)<<16);//
        goto m1;
      }//else
    }//if(editValue == (uint32_t*)&edition_settings.type_of_led)

    if(editValue == (uint32_t*)&edition_settings.type_mtz04_2)
    {
      if(value!=0) value += 2;
      (*editValue) = value*multer;
      goto m1;
    }//if(editValue == (uint32_t*)&edition_settings.type_mtz04_2)

    if(editValue == (uint32_t*)&edition_settings.buttons_mode)
    {
      for(int key=0; key<NUMBER_DEFINED_BUTTONS; key++)
      {
        if(value&(3<<(2*key))) (*editValue) |= (1<<key);
        else (*editValue) &= ~(1<<key);
      }//for
      goto m1;
    }//if(editValue == (uint32_t*)&edition_settings.buttons_mode)

    for(int item=0; item<NUMBER_UP; item++)
    {
      if(editValue == (uint32_t*)&edition_settings.timeout_UP[item][0][grupa_ustavok])
      {
        (*editValue) = multer*value;
        goto m1;
      }//if(editValue == (uint32_t*)&edition_settings.timeout_UP[item][0][grupa_ustavok])
    }//for(int item=0; item<NUMBER_UP; item++)

    if(editValue == (uint32_t*)&edition_settings.setpoint_pochatkovyj_resurs)
    {
      if(offset==MARKER1037)
      {
        (*editValue) &= (uint32_t)~0xffff;
        (*editValue) |= (value & 0xffff);
        goto m1;
      }
      else
      {
        (*editValue)  &= (uint32_t)~(0xffff<<16);
        (*editValue)  |= ((value & 0xffff)<<16);//
        goto m1;
      }//else
    }//if(editValue == (uint32_t*)&edition_settings.setpoint_pochatkovyj_resurs)
    if(editValue == (uint32_t*)&edition_settings.setpoint_pochatkova_k_vymk)
    {
      if(offset==MARKER1040)
      {
        (*editValue) &= (uint32_t)~0xffff;
        (*editValue) |= (value & 0xffff);
        goto m1;
      }
      else
      {
        (*editValue)  &= (uint32_t)~(0xffff<<16);
        (*editValue)  |= ((value & 0xffff)<<16);//
        goto m1;
      }//else
    }//if(editValue == (uint32_t*)&edition_settings.setpoint_pochatkova_k_vymk)

    if(editValue == (uint32_t*)&edition_settings.setpoint_tznp_1_angle[grupa_ustavok])
    {
      float temp_value_f = (float)value;
      edition_settings.setpoint_tznp_1_angle_cos[grupa_ustavok] = (int) (AMPLITUDA_FI*/*cos*/arm_cos_f32(/*(double)*/(PI*temp_value_f/180.0f)));
      edition_settings.setpoint_tznp_1_angle_sin[grupa_ustavok] = (int) (AMPLITUDA_FI*/*sin*/arm_sin_f32(/*(double)*/(PI*temp_value_f/180.0f)));
    }//if(editValue == (uint32_t*)&edition_settings.setpoint_tznp_1_angle[grupa_ustavok])
    if(editValue == (uint32_t*)&edition_settings.setpoint_tznp_2_angle[grupa_ustavok])
    {
      float temp_value_f = (float)value;
      edition_settings.setpoint_tznp_2_angle_cos[grupa_ustavok] = (int) (AMPLITUDA_FI*/*cos*/arm_cos_f32(/*(double)*/(PI*temp_value_f/180.0f)));
      edition_settings.setpoint_tznp_2_angle_sin[grupa_ustavok] = (int) (AMPLITUDA_FI*/*sin*/arm_sin_f32(/*(double)*/(PI*temp_value_f/180.0f)));
    }//if(editValue == (uint32_t*)&edition_settings.setpoint_tznp_1_angle[grupa_ustavok])
    if(editValue == (uint32_t*)&edition_settings.setpoint_tznp_3_angle[grupa_ustavok])
    {
      float temp_value_f = (float)value;
      edition_settings.setpoint_tznp_3_angle_cos[grupa_ustavok] = (int) (AMPLITUDA_FI*/*cos*/arm_cos_f32(/*(double)*/(PI*temp_value_f/180.0f)));
      edition_settings.setpoint_tznp_3_angle_sin[grupa_ustavok] = (int) (AMPLITUDA_FI*/*sin*/arm_sin_f32(/*(double)*/(PI*temp_value_f/180.0f)));
    }//if(editValue == (uint32_t*)&edition_settings.setpoint_tznp_1_angle[grupa_ustavok])

    if(editValue == (uint32_t*)&edition_settings.setpoint_mtz_1_angle[grupa_ustavok])
    {
      float temp_value_f = (float)value;
      edition_settings.setpoint_mtz_1_angle_cos[grupa_ustavok] = (int) (AMPLITUDA_FI*/*cos*/arm_cos_f32(/*(double)*/(PI*temp_value_f/180.0f)));
      edition_settings.setpoint_mtz_1_angle_sin[grupa_ustavok] = (int) (AMPLITUDA_FI*/*sin*/arm_sin_f32(/*(double)*/(PI*temp_value_f/180.0f)));
    }//if(editValue == (uint32_t*)&edition_settings.setpoint_mtz_1_angle[grupa_ustavok])
    if(editValue == (uint32_t*)&edition_settings.setpoint_mtz_2_angle[grupa_ustavok])
    {
      float temp_value_f = (float)value;
      edition_settings.setpoint_mtz_2_angle_cos[grupa_ustavok] = (int) (AMPLITUDA_FI*/*cos*/arm_cos_f32(/*(double)*/(PI*temp_value_f/180.0f)));
      edition_settings.setpoint_mtz_2_angle_sin[grupa_ustavok] = (int) (AMPLITUDA_FI*/*sin*/arm_sin_f32(/*(double)*/(PI*temp_value_f/180.0f)));
    }//if(editValue == (uint32_t*)&edition_settings.setpoint_mtz_2_angle[grupa_ustavok])
    if(editValue == (uint32_t*)&edition_settings.setpoint_mtz_3_angle[grupa_ustavok])
    {
      float temp_value_f = (float)value;
      edition_settings.setpoint_mtz_3_angle_cos[grupa_ustavok] = (int) (AMPLITUDA_FI*/*cos*/arm_cos_f32(/*(double)*/(PI*temp_value_f/180.0f)));
      edition_settings.setpoint_mtz_3_angle_sin[grupa_ustavok] = (int) (AMPLITUDA_FI*/*sin*/arm_sin_f32(/*(double)*/(PI*temp_value_f/180.0f)));
    }//if(editValue == (uint32_t*)&edition_settings.setpoint_mtz_3_angle[grupa_ustavok])
    if(editValue == (uint32_t*)&edition_settings.setpoint_mtz_4_angle[grupa_ustavok])
    {
      float temp_value_f = (float)value;
      edition_settings.setpoint_mtz_4_angle_cos[grupa_ustavok] = (int) (AMPLITUDA_FI*/*cos*/arm_cos_f32(/*(double)*/(PI*temp_value_f/180.0f)));
      edition_settings.setpoint_mtz_4_angle_sin[grupa_ustavok] = (int) (AMPLITUDA_FI*/*sin*/arm_sin_f32(/*(double)*/(PI*temp_value_f/180.0f)));
    }//if(editValue == (uint32_t*)&edition_settings.setpoint_mtz_4_angle[grupa_ustavok])

    for(int item=0; item<NUMBER_UP; item++)
    {
      if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][grupa_ustavok])
      {
        if((offset-grupa_offset*grupa_ustavok)&1)
        {
          //�� �������
          (*editValue) = value*multer;
          goto m1;
        }//if(offset&1)
        else
        {
          //�� �������
          switch(edition_settings.ctrl_UP_input[item])
          {
          case UP_CTRL_P:
          case UP_CTRL_Q:
          case UP_CTRL_S:
            (*editValue)  &= 0xffff;
            (*editValue)  |= ((value & 0xffff)<<16);//
            goto m1;
          default:
            goto m1;
          }//switch
        }//else
      }//if(editValue == (uint32_t*)&edition_settings.setpoint_UP[item][0][grupa_ustavok])
    }//for(int item=0; item<NUMBER_UP; item++)

    (*editValue) = value*multer;

m1:
    grupa_ustavok_control(offset, &grupa_ustavok, &adresGruppa);
    ustFunc000(-offset +adresGruppa, grupa_ustavok, &multer, value*multer, &editValue);
  }//for


//������ ��������
  int item=0;
  int temp1=0;

  for(; item<NUMBER_UP; item++)
  {
    temp1=edition_settings.setpoint_UP[item][0][grupa_ustavok];
    switch(edition_settings.ctrl_UP_input[item])
    {
    case UP_CTRL_P:
    case UP_CTRL_Q:
      if(temp1<0) temp1 = -temp1;//������
      if(temp1<SETPOINT_UP_P_MIN ||
          temp1>SETPOINT_UP_P_MAX
        ) return ERROR_VALID2;//������ ���������
      break;

    case UP_CTRL_S:
      if(temp1<SETPOINT_UP_P_MIN ||
          temp1>SETPOINT_UP_P_MAX
        ) return ERROR_VALID2;//������ ���������
      break;
    }//switch
  }//for(int item=0; item<NUMBER_UP; item++)

  for(int i=0; i<countAdr; i++)
  {
    int offset = i+beginAdr-BEGIN_ADR_REGISTER;
    if(offset==MARKER1037 || offset==MARKER1038 || offset==MARKER1039 || offset==MARKER1040 || offset==MARKER1041)
    {
      temp1=edition_settings.setpoint_pochatkovyj_resurs;
      unsigned int chastka = edition_settings.setpoint_r_kom_st_Inom/edition_settings.setpoint_r_kom_st_Inom_vymk;
      if(
        !(((unsigned int)temp1 >= (2*chastka)) && ((unsigned int)temp1 <= edition_settings.setpoint_r_kom_st_Inom))
      ) return ERROR_VALID2;//������ ���������
      temp1=edition_settings.setpoint_krytychnyj_resurs;
      if(
        !(((unsigned int)temp1 >= chastka) && ((unsigned int)temp1 <= 2*chastka))
      ) return ERROR_VALID2;//������ ���������

      temp1=edition_settings.setpoint_pochatkova_k_vymk;
      if(
        !((unsigned int)temp1 <= edition_settings.setpoint_r_kom_st_Inom)
      ) return ERROR_VALID2;//������ ���������
    }//if

//������ ��������

    if(offset==MARKER248)//(uint32_t*)&edition_settings.ctrl_zdz_type
    {
      temp1=edition_settings.ctrl_zdz_type;
      switch(temp1)
      {
      case 0://0-��� ��������
        break;
      case 1://1-� ��������� �� ����
        if(!(edition_settings.configuration&(1<<MTZ_BIT_CONFIGURATION))) return ERROR_VALID2;
        break;
      case 2://2-� ��������� �� ����������
        if(!(edition_settings.configuration&(1<<UMIN_BIT_CONFIGURATION))) return ERROR_VALID2;
        break;
      case 3://3-� ��������� �� ���� ��� �� ����������
        if(!(edition_settings.configuration&(1<<MTZ_BIT_CONFIGURATION) || edition_settings.configuration&(1<<UMIN_BIT_CONFIGURATION))) return ERROR_VALID2;
        break;
      case 4://4-� ��������� �� ���� � �� ����������
        if(!(edition_settings.configuration&(1<<MTZ_BIT_CONFIGURATION))) return ERROR_VALID2;
        if(!(edition_settings.configuration&(1<<UMIN_BIT_CONFIGURATION))) return ERROR_VALID2;
        break;
      case 5://5-� ��������� �� 3I0
        if(!(edition_settings.configuration&(1<<ZZ_BIT_CONFIGURATION))) return ERROR_VALID2;
        break;
      }//switch
    }//if(offset==MARKER248)//(uint32_t*)&edition_settings.ctrl_zdz_type

  }//for(int i=0; i<countAdr; i++)


  if(flag) upravlSetting = 1;//���� Setting

  return 0;
}//

int privateUSTBigGetReg2(int adrReg)
{
  //��������� ������� ��������
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int grupa_ustavok_control(int  offset, int *grupa_ustavok, int *adresGruppa)
{
  int  grupa_offset=243;
  int  ust_offset=15;
  if(offset>=(ust_offset+grupa_offset*0) && offset<=(ust_offset-1+grupa_offset*1))
  {
    *grupa_ustavok=0;
    *adresGruppa=0;
  }//if
  else if(offset>=(ust_offset+grupa_offset*1) && offset<=(ust_offset-1+grupa_offset*2))
  {
    *grupa_ustavok=1;
    *adresGruppa=grupa_offset*1;
  }//if
  else if(offset>=(ust_offset+grupa_offset*2) && offset<=(ust_offset-1+grupa_offset*3))
  {
    *grupa_ustavok=2;
    *adresGruppa=grupa_offset*2;
  }//if
  else if(offset>=(ust_offset+grupa_offset*3) && offset<=(ust_offset-1+grupa_offset*4))
  {
    *grupa_ustavok=3;
    *adresGruppa=grupa_offset*3;
  }//if
  return grupa_offset;
}//grupa_ustavok_control(int  offset, int *grupa_ustavok, int *adresGruppa)


