#ifndef RTE_SWC_MODIFYECHO_H
#define RTE_SWC_MODIFYECHO_H

#include "std_types.h"

/* Basic RTE types */
typedef unsigned char uint8;
typedef unsigned int uint32;
typedef uint8 Std_ReturnType;

/* AUTOSAR RTE Macros */
#define RTE_APPL_CODE
#define FUNC(rettype, memclass) rettype

/* RTE function declarations */
Std_ReturnType Rte_Read_R_NewCounterIn_CounterValue(uint8* data);
Std_ReturnType Rte_Read_R_EchoResult_CounterValue(uint8* data);
Std_ReturnType Rte_Send_P_SpecialRequest_EventMessage(uint8 data);
void Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue(uint8 data);
Std_ReturnType Rte_Call_R_EchoResult_SetCounterAndAdd(uint8 input, uint8* result);

#endif /* RTE_SWC_MODIFYECHO_H */
