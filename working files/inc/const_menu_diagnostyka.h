#ifndef __CONST_MENU_DIAGNOSTYKA__
#define __CONST_MENU_DIAGNOSTYKA__

#define EKRAN_DIAGNOSTYKA                    (EKRAN_POINT_TIME_RANGUVANNJA + 1)

#define MAX_ROW_FOR_DIAGNOSTYKA              (8*(4 + 4 + 3))  /*2 слова типу unsigned int + ще одне слово але з трьох байт. щоб розмір одного запису реєстратора програмних подій не був більшим 32 байти. А коли треба - то змінимо*/  

#define USED_BITS_IN_LAST_INDEX  0x00ffffff  

enum _error_id {
ERROR_START_VIA_I2C_BIT = 0,
ERROR_SETTINGS_EEPROM_BIT,
ERROR_SETTINGS_EEPROM_EMPTY_BIT,
ERROR_SETTINGS_EEPROM_COMPARISON_BIT,
ERROR_SETTINGS_EEPROM_CONTROL_BIT,
ERROR_SETTINGS_EEPROM_DEVICE_ID_FAIL_BIT,
ERROR_USTUVANNJA_EEPROM_BIT,
ERROR_USTUVANNJA_EEPROM_EMPTY_BIT,
ERROR_USTUVANNJA_EEPROM_COMPARISON_BIT,
ERROR_USTUVANNJA_EEPROM_CONTROL_BIT,
ERROR_USTUVANNJA_EEPROM_ADJUSTMENT_ID_FAIL_BIT,
ERROR_STATE_LEDS_EEPROM_BIT,
ERROR_STATE_OUTPUTS_EEPROM_BIT,
ERROR_STATE_LEDS_OUTPUTS_EEPROM_EMPTY_BIT,
ERROR_STATE_LEDS_OUTPUTS_EEPROM_COMPARISON_BIT,
ERROR_TRG_FUNC_EEPROM_BIT,
ERROR_TRG_FUNC_EEPROM_EMPTY_BIT,
ERROR_TRG_FUNC_EEPROM_COMPARISON_BIT,
ERROR_TRG_FUNC_EEPROM_CONTROL_BIT,
ERROR_INFO_REJESTRATOR_AR_EEPROM_BIT,
ERROR_INFO_REJESTRATOR_AR_EEPROM_EMPTY_BIT,
ERROR_INFO_REJESTRATOR_AR_COMPARISON_BIT,
ERROR_INFO_REJESTRATOR_AR_CONTROL_BIT,
ERROR_INFO_REJESTRATOR_DR_EEPROM_BIT,
ERROR_INFO_REJESTRATOR_DR_EEPROM_EMPTY_BIT,
ERROR_INFO_REJESTRATOR_DR_COMPARISON_BIT,
ERROR_INFO_REJESTRATOR_DR_CONTROL_BIT,
ERROR_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT,
ERROR_INFO_REJESTRATOR_PR_ERR_EEPROM_EMPTY_BIT,
ERROR_INFO_REJESTRATOR_PR_ERR_COMPARISON_BIT,
ERROR_INFO_REJESTRATOR_PR_ERR_CONTROL_BIT,
ERROR_RESURS_EEPROM_BIT,
ERROR_RESURS_EEPROM_EMPTY_BIT,
ERROR_RESURS_EEPROM_COMPARISON_BIT,
ERROR_RESURS_EEPROM_CONTROL_BIT,
ERROR_ENERGY_EEPROM_BIT,
ERROR_ENERGY_EEPROM_EMPTY_BIT,
ERROR_ENERGY_EEPROM_COMPARISON_BIT,

RTC_BATTERY_LOW_BIT,
RTC_OSCILLATOR_STOPED_BIT,
RTC_OSCILLATOR_FAIL_BIT,
RTC_UPDATING_HALTED_BIT,
RTC_WORK_FIELD_NOT_SET_BIT,

ERROR_GND_ADC1_TEST_BIT,
ERROR_VREF_ADC1_TEST_BIT,
ERROR_VDD_ADC1_TEST_BIT,
ERROR_GND_ADC1_TEST_COARSE_BIT,
ERROR_VREF_ADC1_TEST_COARSE_BIT,
ERROR_VDD_ADC1_TEST_COARSE_BIT,

ERROR_GND_ADC2_TEST_BIT,
ERROR_VREF_ADC2_TEST_BIT,
ERROR_VDD_ADC2_TEST_BIT,
ERROR_GND_ADC2_TEST_COARSE_BIT,
ERROR_VREF_ADC2_TEST_COARSE_BIT,
ERROR_VDD_ADC2_TEST_COARSE_BIT,

ERROR_SPI_ADC_BIT,

ERROR_OSCYLOJRAPH_OVERFLOW,

ERROR_DIGITAL_OUTPUT_1_BIT,

ERROR_AR_TEMPORARY_BUSY_BIT = ERROR_DIGITAL_OUTPUT_1_BIT + NUMBER_OUTPUTS,
ERROR_AR_OVERLOAD_BUFFER_BIT,
ERROR_AR_UNDEFINED_BIT,
ERROR_AR_LOSS_INFORMATION_BIT,

ERROR_DR_TEMPORARY_BUSY_BIT,
ERROR_DR_UNDEFINED_BIT,
ERROR_DR_LOSS_INFORMATION_BIT,

ERROR_PR_ERR_OVERLOAD_BIT,
ERROR_PR_ERR_LOSS_INFORMATION_BIT,

EVENT_START_SYSTEM_BIT,
EVENT_RESTART_SYSTEM_BIT,
EVENT_STOP_SYSTEM_BIT,
EVENT_DROP_POWER_BIT,

ERROR_LCD_BIT,
ERROR_SPI_DF_BIT,
ERROR_EXTERNAL_SRAM_BIT,
ERROR_INTERNAL_FLASH_BIT,

ERROR_SELECT_GRUPY_USRAVOK,

LOSE_ENERGY_DATA
};

#define MASKA_AVAR_ERROR_0        (unsigned int)(               \
    (1 << (ERROR_SETTINGS_EEPROM_BIT))                          \
  | (1 << (ERROR_SETTINGS_EEPROM_EMPTY_BIT))                    \
  | (1 << (ERROR_SETTINGS_EEPROM_COMPARISON_BIT) )              \
  | (1 << (ERROR_SETTINGS_EEPROM_CONTROL_BIT))                  \
  | (1 << (ERROR_USTUVANNJA_EEPROM_BIT) )                       \
  | (1 << (ERROR_USTUVANNJA_EEPROM_EMPTY_BIT))                  \
  | (1 << (ERROR_USTUVANNJA_EEPROM_COMPARISON_BIT))             \
  | (1 << (ERROR_USTUVANNJA_EEPROM_CONTROL_BIT))                \
  | (1 << (ERROR_USTUVANNJA_EEPROM_ADJUSTMENT_ID_FAIL_BIT))     \
  | (1 << (ERROR_TRG_FUNC_EEPROM_BIT))                          \
  | (1 << (ERROR_TRG_FUNC_EEPROM_EMPTY_BIT))                    \
  | (1 << (ERROR_TRG_FUNC_EEPROM_COMPARISON_BIT))               \
  | (1 << (ERROR_TRG_FUNC_EEPROM_CONTROL_BIT))                  \
)

#define MASKA_AVAR_ERROR_1        (unsigned int)(               \
    (1 << (ERROR_GND_ADC1_TEST_BIT - 32))                       \
  | (1 << (ERROR_VREF_ADC1_TEST_BIT - 32))                      \
  | (1 << (ERROR_VDD_ADC1_TEST_BIT - 32))                       \
  | (1 << (ERROR_GND_ADC1_TEST_COARSE_BIT - 32))                \
  | (1 << (ERROR_VREF_ADC1_TEST_COARSE_BIT - 32))               \
  | (1 << (ERROR_VDD_ADC1_TEST_COARSE_BIT - 32))                \
  | (1 << (ERROR_GND_ADC2_TEST_BIT - 32))                       \
  | (1 << (ERROR_VREF_ADC2_TEST_BIT - 32))                      \
  | (1 << (ERROR_VDD_ADC2_TEST_BIT - 32))                       \
  | (1 << (ERROR_GND_ADC2_TEST_COARSE_BIT - 32))                \
  | (1 << (ERROR_VREF_ADC2_TEST_COARSE_BIT - 32))               \
  | (1 << (ERROR_VDD_ADC2_TEST_COARSE_BIT - 32))                \
  | (1 << (ERROR_SPI_ADC_BIT - 32))                             \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 0 - 32))                \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 1 - 32))                \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 2 - 32))                \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 3 - 32))                \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 4 - 32))                \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 5 - 32))                \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 6 - 32))                \
)

#define MASKA_AVAR_ERROR_2        (unsigned int)(               \
    (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 7 - 64))                \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 8 - 64))                \
  | (1 << (ERROR_DIGITAL_OUTPUT_1_BIT + 9 - 64))                \
  | (1 << (ERROR_INTERNAL_FLASH_BIT - 64))                      \
)

# define NAME_DIAGN_RU  \
  " Ош.I2C         ",   \
  " Ош.настроек    ",   \
  " Настроек нет   ",   \
  " Ош.зап.настр.  ",   \
  " Ош.контр.настр.",   \
  " Ош.типа настр. ",   \
  " Ош.юстирования ",   \
  " Юстирования нет",   \
  " Ош.зап.юстир.  ",   \
  " Ош.контр.юстир.",   \
  " Ош.юстир.набора",   \
  " Ош.воcст.тр.св ",   \
  " Ош.воcст.с.вых ",   \
  "Инф.вых./св.нет ",   \
  " Ош.зап.вых./св.",   \
  "  Ош.триг.инф.  ",   \
  "  Триг.инф.нет  ",   \
  "Ош.зап.триг.инф.",   \
  "Ош.контр.триг.и.",   \
  " Ош.инф.ан.рег. ",   \
  " Инф.ан.рег.нет ",   \
  "Ош.зап.и.ан.рег.",   \
  "Ош.контр.ан.рег.",   \
  " Ош.инф.д.рег.  ",   \
  " Инф.д.рег.нет  ",   \
  " Ош.зап.и.д.рег.",   \
  " Ош.контр.д.рег.",   \
  "Ош.инф.рег.пр.с.",   \
  "Инф.рег.пр.с.нет",   \
  "Ош.зап.и.р.пр.с.",   \
  "Ош.контр.р.пр.с.",   \
  " Ош.инф.сч.рес. ",   \
  " Инф.сч.рес.нет ",   \
  " Ош.зап.сч.рес. ",   \
  "Ош.контр.сч.рес.",   \
  " Ош.к.с.энергий ",   \
  "  Энергий нет   ",   \
  " Ош.зап.энергий ",   \
  " Батарея разряж.",   \
  "Осцилятор остан.",   \
  "Отказ Осцилятора",   \
  " Ост.обновл.RTC ",   \
  " Не уст.поля RTC",   \
  " Тест GND АЦП1  ",   \
  " Тест VREF АЦП1 ",   \
  " Тест VDD АЦП1  ",   \
  "Тест GND АЦП1 гр",   \
  "Тест VREF АЦП1гр",   \
  "Тест VDD АЦП1 гр",   \
  " Тест GND АЦП2  ",   \
  " Тест VREF АЦП2 ",   \
  " Тест VDD АЦП2  ",   \
  "Тест GND АЦП2 гр",   \
  "Тест VREF АЦП2гр",   \
  "Тест VDD АЦП2 гр",   \
  " Ош. SPI АЦП    ",   \
  "Переп.буф.ц.осц.",   \
  " Ош.вых.реле 1  ",   \
  " Ош.вых.реле 2  ",   \
  " Ош.вых.реле 3  ",   \
  " Ош.вых.реле 4  ",   \
  " Ош.вых.реле 5  ",   \
  " Ош.вых.реле 6  ",   \
  " Ош.вых.реле 7  ",   \
  " Ош.вых.реле 8  ",   \
  " Ош.вых.реле 9  ",   \
  " Ош.вых.реле 10 ",   \
  "Ан.рег.вр.занят.",   \
  " Пер.буф.aн.рег.",   \
  "Неопр.ош.ан.рег.",   \
  "Потеря д.ан.рег.",   \
  " Д.рег.вр.занят.",   \
  " Неопр.ош.д.рег.",   \
  " Потеря д.д.рег.",   \
  "Переп.буф.пр.ош.",   \
  " Потеря д.пр.ош.",   \
  " Старт устр.    ",   \
  " Рестарт устр.  ",   \
  " Останов.устр.  ",   \
  " Пропад.питания ",   \
  " Отказ ЖКИ      ",   \
  " Ош.SPI_DF      ",   \
  " Ош.внешней SRAM",   \
  " Ош.внутр.FLASH ",   \
  " Ош.выб.гр.уст. ",   \
  " Пот.д.энергии  ",   \
  " Ошибка 87      ",   \
  " Ошибка 88      "

# define NAME_DIAGN_UA  \
  " Пом.I2C        ",   \
  " Пом.налаштувань",   \
  "Налаштувань нема",   \
  " Пом.зап.налашт.",   \
  "Пом.контр.налашт",   \
  " Пом.типу налашт",   \
  " Пом.юстування  ",   \
  " Юстування нема ",   \
  " Пом.зап.юст.   ",   \
  " Пом.контр.юст. ",   \
  " Пом.юст.набору ",   \
  " Пом.відн.тр.св.",   \
  " Пом.відн.с.вих.",   \
  "Інф.вих./св.нема",   \
  "Пом.зап.вих./св.",   \
  " Пом.триґ.інф.  ",   \
  " Триґ.інф.нема  ",   \
  " Пом.зап.триґ.і.",   \
  "Пом.контр.триґ.і",   \
  " Пом.інф.ан.р.  ",   \
  " Інф.ан.р.нема  ",   \
  " Пом.зап.і.ан.р.",   \
  " Пом.контр.ан.р.",   \
  " Пом.інф.д.р.   ",   \
  " Інф.д.р.нема   ",   \
  " Пом.зап.і.д.р. ",   \
  " Пом.контр.д.р. ",   \
  " Пом.інф.р.пр.п.",   \
  " Інф.р.пр.п.нема",   \
  "Пом.зап.і.р.пр.п",   \
  "Пом.контр.р.пр.п",   \
  " Пом.інф.ліч.р. ",   \
  " Інф.ліч.р.нема ",   \
  " Пом.зап.ліч.р. ",   \
  "Пом.контр.ліч.р.",   \
  " Пом.к.с.енергій",   \
  "  Енергій нема  ",   \
  " Пом.зап.енергій",   \
  "Батарея разрядж.",   \
  " Осцилятор зуп. ",   \
  " Відм.Осцилятора",   \
  " Зуп.обновл.RTC ",   \
  " Не вст.поля RTC",   \
  " Тест GND АЦП1  ",   \
  " Тест VREF АЦП1 ",   \
  " Тест VDD АЦП1  ",   \
  "Тест GND АЦП1 гр",   \
  "Тест VREF АЦП1гр",   \
  "Тест VDD АЦП1 гр",   \
  " Тест GND АЦП2  ",   \
  " Тест VREF АЦП2 ",   \
  " Тест VDD АЦП2  ",   \
  "Тест GND АЦП2 гр",   \
  "Тест VREF АЦП2гр",   \
  "Тест VDD АЦП2 гр",   \
  " Пом.SPI АЦП    ",   \
  "Переп.буф.ц.осц.",   \
  " Пом.вих.реле 1 ",   \
  " Пом.вих.реле 2 ",   \
  " Пом.вих.реле 3 ",   \
  " Пом.вих.реле 4 ",   \
  " Пом.вих.реле 5 ",   \
  " Пом.вих.реле 6 ",   \
  " Пом.вих.реле 7 ",   \
  " Пом.вих.реле 8 ",   \
  " Пом.вих.реле 9 ",   \
  " Пом.вих.реле 10",   \
  "Ан.р.тимч.зайнят",   \
  " Переп.буф.aн.р.",   \
  "Невизн.пом.ан.р.",   \
  "Втрата д.ан.р.  ",   \
  "Д.р.тимч.зайнят.",   \
  " Невизн.пом.д.р.",   \
  " Втрата д.д.р.  ",   \
  "Переп.буф.р.пр.п",   \
  " Втрата д.р.пр.п",   \
  " Старт пристр.  ",   \
  " Рестарт пристр.",   \
  " Зуп.пристр.    ",   \
  " Пропад.живлення",   \
  " Відмова РКІ    ",   \
  " Пом.SPI_DF     ",   \
  " Пом.зовн.SRAM  ",   \
  " Пом.внутр.FLASH",   \
  " Пом.виб.гр.уст.",   \
  " Втр.д.енергії  ",   \
  " Помилка 87     ",   \
  " Помилка 88     "

# define NAME_DIAGN_EN  \
  " I2C Err.       ",   \
  " Settings Err.  ",   \
  " No settings    ",   \
  " Sett.W.Err.    ",   \
  " Sett.Ctrl.Err. ",   \
  " Sett.Type Err. ",   \
  " Adjust Err.    ",   \
  " No adjust      ",   \
  " Adjust W.Err.  ",   \
  "Adjust Ctrl.Err.",   \
  " Adjust-Set Err.",   \
  "Tr.LED Rest Err.",   \
  "Sign DO Rest Err",   \
  " No DO/LED Inf. ",   \
  " DO/LED W Err.  ",   \
  "  Ош.триг.инф.  ",   \
  "  Триг.инф.нет  ",   \
  "Ош.зап.триг.инф.",   \
  "Ош.контр.триг.и.",   \
  " An.Rec.Inf.Err.",   \
  " No An.Rec.Inf. ",   \
  "An.Rec.Inf.W.Err",   \
  "An.Rec.Ctrl.Err.",   \
  " D.Rec.Inf.Err. ",   \
  " No D.Rec.Inf.  ",   \
  "Inf.D.Rec.W.Err.",   \
  " D.Rec.Ctrl.Err.",   \
  " PER Inf.Err.   ",   \
  " No Inf.of PER  ",   \
  "Inf.W.Err.of PER",   \
  " PER Ctrl.Err.  ",   \
  " Res.C.Inf.Err. ",   \
  " No Res.C.Inf.  ",   \
  "Inf.Res.C.W.Err.",   \
  " Res.C.Ctrl.Err.",   \
  " Ош.к.с.энергий ",   \
  "  Энергий нет   ",   \
  " Ош.зап.энергий ",   \
  " RTC:Battery low",   \
  " RTC:Osc.stop   ",   \
  " RTC:Osc.fail   ",   \
  " RTC:Halt update",   \
  "RTC:No def.sett.",   \
  " ADC1:GND fail  ",   \
  " ADC1:VREF fail ",   \
  " ADC1:VDD fail  ",   \
  "ADC1:GND Test R.",   \
  "ADC1:VREF Test R",   \
  "ADC1:VDD Test R.",   \
  " ADC2:GND fail  ",   \
  " ADC2:VREF fail ",   \
  " ADC2:VDD fail  ",   \
  "ADC2:GND Test R.",   \
  "ADC2:VREF Test R",   \
  "ADC2:VDD Test R.",   \
  " ADC SPI Err.   ",   \
  "Переп.буф.ц.осц.",   \
  " DO1 Ctrl.Err.  ",   \
  " DO2 Ctrl.Err.  ",   \
  " DO3 Ctrl.Err.  ",   \
  " DO4 Ctrl.Err.  ",   \
  " DO5 Ctrl.Err.  ",   \
  " DO6 Ctrl.Err.  ",   \
  " DO7 Ctrl.Err.  ",   \
  " DO8 Ctrl.Err.  ",   \
  " DO9 Ctrl.Err.  ",   \
  " DO10 Ctrl.Err. ",   \
  " An.Rec.busy    ",   \
  " An.Rec.buff.OVF",   \
  "Undef.An.Rec.Err",   \
  "An.Rec.Data lost",   \
  " D.Rec.busy     ",   \
  "Undef.D.Rec.Err.",   \
  " D.Rec.Data lost",   \
  "OVF of PER buff.",   \
  " PER Data lost  ",   \
  " Device Start   ",   \
  " Device Restart ",   \
  " Device Stop    ",   \
  " Пропад.питания ",   \
  " LCD Fail       ",   \
  " DF SPI Err.    ",   \
  " Ext.SRAM Err.  ",   \
  " Int.FLASH Err. ",   \
  " Ош.выб.гр.уст. ",   \
  " Пот.д.энергии  ",   \
  " Error 87       ",   \
  " Error 88       "

# define NAME_DIAGN_KZ  \
  " Ош.I2C         ",   \
  " Ош.настроек    ",   \
  " Настроек нет   ",   \
  " Ош.зап.настр.  ",   \
  " Ош.контр.настр.",   \
  " Ош.типа настр. ",   \
  " Ош.юстирования ",   \
  " Юстирования нет",   \
  " Ош.зап.юстир.  ",   \
  " Ош.контр.юстир.",   \
  " Ош.юстир.набора",   \
  " Ош.воcст.тр.св ",   \
  " Ош.воcст.с.вых ",   \
  "Инф.вых./св.нет ",   \
  " Ош.зап.вых./св.",   \
  "  Ош.триг.инф.  ",   \
  "  Триг.инф.нет  ",   \
  "Ош.зап.триг.инф.",   \
  "Ош.контр.триг.и.",   \
  " Ош.инф.ан.рег. ",   \
  " Инф.ан.рег.нет ",   \
  "Ош.зап.и.ан.рег.",   \
  "Ош.контр.ан.рег.",   \
  " Ош.инф.д.рег.  ",   \
  " Инф.д.рег.нет  ",   \
  " Ош.зап.и.д.рег.",   \
  " Ош.контр.д.рег.",   \
  "Ош.инф.рег.пр.с.",   \
  "Инф.рег.пр.с.нет",   \
  "Ош.зап.и.р.пр.с.",   \
  "Ош.контр.р.пр.с.",   \
  " Ош.инф.сч.рес. ",   \
  " Инф.сч.рес.нет ",   \
  " Ош.зап.сч.рес. ",   \
  "Ош.контр.сч.рес.",   \
  " Ош.к.с.энергий ",   \
  "  Энергий нет   ",   \
  " Ош.зап.энергий ",   \
  " Батарея разряж.",   \
  "Осцилятор остан.",   \
  "Отказ Осцилятора",   \
  " Ост.обновл.RTC ",   \
  " Не уст.поля RTC",   \
  " Тест GND АЦП1  ",   \
  " Тест VREF АЦП1 ",   \
  " Тест VDD АЦП1  ",   \
  " Тест GND АЦП1гр",   \
  "Тест VREF АЦП1гр",   \
  "Тест VDD АЦП1 гр",   \
  " Тест GND АЦП2  ",   \
  " Тест VREF АЦП2 ",   \
  " Тест VDD АЦП2  ",   \
  " Тест GND АЦП2гр",   \
  "Тест VREF АЦП2гр",   \
  "Тест VDD АЦП2 гр",   \
  " Ош.SPI АЦП     ",   \
  "Переп.буф.ц.осц.",   \
  " Ош.вых.реле 1  ",   \
  " Ош.вых.реле 2  ",   \
  " Ош.вых.реле 3  ",   \
  " Ош.вых.реле 4  ",   \
  " Ош.вых.реле 5  ",   \
  " Ош.вых.реле 6  ",   \
  " Ош.вых.реле 7  ",   \
  " Ош.вых.реле 8  ",   \
  " Ош.вых.реле 9  ",   \
  " Ош.вых.реле 10 ",   \
  "Ан.рег.вр.занят.",   \
  " Пер.буф.aн.рег.",   \
  "Неопр.ош.ан.рег.",   \
  "Потеря д.ан.рег.",   \
  " Д.рег.вр.занят.",   \
  " Неопр.ош.д.рег.",   \
  " Потеря д.д.рег.",   \
  "Переп.буф.пр.ош.",   \
  " Потеря д.пр.ош.",   \
  " Старт устр.    ",   \
  " Рестарт устр.  ",   \
  " Останов.устр.  ",   \
  " Пропад.питания ",   \
  " Отказ ЖКИ      ",   \
  " Ош.SPI_DF      ",   \
  " Ош.внешней SRAM",   \
  " Ош.внутр.FLASH ",   \
  " Ош.выб.гр.уст. ",   \
  " Пот.д.энергии  ",   \
  " Ошибка 87      ",   \
  " Ошибка 88      "
    
#endif
