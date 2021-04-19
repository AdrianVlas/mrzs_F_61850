/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

//#include "fatfs.h"
#include "header.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */    

void MX_FATFS_Init(void) 
{
  /*## FatFS: Link the USER driver ###########################*/
  if (FATFS_LinkDriver(&USER_Driver, USERPath) == 0)
  {
    FRESULT res = f_mount(&USERFatFS, (TCHAR const*)USERPath, 1);
    switch (res)
    {
    case FR_OK: break;
    case FR_NO_FILESYSTEM:
      {
        //чекаємо, поки з RTC буде зчитано поточний час
        while(
              (_CHECK_SET_BIT(    diagnostyka, EVENT_START_SYSTEM_BIT       ) != 0) ||
              (_CHECK_SET_BIT(set_diagnostyka, EVENT_START_SYSTEM_BIT       ) != 0) ||
              (_CHECK_SET_BIT(    diagnostyka, EVENT_RESTART_SYSTEM_BIT     ) != 0) ||
              (_CHECK_SET_BIT(set_diagnostyka, EVENT_RESTART_SYSTEM_BIT     ) != 0) ||
              (_CHECK_SET_BIT(    diagnostyka, EVENT_SOFT_RESTART_SYSTEM_BIT) != 0) ||
              (_CHECK_SET_BIT(set_diagnostyka, EVENT_SOFT_RESTART_SYSTEM_BIT) != 0)
             )
        {
//          periodical_operations_communication(((POWER_CTRL->IDR & POWER_CTRL_PIN) == (uint32_t)Bit_RESET));
          periodical_operations(false);
        }
        
        //формуємо файлову систему
        res = f_mkfs (USERPath, 1, 0);
        if (res != FR_OK) _SET_BIT(set_diagnostyka, ERROR_FATFS);
        else
        {
          if (_GET_STATE(FATFS_command, FATFS_FORMAT)) _CLEAR_STATE(FATFS_command, FATFS_FORMAT);
        }
        
        break;
      }
    default:
      {
        _SET_BIT(set_diagnostyka, ERROR_FATFS);
        break;
      }
    }

//    FIL fil;
//    res = f_open(&fil, "0.dat", FA_CREATE_ALWAYS | FA_WRITE);
//    res = f_lseek(&fil, 0);
//    
//    uint16_t array[3] = {10, 12, 1978};
//    unsigned int written;
//    res = f_write(&fil, array, 3*sizeof(uint16_t), &written);
//    res = f_close(&fil);
//    if ((res != FR_OK) || (written != 3*sizeof(uint16_t))) _SET_BIT(set_diagnostyka, ERROR_FATFS);

//    res = f_open(&fil, "0.dat", FA_READ);
//    if (res == FR_OK)
//    {
//      res = f_lseek(&fil, 0);
//    
//      uint16_t array[3*3];
//      uint8_t *p_array = (uint8_t*)array;
//      unsigned int read = 0;
//      while (!f_eof(&fil))
//      {
//        unsigned int read_tmp;
//        res = f_read(&fil, p_array++, 1, &read_tmp);
//        read += read_tmp;
//      }
//      res = f_close(&fil);
//      if (res != FR_OK) _SET_BIT(set_diagnostyka, ERROR_FATFS);
//    }

//    res = f_unlink("0.dat");
//    if (res != FR_OK) _SET_BIT(set_diagnostyka, ERROR_FATFS);
  }
  else  _SET_BIT(set_diagnostyka, ERROR_FATFS);
    

  /* USER CODE BEGIN Init */
  /* additional user code for init */     
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  time_t time_dat_tmp;
  if (save_time_dat_l == 3) time_dat_tmp = time_dat_save_l;
  else
  { 
    copying_time_dat = 1;
    time_dat_tmp = time_dat_copy;
    copying_time_dat = 0;
  }

  struct tm *p;
  p = localtime(&time_dat_tmp);
  
  DWORD res = 0;
  int temp;
  
  //Рік
  temp =  p->tm_year - 100 + 80;
  res |= temp << 25;
  
  //Місяць
  temp = p->tm_mon + 1;
  res |= temp << 21;
  
  //День місяця
  temp = p->tm_mday;
  res |= temp << 16;
  
  //Година
  temp = p->tm_hour;
  res |= temp << 11;
  
  //Хвилини
  temp = p->tm_min;
  res |= temp << 5;
  
  //Секунди
  temp = p->tm_sec;
  res |= (temp >> 1) << 0;

  return res;
  /* USER CODE END get_fattime */  
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
