#ifndef __CONST_MENU_CHOSE_SETTING_UVV__
#define __CONST_MENU_CHOSE_SETTING_UVV__

#define EKRAN_CHOOSE_SETTINGS_UVV        (EKRAN_RANGUVANNJA_ON_CB + 1)
#define EKRAN_DOPUSK_DV_UVV              (EKRAN_CHOOSE_SETTINGS_UVV + 1)
#define EKRAN_TYPE_INPUT_UVV             (EKRAN_DOPUSK_DV_UVV + 1)
#define EKRAN_TYPE_INPUT_SIGNAL_UVV      (EKRAN_TYPE_INPUT_UVV + 1)
#define EKRAN_TYPE_OUTPUT_UVV            (EKRAN_TYPE_INPUT_SIGNAL_UVV + 1)
#define EKRAN_TYPE_LED_UVV               (EKRAN_TYPE_OUTPUT_UVV + 1)
#define EKRAN_TYPE_BUTTON_UVV            (EKRAN_TYPE_LED_UVV + 1)

enum _index_ml_uvv
{
  INDEX_ML_UVV_DOPUSKT = 0,
  INDEX_ML_UVV_TYPE_INPUT,
  INDEX_ML_UVV_TYPE_INPUT_SIGNAL,
  INDEX_ML_UVV_TYPE_OUTPUT,
  INDEX_ML_UVV_TYPE_LED,
  INDEX_ML_UVV_TYPE_DB,
    
  MAX_ROW_FOR_CHOSE_SETTINGS_UVV
};

#define COL_DOPUSK_DV_BEGIN              5
#define COL_DOPUSK_DV_END                6


#endif
