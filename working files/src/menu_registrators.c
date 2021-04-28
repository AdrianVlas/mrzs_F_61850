#include "header.h"

/*****************************************************/
//������� ����� ����������� ��������� ����������
/*****************************************************/
void make_ekran_list_registrators(void)
{
  static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_LIST_REGISTRATORS][MAX_COL_LCD] = 
  {
    {
      " ������.���-�   ",
      " �����. ���-�   ",
      " ����� ��������."
    },
    {
      " ������.�����. ",
      " �����. �����. ",
      " ����� �������."
    },
    {
      " Dst recorder   ",
      " Binary recorder",
      " Diag recorder  "
    },
    {
      " ������.���-�   ",
      " �����. ���-�   ",
      " ����� ��������."
    }
  };

  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //�������  ����� � ������� �����
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
    if (index_of_ekran < MAX_ROW_FOR_LIST_REGISTRATORS)
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //������ �� ���������� ������������ �� ������ �������
  current_ekran.position_cursor_x = 0;
  //³���������� ������ �� ��������
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //������ �������
  current_ekran.cursor_on = 1;
  //������ �� ����
  current_ekran.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//������� ����� ����������� ������ ����������� ����������
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
  
  if (type_registrator == INDEX_ML_ANALOG_REGISTRATOR_INFO) 
  {
    unsigned int first_number = (info_rejestrator_ar.first_number < 0) ? 0 : (info_rejestrator_ar.first_number + 1);
    unsigned int last_number  = (info_rejestrator_ar.last_number  < 0) ? 0 : (info_rejestrator_ar.last_number + 1);
                
    if (first_number == 0) number_records = 0;
    else if (first_number >= last_number) number_records = first_number - last_number + 1;
    else number_records = NUMBER_FATFS_NAME - last_number + first_number + 1;
  }
  else if (type_registrator == INDEX_ML_DIGITAL_REGISTRATOR_INFO) number_records = info_rejestrator_dr.number_records;
  else number_records = info_rejestrator_pr_err.number_records;
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  if(number_records == 0)
  {
    //���� ������
    static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        "      ���       ",
        "    �������     "
      },
      {
        "     ����       ",
        "    ������     "
      },
      {
        "    Records     ",
        "   are absent   "
      },
      {
        "      ���       ",
        "    �������     "
      }
    };

    for(int index_1 = 0; index_1 < MAX_ROW_LCD; index_1++)
    {
      for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        name_string[index_1][index_2] = (index_1 < 2) ? information[index_language][index_1][index_2] : ' ';
    }

    //������ �� �������
    current_ekran.cursor_on = 0;
  }
  else if (current_ekran.index_position < ((int)number_records))
  {
    unsigned char information[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
    {
      " ������         ",
      " �����          ",
      " Record         ",
      " �����          "
    };

    static const unsigned int index_of_number[MAX_NAMBER_LANGUAGE]=  {8, 7, 8, 7};
    unsigned int index_first_symbol_in_number = index_of_number[index_language];
    unsigned int index_last_symbol_in_number = index_first_symbol_in_number + 4 - 1;
    
    unsigned int k = 0;
    while (((index_of_ekran + k) < number_records) && (k < MAX_ROW_FOR_LIST_REGISTRATORS_RECORDS))
    {
      //������� �������� �����
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
      
      //ϳ������� �����, ��� �� ���� ������ ������ ������
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

    //������ �������
    current_ekran.cursor_on = 1;
  }
  else
  {
    //����������� ��������� ��������
    static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        "     ������     ",
        "  �����������   "
      },
      {
        "    �������     ",
        "  �����������  "
      },
      {
        "    Display     ",
        "     error      "
      },
      {
        "     ������     ",
        "  �����������   "
      }
    };

    for(int index_1 = 0; index_1 < MAX_ROW_LCD; index_1++)
    {
      for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        name_string[index_1][index_2] = (index_1 < 2) ? information[index_language][index_1][index_2] : ' ';
    }

    //������ �� �������
    current_ekran.cursor_on = 0;
  }
  
  //�������  ����� � ������� �����
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[i][j];
  }

  //������ �� ���������� ������������ �� ������ �������
  current_ekran.position_cursor_x = 0;
  //³���������� ������ �� ��������
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //������ �� ����
  current_ekran.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//������� ����� ����������� ��������� ���� ��� ������ ����������� ����������
/*****************************************************/
void make_ekran_list_titles_for_record_of_digital_registrator(void)
{
  int index_language = index_language_in_array(current_settings.language);

  if ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU) == 0)
  {
    //������ ���������� ����� � DataFlash ��� ���������
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR][MAX_COL_LCD] = 
    {
      {
        " ����� �������  ",
        " ���.�����.����.",
        " ���.��� �.���� ",
        " ���.��� �.�.0.4",
        " ���.��� 3I0-max",
        " ���.��� 3U0-max",
        " ���.��� U-min  ",
        " ���.��� U-max  ",
        " ���.��� I2-max ",
        " ���.��� f-min  ",
        " ���.��� f-���� "
      },
      {
        " ̳��� ����     ",
        " ��.�����.����. ",
        " ���.��� �.��� ",
        " ���.��� �.�.0.4",
        " ���.��� 3I0-max",
        " ���.��� 3U0-max",
        " ���.��� U-min  ",
        " ���.��� U-max  ",
        " ���.��� I2-max ",
        " ���.��� f-min  ",
        " ���.��� f-���� "
      },
      {
        " Time label     ",
        " Bin S Changes  ",
        " M at Max Cur   ",
        " M at Max LV Cur",
        "M at Max 3I0 Cur",
        "M at Max 3U0 Vol",
        " M at Min Vol   ",
        " M at Max Vol   ",
        " M at Max I2 Cur",
        " M at Min f     ",
        " M.at f -FAR    "
      },
      {
        " ����� �����  ",
        " ���.�����.����.",
        " ���.��� �.���� ",
        " ���.��� �.�.0.4",
        " ���.��� 3I0-max",
        " ���.��� 3U0-max",
        " ���.��� U-min  ",
        " ���.��� U-max  ",
        " ���.��� I2-max ",
        " ���.��� f-min  ",
        " ���.��� f-���� "
      }
    };

    unsigned int position = current_ekran.index_position;
    unsigned int position_temp = position;
    unsigned int shift[MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR];
    for (unsigned int *pShift = shift; pShift < (shift + MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR); ++pShift) *pShift = 0;

    {
      unsigned int *pShift = shift + INDEX_ML_TITLE_DR_CHANGES_SIGNALS + 1;
      unsigned int val_shift = 0;
      for (size_t i = (INDEX_ML_TITLE_DR_CHANGES_SIGNALS + 1); i < MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR; ++i)
      {
        if (
            ((i == INDEX_ML_TITLE_DR_MAX_PHASE     ) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MAX_PHASE_DR     ] == 0)) ||
            ((i == INDEX_ML_TITLE_DR_MAX_PHASE04   ) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MAX_PHASE04_DR   ] == 0)) ||
            ((i == INDEX_ML_TITLE_DR_MAX_3I0       ) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MAX_3I0_DR       ] == 0)) ||
            ((i == INDEX_ML_TITLE_DR_MAX_3U0       ) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MAX_3U0_DR       ] == 0)) ||
            ((i == INDEX_ML_TITLE_DR_MIN_U         ) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MIN_U_DR         ] == 0)) ||
            ((i == INDEX_ML_TITLE_DR_MAX_U         ) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MAX_U_DR         ] == 0)) ||
            ((i == INDEX_ML_TITLE_DR_MAX_ZOP       ) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MAX_ZOP_DR       ] == 0)) ||
            ((i == INDEX_ML_TITLE_DR_MIN_F1        ) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_MIN_F_ACHR_DR    ] == 0)) ||
            ((i == INDEX_ML_TITLE_DR_MIN_F2        ) && (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_F_CHAPV_DR       ] == 0))
           )
        {
          if (i < position)
          {
            --position_temp;
          }
          *pShift = ++val_shift;
        }
        else
        {
          *(pShift++) = val_shift;
        }
      }
      while (pShift < (shift + MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR)) *(pShift++) = val_shift;
    }
  
    unsigned int index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
    //�������  ����� � ������� �����
    for (size_t i = 0; i < MAX_ROW_LCD; ++i)
    {
      unsigned int const real_index = index_of_ekran + shift[index_of_ekran];
      //������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
      if (real_index < MAX_ROW_FOR_TITLES_DIGITAL_REGISTRATOR)
        for (size_t j = 0; j < MAX_COL_LCD; ++j) working_ekran[i][j] = name_string[index_language][real_index][j];
      else
        for (size_t j = 0; j < MAX_COL_LCD; ++j) working_ekran[i][j] = ' ';

      index_of_ekran++;
    }
  
    //������ �������
    current_ekran.cursor_on = 1;
    //³���������� ������ �� ��������
    current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
    
    //���������, �� ����� ��� ����� �������������� �� �����
    rewrite_ekran_once_more = 0;
  }
  else
  {
    //������ ���������� ����� � DataFlash �� �� ���������
    static unsigned char const name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        " ������� ������ ",
        "  �� ��������   "
      },
      {
        " ������ ������� ",
        " �� ����������  "
      },
      {
        "    Reading     ",
        "is not completed"
      },
      {
        " ������� ������ ",
        "  �� ��������   "
      }
    };

    //�������  ����� � ������� �����
    for (size_t i = 0; i < MAX_ROW_LCD; ++i)
    {
      //������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
      if (i < 2)
        for (size_t j = 0; j < MAX_COL_LCD; ++j) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (size_t j = 0; j < MAX_COL_LCD; ++j) working_ekran[i][j] = ' ';
    }

    //������ �� �������
    current_ekran.cursor_on = 0;
    //³���������� ������ �� ��������
    current_ekran.position_cursor_y = 0;

    //���������, �� ��� ����� ����� ������������
    rewrite_ekran_once_more = 1;
  }

  //������ �� ���������� ������������ �� ������ �������
  current_ekran.position_cursor_x = 0;
  //������ �� ����
  current_ekran.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//������� ����� ����������� ���� � ���� ������ ����������
/*
0 - ���������� ���������
1 - ��������� ���������� ����
2 - ���������� ���������
*/
/*****************************************************/
void make_ekran_data_and_time_of_records_registrator(unsigned int type_of_registrator)
{
  int index_language = index_language_in_array(current_settings.language);

  if (
      ((type_of_registrator == 0) && (buffer_for_manu_read_record[FIRST_INDEX_START_START_RECORD_DR] == LABEL_START_RECORD_DR    )) ||
      ((type_of_registrator == 1) && (buffer_for_manu_read_record[0] == LABEL_START_RECORD_PR_ERR)) ||
      ((type_of_registrator == 2) && !_GET_STATE(FATFS_command, FATFS_READ_DATA_FOR_MENU) && (buffer_for_manu_read_record[0] == LABEL_START_RECORD_AR))
     )
  {
    //����� ���� ��������� �� ����� ������� ������ - �������, �� � ����� �������� ���
    unsigned char name_string[MAX_ROW_FOR_EKRAN_DATA_LABEL][MAX_COL_LCD] = 
    {
      "   XX-XX-20XX   ",
      "  XX:XX:XX.XXX  "
    };
  
    unsigned int position_temp = current_ekran.index_position;
    unsigned int index_of_ekran;
  
    index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
    /******************************************/
    //���������� ���� ���������� �������
    /******************************************/
    time_t time_dat_tmp = 0;
    int32_t time_ms_tmp = 0;
    if (type_of_registrator == 2)
    {
      __HEADER_AR header_ar_tmp;
      /*
      � ������ ������ ��������� ������ � ��������� ���������� �����������.
      ��� �������� �� �� ������ ������ ��������� ��������� ����������� ����������
      ��� ����� ���� ��������� ������ ����
      */
      header_ar_tmp = *((__HEADER_AR*)buffer_for_manu_read_record);
      time_dat_tmp = header_ar_tmp.time_dat;
      time_ms_tmp = header_ar_tmp.time_ms;

      //���������, �� ����� ��� ����� �������������� �� �����
      rewrite_ekran_once_more = 0;
    }
    else
    {
      for(size_t i = 0; i < sizeof(time_t); i++) *((unsigned char*)(&time_dat_tmp) + i) =  buffer_for_manu_read_record[1 + i];
      for(size_t i = 0; i < sizeof(int32_t); i++) *((unsigned char*)(&time_ms_tmp) + i) = buffer_for_manu_read_record[1 + sizeof(time_t) + i];
    }
    
    if (time_dat_tmp != 0)
    {
      struct tm *p = localtime(&time_dat_tmp);
      int field;
      
      //����
      field = p->tm_mday;
      name_string[ROW_R_Y_][COL_DY1_R] = (field / 10) + 0x30;
      name_string[ROW_R_Y_][COL_DY2_R] = (field % 10) + 0x30;

      //̳����
      field = p->tm_mon + 1;
      name_string[ROW_R_Y_][COL_MY1_R] = (field / 10) + 0x30;
      name_string[ROW_R_Y_][COL_MY2_R] = (field % 10) + 0x30;

      //г�
      field = p->tm_year - 100;
      name_string[ROW_R_Y_][COL_SY1_R] = (field / 10) + 0x30;
      name_string[ROW_R_Y_][COL_SY2_R] = (field % 10) + 0x30;

      //������
      field = p->tm_hour;
      name_string[ROW_R_T_][COL_HT1_R] = (field / 10) + 0x30;
      name_string[ROW_R_T_][COL_HT2_R] = (field % 10) + 0x30;

      //�������
      field = p->tm_min;
      name_string[ROW_R_T_][COL_MT1_R] = (field / 10) + 0x30;
      name_string[ROW_R_T_][COL_MT2_R] = (field % 10) + 0x30;

      //�������
      field = p->tm_sec;
      name_string[ROW_R_T_][COL_ST1_R] = (field / 10) + 0x30;
      name_string[ROW_R_T_][COL_ST2_R] = (field % 10) + 0x30;

      //������ ������
      field = time_ms_tmp;
      name_string[ROW_R_T_][COL_HST1_R] = (field / 100) + 0x30;
      field %= 100;
      
      name_string[ROW_R_T_][COL_HST2_R] = (field / 10) + 0x30;
      field %= 10;
      
      name_string[ROW_R_T_][COL_HST3_R] = field + 0x30;
    }

    //�������  ����� � ������� �����
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
      if (index_of_ekran < MAX_ROW_FOR_EKRAN_DATA_LABEL)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_of_ekran][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

      index_of_ekran++;
    }
  
    //³���������� ������ �� ��������
    current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  }
  else if ((type_of_registrator == 2) && _GET_STATE(FATFS_command, FATFS_READ_DATA_FOR_MENU))
  {
    //������ ���������� ����� � DataFlash �� �� ���������
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        " ������� ������ ",
        "  �� ��������   "
      },
      {
        " ������ ������� ",
        " �� ����������  "
      },
      {
        "    Reading     ",
        "is not completed"
      },
      {
        " ������� ������ ",
        "  �� ��������   "
      }
    };

    //�������  ����� � ������� �����
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
      if (i < 2)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

    //³���������� ������ �� ��������
    current_ekran.position_cursor_y = 0;

    //���������, �� ��� ����� ����� ������������
    rewrite_ekran_once_more = 1;
  }
  else
  {
    //����� ���� �� ��������� �� ����� ������� ������ - ������ ��������, �� � ���� �� �������� ���
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        " �������������  ",
        "     ������     "
      },
      {
        "  ����������  ",
        "      ���      "
      },
      {
        "    Invalid     ",
        "      data      "
      },
      {
        " �������������  ",
        "     ������     "
      }
    };
    
    if(type_of_registrator == 2)
    {
      //���������, �� ����� ��� ����� ���� ������ ����������� ���������� �������������� �� ����� - �� ������� ������� � ������������� ������
      rewrite_ekran_once_more = 0;
    }

    //�������  ����� � ������� �����
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
      if (i < 2)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

    //³���������� ������ �� ��������
    current_ekran.position_cursor_y = 0;
  }

  //������ �� �������
  current_ekran.cursor_on = 0;

  //������ �� ���������� ������������ �� ������ �������
  current_ekran.position_cursor_x = 0;
  //������ �� ����
  current_ekran.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//������� ����� ����������� ����� ������� ��� �������� ������������/��������� ��������� ������ ����������� ����������
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

  index_cell_into_array_for_min_max_measurement_dr = -1; //�������, �� �� ������ �� ������� ������ ������, ���� ������� ������� �����, ���� ���������� ��������
  if(number_info_records == 0)
  {
    static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        "      ���       ",
        "    �������     "
      },
      {
        "     ����       ",
        "    ������     "
      },
      {
        "    Records     ",
        "   are absent   "
      },
      {
        "      ���       ",
        "    �������     "
      }
    };

    for(int index_1 = 0; index_1 < 2; index_1++)
    {
      for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        name_string[index_1][index_2] = information[index_language][index_1][index_2];
    }

    //������ �� �������
    current_ekran.cursor_on = 0;
  }
  else if (current_ekran.index_position < ((int)number_info_records))
  {
    unsigned int k = 0;
    while (((index_of_ekran + k) < number_info_records) && (k < MAX_ROW_FOR_TITLE_EKRAN_ANALOG_VALUES_DR))
    {
      //������ ����, ���� ������� �������� ���� ������
      unsigned int number_finded_this_type_of_current = 0;
      unsigned int index_cell_into_array = (FIRST_INDEX_FIRST_BLOCK_DR + (sizeof(unsigned int)*SIZE_ARRAY_FIX_MAX_MEASUREMENTS - 1));//������ ������ ���'�� ������������ �� ���� ������� �����, ���� �������� ��� ������ �� ����� ���� ���������
      while ((number_finded_this_type_of_current != (index_of_ekran + k + 1)) && (index_cell_into_array < FIRST_INDEX_FIRST_DATA_DR))
      {
        if (buffer_for_manu_read_record[index_cell_into_array] == type_view_max_values_dr)
          number_finded_this_type_of_current++;
        if (number_finded_this_type_of_current != (index_of_ekran + k + 1))
          index_cell_into_array += (sizeof(unsigned int)*SIZE_ARRAY_FIX_MAX_MEASUREMENTS);
      }
      
      unsigned char information[MAX_COL_LCD] = "                ";
      static const unsigned char ms[MAX_NAMBER_LANGUAGE][3] = 
      {
        "��.",
        "��.",
        "ms ",
        "��."
      };
      if ((number_finded_this_type_of_current == (index_of_ekran + k + 1)) && (index_cell_into_array < FIRST_INDEX_FIRST_DATA_DR))
      {
        //�� ������� �������� ����
        //���� � ����� ����� �� � ����������� ������, �� ����� �����'����� ������ ������� �������� ��������� �����
        if (position_temp  == (index_of_ekran + k))
          index_cell_into_array_for_min_max_measurement_dr = index_cell_into_array - (sizeof(unsigned int)*SIZE_ARRAY_FIX_MAX_MEASUREMENTS - 1);

        //��������� ��� ����
        index_cell_into_array -= 3; //���������� ������ �� ������� ������ ����
        unsigned char sring_of_time[8] = "        ";
        unsigned int time_of_slice = buffer_for_manu_read_record[index_cell_into_array] + (buffer_for_manu_read_record[index_cell_into_array + 1]<<8) + (buffer_for_manu_read_record[index_cell_into_array + 2]<<16);

        //���������� ��� ��� � �����
        unsigned int ost, local_index = 0;
        while ((time_of_slice >= 10) && (local_index < (8 - 1)))
        {
          ost = time_of_slice % 10;
          sring_of_time[7-local_index] = ost + 0x30;
          time_of_slice /= 10;
          local_index++;
        }
        sring_of_time[7-local_index] =  time_of_slice + 0x30;

        //������� ��� ���� ���� � ��� ������� �����
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
        //���������� �� ���� � �� ���� ����� ��������, ��� ���� �� ���� ������, �� �������� ����������� ��� ������� �� �����
        static const unsigned char error_meas[MAX_NAMBER_LANGUAGE][MAX_COL_LCD] = 
        {
          " ������         ",
          " �������        ",
          " Error          ",
          " ������         "
        };
        for (unsigned int i = 0; i < MAX_COL_LCD; i++) information[i] = error_meas[index_language][i];
      }

      for (unsigned int i = 0; i < MAX_COL_LCD; i++) name_string[k][i] = information[i];
      k++;
    }

    //������ �������
    current_ekran.cursor_on = 1;
  }
  else
  {
    //����������� ��������� ��������
    static const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        "     ������     ",
        "  �����������   "
      },
      {
        "    �������     ",
        "  �����������  "
      },
      {
        "    Display     ",
        "     error      "
      },
      {
        "     ������     ",
        "  �����������   "
      }
    };

    for(int index_1 = 0; index_1 < 2; index_1++)
    {
      for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        name_string[index_1][index_2] = information[index_language][index_1][index_2];
    }

    //������ �� �������
    current_ekran.cursor_on = 0;
  }
  
  //�������  ����� � ������� �����
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[i][j];
  }

  //������ �� ���������� ������������ �� ������ �������
  current_ekran.position_cursor_x = 0;
  //³���������� ������ �� ��������
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //������ �� ����
  current_ekran.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//������� ����� ����������� ����� ������� ��� �������� ������������� ������ 3I0/����/���(���) ������ ����������� ����������
/*****************************************************/
void make_ekran_analog_value_records_digital_registrator(void)
{
  int index_language = index_language_in_array(current_settings.language);

  if (buffer_for_manu_read_record[FIRST_INDEX_START_START_RECORD_DR] == LABEL_START_RECORD_DR)
  {
    //����� ���� ��������� �� ����� ������� ������ - �������, �� � ����� �������� ���
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
       //������ � �������
//       if (i == 2)
//       {
//         if (index_language == INDEX_LANGUAGE_EN) name_string[i][4] = 'c';
//         else name_string[i][4] = '�';
//       }

        unsigned int temp_measurement = *(point_unsigned_int + i);
        unsigned int start_number_digit_after_point;
        if ((i == 0) || (i == 1)) start_number_digit_after_point = 2;
        else start_number_digit_after_point = 3;
        convert_and_insert_char_for_measurement(start_number_digit_after_point, temp_measurement, 1, 1, name_string[i], 7);
      }
      else if (i == 18)
      {
        //�������
        int temp_measurement = *(((int *)point_unsigned_int) + i);
        if (temp_measurement < 0)
        {
          if (temp_measurement == (-2*1000))
          {
            /*������� ����� ������ ����������� ���������� MIN_FREQUENCY*/
            name_string[i][4] = '<';
            temp_measurement = MIN_FREQUENCY*1000;
          }
          if (temp_measurement == (-3*1000))
          {
            /*������� ���� ������ ����������� ���������� MAX_FREQUENCY*/
            name_string[i][4] = '>';
            temp_measurement = MAX_FREQUENCY*1000;
          }       
        }
        convert_and_insert_char_for_frequency(temp_measurement, name_string[i]);
      }
      else if (i < 25)
      {
       //�����
        static const unsigned int index_of_start_position_array[MAX_NAMBER_LANGUAGE] = {4, 4, 5, 4};

#define SIZE_R_DIMENSION    2
        static const unsigned int size_dimension_array[MAX_NAMBER_LANGUAGE] = {SIZE_R_DIMENSION, SIZE_R_DIMENSION, SIZE_R_DIMENSION - 1, SIZE_R_DIMENSION};
        static const unsigned char resistance_dimension[MAX_NAMBER_LANGUAGE][SIZE_R_DIMENSION] = {"��", "��", "� ", "��"}; /*� ��� ��� �� ������� ������ ����� ����� ��� ��������� ���������*/
  
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
          //������� ���������� ��������
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
          static const unsigned char undefined[MAX_NAMBER_LANGUAGE][SIZE_UNDEF] =
          {
            "�������. ",
            "��������.",
            "Undef.   ",
            "�������. "  
          };
          for (unsigned int j = 0; j < size_dimension; j++) name_string[i][MAX_COL_LCD - size_dimension + j] = ' ';
          for (unsigned int j = 0; j < SIZE_UNDEF; j++) name_string[i][start_position + 1 + j] = undefined[index_language][j];
#undef SIZE_UNDEF
        }
      }
      else if ((i == 25) && (type_view_max_values_dr == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE))
      {
        //̳��� �����������
#define SIZE_NAME_FIELD         2
        static const unsigned char name_field[MAX_NAMBER_LANGUAGE][SIZE_NAME_FIELD] = {"��", "��", "FD", "��"};
        for (unsigned int j = 0; j < SIZE_NAME_FIELD; j++)
        {
          name_string[i][1 + j] = name_field[index_language][j];
        }
#undef SIZE_NAME_FIELD

#define INDEX_LESS_EQUAL_MORE   4
#define SIZE_L_DIMENSION        2

        static const unsigned char km[MAX_NAMBER_LANGUAGE][SIZE_L_DIMENSION] = {"��", "��", "km", "��"};
  
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
          //������� ���������� ��������
          /********************************/
          if (temp_measurement_1 < 0)
          {
            temp_measurement_1 = -temp_measurement_1;
            name_string[i][INDEX_LESS_EQUAL_MORE + 1] = '-';
          }
          convert_and_insert_char_for_measurement(3, temp_measurement_1, 1, 1, name_string[i], (INDEX_LESS_EQUAL_MORE + 2));
          
          //� ��������� �� ������ � ��������� ��������� ���� �����
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
          static const unsigned char undefined[MAX_NAMBER_LANGUAGE][SIZE_UNDEF] =
          {
            "�������. ",
            "��������.",
            "Undef.   ",
            "�������. "  
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
        //����� � ��� �� ��� ������ �� ��� ����������� ��������
      }
    }
  
    int position_temp = current_ekran.index_position;
    unsigned int index_of_ekran;

    /******************************************/
    //��������� ����, �� �� ����� ����������
    /******************************************/
    int additional_current = 0;
    
    {
      int shift_ind;
      
      if ((control_extra_settings_1_dr_for_manu & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04)) == 0) shift_ind = 8 - additional_current;
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

    if ((control_extra_settings_1_dr_for_manu & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0)
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
  
    //�������  ����� � ������� �����
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
      if (((int)index_of_ekran) < (MAX_ROW_FOR_EKRAN_ANALOG_VALUES_DR - additional_current))
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_of_ekran][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

      index_of_ekran++;
    }
  
    //³���������� ������ �� ��������
    current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
    //������ �������
    current_ekran.cursor_on = 1;
  }
  else
  {
    //����� ���� �� ��������� �� ����� ������� ������ - ������ ��������, �� � ���� �� �������� ���
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        " �������������  ",
        "     ������     "
      },
      {
        "  ����������  ",
        "      ���      "
      },
      {
        "    Invalid     ",
        "      data      "
      },
      {
        " �������������  ",
        "     ������     "
      }
    };

    //�������  ����� � ������� �����
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
      if (i < 2)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

    //³���������� ������ �� ��������
    current_ekran.position_cursor_y = 0;
    //������ �� �������
    current_ekran.cursor_on = 0;
  }


  //������ �� ���������� ������������ �� ������ �������
  current_ekran.position_cursor_x = 0;
  //������ �� ����
  current_ekran.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//������� ����� ����������� ��� ���������� ������� � ������ ����������� ����������
/*****************************************************/
void make_ekran_changing_signals_digital_registrator(void)
{
  int index_language = index_language_in_array(current_settings.language);

  if (buffer_for_manu_read_record[FIRST_INDEX_START_START_RECORD_DR] == LABEL_START_RECORD_DR)
  {
    //����� ���� ��������� �� ����� ������� ������ - �������, �� � ����� �������� ���
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][ NUMBER_TOTAL_SIGNAL_FOR_RANG + (1 - N_IN_GOOSE)  + (1 - N_IN_MMS) + (1 - N_OUT_LAN) + (3  - NUMBER_UP_SIGNAL_FOR_RANG)][MAX_COL_LCD] = 
#else
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][NUMBER_TOTAL_SIGNAL_FOR_RANG + (3 - NUMBER_UP_SIGNAL_FOR_RANG)][MAX_COL_LCD] = 
#endif
    {
      {NAME_RANG_RU},
      {NAME_RANG_UA},
      {NAME_RANG_EN},
      {NAME_RANG_KZ},
    };
    
    uint8_t name_string_tmp[NUMBER_TOTAL_SIGNAL_FOR_RANG][MAX_COL_LCD];
    for(int index_1 = 0; index_1 < NUMBER_TOTAL_SIGNAL_FOR_RANG; index_1++)
    {
      size_t index_row;
      if (index_1 < NUMBER_GENERAL_SIGNAL_FOR_RANG) 
      {
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        if (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) 
        {
          index_row = index_1;
        }
        else if (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) 
        {
          index_row = RANG_BLOCK_IN_GOOSE1 + ((index_1 - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN))) % 1);
        }
        else if (index_1 < (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) 
        {
          index_row = RANG_BLOCK_IN_MMS1 + (1 - N_IN_GOOSE) + ((index_1 - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) % 1);
        }
        else
        {
          index_row = RANG_BLOCK_OUT_LAN1 + (1 - N_IN_GOOSE) + (1 - N_IN_MMS) + ((index_1 - (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) % 1);
        }
#else
        index_row = index_1;
#endif        
      }
      else if (index_1 < RANG_BLOCK_UP1) 
      {
        index_row = index_1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                     + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif        
                     ;
      }
      else if (index_1 < (RANG_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG))
      {
        index_row = RANG_BLOCK_UP1
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                     + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif
                     + ((index_1 - RANG_BLOCK_UP1) % 3);
      }
      else
      {
        index_row = index_1 
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                    + 1 - N_IN_GOOSE + 1 - N_IN_MMS + 1 - N_OUT_LAN
#endif        
                    + 3 - NUMBER_UP_SIGNAL_FOR_RANG;
      }
      
      for(size_t index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      {
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
        if (
            (index_1 >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN)))  &&
            (index_1 <  (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN))) &&
            (index_2 >=  index_n_In_GOOSE[index_language][(index_1 - RANG_BLOCK_IN_GOOSE1) % 1]) &&
            (index_2 <= (index_n_In_GOOSE[index_language][(index_1 - RANG_BLOCK_IN_GOOSE1) % 1] + 1)) 
           )   
        {
          unsigned int n = index_1 - RANG_BLOCK_IN_GOOSE1;
          if ((n + 1) < 10)
          {
            if (index_2 == index_n_In_GOOSE[index_language][n % 1])
              name_string_tmp[index_1][index_2] = 0x30 + (n + 1);
            else
              name_string_tmp[index_1][index_2] = ' ';
          }
          else
          {
            if (index_2 == index_n_In_GOOSE[index_language][n % 1])
              name_string_tmp[index_1][index_2] = 0x30 + (n / 1 + 1) / 10;
            else
              name_string_tmp[index_1][index_2] = 0x30 + (n / 1 + 1) % 10;
          }
        }
        else if (
                 (index_1 >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_IN_MMS + N_OUT_LAN)))  &&
                 (index_1 <  (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN))) &&
                 (index_2 == index_n_In_MMS[index_language][(index_1 - RANG_BLOCK_IN_MMS1) % 1]) 
                )   
        {
          name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - RANG_BLOCK_IN_MMS1) / 1 + 1);
        }
        else if (
                 (index_1 >= (NUMBER_GENERAL_SIGNAL_FOR_RANG - (N_OUT_LAN)))  &&
                 (index_1 <  (NUMBER_GENERAL_SIGNAL_FOR_RANG)) &&
                 (index_2 == index_n_Out_LAN[index_language][(index_1 - RANG_BLOCK_OUT_LAN1) % 1]) 
                )   
        {
          name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - RANG_BLOCK_OUT_LAN1) / 1 + 1);
        }
        else 
#endif
        {
          if (
              (index_1 >=  RANG_BLOCK_UP1)  &&
              (index_1 <  (RANG_BLOCK_UP1 + NUMBER_UP_SIGNAL_FOR_RANG)) &&
              (index_2 == index_number_UP[index_language][(index_1 - RANG_BLOCK_UP1) % 3]) 
             )   
          {
            name_string_tmp[index_1][index_2] = 0x30 + ((index_1 - RANG_BLOCK_UP1) / 3 + 1);
          }
          else name_string_tmp[index_1][index_2] = name_string[index_language][index_row][index_2];
        }
      }
    }
  
    unsigned int max_number_changers_in_record = buffer_for_manu_read_record[FIRST_INDEX_NUMBER_CHANGES_DR] | (buffer_for_manu_read_record[FIRST_INDEX_NUMBER_CHANGES_DR + 1]<<8);
    unsigned int position_temp;
    unsigned int index_of_ekran;
    unsigned int array_old[N_BIG], array_new[N_BIG], array_changing[N_BIG];
    
    //����������, �� �� �� ������ �� �������
    if (current_ekran.index_position < 0) current_ekran.index_position = max_number_changers_in_record - 1;
    else if (current_ekran.index_position >= ((int)max_number_changers_in_record)) current_ekran.index_position = 0;
    current_ekran.index_position = (current_ekran.index_position >> (POWER_MAX_ROW_LCD - 1)) << (POWER_MAX_ROW_LCD - 1);
    position_in_current_level_menu[EKRAN_CHANGES_SIGNALS_DR] = current_ekran.index_position;

    position_temp = current_ekran.index_position;
    index_of_ekran = position_temp & (unsigned int)(~((1<<(POWER_MAX_ROW_LCD>>1)) - 1));
  
    //�������  ����� � ������� �����
    for (unsigned int i=0; i< (MAX_ROW_LCD>>1); i++)
    {
      //������� ����� ����� ���������, �� �� ����� ���������� � ������� �����������
      if (index_of_ekran < max_number_changers_in_record)
      {
        //������ ������ ����: ��������� �� ������� ���� � � ������ ���� ����
        int index_of_the_slice = 0; //�������� � ������� ����
        unsigned int current_number_changes = 0;
        while (
               ((current_number_changes + buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice) + (SD_DR - 2)] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice) + (SD_DR - 1)] << 8)) < (index_of_ekran + 1)) &&
               (index_of_the_slice < MAX_EVENTS_IN_ONE_RECORD)  
              )
        {
          current_number_changes += (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice) + (SD_DR - 2)] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice) + (SD_DR - 1)] << 8));
          index_of_the_slice++;
        }

        for (size_t j = 0; j < ( N_BIG - ((NUMBER_TOTAL_SIGNAL_FOR_RANG % 32) != 0) ); ++j)
        {
          array_new[j] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice    ) +  3 + 4*j] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice    ) +  4 + 4*j]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice    ) +  5 + 4*j]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice    ) +  6 + 4*j]<<24);

          array_old[j] = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice - 1) +  3 + 4*j] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice - 1) +  4 + 4*j]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice - 1) +  5 + 4*j]<<16) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice - 1) +  6 + 4*j]<<24);
        }
        if (( NUMBER_TOTAL_SIGNAL_FOR_RANG % 32 ) != 0)
        {
          array_new[N_BIG - 1] = 0;
          array_old[N_BIG - 1] = 0;
          for (size_t j = 0; j <  (NUMBER_BYTES_SAMPLE_DR - 4*( NUMBER_TOTAL_SIGNAL_FOR_RANG / 32 )); ++j)
          {
            array_new[N_BIG - 1] += (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice    ) +  (3 + j) + 4*(N_BIG - 1)] << (8*j));

            array_old[N_BIG - 1] += (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice - 1) +  (3 + j) + 4*(N_BIG - 1)] << (8*j));
          }
        }
        
        //���������, �� ������� ��������
        for (unsigned int j = 0; j < N_BIG; j++) array_changing[j] = array_new[j] ^ array_old[j];

        //������ �������, ��� ����� ����� ����������
        int index_of_function_in_the_slice = 0; //�������� � ������� ����
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
                //� ������� ����� ���������� ����� �������, ���� �������
                for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[(i<<1)+k][j] = name_string_tmp[index_of_function_in_the_slice][j];
              }
              else
              {
                //� ������� ����� ���������� ��� ���� � ��� �������� ������� ����� ������
                unsigned char second_row[MAX_COL_LCD] = "                ";
                unsigned char sring_of_time[8] = "        ";
                static const unsigned char ms[MAX_NAMBER_LANGUAGE][3] = 
                {
                  "��.",
                  "��.",
                  "ms ",
                  "��."
                };
                static const unsigned char passive_active[MAX_NAMBER_LANGUAGE][2][5] = 
                {
                  {"����.", "���. "},
                  {"���. ", "���. "},
                  {"Pass ", "Act  "},
                  {"����.", "���. "}
                };
                unsigned int time_of_slice = buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice) + 0] + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice) + 1]<<8) + (buffer_for_manu_read_record[FIRST_INDEX_FIRST_DATA_DR + SD_DR*(1 + index_of_the_slice) + 2]<<16);
                
                //���������� ��� ��� � �����
                unsigned int ost, local_index = 0;
                while ((time_of_slice >= 10) && (local_index < (8 - 1)))
                {
                  ost = time_of_slice % 10;
                  sring_of_time[7-local_index] = ost + 0x30;
                  time_of_slice /= 10;
                  local_index++;
                }
                sring_of_time[7-local_index] =  time_of_slice + 0x30;
                
                //������� ��� ���� ���� � ��� ������� �����
                local_index = 0;
                while ((sring_of_time[local_index] == ' ') && (local_index < 8)) local_index++;
                unsigned int local_index1 = 0;
                while (local_index < 8) second_row[local_index1++] = sring_of_time[local_index++];
                if (local_index1 < 8) second_row[local_index1++] = ' ';
                second_row[local_index1++] = ms[index_language][0];
                second_row[local_index1++] = ms[index_language][1];
                second_row[local_index1++] = ms[index_language][2];
                
                //��������� ���� ������ ����������� �������
                if ((array_new[index_of_function_in_the_slice >> 5] & (1 << (index_of_function_in_the_slice & ((1<<5)-1)))) != 0)
                {
                  //������ �������� � �������� ����
                  for (local_index1 = 11; local_index1 < MAX_COL_LCD; local_index1++)
                    second_row[local_index1] = passive_active[index_language][1][local_index1 - 11];
                }
                else
                {
                  //������ �������� � �������� ����
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
              " �������������� ",
              " ������ �������."
            },
            {
              "  �����������   ",
              " ������� �����."
            },
            {
              "   Undefined    ",
              " display error  "
            },
            {
              " �������������� ",
              " ������ �������."
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
  
    //³���������� ������ �� ��������
    current_ekran.position_cursor_y = (position_temp<<1) & (MAX_ROW_LCD - 1);
    //������ ���������
    current_ekran.cursor_on = 0;
  }
  else
  {
    //����� ���� �� ��������� �� ����� ������� ������ - ������ ��������, �� � ���� �� �������� ���
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        " �������������  ",
        "     ������     "
      },
      {
        "  ����������  ",
        "      ���      "
      },
      {
        "    Invalid     ",
        "      data      "
      },
      {
        " �������������  ",
        "     ������     "
      }
    };

    //�������  ����� � ������� �����
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
      if (i < 2)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

    //³���������� ������ �� ��������
    current_ekran.position_cursor_y = 0;
    //������ �� �������
    current_ekran.cursor_on = 0;
    
    current_ekran.index_position = 0;
  }


  //������ �� ���������� ������������ �� ������ �������
  current_ekran.position_cursor_x = 0;
  //������ �� ����
  current_ekran.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//������� ����� ����������� ��������� ���� ��� ������ ���������� ���������� ����
/*****************************************************/
void make_ekran_list_titles_for_record_of_pr_err_registrator(void)
{
  int index_language = index_language_in_array(current_settings.language);

  if ((control_tasks_dataflash & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU) == 0)
  {
    //������ ���������� ����� � DataFlash ��� ���������
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TITLES_PR_ERR_REGISTRATOR][MAX_COL_LCD] = 
    {
      {
        " ����� �������  ",
        " ���.�����������"
      },
      {
        " ̳��� ����     ",
        " ��.���������� "
      },
      {
        " Time label     ",
        " Diagn.Changes  "
      },
      {
        " ����� �����  ",
        " ���.�����������"
      }
    };
  
    unsigned int position_temp = current_ekran.index_position;
    unsigned int index_of_ekran;
  
    index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
    //�������  ����� � ������� �����
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
      if (index_of_ekran < MAX_ROW_FOR_TITLES_PR_ERR_REGISTRATOR)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

      index_of_ekran++;
    }
  
    //������ �������
    current_ekran.cursor_on = 1;
    //³���������� ������ �� ��������
    current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
    
    //���������, �� ����� ��� ����� �������������� �� �����
    rewrite_ekran_once_more = 0;
  }
  else
  {
    //������ ���������� ����� � DataFlash ��� ���������
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        " ������� ������ ",
        "  �� ��������   "
      },
      {
        " ������ ������� ",
        " �� ����������  "
      },
      {
        "    Reading     ",
        "is not completed"
      },
      {
        " ������� ������ ",
        "  �� ��������   "
      }
    };

    //�������  ����� � ������� �����
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
      if (i < 2)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

    //������ �� �������
    current_ekran.cursor_on = 0;
    //³���������� ������ �� ��������
    current_ekran.position_cursor_y = 0;

    //���������, �� ��� ����� ����� ������������
    rewrite_ekran_once_more = 1;
  }

  //������ �� ���������� ������������ �� ������ �������
  current_ekran.position_cursor_x = 0;
  //������ �� ����
  current_ekran.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//������� ����� ����������� ��� ���������� � ����� ���������� ���������� ����
/*****************************************************/
void make_ekran_changing_diagnostics_pr_err_registrator(void)
{
  int index_language = index_language_in_array(current_settings.language);

  if (buffer_for_manu_read_record[0] == LABEL_START_RECORD_PR_ERR)
  {
    //����� ���� ��������� �� ����� ������� ������ - �������, �� � ����� �������� ���
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_DIAGNOSTYKA][MAX_COL_LCD] = 
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

    unsigned char name_string_tmp[MAX_ROW_FOR_DIAGNOSTYKA][MAX_COL_LCD];

    for(unsigned int index_1 = 0; index_1 < MAX_ROW_FOR_DIAGNOSTYKA; index_1++)
    {
      for(unsigned int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
      
      if ((index_1 >= ERROR_DIGITAL_OUTPUT_1_BIT) && (index_1 < (ERROR_DIGITAL_OUTPUT_1_BIT + NUMBER_OUTPUTS)))
      {
        uint32_t value = 0;
        uint32_t index_board = 0, index_number = 0;
        for(size_t index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
        {
          if (name_string_tmp[index_1][index_2] == '?')
          {
            if (value == 0)
            {
              //�� � ������ �������� �����
              index_board = index_2;
              value = (index_1 - ERROR_DIGITAL_OUTPUT_1_BIT) + 1;
            }
            else
            {
              //�� � ������ �������� ������ �� ����
              index_number = index_2;
              break;
            }
          }
        }
        
        int tmp_1 = -1, tmp_2 = -1;
        for (size_t i = 0; i < N_OUTPUT_BOARDS; i++)
        {
          if (value <= output_boards[i][0])
          {
            tmp_1 = output_boards[i][1];
            tmp_2 = (i == 0) ? value : value - output_boards[i - 1][0];
          
            break;
          }
        }
        
        name_string_tmp[index_1][index_board]  = tmp_1 + 0x40;
        name_string_tmp[index_1][index_number] = tmp_2 + 0x30;
      }
    }
    
    unsigned int max_number_changers_in_record = buffer_for_manu_read_record[13];
    unsigned int position_temp;
    unsigned int index_of_ekran;
    unsigned int diagnostic_old[N_DIAGN], diagnostic_new[N_DIAGN], diagnostic_changing[N_DIAGN];

    for (size_t i = 0; i < N_DIAGN_BYTES; i ++)
    {
      size_t n_word = i >> 2;
      size_t n_byte = i & 0x3;
      if (n_byte == 0)
      {
        diagnostic_old[n_word] = 0;
        diagnostic_new[n_word] = 0;
      }
      diagnostic_old[n_word] |= buffer_for_manu_read_record[14                 + i] << (8*n_byte);
      diagnostic_new[n_word] |= buffer_for_manu_read_record[14 + N_DIAGN_BYTES + i] << (8*n_byte);
    }
        
    //���������, �� ������� ��������
    for (size_t i = 0; i < N_DIAGN; i ++) diagnostic_changing[i] = diagnostic_new[i] ^ diagnostic_old[i];
    
    //����������, �� �� �� ������ �� �������
    if (current_ekran.index_position < 0) current_ekran.index_position = max_number_changers_in_record - 1;
    else if (current_ekran.index_position >= ((int)max_number_changers_in_record)) current_ekran.index_position = 0;
    position_in_current_level_menu[EKRAN_CHANGES_DIAGNOSTICS_PR_ERR] = current_ekran.index_position;

    position_temp = current_ekran.index_position;
    index_of_ekran = position_temp & (unsigned int)(~((1<<(POWER_MAX_ROW_LCD>>1)) - 1));
  
    //�������  ����� � ������� �����
    for (unsigned int i=0; i< (MAX_ROW_LCD>>1); i++)
    {
      //������� ����� ����� ���������, �� �� ����� ���������� � ������� �����������
      if (index_of_ekran < max_number_changers_in_record)
      {
        //������ �������, ��� ����� ����� ����������
        unsigned int current_number_changes = 0;
        unsigned int index_of_diagnostic_in_the_slice = 0; //�������� � ����� �������
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
                //� ������� ����� ���������� ����� ����������, ���� �������
                for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[(i<<1)+k][j] = name_string_tmp[index_of_diagnostic_in_the_slice][j];
              }
              else
              {
                //� ������� ����� ���������� ��� �������� �������� ����� ���� ����������
                unsigned char second_row[MAX_COL_LCD] = "                ";

                static const unsigned char passive_active[MAX_NAMBER_LANGUAGE][2][5] = 
                {
                  {"����.", "���. "},
                  {"���. ", "���. "},
                  {"Pass ", "Act  "},
                  {"����.", "���. "}
                };

                //��������� ���� ������ ����������� �������
                if (_CHECK_SET_BIT(diagnostic_new, index_of_diagnostic_in_the_slice) != 0)
                {
                  //������ �������� � �������� ����
                  for (int j = 0; j < 4; j++)
                    second_row[1 + j] = passive_active[index_language][1][j];
                }
                else
                {
                  //������ �������� � �������� ����
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
              " �������������� ",
              " ������ �������."
            },
            {
              "  �����������   ",
              " ������� �����."
            },
            {
              "   Undefined    ",
              " display error  "
            },
            {
              " �������������� ",
              " ������ �������."
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
  
    //³���������� ������ �� ��������
    current_ekran.position_cursor_y = (position_temp<<1) & (MAX_ROW_LCD - 1);
    //������ �������
    current_ekran.cursor_on = 1;
  }
  else
  {
    //����� ���� �� ��������� �� ����� ������� ������ - ������ ��������, �� � ���� �� �������� ���
    static const unsigned char name_string[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
    {
      {
        " �������������  ",
        "     ������     "
      },
      {
        "  ����������  ",
        "      ���      "
      },
      {
        "    Invalid     ",
        "      data      "
      },
      {
        " �������������  ",
        "     ������     "
      }
    };

    //�������  ����� � ������� �����
    for (unsigned int i=0; i< MAX_ROW_LCD; i++)
    {
      //������� ����� ����� ���������, �� �� ����� ���������� � ������� ������������
      if (i < 2)
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][i][j];
      else
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
    }

    //³���������� ������ �� ��������
    current_ekran.position_cursor_y = 0;
    //������ �� �������
    current_ekran.cursor_on = 0;
    
    current_ekran.index_position = 0;
  }


  //������ �� ���������� ������������ �� ������ �������
  current_ekran.position_cursor_x = 0;
  //������ �� ����
  current_ekran.cursor_blinking_on = 0;
  //�������� ������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
