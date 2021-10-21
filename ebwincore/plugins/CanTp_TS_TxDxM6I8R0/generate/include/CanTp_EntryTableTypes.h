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
#ifndef CANTP_ENTRYTABLETYPES_H
#define CANTP_ENTRYTABLETYPES_H

/*==================[inclusions]=============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* typedefs for AUTOSAR com stack */

#include <CanTp_Cfg.h>          /* CanTp configuration */
#include <CanTp_ExitTableTypes.h>   /* CanTp exit jumptable types */
#include <CanTp_InternalCfg.h>  /* CanTp internal configuration */
#include <CanTp_Types_Int.h>    /* CanTp postbuild types */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

[!IF "CanTpJumpTable/CanTpJumpTableMode != 'OFF'"!][!//
/** \brief CanTp entry jumptable type */
/* !LINKSTO CanTp.JTM_0005, 1, CanTp.JTM_0008, 1 */
typedef struct
{
  /* API functions */

  P2FUNC(void, CANTP_CODE, Init)
  (
    P2CONST(CanTp_ConfigType, AUTOMATIC, CANTP_APPL_CONST) CfgPtr
  );   /**< CanTp_Init() function pointer */
#if(CANTP_VERSION_INFO_API == STD_ON)
  P2FUNC(void, CANTP_CODE, GetVersionInfo)
  (
    P2VAR(Std_VersionInfoType, AUTOMATIC, CANTP_APPL_DATA) VersionInfoPtr
  );        /**< CanTp_GetVersionInfo() function pointer */
#endif /* CANTP_VERSION_INFO_API == STD_ON */
  P2FUNC(Std_ReturnType, CANTP_CODE, Transmit)
  (
    PduIdType CanTpTxSduId,
    P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpTxInfoPtr
  );        /**< CanTp_Transmit() function pointer */
#if(CANTP_TX_CANCELLATION_API == STD_ON)
  P2FUNC(Std_ReturnType, CANTP_CODE, CancelTransmit)
  (
    PduIdType CanTpTxSduId
  );        /**< CanTp_CancelTransmit() function pointer */
#endif /* CANTP_TX_CANCELLATION_API == STD_ON */
  P2FUNC(void, CANTP_CODE, MainFunction)(void);
    /**< CanTp_MainFunction() function pointer */

  /* Callback functions */

  P2FUNC(void, CANTP_CODE, RxIndication)
  (
    PduIdType CanTpRxPduId,
    P2VAR(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr
  );        /**< CanTp_RxIndication() function pointer */
  P2FUNC(void, CANTP_CODE, TxConfirmation)
  (
    PduIdType CanTpTxPduId
  );        /**< CanTp_TxConfirmation() function pointer */

[!IF "CanTpGeneral/CanTpGptUsageEnable = 'true'"!][!//
  /* Miscellaneous functions */

  P2FUNC(void, CANTP_CODE, STminCallback)
  (
    CanTp_ChType Channel
  );        /**< CanTp_STminCallback() function pointer */
[!ENDIF!][!//

#if(CANTP_DYNAMIC_NSA_API == STD_ON)
  P2FUNC(Std_ReturnType, CANTP_CODE, GetNSa)
  (
    PduIdType CanTpPduId,
    uint8     CanTpDirection,
    P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) CanTpNSaPtr
  );        /**< CanTp_GetNSa() function pointer */

  P2FUNC(Std_ReturnType, CANTP_CODE, SetNSa)
  (
    PduIdType CanTpPduId,
    uint8     CanTpDirection,
    uint8     CanTpNSa
  );        /**< CanTp_SetNSa() function pointer */
#endif /* CANTP_DYNAMIC_NSA_API == STD_ON */

#if(CANTP_CHANGE_PARAMETER_REQ_API == STD_ON)
  P2FUNC(Std_ReturnType, CANTP_CODE, ChangeParameter)
  (
    PduIdType id,
    TPParameterType parameter,
    uint16 value
  );        /**< CanTp_ChangeParameter() function pointer */
#endif /* CANTP_CHANGE_PARAMETER_REQ_API == STD_ON */

#if(CANTP_READ_PARAMETER_REQ_API == STD_ON)
  P2FUNC(Std_ReturnType, CANTP_CODE, ReadParameter)
  (
    PduIdType id,
    TPParameterType parameter,
    uint16* value
  );        /**< CanTp_ReadParameter() function pointer */
#endif /* CANTP_READ_PARAMETER_REQ_API == STD_ON */

#if(CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON)
  P2FUNC(Std_ReturnType, CANTP_CODE, ChangeTxParameter)
  (
    PduIdType id,
    TPParameterType parameter,
    uint16 value
  );        /**< CanTp_ChangeTxParameter() function pointer */

  P2FUNC(void, CANTP_CODE, ResetTxParameter)
  (
    PduIdType id
  );        /**< CanTp_ResetTxParameter() function pointer */
#endif /* CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON */

#if(CANTP_RX_CANCELLATION_API == STD_ON)
  P2FUNC(Std_ReturnType, CANTP_CODE, CancelReceive)
  (
    PduIdType CanTpRxSduId
  );        /**< CanTp_CancelReceive() function pointer */
#endif /* CANTP_RX_CANCELLATION_API == STD_ON */

} CanTp_EntryTableType;

[!ENDIF!][!//
/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#endif /* ifndef CANTP_ENTRYTABLETYPES_H */
/*==================[end of file]============================================*/
