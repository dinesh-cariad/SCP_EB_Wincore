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
#ifndef CANTP_ENTRYTABLEAPI_H
#define CANTP_ENTRYTABLEAPI_H

/*==================[inclusions]=============================================*/

#include <TSAutosar.h>              /* EB specific standard types */
#include <ComStack_Types.h>         /* typedefs for AUTOSAR com stack */

#include <CanTp_Version.h>          /* CanTp version declarations */
#include <CanTp_Cfg.h>              /* CanTp configuration */

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_OFF)
#include <CanTp_EntryTable.h>       /* CanTp entry jumptable */
#endif

#include <CanTp_Api.h>              /* CanTp public API */

/*==================[macros]=================================================*/

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_OFF)

#if(CANTP_VERSION_INFO_API == STD_ON)

#if (defined CanTp_GetVersionInfo) /* to prevent double declaration */
#error CanTp_GetVersionInfo is already defined
#endif /* if (defined CanTp_GetVersionInfo) */

/** \brief Wrapper macro for API function CanTp_GetVersionInfo() */
#define CanTp_GetVersionInfo  (CanTp_EntryTablePtr->GetVersionInfo)
#endif /* CANTP_VERSION_INFO_API == STD_ON */

#if (defined CanTp_Transmit) /* to prevent double declaration */
#error CanTp_Transmit is already defined
#endif /* if (defined CanTp_Transmit) */

/** \brief Wrapper macro for API function CanTp_Transmit() */
#define CanTp_Transmit        (CanTp_EntryTablePtr->Transmit)

#if(CANTP_TX_CANCELLATION_API == STD_ON)

#if (defined CanTp_CancelTransmit) /* to prevent double declaration */
#error CanTp_CancelTransmit is already defined
#endif /* if (defined CanTp_CancelTransmit) */

/** \brief Wrapper macro for API function CanTp_CancelTransmit() */
#define CanTp_CancelTransmit   (CanTp_EntryTablePtr->CancelTransmit)
#endif /* CANTP_TX_CANCELLATION_API == STD_ON */

#if (defined CanTp_MainFunction) /* to prevent double declaration */
#error CanTp_MainFunction is already defined
#endif /* if (defined CanTp_MainFunction) */

/** \brief Wrapper macro for API function CanTp_MainFunction() */
#define CanTp_MainFunction    (CanTp_EntryTablePtr->MainFunction)

#if(CANTP_DYNAMIC_NSA_API == STD_ON)

#if (defined CanTp_GetNSa) /* to prevent double declaration */
#error CanTp_GetNSa is already defined
#endif /* if (defined CanTp_GetNSa) */

/** \brief Wrapper macro for API function CanTp_GetNSa() */
#define CanTp_GetNSa          (CanTp_EntryTablePtr->GetNSa)

#if (defined CanTp_SetNSa) /* to prevent double declaration */
#error CanTp_SetNSa is already defined
#endif /* if (defined CanTp_SetNSa) */

/** \brief Wrapper macro for API function CanTp_SetNSa() */
#define CanTp_SetNSa          (CanTp_EntryTablePtr->SetNSa)
#endif /* CANTP_DYNAMIC_NSA_API == STD_ON */

#if(CANTP_CHANGE_PARAMETER_REQ_API == STD_ON)

#if (defined CanTp_ChangeParameter) /* to prevent double declaration */
#error CanTp_ChangeParameter is already defined
#endif /* if (defined CanTp_ChangeParameter) */

/** \brief Wrapper macro for API function CanTp_ChangeParameter() */
#define CanTp_ChangeParameter (CanTp_EntryTablePtr->ChangeParameter)
#endif /* CANTP_CHANGE_PARAMETER_REQ_API == STD_ON */

#if(CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON)

#if (defined CanTp_ChangeTxParameter) /* to prevent double declaration */
#error CanTp_ChangeTxParameter is already defined
#endif /* if (defined CanTp_ChangeTxParameter) */

/** \brief Wrapper macro for API function CanTp_ChangeParameter() */
#define CanTp_ChangeTxParameter (CanTp_EntryTablePtr->ChangeTxParameter)


#if (defined CanTp_ResetTxParameter) /* to prevent double declaration */
#error CanTp_ResetTxParameter is already defined
#endif /* if (defined CanTp_ResetTxParameter) */

/** \brief Wrapper macro for API function CanTp_ResetTxParameter() */
#define CanTp_ResetTxParameter (CanTp_EntryTablePtr->ResetTxParameter)
#endif /* CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON */

#if(CANTP_READ_PARAMETER_REQ_API == STD_ON)

#if (defined CanTp_ReadParameter) /* to prevent double declaration */
#error CanTp_ReadParameter is already defined
#endif /* if (defined CanTp_ReadParameter) */

/** \brief Wrapper macro for API function CanTp_ReadParameter() */
#define CanTp_ReadParameter (CanTp_EntryTablePtr->ReadParameter)
#endif /* CANTP_READ_PARAMETER_REQ_API == STD_ON */

#if(CANTP_RX_CANCELLATION_API == STD_ON)

#if (defined CanTp_CancelReceive) /* to prevent double declaration */
#error CanTp_CancelReceive is already defined
#endif /* if (defined CanTp_CancelReceive) */

/** \brief Wrapper macro for API function CanTp_CancelReceive() */
#define CanTp_CancelReceive   (CanTp_EntryTablePtr->CancelReceive)
#endif /* CANTP_RX_CANCELLATION_API == STD_ON */

#endif /*if(CANTP_PROVIDE_API_FUNCTIONS == STD_OFF) */
/*==================[type definitions]=======================================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#endif /* ifndef CANTP_ENTRYTABLEAPI_H */
/*==================[end of file]============================================*/
