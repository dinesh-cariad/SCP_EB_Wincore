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
#ifndef CANTP_EXITTABLETYPES_H

#if (defined CANTP_EXITTABLETYPES_H) /* to prevent double declaration */
#error CANTP_EXITTABLETYPES_H is already defined
#endif /* if (defined CANTP_EXITTABLETYPES_H) */
#define CANTP_EXITTABLETYPES_H

/*==================[inclusions]=============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* typedefs for AUTOSAR com stack */

#include <CanTp_Cfg.h>          /* CanTp configuration */

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                /* AUTOSAR Development Error Tracer */
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
#include <CanIf.h>              /* AUTOSAR Can Interface */
/* do not indirectly include PduR_SymbolicNames_PBcfg.h via PduR_CanTp.h */

#if (defined PDUR_NO_PBCFG_REQUIRED) /* to prevent double declaration */
#error PDUR_NO_PBCFG_REQUIRED is already defined
#endif /* if (defined PDUR_NO_PBCFG_REQUIRED) */
#define PDUR_NO_PBCFG_REQUIRED

#include <PduR_CanTp.h>         /* AUTOSAR PDU Router */

#undef PDUR_NO_PBCFG_REQUIRED

[!IF "CanTpGeneral/CanTpGptUsageEnable = 'true'"!][!//
#include <Gpt.h>                /* AUTOSAR General Purpose Timer */
[!ENDIF!][!//

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

[!IF "CanTpJumpTable/CanTpJumpTableMode != 'OFF'"!][!//

/** \brief CanTp exit jumptable type */
/* !LINKSTO CanTp.JTM_0015, 1 */
typedef struct
{
  /* Det */
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
  P2FUNC(Std_ReturnType, DET_CODE, DetReportError)
    (
      uint16 ModuleId,
      uint8 InstanceId,
      uint8 ApiId,
      uint8 ErrorId
    );  /**< Det_ReportError() function pointer */
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */


[!IF "CanTpJumpTable/CanTpUseSchMMacros = 'false'"!][!//
  /* SchM */
  P2FUNC(void, SCHM_CODE, SchMEnterCanTp)
    (
      void
    );  /**< SchM_EnterCanTp() function pointer */
  P2FUNC(void, SCHM_CODE, SchMExitCanTp)
    (
      void
    );  /**< SchM_ExitCanTp() function pointer */



[!ENDIF!][!//
  /* CanIf */
  P2FUNC(Std_ReturnType, CANIF_CODE, CanIfTransmit)
    (
      PduIdType CanTxPduId,
      P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) PduInfoPtr
    );  /**< CanIf_Transmit() function pointer */



  /* PduR */
  P2FUNC(BufReq_ReturnType, PDUR_CODE, PduRCanTpStartOfReception)
    (
      PduIdType CanTpRxPduId,
      PduLengthType TpSduLength,
      P2VAR(PduLengthType, TYPEDEF, CANTP_APPL_DATA
           ) BufferSizePtr
    );  /**< PduR_CanTpStartOfReception() function pointer */
  P2FUNC(BufReq_ReturnType, PDUR_CODE, PduRCanTpCopyRxData)
    (
      PduIdType CanTpRxPduId,
      P2CONST(PduInfoType, TYPEDEF, CANTP_APPL_DATA
             ) PduInfoPtr,
      P2VAR(PduLengthType, TYPEDEF, CANTP_APPL_DATA
           ) BufferSizePtr
    );  /**< PduR_CanTpCopyRxData() function pointer */
  P2FUNC(void, PDUR_CODE, PduRCanTpRxIndication)
    (
      PduIdType CanTpRxPduId,
      NotifResultType Result
    );  /**< PduR_CanTpRxIndication() function pointer */
  P2FUNC(BufReq_ReturnType, PDUR_CODE, PduRCanTpCopyTxData)
    (
      PduIdType CanTpTxPduId,
      P2VAR(PduInfoType, TYPEDEF, CANTP_APPL_DATA
           ) PduInfoPtr,
      P2VAR(RetryInfoType, TYPEDEF, CANTP_APPL_DATA
           ) RetryPtr,
      P2VAR(PduLengthType, TYPEDEF, CANTP_APPL_DATA
           ) AvailableDataPtr
    );  /**< PduR_CanTpCopyTxData() function pointer */
  P2FUNC(void, PDUR_CODE, PduRCanTpTxConfirmation)
    (
      PduIdType CanTpTxPduId,
      NotifResultType Result
    );  /**< PduR_CanTpTxConfirmation() function pointer */



[!IF "CanTpGeneral/CanTpGptUsageEnable = 'true'"!]
  /* Gpt */
  P2FUNC(void, GPT_CODE, GptEnableNotification)
    (Gpt_ChannelType channel);  /**< Gpt_EnableNotification() function pointer */
  P2FUNC(void, GPT_CODE, GptStartTimer)
    (
      Gpt_ChannelType channel,
      Gpt_ValueType value
    );  /**< Gpt_StartTimer() function pointer */
  P2FUNC(void, GPT_CODE, GptStopTimer)
    (Gpt_ChannelType channel);  /**< Gpt_StopTimer() function pointer */
[!ENDIF!][!//
} CanTp_ExitTableType;

[!ENDIF!][!//
/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#endif /* ifndef CANTP_EXITTABLETYPES_H */
/*==================[end of file]============================================*/
