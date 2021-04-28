#ifndef __CONST_MENU_CHOSE_SETTINGS__
#define __CONST_MENU_CHOSE_SETTINGS__

#define EKRAN_CHOSE_SETTINGS        (EKRAN_STATE_OUTPUTS + 1)

enum _index_ml_choose_settings
{
  INDEX_OF_INFO = 0,
  INDEX_OF_SETTINGS_POINT,

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  INDEX_OF_IEC61850,
#endif

  INDEX_OF_INPUTS_RANGUVANNJA,
  INDEX_OF_OUTPUTS_RANGUVANNJA,
  INDEX_OF_LEDS_RANGUVANNJA,
  INDEX_OF_SWITCHER,
  INDEX_OF_TRANSFORMATOR,
  INDEX_OF_CONTROL_INPUTS_OUTPUTS,
  INDEX_OF_COMMUNICATION,
  INDEX_OF_DATA_AND_TIME,
  INDEX_OF_REGISTRATORS,
  INDEX_OF_EXTENDED_LOGIC,
  INDEX_OF_DEFINED_BUTTONS,
  INDEX_OF_GRUPA_USTAVOK,
  INDEX_OF_EXTRA_SETTINGS,
  INDEX_OF_LIST_PASSWORDS,
  
  MAX_ROW_FOR_CHOSE_SETTINGS
};

#endif
