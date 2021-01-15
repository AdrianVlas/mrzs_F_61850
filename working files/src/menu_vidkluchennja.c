#include "header.h"

/*****************************************************/
//Формуємо екран відображення повідомдень про відключення від захистів
/*****************************************************/
void make_ekran_vidkluchenja(void)
{
  int index_language = index_language_in_array(current_settings.language);
  
  //очищаємо робочий екран
  for (unsigned int i = 0; i < MAX_ROW_LCD; i++)
  {
    for (unsigned int j = 0; j < MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
  }
  int32_t index_position_tmp = current_ekran.index_position;
  __info_vymk info_vymk = info_vidkluchennja_vymykachatime[index_position_tmp];

  time_t time_dat_tmp = info_vymk.time_dat;
  if (time_dat_tmp != 0)
  {
    struct tm *p = localtime(&info_vymk.time_dat);
    int field;

    //День
    field = p->tm_mday;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_DY1] = (field / 10) + 0x30;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_DY2] = (field % 10) + 0x30;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_DY2 + 1] = '-';

    //Місяць
    field = p->tm_mon + 1;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_MY1] = (field / 10) + 0x30;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_MY2] = (field % 10) + 0x30;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_MY2 + 1] = '-';

    //Рік
    field = p->tm_year - 100;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_SY1] = (field / 10) + 0x30;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_SY2] = (field % 10) + 0x30;

    //Година
    field = p->tm_hour;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HT1] = (field / 10) + 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HT2] = (field % 10) + 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HT2 + 1] = ':';

    //Хвилини
    field = p->tm_min;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_MT1] = (field / 10) + 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_MT2] = (field % 10) + 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_MT2 + 1] = ':';

    //Секунди
    field = p->tm_sec;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_ST1] = (field / 10) + 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_ST2] = (field % 10) + 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_ST2 + 1] = '.';

    //Тисячні секунд
    field = info_vymk.time_ms;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HST1] = (field / 100) + 0x30;
    field %= 100;
      
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HST2] = (field / 10) + 0x30;
    field %= 10;

    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HST3] = field + 0x30;
  }
  else
  {
    //День
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_DY1] = 0x30;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_DY2] = 0x30;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_DY2 + 1] = '-';

    //Місяць
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_MY1] = 0x30;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_MY2] = 0x30;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_MY2 + 1] = '-';

    //Рік
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_SY1] = 0x30;
    working_ekran[VIDKL_ROW_Y_][VIDKL_COL_SY2] = 0x30;

    //Година
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HT1] = 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HT2] = 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HT2 + 1] = ':';

    //Хвилини
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_MT1] = 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_MT2] = 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_MT2 + 1] = ':';

    //Секунди
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_ST1] = 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_ST2] = 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_ST2 + 1] = '.';

    //Тисячні секунд
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HST1] = 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HST2] = 0x30;
    working_ekran[VIDKL_ROW_T_][VIDKL_COL_HST3] = 0x30;
  }
  
  const unsigned char information_1[MAX_NAMBER_LANGUAGE][VYMKNENNJA_VID_MAX_NUMBER - NUMBER_UP + 1][7] = 
  {
    {"   МТЗ1", "   МТЗ2", "   МТЗ3", "   МТЗ4", "МТЗ04 1", "МТЗ04 2", "   ЗДЗ", "    3I0", "    3U0", "    НЗЗ", "  ТЗНП1", "  ТЗНП2", "  ТЗНП3", " АЧР от", "   АЧР1", "   АЧР2", "  УРОВ1", "  УРОВ2", "    ЗОП", " ЗНмин1", " ЗНмин2", "ЗНмакc1", "ЗНмакc2", "    УЗx", "    ОВЗ", " Другие"},
    {"   МCЗ1", "   МCЗ2", "   МCЗ3", "   МCЗ4", "МCЗ04 1", "МCЗ04 2", "   ЗДЗ", "    3I0", "    3U0", "    НЗЗ", "  СЗНП1", "  СЗНП2", "  СЗНП3", " АЧР в.", "   АЧР1", "   АЧР2", "  ПРВВ1", "  ПРВВ2", "    ЗЗП", " ЗНмін1", " ЗНмін2", "ЗНмакc1", "ЗНмакc2", "    УЗx", "    ВЗЗ", "   Інші"},
    {"   OCP1", "   OCP2", "   OCP3", "   OCP4", " LVOCP1", " LVOCP2", "   AFD", " SEFOCP", " SEFOVP", "  SDEFP", "  DEFP1", "  DEFP2", "  DEFP3", "    LFС", "   LFС1", "   LFС2", " CBF In", " CBF Ex", "  NSOCP", "   UVP1", "   UVP2", "   OVP1", "   OVP2", "   MFPx", "    Ext", "  Other"},
    {"   МТЗ1", "   МТЗ2", "   МТЗ3", "   МТЗ4", "МТЗ04 1", "МТЗ04 2", "   ЗДЗ", "    3I0", "    3U0", "    НЗЗ", "  ТЗНП1", "  ТЗНП2", "  ТЗНП3", " АЧР от", "   АЧР1", "   АЧР2", "  УРОВ1", "  УРОВ2", "    ЗОП", " ЗНмин1", " ЗНмин2", "ЗНмакc1", "ЗНмакc2", "    УЗx", "    ОВЗ", " Другие"}
  };
  unsigned char const *point;
  if ((index_position_tmp < VYMKNENNJA_VID_UP1) || (index_position_tmp >= (VYMKNENNJA_VID_UP1 + NUMBER_UP)))
  {
    if (index_position_tmp < VYMKNENNJA_VID_UP1)
      point = information_1[index_language][index_position_tmp];
    else
      point = information_1[index_language][index_position_tmp - NUMBER_UP + 1];

    for (unsigned int i = 0; i < 7; i++) working_ekran[VIDKL_ROW_Y_][VIDKL_COL_SY2 + 2 + i] = *(point + i);
  }
  else
  {
    point = information_1[index_language][VYMKNENNJA_VID_UP1];

    for (unsigned int i = 0; i < 7; i++) 
    {
      working_ekran[VIDKL_ROW_Y_][VIDKL_COL_SY2 + 2 + i] = (i != (7 - 1)) ? *(point + i) : (index_position_tmp - VYMKNENNJA_VID_UP1 + 1 + 0x30);
    }
  }
    
  
  const unsigned char information_2[MAX_NAMBER_LANGUAGE][VYMKNENNJA_VID_MAX_NUMBER - NUMBER_UP + 1][3] = 
  {
    {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", " ДВ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
    {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", " ДВ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
    {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "vBI", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "Tr.", "sig"},
    {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", " ДВ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "}
  };

  if (index_position_tmp < VYMKNENNJA_VID_UP1)
    point = information_2[index_language][index_position_tmp];
  else if (index_position_tmp >= (VYMKNENNJA_VID_UP1 + NUMBER_UP))
    point = information_2[index_language][index_position_tmp - NUMBER_UP + 1];
  else
    point = information_2[index_language][VYMKNENNJA_VID_UP1];
  for (unsigned int i = 0; i < 3; i++) working_ekran[VIDKL_ROW_T_][VIDKL_COL_HST2 + 2 + i] = *(point + i);

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  current_ekran.position_cursor_y = 0;
  //Курсор невидимий
  current_ekran.cursor_on = 0;
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
