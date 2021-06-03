

#include "header.h"
#include "cmd_log.h"
#include <string.h>
//.7.4 Реєстратор програмних подій
//.7.4.1 Реєстратор програмних подій повинен фіксувати та зберігати
//.інформацію з прив’язуванням до часу та дати про виниклі під час роботи пристрою
//.несправності та збої з фіксацією типу несправності та збою.
//.7.4.2 Пристрій повинен реєструвати всі перезапуски пристрою з
//.прив’язуванням до часу та дати.
//.7.5 Реєстратор статистики
//.7.5.1 Реєстратор дискретних сигналів повинен зберігати не менше 400
//.останніх записів.
//.45
//.7.5.2 Реєстратор статистики повинен фіксувати всі зміни всіх дискретних
//.сигналів приладу з прив’язуванням до часу та дати.
//.7.5.3 При введенні реєстатора статистики в прилад без модернізації схемно
//.конструктивних рішень допускається збереження реєтратора тільки в оперативній
//.пам’яті.
//.
//.MRZS-F2J1 (3 оптодавачі + 61850)
//.281 сигнал - це 36 байт інформації.
//.
//.8 байт - UnixTime мітка (з точністю до секунди)
//.4 байти - мікросекунди (у нас точність мілісекунд, тому число завжди буде кратне 1000) Хоча можна і 2 байти зробити для мілісекунд
//.
//.36 байт - стан дискретних сигналів.
//.
//.Сума 48 байт.
//.
//.48*400 = 19200
//.
//.400 записів + 1 один стартовий = 401 зрізи
//.401 х 48 = 19248 байт = 19 кБайт
void ChekCurrentStateCmd(unsigned int *p_active_functions){
    UNUSED(p_active_functions);
    
    
}
//.що треба 1)База актуальний стан сигналів
//.2) Постійно робиш XOR, як не 0, то заносиш дані
//.Пересовуєш вказівник вперед. 
//SRAM1 CmdPlusTimeHolder holderCmdPlusTime;
SRAM1  CmdPlusTimeHolder holderCmdPlusTime;

// @In the steps below I am using a timestamp of 1203161493 
// @which corresponds to a date/time of 2-15-2008 11:31:33. 
// @I'm ignoring timezones and whatnot, it isn't necessary for this.

/*SRAM1 */ CmdPlusTimeStampHolder holderCmdPlusTimeStamp;



short GetAmountFixElem(void){
    return holderCmdPlusTime.shTotalFixElem;
}
void CmdPlusTimeLogHundler(unsigned int *p_active_functions){
    register long i;
    UNUSED(p_active_functions);
    
    if(holderCmdPlusTimeStamp.uLLDrecTimeStampVal >= (FIFTY_YEAR_NUMBER))
        holderCmdPlusTimeStamp.uLLDrecTimeStampVal = 0;
    else
        holderCmdPlusTimeStamp.uLLDrecTimeStampVal++;
    
    if(time_dat < 1203161493 )//&& time_ms == 0 
        return;
        
    i = holderCmdPlusTime.shIndexWR;
    if (i == 0)
        i = AMOUNT_CMD_PLUS_TIME_RECORD;
    else 
        i--;
    unsigned long *pCmd = (unsigned long *)&(holderCmdPlusTime.arrCmdPlusTimeHolder[i].cmd.uLCmd[0]);
    bool b_comp = false;       
    for (size_t _i = 0; ( (b_comp == false) && (_i < sizeof(N_BIG)) ); ++_i) 
    {                                                          
        b_comp |= (p_active_functions[_i] != pCmd[_i]);               
    }
    if( b_comp
        //?((p_active_functions[0] != pCmd[0])  )||
        //?((p_active_functions[1] != pCmd[1])  )||
        //?((p_active_functions[2] != pCmd[2])  )||
        //?((p_active_functions[3] != pCmd[3])  )||
        //?((p_active_functions[4] != pCmd[4])  )||
        //?((p_active_functions[5] != pCmd[5])  )||
        //?((p_active_functions[6] != pCmd[6])  )||
        //?((p_active_functions[7] != pCmd[7])  )||
        //?((p_active_functions[8] != pCmd[8])  )
    ){
        i = holderCmdPlusTime.shIndexWR;
        pCmd = (unsigned long *)&(holderCmdPlusTime.arrCmdPlusTimeHolder[i].cmd.uLCmd[0]);
        //?pCmd[0] = p_active_functions[0] ;
        //?pCmd[1] = p_active_functions[1] ;
        //?pCmd[2] = p_active_functions[2] ;
        //?pCmd[3] = p_active_functions[3] ;
        //?pCmd[4] = p_active_functions[4] ;
        //?pCmd[5] = p_active_functions[5] ;
        //?pCmd[6] = p_active_functions[6] ;
        //?pCmd[7] = p_active_functions[7] ;
        //?pCmd[8] = p_active_functions[8] ;
        memcpy((void *)pCmd,(const void*)p_active_functions,sizeof(UNN_CmdState));
        //Put Time
        holderCmdPlusTime.arrCmdPlusTimeHolder[i].unix_time.time_dat = time_dat;
        //Put Mksec
        holderCmdPlusTime.arrCmdPlusTimeHolder[i].mksec.time_ms      = time_ms;
        //i = CmdPlusTimeHolder.shIndexWR;
        i++;
        if( i >= AMOUNT_CMD_PLUS_TIME_RECORD)
            holderCmdPlusTime.shIndexWR = 0;
        else
            holderCmdPlusTime.shIndexWR = i;
        if (holderCmdPlusTime.shTotalFixElem < AMOUNT_CMD_PLUS_TIME_RECORD)
            holderCmdPlusTime.shTotalFixElem++;
		if (holderCmdPlusTime.u32IDModifyIndexWR < 0xffffffff)
            holderCmdPlusTime.u32IDModifyIndexWR++;
		
    }
}

void GetCmdPlusTimeLogElem(unsigned int *p_elem, long lIdx){
    register long i;
    //?UNUSED(p_active_functions);
    if(lIdx > AMOUNT_CMD_PLUS_TIME_RECORD || lIdx < 0)
        lIdx = 0;
    i = holderCmdPlusTime.shIndexWR-1;
    i -= lIdx;
    
    if (i < 0)
        i += AMOUNT_CMD_PLUS_TIME_RECORD;
//void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
    unsigned long *pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[i].cmd.uLCmd[0]);
    memcpy((void *)p_elem,(const void*)pU32,  sizeof(UNN_CmdState));//for avoid situation when change data while reading
    bool data_not_Ok = 0;char u8_iter = 0;
    bool b_comp = false;       
    for (size_t _i = 0; ( (b_comp == false) && (_i < sizeof(N_BIG)) ); ++_i) 
    {                                                          
        b_comp |= (pU32[_i] != p_elem[_i]);               
    }
    do{
        if( b_comp
            //.((p_elem[0] != pU32[0])) ||
            //.((p_elem[1] != pU32[1])) ||
            //.((p_elem[2] != pU32[2])) ||
            //.((p_elem[3] != pU32[3])) ||
            //.((p_elem[4] != pU32[4])) ||
            //.((p_elem[5] != pU32[5])) ||
            //.((p_elem[6] != pU32[6])) ||
            //.((p_elem[7] != pU32[7])) ||
            //.((p_elem[8] != pU32[8])) 
           )
        {
            memcpy((void *)p_elem,(const void*)pU32,sizeof(UNN_CmdState));   
            data_not_Ok = 1;
            u8_iter++;
            //read again
        }   
            
    }while (data_not_Ok && (u8_iter<3));

}
//?inline void GetCmdPlusTimeLogElemUseLocal(unsigned int *p_elem, long lIdx){
//?    register long i;
//?    //?UNUSED(p_active_functions);
//?    i = holderCmdPlusTime.shIndexWR-1;
//?    i -= lIdx;
//?    
//?    if (i < 0)
//?        i += AMOUNT_CMD_PLUS_TIME_RECORD;
//?//void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
//? 
//?    memcpy((void *)p_elem,(const void*)&(holderCmdPlusTime.arrCmdPlusTimeHolder[i].cmd.uLCmd[0]),
//?    sizeof(UNN_CmdState));
//?
//?}

void GetDateTimeLogElem(unsigned int *p_elem, long lIdx){
    register long i;
    //?UNUSED(p_active_functions);
    if(lIdx > AMOUNT_CMD_PLUS_TIME_RECORD || lIdx < 0)
        lIdx = 0;
    i = holderCmdPlusTime.shIndexWR-1;
    i -= lIdx;
    
    if (i < 0)
        i += AMOUNT_CMD_PLUS_TIME_RECORD;
//void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
    unsigned long *pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[i].unix_time.arU32MkTime[0]);
    memcpy((void *)p_elem,(const void*)pU32,    sizeof(UNN_UnixTime));
    bool data_not_Ok = 0;char u8_iter = 0;
    
    do{
        bool b_comp = false;       
        for (size_t _i = 0; ( (b_comp == false) && (_i < (sizeof(time_t)>>2)) ); ++_i) 
        {                                                          
            b_comp |= (pU32[_i] != p_elem[_i]);               
        }                                                     
        if( b_comp 
        )
        {
            memcpy((void *)p_elem,(const void*)pU32,sizeof(UNN_CmdState));   
            data_not_Ok = 1;
            u8_iter++;//read again
        }   
    }while (data_not_Ok && (u8_iter<3));
}


    
void GetMsLogElem(unsigned int *p_elem, long lIdx){
    register long i;
    //?UNUSED(p_active_functions);
    if(lIdx > AMOUNT_CMD_PLUS_TIME_RECORD || lIdx < 0)
        lIdx = 0;
    i = holderCmdPlusTime.shIndexWR-1;
    i -= lIdx;
    
    if (i < 0)
        i += AMOUNT_CMD_PLUS_TIME_RECORD;
//void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
 
    unsigned long *pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[i].mksec.uLMkTime);
    memcpy((void *)p_elem,(const void*)pU32,    sizeof(UNN_MicroSec));
    bool data_not_Ok = 0;char u8_iter = 0;
                                                         
    do{
        if( pU32[0] != p_elem[0]
        )
        {
            memcpy((void *)p_elem,(const void*)pU32,sizeof(UNN_MicroSec));   
            data_not_Ok = 1;
            u8_iter++;//read again
        }   
    }while (data_not_Ok && (u8_iter<3));

}

long GetNumberChangingInLogElem( long lIdx){
    register long i;
    //?UNUSED(p_active_functions);           Errorneus code as this
    //?i = holderCmdPlusTime.shIndexWR-1;    ariphmetic make in lowfunc
    //?i -= lIdx;
    //?
    //?if (i < 0)
    //?    i += AMOUNT_CMD_PLUS_TIME_RECORD;
    if(lIdx > AMOUNT_CMD_PLUS_TIME_RECORD || lIdx < 0)
        lIdx = 0;
    unsigned int array_old[N_BIG], array_new[N_BIG], array_changing[N_BIG];
    GetCmdPlusTimeLogElem(array_new ,lIdx);
    //i--;                                     Errorneus code as this
    //if (i < 0)                               ariphmetic make in lowfunc
    //    i += AMOUNT_CMD_PLUS_TIME_RECORD;
    lIdx++;//Next elem on menu older
    if (lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD)
        lIdx -= AMOUNT_CMD_PLUS_TIME_RECORD;
    GetCmdPlusTimeLogElem(array_old ,lIdx);
    
     for (unsigned int j = 0; j < N_BIG; j++)
         array_changing[j] = array_new[j] ^ array_old[j];
long u32Count = 0;
    for (unsigned int j = 0; j < N_BIG; j++){    
        for(i = 0; i < 32; i++)
            if((array_changing[j] &(1<<i)) != 0)
                u32Count++;
        
    }
            
return u32Count;

}
//?long GetNumberChangingInLogElemUseLocal( long lIdx){
//?    register long i;
//?    //?UNUSED(p_active_functions);           Errorneus code as this
//?    //?i = holderCmdPlusTime.shIndexWR-1;    ariphmetic make in lowfunc
//?    //?i -= lIdx;
//?    //?
//?    //?if (i < 0)
//?    //?    i += AMOUNT_CMD_PLUS_TIME_RECORD;
//?    
//?    unsigned int array_old[N_BIG], array_new[N_BIG], array_changing[N_BIG];
//?    GetCmdPlusTimeLogElemUseLocal(array_new ,lIdx);
//?    //i--;                                     Errorneus code as this
//?    //if (i < 0)                               ariphmetic make in lowfunc
//?    //    i += AMOUNT_CMD_PLUS_TIME_RECORD;
//?    lIdx++;//Next elem on menu older
//?    if (lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD)
//?        lIdx -= AMOUNT_CMD_PLUS_TIME_RECORD;
//?    GetCmdPlusTimeLogElemUseLocal(array_old ,lIdx);
//?    
//?     for (unsigned int j = 0; j < N_BIG; j++)
//?         array_changing[j] = array_new[j] ^ array_old[j];
//?long u32Count = 0;
//?    for (unsigned int j = 0; j < N_BIG; j++){    
//?        for(i = 0; i < 32; i++)
//?            if((array_changing[j] &(1<<i)) != 0)
//?                u32Count++;
//?        
//?    }
//?            
//?return u32Count;
//?
//?}

void CmdPlusTimeStampLogHundler(unsigned int *p_active_functions){

#ifdef __DBG_CODE_  
//Записуємо попередній cтан сигналів перед аварією

#endif    
    register long i;
    register CmdPlusTimeStampHolder *phld  = &holderCmdPlusTimeStamp;
    UNUSED(p_active_functions);
    if(time_dat < 1203161493 )//&& time_ms == 0 
        return;
    i = holderCmdPlusTimeStamp.shIndexWR;
    if (i == 0)
        i = AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
    
        i--;
    
    unsigned long *pCmd = (unsigned long *)&(holderCmdPlusTimeStamp.arrCmdPlusTimeStampElem[i].cmd.uLCmd[0]);    
    bool b_comp = false;       
    for (size_t _i = 0; ( (b_comp == false) && (_i < sizeof(N_BIG)) ); ++_i) 
    {                                                          
        b_comp |= (p_active_functions[_i] != pCmd[_i]);               
    }
    if( b_comp
        //?((p_active_functions[0] != pCmd[0])  )||
        //?((p_active_functions[1] != pCmd[1])  )||
        //?((p_active_functions[2] != pCmd[2])  )||
        //?((p_active_functions[3] != pCmd[3])  )||
        //?((p_active_functions[4] != pCmd[4])  )||
        //?((p_active_functions[5] != pCmd[5])  )||
        //?((p_active_functions[6] != pCmd[6])  )||
        //?((p_active_functions[7] != pCmd[7])  )||
        //?((p_active_functions[8] != pCmd[8])  )
    ){

        long long lLCurrentDifer  = (phld->arrAdditionalInfoCmdPlusTimeStamp[i].ullTimeStamp);
        if( (phld->uLLDrecTimeStampVal) < (unsigned long long)lLCurrentDifer ){
            lLCurrentDifer = (FIFTY_YEAR_NUMBER - lLCurrentDifer) + (phld->uLLDrecTimeStampVal);
        }
        else{
            lLCurrentDifer = (phld->uLLDrecTimeStampVal) - lLCurrentDifer;
        }
               //asm volatile(
               //"bkpt 1"
               //);
        if(lLCurrentDifer < 100){
            phld->shSumDifersElem += lLCurrentDifer;
            long u,k;
            u = phld->shIndexRD;
            do{
                k = phld->uLLDrecTimeStampVal -
                    (phld->arrAdditionalInfoCmdPlusTimeStamp[u].ullTimeStamp);
                if(k <= 100)    
                    break;
                if( ++u >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD)   
                        u = 0;  
            }while ( k > 100);
            
            phld->shIndexRD = u;
            
            if(phld->shSumDifersElem > 100){
                long IdxTimeStamp,j = 0;
                long s = 0;
                if( i < (phld->shAmount100msElem) ){//Idx last elem in 100 ms range
                    j = phld->shAmount100msElem - i;
                    IdxTimeStamp = AMOUNT_CMD_PLUS_TIME_STAMP_RECORD - j;
                }else
                    IdxTimeStamp = i - phld->shAmount100msElem;
                
                //Delete Sum Val which less lLCurrentDifer
                do{
                    s = phld->uLLDrecTimeStampVal -
                    (phld->arrAdditionalInfoCmdPlusTimeStamp[IdxTimeStamp].ullTimeStamp);
                    
                    phld->shAmount100msElem--;
                    if( ++IdxTimeStamp >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD)   
                        IdxTimeStamp = 0;                                    
                }while( (s >= 100)&& (phld->shAmount100msElem > 0) );
                phld->shSumDifersElem = s;
                
            }
            else
                phld->shAmount100msElem++;
        }
        else{
            phld->shAmount100msElem = 0;
            phld->shSumDifersElem = 0;
            phld->shIndexRD = phld->shIndexWR;//..phld->shIndexRD = i;
        }
        //Вираховуємо кількість змін сигналів
          unsigned int number_changes_into_current_item;
        _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY(pCmd, p_active_functions, N_BIG, number_changes_into_current_item);
        
        i = holderCmdPlusTimeStamp.shIndexWR;
        pCmd = (unsigned long *)&(holderCmdPlusTimeStamp.arrCmdPlusTimeStampElem[i].cmd.uLCmd[0]);
        //pCmd[0] = p_active_functions[0] ;
        //pCmd[1] = p_active_functions[1] ;
        //pCmd[2] = p_active_functions[2] ;
        //pCmd[3] = p_active_functions[3] ;
        //pCmd[4] = p_active_functions[4] ;
        //pCmd[5] = p_active_functions[5] ;
        //pCmd[6] = p_active_functions[6] ;
        //pCmd[7] = p_active_functions[7] ;
        //pCmd[8] = p_active_functions[8] ;
        memcpy((void *)pCmd,(const void*)p_active_functions,sizeof(UNN_CmdState));
        //Put Time
        phld->arrCmdPlusTimeStampElem[i].unix_time.time_dat = time_dat;
        //Put Mksec
        phld->arrCmdPlusTimeStampElem[i].mksec.time_ms      = time_ms;
        //i = CmdPlusTimeHolder.shIndexWR;
        (phld->arrAdditionalInfoCmdPlusTimeStamp[i].ullTimeStamp) = (phld->uLLDrecTimeStampVal);
        (phld->arrAdditionalInfoCmdPlusTimeStamp[i].lNumAlterSigInElem) = number_changes_into_current_item; 
        i++;
        if( i >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD)
            phld->shIndexWR = 0;
        else
            phld->shIndexWR = i;
        if (phld->shTotalFixElem < AMOUNT_CMD_PLUS_TIME_STAMP_RECORD)
            phld->shTotalFixElem++;
        
        //.if( phld->shAmount100msElem == 0 )
        //.    phld->shAmount100msElem += 1;
    }
    
}

//extern unsigned int previous_active_functions[N_BIG];
//extern unsigned int number_items_dr; <- Кількість зрізів
//extern unsigned int number_changes_into_dr_record;
//extern unsigned int time_from_start_record_dr;
extern long tstBrrFooSelector;

void TestStateNameSpaceFooBrrr(DigRegUniqVarsAddreses *ptDRecUniqVarsAddreses ) {
    if(tstBrrFooSelector == 0)
        tstBrrFooSelector++;
    else{
        long i,j,k,l;
        i = *ptDRecUniqVarsAddreses->number_items_dr;
        j = *ptDRecUniqVarsAddreses->number_changes_into_dr_record;
        k = *ptDRecUniqVarsAddreses->time_from_start_record_dr;
        l = *ptDRecUniqVarsAddreses->blocking_continue_monitoring_min_U;
        
        tstBrrFooSelector = i+j+k+l;
    }
}

void put_before_info_in_buf (DigRegUniqVarsAddreses *ptDRUVAd){

    //register void* pv;
static  int time_before_start_record_dr = 0;

    //Записуємо попередній cтан сигналів перед аварією
    //Вираховуємо кількість змін сигналів
//    *(ptDRUVAd->number_changes_into_dr_record) = 0;
    //.   asm volatile(
    //.           "bkpt 1"
    //.           );      
    register long i;
    register CmdPlusTimeStampHolder *phld  = &holderCmdPlusTimeStamp;
    char chAmontRecords = 0;//One prev state
//!@!    chAmontRecords += phld->shAmount100msElem ;//1 - add previos elem
    i = phld->shIndexWR;
    if( i > phld->shIndexRD)
        chAmontRecords += i - phld->shIndexRD;
    else 
        chAmontRecords += (AMOUNT_CMD_PLUS_TIME_STAMP_RECORD - phld->shIndexRD)+ phld->shIndexWR;
    
    if(chAmontRecords > 12)
        chAmontRecords = 12;//threeshould or border -- result desigion between I, Taras, MB 
    //i = phld->shIndexWR;
    //i -= phld->shAmount100msElem + 2;//point first elem in 100ms buf;
    //Popint to previos before first elem in 100ms buf; 2 = 1+1 fierst 1 it shIndexWR second  1 it prev position
    //.if( i < 0 ) 
    //.    i += AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
    
    
    i = phld->shIndexRD;
    
    unsigned long *pCmd;
//D if(*(ptDRUVAd->number_items_dr) > 0 )
//D     *(ptDRUVAd->number_items_dr) += 1;
     *(ptDRUVAd->number_changes_into_dr_record) = 0;
    *(ptDRUVAd->number_items_dr) = 0; 

    long j = i - 1;//point prev 
        if (j < 0)
            j += AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;
    do{
        pCmd = (unsigned long *)&(holderCmdPlusTimeStamp.arrCmdPlusTimeStampElem[j].cmd.uLCmd[0]);
        long idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;
        
        time_before_start_record_dr = phld->uLLDrecTimeStampVal
          - (phld->arrAdditionalInfoCmdPlusTimeStamp[j].ullTimeStamp);
        //.time_before_start_record_dr *= (-1); <- very bad reaction Taras Sys, system can`t view negative numbers
        if(*(ptDRUVAd->number_items_dr) != 0)
            time_before_start_record_dr *= (-1);//1;  
          //unsigned int number_changes_into_current_item;
          //_NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY(((unsigned long *)&(holderCmdPlusTimeStamp.arrCmdPlusTimeStampElem[j].cmd.uLCmd[0])), pCmd, N_BIG, number_changes_into_current_item);
          //number_changes_into_dr_record += number_changes_into_current_item;
        if(*(ptDRUVAd->number_items_dr) != 0)
         *(ptDRUVAd->number_changes_into_dr_record) += phld->arrAdditionalInfoCmdPlusTimeStamp[j].lNumAlterSigInElem;
          //Записуємо текучий cтан сигналів
          //Мітка часу
          //buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  0] =  time_before_start_record_dr        & 0xff;
          //buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  1] = (time_before_start_record_dr >> 8 ) & 0xff;
          //buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  2] = (time_before_start_record_dr >> 16) & 0xff;
        if(*(ptDRUVAd->number_items_dr) != 0){
            buffer_for_save_dr_record[idx_data_dr +  0] =  time_before_start_record_dr        & 0xff;
            buffer_for_save_dr_record[idx_data_dr +  1] = (time_before_start_record_dr >> 8 ) & 0xff;
            buffer_for_save_dr_record[idx_data_dr +  2] = (time_before_start_record_dr >> 16) & 0xff;//(3-0)
        }else{
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  0] = 0xff;
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  1] = 0xff;
            buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  2] = 0xff;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_BEFORE_ITEMS_DR] = (time_before_start_record_dr );
        }
          //Текучий стан сигналів
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  3] =  pCmd[0]        & 0xff;
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  4] = (pCmd[0] >> 8 ) & 0xff;
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  5] = (pCmd[0] >> 16) & 0xff;
          
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 32] = (pCmd[7] >> 8 ) & 0xff;
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 33] = (pCmd[7] >> 16) & 0xff;
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 34] = (pCmd[7] >> 24) & 0xff;
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 35] =  pCmd[8]        & 0xff;
        memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&pCmd[0],(NUMBER_BYTES_SAMPLE_DR));//.(36-3)                   
          
          //Кількість змін сигналів у порівнянні із попереднім станом
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 36] = number_changes_into_current_item & 0xff;
          //.buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 37] = (number_changes_into_current_item >> 8) & 0xff;
        if(*(ptDRUVAd->number_items_dr) != 0){
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] = 
            (phld->arrAdditionalInfoCmdPlusTimeStamp[j].lNumAlterSigInElem) & 0xff;
            
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] = 
            ((phld->arrAdditionalInfoCmdPlusTimeStamp[j].lNumAlterSigInElem)>> 8) & 0xff;
        }
        else{
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] =  0;
            buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] =  0;
        }
        //Збільшуємо на один кількість нових зрізів
        *(ptDRUVAd->number_items_dr) += 1;
        if( ++j >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD )//if( ++i >= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD ) 
            j -= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;   //    i -= AMOUNT_CMD_PLUS_TIME_STAMP_RECORD;  
    }while( --chAmontRecords > 0  );
    
    //Записуємо попередній cтан сигналів перед аварією
       long idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;//.?38
//.    //Мітка часу попереднього стану сигналів до моменту початку запису
//.    buffer_for_save_dr_record[idx_data_dr +  0] = 0xff;
//.    buffer_for_save_dr_record[idx_data_dr +  1] = 0xff;
//.    buffer_for_save_dr_record[idx_data_dr +  2] = 0xff;
//.    //Попередній стан  
//.    memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&((ptDRUVAd->previous_active_functions)[0]),(36-3));   
//.    //Нулем позначаємо у цій позиції кількість змін
//.    buffer_for_save_dr_record[idx_data_dr + 36] = 0;
//.    buffer_for_save_dr_record[idx_data_dr + 37] = 0;
//.    *(ptDRUVAd->number_items_dr)++;

    //      //Вираховуємо кількість змін сигналів
    //      number_changes_into_dr_record = 0;
    //      unsigned int number_changes_into_current_item;
    //      _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY(previous_active_functions, p_active_functions, N_BIG, number_changes_into_current_item);
    //      number_changes_into_dr_record += number_changes_into_current_item;
    
    //pCmd = (unsigned long *)(ptDRUVAd->p_active_functions);//unsigned int *pI32 = (ptDRUVAd->previous_active_functions);
    unsigned long *pI32 = (unsigned long *)(ptDRUVAd->p_active_functions);
    *(ptDRUVAd->time_from_start_record_dr) = 0;
    
    unsigned int number_changes_into_current_item;  
    _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY( (pCmd), (pI32), N_BIG, number_changes_into_current_item);
    //Записуємо текучий cтан сигналів
     idx_data_dr =  FIRST_INDEX_FIRST_DATA_DR + (*(ptDRUVAd->number_items_dr))*SD_DR;
    //      //Мітка часу
    //      buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  0] =  time_from_start_record_dr        & 0xff;
    //      buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  1] = (time_from_start_record_dr >> 8 ) & 0xff;
    //      buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  2] = (time_from_start_record_dr >> 16) & 0xff;
    buffer_for_save_dr_record[idx_data_dr +  0] =  *(ptDRUVAd->time_from_start_record_dr)        & 0xff;
    buffer_for_save_dr_record[idx_data_dr +  1] = (*(ptDRUVAd->time_from_start_record_dr) >> 8 ) & 0xff;
    buffer_for_save_dr_record[idx_data_dr +  2] = (*(ptDRUVAd->time_from_start_record_dr) >> 16) & 0xff;//(3-0)

    //      //Текучий стан сигналів
    //      buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 +  3] =  p_active_functions[0]        & 0xff;
    //      buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 35] =  p_active_functions[8]        & 0xff;
    memcpy((void*)&buffer_for_save_dr_record[idx_data_dr +  3],(const void*)&((pI32)[0]),(NUMBER_BYTES_SAMPLE_DR));//?(36-3)
    
    //      
    //      //Кількість змін сигналів у порівнянні із попереднім станом
    //      buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 36] = number_changes_into_current_item & 0xff;
    //      buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*38 + 37] = (number_changes_into_current_item >> 8) & 0xff;
    buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 0)] = number_changes_into_current_item & 0xff;
    buffer_for_save_dr_record[idx_data_dr + (3 + NUMBER_BYTES_SAMPLE_DR + 1)] = (number_changes_into_current_item >> 8) & 0xff;
    //*(ptDRUVAd->number_items_dr) += 1;
    *(ptDRUVAd->number_changes_into_dr_record) += number_changes_into_current_item;// <<= як бути з доаварійним масивом

} 
     
 __root  CmdFunctionDepot* pDbgViewCmd = (CmdFunctionDepot*)&holderCmdPlusTimeStamp.arrCmdPlusTimeStampElem[0];
SRAM1  CmdFunctionDepot arViewCmd;// = holderCmdPlusTimeStamp.arrCmdPlusTimeStampElem[0];
long GetCmdPlusTimeLogElemPlWnum(unsigned int *p_elem, long lIdx,unsigned long ulWorkNumber){
    register long i,d;
    //?UNUSED(p_active_functions);
    if(lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD || lIdx < 0)
        return 1;//Invalid  lIdx
    i = holderCmdPlusTime.shIndexWR-1;
    i -= lIdx;
    if (i < 0)
        i += AMOUNT_CMD_PLUS_TIME_RECORD;
    
    if(ulWorkNumber != 0xffffffff){
        if(ulWorkNumber > holderCmdPlusTime.u32IDModifyIndexWR){
            d = ulWorkNumber - holderCmdPlusTime.u32IDModifyIndexWR;
            if(d >= AMOUNT_CMD_PLUS_TIME_RECORD)
                return 2;//Invalid  lWorkNumber
            i -= AMOUNT_CMD_PLUS_TIME_RECORD - d;//lWorkNumber + holderCmdPlusTime.lIDModifyIndexWR;
        }else if(ulWorkNumber < holderCmdPlusTime.u32IDModifyIndexWR){
            d = holderCmdPlusTime.u32IDModifyIndexWR - ulWorkNumber;
            if(d >= AMOUNT_CMD_PLUS_TIME_RECORD)
                return 2;//Invalid  lWorkNumber
            i -= d;
        }           
        
    
    }
    if (i <= (-AMOUNT_CMD_PLUS_TIME_RECORD))
        return 3;//Invalid  sum lWorkNumber& lIdx
    else if (i < 0)
             i += AMOUNT_CMD_PLUS_TIME_RECORD;
    
    

    unsigned long *pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[i].cmd.uLCmd[0]);
    memcpy((void *)p_elem,(const void*)pU32,  sizeof(UNN_CmdState));//for avoid situation when change data while reading
    bool data_not_Ok = 0;char u8_iter = 0;
    bool b_comp = false;       
    for (size_t _i = 0; ( (b_comp == false) && (_i < sizeof(N_BIG)) ); ++_i) 
    {                                                          
        b_comp |= (pU32[_i] != p_elem[_i]);               
    }
    do{
        if( b_comp

           )
        {
            memcpy((void *)p_elem,(const void*)pU32,sizeof(UNN_CmdState));   
            data_not_Ok = 1;
            u8_iter++;
            //read again
        }   
            
    }while (data_not_Ok && (u8_iter<3));
    p_elem[sizeof(UNN_CmdState)] = holderCmdPlusTime.u32IDModifyIndexWR;
    return 0;
}

long GetDateTimeLogElemPlWnum(unsigned int *p_elem, long lIdx,unsigned long ulWorkNumber){
    register long i,d;
    //?UNUSED(p_active_functions);
    if(lIdx >= AMOUNT_CMD_PLUS_TIME_RECORD || lIdx < 0)
        return 1;//Invalid  lIdx
    
    i = holderCmdPlusTime.shIndexWR-1;
    i -= lIdx;
    
    if (i < 0)
        i += AMOUNT_CMD_PLUS_TIME_RECORD;
    
    
    if(ulWorkNumber != 0xffffffff){
        if(ulWorkNumber > holderCmdPlusTime.u32IDModifyIndexWR){
            d = ulWorkNumber - holderCmdPlusTime.u32IDModifyIndexWR;
            if(d >= AMOUNT_CMD_PLUS_TIME_RECORD)
                return 2;//Invalid  lWorkNumber
            i -= AMOUNT_CMD_PLUS_TIME_RECORD - d;//lWorkNumber + holderCmdPlusTime.lIDModifyIndexWR;
        }else if(ulWorkNumber < holderCmdPlusTime.u32IDModifyIndexWR){
            d = holderCmdPlusTime.u32IDModifyIndexWR - ulWorkNumber;
            if(d >= AMOUNT_CMD_PLUS_TIME_RECORD)
                return 2;//Invalid  lWorkNumber
            i -= d;
        }           
    }    
    if (i <= (-AMOUNT_CMD_PLUS_TIME_RECORD))
        return 3;//Invalid  sum lWorkNumber& lIdx
    else if (i < 0)
             i += AMOUNT_CMD_PLUS_TIME_RECORD;    
    
    unsigned long *pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[i].unix_time.arU32MkTime[0]);
    memcpy((void *)p_elem,(const void*)pU32,    sizeof(UNN_UnixTime));
    bool data_not_Ok = 0;char u8_iter = 0;
    
    do{
        bool b_comp = false;       
        for (size_t _i = 0; ( (b_comp == false) && (_i < (sizeof(time_t)>>2)) ); ++_i) 
        {                                                          
            b_comp |= (pU32[_i] != p_elem[_i]);               
        }                                                     
        if( b_comp 
        )
        {
            memcpy((void *)p_elem,(const void*)pU32,sizeof(UNN_CmdState));   
            data_not_Ok = 1;
            u8_iter++;//read again
        }   
    }while (data_not_Ok && (u8_iter<3));
    p_elem[sizeof(UNN_CmdState)] = holderCmdPlusTime.u32IDModifyIndexWR;
    return holderCmdPlusTime.u32IDModifyIndexWR;
}


    
long GetMsLogElemPlWnum(unsigned int *p_elem, long lIdx,unsigned long ulWorkNumber){
    register long i,d;
    //?UNUSED(p_active_functions);
    if(lIdx > AMOUNT_CMD_PLUS_TIME_RECORD || lIdx < 0)
        return 1;//Invalid  lIdx
    i = holderCmdPlusTime.shIndexWR-1;
    i -= lIdx;
    
    if (i < 0)
        i += AMOUNT_CMD_PLUS_TIME_RECORD;
    
    if(ulWorkNumber != 0xffffffff){
        if(ulWorkNumber > holderCmdPlusTime.u32IDModifyIndexWR){
            d = ulWorkNumber - holderCmdPlusTime.u32IDModifyIndexWR;
            if(d >= AMOUNT_CMD_PLUS_TIME_RECORD)
                return 2;//Invalid  lWorkNumber
            i -= AMOUNT_CMD_PLUS_TIME_RECORD - d;//lWorkNumber + holderCmdPlusTime.lIDModifyIndexWR;
        }else if(ulWorkNumber < holderCmdPlusTime.u32IDModifyIndexWR){
            d = holderCmdPlusTime.u32IDModifyIndexWR - ulWorkNumber;
            if(d >= AMOUNT_CMD_PLUS_TIME_RECORD)
                return 2;//Invalid  lWorkNumber
            i -= d;
        }           
        
    
    }    
    if (i <= (-AMOUNT_CMD_PLUS_TIME_RECORD))
        return 3;//Invalid  sum lWorkNumber& lIdx
    else if (i < 0)
             i += AMOUNT_CMD_PLUS_TIME_RECORD;
    
 
    unsigned long *pU32 = &(holderCmdPlusTime.arrCmdPlusTimeHolder[i].mksec.uLMkTime);
    memcpy((void *)p_elem,(const void*)pU32,    sizeof(UNN_MicroSec));
    bool data_not_Ok = 0;char u8_iter = 0;
                                                         
    do{
        if( pU32[0] != p_elem[0]
        )
        {
            memcpy((void *)p_elem,(const void*)pU32,sizeof(UNN_MicroSec));   
            data_not_Ok = 1;
            u8_iter++;//read again
        }   
    }while (data_not_Ok && (u8_iter<3));
    p_elem[sizeof(UNN_CmdState)] = holderCmdPlusTime.u32IDModifyIndexWR;
    return 0;
}
/*
 __root  const short arrEkransId[] = {
     
    EKRAN_CONTROL_UP                  , 
    //.EKRAN_LIST_TYPE_IEC61850_NODES    ,
    EKRAN_LIST_INPUTS_FOR_RANGUVANNJA ,
    EKRAN_RANGUVANNJA_INPUT_1         ,
    EKRAN_LIST_OUTPUTS_FOR_RANGUVANNJA, 
    EKRAN_RANGUVANNJA_OUTPUT_1        ,
    EKRAN_RANGUVANNJA_OUTPUT_LAST     ,   
     
     
     
    EKRAN_LIST_LEDS_FOR_RANGUVANNJA,
    EKRAN_RANGUVANNJA_LED_1      ,
    
    
    EKRAN_LIST_SETTINGS_FOR_DF          ,
    EKRAN_RANGUVANNJA_DF8_BLK           ,
    
    EKRAN_LIST_DT                      ,
    EKRAN_SET_RESET_DT1                ,
    
    EKRAN_RANGUVANNJA_RESET_DT4_MINUS  ,
    
    
    EKRAN_LIST_D_AND          ,
    EKRAN_RANGUVANNJA_D_AND1  ,
    EKRAN_RANGUVANNJA_D_AND8  ,
    EKRAN_LIST_D_OR          ,
    EKRAN_RANGUVANNJA_D_OR1  ,
    EKRAN_RANGUVANNJA_D_OR8  ,
    EKRAN_LIST_D_XOR         ,
    EKRAN_RANGUVANNJA_D_XOR1 ,
    EKRAN_RANGUVANNJA_D_XOR8 ,
    EKRAN_LIST_D_NOT         ,
    EKRAN_RANGUVANNJA_D_NOT1  ,
    EKRAN_RANGUVANNJA_D_NOT16 ,
    EKRAN_LIST_TF_FOR_RANGUVANNJA,
    EKRAN_LIST_SOURCE_TF1,
    
    
    EKRAN_LIST_BUTTONS_FOR_RANGUVANNJA,
    EKRAN_RANGUVANNJA_BUTTON_1,
    EKRAN_RANGUVANNJA_BUTTON_6,
    
    EKRAN_VIDKLUCHENNJA,
    
    (NUMBER_INPUTS + NUMBER_OUTPUTS + NUMBER_LEDS + 5*NUMBER_DEFINED_FUNCTIONS + 7*NUMBER_DEFINED_TRIGGERS + NUMBER_DEFINED_AND + NUMBER_DEFINED_OR + NUMBER_DEFINED_XOR + NUMBER_DEFINED_NOT + NUMBER_TRANSFER_FUNCTIONS + NUMBER_DEFINED_BUTTONS),
    (N_IN_GOOSE + N_IN_MMS + N_OUT_LAN + NUMBER_INPUTS +
    NUMBER_OUTPUTS + NUMBER_LEDS + 5*NUMBER_DEFINED_FUNCTIONS + 
    7*NUMBER_DEFINED_TRIGGERS + NUMBER_DEFINED_AND + NUMBER_DEFINED_OR + NUMBER_DEFINED_XOR + 
    NUMBER_DEFINED_NOT + NUMBER_TRANSFER_FUNCTIONS + NUMBER_DEFINED_BUTTONS)
    
 };
*/







