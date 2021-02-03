#include "cmd_log.h"
#include "header.h"
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
 CmdPlusTimeHolder holderCmdPlusTime;

// @In the steps below I am using a timestamp of 1203161493 
// @which corresponds to a date/time of 2-15-2008 11:31:33. 
// @I'm ignoring timezones and whatnot, it isn't necessary for this.





void CmdPlusTimeLogHundler(unsigned int *p_active_functions){
    register long i;
    UNUSED(p_active_functions);
    if(time_dat < 1203161493 )//&& time_ms == 0 
        return;
    i = holderCmdPlusTime.shIndexWR;
    if (i == 0)
        i = AMOUNT_CMD_PLUS_TIME_RECORD;
    else 
        i--;
    unsigned long *pCmd = (unsigned long *)&(holderCmdPlusTime.arrCmdPlusTimeHolder[i].cmd.uLCmd[0]);    
    if(
        ((p_active_functions[0] != pCmd[0])  )||
        ((p_active_functions[1] != pCmd[1])  )||
        ((p_active_functions[2] != pCmd[2])  )||
        ((p_active_functions[3] != pCmd[3])  )||
        ((p_active_functions[4] != pCmd[4])  )||
        ((p_active_functions[5] != pCmd[5])  )||
        ((p_active_functions[6] != pCmd[6])  )||
        ((p_active_functions[7] != pCmd[7])  )||
        ((p_active_functions[8] != pCmd[8])  )
    ){
        i = holderCmdPlusTime.shIndexWR;
        pCmd = (unsigned long *)&(holderCmdPlusTime.arrCmdPlusTimeHolder[i].cmd.uLCmd[0]);
        pCmd[0] = p_active_functions[0] ;
        pCmd[1] = p_active_functions[1] ;
        pCmd[2] = p_active_functions[2] ;
        pCmd[3] = p_active_functions[3] ;
        pCmd[4] = p_active_functions[4] ;
        pCmd[5] = p_active_functions[5] ;
        pCmd[6] = p_active_functions[6] ;
        pCmd[7] = p_active_functions[7] ;
        pCmd[8] = p_active_functions[8] ;
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
    }
}

void GetCmdPlusTimeLogElem(unsigned int *p_elem, long lIdx){
    register long i;
    //?UNUSED(p_active_functions);
    i = holderCmdPlusTime.shIndexWR-1;
    i -= lIdx;
    
    if (i < 0)
        i += AMOUNT_CMD_PLUS_TIME_RECORD;
//void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
 
    memcpy((void *)p_elem,(const void*)&(holderCmdPlusTime.arrCmdPlusTimeHolder[i].cmd.uLCmd[0]),
    sizeof(UNN_CmdState));

}

void GetDateTimeLogElem(unsigned int *p_elem, long lIdx){
    register long i;
    //?UNUSED(p_active_functions);
    i = holderCmdPlusTime.shIndexWR-1;
    i -= lIdx;
    
    if (i < 0)
        i += AMOUNT_CMD_PLUS_TIME_RECORD;
//void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
 
    memcpy((void *)p_elem,(const void*)&(holderCmdPlusTime.arrCmdPlusTimeHolder[i].unix_time.arU08MkTime),
    sizeof(UNN_UnixTime));

}


    
void GetMsLogElem(unsigned int *p_elem, long lIdx){
    register long i;
    //?UNUSED(p_active_functions);
    i = holderCmdPlusTime.shIndexWR-1;
    i -= lIdx;
    
    if (i < 0)
        i += AMOUNT_CMD_PLUS_TIME_RECORD;
//void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
 
    memcpy((void *)p_elem,(const void*)&(holderCmdPlusTime.arrCmdPlusTimeHolder[i].mksec.uLMkTime),
    sizeof(UNN_MicroSec));

}

long GetNumberChangingInLogElem( long lIdx){
    register long i;
    //?UNUSED(p_active_functions);           Errorneus code as this
    //?i = holderCmdPlusTime.shIndexWR-1;    ariphmetic make in lowfunc
    //?i -= lIdx;
    //?
    //?if (i < 0)
    //?    i += AMOUNT_CMD_PLUS_TIME_RECORD;
    
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



