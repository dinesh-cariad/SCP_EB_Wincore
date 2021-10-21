/**
 * \file
 *
 * \brief AUTOSAR CanIf
 *
 * This file contains the implementation of the AUTOSAR
 * module CanIf.
 *
 * \version 6.10.15
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]
[!AUTOSPACING!]

/*==================[inclusions]============================================*/
[!IF "CanIfPublicCfg/CanIfDecoupledProcessingSupport = 'true'"!]

[!INCLUDE "../../generate_macros/CanIf_DecoupledProcessing.m"!]
#include <CanIf_MultiMF.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#define CANIF_START_SEC_CODE
#include <CanIf_MemMap.h>
[!CALL "createRxMFAlloc"!]
[!LOOP "text:split($mfRxAlloc,';')"!]
[!VAR "qRxName" = "text:split(.,'#')[3]"!]

/** \brief CanIf Rx main function
 **
 ** Processes decoupled RxPdus
 **
 ** This function has to be called by a task controlled by the
 ** BSW scheduler.
 **
 ** \ServiceID{0x40}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
FUNC(void, CANIF_CODE) [!"$qRxName"!](void)
{
  CanIf_MainFunctionRx_Generic([!"num:i(text:split(.,'#')[1])"!]U);
}

[!ENDLOOP!][!//Rx

[!CALL "createTxMFAlloc"!]
[!LOOP "text:split($mfTxAlloc,';')"!]
[!VAR "qTxName" = "text:split(.,'#')[3]"!]

/** \brief CanIf Tx main function
 **
 ** Performs the processes of the activities that are not directly initiated by
 ** the calls from CAN driver.
 **
 ** This function has to be called by a task controlled by the
 ** BSW scheduler.
 **
 ** \ServiceID{0x41}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
FUNC(void, CANIF_CODE) [!"$qTxName"!](void)
{
  CanIf_MainFunctionTx_Generic([!"num:i(text:split(.,'#')[1])"!]U);
}
[!ENDLOOP!][!//Tx

#define CANIF_STOP_SEC_CODE
#include <CanIf_MemMap.h>
[!ELSE!]
#include <TSCompiler.h>

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT

[!ENDIF!][!//CanIfDecoupledProcessingSupport
/*==================[end of file]===========================================*/
[!ENDCODE!][!//
