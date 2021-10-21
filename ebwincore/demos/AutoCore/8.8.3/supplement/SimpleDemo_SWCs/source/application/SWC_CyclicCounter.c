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

#include "Rte_SWC_CyclicCounter.h"

#include <BswM.h>                       /* BswM API */
#include <Eb_Intgr_BswM_UserCallouts.h>          /* Callout function declarations */
#include <Base_Modules.h>               /* Module enable defines and
                                           standard config pointer names */

#ifdef _X86_
#include <stdio.h>
#include <Os.h>
#endif

#define SWC_CyclicCounter_START_SEC_CODE
#include "SWC_CyclicCounter_MemMap.h"

FUNC(void, RTE_APPL_CODE) SWC_CyclicCounter_Init(void)
{
  uint8 counterValue = 0U;

  /* Read counter value from per-instance memory
     which is configured as RAM mirror for an
     NvM block */
  counterValue = *Rte_Pim_PimCounterValue();

  /* Write counter value to inter-runnable variable */
  Rte_IrvWrite_Cyclic_CurrentCounterValue(counterValue);

#ifdef BASE_COMM_ENABLED
  /* Notify BswM about a state change */
  BswM_RequestMode(EB_INTGR_BSWM_SWC_CYCLIC, EB_INTGR_BSWM_SWC_CYCLIC_RUN);
#endif /* #ifdef BASE_COMM_ENABLED */

  /* request full communication mode */
  (void) Rte_Call_UR_ComMUser_0_RequestComMode(FULL_COMMUNICATION);

}

FUNC(void, RTE_APPL_CODE) SWC_CyclicCounter_Shutdown(void)
{
  uint8 counterValue = 0U;

  /* update the RAM mirror */
  counterValue = Rte_IrvRead_Cyclic_CurrentCounterValue();
  (*Rte_Pim_PimCounterValue()) = counterValue;

  /* and mark block as modified */
  (void) Rte_Call_PS_PersistentCounterValue_SetRamBlockStatus(TRUE);

}

FUNC(void, RTE_APPL_CODE) SWC_CyclicCounter_Cyclic(void)
{
  uint8 counterValue = 0U;
  uint8 specialReceiveCommands[10] = { 0U };
  uint8 i = 0U;
  boolean specialReceiveCommand = FALSE;

  /* get all special commands received since last run */
  while(Rte_Receive_R_SpecialHandling_EventMessage(&specialReceiveCommands[0]) == E_OK)
  {
    if(i >= sizeof(specialReceiveCommands)) break;
    /* save all requests in an temp array, can be used for later processing */
    specialReceiveCommands[sizeof(specialReceiveCommands) - i - 1U]=specialReceiveCommands[0];

    if (specialReceiveCommands[sizeof(specialReceiveCommands) - i - 1U] == 0xf1)
    {
#ifdef _X86_
	  {
	    os_intstatus_t intValue = OS_WINDOWSGoingToUseWindowsService();
	    /* Print counter value to standard out */
	    printf("Received trigger Det command 0xF1\n");
	    OS_WINDOWSFinishedUsingWindowsService(intValue);
	  }
#endif
      /* special command for generating pseudo det error */
      (void) Rte_Call_R_CyclicCounterDet_ReportError(1U,2U,3U);
      specialReceiveCommand = TRUE;
    }
    else if (specialReceiveCommands[sizeof(specialReceiveCommands) - i - 1U] == 0xff)
    {
#ifdef _X86_
  	  {
  	    os_intstatus_t intValue = OS_WINDOWSGoingToUseWindowsService();
        /* Print counter value to standard out */
        printf("Received shutdown command 0xFF\n");
  	    OS_WINDOWSFinishedUsingWindowsService(intValue);
  	  }
#endif

#ifdef BASE_COMM_ENABLED
      /* notify BswM about a state change */
      BswM_RequestMode(EB_INTGR_BSWM_SWC_CYCLIC, EB_INTGR_BSWM_SWC_CYCLIC_SHUTDOWN);
#endif /* #ifdef BASE_COMM_ENABLED */

      /* special command for shutdown */
      (void) Rte_Call_UR_ComMUser_0_RequestComMode(NO_COMMUNICATION);
      specialReceiveCommand = TRUE;
    }
    i++;
  }

  /* don't handle counter values which are special commands */
  if (!specialReceiveCommand)
  {
    /* Read counter value from inter-runnable variable */
     counterValue = Rte_IrvRead_Cyclic_CurrentCounterValue();

    /* toggle LED */
    if ((counterValue % 2U) == 0U)
    {
      (void) Rte_Call_R_MyLED_SetDiscreteValue(1U);
    }
    else
    {
      (void) Rte_Call_R_MyLED_SetDiscreteValue(0U);
    }

    /* Write incremented counter value to inter-runnable variable */
    Rte_IrvWrite_Cyclic_CurrentCounterValue(counterValue + 1U);

#ifdef _X86_
    {
  	  os_intstatus_t intValue = OS_WINDOWSGoingToUseWindowsService();
      /* Print counter value to standard out */
      printf("Counter: %d\n", counterValue);
  	  OS_WINDOWSFinishedUsingWindowsService(intValue);
    }
#endif

    /* Write current counter value */
    (void) Rte_Write_P_CounterOut_CounterValue(counterValue);
  }
}


FUNC(void, RTE_APPL_CODE) SWC_CyclicCounter_SetCounter
(
uint8 newCounterValue,
P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) returnSum
)
{
    uint8 currentCounter = 0U;

    /* read actual cyclic counter value */
    currentCounter = Rte_IrvRead_SetCounter_CurrentCounterValue();

    /* replace the current counter value with new */
    Rte_IrvWrite_SetCounter_CurrentCounterValue(newCounterValue);

    /* return the sum */
    *returnSum = currentCounter + newCounterValue;
}

#define SWC_CyclicCounter_STOP_SEC_CODE
#include "SWC_CyclicCounter_MemMap.h"
