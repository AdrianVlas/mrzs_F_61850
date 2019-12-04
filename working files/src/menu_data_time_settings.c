#include "header.h"

/*****************************************************/
//������� ����� ����������� ��������� ������������ ��� ���� � ����
/*****************************************************/
void make_ekran_chose_data_time_settings(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_DATA_TIME_SETTINGS][MAX_COL_LCD] = 
  {
    {
      " ������� ����   "
#if (MODYFIKACIA_VERSII_PZ >= 10)
                        ,
      " �������������  "
#endif
    },
    {
      " ������� ����   "
#if (MODYFIKACIA_VERSII_PZ >= 10)
                        ,
      " ������������  "
#endif
    },
    {
      " Time zone      "
#if (MODYFIKACIA_VERSII_PZ >= 10)
                        ,
      " Synchronization"
#endif
    },
    {
      " ������� ����   "
#if (MODYFIKACIA_VERSII_PZ >= 10)
                        ,
      " �������������  "
#endif
    }
  };
  int index_language = index_language_in_array(current_settings.language);

  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //�������  ����� � ������� �����
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_DATA_TIME_SETTINGS)
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
//������� ����� ����������� ���������� �� ������ ��� � �������� �� ���� ���
/*****************************************************/
void make_ekran_timezone_dst(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEZONE_SETTINGS][MAX_COL_LCD] = 
  {
    {
      "  ������� ����  ",
      "  ������ �����  "
    },
    {
      "  ������� ����  ",
      "   ����� ���   "
    },
    {
      "   Time zone    ",
      "      DST       "
    },
    {
      "  ������� ����  ",
      "  ������ �����  "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //�������� �� ��� �������� position_temp ������� ��� ����, �� �� ���� ������� �� ������������� ��� ����� (����� + ��������)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < MAX_ROW_FOR_TIMEZONE_SETTINGS)
    {
      if ((i & 0x1) == 0)
      {
        //� ��������� ����� ����� �������� ���������
        for (size_t j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //� ������� ����� ����� �������� ����
        __SETTINGS *point_1 = (view == true) ? &current_settings : &edition_settings;
        
        if (index_of_ekran_tmp == INDEX_ML_TIME_ZONE)
        {
          int dst = point_1->dst;

          for (size_t j = 0; j < MAX_COL_LCD; j++) 
          {
            if (j == 8) 
            {
              if (dst >= 0) working_ekran[i][j] = '+';
              else 
              {
                working_ekran[i][j] = '-';
                dst *= -1; //������, ��� ����� ���� �������
              }
            }
            else if (j == 9) working_ekran[i][j] = ((dst >= 10) ? (dst / 10) : dst ) + 0x30;
            else if (j == 10) working_ekran[i][j] = (dst >= 10) ? ((dst % 10) + 0x30) : ' ';
            else 
            {
              const unsigned char information[MAX_COL_LCD] = "     UTC___     ";
        
              working_ekran[i][j] = information[j];
            }
          }
          if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = 4;
        }
        else if (index_of_ekran_tmp == INDEX_ML_DST)
        {
          unsigned int state = ((point_1->time_zone & MASKA_FOR_BIT(N_BIT_TZ_DST)) != 0);

          for (size_t j = 0; j < MAX_COL_LCD; j++) working_ekran[i][j] = string_off_on[index_language][state][j];
          if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = cursor_x_string_off_on[index_language][state];
        }
      }
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //³���������� ������ �� �������� � ������ ������ �� ���� � ��� �� ��������� ������
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //������ �������
  current_ekran.cursor_on = 1;
  //������ �� ����
  if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  else current_ekran.cursor_blinking_on = 1;
  //�������� ������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

#if (MODYFIKACIA_VERSII_PZ >= 10)
/*****************************************************/
//������� ����� ����������� ����������� ���������� ���� Ethernet
/*****************************************************/
void make_ekran_settings_synchro()
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SYNCHRO][MAX_COL_LCD] = 
  {
    {
      "   IP �������   ",
      "      ����      ",
      "     ������     "
    }, 
    {
      "   IP �������   ",
      "      ����      ",
      "     �����     "
    }, 
    {
      "   Server IP    ",
      "      Port      ",
      "     Period     "
    },
    {
      "   IP �������   ",
      "      ����      ",
      "     ������     "
    }  
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //�������� �� ��� �������� position_temp ������� ��� ����, �� ����� ������� �� ������������� ��� ����� (����� + ��������)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    uint32_t index_of_ekran_tmp = index_of_ekran >> 1;
    unsigned int view = ((current_ekran.edition == 0) || (position_temp != index_of_ekran_tmp));
    if (index_of_ekran_tmp < MAX_ROW_FOR_SYNCHRO)
    {
      if ((i & 0x1) == 0)
      {
        //� ��������� ����� ����� �������� ���������
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //� ������� ����� ����� �������� ��������
        const uint8_t string[] = "0123456789";
        __SETTINGS *point_1 = (view == true) ? &current_settings : &edition_settings;
        
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
        if (index_of_ekran_tmp == INDEX_ML_SYN_IPV4)
        {
          working_ekran[i][4] = working_ekran[i][8] = working_ekran[i][12] = '.';

          uint32_t j = COL_IP4_SERVER_BEGIN + 3 - 1;
          for (size_t k1 = 0; k1 < 4; k1++)
          {
            uint32_t val_tmp = point_1->IP_time_server[k1];
            for (size_t k2 = 0; k2 < 3; k2++)
            {
              uint32_t val = val_tmp % 10;
              val_tmp /= 10;
              if (
                  (current_ekran.edition != 0) ||
                  (k2 == 0) ||
                  (val != 0) ||
                  (val_tmp != 0)  
                 )
              {
                working_ekran[i][j--] = string[val];
              }
              else
              {
                 working_ekran[i][j--] = ' ';
              }
            }
            j += (3 + 1 + 3);
          }
        }
        else if (index_of_ekran_tmp == INDEX_ML_SYN_PORT)
        {
          uint32_t port_tmp = point_1->port_time_server;
        
          uint32_t j = COL_PORT_END;
          for (size_t k2 = 0; k2 < 5; k2++)
          {
            uint32_t val = port_tmp % 10;
            port_tmp /= 10;
            if (
                (current_ekran.edition != 0) ||
                (k2 == 0) ||
                (val != 0) ||
                (port_tmp != 0)  
               )
            {
              working_ekran[i][j--] = string[val];
            }
            else
            {
              working_ekran[i][j--] = ' ';
            }
          }
        }
        else if (index_of_ekran_tmp == INDEX_ML_SYN_PERIOD)
        {
          uint32_t period_tmp = point_1->period_sync;
        
          uint32_t j = COL_PEDIOD_END;
          for (size_t k2 = 0; k2 < 3; k2++)
          {
            uint32_t val = period_tmp % 10;
            period_tmp /= 10;
            if (
                (current_ekran.edition != 0) ||
                (k2 == 0) ||
                (val != 0) ||
                (period_tmp != 0)  
               )
            {
              working_ekran[i][j--] = string[val];
            }
            else
            {
              working_ekran[i][j--] = ' ';
            }
          }
        }
      }
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //³���������� ������ �� �������� � ������ ������ �� ���� � ��� �� ��������� ������
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //������ �� ���������� ������������ �� ������� ������ � �������, ���� �� �� � ����� �����������, ������ ������� ���� ��������� � ������ main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = MAX_COL_LCD;
    if (current_ekran.index_position == INDEX_ML_SYN_IPV4)
    {
      current_ekran.position_cursor_x = COL_IP4_SERVER_BEGIN;
      last_position_cursor_x = COL_IP4_SERVER_END;
    }
    else if (current_ekran.index_position == INDEX_ML_SYN_PORT) 
    {
      current_ekran.position_cursor_x = COL_PORT_BEGIN;
      last_position_cursor_x = COL_PORT_END;
    }
    else if (current_ekran.index_position == INDEX_ML_SYN_PERIOD) 
    {
      current_ekran.position_cursor_x = COL_PERIOD_BEGIN;
      last_position_cursor_x = COL_PEDIOD_END;
    }

    //ϳ������� ������ �� ������� �������
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;

    //������ ������� ���, ��� �� ��� ����� ������
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
  }

  //������ �������, ���� �� � ����� �����������
  current_ekran.cursor_on = 1;
  
  //������ �� ����
  if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  else current_ekran.cursor_blinking_on = 1;
  //�������� ������� ���� �����
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/
#endif

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
