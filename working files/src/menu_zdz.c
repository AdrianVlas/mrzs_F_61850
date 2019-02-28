#include "header.h"

/*****************************************************/
//Формуємо екран відображення витримок ЗДЗ
/*****************************************************/
void make_ekran_timeout_zdz(unsigned int group)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_ZDZ][MAX_COL_LCD] = 
  {
    {
      "     T ЗДЗ      "
    },
    {
      "     T ЗДЗ      "
    },
    {
      "     T ЗДЗ      "
    },
    {
      "     T ЗДЗ      "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < MAX_ROW_FOR_TIMEOUT_ZDZ)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
        if (view == true) value = current_settings.timeout_zdz[group]; //у змінну value поміщаємо значення витримки
        else value = edition_settings.timeout_zdz[group];
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (
              ((j < COL_TMO_ZDZ_BEGIN) ||  (j > COL_TMO_ZDZ_END )) &&
              (j != (COL_TMO_ZDZ_END + 2))  
             )working_ekran[i][j] = ' ';
          else if (j == COL_TMO_ZDZ_COMMA )working_ekran[i][j] = ',';
          else if (j == (COL_TMO_ZDZ_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
          else
            calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_ZDZ_COMMA, view, 0);
        }
      }
        
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    current_ekran.position_cursor_x = COL_TMO_ZDZ_BEGIN;
    int last_position_cursor_x = COL_TMO_ZDZ_END;

    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;

    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
  }
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  else current_ekran.cursor_blinking_on = 1;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення значення управлінської інформації для ЗДЗ
/*****************************************************/
void make_ekran_control_zdz()
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_ZDZ][MAX_COL_LCD] = 
  {
    {
      "      ЗДЗ       ",
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   
      "      ОВД1      ",
      "      ОВД2      ",
      "      ОВД3      ",
      "  Порог чувст.  ",
#endif
      "   Выбор ЗДЗ    ",
      "  Пуск от МТЗ1  ",
      "  Пуск от МТЗ2  ",
      "  Пуск от МТЗ3  ",
      "  Пуск от МТЗ4  ",
      " Пуск от ЗНмин1 ",
      " Пуск от ЗНмин2 "
    },
    {
      "      ЗДЗ       ",
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   
      "      ОВД1      ",
      "      ОВД2      ",
      "      ОВД3      ",
      "  Поріг чутл.   ",
#endif
      "   Вибір ЗДЗ    ",
      " Пуск від МСЗ1  ",
      " Пуск від МСЗ2  ",
      " Пуск від МСЗ3  ",
      " Пуск від МСЗ4  ",
      " Пуск від ЗНмін1",
      " Пуск від ЗНмін2"
    },
    {
      "      ЗДЗ       ",
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   
      "      ОВД1      ",
      "      ОВД2      ",
      "      ОВД3      ",
      "  Порог чувст.  ",
#endif
      "   Выбор ЗДЗ    ",
      " Start from OCP1",
      " Start from OCP2",
      " Start from OCP3",
      " Start from OCP4",
      " Пуск от ЗНмин1 ",
      " Пуск от ЗНмин2 "
    },
    {
      "      ЗДЗ       ",
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   
      "      ОВД1      ",
      "      ОВД2      ",
      "      ОВД3      ",
      "  Порог чувст.  ",
#endif
      "   Выбор ЗДЗ    ",
      "  Пуск от МТЗ1  ",
      "  Пуск от МТЗ2  ",
      "  Пуск от МТЗ3  ",
      "  Пуск от МТЗ4  ",
      " Пуск от ЗНмин1 ",
      " Пуск от ЗНмин2 "
    }
  };
  
  const unsigned char information_1[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
  {
    {"     Откл.      ", "      Вкл.      "},
    {"     Вимк.      ", "     Ввімк.     "},
    {"      Off       ", "       On       "},
    {"     Сљнд.      ", "     Косу.      "}
  };
  const unsigned int cursor_x_1[MAX_NAMBER_LANGUAGE][2] = 
  {
   {4, 5},
   {4, 4},
   {5, 6},
   {4, 4}
  };
  const unsigned char information_2[MAX_NAMBER_LANGUAGE][_ZDZ_CTRL_NUMBER][MAX_COL_LCD] = 
  {
    {"  Без контроля  ", "       I        ", "       U        ", "    I или U     ", "     I и U      ", "      3I0       "},
    {"  Без контролю  ", "       I        ", "       U        ", "    I або U     ", "     I і U      ", "      3I0       "},
    {" Without control", "       I        ", "       U        ", "    I or U      ", "    I and U     ", "      3I0       "},
    {"  Без контроля  ", "       I        ", "       U        ", "    I или U     ", "     I и U      ", "      3I0       "}
  };
  const unsigned int cursor_x_2[MAX_NAMBER_LANGUAGE][_ZDZ_CTRL_NUMBER] = 
  {
    {1, 6, 6, 3, 4, 5},
    {1, 6, 6, 3, 4, 5},
    {0, 6, 6, 3, 3, 5},
    {1, 6, 6, 3, 4, 5}
  };

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   
  const uint8_t information_3[ZDZ_CTRL_PORIG_N][MAX_COL_LCD] = 
  {
    "       1        ",
    "       2        ",
    "       3        ",
    "       4        ",
    "       5        ",
    "       6        ",
    "       7        ",
    "       8        ",
    "       9        ",
    "       10       ",
    "       11       ",
    "       12       "
  };
  const uint32_t cursor_x_3[ZDZ_CTRL_PORIG_N] = {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6};
#endif
  
  int index_language = index_language_in_array(current_settings.language);
  __SETTINGS *point = (current_ekran.edition == 0) ? &current_settings : &edition_settings;

  __ctrl_info ctrl_info[MAX_ROW_FOR_CONTROL_ZDZ] =
  {
    {information_1[index_language][(point->control_zdz >> CTR_ZDZ_STATE_BIT) & 0x1], cursor_x_1[index_language][(point->control_zdz >> CTR_ZDZ_STATE_BIT) & 0x1]},
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   
    {information_1[index_language][(point->control_zdz >> CTR_ZDZ_OVD1_STATE_BIT) & 0x1], cursor_x_1[index_language][(point->control_zdz >> CTR_ZDZ_OVD1_STATE_BIT) & 0x1]},
    {information_1[index_language][(point->control_zdz >> CTR_ZDZ_OVD2_STATE_BIT) & 0x1], cursor_x_1[index_language][(point->control_zdz >> CTR_ZDZ_OVD2_STATE_BIT) & 0x1]},
    {information_1[index_language][(point->control_zdz >> CTR_ZDZ_OVD3_STATE_BIT) & 0x1], cursor_x_1[index_language][(point->control_zdz >> CTR_ZDZ_OVD3_STATE_BIT) & 0x1]},
    {information_3[point->zdz_ovd_porig], cursor_x_3[point->zdz_ovd_porig]},
#endif
    {information_2[index_language][point->ctrl_zdz_type], cursor_x_2[index_language][point->ctrl_zdz_type]},
    {information_1[index_language][(point->control_zdz >> (CTR_ZDZ_STARTED_FROM_MTZ1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))) & 0x1], cursor_x_1[index_language][(point->control_zdz >> (CTR_ZDZ_STARTED_FROM_MTZ1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))) & 0x1]},
    {information_1[index_language][(point->control_zdz >> (CTR_ZDZ_STARTED_FROM_MTZ2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))) & 0x1], cursor_x_1[index_language][(point->control_zdz >> (CTR_ZDZ_STARTED_FROM_MTZ2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))) & 0x1]},
    {information_1[index_language][(point->control_zdz >> (CTR_ZDZ_STARTED_FROM_MTZ3_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))) & 0x1], cursor_x_1[index_language][(point->control_zdz >> (CTR_ZDZ_STARTED_FROM_MTZ3_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))) & 0x1]},
    {information_1[index_language][(point->control_zdz >> (CTR_ZDZ_STARTED_FROM_MTZ4_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))) & 0x1], cursor_x_1[index_language][(point->control_zdz >> (CTR_ZDZ_STARTED_FROM_MTZ4_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))) & 0x1]},
    {information_1[index_language][(point->control_zdz >> (CTR_ZDZ_STARTED_FROM_UMIN1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))) & 0x1], cursor_x_1[index_language][(point->control_zdz >> (CTR_ZDZ_STARTED_FROM_UMIN1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))) & 0x1]},
    {information_1[index_language][(point->control_zdz >> (CTR_ZDZ_STARTED_FROM_UMIN2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))) & 0x1], cursor_x_1[index_language][(point->control_zdz >> (CTR_ZDZ_STARTED_FROM_UMIN2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))) & 0x1]}
  };
  
  unsigned char name_string_tmp[MAX_ROW_FOR_CONTROL_ZDZ][MAX_COL_LCD];

  for(int index_1 = 0; index_1 < MAX_ROW_FOR_CONTROL_ZDZ; index_1++)
  {
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
  }
  
  /******************************************/
  //Виключаємо поля, які не треба відображати
  /******************************************/
  int additional_current_mtz = 0, additional_current_Umin = 0;
  int position_temp = current_ekran.index_position;
  int index_of_ekran;
  
  int additional_current = additional_current_mtz + additional_current_Umin;

  for (int current_index = _CTR_ZDZ_PART_III; current_index < MAX_ROW_FOR_CONTROL_ZDZ; current_index++ )
  {

    if (
        (
         (
          (current_index == CTR_ZDZ_STARTED_FROM_MTZ1_BIT) ||
          (current_index == CTR_ZDZ_STARTED_FROM_MTZ2_BIT) ||
          (current_index == CTR_ZDZ_STARTED_FROM_MTZ3_BIT) ||
          (current_index == CTR_ZDZ_STARTED_FROM_MTZ4_BIT)
         )   
         &&
         ((current_settings.configuration & (1<<MTZ_BIT_CONFIGURATION)) == 0)
        )
        ||
        (
         (
          (current_index == CTR_ZDZ_STARTED_FROM_UMIN1_BIT) ||
          (current_index == CTR_ZDZ_STARTED_FROM_UMIN2_BIT)
         )   
         &&
         ((current_settings.configuration & (1<<UMIN_BIT_CONFIGURATION)) == 0)
        )
       )   
    {
      int i = current_index - additional_current;

      if ((i+1) <= position_temp) position_temp--;
      do
      {
        if ((i+1) < (MAX_ROW_FOR_CONTROL_ZDZ - additional_current))
        {
          ctrl_info[i] = ctrl_info[i + 1];
          for(unsigned int j = 0; j<MAX_COL_LCD; j++) name_string_tmp[i][j] = name_string_tmp[i + 1][j];
        }
        else 
        {
          for(unsigned int j = 0; j<MAX_COL_LCD; j++) name_string_tmp[i][j] = ' ';
        }
        i++;
      }
      while (i < (MAX_ROW_FOR_CONTROL_ZDZ - additional_current));
    
      if (
          (current_index == CTR_ZDZ_STARTED_FROM_MTZ1_BIT) ||
          (current_index == CTR_ZDZ_STARTED_FROM_MTZ2_BIT) ||
          (current_index == CTR_ZDZ_STARTED_FROM_MTZ3_BIT) ||
          (current_index == CTR_ZDZ_STARTED_FROM_MTZ4_BIT)
         )   
        additional_current_mtz++;

      if (
          (current_index == CTR_ZDZ_STARTED_FROM_UMIN1_BIT) ||
          (current_index == CTR_ZDZ_STARTED_FROM_UMIN2_BIT)
         )   
        additional_current_Umin++;
      
      additional_current = additional_current_mtz + additional_current_Umin;
    }
  }
  /******************************************/
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < (MAX_ROW_FOR_CONTROL_ZDZ - additional_current))//Множення на два константи MAX_ROW_FOR_CONTROL_ZDZ потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки

        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ctrl_info[index_of_ekran_tmp].information[j];
        if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = ctrl_info[index_of_ekran_tmp].cursor_x;
      }
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
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
