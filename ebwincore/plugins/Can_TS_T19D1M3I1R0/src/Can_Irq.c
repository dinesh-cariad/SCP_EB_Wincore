/**
 * \file
 *
 * \brief AUTOSAR Can
 *
 * This file contains the implementation of the AUTOSAR
 * module Can.
 *
 * \version 3.1.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * Misra-C:2004 Deviations:
 *
 * MISRA-1) Deviated Rule: 19.1 (advisory)
 *   '#include' statements in a file should only be preceded by other
 *   preprocessor directives or comments.
 *
 *   Reason:
 *   Inclusion of MemMap.h.
 *
 */

/*==[Includes]================================================================*/
#include <os.h>
#include <Can.h>
#include <Can_Internal.h>
#include <CanIf_Cbk.h>
#include <SchM_Can.h>

/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/

/*==[Declaration of functions with internal linkage]==========================*/

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/
/* Avoid unnecessary inclusion of MemMap.h */
#if ((CAN_TX_MODE == CAN_MODE_INTERRUPT) || (CAN_RX_MODE == CAN_MODE_INTERRUPT))

#define CAN_START_SEC_CODE
#include <Can_MemMap.h>

#if (CAN_TX_MODE == CAN_MODE_INTERRUPT)
ISR(Can_Tx_Interrupt)
{
#if (CAN_NUM_HTH > 0)
  Can_ProcessTxFrame();
#endif /* (CAN_NUM_HTH > 0)*/
}
#endif /* (CAN_TX_MODE == CAN_MODE_INTERRUPT) */

#if (CAN_RX_MODE == CAN_MODE_INTERRUPT)
ISR(Can_Rx_Interrupt)
{
#if (CAN_NUM_HRH > 0)
  Can_ProcessRxFrame();
#endif /* (CAN_NUM_HRH > 0)*/
}
#endif /* (CAN_RX_MODE == CAN_MODE_INTERRUPT) */

/* Deviation MISRA-1 */
#define CAN_STOP_SEC_CODE
#include <Can_MemMap.h>

#endif /* ((CAN_TX_MODE == CAN_MODE_INTERRUPT) || (CAN_RX_MODE == CAN_MODE_INTERRUPT)) */

/*==[Definition of functions with internal linkage]===========================*/
