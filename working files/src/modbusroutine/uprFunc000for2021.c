
#include "header.h"

#define CLRVALID_DATA  0

extern int upravlSetting;//флаг Setting
extern int upravlSchematic;//флаг Rang

int uprFunc000for2021(int actControl, int fileRecordNumber, int registerNumber, uint32_t *uprMaska, int validData, uint32_t **editControl);
int isValidCONFCondition(unsigned int confMaska);
int isValidZZTYPECondition(unsigned int extraMaska);

/**************************************/
//База данных управления
/**************************************/

void uprFuncRead000for2021(int fileRecordNumber, int registerNumber, uint32_t *uprMaska, uint32_t **editControl)
{
  uprFunc000for2021(0, fileRecordNumber, registerNumber, uprMaska, CLRVALID_DATA, editControl);
}//uprFuncRead000for2021
void uprFuncValidWrite000for2021(int action, int fileRecordNumber, int registerNumber, uint32_t *uprMaska, uint32_t **editControl)
{
  uprFunc000for2021(action, fileRecordNumber, registerNumber, uprMaska, CLRVALID_DATA, editControl);
}//uprFuncValidWrite000for2021

int setUprRegister000for21(int fileRecordNumber, int registerNumber, int dataReg)
{
  uint32_t uprMaska = 0;
  uint32_t *editControl = NULL;
  if(!(dataReg == 0 || dataReg == 1)) return MARKER_ERRORDIAPAZON;
  if(dataReg) //валидация на включение
    if(!uprFunc000for2021(0, fileRecordNumber, registerNumber, &uprMaska, CLRVALID_DATA, &editControl)) return MARKER_ERRORDIAPAZON;
  return 0;
}//setUprRegister21

int setGroupUprRegister000for21(int fileRecordNumber, int registerNumber, int countGroupBit, int dataReg)
{
  uint32_t uprMaska = 0;
  uint32_t *editControl = NULL;

  if(dataReg&((uint32_t)(~((1<<countGroupBit)-1)))) return MARKER_ERRORDIAPAZON;

  for(int item=0; item<countGroupBit; item++)
  {
    if(dataReg&(1<<item)) //валидация на включение
      if(!uprFunc000for2021(0, fileRecordNumber, (item<<16)//registerFractNumber дробова частина номер регістру
                            +registerNumber, &uprMaska, CLRVALID_DATA, &editControl)) return MARKER_ERRORDIAPAZON;
  }//for

  return 0;
}//setGroupUprRegister21

int getUprRegister000for20(int fileRecordNumber, int registerNumber)
{
  uint32_t uprMaska = 0;
  uint32_t *editControl = NULL;
  uprFuncRead000for2021(fileRecordNumber, registerNumber, &uprMaska, &editControl);
  if(editControl!=NULL) //читать значение
    if((*editControl) & (1<<uprMaska)) return 1;
  return 0;
}//getUprRegister20

int getGroupUprRegister000for20(int fileRecordNumber, int registerNumber, int countGroupBit)
{
  unsigned short tempReadShort = 0;
  for(int item=0; item<countGroupBit; item++)
  {
    int value =0;

    uint32_t uprMaska=0;
    uint32_t *editControl=NULL;
    uprFuncRead000for2021(fileRecordNumber, (item<<16)//registerFractNumber дробова частина номер регістру
                          +registerNumber, &uprMaska, &editControl);
    if(editControl==NULL) value=0;
    else
    {
      //читать значение
      if(uprMaska == 0xFFFFFFFF) value = 0;
      else value = (*editControl)&(1<<uprMaska);
    }//else

    int tsdata = 0;
    if(value) tsdata=1;
    tempReadShort |= tsdata<<item;
  }//for
  return tempReadShort;
}//getGroupUprRegister20

int postGroupUprRegister000for21(int fileRecordNumber, int registerNumber, int countGroupBit, unsigned int value)
{
  for(int item=0; item<countGroupBit; item++)
  {
    uint32_t uprMaska = 0;
    uint32_t *editControl = NULL;
    uprFuncValidWrite000for2021(0, fileRecordNumber, (item<<16)//registerFractNumber дробова частина номер регістру
                                +registerNumber, &uprMaska, &editControl);
    if(editControl == NULL) continue;
    if(uprMaska == 0xFFFFFFFF) continue;

    int temp = 0;
    if(value & (1<<item)) temp = 1;

    //установка изменений
    (*editControl) &= ~(1<<uprMaska);
    (*editControl) |= (temp<<uprMaska);

    //actControl
    if(temp)//валидация на включение
      uprFuncValidWrite000for2021(1, fileRecordNumber, (item<<16)//registerFractNumber дробова частина номер регістру
                                  +registerNumber, &uprMaska, &editControl);

    //ОСОБАЯ СБОРКА
    if(  (editControl == (uint32_t*)&edition_settings.control_extra_settings_1) ||
         (editControl == (uint32_t*)&edition_settings.configuration)
      )
    {
      if(!value)//валидация на отключение
        if(!uprFunc000for2021(1,
                              fileRecordNumber, (item<<16)//registerFractNumber дробова частина номер регістру
                              +registerNumber,
                              &uprMaska, CLRVALID_DATA, &editControl)) return ERROR_VALID3;//ошибка валидации
      upravlSchematic = 1;//флаг Rang
    }//if
  }//for

  upravlSetting = 1;//флаг Setting
  return 0;
}//postGroupUprRegister21

int postUprRegister000for21(int fileRecordNumber, int registerNumber, unsigned int value)
{
  uint32_t uprMaska = 0;
  uint32_t *editControl = NULL;

  uprFuncValidWrite000for2021(0,//action
                              fileRecordNumber, registerNumber,
                              &uprMaska, &editControl);
  if(editControl == NULL) return 1;//continue;

  //установка изменений
  (*editControl) &= ~(1<<uprMaska);
  (*editControl) |= (value<<uprMaska);

  //actControl
  if(value)//валидация на включение
    if(!uprFunc000for2021(1, //action
                          fileRecordNumber, registerNumber,
                          &uprMaska, 1, &editControl)) return ERROR_VALID3;//ошибка валидации

  if(  (editControl == (uint32_t*)&edition_settings.control_extra_settings_1) ||
       (editControl == (uint32_t*)&edition_settings.configuration)
    )
  {
    if(!value)//валидация на отключение
      if(!uprFunc000for2021(1,
                            fileRecordNumber, registerNumber,
                            &uprMaska, CLRVALID_DATA, &editControl)) return ERROR_VALID3;//ошибка валидации
    upravlSchematic = 1;//флаг Rang
  }//if

  upravlSetting = 1;//флаг Setting
  return 0;
}//postUprRegister000for21

int isValidCONFCondition(unsigned int confMaska)
{
  if(!(edition_settings.configuration&(1<<confMaska))) return 1;
  return 0;
}//isValidCONFCondition(unsigned int confControl, unsigned int confMaska, uint32_t uprMaska, int validData)
int isValidZZTYPECondition(unsigned int extraMaska)
{
  if(edition_settings.control_zz&(1<<extraMaska)) return 1;
  return 0;
}//isValidZZTYPECondition(unsigned int extraControl, unsigned int extraMaska, uint32_t uprMaska, int validData)

int uprFunc000for2021(int actControl, int fileRecordNumber, int registerNumber, uint32_t *uprMaska, int validData, uint32_t **editControl)
{
//База даних управління
  int fileNumber       = fileRecordNumber  & 0xFFFF0000;//номер файлу
  int recordNumber     = fileRecordNumber  & 0x0000FFFF;//номер запису
  int registerWholeNumber = registerNumber & 0x0000FFFF;//ціла частина номер регістру
  int registerFractNumber = registerNumber & 0xFFFF0000;//дробова частина номер регістру

  int isValid = 1;
  (*uprMaska) = 0xFFFFFFFF;

  switch(fileNumber)
  {
  case (FILENUMBER(FILEUVV_DOUTREGISTER2021)):
    (*uprMaska)   = recordNumber-1;//idx_bit

    switch(registerNumber)
    {
    case 1:
      (*editControl) = (uint32_t*)&edition_settings.type_of_output;
      break;
    case 2:
      (*editControl) = (uint32_t*)&edition_settings.type_of_output_modif;
      break;
    }//switch(recordOffset)
    break;

  case (FILENUMBER(FILEUVV_DVREGISTER2021)):
    (*uprMaska)   = recordNumber-1;//idx_bit

    switch(registerNumber)
    {
    case 1:
      (*editControl) = (uint32_t*)&edition_settings.type_of_input_signal;
      break;
    case 2:
      (*editControl) = (uint32_t*)&edition_settings.type_of_input;
      break;
    }//switch(recordOffset)
    break;

  case (FILENUMBER(FILEUVV_SDIREGISTER2021)):
    (*uprMaska)   = recordNumber-1;//idx_bit

    switch(registerNumber)
    {
    case 1:
      (*editControl) = (uint32_t*)&edition_settings.type_of_led;
      break;
    }//switch(recordOffset)
    break;

////UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+
  case (FILENUMBER(FILEPROT_UPREGISTER2021)):
  {
    if(!(((recordNumber/100) <= 7) && ((recordNumber%100) == 1))) break;//1 101 201 301 401 501 601 701
    (*editControl) = &edition_settings.control_UP;
    if(isValidCONFCondition(UP_BIT_CONFIGURATION)) isValid = 0;
    (*uprMaska)   = (registerFractNumber>>16)+(recordNumber/100)*3;//maska;
    if(actControl)
    {
      //Функція обновлення коефіцієнта повернення після зміни налаштувань УЗ
      action_after_changing_ctrl_UP(&edition_settings, edition_settings.control_UP);

    }//if(actControl)
  }//FILENUMBER(FILEPROT_UPREGISTER2021)
  break;
////UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-
////MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+
  case (FILENUMBER(FILEPERIF_ELMFTREGISTER2021)):
  {
    (*uprMaska)   = recordNumber/4;
    (*editControl) = &edition_settings.type_df;
  }//FILENUMBER(FILEPERIF_MFTREGISTER2021
  break;
////MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-

  }//switch(fileOffset)

  if((*uprMaska)==0xFFFFFFFF)
    switch(fileRecordNumber)
    {
////MTZ+MTZ+MTZ+MTZ+MTZ+MTZ+MTZ+MTZ+MTZ+MTZ
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = MTZ_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_mtz;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = N_BIT_CTRMTZ_1;
        break;
      case 0x10000:
        (*uprMaska)   = N_BIT_CTRMTZ_1_VPERED;
        break;
      case 0x20000:
        (*uprMaska)   = N_BIT_CTRMTZ_1_NAZAD;
        break;
      }//switch(registerFractNumber)
      break;

    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(101)):
      (*editControl) = &edition_settings.control_mtz;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = N_BIT_CTRMTZ_2;
        break;
      case 0x10000:
        (*uprMaska)   = N_BIT_CTRMTZ_2_VPERED;
        break;
      case 0x20000:
        (*uprMaska)   = N_BIT_CTRMTZ_2_NAZAD;
        break;
      case 0x30000:
        (*uprMaska)   = N_BIT_CTRMTZ_2_PRYSKORENNJA;
        break;
      case 0x40000:
        (*uprMaska)   = N_BIT_CTRMTZ_2_PRYSKORENA;
        break;
      }//switch(registerFractNumber)
      break;

    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(201)):
      (*editControl) = &edition_settings.control_mtz;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = N_BIT_CTRMTZ_3;
        break;
      case 0x10000:
        (*uprMaska)   = N_BIT_CTRMTZ_3_VPERED;
        break;
      case 0x20000:
        (*uprMaska)   = N_BIT_CTRMTZ_3_NAZAD;
        break;
      }//switch(registerFractNumber)
      break;

    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(301)):
      (*editControl) = &edition_settings.control_mtz;
      if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = N_BIT_CTRMTZ_4;
        break;
      case 0x10000:
        (*uprMaska)   = N_BIT_CTRMTZ_4_VPERED;
        break;
      case 0x20000:
        (*uprMaska)   = N_BIT_CTRMTZ_4_NAZAD;
        break;
      case 0x60000:
        (*uprMaska)   = N_BIT_CTRMTZ_NESPR_KIL_NAPR;
        break;
      }//switch(registerFractNumber)
      break;
////MTZ-MTZ-MTZ-MTZ-MTZ-MTZ-MTZ-MTZ-MTZ-MTZ
////MTZ04+MTZ04+MTZ04+MTZ04+MTZ04+MTZ04+MTZ04+MTZ04+MTZ04+MTZ04
    case (FILENUMBER(FILEPROT_MTZ04REGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = MTZ04_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        if(validData)if(!(edition_settings.control_extra_settings_1&(1<<INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04))) isValid = 0;
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPROT_MTZ04REGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_mtz04;
      if(isValidCONFCondition(MTZ04_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = N_BIT_CTRMTZ04_1;
        break;
      }//switch(registerFractNumber)
      break;

    case (FILENUMBER(FILEPROT_MTZ04REGISTER2021)+RECORDNUMBER(101)):
      (*editControl) = &edition_settings.control_mtz04;
      if(isValidCONFCondition(MTZ04_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = N_BIT_CTRMTZ04_2;
        break;
      case 0x10000:
        (*uprMaska)   = N_BIT_CTRMTZ04_2_PRYSKORENNJA;
        break;
      case 0x20000:
        (*uprMaska)   = N_BIT_CTRMTZ04_2_PRYSKORENA;
        break;
      }//switch(registerFractNumber)
      break;
////MTZ04-MTZ04-MTZ04-MTZ04-MTZ04-MTZ04-MTZ04-MTZ04-MTZ04-MTZ04
////ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+
    case (FILENUMBER(FILEPROT_ZDZREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = ZDZ_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPROT_ZDZREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_zdz;
      if(isValidCONFCondition(ZDZ_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = CTR_ZDZ_STATE_BIT;
        break;
      case 0x10000:
        (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_MTZ1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));
        if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
        break;
      case 0x20000:
        (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_MTZ2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));
        if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
        break;
      case 0x30000:
        (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_MTZ3_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));
        if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
        break;
      case 0x40000:
        (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_MTZ4_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));
        if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
        break;
      case 0x50000:
        (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_UMIN1_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));
        if(isValidCONFCondition(UMIN_BIT_CONFIGURATION)) isValid = 0;
        break;
      case 0x60000:
        (*uprMaska)   = (CTR_ZDZ_STARTED_FROM_UMIN2_BIT - (_CTR_ZDZ_PART_III - _CTR_ZDZ_PART_II));
        if(isValidCONFCondition(UMIN_BIT_CONFIGURATION)) isValid = 0;
        break;

#ifdef  MODYFIKACIA_VERSII_OVD_ZDZ
      case 0x70000:
        (*uprMaska)   = CTR_ZDZ_OVD1_STATE_BIT;
        break;
      case 0x80000:
        (*uprMaska)   = CTR_ZDZ_OVD2_STATE_BIT;
        break;
      case 0x90000:
        (*uprMaska)   = CTR_ZDZ_OVD3_STATE_BIT;
        break;
#endif
      }//switch(registerFractNumber)
      break;
////ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-
////NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+
    case (FILENUMBER(FILEPROT_NZZREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = ZZ_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPROT_NZZREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_zz;
      if(isValidCONFCondition(ZZ_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_ML_CTR_ZZ1_3I0_STATE;
        break;
      case 0x10000:
        (*uprMaska)   = INDEX_ML_CTR_ZZ1_SECTOR;
        break;
      case 0x20000:
        (*uprMaska)   = INDEX_ML_CTR_ZZ1_NZZ_STATE;
        if(isValidZZTYPECondition(INDEX_ML_CTR_ZZ1_TYPE_BIT)) isValid = 0;
        break;
      case 0x30000:
        (*uprMaska)   = INDEX_ML_CTR_ZZ1_3U0_STATE;
        break;
      case 0x40000:
        (*uprMaska)   = INDEX_ML_CTR_ZZ1_TYPE_BIT;
        if(actControl)
        {
          //Функція обновлення змінних при зміні типу реле 33/3I0
          action_after_changing_zz1_type(&edition_settings);
        }//if(actControl)
        break;
      }//switch(registerFractNumber)
      break;
////NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-
////TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = TZNP_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(validData)if((edition_settings.control_extra_settings_1&(1<<INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04))) isValid = 0;
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_tznp;
      if(isValidCONFCondition(TZNP_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_ML_CTR_TZNP1;
        break;
      case 0x10000:
        (*uprMaska)   = INDEX_ML_CTR_TZNP1_VPERED;
        break;
      case 0x20000:
        (*uprMaska)   = INDEX_ML_CTR_TZNP1_NAZAD;
        break;
      }//switch(registerFractNumber)
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(101)):
      (*editControl) = &edition_settings.control_tznp;
      if(isValidCONFCondition(TZNP_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_ML_CTR_TZNP2;
        break;
      case 0x10000:
        (*uprMaska)   = INDEX_ML_CTR_TZNP2_VPERED;
        break;
      case 0x20000:
        (*uprMaska)   = INDEX_ML_CTR_TZNP2_NAZAD;
        break;
      }//switch(registerFractNumber)
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(201)):
      (*editControl) = &edition_settings.control_tznp;
      if(isValidCONFCondition(TZNP_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_ML_CTR_TZNP3;
        break;
      case 0x10000:
        (*uprMaska)   = INDEX_ML_CTR_TZNP3_VPERED;
        break;
      case 0x20000:
        (*uprMaska)   = INDEX_ML_CTR_TZNP3_NAZAD;
        break;
      }//switch(registerFractNumber)
      break;
////TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-
////ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+
    case (FILENUMBER(FILEPROT_ZOPREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = ZOP_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPROT_ZOPREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_zop;
      if(isValidCONFCondition(ZOP_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = CTR_ZOP_STATE_BIT;
        break;
      }//switch(registerFractNumber)
      break;
////ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-
////ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = UMIN_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_Umin;
      if(isValidCONFCondition(UMIN_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_CTR_UMIN1;
        break;
      case 0x10000:
        (*uprMaska)   = INDEX_CTR_PO_UMIN1_OR_AND;
        break;
      case 0x20000:
        (*uprMaska)   = INDEX_CTR_UMIN1_UBLK;
        break;
      case 0x30000:
        (*uprMaska)   = INDEX_CTR_UMIN1_IBLK;
        break;
      }//switch(registerFractNumber)
      break;
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(101)):
      (*editControl) = &edition_settings.control_Umin;
      if(isValidCONFCondition(UMIN_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_CTR_UMIN2;
        break;
      case 0x10000:
        (*uprMaska)   = INDEX_CTR_PO_UMIN2_OR_AND;
        break;
      case 0x20000:
        (*uprMaska)   = INDEX_CTR_UMIN2_UBLK;
        break;
      case 0x30000:
        (*uprMaska)   = INDEX_CTR_UMIN2_IBLK;
        break;
      }//switch(registerFractNumber)
      break;
////ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-
////ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = UMAX_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_Umax;
      if(isValidCONFCondition(UMAX_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_CTR_UMAX1;
        break;
      case 0x10000:
        (*uprMaska)   = INDEX_CTR_PO_UMAX1_OR_AND;
        break;
      }//switch(registerFractNumber)
      break;
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(101)):
      (*editControl) = &edition_settings.control_Umax;
      if(isValidCONFCondition(UMAX_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_CTR_UMAX2;
        break;
      case 0x10000:
        (*uprMaska)   = INDEX_CTR_PO_UMAX2_OR_AND;
        break;
      }//switch(registerFractNumber)
      break;
////ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-
////APV+APV+APV+APV+APV+APV+APV+APV+APV+APV+APV+APV+APV+APV+APV+APV+APV+
    case (FILENUMBER(FILEPROT_APVREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = APV_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPROT_APVREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_apv;
      if(isValidCONFCondition(APV_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_ML_CTRAPV_STAGE_1;
        break;
      case 0x10000:
        (*uprMaska)   = INDEX_ML_CTRAPV_STAGE_2;
        break;
      case 0x20000:
        (*uprMaska)   = INDEX_ML_CTRAPV_STAGE_3;
        break;
      case 0x30000:
        (*uprMaska)   = INDEX_ML_CTRAPV_STAGE_4;
        break;
//breakOrder
      case 0x60000:
        (*uprMaska)   = INDEX_ML_CTRAPV_BLK_CTRL_VV;
        break;
      case 0x70000:
        (*uprMaska)   = INDEX_ML_CTRAPV_BLK_CTRL_PRVV2;
        if(isValidCONFCondition(UROV_BIT_CONFIGURATION)) isValid = 0;
        break;
      case 0x80000:
        (*uprMaska)   = INDEX_ML_CTRAPV_STARTED_FROM_MTZ1;
        if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
        break;
      case 0x90000:
        (*uprMaska)   = INDEX_ML_CTRAPV_STARTED_FROM_MTZ2;
        if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
        break;
      case 0xA0000:
        (*uprMaska)   = INDEX_ML_CTRAPV_STARTED_FROM_MTZ3;
        if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
        break;
      case 0xB0000:
        (*uprMaska)   = INDEX_ML_CTRAPV_STARTED_FROM_MTZ4;
        if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
        break;
      }//switch(registerFractNumber)
      break;
////APV-APV-APV-APV-APV-APV-APV-APV-APV-APV-APV-APV-APV-APV-APV-
////UROV+UROV+UROV+UROV+UROV+UROV+UROV+UROV+UROV+UROV+UROV+UROV+
    case (FILENUMBER(FILEPROT_UROVREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = UROV_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPROT_UROVREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_urov;
      if(isValidCONFCondition(UROV_BIT_CONFIGURATION)) isValid = 0;

      if(registerWholeNumber == 1 )//ціла частина номер регістру
        switch(registerFractNumber)//дробова частина номер регістру
        {
        case 0x00000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STATE;
          break;
        case 0x10000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ1;
          if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
          break;
        case 0x20000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ2;
          if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
          break;
        case 0x30000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ3;
          if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
          break;
        case 0x40000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ4;
          if(isValidCONFCondition(MTZ_BIT_CONFIGURATION)) isValid = 0;
          break;
        case 0x50000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_1;
          if(isValidCONFCondition(MTZ04_BIT_CONFIGURATION)) isValid = 0;
          break;
        case 0x60000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_MTZ04_2;
          if(isValidCONFCondition(MTZ04_BIT_CONFIGURATION)) isValid = 0;
          break;
        case 0x70000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UMAX1;
          if(isValidCONFCondition(UMAX_BIT_CONFIGURATION)) isValid = 0;
          break;
        case 0x80000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UMAX2;
          if(isValidCONFCondition(UMAX_BIT_CONFIGURATION)) isValid = 0;
          break;
        case 0x90000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UMIN1;
          if(isValidCONFCondition(UMIN_BIT_CONFIGURATION)) isValid = 0;
          break;
        case 0xA0000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UMIN2;
          if(isValidCONFCondition(UMIN_BIT_CONFIGURATION)) isValid = 0;
          break;
        case 0xB0000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_ZOP1;
          if(isValidCONFCondition(ZOP_BIT_CONFIGURATION)) isValid = 0;
          break;
//breakOrder
        case 0xD0000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_NZZ;
          if(isValidCONFCondition(ZZ_BIT_CONFIGURATION)) isValid = 0;
          if(isValidZZTYPECondition(INDEX_ML_CTR_ZZ1_TYPE_BIT)) isValid = 0;
          break;
//breakOrder
        case 0xF0000:
          (*uprMaska)   =  INDEX_ML_CTRUROV_STARTED_FROM_3I0;
          if(isValidCONFCondition(ZZ_BIT_CONFIGURATION)) isValid = 0;
          break;
        }//switch(registerFractNumber)

      else if(registerWholeNumber == 2 )//ціла частина номер регістру
        switch(registerFractNumber)//дробова частина номер регістру
        {
//breakOrder
        case 0x10000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_3U0;
          if(isValidCONFCondition(ZZ_BIT_CONFIGURATION)) isValid = 0;
          break;
//breakOrder
        case 0x40000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_TZNP1;
          if(isValidCONFCondition(TZNP_BIT_CONFIGURATION)) isValid = 0;
          break;
        case 0x50000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_TZNP2;
          if(isValidCONFCondition(TZNP_BIT_CONFIGURATION)) isValid = 0;
          break;
        case 0x60000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_TZNP3;
          if(isValidCONFCondition(TZNP_BIT_CONFIGURATION)) isValid = 0;
          break;
//breakOrder
        case 0x80000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_ZDZ;
          if(isValidCONFCondition(ZDZ_BIT_CONFIGURATION)) isValid = 0;
          break;
        case 0x90000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_ACHR1;
          if(isValidCONFCondition(ACHR_CHAPV_BIT_CONFIGURATION)) isValid = 0;
          break;
        case 0xA0000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_ACHR2;
          if(isValidCONFCondition(ACHR_CHAPV_BIT_CONFIGURATION)) isValid = 0;
          break;
//breakOrder
        case 0xC0000:
        case 0xD0000:
        case 0xE0000:
        case 0xF0000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UP1+((registerFractNumber-0xC0000)>>16);
          if(isValidCONFCondition(UP_BIT_CONFIGURATION)) isValid = 0;
          break;
        }//switch(registerFractNumber)

      else if(registerWholeNumber == 3 )//ціла частина номер регістру
        switch(registerFractNumber)//дробова частина номер регістру
        {
        case 0x00000:
        case 0x10000:
        case 0x20000:
        case 0x30000:
          (*uprMaska)   = INDEX_ML_CTRUROV_STARTED_FROM_UP1+4+((registerFractNumber-0)>>16);
          if(isValidCONFCondition(UP_BIT_CONFIGURATION)) isValid = 0;
          break;
        }//switch(registerFractNumber)
      break;
////UROV-UROV-UROV-UROV-UROV-UROV-UROV-UROV-UROV-UROV-UROV-UROV-UROV-
////AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+
    case (FILENUMBER(FILEPROT_AJRREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = ACHR_CHAPV_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPROT_AJRREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_achr_chapv;
      if(isValidCONFCondition(ACHR_CHAPV_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_CTR_ACHR1;
        break;
      case 0x10000:
        (*uprMaska)   = INDEX_CTR_CHAPV1;
        break;
      case 0x20000:
        (*uprMaska)   = INDEX_CTR_CHAPV_VID_DV;
        break;
      }//switch(registerFractNumber)
      break;
    case (FILENUMBER(FILEPROT_AJRREGISTER2021)+RECORDNUMBER(101)):
      if(registerWholeNumber !=1 ) break;//ціла частина номер регістру
      (*editControl) = &edition_settings.control_achr_chapv;
      if(isValidCONFCondition(ACHR_CHAPV_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_CTR_ACHR2;
        break;
      case 0x10000:
        (*uprMaska)   = INDEX_CTR_CHAPV2;
        break;
      }//switch(registerFractNumber)
      break;
////AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-
////UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+
    case (FILENUMBER(FILEPROT_UPREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = UP_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        //Функція обновлення змінних при зміні конфігурації
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
////UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-
////OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+
    case (FILENUMBER(FILEPERIF_OMPREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = VMP_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPERIF_OMPREGISTER2021)+RECORDNUMBER(1)):
      (*editControl) = &edition_settings.control_vmp;
      if(isValidCONFCondition(VMP_BIT_CONFIGURATION)) isValid = 0;
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_CTR_VMP_STATE;
        break;
      }//switch(registerFractNumber)
      break;
////OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-
////PLINE+PLINE+PLINE+PLINE+PLINE+PLINE+PLINE+PLINE+PLINE+PLINE+
    case (FILENUMBER(FILEPERIF_PLINEREGISTER2021)+RECORDNUMBER(1)):
      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_PHASE_LINE;
      (*editControl) = &edition_settings.control_extra_settings_1;
      if(actControl)
      {
        //Функція обновлення змінних
        if(action_after_changing_extra_settings(edition_settings.control_extra_settings_1, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPERIF_PLINEREGISTER2021)+RECORDNUMBER(3)):
      (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_IB_I04;
      (*editControl) = &edition_settings.control_extra_settings_1;
      if(actControl)
      {
        //Функція обновлення змінних
        if(action_after_changing_extra_settings(edition_settings.control_extra_settings_1, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
    case (FILENUMBER(FILEPERIF_PLINEREGISTER2021)+RECORDNUMBER(4)):
      (*uprMaska)   = INDEX_ML_CTR_TRANSFORMATOR_PHASE_LINE;
      (*editControl) = &edition_settings.control_transformator;
      break;
////PLINE-PLINE-PLINE-PLINE-PLINE-PLINE-PLINE-PLINE-PLINE-PLINE-PLINE-PLINE-PLINE-PLINE-
////CTRLVV+CTRLVV+CTRLVV+CTRLVV+CTRLVV+CTRLVV+CTRLVV+CTRLVV+CTRLVV+CTRLVV+CTRLVV+CTRLVV+
    case (FILENUMBER(FILEPERIF_CTRLVVREGISTER2021)+RECORDNUMBER(1)):
      switch(registerFractNumber)//дробова частина номер регістру
      {
      case 0x00000:
        (*uprMaska)   = INDEX_ML_CTRPRYVOD_VV;
        (*editControl) = &edition_settings.control_switch;
        break;
      case 0x50000:
        //Контроль ФК
        (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_FB_ACTIVATION;
        (*editControl) = &edition_settings.control_extra_settings_1;
        break;
      case 0x60000:
        //Ресурс ВВ
        (*uprMaska)   = INDEX_ML_CTRRESURS_VV;
        (*editControl) = &edition_settings.control_switch;
        break;
      case 0x70000:
        //Разрешение блокировки готовности к ТУ от защит
        (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_READY_TU;
        (*editControl) = &edition_settings.control_extra_settings_1;
        break;
      case 0x80000:
        //Окно информации об отключении ВВ
        (*uprMaska)   = INDEX_ML_CTREXTRA_SETTINGS_1_CTRL_WINDOW_OFF_CB;
        (*editControl) = &edition_settings.control_extra_settings_1;
        break;
      }//switch(registerFractNumber)
      break;
////CTRLVV-CTRLVV-CTRLVV-CTRLVV-CTRLVV-CTRLVV-CTRLVV-CTRLVV-CTRLVV-CTRLVV-CTRLVV-
////ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+ELAND+
    case (FILENUMBER(FILEPERIF_ELANDREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = EL_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
////ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-ELAND-
////REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+
    case (FILENUMBER(FILEPERIF_REGANLREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = INDEX_ML_CTR_AR_AVAR_STATE;
      (*editControl) = &edition_settings.control_ar;
      break;
////REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-
////BLOCKDS+BLOCKDS+BLOCKDS+BLOCKDS+BLOCKDS+BLOCKDS+BLOCKDS+BLOCKDS+BLOCKDS+BLOCKDS+
#ifdef  MODYFIKACIA_VERSII_DS
    case (FILENUMBER(FILEPERIF_BLOCKDSREGISTER2021)+RECORDNUMBER(0)):
      (*uprMaska)   = DS_BIT_CONFIGURATION;
      (*editControl) = &edition_settings.configuration;
      if(actControl)
      {
        if(action_after_changing_of_configuration(edition_settings.configuration, &edition_settings)) isValid = 0;
      }//if(actControl)
      break;
#endif
////BLOCKDS-BLOCKDS-BLOCKDS-BLOCKDS-BLOCKDS-BLOCKDS-BLOCKDS-BLOCKDS-BLOCKDS-
    }//switch(fileRecordNumber)

  return isValid;
}//uprFunc000for2021
