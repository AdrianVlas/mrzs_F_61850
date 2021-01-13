#ifndef __CONST_MENU_TRANSFORMATOR__
#define __CONST_MENU_TRANSFORMATOR__

#define EKRAN_TRANSFORMATOR_INFO            (EKRAN_LIST_LEDS_FOR_RANGUVANNJA + 1)
#define EKRAN_TRANSFORMATOR_INFO_SETPOINT   (EKRAN_TRANSFORMATOR_INFO          + 1)
#define EKRAN_TRANSFORMATOR_INFO_CONTROL    (EKRAN_TRANSFORMATOR_INFO_SETPOINT + 1)

#define MAX_ROW_FOR_TRANSFORMATOR_INFO_SETPOINT         4  
#define MAX_ROW_FOR_TRANSFORMATOR_INFO_CONTROL          1  

#define INDEX_ML_T0                         0
#define INDEX_ML_TT                         1
#define INDEX_ML_TT04                       2
#define INDEX_ML_TN                         3

#define COL_T0_BEGIN                        6
#define COL_T0_END                          8

#define COL_TT_BEGIN                        6
#define COL_TT_END                          9

#define COL_TT04_BEGIN                      6
#define COL_TT04_END                        9

#define COL_TN_BEGIN                        6
#define COL_TN_END                          9

#define INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE           0          
#define CTR_TRANSFORMATOR_PHASE_LINE                    (1 << INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE)   

#define CTR_TRANSFORMATOR_MASKA                         (                                 \
                                                         CTR_TRANSFORMATOR_PHASE_LINE     \
                                                        )

#endif
