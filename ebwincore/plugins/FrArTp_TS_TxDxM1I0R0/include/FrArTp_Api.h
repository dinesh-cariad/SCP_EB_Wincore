/**
 * \file
 *
 * \brief AUTOSAR FrArTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrArTp.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRARTP_API_H
#define FRARTP_API_H

/*==================[includes]================================================*/

#include <FrArTp_Cfg.h>           /* FrArTp pre-compile-time configuration */
#include <FrArTp_CfgTypes.h>      /* FrArTp pre-compile-time dependent configuration types */
#include <FrArTp_Version.h>       /* this module's version declaration */
#include <ComStack_Types.h>       /* basic commstack types from AUTOSAR */

/*==================[macros]================================================*/

/*
 * Instance ID for DET reporting
 */
/* instance ID reported to DET and SchM */
#define FRARTP_DET_GENERIC_INSTANCE_ID               0x0U

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00215,1 */
/**
 * \brief Service identifiers for FrArTp_GetVersionInfo API function
 */
#define FRARTP_GETVERSIONINFO_SERVICE_ID             0x27U

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00147,1 */
/**
 * \brief Service identifiers for FrArTp_Init API function
 */
#define FRARTP_INIT_SERVICE_ID                       0x00U

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00149,1 */
/**
 * \brief Service identifiers for FrArTp_Transmit API function
 */
#define FRARTP_TRANSMIT_SERVICE_ID                   0x02U

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00150,1 */
/**
 * \brief Service identifiers for FrArTp_CancelTransmit API function
 */
#define FRARTP_CANCELTRANSMIT_SERVICE_ID             0x03U

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00229,1 */
/**
 * \brief Service identifiers for FrArTp_CancelReceive API function
 */
#define FRARTP_CANCELRECEIVE_SERVICE_ID              0x08U

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00151,1 */
/**
 * \brief Service identifiers for FrArTp_ChangeParameter API
 */
#define FRARTP_CHANGEPARAMETER_SERVICE_ID            0x04U

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00154,1 */
/**
 * \brief Service identifiers for FrArTp_TriggerTransmit API function
 */
#define FRARTP_TRIGGERTRANSMIT_SERVICE_ID            0x41U

/* !LINKSTO FrArTp.EB.FrArTp_RxIndication,1 */
/**
 * \brief Service identifiers for FrArTp_RxIndication API function
 */
#define FRARTP_RXINDICATION_SERVICE_ID               0x42U

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00153,1 */
/**
 * \brief Service identifiers for FrArTp_TxConfirmation API function
 */
#define FRARTP_TXCONFIRMATION_SERVICE_ID             0x40U

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00162,1 */
/**
 * \brief Service identifiers for FrArTp_MainFunction API function
 */
#define FRARTP_MAINFUNCTION_SERVICE_ID               0x10U

/*------------------[DET error IDs]-----------------------------------------*/

/** \brief Det error Id FRARTP_E_UNINIT */
#define FRARTP_E_UNINIT                   0x01U

/** \brief Det error Id FRARTP_E_PARAM_POINTER */
#define FRARTP_E_PARAM_POINTER            0x02U

/** \brief Det error Id FRARTP_E_INVALID_PDU_SDU_ID */
#define FRARTP_E_INVALID_PDU_SDU_ID       0x03U

/** \brief Det error Id FRARTP_E_INIT_FAILED */
#define FRARTP_E_INIT_FAILED              0x04U

/** \brief Det error Id FRARTP_E_INVALID_PARAMETER */
#define FRARTP_E_INVALID_PARAMETER        0x05U


/*==================[type definitions]=========================*/

/*==================[external function declarations]=========================*/

#define FRARTP_START_SEC_CODE
#include <FrArTp_MemMap.h>

/** \brief Checks compatibility of the post-build-time configuration
 *
 * This service checks the compatibility of the post-build-time configuration
 * against the source code.
 *
 * \param[in] voidConfigPtr Pointer to the configuration data of the FrArTp module.
 *
 * \return Result of compatibility check
 * \retval E_OK      Provided configuration is compatible.
 * \retval E_NOT_OK  Provided configuration is notcompatible.
 *
 * \ServiceID{N/A}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRARTP_CODE) FrArTp_IsValidConfig
(
  P2CONST(void,AUTOMATIC,FRARTP_APPL_CONST) voidConfigPtr
);

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00147,1 */
/** \brief Initializes the FrArTp stack
 *
 * This service initializes the TCP/IP Stack.
 * The call of this service is mandatory before using the FrArTp instance for further processing.
 *
 * \param[in] ConfigPtr Address of the post-build time configuration data of the FrArTp module.
 *
 * \ServiceID{0x01}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, FRARTP_CODE) FrArTp_Init(P2CONST(FrArTp_ConfigType, AUTOMATIC, FRARTP_APPL_CONST) ConfigPtr);

#if (FRARTP_VERSION_INFO_API == STD_ON)
/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00215,1 */
/** \brief Get version information of the FrArTp module.
 *
 * This service returns the version information of this module. The version
 * information includes:
 *  - Module Id
 *  - Vendor Id
 *  - Vendor specific version numbers
 *
 * \param[out] versioninfo Pointer to where to store the version
 * information of this module.
 *
 * \ServiceID{0x02}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, FRARTP_CODE) FrArTp_GetVersionInfo(P2VAR(Std_VersionInfoType,AUTOMATIC, FRARTP_APPL_DATA) versioninfo);
#endif /* FRARTP_VERSION_INFO_API == STD_ON */

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00149,1 */
/**
 * \brief Request the transmission of data.
 *
 * This service is utilized to request the transmission of data.
 * This function has to be called with FrArTp's SDU-Id, i.e. the upper layer has to
 * translate its own PDU-Id into the FrArTp's SDU-ID for the corresponding message.
 * Within the provided FrArTpSduInfoPtr only SduLength is valid (no data)!
 * If this function returns E_OK then there will arise an call of PduR_FrArTpCopyTxData
 * in order to get data for sending
 *
 * \ServiceID{0x02}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Asynchronous}
 *
 * \param[in] FrArTpTxSduId        This parameter contains the unique identifier of
 *                                 the FrArTp N-SDU to be transmitted.
 *
 * \param[in] FrArTpTxSduInfoPtr   Tx N-SDU Information Structure which contains
 *                                 a) pointer to the FrArTp Tx N-SDU
 *                                 b) the length of the FrArTp Tx N-SDU
 *
 *
 * \return E_OK            The request has been accepted
 *
 * \return E_NOT_OK        The request has not been accepted, e. g. because the parameter
 *                         check has failed (invalid N-SDU ID or size), the corresponding connection
 *                         is still occupied, or no state machine is free
 *                         (FrArTpConcurrentConnections).
 */
extern FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_Transmit
(
    VAR(PduIdType,AUTOMATIC) FrArTpTxSduId,
    P2CONST(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) FrArTpTxSduInfoPtr
);

#if FRARTP_CANCEL_API == STD_ON

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00150,1 */
/**
 * \brief This service primitive is used to cancel the transmission of pending Fr
 *        N-SDUs. The connection is identified by FrArTpTxSduId.
 *
 * When the function returns, no transmission is in progress
 * anymore with the given N-SDU identifier.
 *
 * \ServiceID{0x03}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 *
 * \param[in] FrArTpTxSduId  Contains the unique identifier of the transmitted
 *                           Fr N-SDU which has to be canceled.
 *
 * \return E_OK              Cancellation request of the transmission
 *                           of the specified Fr N-SDU is accepted.
 *
 * \return E_NOT_OK          Cancellation request of the transmission of the specified
 *                           Fr N-SDU is rejected.
 */
extern FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_CancelTransmit
(
    VAR(PduIdType,AUTOMATIC) FrArTpTxSduId
);


/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00229,1 */
/**
 * \brief By calling this API with the corresponding RxSduId the currently
 *        ongoing data reception is terminated immediately. When the function
 *        returns, no reception is in progress anymore with the given N-SDU identifier.
 *
 * \ServiceID{0x08}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 *
 * \param[in] FrArTpRxSduId  SDU-Id of currently ongoing reception.
 *
 * \return E_OK              Reception was terminated sucessfully.
 *
 * \return E_NOT_OK          Reception was not terminated.
 */

extern FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_CancelReceive
(
    VAR(PduIdType,AUTOMATIC) FrArTpRxSduId
);

#endif /* FRARTP_CANCEL_API == STD_ON */

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00151,1 */
/**
 * \brief This service is used to request the change of the reception
 *        parameters STmin and BS for the specified N-SDU.
 *
 * \ServiceID{0x04}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 *
 * \param[in] id           Identifies the affected N-SDU.
 * \param[in] parameter    Specifies the parameter which shall be changed.
 * \param[in] value        The new value of the parameter.
 *
 * \return E_OK                     request is accepted
 *
 * \return E_NOT_OK                 request is not accepted
 */
extern FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_ChangeParameter
(
    VAR(PduIdType,AUTOMATIC) id,
    VAR(TPParameterType,AUTOMATIC) parameter,
    VAR(uint16,AUTOMATIC) value
);

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00162,1 */
/**
 * \brief The main function for scheduling the TP (Entry point for scheduling)
 *
 * \ServiceID{0x10}
 */
extern FUNC(void,FRARTP_CODE) FrArTp_MainFunction(void);

#define FRARTP_STOP_SEC_CODE
#include <FrArTp_MemMap.h>


#endif /* ifndef FRARTP_API_H */
/*==================[end of file]===========================================*/
