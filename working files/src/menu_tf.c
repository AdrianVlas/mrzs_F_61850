#include "header.h"

/*****************************************************/
//Формуємо екран відображення типів джерел ранжування передавальних функцій
/*****************************************************/
void make_ekran_list_source_tf(void)
{
  const uint8_t name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_LIST_SOURCE_TF][MAX_COL_LCD] = 
  {
    {
      "      Вход      ",
      "     Выход      "
    },
    {
      "      Вхід      ",
      "     Вихід      "
    },
    {
      "     Input      ",
      "     Output     "
    },
    {
      "      Вход      ",
      "     Выход      "
    }
  };
  const uint8_t input_signals[MAX_NAMBER_LANGUAGE][1 + NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG + 3][MAX_COL_LCD] = 
  {
    {"     Пусто      ", NAME_RANG_RU},
    {"     Пусто      ", NAME_RANG_UA},
    {"     Empty      ", NAME_RANG_EN},
    {"     Пусто      ", NAME_RANG_KZ}
  };
  const uint8_t output_signals[MAX_NAMBER_LANGUAGE][1 + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - NUMBER_UP_SIGNAL_FOR_RANG_SMALL + 1][MAX_COL_LCD] = 
  {
    {"     Пусто      ", NAME_RANG_SMALL_RU},
    {"     Пусто      ", NAME_RANG_SMALL_UA},
    {"     Empty      ", NAME_RANG_SMALL_EN},
    {"     Пусто      ", NAME_RANG_SMALL_KZ}
  };
  const uint32_t index_number_UP[MAX_NAMBER_LANGUAGE][3] = 
  {
    {11, 10, 8}, 
    {11, 10, 8}, 
    {12,  7, 8}, 
    {11, 10, 8}, 
  };

  int index_language = index_language_in_array(current_settings.language);
  
  uint8_t input_signals_tmp[1 + NUMBER_TOTAL_SIGNAL_FOR_RANG][MAX_COL_LCD];
  for(int index_1 = 0; index_1 < (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG); index_1++)
  {
    unsigned int index_row;
    if (index_1 < (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG)) 
    {
      index_row = index_1;
    }
    else if (index_1 < (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG))
    {
      index_row = (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG) + ((index_1 - (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG)) % 3);
    }
    else
    {
      index_row = index_1 - NUMBER_UP_SIGNAL_FOR_RANG + 3;
    }
      
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
    {
      if (
          (index_1 >= (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG))  &&
          (index_1 <  (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG)) &&
          (index_2 == index_number_UP[index_language][(index_1 - (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG)) % 3]) 
         )
      {
        input_signals_tmp[index_1][index_2] = 0x30 + ((index_1 - (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG)) / 3 + 1);
      }
      else input_signals_tmp[index_1][index_2] = input_signals[index_language][index_row][index_2];
    }
  }

  uint8_t output_signals_tmp[1 + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL][MAX_COL_LCD];
  for(int index_1 = 0; index_1 < (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL); index_1++)
  {
    unsigned int index_row;
    if (index_1 < (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - NUMBER_EL_SIGNAL_FOR_RANG_SMALL - NUMBER_VMP_SIGNAL_FOR_RANG_SMALL - NUMBER_UP_SIGNAL_FOR_RANG_SMALL)) 
    {
      index_row = index_1;
    }
    else if (index_1 < (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - NUMBER_EL_SIGNAL_FOR_RANG_SMALL - NUMBER_VMP_SIGNAL_FOR_RANG_SMALL))
    {
      index_row = (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - NUMBER_EL_SIGNAL_FOR_RANG_SMALL - NUMBER_VMP_SIGNAL_FOR_RANG_SMALL - NUMBER_UP_SIGNAL_FOR_RANG_SMALL) + ((index_1 - (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - NUMBER_EL_SIGNAL_FOR_RANG_SMALL - NUMBER_VMP_SIGNAL_FOR_RANG_SMALL - NUMBER_UP_SIGNAL_FOR_RANG_SMALL)) % 1);
    }
    else
    {
      index_row = index_1 - NUMBER_UP_SIGNAL_FOR_RANG_SMALL + 1;
    }
      
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
    {
      if (
          (index_1 >= (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - NUMBER_EL_SIGNAL_FOR_RANG_SMALL - NUMBER_VMP_SIGNAL_FOR_RANG_SMALL - NUMBER_UP_SIGNAL_FOR_RANG_SMALL))  &&
          (index_1 <  (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - NUMBER_EL_SIGNAL_FOR_RANG_SMALL - NUMBER_VMP_SIGNAL_FOR_RANG_SMALL)) &&
          (index_2 == index_number_UP[index_language][(index_1 - (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - NUMBER_EL_SIGNAL_FOR_RANG_SMALL - NUMBER_VMP_SIGNAL_FOR_RANG_SMALL - NUMBER_UP_SIGNAL_FOR_RANG_SMALL)) % 1]) 
         )
      {
        output_signals_tmp[index_1][index_2] = 0x30 + ((index_1 - (1 + NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL - NUMBER_EL_SIGNAL_FOR_RANG_SMALL - NUMBER_VMP_SIGNAL_FOR_RANG_SMALL - NUMBER_UP_SIGNAL_FOR_RANG_SMALL)) / 1 + 1);
      }
      else output_signals_tmp[index_1][index_2] = output_signals[index_language][index_row][index_2];
    }
  }

  if ((current_settings.control_zz & CTR_ZZ1_TYPE) != 0)
  {
    const unsigned char name_block_zz[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
    {
      "    Блок.ЗЗ     ",
      "    Блок.ЗЗ     ",
      "    Блок.ЗЗ     ",
      "    Блок.ЗЗ     "
    };
    for (unsigned int index_1 = 0; index_1 < MAX_COL_LCD; index_1++)
    {
      input_signals_tmp[1 + RANG_BLOCK_NZZ][index_1] = name_block_zz[index_language][index_1];
      output_signals_tmp[1 + RANG_SMALL_BLOCK_NZZ][index_1] = name_block_zz[index_language][index_1];
    }
  }
  
  uint32_t position_temp = current_ekran.index_position;
  uint32_t index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  uint32_t value = ((current_ekran.edition == 0) ? current_settings : edition_settings).ranguvannja_tf[current_ekran.current_level - EKRAN_LIST_SOURCE_TF1];
  //Копіюємо  рядки у робочий екран
  for (size_t i =0; i < MAX_ROW_LCD; i++)
  {
    int32_t index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < MAX_ROW_LIST_SOURCE_TF)//Множення на два константи потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (size_t j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення
        for (size_t j = 0; j < MAX_COL_LCD; j++) working_ekran[i][j] = ((index_of_ekran_tmp == INDEX_ML_LIST_SOURCE_INPUT_TF) ? input_signals_tmp : output_signals_tmp)[(value >> (16*index_of_ekran_tmp)) & 0xffff][j];
        if (position_temp == index_of_ekran_tmp) 
        {
          current_ekran.position_cursor_x = 0;
          while (
                 (current_ekran.position_cursor_x < (MAX_COL_LCD - 1)) &&
                 (working_ekran[i][current_ekran.position_cursor_x + 1] == ' ')  
                )
          {
            current_ekran.position_cursor_x++;
          }
        }
      }
    }
    else
      for (size_t j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp << 1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 1;

  if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  else current_ekran.cursor_blinking_on = 1;

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
