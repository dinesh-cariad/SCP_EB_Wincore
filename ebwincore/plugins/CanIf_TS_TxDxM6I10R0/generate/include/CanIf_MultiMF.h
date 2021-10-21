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

#ifndef CANIF_MULTIMF_H
#define CANIF_MULTIMF_H

/*==================[inclusions]============================================*/
[!IF "CanIfPublicCfg/CanIfDecoupledProcessingSupport = 'true'"!]

[!INCLUDE "../../generate_macros/CanIf_DecoupledProcessing.m"!]
#include <ComStack_Types.h>     /* AUTOSAR COM types */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define CANIF_START_SEC_CODE
#include <CanIf_MemMap.h>

/** \brief CanIf Rx main function
 **
 ** Processes RxPdus decoupled from the reception events
 **
 ** This function has to be called periodically by the BSW scheduler.
 **
 ** \ServiceID{0x40}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CANIF_CODE) CanIf_MainFunctionRx_Generic(uint8 Idx);


/** \brief CanIf Tx main function
 **
 ** Processes TxPdus decoupled from the transmit confirmation events
 **
 ** This function has to be called by the BSW scheduler.
 **
 ** \ServiceID{0x41}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CANIF_CODE) CanIf_MainFunctionTx_Generic(uint8 Idx);


[!CALL "createRxMFAlloc"!]
[!LOOP "text:split($mfRxAlloc,';')"!]
[!VAR "qName" = "text:split(.,'#')[3]"!]

#ifndef RTE_SCHM_SWCBSW_[!"text:toupper($qName)"!]
#define RTE_SCHM_SWCBSW_[!"text:toupper($qName)"!]
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
extern FUNC(void, CANIF_CODE) [!"$qName"!](void);
#endif /* RTE_SCHM_SWCBSW_[!"text:toupper($qName)"!] */

[!ENDLOOP!][!//Rx

[!CALL "createTxMFAlloc"!]
[!LOOP "text:split($mfTxAlloc,';')"!]
[!VAR "qName" = "text:split(.,'#')[3]"!]

#ifndef RTE_SCHM_SWCBSW_[!"text:toupper($qName)"!]
#define RTE_SCHM_SWCBSW_[!"text:toupper($qName)"!]
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
extern FUNC(void, CANIF_CODE) [!"$qName"!](void);
#endif /* RTE_SCHM_SWCBSW_[!"text:toupper($qName)"!] */

[!ENDLOOP!][!//Tx

#define CANIF_STOP_SEC_CODE
#include <CanIf_MemMap.h>
[!ELSE!]

#include <TSCompiler.h>

TS_PREVENTEMPTYTRANSLATIONUNIT

[!ENDIF!][!//CanIfDecoupledProcessingSupport

/*==================[internal function definitions]=========================*/
#endif /* ifndef CANIF_MULTIMF_H */
/*==================[end of file]============================================*/
[!ENDCODE!][!//

