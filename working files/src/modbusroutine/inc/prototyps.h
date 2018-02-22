#ifndef __PROTOTYPS_H
#define __PROTOTYPS_H

void global_component_installation(void);
//int  superReader20(int);
//int  superReader20Pro(int);
int  superReaderRegister(int);
int  superWriterRegister(int, int);
int  superReaderBit(int);
int  superWriterBit(int, int);
void superPreReadAction(void);
void superPreWriteAction(void);
int  superPostWriteAction(void);
void superSetOperativMarker(COMPONENT_OBJ *component, int adrReg);
void superSetTempWriteArray(int dataReg);
int  superFindTempWriteArrayOffset(int adr);
void superClearActiveActualData(void);
int  controlPerimetr(int adrReg, int begin_adr, int end_adr);
//void fix_change_settings_m(unsigned int setting_rang, unsigned int source);

int decoderN_BIGACMD(int idxBit);
unsigned int encoderN_BIGACMD(int offsetCMD);
int decoderN_SMALLACMD(int idxBit);
unsigned int encoderN_SMALLACMD(int offsetCMD);
int getACMDSmallBeginAdr(void);
int getSequenceN_BIGIndexActiveBit(int propusk, unsigned int *array);
int getSequenceN_SMALLIndexActiveBit(int propusk, unsigned int *array);
int validN_BIGACMD(unsigned short dataReg);
int validN_SMALLACMD(unsigned short dataReg);
int getRangN_BIGModbusRegister(unsigned int *ranguvannja, int countItem, int offset );
void writeRangN_BIGModbusRegister(unsigned int *ranguvannja, int countItem, int beginAdr, 
                                  int countAdr, int begin_adr_register);
int getRangN_SMALLModbusRegister(unsigned int *ranguvannja, int countItem, int offset );
void writeRangN_SMALLModbusRegister(unsigned int *ranguvannja, int countItem, int beginAdr, 
                                  int countAdr, int begin_adr_register);

void constructorIUSmallComponent(COMPONENT_OBJ *);
void constructorUSTBigComponent(COMPONENT_OBJ *);
void constructorACMDSmallComponent(COMPONENT_OBJ *);
void constructorUPRBigComponent(COMPONENT_OBJ *);
void constructorANDBigComponent(COMPONENT_OBJ *);
void constructorDOUTBigComponent(COMPONENT_OBJ *);
void constructorDOUTSmallComponent(COMPONENT_OBJ *);
void constructorDTRBigComponent(COMPONENT_OBJ *);
void constructorDVBigComponent(COMPONENT_OBJ *);
void constructorDVSmallComponent(COMPONENT_OBJ *);
void constructorGCMDSmallComponent(COMPONENT_OBJ *);
void constructorKEYBigComponent(COMPONENT_OBJ *);
void constructorMFTBigComponent(COMPONENT_OBJ *);
void constructorNOTBigComponent(COMPONENT_OBJ *);
void constructorORBigComponent(COMPONENT_OBJ *);
void constructorPKVBigComponent(COMPONENT_OBJ *);
void constructorRAISmallComponent(COMPONENT_OBJ *);
void constructorREGBigComponent(COMPONENT_OBJ *);
void constructorPREGBigComponent(COMPONENT_OBJ *);
void constructorRPRBigComponent(COMPONENT_OBJ *);
void constructorRPRSmallComponent(COMPONENT_OBJ *);
void constructorSDIBigComponent(COMPONENT_OBJ *);
void constructorSDISmallComponent(COMPONENT_OBJ *);
void constructorVVBigComponent(COMPONENT_OBJ *);
void constructorXORBigComponent(COMPONENT_OBJ *);
void constructorYustBigComponent(COMPONENT_OBJ *);
void constructorAISmallComponent(COMPONENT_OBJ *);
void constructorQAISmallComponent(COMPONENT_OBJ *);
void constructorPFBigComponent(COMPONENT_OBJ *);
#endif
