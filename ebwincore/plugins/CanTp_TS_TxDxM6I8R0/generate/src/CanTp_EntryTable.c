/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!IF "(CanTpJumpTable/CanTpJumpTableMode = 'CLIENT')"!][!//

[!ENDIF!][!//
/*==================[inclusions]=============================================*/

#include <TSAutosar.h>              /* EB specific standard types */
#include <ComStack_Types.h>         /* typedefs for AUTOSAR com stack */

#include <CanTp_EntryTable.h>       /* CanTp entry jumptable */
#include <CanTp_ExitTable.h>        /* CanTp exit jumptable */
#include <CanTp_Api.h>              /* CanTp public API */
#include <CanTp_Cbk.h>              /* CanTp callback declarations */
#include <CanTp_Internal.h>         /* internal macros and variables */
[!IF "    (CanTpJumpTable/CanTpJumpTableMode = 'CLIENT')
      and node:exists(CanTpJumpTable/CanTpJumpTableIncludeFile)"!][!//
#include <[!"CanTpJumpTable/CanTpJumpTableIncludeFile"!]>   /* entry jumptable include file */
[!ENDIF!][!//

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/
[!IF "(CanTpJumpTable/CanTpJumpTableMode = 'SERVER')"!][!//

#define CANTP_START_SEC_CONST_ENTRY_JUMP_TABLE
#include <CanTp_MemMap.h>

/** \brief CanTp entry jumptable */
/* !LINKSTO CanTp.JTM_0006, 1 */
CONST(CanTp_EntryTableType, CANTP_ENTRY_JUMP_TABLE) CanTp_EntryTable =
  {
    &CanTp_Init,
#if(CANTP_VERSION_INFO_API == STD_ON)
    &CanTp_GetVersionInfo,
#endif /* CANTP_VERSION_INFO_API == STD_ON */
    &CanTp_Transmit,
#if(CANTP_TX_CANCELLATION_API == STD_ON)
    &CanTp_CancelTransmit,
#endif /* CANTP_TX_CANCELLATION_API == STD_ON */
    &CanTp_MainFunction,
    &CanTp_RxIndication,
    &CanTp_TxConfirmation,
[!IF "count(CanTpConfig/*[1]/CanTpChannel/*/CanTpSTminTimeoutHandling[.='Gpt']) > 0"!][!// compare CANTP_USE_GPT
    &CanTp_STminCallback,
[!ENDIF!][!//
#if(CANTP_DYNAMIC_NSA_API == STD_ON)
    &CanTp_GetNSa,
    &CanTp_SetNSa,
#endif /* CANTP_DYNAMIC_NSA_API == STD_ON */
#if(CANTP_CHANGE_PARAMETER_REQ_API == STD_ON)
    &CanTp_ChangeParameter,
#endif /* CANTP_CHANGE_PARAMETER_REQ_API == STD_ON */
#if(CANTP_READ_PARAMETER_REQ_API == STD_ON)
    &CanTp_ReadParameter,
#endif /* CANTP_READ_PARAMETER_REQ_API == STD_ON */
#if(CANTP_RX_CANCELLATION_API == STD_ON)
    &CanTp_CancelReceive,
#endif /* CANTP_RX_CANCELLATION_API == STD_ON */
#if(CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON)
    &CanTp_ChangeTxParameter,
    &CanTp_ResetTxParameter
#endif /* CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON */
  };

#define CANTP_STOP_SEC_CONST_ENTRY_JUMP_TABLE
#include <CanTp_MemMap.h>

[!ELSEIF "(CanTpJumpTable/CanTpJumpTableMode = 'CLIENT')"!][!//

#define CANTP_START_SEC_CONST_UNSPECIFIED
#include <CanTp_MemMap.h>

/** \brief CanTp entry jumptable pointer */
CONSTP2CONST(CanTp_EntryTableType, CANTP_CONST, CANTP_ENTRY_JUMP_TABLE) CanTp_EntryTablePtr =
  (P2CONST(CanTp_EntryTableType, CANTP_CONST, CANTP_ENTRY_JUMP_TABLE))[!"CanTpJumpTable/CanTpJumpTableAddress"!];

#define CANTP_STOP_SEC_CONST_UNSPECIFIED
#include <CanTp_MemMap.h>

[!ENDIF!][!//
/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
