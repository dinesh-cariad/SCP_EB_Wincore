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

/*==================[inclusions]=============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* typedefs for AUTOSAR com stack */

#include <CanTp_ExitTable.h>    /* CanTp exit jumptable */
#include <CanTp_InternalCfg.h>  /* CanTp internal configuration */

[!IF "(CanTpJumpTable/CanTpJumpTableMode != 'OFF') and (CanTpJumpTable/CanTpUseSchMMacros = 'false')"!][!//
/* !LINKSTO CanTp.ASR40.CANTP165,1 */
#include <SchM_CanTp.h>         /* Schedule manager public API */

[!ENDIF!][!//
/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

[!IF "(CanTpJumpTable/CanTpJumpTableMode != 'OFF')"!][!//
#define CANTP_START_SEC_CONST_EXIT_JUMP_TABLE
#include <CanTp_MemMap.h>

/** \brief CanTp exit jumptable */
/* !LINKSTO CanTp.JTM_0016, 1 */
CONST(CanTp_ExitTableType, CANTP_EXIT_JUMP_TABLE) CanTp_ExitTable =
  {
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
    &Det_ReportError,
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */

[!IF "CanTpJumpTable/CanTpUseSchMMacros = 'false'"!][!//
    &SchM_Enter_CanTp_SCHM_CANTP_EXCLUSIVE_AREA_0,
    &SchM_Exit_CanTp_SCHM_CANTP_EXCLUSIVE_AREA_0,

[!ENDIF!][!//
    &CanIf_Transmit,

    &PduR_CanTpStartOfReception,
    &PduR_CanTpCopyRxData,
    &PduR_CanTpRxIndication,
    &PduR_CanTpCopyTxData,
    &PduR_CanTpTxConfirmation,

#if(CANTP_USE_GPT == STD_ON)
    &Gpt_EnableNotification,
    &Gpt_StartTimer,
    &Gpt_StopTimer,
#endif /* CANTP_USE_GPT == STD_ON */
  };

#define CANTP_STOP_SEC_CONST_EXIT_JUMP_TABLE
#include <CanTp_MemMap.h>

[!ENDIF!][!//
/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
