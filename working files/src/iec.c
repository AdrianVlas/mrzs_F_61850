//inline void main_protection(void)



typedef union net_in_cmd_area_Unn{ 
   
   struct {
      unsigned int out1 : 1; //0
      unsigned int out2 : 1; //1
      unsigned int out3 : 1; //2
      unsigned int out4 : 1; //3
      unsigned int out5 : 1; //4 
      unsigned int out6 : 1; //5 
      unsigned int out7 : 1; //6 
      unsigned int out8 : 1; //7    
    } bool_val;
    unsigned char U8_Vl;//long lVl;
} NetIn_stt; //
typedef union goose_block_Unn{ 
   unsigned char arCh[4];
   unsigned short arSh[2];
   unsigned long GS_Vl;
   struct{
        struct {
            unsigned int in1 : 1; //0
            unsigned int in2 : 1; //1
            unsigned int in3 : 1; //2
            unsigned int in4 : 1; //3
            unsigned int in5 : 1; //4 
            unsigned int in6 : 1; //5 
            unsigned int in7 : 1; //6 
            unsigned int in8 : 1; //7    
        } in_bool; //
    unsigned char Blk;
    struct {
        unsigned int out1 : 1; //0
        unsigned int out2 : 1; //1
        unsigned int out3 : 1; //2
        unsigned int out4 : 1; //3
        unsigned int out5 : 1; //4 
        unsigned int out6 : 1; //5 
        unsigned int out7 : 1; //6 
        unsigned int out8 : 1; //7    
    } out_bool; //unsigned char Out;
    unsigned char Res;
   }GS_Hld;
    
} GsBlock_stt; //
/*

typedef union mms_block_Unn{ 
   struct {
      unsigned int out1 : 1; //0
      unsigned int out2 : 1; //1
      unsigned int out3 : 1; //2
      unsigned int out4 : 1; //3
      unsigned int out5 : 1; //4 
      unsigned int out6 : 1; //5 
      unsigned int out7 : 1; //6 
      unsigned int out8 : 1; //7    
    } bool_val;
    unsigned char U8_Vl;//long lVl;
} MmsBlock_stt; //
*/
  typedef union mms_block_Unn{ 
    unsigned char arCh[4];
    unsigned short arSh[2];
    unsigned long Mms_Vl;
    struct{
        struct {
            unsigned int in1 : 1; //0
            unsigned int in2 : 1; //1
            unsigned int in3 : 1; //2
            unsigned int in4 : 1; //3
            unsigned int in5 : 1; //4 
            unsigned int in6 : 1; //5 
            unsigned int in7 : 1; //6 
            unsigned int in8 : 1; //7    
        } in_bool; //
    unsigned char Blk;
    struct {
        unsigned int out1 : 1; //0
        unsigned int out2 : 1; //1
        unsigned int out3 : 1; //2
        unsigned int out4 : 1; //3
        unsigned int out5 : 1; //4 
        unsigned int out6 : 1; //5 
        unsigned int out7 : 1; //6 
        unsigned int out8 : 1; //7    
    } out_bool; //unsigned char Out;
    unsigned char Res;
   }Mms_Hld;
    
} MmBlock_stt;
register unsigned long rU;
struct{
    char  ch_while_breaker;
    __SETTINGS *p_current_settings_prt;

    
}sLV;




//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//                  
//....................................................................................................
//=====================================================================================================
do{
    sLV.ch_while_breaker = 0;
// ----------------    -------------------------
//   if(_CHECK_SET_BIT(p_active_functions, RANG_OP_PR_DZ2) != 0)
//                p1.bool_vars.or2__i_1 = 1;
//active_functions 
//register unsigned int *p_af = sLV.p_active_functions;

   
}while(sLV.ch_while_breaker);
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````



//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//                  
//....................................................................................................
//=====================================================================================================
do{
// ----------------    -------------------------
  
    
}while();
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````




            
//=====================================================================================================
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//                  
//....................................................................................................
//=====================================================================================================
void iec_handler(unsigned int *p_active_functions, unsigned int number_group_stp){
// ----------------    -------------------------
  UNUSED(p_active_functions);
  UNUSED(number_group_stp);
    
}
//
//--------------------------------------------------------------------------------------------------------
//````````````````````````````````````````````````````````````````````````````````````````````````````````

/*

Тут буде одна «незручність».
Якщо ми будемо реалізовувати Вх.GOOSE як «віртуальні входи», то, я так розумію. 
треба буде і реалізувати існуючий функціонал, що команди «Ввімк.ВВ» і «Вимк.ВВ» 

можуть блокуватися від входів (поки тільки від дискретних) у місцевому режимі. 
а можуть не блокуватися.
Якщо цей функціонал перенести на Вх.GOOSE, то треба цей алгоритм реалізувати як 
для дискретних входів, а саме через активацію змінних локальних vvimk_VV_vid_DV 

і vymk_VV_vid_DV
GOOSE i MMS
Taras, 12:40
enum __rang_small

У мене діапазон в 16 команд. В загальному випадку він може розміщатись 
0-к-8біт-м.Тобто може бути 2-3 байти блокування і це треба врахувати.
Може бути к або 0 біт,адреса і 8 біт і адреса і м або 0 біт.
Тоб-то спочатку вираховую адресу початкового біту
Припустимо номер біту RANG_BLOCK_IN_GOOSE1.
 Індекс байтової адреси і = RANG_BLOCK_IN_GOOSE1>>3. 
Зміщення біту -- о = RANG_BLOCK_IN_GOOSE1 - 8*і;
Маска -- 

Вихід - Знайти індекс початку і_с = RANG_BLOCK_IN_GOOSE1.Індекс кінця
 і_е = (RANG_BLOCK_IN_GOOSE1+N_IN_GOOSE)>>3.
Скільки треба зчитати -- і_е - і_с;
Зміщення
#define  RANG_BLOCK_IN_GOOSE1_U8_MSK
            (1<<( (  RANG_BLOCK_IN_GOOSE1) - (8*( RANG_BLOCK_IN_GOOSE1 >>3)) )  )

Це початкове зміщення. Відповідно слово потрібно зсунути "вліво" на 
RANG_BLOCK_IN_GOOSE1_U8_MSK
І виділити 16 біт. Тобто потім старші 16 біт можна відкинути.
Якщо знайти 32 зміщення кінця, то відповідно можна вирахувати величину зсуву 
вправо.
#define  RESERV00_DI_EPRF_U32_MSK
             (1<<( ( RANG_BLOCK_IN_GOOSE1+N_IN_GOOSE )
             - (32*(  RANG_BLOCK_IN_GOOSE1+N_IN_GOOSE      >>5)) )  )

Зчитую по байтам в 32 слово. 

Що треба, щоб виділити 16 біт.  
далі копіюю 16 біт в 32 слово по байтам.

register union { 
   struct {
      unsigned int b00:1;//00.
        unsigned int b01:1;//01.
        unsigned int b02:1;//02.
        unsigned int b03:1;//03.
        unsigned int b04:1;//04.
        unsigned int b05:1;//05.
        unsigned int b06:1;//06.
        unsigned int b07:1;//07.
        unsigned int b08:1;//08.
        unsigned int b09:1;//09.
        unsigned int b10:1;//10.
        unsigned int b11:1;//11.
        unsigned int b12:1;//12.
        unsigned int b13:1;//13.
        unsigned int b14:1;//14.
        unsigned int b15:1;//15.
        unsigned int b16:1;//16.
        unsigned int b17:1;//17.
        unsigned int b18:1;//18.
        unsigned int b19:1;//19.
        unsigned int b20:1;//20.
        unsigned int b21:1;//21.
        unsigned int b22:1;//22.
        unsigned int b23:1;//23.
        unsigned int b24:1;//24.
        unsigned int b25:1;//25.
        unsigned int b26:1;//26.
        unsigned int b27:1;//27.
        unsigned int b28:1;//28.
        unsigned int b29:1;//29.
        unsigned int b30:1;//30.
        unsigned int b31:1;//31.

   } bool_vars;
  long lVl;
  unsigned char arCh[4];
  unsigned short arSh[2];
}wrp
*/
	//pv = (void*) sLV.p_active_functions;
	//--r_p_active_functions = sLV.p_active_functions;
	//--if(_CHECK_SET_BIT((unsigned int*)r_p_active_functions, RANG_BLOCK_IN_GOOSE1) != 0){
	//--//clr block
	//--}else{
	//--
	//--}
	//--if(_CHECK_SET_BIT((unsigned int*)r_p_active_functions, (RANG_BLOCK_IN_GOOSE1 + 1)) != 0){
	//--//clr block
	//--}else{
	//--
	//--}
	//--if(_CHECK_SET_BIT((unsigned int*)r_p_active_functions, (RANG_BLOCK_IN_GOOSE1 + 2)) != 0){
	//--//clr block
	//--}else{
	//--
	//--}
	//--if(_CHECK_SET_BIT((unsigned int*)r_p_active_functions, (RANG_BLOCK_IN_GOOSE1 + 3)) != 0){
	//--//clr block
	//--}else{
	//--
	//--}
	//--if(_CHECK_SET_BIT((unsigned int*)r_p_active_functions, (RANG_BLOCK_IN_GOOSE1 + 15)) != 0){
	//--//clr block
	//--}else{
	//--
	//--}
