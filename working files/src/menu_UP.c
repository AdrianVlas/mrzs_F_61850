#include "header.h"

/*****************************************************/
//Формуємо екран відображення значення управлінської інформації для Універсального Захисту
/*****************************************************/
void make_ekran_control_UP()
{
  const uint8_t name_string[MAX_NAMBER_LANGUAGE][NUMBER_UP*MAX_ROW_FOR_CONTROL_UP][MAX_COL_LCD] = 
  {
    {
      "    Вход УЗ1    ",
      "      УЗ1       ",
      " ИЛИ/И для УЗ1  ",
      "  >/< для УЗ1   ",
      "    Вход УЗ2    ",
      "      УЗ2       ",
      " ИЛИ/И для УЗ2  ",
      "  >/< для УЗ2   ",
      "    Вход УЗ3    ",
      "      УЗ3       ",
      " ИЛИ/И для УЗ3  ",
      "  >/< для УЗ3   ",
      "    Вход УЗ4    ",
      "      УЗ4       ",
      " ИЛИ/И для УЗ4  ",
      "  >/< для УЗ4   ",
      "    Вход УЗ5    ",
      "      УЗ5       ",
      " ИЛИ/И для УЗ5  ",
      "  >/< для УЗ5   ",
      "    Вход УЗ6    ",
      "      УЗ6       ",
      " ИЛИ/И для УЗ6  ",
      "  >/< для УЗ6   ",
      "    Вход УЗ7    ",
      "      УЗ7       ",
      " ИЛИ/И для УЗ7  ",
      "  >/< для УЗ7   ",
      "    Вход УЗ8    ",
      "      УЗ8       ",
      " ИЛИ/И для УЗ8  ",
      "  >/< для УЗ8   "
    },
    {
      "    Вхід УЗ1    ",
      "      УЗ1       ",
      " АБО/І для УЗ1  ",
      "  >/< для УЗ1   ",
      "    Вхід УЗ2    ",
      "      УЗ2       ",
      " АБО/І для УЗ2  ",
      "  >/< для УЗ2   ",
      "    Вхід УЗ3    ",
      "      УЗ3       ",
      " АБО/І для УЗ3  ",
      "  >/< для УЗ3   ",
      "    Вхід УЗ4    ",
      "      УЗ4       ",
      " АБО/І для УЗ4  ",
      "  >/< для УЗ4   ",
      "    Вхід УЗ5    ",
      "      УЗ5       ",
      " АБО/І для УЗ5  ",
      "  >/< для УЗ5   ",
      "    Вхід УЗ6    ",
      "      УЗ6       ",
      " АБО/І для УЗ6  ",
      "  >/< для УЗ6   ",
      "    Вхід УЗ7    ",
      "      УЗ7       ",
      " АБО/І для УЗ7  ",
      "  >/< для УЗ7   ",
      "    Вхід УЗ8    ",
      "      УЗ8       ",
      " АБО/І для УЗ8  ",
      "  >/< для УЗ8   "
    },
    {
      "   UP1 Input    ",
      "      UP1       ",
      " OR/AND for UP1 ",
      "  >/< for UP1   ",
      "   UP2 Input    ",
      "      UP2       ",
      " OR/AND for UP2 ",
      "  >/< for UP2   ",
      "   UP3 Input    ",
      "      UP3       ",
      " OR/AND for UP3 ",
      "  >/< for UP3   ",
      "   UP4 Input    ",
      "      UP4       ",
      " OR/AND for UP4 ",
      "  >/< for UP4   ",
      "   UP5 Input    ",
      "      UP5       ",
      " OR/AND for UP5 ",
      "  >/< for UP5   ",
      "   UP6 Input    ",
      "      UP6       ",
      " OR/AND for UP6 ",
      "  >/< for UP6   ",
      "   UP7 Input    ",
      "      UP7       ",
      " OR/AND for UP7 ",
      "  >/< for UP7   ",
      "   UP8 Input    ",
      "      UP8       ",
      " OR/AND for UP8 ",
      "  >/< for UP8   "
    },
    {
      "    Вход УЗ1    ",
      "      УЗ1       ",
      " ИЛИ/И для УЗ1  ",
      "  >/< для УЗ1   ",
      "    Вход УЗ2    ",
      "      УЗ2       ",
      " ИЛИ/И для УЗ2  ",
      "  >/< для УЗ2   ",
      "    Вход УЗ3    ",
      "      УЗ3       ",
      " ИЛИ/И для УЗ3  ",
      "  >/< для УЗ3   ",
      "    Вход УЗ4    ",
      "      УЗ4       ",
      " ИЛИ/И для УЗ4  ",
      "  >/< для УЗ4   ",
      "    Вход УЗ5    ",
      "      УЗ5       ",
      " ИЛИ/И для УЗ5  ",
      "  >/< для УЗ5   ",
      "    Вход УЗ6    ",
      "      УЗ6       ",
      " ИЛИ/И для УЗ6  ",
      "  >/< для УЗ6   ",
      "    Вход УЗ7    ",
      "      УЗ7       ",
      " ИЛИ/И для УЗ7  ",
      "  >/< для УЗ7   ",
      "    Вход УЗ8    ",
      "      УЗ8       ",
      " ИЛИ/И для УЗ8  ",
      "  >/< для УЗ8   "
    },
  };
  
  const uint8_t information_1[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
  {
    {"     Откл.      ", "      Вкл.      "},
    {"     Вимк.      ", "     Ввімк.     "},
    {"      Off       ", "       On       "},
    {"     Сљнд.      ", "     Косу.      "}
  };
  const uint32_t cursor_x_1[MAX_NAMBER_LANGUAGE][2] = 
  {
   {4, 5},
   {4, 4},
   {5, 6},
   {4, 4}
  };
  uint8_t information_2[_UP_CTRL_NUMBER][MAX_COL_LCD] = 
  {
    "    Ia/Ib/Ic    ",
    "       Ia       ",
    "       Ib       ",
    "       Ic       ",
    "       I1       ",
    "       I2       ",
    "      I0.4      ",
    "      3I0 .     ",
    "       3I0      ",
    "      3I0**     ",
    "                ",
    "                ",
    "                ",
    "                ",
    "       U1       ",
    "       U2       ",
    "       3U0      ",
    "       P        ",
    "       Q        ",
    "       S        "
  };
  const uint32_t cursor_x_2[_UP_CTRL_NUMBER] = {3, 6, 6, 6, 6, 6, 5, 5, 6, 5, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6};
  
  const uint8_t information_U[4][2][MAX_COL_LCD] = 
  {
    {"    Ua/Ub/Uc    ", "  Uab/Ubc/Uca   "},
    {"       Ua       ", "      Uab       "},
    {"       Ub       ", "      Ubc       "},
    {"       Uc       ", "      Uca       "}
  };
  const uint32_t cursor_x_U[4][2] = 
  {
    {3, 1},
    {6, 5},
    {6, 5},
    {6, 5}
  };
  
  const uint8_t information_3[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] =
  {
    {"      ИЛИ       ", "       И        "},
    {"      АБО       ", "       І        "},
    {"       OR       ", "      AND       "},
    {"      ИЛИ       ", "       И        "}
  };
  const uint32_t cursor_x_3[MAX_NAMBER_LANGUAGE][2] = 
  {
    {5, 6},
    {5, 6},
    {6, 5},
    {5, 6}
  };
    
  const uint8_t information_4[2][MAX_COL_LCD] = 
  {
    "       >        ",
    "       <        "
  };
  const uint32_t cursor_x_4 = 6;
        
  int index_language = index_language_in_array(current_settings.language);

  if (index_language == INDEX_LANGUAGE_EN) information_2[UP_CTRL_3I0_r][9] = 'c';
  else information_2[UP_CTRL_3I0_r][9] = 'р';
  
  __SETTINGS *point = (current_ekran.edition == 0) ? &current_settings : &edition_settings;
  
  __ctrl_info ctrl_info[NUMBER_UP*MAX_ROW_FOR_CONTROL_UP];
  for (size_t i = 0; i < (NUMBER_UP*MAX_ROW_FOR_CONTROL_UP); i++)
  {
    switch (i)
    {
    case 0:
    case 4:
    case 8:
    case 12:
    case 16:
    case 20:
    case 24:
    case 28:
      {
        uint32_t index = point->ctrl_UP_input[i / MAX_ROW_FOR_CONTROL_UP];
        switch (index)
        {
        case UP_CTRL_Ua_Ub_Uc:
        case UP_CTRL_Ua:
        case UP_CTRL_Ub:
        case UP_CTRL_Uc:
          {
            uint32_t index_2 = ((current_settings.control_extra_settings_1 & CTR_EXTRA_SETTINGS_1_CTRL_PHASE_LINE) != 0);
            ctrl_info[i].information = information_U[index - UP_CTRL_Ua_Ub_Uc][index_2];
            ctrl_info[i].cursor_x = cursor_x_U[index - UP_CTRL_Ua_Ub_Uc][index_2];
            
            break;
          }
        default:
          {
            ctrl_info[i].information = information_2[index];
            ctrl_info[i].cursor_x = cursor_x_2[index];
            
            break;
          }
        }
        break;
      }
    case 1:
    case 5:
    case 9:
    case 13:
    case 17:
    case 21:
    case 25:
    case 29:
      {
        uint32_t index = (point->control_UP >> ((i / MAX_ROW_FOR_CONTROL_UP)*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) & 0x1;
        ctrl_info[i].information = information_1[index_language][index];
        ctrl_info[i].cursor_x = cursor_x_1[index_language][index];
        break;
      }
    case 2:
    case 6:
    case 10:
    case 14:
    case 18:
    case 22:
    case 26:
    case 30:
      {
        uint32_t index = (point->control_UP >> ((i / MAX_ROW_FOR_CONTROL_UP)*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_AND_OR_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) & 0x1;
        ctrl_info[i].information = information_3[index_language][index];
        ctrl_info[i].cursor_x = cursor_x_3[index_language][index];
        break;
      }
    case 3:
    case 7:
    case 11:
    case 15:
    case 19:
    case 23:
    case 27:
    case 31:
      {
        uint32_t index = (point->control_UP >> ((i / MAX_ROW_FOR_CONTROL_UP)*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_LESS_MORE_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) & 0x1;
        ctrl_info[i].information = information_4[index];
        ctrl_info[i].cursor_x = cursor_x_4;
        break;
      }
    default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed(90);
      }
    }
  }

  uint8_t name_string_tmp[NUMBER_UP*MAX_ROW_FOR_CONTROL_UP][MAX_COL_LCD];

  for(size_t index_1 = 0; index_1 < (NUMBER_UP*MAX_ROW_FOR_CONTROL_UP); index_1++)
  {
    for(size_t index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
  }
  
  /******************************************/
  //Виключаємо поля, які не треба відображати
  /******************************************/
  int32_t additional_current = 0;
  int32_t position_temp = current_ekran.index_position;
  
  for (size_t current_index = 0; current_index < (NUMBER_UP*MAX_ROW_FOR_CONTROL_UP); current_index++ )
  {
    uint32_t ctrl_UP_input = point->ctrl_UP_input[current_index / (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I)];
    if (
         ((current_index % (_CTR_UP_NEXT_BIT - _CTR_UP_PART_I)) == CTR_UP_AND_OR_BIT) &&
         (ctrl_UP_input != UP_CTRL_Ia_Ib_Ic) &&
         (ctrl_UP_input != UP_CTRL_Ua_Ub_Uc)
       )   
    {
      int i = current_index - additional_current;

      if ((i+1) <= position_temp) position_temp--;
      do
      {
        if ((i+1) < (NUMBER_UP*MAX_ROW_FOR_CONTROL_UP - additional_current))
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
      while (i < (NUMBER_UP*MAX_ROW_FOR_CONTROL_UP - additional_current));
    
      additional_current++;
    }
  }
  /******************************************/
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  int32_t index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (size_t i=0; i< MAX_ROW_LCD; i++)
  {
    int32_t index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < (NUMBER_UP*MAX_ROW_FOR_CONTROL_UP - additional_current))//Множення на два константи потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (size_t j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки

        for (size_t j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ctrl_info[index_of_ekran_tmp].information[j];
        if (position_temp == index_of_ekran_tmp) current_ekran.position_cursor_x = ctrl_info[index_of_ekran_tmp].cursor_x;
      }
    }
    else
      for (size_t j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

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
