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

#ifdef _X86_
#include <stdio.h>
#include <Os.h>
#define MY_LED_CYCLIC 0U
#define Dio_ChannelType uint32
#else
#include "Dio.h"
#endif


#define SWC_IoHwAbs_START_SEC_VAR_UNSPECIFIED
#include <SWC_IoHwAbs_MemMap.h>

STATIC Dio_ChannelType SetPortArgToChannel[1] = {MY_LED_CYCLIC};

#define SWC_IoHwAbs_STOP_SEC_VAR_UNSPECIFIED
#include <SWC_IoHwAbs_MemMap.h>


#define SWC_IoHwAbs_START_SEC_CODE
#include "SWC_IoHwAbs_MemMap.h"

FUNC(void, RTE_APPL_CODE) SWC_IoHwAbs_SetDiscreteValue (uint32 val0, uint32 value)
{
#ifdef _X86_
  {
      os_intstatus_t intValue = OS_WINDOWSGoingToUseWindowsService();
      /* Print Dio channel */
      printf("Dio channel %d: %d\n",SetPortArgToChannel[val0], value);
      OS_WINDOWSFinishedUsingWindowsService(intValue);
  }
#else
  /* write to Dio channel */
  Dio_WriteChannel(SetPortArgToChannel[val0], (Dio_LevelType) value);
#endif
}

#define SWC_IoHwAbs_STOP_SEC_CODE
#include "SWC_IoHwAbs_MemMap.h"
