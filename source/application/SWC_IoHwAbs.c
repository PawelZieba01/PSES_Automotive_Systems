/**
 * \file
 *
 * \brief AUTOSAR Workflows
 *
 * This file contains the implementation of the AUTOSAR
 * module Workflows.
 *
 * \version 2.3.2
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#include "Rte_SWC_IoHwAbs.h"

#include "Dio.h"



#define SWC_IoHwAbs_START_SEC_VAR_UNSPECIFIED
#include <SWC_IoHwAbs_MemMap.h>

STATIC Dio_ChannelType SetPortArgToChannel[1] = {DioConf_DioChannel_MY_LED_CYCLIC};

#define SWC_IoHwAbs_STOP_SEC_VAR_UNSPECIFIED
#include <SWC_IoHwAbs_MemMap.h>


#define SWC_IoHwAbs_START_SEC_CODE
#include "SWC_IoHwAbs_MemMap.h"

FUNC(void, RTE_APPL_CODE) SWC_IoHwAbs_SetDiscreteValue (uint32 val0, uint32 value)
{
  /* write to Dio channel */
  Dio_WriteChannel(SetPortArgToChannel[val0], (Dio_LevelType) value);
}


FUNC(void, RTE_APPL_CODE) SWC_IoHwAbs_UpdateLED(void)
{
    uint32 P_MyLed = 0U;

    /* Odczyt sygnału z RTE (przykład) */
    (void)Rte_Read_P_MyLed(&P_MyLed);  

    /* Włączenie/wyłączenie diody przez DIO */
    if (P_MyLed != 0U) {
        SWC_IoHwAbs_SetDiscreteValue(0, 1U); /* LED ON */
    } else {
        SWC_IoHwAbs_SetDiscreteValue(0, 0U); /* LED OFF */
    }
}

#define SWC_IoHwAbs_STOP_SEC_CODE
#include "SWC_IoHwAbs_MemMap.h"
