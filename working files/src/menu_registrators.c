#include "header.h"

/*****************************************************/
//Формуємо екран відображення заголовків реєстраторів
/*****************************************************/
void make_ekran_list_registrators(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_LIST_REGISTRATORS][MAX_COL_LCD] = 
  {
    {
      " Аналог.рег-р   ",
      " Дискр. рег-р   ",
      " Архив диагност."
    },
    {
      " Аналог.реєстр. ",
      " Дискр. реєстр. ",
      " Архів діагност."
    },
    {
      " Analog recorder",
      "Digital recorder",
      " Diagn.Archive  "
    },
    {
      " Аналог.рег-р   ",
      " Дискр. рег-р   ",
      " Архив диагност."
    }
  };

  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_LIST_REGISTRATORS)
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

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
//Формуємо екран відображення записів дискретного реєстратора
/*****************************************************/
void make_ekran_list_records_registrator(unsigned int type_registrator)
{
  unsigned char name_string[MAX_ROW_FOR_LIST_REGISTRATORS_RECORDS][MAX_COL_LCD];
  for(int index_1 = 0; index_1 < MAX_ROW_LCD; index_1++)
  {
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++) name_string[index_1][index_2] = ' ';
  }

  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int number_records;
  
  if (type_registrator == INDEX_ML_ANALOG_REGISTRATOR_INFO) number_records = info_rejestrator_ar.number_records;
  else if (type_registrator == INDEX_ML_DIGITAL_REGISTRATOR_INFO) number_records = info_rejestrator_dr.number_records;
  else number_records = info_rejestrator_pr_err.number_records;
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  if(number_records == 0)
  {
    //Немає записів
    const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        "      Нет       ",
        "    записей     "
      },
      {
        "     Нема       ",
        "    записів     "
      },
      {
        "    Records     ",
        "   are absent   "
      },
      {
        "      Нет       ",
        "    записей     "
      }
    };

    for(int index_1 = 0; index_1 < MAX_ROW_LCD; index_1++)
    {
      for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        name_string[index_1][index_2] = (index_1 < 2) ? information[index_language][index_1][index_2] : ' ';
    }

    //Курсор не видимий
    current_ekran.cursor_on = 0;
  }
  else if (current_ekran.index_position < ((int)number_records))
  {
    unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
    {
      " Запись         ",
      " Запис          ",
      " Record         ",
      " Жазба          "
    };

    const unsigned int index_of_number[MAX_NAMBER_LANGUAGE]=  {8, 7, 8, 7};
    unsigned int index_first_symbol_in_number = index_of_number[index_language];
    unsigned int index_last_symbol_in_number = index_first_symbol_in_number + 4 - 1;
    
    unsigned int k = 0;
    while (((index_of_ekran + k) < number_records) && (k < MAX_ROW_FOR_LIST_REGISTRATORS_RECORDS))
    {
      //Очищаємо попереднє число
      for(unsigned int i = 0; i < 4; i++) information[index_language][index_first_symbol_in_number + i] = ' ';
      
      unsigned int temp_value = index_of_ekran + k;
      unsigned int index = 0;
      while (temp_value >= 10)
      {
        unsigned int value;
        value = temp_value % 10;
        temp_value /= 10;
        information[index_language][index_last_symbol_in_number - index] = value + 0x30;
        index++;
      }
      information[index_language][index_last_symbol_in_number - index] = temp_value + 0x30;
      
      //Підтягуємо число, щоб не було спереді багато пробілів
      unsigned int iteration = 0;
      while ((information[index_language][index_first_symbol_in_number] == ' ') && (iteration < 4))
      {
        for (unsigned int i = 0; i < (3 - iteration); i ++)
          information[index_language][index_first_symbol_in_number + i] = information[index_language][index_first_symbol_in_number + i + 1];
        
        information[index_language][index_first_symbol_in_number + (4 - 1) - iteration] = ' ';
        iteration++;
      }

      for (unsigned int i = 0; i < MAX_COL_LCD; i++) name_string[k][i] = information[index_language][i];
      k++;
    }

    //Курсор видимий
    current_ekran.cursor_on = 1;
  }
  else
  {
    //Зафіксована помилкова ситуація
    const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        "     Ошибка     ",
        "  отображения   "
      },
      {
        "    Помилка     ",
        "  відображення  "
      },
      {
        "    Display     ",
        "     error      "
      },
      {
        "     Ошибка     ",
        "  отображения   "
      }
    };

    for(int index_1 = 0; index_1 < MAX_ROW_LCD; index_1++)
    {
      for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        name_string[index_1][index_2] = (index_1 < 2) ? information[index_language][index_1][index_2] : ' ';
    }

    //Курсор не видимий
    current_ekran.cursor_on = 0;
  }
  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[i][j];
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення заголовків груп для запису дискретного реєстратора
/*****************************************************/
void make_ekran_list_titles_for_record_of_digital_registrator(void)
{
  int index_language = index_language_in_array(current_settings.language);

  if ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU) == 0)
  {
    //Процес зчитування даних з DataFlash вже закінчився
    const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR][MAX_COL_LCD] = 
    {
      {
        " Метка времени  ",
        " Изм.дискр.сигн.",
        " Изм.при м.фазе ",
        " Изм.при м.ф.0.4",
        " Изм.при 3I0-max",
        " Изм.при 3U0-max",
        " Изм.при U-min  ",
        " Изм.при U-max  ",
        " Изм.при I2-max ",
        " Изм.при f-min  ",
        " Изм.при f-ЧАПВ "
      },
      {
        " Мітка часу     ",
        " Зм.дискр.сигн. ",
        " Вим.при м.фазі ",
        " Вим.при м.ф.0.4",
        " Вим.при 3I0-max",
        " Вим.при 3U0-max",
        " Вим.при U-min  ",
        " Вим.при U-max  ",
        " Вим.при I2-max ",
        " Вим.при f-min  ",
        " Вим.при f-ЧАПВ "
      },
      {
        " Time label     ",
        " Digit.S.Changes",
        "M.at Max.Ph Cur ",
        "M.at Max.Ph 0.4 ",
        "M.at 3I0 Max.   ",
        " Изм.при 3U0-max",
        " Изм.при U-min  ",
        " Изм.при U-max  ",
        " M.at Max.I2 Cur",
        " Изм.при f-min  ",
        " Изм.при f-ЧАПВ "
      },
      {
        " Уакыт белгісі  ",
        " Изм.дискр.сигн.",
        " Изм.при м.фазе ",
        " Изм.при м.ф.0.4",
        " Изм.при 3I0-max",
        " Изм.при 3U0-max",
        " Изм.при U-min  ",
        " Изм.при U-max  ",
        " Изм.при I2-max ",
        " Изм.при f-min  ",
        " Изм.при f-ЧАПВ "
      }
    };

    unsigned int position_temp = current_ekran.index_position;
    unsigned int index_of_ekran;
  
    index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
    //Копіюємо  рядки у робочий екран
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (index_of_ekran < MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

      index_of_ekran++;
    }
  
    //Курсор видимий
    current_ekran.cursor_on = 1;
    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
    
    //Позначаємо, що більше цей екран перерисовувати не треба
    rewrite_ekran_once_more = 0;
  }
  else
  {
    //Процес зчитування даних з DataFlash ще не закінчився
    const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        " Процесс чтения ",
        "  не завершен   "
      },
      {
        " Процес читання ",
        " не завершений  "
      },
      {
        "    Reading     ",
        "is not completed"
      },
      {
        " Процесс чтения ",
        "  не завершен   "
      }
    };

    //Копіюємо  рядки у робочий екран
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (i < 2)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

    //Курсор не видимий
    current_ekran.cursor_on = 0;
    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;

    //Позначаємо, що цей екран треба перерисувати
    rewrite_ekran_once_more = 1;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення дати і часу запису реєстраторів
/*
0 - дискретний реєстратор
1 - реєстратор програмних подій
2 - аналоговий реєстратор
*/
/*****************************************************/
void make_ekran_data_and_time_of_records_registrator(unsigned int type_of_registrator)
{
  int index_language = index_language_in_array(current_settings.language);

  if (
      ((type_of_registrator == 0) && (buffer_for_manu_read_record[FIRST_INDEX_START_START_RECORD_DR] == LABEL_START_RECORD_DR    )) ||
      ((type_of_registrator == 1) && (buffer_for_manu_read_record[0] == LABEL_START_RECORD_PR_ERR)) ||
      ((type_of_registrator == 2) && (buffer_for_manu_read_record[0] == LABEL_START_RECORD_AR) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_MENU) == 0))
     )
  {
    //Пеший байт сходиться із міткою початку запису - вважаємо, що у буфері достовірні дані
    unsigned char name_string[MAX_ROW_FOR_EKRAN_DATA_LABEL][MAX_COL_LCD] = 
    {
      "   XX-XX-20XX   ",
      "   XX:XX:XX.XX  ",
    };
  
    unsigned int position_temp = current_ekran.index_position;
    unsigned int index_of_ekran;
  
    index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
    /******************************************/
    //Заповнюємо поля відповідними цифрами
    /******************************************/
    if (type_of_registrator == 2)
    {
      __HEADER_AR header_ar_tmp;
      /*
      У перших байтах зчитаного буферу є заголовок аналоговог ореєстратора.
      Для зручності на цю адресу ставим структуру заголовку аналогового реєстратора
      щоб легше було можливість читати поля
      */
      header_ar_tmp = *((__HEADER_AR*)buffer_for_manu_read_record);
      unsigned int field;
      
      //День
      field = header_ar_tmp.time[4];
      name_string[ROW_R_Y_][COL_DY1_R] = (field >>  4) + 0x30;
      name_string[ROW_R_Y_][COL_DY2_R] = (field & 0xf) + 0x30;

      //Місяць
      field = header_ar_tmp.time[5];
      name_string[ROW_R_Y_][COL_MY1_R] = (field >>  4) + 0x30;
      name_string[ROW_R_Y_][COL_MY2_R] = (field & 0xf) + 0x30;

      //Рік
      field = header_ar_tmp.time[6];
      name_string[ROW_R_Y_][COL_SY1_R] = (field >>  4) + 0x30;
      name_string[ROW_R_Y_][COL_SY2_R] = (field & 0xf) + 0x30;

      //Година
      field = header_ar_tmp.time[3];
      name_string[ROW_R_T_][COL_HT1_R] = (field >>  4) + 0x30;
      name_string[ROW_R_T_][COL_HT2_R] = (field & 0xf) + 0x30;

      //Хвилини
      field = header_ar_tmp.time[2];
      name_string[ROW_R_T_][COL_MT1_R] = (field >>  4) + 0x30;
      name_string[ROW_R_T_][COL_MT2_R] = (field & 0xf) + 0x30;

      //Секунди
      field = header_ar_tmp.time[1];
      name_string[ROW_R_T_][COL_ST1_R] = (field >>  4) + 0x30;
      name_string[ROW_R_T_][COL_ST2_R] = (field & 0xf) + 0x30;

      //Соті секунд
      field = header_ar_tmp.time[0];
      name_string[ROW_R_T_][COL_HST1_R] = (field >>  4) + 0x30;
      name_string[ROW_R_T_][COL_HST2_R] = (field & 0xf) + 0x30;

      //Позначаємо, що більше цей екран перерисовувати не треба
      rewrite_ekran_once_more = 0;
    }
    else
    {
      //День
      name_string[ROW_R_Y_][COL_DY1_R] = (buffer_for_manu_read_record[5] >>  4) + 0x30;
      name_string[ROW_R_Y_][COL_DY2_R] = (buffer_for_manu_read_record[5] & 0xf) + 0x30;

      //Місяць
      name_string[ROW_R_Y_][COL_MY1_R] = (buffer_for_manu_read_record[6] >>  4) + 0x30;
      name_string[ROW_R_Y_][COL_MY2_R] = (buffer_for_manu_read_record[6] & 0xf) + 0x30;

      //Рік
      name_string[ROW_R_Y_][COL_SY1_R] = (buffer_for_manu_read_record[7] >>  4) + 0x30;
      name_string[ROW_R_Y_][COL_SY2_R] = (buffer_for_manu_read_record[7] & 0xf) + 0x30;

      //Година
      name_string[ROW_R_T_][COL_HT1_R] = (buffer_for_manu_read_record[4] >>  4) + 0x30;
      name_string[ROW_R_T_][COL_HT2_R] = (buffer_for_manu_read_record[4] & 0xf) + 0x30;

      //Хвилини
      name_string[ROW_R_T_][COL_MT1_R] = (buffer_for_manu_read_record[3] >>  4) + 0x30;
      name_string[ROW_R_T_][COL_MT2_R] = (buffer_for_manu_read_record[3] & 0xf) + 0x30;

      //Секунди
      name_string[ROW_R_T_][COL_ST1_R] = (buffer_for_manu_read_record[2] >>  4) + 0x30;
      name_string[ROW_R_T_][COL_ST2_R] = (buffer_for_manu_read_record[2] & 0xf) + 0x30;

      //Соті секунд
      name_string[ROW_R_T_][COL_HST1_R] = (buffer_for_manu_read_record[1] >>  4) + 0x30;
      name_string[ROW_R_T_][COL_HST2_R] = (buffer_for_manu_read_record[1] & 0xf) + 0x30;
    }

    //Копіюємо  рядки у робочий екран
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (index_of_ekran < MAX_ROW_FOR_EKRAN_DATA_LABEL)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_of_ekran][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

      index_of_ekran++;
    }
  
    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  }
  else if ((type_of_registrator == 2) && ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_AR_MENU) != 0))
  {
    //Процес зчитування даних з DataFlash ще не закінчився
    const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        " Процесс чтения ",
        "  не завершен   "
      },
      {
        " Процес читання ",
        " не завершений  "
      },
      {
        "    Reading     ",
        "is not completed"
      },
      {
        " Процесс чтения ",
        "  не завершен   "
      }
    };

    //Копіюємо  рядки у робочий екран
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (i < 2)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;

    //Позначаємо, що цей екран треба перерисувати
    rewrite_ekran_once_more = 1;
  }
  else
  {
    //Пеший байт не сходиться із міткою початку запису - робимо висновок, що у біфері не достовірні дані
    const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        " Недостоверные  ",
        "     данные     "
      },
      {
        "  Недостовірні  ",
        "      дані      "
      },
      {
        "    Invalid     ",
        "      data      "
      },
      {
        " Недостоверные  ",
        "     данные     "
      }
    };
    
    if(type_of_registrator == 2)
    {
      //Позначаємо, що більше цей екран часу запису аналогового реєстратора перерисовувати не треба - бо виникла помилка з недостовірними даними
      rewrite_ekran_once_more = 0;
    }

    //Копіюємо  рядки у робочий екран
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (i < 2)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;
  }

  //Курсор не видимий
  current_ekran.cursor_on = 0;

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення діючих значень при фіксації максимальних/мінімальних вимірювань запису дискретного реєстратора
/*****************************************************/
void make_ekran_title_analog_value_records_digital_registrator(void)
{
  unsigned char name_string[MAX_ROW_FOR_TITLE_EKRAN_ANALOG_VALUES_DR][MAX_COL_LCD];
  for(int index_1 = 0; index_1 < MAX_ROW_LCD; index_1++)
  {
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++) name_string[index_1][index_2] = ' ';
  }
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int number_info_records;
  
  number_info_records = buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MAX_PHASE_DR + type_view_max_values_dr - IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE];
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  index_cell_into_array_for_min_max_measurement_dr = -1; //Помічаємо, що ми покищо не знайшли індекс комірки, який відповідає першому блоку, який визначений курсором
  if(number_info_records == 0)
  {
    const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        "      Нет       ",
        "    записей     "
      },
      {
        "     Нема       ",
        "    записів     "
      },
      {
        "    Records     ",
        "   are absent   "
      },
      {
        "      Нет       ",
        "    записей     "
      }
    };

    for(int index_1 = 0; index_1 < 2; index_1++)
    {
      for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        name_string[index_1][index_2] = information[index_language][index_1][index_2];
    }

    //Курсор не видимий
    current_ekran.cursor_on = 0;
  }
  else if (current_ekran.index_position < ((int)number_info_records))
  {
    unsigned int k = 0;
    while (((index_of_ekran + k) < number_info_records) && (k < MAX_ROW_FOR_TITLE_EKRAN_ANALOG_VALUES_DR))
    {
      //Шукаємо блок, який відповідає заданому типу струму
      unsigned int number_finded_this_type_of_current = 0;
      unsigned int index_cell_into_array = (FIRST_INDEX_FIRST_BLOCK_DR + (sizeof(unsigned int)*SIZE_ARRAY_FIX_MAX_MEASUREMENTS - 1));//Індекс комірки пам'яті встановлюємо на байт першого блоку, який відображає тип струму по якому йшов моніторинг
      while ((number_finded_this_type_of_current != (index_of_ekran + k + 1)) && (index_cell_into_array < FIRST_INDEX_FIRST_DATA_DR))
      {
        if (buffer_for_manu_read_record[index_cell_into_array] == type_view_max_values_dr)
          number_finded_this_type_of_current++;
        if (number_finded_this_type_of_current != (index_of_ekran + k + 1))
          index_cell_into_array += (sizeof(unsigned int)*SIZE_ARRAY_FIX_MAX_MEASUREMENTS);
      }
      
      unsigned char information[MAX_COL_LCD] = "                ";
      const unsigned char ms[MAX_NAMBER_LANGUAGE][3] = 
      {
        "мс.",
        "мс.",
        "ms.",
        "мс."
      };
      if ((number_finded_this_type_of_current == (index_of_ekran + k + 1)) && (index_cell_into_array < FIRST_INDEX_FIRST_DATA_DR))
      {
        //Ми знайшли потрібний блок
        //Якщо у цьому рядку ще і знаходиться курсор, то треба запам'ятати індекс першого елементу найденого блоку
        if (position_temp  == (index_of_ekran + k))
          index_cell_into_array_for_min_max_measurement_dr = index_cell_into_array - (sizeof(unsigned int)*SIZE_ARRAY_FIX_MAX_MEASUREMENTS - 1);

        //Визначаємо час зрізу
        index_cell_into_array -= 3; //Переводимо індекс на початок часової мітки
        unsigned char sring_of_time[8] = "        ";
        unsigned int time_of_slice = buffer_for_manu_read_record[index_cell_into_array] + (buffer_for_manu_read_record[index_cell_into_array + 1]<<8) + (buffer_for_manu_read_record[index_cell_into_array + 2]<<16);

        //Конвертуємо цей час у рядок
        unsigned int ost, local_index = 0;
        while ((time_of_slice >= 10) && (local_index < (8 - 1)))
        {
          ost = time_of_slice % 10;
          sring_of_time[7-local_index] = ost + 0x30;
          time_of_slice /= 10;
          local_index++;
        }
        sring_of_time[7-local_index] =  time_of_slice + 0x30;

        //Копіюємо цей рідок часу у наш робочий рядок
        local_index = 0;
        while ((sring_of_time[local_index] == ' ') && (local_index < 8)) local_index++;
        unsigned int local_index1 = 1;
        while (local_index < 8) information[local_index1++] = sring_of_time[local_index++];
        information[local_index1++] = ' ';
        information[local_index1++] = ms[index_language][0];
        information[local_index1++] = ms[index_language][1];
        information[local_index1++] = ms[index_language][2];
      }
      else
      {
        //теоретично ми сюди б не мали ніколи заходити, але якщо ми сюди зайшли, то виводимо сигналізацію про помилку на екран
        const unsigned char error_meas[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
        {
          " Ошибка         ",
          " Помилка        ",
          " Error          ",
          " Ошибка         "
        };
        for (unsigned int i = 0; i < MAX_COL_LCD; i++) information[i] = error_meas[index_language][i];
      }

      for (unsigned int i = 0; i < MAX_COL_LCD; i++) name_string[k][i] = information[i];
      k++;
    }

    //Курсор видимий
    current_ekran.cursor_on = 1;
  }
  else
  {
    //Зафіксована помилкова ситуація
    const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        "     Ошибка     ",
        "  отображения   "
      },
      {
        "    Помилка     ",
        "  відображення  "
      },
      {
        "    Display     ",
        "     error      "
      },
      {
        "     Ошибка     ",
        "  отображения   "
      }
    };

    for(int index_1 = 0; index_1 < 2; index_1++)
    {
      for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        name_string[index_1][index_2] = information[index_language][index_1][index_2];
    }

    //Курсор не видимий
    current_ekran.cursor_on = 0;
  }
  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[i][j];
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення діючих значень при фіксації максимального струму 3I0/фази/ЗОП(КОФ) запису дискретного реєстратора
/*****************************************************/
void make_ekran_analog_value_records_digital_registrator(void)
{
  int index_language = index_language_in_array(current_settings.language);

  if (buffer_for_manu_read_record[FIRST_INDEX_START_START_RECORD_DR] == LABEL_START_RECORD_DR)
  {
    //Пеший байт сходиться із міткою початку запису - вважаємо, що у буфері достовірні дані
    unsigned char name_string[MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR][MAX_COL_LCD] = 
    {
      " 3I0  =         ",
      " 3I0**=         ",
      " 3I0-1=         ",
      " Ia   =         ",
      " Ib   =         ",
      " Ic   =         ",
      " I2   =         ",
      " I1   =         ",
      " I0.4 =         ",
      " Ua   =         ",
      " Ub   =         ",
      " Uc   =         ",
      " 3U0  =         ",
      " U2   =         ",
      " U1   =         ",
      " Uab  =         ",
      " Ubc  =         ",
      " Uca  =         ",
      " f  =           ",
      " Rab            ",
      " Xab            ",
      " Rbc            ",
      " Xbc            ",
      " Rca            ",
      " Xca            ",
      "                "
    };
    unsigned char *point_unsigned_char = (unsigned char *)(buffer_for_manu_read_record + index_cell_into_array_for_min_max_measurement_dr);
    unsigned int *point_unsigned_int = (unsigned int*)point_unsigned_char;

    for (unsigned int i = 0; i < MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR; i++)
    {
      if (i < 18)
      {
       //Струми і напруги
//       if (i == 2)
//       {
//         if (index_language == INDEX_LANGUAGE_EN) name_string[i][4] = 'c';
//         else name_string[i][4] = 'р';
//       }

        unsigned int temp_measurement = *(point_unsigned_int + i);
        unsigned int start_number_digit_after_point;
        if ((i == 0) || (i == 1)) start_number_digit_after_point = 2;
        else start_number_digit_after_point = 3;
        convert_and_insert_char_for_measurement(start_number_digit_after_point, temp_measurement, 1, 1, name_string[i], 7);
      }
      else if (i == 18)
      {
        //Частота
        int temp_measurement = *(((int *)point_unsigned_int) + i);
        if (temp_measurement < 0)
        {
          if (temp_measurement == (-2*1000))
          {
            /*Частота нижче порогу визначеного константою MIN_FREQUENCY*/
            name_string[i][4] = '<';
            temp_measurement = MIN_FREQUENCY*1000;
          }
          if (temp_measurement == (-3*1000))
          {
            /*Частота вище порогу визначеного константою MAX_FREQUENCY*/
            name_string[i][4] = '>';
            temp_measurement = MAX_FREQUENCY*1000;
          }       
        }
        convert_and_insert_char_for_frequency(temp_measurement, name_string[i]);
      }
      else if (i < 25)
      {
       //Опори
        const unsigned int index_of_start_position_array[MAX_NAMBER_LANGUAGE] = {4, 4, 5, 4};

#define SIZE_R_DIMENSION    2
        const unsigned int size_dimension_array[MAX_NAMBER_LANGUAGE] = {SIZE_R_DIMENSION, SIZE_R_DIMENSION, SIZE_R_DIMENSION - 1, SIZE_R_DIMENSION};
        const unsigned char resistance_dimension[MAX_NAMBER_LANGUAGE][SIZE_R_DIMENSION] = {"Ом", "Ом", "Ї ", "Ом"}; /*Ї тут іде як замінник великої букви Омега для англійської розкладки*/
  
        unsigned int start_position = index_of_start_position_array[index_language];
        unsigned int size_dimension = size_dimension_array[index_language];
        for (unsigned int j = 0; j < size_dimension; j++)
        {
          name_string[i][MAX_COL_LCD - size_dimension + j] = resistance_dimension[index_language][j];
        }
        name_string[i][start_position] = '=';

#undef SIZE_R_DIMENSION
        start_position++;

        int temp_measurement = *(point_unsigned_int + i);
        if(((unsigned int)temp_measurement) != ((unsigned int)UNDEF_RESISTANCE))
        {
          /********************************/
          //Вводимо вимірювальні значення
          /********************************/
          if (temp_measurement < 0)
          {
            temp_measurement = -temp_measurement;
            name_string[i][start_position] = '-';
          }
          convert_and_insert_char_for_measurement(3, temp_measurement, 1, 1, name_string[i], (start_position + 1));

          unsigned int shift = 0;
          unsigned int start_position_to_shift = start_position + 1;
          while (
                 (name_string[i][start_position_to_shift] == ' ') &&
                 ((start_position_to_shift + shift) < MAX_COL_LCD)  
                ) 
          {
            for (unsigned int j = start_position_to_shift; j < (MAX_COL_LCD - 1); j++ ) name_string[i][j] = name_string[i][j + 1];
            name_string[i][MAX_COL_LCD - 1] = ' ';
            shift++;
          }
          /********************************/
        }
        else
        {
#define SIZE_UNDEF      9
          const unsigned char undefined[MAX_NAMBER_LANGUAGE][SIZE_UNDEF] =
          {
            "Неопред. ",
            "Невизнач.",
            "Undef.   ",
            "Неопред. "  
          };
          for (unsigned int j = 0; j < size_dimension; j++) name_string[i][MAX_COL_LCD - size_dimension + j] = ' ';
          for (unsigned int j = 0; j < SIZE_UNDEF; j++) name_string[i][start_position + 1 + j] = undefined[index_language][j];
#undef SIZE_UNDEF
        }
      }
      else if ((i == 25) && (type_view_max_values_dr == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE))
      {
        //Місце пошкодження
#define SIZE_NAME_FIELD         2
        const unsigned char name_field[MAX_NAMBER_LANGUAGE][SIZE_NAME_FIELD] = {"МП", "МП", "FP", "МП"};
        for (unsigned int j = 0; j < SIZE_NAME_FIELD; j++)
        {
          name_string[i][1 + j] = name_field[index_language][j];
        }
#undef SIZE_NAME_FIELD

#define INDEX_LESS_EQUAL_MORE   4
#define SIZE_L_DIMENSION        2

        const unsigned char km[MAX_NAMBER_LANGUAGE][SIZE_L_DIMENSION] = {"км", "км", "km", "км"};
  
        for (unsigned int j = 0; j < SIZE_L_DIMENSION; j++)
        {
          name_string[i][INDEX_LESS_EQUAL_MORE + 2 + 7 + j] = km[index_language][j];
        }

        int temp_measurement_1 = *(point_unsigned_int + i);
        int temp_measurement_2 = *(point_unsigned_int + i + 1);
        if(((unsigned int)temp_measurement_1) != ((unsigned int)UNDEF_VMP))
        {
          if (temp_measurement_2 == true) name_string[i][INDEX_LESS_EQUAL_MORE] = '=';
          else name_string[i][INDEX_LESS_EQUAL_MORE] = '>';
          /********************************/
          //Вводимо вимірювальні значення
          /********************************/
          if (temp_measurement_1 < 0)
          {
            temp_measurement_1 = -temp_measurement_1;
            name_string[i][INDEX_LESS_EQUAL_MORE + 1] = '-';
          }
          convert_and_insert_char_for_measurement(3, temp_measurement_1, 1, 1, name_string[i], (INDEX_LESS_EQUAL_MORE + 2));
          
          //Є можливісьт між числом і розмірністю поставити один пробіл
          for (unsigned int j = 0; j < SIZE_L_DIMENSION; j++)
          {
            name_string[i][MAX_COL_LCD - 1 - j] = name_string[i][MAX_COL_LCD - 1 - j - 1];
          }
          name_string[i][MAX_COL_LCD - 1 - SIZE_L_DIMENSION] = ' ';

          unsigned int shift = 0;
          unsigned int start_position_to_shift = INDEX_LESS_EQUAL_MORE + 1 + 1;
          while (
                 (name_string[i][start_position_to_shift] == ' ') &&
                 ((start_position_to_shift + shift) < MAX_COL_LCD)  
                ) 
          {
            for (unsigned int j = start_position_to_shift; j < (MAX_COL_LCD - 1); j++ ) name_string[i][j] = name_string[i][j + 1];
            name_string[i][MAX_COL_LCD - 1] = ' ';
            shift++;
          }
          /********************************/
        }
        else
        {
          name_string[i][INDEX_LESS_EQUAL_MORE] = '=';
          
#define SIZE_UNDEF      9
          const unsigned char undefined[MAX_NAMBER_LANGUAGE][SIZE_UNDEF] =
          {
            "Неопред. ",
            "Невизнач.",
            "Undef.   ",
            "Неопред. "  
          };
          for (unsigned int j = 0; j < SIZE_L_DIMENSION; j++) name_string[i][INDEX_LESS_EQUAL_MORE + 2 + 7 + j] = ' ';
          for (unsigned int j = 0; j < SIZE_UNDEF; j++) name_string[i][INDEX_LESS_EQUAL_MORE + 2 + j] = undefined[index_language][j];
#undef SIZE_UNDEF
        }
#undef SIZE_L_DIMENSION
#undef INDEX_LESS_EQUAL_MORE
      }
      
      if (i < 9)
        name_string[i][MAX_COL_LCD - 1] = odynyci_vymirjuvannja[index_language][INDEX_A];
      else if (i < 18)
        name_string[i][MAX_COL_LCD - 1] = odynyci_vymirjuvannja[index_language][INDEX_V];
      else
      {
        //Герци і оми ми вже вивели під час відображення значення
      }
    }
  
    int position_temp = current_ekran.index_position;
    unsigned int index_of_ekran;

    /******************************************/
    //Виключаємо поля, які не треба відображати
    /******************************************/
    int additional_current = 0;
    
    {
      int shift_ind;
      
      if ((control_extra_settings_1_dr_for_manu & CTR_EXTRA_SETTINGS_1_CTRL_IB_I04) == 0) shift_ind = 8 - additional_current;
      else shift_ind = 4 - additional_current; 
      
      if ((shift_ind + 1) <= position_temp) position_temp--;
      do  
      {
        for(unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((shift_ind + 1) < (MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR - additional_current)) name_string[shift_ind][j] = name_string[shift_ind + 1][j];
          else name_string[shift_ind][j] = ' ';
        }
        shift_ind++;
      }
      while (shift_ind < (MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR - additional_current));
      additional_current++;
    }

    if ((control_extra_settings_1_dr_for_manu & CTR_EXTRA_SETTINGS_1_CTRL_PHASE_LINE) != 0)
    {
      int shift_ind_min = 9;
      int shift_ind_max = 11;
      
      for (int i = 0; i <= (shift_ind_max - shift_ind_min); i++)
      {
        int shift_ind = shift_ind_min - additional_current + i;
    
        if ((shift_ind_max - additional_current + 1) <= position_temp) position_temp--;
        do  
        {
          for(unsigned int j = 0; j<MAX_COL_LCD; j++)
          {
            if ((shift_ind + 1) < (MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR - additional_current)) name_string[shift_ind][j] = name_string[shift_ind + 1][j];
            else name_string[shift_ind][j] = ' ';
          }
          shift_ind++;
        }
        while (shift_ind < (MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR - additional_current));
        additional_current++;
      }
    }

    if (type_view_max_values_dr != IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE)
    {
      int shift_ind = 25 - additional_current;
      if ((shift_ind + 1) <= position_temp) position_temp--;
      do  
      {
        for(unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((shift_ind + 1) < (MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR - additional_current)) name_string[shift_ind][j] = name_string[shift_ind + 1][j];
          else name_string[shift_ind][j] = ' ';
        }
        shift_ind++;
      }
      while (shift_ind < (MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR - additional_current));
      additional_current++;
    }
    /******************************************/
    
    index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
    //Копіюємо  рядки у робочий екран
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (((int)index_of_ekran) < (MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR - additional_current))
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_of_ekran][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

      index_of_ekran++;
    }
  
    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
    //Курсор видимий
    current_ekran.cursor_on = 1;
  }
  else
  {
    //Пеший байт не сходиться із міткою початку запису - робимо висновок, що у біфері не достовірні дані
    const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        " Недостоверные  ",
        "     данные     "
      },
      {
        "  Недостовірні  ",
        "      дані      "
      },
      {
        "    Invalid     ",
        "      data      "
      },
      {
        " Недостоверные  ",
        "     данные     "
      }
    };

    //Копіюємо  рядки у робочий екран
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (i < 2)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;
    //Курсор не видимий
    current_ekran.cursor_on = 0;
  }


  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення змін дискретних сигналів з запису дискретного реєстратора
/*****************************************************/
void make_ekran_changing_signals_digital_registrator(void)
{
  int index_language = index_language_in_array(current_settings.language);

  if (buffer_for_manu_read_record[FIRST_INDEX_START_START_RECORD_DR] == LABEL_START_RECORD_DR)
  {
    //Пеший байт сходиться із міткою початку запису - вважаємо, що у буфері достовірні дані
    const unsigned char name_string[MAX_NAMBER_LANGUAGE][NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG + 3][MAX_COL_LCD] = 
    {
      {NAME_RANG_RU},
      {NAME_RANG_UA},
      {NAME_RANG_EN},
      {NAME_RANG_KZ},
    };
    const uint32_t index_number_UP[MAX_NAMBER_LANGUAGE][3] = 
    {
      {11, 10, 8}, 
      {11, 10, 8}, 
      {12,  7, 8}, 
      {11, 10, 8}, 
    };
    uint8_t name_string_tmp[NUMBER_TOTAL_SIGNAL_FOR_RANG][MAX_COL_LCD];
    for(int index_1 = 0; index_1 < NUMBER_TOTAL_SIGNAL_FOR_RANG; index_1++)
    {
      unsigned int index_row;
      if (index_1 < (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG)) 
      {
        index_row = index_1;
      }
      else if (index_1 < (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG))
      {
        index_row = (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG) + ((index_1 - (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG)) % 3);
      }
      else
      {
        index_row = index_1 - NUMBER_UP_SIGNAL_FOR_RANG + 3;
      }
      
      for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      {
        if (
            (index_1 >= (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG))  &&
            (index_1 <  (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG)) &&
            (index_2 == index_number_UP[index_language][(index_1 - (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG)) % 3]) 
           )
        {
          name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - (NUMBER_TOTAL_SIGNAL_FOR_RANG - NUMBER_EL_SIGNAL_FOR_RANG - NUMBER_VMP_SIGNAL_FOR_RANG - NUMBER_UP_SIGNAL_FOR_RANG)) / 3 + 1);
        }
        else name_string_tmp[index_1][index_2] = name_string[index_language][index_row][index_2];
      }
    }
  
    unsigned int max_number_changers_in_record = buffer_for_manu_read_record[FIRST_INDEX_NUMBER_CHANGES_DR] | (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_CHANGES_DR + 1]<<8);
    unsigned int position_temp;
    unsigned int index_of_ekran;
    unsigned int array_old[N_BIG], array_new[N_BIG], array_changing[N_BIG];
    
    //Перевіряємо, чи ми не вийшли за границі
    if (current_ekran.index_position < 0) current_ekran.index_position = max_number_changers_in_record - 1;
    else if (current_ekran.index_position >= ((int)max_number_changers_in_record)) current_ekran.index_position = 0;
    current_ekran.index_position = (current_ekran.index_position >> (POWER_MAX_ROW_LCD - 1)) << (POWER_MAX_ROW_LCD - 1);
    position_in_current_level_menu[EKRAN_CHANGES_SIGNALS_DR] = current_ekran.index_position;

    position_temp = current_ekran.index_position;
    index_of_ekran = position_temp & (unsigned int)(~((1<<(POWER_MAX_ROW_LCD>>1)) - 1));
  
    //Копіюємо  рядки у робочий екран
    for (unsigned int i=0; i< (MAX_ROW_LCD>>1); i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
      if (index_of_ekran < max_number_changers_in_record)
      {
        //Шукаємо масиви зрізів: попередній до вибраної зміни і у момент даної зміни
        int index_of_the_slice = 0; //починаємо з першого зрізу
        unsigned int current_number_changes = 0;
        while (
               ((current_number_changes + buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice) + (38 - 2)] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice) + (38 - 1)] << 8)) < (index_of_ekran + 1)) &&
               (index_of_the_slice < MAX_EVENTS_IN_ONE_RECORD)  
              )
        {
          current_number_changes += (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice) + (38 - 2)] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice) + (38 - 1)] << 8));
          index_of_the_slice++;
        }
        array_new[0] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) +  3] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) +  4]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) +  5]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) +  6]<<24);
        array_new[1] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) +  7] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) +  8]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) +  9]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 10]<<24);
        array_new[2] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 11] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 12]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 13]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 14]<<24);
        array_new[3] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 15] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 16]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 17]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 18]<<24);
        array_new[4] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 19] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 20]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 21]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 22]<<24);
        array_new[5] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 23] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 24]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 25]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 26]<<24);
        array_new[6] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 27] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 28]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 29]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 30]<<24);
        array_new[7] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 31] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 32]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 33]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 34]<<24);
        array_new[8] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice    ) + 35];

        array_old[0] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) +  3] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) +  4]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) +  5]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) +  6]<<24);
        array_old[1] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) +  7] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) +  8]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) +  9]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 10]<<24);
        array_old[2] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 11] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 12]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 13]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 14]<<24);
        array_old[3] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 15] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 16]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 17]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 18]<<24);
        array_old[4] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 19] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 20]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 21]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 22]<<24);
        array_old[5] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 23] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 24]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 25]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 26]<<24);
        array_old[6] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 27] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 28]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 29]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 30]<<24);
        array_old[7] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 31] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 32]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 33]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 34]<<24);
        array_old[8] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice - 1) + 35];
        
        //Визначаємо, які сигнали змінилися
        for (unsigned int j = 0; j < N_BIG; j++) array_changing[j] = array_new[j] ^ array_old[j];

        //Шукаємо функцію, яку треба зараз відобразити
        int index_of_function_in_the_slice = 0; //починаємо з першого зрізу
        do
        {
          if ((array_changing[index_of_function_in_the_slice >> 5] & (1 << (index_of_function_in_the_slice & ((1<<5)-1)))) != 0)
            current_number_changes++;
          if (current_number_changes  < (index_of_ekran + 1)) index_of_function_in_the_slice++;
        }
        while (
               (current_number_changes  < (index_of_ekran + 1)) &&
               (index_of_function_in_the_slice < NUMBER_TOTAL_SIGNAL_FOR_RANG)  
              );
        
        
        if ((index_of_the_slice < MAX_EVENTS_IN_ONE_RECORD) && (index_of_function_in_the_slice < NUMBER_TOTAL_SIGNAL_FOR_RANG) )
        {
          for (unsigned int k = 0; k < 2; k++)
          {
            if (((i<<1)+k) < MAX_ROW_LCD)
            {
              if (k == 0)
              {
                //У першому рядку відображаємо назву сигналу, який змінився
                for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[(i<<1)+k][j] = name_string_tmp[index_of_function_in_the_slice][j];
              }
              else
              {
                //У другому рядку відображаємо час зміни і яке значення прийняв зараз сигнал
                unsigned char second_row[MAX_COL_LCD] = "                ";
                unsigned char sring_of_time[8] = "        ";
                const unsigned char ms[MAX_NAMBER_LANGUAGE][3] = 
                {
                  "мс.",
                  "мс.",
                  "ms.",
                  "мс."
                };
                const unsigned char passive_active[MAX_NAMBER_LANGUAGE][2][5] = 
                {
                  {"Пасс.", "Акт. "},
                  {"Пас. ", "Акт. "},
                  {"Pass.", "Act. "},
                  {"Пасс.", "Акт. "}
                };
                unsigned int time_of_slice = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice) + 0] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice) + 1]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + 38*(1 + index_of_the_slice) + 2]<<16);
                
                //Конвертуємо цей час у рядок
                unsigned int ost, local_index = 0;
                while ((time_of_slice >= 10) && (local_index < (8 - 1)))
                {
                  ost = time_of_slice % 10;
                  sring_of_time[7-local_index] = ost + 0x30;
                  time_of_slice /= 10;
                  local_index++;
                }
                sring_of_time[7-local_index] =  time_of_slice + 0x30;
                
                //Копіюємо цей рідок часу у наш робочий рядок
                local_index = 0;
                while ((sring_of_time[local_index] == ' ') && (local_index < 8)) local_index++;
                unsigned int local_index1 = 0;
                while (local_index < 8) second_row[local_index1++] = sring_of_time[local_index++];
                if (local_index1 < 8) second_row[local_index1++] = ' ';
                second_row[local_index1++] = ms[index_language][0];
                second_row[local_index1++] = ms[index_language][1];
                second_row[local_index1++] = ms[index_language][2];
                
                //Визначаємо стан даного дискретного сигналу
                if ((array_new[index_of_function_in_the_slice >> 5] & (1 << (index_of_function_in_the_slice & ((1<<5)-1)))) != 0)
                {
                  //Сигнал перейшов у активний стан
                  for (local_index1 = 11; local_index1 < MAX_COL_LCD; local_index1++)
                    second_row[local_index1] = passive_active[index_language][1][local_index1 - 11];
                }
                else
                {
                  //Сигнал перейшов у пасивний стан
                  for (local_index1 = 11; local_index1 < MAX_COL_LCD; local_index1++)
                    second_row[local_index1] = passive_active[index_language][0][local_index1 - 11];
                }

                for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[(i<<1)+k][j] = second_row[j];
              }
            }
          }
        }
        else
        {
          unsigned char error_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
          {
            {
              " Неопределенная ",
              " ошибка отображ."
            },
            {
              "  Невизначена   ",
              " помилка відобр."
            },
            {
              "   Undefined    ",
              " display error  "
            },
            {
              " Неопределенная ",
              " ошибка отображ."
            }
          };
          for (unsigned int k = 0; k < 2; k++)
          {
            if (((i<<1)+k) < MAX_ROW_LCD)
              for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[(i<<1)+k][j] = error_string[index_language][k][j];
          }
        }
      }
      else
      {
        for (unsigned int k = 0; k < 2; k++)
        {
          if (((i<<1)+k) < MAX_ROW_LCD)
            for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[(i<<1)+k][j] = ' ';
        }
      }

      index_of_ekran++;
    }
  
    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = (position_temp<<1) & (MAX_ROW_LCD - 1);
    //Курсор невидимий
    current_ekran.cursor_on = 0;
  }
  else
  {
    //Пеший байт не сходиться із міткою початку запису - робимо висновок, що у біфері не достовірні дані
    const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        " Недостоверные  ",
        "     данные     "
      },
      {
        "  Недостовірні  ",
        "      дані      "
      },
      {
        "    Invalid     ",
        "      data      "
      },
      {
        " Недостоверные  ",
        "     данные     "
      }
    };

    //Копіюємо  рядки у робочий екран
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (i < 2)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;
    //Курсор не видимий
    current_ekran.cursor_on = 0;
    
    current_ekran.index_position = 0;
  }


  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення заголовків груп для запису реєстратора програмних подій
/*****************************************************/
void make_ekran_list_titles_for_record_of_pr_err_registrator(void)
{
  int index_language = index_language_in_array(current_settings.language);

  if ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU) == 0)
  {
    //Процес зчитування даних з DataFlash вже закінчився
    const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TITLES_PR_ERR_REGISTRATOR][MAX_COL_LCD] = 
    {
      {
        " Метка времени  ",
        " Изм.диагностики"
      },
      {
        " Мітка часу     ",
        " Зм.діагностики "
      },
      {
        " Time label     ",
        " Diagn.Changes  "
      },
      {
        " Уакыт белгісі  ",
        " Изм.диагностики"
      }
    };
  
    unsigned int position_temp = current_ekran.index_position;
    unsigned int index_of_ekran;
  
    index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
    //Копіюємо  рядки у робочий екран
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (index_of_ekran < MAX_ROW_FOR_TITLES_PR_ERR_REGISTRATOR)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

      index_of_ekran++;
    }
  
    //Курсор видимий
    current_ekran.cursor_on = 1;
    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
    
    //Позначаємо, що більше цей екран перерисовувати не треба
    rewrite_ekran_once_more = 0;
  }
  else
  {
    //Процес зчитування даних з DataFlash вже закінчився
    const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        " Процесс чтения ",
        "  не завершен   "
      },
      {
        " Процес читання ",
        " не завершений  "
      },
      {
        "    Reading     ",
        "is not completed"
      },
      {
        " Процесс чтения ",
        "  не завершен   "
      }
    };

    //Копіюємо  рядки у робочий екран
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (i < 2)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

    //Курсор не видимий
    current_ekran.cursor_on = 0;
    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;

    //Позначаємо, що цей екран треба перерисувати
    rewrite_ekran_once_more = 1;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//Формуємо екран відображення змін діагностики у записі реєстратора програмних подій
/*****************************************************/
void make_ekran_changing_diagnostics_pr_err_registrator(void)
{
  int index_language = index_language_in_array(current_settings.language);

  if (buffer_for_manu_read_record[0] == LABEL_START_RECORD_PR_ERR)
  {
    //Пеший байт сходиться із міткою початку запису - вважаємо, що у буфері достовірні дані
    const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_DIAGNOSTYKA][MAX_COL_LCD] = 
    {
      {
        NAME_DIAGN_RU
      },
      {
        NAME_DIAGN_UA
      },
      {
        NAME_DIAGN_EN
      },
      {
        NAME_DIAGN_KZ
      }
    };
  
    unsigned int max_number_changers_in_record = buffer_for_manu_read_record[8];
    unsigned int position_temp;
    unsigned int index_of_ekran;
    unsigned int diagnostic_old[3], diagnostic_new[3], diagnostic_changing[3];

    diagnostic_old[0] = buffer_for_manu_read_record[ 9] + (buffer_for_manu_read_record[10]<<8) + (buffer_for_manu_read_record[11]<<16) + (buffer_for_manu_read_record[12]<<24);
    diagnostic_old[1] = buffer_for_manu_read_record[13] + (buffer_for_manu_read_record[14]<<8) + (buffer_for_manu_read_record[15]<<16) + (buffer_for_manu_read_record[16]<<24);
    diagnostic_old[2] = buffer_for_manu_read_record[17] + (buffer_for_manu_read_record[18]<<8) + (buffer_for_manu_read_record[19]<<16) + (buffer_for_manu_read_record[20]<<24);
    diagnostic_new[0] = buffer_for_manu_read_record[21] + (buffer_for_manu_read_record[22]<<8) + (buffer_for_manu_read_record[23]<<16) + (buffer_for_manu_read_record[24]<<24);
    diagnostic_new[1] = buffer_for_manu_read_record[25] + (buffer_for_manu_read_record[26]<<8) + (buffer_for_manu_read_record[27]<<16) + (buffer_for_manu_read_record[28]<<24);
    diagnostic_new[2] = buffer_for_manu_read_record[29] + (buffer_for_manu_read_record[30]<<8) + (buffer_for_manu_read_record[31]<<16) + (buffer_for_manu_read_record[32]<<24);
        
    //Визначаємо, які сигнали змінилися
    diagnostic_changing[0] = diagnostic_new[0] ^ diagnostic_old[0];
    diagnostic_changing[1] = diagnostic_new[1] ^ diagnostic_old[1];
    diagnostic_changing[2] = diagnostic_new[2] ^ diagnostic_old[2];
    
    //Перевіряємо, чи ми не вийшли за границі
    if (current_ekran.index_position < 0) current_ekran.index_position = max_number_changers_in_record - 1;
    else if (current_ekran.index_position >= ((int)max_number_changers_in_record)) current_ekran.index_position = 0;
    position_in_current_level_menu[EKRAN_CHANGES_DIAGNOSTICS_PR_ERR] = current_ekran.index_position;

    position_temp = current_ekran.index_position;
    index_of_ekran = position_temp & (unsigned int)(~((1<<(POWER_MAX_ROW_LCD>>1)) - 1));
  
    //Копіюємо  рядки у робочий екран
    for (unsigned int i=0; i< (MAX_ROW_LCD>>1); i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
      if (index_of_ekran < max_number_changers_in_record)
      {
        //Шукаємо функцію, яку треба зараз відобразити
        unsigned int current_number_changes = 0;
        unsigned int index_of_diagnostic_in_the_slice = 0; //починаємо з першої функції
        do
        {
          if (_CHECK_SET_BIT(diagnostic_changing, index_of_diagnostic_in_the_slice) != 0)
            current_number_changes++;
          if (current_number_changes  < (index_of_ekran + 1)) index_of_diagnostic_in_the_slice++;
        }
        while (
               (current_number_changes  < (index_of_ekran + 1)) &&
               (index_of_diagnostic_in_the_slice < MAX_ROW_FOR_DIAGNOSTYKA)  
              );
        
        
        if (index_of_diagnostic_in_the_slice < MAX_ROW_FOR_DIAGNOSTYKA)
        {
          for (unsigned int k = 0; k < 2; k++)
          {
            if (((i<<1)+k) < MAX_ROW_LCD)
            {
              if (k == 0)
              {
                //У першому рядку відображаємо назву діагностики, який змінився
                for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[(i<<1)+k][j] = name_string[index_language][index_of_diagnostic_in_the_slice][j];
              }
              else
              {
                //У другому рядку відображаємо яке значення прийняла зараз дана діагностика
                unsigned char second_row[MAX_COL_LCD] = "                ";

                const unsigned char passive_active[MAX_NAMBER_LANGUAGE][2][5] = 
                {
                  {"Пасс.", "Акт. "},
                  {"Пас. ", "Акт. "},
                  {"Pass.", "Act. "},
                  {"Пасс.", "Акт. "}
                };

                //Визначаємо стан даного дискретного сигналу
                if (_CHECK_SET_BIT(diagnostic_new, index_of_diagnostic_in_the_slice) != 0)
                {
                  //Сигнал перейшов у активний стан
                  for (int j = 0; j < 4; j++)
                    second_row[1 + j] = passive_active[index_language][1][j];
                }
                else
                {
                  //Сигнал перейшов у пасивний стан
                  for (int j = 0; j < 4; j++)
                    second_row[1 + j] = passive_active[index_language][0][j];
                }

                for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[(i<<1)+k][j] = second_row[j];
              }
            }
          }
        }
        else
        {
          unsigned char error_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
          {
            {
              " Неопределенная ",
              " ошибка отображ."
            },
            {
              "  Невизначена   ",
              " помилка відобр."
            },
            {
              "   Undefined    ",
              " display error  "
            },
            {
              " Неопределенная ",
              " ошибка отображ."
            }
          };
          for (unsigned int k = 0; k < 2; k++)
          {
            if (((i<<1)+k) < MAX_ROW_LCD)
              for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[(i<<1)+k][j] = error_string[index_language][k][j];
          }
        }
      }
      else
      {
        for (unsigned int k = 0; k < 2; k++)
        {
          if (((i<<1)+k) < MAX_ROW_LCD)
            for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[(i<<1)+k][j] = ' ';
        }
      }

      index_of_ekran++;
    }
  
    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = (position_temp<<1) & (MAX_ROW_LCD - 1);
    //Курсор видимий
    current_ekran.cursor_on = 1;
  }
  else
  {
    //Пеший байт не сходиться із міткою початку запису - робимо висновок, що у біфері не достовірні дані
    const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        " Недостоверные  ",
        "     данные     "
      },
      {
        "  Недостовірні  ",
        "      дані      "
      },
      {
        "    Invalid     ",
        "      data      "
      },
      {
        " Недостоверные  ",
        "     данные     "
      }
    };

    //Копіюємо  рядки у робочий екран
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
      if (i < 2)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

    //Відображення курору по вертикалі
    current_ekran.position_cursor_y = 0;
    //Курсор не видимий
    current_ekran.cursor_on = 0;
    
    current_ekran.index_position = 0;
  }


  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
