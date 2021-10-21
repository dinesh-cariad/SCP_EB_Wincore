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

#include "Rte_SWC_ModifyEcho.h"

#define SWC_ModifyEcho_START_SEC_CODE
#include "SWC_ModifyEcho_MemMap.h"

FUNC(void, RTE_APPL_CODE) SWC_ModifyEcho_ModifyEcho(void)
{
  uint8 echoValue = 0U;
  uint8 inputValue = 0U;

  /* Get InputValue from bus system */
  (void) Rte_Read_R_NewCounterIn_CounterValue(&inputValue);

  if( inputValue >= 0xF0 )
  {
  /* this was an additional request that will be handled otherwise
     don't use as counter value  */
    (void) Rte_Send_P_SpecialRequest_EventMessage(inputValue);

    /* write special command as output,
       Rte_IWrite must be always be called in this
       runnable otherwise 0 is send (implicit communication) */
    Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue(inputValue);

  }
  else
  {
    /* calc output value using Server function */
    (void) Rte_Call_R_EchoResult_SetCounterAndAdd(inputValue,&echoValue);

    /* write echo value as output */
    Rte_IWrite_ModifyEcho_P_EchoOut_CounterValue(echoValue);

  }

}

#define SWC_ModifyEcho_STOP_SEC_CODE
#include "SWC_ModifyEcho_MemMap.h"
