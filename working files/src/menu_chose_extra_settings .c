#include "header.h"

/*****************************************************/
//Формуємо екран відображення заголовків настройок
/*****************************************************/
void make_ekran_chose_extra_settings(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_EXTRA_SETTINGS][MAX_COL_LCD] = 
  {
    {
      "    Ib/I0.4     ",
      " Вх.напряжения  ",
      "Бл.Гот.к ТУ от з",
      " Инф.об отключ. ",
      " Контр.акт.ФК   ",
      " Бл.Вкл.ВВ от ДВ",
      "Бл.Откл.ВВ от ДВ"
    },
    {
      "    Ib/I0.4     ",
      " Вхідні напруги ",
      "Бл.Гот.до ТУ в.з",
      "Інф.про вимкнен.",
      " Контр.акт.ФК   ",
      " Бл.Ввімк.від ДВ",
      " Бл.Вимк.від ДВ "
    },
    {
      "    Ib/I0.4     ",
      " Вх.напряжения  ",
      "Бл.Гот.к ТУ от з",
      " Инф.об отключ. ",
      " FK activ.Ctrl  ",
      " Blc.On CB fr.DI", 
      "Blc.Off CB fr.DI"
    },
    {
      "    Ib/I0.4     ",
      " Вх.напряжения  ",
      "Бл.Гот.к ТУ от з",
      " Инф.об отключ. ",
      " Контр.акт.ФК   ",
      " Бл.Вкл.ВВ от ДВ",
      "Бл.Откл.ВВ от ДВ"
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < MAX_ROW_FOR_CHOSE_EXTRA_SETTINGS)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо стан
        const unsigned char information[MAX_ROW_FOR_CHOSE_EXTRA_SETTINGS][MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
        {
          {
            {"       Ib       ", "      I0.4      "},
            {"       Ib       ", "      I0.4      "},
            {"       Ib       ", "      I0.4      "},
            {"       Ib       ", "      I0.4      "}
          },
          {
            {"     Фазные     ", "    Линейные    "},
            {"     Фазні      ", "    Лінійні     "},
            {"     Фазные     ", "    Линейные    "},
            {"     Фазные     ", "    Линейные    "}
          },
          {
            {"     Откл.      ", "      Вкл.      "},
            {"     Вимк.      ", "     Ввімк.     "},
            {"      Off       ", "       On       "},
            {"     Сљнд.      ", "     Косу.      "}
          },
          {
            {"     Откл.      ", "      Вкл.      "},
            {"     Вимк.      ", "     Ввімк.     "},
            {"      Off       ", "       On       "},
            {"     Сљнд.      ", "     Косу.      "}
          },
          {
            {"     Откл.      ", "      Вкл.      "},
            {"     Вимк.      ", "     Ввімк.     "},
            {"      Off       ", "       On       "},
            {"     Сљнд.      ", "     Косу.      "}
          },
          {
            {"     Откл.      ", "      Вкл.      "},
            {"     Вимк.      ", "     Ввімк.     "},
            {"      Off       ", "       On       "},
            {"     Сљнд.      ", "     Косу.      "}
          },
          {
            {"     Откл.      ", "      Вкл.      "},
            {"     Вимк.      ", "     Ввімк.     "},
            {"      Off       ", "       On       "},
            {"     Сљнд.      ", "     Косу.      "}
          }
        };
        const unsigned int cursor_x[MAX_ROW_FOR_CHOSE_EXTRA_SETTINGS][MAX_NAMBER_LANGUAGE][2] = 
        {
          {
            {6, 5},
            {6, 5},
            {6, 5},
            {6, 5}
          },
          {
            {4, 3},
            {4, 3},
            {4, 3},
            {4, 3}
          },
          {
            {4, 5},
            {4, 4},
            {5, 6},
            {4, 4}
          },
          {
            {4, 5},
            {4, 4},
            {5, 6},
            {4, 4}
          },
          {
            {4, 5},
            {4, 4},
            {5, 6},
            {4, 4}
          },
          {
            {4, 5},
            {4, 4},
            {5, 6},
            {4, 4}
          },
          {
            {4, 5},
            {4, 4},
            {5, 6},
            {4, 4}
          }
        };
        
        unsigned int index_ctr = index_of_ekran_tmp;

        unsigned int temp_data;
        if(current_ekran.edition == 0) temp_data = current_settings.control_extra_settings_1;
        else temp_data = edition_settings.control_extra_settings_1;
          
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_ctr][index_language][(temp_data >> index_ctr) & 0x1][j];
        if (position_temp == index_of_ekran_tmp)
        {
          current_ekran.position_cursor_x = cursor_x[index_ctr][index_language][(temp_data >> index_ctr) & 0x1];
        }
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
