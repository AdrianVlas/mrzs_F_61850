#include "header.h"

#if (MODYFIKACIA_VERSII_PZ >= 10)

/*****************************************************/
//������� ����� ����������� ��������� ������������ ��������� ���� RS-485
/*****************************************************/
void make_ekran_type_IEC61850_nodes(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TYPE_IEC61850_NODES][MAX_COL_LCD] = 
  {
    {
      " ��.GOOSE ����  ",
      " ��.MMS ����    ",
      " ���            "
    },
    {
      " ��.GOOSE ����  ",
      " ��.MMS ����    ",
      " ���            "
    },
    {
      " In.GOOSE block ",
      " In.MMS block   ",
      " Out.LAN block  "
    },
    {
      " ��.GOOSE ����  ",
      " ��.MMS ����    ",
      " ���            "
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
    if (index_of_ekran < MAX_ROW_FOR_TYPE_IEC61850_NODES)
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
//
/*****************************************************/
/*****************************************************/

#endif
