#include "header.h"

/*****************************************************/
//Формуємо екран відображення уставок ТЗНП
/*****************************************************/
void make_ekran_setpoint_tznp(unsigned int group)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETPOINT_TZNP][MAX_COL_LCD] = 
  {
    {
      "3I0 ТЗНП1 Вперёд",
      "3U0 ТЗНП1 Вперёд",
      "3I0 ТЗНП1 Назад ",
      "3U0 ТЗНП1 Назад ",
      " Угол дов.ТЗНП1 ",
      "3I0 ТЗНП2 Вперёд",
      "3U0 ТЗНП2 Вперёд",
      "3I0 ТЗНП2 Назад ",
      "3U0 ТЗНП2 Назад ",
      " Угол дов.ТЗНП2 ",
      "3I0 ТЗНП3 Вперёд",
      "3U0 ТЗНП3 Вперёд",
      "3I0 ТЗНП3 Назад ",
      "3U0 ТЗНП3 Назад ",
      " Угол дов.ТЗНП3 "
    },
    {
      "3I0 СЗНП1 Вперед",
      "3U0 СЗНП1 Вперед",
      "3I0 СЗНП1 Назад ",
      "3U0 СЗНП1 Назад ",
      " Кут дов.СЗНП1  ",
      "3I0 СЗНП2 Вперед",
      "3U0 СЗНП2 Вперед",
      "3I0 СЗНП2 Назад ",
      "3U0 СЗНП2 Назад ",
      " Кут дов.СЗНП2  ",
      "3I0 СЗНП3 Вперед",
      "3U0 СЗНП3 Вперед",
      "3I0 СЗНП3 Назад ",
      "3U0 СЗНП3 Назад ",
      " Кут дов.СЗНП3  "
    },
    {
      "3I0 ТЗНП1 Вперёд",
      "3U0 ТЗНП1 Вперёд",
      "3I0 ТЗНП1 Назад ",
      "3U0 ТЗНП1 Назад ",
      " Угол дов.ТЗНП1 ",
      "3I0 ТЗНП2 Вперёд",
      "3U0 ТЗНП2 Вперёд",
      "3I0 ТЗНП2 Назад ",
      "3U0 ТЗНП2 Назад ",
      " Угол дов.ТЗНП2 ",
      "3I0 ТЗНП3 Вперёд",
      "3U0 ТЗНП3 Вперёд",
      "3I0 ТЗНП3 Назад ",
      "3U0 ТЗНП3 Назад ",
      " Угол дов.ТЗНП3 "
    },
    {
      "3I0 ТЗНП1 Вперёд",
      "3U0 ТЗНП1 Вперёд",
      "3I0 ТЗНП1 Назад ",
      "3U0 ТЗНП1 Назад ",
      " Угол дов.ТЗНП1 ",
      "3I0 ТЗНП2 Вперёд",
      "3U0 ТЗНП2 Вперёд",
      "3I0 ТЗНП2 Назад ",
      "3U0 ТЗНП2 Назад ",
      " Угол дов.ТЗНП2 ",
      "3I0 ТЗНП3 Вперёд",
      "3U0 ТЗНП3 Вперёд",
      "3I0 ТЗНП3 Назад ",
      "3U0 ТЗНП3 Назад ",
      " Угол дов.ТЗНП3 "
    },
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < MAX_ROW_FOR_SETPOINT_TZNP)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == INDEX_ML_STPTZNP1_3I0_VPERED)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_1_3I0_vpered[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_1_3I0_vpered[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPTZNP1_3U0_VPERED)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_1_3U0_vpered[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_1_3U0_vpered[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPTZNP1_3I0_NAZAD)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_1_3I0_nazad[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_1_3I0_nazad[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPTZNP1_3U0_NAZAD)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_1_3U0_nazad[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_1_3U0_nazad[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPTZNP1_ANGLE)
        {
          vaga = 10; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_1_angle[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_1_angle[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPTZNP2_3I0_VPERED)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_2_3I0_vpered[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_2_3I0_vpered[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPTZNP2_3U0_VPERED)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_2_3U0_vpered[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_2_3U0_vpered[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPTZNP2_3I0_NAZAD)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_2_3I0_nazad[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_2_3I0_nazad[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPTZNP2_3U0_NAZAD)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_2_3U0_nazad[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_2_3U0_nazad[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPTZNP2_ANGLE)
        {
          vaga = 10; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_2_angle[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_2_angle[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPTZNP3_3I0_VPERED)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_3_3I0_vpered[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_3_3I0_vpered[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPTZNP3_3U0_VPERED)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_3_3U0_vpered[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_3_3U0_vpered[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPTZNP3_3I0_NAZAD)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_3_3I0_nazad[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_3_3I0_nazad[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPTZNP3_3U0_NAZAD)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_3_3U0_nazad[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_3_3U0_nazad[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_STPTZNP3_ANGLE)
        {
          vaga = 10; //максимальний ваговий коефіцієнт для вилілення старшого розряду
          if (view == true) value = current_settings.setpoint_tznp_3_angle[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_tznp_3_angle[group];
        }
        
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_STPTZNP1_3I0_VPERED)
          {
            if (
                ((j < COL_SETPOINT_TZNP1_3I0_VPERED_BEGIN) ||  (j > COL_SETPOINT_TZNP1_3I0_VPERED_END ))  &&
                (j != (COL_SETPOINT_TZNP1_3I0_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_TZNP1_3I0_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_TZNP1_3I0_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_TZNP1_3I0_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPTZNP1_3U0_VPERED)
          {
            if (
                ((j < COL_SETPOINT_TZNP1_3U0_VPERED_BEGIN) ||  (j > COL_SETPOINT_TZNP1_3U0_VPERED_END ))  &&
                (j != (COL_SETPOINT_TZNP1_3U0_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_TZNP1_3U0_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_TZNP1_3U0_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_TZNP1_3U0_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPTZNP1_3I0_NAZAD)
          {
            if (
                ((j < COL_SETPOINT_TZNP1_3I0_NAZAD_BEGIN) ||  (j > COL_SETPOINT_TZNP1_3I0_NAZAD_END ))  &&
                (j != (COL_SETPOINT_TZNP1_3I0_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_TZNP1_3I0_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_TZNP1_3I0_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_TZNP1_3I0_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPTZNP1_3U0_NAZAD)
          {
            if (
                ((j < COL_SETPOINT_TZNP1_3U0_NAZAD_BEGIN) ||  (j > COL_SETPOINT_TZNP1_3U0_NAZAD_END ))  &&
                (j != (COL_SETPOINT_TZNP1_3U0_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_TZNP1_3U0_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_TZNP1_3U0_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_TZNP1_3U0_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPTZNP1_ANGLE)
          {
            if (
                ((j < COL_SETPOINT_TZNP1_ANGLE_BEGIN) ||  (j > COL_SETPOINT_TZNP1_ANGLE_END ))  &&
                (j != (COL_SETPOINT_TZNP1_ANGLE_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == (COL_SETPOINT_TZNP1_ANGLE_END + 2)) working_ekran[i][j] = '°';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPTZNP2_3I0_VPERED)
          {
            if (
                ((j < COL_SETPOINT_TZNP2_3I0_VPERED_BEGIN) ||  (j > COL_SETPOINT_TZNP2_3I0_VPERED_END ))  &&
                (j != (COL_SETPOINT_TZNP2_3I0_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_TZNP2_3I0_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_TZNP2_3I0_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_TZNP2_3I0_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPTZNP2_3U0_VPERED)
          {
            if (
                ((j < COL_SETPOINT_TZNP2_3U0_VPERED_BEGIN) ||  (j > COL_SETPOINT_TZNP2_3U0_VPERED_END ))  &&
                (j != (COL_SETPOINT_TZNP2_3U0_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_TZNP2_3U0_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_TZNP2_3U0_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_TZNP2_3U0_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPTZNP2_3I0_NAZAD)
          {
            if (
                ((j < COL_SETPOINT_TZNP2_3I0_NAZAD_BEGIN) ||  (j > COL_SETPOINT_TZNP2_3I0_NAZAD_END ))  &&
                (j != (COL_SETPOINT_TZNP2_3I0_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_TZNP2_3I0_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_TZNP2_3I0_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_TZNP2_3I0_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPTZNP2_3U0_NAZAD)
          {
            if (
                ((j < COL_SETPOINT_TZNP2_3U0_NAZAD_BEGIN) ||  (j > COL_SETPOINT_TZNP2_3U0_NAZAD_END ))  &&
                (j != (COL_SETPOINT_TZNP2_3U0_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_TZNP2_3U0_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_TZNP2_3U0_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_TZNP2_3U0_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPTZNP2_ANGLE)
          {
            if (
                ((j < COL_SETPOINT_TZNP2_ANGLE_BEGIN) ||  (j > COL_SETPOINT_TZNP2_ANGLE_END ))  &&
                (j != (COL_SETPOINT_TZNP2_ANGLE_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == (COL_SETPOINT_TZNP2_ANGLE_END + 2)) working_ekran[i][j] = '°';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPTZNP3_3I0_VPERED)
          {
            if (
                ((j < COL_SETPOINT_TZNP3_3I0_VPERED_BEGIN) ||  (j > COL_SETPOINT_TZNP3_3I0_VPERED_END ))  &&
                (j != (COL_SETPOINT_TZNP3_3I0_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_TZNP3_3I0_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_TZNP3_3I0_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_TZNP3_3I0_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPTZNP3_3U0_VPERED)
          {
            if (
                ((j < COL_SETPOINT_TZNP3_3U0_VPERED_BEGIN) ||  (j > COL_SETPOINT_TZNP3_3U0_VPERED_END ))  &&
                (j != (COL_SETPOINT_TZNP3_3U0_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_TZNP3_3U0_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_TZNP3_3U0_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_TZNP3_3U0_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPTZNP3_3I0_NAZAD)
          {
            if (
                ((j < COL_SETPOINT_TZNP3_3I0_NAZAD_BEGIN) ||  (j > COL_SETPOINT_TZNP3_3I0_NAZAD_END ))  &&
                (j != (COL_SETPOINT_TZNP3_3I0_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_TZNP3_3I0_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_TZNP3_3I0_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_TZNP3_3I0_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPTZNP3_3U0_NAZAD)
          {
            if (
                ((j < COL_SETPOINT_TZNP3_3U0_NAZAD_BEGIN) ||  (j > COL_SETPOINT_TZNP3_3U0_NAZAD_END ))  &&
                (j != (COL_SETPOINT_TZNP3_3U0_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_TZNP3_3U0_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_TZNP3_3U0_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_TZNP3_3U0_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_STPTZNP3_ANGLE)
          {
            if (
                ((j < COL_SETPOINT_TZNP3_ANGLE_BEGIN) ||  (j > COL_SETPOINT_TZNP3_ANGLE_END ))  &&
                (j != (COL_SETPOINT_TZNP3_ANGLE_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == (COL_SETPOINT_TZNP3_ANGLE_END + 2)) working_ekran[i][j] = '°';
            else
              calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, view);
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
    if (current_ekran.index_position == INDEX_ML_STPTZNP1_3I0_VPERED)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP1_3I0_VPERED_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP1_3I0_VPERED_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPTZNP1_3U0_VPERED)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP1_3U0_VPERED_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP1_3U0_VPERED_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPTZNP1_3I0_NAZAD)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP1_3I0_NAZAD_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP1_3I0_NAZAD_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPTZNP1_3U0_NAZAD)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP1_3U0_NAZAD_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP1_3U0_NAZAD_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPTZNP1_ANGLE)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP1_ANGLE_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP1_ANGLE_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPTZNP2_3I0_VPERED)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP2_3I0_VPERED_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP2_3I0_VPERED_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPTZNP2_3U0_VPERED)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP2_3U0_VPERED_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP2_3U0_VPERED_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPTZNP2_3I0_NAZAD)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP2_3I0_NAZAD_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP2_3I0_NAZAD_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPTZNP2_3U0_NAZAD)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP2_3U0_NAZAD_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP2_3U0_NAZAD_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPTZNP2_ANGLE)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP2_ANGLE_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP2_ANGLE_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPTZNP3_3I0_VPERED)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP3_3I0_VPERED_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP3_3I0_VPERED_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPTZNP3_3U0_VPERED)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP3_3U0_VPERED_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP3_3U0_VPERED_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPTZNP3_3I0_NAZAD)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP3_3I0_NAZAD_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP3_3I0_NAZAD_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPTZNP3_3U0_NAZAD)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP3_3U0_NAZAD_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP3_3U0_NAZAD_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPTZNP3_ANGLE)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_TZNP3_ANGLE_BEGIN;
      last_position_cursor_x = COL_SETPOINT_TZNP3_ANGLE_END;
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
//Формуємо екран відображення витримок ТЗНП
/*****************************************************/
void make_ekran_timeout_tznp(unsigned int group)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_TZNP][MAX_COL_LCD] = 
  {
    {
      "  ТЗНП1 Вперёд  ",
      "  ТЗНП1 Назад   ",
      "  ТЗНП2 Вперёд  ",
      "  ТЗНП2 Назад   ",
      "  ТЗНП3 Вперёд  ",
      "  ТЗНП3 Назад   "
    },
    {
      "  СЗНП1 Вперед  ",
      "  СЗНП1 Назад   ",
      "  СЗНП2 Вперед  ",
      "  СЗНП2 Назад   ",
      "  СЗНП3 Вперед  ",
      "  СЗНП3 Назад   ",
    },
    {
      "  ТЗНП1 Вперёд  ",
      "  ТЗНП1 Назад   ",
      "  ТЗНП2 Вперёд  ",
      "  ТЗНП2 Назад   ",
      "  ТЗНП3 Вперёд  ",
      "  ТЗНП3 Назад   "
    },
    {
      "  ТЗНП1 Вперёд  ",
      "  ТЗНП1 Назад   ",
      "  ТЗНП2 Вперёд  ",
      "  ТЗНП2 Назад   ",
      "  ТЗНП3 Вперёд  ",
      "  ТЗНП3 Назад   "
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
    if (index_of_ekran_tmp < MAX_ROW_FOR_TIMEOUT_TZNP)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
        if (index_of_ekran_tmp == INDEX_ML_TMOTZNP1_VPERED)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_tznp_1_vpered[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_tznp_1_vpered[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOTZNP1_NAZAD)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_tznp_1_nazad[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_tznp_1_nazad[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOTZNP2_VPERED)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_tznp_2_vpered[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_tznp_2_vpered[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOTZNP2_NAZAD)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_tznp_2_nazad[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_tznp_2_nazad[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOTZNP3_VPERED)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_tznp_3_vpered[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_tznp_3_vpered[group];
        }
        else if (index_of_ekran_tmp == INDEX_ML_TMOTZNP3_NAZAD)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (view == true) value = current_settings.timeout_tznp_3_nazad[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_tznp_3_nazad[group];
        }

        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if (index_of_ekran_tmp == INDEX_ML_TMOTZNP1_VPERED)
          {
            if (
                ((j < COL_TMO_TZNP1_VPERED_BEGIN) ||  (j > COL_TMO_TZNP1_VPERED_END )) &&
                (j != (COL_TMO_TZNP1_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_TZNP1_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_TZNP1_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_TZNP1_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOTZNP1_NAZAD)
          {
            if (
                ((j < COL_TMO_TZNP1_NAZAD_BEGIN) ||  (j > COL_TMO_TZNP1_NAZAD_END )) &&
                (j != (COL_TMO_TZNP1_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_TZNP1_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_TZNP1_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_TZNP1_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOTZNP2_VPERED)
          {
            if (
                ((j < COL_TMO_TZNP2_VPERED_BEGIN) ||  (j > COL_TMO_TZNP2_VPERED_END )) &&
                (j != (COL_TMO_TZNP2_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_TZNP2_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_TZNP2_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_TZNP2_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOTZNP2_NAZAD)
          {
            if (
                ((j < COL_TMO_TZNP2_NAZAD_BEGIN) ||  (j > COL_TMO_TZNP2_NAZAD_END )) &&
                (j != (COL_TMO_TZNP2_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_TZNP2_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_TZNP2_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_TZNP2_NAZAD_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOTZNP3_VPERED)
          {
            if (
                ((j < COL_TMO_TZNP3_VPERED_BEGIN) ||  (j > COL_TMO_TZNP3_VPERED_END )) &&
                (j != (COL_TMO_TZNP3_VPERED_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_TZNP3_VPERED_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_TZNP3_VPERED_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_TZNP3_VPERED_COMMA, view, 0);
          }
          else if (index_of_ekran_tmp == INDEX_ML_TMOTZNP3_NAZAD)
          {
            if (
                ((j < COL_TMO_TZNP3_NAZAD_BEGIN) ||  (j > COL_TMO_TZNP3_NAZAD_END )) &&
                (j != (COL_TMO_TZNP3_NAZAD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_TZNP3_NAZAD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_TZNP3_NAZAD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_TZNP3_NAZAD_COMMA, view, 0);
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
    if (current_ekran.index_position == INDEX_ML_TMOTZNP1_VPERED) 
    {
      current_ekran.position_cursor_x = COL_TMO_TZNP1_VPERED_BEGIN;
      last_position_cursor_x = COL_TMO_TZNP1_VPERED_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOTZNP1_NAZAD) 
    {
      current_ekran.position_cursor_x = COL_TMO_TZNP1_NAZAD_BEGIN;
      last_position_cursor_x = COL_TMO_TZNP1_NAZAD_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOTZNP2_VPERED) 
    {
      current_ekran.position_cursor_x = COL_TMO_TZNP2_VPERED_BEGIN;
      last_position_cursor_x = COL_TMO_TZNP2_VPERED_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOTZNP2_NAZAD) 
    {
      current_ekran.position_cursor_x = COL_TMO_TZNP2_NAZAD_BEGIN;
      last_position_cursor_x = COL_TMO_TZNP2_NAZAD_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOTZNP3_VPERED) 
    {
      current_ekran.position_cursor_x = COL_TMO_TZNP3_VPERED_BEGIN;
      last_position_cursor_x = COL_TMO_TZNP3_VPERED_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOTZNP3_NAZAD) 
    {
      current_ekran.position_cursor_x = COL_TMO_TZNP3_NAZAD_BEGIN;
      last_position_cursor_x = COL_TMO_TZNP3_NAZAD_END;
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
//Формуємо екран відображення значення управлінської інформації для ТЗНП
/*****************************************************/
void make_ekran_control_tznp()
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_TZNP][MAX_COL_LCD] = 
  {
    {
      "     ТЗНП1      ",
      "  ТЗНП1 Вперёд  ",
      "  ТЗНП1 Назад   ",
      "     ТЗНП2      ",
      "  ТЗНП2 Вперёд  ",
      "  ТЗНП2 Назад   ",
      "     ТЗНП3      ",
      "  ТЗНП3 Вперёд  ",
      "  ТЗНП3 Назад   "
    },
    {
      "     СЗНП1      ",
      "  СЗНП1 Вперед  ",
      "  СЗНП1 Назад   ",
      "     СЗНП2      ",
      "  СЗНП2 Вперед  ",
      "  СЗНП2 Назад   ",
      "     СЗНП3      ",
      "  СЗНП3 Вперед  ",
      "  СЗНП3 Назад   "
    },
    {
      "     ТЗНП1      ",
      "  ТЗНП1 Вперёд  ",
      "  ТЗНП1 Назад   ",
      "     ТЗНП2      ",
      "  ТЗНП2 Вперёд  ",
      "  ТЗНП2 Назад   ",
      "     ТЗНП3      ",
      "  ТЗНП3 Вперёд  ",
      "  ТЗНП3 Назад   "
    },
    {
      "     ТЗНП1      ",
      "  ТЗНП1 Вперёд  ",
      "  ТЗНП1 Назад   ",
      "     ТЗНП2      ",
      "  ТЗНП2 Вперёд  ",
      "  ТЗНП2 Назад   ",
      "     ТЗНП3      ",
      "  ТЗНП3 Вперёд  ",
      "  ТЗНП3 Назад   "
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
    if (index_of_ekran_tmp < MAX_ROW_FOR_CONTROL_TZNP)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
        {
          {"     Откл.      ", "      Вкл.      "},
          {"     Вимк.      ", "     Ввімк.     "},
          {"      Off       ", "       On       "},
          {"     Сљнд.      ", "     Косу.      "}
        };
        const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][2] = 
        {
         {4, 5},
         {4, 4},
         {5, 6},
         {4, 4}
        };

        unsigned int index_ctr = index_of_ekran_tmp;

        unsigned int temp_data;
          
        if(current_ekran.edition == 0) temp_data = current_settings.control_tznp;
        else temp_data = edition_settings.control_tznp;
          
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][(temp_data >> index_ctr) & 0x1][j];
        if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = cursor_x[index_language][(temp_data >> index_ctr) & 0x1];
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
