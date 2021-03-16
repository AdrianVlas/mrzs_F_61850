#include "header.h"

/*****************************************************/
//����� ������ ������ ��� ���������
/*****************************************************/
inline void find_new_ADC_canal_to_read(unsigned int command_word_adc_diff, unsigned int *point_active_index_command_word_adc)
{
  unsigned int command_word_adc_diff_tmp = command_word_adc_diff;
  unsigned int command_word_adc_diff_fapch = command_word_adc_diff_tmp & maska_canaliv_fapch;
    
  if (
      ((status_adc_read_work & DATA_VAL_READ) != 0) &&
      (command_word_adc_diff_fapch != 0) 
     )  
  {
    command_word_adc_diff_tmp = command_word_adc_diff_fapch;
  }
  else
  {
    command_word_adc_diff_tmp &= (unsigned int)(~maska_canaliv_fapch);
  }
  
  while ((command_word_adc_diff_tmp & (1 << (*point_active_index_command_word_adc))) == 0)
  {
    /*
    �� �� �������� ������, ���� ����� ������������
    */
    *point_active_index_command_word_adc = (*point_active_index_command_word_adc + 1) % NUMBER_INPUTs_ADCs;
  }
}
/*****************************************************/

/*****************************************************/
//��������� �������� ����� � ���
/*****************************************************/
void control_reading_ADCs(void)
{
  //���������� ������ �������� ����� � �������� �� ������ ����� ������������
  if (adc_DATA_VAL_read != 0)
  {
    adc_DATA_VAL_read = false;
    status_adc_read_work |= DATA_VAL_READ;
      
    /*
    ������ �� ���� ���� ��������������� ������� ����� �������������� �� 
    ����� ������ �� ����������� ������� ������ ������� ���������
    */
    command_word_adc      &= (unsigned int)(~maska_canaliv_fapch);
    command_word_adc_work &= (unsigned int)(~maska_canaliv_fapch);
        
    command_word_adc |= READ_DATA_VAL;
  }

//  if (adc_TEST_VAL_read != 0) 
//  {
//    adc_TEST_VAL_read = false;
//    status_adc_read_work |= TEST_VAL_READ;
//      
//    command_word_adc |= READ_TEST_VAL;
//  }
  
  unsigned int command_word_adc_diff = command_word_adc ^ command_word_adc_work;
  if (command_word_adc_diff != 0)
  {
    /*
    � ������, �� ������� �� ���������
    */
    find_new_ADC_canal_to_read(command_word_adc_diff, &active_index_command_word_adc);
      
    /*
    ����� active_index_command_word_adc ����� � ����� input_adc �� �����, 
    ���� ����� ����������
    */
      
    /*
    ���������, ���� ����� �������� ���
    */
    unsigned int active_adc_old = ((GPIO_SELECT_ADC->ODR & GPIO_SELECTPin_ADC) == 0) ? 1 : 2;

    /*
    ���������, ���� ����� ����� ������������ ��� � �� ��� ����� ��������
    */
    unsigned int active_adc_new = input_adc[active_index_command_word_adc][0];
    unsigned int command_word = input_adc[active_index_command_word_adc][1];
    
    //��������� ��������� ����
    if (
        (state_reading_ADCs == STATE_READING_ADCs_NONE) ||
        (state_reading_ADCs == STATE_READING_READ     )
       )
    {
      state_reading_ADCs = STATE_READING_WRITE;
      /*
      ��������� ��������� ���
      */
      switch (active_adc_new)
      {
      case 1:
        {
          //���1
          GPIO_SELECT_ADC->BSRRH = GPIO_SELECTPin_ADC;
          break;
        }
        case 2:
        {
          //���2
          GPIO_SELECT_ADC->BSRRL = GPIO_SELECTPin_ADC;
          break;
        }
      default:
        {
          //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
          total_error_sw_fixed(50);
        } 
      }
    }
    else
    {
      if (active_adc_old == active_adc_new)
      {
        state_reading_ADCs = STATE_READING_WRITE_READ;
      }
      else
      {
        state_reading_ADCs = STATE_READING_READ;
        command_word = 0;
      }
    }
    
    if (command_word != 0)
    {
      /*
      �������, �� ����� ������ ���� �������� ����� ����������
      */
      command_word_adc_work |= (1 << active_index_command_word_adc);
    }

    //�������� �������� �����
    while ((SPI_ADC->SR & SPI_I2S_FLAG_TXE) == RESET);      //�������, ���� SPI ����� ������
    GPIO_SPI_ADC->BSRRH = GPIO_NSSPin_ADC;                  //����������� chip_select
    SPI_ADC->DR = (uint16_t)command_word;                   //³���������� �������� �����
    
    channel_answer = channel_request;
    channel_request = (active_adc_new - 1)*NUMBER_CANALs_ADC + ((command_word >> 10) & 0xf);
  }
  else 
  {
    if (
        (state_reading_ADCs == STATE_READING_READ     ) ||
        (state_reading_ADCs == STATE_READING_ADCs_NONE)
       )
    {
      //�� ������ ��� ���������
      state_reading_ADCs = STATE_READING_ADCs_NONE;

      command_word_adc = 0;
      command_word_adc_work = 0;
      active_index_command_word_adc = 0;
    }
    else
    {
      /*
      ����� �� �������� �������� � ���������� ��������, ���� ����� �����
      ���� �������������
      */
      state_reading_ADCs = STATE_READING_READ;
      while ((SPI_ADC->SR & SPI_I2S_FLAG_TXE) == RESET);      //�������, ���� SPI ����� ������
      GPIO_SPI_ADC->BSRRH = GPIO_NSSPin_ADC;                  //����������� chip_select
      SPI_ADC->DR = 0;                                        //³���������� ����� (��� ���, ��� ���� ���������� ����� �� ������������)
    
      channel_answer = channel_request;
    }
  }
}
/*****************************************************/

/*************************************************************************
����������� ��� ��� ������������ ���'�
 *************************************************************************/
void Fourier(void)
{
  unsigned int index_data_sin_cos_array_tmp = index_data_sin_cos_array;
  unsigned int index_sin_cos_array_tmp = index_sin_cos_array;

  long long data64_new = (long long)ADCs_data[I_3I0];
  unsigned long long square_new = data64_new*data64_new;

  sum_sqr_data_3I0_irq += square_new;
  sum_sqr_data_3I0_irq -= sqr_current_data_3I0[index_array_of_one_value_fourier];
  sqr_current_data_3I0[index_array_of_one_value_fourier] = square_new;
    
  if((++index_array_of_one_value_fourier) == NUMBER_POINT)
    index_array_of_one_value_fourier = 0;
  else if (index_array_of_one_value_fourier > NUMBER_POINT)
  {
    //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
    total_error_sw_fixed(58);
  }

  for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
  {
    //������� ������ �������� ��� ����� ����������
    int temp_value_1 = ADCs_data[i];
    int temp_value_2;
    unsigned int i_ort_tmp = 2*i;

    //����������� SIN
    ortogonal_irq[i_ort_tmp] -= data_sin[index_data_sin_cos_array_tmp];
    temp_value_2 = (int)((float)temp_value_1*sin_data_f[index_sin_cos_array_tmp]);
    data_sin[index_data_sin_cos_array_tmp] = temp_value_2;
    ortogonal_irq[i_ort_tmp] += temp_value_2;
    
    //����������� COS
    ortogonal_irq[i_ort_tmp + 1] -= data_cos[index_data_sin_cos_array_tmp];
    temp_value_2 = (int)((float)temp_value_1*cos_data_f[index_sin_cos_array_tmp]);
    data_cos[index_data_sin_cos_array_tmp] = temp_value_2;
    ortogonal_irq[i_ort_tmp + 1] += temp_value_2;
    
    if((++index_data_sin_cos_array_tmp) >= (NUMBER_POINT*NUMBER_ANALOG_CANALES)) index_data_sin_cos_array_tmp = 0;
  }
  index_data_sin_cos_array = index_data_sin_cos_array_tmp;
  
  if((++index_sin_cos_array_tmp) >= NUMBER_POINT) index_sin_cos_array_tmp = 0;
  index_sin_cos_array = index_sin_cos_array_tmp;

  //��������� ��� ����� ������
  unsigned int bank_ortogonal_tmp = bank_ortogonal;
  for(unsigned int i = 0; i < (2*NUMBER_ANALOG_CANALES); i++ ) ortogonal[i][bank_ortogonal_tmp] = ortogonal_irq[i];
  sum_sqr_data_3I0[bank_ortogonal_tmp] = sum_sqr_data_3I0_irq;
}
/*************************************************************************/

/*************************************************************************
�������� ������� ��� ������ ����� 1
*************************************************************************/
void fapch(void)
{
  unsigned int bank_measurement_high_tmp = bank_measurement_high;
  unsigned int canal_phase_line = current_settings.control_extra_settings_1 & CTR_EXTRA_SETTINGS_1_CTRL_PHASE_LINE;
  int index_1 = -1;
  unsigned int maska_canaliv_fapch_tmp = 0;

  /*****
  ������ ������ �� ����� ������ ������������� �������
  *****/  
  if (
      (
       (canal_phase_line == 0) &&
       (measurement_high[bank_measurement_high_tmp][IM_UA] >= PORIG_FOR_FAPCH)
      )   
      ||  
      (
       (canal_phase_line != 0) &&
       (measurement_high[bank_measurement_high_tmp][IM_UAB] >= PORIG_FOR_FAPCH)
      )   
     )   
  {
    index_1 = INDEX_PhK_UA;
    maska_canaliv_fapch_tmp = READ_Ua;
  }
  else if (
           (
            (canal_phase_line == 0) &&
            (measurement_high[bank_measurement_high_tmp][IM_UB] >= PORIG_FOR_FAPCH)
           )   
           ||  
           (
            (canal_phase_line != 0) &&
            (measurement_high[bank_measurement_high_tmp][IM_UBC] >= PORIG_FOR_FAPCH)
           )   
          )   
  {
    index_1 = INDEX_PhK_UB;
    maska_canaliv_fapch_tmp = READ_Ub;
  }
  else if (
           (
            (canal_phase_line == 0) &&
            (measurement_high[bank_measurement_high_tmp][IM_UC] >= PORIG_FOR_FAPCH)
           )   
           ||  
           (
            (canal_phase_line != 0) &&
            (measurement_high[bank_measurement_high_tmp][IM_UCA] >= PORIG_FOR_FAPCH)
           )   
          )
  {
    index_1 = INDEX_PhK_UC;
    maska_canaliv_fapch_tmp = READ_Uc;
  }
  else if (measurement_high[bank_measurement_high_tmp][IM_3U0] >= PORIG_FOR_FAPCH) 
  {
    index_1 = INDEX_PhK_3U0;
    maska_canaliv_fapch_tmp = READ_3U0;
  }
  maska_canaliv_fapch = maska_canaliv_fapch_tmp;
  /*****/

  uint32_t step_timer_adc_tmp;
  /*****/
  //�������
  /*****/
  step_timer_adc_tmp = step_timer_adc;
  if (
      (index_1 >= 0) &&
      (fix_perechid_cherez_nul[index_1] != 0)
     )   
  {
    fix_perechid_cherez_nul[index_1] = 0;
    
    unsigned int delta_tick;
    long long tick_tmp;
    int delta_value;
    unsigned int tick_p, x1_tmp, x2_tmp;
      
    /*��������� ��� �������� ����� 0 ������������ ���� � ����������� ����� ������������*/
    delta_value = perechid_cherez_nul[index_1][0].y2 - perechid_cherez_nul[index_1][0].y1;
    x1_tmp = perechid_cherez_nul[index_1][0].x1;
    x2_tmp = perechid_cherez_nul[index_1][0].x2;
    if (x2_tmp > x1_tmp) delta_tick = x2_tmp - x1_tmp;
    else
    {
      long long delta_tick_64 = x2_tmp + 0x100000000 - x1_tmp;
      delta_tick = delta_tick_64;
    }
    tick_tmp = ((long long)perechid_cherez_nul[index_1][0].x1) - ((long long)perechid_cherez_nul[index_1][0].y1)*((long long)delta_tick)/((long long)delta_value);
    if (tick_tmp < 0) 
    {
      tick_tmp += 0x100000000;
      tick_p = (unsigned int)tick_tmp;
    }
    else
    {
      if (tick_tmp < 0x100000000) tick_p = (unsigned int)tick_tmp;
      else 
      {
        tick_tmp -= 0x100000000;
        tick_p = (unsigned int)tick_tmp;
      }
    }

    /*��������� ��� �������� ����� 0 ��������� ���� � ����������� ����� ������������*/
    delta_value = perechid_cherez_nul[index_1][1].y2 - perechid_cherez_nul[index_1][1].y1;
    x1_tmp = perechid_cherez_nul[index_1][1].x1;
    x2_tmp = perechid_cherez_nul[index_1][1].x2;
    if (x2_tmp > x1_tmp) delta_tick = x2_tmp - x1_tmp;
    else
    {
      long long delta_tick_64 = x2_tmp + 0x100000000 - x1_tmp;
      delta_tick = delta_tick_64;
    }
    tick_tmp = ((long long)perechid_cherez_nul[index_1][1].x1) - ((long long)perechid_cherez_nul[index_1][1].y1)*((long long)delta_tick)/((long long)delta_value);
    if (tick_tmp < 0) 
    {
      tick_tmp += 0x100000000;
      tick_c = (unsigned int)tick_tmp;
    }
    else
    {
      if (tick_tmp < 0x100000000) tick_c = (unsigned int)tick_tmp;
      else 
      {
        tick_tmp -= 0x100000000;
        tick_c = (unsigned int)tick_tmp;
      }
    }
    /***/
      
    if (tick_c > tick_p) delta_tick = tick_c - tick_p;
    else
    {
      long long delta_tick_64 = tick_c + 0x100000000 - tick_p;
      delta_tick = delta_tick_64;
    }
    tick_period = delta_tick;
    
    /*****
    ���������� �������
    *****/
    if (
        (tick_period <= MAX_TICK_PERIOD) &&
        (tick_period >= MIN_TICK_PERIOD)
       ) 
    {
      frequency = (float)MEASUREMENT_TIM_FREQUENCY/(float)tick_period;

      step_timer_adc_tmp = tick_period >> VAGA_NUMBER_POINT;
      if ((tick_period - (step_timer_adc_tmp << VAGA_NUMBER_POINT)) >= (1 << (VAGA_NUMBER_POINT - 1))) step_timer_adc_tmp++;
    }
    else
    {
      step_timer_adc_tmp = TIM5_CCR1_2_VAL;
      if (tick_period > MAX_TICK_PERIOD) frequency = -2; /*������� ����� ������ ����������� ���������� MIN_FREQUENCY*/
      else frequency = -3; /*������� ���� ������ ����������� ���������� MAX_FREQUENCY*/
    }
    /****/
  }
  else
  {
    if (index_1 < 0)
    {
      step_timer_adc_tmp = TIM5_CCR1_2_VAL;
      frequency = -1; /*������� �� ���������*/
    }
  }
  /*****/

  /*****/
  //����
  /*****/
  if (step_timer_adc != step_timer_adc_tmp)
  {
    //����� ������ ������� �������������
    step_timer_adc = step_timer_adc_tmp;
  }
  
  if ((command_restart_monitoring_frequency & (1 << 0)) != 0)
  {
    frequency_min = 50;
    frequency_max = 50;
    
    command_restart_monitoring_frequency &= (unsigned int)(~(1 << 0));
  }
  else
  {
    if (frequency >= 0)
    {
      if (frequency > frequency_max) frequency_max = frequency;
      if (frequency < frequency_min) frequency_min = frequency;
    }
  }
  /*****/
}
/*****************************************************/

/*****************************************************/
//����������� �� ���������� ����� �� ������ SPI, ���� ��������� ����������� �������
/*****************************************************/
void SPI_ADC_IRQHandler(void)
{
//#ifdef SYSTEM_VIEWER_ENABLE
//  SEGGER_SYSVIEW_RecordEnterISR();
//#endif

  //Գ����� ���� � ����������� ������� ����� �� ���
  semaphore_adc_irq  = true;
  
  //Գ����� ��� ���������
  uint32_t tick_output_adc_tmp = TIM5->CNT;
  
  //�� ������ ������� ������ chip_select
  GPIO_SPI_ADC->BSRRL = GPIO_NSSPin_ADC;
  uint16_t read_value = SPI_ADC->DR;

  static uint32_t tick_output_adc_p;
  /*
  �������� ������� ���
  */
  if (
      (state_reading_ADCs == STATE_READING_WRITE_READ) ||
      (state_reading_ADCs == STATE_READING_READ)
     )
  {
    unsigned int shift = ((GPIO_SELECT_ADC->ODR & GPIO_SELECTPin_ADC) == 0) ? 0 : NUMBER_CANALs_ADC;
    unsigned int number_canal = shift + ((read_value >> 12) & 0xf);
    
    output_adc[number_canal].tick = tick_output_adc_p;
    
    static uint32_t error_spi_adc;
    if(channel_answer != number_canal) 
    {
      if (error_spi_adc < 3 ) error_spi_adc++;
      if (error_spi_adc >= 3 )_SET_BIT(set_diagnostyka, ERROR_SPI_ADC_BIT);
    }
    else 
    {
      error_spi_adc = 0;
      
      _SET_BIT(clear_diagnostyka, ERROR_SPI_ADC_BIT);
      output_adc[number_canal].value = read_value & 0xfff;
    }
  }
  tick_output_adc_p = tick_output_adc_tmp;
  /***/
  
  //�������� �������� � ������� ���
  control_reading_ADCs();
  
  /*
  �������� �� �������� ����� ���, ���� �� ������ ��� ���������
  */
  if (state_reading_ADCs == STATE_READING_ADCs_NONE)
  {
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif
  
    /*******************************************************/
    //����������, �� ���������� ���� ���������
    /*******************************************************/
    if (changed_ustuvannja == CHANGED_ETAP_ENDED) /*�� � �����, �� ��� ��� ���������� ��� �������� �� � ������ ������������ �������� (� ��� ����� ����� ��� �� ���������)*/
    {
      //������� ����� ��������� � ���� ����� ������ ��� � ���� ������ (���� � �����) ����� ����������� ���������
      for(unsigned int k = 0; k < NUMBER_ANALOG_CANALES; k++) 
      {
        ustuvannja_meas[k] = ustuvannja[k];
      }
      
      //�������, �� ���� ������� ������������ ��������, ��� �� ����� �������� �������� �������
      changed_ustuvannja = CHANGED_ETAP_ENDED_EXTRA_ETAP;
    }
    /*****************************************************/

    /*
    ������� �������� ������������� ������
    */
    unsigned int command_word = 0;
    if ((status_adc_read_work & DATA_VAL_READ) != 0)
    {
      command_word |= (1 << I_3I0)|
                      (1 << I_Ia) | (1 << I_Ib_I04) | (1 << I_Ic) |
                      (1 << I_Ua) | (1 << I_Ub    ) | (1 << I_Uc) |
                       (1 << I_3U0);
        
    }
      
    uint32_t _x1, _x2, _DX, _dx;
    int _y1, _y2;
    long long _y;
      
//    unsigned int gnd_adc  = gnd_adc1; 
//    unsigned int vref_adc/* = vref_adc1*/; 

    uint32_t _x = previous_tick_DATA_VAL;
    /*****/
    //������� �������� 3I0
    /*****/
    if ((command_word & (1 << I_3I0)) != 0)
    {
      _x1 = ADCs_data_raw[I_3I0].tick;
      _y1 = ADCs_data_raw[I_3I0].value;
      
      static uint32_t index_array_of_one_value_3I0;
      
      uint32_t val_C_3I0_16 = output_adc[C_3I0_16].value;
      vref_adc_averange_sum[I_3I0] += val_C_3I0_16;
      
      if((++index_array_of_one_value_3I0) == NUMBER_POINT)
      {
        index_array_of_one_value_3I0 = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_3I0] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_3I0] = 0;

        //������ ����� ����������� �� �������
        static uint32_t index_array_of_one_value_3I0_1s;
        
        vref_adc_averange_sum_1s[I_3I0] += vref_adc_period;
        vref_adc_averange_sum_1s[I_3I0] -= vref_adc_moment_value_1s[I_3I0][index_array_of_one_value_3I0_1s];
        vref_adc_moment_value_1s[I_3I0][index_array_of_one_value_3I0_1s] = vref_adc_period;
        vref_adc[I_3I0] = vref_adc_averange_sum_1s[I_3I0] / MAIN_FREQUENCY;
        
        if((++index_array_of_one_value_3I0_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_3I0_1s = 0;
        else if (index_array_of_one_value_3I0_1s > MAIN_FREQUENCY)
        {
          //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
          total_error_sw_fixed(104);
        }
      }
      else if (index_array_of_one_value_3I0 > NUMBER_POINT)
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(21);
      }
        
//      _y2 = output_adc[C_3I0_1].value - gnd_adc - vref_adc;
//      if (abs(_y2) > 87)
//      {
//        _x2 = output_adc[C_3I0_1].tick;
//        _y2 = (int)(_y2*ustuvannja_meas[I_3I0])>>(USTUVANNJA_VAGA - 2*4);
//      }
//      else
//      {
        _y2 = val_C_3I0_16 - /*gnd_adc - */ vref_adc[I_3I0];
        if (abs(_y2) > 87)
        {
          _x2 = output_adc[C_3I0_16].tick;
          _y2 = (int)(_y2*ustuvannja_meas[I_3I0])>>(USTUVANNJA_VAGA - 4);
        }
        else
        {
          _y2 = output_adc[C_3I0_256].value - /*gnd_adc - */ vref_adc[I_3I0];

          _x2 = output_adc[C_3I0_256].tick;
          _y2 = (int)((-_y2)*ustuvannja_meas[I_3I0])>>(USTUVANNJA_VAGA);
        }
//      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);

      ADCs_data[I_3I0] = _y;
      
      ADCs_data_raw[I_3I0].tick = _x2;
      ADCs_data_raw[I_3I0].value = _y2;
    }
    /*****/

    /*****/
    //������� �������� Ia
    /*****/
    if ((command_word & (1 << I_Ia)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ia].tick;
      _y1 = ADCs_data_raw[I_Ia].value;
        
      static uint32_t index_array_of_one_value_Ia;
      
      uint32_t val_C_Ia_1 = output_adc[C_Ia_1].value;
      vref_adc_averange_sum[I_Ia] += val_C_Ia_1;
      
      if((++index_array_of_one_value_Ia) == NUMBER_POINT)
      {
        index_array_of_one_value_Ia = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_Ia] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_Ia] = 0;

        //������ ����� ����������� �� �������
        static uint32_t index_array_of_one_value_Ia_1s;
        
        vref_adc_averange_sum_1s[I_Ia] += vref_adc_period;
        vref_adc_averange_sum_1s[I_Ia] -= vref_adc_moment_value_1s[I_Ia][index_array_of_one_value_Ia_1s];
        vref_adc_moment_value_1s[I_Ia][index_array_of_one_value_Ia_1s] = vref_adc_period;
        vref_adc[I_Ia] = vref_adc_averange_sum_1s[I_Ia] / MAIN_FREQUENCY;
        
        if((++index_array_of_one_value_Ia_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_Ia_1s = 0;
        else if (index_array_of_one_value_Ia_1s > MAIN_FREQUENCY)
        {
          //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
          total_error_sw_fixed(106);
        }
      }
      else if (index_array_of_one_value_Ia > NUMBER_POINT)
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(105);
      }

      _y2 = val_C_Ia_1 - /*gnd_adc - */ vref_adc[I_Ia];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ia_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Ia])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ia_16].value - /*gnd_adc - */ vref_adc[I_Ia];

        _x2 = output_adc[C_Ia_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Ia])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);

      ADCs_data[I_Ia] = _y;
      
      ADCs_data_raw[I_Ia].tick = _x2;
      ADCs_data_raw[I_Ia].value = _y2;
    }
    /*****/

    /*****/
    //������� �������� Ib/I0.4
    /*****/
    if ((command_word & (1 << I_Ib_I04)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ib_I04].tick;
      _y1 = ADCs_data_raw[I_Ib_I04].value;
        
      static uint32_t index_array_of_one_value_Ib_I04;
      
      uint32_t val_C_Ib_I04_1 = output_adc[C_Ib_1].value;
      vref_adc_averange_sum[I_Ib_I04] += val_C_Ib_I04_1;
      
      if((++index_array_of_one_value_Ib_I04) == NUMBER_POINT)
      {
        index_array_of_one_value_Ib_I04 = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_Ib_I04] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_Ib_I04] = 0;

        //������ ����� ����������� �� �������
        static uint32_t index_array_of_one_value_Ib_I04_1s;
        
        vref_adc_averange_sum_1s[I_Ib_I04] += vref_adc_period;
        vref_adc_averange_sum_1s[I_Ib_I04] -= vref_adc_moment_value_1s[I_Ib_I04][index_array_of_one_value_Ib_I04_1s];
        vref_adc_moment_value_1s[I_Ib_I04][index_array_of_one_value_Ib_I04_1s] = vref_adc_period;
        vref_adc[I_Ib_I04] = vref_adc_averange_sum_1s[I_Ib_I04] / MAIN_FREQUENCY;
        
        if((++index_array_of_one_value_Ib_I04_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_Ib_I04_1s = 0;
        else if (index_array_of_one_value_Ib_I04_1s > MAIN_FREQUENCY)
        {
          //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
          total_error_sw_fixed(108);
        }
      }
      else if (index_array_of_one_value_Ib_I04 > NUMBER_POINT)
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(107);
      }

      _y2 = val_C_Ib_I04_1 - /*gnd_adc - */ vref_adc[I_Ib_I04];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ib_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Ib_I04])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ib_16].value - /*gnd_adc - */ vref_adc[I_Ib_I04];

        _x2 = output_adc[C_Ib_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Ib_I04])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);

      ADCs_data[I_Ib_I04] = _y;
      
      ADCs_data_raw[I_Ib_I04].tick = _x2;
      ADCs_data_raw[I_Ib_I04].value = _y2;
    }
    /*****/
    
    /*****/
    //������� �������� Ic
    /*****/
    if ((command_word & (1 << I_Ic)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ic].tick;
      _y1 = ADCs_data_raw[I_Ic].value;
        
      static uint32_t index_array_of_one_value_Ic;
      
      uint32_t val_C_Ic_1 = output_adc[C_Ic_1].value;
      vref_adc_averange_sum[I_Ic] += val_C_Ic_1;
      
      if((++index_array_of_one_value_Ic) == NUMBER_POINT)
      {
        index_array_of_one_value_Ic = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_Ic] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_Ic] = 0;

        //������ ����� ����������� �� �������
        static uint32_t index_array_of_one_value_Ic_1s;
        
        vref_adc_averange_sum_1s[I_Ic] += vref_adc_period;
        vref_adc_averange_sum_1s[I_Ic] -= vref_adc_moment_value_1s[I_Ic][index_array_of_one_value_Ic_1s];
        vref_adc_moment_value_1s[I_Ic][index_array_of_one_value_Ic_1s] = vref_adc_period;
        vref_adc[I_Ic] = vref_adc_averange_sum_1s[I_Ic] / MAIN_FREQUENCY;
        
        if((++index_array_of_one_value_Ic_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_Ic_1s = 0;
        else if (index_array_of_one_value_Ic_1s > MAIN_FREQUENCY)
        {
          //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
          total_error_sw_fixed(110);
        }
      }
      else if (index_array_of_one_value_Ic > NUMBER_POINT)
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(109);
      }

      _y2 = val_C_Ic_1 - /*gnd_adc - */ vref_adc[I_Ic];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ic_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Ic])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ic_16].value - /*gnd_adc - */ vref_adc[I_Ic];

        _x2 = output_adc[C_Ic_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Ic])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);

      ADCs_data[I_Ic] = _y;
      
      ADCs_data_raw[I_Ic].tick = _x2;
      ADCs_data_raw[I_Ic].value = _y2;
    }
    /*****/
    
//    gnd_adc  = gnd_adc2; 
//    vref_adc = vref_adc2; 

    /*****/
    //������� �������� 3U0
    /*****/
    if ((command_word & (1 << I_3U0)) != 0)
    {
      _x1 = ADCs_data_raw[I_3U0].tick;
      _y1 = ADCs_data_raw[I_3U0].value;
        
      static uint32_t index_array_of_one_value_3U0;
      
      uint32_t val_C_3U0_1 = output_adc[C_3U0_1].value;
      vref_adc_averange_sum[I_3U0] += val_C_3U0_1;
      
      if((++index_array_of_one_value_3U0) == NUMBER_POINT)
      {
        index_array_of_one_value_3U0 = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_3U0] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_3U0] = 0;

        //������ ����� ����������� �� �������
        static uint32_t index_array_of_one_value_3U0_1s;
        
        vref_adc_averange_sum_1s[I_3U0] += vref_adc_period;
        vref_adc_averange_sum_1s[I_3U0] -= vref_adc_moment_value_1s[I_3U0][index_array_of_one_value_3U0_1s];
        vref_adc_moment_value_1s[I_3U0][index_array_of_one_value_3U0_1s] = vref_adc_period;
        vref_adc[I_3U0] = vref_adc_averange_sum_1s[I_3U0] / MAIN_FREQUENCY;
        
        if((++index_array_of_one_value_3U0_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_3U0_1s = 0;
        else if (index_array_of_one_value_3U0_1s > MAIN_FREQUENCY)
        {
          //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
          total_error_sw_fixed(112);
        }
      }
      else if (index_array_of_one_value_3U0 > NUMBER_POINT)
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(111);
      }

      _y2 = val_C_3U0_1 - /*gnd_adc - */ vref_adc[I_3U0];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_3U0_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_3U0])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_3U0_16].value - /*gnd_adc - */ vref_adc[I_3U0];

        _x2 = output_adc[C_3U0_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_3U0])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);

      if ((_y >= 0) && (ADCs_data[I_3U0] < 0))
      {
        /*
        ������������ ������� ����� ����
        */
        //��������� �������
        perechid_cherez_nul[INDEX_PhK_3U0][0].x1 = poperednij_perechid.U0_x1;
        perechid_cherez_nul[INDEX_PhK_3U0][0].y1 = poperednij_perechid.U0_y1;
        perechid_cherez_nul[INDEX_PhK_3U0][0].x2 = poperednij_perechid.U0_x2;
        perechid_cherez_nul[INDEX_PhK_3U0][0].y2 = poperednij_perechid.U0_y2;
          
        //�������� �������
        poperednij_perechid.U0_x1 = perechid_cherez_nul[INDEX_PhK_3U0][1].x1 = penultimate_tick_DATA_VAL;
        poperednij_perechid.U0_y1 = perechid_cherez_nul[INDEX_PhK_3U0][1].y1 = ADCs_data[I_3U0];
        poperednij_perechid.U0_x2 = perechid_cherez_nul[INDEX_PhK_3U0][1].x2 = _x;
        poperednij_perechid.U0_y2 = perechid_cherez_nul[INDEX_PhK_3U0][1].y2 = _y;
          
        //�������, �� ������� ������������
        fix_perechid_cherez_nul[INDEX_PhK_3U0] = 0xff;
      }
      ADCs_data[I_3U0] = _y;
      
      ADCs_data_raw[I_3U0].tick = _x2;
      ADCs_data_raw[I_3U0].value = _y2;
    }
    /*****/

    /*****/
    //������� �������� Ua
    /*****/
    if ((command_word & (1 << I_Ua)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ua].tick;
      _y1 = ADCs_data_raw[I_Ua].value;
        
      static uint32_t index_array_of_one_value_Ua;
      
      uint32_t val_C_Ua_1 = output_adc[C_Ua_1].value;
      vref_adc_averange_sum[I_Ua] += val_C_Ua_1;
      
      if((++index_array_of_one_value_Ua) == NUMBER_POINT)
      {
        index_array_of_one_value_Ua = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_Ua] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_Ua] = 0;

        //������ ����� ����������� �� �������
        static uint32_t index_array_of_one_value_Ua_1s;
        
        vref_adc_averange_sum_1s[I_Ua] += vref_adc_period;
        vref_adc_averange_sum_1s[I_Ua] -= vref_adc_moment_value_1s[I_Ua][index_array_of_one_value_Ua_1s];
        vref_adc_moment_value_1s[I_Ua][index_array_of_one_value_Ua_1s] = vref_adc_period;
        vref_adc[I_Ua] = vref_adc_averange_sum_1s[I_Ua] / MAIN_FREQUENCY;
        
        if((++index_array_of_one_value_Ua_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_Ua_1s = 0;
        else if (index_array_of_one_value_Ua_1s > MAIN_FREQUENCY)
        {
          //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
          total_error_sw_fixed(114);
        }
      }
      else if (index_array_of_one_value_Ua > NUMBER_POINT)
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(113);
      }

      _y2 = val_C_Ua_1 - /*gnd_adc - */ vref_adc[I_Ua];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ua_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Ua])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ua_16].value - /*gnd_adc - */ vref_adc[I_Ua];

        _x2 = output_adc[C_Ua_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Ua])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);
      
      if ((_y >= 0) && (ADCs_data[I_Ua] < 0))
      {
        /*
        ������������ ������� ����� ����
        */
        //��������� �������
        perechid_cherez_nul[INDEX_PhK_UA][0].x1 = poperednij_perechid.Ua_x1;
        perechid_cherez_nul[INDEX_PhK_UA][0].y1 = poperednij_perechid.Ua_y1;
        perechid_cherez_nul[INDEX_PhK_UA][0].x2 = poperednij_perechid.Ua_x2;
        perechid_cherez_nul[INDEX_PhK_UA][0].y2 = poperednij_perechid.Ua_y2;
          
        //�������� �������
        poperednij_perechid.Ua_x1 = perechid_cherez_nul[INDEX_PhK_UA][1].x1 = penultimate_tick_DATA_VAL;
        poperednij_perechid.Ua_y1 = perechid_cherez_nul[INDEX_PhK_UA][1].y1 = ADCs_data[I_Ua];
        poperednij_perechid.Ua_x2 = perechid_cherez_nul[INDEX_PhK_UA][1].x2 = _x;
        poperednij_perechid.Ua_y2 = perechid_cherez_nul[INDEX_PhK_UA][1].y2 = _y;
          
        //�������, �� ������� ������������
        fix_perechid_cherez_nul[INDEX_PhK_UA] = 0xff;
      }
      ADCs_data[I_Ua] = _y;
      
      ADCs_data_raw[I_Ua].tick = _x2;
      ADCs_data_raw[I_Ua].value = _y2;
    }
    /*****/
    
    /*****/
    //������� �������� Ub
    /*****/
    if ((command_word & (1 << I_Ub)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ub].tick;
      _y1 = ADCs_data_raw[I_Ub].value;
        
      static uint32_t index_array_of_one_value_Ub;
      
      uint32_t val_C_Ub_1 = output_adc[C_Ub_1].value;
      vref_adc_averange_sum[I_Ub] += val_C_Ub_1;
      
      if((++index_array_of_one_value_Ub) == NUMBER_POINT)
      {
        index_array_of_one_value_Ub = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_Ub] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_Ub] = 0;

        //������ ����� ����������� �� �������
        static uint32_t index_array_of_one_value_Ub_1s;
        
        vref_adc_averange_sum_1s[I_Ub] += vref_adc_period;
        vref_adc_averange_sum_1s[I_Ub] -= vref_adc_moment_value_1s[I_Ub][index_array_of_one_value_Ub_1s];
        vref_adc_moment_value_1s[I_Ub][index_array_of_one_value_Ub_1s] = vref_adc_period;
        vref_adc[I_Ub] = vref_adc_averange_sum_1s[I_Ub] / MAIN_FREQUENCY;
        
        if((++index_array_of_one_value_Ub_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_Ub_1s = 0;
        else if (index_array_of_one_value_Ub_1s > MAIN_FREQUENCY)
        {
          //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
          total_error_sw_fixed(116);
        }
      }
      else if (index_array_of_one_value_Ub > NUMBER_POINT)
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(115);
      }

      _y2 = val_C_Ub_1 - /*gnd_adc - */ vref_adc[I_Ub];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ub_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Ub])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ub_16].value - /*gnd_adc - */ vref_adc[I_Ub];

        _x2 = output_adc[C_Ub_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Ub])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);
 
      if ((_y >= 0) && (ADCs_data[I_Ub] < 0))
      {
        /*
        ������������ ������� ����� ����
        */
        //��������� �������
        perechid_cherez_nul[INDEX_PhK_UB][0].x1 = poperednij_perechid.Ub_x1;
        perechid_cherez_nul[INDEX_PhK_UB][0].y1 = poperednij_perechid.Ub_y1;
        perechid_cherez_nul[INDEX_PhK_UB][0].x2 = poperednij_perechid.Ub_x2;
        perechid_cherez_nul[INDEX_PhK_UB][0].y2 = poperednij_perechid.Ub_y2;
          
        //�������� �������
        poperednij_perechid.Ub_x1 = perechid_cherez_nul[INDEX_PhK_UB][1].x1 = penultimate_tick_DATA_VAL;
        poperednij_perechid.Ub_y1 = perechid_cherez_nul[INDEX_PhK_UB][1].y1 = ADCs_data[I_Ub];
        poperednij_perechid.Ub_x2 = perechid_cherez_nul[INDEX_PhK_UB][1].x2 = _x;
        poperednij_perechid.Ub_y2 = perechid_cherez_nul[INDEX_PhK_UB][1].y2 = _y;
          
        //�������, �� ������� ������������
        fix_perechid_cherez_nul[INDEX_PhK_UB] = 0xff;
      }
      ADCs_data[I_Ub] = _y;
      
      ADCs_data_raw[I_Ub].tick = _x2;
      ADCs_data_raw[I_Ub].value = _y2;
    }
    /*****/
    
    /*****/
    //������� �������� Uc
    /*****/
    if ((command_word & (1 << I_Uc)) != 0)
    {
      _x1 = ADCs_data_raw[I_Uc].tick;
      _y1 = ADCs_data_raw[I_Uc].value;
        
      static uint32_t index_array_of_one_value_Uc;
      
      uint32_t val_C_Uc_1 = output_adc[C_Uc_1].value;
      vref_adc_averange_sum[I_Uc] += val_C_Uc_1;
      
      if((++index_array_of_one_value_Uc) == NUMBER_POINT)
      {
        index_array_of_one_value_Uc = 0;
        uint32_t vref_adc_period = vref_adc_averange_sum[I_Uc] >> VAGA_NUMBER_POINT;
        vref_adc_averange_sum[I_Uc] = 0;

        //������ ����� ����������� �� �������
        static uint32_t index_array_of_one_value_Uc_1s;
        
        vref_adc_averange_sum_1s[I_Uc] += vref_adc_period;
        vref_adc_averange_sum_1s[I_Uc] -= vref_adc_moment_value_1s[I_Uc][index_array_of_one_value_Uc_1s];
        vref_adc_moment_value_1s[I_Uc][index_array_of_one_value_Uc_1s] = vref_adc_period;
        vref_adc[I_Uc] = vref_adc_averange_sum_1s[I_Uc] / MAIN_FREQUENCY;
        
        if((++index_array_of_one_value_Uc_1s) == MAIN_FREQUENCY)
          index_array_of_one_value_Uc_1s = 0;
        else if (index_array_of_one_value_Uc_1s > MAIN_FREQUENCY)
        {
          //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
          total_error_sw_fixed(118);
        }
      }
      else if (index_array_of_one_value_Uc > NUMBER_POINT)
      {
        //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
        total_error_sw_fixed(117);
      }

      _y2 = val_C_Uc_1 - /*gnd_adc - */ vref_adc[I_Uc];
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Uc_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Uc])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Uc_16].value - /*gnd_adc - */ vref_adc[I_Uc];

        _x2 = output_adc[C_Uc_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Uc])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);

      if ((_y >= 0) && (ADCs_data[I_Uc] < 0))
      {
        /*
        ������������ ������� ����� ����
        */
        //��������� �������
        perechid_cherez_nul[INDEX_PhK_UC][0].x1 = poperednij_perechid.Uc_x1;
        perechid_cherez_nul[INDEX_PhK_UC][0].y1 = poperednij_perechid.Uc_y1;
        perechid_cherez_nul[INDEX_PhK_UC][0].x2 = poperednij_perechid.Uc_x2;
        perechid_cherez_nul[INDEX_PhK_UC][0].y2 = poperednij_perechid.Uc_y2;
          
        //�������� �������
        poperednij_perechid.Uc_x1 = perechid_cherez_nul[INDEX_PhK_UC][1].x1 = penultimate_tick_DATA_VAL;
        poperednij_perechid.Uc_y1 = perechid_cherez_nul[INDEX_PhK_UC][1].y1 = ADCs_data[I_Uc];
        poperednij_perechid.Uc_x2 = perechid_cherez_nul[INDEX_PhK_UC][1].x2 = _x;
        poperednij_perechid.Uc_y2 = perechid_cherez_nul[INDEX_PhK_UC][1].y2 = _y;
          
        //�������, �� ������� ������������
        fix_perechid_cherez_nul[INDEX_PhK_UC] = 0xff;
      }
      ADCs_data[I_Uc] = _y;
      
      ADCs_data_raw[I_Uc].tick = _x2;
      ADCs_data_raw[I_Uc].value = _y2;
    }
    /*****/
    
    unsigned int head_data_for_oscylograph_tmp = head_data_for_oscylograph;
    unsigned int x2, x1, delta_x; 

    if ((status_adc_read_work & DATA_VAL_READ) != 0)
    {
      /*
      ��������� ���������� ��������� ��� ��� ������������ ���'�
      */
      Fourier();
      
      //������� ��� ��� ��������� �������
      x1 = rozshyrena_vyborka.time_p = penultimate_tick_DATA_VAL;
      x2 = rozshyrena_vyborka.time_c = previous_tick_DATA_VAL;
      for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++) 
      {
        rozshyrena_vyborka.data_p[i] = rozshyrena_vyborka.data_c[i];
        rozshyrena_vyborka.data_c[i] = ADCs_data[i];
      }

      /*******************************************************
      ���������� �������������� �������
      *******************************************************/
      if (head_data_for_oscylograph_tmp != DATA_VAL_tail_data_for_oscylograph)
      {
        if (x2 > x1) delta_x = x2 - x1;
        else
        {
          long long delta_x_64 = x2 + 0x100000000 - x1;
          delta_x = delta_x_64;
        }

        while (head_data_for_oscylograph_tmp != DATA_VAL_tail_data_for_oscylograph)
        {
          unsigned int DATA_VAL_tail_data_for_oscylograph_tmp = DATA_VAL_tail_data_for_oscylograph;
    
          unsigned int x = data_for_oscylograph[DATA_VAL_tail_data_for_oscylograph_tmp].time_stemp;
        
          unsigned int dx;
          if (x >= x1) dx = x - x1;
          else
          {
            long long dx_64 = x + 0x100000000 - x1;
            dx = dx_64;
          }
          
          if (dx > delta_x) 
          {
            break;
          }

          for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
          {
            int y1 = rozshyrena_vyborka.data_p[i], y2 = rozshyrena_vyborka.data_c[i];
            long long y;
            if (dx <= delta_x)
            {
              y = ((long long)(y2 - y1))*((long long)dx)/((long long)delta_x) + ((long long)y1);
            }
            else
            {
              y = 0;
            }
            data_for_oscylograph[DATA_VAL_tail_data_for_oscylograph_tmp].data[I_3I0 + i] = y;
          }
          data_for_oscylograph[DATA_VAL_tail_data_for_oscylograph_tmp].DATA_fix = 0xff;

          if (++DATA_VAL_tail_data_for_oscylograph >= MAX_INDEX_DATA_FOR_OSCYLOGRAPH) DATA_VAL_tail_data_for_oscylograph = 0;
        }
      }
      /******************************************************/
      
      /*
      �������� �������� �� ���������� ������� � ��������� �������
      */
      fapch();
    
      status_adc_read_work &= (unsigned int)(~DATA_VAL_READ);

      /**************************************************/
      //����������� ����������� ��� ���������� ������ ����� ����� ������������ �������
      /**************************************************/
      control_word_of_watchdog |= WATCHDOG_MEASURE_STOP_DATA_VAL;
      /**************************************************/
    }
    
    /********************************************************
    ������� ����� ������� ������� � �������� �������� ��� ����������� ����������
    ********************************************************/
    unsigned int number_postfault_slices = 0;
    if (head_data_for_oscylograph_tmp != tail_data_for_oscylograph)
    {
      while (
             (head_data_for_oscylograph_tmp != tail_data_for_oscylograph) &&
             (data_for_oscylograph[tail_data_for_oscylograph].DATA_fix != 0)
            )
      {
        unsigned int tail_data_for_oscylograph_tmp = tail_data_for_oscylograph;
        for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
        {
          int data_tmp = data_for_oscylograph[tail_data_for_oscylograph_tmp].data[i];
          
          
          if((prescaler_ar & MASKA_BIT_FOR_PRESCALER) == 0)
          {
            //����� ������� ���������� ������� ��� ����������� ����������
//            array_ar[index_array_ar_current++] = data_tmp;
            AR_WRITE(index_array_ar_current, data_tmp);
          }
        }

        //����� ���������� ������� ��� ����������� ����������
        unsigned int *label_to_active_functions_source = data_for_oscylograph[tail_data_for_oscylograph_tmp].active_functions;
        if((prescaler_ar & MASKA_BIT_FOR_PRESCALER) != 0)
        {
          for (unsigned int i = 0; i < N_BIG; i++)  active_functions_trg[i] = *(label_to_active_functions_source + i);
        }
        else
        {
          for (unsigned int i = 0; i < N_BIG; i++)  active_functions_trg[i] |= *(label_to_active_functions_source + i);

          unsigned short int *label_to_active_functions_trg = (unsigned short int*)active_functions_trg;
          for(unsigned int i = 0; i < number_word_digital_part_ar; i++) 
          {
//            array_ar[index_array_ar_current++] = *(label_to_active_functions_trg + i);
            AR_WRITE(index_array_ar_current, *(label_to_active_functions_trg + i));
          }
          //������ ������ ��'������� ������� ��� ����������� ����������
          if (index_array_ar_current >= SIZE_BUFFER_FOR_AR) index_array_ar_current = 0;/*����� ���� � ���� ==, ��� ��� ��������������� �� ����������� ������� � �������� >=*/
          
          //������������ ������� ���� ������� � ������ ���������� �� ������������� ������ ��'������� ������� ��� ����������� ����������
          if (
              (data_for_oscylograph[tail_data_for_oscylograph_tmp].state_ar_record == STATE_AR_START) ||
              (data_for_oscylograph[tail_data_for_oscylograph_tmp].state_ar_record == STATE_AR_SAVE_SRAM_AND_SAVE_FLASH)
             )
          {
            number_postfault_slices++;
          }
        }
        prescaler_ar++;
    
        data_for_oscylograph[tail_data_for_oscylograph_tmp].DATA_fix = 0;

        if (++tail_data_for_oscylograph >= MAX_INDEX_DATA_FOR_OSCYLOGRAPH) tail_data_for_oscylograph = 0;
      }
    }
//    /**************************************************/
//    //��� ����������� ����������� ��� ����� � ���������� ������� �������
//    /**************************************************/
//    if (wait_of_receiving_current_data  == true) wait_of_receiving_current_data  = false;
//    /**************************************************/

    //��������� ���������� ������������
    if (
        ((state_ar_record == STATE_AR_START) && (number_postfault_slices != 0)) || 
        (state_ar_record == STATE_AR_SAVE_SRAM_AND_SAVE_FLASH)
       )
    {
      static unsigned int uncopied_postfault_time_sapmles;

      /*
      ��� ����� ��� ������������� ������ � ������� ������� � �����
      */
        
      if (state_ar_record == STATE_AR_START)
      {
        int difference;
        /*
        ������� number_postfault_slices ������������ ���� ������� � �����,
        �� ��� ���������� ����� ���� ������������� ������ �� index_array_ar_current
        ������ ������� ������� ������� � ������ ���
        */
        /*
        ������������ ���� ������� �������� �������� ������������ ������ � 
        ��������� �������� �� � ����� "��������" ��� ����, ��� ������ �������� 
        ���� "������������", ����� �������, ������
        */
        difference = index_array_ar_current - number_postfault_slices*(NUMBER_ANALOG_CANALES + number_word_digital_part_ar);
        if (difference >= 0) index_array_ar_heat = difference;
        else index_array_ar_heat = difference + SIZE_BUFFER_FOR_AR;

        //������������ ���� "��������"
        difference = index_array_ar_heat - (current_settings_prt.prefault_number_periods << VAGA_NUMBER_POINT_AR)*(NUMBER_ANALOG_CANALES + number_word_digital_part_ar);
        if (difference >= 0) index_array_ar_tail = difference;
        else index_array_ar_tail = difference + SIZE_BUFFER_FOR_AR;
          
        //��������� ������ ����� ���� �������� ��� ������������� ������
        /*
        !!!� ������ ���� ������� ���� � �� ������� ������� �������, ��� �����
        ���� ������� � ����� ���� ��� �������� �� �������� � ������ � ������� ������,
        � ������� �� �������� �� �������!!!
        */
        uncopied_postfault_time_sapmles = (current_settings_prt.postfault_number_periods << VAGA_NUMBER_POINT_AR);

        /*
        ��� �� ���� ��������, �� ����������� �� ������� �� �� ������� ���� �����
        ����, �� �� ����� ������ �� ������������� ������ - �� ������� �� ����� ����, �� ���� ��� 
        ����� ���� ������� � ���������� ���������.
        */
        unsigned int tail_data_for_oscylograph_tmp = tail_data_for_oscylograph;
        while (head_data_for_oscylograph_tmp != tail_data_for_oscylograph_tmp)
        {
           data_for_oscylograph[tail_data_for_oscylograph_tmp++].state_ar_record = state_ar_record;
           if (tail_data_for_oscylograph_tmp >= MAX_INDEX_DATA_FOR_OSCYLOGRAPH) tail_data_for_oscylograph_tmp = 0;
        }

        //���������� � ���� ������ ����������� ���������� "�������� � ��������"
        state_ar_record = STATE_AR_SAVE_SRAM_AND_SAVE_FLASH;

      }
        
      /*
      ������������ ���� "��������" � ���������� ������������ ������ � ����� 
      ��������, �� ���� �������� ���������� �� �������, �� �� ����� ���� � 
      ����� index_array_ar_heat ����������� �������� ���� "��������" �� ��������� 
      number_postfault_slices ����. 
      ��� ������ ��� ������ number_postfault_slices ���� � �����
      index_array_ar_current ������� � ������� ���� "����������" ������ ������� 
      ������� ��� ����������� ����������.
      */

      /*
      �������� ��������� �������� ������������ ������ (����������� �� ������� ����� �
      �� ���� ��������, ���� � ��������� ������ ����� ������ � ������������ 
      ������������� �������� ����������� � ������������� ������ � ����������
      "��������" � "��������")
      */
      int difference_before, difference_after;
      unsigned int index_array_ar_tail_tmp = index_array_ar_tail;
      difference_before = (index_array_ar_heat - index_array_ar_tail_tmp);
      if (difference_before < 0) difference_before += SIZE_BUFFER_FOR_AR;
      difference_after = (index_array_ar_current - index_array_ar_tail_tmp);
      if (difference_after < 0) difference_after += SIZE_BUFFER_FOR_AR;
      if ((difference_after - difference_before) != number_postfault_slices*(NUMBER_ANALOG_CANALES + number_word_digital_part_ar))
      {
        //��������� ��������, ��� ��������� ������������� 
        _SET_BIT(set_diagnostyka, ERROR_AR_OVERLOAD_BUFFER_BIT);
      }
      else
      {
        //���� ��������� ��������, ��� ��������� �������������
        _SET_BIT(clear_diagnostyka, ERROR_AR_OVERLOAD_BUFFER_BIT);
      }

      //������������ ���� "��������" � ������� ����, �� �� ����� �������� �� ������������� ������
      if (uncopied_postfault_time_sapmles >= number_postfault_slices)
      {
        index_array_ar_heat = index_array_ar_current;
        uncopied_postfault_time_sapmles -= number_postfault_slices;
      }
      else
      {
        int difference = index_array_ar_current - (number_postfault_slices - uncopied_postfault_time_sapmles);
        if (difference >= 0) index_array_ar_heat = difference;
        else index_array_ar_heat = difference + SIZE_BUFFER_FOR_AR;

        uncopied_postfault_time_sapmles = 0;
      }

      if (uncopied_postfault_time_sapmles == 0)
      {
        /*
        �� ���������� ���� ��� �������� � ���� ���������� ����� ������ 
        ����������� ���������� � ����� ����� ������ ��� ������� �� �� �� �������
        */
        state_ar_record = STATE_AR_ONLY_SAVE_FLASH;

        /*
        ��� �� ���� ��������, �� ����������� �� ������� ��� ������� ��� �����
        ����, �� ����� ������ �� ������������� ������, ��� �� ���� �� ��������,
        �� �� ���� ��������� ��� - �� ������� �� ����� ����, �� ���� ��� 
        �������� �� ������������ �����.
        */
        unsigned int tail_data_for_oscylograph_tmp = tail_data_for_oscylograph;
        while (head_data_for_oscylograph_tmp != tail_data_for_oscylograph_tmp)
        {
           data_for_oscylograph[tail_data_for_oscylograph_tmp++].state_ar_record = state_ar_record;
           if (tail_data_for_oscylograph_tmp >= MAX_INDEX_DATA_FOR_OSCYLOGRAPH) tail_data_for_oscylograph_tmp = 0;
        }
      }
        
    }
    /*******************************************************/
    
//    if ((status_adc_read_work & TEST_VAL_READ) != 0)
    {
      //�������� �������� � ����������� �������� ����� �� ��� �������
      uint32_t min_vref_adc = vref_adc[0];
      uint32_t max_vref_adc = min_vref_adc;
  
      for (size_t i = 1; i < NUMBER_ANALOG_CANALES; i++)
      {
        if (min_vref_adc > vref_adc[i]) min_vref_adc = vref_adc[i];
        if (max_vref_adc < vref_adc[i]) max_vref_adc = vref_adc[i];
      }

      //���������� ����������� ��������
      if ((min_vref_adc < 0x709) || (max_vref_adc > 0x8f5)) _SET_BIT(set_diagnostyka, ERROR_VREF_ADC_TEST_BIT);
      else _SET_BIT(clear_diagnostyka,ERROR_VREF_ADC_TEST_BIT);
    
//      status_adc_read_work &= (unsigned int)(~TEST_VAL_READ);

//      /**************************************************/
//      //����������� ����������� ��� ���������� �������� �������
//      /**************************************************/
//      control_word_of_watchdog |= WATCHDOG_MEASURE_STOP_TEST_VAL;
//      /**************************************************/
      
      /**************************************************/
      //���� ����� ����� ���������� �� ���� ������
      /**************************************************/
      if (_CHECK_SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT) != 0)
      {
        //����� ���������������� ����� � ����������� ��� �������� I2C
        _CLEAR_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
      }
      /**************************************************/
    }
  
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
  }

  if ((GPIO_SPI_ADC->ODR & GPIO_NSSPin_ADC) != 0)
  {
    //����� ���� �� ������� �������� control_reading_ADCs
    
    /*
    ������� � ���� ���������� ������� ������� control_reading_ADCs �� ������
    ������� ����������� �� ������� ��������� ��� ������ �� ���������
    */
    
    /*
    ����������� ��������� ����������, ��� ���� �� �������� ��� ���� ����� �����
    �� ��������� �� ����� �� ���������
    */
    __disable_interrupt();
    
    /*
    ��������, �� ���� ����� ����� �� ��������� � ���� �� ������ ������� �����
    ����� �� ���������, �� ������������ ��������� ������� ��������� � �����������
    ������� ( chip select ����������� � 1)
    */
    if (
        (adc_DATA_VAL_read == false) /*&&
        (adc_TEST_VAL_read == false)*/
       )
    {
      semaphore_adc_irq  = false;
    }
    
    /*
    ���������� ��������� ����������
    */
     __enable_interrupt();
  }
  else semaphore_adc_irq  = false;

  
  if (semaphore_adc_irq  != false)
  {
    /*
    �� ����� ���� ���� ������ � ������ �������: ���� ��� ����� �� ��� ��� �� ���������
    � ������� ��������� ������� ������� control_reading_ADCs �� �������� ����������
    �������� __disable_interrupt ��� ������� ���������.
    
    ������ � ���� ������� ������� control_reading_ADCs. �������� �� ����, ��
    chip_select ������ ����������� � ������� ���� (�� �� ������� �������� 
    if ((GPIO_SPI_ADC->ODR & GPIO_NSSPin_ADC) != 0)) � ����� semaphore_adc_irq ��
    �� ���� false, � ��� ����� ����� ������ ��������� � ����������� �������
    */
    
    control_reading_ADCs();
    
    /*
    ������� ���������. ���� �������� �� �� � ���������� ������� ����������� �����, �� ������� �� SPI
    ����������. ��� ��� ��������� ��� ����� �� �����, �� ������ ������� control_reading_ADCs
    ��� ����������� ����, �� ��� �� ��������� "������ � ����"  ������ ������
    ��������� - � �� ������� ����� ���, ���� chip_select ���� �������� ���� ������������ � ������� �����
    
    � ������������ chip_select ����� � ������� ����� ����� ���� ����� ����������� ����������� ��
    ����� SPI
    */
    semaphore_adc_irq  = false;
  }
  
//#ifdef SYSTEM_VIEWER_ENABLE
//  SEGGER_SYSVIEW_RecordExitISR();
//#endif
}
/*****************************************************/

/*****************************************************/
//����������� ����
/*****************************************************/
void calc_angle(void) 
{
  //������� ����������
  semaphore_measure_values_low1 = 1;
  for (unsigned int i = 0; i < _NUMBER_IM; i++ ) 
  {
    measurement_low[i] = measurement_middle[i];
  }
  semaphore_measure_values_low1 = 0;
              
  int ortogonal_low_tmp[2*FULL_ORT_MAX];
  
  //����������� ������� �������� ���������� ������� � ������� �������
  semaphore_measure_values_low = 1;
  for(unsigned int i = 0; i < (2*FULL_ORT_MAX); i++ )
  {
      ortogonal_low_tmp[i] = ortogonal_calc_low[i];
  } 
  //������ ������� �������� ���������� ������� � ������� �������
  semaphore_measure_values_low = 0;
  
  state_calc_phi_angle = true;
  bank_for_calc_phi_angle = (bank_for_calc_phi_angle ^ 0x1) & 0x1;

  //���������, ���� ������ ������ �� ������
  __full_ort_index index_base;
  if ((current_settings.control_extra_settings_1 & CTR_EXTRA_SETTINGS_1_CTRL_PHASE_LINE) == 0) index_base = FULL_ORT_Ua;
  else 
  {
    index_base = FULL_ORT_Uab;
    
    //� ����� ������� ���� �� ������� ��������� �����������
    phi_angle[bank_for_calc_phi_angle][FULL_ORT_Uc] = phi_angle[bank_for_calc_phi_angle][FULL_ORT_Ub] = phi_angle[bank_for_calc_phi_angle][FULL_ORT_Ua] = -1;
  }

  /***
  ���������, ���� ������� ������ ����� ����� �� ������
  ***/
  __full_ort_index index_last = FULL_ORT_3U0;
  /***/
  
  int base_index_for_angle_tmp = -1;
  __full_ort_index index = index_base;
  while( index <= index_last)
  {
    unsigned int index_m;
    switch (index)
    {
    case FULL_ORT_Ua:
      {
        index_m = IM_UA;
        break;
      }
    case FULL_ORT_Ub:
      {
        index_m = IM_UB;
        break;
      }
    case FULL_ORT_Uc:
      {
        index_m = IM_UC;
        break;
      }
    case FULL_ORT_Uab:
      {
        index_m = IM_UAB;
        break;
      }
    case FULL_ORT_Ubc:
      {
        index_m = IM_UBC;
        break;
      }
    case FULL_ORT_Uca:
      {
        index_m = IM_UCA;
        break;
      }
    case FULL_ORT_3U0:
      {
        index_m = IM_3U0;
        break;
      }
    default:
      {
        //���������� ����� ����� �� ���� � ����
        total_error_sw_fixed(61);
      }
    }
  
    if (measurement_low[index_m] >= PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE)
    {
      base_index_for_angle_tmp = index;
      break; //����� �� ����� while
    }
    index++;
  }
  base_index_for_angle = base_index_for_angle_tmp;
  
  if (base_index_for_angle_tmp >= 0)
  {
    //���������� �������� �������, ������� ����� ������ ������ ����
    int ortogonal_base[2] = {ortogonal_low_tmp[2*base_index_for_angle_tmp], ortogonal_low_tmp[2*base_index_for_angle_tmp + 1]};
    
#define SIN_BASE   ortogonal_base[0]
#define COS_BASE   ortogonal_base[1]

    unsigned int amplituda_base = sqrt_64((unsigned long long)((long long)SIN_BASE*(long long)SIN_BASE) + (unsigned long long)((long long)COS_BASE*(long long)COS_BASE));
    if (amplituda_base != 0)
    {
      for (__full_ort_index index_tmp = index_base; index_tmp < FULL_ORT_MAX; index_tmp++)
      {
        if (index_tmp == index)
        {
          phi_angle[bank_for_calc_phi_angle][index_tmp] = 0;
          continue;
        }
        else
        {
          unsigned int porig_chutlyvosti;
          unsigned int index_m;
          switch (index_tmp)
          {
          case FULL_ORT_Ua:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
              index_m = IM_UA;
              break;
            }
          case FULL_ORT_Ub:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
              index_m = IM_UB;
              break;
            }
          case FULL_ORT_Uc:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
              index_m = IM_UC;
              break;
            }
          case FULL_ORT_Uab:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
              index_m = IM_UAB;
              break;
            }
          case FULL_ORT_Ubc:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
              index_m = IM_UBC;
              break;
            }
          case FULL_ORT_Uca:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
              index_m = IM_UCA;
              break;
            }
          case FULL_ORT_3U0:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
              index_m = IM_3U0;
              break;
            }
          case FULL_ORT_Ia:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
              index_m = IM_IA;
              break;
            }
          case FULL_ORT_Ib:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
              index_m = IM_IB;
              break;
            }
          case FULL_ORT_Ic:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
              index_m = IM_IC;
              break;
            }
          case FULL_ORT_3I0:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
              index_m = IM_3I0;
              break;
            }
          case FULL_ORT_3I0_r:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
              index_m = IM_3I0_r;
              break;
            }
          case FULL_ORT_I04:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
              index_m = IM_I04;
              break;
            }
          default:
            {
              //���������� ����� ����� �� ���� � ����
              total_error_sw_fixed(62);
            }
          }
      
          if (measurement_low[index_m] >= porig_chutlyvosti)
          {
            //����������� ���
#define SIN_TARGET ortogonal_low_tmp[2*index_tmp]
#define COS_TARGET ortogonal_low_tmp[2*index_tmp + 1]

            unsigned int amplituda_target = sqrt_64((unsigned long long)((long long)SIN_TARGET*(long long)SIN_TARGET) + (unsigned long long)((long long)COS_TARGET*(long long)COS_TARGET));
      
            if (amplituda_target != 0)
            {
              //���������� COS � SIN ���� ������ �� ���������
              long long cos_fi = COS_TARGET*COS_BASE + SIN_TARGET*SIN_BASE;
              long long sin_fi = -(SIN_TARGET*COS_BASE - COS_TARGET*SIN_BASE);//� ������: "���������, ��� ����", ����� �� � �� ���� ��������

#undef SIN_TARGET
#undef COS_TARGET
      
              float sin_fi_f = ( ((float)sin_fi) / ((float)amplituda_target) ) / ((float)amplituda_base);
              if (sin_fi_f > 1) sin_fi_f = 1;
              else if (sin_fi_f < -1) sin_fi_f = -1;
      
              float angle_f = asin(sin_fi_f)*180 / PI;
              int angle_int = (int)(angle_f*10); //������ ������� �� �������
              if (angle_int < 0) angle_int = -angle_int;
      
              if ((sin_fi >= 0) && (cos_fi >= 0))
              {
                //1-�� ��������
                //angle_int ���������� ��� ����
              }
              else if ((sin_fi >= 0) && (cos_fi <  0))
              {
                //2-�� ��������
                angle_int = 1800 - angle_int;
              }
              else if ((sin_fi <  0) && (cos_fi <  0))
              {
                //3-�� ��������
                angle_int = 1800 + angle_int;
              }
              else
              {
                //4-�� ��������
                angle_int = 3600 - angle_int;
              }
    
              if (angle_int >= 3600) angle_int -= 3600;
              else if (angle_int < 0) angle_int += 3600;
      
              phi_angle[bank_for_calc_phi_angle][index_tmp] = angle_int;
      
            }
            else
            {
              phi_angle[bank_for_calc_phi_angle][index_tmp] = -1;
            }

          }
          else
          {
            //������ ������������� ������� ����� ������ - ��� ������������
            phi_angle[bank_for_calc_phi_angle][index_tmp] = -1;
          }
        }
      }
    }
    else
    {
      //�������� �������� ������� ���������� �� ���������� ��� ���� ������� ���� 0 (� �����, �� ���� �������� �� ���� � ����� ��������). �� �������������.
      for (__full_ort_index index_tmp = FULL_ORT_Ua; index_tmp < FULL_ORT_MAX; index_tmp++) phi_angle[bank_for_calc_phi_angle][index_tmp] = -1;
    }

#undef SIN_BASE
#undef COS_BASE
    
  }
  else
  {
    //�� ����������� ������� ����������, ������� ����� ����� ������������� ����
    for (__full_ort_index index_tmp = FULL_ORT_Ua; index_tmp < FULL_ORT_MAX; index_tmp++) phi_angle[bank_for_calc_phi_angle][index_tmp] = -1;
  }

  state_calc_phi_angle = false;
}

/*****************************************************/

/*****************************************************/
//����������� ������ ���������
/*****************************************************/
void calc_power_and_energy(void) 
{
  //��������� ���� �� ������������� ������ ��� �������
  unsigned int bank_for_enegry_tmp = (bank_for_enegry + 1) & 0x1;
  
  int P_tmp = P_plus[bank_for_enegry_tmp] - P_minus[bank_for_enegry_tmp];
  int Q_tmp = Q_1q[bank_for_enegry_tmp] + Q_2q[bank_for_enegry_tmp] - Q_3q[bank_for_enegry_tmp] - Q_4q[bank_for_enegry_tmp];

  //����������� ������
  unsigned int clean_energy_tmp = clean_energy;
  if (clean_energy_tmp != 0) 
  {
    clean_energy = 0;

    if ((POWER_CTRL->IDR & POWER_CTRL_PIN) != (uint32_t)Bit_RESET)
    {
      //��������� ����� � EEPROM
      _SET_BIT(control_spi1_taskes, TASK_START_WRITE_ENERGY_EEPROM_BIT);
    }
    else number_minutes = PERIOD_SAVE_ENERGY_IN_MINUTES; /*���� �������� ����������, ��� ����� ���� ������ ������� �� �����*/

    //�������, �� �������� ������� ��������� ������
    information_about_clean_energy |= (
                                         (1 << USB_RECUEST)
                                       | (1 << RS485_RECUEST)
#if (MODYFIKACIA_VERSII_PZ >= 10)
                                       | (1 << LAN_RECUEST)
#endif
                                      );
  }
  
  state_calc_energy = true;
  for (__index_energy i = INDEX_EA_PLUS; i < MAX_NUMBER_INDEXES_ENERGY; i++)
  {
    if (clean_energy_tmp != 0) energy[0][i] = 0;
    
    int power_data;
    switch (i)
    {
    case INDEX_EA_PLUS:
      {
        power_data = P_plus[bank_for_enegry_tmp];
        break;
      }
    case INDEX_EA_MINUS:
      {
        power_data = P_minus[bank_for_enegry_tmp];
        break;
      }
    case INDEX_ER_1:
      {
        power_data = Q_1q[bank_for_enegry_tmp];
        break;
      }
    case INDEX_ER_2:
      {
        power_data = Q_2q[bank_for_enegry_tmp];
        break;
      }
    case INDEX_ER_3:
      {
        power_data = Q_3q[bank_for_enegry_tmp];
        break;
      }
    case INDEX_ER_4:
      {
        power_data = Q_4q[bank_for_enegry_tmp];
        break;
      }
    default:
      {
        //���������� ����� ����� �� ���� � ����
        total_error_sw_fixed(65);
      }
    }
    
    if (power_data >= (PORIG_POWER_ENERGY*MAIN_FREQUENCY)) /*�� � power_data � ���� ������� ����������� �� 1�, �� ��������������� ���� 20��*/
    {
      double power_quantum = ((double)power_data)/(((double)MAIN_FREQUENCY)*DIV_kWh);
      double erergy_tmp = energy[0][i] + power_quantum;
      if (erergy_tmp > 999999.999) erergy_tmp = erergy_tmp - 999999.999;
      energy[0][i] = erergy_tmp;
    }
  }
  state_calc_energy = false;
  for (__index_energy i = INDEX_EA_PLUS; i < MAX_NUMBER_INDEXES_ENERGY; i++) energy[1][i] = energy[0][i];
  
  float P_float = ((float)P_tmp)/((float)MAIN_FREQUENCY);
  float Q_float = ((float)Q_tmp)/((float)MAIN_FREQUENCY);
  
  state_calc_power = true;
  bank_for_calc_power = (bank_for_calc_power ^ 0x1) & 0x1;
  P[bank_for_calc_power] = (int)P_float;
  Q[bank_for_calc_power] = (int)Q_float;
  
  //����� ���������
  if ( (P[bank_for_calc_power] != 0) || (Q[bank_for_calc_power] != 0))
  {
    float in_square_root, S_float;
    in_square_root = P_float*P_float + Q_float*Q_float;
    
    if (arm_sqrt_f32(in_square_root, &S_float) == ARM_MATH_SUCCESS)
    {
      S[bank_for_calc_power] = (unsigned int)S_float;
    }
    else
    {
      //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
      total_error_sw_fixed(53);
    }
    
    cos_phi_x1000[bank_for_calc_power] = (int)(1000.0f*P_float/S_float);
  }
  else
  {
    S[bank_for_calc_power] = 0;
    cos_phi_x1000[bank_for_calc_power] = 0;
  }
  state_calc_power = false;
  
}
/*****************************************************/

/*****************************************************/
//ĳ��������� ���
/*****************************************************/
//inline void diagnostyca_adc_execution(void)
//{
//  if (gnd_adc1 >0x51) _SET_BIT(set_diagnostyka, ERROR_GND_ADC1_TEST_BIT);
//  else _SET_BIT(clear_diagnostyka, ERROR_GND_ADC1_TEST_BIT);
//
//  if (gnd_adc2 >0x51) _SET_BIT(set_diagnostyka, ERROR_GND_ADC2_TEST_BIT);
//  else _SET_BIT(clear_diagnostyka, ERROR_GND_ADC2_TEST_BIT);
//
//  if ((vref_adc1 <0x709) || (vref_adc1 > 0x8f5)) _SET_BIT(set_diagnostyka, ERROR_VREF_ADC1_TEST_BIT);
//  else _SET_BIT(clear_diagnostyka,ERROR_VREF_ADC1_TEST_BIT);
//
//  if ((vref_adc2 <0x709) || (vref_adc2 > 0x8f5)) _SET_BIT(set_diagnostyka, ERROR_VREF_ADC2_TEST_BIT);
//  else _SET_BIT(clear_diagnostyka,ERROR_VREF_ADC2_TEST_BIT);
//
//  if ((vdd_adc1 <0x8F9) || (vdd_adc1 > 0xE13)) _SET_BIT(set_diagnostyka, ERROR_VDD_ADC1_TEST_BIT);
//  else _SET_BIT(clear_diagnostyka, ERROR_VDD_ADC1_TEST_BIT);
//
//  if ((vdd_adc2 <0x8F9) || (vdd_adc2 > 0xE13)) _SET_BIT(set_diagnostyka, ERROR_VDD_ADC2_TEST_BIT);
//  else _SET_BIT(clear_diagnostyka, ERROR_VDD_ADC2_TEST_BIT);
//}
/*****************************************************/

/*****************************************************/
//����� ���������� �����
/*****************************************************/
inline unsigned int norma_value(unsigned long long y)
{
  unsigned long long temp=y;
  unsigned int rezultat =0;
  if (temp == 0) return 0;

  while(temp !=0)
  {
    temp = temp>>1;
    rezultat++;
  }
  return rezultat-1;
}
/*****************************************************/

/*****************************************************/
//����� ���������� ����� ���� int
/*****************************************************/
inline unsigned int get_order(int value)
{
  unsigned int i = 0;

  if (value == 0) return 1;  
  if (value < 0) value =-value;

  while ((value >> (++i)) != 0); 

  return i;    
}
/*****************************************************/

/*****************************************************/
//���������� ������ ����������� ������� ����������� ������  � �������������� ���������� �����
/*****************************************************/
unsigned int sqrt_64(unsigned long long y)
{
   unsigned int b;
   unsigned int a;
   unsigned int c;
   unsigned int norma_rez;
   unsigned long long temp;
   
   norma_rez = norma_value(y)>>1;
   
   a = (1<<norma_rez) - 1;
   b = (1<<(norma_rez+1));
 
   do 
    {
     c = (a + b)>>1;
     temp = (unsigned long long)c*(unsigned long long)c; 
     if (temp != y)
       {
        if ( temp > y) b= c; else a= c;
       } 
     else return c;
    }
   while ((b-a)>1);

   c = (a + b)>>1;

   return c ;
}
/*****************************************************/

/*****************************************************/
//���������� ������ ����������� ������� ����������� ������  � �������������� ���������� �����
/*****************************************************/
inline unsigned int sqrt_32(unsigned int y)
{
   unsigned int b;
   unsigned int a;
   unsigned int c;
   unsigned int norma_rez;
   unsigned int temp;
   
   norma_rez = norma_value(y)>>1;
   
   a = (1<<norma_rez) - 1;
   b = (1<<(norma_rez+1));
 
   do 
    {
     c = (a + b)>>1;
     temp = c*c; 
     if (temp != y)
       {
        if ( temp > y) b= c; else a= c;
       } 
     else return c;
    }
   while ((b-a)>1);

   c = (a + b)>>1;

   return c ;
}
/*****************************************************/

/*****************************************************/
//���������� ������ ��������� �����������
/*****************************************************/
inline void velychyna_zvorotnoi_poslidovnosti(int ortogonal_local_calc[], const __index_I_U i_u)
{
  enum _n_phase {_A = 0, _B, _C, _A_B_C};
  static const enum _full_ort_index a_b_c[_NUMBER_FOR_I_U][_A_B_C] = 
  {
    {FULL_ORT_Ia, FULL_ORT_Ib, FULL_ORT_Ic},
    {FULL_ORT_Ua, FULL_ORT_Ub, FULL_ORT_Uc}
  };
  static const uint32_t const_val[_NUMBER_FOR_I_U][4] = 
  {
    {IM_I2, IM_I1, MNOGNYK_I_DIJUCHE, (VAGA_DILENNJA_I_DIJUCHE + 4)},
    {IM_U2, IM_U1, MNOGNYK_U_DIJUCHE, (VAGA_DILENNJA_U_DIJUCHE + 4)}
  };
    
  int ortogonal_tmp[2];
  int mul_x1, mul_x2, mul_x3, mul_x4, mul_y1, mul_y2, mul_y3, mul_y4;
  
  mul_x1 = (      ortogonal_local_calc[2*a_b_c[i_u][_B]    ]>>1 );
  mul_y1 = (0x376*ortogonal_local_calc[2*a_b_c[i_u][_B]    ]>>10);

  mul_x2 = (0x376*ortogonal_local_calc[2*a_b_c[i_u][_B] + 1]>>10);
  mul_y2 = (      ortogonal_local_calc[2*a_b_c[i_u][_B] + 1]>>1 );

  mul_x3 = (      ortogonal_local_calc[2*a_b_c[i_u][_C]    ]>>1 );
  mul_y3 = (0x376*ortogonal_local_calc[2*a_b_c[i_u][_C]    ]>>10);

  mul_x4 = (0x376*ortogonal_local_calc[2*a_b_c[i_u][_C] + 1]>>10);
  mul_y4 = (      ortogonal_local_calc[2*a_b_c[i_u][_C] + 1]>>1 );

  //�������� �����������
  ortogonal_tmp[0] = ((int)(0x155*(ortogonal_local_calc[2*a_b_c[i_u][_A]    ] - mul_x1  + mul_x2  - mul_x3 - mul_x4)))>>10;
  ortogonal_tmp[1] = ((int)(0x155*(ortogonal_local_calc[2*a_b_c[i_u][_A] + 1] - mul_y1  - mul_y2  + mul_y3 - mul_y4)))>>10;
  measurement[const_val[i_u][0]] = (unsigned int)((unsigned long long)( const_val[i_u][2]*(sqrt_32((unsigned int)ortogonal_tmp[0]*ortogonal_tmp[0] + (unsigned int)ortogonal_tmp[1]*ortogonal_tmp[1])) ) >> const_val[i_u][3]);

  //����� �����������
  ortogonal_tmp[0] = ((int)(0x155*(ortogonal_local_calc[2*a_b_c[i_u][_A]    ] - mul_x1  - mul_x2  - mul_x3 + mul_x4)))>>10;
  ortogonal_tmp[1] = ((int)(0x155*(ortogonal_local_calc[2*a_b_c[i_u][_A] + 1] + mul_y1  - mul_y2  - mul_y3 - mul_y4)))>>10;
  measurement[const_val[i_u][1]] = (unsigned int)((unsigned long long)( const_val[i_u][2]*(sqrt_32((unsigned int)ortogonal_tmp[0]*ortogonal_tmp[0] + (unsigned int)ortogonal_tmp[1]*ortogonal_tmp[1])) ) >> const_val[i_u][3]);
}
/*****************************************************/

/*****************************************************
ort_i  - �������� �� ����������� 3I0
ort_u  - �������� �� ����������� 3U0
*****************************************************/
inline void detector_kuta_nzz(int ortogonal_local_calc[]) 
{
  int cos_fi, sin_fi;

#define SIN_3I0   ortogonal_local_calc[2*FULL_ORT_3I0    ]
#define COS_3I0   ortogonal_local_calc[2*FULL_ORT_3I0 + 1]
#define SIN_3U0   ortogonal_local_calc[2*FULL_ORT_3U0    ]
#define COS_3U0   ortogonal_local_calc[2*FULL_ORT_3U0 + 1]

  /***
  �������������� ���������� ������� ������ 
  ***/
//  cos_fi = COS_3I0*COS_3U0 + SIN_3I0*SIN_3U0;
//  sin_fi = -(SIN_3I0*COS_3U0 - COS_3I0*SIN_3U0);//� ������: "���������, ��� ����", ����� �� � �� ���� ��������
  cos_fi = COS_3U0*COS_3I0 + SIN_3U0*SIN_3I0;
  sin_fi = -(SIN_3U0*COS_3I0 - COS_3U0*SIN_3I0);//� ������: "���������, ��� ����", ����� �� � �� ���� ��������
  /***/

#undef SIN_3I0
#undef COS_3I0
#undef SIN_3U0
#undef COS_3U0

  /*
  � ��������� ����������� ����������� � ������� ��������� ����� ������� (calc_measurement())
  �������, �� ����������� ������� ������� 3I0 � 3U0 � 15-��� ���� + ����.
  ��� COS � SIN ������ ��� ������� ���� ���� 31-���� �����
  �������� ���������� ������� ������� ������ 7-������ ������� + ����
  ��� ��� �� �������� ������������ ����� �����������, �� ���������� ������ �������
  COS � SIN ���� = 31 - (7 + 1) = 23
  7 - �� ���������� ��������� ������� ������� �������
  1 - �� ���������� �� �� ���������⳺�� ������� (ab+cd)
  */
  unsigned int order_cos, order_sin, max_order, shift = 0;
  order_cos = get_order(cos_fi);
  order_sin = get_order(sin_fi);
  if (order_cos > order_sin) max_order = order_cos; else max_order = order_sin;
  if (max_order > 23) shift = max_order - 23;
  
  /*
  ��� ���� � ����� ���� ���������� ��������� - �� ��� � ������ �� ����� ����������� ����,
  ��� ���� � �������� ������� ���������� ���������.
  ��� ���� ��� � ������������?..
  */

  /***
  ���������� ���������
  ***/
  int sin_fi1_minus_fi2;
  if ((cos_fi >= 0) && (sin_fi >= 0))
  {
    //1-�� ��������
    cos_fi = cos_fi >> shift;
    sin_fi = sin_fi >> shift;

#define COS_SECTOR sector_1[0]    
#define SIN_SECTOR sector_1[1]    

    sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
    sector_i_minus_u_1 = (sin_fi1_minus_fi2 < 0) ?  0 : 1;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    

#define COS_SECTOR sector_2[0]    
#define SIN_SECTOR sector_2[1]    

    sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
    sector_i_minus_u_2 = (sin_fi1_minus_fi2 < 0) ?  0 : 1;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    
  }
  else if ((cos_fi < 0) && (sin_fi >= 0))
  {
     //2-�� ��������
    cos_fi = cos_fi >> shift;
    sin_fi = sin_fi >> shift;

#define COS_SECTOR sector_1[2]    
#define SIN_SECTOR sector_1[3]    

    sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
    sector_i_minus_u_1 = (sin_fi1_minus_fi2 > 0) ?  0 : 2;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    

#define COS_SECTOR sector_2[2]    
#define SIN_SECTOR sector_2[3]    

    sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
    sector_i_minus_u_2 = (sin_fi1_minus_fi2 > 0) ?  0 : 2;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    
  }
  else if ((cos_fi < 0) && (sin_fi < 0))
  {
    //3-�� ��������
    cos_fi = cos_fi >> shift;
    sin_fi = sin_fi >> shift;

#define COS_SECTOR sector_1[4]    
#define SIN_SECTOR sector_1[5]    

    sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
    sector_i_minus_u_1 = (sin_fi1_minus_fi2 < 0) ?  0 : 3;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    

#define COS_SECTOR sector_2[4]    
#define SIN_SECTOR sector_2[5]    

    sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
    sector_i_minus_u_2 = (sin_fi1_minus_fi2 < 0) ?  0 : 3;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    
  }
  else
  {
    //4-�� ��������
    cos_fi = cos_fi >> shift;
    sin_fi = sin_fi >> shift;

#define COS_SECTOR sector_1[6]    
#define SIN_SECTOR sector_1[7]    

    sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
    sector_i_minus_u_1 = (sin_fi1_minus_fi2 > 0) ?  0 : 4;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    

#define COS_SECTOR sector_2[6]    
#define SIN_SECTOR sector_2[7]    

    sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
    sector_i_minus_u_2 = (sin_fi1_minus_fi2 > 0) ?  0 : 4;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    
  }
  /***/
}
/*****************************************************/

/*****************************************************/
//������������ ��� � ����������� �������
/*****************************************************/
inline void directional_mtz(int ortogonal_local_calc[], unsigned int number_group_stp) 
{
  for (unsigned int mtz = 0; mtz < 4; mtz++)
  {
    int a_cos_fi, a_sin_fi;
    switch (mtz)
    {
    case 0:
      {
        a_cos_fi = current_settings_prt.setpoint_mtz_1_angle_cos[number_group_stp];
        a_sin_fi = current_settings_prt.setpoint_mtz_1_angle_sin[number_group_stp];
        
        break;
      }
    case 1:
      {
        a_cos_fi = current_settings_prt.setpoint_mtz_2_angle_cos[number_group_stp];
        a_sin_fi = current_settings_prt.setpoint_mtz_2_angle_sin[number_group_stp];
        
        break;
      }
    case 2:
      {
        a_cos_fi = current_settings_prt.setpoint_mtz_3_angle_cos[number_group_stp];
        a_sin_fi = current_settings_prt.setpoint_mtz_3_angle_sin[number_group_stp];
        
        break;
      }
    case 3:
      {
        a_cos_fi = current_settings_prt.setpoint_mtz_4_angle_cos[number_group_stp];
        a_sin_fi = current_settings_prt.setpoint_mtz_4_angle_sin[number_group_stp];
        
        break;
      }
    default:
      {
        //���������� ����� ����� �� ���� � ����
        total_error_sw_fixed(54);
      }
    }
    a_sin_fi = -a_sin_fi; //� ������: "���������, ��� ����", ����� �� � �� ���� ��������

    //������ ��������� �� ������������ ���
    /*
    �� ����������� �������� ��� ���������� ����� ������� ���� ����������� � � ������ (15 �� + ����) = 16-�������� �����
    */
    int amp_cos_U_fi;
    int amp_sin_U_fi;
  
    for (unsigned int i = 0; i < 3; i++)
    {
      unsigned int index_I, index_U;
      unsigned int index_I_ort, index_U_ort;
      switch (i)
      {
      case 0:
        {
          index_I = IM_IA;
          index_U = IM_UBC;

          index_I_ort = FULL_ORT_Ia;
          index_U_ort = FULL_ORT_Ubc;

          break;
        }
      case 1:
        {
          index_I = IM_IB;
          index_U = IM_UCA;

          index_I_ort = FULL_ORT_Ib;
          index_U_ort = FULL_ORT_Uca;

          break;
        }
      case 2:
        {
          index_I = IM_IC;
          index_U = IM_UAB;

          index_I_ort = FULL_ORT_Ic;
          index_U_ort = FULL_ORT_Uab;

          break;
        }
      default:
        {
          //���������� ����� ����� �� ���� � ����
          total_error_sw_fixed(60);
        }
      }

      unsigned int porig_Uxy;
      if (Uxy_bilshe_porogu[i] == 0) porig_Uxy = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE*KOEF_POVERNENNJA_U_SECTOR_BLK/100;
      else porig_Uxy = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
      unsigned int Uxy_bilshe_porogu_tmp = Uxy_bilshe_porogu[i] = (measurement[index_U] >= porig_Uxy);
      
      unsigned int porig_Ix;
      if (Ix_bilshe_porogu[i] == 0) porig_Ix = PORIG_CHUTLYVOSTI_CURRENT*KOEF_POVERNENNJA_I_SECTOR_BLK/100;
      else porig_Ix = PORIG_CHUTLYVOSTI_CURRENT;
      unsigned int Ix_bilshe_porogu_tmp = Ix_bilshe_porogu[i]  = (measurement[index_I] >= porig_Ix );

      if (
          (Uxy_bilshe_porogu_tmp != 0) &&
          (Ix_bilshe_porogu_tmp  != 0)
         )
      {
#define SIN_I   ortogonal_local_calc[2*index_I_ort    ]
#define COS_I   ortogonal_local_calc[2*index_I_ort + 1]
#define SIN_U   ortogonal_local_calc[2*index_U_ort    ]
#define COS_U   ortogonal_local_calc[2*index_U_ort + 1]

        amp_cos_U_fi = (COS_U*a_cos_fi - SIN_U*a_sin_fi) >> AMPLITUDA_FI_VAGA; //� ����������� �� ��������� ���������� �������������
        amp_sin_U_fi = (SIN_U*a_cos_fi + COS_U*a_sin_fi) >> AMPLITUDA_FI_VAGA; //� ����������� �� ��������� ���������� �������������

        //���������� COS � SIN ���� ������ �� (U[i]+fi) � I[i]
        int cos_fi, sin_fi;
        /*
        � ��������� ����������� ����������� � ������� ��������� ����� ������� (calc_measurement())
        �������, �� ����������� �������� ������������� ��� ������ ���� ���� 0x6E51, ��� ����� ������� 
        0x120FC, �� ������� �� �� ���� 0x7C87B7BC  - 31 ���� ����� (���� ���� ���)
        ����� �� ���������� � ��� int.
        */
        cos_fi = amp_cos_U_fi*COS_I + amp_sin_U_fi*SIN_I;
        sin_fi = -(amp_sin_U_fi*COS_I - amp_cos_U_fi*SIN_I);//� ������: "���������, ��� ����", ����� �� � �� ���� ��������

#undef SIN_I
#undef COS_I
#undef SIN_U
#undef COS_U

        /*
        � ���� ��������� ����������� ��������, �� COS � SIN ������ ������ � ������� ���� ���� 31-���� �����
        ������� ���������� ������� ������� ������ 7-������ ������� + ����
        ��� ��� �� �������� ������������ ����� �����������, �� ���������� ������ �������
        COS � SIN ���� = 31 - (7 + 1) = 23
        7 - �� ���������� ��������� ������� ������� �������
        1 - �� ���������� �� �� ���������⳺�� ������� (ab+cd)
        */
        unsigned int order_cos, order_sin, max_order, shift = 0;
        order_cos = get_order(cos_fi);
        order_sin = get_order(sin_fi);
        if (order_cos > order_sin) max_order = order_cos; else max_order = order_sin;
        if (max_order > 23) shift = max_order - 23;
  
        /*
        ��� ���� � ����� ���� ���������� ��������� - �� ��� � ������ �� ����� ����������� ����,
        ��� ���� � �������� ������� ���������� ���������.
        ��� ���� ��� � ������������?..
        */

        //���������� �������
        int sin_fi1_minus_fi2;
        int *sector;
        if ((sin_fi >= 0) && (cos_fi >= 0))
        {
          //1-�� ��������
          cos_fi = cos_fi >> shift;
          sin_fi = sin_fi >> shift;
          
          if (sector_directional_mtz[mtz][i] != 1)
            sector = sector_1_mtz_tznp;
          else
            sector = sector_2_mtz_tznp;
          
#define COS_SECTOR sector[0]    
#define SIN_SECTOR sector[1]    

          sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
          sector_directional_mtz[mtz][i] = (sin_fi1_minus_fi2 <= 0) ?  1 : 0;
    
#undef COS_SECTOR    
#undef SIN_SECTOR   
        }
        else if ((sin_fi >= 0) && (cos_fi < 0))
        {
          //2-�� ��������
          cos_fi = cos_fi >> shift;
          sin_fi = sin_fi >> shift;

          if (sector_directional_mtz[mtz][i] != 2)
            sector = sector_1_mtz_tznp;
          else
            sector = sector_2_mtz_tznp;

#define COS_SECTOR sector[2]    
#define SIN_SECTOR sector[3]    

          sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
          sector_directional_mtz[mtz][i] = (sin_fi1_minus_fi2 >= 0) ?  2 : 0;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    
        }
        else if ((sin_fi < 0) && (cos_fi < 0))
        {
          //3-�� ��������
          cos_fi = cos_fi >> shift;
          sin_fi = sin_fi >> shift;

          if (sector_directional_mtz[mtz][i] != 2)
            sector = sector_1_mtz_tznp;
          else
            sector = sector_2_mtz_tznp;

#define COS_SECTOR sector[4]    
#define SIN_SECTOR sector[5]    

          sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
          sector_directional_mtz[mtz][i] = (sin_fi1_minus_fi2 <= 0) ?  2 : 0;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    
        }
        else
        {
          //4-�� ��������
          cos_fi = cos_fi >> shift;
          sin_fi = sin_fi >> shift;

          if (sector_directional_mtz[mtz][i] != 1)
            sector = sector_1_mtz_tznp;
          else
            sector = sector_2_mtz_tznp;

#define COS_SECTOR sector[6]    
#define SIN_SECTOR sector[7]    

          sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
          sector_directional_mtz[mtz][i] = (sin_fi1_minus_fi2 >= 0) ?  1 : 0;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    
        }
      }
      else
      {
        sector_directional_mtz[mtz][i] = 0;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//������������ ���� � ����������� �������
/*****************************************************/
inline void directional_tznp(int ortogonal_local_calc[], unsigned int number_group_stp) 
{
  for (unsigned int tznp = 0; tznp < 3; tznp++)
  {
    int a_cos_fi, a_sin_fi;
    switch (tznp)
    {
    case 0:
      {
        a_cos_fi = current_settings_prt.setpoint_tznp_1_angle_cos[number_group_stp];
        a_sin_fi = current_settings_prt.setpoint_tznp_1_angle_sin[number_group_stp];
        
        break;
      }
    case 1:
      {
        a_cos_fi = current_settings_prt.setpoint_tznp_2_angle_cos[number_group_stp];
        a_sin_fi = current_settings_prt.setpoint_tznp_2_angle_sin[number_group_stp];
        
        break;
      }
    case 2:
      {
        a_cos_fi = current_settings_prt.setpoint_tznp_3_angle_cos[number_group_stp];
        a_sin_fi = current_settings_prt.setpoint_tznp_3_angle_sin[number_group_stp];
        
        break;
      }
    default:
      {
        //���������� ����� ����� �� ���� � ����
        total_error_sw_fixed(81);
      }
    }
    a_sin_fi = -a_sin_fi; //� ������: "���������, ��� ����", ����� �� � �� ���� ��������

    //������ ��������� �� ������������ ���
    /*
    �� ����������� �������� ��� ���������� ����� ������� ���� ����������� � � ������ (15 �� + ����) = 16-�������� �����
    */
    unsigned int porig_U;
    if (TZNP_3U0_bilshe_porogu == 0) porig_U = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE*KOEF_POVERNENNJA_U_SECTOR_BLK/100;
    else porig_U = PORIG_CHUTLYVOSTI_VOLTAGE_ANGLE;
    unsigned int U_bilshe_porogu_tmp = TZNP_3U0_bilshe_porogu = (measurement[IM_3U0] >= porig_U);
      
    unsigned int porig_I;
    if (TZNP_3I0_r_bilshe_porogu == 0) porig_I = PORIG_CHUTLYVOSTI_CURRENT*KOEF_POVERNENNJA_I_SECTOR_BLK/100;
    else porig_I = PORIG_CHUTLYVOSTI_CURRENT;
    unsigned int I_bilshe_porogu_tmp = TZNP_3I0_r_bilshe_porogu  = (measurement[IM_3I0_r] >= porig_I);

    if (
        (U_bilshe_porogu_tmp != 0) &&
        (I_bilshe_porogu_tmp != 0)
       )
    {
      /*
      ����� � �� ������� ����� ��������� �� 180 �������.
      cos(phi + Pi) = -cos(phi)
      sin(phi + Pi) = -sin(phi)
      ���� COS_U ������� ����
      */
#define SIN_I           ( ortogonal_local_calc[2*FULL_ORT_3I0_r    ])
#define COS_I           ( ortogonal_local_calc[2*FULL_ORT_3I0_r + 1])
#define SIN_U_plus_Pi   (-ortogonal_local_calc[2*FULL_ORT_3U0      ])
#define COS_U_plus_Pi   (-ortogonal_local_calc[2*FULL_ORT_3U0   + 1])

      int cos_I_fi = (COS_I*a_cos_fi - SIN_I*a_sin_fi) >> AMPLITUDA_FI_VAGA; //� ����������� �� ��������� ���������� �������������
      int sin_I_fi = (SIN_I*a_cos_fi + COS_I*a_sin_fi) >> AMPLITUDA_FI_VAGA; //� ����������� �� ��������� ���������� �������������

      //���������� COS � SIN ���� ������ �� (3I0_r+fi) � (3U0 + 180�)
      int cos_fi, sin_fi;
      /*
      � ��������� ����������� ����������� � ������� ��������� ����� ������� (calc_measurement())
      �������, �� ����������� �������� ������������� ��� ������ ���� ���� 0x6E51, ��� ����� ������� (3U0)
      0x907E, �� ������� �� �� ���� 0x3E43DBDE  - 30 ���� ����� (���� ���� ���)
      ����� �� ���������� � ��� int.
      */
      cos_fi = cos_I_fi*COS_U_plus_Pi + sin_I_fi*SIN_U_plus_Pi;
      sin_fi = -(sin_I_fi*COS_U_plus_Pi - cos_I_fi*SIN_U_plus_Pi);//� ������: "���������, ��� ����", ����� �� � �� ���� ��������

#undef SIN_I
#undef COS_I
#undef SIN_U_plus_Pi
#undef COS_U_plus_Pi

      /*
      � ���� ��������� ���������� ��������, �� COS � SIN ������ ������ � ������� ���� ���� 31(30)-���� �����
      ������� ���������� ������� ������� ������ 7-������ ������� + ����
      ��� ��� �� �������� ������������ ����� �����������, �� ���������� ������ �������
      COS � SIN ���� = 31 - (7 + 1) = 23
      7 - �� ���������� ��������� ������� ������� �������
      1 - �� ���������� �� �� ���������⳺�� ������� (ab+cd)
      */
      unsigned int order_cos, order_sin, max_order, shift = 0;
      order_cos = get_order(cos_fi);
      order_sin = get_order(sin_fi);
      if (order_cos > order_sin) max_order = order_cos; else max_order = order_sin;
      if (max_order > 23) shift = max_order - 23;
  
      /*
      ��� ���� � ����� ���� ���������� ��������� - �� ��� � ������ �� ����� ����������� ����,
      ��� ���� � �������� ������� ���������� ���������.
      ��� ���� ��� � ������������?..
      */

      //���������� �������
      int sin_fi1_minus_fi2;
      int *sector;
      if ((sin_fi >= 0) && (cos_fi >= 0))
      {
        //1-�� ��������
        cos_fi = cos_fi >> shift;
        sin_fi = sin_fi >> shift;
          
        if (sector_directional_tznp[tznp] != 1)
          sector = sector_1_mtz_tznp;
        else
          sector = sector_2_mtz_tznp;
          
#define COS_SECTOR sector[0]    
#define SIN_SECTOR sector[1]    

        sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
        sector_directional_tznp[tznp] = (sin_fi1_minus_fi2 <= 0) ?  1 : 0;
    
#undef COS_SECTOR    
#undef SIN_SECTOR   
      }
      else if ((sin_fi >= 0) && (cos_fi < 0))
      {
        //2-�� ��������
        cos_fi = cos_fi >> shift;
        sin_fi = sin_fi >> shift;

        if (sector_directional_tznp[tznp] != 2)
          sector = sector_1_mtz_tznp;
        else
          sector = sector_2_mtz_tznp;

#define COS_SECTOR sector[2]    
#define SIN_SECTOR sector[3]    

        sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
        sector_directional_tznp[tznp] = (sin_fi1_minus_fi2 >= 0) ?  2 : 0;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    
      }
      else if ((sin_fi < 0) && (cos_fi < 0))
      {
        //3-�� ��������
        cos_fi = cos_fi >> shift;
        sin_fi = sin_fi >> shift;

        if (sector_directional_tznp[tznp] != 2)
          sector = sector_1_mtz_tznp;
        else
          sector = sector_2_mtz_tznp;

#define COS_SECTOR sector[4]    
#define SIN_SECTOR sector[5]    

        sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
        sector_directional_tznp[tznp] = (sin_fi1_minus_fi2 <= 0) ?  2 : 0;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    
      }
      else
      {
        //4-�� ��������
        cos_fi = cos_fi >> shift;
        sin_fi = sin_fi >> shift;

        if (sector_directional_tznp[tznp] != 1)
          sector = sector_1_mtz_tznp;
        else
          sector = sector_2_mtz_tznp;

#define COS_SECTOR sector[6]    
#define SIN_SECTOR sector[7]    

        sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
        sector_directional_tznp[tznp] = (sin_fi1_minus_fi2 >= 0) ?  1 : 0;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    
      }
    }
    else
    {
      sector_directional_tznp[tznp] = 0;
    }
  }
}
/*****************************************************/

/*****************************************************/
//����������� �����
/*****************************************************/
inline void calc_resistance(int ortogonal_local_calc[], int resistance_output[]) 
{
  static const unsigned int index_line_voltage[3]  = {FULL_ORT_Uab, FULL_ORT_Ubc, FULL_ORT_Uca};
  static const unsigned int index_begin_current[3] = {FULL_ORT_Ia , FULL_ORT_Ib , FULL_ORT_Ic };
  static const unsigned int index_end_current[3]   = {FULL_ORT_Ib , FULL_ORT_Ic , FULL_ORT_Ia };
  
  for (unsigned int i = 0; i < 3; i++)
  {
    //����������� ���������� ������ Ixy
    int _a2, _b2;
    /*
    � ��������� ����������� ����������� � ������� ��������� ����� ������� (calc_measurement())
    �������, �� ����������� �������� ������������� ��� ������ ���� ���� 0x6E51, ��� ����� ������� 
    0x120FC

    � ������ ������ �������������� ���� ������, ��, ���� ���������� �� ������� �������� � ��� �������, ��  �����������
    �������� ������������� ���� ����
    0x6E51*2 = 0xDCA2 �� � 16 ���� ����� (+ �������� ����) - ����� ����� �������� 17-����
    */
    
    _a2 = ortogonal_local_calc[2*index_begin_current[i] + 0] - ortogonal_local_calc[2*index_end_current[i] + 0];
    _b2 = ortogonal_local_calc[2*index_begin_current[i] + 1] - ortogonal_local_calc[2*index_end_current[i] + 1];

#define _A1     ortogonal_local_calc[2*index_line_voltage[i] + 0]
#define _B1     ortogonal_local_calc[2*index_line_voltage[i] + 1]
    
    long long mod = (long long)_a2*(long long)_a2 + (long long)_b2*(long long)_b2;
    
    //����������� �������� ������ Ixy
    unsigned int Ixy = ( MNOGNYK_I_DIJUCHE*(sqrt_64(mod)) ) >> (VAGA_DILENNJA_I_DIJUCHE + 4);
    
    if (Ixy >= PORIG_Ixy)
    {
      //����� ������������� �������� ���
      /*
       .
      Uxy   Re(Uxy) + iIm(Uxy)   a1 + ib1    (a1 + ib1)(a2 - ib2)    (a1a2 + b1b2) + i(a2b1 - a1b2)    a1a2 + b1b2      a2b1 - a1b2
      --- = ------------------ = --------- = --------------------- = ------------------------------ =  ------------ + i--------------
       .                                                                     2      2                     2      2         2      2
      Ixy   Re(Ixy) + iIm(Ixy)   a2 + ib2    (a2 + ib2)(a2 - ib2)          a2   + b2                    a2   + b2        a2   + b2
      */
      
      resistance_output[2*i    ] = (int)(MNOGNYK_R_FLOAT*((float)((long long)_A1*(long long)_a2 + (long long)_B1*(long long)_b2))/((float)mod));
      resistance_output[2*i + 1] = (int)(MNOGNYK_R_FLOAT*((float)((long long)_a2*(long long)_B1 - (long long)_A1*(long long)_b2))/((float)mod));
    }
    else
    {
      //�� ����� ������������� �������� ���
      resistance_output[2*i    ] = UNDEF_RESISTANCE;
      resistance_output[2*i + 1] = UNDEF_RESISTANCE;
    }
    
#undef _A1
#undef _B1
  }
}
/*****************************************************/

/*****************************************************/
//����������� ������ ���������
/*****************************************************/
inline void calc_power(int ortogonal_local_calc[]) 
{
  /*
  ����������� ����� � ����� ������� ��������� � ���������� ������
  
  .    .  .     .  .
  S = UabIa* - UbcIc*
  */
  
#define IA_SIN          ortogonal_local_calc[2*FULL_ORT_Ia + 1]
#define IA_COS          ortogonal_local_calc[2*FULL_ORT_Ia + 0]
#define UAB_SIN         ortogonal_local_calc[2*FULL_ORT_Uab + 1]
#define UAB_COS         ortogonal_local_calc[2*FULL_ORT_Uab + 0]
  
#define IC_SIN          ortogonal_local_calc[2*FULL_ORT_Ic + 1]
#define IC_COS          ortogonal_local_calc[2*FULL_ORT_Ic + 0]
#define UBC_SIN         ortogonal_local_calc[2*FULL_ORT_Ubc + 1]
#define UBC_COS         ortogonal_local_calc[2*FULL_ORT_Ubc + 0]
  
  long long Re_IaUab, Im_IaUab;
  if (measurement[IM_IA] >= PORIG_I_ENERGY)
  {
    Re_IaUab = UAB_COS*IA_COS + UAB_SIN*IA_SIN;
    Im_IaUab = UAB_SIN*IA_COS - UAB_COS*IA_SIN;
  
  }
  else
  {
    Re_IaUab = 0;
    Im_IaUab = 0;
  }

  long long Re_IcUbc, Im_IcUbc;
  if (measurement[IM_IC] >= PORIG_I_ENERGY)
  {  
    Re_IcUbc = UBC_COS*IC_COS + UBC_SIN*IC_SIN;
    Im_IcUbc = UBC_SIN*IC_COS - UBC_COS*IC_SIN;
  }
  else
  {
    Re_IcUbc = 0;
    Im_IcUbc = 0;
  }
#undef IA_SIN
#undef IA_COS
#undef UAB_SIN
#undef UAB_COS
  
#undef IC_SIN
#undef IC_COS
#undef UBC_SIN
#undef UBC_COS
  
  long long P_adc_x16 = Re_IaUab - Re_IcUbc; /*  ������� ��������� � ������� ��� � � �������� ���������, �� � ������� � 16 ����*/
  long long Q_adc_x16 = Im_IaUab - Im_IcUbc; /*��������� ��������� � ������� ��� � � �������� ���������, �� � ������� � 16 ����*/
  
  /*
  ���� ����������� �� ����������� MNOGNYK_I_DIJUCHE_FLOAT � MNOGNYK_U_DIJUCHE_FLOAT,
  � ���� ������� �� 16x16 - �� �������� ����� = ����/�����/����
  
  �� ������ ������� ����� � ���/����/���. 
  
  ��� ����� ������� �� �� 1000
  */
  
  //������� ���������
  double P_float = ((double)P_adc_x16) * ((double)(MNOGNYK_I_DIJUCHE_DOUBLE*MNOGNYK_U_DIJUCHE_DOUBLE/(1000.0*16.0*16.0)));
  int P_tmp = (int)(P_float); // � ������� �� ���

  //��������� ���������
  double Q_float = ((double)Q_adc_x16) * ((double)(MNOGNYK_I_DIJUCHE_DOUBLE*MNOGNYK_U_DIJUCHE_DOUBLE/(1000.0*16.0*16.0)));
  int Q_tmp  = (int)(Q_float); // � ������� �� ����

  unsigned int bank_for_enegry_tmp = bank_for_enegry;  
  int *energy_array;
  
  //������� ���������
  if (P_tmp >= 0) energy_array = P_plus;
  else energy_array = P_minus;
  *(energy_array + bank_for_enegry_tmp) += abs(P_tmp);
  
  //��������� ���������
  if     ((Q_tmp >= 0) && (P_tmp >= 0)) energy_array = Q_1q;
  else if((Q_tmp >= 0) && (P_tmp <  0)) energy_array = Q_2q;
  else if((Q_tmp <  0) && (P_tmp <  0)) energy_array = Q_3q;
  else                                  energy_array = Q_4q;
  *(energy_array + bank_for_enegry_tmp) += abs(Q_tmp);
  
  if (++lichylnyk_1s_po_20ms >= MAIN_FREQUENCY)
  {
    if (lichylnyk_1s_po_20ms > MAIN_FREQUENCY)
    {
      //���������� ����� ����� �� ���� � ����
      total_error_sw_fixed(64);
    }

    //����������� ���� ��� ���������� ����� ����� s ��������� �������� � ������ �����
    bank_for_enegry = bank_for_enegry_tmp = (bank_for_enegry_tmp + 1) & 0x1;
    P_plus[bank_for_enegry_tmp] = 0;
    P_minus[bank_for_enegry_tmp] = 0;
    Q_1q[bank_for_enegry_tmp] = 0;
    Q_2q[bank_for_enegry_tmp] = 0;
    Q_3q[bank_for_enegry_tmp] = 0;
    Q_4q[bank_for_enegry_tmp] = 0;
    
    lichylnyk_1s_po_20ms = 0;
    if (periodical_tasks_CALC_ENERGY_DATA == 0)
    {
      //������� ����������� � ���� ����������
      _SET_BIT(clear_diagnostyka, LOSE_ENERGY_DATA);
    }
    else
    {
      //���������� ����������� � ���� ����������
      _SET_BIT(set_diagnostyka, LOSE_ENERGY_DATA);
    }
    periodical_tasks_CALC_ENERGY_DATA = true;
  }
  
}
/*****************************************************/

/*****************************************************/
//���������� ���������
/*****************************************************/
void calc_measurement(unsigned int number_group_stp)
{
  int ortogonal_local[2*NUMBER_ANALOG_CANALES];
  unsigned long long sum_sqr_data_3I0_local;
  float value_3I0_i_float, value_3I0_f_float;

  //����������� ������� �������� ���������� ������� � ����������� �������
//  semaphore_measure_values = 1;

  //������� ����� �������� � ������� ����
  /*
  ������� ��� ����������� ������������ ��� ����������� ������� �� ����� ������� � ��������� �� 2 (��������� 2/�),
  �� �� �� ������ ������� ��������
  */
  /*
  �������� ���� ���������� ���������, �� ���� ����������� ������� ������� ������� �� �����/������� �� �����,
  � ���� ��������� ������� �� 2/� (������� �� ��������� �������, ��� ���� � ����� ����),
  �� ��������� ���������� ���������� ���� ���� ���������� ���������� �������� �������
  ����� ��� 3I0            - �� 19 �� + ���� = ((11 �� + ����)*16*16)
        ��� ������ ������ - �� 15 �� + ���� = ((11 �� + ����)*16   )
  ������� ��� �� ����� �� ����� ������� �� �������� � �� ���� �������� ����� ���������� � ���� ��������, ��
  ������ ����� ����� �������� �� �������� - ������������ �� ����, �� (15 �� *2) = 30 �� �� 32 �� unsigned int
  � �� ���� ������� ��������� �� 16, ��� �������� ������� ����������
  
  ��� 3I0 ������� ������������ - ���� ����� ���:
  1.  
  ����� 16-������ ��������� ������� ����� � ������������� ��� 3I0,
  ��� ����������� ������� �� ����� 15-���������� ����� + ����.
  ����� 16-������ ��������� ������� ��� � ����������� ���������
    
  2.����������� 64-���� ����������.
    
  �� 17 ��������� 2014 ���� ��������������� ������ �����.
  ��� �������� ������� ��� ���������� ����� ����� �������. ���������, �� ����� ����
  ��'����� � ���, �� ���� ���������� ������� ������� - �� ����� ����� �������� ������
  ����� ������ �� �������� �������� - � ��� �������� ����� ����� �������
    
  ���� ������ ����������� ������ ����� � 17 ��������� 2014 ����  
  
  ��� ���������� ������� � 18 ��������� 2014 ���� � ������� �������� sin/cos ���
  ����� ������ ������������
  */
  
  unsigned int bank_ortogonal_tmp = (bank_ortogonal + 1) & 0x1;
  for(unsigned int i=0; i<(2*NUMBER_ANALOG_CANALES); i++ )
  {
    ortogonal_local[i] = ortogonal[i][bank_ortogonal_tmp];
  }
  
  /*
  ������� ��� ������������� ���������� ���� �������� � ����� ������� �� �����, 
  �� ��� ����������� ������� �������� ������ �� ������� �������, �� �� �� ������ ����� � ������ ��������
  */
//  sum_sqr_data_3I0_period_local = sum_sqr_data_3I0_period;
  sum_sqr_data_3I0_local = sum_sqr_data_3I0[bank_ortogonal_tmp];

  bank_ortogonal = bank_ortogonal_tmp;
  
  //������ ������� �������� ���������� ������� � ����������� �������
//  semaphore_measure_values = 0;
  
  /*******************************************************/
  //����������, �� ���������� ���� ���������
  /*******************************************************/
  if (changed_ustuvannja == CHANGED_ETAP_ENDED_EXTRA_ETAP) /*�� � �����, �� ��� ��� ���������� ��� �������� �� � ������ �������� �������(� ��� ����� ����� ��� �� ���������)*/
  {
    //������� ����� ��������� � ���� ����� ������ ��� � ���� ������ (���� � �����) ����� ����������� ���������
    for(unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++) 
    {
      phi_ustuvannja_meas[i] = phi_ustuvannja[i];
      phi_ustuvannja_sin_cos_meas[2*i    ] = phi_ustuvannja_sin_cos[2*i    ];
      phi_ustuvannja_sin_cos_meas[2*i + 1] = phi_ustuvannja_sin_cos[2*i + 1];
    }
      
    //�������, �� ���� ������� �������� �������
    changed_ustuvannja = CHANGED_ETAP_NONE;
  }
  /*****************************************************/

  /***
  ��������� ���� � ������� ����������� ��� ����������������� �����
  ***/
  static const unsigned int *array_point_to_index_converter[4] = {index_converter_Ib_p, index_converter_I04_p, index_converter_Ib_l, index_converter_I04_l};
  const unsigned int *point_to_index_converter = array_point_to_index_converter[current_settings_prt.control_extra_settings_1 & (CTR_EXTRA_SETTINGS_1_CTRL_IB_I04 | CTR_EXTRA_SETTINGS_1_CTRL_PHASE_LINE)];

  unsigned int copy_to_low_tasks = (semaphore_measure_values_low == 0) ? true : false;
  for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
  {
    float sin_alpha = ((float)ortogonal_local[2*i    ])/((float)((1 << (VAGA_NUMBER_POINT - 1))));
    float cos_alpha = ((float)ortogonal_local[2*i + 1])/((float)((1 << (VAGA_NUMBER_POINT - 1))));
    
    float sin_beta =  phi_ustuvannja_sin_cos_meas[2*i    ];
    float cos_beta =  phi_ustuvannja_sin_cos_meas[2*i + 1];
    
    unsigned int new_index = *(point_to_index_converter + i);
    int ortogonal_sin = ortogonal_calc[2*new_index    ] = (int)(sin_alpha*cos_beta + cos_alpha*sin_beta);
    int ortogonal_cos = ortogonal_calc[2*new_index + 1] = (int)(cos_alpha*cos_beta - sin_alpha*sin_beta);

    
    //������� ����������� ��� ���������� ����
    if (copy_to_low_tasks == true)
    {
      ortogonal_calc_low[2*new_index    ] = ortogonal_sin;
      ortogonal_calc_low[2*new_index + 1] = ortogonal_cos;
    }
  }
  /***/
  
  /***/
  //����������� ���� �������� 3I0 �� ����������� �������
  /***/
  /*�������� ���������� �����*/
  sum_sqr_data_3I0_local = sqrt_64(sum_sqr_data_3I0_local);
  
  /*��� ���������� ����� �������� � �� ����� ��������� �� ��� ����������*/
  /*�� ������ ����� � ��������� � 256 ���, ���� ���� ���� �� 256*/
  /*��� ���� �����'������� �� �������� ��� ���������� ������ �������� ����� ���������*/

  /*
  �� ���� �� �������� ����� ������� �� ����� � NUMBER_POINT = 32 = 16*2
  ����� �� ���� ������� �� 4*sqrt(2)
  4 �� ������� �� 2
  ((MNOGNYK_3I0_D * X )>> VAGA_DILENNJA_3I0_D)/sqrt(2) �������
   (MNOGNYK_3I0_DIJUCHE_D * X )>> VAGA_DILENNJA_3I0_DIJUCHE_D 
  
  ���� ������ ����� ���� (MNOGNYK_3I0_DIJUCHE_D;VAGA_DILENNJA_3I0_DIJUCHE_D) � (MNOGNYK_3I0_D;VAGA_DILENNJA_3I0_D)
  �� ������ ������� ��� ���������� ����� �������.
  ���� ����� ��� �� ��� ��������� ������
  */
  value_3I0_i_float = (unsigned int)(MNOGNYK_3I0_FLOAT*((float)sum_sqr_data_3I0_local)/(1024.0f)); /*1024 = 4*256. 256 - �� ��������� ������ 3I0; 4 - �� sqrt(16), � 16 �������� � ����. �� 32 = 16*2 */
  measurement[IM_3I0_i] = (unsigned int)value_3I0_i_float; 
  /***/
  
  /*
  ---------------------------------------------------------------------------------------------------------
  150� (150 000��) - ������������ ������ �����
  ����������� ����������� � ��  - Koef_1 = 84,978173543997808495193432804655 ��� ������ ������ (5439/64 = 84,984375)
  
  ��� ��� 150� ����������� ������� �������� ������������� ���� ����
  150000/Koef_1 = 1765,0303364589078874793160507446
  ���� ���������, �� ������ � ��������� � 16 ���, �� ����������� �������� ������������� ���� ����
  16*150000/Koef_1 = 28240,485383342526199669056811914 < 28241(0x6E51) �� � 15 ���� ����� (+ �������� ����)
  ---------------------------------------------------------------------------------------------------------

  ---------------------------------------------------------------------------------------------------------
  150� (150 000��) - ����������� ����� �������
  ����������� ����������� � ��  - Koef_1 = 64,883134509545420915167731259667 ��� ������ ������ (4152/64 = 64,875)
  
  ��� ��� 150� ����������� ������� �������� ������������� ���� ����
  150000/Koef_1 = 2311,848851536795430557291797995
  ���� ���������, �� ������ � ��������� � 16 ���, �� ����������� �������� ������������� ���� ����
  16*150000/Koef_1 = 36989,581624588726888916668767919 < 36990(0x907E) �� � 16 ���� ����� (+ �������� ����) - ����� ����� �������� 17-����
  
  ���� � ����� ������� ������������� ����� �������, ��, ���� ���������� �� ������� �������� � ��� �������, ��  �����������
  �������� ������������� ���� ����
  2*16*150000/Koef_1 = 73979,163249177453777833337535838 < 73980(0x120FC) �� � 17 ���� ����� (+ �������� ����) - ����� ����� �������� 18-����
  ---------------------------------------------------------------------------------------------------------
  
  ---------------------------------------------------------------------------------------------------------
  2� (2 000�� * 10 = 20 000(������� ��)) - ������������ ����� 3I0
  ����������� ����������� � ����� ��  - Koef_1* = 169,95634708799561699038686560931 ��� 3I0  ��� 3I0 ��� �������� � ������� �������� (170/1 = 170)

  
  ��� ��� 2� ����������� ������� �������� ������������� ���� ����
  20000/Koef_1* = 117,67727621049018824880803941698
  ���� ���������, �� ������ � ��������� � 16 ��� (��������� � 256 ��� �� ��� �������� �� 16), �� ����������� �������� ������������� ���� ����
  16*20000/Koef_1* = 1882,8364193678430119809286306717 < 1883(0x075B) �� � 11 ���� ����� (+ �������� ����)
  ---------------------------------------------------------------------------------------------------------
  */
  
  /***/
  //����������� ���� �������� ����� ������������ ���'�
  /***/
  for(unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
  {
    if (i == I_3I0)
    {
      long long a, b;
      a = (long long)ortogonal_calc[2*FULL_ORT_3I0];
      b = (long long)ortogonal_calc[2*FULL_ORT_3I0 + 1];
      unsigned long long a2, b2, c;
      a2 = a*a;
      b2 = b*b;
      c = a2 + b2;
      unsigned int d;
      d = sqrt_64(c);
      value_3I0_f_float = (unsigned int)(MNOGNYK_3I0_FLOAT*((float)d)/(256.0f)); /*256 - �� ��������� ������� 3I0*/

      measurement[IM_3I0] = (unsigned int)value_3I0_f_float;
    }
    else if ((i >= I_Ia) && (i <= I_Ic))
    {
      unsigned int index_m, index_ort;
      switch (i)
      {
      case I_Ia:
        {
          index_m = IM_IA;
          index_ort = FULL_ORT_Ia;
          break;
        }
      case I_Ib_I04:
        {
          if ((current_settings_prt.control_extra_settings_1 & CTR_EXTRA_SETTINGS_1_CTRL_IB_I04) == 0)
          {
            index_m = IM_IB;
            index_ort = FULL_ORT_Ib;
          }
          else
          {
            index_m = IM_I04;
            index_ort = FULL_ORT_I04;
          }
          break;
        }
      case I_Ic:
        {
          index_m = IM_IC;
          index_ort = FULL_ORT_Ic;
          break;
        }
      default:
        {
          //���������� ����� ����� �� ���� � ����
          total_error_sw_fixed(66);
        }
      }
      
      measurement[index_m] = ( MNOGNYK_I_DIJUCHE*(sqrt_32((unsigned int)(ortogonal_calc[2*index_ort]*ortogonal_calc[2*index_ort]) + (unsigned int)(ortogonal_calc[2*index_ort+1]*ortogonal_calc[2*index_ort+1]))) ) >> (VAGA_DILENNJA_I_DIJUCHE + 4);
    }
    else
    {
      unsigned int index_m, index_ort;
      switch (i)
      {
      case I_Ua:
      case I_Ub:
      case I_Uc:
        {
          unsigned int delta_index = (i - I_Ua);
          if ((current_settings_prt.control_extra_settings_1 & CTR_EXTRA_SETTINGS_1_CTRL_PHASE_LINE) == 0)
          {
            index_m = IM_UA + delta_index;
            index_ort = FULL_ORT_Ua + delta_index;
          }
          else
          {
            index_m = IM_UAB + delta_index;
            index_ort = FULL_ORT_Uab + delta_index;
          }
          break;
        }
      case I_3U0:
        {
          index_m = IM_3U0;
          index_ort = FULL_ORT_3U0;
          break;
        }
      default:
        {
          //���������� ����� ����� �� ���� � ����
          total_error_sw_fixed(67);
        }
      }
      
      measurement[index_m] = ( MNOGNYK_U_DIJUCHE*(sqrt_32((unsigned int)(ortogonal_calc[2*index_ort]*ortogonal_calc[2*index_ort]) + (unsigned int)(ortogonal_calc[2*index_ort+1]*ortogonal_calc[2*index_ort+1]))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);
    }
    
  }
  /***/

  /***/
  //����������� ���� �������� ����� ������� 3I0
  /***/
  if (value_3I0_i_float > value_3I0_f_float)
  {
    float in_square_root, out_square_root;
    in_square_root = value_3I0_i_float*value_3I0_i_float - value_3I0_f_float*value_3I0_f_float;
    if (arm_sqrt_f32(in_square_root, &out_square_root) == ARM_MATH_SUCCESS)
    {
      measurement[IM_3I0_other_g] = (unsigned int)out_square_root;
    }
    else
    {
      //���� ���� ����� ��������, ������� �������� ����������� �������, ���� ����� ��������� ��������, ��� ���� ���� �� ������������
      total_error_sw_fixed(59);
    }
  }
  else
    measurement[IM_3I0_other_g] = 0;
  /***/

  /***
  �������������� 3I0(������������, ���� � Ib), ������ ������ � Ib(���� Ib ����)
  ***/
  int _x, _y;

  if ((current_settings_prt.control_extra_settings_1 & CTR_EXTRA_SETTINGS_1_CTRL_IB_I04) == 0)
  {   
    //3I0(������������), ����� I0.4 ����
    
    ortogonal_calc[2*FULL_ORT_I04 + 0] = 0;
    ortogonal_calc[2*FULL_ORT_I04 + 1] = 0;
    measurement[IM_I04] = 0;
    
    _x = ortogonal_calc[2*FULL_ORT_3I0_r + 0] = ortogonal_calc[2*FULL_ORT_Ia    ] + ortogonal_calc[2*FULL_ORT_Ib    ] + ortogonal_calc[2*FULL_ORT_Ic    ];
    _y = ortogonal_calc[2*FULL_ORT_3I0_r + 1] = ortogonal_calc[2*FULL_ORT_Ia + 1] + ortogonal_calc[2*FULL_ORT_Ib + 1] + ortogonal_calc[2*FULL_ORT_Ic + 1];
    if (copy_to_low_tasks == true)
    {
      ortogonal_calc_low[2*FULL_ORT_I04 + 0] = 0;
      ortogonal_calc_low[2*FULL_ORT_I04 + 1] = 0;
      
      ortogonal_calc_low[2*FULL_ORT_3I0_r + 0] = _x;
      ortogonal_calc_low[2*FULL_ORT_3I0_r + 1] = _y;
    }
    measurement[IM_3I0_r] = ( MNOGNYK_I_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_I_DIJUCHE + 4);
  }
  else
  {
    //Ib(������������), ������ 3I0(������������) ����
    
    ortogonal_calc[2*FULL_ORT_3I0_r + 0] = 0;
    ortogonal_calc[2*FULL_ORT_3I0_r + 1] = 0;
    measurement[IM_3I0_r] = 0;
    
    int ortogonal_local_3I0[2];
  
#if (4 + VAGA_DILENNJA_3I0_DIJUCHE_D_mA) >= VAGA_DILENNJA_I_DIJUCHE  
    ortogonal_local_3I0[0] = ((MNOGNYK_3I0_DIJUCHE_D_mA*ortogonal_calc[2*FULL_ORT_3I0 + 0]) >> (4 + VAGA_DILENNJA_3I0_DIJUCHE_D_mA - VAGA_DILENNJA_I_DIJUCHE))/MNOGNYK_I_DIJUCHE;
    ortogonal_local_3I0[1] = ((MNOGNYK_3I0_DIJUCHE_D_mA*ortogonal_calc[2*FULL_ORT_3I0 + 1]) >> (4 + VAGA_DILENNJA_3I0_DIJUCHE_D_mA - VAGA_DILENNJA_I_DIJUCHE))/MNOGNYK_I_DIJUCHE;
#else
    ortogonal_local_3I0[0] = ((MNOGNYK_3I0_DIJUCHE_D_mA*ortogonal_calc[2*FULL_ORT_3I0 + 0]) << (VAGA_DILENNJA_I_DIJUCHE - (VAGA_DILENNJA_3I0_DIJUCHE_D_mA + 4)))/MNOGNYK_I_DIJUCHE;
    ortogonal_local_3I0[1] = ((MNOGNYK_3I0_DIJUCHE_D_mA*ortogonal_calc[2*FULL_ORT_3I0 + 1]) << (VAGA_DILENNJA_I_DIJUCHE - (VAGA_DILENNJA_3I0_DIJUCHE_D_mA + 4)))/MNOGNYK_I_DIJUCHE;
#endif
  
    int T0 = (int)current_settings_prt.T0, TCurrent = (int)current_settings_prt.TCurrent;
    _x = ortogonal_calc[2*FULL_ORT_Ib + 0] = T0*ortogonal_local_3I0[0]/TCurrent - (ortogonal_calc[2*FULL_ORT_Ia + 0] + ortogonal_calc[2*FULL_ORT_Ic + 0]);
    _y = ortogonal_calc[2*FULL_ORT_Ib + 1] = T0*ortogonal_local_3I0[1]/TCurrent - (ortogonal_calc[2*FULL_ORT_Ia + 1] + ortogonal_calc[2*FULL_ORT_Ic + 1]);
    if (copy_to_low_tasks == true)
    {
      ortogonal_calc_low[2*FULL_ORT_3I0_r + 0] = 0;
      ortogonal_calc_low[2*FULL_ORT_3I0_r + 1] = 0;

      ortogonal_calc_low[2*FULL_ORT_Ib + 0] = _x;
      ortogonal_calc_low[2*FULL_ORT_Ib + 1] = _y;
    }
    measurement[IM_IB] = ( MNOGNYK_I_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_I_DIJUCHE + 4);
  }
    
  if ((current_settings_prt.control_extra_settings_1 & CTR_EXTRA_SETTINGS_1_CTRL_PHASE_LINE) == 0)
  {
    //Ubc
    _x = ortogonal_calc[2*FULL_ORT_Ubc + 0] = ortogonal_calc[2*FULL_ORT_Ub    ] - ortogonal_calc[2*FULL_ORT_Uc    ];
    _y = ortogonal_calc[2*FULL_ORT_Ubc + 1] = ortogonal_calc[2*FULL_ORT_Ub + 1] - ortogonal_calc[2*FULL_ORT_Uc + 1];
    if (copy_to_low_tasks == true)
    {
      ortogonal_calc_low[2*FULL_ORT_Ubc + 0] = _x;
      ortogonal_calc_low[2*FULL_ORT_Ubc + 1] = _y;
    }
    measurement[IM_UBC] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);
  
    //Uca
    _x = ortogonal_calc[2*FULL_ORT_Uca + 0] = ortogonal_calc[2*FULL_ORT_Uc    ] - ortogonal_calc[2*FULL_ORT_Ua    ];
    _y = ortogonal_calc[2*FULL_ORT_Uca + 1] = ortogonal_calc[2*FULL_ORT_Uc + 1] - ortogonal_calc[2*FULL_ORT_Ua + 1];
    if (copy_to_low_tasks == true)
    {
      ortogonal_calc_low[2*FULL_ORT_Uca + 0] = _x;
      ortogonal_calc_low[2*FULL_ORT_Uca + 1] = _y;
    }
    measurement[IM_UCA] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);

    //Uab
    _x = ortogonal_calc[2*FULL_ORT_Uab + 0] = ortogonal_calc[2*FULL_ORT_Ua    ] - ortogonal_calc[2*FULL_ORT_Ub    ];
    _y = ortogonal_calc[2*FULL_ORT_Uab + 1] = ortogonal_calc[2*FULL_ORT_Ua + 1] - ortogonal_calc[2*FULL_ORT_Ub + 1];
    if (copy_to_low_tasks == true)
    {
      ortogonal_calc_low[2*FULL_ORT_Uab + 0] = _x;
      ortogonal_calc_low[2*FULL_ORT_Uab + 1] = _y;
    }
    measurement[IM_UAB] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);

    /***/
    //����������� ������� ����� � ��������� �����������
    /***/
    velychyna_zvorotnoi_poslidovnosti(ortogonal_calc, INDEX_U);
    /***/
  }
  else
  {
    //Ua
    ortogonal_calc[2*FULL_ORT_Ua + 0] = 0;
    ortogonal_calc[2*FULL_ORT_Ua + 1] = 0;
    measurement[IM_UA] = 0;

    //Ub
    ortogonal_calc[2*FULL_ORT_Ub + 0] = 0;
    ortogonal_calc[2*FULL_ORT_Ub + 1] = 0;
    measurement[IM_UB] = 0;

    //Uc
    ortogonal_calc[2*FULL_ORT_Uc + 0] = 0;
    ortogonal_calc[2*FULL_ORT_Uc + 1] = 0;
    measurement[IM_UC] = 0;
    
    //U2
    measurement[IM_U2] = 0;

    //U1
    measurement[IM_U1] = 0;
    
    if (copy_to_low_tasks == true)
    {
      //Ua
      ortogonal_calc_low[2*FULL_ORT_Ua + 0] = 0;
      ortogonal_calc_low[2*FULL_ORT_Ua + 1] = 0;

      //Ub
      ortogonal_calc_low[2*FULL_ORT_Ub + 0] = 0;
      ortogonal_calc_low[2*FULL_ORT_Ub + 1] = 0;

      //Uc
      ortogonal_calc_low[2*FULL_ORT_Uc + 0] = 0;
      ortogonal_calc_low[2*FULL_ORT_Uc + 1] = 0;
    }
  
  }
  /***/

  /***/
  //������������ ������� ��� ��� (��� �������)
  /***/
  directional_mtz(ortogonal_calc, number_group_stp);
  /***/

  if (
      ((current_settings_prt.control_extra_settings_1 & CTR_EXTRA_SETTINGS_1_CTRL_IB_I04) == 0) &&
      ((current_settings_prt.configuration & (1<<TZNP_BIT_CONFIGURATION)) != 0)
     )   
  {
    /***/
    //������������ ������� ��� ���� (��� �������)
    /***/
    directional_tznp(ortogonal_calc, number_group_stp);
    /***/
  }
  
  if ((current_settings_prt.configuration & (1<<ZZ_BIT_CONFIGURATION)) != 0)
  {
    if ((current_settings_prt.control_zz & CTR_ZZ1_TYPE) == 0)
    {
      /***/
      //������������ ������� ��� ���
      /***/
      detector_kuta_nzz(ortogonal_calc);
      /***/
    }
  }
  
  /***/
  //����������� ����� ����� � ��������� �����������
  /***/
  velychyna_zvorotnoi_poslidovnosti(ortogonal_calc, INDEX_I);
  /***/
  
  /***/
  //����������� �����
  /***/
  calc_resistance(ortogonal_calc, resistance);
  /***/

  if(++number_inputs_for_fix_one_period >= 20)
  {
    /***/
    //���������� ������� ����������� (��� �� 20��)
    /***/
    calc_power(ortogonal_calc);
    /***/
    
    number_inputs_for_fix_one_period = 0;
  }
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
