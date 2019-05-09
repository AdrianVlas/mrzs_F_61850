#ifndef __CONSTANTS_INPUTS__
#define __CONSTANTS_INPUTS__

#define EKRAN_LIST_INPUTS_FOR_RANGUVANNJA        (EKRAN_CONTROL_UP + 1)

#define EKRAN_RANGUVANNJA_INPUT_1                (EKRAN_TRANSFORMATOR_INFO_CONTROL + 1)
#define EKRAN_RANGUVANNJA_INPUT_2                (EKRAN_RANGUVANNJA_INPUT_1 + 1)
#define EKRAN_RANGUVANNJA_INPUT_3                (EKRAN_RANGUVANNJA_INPUT_2 + 1)
#define EKRAN_RANGUVANNJA_INPUT_4                (EKRAN_RANGUVANNJA_INPUT_3 + 1)
#define EKRAN_RANGUVANNJA_INPUT_5                (EKRAN_RANGUVANNJA_INPUT_4 + 1)
#define EKRAN_RANGUVANNJA_INPUT_6                (EKRAN_RANGUVANNJA_INPUT_5 + 1)
#define EKRAN_RANGUVANNJA_INPUT_7                (EKRAN_RANGUVANNJA_INPUT_6 + 1)
#define EKRAN_RANGUVANNJA_INPUT_8                (EKRAN_RANGUVANNJA_INPUT_7 + 1)

#if (                                \
     (MODYFIKACIA_VERSII_PZ == 0) || \
     (MODYFIKACIA_VERSII_PZ == 1) || \
     (MODYFIKACIA_VERSII_PZ == 3)    \
    )

#define EKRAN_RANGUVANNJA_INPUT_9                (EKRAN_RANGUVANNJA_INPUT_8 + 1)
#define EKRAN_RANGUVANNJA_INPUT_10               (EKRAN_RANGUVANNJA_INPUT_9 + 1)
#define EKRAN_RANGUVANNJA_INPUT_11               (EKRAN_RANGUVANNJA_INPUT_10 + 1)
#define EKRAN_RANGUVANNJA_INPUT_12               (EKRAN_RANGUVANNJA_INPUT_11 + 1)
#define EKRAN_RANGUVANNJA_INPUT_13               (EKRAN_RANGUVANNJA_INPUT_12 + 1)
#define EKRAN_RANGUVANNJA_INPUT_14               (EKRAN_RANGUVANNJA_INPUT_13 + 1)
#define EKRAN_RANGUVANNJA_INPUT_15               (EKRAN_RANGUVANNJA_INPUT_14 + 1)
#define EKRAN_RANGUVANNJA_INPUT_16               (EKRAN_RANGUVANNJA_INPUT_15 + 1)

#if (MODYFIKACIA_VERSII_PZ == 0)

#define EKRAN_RANGUVANNJA_INPUT_17               (EKRAN_RANGUVANNJA_INPUT_16 + 1)
#define EKRAN_RANGUVANNJA_INPUT_18               (EKRAN_RANGUVANNJA_INPUT_17 + 1)
#define EKRAN_RANGUVANNJA_INPUT_19               (EKRAN_RANGUVANNJA_INPUT_18 + 1)
#define EKRAN_RANGUVANNJA_INPUT_20               (EKRAN_RANGUVANNJA_INPUT_19 + 1)

#endif

#endif

#if (MODYFIKACIA_VERSII_PZ == 0)

#define EKRAN_RANGUVANNJA_INPUT_LAST             EKRAN_RANGUVANNJA_INPUT_20

#elif (                                 \
       (MODYFIKACIA_VERSII_PZ == 1) ||  \
       (MODYFIKACIA_VERSII_PZ == 3)     \
      )   

#define EKRAN_RANGUVANNJA_INPUT_LAST             EKRAN_RANGUVANNJA_INPUT_16

#else

#define EKRAN_RANGUVANNJA_INPUT_LAST             EKRAN_RANGUVANNJA_INPUT_8

#endif

#define MAX_ROW_LIST_INPUTS_FOR_RANGUVANNJA       NUMBER_INPUTS
#define MAX_ROW_RANGUVANNJA_INPUT                 NUMBER_TOTAL_SIGNAL_FOR_RANG_SMALL

#define NAME_RANG_SMALL_RU         \
  "  Блок.вкл.ВВ   ",                   \
  " Сброс индикации",                   \
  "   Сброс реле   ",                   \
  " Местн./Дистанц.",                   \
  "  Положение ВВ  ",                   \
  "Откл.от вн.защит",                   \
  "     Вкл.ВВ     ",                   \
  " Контроль Вкл.  ",                   \
  "    Откл.ВВ     ",                   \
  " Контроль Откл. ",                   \
  " 1-я гр.уставок ",                   \
  " 2-я гр.уставок ",                   \
  " 3-я гр.уставок ",                   \
  " 4-я гр.уставок ",                   \
  " С.блк.Гот.к ТУ ",                   \
  "   Блок.МТЗ1    ",                   \
  "   Блок.МТЗ2    ",                   \
  " Блок.уск.МТЗ2  ",                   \
  "   Блок.МТЗ3    ",                   \
  "   Блок.МТЗ4    ",                   \
  "Блок.МТЗ 0.4кВ 1",                   \
  "Блок.МТЗ 0.4кВ 2",                   \
  "Бл.у.МТЗ 0.4кВ 2",                   \
  "    Блок.ЗДЗ    ",                   \
  " Пуск ЗДЗ от ДВ ",                   \
  "    Блок.НЗЗ    ",                   \
  "   Блок.ТЗНП1   ",                   \
  "   Блок.ТЗНП2   ",                   \
  "   Блок.ТЗНП3   ",                   \
  " Стат.блок.АПВ  ",                   \
  " АЧР/ЧАПВ от ДВ ",                   \
  "   Блок.АЧР1    ",                   \
  "   Блок.АЧР2    ",                   \
  " Пуск УРОВ от ДВ",                   \
  " Блок.ЗОП(КОФ)  ",                   \
  "  Блок.ЗНмин1   ",                   \
  "  Пуск ЗНмин1   ",                   \
  "  Блок.ЗНмин2   ",                   \
  "  Пуск ЗНмин2   ",                   \
  "  Блок.ЗНмакс1  ",                   \
  "  Блок.ЗНмакс2  ",                   \
  "    Блок.УЗx    ",                   \
  " Вх.О-функции1  ",                   \
  " Вх.О-функции2  ",                   \
  " Вх.О-функции3  ",                   \
  " Вх.О-функции4  ",                   \
  " Вх.О-функции5  ",                   \
  " Вх.О-функции6  ",                   \
  " Вх.О-функции7  ",                   \
  " Вх.О-функции8  ",                   \
  " Уст.О-триггера1",                   \
  " Сбр.О-триггера1",                   \
  " Уст.О-триггера2",                   \
  " Сбр.О-триггера2",                   \
  " Уст.О-триггера3",                   \
  " Сбр.О-триггера3",                   \
  " Уст.О-триггера4",                   \
  " Сбр.О-триггера4"

#define NAME_RANG_SMALL_UA         \
  " Блок.ввімкн.ВВ ",                   \
  " Скид.індикації ",                   \
  "   Скид.реле    ",                   \
  " Місц./Дистанц. ",                   \
  "    Стан ВВ     ",                   \
  "Вимк.від зовн.з.",                   \
  "    Ввімк.ВВ    ",                   \
  " Контроль Ввімк.",                   \
  "    Вимк.ВВ     ",                   \
  " Контроль Вимк. ",                   \
  " 1-а гр.уставок ",                   \
  " 2-а гр.уставок ",                   \
  " 3-а гр.уставок ",                   \
  " 4-а гр.уставок ",                   \
  " С.блк.Гот.до ТУ",                   \
  "   Блок.МСЗ1    ",                   \
  "   Блок.МСЗ2    ",                   \
  " Блок.приск.МСЗ2",                   \
  "   Блок.МСЗ3    ",                   \
  "   Блок.МСЗ4    ",                   \
  "Блок.МСЗ 0.4кВ 1",                   \
  "Блок.МСЗ 0.4кВ 2",                   \
  "Бл.п.МСЗ 0.4кВ 2",                   \
  "    Блок.ЗДЗ    ",                   \
  " Пуск ЗДЗ від ДВ",                   \
  "    Блок.НЗЗ    ",                   \
  "   Блок.СЗНП1   ",                   \
  "   Блок.СЗНП2   ",                   \
  "   Блок.СЗНП3   ",                   \
  " Стат.блок.АПВ  ",                   \
  " АЧР/ЧАПВ від ДВ",                   \
  "   Блок.АЧР1    ",                   \
  "   Блок.АЧР2    ",                   \
  "Пуск ПРВВ від ДВ",                   \
  " Блок.ЗЗП(КОФ)  ",                   \
  "  Блок.ЗНмин1   ",                   \
  "  Пуск ЗНмин1   ",                   \
  "  Блок.ЗНмин2   ",                   \
  "  Пуск ЗНмин2   ",                   \
  "  Блок.ЗНмакс1  ",                   \
  "  Блок.ЗНмакс2  ",                   \
  "    Блок.УЗx    ",                   \
  " Вх.В-функції1  ",                   \
  " Вх.В-функції2  ",                   \
  " Вх.В-функції3  ",                   \
  " Вх.В-функції4  ",                   \
  " Вх.В-функції5  ",                   \
  " Вх.В-функції6  ",                   \
  " Вх.В-функції7  ",                   \
  " Вх.В-функції8  ",                   \
  " Вст.В-триґера1 ",                   \
  " Ск.В-триґера1  ",                   \
  " Вст.В-триґера2 ",                   \
  " Ск.В-триґера2  ",                   \
  " Вст.В-триґера3 ",                   \
  " Ск.В-триґера3  ",                   \
  " Вст.В-триґера4 ",                   \
  " Ск.В-триґера4  "

#define NAME_RANG_SMALL_EN         \
  " ON CB Blocking ",                   \
  "Indication Reset",                   \
  "  Relay Reset   ",                   \
  "  Local/Remote  ",                   \
  "  State of CB   ",                   \
  "Откл.от вн.защит",                   \
  "     On CB      ",                   \
  " On CB Control  ",                   \
  "     Off CB     ",                   \
  " Off CB Control ",                   \
  " Pick-up Set 1  ",                   \
  " Pick-up Set 2  ",                   \
  " Pick-up Set 3  ",                   \
  " Pick-up Set 4  ",                   \
  " С.блк.Гот.к ТУ ",                   \
  "  Blc.of OCP1   ",                   \
  "  Blc.of OCP2   ",                   \
  "  OCP2 Acc.Blc. ",                   \
  "  Blc.of OCP3   ",                   \
  "  Blc.of OCP4   ",                   \
  "OCP 0.4kV 1 Blc.",                   \
  "OCP 0.4kV 2 Blc.",                   \
  "OCP 0.4kV 2 Ac.B",                   \
  "    Блок.ЗДЗ    ",                   \
  " Пуск ЗДЗ от ДВ ",                   \
  "    Блок.НЗЗ    ",                   \
  "   Блок.ТЗНП1   ",                   \
  "   Блок.ТЗНП2   ",                   \
  "   Блок.ТЗНП3   ",                   \
  " Стат.блок.АПВ  ",                   \
  " АЧР/ЧАПВ от ДВ ",                   \
  "   Блок.АЧР1    ",                   \
  "   Блок.АЧР2    ",                   \
  " CBFP Start f.DI",                   \
  "  Blc.of NPSP   ",                   \
  "   Блок.Umin1   ",                   \
  "   Пуск Umin1   ",                   \
  "   Блок.Umin2   ",                   \
  "   Пуск Umin2   ",                   \
  "   Блок.Umax1   ",                   \
  "   Блок.Umax2   ",                   \
  "   Blc.of UPx   ",                   \
  "    UDF1 In     ",                   \
  "    UDF2 In     ",                   \
  "    UDF3 In     ",                   \
  "    UDF4 In     ",                   \
  "    UDF5 In     ",                   \
  "    UDF6 In     ",                   \
  "    UDF7 In     ",                   \
  "    UDF8 In     ",                   \
  "UD Flip-Flop1 S.",                   \
  "UD Flip-Flop1 R.",                   \
  "UD Flip-Flop2 S.",                   \
  "UD Flip-Flop2 R.",                   \
  "UD Flip-Flop3 S.",                   \
  "UD Flip-Flop3 R.",                   \
  "UD Flip-Flop4 S.",                   \
  "UD Flip-Flop4 R."
        
#define NAME_RANG_SMALL_KZ         \
  "  Блок.вкл.ВВ   ",                   \
  " Сброс индикации",                   \
  "   Сброс реле   ",                   \
  " Местн./Дистанц.",                   \
  "  Положение ВВ  ",                   \
  "Откл.от вн.защит",                   \
  "     Вкл.ВВ     ",                   \
  " Контроль Вкл.  ",                   \
  "    Откл.ВВ     ",                   \
  " Контроль Откл. ",                   \
  " 1-я гр.уставок ",                   \
  " 2-я гр.уставок ",                   \
  " 3-я гр.уставок ",                   \
  " 4-я гр.уставок ",                   \
  " С.блк.Гот.к ТУ ",                   \
  "   Блок.МТЗ1    ",                   \
  "   Блок.МТЗ2    ",                   \
  " Блок.уск.МТЗ2  ",                   \
  "   Блок.МТЗ3    ",                   \
  "   Блок.МТЗ4    ",                   \
  "Блок.МТЗ 0.4кВ 1",                   \
  "Блок.МТЗ 0.4кВ 2",                   \
  "Бл.у.МТЗ 0.4кВ 2",                   \
  "    Блок.ЗДЗ    ",                   \
  " Пуск ЗДЗ от ДВ ",                   \
  "    Блок.НЗЗ    ",                   \
  "   Блок.ТЗНП1   ",                   \
  "   Блок.ТЗНП2   ",                   \
  "   Блок.ТЗНП3   ",                   \
  " Стат.блок.АПВ  ",                   \
  " АЧР/ЧАПВ от ДВ ",                   \
  "   Блок.АЧР1    ",                   \
  "   Блок.АЧР2    ",                   \
  " Пуск УРОВ от ДВ",                   \
  " Блок.ЗОП(КОФ)  ",                   \
  "  Блок.ЗНмин1   ",                   \
  "  Пуск ЗНмин1   ",                   \
  "  Блок.ЗНмин2   ",                   \
  "  Пуск ЗНмин2   ",                   \
  "  Блок.ЗНмакс1  ",                   \
  "  Блок.ЗНмакс2  ",                   \
  "    Блок.УЗx    ",                   \
  " Вх.О-функции1  ",                   \
  " Вх.О-функции2  ",                   \
  " Вх.О-функции3  ",                   \
  " Вх.О-функции4  ",                   \
  " Вх.О-функции5  ",                   \
  " Вх.О-функции6  ",                   \
  " Вх.О-функции7  ",                   \
  " Вх.О-функции8  ",                   \
  " Уст.О-триггера1",                   \
  " Сбр.О-триггера1",                   \
  " Уст.О-триггера2",                   \
  " Сбр.О-триггера2",                   \
  " Уст.О-триггера3",                   \
  " Сбр.О-триггера3",                   \
  " Уст.О-триггера4",                   \
  " Сбр.О-триггера4"
        
#endif 
