
#include "header.h"

/**************************************/
//База данных уставок
/**************************************/

int privateArmCosF32(float temp_value_f);
int privateArmSinF32(float temp_value_f);

int postUstRegister000for21(int fileRecordNumber, int registerNumber, unsigned int value)
{
  extern int upravlSetting;//флаг Setting
  int  multer = 1;
  uint32_t *editValue = NULL;
  ustFunc000for2021(fileRecordNumber, registerNumber, &multer, 0, &editValue);
  if(editValue == NULL) return 1;

  (*editValue) = value*multer;

  ustFunc000for2021(-fileRecordNumber, registerNumber, &multer, 0, &editValue);//actControl=1

  upravlSetting = 1;//флаг Setting
  return 0;
}//postUstRegister21

int getUstRegister000for20(int fileRecordNumber, int registerNumber)
{
  int  multer = 1;
  uint32_t *editValue = NULL;
  ustFunc000for2021(fileRecordNumber, registerNumber, &multer, 0, &editValue);
  if(editValue == NULL) return 0;

  return ((*editValue)/multer) &0xFFFF;
}//getUstRegister20
int setUstRegister000for21(int fileRecordNumber, int registerNumber, unsigned int value)
{
  int  multer = 1;
  uint32_t *editValue = NULL;
  if(!ustFunc000for2021(fileRecordNumber, registerNumber, &multer, value, &editValue)) return MARKER_ERRORDIAPAZON;
  return 0;
}//setUstRegister21

int privateArmCosF32(float temp_value_f)
{
  return (int) (AMPLITUDA_FI*/*cos*/arm_cos_f32(/*(double)*/(PI*temp_value_f/180.0f)));
}//privateArmCosF32
int privateArmSinF32(float temp_value_f)
{
  return (int) (AMPLITUDA_FI*/*cos*/arm_sin_f32(/*(double)*/(PI*temp_value_f/180.0f)));
}//privateArmCosF32

int ustFunc000for2021(int fileRecordNumber, int registerNumber, int *multer, int regUst, uint32_t **editValue)
{
  int diapazon = 1;
  int actControl = 0;
  if(fileRecordNumber<0)
  {
    fileRecordNumber = -fileRecordNumber;
    actControl = 1;
  }//if(fileRecordNumber<0)
  (*multer) = 10;

  int fileNumber       = fileRecordNumber&0xFFFF0000;//номер файла
  int recordNumber     = fileRecordNumber&0x0000FFFF;//номер запису

  if(registerNumber == 0) return diapazon;
  int grupa_ustavok = registerNumber - 1;

  switch(fileNumber)
  {
  case (FILENUMBER(FILEUVV_DVREGISTER2021)):
  {
    int item = recordNumber-1;
    (*editValue) = (uint32_t*)&edition_settings.dopusk_dv[item];
    (*multer) = 1;
    if(regUst<KOEF_DOPUSK_DV_POST_MIN || regUst>KOEF_DOPUSK_DV_MAX) diapazon=0;
    if(!((edition_settings.type_of_input_signal)&(1<<item)))
    {
      //только для постоянки
    }//if
    else
    {
      //только для переменки
      if(regUst<KOEF_DOPUSK_DV_ZMIN_MIN) diapazon=0;//не меньше 20 мс
      if((regUst%10)!=0) diapazon=0;//всегда четные
    }
  }//case (FILENUMBER(FILEUVV_DVREGISTER2021))
  break;

////MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+MFT+
  case (FILENUMBER(FILEPERIF_ELMFTREGISTER2021)):
  {
    int item = (recordNumber/4);
    switch(registerNumber)
    {
    case 2:
      (*editValue) = (uint32_t*)&edition_settings.timeout_pause_df[item];
      if(regUst<TIMEOUT_DF_PAUSE_MIN/10 || regUst>TIMEOUT_DF_PAUSE_MAX/10) diapazon=0;
      break;
    case 3:
      (*editValue) = (uint32_t*)&edition_settings.timeout_work_df[item];
      if(regUst<TIMEOUT_DF_WORK_MIN/10 || regUst>TIMEOUT_DF_WORK_MAX/10) diapazon=0;
      break;
    }//switch
  }//FILENUMBER(FILEPERIF_MFTREGISTER2021
  break;
////MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-MFT-
////OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+
  case (FILENUMBER(FILEPERIF_OMPREGISTER2021)):
  {
    int direct = -1;
    int item = -1;
    if((recordNumber >= 6) && (recordNumber <= 21)) {
      direct = 0;
      item = (recordNumber-6)/2;
    }
    if((recordNumber >= 23) && (recordNumber <= 38)) {
      direct = 1;
      item = (recordNumber-23)/2;
    }
    if(direct >= 0)
      switch(recordNumber&1)
      {
      case 0://6 8 10 12 14 16 18 20 //24 26 28 30 32 34 36 38
        (*editValue) = (uint32_t*)&edition_settings.dovgyna[direct][item];
        if(regUst<SETPOINT_DOVGYNA_VMP_MIN || regUst>SETPOINT_DOVGYNA_VMP_MAX) diapazon=0;
        break;
      case 1://7 9 11 13 15 17 19 21 //23 25 27 29 31 33 35 37
        (*editValue) = (uint32_t*)&edition_settings.opir[direct][item];
        if(regUst<SETPOINT_OPIR_VMP_MIN || regUst>SETPOINT_OPIR_VMP_MAX) diapazon=0;
      }//switch(recordNumber&1)
  }//FILENUMBER(FILEPERIF_OMPREGISTER2021
  break;
////OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-

////UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+UP+
  case (FILENUMBER(FILEPROT_UPREGISTER2021)):
  {
    int item = recordNumber/100;
    switch(recordNumber%100)
    {
    case 2://2 102 202 302 402 502 602 702
      (*editValue) = (uint32_t*)&edition_settings.ctrl_UP_input[item];
      (*multer) = 1;
      if(regUst<0 || regUst>19) diapazon=0;
      if(actControl)
        if(regUst != current_settings_interfaces.ctrl_UP_input[item])
        {
          uint32_t tmp = edition_settings.ctrl_UP_input[item];
          edition_settings.ctrl_UP_input[item] = -1;
          action_after_changing_input_UP(&edition_settings, item, tmp);
        }//if(actControl)
      break;
    case 6://6 106 206 306 406 506 606 706
      (*editValue) = (uint32_t*)&edition_settings.timeout_UP[item][0][grupa_ustavok/2];
      if(regUst<TIMEOUT_UP_MIN/10 || regUst>TIMEOUT_UP_MAX/10) diapazon=0;
      break;
    case 7://7 107 207 307 407 507 607 707
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_UP_KP[item][0][grupa_ustavok/2];

      if(edition_settings.control_UP& (1<<(2+3*item)))//more_less_control
      {
        if(regUst<SETPOINT_UP_KP_LESS_MIN || regUst>SETPOINT_UP_KP_LESS_MAX) diapazon=0;
      }//if(edition_settings.control_UP& (1<<(2+3*item)))//more_less_control
      else  if(regUst<SETPOINT_UP_KP_MORE_MIN || regUst>SETPOINT_UP_KP_MORE_MAX) diapazon=0;
      break;
    }//switch(recordNumber%100)
  }//FILENUMBER(FILEPROT_UPREGISTER2021
  break;
////UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-UP-

  }//switch

  if((*editValue) == NULL)
    switch(fileRecordNumber)
    {
    case (FILENUMBER(FILEPROT_COMMREGISTER2021)+RECORDNUMBER(1)):
      (*editValue) = (uint32_t*)&edition_settings.grupa_ustavok;
      (*multer) = 1;
      if(regUst<1 || regUst>4) diapazon=0;
      break;

////MTZ+MTZ+MTZ+MTZ+MTZ+MTZ+MTZ+MTZ+MTZ+MTZ
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(2)):
      (*editValue) = (uint32_t*)&edition_settings.type_mtz1;
      (*multer) = 1;
      if(regUst<0 || regUst>2) diapazon=0;
      break;

    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(5)):
      (*editValue) = &edition_settings.setpoint_mtz_1[grupa_ustavok];
      if(regUst<SETPOINT_MTZ1_MIN/10 || regUst>SETPOINT_MTZ1_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(6)):
      (*editValue) = &edition_settings.setpoint_mtz_1_n_vpered[grupa_ustavok];
      if(regUst<SETPOINT_MTZ1_N_VPERED_MIN/10 || regUst>SETPOINT_MTZ1_N_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(7)):
      (*editValue) = &edition_settings.setpoint_mtz_1_n_nazad[grupa_ustavok];
      if(regUst<SETPOINT_MTZ1_N_NAZAD_MIN/10 || regUst>SETPOINT_MTZ1_N_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(8)):
      (*editValue) = &edition_settings.setpoint_mtz_1_po_napruzi[grupa_ustavok];
      if(regUst<SETPOINT_MTZ1_PO_NAPRUZI_MIN/10 || regUst>SETPOINT_MTZ1_PO_NAPRUZI_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(9)):
      (*editValue) = &edition_settings.setpoint_mtz_1_U[grupa_ustavok];
      if(regUst<SETPOINT_MTZ1_U_MIN/10 || regUst>SETPOINT_MTZ1_U_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(10)):
      (*multer) = 1;
      (*editValue) = &edition_settings.setpoint_mtz_1_angle[grupa_ustavok];
      if(regUst<SETPOINT_MTZ1_ANGLE_MIN || regUst>SETPOINT_MTZ1_ANGLE_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(11)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_1[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ1_MIN/10 || regUst>TIMEOUT_MTZ1_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(12)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_1_n_vpered[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ1_N_VPERED_MIN/10 || regUst>TIMEOUT_MTZ1_N_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(13)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_1_n_nazad[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ1_N_NAZAD_MIN/10 || regUst>TIMEOUT_MTZ1_N_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(14)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_1_po_napruzi[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ1_PO_NAPRUZI_MIN/10 || regUst>TIMEOUT_MTZ1_PO_NAPRUZI_MAX/10) diapazon=0;
      break;

    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(102)):
      (*editValue) = (uint32_t*)&edition_settings.type_mtz2;
      (*multer) = 1;
      if(regUst<0 || regUst>7) diapazon=0;
      break;

    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(105)):
      (*editValue) = &edition_settings.setpoint_mtz_2[grupa_ustavok];
      if(regUst<SETPOINT_MTZ2_MIN/10 || regUst>SETPOINT_MTZ2_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(106)):
      (*editValue) = &edition_settings.setpoint_mtz_2_n_vpered[grupa_ustavok];
      if(regUst<SETPOINT_MTZ2_N_VPERED_MIN/10 || regUst>SETPOINT_MTZ2_N_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(107)):
      (*editValue) = &edition_settings.setpoint_mtz_2_n_nazad[grupa_ustavok];
      if(regUst<SETPOINT_MTZ2_N_NAZAD_MIN/10 || regUst>SETPOINT_MTZ2_N_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(108)):
      (*editValue) = &edition_settings.setpoint_mtz_2_po_napruzi[grupa_ustavok];
      if(regUst<SETPOINT_MTZ2_PO_NAPRUZI_MIN/10 || regUst>SETPOINT_MTZ2_PO_NAPRUZI_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(109)):
      (*editValue) = &edition_settings.setpoint_mtz_2_U[grupa_ustavok];
      if(regUst<SETPOINT_MTZ2_U_MIN/10 || regUst>SETPOINT_MTZ2_U_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(110)):
      (*multer) = 1;
      (*editValue) = &edition_settings.setpoint_mtz_2_angle[grupa_ustavok];
      if(regUst<SETPOINT_MTZ2_ANGLE_MIN || regUst>SETPOINT_MTZ2_ANGLE_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(111)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ2_MIN/10 || regUst>TIMEOUT_MTZ2_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(112)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_n_vpered[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ2_N_VPERED_MIN/10 || regUst>TIMEOUT_MTZ2_N_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(113)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_n_nazad[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ2_N_NAZAD_MIN/10 || regUst>TIMEOUT_MTZ2_N_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(114)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_po_napruzi[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ2_PO_NAPRUZI_MIN/10 || regUst>TIMEOUT_MTZ2_PO_NAPRUZI_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(115)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_vvid_pr[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ2_VVID_PR_MIN/10 || regUst>TIMEOUT_MTZ2_VVID_PR_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(116)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_pr[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ2_PR_MIN/10 || regUst>TIMEOUT_MTZ2_PR_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(117)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_n_vpered_pr[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ2_N_VPERED_PR_MIN/10 || regUst>TIMEOUT_MTZ2_N_VPERED_PR_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(118)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_n_nazad_pr[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ2_N_NAZAD_PR_MIN/10 || regUst>TIMEOUT_MTZ2_N_NAZAD_PR_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(119)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_2_po_napruzi_pr[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ2_PO_NAPRUZI_PR_MIN/10 || regUst>TIMEOUT_MTZ2_PO_NAPRUZI_PR_MAX/10) diapazon=0;
      break;

    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(202)):
      (*editValue) = (uint32_t*)&edition_settings.type_mtz3;
      (*multer) = 1;
      if(regUst<0 || regUst>7) diapazon=0;
      break;

    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(205)):
      (*editValue) = &edition_settings.setpoint_mtz_3[grupa_ustavok];
      if(regUst<SETPOINT_MTZ3_MIN/10 || regUst>SETPOINT_MTZ3_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(206)):
      (*editValue) = &edition_settings.setpoint_mtz_3_n_vpered[grupa_ustavok];
      if(regUst<SETPOINT_MTZ3_N_VPERED_MIN/10 || regUst>SETPOINT_MTZ3_N_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(207)):
      (*editValue) = &edition_settings.setpoint_mtz_3_n_nazad[grupa_ustavok];
      if(regUst<SETPOINT_MTZ3_N_NAZAD_MIN/10 || regUst>SETPOINT_MTZ3_N_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(208)):
      (*editValue) = &edition_settings.setpoint_mtz_3_po_napruzi[grupa_ustavok];
      if(regUst<SETPOINT_MTZ3_PO_NAPRUZI_MIN/10 || regUst>SETPOINT_MTZ3_PO_NAPRUZI_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(209)):
      (*editValue) = &edition_settings.setpoint_mtz_3_U[grupa_ustavok];
      if(regUst<SETPOINT_MTZ3_U_MIN/10 || regUst>SETPOINT_MTZ3_U_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(210)):
      (*multer) = 1;
      (*editValue) = &edition_settings.setpoint_mtz_3_angle[grupa_ustavok];
      if(regUst<SETPOINT_MTZ3_ANGLE_MIN || regUst>SETPOINT_MTZ3_ANGLE_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(211)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_3[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ3_MIN/10 || regUst>TIMEOUT_MTZ3_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(212)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_3_n_vpered[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ3_N_VPERED_MIN/10 || regUst>TIMEOUT_MTZ3_N_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(213)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_3_n_nazad[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ3_N_NAZAD_MIN/10 || regUst>TIMEOUT_MTZ3_N_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(214)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_3_po_napruzi[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ3_PO_NAPRUZI_MIN/10 || regUst>TIMEOUT_MTZ3_PO_NAPRUZI_MAX/10) diapazon=0;
      break;

    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(302)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.type_mtz4;
      if(regUst<0 || regUst>2) diapazon=0;
      break;

    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(305)):
      (*editValue) = &edition_settings.setpoint_mtz_4[grupa_ustavok];
      if(regUst<SETPOINT_MTZ4_MIN/10 || regUst>SETPOINT_MTZ4_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(306)):
      (*editValue) = &edition_settings.setpoint_mtz_4_n_vpered[grupa_ustavok];
      if(regUst<SETPOINT_MTZ4_N_VPERED_MIN/10 || regUst>SETPOINT_MTZ4_N_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(307)):
      (*editValue) = &edition_settings.setpoint_mtz_4_n_nazad[grupa_ustavok];
      if(regUst<SETPOINT_MTZ4_N_NAZAD_MIN/10 || regUst>SETPOINT_MTZ4_N_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(308)):
      (*editValue) = &edition_settings.setpoint_mtz_4_po_napruzi[grupa_ustavok];
      if(regUst<SETPOINT_MTZ4_PO_NAPRUZI_MIN/10 || regUst>SETPOINT_MTZ4_PO_NAPRUZI_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(309)):
      (*editValue) = &edition_settings.setpoint_mtz_4_U[grupa_ustavok];
      if(regUst<SETPOINT_MTZ4_U_MIN/10 || regUst>SETPOINT_MTZ4_U_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(310)):
      (*multer) = 1;
      (*editValue) = &edition_settings.setpoint_mtz_4_angle[grupa_ustavok];
      if(regUst<SETPOINT_MTZ4_ANGLE_MIN || regUst>SETPOINT_MTZ4_ANGLE_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(311)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_4[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ4_MIN/10 || regUst>TIMEOUT_MTZ4_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(312)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_4_n_vpered[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ4_N_VPERED_MIN/10 || regUst>TIMEOUT_MTZ4_N_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(313)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_4_n_nazad[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ4_N_NAZAD_MIN/10 || regUst>TIMEOUT_MTZ4_N_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZREGISTER2021)+RECORDNUMBER(314)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz_4_po_napruzi[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ4_PO_NAPRUZI_MIN/10 || regUst>TIMEOUT_MTZ4_PO_NAPRUZI_MAX/10) diapazon=0;
      break;
////MTZ-MTZ-MTZ-MTZ-MTZ-MTZ-MTZ-MTZ-MTZ-MTZ
////MTZ04+MTZ04+MTZ04+MTZ04+MTZ04+MTZ04+MTZ04+MTZ04+MTZ04+MTZ04
    case (FILENUMBER(FILEPROT_MTZ04REGISTER2021)+RECORDNUMBER(5)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_mtz04_1[grupa_ustavok];
      if(regUst<SETPOINT_MTZ04_1_MIN/10 || regUst>SETPOINT_MTZ04_1_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZ04REGISTER2021)+RECORDNUMBER(6)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz04_1[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ04_1_MIN/10 || regUst>TIMEOUT_MTZ04_1_MAX/10) diapazon=0;
      break;

    case (FILENUMBER(FILEPROT_MTZ04REGISTER2021)+RECORDNUMBER(105)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_mtz04_2[grupa_ustavok];
      if(regUst<SETPOINT_MTZ04_2_MIN/10 || regUst>SETPOINT_MTZ04_2_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZ04REGISTER2021)+RECORDNUMBER(106)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz04_2[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ04_2_MIN/10 || regUst>TIMEOUT_MTZ04_2_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZ04REGISTER2021)+RECORDNUMBER(107)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz04_2_vvid_pr[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ04_2_VVID_PR_MIN/10 || regUst>TIMEOUT_MTZ04_2_VVID_PR_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_MTZ04REGISTER2021)+RECORDNUMBER(108)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_mtz04_2_pr[grupa_ustavok];
      if(regUst<TIMEOUT_MTZ04_2_PR_MIN/10 || regUst>TIMEOUT_MTZ04_2_PR_MAX/10) diapazon=0;
      break;
////MTZ04-MTZ04-MTZ04-MTZ04-MTZ04-MTZ04-MTZ04-MTZ04-MTZ04-MTZ04
////ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+ZDZ+
    case (FILENUMBER(FILEPROT_ZDZREGISTER2021)+RECORDNUMBER(5)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.ctrl_zdz_type;
      if(regUst<0 || regUst>5) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZDZREGISTER2021)+RECORDNUMBER(6)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.timeout_zdz[grupa_ustavok];
      if(regUst<TIMEOUT_ZDZ_MIN || regUst>TIMEOUT_ZDZ_MAX) diapazon=0;
      break;
////ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-ZDZ-
////NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+NZZ+
    case (FILENUMBER(FILEPROT_NZZREGISTER2021)+RECORDNUMBER(5)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_zz_3I0[grupa_ustavok];
      if(regUst<SETPOINT_ZZ_3I0_MIN || regUst>SETPOINT_ZZ_3I0_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_NZZREGISTER2021)+RECORDNUMBER(6)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_zz_3I0[grupa_ustavok];
      if(regUst<TIMEOUT_ZZ_3I0_MIN/10 || regUst>TIMEOUT_ZZ_3I0_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_NZZREGISTER2021)+RECORDNUMBER(7)):
      (*multer) = 1;
      (*editValue) = &edition_settings.setpoint_zz_3U0[grupa_ustavok];
      if(regUst<SETPOINT_ZZ_3U0_MIN/10 || regUst>SETPOINT_ZZ_3U0_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_NZZREGISTER2021)+RECORDNUMBER(8)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_zz_3U0[grupa_ustavok];
      if(regUst<TIMEOUT_ZZ_3U0_MIN/10 || regUst>TIMEOUT_ZZ_3U0_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_NZZREGISTER2021)+RECORDNUMBER(10)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_nzz[grupa_ustavok];
      if(regUst<TIMEOUT_NZZ_MIN/10 || regUst>TIMEOUT_NZZ_MAX/10) diapazon=0;
      break;
////NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-NZZ-
////TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+TZNP+
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(5)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_1_3I0_vpered[grupa_ustavok];
      if(regUst<SETPOINT_TZNP1_3I0_VPERED_MIN/10 || regUst>SETPOINT_TZNP1_3I0_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(6)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_1_3U0_vpered[grupa_ustavok];
      if(regUst<SETPOINT_TZNP1_3U0_VPERED_MIN/10 || regUst>SETPOINT_TZNP1_3U0_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(7)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_tznp_1_vpered[grupa_ustavok];
      if(regUst<TIMEOUT_TZNP1_VPERED_MIN/10 || regUst>TIMEOUT_TZNP1_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(8)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_1_3I0_nazad[grupa_ustavok];
      if(regUst<SETPOINT_TZNP1_3I0_NAZAD_MIN/10 || regUst>SETPOINT_TZNP1_3I0_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(9)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_1_3U0_nazad[grupa_ustavok];
      if(regUst<SETPOINT_TZNP1_3U0_NAZAD_MIN/10 || regUst>SETPOINT_TZNP1_3U0_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(10)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_tznp_1_nazad[grupa_ustavok];
      if(regUst<TIMEOUT_TZNP1_NAZAD_MIN/10 || regUst>TIMEOUT_TZNP1_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(11)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_1_angle[grupa_ustavok];
      if(regUst<SETPOINT_TZNP1_ANGLE_MIN || regUst>SETPOINT_TZNP1_ANGLE_MAX) diapazon=0;
      break;

    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(105)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_2_3I0_vpered[grupa_ustavok];
      if(regUst<SETPOINT_TZNP2_3I0_VPERED_MIN/10 || regUst>SETPOINT_TZNP2_3I0_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(106)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_2_3U0_vpered[grupa_ustavok];
      if(regUst<SETPOINT_TZNP2_3U0_VPERED_MIN/10 || regUst>SETPOINT_TZNP2_3U0_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(107)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_tznp_2_vpered[grupa_ustavok];
      if(regUst<TIMEOUT_TZNP2_VPERED_MIN/10 || regUst>TIMEOUT_TZNP2_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(108)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_2_3I0_nazad[grupa_ustavok];
      if(regUst<SETPOINT_TZNP2_3I0_NAZAD_MIN/10 || regUst>SETPOINT_TZNP2_3I0_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(109)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_2_3U0_nazad[grupa_ustavok];
      if(regUst<SETPOINT_TZNP2_3U0_NAZAD_MIN/10 || regUst>SETPOINT_TZNP2_3U0_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(101)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_tznp_2_nazad[grupa_ustavok];
      if(regUst<TIMEOUT_TZNP2_NAZAD_MIN/10 || regUst>TIMEOUT_TZNP2_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(111)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_2_angle[grupa_ustavok];
      if(regUst<SETPOINT_TZNP2_ANGLE_MIN || regUst>SETPOINT_TZNP2_ANGLE_MAX) diapazon=0;
      break;

    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(205)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_3_3I0_vpered[grupa_ustavok];
      if(regUst<SETPOINT_TZNP3_3I0_VPERED_MIN/10 || regUst>SETPOINT_TZNP3_3I0_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(206)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_3_3U0_vpered[grupa_ustavok];
      if(regUst<SETPOINT_TZNP3_3U0_VPERED_MIN/10 || regUst>SETPOINT_TZNP3_3U0_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(207)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_tznp_3_vpered[grupa_ustavok];
      if(regUst<TIMEOUT_TZNP3_VPERED_MIN/10 || regUst>TIMEOUT_TZNP3_VPERED_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(208)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_3_3I0_nazad[grupa_ustavok];
      if(regUst<SETPOINT_TZNP3_3I0_NAZAD_MIN/10 || regUst>SETPOINT_TZNP3_3I0_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(209)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_3_3U0_nazad[grupa_ustavok];
      if(regUst<SETPOINT_TZNP3_3U0_NAZAD_MIN/10 || regUst>SETPOINT_TZNP3_3U0_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(201)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_tznp_3_nazad[grupa_ustavok];
      if(regUst<TIMEOUT_TZNP3_NAZAD_MIN/10 || regUst>TIMEOUT_TZNP3_NAZAD_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_TZNPREGISTER2021)+RECORDNUMBER(211)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_tznp_3_angle[grupa_ustavok];
      if(regUst<SETPOINT_TZNP3_ANGLE_MIN || regUst>SETPOINT_TZNP3_ANGLE_MAX) diapazon=0;
      break;
////TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-TZNP-
////ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+ZOP+
    case (FILENUMBER(FILEPROT_ZOPREGISTER2021)+RECORDNUMBER(5)):
      (*multer) = 1;
      (*editValue) = &edition_settings.setpoint_zop[grupa_ustavok];
      if(regUst<SETPOINT_ZOP_MIN || regUst>SETPOINT_ZOP_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZOPREGISTER2021)+RECORDNUMBER(6)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_zop[grupa_ustavok];
      if(regUst<TIMEOUT_ZOP_MIN/10 || regUst>TIMEOUT_ZOP_MAX/10) diapazon=0;
      break;
////ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-ZOP-
////ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+ZNMIN+
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(5)):
      (*multer) = 100;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_Umin1[grupa_ustavok];
      if(regUst<SETPOINT_UMIN1_MIN/100 || regUst>SETPOINT_UMIN1_MAX/100) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(6)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_Umin1[grupa_ustavok];
      if(regUst<TIMEOUT_UMIN1_MIN/10 || regUst>TIMEOUT_UMIN1_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(7)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_Umin1_Iblk[grupa_ustavok];
      if(regUst<SETPOINT_UMIN1_IBLK_MIN/10 || regUst>SETPOINT_UMIN1_IBLK_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(105)):
      (*multer) = 100;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_Umin2[grupa_ustavok];
      if(regUst<SETPOINT_UMIN2_MIN/100 || regUst>SETPOINT_UMIN2_MAX/100) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(106)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_Umin2[grupa_ustavok];
      if(regUst<TIMEOUT_UMIN2_MIN/10 || regUst>TIMEOUT_UMIN2_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZNMINREGISTER2021)+RECORDNUMBER(107)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_Umin2_Iblk[grupa_ustavok];
      if(regUst<SETPOINT_UMIN2_IBLK_MIN/10 || regUst>SETPOINT_UMIN2_IBLK_MAX/10) diapazon=0;
      break;
////ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-ZNMIN-
////ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+ZNMAX+
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(5)):
      (*multer) = 100;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_Umax1[grupa_ustavok];
      if(regUst<SETPOINT_UMAX1_MIN/100 || regUst>SETPOINT_UMAX1_MAX/100) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(6)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_Umax1[grupa_ustavok];
      if(regUst<TIMEOUT_UMAX1_MIN/10 || regUst>TIMEOUT_UMAX1_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(7)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_kp_Umax[grupa_ustavok];
      if(regUst<SETPOINT_KP_UMAX_MIN || regUst>SETPOINT_KP_UMAX_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(105)):
      (*multer) = 100;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_Umax2[grupa_ustavok];
      if(regUst<SETPOINT_UMAX2_MIN/100 || regUst>SETPOINT_UMAX2_MAX/100) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_ZNMAXREGISTER2021)+RECORDNUMBER(106)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_Umax2[grupa_ustavok];
      if(regUst<TIMEOUT_UMAX2_MIN/10 || regUst>TIMEOUT_UMAX2_MAX/10) diapazon=0;
      break;
////ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-ZNMAX-
////APV+APV+APV+APV+APV+APV+APV+APV+APV+APV+APV+APV+APV+APV+APV+
    case (FILENUMBER(FILEPROT_APVREGISTER2021)+RECORDNUMBER(5)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_apv_block_vid_VV[grupa_ustavok];
      if(regUst<TIMEOUT_APV_BLOCK_VID_VV_MIN/10 || regUst>TIMEOUT_APV_BLOCK_VID_VV_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_APVREGISTER2021)+RECORDNUMBER(6)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_apv_1[grupa_ustavok];
      if(regUst<TIMEOUT_APV1_MIN/10 || regUst>TIMEOUT_APV1_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_APVREGISTER2021)+RECORDNUMBER(7)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_apv_2[grupa_ustavok];
      if(regUst<TIMEOUT_APV2_MIN/10 || regUst>TIMEOUT_APV2_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_APVREGISTER2021)+RECORDNUMBER(8)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_apv_3[grupa_ustavok];
      if(regUst<TIMEOUT_APV3_MIN/10 || regUst>TIMEOUT_APV3_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_APVREGISTER2021)+RECORDNUMBER(9)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_apv_4[grupa_ustavok];
      if(regUst<TIMEOUT_APV4_MIN/10 || regUst>TIMEOUT_APV4_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_APVREGISTER2021)+RECORDNUMBER(10)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_apv_block_vid_apv1[grupa_ustavok];
      if(regUst<TIMEOUT_APV_BLOCK_VID_APV1_MIN/10 || regUst>TIMEOUT_APV_BLOCK_VID_APV1_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_APVREGISTER2021)+RECORDNUMBER(11)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_apv_block_vid_apv2[grupa_ustavok];
      if(regUst<TIMEOUT_APV_BLOCK_VID_APV2_MIN/10 || regUst>TIMEOUT_APV_BLOCK_VID_APV2_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_APVREGISTER2021)+RECORDNUMBER(12)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_apv_block_vid_apv3[grupa_ustavok];
      if(regUst<TIMEOUT_APV_BLOCK_VID_APV3_MIN/10 || regUst>TIMEOUT_APV_BLOCK_VID_APV3_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_APVREGISTER2021)+RECORDNUMBER(13)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_apv_block_vid_apv4[grupa_ustavok];
      if(regUst<TIMEOUT_APV_BLOCK_VID_APV4_MIN/10 || regUst>TIMEOUT_APV_BLOCK_VID_APV4_MAX/10) diapazon=0;
      break;
////APV-APV-APV-APV-APV-APV-APV-APV-APV-APV-APV-APV-APV-APV-APV-
////UROV+UROV+UROV+UROV+UROV+UROV+UROV+UROV+UROV+UROV+UROV+UROV+
    case (FILENUMBER(FILEPROT_UROVREGISTER2021)+RECORDNUMBER(5)):
      (*editValue) = &edition_settings.setpoint_urov[grupa_ustavok];
      if(regUst<SETPOINT_UROV_MIN/10 || regUst>SETPOINT_UROV_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_UROVREGISTER2021)+RECORDNUMBER(6)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_urov_1[grupa_ustavok];
      if(regUst<TIMEOUT_UROV1_MIN/10 || regUst>TIMEOUT_UROV1_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_UROVREGISTER2021)+RECORDNUMBER(7)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_urov_2[grupa_ustavok];
      if(regUst<TIMEOUT_UROV2_MIN/10 || regUst>TIMEOUT_UROV2_MAX/10) diapazon=0;
      break;
////UROV-UROV-UROV-UROV-UROV-UROV-UROV-UROV-UROV-UROV-UROV-UROV-
////AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+AJR+
    case (FILENUMBER(FILEPROT_AJRREGISTER2021)+RECORDNUMBER(5)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_achr1_f_rab[grupa_ustavok];
      if(regUst<SETPOINT_ACHR1_F_RAB_MIN/10 || regUst>SETPOINT_ACHR1_F_RAB_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_AJRREGISTER2021)+RECORDNUMBER(6)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_achr_1[grupa_ustavok];
      if(regUst<TIMEOUT_ACHR1_MIN/10 || regUst>TIMEOUT_ACHR1_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_AJRREGISTER2021)+RECORDNUMBER(7)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_chapv1_f_rab[grupa_ustavok];
      if(regUst<SETPOINT_CHAPV1_F_RAB_MIN/10 || regUst>SETPOINT_CHAPV1_F_RAB_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_AJRREGISTER2021)+RECORDNUMBER(8)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_chapv_1[grupa_ustavok];
      if(regUst<TIMEOUT_CHAPV1_MIN/10 || regUst>TIMEOUT_CHAPV1_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_AJRREGISTER2021)+RECORDNUMBER(9)):
      (*multer) = 100;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_achr_chapv_uf[grupa_ustavok];
      if(regUst<SETPOINT_ACHR_CHAPV_UF_MIN/100 || regUst>SETPOINT_ACHR_CHAPV_UF_MAX/100) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_AJRREGISTER2021)+RECORDNUMBER(105)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_achr2_f_rab[grupa_ustavok];
      if(regUst<SETPOINT_ACHR2_F_RAB_MIN/10 || regUst>SETPOINT_ACHR2_F_RAB_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_AJRREGISTER2021)+RECORDNUMBER(106)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_achr_2[grupa_ustavok];
      if(regUst<TIMEOUT_ACHR2_MIN/10 || regUst>TIMEOUT_ACHR2_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_AJRREGISTER2021)+RECORDNUMBER(107)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_chapv2_f_rab[grupa_ustavok];
      if(regUst<SETPOINT_CHAPV2_F_RAB_MIN/10 || regUst>SETPOINT_CHAPV2_F_RAB_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPROT_AJRREGISTER2021)+RECORDNUMBER(108)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_chapv_2[grupa_ustavok];
      if(regUst<TIMEOUT_CHAPV2_MIN/10 || regUst>TIMEOUT_CHAPV2_MAX/10) diapazon=0;
      break;
////AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-AJR-
////OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+OMP+
    case (FILENUMBER(FILEPERIF_OMPREGISTER2021)+RECORDNUMBER(5)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.lines[0];
      if(regUst<NUMBER_LINES_FORWARD_MIN || regUst>NUMBER_LINES_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_OMPREGISTER2021)+RECORDNUMBER(22)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.lines[1];
      if(regUst<NUMBER_LINES_BACKWARD_MIN || regUst>NUMBER_LINES_MAX) diapazon=0;
      break;
////OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-OMP-
////CTRLVV+CTRLVV+CTRLVV+CTRLVV+CTRLVV+CTRLVV+CTRLVV+CTRLVV+CTRLVV+CTRLVV+
    case (FILENUMBER(FILEPERIF_CTRLVVREGISTER2021)+RECORDNUMBER(5)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_swch_udl_blk_on;
      if(regUst<TIMEOUT_SWCH_UDL_BLK_ON_MIN/10 || regUst>TIMEOUT_SWCH_UDL_BLK_ON_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_CTRLVVREGISTER2021)+RECORDNUMBER(6)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_swch_on;
      if(regUst<TIMEOUT_SWCH_ON_MIN/10 || regUst>TIMEOUT_SWCH_ON_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_CTRLVVREGISTER2021)+RECORDNUMBER(7)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_swch_off;
      if(regUst<TIMEOUT_SWCH_OFF_MIN/10 || regUst>TIMEOUT_SWCH_OFF_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_CTRLVVREGISTER2021)+RECORDNUMBER(8)):
      (*editValue) = (uint32_t*)&edition_settings.timeout_pryvoda_VV;
      if(regUst<TIMEOUT_PRYVODA_VV_MIN/10 || regUst>TIMEOUT_PRYVODA_VV_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_CTRLVVREGISTER2021)+RECORDNUMBER(15)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_Inom;
      if(regUst<SETPOINT_Inom_MIN || regUst>SETPOINT_Inom_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_CTRLVVREGISTER2021)+RECORDNUMBER(16)):
      (*editValue) = (uint32_t*)&edition_settings.setpoint_r_kom_st_Inom;
      if(regUst<SETPOINT_RKS_Inom_MIN/10 || regUst>SETPOINT_RKS_Inom_MAX/10) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_CTRLVVREGISTER2021)+RECORDNUMBER(17)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_Inom_vymk;
      if(regUst<SETPOINT_Inom_vymk_MIN || regUst>SETPOINT_Inom_vymk_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_CTRLVVREGISTER2021)+RECORDNUMBER(18)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_r_kom_st_Inom_vymk;
      if(regUst<SETPOINT_RKS_Inom_vymk_MIN || regUst>SETPOINT_RKS_Inom_vymk_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_CTRLVVREGISTER2021)+RECORDNUMBER(19)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_pochatkovyj_resurs;
      break;
    case (FILENUMBER(FILEPERIF_CTRLVVREGISTER2021)+RECORDNUMBER(20)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_krytychnyj_resurs;
      break;
    case (FILENUMBER(FILEPERIF_CTRLVVREGISTER2021)+RECORDNUMBER(21)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.setpoint_pochatkova_k_vymk;
      break;
////CTRLVV-CTRLVV-CTRLVV-CTRLVV-CTRLVV-CTRLVV-CTRLVV-CTRLVV-CTRLVV-CTRLVV-CTRLVV-
////REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+REGANL+
    case (FILENUMBER(FILEPERIF_REGANLREGISTER2021)+RECORDNUMBER(1)):
      switch(registerNumber)
      {
      case 1:
        (*multer) = 1;
        //Время записи аналогового регистратора (доаварийный массив)
        //В таблицю настройок записуємо не мілісекунди, а кількість періодів
        (*editValue) = (uint32_t*)&edition_settings.prefault_number_periods;
        if(regUst<TIMEOUT_PREFAULT_MIN/20 || regUst>TIMEOUT_PREFAULT_MAX/20)  diapazon=0;
        break;
      case 2:
        (*multer) = 1;
        //Время записи аналогового регистратора (послеаварый массив)
        //В таблицю настройок записуємо не мілісекунди, а кількість періодів
        (*editValue) = (uint32_t*)&edition_settings.postfault_number_periods;
        if(regUst<TIMEOUT_POSTFAULT_MIN/20 || regUst>TIMEOUT_POSTFAULT_MAX/20)  diapazon=0;
        break;
      }//switch
      break;
////REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-REGANL-
////REGDIS+REGDIS+REGDIS+REGDIS+REGDIS+REGDIS+REGDIS+REGDIS+REGDIS+REGDIS+REGDIS+
    case (FILENUMBER(FILEPERIF_REGDISREGISTER2021)+RECORDNUMBER(1)):
      //Выдержка доаварийного массива DR
      (*editValue) = (uint32_t*)&edition_settings.timeout_prolongation_work_digital_registrator;
      if(regUst<TIMEOUT_DR_ELONGATION_MIN/10 || regUst>TIMEOUT_DR_ELONGATION_MAX/10) diapazon=0;
      break;
////REGDIS-REGDIS-REGDIS-REGDIS-REGDIS-REGDIS-REGDIS-REGDIS-REGDIS-REGDIS-REGDIS-
////TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+TRANS+
    case (FILENUMBER(FILEPERIF_TRANSREGISTER2021)+RECORDNUMBER(1)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.TVoltage;
      if(regUst<KOEF_TN_MIN || regUst>KOEF_TN_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_TRANSREGISTER2021)+RECORDNUMBER(2)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.TCurrent;
      if(regUst<KOEF_TT_MIN || regUst>KOEF_TT_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_TRANSREGISTER2021)+RECORDNUMBER(7)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.T0;
      if(regUst<KOEF_TO_MIN || regUst>KOEF_TO_MAX) diapazon=0;
      break;
    case (FILENUMBER(FILEPERIF_TRANSREGISTER2021)+RECORDNUMBER(8)):
      (*multer) = 1;
      (*editValue) = (uint32_t*)&edition_settings.TCurrent04;
      if(regUst<KOEF_TT04_MIN || regUst>KOEF_TT04_MAX) diapazon=0;
      break;
////TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-TRANS-
////PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+PKV1+
    case (FILENUMBER(FILEPERIF_PKV1REGISTER2021)+RECORDNUMBER(1)):
      (*multer) = 1;
      //Язык пользовательского интерфейса
      (*editValue) = (uint32_t *)&edition_settings.language;
      if(regUst<VALUE_SETTING_LANGUAGE_MIN || regUst>VALUE_SETTING_LANGUAGE_MAX) diapazon=0;
      break;
////PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-PKV1-
////PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+PKV2+
    case (FILENUMBER(FILEPERIF_PKV2REGISTER2021)+RECORDNUMBER(2)):
      (*multer) = 1;
      switch(pointInterface)//Время активации пароля после простоя
      {
      case USB_RECUEST:
        (*editValue) = &edition_settings.timeout_deactivation_password_interface_USB;
        break;
      case RS485_RECUEST:
        (*editValue) = &edition_settings.timeout_deactivation_password_interface_RS485;
        break;
#if (((MODYFIKACIA_VERSII_PZ / 10) & 0x1) != 0)
      case LAN_RECUEST:
        (*editValue) = &edition_settings.timeout_deactivation_password_interface_LAN;
        break;
#endif
      }//switch
      break;
    case (FILENUMBER(FILEPERIF_PKV2REGISTER2021)+RECORDNUMBER(3)):
      (*multer) = 1;
      //Тайм-аут применения изменений
      (*editValue) = &edition_settings.timeout_idle_new_settings;
      if(regUst<TIMEOUT_NEW_SETTINGS_MIN || regUst>TIMEOUT_NEW_SETTINGS_MAX) diapazon=0;
      break;
////PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-PKV2-
////PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+PKV4+
    case (FILENUMBER(FILEPERIF_PKV4REGISTER2021)+RECORDNUMBER(2)):
      switch(registerNumber)
      {
      case 1:
        //Скорость порта связи
        (*multer) = 1;
        (*editValue) = (uint32_t *)&edition_settings.speed_RS485;
        if(regUst<VALUE_SPEED_INTERFACE_MIN || regUst>VALUE_SPEED_INTERFACE_MAX) diapazon=0;
        break;
      case 3://Паритет
        (*multer) = 1;
        (*editValue) = (uint32_t *)&edition_settings.pare_bit_RS485;
        if(regUst<VALUE_PARE_INTERFACE_MIN || regUst>VALUE_PARE_INTERFACE_MAX) diapazon=0;
        break;
      case 4://Количество стоп-бит
        (*multer) = 1;
        (*editValue) = (uint32_t *)&edition_settings.number_stop_bit_RS485;
        if(regUst<1 || regUst>MAX_NUMBER_STOP_BITS_INTERFACE) diapazon=0;
        break;
      }//switch
      break;
    case (FILENUMBER(FILEPERIF_PKV4REGISTER2021)+RECORDNUMBER(3)):
      //Задержка приёма
      (*multer) = 1;
      (*editValue) = &edition_settings.time_out_1_RS485;
      if(regUst<VALUE_TIME_OUT_1_INTERFACE_MIN || regUst>VALUE_TIME_OUT_1_INTERFACE_MAX) diapazon=0;
      break;
////PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-PKV4-
////PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+PKV6+
    case (FILENUMBER(FILEPERIF_PKV6REGISTER2021)+RECORDNUMBER(2)):
      //Адрес устройства в сети
      (*multer) = 1;
      (*editValue) = &edition_settings.address;
      if(regUst<KOEF_ADDRESS_MIN || regUst>KOEF_ADDRESS_MAX) diapazon=0;
      break;
////PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-PKV6-

    }//switch

  return diapazon;
}//ustFunc000




