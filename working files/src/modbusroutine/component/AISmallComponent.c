
#include "header.h"

//начальный регистр в карте памяти
#define BEGIN_ADR_REGISTER 332
//конечный регистр в карте памяти
#define END_ADR_REGISTER 408

int getAISmallModbusRegister(int);//получить содержимое регистра
int getAISmallModbusBit(int);//получить содержимое бита
int setAISmallModbusRegister(int, int);//получить содержимое регистра
int setAISmallModbusBit(int, int);//получить содержимое бита

void preAISmallReadAction(void);//action до чтения
void preAISmallWriteAction(void);//action до записи
int  postAISmallWriteAction(void);//action после записи
void loadAISmallActualData(void);

int privateAISmallGetReg2(int adrReg);

COMPONENT_OBJ *aismallcomponent;

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

  aismallcomponent->preReadAction   = preAISmallReadAction;//action до чтения
  aismallcomponent->preWriteAction  = preAISmallWriteAction;//action до записи
  aismallcomponent->postWriteAction = postAISmallWriteAction;//action после записи

  aismallcomponent->isActiveActualData = 0;
}//constructorIUSmallComponent(COMPONENT_OBJ *iucomp)

int getAISmallModbusRegister(int adrReg)
{
  //получить содержимое регистра
  if(privateAISmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
    int offset = adrReg-BEGIN_ADR_REGISTER;
    switch (offset)
    {
    case 0://UA
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 332;
#else
        return (measurement_low[IM_UA] >> 3) &0xFFFF;
#endif
      }
    case 1://UB
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 333;
#else
        return (measurement_low[IM_UB] >> 3) &0xFFFF;
#endif
      }
    case 2://UC
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 334;
#else
        return (measurement_low[IM_UC] >> 3) &0xFFFF;
#endif
      }

    case 3://IA
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 335;
#else
        return (measurement_low[IM_IA] >> 2) &0xFFFF;
#endif
      }
    case 4://IB
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 336;
#else
        return (measurement_low[IM_IB] >> 2) &0xFFFF;
#endif
      }
    case 5://IC
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 337;
#else
        return (measurement_low[IM_IC] >> 2) &0xFFFF;
#endif
      }

    case 6://I04
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 338;
#else
        return (measurement_low[IM_I04] >> 2) &0xFFFF;
#endif
      }
    case 7://P
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 339;
#else
        return (P[0]/50) &0xFFFF;
#endif
      }
    case 8://P
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 340;
#else
        return (Q[0]/50) &0xFFFF;
#endif
      }
    case 9://P
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 341;
#else
        return (S[0]/50) &0xFFFF;
#endif
      }
    case 10://cos f
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 342;
#else
        if (S != 0)
          return (cos_phi_x1000) &0xFFFF;
          return 0x0;
#endif
      }
    case 11://3I0
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 343;
#else
        return (measurement_low[IM_3I0]) &0xFFFF;
#endif
      }
    case 12://freq
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 344;
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
        return 345+(offset-13);
#else
        unsigned int enrg = (unsigned int)(energy[(offset-13) >> 1]*1000.0);
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
        return 357;
#else
        return (measurement_low[IM_UAB] >> 3) &0xFFFF;
#endif
      }
    case 26://UBC
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 358;
#else
        return (measurement_low[IM_UBC] >> 3) &0xFFFF;
#endif
      }
    case 27://UCA
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 359;
#else
        return (measurement_low[IM_UCA] >> 3) &0xFFFF;
#endif
      }
    case 28://3U0
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 360;
#else
        return (measurement_low[IM_3U0] >> 3) &0xFFFF;
#endif
      }
    case 29://3I0-1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 361;
#else
        return (measurement_low[IM_3I0_r] >> 2) &0xFFFF;
#endif
      }
    case 30://IM_3I0_other_g
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 362;
#else
        return (measurement_low[IM_3I0_other_g]) &0xFFFF;
#endif
      }
    case 31://IM_I1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 363;
#else
        return (measurement_low[IM_I1] >> 2) &0xFFFF;
#endif
      }
    case 32://IM_I2
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 364;
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
    case 44://U1
#ifdef TESTZBIRKA_VERSII_PZ
        return 376;
#else
        return (measurement_low[IM_U1] >> 3) &0xFFFF;
#endif
    case 45://U2
#ifdef TESTZBIRKA_VERSII_PZ
        return 377;
#else
        return (measurement_low[IM_U2] >> 3) &0xFFFF;
#endif
//    case 46://freq TH2
//    case 47://Z0
    case 48://baza
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 380;
#else
        if (base_index_for_angle <= FULL_ORT_3U0) return (base_index_for_angle + 1) &0xFFFF;
        else
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed(72);
          return 0;
        }
#endif
      } break;
    case 49://OFFSET_ANGLE_UA_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 381;
#else
        return ((unsigned int)phi_angle[FULL_ORT_Ua]) &0xFFFF;
#endif
      }
    case 50://OFFSET_ANGLE_UB_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 382;
#else
        return ((unsigned int)phi_angle[FULL_ORT_Ub]) &0xFFFF;
#endif
      }
    case 51://OFFSET_ANGLE_UC_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 383;
#else
        return ((unsigned int)phi_angle[FULL_ORT_Uc]) &0xFFFF;
#endif
      }
    case 52://OFFSET_ANGLE_UAB_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 384;
#else
        return ((unsigned int)phi_angle[FULL_ORT_Uab]) &0xFFFF;
#endif
      }
    case 53://OFFSET_ANGLE_UBC_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 385;
#else
        return ((unsigned int)phi_angle[FULL_ORT_Ubc]) &0xFFFF;
#endif
      }
    case 54://OFFSET_ANGLE_UCA_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 386;
#else
        return ((unsigned int)phi_angle[FULL_ORT_Uca]) &0xFFFF;
#endif
      }
    case 55://OFFSET_ANGLE_3U0_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 387;
#else
        return ((unsigned int)phi_angle[FULL_ORT_3U0]) &0xFFFF;
#endif
      }
//    case 56://OFFSET_ANGLE_UA_2
//    case 57://OFFSET_ANGLE_UB_2
//    case 58://OFFSET_ANGLE_UC_2
//    case 59://OFFSET_ANGLE_UAB_2
//    case 60://OFFSET_ANGLE_UBC_2
//    case 61://OFFSET_ANGLE_UCA_2
    case 62://OFFSET_ANGLE_Ia_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 394;
#else
        return ((unsigned int)phi_angle[FULL_ORT_Ia]) &0xFFFF;
#endif
      }
    case 63://OFFSET_ANGLE_Ib_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 395;
#else
        return ((unsigned int)phi_angle[FULL_ORT_Ib]) &0xFFFF;
#endif
      }
    case 64://OFFSET_ANGLE_Ic_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 396;
#else
        return ((unsigned int)phi_angle[FULL_ORT_Ic]) &0xFFFF;
#endif
      }
    case 65://OFFSET_ANGLE_I04
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 397;
#else
        return ((unsigned int)phi_angle[FULL_ORT_I04]) &0xFFFF;
#endif
      }
    case 66://OFFSET_ANGLE_3I0_1
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 398;
#else
        return ((unsigned int)phi_angle[FULL_ORT_3I0]) &0xFFFF;
#endif
      }
    case 67://OFFSET_ANGLE_3I0_r
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 399;
#else
        return ((unsigned int)phi_angle[FULL_ORT_3I0_r]) &0xFFFF;
#endif
      }
//    case 68://
//    case 69://
//    case 70://
//    case 71://
//    case 72://
    case 73://
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 405;
#else
        return resurs_vidkljuchennja & 0xffff;
#endif
      }
    case 74://
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 406;
#else
        return (resurs_vidkljuchennja >> 16) & 0xffff;
#endif
      }
    case 75://
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 407;
#else
        return resurs_vymykacha & 0xffff;
#endif
      }
    case 76://
      {
#ifdef TESTZBIRKA_VERSII_PZ
        return 408;
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

void preAISmallReadAction(void) {
//action до чтения
  aismallcomponent->isActiveActualData = 1;
}//

void preAISmallWriteAction(void) {
//action до записи
  aismallcomponent->operativMarker[0] = -1;
  aismallcomponent->operativMarker[1] = -1;//оперативный маркер
  aismallcomponent->isActiveActualData = 1;
}//
int postAISmallWriteAction(void) {
//action после записи
 return 0;
}//

int privateAISmallGetReg2(int adrReg)
{
  //проверить внешний периметр
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

