#ifndef __CONST_MENU_CHOSE_DATA_TIME_SETTINGS__
#define __CONST_MENU_CHOSE_DATA_TIME_SETTINGS__

#define EKRAN_CHOSE_DATA_TIME                    (_EKRAN_COMMUNICATION_PARAMETERS_LAST + 1)
#define EKRAN_TIME_ZONE                          (EKRAN_CHOSE_DATA_TIME + 1)
#if (MODYFIKACIA_VERSII_PZ >= 10)  
#define EKRAN_SYNCHRO                            (EKRAN_TIME_ZONE + 1)
#endif

#if (MODYFIKACIA_VERSII_PZ < 10)
#define _EKRAN_DATA_TIME_LAST                     EKRAN_TIME_ZONE
#else
#define _EKRAN_DATA_TIME_LAST                     EKRAN_SYNCHRO
#endif


enum _index_ml_data_time_settings
{
  INDEX_ML_CHDT_TIME_ZONE = 0,

#if (MODYFIKACIA_VERSII_PZ >= 10)  
  INDEX_ML_CHDT_SYNCHRO,
#endif
  
  MAX_ROW_FOR_CHOSE_DATA_TIME_SETTINGS
};

#if (MODYFIKACIA_VERSII_PZ >= 10)  
enum _index_ml_syn
{
  INDEX_ML_SYN_IPV4 = 0,
  INDEX_ML_SYN_PORT,
  INDEX_ML_SYN_PERIOD,
  
  MAX_ROW_FOR_SYNCHRO
};

#define COL_IP4_SERVER_BEGIN           1
#define COL_IP4_SERVER_END             MAX_COL_LCD

#define COL_PORT_BEGIN                 5
#define COL_PORT_END                   9

#define COL_PERIOD_BEGIN               6
#define COL_PEDIOD_END                 8

#endif

#endif
