#include "header.h"

/*****************************************************/
//Формуємо екран вибору ДВ/ДВих/Св для ранжування
/* 
-------------------------------------------------------
Вхідний параметр
0 - входи
1 - виходи
2 - світодіоди
3 - опреділювальні функції
4 - функціональні кнопки
5 - опреділювальні триґери
6 - визначувані "І"
7 - визначувані "АБО"
8 - визначувані "Викл.АБО"
9 - визначувані "НЕ"
-------------------------------------------------------
*/
/*****************************************************/
void make_ekran_chose_of_inputs_outputs_leds_df_buttons_for_ranguvannja(unsigned int type_of_window)
{
  const unsigned char information[MAX_NAMBER_LANGUAGE][10][MAX_COL_LCD] = 
  {
    {" Двх            ", " Двых           ", " Св             ", " О-функция      ", " F              ", " О-триггер      ", " О-И            ", " О-ИЛИ          ", " О-Искл.ИЛИ     ", " О-НЕ           "},
    {" Двх            ", " Двих           ", " Св             ", " В-функція      ", " F              ", " В-триґер       ", " В-І            ", " В-АБО          ", " В-Викл.АБО     ", " В-НЕ           "},
    {" DI             ", " DO             ", " LED            ", " UD Function    ", " F              ", " UD Flip-Flop   ", " UD AND         ", " UD OR          ", " UD XOR         ", " UD NOT         "},
    {" Двх            ", " Двых           ", " Св             ", " О-функция      ", " F              ", " О-триггер      ", " О-И            ", " О-ИЛИ          ", " О-Искл.ИЛИ     ", " О-НЕ           "}
  };

  const unsigned int first_index_number[MAX_NAMBER_LANGUAGE][10] = 
  {
    {4, 5, 3, 10, 2, 10, 4, 6, 11, 5},
    {4, 5, 3, 10, 2,  9, 4, 6, 11, 5},
    {3, 3, 4, 12, 2, 13, 7, 6,  7, 6},
    {4, 5, 3, 10, 2, 10, 4, 6, 11, 5}
  };
  
  const unsigned int max_row[10] =
  {
    MAX_ROW_LIST_INPUTS_FOR_RANGUVANNJA,
    MAX_ROW_LIST_OUTPUTS_FOR_RANGUVANNJA,
    MAX_ROW_LIST_LEDS_FOR_RANGUVANNJA,
    MAX_ROW_FOR_LIST_DF,
    MAX_ROW_LIST_BUTTONS_FOR_RANGUVANNJA,
    MAX_ROW_FOR_LIST_DT,
    MAX_ROW_FOR_LIST_D_AND,
    MAX_ROW_FOR_LIST_D_OR,
    MAX_ROW_FOR_LIST_D_XOR,
    MAX_ROW_FOR_LIST_D_NOT
  };
  int index_language = index_language_in_array(current_settings.language);
  unsigned int first_index_number_1 = first_index_number[index_language][type_of_window];
        
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  //Формуємо рядки  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int number = index_of_ekran + 1;
    unsigned int tmp_1 = (number / 10), tmp_2 = number - tmp_1*10;

    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < max_row[type_of_window])
    {
      for (unsigned int j = 0; j<MAX_COL_LCD; j++)
      {
        if ((j < first_index_number_1) || (j >= (first_index_number_1 + 2 + 3)))
           working_ekran[i][j] = information[index_language][type_of_window][j];
        else
        {
          if (j == first_index_number_1)
          {
            if (tmp_1 > 0 ) working_ekran[i][j] = tmp_1 + 0x30;
          }
          else if (j == (first_index_number_1 + 1))     
          {
            if (tmp_1 > 0 )
            {
              working_ekran[i][j] = tmp_2 + 0x30;
            }
            else
            {
              working_ekran[i][j - 1] = tmp_2 + 0x30;
              working_ekran[i][j] = '.';
            }
          }
          else
          {
            if (tmp_1 > 0 )
            {
              working_ekran[i][j] = '.';
            }
            else
            {
              if ( j < (first_index_number_1 + 2 + 3 - 1))
                working_ekran[i][j] = '.';
              else
                working_ekran[i][j] = ' ';
            }
          }
        }
      }
    }
    else
    {
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

    index_of_ekran++;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення зранжованих сигналів на функціональну кнопку
/*****************************************************/
void make_ekran_set_function_in_button(unsigned int number_ekran)
{
#define NUMBER_ROW_FOR_NOTHING_INFORMATION 2
  
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_RANGUVANNJA_BUTTON + NUMBER_ROW_FOR_NOTHING_INFORMATION][MAX_COL_LCD] = 
  {
    {
      "      Нет       ",
      "  ранжирования  ",
      " Сброс индикации",
      "   Сброс реле   ",
      " Местн./Дистанц.",
      "     Вкл.ВВ     ",
      "    Откл.ВВ     ",
      " С.блк.Гот.к ТУ ",
      " Вх.О-функции1  ",
      " Вх.О-функции2  ",
      " Вх.О-функции3  ",
      " Вх.О-функции4  ",
      " Вх.О-функции5  ",
      " Вх.О-функции6  ",
      " Вх.О-функции7  ",
      " Вх.О-функции8  ",
      " Уст.О-триггера1",
      " Сбр.О-триггера1",
      " Уст.О-триггера2",
      " Сбр.О-триггера2",
      " Уст.О-триггера3",
      " Сбр.О-триггера3",
      " Уст.О-триггера4",
      " Сбр.О-триггера4"
    },
    {
      "      Нема      ",
      "   ранжування   ",
      " Скид.індикації ",
      "   Скид.реле    ",
      " Місц./Дистанц. ",
      "    Ввімк.ВВ    ",
      "    Вимк.ВВ     ",
      " С.блк.Гот.до ТУ",
      " Вх.В-функції1  ",
      " Вх.В-функції2  ",
      " Вх.В-функції3  ",
      " Вх.В-функції4  ",
      " Вх.В-функції5  ",
      " Вх.В-функції6  ",
      " Вх.В-функції7  ",
      " Вх.В-функції8  ",
      " Вст.В-триґера1 ",
      " Ск.В-триґера1  ",
      " Вст.В-триґера2 ",
      " Ск.В-триґера2  ",
      " Вст.В-триґера3 ",
      " Ск.В-триґера3  ",
      " Вст.В-триґера4 ",
      " Ск.В-триґера4  "
    },
    {
      "       No       ",
      "    ranking     ",
      "Indication Reset",
      "  Relay Reset   ",
      "  Local/Remote  ",
      "     On CB      ",
      "     Off CB     ",
      " С.блк.Гот.к ТУ ",
      "    UDF1 In     ",
      "    UDF2 In     ",
      "    UDF3 In     ",
      "    UDF4 In     ",
      "    UDF5 In     ",
      "    UDF6 In     ",
      "    UDF7 In     ",
      "    UDF8 In     ",
      "UD Flip-Flop1 S.",
      "UD Flip-Flop1 R.",
      "UD Flip-Flop2 S.",
      "UD Flip-Flop2 R.",
      "UD Flip-Flop3 S.",
      "UD Flip-Flop3 R.",
      "UD Flip-Flop4 S.",
      "UD Flip-Flop4 R."
    },
    {
      "      Нет       ",
      "  ранжирования  ",
      " Сброс индикации",
      "   Сброс реле   ",
      " Местн./Дистанц.",
      "     Вкл.ВВ     ",
      "    Откл.ВВ     ",
      " С.блк.Гот.к ТУ ",
      " Вх.О-функции1  ",
      " Вх.О-функции2  ",
      " Вх.О-функции3  ",
      " Вх.О-функции4  ",
      " Вх.О-функции5  ",
      " Вх.О-функции6  ",
      " Вх.О-функции7  ",
      " Вх.О-функции8  ",
      " Уст.О-триггера1",
      " Сбр.О-триггера1",
      " Уст.О-триггера2",
      " Сбр.О-триггера2",
      " Уст.О-триггера3",
      " Сбр.О-триггера3",
      " Уст.О-триггера4",
      " Сбр.О-триггера4"
    }
  };
  unsigned char name_string_tmp[MAX_ROW_RANGUVANNJA_BUTTON + NUMBER_ROW_FOR_NOTHING_INFORMATION][MAX_COL_LCD];

  int index_language = index_language_in_array(current_settings.language);
  for(int index_1 = 0; index_1 < (MAX_ROW_RANGUVANNJA_BUTTON + NUMBER_ROW_FOR_NOTHING_INFORMATION); index_1++)
  {
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
  }
  
  if(current_ekran.edition == 0)
  {
    //Випадок, коли ми продивляємося зранжовані функції на кнопці
    unsigned int state_viewing_input = current_settings.ranguvannja_buttons[number_ekran - EKRAN_RANGUVANNJA_BUTTON_1];
    
    if (state_viewing_input == 0)
    {
      //Це означає, що нічого не відранжовано на кнопку
      
      //Текучу позицію в сипску переводимо на сам початок
      current_ekran.index_position = 0;
      position_in_current_level_menu[number_ekran] = 0;

      //Копіюємо  рядки у робочий екран
      for (unsigned int i=0; i< MAX_ROW_LCD; i++)
      {
        //Копіюємо в робочий екран інформацію, що нічого не відранжовано
        if (i < NUMBER_ROW_FOR_NOTHING_INFORMATION)
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[i][j];
        else
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
      }

      //Відображення курору по вертикалі
      current_ekran.position_cursor_y = 0;
      //Курсор невидимий
      current_ekran.cursor_on = 0;
    }
    else
    {
      /************************************************************/
      //Формуємо список із функцій, які реально відранжовані
      /************************************************************/
      unsigned int position_temp = current_ekran.index_position;
      unsigned int index_of_ekran;
      unsigned int i = 0, offset = 0;

      while ((i + offset) < MAX_ROW_RANGUVANNJA_BUTTON)
      {
        if ((state_viewing_input & (1<<(i + offset))) == 0)
        {
          for (unsigned int j = i; j < (MAX_ROW_RANGUVANNJA_BUTTON - offset); j++)
          {
            if ((j + 1) < (MAX_ROW_RANGUVANNJA_BUTTON - offset))
            {
              for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
            }
            else 
            {
              for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
            }
          }
          if (current_ekran.index_position >= ((int)(i + offset))) position_temp--;
          offset++;
        }
        else i++;
      }
      /************************************************************/

    
      index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
      
      //Копіюємо  рядки у робочий екран
      for (i=0; i< MAX_ROW_LCD; i++)
      {
        //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
        if (index_of_ekran < MAX_ROW_RANGUVANNJA_BUTTON)
        {
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran + NUMBER_ROW_FOR_NOTHING_INFORMATION][j];

          //Підтягуємо назву, щоб не було спереді багато пробілів
          unsigned int iteration = 0;
          while (
                 (working_ekran[i][0] == ' ') &&
                 (working_ekran[i][1] == ' ') &&
                 (iteration < (MAX_COL_LCD - 1 - 1))
                )
          {
            for (unsigned int j = 1; j < MAX_COL_LCD; j++)
            {
              if ((j + 1) < MAX_COL_LCD)
                working_ekran[i][j] = working_ekran[i][j + 1];
              else
                working_ekran[i][j] = ' ';
            }
            iteration++;
          }
        }
        else
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

        index_of_ekran++;
      }
      //Відображення курору по вертикалі
      current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
      //Курсор видимий
      current_ekran.cursor_on = 1;
    }
    
    //Курсор по горизонталі відображається на першій позиції
    current_ekran.position_cursor_x = 0;
    //Курсор не мигає
    current_ekran.cursor_blinking_on = 0;
  }
  else
  {
    unsigned int temp_data = edition_settings.ranguvannja_buttons[current_ekran.current_level - EKRAN_RANGUVANNJA_BUTTON_1];
    unsigned int position_temp = current_ekran.index_position;
    unsigned int index_of_ekran;
    unsigned int i, offset = 0;
    int min_max_number[TOTAL_NUMBER_PROTECTION][2] ={
                                                     {-1,-1},
                                                     {-1,-1},
                                                     {-1,-1},
                                                     {-1,-1},
                                                     {-1,-1},
                                                     {-1,-1},
                                                     {-1,-1},
                                                     {-1,-1},
                                                     {-1,-1},
                                                     {-1,-1},
                                                     {-1,-1},
                                                     {-1,-1},
                                                     {
                                                      (NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON + NUMBER_MTZ_SIGNAL_FOR_RANG_BUTTON + NUMBER_MTZ04_SIGNAL_FOR_RANG_BUTTON + NUMBER_ZDZ_SIGNAL_FOR_RANG_BUTTON + NUMBER_ZZ_SIGNAL_FOR_RANG_BUTTON + NUMBER_TZNP_SIGNAL_FOR_RANG_BUTTON + NUMBER_APV_SIGNAL_FOR_RANG_BUTTON + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_BUTTON + NUMBER_UROV_SIGNAL_FOR_RANG_BUTTON + NUMBER_ZOP_SIGNAL_FOR_RANG_BUTTON + NUMBER_UMIN_SIGNAL_FOR_RANG_BUTTON + NUMBER_UMAX_SIGNAL_FOR_RANG_BUTTON + NUMBER_VMP_SIGNAL_FOR_RANG_BUTTON),
                                                      (NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON + NUMBER_MTZ_SIGNAL_FOR_RANG_BUTTON + NUMBER_MTZ04_SIGNAL_FOR_RANG_BUTTON + NUMBER_ZDZ_SIGNAL_FOR_RANG_BUTTON + NUMBER_ZZ_SIGNAL_FOR_RANG_BUTTON + NUMBER_TZNP_SIGNAL_FOR_RANG_BUTTON + NUMBER_APV_SIGNAL_FOR_RANG_BUTTON + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_BUTTON + NUMBER_UROV_SIGNAL_FOR_RANG_BUTTON + NUMBER_ZOP_SIGNAL_FOR_RANG_BUTTON + NUMBER_UMIN_SIGNAL_FOR_RANG_BUTTON + NUMBER_UMAX_SIGNAL_FOR_RANG_BUTTON + NUMBER_VMP_SIGNAL_FOR_RANG_BUTTON + NUMBER_EL_SIGNAL_FOR_RANG_BUTTON - 1)
                                                     }
                                                    };
      
    /*************************************************************/
    //Фільтруємо сигнали, яких у даній конфігурації неприсутні
    /*************************************************************/
    //Функції загального призначення пропускаємо (вони знаходяться у початку списку), тому починаємо з першого записту
    int index_in_list = NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON;
    
    for (i = 0; i < TOTAL_NUMBER_PROTECTION; i++)
    {
      
      if((current_settings.configuration & (1 << i)) != 0)
      {
        /*
        Захист присутнійсть, тому функції фільтрувати не потрібно - переводимо індекс на наступні функції
        Додаємо кількість функцій до поточного індексу, якщо для поточного захисту реально були присутні функції,
        бо інкаше ми вже знаходимося на індексі наступного захисту
        */
        if(min_max_number[i][0] >=0)
        {
//          if (i == EL_BIT_CONFIGURATION)
//          {
//            /*
//            Випадок коли деякі сигнали розширеної логіки треба відфільтрувати
//            */
//
//            //Відкидати імена функцій і зміщати біти треба тільки у тому випадку, якщо функції пристні у списку для ранжування для даного захисту
//            //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
//            unsigned int maska = 0;
//            unsigned int j1 = 0;
//            for (j1 = 0; j1 < (min_max_number[i][0] - offset); j1++) maska |= (1 << j1);
//          
//            //Відкидаємо назви функцій із списку, які є зайвими
//            while(index_in_list <= min_max_number[i][1])
//            {
//              if (
//                  (
//                   (index_in_list >= (int)(RANG_BUTTON_DF1_IN + current_settings.number_defined_df)) &&
//                   (index_in_list <= RANG_BUTTON_DF8_IN)
//                  )
//                  ||  
//                  (
//                   (index_in_list >= (int)(RANG_BUTTON_DT1_SET + 2*current_settings.number_defined_dt)) &&
//                   (index_in_list <= RANG_BUTTON_DT4_RESET)
//                  )
//                 )
//              {
//                /***/
//                //Зміщуємо біти стану реанжування функцій разом із їх назвами
//                /***/
//                unsigned int new_temp_data_1, new_temp_data_2;
//                new_temp_data_1 = temp_data & maska;
//                new_temp_data_2 = temp_data & (~maska);
//                new_temp_data_2 = new_temp_data_2 >>1;
//                new_temp_data_2 &= (~maska);
//                temp_data = new_temp_data_1 | new_temp_data_2;
//                
//                /***/
//                for (unsigned int j = (index_in_list - offset); j < (MAX_ROW_RANGUVANNJA_BUTTON - offset); j++)
//                {
//                  if ((j + 1) < (MAX_ROW_RANGUVANNJA_BUTTON - offset))
//                  {
//                    for (unsigned int k = 0; k < MAX_COL_LCD; k++)
//                      name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
//                  }
//                  else 
//                  {
//                    for (unsigned int k = 0; k<MAX_COL_LCD; k++)
//                      name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
//                  }
//                }
//                if (current_ekran.index_position >= index_in_list) position_temp--;
//                offset++;
//              }
//              else
//              {
//                maska |= (1 << j1);
//                j1++;
//              }
//                
//              index_in_list++;
//            }
//          }
//          else
            index_in_list += ((min_max_number[i][1] - min_max_number[i][0]) + 1);
        }
      }
      else if (min_max_number[i][0] >=0)
      {
        //Відкидати імена функцій і зміщати біти треба тільки у тому випадку, якщо функції пристні у списку для ранжування для даного захисту
        //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
        unsigned int maska = 0;
        for (unsigned int j = 0; j < (min_max_number[i][0] - offset); j++) maska = (maska << 1) + 0x1;
          
        //Відкидаємо назви функцій із списку, які є зайвими
        while(index_in_list <= min_max_number[i][1])
        {
          unsigned int new_temp_data_1, new_temp_data_2;
          //Зміщуємо біти стану реанжування функцій разом із їх назвами
          new_temp_data_1 = temp_data & maska;
          new_temp_data_2 = temp_data & (~maska);
          new_temp_data_2 = new_temp_data_2 >>1;
          new_temp_data_2 &= (~maska);
          temp_data = new_temp_data_1 | new_temp_data_2;

          for (unsigned int j = (index_in_list - offset); j < (MAX_ROW_RANGUVANNJA_BUTTON - offset); j++)
          {
            if ((j + 1) < (MAX_ROW_RANGUVANNJA_BUTTON - offset))
            {
              for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
            }
            else 
            {
              for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
            }
          }
          if (current_ekran.index_position >= index_in_list) position_temp--;
          
          offset++;
          index_in_list++;
        }
      }
    }
    /*************************************************************/

    //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

    for (i=0; i< MAX_ROW_LCD; i++)
    {
     if (index_of_ekran < (MAX_ROW_RANGUVANNJA_BUTTON<<1))//Множення на два константи  MAX_ROW_RANGUVANNJA_BUTTON потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
     {
       if ((i & 0x1) == 0)
       {
         //У непарному номері рядку виводимо заголовок
         for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[(index_of_ekran>>1) + NUMBER_ROW_FOR_NOTHING_INFORMATION][j];
       }
       else
       {
         //У парному номері рядку виводимо значення
         const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
         {
           {"      ОТКЛ      ", "      ВКЛ       "},
           {"      ВИМК      ", "     ВВІМК      "},
           {"      OFF       ", "       ON       "},
           {"      СЉНД      ", "      КОСУ      "}
        };
        unsigned int maska = 1 << (index_of_ekran >> 1);
          
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][((temp_data & maska) != 0)][j];
       }
     }
     else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

     index_of_ekran++;
    }

    const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][2] = 
    {
      {5, 5},
      {5, 4},
      {5, 6},
      {5, 5}
    };

    //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
    current_ekran.position_cursor_x =  cursor_x[index_language][((temp_data & (1 << position_temp)) != 0)];
    current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
    
    //Курсор мигає
    current_ekran.cursor_blinking_on = 1;
    //Режим відображення у режимі редагування
  }
  

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;

#undef NUMBER_ROW_FOR_NOTHING_INFORMATION
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення зранжованих сигналів на вибраний вхід
/*****************************************************/
void make_ekran_set_function_in_input(unsigned int number_ekran)
{
#define NUMBER_ROW_FOR_NOTHING_INFORMATION 2
  
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_RANGUVANNJA_INPUT + NUMBER_ROW_FOR_NOTHING_INFORMATION][MAX_COL_LCD] = 
  {
    {
      "      Нет       ",
      "  ранжирования  ",
      "  Блок.вкл.ВВ   ",
      " Сброс индикации",
      "   Сброс реле   ",
      " Местн./Дистанц.",
      "  Положение ВВ  ",
      "Откл.от вн.защит",
      "     Вкл.ВВ     ",
      " Контроль Вкл.  ",
      "    Откл.ВВ     ",
      " Контроль Откл. ",
      " 1-я гр.уставок ",
      " 2-я гр.уставок ",
      " 3-я гр.уставок ",
      " 4-я гр.уставок ",
      " С.блк.Гот.к ТУ ",
      "   Блок.МТЗ1    ",
      "   Блок.МТЗ2    ",
      " Блок.уск.МТЗ2  ",
      "   Блок.МТЗ3    ",
      "   Блок.МТЗ4    ",
      "Блок.МТЗ 0.4кВ 1",
      "Блок.МТЗ 0.4кВ 2",
      "Бл.у.МТЗ 0.4кВ 2",
      " Пуск ЗДЗ от ДВ ",
      "    Блок.НЗЗ    ",
      "   Блок.ТЗНП1   ",
      "   Блок.ТЗНП2   ",
      "   Блок.ТЗНП3   ",
      " Стат.блок.АПВ  ",
      " АЧР/ЧАПВ от ДВ ",
      "   Блок.АЧР1    ",
      "   Блок.АЧР2    ",
      " Пуск УРОВ от ДВ",
      " Блок.ЗОП(КОФ)  ",
      "  Блок.ЗНмин1   ",
      "  Пуск ЗНмин1   ",
      "  Блок.ЗНмин2   ",
      "  Пуск ЗНмин2   ",
      "  Блок.ЗНмакс1  ",
      "  Блок.ЗНмакс2  ",
      " Вх.О-функции1  ",
      " Вх.О-функции2  ",
      " Вх.О-функции3  ",
      " Вх.О-функции4  ",
      " Вх.О-функции5  ",
      " Вх.О-функции6  ",
      " Вх.О-функции7  ",
      " Вх.О-функции8  ",
      " Уст.О-триггера1",
      " Сбр.О-триггера1",
      " Уст.О-триггера2",
      " Сбр.О-триггера2",
      " Уст.О-триггера3",
      " Сбр.О-триггера3",
      " Уст.О-триггера4",
      " Сбр.О-триггера4"
    },
    {
      "      Нема      ",
      "   ранжування   ",
      " Блок.ввімкн.ВВ ",
      " Скид.індикації ",
      "   Скид.реле    ",
      " Місц./Дистанц. ",
      "    Стан ВВ     ",
      "Вимк.від зовн.з.",
      "    Ввімк.ВВ    ",
      " Контроль Ввімк.",
      "    Вимк.ВВ     ",
      " Контроль Вимк. ",
      " 1-а гр.уставок ",
      " 2-а гр.уставок ",
      " 3-а гр.уставок ",
      " 4-а гр.уставок ",
      " С.блк.Гот.до ТУ",
      "   Блок.МСЗ1    ",
      "   Блок.МСЗ2    ",
      " Блок.приск.МСЗ2",
      "   Блок.МСЗ3    ",
      "   Блок.МСЗ4    ",
      "Блок.МСЗ 0.4кВ 1",
      "Блок.МСЗ 0.4кВ 2",
      "Бл.п.МСЗ 0.4кВ 2",
      " Пуск ЗДЗ від ДВ",
      "    Блок.НЗЗ    ",
      "   Блок.СЗНП1   ",
      "   Блок.СЗНП2   ",
      "   Блок.СЗНП3   ",
      " Стат.блок.АПВ  ",
      " АЧР/ЧАПВ від ДВ",
      "   Блок.АЧР1    ",
      "   Блок.АЧР2    ",
      "Пуск ПРВВ від ДВ",
      " Блок.ЗЗП(КОФ)  ",
      "  Блок.ЗНмин1   ",
      "  Пуск ЗНмин1   ",
      "  Блок.ЗНмин2   ",
      "  Пуск ЗНмин2   ",
      "  Блок.ЗНмакс1  ",
      "  Блок.ЗНмакс2  ",
      " Вх.В-функції1  ",
      " Вх.В-функції2  ",
      " Вх.В-функції3  ",
      " Вх.В-функції4  ",
      " Вх.В-функції5  ",
      " Вх.В-функції6  ",
      " Вх.В-функції7  ",
      " Вх.В-функції8  ",
      " Вст.В-триґера1 ",
      " Ск.В-триґера1  ",
      " Вст.В-триґера2 ",
      " Ск.В-триґера2  ",
      " Вст.В-триґера3 ",
      " Ск.В-триґера3  ",
      " Вст.В-триґера4 ",
      " Ск.В-триґера4  "
    },
    {
      "       No       ",
      "    ranking     ",
      " ON CB Blocking ",
      "Indication Reset",
      "  Relay Reset   ",
      "  Local/Remote  ",
      "  State of CB   ",
      "Откл.от вн.защит",
      "     On CB      ",
      " On CB Control  ",
      "     Off CB     ",
      " Off CB Control ",
      " Pick-up Set 1  ",
      " Pick-up Set 2  ",
      " Pick-up Set 3  ",
      " Pick-up Set 4  ",
      " С.блк.Гот.к ТУ ",
      "  Blc.of OCP1   ",
      "  Blc.of OCP2   ",
      "  OCP2 Acc.Blc. ",
      "  Blc.of OCP3   ",
      "  Blc.of OCP4   ",
      "OCP 0.4kV 1 Blc.",
      "OCP 0.4kV 2 Blc.",
      "OCP 0.4kV 2 Ac.B",
      " Пуск ЗДЗ от ДВ ",
      "    Блок.НЗЗ    ",
      "   Блок.ТЗНП1   ",
      "   Блок.ТЗНП2   ",
      "   Блок.ТЗНП3   ",
      " Стат.блок.АПВ  ",
      " АЧР/ЧАПВ от ДВ ",
      "   Блок.АЧР1    ",
      "   Блок.АЧР2    ",
      " CBFP Start f.DI",
      "  Blc.of NPSP   ",
      "   Блок.Umin1   ",
      "   Пуск Umin1   ",
      "   Блок.Umin2   ",
      "   Пуск Umin2   ",
      "   Блок.Umax1   ",
      "   Блок.Umax2   ",
      "    UDF1 In     ",
      "    UDF2 In     ",
      "    UDF3 In     ",
      "    UDF4 In     ",
      "    UDF5 In     ",
      "    UDF6 In     ",
      "    UDF7 In     ",
      "    UDF8 In     ",
      "UD Flip-Flop1 S.",
      "UD Flip-Flop1 R.",
      "UD Flip-Flop2 S.",
      "UD Flip-Flop2 R.",
      "UD Flip-Flop3 S.",
      "UD Flip-Flop3 R.",
      "UD Flip-Flop4 S.",
      "UD Flip-Flop4 R."
    },
    {
      "      Нет       ",
      "  ранжирования  ",
      "  Блок.вкл.ВВ   ",
      " Сброс индикации",
      "   Сброс реле   ",
      " Местн./Дистанц.",
      "  Положение ВВ  ",
      "Откл.от вн.защит",
      "     Вкл.ВВ     ",
      " Контроль Вкл.  ",
      "    Откл.ВВ     ",
      " Контроль Откл. ",
      " 1-я гр.уставок ",
      " 2-я гр.уставок ",
      " 3-я гр.уставок ",
      " 4-я гр.уставок ",
      " С.блк.Гот.к ТУ ",
      "   Блок.МТЗ1    ",
      "   Блок.МТЗ2    ",
      " Блок.уск.МТЗ2  ",
      "   Блок.МТЗ3    ",
      "   Блок.МТЗ4    ",
      "Блок.МТЗ 0.4кВ 1",
      "Блок.МТЗ 0.4кВ 2",
      "Бл.у.МТЗ 0.4кВ 2",
      " Пуск ЗДЗ от ДВ ",
      "    Блок.НЗЗ    ",
      "   Блок.ТЗНП1   ",
      "   Блок.ТЗНП2   ",
      "   Блок.ТЗНП3   ",
      " Стат.блок.АПВ  ",
      " АЧР/ЧАПВ от ДВ ",
      "   Блок.АЧР1    ",
      "   Блок.АЧР2    ",
      " Пуск УРОВ от ДВ",
      " Блок.ЗОП(КОФ)  ",
      "  Блок.ЗНмин1   ",
      "  Пуск ЗНмин1   ",
      "  Блок.ЗНмин2   ",
      "  Пуск ЗНмин2   ",
      "  Блок.ЗНмакс1  ",
      "  Блок.ЗНмакс2  ",
      " Вх.О-функции1  ",
      " Вх.О-функции2  ",
      " Вх.О-функции3  ",
      " Вх.О-функции4  ",
      " Вх.О-функции5  ",
      " Вх.О-функции6  ",
      " Вх.О-функции7  ",
      " Вх.О-функции8  ",
      " Уст.О-триггера1",
      " Сбр.О-триггера1",
      " Уст.О-триггера2",
      " Сбр.О-триггера2",
      " Уст.О-триггера3",
      " Сбр.О-триггера3",
      " Уст.О-триггера4",
      " Сбр.О-триггера4"
    }
  };
  unsigned char name_string_tmp[MAX_ROW_RANGUVANNJA_INPUT + NUMBER_ROW_FOR_NOTHING_INFORMATION][MAX_COL_LCD];

  int index_language = index_language_in_array(current_settings.language);
  for(int index_1 = 0; index_1 < (MAX_ROW_RANGUVANNJA_INPUT + NUMBER_ROW_FOR_NOTHING_INFORMATION); index_1++)
  {
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
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
      name_string_tmp[RANG_INPUT_BLOCK_NZZ + NUMBER_ROW_FOR_NOTHING_INFORMATION][index_1] = name_block_zz[index_language][index_1];
    }
  }
  
  if(current_ekran.edition == 0)
  {
    //Випадок, коли ми продивляємося зранжовані функції на вході
    unsigned int state_viewing_input[N_SMALL];
    
    state_viewing_input[0] = current_settings.ranguvannja_inputs[N_SMALL*(number_ekran - EKRAN_RANGUVANNJA_INPUT_1)    ];
    state_viewing_input[1] = current_settings.ranguvannja_inputs[N_SMALL*(number_ekran - EKRAN_RANGUVANNJA_INPUT_1) + 1];
    
    if (
        (state_viewing_input[0] == 0) &&
        (state_viewing_input[1] == 0)
       )
    {
      //Це означає, що на даний вхід нічого не відранжовано
      
      //Текучу позицію в сипску переводимо на сам початок
      current_ekran.index_position = 0;
      position_in_current_level_menu[number_ekran] = 0;

      //Копіюємо  рядки у робочий екран
      for (unsigned int i=0; i< MAX_ROW_LCD; i++)
      {
        //Копіюємо в робочий екран інформацію, що нічого не відранжовано
        if (i < NUMBER_ROW_FOR_NOTHING_INFORMATION)
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[i][j];
        else
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
      }

      //Відображення курору по вертикалі
      current_ekran.position_cursor_y = 0;
      //Курсор невидимий
      current_ekran.cursor_on = 0;
    }
    else
    {
      /************************************************************/
      //Формуємо список із функцій, які реально відранжовані
      /************************************************************/
      unsigned int position_temp = current_ekran.index_position;
      unsigned int index_of_ekran;
      unsigned int i = 0, offset = 0;
      unsigned int state_current_bit;

      while ((i + offset) < MAX_ROW_RANGUVANNJA_INPUT)
      {
        state_current_bit = state_viewing_input[(i + offset)>>5] & (1<<((i + offset) & 0x1f));

        if (state_current_bit == 0)
        {
          for (unsigned int j = i; j < (MAX_ROW_RANGUVANNJA_INPUT - offset); j++)
          {
            if ((j + 1) < (MAX_ROW_RANGUVANNJA_INPUT - offset))
            {
              for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
            }
            else 
            {
              for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
            }
          }
          if (current_ekran.index_position >= ((int)(i + offset))) position_temp--;
          offset++;
        }
        else i++;
      }
      /************************************************************/

      index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
      
      //Копіюємо  рядки у робочий екран
      for (i=0; i< MAX_ROW_LCD; i++)
      {
        //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
        if (index_of_ekran < MAX_ROW_RANGUVANNJA_INPUT)
        {
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran + NUMBER_ROW_FOR_NOTHING_INFORMATION][j];

          //Підтягуємо назву, щоб не було спереді багато пробілів
          unsigned int iteration = 0;
          while (
                 (working_ekran[i][0] == ' ') &&
                 (working_ekran[i][1] == ' ') &&
                 (iteration < (MAX_COL_LCD - 1 - 1))
                )
          {
            for (unsigned int j = 1; j < MAX_COL_LCD; j++)
            {
              if ((j + 1) < MAX_COL_LCD)
                working_ekran[i][j] = working_ekran[i][j + 1];
              else
                working_ekran[i][j] = ' ';
            }
            iteration++;
          }
        }
        else
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

        index_of_ekran++;
      }
      //Відображення курору по вертикалі
      current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
      //Курсор видимий
      current_ekran.cursor_on = 1;
    }
    
    //Курсор по горизонталі відображається на першій позиції
    current_ekran.position_cursor_x = 0;
    //Курсор не мигає
    current_ekran.cursor_blinking_on = 0;
  }
  else
  {
    unsigned int temp_data[N_SMALL];
    temp_data[0] = edition_settings.ranguvannja_inputs[N_SMALL*(current_ekran.current_level - EKRAN_RANGUVANNJA_INPUT_1)    ];
    temp_data[1] = edition_settings.ranguvannja_inputs[N_SMALL*(current_ekran.current_level - EKRAN_RANGUVANNJA_INPUT_1) + 1];
    
    unsigned int position_temp = current_ekran.index_position;
    unsigned int index_of_ekran;
    unsigned int i, offset = 0;
    int min_max_number[TOTAL_NUMBER_PROTECTION][2] =
    {
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT + NUMBER_APV_SIGNAL_FOR_RANG_INPUT - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT + NUMBER_APV_SIGNAL_FOR_RANG_INPUT),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT + NUMBER_APV_SIGNAL_FOR_RANG_INPUT + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_INPUT - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT + NUMBER_APV_SIGNAL_FOR_RANG_INPUT + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_INPUT),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT + NUMBER_APV_SIGNAL_FOR_RANG_INPUT + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_INPUT + NUMBER_UROV_SIGNAL_FOR_RANG_INPUT - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT + NUMBER_APV_SIGNAL_FOR_RANG_INPUT + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_INPUT + NUMBER_UROV_SIGNAL_FOR_RANG_INPUT),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT + NUMBER_APV_SIGNAL_FOR_RANG_INPUT + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_INPUT + NUMBER_UROV_SIGNAL_FOR_RANG_INPUT + NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT + NUMBER_APV_SIGNAL_FOR_RANG_INPUT + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_INPUT + NUMBER_UROV_SIGNAL_FOR_RANG_INPUT + NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT + NUMBER_APV_SIGNAL_FOR_RANG_INPUT + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_INPUT + NUMBER_UROV_SIGNAL_FOR_RANG_INPUT + NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT + NUMBER_UMIN_SIGNAL_FOR_RANG_INPUT - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT + NUMBER_APV_SIGNAL_FOR_RANG_INPUT + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_INPUT + NUMBER_UROV_SIGNAL_FOR_RANG_INPUT + NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT + NUMBER_UMIN_SIGNAL_FOR_RANG_INPUT),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT + NUMBER_APV_SIGNAL_FOR_RANG_INPUT + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_INPUT + NUMBER_UROV_SIGNAL_FOR_RANG_INPUT + NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT + NUMBER_UMIN_SIGNAL_FOR_RANG_INPUT + NUMBER_UMAX_SIGNAL_FOR_RANG_INPUT - 1)
      },
      {-1,-1},
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT + NUMBER_APV_SIGNAL_FOR_RANG_INPUT + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_INPUT + NUMBER_UROV_SIGNAL_FOR_RANG_INPUT + NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT + NUMBER_UMIN_SIGNAL_FOR_RANG_INPUT + NUMBER_UMAX_SIGNAL_FOR_RANG_INPUT + NUMBER_VMP_SIGNAL_FOR_RANG_INPUT),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT + NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT + NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT + NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT + NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT + NUMBER_APV_SIGNAL_FOR_RANG_INPUT + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_INPUT + NUMBER_UROV_SIGNAL_FOR_RANG_INPUT + NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT + NUMBER_UMIN_SIGNAL_FOR_RANG_INPUT + NUMBER_UMAX_SIGNAL_FOR_RANG_INPUT + NUMBER_VMP_SIGNAL_FOR_RANG_INPUT + NUMBER_EL_SIGNAL_FOR_RANG_INPUT - 1)
      }
     };
    
    /*************************************************************/
    //Фільтруємо сигнали, яких у даній конфігурації неприсутні
    /*************************************************************/
    //Функції загального призначення пропускаємо (вони знаходяться у початку списку), тому починаємо з першого записту
    int index_in_list = NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT;
    
    for (i = 0; i < TOTAL_NUMBER_PROTECTION; i++)
    {
      
      if((current_settings.configuration & (1 << i)) != 0)
      {
        //Захист присутнійсть, тому функції фільтрувати не потрібно - переводимо індекс на наступні функції
        //Додаємо кількість функцій до поточного індексу, якщо для поточного захисту реально були присутні функції,
        //бо інкаше ми вже знаходимося на індексі наступного захисту
        if(min_max_number[i][0] >=0)
        {
//          if (i == EL_BIT_CONFIGURATION)
//          {
//            /*
//            Випадок коли деякі сигнали розширеної логіки треба відфільтрувати
//            */
//
//            //Відкидати імена функцій і зміщати біти треба тільки у тому випадку, якщо функції пристні у списку для ранжування для даного захисту
//            //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
//            unsigned int maska[N_SMALL] = {0, 0};
//            unsigned int j1;
//            for (j1 = 0; j1 < (min_max_number[i][0] - offset); j1++) _SET_BIT(maska, j1);
//          
//            //Відкидаємо назви функцій із списку, які є зайвими
//            while(index_in_list <= min_max_number[i][1])
//            {
//              if (
//                  (
//                   (index_in_list >= (int)(RANG_INPUT_DF1_IN + current_settings.number_defined_df)) &&
//                   (index_in_list <= RANG_INPUT_DF8_IN)
//                  )   
//                  ||  
//                  (
//                   (index_in_list >= (int)(RANG_INPUT_DT1_SET + 2*current_settings.number_defined_dt)) &&
//                   (index_in_list <= RANG_INPUT_DT4_RESET)
//                  )   
//                 )
//              {
//                /***/
//                //Зміщуємо біти стану реанжування функцій разом із їх назвами
//                /***/
//                unsigned int new_temp_data_1[N_SMALL], new_temp_data_2[N_SMALL];
//
//                for (unsigned int k = 0; k < N_SMALL; k++)
//                {
//                  new_temp_data_1[k] = temp_data[k] & maska[k];
//
//                  new_temp_data_2[k] = temp_data[k] & (~maska[k]);
//                }
//
//                for (unsigned int k = 0; k < (N_SMALL - 1); k++)
//                {
//                  new_temp_data_2[k] = ( (new_temp_data_2[k] >> 1) | ((new_temp_data_2[k + 1] & 0x1) << 31) ) & (~maska[k]);
//                }
//                new_temp_data_2[N_SMALL - 1] =  (new_temp_data_2[N_SMALL - 1] >> 1) & (~maska[N_SMALL - 1]);
//                
//                for (unsigned int k = 0; k < N_SMALL; k++)
//                {
//                  temp_data[k] = new_temp_data_1[k] | new_temp_data_2[k];
//                }
//                /***/
//                for (unsigned int j = (index_in_list - offset); j < (MAX_ROW_RANGUVANNJA_INPUT - offset); j++)
//                {
//                  if ((j + 1) < (MAX_ROW_RANGUVANNJA_INPUT - offset))
//                  {
//                    for (unsigned int k = 0; k < MAX_COL_LCD; k++)
//                      name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
//                  }
//                  else 
//                  {
//                    for (unsigned int k = 0; k<MAX_COL_LCD; k++)
//                      name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
//                  }
//                }
//                if (current_ekran.index_position >= index_in_list) position_temp--;
//          
//                offset++;
//              }
//              else
//              {
//                _SET_BIT(maska, j1);
//                j1++;
//              }
//                
//              index_in_list++;
//            }
//          }
//          else
            index_in_list += ((min_max_number[i][1] - min_max_number[i][0]) + 1);
        }
      }
      else if (min_max_number[i][0] >=0)
      {
        //Відкидати імена функцій і зміщати біти треба тільки у тому випадку, якщо функції пристні у списку для ранжування для даного захисту
        //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
        unsigned int maska[N_SMALL] = {0, 0};
        for (unsigned int j = 0; j < (min_max_number[i][0] - offset); j++) _SET_BIT(maska, j);
          
        //Відкидаємо назви функцій із списку, які є зайвими
        while(index_in_list <= min_max_number[i][1])
        {
          /***/
          //Зміщуємо біти стану реанжування функцій разом із їх назвами
          /***/
          unsigned int new_temp_data_1[N_SMALL], new_temp_data_2[N_SMALL];

          for (unsigned int k = 0; k < N_SMALL; k++)
          {
            new_temp_data_1[k] = temp_data[k] & maska[k];

            new_temp_data_2[k] = temp_data[k] & (~maska[k]);
          }

          for (unsigned int k = 0; k < (N_SMALL - 1); k++)
          {
            new_temp_data_2[k] = ( (new_temp_data_2[k] >> 1) | ((new_temp_data_2[k + 1] & 0x1) << 31) ) & (~maska[k]);
          }
          new_temp_data_2[N_SMALL - 1] =  (new_temp_data_2[N_SMALL - 1] >> 1) & (~maska[N_SMALL - 1]);
          
          for (unsigned int k = 0; k < N_SMALL; k++)
          {
            temp_data[k] = new_temp_data_1[k] | new_temp_data_2[k];
           }
          /***/
          for (unsigned int j = (index_in_list - offset); j < (MAX_ROW_RANGUVANNJA_INPUT - offset); j++)
          {
            if ((j + 1) < (MAX_ROW_RANGUVANNJA_INPUT - offset))
            {
              for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
            }
            else 
            {
              for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
            }
          }
          if (current_ekran.index_position >= index_in_list) position_temp--;
          
          offset++;
          index_in_list++;
        }
      }
    }
    /*************************************************************/
      
    //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

    for (i=0; i< MAX_ROW_LCD; i++)
    {
     if (index_of_ekran < (MAX_ROW_RANGUVANNJA_INPUT<<1))//Множення на два константи  MAX_ROW_RANGUVANNJA_INPUT потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
     {
       if ((i & 0x1) == 0)
       {
         //У непарному номері рядку виводимо заголовок
         for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[(index_of_ekran>>1) + NUMBER_ROW_FOR_NOTHING_INFORMATION][j];
       }
       else
       {
         //У парному номері рядку виводимо стан функції
         const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
         {
           {"      ОТКЛ      ", "      ВКЛ       "},
           {"      ВИМК      ", "     ВВІМК      "},
           {"      OFF       ", "       ON       "},
           {"      СЉНД      ", "      КОСУ      "}
        };
        unsigned int index_bit = index_of_ekran >> 1;
          
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][((temp_data[index_bit >> 5] & ( 1<< (index_bit & 0x1f))) != 0)][j];
       }
     }
     else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

     index_of_ekran++;
    }
    
    const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][2] = 
    {
      {5, 5},
      {5, 4},
      {5, 6},
      {5, 5}
    };

    //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням
    current_ekran.position_cursor_x =  cursor_x[index_language][((temp_data[position_temp >> 5] & (1 << (position_temp & 0x1f))) != 0)];
    current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
    
    //Курсор мигає
    current_ekran.cursor_blinking_on = 1;
    //Режим відображення у режимі редагування
  }

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;

#undef NUMBER_ROW_FOR_NOTHING_INFORMATION
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення зранжованих сигналів на вибраний вихід-світлоіндикаторів-о-функцій-реєстраторів
/*****************************************************/
void make_ekran_set_function_in_output_led_df_dt_reg(unsigned int number_ekran, unsigned int type_ekran)
{
#define NUMBER_ROW_FOR_NOTHING_INFORMATION 2
  
  unsigned int state_viewing_input[N_BIG];
  unsigned int max_row_ranguvannja;
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_RANGUVANNJA_OUTPUT + NUMBER_ROW_FOR_NOTHING_INFORMATION][MAX_COL_LCD] = 
  {
    {
      "      Нет       ",
      "  ранжирования  ",
      "  Блок.вкл.ВВ   ",
      " Сброс индикации",
      "   Сброс реле   ",
      " Местн./Дистанц.",
      "  Положение ВВ  ",
      "Откл.от вн.защит",
      "     Вкл.ВВ     ",
      " Контроль Вкл.  ",
      "    Откл.ВВ     ",
      " Контроль Откл. ",
      "   Привод ВВ    ",
      " Прев.Iот.ном.  ",
      " Крит.Ресурс ВВ ",
      " Исч.Ресурс ВВ  ",
      "  Неиспр.Общая  ",
      "  Неиспр.Авар.  ",
      " Работа Ан.Рег. ",
      " Работа Д.Рег.  ",
      " Откл.от защит  ",
      "   Работа БО    ",
      "   Работа БВ    ",
      " 1-я гр.уставок ",
      " 2-я гр.уставок ",
      " 3-я гр.уставок ",
      " 4-я гр.уставок ",
      " Инв.ДВ гр.уст. ",
      "Блк.гр.уст.от з.",
      " С.блк.Гот.к ТУ ",
      " Готовность к ТУ",
      "   Блок.МТЗ1    ",
      "   Блок.МТЗ2    ",
      " Блок.уск.МТЗ2  ",
      "   Блок.МТЗ3    ",
      "   Блок.МТЗ4    ",
      " Сект.МТЗН1 вп. ",
      " Сект.МТЗН1 наз.",
      "    ПО МТЗ1     ",
      " ПО МТЗН1 вперёд",
      " ПО МТЗН1 назад ",
      "  ПО U МТЗПН1   ",
      "   ПО МТЗПН1    ",
      "      МТЗ1      ",
      " Сект.МТЗН2 вп. ",
      " Сект.МТЗН2 наз.",
      "    ПО МТЗ2     ",
      " ПО МТЗН2 вперёд",
      " ПО МТЗН2 назад ",
      "  ПО U МТЗПН2   ",
      "   ПО МТЗПН2    ",
      "      МТЗ2      ",
      " Сект.МТЗН3 вп. ",
      " Сект.МТЗН3 наз.",
      "    ПО МТЗ3     ",
      " ПО МТЗН3 вперёд",
      " ПО МТЗН3 назад ",
      "  ПО U МТЗПН3   ",
      "   ПО МТЗПН3    ",
      "      МТЗ3      ",
      " Сект.МТЗН4 вп. ",
      " Сект.МТЗН4 наз.",
      "    ПО МТЗ4     ",
      " ПО МТЗН4 вперёд",
      " ПО МТЗН4 назад ",
      "  ПО U МТЗПН4   ",
      "   ПО МТЗПН4    ",
      "      МТЗ4      ",
      " ПО блок.U МТЗН ",
      "    НЦН-МТЗ     ",
      "Блок.МТЗ 0.4кВ 1",
      "Блок.МТЗ 0.4кВ 2",
      "Бл.у.МТЗ 0.4кВ 2",
      " ПО МТЗ 0.4кВ 1 ",
      "  МТЗ 0.4кВ 1   ",
      " ПО МТЗ 0.4кВ 2 ",
      "  МТЗ 0.4кВ 2   ",
      " Пуск ЗДЗ от ДВ ",
      "      ЗДЗ       ",
      "    Блок.НЗЗ    ",
      "     ПО НЗЗ     ",
      "      НЗЗ       ",
      "   ПО ЗЗ(3I0)   ",
      "    ЗЗ(3I0)     ",
      "   ПО ЗЗ(3U0)   ",
      "    ЗЗ(3U0)     ",
      "   Сектор НЗЗ   ",
      "   Блок.ТЗНП1   ",
      " Сект.ТЗНП1 вп. ",
      " Сект.ТЗНП1 наз.",
      "ПО 3I0 ТЗНП1 вп.",
      "ПО 3I0 ТЗНП1 наз",
      "ПО 3U0 ТЗНП1 вп.",
      "ПО 3U0 ТЗНП1 наз",
      "  ПО ТЗНП1 вп.  ",
      " ПО ТЗНП1 наз.  ",
      "     ТЗНП1      ",
      "   Блок.ТЗНП2   ",
      " Сект.ТЗНП2 вп. ",
      " Сект.ТЗНП2 наз.",
      "ПО 3I0 ТЗНП2 вп.",
      "ПО 3I0 ТЗНП2 наз",
      "ПО 3U0 ТЗНП2 вп.",
      "ПО 3U0 ТЗНП2 наз",
      "  ПО ТЗНП2 вп.  ",
      " ПО ТЗНП2 наз.  ",
      "     ТЗНП2      ",
      "   Блок.ТЗНП3   ",
      " Сект.ТЗНП3 вп. ",
      " Сект.ТЗНП3 наз.",
      "ПО 3I0 ТЗНП3 вп.",
      "ПО 3I0 ТЗНП3 наз",
      "ПО 3U0 ТЗНП3 вп.",
      "ПО 3U0 ТЗНП3 наз",
      "  ПО ТЗНП3 вп.  ",
      " ПО ТЗНП3 наз.  ",
      "     ТЗНП3      ",
      " Стат.блок.АПВ  ",
      "      АПВ       ",
      "      АПВ2      ",
      "      АПВ3      ",
      "      АПВ4      ",
      "   Работа АПВ   ",
      " АЧР/ЧАПВ от ДВ ",
      "   Блок.АЧР1    ",
      "   Блок.АЧР2    ",
      "   Разр.ЧАПВ    ",
      "   Блок.ЧАПВ    ",
      "    ПО АЧР1     ",
      "    ПО ЧАПВ1    ",
      "   АЧР/ЧАПВ1    ",
      "    ПО АЧР2     ",
      "    ПО ЧАПВ2    ",
      "   АЧР/ЧАПВ2    ",
      " Пуск УРОВ от ДВ",
      "    ПО УРОВ     ",
      "     УРОВ1      ",
      "     УРОВ2      ",
      " Блок.ЗОП(КОФ)  ",
      "  ПО ЗОП(КОФ)   ",
      "    ЗОП(КОФ)    ",
      "  Блок.ЗНмин1   ",
      "  Пуск ЗНмин1   ",
      "  Блок.ЗНмин2   ",
      "  Пуск ЗНмин2   ",
      "   ПО ЗНмин1    ",
      " ПО Uблк.ЗНмин1 ",
      " ПО Iблк.ЗНмин1 ",
      "     ЗНмин1     ",
      "     ПО ЗНмин2  ",
      " ПО Uблк.ЗНмин2 ",
      " ПО Iблк.ЗНмин2 ",
      "     ЗНмин2     ",
      "  Блок.ЗНмакс1  ",
      "   ПО ЗНмакс1   ",
      "    ЗНмакс1     ",
      "  Блок.ЗНмакс2  ",
      "   ПО ЗНмакс2   ",
      "    ЗНмакс2     ",
      " Вх.О-функции1  ",
      " Вых.О-функции1 ",
      " Вх.О-функции2  ",
      " Вых.О-функции2 ",
      " Вх.О-функции3  ",
      " Вых.О-функции3 ",
      " Вх.О-функции4  ",
      " Вых.О-функции4 ",
      " Вх.О-функции5  ",
      " Вых.О-функции5 ",
      " Вх.О-функции6  ",
      " Вых.О-функции6 ",
      " Вх.О-функции7  ",
      " Вых.О-функции7 ",
      " Вх.О-функции8  ",
      " Вых.О-функции8 ",
      " Уст.О-триггера1",
      " Сбр.О-триггера1",
      " Вых.О-триггера1",
      " Уст.О-триггера2",
      " Сбр.О-триггера2",
      " Вых.О-триггера2",
      " Уст.О-триггера3",
      " Сбр.О-триггера3",
      " Вых.О-триггера3",
      " Уст.О-триггера4",
      " Сбр.О-триггера4",
      " Вых.О-триггера4",
      "      О-И1      ",
      "      О-И2      ",
      "      О-И3      ",
      "      О-И4      ",
      "      О-И5      ",
      "      О-И6      ",
      "      О-И7      ",
      "      О-И8      ",
      "     О-ИЛИ1     ",
      "     О-ИЛИ2     ",
      "     О-ИЛИ3     ",
      "     О-ИЛИ4     ",
      "     О-ИЛИ5     ",
      "     О-ИЛИ6     ",
      "     О-ИЛИ7     ",
      "     О-ИЛИ8     ",
      "  О-Искл.ИЛИ1   ",
      "  О-Искл.ИЛИ2   ",
      "  О-Искл.ИЛИ3   ",
      "  О-Искл.ИЛИ4   ",
      "  О-Искл.ИЛИ5   ",
      "  О-Искл.ИЛИ6   ",
      "  О-Искл.ИЛИ7   ",
      "  О-Искл.ИЛИ8   ",
      "     О-НЕ1      ",
      "     О-НЕ2      ",
      "     О-НЕ3      ",
      "     О-НЕ4      ",
      "     О-НЕ5      ",
      "     О-НЕ6      ",
      "     О-НЕ7      ",
      "     О-НЕ8      ",
      "     О-НЕ9      ",
      "     О-НЕ10     ",
      "     О-НЕ11     ",
      "     О-НЕ12     ",
      "     О-НЕ13     ",
      "     О-НЕ14     ",
      "     О-НЕ15     ",
      "     О-НЕ16     ",
      " Ош.настр.р.лог."
    },
    {
      "      Нема      ",
      "   ранжування   ",
      " Блок.ввімкн.ВВ ",
      " Скид.індикації ",
      "   Скид.реле    ",
      " Місц./Дистанц. ",
      "    Стан ВВ     ",
      "Вимк.від зовн.з.",
      "    Ввімк.ВВ    ",
      " Контроль Ввімк.",
      "    Вимк.ВВ     ",
      " Контроль Вимк. ",
      "   Привід ВВ    ",
      " Перев.Iв.ном.  ",
      " Крит.Ресурс ВВ ",
      " Вич.Ресурс ВВ  ",
      " Неспр.Загальна ",
      "  Неспр.Авар.   ",
      " Роб.Ан.Реєстр. ",
      " Роб.Д.Реєстр.  ",
      " Вимк.від зах.  ",
      " Робота БВимк.  ",
      " Робота БВвімк. ",
      " 1-а гр.уставок ",
      " 2-а гр.уставок ",
      " 3-а гр.уставок ",
      " 4-а гр.уставок ",
      " Інв.ДВ гр.уст. ",
      "Бл.гр.уст.від з.",
      " С.блк.Гот.до ТУ",
      "Готовність до ТУ",
      "   Блок.МСЗ1    ",
      "   Блок.МСЗ2    ",
      " Блок.приск.МСЗ2",
      "   Блок.МСЗ3    ",
      "   Блок.МСЗ4    ",
      " Сект.МСЗН1 вп. ",
      " Сект.МСЗН1 наз.",
      "    ПО МСЗ1     ",
      " ПО МСЗН1 вперед",
      " ПО МСЗН1 назад ",
      "  ПО U МСЗПН1   ",
      "   ПО МСЗПН1    ",
      "      МСЗ1      ",
      " Сект.МСЗН2 вп. ",
      " Сект.МСЗН2 наз.",
      "    ПО МСЗ2     ",
      " ПО МСЗН2 вперед",
      " ПО МСЗН2 назад ",
      "  ПО U МСЗПН2   ",
      "   ПО МСЗПН2    ",
      "      МСЗ2      ",
      " Сект.МСЗН3 вп. ",
      " Сект.МСЗН3 наз.",
      "    ПО МСЗ3     ",
      " ПО МСЗН3 вперед",
      " ПО МСЗН3 назад ",
      "  ПО U МСЗПН3   ",
      "   ПО МСЗПН3    ",
      "      МСЗ3      ",
      " Сект.МСЗН4 вп. ",
      " Сект.МСЗН4 наз.",
      "    ПО МСЗ4     ",
      " ПО МСЗН4 вперед",
      " ПО МСЗН4 назад ",
      "  ПО U МСЗПН4   ",
      "   ПО МСЗПН4    ",
      "      МСЗ4      ",
      " ПО блок.U МСЗН ",
      "    НКН-МСЗ     ",
      "Блок.МСЗ 0.4кВ 1",
      "Блок.МСЗ 0.4кВ 2",
      "Бл.п.МСЗ 0.4кВ 2",
      " ПО МСЗ 0.4кВ 1 ",
      "  МСЗ 0.4кВ 1   ",
      " ПО МСЗ 0.4кВ 2 ",
      "  МСЗ 0.4кВ 2   ",
      " Пуск ЗДЗ від ДВ",
      "      ЗДЗ       ",
      "    Блок.НЗЗ    ",
      "     ПО НЗЗ     ",
      "      НЗЗ       ",
      "   ПО ЗЗ(3I0)   ",
      "    ЗЗ(3I0)     ",
      "   ПО ЗЗ(3U0)   ",
      "    ЗЗ(3U0)     ",
      "   Сектор НЗЗ   ",
      "   Блок.СЗНП1   ",
      " Сект.СЗНП1 вп. ",
      " Сект.СЗНП1 наз.",
      "ПО 3I0 СЗНП1 вп.",
      "ПО 3I0 СЗНП1 наз",
      "ПО 3U0 СЗНП1 вп.",
      "ПО 3U0 СЗНП1 наз",
      "  ПО СЗНП1 вп.  ",
      " ПО СЗНП1 наз.  ",
      "     СЗНП1      ",
      "   Блок.СЗНП2   ",
      " Сект.СЗНП2 вп. ",
      " Сект.СЗНП2 наз.",
      "ПО 3I0 СЗНП2 вп.",
      "ПО 3I0 СЗНП2 наз",
      "ПО 3U0 СЗНП2 вп.",
      "ПО 3U0 СЗНП2 наз",
      "  ПО СЗНП2 вп.  ",
      " ПО СЗНП2 наз.  ",
      "     СЗНП2      ",
      "   Блок.СЗНП3   ",
      " Сект.СЗНП3 вп. ",
      " Сект.СЗНП3 наз.",
      "ПО 3I0 СЗНП3 вп.",
      "ПО 3I0 СЗНП3 наз",
      "ПО 3U0 СЗНП3 вп.",
      "ПО 3U0 СЗНП3 наз",
      "  ПО СЗНП3 вп.  ",
      " ПО СЗНП3 наз.  ",
      "     СЗНП3      ",
      " Стат.блок.АПВ  ",
      "      АПВ       ",
      "      АПВ2      ",
      "      АПВ3      ",
      "      АПВ4      ",
      "   Робота АПВ   ",
      " АЧР/ЧАПВ від ДВ",
      "   Блок.АЧР1    ",
      "   Блок.АЧР2    ",
      "  Дозвіл ЧАПВ   ",
      "   Блок.ЧАПВ    ",
      "    ПО АЧР1     ",
      "    ПО ЧАПВ1    ",
      "   АЧР/ЧАПВ1    ",
      "    ПО АЧР2     ",
      "    ПО ЧАПВ2    ",
      "   АЧР/ЧАПВ2    ",
      "Пуск ПРВВ від ДВ",
      "    ПО ПРВВ     ",
      "     ПРВВ1      ",
      "     ПРВВ2      ",
      " Блок.ЗЗП(КОФ)  ",
      "  ПО ЗЗП(КОФ)   ",
      "    ЗЗП(КОФ)    ",
      "  Блок.ЗНмін1   ",
      "  Пуск ЗНмін1   ",
      "  Блок.ЗНмін2   ",
      "  Пуск ЗНмін2   ",
      "   ПО ЗНмін1    ",
      " ПО Uблк.ЗНмін1 ",
      " ПО Iблк.ЗНмін1 ",
      "     ЗНмін1     ",
      "     ПО ЗНмін2  ",
      " ПО Uблк.ЗНмін2 ",
      " ПО Iблк.ЗНмін2 ",
      "     ЗНмін2     ",
      "  Блок.ЗНмакс1  ",
      "   ПО ЗНмакс1   ",
      "    ЗНмакс1     ",
      "  Блок.ЗНмакс2  ",
      "   ПО ЗНмакс2   ",
      "    ЗНмакс2     ",
      " Вх.В-функції1  ",
      " Вих.В-функції1 ",
      " Вх.В-функції2  ",
      " Вих.В-функції2 ",
      " Вх.В-функції3  ",
      " Вих.В-функції3 ",
      " Вх.В-функції4  ",
      " Вих.В-функції4 ",
      " Вх.В-функції5  ",
      " Вих.В-функції5 ",
      " Вх.В-функції6  ",
      " Вих.В-функції6 ",
      " Вх.В-функції7  ",
      " Вих.В-функції7 ",
      " Вх.В-функції8  ",
      " Вих.В-функції8 ",
      " Вст.В-триґера1 ",
      " Ск.В-триґера1  ",
      " Вих.В-триґера1 ",
      " Вст.В-триґера2 ",
      " Ск.В-триґера2  ",
      " Вих.В-триґера2 ",
      " Вст.В-триґера3 ",
      " Ск.В-триґера3  ",
      " Вих.В-триґера3 ",
      " Вст.В-триґера4 ",
      " Ск.В-триґера4  ",
      " Вих.В-триґера4 ",
      "      В-І1      ",
      "      В-І2      ",
      "      В-І3      ",
      "      В-І4      ",
      "      В-І5      ",
      "      В-І6      ",
      "      В-І7      ",
      "      В-І8      ",
      "     В-АБО1     ",
      "     В-АБО2     ",
      "     В-АБО3     ",
      "     В-АБО4     ",
      "     В-АБО5     ",
      "     В-АБО6     ",
      "     В-АБО7     ",
      "     В-АБО8     ",
      "  В-Викл.АБО1   ",
      "  В-Викл.АБО2   ",
      "  В-Викл.АБО3   ",
      "  В-Викл.АБО4   ",
      "  В-Викл.АБО5   ",
      "  В-Викл.АБО6   ",
      "  В-Викл.АБО7   ",
      "  В-Викл.АБО8   ",
      "     В-НЕ1      ",
      "     В-НЕ2      ",
      "     В-НЕ3      ",
      "     В-НЕ4      ",
      "     В-НЕ5      ",
      "     В-НЕ6      ",
      "     В-НЕ7      ",
      "     В-НЕ8      ",
      "     В-НЕ9      ",
      "     В-НЕ10     ",
      "     В-НЕ11     ",
      "     В-НЕ12     ",
      "     В-НЕ13     ",
      "     В-НЕ14     ",
      "     В-НЕ15     ",
      "     В-НЕ16     ",
      " Пом.нал.р.лог. "
    },
    {
      "       No       ",
      "    ranking     ",
      " ON CB Blocking ",
      "Indication Reset",
      "  Relay Reset   ",
      "  Local/Remote  ",
      "  State of CB   ",
      "Откл.от вн.защит",
      "     On CB      ",
      " On CB Control  ",
      "     Off CB     ",
      " Off CB Control ",
      "  CB Actuator   ",
      " Прев.Iот.ном.  ",
      " Крит.Ресурс ВВ ",
      " Исч.Ресурс ВВ  ",
      "  Total Fault   ",
      " Emergence Fault",
      "An.Rec.Operation",
      " D.Rec.Operation",
      " Откл.от защит  ",
      "Off CB Operation",
      " On CB Operation",
      " Pick-up Set 1  ",
      " Pick-up Set 2  ",
      " Pick-up Set 3  ",
      " Pick-up Set 4  ",
      " Инв.ДВ гр.уст. ",
      "Блк.гр.уст.от з.",
      " С.блк.Гот.к ТУ ",
      " Готовность к ТУ",
      "  Blc.of OCP1   ",
      "  Blc.of OCP2   ",
      "  OCP2 Acc.Blc. ",
      "  Blc.of OCP3   ",
      "  Blc.of OCP4   ",
      " Сект.МТЗН1 вп. ",
      " Сект.МТЗН1 наз.",
      "    OCP1 SE     ",
      " ПО МТЗН1 вперёд",
      " ПО МТЗН1 назад ",
      "  ПО U МТЗПН1   ",
      "   ПО МТЗПН1    ",
      "      OCP1      ",
      " Сект.МТЗН2 вп. ",
      " Сект.МТЗН2 наз.",
      "    OCP2 SE     ",
      " ПО МТЗН2 вперёд",
      " ПО МТЗН2 назад ",
      "  ПО U МТЗПН2   ",
      "   ПО МТЗПН2    ",
      "      OCP2      ",
      " Сект.МТЗН3 вп. ",
      " Сект.МТЗН3 наз.",
      "    OCP3 SE     ",
      " ПО МТЗН3 вперёд",
      " ПО МТЗН3 назад ",
      "  ПО U МТЗПН3   ",
      "   ПО МТЗПН3    ",
      "      OCP3      ",
      " Сект.МТЗН4 вп. ",
      " Сект.МТЗН4 наз.",
      "    OCP4 SE     ",
      " ПО МТЗН4 вперёд",
      " ПО МТЗН4 назад ",
      "  ПО U МТЗПН4   ",
      "   ПО МТЗПН4    ",
      "      OCP4      ",
      " ПО блок.U МТЗН ",
      "    НЦН-МТЗ     ",
      "OCP 0.4kV 1 Blc.",
      "OCP 0.4kV 2 Blc.",
      "OCP 0.4kV 2 Ac.B",
      " OCP 0.4kV 1 SE ",
      "  OCP 0.4kV 1   ",
      " OCP 0.4kV 2 SE ",
      "  OCP 0.4kV 2   ",
      " Пуск ЗДЗ от ДВ ",
      "      ЗДЗ       ",
      "    Блок.НЗЗ    ",
      "     ПО НЗЗ     ",
      "      НЗЗ       ",
      "  SGFP(3Io) SE  ",
      "   SGFP(3Io)    ",
      "   ПО ЗЗ(3U0)   ",
      "    ЗЗ(3U0)     ",
      "   Сектор НЗЗ   ",
      "   Блок.ТЗНП1   ",
      " Сект.ТЗНП1 вп. ",
      " Сект.ТЗНП1 наз.",
      "ПО 3I0 ТЗНП1 вп.",
      "ПО 3I0 ТЗНП1 наз",
      "ПО 3U0 ТЗНП1 вп.",
      "ПО 3U0 ТЗНП1 наз",
      "  ПО ТЗНП1 вп.  ",
      " ПО ТЗНП1 наз.  ",
      "     ТЗНП1      ",
      "   Блок.ТЗНП2   ",
      " Сект.ТЗНП2 вп. ",
      " Сект.ТЗНП2 наз.",
      "ПО 3I0 ТЗНП2 вп.",
      "ПО 3I0 ТЗНП2 наз",
      "ПО 3U0 ТЗНП2 вп.",
      "ПО 3U0 ТЗНП2 наз",
      "  ПО ТЗНП2 вп.  ",
      " ПО ТЗНП2 наз.  ",
      "     ТЗНП2      ",
      "   Блок.ТЗНП3   ",
      " Сект.ТЗНП3 вп. ",
      " Сект.ТЗНП3 наз.",
      "ПО 3I0 ТЗНП3 вп.",
      "ПО 3I0 ТЗНП3 наз",
      "ПО 3U0 ТЗНП3 вп.",
      "ПО 3U0 ТЗНП3 наз",
      "  ПО ТЗНП3 вп.  ",
      " ПО ТЗНП3 наз.  ",
      "     ТЗНП3      ",
      "Static Blc.of AR",
      "       AR       ",
      "      AR2       ",
      "      AR3       ",
      "      AR4       ",
      "   Работа АПВ   ",
      "UFLS/FAR from DI",
      "   Блок.АЧР1    ",
      "   Блок.АЧР2    ",
      "   Разр.ЧАПВ    ",
      "   Блок.ЧАПВ    ",
      "    ПО АЧР1     ",
      "   ПО ЧАПВ1     ",
      "   UFLS/FAR1    ",
      "    ПО АЧР2     ",
      "   ПО ЧАПВ2     ",
      "   UFLS/FAR2    ",
      " CBFP Start f.DI",
      "    CBFP SE     ",
      "     CBFP1      ",
      "     CBFP2      ",
      "  Blc.of NPSP   ",
      "    NPSP SE     ",
      "      NPSP      ",
      "   Блок.Umin1   ",
      "   Пуск Umin1   ",
      "   Блок.Umin2   ",
      "   Пуск Umin2   ",
      "    ПО Umin1    ",
      " ПО Uблк.Umin1  ",
      " ПО Iблк.Umin1  ",
      "     Umin1      ",
      "    ПО Umin2    ",
      " ПО Uблк.Umin2  ",
      " ПО Iблк.Umin2  ",
      "     Umin2      ",
      "   Блок.Umax1   ",
      "    ПО Umax1    ",
      "     Umax1      ",
      "   Блок.Umax2   ",
      "    ПО Umax2    ",
      "     Umax2      ",
      "    UDF1 In     ",
      "    UDF1 Out    ",
      "    UDF2 In     ",
      "    UDF2 Out    ",
      "    UDF3 In     ",
      "    UDF3 Out    ",
      "    UDF4 In     ",
      "    UDF4 Out    ",
      "    UDF5 In     ",
      "    UDF5 Out    ",
      "    UDF6 In     ",
      "    UDF6 Out    ",
      "    UDF7 In     ",
      "    UDF7 Out    ",
      "    UDF8 In     ",
      "    UDF8 Out    ",
      "UD Flip-Flop1 S.",
      "UD Flip-Flop1 R.",
      "UD Flip-Flop1 O.",
      "UD Flip-Flop2 S.",
      "UD Flip-Flop2 R.",
      "UD Flip-Flop2 O.",
      "UD Flip-Flop3 S.",
      "UD Flip-Flop3 R.",
      "UD Flip-Flop3 O.",
      "UD Flip-Flop4 S.",
      "UD Flip-Flop4 R.",
      "UD Flip-Flop4 O.",
      "    UD AND1     ",
      "    UD AND2     ",
      "    UD AND3     ",
      "    UD AND4     ",
      "    UD AND5     ",
      "    UD AND6     ",
      "    UD AND7     ",
      "    UD AND8     ",
      "     UD OR1     ",
      "     UD OR2     ",
      "     UD OR3     ",
      "     UD OR4     ",
      "     UD OR5     ",
      "     UD OR6     ",
      "     UD OR7     ",
      "     UD OR8     ",
      "    UD XOR1     ",
      "    UD XOR2     ",
      "    UD XOR3     ",
      "    UD XOR4     ",
      "    UD XOR5     ",
      "    UD XOR6     ",
      "    UD XOR7     ",
      "    UD XOR8     ",
      "    UD NOT1     ",
      "    UD NOT2     ",
      "    UD NOT3     ",
      "    UD NOT4     ",
      "    UD NOT5     ",
      "    UD NOT6     ",
      "    UD NOT7     ",
      "    UD NOT8     ",
      "    UD NOT9     ",
      "    UD NOT10    ",
      "    UD NOT11    ",
      "    UD NOT12    ",
      "    UD NOT13    ",
      "    UD NOT14    ",
      "    UD NOT15    ",
      "    UD NOT16    ",
      " Ош.настр.р.лог."
    },
    {
      "      Нет       ",
      "  ранжирования  ",
      "  Блок.вкл.ВВ   ",
      " Сброс индикации",
      "   Сброс реле   ",
      " Местн./Дистанц.",
      "  Положение ВВ  ",
      "Откл.от вн.защит",
      "     Вкл.ВВ     ",
      " Контроль Вкл.  ",
      "    Откл.ВВ     ",
      " Контроль Откл. ",
      "   Привод ВВ    ",
      " Прев.Iот.ном.  ",
      " Крит.Ресурс ВВ ",
      " Исч.Ресурс ВВ  ",
      "  Неиспр.Общая  ",
      "  Неиспр.Авар.  ",
      " Работа Ан.Рег. ",
      " Работа Д.Рег.  ",
      " Откл.от защит  ",
      "   Работа БО    ",
      "   Работа БВ    ",
      " 1-я гр.уставок ",
      " 2-я гр.уставок ",
      " 3-я гр.уставок ",
      " 4-я гр.уставок ",
      " Инв.ДВ гр.уст. ",
      "Блк.гр.уст.от з.",
      " С.блк.Гот.к ТУ ",
      " Готовность к ТУ",
      "   Блок.МТЗ1    ",
      "   Блок.МТЗ2    ",
      " Блок.уск.МТЗ2  ",
      "   Блок.МТЗ3    ",
      "   Блок.МТЗ4    ",
      " Сект.МТЗН1 вп. ",
      " Сект.МТЗН1 наз.",
      "    ПО МТЗ1     ",
      " ПО МТЗН1 вперёд",
      " ПО МТЗН1 назад ",
      "  ПО U МТЗПН1   ",
      "   ПО МТЗПН1    ",
      "      МТЗ1      ",
      " Сект.МТЗН2 вп. ",
      " Сект.МТЗН2 наз.",
      "    ПО МТЗ2     ",
      " ПО МТЗН2 вперёд",
      " ПО МТЗН2 назад ",
      "  ПО U МТЗПН2   ",
      "   ПО МТЗПН2    ",
      "      МТЗ2      ",
      " Сект.МТЗН3 вп. ",
      " Сект.МТЗН3 наз.",
      "    ПО МТЗ3     ",
      " ПО МТЗН3 вперёд",
      " ПО МТЗН3 назад ",
      "  ПО U МТЗПН3   ",
      "   ПО МТЗПН3    ",
      "      МТЗ3      ",
      " Сект.МТЗН4 вп. ",
      " Сект.МТЗН4 наз.",
      "    ПО МТЗ4     ",
      " ПО МТЗН4 вперёд",
      " ПО МТЗН4 назад ",
      "  ПО U МТЗПН4   ",
      "   ПО МТЗПН4    ",
      "      МТЗ4      ",
      " ПО блок.U МТЗН ",
      "    НЦН-МТЗ     ",
      "Блок.МТЗ 0.4кВ 1",
      "Блок.МТЗ 0.4кВ 2",
      "Бл.у.МТЗ 0.4кВ 2",
      " ПО МТЗ 0.4кВ 1 ",
      "  МТЗ 0.4кВ 1   ",
      " ПО МТЗ 0.4кВ 2 ",
      "  МТЗ 0.4кВ 2   ",
      " Пуск ЗДЗ от ДВ ",
      "      ЗДЗ       ",
      "    Блок.НЗЗ    ",
      "     ПО НЗЗ     ",
      "      НЗЗ       ",
      "   ПО ЗЗ(3I0)   ",
      "    ЗЗ(3I0)     ",
      "   ПО ЗЗ(3U0)   ",
      "    ЗЗ(3U0)     ",
      "   Сектор НЗЗ   ",
      "   Блок.ТЗНП1   ",
      " Сект.ТЗНП1 вп. ",
      " Сект.ТЗНП1 наз.",
      "ПО 3I0 ТЗНП1 вп.",
      "ПО 3I0 ТЗНП1 наз",
      "ПО 3U0 ТЗНП1 вп.",
      "ПО 3U0 ТЗНП1 наз",
      "  ПО ТЗНП1 вп.  ",
      " ПО ТЗНП1 наз.  ",
      "     ТЗНП1      ",
      "   Блок.ТЗНП2   ",
      " Сект.ТЗНП2 вп. ",
      " Сект.ТЗНП2 наз.",
      "ПО 3I0 ТЗНП2 вп.",
      "ПО 3I0 ТЗНП2 наз",
      "ПО 3U0 ТЗНП2 вп.",
      "ПО 3U0 ТЗНП2 наз",
      "  ПО ТЗНП2 вп.  ",
      " ПО ТЗНП2 наз.  ",
      "     ТЗНП2      ",
      "   Блок.ТЗНП3   ",
      " Сект.ТЗНП3 вп. ",
      " Сект.ТЗНП3 наз.",
      "ПО 3I0 ТЗНП3 вп.",
      "ПО 3I0 ТЗНП3 наз",
      "ПО 3U0 ТЗНП3 вп.",
      "ПО 3U0 ТЗНП3 наз",
      "  ПО ТЗНП3 вп.  ",
      " ПО ТЗНП3 наз.  ",
      "     ТЗНП3      ",
      " Стат.блок.АПВ  ",
      "      АПВ       ",
      "      АПВ2      ",
      "      АПВ3      ",
      "      АПВ4      ",
      "   Работа АПВ   ",
      " АЧР/ЧАПВ от ДВ ",
      "   Блок.АЧР1    ",
      "   Блок.АЧР2    ",
      "   Разр.ЧАПВ    ",
      "   Блок.ЧАПВ    ",
      "    ПО АЧР1     ",
      "    ПО ЧАПВ1    ",
      "   АЧР/ЧАПВ1    ",
      "    ПО АЧР2     ",
      "    ПО ЧАПВ2    ",
      "   АЧР/ЧАПВ2    ",
      " Пуск УРОВ от ДВ",
      "    ПО УРОВ     ",
      "     УРОВ1      ",
      "     УРОВ2      ",
      " Блок.ЗОП(КОФ)  ",
      "  ПО ЗОП(КОФ)   ",
      "    ЗОП(КОФ)    ",
      "  Блок.ЗНмин1   ",
      "  Пуск ЗНмин1   ",
      "  Блок.ЗНмин2   ",
      "  Пуск ЗНмин2   ",
      "   ПО ЗНмин1    ",
      " ПО Uблк.ЗНмин1 ",
      " ПО Iблк.ЗНмин1 ",
      "     ЗНмин1     ",
      "     ПО ЗНмин2  ",
      " ПО Uблк.ЗНмин2 ",
      " ПО Iблк.ЗНмин2 ",
      "     ЗНмин2     ",
      "  Блок.ЗНмакс1  ",
      "   ПО ЗНмакс1   ",
      "    ЗНмакс1     ",
      "  Блок.ЗНмакс2  ",
      "   ПО ЗНмакс2   ",
      "    ЗНмакс2     ",
      " Вх.О-функции1  ",
      " Вых.О-функции1 ",
      " Вх.О-функции2  ",
      " Вых.О-функции2 ",
      " Вх.О-функции3  ",
      " Вых.О-функции3 ",
      " Вх.О-функции4  ",
      " Вых.О-функции4 ",
      " Вх.О-функции5  ",
      " Вых.О-функции5 ",
      " Вх.О-функции6  ",
      " Вых.О-функции6 ",
      " Вх.О-функции7  ",
      " Вых.О-функции7 ",
      " Вх.О-функции8  ",
      " Вых.О-функции8 ",
      " Уст.О-триггера1",
      " Сбр.О-триггера1",
      " Вых.О-триггера1",
      " Уст.О-триггера2",
      " Сбр.О-триггера2",
      " Вых.О-триггера2",
      " Уст.О-триггера3",
      " Сбр.О-триггера3",
      " Вых.О-триггера3",
      " Уст.О-триггера4",
      " Сбр.О-триггера4",
      " Вых.О-триггера4",
      "      О-И1      ",
      "      О-И2      ",
      "      О-И3      ",
      "      О-И4      ",
      "      О-И5      ",
      "      О-И6      ",
      "      О-И7      ",
      "      О-И8      ",
      "     О-ИЛИ1     ",
      "     О-ИЛИ2     ",
      "     О-ИЛИ3     ",
      "     О-ИЛИ4     ",
      "     О-ИЛИ5     ",
      "     О-ИЛИ6     ",
      "     О-ИЛИ7     ",
      "     О-ИЛИ8     ",
      "  О-Искл.ИЛИ1   ",
      "  О-Искл.ИЛИ2   ",
      "  О-Искл.ИЛИ3   ",
      "  О-Искл.ИЛИ4   ",
      "  О-Искл.ИЛИ5   ",
      "  О-Искл.ИЛИ6   ",
      "  О-Искл.ИЛИ7   ",
      "  О-Искл.ИЛИ8   ",
      "     О-НЕ1      ",
      "     О-НЕ2      ",
      "     О-НЕ3      ",
      "     О-НЕ4      ",
      "     О-НЕ5      ",
      "     О-НЕ6      ",
      "     О-НЕ7      ",
      "     О-НЕ8      ",
      "     О-НЕ9      ",
      "     О-НЕ10     ",
      "     О-НЕ11     ",
      "     О-НЕ12     ",
      "     О-НЕ13     ",
      "     О-НЕ14     ",
      "     О-НЕ15     ",
      "     О-НЕ16     ",
      " Ош.настр.р.лог."
    }
  };
  unsigned char name_string_tmp[MAX_ROW_RANGUVANNJA_OUTPUT + NUMBER_ROW_FOR_NOTHING_INFORMATION][MAX_COL_LCD];

  int index_language = index_language_in_array(current_settings.language);
  for(int index_1 = 0; index_1 < (MAX_ROW_RANGUVANNJA_OUTPUT + NUMBER_ROW_FOR_NOTHING_INFORMATION); index_1++)
  {
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
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
      name_string_tmp[RANG_BLOCK_NZZ + NUMBER_ROW_FOR_NOTHING_INFORMATION][index_1] = name_block_zz[index_language][index_1];
    }
  }

  if(type_ekran == INDEX_VIEWING_DF)
  {
    unsigned int index_in_ekran_list = number_ekran - EKRAN_RANGUVANNJA_DF1_PLUS;
    unsigned int type_source = index_in_ekran_list % 3;
    unsigned int index_of_df = index_in_ekran_list / 3;
    
    if(current_ekran.edition == 0)
    {
      if(type_source == 0)
      {
        for (unsigned int i = 0; i < N_BIG; i++)
        {
          state_viewing_input[i] = current_settings.ranguvannja_df_source_plus[N_BIG*index_of_df + i];
        }
      }
      else if(type_source == 1)
      {
        for (unsigned int i = 0; i < N_BIG; i++)
        {
          state_viewing_input[i] = current_settings.ranguvannja_df_source_minus[N_BIG*index_of_df + i];
        }
      }
      else
      {
        for (unsigned int i = 0; i < N_BIG; i++)
        {
          state_viewing_input[i] = current_settings.ranguvannja_df_source_blk[N_BIG*index_of_df + i];
        }
      }
    }
    else
    {
      if(type_source == 0)
      {
        for (unsigned int i = 0; i < N_BIG; i++)
        {
          state_viewing_input[i] = edition_settings.ranguvannja_df_source_plus[N_BIG*index_of_df + i];
        }
      }
      else if(type_source == 1)
      {
        for (unsigned int i = 0; i < N_BIG; i++)
        {
          state_viewing_input[i] = edition_settings.ranguvannja_df_source_minus[N_BIG*index_of_df + i];
        }
      }
      else
      {
        for (unsigned int i = 0; i < N_BIG; i++)
        {
          state_viewing_input[i] = edition_settings.ranguvannja_df_source_blk[N_BIG*index_of_df + i];
        }
      }
    }
    max_row_ranguvannja = MAX_ROW_RANGUVANNJA_DF;
  }
  else if(type_ekran == INDEX_VIEWING_DT)
  {
    unsigned int index_in_ekran_list = number_ekran - EKRAN_RANGUVANNJA_SET_DT1_PLUS;
    unsigned int type_source = index_in_ekran_list % 2;
    unsigned int type_of_action = (index_in_ekran_list / 2) & 0x1;
    unsigned int index_of_dt = index_in_ekran_list / 4;
    
    if(current_ekran.edition == 0)
    {
      if (type_of_action == INDEX_ML_SET_DT)
      {
        if(type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT)
        {
          for (unsigned int i = 0; i < N_BIG; i++)
          {
            state_viewing_input[i] = current_settings.ranguvannja_set_dt_source_plus[N_BIG*index_of_dt + i];
          }
        }
        else
        {
          for (unsigned int i = 0; i < N_BIG; i++)
          {
            state_viewing_input[i] = current_settings.ranguvannja_set_dt_source_minus[N_BIG*index_of_dt + i];
          }
        }
      }
      else
      {
        if(type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT)
        {
          for (unsigned int i = 0; i < N_BIG; i++)
          {
            state_viewing_input[i] = current_settings.ranguvannja_reset_dt_source_plus[N_BIG*index_of_dt + i];
          }
        }
        else
        {
          for (unsigned int i = 0; i < N_BIG; i++)
          {
            state_viewing_input[i] = current_settings.ranguvannja_reset_dt_source_minus[N_BIG*index_of_dt + i];
          }
        }
      }
    }
    else
    {
      if (type_of_action == INDEX_ML_SET_DT)
      {
        if(type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT)
        {
          for (unsigned int i = 0; i < N_BIG; i++)
          {
            state_viewing_input[i] = edition_settings.ranguvannja_set_dt_source_plus[N_BIG*index_of_dt + i];
          }
        }
        else
        { 
          for (unsigned int i = 0; i < N_BIG; i++)
          {
            state_viewing_input[i] = edition_settings.ranguvannja_set_dt_source_minus[N_BIG*index_of_dt + i];
          }
        }
      }
      else
      {
        if(type_source == INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT)
        {
          for (unsigned int i = 0; i < N_BIG; i++)
          {
            state_viewing_input[i] = edition_settings.ranguvannja_reset_dt_source_plus[N_BIG*index_of_dt + i];
          }
        }
        else
        { 
          for (unsigned int i = 0; i < N_BIG; i++)
          {
            state_viewing_input[i] = edition_settings.ranguvannja_reset_dt_source_minus[N_BIG*index_of_dt + i];
          }
        }
      }
    }
    max_row_ranguvannja = MAX_ROW_RANGUVANNJA_DT;
  }
  else if(type_ekran == INDEX_VIEWING_D_AND)
  {
    if(current_ekran.edition == 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = current_settings.ranguvannja_d_and[N_BIG*(number_ekran - EKRAN_RANGUVANNJA_D_AND1) + i];
      }
    }
    else
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = edition_settings.ranguvannja_d_and[N_BIG*(number_ekran - EKRAN_RANGUVANNJA_D_AND1) + i];
      }
    }
    max_row_ranguvannja = MAX_ROW_RANGUVANNJA_D_AND;
  }
  else if(type_ekran == INDEX_VIEWING_D_OR)
  {
    if(current_ekran.edition == 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = current_settings.ranguvannja_d_or[N_BIG*(number_ekran - EKRAN_RANGUVANNJA_D_OR1) + i];
      }
    }
    else
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = edition_settings.ranguvannja_d_or[N_BIG*(number_ekran - EKRAN_RANGUVANNJA_D_OR1) + i];
      }
    }
    max_row_ranguvannja = MAX_ROW_RANGUVANNJA_D_OR;
  }
  else if(type_ekran == INDEX_VIEWING_D_XOR)
  {
    if(current_ekran.edition == 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = current_settings.ranguvannja_d_xor[N_BIG*(number_ekran - EKRAN_RANGUVANNJA_D_XOR1) + i];
      }
    }
    else
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = edition_settings.ranguvannja_d_xor[N_BIG*(number_ekran - EKRAN_RANGUVANNJA_D_XOR1) + i];
      }
    }
    max_row_ranguvannja = MAX_ROW_RANGUVANNJA_D_XOR;
  }
  else if(type_ekran == INDEX_VIEWING_D_NOT)
  {
    if(current_ekran.edition == 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = current_settings.ranguvannja_d_not[N_BIG*(number_ekran - EKRAN_RANGUVANNJA_D_NOT1) + i];
      }
    }
    else
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = edition_settings.ranguvannja_d_not[N_BIG*(number_ekran - EKRAN_RANGUVANNJA_D_NOT1) + i];
      }
    }
    max_row_ranguvannja = MAX_ROW_RANGUVANNJA_D_NOT;
  }
  else if(type_ekran == INDEX_VIEWING_OUTPUT)
  {
    if(current_ekran.edition == 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = current_settings.ranguvannja_outputs[N_BIG*(number_ekran - EKRAN_RANGUVANNJA_OUTPUT_1) + i];
      }
    }
    else
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = edition_settings.ranguvannja_outputs[N_BIG*(number_ekran - EKRAN_RANGUVANNJA_OUTPUT_1) + i];
      }
    }
    max_row_ranguvannja = MAX_ROW_RANGUVANNJA_OUTPUT;
  }
  else if(type_ekran == INDEX_VIEWING_LED)
  {
    if(current_ekran.edition == 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = current_settings.ranguvannja_leds[N_BIG*(number_ekran - EKRAN_RANGUVANNJA_LED_1) + i];
      }
    }
    else
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = edition_settings.ranguvannja_leds[N_BIG*(number_ekran - EKRAN_RANGUVANNJA_LED_1) + i];
      }
    }
    max_row_ranguvannja = MAX_ROW_RANGUVANNJA_LED;
  }
  else if(type_ekran == INDEX_VIEWING_A_REG)
  {
    if(current_ekran.edition == 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = current_settings.ranguvannja_analog_registrator[i];
      }
    }
    else
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = edition_settings.ranguvannja_analog_registrator[i];
      }
    }
    max_row_ranguvannja = MAX_ROW_RANGUVANNJA_ANALOG_REGISTRATOR;
  }
  else if(type_ekran == INDEX_VIEWING_D_REG)
  {
    if(current_ekran.edition == 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = current_settings.ranguvannja_digital_registrator[i];
      }
    }
    else
    {
      for (unsigned int i = 0; i < N_BIG; i++)
      {
        state_viewing_input[i] = edition_settings.ranguvannja_digital_registrator[i];
      }
    }
    max_row_ranguvannja = MAX_ROW_RANGUVANNJA_DIGITAL_REGISTRATOR;
  }

  if(current_ekran.edition == 0)
  {
    //Випадок, коли ми продивляємося зранжовані функції на вході
    if (
        (state_viewing_input[0] == 0) && 
        (state_viewing_input[1] == 0) &&
        (state_viewing_input[2] == 0) &&
        (state_viewing_input[3] == 0) &&
        (state_viewing_input[4] == 0) &&
        (state_viewing_input[5] == 0) &&
        (state_viewing_input[6] == 0) &&
        (state_viewing_input[7] == 0)
       )
    {
      //Це означає, що на даний вхід нічого не відранжовано
      
      //Текучу позицію в сипску переводимо на сам початок
      current_ekran.index_position = 0;
      position_in_current_level_menu[number_ekran] = 0;

      //Копіюємо  рядки у робочий екран
      for (unsigned int i=0; i< MAX_ROW_LCD; i++)
      {
        //Копіюємо в робочий екран інформацію, що нічого не відранжовано
        if (i < NUMBER_ROW_FOR_NOTHING_INFORMATION)
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[i][j];
        else
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
      }

      //Відображення курсору по вертикалі
      current_ekran.position_cursor_y = 0;
      //Курсор невидимий
      current_ekran.cursor_on = 0;
    }
    else
    {
      /************************************************************/
      //Формуємо список із функцій, які реально відранжовані
      /************************************************************/
      unsigned int position_temp = current_ekran.index_position;
      unsigned int index_of_ekran;
      unsigned int i = 0, offset = 0;
      unsigned int state_current_bit;
      
      while ((i + offset) < max_row_ranguvannja)
      {
        state_current_bit = state_viewing_input[(i + offset)>>5] & (1<<((i + offset) & 0x1f));
          
        if (state_current_bit == 0)
        {
          for (unsigned int j = i; j < (max_row_ranguvannja - offset); j++)
          {
            if ((j + 1) < (max_row_ranguvannja - offset))
            {
              for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
            }
            else 
            {
              for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
            }
          }
          if (current_ekran.index_position >= ((int)(i + offset))) position_temp--;
          offset++;
        }
        else i++;
      }
      /************************************************************/

      index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
      
      //Копіюємо  рядки у робочий екран
      for (i=0; i< MAX_ROW_LCD; i++)
      {
        //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
        if (index_of_ekran < max_row_ranguvannja)
        {
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran + NUMBER_ROW_FOR_NOTHING_INFORMATION][j];

          //Підтягуємо назву, щоб не було спереді багато пробілів
          unsigned int iteration = 0;
          while (
                 (working_ekran[i][0] == ' ') &&
                 (working_ekran[i][1] == ' ') &&
                 (iteration < (MAX_COL_LCD - 1 - 1))
                )
          {
            for (unsigned int j = 1; j < MAX_COL_LCD; j++)
            {
              if ((j + 1) < MAX_COL_LCD)
                working_ekran[i][j] = working_ekran[i][j + 1];
              else
                working_ekran[i][j] = ' ';
            }
            iteration++;
          }
        }
        else
          for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

        index_of_ekran++;
      }
      //Відображення курору по вертикалі
      current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
      //Курсор видимий
      current_ekran.cursor_on = 1;
    }

    //Курсор по горизонталі відображається на першій позиції
    current_ekran.position_cursor_x = 0;
    //Курсор не мигає
    current_ekran.cursor_blinking_on = 0;
  }
  else
  {
    unsigned int position_temp = current_ekran.index_position;
    unsigned int index_of_ekran;
    unsigned int i, offset = 0;
    int min_max_number[TOTAL_NUMBER_PROTECTION][2] =
    {
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG + NUMBER_APV_SIGNAL_FOR_RANG - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG + NUMBER_APV_SIGNAL_FOR_RANG),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG + NUMBER_APV_SIGNAL_FOR_RANG + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG + NUMBER_APV_SIGNAL_FOR_RANG + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG + NUMBER_APV_SIGNAL_FOR_RANG + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG  + NUMBER_UROV_SIGNAL_FOR_RANG - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG + NUMBER_APV_SIGNAL_FOR_RANG + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG  + NUMBER_UROV_SIGNAL_FOR_RANG),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG + NUMBER_APV_SIGNAL_FOR_RANG + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG  + NUMBER_UROV_SIGNAL_FOR_RANG + NUMBER_ZOP_SIGNAL_FOR_RANG - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG + NUMBER_APV_SIGNAL_FOR_RANG + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG  + NUMBER_UROV_SIGNAL_FOR_RANG + NUMBER_ZOP_SIGNAL_FOR_RANG),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG + NUMBER_APV_SIGNAL_FOR_RANG + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG  + NUMBER_UROV_SIGNAL_FOR_RANG + NUMBER_ZOP_SIGNAL_FOR_RANG + NUMBER_UMIN_SIGNAL_FOR_RANG - 1)
      },
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG + NUMBER_APV_SIGNAL_FOR_RANG + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG  + NUMBER_UROV_SIGNAL_FOR_RANG + NUMBER_ZOP_SIGNAL_FOR_RANG + NUMBER_UMIN_SIGNAL_FOR_RANG),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG + NUMBER_APV_SIGNAL_FOR_RANG + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG  + NUMBER_UROV_SIGNAL_FOR_RANG + NUMBER_ZOP_SIGNAL_FOR_RANG + NUMBER_UMIN_SIGNAL_FOR_RANG + NUMBER_UMAX_SIGNAL_FOR_RANG - 1)
      },
      {-1,-1},
      {
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG + NUMBER_APV_SIGNAL_FOR_RANG + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG + NUMBER_UROV_SIGNAL_FOR_RANG + NUMBER_ZOP_SIGNAL_FOR_RANG + NUMBER_UMIN_SIGNAL_FOR_RANG + NUMBER_UMAX_SIGNAL_FOR_RANG + NUMBER_VMP_SIGNAL_FOR_RANG),
       (NUMBER_GENERAL_SIGNAL_FOR_RANG + NUMBER_MTZ_SIGNAL_FOR_RANG + NUMBER_MTZ04_SIGNAL_FOR_RANG + NUMBER_ZDZ_SIGNAL_FOR_RANG + NUMBER_ZZ_SIGNAL_FOR_RANG + NUMBER_TZNP_SIGNAL_FOR_RANG + NUMBER_APV_SIGNAL_FOR_RANG + NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG + NUMBER_UROV_SIGNAL_FOR_RANG + NUMBER_ZOP_SIGNAL_FOR_RANG + NUMBER_UMIN_SIGNAL_FOR_RANG + NUMBER_UMAX_SIGNAL_FOR_RANG + NUMBER_VMP_SIGNAL_FOR_RANG + NUMBER_EL_SIGNAL_FOR_RANG - 1)
      }
    };
    
    /*************************************************************/
    //Фільтруємо сигнали, яких у даній конфігурації неприсутні
    /*************************************************************/
    if(
       (type_ekran == INDEX_VIEWING_A_REG) ||
       (type_ekran == INDEX_VIEWING_D_REG)
      )
    {
      /*************************************************************/
      //У випадку, якщо відображення здійснюється вікна аналогового реєстратора чи дискретного реєстратора, то відктдпємо ті функції, які не можуть бути джерелати
      /*************************************************************/
      unsigned int index_deleted_function;
      
      if (type_ekran == INDEX_VIEWING_A_REG)
        index_deleted_function = RANG_WORK_A_REJESTRATOR;
      else
        index_deleted_function = RANG_WORK_D_REJESTRATOR;
      
      /*************************************************************/
      //Відкидаємо ім'я даної функції і зміщаємо біти
      /*************************************************************/

      //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
      unsigned int maska[N_BIG] = {0, 0, 0, 0, 0, 0, 0, 0};
      for (unsigned int j = 0; j < (index_deleted_function - offset); j++) _SET_BIT(maska, j);
          
      /***/
      //Зміщуємо біти стану реанжування функцій разом із їх назвами
      /***/
      unsigned int new_temp_data_1[N_BIG], new_temp_data_2[N_BIG];

      for (unsigned int k = 0; k < N_BIG; k++)
      {
        new_temp_data_1[k] = state_viewing_input[k] & maska[k];

        new_temp_data_2[k] = state_viewing_input[k] & (~maska[k]);
      }

      for (unsigned int k = 0; k < (N_BIG - 1); k++)
      {
        new_temp_data_2[k] = ( (new_temp_data_2[k] >> 1) | ((new_temp_data_2[k + 1] & 0x1) << 31) ) & (~maska[k]);
      }
      new_temp_data_2[N_BIG - 1] =  (new_temp_data_2[N_BIG - 1] >> 1) & (~maska[N_BIG - 1]);
                
      for (unsigned int k = 0; k < N_BIG; k++)
      {
        state_viewing_input[k] = new_temp_data_1[k] | new_temp_data_2[k];
      }
      /***/
      for (unsigned int j = (index_deleted_function - offset); j < (max_row_ranguvannja - offset); j++)
      {
        if ((j + 1) < (max_row_ranguvannja - offset))
        {
          for (unsigned int k = 0; k<MAX_COL_LCD; k++)
            name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
        }
        else 
        {
          for (unsigned int k = 0; k<MAX_COL_LCD; k++)
            name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
        }
      }
      if (current_ekran.index_position >= ((int)index_deleted_function)) position_temp--;
      offset++;
      /*************************************************************/

      /*************************************************************/
    }
    else if(type_ekran == INDEX_VIEWING_OUTPUT)
    {
      /*************************************************************/
      //У випадку, якщо відображення здійснюється вікна виходів, то відкидаємо ті функції, які не можуть бути зранжовані на дискретний вихід
      /*
      А це:
            "Робота БО" - якщо вона вже є зранжованою на інші виходи (не той що зараз ранжується, щоб мати можливість його зняти)
            "Робота БВ" - якщо вона вже є зранжованою на інші виходи (не той що зараз ранжується, щоб мати можливість його зняти)
      */
      
      unsigned int current_number_output = number_ekran - EKRAN_RANGUVANNJA_OUTPUT_1;

      for (i = 0; i < 2; i++)
      {
        unsigned int index_deleted_function;
        unsigned int maska_func[N_BIG] = {0, 0, 0, 0, 0, 0, 0, 0};
        unsigned int need_filtration = 0;
        //Визначаємо індекс функції, яку потенційно можливо треба буде фільтрувати із сприску
        //Першою фільтруємо функцію з меншим номером
        if (i == 0)
        {
          index_deleted_function = (RANG_WORK_BV > RANG_WORK_BO) ? RANG_WORK_BO : RANG_WORK_BV;
        }
        else
        {
          index_deleted_function = (RANG_WORK_BV > RANG_WORK_BO) ? RANG_WORK_BV : RANG_WORK_BO;
        }
        
        //Формуємо маску  для цієї функції
        _SET_BIT(maska_func, index_deleted_function);
        
        unsigned int index = 0;
        while ((need_filtration == 0) && (index < NUMBER_OUTPUTS))
        {
          //Пропускаємо текучий номер виходу, бо на ньому дана функція має відображатися під час редагування
          if (index != current_number_output)
          {
            if (
                ((current_settings.ranguvannja_outputs[N_BIG*index    ] & maska_func[0]) != 0) ||
                ((current_settings.ranguvannja_outputs[N_BIG*index + 1] & maska_func[1]) != 0) ||
                ((current_settings.ranguvannja_outputs[N_BIG*index + 2] & maska_func[2]) != 0) ||
                ((current_settings.ranguvannja_outputs[N_BIG*index + 3] & maska_func[3]) != 0) ||
                ((current_settings.ranguvannja_outputs[N_BIG*index + 4] & maska_func[4]) != 0) ||
                ((current_settings.ranguvannja_outputs[N_BIG*index + 5] & maska_func[5]) != 0) ||
                ((current_settings.ranguvannja_outputs[N_BIG*index + 6] & maska_func[6]) != 0) ||
                ((current_settings.ranguvannja_outputs[N_BIG*index + 7] & maska_func[7]) != 0)
               )
            {
              need_filtration = 1;
            }
          }
          index++;
        }
        
        //У випадку, якщо ввстановлено що дану функцію треба відфільтрувати, то відктдпємо її
        if (need_filtration != 0)
        {
          //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
          unsigned int maska[N_BIG] = {0, 0, 0, 0, 0, 0, 0, 0};
          for (unsigned int j = 0; j < (index_deleted_function - offset); j++) _SET_BIT(maska, j);
          
          /***/
          //Зміщуємо біти стану реанжування функцій разом із їх назвами
          /***/
          unsigned int new_temp_data_1[N_BIG], new_temp_data_2[N_BIG];

          for (unsigned int k = 0; k < N_BIG; k++)
          {
            new_temp_data_1[k] = state_viewing_input[k] & maska[k];

            new_temp_data_2[k] = state_viewing_input[k] & (~maska[k]);
          }

          for (unsigned int k = 0; k < (N_BIG - 1); k++)
          {
            new_temp_data_2[k] = ( (new_temp_data_2[k] >> 1) | ((new_temp_data_2[k + 1] & 0x1) << 31) ) & (~maska[k]);
          }
          new_temp_data_2[N_BIG - 1] =  (new_temp_data_2[N_BIG - 1] >> 1) & (~maska[N_BIG - 1]);
                
          for (unsigned int k = 0; k < N_BIG; k++)
          {
            state_viewing_input[k] = new_temp_data_1[k] | new_temp_data_2[k];
          }
          /***/
          for (unsigned int j = (index_deleted_function - offset); j < (max_row_ranguvannja - offset); j++)
          {
            if ((j + 1) < (max_row_ranguvannja - offset))
            {
              for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
            }
            else 
            {
              for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
            }
          }
          if (current_ekran.index_position >= ((int)index_deleted_function)) position_temp--;
          offset++;
          /*************************************************************/
        }
      }
      /*************************************************************/
    }

    //Функції загального призначення пропускаємо (вони знаходяться у початку списку), тому починаємо з першого записту
    int index_in_list = NUMBER_GENERAL_SIGNAL_FOR_RANG;
    
    for (i = 0; i < TOTAL_NUMBER_PROTECTION; i++)
    {
      
      if((current_settings.configuration & (1 << i)) != 0)
      {
        //Захист присутнійсть, тому функції фільтрувати не потрібно - переводимо індекс на наступні функції
        //Додаємо кількість функцій до поточного індексу, якщо для поточного захисту реально були присутні функції,
        //бо інкаше ми вже знаходимося на індексі наступного захисту
        if(min_max_number[i][0] >=0)
        {
          if (
              (i == ZZ_BIT_CONFIGURATION) &&
              ((current_settings.control_zz & CTR_ZZ1_TYPE) != 0)
             )
          {
            /*
            Випадок коли сигнали, які відповідають за НЗЗ треба відфільтрувати
            */

            //Відкидати імена функцій і зміщати біти треба тільки у тому випадку, якщо функції пристні у списку для ранжування для даного захисту
            //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
            unsigned int maska[N_BIG] = {0, 0, 0, 0, 0, 0, 0, 0};
            unsigned int j1;
            for (j1 = 0; j1 < (min_max_number[i][0] - offset); j1++) _SET_BIT(maska, j1);
          
            //Відкидаємо назви функцій із списку, які є зайвими
            while(index_in_list <= min_max_number[i][1])
            {
              if (
                  (index_in_list == RANG_PO_NZZ    ) ||
                  (index_in_list == RANG_NZZ       ) ||
                  (index_in_list == RANG_SECTOR_NZZ)
                 )
              {
                /***/
                //Зміщуємо біти стану реанжування функцій разом із їх назвами
                /***/
                unsigned int new_temp_data_1[N_BIG], new_temp_data_2[N_BIG];

                for (unsigned int k = 0; k < N_BIG; k++)
                {
                  new_temp_data_1[k] = state_viewing_input[k] & maska[k];

                  new_temp_data_2[k] = state_viewing_input[k] & (~maska[k]);
                }

                for (unsigned int k = 0; k < (N_BIG - 1); k++)
                {
                  new_temp_data_2[k] = ( (new_temp_data_2[k] >> 1) | ((new_temp_data_2[k + 1] & 0x1) << 31) ) & (~maska[k]);
                }
                new_temp_data_2[N_BIG - 1] =  (new_temp_data_2[N_BIG - 1] >> 1) & (~maska[N_BIG - 1]);
                
                for (unsigned int k = 0; k < N_BIG; k++)
                {
                  state_viewing_input[k] = new_temp_data_1[k] | new_temp_data_2[k];
                }
                /***/
                for (unsigned int j = (index_in_list - offset); j < max_row_ranguvannja; j++)
                {
                  if ((j + 1) < (max_row_ranguvannja - offset))
                  {
                    for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                      name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
                  }
                  else 
                  {
                    for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                      name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
                  }
                }
                if (current_ekran.index_position >= index_in_list) position_temp--;
          
                offset++;
              }
              else
              {
                _SET_BIT(maska, j1);
                j1++;
              }
                
              index_in_list++;
            }
          }
          else if (i == EL_BIT_CONFIGURATION)
          {
            /*
            Випадок коли деякі сигнали розширеної логіки треба відфільтрувати
            */

            //Відкидати імена функцій і зміщати біти треба тільки у тому випадку, якщо функції пристні у списку для ранжування для даного захисту
            //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
            unsigned int maska[N_BIG] = {0, 0, 0, 0, 0, 0, 0, 0};
            unsigned int j1;
            for (j1 = 0; j1 < (min_max_number[i][0] - offset); j1++) _SET_BIT(maska, j1);
          
            //Відкидаємо назви функцій із списку, які є зайвими
            while(index_in_list <= min_max_number[i][1])
            {
              if (
                  (
                   /*
                   (
                    (index_in_list >= (int)(RANG_DF1_IN + 2*current_settings.number_defined_df)) &&
                    (index_in_list <= RANG_DF8_OUT)
                   )
                   ||
                   */
                   (
                    (type_ekran == INDEX_VIEWING_DF) &&
                    (
                     (index_in_list == (RANG_DF1_IN  + 2*(number_ekran - EKRAN_RANGUVANNJA_DF1_PLUS) / 3)) ||
                     (index_in_list == (RANG_DF1_OUT + 2*(number_ekran - EKRAN_RANGUVANNJA_DF1_PLUS) / 3))
                    )  
                   )
                  )   
                  ||  
                  (
                   /*
                   (
                    (index_in_list >= (int)(RANG_DT1_SET + 3*current_settings.number_defined_dt)) &&
                    (index_in_list <= RANG_DT4_OUT)
                   )
                   ||
                   */
                   (
                    (type_ekran == INDEX_VIEWING_DT) &&
                    (
                     (index_in_list == (RANG_DT1_SET   + 3*(number_ekran - EKRAN_RANGUVANNJA_SET_DT1_PLUS) / 4)) ||
                     (index_in_list == (RANG_DT1_RESET + 3*(number_ekran - EKRAN_RANGUVANNJA_SET_DT1_PLUS) / 4)) ||
                     (index_in_list == (RANG_DT1_OUT   + 3*(number_ekran - EKRAN_RANGUVANNJA_SET_DT1_PLUS) / 4))
                    )  
                   ) 
                  )   
                  ||  
                  (
                   /*
                   (
                    (index_in_list >= (int)(RANG_D_AND1 + current_settings.number_defined_and)) &&
                    (index_in_list <= RANG_D_AND8)
                   )   
                   ||
                   */
                   (
                    (type_ekran == INDEX_VIEWING_D_AND) &&
                    (index_in_list == (RANG_D_AND1 + (number_ekran - EKRAN_RANGUVANNJA_D_AND1)))
                   )   
                  )   
                  ||  
                  (
                   /*
                   (
                    (index_in_list >= (int)(RANG_D_OR1 + current_settings.number_defined_or)) &&
                    (index_in_list <= RANG_D_OR8)
                   )   
                   ||
                   */
                   (
                    (type_ekran == INDEX_VIEWING_D_OR) &&
                    (index_in_list == (RANG_D_OR1 + (number_ekran - EKRAN_RANGUVANNJA_D_OR1)))
                   )   
                  )   
                  ||  
                  (
                   /*
                   (
                    (index_in_list >= (int)(RANG_D_XOR1 + current_settings.number_defined_xor)) &&
                    (index_in_list <= RANG_D_XOR8)
                   )   
                   ||
                   */
                   (
                    (type_ekran == INDEX_VIEWING_D_XOR) &&
                    (index_in_list == (RANG_D_XOR1 + (number_ekran - EKRAN_RANGUVANNJA_D_XOR1)))
                   )   
                  )   
                  ||  
                  (
                   /*
                   (
                    (index_in_list >= (int)(RANG_D_NOT1 + current_settings.number_defined_not)) &&
                    (index_in_list <= RANG_D_NOT16)
                   )   
                   ||
                   */
                   (
                    (type_ekran == INDEX_VIEWING_D_NOT) &&
                    (index_in_list == (RANG_D_NOT1 + (number_ekran - EKRAN_RANGUVANNJA_D_NOT1)))
                   )   
                  )   
                 )
              {
                /***/
                //Зміщуємо біти стану реанжування функцій разом із їх назвами
                /***/
                unsigned int new_temp_data_1[N_BIG], new_temp_data_2[N_BIG];

                for (unsigned int k = 0; k < N_BIG; k++)
                {
                  new_temp_data_1[k] = state_viewing_input[k] & maska[k];

                  new_temp_data_2[k] = state_viewing_input[k] & (~maska[k]);
                }

                for (unsigned int k = 0; k < (N_BIG - 1); k++)
                {
                  new_temp_data_2[k] = ( (new_temp_data_2[k] >> 1) | ((new_temp_data_2[k + 1] & 0x1) << 31) ) & (~maska[k]);
                }
                new_temp_data_2[N_BIG - 1] =  (new_temp_data_2[N_BIG - 1] >> 1) & (~maska[N_BIG - 1]);
                
                for (unsigned int k = 0; k < N_BIG; k++)
                {
                  state_viewing_input[k] = new_temp_data_1[k] | new_temp_data_2[k];
                }
                /***/
                for (unsigned int j = (index_in_list - offset); j < (max_row_ranguvannja - offset); j++)
                {
                  if ((j + 1) < (max_row_ranguvannja - offset))
                  {
                    for (unsigned int k = 0; k < MAX_COL_LCD; k++)
                      name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
                  }
                  else 
                  {
                    for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                      name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
                  }
                }
                if (current_ekran.index_position >= index_in_list) position_temp--;
          
                offset++;
              }
              else
              {
                _SET_BIT(maska, j1);
                j1++;
              }
                
              index_in_list++;
            }
          }
          else
            index_in_list += ((min_max_number[i][1] - min_max_number[i][0]) + 1);
        }
      }
      else if (min_max_number[i][0] >=0)
      {
        //Відкидати імена функцій і зміщати біти треба тільки у тому випадку, якщо функції пристні у списку для ранжування для даного захисту
        //Формуємо маску біт, які не треба переміщати при переміщенні імен полів
        unsigned int maska[N_BIG] = {0, 0, 0, 0, 0, 0, 0, 0};
        for (unsigned int j = 0; j < (min_max_number[i][0] - offset); j++) _SET_BIT(maska, j);
          
        //Відкидаємо назви функцій із списку, які є зайвими
        while(index_in_list <= min_max_number[i][1])
        {
          /***/
          //Зміщуємо біти стану реанжування функцій разом із їх назвами
          /***/
          unsigned int new_temp_data_1[N_BIG], new_temp_data_2[N_BIG];

          for (unsigned int k = 0; k < N_BIG; k++)
          {
            new_temp_data_1[k] = state_viewing_input[k] & maska[k];

            new_temp_data_2[k] = state_viewing_input[k] & (~maska[k]);
          }

          for (unsigned int k = 0; k < (N_BIG - 1); k++)
          {
            new_temp_data_2[k] = ( (new_temp_data_2[k] >> 1) | ((new_temp_data_2[k + 1] & 0x1) << 31) ) & (~maska[k]);
          }
          new_temp_data_2[N_BIG - 1] =  (new_temp_data_2[N_BIG - 1] >> 1) & (~maska[N_BIG - 1]);
                
          for (unsigned int k = 0; k < N_BIG; k++)
          {
            state_viewing_input[k] = new_temp_data_1[k] | new_temp_data_2[k];
          }
          /***/
          for (unsigned int j = (index_in_list - offset); j < (max_row_ranguvannja - offset); j++)
          {
            if ((j + 1) < (max_row_ranguvannja - offset))
            {
              for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION + 1][k];
            }
            else 
            {
              for (unsigned int k = 0; k<MAX_COL_LCD; k++)
                name_string_tmp[j + NUMBER_ROW_FOR_NOTHING_INFORMATION][k] = ' ';
            }
          }
          if (current_ekran.index_position >= index_in_list) position_temp--;
          
          offset++;
          index_in_list++;
        }
      }
    }
    /*************************************************************/

    //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

    for (i=0; i< MAX_ROW_LCD; i++)
    {
     if (index_of_ekran < (max_row_ranguvannja<<1))//Множення на два константи  max_row_ranguvannja потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
     {
       if ((i & 0x1) == 0)
       {
         //У непарному номері рядку виводимо заголовок
         for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[(index_of_ekran>>1) + NUMBER_ROW_FOR_NOTHING_INFORMATION][j];
       }
       else
       {
         //У парному номері рядку виводимо стан функції
         const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
         {
           {"      ОТКЛ      ", "      ВКЛ       "},
           {"      ВИМК      ", "     ВВІМК      "},
           {"      OFF       ", "       ON       "},
           {"      СЉНД      ", "      КОСУ      "}
         };
         unsigned int index_bit = index_of_ekran >> 1;
         
         for (unsigned int j = 0; j < MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][((state_viewing_input[index_bit >> 5] & ( 1<< (index_bit & 0x1f))) != 0)][j];
       }
     }
     else
     for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

     index_of_ekran++;
    }
    
    const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][2] = 
    {
      {5, 5},
      {5, 4},
      {5, 6},
      {5, 5}
    };

    //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
    current_ekran.position_cursor_x =  cursor_x[index_language][((state_viewing_input[position_temp >> 5] & (1 << (position_temp & 0x1f))) != 0)];
    current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
    
    //Курсор мигає
    current_ekran.cursor_blinking_on = 1;
    //Режим відображення у режимі редагування
  }
  

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;

#undef NUMBER_ROW_FOR_NOTHING_INFORMATION
}
/*****************************************************/

/*****************************************************/
//Перевірка, чи текучий індекс вказує на функцію, яка присутня у даній конфігурації
/*****************************************************/
void check_current_index_is_presented_in_configuration(
                                                         unsigned int* found_new_index_tmp,
                                                                  int* add_filter_point,
                                                                  /*EL_FILTER_STRUCT el_filter[],*/
                                                                  int plus_minus,
                                                                  int number_general_function,
                                                                  int number_mtz_function,
                                                                  int number_mtz04_function,
                                                                  int number_zdz_function,
                                                                  int number_zz_function,
                                                                  int number_tznp_function,
                                                                  int number_apv_function,
                                                                  int number_achr_chapv_function,
                                                                  int number_urov_function,
                                                                  int number_zop_function,
                                                                  int number_Umin_function,
                                                                  int number_Umax_function,
                                                                  int number_vmp_function,
                                                                  int number_el_function
                                                        )
{
  if(plus_minus == 1)
  {
    if (current_ekran.index_position < number_general_function) *found_new_index_tmp = 1;
    else
    {
      if (current_ekran.index_position < (number_general_function + number_mtz_function))
      {
        if ((current_settings.configuration & (1<<MTZ_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function))
      {
        if ((current_settings.configuration & (1<<MTZ04_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function))
      {
        if ((current_settings.configuration & (1<<ZDZ_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function))
      {
        if ((current_settings.configuration & (1<<ZZ_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function))
      {
        if ((current_settings.configuration & (1<<TZNP_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function))
      {
        if ((current_settings.configuration & (1<<APV_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function))
      {
        if ((current_settings.configuration & (1<<ACHR_CHAPV_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function))
      {
        if ((current_settings.configuration & (1<<UROV_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function))
      {
        if ((current_settings.configuration & (1<<ZOP_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function))
      {
        if ((current_settings.configuration & (1<<UMIN_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function + number_Umax_function))
      {
        if ((current_settings.configuration & (1<<UMAX_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function + number_Umax_function;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function + number_Umax_function + number_vmp_function))
      {
        if ((current_settings.configuration & (1<<VMP_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function + number_Umax_function + number_vmp_function;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function + number_Umax_function + number_vmp_function + number_el_function))
      {
        if ((current_settings.configuration & (1<<EL_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function + number_Umax_function + number_vmp_function + number_el_function;
      }
    }

    if ((current_settings.control_zz & CTR_ZZ1_TYPE) != 0)  
    {
      unsigned int i = 0;
      while (
             (*found_new_index_tmp == 1) &&
             (add_filter_point[i]  >= 0)
            )
      {
        if (current_ekran.index_position == add_filter_point[i])
        {
          *found_new_index_tmp = 0;
          current_ekran.index_position++;
        }
        else i++;
      }
    }
    
//    for (unsigned int i = 0; i < NUMBER_DEFINED_ELEMENTS; i++)
//    {
//      if (
//          (el_filter[i].present != 0) &&
//          (current_ekran.index_position >= (el_filter[i].start_index + el_filter[i].number_per_index*el_filter[i].real_number)) &&
//          (current_ekran.index_position <=  el_filter[i].stop_index)
//         )
//      {
//        *found_new_index_tmp = 0;
//        current_ekran.index_position++;
//        break; //вихід із циклу
//      }
//    }
  }
  else
  {
    if (current_ekran.index_position < number_general_function) *found_new_index_tmp = 1;
    else
    {
      if (current_ekran.index_position < (number_general_function + number_mtz_function))
      {
        if ((current_settings.configuration & (1<<MTZ_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function - 1;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function))
      {
        if ((current_settings.configuration & (1<<MTZ04_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function - 1;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function))
      {
        if ((current_settings.configuration & (1<<ZDZ_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function - 1;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function))
      {
        if ((current_settings.configuration & (1<<ZZ_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function - 1;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function))
      {
        if ((current_settings.configuration & (1<<TZNP_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function - 1;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function))
      {
        if ((current_settings.configuration & (1<<APV_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function - 1;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function))
      {
        if ((current_settings.configuration & (1<<ACHR_CHAPV_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + - 1;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function))
      {
        if ((current_settings.configuration & (1<<UROV_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function - 1;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function))
      {
        if ((current_settings.configuration & (1<<ZOP_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function - 1;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function))
      {
        if ((current_settings.configuration & (1<<UMIN_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function - 1;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function + number_Umax_function))
      {
        if ((current_settings.configuration & (1<<UMAX_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function - 1;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function + number_Umax_function + number_vmp_function))
      {
        if ((current_settings.configuration & (1<<VMP_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function + number_Umax_function - 1;
      }
      else if (current_ekran.index_position < (number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function + number_Umax_function + number_vmp_function + number_el_function))
      {
        if ((current_settings.configuration & (1<<EL_BIT_CONFIGURATION)) != 0) *found_new_index_tmp = 1;
        else current_ekran.index_position = number_general_function + number_mtz_function + number_mtz04_function + number_zdz_function + number_zz_function + number_tznp_function + number_apv_function + number_achr_chapv_function + number_urov_function + number_zop_function + number_Umin_function + number_Umax_function + number_vmp_function - 1;
      }
    }

    if ((current_settings.control_zz & CTR_ZZ1_TYPE) != 0)
    {
      unsigned int i = 0;
      while (
             (*found_new_index_tmp == 1) &&
             (add_filter_point[i]  >= 0)
            )
      {
        if (current_ekran.index_position == add_filter_point[i])
        {
          *found_new_index_tmp = 0;
          current_ekran.index_position--;
        }
        else i++;
      }
    }
    
//    for (unsigned int i = 0; i < NUMBER_DEFINED_ELEMENTS; i++)
//    {
//      if (
//          (el_filter[i].present != 0) &&
//          (current_ekran.index_position >= (el_filter[i].start_index + el_filter[i].number_per_index*el_filter[i].real_number)) &&
//          (current_ekran.index_position <=  el_filter[i].stop_index)
//         )
//      {
//        *found_new_index_tmp = 0;
//        current_ekran.index_position--;
//        break; //вихід із циклу
//      }
//    }
  }
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
