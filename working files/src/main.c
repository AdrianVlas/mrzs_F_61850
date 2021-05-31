/* Includes ------------------------------------------------------------------*/
#include "constants.h"
#include "fatfs.h"
#include "libraries.h"
#include "variables_global.h"
#include "functions_global.h"
#include "variables_global_m.h"

unsigned int before_full_start;

/*******************************************************************************/
//Робота з Watchdog
/*******************************************************************************/
inline void watchdog_routine(unsigned int maska)
{
  time_1_watchdog_input = time_2_watchdog_input;
  time_2_watchdog_input = TIM4->CNT;
  unsigned int delta_time;
  if (time_2_watchdog_input >= time_1_watchdog_input) delta_time = time_2_watchdog_input - time_1_watchdog_input;
  else delta_time = time_2_watchdog_input + 0xffff - time_1_watchdog_input;
  time_delta_watchdog_input = delta_time* 10;
  
  //Робота з watchdogs з контролем всіх інших систем
  if((control_word_of_watchdog & maska) == maska)
  {
    //Змінюємо стан біту зовнішнього Watchdog на протилежний
    if (test_watchdogs != CMD_TEST_EXTERNAL_WATCHDOG)
    {
      GPIO_WriteBit(
                    GPIO_EXTERNAL_WATCHDOG,
                    GPIO_PIN_EXTERNAL_WATCHDOG,
                    (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_EXTERNAL_WATCHDOG, GPIO_PIN_EXTERNAL_WATCHDOG))
                   );

      time_1_watchdog_output = time_2_watchdog_output;
      time_2_watchdog_output = TIM4->CNT;
      if (time_2_watchdog_output >= time_1_watchdog_output) delta_time = time_2_watchdog_output - time_1_watchdog_output;
      else delta_time = time_2_watchdog_output + 0xffff - time_1_watchdog_output;
      time_delta_watchdog_output = delta_time* 10;
    }

    control_word_of_watchdog =  0;
  }
#ifdef DEBUG_TEST
//  else
//  {
//    unsigned int time_1_watchdog_output_tmp = time_2_watchdog_output;
//    unsigned int time_2_watchdog_output_tmp = TIM4->CNT;
//    if (time_2_watchdog_output_tmp >= time_1_watchdog_output_tmp) delta_time = time_2_watchdog_output_tmp - time_1_watchdog_output_tmp;
//    else delta_time = time_2_watchdog_output_tmp + 0xffff - time_1_watchdog_output_tmp;
//    unsigned int time_delta_watchdog_output_tmp = delta_time* 10;
//    
//    if (time_delta_watchdog_output_tmp > 100000)
//    {
//      while(time_delta_watchdog_output_tmp != 0);
//    }
//  }
#endif

  if (restart_timing_watchdog == 0)
  {
    if (time_delta_watchdog_input < time_delta_watchdog_input_min) time_delta_watchdog_input_min = time_delta_watchdog_input;
    if (time_delta_watchdog_input > time_delta_watchdog_input_max) time_delta_watchdog_input_max = time_delta_watchdog_input;

    if (time_delta_watchdog_output < time_delta_watchdog_output_min) time_delta_watchdog_output_min = time_delta_watchdog_output;
    if (time_delta_watchdog_output > time_delta_watchdog_output_max) time_delta_watchdog_output_max = time_delta_watchdog_output;
    
//    while (time_delta_watchdog_output > 100000);
  }
  else
  {
    restart_timing_watchdog = 0;
        
    time_delta_watchdog_input = 0;
    time_delta_watchdog_input_min = 0xffff*10;
    time_delta_watchdog_input_max = 0;
        
    time_delta_watchdog_output = 0;
    time_delta_watchdog_output_min = 0xffff*10;
    time_delta_watchdog_output_max = 0;
  }
}
/*******************************************************************************/

/*************************************************************************
Періодичні низькопріоритетні задачі
*************************************************************************/
void periodical_operations(unsigned int full_actions)
{
  watchdog_routine((before_full_start == true) ? UNITED_BITS_WATCHDOG_SHORT : UNITED_BITS_WATCHDOG);

  //Обмін через SPI_1
  if (  
      (control_spi1_taskes[0] != 0) || 
      (control_spi1_taskes[1] != 0) || 
      (state_execution_spi1 > 0)
     )
  {
    mutex_spi1 = true;
    if (driver_spi_df[number_chip_dataflsh_exchange].state_execution == TRANSACTION_EXECUTING_NONE)
    {
      main_routines_for_spi1();
    }
    mutex_spi1 = false;
  }

  //Обміну через I2C
  if (
      (control_i2c_taskes[0]     != 0) || 
      (driver_i2c.state_execution > 0)
     )
    main_routines_for_i2c();


  //Обробка дій системи меню
  if ((reinit_LCD) && (full_actions == true))
  {
    reinit_LCD = false;
    lcd_init();
    new_state_keyboard |= (1<<BIT_REWRITE);
  }
  main_manu_function();
  //Обновляємо інформацію на екрані
  view_whole_ekran();
    
  //Робота з Watchdog
  watchdog_routine((before_full_start == true) ? UNITED_BITS_WATCHDOG_SHORT : UNITED_BITS_WATCHDOG);

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  /*******************/
  //Управління Каналом 2 міжпроцесорного обміну між БАв і комунікаційною платою
  /*******************/
  if (
      (restart_KP_irq == 0) &&
      (IEC_board_uncall == 0) &&
      (Canal2 == false)  
     )   
  {
    CANAL2_MO_routine();
  }
  else if ((Canal1 == true) && (Canal2 == true)) Canal2 = false;
  Canal1 = false;
  /*******************/
#endif

  if (watchdog_l2) 
  {
    //Теоретично цього ніколи не мало б бути
    total_error_sw_fixed();
  }

  /*******************/
  //Контроль достовірності важливих даних
  /*******************/
  /*
  Я перевіряю поставлені задачі через
  if ()
  else if()
  else if()
  ...
  else if ()
  Щоб за один оберт виконувалася тільки одна перевірка, тобто щоб в одному оберті
  не було надто довга затримка на фонову перевірку, хоч і важливу.
  */
  if (periodical_tasks_CALC_ENERGY_DATA != 0)
  {
    //Стоїть у черзі активна задача розразунку потужності і енергій
      
    calc_power_and_energy();

    //Скидаємо активну задачу розрахунку потужності і енергій
    periodical_tasks_CALC_ENERGY_DATA = false;
  }
  else if (periodical_tasks_CALCULATION_ANGLE != 0)
  {
    //Стоїть у черзі активна задача розразунку кутів
      
    calc_angle();

    //Скидаємо активну задачу розрахунку кутів
    periodical_tasks_CALCULATION_ANGLE = false;
  }
  else if (full_actions == true)
  {
    if (periodical_tasks_TEST_SETTINGS != 0)
    {
      //Стоїть у черзі активна задача самоконтролю таблиці настройок
      if ((state_spi1_task & STATE_SETTINGS_EEPROM_GOOD) != 0)
      {
        //Перевірку здійснюємо тільки тоді, коли таблиця настройок була успішно прочитана
        if (
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_SETTINGS_EEPROM_BIT) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_SETTINGS_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_SETTINGS_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_SETTINGS_EEPROM_BIT    ) == 0) &&
            (changed_settings == CHANGED_ETAP_NONE)  
           ) 
        {
          //На даний моммент не іде читання-запис таблиці настройок, тому можна здійснити контроль достовірності
          control_settings();

          //Скидаємо активну задачу самоконтролю таблиці настройок
          periodical_tasks_TEST_SETTINGS = false;
        }
      }
      else
      {
        //Скидаємо активну задачу самоконтролю таблиці настройок, бо не було її успішне зчитування
        periodical_tasks_TEST_SETTINGS = false;
      }
    }
    else if (periodical_tasks_TEST_USTUVANNJA != 0)
    {
      //Стоїть у черзі активна задача самоконтролю юстування (і щоб не ускладнювати задачу і серійного номеру пристрою)
      if ((state_spi1_task & STATE_USTUVANNJA_EEPROM_GOOD) != 0)
      {
        //Перевірку здійснюємо тільки тоді, коли юстування було успішно прочитане
        if (
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_WRITE_USTUVANNJA_EEPROM_BIT) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_WRITING_USTUVANNJA_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_START_READ_USTUVANNJA_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_spi1_taskes, TASK_READING_USTUVANNJA_EEPROM_BIT    ) == 0) &&
            (changed_ustuvannja == CHANGED_ETAP_NONE)  
           ) 
        {
          //На даний моммент не іде читання-запис юстування, тому можна здійснити контроль достовірності
          control_ustuvannja();

          //Скидаємо активну задачу самоконтролю юстування
          periodical_tasks_TEST_USTUVANNJA = false;
        }
      }
      else
      {
        //Скидаємо активну задачу самоконтролю таблиці настройок, бо не було її успішне зчитування
        periodical_tasks_TEST_USTUVANNJA = false;
      }
    }
    else if (periodical_tasks_TEST_TRG_FUNC_LOCK != 0)
    {
      //Стоїть у черзі активна задача самоконтролю по резервній копії для триґерної інформації
      //Виконуємо її
      control_trg_func();
      
      //Скидаємо активну задачу самоконтролю по резервній копії для триґерної інформації
      periodical_tasks_TEST_TRG_FUNC_LOCK = false;
    }
    else if (periodical_tasks_TEST_INFO_REJESTRATOR_AR_LOCK != 0)
    {
      //Стоїть у черзі активна задача самоконтролю по резервній копії для аналогового реєстратора
      //Виконуємо її
      unsigned int result;
      result = control_info_ar_rejestrator(&info_rejestrator_ar_ctrl, crc_info_rejestrator_ar_ctrl);
      
      if (result == 1)
      {
        //Контроль достовірності реєстратора пройшов успішно
    
        //Скидаємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_AR_CONTROL_BIT);
      }
      else
      {
        //Контроль достовірності реєстратора не пройшов

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_AR_CONTROL_BIT);
      }

      //Скидаємо активну задачу самоконтролю по резервній копії для аналогового реєстратора
      periodical_tasks_TEST_INFO_REJESTRATOR_AR_LOCK = false;
    }
    else if (periodical_tasks_TEST_INFO_REJESTRATOR_DR_LOCK != 0)
    {
      //Стоїть у черзі активна задача самоконтролю по резервній копії для дискретного реєстратора
      //Виконуємо її
      unsigned int result;
      result = control_info_rejestrator(&info_rejestrator_dr_ctrl, crc_info_rejestrator_dr_ctrl);
      
      if (result == 1)
      {
        //Контроль достовірності реєстратора пройшов успішно
    
        //Скидаємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_DR_CONTROL_BIT);
      }
      else
      {
        //Контроль достовірності реєстратора не пройшов

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_DR_CONTROL_BIT);
      }

      //Скидаємо активну задачу самоконтролю по резервній копії для аналогового реєстратора
      periodical_tasks_TEST_INFO_REJESTRATOR_DR_LOCK = false;
    }
    else if (periodical_tasks_TEST_INFO_REJESTRATOR_PR_ERR_LOCK != 0)
    {
      //Стоїть у черзі активна задача самоконтролю по резервній копії для реєстратора програмних подій
      //Виконуємо її
      unsigned int result;
      result = control_info_rejestrator(&info_rejestrator_pr_err_ctrl, crc_info_rejestrator_pr_err_ctrl);
      
      if (result == 1)
      {
        //Контроль достовірності реєстратора пройшов успішно
    
        //Скидаємо повідомлення у слові діагностики
        _SET_BIT(clear_diagnostyka, ERROR_INFO_REJESTRATOR_PR_ERR_CONTROL_BIT);
      }
      else
      {
        //Контроль достовірності реєстратора не пройшов

        //Виствляємо повідомлення у слові діагностики
        _SET_BIT(set_diagnostyka, ERROR_INFO_REJESTRATOR_PR_ERR_CONTROL_BIT);
      }

      //Скидаємо активну задачу самоконтролю по резервній копії для аналогового реєстратора
      periodical_tasks_TEST_INFO_REJESTRATOR_PR_ERR_LOCK = false;
    }
    else if (periodical_tasks_TEST_RESURS_LOCK != 0)
    {
      //Стоїть у черзі активна задача самоконтролю по резервній копії для ресурсу лічильника
      //Виконуємо її
      control_resurs();

      //Скидаємо активну задачу самоконтролю по резервній копії для аналогового реєстратора
      periodical_tasks_TEST_RESURS_LOCK = false;
    }
  }

  /*******************/

  //Підрахунок вільного ресуру процесор-програма
  if(resurs_temp < 0xfffffffe) resurs_temp++;

  //Робота з Watchdog
  watchdog_routine((before_full_start == true) ? UNITED_BITS_WATCHDOG_SHORT : UNITED_BITS_WATCHDOG);
}
/*************************************************************************/

/*************************************************************************
Періодичні низькопріоритетні задачі комунікації
*************************************************************************/
void periodical_operations_communication(unsigned int ar_working)
{
  //Робота з Watchdog
  watchdog_routine(UNITED_BITS_WATCHDOG);
  
  //Робота з таймером очікування нових змін налаштувань
  if ((timeout_idle_new_settings >= current_settings.timeout_idle_new_settings) && (restart_timeout_idle_new_settings == 0))
  {
    if (_CHECK_SET_BIT(active_functions, RANG_SETTINGS_CHANGED) != 0) 
    {
      current_settings_interfaces = current_settings;
      type_of_settings_changed = 0;
      _CLEAR_BIT(active_functions, RANG_SETTINGS_CHANGED);
    }
  }
  
  static unsigned int selection_interface;

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  if ((ar_working == false) && (selection_interface == LAN_RECUEST))
  {
  //Обмін по LAN
    if (current_settings.password_interface_LAN)
    {
      unsigned int timeout = current_settings.timeout_deactivation_password_interface_LAN;
      if ((timeout != 0) && (timeout_idle_LAN >= timeout) && ((restart_timeout_interface & (1 << LAN_RECUEST)) == 0)) password_set_LAN = 1;
    }
    
    if (LAN_received_count > 0) inputPacketParserLAN();
  }
#endif

  if ((ar_working == false) && (selection_interface  == USB_RECUEST))
  {
    //Обмін по USB
    if (current_settings.password_interface_USB)
    {
      unsigned int timeout = current_settings.timeout_deactivation_password_interface_USB;
      if ((timeout != 0) && (timeout_idle_USB >= timeout) && ((restart_timeout_interface & (1 << USB_RECUEST)) == 0)) password_set_USB = 1;
    }
    Usb_routines();
  }

  if ((ar_working == false) && (selection_interface == RS485_RECUEST))
  {
    //Обмін по RS-485
    if (current_settings.password_interface_RS485)
    {
      unsigned int timeout = current_settings.timeout_deactivation_password_interface_RS485;
      if ((timeout != 0) && (timeout_idle_RS485 >= timeout) && ((restart_timeout_interface & (1 << RS485_RECUEST)) == 0)) password_set_RS485 = 1;
    }
    if(
       (RxBuffer_RS485_count != 0) &&
       (make_reconfiguration_RS_485 == 0) &&
       ((DMA_StreamRS485_Rx->CR & (uint32_t)DMA_SxCR_EN) == 0)
      )
    {
      //Це є умовою, що дані стоять у черзі  на обробку
      
      //Робота з Watchdog
      watchdog_routine(UNITED_BITS_WATCHDOG);

      //Обробляємо запит
      inputPacketParserRS485();
    
      //Виставляємо, що кількість прийнятих байт рівна 0
      RxBuffer_RS485_count = 0;
    }
    else if (make_reconfiguration_RS_485 != 0)
    {
      //Стоїть умова переконфігурувати RS-485
      
      //Перевіряємо чи на даний моент не іде передача даних на верхній рівень
      if (GPIO_ReadOutputDataBit(GPIO_485DE, GPIO_PIN_485DE) == Bit_RESET)
      {

        //Переконфігуровуємо USART для RS-485
        USART_RS485_Configure();

        //Відновлюємо моніторинг каналу RS-485
        restart_monitoring_RS485();
      
        //Знімаємо індикацю про невикану переконфігупацію інтерфейсу RS-485
        make_reconfiguration_RS_485 = 0;
      }
    }
  }

  //Робота з Watchdog
  watchdog_routine(UNITED_BITS_WATCHDOG);
       
  selection_interface++;
  selection_interface %= MAX_INTERFACES;
}
/*************************************************************************/

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
//  /************************************************************/
//  //Перевірка контрольної суми програми
//  /************************************************************/
//  {
//    unsigned short sum = 0;
//    unsigned char *point = ((unsigned char *)&__checksum_begin);
//    for (unsigned int i = ((unsigned int)&__checksum_end -(unsigned int)&__checksum_begin +1); i > 0; i--)
//      sum += *point++;
//    if (sum != (unsigned short)__checksum)
//    {
//      while(1);
//    }
//  }
//  /************************************************************/
  
  /************************************************************/
  //Стартова ініціалізація
  /************************************************************/
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_Conf();            /* Configure and initialize SystemView  */
#endif
  
  //Виставляємо подію про зупинку пристрою у попередньому сеансі роботи, а час встановиться пізніше, RTC запм'ятовує час пропадання живлення
  _SET_BIT(set_diagnostyka, EVENT_STOP_SYSTEM_BIT);
  changing_diagnostyka_state();//Підготовлюємо новий запис для реєстратора програмних подій
  
  //Перевіряємо, що відбулося: запуск приладу, чи перезапуск (перезапуск роботи приладу без зняття оперативного живлення) 
  if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) == SET)
  {
    //Виставляємо подію про програмний перезапуск пристрою
    _SET_BIT(set_diagnostyka, EVENT_SOFT_RESTART_SYSTEM_BIT);
  }
  else if (RCC_GetFlagStatus(RCC_FLAG_BORRST/*RCC_FLAG_PORRST*/) != SET)
  {
    //Виставляємо подію про перезапуск пристрою (бо не зафіксовано подію Power-on/Power-down)
    _SET_BIT(set_diagnostyka, EVENT_RESTART_SYSTEM_BIT);
  }
  else
  {
    //Виставляємо подію про запуск пристрою 
    _SET_BIT(set_diagnostyka, EVENT_START_SYSTEM_BIT);
  }
  //Очищаємо прапорці
  RCC->CSR |= RCC_CSR_RMVF;
  changing_diagnostyka_state();//Підготовлюємо новий запис для реєстратора програмних подій

  //Стартова настройка периферії процесора
  start_settings_peripherals();
  
  if(
     ((state_spi1_task & STATE_SETTINGS_EEPROM_GOOD) != 0) &&
     ((state_spi1_task & STATE_TRG_FUNC_EEPROM_GOOD) != 0)
    )   
  {
    //Випадок, якщо настройки успішно зчитані
          
    //Дозволяєм роботу таймера вимірювальної системи
    TIM_Cmd(TIM5, ENABLE);
    // Дозволяєм роботу таймера системи захистів
    TIM_Cmd(TIM2, ENABLE);


//    TEST_OUTPUT->BSRRH = TEST_OUTPUT_PIN;

    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD);

    /**********************/
    //Конфігуруємо I2C
    /**********************/
    Configure_I2C(I2C);
    /**********************/

    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD);

    //Виставляємо признак, що требаа прочитати всі регістри RTC, а потім, при потребі відкоректувати його поля
    //При цьому виставляємо біт блокування негайного запуску операції, щоб засинхронізуватися з роботою вимірювальної системи
    _SET_BIT(control_i2c_taskes, TASK_START_READ_RTC_BIT);
    _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
    //Обмін через I2C
    while (
           (control_i2c_taskes[0]     != 0) || 
           (driver_i2c.state_execution > 0)
          )
      main_routines_for_i2c();
    
    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD);


    //Ініціалізація LCD
    lcd_init();
    changing_diagnostyka_state();//Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій
  }
  else
  {
    //Випадок, якщо настройки успішно не зчитані, або їх взагалі немає
    
    //Ініціалізація LCD
    lcd_init();
    changing_diagnostyka_state();//Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій
  
    //Якщо настройки не зчитані успішно з EEPROM, то спочатку виводимо на екран повідомлення про це
    while (
           ((state_spi1_task & STATE_SETTINGS_EEPROM_GOOD) == 0) ||
           ((state_spi1_task & STATE_TRG_FUNC_EEPROM_GOOD) == 0)
          )   
    {
      error_reading_with_eeprom();
    }

    /*****/
    /*
    Очищаємо структуру інформації по аналоговому реєстраторі оскільки настройки 
    встановлені у мінімальну конфігурацію, а це значить, що, можливо, величини 
    ширин доаварійного і післяаварійного масивів не будуть співпадати (на яких 
    до цього часу працював аналоговий реєстратор і при мінімильній конфігурації)
    */
    /*****/
    //Виставляємо команду запису цієї структури у EEPROM
    /*
    Команду виставляємо скоріше, а потім робимо зміни у полях, які треба змінити,
    бо по вимозі проконтролювати достовірність даних інформації по аналоговому
    реєстратору відбувається копіювання з системи захистів структури
    info_rejestrator_ar у резервну мкопію. Це копіювання блокується у випадку 
    "читання з"/"запису в" EEPROM цієї інформації. Тому виставлення спочатку команди
    запису заблокує копіювання.
    З другої сторони не можливо, щоб почався запис до модифікації, 
    бо запис ініціюється функцією main_routines_for_spi1 - яка виконується на одному і тому ж
    рівні пріоритетності, що і функція main.
    Тобто спочатку треба дійти до виклику функції main_routines_for_spi1, і аж тоді можливе
    виконання команди, яку ми виставили перед зміною даних, яку 
    ми зараз гарантовано зробимо (до виклику функції main_routines_for_spi1)
    */
    _SET_BIT(control_spi1_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);
    
    info_rejestrator_ar.first_number = -1;
    info_rejestrator_ar.last_number = -1;
    _SET_STATE(FATFS_command, FATFS_FORMAT);
    while(
          (control_spi1_taskes[0]     != 0) ||
          (control_spi1_taskes[1]     != 0) ||
          (state_execution_spi1 > 0)
         )
    {
      //Робота з watchdogs
      watchdog_routine(WATCHDOG_KYYBOARD);

      main_routines_for_spi1();
    }
    /*****/
          
    //Дозволяєм роботу таймера вимірювальної системи
    TIM_Cmd(TIM5, ENABLE);
    //Дозволяєм роботу таймера системи захистів
    TIM_Cmd(TIM2, ENABLE);


    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD);

    /**********************/
    //Конфігуруємо I2C
    /**********************/
    Configure_I2C(I2C);
    /**********************/

    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD);
    
    //Виставляємо признак, що требаа прочитати всі регістри RTC, а потім, при потребі відкоректувати його поля
    //При цьому виставляємо біт блокування негайного запуску операції, щоб засинхронізуватися з роботою вимірювальної системи
    _SET_BIT(control_i2c_taskes, TASK_START_READ_RTC_BIT);
    _SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
    
    //Обмін через I2C
    while (
           (control_i2c_taskes[0]     != 0) || 
           (driver_i2c.state_execution > 0)
          )
      main_routines_for_i2c();

    //Робота з watchdogs
    watchdog_routine(WATCHDOG_KYYBOARD);

  }
  changing_diagnostyka_state();//Підготовлюємо новий потенційно можливий запис для реєстратора програмних подій

  /**********************/
  //Завершальна частина запуску логіки приладу
  /**********************/
  //Робота з watchdogs
  watchdog_routine(WATCHDOG_KYYBOARD);

  //Перевірка параметрування мікросхем DataFlash
  start_checking_dataflash();

  //Запускаємо генерацію переривань кожну кожну 1 мс від каналу 2 таймеру 4 для виконання періодичних низькопріоритетних задач
  start_tim4_canal2_for_interrupt_1mc();
  
//  /***/
//  {
//    size_t col = 0;
//    for(size_t i = 0; i < 0x10000; ++i)
//    {
//      _DEVICE_REGISTER_V2(Bank1_SRAM2_ADDR, OFFSET_DD32_DD38) = ((1 << col) << LED_N_ROW) | ((uint32_t)(0) & ((1 << LED_N_ROW) - 1));
//      if (++col >= LED_N_COL) col = 0;
//      for (size_t j = 0; j < 100; ++j) watchdog_routine(WATCHDOG_KYYBOARD);
//    }
//  }
//  /***/
  //Ініціалізація FATFs
  MX_FATFS_Init();
  
  if((POWER_CTRL->IDR & POWER_CTRL_PIN) == (uint32_t)Bit_RESET)
  {
    unsigned int number_seconds_tmp = (number_seconds + 2) % 60;
    while (
           ((POWER_CTRL->IDR & POWER_CTRL_PIN) == (uint32_t)Bit_RESET) &&
           (
            (
             (measurement[IM_IA] < POWEER_ISNOT_FROM_IA_IC) &&
             (measurement[IM_IC] < POWEER_ISNOT_FROM_IA_IC)
            )
            ||  
            (number_seconds != number_seconds_tmp)
           )   
          )
    {
      before_full_start = true;
      ar_routine_with_fatfs(true);
      watchdog_routine(UNITED_BITS_WATCHDOG_SHORT);
      if (
          (measurement[IM_IA] < POWEER_ISNOT_FROM_IA_IC) &&
          (measurement[IM_IC] < POWEER_ISNOT_FROM_IA_IC)
         )
      {
        number_seconds_tmp = (number_seconds + 2) % 60;
      }
    }
    before_full_start = false;
  }
  /**********************/
  
  /**********************/
  //Ініціалізація компонет Ігоря для Modbus + USB
  /**********************/
  watchdog_l2 = true;
  global_component_installation();  
  
  USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS 
            USB_OTG_HS_CORE_ID,
#else            
            USB_OTG_FS_CORE_ID,
#endif  
            &USR_desc, 
            &USBD_CDC_cb, 
            &USR_cb);
  watchdog_l2 = false;
  
  //Робота з watchdogs
  watchdog_routine(WATCHDOG_KYYBOARD);
  /**********************/
    
  timeout_idle_new_settings = current_settings.timeout_idle_new_settings;
  //Визначаємо, чи стоїть дозвіл запису через інтерфейси з паролем
  if (current_settings.password_interface_RS485 == 0) password_set_RS485 = 0;
  else password_set_RS485 = 1;
  timeout_idle_RS485 = current_settings.timeout_deactivation_password_interface_RS485;
  
  if (current_settings.password_interface_USB   == 0) password_set_USB   = 0;
  else password_set_USB   = 1;
  timeout_idle_USB = current_settings.timeout_deactivation_password_interface_USB;

#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
  if (current_settings.password_interface_LAN == 0) password_set_LAN = 0;
  else password_set_LAN = 1;
  timeout_idle_LAN = current_settings.timeout_deactivation_password_interface_LAN;
#endif
  
  //Підраховуємо величину затримки у бітах, яка допускається між байтами у RS-485 згідно з визначеними настройками
  calculate_namber_bit_waiting_for_rs_485();
  //Запускаємо генерацію переривань з плаваючим періодом (час очікування наступного символа) від каналу 3 таймеру 4 для RS-485
  start_tim4_canal3_for_interrupt_10mkc();
  //Настроюємо USART на роботу з RS-485
  USART_RS485_Configure();
  //Починаємо моніторинг каналу RS-485
  restart_monitoring_RS485();
  /************************************************************/

  //Виставляємо признак, що на екрані треба обновити стартову інформацію
  new_state_keyboard |= (1<<BIT_REWRITE);
  
  //Робота з watchdogs
  watchdog_routine(WATCHDOG_KYYBOARD);
  restart_resurs_count = 0xff;/*Ненульове значення перезапускає лічильники*/

  time_2_watchdog_input = time_2_watchdog_output = TIM4->CNT;
  restart_timing_watchdog = 0xff;
  
  /* Періодичні задачі */
  while (1)
  {
    //Немає активних операцій по Аналоговому реєстратору
    before_full_start = false;
    if (periodical_tasks_TEST_FLASH_MEMORY != 0)
    {
      /************************************************************/
      //Перевірка контрольної суми програми
      /************************************************************/
      unsigned short sum = 0;
      unsigned char *point = ((unsigned char *)&__checksum_begin);
      for (unsigned int i = ((unsigned int)&__checksum_end -(unsigned int)&__checksum_begin +1); i > 0; i--)
      {
        sum += *point++;
        ar_routine_with_fatfs(false);
        watchdog_routine(UNITED_BITS_WATCHDOG);
      }
      if (sum != (unsigned short)__checksum) _SET_BIT(set_diagnostyka, ERROR_INTERNAL_FLASH_BIT);
      else _SET_BIT(clear_diagnostyka, ERROR_INTERNAL_FLASH_BIT);
      /************************************************************/

      periodical_tasks_TEST_FLASH_MEMORY = false;
    }
    else 
    {
      ar_routine_with_fatfs(false);
      watchdog_routine(UNITED_BITS_WATCHDOG);
    }
  }
}
/*******************************************************************************/

/*******************************************************************************/
//Глобальна помилка програмного забеспечення
/*******************************************************************************/
void total_error_sw_fixed(void)
{
  while (1);
}
/*******************************************************************************/

#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


#ifndef RSTATYSYKA
#warning "Small Statistic Registrator"
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
