
#include "header.h"

//��������� ������� � ����� ������
#define BEGIN_ADR_REGISTER 332
//�������� ������� � ����� ������
#define END_ADR_REGISTER 408

int getAISmallModbusRegister(int);//�������� ���������� ��������
int getAISmallModbusBit(int);//�������� ���������� ����
int setAISmallModbusRegister(int, int);//�������� ���������� ��������
int setAISmallModbusBit(int, int);//�������� ���������� ����

void preAISmallReadAction(void);//action �� ������
void preAISmallWriteAction(void);//action �� ������
int  postAISmallWriteAction(void);//action ����� ������
void loadAISmallActualData(void);

int privateAISmallGetReg2(int adrReg);

COMPONENT_OBJ *aismallcomponent;

/**************************************/
//��������� ���������
/**************************************/
void constructorAISmallComponent(COMPONENT_OBJ *aismallcomp)
{
  aismallcomponent = aismallcomp;

  aismallcomponent->getModbusRegister = getAISmallModbusRegister;//�������� ���������� ��������
  aismallcomponent->getModbusBit      = getAISmallModbusBit;//�������� ���������� ����
  aismallcomponent->setModbusRegister = setAISmallModbusRegister;//�������� ���������� ��������
  aismallcomponent->setModbusBit      = setAISmallModbusBit;//�������� ���������� ����

  aismallcomponent->preReadAction   = preAISmallReadAction;//action �� ������
  aismallcomponent->preWriteAction  = preAISmallWriteAction;//action �� ������
  aismallcomponent->postWriteAction = postAISmallWriteAction;//action ����� ������

  aismallcomponent->isActiveActualData = 0;
}//constructorIUSmallComponent(COMPONENT_OBJ *iucomp)

int getAISmallModbusRegister(int adrReg)
{
  //�������� ���������� ��������
  if(privateAISmallGetReg2(adrReg)==MARKER_OUTPERIMETR) return MARKER_OUTPERIMETR;
    int offset = adrReg-BEGIN_ADR_REGISTER;
    switch (offset)
    {
    case 0://UA
      {
        return measurement_low[IM_UA] >> 3;
      }
    case 1://UB
      {
        return measurement_low[IM_UB] >> 3;
      }
    case 2://UC
      {
        return measurement_low[IM_UC] >> 3;
      }

    case 3://IA
      {
        return measurement_low[IM_IA] >> 2;
      }
    case 4://IB
      {
        return measurement_low[IM_IB] >> 2;
      }
    case 5://IC
      {
        return measurement_low[IM_IC] >> 2;
      }

    case 6://I04
      {
        return measurement_low[IM_I04] >> 2;
      }
    case 7://P
      {
        return P[0]/50;
      }
    case 8://P
      {
        return Q[0]/50;
      }
    case 9://P
      {
        return S[0]/50;
      }
    case 10://cos f
      {
        if (S != 0)
          return cos_phi_x1000;
        else
          return 0x0;
      }
    case 11://3I0
      {
        return measurement_low[IM_3I0];
      }
    case 12://freq
      {
        int int_frequency = (int)(frequency*100);
        
        if (int_frequency > 0 /*�� ����� ������ - ������� �� ���������*/)
          return int_frequency;
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
        unsigned int enrg = (unsigned int)(energy[(offset-13) >> 1]*1000.0);
        if((offset-13)&1)
          //�������� ������ �����
        return (enrg>> 16)& 0xffff;
          //�������� ������� �����
        else return enrg& 0xffff;
      } break;
    case 25://UAB
      {
        return measurement_low[IM_UAB] >> 3;
      }
    case 26://UBC
      {
        return measurement_low[IM_UBC] >> 3;
      }
    case 27://UCA
      {
        return measurement_low[IM_UCA] >> 3;
      }
    case 28://3U0
      {
        return measurement_low[IM_3U0] >> 3;
      }
    case 29://3I0-1
      {
        return measurement_low[IM_3I0];
      }
    case 30://IM_3I0_other_g
      {
        return measurement_low[IM_3I0_other_g];
      }
    case 31://IM_I1
      {
        return measurement_low[IM_I1] >> 2;
      }
    case 32://IM_I2
      {
        return measurement_low[IM_I2] >> 2;
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
//    case 44://U1
//    case 45://U2
//    case 46://freq TH2
//    case 47://Z0
    case 48://baza
      {
        if (base_index_for_angle <= FULL_ORT_3U0) return base_index_for_angle + 1;
        else
        {
          //���������� ����� ����� �� ���� � ����
          total_error_sw_fixed(72);
        }
      } break;
    case 49://OFFSET_ANGLE_UA_1
      {
        return (unsigned int)phi_angle[FULL_ORT_Ua];
      }
    case 50://OFFSET_ANGLE_UB_1
      {
        return (unsigned int)phi_angle[FULL_ORT_Ub];
      }
    case 51://OFFSET_ANGLE_UC_1
      {
        return (unsigned int)phi_angle[FULL_ORT_Uc];
      }
    case 52://OFFSET_ANGLE_UAB_1
      {
        return (unsigned int)phi_angle[FULL_ORT_Uab];
      }
    case 53://OFFSET_ANGLE_UBC_1
      {
        return (unsigned int)phi_angle[FULL_ORT_Ubc];
      }
    case 54://OFFSET_ANGLE_UCA_1
      {
        return (unsigned int)phi_angle[FULL_ORT_Uca];
      }
    case 55://OFFSET_ANGLE_3U0_1
      {
        return (unsigned int)phi_angle[FULL_ORT_3U0];
      }
//    case 56://OFFSET_ANGLE_UA_2
//    case 57://OFFSET_ANGLE_UB_2
//    case 58://OFFSET_ANGLE_UC_2
//    case 59://OFFSET_ANGLE_UAB_2
//    case 60://OFFSET_ANGLE_UBC_2
//    case 61://OFFSET_ANGLE_UCA_2
    case 62://OFFSET_ANGLE_Ia_1
      {
        return (unsigned int)phi_angle[FULL_ORT_Ia];
      }
    case 63://OFFSET_ANGLE_Ib_1
      {
        return (unsigned int)phi_angle[FULL_ORT_Ib];
      }
    case 64://OFFSET_ANGLE_Ic_1
      {
        return (unsigned int)phi_angle[FULL_ORT_Ic];
      }
    case 65://OFFSET_ANGLE_I04
      {
        return (unsigned int)phi_angle[FULL_ORT_I04];
      }
    case 66://OFFSET_ANGLE_3I0_1
      {
        return (unsigned int)phi_angle[FULL_ORT_3I0];
      }
    case 67://OFFSET_ANGLE_3I0_r
      {
        return (unsigned int)phi_angle[FULL_ORT_3I0_r];
      }
//    case 68://
//    case 69://
//    case 70://
//    case 71://
//    case 72://
    case 73://
      {
        return resurs_vidkljuchennja & 0xffff;
      }
    case 74://
      {
        return (resurs_vidkljuchennja >> 16) & 0xffff;
      }
    case 75://
      {
        return resurs_vymykacha & 0xffff;
      }
    case 76://
      {
        return (resurs_vymykacha >> 16) & 0xffff;
      }

    }//switch
    return 0;
}//getDVModbusRegister(int adrReg)
int getAISmallModbusBit(int x) {
  //�������� ���������� bit
  UNUSED(x);
  return MARKER_OUTPERIMETR;
}//getIUModbusBit(int )
int setAISmallModbusRegister(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� ��������
  return MARKER_OUTPERIMETR;
}//setIUModbusRegister(int, int)
int setAISmallModbusBit(int x, int y) {
  UNUSED(x);
  UNUSED(y);
  //�������� ���������� bit
  return MARKER_OUTPERIMETR;
}//setIUModbusBit(int, int )

void preAISmallReadAction(void) {
//action �� ������
  aismallcomponent->isActiveActualData = 1;
}//

void preAISmallWriteAction(void) {
//action �� ������
  aismallcomponent->operativMarker[0] = -1;
  aismallcomponent->operativMarker[1] = -1;//����������� ������
  aismallcomponent->isActiveActualData = 1;
}//
int postAISmallWriteAction(void) {
//action ����� ������
 return 0;
}//

int privateAISmallGetReg2(int adrReg)
{
  //��������� ������� ��������
 return controlPerimetr(adrReg, BEGIN_ADR_REGISTER, END_ADR_REGISTER);
}//privateGetReg2(int adrReg)

