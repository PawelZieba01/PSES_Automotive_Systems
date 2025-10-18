#ifndef RTE_SWC_IOHWABS_H
#define RTE_SWC_IOHWABS_H

/* 
 * Rte_SWC_IoHwAbs.h
 * AUTOSAR RTE header for SWC_IoHwAbs
 */

#include "Std_Types.h"

/* Function prototypes */
Std_ReturnType Rte_Read_P_MyLed(uint32* value);

void SWC_IoHwAbs_SetDiscreteValue(uint32 val0, uint32 value);
void SWC_IoHwAbs_UpdateLED(void);

#endif /* RTE_SWC_IOHWABS_H */