
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 500
//начальный bit в карте памяти
#define BEGIN_ADR_BIT 25000

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
//конечный регистр в карте памяти
#define END_ADR_REGISTER 580
//конечный bit в карте памяти
#define END_ADR_BIT 26283

#else

//конечный регистр в карте памяти
#define END_ADR_REGISTER 577
//конечный bit в карте памяти
#define END_ADR_BIT 26239
#endif

int privateACMDSmallGetReg2(int adrReg);
int privateACMDSmallGetBit2(int adrBit);

int getACMDSmallModbusRegister(int);//получить содержимое регистра
int getACMDSmallModbusBit(int);//получить содержимое бита
int setACMDSmallModbusRegister(int, int);//записать регистр
int setACMDSmallModbusBit(int, int);//записать бит

int  postACMDSmallWriteAction(void);//action после записи

int  cmdFunc000(int inOffset, int *outMaska, int *dvMaska, int actControl);
void loadACMDSmallActualDataBit(int cmdSwitch, int beginOffset, int endOffset);
unsigned int encoderValidN_BIGACMD(int offsetCMD, int *validCMD, int actControl);
unsigned int encoderValidN_SMALLACMD(int offsetCMD, int *validCMD);
int validBazaN_BIGACMD(unsigned short dataReg, int actControl);
int writeACMDSmallActualDataBit(int offset, int dataBit);
int passwordImunitetBitACMDSmallComponent(int adrReg);

void decoderN_BIGACMDArrayLoader(void);
void decoderN_SMALLACMDArrayLoader(void);

SRAM1 unsigned short decoderN_BIGACMDArray[N_BIG*32];//массив декодирования битов N_BIG
SRAM1 unsigned short decoderN_SMALLACMDArray[N_SMALL*32];//массив декодирования битов N_SMALL

#define CLRACT_CONTROL  0

SRAM1 COMPONENT_OBJ *acmdsmallcomponent;
int cmdFunc000(int inOffset, int *outMaska, int *dvMaska, int actControl)
{
  int isValid = 1;
  (*outMaska) = -1;
  switch(inOffset)//адр команд карты памяти modbus
  {
  case 0:
    (*outMaska) = RANG_1_GRUPA_USTAVOK;
    (*dvMaska)  = RANG_SMALL_1_GRUPA_USTAVOK;
    break;
  case 1:
    (*outMaska) = RANG_2_GRUPA_USTAVOK;
    (*dvMaska)  = RANG_SMALL_2_GRUPA_USTAVOK;
    break;
  case 2:
    (*outMaska) = RANG_3_GRUPA_USTAVOK;
    (*dvMaska)  = RANG_SMALL_3_GRUPA_USTAVOK;
    break;
  case 3:
    (*outMaska) = RANG_4_GRUPA_USTAVOK;
    (*dvMaska)  = RANG_SMALL_4_GRUPA_USTAVOK;
    break;

//  int count_bit = 3;
#define MTZ_CONFIGURATION_BEGIN 16
  case MTZ_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_BLOCK_MTZ1;
    (*dvMaska) = RANG_SMALL_BLOCK_MTZ1;
    break;
  case 17:
    (*outMaska) = RANG_MTZ1;
//        (*dvMaska) =
    break;
  case 18:
    (*outMaska) = RANG_PO_MTZ1;
//        (*dvMaska) =
    break;
  case 19:
    (*outMaska) = RANG_PO_MTZPN1;
//        (*dvMaska) =
    break;
  case 20:
    (*outMaska) = RANG_PO_MTZN1_VPERED;
//        (*dvMaska) =
    break;
  case 21:
    (*outMaska) = RANG_PO_MTZN1_NAZAD;
//        (*dvMaska) =
    break;
  case 22:
    (*outMaska) = RANG_SECTOR_VPERED_MTZN1;
//        (*dvMaska) =
    break;
  case 23:
    (*outMaska) = RANG_SECTOR_NAZAD_MTZN1;
//        (*dvMaska) =
    break;
  case 24:
    (*outMaska) = RANG_PO_U_MTZPN1;
//        (*dvMaska) =
    break;

//  count_bit = 3;
  case 32:
    (*outMaska) = RANG_BLOCK_MTZ2;
    (*dvMaska) = RANG_SMALL_BLOCK_MTZ2;
    break;
  case 33:
    (*outMaska) = RANG_MTZ2;
//        (*dvMaska) =
    break;
  case 34:
    (*outMaska) = RANG_PO_MTZ2;
//        (*dvMaska) =
    break;
  case 35:
    (*outMaska) = RANG_PO_MTZPN2;
//        (*dvMaska) =
    break;
  case 36:
    (*outMaska) = RANG_PO_MTZN2_VPERED;
//        (*dvMaska) =
    break;
  case 37:
    (*outMaska) = RANG_PO_MTZN2_NAZAD;
//        (*dvMaska) =
    break;
  case 38:
    (*outMaska) = RANG_SECTOR_VPERED_MTZN2;
//        (*dvMaska) =
    break;
  case 39:
    (*outMaska) = RANG_SECTOR_NAZAD_MTZN2;
//        (*dvMaska) =
    break;
  case 40:
    (*outMaska) = RANG_PO_U_MTZPN2;
//        (*dvMaska) =
    break;
  case 41:
    (*outMaska) = RANG_BLOCK_USK_MTZ2;
    (*dvMaska) = RANG_SMALL_BLOCK_USK_MTZ2;
    break;

//  count_bit = 3;
  case 48:
    (*outMaska) = RANG_BLOCK_MTZ3;
    (*dvMaska) = RANG_SMALL_BLOCK_MTZ3;
    break;
  case 49:
    (*outMaska) = RANG_MTZ3;
//        (*dvMaska) =
    break;
  case 50:
    (*outMaska) = RANG_PO_MTZ3;
//        (*dvMaska) =
    break;
  case 51:
    (*outMaska) = RANG_PO_MTZPN3;
//        (*dvMaska) =
    break;
  case 52:
    (*outMaska) = RANG_PO_MTZN3_VPERED;
//        (*dvMaska) =
    break;
  case 53:
    (*outMaska) = RANG_PO_MTZN3_NAZAD;
//        (*dvMaska) =
    break;
  case 54:
    (*outMaska) = RANG_SECTOR_VPERED_MTZN3;
//        (*dvMaska) =
    break;
  case 55:
    (*outMaska) = RANG_SECTOR_NAZAD_MTZN3;
//        (*dvMaska) =
    break;
  case 56:
    (*outMaska) = RANG_PO_U_MTZPN3;
//        (*dvMaska) =
    break;

//  count_bit = 3;
  case 64:
    (*outMaska) = RANG_BLOCK_MTZ4;
    (*dvMaska) = RANG_SMALL_BLOCK_MTZ4;
    break;
  case 65:
    (*outMaska) = RANG_MTZ4;
//        (*dvMaska) =
    break;
  case 66:
    (*outMaska) = RANG_PO_MTZ4;
//        (*dvMaska) =
    break;
  case 67:
    (*outMaska) = RANG_PO_MTZPN4;
//        (*dvMaska) =
    break;
  case 68:
    (*outMaska) = RANG_PO_MTZN4_VPERED;
//        (*dvMaska) =
    break;
  case 69:
    (*outMaska) = RANG_PO_MTZN4_NAZAD;
//        (*dvMaska) =
    break;
  case 70:
    (*outMaska) = RANG_SECTOR_VPERED_MTZN4;
//        (*dvMaska) =
    break;
  case 71:
    (*outMaska) = RANG_SECTOR_NAZAD_MTZN4;
//        (*dvMaska) =
    break;
#define MTZ_CONFIGURATION_END 72
  case MTZ_CONFIGURATION_END:
    (*outMaska) = RANG_PO_U_MTZPN4;
//        (*dvMaska) =
    break;

//  count_bit = 3;
#define MTZ04_CONFIGURATION_BEGIN 80
  case MTZ04_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_BLOCK_MTZ04_1;
    (*dvMaska) = RANG_SMALL_BLOCK_MTZ04_1;
    break;
  case 81:
    (*outMaska) = RANG_MTZ04_1;
//        (*dvMaska) =
    break;
  case 82:
    (*outMaska) = RANG_PO_MTZ04_1;
//        (*dvMaska) =
    break;
  case 83:
    (*outMaska) = RANG_BLOCK_MTZ04_2;
    (*dvMaska) = RANG_SMALL_BLOCK_MTZ04_2;
    break;
  case 84:
    (*outMaska) = RANG_MTZ04_2;
//        (*dvMaska) =
    break;
  case 85:
    (*outMaska) = RANG_PO_MTZ04_2;
//        (*dvMaska) =
    break;
#define MTZ04_CONFIGURATION_END 86
  case MTZ04_CONFIGURATION_END:
    (*outMaska) = RANG_BLOCK_USK_MTZ04_2;
    (*dvMaska) = RANG_SMALL_BLOCK_USK_MTZ04_2;
    break;

  case 96:
    (*outMaska) = RANG_NCN_MTZ;
//        (*dvMaska) =
    if(!(edition_settings.configuration&(1<<MTZ_BIT_CONFIGURATION))) isValid = 0;
    break;
  case 97:
    (*outMaska) = RANG_PO_BLOCK_U_MTZN;
//        (*dvMaska) =
    if(!(edition_settings.configuration&(1<<MTZ_BIT_CONFIGURATION))) isValid = 0;
    break;

#define  CMD_ITEM_HOLES97 if(item>97 && item<176) continue;

//  count_bit = 3;
#define ZDZ_CONFIGURATION_BEGIN 176
  case ZDZ_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_BLOCK_ZDZ;
    (*dvMaska)  = RANG_SMALL_BLOCK_ZDZ;
    break;
  case 177:
    (*outMaska) = RANG_PUSK_ZDZ_VID_DV;
    (*dvMaska)  = RANG_SMALL_PUSK_ZDZ_VID_DV;
    break;
  case 178:
    (*outMaska) = RANG_PO_ZDZ;
//        (*dvMaska) =
    break;
  case 179:
    (*outMaska) = RANG_ZDZ;
//        (*dvMaska) =
    break;
  case 180:
    (*outMaska) = RANG_LIGHT_ZDZ_FROM_DV;
//        (*dvMaska) =
    break;

#ifdef  MODYFIKACIA_VERSII_OVD_ZDZ
  case 181:
    (*outMaska) = RANG_LIGHT_ZDZ_FROM_OVD1;
//        (*dvMaska) =
    break;
  case 182:
    (*outMaska) = RANG_LIGHT_ZDZ_FROM_OVD2;
//        (*dvMaska) =
    break;
#define ZDZ_CONFIGURATION_END 183
  case ZDZ_CONFIGURATION_END:
    (*outMaska) = RANG_LIGHT_ZDZ_FROM_OVD3;
//        (*dvMaska) =
    break;
#else
#define ZDZ_CONFIGURATION_END 180
#endif

//  count_bit = 5;
#define ZZ_CONFIGURATION_BEGIN 192
  case ZZ_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_BLOCK_NZZ;
    (*dvMaska) = RANG_SMALL_BLOCK_NZZ;
    break;
  case 193:
    (*outMaska) = RANG_PO_NZZ;
//        (*dvMaska) =
    if(edition_settings.control_zz&(1<<INDEX_ML_CTR_ZZ1_TYPE_BIT)) isValid = 0;
    break;
  case 194:
    (*outMaska) = RANG_NZZ;
//        (*dvMaska) =
    if(edition_settings.control_zz&(1<<INDEX_ML_CTR_ZZ1_TYPE_BIT)) isValid = 0;
    break;
  case 195:
    (*outMaska) = RANG_PO_3I0;
//        (*dvMaska) =
    break;
  case 196:
    (*outMaska) = RANG_3I0;
//        (*dvMaska) =
    break;
  case 197:
    (*outMaska) = RANG_PO_3U0;
//        (*dvMaska) =
    break;
  case 198:
    (*outMaska) = RANG_3U0;
//        (*dvMaska) =
    break;
#define ZZ_CONFIGURATION_END 199
  case ZZ_CONFIGURATION_END:
    (*outMaska) = RANG_SECTOR_NZZ;
//        (*dvMaska) =
    if(edition_settings.control_zz&(1<<INDEX_ML_CTR_ZZ1_TYPE_BIT)) isValid = 0;
    break;

//  count_bit = 3;
#define TZNP_CONFIGURATION_BEGIN 224
  case TZNP_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_BLOCK_TZNP1;
    (*dvMaska) = RANG_SMALL_BLOCK_TZNP1;
    break;
  case 225:
    (*outMaska) = RANG_PO_3I0_TZNP1_VPERED;
//        (*dvMaska) =
    break;
  case 226:
    (*outMaska) = RANG_PO_3I0_TZNP1_NAZAD;
//        (*dvMaska) =
    break;
  case 227:
    (*outMaska) = RANG_PO_3U0_TZNP1_VPERED;
//        (*dvMaska) =
    break;
  case 228:
    (*outMaska) = RANG_PO_3U0_TZNP1_NAZAD;
//        (*dvMaska) =
    break;
  case 229:
    (*outMaska) = RANG_PO_TZNP1_VPERED;
//        (*dvMaska) =
    break;
  case 230:
    (*outMaska) = RANG_PO_TZNP1_NAZAD;
//        (*dvMaska) =
    break;
  case 231:
    (*outMaska) = RANG_SECTOR_TZNP1_VPERED;
//        (*dvMaska) =
    break;
  case 232:
    (*outMaska) = RANG_SECTOR_TZNP1_NAZAD;
//        (*dvMaska) =
    break;
  case 233:
    (*outMaska) = RANG_TZNP1;
//        (*dvMaska) =
    break;

//  count_bit = 3;
  case 240:
    (*outMaska) = RANG_BLOCK_TZNP2;
    (*dvMaska) = RANG_SMALL_BLOCK_TZNP2;
    break;
  case 241:
    (*outMaska) = RANG_PO_3I0_TZNP2_VPERED;
//        (*dvMaska) =
    break;
  case 242:
    (*outMaska) = RANG_PO_3I0_TZNP2_NAZAD;
//        (*dvMaska) =
    break;
  case 243:
    (*outMaska) = RANG_PO_3U0_TZNP2_VPERED;
//        (*dvMaska) =
    break;
  case 244:
    (*outMaska) = RANG_PO_3U0_TZNP2_NAZAD;
//        (*dvMaska) =
    break;
  case 245:
    (*outMaska) = RANG_PO_TZNP2_VPERED;
//        (*dvMaska) =
    break;
  case 246:
    (*outMaska) = RANG_PO_TZNP2_NAZAD;
//        (*dvMaska) =
    break;
  case 247:
    (*outMaska) = RANG_SECTOR_TZNP2_VPERED;
//        (*dvMaska) =
    break;
  case 248:
    (*outMaska) = RANG_SECTOR_TZNP2_NAZAD;
//        (*dvMaska) =
    break;
  case 249:
    (*outMaska) = RANG_TZNP2;
//        (*dvMaska) =
    break;

//  count_bit = 3;
  case 256:
    (*outMaska) = RANG_BLOCK_TZNP3;
    (*dvMaska) = RANG_SMALL_BLOCK_TZNP3;
    break;
  case 257:
    (*outMaska) = RANG_PO_3I0_TZNP3_VPERED;
//        (*dvMaska) =
    break;
  case 258:
    (*outMaska) = RANG_PO_3I0_TZNP3_NAZAD;
//        (*dvMaska) =
    break;
  case 259:
    (*outMaska) = RANG_PO_3U0_TZNP3_VPERED;
//        (*dvMaska) =
    break;
  case 260:
    (*outMaska) = RANG_PO_3U0_TZNP3_NAZAD;
//        (*dvMaska) =
    break;
  case 261:
    (*outMaska) = RANG_PO_TZNP3_VPERED;
//        (*dvMaska) =
    break;
  case 262:
    (*outMaska) = RANG_PO_TZNP3_NAZAD;
//        (*dvMaska) =
    break;
  case 263:
    (*outMaska) = RANG_SECTOR_TZNP3_VPERED;
//        (*dvMaska) =
    break;
  case 264:
    (*outMaska) = RANG_SECTOR_TZNP3_NAZAD;
//        (*dvMaska) =
    break;
#define TZNP_CONFIGURATION_END 265
  case TZNP_CONFIGURATION_END:
    (*outMaska) = RANG_TZNP3;
//        (*dvMaska) =
    break;

//  count_bit = 3;
#define ZOP_CONFIGURATION_BEGIN 288
  case ZOP_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_BLOCK_ZOP;
    (*dvMaska) = RANG_SMALL_BLOCK_ZOP;
    break;
  case 289:
    (*outMaska) = RANG_ZOP;
//        (*dvMaska) =
    break;
#define ZOP_CONFIGURATION_END 290
  case ZOP_CONFIGURATION_END:
    (*outMaska) = RANG_PO_ZOP;
//        (*dvMaska) =
    break;

//  count_bit = 3;
#define UMIN_CONFIGURATION_BEGIN 304
  case UMIN_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_BLOCK_UMIN1;
    (*dvMaska) = RANG_SMALL_BLOCK_UMIN1;
    break;
  case 305:
    (*outMaska) = RANG_PO_UBLK_UMIN1;
//        (*dvMaska) =
    break;
  case 306:
    (*outMaska) = RANG_PO_IBLK_UMIN1;
//        (*dvMaska) =
    break;
  case 307:
    (*outMaska) = RANG_UMIN1;
//        (*dvMaska) =
    break;
  case 308:
    (*outMaska) = RANG_PO_UMIN1;
//        (*dvMaska) =
    break;
  case 309:
    (*outMaska) = RANG_START_UMIN1;
    (*dvMaska)  = RANG_SMALL_START_UMIN1;
    break;

//  count_bit = 3;
  case 320:
    (*outMaska) = RANG_BLOCK_UMIN2;
    (*dvMaska)  = RANG_SMALL_BLOCK_UMIN2;
    break;
  case 321:
    (*outMaska) = RANG_PO_UBLK_UMIN2;
//        (*dvMaska) =
    break;
  case 322:
    (*outMaska) = RANG_PO_IBLK_UMIN2;
//        (*dvMaska) =
    break;
  case 323:
    (*outMaska) = RANG_UMIN2;
//        (*dvMaska) =
    break;
  case 324:
    (*outMaska) = RANG_PO_UMIN2;
//        (*dvMaska) =
    break;
#define UMIN_CONFIGURATION_END 325
  case UMIN_CONFIGURATION_END:
    (*outMaska) = RANG_START_UMIN2;
    (*dvMaska) = RANG_SMALL_START_UMIN2;
    break;

//  count_bit = 3;
#define UMAX_CONFIGURATION_BEGIN 368
  case UMAX_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_BLOCK_UMAX1;
    (*dvMaska)  = RANG_SMALL_BLOCK_UMAX1;
    break;
  case 369:
    (*outMaska) = RANG_UMAX1;
//        (*dvMaska) =
    break;
  case 370:
    (*outMaska) = RANG_PO_UMAX1;
//        (*dvMaska) =
    break;

//  count_bit = 3;
  case 384:
    (*outMaska) = RANG_BLOCK_UMAX2;
    (*dvMaska)  = RANG_SMALL_BLOCK_UMAX2;
    break;
  case 385:
    (*outMaska) = RANG_UMAX2;
//        (*dvMaska) =
    break;
#define UMAX_CONFIGURATION_END 386
  case UMAX_CONFIGURATION_END:
    (*outMaska) = RANG_PO_UMAX2;
//        (*dvMaska) =
    break;

//  count_bit = 3;
#define APV_CONFIGURATION_BEGIN 496
  case APV_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_STAT_BLK_APV;
    (*dvMaska) = RANG_SMALL_STAT_BLK_APV;
    break;

//  count_bit = 3;
  case 498:
    (*outMaska) = RANG_APV1;
//        (*dvMaska) =
    break;
  case 499:
    (*outMaska) = RANG_APV2;
//        (*dvMaska) =
    break;
  case 500:
    (*outMaska) = RANG_APV3;
//        (*dvMaska) =
    break;
  case 501:
    (*outMaska) = RANG_APV4;
//        (*dvMaska) =
    break;
#define APV_CONFIGURATION_END 502
  case APV_CONFIGURATION_END:
    (*outMaska) = RANG_APV_WORK;
//        (*dvMaska) =
    break;

//  count_bit = 5;
#define UROV_CONFIGURATION_BEGIN 512
  case UROV_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_PUSK_UROV_VID_DV;
    (*dvMaska) = RANG_SMALL_PUSK_UROV_VID_DV;
    break;
  case 513:
    (*outMaska) = RANG_UROV1;
//        (*dvMaska) =
    break;
  case 514:
    (*outMaska) = RANG_UROV2;
//        (*dvMaska) =
    break;
#define UROV_CONFIGURATION_END 515
  case UROV_CONFIGURATION_END:
    (*outMaska) = RANG_PO_UROV;
//        (*dvMaska) =
    break;

#define  CMD_ITEM_HOLES515 if(item>515 && item<672) continue;

//  count_bit = 1;
#define ACHR_CHAPV_CONFIGURATION_BEGIN 672
  case ACHR_CHAPV_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_ACHR_CHAPV_VID_DV;
    (*dvMaska) = RANG_SMALL_ACHR_CHAPV_VID_DV;
    break;
  case 673:
    (*outMaska) = RANG_BLOCK_ACHR1;
    (*dvMaska) = RANG_SMALL_BLOCK_ACHR1;
    break;
  case 674:
    (*outMaska) = RANG_BLOCK_ACHR2;
    (*dvMaska) = RANG_SMALL_BLOCK_ACHR2;
    break;
  case 675:
    (*outMaska) = RANG_VN_BLOCK_CHAPV;
//        (*dvMaska) =
    break;
  case 676:
    (*outMaska) = RANG_PO_ACHR1;
//        (*dvMaska) =
    break;
  case 677:
    (*outMaska) = RANG_PO_ACHR2;
//        (*dvMaska) =
    break;
  case 678:
    (*outMaska) = RANG_ACHR_CHAPV1;
//        (*dvMaska) =
    break;
  case 679:
    (*outMaska) = RANG_ACHR_CHAPV2;
//        (*dvMaska) =
    break;
  case 680:
    (*outMaska) = RANG_RAZR_CHAPV;
//        (*dvMaska) =
    break;
  case 681:
    (*outMaska) = RANG_PO_CHAPV1;
//        (*dvMaska) =
    break;
#define ACHR_CHAPV_CONFIGURATION_END 682
  case ACHR_CHAPV_CONFIGURATION_END:
    (*outMaska) = RANG_PO_CHAPV2;
//        (*dvMaska) =
    break;

//IF ВСТАВКА 688-711

#define  CMD_ITEM_HOLES711 if(item>711 && item<960) continue;

//  count_bit = 3;
#define  IMUNITET_BITACMD960 960
  case IMUNITET_BITACMD960:
    (*outMaska) = RANG_VKL_VV;
    (*dvMaska) = RANG_SMALL_VKL_VV;
    break;
#define  IMUNITET_BITACMD961 961
  case IMUNITET_BITACMD961:
    (*outMaska) = RANG_OTKL_VV;
    (*dvMaska) = RANG_SMALL_OTKL_VV;
    break;
  case 962:
    (*outMaska) = RANG_BLOCK_VKL_VV;
    (*dvMaska) = RANG_SMALL_BLOCK_VKL_VV;
    break;

//  count_bit = 15;
  case 964:
    (*outMaska) = RANG_WORK_BO;
//        (*dvMaska) =
    break;
  case 965:
    (*outMaska) = RANG_WORK_BV;
//        (*dvMaska) =
    break;
  case 966:
    (*outMaska) = RANG_STATE_VV;
    (*dvMaska) = RANG_SMALL_STATE_VV;
    break;
  case 967:
    (*outMaska) = RANG_PRYVID_VV;
//        (*dvMaska) =
    break;
  case 968:
    (*outMaska) = RANG_CTRL_VKL;
    (*dvMaska) = RANG_SMALL_CTRL_VKL;
    break;
  case 969:
    (*outMaska) = RANG_CTRL_OTKL;
    (*dvMaska) = RANG_SMALL_CTRL_OTKL;
    break;
  case 970:
    (*outMaska) = RANG_PEREVYSHCHENNJA_Inom_VYMK;
//        (*dvMaska) =
    break;
  case 971:
    (*outMaska) = RANG_KRYTYCHNYJ_RESURS_VYMYKACHA;
//        (*dvMaska) =
    break;
  case 972:
    (*outMaska) = RANG_VYCHERPANYJ_RESURS_VYMYKACHA;
//        (*dvMaska) =
    break;

#define  IMUNITET_BITACMD973 973

//  count_bit = 12;
  case 981:
    (*outMaska) = RANG_VIDKL_VID_ZAKHYSTIV;
//        (*dvMaska) =
    break;

//  count_bit = 12;
  case 1040:
    (*outMaska) = RANG_DEFECT;
//        (*dvMaska) =
    break;
  case 1041:
    (*outMaska) = RANG_AVAR_DEFECT;
//        (*dvMaska) =
    break;
#define  IMUNITET_BITACMD1042 1042
  case IMUNITET_BITACMD1042:
    (*outMaska) = RANG_RESET_LEDS;
    (*dvMaska) = RANG_SMALL_RESET_LEDS;
    break;
#define  IMUNITET_BITACMD1043 1043
  case IMUNITET_BITACMD1043:
    (*outMaska) = RANG_RESET_RELES;
    (*dvMaska) = RANG_SMALL_RESET_RELES;
    break;

#define  IMUNITET_BITACMD1044 1044

  case 1045:
    (*outMaska) = RANG_SETTINGS_CHANGED;
//        (*dvMaska) =
    break;

#define  BITACMD1046 1046

#define PASSWORD_SETCMD 1047

  case 1048:
    (*outMaska) = RANG_MISCEVE_DYSTANCIJNE;
    (*dvMaska) = RANG_SMALL_MISCEVE_DYSTANCIJNE;
    break;

  case 1049:
    (*outMaska) = RANG_WORK_A_REJESTRATOR;
//        (*dvMaska) =
    break;

//  count_bit = 1;
  case 1052:
    (*outMaska) = RANG_WORK_D_REJESTRATOR;
//        (*dvMaska) =
    break;

//  count_bit = 1;
  case 1060:
    (*outMaska) = RANG_OTKL_VID_ZOVN_ZAHYSTIV;
    (*dvMaska)  = RANG_SMALL_OTKL_VID_ZOVN_ZAHYSTIV;
    break;
  case 1061:
    (*outMaska) = RANG_ERROR_CONF_EL;
//        (*dvMaska) =
    break;

//  count_bit = 5;
  case 1071:
    (*outMaska) = RANG_BLK_GRUP_USTAVOK_VID_ZACHYSTIV;
//        (*dvMaska) =
    break;

  case 1072:
    (*outMaska) = RANG_INVERS_DV_GRUPA_USTAVOK;
//        (*dvMaska) =
    break;
  case 1073:
    (*outMaska) = RANG_READY_TU;
//        (*dvMaska) =
    break;
#define  IMUNITET_BITACMD1074 1074
  case IMUNITET_BITACMD1074:
    (*outMaska) = RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV;
    (*dvMaska) = RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV;
    break;

//  count_bit = 8;
#define  BITACMD1077 1077

//  count_bit = 8;
#define EL_CONFIGURATION_BEGIN 1088
  case EL_CONFIGURATION_BEGIN:
    (*outMaska) = RANG_DF1_IN;
    (*dvMaska) = RANG_SMALL_DF1_IN;
    if(actControl==(1+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1089:
    (*outMaska) = RANG_DF2_IN;
    (*dvMaska) = RANG_SMALL_DF2_IN;
    if(actControl==(2+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1090:
    (*outMaska) = RANG_DF3_IN;
    (*dvMaska) = RANG_SMALL_DF3_IN;
    if(actControl==(3+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1091:
    (*outMaska) = RANG_DF4_IN;
    (*dvMaska) = RANG_SMALL_DF4_IN;
    if(actControl==(4+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1092:
    (*outMaska) = RANG_DF5_IN;
    (*dvMaska) = RANG_SMALL_DF5_IN;
    if(actControl==(5+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1093:
    (*outMaska) = RANG_DF6_IN;
    (*dvMaska) = RANG_SMALL_DF6_IN;
    if(actControl==(6+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1094:
    (*outMaska) = RANG_DF7_IN;
    (*dvMaska) = RANG_SMALL_DF7_IN;
    if(actControl==(7+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1095:
    (*outMaska) = RANG_DF8_IN;
    (*dvMaska) = RANG_SMALL_DF8_IN;
    if(actControl==(8+SOURCEMARKER_MFT)) isValid = 0;
    break;

//  count_bit = 8;
  case 1120:
    (*outMaska) = RANG_DF1_OUT;
//        (*dvMaska) =
    if(actControl==(1+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1121:
    (*outMaska) = RANG_DF2_OUT;
//        (*dvMaska) =
    if(actControl==(2+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1122:
    (*outMaska) = RANG_DF3_OUT;
//        (*dvMaska) =
    if(actControl==(3+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1123:
    (*outMaska) = RANG_DF4_OUT;
//        (*dvMaska) =
    if(actControl==(4+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1124:
    (*outMaska) = RANG_DF5_OUT;
//        (*dvMaska) =
    if(actControl==(5+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1125:
    (*outMaska) = RANG_DF6_OUT;
//        (*dvMaska) =
    if(actControl==(6+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1126:
    (*outMaska) = RANG_DF7_OUT;
//        (*dvMaska) =
    if(actControl==(7+SOURCEMARKER_MFT)) isValid = 0;
    break;
  case 1127:
    (*outMaska) = RANG_DF8_OUT;
//        (*dvMaska) =
    if(actControl==(8+SOURCEMARKER_MFT)) isValid = 0;
    break;

//  count_bit = 12;
  case 1136:
    (*outMaska) = RANG_DT1_SET;
    (*dvMaska) = RANG_SMALL_DT1_SET;
    if(actControl==(1+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1137:
    (*outMaska) = RANG_DT2_SET;
    (*dvMaska) = RANG_SMALL_DT2_SET;
    if(actControl==(2+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1138:
    (*outMaska) = RANG_DT3_SET;
    (*dvMaska) = RANG_SMALL_DT3_SET;
    if(actControl==(3+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1139:
    (*outMaska) = RANG_DT4_SET;
    (*dvMaska) = RANG_SMALL_DT4_SET;
    if(actControl==(4+SOURCEMARKER_DTR)) isValid = 0;
    break;

//  count_bit = 12;
  case 1152:
    (*outMaska) = RANG_DT1_RESET;
    (*dvMaska) = RANG_SMALL_DT1_RESET;
    if(actControl==(1+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1153:
    (*outMaska) = RANG_DT2_RESET;
    (*dvMaska) = RANG_SMALL_DT2_RESET;
    if(actControl==(2+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1154:
    (*outMaska) = RANG_DT3_RESET;
    (*dvMaska) = RANG_SMALL_DT3_RESET;
    if(actControl==(3+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1155:
    (*outMaska) = RANG_DT4_RESET;
    (*dvMaska) = RANG_SMALL_DT4_RESET;
    if(actControl==(4+SOURCEMARKER_DTR)) isValid = 0;
    break;

//  count_bit = 12;
  case 1168:
    (*outMaska) = RANG_DT1_OUT;
//        (*dvMaska) =
    if(actControl==(1+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1169:
    (*outMaska) = RANG_DT2_OUT;
//        (*dvMaska) =
    if(actControl==(2+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1170:
    (*outMaska) = RANG_DT3_OUT;
//        (*dvMaska) =
    if(actControl==(3+SOURCEMARKER_DTR)) isValid = 0;
    break;
  case 1171:
    (*outMaska) = RANG_DT4_OUT;
//        (*dvMaska) =
    if(actControl==(4+SOURCEMARKER_DTR)) isValid = 0;
    break;

//  count_bit = 40;
  case 1184:
    (*outMaska) = RANG_D_AND1;
//        (*dvMaska) =
    break;
  case 1185:
    (*outMaska) = RANG_D_AND2;
//        (*dvMaska) =
    break;
  case 1186:
    (*outMaska) = RANG_D_AND3;
//        (*dvMaska) =
    break;
  case 1187:
    (*outMaska) = RANG_D_AND4;
//        (*dvMaska) =
    break;
  case 1188:
    (*outMaska) = RANG_D_AND5;
//        (*dvMaska) =
    break;
  case 1189:
    (*outMaska) = RANG_D_AND6;
//        (*dvMaska) =
    break;
  case 1190:
    (*outMaska) = RANG_D_AND7;
//        (*dvMaska) =
    break;
  case 1191:
    (*outMaska) = RANG_D_AND8;
//        (*dvMaska) =
    break;

//  count_bit = 12;
  case 1200:
    (*outMaska) = RANG_D_OR1;
//        (*dvMaska) =
    break;
  case 1201:
    (*outMaska) = RANG_D_OR2;
//        (*dvMaska) =
    break;
  case 1202:
    (*outMaska) = RANG_D_OR3;
//        (*dvMaska) =
    break;
  case 1203:
    (*outMaska) = RANG_D_OR4;
//        (*dvMaska) =
    break;
  case 1204:
    (*outMaska) = RANG_D_OR5;
//        (*dvMaska) =
    break;
  case 1205:
    (*outMaska) = RANG_D_OR6;
//        (*dvMaska) =
    break;
  case 1206:
    (*outMaska) = RANG_D_OR7;
//        (*dvMaska) =
    break;
  case 1207:
    (*outMaska) = RANG_D_OR8;
//        (*dvMaska) =
    break;

//  count_bit = 12;
  case 1216:
    (*outMaska) = RANG_D_NOT1;
//        (*dvMaska) =
    break;
  case 1217:
    (*outMaska) = RANG_D_NOT2;
//        (*dvMaska) =
    break;
  case 1218:
    (*outMaska) = RANG_D_NOT3;
//        (*dvMaska) =
    break;
  case 1219:
    (*outMaska) = RANG_D_NOT4;
//        (*dvMaska) =
    break;
  case 1220:
    (*outMaska) = RANG_D_NOT5;
//        (*dvMaska) =
    break;
  case 1221:
    (*outMaska) = RANG_D_NOT6;
//        (*dvMaska) =
    break;
  case 1222:
    (*outMaska) = RANG_D_NOT7;
//        (*dvMaska) =
    break;
  case 1223:
    (*outMaska) = RANG_D_NOT8;
//        (*dvMaska) =
    break;
  case 1224:
    (*outMaska) = RANG_D_NOT9;
//        (*dvMaska) =
    break;
  case 1225:
    (*outMaska) = RANG_D_NOT10;
//        (*dvMaska) =
    break;
  case 1226:
    (*outMaska) = RANG_D_NOT11;
//        (*dvMaska) =
    break;
  case 1227:
    (*outMaska) = RANG_D_NOT12;
//        (*dvMaska) =
    break;
  case 1228:
    (*outMaska) = RANG_D_NOT13;
//        (*dvMaska) =
    break;
  case 1229:
    (*outMaska) = RANG_D_NOT14;
//        (*dvMaska) =
    break;
  case 1230:
    (*outMaska) = RANG_D_NOT15;
//        (*dvMaska) =
    break;
  case 1231:
    (*outMaska) = RANG_D_NOT16;
//        (*dvMaska) =
    break;
  case 1232:
    (*outMaska) = RANG_D_XOR1;
//        (*dvMaska) =
    break;
  case 1233:
    (*outMaska) = RANG_D_XOR2;
//        (*dvMaska) =
    break;
  case 1234:
    (*outMaska) = RANG_D_XOR3;
//        (*dvMaska) =
    break;
  case 1235:
    (*outMaska) = RANG_D_XOR4;
//        (*dvMaska) =
    break;
  case 1236:
    (*outMaska) = RANG_D_XOR5;
//        (*dvMaska) =
    break;
  case 1237:
    (*outMaska) = RANG_D_XOR6;
//        (*dvMaska) =
    break;
  case 1238:
    (*outMaska) = RANG_D_XOR7;
//        (*dvMaska) =
    break;
#define EL_CONFIGURATION_END 1239
  case EL_CONFIGURATION_END:
    (*outMaska) = RANG_D_XOR8;
//        (*dvMaska) =
    break;

  }//switch
  if((*outMaska)!=-1)
  {
    if(inOffset>=MTZ_CONFIGURATION_BEGIN && inOffset<=MTZ_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<MTZ_BIT_CONFIGURATION))) isValid = 0;

    if(inOffset>=MTZ04_CONFIGURATION_BEGIN && inOffset<=MTZ04_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<MTZ04_BIT_CONFIGURATION))) isValid = 0;

    if(inOffset>=ZDZ_CONFIGURATION_BEGIN && inOffset<=ZDZ_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<ZDZ_BIT_CONFIGURATION))) isValid = 0;

    if(inOffset>=ZZ_CONFIGURATION_BEGIN && inOffset<=ZZ_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<ZZ_BIT_CONFIGURATION))) isValid = 0;

    if(inOffset>=TZNP_CONFIGURATION_BEGIN && inOffset<=TZNP_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<TZNP_BIT_CONFIGURATION))) isValid = 0;

    if(inOffset>=ZOP_CONFIGURATION_BEGIN && inOffset<=ZOP_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<ZOP_BIT_CONFIGURATION))) isValid = 0;

    if(inOffset>=UMIN_CONFIGURATION_BEGIN && inOffset<=UMIN_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<UMIN_BIT_CONFIGURATION))) isValid = 0;

    if(inOffset>=UMAX_CONFIGURATION_BEGIN && inOffset<=UMAX_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<UMAX_BIT_CONFIGURATION))) isValid = 0;

    if(inOffset>=APV_CONFIGURATION_BEGIN && inOffset<=APV_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<APV_BIT_CONFIGURATION))) isValid = 0;

    if(inOffset>=UROV_CONFIGURATION_BEGIN && inOffset<=UROV_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<UROV_BIT_CONFIGURATION))) isValid = 0;

    if(inOffset>=EL_CONFIGURATION_BEGIN && inOffset<=EL_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<EL_BIT_CONFIGURATION))) isValid = 0;

    if(inOffset>=ACHR_CHAPV_CONFIGURATION_BEGIN && inOffset<=ACHR_CHAPV_CONFIGURATION_END)
      if(!(edition_settings.configuration&(1<<ACHR_CHAPV_BIT_CONFIGURATION))) isValid = 0;

  }//if((*outMaska)!=-1)

  if(inOffset>=688 && inOffset<712)
  {
    if(!(edition_settings.configuration&(1<<UP_BIT_CONFIGURATION))) isValid = 0;
    int inupOffset = inOffset-688;
    int subObj = inupOffset/3;
    int offsetObj = inupOffset%3;

    switch(offsetObj)
    {
    case 0:
      (*outMaska) = RANG_BLOCK_UP1 + 3*subObj;
      (*dvMaska)  = RANG_SMALL_BLOCK_UP1 + 1*subObj;
      break;
    case 1:
      (*outMaska) = RANG_PO_UP1 + 3*subObj;
//        (*dvMaska) =
      break;
    case 2:
      (*outMaska) = RANG_UP1 + 3*subObj;
//        (*dvMaska) =
      break;
    }//switch
  }//if

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  if(inOffset>=1248 && inOffset<1264)
  {
    int ingoosOffset = inOffset-1248;
    (*outMaska) = RANG_BLOCK_IN_GOOSE1 + ingoosOffset;
    (*dvMaska)  = RANG_SMALL_BLOCK_IN_GOOSE1 + ingoosOffset;
  }//if(inOffset>=608 && inOffset<624)
  else if(inOffset>=1264 && inOffset<1268)
  {
    int inmmsOffset = inOffset-1264;
    (*outMaska) = RANG_BLOCK_IN_MMS1 + inmmsOffset;
    (*dvMaska)  = RANG_SMALL_BLOCK_IN_MMS1 + inmmsOffset;
  }//if(inOffset>=624 && inOffset<628)
  else if(inOffset>=1280 && inOffset<1284)
  {
    int inlanOffset = inOffset-1280;
    (*outMaska) = RANG_BLOCK_OUT_LAN1 + inlanOffset;
    (*dvMaska)  = RANG_SMALL_BLOCK_OUT_LAN1 + inlanOffset;
  }//if(inOffset>=640 && inOffset<644)
#endif

  return isValid;
}//cmdFunc000(int inOffset, int *outMaska, int *dvMaska)

int getACMDSmallBeginAdr(void)
{
  return BEGIN_ADR_BIT;
}//getACMDSmallBeginAdr(void)
/**************************************/
//компонент Текущие активные и сработавшие функции
/**************************************/
void constructorACMDSmallComponent(COMPONENT_OBJ *acmdcomp)
{
  acmdsmallcomponent = acmdcomp;

  acmdsmallcomponent->getModbusRegister = getACMDSmallModbusRegister;//получить содержимое регистра
  acmdsmallcomponent->getModbusBit      = getACMDSmallModbusBit;//получить содержимое бита
  acmdsmallcomponent->setModbusRegister = setACMDSmallModbusRegister;// регистра
  acmdsmallcomponent->setModbusBit      = setACMDSmallModbusBit;// бита

  acmdsmallcomponent->postWriteAction = postACMDSmallWriteAction;//action после записи

//  for(int i=0; i<N_BIG*32; i++)   decoderN_BIGACMDArray[i] = (unsigned short)decoderN_BIGACMDArrayLoader(i);//декодировщик индекса бита в адрес modbus  для реле
//  for(int i=0; i<N_SMALL*32; i++) decoderN_SMALLACMDArray[i] = (unsigned short)decoderN_SMALLACMDArrayLoader(i);//декодировщик индекса бита в адрес modbus  для DV
}//constructorDOUTSmallComponent(COMPONENT_OBJ *doutcomp)

void decoderN_BIGACMDArrayLoader(void)
{
//декодировщик индекса бита в адрес modbus  для реле
  static int idxBit_decoderN_BIGACMDArrayLoader;
  if(idxBit_decoderN_BIGACMDArrayLoader>=N_BIG*32) return;

  int outMaska = -1;
  int dvMaska = -1;
  decoderN_BIGACMDArray[idxBit_decoderN_BIGACMDArrayLoader] = 0xFFFF;
  for(int item=0; item<(END_ADR_BIT-BEGIN_ADR_BIT+1); item++)
  {

    CMD_ITEM_HOLES97
    CMD_ITEM_HOLES515
    CMD_ITEM_HOLES711

    cmdFunc000(item, &outMaska, &dvMaska, CLRACT_CONTROL);

    if(outMaska==idxBit_decoderN_BIGACMDArrayLoader)
    {
      decoderN_BIGACMDArray[idxBit_decoderN_BIGACMDArrayLoader] = (unsigned short) item;
      break;
    }//if
  }//for
  idxBit_decoderN_BIGACMDArrayLoader++;
}//decoderN_BIGACMDArrayLoader()

void decoderN_SMALLACMDArrayLoader(void)
{
//декодировщик индекса бита в адрес modbus  для ДВ
  static int idxBit_decoderN_SMALLACMDArrayLoader;
  if(idxBit_decoderN_SMALLACMDArrayLoader>=N_SMALL*32) return;

  int outMaska=-1;
  int dvMaska=-1;

  decoderN_SMALLACMDArray[idxBit_decoderN_SMALLACMDArrayLoader] = 0xFFFF;

  for(int item=0; item<(END_ADR_BIT-BEGIN_ADR_BIT+1); item++)
  {
    CMD_ITEM_HOLES97
    CMD_ITEM_HOLES515
    CMD_ITEM_HOLES711

    cmdFunc000(item, &outMaska, &dvMaska, CLRACT_CONTROL);
    if(dvMaska==idxBit_decoderN_SMALLACMDArrayLoader)
    {
      decoderN_SMALLACMDArray[idxBit_decoderN_SMALLACMDArrayLoader] = (unsigned short) item;
      break;
    }//if
  }//for
  idxBit_decoderN_SMALLACMDArrayLoader++;
}//decoderN_SMALLACMDArrayLoader()

int getRangN_BIGModbusRegister2021(unsigned int *ranguvannja_d, int offset )
{
//поиск активного бита
  int bit = getSequenceN_BIGIndexActiveBit(offset, ranguvannja_d);//индекс активного бита
  if(bit!=-1)
  {
    int adr = decoderN_BIGACMD(bit);
    return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus
  }//if(bit!=-1)
  return 0;
}//getRangN_BIGModbusRegister2021(unsigned int *ranguvannja_d, int offset )

void writeRangN_BIGModbusRegister2021(unsigned int *ranguvannja_d, unsigned short *dataPacket, int countAdr)
{
  unsigned int tmp[N_BIG];
  int idxObjOld = -1;
  //убрать старое
  for(int i=0; i<countAdr; i++)
  {
    if(idxObjOld != 0)
    {
      idxObjOld = 0;
      for(int dx=0; dx<N_BIG; dx++) tmp[dx]=ranguvannja_d[dx];
    }//if

    int bitOld = getSequenceN_BIGIndexActiveBit(i, tmp);
    if(bitOld != -1) {
      ranguvannja_d[bitOld/32] &= ~(1<<(bitOld%32));
    }//if
  }//for

  //добавить новое
  for(int i=0; i<countAdr; i++)
  {
    unsigned int adr = (unsigned short) swapByteInShort(dataPacket[i]);//новое значение
    if(adr == 0) continue;
    int bit = encoderN_BIGACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для реле
    if(bit != -1)
    {
      ranguvannja_d[bit/32] |= (1<<(bit%32));
    }//if
  }//for
}//writeRangN_BIGModbusRegister2021

int getRangN_SMALLModbusRegister2021(unsigned int *ranguvannja_d, int offset )
{
//поиск активного бита
  int bit = getSequenceN_SMALLIndexActiveBit(offset, ranguvannja_d);//индекс активного бита
  if(bit!=-1)
  {
    int adr = decoderN_SMALLACMD(bit);
    return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus
  }//if(bit!=-1)
  return 0;
}//getRangN_SMALLModbusRegister2021(unsigned int *ranguvannja_d, int offset )

void writeRangN_SMALLModbusRegister2021(unsigned int *ranguvannja_d, unsigned short *dataPacket, int countAdr)
{
  unsigned int tmp[N_SMALL];
  int idxObjOld = -1;
  //убрать старое
  for(int i=0; i<countAdr; i++)
  {
    if(idxObjOld != 0)
    {
      idxObjOld = 0;
      for(int dx=0; dx<N_SMALL; dx++) tmp[dx]=ranguvannja_d[dx];
    }//if

    int bitOld = getSequenceN_SMALLIndexActiveBit(i, tmp);
    if(bitOld != -1) {
      ranguvannja_d[bitOld/32] &= ~(1<<(bitOld%32));
    }//if
  }//for

  //добавить новое
  for(int i=0; i<countAdr; i++)
  {
    unsigned int adr = (unsigned short) swapByteInShort(dataPacket[i]);//новое значение
    if(adr == 0) continue;
    int bit = encoderN_SMALLACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для реле
    if(bit != -1)
    {
      ranguvannja_d[bit/32] |= (1<<(bit%32));
    }//if
  }//for
}//writeRangN_SMALLModbusRegister2021

void loadACMDSmallActualDataBit(int cmdSwitch, int beginOffset, int endOffset)
{
  //ActualData
  for(int ii=0; ii<(1+(END_ADR_BIT-BEGIN_ADR_BIT+1)/16); ii++) tempReadArray[ii] = 0;
  for(int item=0; item<(END_ADR_BIT-BEGIN_ADR_BIT+1); item++)
  {
    int value =0;
    int tsdata = 0;
    if(item>=beginOffset && item<endOffset)
    {
      switch(item)
      {
      case PASSWORD_SETCMD://Пароль установлен
      {
        switch(pointInterface)//метка интерфейса 0-USB 1-RS485
        {
        case USB_RECUEST:
          if(cmdSwitch==0) //ACMD
            value = password_set_USB;//Пароль установлен
          break;
        case RS485_RECUEST:
          if(cmdSwitch==0) //ACMD
            value = password_set_RS485;//Пароль установлен
          break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        case LAN_RECUEST:
          if(cmdSwitch==0) //ACMD
            value = password_set_LAN;//Пароль установлен
          break;
#endif
        }//switch(pointInterface)
        goto m1;
      }//case PASSWORD_SETCMD://Пароль установлен

      case IMUNITET_BITACMD973://Сброс счетчика ресурса выключателя
      {
        value = 0;
        if(pointInterface==RS485_RECUEST)//метка интерфейса 0-USB 1-RS485
          if(cmdSwitch==1) //GCMD
          {
            if(information_about_restart_counter & /*(1 << USB_RECUEST)|*/(1 << RS485_RECUEST))
              value = 1;
            goto m1;
          }//if

        if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
          if(cmdSwitch==1) //GCMD
          {
            if(information_about_restart_counter & (1 << USB_RECUEST)/*(1 << RS485_RECUEST)*/)
              value = 1;
            goto m1;
          }//if

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        if(pointInterface==LAN_RECUEST)//метка интерфейса 0-USB 1-RS485
          if(cmdSwitch==1) //GCMD
          {
            if(information_about_restart_counter & (1 << LAN_RECUEST)/*(1 << RS485_RECUEST)*/)
              value = 1;
            goto m1;
          }//if
#endif

        goto m1;
      }//case IMUNITET_BITACMD973
      }//switch(item)
      value = encoderN_BIGACMD(item);//кодировщик адреса modbus в индекс бита для реле
      if(value==-1) value=0;
      else {
        //читать значение команд
        if(cmdSwitch==0) {
          //ACMD
          value = active_functions[value/32] & (1<<(value%32));
        }//if(cmdSwitch==0)
        else if(cmdSwitch==1) {
          //GCMD
          if(pointInterface==USB_RECUEST)//метка интерфейса 0-USB 1-RS485
          {
            value = trigger_functions_USB[value/32] & (1<<(value%32));
          }//if
          else if(pointInterface==RS485_RECUEST)
          {
            value = trigger_functions_RS485[value/32] & (1<<(value%32));
          }//else
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
          else if(pointInterface==LAN_RECUEST)
          {
            value = trigger_functions_LAN[value/32] & (1<<(value%32));
          }//else
#endif
          else
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed();
          }
        }//if(cmdSwitch==1)
        else if(cmdSwitch==2) {
          //RDS
          value = rds_functions[value/32] & (1<<(value%32));
        }//if(cmdSwitch==2)

      }
    }//if(item>=beginOffset && item<endOffset)
m1:
    if(value) tsdata=1;
    tempReadArray[item/16] |= tsdata<<(item%16);
  }//for(int item=0; item<568; item++)
}//loadACMDSmallActualDataBit(int cmdSwitch, int beginOffset, int endOffset)

int validN_BIGACMD(unsigned short dataReg)
{
  return validBazaN_BIGACMD(dataReg, CLRACT_CONTROL);
}//validN_BIGACMD(unsigned short dataReg)
int validBazaN_BIGACMD(unsigned short dataReg, int actControl)
{
//расширенная проверка адреса команды for rele
  if(dataReg!=0)
  {
    int validCMD=0;
    if(((int)encoderValidN_BIGACMD(dataReg-getACMDSmallBeginAdr(), &validCMD, actControl))<0) return MARKER_ERRORPERIMETR; //кодировщик адреса modbus в индекс бита для реле
    if(!validCMD) return MARKER_ERRORPERIMETR; //не прошла валидация
  }//if(dataReg!=0)

  return 0;
}//validBazaN_BIGACMD(unsigned short dataReg, int actControl)
int validN_SMALLACMD(unsigned short dataReg)
{
//расширенная проверка адреса команды for dv
  if(dataReg!=0)
  {
    int validCMD=0;
    if(((int)encoderValidN_SMALLACMD(dataReg-getACMDSmallBeginAdr(), &validCMD))<0) return MARKER_ERRORPERIMETR; //кодировщик адреса modbus в индекс бита для реле
    if(!validCMD) return MARKER_ERRORPERIMETR; //не прошла валидация
  }//if(dataReg!=0)

  return 0;
}//validN_SMALLACMD(int dataReg)
unsigned int encoderN_BIGACMD(int offsetCMD)
{
  int validCMD=0;
  return encoderValidN_BIGACMD(offsetCMD, &validCMD, CLRACT_CONTROL);
}//encoderN_BIGACMD(int adrCMD)
unsigned int encoderValidN_BIGACMD(int offsetCMD, int *validCMD, int actControl)
{
//кодировщик адреса modbus в индекс бита для реле
  int outMaska=-1;
  int dvMaska=-1;
  (*validCMD) = cmdFunc000(offsetCMD, &outMaska, &dvMaska, actControl);
  return outMaska;
}//encoderValidN_BIGACMD(int offsetCMD, int *validCMD)
unsigned int encoderN_SMALLACMD(int offsetCMD)
{
  int validCMD=0;
  return encoderValidN_SMALLACMD(offsetCMD, &validCMD);
}//encoderN_SMALLACMD(int adrCMD)
unsigned int encoderValidN_SMALLACMD(int offsetCMD, int *validCMD)
{
//кодировщик адреса modbus в индекс бита для ДВ
  int outMaska=-1;
  int dvMaska=-1;
  (*validCMD) = cmdFunc000(offsetCMD, &outMaska, &dvMaska, CLRACT_CONTROL);
  return dvMaska;
}//encoderValidN_SMALLACMD(int offsetCMD, int *validCMD)

int decoderN_BIGACMD(int idxBit)
{
//декодировщик индекса бита в адрес modbus  для реле
  unsigned short result = decoderN_BIGACMDArray[idxBit];
  if(result==0xFFFF) return -1;
  return result;
}//decoderN_BIGACMD(int idxBit)
/*
int decoderN_BIGACMDArrayLoader(int idxBit)
{
//декодировщик индекса бита в адрес modbus  для реле
  for(int item=0; item<(END_ADR_BIT-BEGIN_ADR_BIT+1); item++)
  {
    int outMaska=-1;
    int dvMaska=-1;
    cmdFunc000(item, &outMaska, &dvMaska, CLRACT_CONTROL);
    if(outMaska==idxBit) return item;
  }//for(int item=0; item<568; item++)
  return -1;
}//decoderN_BIGACMD(int idxBit)
*/
int decoderN_SMALLACMD(int idxBit)
{
//декодировщик индекса бита в адрес modbus  для ДВ
  unsigned short result = decoderN_SMALLACMDArray[idxBit];
  if(result==0xFFFF) return -1;
  return result;
}//decoderN_SMALLACMD(int idxBit)
/*
int decoderN_SMALLACMDArrayLoader(int idxBit)
{
//декодировщик индекса бита в адрес modbus  для ДВ
  for(int item=0; item<(END_ADR_BIT-BEGIN_ADR_BIT+1); item++)
  {
    int outMaska=-1;
    int dvMaska=-1;
    cmdFunc000(item, &outMaska, &dvMaska, CLRACT_CONTROL);
    if(dvMaska==idxBit) return item;
  }//for(int item=0; item<568; item++)
  return -1;
}//decoderN_SMALLACMD(int idxBit)
*/
int getSequenceN_BIGIndexActiveBit(int pps, unsigned int *array)
{
//индекс активного бита N_BIG
  int bit=0;
  unsigned int arrayData = 0;
  int propusk = pps;
  for(; bit<N_BIG*32; bit++)
  {
    arrayData = array[bit/32];
    if(arrayData==0) {
      bit+=31;
      continue;
    }
    unsigned int data = arrayData&(1<<(bit%32));
    if(data!=0&&propusk==0) break;
    if(data!=0&&propusk!=0) propusk--;
  }//for
  if(bit!=N_BIG*32) return bit;
  return -1;
}//getSequenceN_BIGIndexActiveBit(int propusk, unsigned int *array)

int getSequenceN_SMALLIndexActiveBit(int pps, unsigned int *array)
{
//индекс активного бита N_SMALL
  int bit=0;
  unsigned int arrayData = 0;
  int propusk = pps;
  for(; bit<N_SMALL*32; bit++)
  {
    arrayData = array[bit/32];
    if(arrayData==0) {
      bit+=31;
      continue;
    }
    unsigned int data = arrayData&(1<<(bit%32));
    if(data!=0&&propusk==0) break;
    if(data!=0&&propusk!=0) propusk--;
  }//for
  if(bit!=N_SMALL*32) return bit;
  return -1;
}//getSequenceN_SMALLIndexActiveBit(int propusk, unsigned int *array)
int getRangN_BIGModbusRegister(unsigned int *ranguvannja, int countItem, int offset )
{
//поиск активного бита
  int subObj = offset/countItem;
  int bit = getSequenceN_BIGIndexActiveBit(offset%countItem, &ranguvannja[N_BIG*subObj]);//индекс активного бита
  if(bit!=-1)
  {
    int adr = decoderN_BIGACMD(bit);
    return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus  для rele
  }//if(bit!=-1)
  return 0;
}//getRangN_BIGModbusRegister(unsigned int *ranguvannja, int countItem, int offset )

int getRangN_SMALLModbusRegister(unsigned int *ranguvannja, int countItem, int offset )
{
//поиск активного бита
  int subObj = offset/countItem;
  int bit = getSequenceN_SMALLIndexActiveBit(offset%countItem, &ranguvannja[N_SMALL*subObj]);//индекс активного бита
  if(bit!=-1)
  {
    int adr = decoderN_SMALLACMD(bit);
    return adr<0? 0: adr+getACMDSmallBeginAdr();//декодировщик индекса бита в адрес modbus  для rele
  }//if(bit!=-1)
  return 0;
}//getRangN_SMALLModbusRegister(unsigned int *ranguvannja, int countItem, int offset )
void writeRangN_BIGModbusRegister(unsigned int *ranguvannja, int countItem, int beginAdr,
                                  int countAdr, int begin_adr_register)
{
  int offsetTempWriteArray = superFindTempWriteArrayOffset(begin_adr_register);//найти смещение TempWriteArray
  unsigned int tmp[N_BIG];//для сохр ранжирования
  int idxObjOld=-1;//индекс суб обекта old
  //убрать старое
  for(int i=0; i<countAdr; i++)
  {
    int offset = beginAdr-begin_adr_register+i;

    int idxObj = offset/countItem;//индекс суб обекта
    unsigned int *ranguvannja_d = &ranguvannja[N_BIG*idxObj]; //Ранжування
    if(idxObj!=idxObjOld) {
      idxObjOld = idxObj;
      for(int dx=0; dx<N_BIG; dx++) tmp[dx]=ranguvannja_d[dx];//сохр старое ранж
    }//if

    int bitOld = getSequenceN_BIGIndexActiveBit(offset%countItem, tmp);//индекс активного бита
    if(bitOld!=-1) {
      ranguvannja_d[bitOld/32] &= ~(1<<(bitOld%32));
    }//if
  }//for
  //добавить новое
  for(int i=0; i<countAdr; i++)
  {
    unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//новое значение
    if(adr==0) continue;
    int offset = beginAdr-begin_adr_register+i;

    int idxObj = offset/countItem;//индекс суб обекта
    unsigned int *ranguvannja_d = &ranguvannja[N_BIG*idxObj]; //Ранжування
    int bit = encoderN_BIGACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для реле
    if(bit!=-1) {
      ranguvannja_d[bit/32] |= (1<<(bit%32));
    }//if
  }//for
}//writeRangN_BIGModbusRegister(unsigned int *ranguvannja, int countItem, int beginAdr, int countAdr,
void writeRangN_SMALLModbusRegister(unsigned int *ranguvannja, int countItem, int beginAdr,
                                    int countAdr, int begin_adr_register)
{
  int offsetTempWriteArray = superFindTempWriteArrayOffset(begin_adr_register);//найти смещение TempWriteArray
  unsigned int tmp[N_SMALL];//для сохр ранжирования
  int idxObjOld=-1;//индекс суб обекта old
  //убрать старое
  for(int i=0; i<countAdr; i++)
  {
    int offset = beginAdr-begin_adr_register+i;

    int idxObj = offset/countItem;//индекс суб обекта
    unsigned int *ranguvannja_d = &ranguvannja[N_SMALL*idxObj]; //Ранжування
    if(idxObj!=idxObjOld) {
      idxObjOld = idxObj;
      for(int dx=0; dx<N_SMALL; dx++) tmp[dx]=ranguvannja_d[dx];//сохр старое ранж
    }//if

    int bitOld = getSequenceN_SMALLIndexActiveBit(offset%countItem, tmp);//индекс активного бита
    if(bitOld!=-1) {
      ranguvannja_d[bitOld/32] &= ~(1<<(bitOld%32));
    }//if
  }//for
  //добавить новое
  for(int i=0; i<countAdr; i++)
  {
    unsigned int adr = (unsigned short)tempWriteArray[offsetTempWriteArray+i];//новое значение
    if(adr==0) continue;
    int offset = beginAdr-begin_adr_register+i;

    int idxObj = offset/countItem;//индекс суб обекта
    unsigned int *ranguvannja_d = &ranguvannja[N_SMALL*idxObj]; //Ранжування
    int bit = encoderN_SMALLACMD(adr-getACMDSmallBeginAdr()); //кодировщик адреса modbus в индекс бита для dv
    if(bit!=-1) {
      ranguvannja_d[bit/32] |= (1<<(bit%32));
    }//if
  }//for
}//writeRangN_SMALLModbusRegister(unsigned int *ranguvannja, int countItem, int beginAdr, int countAdr,

int getACMDSmallModbusRegister(int adrReg)
{
  extern int globalcntReg;//к-во reg для чтения
  extern int globalbeginAdrReg;//адрес нач reg
  //получить содержимое регистра
  if(privateACMDSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  if(acmdsmallcomponent->isActiveActualData)
  {
    int begin = globalbeginAdrReg-BEGIN_ADR_REGISTER;
    if(begin<0) globalcntReg += begin;

    int beginOffset = (adrReg-BEGIN_ADR_REGISTER)*16;
    int endOffset   = beginOffset +globalcntReg*16;

    loadACMDSmallActualDataBit(0, beginOffset, endOffset); //ActualData
  }//if(acmdsmallcomponent->isActiveActualData)
  acmdsmallcomponent->isActiveActualData = 0;

  return tempReadArray[adrReg-BEGIN_ADR_REGISTER];
}//getDOUTModbusRegister(int adrReg)
int getACMDSmallModbusBit(int adrBit)
{
  extern int globalcntBit;//к-во бит для чтения
  extern int globalbeginAdrBit;//адрес нач бит
  //получить содержимое bit

  if(privateACMDSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  int beginOffset = adrBit-BEGIN_ADR_BIT;
  if(acmdsmallcomponent->isActiveActualData)
  {
    int begin = globalbeginAdrBit-BEGIN_ADR_BIT;
    if(begin<0) globalcntBit += begin;

    int endOffset   = beginOffset +globalcntBit;

    loadACMDSmallActualDataBit(0, beginOffset, endOffset); //ActualData
  }//if(acmdsmallcomponent->isActiveActualData)
  acmdsmallcomponent->isActiveActualData = 0;

  short tmp   = tempReadArray[beginOffset/16];
  short maska = 1<<(beginOffset%16);
  if(tmp&maska) return 1;
  return 0;
}//getDOUTModbusBit(int adrReg)
int setACMDSmallModbusRegister(int adrReg, int dataReg)
{
  UNUSED(dataReg);
  if(privateACMDSmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  /*
    int beginOffset = (adrReg-BEGIN_ADR_REGISTER)*16;
    int endOffset   = beginOffset+16;// + beginOffset;
    for(int offset=beginOffset; offset<endOffset; offset++)
    {
      if(!(dataReg & (1<<(offset%16)))) continue;
      if(writeACMDSmallActualDataBit(offset, 0)!=0) return MARKER_ERRORPERIMETR;
    }//for
  */
  return MARKER_ERRORPERIMETR;
}//getDOUTModbusRegister(int adrReg)
int setACMDSmallModbusBit(int adrBit, int dataBit)
{
  //записать содержимое bit
  if(privateACMDSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;

  superSetOperativMarker(acmdsmallcomponent, adrBit);
  superSetTempWriteArray(dataBit);//записать в буфер
  if(adrBit!=BEGIN_ADR_BIT+BITACMD1046)//Активация конфигурации
  {
    if(dataBit==0) return MARKER_ERRORPERIMETR;
  }//if(adrBit!=50567)
  else return 0;

  if(adrBit == BEGIN_ADR_BIT+BITACMD1077)//Очистить регистратор статистики
  {
    if(
      ((clean_rejestrators & CLEAN_SR_ERR) != 0)
    ) return ERROR_VALID2;//ошибка валидации
  }//if

  if(writeACMDSmallActualDataBit(adrBit-BEGIN_ADR_BIT, 0)!=0) return MARKER_ERRORPERIMETR;

  return 0;
}//getDOUTModbusRegister(int adrReg)

int writeACMDSmallActualDataBit(int inOffset, int dataBit)
{
  int actControl = 0;
  if(inOffset<0)
  {
    inOffset = -inOffset;
    actControl = 1;
  }//if(offset<0)
  switch(inOffset)
  {
  case 960://
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_VKL_VV); //Вкл.  виключателя
      mutex_interface = false;
    }//if(action)
    return 0;
  case 961://
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_OTKL_VV); //Викл. виключателя
      mutex_interface = false;
    }//if(action)
    return 0;

  case 973://Сигнал про очищення ресурсу лічильників з системи захистів
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      restart_counter = 0xff; //Сигнал про очищення ресурсу лічильників з системи захистів
    }//if(action)
    return 0;

  case 1042://
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_RESET_LEDS);//Очищення індикації
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1043://
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_RESET_RELES); //Скидання реле
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1044://Скидання загальних функцій
    if(actControl&&dataBit)
    {
      //Скидання загальних функцій
      switch(pointInterface)//метка интерфейса 0-USB 1-RS485
      {
      case USB_RECUEST:
        reset_trigger_function_from_interface |= (1 << USB_RECUEST);
        break;
      case RS485_RECUEST:
        reset_trigger_function_from_interface |= (1 << RS485_RECUEST);
        break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      case LAN_RECUEST:
        reset_trigger_function_from_interface |= (1 << LAN_RECUEST);
        break;
#endif
      }//switch
    }//if(action)
    return 0;

  case 1046://Активизация конфигурации
    if(actControl)
    {
      if(dataBit)
      {
        //Активація внесекних змін
        int typI = 2;
        if(pointInterface==RS485_RECUEST) typI = 3;//метка интерфейса 0-USB 1-RS485
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        else if(pointInterface==LAN_RECUEST) typI = 4;//метка интерфейса 0-USB 1-RS485
#endif
        if(set_new_settings_from_interface(typI))//2-USB
        {
          type_of_settings_changed = 0;
          _CLEAR_BIT(active_functions, RANG_SETTINGS_CHANGED);
          return ERROR_VALID3;//ошибка
        }//if
      }//if
      else//деактивация
      {
        //Відміна внесекних змін
        current_settings_interfaces = current_settings;
      }
      type_of_settings_changed = 0;
      _CLEAR_BIT(active_functions, RANG_SETTINGS_CHANGED);
    }//if(action)
    return 0;

  case 1049://Запуск аналогового реєстратора
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      start_ar = 0xff; //Запуск аналогового реєстратора
    }//if(action)
    return 0;

  case 1074://Скидання блокування готорності ТУ від захистів
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV); //Скидання блокування готорності ТУ від захистів
      mutex_interface = false;
    }//if(action)
    return 0;

  case 1077://Очистить регистратор статистики
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      clean_rejestrators |= CLEAN_SR_ERR;
    }//if(action)
    return 0;

  case 1088://OF
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DF1_IN);
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1089://OF
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DF2_IN);
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1090://OF
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DF3_IN);
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1091://OF
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DF4_IN);
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1092://OF
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DF5_IN);
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1093://OF
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DF6_IN);
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1094://OF
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DF7_IN);
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1095://OF
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DF8_IN);
      mutex_interface = false;
    }//if(action)
    return 0;

  case 1136://DTR
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DT1_SET);
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1137://DTR
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DT2_SET);
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1138://DTR
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DT3_SET);
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1139://DTR
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DT4_SET);
      mutex_interface = false;
    }//if(action)
    return 0;

  case 1152://DTR
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DT1_RESET);
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1153://DTR
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DT2_RESET);
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1154://DTR
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DT3_RESET);
      mutex_interface = false;
    }//if(action)
    return 0;
  case 1155://DTR
    if(_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE) != 0) return MARKER_ERRORPERIMETR;
    if (_CHECK_SET_BIT(active_functions, RANG_READY_TU) == 0)return MARKER_ERRORPERIMETR;
    if(actControl&&dataBit)
    {
      mutex_interface = true;
      _SET_BIT(activation_function_from_interface, RANG_SMALL_DT4_RESET);
      mutex_interface = false;
    }//if(action)
    return 0;

  }//switch
  return MARKER_ERRORPERIMETR;
}//writeACMDSmallActualDataBit(int offset)

int postACMDSmallWriteAction(void)
{
//action после записи
  int beginAdr = acmdsmallcomponent->operativMarker[0];
  if(beginAdr<0) return 0;//не было записи
  int endAdr   = acmdsmallcomponent->operativMarker[1];
  int countAdr = endAdr-beginAdr+1;
  if(endAdr<0) countAdr = 1;

  if(beginAdr>=BEGIN_ADR_BIT)
  {
    //работа с битами
    int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_BIT);//найти смещение TempWriteArray

    for(int i=0; i<countAdr; i++) {
      int offset = i+beginAdr-BEGIN_ADR_BIT;
      uint32_t value = tempWriteArray[offsetTempWriteArray+i];
      writeACMDSmallActualDataBit(-offset, value);//action
    }//for
  }//if(beginAdr>=BEGIN_ADR_BIT)
  else
  {
    //работа с регистрами
    int offsetTempWriteArray = superFindTempWriteArrayOffset(BEGIN_ADR_REGISTER);//найти смещение TempWriteArray
    for(int i=0; i<countAdr; i++) {
      int offset = i+beginAdr-BEGIN_ADR_REGISTER;
      for(int bit=0; bit<16; bit++)
      {
        uint32_t value = tempWriteArray[offsetTempWriteArray+i];
        int temp = 0;
        if(value&(1<<bit)) temp = 1;
        writeACMDSmallActualDataBit(-(offset*16+bit), temp);//action
      }//for
    }//for
  }//else

  return 0;
}//

int privateACMDSmallGetReg2(int adrReg)
{
  //проверить внешний периметр
  return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

int privateACMDSmallGetBit2(int adrBit)
{
  //проверить внешний периметр
  return controlPerimetr(adrBit, BEGIN_ADR_BIT, END_ADR_BIT);
}//privateGetReg2(int adrReg)

int passwordImunitetBitACMDSmallComponent(int adrBit)
{
  //имунитетные к паролю адреса bit 0 - есть имунитет
  if(privateACMDSmallGetBit2(adrBit)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
  switch(adrBit)
  {
  case BEGIN_ADR_BIT+IMUNITET_BITACMD960://Включение ВВ
  case BEGIN_ADR_BIT+IMUNITET_BITACMD961://Отключение ВВ

  case BEGIN_ADR_BIT+IMUNITET_BITACMD973://Сброс счетчика ресурса выключателя

  case BEGIN_ADR_BIT+IMUNITET_BITACMD1042://Сброс индикации
  case BEGIN_ADR_BIT+IMUNITET_BITACMD1043://Сброс реле
  case BEGIN_ADR_BIT+IMUNITET_BITACMD1044://Сброс сработавших функций

  case BEGIN_ADR_BIT+IMUNITET_BITACMD1074://Сброс блокировки готовности к ТУ от защит
    return 0;//есть имунитет
  }//switch
  return MARKER_OUTPERIMETR;
}//passwordImunitetBitACMDSmallComponent(int adrBit)
