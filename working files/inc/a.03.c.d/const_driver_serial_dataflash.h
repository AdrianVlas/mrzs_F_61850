#ifndef __CONST_DRIVER_SERIAL_FLASH__
#define __CONST_DRIVER_SERIAL_FLASH__

#define NUMBER_DATAFLASH_CHIP      2

#define INDEX_DATAFLASH_1           0
#define INDEX_DATAFLASH_2           1
#define INDEX_EEPROM                3

#define VAGA_SIZE_PAGE_DATAFLASH_1      8 //ця константа потрібна тільки для того, щоб можна було легко множити на 256 і ділити на 256
#define SIZE_PAGE_DATAFLASH_1           (1u << VAGA_SIZE_PAGE_DATAFLASH_1) //256 байт
#define NUMBER_PAGES_INTO_DATAFLASH_1   512

#define VAGA_SIZE_PAGE_DATAFLASH_2      9 //ця константа потрібна тільки для того, щоб можна було легко множити на 512 і ділити на 512
#define SIZE_PAGE_DATAFLASH_2           (1 << VAGA_SIZE_PAGE_DATAFLASH_2) //512 байт
#define NUMBER_PAGES_INTO_DATAFLASH_2   4096

#if SIZE_PAGE_DATAFLASH_2 >= SIZE_PAGE_DATAFLASH_1
#define SIZE_PAGE_DATAFLASH_MAX         SIZE_PAGE_DATAFLASH_2
#else
#define SIZE_PAGE_DATAFLASH_MAX         SIZE_PAGE_DATAFLASH_1
#endif

#define NUMBER_PAGES_INTO_AR            NUMBER_PAGES_INTO_DATAFLASH_2
#define NUMBER_PAGES_INTO_DR            480
#define NUMBER_PAGES_INTO_PR_ERR        (NUMBER_PAGES_INTO_DATAFLASH_1 - NUMBER_PAGES_INTO_DR)

#define NUMBER_PAGES_IN_ONE_DR_RECORD         10
#define SIZE_BUFFER_FOR_DR_RECORD             (SIZE_PAGE_DATAFLASH_1*NUMBER_PAGES_IN_ONE_DR_RECORD) 
#define MAX_NUMBER_RECORDS_INTO_DR            48
#define MIN_ADDRESS_DR_AREA                   0x0
#define SIZE_DR_AREA                          (NUMBER_PAGES_INTO_DR<<VAGA_SIZE_PAGE_DATAFLASH_1)
#define MAX_ADDRESS_DR_AREA                   (MIN_ADDRESS_DR_AREA + SIZE_DR_AREA - 1)

#define NUMBER_BYTES_SAMPLE_DR                ( ( NUMBER_TOTAL_SIGNAL_FOR_RANG / 8 ) + ( ( NUMBER_TOTAL_SIGNAL_FOR_RANG % 8 ) != 0))
#define SD_DR                                 (3 + NUMBER_BYTES_SAMPLE_DR + 2)  

#define FIRST_INDEX_START_START_RECORD_DR     0
#define FIRST_INDEX_DATA_TIME_DR              1
#define FIRST_INDEX_EXTRA_SETTINGS_DR         13
#define FIRST_INDEX_NAME_OF_CELL_DR           17
#define FIRST_INDEX_SOURCE_DR                 49
#define FIRST_INDEX_NUMBER_ITEMS_DR           (49 + NUMBER_BYTES_SAMPLE_DR)
#define FIRST_INDEX_NUMBER_CHANGES_DR         (50 + NUMBER_BYTES_SAMPLE_DR)
#define FIRST_INDEX_NUMBER_MAX_PHASE_DR       (52 + NUMBER_BYTES_SAMPLE_DR)
#define FIRST_INDEX_NUMBER_MAX_PHASE04_DR     (53 + NUMBER_BYTES_SAMPLE_DR)
#define FIRST_INDEX_NUMBER_MAX_3I0_DR         (54 + NUMBER_BYTES_SAMPLE_DR)
#define FIRST_INDEX_NUMBER_MAX_3U0_DR         (55 + NUMBER_BYTES_SAMPLE_DR)
#define FIRST_INDEX_NUMBER_MIN_U_DR           (56 + NUMBER_BYTES_SAMPLE_DR)
#define FIRST_INDEX_NUMBER_MAX_U_DR           (57 + NUMBER_BYTES_SAMPLE_DR)
#define FIRST_INDEX_NUMBER_MAX_ZOP_DR         (58 + NUMBER_BYTES_SAMPLE_DR)
#define FIRST_INDEX_NUMBER_MIN_F_ACHR_DR      (59 + NUMBER_BYTES_SAMPLE_DR)
#define FIRST_INDEX_NUMBER_F_CHAPV_DR         (60 + NUMBER_BYTES_SAMPLE_DR)
#define FIRST_INDEX_FIRST_BLOCK_DR            (61 + NUMBER_BYTES_SAMPLE_DR)
#define FIRST_INDEX_FIRST_DATA_DR             (1181 + NUMBER_BYTES_SAMPLE_DR)

#define VAGA_SIZE_ONE_RECORD_PR_ERR           6
#define SIZE_ONE_RECORD_PR_ERR                (1<<VAGA_SIZE_ONE_RECORD_PR_ERR)
#define NUMBER_RECORDS_PR_ERR_INTO_PAGE       (SIZE_PAGE_DATAFLASH_1>>VAGA_SIZE_ONE_RECORD_PR_ERR) //кількість записів програмних подій у одній строрінці мікросхеми послідовної DataFlash
#define MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER 32 //максимальнакількість записів в буфері типу FIFO, які чекають на запису DataFlash
#define SIZE_BUFFER_FOR_PR_ERR                (SIZE_ONE_RECORD_PR_ERR*MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER)
#define MAX_NUMBER_RECORDS_INTO_PR_ERR        (NUMBER_PAGES_INTO_PR_ERR*NUMBER_RECORDS_PR_ERR_INTO_PAGE)
#define MIN_ADDRESS_PR_ERR_AREA               (MAX_ADDRESS_DR_AREA + 1)
#define SIZE_PR_ERR_AREA                      (NUMBER_PAGES_INTO_PR_ERR<<VAGA_SIZE_PAGE_DATAFLASH_1)
#define MAX_ADDRESS_PR_ERR_AREA               (MIN_ADDRESS_PR_ERR_AREA + SIZE_PR_ERR_AREA - 1)

#define MASKA_BIT_FOR_PRESCALER               0x1
#define VAGA_NUMBER_POINT_AR                  (VAGA_NUMBER_POINT - 1)
#define NUMBER_POINT_AR                       (1 << VAGA_NUMBER_POINT_AR)
#define MIN_ADDRESS_AR_AREA                   0x0
#define SIZE_AR_AREA                          (NUMBER_PAGES_INTO_AR<<VAGA_SIZE_PAGE_DATAFLASH_2)
#define MAX_ADDRESS_AR_AREA                   (MIN_ADDRESS_AR_AREA + SIZE_AR_AREA - 1)


#define TRANSACTION_EXECUTING_NONE              0
#define TRANSACTION_EXECUTING                   1
#define TRANSACTION_EXECUTED_WAIT_ANALIZE       2

#define CODE_OPERATION_NONE                                 0
#define CODE_OPERATION_STATUS_READ                          1
#define CODE_OPERATION_PAGESIZE_256                         2
#define CODE_OPERATION_ERASE                                3
#define CODE_OPERATION_WRITE_PAGE_THROUGH_BUFFER            4
#define CODE_OPERATION_READ_HIGH_FREQ                       5
#define CODE_OPERATION_READ_PAGE_INTO_BUFFER                6
#define CODE_OPERATION_WRITE_BUFFER                         7
#define CODE_OPERATION_WRITE_BUFFER_INTO_MEMORY_WITH_ERASE  8

/*Біти 0-15 відносять до мікросхеми DataFlash дискретного реєстратора і реєстратора програмних подій*/
#define TASK_ERASE_DATAFLASH_1_BIT                                          0
#define TASK_ERASE_DATAFLASH_1                                              (1<<TASK_ERASE_DATAFLASH_1_BIT)
#define TASK_WRITE_PR_ERR_RECORDS_INTO_DATAFLASH_BIT                        1
#define TASK_WRITE_PR_ERR_RECORDS_INTO_DATAFLASH                            (1<<TASK_WRITE_PR_ERR_RECORDS_INTO_DATAFLASH_BIT)
#define TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR_BIT        2
#define TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR            (1<<TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR_BIT)
#define TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU_BIT                          3
#define TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU                              (1<<TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU_BIT)
#define TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU_BIT                      4
#define TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU                          (1<<TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU_BIT)
#define TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB_BIT                           5
#define TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB                               (1<<TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB_BIT)
#define TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485_BIT                         6
#define TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485                             (1<<TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485_BIT)
#define TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB_BIT                       7
#define TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB                           (1<<TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB_BIT)
#define TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485_BIT                     8
#define TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485                         (1<<TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485_BIT)

#define  SEPARATOR_BIT_TASKS_DATADLASH1_AND_TASKS_DATADLASH2                16

/*Біти 16-31 відносять до мікросхеми DataFlash аналогового реєстратора*/
#define TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_FS_BIT   (0 + SEPARATOR_BIT_TASKS_DATADLASH1_AND_TASKS_DATADLASH2)
#define TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_FS       (1<<TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_FS_BIT)
#define TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_FS_BIT        (1 + SEPARATOR_BIT_TASKS_DATADLASH1_AND_TASKS_DATADLASH2)
#define TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_FS            (1<<TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_FS_BIT)
#define TASK_MAMORY_READ_DATAFLASH_FOR_FS_BIT                               (2 + SEPARATOR_BIT_TASKS_DATADLASH1_AND_TASKS_DATADLASH2)
#define TASK_MAMORY_READ_DATAFLASH_FOR_FS                                   (1<<TASK_MAMORY_READ_DATAFLASH_FOR_FS_BIT)

#define WIGHT_OF_DR_WAITING                       2

#define STATE_DR_NO_RECORD                        0 //На даний момент ніких дій з дискретним реєстратором не виконується
#define STATE_DR_FORCE_START_NEW_RECORD           1 //Оформлені події оформити як запис, але ще не досягнуто умови завершення роботи дискретного реєстратора тому при наступному проході розпочати новий запис
#define STATE_DR_EXECUTING_RECORD                 2 //Іде процес запису реєстратора
#define STATE_DR_MAKE_RECORD                      3 //Оформити запис для  запису у Dataflash
#define STATE_DR_CUT_RECORD                       4 //Оформлені події оформити як запис, але ще не досягнуто умови завершення роботи дискретного реєстратора тому при наступному проході розпочати новий запис

#define MAX_TIME_OFFSET_FROM_START                      0xFFFFFF // 4 год. 39 хв. 37 сек. 215 мілісек.
#define MAX_EVENTS_IN_ONE_RECORD                        36
#define MAX_NUMBER_FIX_MAX_MEASUREMENTS                 10
#define SIZE_ARRAY_FIX_MAX_MEASUREMENTS                 (NUMBER_ANALOG_CANALES + 20)

#define LABEL_START_RECORD_DR           0xA5

enum _identifier_bit_arrays
{
  IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE  = 1,
  IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE04,
  IDENTIFIER_BIT_ARRAY_MAX_CURRENT_3I0,
  IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE_3U0,
  IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE,
  IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE,
  IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP,
  IDENTIFIER_BIT_ARRAY_MIN_FREQUENCY_ACHR,
  IDENTIFIER_BIT_ARRAY_FREQUENCY_CHAPV
};

#define READING_DR_FOR_MENU             1
#define READING_DR_FOR_USB              2
#define READING_DR_FOR_RS485            3
#define READING_PR_ERR_FOR_MENU         4
#define READING_PR_ERR_FOR_USB          5
#define READING_PR_ERR_FOR_RS485        6


#define LABEL_START_RECORD_PR_ERR       0x5A

#define ETAP_NONE                                 0
#define ETAP_READ_MEMORY_INTO_BUFFER              1
#define ETAP_HAVE_READ_MEMORY_INTO_BUFFER         2
#define ETAP_MODIFY_AND_WRITE_BUFFER              3
#define ETAP_MODIFIED_AND_WRITTEN_BUFFER          4
#define ETAP_WRITE_BUFFER_INTO_MEMORY             5
#define ETAP_ERROR_BEFALLEN                       6

#define LABEL_START_RECORD_AR           0xAB

#define NUMBER_WORD_DIGITAL_PART_AR ( ( NUMBER_TOTAL_SIGNAL_FOR_RANG / (8*sizeof(short int)) ) + ( ( NUMBER_TOTAL_SIGNAL_FOR_RANG % (8*sizeof(short int))) != 0))

#define AR_TAIL_MIN_NUMBER_PERIOD       1
#define POWER_DOWN_TIME      70
#define POWEER_IS_FROM_IA_IC       3850
#define POWEER_ISNOT_FROM_IA_IC    3500

enum _state_ar_m
{
  STATE_AR_NONE_M = 0,
  STATE_AR_WORK_M,
  STATE_AR_WORK_STOP_M,
  STATE_AR_BLOCK_M
};

enum _state_ar_prt
{
  STATE_AR_NONE_PRT = 0,
  STATE_AR_AVAR_PRT,
  STATE_AR_POSTAVAR_PRT,
  STATE_AR_BLOCK_PRT
};

enum _state_ar_fatfs
{
  STATE_AR_NONE_FATFS = 0,
  STATE_AR_WAIT_TO_WRITE_FATFS,
  STATE_AR_WRITE_FATFS,
  STATE_AR_WAIT_TO_STOP_WRITE_FATFS,
  STATE_AR_STOP_WRITE_FATFS,

  STATE_AR_MEMORY_FULL_FATFS,
  STATE_AR_BLOCK_FATFS
};

enum _level_ar_writing
{
  LEVEL_AR_WRITING_SEL_NUMBER = 0,
  LEVEL_AR_WRITING_OPEN_FILE,
  LEVEL_AR_WRITING_HEADER,
  LEVEL_AR_REWRITING_DATE_TIME,
  LEVEL_AR_WRITING_DATA
};

enum _fix_date_time_avar
{
  AVAR_DATE_TIME_NONE = 0,
  AVAT_DATE_TIMR_FIX_LEVEL1,
  AVAT_DATE_TIMR_FIX_LEVEL2
};

enum _command_fatfs
{
  FATFS_FORMAT = 0,
  
  FATFS_READ_DATA_FOR_MENU,
  
  FATFS_MAX_NUMBER
};

#define MAX_TIME_FULL_AR_RECORD         40000 /*120000*/

/*
Я не використовую довгих імен, а тому формат у мене 8.3
99 999 999 = 0х5F5 E0FF
ому я роблю обмеження на 0xFF FFFF (це число 16 777 215)
*/
#define NUMBER_FATFS_NAME       0x1000000       
#define MASKA_FATFS_NAME        (NUMBER_FATFS_NAME - 1)

#define SIZE_BUFFER_FOR_AR    ((10349)*(NUMBER_ANALOG_CANALES + NUMBER_WORD_DIGITAL_PART_AR))

#define CLEAN_AR_BIT          0
#define CLEAN_AR              (1 << CLEAN_AR_BIT)
#define CLEAN_DR_BIT          1
#define CLEAN_DR              (1 << CLEAN_DR_BIT)
#define CLEAN_PR_ERR_BIT      2
#define CLEAN_PR_ERR          (1 << CLEAN_PR_ERR_BIT)

#endif
