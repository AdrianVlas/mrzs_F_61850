#ifndef __CONST_GLOBAL__
#define __CONST_GLOBAL__

#define N_BIG   9
#define N_SMALL 3

//Величина, яка необхідна для періодичної (період 1  мс   ) генерації переривань каналом 1 таймеру 2 з прескаллером 2000 = (1999 + 1)
#define TIM2_CCR1_VAL                   1000
#define TIM2_MIN_PERIOD_WRITE_READ      1 /*мкс*/
//Величина, яка необхідна для періодичної (період 10 мс   ) генерації переривань каналом 1 таймеру 4 з прескаллером 600 = (599 + 1)
#define TIM4_CCR1_VAL                   1000
//Величина, яка необхідна для періодичної (період 1 мс   ) генерації переривань каналом 2 таймеру 4 з прескаллером 600 = (599 + 1)
#define TIM4_CCR2_VAL                   100
//Величина, яка необхідна для періодичної (як мінімум, період 10 мкс   ) генерації переривань каналом 3 таймеру 4 з прескаллером 600 = (599 + 1)
#define TIM4_CCR3_VAL                   1
//Величина, яка необхідна для періодичної (період 0,625 мс) генерації переривань каналом 1 таймеру 3 з прескаллером 1 = (0 + 1)
#define TIM5_CCR1_2_VAL               37500

#define MAX_NUMBER_LINES_VMP            8

#define DELTA_TIME_FOR_TIMERS           1
#define PERIOD_SIGNAL_OUTPUT_MODE_2     1000 /*мс*/

#define NUMBER_ANALOG_CANALES           8

#define NUMBER_INPUTS                   16
#define NUMBER_OUTPUTS                  16

#define NUMBER_LEDS                     17

#define NUMBER_UP                       8

//#define NUMBER_DEFINED_ELEMENTS         6
#define NUMBER_DEFINED_FUNCTIONS        8
#define NUMBER_DEFINED_TRIGGERS         4
#define NUMBER_DEFINED_AND              8
#define NUMBER_DEFINED_OR               8
#define NUMBER_DEFINED_XOR              8
#define NUMBER_DEFINED_NOT              16
#define NUMBER_TRANSFER_FUNCTIONS       16

#define NUMBER_DEFINED_BUTTONS          6

#define NUMBER_IN_AND                   8
#define NUMBER_IN_OR                    8

#define NUMBER_GROUP_USTAVOK            4

#define N_INPUT_BOARDS          2
#define N_OUTPUT_BOARDS         3

enum _configuration {
MTZ_BIT_CONFIGURATION = 0,
MTZ04_BIT_CONFIGURATION,
ZDZ_BIT_CONFIGURATION,
ZZ_BIT_CONFIGURATION,
TZNP_BIT_CONFIGURATION,
APV_BIT_CONFIGURATION,
ACHR_CHAPV_BIT_CONFIGURATION,
UROV_BIT_CONFIGURATION,
ZOP_BIT_CONFIGURATION,
UMIN_BIT_CONFIGURATION,
UMAX_BIT_CONFIGURATION,
UP_BIT_CONFIGURATION,
VMP_BIT_CONFIGURATION,
EL_BIT_CONFIGURATION,

TOTAL_NUMBER_PROTECTION
};

/*****************************************/
//Константи для ранжування дискретних входів
/*****************************************/
enum __rang_small {
RANG_SMALL_BLOCK_VKL_VV = 0,
RANG_SMALL_RESET_LEDS,
RANG_SMALL_RESET_RELES,
RANG_SMALL_MISCEVE_DYSTANCIJNE,
RANG_SMALL_STATE_VV,
RANG_SMALL_OTKL_VID_ZOVN_ZAHYSTIV,
RANG_SMALL_VKL_VV,
RANG_SMALL_CTRL_VKL,
RANG_SMALL_OTKL_VV,
RANG_SMALL_CTRL_OTKL,
RANG_SMALL_1_GRUPA_USTAVOK,
RANG_SMALL_2_GRUPA_USTAVOK,
RANG_SMALL_3_GRUPA_USTAVOK,
RANG_SMALL_4_GRUPA_USTAVOK,
RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV,

RANG_SMALL_BLOCK_MTZ1,
RANG_SMALL_BLOCK_MTZ2,
RANG_SMALL_BLOCK_USK_MTZ2,
RANG_SMALL_BLOCK_MTZ3,
RANG_SMALL_BLOCK_MTZ4,

RANG_SMALL_BLOCK_MTZ04_1,
RANG_SMALL_BLOCK_MTZ04_2,
RANG_SMALL_BLOCK_USK_MTZ04_2,

RANG_SMALL_BLOCK_ZDZ,
RANG_SMALL_PUSK_ZDZ_VID_DV,

RANG_SMALL_BLOCK_NZZ,

RANG_SMALL_BLOCK_TZNP1,
RANG_SMALL_BLOCK_TZNP2,
RANG_SMALL_BLOCK_TZNP3,

RANG_SMALL_STAT_BLK_APV,

RANG_SMALL_ACHR_CHAPV_VID_DV,
RANG_SMALL_BLOCK_ACHR1,
RANG_SMALL_BLOCK_ACHR2,

RANG_SMALL_PUSK_UROV_VID_DV,

RANG_SMALL_BLOCK_ZOP,

RANG_SMALL_BLOCK_UMIN1,
RANG_SMALL_START_UMIN1,
RANG_SMALL_BLOCK_UMIN2,
RANG_SMALL_START_UMIN2,

RANG_SMALL_BLOCK_UMAX1,
RANG_SMALL_BLOCK_UMAX2,

RANG_SMALL_BLOCK_UP1,

RANG_SMALL_DF1_IN = (RANG_SMALL_BLOCK_UP1 + NUMBER_UP),
RANG_SMALL_DF2_IN,
RANG_SMALL_DF3_IN,
RANG_SMALL_DF4_IN,
RANG_SMALL_DF5_IN,
RANG_SMALL_DF6_IN,
RANG_SMALL_DF7_IN,
RANG_SMALL_DF8_IN,
RANG_SMALL_DT1_SET,
RANG_SMALL_DT1_RESET,
RANG_SMALL_DT2_SET,
RANG_SMALL_DT2_RESET,
RANG_SMALL_DT3_SET,
RANG_SMALL_DT3_RESET,
RANG_SMALL_DT4_SET,
RANG_SMALL_DT4_RESET,
};

#define NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL     15
#define NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL         5
#define NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL       3
#define NUMBER_ZDZ_SIGNAL_FOR_RANG_SMALL         2
#define NUMBER_ZZ_SIGNAL_FOR_RANG_SMALL          1
#define NUMBER_TZNP_SIGNAL_FOR_RANG_SMALL        3
#define NUMBER_APV_SIGNAL_FOR_RANG_SMALL         1
#define NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_SMALL  3
#define NUMBER_UROV_SIGNAL_FOR_RANG_SMALL        1
#define NUMBER_ZOP_SIGNAL_FOR_RANG_SMALL         1
#define NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL        4
#define NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL        2
#define NUMBER_UP_SIGNAL_FOR_RANG_SMALL          NUMBER_UP
#define NUMBER_VMP_SIGNAL_FOR_RANG_SMALL         0
#define NUMBER_EL_SIGNAL_FOR_RANG_SMALL          16

#define NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL       (                                          \
                                                  NUMBER_GENERAL_SIGNAL_FOR_RANG_SMALL    + \
                                                  NUMBER_MTZ_SIGNAL_FOR_RANG_SMALL        + \
                                                  NUMBER_MTZ04_SIGNAL_FOR_RANG_SMALL      + \
                                                  NUMBER_ZDZ_SIGNAL_FOR_RANG_SMALL        + \
                                                  NUMBER_ZZ_SIGNAL_FOR_RANG_SMALL         + \
                                                  NUMBER_TZNP_SIGNAL_FOR_RANG_SMALL       + \
                                                  NUMBER_APV_SIGNAL_FOR_RANG_SMALL        + \
                                                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_SMALL + \
                                                  NUMBER_UROV_SIGNAL_FOR_RANG_SMALL       + \
                                                  NUMBER_ZOP_SIGNAL_FOR_RANG_SMALL        + \
                                                  NUMBER_UMIN_SIGNAL_FOR_RANG_SMALL       + \
                                                  NUMBER_UMAX_SIGNAL_FOR_RANG_SMALL       + \
                                                  NUMBER_UP_SIGNAL_FOR_RANG_SMALL         + \
                                                  NUMBER_VMP_SIGNAL_FOR_RANG_SMALL        + \
                                                  NUMBER_EL_SIGNAL_FOR_RANG_SMALL           \
                                                 ) 
/*****************************************/

/*****************************************/
//Константи для ранжування дискретних виходів-свтодіодів-О-функцій-реєстраторів
/*****************************************/
enum __rang_output_led_df_reg {
RANG_BLOCK_VKL_VV = 0,
RANG_RESET_LEDS,
RANG_RESET_RELES,
RANG_MISCEVE_DYSTANCIJNE,
RANG_STATE_VV,
RANG_OTKL_VID_ZOVN_ZAHYSTIV,
RANG_VKL_VV,
RANG_CTRL_VKL,
RANG_OTKL_VV,
RANG_CTRL_OTKL,
RANG_PRYVID_VV,
RANG_PEREVYSHCHENNJA_Inom_VYMK,
RANG_KRYTYCHNYJ_RESURS_VYMYKACHA,
RANG_VYCHERPANYJ_RESURS_VYMYKACHA,
RANG_DEFECT,
RANG_AVAR_DEFECT,
RANG_WORK_A_REJESTRATOR,
RANG_WORK_D_REJESTRATOR,
RANG_VIDKL_VID_ZAKHYSTIV,
RANG_WORK_BO,
RANG_WORK_BV,
RANG_1_GRUPA_USTAVOK,
RANG_2_GRUPA_USTAVOK,
RANG_3_GRUPA_USTAVOK,
RANG_4_GRUPA_USTAVOK,
RANG_INVERS_DV_GRUPA_USTAVOK,
RANG_BLK_GRUP_USTAVOK_VID_ZACHYSTIV,
RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV,
RANG_READY_TU,
RANG_SETTINGS_CHANGED,

RANG_BLOCK_MTZ1,
RANG_BLOCK_MTZ2,
RANG_BLOCK_USK_MTZ2,
RANG_BLOCK_MTZ3,
RANG_BLOCK_MTZ4,
RANG_SECTOR_VPERED_MTZN1,
RANG_SECTOR_NAZAD_MTZN1,
RANG_PO_MTZ1,
RANG_PO_MTZN1_VPERED,
RANG_PO_MTZN1_NAZAD,
RANG_PO_U_MTZPN1,
RANG_PO_MTZPN1,
RANG_MTZ1,
RANG_SECTOR_VPERED_MTZN2,
RANG_SECTOR_NAZAD_MTZN2,
RANG_PO_MTZ2,
RANG_PO_MTZN2_VPERED,
RANG_PO_MTZN2_NAZAD,
RANG_PO_U_MTZPN2,
RANG_PO_MTZPN2,
RANG_MTZ2,
RANG_SECTOR_VPERED_MTZN3,
RANG_SECTOR_NAZAD_MTZN3,
RANG_PO_MTZ3,
RANG_PO_MTZN3_VPERED,
RANG_PO_MTZN3_NAZAD,
RANG_PO_U_MTZPN3,
RANG_PO_MTZPN3,
RANG_MTZ3,
RANG_SECTOR_VPERED_MTZN4,
RANG_SECTOR_NAZAD_MTZN4,
RANG_PO_MTZ4,
RANG_PO_MTZN4_VPERED,
RANG_PO_MTZN4_NAZAD,
RANG_PO_U_MTZPN4,
RANG_PO_MTZPN4,
RANG_MTZ4,
RANG_PO_BLOCK_U_MTZN,
RANG_NCN_MTZ,

RANG_BLOCK_MTZ04_1,
RANG_BLOCK_MTZ04_2,
RANG_BLOCK_USK_MTZ04_2,
RANG_PO_MTZ04_1,
RANG_MTZ04_1,
RANG_PO_MTZ04_2,
RANG_MTZ04_2,

RANG_BLOCK_ZDZ,
RANG_PUSK_ZDZ_VID_DV,
RANG_LIGHT_ZDZ_FROM_DV,
RANG_PO_ZDZ,
RANG_ZDZ,

RANG_BLOCK_NZZ,
RANG_PO_NZZ,
RANG_NZZ,
RANG_PO_3I0,
RANG_3I0,
RANG_PO_3U0,
RANG_3U0,
RANG_SECTOR_NZZ,

RANG_BLOCK_TZNP1,
RANG_SECTOR_TZNP1_VPERED,
RANG_SECTOR_TZNP1_NAZAD,
RANG_PO_3I0_TZNP1_VPERED,
RANG_PO_3I0_TZNP1_NAZAD,
RANG_PO_3U0_TZNP1_VPERED,
RANG_PO_3U0_TZNP1_NAZAD,
RANG_PO_TZNP1_VPERED,
RANG_PO_TZNP1_NAZAD,
RANG_TZNP1,
RANG_BLOCK_TZNP2,
RANG_SECTOR_TZNP2_VPERED,
RANG_SECTOR_TZNP2_NAZAD,
RANG_PO_3I0_TZNP2_VPERED,
RANG_PO_3I0_TZNP2_NAZAD,
RANG_PO_3U0_TZNP2_VPERED,
RANG_PO_3U0_TZNP2_NAZAD,
RANG_PO_TZNP2_VPERED,
RANG_PO_TZNP2_NAZAD,
RANG_TZNP2,
RANG_BLOCK_TZNP3,
RANG_SECTOR_TZNP3_VPERED,
RANG_SECTOR_TZNP3_NAZAD,
RANG_PO_3I0_TZNP3_VPERED,
RANG_PO_3I0_TZNP3_NAZAD,
RANG_PO_3U0_TZNP3_VPERED,
RANG_PO_3U0_TZNP3_NAZAD,
RANG_PO_TZNP3_VPERED,
RANG_PO_TZNP3_NAZAD,
RANG_TZNP3,

RANG_STAT_BLK_APV,
RANG_APV1,
RANG_APV2,
RANG_APV3,
RANG_APV4,
RANG_APV_WORK,

RANG_ACHR_CHAPV_VID_DV,
RANG_BLOCK_ACHR1,
RANG_BLOCK_ACHR2,
RANG_RAZR_CHAPV,
RANG_VN_BLOCK_CHAPV,
RANG_PO_ACHR1,
RANG_PO_CHAPV1,
RANG_ACHR_CHAPV1,
RANG_PO_ACHR2,
RANG_PO_CHAPV2,
RANG_ACHR_CHAPV2,

RANG_PUSK_UROV_VID_DV,
RANG_PO_UROV,
RANG_UROV1,
RANG_UROV2,

RANG_BLOCK_ZOP,
RANG_PO_ZOP,
RANG_ZOP,

RANG_BLOCK_UMIN1,
RANG_START_UMIN1,
RANG_BLOCK_UMIN2,
RANG_START_UMIN2,
RANG_PO_UMIN1,
RANG_PO_UBLK_UMIN1,
RANG_PO_IBLK_UMIN1,
RANG_UMIN1,
RANG_PO_UMIN2,
RANG_PO_UBLK_UMIN2,
RANG_PO_IBLK_UMIN2,
RANG_UMIN2,

RANG_BLOCK_UMAX1,
RANG_PO_UMAX1,
RANG_UMAX1,
RANG_BLOCK_UMAX2,
RANG_PO_UMAX2,
RANG_UMAX2,

RANG_BLOCK_UP1,
RANG_PO_UP1,
RANG_UP1,

RANG_DF1_IN = (RANG_BLOCK_UP1 + 3*NUMBER_UP),
RANG_DF1_OUT,
RANG_DF2_IN,
RANG_DF2_OUT,
RANG_DF3_IN,
RANG_DF3_OUT,
RANG_DF4_IN,
RANG_DF4_OUT,
RANG_DF5_IN,
RANG_DF5_OUT,
RANG_DF6_IN,
RANG_DF6_OUT,
RANG_DF7_IN,
RANG_DF7_OUT,
RANG_DF8_IN,
RANG_DF8_OUT,
RANG_DT1_SET,
RANG_DT1_RESET,
RANG_DT1_OUT,
RANG_DT2_SET,
RANG_DT2_RESET,
RANG_DT2_OUT,
RANG_DT3_SET,
RANG_DT3_RESET,
RANG_DT3_OUT,
RANG_DT4_SET,
RANG_DT4_RESET,
RANG_DT4_OUT,
RANG_D_AND1,
RANG_D_AND2,
RANG_D_AND3,
RANG_D_AND4,
RANG_D_AND5,
RANG_D_AND6,
RANG_D_AND7,
RANG_D_AND8,
RANG_D_OR1,
RANG_D_OR2,
RANG_D_OR3,
RANG_D_OR4,
RANG_D_OR5,
RANG_D_OR6,
RANG_D_OR7,
RANG_D_OR8,
RANG_D_XOR1,
RANG_D_XOR2,
RANG_D_XOR3,
RANG_D_XOR4,
RANG_D_XOR5,
RANG_D_XOR6,
RANG_D_XOR7,
RANG_D_XOR8,
RANG_D_NOT1,
RANG_D_NOT2,
RANG_D_NOT3,
RANG_D_NOT4,
RANG_D_NOT5,
RANG_D_NOT6,
RANG_D_NOT7,
RANG_D_NOT8,
RANG_D_NOT9,
RANG_D_NOT10,
RANG_D_NOT11,
RANG_D_NOT12,
RANG_D_NOT13,
RANG_D_NOT14,
RANG_D_NOT15,
RANG_D_NOT16,
RANG_ERROR_CONF_EL
};

#define NUMBER_GENERAL_SIGNAL_FOR_RANG     30
#define NUMBER_MTZ_SIGNAL_FOR_RANG         39
#define NUMBER_MTZ04_SIGNAL_FOR_RANG       7
#define NUMBER_ZDZ_SIGNAL_FOR_RANG         5
#define NUMBER_ZZ_SIGNAL_FOR_RANG          8
#define NUMBER_TZNP_SIGNAL_FOR_RANG        30
#define NUMBER_APV_SIGNAL_FOR_RANG         6
#define NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG  11
#define NUMBER_UROV_SIGNAL_FOR_RANG        4
#define NUMBER_ZOP_SIGNAL_FOR_RANG         3
#define NUMBER_UMIN_SIGNAL_FOR_RANG        12
#define NUMBER_UMAX_SIGNAL_FOR_RANG        6
#define NUMBER_UP_SIGNAL_FOR_RANG          (3*NUMBER_UP)
#define NUMBER_VMP_SIGNAL_FOR_RANG         0
#define NUMBER_EL_SIGNAL_FOR_RANG          69

#define NUMBER_TOTAL_SIGNAL_FOR_RANG       (                                    \
                                            NUMBER_GENERAL_SIGNAL_FOR_RANG    + \
                                            NUMBER_MTZ_SIGNAL_FOR_RANG        + \
                                            NUMBER_MTZ04_SIGNAL_FOR_RANG      + \
                                            NUMBER_ZDZ_SIGNAL_FOR_RANG        + \
                                            NUMBER_ZZ_SIGNAL_FOR_RANG         + \
                                            NUMBER_TZNP_SIGNAL_FOR_RANG       + \
                                            NUMBER_APV_SIGNAL_FOR_RANG        + \
                                            NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG + \
                                            NUMBER_UROV_SIGNAL_FOR_RANG       + \
                                            NUMBER_ZOP_SIGNAL_FOR_RANG        + \
                                            NUMBER_UMIN_SIGNAL_FOR_RANG       + \
                                            NUMBER_UMAX_SIGNAL_FOR_RANG       + \
                                            NUMBER_UP_SIGNAL_FOR_RANG         + \
                                            NUMBER_VMP_SIGNAL_FOR_RANG        + \
                                            NUMBER_EL_SIGNAL_FOR_RANG           \
                                           ) 

/*****************************************/

enum __mtz_const {
  RANG_BLOCK_MTZ = 0,
  RANG_BLOCK_USK_MTZ,
  RANG_SECTOR_VPERED_MTZN,
  RANG_SECTOR_NAZAD_MTZN,
  RANG_PO_MTZ,
  RANG_PO_MTZN_VPERED,
  RANG_PO_MTZN_NAZAD,
  RANG_PO_U_MTZPN,
  RANG_PO_MTZPN,
  RANG_MTZ,
  /*************************************/
  MTZ_SETTINGS_LENGTH
};

enum __mtz_levels_const {
  MTZ_LEVEL1 = 0,
  MTZ_LEVEL2,
  MTZ_LEVEL3,
  MTZ_LEVEL4,
  /*************************************/
  NUMBER_LEVEL_MTZ //Количество ступеней МТЗ
};

enum __mtz_abc_const {
  PHASE_A_INDEX = 0,
  PHASE_B_INDEX,
  PHASE_C_INDEX
};

enum __mtz_abc_direction_const {
  MTZ_NEVYZN = 0,
  MTZ_VPERED,
  MTZ_NAZAD
};

/*****************************************/
//Макски сигналів, які можна зранжувати на функціональні кнопки у Режимі Кнопка
/*****************************************/
#define MASKA_BUTTON_MODE_0_SIGNALS_0 (unsigned int)(         \
    (1 << (RANG_SMALL_RESET_LEDS - 0))                        \
  | (1 << (RANG_SMALL_RESET_RELES - 0))                       \
  | (1 << (RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0)) \
)

#define MASKA_BUTTON_MODE_0_SIGNALS_1 (unsigned int)(         \
    (1 << (RANG_SMALL_DF1_IN - 32))                           \
  | (1 << (RANG_SMALL_DF2_IN - 32))                           \
  | (1 << (RANG_SMALL_DF3_IN - 32))                           \
  | (1 << (RANG_SMALL_DF4_IN - 32))                           \
  | (1 << (RANG_SMALL_DF5_IN - 32))                           \
  | (1 << (RANG_SMALL_DF6_IN - 32))                           \
  | (1 << (RANG_SMALL_DF7_IN - 32))                           \
  | (1 << (RANG_SMALL_DF8_IN - 32))                           \
  | (1 << (RANG_SMALL_DT1_SET - 32))                          \
  | (1 << (RANG_SMALL_DT1_RESET - 32))                        \
  | (1 << (RANG_SMALL_DT2_SET - 32))                          \
  | (1 << (RANG_SMALL_DT2_RESET - 32))                        \
  | (1 << (RANG_SMALL_DT3_SET - 32))                          \
  | (1 << (RANG_SMALL_DT3_RESET - 32))                        \
  | (1 << (RANG_SMALL_DT4_SET - 32))                          \
)

#define MASKA_BUTTON_MODE_0_SIGNALS_2 (unsigned int)(         \
    (1 << (RANG_SMALL_DT4_RESET - 64))                        \
)
/*****************************************/

/*****************************************/
//Макски сигналів, які можна зранжувати на функціональні кнопки у Режимі Ключ
/*****************************************/
#define MASKA_BUTTON_MODE_1_SIGNALS_0 (unsigned int)(         \
    (1 << (RANG_SMALL_BLOCK_VKL_VV - 0))                      \
  | (1 << (RANG_SMALL_MISCEVE_DYSTANCIJNE - 0))               \
  | (1 << (RANG_SMALL_1_GRUPA_USTAVOK - 0))                   \
  | (1 << (RANG_SMALL_2_GRUPA_USTAVOK - 0))                   \
  | (1 << (RANG_SMALL_3_GRUPA_USTAVOK - 0))                   \
  | (1 << (RANG_SMALL_4_GRUPA_USTAVOK - 0))                   \
  | (1 << (RANG_SMALL_BLOCK_MTZ1 - 0))                        \
  | (1 << (RANG_SMALL_BLOCK_MTZ2 - 0))                        \
  | (1 << (RANG_SMALL_BLOCK_USK_MTZ2 - 0))                    \
  | (1 << (RANG_SMALL_BLOCK_MTZ3 - 0))                        \
  | (1 << (RANG_SMALL_BLOCK_MTZ4 - 0))                        \
  | (1 << (RANG_SMALL_BLOCK_MTZ04_1 - 0))                     \
  | (1 << (RANG_SMALL_BLOCK_MTZ04_2 - 0))                     \
  | (1 << (RANG_SMALL_BLOCK_USK_MTZ04_2 - 0))                 \
  | (1 << (RANG_SMALL_BLOCK_ZDZ - 0))                         \
  | (1 << (RANG_SMALL_BLOCK_NZZ - 0))                         \
  | (1 << (RANG_SMALL_BLOCK_TZNP1 - 0))                       \
  | (1 << (RANG_SMALL_BLOCK_TZNP2 - 0))                       \
  | (1 << (RANG_SMALL_BLOCK_TZNP3 - 0))                       \
  | (1 << (RANG_SMALL_STAT_BLK_APV - 0))                      \
  | (1 << (RANG_SMALL_BLOCK_ACHR1 - 0))                       \
)

#define MASKA_BUTTON_MODE_1_SIGNALS_1 (unsigned int)(         \
    (1 << (RANG_SMALL_BLOCK_ACHR2 - 32))                      \
  | (1 << (RANG_SMALL_BLOCK_ZOP - 32))                        \
  | (1 << (RANG_SMALL_BLOCK_UMIN1 - 32))                      \
  | (1 << (RANG_SMALL_BLOCK_UMIN2 - 32))                      \
  | (1 << (RANG_SMALL_BLOCK_UMAX1 - 32))                      \
  | (1 << (RANG_SMALL_BLOCK_UMAX2 - 32))                      \
  | (1 << (RANG_SMALL_BLOCK_UP1 + 0 - 32))                    \
  | (1 << (RANG_SMALL_BLOCK_UP1 + 1 - 32))                    \
  | (1 << (RANG_SMALL_BLOCK_UP1 + 2 - 32))                    \
  | (1 << (RANG_SMALL_BLOCK_UP1 + 3 - 32))                    \
  | (1 << (RANG_SMALL_BLOCK_UP1 + 4 - 32))                    \
  | (1 << (RANG_SMALL_BLOCK_UP1 + 5 - 32))                    \
  | (1 << (RANG_SMALL_BLOCK_UP1 + 6 - 32))                    \
  | (1 << (RANG_SMALL_BLOCK_UP1 + 7 - 32))                    \
  | (1 << (RANG_SMALL_DF1_IN - 32))                           \
  | (1 << (RANG_SMALL_DF2_IN - 32))                           \
  | (1 << (RANG_SMALL_DF3_IN - 32))                           \
  | (1 << (RANG_SMALL_DF4_IN - 32))                           \
  | (1 << (RANG_SMALL_DF5_IN - 32))                           \
  | (1 << (RANG_SMALL_DF6_IN - 32))                           \
  | (1 << (RANG_SMALL_DF7_IN - 32))                           \
  | (1 << (RANG_SMALL_DF8_IN - 32))                           \
  | (1 << (RANG_SMALL_DT1_SET - 32))                          \
  | (1 << (RANG_SMALL_DT1_RESET - 32))                        \
  | (1 << (RANG_SMALL_DT2_SET - 32))                          \
  | (1 << (RANG_SMALL_DT2_RESET - 32))                        \
  | (1 << (RANG_SMALL_DT3_SET - 32))                          \
  | (1 << (RANG_SMALL_DT3_RESET - 32))                        \
  | (1 << (RANG_SMALL_DT4_SET - 32))                          \
)

#define MASKA_BUTTON_MODE_1_SIGNALS_2 (unsigned int)(         \
    (1 << (RANG_SMALL_DT4_RESET - 64))                        \
)
/*****************************************/

/*****************************************/
//Макски сигналів, які активовуються або з д.входів, або з інтирфейсів чи функціональних кнопок
/*****************************************/

#define MASKA_FOR_INPUT_SIGNALS_0        (unsigned int)(        \
    (1 << (RANG_BLOCK_VKL_VV - 0))                              \
  | (1 << (RANG_RESET_LEDS - 0))                                \
  | (1 << (RANG_RESET_RELES - 0))                               \
  | (1 << (RANG_MISCEVE_DYSTANCIJNE - 0))                       \
  | (1 << (RANG_STATE_VV - 0))                                  \
  | (1 << (RANG_OTKL_VID_ZOVN_ZAHYSTIV - 0))                    \
  | (1 << (RANG_VKL_VV - 0))                                    \
  | (1 << (RANG_CTRL_VKL - 0))                                  \
  | (1 << (RANG_OTKL_VV - 0))                                   \
  | (1 << (RANG_CTRL_OTKL - 0))                                 \
  | (1 << (RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0))         \
  | (1 << (RANG_BLOCK_MTZ1 - 0))                                \
  | (1 << (RANG_BLOCK_MTZ2 - 0))                                \
)

#define MASKA_FOR_INPUT_SIGNALS_1        (unsigned int)(        \
    (1 << (RANG_BLOCK_USK_MTZ2 - 32))                           \
  | (1 << (RANG_BLOCK_MTZ3 - 32))                               \
  | (1 << (RANG_BLOCK_MTZ4 - 32))                               \
)

#define MASKA_FOR_INPUT_SIGNALS_2        (unsigned int)(      \
    (1 << (RANG_BLOCK_MTZ04_1 - 64))                          \
  | (1 << (RANG_BLOCK_MTZ04_2 - 64))                          \
  | (1 << (RANG_BLOCK_USK_MTZ04_2 - 64))                      \
  | (1 << (RANG_BLOCK_ZDZ - 64))                              \
  | (1 << (RANG_PUSK_ZDZ_VID_DV - 64))                        \
  | (1 << (RANG_BLOCK_NZZ - 64))                              \
  | (1 << (RANG_BLOCK_TZNP1 - 64))                            \
)
    
#define MASKA_FOR_INPUT_SIGNALS_3        (unsigned int)(       \
    (1 << (RANG_BLOCK_TZNP2 - 96))                             \
  | (1 << (RANG_BLOCK_TZNP3 - 96))                             \
  | (1 << (RANG_STAT_BLK_APV - 96))                            \
  | (1 << (RANG_ACHR_CHAPV_VID_DV - 96))                       \
  | (1 << (RANG_BLOCK_ACHR1 - 96))                             \
  | (1 << (RANG_BLOCK_ACHR2 - 96))                             \
)
    
#define MASKA_FOR_INPUT_SIGNALS_4        (unsigned int)(      \
    (1 << (RANG_PUSK_UROV_VID_DV - 128))                      \
  | (1 << (RANG_BLOCK_ZOP - 128))                             \
  | (1 << (RANG_BLOCK_UMIN1 - 128))                           \
  | (1 << (RANG_START_UMIN1 - 128))                           \
  | (1 << (RANG_BLOCK_UMIN2 - 128))                           \
  | (1 << (RANG_START_UMIN2 - 128))                           \
  | (1 << (RANG_BLOCK_UMAX1 - 128))                           \
  | (1 << (RANG_BLOCK_UMAX2 - 128))                           \
)
    
#define MASKA_FOR_INPUT_SIGNALS_5        (unsigned int)(      \
    (1 << (RANG_BLOCK_UP1 + 3*0 - 160))                       \
  | (1 << (RANG_BLOCK_UP1 + 3*1 - 160))                       \
  | (1 << (RANG_BLOCK_UP1 + 3*2 - 160))                       \
  | (1 << (RANG_BLOCK_UP1 + 3*3 - 160))                       \
  | (1 << (RANG_BLOCK_UP1 + 3*4 - 160))                       \
  | (1 << (RANG_BLOCK_UP1 + 3*5 - 160))                       \
  | (1 << (RANG_BLOCK_UP1 + 3*6 - 160))                       \
  | (1 << (RANG_BLOCK_UP1 + 3*7 - 160))                       \
  | (1 << (RANG_DF1_IN - 160))                                \
  | (1 << (RANG_DF2_IN - 160))                                \
  | (1 << (RANG_DF3_IN - 160))                                \
  | (1 << (RANG_DF4_IN - 160))                                \
)

#define MASKA_FOR_INPUT_SIGNALS_6        (unsigned int)(      \
    (1 << (RANG_DF5_IN - 192))                                \
  | (1 << (RANG_DF6_IN - 192))                                \
  | (1 << (RANG_DF7_IN - 192))                                \
  | (1 << (RANG_DF8_IN - 192))                                \
  | (1 << (RANG_DT1_SET - 192))                               \
  | (1 << (RANG_DT1_RESET - 192))                             \
  | (1 << (RANG_DT2_SET - 192))                               \
  | (1 << (RANG_DT2_RESET - 192))                             \
  | (1 << (RANG_DT3_SET - 192))                               \
  | (1 << (RANG_DT3_RESET - 192))                             \
  | (1 << (RANG_DT4_SET - 192))                               \
  | (1 << (RANG_DT4_RESET - 192))                             \
)

#define MASKA_FOR_INPUT_SIGNALS_7                  0
#define MASKA_FOR_INPUT_SIGNALS_8                  0
/*****************************************/

/*****************************************/
//Макски всіх сигналів МСЗ
/*****************************************/
#define MASKA_MTZ_SIGNALS_0        (unsigned int)(            \
     (1 << (RANG_BLOCK_MTZ1 - 0))                             \
   | (1 << (RANG_BLOCK_MTZ2 - 0))                             \
)

#define MASKA_MTZ_SIGNALS_1        (unsigned int)(            \
     (1 << (RANG_BLOCK_USK_MTZ2 - 32))                        \
   | (1 << (RANG_BLOCK_MTZ3 - 32))                            \
   | (1 << (RANG_BLOCK_MTZ4 - 32))                            \
   | (1 << (RANG_SECTOR_VPERED_MTZN1 - 32))                   \
   | (1 << (RANG_SECTOR_NAZAD_MTZN1 - 32))                    \
   | (1 << (RANG_PO_MTZ1 - 32))                               \
   | (1 << (RANG_PO_MTZN1_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN1_NAZAD - 32))                        \
   | (1 << (RANG_PO_U_MTZPN1 - 32))                           \
   | (1 << (RANG_PO_MTZPN1 - 32))                             \
   | (1 << (RANG_MTZ1 - 32))                                  \
   | (1 << (RANG_SECTOR_VPERED_MTZN2 - 32))                   \
   | (1 << (RANG_SECTOR_NAZAD_MTZN2 - 32))                    \
   | (1 << (RANG_PO_MTZ2 - 32))                               \
   | (1 << (RANG_PO_MTZN2_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN2_NAZAD - 32))                        \
   | (1 << (RANG_PO_U_MTZPN2 - 32))                           \
   | (1 << (RANG_PO_MTZPN2 - 32))                             \
   | (1 << (RANG_MTZ2 - 32))                                  \
   | (1 << (RANG_SECTOR_VPERED_MTZN3 - 32))                   \
   | (1 << (RANG_SECTOR_NAZAD_MTZN3 - 32))                    \
   | (1 << (RANG_PO_MTZ3 - 32))                               \
   | (1 << (RANG_PO_MTZN3_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN3_NAZAD - 32))                        \
   | (1 << (RANG_PO_U_MTZPN3 - 32))                           \
   | (1 << (RANG_PO_MTZPN3 - 32))                             \
   | (1 << (RANG_MTZ3 - 32))                                  \
   | (1 << (RANG_SECTOR_VPERED_MTZN4 - 32))                   \
   | (1 << (RANG_SECTOR_NAZAD_MTZN4 - 32))                    \
   | (1 << (RANG_PO_MTZ4 - 32))                               \
   | (1 << (RANG_PO_MTZN4_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN4_NAZAD - 32))                        \
)

#define MASKA_MTZ_SIGNALS_2        (unsigned int)(            \
     (1 << (RANG_PO_U_MTZPN4 - 64))                           \
   | (1 << (RANG_PO_MTZPN4 - 64))                             \
   | (1 << (RANG_MTZ4 - 64))                                  \
   | (1 << (RANG_PO_BLOCK_U_MTZN - 64))                       \
   | (1 << (RANG_NCN_MTZ - 64))                               \
)

#define MASKA_MTZ_SIGNALS_3                  0
#define MASKA_MTZ_SIGNALS_4                  0
#define MASKA_MTZ_SIGNALS_5                  0
#define MASKA_MTZ_SIGNALS_6                  0
#define MASKA_MTZ_SIGNALS_7                  0
#define MASKA_MTZ_SIGNALS_8                  0
/*****************************************/
     
/*****************************************/
//Макски всіх сигналів МСЗ 0.4кВ
/*****************************************/
#define MASKA_MTZ04_SIGNALS_0                0
#define MASKA_MTZ04_SIGNALS_1                0

#define MASKA_MTZ04_SIGNALS_2      (unsigned int)(            \
     (1 << (RANG_BLOCK_MTZ04_1 - 64))                         \
   | (1 << (RANG_BLOCK_MTZ04_2 - 64))                         \
   | (1 << (RANG_BLOCK_USK_MTZ04_2 - 64))                     \
   | (1 << (RANG_PO_MTZ04_1 - 64))                            \
   | (1 << (RANG_MTZ04_1 - 64))                               \
   | (1 << (RANG_PO_MTZ04_2 - 64))                            \
   | (1 << (RANG_MTZ04_2 - 64))                               \
)

#define MASKA_MTZ04_SIGNALS_3                0
#define MASKA_MTZ04_SIGNALS_4                0
#define MASKA_MTZ04_SIGNALS_5                0
#define MASKA_MTZ04_SIGNALS_6                0
#define MASKA_MTZ04_SIGNALS_7                0
#define MASKA_MTZ04_SIGNALS_8                0
/*****************************************/
     
/*****************************************/
//Макски всіх сигналів ЗДЗ
/*****************************************/
#define MASKA_ZDZ_SIGNALS_0                  0
#define MASKA_ZDZ_SIGNALS_1                  0

#define MASKA_ZDZ_SIGNALS_2        (unsigned int)(            \
     (1 << (RANG_BLOCK_ZDZ - 64))                             \
   | (1 << (RANG_PUSK_ZDZ_VID_DV - 64))                       \
   | (1 << (RANG_LIGHT_ZDZ_FROM_DV - 64))                     \
   | (1 << (RANG_PO_ZDZ - 64))                                \
   | (1 << (RANG_ZDZ - 64))                                   \
)

#define MASKA_ZDZ_SIGNALS_3                  0
#define MASKA_ZDZ_SIGNALS_4                  0
#define MASKA_ZDZ_SIGNALS_5                  0
#define MASKA_ZDZ_SIGNALS_6                  0
#define MASKA_ZDZ_SIGNALS_7                  0
#define MASKA_ZDZ_SIGNALS_8                  0
/*****************************************/
     
/*****************************************/
//Макски всіх сигналів НЗЗ
/*****************************************/
#define MASKA_NZZ_SIGNALS_0                  0
#define MASKA_NZZ_SIGNALS_1                  0

#define MASKA_NZZ_SIGNALS_2        (unsigned int)(            \
     (1 << (RANG_BLOCK_NZZ - 64))                             \
   | (1 << (RANG_PO_NZZ - 64))                                \
   | (1 << (RANG_NZZ - 64))                                   \
   | (1 << (RANG_PO_3I0 - 64))                                \
   | (1 << (RANG_3I0 - 64))                                   \
   | (1 << (RANG_PO_3U0 - 64))                                \
   | (1 << (RANG_3U0 - 64))                                   \
   | (1 << (RANG_SECTOR_NZZ - 64))                            \
)

#define MASKA_NZZ_SIGNALS_3                  0
#define MASKA_NZZ_SIGNALS_4                  0
#define MASKA_NZZ_SIGNALS_5                  0
#define MASKA_NZZ_SIGNALS_6                  0
#define MASKA_NZZ_SIGNALS_7                  0
#define MASKA_NZZ_SIGNALS_8                  0
/*****************************************/
     
/*****************************************/
//Макски всіх сигналів СЗНП
/*****************************************/
#define MASKA_TZNP_SIGNALS_0                 0
#define MASKA_TZNP_SIGNALS_1                 0
     
#define MASKA_TZNP_SIGNALS_2       (unsigned int)(            \
     (1 << (RANG_BLOCK_TZNP1 - 64))                           \
   | (1 << (RANG_SECTOR_TZNP1_VPERED - 64))                   \
   | (1 << (RANG_SECTOR_TZNP1_NAZAD - 64))                    \
   | (1 << (RANG_PO_3I0_TZNP1_VPERED - 64))                   \
   | (1 << (RANG_PO_3I0_TZNP1_NAZAD - 64))                    \
   | (1 << (RANG_PO_3U0_TZNP1_VPERED - 64))                   \
   | (1 << (RANG_PO_3U0_TZNP1_NAZAD - 64))                    \
)

#define MASKA_TZNP_SIGNALS_3       (unsigned int)(            \
     (1 << (RANG_PO_TZNP1_VPERED - 96))                       \
   | (1 << (RANG_PO_TZNP1_NAZAD - 96))                        \
   | (1 << (RANG_TZNP1 - 96))                                 \
   | (1 << (RANG_BLOCK_TZNP2 - 96))                           \
   | (1 << (RANG_SECTOR_TZNP2_VPERED - 96))                   \
   | (1 << (RANG_SECTOR_TZNP2_NAZAD - 96))                    \
   | (1 << (RANG_PO_3I0_TZNP2_VPERED - 96))                   \
   | (1 << (RANG_PO_3I0_TZNP2_NAZAD - 96))                    \
   | (1 << (RANG_PO_3U0_TZNP2_VPERED - 96))                   \
   | (1 << (RANG_PO_3U0_TZNP2_NAZAD - 96))                    \
   | (1 << (RANG_PO_TZNP2_VPERED - 96))                       \
   | (1 << (RANG_PO_TZNP2_NAZAD - 96))                        \
   | (1 << (RANG_TZNP2 - 96))                                 \
   | (1 << (RANG_BLOCK_TZNP3 - 96))                           \
   | (1 << (RANG_SECTOR_TZNP3_VPERED - 96))                   \
   | (1 << (RANG_SECTOR_TZNP3_NAZAD - 96))                    \
   | (1 << (RANG_PO_3I0_TZNP3_VPERED - 96))                   \
   | (1 << (RANG_PO_3I0_TZNP3_NAZAD - 96))                    \
   | (1 << (RANG_PO_3U0_TZNP3_VPERED - 96))                   \
   | (1 << (RANG_PO_3U0_TZNP3_NAZAD - 96))                    \
   | (1 << (RANG_PO_TZNP3_VPERED - 96))                       \
   | (1 << (RANG_PO_TZNP3_NAZAD - 96))                        \
   | (1 << (RANG_TZNP3 - 96))                                 \
)

#define MASKA_TZNP_SIGNALS_4                 0
#define MASKA_TZNP_SIGNALS_5                 0
#define MASKA_TZNP_SIGNALS_6                 0
#define MASKA_TZNP_SIGNALS_7                 0
#define MASKA_TZNP_SIGNALS_8                 0
/*****************************************/
     
/*****************************************/
//Макски всіх сигналів АПВ
/*****************************************/
#define MASKA_APV_SIGNALS_0                  0
#define MASKA_APV_SIGNALS_1                  0
#define MASKA_APV_SIGNALS_2                  0
     
#define MASKA_APV_SIGNALS_3        (unsigned int)(            \
     (1 << (RANG_STAT_BLK_APV - 96))                          \
   | (1 << (RANG_APV1 - 96))                                  \
   | (1 << (RANG_APV2 - 96))                                  \
   | (1 << (RANG_APV3 - 96))                                  \
   | (1 << (RANG_APV4 - 96))                                  \
   | (1 << (RANG_APV_WORK - 96))                              \
)

#define MASKA_APV_SIGNALS_4                  0
#define MASKA_APV_SIGNALS_5                  0
#define MASKA_APV_SIGNALS_6                  0
#define MASKA_APV_SIGNALS_7                  0
#define MASKA_APV_SIGNALS_8                  0
/*****************************************/

/*****************************************/
//Макски всіх сигналів АЧР/ЧАПВ
/*****************************************/
#define MASKA_ACHR_CHAPV_SIGNALS_0           0
#define MASKA_ACHR_CHAPV_SIGNALS_1           0
#define MASKA_ACHR_CHAPV_SIGNALS_2           0

#define MASKA_ACHR_CHAPV_SIGNALS_3 (unsigned int)(            \
     (1 << (RANG_ACHR_CHAPV_VID_DV - 96))                     \
   | (1 << (RANG_BLOCK_ACHR1 - 96))                           \
   | (1 << (RANG_BLOCK_ACHR2 - 96))                           \
)

#define MASKA_ACHR_CHAPV_SIGNALS_4 (unsigned int)(            \
     (1 << (RANG_RAZR_CHAPV - 128))                           \
   | (1 << (RANG_VN_BLOCK_CHAPV - 128))                       \
   | (1 << (RANG_PO_ACHR1 - 128))                             \
   | (1 << (RANG_PO_CHAPV1 - 128))                            \
   | (1 << (RANG_ACHR_CHAPV1 - 128))                          \
   | (1 << (RANG_PO_ACHR2 - 128))                             \
   | (1 << (RANG_PO_CHAPV2 - 128))                            \
   | (1 << (RANG_ACHR_CHAPV2 - 128))                          \
)

#define MASKA_ACHR_CHAPV_SIGNALS_5           0
#define MASKA_ACHR_CHAPV_SIGNALS_6           0
#define MASKA_ACHR_CHAPV_SIGNALS_7           0
#define MASKA_ACHR_CHAPV_SIGNALS_8           0
/*****************************************/
     
/*****************************************/
//Макски всіх сигналів ПРВВ
/*****************************************/
#define MASKA_UROV_SIGNALS_0                 0
#define MASKA_UROV_SIGNALS_1                 0
#define MASKA_UROV_SIGNALS_2                 0
#define MASKA_UROV_SIGNALS_3                 0

#define MASKA_UROV_SIGNALS_4       (unsigned int)(            \
     (1 << (RANG_PUSK_UROV_VID_DV - 128))                     \
   | (1 << (RANG_PO_UROV - 128))                              \
   | (1 << (RANG_UROV1 - 128))                                \
   | (1 << (RANG_UROV2 - 128))                                \
)

#define MASKA_UROV_SIGNALS_5                 0
#define MASKA_UROV_SIGNALS_6                 0
#define MASKA_UROV_SIGNALS_7                 0
#define MASKA_UROV_SIGNALS_8                 0
/*****************************************/

/*****************************************/
//Макски всіх сигналів ЗЗП
/*****************************************/
#define MASKA_ZOP_SIGNALS_0                  0
#define MASKA_ZOP_SIGNALS_1                  0
#define MASKA_ZOP_SIGNALS_2                  0
#define MASKA_ZOP_SIGNALS_3                  0

#define MASKA_ZOP_SIGNALS_4        (unsigned int)(            \
     (1 << (RANG_BLOCK_ZOP - 128))                            \
   | (1 << (RANG_PO_ZOP - 128))                               \
   | (1 << (RANG_ZOP - 128))                                  \
)

#define MASKA_ZOP_SIGNALS_5                  0
#define MASKA_ZOP_SIGNALS_6                  0
#define MASKA_ZOP_SIGNALS_7                  0
#define MASKA_ZOP_SIGNALS_8                  0
/*****************************************/

/*****************************************/
//Макски всіх сигналів ЗНмін
/*****************************************/
#define MASKA_UMIN_SIGNALS_0                 0
#define MASKA_UMIN_SIGNALS_1                 0
#define MASKA_UMIN_SIGNALS_2                 0
#define MASKA_UMIN_SIGNALS_3                 0
     
#define MASKA_UMIN_SIGNALS_4       (unsigned int)(            \
     (1 << (RANG_BLOCK_UMIN1 - 128))                          \
   | (1 << (RANG_START_UMIN1 - 128))                          \
   | (1 << (RANG_BLOCK_UMIN2 - 128))                          \
   | (1 << (RANG_START_UMIN2 - 128))                          \
   | (1 << (RANG_PO_UMIN1 - 128))                             \
   | (1 << (RANG_PO_UBLK_UMIN1 - 128))                        \
   | (1 << (RANG_PO_IBLK_UMIN1 - 128))                        \
   | (1 << (RANG_UMIN1 - 128))                                \
   | (1 << (RANG_PO_UMIN2 - 128))                             \
   | (1 << (RANG_PO_UBLK_UMIN2 - 128))                        \
   | (1 << (RANG_PO_IBLK_UMIN2 - 128))                        \
   | (1 << (RANG_UMIN2 - 128))                                \
)

#define MASKA_UMIN_SIGNALS_5                 0
#define MASKA_UMIN_SIGNALS_6                 0
#define MASKA_UMIN_SIGNALS_7                 0
#define MASKA_UMIN_SIGNALS_8                 0
/*****************************************/

/*****************************************/
//Макски всіх сигналів ЗНмакс
/*****************************************/
#define MASKA_UMAX_SIGNALS_0                 0
#define MASKA_UMAX_SIGNALS_1                 0
#define MASKA_UMAX_SIGNALS_2                 0
#define MASKA_UMAX_SIGNALS_3                 0
     
#define MASKA_UMAX_SIGNALS_4       (unsigned int)(            \
     (1 << (RANG_BLOCK_UMAX1 - 128))                          \
   | (1 << (RANG_PO_UMAX1 - 128))                             \
   | (1 << (RANG_UMAX1 - 128))                                \
   | (1 << (RANG_BLOCK_UMAX2 - 128))                          \
   | (1 << (RANG_PO_UMAX2 - 128))                             \
)

#define MASKA_UMAX_SIGNALS_5       (unsigned int)(            \
     (1 << (RANG_UMAX2 - 160))                                \
)

#define MASKA_UMAX_SIGNALS_6                 0
#define MASKA_UMAX_SIGNALS_7                 0
#define MASKA_UMAX_SIGNALS_8                 0
/*****************************************/

/*****************************************/
//Макски всіх сигналів Універсального Захисту
/*****************************************/
#define MASKA_UP_SIGNALS_0                 0
#define MASKA_UP_SIGNALS_1                 0
#define MASKA_UP_SIGNALS_2                 0
#define MASKA_UP_SIGNALS_3                 0
#define MASKA_UP_SIGNALS_4                 0

#define MASKA_UP_SIGNALS_5       (unsigned int)(            \
     (1 << (RANG_BLOCK_UP1 + 3*0 - 160))                    \
   | (1 << (RANG_PO_UP1 + 3*0 - 160))                       \
   | (1 << (RANG_UP1 + 3*0 - 160))                          \
   | (1 << (RANG_BLOCK_UP1 + 3*1 - 160))                    \
   | (1 << (RANG_PO_UP1 + 3*1 - 160))                       \
   | (1 << (RANG_UP1 + 3*1 - 160))                          \
   | (1 << (RANG_BLOCK_UP1 + 3*2 - 160))                    \
   | (1 << (RANG_PO_UP1 + 3*2 - 160))                       \
   | (1 << (RANG_UP1 + 3*2 - 160))                          \
   | (1 << (RANG_BLOCK_UP1 + 3*3 - 160))                    \
   | (1 << (RANG_PO_UP1 + 3*3 - 160))                       \
   | (1 << (RANG_UP1 + 3*3 - 160))                          \
   | (1 << (RANG_BLOCK_UP1 + 3*4 - 160))                    \
   | (1 << (RANG_PO_UP1 + 3*4 - 160))                       \
   | (1 << (RANG_UP1 + 3*4 - 160))                          \
   | (1 << (RANG_BLOCK_UP1 + 3*5 - 160))                    \
   | (1 << (RANG_PO_UP1 + 3*5 - 160))                       \
   | (1 << (RANG_UP1 + 3*5 - 160))                          \
   | (1 << (RANG_BLOCK_UP1 + 3*6 - 160))                    \
   | (1 << (RANG_PO_UP1 + 3*6 - 160))                       \
   | (1 << (RANG_UP1 + 3*6 - 160))                          \
   | (1 << (RANG_BLOCK_UP1 + 3*7 - 160))                    \
   | (1 << (RANG_PO_UP1 + 3*7 - 160))                       \
   | (1 << (RANG_UP1 + 3*7 - 160))                          \
)

#define MASKA_UP_SIGNALS_6                 0
#define MASKA_UP_SIGNALS_7                 0
#define MASKA_UP_SIGNALS_8                 0
/*****************************************/

/*****************************************/
//Макски всіх сигналів розширеної логіки
/*****************************************/
#define MASKA_EL_SIGNALS_0                  0
#define MASKA_EL_SIGNALS_1                  0
#define MASKA_EL_SIGNALS_2                  0
#define MASKA_EL_SIGNALS_3                  0
#define MASKA_EL_SIGNALS_4                  0
    
#define MASKA_EL_SIGNALS_5        (unsigned int)(             \
    (1 << (RANG_DF1_IN - 160))                                \
  | (1 << (RANG_DF1_OUT - 160))                               \
  | (1 << (RANG_DF2_IN - 160))                                \
  | (1 << (RANG_DF2_OUT - 160))                               \
  | (1 << (RANG_DF3_IN - 160))                                \
  | (1 << (RANG_DF3_OUT - 160))                               \
  | (1 << (RANG_DF4_IN - 160))                                \
)

#define MASKA_EL_SIGNALS_6        (unsigned int)(             \
    (1 << (RANG_DF4_OUT - 192))                               \
  | (1 << (RANG_DF5_IN - 192))                                \
  | (1 << (RANG_DF5_OUT - 192))                               \
  | (1 << (RANG_DF6_IN - 192))                                \
  | (1 << (RANG_DF6_OUT - 192))                               \
  | (1 << (RANG_DF7_IN - 192))                                \
  | (1 << (RANG_DF7_OUT - 192))                               \
  | (1 << (RANG_DF8_IN - 192))                                \
  | (1 << (RANG_DF8_OUT - 192))                               \
  | (1 << (RANG_DT1_SET - 192))                               \
  | (1 << (RANG_DT1_RESET - 192))                             \
  | (1 << (RANG_DT1_OUT - 192))                               \
  | (1 << (RANG_DT2_SET - 192))                               \
  | (1 << (RANG_DT2_RESET - 192))                             \
  | (1 << (RANG_DT2_OUT - 192))                               \
  | (1 << (RANG_DT3_SET - 192))                               \
  | (1 << (RANG_DT3_RESET - 192))                             \
  | (1 << (RANG_DT3_OUT - 192))                               \
  | (1 << (RANG_DT4_SET - 192))                               \
  | (1 << (RANG_DT4_RESET - 192))                             \
  | (1 << (RANG_DT4_OUT - 192))                               \
  | (1 << (RANG_D_AND1 - 192))                                \
  | (1 << (RANG_D_AND2 - 192))                                \
  | (1 << (RANG_D_AND3 - 192))                                \
  | (1 << (RANG_D_AND4 - 192))                                \
  | (1 << (RANG_D_AND5 - 192))                                \
  | (1 << (RANG_D_AND6 - 192))                                \
  | (1 << (RANG_D_AND7 - 192))                                \
  | (1 << (RANG_D_AND8 - 192))                                \
  | (1 << (RANG_D_OR1 - 192))                                 \
  | (1 << (RANG_D_OR2 - 192))                                 \
  | (1 << (RANG_D_OR3 - 192))                                 \
)

#define MASKA_EL_SIGNALS_7        (unsigned int)(             \
    (1 << (RANG_D_OR4 - 224))                                 \
  | (1 << (RANG_D_OR5 - 224))                                 \
  | (1 << (RANG_D_OR6 - 224))                                 \
  | (1 << (RANG_D_OR7 - 224))                                 \
  | (1 << (RANG_D_OR8 - 224))                                 \
  | (1 << (RANG_D_XOR1 - 224))                                \
  | (1 << (RANG_D_XOR2 - 224))                                \
  | (1 << (RANG_D_XOR3 - 224))                                \
  | (1 << (RANG_D_XOR4 - 224))                                \
  | (1 << (RANG_D_XOR5 - 224))                                \
  | (1 << (RANG_D_XOR6 - 224))                                \
  | (1 << (RANG_D_XOR7 - 224))                                \
  | (1 << (RANG_D_XOR8 - 224))                                \
  | (1 << (RANG_D_NOT1 - 224))                                \
  | (1 << (RANG_D_NOT2 - 224))                                \
  | (1 << (RANG_D_NOT3 - 224))                                \
  | (1 << (RANG_D_NOT4 - 224))                                \
  | (1 << (RANG_D_NOT5 - 224))                                \
  | (1 << (RANG_D_NOT6 - 224))                                \
  | (1 << (RANG_D_NOT7 - 224))                                \
  | (1 << (RANG_D_NOT8 - 224))                                \
  | (1 << (RANG_D_NOT9 - 224))                                \
  | (1 << (RANG_D_NOT10 - 224))                               \
  | (1 << (RANG_D_NOT11 - 224))                               \
  | (1 << (RANG_D_NOT12 - 224))                               \
  | (1 << (RANG_D_NOT13 - 224))                               \
  | (1 << (RANG_D_NOT14 - 224))                               \
  | (1 << (RANG_D_NOT15 - 224))                               \
  | (1 << (RANG_D_NOT16 - 224))                               \
  | (1 << (RANG_ERROR_CONF_EL - 224))                         \
)

#define MASKA_EL_SIGNALS_8                  0
/*****************************************/


/*****************************************/
//Макска сигналів, які мають записуватися у енергонезалежну пам'ять
/*****************************************/
#define MASKA_TRIGGER_SIGNALES_0                  0
#define MASKA_TRIGGER_SIGNALES_1                  0
#define MASKA_TRIGGER_SIGNALES_2                  0
#define MASKA_TRIGGER_SIGNALES_3                  0
#define MASKA_TRIGGER_SIGNALES_4                  0
#define MASKA_TRIGGER_SIGNALES_5                  0

#define MASKA_TRIGGER_SIGNALES_6               (unsigned int)(\
     (1 << (RANG_DT1_OUT - 192))                              \
   | (1 << (RANG_DT2_OUT - 192))                              \
   | (1 << (RANG_DT3_OUT - 192))                              \
   | (1 << (RANG_DT4_OUT - 192))                              \
)     

#define MASKA_TRIGGER_SIGNALES_7                  0
#define MASKA_TRIGGER_SIGNALES_8                  0
/*****************************************/

/*****************************************/
//Макска для блокування зміни груп  уставок
/*****************************************/
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_0 (unsigned int)(\
     (1 << (RANG_WORK_BO - 0))                                \
   | (1 << (RANG_WORK_BV - 0))                                \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_1 (unsigned int)(\
     (1 << (RANG_PO_MTZ1 - 32))                               \
   | (1 << (RANG_PO_MTZN1_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN1_NAZAD - 32))                        \
   | (1 << (RANG_PO_MTZPN1 - 32))                             \
   | (1 << (RANG_MTZ1 - 32))                                  \
   | (1 << (RANG_PO_MTZ2 - 32))                               \
   | (1 << (RANG_PO_MTZN2_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN2_NAZAD - 32))                        \
   | (1 << (RANG_PO_MTZPN2 - 32))                             \
   | (1 << (RANG_MTZ2 - 32))                                  \
   | (1 << (RANG_PO_MTZ3 - 32))                               \
   | (1 << (RANG_PO_MTZN3_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN3_NAZAD - 32))                        \
   | (1 << (RANG_PO_MTZPN3 - 32))                             \
   | (1 << (RANG_MTZ3 - 32))                                  \
   | (1 << (RANG_PO_MTZ4 - 32))                               \
   | (1 << (RANG_PO_MTZN4_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN4_NAZAD - 32))                        \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_2 (unsigned int)(\
     (1 << (RANG_PO_MTZPN4 - 64))                             \
   | (1 << (RANG_MTZ4 - 64))                                  \
   | (1 << (RANG_PO_MTZ04_1 - 64))                            \
   | (1 << (RANG_MTZ04_1 - 64))                               \
   | (1 << (RANG_PO_MTZ04_2 - 64))                            \
   | (1 << (RANG_MTZ04_2 - 64))                               \
   | (1 << (RANG_PO_NZZ - 64))                                \
   | (1 << (RANG_NZZ - 64))                                   \
   | (1 << (RANG_PO_3I0 - 64))                                \
   | (1 << (RANG_3I0 - 64))                                   \
   | (1 << (RANG_PO_3U0 - 64))                                \
   | (1 << (RANG_3U0 - 64))                                   \
   | (1 << (RANG_PO_3I0_TZNP1_VPERED - 64))                   \
   | (1 << (RANG_PO_3I0_TZNP1_NAZAD - 64))                    \
   | (1 << (RANG_PO_3U0_TZNP1_VPERED - 64))                   \
   | (1 << (RANG_PO_3U0_TZNP1_NAZAD - 64))                    \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_3 (unsigned int)(\
     (1 << (RANG_PO_TZNP1_VPERED - 96))                       \
   | (1 << (RANG_PO_TZNP1_NAZAD - 96))                        \
   | (1 << (RANG_TZNP1 - 96))                                 \
   | (1 << (RANG_PO_3I0_TZNP2_VPERED - 96))                   \
   | (1 << (RANG_PO_3I0_TZNP2_NAZAD - 96))                    \
   | (1 << (RANG_PO_3U0_TZNP2_VPERED - 96))                   \
   | (1 << (RANG_PO_3U0_TZNP2_NAZAD - 96))                    \
   | (1 << (RANG_PO_TZNP2_VPERED - 96))                       \
   | (1 << (RANG_PO_TZNP2_NAZAD - 96))                        \
   | (1 << (RANG_TZNP2 - 96))                                 \
   | (1 << (RANG_PO_3I0_TZNP3_VPERED - 96))                   \
   | (1 << (RANG_PO_3I0_TZNP3_NAZAD - 96))                    \
   | (1 << (RANG_PO_3U0_TZNP3_VPERED - 96))                   \
   | (1 << (RANG_PO_3U0_TZNP3_NAZAD - 96))                    \
   | (1 << (RANG_PO_TZNP3_VPERED - 96))                       \
   | (1 << (RANG_PO_TZNP3_NAZAD - 96))                        \
   | (1 << (RANG_TZNP3 - 96))                                 \
   | (1 << (RANG_APV_WORK - 96))                              \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_4 (unsigned int)(\
     (1 << (RANG_PO_ACHR1 - 128))                             \
   | (1 << (RANG_ACHR_CHAPV1 - 128))                          \
   | (1 << (RANG_PO_ACHR2 - 128))                             \
   | (1 << (RANG_ACHR_CHAPV2 - 128))                          \
   | (1 << (RANG_PO_UROV - 128))                              \
   | (1 << (RANG_UROV1 - 128))                                \
   | (1 << (RANG_UROV2 - 128))                                \
   | (1 << (RANG_PO_ZOP - 128))                               \
   | (1 << (RANG_ZOP - 128))                                  \
   | (1 << (RANG_PO_UMIN1 - 128))                             \
   | (1 << (RANG_UMIN1 - 128))                                \
   | (1 << (RANG_PO_UMIN2 - 128))                             \
   | (1 << (RANG_UMIN2 - 128))                                \
   | (1 << (RANG_PO_UMAX1 - 128))                             \
   | (1 << (RANG_UMAX1 - 128))                                \
   | (1 << (RANG_PO_UMAX2 - 128))                             \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_5 (unsigned int)(\
     (1 << (RANG_UMAX2 - 160))                                \
   | (1 << (RANG_PO_UP1 + 3*0 - 160))                         \
   | (1 << (RANG_UP1 + 3*0 - 160))                            \
   | (1 << (RANG_PO_UP1 + 3*1 - 160))                         \
   | (1 << (RANG_UP1 + 3*1 - 160))                            \
   | (1 << (RANG_PO_UP1 + 3*2 - 160))                         \
   | (1 << (RANG_UP1 + 3*2 - 160))                            \
   | (1 << (RANG_PO_UP1 + 3*3 - 160))                         \
   | (1 << (RANG_UP1 + 3*3 - 160))                            \
   | (1 << (RANG_PO_UP1 + 3*4 - 160))                         \
   | (1 << (RANG_UP1 + 3*4 - 160))                            \
   | (1 << (RANG_PO_UP1 + 3*5 - 160))                         \
   | (1 << (RANG_UP1 + 3*5 - 160))                            \
   | (1 << (RANG_PO_UP1 + 3*6 - 160))                         \
   | (1 << (RANG_UP1 + 3*6 - 160))                            \
   | (1 << (RANG_PO_UP1 + 3*7 - 160))                         \
   | (1 << (RANG_UP1 + 3*7 - 160))                            \
)

#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_6  0
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_7  0
#define MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_8  0
/*****************************************/

/*****************************************/
//Макска інформативних сигналів
/*****************************************/

#define MASKA_INFO_SIGNALES_0  (unsigned int)(                \
     (1 << (RANG_STATE_VV - 0))                               \
   | (1 << (RANG_DEFECT - 0))                                 \
   | (1 << (RANG_AVAR_DEFECT - 0))                            \
   | (1 << (RANG_WORK_A_REJESTRATOR - 0))                     \
   | (1 << (RANG_WORK_D_REJESTRATOR - 0))                     \
   | (1 << (RANG_SETTINGS_CHANGED - 0))                       \
)     

#define MASKA_INFO_SIGNALES_1                  0
#define MASKA_INFO_SIGNALES_2                  0
#define MASKA_INFO_SIGNALES_3                  0
#define MASKA_INFO_SIGNALES_4                  0
#define MASKA_INFO_SIGNALES_5                  0
#define MASKA_INFO_SIGNALES_6                  0
#define MASKA_INFO_SIGNALES_7                  0
#define MASKA_INFO_SIGNALES_8                  0
/*****************************************/

/*****************************************/
/*
Макска для дискретного реєстратора, яка визначає, при активності яких функцій продовжувати утримувати дискретний реєстратор у активному стані
(якщо) він вже є активним до цього часу
*/
/*****************************************/
#define MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_0 (unsigned int)(\
     (1 << (RANG_OTKL_VID_ZOVN_ZAHYSTIV - 0))                  \
   | (1 << (RANG_VKL_VV - 0))                                  \
   | (1 << (RANG_OTKL_VV - 0))                                 \
   | (1 << (RANG_VIDKL_VID_ZAKHYSTIV - 0))                     \
   | (1 << (RANG_WORK_BO - 0))                                 \
   | (1 << (RANG_WORK_BV - 0))                                 \
)  

#define MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_1 (unsigned int)(\
     (1 << (RANG_PO_MTZ1 - 32))                                \
   | (1 << (RANG_PO_MTZN1_VPERED - 32))                        \
   | (1 << (RANG_PO_MTZN1_NAZAD - 32))                         \
   | (1 << (RANG_PO_MTZPN1 - 32))                              \
   | (1 << (RANG_MTZ1 - 32))                                   \
   | (1 << (RANG_PO_MTZ2 - 32))                                \
   | (1 << (RANG_PO_MTZN2_VPERED - 32))                        \
   | (1 << (RANG_PO_MTZN2_NAZAD - 32))                         \
   | (1 << (RANG_PO_MTZPN2 - 32))                              \
   | (1 << (RANG_MTZ2 - 32))                                   \
   | (1 << (RANG_PO_MTZ3 - 32))                                \
   | (1 << (RANG_PO_MTZN3_VPERED - 32))                        \
   | (1 << (RANG_PO_MTZN3_NAZAD - 32))                         \
   | (1 << (RANG_PO_MTZPN3 - 32))                              \
   | (1 << (RANG_MTZ3 - 32))                                   \
   | (1 << (RANG_PO_MTZ4 - 32))                                \
   | (1 << (RANG_PO_MTZN4_VPERED - 32))                        \
   | (1 << (RANG_PO_MTZN4_NAZAD - 32))                         \
)            

#define MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_2 (unsigned int)(\
     (1 << (RANG_PO_MTZPN4 - 64))                              \
   | (1 << (RANG_MTZ4 - 64))                                   \
   | (1 << (RANG_PO_MTZ04_1 - 64))                             \
   | (1 << (RANG_MTZ04_1 - 64))                                \
   | (1 << (RANG_PO_MTZ04_2 - 64))                             \
   | (1 << (RANG_MTZ04_2 - 64))                                \
   | (1 << (RANG_PO_NZZ - 64))                                 \
   | (1 << (RANG_NZZ - 64))                                    \
   | (1 << (RANG_PO_3I0 - 64))                                 \
   | (1 << (RANG_3I0 - 64))                                    \
)  

#define MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_3 (unsigned int)(\
     (1 << (RANG_TZNP1 - 96))                                  \
   | (1 << (RANG_TZNP2 - 96))                                  \
   | (1 << (RANG_TZNP3 - 96))                                  \
   | (1 << (RANG_APV1 - 96))                                   \
   | (1 << (RANG_APV2 - 96))                                   \
   | (1 << (RANG_APV3 - 96))                                   \
   | (1 << (RANG_APV4 - 96))                                   \
   | (1 << (RANG_APV_WORK - 96))                               \
)  

#define MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_4 (unsigned int)(\
     (1 << (RANG_PUSK_UROV_VID_DV - 128))                      \
   | (1 << (RANG_PO_UROV - 128))                               \
   | (1 << (RANG_UROV1 - 128))                                 \
   | (1 << (RANG_UROV2 - 128))                                 \
   | (1 << (RANG_PO_ZOP - 128))                                \
   | (1 << (RANG_ZOP - 128))                                   \
   | (1 << (RANG_PO_UMAX1 - 128))                              \
   | (1 << (RANG_UMAX1 - 128))                                 \
   | (1 << (RANG_PO_UMAX2 - 128))                              \
)

#define MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_5 (unsigned int)(\
     (1 << (RANG_UMAX2 - 160))                                 \
)

#define MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_6        0
#define MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_7        0
#define MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_8        0
/*****************************************/

/*****************************************/
//Макска моніторингу максимального фазного струму для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_PHASE_SIGNALES_0        0

#define MASKA_MONITOTYNG_PHASE_SIGNALES_1 (unsigned int)(     \
     (1 << (RANG_PO_MTZ1 - 32))                               \
   | (1 << (RANG_PO_MTZN1_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN1_NAZAD - 32))                        \
   | (1 << (RANG_PO_MTZPN1 - 32))                             \
   | (1 << (RANG_MTZ1 - 32))                                  \
   | (1 << (RANG_PO_MTZ2 - 32))                               \
   | (1 << (RANG_PO_MTZN2_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN2_NAZAD - 32))                        \
   | (1 << (RANG_PO_MTZPN2 - 32))                             \
   | (1 << (RANG_MTZ2 - 32))                                  \
   | (1 << (RANG_PO_MTZ3 - 32))                               \
   | (1 << (RANG_PO_MTZN3_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN3_NAZAD - 32))                        \
   | (1 << (RANG_PO_MTZPN3 - 32))                             \
   | (1 << (RANG_MTZ3 - 32))                                  \
   | (1 << (RANG_PO_MTZ4 - 32))                               \
   | (1 << (RANG_PO_MTZN4_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN4_NAZAD - 32))                        \
)

#define MASKA_MONITOTYNG_PHASE_SIGNALES_2 (unsigned int)(     \
     (1 << (RANG_PO_MTZPN4 - 64))                             \
   | (1 << (RANG_MTZ4 - 64))                                  \
)

#define MASKA_MONITOTYNG_PHASE_SIGNALES_3        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_4        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_5        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_6        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_7        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_8        0
/*****************************************/

/*****************************************/
//Макска моніторингу максимального фазного струму сторони 0.4кВ для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_0        0
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_1        0

#define MASKA_MONITOTYNG_PHASE04_SIGNALES_2 (unsigned int)(   \
     (1 << (RANG_PO_MTZ04_1 - 64))                            \
   | (1 << (RANG_MTZ04_1 - 64))                               \
   | (1 << (RANG_PO_MTZ04_2 - 64))                            \
   | (1 << (RANG_MTZ04_2 - 64))                               \
)

#define MASKA_MONITOTYNG_PHASE04_SIGNALES_3        0
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_4        0
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_5        0
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_6        0
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_7        0
#define MASKA_MONITOTYNG_PHASE04_SIGNALES_8        0
/*****************************************/

/*****************************************/
//Макска моніторингу максимального струму 3I0 для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_3I0_SIGNALES_0        0
#define MASKA_MONITOTYNG_3I0_SIGNALES_1        0

#define MASKA_MONITOTYNG_3I0_SIGNALES_2        (unsigned int)(\
     (1 << (RANG_PO_NZZ - 64))                                \
   | (1 << (RANG_NZZ - 64))                                   \
   | (1 << (RANG_PO_3I0 - 64))                                \
   | (1 << (RANG_3I0 - 64))                                   \
   | (1 << (RANG_PO_3I0_TZNP1_VPERED - 64))                   \
   | (1 << (RANG_PO_3I0_TZNP1_NAZAD - 64))                    \
)            

#define MASKA_MONITOTYNG_3I0_SIGNALES_3        (unsigned int)(\
     (1 << (RANG_PO_TZNP1_VPERED - 96))                       \
   | (1 << (RANG_PO_TZNP1_NAZAD - 96))                        \
   | (1 << (RANG_TZNP1 - 96))                                 \
   | (1 << (RANG_PO_3I0_TZNP2_VPERED - 96))                   \
   | (1 << (RANG_PO_3I0_TZNP2_NAZAD - 96))                    \
   | (1 << (RANG_PO_TZNP2_VPERED - 96))                       \
   | (1 << (RANG_PO_TZNP2_NAZAD - 96))                        \
   | (1 << (RANG_TZNP2 - 96))                                 \
   | (1 << (RANG_PO_3I0_TZNP3_VPERED - 96))                   \
   | (1 << (RANG_PO_3I0_TZNP3_NAZAD - 96))                    \
   | (1 << (RANG_PO_TZNP3_VPERED - 96))                       \
   | (1 << (RANG_PO_TZNP3_NAZAD - 96))                        \
   | (1 << (RANG_TZNP3 - 96))                                 \
)            

#define MASKA_MONITOTYNG_3I0_SIGNALES_4        0
#define MASKA_MONITOTYNG_3I0_SIGNALES_5        0
#define MASKA_MONITOTYNG_3I0_SIGNALES_6        0
#define MASKA_MONITOTYNG_3I0_SIGNALES_7        0
#define MASKA_MONITOTYNG_3I0_SIGNALES_8        0
/*****************************************/

/*****************************************/
//Макска моніторингу максимальної напргуи 3U0 для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_3U0_SIGNALES_0        0
#define MASKA_MONITOTYNG_3U0_SIGNALES_1        0

#define MASKA_MONITOTYNG_3U0_SIGNALES_2        (unsigned int)(\
     (1 << (RANG_PO_NZZ - 64))                                \
   | (1 << (RANG_NZZ - 64))                                   \
   | (1 << (RANG_PO_3U0 - 64))                                \
   | (1 << (RANG_3U0 - 64))                                   \
   | (1 << (RANG_PO_3U0_TZNP1_VPERED - 64))                   \
   | (1 << (RANG_PO_3U0_TZNP1_NAZAD - 64))                    \
)            

#define MASKA_MONITOTYNG_3U0_SIGNALES_3        (unsigned int)(\
     (1 << (RANG_PO_TZNP1_VPERED - 96))                       \
   | (1 << (RANG_PO_TZNP1_NAZAD - 96))                        \
   | (1 << (RANG_TZNP1 - 96))                                 \
   | (1 << (RANG_PO_3U0_TZNP2_VPERED - 96))                   \
   | (1 << (RANG_PO_3U0_TZNP2_NAZAD - 96))                    \
   | (1 << (RANG_PO_TZNP2_VPERED - 96))                       \
   | (1 << (RANG_PO_TZNP2_NAZAD - 96))                        \
   | (1 << (RANG_TZNP2 - 96))                                 \
   | (1 << (RANG_PO_3U0_TZNP3_VPERED - 96))                   \
   | (1 << (RANG_PO_3U0_TZNP3_NAZAD - 96))                    \
   | (1 << (RANG_PO_TZNP3_VPERED - 96))                       \
   | (1 << (RANG_PO_TZNP3_NAZAD - 96))                        \
   | (1 << (RANG_TZNP3 - 96))                                 \
)            

#define MASKA_MONITOTYNG_3U0_SIGNALES_4        0
#define MASKA_MONITOTYNG_3U0_SIGNALES_5        0
#define MASKA_MONITOTYNG_3U0_SIGNALES_6        0
#define MASKA_MONITOTYNG_3U0_SIGNALES_7        0
#define MASKA_MONITOTYNG_3U0_SIGNALES_8        0
/*****************************************/

/*****************************************/
//Макска моніторингу Umin для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_UMIN_SIGNALES_0        0
#define MASKA_MONITOTYNG_UMIN_SIGNALES_1        0
#define MASKA_MONITOTYNG_UMIN_SIGNALES_2        0
#define MASKA_MONITOTYNG_UMIN_SIGNALES_3        0

#define MASKA_MONITOTYNG_UMIN_SIGNALES_4 (unsigned int)(      \
     (1 << (RANG_PO_UMIN1 - 128))                             \
   | (1 << (RANG_UMIN1 - 128))                                \
   | (1 << (RANG_PO_UMIN2 - 128))                             \
   | (1 << (RANG_UMIN2 - 128))                                \
)

#define MASKA_MONITOTYNG_UMIN_SIGNALES_5        0
#define MASKA_MONITOTYNG_UMIN_SIGNALES_6        0
#define MASKA_MONITOTYNG_UMIN_SIGNALES_7        0
#define MASKA_MONITOTYNG_UMIN_SIGNALES_8        0
/*****************************************/

/*****************************************/
//Макска моніторингу Umax для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_UMAX_SIGNALES_0        0
#define MASKA_MONITOTYNG_UMAX_SIGNALES_1        0
#define MASKA_MONITOTYNG_UMAX_SIGNALES_2        0
#define MASKA_MONITOTYNG_UMAX_SIGNALES_3        0

#define MASKA_MONITOTYNG_UMAX_SIGNALES_4 (unsigned int)(      \
     (1 << (RANG_PO_UMAX1 - 128))                             \
   | (1 << (RANG_UMAX1 - 128))                                \
   | (1 << (RANG_PO_UMAX2 - 128))                             \
)

#define MASKA_MONITOTYNG_UMAX_SIGNALES_5 (unsigned int)(      \
     (1 << (RANG_UMAX2 - 160))                                \
)

#define MASKA_MONITOTYNG_UMAX_SIGNALES_6        0
#define MASKA_MONITOTYNG_UMAX_SIGNALES_7        0
#define MASKA_MONITOTYNG_UMAX_SIGNALES_8        0
/*****************************************/

/*****************************************/
//Макска моніторингу максимального струму зворотньої послідовності для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_ZOP_SIGNALES_0                  0
#define MASKA_MONITOTYNG_ZOP_SIGNALES_1                  0
#define MASKA_MONITOTYNG_ZOP_SIGNALES_2                  0
#define MASKA_MONITOTYNG_ZOP_SIGNALES_3                  0

#define MASKA_MONITOTYNG_ZOP_SIGNALES_4 (unsigned int)(       \
     (1 << (RANG_PO_ZOP - 128))                               \
   | (1 << (RANG_ZOP - 128))                                  \
)

#define MASKA_MONITOTYNG_ZOP_SIGNALES_5                  0
#define MASKA_MONITOTYNG_ZOP_SIGNALES_6                  0
#define MASKA_MONITOTYNG_ZOP_SIGNALES_7                  0
#define MASKA_MONITOTYNG_ZOP_SIGNALES_8                  0
/*****************************************/

/*****************************************/
//Макска моніторингу частоти при АЧР для дискретного реєстратора
/*****************************************/
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_0   0
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_1   0
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_2   0
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_3   0

#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_4   (unsigned int)(\
     (1 << (RANG_PO_ACHR1 - 128))                               \
   | (1 << (RANG_ACHR_CHAPV1 - 128))                            \
   | (1 << (RANG_PO_ACHR2 - 128))                               \
   | (1 << (RANG_ACHR_CHAPV2 - 128))                            \
)     

#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_5   0
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_6   0
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_7   0
#define MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_8   0
/*****************************************/

/*****************************************/
//Макска моніторингу максимального фазного струму для фіксації КЗ
/*****************************************/
#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_0        0

#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_1 (unsigned int)(  \
     (1 << (RANG_PO_MTZ1 - 32))                               \
   | (1 << (RANG_PO_MTZN1_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN1_NAZAD - 32))                        \
   | (1 << (RANG_PO_MTZPN1 - 32))                             \
   | (1 << (RANG_PO_MTZ2 - 32))                               \
   | (1 << (RANG_PO_MTZN2_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN2_NAZAD - 32))                        \
   | (1 << (RANG_PO_MTZPN2 - 32))                             \
   | (1 << (RANG_PO_MTZ3 - 32))                               \
   | (1 << (RANG_PO_MTZN3_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN3_NAZAD - 32))                        \
   | (1 << (RANG_PO_MTZPN3 - 32))                             \
   | (1 << (RANG_PO_MTZ4 - 32))                               \
   | (1 << (RANG_PO_MTZN4_VPERED - 32))                       \
   | (1 << (RANG_PO_MTZN4_NAZAD - 32))                        \
)

#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_2 (unsigned int)(  \
     (1 << (RANG_PO_MTZPN4 - 64))                             \
)

#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_3        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_4        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_5        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_6        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_7        0
#define MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_8        0
/*****************************************/


/*****************************************/
//Відключення від захистів
/*****************************************/
typedef enum __vymknennja_vid_zakhystiv__
{
  VYMKNENNJA_VID_MTZ1,
  VYMKNENNJA_VID_MTZ2,
  VYMKNENNJA_VID_MTZ3,
  VYMKNENNJA_VID_MTZ4,

  VYMKNENNJA_VID_MTZ04_1,
  VYMKNENNJA_VID_MTZ04_2,

  VYMKNENNJA_VID_ZDZ,

  VYMKNENNJA_VID_3I0,
  VYMKNENNJA_VID_3U0,
  VYMKNENNJA_VID_NZZ,

  VYMKNENNJA_VID_TZNP1,
  VYMKNENNJA_VID_TZNP2,
  VYMKNENNJA_VID_TZNP3,

  VYMKNENNJA_VID_ACHR_CHAPV_VID_DV,
  VYMKNENNJA_VID_ACHR_CHAPV1,
  VYMKNENNJA_VID_ACHR_CHAPV2,

  VYMKNENNJA_VID_UROV1,
  VYMKNENNJA_VID_UROV2,

  VYMKNENNJA_VID_ZOP,

  VYMKNENNJA_VID_UMIN1,
  VYMKNENNJA_VID_UMIN2,

  VYMKNENNJA_VID_UMAX1,
  VYMKNENNJA_VID_UMAX2,

  VYMKNENNJA_VID_UP1,
  
  VYMKNENNJA_VID_ZOVNISHNIKH_ZAKHYSTIV = VYMKNENNJA_VID_UP1 + NUMBER_UP,
  
  VYMKNENNJA_VID_INSHYKH_SYGNALIV,
    
  VYMKNENNJA_VID_MAX_NUMBER  
  
}__vymknennja_vid_zakhystiv;
/*****************************************/

/*****************************************/
//Світлоіндикатори
/*****************************************/
//Нумерація  усправлінських світлоіндикаторів
enum _leds_ctrl
{
  LED_CTRL_R_E = 0,
  LED_CTRL_START,
  LED_CTRL_TRIP,
  LED_CTRL_I,
  LED_CTRL_O
};

enum _leds_colors
{
  LED_COLOR_RED_BIT = 0,
  LED_COLOR_GREEN_BIT,
  
  NUMBER_LED_COLOR
};

//Кількість управлінських стовпців
#define LED_N_COL       7
#define LED_N_ROW       8
/*****************************************/

/*****************************************/
//Максимальна кількість сигналів (функцій), які можуть бути зранжовані на входи/виходи/світлоіндикатори/д.реєстратор/оф
/*****************************************/
#define VAGA_MAX_FUNCTIONS_IN_INPUT             3
#define MAX_FUNCTIONS_IN_INPUT                  (1 << VAGA_MAX_FUNCTIONS_IN_INPUT)
#define VAGA_MAX_FUNCTIONS_IN_OUTPUT            4
#define MAX_FUNCTIONS_IN_OUTPUT                 (1 << VAGA_MAX_FUNCTIONS_IN_OUTPUT)
#define VAGA_MAX_FUNCTIONS_IN_LED               3
#define MAX_FUNCTIONS_IN_LED                    (1 << VAGA_MAX_FUNCTIONS_IN_LED)
#define VAGA_MAX_FUNCTIONS_IN_DF                4
#define MAX_FUNCTIONS_IN_DF                     (1 << VAGA_MAX_FUNCTIONS_IN_DF)
#define MAX_FUNCTIONS_IN_DT                     6
#define MAX_FUNCTIONS_IN_D_AND                  NUMBER_IN_AND
#define MAX_FUNCTIONS_IN_D_OR                   NUMBER_IN_OR
#define MAX_FUNCTIONS_IN_D_XOR                  2
#define MAX_FUNCTIONS_IN_D_NOT                  1
#define VAGA_MAX_FUNCTIONS_IN_DB                3
#define MAX_FUNCTIONS_IN_DB                     (1 << VAGA_MAX_FUNCTIONS_IN_DB)
#define VAGA_MAX_FUNCTIONS_IN_AREG              5
#define MAX_FUNCTIONS_IN_AREG                   (1 << VAGA_MAX_FUNCTIONS_IN_AREG)
#define VAGA_MAX_FUNCTIONS_IN_DREG              5
#define MAX_FUNCTIONS_IN_DREG                   (1 << VAGA_MAX_FUNCTIONS_IN_DREG)
#define VAGA_MAX_FUNCTIONS_IN_OFF_CB            5
#define MAX_FUNCTIONS_IN_OFF_CB                 (1 << VAGA_MAX_FUNCTIONS_IN_OFF_CB)
#define VAGA_MAX_FUNCTIONS_IN_ON_CB             5
#define MAX_FUNCTIONS_IN_ON_CB                  (1 << VAGA_MAX_FUNCTIONS_IN_ON_CB)
/*****************************************/


/*****************************************/
//Константи для комунікації
/*****************************************/
#define MAX_CHAR_IN_NAME_OF_CELL                              32
/*****************************************/

/*****************************************/
//Константи для фіксації зміни настройок
/*****************************************/
#define CHANGED_ETAP_NONE                                     0
#define CHANGED_ETAP_EXECUTION                                1
#define CHANGED_ETAP_ENDED                                    2
#define CHANGED_ETAP_ENDED_EXTRA_ETAP                         3
/*****************************************/

/*****************************************/
//Константи для ранжування функціональних кнопок
/*****************************************/
enum __odynyci_vymirjuvannja {
INDEX_A = 0,
INDEX_V,
INDEX_SECOND,

NUMBER_ODYNYCI_VYMIRJUVANNJA
};
/*****************************************/

/*****************************************/
//Індетифікатор типу набору юстуючих коефіцієнтів
/*****************************************/
//#define ADJUSTMENT_3I0_IA_IB_IC                             1
//#define ADJUSTMENT_3I0_IA_IC_3U0                            2
//#define ADJUSTMENT_3I0_IA_IB_IC_UA_UB_UC_UABTN2             3
#define ADJUSTMENT_3I0_IA_IB_IC_UA_UB_UC_3U0                  4
#define ADJUSTMENT_ID                                         ADJUSTMENT_3I0_IA_IB_IC_UA_UB_UC_3U0
/*****************************************/

/*****************************************/
//Версія програмного забезпечення
/*****************************************/
#define VERSIA_PZ                                             14
#define MODYFIKACIA_VERSII_PZ                                 1
#define ZBIRKA_VERSII_PZ                                      0
#define ZBIRKA_PIDVERSII_PZ                                   4
/*****************************************/

/*****************************************/
//Час компіляції
/*****************************************/

#define YEAR_VER                                              ((__DATE__[9] != ' ') ? (((__DATE__[9] - 0x30) << 4) | (__DATE__[10] - 0x30)) : (__DATE__[10] - 0x30))
#define MONTH_VER                                             str_to_int_DATE_Mmm()
#define DAY_VER                                               ((__DATE__[4] != ' ') ? (((__DATE__[4] - 0x30) << 4) | (__DATE__[5] - 0x30)) : (__DATE__[5] - 0x30))
#define HOUR_VER                                              (((__TIME__[0] - 0x30) << 4) | (__TIME__[1] - 0x30))
#define MINUTE_VER                                            (((__TIME__[3] - 0x30) << 4) | (__TIME__[4] - 0x30))
#define SECOND_VER                                            (((__TIME__[6] - 0x30) << 4) | (__TIME__[7] - 0x30))
/*****************************************/

/*****************************************/
//Версія універсальної карти памяті
/*****************************************/
#define VERSIA_GMM                                             8
#define MODYFIKACIA_VERSII_GMM                                 0
/*****************************************/
#endif
