#include "header.h"

/*****************************************************/
//Вираховуваня цілого символу і поміщення його в робочий екран
/*****************************************************/
void calc_int_symbol_and_put_into_working_ekran(unsigned char* point_in_working_ekran, unsigned int* point_value, unsigned int* point_vaga, unsigned int* point_first_symbol)
{
  unsigned int temp_data;
  temp_data = (*point_value) / (*point_vaga); //виділяємо число, яке треба перетворити у символ і помістити у дану позицію екрану
  *point_value %= *(point_vaga); //вираховуємо число без символа, який ми зараз будемо виводити на екран
  *point_vaga /=10; //зменшуємо ваговий коефіцієнт в 10 разів
  if(current_ekran.edition != 0) *point_in_working_ekran = temp_data + 0x30;
  else
  {
    //У випадку, якщо ми не у режимі редагування, то нулі перед найстаршим значущим числом приховуємо
    if ((temp_data !=0) || ((*point_first_symbol) != 0))
    {
      *point_in_working_ekran = temp_data + 0x30;
      if ((*point_first_symbol) == 0) *point_first_symbol = 1;
    }
    else
    {
      //Нуль виводимо тільки у тому випадку, якщо це є символ одиниць числа (текуча вага числа рівна 1)
      if ((*point_vaga) >= 1 ) *point_in_working_ekran = ' ';
      else
      {
        *point_in_working_ekran = temp_data + 0x30;
        if ((*point_first_symbol) == 0) *point_first_symbol = 1;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення коефіцієнтів трансформації
/*****************************************************/
void make_ekran_transformator()
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TRANSFORMATOR_INFO][MAX_COL_LCD] = 
  {
    {
      "   К-т.тр.Т0    ",
      "   К-т.тр.ТТ    ",
      " К-т.тр.ТТ 0.4кВ",
      "   К-т.тр.ТН    "
    },
    {
      "   К-т.тр.Т0    ",
      "   К-т.тр.ТС    ",
      " К-т.тр.ТС 0.4кВ",
      "   К-т.тр.ТН    "
    },
    {
      "  3I0 T Ratio   ",
      " CT 0.4kV Ratio ",
      "    CT Ratio    ",
      "    VT Ratio    "
    },
    {
      "   К-т.тр.Т0    ",
      "   К-т.тр.ТТ    ",
      " К-т.тр.ТТ 0.4кВ",
      "   К-т.тр.ТН    "
    }
  };
  unsigned char name_string_tmp[MAX_ROW_FOR_TRANSFORMATOR_INFO][MAX_COL_LCD];

  int index_language = index_language_in_array(current_settings.language);
  for(int index_1 = 0; index_1 < MAX_ROW_FOR_TRANSFORMATOR_INFO; index_1++)
  {
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
  }

  /******************************************/
  //Виключаємо поля, які не треба відображати
  /******************************************/
  int additional_current = 0;
  int position_temp = current_ekran.index_position;
  int index_of_ekran;

  for (int current_index = 0; current_index < (MAX_ROW_FOR_TRANSFORMATOR_INFO - additional_current); current_index++ )
  {

    if (
        ((current_settings.control_extra_settings_1 & CTR_EXTRA_SETTINGS_1_CTRL_IB_I04) == 0)
        &&
        (current_ekran.index_position == INDEX_ML_TT04)
       )   
    {
      int i = current_index - additional_current;
      if ((i+1) <= position_temp) position_temp--;
      do
      {
        for(unsigned int j = 0; j < MAX_COL_LCD; j++)
        {
          if ((i+1) < (MAX_ROW_FOR_TRANSFORMATOR_INFO - additional_current)) name_string_tmp[i][j] = name_string_tmp[i + 1][j];
          else name_string_tmp[i][j] = ' ';
        }

        i++;
      }
      while (i< (MAX_ROW_FOR_TRANSFORMATOR_INFO - additional_current));
      additional_current++;
    }
  }
  /******************************************/
  
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < ((MAX_ROW_FOR_TRANSFORMATOR_INFO - additional_current)<<1))//Множення на два константи MAX_ROW_FOR_TRANSFORMATOR_INFO потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran>>1][j];
        if ((index_of_ekran>>1) == INDEX_ML_T0)
        {
          vaga = 100; //максимальний ваговий коефіцієнт для коефіцієнта трансформації T0
          if (current_ekran.edition == 0) value = current_settings.T0; //у змінну value поміщаємо значення коефіцієнта трансформації T0
          else value = edition_settings.T0;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TT)
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для коефіцієнта трансформації TT
          if (current_ekran.edition == 0) value = current_settings.TCurrent; //у змінну value поміщаємо значення коефіцієнта трансформації TT
          else value = edition_settings.TCurrent;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TT04)
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для коефіцієнта трансформації TT сторони 0.4кВ
          if (current_ekran.edition == 0) value = current_settings.TCurrent04; //у змінну value поміщаємо значення коефіцієнта трансформації TT сторони 0.4кВ
          else value = edition_settings.TCurrent04;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TN)
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для коефіцієнта трансформації TН
          if (current_ekran.edition == 0) value = current_settings.TVoltage; //у змінну value поміщаємо значення коефіцієнта трансформації TН
          else value = edition_settings.TVoltage;
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
      }
      else 
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((index_of_ekran>>1) == INDEX_ML_T0)
          {
            if ((j < COL_T0_BEGIN) ||  (j > COL_T0_END ))working_ekran[i][j] = ' ';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TT)
          {
            if ((j < COL_TT_BEGIN) ||  (j > COL_TT_END ))working_ekran[i][j] = ' ';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TT04)
          {
            if ((j < COL_TT04_BEGIN) ||  (j > COL_TT04_END ))working_ekran[i][j] = ' ';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TN)
          {
            if ((j < COL_TN_BEGIN) ||  (j > COL_TN_END ))working_ekran[i][j] = ' ';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol);
          }
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
    int last_position_cursor_x = MAX_COL_LCD;
    if (current_ekran.index_position == INDEX_ML_T0)
    {
      current_ekran.position_cursor_x = COL_T0_BEGIN;
      last_position_cursor_x = COL_T0_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TT)
    {
      current_ekran.position_cursor_x = COL_TT_BEGIN;
      last_position_cursor_x = COL_TT_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TT04)
    {
      current_ekran.position_cursor_x = COL_TT04_BEGIN;
      last_position_cursor_x = COL_TT04_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TN)
    {
      current_ekran.position_cursor_x = COL_TN_BEGIN;
      last_position_cursor_x = COL_TN_END;
    }

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
//
/*****************************************************/
/*****************************************************/
