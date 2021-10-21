/***************************************************************************
 *
 * \file Dccm_Cfg.c
 *
 * \brief Dccm configuration
 *
 * Copyright 2020 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 */

/*==================[inclusions]=================================================================*/

#include <Dccm_Internal.h>
#include <Dccm_Cfg.h>

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[internal constants]=========================================================*/

#define DCCM_START_SEC_CONST_UNSPECIFIED
#include <MemMap.h>

/* Initialize global arrays holding the values of each TxPduId/RxPduId for all configured functional
 * and physical communication protocols.
 */

/* Array of physical addresses */
[!IF "num:i(count(DccmPhysicalPduIds/*)) = '0'"!][!//
CONST(Dccm_PduIdsConfigurationType,DCCM_CONST) Dccm_PhysicalPduIdsArray[1U];
[!ELSE!][!//
CONST(Dccm_PduIdsConfigurationType,DCCM_CONST) Dccm_PhysicalPduIdsArray[[!"num:i(count(DccmPhysicalPduIds/*))"!]U] =
{[!INDENT "1"!]
 [!LOOP "node:order(DccmPhysicalPduIds/*, 'DccmPhysicalEcuId')"!][!//
 [!VAR "EcuId" = "num:integer(DccmPhysicalEcuId)"!][!//
 [!VAR "TxPduId" = "num:integer(DccmPhysicalTxPduId)"!][!//
 [!VAR "RxPduId" = "num:integer(DccmPhysicalRxPduId)"!][!//
 { [!"$EcuId"!]U, [!"$TxPduId"!]U, [!"$RxPduId"!]U},
 [!ENDLOOP!][!ENDINDENT!]};
[!ENDIF!][!//

/* Array of functional addresses */
[!IF "num:i(count(DccmFunctionalPduIds/*)) = '0'"!][!//
CONST(Dccm_PduIdsConfigurationType,DCCM_CONST) Dccm_FunctionalPduIdsArray[1U];
[!ELSE!][!//
CONST(Dccm_PduIdsConfigurationType,DCCM_CONST) Dccm_FunctionalPduIdsArray[[!"num:i(count(DccmFunctionalPduIds/*))"!]U] =
{[!INDENT "1"!]
 [!LOOP "node:order(DccmFunctionalPduIds/*, 'DccmFunctionalEcuId')"!][!//
 [!VAR "EcuId" = "num:integer(DccmFunctionalEcuId)"!][!//
 [!VAR "TxPduId" = "num:integer(DccmFunctionalTxPduId)"!][!//
 [!VAR "RxPduId" = "num:integer(DccmFunctionalRxPduId)"!][!//
 { [!"$EcuId"!]U, [!"$TxPduId"!]U, [!"$RxPduId"!]U},
 [!ENDLOOP!][!ENDINDENT!]};
[!ENDIF!][!//

#define DCCM_STOP_SEC_CONST_UNSPECIFIED
#include <MemMap.h>

/*==================[internal data]==============================================================*/

/*==================[external constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

/*==================[end of file]================================================================*/
