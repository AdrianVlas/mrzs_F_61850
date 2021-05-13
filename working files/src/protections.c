#include "header.h"

/*****************************************************/
//������ ������� ���������� �����
/*****************************************************/
inline void input_scan(void)
{
  unsigned int state_inputs_into_pin = 0; //���� � ��� ������� �������� ����� ������� �� ����� ��������� (���)
  static unsigned int state_inputs_into_pin_trigger; //� ��� ����� ���������� ��������� ���� ���, � ������� ���� �� ���������� .�� ���������� ��� ���� �� ���� ���������� ������� �������

  /***************************/
  //� ��� ������ ������ ����� ����� ��������� (���), �� ���������� �� �������� �����
  /*
  -----------------------------
  �������� ��������� � �������� ��� ����� state_inputs_into_pin
    "�     ������ " - ������� ������������� ��� (1)
    "���� �������" - ������� ���������     ��� (0)
  -----------------------------
  */
  state_inputs_into_pin |=  ( _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36)       & 0xffff)
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 10)      \
    )                                   
                         | (((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >>  8) & 0xf) << 16)
#elif (                                   \
       (MODYFIKACIA_VERSII_PZ == 5) ||    \
       (MODYFIKACIA_VERSII_PZ == 15)      \
      )                                   
                         | (((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 12) & 0xf) << 16)
#endif
                        ;
  /***************************/
  
  /***************************/
  //������� ������� ������� ���������� �����
  /***************************/
  for(unsigned int i = 0; i < NUMBER_INPUTS; i++)
  {
    unsigned int maska = 1<<i;
    int max_value_timer = current_settings_prt.dopusk_dv[i];

    if (global_timers[INDEX_TIMER_INPUT_START + i] < 0)
    {
      //������� �� ���� ����� ����������� ����� ����������� ����� ���, ���� �������� ������� � ��'�����
      //�� ������ �� ������ �� ��������� � ��������� ������
      
      //���������� �� ��������� ���� ����� ������� ������������ �����
      if ((state_inputs_into_pin & maska) != (state_inputs & maska))
      {
        //���� ���� ����� �������, �� �������� 䳿 �� ��� �����

        //1-��: �����'������� ������� ���� �����
        state_inputs_into_pin_trigger &= ~maska;
        state_inputs_into_pin_trigger |= (state_inputs_into_pin & maska);

        //2-��: ��������� ������ ������� ����������� �����
        global_timers[INDEX_TIMER_INPUT_START + i] = 0;
      }
    }
    else
    {
      //���� ������ ��������� �������� 䳿 � ��������
      if (global_timers[INDEX_TIMER_INPUT_START + i] < max_value_timer)
      {
        //���� ������ �� �� ����� �� ����� ���������, �� ������ �������� ���� ��������
        global_timers[INDEX_TIMER_INPUT_START + i] += DELTA_TIME_FOR_TIMERS;
        
        //� �������, ���� ��� ������� �� ���� �������� ������
        //� �� ���������� �� �� ������� ������� "� ������"->"���� �������"
        //�� ����� ������� �� ��� ���������� ������� ������� ������, �� ������ �� ���� � - 
        //� �� ������, �� ����� �������� n�����, �� �������� "� ������"->"���� �������" �� ������ ���������� ������� �� ����������� 
        if ((current_settings_prt.type_of_input_signal & maska) != 0)
        {
           if ((state_inputs_into_pin_trigger & maska) == 0)
           {
             if ((state_inputs_into_pin & maska) != 0)
               global_timers[INDEX_TIMER_INPUT_START + i] = -1;
           }
        }
      }
    }
    
    //���� �������� ������� ������� ����������� � ����� ������������� �������, �� ����������, �� �������� ������� ����� �����
    if (global_timers[INDEX_TIMER_INPUT_START + i] >= max_value_timer)
    {
      //����������, �� ���� ��� ������� � �������� �� ������, ���� ��� �� ������ ������� ������� ������� ����������� �����
      unsigned int state_1, state_2;
        
      state_1 = state_inputs_into_pin_trigger & maska;
      state_2 = state_inputs_into_pin  & maska;
        
      if (state_1 == state_2)
      {
        //���� ��� ����� ����������, �� �� �������, �� �������� ��� ����� ����������� ����� � ������� ����� ���� �����
        //��� ����� ��������� �� ��� �������    ����� 1 - �� �������� ����, � 0 - �� �������� ����
        //                        ��� ���������� ����� 0 - �� �������� ����, � 1 - �� �������� ����
          state_inputs &=  ~maska;
          state_inputs |=   state_2;
      }
        
      //� ����-����� ������, �� ������� ���� �����, �� �, � ������� ������ ������� ���������� �� ����, �� ������������� � �������� ��'���� ��������
      global_timers[INDEX_TIMER_INPUT_START + i] = -1;
    }
  }
  /***************************/
  
  /***************************/
  //���������, ���� ������ � ����� ��������
  /***************************/
  active_inputs = (state_inputs ^ current_settings_prt.type_of_input);
  /***************************/
  
}
/*****************************************************/

/*****************************************************/
//������� ������� �������
/*****************************************************/
inline void clocking_global_timers(void)
{
  //����������� �������� �����
  input_scan();
  
  //����������� �� ����� ������� �����
  for (int *p = (global_timers + _INDEX_TIMER_LOGIC_BEGIN); p != (global_timers + _MAX_NUMBER_GLOBAL_TIMERS); ++p)
  {
    if (*p >= 0)
    {
      //������ ������ ����, �� ������ ����� ��������� � ��� ����, �� �������� ������� �� ��'����

      //���������� �� ����� ���������� �������� �������, ���� �� �� �� �������� ����� ���������
      if (*p <= (0x7fffffff - DELTA_TIME_FOR_TIMERS)) *p += DELTA_TIME_FOR_TIMERS;
    }
  }


  static unsigned int power_is;
  if ((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t)Bit_RESET) 
  {
    power_is = true;

    timePowerDown_total = timePowerDown = -1;
  }
  else
  {
    if (
        (measurement[IM_IA] > POWEER_IS_FROM_IA_IC) ||
        (measurement[IM_IC] > POWEER_IS_FROM_IA_IC)
       )
    {
      power_is = true;
    }
    else if (
             (measurement[IM_IA] < POWEER_ISNOT_FROM_IA_IC) &&
             (measurement[IM_IC] < POWEER_ISNOT_FROM_IA_IC)
            )
    {
      power_is = false;
    }
    else
    {
      //power_is ���������� ��� ���
    }
    
    if (timePowerDown < 0) timePowerDown = 0;
    else if (power_is) timePowerDown = 0;
    else if (
             (timePowerDown >= 0) &&
             (timePowerDown <= (0x7fffffff - DELTA_TIME_FOR_TIMERS))  
            )
    {
      timePowerDown += DELTA_TIME_FOR_TIMERS;
    }

    if (timePowerDown_total < 0) timePowerDown_total = 0;
    else if (
             (timePowerDown_total >= 0) &&
             (timePowerDown_total <= (0x7fffffff - DELTA_TIME_FOR_TIMERS))  
            )
    {
      timePowerDown_total += DELTA_TIME_FOR_TIMERS;
    }
  }


  
  if (++timer_prt_signal_output_mode_2 >= PERIOD_SIGNAL_OUTPUT_MODE_2)
  {
    timer_prt_signal_output_mode_2 = 0;
    output_timer_prt_signal_output_mode_2 ^= true;
  }
}
/*****************************************************/

/*****************************************************/
//����������� ������������� ������� - �� ����������� ���� ������������� ����� ��� �������
/*****************************************************/
inline void df_handler(unsigned int *p_active_functions)
{
  unsigned int logic_df[NUMBER_DEFINED_FUNCTIONS];
  
  //���������, �� ������������� ���������� ������� ����� ��� �������� �������-�������
  for (unsigned int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
  {
    unsigned int *p_logic_df = (logic_df + i);
    *p_logic_df = 0;
    
    unsigned int number_byte_in = 0, number_bit_in = 0;
    switch (i)
    {
    case 0:
      {
        number_byte_in = RANG_DF1_IN >> 5;
        number_bit_in = RANG_DF1_IN & 0x1f;

        break;
      }
    case 1:
      {
        number_byte_in = RANG_DF2_IN >> 5;
        number_bit_in = RANG_DF2_IN & 0x1f;

        break;
      }
    case 2:
      {
        number_byte_in = RANG_DF3_IN >> 5;
        number_bit_in = RANG_DF3_IN & 0x1f;
        
        break;
      }
    case 3:
      {
        number_byte_in = RANG_DF4_IN >> 5;
        number_bit_in = RANG_DF4_IN & 0x1f;

        break;
      }
    case 4:
      {
        number_byte_in = RANG_DF5_IN >> 5;
        number_bit_in = RANG_DF5_IN & 0x1f;

        break;
      }
    case 5:
      {
        number_byte_in = RANG_DF6_IN >> 5;
        number_bit_in = RANG_DF6_IN & 0x1f;

        break;
      }
    case 6:
      {
        number_byte_in = RANG_DF7_IN >> 5;
        number_bit_in = RANG_DF7_IN & 0x1f;

        break;
      }
    case 7:
      {
        number_byte_in = RANG_DF8_IN >> 5;
        number_bit_in = RANG_DF8_IN & 0x1f;

        break;
      }
    default:
      {
        //���������� ����� ����� �� ���� � ����
        total_error_sw_fixed();
        break;
      }
    }

    /***
    ������� ��������� ��-��
    ***/
    *p_logic_df |= ((p_active_functions[number_byte_in] & (1 << number_bit_in) ) >> number_bit_in ) << 0;
    //���������� ��, �� �� ��� ���������� �������� ������� �� ����� ������-����������� (��� ��������� ����� ������ ��� ���������)
    if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] >= 0)
    {
      //������ ���������, ��� ��� ���������
      //���� ������� ����� ������� ������, �� ��������� ���������� ����� ������, ��� ���������
      //���� ��� ������������ ������ ������ ����� �� ���� ������ ����� ������� �����������, �� ������� ��������� �������
      *p_logic_df |= (1 << 0);
      
      //³������, ������� ��������� ����������� � ��������� ���� � ����� ���������� �������
      p_active_functions[number_byte_in] |= (1 << number_bit_in);
      
      //� �������, ���� ������ ����� �� ����� ������������ ��������, �� ������� ������ ����� �������
      if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] >= ((int)current_settings_prt.timeout_pause_df[i]))
        global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] = -1;
    }
    /***/

    int flag=0;
    for(int m=0; m<N_BIG; m++) if( ( current_settings_prt.ranguvannja_df_source_plus[N_BIG*i    +m] & p_active_functions[m] ) != 0) {flag=1; break;}
    if(flag)
    {
      *p_logic_df |= (1 << 0);
    }

    flag=0;
    for(int m=0; m<N_BIG; m++) if( ( current_settings_prt.ranguvannja_df_source_minus[N_BIG*i    +m] & ((unsigned int)(~p_active_functions[m])) ) != 0 ) {flag=1;break;}
    if(flag)
    {
      *p_logic_df |= (1 << 0);
    }

    flag=1;
    for(int m=0; m<N_BIG; m++) if( ( current_settings_prt.ranguvannja_df_source_blk[N_BIG*i    +m] & p_active_functions[m] ) != 0 ) {flag=0; break;}
    if(flag)
    {
      *p_logic_df |= (1 << 1);
    }
    
    /***
    �������� � ����� ���� ������� ����� ��� �����������/�������� ��� ��� ���� ����������� �������
    ��� ����� ���� ���������� ������� �� ��� ��������� � ���� ��������
    ***/
    _TIMER_T_0(INDEX_TIMER_DF_PAUSE_START + i, current_settings_prt.timeout_pause_df[i], (*p_logic_df), 0, (*p_logic_df), 2);
    
    if ((current_settings_prt.type_df & (1 << i)) == 0)
    {
      _TIMER_IMPULSE(INDEX_TIMER_DF_WORK_START + i, current_settings_prt.timeout_work_df[i], static_logic_df, i, (*p_logic_df), 2, (*p_logic_df), 3);
    }
    else
    {
      _TIMER_0_T    (INDEX_TIMER_DF_WORK_START + i, current_settings_prt.timeout_work_df[i],                     (*p_logic_df), 2, (*p_logic_df), 3);
      static_logic_df &= ~(1u << i);
    }
      
    _AND2((*p_logic_df), 3, (*p_logic_df), 1, (*p_logic_df), 4);
    /***/

    
  }
  
  //������������, ��� ������� �� � ����� �������, �� ����� ������ ��������������
  /*
  ��� ���� � ��������� �� ��'���� � ����, � ������� �������, �� � ������� �� �������������
  ����� p_active_functions � ����� �� �� ���������� ������ ��-��, ���� �� ���� ��-��
  ������ ���� �������� ���������, ��� �������� ��������� ���� ���� �������� �������� ��, � �� ��,
  �� ����� ��������������. � �������� � ������� ����� �� ������������ �������� � �����
  p_active_functions, �� ��������� ����� ����� ����, �� ��� �� ������ ��'������� ������������ ��������
  � ����������� �� ����� ���'����� (���� ������ ������ � ����� � ������ ���������/�����������)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
  {
    //������������, ��� ������� ��
    unsigned int index_df = 0;
    switch (i)
    {
    case 0:
      {
       index_df = RANG_DF1_OUT;
        break;
      }
    case 1:
      {
        index_df = RANG_DF2_OUT;
        break;
      }
    case 2:
      {
        index_df = RANG_DF3_OUT;
        break;
      }
    case 3:
      {
        index_df = RANG_DF4_OUT;
        break;
      }
    case 4:
      {
        index_df = RANG_DF5_OUT;
        break;
      }
    case 5:
      {
        index_df = RANG_DF6_OUT;
        break;
      }
    case 6:
      {
        index_df = RANG_DF7_OUT;
        break;
      }
    case 7:
      {
        index_df = RANG_DF8_OUT;
        break;
      }
    default:
      {
        //���������� ����� ����� �� ���� � ����
        total_error_sw_fixed();
        break;
      }
    }
      
    if ((logic_df[i] & (1 << 4)) != 0 ) _SET_BIT(p_active_functions, index_df);
    else _CLEAR_BIT(p_active_functions, index_df);
  }
}
/*****************************************************/

/*****************************************************/
//����������� ������������� ������ - �� ����������� ���� ������������� �������������� �������
/*****************************************************/
inline void dt_handler(unsigned int *p_active_functions)
{
  /*
  ��������� ���� ������������ ������ ������� � state_defined_triggers
  ������� ������������ ������� � source_set_dt
  ������� �������� ������� � source_reset_dt
  */
  unsigned int state_defined_triggers = 0, source_set_dt = 0, source_reset_dt = 0;
  for (unsigned int i = 0; i < NUMBER_DEFINED_TRIGGERS/*current_settings_prt.number_defined_dt*/; i++)
  {
    unsigned int number_byte_set = 0, number_bit_set = 0, number_byte_reset = 0, number_bit_reset = 0;
    unsigned int index_dt = 0;
    switch (i)
    {
    case 0:
      {
        number_byte_set = RANG_DT1_SET >> 5;
        number_bit_set = RANG_DT1_SET & 0x1f;

        number_byte_reset = RANG_DT1_RESET >> 5;
        number_bit_reset = RANG_DT1_RESET & 0x1f;
        
        index_dt = RANG_DT1_OUT;
        
        break;
      }
    case 1:
      {
        number_byte_set = RANG_DT2_SET >> 5;
        number_bit_set = RANG_DT2_SET & 0x1f;

        number_byte_reset = RANG_DT2_RESET >> 5;
        number_bit_reset = RANG_DT2_RESET & 0x1f;
        
        index_dt = RANG_DT2_OUT;
        
        break;
      }
    case 2:
      {
        number_byte_set = RANG_DT3_SET >> 5;
        number_bit_set = RANG_DT3_SET & 0x1f;

        number_byte_reset = RANG_DT3_RESET >> 5;
        number_bit_reset = RANG_DT3_RESET & 0x1f;
        
        index_dt = RANG_DT3_OUT;
        
        break;
      }
    case 3:
      {
        number_byte_set = RANG_DT4_SET >> 5;
        number_bit_set = RANG_DT4_SET & 0x1f;

        number_byte_reset = RANG_DT4_RESET >> 5;
        number_bit_reset = RANG_DT4_RESET & 0x1f;
        
        index_dt = RANG_DT4_OUT;
        
        break;
      }
    default:
      {
        //���������� ����� ����� �� ���� � ����
        total_error_sw_fixed();
        break;
      }
    }

    state_defined_triggers |= ( (p_active_functions[index_dt >> 5] & ( 1 << (index_dt & 0x1f) ) ) >> (index_dt & 0x1f) ) << i;
    /***
    ������� ������������ � �������� ��
    ***/
    source_set_dt   |= ((p_active_functions[number_byte_set  ] & (1 << number_bit_set  ) ) >> number_bit_set   ) << i;
    source_reset_dt |= ((p_active_functions[number_byte_reset] & (1 << number_bit_reset) ) >> number_bit_reset ) << i;
    /***/
  }
  
  //���������, �� ��������������/���������� ���������� ������ ����� ��� �������� �������-�������
  for (unsigned int i = 0; i < NUMBER_DEFINED_TRIGGERS/*current_settings_prt.number_defined_dt*/; i++)
  {
    int flag=0;
    for(int m=0; m<N_BIG; m++) if(current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i    +m] !=0) {flag=1; break;}
    if (flag)
    {
      //�������, ���� ������� ��������� �� ������� ������ �������
      flag=0;
      for(int m=0; m<N_BIG; m++) if(( current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i    +m] & p_active_functions[m]) != 0) {flag=1; break;}
      if(flag)
      {
        source_set_dt |= (1 << i);
      }
    }

    flag=0;
    for(int m=0; m<N_BIG; m++) if(current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i    +m] !=0) {flag=1; break;}
    if (flag)
    {
      //�������, ���� ������� ��������� �� ������� ��������� �������
      flag=0;
      for(int m=0; m<N_BIG; m++) if( ( current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i    +m] & ((unsigned int)(~p_active_functions[m])) ) != 0 ) {flag=1; break;}
      if(flag)
      {
        source_set_dt |= (1<< i);
      }
    }

    flag=0;
    for(int m=0; m<N_BIG; m++) if(current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i    +m] !=0) {flag=1; break;}
    if (flag)
    {
      //�������, ���� ������� ��������� �� ������� ������ �������
      flag=0;
      for(int m=0; m<N_BIG; m++) if(( current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i    +m] & p_active_functions[m]) != 0) {flag=1; break;}
      if(flag)
      {
        source_reset_dt |= (1 << i);
      }
    }

    flag=0;
    for(int m=0; m<N_BIG; m++) if(current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i    +m] !=0) {flag=1; break;}
    if (flag)
    {
      //�������, ���� ������� ��������� �� ������� ��������� �������
      flag=0;
      for(int m=0; m<N_BIG; m++) if( ( current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i    +m] & ((unsigned int)(~p_active_functions[m])) ) != 0 ) {flag=1; break;}
      if(flag)
      {
        source_reset_dt |= (1<< i);
      }
    }

    //��������� � ������ ������� ����� ������ ������������� ������ (������������ �� ��������)
    if ((source_set_dt   & (1 << i)) != 0) state_defined_triggers |= (1 << i);
    if ((source_reset_dt & (1 << i)) != 0) state_defined_triggers &= (unsigned int)(~(1 << i));
  }

  //������������, ��� ������� �� � ����� �������, �� ����� ������ ��������������
  /*
  ��� ���� � ��������� �� ��'���� � ����, � ������� �������, �� � ������� �� �������������
  ����� p_active_functions � ����� �� �� ���������� ��� �������� ������� ��-��, ���� �� ���� ��-�
  ������ ���� �������� ������������/��������, ��� �������� ������������/�������� ���� ���� �������� �������� ��, � �� ��,
  �� ����� ��������������. � �������� � �� ������������ �������� � �����
  p_active_functions, �� ��������� ����� ����� ����, �� ��� �� ������ ��'������� ������������ ��������
  � ����������� �� ����� ���'����� (���� ������ ������ � ����� � ������ ������������/��������)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
  {
    //������������, ��� ������� ��
    unsigned int index_dt = 0;
    switch (i)
    {
    case 0:
      {
       index_dt = RANG_DT1_OUT;
        break;
      }
    case 1:
      {
       index_dt = RANG_DT2_OUT;
        break;
      }
    case 2:
      {
       index_dt = RANG_DT3_OUT;
        break;
      }
    case 3:
      {
       index_dt = RANG_DT4_OUT;
        break;
      }
    default:
      {
        //���������� ����� ����� �� ���� � ����
        total_error_sw_fixed();
        break;
      }
    }
      
    if ((state_defined_triggers & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_dt);
    else _CLEAR_BIT(p_active_functions, index_dt);
  }
}
/*****************************************************/

/*****************************************************/
//����������� ������������ "�"
/*****************************************************/
inline void d_and_handler(unsigned int *p_active_functions)
{
  unsigned int state_defined_and = 0;

  //��������� ���� ��� ������������ "�" (�� ����������� ���� �� �� � �������� �����)
  for (unsigned int i = 0; i < NUMBER_DEFINED_AND/*current_settings_prt.number_defined_and*/; i++)
  {
    int flag=1;
    for(int m=0; m<N_BIG; m++) if((current_settings_prt.ranguvannja_d_and[N_BIG*i + m] & p_active_functions[m]) != current_settings_prt.ranguvannja_d_and[N_BIG*i + m]) {flag=0; break;}
    if (flag)
    {
      state_defined_and |= (1 << i);
    }
  }

  //������������, ��� ������� ���������� "�" � ����� �������, �� ����� ������ ��������������
  /*
  ��� ���� � ��������� �� ��'���� � ����, � ������� �������, �� � ������� �� �������������
  ����� p_active_functions � ����� �� �� ���������� ��� �������� ������ �-"�", ���� �� ���� �-"�"
  ������ ���� �������� , ��� �������� ���� ���� �������� �������� �-"�", � �� ��,
  �� ����� ��������������. � �������� �� ������������ �������� � �����
  p_active_functions, �� ��������� ����� ����� ����, �� ��� �� ������ ��'������� ������������ ��������
  � ����������� �� ����� ���'����� (���� ������ ������ � ����� � ������ ������������/��������)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
  {
    //������������, ��� ������� �-"�"
    unsigned int index_d_and = RANG_D_AND1 + i;
      
    if ((state_defined_and & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_and);
    else _CLEAR_BIT(p_active_functions, index_d_and);
  }
}
/*****************************************************/

/*****************************************************/
//����������� ������������ "���"
/*****************************************************/
inline void d_or_handler(unsigned int *p_active_functions)
{
  unsigned int state_defined_or = 0;

  //��������� ���� ��� ������������ "���" (�� ����������� ���� �� �� � �������� �����)
  for (unsigned int i = 0; i < NUMBER_DEFINED_OR/*current_settings_prt.number_defined_or*/; i++)
  {
    int flag=0;
    for(int m=0; m<N_BIG; m++) if((current_settings_prt.ranguvannja_d_or[N_BIG*i + m] & p_active_functions[m]) != 0) {flag=1; break;}
    if (flag)
    {
      state_defined_or |= (1 << i);
    }
  }

  //������������, ��� ������� ���������� "���" � ����� �������, �� ����� ������ ��������������
  /*
  ��� ���� � ��������� �� ��'���� � ����, � ������� �������, �� � ������� �� �������������
  ����� p_active_functions � ����� �� �� ���������� ��� �������� ������ �-"���", ���� �� ���� �-"���"
  ������ ���� �������� , ��� �������� ���� ���� �������� �������� �-"���", � �� ��,
  �� ����� ��������������. � �������� �� ������������ �������� � �����
  p_active_functions, �� ��������� ����� ����� ����, �� ��� �� ������ ��'������� ������������ ��������
  � ����������� �� ����� ���'����� (���� ������ ������ � ����� � ������ ������������/��������)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
  {
    //������������, ��� ������� �-"���"
    unsigned int index_d_or = RANG_D_OR1 + i;
      
    if ((state_defined_or & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_or);
    else _CLEAR_BIT(p_active_functions, index_d_or);
  }
}
/*****************************************************/

/*****************************************************/
//����������� ������������ "����.���"
/*****************************************************/
inline void d_xor_handler(unsigned int *p_active_functions)
{
  unsigned int state_defined_xor = 0;

  //��������� ���� ��� ������������ "����.���" (�� ����������� ���� �� �� � �������� �����)
  for (unsigned int i = 0; i < NUMBER_DEFINED_XOR/*current_settings_prt.number_defined_xor*/; i++)
  {
    unsigned int temp_array[N_BIG];
    for(int m=0; m<N_BIG; m++) temp_array[m] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + m] & p_active_functions[m];
    
    int flag=0;
    for(int m=0; m<N_BIG; m++) if(temp_array[m] != 0) {flag=1; break;}
    if (flag)
    {
      unsigned int signals = 0;
      for (unsigned int j = 0; j < N_BIG; j++)
      {
        if (temp_array[j] == 0) continue; 
        else
        {
          for (unsigned int k = 0; k < 32; k++)
          {
            if ((temp_array[j] & (1 << k)) != 0) 
            {
              signals++;
              if (signals > 1) break;
            }
          }
          if (signals > 1) break;
        }
      }
      if (signals == 1) state_defined_xor |= (1 << i);
    }
  }

  //������������, ��� ������� ���������� "����.���" � ����� �������, �� ����� ������ ��������������
  /*
  ��� ���� � ��������� �� ��'���� � ����, � ������� �������, �� � ������� �� �������������
  ����� p_active_functions � ����� �� �� ���������� ��� �������� ������ �-"����.���", ���� �� ���� �-"����.���"
  ������ ���� �������� , ��� �������� ���� ���� �������� �������� �-"����.���", � �� ��,
  �� ����� ��������������. � �������� �� ������������ �������� � �����
  p_active_functions, �� ��������� ����� ����� ����, �� ��� �� ������ ��'������� ������������ ��������
  � ����������� �� ����� ���'����� (���� ������ ������ � ����� � ������ ������������/��������)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
  {
    //������������, ��� ������� �-"����.���"
    unsigned int index_d_xor = RANG_D_XOR1 + i;
      
    if ((state_defined_xor & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_xor);
    else _CLEAR_BIT(p_active_functions, index_d_xor);
  }
}
/*****************************************************/

/*****************************************************/
//����������� ������������ "��"
/*****************************************************/
inline void d_not_handler(unsigned int *p_active_functions)
{
  unsigned int state_defined_not = 0;

  //��������� ���� ��� ������������ "��" (�� ����������� ���� �� �� � �������� �����)
  for (unsigned int i = 0; i < NUMBER_DEFINED_NOT/*current_settings_prt.number_defined_not*/; i++)
  {
    int flag=1;
    for(int m=0; m<N_BIG; m++) if((current_settings_prt.ranguvannja_d_not[N_BIG*i + m] & p_active_functions[m]) != 0) {flag=0; break;}
    if (flag)
    {
      state_defined_not |= (1 << i);
    }
  }

  //������������, ��� ������� ���������� "��" � ����� �������, �� ����� ������ ��������������
  /*
  ��� ���� � ��������� �� ��'���� � ����, � ������� �������, �� � ������� �� �������������
  ����� p_active_functions � ����� �� �� ���������� ��� �������� ������ �-"��", ���� �� ���� �-"��"
  ������ ���� �������� , ��� �������� ���� ���� �������� �������� �-"��", � �� ��,
  �� ����� ��������������. � �������� �� ������������ �������� � �����
  p_active_functions, �� ��������� ����� ����� ����, �� ��� �� ������ ��'������� ������������ ��������
  � ����������� �� ����� ���'����� (���� ������ ������ � ����� � ������ ������������/��������)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
  {
    //������������, ��� ������� �-"��"
    unsigned int index_d_not = RANG_D_NOT1 + i;
      
    if ((state_defined_not & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_not);
    else _CLEAR_BIT(p_active_functions, index_d_not);
  }
}
/*****************************************************/

/*****************************************************/
//����������� ������������� �������
/*****************************************************/
inline void tf_handler(unsigned int *p_previous_active_functions, unsigned int *p_active_functions)
{
  for (size_t i = 0; i < NUMBER_TRANSFER_FUNCTIONS; i++)
  {
    uint32_t value = current_settings_prt.ranguvannja_tf[i];
    uint32_t value_input = value & 0xffff;
    if (value_input > 0)
    {
      value_input--;
      if (value_input < NUMBER_TOTAL_SIGNAL_FOR_RANG)
      {
        if (_CHECK_SET_BIT(p_previous_active_functions, value_input) != 0)
        {
          uint32_t value_output = (value >> 16) & 0xffff;
          if (value_output > 0)
          {
            value_output--;
            if (value_output < NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL)
            {
              _SET_BIT(p_active_functions, small_big_rang[value_output]);
            }
            else
            {
              //³������� ����������� �������, ���� ����� ��� �� ����������������
              total_error_sw_fixed();
            }
          }
        }
      }
      else
      {
        //³������� ����������� �������, ���� ����� ��� �� ����������������
        total_error_sw_fixed();
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//������������� ������ ��������  ��� ������� ���2 (����������� �������)
/*
-------------------------------------------------
�(c) = K*T�/( ((I/I�)^alpha) - 1 )
-------------------------------------------------
*/
/*****************************************************/
inline int timeout_dependent_general(unsigned int i, unsigned int setpoint_mtz_2, int timeout_mtz_2, int type_mtz2_tmp)
{
  int timeout_result = 0;
  
  if (
      (type_mtz2_tmp >= TYPE_MTZ_DEPENDENT_A) &&
      (type_mtz2_tmp <= TYPE_MTZ_DEPENDENT_RTV_I)
     )   
  {
    unsigned int Iust = setpoint_mtz_2;
    if (i > Iust)
    {
      register float timeout = (((float)timeout_mtz_2)/1000.0f);
      register float timeout_dependent = 0;
      
      if (
          (type_mtz2_tmp >= TYPE_MTZ_DEPENDENT_A) &&
          (type_mtz2_tmp <= TYPE_MTZ_DEPENDENT_C)
         )   
      {
        register float K = 0.0f;
        register float alpha = 0.0f;

        switch (type_mtz2_tmp)
        {
          case TYPE_MTZ_DEPENDENT_A:
          {
            K = 0.14f;
            alpha = 0.02f;
            break;
          }
        case TYPE_MTZ_DEPENDENT_B:
          {
            K = 13.5f;
            alpha = 1.0f;
            break;
          }
        case TYPE_MTZ_DEPENDENT_C:
          {
            K = 80.0f;
            alpha = 2.0f;
            break;
          }
        default:
          {
            //���������� ����� ����� �� ���� � ����
            total_error_sw_fixed();
          }
        }
    
        register float I_div_Iusy = ((float)i)/((float)Iust);
        register float rising_to_power = powf(I_div_Iusy, alpha);
        if (
            (isnan(rising_to_power) != 0) || 
            (isinf(rising_to_power) != 0) ||
            (rising_to_power <= 1)
          )
        {
          //���������� ����� ����� �� ���� � ����
          total_error_sw_fixed();
        }
  
        timeout_dependent = K*timeout/(rising_to_power - 1);
      }
      else if (
               (type_mtz2_tmp >= TYPE_MTZ_DEPENDENT_RT_80) &&
               (type_mtz2_tmp <= TYPE_MTZ_DEPENDENT_RTV_I)
              )   
      {
        register float I_div_Iusy = ((float)i)/((float)Iust);
        I_div_Iusy = I_div_Iusy - 1.0f;

        register float K = 0.0f;
        register float alpha = 0.0f;

        switch (type_mtz2_tmp)
        {
        case TYPE_MTZ_DEPENDENT_RT_80:
          {
            K = 20.0f;
            alpha = 1.8f;
          
            I_div_Iusy /= 6.0f;
            break;
          }
        case TYPE_MTZ_DEPENDENT_RTV_I:
          {
            K = 30.0f;
            alpha = 3.0f;
            break;
          } 
        default:
          {
            //���������� ����� ����� �� ���� � ����
            total_error_sw_fixed();
          }
        }
      
        register float rising_to_power = powf(I_div_Iusy, alpha);
        if (
            (isnan(rising_to_power) != 0) || 
            (isinf(rising_to_power) != 0) ||
            (rising_to_power <= 0)
           )
        {
          //���������� ����� ����� �� ���� � ����
          total_error_sw_fixed();
        }
      
        timeout_dependent = 1.0f/(K*rising_to_power)+ timeout;
      }

      int timeout_dependent_int = (int)timeout_dependent;
      if (timeout_dependent_int > (TIMEOUT_MTZ2_MAX/1000)) timeout_dependent = (TIMEOUT_MTZ2_MAX/1000);
      else if (timeout_dependent_int < 0) timeout_dependent = 0;
    
      timeout_result = (int)(timeout_dependent*1000.0f);
    }
    else timeout_result = TIMEOUT_MTZ2_MAX;
  }
  
  return timeout_result;
}
/*****************************************************/

/*****************************************************/
// ���
/*****************************************************/
inline void mtz_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  unsigned int tmp_value;
  
  unsigned int po_mtz_x;
  unsigned int direction_ABC_tmp;
  unsigned int po_mtzn_x_vpered_setpoint;
  unsigned int po_mtzn_x_nazad_setpoint;
//  unsigned int po_block_u_mtzn_x_setpoint;
  unsigned int po_i_ncn_setpoint;
  unsigned int po_u_ncn_setpoint;
  unsigned int po_u_mtzpn_x_setpoint;
  unsigned int po_mtzpn_x_setpoint;
  
  /******������������� ����� ���������� ��� 4-� ��������*******/
  _Bool ctr_mtz_nespr_kil_napr = (current_settings_prt.control_mtz & CTR_MTZ_NESPR_KIL_NAPR) != 0; //������. ����. ���. (�)
  
  //�� I���
  po_i_ncn_setpoint = previous_state_mtz_po_incn ?
            i_nom_const * KOEF_POVERNENNJA_MTZ_I_DOWN / 100 :
            i_nom_const;
  
  previous_state_mtz_po_incn = ((measurement[IM_IA] <= po_i_ncn_setpoint)   &&
                                (measurement[IM_IB] <= po_i_ncn_setpoint) &&
                                (measurement[IM_IC] <= po_i_ncn_setpoint));
  
  //�� U���
  if ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0)
  {
    po_u_ncn_setpoint = previous_state_mtz_po_uncn ?
              u_f_nom_const * U_DOWN / 100 :
              u_f_nom_const;
  
    previous_state_mtz_po_uncn = ((measurement[IM_UA] <= po_u_ncn_setpoint) ||
                                  (measurement[IM_UB] <= po_u_ncn_setpoint) ||
                                  (measurement[IM_UC] <= po_u_ncn_setpoint));
  }
  else
  {
    po_u_ncn_setpoint = previous_state_mtz_po_uncn ?
              u_linear_nom_const * U_DOWN / 100 :
              u_linear_nom_const;
  
    previous_state_mtz_po_uncn = ((measurement[IM_UAB] <= po_u_ncn_setpoint) ||
                                  (measurement[IM_UBC] <= po_u_ncn_setpoint) ||
                                  (measurement[IM_UCA] <= po_u_ncn_setpoint));
  }
  
  ctr_mtz_nespr_kil_napr = ctr_mtz_nespr_kil_napr && previous_state_mtz_po_incn && previous_state_mtz_po_uncn; //������������� ����� ���������� (_AND3)
  
  //������������� ����� ����������
  if (ctr_mtz_nespr_kil_napr)
    _SET_BIT(p_active_functions, RANG_NCN_MTZ);
  else
    _CLEAR_BIT(p_active_functions, RANG_NCN_MTZ);
  /******������������� ����� ���������� ��� 4-� ��������*******/
  
  /******�� U ����. �����***********************/
  //������� �� U ����. ����� � ����������� ����������
  unsigned int po_block_u_mtzn_x_setpoint = (_CHECK_SET_BIT(p_active_functions, RANG_PO_BLOCK_U_MTZN) == 0) ? PORIG_CHUTLYVOSTI_VOLTAGE : PORIG_CHUTLYVOSTI_VOLTAGE * U_DOWN / 100;
  
  //�� U ����. �����
  if (
      (measurement[IM_UAB] <= po_block_u_mtzn_x_setpoint) ||
      (measurement[IM_UBC] <= po_block_u_mtzn_x_setpoint) ||
      (measurement[IM_UCA] <= po_block_u_mtzn_x_setpoint)
     )
  {
    _SET_BIT(p_active_functions, RANG_PO_BLOCK_U_MTZN);
  }
  else
  {
    _CLEAR_BIT(p_active_functions, RANG_PO_BLOCK_U_MTZN);
  }
  /******�� U ����. �����***********************/

  for (int mtz_level = 0; mtz_level < NUMBER_LEVEL_MTZ; mtz_level++) {
    //��� �������
//    if (mtz_level == 0) {
//      mtz_level = 0;
//    } else if (mtz_level == 1) {
//      mtz_level = 1;
//    } else if (mtz_level == 2) {
//      mtz_level = 2;
//    } else if (mtz_level == 3) {
//      mtz_level = 3;
//    }
    
    //�
    /*��������� ��� ���*/
    tmp_value = (*type_mtz_arr[mtz_level] == 1)                                  << 0; //������������
    tmp_value |= (*type_mtz_arr[mtz_level] == 2)                                 << 1; //� ������
    tmp_value |= (*type_mtz_arr[mtz_level] == 0)                                 << 2; //�������
    tmp_value |= (
                  (mtz_level == 1)
                  && (*type_mtz_arr[mtz_level] >= TYPE_MTZ_DEPENDENT_A) //A, B, C (3-5)
                  && (*type_mtz_arr[mtz_level] <= TYPE_MTZ_DEPENDENT_RTV_I)
                 )                                                               << 3; //��������� (���� mtz_level == 1 (2-� ������� ���))
    
    /******�� ����***********************/
    //������� �� ���� � ������ �����������
    po_mtz_x = (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZ]) != 0) ?
            *(setpoint_mtz[mtz_level] + number_group_stp) * KOEF_POVERNENNJA_MTZ_I_UP / 100 :
            *(setpoint_mtz[mtz_level] + number_group_stp);
    
    tmp_value |= ((measurement[IM_IA] >= po_mtz_x) ||
          (measurement[IM_IB] >= po_mtz_x) ||
          (measurement[IM_IC] >= po_mtz_x)) << 4; //�� ����
    /******�� ����***********************/
    
    //�
    tmp_value |= ((current_settings_prt.control_mtz & mtz_const_menu_settings_prt[mtz_level][CTR_MTZ]) != 0) << 5; //���� ���.
    //��
    tmp_value |= (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_BLOCK_MTZ]) == 0) << 6; //���������� ����
    //�
    tmp_value |= ((current_settings_prt.control_mtz & mtz_const_menu_settings_prt[mtz_level][CTR_MTZ_VPERED]) != 0) << 7; //�����: ���. ����
    tmp_value |= ((current_settings_prt.control_mtz & mtz_const_menu_settings_prt[mtz_level][CTR_MTZ_NAZAD]) != 0) << 8; //�����: ���. ���
    
    /******������ ����� ������/�����**********/
    //�������� �������������� ��� �,�,�
    direction_ABC_tmp = (sector_directional_mtz[mtz_level][PHASE_A_INDEX] == MTZ_VPERED) << 0; //�������� �������������� ���� � ������
    direction_ABC_tmp |= (sector_directional_mtz[mtz_level][PHASE_B_INDEX] == MTZ_VPERED) << 1; //�������� �������������� ���� B ������
    direction_ABC_tmp |= (sector_directional_mtz[mtz_level][PHASE_C_INDEX] == MTZ_VPERED) << 2; //�������� �������������� ���� C ������
    
    _OR3(direction_ABC_tmp, 0, direction_ABC_tmp, 1, direction_ABC_tmp, 2, direction_ABC_tmp, 3);
    
    //������ ����� ������
    if (_GET_STATE(direction_ABC_tmp, 3))
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_SECTOR_VPERED_MTZN]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_SECTOR_VPERED_MTZN]);
    
    direction_ABC_tmp |= (sector_directional_mtz[mtz_level][PHASE_A_INDEX] == MTZ_NAZAD) << 4; //�������� �������������� ���� � �����
    direction_ABC_tmp |= (sector_directional_mtz[mtz_level][PHASE_B_INDEX] == MTZ_NAZAD) << 5; //�������� �������������� ���� B �����
    direction_ABC_tmp |= (sector_directional_mtz[mtz_level][PHASE_C_INDEX] == MTZ_NAZAD) << 6; //�������� �������������� ���� C �����
    
    _OR3(direction_ABC_tmp, 4, direction_ABC_tmp, 5, direction_ABC_tmp, 6, direction_ABC_tmp, 7);
    
    //������ ����� �����
    if (_GET_STATE(direction_ABC_tmp, 7))
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_SECTOR_NAZAD_MTZN]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_SECTOR_NAZAD_MTZN]);
    
    //������� �� ����1 ����. � ������ �����������
    po_mtzn_x_vpered_setpoint = (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_VPERED]) != 0) ?
            *(setpoint_mtz_n_vpered[mtz_level] + number_group_stp) * KOEF_POVERNENNJA_MTZ_I_UP / 100 :
            *(setpoint_mtz_n_vpered[mtz_level] + number_group_stp);
    
    direction_ABC_tmp |= (measurement[IM_IA] >= po_mtzn_x_vpered_setpoint) << 8; //���������� � �������� ���� �� ���� � (������)
    direction_ABC_tmp |= (measurement[IM_IB] >= po_mtzn_x_vpered_setpoint) << 9; //���������� � �������� ���� �� ���� B (������)
    direction_ABC_tmp |= (measurement[IM_IC] >= po_mtzn_x_vpered_setpoint) << 10; //���������� � �������� ���� �� ���� C (������)
    
    //������� �� ����1 ����. � ������ �����������
    po_mtzn_x_nazad_setpoint = (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_NAZAD]) != 0) ?
            *(setpoint_mtz_n_nazad[mtz_level] + number_group_stp) * KOEF_POVERNENNJA_MTZ_I_UP / 100 :
            *(setpoint_mtz_n_nazad[mtz_level] + number_group_stp);
    
    direction_ABC_tmp |= (measurement[IM_IA] >= po_mtzn_x_nazad_setpoint) << 11; //���������� � �������� ���� �� ���� � (�����)
    direction_ABC_tmp |= (measurement[IM_IB] >= po_mtzn_x_nazad_setpoint) << 12; //���������� � �������� ���� �� ���� B (�����)
    direction_ABC_tmp |= (measurement[IM_IC] >= po_mtzn_x_nazad_setpoint) << 13; //���������� � �������� ���� �� ���� C (�����)
    
    _AND2(direction_ABC_tmp, 8, direction_ABC_tmp, 0, direction_ABC_tmp, 14);
    _AND2(direction_ABC_tmp, 9, direction_ABC_tmp, 1, direction_ABC_tmp, 15);
    _AND2(direction_ABC_tmp, 10, direction_ABC_tmp, 2, direction_ABC_tmp, 16);
    
    _AND2(direction_ABC_tmp, 11, direction_ABC_tmp, 4, direction_ABC_tmp, 17);
    _AND2(direction_ABC_tmp, 12, direction_ABC_tmp, 5, direction_ABC_tmp, 18);
    _AND2(direction_ABC_tmp, 13, direction_ABC_tmp, 6, direction_ABC_tmp, 19);
    
    _OR3(direction_ABC_tmp, 14, direction_ABC_tmp, 15, direction_ABC_tmp, 16, tmp_value, 12);
    _OR3(direction_ABC_tmp, 17, direction_ABC_tmp, 18, direction_ABC_tmp, 19, tmp_value, 13);
    /******������ ����� ������/�����**********/
    
//    /******�� U ����. �����***********************/
//    //������� �� U ����. ����� � ������ �����������
//    po_block_u_mtzn_x_setpoint = (_CHECK_SET_BIT(p_active_functions, RANG_PO_BLOCK_U_MTZN) != 0) ?
//           PORIG_CHUTLYVOSTI_VOLTAGE :
//           PORIG_CHUTLYVOSTI_VOLTAGE * U_DOWN / 100;
//    
//    tmp_value |= ((measurement[IM_UAB] <= po_block_u_mtzn_x_setpoint) ||
//                  (measurement[IM_UBC] <= po_block_u_mtzn_x_setpoint) ||
//                  (measurement[IM_UCA] <= po_block_u_mtzn_x_setpoint)) << 14; //�� U ����. �����
//    
//    //�� U ����. �����
//    if (_GET_STATE(tmp_value, 14))
//      _SET_BIT(p_active_functions, RANG_PO_BLOCK_U_MTZN);
//    else
//      _CLEAR_BIT(p_active_functions, RANG_PO_BLOCK_U_MTZN);
//    /******�� U ����. �����***********************/
    tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_PO_BLOCK_U_MTZN) != 0) << 14;
    
    //������������� ����� ���������� ��� ������� ����
    tmp_value |= (
                   !(_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_VPERED]) != 0 ||
                     _CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_NAZAD])  != 0 ||
                     _CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZPN])     != 0)
                   &&
                   ctr_mtz_nespr_kil_napr
                 ) << 15;
    
    /******�� U ������***********************/
    po_u_mtzpn_x_setpoint = (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_U_MTZPN]) != 0) ?
            *(setpoint_mtz_U[mtz_level] + number_group_stp) * U_DOWN / 100:
            *(setpoint_mtz_U[mtz_level] + number_group_stp) ;
    
    tmp_value |= ((measurement[IM_UAB] <= po_u_mtzpn_x_setpoint) ||
                  (measurement[IM_UBC] <= po_u_mtzpn_x_setpoint) ||
                  (measurement[IM_UCA] <= po_u_mtzpn_x_setpoint)) << 16; //�� U ������
    
    //�� U ������
    if (_GET_STATE(tmp_value, 16))
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_U_MTZPN]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_U_MTZPN]);
    /******�� U ������***********************/
    
    /******�� ������***********************/
    po_mtzpn_x_setpoint = (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZPN]) != 0) ?
            *(setpoint_mtz_po_napruzi[mtz_level] + number_group_stp) * KOEF_POVERNENNJA_MTZ_I_UP / 100:
            *(setpoint_mtz_po_napruzi[mtz_level] + number_group_stp);
    
    tmp_value |= ((measurement[IM_IA] >= po_mtzpn_x_setpoint)   ||
                  (measurement[IM_IB] >= po_mtzpn_x_setpoint) ||
                  (measurement[IM_IC] >= po_mtzpn_x_setpoint)) << 17; //�� ������
    /******�� ������***********************/
    
    if (mtz_level == 1) { //������ ��� 2-�� �������
      //��
      tmp_value |= (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_BLOCK_USK_MTZ]) == 0) << 9; //���������� ��������� ��� 2
      //�
      tmp_value |= ((current_settings_prt.control_mtz & CTR_MTZ_2_PRYSKORENA) != 0) << 10; //���2 ����������
      //��
      tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_STATE_VV) != 0) << 18; //��������� ��
      //M
      tmp_value |= ((current_settings_prt.control_mtz & CTR_MTZ_2_PRYSKORENNJA) != 0) << 11; //��������� ���2 ���.
    }
    
    if (_GET_STATE(tmp_value, 15)) { //���� ������������� ������������� ����� ���������� ��� ������� ����
      _CLEAR_STATE(tmp_value, 16); //������� �� U ������
    }
    
    _OR2_INVERTOR(tmp_value, 14, tmp_value, 15, tmp_value, 14);
    
    if (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZ]) != 0) 
    {
      _SET_STATE(tmp_value, 19);
    } 
    else 
    {
      if (mtz_level == 1)
      {
        //������ ��� 2-�� ������� ��� ������������ 19-�� ������� ����� �����������
        //������ "���������" (tmp_value 3)
        if (_GET_STATE(tmp_value, 14)) 
        {
          _OR3(tmp_value, 2, tmp_value, 3, tmp_value, 15, tmp_value, 19);
        }
        else 
        {
          _OR4(tmp_value, 0, tmp_value, 2, tmp_value, 3, tmp_value, 15, tmp_value, 19);
        }
      }
      else 
      {
        if (_GET_STATE(tmp_value, 14))
        {
          _OR2(tmp_value, 2, tmp_value, 15, tmp_value, 19);
        } 
        else
        {
          _OR3(tmp_value, 0, tmp_value, 2, tmp_value, 15, tmp_value, 19);
        }
      }
    }
    
    _AND2(tmp_value, 5, tmp_value, 6, tmp_value, 20);
    
    //�� ����
    _AND3(tmp_value, 19, tmp_value, 4, tmp_value, 20, tmp_value, 21);
    if (_GET_STATE(tmp_value, 21))
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZ]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZ]);
    
    //�� ����� ������
    _AND5(tmp_value, 0, tmp_value, 20, tmp_value, 7, tmp_value, 12, tmp_value, 14, tmp_value, 22);
    if (_GET_STATE(tmp_value, 22))
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_VPERED]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_VPERED]);
    
    //�� ����� �����
    _AND5(tmp_value, 0, tmp_value, 20, tmp_value, 8, tmp_value, 13, tmp_value, 14, tmp_value, 23);
    if (_GET_STATE(tmp_value, 23)) 
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_NAZAD]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_NAZAD]);
    
    //�� ������
    _AND4(tmp_value, 1, tmp_value, 20, tmp_value, 16, tmp_value, 17, tmp_value, 24);
    if (_GET_STATE(tmp_value, 24))
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZPN]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZPN]);
    
    if (mtz_level != 1) { //��� ���� �������� ����� 2-��
      _TIMER_T_0(mtz_tmr_const[mtz_level][INDEX_TIMER_MTZ], *(timeout_mtz[mtz_level] + number_group_stp), tmp_value, 21, tmp_value, 25);
      _TIMER_T_0(mtz_tmr_const[mtz_level][INDEX_TIMER_MTZ_N_VPERED], *(timeout_mtz_n_vpered[mtz_level] + number_group_stp), tmp_value, 22, tmp_value, 26);
      _TIMER_T_0(mtz_tmr_const[mtz_level][INDEX_TIMER_MTZ_N_NAZAD], *(timeout_mtz_n_nazad[mtz_level] + number_group_stp), tmp_value, 23, tmp_value, 27);
      _TIMER_T_0(mtz_tmr_const[mtz_level][INDEX_TIMER_MTZ_PO_NAPRUZI], *(timeout_mtz_po_napruzi[mtz_level] + number_group_stp), tmp_value, 24, tmp_value, 28);
      _OR4(tmp_value, 25, tmp_value, 26, tmp_value, 27, tmp_value, 28, tmp_value, 31);
    } else {
      unsigned int tmp = 0;
      _AND2(tmp_value, 18, tmp_value, 11, tmp, 0);
      _TIMER_IMPULSE(INDEX_TIMER_MTZ2_VVID_PR, current_settings_prt.timeout_mtz_2_vvid_pr[number_group_stp], temp_states_for_mtz, 0, tmp, 0, tmp, 1);
      _OR2(tmp, 1, tmp_value, 10, tmp, 2);
      _AND2(tmp_value, 9, tmp, 2, tmp, 3);
      _INVERTOR(tmp, 3, tmp, 4);
      _INVERTOR(tmp_value, 3, tmp, 23); /*��������� ������� � ��������� ��������*/
      _AND3(tmp, 4, tmp_value, 3, tmp_value, 21, tmp, 5);
      _AND2(tmp, 3, tmp_value, 21, tmp, 6);
      _AND3(tmp, 4, tmp_value, 21, tmp, 23, tmp, 7); /*������������ ������� � ��������� �������*/
      _AND2(tmp, 4, tmp_value, 22, tmp, 8);
      _AND2(tmp, 3, tmp_value, 22, tmp, 9);
      _AND2(tmp, 4, tmp_value, 23, tmp, 10);
      _AND2(tmp, 3, tmp_value, 23, tmp, 11);
      _AND2(tmp, 4, tmp_value, 24, tmp, 12);
      _AND2(tmp, 3, tmp_value, 24, tmp, 13);
      
      unsigned int i_max = measurement[IM_IA];
      if (i_max < measurement[IM_IB]) i_max = measurement[IM_IB];
      if (i_max < measurement[IM_IC]) i_max = measurement[IM_IC];
      _TIMER_T_0_LOCK(INDEX_TIMER_MTZ2_DEPENDENT, timeout_dependent_general(i_max, current_settings_prt.setpoint_mtz_2[number_group_stp], current_settings_prt.timeout_mtz_2[number_group_stp], current_settings_prt.type_mtz2), tmp, 5, p_global_trigger_state_mtz2, 0);
      _TIMER_T_0(INDEX_TIMER_MTZ2_PR, current_settings_prt.timeout_mtz_2_pr[number_group_stp], tmp, 6, tmp, 15);
      _TIMER_T_0(INDEX_TIMER_MTZ2, current_settings_prt.timeout_mtz_2[number_group_stp], tmp, 7, tmp, 16);
      _TIMER_T_0(INDEX_TIMER_MTZ2_N_VPERED, current_settings_prt.timeout_mtz_2_n_vpered[number_group_stp], tmp, 8, tmp, 17);
      _TIMER_T_0(INDEX_TIMER_MTZ2_N_VPERED_PR, current_settings_prt.timeout_mtz_2_n_vpered_pr[number_group_stp], tmp, 9, tmp, 18);
      _TIMER_T_0(INDEX_TIMER_MTZ2_N_NAZAD, current_settings_prt.timeout_mtz_2_n_nazad[number_group_stp], tmp, 10, tmp, 19);
      _TIMER_T_0(INDEX_TIMER_MTZ2_N_NAZAD_PR, current_settings_prt.timeout_mtz_2_n_nazad_pr[number_group_stp], tmp, 11, tmp, 20);
      _TIMER_T_0(INDEX_TIMER_MTZ2_PO_NAPRUZI, current_settings_prt.timeout_mtz_2_po_napruzi[number_group_stp], tmp, 12, tmp, 21);
      _TIMER_T_0(INDEX_TIMER_MTZ2_PO_NAPRUZI_PR, current_settings_prt.timeout_mtz_2_po_napruzi_pr[number_group_stp], tmp, 13, tmp, 22);
      
      _OR6(p_global_trigger_state_mtz2, 0, tmp, 15, tmp, 16, tmp, 17, tmp, 18, tmp, 19, tmp_value, 29);
      _OR3(tmp, 20, tmp, 21, tmp, 22, tmp_value, 30);
      _OR2(tmp_value, 29, tmp_value, 30, tmp_value, 31);
    }
    
    //����.����
    if (_GET_STATE(tmp_value, 31))
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_MTZ]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_MTZ]);
  }
}
/*****************************************************/

/*****************************************************/
// ���
/*****************************************************/
inline void zdz_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  
  uint32_t control_zdz_tmp = current_settings_prt.control_zdz;

#if (                                   \
       (MODYFIKACIA_VERSII_PZ == 0) ||  \
       (MODYFIKACIA_VERSII_PZ == 3) ||  \
       (MODYFIKACIA_VERSII_PZ == 4) ||  \
       (MODYFIKACIA_VERSII_PZ == 6) ||  \
       (MODYFIKACIA_VERSII_PZ == 10)||  \
       (MODYFIKACIA_VERSII_PZ == 13)||  \
       (MODYFIKACIA_VERSII_PZ == 14)    \
      )   

  static uint32_t test;
  static uint32_t swiched_on_OVD;
  static uint32_t new_OVD;

  uint32_t swiched_on_OVD_tmp = (control_zdz_tmp & ((1 << CTR_ZDZ_OVD1_STATE_BIT) | (1 << CTR_ZDZ_OVD2_STATE_BIT) | (1 << CTR_ZDZ_OVD3_STATE_BIT))) >> CTR_ZDZ_OVD1_STATE_BIT;

  new_OVD |= (swiched_on_OVD ^ swiched_on_OVD_tmp) & swiched_on_OVD_tmp;
  swiched_on_OVD = swiched_on_OVD_tmp;

  /***
  ��������, �� �� ��������� ����� � ����� �� ����� �� ������� ���� ���
  ***/
  {
    uint32_t temp_value = zdz_ovd_diagnostyka & (uint32_t)(~swiched_on_OVD);
    if (temp_value)
    {
      if (temp_value & (1 << 0))
      {
        zdz_ovd_diagnostyka &= (uint32_t)(~(1 << 0));
      
        _SET_BIT(clear_diagnostyka, TEST_OVD1);
      }
    
      if (temp_value & (1 << 1))
      {
        zdz_ovd_diagnostyka &= (uint32_t)(~(1 << 1));
      
        _SET_BIT(clear_diagnostyka, TEST_OVD2);
      }

      if (temp_value & (1 << 2))
      {
        zdz_ovd_diagnostyka &= (uint32_t)(~(1 << 2));
      
        _SET_BIT(clear_diagnostyka, TEST_OVD3);
      }
    }
  }
  /***/
   
  //������ ���� � ������������
  uint32_t light = (_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 12) & 0x7;

  /***
  ��� ��������, ���� ������� �� ���������� ������� �������
  ***/
  if (test != 0)
  {
    //���� ����������
    zdz_ovd_diagnostyka = (light ^ test) & test;
    
    if (zdz_ovd_diagnostyka & (1 << 0)) _SET_BIT(set_diagnostyka, TEST_OVD1);
    else _SET_BIT(clear_diagnostyka, TEST_OVD1);
      
    if (zdz_ovd_diagnostyka & (1 << 1)) _SET_BIT(set_diagnostyka, TEST_OVD2);
    else _SET_BIT(clear_diagnostyka, TEST_OVD2);
      
    if (zdz_ovd_diagnostyka & (1 << 2)) _SET_BIT(set_diagnostyka, TEST_OVD3);
    else _SET_BIT(clear_diagnostyka, TEST_OVD3);
      
    //���, �� ����� ������������ �� ���� �������������, �� ��� ����� ��  ���� �� ��������
    light &= (uint32_t)(~test);
    
    //������� ����
    test = 0;
  }
  
  //³��� ���� �� ����������� ��������� ����� ���
  delta_time_test += DELTA_TIME_FOR_TIMERS;
  
  //Գ������ ������� ������ ����� ��� ���
  if (
      (light == 0)
      &&  
      (
       (new_OVD) ||
       ( delta_time_test >= PERIOD_ZDZ_TEST)
      )   
     )   
  {
    new_OVD = 0; /*������� �������� �������� � ������ ������ ������*/
    delta_time_test = 0;
    
    //�������� ����
    test = swiched_on_OVD & 0x7;
  }

  if (test != 0) _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD28_DD30) = (1 << 8) | ((test & 0xf) << 12);
  else _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD28_DD30) = (((current_settings_prt.zdz_ovd_porig + 1) & 0xf) << 8);
  /***/

#endif  

  uint32_t logic_zdz_0 = 0;

  logic_zdz_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_PUSK_ZDZ_VID_DV) != 0) << 0; //���� ��� �� ��

  logic_zdz_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_ZDZ) == 0) << 1; //����. ���
  
  _AND2(control_zdz_tmp, CTR_ZDZ_STATE_BIT, logic_zdz_0, 1, logic_zdz_0, 2);
  _AND2(logic_zdz_0, 2, logic_zdz_0, 0, logic_zdz_0, 3);
  //"��.��� �� ��"
  if (_GET_STATE(logic_zdz_0, 3))
    _SET_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_DV);
  else
    _CLEAR_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_DV);

#if (                                   \
       (MODYFIKACIA_VERSII_PZ == 0) ||  \
       (MODYFIKACIA_VERSII_PZ == 3) ||  \
       (MODYFIKACIA_VERSII_PZ == 4) ||  \
       (MODYFIKACIA_VERSII_PZ == 6) ||  \
       (MODYFIKACIA_VERSII_PZ == 10)||  \
       (MODYFIKACIA_VERSII_PZ == 13)||  \
       (MODYFIKACIA_VERSII_PZ == 14)    \
      ) 

  uint32_t zdz_ovd_diagnostyka_inv = (uint32_t)(~zdz_ovd_diagnostyka);
  _AND4(logic_zdz_0, 2, light, 0, zdz_ovd_diagnostyka_inv, 0, control_zdz_tmp, CTR_ZDZ_OVD1_STATE_BIT, logic_zdz_0, 4);
  //"��.��� �� ���1"
  if (_GET_STATE(logic_zdz_0, 4))
    _SET_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_OVD1);
  else
    _CLEAR_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_OVD1);

  _AND4(logic_zdz_0, 2, light, 1, zdz_ovd_diagnostyka_inv, 1, control_zdz_tmp, CTR_ZDZ_OVD2_STATE_BIT, logic_zdz_0, 5);
  //"��.��� �� ���2"
  if (_GET_STATE(logic_zdz_0, 5))
    _SET_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_OVD2);
  else
    _CLEAR_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_OVD2);

  _AND4(logic_zdz_0, 2, light, 2, zdz_ovd_diagnostyka_inv, 2, control_zdz_tmp, CTR_ZDZ_OVD3_STATE_BIT, logic_zdz_0, 6);
  //"��.��� �� ���3"
  if (_GET_STATE(logic_zdz_0, 6))
    _SET_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_OVD3);
  else
    _CLEAR_BIT(p_active_functions, RANG_LIGHT_ZDZ_FROM_OVD3);

  _OR4(logic_zdz_0, 3, logic_zdz_0, 4, logic_zdz_0, 5, logic_zdz_0, 6, logic_zdz_0, 7);
#else
  if (_GET_STATE(logic_zdz_0, 3)) _SET_STATE(logic_zdz_0, 7);
#endif
  
  uint32_t logic_zdz_rez = 0;
  if (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_NONE)
  {
    logic_zdz_rez |= _GET_STATE(logic_zdz_0, 7) << 0;
  }
  else if (current_settings_prt.ctrl_zdz_type < _ZDZ_CTRL_NUMBER)
  {
    if (
        (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I) ||
        (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I_OR_U) ||
        (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I_AND_U)
       )   
    {
      if(
         (_GET_STATE(control_zdz_tmp, (CTR_ZDZ_STARTED_FROM_MTZ1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))))
         &&
         (
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZ1) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN1_VPERED) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN1_NAZAD) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZPN1) != 0)
         ) 
        )   
      {
        _SET_STATE(logic_zdz_0, 8);
      }

      if(
         (_GET_STATE(control_zdz_tmp, (CTR_ZDZ_STARTED_FROM_MTZ2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))))
         &&
         (
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZ2) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN2_VPERED) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN2_NAZAD) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZPN2) != 0)
         ) 
        )   
      {
        _SET_STATE(logic_zdz_0, 9);
      }

      if(
         (_GET_STATE(control_zdz_tmp, (CTR_ZDZ_STARTED_FROM_MTZ3_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))))
         &&
         (
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZ3) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN3_VPERED) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN3_NAZAD) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZPN3) != 0)
         ) 
        )   
      {
        _SET_STATE(logic_zdz_0, 10);
      }

      if(
         (_GET_STATE(control_zdz_tmp, (CTR_ZDZ_STARTED_FROM_MTZ4_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II))))
         &&
         (
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZ4) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN4_VPERED) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZN4_NAZAD) != 0) ||
          (_CHECK_SET_BIT(p_active_functions, RANG_PO_MTZPN4) != 0)
         ) 
        )   
      {
        _SET_STATE(logic_zdz_0, 11);
      }
      
      _OR4(logic_zdz_0, 8, logic_zdz_0, 9, logic_zdz_0, 10, logic_zdz_0, 11, logic_zdz_0, 12);
    }

    if (
        (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_U) ||
        (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I_OR_U) ||
        (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I_AND_U)
       )   
    {
      if(
         (_GET_STATE(control_zdz_tmp, (CTR_ZDZ_STARTED_FROM_UMIN1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II)))) &&
         (_CHECK_SET_BIT(p_active_functions, RANG_PO_UMIN1) != 0)
        )   
      {
        _SET_STATE(logic_zdz_0, 13);
      }
      
      if(
         (_GET_STATE(control_zdz_tmp, (CTR_ZDZ_STARTED_FROM_UMIN2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II)))) &&
         (_CHECK_SET_BIT(p_active_functions, RANG_PO_UMIN2) != 0)
        )   
      {
        _SET_STATE(logic_zdz_0, 14);
      }
      
      _OR2(logic_zdz_0, 13, logic_zdz_0, 14, logic_zdz_0, 15);
    }

    if (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I)
    {
      _AND2(logic_zdz_0, 7, logic_zdz_0, 12, logic_zdz_rez, 0);
    }
    else if (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_U)
    {
      _AND2(logic_zdz_0, 7, logic_zdz_0, 15, logic_zdz_rez, 0);
    }
    else if (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_3I0)
    {
      if(
         (_CHECK_SET_BIT(p_active_functions, RANG_PO_NZZ) != 0) ||
         (_CHECK_SET_BIT(p_active_functions, RANG_PO_3I0) != 0)
        )   
      {
        _SET_STATE(logic_zdz_0, 17);
      }
      _AND2(logic_zdz_0, 7, logic_zdz_0, 17, logic_zdz_rez, 0);
    }
    else
    {
      if (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I_OR_U)
      {
        _OR2(logic_zdz_0, 12, logic_zdz_0, 15, logic_zdz_0, 16);
      }
      else if (current_settings_prt.ctrl_zdz_type == ZDZ_CTRL_I_AND_U)
      {
        _AND2(logic_zdz_0, 12, logic_zdz_0, 15, logic_zdz_0, 16);
      }
      else
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed();
      }

      _AND2(logic_zdz_0, 7, logic_zdz_0, 16, logic_zdz_rez, 0);
    }
  }
  else
  {
    //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
    total_error_sw_fixed();
  }
  
  //"�� ���"
  if (_GET_STATE(logic_zdz_rez, 0))
    _SET_BIT(p_active_functions, RANG_PO_ZDZ);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_ZDZ);
  
  _TIMER_T_0(INDEX_TIMER_ZDZ, current_settings_prt.timeout_zdz[number_group_stp], logic_zdz_rez, 0, logic_zdz_rez, 1);
  //"���"
  if (_GET_STATE(logic_zdz_rez, 1))
    _SET_BIT(p_active_functions, RANG_ZDZ);
  else
    _CLEAR_BIT(p_active_functions, RANG_ZDZ);
}
/*****************************************************/

/*****************************************************/
// ��
/*****************************************************/
inline void zz_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  /********************************
  ������ ���

  "������ ���" ����������� ��� �������:
  
  ����� 3I0 (���������� � ������� ��������) > 5 (��)
  ����� 3U0 (���������� �         ���������) > 6 (�)
  
  *********************************/
  unsigned int porig_3I0;
  if (Nzz_3I0_bilshe_porogu == 0) porig_3I0 = PORIG_CHUTLYVOSTI_3I0*KOEF_POVERNENNJA_3I0_SECTOR_BLK/100;
  else porig_3I0 = PORIG_CHUTLYVOSTI_3I0;
  unsigned int Nzz_3I0_bilshe_porogu_tmp = Nzz_3I0_bilshe_porogu = (measurement[IM_3I0] >= porig_3I0);

  unsigned int porig_3U0;
  if (Nzz_3U0_bilshe_porogu == 0) porig_3U0 = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE*KOEF_POVERNENNJA_U_SECTOR_BLK/100;
  else porig_3U0 = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
  unsigned int Nzz_3U0_bilshe_porogu_tmp = Nzz_3U0_bilshe_porogu = (measurement[IM_3U0] >= porig_3U0);
      
  if (
      ((current_settings_prt.control_zz & CTR_ZZ1_TYPE) == 0) &&
      (Nzz_3I0_bilshe_porogu_tmp != 0) &&
      (Nzz_3U0_bilshe_porogu_tmp != 0)
     )
  {
    unsigned int sector_i_minus_u;
    if (sector_NZZ == 0)
    {
      //�������� �� ������� ����������� ��� ���
      sector_i_minus_u = sector_i_minus_u_1;
    }
    else
    {
      //�������� �� ������� ���������� ��� ���
      sector_i_minus_u = sector_i_minus_u_2;
    }
  
    if ((current_settings_prt.control_zz & CTR_ZZ1_SECTOR) != 0)
    {
      //�������� �� ������� "������" (��������� 1 � 2 ��� "��� ������")
      if ((sector_i_minus_u == 1) || (sector_i_minus_u == 2))
      {
        /*
        ����-��� ��������� �������� ������, �� �� � ������ �����������
        */
        sector_NZZ = 0xff;
      }
      else
      {
        /*
        ������� �������� ������, �� �� �� � ������ �����������
        */
        sector_NZZ = 0;
      }
    }
    else
    {
      //�������� �� ������� "�����" (��������� 3 � 4 ��� "��� ������")
      if ((sector_i_minus_u == 3) || (sector_i_minus_u == 4))
      {
        /*
        ����-��� ��������� �������� ������, �� �� � ������ �����������
        */
        sector_NZZ = 0xff;
      }
      else
      {
        /*
        ������� �������� ������, �� �� �� � ������ �����������
        */
        sector_NZZ = 0;
      }
    }
  }
  else sector_NZZ = 0;

  
  //������������ ������ "������ ���",���� �� �������� - � ������ ������� �� ��������
  if (sector_NZZ != 0)
    _SET_BIT(p_active_functions, RANG_SECTOR_NZZ);
  else
    _CLEAR_BIT(p_active_functions, RANG_SECTOR_NZZ);
  /*******************************/

  /*******************************/
  //��������� �� ����� ���������� ������ ���������� � ���� ���������� ����������
  /*******************************/
  unsigned int max_3i0_current, koef_povernennja;
  
  if((current_settings_prt.control_zz & CTR_ZZ1_TYPE) == 0)
  {
    max_3i0_current = measurement[IM_3I0];
    koef_povernennja = KOEF_POVERNENNJA_GENERAL_UP;
  }
  else
  {
    max_3i0_current = measurement[IM_3I0_other_g];
    koef_povernennja = KOEF_POVERNENNJA_3I0_OTHER;
  }
  /*******************************/
  
  unsigned int setpoint;
  /********************************
  �� 3I0 (�� ��� ����������� ��������, ������� �����������/����������)
  ********************************/
  /*
  ������� ���������� � ��� � ������� �� ������� �������, � ������� � ��� � ���������,
  �� ������� ������ �� 10, ��� �������� �� ����� � ������� ��������
  */
  if (po_3I0 == 0)
  {
    //�������� �� ������� ����������� ��� 3I0
    setpoint = current_settings_prt.setpoint_zz_3I0[number_group_stp]*10;
  }
  else
  {
    //�������� �� ������� ���������� ��� 3I0
    /*
    ���������� ��������� ������
    setpoint = (current_settings_prt.setpoint_zz_3I0[number_group_stp]*koef_povernennja/100)*10 =  current_settings_prt.setpoint_zz_3I0[number_group_stp]*koef_povernennja/10
    */
    setpoint = current_settings_prt.setpoint_zz_3I0[number_group_stp]*koef_povernennja/10;
  }
  
  if (max_3i0_current >= setpoint)
  {
    /*
    ����-��� ��������� �������� ������, �� ��������� �������� ����-���� ������ ������������ �������
    */
    po_3I0 = 0xff;
  }
  else
  {
    /*
    ������� �������� ������, �� ��������� �������� ����� ������ ������������ �������
    */
    po_3I0 = 0;
  }
  /*******************************/

  /********************************
  �� 3U0 (�� ��� ����������� ��������, ������� �����������/����������)
  ********************************/
  if (po_3U0 == 0)
  {
    //�������� �� ������� ����������� ��� 3U0
    setpoint = current_settings_prt.setpoint_zz_3U0[number_group_stp];
  }
  else
  {
    //�������� �� ������� ���������� ��� 3U0
    setpoint = current_settings_prt.setpoint_zz_3U0[number_group_stp]*KOEF_POVERNENNJA_3U0/100;
  }
  
  if (measurement[IM_3U0] >= setpoint)
  {
    /*
    ����-��� ��������� �������� ������, �� ��������� �������� ����-���� ������ ������������ �������
    */
    po_3U0 = 0xff;
  }
  else
  {
    /*
    ������� �������� ������, �� ��������� �������� ����� ������ ������������ �������
    */
    po_3U0 = 0;
  }
  /*******************************/
  
  if (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_NZZ) == 0)
  {
    //���, ��/3I0 � ��/3U0 �� ��������� � ����������� �����
    /*******************************/
    //���
    /*******************************/
    if (
        ((current_settings_prt.control_zz & CTR_ZZ1_NZZ_STATE) != 0) &&
        ((current_settings_prt.control_zz & CTR_ZZ1_TYPE) == 0)
       )   
    {
      //1 ������ ��/3U0 ��������, ��������� � �� ���������
    
      unsigned int previous_state_po_nzz = _CHECK_SET_BIT(p_active_functions, RANG_PO_NZZ);
    
      //�����������, ��� ������� ������ "�� NZZ"
      if (
          (po_3I0     != 0) &&
          (po_3U0     != 0) &&
          (sector_NZZ != 0)
         )
      {
        //���� ����� ������ �� ���
        if(previous_state_po_nzz == 0)
        {
          //������������ ������ "�� NZZ"
          _SET_BIT(p_active_functions, RANG_PO_NZZ);
      
          //��������� ������ ���, ���� �� �� �� ���������
          global_timers[INDEX_TIMER_NZZ] = 0;
        }
      }
      else 
      {
        //�� ���� ����� ������ �� ���
        if(previous_state_po_nzz != 0)
        {
          //������� ������ "�� ���"
          _CLEAR_BIT(p_active_functions, RANG_PO_NZZ);
          //�� � ������ ����� �������� ������� "����. ���"
          _CLEAR_BIT(p_active_functions, RANG_NZZ);
          //���� ������ �� �� ��������, �� ������� ����
          if ( global_timers[INDEX_TIMER_NZZ] >=0) global_timers[INDEX_TIMER_NZZ] = -1;
        }
      }
    
      if(global_timers[INDEX_TIMER_NZZ] >= current_settings_prt.timeout_nzz[number_group_stp])
      {
        //���� ��������� "�������� ���" �� ������������ ������ "����. ���"
        _SET_BIT(p_active_functions, RANG_NZZ);

        //������� ������ �� ���
        global_timers[INDEX_TIMER_NZZ] = -1;
      }
    }
    else
    {
      //���� ��� �� �����������, �� ����� ������� �� ������� � �������, �� �� �� ����������
      _CLEAR_BIT(p_active_functions, RANG_PO_NZZ);
      _CLEAR_BIT(p_active_functions, RANG_NZZ);
      global_timers[INDEX_TIMER_NZZ] = -1;
    }  
    /*******************************/

    /*******************************/
    //1 ������ ��/3U0
    /*******************************/
    if ((current_settings_prt.control_zz & CTR_ZZ1_3U0_STATE) != 0)
    {
      //1 ������ ��/3U0 �������� � �� ���������
    
      unsigned int previous_state_po_3U0 = _CHECK_SET_BIT(p_active_functions, RANG_PO_3U0);
    
      //�����������, ��� ������� ������ "�� ��/3U0"
      if (po_3U0 != 0)
      {
        //����������� ������� 3U0 �������� ���� �������
        if(previous_state_po_3U0 == 0)
        {
          //������������ ������ "�� ��/3U0"
          _SET_BIT(p_active_functions, RANG_PO_3U0);
        
          //��������� ������ ��/3U0, ���� �� �� �� ���������
          global_timers[INDEX_TIMER_ZZ_3U0] = 0;
        }
      }
      else 
      {
        //����������� ������� 3I0 ����� �������
        if(previous_state_po_3U0 != 0)
        {
          //������� ������ "�� ��/3U0"
          _CLEAR_BIT(p_active_functions, RANG_PO_3U0);
          //�� � ������ ����� �������� ������� "����. ��/3U0"
          _CLEAR_BIT(p_active_functions, RANG_3U0);
          //���� ������ �� �� ��������, �� ������� ����
          if ( global_timers[INDEX_TIMER_ZZ_3U0] >=0) global_timers[INDEX_TIMER_ZZ_3U0] = -1;
        }
      }
    
      if(global_timers[INDEX_TIMER_ZZ_3U0] >= current_settings_prt.timeout_zz_3U0[number_group_stp])
      {
        //���� ��������� "�������� ��/3U0" �� ������������ ������ "����. ��/3U0"
        _SET_BIT(p_active_functions, RANG_3U0);

        //������� ������ �� ��/3U0
        global_timers[INDEX_TIMER_ZZ_3U0] = -1;
      }
    }
    else
    {
      //���� 1 ������ ��/3U0 �� �����������, �� ����� ������� �� ������� � �������, �� �� �� ����������
      _CLEAR_BIT(p_active_functions, RANG_PO_3U0);
      _CLEAR_BIT(p_active_functions, RANG_3U0);
      global_timers[INDEX_TIMER_ZZ_3U0] = -1;
    }    
    /*******************************/
  
    /*******************************/
    //1 ������ ��/3I0
    /*******************************/
    if ((current_settings_prt.control_zz & CTR_ZZ1_3I0_STATE) != 0)
    {
      //1 ������ ��/3I0 �������� � �� ���������
    
      unsigned int previous_state_po_3I0 = _CHECK_SET_BIT(p_active_functions, RANG_PO_3I0);
    
      //�����������, ��� ������� ������ "�� ��/3I0"
      if (po_3I0 != 0)
      {
        //������������ ���� 3I0 �������� ���� �������
        if(previous_state_po_3I0 == 0)
        {
          //������������ ������ "�� ��/3I0"
          _SET_BIT(p_active_functions, RANG_PO_3I0);
      
          //��������� ������ ��/3I0, ���� �� �� �� ���������
          global_timers[INDEX_TIMER_ZZ_3I0] = 0;
        }
      }
      else 
      {
        //������������ ���� 3I0 ����� �������
        if(previous_state_po_3I0 != 0)
        {
          //������� ������ "�� ��/3I0"
          _CLEAR_BIT(p_active_functions, RANG_PO_3I0);
          //�� � ������ ����� �������� ������� "����. ��/3I0"
          _CLEAR_BIT(p_active_functions, RANG_3I0);
          //���� ������ �� �� ��������, �� ������� ����
          if ( global_timers[INDEX_TIMER_ZZ_3I0] >=0) global_timers[INDEX_TIMER_ZZ_3I0] = -1;
        }
      }
    
      if(global_timers[INDEX_TIMER_ZZ_3I0] >= current_settings_prt.timeout_zz_3I0[number_group_stp])
      {
        //���� ��������� "�������� ��/3I0" �� ������������ ������ "����. ��"
        _SET_BIT(p_active_functions, RANG_3I0);

        //������� ������ �� ��/3I0
        global_timers[INDEX_TIMER_ZZ_3I0] = -1;
      }
    }
    else
    {
      //���� 1 ������ ��/3I0 �� �����������, �� ����� ������� �� ������� � �������, �� �� �� ����������
      _CLEAR_BIT(p_active_functions, RANG_PO_3I0);
      _CLEAR_BIT(p_active_functions, RANG_3I0);
      global_timers[INDEX_TIMER_ZZ_3I0] = -1;
    }  
    /*******************************/
  }
  else
  {
    //������� ��/��� ��������� � ����������� �����, �� ����� ������� �� ������� � �������, �� �� ��� ����������
    _CLEAR_BIT(p_active_functions, RANG_PO_NZZ);
    _CLEAR_BIT(p_active_functions, RANG_NZZ);
    _CLEAR_BIT(p_active_functions, RANG_PO_3I0);
    _CLEAR_BIT(p_active_functions, RANG_3I0);
    _CLEAR_BIT(p_active_functions, RANG_PO_3U0);
    _CLEAR_BIT(p_active_functions, RANG_3U0);

    global_timers[INDEX_TIMER_NZZ   ] = -1;
    global_timers[INDEX_TIMER_ZZ_3U0] = -1;
    global_timers[INDEX_TIMER_ZZ_3I0] = -1;
  }  
}
/*****************************************************/

/*****************************************************/
//����
/*****************************************************/
inline void tznp_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
#define CTR_TZNP                CTR_TZNP1      
#define CTR_TZNP_VPERED         CTR_TZNP1_VPERED      
#define CTR_TZNP_NAZAD          CTR_TZNP1_NAZAD      

#define RANG_BLOCK_TZNP         RANG_BLOCK_TZNP1
#define RANG_SECTOR_TZNP_VPERED RANG_SECTOR_TZNP1_VPERED
#define RANG_SECTOR_TZNP_NAZAD  RANG_SECTOR_TZNP1_NAZAD
#define RANG_PO_3I0_TZNP_VPERED RANG_PO_3I0_TZNP1_VPERED
#define RANG_PO_3I0_TZNP_NAZAD  RANG_PO_3I0_TZNP1_NAZAD
#define RANG_PO_3U0_TZNP_VPERED RANG_PO_3U0_TZNP1_VPERED
#define RANG_PO_3U0_TZNP_NAZAD  RANG_PO_3U0_TZNP1_NAZAD
#define RANG_PO_TZNP_VPERED     RANG_PO_TZNP1_VPERED
#define RANG_PO_TZNP_NAZAD      RANG_PO_TZNP1_NAZAD
#define RANG_TZNP               RANG_TZNP1
  
  unsigned int maska_ctrl_bits = CTR_TZNP | CTR_TZNP_VPERED | CTR_TZNP_NAZAD;
  
  for (unsigned int tznp = 0; tznp < 3; tznp++)
  {
    unsigned int setpoint_tznp_3I0_vpered = 0;
    unsigned int setpoint_tznp_3U0_vpered = 0;
    unsigned int setpoint_tznp_3I0_nazad = 0;
    unsigned int setpoint_tznp_3U0_nazad = 0;

    int timeout_tznp_vpered = 0;
    int timeout_tznp_nazad = 0;
    
    unsigned int index_timer_vpered = 0;
    unsigned int index_timer_nazad = 0;
    
    unsigned int control_tznp = 0;
    
    unsigned int shift_to_base_rang_index = 0;
    
    switch(tznp)
    {
    case 0:
      {
        setpoint_tznp_3I0_vpered = current_settings_prt.setpoint_tznp_1_3I0_vpered[number_group_stp];
        setpoint_tznp_3U0_vpered = current_settings_prt.setpoint_tznp_1_3U0_vpered[number_group_stp];
        setpoint_tznp_3I0_nazad  = current_settings_prt.setpoint_tznp_1_3I0_nazad[number_group_stp];
        setpoint_tznp_3U0_nazad  = current_settings_prt.setpoint_tznp_1_3U0_nazad[number_group_stp];
        
        timeout_tznp_vpered = current_settings_prt.timeout_tznp_1_vpered[number_group_stp];
        timeout_tznp_nazad  = current_settings_prt.timeout_tznp_1_nazad[number_group_stp]; 
        
        index_timer_vpered = INDEX_TIMER_TZNP1_VPERED; 
        index_timer_nazad  = INDEX_TIMER_TZNP1_NAZAD;
        
        control_tznp = (current_settings_prt.control_tznp >> INDEX_ML_CTR_TZNP1) & maska_ctrl_bits;
        
        shift_to_base_rang_index = RANG_BLOCK_TZNP1 - RANG_BLOCK_TZNP;
        
        break;
      }
    case 1:
      {
        setpoint_tznp_3I0_vpered = current_settings_prt.setpoint_tznp_2_3I0_vpered[number_group_stp];
        setpoint_tznp_3U0_vpered = current_settings_prt.setpoint_tznp_2_3U0_vpered[number_group_stp];
        setpoint_tznp_3I0_nazad  = current_settings_prt.setpoint_tznp_2_3I0_nazad[number_group_stp];
        setpoint_tznp_3U0_nazad  = current_settings_prt.setpoint_tznp_2_3U0_nazad[number_group_stp];
        
        timeout_tznp_vpered = current_settings_prt.timeout_tznp_2_vpered[number_group_stp];
        timeout_tznp_nazad  = current_settings_prt.timeout_tznp_2_nazad[number_group_stp]; 
        
        index_timer_vpered = INDEX_TIMER_TZNP2_VPERED; 
        index_timer_nazad  = INDEX_TIMER_TZNP2_NAZAD;
        
        control_tznp = (current_settings_prt.control_tznp >> INDEX_ML_CTR_TZNP2) & maska_ctrl_bits;
        
        shift_to_base_rang_index = RANG_BLOCK_TZNP2 - RANG_BLOCK_TZNP;
        
        break;
      }
    case 2:
      {
        setpoint_tznp_3I0_vpered = current_settings_prt.setpoint_tznp_3_3I0_vpered[number_group_stp];
        setpoint_tznp_3U0_vpered = current_settings_prt.setpoint_tznp_3_3U0_vpered[number_group_stp];
        setpoint_tznp_3I0_nazad  = current_settings_prt.setpoint_tznp_3_3I0_nazad[number_group_stp];
        setpoint_tznp_3U0_nazad  = current_settings_prt.setpoint_tznp_3_3U0_nazad[number_group_stp];
        
        timeout_tznp_vpered = current_settings_prt.timeout_tznp_3_vpered[number_group_stp];
        timeout_tznp_nazad  = current_settings_prt.timeout_tznp_3_nazad[number_group_stp]; 
        
        index_timer_vpered = INDEX_TIMER_TZNP3_VPERED; 
        index_timer_nazad  = INDEX_TIMER_TZNP3_NAZAD;
        
        control_tznp = (current_settings_prt.control_tznp >> INDEX_ML_CTR_TZNP3) & maska_ctrl_bits;
        
        shift_to_base_rang_index = RANG_BLOCK_TZNP3 - RANG_BLOCK_TZNP;
        
        break;
      }
    default:
      {
        //���������� ����� ����� �� ���� � ����
        total_error_sw_fixed();
      }
    }

    unsigned int logic_TZNP_0 = 0;
    unsigned int setpoint;
    
    /***
    ������ ������
    ***/
    if(sector_directional_tznp[tznp] == 1)
    {
      logic_TZNP_0 |= 1 << 0;
      _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_SECTOR_TZNP_VPERED));
    }
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_SECTOR_TZNP_VPERED));
    /***/
    
    /***
    ������ �����
    ***/
    if(sector_directional_tznp[tznp] == 2)
    {
      logic_TZNP_0 |= 1 << 1;
      _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_SECTOR_TZNP_NAZAD));
    }
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_SECTOR_TZNP_NAZAD));
    /***/
    
    /***
    �� 3I0 ������
    ***/
    if (_CHECK_SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3I0_TZNP_VPERED)) == 0)
    {
      //�������� �� ������� �����������
      setpoint = setpoint_tznp_3I0_vpered;
    }
    else
    {
      //�������� �� ������� ����������
      setpoint = setpoint_tznp_3I0_vpered*KOEF_POVERNENNJA_GENERAL_UP/100;
    }
    if (measurement[IM_3I0_r] >= setpoint)
    {
      logic_TZNP_0 |=  1 << 3;
      _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3I0_TZNP_VPERED));
    }
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3I0_TZNP_VPERED));
    /***/
    
    /***
    �� 3U0 ������
    ***/
    if (_CHECK_SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3U0_TZNP_VPERED)) == 0)
    {
      //�������� �� ������� �����������
      setpoint = setpoint_tznp_3U0_vpered;
    }
    else
    {
      //�������� �� ������� ����������
      setpoint = setpoint_tznp_3U0_vpered*KOEF_POVERNENNJA_3U0/100;
    }
    if (measurement[IM_3U0] >= setpoint)
    {
      logic_TZNP_0 |=  1 << 4;
      _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3U0_TZNP_VPERED));
    }
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3U0_TZNP_VPERED));
    /***/
    
    /***
    �� 3I0 �����
    ***/
    if (_CHECK_SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3I0_TZNP_NAZAD)) == 0)
    {
      //�������� �� ������� �����������
      setpoint = setpoint_tznp_3I0_nazad;
    }
    else
    {
      //�������� �� ������� ����������
      setpoint = setpoint_tznp_3I0_nazad*KOEF_POVERNENNJA_GENERAL_UP/100;
    }
    if (measurement[IM_3I0_r] >= setpoint)
    {
      logic_TZNP_0 |=  1 << 5;
      _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3I0_TZNP_NAZAD));
    }
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3I0_TZNP_NAZAD));
    /***/
    
    /***
    �� 3U0 �����
    ***/
    if (_CHECK_SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3U0_TZNP_NAZAD)) == 0)
    {
      //�������� �� ������� �����������
      setpoint = setpoint_tznp_3U0_nazad;
    }
    else
    {
      //�������� �� ������� ����������
      setpoint = setpoint_tznp_3U0_nazad*KOEF_POVERNENNJA_3U0/100;
    }
    if (measurement[IM_3U0] >= setpoint)
    {
      logic_TZNP_0 |=  1 << 6;
      _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3U0_TZNP_NAZAD));
    }
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_3U0_TZNP_NAZAD));
    /***/
    
    //���� ������� �������
    logic_TZNP_0 |= ((control_tznp & CTR_TZNP) != 0) << 7;
    
    //���� ���������� �������
    logic_TZNP_0 |= (_CHECK_SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_BLOCK_TZNP)) == 0) << 8;
    
    //���� �������� ������
    logic_TZNP_0 |= ((control_tznp & CTR_TZNP_VPERED) != 0) << 10;
    
    //���� �������� �����
    logic_TZNP_0 |= ((control_tznp & CTR_TZNP_NAZAD) != 0) << 11;

    _AND2(logic_TZNP_0, 7, logic_TZNP_0, 8, logic_TZNP_0, 9);

    //�� ���� ������
    _AND5(logic_TZNP_0, 9, logic_TZNP_0, 10, logic_TZNP_0, 3, logic_TZNP_0, 4, logic_TZNP_0, 0, logic_TZNP_0, 12);
    if (_GET_STATE(logic_TZNP_0, 12)) _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_TZNP_VPERED));
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_TZNP_VPERED));
      
    //�� ���� �����
    _AND5(logic_TZNP_0, 9, logic_TZNP_0, 11, logic_TZNP_0, 5, logic_TZNP_0, 6, logic_TZNP_0, 1, logic_TZNP_0, 13);
    if (_GET_STATE(logic_TZNP_0, 13)) _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_TZNP_NAZAD));
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_PO_TZNP_NAZAD));

    _TIMER_T_0(index_timer_vpered, timeout_tznp_vpered, logic_TZNP_0, 12, logic_TZNP_0, 14);
    _TIMER_T_0(index_timer_nazad,  timeout_tznp_nazad,  logic_TZNP_0, 13, logic_TZNP_0, 15);
    
    //����
    _OR2(logic_TZNP_0, 14, logic_TZNP_0, 15, logic_TZNP_0, 16);
    if (_GET_STATE(logic_TZNP_0, 16)) _SET_BIT(p_active_functions, (shift_to_base_rang_index + RANG_TZNP));
    else _CLEAR_BIT(p_active_functions, (shift_to_base_rang_index + RANG_TZNP));
  }

#undef CTR_TZNP
#undef CTR_TZNP_VPERED
#undef CTR_TZNP_NAZAD

#undef RANG_BLOCK_TZNP
#undef RANG_SECTOR_TZNP_VPERED
#undef RANG_SECTOR_TZNP_NAZAD
#undef RANG_PO_3I0_TZNP_VPERED
#undef RANG_PO_3I0_TZNP_NAZAD
#undef RANG_PO_3U0_TZNP_VPERED
#undef RANG_PO_3U0_TZNP_NAZAD
#undef RANG_PO_TZNP_VPERED
#undef RANG_PO_TZNP_NAZAD
#undef RANG_TZNP
}
/*****************************************************/

/*****************************************************/
// ���(���)
/*****************************************************/
inline void zop_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  /*******************************/
  //������� � �������� ����� ������ ������ ��������� ����������� � ����������������
  /*******************************/
  unsigned int i2_current = measurement[IM_I2], i1_current = measurement[IM_I1];
  /*******************************/
  
  /*******************************/
  //1 ������ ���(���)
  /*******************************/
  if ((current_settings_prt.control_zop & CTR_ZOP_STATE) != 0)
  {
    //1 ������ ���(���) ��������
    unsigned int setpoint; //������� - � ���� ���������� ����������� ��������
    unsigned int setpoint_i1, setpoint_i2;
    unsigned int previous_state_po_zop1;
    
    if (i1_bilshe_porogu == 0) setpoint_i1 = MIN_LIMIT_FOR_I1_AND_I2*KOEF_POVERNENNJA_ZOP_BLK/100;
    else setpoint_i1 = MIN_LIMIT_FOR_I1_AND_I2;
    unsigned int i1_bilshe_porogu_tmp = i1_bilshe_porogu = (i1_current >= setpoint_i1);
    
    if (i2_bilshe_porogu == 0) setpoint_i2 = MIN_LIMIT_FOR_I1_AND_I2*KOEF_POVERNENNJA_ZOP_BLK/100;
    else setpoint_i2 = MIN_LIMIT_FOR_I1_AND_I2;
    unsigned int i2_bilshe_porogu_tmp = i2_bilshe_porogu = (i2_current >= setpoint_i2);
    
    //���� �� ���(���) �� �� �������, �� ����� ��������� �� ������ ���������� - �������, ��� ��������� �� ������� � ������� ���� �� ��������� ����
    //���� �� ���(���) ��� ����������, �� ����� ��������� �� ������� ���������� - �������� ������� �� ������� ������ �� ����������� ����������
    if(( previous_state_po_zop1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_ZOP) ) == 0 )
    {
      //�������� �� ������ �����������
      setpoint = current_settings_prt.setpoint_zop[number_group_stp];
    }
    else
    {
      //�������� �� ������ ����������
      setpoint = current_settings_prt.setpoint_zop[number_group_stp]*KOEF_POVERNENNJA_GENERAL_UP/100;
    }
    
    //�����������, ��� ������� ������ "�� ���"
    /*
    I2
    -- >= K
    I1
    ������� �������� ������ � ��� � ����������� �� 1000, ,�� 0,010 ������� 10, � 1,000 ������� 1000,
    �� K*1000 = SETPOINT =>
        SETPOINT
    K = --------  =>
         1000
    I2    SETPOINT
    -- >= ---------
    I1     1000
    
    ������ ��������� � ����� ����������� �� ����� � ���� �������������, �� ���� � � ��������� ��������� (��������), � ��� �������� �� ���������
    
    (I2*1000) >= (SETPOINT*I1)
    */
    if (
        (i1_bilshe_porogu_tmp != 0) &&
        (i2_bilshe_porogu_tmp != 0) &&
        ((i2_current*1000) >= (i1_current*setpoint))                            && 
        (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_ZOP) == 0)
       )
    {
      //���� ����� ��������� ��������� ������ ��������� �����������
      if(previous_state_po_zop1 == 0)
      {
        //������������ ������ "�� ���"
        _SET_BIT(p_active_functions, RANG_PO_ZOP);
      
        //��������� ������ ���(���), ���� �� �� �� ���������
        global_timers[INDEX_TIMER_ZOP] = 0;
      }
    }
    else 
    {
      //�� ���� ����� ��������� ��������� ������ ��������� �����������
      if(previous_state_po_zop1 != 0)
      {
        //������� ������ "�� ���"
        _CLEAR_BIT(p_active_functions, RANG_PO_ZOP);
        //�� � ������ ����� �������� ������� "����. ���"
        _CLEAR_BIT(p_active_functions, RANG_ZOP);
        //���� ������ �� �� ��������? �� ������� ����
        if ( global_timers[INDEX_TIMER_ZOP] >=0) global_timers[INDEX_TIMER_ZOP] = -1;
      }
    }
    
    if(global_timers[INDEX_TIMER_ZOP] >= current_settings_prt.timeout_zop[number_group_stp])
    {
      //���� ��������� �������� ���(���) �� ������������ ������ "����. ���"
      _SET_BIT(p_active_functions, RANG_ZOP);

      //������� ������ ���(���)
      global_timers[INDEX_TIMER_ZOP] = -1;
    }
  }
  else
  {
    //����� ������� �� ������� � �������, �� �� 1 ������ ���(���) ����������
    _CLEAR_BIT(p_active_functions, RANG_PO_ZOP);
    _CLEAR_BIT(p_active_functions, RANG_ZOP);
    global_timers[INDEX_TIMER_ZOP] = -1;

  }  
}
/*****************************************************/

/*****************************************************/
// �����1
/*****************************************************/
void umin1_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  _Bool previous_state_po_umin1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UMIN1) > 0;
  _Bool previous_state_po_ublk_umin1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN1) > 0;
  _Bool previous_state_po_iblk_umin1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_IBLK_UMIN1) > 0;
  
  unsigned int setpoint1 = previous_state_po_umin1 ?
          current_settings_prt.setpoint_Umin1[number_group_stp] * U_DOWN / 100 :
          current_settings_prt.setpoint_Umin1[number_group_stp];
  
  _Bool Uab_is_smaller_than_Umin1 = measurement[IM_UAB] <= setpoint1;
  _Bool Ubc_is_smaller_than_Umin1 = measurement[IM_UBC] <= setpoint1;
  _Bool Uca_is_smaller_than_Umin1 = measurement[IM_UCA] <= setpoint1;
  
  _Bool Ua_is_smaller_than_Umin1 = measurement[IM_UA] <= setpoint1;
  _Bool Ub_is_smaller_than_Umin1 = measurement[IM_UB] <= setpoint1;
  _Bool Uc_is_smaller_than_Umin1 = measurement[IM_UC] <= setpoint1;
  
  unsigned int setpoint2 = previous_state_po_ublk_umin1 ?
          KOEF_MIN_UMIN * U_DOWN_1 / 100 : KOEF_MIN_UMIN;
          
  _Bool Ua_or_Ub_or_Uc_is_smaller_than_250mV = (measurement[IM_UA] <= setpoint2) || (measurement[IM_UB] <= setpoint2) || (measurement[IM_UC] <= setpoint2);
  _Bool Uab_or_Ubc_or_Uca_is_smaller_than_250mV = (measurement[IM_UAB] <= setpoint2) || (measurement[IM_UBC] <= setpoint2) || (measurement[IM_UCA] <= setpoint2);
  
  unsigned int setpoint3 = previous_state_po_iblk_umin1 ?
          current_settings_prt.setpoint_Umin1_Iblk[number_group_stp] * KOEF_POVERNENNJA_GENERAL_DOWN / 100 :
          current_settings_prt.setpoint_Umin1_Iblk[number_group_stp];
  
  _Bool Ia_and_Ic_is_smaller_than_Iust = (measurement[IM_IA] <= setpoint3) &&
                                         (measurement[IM_IB] <= setpoint3) &&
                                         (measurement[IM_IC] <= setpoint3);
  //�
  unsigned int tmp_value = (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)) << 0;
//  tmp_value |= ((current_settings_prt.control_Umin & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0)                                 << 1;
//  _INVERTOR(tmp_value, 1, tmp_value, 1);
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN1) != 0)                                                            << 2;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_PO_UMIN1_OR_AND) != 0)                                                  << 3;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN1_UBLK) != 0)                                                       << 4;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN1_IBLK) != 0)                                                       << 5;
  
  //��
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_UMIN1) != 0)                                     << 6;
  _INVERTOR(tmp_value, 6, tmp_value, 6);
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_START_UMIN1) != 0)                                     << 7;
  
//  _AND2(tmp_value, 0, tmp_value, 1, tmp_value, 8);
  
  if (_GET_STATE(tmp_value, 0)) 
  {
    //������
    _AND4(Ua_is_smaller_than_Umin1, 0, Ub_is_smaller_than_Umin1, 0, Uc_is_smaller_than_Umin1, 0, tmp_value, 3, tmp_value, 9);
    _OR3(Ua_is_smaller_than_Umin1, 0, Ub_is_smaller_than_Umin1, 0, Uc_is_smaller_than_Umin1, 0, tmp_value, 10);
    _INVERTOR(tmp_value, 3, tmp_value, 3);
    _AND2(tmp_value, 10, tmp_value, 3, tmp_value, 11);
    _AND2(tmp_value, 5, Ia_and_Ic_is_smaller_than_Iust, 0, tmp_value, 12);
    _INVERTOR(tmp_value, 5, tmp_value, 5);
    _AND3(Ua_or_Ub_or_Uc_is_smaller_than_250mV, 0, tmp_value, 4, tmp_value, 5, tmp_value, 13);
    
    //�� U���. Umin1
    if (Ua_or_Ub_or_Uc_is_smaller_than_250mV)
      _SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN1);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_UBLK_UMIN1);
  } 
  else 
  {
    //��������
    _AND4(Uab_is_smaller_than_Umin1, 0, Ubc_is_smaller_than_Umin1, 0, Uca_is_smaller_than_Umin1, 0, tmp_value, 3, tmp_value, 9);
    _OR3(Uab_is_smaller_than_Umin1, 0, Ubc_is_smaller_than_Umin1, 0, Uca_is_smaller_than_Umin1, 0, tmp_value, 10);
    _INVERTOR(tmp_value, 3, tmp_value, 3);
    _AND2(tmp_value, 10, tmp_value, 3, tmp_value, 11);
    _AND2(tmp_value, 5, Ia_and_Ic_is_smaller_than_Iust, 0, tmp_value, 12);
    _INVERTOR(tmp_value, 5, tmp_value, 5);
    _AND3(Uab_or_Ubc_or_Uca_is_smaller_than_250mV, 0, tmp_value, 4, tmp_value, 5, tmp_value, 13);
    
    //�� U���. Umin1
    if (Uab_or_Ubc_or_Uca_is_smaller_than_250mV)
      _SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN1);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_UBLK_UMIN1);
  }
  _INVERTOR(tmp_value, 12, tmp_value, 12);
  _INVERTOR(tmp_value, 13, tmp_value, 13);
  _OR3(tmp_value, 7, tmp_value, 9, tmp_value, 11, tmp_value, 14);
  
  _AND5(tmp_value, 6, tmp_value, 2, tmp_value, 14, tmp_value, 13, tmp_value, 12, tmp_value, 15);
  
  //�� I���. Umin1
  if (Ia_and_Ic_is_smaller_than_Iust)
    _SET_BIT(p_active_functions, RANG_PO_IBLK_UMIN1);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_IBLK_UMIN1);
  
  //�� Umin1
  if (_GET_STATE(tmp_value, 15))
    _SET_BIT(p_active_functions, RANG_PO_UMIN1);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_UMIN1);
  
  _TIMER_T_0(INDEX_TIMER_UMIN1, current_settings_prt.timeout_Umin1[number_group_stp], tmp_value, 15, tmp_value, 16);
  
  //����. Umin1
  if (_GET_STATE(tmp_value, 16))
    _SET_BIT(p_active_functions, RANG_UMIN1);
  else
    _CLEAR_BIT(p_active_functions, RANG_UMIN1);
}
/*****************************************************/

/*****************************************************/
// �����2
/*****************************************************/
void umin2_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  _Bool previous_state_po_umin2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UMIN2) > 0;
  _Bool previous_state_po_ublk_umin2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN2) > 0;
  _Bool previous_state_po_iblk_umin2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_IBLK_UMIN2) > 0;
  
  unsigned int setpoint1 = previous_state_po_umin2 ?
          current_settings_prt.setpoint_Umin2[number_group_stp] * U_DOWN / 100 :
          current_settings_prt.setpoint_Umin2[number_group_stp];
  
  _Bool Uab_is_smaller_than_Umin2 = measurement[IM_UAB] <= setpoint1;
  _Bool Ubc_is_smaller_than_Umin2 = measurement[IM_UBC] <= setpoint1;
  _Bool Uca_is_smaller_than_Umin2 = measurement[IM_UCA] <= setpoint1;
  
  _Bool Ua_is_smaller_than_Umin2 = measurement[IM_UA] <= setpoint1;
  _Bool Ub_is_smaller_than_Umin2 = measurement[IM_UB] <= setpoint1;
  _Bool Uc_is_smaller_than_Umin2 = measurement[IM_UC] <= setpoint1;
  
  unsigned int setpoint2 = previous_state_po_ublk_umin2 ?
          KOEF_MIN_UMIN * U_DOWN_1 / 100 : KOEF_MIN_UMIN;
          
  _Bool Ua_or_Ub_or_Uc_is_smaller_than_250mV = (measurement[IM_UA] <= setpoint2) || (measurement[IM_UB] <= setpoint2) || (measurement[IM_UC] <= setpoint2);
  _Bool Uab_or_Ubc_or_Uca_is_smaller_than_250mV = (measurement[IM_UAB] <= setpoint2) || (measurement[IM_UBC] <= setpoint2) || (measurement[IM_UCA] <= setpoint2);
  
  unsigned int setpoint3 = previous_state_po_iblk_umin2 ?
          current_settings_prt.setpoint_Umin2_Iblk[number_group_stp] * KOEF_POVERNENNJA_GENERAL_DOWN / 100 :
          current_settings_prt.setpoint_Umin2_Iblk[number_group_stp];
  
  _Bool Ia_and_Ic_is_smaller_than_Iust = (measurement[IM_IA] <= setpoint3) &&
                                         (measurement[IM_IB] <= setpoint3) &&
                                         (measurement[IM_IC] <= setpoint3);
  //�
  unsigned int tmp_value = (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)) << 0;
//  tmp_value |= ((current_settings_prt.control_Umin & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0)                                 << 1;
//  _INVERTOR(tmp_value, 1, tmp_value, 1);
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN2) != 0)                                                            << 2;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_PO_UMIN2_OR_AND) != 0)                                                  << 3;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN2_UBLK) != 0)                                                       << 4;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN2_IBLK) != 0)                                                       << 5;
  
  //��
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_UMIN2) != 0)                                    << 6;
  _INVERTOR(tmp_value, 6, tmp_value, 6);
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_START_UMIN2) != 0)                                    << 7;
  
//  _AND2(tmp_value, 0, tmp_value, 1, tmp_value, 8);
  
  if (_GET_STATE(tmp_value, 0))
  {
    //������
    _AND4(Ua_is_smaller_than_Umin2, 0, Ub_is_smaller_than_Umin2, 0, Uc_is_smaller_than_Umin2, 0, tmp_value, 3, tmp_value, 9);
    _OR3(Ua_is_smaller_than_Umin2, 0, Ub_is_smaller_than_Umin2, 0, Uc_is_smaller_than_Umin2, 0, tmp_value, 10);
    _INVERTOR(tmp_value, 3, tmp_value, 3);
    _AND2(tmp_value, 10, tmp_value, 3, tmp_value, 11);
    _AND2(tmp_value, 5, Ia_and_Ic_is_smaller_than_Iust, 0, tmp_value, 12);
    _INVERTOR(tmp_value, 5, tmp_value, 5);
    _AND3(Ua_or_Ub_or_Uc_is_smaller_than_250mV, 0, tmp_value, 4, tmp_value, 5, tmp_value, 13);
    
    //�� U���. Umin2
    if (Ua_or_Ub_or_Uc_is_smaller_than_250mV)
      _SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN2);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_UBLK_UMIN2);
    
  } 
  else 
  {
    //��������
    _AND4(Uab_is_smaller_than_Umin2, 0, Ubc_is_smaller_than_Umin2, 0, Uca_is_smaller_than_Umin2, 0, tmp_value, 3, tmp_value, 9);
    _OR3(Uab_is_smaller_than_Umin2, 0, Ubc_is_smaller_than_Umin2, 0, Uca_is_smaller_than_Umin2, 0, tmp_value, 10);
    _INVERTOR(tmp_value, 3, tmp_value, 3);
    _AND2(tmp_value, 10, tmp_value, 3, tmp_value, 11);
    _AND2(tmp_value, 5, Ia_and_Ic_is_smaller_than_Iust, 0, tmp_value, 12);
    _INVERTOR(tmp_value, 5, tmp_value, 5);
    _AND3(Uab_or_Ubc_or_Uca_is_smaller_than_250mV, 0, tmp_value, 4, tmp_value, 5, tmp_value, 13);
    
    //�� U���. Umin2
    if (Uab_or_Ubc_or_Uca_is_smaller_than_250mV)
      _SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN2);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_UBLK_UMIN2);
  }
  _INVERTOR(tmp_value, 12, tmp_value, 12);
  _INVERTOR(tmp_value, 13, tmp_value, 13);
  _OR3(tmp_value, 7, tmp_value, 9, tmp_value, 11, tmp_value, 14);
  
  _AND5(tmp_value, 6, tmp_value, 2, tmp_value, 14, tmp_value, 13, tmp_value, 12, tmp_value, 15);
  
  //�� I���. Umin2
  if (Ia_and_Ic_is_smaller_than_Iust)
    _SET_BIT(p_active_functions, RANG_PO_IBLK_UMIN2);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_IBLK_UMIN2);
  
  //�� Umin2
  if (_GET_STATE(tmp_value, 15))
    _SET_BIT(p_active_functions, RANG_PO_UMIN2);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_UMIN2);
  
  _TIMER_T_0(INDEX_TIMER_UMIN2, current_settings_prt.timeout_Umin2[number_group_stp], tmp_value, 15, tmp_value, 16);
  
  //����. Umin2
  if (_GET_STATE(tmp_value, 16))
    _SET_BIT(p_active_functions, RANG_UMIN2);
  else
    _CLEAR_BIT(p_active_functions, RANG_UMIN2);
}
/*****************************************************/

/*****************************************************/
// ���04
/*****************************************************/
void mtz04_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
 //�������� ����������� ��������� �� ���04_1
  _Bool previous_state_po_mtz04_1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_MTZ04_1) > 0;
  _Bool previous_state_po_mtz04_2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_MTZ04_2) > 0;
//������ ���� � ���� ��� �� � ����������� �� �� �������
  unsigned int setpoint1 = previous_state_po_mtz04_1 ?
          current_settings_prt.setpoint_mtz04_1[number_group_stp] * U_UP / 100 :
          current_settings_prt.setpoint_mtz04_1[number_group_stp];

  unsigned int setpoint2 = previous_state_po_mtz04_2 ?
          current_settings_prt.setpoint_mtz04_2[number_group_stp] * U_UP / 100 :
          current_settings_prt.setpoint_mtz04_2[number_group_stp];

//#define IM_I04         9
  _Bool I04_is_larger_than_ust1 = measurement[IM_I04] >= setpoint1;
  _Bool I04_is_larger_than_ust2 = measurement[IM_I04] >= setpoint2;
  //�
  //���-����  ���04_1
  unsigned int tmp_value = ((current_settings_prt.control_mtz04 & CTR_MTZ04_1) != 0)  << 0;
  //���-����  ���04_2
  tmp_value |= ((current_settings_prt.control_mtz04 & CTR_MTZ04_2) != 0) << 1;
  //���-����  ��������� ���04_2
  tmp_value |= ((current_settings_prt.control_mtz04 & CTR_MTZ04_2_PRYSKORENNJA) != 0) << 2;
  //���-����  ���������� ���04_2
  tmp_value |= ((current_settings_prt.control_mtz04 & CTR_MTZ04_2_PRYSKORENA) != 0) << 3;
  //�� ���� ���04_1
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_MTZ04_1) != 0) << 4;
  //�� ���� ���04_2
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_MTZ04_2) != 0) << 5;
  //�� ���� ��������� ���04_2
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_USK_MTZ04_2) != 0) << 6;
  //����� ��
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_STATE_VV) != 0)  << 7;
/*
//��� ���04
 unsigned int tmp3=0;
 //�������
 if(current_settings_prt.type_mtz04_2==0) _SET_BIT(tmp3, 0);
  unsigned int type_mtz04 = 0;
 //��������� �
 if(current_settings_prt.type_mtz04_2==3) type_mtz04 = TYPE_MTZ_DEPENDENT_A;
 //��������� �
 if(current_settings_prt.type_mtz04_2==4) type_mtz04 = TYPE_MTZ_DEPENDENT_B;
 //��������� �
 if(current_settings_prt.type_mtz04_2==5) type_mtz04 = TYPE_MTZ_DEPENDENT_C;
*/
//RANG_BLOCK_MTZ04_1,
 //MTZ04_1 LOGIKA
  _INVERTOR(tmp_value, 4, tmp_value, 8);
        //�� ���04_1
  _AND3(I04_is_larger_than_ust1, 0, 
        //���-����  ���04_1
        tmp_value, 0,
        //INVERTOR
        tmp_value, 8, tmp_value, 9);
  //����.�� MTZ04_1
  if (_GET_STATE(tmp_value, 9)) 
  {
     //PO MTZ04_1
    _SET_BIT(p_active_functions, RANG_PO_MTZ04_1);
  }
  else _CLEAR_BIT(p_active_functions, RANG_PO_MTZ04_1);

    _TIMER_T_0(INDEX_TIMER_MTZ04_1, current_settings_prt.timeout_mtz04_1[number_group_stp], tmp_value, 9, tmp_value, 10);
  //����. MTZ04_1
  if (_GET_STATE(tmp_value, 10)) 
  {
     //MTZ04_1
    _SET_BIT(p_active_functions, RANG_MTZ04_1);
  }
  else _CLEAR_BIT(p_active_functions, RANG_MTZ04_1);
    
//MTZ04_2
//RANG_STATE_VV,
//����� ��
//  if (_GET_STATE(p_active_functions, RANG_STATE_VV)) {
//  }//if

 unsigned int tmp2=0;
  _AND2(tmp_value, 7, 
        //��������� ���04_2
        tmp_value, 2, tmp2, 0);

  _TIMER_IMPULSE(INDEX_TIMER_MTZ04_2, current_settings_prt.timeout_mtz04_2_vvid_pr[number_group_stp],
                 previous_states_MTZ04_vvid_pr_0, 0, tmp2, 0, tmp2, 1);
//����������
  _OR2(tmp_value, 3, 
        tmp2, 1, tmp2, 2);
  _INVERTOR(tmp_value, 6, tmp2, 3);
  _AND2(tmp2, 3, 
        tmp2, 2, tmp2, 4);
  _INVERTOR(tmp2, 4, tmp2, 5);
  _INVERTOR(tmp_value, 5, tmp2, 6);
  _AND3(I04_is_larger_than_ust2, 0, 
        tmp_value, 1, 
        tmp2, 6, tmp2, 7);
  //����.�� MTZ04_2
  if (_GET_STATE(tmp2, 7)) 
  {
     //PO MTZ04_2
    _SET_BIT(p_active_functions, RANG_PO_MTZ04_2);
  }
  else _CLEAR_BIT(p_active_functions, RANG_PO_MTZ04_2);
    
//���������
  _AND2(tmp2, 4, 
        tmp2, 7, tmp2, 8);
//�������
//��� ���04
 unsigned int tmp3=0;
 //�������
// if(current_settings_prt.type_mtz04_2==0){
//   _SET_STATE(tmp3, 0);
// }//if
  _AND2(tmp2, 5, 
        tmp2, 7, 
             tmp2, 10);
//��������� � B C
//  _INVERTOR(tmp2, 9, tmp2, 10);
//  _AND3(tmp2, 4, 
  //      tmp2, 7, 
    //    tmp3, 0, tmp2, 10);
//���������
  _TIMER_T_0(INDEX_TIMER_MTZ04_3, current_settings_prt.timeout_mtz04_2_pr[number_group_stp], tmp2, 8, tmp3, 4);
//�������
 switch(current_settings_prt.type_mtz04_2)
 {
 case 0:
   {
     _TIMER_T_0(INDEX_TIMER_MTZ04_4, current_settings_prt.timeout_mtz04_2[number_group_stp], tmp2, 10, /*tmp3, 6*/ p_global_trigger_state_mtz04_2, 0);
     break;
   }
 case TYPE_MTZ_DEPENDENT_A:
 case TYPE_MTZ_DEPENDENT_B:
 case TYPE_MTZ_DEPENDENT_C:
   {
     //������ ��������
     _TIMER_T_0_LOCK(INDEX_TIMER_MTZ04_4,  timeout_dependent_general(measurement[IM_I04], current_settings_prt.setpoint_mtz04_2[number_group_stp], current_settings_prt.timeout_mtz04_2[number_group_stp], current_settings_prt.type_mtz04_2), tmp2, 10, /*tmp3, 6*/ p_global_trigger_state_mtz04_2, 0);
     break;
   }
 }

  _OR2(tmp3, 4, /*tmp3, 6*/p_global_trigger_state_mtz04_2, 0, tmp3, 7);
  //����. MTZ04_2
  if (_GET_STATE(tmp3, 7))
  {
     //MTZ04_1
    _SET_BIT(p_active_functions, RANG_MTZ04_2);
  }
  else _CLEAR_BIT(p_active_functions, RANG_MTZ04_2);

}//mtz04

/*****************************************************/
// ������1
/*****************************************************/
void umax1_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  _Bool previous_state_po_umax1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UMAX1) > 0;
  
  unsigned int setpoint1 = previous_state_po_umax1 ?
          current_settings_prt.setpoint_Umax1[number_group_stp] * current_settings_prt.setpoint_kp_Umax[number_group_stp] / 100 :
          current_settings_prt.setpoint_Umax1[number_group_stp];
  
  _Bool Uab_is_larger_than_Umax1 = measurement[IM_UAB] >= setpoint1;
  _Bool Ubc_is_larger_than_Umax1 = measurement[IM_UBC] >= setpoint1;
  _Bool Uca_is_larger_than_Umax1 = measurement[IM_UCA] >= setpoint1;
  
  _Bool Ua_is_larger_than_Umax1 = measurement[IM_UA] >= setpoint1;
  _Bool Ub_is_larger_than_Umax1 = measurement[IM_UB] >= setpoint1;
  _Bool Uc_is_larger_than_Umax1 = measurement[IM_UC] >= setpoint1;
  
  //�
  unsigned int tmp_value = (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)) << 0;
//  tmp_value |= ((current_settings_prt.control_Umax & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0)                                 << 1;
//  _INVERTOR(tmp_value, 1, tmp_value, 1);
  tmp_value |= ((current_settings_prt.control_Umax & CTR_PO_UMAX1_OR_AND) != 0)                                                  << 2;
  tmp_value |= ((current_settings_prt.control_Umax & CTR_UMAX1) != 0)                                                            << 3;
  
  //��
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_UMAX1) != 0)                                    << 4;
  
  if (_GET_STATE(tmp_value, 0)) {
    _AND4(Ua_is_larger_than_Umax1, 0, Ub_is_larger_than_Umax1, 0, Uc_is_larger_than_Umax1, 0, tmp_value, 2, tmp_value, 5);
    _OR3(Ua_is_larger_than_Umax1, 0, Ub_is_larger_than_Umax1, 0, Uc_is_larger_than_Umax1, 0, tmp_value, 6);
  } else {
    _AND4(Uab_is_larger_than_Umax1, 0, Ubc_is_larger_than_Umax1, 0, Uca_is_larger_than_Umax1, 0, tmp_value, 2, tmp_value, 5);
    _OR3(Uab_is_larger_than_Umax1, 0, Ubc_is_larger_than_Umax1, 0, Uca_is_larger_than_Umax1, 0, tmp_value, 6);
  }
  
  _INVERTOR(tmp_value, 2, tmp_value, 2);
  _AND2(tmp_value, 6, tmp_value, 2, tmp_value, 7);
  _OR2(tmp_value, 5, tmp_value, 7, tmp_value, 8);
  _INVERTOR(tmp_value, 4, tmp_value, 4);
  _AND3(tmp_value, 8, tmp_value, 4, tmp_value, 3, tmp_value, 9);
  
  //�� Umax1
  if (_GET_STATE(tmp_value, 9))
    _SET_BIT(p_active_functions, RANG_PO_UMAX1);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_UMAX1);
  
  _TIMER_T_0(INDEX_TIMER_UMAX1, current_settings_prt.timeout_Umax1[number_group_stp], tmp_value, 9, tmp_value, 10);
  
  //����. Umax1
  if (_GET_STATE(tmp_value, 10))
    _SET_BIT(p_active_functions, RANG_UMAX1);
  else
    _CLEAR_BIT(p_active_functions, RANG_UMAX1);
}
/*****************************************************/

/*****************************************************/
// ������2
/*****************************************************/
void umax2_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  _Bool previous_state_po_umax2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UMAX2) > 0;
  
  unsigned int setpoint1 = previous_state_po_umax2 ?
          current_settings_prt.setpoint_Umax2[number_group_stp] * current_settings_prt.setpoint_kp_Umax[number_group_stp] / 100 :
          current_settings_prt.setpoint_Umax2[number_group_stp];
  
  _Bool Uab_is_larger_than_Umax2 = measurement[IM_UAB] >= setpoint1;
  _Bool Ubc_is_larger_than_Umax2 = measurement[IM_UBC] >= setpoint1;
  _Bool Uca_is_larger_than_Umax2 = measurement[IM_UCA] >= setpoint1;
  
  _Bool Ua_is_larger_than_Umax2 = measurement[IM_UA] >= setpoint1;
  _Bool Ub_is_larger_than_Umax2 = measurement[IM_UB] >= setpoint1;
  _Bool Uc_is_larger_than_Umax2 = measurement[IM_UC] >= setpoint1;
  
  //�
  unsigned int tmp_value = (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)) << 0;
//  tmp_value |= ((current_settings_prt.control_Umax & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0)                                 << 1;
//  _INVERTOR(tmp_value, 1, tmp_value, 1);
  tmp_value |= ((current_settings_prt.control_Umax & CTR_PO_UMAX2_OR_AND) != 0)                                                  << 2;
  tmp_value |= ((current_settings_prt.control_Umax & CTR_UMAX2) != 0)                                                            << 3;
  
  //��
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_UMAX2) != 0)                                    << 4;
  
  if (_GET_STATE(tmp_value, 0)) {
    _AND4(Ua_is_larger_than_Umax2, 0, Ub_is_larger_than_Umax2, 0, Uc_is_larger_than_Umax2, 0, tmp_value, 2, tmp_value, 5);
    _OR3(Ua_is_larger_than_Umax2, 0, Ub_is_larger_than_Umax2, 0, Uc_is_larger_than_Umax2, 0, tmp_value, 6);
  } else {
    _AND4(Uab_is_larger_than_Umax2, 0, Ubc_is_larger_than_Umax2, 0, Uca_is_larger_than_Umax2, 0, tmp_value, 2, tmp_value, 5);
    _OR3(Uab_is_larger_than_Umax2, 0, Ubc_is_larger_than_Umax2, 0, Uca_is_larger_than_Umax2, 0, tmp_value, 6);
  }
  
  _INVERTOR(tmp_value, 2, tmp_value, 2);
  _AND2(tmp_value, 6, tmp_value, 2, tmp_value, 7);
  _OR2(tmp_value, 5, tmp_value, 7, tmp_value, 8);
  _INVERTOR(tmp_value, 4, tmp_value, 4);
  _AND3(tmp_value, 8, tmp_value, 4, tmp_value, 3, tmp_value, 9);
  
  //�� Umax1
  if (_GET_STATE(tmp_value, 9))
    _SET_BIT(p_active_functions, RANG_PO_UMAX2);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_UMAX2);
  
  _TIMER_T_0(INDEX_TIMER_UMAX2, current_settings_prt.timeout_Umax2[number_group_stp], tmp_value, 9, tmp_value, 10);
  
  //����. Umax1
  if (_GET_STATE(tmp_value, 10))
    _SET_BIT(p_active_functions, RANG_UMAX2);
  else
    _CLEAR_BIT(p_active_functions, RANG_UMAX2);
}
/*****************************************************/

/*****************************************************/
// ��� ����
/*****************************************************/
void achr_chapv_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
//RANG_ACHR_CHAPV_VID_DV,
/*
  //���� ���� �� ���
  _Bool previous_state_po_f1_achr = _CHECK_SET_BIT(p_active_functions, RANG_PO_ACHR1);
  //���� ���� �� ����
  _Bool previous_state_po_f1_chapv = _CHECK_SET_BIT(p_active_functions, RANG_PO_CHAPV1);
 // 
����������
*/
  //���� ���� �� ���1
  _Bool previous_state_po_f1_achr1  = _CHECK_SET_BIT(p_active_functions, RANG_PO_ACHR1);
  //���� ���� �� ����1
  _Bool previous_state_po_f1_chapv1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_CHAPV1);
  //���� ���� �� ���2
  _Bool previous_state_po_f1_achr2  = _CHECK_SET_BIT(p_active_functions, RANG_PO_ACHR2);
  //���� ���� �� ����2
  _Bool previous_state_po_f1_chapv2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_CHAPV2);
//----------
/*
���������
*/

  unsigned int setpoint1 = previous_state_po_achr_chapv_uaf1 ?
          current_settings_prt.setpoint_achr_chapv_uf[number_group_stp] * U_UP / 100 :
          current_settings_prt.setpoint_achr_chapv_uf[number_group_stp];
          
  unsigned int setpoint2 = previous_state_po_achr_chapv_ubf1 ?
          current_settings_prt.setpoint_achr_chapv_uf[number_group_stp] * U_UP / 100 :
          current_settings_prt.setpoint_achr_chapv_uf[number_group_stp];
  unsigned int setpoint3 = previous_state_po_achr_chapv_ucf1 ?
          current_settings_prt.setpoint_achr_chapv_uf[number_group_stp] * U_UP / 100 :
          current_settings_prt.setpoint_achr_chapv_uf[number_group_stp];
  
  //��������
  _Bool Uab_is_larger_than_UF1 = measurement[IM_UAB] >= setpoint1;
  _Bool Ubc_is_larger_than_UF1 = measurement[IM_UBC] >= setpoint2;
  _Bool Uca_is_larger_than_UF1 = measurement[IM_UCA] >= setpoint3;
  
  //������
  _Bool Ua_is_larger_than_UF1 = measurement[IM_UA] >= setpoint1;
  _Bool Ub_is_larger_than_UF1 = measurement[IM_UB] >= setpoint2;
  _Bool Uc_is_larger_than_UF1 = measurement[IM_UC] >= setpoint3;
  
  /*----------------����� ������/��������-------------------------------------*/
  _Bool UF1_phase = ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0);
  /*--------------------------------------------------------------------------*/
  
  _Bool timer1 = 0;
  _Bool UF1_is_larger_than_U_setpoint_F1 = 0;
//  _Bool UF1_is_smaller_than_U_setpoint_F1 = 0;
  //�� ������-��������
  if (UF1_phase)
  {
    //�� UAF1
    previous_state_po_achr_chapv_uaf1 = Ua_is_larger_than_UF1;
    //�� UBF1
    previous_state_po_achr_chapv_ubf1 = Ub_is_larger_than_UF1;
    //�� UCF1
    previous_state_po_achr_chapv_ucf1 = Uc_is_larger_than_UF1;
    _AND3(Ua_is_larger_than_UF1, 0, Ub_is_larger_than_UF1, 0, Uc_is_larger_than_UF1, 0, timer1, 0);
  }
  else 
  {
    //�� UAF1
    previous_state_po_achr_chapv_uaf1 = Uab_is_larger_than_UF1;
    //�� UBF1
    previous_state_po_achr_chapv_ubf1 = Ubc_is_larger_than_UF1;
    //�� UCF1
    previous_state_po_achr_chapv_ucf1 = Uca_is_larger_than_UF1;
    _AND3(Uab_is_larger_than_UF1, 0, Ubc_is_larger_than_UF1, 0, Uca_is_larger_than_UF1, 0, timer1, 0);
  }
  _TIMER_T_0(INDEX_TIMER_ACHR_CHAPV_100MS_1, TIMEOUT_ACHR_CHAPV_100MS, timer1, 0, UF1_is_larger_than_U_setpoint_F1, 0);
  //L1
//  UF1_is_smaller_than_U_setpoint_F1 = !UF1_is_larger_than_U_setpoint_F1;
 //L2, 3 
  int F1 = (int) (frequency * 100);
/*
  _Bool po_f1_achr_rab = F1 <= ((int)current_settings_prt.setpoint_achr1_f_rab[number_group_stp]);
  _Bool po_f1_chapv_rab = F1 >= ((int)current_settings_prt.setpoint_chapv1_f_rab[number_group_stp]);
  _Bool po_f1_achr = 0;
����������
*/
  _Bool po_f1_achr1_rab = F1 <= ((int)current_settings_prt.setpoint_achr1_f_rab[number_group_stp]);
  _Bool po_f1_chapv1_rab = F1 >= ((int)current_settings_prt.setpoint_chapv1_f_rab[number_group_stp]);
  _Bool po_f1_achr1 = 0;
  _Bool po_f1_achr2_rab = F1 <= ((int)current_settings_prt.setpoint_achr2_f_rab[number_group_stp]);
  _Bool po_f1_chapv2_rab = F1 >= ((int)current_settings_prt.setpoint_chapv2_f_rab[number_group_stp]);
  _Bool po_f1_achr2 = 0;
//----------

/*  
  //��
  unsigned int tmp_value = (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_ACHR1) != 0) << 0;
  //L5
  _INVERTOR(tmp_value, 0, tmp_value, 0);
����������
*/
  //��
  unsigned int tmp_value1 = (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_ACHR1) != 0) << 0;
  //L5.1
  _INVERTOR(tmp_value1, 0, tmp_value1, 0);
  //��
  unsigned int tmp_value2 = (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_ACHR2) != 0) << 0;
  //L5
  _INVERTOR(tmp_value2, 0, tmp_value2, 0);
//----------

/*  
  //�
  tmp_value |= ((current_settings_prt.control_achr_chapv & CTR_ACHR1) != 0) << 1;
  //L4
  _AND3_INVERTOR(tmp_value, 0, tmp_value, 1, UF1_is_larger_than_U_setpoint_F1, 0, tmp_value, 3);
����������
*/
  //�
  tmp_value1 |= ((current_settings_prt.control_achr_chapv & CTR_ACHR1) != 0) << 1;
  //L4.1
  _AND3_INVERTOR(tmp_value1, 0, tmp_value1, 1, UF1_is_larger_than_U_setpoint_F1, 0, tmp_value1, 3);
  //�
  tmp_value2 |= ((current_settings_prt.control_achr_chapv & CTR_ACHR2) != 0) << 1;
  //L4.2
  _AND3_INVERTOR(tmp_value2, 0, tmp_value2, 1, UF1_is_larger_than_U_setpoint_F1, 0, tmp_value2, 3);
//----------
/*
  //Reset ����� L2
  if (!_GET_STATE(tmp_value, 3))
  {
    //�� ���1
    if (previous_state_po_f1_achr) 
    {
      //���������� ���
//  _Bool po_f1_chapv_rab = F1 >= ((int)current_settings_prt.setpoint_chapv1_f_rab[number_group_stp]);
      if (!po_f1_chapv_rab) 
      {
        //����� �������� 1 ����� ��������� �� ��� ��� ���� �� ��������� po_f1_chapv_rab (������� ����������)
        _SET_BIT(p_active_functions, RANG_PO_ACHR1);
        po_f1_achr = 1;
      }
    }
    else
    {
      //��������
//  _Bool po_f1_achr_rab = F1 <= ((int)current_settings_prt.setpoint_achr1_f_rab[number_group_stp]);
      
      if (po_f1_achr_rab)
      {
        _SET_BIT(p_active_functions, RANG_PO_ACHR1);
        po_f1_achr = 1;
      }
    }
  }//if
����������
*/
//---1----
  //Reset ����� L2
  if (!_GET_STATE(tmp_value1, 3))
  {
    //�� ���1
    if (previous_state_po_f1_achr1) 
    {
      //���������� ���
//  _Bool po_f1_chapv_rab = F1 >= ((int)current_settings_prt.setpoint_chapv1_f_rab[number_group_stp]);
      if (!po_f1_chapv1_rab) 
      {
        //����� �������� 1 ����� ��������� �� ��� ��� ���� �� ��������� po_f1_chapv_rab (������� ����������)
        _SET_BIT(p_active_functions, RANG_PO_ACHR1);
        po_f1_achr1 = 1;
      }
      else _CLEAR_BIT(p_active_functions, RANG_PO_ACHR1);
    }
    else
    {
      //��������
//  _Bool po_f1_achr_rab = F1 <= ((int)current_settings_prt.setpoint_achr1_f_rab[number_group_stp]);
      
      if (po_f1_achr1_rab)
      {
        _SET_BIT(p_active_functions, RANG_PO_ACHR1);
        po_f1_achr1 = 1;
      }
      else _CLEAR_BIT(p_active_functions, RANG_PO_ACHR1);
    }
  }//if
  else
  {
    _CLEAR_BIT(p_active_functions, RANG_PO_ACHR1);
  }
//----2----
  //Reset ����� L2
  if (!_GET_STATE(tmp_value2, 3))
  {
    //�� ���1
    if (previous_state_po_f1_achr2) 
    {
      //���������� ���
//  _Bool po_f1_chapv_rab = F1 >= ((int)current_settings_prt.setpoint_chapv1_f_rab[number_group_stp]);
      if (!po_f1_chapv2_rab) 
      {
        //����� �������� 1 ����� ��������� �� ��� ��� ���� �� ��������� po_f1_chapv_rab (������� ����������)
        _SET_BIT(p_active_functions, RANG_PO_ACHR2);
        po_f1_achr2 = 1;
      }
      else _CLEAR_BIT(p_active_functions, RANG_PO_ACHR2);
    }
    else
    {
      //��������
//  _Bool po_f1_achr_rab = F1 <= ((int)current_settings_prt.setpoint_achr1_f_rab[number_group_stp]);
      
      if (po_f1_achr2_rab)
      {
        _SET_BIT(p_active_functions, RANG_PO_ACHR2);
        po_f1_achr2 = 1;
      }
      else _CLEAR_BIT(p_active_functions, RANG_PO_ACHR2);
    }
  }//if
  else
  {
    _CLEAR_BIT(p_active_functions, RANG_PO_ACHR2);
  }
//--------------  
/*
  _TIMER_T_0(INDEX_TIMER_ACHR1, current_settings_prt.timeout_achr_1[number_group_stp], po_f1_achr, 0, tmp_value, 5);
����������
*/
//---1---
  _TIMER_T_0(INDEX_TIMER_ACHR1, current_settings_prt.timeout_achr_1[number_group_stp], po_f1_achr1, 0, tmp_value1, 5);
//---2---
  _TIMER_T_0(INDEX_TIMER_ACHR2, current_settings_prt.timeout_achr_2[number_group_stp], po_f1_achr2, 0, tmp_value2, 5);
//-------
  
  //���� ����
  if (UF1_is_larger_than_U_setpoint_F1)
    _SET_BIT(p_active_functions, RANG_RAZR_CHAPV);
  else
    _CLEAR_BIT(p_active_functions, RANG_RAZR_CHAPV);

/*  
  _Bool tmp1 = 0;
  do 
  {
    //L7
    _OR2(tmp_value, 3, !trigger_CHAPV1, 0, tmp_value, 30);
    
    //�� ����1
    _Bool po_f1_chapv = 0;
    if (previous_state_po_f1_chapv) 
    {
      po_f1_chapv = !po_f1_achr_rab;
    }
    else 
    {
      po_f1_chapv = po_f1_chapv_rab;
    }
    //L3
    _Bool po_f1_chapv_tmp = (!_GET_STATE(tmp_value, 30) && po_f1_chapv);
    if (po_f1_chapv_tmp) 
    {
      _SET_BIT(p_active_functions, RANG_PO_CHAPV1);
    }
    
    _TIMER_T_0(INDEX_TIMER_CHAPV1, current_settings_prt.timeout_chapv_1[number_group_stp], po_f1_chapv_tmp, 0, tmp_value, 6);
    //L8
    _AND2(po_f1_achr, 0, !po_f1_chapv_tmp, 0, tmp_value, 12);
    //L9
    _OR2(tmp_value, 5, tmp_value, 6, tmp_value, 13);

    _D_TRIGGER(_GET_STATE(tmp_value, 12), 0, _GET_STATE(UF1_is_smaller_than_U_setpoint_F1, 0), previous_states_CHAPV1, 0, tmp_value, 13, trigger_CHAPV1, 0);
    if (tmp1 == trigger_CHAPV1) break;
    
    tmp1 = trigger_CHAPV1;
  } 
  while(1);
����������
*/
//----1----
  _Bool tmp1_1 = 0;
  do 
  {
    //L7_1
    _OR2(tmp_value1, 3, !trigger_CHAPV1, 0, tmp_value1, 30);
    
    //�� ����1
    _Bool po_f1_chapv1 = 0;
    if (previous_state_po_f1_chapv1) 
    {
      po_f1_chapv1 = !po_f1_achr1_rab;
    }
    else 
    {
      po_f1_chapv1 = po_f1_chapv1_rab;
    }
    //L3
    _Bool po_f1_chapv1_tmp = (!_GET_STATE(tmp_value1, 30) && po_f1_chapv1);
    if (po_f1_chapv1_tmp) 
      _SET_BIT(p_active_functions, RANG_PO_CHAPV1);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_CHAPV1);
    
    _TIMER_T_0(INDEX_TIMER_CHAPV1, current_settings_prt.timeout_chapv_1[number_group_stp], po_f1_chapv1_tmp, 0, tmp_value1, 6);
    //L8
    _AND2(po_f1_achr1, 0, !po_f1_chapv1_tmp, 0, tmp_value1, 12);
    //L9
    _OR2(tmp_value1, 5, tmp_value1, 6, tmp_value1, 13);

    _D_TRIGGER(_GET_STATE(tmp_value1, 12), 0, _GET_STATE(tmp_value1, 3), previous_states_CHAPV1, 0, tmp_value1, 13, trigger_CHAPV1, 0);
    if (tmp1_1 == trigger_CHAPV1) break;
    
    tmp1_1 = trigger_CHAPV1;
  } 
  while(1);
//----2----
  _Bool tmp1_2 = 0;
  do 
  {
    //L7_2
    _OR2(tmp_value2, 3, !trigger_CHAPV2, 0, tmp_value2, 30);
    
    //�� ����2
    _Bool po_f1_chapv2 = 0;
    if (previous_state_po_f1_chapv2) 
    {
      po_f1_chapv2 = !po_f1_achr2_rab;
    }
    else 
    {
      po_f1_chapv2 = po_f1_chapv2_rab;
    }
    //L3_2
    _Bool po_f1_chapv2_tmp = (!_GET_STATE(tmp_value2, 30) && po_f1_chapv2);
    if (po_f1_chapv2_tmp) 
      _SET_BIT(p_active_functions, RANG_PO_CHAPV2);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_CHAPV2);
    
    _TIMER_T_0(INDEX_TIMER_CHAPV2, current_settings_prt.timeout_chapv_2[number_group_stp], po_f1_chapv2_tmp, 0, tmp_value2, 6);
    //L8_2
    _AND2(po_f1_achr2, 0, !po_f1_chapv2_tmp, 0, tmp_value2, 12);
    //L9_2
    _OR2(tmp_value2, 5, tmp_value2, 6, tmp_value2, 13);

    _D_TRIGGER(_GET_STATE(tmp_value2, 12), 0, _GET_STATE(tmp_value2, 3), previous_states_CHAPV2, 0, tmp_value2, 13, trigger_CHAPV2, 0);
    if (tmp1_2 == trigger_CHAPV2) break;
    
    tmp1_2 = trigger_CHAPV2;
  } 
  while(1);
//---------
/*  
  //���/����
  if (_GET_STATE(trigger_CHAPV1, 0)) 
  {
    _SET_BIT(p_active_functions, RANG_ACHR_CHAPV1);
  }
  
  _Bool chapv_timer_1ms = 0;
  _Bool razr_chapv_inv = 0;
  _TIMER_0_T(INDEX_TIMER_CHAPV_1MS, TIMEOUT_CHAPV_1MS, trigger_CHAPV1, 0, chapv_timer_1ms, 0);
  _INVERTOR(UF1_is_larger_than_U_setpoint_F1, 0, razr_chapv_inv, 0);
  _AND2(chapv_timer_1ms, 0, razr_chapv_inv, 0, tmp_value, 24);
  _TIMER_0_T(INDEX_TIMER_BLOCK_CHAPV_5MS, TIMEOUT_BLOCK_CHAPV_5MS, tmp_value, 24, tmp_value, 25);
����������
*/
//----1----
  //���/����
  if (_GET_STATE(trigger_CHAPV1, 0)) 
    _SET_BIT(p_active_functions, RANG_ACHR_CHAPV1);
  else
    _CLEAR_BIT(p_active_functions, RANG_ACHR_CHAPV1);

//----2----
  //���/����
  if (_GET_STATE(trigger_CHAPV2, 0)) 
    _SET_BIT(p_active_functions, RANG_ACHR_CHAPV2);
  else
    _CLEAR_BIT(p_active_functions, RANG_ACHR_CHAPV2);

  //���������� ����
  unsigned int tmp_value_g = 0;
  _TIMER_0_T(INDEX_TIMER_CHAPV1_1MS, TIMEOUT_CHAPV_1MS, trigger_CHAPV1, 0, tmp_value_g, 0);
  _TIMER_0_T(INDEX_TIMER_CHAPV2_1MS, TIMEOUT_CHAPV_1MS, trigger_CHAPV2, 0, tmp_value_g, 1);
  
  _AND2(tmp_value_g, 0, tmp_value1, 3, tmp_value_g, 2);
  _AND2(tmp_value_g, 1, tmp_value2, 3, tmp_value_g, 3);
  
  _TIMER_0_T(INDEX_TIMER_BLOCK_CHAPV1_5MS, TIMEOUT_BLOCK_CHAPV_5MS, tmp_value_g, 2, tmp_value_g, 4);
  _TIMER_0_T(INDEX_TIMER_BLOCK_CHAPV2_5MS, TIMEOUT_BLOCK_CHAPV_5MS, tmp_value_g, 3, tmp_value_g, 5);
  
  _OR2(tmp_value_g, 4, tmp_value_g, 5, tmp_value_g, 6);

  if (_GET_STATE(tmp_value_g, 6))
      _SET_BIT(p_active_functions, RANG_VN_BLOCK_CHAPV);
  else
      _CLEAR_BIT(p_active_functions, RANG_VN_BLOCK_CHAPV);
}

/*****************************************************/

/*****************************************************/
// ���������� � ��
/*****************************************************/
void ready_tu(unsigned int *p_active_functions)
{
  unsigned int tmp_value = (_CHECK_SET_BIT(p_active_functions, RANG_PRYVID_VV) == 0)                  << 0;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_VIDKL_VID_ZAKHYSTIV) != 0)                    << 1;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV) != 0)      << 2;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_AVAR_DEFECT) == 0)                            << 3;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_MISCEVE_DYSTANCIJNE) == 0)                    << 6;
  
  _Bool ctrl_ready_tu = ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_READY_TU)) == 0);
  
  if (!previous_states_ready_tu && _GET_STATE(tmp_value, 1)) {
    previous_states_ready_tu = 0;
  }
  
  _OR2(tmp_value, 2, ctrl_ready_tu, 0, tmp_value, 7);
  _D_TRIGGER(1, 0, _GET_STATE(tmp_value, 7), previous_states_ready_tu, 0, tmp_value, 1, trigger_ready_tu, 0);
  
  _AND4(tmp_value, 0, tmp_value, 3, !trigger_ready_tu, 0, tmp_value, 6, tmp_value, 5);
  
  //���������� � ��
  if (_GET_STATE(tmp_value, 5))
    _SET_BIT(p_active_functions, RANG_READY_TU);
  else
    _CLEAR_BIT(p_active_functions, RANG_READY_TU);
}
/*****************************************************/

/*****************************************************/
// ����
/*****************************************************/
inline void urov_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  /*******************************/
  //��������� ������������ ������� ����� ��� ����
  /*******************************/
  unsigned int max_faze_current = measurement[IM_IA];
  if (max_faze_current < measurement[IM_IB]) max_faze_current = measurement[IM_IB];
  if (max_faze_current < measurement[IM_IC]) max_faze_current = measurement[IM_IC];
  /*******************************/
  
  /*******************************/
  uint32_t state_UROV = (( current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STATE)) != 0);
  uint32_t start_from_UP = false;
  if (
      (state_UROV != 0) &&
      ((current_settings_prt.control_urov & ((MASKA_FOR_BIT(NUMBER_UP) - 1) << INDEX_ML_CTRUROV_STARTED_FROM_UP1)) != 0)
     )   
  {
    //����������� ������ ���� �� ��
    for (size_t n_UP = 0; n_UP < NUMBER_UP; n_UP++)
    {
      if ( 
          ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_UP1 + n_UP)) != 0) && 
          (_CHECK_SET_BIT(p_active_functions, (RANG_UP1 + 3*n_UP)      ) != 0)
         )
      {
        start_from_UP = true;
      }
    }
  }
  
  if(
     (state_UROV != 0) &&
     (
      (start_from_UP == true) ||
      (_CHECK_SET_BIT(p_active_functions, RANG_PUSK_UROV_VID_DV) != 0) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ1)   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ1             ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ2)   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ2             ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ3)   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ3             ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ4)   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ4             ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_1)) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ04_1          ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_2)) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ04_2          ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_ZDZ)    ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_ZDZ              ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_3I0)    ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_3I0              ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_3U0)    ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_3U0              ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_NZZ)    ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_NZZ              ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_TZNP1)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_TZNP1            ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_TZNP2)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_TZNP2            ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_TZNP3)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_TZNP3            ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_ZOP1)   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_ZOP              ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_UMIN1)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_UMIN1            ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_UMIN2)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_UMIN2            ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_UMAX1)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_UMAX1            ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_UMAX2)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_UMAX2            ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_ACHR1)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_ACHR_CHAPV1      ) != 0)) ||
      ( ((current_settings_prt.control_urov & MASKA_FOR_BIT(INDEX_ML_CTRUROV_STARTED_FROM_ACHR2)  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_ACHR_CHAPV2      ) != 0))
     )     
    )
  {
    //� ����� ������� ����
    
    unsigned int setpoint; //������� - � ���� ���������� ����������� ��������
    unsigned int previous_state_po_urov;
    
    //���� �� ���� �� �� �������, �� ����� ��������� �� ������ ���������� - �������, ��� ��������� �� ������� � ������� ���� �� ��������� ����
    //���� �� ���� ��� ����������, �� ����� ��������� �� ������� ���������� - �������� ������� �� ������� ������ �� ����������� ����������
    if(( previous_state_po_urov = _CHECK_SET_BIT(p_active_functions, RANG_PO_UROV) ) ==0 )
    {
      //�������� �� ������ �����������
      setpoint = current_settings_prt.setpoint_urov[number_group_stp];
    }
    else
    {
      //�������� �� ������ ����������
      setpoint = current_settings_prt.setpoint_urov[number_group_stp]*KOEF_POVERNENNJA_GENERAL_UP/100;
    }
    
    //�����������, ��� ������� ������ "�� ����"
    if (max_faze_current >= setpoint)
    {
      //������������ ������ ���� �������� ���� �������
      if(previous_state_po_urov == 0)
      {
        //������������ ������ "�� ����"
        _SET_BIT(p_active_functions, RANG_PO_UROV);
      
        //��������� ������� ����1 � ����2, ���� ���� �� �� �������
        global_timers[INDEX_TIMER_UROV1] = 0;
        global_timers[INDEX_TIMER_UROV2] = 0;
      }
    }
    else 
    {
      //������������ ������ ���� ����� �������
      if(previous_state_po_urov != 0)
      {
        //������� ������ "�� ����"
        _CLEAR_BIT(p_active_functions, RANG_PO_UROV);
        //�� � ������ ����� �������� ������� "����. ����1" � "����. ����2"
        _CLEAR_BIT(p_active_functions, RANG_UROV1);
        _CLEAR_BIT(p_active_functions, RANG_UROV2);
        //���� ������� �� �� ������, �� ������� ��
        if ( global_timers[INDEX_TIMER_UROV1] >=0) global_timers[INDEX_TIMER_UROV1] = -1;
        if ( global_timers[INDEX_TIMER_UROV2] >=0) global_timers[INDEX_TIMER_UROV2] = -1;
      }
    }
    
    //���������� �� ������ ����1 �������� �������� �� ��������
    if(global_timers[INDEX_TIMER_UROV1] >= current_settings_prt.timeout_urov_1[number_group_stp])
    {
      //���� ��������� �������� ����1 �� ������������ ������ "����. ����1"
      _SET_BIT(p_active_functions, RANG_UROV1);

      //������� ������ ����1
      global_timers[INDEX_TIMER_UROV1] = -1;
    }

    //���������� �� ������ ����2 �������� �������� �� ��������
    if(global_timers[INDEX_TIMER_UROV2] >= current_settings_prt.timeout_urov_2[number_group_stp])
    {
      //���� ��������� �������� ����2 �� ������������ ������ "����. ����2"
      _SET_BIT(p_active_functions, RANG_UROV2);

      //������� ������ ����2
      global_timers[INDEX_TIMER_UROV2] = -1;
    }
  }
  else
  {
    //����� ������� �� ������� � �������, �� �� ���� ����������
    _CLEAR_BIT(p_active_functions, RANG_PO_UROV);
    _CLEAR_BIT(p_active_functions, RANG_UROV1);
    _CLEAR_BIT(p_active_functions, RANG_UROV2);
    global_timers[INDEX_TIMER_UROV1] = -1;
    global_timers[INDEX_TIMER_UROV2] = -1;
  }  
}
  /*******************************/
/*****************************************************/

/*****************************************************/
//���
/*****************************************************/
inline void apv_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  unsigned int logic_APV_0 = 0;
  unsigned int logic_APV_1 = 0;
  
  unsigned int extra_logic_APV_0 = 0;

  //������� ��������
  logic_APV_1 |= (start_restart != 0) << 19;
  //������� ����������� ��� ������� ��������
  start_restart = 0;

  //"���� ��������"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_STATE_VV) != 0) << 21;
  _TIMER_IMPULSE(INDEX_TIMER_APV_BLOCK_VID_VV, current_settings_prt.timeout_apv_block_vid_VV[number_group_stp], previous_states_APV_0, 0, logic_APV_0, 21, logic_APV_0, 22);

  //"�������� ����������"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_STAT_BLK_APV) != 0) << 0;
  _INVERTOR(logic_APV_0, 0, logic_APV_0, 1);

  //�:"���� �� ���1"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STARTED_FROM_MTZ1) != 0) << 2;
  //"���1"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_MTZ1) != 0) << 3;
  _AND2(logic_APV_0, 2, logic_APV_0, 3, logic_APV_0, 4);

  //�:"���� �� ���2"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STARTED_FROM_MTZ2) != 0) << 5;
  //"���2"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_MTZ2) != 0) << 6;
  _AND2(logic_APV_0, 5, logic_APV_0, 6, logic_APV_0, 7);

  //�:"���� �� ���3"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STARTED_FROM_MTZ3) != 0) << 8;
  //"���3"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_MTZ3) != 0) << 9;
  _AND2(logic_APV_0, 8, logic_APV_0, 9, logic_APV_0, 10);

  //�:"���� �� ���4"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STARTED_FROM_MTZ4) != 0) << 11;
  //"���4"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_MTZ4) != 0) << 12;
  _AND2(logic_APV_0, 11, logic_APV_0, 12, logic_APV_0, 13);

  _OR4(logic_APV_0, 4, logic_APV_0, 7, logic_APV_0, 10, logic_APV_0, 13, logic_APV_0, 14);

  //�:"����.�����.��.��"
  extra_logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_CTRAPV_BLK_CTRL_VV) != 0) << 8;
  _INVERTOR(extra_logic_APV_0, 8, extra_logic_APV_0, 9);
  _AND2(logic_APV_0, 21, extra_logic_APV_0, 8, extra_logic_APV_0, 10);
  _OR3(extra_logic_APV_0, 10, extra_logic_APV_0, 9, static_logic_APV_0, 0, extra_logic_APV_0, 0);
  _AND2(logic_APV_0, 14, extra_logic_APV_0, 0, static_logic_APV_0, 0);
  
  _TIMER_0_T(INDEX_TIMER_APV_TMP1, 1, static_logic_APV_0, 0, logic_APV_0, 15);

  //�:"���� 1"
  logic_APV_0 |= ((current_settings_prt.control_achr_chapv & CTR_CHAPV1) != 0) << 16;
  //"���/���� 1"
  logic_APV_1 |= (_CHECK_SET_BIT(p_active_functions, RANG_ACHR_CHAPV1) != 0) << 21;
  //�:"���� 2"
  logic_APV_1 |= ((current_settings_prt.control_achr_chapv & CTR_CHAPV2) != 0) << 27;
  //"���/���� 2"
  logic_APV_1 |= (_CHECK_SET_BIT(p_active_functions, RANG_ACHR_CHAPV2) != 0) << 28;
  //"���� ����"
  logic_APV_1 |= (_CHECK_SET_BIT(p_active_functions, RANG_RAZR_CHAPV) != 0) << 20;

  //"���/���� �� ��"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_ACHR_CHAPV_VID_DV) != 0) << 17;
  //�:"���� �� ��"
  logic_APV_1 |= ((current_settings_prt.control_achr_chapv & CTR_CHAPV_VID_DV) != 0) << 22;
  
  _AND3(logic_APV_0, 16, logic_APV_1, 21, logic_APV_1, 20, logic_APV_1, 23);
  _AND3(logic_APV_1, 27, logic_APV_1, 28, logic_APV_1, 20, logic_APV_1, 29);
  _AND2(logic_APV_0, 17, logic_APV_1, 22, logic_APV_1, 24);
  _OR3(logic_APV_1, 23, logic_APV_1, 29, logic_APV_1, 24, logic_APV_0, 18)

  //"����.����"
  logic_APV_1 |= (_CHECK_SET_BIT(p_active_functions, RANG_VN_BLOCK_CHAPV) != 0) << 25;
  _OR2(logic_APV_0, 18, logic_APV_1, 25, logic_APV_1, 26);
    
  _TIMER_0_T(INDEX_TIMER_ACHR_CHAPV, MAX_VALUE_TIMER_FOR_COUNT_SIGNAL_ACHR_CHAPV, logic_APV_0, 18, logic_APV_0, 19);

  _OR2_INVERTOR(logic_APV_0, 15, logic_APV_0, 19, logic_APV_0, 20);

  //�:"���"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STAGE_1) != 0) << 23;
  _INVERTOR(logic_APV_0, 23, logic_APV_0, 24);

  //�:"���2"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STAGE_2) != 0) << 25;
  _INVERTOR(logic_APV_0, 25, logic_APV_0, 26);

  //�:"���3"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STAGE_3) != 0) << 27;
  _INVERTOR(logic_APV_0, 27, logic_APV_0, 28);

  //�:"���4"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STAGE_4) != 0) << 29;
  _INVERTOR(logic_APV_0, 29, logic_APV_0, 30);

  //������� �������� ����������� ���������� ���� �� ������� �� ������������� � ���������� ����
  logic_APV_0 |= _GET_STATE(previous_states_APV_0, 1) << 31;
  logic_APV_1 |= _GET_STATE(previous_states_APV_0, 2) <<  0;
  logic_APV_1 |= _GET_STATE(previous_states_APV_0, 3) <<  1;
  logic_APV_1 |= _GET_STATE(previous_states_APV_0, 4) <<  2;
  
  extra_logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_UROV2) != 0) << 1;
  extra_logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_CTRAPV_BLK_CTRL_PRVV2) != 0) << 3;
  _AND2(extra_logic_APV_0, 1, extra_logic_APV_0, 3, extra_logic_APV_0, 11);
  _TIMER_0_T(INDEX_TIMER_APV_TMP2, 40, extra_logic_APV_0, 11, extra_logic_APV_0, 2);

  unsigned int previous_trigger_APV_0;
  
  /*
  ������� �������� �� �������, �� � ���� do-while ������ �� ����������,
  ��� ���� ����������� �� ��� ������ �� �������� ������, ���� �, ��������, ����
  �������� �������� ��� �������.
  ��� ����������� ����������� ��� �������, �� �� ������� ��� ����� ������, �
  � ���� ������ ����� ����������
  */
  _CLEAR_BIT(p_active_functions, RANG_APV1);
  _CLEAR_BIT(p_active_functions, RANG_APV2);
  _CLEAR_BIT(p_active_functions, RANG_APV3);
  _CLEAR_BIT(p_active_functions, RANG_APV4);
  do
  {
    /***
    Գ����� �������� ���� �������, �� ����, �������, �������� � �� ����
    ����� ���� �������� ���������� ������������ �������
    ***/
    _CLEAR_STATE(previous_states_APV_0, 1);
    previous_states_APV_0 |= _GET_STATE(logic_APV_0, 31) << 1;
    _CLEAR_STATE(previous_states_APV_0, 2);
    previous_states_APV_0 |= _GET_STATE(logic_APV_1,  0) << 2;
    _CLEAR_STATE(previous_states_APV_0, 3);
    previous_states_APV_0 |= _GET_STATE(logic_APV_1,  1) << 3;
    _CLEAR_STATE(previous_states_APV_0, 4);
    previous_states_APV_0 |= _GET_STATE(logic_APV_1,  2) << 4;
    
    previous_trigger_APV_0 = trigger_APV_0;
    /***/
    
    _OR2(logic_APV_1, 2, logic_APV_1, 1, logic_APV_1, 3);
    
    
    //���1
    _OR6(logic_APV_1, 19, static_logic_APV_0, 0, trigger_APV_0, 1, trigger_APV_0, 2, trigger_APV_0, 3, logic_APV_0, 31, logic_APV_1, 4);
    _OR6(logic_APV_1, 26, logic_APV_0, 22, logic_APV_0, 24, logic_APV_1, 3, logic_APV_1, 0, logic_APV_1, 4, logic_APV_1, 8);
    _OR2(logic_APV_1, 8, extra_logic_APV_0, 2, extra_logic_APV_0, 4);
    _D_TRIGGER(_GET_STATE(logic_APV_0, 1),  0, _GET_STATE(extra_logic_APV_0, 4), previous_states_APV_0, 5, logic_APV_0, 20, trigger_APV_0, 0);                 
    _TIMER_T_0(INDEX_TIMER_APV_1, current_settings_prt.timeout_apv_1[number_group_stp], trigger_APV_0, 0, logic_APV_1, 9);
    /*
    ��� ������ ����������� � ����� p_active_functions � ���� � ����������
    do-while, �� ��� �������� ����������� ��� ������, � ���� �� �����������, ��
    ��� ������ �� ������� ������ � ��� ������ ����������. ��� ������������ �� ��� "0"
    ���� ������������ "1" �� ���� � ����� ��� ������ � �� ���� ���� ����������� �� 
    �������� ������ ������� ������� (�������� ��� ������� ����������� �����
    ������ do-while).
    */
    if (_GET_STATE(logic_APV_1, 9)) _SET_BIT(p_active_functions, RANG_APV1);
    _TIMER_0_T(INDEX_TIMER_APV_BLOCK_VID_APV1, current_settings_prt.timeout_apv_block_vid_apv1[number_group_stp], logic_APV_1, 9, logic_APV_0, 31);

    //���2
    _AND2_INVERTOR(logic_APV_0, 31, logic_APV_0, 15, logic_APV_1, 10);
    _OR6(logic_APV_1, 19, static_logic_APV_0, 0, trigger_APV_0, 0, trigger_APV_0, 2, trigger_APV_0, 3, logic_APV_1,  0, logic_APV_1, 5);
    _OR6(logic_APV_1, 26, logic_APV_0, 24, logic_APV_0, 26, logic_APV_1, 2, logic_APV_1, 1, logic_APV_1, 5, logic_APV_1, 11);
    _OR2(logic_APV_1, 11, extra_logic_APV_0, 2, extra_logic_APV_0, 5);
    _D_TRIGGER(1,  0, _GET_STATE(extra_logic_APV_0, 5), previous_states_APV_0, 6, logic_APV_1, 10, trigger_APV_0, 1);    
    _TIMER_T_0(INDEX_TIMER_APV_2, current_settings_prt.timeout_apv_2[number_group_stp], trigger_APV_0, 1, logic_APV_1, 12);
    /*
    ��� ������ ����������� � ����� p_active_functions � ���� � ����������
    do-while, �� ��� �������� ����������� ��� ������, � ���� �� �����������, ��
    ��� ������ �� ������� ������ � ��� ������ ����������. ��� ������������ �� ��� "0"
    ���� ������������ "1" �� ���� � ����� ��� ������ � �� ���� ���� ����������� �� 
    �������� ������ ������� ������� (�������� ��� ������� ����������� �����
    ������ do-while).
    */
    if (_GET_STATE(logic_APV_1, 12)) _SET_BIT(p_active_functions, RANG_APV2);
    _TIMER_0_T(INDEX_TIMER_APV_BLOCK_VID_APV2, current_settings_prt.timeout_apv_block_vid_apv2[number_group_stp], logic_APV_1, 12, logic_APV_1, 0);

    //���3
    _AND2_INVERTOR(logic_APV_1, 0, logic_APV_0, 15, logic_APV_1, 13);
    _OR6(logic_APV_1, 19, static_logic_APV_0, 0, trigger_APV_0, 0, trigger_APV_0, 1, trigger_APV_0, 3, logic_APV_1,  1, logic_APV_1, 6);
    _OR5(logic_APV_1, 26, logic_APV_0, 24, logic_APV_0, 28, logic_APV_1, 2, logic_APV_1, 6, logic_APV_1, 14);
    _OR2(logic_APV_1, 14, extra_logic_APV_0, 2, extra_logic_APV_0, 6);
    _D_TRIGGER(1,  0, _GET_STATE(extra_logic_APV_0, 6), previous_states_APV_0, 7, logic_APV_1, 13, trigger_APV_0, 2);                 
    _TIMER_T_0(INDEX_TIMER_APV_3, current_settings_prt.timeout_apv_3[number_group_stp], trigger_APV_0, 2, logic_APV_1, 15);
    /*
    ��� ������ ����������� � ����� p_active_functions � ���� � ����������
    do-while, �� ��� �������� ����������� ��� ������, � ���� �� �����������, ��
    ��� ������ �� ������� ������ � ��� ������ ����������. ��� ������������ �� ��� "0"
    ���� ������������ "1" �� ���� � ����� ��� ������ � �� ���� ���� ����������� �� 
    �������� ������ ������� ������� (�������� ��� ������� ����������� �����
    ������ do-while).
    */
    if (_GET_STATE(logic_APV_1, 15)) _SET_BIT(p_active_functions, RANG_APV3);
    _TIMER_0_T(INDEX_TIMER_APV_BLOCK_VID_APV3, current_settings_prt.timeout_apv_block_vid_apv3[number_group_stp], logic_APV_1, 15, logic_APV_1, 1);

    //���4
    _AND2_INVERTOR(logic_APV_1, 1, logic_APV_0, 15, logic_APV_1, 16);
    _OR6(logic_APV_1, 19, static_logic_APV_0, 0, trigger_APV_0, 0, trigger_APV_0, 1, trigger_APV_0, 2, logic_APV_1,  2, logic_APV_1, 7);
    _OR4(logic_APV_1, 26, logic_APV_0, 24, logic_APV_0, 30, logic_APV_1, 7, logic_APV_1, 17);
    _OR2(logic_APV_1, 17, extra_logic_APV_0, 2, extra_logic_APV_0, 7);
    _D_TRIGGER(1,  0, _GET_STATE(extra_logic_APV_0, 7), previous_states_APV_0, 8, logic_APV_1, 16, trigger_APV_0, 3);                 
    _TIMER_T_0(INDEX_TIMER_APV_4, current_settings_prt.timeout_apv_4[number_group_stp], trigger_APV_0, 3, logic_APV_1, 18);
    /*
    ��� ������ ����������� � ����� p_active_functions � ���� � ����������
    do-while, �� ��� �������� ����������� ��� ������, � ���� �� �����������, ��
    ��� ������ �� ������� ������ � ��� ������ ����������. ��� ������������ �� ��� "0"
    ���� ������������ "1" �� ���� � ����� ��� ������ � �� ���� ���� ����������� �� 
    �������� ������ ������� ������� (�������� ��� ������� ����������� �����
    ������ do-while).
    */
    if (_GET_STATE(logic_APV_1, 18)) _SET_BIT(p_active_functions, RANG_APV4);
    _TIMER_0_T(INDEX_TIMER_APV_BLOCK_VID_APV4, current_settings_prt.timeout_apv_block_vid_apv4[number_group_stp], logic_APV_1, 18, logic_APV_1, 2);
  }
  while (
         (_GET_STATE(logic_APV_0, 31) != _GET_STATE(previous_states_APV_0, 1)) ||
         (_GET_STATE(logic_APV_1,  0) != _GET_STATE(previous_states_APV_0, 2)) ||
         (_GET_STATE(logic_APV_1,  1) != _GET_STATE(previous_states_APV_0, 3)) ||
         (_GET_STATE(logic_APV_1,  2) != _GET_STATE(previous_states_APV_0, 4)) ||
         ( trigger_APV_0                     !=  previous_trigger_APV_0                    )  
        );
  
  _Bool work_apv = 0;
  _OR4(trigger_APV_0, 0, trigger_APV_0, 1, trigger_APV_0, 2, trigger_APV_0, 3, work_apv, 0);
  
  //������ ���
  if (work_apv) _SET_BIT(p_active_functions, RANG_APV_WORK);
  else  _CLEAR_BIT(p_active_functions, RANG_APV_WORK);
  
}
/*****************************************************/

/*****************************************************/
// ������������ ������
/*****************************************************/
inline void up_handler(unsigned int *p_active_functions, unsigned int number_group_stp)
{
  for (size_t n_UP = 0; n_UP < NUMBER_UP; n_UP++)
  {
    uint32_t logic_UP_0 = 0;
    
    logic_UP_0 |= (
                   ((current_settings_prt.control_UP & MASKA_FOR_BIT(n_UP*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_STATE_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) != 0)
                   &&
                   (_CHECK_SET_BIT(p_active_functions, (RANG_BLOCK_UP1 + 3*n_UP)) == 0)
                  ) << 0;

    int32_t pickup = current_settings_prt.setpoint_UP[n_UP][0][number_group_stp];
    if (
        (current_settings_prt.ctrl_UP_input[n_UP] == UP_CTRL_3I0) ||
        (current_settings_prt.ctrl_UP_input[n_UP] == UP_CTRL_3I0_others)
       ) 
    {
      if (_CHECK_SET_BIT(p_active_functions, (RANG_PO_UP1 + 3*n_UP)) != 0)
      {
        /*
        ���������� ��������� ������
        setpoint = (pickup*koef_povernennja/100)*10 =  pickup*koef_povernennja/10
        */
        pickup = (pickup * (int32_t)current_settings_prt.setpoint_UP_KP[n_UP][0][number_group_stp])/10;
      }
      else
      {
        pickup *= 10;
      }
        
    }
    else
    {
      if (_CHECK_SET_BIT(p_active_functions, (RANG_PO_UP1 + 3*n_UP)) != 0) pickup = (pickup * (int32_t)current_settings_prt.setpoint_UP_KP[n_UP][0][number_group_stp])/100;
    }

    unsigned int more_less = ((current_settings_prt.control_UP & MASKA_FOR_BIT(n_UP*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_MORE_LESS_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) != 0);
    
    int32_t analog_value = 0;
    uint32_t PQ = false;
    uint32_t bank_for_calc_power_tmp = (state_calc_power == false ) ? bank_for_calc_power : ((bank_for_calc_power ^ 0x1) & 0x1);

    switch (current_settings_prt.ctrl_UP_input[n_UP])
    {
    case UP_CTRL_Ia_Ib_Ic:
      {
        analog_value = measurement[IM_IA];

        unsigned int or_and = ((current_settings_prt.control_UP & MASKA_FOR_BIT(n_UP*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) != 0);
        if (
            ((more_less == 0) && (or_and == 0)) ||
            ((more_less != 0) && (or_and != 0))
           )   
        {
         if ((uint32_t)analog_value < measurement[IM_IB]) analog_value = measurement[IM_IB];
         if ((uint32_t)analog_value < measurement[IM_IC]) analog_value = measurement[IM_IC];
        }
        else
        {
         if ((uint32_t)analog_value > measurement[IM_IB]) analog_value = measurement[IM_IB];
         if ((uint32_t)analog_value > measurement[IM_IC]) analog_value = measurement[IM_IC];
        }
        
        break;
      }
    case UP_CTRL_Ia:
      {
        analog_value = measurement[IM_IA];
        
        break;
      }
    case UP_CTRL_Ib:
      {
        analog_value = measurement[IM_IB];
        
        break;
      }
    case UP_CTRL_Ic:
      {
        analog_value = measurement[IM_IC];
        
        break;
      }
    case UP_CTRL_I1:
      {
        analog_value = measurement[IM_I1];
        
        break;
      }
    case UP_CTRL_I2:
      {
        analog_value = measurement[IM_I2];
        
        break;
      }
    case UP_CTRL_I04:
      {
        analog_value = measurement[IM_I04];
        
        break;
      }
    case UP_CTRL_3I0_r:
      {
        analog_value = measurement[IM_3I0_r];
        
        break;
      }
    case UP_CTRL_3I0:
      {
        analog_value = measurement[IM_3I0];
        
        break;
      }
    case UP_CTRL_3I0_others:
      {
        analog_value = measurement[IM_3I0_other_g];
        
        break;
      }
    case UP_CTRL_Ua_Ub_Uc:
      {
        uint32_t phase_line = ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0);
        analog_value = (phase_line == 0) ? measurement[IM_UA] : measurement[IM_UAB];
        
        unsigned int or_and = ((current_settings_prt.control_UP & MASKA_FOR_BIT(n_UP*(_CTR_UP_NEXT_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I) - _CTR_UP_PART_I) + CTR_UP_OR_AND_BIT - (_CTR_UP_PART_II - _CTR_UP_PART_I))) != 0);
        uint32_t analog_value_tmp = (phase_line == 0) ? measurement[IM_UB] : measurement[IM_UBC];
        if (
            ((more_less == 0) && (or_and == 0)) ||
            ((more_less != 0) && (or_and != 0))
           )   
        {
          if ((uint32_t)analog_value < analog_value_tmp) analog_value = analog_value_tmp;
         
          analog_value_tmp = (phase_line == 0) ? measurement[IM_UC] : measurement[IM_UCA];
          if ((uint32_t)analog_value < analog_value_tmp) analog_value = analog_value_tmp;
        }
        else
        {
          if ((uint32_t)analog_value > analog_value_tmp) analog_value = analog_value_tmp;
         
          analog_value_tmp = (phase_line == 0) ? measurement[IM_UC] : measurement[IM_UCA];
          if ((uint32_t)analog_value > analog_value_tmp) analog_value = analog_value_tmp;
        }
        
        break;
      }
    case UP_CTRL_Ua:
      {
        analog_value = ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) ? measurement[IM_UA] : measurement[IM_UAB];
        
        break;
      }
    case UP_CTRL_Ub:
      {
        analog_value = ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) ? measurement[IM_UB] : measurement[IM_UBC];
        
        break;
      }
    case UP_CTRL_Uc:
      {
        analog_value = ((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) ? measurement[IM_UC] : measurement[IM_UCA];
        
        break;
      }
    case UP_CTRL_U1:
      {
        analog_value = measurement[IM_U1];
        
        break;
      }
    case UP_CTRL_U2:
      {
        analog_value = measurement[IM_U2];
        
        break;
      }
    case UP_CTRL_3U0:
      {
        analog_value = measurement[IM_3U0];
        
        break;
      }
    case UP_CTRL_P:
      {
        PQ = true;
        analog_value = P[bank_for_calc_power_tmp];
        
        break;
      }
    case UP_CTRL_Q:
      {
        PQ = true;
        analog_value = Q[bank_for_calc_power_tmp];
        
        break;
      }
    case UP_CTRL_S:
      {
        analog_value = S[bank_for_calc_power_tmp];
        
        break;
      }
    default:
      {
        //���������� ����� ����� �� ���� � ����
        total_error_sw_fixed();
      }
    }
    
    if (more_less)
    {
      if (PQ)
      {
        if (
            (
             (analog_value >= 0) &&
             (pickup >= 0)  
            )
            ||
            (
             (analog_value < 0) &&
             (pickup < 0)  
            ) 
           ) 
        {
          logic_UP_0 |= (abs(analog_value) <= abs(pickup)) << 1;
        }
      }
      else
      {
        logic_UP_0 |= (analog_value <= pickup) << 1;
      }
    }
    else
    {
      if (PQ)
      {
        if (
            (
             (analog_value >= 0) &&
             (pickup >= 0)  
            )
            ||
            (
             (analog_value < 0) &&
             (pickup < 0)  
            ) 
           ) 
        {
          logic_UP_0 |= (abs(analog_value) >= abs(pickup)) << 1;
        }
      }
      else
      {
        logic_UP_0 |= (analog_value >= pickup) << 1;
      }
    }

    _AND2(logic_UP_0, 0, logic_UP_0, 1, logic_UP_0, 2);
    if (_GET_STATE(logic_UP_0, 2)) _SET_BIT(p_active_functions, (RANG_PO_UP1 + 3*n_UP));
    else _CLEAR_BIT(p_active_functions, (RANG_PO_UP1 + 3*n_UP));
    
    _TIMER_T_0(INDEX_TIMER_UP1 + n_UP, current_settings_prt.timeout_UP[n_UP][0][number_group_stp], logic_UP_0, 2, logic_UP_0, 3);
    if (_GET_STATE(logic_UP_0, 3)) _SET_BIT(p_active_functions, (RANG_UP1 + 3*n_UP));
    else _CLEAR_BIT(p_active_functions, (RANG_UP1 + 3*n_UP));
  }
}
/*****************************************************/

/*****************************************************/
//������� ��������� ������� ��������� � ����������
/*****************************************************/
inline void on_off_handler(unsigned int *p_active_functions)
{
  static unsigned int previous_active_functions[N_BIG];

  /*********************/
  //�������� ����������� �������
  /*********************/
  //������  ����������
  if (global_timers[INDEX_TIMER_VIDKL_VV] >= 0)
  {
    //������ �� ����� �������� � �� ����� ����� ����� ��������� ���� ������
    if (global_timers[INDEX_TIMER_VIDKL_VV] >= current_settings_prt.timeout_swch_off)
    {
      //������ �������� ����� ������������� ��������
      global_timers[INDEX_TIMER_VIDKL_VV] = -1;
      //³������ � ����� �������, �� ����� �����������, �� ���� �� �� ���� ������������
      _CLEAR_BIT(p_active_functions, RANG_WORK_BO);
    }
    //����������� ������ ����� �� ������, �� ������ ���������� �� �� ���� ��������� � ����������� � ����� ����������� �������,
    //��� �� ����� ���� �� ��������, ��� ���������� ��������� �������� �� ���� ��� � ������� ��������� ������ ����� ��
    global_timers[INDEX_TIMER_BLK_VKL_VV] = 0;
  }
    
  //������  ���������� ���������
  if (global_timers[INDEX_TIMER_BLK_VKL_VV] >= 0)
  {
    //������ ���������� ��������� �� ����� �������� � �� ����� ����� ����� ��������� ���� ������
    if (global_timers[INDEX_TIMER_BLK_VKL_VV] >= current_settings_prt.timeout_swch_udl_blk_on)
    {
      //������ �������� ����� ������������� ��������
      global_timers[INDEX_TIMER_BLK_VKL_VV] = -1;
    }
  }

  //������  ���������
  if (global_timers[INDEX_TIMER_VKL_VV] >= 0)
  {
    //������ �� ����� �������� � �� ����� ����� ����� ��������� ���� ������

    //���� �� ����� ������ ������ ��������� ������, ��� ����, �� ������� �� � ���������� ��������� �� �� ������, �� ������ ��������� ����� �������
    if ((global_timers[INDEX_TIMER_VIDKL_VV] >= 0) || (global_timers[INDEX_TIMER_BLK_VKL_VV] >=0))
    {
      global_timers[INDEX_TIMER_VKL_VV] = -1;
      //³������ � ����� �������, �� ����� �����������, �� ���� �B �� ���� ������������
      _CLEAR_BIT(p_active_functions, RANG_WORK_BV);
    }
    else
    {
      //����������, �� ������ ��������� �� �������� ����� ������������ ��������
      if (global_timers[INDEX_TIMER_VKL_VV] >= current_settings_prt.timeout_swch_on)
      {
        //������ �������� ����� ������������� ��������
        global_timers[INDEX_TIMER_VKL_VV] = -1;
        //³������ � ����� �������, �� ����� �����������, �� ���� �B �� ���� ������������
        _CLEAR_BIT(p_active_functions, RANG_WORK_BV);
      }
    }
  }
  /*********************/

  /*********************/
  //������ ������������ ���� ����������, �� �� ���� ��������� ��������� ��������
  /*********************/
  uint32_t off_cb_tmp[N_BIG];
  for(size_t m = 0; m < N_BIG; ++m) 
  {
    off_cb_tmp[m] = (p_active_functions[m] & current_settings_prt.ranguvannja_off_cb[m]);
  }
  
  /*
  ��� ������ �������������� ����� � ������ ��������, ���� �� ������������ ���� ����� �������,
  � ���� ���� ������� - �� ������������
  */
  _CLEAR_BIT(p_active_functions, RANG_VIDKL_VID_ZAKHYSTIV);
  int flag = 0;
  for(size_t m = 0; m < N_BIG; ++m)
  {
    if(off_cb_tmp[m] != 0) 
    {
      flag=1; 
      break;
    }
  }
  if (flag)
  {
    //� ����� ��������� ���� ���������
    _SET_BIT(p_active_functions, RANG_WORK_BO);

    //��������� (��� ���������� ���������� � 0, ���� �� ������� ������ ��������� ��) �������: ����� ��, ����������� ��.
    global_timers[INDEX_TIMER_VIDKL_VV  ] = 0;
    global_timers[INDEX_TIMER_BLK_VKL_VV] = 0;
    
    //������� ��������� ����� ���������
    _CLEAR_BIT(p_active_functions, RANG_WORK_BV);
    //������� ������ ���� ���������
    global_timers[INDEX_TIMER_VKL_VV] = -1;  

    /*
    ������� ������ "³��������� �� �������" (�� ����� �������� ����� �������
    ��������� ������� "������ ��" ����-���� �������� �� ����������� "�����. ��")
    */
    _CLEAR_BIT(off_cb_tmp, RANG_OTKL_VV);
    flag = 0;
    for(size_t m = 0; m < N_BIG; ++m) 
    {
      if(off_cb_tmp[m] != 0) 
      {
        flag = 1; 
        break;
      }
    }
    if (flag)
    {
      //��������� �� �������
      _SET_BIT(p_active_functions, RANG_VIDKL_VID_ZAKHYSTIV);
      
      /*****************************************************
      ��������� ���������� ��� ������� ���������� ��� ����
      *****************************************************/
      __info_vymk info_vymk_tmp = {time_dat, time_ms};
          
      //���1
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_MTZ1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ1) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_MTZ1);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ1] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_MTZ1);
      }
      
      //���2
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_MTZ2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ2) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_MTZ2);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ2] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_MTZ2);
      }
      
      //���3
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_MTZ3) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ3) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_MTZ3);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ3] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_MTZ3);
      }
      
      //���4
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_MTZ4) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ4) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_MTZ4);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ4] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_MTZ4);
      }
      
          //���1 04 ��
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_MTZ04_1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ04_1) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_MTZ04_1);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ04_1] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_MTZ04_1);
      }
          
      //���2 04��
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_MTZ04_2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ04_2) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_MTZ04_2);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ04_2] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_MTZ04_2);
      }
          
      //���
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_ZDZ) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_ZDZ) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_ZDZ);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ZDZ] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_ZDZ);
      }
      
      //��/3I0
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_3I0) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_3I0) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_3I0);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_3I0] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_3I0);
      }
          
      //��/3U0
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_3U0) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_3U0) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_3U0);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_3U0] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_3U0);
      }
          
      //���
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_NZZ) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_NZZ) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_NZZ);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_NZZ] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_NZZ);
      }
          
      //����1
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_TZNP1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_TZNP1) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_TZNP1);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_TZNP1] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_TZNP1);
      }

      //����2
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_TZNP2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_TZNP2) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_TZNP2);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_TZNP2] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_TZNP2);
      }

      //����3
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_TZNP3) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_TZNP3) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_TZNP3);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_TZNP3] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_TZNP3);
      }

      //���/���� �� ��
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_ACHR_CHAPV_VID_DV) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV_VID_DV) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_ACHR_CHAPV_VID_DV);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ACHR_CHAPV_VID_DV] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_ACHR_CHAPV_VID_DV);
      }
          
      //���/����1
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_ACHR_CHAPV1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV1) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_ACHR_CHAPV1);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ACHR_CHAPV1] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_ACHR_CHAPV1);
      }
          
      //���/����2
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_ACHR_CHAPV2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV2) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_ACHR_CHAPV2);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ACHR_CHAPV2] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_ACHR_CHAPV2);
      }
          
      //����1
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_UROV1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UROV1) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_UROV1);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UROV1] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_UROV1);
      }
      
      //����2
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_UROV2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UROV2) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_UROV2);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UROV2] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_UROV2);
      }
      
      //���
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_ZOP) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_ZOP) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_ZOP);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ZOP] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_ZOP);
      }
      
      //Umin1
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_UMIN1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UMIN1) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_UMIN1);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UMIN1] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_UMIN1);
      }
      
      //Umin2
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_UMIN2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UMIN2) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_UMIN2);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UMIN2] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_UMIN2);
      }
      
      //Umax1
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_UMAX1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UMAX1) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_UMAX1);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UMAX1] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_UMAX1);
      }
      
      //Umax2
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_UMAX2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UMAX2) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_UMAX2);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UMAX2] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_UMAX2);
      }
      
      //������������ ������
      for (size_t n_UP = 0; n_UP < NUMBER_UP; n_UP++)
      {
        if(
           (_CHECK_SET_BIT(off_cb_tmp, (RANG_UP1 + 3*n_UP)) != 0) &&
           (_CHECK_SET_BIT(previous_active_functions, (RANG_UP1 + 3*n_UP)) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
          )   
        {
          _SET_BIT(info_vidkluchennja_vymykacha, (VYMKNENNJA_VID_UP1 + n_UP));
          info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UP1 + n_UP] = info_vymk_tmp;

          _CLEAR_BIT(off_cb_tmp, (RANG_UP1 + 3*n_UP));
        }
      }
      
      //³��������� �� ������� �������
      if(
         (_CHECK_SET_BIT(off_cb_tmp, RANG_OTKL_VID_ZOVN_ZAHYSTIV) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_OTKL_VID_ZOVN_ZAHYSTIV) == 0) /*�����, �� ������ ����� ���������� (��� ����������� ��� ������)*/
        )   
      {
        _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_ZOVNISHNIKH_ZAKHYSTIV);
        info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ZOVNISHNIKH_ZAKHYSTIV] = info_vymk_tmp;

        _CLEAR_BIT(off_cb_tmp, RANG_OTKL_VID_ZOVN_ZAHYSTIV);
      }
      
      //³��������� �� ����� ������� (��� ���������� �� ������� "����.��")
      flag = 0;
      for(size_t m = 0; m < N_BIG; ++m) 
      {
        if(off_cb_tmp[m] != 0) 
        {
          flag = 1; 
          break;
        }
      }
      
      if (flag)
      {
        flag = 0;
        for(size_t m = 0; m < N_BIG; ++m) 
        {
          if(((previous_active_functions[m] & off_cb_tmp[m])!= off_cb_tmp[m])) 
          {
            flag = 1; 
            break;
          }
        }
        if(flag)   
        {
          _SET_BIT(info_vidkluchennja_vymykacha, VYMKNENNJA_VID_INSHYKH_SYGNALIV);
          info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_INSHYKH_SYGNALIV] = info_vymk_tmp;
        }
      }
      /*****************************************************/
    }
  }

  /*********************/
  //���� ������������ ���� ���������
  /*********************/
  if (
      (global_timers[INDEX_TIMER_VIDKL_VV  ] < 0) && 
      (global_timers[INDEX_TIMER_BLK_VKL_VV] < 0) &&
      (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_VKL_VV) == 0)
     )
  {
    //������� �� �������� ������� �� � ���������� ��������� ��, � ����� ���� ������� ���������� ��������� ��
    //���� ����������, �� ���� ����� ������� ��

    flag = 0;
    for(size_t m = 0; m < N_BIG; ++m) 
    {
      if((p_active_functions[m] & current_settings_prt.ranguvannja_on_cb[m]) != 0) 
      {
        flag = 1; 
        break;
      }
    }
    if (flag)
    {
      //³������ � ����� �������, �� ����� �����������, �� �� ����� ���������� ���� �� (���� �� �� �� ��������)
      _SET_BIT(p_active_functions, RANG_WORK_BV);

      //��������� (��� ���������� ���������� � 0, ���� �� ������� ������ ��������� ��) ������ ������ ��
      global_timers[INDEX_TIMER_VKL_VV] = 0;
    }
  }
  else
  {
    //�� ����� ������ ���� ���� ��� ����� ���� ���������� ��
    global_timers[INDEX_TIMER_VKL_VV] = -1;
    _CLEAR_BIT(p_active_functions, RANG_WORK_BV);
  }
  /*********************/

  /*********************/
  //������� ���������� ���� ������� ��� ������� ���������/���������
  /*********************/
  for (unsigned int i = 0; i < N_BIG; i++) previous_active_functions[i] = p_active_functions[i];
  /*********************/
}
/*****************************************************/

/*****************************************************/
//���������� ���� �� �����������
/*****************************************************/
inline void vmp_handler(unsigned int p_active_functions[])
{
  //���������� �� ����� ���������� ����������� �� �� �� ������ ����
  static unsigned int const maska_vmp_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_0,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_1,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_2,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_3,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_4,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_5,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_6,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_7
      
#ifdef MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_8
                                        ,
    MASKA_MONITOTYNG_PHASE_SIGNALES_KZ_8
#endif
  };
  
  unsigned int comp = false;
  COMPARE_NOT_ZERO_OR(comp, p_active_functions, maska_vmp_signals, N_BIG)
  
  if(comp)
  {
    //� ����� ��
    VMP_last_KZ = UNDEF_VMP; /*�������, �� ���������� ���� �� ����������� �� �� ���������*/
    
    //���������� �� �������� ������� �� ��������� �� ("������ ��")
    vymknennja_vid_KZ_prt |= (_CHECK_SET_BIT(p_active_functions, RANG_WORK_BO) != 0);

    
    //Գ����� ������������ ������ ����� ��� ��
    unsigned int max_faze_current = measurement[IM_IA];
    if (max_faze_current < measurement[IM_IB]) max_faze_current = measurement[IM_IB];
    if (max_faze_current < measurement[IM_IC]) max_faze_current = measurement[IM_IC];
    
    if (max_faze_current > I_max_KZ_prt)
    {
      //����������� ���� �������� ������������� ������� ������ ��� ��
      I_max_KZ_prt = max_faze_current;
      
      //��������� ������ ��� ���� ����� � ��
      unsigned int I_max_KZ_0_9 = (unsigned int)(0.9f*((float)max_faze_current));
      
      unsigned int number_of_phases_tmp = 0;
      if (measurement[IM_IA] >= I_max_KZ_0_9) number_of_phases_tmp++;
      if (measurement[IM_IB] >= I_max_KZ_0_9) number_of_phases_tmp++;
      if (measurement[IM_IC] >= I_max_KZ_0_9) number_of_phases_tmp++;
      if (number_of_phases_KZ_prt < number_of_phases_tmp) number_of_phases_KZ_prt = number_of_phases_tmp;
        
      int X_resistance[3] = {resistance[X_AB], resistance[X_BC], resistance[X_CA]};
      if (
          (((unsigned int)X_resistance[0]) != ((unsigned int)UNDEF_RESISTANCE)) ||
          (((unsigned int)X_resistance[1]) != ((unsigned int)UNDEF_RESISTANCE)) ||
          (((unsigned int)X_resistance[2]) != ((unsigned int)UNDEF_RESISTANCE))
         )
      {
        //� ������� ���������� ������ �����

        //Գ����� ��������� �������� ���������� ��� ��� ��
        //���� 1: �������� ������ ���������� ��������� ��� � ������� ���� �������� �� ��������
        unsigned int min_interphase_X = 0;
        int R_KZ_tmp = 0;
        if (((unsigned int)X_resistance[0]) != ((unsigned int)UNDEF_RESISTANCE))
        {
          min_interphase_X = abs(X_resistance[0]);
          R_KZ_tmp = resistance[R_AB];
        }
        else if (((unsigned int)X_resistance[1]) != ((unsigned int)UNDEF_RESISTANCE))
        {
          min_interphase_X = abs(X_resistance[1]);
          R_KZ_tmp = resistance[R_BC];
        }
        else if (((unsigned int)X_resistance[2]) != ((unsigned int)UNDEF_RESISTANCE))
        {
          min_interphase_X = abs(X_resistance[2]);
          R_KZ_tmp = resistance[R_CA];
        }
        else
        {
          //���������� ����� ����� �� ���� � ����
          total_error_sw_fixed();
        }
      
        //���� 2: ����� ��� ���������� �������� ����� ��������� ��������
        unsigned int X_mod;
        if ((((unsigned int)X_resistance[0]) != ((unsigned int)UNDEF_RESISTANCE)) && (min_interphase_X > (X_mod = abs(X_resistance[0])))) 
        {
          min_interphase_X = X_mod;
          R_KZ_tmp = resistance[R_AB];
        }
        if ((((unsigned int)X_resistance[1]) != ((unsigned int)UNDEF_RESISTANCE)) && (min_interphase_X > (X_mod = abs(X_resistance[1]))))
        {
          min_interphase_X = X_mod;
          R_KZ_tmp = resistance[R_BC];
        }
        if ((((unsigned int)X_resistance[2]) != ((unsigned int)UNDEF_RESISTANCE)) && (min_interphase_X > (X_mod = abs(X_resistance[2]))))
        {
          min_interphase_X = X_mod;
          R_KZ_tmp = resistance[R_CA];
        }
      
        //���� 3: ���������� �� ��������� ��� �� ��� ������ �� � ��������� � ������� ���������� ��
        if (
            (min_interphase_X != ((unsigned int)UNDEF_RESISTANCE))
            &&
            (
             (X_min_KZ_prt == ((unsigned int)UNDEF_RESISTANCE)) || /*�� � �������, �� ��� ������ �� �� ������ ��� ������� ��������� ���, ���� � ���� �������� ������� �� ��������*/
             (X_min_KZ_prt > min_interphase_X)
            )   
           )
        {
          //����������� ���� �������� ���������� ��������� ����� ��� ��
          X_min_KZ_prt = min_interphase_X;
          R_KZ_prt = R_KZ_tmp; //���� �� �� ����� ������� ����� ��� ���������� ����� (��� ��������� � ����� ������� �������� ��)
        }
      }
      else
      {
        X_min_KZ_prt = (unsigned int)UNDEF_RESISTANCE;
      }
    }
  }
  else
  {
    //�� ����� ������ ���� ������� ��
    if(
       (vymknennja_vid_KZ_prt != 0) &&  /*�����, �� ���������� ��������� �� ��� ���������� ��*/
       ((number_of_phases_last_KZ = number_of_phases_KZ_prt) > 1) && /*�����, �� �� � ��������*/
       ((current_settings_prt.control_vmp & CTR_VMP_STATE) != 0) && /*��� ��������*/ 
       (X_min_KZ_prt != ((unsigned int)UNDEF_RESISTANCE)) /*�����, �� ���� � ���� �������� ��� ��� ����������, � ���� � � ������������ ��������� ���������� �������� ���*/
      )
    {
      //��������� ������ �� ���� ����������� ���� �� ��� �������� 
      unsigned int X_tmp = X_min_KZ_prt*current_settings_prt.TVoltage/current_settings_prt.TCurrent; /*��������������� ��� �� �������� �������. �������� �������� � ���*/
      
      unsigned int forward_backward = (R_KZ_prt < 0);
      unsigned int lines_tmp = current_settings_prt.lines[forward_backward];
      unsigned int defined_VMP = false;
      unsigned int i = 0;
      int VMP_tmp = 0;
      while ((defined_VMP == false) && (i < lines_tmp))
      {
        unsigned int dovgyna_tmp = current_settings_prt.dovgyna[forward_backward][i];
        unsigned int opir_tmp = current_settings_prt.opir[forward_backward][i];
        unsigned int X_current_line = dovgyna_tmp*opir_tmp/1000; /*���������*[�]x[���/��] =  [�]x[���/1000�] =[���]/1000 */
        if (X_tmp <= X_current_line)
        {
          //�� ���� ������ ����������� �����������
//          VMP_tmp += (int)(((unsigned long long)dovgyna_tmp)*((unsigned long long)X_tmp)/((unsigned long long)X_current_line));
          VMP_tmp += X_tmp*1000/opir_tmp;
          defined_VMP = true;
        }
        else
        {
          //�� ���� ������ �� �� ����������� �����������
          VMP_tmp += dovgyna_tmp;
          X_tmp -= X_current_line;
        }
          
        i++;
      }
      
      //��������� ��������� ���
      equal_more_KZ = defined_VMP;
      if(forward_backward == 0) VMP_last_KZ = VMP_tmp;
      else VMP_last_KZ = -VMP_tmp;
    }

    /*
    ��������� �������� ������������ � ���� ���������� ������ ��
    
    �� �������� ���� � ����� ���� ���������� ����� ��� ���� ������������� ��,
    ��� �������  � �� ������ ��������� ��, � ����� ��������� ��� �� (������������ ��
    ���� �� �������� �� ���������, ��� ���� ������� ��������� �� �����������, � ��
    �� ���������) - �� ��� �� ������������ ��������, � ������ �� �������� ��������� ���� ���� ��
    � �����, �� �� �� ��� ������ � ���� � �� ���� ������ ������� ������ ���������
    */
    vymknennja_vid_KZ_prt = 0;
    I_max_KZ_prt = 0;
    number_of_phases_KZ_prt = 0;
    X_min_KZ_prt = (unsigned int)UNDEF_RESISTANCE;
    R_KZ_prt = 0;
  }
      
}
/*****************************************************/

/*****************************************************/
//�������� ������� ��������
/*****************************************************/
void control_VV(unsigned int *p_active_functions)
{
  unsigned int logic_control_VV_0 = 0;

  //"�������� ���."
  logic_control_VV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_CTRL_VKL ) != 0) << 0;
  //"�������� ����."
  logic_control_VV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_CTRL_OTKL) != 0) << 1;
  
  _XOR_INVERTOR(logic_control_VV_0, 0, logic_control_VV_0, 1, logic_control_VV_0, 2);

  _TIMER_T_0(INDEX_TIMER_PRYVOD_VV, current_settings_prt.timeout_pryvoda_VV, logic_control_VV_0, 2, logic_control_VV_0, 3);

  //�:"�������� ��"
  logic_control_VV_0 |= ((current_settings_prt.control_switch & CTR_PRYVOD_VV) != 0) << 4;
  
  _AND2(logic_control_VV_0, 3, logic_control_VV_0, 4, logic_control_VV_0, 5);
  
  if (_GET_STATE(logic_control_VV_0, 5))
    _SET_BIT(p_active_functions, RANG_PRYVID_VV);
  else
    _CLEAR_BIT(p_active_functions, RANG_PRYVID_VV);
}
/*****************************************************/

/*****************************************************/
//���������� ������������ ��� ��������� ������� ��������
/*****************************************************/
void make_koef_for_resurs(void)
{
  float tmp_1, tmp_2;
  
  tmp_1 = (float)current_settings.setpoint_r_kom_st_Inom/(float)current_settings.setpoint_r_kom_st_Inom_vymk;
  tmp_1  = log10f(tmp_1);

  tmp_2 = (float)current_settings.setpoint_Inom_vymk/(float)current_settings.setpoint_Inom;
  tmp_2  = log10f(tmp_2);
  
  //�������, �� ����������� ��� ���������� ������� �������� ����� ������ �����
  koef_resurs_changed = CHANGED_ETAP_EXECUTION;

  K_resurs = tmp_1/tmp_2;
  
  //�������, �� ����������� ��� ���������� ������� �������� ����� � ������� �� ������ �������� �������
  koef_resurs_changed = CHANGED_ETAP_ENDED;
}
/*****************************************************/

/*****************************************************/
//˳������� �������
/*****************************************************/
inline void resurs_vymykacha_handler(unsigned int *p_active_functions)
{
  static unsigned int previous_active_functions[N_BIG];
  
  if (restart_counter != 0)
  {
    //����� ������ ������� ������� ������� ��������
    resurs_vymykacha = current_settings_prt.setpoint_pochatkovyj_resurs;
    resurs_vidkljuchennja = current_settings_prt.setpoint_pochatkova_k_vymk;
    
     restart_counter = 0;

    //��������� ����� � EEPROM
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_RESURS_EEPROM_BIT);

    //�������, �� �������� ������� ������� ��������
    information_about_restart_counter |= (
                                            (1 << USB_RECUEST)
                                          | (1 << RS485_RECUEST)
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                          | (1 << LAN_RECUEST)
#endif
                                         );
  }
  
  /*******************************/
  //��������� ������������ ������� ����� �� ��� ������ ����� ��������� (��)
  /*******************************/
  static unsigned int max_faze_current_vv;
  if (_CHECK_SET_BIT(p_active_functions, RANG_WORK_BO) != 0)
  {
    unsigned int max_faze_current_tmp = measurement[IM_IA];
    if (max_faze_current_tmp < measurement[IM_IB]) max_faze_current_tmp = measurement[IM_IB];
    if (max_faze_current_tmp < measurement[IM_IC]) max_faze_current_tmp = measurement[IM_IC];
    max_faze_current_tmp = max_faze_current_tmp*current_settings_prt.TCurrent/1000;
  
    if (_CHECK_SET_BIT(previous_active_functions, RANG_WORK_BO) == 0)
    {
      //����������� ����� ������ ��
      max_faze_current_vv = max_faze_current_tmp;
    }
    else if (_CHECK_SET_BIT(previous_active_functions, RANG_WORK_BO) != 0)
    {
      //�� �������� ���� ������
      if (max_faze_current_vv < max_faze_current_tmp) max_faze_current_vv = max_faze_current_tmp;
    }
    
    if (max_faze_current_vv > current_settings_prt.setpoint_Inom_vymk) 
    {
      _SET_BIT(p_active_functions, RANG_PEREVYSHCHENNJA_Inom_VYMK);
    }
    else
    {
      _CLEAR_BIT(p_active_functions, RANG_PEREVYSHCHENNJA_Inom_VYMK);
    }
  }
  /*******************************/
  
  if (
      ((current_settings_prt.control_switch & CTR_RESURS_VV) != 0) &&
      (_CHECK_SET_BIT(previous_active_functions, RANG_WORK_BO) != 0) &&
      (_CHECK_SET_BIT(p_active_functions , RANG_WORK_BO) == 0)
     ) 
  {
    //����������� ���������� "������ ��"
    
    /*******************************/
    //������������ ������ �������a
    /*******************************/
    unsigned int N = 0;
    if (max_faze_current_vv >= current_settings_prt.setpoint_Inom_vymk) {
      N = (unsigned int) ceilf((float)current_settings_prt.setpoint_r_kom_st_Inom/(float)current_settings_prt.setpoint_r_kom_st_Inom_vymk);
    } else if (max_faze_current_vv <= current_settings_prt.setpoint_Inom) {
      N = 1;
    } else {
      float I_div_Inom = (float)max_faze_current_vv / (float)current_settings_prt.setpoint_Inom;
      float lg_I_div_Inom = log10f(I_div_Inom);
      float lg_N = K_resurs_prt * lg_I_div_Inom;
      float N_ = powf(10, lg_N);
      N = (unsigned int) ceilf(N_);
      if ((((float)N) - N_) >= 0.5f)
      {
        N--;
      }
      
    }
    
    if (resurs_vymykacha <= N) {
      resurs_vymykacha = 0;
    } else {
      resurs_vymykacha -= N;
    }
    
    //�������� ����� �����������
    if ((0xffffffff - resurs_vidkljuchennja) >= 1)
    {
      //��������� ������� �� ������� ������������
      resurs_vidkljuchennja++;
    }

    //��������� ����� � EEPROM
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_RESURS_EEPROM_BIT);
  }
  
  if (resurs_vymykacha <= current_settings_prt.setpoint_krytychnyj_resurs) 
  {
    _SET_BIT(p_active_functions, RANG_KRYTYCHNYJ_RESURS_VYMYKACHA);
  }
  else
  {
    _CLEAR_BIT(p_active_functions, RANG_KRYTYCHNYJ_RESURS_VYMYKACHA);
  }

  if (resurs_vymykacha == 0) 
  {
    _SET_BIT(p_active_functions, RANG_VYCHERPANYJ_RESURS_VYMYKACHA);
  }
  else
  {
    _CLEAR_BIT(p_active_functions, RANG_VYCHERPANYJ_RESURS_VYMYKACHA);
  }
 

  /*********************/
  //������� ���������� ���� ������� ��� ������� ���������/���������
  /*********************/
  for (unsigned int i = 0; i < N_BIG; i++) previous_active_functions[i] = p_active_functions[i];
  /*********************/
}
/*****************************************************/

/*****************************************************/
//�������� �� ����������� ���������� ������ �����������/����������� ����������
/*****************************************************/
inline unsigned int stop_regisrator(unsigned int* carrent_active_functions, unsigned int* ranguvannja_registrator)
{
  unsigned int stop = 0;

  {
    int flag = 1;
    for(size_t m = 0; m < N_BIG; ++m) 
    {
      if((carrent_active_functions[m] & ranguvannja_registrator[m]) != 0) 
      {
        flag = 0;
        break;
      }
    }
    if (flag)
    {
      //�����������, �� � ���� ������� ��������� ���������� ����� �� �������
      
      flag = 1;
      static unsigned int const maska_const[N_BIG] = 
      {
        MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_0,
        MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_1,
        MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_2,
        MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_3,
        MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_4,
        MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_5,
        MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_6,
        MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_7

#ifdef MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_8
                                               ,
        MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_8
#endif
      };
      for(size_t m = 0; m < N_BIG; ++m) 
      {
        if((carrent_active_functions[m] & maska_const[m]) != 0) 
        {
          flag = 0;
          break;
        }
      }
      if (flag)
      {
        //�����������, �� �� �������, �� ������ ���������� ��������� �������� ����� ������
          
        //����������, �� �� �������, �� ������� � ����� ����� ��������
        unsigned int global_timers_work = false;
        for (int *p = (global_timers + INDEX_TIMER_VIDKL_VV); ((global_timers_work == 0) && (p != (global_timers + _MAX_NUMBER_GLOBAL_TIMERS))); ++p)
        {
          if (*p >= 0) 
          {
            size_t shift = p - global_timers;
            if (
                (
                 (shift != INDEX_TIMER_PRYVOD_VV) ||
                 (  
                  ((current_settings_prt.control_switch & CTR_PRYVOD_VV) != 0) &&
                  (*p < current_settings_prt.timeout_pryvoda_VV) 
                 )   
                ) 
                &&
                (shift != INDEX_TIMER_ACHR_CHAPV_100MS_1)
                &&
                (shift != INDEX_TIMER_POSTFAULT)
                &&
                (shift != INDEX_TIMER_FULL_AR_RECORD)
               )
            global_timers_work = true;
          }
        }
          
        if (global_timers_work == 0)
        {
          //�����������, �� �� �������, �� �������� � ����� ��������
        
          //�������, �� ��������� ���� ���� ���������
          stop = 0xff;
        }
      }
    }
  }
  
  return stop;
}
/*****************************************************/

/*****************************************************/
//����������� ����������� ������� ������ ����������� ����������
/*****************************************************/
void fix_undefined_error_dr(unsigned int* carrent_active_functions)
{
  //����������� ������� � ������� � ��������� ���������
  _SET_BIT(set_diagnostyka, ERROR_DR_UNDEFINED_BIT);
  _SET_BIT(carrent_active_functions, RANG_DEFECT);
  //���������� ����� ������ � ����������� � ���� "�� ����� ������ ���� �� � ���������� ����������� �� ����������" 
  state_dr_record = STATE_DR_NO_RECORD;
  //������� ������ ������ ����������� ����������
  _CLEAR_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);
}
/*****************************************************/

/*****************************************************/
//������� ���������� ������������� ������� ������ 
/*****************************************************/
inline void start_monitoring_max_phase_current(unsigned int time_tmp)
{
  //�������� ������� ���������� ������� ������������� ������� ������
  number_max_phase_dr++;
  
  //�������, �� ����� �������� � ����, �� ����� �������� ����� ��������� �����������, ���� �������� �� �� ��������
  int frequency_int = (int)frequency;
  if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
  measurements_phase_max_dr[ 0] = measurement[IM_3I0];
  measurements_phase_max_dr[ 1] = measurement[IM_3I0_other_g];
  measurements_phase_max_dr[ 2] = measurement[IM_3I0_r];
  measurements_phase_max_dr[ 3] = measurement[IM_IA];
  measurements_phase_max_dr[ 4] = measurement[IM_IB];
  measurements_phase_max_dr[ 5] = measurement[IM_IC];
  measurements_phase_max_dr [6] = measurement[IM_I2];
  measurements_phase_max_dr[ 7] = measurement[IM_I1];
  measurements_phase_max_dr[ 8] = measurement[IM_I04];
  measurements_phase_max_dr[ 9] = measurement[IM_UA];
  measurements_phase_max_dr[10] = measurement[IM_UB];
  measurements_phase_max_dr[11] = measurement[IM_UC];
  measurements_phase_max_dr[12] = measurement[IM_3U0];
  measurements_phase_max_dr[13] = measurement[IM_U2];
  measurements_phase_max_dr[14] = measurement[IM_U1];
  measurements_phase_max_dr[15] = measurement[IM_UAB];
  measurements_phase_max_dr[16] = measurement[IM_UBC];
  measurements_phase_max_dr[17] = measurement[IM_UCA];
  measurements_phase_max_dr[18] = (unsigned int)frequency_int;
  measurements_phase_max_dr[19] = (unsigned int)resistance[R_AB];
  measurements_phase_max_dr[20] = (unsigned int)resistance[X_AB];
  measurements_phase_max_dr[21] = (unsigned int)resistance[R_BC];
  measurements_phase_max_dr[22] = (unsigned int)resistance[X_BC];
  measurements_phase_max_dr[23] = (unsigned int)resistance[R_CA];
  measurements_phase_max_dr[24] = (unsigned int)resistance[X_CA];
  measurements_phase_max_dr[25] = (unsigned int)UNDEF_VMP;
  measurements_phase_max_dr[26] = 0;
  
  //��������� ����������� ������ ����� �� ������ ������
  max_phase_current_dr = measurements_phase_max_dr[2];
  if (max_phase_current_dr < measurements_phase_max_dr[3]) max_phase_current_dr = measurements_phase_max_dr[3];
  if (max_phase_current_dr < measurements_phase_max_dr[4]) max_phase_current_dr = measurements_phase_max_dr[4];

  //Գ����� ��� � ������� ������� ��������� ������
  measurements_phase_max_dr[27] = time_tmp;

  //�������, �� �� �� ���䳿 ���������� ������������� ������� ������
  state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE);
}
/*****************************************************/

/*****************************************************/
//����������� ���������� ������������� ������� ������
/*****************************************************/
inline void continue_monitoring_max_phase_current(unsigned int time_tmp)
{
  //��������, �� �� � ����� ������ ����� ������, �� ��� �� �������� ������������
  if (
      (max_phase_current_dr < measurement[IM_IA]) ||
      (max_phase_current_dr < measurement[IM_IB]) ||
      (max_phase_current_dr < measurement[IM_IC])
     )
  {
    //����������� ��� ��� ��������� �������� ����� � ������� ������� ������������� �� ���
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
    measurements_phase_max_dr[ 0] = measurement[IM_3I0];
    measurements_phase_max_dr[ 1] = measurement[IM_3I0_other_g];
    measurements_phase_max_dr[ 2] = measurement[IM_3I0_r];
    measurements_phase_max_dr[ 3] = measurement[IM_IA];
    measurements_phase_max_dr[ 4] = measurement[IM_IB];
    measurements_phase_max_dr[ 5] = measurement[IM_IC];
    measurements_phase_max_dr [6] = measurement[IM_I2];
    measurements_phase_max_dr[ 7] = measurement[IM_I1];
    measurements_phase_max_dr[ 8] = measurement[IM_I04];
    measurements_phase_max_dr[ 9] = measurement[IM_UA];
    measurements_phase_max_dr[10] = measurement[IM_UB];
    measurements_phase_max_dr[11] = measurement[IM_UC];
    measurements_phase_max_dr[12] = measurement[IM_3U0];
    measurements_phase_max_dr[13] = measurement[IM_U2];
    measurements_phase_max_dr[14] = measurement[IM_U1];
    measurements_phase_max_dr[15] = measurement[IM_UAB];
    measurements_phase_max_dr[16] = measurement[IM_UBC];
    measurements_phase_max_dr[17] = measurement[IM_UCA];
    measurements_phase_max_dr[18] = (unsigned int)frequency_int;
    measurements_phase_max_dr[19] = (unsigned int)resistance[R_AB];
    measurements_phase_max_dr[20] = (unsigned int)resistance[X_AB];
    measurements_phase_max_dr[21] = (unsigned int)resistance[R_BC];
    measurements_phase_max_dr[22] = (unsigned int)resistance[X_BC];
    measurements_phase_max_dr[23] = (unsigned int)resistance[R_CA];
    measurements_phase_max_dr[24] = (unsigned int)resistance[X_CA];

    max_phase_current_dr = measurements_phase_max_dr[2];
    if (max_phase_current_dr < measurements_phase_max_dr[3]) max_phase_current_dr = measurements_phase_max_dr[3];
    if (max_phase_current_dr < measurements_phase_max_dr[4]) max_phase_current_dr = measurements_phase_max_dr[4];

    //Գ����� ��� � ������� ������� ��������� ������
    measurements_phase_max_dr[27] = time_tmp;
  }
}
/*****************************************************/

/*****************************************************/
//������� ���������� ������������� ������� ������ � ������� 0.4��
/*****************************************************/
inline void start_monitoring_max_phase04_current(unsigned int time_tmp)
{
  //�������� ������� ���������� ������� ������������� ������� ������ � ������� 0.4��
  number_max_phase04_dr++;
  
  //�������, �� ����� �������� � ����, �� ����� �������� ����� ��������� �����������, ���� �������� �� �� ��������
  int frequency_int = (int)frequency;
  if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
  measurements_phase04_max_dr[ 0] = measurement[IM_3I0];
  measurements_phase04_max_dr[ 1] = measurement[IM_3I0_other_g];
  measurements_phase04_max_dr[ 2] = measurement[IM_3I0_r];
  measurements_phase04_max_dr[ 3] = measurement[IM_IA];
  measurements_phase04_max_dr[ 4] = measurement[IM_IB];
  measurements_phase04_max_dr[ 5] = measurement[IM_IC];
  measurements_phase04_max_dr [6] = measurement[IM_I2];
  measurements_phase04_max_dr[ 7] = measurement[IM_I1];
  measurements_phase04_max_dr[ 8] = measurement[IM_I04];
  measurements_phase04_max_dr[ 9] = measurement[IM_UA];
  measurements_phase04_max_dr[10] = measurement[IM_UB];
  measurements_phase04_max_dr[11] = measurement[IM_UC];
  measurements_phase04_max_dr[12] = measurement[IM_3U0];
  measurements_phase04_max_dr[13] = measurement[IM_U2];
  measurements_phase04_max_dr[14] = measurement[IM_U1];
  measurements_phase04_max_dr[15] = measurement[IM_UAB];
  measurements_phase04_max_dr[16] = measurement[IM_UBC];
  measurements_phase04_max_dr[17] = measurement[IM_UCA];
  measurements_phase04_max_dr[18] = (unsigned int)frequency_int;
  measurements_phase04_max_dr[19] = (unsigned int)resistance[R_AB];
  measurements_phase04_max_dr[20] = (unsigned int)resistance[X_AB];
  measurements_phase04_max_dr[21] = (unsigned int)resistance[R_BC];
  measurements_phase04_max_dr[22] = (unsigned int)resistance[X_BC];
  measurements_phase04_max_dr[23] = (unsigned int)resistance[R_CA];
  measurements_phase04_max_dr[24] = (unsigned int)resistance[X_CA];
  measurements_phase04_max_dr[25] = (unsigned int)UNDEF_VMP;
  measurements_phase04_max_dr[26] = 0;
  
  //Գ����� ��� � ������� ������� ��������� ������
  measurements_phase04_max_dr[27] = time_tmp;

  //�������, �� �� �� ���䳿 ���������� ������������� ������� ������ � ������� 0.4��
  state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04);
}
/*****************************************************/

/*****************************************************/
//����������� ���������� ������������� ������� ������ � ������� 0.4��
/*****************************************************/
inline void continue_monitoring_max_phase04_current(unsigned int time_tmp)
{
  //��������, �� �� � ����� ������ ����� ������, �� ��� �� �������� ������������
  if(measurements_phase04_max_dr[8] < measurement[IM_I04])
  {
    //����������� ��� ��� ��������� �������� ����� � ������� ������� ������������� �� ���
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
    measurements_phase04_max_dr[ 0] = measurement[IM_3I0];
    measurements_phase04_max_dr[ 1] = measurement[IM_3I0_other_g];
    measurements_phase04_max_dr[ 2] = measurement[IM_3I0_r];
    measurements_phase04_max_dr[ 3] = measurement[IM_IA];
    measurements_phase04_max_dr[ 4] = measurement[IM_IB];
    measurements_phase04_max_dr[ 5] = measurement[IM_IC];
    measurements_phase04_max_dr [6] = measurement[IM_I2];
    measurements_phase04_max_dr[ 7] = measurement[IM_I1];
    measurements_phase04_max_dr[ 8] = measurement[IM_I04];
    measurements_phase04_max_dr[ 9] = measurement[IM_UA];
    measurements_phase04_max_dr[10] = measurement[IM_UB];
    measurements_phase04_max_dr[11] = measurement[IM_UC];
    measurements_phase04_max_dr[12] = measurement[IM_3U0];
    measurements_phase04_max_dr[13] = measurement[IM_U2];
    measurements_phase04_max_dr[14] = measurement[IM_U1];
    measurements_phase04_max_dr[15] = measurement[IM_UAB];
    measurements_phase04_max_dr[16] = measurement[IM_UBC];
    measurements_phase04_max_dr[17] = measurement[IM_UCA];
    measurements_phase04_max_dr[18] = (unsigned int)frequency_int;
    measurements_phase04_max_dr[19] = (unsigned int)resistance[R_AB];
    measurements_phase04_max_dr[20] = (unsigned int)resistance[X_AB];
    measurements_phase04_max_dr[21] = (unsigned int)resistance[R_BC];
    measurements_phase04_max_dr[22] = (unsigned int)resistance[X_BC];
    measurements_phase04_max_dr[23] = (unsigned int)resistance[R_CA];
    measurements_phase04_max_dr[24] = (unsigned int)resistance[X_CA];

    //Գ����� ��� � ������� ������� ��������� ������
    measurements_phase04_max_dr[27] = time_tmp;
  }
}
/*****************************************************/

/*****************************************************/
//������� ���������� ������������� ������ 3I0
/*****************************************************/
inline void start_monitoring_max_3I0(unsigned int time_tmp)
{
  //�������� ������� ���������� ������� ������������� 3I0
  number_max_3I0_dr++;
  
  //�������, �� ����� �������� � ����, �� ����� �������� ����� ��������� �����������, ���� �������� �� �� ��������
  int frequency_int = (int)frequency;
  if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
  measurements_3I0_max_dr[ 0] = measurement[IM_3I0];
  measurements_3I0_max_dr[ 1] = measurement[IM_3I0_other_g];
  measurements_3I0_max_dr[ 2] = measurement[IM_3I0_r];
  measurements_3I0_max_dr[ 3] = measurement[IM_IA];
  measurements_3I0_max_dr[ 4] = measurement[IM_IB];
  measurements_3I0_max_dr[ 5] = measurement[IM_IC];
  measurements_3I0_max_dr [6] = measurement[IM_I2];
  measurements_3I0_max_dr[ 7] = measurement[IM_I1];
  measurements_3I0_max_dr[ 8] = measurement[IM_I04];
  measurements_3I0_max_dr[ 9] = measurement[IM_UA];
  measurements_3I0_max_dr[10] = measurement[IM_UB];
  measurements_3I0_max_dr[11] = measurement[IM_UC];
  measurements_3I0_max_dr[12] = measurement[IM_3U0];
  measurements_3I0_max_dr[13] = measurement[IM_U2];
  measurements_3I0_max_dr[14] = measurement[IM_U1];
  measurements_3I0_max_dr[15] = measurement[IM_UAB];
  measurements_3I0_max_dr[16] = measurement[IM_UBC];
  measurements_3I0_max_dr[17] = measurement[IM_UCA];
  measurements_3I0_max_dr[18] = (unsigned int)frequency_int;
  measurements_3I0_max_dr[19] = (unsigned int)resistance[R_AB];
  measurements_3I0_max_dr[20] = (unsigned int)resistance[X_AB];
  measurements_3I0_max_dr[21] = (unsigned int)resistance[R_BC];
  measurements_3I0_max_dr[22] = (unsigned int)resistance[X_BC];
  measurements_3I0_max_dr[23] = (unsigned int)resistance[R_CA];
  measurements_3I0_max_dr[24] = (unsigned int)resistance[X_CA];
  measurements_3I0_max_dr[25] = (unsigned int)UNDEF_VMP;
  measurements_3I0_max_dr[26] = 0;
  
  //Գ����� ��� � ������� ������� ��������� ������
  measurements_3I0_max_dr[27] = time_tmp;
  
  //�������, �� �� �� ���䳿 ���������� ������������� ������ 3I0
  state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0);
}
/*****************************************************/

/*****************************************************/
//����������� ���������� ������������� ������ 3I0
/*****************************************************/
inline void continue_monitoring_max_3I0(unsigned int time_tmp)
{
  //��������, �� �� � ����� ����� 3I0 ������, �� ��� �� �������� ������������
  if(
     (
      ((current_settings_prt.configuration & (1<<TZNP_BIT_CONFIGURATION)) != 0) && 
      (                                                            (measurements_3I0_max_dr[2] < measurement[IM_3I0_r]))
     )   
     ||
     (
      ((current_settings_prt.configuration & (1<<TZNP_BIT_CONFIGURATION)) == 0) &&
      (
       (((current_settings_prt.control_zz & CTR_ZZ1_TYPE) == 0) && (measurements_3I0_max_dr[0] < measurement[IM_3I0])) ||
       (((current_settings_prt.control_zz & CTR_ZZ1_TYPE) != 0) && (measurements_3I0_max_dr[1] < measurement[IM_3I0_other_g]))
      )   
     )   
    )
  {
    //����������� ��� ��� ��������� ����� 3I0 � ������� ������������� �� ���
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
    measurements_3I0_max_dr[ 0] = measurement[IM_3I0];
    measurements_3I0_max_dr[ 1] = measurement[IM_3I0_other_g];
    measurements_3I0_max_dr[ 2] = measurement[IM_3I0_r];
    measurements_3I0_max_dr[ 3] = measurement[IM_IA];
    measurements_3I0_max_dr[ 4] = measurement[IM_IB];
    measurements_3I0_max_dr[ 5] = measurement[IM_IC];
    measurements_3I0_max_dr [6] = measurement[IM_I2];
    measurements_3I0_max_dr[ 7] = measurement[IM_I1];
    measurements_3I0_max_dr[ 8] = measurement[IM_I04];
    measurements_3I0_max_dr[ 9] = measurement[IM_UA];
    measurements_3I0_max_dr[10] = measurement[IM_UB];
    measurements_3I0_max_dr[11] = measurement[IM_UC];
    measurements_3I0_max_dr[12] = measurement[IM_3U0];
    measurements_3I0_max_dr[13] = measurement[IM_U2];
    measurements_3I0_max_dr[14] = measurement[IM_U1];
    measurements_3I0_max_dr[15] = measurement[IM_UAB];
    measurements_3I0_max_dr[16] = measurement[IM_UBC];
    measurements_3I0_max_dr[17] = measurement[IM_UCA];
    measurements_3I0_max_dr[18] = (unsigned int)frequency_int;
    measurements_3I0_max_dr[19] = (unsigned int)resistance[R_AB];
    measurements_3I0_max_dr[20] = (unsigned int)resistance[X_AB];
    measurements_3I0_max_dr[21] = (unsigned int)resistance[R_BC];
    measurements_3I0_max_dr[22] = (unsigned int)resistance[X_BC];
    measurements_3I0_max_dr[23] = (unsigned int)resistance[R_CA];
    measurements_3I0_max_dr[24] = (unsigned int)resistance[X_CA];

    //Գ����� ��� � ������� ������� ��������� ������
    measurements_3I0_max_dr[27] = time_tmp;
  }
}
/*****************************************************/

/*****************************************************/
//������� ���������� ����������� ������� 3U0
/*****************************************************/
inline void start_monitoring_max_3U0(unsigned int time_tmp)
{
  //�������� ������� ���������� ������� ����������� 3U0
  number_max_3U0_dr++;
  
  //�������, �� ����� �������� � ����, �� ����� �������� ����� ��������� �����������, ���� �������� �� �� ��������
  int frequency_int = (int)frequency;
  if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
  measurements_3U0_max_dr[ 0] = measurement[IM_3I0];
  measurements_3U0_max_dr[ 1] = measurement[IM_3I0_other_g];
  measurements_3U0_max_dr[ 2] = measurement[IM_3I0_r];
  measurements_3U0_max_dr[ 3] = measurement[IM_IA];
  measurements_3U0_max_dr[ 4] = measurement[IM_IB];
  measurements_3U0_max_dr[ 5] = measurement[IM_IC];
  measurements_3U0_max_dr[ 6] = measurement[IM_I2];
  measurements_3U0_max_dr[ 7] = measurement[IM_I1];
  measurements_3U0_max_dr[ 8] = measurement[IM_I04];
  measurements_3U0_max_dr[ 9] = measurement[IM_UA];
  measurements_3U0_max_dr[10] = measurement[IM_UB];
  measurements_3U0_max_dr[11] = measurement[IM_UC];
  measurements_3U0_max_dr[12] = measurement[IM_3U0];
  measurements_3U0_max_dr[13] = measurement[IM_U2];
  measurements_3U0_max_dr[14] = measurement[IM_U1];
  measurements_3U0_max_dr[15] = measurement[IM_UAB];
  measurements_3U0_max_dr[16] = measurement[IM_UBC];
  measurements_3U0_max_dr[17] = measurement[IM_UCA];
  measurements_3U0_max_dr[18] = (unsigned int)frequency_int;
  measurements_3U0_max_dr[19] = (unsigned int)resistance[R_AB];
  measurements_3U0_max_dr[20] = (unsigned int)resistance[X_AB];
  measurements_3U0_max_dr[21] = (unsigned int)resistance[R_BC];
  measurements_3U0_max_dr[22] = (unsigned int)resistance[X_BC];
  measurements_3U0_max_dr[23] = (unsigned int)resistance[R_CA];
  measurements_3U0_max_dr[24] = (unsigned int)resistance[X_CA];
  measurements_3U0_max_dr[25] = (unsigned int)UNDEF_VMP;
  measurements_3U0_max_dr[26] = 0;
  
  //Գ����� ��� � ������� ������� ��������� ������
  measurements_3U0_max_dr[27] = time_tmp;
  
  //�������, �� �� �� ���䳿 ���������� ����������� ������� 3U0
  state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0);
}
/*****************************************************/

/*****************************************************/
//����������� ���������� ������������� ������ 3U0
/*****************************************************/
inline void continue_monitoring_max_3U0(unsigned int time_tmp)
{
  //��������, �� �� � ����� ������� 3U0 �����, �� �� �� ������� ������������
  if(measurements_3U0_max_dr[12] < measurement[IM_3U0])
  {
    //����������� ��� ��� ��������� ����� 3I0 � ������� ������������� �� ���
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
    measurements_3U0_max_dr[ 0] = measurement[IM_3I0];
    measurements_3U0_max_dr[ 1] = measurement[IM_3I0_other_g];
    measurements_3U0_max_dr[ 2] = measurement[IM_3I0_r];
    measurements_3U0_max_dr[ 3] = measurement[IM_IA];
    measurements_3U0_max_dr[ 4] = measurement[IM_IB];
    measurements_3U0_max_dr[ 5] = measurement[IM_IC];
    measurements_3U0_max_dr[ 6] = measurement[IM_I2];
    measurements_3U0_max_dr[ 7] = measurement[IM_I1];
    measurements_3U0_max_dr[ 8] = measurement[IM_I04];
    measurements_3U0_max_dr[ 9] = measurement[IM_UA];
    measurements_3U0_max_dr[10] = measurement[IM_UB];
    measurements_3U0_max_dr[11] = measurement[IM_UC];
    measurements_3U0_max_dr[12] = measurement[IM_3U0];
    measurements_3U0_max_dr[13] = measurement[IM_U2];
    measurements_3U0_max_dr[14] = measurement[IM_U1];
    measurements_3U0_max_dr[15] = measurement[IM_UAB];
    measurements_3U0_max_dr[16] = measurement[IM_UBC];
    measurements_3U0_max_dr[17] = measurement[IM_UCA];
    measurements_3U0_max_dr[18] = (unsigned int)frequency_int;
    measurements_3U0_max_dr[19] = (unsigned int)resistance[R_AB];
    measurements_3U0_max_dr[20] = (unsigned int)resistance[X_AB];
    measurements_3U0_max_dr[21] = (unsigned int)resistance[R_BC];
    measurements_3U0_max_dr[22] = (unsigned int)resistance[X_BC];
    measurements_3U0_max_dr[23] = (unsigned int)resistance[R_CA];
    measurements_3U0_max_dr[24] = (unsigned int)resistance[X_CA];

    //Գ����� ��� � ������� ������� ��������� ������
    measurements_3U0_max_dr[27] = time_tmp;
  }
}
/*****************************************************/

/*****************************************************/
//������� ���������� �������� �����/����� ������� 
/*****************************************************/
inline void start_monitoring_min_U(unsigned int time_tmp)
{
  //�������� ������� ���������� ������� �������� �����/����� �������
  number_min_U_dr++;
  
  //�������, �� ����� �������� � ����, �� ����� �������� ����� ��������� �����������, ���� �������� �� �� ��������
  int frequency_int = (int)frequency;
  if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
  measurements_U_min_dr[ 0] = measurement[IM_3I0];
  measurements_U_min_dr[ 1] = measurement[IM_3I0_other_g];
  measurements_U_min_dr[ 2] = measurement[IM_3I0_r];
  measurements_U_min_dr[ 3] = measurement[IM_IA];
  measurements_U_min_dr[ 4] = measurement[IM_IB];
  measurements_U_min_dr[ 5] = measurement[IM_IC];
  measurements_U_min_dr[ 6] = measurement[IM_I2];
  measurements_U_min_dr[ 7] = measurement[IM_I1];
  measurements_U_min_dr[ 8] = measurement[IM_I04];
  measurements_U_min_dr[ 9] = measurement[IM_UA];
  measurements_U_min_dr[10] = measurement[IM_UB];
  measurements_U_min_dr[11] = measurement[IM_UC];
  measurements_U_min_dr[12] = measurement[IM_3U0];
  measurements_U_min_dr[13] = measurement[IM_U2];
  measurements_U_min_dr[14] = measurement[IM_U1];
  measurements_U_min_dr[15] = measurement[IM_UAB];
  measurements_U_min_dr[16] = measurement[IM_UBC];
  measurements_U_min_dr[17] = measurement[IM_UCA];
  measurements_U_min_dr[18] = (unsigned int)frequency_int;
  measurements_U_min_dr[19] = (unsigned int)resistance[R_AB];
  measurements_U_min_dr[20] = (unsigned int)resistance[X_AB];
  measurements_U_min_dr[21] = (unsigned int)resistance[R_BC];
  measurements_U_min_dr[22] = (unsigned int)resistance[X_BC];
  measurements_U_min_dr[23] = (unsigned int)resistance[R_CA];
  measurements_U_min_dr[24] = (unsigned int)resistance[X_CA];
  measurements_U_min_dr[25] = (unsigned int)UNDEF_VMP;
  measurements_U_min_dr[26] = 0;
  
  if (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0))
  {
    //��������� �������� ����� ������� �� ������ ������
    min_voltage_dr = measurements_U_min_dr[9];
    if (min_voltage_dr > measurements_U_min_dr[10]) min_voltage_dr = measurements_U_min_dr[10];
    if (min_voltage_dr > measurements_U_min_dr[11]) min_voltage_dr = measurements_U_min_dr[11];
  }
  else
  {
    //��������� �������� ����� ������� �� ������ ������
    min_voltage_dr = measurements_U_min_dr[15];
    if (min_voltage_dr > measurements_U_min_dr[16]) min_voltage_dr = measurements_U_min_dr[16];
    if (min_voltage_dr > measurements_U_min_dr[17]) min_voltage_dr = measurements_U_min_dr[17];
  }

  //Գ����� ��� � ������� ������� ��������� ������
  measurements_U_min_dr[27] = time_tmp;

  //�������, �� �� �� ���䳿 ���������� �������� �����/����� �������
  state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE);
}
/*****************************************************/

/*****************************************************/
//����������� ���������� �������� �����/����� �������
/*****************************************************/
inline void continue_monitoring_min_U(unsigned int time_tmp)
{
  //��������, �� �� � ����� ����������� ������ �����, �� �� �� ������� ���������
  if (
      (
       (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)) &&
       (  
        (min_voltage_dr > measurement[IM_UA]) ||
        (min_voltage_dr > measurement[IM_UB]) ||
        (min_voltage_dr > measurement[IM_UC])
       )
      )   
      || 
      (
       (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0) || ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) != 0)) &&
       (  
        (min_voltage_dr > measurement[IM_UAB]) ||
        (min_voltage_dr > measurement[IM_UBC]) ||
        (min_voltage_dr > measurement[IM_UCA])
       )
     )
    )    
  {
    //����������� ��� ��� ��������� ����������� ������ � ������� ������� ������������� �� ���
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
    measurements_U_min_dr[ 0] = measurement[IM_3I0];
    measurements_U_min_dr[ 1] = measurement[IM_3I0_other_g];
    measurements_U_min_dr[ 2] = measurement[IM_3I0_r];
    measurements_U_min_dr[ 3] = measurement[IM_IA];
    measurements_U_min_dr[ 4] = measurement[IM_IB];
    measurements_U_min_dr[ 5] = measurement[IM_IC];
    measurements_U_min_dr[ 6] = measurement[IM_I2];
    measurements_U_min_dr[ 7] = measurement[IM_I1];
    measurements_U_min_dr[ 8] = measurement[IM_I04];
    measurements_U_min_dr[ 9] = measurement[IM_UA];
    measurements_U_min_dr[10] = measurement[IM_UB];
    measurements_U_min_dr[11] = measurement[IM_UC];
    measurements_U_min_dr[12] = measurement[IM_3U0];
    measurements_U_min_dr[13] = measurement[IM_U2];
    measurements_U_min_dr[14] = measurement[IM_U1];
    measurements_U_min_dr[15] = measurement[IM_UAB];
    measurements_U_min_dr[16] = measurement[IM_UBC];
    measurements_U_min_dr[17] = measurement[IM_UCA];
    measurements_U_min_dr[18] = (unsigned int)frequency_int;
    measurements_U_min_dr[19] = (unsigned int)resistance[R_AB];
    measurements_U_min_dr[20] = (unsigned int)resistance[X_AB];
    measurements_U_min_dr[21] = (unsigned int)resistance[R_BC];
    measurements_U_min_dr[22] = (unsigned int)resistance[X_BC];
    measurements_U_min_dr[23] = (unsigned int)resistance[R_CA];
    measurements_U_min_dr[24] = (unsigned int)resistance[X_CA];

    if (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0))
    {
      //��������� �������� ����� ������� �� ������ ������
      min_voltage_dr = measurements_U_min_dr[9];
      if (min_voltage_dr > measurements_U_min_dr[10]) min_voltage_dr = measurements_U_min_dr[10];
      if (min_voltage_dr > measurements_U_min_dr[11]) min_voltage_dr = measurements_U_min_dr[11];
    }
    else
    {
      //��������� �������� ����� ������� �� ������ ������
      min_voltage_dr = measurements_U_min_dr[15];
      if (min_voltage_dr > measurements_U_min_dr[16]) min_voltage_dr = measurements_U_min_dr[16];
      if (min_voltage_dr > measurements_U_min_dr[17]) min_voltage_dr = measurements_U_min_dr[17];
    }

    //Գ����� ��� � ������� ������� ��������� ������
    measurements_U_min_dr[27] = time_tmp;
  }
}
/*****************************************************/

/*****************************************************/
//������� ���������� ����������� �����/����� ������� 
/*****************************************************/
inline void start_monitoring_max_U(unsigned int time_tmp)
{
  //�������� ������� ���������� ������� ����������� �����/����� �������
  number_max_U_dr++;
  
  //�������, �� ����� �������� � ����, �� ����� �������� ����� ��������� �����������, ���� �������� �� �� ��������
  int frequency_int = (int)frequency;
  if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
  measurements_U_max_dr[ 0] = measurement[IM_3I0];
  measurements_U_max_dr[ 1] = measurement[IM_3I0_other_g];
  measurements_U_max_dr[ 2] = measurement[IM_3I0_r];
  measurements_U_max_dr[ 3] = measurement[IM_IA];
  measurements_U_max_dr[ 4] = measurement[IM_IB];
  measurements_U_max_dr[ 5] = measurement[IM_IC];
  measurements_U_max_dr[ 6] = measurement[IM_I2];
  measurements_U_max_dr[ 7] = measurement[IM_I1];
  measurements_U_max_dr[ 8] = measurement[IM_I04];
  measurements_U_max_dr[ 9] = measurement[IM_UA];
  measurements_U_max_dr[10] = measurement[IM_UB];
  measurements_U_max_dr[11] = measurement[IM_UC];
  measurements_U_max_dr[12] = measurement[IM_3U0];
  measurements_U_max_dr[13] = measurement[IM_U2];
  measurements_U_max_dr[14] = measurement[IM_U1];
  measurements_U_max_dr[15] = measurement[IM_UAB];
  measurements_U_max_dr[16] = measurement[IM_UBC];
  measurements_U_max_dr[17] = measurement[IM_UCA];
  measurements_U_max_dr[18] = (unsigned int)frequency_int;
  measurements_U_max_dr[19] = (unsigned int)resistance[R_AB];
  measurements_U_max_dr[20] = (unsigned int)resistance[X_AB];
  measurements_U_max_dr[21] = (unsigned int)resistance[R_BC];
  measurements_U_max_dr[22] = (unsigned int)resistance[X_BC];
  measurements_U_max_dr[23] = (unsigned int)resistance[R_CA];
  measurements_U_max_dr[24] = (unsigned int)resistance[X_CA];
  measurements_U_max_dr[25] = (unsigned int)UNDEF_VMP;
  measurements_U_max_dr[26] = 0;
  
  if (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0))
  {
    //��������� ���������� ����� ������� �� ������ ������
    max_voltage_dr = measurements_U_max_dr[9];
    if (max_voltage_dr < measurements_U_max_dr[10]) max_voltage_dr = measurements_U_max_dr[10];
    if (max_voltage_dr < measurements_U_max_dr[11]) max_voltage_dr = measurements_U_max_dr[11];
  }
  else
  {
    //��������� ���������� ����� ������� �� ������ ������
    max_voltage_dr = measurements_U_max_dr[15];
    if (max_voltage_dr < measurements_U_max_dr[16]) max_voltage_dr = measurements_U_max_dr[16];
    if (max_voltage_dr < measurements_U_max_dr[17]) max_voltage_dr = measurements_U_max_dr[17];
  }

  //Գ����� ��� � ������� ������� ��������� ������
  measurements_U_max_dr[27] = time_tmp;

  //�������, �� �� �� ���䳿 ���������� ����������� �����/����� �������
  state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE);
}
/*****************************************************/

/*****************************************************/
//����������� ���������� ����������� �����/����� �������
/*****************************************************/
inline void continue_monitoring_max_U(unsigned int time_tmp)
{
  //��������, �� �� � ����� ����������� ������ �����, �� �� �� ������� ������������
  if (
      (
       (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0)) &&
       (  
        (max_voltage_dr < measurement[IM_UA]) ||
        (max_voltage_dr < measurement[IM_UB]) ||
        (max_voltage_dr < measurement[IM_UC])
       )
      )   
      || 
      (
       (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) != 0) || ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) != 0)) &&
       (  
        (max_voltage_dr < measurement[IM_UAB]) ||
        (max_voltage_dr < measurement[IM_UBC]) ||
        (max_voltage_dr < measurement[IM_UCA])
       )
     )
    )    
  {
    //����������� ��� ��� ��������� ����������� ������ � ������� ������� ������������� �� ���
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
    measurements_U_max_dr[ 0] = measurement[IM_3I0];
    measurements_U_max_dr[ 1] = measurement[IM_3I0_other_g];
    measurements_U_max_dr[ 2] = measurement[IM_3I0_r];
    measurements_U_max_dr[ 3] = measurement[IM_IA];
    measurements_U_max_dr[ 4] = measurement[IM_IB];
    measurements_U_max_dr[ 5] = measurement[IM_IC];
    measurements_U_max_dr[ 6] = measurement[IM_I2];
    measurements_U_max_dr[ 7] = measurement[IM_I1];
    measurements_U_max_dr[ 8] = measurement[IM_I04];
    measurements_U_max_dr[ 9] = measurement[IM_UA];
    measurements_U_max_dr[10] = measurement[IM_UB];
    measurements_U_max_dr[11] = measurement[IM_UC];
    measurements_U_max_dr[12] = measurement[IM_3U0];
    measurements_U_max_dr[13] = measurement[IM_U2];
    measurements_U_max_dr[14] = measurement[IM_U1];
    measurements_U_max_dr[15] = measurement[IM_UAB];
    measurements_U_max_dr[16] = measurement[IM_UBC];
    measurements_U_max_dr[17] = measurement[IM_UCA];
    measurements_U_max_dr[18] = (unsigned int)frequency_int;
    measurements_U_max_dr[19] = (unsigned int)resistance[R_AB];
    measurements_U_max_dr[20] = (unsigned int)resistance[X_AB];
    measurements_U_max_dr[21] = (unsigned int)resistance[R_BC];
    measurements_U_max_dr[22] = (unsigned int)resistance[X_BC];
    measurements_U_max_dr[23] = (unsigned int)resistance[R_CA];
    measurements_U_max_dr[24] = (unsigned int)resistance[X_CA];

    if (((current_settings_prt.control_extra_settings_1 & MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE)) == 0) && ((current_settings_prt.control_transformator & MASKA_FOR_BIT(INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)) == 0))
    {
      //��������� ���������� ����� ������� �� ������ ������
      max_voltage_dr = measurements_U_max_dr[9];
      if (max_voltage_dr < measurements_U_max_dr[10]) max_voltage_dr = measurements_U_max_dr[10];
      if (max_voltage_dr < measurements_U_max_dr[11]) max_voltage_dr = measurements_U_max_dr[11];
    }
    else
    {
      //��������� ���������� ����� ������� �� ������ ������
      max_voltage_dr = measurements_U_max_dr[15];
      if (max_voltage_dr < measurements_U_max_dr[16]) max_voltage_dr = measurements_U_max_dr[16];
      if (max_voltage_dr < measurements_U_max_dr[17]) max_voltage_dr = measurements_U_max_dr[17];
    }

    //Գ����� ��� � ������� ������� ��������� ������
    measurements_U_max_dr[27] = time_tmp;
  }
}
/*****************************************************/

/*****************************************************/
//������� ���������� ������������� ������  ��������� �����������
/*****************************************************/
inline void start_monitoring_max_ZOP(unsigned int time_tmp)
{
  //�������� ������� ���������� ������� ������������� 3I0
  number_max_ZOP_dr++;
  
  //�������, �� ����� �������� � ����, �� ����� �������� ����� ��������� �����������, ���� �������� �� �� ��������
  int frequency_int = (int)frequency;
  if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
  measurements_ZOP_max_dr[ 0] = measurement[IM_3I0];
  measurements_ZOP_max_dr[ 1] = measurement[IM_3I0_other_g];
  measurements_ZOP_max_dr[ 2] = measurement[IM_3I0_r];
  measurements_ZOP_max_dr[ 3] = measurement[IM_IA];
  measurements_ZOP_max_dr[ 4] = measurement[IM_IB];
  measurements_ZOP_max_dr[ 5] = measurement[IM_IC];
  measurements_ZOP_max_dr[ 6] = measurement[IM_I2];
  measurements_ZOP_max_dr[ 7] = measurement[IM_I1];
  measurements_ZOP_max_dr[ 8] = measurement[IM_I04];
  measurements_ZOP_max_dr[ 9] = measurement[IM_UA];
  measurements_ZOP_max_dr[10] = measurement[IM_UB];
  measurements_ZOP_max_dr[11] = measurement[IM_UC];
  measurements_ZOP_max_dr[12] = measurement[IM_3U0];
  measurements_ZOP_max_dr[13] = measurement[IM_U2];
  measurements_ZOP_max_dr[14] = measurement[IM_U1];
  measurements_ZOP_max_dr[15] = measurement[IM_UAB];
  measurements_ZOP_max_dr[16] = measurement[IM_UBC];
  measurements_ZOP_max_dr[17] = measurement[IM_UCA];
  measurements_ZOP_max_dr[18] = (unsigned int)frequency_int;
  measurements_ZOP_max_dr[19] = (unsigned int)resistance[R_AB];
  measurements_ZOP_max_dr[20] = (unsigned int)resistance[X_AB];
  measurements_ZOP_max_dr[21] = (unsigned int)resistance[R_BC];
  measurements_ZOP_max_dr[22] = (unsigned int)resistance[X_BC];
  measurements_ZOP_max_dr[23] = (unsigned int)resistance[R_CA];
  measurements_ZOP_max_dr[24] = (unsigned int)resistance[X_CA];
  measurements_ZOP_max_dr[25] = (unsigned int)UNDEF_VMP;
  measurements_ZOP_max_dr[26] = 0;

  //Գ����� ��� � ������� ������� ��������� ������
  measurements_ZOP_max_dr[27] = time_tmp;

  //�������, �� �� �� ���䳿 ���������� ������������� ������ ��������� �����������
  state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP);
}
/*****************************************************/

/*****************************************************/
//����������� ���������� ������������� ������ ��������� �����������
/*****************************************************/
inline void continue_monitoring_max_ZOP(unsigned int time_tmp)
{
  //��������, �� �� � ����� ����� ��������� ����������� ������, �� ��� �� �������� ������������
/*
      _I2 - ����� ��������� ����������� ��� ������������ ������������� ������������� �������� I2/I1
      _I1 - ����� ����� ����������� ��� ������������ ������������� ������������� �������� I2/I1
      
      I2 -  ������� ����� ��������� �����������
      I1 -  ������� ����� ����� �����������
      
      ����� ���������� ���������:
      _I2     I2
      ---  < --- /x �� _I1*I1  
      _I1     I1
      
      (_I2*I1 < I2*_I1)  - �� � �����, �� ����� � ����� ���(���) �� ��������� ���
*/
  unsigned int I2, I1, _I1, _I2;
   I2 = measurement[IM_I2];
   I1 = measurement[IM_I1];
  _I2 = measurements_ZOP_max_dr[6];
  _I1 = measurements_ZOP_max_dr[7];
  if (
      ( (_I1 >  0) && ( ((_I2*I1) < (I2*_I1)) || (I1 == 0) ) ) ||
      ( (_I1 == 0) && (I1 == 0) && ( _I2 < I2 ) ) 
     )
  {
    //����������� ��� ��� ��������� ����� ��������� ����������� � ������� ������� ������
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
    measurements_ZOP_max_dr[ 0] = measurement[IM_3I0];
    measurements_ZOP_max_dr[ 1] = measurement[IM_3I0_other_g];
    measurements_ZOP_max_dr[ 2] = measurement[IM_3I0_r];
    measurements_ZOP_max_dr[ 3] = measurement[IM_IA];
    measurements_ZOP_max_dr[ 4] = measurement[IM_IB];
    measurements_ZOP_max_dr[ 5] = measurement[IM_IC];
    measurements_ZOP_max_dr[ 6] = measurement[IM_I2];
    measurements_ZOP_max_dr[ 7] = measurement[IM_I1];
    measurements_ZOP_max_dr[ 8] = measurement[IM_I04];
    measurements_ZOP_max_dr[ 9] = measurement[IM_UA];
    measurements_ZOP_max_dr[10] = measurement[IM_UB];
    measurements_ZOP_max_dr[11] = measurement[IM_UC];
    measurements_ZOP_max_dr[12] = measurement[IM_3U0];
    measurements_ZOP_max_dr[13] = measurement[IM_U2];
    measurements_ZOP_max_dr[14] = measurement[IM_U1];
    measurements_ZOP_max_dr[15] = measurement[IM_UAB];
    measurements_ZOP_max_dr[16] = measurement[IM_UBC];
    measurements_ZOP_max_dr[17] = measurement[IM_UCA];
    measurements_ZOP_max_dr[18] = (unsigned int)frequency_int;
    measurements_ZOP_max_dr[19] = (unsigned int)resistance[R_AB];
    measurements_ZOP_max_dr[20] = (unsigned int)resistance[X_AB];
    measurements_ZOP_max_dr[21] = (unsigned int)resistance[R_BC];
    measurements_ZOP_max_dr[22] = (unsigned int)resistance[X_BC];
    measurements_ZOP_max_dr[23] = (unsigned int)resistance[R_CA];
    measurements_ZOP_max_dr[24] = (unsigned int)resistance[X_CA];

    //Գ����� ��� � ������� ������� ��������� ������
    measurements_ZOP_max_dr[27] = time_tmp;
  }
}
/*****************************************************/

/*****************************************************/
//������� ���������� �������� ������� 1 
/*****************************************************/
inline void start_monitoring_min_f(unsigned int time_tmp)
{
  //�������� ������� ���������� ������� �������� ������� 1
  number_min_f_achr_dr++;
  
  //�������, �� ����� �������� � ����, �� ����� �������� ����� ��������� �����������, ���� �������� �� �� ��������
  int frequency_int = (int)frequency;
  if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
  measurements_f_min_achr_dr[ 0] = measurement[IM_3I0];
  measurements_f_min_achr_dr[ 1] = measurement[IM_3I0_other_g];
  measurements_f_min_achr_dr[ 2] = measurement[IM_3I0_r];
  measurements_f_min_achr_dr[ 3] = measurement[IM_IA];
  measurements_f_min_achr_dr[ 4] = measurement[IM_IB];
  measurements_f_min_achr_dr[ 5] = measurement[IM_IC];
  measurements_f_min_achr_dr[ 6] = measurement[IM_I2];
  measurements_f_min_achr_dr[ 7] = measurement[IM_I1];
  measurements_f_min_achr_dr[ 8] = measurement[IM_I04];
  measurements_f_min_achr_dr[ 9] = measurement[IM_UA];
  measurements_f_min_achr_dr[10] = measurement[IM_UB];
  measurements_f_min_achr_dr[11] = measurement[IM_UC];
  measurements_f_min_achr_dr[12] = measurement[IM_3U0];
  measurements_f_min_achr_dr[13] = measurement[IM_U2];
  measurements_f_min_achr_dr[14] = measurement[IM_U1];
  measurements_f_min_achr_dr[15] = measurement[IM_UAB];
  measurements_f_min_achr_dr[16] = measurement[IM_UBC];
  measurements_f_min_achr_dr[17] = measurement[IM_UCA];
  measurements_f_min_achr_dr[18] = (unsigned int)frequency_int;
  measurements_f_min_achr_dr[19] = (unsigned int)resistance[R_AB];
  measurements_f_min_achr_dr[20] = (unsigned int)resistance[X_AB];
  measurements_f_min_achr_dr[21] = (unsigned int)resistance[R_BC];
  measurements_f_min_achr_dr[22] = (unsigned int)resistance[X_BC];
  measurements_f_min_achr_dr[23] = (unsigned int)resistance[R_CA];
  measurements_f_min_achr_dr[24] = (unsigned int)resistance[X_CA];
  measurements_f_min_achr_dr[25] = (unsigned int)UNDEF_VMP;
  measurements_f_min_achr_dr[26] = 0;
  
  //Գ����� ��� � ������� ������� ��������� ������
  measurements_f_min_achr_dr[27] = time_tmp;

  //�������, �� �� �� ���䳿 ���������� �������� �������
  state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR);
}
/*****************************************************/

/*****************************************************/
//����������� ���������� �������� ������� 1
/*****************************************************/
inline void continue_monitoring_min_f(unsigned int time_tmp)
{
  if (((int)measurements_f_min_achr_dr[18]) != (-2)) /*�� �����������, �� ������� ���� ����� ������ ����������� ���������� MIN_FREQUENCY - ������ ��� ��������� �������� ������� ���� �����*/
  {
    //��������, �� �� � ����� ����������� ������� �����, �� �� �� ������� ���������
    int frequency_int = (int)frequency;
    if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
    if(
       (frequency_int != (-1)) && /*������� �� ���������*/
       (frequency_int != (-3)) &&  /*������� ���� ������ ����������� ���������� MAX_FREQUENCY*/
       (
        (((int)measurements_f_min_achr_dr[18]) > frequency_int) ||
        (frequency_int == (-2)) /*������� ����� ������ ����������� ���������� MIN_FREQUENCY - �� �������� ������, �� � ��������� ������ ���� ����� ��� �������� ������� ���� �� ����� -1 ��� -3*/
       )   
      )   
    {
      //����������� ��� ��� ��������� ����������� ������ � ������� ������� ������������� �� ���
      measurements_f_min_achr_dr[ 0] = measurement[IM_3I0];
      measurements_f_min_achr_dr[ 1] = measurement[IM_3I0_other_g];
      measurements_f_min_achr_dr[ 2] = measurement[IM_3I0_r];
      measurements_f_min_achr_dr[ 3] = measurement[IM_IA];
      measurements_f_min_achr_dr[ 4] = measurement[IM_IB];
      measurements_f_min_achr_dr[ 5] = measurement[IM_IC];
      measurements_f_min_achr_dr[ 6] = measurement[IM_I2];
      measurements_f_min_achr_dr[ 7] = measurement[IM_I1];
      measurements_f_min_achr_dr[ 8] = measurement[IM_I04];
      measurements_f_min_achr_dr[ 9] = measurement[IM_UA];
      measurements_f_min_achr_dr[10] = measurement[IM_UB];
      measurements_f_min_achr_dr[11] = measurement[IM_UC];
      measurements_f_min_achr_dr[12] = measurement[IM_3U0];
      measurements_f_min_achr_dr[13] = measurement[IM_U2];
      measurements_f_min_achr_dr[14] = measurement[IM_U1];
      measurements_f_min_achr_dr[15] = measurement[IM_UAB];
      measurements_f_min_achr_dr[16] = measurement[IM_UBC];
      measurements_f_min_achr_dr[17] = measurement[IM_UCA];
      measurements_f_min_achr_dr[18] = (unsigned int)frequency_int;
      measurements_f_min_achr_dr[19] = (unsigned int)resistance[R_AB];
      measurements_f_min_achr_dr[20] = (unsigned int)resistance[X_AB];
      measurements_f_min_achr_dr[21] = (unsigned int)resistance[R_BC];
      measurements_f_min_achr_dr[22] = (unsigned int)resistance[X_BC];
      measurements_f_min_achr_dr[23] = (unsigned int)resistance[R_CA];
      measurements_f_min_achr_dr[24] = (unsigned int)resistance[X_CA];

      //Գ����� ��� � ������� ������� ��������� ������
      measurements_f_min_achr_dr[27] = time_tmp;
    }
  }
}
/*****************************************************/

/*****************************************************/
//���������� ���������� ������������� ������
/*
  type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE        - ���������� ���������� ������������� ������� ������
  type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04      - ���������� ���������� ������������� ������� ������ ������� 0.4��
  type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0          - ���������� ���������� ������������� ������ 3I0
  type_current == IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0          - ���������� ���������� ������������� ������ 3U0
  type_current == IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE              - ���������� ���������� �������� �������
  type_current == IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE              - ���������� ���������� ����������� �������
  type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP          - ���������� ���������� ������������� ������ ��������� �����������
  type_current == IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR       - ���������� ���������� �������� ������� ��� ���
  type_current == IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV          - �������� ������� ��� ����
*/
/*****************************************************/
inline void end_monitoring_min_max_measurement(unsigned int type_current, unsigned int* carrent_active_functions)
{
  if(
     (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE   ) ||
     (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04 ) ||
     (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0     ) ||
     (type_current == IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0     ) ||
     (type_current == IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE         ) ||
     (type_current == IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE         ) ||
     (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP     ) ||
     (type_current == IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR  ) ||
     (type_current == IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV     )
    )
  {
    int step = number_max_phase_dr   + 
               number_max_phase04_dr +
               number_max_3I0_dr     + 
               number_max_3U0_dr     + 
               number_min_U_dr       +
               number_max_U_dr       +
               number_max_ZOP_dr     +
               number_min_f_achr_dr  +
               number_f_chapv_dr;
    
    //�������� �� ���������� ������ ����������� ������������
    if(
       ( (number_max_phase_dr   > 0) || ( (number_max_phase_dr   == 0) && ((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE  )) == 0) ) ) &&
       ( (number_max_phase04_dr > 0) || ( (number_max_phase04_dr == 0) && ((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04)) == 0) ) ) &&
       ( (number_max_3I0_dr     > 0) || ( (number_max_3I0_dr     == 0) && ((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0    )) == 0) ) ) &&
       ( (number_max_3U0_dr     > 0) || ( (number_max_3U0_dr     == 0) && ((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0    )) == 0) ) ) &&
       ( (number_min_U_dr       > 0) || ( (number_min_U_dr       == 0) && ((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE        )) == 0) ) ) &&
       ( (number_max_U_dr       > 0) || ( (number_max_U_dr       == 0) && ((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE        )) == 0) ) ) &&
       ( (number_max_ZOP_dr     > 0) || ( (number_max_ZOP_dr     == 0) && ((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP    )) == 0) ) ) &&
       ( (number_min_f_achr_dr  > 0) || ( (number_min_f_achr_dr  == 0) && ((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR )) == 0) ) ) &&
       ( (number_f_chapv_dr     > 0) || ( (number_f_chapv_dr     == 0) && ((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV    )) == 0) ) ) &&
       ( (step > 0) && (step <= MAX_NUMBER_FIX_MAX_MEASUREMENTS)) 
      )
    {
      unsigned char *input_data_point = NULL, *output_data_point;
      
      //��������� ������� �����  ������ �������� ���
      if(type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE)
      {
        measurements_phase_max_dr[25] = (unsigned int)VMP_last_KZ;
        measurements_phase_max_dr[26] = equal_more_KZ;
        input_data_point = (unsigned char *)(measurements_phase_max_dr);
      }
      else if(type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04)
      {
        measurements_phase04_max_dr[25] = (unsigned int)UNDEF_VMP;
        measurements_phase04_max_dr[26] = 0;
        input_data_point = (unsigned char *)(measurements_phase04_max_dr);
      }
      else if(type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0)
      {
        measurements_3I0_max_dr[25] = (unsigned int)UNDEF_VMP;
        measurements_3I0_max_dr[26] = 0;
        input_data_point = (unsigned char *)(measurements_3I0_max_dr);
      }
      else if(type_current == IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0)
      {
        measurements_3U0_max_dr[25] = (unsigned int)UNDEF_VMP;
        measurements_3U0_max_dr[26] = 0;
        input_data_point = (unsigned char *)(measurements_3U0_max_dr);
      }
      else if(type_current == IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)
      {
        measurements_U_min_dr[25] = (unsigned int)UNDEF_VMP;
        measurements_U_min_dr[26] = 0;
        input_data_point = (unsigned char *)(measurements_U_min_dr);
      }
      else if(type_current == IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE)
      {
        measurements_U_max_dr[25] = (unsigned int)UNDEF_VMP;
        measurements_U_max_dr[26] = 0;
        input_data_point = (unsigned char *)(measurements_U_max_dr);
      }
      else if(type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP)
      {
        measurements_ZOP_max_dr[25] = (unsigned int)UNDEF_VMP;
        measurements_ZOP_max_dr[26] = 0;
        input_data_point = (unsigned char *)(measurements_ZOP_max_dr);
      }
      else if(type_current == IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR)
      {
        measurements_f_min_achr_dr[25] = (unsigned int)UNDEF_VMP;
        measurements_f_min_achr_dr[26] = 0;
        input_data_point = (unsigned char *)(measurements_f_min_achr_dr);
      }
      else if(type_current == IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV)
      {
        measurements_f_chapv_dr[25] = (unsigned int)UNDEF_VMP;
        measurements_f_chapv_dr[26] = 0;
        input_data_point = (unsigned char *)(measurements_f_chapv_dr);
      }
      
      //��������� ������ ���� ��� ����� ��������
      step -= 1; //���� ��  ��������� ���������� � 0, � �� � 1 (step ����������� �� ����� 1(�� ��������� ����), ���� ��'������ ����� �� ���� ����)
      if(((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE  )) != 0) && (type_current != IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE  ))
        step -= 1;
      if(((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04)) != 0) && (type_current != IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04))
        step -= 1;
      if(((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0    )) != 0) && (type_current != IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0    ))
        step -= 1;
      if(((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0    )) != 0) && (type_current != IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0    ))
        step -= 1;
      if(((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE        )) != 0) && (type_current != IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE        ))
        step -= 1;
      if(((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE        )) != 0) && (type_current != IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE        ))
        step -= 1;
      if(((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP    )) != 0) && (type_current != IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP    ))
        step -= 1;
      if(((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR )) != 0) && (type_current != IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR ))
        step -= 1;
      if(((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV    )) != 0) && (type_current != IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV    ))
        step -= 1;
      
      if(step >= 0)
      {
        output_data_point = (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR +step*sizeof(unsigned int)*SIZE_ARRAY_FIX_MAX_MEASUREMENTS);
        
        for(unsigned int i = 0; i < (sizeof(unsigned int)*SIZE_ARRAY_FIX_MAX_MEASUREMENTS); i++)
        {
          if(i != (sizeof(unsigned int)*SIZE_ARRAY_FIX_MAX_MEASUREMENTS - 1))
          {
            *(output_data_point + i) = *(input_data_point + i);
          }
          else
          {
            //� ������� ���� ������, �� ������� ����� � Little-endian ������� �������� ����� ���������� �������� ������, ���� ���� ������, �� ����� ���������� ������������ �����
            *(output_data_point + i) = type_current;
          }
        }

        //������ ������, �� �� �� ���䳿 ���������� 
        state_current_monitoring &= ~(1<<type_current);
      }
      else
      {
        //³������� ���������� �������� - ���� �������� ���������� ����� �� ���� � ��������
        fix_undefined_error_dr(carrent_active_functions);
      }
    }
    else
    {
      //³������� ���������� �������� - ���� �������� ���������� ����� �� ���� � ��������
      fix_undefined_error_dr(carrent_active_functions);
    }
  }
  else
  {
    //³������� ���������� �������� - ���� �������� ���������� ����� �� ���� � ��������
    fix_undefined_error_dr(carrent_active_functions);
  }
}
/*****************************************************/

/*****************************************************/
//������ ������� ������ ����� � DataFalsh
/*****************************************************/
inline void command_start_saving_record_dr_into_dataflash(void)
{
  //� �������� �� ���������� ����� ���������� ����������� �����������, �� ������� ����� � �� �� ���������
  _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);
  info_rejestrator_dr.saving_execution = 1;
        
  //����������� ����� ������� ������
  part_writing_dr_into_dataflash = 0;
  //����������� ������� ������ � ��������� DataFlash
  control_tasks_dataflash |= TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR;
}
/*****************************************************/

/*****************************************************/
//������� ������� ����� ����������� ������ ����������� ����������
/*****************************************************/
inline void routine_for_queue_dr(void)
{
  if(
     (state_dr_record == STATE_DR_MAKE_RECORD)            || /*����� ����� ��������� ��� �������� �� ����� � DataFlsh ��� ���������� �������  ������ ������, �� ����� ���������� �����    ���������, � �� ������ ����� ����������� ���������� ������ ������������ ������*/
     (state_dr_record == STATE_DR_CUT_RECORD)             || /*����� ����� ��������� ��� �������� �� ����� � DataFlsh   � ���������  �������� ������ ������, �� ����� ���������� ����� �� ���������, �    ������ ����� ����������� ���������� ������ ������������ ������*/
     (number_records_dr_waiting_for_saving_operation != 0)  
    )
  {
    if ((control_tasks_dataflash & TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR) == 0)
    {
      //�� ����� ������ ������ � Datafalsh �� �����������
      unsigned char *buffer_source, *buffer_target;
        
      if (number_records_dr_waiting_for_saving_operation == 0)
      {
        buffer_target = buffer_for_save_dr_record_level_2;
        buffer_source = buffer_for_save_dr_record;
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++)
        {
          *(buffer_target + i) = *(buffer_source + i);
          *(buffer_source + i) = 0xff;
        }
        number_records_dr_waiting_for_saving_operation = 0;
      }
      else if (number_records_dr_waiting_for_saving_operation == 1)
      {
        buffer_target = buffer_for_save_dr_record_level_2;
        buffer_source = buffer_for_save_dr_record_level_1;
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) *(buffer_target +i ) = *(buffer_source + i);
        
        if((state_dr_record == STATE_DR_MAKE_RECORD) || (state_dr_record == STATE_DR_CUT_RECORD))
        {
          buffer_target = buffer_for_save_dr_record_level_1;
          buffer_source = buffer_for_save_dr_record;
          for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) 
          {
            *(buffer_target + i) = *(buffer_source + i);
            *(buffer_source + i) = 0xff;
          }
          number_records_dr_waiting_for_saving_operation = 1;
        }
        else number_records_dr_waiting_for_saving_operation = 0;
      }
      else
      {
        buffer_target = buffer_for_save_dr_record_level_2;
        buffer_source = buffer_for_save_dr_record_level_1;
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) *(buffer_target + i) = *(buffer_source + i);
        buffer_target = buffer_for_save_dr_record_level_1;
        buffer_source = buffer_for_save_dr_record;
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) 
        {
          *(buffer_target + i) = *(buffer_source + i);
          *(buffer_source + i) = 0xff;
        }
        number_records_dr_waiting_for_saving_operation = 1;
      }
        
      //������ ������� ������ ����� � DataFalsh
      command_start_saving_record_dr_into_dataflash();
    }
    else
    {
      //�� ����� ������ ������ � Datafalsh �����������
      //����� ��� �������� ����� ��������� � �����
      if (number_records_dr_waiting_for_saving_operation == 0)
      {
        //�� ����� ������ ��� ����, ������ - � ����� ��� ������������ ����� � DataFalsh, ����� ����� ������ � ���� ����
        //��������� ������� ����� � ����� buffer_for_save_dr_record_level_1
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) 
        {
          buffer_for_save_dr_record_level_1[i] = buffer_for_save_dr_record[i];
          buffer_for_save_dr_record[i] = 0xff;
        }
    
        //�������, �� ���� ����� ����� ������� ���� �� ������������ ����� � dataFalsh
        number_records_dr_waiting_for_saving_operation = 1;
      }
      else if (number_records_dr_waiting_for_saving_operation >= 1)
      {
         if((state_dr_record == STATE_DR_MAKE_RECORD) || (state_dr_record == STATE_DR_CUT_RECORD))
         {
          //�� ����� ������ ��� ����, ������ - � ����� ��� ������������ ����� � DataFalsh, �� ���� ����������� ����� ����� � ����� buffer_for_save_dr_record_level_1
          /*
          ���� ����� ����� �������� � ����� ������ ����� buffer_for_save_dr_record, ��� �������� ������� ������. �� ������ � ����
          � ���� ���� ������ ������ ����� �����, �� ������ ��������� ������� ������ �����
          */
          number_records_dr_waiting_for_saving_operation = 2;
         }
      }
    }
    
    if (state_dr_record == STATE_DR_MAKE_RECORD)
    {
      //�������, �� �� ����� �������� ��������� �����
      state_dr_record = STATE_DR_NO_RECORD;
    }
    else if (state_dr_record == STATE_DR_CUT_RECORD)
    {
      //�������, �� ����� ��������� ��������� ����� �����
      state_dr_record = STATE_DR_FORCE_START_NEW_RECORD;
    }
      
  }
}
/*****************************************************/

/*****************************************************/
//������� ������� ����� ����������� ����������
/*****************************************************/
inline void digital_registrator(unsigned int* carrent_active_functions)
{
  /***
  ���� ������������ �������� �� ����� �������� �� ���������� ��������� ���� ���������
  ***/
  enum _statePowerDown
  {
    STATE_POWER_DOWN_NONE = 0,
    STATE_POWER_DOWN_ETAP_BEFORE,
    STATE_POWER_DOWN_ETAP_CUT,
    STATE_POWER_DOWN_ETAP_CUT_CONFIRMED,
    STATE_POWER_DOWN_ETAP_AFTER
  };
  
  static enum _statePowerDown statePowerDown;
  if (((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t)Bit_RESET)) statePowerDown = STATE_POWER_DOWN_NONE;
  else
  {
    if (timePowerDown < POWER_DOWN_TIME) statePowerDown = STATE_POWER_DOWN_ETAP_BEFORE;
    else
    {
      if (statePowerDown == STATE_POWER_DOWN_ETAP_BEFORE) statePowerDown = STATE_POWER_DOWN_ETAP_CUT;
      else if (statePowerDown == STATE_POWER_DOWN_ETAP_CUT_CONFIRMED) statePowerDown = STATE_POWER_DOWN_ETAP_AFTER;
    }
  }
  /***/
 

  static unsigned int previous_active_functions[N_BIG];
  
  static unsigned int number_items_dr;
  static unsigned int number_changes_into_dr_record;
  static unsigned int time_from_start_record_dr;
  static unsigned int blocking_continue_monitoring_min_U;
  
  static unsigned int const monitoring_phase_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_PHASE_SIGNALES_0,
    MASKA_MONITOTYNG_PHASE_SIGNALES_1,
    MASKA_MONITOTYNG_PHASE_SIGNALES_2,
    MASKA_MONITOTYNG_PHASE_SIGNALES_3,
    MASKA_MONITOTYNG_PHASE_SIGNALES_4,
    MASKA_MONITOTYNG_PHASE_SIGNALES_5,
    MASKA_MONITOTYNG_PHASE_SIGNALES_6,
    MASKA_MONITOTYNG_PHASE_SIGNALES_7

#ifdef MASKA_MONITOTYNG_PHASE_SIGNALES_8
                                     ,
    MASKA_MONITOTYNG_PHASE_SIGNALES_8
#endif
  };
  static unsigned int const monitoring_phase04_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_PHASE04_SIGNALES_0,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_1,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_2,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_3,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_4,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_5,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_6,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_7
      
#ifdef MASKA_MONITOTYNG_PHASE04_SIGNALES_8
                                        ,
    MASKA_MONITOTYNG_PHASE04_SIGNALES_8
#endif
  };
  static unsigned int const monitoring_3I0_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_3I0_SIGNALES_0,
    MASKA_MONITOTYNG_3I0_SIGNALES_1,
    MASKA_MONITOTYNG_3I0_SIGNALES_2,
    MASKA_MONITOTYNG_3I0_SIGNALES_3,
    MASKA_MONITOTYNG_3I0_SIGNALES_4,
    MASKA_MONITOTYNG_3I0_SIGNALES_5,
    MASKA_MONITOTYNG_3I0_SIGNALES_6,
    MASKA_MONITOTYNG_3I0_SIGNALES_7
      
#ifdef MASKA_MONITOTYNG_3I0_SIGNALES_8
                                   ,
    MASKA_MONITOTYNG_3I0_SIGNALES_8
#endif
  };
  static unsigned int const monitoring_3U0_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_3U0_SIGNALES_0,
    MASKA_MONITOTYNG_3U0_SIGNALES_1,
    MASKA_MONITOTYNG_3U0_SIGNALES_2,
    MASKA_MONITOTYNG_3U0_SIGNALES_3,
    MASKA_MONITOTYNG_3U0_SIGNALES_4,
    MASKA_MONITOTYNG_3U0_SIGNALES_5,
    MASKA_MONITOTYNG_3U0_SIGNALES_6,
    MASKA_MONITOTYNG_3U0_SIGNALES_7
      
#ifdef MASKA_MONITOTYNG_3U0_SIGNALES_8
                                   ,
    MASKA_MONITOTYNG_3U0_SIGNALES_8
#endif
  };
  static unsigned int const monitoring_Umin_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_UMIN_SIGNALES_0,
    MASKA_MONITOTYNG_UMIN_SIGNALES_1,
    MASKA_MONITOTYNG_UMIN_SIGNALES_2,
    MASKA_MONITOTYNG_UMIN_SIGNALES_3,
    MASKA_MONITOTYNG_UMIN_SIGNALES_4,
    MASKA_MONITOTYNG_UMIN_SIGNALES_5,
    MASKA_MONITOTYNG_UMIN_SIGNALES_6,
    MASKA_MONITOTYNG_UMIN_SIGNALES_7
      
#ifdef MASKA_MONITOTYNG_UMIN_SIGNALES_8
                                    ,
    MASKA_MONITOTYNG_UMIN_SIGNALES_8
#endif
  };
  static unsigned int const monitoring_Umax_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_UMAX_SIGNALES_0,
    MASKA_MONITOTYNG_UMAX_SIGNALES_1,
    MASKA_MONITOTYNG_UMAX_SIGNALES_2,
    MASKA_MONITOTYNG_UMAX_SIGNALES_3,
    MASKA_MONITOTYNG_UMAX_SIGNALES_4,
    MASKA_MONITOTYNG_UMAX_SIGNALES_5,
    MASKA_MONITOTYNG_UMAX_SIGNALES_6,
    MASKA_MONITOTYNG_UMAX_SIGNALES_7
      
#ifdef MASKA_MONITOTYNG_UMAX_SIGNALES_8
                                    ,
    MASKA_MONITOTYNG_UMAX_SIGNALES_8
#endif
  };
  static unsigned int const monitoring_zzp_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_ZOP_SIGNALES_0,
    MASKA_MONITOTYNG_ZOP_SIGNALES_1,
    MASKA_MONITOTYNG_ZOP_SIGNALES_2,
    MASKA_MONITOTYNG_ZOP_SIGNALES_3,
    MASKA_MONITOTYNG_ZOP_SIGNALES_4,
    MASKA_MONITOTYNG_ZOP_SIGNALES_5,
    MASKA_MONITOTYNG_ZOP_SIGNALES_6,
    MASKA_MONITOTYNG_ZOP_SIGNALES_7
      
#ifdef MASKA_MONITOTYNG_ZOP_SIGNALES_8
                                   ,
    MASKA_MONITOTYNG_ZOP_SIGNALES_8
#endif
  };
  static unsigned int const monitoring_f_min_achr_signals[N_BIG] =
  {
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_0,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_1,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_2,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_3,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_4,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_5,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_6,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_7
      
#ifdef MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_8
                                          ,
    MASKA_MONITOTYNG_F_MIN_ACHR_SIGNALES_8
#endif
  };
  
  //�� �������� �������� ����� � ���� �������, ���� ��� ������ ���������� ������ ������
  if(state_dr_record == STATE_DR_EXECUTING_RECORD)
  {
    //���������� �� �� ������� �����, �� ������� ���� ������ �������� ������������ ������
    unsigned int temp_value_for_max_min_fix_measurement = (
                                                            number_max_phase_dr   + 
                                                            number_max_phase04_dr + 
                                                            number_max_3I0_dr     +
                                                            number_max_3U0_dr     +
                                                            number_min_U_dr       +
                                                            number_max_U_dr       +
                                                            number_max_ZOP_dr     +
                                                            number_min_f_achr_dr  +
                                                            number_f_chapv_dr    
                                                      );
    if(temp_value_for_max_min_fix_measurement > MAX_NUMBER_FIX_MAX_MEASUREMENTS)
    {
      //����, ���������� �������� ���� �� ���� � ��������, ��� ���� ������, ������� ��������� ������ ����������� ����������
      fix_undefined_error_dr(carrent_active_functions);

      /*
      ������� ������ ������ ����������� ���������� � ����� ��������� �������� �������
      �� �� ������ ��� ����, ��� � ������� ����� ������ ������ ���� ����������� ��������� ������ ����������� ����������
      */
      _CLEAR_BIT(previous_active_functions, RANG_WORK_D_REJESTRATOR);
    }
    else
    {
      //���������� �� ����� ����� ������ ��������� ������������ ������ �����
      unsigned int comp = false;
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_phase_signals, N_BIG)
      if(comp)
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE)) == 0)
        {
          //� ����� ������ ����� ��������� ������������� ������� ������
          temp_value_for_max_min_fix_measurement++;
        }
      }

      //���������� �� ����� ����� ������ ��������� ������������ ������ ����� ������� 0.4��
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_phase04_signals, N_BIG)
      if(comp)
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04)) == 0)
        {
          //� ����� ������ ����� ��������� ������������� ������� ������ ������� 0.4��
          temp_value_for_max_min_fix_measurement++;
        }
      }

      //���������� �� ����� ����� ������ ��������� ������������ ����� 3I0
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3I0_signals, N_BIG)
      if(comp)
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0)) == 0)
        {
          //� ����� ������ ����� ��������� ������������� ������ 3I0
          temp_value_for_max_min_fix_measurement++;
        }
      }
      
      //���������� �� ����� ����� ������ ��������� ����������� ������� 3U0
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3U0_signals, N_BIG)
      if(comp)
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0)) == 0)
        {
          //� ����� ������ ����� ��������� ����������� ������� 3U0
          temp_value_for_max_min_fix_measurement++;
        }
      }
      
      //���������� �� ����� ����� ������ ��������� �������� �������
      if (
          (blocking_continue_monitoring_min_U == 0) ||
          (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_BV) != 0)  
         )
      {
        COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_Umin_signals, N_BIG)
        if(comp)
        {
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)) == 0)
          {
            //� ����� ������ ����� ��������� �������� ����� �������
            temp_value_for_max_min_fix_measurement++;
          }
        }
      }
      
      //���������� �� ����� ����� ������ ��������� ����������� �������
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_Umax_signals, N_BIG)
      if(comp)
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE)) == 0)
        {
          //� ����� ������ ����� ��������� ����������� ����� �������
          temp_value_for_max_min_fix_measurement++;
        }
      }
      
      //���������� �� ����� ����� ������ ��������� ������������ ����� ��������� �����������
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_zzp_signals, N_BIG)
      if(comp)
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP)) == 0)
        {
          //� ����� ������ ����� ��������� ������������� ������ ���������� �����������
          temp_value_for_max_min_fix_measurement++;
        }
      }
    
      //���������� �� ����� ����� ������ ��������� �������� ������� ��� ���
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_f_min_achr_signals, N_BIG)
      if(comp)
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR)) == 0)
        {
          //� ����� ������ ����� ��������� �������� �������
          temp_value_for_max_min_fix_measurement++;
        }
      }
      
      
      if (
          /*� ����� ������� ������ ������� ����*/
          (_CHECK_SET_BIT(carrent_active_functions , RANG_VN_BLOCK_CHAPV) == 0)
          &&
          (
           (
            /*����� ����������� ������� ��� ���������� ����*/
            (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV_VID_DV) != 0) &&
            (_CHECK_SET_BIT(carrent_active_functions , RANG_ACHR_CHAPV_VID_DV) == 0)
           )
           ||
           (
            /*����� ����������� ������� �� ����������� ����*/
            (_CHECK_SET_BIT(carrent_active_functions , RANG_RAZR_CHAPV) != 0) 
            &&
            (
             (  
              (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV1  ) != 0) &&
              (_CHECK_SET_BIT(carrent_active_functions , RANG_ACHR_CHAPV1  ) == 0)
             )   
             ||  
             (  
              (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV2  ) != 0) &&
              (_CHECK_SET_BIT(carrent_active_functions , RANG_ACHR_CHAPV2  ) == 0)
             )   
            )           
           ) 
          )
         )   
      {
        //� ����� ����������� ������� �� ����
        temp_value_for_max_min_fix_measurement++;
      }
          
      
      if(temp_value_for_max_min_fix_measurement > MAX_NUMBER_FIX_MAX_MEASUREMENTS)
      {
        //������� ��������, �� ������� ���� ������ �������� ������������ ���������
        //����� ��������� ��������� ������� ����� � ������ ��������� �����

        buffer_for_save_dr_record[FIRST_INDEX_NUMBER_ITEMS_DR      ] = number_items_dr;
        buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR    ] = number_changes_into_dr_record        & 0xff;
        buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR + 1] = (number_changes_into_dr_record >> 8) & 0xff;

        //���������� �� ����� ��������� ��������� ������������� ������� ������
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE, carrent_active_functions);

        //���������� �� ����� ��������� ��������� ������������� ������� ������ ������� 0.4��
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04, carrent_active_functions);

        //���������� �� ����� ��������� ��������� ������������� ������ 3I0
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0, carrent_active_functions);

        //���������� �� ����� ��������� ��������� ����������� ������� 3U0
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0, carrent_active_functions);

        //���������� �� ����� ��������� ��������� �������� �������
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);

        //���������� �� ����� ��������� ��������� ����������� �������
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE, carrent_active_functions);

        //���������� �� ����� ��������� ��������� ������������� ������ ������� �����������
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP, carrent_active_functions);
        
        //���������� �� ����� ��������� ��������� �������� ������� ��� ���
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR, carrent_active_functions);
        
        /*
        �� �� ��������� �� ����� ��������� ��������� �������� ������� ��� ����,
        �� ���� ��������� �� �������, �� ����� ��������� ������������ (�� ��
        ������ �������� �� ����������� ������� - � ������� ������� ��� ������� ���
        �� ����)
        */

        //������ 䳿 �������� ����� � ���� �������, ���� ������� end_monitoring_min_max_measurement �� ����������� ������� � �� ������� state_dr_record � STATE_DR_NO_RECORD
        if(state_dr_record != STATE_DR_NO_RECORD)
        {
          //�������� ������� ������������ ������������ ������ ��� ����
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_PHASE_DR  ] = number_max_phase_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_PHASE04_DR] = number_max_phase04_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_3I0_DR    ] = number_max_3I0_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_3U0_DR    ] = number_max_3U0_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MIN_U_DR      ] = number_min_U_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_U_DR      ] = number_max_U_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_ZOP_DR    ] = number_max_ZOP_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MIN_F_ACHR_DR ] = number_min_f_achr_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_F_CHAPV_DR    ] = number_f_chapv_dr;

          //�������, �� ����� ��� ������ �� ����� ������ ����� �����, �� ��������� ����� ��� ��������� ���������
          state_dr_record = STATE_DR_CUT_RECORD;
        
          //������� ������ ������ ����������� ����������
          _CLEAR_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);
        
          //����������� ����� ������ � ����� ��� ������
          routine_for_queue_dr();
        }
        /*
        ������� ������ ������ ����������� ���������� � ����� ��������� �������� �������
        �� �� ������ ��� ����, ��� � ������� ����� ������ ������ ���� ����������� ��������� ������ ����������� ����������
        */
        _CLEAR_BIT(previous_active_functions, RANG_WORK_D_REJESTRATOR);
        
      }
    }
  }
  
  switch (state_dr_record)
  {
  case STATE_DR_NO_RECORD:
  case STATE_DR_FORCE_START_NEW_RECORD:
    {
      //���������� ������� ����������� �������  ������ ����������� ����������
      _SET_BIT(clear_diagnostyka, ERROR_DR_UNDEFINED_BIT);
      if(number_records_dr_waiting_for_saving_operation < (WIGHT_OF_DR_WAITING - 1))
      {
        //�� �������� ������, �� �� ����� �� ���� ����� ����� � ��� � ����� ���, ���� ������� ����������� ��� ������ �����
        _SET_BIT(clear_diagnostyka, ERROR_DR_TEMPORARY_BUSY_BIT);
      }
      
      //�� ������� ������ ������ �� ���������� ��������� �� ���������
      
      //��������, �� ����� ����� ������� ����������� ����������
      unsigned int cur_active_sources[N_BIG];
      for (size_t i = 0; i != N_BIG; ++i) 
      {
        cur_active_sources[i] = carrent_active_functions[i] & current_settings_prt.ranguvannja_digital_registrator[i];
      }
      unsigned int comp = false;
      NOT_ZERO_OR(comp, cur_active_sources, N_BIG)
      if (
          (comp) ||
          (state_dr_record == STATE_DR_FORCE_START_NEW_RECORD)
         )
      {
        //� ����� ������� ����������� ����������
        
        //����������, �� ��� ������� ������ ������ �� �� �������� ��������� ����������
        if(number_records_dr_waiting_for_saving_operation < WIGHT_OF_DR_WAITING)
        {
          //����� �������� ����� �����
          
          //���������� ����� ������ �� ���������� ����������� � ���� "��� ������ ������ ����������"
          state_dr_record = STATE_DR_EXECUTING_RECORD;
          //����������� ������� �������
          _SET_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);
        
          //�������� ���� ������� ������
          buffer_for_save_dr_record[FIRST_INDEX_START_START_RECORD_DR] = LABEL_START_RECORD_DR;
         
          //�������� ��� ������� ������
          drDateTimeState = (realDateTime != 0) ? AVAT_DATE_TIMR_FIX_LEVEL2 : AVAR_DATE_TIME_NONE;

          for(size_t i = 0; i < sizeof(time_t); i++)  buffer_for_save_dr_record[FIRST_INDEX_DATA_TIME_DR + i] = *((unsigned char*)(&time_dat) + i);
          for(size_t i = 0; i < sizeof(int32_t); i++)  buffer_for_save_dr_record[FIRST_INDEX_DATA_TIME_DR + sizeof(time_t) + i] = *((unsigned char*)(&time_ms) + i);
          
          //�������� ������������ ��� ���� ���� �������� ���������� ���������
          unsigned int control_extra_settings_1_tmp = current_settings_prt.control_extra_settings_1 & (MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE) | MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04));
          unsigned char *point_to_extra_settings = (unsigned char *)(&control_extra_settings_1_tmp);
          for (unsigned int i = 0; i < sizeof(control_extra_settings_1_tmp); i++)
            buffer_for_save_dr_record[FIRST_INDEX_EXTRA_SETTINGS_DR + i] = *(point_to_extra_settings + i);

           //�'�� ������
          for(unsigned int i=0; i< MAX_CHAR_IN_NAME_OF_CELL; i++) 
            buffer_for_save_dr_record[FIRST_INDEX_NAME_OF_CELL_DR + i] = current_settings_prt.name_of_cell[i] & 0xff;

           //������� �������
          for(unsigned int i = 0; i < NUMBER_BYTES_SAMPLE_DR; i++) 
            buffer_for_save_dr_record[FIRST_INDEX_SOURCE_DR + i] = *(((unsigned char*)cur_active_sources) + i);

          //�������� ��������� c��� ������� ����� �����
          //̳��� ���� ������������ ����� ������� �� ������� ������� ������
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  0] = 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  1] = 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  2] = 0xff;

          //������� ������ ���� ������� � ������� ������� ������
          time_from_start_record_dr = 0;
          
          //������� ������� ����� ����
          number_items_dr = 1;
      
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR +  0] =  time_from_start_record_dr        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR +  1] = (time_from_start_record_dr >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR +  2] = (time_from_start_record_dr >> 16) & 0xff;
          
          for (size_t i = 0; i < NUMBER_BYTES_SAMPLE_DR; ++i)
          {
            size_t offset = i >> 2;
            size_t shift = 8*(i & ((1u << 2) - 1));
            
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR                        + 3 + i] = (previous_active_functions[offset] >> shift) & 0xff;

           buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR + 3 + i] = (carrent_active_functions[offset] >> shift) & 0xff;;
          }
          //����� ��������� � ��� ������� ������� ���
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 3 + NUMBER_BYTES_SAMPLE_DR + 0] = 0;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 3 + NUMBER_BYTES_SAMPLE_DR + 1] = 0;

          //���������� ������� ��� �������
          number_changes_into_dr_record = 0;
          unsigned int number_changes_into_current_item;
          _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY(previous_active_functions, carrent_active_functions, N_BIG, number_changes_into_current_item);
          number_changes_into_dr_record += number_changes_into_current_item;
      
          //ʳ������ ��� ������� � �������� �� ��������� ������
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR + 3 + NUMBER_BYTES_SAMPLE_DR + 0] = number_changes_into_current_item & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR + 3 + NUMBER_BYTES_SAMPLE_DR + 1] = (number_changes_into_current_item >> 8) & 0xff;

          //������� ������� �������� ������������ ������/������
          number_max_phase_dr = 0;
          number_max_phase04_dr = 0;
          number_max_3I0_dr = 0;
          number_max_3U0_dr = 0;
          number_min_U_dr = 0;
          number_max_U_dr = 0;
          number_max_ZOP_dr = 0;
          number_min_f_achr_dr = 0;
          number_f_chapv_dr = 0;
          
          //������ ����������� ��� ��������� ������������ ������
          state_current_monitoring = 0;
          
          //���������� �� ����� ����� ��������� ������������ ������ �����
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_phase_signals, N_BIG)
          if(comp)
          {
            start_monitoring_max_phase_current(time_from_start_record_dr);
          }

          //���������� �� ����� ����� ��������� ������������ ������ ����� ������� 0.4��
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_phase04_signals, N_BIG)
          if(comp)
          {
            start_monitoring_max_phase04_current(time_from_start_record_dr);
          }

          //���������� �� ����� ����� ��������� ������������ ����� 3I0
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3I0_signals, N_BIG)
          if(comp)
          {
            start_monitoring_max_3I0(time_from_start_record_dr);
          }

          //���������� �� ����� ����� ��������� ����������� ������� 3U0
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3U0_signals, N_BIG)
          if(comp)
          {
            start_monitoring_max_3U0(time_from_start_record_dr);
          }

          //������ ���������� ���������� �������� �������
          blocking_continue_monitoring_min_U = 0;
          //���������� �� ����� ����� ��������� �������� �������
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_Umin_signals, N_BIG)
          if(comp)
          {
            start_monitoring_min_U(time_from_start_record_dr);
          }
          //����������, �� ����� ����� ��������� ��������� �������� ������� ���� ��������� ��������
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)) != 0)
          {
            if (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_BO) != 0)
            {
              blocking_continue_monitoring_min_U = 0xff;
              end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);
            }
          }

          //���������� �� ����� ����� ��������� ����������� �������
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_Umax_signals, N_BIG)
          if(comp)
          {
            start_monitoring_max_U(time_from_start_record_dr);
          }

          //���������� �� ����� ����� ��������� ������������ ����� ��������� �����������
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_zzp_signals, N_BIG)
          if(comp)
          {
            start_monitoring_max_ZOP(time_from_start_record_dr);
          }
      
          //���������� �� ����� ����� ��������� �������� �������
          COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_f_min_achr_signals, N_BIG)
          if(comp)
          {
            start_monitoring_min_f(time_from_start_record_dr);
          }

          /*
          �������� ����������� ������ �������� ��������� ��� ����� ���� ����������
          �� �������, ���� �� �� ����� ��������� ���������� ���������, � ����� ������
          ��������� ��� ������� ���� ���������� � ���� ���� � ���� ������ ����
          ������� ������� "��������"->"��������"
          */
        }
        else
        {
          //����������� �������, �� � ����� �� ������ ����������� ���������� ���, �� �� ���� ������ ������
          _SET_BIT(set_diagnostyka, ERROR_DR_TEMPORARY_BUSY_BIT);
          _SET_BIT(carrent_active_functions, RANG_DEFECT);
        }
      }
      else state_dr_record = STATE_DR_NO_RECORD;
      
      break;
    }
  case STATE_DR_EXECUTING_RECORD:
    {
      if (
          (drDateTimeState == AVAR_DATE_TIME_NONE) &&
          (realDateTime != 0)  
         )
      {
        drDateTimeState = AVAT_DATE_TIMR_FIX_LEVEL2;
        for(size_t i = 0; i < sizeof(time_t); i++)  buffer_for_save_dr_record[FIRST_INDEX_DATA_TIME_DR + i] = *((unsigned char*)(&time_dat) + i);
        for(size_t i = 0; i < sizeof(int32_t); i++)  buffer_for_save_dr_record[FIRST_INDEX_DATA_TIME_DR + sizeof(time_t) + i] = *((unsigned char*)(&time_ms) + i);
      }

      //�������� ��� � ������� ������� ������
      time_from_start_record_dr++;
      //������� �� ����� ���� ��� ������ ������

      //��������-�������� ������������ ���������� �������
      
      //���������� �� ����� ����� ��������� ������������ ������ �����
      unsigned int comp = false;
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_phase_signals, N_BIG)
      if(comp)
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE)) != 0)
          continue_monitoring_max_phase_current(time_from_start_record_dr);
        else
          start_monitoring_max_phase_current(time_from_start_record_dr);
      }
      else
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE, carrent_active_functions);
      }

      //���������� �� ����� ����� ��������� ������������ ������ ����� ������� 0.4��
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_phase04_signals, N_BIG)
      if(comp)
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04)) != 0)
          continue_monitoring_max_phase04_current(time_from_start_record_dr);
        else
          start_monitoring_max_phase04_current(time_from_start_record_dr);
      }
      else
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04, carrent_active_functions);
      }

      //���������� �� ����� ����� ��������� ������������ ����� 3I0
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3I0_signals, N_BIG)
      if(comp)
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0)) != 0)
          continue_monitoring_max_3I0(time_from_start_record_dr);
        else
          start_monitoring_max_3I0(time_from_start_record_dr);
      }
      else
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0, carrent_active_functions);
      }
      
      //���������� �� ����� ����� ��������� ����������� ������� 3U0
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_3U0_signals, N_BIG)
      if(comp)
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0)) != 0)
          continue_monitoring_max_3U0(time_from_start_record_dr);
        else
          start_monitoring_max_3U0(time_from_start_record_dr);
      }
      else
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0, carrent_active_functions);
      }
      
      //���������� �� ����� ����� �������� ��������� �������� �������
      if (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_BV) != 0)
      {
        //������ ���������� ���������� �������� �������
        blocking_continue_monitoring_min_U = 0;
      }
      //���������� �� ����� ����� ��������� �������� �������
      if (blocking_continue_monitoring_min_U == 0)
      {
        COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_Umin_signals, N_BIG)
        if(comp)
        {
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)) != 0)
            continue_monitoring_min_U(time_from_start_record_dr);
          else
            start_monitoring_min_U(time_from_start_record_dr);
        }
        else
        {
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)) != 0)
            end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);
        }
      }
      //����������, �� ����� ����� ��������� ��������� �������� ������� ���� ��������� ��������
      if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)) != 0)
      {
        if (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_BO) != 0)
        {
          blocking_continue_monitoring_min_U = 0xff;
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);
        }
      }
      
      //���������� �� ����� ����� ��������� ����������� �������
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_Umax_signals, N_BIG)
      if(comp)
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE)) != 0)
          continue_monitoring_max_U(time_from_start_record_dr);
        else
          start_monitoring_max_U(time_from_start_record_dr);
      }
      else
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE, carrent_active_functions);
      }
      
      //���������� �� ����� ����� ��������� ������������ ����� ��������� �����������
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_zzp_signals, N_BIG)
      if(comp)
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP)) != 0)
          continue_monitoring_max_ZOP(time_from_start_record_dr);
        else
          start_monitoring_max_ZOP(time_from_start_record_dr);
      }
      else
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP, carrent_active_functions);
      }
      
      //���������� �� ����� ����� ��������� �������� ������� ��� ���
      COMPARE_NOT_ZERO_OR(comp, carrent_active_functions, monitoring_f_min_achr_signals, N_BIG)
      if(comp)
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR)) != 0)
          continue_monitoring_min_f(time_from_start_record_dr);
        else
          start_monitoring_min_f(time_from_start_record_dr);
      }
      else
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR, carrent_active_functions);
      }
      
      if (
          /*� ����� ������� ������ ������� ����*/
          (_CHECK_SET_BIT(carrent_active_functions , RANG_VN_BLOCK_CHAPV) == 0)
          &&
          (
           (
            /*����� ����������� ������� �� ���������� ����*/
            (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV_VID_DV) != 0) &&
            (_CHECK_SET_BIT(carrent_active_functions , RANG_ACHR_CHAPV_VID_DV) == 0)
           )
           ||
           (
            /*����� ����������� ������� �� ����*/
            (_CHECK_SET_BIT(carrent_active_functions , RANG_RAZR_CHAPV) != 0)
            &&
            (
             (
              (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV1  ) != 0) &&
              (_CHECK_SET_BIT(carrent_active_functions , RANG_ACHR_CHAPV1  ) == 0)
             )   
             ||  
             (
              (_CHECK_SET_BIT(previous_active_functions, RANG_ACHR_CHAPV2  ) != 0) &&
              (_CHECK_SET_BIT(carrent_active_functions , RANG_ACHR_CHAPV2  ) == 0)
             )   
            ) 
           )
          ) 
         )   
      {
        //�������� ������� ���������� ������� ������� �� ����
        number_f_chapv_dr++;
  
        int frequency_int = (int)frequency;
        if (frequency_int >= 0) frequency_int = (int)(frequency*1000);
  
        measurements_f_chapv_dr[ 0] = measurement[IM_3I0];
        measurements_f_chapv_dr[ 1] = measurement[IM_3I0_other_g];
        measurements_f_chapv_dr[ 2] = measurement[IM_3I0_r];
        measurements_f_chapv_dr[ 3] = measurement[IM_IA];
        measurements_f_chapv_dr[ 4] = measurement[IM_IB];
        measurements_f_chapv_dr[ 5] = measurement[IM_IC];
        measurements_f_chapv_dr[ 6] = measurement[IM_I2];
        measurements_f_chapv_dr[ 7] = measurement[IM_I1];
        measurements_f_chapv_dr[ 8] = measurement[IM_I04];
        measurements_f_chapv_dr[ 9] = measurement[IM_UA];
        measurements_f_chapv_dr[10] = measurement[IM_UB];
        measurements_f_chapv_dr[11] = measurement[IM_UC];
        measurements_f_chapv_dr[12] = measurement[IM_3U0];
        measurements_f_chapv_dr[13] = measurement[IM_U2];
        measurements_f_chapv_dr[14] = measurement[IM_U1];
        measurements_f_chapv_dr[15] = measurement[IM_UAB];
        measurements_f_chapv_dr[16] = measurement[IM_UBC];
        measurements_f_chapv_dr[17] = measurement[IM_UCA];
        measurements_f_chapv_dr[18] = (unsigned int)frequency_int;
        measurements_f_chapv_dr[19] = (unsigned int)resistance[R_AB];
        measurements_f_chapv_dr[20] = (unsigned int)resistance[X_AB];
        measurements_f_chapv_dr[21] = (unsigned int)resistance[R_BC];
        measurements_f_chapv_dr[22] = (unsigned int)resistance[X_BC];
        measurements_f_chapv_dr[23] = (unsigned int)resistance[R_CA];
        measurements_f_chapv_dr[24] = (unsigned int)resistance[X_CA];
        measurements_f_chapv_dr[25] = (unsigned int)UNDEF_VMP;
        measurements_f_chapv_dr[26] = 0;
  
        //Գ����� ��� � ������� ������� ��������� ������
        measurements_f_chapv_dr[27] = time_from_start_record_dr;

        //�������, �� �� ������� ������ �� ����
        state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV);
            
        //����� �� ���� ������� � �����, ���� ����� �������� �����
        end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV, carrent_active_functions);
      }
      
      //������ 䳿 �������� ����� � ���� �������, ���� ������� end_monitoring_min_max_measurement �� ����������� ������� � �� ������� state_dr_record � STATE_DR_NO_RECORD
      if(state_dr_record != STATE_DR_NO_RECORD)
      {
        //����������, �� �� ���� ����� ����������� ������
        //���� ���� ����� ���� - �� ������� ������ ���������� ����������� ����������, �� �� ����������� � ������� ��������
        if (stop_regisrator(carrent_active_functions, current_settings_prt.ranguvannja_digital_registrator) != 0)
        {
          //������� ������ ������ ����������� ����������
          _CLEAR_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);

          //���������� ����� ������ �� ���������� ����������� � ���� "���������� ������������ ����� � ��������� DataFlash"
          state_dr_record = STATE_DR_MAKE_RECORD;
        }
      
        //���������� �� �������� ���� ������� � �������� �� ��������� ������. ���� ���� ���� �, �� ������ ����� ��� ������� � �����
        COMPARE_OR(comp, carrent_active_functions, previous_active_functions, N_BIG)
        if (comp)
        {
          //�������� ���� ������� �� ������� � ��������� ������ �������

          //�������� �� ���� ������� ����� ����
          number_items_dr++;
      
          //���������� ������� ��� �������
          unsigned int number_changes_into_current_item;
          _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY(previous_active_functions, carrent_active_functions, N_BIG, number_changes_into_current_item);
          number_changes_into_dr_record += number_changes_into_current_item;
      
          //�������� �������� c��� �������
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR +  0] =  time_from_start_record_dr        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR +  1] = (time_from_start_record_dr >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR +  2] = (time_from_start_record_dr >> 16) & 0xff;

          for (size_t i = 0; i < NUMBER_BYTES_SAMPLE_DR; ++i)
          {
            size_t offset = i >> 2;
            size_t shift = 8*(i & ((1u << 2) - 1));
             
           buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR + 3 + i] = (carrent_active_functions[offset] >> shift) & 0xff;;
          }
          //ʳ������ ��� ������� � �������� �� ��������� ������
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR + 3 + NUMBER_BYTES_SAMPLE_DR + 0] = number_changes_into_current_item & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*SD_DR + 3 + NUMBER_BYTES_SAMPLE_DR + 1] = (number_changes_into_current_item >> 8) & 0xff;
        }
        
        //����������, �� ����� ����� ���������� ������
        if (
            (state_dr_record == STATE_DR_MAKE_RECORD)                  ||
            (time_from_start_record_dr >= MAX_TIME_OFFSET_FROM_START)  ||  
            ((number_items_dr + 1)     >= MAX_EVENTS_IN_ONE_RECORD  )  ||
            (statePowerDown == STATE_POWER_DOWN_ETAP_CUT)  
           )
        {
          //���� ����� ����������� ������, ��� � ����� ���������� ������ - ��������� ���������� ������ � ������ ������� �� �����
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_ITEMS_DR      ] = number_items_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR    ] =  number_changes_into_dr_record       & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR + 1] = (number_changes_into_dr_record >> 8) & 0xff;

          //���������� �� ����� ��������� ��������� ������������� ������� ������
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE)) != 0)
            end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE, carrent_active_functions);

          //���������� �� ����� ��������� ��������� ������������� ������� ������ ������� 0.4��
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04)) != 0)
            end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04, carrent_active_functions);

          //���������� �� ����� ��������� ��������� ������������� ������ 3I0
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0)) != 0)
            end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0, carrent_active_functions);

          //���������� �� ����� ��������� ��������� ����������� ������� 3U0
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0)) != 0)
            end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0, carrent_active_functions);

          //���������� �� ����� ��������� ��������� �������� �������
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)) != 0)
            end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);

          //���������� �� ����� ��������� ��������� ����������� �������
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE)) != 0)
            end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE, carrent_active_functions);

          //���������� �� ����� ��������� ��������� ������������� ������ �����������
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP)) != 0)
            end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP, carrent_active_functions);
        
          //���������� �� ����� ��������� ��������� �������� ������� ��� ���
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR)) != 0)
            end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR, carrent_active_functions);

          /*
          �������� ����������� ������ �������� ��������� ��� ����� ���� ����������
          �� �������, ���� �� ����� ��� �������� �� ����� ��������� �������� ������ ����������
          ��������� ������. ���� ���� �������� ������� ��� �� ����, �� �� ���� ���� � ������������ ����
          � ���� ��������, �� ������������ ����� ������������ �������
          */

          //������ 䳿 �������� ����� � ���� �������, ���� ������� end_monitoring_min_max_measurement �� ����������� ������� � �� ������� state_dr_record � STATE_DR_NO_RECORD
          if(state_dr_record != STATE_DR_NO_RECORD)
          {
            //�������� ������� ������������ ������������ ��������� ��� ����
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_PHASE_DR  ] = number_max_phase_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_PHASE04_DR] = number_max_phase04_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_3I0_DR    ] = number_max_3I0_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_3U0_DR    ] = number_max_3U0_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MIN_U_DR      ] = number_min_U_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_U_DR      ] = number_max_U_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_ZOP_DR    ] = number_max_ZOP_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MIN_F_ACHR_DR ] = number_min_f_achr_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_F_CHAPV_DR    ] = number_f_chapv_dr;

            //���������� ����� ������ �� ���������� ����������� � ���� "���������� ������������ ����� � DataFlash"
            if (state_dr_record != STATE_DR_MAKE_RECORD)
            {
              if (time_from_start_record_dr >= MAX_TIME_OFFSET_FROM_START)
              {
                //���� �������� ����������� �� ���� ������, �� ������ ������� ��������� ����� ��� ����������� ���� ����� ������ � ���������� �����
                state_dr_record = STATE_DR_MAKE_RECORD;
              }
              else
              {
                //���� �������� ����������� �� ��������� ����������� ������� ���� (��� ���� �������, ��� ������ �� ���������, ��� ���� �'������� � �����������), �� ������ ������� ��������� ����� ��� �� ���������� ������ ������ ����� �����
                state_dr_record = STATE_DR_CUT_RECORD;
              }
            }
          }
        
          //������� ������ ������ ����������� ����������
          _CLEAR_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);
        }
      }

      break;
    }
  default:
    {
      //�� ��� ���� �������� ����� �� ���� � �����
      fix_undefined_error_dr(carrent_active_functions);
      break;
    }
  }
  
  //����������, �� ����� ����� ����������� ����� �������� �� ����� � DataFlash
  routine_for_queue_dr();

  /*********************/
  //������� ���������� ���� ������� ��� ������� ���������/���������
  /*********************/
  for (unsigned int i = 0; i < N_BIG; i++) previous_active_functions[i] = carrent_active_functions[i];
  /*********************/
}
/*****************************************************/

/*****************************************************/
//������� ������� ����� ����������� ����������
/*****************************************************/
inline void analog_registrator(unsigned int* carrent_active_functions)
{
  static unsigned int prev_active_sources[N_BIG];
  unsigned int cur_active_sources[N_BIG];
  for(size_t i = 0; i < N_BIG; ++i) cur_active_sources[i] = carrent_active_functions[i] & current_settings_prt.ranguvannja_analog_registrator[i];

  int comp = true;
  
  if (forbidden_new_record_ar_mode_0 != 0)
  {
    /*
    �� �������� ������, �� ���� ��������� ������� ����������� ����������, �� ���������
    � ������ ���������� ���������, � ����� ��� ������������� ��������� ��������� ����� ����� �� 
    ������ ��������, ��
    - �� ������� ��������� ������������� 
    - �������� ����� ������ �������, ����� �� ���� �����
    */
    ZERO_AND(comp, cur_active_sources, N_BIG)
    if(comp) 
    {
      //����� ����� ������������� ��������� ������� ������ ������ ��������
      forbidden_new_record_ar_mode_0 = 0;
    }
    else if ((current_settings_prt.control_ar & MASKA_FOR_BIT(INDEX_ML_CTR_AR_AVAR_STATE)) == 0)
    {
      //��������� ����� ������� ��������� � DataFlash, ��� �� ���� ������� ��������� �� ���������������
      unsigned int diff_active_sources[N_BIG];
      for(size_t i = 0; i < N_BIG; ++i) diff_active_sources[i] = prev_active_sources[i] ^ cur_active_sources[i];

      COMPARE_NOT_ZERO_OR(comp, diff_active_sources, cur_active_sources, N_BIG)
      if(comp) 
      {
        //����� ����� ������������� ��������� ������� ������ ������ ��������
        forbidden_new_record_ar_mode_0 = 0;
      }
    }
  }


  static int prefault_number_periods_tmp;

  if (
      (global_timers[INDEX_TIMER_FULL_AR_RECORD] >= MAX_TIME_FULL_AR_RECORD) ||
      (state_ar_record_m == STATE_AR_BLOCK_M) ||
      (state_ar_record_fatfs == STATE_AR_MEMORY_FULL_FATFS) ||
      (state_ar_record_fatfs == STATE_AR_BLOCK_FATFS)
     )   
  {
    if (global_timers[INDEX_TIMER_FULL_AR_RECORD] >= MAX_TIME_FULL_AR_RECORD) _SET_BIT(set_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);

    state_ar_record_prt = STATE_AR_BLOCK_PRT;
    global_timers[INDEX_TIMER_POSTFAULT] = -1;
    global_timers[INDEX_TIMER_FULL_AR_RECORD] = -1;
  }
  
  if (
      (arDateTimeState == AVAR_DATE_TIME_NONE) &&
      (state_ar_record_fatfs != STATE_AR_NONE_FATFS) &&
      (state_ar_record_fatfs != STATE_AR_BLOCK_FATFS) &&
      (realDateTime != 0)  
     )
  {
    arDateTimeState = AVAT_DATE_TIMR_FIX_LEVEL1;

    header_ar.time_dat = time_dat;
    header_ar.time_ms = time_ms;
  }
 
  switch (state_ar_record_prt)
  {
  case STATE_AR_NONE_PRT:
    {
      if (state_ar_record_fatfs == STATE_AR_NONE_FATFS)
      {
        global_timers[INDEX_TIMER_FULL_AR_RECORD] = -1;
        _SET_BIT(clear_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);
      }

      //��������, �� ����� ����� ������� ����������� ����������
      NOT_ZERO_OR(comp, cur_active_sources, N_BIG)
      if (
          (
           (comp)
           &&  
           (forbidden_new_record_ar_mode_0 == 0) /*��� ��������� ����� ��.���������� (���� ���� ����) ��� �� ������� ��������� ���� ���� � ����� ���� ����� �������*/ 
          )
          ||
          (start_ar != 0) 
         )    
      {
        //� ����� ������� ����������� ����������

        if (state_ar_record_fatfs != STATE_AR_STOP_WRITE_FATFS)
        {
          //���������� ����� ������ �� ���������� ����������� � ���� "������ ������ ������"
          if (current_settings_prt.control_ar & MASKA_FOR_BIT(INDEX_ML_CTR_AR_AVAR_STATE))
          {
            state_ar_record_prt = STATE_AR_AVAR_PRT;
          }
          else
          {
            state_ar_record_prt = STATE_AR_POSTAVAR_PRT;
            global_timers[INDEX_TIMER_POSTFAULT] = 0; //��������� ������ ������������� �������
          }
        
          if (state_ar_record_fatfs == STATE_AR_NONE_FATFS)
          {
            //����� �� ��� FATFs ����� �� �����������, ���� ����� ���������� ���������� ��� ����� ������ ������ ������
          
//             global_timers[INDEX_TIMER_FULL_AR_RECORD] = 20*current_settings_prt.prefault_number_periods; //��������� ������ ������ ������  � ����������� ����� ������� ���������� �����
            prefault_number_periods_tmp = ((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t)Bit_RESET) ? current_settings_prt.prefault_number_periods : AR_TAIL_MIN_NUMBER_PERIOD;
            global_timers[INDEX_TIMER_FULL_AR_RECORD] =  20*prefault_number_periods_tmp; //��������� ������ ������ ������  � ����������� �� ���� ������� ���������� �����

          
            //�������� ���� ������� ������
            header_ar.label_start_record = LABEL_START_RECORD_AR;
            //�������� ��� ������� ������
            arDateTimeState = (realDateTime != 0) ? AVAT_DATE_TIMR_FIX_LEVEL2 : AVAR_DATE_TIME_NONE;
            header_ar.time_dat = time_dat;
            header_ar.time_ms = time_ms;

            //���������� ������������� T0
            header_ar.T0 = current_settings_prt.T0;
            //���������� ������������� TT
            header_ar.TCurrent = current_settings_prt.TCurrent;
            //���������� ������������� TT ������� 0.4��
            header_ar.TCurrent04 = current_settings_prt.TCurrent04;
            //���������� ������������� T�
            header_ar.TVoltage = current_settings_prt.TVoltage;
            
            //�������� ������������ ��� ���� ���� �������� ���������� ���������
            header_ar.control_extra_settings_1 = current_settings_prt.control_extra_settings_1 & (MASKA_FOR_BIT(INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE));

            //��� ����������� ������
            header_ar.prefault_number_periods = prefault_number_periods_tmp;

            //�'�� ������
            for(unsigned int i=0; i<MAX_CHAR_IN_NAME_OF_CELL; i++)
              header_ar.name_of_cell[i] = current_settings_prt.name_of_cell[i] & 0xff;
            //�������, �� ��������� � ������ ���������� ���������
            for (size_t i = 0; i < N_BIG; i++)
              header_ar.cur_active_sources[i] = (prev_active_sources[i] ^ cur_active_sources[i]) & cur_active_sources[i];
          }
        }
        else
        {
          _SET_BIT(set_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);
        }
      }

      break;
    }
  case STATE_AR_AVAR_PRT:
    {
      ZERO_AND(comp, cur_active_sources, N_BIG)
      if (
          (comp)
          || 
          ((current_settings_prt.control_ar & MASKA_FOR_BIT(INDEX_ML_CTR_AR_AVAR_STATE)) == 0) /*���� ������� ����, ���� �� ��� ������ ��.���������� ������ �� ������������*/
         )
      {
        state_ar_record_prt = STATE_AR_POSTAVAR_PRT;
        global_timers[INDEX_TIMER_POSTFAULT] = 0; //��������� ������ ������������� �������
      }
      
      break;
    }
  case STATE_AR_POSTAVAR_PRT:
    {
      NOT_ZERO_OR(comp, cur_active_sources, N_BIG)
        
      if (
          ((current_settings_prt.control_ar & MASKA_FOR_BIT(INDEX_ML_CTR_AR_AVAR_STATE)) != 0) &&
          (comp)
         )
      {
        //����������� �� ��������� �������
        state_ar_record_prt = STATE_AR_AVAR_PRT;
        global_timers[INDEX_TIMER_POSTFAULT] = -1; //��������� ������ ������������� �������
      }
      else if (global_timers[INDEX_TIMER_POSTFAULT] >= (int)(20*current_settings_prt.postfault_number_periods))
      {
        //���������� ������������ ������
        global_timers[INDEX_TIMER_POSTFAULT] = -1; //��������� ������ ������������� �������
        state_ar_record_prt = STATE_AR_NONE_PRT;
        
        if (
            /*�������� �� ��, �� ����� "�������������� ������" ��������� �� �����, �� ��� ���� �������� ������ �� � ������ � ��������� ����� �� � ������������ ������� ��� ������� �� ��������� �������*/
            (comp)
           )
        {
          forbidden_new_record_ar_mode_0 = 0xff; /*������� ����-���� ������, �� � ��������� ���� ������� �� ������������ �������*/
        }
      }
      
      break;
    }
  case STATE_AR_BLOCK_PRT:
    {
      //�������� �� ���� ����� ������ ����� ����� ���� �� �� ������ � ����������� �����
      if (
          (state_ar_record_fatfs == STATE_AR_NONE_FATFS) &&
          (state_ar_record_m == STATE_AR_NONE_M)
         )
      {
        state_ar_record_prt = STATE_AR_NONE_PRT;
       
        NOT_ZERO_OR(comp, cur_active_sources, N_BIG)
        if(comp)   
        {
          forbidden_new_record_ar_mode_0 = 0xff; /*������� ����-���� ������, �� � ��������� ���� ������� �� ������������ �������*/
        }
      }
      
      break;
    }
  default:
    {
      //���������� ����� ����� �� ���� � ����
      total_error_sw_fixed();
      break;
    }
  }
  
  //�����������/�������� ������� ������ ����������� ����������
  if (
      (state_ar_record_prt == STATE_AR_AVAR_PRT) ||
      (state_ar_record_prt == STATE_AR_POSTAVAR_PRT)
     )
  {
    _SET_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR);
    if (truncPrefault)
    {
      truncPrefault = false;
      int trunc = 20*(prefault_number_periods_tmp - AR_TAIL_MIN_NUMBER_PERIOD);
      if (
          (trunc > 0) && //�������������, �� ������ � ��� ���� ���� � ��� ��������
          (global_timers[INDEX_TIMER_FULL_AR_RECORD] >= trunc) //�������������, �� ������ � ��� ���� ���� � ��� ��������
         )   
      {
        global_timers[INDEX_TIMER_FULL_AR_RECORD] -= trunc;
      }
    }

  }
  else
  {
    _CLEAR_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR);
  }

  for (size_t i = 0; i != N_BIG; ++i) prev_active_sources[i] = cur_active_sources[i];
  
  start_ar = 0;
}
/*****************************************************/


/*****************************************************/
//������� ������� � ��� ����������� �� ���� ��������
/*****************************************************/
inline void main_protection(void)
{
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  /***
  ����������� �� � ��������� ������� ���������� ��.GOOSE ����� � ��.MMS �����
  ***/
enum WRP_GOOSE_BIT_POS { 
    GS_BLOCK_ORD_NUM_00,//0
    GS_BLOCK_ORD_NUM_01,//1
    GS_BLOCK_ORD_NUM_02,//2
    GS_BLOCK_ORD_NUM_03,//3
    GS_BLOCK_ORD_NUM_04,//4
    GS_BLOCK_ORD_NUM_05,//5
    GS_BLOCK_ORD_NUM_06,//6
    GS_BLOCK_ORD_NUM_07,//7
    GS_BLOCK_ORD_NUM_08,//8
    GS_BLOCK_ORD_NUM_09,//9
    GS_BLOCK_ORD_NUM_10,//10
    GS_BLOCK_ORD_NUM_11,//11
    GS_BLOCK_ORD_NUM_12,//12
    GS_BLOCK_ORD_NUM_13,//13
    GS_BLOCK_ORD_NUM_14,//14
    GS_BLOCK_ORD_NUM_15,//15

TOTAL_GOOSE_BLOCK
//
};
enum WRP_MMS_BIT_POS { 
    MMS_BLOCK_ORD_NUM_00,//0
    MMS_BLOCK_ORD_NUM_01,//1
    MMS_BLOCK_ORD_NUM_02,//2
    MMS_BLOCK_ORD_NUM_03,//3
TOTAL_MMS_BLOCK
//
};
enum WRP_LAN_BIT_POS { 
    LAN_BLOCK_ORD_NUM_00,//0
    LAN_BLOCK_ORD_NUM_01,//1
    LAN_BLOCK_ORD_NUM_02,//2
    LAN_BLOCK_ORD_NUM_03,//3
TOTAL_LAN_BLOCK
//
};

//.typedef union lan_block_Unn{ 
//.    unsigned char arCh[4];
//.    unsigned short arSh[2];
//.    unsigned long Lan_Vl;
//.    struct{
//.        struct {
//.            unsigned int in1 : 1; //0
//.            unsigned int in2 : 1; //1
//.            unsigned int in3 : 1; //2
//.            unsigned int in4 : 1; //3
//.            unsigned int in5 : 1; //4 
//.            unsigned int in6 : 1; //5 
//.            unsigned int in7 : 1; //6 
//.            unsigned int in8 : 1; //7    
//.        } in_bool; //
//.    unsigned char Blk;
//.    struct {
//.        unsigned int out1 : 1; //0
//.        unsigned int out2 : 1; //1
//.        unsigned int out3 : 1; //2
//.        unsigned int out4 : 1; //3
//.        unsigned int out5 : 1; //4 
//.        unsigned int out6 : 1; //5 
//.        unsigned int out7 : 1; //6 
//.        unsigned int out8 : 1; //7    
//.    } out_bool; //unsigned char Out;
//.    unsigned char Res;
//.   }Lan_Hld;
//.    
//.} LanBlock_stt; 
  
typedef union mms_block_Unn{ 
    unsigned char arCh[4];
    unsigned short arSh[2];
    unsigned long Mms_Vl;
    struct{
        struct {
            unsigned int in1 : 1; //0
            unsigned int in2 : 1; //1
            unsigned int in3 : 1; //2
            unsigned int in4 : 1; //3
            unsigned int in5 : 1; //4 
            unsigned int in6 : 1; //5 
            unsigned int in7 : 1; //6 
            unsigned int in8 : 1; //7    
        } in_bool; //
    unsigned char Blk;
    struct {
        unsigned int out1 : 1; //0
        unsigned int out2 : 1; //1
        unsigned int out3 : 1; //2
        unsigned int out4 : 1; //3
        unsigned int out5 : 1; //4 
        unsigned int out6 : 1; //5 
        unsigned int out7 : 1; //6 
        unsigned int out8 : 1; //7    
    } out_bool; //unsigned char Out;
    unsigned char Res;
   }Mms_Hld;
    
} MmsBlock_stt; 
typedef union goose_block_Unn{ 
   unsigned char arCh[4];
   unsigned short arSh[2];
   unsigned long GS_Vl;
   struct{
        struct {
            unsigned int in1 : 1; //0
            unsigned int in2 : 1; //1
            unsigned int in3 : 1; //2
            unsigned int in4 : 1; //3
            unsigned int in5 : 1; //4 
            unsigned int in6 : 1; //5 
            unsigned int in7 : 1; //6 
            unsigned int in8 : 1; //7    
        } in_bool; //
    unsigned char Blk;
    struct {
        unsigned int out1 : 1; //0
        unsigned int out2 : 1; //1
        unsigned int out3 : 1; //2
        unsigned int out4 : 1; //3
        unsigned int out5 : 1; //4 
        unsigned int out6 : 1; //5 
        unsigned int out7 : 1; //6 
        unsigned int out8 : 1; //7    
    } out_bool; //unsigned char Out;
    unsigned char Res;
   }GS_Hld;
    
} GsBlock_stt; //
typedef struct tag_GsBlkParam
{
    union {
        unsigned char   u8Ar [4];
        unsigned short  u16Ar [2];
        unsigned long   ulV;
        struct {
        unsigned char OrdNumGsBlk;
        //unsigned char Res;
        } GsBlkHdr;
    }unnV1[N_IN_GOOSE];
    //void* pAddrGsBlk[N_IN_GOOSE];
    char ch_amount_blk_src;
    char ch_amount_active_src;
    short sh_amount_gs_active_src;//use whith arrOrdNumsGsSignal
}GsBlkParamDsc;
typedef struct tag_MmsBlkParam
{
    union {
        unsigned char   u8Ar [4];
        unsigned short  u16Ar [2];
        unsigned long   ulV;
        struct {
        unsigned char OrdNumMmsBlk;
        //unsigned char Res;
        } MmsBlkHdr;
    }unnV1[N_IN_MMS];
    //void* pAddrGsBlk[N_IN_MMS];
    char ch_amount_blk_src;
    char ch_amount_active_src;
    short sh_amount_mms_active_src;//use whith arrOrdNumsGsSignal
}MmsBlkParamDsc;
typedef struct tag_LanBlkParam
{
    union {
        unsigned char   u8Ar [4];
        unsigned short  u16Ar [2];
        unsigned long   ulV;
        struct {
        unsigned char OrdNumLanBlk;
        //unsigned char Res;
        } LanBlkHdr;
    }unnV1[N_OUT_LAN];
    //void* pAddrGsBlk[N_OUT_LAN];

}LanBlkParamDsc;

static GsBlock_stt  arrGsBlk [N_IN_GOOSE];
static MmsBlock_stt arrMmsBlk[N_IN_MMS  ];
//static LanBlock_stt arrLanBlk[N_OUT_LAN ];

static GsBlkParamDsc hldGsBlkParam;
static MmsBlkParamDsc hldMmsBlkParam;
static LanBlkParamDsc hldLanBlkParam = {
    LAN_BLOCK_ORD_NUM_00,0,0,0,
    LAN_BLOCK_ORD_NUM_01,0,0,0,
    LAN_BLOCK_ORD_NUM_02,0,0,0,
    LAN_BLOCK_ORD_NUM_03,0,0,0
};

static short arrOrdNumsGsSignal [N_IN_GOOSE*N_IN_GOOSE_MMS_OUT];
static short arrOrdNumsMmsSignal[N_IN_MMS  *N_IN_GOOSE_MMS_OUT];



struct{
    char  ch_while_breaker;
    char ch_amount_blk_src,ch_amount_mms_blk_src;
    char ch_amount_active_src,ch_amount_mms_active_src;
    char ch_amount_GsSignal,ch_amount_MmsSignal;
    __SETTINGS *p_current_settings_prt;
    unsigned int *p_active_functions;

}sLV;
    sLV.p_active_functions = &active_functions[0];
  //Check GOOSE Blk Cmd
  //RANG_BLOCK_IN_GOOSE1
//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//     GOOSE BLOCK             
//....................................................................................................
//=====================================================================================================
    do{
     sLV.ch_amount_active_src = sLV.ch_amount_blk_src = sLV.ch_while_breaker = 0;//! optimize then
     sLV.ch_amount_GsSignal =  sLV.ch_amount_MmsSignal = 0;
     register long i,lV;
     //register void* pv;
     register unsigned int *r_p_active_functions;
// ----------------    -------------------------
//    i = (RANG_BLOCK_IN_GOOSE1>>5);               <- old code
//    lV = sLV.p_active_functions[i];//whole word  <- old code
//    sLV.wrp.lVl = lV;                            <- old code
    //pv = (void*) sLV.p_active_functions;
    r_p_active_functions = sLV.p_active_functions;
    
    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), RANG_BLOCK_IN_GOOSE1) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_00;
        sLV.ch_amount_blk_src++;
		
		arrGsBlk[GS_BLOCK_ORD_NUM_00].GS_Hld.Blk = 1;
		
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_00;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_00].GS_Hld.Blk = 0;
		
    }
	
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 1)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_01;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_01].GS_Hld.Blk = 1;
    }else{
        //Insert
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_01;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_01].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 2)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_02;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_02].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_02;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_02].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 3)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_03;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_03].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_03;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_03].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 4)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_04;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_04].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_04;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_04].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 5)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_05;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_05].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_05;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_05].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 6)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_06;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_06].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_06;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_06].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 7)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_07;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_07].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_07;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_07].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 8)) != 0){
    //clr block
                i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_08;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_08].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_08;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_08].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 9)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_09;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_09].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_09;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_09].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 10)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_10;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_10].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_10;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_10].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 11)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_11;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_11].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_11;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_11].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 12)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_12;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_12].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_12;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_12].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 13)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_13;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_13].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_13;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_13].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 14)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_14;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_14].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_14;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_14].GS_Hld.Blk = 0;
    }
    if(_CHECK_SET_BIT( ((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_GOOSE1 + 15)) != 0){
    //clr block
        i = GS_BLOCK_ORD_NUM_15 - sLV.ch_amount_blk_src;
        hldGsBlkParam.unnV1[i].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_15;
        sLV.ch_amount_blk_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_15].GS_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_active_src;
        hldGsBlkParam.unnV1[lV].GsBlkHdr.OrdNumGsBlk = GS_BLOCK_ORD_NUM_15;
        sLV.ch_amount_active_src++;
		arrGsBlk[GS_BLOCK_ORD_NUM_15].GS_Hld.Blk = 0;
    }
        hldGsBlkParam.ch_amount_blk_src    = sLV.ch_amount_blk_src;
        hldGsBlkParam.ch_amount_active_src = sLV.ch_amount_active_src;


    }while(sLV.ch_while_breaker);
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````


//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//             MMS BLOCK     
//....................................................................................................
//=====================================================================================================
do{
    sLV.ch_while_breaker = 0;sLV.ch_amount_mms_blk_src = sLV.ch_amount_mms_active_src = 0;//! optimize then
    register long i,lV;
    register void* pv;
    
     register unsigned int *r_p_active_functions;
// ----------------    -------------------------
    r_p_active_functions = sLV.p_active_functions;
    pv = (void*)&hldMmsBlkParam;
    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_MMS1 + ((unsigned int)MMS_BLOCK_ORD_NUM_00))) != 0){
    //clr block
        i = MMS_BLOCK_ORD_NUM_03 - sLV.ch_amount_mms_blk_src;
        ((MmsBlkParamDsc*)pv)->unnV1[i].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_00;
        sLV.ch_amount_mms_blk_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_00].Mms_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_mms_active_src;
        //hldGsBlkParam.pAddrGsBlk[lV] = (void*)(&arrMmsBlk[GS_BLOCK_ORD_NUM_00]);
        hldMmsBlkParam.unnV1[lV].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_00;
		//hldMmsBlkParam.unnV1[MMS_BLOCK_ORD_NUM_00].u8Ar[] = 
		//arrMmsBlk[MMS_BLOCK_ORD_NUM_00].arCh[2] = 0;
        sLV.ch_amount_mms_active_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_00].Mms_Hld.Blk = 0;
    }

    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_MMS1+((unsigned int)MMS_BLOCK_ORD_NUM_01))) != 0){
    //clr block
        i = MMS_BLOCK_ORD_NUM_03 - sLV.ch_amount_mms_blk_src;
        hldMmsBlkParam.unnV1[i].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_01;
        sLV.ch_amount_mms_blk_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_01].Mms_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_mms_active_src;
        //hldGsBlkParam.pAddrGsBlk[lV] = (void*)(&arrMmsBlk[GS_BLOCK_ORD_NUM_00]);
        hldMmsBlkParam.unnV1[lV].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_01;
        sLV.ch_amount_mms_active_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_01].Mms_Hld.Blk = 0;
    }

    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_MMS1+((unsigned int)MMS_BLOCK_ORD_NUM_02))) != 0){
    //clr block
        i = MMS_BLOCK_ORD_NUM_03 - sLV.ch_amount_mms_blk_src;
        hldMmsBlkParam.unnV1[i].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_02;
        sLV.ch_amount_mms_blk_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_02].Mms_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_mms_active_src;
        //hldGsBlkParam.pAddrGsBlk[lV] = (void*)(&arrMmsBlk[GS_BLOCK_ORD_NUM_00]);
        hldMmsBlkParam.unnV1[lV].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_02;
        sLV.ch_amount_mms_active_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_02].Mms_Hld.Blk = 0;
    }

    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_IN_MMS1+((unsigned int)MMS_BLOCK_ORD_NUM_03))) != 0){
    //clr block
        i = MMS_BLOCK_ORD_NUM_03 - sLV.ch_amount_mms_blk_src;
        hldMmsBlkParam.unnV1[i].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_03;
        sLV.ch_amount_mms_blk_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_03].Mms_Hld.Blk = 1;
    }else{
        lV = sLV.ch_amount_mms_active_src;
        //hldGsBlkParam.pAddrGsBlk[lV] = (void*)(&arrMmsBlk[GS_BLOCK_ORD_NUM_00]);
        hldMmsBlkParam.unnV1[lV].MmsBlkHdr.OrdNumMmsBlk = MMS_BLOCK_ORD_NUM_03;
        sLV.ch_amount_mms_active_src++;
		arrMmsBlk[MMS_BLOCK_ORD_NUM_03].Mms_Hld.Blk = 0;
    }

    hldMmsBlkParam.ch_amount_blk_src    = sLV.ch_amount_blk_src;
    hldMmsBlkParam.ch_amount_active_src = sLV.ch_amount_active_src;
	if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_MISCEVE_DYSTANCIJNE )) != 1)
		hldMmsBlkParam.ch_amount_blk_src |= 0x80;



   
}while(sLV.ch_while_breaker);
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````


  
  
  /***/
#endif

  copying_active_functions = 1; //�������, �� ����� ���������� �������� �������� �������
  
  //������� � �������, �� ���������� �� �����, ������ � ��������� � ����������
  static const unsigned int maska_input_signals[N_BIG] = 
  {
    MASKA_FOR_INPUT_SIGNALS_0, 
    MASKA_FOR_INPUT_SIGNALS_1, 
    MASKA_FOR_INPUT_SIGNALS_2, 
    MASKA_FOR_INPUT_SIGNALS_3, 
    MASKA_FOR_INPUT_SIGNALS_4, 
    MASKA_FOR_INPUT_SIGNALS_5, 
    MASKA_FOR_INPUT_SIGNALS_6, 
    MASKA_FOR_INPUT_SIGNALS_7
      
#ifdef fMASKA_FOR_INPUT_SIGNALS_8
                             ,
    MASKA_FOR_INPUT_SIGNALS_8
#endif
  };
  for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_input_signals[i]);
  
  /**************************/
  //��������, �� ����� �������� ������ �������
  /**************************/
  if (reset_trigger_function_from_interface !=0)
  {
    if ((reset_trigger_function_from_interface & (1 << USB_RECUEST)) != 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++) trigger_functions_USB[i] = 0;
      
      information_about_restart_counter  &= (unsigned int)(~(1 << USB_RECUEST));
      information_about_clean_energy     &= (unsigned int)(~(1 << USB_RECUEST));
    }
    if ((reset_trigger_function_from_interface & (1 << RS485_RECUEST)) != 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++) trigger_functions_RS485[i] = 0;
      
      information_about_restart_counter  &= (unsigned int)(~(1 << RS485_RECUEST));
      information_about_clean_energy     &= (unsigned int)(~(1 << RS485_RECUEST));
    }
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    if ((reset_trigger_function_from_interface & (1 << LAN_RECUEST)) != 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++) trigger_functions_LAN[i] = 0;
      
      information_about_restart_counter  &= (unsigned int)(~(1 << LAN_RECUEST));
      information_about_clean_energy     &= (unsigned int)(~(1 << LAN_RECUEST));
    }
#endif
    
    //������� �� �� �������� ������� �� �Ѳ� �����������
    reset_trigger_function_from_interface = 0;
  }
  /**************************/

  unsigned int active_inputs_grupa_ustavok = 0;
  /**************************/
  //����������� ��, �������� ����� � ������ �����
  /**************************/
  //���������� ����� ���������� ������
  uint32_t pressed_buttons_tmp = 0;
  uint32_t buttons_mode_tmp = current_settings_prt.buttons_mode;
  if (
      (mutex_buttons == false) &&
      (pressed_buttons != 0)
     )   
  {
    pressed_buttons_tmp = pressed_buttons;
    //������� �������� �����b, ���������� ��� ���� �� ��� ������� � ������ (��� �� �� ����������)
    pressed_buttons = 0;
  }
      
  uint32_t pressed_buttons_switcher = pressed_buttons_tmp & buttons_mode_tmp; /*�������� � ����� ������ ������-�����*/
  pressed_buttons_tmp &= (uint32_t)(~buttons_mode_tmp); /*�������� � ����� ������ �� ���� ���� ������*/
      
  //���������� �������� ������-�����
  uint32_t fix_active_buttons_tmp = (fix_active_buttons ^ pressed_buttons_switcher) & buttons_mode_tmp;
  if (fix_active_buttons_tmp != fix_active_buttons) 
  {
    fix_active_buttons = fix_active_buttons_tmp;
    /*
    �������� ����� ������-������
    */
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT);
  }
    
  //������� ���� ���������� ������ � ����������� ������ ����� ����� ����� � ����� ������
  uint32_t pressed_buttons_united = pressed_buttons_tmp | fix_active_buttons_tmp;

  //������� ��������� ���������� ������-������
  for (size_t i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
  {
    uint32_t maska_yellow = ((1 << (i*NUMBER_LED_COLOR + LED_COLOR_RED_BIT)) | (1 << (i*NUMBER_LED_COLOR + LED_COLOR_GREEN_BIT)));
    if (((buttons_mode_tmp >> i) & 0x1) == BUTTON_MODE_SWITCHER)
    {
      if (((fix_active_buttons_tmp >> i) & 0x1) != 0) 
      {
#ifndef KEYBOARD_VER_2_1
        state_leds_Fx[0] &= (uint32_t)(~maska_yellow);
        state_leds_Fx[1] |= maska_yellow;
#else
        if (i != (4 - 1))
        {
          state_leds_Fx[0] &= (uint32_t)(~maska_yellow);
          state_leds_Fx[1] |= maska_yellow;
        }
        else
        {
          state_leds_Fx[1] &= (uint32_t)(~maska_yellow);
          state_leds_Fx[0] |= maska_yellow;
        }
#endif
      }    
      else
      {
#ifndef KEYBOARD_VER_2_1
        state_leds_Fx[1] &= (uint32_t)(~maska_yellow);
        state_leds_Fx[0] |= maska_yellow;
#else
        if (i != (4 - 1))
        {
          state_leds_Fx[1] &= (uint32_t)(~maska_yellow);
          state_leds_Fx[0] |= maska_yellow;
        }
        else
        {
          state_leds_Fx[0] &= (uint32_t)(~maska_yellow);
          state_leds_Fx[1] |= maska_yellow;
        }
#endif
      }    
    }
    else
    {
      state_leds_Fx[0] &= (uint32_t)(~maska_yellow);
      state_leds_Fx[1] &= (uint32_t)(~maska_yellow);
    }
  }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  /***
  ����������� �� � ��������� ������ ��.GOOSE ����� (� �����������, �� ���� �� ���� ����������� ��������� � ����� ���������� � ��������� ����) � ��.MMS �����
  ***/
  
    //=====================================================================================================
    //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    //    state_outs_copy = state_outs;
    //    state_outs_copy &= Input_ctrl_In_GOOSE_block;//First clear changed bits
    //    j = state_outs^Input_In_GOOSE_block;//Select changed position
    //    l = j"Select changed position" & Input_In_GOOSE_block;//Select Ones in bits
    //    state_outs_copy |= l&Input_In_GOOSE_block;//Move changed ones; <-have Error
    //....................................................................................................
    //=====================================================================================================
    do{
    // ----------------    -------------------------
     
     register long i,lV,j;
     register long lIdxBlk;
     
     register void *pvl;
     
        pvl = (void*)&hldGsBlkParam;
        sLV.ch_amount_active_src = i = N_IN_GOOSE;//hldGsBlkParam.ch_amount_active_src;
        sLV.ch_amount_GsSignal = 0;
        if(i > 0){
            ((GsBlkParamDsc*)pvl)->sh_amount_gs_active_src = lV = lIdxBlk = 0;asm ("nop" ::"r"(lIdxBlk),"r"(lV));
            while(lIdxBlk < N_IN_GOOSE){ //do{
                //--i = ((GsBlkParamDsc*)pvl)-> unnV1[lIdxBlk].GsBlkHdr.OrdNumGsBlk;;
                //--//Check State Inputs                                            ;
                //--lV = arrGsBlk[i].arCh[2];//Outs                                 ;
                i = lIdxBlk;
                lV = arrGsBlk[i].arCh[2];//!!Outs arrGsBlk[i].arCh[0]
                
				
                lV &= ~((unsigned long)(Input_ctrl_In_GOOSE_block[i]));//Clear all changeble bits
                lV |= Input_In_GOOSE_block[i]&Input_ctrl_In_GOOSE_block[i];//Set ones only
                //if(noerror)
                    arrGsBlk[i].arCh[2] = lV;//arrGsBlk[i].arCh[0]
                if(arrGsBlk[i].GS_Hld.Blk == 0){   
					register unsigned long wrpLV;
					wrpLV = arrGsBlk[i].arCh[2];//!!arrGsBlk[i].arCh[2] = arrGsBlk[i].arCh[0]
					j = 0;lV = ((GsBlkParamDsc*)pvl)->sh_amount_gs_active_src;
					while( (wrpLV != 0) ){//&& j < 8
						if((wrpLV & (1<<j)) != 0 ){
							arrOrdNumsGsSignal[lV] = (lIdxBlk<<3)+j;lV++;wrpLV &= ~(1<<j);
						}
						j++;
						
					}
					((GsBlkParamDsc*)pvl)->sh_amount_gs_active_src = lV;
				}
                lIdxBlk++;//  += lV;//! ????
            }//while(lIdxBlk);//(--sLV.ch_amount_active_src) != 0
            sLV.ch_amount_GsSignal = ((GsBlkParamDsc*)pvl)->sh_amount_gs_active_src;
        }
    //Try bield list of outs which sets in ones.
    
    }while(sLV.ch_while_breaker);
    //
    //--------------------------------------------------------------------------------------------------------
    //````````````````````````````````````````````````````````````````````````````````````````````````````````


    //=====================================================================================================
    //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    //   
    //   
    //   
    //   
    //   
    //....................................................................................................
    //=====================================================================================================
    do{
    // ----------------    -------------------------
     register long i,lV,j;
     register long lIdxBlk;
     register void *pvl;
        pvl = (void*)&hldMmsBlkParam;
        sLV.ch_amount_active_src = i  = hldMmsBlkParam.ch_amount_active_src;
        if(i > 0){
            ((MmsBlkParamDsc*)pvl)->sh_amount_mms_active_src = lV = lIdxBlk = 0;asm ("nop" ::"r"(lIdxBlk),"r"(lV));
            while(lIdxBlk < N_IN_MMS){ //do{
                //--;
                //--;
                //--;
                i = lIdxBlk;//.i = ((MmsBlkParamDsc*)pvl)-> unnV1[lIdxBlk].MmsBlkHdr.OrdNumMmsBlk;
                //Check State Inputs
                lV = arrMmsBlk[i].arCh[2];//Outs
                lV &= ~((unsigned long)(Input_In_MMS_block[i]));//Clear all changeble bits
                lV |= Input_In_MMS_block[i]&Input_ctrl_In_MMS_block[i];//Set ones only
                //if(noerror)
                    arrMmsBlk[i].arCh[2] = lV;//
                if(arrMmsBlk[i].Mms_Hld.Blk == 0 && (((MmsBlkParamDsc*)pvl)->ch_amount_blk_src&0x80)){ 
                    register unsigned long wrpLV;
        
					wrpLV = lV;//arrMmsBlk[i].arCh[2];
					j = 0;lV = ((MmsBlkParamDsc*)pvl)->sh_amount_mms_active_src;
					while( (wrpLV != 0) ){//&& j < 8
						if((wrpLV & (1<<j)) != 0 ){
							arrOrdNumsMmsSignal[lV] = (lIdxBlk<<3)+j;lV++;wrpLV &= ~(1<<j);
						}
						j++;
					}
                }
                ((MmsBlkParamDsc*)pvl)->sh_amount_mms_active_src = lV;
                lIdxBlk++;//  += lV;//! ????
            }//while(lIdxBlk);//(--sLV.ch_amount_active_src) != 0
            sLV.ch_amount_MmsSignal = ((MmsBlkParamDsc*)pvl)->sh_amount_mms_active_src; 
        }    

    //Try bield list of outs which sets in ones.
    
    }while(sLV.ch_while_breaker);
    //
    //--------------------------------------------------------------------------------------------------------
    //````````````````````````````````````````````````````````````````````````````````````````````````````````


  
  
  
  
  
  /***/
#endif

  //���������� �� � ����� ������ �����
  int flag=0;
  for(int m=0; m<N_SMALL; m++) if(activation_function_from_interface[m] != 0) {flag=1; break;}
  if (
      (pressed_buttons_united !=0)
      ||  
      (  
       (mutex_interface == false) &&
       (flag)
      )
      ||  
      (active_inputs !=0)
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
     /* ���� � ��������� ������ �� ��.GOOSE ����� � ��.MMS �����*/   
      || ((sLV.ch_amount_MmsSignal+sLV.ch_amount_GsSignal)>0)
#endif
     )   
  {
    unsigned int temp_value_for_activated_function_button_interface[N_SMALL];
    unsigned int temp_value_for_activated_function[N_SMALL];
    for(int m=0; m<N_SMALL; m++) temp_value_for_activated_function_button_interface[m]=0;
    for(int m=0; m<N_SMALL; m++) temp_value_for_activated_function[m]=0;

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    /***
    ���������� ����� ��.GOOSE ����� � ��.MMS ����� � ��������� �������, �� ���� �����������
    ***/
 
    /**************************/
    //����������� ����� ��� ��.GOOSE ����� � ��������� �������� �� � temp_value_for_activated_function �����
    /**************************/
    /**************************/
    //=====================================================================================================
    //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    //                  
    //....................................................................................................
    //=====================================================================================================
    do{
        sLV.ch_while_breaker = 0;
    // ----------------    -------------------------       
//        register unsigned long i,lV,j;
//         unsigned long lCtrGsSrc;
//         register void *pvlc,*pvll;
//        pvlc = (void*)temp_value_for_activated_function;//ui32
//        pvll = (void*)&current_settings_prt.ranguvannja_In_GOOSE;//uint32_t
//        i = j = lV = 0;
//        //sLV.ch_amount_active_src = hldGsBlkParam.ch_amount_active_src;
//        //pvl = (void*)&hldGsBlkParam;((GsBlkParamDsc*)pvl)->
//        lCtrGsSrc = hldGsBlkParam.sh_amount_gs_active_src;   
//        asm ("nop" ::"r"(i),"r"(j),"r"(lV));
//        while(lCtrGsSrc){
//            lCtrGsSrc--;
//            lV = arrOrdNumsGsSignal[lCtrGsSrc];
//            i = lV>>3;j = lV - (i<<3);
//            
//            
//            //temp_value_for_activated_function[N_SMALL];//;ranguvannja_In_GOOSE[i][j][0]
//            lV = i*N_IN_GOOSE_MMS_OUT*N_SMALL*sizeof(long) +j*N_SMALL*sizeof(long);
//            for(register unsigned long k = 0; k < N_SMALL; k++){
//                ((unsigned long*)pvlc)[k] |= ((unsigned long*)pvll+lV) [k];
//            }
        proc_Gs_blk_out((void*)&temp_value_for_activated_function,hldGsBlkParam.sh_amount_gs_active_src,arrOrdNumsGsSignal );    
//        }
    }while(sLV.ch_while_breaker);
    //
    //--------------------------------------------------------------------------------------------------------
    //````````````````````````````````````````````````````````````````````````````````````````````````````````
    

    /**************************/
    //����������� ����� ��� ��.MMS ����� ��������� �������� �� � temp_value_for_activated_function_button_interface �����
    /**************************/
    /**************************/
    //=====================================================================================================
    //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    //                  
    //....................................................................................................
    //=====================================================================================================
    do{
        sLV.ch_while_breaker = 0;
    // ----------------    -------------------------       
//        register long i,lV,j,lCtrMmsSrc;
//         register void *pvlc,*pvll;
//        pvlc = (void*)temp_value_for_activated_function;
//        pvll = (void*)&current_settings_prt.ranguvannja_In_MMS;
//        //sLV.ch_amount_active_src = hldGsBlkParam.ch_amount_active_src;
//        //pvl = (void*)&hldGsBlkParam;((GsBlkParamDsc*)pvl)->
//        lCtrMmsSrc = hldMmsBlkParam.sh_amount_mms_active_src;   
        proc_Mms_blk_out((void*)&temp_value_for_activated_function_button_interface,hldMmsBlkParam.sh_amount_mms_active_src,arrOrdNumsMmsSignal );        
//        while(lCtrMmsSrc){
//            lV = arrOrdNumsMmsSignal[lCtrMmsSrc];
//            i = lV>>3;j = lV - i;
//            lCtrMmsSrc--;
//            //temp_value_for_activated_function[N_SMALL];//;ranguvannja_In_GOOSE[i][j][0]
//            lV = i*N_IN_GOOSE_MMS_OUT*N_SMALL*sizeof(long) +j*N_SMALL*sizeof(long);
//            for(long k = 0; k < N_SMALL; k++){
//                ((long*)pvlc)[k] |= ((long*)pvll+lV) [k];
//            }
//            
//        }
    }while(sLV.ch_while_breaker);
    //
    //--------------------------------------------------------------------------------------------------------
    //````````````````````````````````````````````````````````````````````````````````````````````````````````
            
  
    /***/
#endif
  
    //��������� � �������
    if (pressed_buttons_united != 0)
    {
      for (unsigned int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        if ((pressed_buttons_united & (1 << i)) != 0)
        {
          unsigned int *point_temp_value_for_activated_function = ((pressed_buttons_tmp & (1 << i)) != 0) ? temp_value_for_activated_function_button_interface : temp_value_for_activated_function;
          for (size_t j = 0; j < N_SMALL; ++j ) point_temp_value_for_activated_function[j] |= current_settings_prt.ranguvannja_buttons[N_SMALL*i + j];
        }
      }
      if ((pressed_buttons_united & (1 << (BIT_KEY_C - BIT_KEY_1))) != 0)
      {
        _SET_BIT(temp_value_for_activated_function, RANG_SMALL_RESET_LEDS);
        _SET_BIT(temp_value_for_activated_function, RANG_SMALL_RESET_RELES);
      }
      if ((pressed_buttons_united & (1 << (BIT_KEY_I - BIT_KEY_1))) != 0) _SET_BIT(temp_value_for_activated_function, RANG_SMALL_VKL_VV);
      if ((pressed_buttons_united & (1 << (BIT_KEY_O - BIT_KEY_1))) != 0) _SET_BIT(temp_value_for_activated_function, RANG_SMALL_OTKL_VV);
    }
    
    //��������� � ����������
    if (mutex_interface == false)
    {
      for (size_t i = 0; i < N_SMALL; ++i)
      {
        temp_value_for_activated_function_button_interface[i] |= activation_function_from_interface[i];
        activation_function_from_interface[i] = 0;
      }
    }
    
    //������� ��������� � ������ (����� ������) + ��������� � ���������� � ������ (����� ����) + ������ ����������� �����������
    for (size_t i = 0; i < N_SMALL; ++i) temp_value_for_activated_function[i] |= temp_value_for_activated_function_button_interface[i];

    //��������� � �.�����
    if (active_inputs != 0)
    {
      for (unsigned int i = 0; i < NUMBER_INPUTS; i++)
      {
        if ((active_inputs & (1 << i)) != 0)
        {
          for (size_t j = 0; j < N_SMALL; ++j) temp_value_for_activated_function[j] |= current_settings_prt.ranguvannja_inputs[N_SMALL*i + j];
        }
      }
    }
    
    //���������� �������
    for (size_t i = 0; i != NUMBER_DEFINED_FUNCTIONS; ++i)
    {
      size_t index_big = RANG_DF1_IN + 2*i;
      size_t index_small = RANG_SMALL_DF1_IN + i;
      active_functions[index_big >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, index_small) != 0) << (index_big & 0x1f);
    
      //�������� �� ����������� ����������� ��������� �-������� �� ���  ������� �����
      if (_CHECK_SET_BIT(temp_value_for_activated_function_button_interface, (RANG_SMALL_DF1_IN + i)) != 0)
      {
        //����� �� ������������ �-��������, ���� ����� ��������� ������ �� �����������,
        //��� ����, ��� ���� ��� ��� ����� ���� ������� � ����� ������� �����
        if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] < 0)
        {
          //��������� ������ ������ ����������� ���� ������� � ��������� ���� (������� ��������� �����)
          //������ ������ ����� �� ���� ��������, ���� �� �� �� �������
          global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] = 0;
        }
      }
    }

    //���������� ������
    for (size_t i = 0; i != NUMBER_DEFINED_TRIGGERS; ++i)
    {
      size_t index_big = RANG_DT1_SET + 3*i;
      size_t index_small = RANG_SMALL_DT1_SET + 2*i;
      active_functions[(index_big    ) >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, (index_small    )) != 0) << ((index_big    ) & 0x1f);
      active_functions[(index_big + 1) >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, (index_small + 1)) != 0) << ((index_big + 1) & 0x1f);
    }

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    {
      size_t word_n_small = RANG_SMALL_BLOCK_IN_GOOSE1 >> 5;
      unsigned int maska_small = (unsigned int)(1 << (RANG_SMALL_BLOCK_IN_GOOSE1 & 0x1f));
      
      size_t word_n_full = RANG_BLOCK_IN_GOOSE1 >> 5;
      unsigned int maska_full = (unsigned int)(1 << (RANG_BLOCK_IN_GOOSE1 & 0x1f));

      for (size_t i = 0; i < (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN); i++)
      {
        if (temp_value_for_activated_function[word_n_small] & maska_small) 
        {
          active_functions[word_n_full] |= maska_full;
        }
        
        maska_small <<= 1;
        if (maska_small == 0)
        {
          word_n_small++;
          maska_small = 1;
        }
        
        maska_full <<= 1;
        if (maska_full == 0)
        {
          word_n_full++;
          maska_full = 1;
        }
      }
    }
#endif    
      
    //������� ������� (��� ��-�� � �������, �� ������ ����������� � �������� ��������)
    active_functions[RANG_BLOCK_VKL_VV                      >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_VKL_VV                     ) != 0) << (RANG_BLOCK_VKL_VV                      & 0x1f);
    active_functions[RANG_RESET_LEDS                        >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_RESET_LEDS                       ) != 0) << (RANG_RESET_LEDS                        & 0x1f);
    active_functions[RANG_RESET_RELES                       >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_RESET_RELES                      ) != 0) << (RANG_RESET_RELES                       & 0x1f);
    active_functions[RANG_MISCEVE_DYSTANCIJNE               >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_MISCEVE_DYSTANCIJNE              ) != 0) << (RANG_MISCEVE_DYSTANCIJNE               & 0x1f);
    active_functions[RANG_STATE_VV                          >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_STATE_VV                         ) != 0) << (RANG_STATE_VV                          & 0x1f);
    active_functions[RANG_CTRL_VKL                          >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_CTRL_VKL                         ) != 0) << (RANG_CTRL_VKL                          & 0x1f);
    active_functions[RANG_CTRL_OTKL                         >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_CTRL_OTKL                        ) != 0) << (RANG_CTRL_OTKL                         & 0x1f);
    active_functions[RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_RESET_BLOCK_READY_TU_VID_ZAHYSTIV) != 0) << (RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV & 0x1f);
    active_functions[RANG_OTKL_VID_ZOVN_ZAHYSTIV            >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_OTKL_VID_ZOVN_ZAHYSTIV     ) != 0) << (RANG_OTKL_VID_ZOVN_ZAHYSTIV            & 0x1f);

    active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_1_GRUPA_USTAVOK    ) != 0) << (RANG_SMALL_1_GRUPA_USTAVOK - RANG_SMALL_1_GRUPA_USTAVOK);
    active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_2_GRUPA_USTAVOK    ) != 0) << (RANG_SMALL_2_GRUPA_USTAVOK - RANG_SMALL_1_GRUPA_USTAVOK);
    active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_3_GRUPA_USTAVOK    ) != 0) << (RANG_SMALL_3_GRUPA_USTAVOK - RANG_SMALL_1_GRUPA_USTAVOK);
    active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_4_GRUPA_USTAVOK    ) != 0) << (RANG_SMALL_4_GRUPA_USTAVOK - RANG_SMALL_1_GRUPA_USTAVOK);
      
    //��������� ��
    active_functions[RANG_VKL_VV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_VKL_VV) != 0) << (RANG_VKL_VV & 0x1f);

    //��������� ��
    active_functions[RANG_OTKL_VV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_OTKL_VV) != 0) << (RANG_OTKL_VV & 0x1f);

    //���
    active_functions[RANG_BLOCK_MTZ1     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_MTZ1    ) != 0) << (RANG_BLOCK_MTZ1     & 0x1f);
    active_functions[RANG_BLOCK_MTZ2     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_MTZ2    ) != 0) << (RANG_BLOCK_MTZ2     & 0x1f);
    active_functions[RANG_BLOCK_USK_MTZ2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_USK_MTZ2) != 0) << (RANG_BLOCK_USK_MTZ2 & 0x1f);
    active_functions[RANG_BLOCK_MTZ3     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_MTZ3    ) != 0) << (RANG_BLOCK_MTZ3     & 0x1f);
    active_functions[RANG_BLOCK_MTZ4     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_MTZ4    ) != 0) << (RANG_BLOCK_MTZ4     & 0x1f);

    //���� ��� ��� 0.4��
    active_functions[RANG_BLOCK_MTZ04_1     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_MTZ04_1    ) != 0) << (RANG_BLOCK_MTZ04_1     & 0x1f);
    active_functions[RANG_BLOCK_MTZ04_2     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_MTZ04_2    ) != 0) << (RANG_BLOCK_MTZ04_2     & 0x1f);
    active_functions[RANG_BLOCK_USK_MTZ04_2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_USK_MTZ04_2) != 0) << (RANG_BLOCK_USK_MTZ04_2 & 0x1f);

    //���� ��� ���
    active_functions[RANG_BLOCK_ZDZ       >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_ZDZ      ) != 0) << (RANG_BLOCK_ZDZ       & 0x1f);
    active_functions[RANG_PUSK_ZDZ_VID_DV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_PUSK_ZDZ_VID_DV) != 0) << (RANG_PUSK_ZDZ_VID_DV & 0x1f);

    //���
    active_functions[RANG_BLOCK_NZZ >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_NZZ) != 0) << (RANG_BLOCK_NZZ & 0x1f);

    //���� ����
    active_functions[RANG_BLOCK_TZNP1 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_TZNP1) != 0) << (RANG_BLOCK_TZNP1 & 0x1f);
    active_functions[RANG_BLOCK_TZNP2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_TZNP2) != 0) << (RANG_BLOCK_TZNP2 & 0x1f);
    active_functions[RANG_BLOCK_TZNP3 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_TZNP3) != 0) << (RANG_BLOCK_TZNP3 & 0x1f);

    //���
    active_functions[RANG_STAT_BLK_APV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_STAT_BLK_APV) != 0) << (RANG_STAT_BLK_APV & 0x1f);
      
    //����
    active_functions[RANG_ACHR_CHAPV_VID_DV  >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_ACHR_CHAPV_VID_DV ) != 0) << (RANG_ACHR_CHAPV_VID_DV & 0x1f);
    active_functions[RANG_BLOCK_ACHR1        >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_ACHR1      ) != 0) << (RANG_BLOCK_ACHR1       & 0x1f);
    active_functions[RANG_BLOCK_ACHR2        >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_ACHR2      ) != 0) << (RANG_BLOCK_ACHR2       & 0x1f);

    //����
    active_functions[RANG_PUSK_UROV_VID_DV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_PUSK_UROV_VID_DV) != 0) << (RANG_PUSK_UROV_VID_DV & 0x1f);

    //���� ���(���)
    active_functions[RANG_BLOCK_ZOP >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_ZOP) != 0) << (RANG_BLOCK_ZOP & 0x1f);

    //���� ��� Umin
    active_functions[RANG_BLOCK_UMIN1 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_UMIN1) != 0) << (RANG_BLOCK_UMIN1 & 0x1f);
    active_functions[RANG_START_UMIN1 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_START_UMIN1) != 0) << (RANG_START_UMIN1 & 0x1f);
    active_functions[RANG_BLOCK_UMIN2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_UMIN2) != 0) << (RANG_BLOCK_UMIN2 & 0x1f);
    active_functions[RANG_START_UMIN2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_START_UMIN2) != 0) << (RANG_START_UMIN2 & 0x1f);
      
    //���� ��� Umax
    active_functions[RANG_BLOCK_UMAX1 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_UMAX1) != 0) << (RANG_BLOCK_UMAX1 & 0x1f);
    active_functions[RANG_BLOCK_UMAX2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, RANG_SMALL_BLOCK_UMAX2) != 0) << (RANG_BLOCK_UMAX2 & 0x1f);

    //���������� ��� ��
    for (size_t i = 0; i < NUMBER_UP; i++)
    {
      uint32_t rang_small_block_up = RANG_SMALL_BLOCK_UP1 + i;
      uint32_t rang_block_up = RANG_BLOCK_UP1 + 3*i;
      active_functions[rang_block_up >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function, rang_small_block_up) != 0) << (rang_block_up & 0x1f);
    }
  }
  /**************************/

  /**************************/
  //�������� ���������� ���������� ������ ��� ��� �������, �� ����� ������������ ����� �� ���������� �������
  /**************************/
  {
    static unsigned int previous_activating_functions[N_BIG];
    //������� ����� �������, �� ����� ���������, �� ������ ����� � ������ �������� � "0" � "1"
    unsigned int temp_maska_filter_function[N_BIG];
    for(int i=0; i<N_BIG; i++) temp_maska_filter_function[i]=0;
    unsigned int temp_activating_functions[N_BIG];
    for(int i=0; i<N_BIG; i++) temp_activating_functions[i]=0;
  
    //������ "����� ���������"
    _SET_BIT(temp_maska_filter_function, RANG_RESET_LEDS);
  
    //������ "����� ����"
    _SET_BIT(temp_maska_filter_function, RANG_RESET_RELES);

    //������ "�������� ��"
    _SET_BIT(temp_maska_filter_function, RANG_VKL_VV);
    
    //������ "��������� ��"
    _SET_BIT(temp_maska_filter_function, RANG_OTKL_VV);

    //������ "�������� ���������� ��������� �� ��"
    _SET_BIT(temp_maska_filter_function, RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV);
  
    for (unsigned int i = 0; i < N_BIG; i++)
    {
      //� ������ ��������� ����� �������� ����� � �������, ��� ��� �������� ����� ��� � ������� �� � ���������� �����
      temp_activating_functions[i] = previous_activating_functions[i] & temp_maska_filter_function[i];
  
      //� ����������� ����� �������� ����� � �������, � ���� ����������� ��� �������, ��� ����� ������
      temp_activating_functions[i] ^= (active_functions[i] & temp_maska_filter_function[i]);
  
      //����� �������� � ����������� ����� ����� � ������� � ���� ����� �������� ������ ��� "1" (����� ������� ������� � "0" � "1")
      temp_activating_functions[i] &= active_functions[i];

      /*
      ����� ���, �� ������� � �������, �� � ����� ���� ������ ������ �� ������ 
      � ����������� ����, �� ���� ������� ����������� �� ���������� �����
      ������� ��������� ���� ������� (��� ���������� ������) � ���������� �����
      ��� ��� ���������� ����� ���� ��������� ��������� ���� �������� �������
      */ 
      previous_activating_functions[i] = active_functions[i];

      //���������� ����� �������, �� ����� ����������� � ����������� ����, �� ����� �������� ������� ������ ��������� ����� ���� ����� �, � ���� ������� ��� � "0" � "1"
      active_functions[i] = (active_functions[i] & (~temp_maska_filter_function[i])) | temp_activating_functions[i];
    }
  }
  /**************************/
  
  /**************************
  ������� ��������� ����� ��������
  **************************/
  {
    uint32_t state_vv_dv = false;
    for (size_t i = 0; i < NUMBER_INPUTS; i++)
    {
      if (_CHECK_SET_BIT ((current_settings_prt.ranguvannja_inputs + N_SMALL*i), RANG_SMALL_STATE_VV) != 0)
      {
        state_vv_dv = true;
        break;
      }
    }
    
    if (state_vv_dv)
    {
      if (_CHECK_SET_BIT(active_functions, RANG_STATE_VV) !=0) 
      {
        state_leds_ctrl &=  (uint32_t)(~((1 << LED_COLOR_GREEN_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_O)));
        state_leds_ctrl |=  (uint32_t)(  (1 << LED_COLOR_RED_BIT  ) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_I) );
      }
      else
      {
        state_leds_ctrl &=  (uint32_t)(~((1 << LED_COLOR_RED_BIT  ) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_I)));
        state_leds_ctrl |=  (uint32_t)(  (1 << LED_COLOR_GREEN_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_O) );
      }
    }
    else
    {
      state_leds_ctrl &=  (uint32_t)(~(((1 << LED_COLOR_GREEN_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_O)) | ((1 << LED_COLOR_RED_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_I))));
    }
  }
  /**************************/

  /**************************/
  //���� ����� �������
  /**************************/
  for (size_t i = 0; i < NUMBER_GROUP_USTAVOK; i++)
  {
    if (active_inputs_grupa_ustavok & (1 << i)) 
    {
      _CLEAR_BIT(active_functions, RANG_INVERS_DV_GRUPA_USTAVOK);
      break;
    }
    else
    {
      if (i == (NUMBER_GROUP_USTAVOK - 1)) _SET_BIT(active_functions, RANG_INVERS_DV_GRUPA_USTAVOK);
    }
  }
  
  if (count_number_set_bit(&active_inputs_grupa_ustavok, NUMBER_GROUP_USTAVOK) > 1)
    _SET_BIT(set_diagnostyka, ERROR_SELECT_GRUPY_USRAVOK);
  else
    _SET_BIT(clear_diagnostyka, ERROR_SELECT_GRUPY_USRAVOK);

  static unsigned int const maska_signals_for_lock_group[N_BIG] =
  {
    MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_0,
    MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_1,
    MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_2,
    MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_3,
    MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_4,
    MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_5,
    MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_6,
    MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_7
      
#ifdef MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_8
                                          ,
    MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_8
#endif
  };
  unsigned int comp = false;
  COMPARE_NOT_ZERO_OR(comp, active_functions, maska_signals_for_lock_group, N_BIG)
  if (comp)
  {
    //��� ���������� ���� ������� - ����� ������� ���������� ���, ��� ������� ����������
    _SET_BIT(active_functions, RANG_BLK_GRUP_USTAVOK_VID_ZACHYSTIV);

    unsigned int number_group_stp_tmp = 0;
    if(( _CHECK_SET_BIT(active_functions, RANG_1_GRUPA_USTAVOK) ) !=0 ) number_group_stp_tmp++;
    if(( _CHECK_SET_BIT(active_functions, RANG_2_GRUPA_USTAVOK) ) !=0 ) number_group_stp_tmp++;
    if(( _CHECK_SET_BIT(active_functions, RANG_3_GRUPA_USTAVOK) ) !=0 ) number_group_stp_tmp++;
    if(( _CHECK_SET_BIT(active_functions, RANG_4_GRUPA_USTAVOK) ) !=0 ) number_group_stp_tmp++;
    
    if (number_group_stp_tmp != 1)
    {
      //���������� ����� ����� �� ���� � ����
      total_error_sw_fixed();
    }
  }
  else
  {
    //����� �������� ����� ������� - ������� �� �������� �� ��������
    _CLEAR_BIT(active_functions, RANG_BLK_GRUP_USTAVOK_VID_ZACHYSTIV);
    setpoints_selecting(active_functions, active_inputs_grupa_ustavok);
  }

  unsigned int number_group_stp = 0;
  if     (( _CHECK_SET_BIT(active_functions, RANG_1_GRUPA_USTAVOK) ) !=0 ) number_group_stp = 0;
  else if(( _CHECK_SET_BIT(active_functions, RANG_2_GRUPA_USTAVOK) ) !=0 ) number_group_stp = 1;
  else if(( _CHECK_SET_BIT(active_functions, RANG_3_GRUPA_USTAVOK) ) !=0 ) number_group_stp = 2;
  else if(( _CHECK_SET_BIT(active_functions, RANG_4_GRUPA_USTAVOK) ) !=0 ) number_group_stp = 3;
  else
  {
    //���������� ����� ����� �� ���� � ����
    total_error_sw_fixed();
  }
  /**************************/

  /***********************************************************/
  //���������� ���������
  /***********************************************************/
  calc_measurement(number_group_stp);

//  //ĳ��������� ��������� ��� �� �����
//  diagnostyca_adc_execution();
  
  //������� ���������� ��� ����������������� � ����������������� �������
  unsigned int bank_measurement_high_tmp = (bank_measurement_high ^ 0x1) & 0x1;
  if(semaphore_measure_values_low1 == 0)
  {
    for (unsigned int i = 0; i < _NUMBER_IM; i++) 
    {
      measurement_high[bank_measurement_high_tmp][i] = measurement_middle[i] = measurement[i];
    }
    for (unsigned int i = 0; i < MAX_NUMBER_INDEXES_RESISTANCE; i++) resistance_middle[i] = resistance[i];
  }
  else
  {
    for (unsigned int i = 0; i < _NUMBER_IM; i++) 
    {
      measurement_high[bank_measurement_high_tmp][i] = measurement[i];
    }
  }
  bank_measurement_high = bank_measurement_high_tmp;
  /***********************************************************/
  
  
  /**************************/
  //������ "������������ �����"
  /**************************/
  unsigned int diagnostyka_tmp[N_DIAGN];
  for (size_t i = 0; i < N_DIAGN; i ++)
  {
    diagnostyka_tmp[i] = diagnostyka[i];

    diagnostyka_tmp[i] &= (unsigned int)(~clear_diagnostyka[i]); 
    diagnostyka_tmp[i] |= set_diagnostyka[i]; 
  }

  _CLEAR_BIT(diagnostyka_tmp, EVENT_START_SYSTEM_BIT);
  _CLEAR_BIT(diagnostyka_tmp, EVENT_SOFT_RESTART_SYSTEM_BIT);
  _CLEAR_BIT(diagnostyka_tmp, EVENT_DROP_POWER_BIT);
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  _CLEAR_BIT(diagnostyka_tmp, EVENT_RESTART_CB_BIT);
#endif
  unsigned int not_null = false;
  for (size_t i = 0; i < N_DIAGN; i++) 
  {
    not_null |= (diagnostyka_tmp[i] != 0);
    if (not_null) break;
  }
              
  if (not_null)
  {
    _SET_BIT(active_functions, RANG_DEFECT);
    /**************************/
    //������ "������������ ���������"
    /**************************/
    static const unsigned int maska_avar_error[N_DIAGN] = {MASKA_AVAR_ERROR_0, MASKA_AVAR_ERROR_1, MASKA_AVAR_ERROR_2, MASKA_AVAR_ERROR_3};

    not_null = false;
    for (size_t i = 0; i < N_DIAGN; i++) 
    {
      not_null |= ((diagnostyka_tmp[i] & maska_avar_error[i])  != 0);
      if (not_null) break;
    }
    if (not_null)
    {
      _SET_BIT(active_functions, RANG_AVAR_DEFECT);
//       #warning "No Avar Error"
    }
    else
    {
      _CLEAR_BIT(active_functions, RANG_AVAR_DEFECT);
    }
    /**************************/
  }
  else
  {
    _CLEAR_BIT(active_functions, RANG_DEFECT);
    _CLEAR_BIT(active_functions, RANG_AVAR_DEFECT);
  }
  /**************************/

  
  static unsigned int previous_active_functions[N_BIG];
  
  //����� ����� ����� ��������� ����� � ���� �������, ���� ��� ������� "��������� �������������"
  if (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0)
  {
    //������� �������� �� �����������
    
    /**************************/
    //������� ������������� �������
    /**************************/
    tf_handler(previous_active_functions, active_functions);
    /**************************/

    /**************************/
    //�������� ������� ��
    /**************************/
    control_VV(active_functions);
    /**************************/

    /**************************/
    //���
    /**************************/
    if ((current_settings_prt.configuration & (1 << MTZ_BIT_CONFIGURATION)) != 0)
    {
      mtz_handler(active_functions, number_group_stp);
    }
    else
    {
      //������� �������, �� �� ������ ���� � ���� ������������
      static const unsigned int maska_mtz_signals[N_BIG] = 
      {
        MASKA_MTZ_SIGNALS_0,
        MASKA_MTZ_SIGNALS_1,
        MASKA_MTZ_SIGNALS_2, 
        MASKA_MTZ_SIGNALS_3, 
        MASKA_MTZ_SIGNALS_4, 
        MASKA_MTZ_SIGNALS_5,
        MASKA_MTZ_SIGNALS_6, 
        MASKA_MTZ_SIGNALS_7
          
#ifdef MASKA_MTZ_SIGNALS_8
                           ,
        MASKA_MTZ_SIGNALS_8
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_mtz_signals[i]);
      for (int *p = (global_timers + _INDEX_MTZ_BEGIN); p <= (global_timers + _INDEX_MTZ_END); ++p) *p = -1;
    }
    /**************************/
    
    /**************************/
    //���04
    /**************************/
    if ((current_settings_prt.configuration & (1 << MTZ04_BIT_CONFIGURATION)) != 0)
    {
      mtz04_handler(active_functions, number_group_stp);
    }
    else
    {
      //������� �������, �� �� ������ ���� � ���� ������������
      static const unsigned int maska_mtz04_signals[N_BIG] = 
      {
        MASKA_MTZ04_SIGNALS_0, 
        MASKA_MTZ04_SIGNALS_1, 
        MASKA_MTZ04_SIGNALS_2,
        MASKA_MTZ04_SIGNALS_3, 
        MASKA_MTZ04_SIGNALS_4, 
        MASKA_MTZ04_SIGNALS_5, 
        MASKA_MTZ04_SIGNALS_6, 
        MASKA_MTZ04_SIGNALS_7
          
#ifdef MASKA_MTZ04_SIGNALS_8
                             ,
        MASKA_MTZ04_SIGNALS_8
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_mtz04_signals[i]);
      for (int *p = (global_timers + _INDEX_MTZ04_BEGIN); p <= (global_timers + _INDEX_MTZ04_END); ++p) *p = -1;
    }

    /**************************/
    //��
    /**************************/
    if ((current_settings_prt.configuration & (1 << ZZ_BIT_CONFIGURATION)) != 0)
    {
      zz_handler(active_functions, number_group_stp);
    }
    else
    {
      //������� �������, �� �� ������ ���� � ���� ������������
      static const unsigned int maska_nzz_signals[N_BIG] = 
      {
        MASKA_NZZ_SIGNALS_0, 
        MASKA_NZZ_SIGNALS_1, 
        MASKA_NZZ_SIGNALS_2,
        MASKA_NZZ_SIGNALS_3, 
        MASKA_NZZ_SIGNALS_4, 
        MASKA_NZZ_SIGNALS_5, 
        MASKA_NZZ_SIGNALS_6, 
        MASKA_NZZ_SIGNALS_7
          
#ifdef MASKA_NZZ_SIGNALS_8
                           ,
        MASKA_NZZ_SIGNALS_8
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_nzz_signals[i]);
      for (int *p = (global_timers + _INDEX_NZZ_BEGIN); p <= (global_timers + _INDEX_NZZ_END); ++p) *p = -1;
    }
    /**************************/

    /**************************/
    //����
    /**************************/
    if ((current_settings_prt.configuration & (1 << TZNP_BIT_CONFIGURATION)) != 0)
    {
      tznp_handler(active_functions, number_group_stp);
    }
    else
    {
      //������� �������, �� �� ������ ���� � ���� ������������
      static const unsigned int maska_tznp_signals[N_BIG] = 
      {
        MASKA_TZNP_SIGNALS_0, 
        MASKA_TZNP_SIGNALS_1, 
        MASKA_TZNP_SIGNALS_2,
        MASKA_TZNP_SIGNALS_3, 
        MASKA_TZNP_SIGNALS_4, 
        MASKA_TZNP_SIGNALS_5, 
        MASKA_TZNP_SIGNALS_6, 
        MASKA_TZNP_SIGNALS_7
          
#ifdef MASKA_TZNP_SIGNALS_8
                            ,
        MASKA_TZNP_SIGNALS_8
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_tznp_signals[i]);
      for (int *p = (global_timers + _INDEX_SZNP_BEGIN); p <= (global_timers + _INDEX_SZNP_END); ++p) *p = -1;
    }
    /**************************/
  
    /**************************/
    //���(���)
    /**************************/
    if ((current_settings_prt.configuration & (1 << ZOP_BIT_CONFIGURATION)) != 0)
    {
      zop_handler(active_functions, number_group_stp);
    }
    else
    {
      //������� �������, �� �� ������ ���� � ���� ������������
      static const unsigned int maska_zop_signals[N_BIG] = 
      {
        MASKA_ZOP_SIGNALS_0, 
        MASKA_ZOP_SIGNALS_1, 
        MASKA_ZOP_SIGNALS_2,
        MASKA_ZOP_SIGNALS_3, 
        MASKA_ZOP_SIGNALS_4, 
        MASKA_ZOP_SIGNALS_5, 
        MASKA_ZOP_SIGNALS_6, 
        MASKA_ZOP_SIGNALS_7
          
#ifdef MASKA_ZOP_SIGNALS_8
                           ,
        MASKA_ZOP_SIGNALS_8
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_zop_signals[i]);
      for (int *p = (global_timers + _INDEX_ZZP_BEGIN); p <= (global_timers + _INDEX_ZZP_END); ++p) *p = -1;
    }
    /**************************/
    
    if ((current_settings_prt.configuration & (1 << UMIN_BIT_CONFIGURATION)) != 0) 
    {
      /**************************/
      //�����1
      /**************************/
      umin1_handler(active_functions, number_group_stp);
      /**************************/
      
      /**************************/
      //�����2
      /**************************/
      umin2_handler(active_functions, number_group_stp);
      /**************************/
    } 
    else 
    {
      //������� �������, �� �� ������ ���� � ���� ������������
      static const unsigned int maska_umin_signals[N_BIG] = 
      {
        MASKA_UMIN_SIGNALS_0, 
        MASKA_UMIN_SIGNALS_1, 
        MASKA_UMIN_SIGNALS_2,
        MASKA_UMIN_SIGNALS_3, 
        MASKA_UMIN_SIGNALS_4, 
        MASKA_UMIN_SIGNALS_5, 
        MASKA_UMIN_SIGNALS_6, 
        MASKA_UMIN_SIGNALS_7 
          
#ifdef MASKA_UMIN_SIGNALS_8
                            ,
        MASKA_UMIN_SIGNALS_8
#endif
      };
      for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_umin_signals[i]);
      for (int *p = (global_timers + _INDEX_UMIN_BEGIN); p <= (global_timers + _INDEX_UMIN_END); ++p) *p = -1;
    }
    
    if ((current_settings_prt.configuration & (1 << UMAX_BIT_CONFIGURATION)) != 0) 
    {
      /**************************/
      //������1
      /**************************/
      umax1_handler(active_functions, number_group_stp);
      /**************************/
      
      /**************************/
      //������2
      /**************************/
      umax2_handler(active_functions, number_group_stp);
      /**************************/
    } 
    else 
    {
      //������� �������, �� �� ������ ���� � ���� ������������
      static const unsigned int maska_umax_signals[N_BIG] = 
      {
        MASKA_UMAX_SIGNALS_0, 
        MASKA_UMAX_SIGNALS_1, 
        MASKA_UMAX_SIGNALS_2,
        MASKA_UMAX_SIGNALS_3, 
        MASKA_UMAX_SIGNALS_4, 
        MASKA_UMAX_SIGNALS_5, 
        MASKA_UMAX_SIGNALS_6, 
        MASKA_UMAX_SIGNALS_7
          
#ifdef MASKA_UMAX_SIGNALS_8
                            ,
        MASKA_UMAX_SIGNALS_8
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_umax_signals[i]);
      for (int *p = (global_timers + _INDEX_UMAX_BEGIN); p <= (global_timers + _INDEX_UMAX_END); ++p) *p = -1;
    }
    
    if ((current_settings_prt.configuration & (1 << ACHR_CHAPV_BIT_CONFIGURATION)) != 0)
    {
      /**************************/
      //��� ����
      /**************************/
      achr_chapv_handler(active_functions, number_group_stp);
      /**************************/
    } 
    else 
    {
      //������� �������, �� �� ������ ���� � ���� ������������
      static const unsigned int maska_achr_chapv_signals[N_BIG] = 
      {
        MASKA_ACHR_CHAPV_SIGNALS_0, 
        MASKA_ACHR_CHAPV_SIGNALS_1, 
        MASKA_ACHR_CHAPV_SIGNALS_2,
        MASKA_ACHR_CHAPV_SIGNALS_3, 
        MASKA_ACHR_CHAPV_SIGNALS_4, 
        MASKA_ACHR_CHAPV_SIGNALS_5, 
        MASKA_ACHR_CHAPV_SIGNALS_6, 
        MASKA_ACHR_CHAPV_SIGNALS_7
          
#ifdef MASKA_ACHR_CHAPV_SIGNALS_8
                                  ,
        MASKA_ACHR_CHAPV_SIGNALS_8
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_achr_chapv_signals[i]);
      for (int *p = (global_timers + _INDEX_ACHR_CHAPV_BEGIN); p <= (global_timers + _INDEX_ACHR_CHAPV_END); ++p) *p = -1;


      previous_state_po_achr_chapv_uaf1 = 0;
      previous_state_po_achr_chapv_ubf1 = 0;
      previous_state_po_achr_chapv_ucf1 = 0;
      previous_states_CHAPV1 = 0;
      trigger_CHAPV1 = 0;
      previous_states_CHAPV2= 0;
      trigger_CHAPV2= 0;
    }
    
    /**************************/
    //���
    /**************************/
    if ((current_settings_prt.configuration & (1 << ZDZ_BIT_CONFIGURATION)) != 0)
    {
      zdz_handler(active_functions, number_group_stp);
    }
    else
    {
#if   (                                 \
       (MODYFIKACIA_VERSII_PZ == 0) ||  \
       (MODYFIKACIA_VERSII_PZ == 3) ||  \
       (MODYFIKACIA_VERSII_PZ == 4) ||  \
       (MODYFIKACIA_VERSII_PZ == 6) ||  \
       (MODYFIKACIA_VERSII_PZ == 10)||  \
       (MODYFIKACIA_VERSII_PZ == 13)||  \
       (MODYFIKACIA_VERSII_PZ == 14)    \
      )   
      //�������� �������� ����� ���������� ����������
      _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD28_DD30) = (((current_settings_prt.zdz_ovd_porig + 1) & 0xf) << 8) | (0 << 12);
      if (zdz_ovd_diagnostyka)
      {
        if (zdz_ovd_diagnostyka & (1 << 0)) _SET_BIT(clear_diagnostyka, TEST_OVD1);
        if (zdz_ovd_diagnostyka & (1 << 1)) _SET_BIT(clear_diagnostyka, TEST_OVD2);
        if (zdz_ovd_diagnostyka & (1 << 2)) _SET_BIT(clear_diagnostyka, TEST_OVD3);
        
        zdz_ovd_diagnostyka = 0;
      }
      delta_time_test = PERIOD_ZDZ_TEST;
#endif
      
      //������� �������, �� �� ������ ���� � ���� ������������
      static const unsigned int maska_zdz_signals[N_BIG] = 
      {
        MASKA_ZDZ_SIGNALS_0, 
        MASKA_ZDZ_SIGNALS_1, 
        MASKA_ZDZ_SIGNALS_2,
        MASKA_ZDZ_SIGNALS_3, 
        MASKA_ZDZ_SIGNALS_4, 
        MASKA_ZDZ_SIGNALS_5, 
        MASKA_ZDZ_SIGNALS_6, 
        MASKA_ZDZ_SIGNALS_7
          
#ifdef MASKA_ZDZ_SIGNALS_8
                           ,
        MASKA_ZDZ_SIGNALS_8
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_zdz_signals[i]);
      for (int *p = (global_timers + _INDEX_ZDZ_BEGIN); p <= (global_timers + _INDEX_ZDZ_END); ++p) *p = -1;
    }
    /**************************/

    /**************************/
    //����
    /**************************/
    if ((current_settings_prt.configuration & (1 << UROV_BIT_CONFIGURATION)) != 0)
    {
      urov_handler(active_functions, number_group_stp);
    }
    else
    {
      //������� �������, �� �� ������ ���� � ���� ������������
      static const unsigned int maska_urov_signals[N_BIG] = 
      {
        MASKA_UROV_SIGNALS_0, 
        MASKA_UROV_SIGNALS_1, 
        MASKA_UROV_SIGNALS_2,
        MASKA_UROV_SIGNALS_3, 
        MASKA_UROV_SIGNALS_4, 
        MASKA_UROV_SIGNALS_5, 
        MASKA_UROV_SIGNALS_6, 
        MASKA_UROV_SIGNALS_7
          
#ifdef MASKA_UROV_SIGNALS_8
                            ,
        MASKA_UROV_SIGNALS_8
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_urov_signals[i]);
      for (int *p = (global_timers + _INDEX_PRVV_BEGIN); p <= (global_timers + _INDEX_PRVV_END); ++p) *p = -1;
    }
    /**************************/

    /**************************/
    //���
    /**************************/
    if ((current_settings_prt.configuration & (1 << APV_BIT_CONFIGURATION)) != 0)
    {
      apv_handler(active_functions, number_group_stp);
    }
    else
    {
      //������� �������, �� �� ������ ���� � ���� ������������
      static const unsigned int maska_apv_signals[N_BIG] = 
      {
        MASKA_APV_SIGNALS_0, 
        MASKA_APV_SIGNALS_1, 
        MASKA_APV_SIGNALS_2,
        MASKA_APV_SIGNALS_3, 
        MASKA_APV_SIGNALS_4, 
        MASKA_APV_SIGNALS_5, 
        MASKA_APV_SIGNALS_6, 
        MASKA_APV_SIGNALS_7
          
#ifdef MASKA_APV_SIGNALS_8
                           ,
        MASKA_APV_SIGNALS_8
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_apv_signals[i]);
      for (int *p = (global_timers + _INDEX_APV_BEGIN); p <= (global_timers + _INDEX_APV_END); ++p) *p = -1;

      static_logic_APV_0 = 0;
      previous_states_APV_0 = 0;
      trigger_APV_0 = 0;
    }
    /**************************/

    if ((current_settings_prt.configuration & (1 << UP_BIT_CONFIGURATION)) != 0) 
    {
      up_handler(active_functions, number_group_stp);
    } 
    else 
    {
      //������� �������, �� �� ������ ���� � ���� ������������
      static const unsigned int maska_up_signals[N_BIG] = 
      {
        MASKA_UP_SIGNALS_0, 
        MASKA_UP_SIGNALS_1, 
        MASKA_UP_SIGNALS_2,
        MASKA_UP_SIGNALS_3, 
        MASKA_UP_SIGNALS_4, 
        MASKA_UP_SIGNALS_5, 
        MASKA_UP_SIGNALS_6, 
        MASKA_UP_SIGNALS_7
          
#ifdef MASKA_UP_SIGNALS_8
                          ,
        MASKA_UP_SIGNALS_8
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_up_signals[i]);
      for (int *p = (global_timers + _INDEX_UP_BEGIN); p <= (global_timers + _INDEX_UP_END); ++p) *p = -1;
    }
    
    /**************************/
    //��������� �����
    /**************************/
    if ((current_settings_prt.configuration & (1 << EL_BIT_CONFIGURATION)) != 0)
    {
      unsigned int active_functions_tmp[NUMBER_ITERATION_EL_MAX][N_BIG];
      unsigned int iteration = 0;
      unsigned int repeat_state = false;
      while (iteration < current_settings_prt.number_iteration_el)
      {
        if (iteration > 0)
        {
          //�������� �� ������ ��������� ����� � ������������
          COMPARE_AND(comp, active_functions_tmp[iteration - 1], active_functions, N_BIG)
          if (comp) break;
          
          //�����, �� ��� ��� �������� ���� ������������ � ��������� ���������
          for (int i = (iteration - 2); ((repeat_state == false) && (i >= 0)); --i)
          {
            COMPARE_AND(repeat_state, active_functions_tmp[i], active_functions, N_BIG)
          }            
        }
        if (repeat_state) break;
        
        for (size_t i = 0; i != N_BIG; ++i) active_functions_tmp[iteration][i] = active_functions[i];

        d_and_handler(active_functions);
        d_or_handler(active_functions);
        d_xor_handler(active_functions);
        d_not_handler(active_functions);
        df_handler(active_functions);
        dt_handler(active_functions);
        
        ++iteration;
      }
      
      if (
          (repeat_state) ||
          (iteration >= current_settings_prt.number_iteration_el)
         )
      {
        _SET_BIT(active_functions, RANG_ERROR_CONF_EL);
      }
      else
      {
        _CLEAR_BIT(active_functions, RANG_ERROR_CONF_EL);
      }
    }
    else
    {
      //������� �������, �� �� ������ ���� � ���� ������������
      static const unsigned int maska_el_signals[N_BIG] = 
      {
        MASKA_EL_SIGNALS_0, 
        MASKA_EL_SIGNALS_1, 
        MASKA_EL_SIGNALS_2,
        MASKA_EL_SIGNALS_3, 
        MASKA_EL_SIGNALS_4, 
        MASKA_EL_SIGNALS_5, 
        MASKA_EL_SIGNALS_6, 
        MASKA_EL_SIGNALS_7
          
#ifdef MASKA_EL_SIGNALS_8                          
                          ,
        MASKA_EL_SIGNALS_8
#endif
      };
      for (size_t i = 0; i < N_BIG; ++i) active_functions[i] &= (unsigned int)(~maska_el_signals[i]);
      
      //������� �� �������, �� ���������� �� ��������� �����
      for(int *p = (global_timers + INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START); p <= (global_timers + INDEX_TIMER_DF_WORK_START + NUMBER_DEFINED_FUNCTIONS - 1); ++p) *p = -1;
      
      static_logic_df = 0;
    }
    /**************************/

    /**************************/
    //���������-³���������
    /**************************/
    on_off_handler(active_functions);
    /**************************/
    
    /**************************/
    //��� (������ ����� ����� ����� ���������)
    /**************************/
    vmp_handler(active_functions);
    /**************************/

    /**************************/
    //���������� � ��
    /**************************/
    ready_tu(active_functions);
    /**************************/

    /**************************/
    //������ �������� (������ ����� ����� ����� ���������)
    /**************************/
    resurs_vymykacha_handler(active_functions);
    /**************************/
  }
  else
  {
    //������� �������� �����������
    
    //������� �� ������ �������, ��� �������������
    static unsigned int const maska_info_signals[N_BIG] = 
    {
      MASKA_INFO_SIGNALES_0,
      MASKA_INFO_SIGNALES_1,
      MASKA_INFO_SIGNALES_2,
      MASKA_INFO_SIGNALES_3,
      MASKA_INFO_SIGNALES_4,
      MASKA_INFO_SIGNALES_5,
      MASKA_INFO_SIGNALES_6,
      MASKA_INFO_SIGNALES_7
        
#ifdef MASKA_INFO_SIGNALES_8
                           ,
      MASKA_INFO_SIGNALES_8
#endif
    };
    
    for (size_t i = 0; i != N_BIG; ++i) active_functions[i] &= maska_info_signals[i];

    //������� �� �������, �� ������� � �����
    for (int *p = (global_timers + _INDEX_TIMER_LOGIC_BEGIN); p != (global_timers + _MAX_NUMBER_GLOBAL_TIMERS); ++p) *p = -1;
    
    //������������ �� ����
    state_outputs = 0;
#ifdef NUMBER_DS
    ds = 0;
#endif
    
    //���������� � ���������� ���� ���� �������� ����
    static_logic_APV_0 = 0;
    previous_states_APV_0 = 0;
    trigger_APV_0 = 0;
    previous_state_po_achr_chapv_uaf1 = 0;
    previous_state_po_achr_chapv_ubf1 = 0;
    previous_state_po_achr_chapv_ucf1 = 0;
    previous_states_CHAPV1 = 0;
    trigger_CHAPV1 = 0;
    previous_states_ready_tu = 0;
    trigger_ready_tu = 0;
    

    static_logic_df = 0;
  }

  /**************************/
  //������� ����������� ����������
  /**************************/
  analog_registrator(active_functions);
  /**************************/

  /**************************/
  //������� ����������� ����������
  /**************************/
  digital_registrator(active_functions);
  /**************************/

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  /***
  ���������� ����� ��������� ���������� �����
  ***/

//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//             LAN BLOCK     
//....................................................................................................
//=====================================================================================================
do{
    sLV.ch_while_breaker = 0;//! optimize then
    
    register void* pvl;
    
     register unsigned int *r_p_active_functions;
// ----------------    -------------------------
    r_p_active_functions = sLV.p_active_functions;
    pvl = (void*)&hldLanBlkParam;//hldMmsBlkParam;
    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_OUT_LAN1 + ((unsigned int)LAN_BLOCK_ORD_NUM_00))) != 0){
    //clr block
        Output_Out_LAN_block[LAN_BLOCK_ORD_NUM_00] = 0;
        //
    }else{
        proc_Lan_blk_out(&current_settings_prt.ranguvannja_Out_LAN[0][0][0],r_p_active_functions,(void*)&(((GsBlkParamDsc*)pvl)-> unnV1[LAN_BLOCK_ORD_NUM_00].ulV));
    }

    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_OUT_LAN1+((unsigned int)LAN_BLOCK_ORD_NUM_01))) != 0){
    //clr block
        Output_Out_LAN_block[LAN_BLOCK_ORD_NUM_01] = 0; 
    }else{
        proc_Lan_blk_out(&current_settings_prt.ranguvannja_Out_LAN[0][0][0],r_p_active_functions,(void*)&(((GsBlkParamDsc*)pvl)-> unnV1[LAN_BLOCK_ORD_NUM_01].ulV));
    }

    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_OUT_LAN1+((unsigned int)LAN_BLOCK_ORD_NUM_02))) != 0){
    //clr block
        Output_Out_LAN_block[LAN_BLOCK_ORD_NUM_02] = 0;
    }else{
        proc_Lan_blk_out(&current_settings_prt.ranguvannja_Out_LAN[0][0][0],r_p_active_functions,(void*)&(((GsBlkParamDsc*)pvl)-> unnV1[LAN_BLOCK_ORD_NUM_02].ulV));
    }

    if(_CHECK_SET_BIT(((unsigned int*)r_p_active_functions), (RANG_BLOCK_OUT_LAN1+((unsigned int)LAN_BLOCK_ORD_NUM_03))) != 0){
    //clr block
        Output_Out_LAN_block[LAN_BLOCK_ORD_NUM_03] = 0;
    }else{
        proc_Lan_blk_out(&current_settings_prt.ranguvannja_Out_LAN[0][0][0],r_p_active_functions,(void*)&(((GsBlkParamDsc*)pvl)-> unnV1[LAN_BLOCK_ORD_NUM_03].ulV));
    }





   
}while(sLV.ch_while_breaker);
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````

  


  

  /***/
#endif


  /**************************/
  //������ � ���������, �� ����� ������������ � ��������������� ���'���
  /**************************/
  static const unsigned int maska_trg_func_array[N_BIG] = 
  {
    MASKA_TRIGGER_SIGNALES_0,
    MASKA_TRIGGER_SIGNALES_1,
    MASKA_TRIGGER_SIGNALES_2,
    MASKA_TRIGGER_SIGNALES_3,
    MASKA_TRIGGER_SIGNALES_4,
    MASKA_TRIGGER_SIGNALES_5,
    MASKA_TRIGGER_SIGNALES_6,
    MASKA_TRIGGER_SIGNALES_7
      
#ifdef MASKA_TRIGGER_SIGNALES_8
                            ,
    MASKA_TRIGGER_SIGNALES_8
#endif
  };
  comp = true;
  for (size_t i = 0; i != N_BIG; ++i)
  {
    unsigned int tmp_data = active_functions[i] & maska_trg_func_array[i];
    if (trigger_active_functions[i] != tmp_data)
    {
      comp = false;
      trigger_active_functions[i] = tmp_data;
    }
  }
  if (comp != true)
  {
    /*
    �������, �������� ���� ���������� � ��������������� ���'1��� ��������.
    ������ ������� �� �� ����� � ��������������� ���'���
    */
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT);
  }
  /**************************/

  /**************************/
  //����������� ��������� � ������ �������� � ��������� �������
  /**************************/
  for (size_t i = 0; i != N_BIG; ++i)
  {
    unsigned int temp_data = active_functions[i];
    trigger_functions_USB[i]   |= temp_data;
    trigger_functions_RS485[i] |= temp_data;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    trigger_functions_LAN[i]   |= temp_data;
#endif
  }

  copying_active_functions = 0; //�������, �� ���������� �������� �������� ������� ���������
  
  /*
  ������ ���� �������� �������� ������� ��� ����, ��� ���� �� ������� ������
  �����������, �� ����� ���� � ����� �������  (����� � ��'������� ����������
  ���������) ����� ��������, ��� ��������� ��������
  */
  for (unsigned int i = 0; i < N_BIG; i++) 
  {
    unsigned int temp_data = active_functions[i];
    active_functions_copy[i] = temp_data;
    previous_active_functions[i] = temp_data;
  }
  /**************************/

  /**************************/
  //���� ���������� �� ������
  /**************************/
  //�������� ����������, �� �� �������������� ������� "����� ����" - � ���� ���, �� ���������� ������� �� ����
  if (_CHECK_SET_BIT(active_functions, RANG_RESET_RELES) !=0)
  {
    state_outputs = 0;
  }
  
  if (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0)
  {
    //�� ����������� ������� ��������, ���� ������������� ���� �����
    
    //���������, �� ���� ����� ����� ���� ����������
    for (unsigned int i = 0; i < NUMBER_SIMPLE_OUTPUTS; i++)
    {
      //� ���������� ����� ������� ��ò��� � ���������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
      unsigned int temp_array_of_outputs[N_BIG];
    
      for (unsigned int j = 0; j < N_BIG; j++) temp_array_of_outputs[j] = current_settings_prt.ranguvannja_outputs[N_BIG*i + j] & active_functions[j];

      //������ "������� �����������" ������ � ���������� �����: ������ ���� �� ����� ���������� � �������, ���� ����� ������ �� ��������
      if(_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_AVAR_DEFECT) !=0)
      {
        //������ "A�������� �������������"  ������ ���������� �� ����� �����
        if (_CHECK_SET_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT) == 0)
        {
          //������ "A�������� �������������" �� � ��������
          //���������� ������������ ���� � �������� ���� � �����, ���� �  ��ò���� � ��������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
          _SET_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT);
        }
        else
        {
          //������ "A�������� �������������" � ��������
          //���������� ���������� ���� � �������� ���� � �����, ���� �  ��ò���� � ��������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
          _CLEAR_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT);
        }
      }
      
      //������ "�������� �����������" ������ � ���������� �����: ������ ���� �� ����� ���������� � �������, ���� ����� ������ �� ��������
      if(_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_DEFECT) !=0)
      {
        //������ "�������� �����������"  ������ ���������� �� ����� �����
        if (_CHECK_SET_BIT(temp_array_of_outputs, RANG_DEFECT) ==0)
        {
          //������ "�������� �����������" �� � ��������
          //���������� ������������ ���� � �������� ���� � �����, ���� �  ��ò���� � ��������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
          _SET_BIT(temp_array_of_outputs, RANG_DEFECT);
        }
        else
        {
          //������ "�������� �����������" � ��������
          //���������� ���������� ���� � �������� ���� � �����, ���� �  ��ò���� � ��������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
          _CLEAR_BIT(temp_array_of_outputs, RANG_DEFECT);
        }
      }

      //����������, �� � ��������� �� ����������� ��������� �� ����� ����� � ���������, �� ����� � ��������� - ����� ��������� ������
      flag=0;
      for(int m=0; m<N_BIG; m++) if(temp_array_of_outputs[m] != 0) {flag=1; break;}
      if (flag)
      {
        //��� ����������� ���� �������� ���� ���������, � ��� ���������� ���������� �� ���� ������ ���������� ���� ��� ���� �� �� ����� �������� ���� ���������, ������� �� ����������
        if ((current_settings_prt.type_of_output & (1 << i)) != 0)
        {
          //����� ����������, ���� � ���� ����� ��� �������� ����
          //³������, �� ����� ����� - ���������
          state_outputs |= (1 << i);
        }
        else
        {
          //����� ���������, ���� ���������� �� �� ��� ������ ���������� ����, �� ��� �������� ��, ������� ���� �� � ������� ������ ���������� (����������)
          if (_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_WORK_BV) ==0)
          {
            //�� ���� ���� �� ���������� ������ �� (���� ���������)
          
            //³������, �� ����� ����� - ���������
            state_outputs |= (1 << i);
          }
          else
          {
            //�� ���� ���� ���������� ������ �� (���� ���������)
          
            //³������, �� ����� ����� - ��������� ����� ���, ���� ������� �� ������� �����
            if (_CHECK_SET_BIT(active_functions, RANG_WORK_BV) !=0)
              state_outputs |= (1 << i);
            else
              state_outputs &= ~(1 << i);
          }
        }
      }
      else
      {
        //����������, �� ����� ���������, �� ����������
        if ((current_settings_prt.type_of_output & (1 << i)) == 0)
        {
          //����� ���������
        
          //³������, �� ����� ����� - ��ǲ�������
          state_outputs &= ~(1 << i);
        }
      }
    }

#ifdef NUMBER_DS
    if ((current_settings_prt.configuration & (1 << DS_BIT_CONFIGURATION)) != 0)
    {
      for (size_t i = NUMBER_SIMPLE_OUTPUTS; i != NUMBER_OUTPUTS; ++i)
      {
        //� ���������� ����� ������� ��ò��� � ���������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
        unsigned int temp_array_of_outputs[N_BIG];
    
        for (unsigned int j = 0; j < N_BIG; j++) temp_array_of_outputs[j] = current_settings_prt.ranguvannja_outputs[N_BIG*i + j] & active_functions[j];

        //������ "������� �����������" ������ � ���������� �����: ������ ���� �� ����� ���������� � �������, ���� ����� ������ �� ��������
        if(_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_AVAR_DEFECT) !=0)
        {
          //������ "A�������� �������������"  ������ ���������� �� ����� �����
          if (_CHECK_SET_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT) == 0)
          {
            //������ "A�������� �������������" �� � ��������
            //���������� ������������ ���� � �������� ���� � �����, ���� �  ��ò���� � ��������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
            _SET_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT);
          }
          else
          {
            //������ "A�������� �������������" � ��������
            //���������� ���������� ���� � �������� ���� � �����, ���� �  ��ò���� � ��������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
            _CLEAR_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT);
          }
        }
      
        //������ "�������� �����������" ������ � ���������� �����: ������ ���� �� ����� ���������� � �������, ���� ����� ������ �� ��������
        if(_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_DEFECT) !=0)
        {
          //������ "�������� �����������"  ������ ���������� �� ����� �����
          if (_CHECK_SET_BIT(temp_array_of_outputs, RANG_DEFECT) ==0)
          {
            //������ "�������� �����������" �� � ��������
            //���������� ������������ ���� � �������� ���� � �����, ���� �  ��ò���� � ��������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
            _SET_BIT(temp_array_of_outputs, RANG_DEFECT);
          }
          else
          {
            //������ "�������� �����������" � ��������
            //���������� ���������� ���� � �������� ���� � �����, ���� �  ��ò���� � ��������� ������, ���� ����������� ��������� "i" � �������, �� ����� � ���������
            _CLEAR_BIT(temp_array_of_outputs, RANG_DEFECT);
          }
        }

        //����������, �� � ��������� �� ����������� ��������� �� ����� ����� � ���������, �� ����� � ��������� - ����� ��������� ������
        NOT_ZERO_OR(comp, temp_array_of_outputs, N_BIG)
        if (comp)
        {
          if (_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_WORK_BV) == 0)
          {
            //�� ���� ���� �� ���������� ������ �� (���� ���������)
          
            //³������, �� ����� ����� - ���������
            _SET_STATE(ds, (i - NUMBER_SIMPLE_OUTPUTS));
          }
          else
          {
            //�� ���� ���� ���������� ������ �� (���� ���������)
          
            //³������, �� ����� ����� - ��������� ����� ���, ���� ������� �� ������� �����
            if (_CHECK_SET_BIT(active_functions, RANG_WORK_BV) != 0)
              _SET_STATE(ds, (i - NUMBER_SIMPLE_OUTPUTS));
            else
              _CLEAR_STATE(ds, (i - NUMBER_SIMPLE_OUTPUTS));
          }
        }
        else
        {
          //³������, �� ����� ����� - ��ǲ�������
          _CLEAR_STATE(ds, (i - NUMBER_SIMPLE_OUTPUTS));
        }
      }
    }
    else ds = 0;
#endif    
  }
  else
  {
    //����������� ������e ��������, ���� ���������� �� ����!!!

    //������������ �� ����
    state_outputs = 0;
#ifdef NUMBER_DS    
    ds = 0;
#endif
  }
  
  //���������� �� ����� ���������� ���� ���������� ������ � EEPROM
  if((state_outputs  & current_settings_prt.type_of_output) != state_signal_outputs)
  {
    state_signal_outputs = state_outputs  & current_settings_prt.type_of_output;
    //����������� ����������� ��� ��, �� � EEPROM ����� �������� ��� �������� ���������� ������ � ��������� ��������������
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_STATE_LEDS_OUTPUTS_EEPROM_BIT);
  }
  
  //���� ������ � ����������� ����������� ������ ������ ���������� ������
  unsigned int output_signal_modif = (current_settings_prt.type_of_output_modif & current_settings_prt.type_of_output);
  state_outputs_raw = ( state_outputs & ((unsigned int)(~output_signal_modif)) ) | ((state_outputs & output_signal_modif)*output_timer_prt_signal_output_mode_2);
  
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37) = state_outputs_raw;
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 5) ||    \
     (MODYFIKACIA_VERSII_PZ == 15)      \
    )  

  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD28_DD30) = (state_outputs_raw >> 16)<< 8;

#else

#ifdef NUMBER_DS
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD28_DD30) = (ds & (MASKA_FOR_BIT(NUMBER_DS) - 1)) << 8;
#endif
  
#endif

  TIM_PRT_write_tick = TIM2->CNT;
  /**************************/

  /**************************/
  //���� ���������� �� ���������
  /**************************/
  //�������� ����������, �� �� �������������� ������� "����� ���������" - � ���� ���, �� ���������� ������� ��� ���������
  if (_CHECK_SET_BIT(active_functions, RANG_RESET_LEDS) !=0)
  {
    state_leds = 0;
  }
  
  //���������, �� �������������� ����� ����� ���� ���������
  for (unsigned int i = 0; i < NUMBER_LEDS; i++)
  {
    //� ���������� ����� ������� ���������� ��������������, ���� ����������� ��������� "i"
    unsigned int temp_array_of_leds[N_BIG];
    
    //����������, �� � ��������� �� ����������� ��������� �� ����� ������������� � ���������, �� ����� � ��������� - ����� ��������� ������
    for (unsigned int j = 0; j < N_BIG; j++ ) temp_array_of_leds[j] = current_settings_prt.ranguvannja_leds[N_BIG*i + j] & active_functions[j];

    flag=0;
    for(int m=0; m<N_BIG; m++) if(temp_array_of_leds[m] != 0) {flag=1; break;}
    if (flag)
    {
      //³������, �� ����� ������������� - ������
      state_leds |= (1 << i);
    }
    else
    {
      //����������, �� ����� ������������� ����������, �� ���������
      if ((current_settings_prt.type_of_led & (1 << i)) == 0)
      {
        //������������� ����������

        //³������, �� ����� ������������� - ���������
        state_leds &= ~(1 << i);
      }
    }
  }
  //���������� �� ����� ���������� ���� ��������� �������������� � EEPROM
  if((state_leds  & current_settings_prt.type_of_led) != state_trigger_leds)
  {
    state_trigger_leds = state_leds  & current_settings_prt.type_of_led;
    //����������� ����������� ��� ��, �� � EEPROM ����� �������� ��� �������� ���������� ������ � ��������� ��������������
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_STATE_LEDS_OUTPUTS_EEPROM_BIT);
  }
  
  //������ ��������������
  /*Run_Error*/
  if (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0) state_leds_ctrl |=  (1 << LED_COLOR_GREEN_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_R_E);
  else state_leds_ctrl &=  (uint32_t)(~((1 << LED_COLOR_GREEN_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_R_E)));
  if (
      (_CHECK_SET_BIT(active_functions, RANG_DEFECT     ) != 0) ||
      (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) != 0)
     ) state_leds_ctrl |=  (1 << LED_COLOR_RED_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_R_E);
  else state_leds_ctrl &=  (uint32_t)(~((1 << LED_COLOR_RED_BIT) << ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_R_E)));
  
  static uint32_t state_leds_lock[2];
  static uint32_t state_leds_ctrl_lock[2];
  static uint32_t state_leds_Fx_lock[2][2];
  static size_t bank_lock;
  
  size_t bank_lock_tmp1 = bank_lock;
  size_t bank_lock_tmp2 = (bank_lock_tmp1 + 1) & 0x1;
  
  state_leds_lock[bank_lock_tmp2] |= state_leds;
  state_leds_ctrl_lock[bank_lock_tmp2] |= state_leds_ctrl;
  state_leds_Fx_lock[bank_lock_tmp2][0] |= state_leds_Fx[0];
  state_leds_Fx_lock[bank_lock_tmp2][1] |= state_leds_Fx[1];

  static uint32_t current_LED_N_COL;
  
  //������� ��������� ����������
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD32_DD38) = ((1 << current_LED_N_COL) << LED_N_ROW) | ((uint32_t)(~0) & ((1 << LED_N_ROW) - 1));
  
  uint32_t state_leds_tmp = 0;
  
  switch (current_LED_N_COL)
  {
  case 0:
    {
      uint32_t state_leds_auto = state_leds_lock[bank_lock_tmp1];
      
      state_leds_tmp = (((state_leds_auto >>  0) & 0x1) << 0) |
                       (((state_leds_auto >>  2) & 0x1) << 1) |
                       (((state_leds_auto >>  4) & 0x1) << 2) |
                       (((state_leds_auto >>  6) & 0x1) << 3) |
                       (((state_leds_auto >>  8) & 0x1) << 4) |
                       (((state_leds_auto >> 10) & 0x1) << 5) |
                       (((state_leds_auto >> 12) & 0x1) << 6) |
                       (((state_leds_auto >> 14) & 0x1) << 7);
      break;
    }
  case 1:
    {
      uint32_t state_leds_auto = state_leds_lock[bank_lock_tmp1];
      uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];
      
      state_leds_tmp = (((state_leds_auto >>  1) & 0x1) << 0) |
                       (((state_leds_auto >>  3) & 0x1) << 1) |
                       (((state_leds_auto >>  5) & 0x1) << 2) |
                       (((state_leds_auto >>  7) & 0x1) << 3) |
                       (((state_leds_auto >>  9) & 0x1) << 4) |
                       (((state_leds_auto >> 11) & 0x1) << 5) |
                       (((state_leds_auto >> 13) & 0x1) << 6) |
                       ((((state_leds_ctrl_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_O)) & (1 << LED_COLOR_RED_BIT)) != 0) << 7);
      break;
    }
  case 2:
    {
      uint32_t state_leds_auto = state_leds_lock[bank_lock_tmp1];
      uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];
      uint32_t state_leds_Fx0_auto = state_leds_Fx_lock[bank_lock_tmp1][0];

      state_leds_tmp = ((((state_leds_ctrl_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_R_E  )) & (1 << LED_COLOR_RED_BIT)) != 0) << 0) |

                       (((state_leds_auto >> 15) & 0x1) << 1) |
                       (((state_leds_auto >> 16) & 0x1) << 2) |
      
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(1 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 3) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(2 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 4) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(3 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 5) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(4 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 6) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(5 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 7);
      break;
    }
  case 3:
    {
      uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];
      uint32_t state_leds_Fx0_auto = state_leds_Fx_lock[bank_lock_tmp1][0];

      state_leds_tmp = ((((state_leds_ctrl_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_R_E  )) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 0) |

                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(1 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 3) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(2 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 4) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(3 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 5) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(4 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 6) | 
                       ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(5 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 7);

      break;
    }
  case 4:
    {
      uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];
      uint32_t state_leds_Fx0_auto = state_leds_Fx_lock[bank_lock_tmp1][0];
      uint32_t state_leds_Fx1_auto = state_leds_Fx_lock[bank_lock_tmp1][1];

      state_leds_tmp = ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(6 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 0) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(1 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 1) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(2 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 2) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(3 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 3) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(4 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 4) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(5 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 5) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(6 - 1))) & (1 << LED_COLOR_RED_BIT)) != 0) << 6) |
                       ((((state_leds_ctrl_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_I)) & (1 << LED_COLOR_RED_BIT)) != 0) << 7);
      break;
    }
  case 5:
    {
      uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];
      uint32_t state_leds_Fx0_auto = state_leds_Fx_lock[bank_lock_tmp1][0];
      uint32_t state_leds_Fx1_auto = state_leds_Fx_lock[bank_lock_tmp1][1];

      state_leds_tmp = ((((state_leds_Fx0_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(6 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 0) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(1 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 1) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(2 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 2) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(3 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 3) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(4 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 4) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(5 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 5) |
                       ((((state_leds_Fx1_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)(6 - 1))) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 6) |
                       ((((state_leds_ctrl_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_I)) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 7);
      break;
    }
  case 6:
    {
      uint32_t state_leds_ctrl_auto = state_leds_ctrl_lock[bank_lock_tmp1];

      state_leds_tmp = ((((state_leds_ctrl_auto >> ((uint32_t)NUMBER_LED_COLOR*(uint32_t)LED_CTRL_O)) & (1 << LED_COLOR_GREEN_BIT)) != 0) << 7);
      break;
    }
  default:
    {
      //���������� ����� ����� �� ���� � ����
      total_error_sw_fixed();
    }
  }

  //�������� ���������� �� ��������������� �� ���������
  _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD32_DD38) = ((1 << current_LED_N_COL) << LED_N_ROW) | ((uint32_t)(~state_leds_tmp) & ((1 << LED_N_ROW) - 1));

  //���������� �� ��������� ��������
  if (++current_LED_N_COL >= LED_N_COL) 
  {
    current_LED_N_COL = 0;
    bank_lock = bank_lock_tmp2;

    state_leds_lock[bank_lock_tmp1] = 0;
    state_leds_ctrl_lock[bank_lock_tmp1] = 0;
    state_leds_Fx_lock[bank_lock_tmp1][0] = 0;
    state_leds_Fx_lock[bank_lock_tmp1][1] = 0;
  }
  /**************************/

  /**************************/
  //
  /**************************/
  /**************************/
}
/*****************************************************/

/*****************************************************/
//����������� �� ������� TIM2, ���� ��������� ������� �������
/*****************************************************/
void TIM2_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif
  
  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
  {
    /***********************************************************************************************/
    //����������� �������� �� ������ 1, ���� ������ ����������� ���� 1 ��, ��� ��������������� ������� � ������ �������
    /***********************************************************************************************/
    TIM2->SR = (uint16_t)((~(uint32_t)TIM_IT_CC1) & 0xffff);
    uint32_t current_tick = TIM2->CCR1;
    
    /*************************************
    ��������� ���� � UNIX-������
    *************************************/
    ++clk_count;

    if ( ++time_ms >= 1000) 
    {
      time_ms = 0;
      time_dat++;
    }
      
    if ((save_time_dat_h == 3) || (save_time_dat_l == 3))
    {
      //������ ������ ������ ����
      if (save_time_dat_l == 3)
      {
        time_ms = time_ms_save_l;
        time_dat = time_dat_save_l;
        
        save_time_dat_l = 2;
      }
      if (save_time_dat_h == 3)
      {
        time_ms = time_ms_save_h;
        time_dat = time_dat_save_h;
        
        save_time_dat_h = 2;
      }
      
      if (copying_time_to_RTC == 1) copying_time_to_RTC = 0;
      realDateTime = true;
    }
    else 
    {
      //�������� �� �� ������� ������� ��� � RTC
      if (copying_time_to_RTC == 1)
      {
        int32_t diff_ms = time_ms - time_ms_RTC;
        time_t diff_s = time_dat - time_dat_RTC;
        if (diff_ms < 0) 
        {
          diff_ms += 1000;
          --diff_s;
        }
        if (llabs(diff_s*1000 + diff_ms) > 2000)
        {
          time_ms = time_ms_RTC;
          time_dat = time_dat_RTC;
        }
        copying_time_to_RTC = 0;
        realDateTime = true;
      }
    }
    
    
    if (!copying_time_dat)
    {
      time_ms_copy = time_ms;
      time_dat_copy = time_dat;
    }
    /*************************************/

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    /***********************************************************/
    //������ ���������� � ������������ �����
    /***********************************************************/
    if (restart_KP_irq == 0)
    {
      //���������� �� ������� ��� �� ������ CANAL1_MO � ������������ �����
      GPIO_CANAL1_MO_Out1->BSRRL = GPIO_PIN_CANAL1_MO_Out1; //���������� �� canal1_Out1 � ���� "1"
    }
    else
    {
      _CLEAR_STATE(queue_mo, STATE_QUEUE_MO_RESTART_KP);
      
      if (--restart_KP_irq != 0) GPIO_KP_SOFT_RESET->BSRRL = GPIO_PIN_KP_SOFT_RESET; //������ ������� �� ���������� ����������� �����
      else 
      {
        _SET_BIT(clear_diagnostyka, EVENT_RESTART_CB_BIT);
        GPIO_KP_SOFT_RESET->BSRRH = GPIO_PIN_KP_SOFT_RESET; //������ ������� �� ���������� ����������� �����
        
        queue_mo_irq = 0;
        IEC_board_uncall = 200;
      }
    }
    start_receive_data_via_CANAL1_MO();
    Canal1 = true;
    GPIO_CANAL1_MO_Out1->BSRRH = GPIO_PIN_CANAL1_MO_Out1; //���������� �� canal1_Out1 � ���� "0"
    /***********************************************************/
#endif

    /***********************************************************/
    //����������, �� ���������� ���� ���������
    /***********************************************************/
    if (changed_settings == CHANGED_ETAP_ENDED) /*�� � �����, �� ��� ��� ���������� ��� �������� �� � ������ �������� ������� (� ��� ����� ����� ��� �� ���������)*/
    {
      //������� ������� ��������� � ���� ���� ������� ��� � ���� ������ (���� � �����) ����� ������� �������
      current_settings_prt = current_settings;
      
      //�������, �� ���� ������� �������� �������
      changed_settings = CHANGED_ETAP_NONE;
    }

    if (koef_resurs_changed == CHANGED_ETAP_ENDED)
    {
      //����������� ��� ��������� ������� ��������
      K_resurs_prt = K_resurs;

      //�������, �� ���� ������� �������� �������
      koef_resurs_changed = CHANGED_ETAP_NONE;
    }
    /***********************************************************/

    /***********************************************************/
    //���������� ����������� ������� ����������� ����������
    /***********************************************************/
    if (
        ((clean_rejestrators & CLEAN_DR) != 0)
        &&  
        (
         (control_tasks_dataflash & (
                                     TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR | 
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB                    |
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485                  |
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_LAN                    |
#endif  
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU
                                    )
         ) == 0
        )
       )   
    {
      //���������� ������� �������� ����������� ���������

      //����������� ������� ������ ���� ��������� � EEPROM
      _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);

      //������� ��������� ���������� �� ����������� ���������
      info_rejestrator_dr.next_address = MIN_ADDRESS_DR_AREA;
      info_rejestrator_dr.saving_execution = 0;
      info_rejestrator_dr.number_records = 0;

      //�������, �� ����� ������ �� ��������
      number_record_of_dr_for_menu  = 0xffff;
      number_record_of_dr_for_USB   = 0xffff;
      number_record_of_dr_for_RS485 = 0xffff;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      number_record_of_dr_for_LAN = 0xffff;
#endif  

      //������ ������� ������� ����������� ����������
      clean_rejestrators &= (unsigned int)(~CLEAN_DR);
    }
    /***********************************************************/

    /***********************************************************/
    //���������� ������� �������� � ���������� ����� ����� ���� ��������� ������ ��������
    /***********************************************************/
    clocking_global_timers();
    /***********************************************************/
    
    /***********************************************************/
    //���������� ������� �������
    /***********************************************************/
    //ĳ��������� �����, ��� ����� ��������� ����� ��� ����� �������� ��������������� ����� ��������
    uint32_t TIM_PRT_read_tick = TIM2->CNT;

    uint64_t TIM_PRT_delta_write_read;
    if (TIM_PRT_read_tick < TIM_PRT_write_tick)
      TIM_PRT_delta_write_read = TIM_PRT_read_tick + 0x100000000 - TIM_PRT_write_tick;
    else TIM_PRT_delta_write_read = TIM_PRT_read_tick - TIM_PRT_write_tick;
    if (TIM_PRT_delta_write_read > (TIM2_MIN_PERIOD_WRITE_READ + 1))
    {
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 5) ||    \
     (MODYFIKACIA_VERSII_PZ == 15)      \
    )                                   
      unsigned int control_state_outputs =    ((~((unsigned int)(_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37)     ))) & 0xffff)
                                           | (((~((unsigned int)(_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD28_DD30          ) >> 8))) & 0xf   ) << 16);
#else
      unsigned int control_state_outputs = ((~((unsigned int)(_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD31_DD34_DD35_DD37)))) & 0xffff);

#ifdef NUMBER_DS
      unsigned int control_ds =  (~((unsigned int)(_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD28_DD30) >> 8))) & (MASKA_FOR_BIT(NUMBER_DS) - 1);
#endif
      
#endif
     

      static uint32_t error_rele[NUMBER_OUTPUTS];
      if (control_state_outputs != state_outputs_raw)
      {
        for (size_t index = 0; index < NUMBER_SIMPLE_OUTPUTS; ++index)
        {
          uint32_t maska = 1 << index;
        
          if ((control_state_outputs & maska) != (state_outputs_raw & maska))
          {
            if (error_rele[index] < 3) ++error_rele[index];
            if (error_rele[index] >= 3 ) _SET_BIT(set_diagnostyka, (ERROR_DIGITAL_OUTPUT_1_BIT + index));
          }
          else error_rele[index] = 0;
        }
      }
      else
      {
        for (size_t index = 0; index < NUMBER_SIMPLE_OUTPUTS; ++index) error_rele[index] = 0;
      }

#ifdef NUMBER_DS
      if (control_ds != ds)  
      {
        for (size_t index = 0; index < NUMBER_DS; ++index)
        {
          uint32_t maska = 1 << index;
        
          if ((control_ds & maska) != (ds & maska))
          {
            if (error_rele[NUMBER_SIMPLE_OUTPUTS + index] < 3) ++error_rele[NUMBER_SIMPLE_OUTPUTS + index];
            if (error_rele[NUMBER_SIMPLE_OUTPUTS + index] >= 3 ) _SET_BIT(set_diagnostyka, (ERROR_DS_OUTPUT_BIT + index));
          }
          else error_rele[NUMBER_SIMPLE_OUTPUTS + index] = 0;
        }
      }
      else
      {
        for (size_t index = 0; index < NUMBER_DS; ++index) error_rele[NUMBER_SIMPLE_OUTPUTS + index] = 0;
      }
#endif
    }
    
    //ĳ��������� ���������-��������� ����
    {
      uint32_t board_register_tmp = _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47);
      uint32_t board_register_diff = board_register_tmp ^ board_register;
      board_register = board_register_tmp;

      if ((board_register_tmp & 0x01) !=  0x1) _SET_BIT(set_diagnostyka, ERROR_BA_1_FIX);
      else if (board_register_diff & 0x01)
      {
        _SET_BIT(clear_diagnostyka, ERROR_BA_1_FIX);
        _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x1;
        if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) >> 8) != 0x11)  _SET_BIT(set_diagnostyka, ERROR_BA_1_CTLR);
        _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x0;
      }
      
      if ((board_register_tmp & 0x02) !=  0x2) _SET_BIT(set_diagnostyka, ERROR_BDVV5_1_FIX);
      else if (board_register_diff & 0x02)
      {
        _SET_BIT(clear_diagnostyka, ERROR_BDVV5_1_FIX);
        _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x2;
        if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) & 0xff) != 0x25)  _SET_BIT(set_diagnostyka, ERROR_BDVV5_1_CTLR);
        _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x0;
      }
      
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      if ((board_register_tmp & 0x08) !=  0x8) _SET_BIT(set_diagnostyka, ERROR_CB_FIX);
      else if (board_register_diff & 0x08)
      {
        _SET_BIT(clear_diagnostyka, ERROR_CB_FIX);
      }
#endif

#if (                                \
     (MODYFIKACIA_VERSII_PZ == 0) || \
     (MODYFIKACIA_VERSII_PZ == 1) || \
     (MODYFIKACIA_VERSII_PZ == 3) || \
     (MODYFIKACIA_VERSII_PZ == 5) || \
     (MODYFIKACIA_VERSII_PZ == 10)|| \
     (MODYFIKACIA_VERSII_PZ == 11)|| \
     (MODYFIKACIA_VERSII_PZ == 13)|| \
     (MODYFIKACIA_VERSII_PZ == 15)|| \
     (MODYFIKACIA_VERSII_PZ == 23)|| \
     (MODYFIKACIA_VERSII_PZ == 33)   \
    )
      if ((board_register_tmp & 0x04) !=  0x4) _SET_BIT(set_diagnostyka, ERROR_BDVV5_2_FIX);
      else if (board_register_diff & 0x04)
      {
        _SET_BIT(clear_diagnostyka, ERROR_BDVV5_2_FIX);
        _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x4;
        if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) >> 8) != 0x37)  _SET_BIT(set_diagnostyka, ERROR_BDVV5_2_CTLR);
        _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x0;
      }
#elif (                                \
       (MODYFIKACIA_VERSII_PZ == 6) || \
       (MODYFIKACIA_VERSII_PZ == 26)   \
      )
      if ((board_register_tmp & 0x04) !=  0x4) _SET_BIT(set_diagnostyka, ERROR_BDVV9_FIX);
      else if (board_register_diff & 0x04)
      {
        _SET_BIT(clear_diagnostyka, ERROR_BDVV9_FIX);
        _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x4;
        if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD33_DD36) >> 8) != 0x29)  _SET_BIT(set_diagnostyka, ERROR_BDVV9_CTLR);
        _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x0;
      }
#endif

#if (                                   \
       (MODYFIKACIA_VERSII_PZ == 0) ||  \
       (MODYFIKACIA_VERSII_PZ == 3) ||  \
       (MODYFIKACIA_VERSII_PZ == 4) ||  \
       (MODYFIKACIA_VERSII_PZ == 5) ||  \
       (MODYFIKACIA_VERSII_PZ == 6) ||  \
       (MODYFIKACIA_VERSII_PZ == 10)||  \
       (MODYFIKACIA_VERSII_PZ == 13)||  \
       (MODYFIKACIA_VERSII_PZ == 14)||  \
       (MODYFIKACIA_VERSII_PZ == 15)||  \
       (MODYFIKACIA_VERSII_PZ == 23)||  \
       (MODYFIKACIA_VERSII_PZ == 24)||  \
       (MODYFIKACIA_VERSII_PZ == 26)||  \
       (MODYFIKACIA_VERSII_PZ == 33)||  \
       (MODYFIKACIA_VERSII_PZ == 34)    \
      )   
      if ((board_register_tmp & 0x010) != 0x10) 
      
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 10)      \
    )                                   
        _SET_BIT(set_diagnostyka, ERROR_BDV_DZ_FIX);
#elif (                                 \
       (MODYFIKACIA_VERSII_PZ == 5) ||  \
       (MODYFIKACIA_VERSII_PZ == 15)    \
      )   
        _SET_BIT(set_diagnostyka, ERROR_BDVV6_FIX);
#elif (                                 \
       (MODYFIKACIA_VERSII_PZ == 3) ||  \
       (MODYFIKACIA_VERSII_PZ == 4) ||  \
       (MODYFIKACIA_VERSII_PZ == 6) ||  \
       (MODYFIKACIA_VERSII_PZ == 13)||  \
       (MODYFIKACIA_VERSII_PZ == 14)    \
      )
        _SET_BIT(set_diagnostyka, ERROR_BDZ_FIX);
#elif (                                 \
       (MODYFIKACIA_VERSII_PZ == 23) || \
       (MODYFIKACIA_VERSII_PZ == 24) || \
       (MODYFIKACIA_VERSII_PZ == 26) || \
       (MODYFIKACIA_VERSII_PZ == 33) || \
       (MODYFIKACIA_VERSII_PZ == 34)    \
      )
        _SET_BIT(set_diagnostyka, ERROR_BDSH_FIX);
#else
 #error  "UDEFINED MODIFIKACIA"
#endif
      else if (board_register_diff & 0x10)
      {
#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 10)      \
    )                                   
        _SET_BIT(clear_diagnostyka, ERROR_BDV_DZ_FIX);
#elif (                                 \
       (MODYFIKACIA_VERSII_PZ == 5) ||  \
       (MODYFIKACIA_VERSII_PZ == 15)    \
      )   
        _SET_BIT(clear_diagnostyka, ERROR_BDVV6_FIX);
#elif (                                 \
       (MODYFIKACIA_VERSII_PZ == 3) ||  \
       (MODYFIKACIA_VERSII_PZ == 4) ||  \
       (MODYFIKACIA_VERSII_PZ == 6) ||  \
       (MODYFIKACIA_VERSII_PZ == 13)||  \
       (MODYFIKACIA_VERSII_PZ == 14)    \
      )
        _SET_BIT(clear_diagnostyka, ERROR_BDZ_FIX);
#elif (                                 \
       (MODYFIKACIA_VERSII_PZ == 23) || \
       (MODYFIKACIA_VERSII_PZ == 24) || \
       (MODYFIKACIA_VERSII_PZ == 26) || \
       (MODYFIKACIA_VERSII_PZ == 33) || \
       (MODYFIKACIA_VERSII_PZ == 34)    \
      )
        _SET_BIT(clear_diagnostyka, ERROR_BDSH_FIX);
#else
 #error  "UDEFINED MODIFIKACIA"
#endif

        _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x10;

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 10)      \
    )                                   
        if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 8) != 0x14)  _SET_BIT(set_diagnostyka, ERROR_BDV_DZ_CTLR);
#elif (                                 \
       (MODYFIKACIA_VERSII_PZ == 5) ||  \
       (MODYFIKACIA_VERSII_PZ == 15)    \
      )   
        if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 8) != 0x12)  _SET_BIT(set_diagnostyka, ERROR_BDVV6_CTLR);
#elif (                                 \
       (MODYFIKACIA_VERSII_PZ == 3) ||  \
       (MODYFIKACIA_VERSII_PZ == 4) ||  \
       (MODYFIKACIA_VERSII_PZ == 6) ||  \
       (MODYFIKACIA_VERSII_PZ == 13)||  \
       (MODYFIKACIA_VERSII_PZ == 14)    \
      )
        if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 8) != 0x18)  _SET_BIT(set_diagnostyka, ERROR_BDZ_CTLR);
#elif (                                 \
       (MODYFIKACIA_VERSII_PZ == 23)||  \
       (MODYFIKACIA_VERSII_PZ == 24)||  \
       (MODYFIKACIA_VERSII_PZ == 26)||  \
       (MODYFIKACIA_VERSII_PZ == 33)||  \
       (MODYFIKACIA_VERSII_PZ == 34)    \
      )
        if ((_DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD26_DD29) >> 8) != 0x13)  _SET_BIT(set_diagnostyka, ERROR_BDSH_CTLR);
#else
 #error  "UDEFINED MODIFIKACIA"
#endif

        _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD39_DD40_DD47) = 0x0;
      }
#endif
    }

    //������� �������
    main_protection();
    
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
    /***
    ����������, ��� ��������� ����� ����������� �������� ������������
    ***/
//    while ((DMA_StreamCANAL1_MO_Tx->CR & DMA_IT_TC) != 0);
    while (DMA_StreamCANAL1_MO_Tx->NDTR != 0);

//    uint32_t tick_for_Canal_1_tmp = tick_for_Canal_1;
    uint32_t tick_for_Canal_1_tmp = TIM2->CNT;
    uint64_t delta_tmp = 0;
    do
    {
      uint32_t tick_tmp = TIM2->CNT;
      delta_tmp = (tick_tmp < tick_for_Canal_1_tmp) ? (tick_tmp + 0x100000000 - tick_for_Canal_1_tmp) : (tick_tmp - tick_for_Canal_1_tmp);
    }
    while(delta_tmp < 2);
    /***/

    //�������� �������� ����� �� ������ CANAL1_MO � ������������ �����
    start_transmint_data_via_CANAL1_MO();
#endif
    /***********************************************************/

    /***********************************************************/
    //�������� �� ����������� ������� ������� ��ﳿ ����� ��� ������������
    /***********************************************************/
    //������� ����������
    if (periodical_tasks_TEST_TRG_FUNC != 0)
    {
      //����� � ���� ������� ������ ������� ������� ��ﳿ �����
      if ((state_spi1_task & STATE_TRG_FUNC_EEPROM_GOOD) != 0)
      {
        //������ ���� ����� ���, ���� ������� ���������� ������ ������� � ���������� ���������� ����
        if (
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_TRG_FUNC_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_TRG_FUNC_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_TRG_FUNC_EEPROM_BIT    ) == 0)
           ) 
        {
          //�� ����� ������� �� ��� �������-����� ������� ����������, ���� ����� �������� ���������
          fix_active_buttons_ctrl = fix_active_buttons;
          for (unsigned int i = 0; i < N_BIG; i++) trigger_active_functions_ctrl[i] = trigger_active_functions[i];
          crc_trg_func_ctrl = crc_trg_func;

          //������� ������� ������ ���������� �������� ��ﳿ 
          periodical_tasks_TEST_TRG_FUNC = false;
          //����������� ������� ������ �������� ����������� �� �������� ��ﳿ 
          periodical_tasks_TEST_TRG_FUNC_LOCK = true;
        }
      }
      else
      {
        //������� ������� ������ ���������� �������� ��ﳿ 
        periodical_tasks_TEST_TRG_FUNC = false;
      }
    }

    //���������� ���������
    if (periodical_tasks_TEST_INFO_REJESTRATOR_AR != 0)
    {
      //����� � ���� ������� ������ ������� ������� ��ﳿ �����
      if ((state_spi1_task & STATE_INFO_REJESTRATOR_AR_EEPROM_GOOD) != 0)
      {
        //������ ���� ����� ���, ���� ��������� ���������� ���������� ������ ������� � ���������� ���������� ����
        if (
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_AR_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_AR_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_AR_EEPROM_BIT    ) == 0)
           ) 
        {
          //�� ����� ������� �� ��� �������-����� ��������� ���������� ����������, ���� ����� �������� ���������
          info_rejestrator_ar_ctrl = info_rejestrator_ar;
          crc_info_rejestrator_ar_ctrl = crc_info_rejestrator_ar;

          //������� ������� ������ ���������� �������� ��ﳿ 
          periodical_tasks_TEST_INFO_REJESTRATOR_AR = false;
          //����������� ������� ������ �������� ����������� �� �������� ��ﳿ 
          periodical_tasks_TEST_INFO_REJESTRATOR_AR_LOCK = true;
        }
      }
      else
      {
        //������� ������� ������ ���������� �������� ��ﳿ 
        periodical_tasks_TEST_INFO_REJESTRATOR_AR = false;
      }
    }

    //���������� ���������
    if (periodical_tasks_TEST_INFO_REJESTRATOR_DR != 0)
    {
      //����� � ���� ������� ������ ������� ������� ��ﳿ �����
      if ((state_spi1_task & STATE_INFO_REJESTRATOR_DR_EEPROM_GOOD) != 0)
      {
        //������ ���� ����� ���, ���� ��������� ���������� ���������� ������ ������� � ���������� ���������� ����
        if (
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_INFO_REJESTRATOR_DR_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_INFO_REJESTRATOR_DR_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_INFO_REJESTRATOR_DR_EEPROM_BIT    ) == 0)
           ) 
        {
          //�� ����� ������� �� ��� �������-����� ��������� ���������� ����������, ���� ����� �������� ���������
          info_rejestrator_dr_ctrl = info_rejestrator_dr;
          crc_info_rejestrator_dr_ctrl = crc_info_rejestrator_dr;

          //������� ������� ������ ���������� �������� ��ﳿ 
          periodical_tasks_TEST_INFO_REJESTRATOR_DR = false;
          //����������� ������� ������ �������� ����������� �� �������� ��ﳿ 
          periodical_tasks_TEST_INFO_REJESTRATOR_DR_LOCK = true;
        }
      }
      else
      {
        //������� ������� ������ ���������� �������� ��ﳿ 
        periodical_tasks_TEST_INFO_REJESTRATOR_DR = false;
      }
    }
    /***********************************************************/

    //˳������� �������
    if (periodical_tasks_TEST_RESURS != 0)
    {
      //����� � ���� ������� ������ ������� ������� ��ﳿ �����
      if ((state_spi1_task & STATE_RESURS_EEPROM_GOOD) != 0)
      {
        //������ ���� ����� ���, ���� ���������z ������ ������� � ���������� ���������� ����
        if (
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_RESURS_EEPROM_BIT) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_RESURS_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_RESURS_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_RESURS_EEPROM_BIT    ) == 0)
           ) 
        {
          //�� ����� ������� �� ��� �������-����� ������� ��������, ���� ����� �������� ���������
          resurs_vymykacha_ctrl = resurs_vymykacha;          
          resurs_vidkljuchennja_ctrl = resurs_vidkljuchennja;
          crc_resurs_ctrl = crc_resurs;

          //������� ������� ������ ���������� �������� ��ﳿ 
          periodical_tasks_TEST_RESURS = false;
          //����������� ������� ������ �������� ����������� �� �������� ��ﳿ 
          periodical_tasks_TEST_RESURS_LOCK = true;
        }
      }
      else
      {
        //������� ������� ������ ���������� �������� ��ﳿ 
        periodical_tasks_TEST_RESURS = false;
      }
    }

    /***********************************************************/
    //������������ "�������� ��������� ��� ���������� �����������"
    /***********************************************************/
    uint32_t capture_new;
    unsigned int delta;
    TIM2->CCR1 = (capture_new = (current_tick + (delta = TIM2_CCR1_VAL)));
    
    unsigned int repeat;
    unsigned int previous_tick = current_tick;
    do
    {
      repeat = 0;
      current_tick = TIM2->CNT;

      uint64_t delta_time = 0;
      if (capture_new < current_tick)
        delta_time = capture_new + 0x100000000 - current_tick;
      else delta_time = capture_new - current_tick;

      if ((delta_time > delta) || (delta_time == 0))
      {
        if (TIM_GetITStatus(TIM2, TIM_IT_CC1) == RESET)
        {
          if (delta < TIM2_CCR1_VAL)
          {
            uint64_t delta_tick;
            if (current_tick < previous_tick)
              delta_tick = current_tick + 0x100000000 - previous_tick;
            else delta_tick = current_tick - previous_tick;
              
            delta = delta_tick + 1;
          }
          else if (delta == TIM2_CCR1_VAL)
            delta = 1; /*����������, ��� ���� ���������� ����������� �� ����� ������*/
          else
          {
            //���������� ����� ����� �� ���� � ����
            total_error_sw_fixed();
          }
          TIM2->CCR1 = (capture_new = (TIM2->CNT +  delta));
          previous_tick = current_tick;
          repeat = 0xff;
        }
      }
    }
    while (repeat != 0);
    /***********************************************************/
    
    /***********************************************************/
    //����������� ����������� ��� ��, ������� ������ �������
    /***********************************************************/
    control_word_of_watchdog |= WATCHDOG_PROTECTION;
    /***********************************************************/
    /***********************************************************************************************/
  }
  else
  {
    total_error_sw_fixed();
  }
  
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

/*****************************************************/
//���� ����� �������
/*****************************************************/
void setpoints_selecting(unsigned int *p_active_functions, unsigned int act_inp_gr_ustavok) {
  unsigned int grupa_ustavok = 0;
  act_inp_gr_ustavok &= 0xf;
  if (current_settings_prt.grupa_ustavok < SETPOINT_GRUPA_USTAVOK_MIN ||
      current_settings_prt.grupa_ustavok > SETPOINT_GRUPA_USTAVOK_MAX) {
    //�������������� ������
    while (1);
  } else {
    grupa_ustavok = 1 << (current_settings_prt.grupa_ustavok - 1);
  }
  
  if (_CHECK_SET_BIT(p_active_functions, RANG_INVERS_DV_GRUPA_USTAVOK)) _SET_STATE(act_inp_gr_ustavok, 4);
  
  _AND2(grupa_ustavok, 0, act_inp_gr_ustavok, 4, grupa_ustavok, 4);
  _AND2(grupa_ustavok, 1, act_inp_gr_ustavok, 4, grupa_ustavok, 5);
  _AND2(grupa_ustavok, 2, act_inp_gr_ustavok, 4, grupa_ustavok, 6);
  _AND2(grupa_ustavok, 3, act_inp_gr_ustavok, 4, grupa_ustavok, 7);
  
  unsigned int tmp = gr_ustavok_tmp;
  do {
    _AND4(act_inp_gr_ustavok, 0, gr_ustavok_tmp, 1, gr_ustavok_tmp, 2, gr_ustavok_tmp, 3, act_inp_gr_ustavok, 5);
    _INVERTOR(act_inp_gr_ustavok, 5, tmp, 0);
    
    _AND4(tmp, 0, act_inp_gr_ustavok, 1, tmp, 2, tmp, 3, act_inp_gr_ustavok, 6);
    _INVERTOR(act_inp_gr_ustavok, 6, tmp, 1);
    
    _AND4(tmp, 0, tmp, 1, act_inp_gr_ustavok, 2, tmp, 3, act_inp_gr_ustavok, 7);
    _INVERTOR(act_inp_gr_ustavok, 7, tmp, 2);
    
    _AND4(tmp, 0, tmp, 1, tmp, 2, act_inp_gr_ustavok, 3, act_inp_gr_ustavok, 8);
    _INVERTOR(act_inp_gr_ustavok, 8, tmp, 3);
    if (tmp == gr_ustavok_tmp) {
      break;
    }
    gr_ustavok_tmp = tmp;
  } while (1);
  
  _OR2(grupa_ustavok, 4, act_inp_gr_ustavok, 5, grupa_ustavok, 8);
  _OR2(grupa_ustavok, 5, act_inp_gr_ustavok, 6, grupa_ustavok, 9);
  _OR2(grupa_ustavok, 6, act_inp_gr_ustavok, 7, grupa_ustavok, 10);
  _OR2(grupa_ustavok, 7, act_inp_gr_ustavok, 8, grupa_ustavok, 11);
  
  if (_GET_STATE(grupa_ustavok, 8)) 
    _SET_BIT(p_active_functions, RANG_1_GRUPA_USTAVOK);
  else
    _CLEAR_BIT(p_active_functions, RANG_1_GRUPA_USTAVOK);

  if (_GET_STATE(grupa_ustavok, 9))
    _SET_BIT(p_active_functions, RANG_2_GRUPA_USTAVOK);
  else
    _CLEAR_BIT(p_active_functions, RANG_2_GRUPA_USTAVOK);

  if (_GET_STATE(grupa_ustavok, 10))
      _SET_BIT(p_active_functions, RANG_3_GRUPA_USTAVOK);
  else
    _CLEAR_BIT(p_active_functions, RANG_3_GRUPA_USTAVOK);

  if (_GET_STATE(grupa_ustavok, 11))
      _SET_BIT(p_active_functions, RANG_4_GRUPA_USTAVOK);
  else
    _CLEAR_BIT(p_active_functions, RANG_4_GRUPA_USTAVOK);
}
/*****************************************************/

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
void proc_Gs_blk_out(void* pv,unsigned long lCtrGsSrc,short* p_arrOrdNumsGsSignal ){

    // ----------------    -------------------------       
        register unsigned long i,lV,j;
         
         register void *pvll;

        pvll = (void*)&current_settings_prt.ranguvannja_In_GOOSE;//
        i = j = lV = 0;

        asm ("nop" ::"r"(i),"r"(j),"r"(lV));
        while(lCtrGsSrc){
            lCtrGsSrc--;
            lV = p_arrOrdNumsGsSignal[lCtrGsSrc];
            i = lV>>3;j = lV - (i<<3);

            lV = i*N_IN_GOOSE_MMS_OUT*N_SMALL +j*N_SMALL;
            for(register unsigned long k = 0; k < N_SMALL; k++){
                ((unsigned long*)pv)[k] |= ((unsigned long*)pvll+lV) [k];
            }
            
        }

}
void proc_Mms_blk_out(void* pv,unsigned long lCtrMmsSrc,short* p_arrOrdNumsMmsSignal ){

    // ----------------    -------------------------       
        register unsigned long i,lV,j;
         
         register void *pvll;

        pvll = (void*)&current_settings_prt.ranguvannja_In_MMS;//
        i = j = lV = 0;

        asm ("nop" ::"r"(i),"r"(j),"r"(lV));
        while(lCtrMmsSrc){
            lCtrMmsSrc--;
            lV = p_arrOrdNumsMmsSignal[lCtrMmsSrc];
            i = lV>>3;j = lV - (i<<3);

            lV = i*N_IN_GOOSE_MMS_OUT*N_SMALL +j*N_SMALL;
            for(register unsigned long k = 0; k < N_SMALL; k++){
                ((unsigned long*)pv)[k] |= ((unsigned long*)pvll+lV) [k];
            }
			
        }
		//
		/*
		2. � ������ ,���� �� ��������� "���� ���" (���� ����-��� ����������� �������) � MMS (GOOSE �� �� ���������!), �� ��� ����� ��������� ������:

global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + _n] = 0;

�� _n - �� ����� ����������� �������

���.
-----------------------------------------

��� ��������, �� � �� ����� � ������ 10140-10152 protection.c

�� ������ � �������. �� ��������� ����������� ����������� ������� � MMS, ���� � �� ���������� ������ ����� ����������. ����, � ������, �� �� ��������������� ������ ����� ��� ���� ���� � ����.
		*/
//.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF1_IN) == 0){
//.			//����� �� ������������ �-��������, ���� ����� ��������� ������ �� �����������,
//.			//��� ����, ��� ���� ��� ��� ����� ���� ������� � ����� ������� �����
//.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 0] < 0)
//.			{
//.			//��������� ������ ������ ����������� ���� ������� � ��������� ���� (������� ��������� �����)
//.			//������ ������ ����� �� ���� ��������, ���� �� �� �� �������
//.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 0] = 0;
//.			}
//.		}
//.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF2_IN) == 0){
//.			//����� �� ������������ �-��������, ���� ����� ��������� ������ �� �����������,
//.			//��� ����, ��� ���� ��� ��� ����� ���� ������� � ����� ������� �����
//.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 1] < 0)
//.			{
//.			//��������� ������ ������ ����������� ���� ������� � ��������� ���� (������� ��������� �����)
//.			//������ ������ ����� �� ���� ��������, ���� �� �� �� �������
//.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 1] = 0;
//.			}
//.		}
//.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF3_IN) == 0){
//.			//����� �� ������������ �-��������, ���� ����� ��������� ������ �� �����������,
//.			//��� ����, ��� ���� ��� ��� ����� ���� ������� � ����� ������� �����
//.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 2] < 0)
//.			{
//.			//��������� ������ ������ ����������� ���� ������� � ��������� ���� (������� ��������� �����)
//.			//������ ������ ����� �� ���� ��������, ���� �� �� �� �������
//.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 2] = 0;
//.			}
//.		}
//.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF4_IN) == 0){
//.			//����� �� ������������ �-��������, ���� ����� ��������� ������ �� �����������,
//.			//��� ����, ��� ���� ��� ��� ����� ���� ������� � ����� ������� �����
//.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 3] < 0)
//.			{
//.			//��������� ������ ������ ����������� ���� ������� � ��������� ���� (������� ��������� �����)
//.			//������ ������ ����� �� ���� ��������, ���� �� �� �� �������
//.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 3] = 0;
//.			}
//.		}
//.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF5_IN) == 0){
//.			//����� �� ������������ �-��������, ���� ����� ��������� ������ �� �����������,
//.			//��� ����, ��� ���� ��� ��� ����� ���� ������� � ����� ������� �����
//.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 4] < 0)
//.			{
//.			//��������� ������ ������ ����������� ���� ������� � ��������� ���� (������� ��������� �����)
//.			//������ ������ ����� �� ���� ��������, ���� �� �� �� �������
//.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 4] = 0;
//.			}
//.		}
//.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF6_IN) == 0){
//.			//����� �� ������������ �-��������, ���� ����� ��������� ������ �� �����������,
//.			//��� ����, ��� ���� ��� ��� ����� ���� ������� � ����� ������� �����
//.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 5] < 0)
//.			{
//.			//��������� ������ ������ ����������� ���� ������� � ��������� ���� (������� ��������� �����)
//.			//������ ������ ����� �� ���� ��������, ���� �� �� �� �������
//.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 5] = 0;
//.			}
//.		}
//.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF7_IN) == 0){
//.			//����� �� ������������ �-��������, ���� ����� ��������� ������ �� �����������,
//.			//��� ����, ��� ���� ��� ��� ����� ���� ������� � ����� ������� �����
//.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 6] < 0)
//.			{
//.			//��������� ������ ������ ����������� ���� ������� � ��������� ���� (������� ��������� �����)
//.			//������ ������ ����� �� ���� ��������, ���� �� �� �� �������
//.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 6] = 0;
//.			}
//.		}
//.		if (_CHECK_SET_BIT(((unsigned long*)pv), RANG_SMALL_DF8_IN) == 0){
//.			//����� �� ������������ �-��������, ���� ����� ��������� ������ �� �����������,
//.			//��� ����, ��� ���� ��� ��� ����� ���� ������� � ����� ������� �����
//.			if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 7] < 0)
//.			{
//.			//��������� ������ ������ ����������� ���� ������� � ��������� ���� (������� ��������� �����)
//.			//������ ������ ����� �� ���� ��������, ���� �� �� �� �������
//.			global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + 7] = 0;
//.			}
//.		}
}

void proc_Lan_blk_out(unsigned short *p_rang_Out_LAN,unsigned int *p_active_functions, void *pLanDsc){
    register unsigned long rU_V,rU_bit,rU_out, rU_Idx;
   
    unsigned long IdxBlk,l_O;
// ----------------    -------------------------
    IdxBlk = *((unsigned char*)pLanDsc);l_O = 0;//sLV.IdxBlk = LAN_BLOCK_ORD_NUM_00;    
    for(rU_out = 0; rU_out < 8; rU_out++){
        rU_bit = rU_V = 0;
        while( rU_bit < MAX_FUNCTIONS_IN_OUT_LAN ){
            //
            rU_V = (IdxBlk*N_OUT_LAN_IN*MAX_FUNCTIONS_IN_OUT_LAN) + (rU_out*MAX_FUNCTIONS_IN_OUT_LAN) + rU_bit;//find index
            rU_Idx = p_rang_Out_LAN[rU_V];//find index
            if(rU_Idx>0){
                rU_Idx--;
                //Create 32 bit mask
                rU_V = p_active_functions[rU_Idx >> 5] & ( (unsigned int)( 1 << (rU_Idx & 0x1f)));
                if(rU_V != 0){
                    ;//Set Val
                    rU_bit = MAX_FUNCTIONS_IN_OUT_LAN+1;
                }
            }
            rU_bit++;
        }
        rU_Idx = IdxBlk;//sLV.IdxBlk;
        if(rU_bit == (MAX_FUNCTIONS_IN_OUT_LAN+2) ){ //Activate Out
            //((unsigned char*)pLanDsc+1)[rU_Idx] |= 1<< rU_out;
            l_O  |= 1<< rU_out;
        }
    
    }   
    Output_Out_LAN_block[IdxBlk] = l_O;
}

#endif
/*****************************************************/
