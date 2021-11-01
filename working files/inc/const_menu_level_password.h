#ifndef __CONST_MENU_LEVEL_PASSWORD__
#define __CONST_MENU_LEVEL_PASSWORD__

#define EKRAN_LEVEL_PASSWORD            0
#define EKRAN_LEVEL_PASSWORD_HARD       (EKRAN_LEVEL_PASSWORD + 1)
#define EKRAN_LEVEL_PASSWORD_REPROGRAM  (EKRAN_LEVEL_PASSWORD_HARD + 1)
#define EKRAN_LEVEL_CHOOSE_PASSWORDS    (EKRAN_RANGUVANNJA_BUTTON_6 + 1)
#define EKRAN_LEVEL_SET_NEW_PASSWORD1   (EKRAN_LEVEL_CHOOSE_PASSWORDS + 1)
#define EKRAN_LEVEL_SET_NEW_PASSWORD2   (EKRAN_LEVEL_SET_NEW_PASSWORD1 + 1)
#define EKRAN_LEVEL_SET_NEW_PASSWORD3   (EKRAN_LEVEL_SET_NEW_PASSWORD2 + 1)

#define MAX_ROW_FOR_LEVEL_PASSWORD    2

#define MAX_NUMBER_OF_SYMPOLS_IN_PASSWORD       4
#define COL_NEW_PASSWORD_BEGIN                  5
#define COL_NEW_PASSWORD_END                    (COL_NEW_PASSWORD_BEGIN + MAX_NUMBER_OF_SYMPOLS_IN_PASSWORD - 1)

#define  INDEX_LINE_NUMBER_1_FOR_LEVEL_PASSWORD 0
#define  INDEX_LINE_NUMBER_2_FOR_LEVEL_PASSWORD 1


enum _index_of_password
{
  INDEX_OF_PASSWORD1 = 0,
  INDEX_OF_PASSWORD2,
  INDEX_OF_PASSWORD3,
  
  MAX_ROW_FOR_CHOOSE_PASSWORDS
};

#endif
