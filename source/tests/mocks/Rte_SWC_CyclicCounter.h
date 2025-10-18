#ifndef RTE_SWC_CYCLICCOUNTER_H
#define RTE_SWC_CYCLICCOUNTER_H

/* Basic types */
typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned char boolean;
typedef unsigned char Std_ReturnType;

/* Common macros */
#ifndef TRUE
#define TRUE 1U
#endif
#ifndef FALSE
#define FALSE 0U
#endif

#ifndef E_OK
#define E_OK 0U
#endif

/* Communication mode macros */
#define FULL_COMMUNICATION 1U
#define NO_COMMUNICATION 0U

/* AUTOSAR helper macros used in sources */
#define RTE_APPL_CODE
#define FUNC(rettype, memclass) rettype
#define P2VAR(type, memclass, ptrclass) type*

/* RTE API prototypes (mocks) used by SWC_CyclicCounter.c */

/* PIM access (per-instance memory pointer) */
uint8* Rte_Pim_PimCounterValue(void);

/* Inter-runnable variables */
void Rte_IrvWrite_Cyclic_CurrentCounterValue(uint8 val);
uint8 Rte_IrvRead_Cyclic_CurrentCounterValue(void);

uint8 Rte_IrvRead_SetCounter_CurrentCounterValue(void);
void Rte_IrvWrite_SetCounter_CurrentCounterValue(uint8 val);

/* Calls to other modules / services */
Std_ReturnType Rte_Call_UR_ComMUser_0_RequestComMode(uint8 mode);
Std_ReturnType Rte_Call_PS_PersistentCounterValue_SetRamBlockStatus(boolean status);

/* Receive/send API */
Std_ReturnType Rte_Receive_R_SpecialHandling_EventMessage(uint8* data);
Std_ReturnType Rte_Write_P_CounterOut_CounterValue(uint8 val);

/* EchoCalc */
Std_ReturnType Rte_Write_P_EchoCalc_CounterValue(uint8 val);

/* Client-server and other calls */
Std_ReturnType Rte_Call_R_CyclicCounterDet_ReportError(uint8 a, uint8 b, uint8 c);
Std_ReturnType Rte_Call_R_MyLED_SetDiscreteValue(uint8 val);

#endif /* RTE_SWC_CYCLICCOUNTER_H */
