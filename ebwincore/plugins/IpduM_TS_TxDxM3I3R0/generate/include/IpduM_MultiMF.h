/**
 * \file
 *
 * \brief AUTOSAR IpduM
 *
 * This file contains the implementation of the AUTOSAR
 * module IpduM.
 *
 * \version 3.3.40
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef IPDUM_MULTIMF_H
#define IPDUM_MULTIMF_H

[!AUTOSPACING!]


[!INCLUDE "../../generate_macros/IpduM_Macros.m"!]

/*==================[inclusions]============================================*/
#include <IpduM_Types.h>
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

[!IF "$IpduMMultiMF = 'true'"!]
/** \brief IpduM RX main function
**
** Processes DEFERRED ContainerRxPdus
**
** This function has to be called periodically by a task controlled by the
** BSW scheduler.
**
** \ServiceID{0x11}
** \Reentrancy{Non-Reentrant}
** \Synchronicity{Synchronous}
*/
#define IPDUM_START_SEC_CODE
#include <IpduM_MemMap.h>
extern FUNC(void, IPDUM_CODE) IpduM_MainFunctionRx_Generic(IpduM_MainFuncIdxType MFIdx);
#define IPDUM_STOP_SEC_CODE
#include <IpduM_MemMap.h>
/** \brief IpduM TX main function
**
** Performs the processes of the activities that are not directly initiated by
** the calls from PDU-R. This includes at least the TxConfirmation time observation.
**
** This function has to be called periodically by a task controlled by the
** BSW scheduler.
**
** \ServiceID{0x12}
** \Reentrancy{Non-Reentrant}
** \Synchronicity{Synchronous}
*/
#define IPDUM_START_SEC_CODE
#include <IpduM_MemMap.h>
extern FUNC(void, IPDUM_CODE) IpduM_MainFunctionTx_Generic(IpduM_MainFuncIdxType MFIdx);
#define IPDUM_STOP_SEC_CODE
#include <IpduM_MemMap.h>

[!CALL "createRXMFFuncNames"!]
[!LOOP "text:split($mfRXalloc,';')"!]
[!VAR "shName" = "text:split(.,'#')[2]"!]
[!VAR "qName" = "text:split(.,'#')[3]"!]
[!CALL "createSegmentStr","shortName"="$shName"!]
#define [!"$secSTART"!]
#include <IpduM_MemMap.h>
extern FUNC(void, [!"$segCODE"!]) [!"$qName"!](void);
#define [!"$secSTOP"!]
#include <IpduM_MemMap.h>

[!ENDLOOP!]

[!CALL "createTXMFAlloc"!]
[!LOOP "text:split($mfTXalloc,';')"!]
[!VAR "shName" = "text:split(.,'#')[2]"!]
[!VAR "qName" = "text:split(.,'#')[3]"!]
[!CALL "createSegmentStr","shortName"="$shName"!]
#define [!"$secSTART"!]
#include <IpduM_MemMap.h>
extern FUNC(void, [!"$segCODE"!]) [!"$qName"!](void);
#define [!"$secSTOP"!]
#include <IpduM_MemMap.h>
[!ENDLOOP!][!//TX


[!ENDIF!][!//IpduMMultiMF

/*==================[internal function definitions]=========================*/


#endif /* ifndef IPDUM_MULTIMF_H */
