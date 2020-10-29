#ifndef __CONST_MENU_INFO__
#define __CONST_MENU_INFO__

#define EKRAN_INFO                           (EKRAN_VIEW_GRUPA_USTAVOK + 1)
#define EKRAN_DATE_TIME_PZ                   (EKRAN_INFO + 1)

enum _index_ml_info
{
  INDEX_ML_INFO_FIRMWARE = 0,
#if (MODYFIKACIA_VERSII_PZ >= 10)  
  INDEX_ML_INFO_FIRMWARE_CB,
#endif
  INDEX_ML_INFO_MEMORY_MAP,
  INDEX_ML_INFO_SERIAL_NUMBER,
  
#if (MODYFIKACIA_VERSII_PZ >= 10)  
  INDEX_ML_INFO_MAC_ADDRESS,
#endif
  
  MAX_ROW_FOR_INFO
};

#define MAX_ROW_FOR_DATE_TIME_PZ             1 

#define COL_INFO_BEGIN_2                     5
#define COL_END_END_2                        9

#endif
