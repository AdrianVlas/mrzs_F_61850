#ifndef __HEADER_M_H
#define __HEADER_M_H

#define FILENUMBER(fileNumber)     (fileNumber    <<16)
#define RECORDNUMBER(recordNumber) (recordNumber  &0xFFFF)
#define RECORDNUMBER_recordNumber_ (recordNumber  &0xFFFF)

#define GET_UPR_REGISTER000FOR20(fileNum)       getUprRegister000for20      \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                    \
        registerNumber)

#define GET_GROUPUPR_REGISTER000FOR20(fileNum)  getGroupUprRegister000for20 \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                    \
        registerNumber, countGroupBit)

#define GET_UST_REGISTER000FOR20(fileNum)       getUstRegister000for20      \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                    \
        registerNumber)

#define SET_UPR_REGISTER000FOR21(fileNum)       setUprRegister000for21      \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                    \
        registerNumber, value)

#define SET_GROUPUPR_REGISTER000FOR21(fileNum)  setGroupUprRegister000for21 \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                    \
        registerNumber, countGroupBit, value)

#define SET_UST_REGISTER000FOR21(fileNum)       setUstRegister000for21      \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                    \
        registerNumber, value)

#define POST_UPR_REGISTER000FOR21(fileNum)      postUprRegister000for21     \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                    \
        registerNumber, value)

#define POST_GROUPUPR_REGISTER000FOR21(fileNum) postGroupUprRegister000for21 \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                     \
        registerNumber, countGroupBit, value)

#define POST_UST_REGISTER000FOR21(fileNum)      postUstRegister000for21      \
        (FILENUMBER(fileNum)+RECORDNUMBER_recordNumber_,                     \
        registerNumber, value)

#if (                                \
     (MODYFIKACIA_VERSII_PZ == 23) || \
     (MODYFIKACIA_VERSII_PZ == 24) || \
     (MODYFIKACIA_VERSII_PZ == 26) || \
     (MODYFIKACIA_VERSII_PZ == 33) || \
     (MODYFIKACIA_VERSII_PZ == 34)    \
    )   
#define  MODYFIKACIA_VERSII_DS
#endif
#if (                                   \
       (MODYFIKACIA_VERSII_PZ == 0) ||  \
       (MODYFIKACIA_VERSII_PZ == 3) ||  \
       (MODYFIKACIA_VERSII_PZ == 4) ||  \
       (MODYFIKACIA_VERSII_PZ == 6) ||  \
       (MODYFIKACIA_VERSII_PZ == 10)||  \
       (MODYFIKACIA_VERSII_PZ == 13)||  \
       (MODYFIKACIA_VERSII_PZ == 14)    \
      )   
#define  MODYFIKACIA_VERSII_OVD_ZDZ
#endif

#include "componentdef.h"
#include "componentenum.h"
#include "prototyps.h"

#endif
