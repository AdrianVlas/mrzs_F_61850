#ifndef __CONST_FSMC__
#define __CONST_FSMC__

#define Bank1_SRAM2_ADDR    0x64000000
#define Bank1_SRAM3_ADDR    0x68000000
#define Bank1_SRAM4_ADDR    0x6C000000

#define OFFSET_DD33_DD36                0x0800000
#define OFFSET_DD32_DD38                0x1000000
#define OFFSET_DD39_DD40_DD47           0x1400000
#define OFFSET_DD31_DD34_DD35_DD37      0x1800000

#if (                                   \
     (MODYFIKACIA_VERSII_PZ == 0) ||    \
     (MODYFIKACIA_VERSII_PZ == 3)       \
    )   

#define OFFSET_DD26_DD29                0x0C00000
#define OFFSET_DD28                     0x1C00000

#endif


#endif
