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

#define AMOUNT_CMD_PLUS_TIME_RECORD 4 //400

typedef struct tag_CmdPlusTimeHolder{
    CmdPlusTimeStateElem arrCmdPlusTimeHolder[AMOUNT_CMD_PLUS_TIME_RECORD];
    short shIndexWR;
	short shTotalFixElem;
} CmdPlusTimeHolder;    
extern CmdPlusTimeHolder holderCmdPlusTime;

void ChekCurrentStateCmd(unsigned int *p_active_functions);
void CmdPlusTimeLogHundler(unsigned int *p_active_functions);
extern void GetCmdPlusTimeLogElem(unsigned int *p_elem, long lIdx);
extern void GetDateTimeLogElem(unsigned int *p_elem, long lIdx);
extern void GetMsLogElem(unsigned int *p_elem, long lIdx);

#endif
