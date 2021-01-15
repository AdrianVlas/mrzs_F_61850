#ifndef __CONST_GLOBAL_TIMERS__
#define __CONST_GLOBAL_TIMERS__

//Величина, яка необхідна для періодичної (період 1  мс   ) генерації переривань каналом 1 таймеру 2 з прескаллером 2000 = (1999 + 1)
#define TIM2_CCR1_VAL                   1000
#define TIM2_MIN_PERIOD_WRITE_READ      1 /*мкс*/
//Величина, яка необхідна для періодичної (період 10 мс   ) генерації переривань каналом 1 таймеру 4 з прескаллером 600 = (599 + 1)
#define TIM4_CCR1_VAL                   1000
//Величина, яка необхідна для періодичної (період 1 мс   ) генерації переривань каналом 2 таймеру 4 з прескаллером 600 = (599 + 1)
#define TIM4_CCR2_VAL                   100
//Величина, яка необхідна для періодичної (як мінімум, період 10 мкс   ) генерації переривань каналом 3 таймеру 4 з прескаллером 600 = (599 + 1)
#define TIM4_CCR3_VAL                   1
//Величина, яка необхідна для періодичної (період 0,625 мс) генерації переривань каналом 1 таймеру 3 з прескаллером 1 = (0 + 1)
#define TIM5_CCR1_2_VAL               37500

#define DELTA_TIME_FOR_TIMERS           1
#define PERIOD_SIGNAL_OUTPUT_MODE_2     1000 /*мс*/

#define CLOCKS_PER_SEC (1000)

#define MAX_VALUE_TIMER_FOR_COUNT_SIGNAL_ACHR_CHAPV            4 //мс - досягнення цього значення є умовою спроби запустити сигнал АПВ (якщо він не блокується)
#define TIMEOUT_ACHR_CHAPV_100MS                               100 //мс
#define TIMEOUT_CHAPV_1MS                                      (1 + 1) /*попревка на 1 зроблена, щоб правильно працював макрос з реалізацією логіки АЧР/ЧАПВ*/
#define TIMEOUT_BLOCK_CHAPV_5MS                                5


enum _index_timer
{
  INDEX_TIMER_INPUT_START = 0,
  
  INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START = (INDEX_TIMER_INPUT_START + NUMBER_INPUTS),
  
  INDEX_TIMER_DF_PAUSE_START = (INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + NUMBER_DEFINED_FUNCTIONS),
    
  INDEX_TIMER_DF_WORK_START = (INDEX_TIMER_DF_PAUSE_START + NUMBER_DEFINED_FUNCTIONS),
  
  INDEX_TIMER_VIDKL_VV = (INDEX_TIMER_DF_WORK_START + NUMBER_DEFINED_FUNCTIONS),
  INDEX_TIMER_BLK_VKL_VV,
  INDEX_TIMER_VKL_VV,
  INDEX_TIMER_PRYVOD_VV,

  INDEX_TIMER_MTZ1,
  INDEX_TIMER_MTZ1_N_VPERED,
  INDEX_TIMER_MTZ1_N_NAZAD,
  INDEX_TIMER_MTZ1_PO_NAPRUZI,
  INDEX_TIMER_MTZ2,
  INDEX_TIMER_MTZ2_DEPENDENT,
  INDEX_TIMER_MTZ2_PR,
  INDEX_TIMER_MTZ2_N_VPERED,
  INDEX_TIMER_MTZ2_N_VPERED_PR,
  INDEX_TIMER_MTZ2_N_NAZAD,
  INDEX_TIMER_MTZ2_N_NAZAD_PR,
  INDEX_TIMER_MTZ2_PO_NAPRUZI,
  INDEX_TIMER_MTZ2_PO_NAPRUZI_PR,
  INDEX_TIMER_MTZ2_VVID_PR,
  INDEX_TIMER_MTZ3,
  INDEX_TIMER_MTZ3_N_VPERED,
  INDEX_TIMER_MTZ3_N_NAZAD,
  INDEX_TIMER_MTZ3_PO_NAPRUZI,
  INDEX_TIMER_MTZ4,
  INDEX_TIMER_MTZ4_N_VPERED,
  INDEX_TIMER_MTZ4_N_NAZAD,
  INDEX_TIMER_MTZ4_PO_NAPRUZI,
  
  INDEX_TIMER_MTZ04_1,
  INDEX_TIMER_MTZ04_2,
  INDEX_TIMER_MTZ04_3,
  INDEX_TIMER_MTZ04_4,
  INDEX_TIMER_MTZ04_5,
  
  INDEX_TIMER_ZDZ,
  
  INDEX_TIMER_ZZ_3I0,
  INDEX_TIMER_ZZ_3U0,
  INDEX_TIMER_NZZ,
  
  INDEX_TIMER_TZNP1_VPERED,
  INDEX_TIMER_TZNP1_NAZAD,
  INDEX_TIMER_TZNP2_VPERED,
  INDEX_TIMER_TZNP2_NAZAD,
  INDEX_TIMER_TZNP3_VPERED,
  INDEX_TIMER_TZNP3_NAZAD,
  
  INDEX_TIMER_ZOP,
  
  INDEX_TIMER_UROV1,
  INDEX_TIMER_UROV2,
  
  INDEX_TIMER_APV_1,
  INDEX_TIMER_APV_2,
  INDEX_TIMER_APV_3,
  INDEX_TIMER_APV_4,
  INDEX_TIMER_APV_BLOCK_VID_APV1,
  INDEX_TIMER_APV_BLOCK_VID_APV2,
  INDEX_TIMER_APV_BLOCK_VID_APV3,
  INDEX_TIMER_APV_BLOCK_VID_APV4,
  INDEX_TIMER_APV_BLOCK_VID_VV,
  INDEX_TIMER_APV_TMP1,
  INDEX_TIMER_ACHR_CHAPV,
  INDEX_TIMER_APV_TMP2,
  
  INDEX_TIMER_ACHR_CHAPV_100MS_1,
  INDEX_TIMER_CHAPV1_1MS,
  INDEX_TIMER_CHAPV2_1MS,
  INDEX_TIMER_BLOCK_CHAPV1_5MS,
  INDEX_TIMER_BLOCK_CHAPV2_5MS,
  INDEX_TIMER_ACHR1,
  INDEX_TIMER_CHAPV1,
  INDEX_TIMER_ACHR2,
  INDEX_TIMER_CHAPV2,

  INDEX_TIMER_UMIN1,
  INDEX_TIMER_UMIN2,
  
  INDEX_TIMER_UMAX1,
  INDEX_TIMER_UMAX2,
  
  INDEX_TIMER_UP1,
  
  NEXT_TIMER = (INDEX_TIMER_UP1 + NUMBER_UP)
};

enum __mtz_tmr_const {
  INDEX_TIMER_MTZ = 0,
  INDEX_TIMER_MTZ_N_VPERED,
  INDEX_TIMER_MTZ_N_NAZAD,
  INDEX_TIMER_MTZ_PO_NAPRUZI,
  /*************************************/
  NUMBER_LEVEL_TMR_CONST
};

#define MAX_NUMBER_GLOBAL_TIMERS                               NEXT_TIMER   //Максимальна кількість таймерів

#define PERIOD_ZDZ_TEST                                         (60*1000)

#endif
