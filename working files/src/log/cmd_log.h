#ifndef LOG_H
#define LOG_H
//#include <time.h>
//#include <stdint.h>
#include "constants.h"
#include "libraries.h"
#include "variables_external.h"
#include "functions_external.h"


typedef union{
    int32_t time_ms;
    unsigned long uLMkTime;
    unsigned char uChMkTime[4];
    
}UNN_MicroSec;

typedef union{
    time_t time_dat;
    unsigned long arU32MkTime[sizeof(time_t)>>2];
    unsigned char arU08MkTime[sizeof(time_t)];
    
}UNN_UnixTime;

typedef union{
    unsigned long uLCmd[9];
    unsigned char uChCmd[36];
    
}UNN_CmdState;

typedef struct tag_CmdPlusTimeStateElem{
    UNN_CmdState cmd;
    UNN_UnixTime unix_time;
    UNN_MicroSec mksec;
} CmdPlusTimeStateElem;

#define AMOUNT_CMD_PLUS_TIME_RECORD 400 //400





#define AMOUNT_CMD_PLUS_TIME_STAMP_RECORD 100 //400
#define FIFTY_YEAR_NUMBER  ((unsigned long long)1000*3600*365*50)

typedef struct tag_CmdPlusTimeHolder{
    CmdPlusTimeStateElem arrCmdPlusTimeHolder[AMOUNT_CMD_PLUS_TIME_RECORD];
    short shIndexWR;
	short shTotalFixElem;
} CmdPlusTimeHolder;    
extern CmdPlusTimeHolder holderCmdPlusTime;



typedef struct tag_CmdPlusTimeStampElem{
    UNN_CmdState cmd;
    UNN_UnixTime unix_time;
    UNN_MicroSec mksec;
} CmdPlusTimeStampElem;

typedef struct tag_AdditionalInfoCmdPlusTimeStamp{
	unsigned long long ullTimeStamp;
	long lNumAlterSigInElem;// analog number_changes_into_current_item
} AdditionalInfoCmdPlusTimeStamp;

typedef struct tag_CmdPlusTimeStampHolder{
    CmdPlusTimeStampElem arrCmdPlusTimeStampElem[AMOUNT_CMD_PLUS_TIME_STAMP_RECORD];
	AdditionalInfoCmdPlusTimeStamp arrAdditionalInfoCmdPlusTimeStamp[AMOUNT_CMD_PLUS_TIME_STAMP_RECORD];
    short shIndexWR;
	short shIndexRD;
	short shTotalFixElem;
	short shSumDifersElem;
	short shAmount100msElem;
	unsigned long long uLLDrecTimeStampVal;
} CmdPlusTimeStampHolder;    
extern CmdPlusTimeStampHolder holderCmdPlusTimeStamp;

typedef struct tag_dig_reg_uniq_vars{
   unsigned int *number_items_dr;
   unsigned int *number_changes_into_dr_record;
   unsigned int *time_from_start_record_dr;
   unsigned int *blocking_continue_monitoring_min_U;	
   unsigned int *previous_active_functions;
   unsigned int *p_active_functions;
} DigRegUniqVarsAddreses;






void ChekCurrentStateCmd(unsigned int *p_active_functions);
void CmdPlusTimeLogHundler(unsigned int *p_active_functions);
extern void GetCmdPlusTimeLogElem(unsigned int *p_elem, long lIdx);
extern void GetDateTimeLogElem(unsigned int *p_elem, long lIdx);
extern void GetMsLogElem(unsigned int *p_elem, long lIdx);

extern void CmdPlusTimeStampLogHundler(unsigned int *p_active_functions);
extern void put_before_info_in_buf (DigRegUniqVarsAddreses *ptDRecUniqVarsAddreses);

extern void TestStateNameSpaceFooBrrr(DigRegUniqVarsAddreses *ptDRecUniqVarsAddreses );







typedef union U32_cmd1_Unn{ 
   struct {
      unsigned int BLOCK_VKL_VV                :1; //0
      unsigned int RESET_LEDS                  :1; //1
      unsigned int RESET_RELES                 :1; //2
      unsigned int MISCEVE_DYSTANCIJNE         :1; //3
      unsigned int STATE_VV                    :1; //4
      unsigned int OTKL_VID_ZOVN_ZAHYSTIV      :1; //5
      unsigned int VKL_VV                      :1; //6
      unsigned int CTRL_VKL                    :1; //7
      unsigned int OTKL_VV                     :1; //8
      unsigned int CTRL_OTKL                   :1; //9
      unsigned int PRYVID_VV                   :1;//10
      unsigned int PEREVYSHCHENNJA_Inom_V      :1;//11
      unsigned int KRYTYCHNYJ_RESURS_VYMY      :1;//12
      unsigned int VYCHERPANYJ_RESURS_VYM      :1;//13
      unsigned int DEFECT                      :1;//14
      unsigned int AVAR_DEFECT                 :1;//15
      unsigned int WORK_A_REJESTRATOR          :1;//16
      unsigned int WORK_D_REJESTRATOR          :1;//17
      unsigned int VIDKL_VID_ZAKHYSTIV         :1;//18
      unsigned int WORK_BO                     :1;//19
      unsigned int WORK_BV                     :1;//20
      unsigned int l1_GRUPA_USTAVOK             :1;//21
      unsigned int l2_GRUPA_USTAVOK             :1;//22
      unsigned int l3_GRUPA_USTAVOK             :1;//23
      unsigned int l4_GRUPA_USTAVOK             :1;//24
      unsigned int INVERS_DV_GRUPA_USTAVO      :1;//25
      unsigned int BLK_GRUP_USTAVOK_VID_Z      :1;//26
      unsigned int RESET_BLOCK_READY_TU_V      :1;//27
      unsigned int READY_TU                    :1;//28
      unsigned int SETTINGS_CHANGED            :1;//29
      unsigned int BLOCK_MTZ1                  :1;//30
      unsigned int BLOCK_MTZ2                  :1;//31    
  
    } bool_val;
    unsigned long U32V;
} U32_cmd1_wrp; 

typedef union U32_cmd2_Unn{ 
   struct {
      unsigned int BLOCK_USK_MTZ2            :1; //0
      unsigned int BLOCK_MTZ3                :1; //1
      unsigned int BLOCK_MTZ4                :1; //2
      unsigned int SECTOR_VPERED_MTZN1       :1; //3
      unsigned int SECTOR_NAZAD_MTZN1        :1; //4
      unsigned int PO_MTZ1                   :1; //5
      unsigned int PO_MTZN1_VPERED           :1; //6
      unsigned int PO_MTZN1_NAZAD            :1; //7
      unsigned int PO_U_MTZPN1               :1; //8
      unsigned int PO_MTZPN1                 :1; //9
      unsigned int MTZ1                      :1;//10
      unsigned int SECTOR_VPERED_MTZN2       :1;//11
      unsigned int SECTOR_NAZAD_MTZN2        :1;//12
      unsigned int PO_MTZ2                   :1;//13
      unsigned int PO_MTZN2_VPERED           :1;//14
      unsigned int PO_MTZN2_NAZAD            :1;//15
      unsigned int PO_U_MTZPN2               :1;//16
      unsigned int PO_MTZPN2                 :1;//17
      unsigned int MTZ2                      :1;//18
      unsigned int SECTOR_VPERED_MTZN3       :1;//19
      unsigned int SECTOR_NAZAD_MTZN3        :1;//20
      unsigned int PO_MTZ3                   :1;//21
      unsigned int PO_MTZN3_VPERED           :1;//22
      unsigned int PO_MTZN3_NAZAD            :1;//23
      unsigned int PO_U_MTZPN3               :1;//24
      unsigned int PO_MTZPN3                 :1;//25
      unsigned int MTZ3                      :1;//26
      unsigned int SECTOR_VPERED_MTZN4       :1;//27
      unsigned int SECTOR_NAZAD_MTZN4        :1;//28
      unsigned int PO_MTZ4                   :1;//29
      unsigned int PO_MTZN4_VPERED           :1;//30
      unsigned int PO_MTZN4_NAZAD            :1;//31    
                             
    } bool_val;
    unsigned long U32V;
} U32_cmd2_wrp; 


typedef union U32_cmd3_Unn{ 
   struct {
      unsigned int PO_U_MTZPN4         :1; //0
      unsigned int PO_MTZPN4           :1; //1
      unsigned int MTZ4                :1; //2
      unsigned int PO_BLOCK_U_MTZN     :1; //3
      unsigned int NCN_MTZ             :1; //4
      unsigned int BLOCK_MTZ04_1       :1; //5
      unsigned int BLOCK_MTZ04_2       :1; //6
      unsigned int BLOCK_USK_MTZ       :1; //7
      unsigned int PO_MTZ04_1          :1; //8
      unsigned int MTZ04_1             :1; //9
      unsigned int PO_MTZ04_2          :1;//10
      unsigned int MTZ04_2             :1;//11
      unsigned int BLOCK_ZDZ           :1;//12
      unsigned int PUSK_ZDZ_VID_       :1;//13
      unsigned int LIGHT_ZDZ_FRO       :1;//14
      unsigned int PO_ZDZ              :1;//15
      unsigned int ZDZ                 :1;//16
      unsigned int BLOCK_NZZ           :1;//17
      unsigned int PO_NZZ              :1;//18
      unsigned int NZZ                 :1;//19
      unsigned int PO_3I0              :1;//20
      unsigned int l3I0                 :1;//21
      unsigned int lPO_3U0              :1;//22
      unsigned int l3U0                 :1;//23
      unsigned int SECTOR_NZZ           :1;//24
      unsigned int BLOCK_TZNP1          :1;//25
      unsigned int SECTOR_TZNP1_VPERED  :1;//26
      unsigned int SECTOR_TZNP1_NAZAD   :1;//27
      unsigned int PO_3I0_TZNP1_VPERED  :1;//28
      unsigned int PO_3I0_TZNP1_NAZAD   :1;//29
      unsigned int PO_3U0_TZNP1_VPERED  :1;//30
      unsigned int PO_3U0_TZNP1_NAZAD   :1;//31    
  
    } bool_val;
    unsigned long U32V;
} U32_cmd3_wrp; 

typedef union U32_cmd4_Unn{ 
   struct {
      unsigned int PO_TZNP1_VPERED       :1; //0
      unsigned int PO_TZNP1_NAZAD        :1; //1
      unsigned int TZNP1                 :1; //2
      unsigned int BLOCK_TZNP2           :1; //3
      unsigned int SECTOR_TZNP2_VPERED   :1; //4
      unsigned int SECTOR_TZNP2_NAZAD    :1; //5
      unsigned int PO_3I0_TZNP2_VPERED   :1; //6
      unsigned int PO_3I0_TZNP2_NAZAD    :1; //7
      unsigned int PO_3U0_TZNP2_VPERED   :1; //8
      unsigned int PO_3U0_TZNP2_NAZAD    :1; //9
      unsigned int PO_TZNP2_VPERED       :1;//10
      unsigned int PO_TZNP2_NAZAD        :1;//11
      unsigned int TZNP2                 :1;//12
      unsigned int BLOCK_TZNP3           :1;//13
      unsigned int SECTOR_TZNP3_VPERED   :1;//14
      unsigned int SECTOR_TZNP3_NAZAD    :1;//15
      unsigned int PO_3I0_TZNP3_VPERED   :1;//16
      unsigned int PO_3I0_TZNP3_NAZAD    :1;//17
      unsigned int PO_3U0_TZNP3_VPERED   :1;//18
      unsigned int PO_3U0_TZNP3_NAZAD    :1;//19
      unsigned int PO_TZNP3_VPERED       :1;//20
      unsigned int PO_TZNP3_NAZAD        :1;//21
      unsigned int TZNP3                 :1;//22
      unsigned int STAT_BLK_APV          :1;//23
      unsigned int APV1                  :1;//24
      unsigned int APV2                  :1;//25
      unsigned int APV3                  :1;//26
      unsigned int APV4                  :1;//27
      unsigned int APV_WORK              :1;//28
      unsigned int ACHR_CHAPV_VID_DV     :1;//29
      unsigned int BLOCK_ACHR1           :1;//30
      unsigned int BLOCK_ACHR2  :1;//31    
  
    } bool_val;
    unsigned long U32V;
} U32_cmd4_wrp; 

typedef union U32_cmd5_Unn{ 
   struct {
      unsigned int RAZR_CHAPV           :1; //0
      unsigned int VN_BLOCK_CHAPV       :1; //1
      unsigned int PO_ACHR1             :1; //2
      unsigned int PO_CHAPV1            :1; //3
      unsigned int ACHR_CHAPV1          :1; //4
      unsigned int PO_ACHR2             :1; //5
      unsigned int PO_CHAPV2            :1; //6
      unsigned int ACHR_CHAPV2          :1; //7
      unsigned int PUSK_UROV_VID_DV     :1; //8
      unsigned int PO_UROV              :1; //9
      unsigned int UROV1                :1;//10
      unsigned int UROV2                :1;//11
      unsigned int BLOCK_ZOP            :1;//12
      unsigned int PO_ZOP               :1;//13
      unsigned int ZOP                  :1;//14
      unsigned int BLOCK_UMIN1          :1;//15
      unsigned int START_UMIN1          :1;//16
      unsigned int BLOCK_UMIN2          :1;//17
      unsigned int START_UMIN2          :1;//18
      unsigned int PO_UMIN1             :1;//19
      unsigned int PO_UBLK_UMIN1        :1;//20
      unsigned int PO_IBLK_UMIN1        :1;//21
      unsigned int UMIN1                :1;//22
      unsigned int PO_UMIN2             :1;//23
      unsigned int PO_UBLK_UMIN2        :1;//24
      unsigned int PO_IBLK_UMIN2        :1;//25
      unsigned int UMIN2                :1;//26
      unsigned int BLOCK_UMAX1          :1;//27
      unsigned int PO_UMAX1             :1;//28
      unsigned int UMAX1                :1;//29
      unsigned int BLOCK_UMAX2          :1;//30
      unsigned int PO_UMAX2             :1;//31    
  
    } bool_val;
    unsigned long U32V;
} U32_cmd5_wrp; 

typedef union U32_cmd6_Unn{ 
   struct {
      unsigned int UMAX2      :1; //0
      unsigned int BLOCK_UP1  :1; //1
      unsigned int PO_UP1     :1; //2
      unsigned int UP1        :1; //3
      unsigned int BLOCK_UP2  :1; //4
      unsigned int PO_UP2  :1; //5
      unsigned int UP2  :1; //6
      unsigned int BLOCK_UP3  :1; //7
      unsigned int PO_UP3  :1; //8
      unsigned int UP3  :1; //9
      unsigned int BLOCK_UP4  :1;//10
      unsigned int PO_UP4  :1;//11
      unsigned int UP4  :1;//12
      unsigned int BLOCK_UP5  :1;//13
      unsigned int PO_UP5  :1;//14
      unsigned int UP5  :1;//15
      unsigned int BLOCK_UP6  :1;//16
      unsigned int PO_UP6  :1;//17
      unsigned int UP6  :1;//18
      unsigned int BLOCK_UP7  :1;//19
      unsigned int PO_UP7  :1;//20
      unsigned int UP7  :1;//21
      unsigned int BLOCK_UP8  :1;//22
      unsigned int PO_UP8  :1;//23
      unsigned int UP8  :1;//24
      unsigned int DF1_IN   :1;//25
      unsigned int DF1_OUT  :1;//26
      unsigned int DF2_IN   :1;//27
      unsigned int DF2_OUT  :1;//28
      unsigned int DF3_IN   :1;//29
      unsigned int DF3_OUT  :1;//30
      unsigned int DF4_IN   :1;//31    
  
    } bool_val;
    unsigned long U32V;
} U32_cmd6_wrp; 

typedef union U32_cmd7_Unn{ 
   struct {
      unsigned int DF4_OUT      :1; //0
      unsigned int DF5_IN       :1; //1
      unsigned int DF5_OUT      :1; //2
      unsigned int DF6_IN       :1; //3
      unsigned int DF6_OUT      :1; //4
      unsigned int DF7_IN       :1; //5
      unsigned int DF7_OUT      :1; //6
      unsigned int DF8_IN       :1; //7
      unsigned int DF8_OUT      :1; //8
      unsigned int DT1_SET      :1; //9
      unsigned int DT1_RESET    :1;//10
      unsigned int DT1_OUT      :1;//11
      unsigned int DT2_SET      :1;//12
      unsigned int DT2_RESET    :1;//13
      unsigned int DT2_OUT      :1;//14
      unsigned int DT3_SET      :1;//15
      unsigned int DT3_RESET    :1;//16
      unsigned int DT3_OUT      :1;//17
      unsigned int DT4_SET      :1;//18
      unsigned int DT4_RESET    :1;//19
      unsigned int DT4_OUT      :1;//20
      unsigned int D_AND1       :1;//21
      unsigned int D_AND2       :1;//22
      unsigned int D_AND3       :1;//23
      unsigned int D_AND4       :1;//24
      unsigned int D_AND5       :1;//25
      unsigned int D_AND6       :1;//26
      unsigned int D_AND7       :1;//27
      unsigned int D_AND8       :1;//28
      unsigned int D_OR1        :1;//29
      unsigned int D_OR2        :1;//30
      unsigned int D_OR3        :1;//31    
  
    } bool_val;
    unsigned long U32V;
} U32_cmd7_wrp; 

typedef union U32_cmd8_Unn{ 
   struct {
      unsigned int D_OR4             :1; //0
      unsigned int D_OR5             :1; //1
      unsigned int D_OR6             :1; //2
      unsigned int D_OR7             :1; //3
      unsigned int D_OR8             :1; //4
      unsigned int D_XOR1            :1; //5
      unsigned int D_XOR2            :1; //6
      unsigned int D_XOR3            :1; //7
      unsigned int D_XOR4            :1; //8
      unsigned int D_XOR5            :1; //9
      unsigned int D_XOR6            :1;//10
      unsigned int D_XOR7            :1;//11
      unsigned int D_XOR8            :1;//12
      unsigned int D_NOT1            :1;//13
      unsigned int D_NOT2            :1;//14
      unsigned int D_NOT3            :1;//15
      unsigned int D_NOT4            :1;//16
      unsigned int D_NOT5            :1;//17
      unsigned int D_NOT6            :1;//18
      unsigned int D_NOT7            :1;//19
      unsigned int D_NOT8            :1;//20
      unsigned int D_NOT9            :1;//21
      unsigned int D_NOT10           :1;//22
      unsigned int D_NOT11           :1;//23
      unsigned int D_NOT12           :1;//24
      unsigned int D_NOT13           :1;//25
      unsigned int D_NOT14           :1;//26
      unsigned int D_NOT15           :1;//27
      unsigned int D_NOT16           :1;//28
      unsigned int ERROR_CONF_EL     :1;//29
      unsigned int b30    :1;//30
      unsigned int b31    :1;//31    
  
    } bool_val;
    unsigned long U32V;
} U32_cmd8_wrp; 

typedef struct tag_cmd_bit_depot{
   U32_cmd1_wrp w1;
   U32_cmd2_wrp w2;
   U32_cmd3_wrp w3;
   U32_cmd4_wrp w4;
   U32_cmd5_wrp w5;
   U32_cmd6_wrp w6;
   U32_cmd7_wrp	w7;
   U32_cmd8_wrp w8;
   U32_cmd8_wrp w9;
} CmdFunctionDepot;

extern CmdFunctionDepot* pDbgViewCmd;




#endif
