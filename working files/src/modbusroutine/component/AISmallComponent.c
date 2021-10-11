
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 1000
//конечный регистр в карте памяти
#define END_ADR_REGISTER 1094

int getAISmallModbusRegister(int);//получить содержимое регистра
int getAISmallModbusBit(int);//получить содержимое бита
int setAISmallModbusRegister(int, int);//получить содержимое регистра
int setAISmallModbusBit(int, int);//получить содержимое бита

int  postAISmallWriteAction(void);//action после записи
void loadAISmallActualData(void);

int privateAISmallGetReg2(int adrReg);

SRAM1 COMPONENT_OBJ *aismallcomponent;

/**************************************/
//компонент измерений
/**************************************/
void constructorAISmallComponent(COMPONENT_OBJ *aismallcomp)
{
  aismallcomponent = aismallcomp;

  aismallcomponent->getModbusRegister = getAISmallModbusRegister;//получить содержимое регистра
  aismallcomponent->getModbusBit      = getAISmallModbusBit;//получить содержимое бита
  aismallcomponent->setModbusRegister = setAISmallModbusRegister;//получить содержимое регистра
  aismallcomponent->setModbusBit      = setAISmallModbusBit;//получить содержимое бита

  aismallcomponent->postWriteAction = postAISmallWriteAction;//action после записи
}//constructorIUSmallComponent(COMPONENT_OBJ *iucomp)

int getAISmallModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateAISmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
    int offset = adrReg-BEGIN_ADR_REGISTER;

  semaphore_measure_values_low1 = 1;
  for (unsigned int i = 0; i < _NUMBER_IM; i++ ) 
  {
    measurement_low[i] = measurement_middle[i];
  }
  semaphore_measure_values_low1 = 0;

    switch (offset)
    {
    case 0://UA
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1000;
#else
        return (measurement_low[IM_UA] >> 3) &0xFFFF;
#endif
      }
    case 1://UB
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1001;
#else
        return (measurement_low[IM_UB] >> 3) &0xFFFF;
#endif
      }
    case 2://UC
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1002;
#else
        return (measurement_low[IM_UC] >> 3) &0xFFFF;
#endif
      }

    case 3://IA
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1003;
#else
        return (measurement_low[IM_IA] >> 2) &0xFFFF;
#endif
      }
    case 4://IB
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1004;
#else
        return (measurement_low[IM_IB] >> 2) &0xFFFF;
#endif
      }
    case 5://IC
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1005;
#else
        return (measurement_low[IM_IC] >> 2) &0xFFFF;
#endif
      }

    case 6://I04
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1006;
#else
        return (measurement_low[IM_I04] >> 2) &0xFFFF;
#endif
      }
    case 7://P
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1007;
#else
        return (P[bank_for_calc_power]/50) &0xFFFF;
#endif
      }
    case 8://P
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1008;
#else
        return (Q[bank_for_calc_power]/50) &0xFFFF;
#endif
      }
    case 9://P
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1009;
#else
        return (S[bank_for_calc_power]/50) &0xFFFF;
#endif
      }
    case 10://cos f
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1010;
#else
        if (S[bank_for_calc_power] != 0)
          return (cos_phi_x1000[bank_for_calc_power]) &0xFFFF;
          return 0x0;
#endif
      }
    case 11://3I0
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1011;
#else
        return (measurement_low[IM_3I0]) &0xFFFF;
#endif
      }
    case 12://freq
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1012;
#else
        int int_frequency = (int)(frequency*100);
        
        if (int_frequency > 0 /*це число означає - частота не визначена*/)
          return (int_frequency) &0xFFFF;
        else
        {
          if (int_frequency == (-1*100))
            return (-1)&0x0000FFFF;
          else if (int_frequency == (-2*100))
            return (-2)&0x0000FFFF;
          else if (int_frequency == (-3*100))
            return (-3)&0x0000FFFF;
          else
            return (-4)&0x0000FFFF;
        }
#endif
      } break;
    case 13://Ea+
    case 14://Ea+
    case 15://Ea-
    case 16://Ea-
    case 17://Eq1
    case 18://Eq1
    case 19://Eq2
    case 20://Eq2
    case 21://Eq3
    case 22://Eq3
    case 23://Eq4
    case 24://Eq4
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1000+(offset-13);
#else
        unsigned int enrg = (unsigned int)(energy[0][(offset-13) >> 1]*1000.0);
        if((offset-13)&1)
          //Передаємо старше слово
        return (enrg>> 16)& 0xffff;
          //Передаємо молодше слово
        return enrg& 0xffff;
#endif
      } break;
    case 25://UAB
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1025;
#else
        return (measurement_low[IM_UAB] >> 3) &0xFFFF;
#endif
      }
    case 26://UBC
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1026;
#else
        return (measurement_low[IM_UBC] >> 3) &0xFFFF;
#endif
      }
    case 27://UCA
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1027;
#else
        return (measurement_low[IM_UCA] >> 3) &0xFFFF;
#endif
      }
    case 28://3U0
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1028;
#else
        return (measurement_low[IM_3U0] >> 3) &0xFFFF;
#endif
      }

//
    case 30://3I0-1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1030;
#else
        return (measurement_low[IM_3I0_r] >> 2) &0xFFFF;
#endif
      }
    case 31://IM_3I0_other_g
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1031;
#else
        return (measurement_low[IM_3I0_other_g]) &0xFFFF;
#endif
      }
    case 32://IM_I1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1032;
#else
        return (measurement_low[IM_I1] >> 2) &0xFFFF;
#endif
      }
    case 33://IM_I2
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1033;
#else
        return (measurement_low[IM_I2] >> 2) &0xFFFF;
#endif
      }
//    case 33://Iah2
//    case 34://Ibh2
//    case 35://Ich2
//    case 36://Uab TCH
//    case 37://Ubc TCH
//    case 38://Ua TH2
//    case 39://Ub TH2
//    case 40://Uc TH2
//    case 41://Uab TH2
//    case 42://Ubc TH2
//    case 43://Uca TH2
    case 42://U1
#ifdef TESTZBIRKA_VERSII_PZ
        return 1042;
#else
        return (measurement_low[IM_U1] >> 3) &0xFFFF;
#endif
    case 43://U2
#ifdef TESTZBIRKA_VERSII_PZ
        return 1043;
#else
        return (measurement_low[IM_U2] >> 3) &0xFFFF;
#endif
//    case 46://freq TH2
//    case 47://Z0
    case 63://baza
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1063;
#else
        if (base_index_for_angle <= FULL_ORT_3U0) return (base_index_for_angle + 1) &0xFFFF;
        else
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed();
          return 0;
        }
#endif
      } break;
    case 64://OFFSET_ANGLE_UA_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1064;
#else
        return ((unsigned int)phi_angle[bank_for_calc_phi_angle][FULL_ORT_Ua]) &0xFFFF;
#endif
      }
    case 65://OFFSET_ANGLE_UB_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1065;
#else
        return ((unsigned int)phi_angle[bank_for_calc_phi_angle][FULL_ORT_Ub]) &0xFFFF;
#endif
      }
    case 66://OFFSET_ANGLE_UC_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1066;
#else
        return ((unsigned int)phi_angle[bank_for_calc_phi_angle][FULL_ORT_Uc]) &0xFFFF;
#endif
      }
    case 67://OFFSET_ANGLE_UAB_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1067;
#else
        return ((unsigned int)phi_angle[bank_for_calc_phi_angle][FULL_ORT_Uab]) &0xFFFF;
#endif
      }
    case 68://OFFSET_ANGLE_UBC_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1068;
#else
        return ((unsigned int)phi_angle[bank_for_calc_phi_angle][FULL_ORT_Ubc]) &0xFFFF;
#endif
      }
    case 69://OFFSET_ANGLE_UCA_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1069;
#else
        return ((unsigned int)phi_angle[bank_for_calc_phi_angle][FULL_ORT_Uca]) &0xFFFF;
#endif
      }
    case 70://OFFSET_ANGLE_3U0_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1070;
#else
        return ((unsigned int)phi_angle[bank_for_calc_phi_angle][FULL_ORT_3U0]) &0xFFFF;
#endif
      }
    case 71://OFFSET_ANGLE_Ia_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1071;
#else
        return ((unsigned int)phi_angle[bank_for_calc_phi_angle][FULL_ORT_Ia]) &0xFFFF;
#endif
      }
    case 72://OFFSET_ANGLE_Ib_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1072;
#else
        return ((unsigned int)phi_angle[bank_for_calc_phi_angle][FULL_ORT_Ib]) &0xFFFF;
#endif
      }
    case 73://OFFSET_ANGLE_Ic_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1073;
#else
        return ((unsigned int)phi_angle[bank_for_calc_phi_angle][FULL_ORT_Ic]) &0xFFFF;
#endif
      }
    case 74://OFFSET_ANGLE_I04
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1074;
#else
        return ((unsigned int)phi_angle[bank_for_calc_phi_angle][FULL_ORT_I04]) &0xFFFF;
#endif
      }
    case 75://OFFSET_ANGLE_3I0_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1075;
#else
        return ((unsigned int)phi_angle[bank_for_calc_phi_angle][FULL_ORT_3I0]) &0xFFFF;
#endif
      }
    case 76://OFFSET_ANGLE_3I0_r
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1076;
#else
        return ((unsigned int)phi_angle[bank_for_calc_phi_angle][FULL_ORT_3I0_r]) &0xFFFF;
#endif
      }
//    case 68://
//    case 69://
//    case 70://
//    case 71://
//    case 72://
    case 92://
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1092;
#else
        return resurs_vidkljuchennja & 0xffff;
#endif
      }
    case 93://
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1093;
#else
        return (resurs_vidkljuchennja >> 16) & 0xffff;
#endif
      }
    case 94://
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1094;
#else
        return resurs_vymykacha & 0xffff;
#endif
      }
    case 95://
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 1095;
#else
        return (resurs_vymykacha >> 16) & 0xffff;
#endif
      }

    }//switch
    return 0;
}//getDVModbusRegister(int adrReg)
int getAISmallModbusBit(int x) {
  //получить содержимое bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getIUModbusBit(int )
int setAISmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое регистра
  return MARKER_OUTPERIMETR;
}//setIUModbusRegister(int, int)
int setAISmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //записать содержимое bit
  return MARKER_OUTPERIMETR;
}//setIUModbusBit(int, int )

int postAISmallWriteAction(void) {
//action после записи
 return 0;
}//

int privateAISmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

