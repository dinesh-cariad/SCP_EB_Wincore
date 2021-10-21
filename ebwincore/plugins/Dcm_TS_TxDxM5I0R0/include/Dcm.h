/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DCM_H
#define DCM_H

/* !LINKSTO Dcm.Dsn.File.Dcm.PublicApi,1 */
/* This file contains the public interface of the Dcm module. */

/*==================[inclusions]=================================================================*/

#include <ComStack_Types.h>                                           /* AUTOSAR Com Stack types */

#include <Dcm_Dsl_ComMChannelStateManager_External_Interface.h>   /* Dcm Dsl ComMChannelStateManager
                                                                     Interface header file */

#include <Dcm_Version.h>                                          /* Module version declarations */
#include <Dcm_Types.h>                                                    /* DCM type definition */
#include <Dcm_Dsl_Cfg.h>                                                    /* DSL configuration */

/*==================[macros]=====================================================================*/

/** \brief This value specifies that the server shall transmit the requested dataRecord information
 ** at a slow rate in response to the request message */
#if (defined DCM_UDS_RDBPI_TRANSMISSIONMODE_SLOWRATE)
  #error "DCM_UDS_RDBPI_TRANSMISSIONMODE_SLOWRATE is already defined"
#endif
#define DCM_UDS_RDBPI_TRANSMISSIONMODE_SLOWRATE    (0x01U)
/** \brief This value specifies that the server shall transmit the requested dataRecord information
 ** at a medium rate in response to the request message */
#if (defined DCM_UDS_RDBPI_TRANSMISSIONMODE_MEDIUMRATE)
  #error "DCM_UDS_RDBPI_TRANSMISSIONMODE_MEDIUMRATE is already defined"
#endif
#define DCM_UDS_RDBPI_TRANSMISSIONMODE_MEDIUMRATE  (0x02U)
/** \brief This value specifies that the server shall transmit the requested dataRecord information
 ** at a fast rate in response to the request message */
#if (defined DCM_UDS_RDBPI_TRANSMISSIONMODE_FASTRATE)
  #error "DCM_UDS_RDBPI_TRANSMISSIONMODE_FASTRATE is already defined"
#endif
#define DCM_UDS_RDBPI_TRANSMISSIONMODE_FASTRATE    (0x03U)
/** \brief The server stops transmitting positive response messages send periodically/repeatedly */
#if (defined DCM_UDS_RDBPI_TRANSMISSIONMODE_STOPSENDING)
  #error "DCM_UDS_RDBPI_TRANSMISSIONMODE_STOPSENDING is already defined"
#endif
#define DCM_UDS_RDBPI_TRANSMISSIONMODE_STOPSENDING (0x04U)

/*==================[type definitions]===========================================================*/

/* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Dcm_ResponseConfirmationTypeType,1 */
/** \brief Encodes the type of the response which is transmitted */
typedef uint8 Dcm_ResponseConfirmationTypeType;

/* !LINKSTO Dcm.EB.Dcm_ServiceIDType,1 */
/** \brief Type to indicate ID of a diagnostic service */
typedef uint8 Dcm_ServiceIDType;

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
/* !LINKSTO Dcm.EB.DefaultInterface.Enabled.Dcm_DIDIDType,1 */
/** \brief Type of an unconfigured Did. */
typedef uint16 Dcm_DIDIDType;
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Initialize Dcm
 ** Service for basic initialization of Dcm module. The initialization function will initialize all
 ** Dcm global variables with the values of the configuration.
 **
 ** The Dcm supports only one configuration variant. Therefore the ConfigPtr is unused.
 **
 ** \param[in] ConfigPtr    Pointer to configuration set (unused).
 **
 ** \ServiceID{::DCM_SERVID_INIT}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DCM_CODE) Dcm_Init(
  P2CONST(Dcm_ConfigType, AUTOMATIC, DCM_APPL_CONST) ConfigPtr);

#if (DCM_VERSION_INFO_API == STD_ON)
/** \brief This function returns the version information of this module.
 **
 ** \param[out] versioninfo   Pointer to location where version info is written to
 **
 ** \ServiceID{::DCM_SERVID_GETVERSIONINFO}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DCM_CODE) Dcm_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, DCM_APPL_DATA) versioninfo);
#endif /* DCM_VERSION_INFO_API */

/* hide declaration in own header if included by DCM itself and RTE is
 * enabled */
#if (DCM_INCLUDE_RTE == STD_OFF)

/** \brief Provides active protocol.
 **
 ** This Dcm function returns the active protocol.
 **
 ** \param[out] ActiveProtocol Active diagnostic protocol (::DCM_UDS_ON_CAN, ::DCM_OBD_ON_CAN, ...)
 **
 ** \return
 **         - ::E_OK - valid call of this function
 **         - ::E_NOT_OK - if Det is enabled and a Det check fails.
 **
 ** \ServiceID{::DCM_SERVID_GETACTIVEPROTOCOL}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_GetActiveProtocol(
  P2VAR(Dcm_ProtocolType, AUTOMATIC, DCM_APPL_DATA) ActiveProtocol);

/** \brief Provides the active session control type value.
 **
 ** This function provides the active session control type value.
 **
 ** \param[out] SesCtrlType  Active session control type value content is according to
 ** "diagnosticSessionType" parameter of DiagnosticSessionControl request
 **
 ** \return
 **         - ::E_OK - valid call of this function
 **         - ::E_NOT_OK - if Det is enabled and a Det check fails
 **
 ** \ServiceID{::DCM_SERVID_GETSESCTRLTYPE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_GetSesCtrlType(
  P2VAR(Dcm_SesCtrlType, AUTOMATIC, DCM_APPL_DATA) SesCtrlType);

/** \brief Provides the active security level value.
 **
 ** This function provides the active security level value.
 **
 ** \param[out] SecLevel  Active Security Level value
 ** Conversion formula to calculate SecurityLevel out of tester requested
 ** SecurityAccessType parameter:
 ** SecurityLevel = (SecurityAccessType + 1) / 2
 ** Content of SecurityAccessType is according to "securityAccessType"
 ** parameter of SecurityAccess request
 **
 ** \return
 **         - ::E_OK - valid security level available
 **         - ::E_NOT_OK - if Det is enabled and a Det check fails
 **
 ** \ServiceID{::DCM_SERVID_GETSECURITYLEVEL}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_GetSecurityLevel(
  P2VAR(Dcm_SecLevelType, AUTOMATIC, DCM_APPL_DATA) SecLevel);

/** \brief Provides the ResetToDefaultSession interface for user.

 **  When this function is called current session will be switched to default
 **  session. If current session is default session, E_OK is returned without any session change.
 **  BswM is not informed of the transition to default session.
 **
 ** \return
 **         - ::E_OK - valid call of this function
 **         - ::E_NOT_OK - if Det is enabled and a Det check fails
 **
 ** \ServiceID{::DCM_SERVID_RESETTODEFAULTSESSION}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_ResetToDefaultSession(void);

#ifndef RTE_SCHM_SWCBSW_DCM_MAINFUNCTION
#define RTE_SCHM_SWCBSW_DCM_MAINFUNCTION
/** \brief Main loop within the DSD part of the DCM module
 **
 ** This function is used for processing the tasks of the main loop.
 **
 ** \ServiceID{::DCM_SERVID_MAINFUNCTION}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(void, DCM_CODE) Dcm_MainFunction(void);
#endif /* #ifndef RTE_SCHM_SWCBSW_DCM_MAINFUNCTION */

#endif /* #if (DCM_INCLUDE_RTE == STD_OFF) */

#if ((DCM_INCLUDE_RTE == STD_OFF) || \
     ((DCM_INCLUDE_RTE == STD_ON) && (DCM_ROE_NUM_PRECONF_EVENT == 0U)))
/** \brief Allows to notify Dcm that a event has been triggered (for service RoE).
 **
 ** When this function is called, Dcm executes the service requested in subfunction
 ** serviceToRespondTo.
 **
 ** \param[in] RoeEventId Identifier of the event that is triggered
 **
 ** \return Validity of the event identifier RoeEventId
 ** \retval E_OK - valid call of this function
 ** \retval E_NOT_OK - if any other checks fail
 **
 ** \ServiceID{::DCM_SERVID_TRIGGERONEVENT}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE)  Dcm_TriggerOnEvent(
  uint8 RoeEventId );

#endif

/** \brief Allows Dem to notify Dcm on status change of a DTC (for service RoE).
 **
 ** When this function is called, Dcm verifies the DTC against the status mask in
 ** the diagnostic request and if the verification is positive, executes the associated
 ** service (serviceToRespondTo).
 **
 ** \param[in] DTC Identifier of the DTC
 ** \param[in] DTCStatusOld Status of the DTC before the change
 ** \param[in] DTCStatusNew Status of the DTC after the change
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_DEMTRIGGERONDTC}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
 extern FUNC(Std_ReturnType, DCM_CODE)  Dcm_DemTriggerOnDTCStatus(
  uint32 DTC,
  uint8 DTCStatusOld,
  uint8 DTCStatusNew);

/** \brief Send a response
 **
 ** When this function is called, a response will be sent based on the data contained in
 ** pMsgContext. If Dcm_ExternalSetNegResponse() was called before, a negative response is sent,
 ** otherwise a positive response will be sent. The application does not have to care about
 ** any timing requirement to process a request.
 **
 ** For diagnostic experts:
 ** Between the arrival of a request (XXX_Dcm<DiagnosticService>) and finishing the corresponding
 ** response (Dcm_Processing_Done), busy-acknowledges (negative response with response code 0x78)
 ** are sent automatically.
 **
 ** \param[in] pMsgContext Message-related information for one one diagnostic protocol identifier.
 **
 ** \ServiceID{::DCM_SERVID_PROCESSINGDONE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DCM_CODE) Dcm_ExternalProcessingDone(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_APPL_DATA) pMsgContext);

#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
/** \brief Notification of Reset Mode entered
 **
 ** This provides a confirmation for a 'HardReset' request received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_HardResetModeEntry(void);

/** \brief Notification of Reset Mode entered
 **
 ** This provides a confirmation for a 'KeyOnOffReset' request received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_KeyOnOffResetModeEntry(void);

/** \brief Notification of Reset Mode entered
 **
 ** This provides a confirmation for a 'SoftReset' request received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_SoftResetModeEntry(void);

/** \brief Notification of Reset Mode entered
 **
 ** This provides a confirmation for a 'EnableRapidPowerShutdownReset' request received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_EnableRapidPowerShutdownResetModeEntry(void);

/** \brief Notification of Reset Mode entered
 **
 ** This provides a confirmation for a 'DisableRapidPowerShutdownReset' request received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DisableRapidPowerShutdownResetModeEntry(void);

/** \brief Notification of Reset Mode entered
 **
 ** This provides a confirmation for a 'BootloaderReset' request received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_BootloaderResetModeEntry(void);

/** \brief Notification of Reset Mode entered
 **
 ** This provides a confirmation for a 'SsBootloaderReset' request received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_SsBootloaderResetModeEntry(void);
#endif

#if(DCM_DSP_USE_SERVICE_0X28 == STD_ON)

/** \brief Notification of Communication Mode entered
 **
 ** This provides a confirmation for an 'EnableRxTx' request for normalCommunicationMessages
 ** received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_EnableRxTxNormModeEntry(void);

/** \brief Notification of Communication Mode entered
 **
 ** This provides a confirmation for an 'EnableRxDisableTx' request for normalCommunicationMessages
 ** received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_EnableRxDisableTxNormModeEntry(void);

/** \brief Notification of Communication Mode entered
 **
 ** This provides a confirmation for a 'DisableRxEnableTx' request for normalCommunicationMessages
 ** received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DisableRxEnableTxNormModeEntry(void);

/** \brief Notification of Communication Mode entered
 **
 ** This provides a confirmation for a 'DisableRxTx' request for normalCommunicationMessages
 ** received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DisableRxTxNormModeEntry(void);

/** \brief Notification of Communication Mode entered
 **
 ** This provides a confirmation for a 'EnableRxTx' request for
 ** networkManagementCommunicationMessages received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_EnableRxTxNmModeEntry(void);

/** \brief Notification of Communication Mode entered
 **
 ** This provides a confirmation for a 'EnableRxDisableTx' request for
 ** networkManagementCommunicationMessages received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_EnableRxDisableTxNmModeEntry(void);

/** \brief Notification of Communication Mode entered
 **
 ** This provides a confirmation for a 'DisableRxEnableTx' request for
 ** networkManagementCommunicationMessages received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DisableRxEnableTxNmModeEntry(void);

/** \brief Notification of Communication Mode entered
 **
 ** This provides a confirmation for a 'DisableRxTx' request for
 ** networkManagementCommunicationMessages received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DisableRxTxNmModeEntry(void);

/** \brief Notification of Communication Mode entered
 **
 ** This provides a confirmation for a 'EnableRxTx' request for
 ** networkManagementCommunicationMessages and normalCommunicationMessages received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_EnableRxTxNormNmModeEntry(void);

/** \brief Notification of Communication Mode entered
 **
 ** This provides a confirmation for a 'EnableRxDisableTx' request for
 ** networkManagementCommunicationMessages and normalCommunicationMessages received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_EnableRxDisableTxNormNmModeEntry(void);

/** \brief Notification of Communication Mode entered
 **
 ** This provides a confirmation for a 'DisableRxEnableTx' request for
 ** networkManagementCommunicationMessages and normalCommunicationMessages received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DisableRxEnableTxNormNmModeEntry(void);

/** \brief Notification of Communication Mode entered
 **
 ** This provides a confirmation for a 'DisableRxTx' request for
 ** networkManagementCommunicationMessages and normalCommunicationMessages received from tester
 **
 ** \return Always return E_OK.
 **
 ** \ServiceID{::DCM_SERVID_COMMON_COMM_MODE_ENTRY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DisableRxTxNormNmModeEntry(void);

#endif /* #if(DCM_DSP_USE_SERVICE_0X28 == STD_ON) */

#if (DCM_USE_API_CALLER_GENERIC_SVCH_IMPLEMENTATION == STD_ON)
/** \brief Caller of internally implemented SvcH
 **
 ** This API provides a correct context for the internally implemented SvcH to process the request.
 **
 ** \param [in]     OpStatus    Operation state passed by External Service Handler to internal SvcH
 ** \param [in,out] pMsgContext Contextual information message passed to a diagnostic protocol
 **
 ** \return code of API
 ** \retval return_code_of_internally_implemented_SvcH if internal SvcH exists
 ** \retval DCM_E_SERVICENOTSUPPORTED if internal SvcH does not exists
 **
 ** \ServiceID{::DCM_API_CALLER_GENERIC_SVCH_IMPLEMENTATION_ID}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_APICaller_GenericSvcHImplementation
(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
);
#endif /* #if (DCM_USE_API_CALLER_GENERIC_SVCH_IMPLEMENTATION == STD_ON) */

#if (DCM_DSL_APPLICATION_DIAGNOSTIC_REQUESTS == STD_ON)
/** \brief Injector Interface function to CDD
 **
 ** This API allows the caller to directly inject a Service request for DCM processing.
 **
 ** \param [in] RxPduID       Reception Pdu Identifier of the PDU where the request
 **                           is inteded for processing.
 ** \param [in] RequestData   Pointer to data array where the request intended for processing
 **                           is stored.
 ** \param [in] RequestLength Lenght of the request intended for processing.
 **
 ** \return Result of the injection attempt.
 ** \retval BUFREQ_OK The injection has been accepted and will be processed.
 ** \retval BUFREQ_E_BUSY A request for injection is already ongoing.
 ** \retval BUFREQ_E_OVFL The request is too long and cannot be processed.
 ** \retval BUFREQ_E_NOT_OK The provided RxPduId is invalid.
 **
 */
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_InjectDiagnosticRequest
(
  PduIdType RxPduID,
  P2CONST(uint8, AUTOMATIC, DCM_VAR) RequestData,
  PduLengthType RequestLength
);
#endif /* #if (DCM_DSL_APPLICATION_DIAGNOSTIC_REQUESTS == STD_ON) */

/** \brief Provides the active session control type value.
 **
 ** This function provides the active session control type value and it is visible all the time.
 **
 ** \param[out] SesCtrlType  Active session control type value content is according to
 ** "diagnosticSessionType" parameter of DiagnosticSessionControl request
 **
 ** \return
 **         - ::E_OK - valid call of this function
 **         - ::E_NOT_OK - if Det is enabled and a Det check fails
 **
 ** \ServiceID{::DCM_SERVID_GETSESCTRLTYPE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_GetSesCtrlTypeExt(
  P2VAR(Dcm_SesCtrlType, AUTOMATIC, DCM_APPL_DATA) SesCtrlType);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*------------------[Callouts functions]-----------------------------*/

#define DCM_START_SEC_CALLOUT_CODE
#include <Dcm_MemMap.h>

#if (DCM_DSP_USE_SERVICE_0X34 == STD_ON)
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
/* !LINKSTO Dcm.EB.ProcessRequestDownload.AUTOSAR403,1 */
/** \brief Callout for RequestDownload
 **
 **  This callout allows the user to implement the negotiation of a data transfer
 ** from the tester to the ECU.
 **
 ** \param[in]    OpStatus Operation status
 ** \param[in]    DataFormatIdentifier Format identifier from the request
 ** \param[in]    MemoryAddress Memory address to download
 ** \param[in]    MemorySize Size of memory to be downloaded
 ** \param[in]    BlockLength Maximum number of bytes for a write operation
 ** \param[out]   ErrorCode NRC if the request for download fails.
 **
 ** \return E_OK, E_NOT_OK or DCM_E_PENDING.
 **
 ** \ServiceID{::DCM_SVCID_REQUESTDOWNLOAD}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) Dcm_ProcessRequestDownload(
  Dcm_OpStatusType  OpStatus,
  uint8 DataFormatIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  uint32 BlockLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
#else
/* !LINKSTO Dcm.EB.ProcessRequestDownload.AUTOSAR430,2 */
/** \brief Callout for RequestDownload
 **
 **  This callout allows the user to implement the negotiation of a data transfer
 ** from the tester to the ECU.
 **
 ** \param[in]    OpStatus Operation status
 ** \param[in]    DataFormatIdentifier Format identifier from the request
 ** \param[in]    MemoryAddress Memory address to download
 ** \param[in]    MemorySize Size of memory to be downloaded
 ** \param[out]   BlockLength Maximum number of bytes for a write operation
 ** \param[out]   ErrorCode NRC if the request for download fails.
 **
 ** \return E_OK, E_NOT_OK or DCM_E_PENDING.
 **
 ** \ServiceID{::DCM_SVCID_REQUESTDOWNLOAD}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) Dcm_ProcessRequestDownload(
  Dcm_OpStatusType  OpStatus,
  uint8 DataFormatIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2VAR(uint32, AUTOMATIC, DCM_APPL_DATA) BlockLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/
#endif /*(DCM_DSP_USE_SERVICE_0X34 == STD_ON)*/

#if (DCM_DSP_USE_SERVICE_0X35 == STD_ON)
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)

/* !LINKSTO Dcm.EB.ProcessRequestUpload.AUTOSAR403,1 */
/** \brief Callout for RequestUpload.
 **
 **  This callout allows the user to implement the negotiation of a data transfer
 ** from the ECU to the tester.
 **
 ** \param[in]    OpStatus Operation status
 ** \param[in]    DataFormatIdentifier Format identifier from the request
 ** \param[in]    MemoryAddress Memory address to upload
 ** \param[in]    MemorySize Size of memory to be uploaded
 ** \param[out]   ErrorCode NRC if the request for upload fails.
 **
 ** \return E_OK, E_NOT_OK or DCM_E_PENDING.
 **
 ** \ServiceID{::DCM_SVCID_REQUESTUPLOAD}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) Dcm_ProcessRequestUpload(
  Dcm_OpStatusType  OpStatus,
  uint8 DataFormatIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
#else
/* !LINKSTO Dcm.EB.ProcessRequestUpload.AUTOSAR430,1 */
/** \brief Callout for RequestUpload.
 **
 **  This callout allows the user to implement the negotiation of a data transfer
 ** from the ECU to the tester.
 **
 ** \param[in]    OpStatus Operation status
 ** \param[in]    DataFormatIdentifier Format identifier from the request
 ** \param[in]    MemoryAddress Memory address to upload
 ** \param[in]    MemorySize Size of memory to be uploaded
 ** \param[out]   BlockLength Maximum number of bytes for a read operation
 ** \param[out]   ErrorCode NRC if the request for upload fails.
 **
 ** \return E_OK, E_NOT_OK or DCM_E_PENDING.
 **
 ** \ServiceID{::DCM_SVCID_REQUESTUPLOAD}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) Dcm_ProcessRequestUpload(
  Dcm_OpStatusType  OpStatus,
  uint8 DataFormatIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2VAR(uint32, AUTOMATIC, DCM_APPL_DATA) BlockLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/
#endif /*(DCM_DSP_USE_SERVICE_0X35 == STD_ON)*/

/* !LINKSTO Dcm.EB.ReadMemory.WriteMemory.CallbacksNotDefined,1 */
#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X2C == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X36 == STD_ON))

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)

/* !LINKSTO Dcm.EB.ReadMemory.AUTOSAR403,2 */
/** \brief Callout for upload of data (used for service TransferData).
 **
 ** This callout allows the user to transfer data from the ECU to the tester, after
 ** successful execution of a RequestUpload service.
 **
 ** \param[in]    OpStatus Operation status
 ** \param[in]    MemoryIdentifier Identifier of the memory device (optional)
 ** \param[in]    MemoryAddress Start address of the memory from which data is to be retrieved
 ** \param[in]    MemorySize Number of bytes to be read
 ** \param[out]   MemoryData Pointer to the buffer in Dcm
 **
 ** \return DCM_READ_OK, DCM_READ_PENDING or DCM_READ_FAILED.
 **
 ** \ServiceID{::DCM_SVCID_READMEMORY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Dcm_ReturnReadMemoryType, DCM_APPL_CODE) Dcm_ReadMemory(
  Dcm_OpStatusType  OpStatus,
  uint8 MemoryIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) MemoryData);
#else
/* !LINKSTO Dcm.EB.ReadMemory.AUTOSAR430,1 */
/** \brief Callout for upload of data (used for service TransferData).
 **
 ** This callout allows the user to transfer data from the ECU to the tester, after
 ** successful execution of a RequestUpload service.
 **
 ** \param[in]    OpStatus Operation status
 ** \param[in]    MemoryIdentifier Identifier of the memory device (optional)
 ** \param[in]    MemoryAddress Start address of the memory from which data is to be retrieved
 ** \param[in]    MemorySize Number of bytes to be read
 ** \param[out]   MemoryData Pointer to the buffer in Dcm
 ** \param[out]   ErrorCode If the operation Dcm_ReadMemory returns value
 **                         DCM_READ_FAILED, the Dcm module shall send a
 **                         negative response with NRC code equal to the parameter
 **                         ErrorCode parameter value.
 **
 ** \return DCM_READ_OK, DCM_READ_PENDING, DCM_READ_FAILED or DCM_READ_FORCE_RCRRP.
 **
 ** \ServiceID{::DCM_SVCID_READMEMORY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Dcm_ReturnReadMemoryType, DCM_APPL_CODE) Dcm_ReadMemory(
  Dcm_OpStatusType  OpStatus,
  uint8 MemoryIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) MemoryData,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/
#endif/*#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) || \
             (DCM_DSP_USE_SERVICE_0X2C == STD_ON) || \
             (DCM_DSP_USE_SERVICE_0X36 == STD_ON))*/

#if ((DCM_DSP_USE_SERVICE_0X3D == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X36 == STD_ON))

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)

/* !LINKSTO Dcm.EB.WriteMemory.AUTOSAR403,1 */
/** \brief Callout for download of data (used for service TransferData).
 **
 ** This callout allows the user to transfer data from the tester to the ECU, after
 ** successful execution of a RequestDownload service.
 **
 ** \param[in]     OpStatus Operation status
 ** \param[in]     MemoryIdentifier Identifier of the memory device (optional)
 ** \param[in]     MemoryAddress Start address of the memory from which data is to be retrieved
 ** \param[in]     MemorySize Number of bytes to be read
 ** \param[in]     MemoryData Pointer to the buffer in Dcm
 **
 ** \return DCM_WRITE_OK, DCM_WRITE_PENDING, DCM_WRITE_FAILED or DCM_WRITE_FORCE_RCRRP.
 **
 ** \ServiceID{::DCM_SVCID_WRITEMEMORY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Dcm_ReturnWriteMemoryType, DCM_APPL_CODE) Dcm_WriteMemory(
  Dcm_OpStatusType  OpStatus,
  uint8 MemoryIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) MemoryData);

#elif (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_422)

/* !LINKSTO Dcm.EB.WriteMemory.AUTOSAR422,1 */
/** \brief Callout for download of data (used for service TransferData).
 **
 ** This callout allows the user to transfer data from the tester to the ECU, after
 ** successful execution of a RequestDownload service.
 **
 ** \param[in]     OpStatus Operation status
 ** \param[in]     MemoryIdentifier Identifier of the memory device (optional)
 ** \param[in]     MemoryAddress Start address of the memory from which data is to be retrieved
 ** \param[in]     MemorySize Number of bytes to be read
 ** \param[in]     MemoryData Pointer to the buffer in Dcm
 ** \param[out]    ErrorCode If the operation Dcm_WriteMemory returns value
 **                          DCM_WRITE_FAILED, the Dcm module shall send a
 **                          negative response with NRC code equal to the parameter
 **                          ErrorCode parameter value.
 **
 ** \return DCM_WRITE_OK, DCM_WRITE_PENDING, DCM_WRITE_FAILED or DCM_WRITE_FORCE_RCRRP
 **
 ** \ServiceID{::DCM_SVCID_WRITEMEMORY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Dcm_ReturnWriteMemoryType, DCM_APPL_CODE) Dcm_WriteMemory(
  Dcm_OpStatusType  OpStatus,
  uint8 MemoryIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) MemoryData,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

#else
/* !LINKSTO Dcm.EB.WriteMemory.AUTOSAR430,1 */
/** \brief Callout for download of data (used for service TransferData).
 **
 ** This callout allows the user to transfer data from the tester to the ECU, after
 ** successful execution of a RequestDownload service.
 **
 ** \param[in]     OpStatus Operation status
 ** \param[in]     MemoryIdentifier Identifier of the memory device (optional)
 ** \param[in]     MemoryAddress Start address of the memory from which data is to be retrieved
 ** \param[in]     MemorySize Number of bytes to be read
 ** \param[in]     MemoryData Pointer to the buffer in Dcm
 ** \param[out]    ErrorCode If the operation Dcm_WriteMemory returns value
 **                          DCM_WRITE_FAILED, the Dcm module shall send a
 **                          negative response with NRC code equal to the parameter
 **                          ErrorCode parameter value.
 **
 ** \return DCM_WRITE_OK, DCM_WRITE_PENDING, DCM_WRITE_FAILED or DCM_WRITE_FORCE_RCRRP
 **
 ** \ServiceID{::DCM_SVCID_WRITEMEMORY}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Dcm_ReturnWriteMemoryType, DCM_APPL_CODE) Dcm_WriteMemory(
  Dcm_OpStatusType  OpStatus,
  uint8 MemoryIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2CONST(uint8, AUTOMATIC, DCM_VAR) MemoryData,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/
#endif /*#if ((DCM_DSP_USE_SERVICE_0X3D == STD_ON) || \
             (DCM_DSP_USE_SERVICE_0X36 == STD_ON))*/

#if (DCM_DSP_USE_SERVICE_0X37 == STD_ON)
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)

/* !LINKSTO Dcm.EB.RequestTransferExit.AUTOSAR403,1 */
/** \brief Callout for termination of an upload or download process.
 **
 ** This callout allows the user to terminate the data transfer between the ECU and the tester.
 **
 ** \param[in]    OpStatus Operation status
 ** \param[in]    ParameterRecord Pointer to vehicle/manufacturer specific data (optional)
 ** \param[in]    ParameterRecordSize Length of ParameterRecord in bytes (optional)
 ** \param[out]   ErrorCode NRC if the request for termination fails.
 **
 ** \return E_OK, E_NOT_OK or DCM_E_PENDING.
 **
 ** \ServiceID{::DCM_SVCID_REQUESTTRANSFEREXIT}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) Dcm_ProcessRequestTransferExit(
  Dcm_OpStatusType  OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) ParameterRecord,
  uint32 ParameterRecordSize,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

#elif (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_422)

/* !LINKSTO Dcm.EB.RequestTransferExit.AUTOSAR422,1 */
/** \brief Callout for termination of an upload or download process.
 **
 ** This callout allows the user to terminate the data transfer between the ECU and the tester.
 **
 ** \param[in]    OpStatus Operation status
 ** \param[in]    transferRequestParameterRecord Pointer to vehicle/manufacturer specific data
 ** \param[in]    transferRequestParameterRecordSize Length of ParameterRecord in bytes
 ** \param[inout] transferResponseParameterRecordSize When the function is called this parameter
 **                                                   contains the maximum number of data bytes
 **                                                   that can be written to the buffer. The
 **                                                  function returns the actual number of written
 **                                                  data bytes in transferResponseParameterRecord
 ** \param[out]   transferResponseParameterRecord Pointer to vehicle-manufacturer-specific data
 ** \param[out]   ErrorCode NRC if the request for termination fails.
 **
 ** \return E_OK, E_NOT_OK or DCM_E_PENDING.
 **
 ** \ServiceID{::DCM_SVCID_REQUESTTRANSFEREXIT}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) Dcm_ProcessRequestTransferExit(
  Dcm_OpStatusType  OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) transferRequestParameterRecord,
  uint32 transferRequestParameterRecordSize,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) transferResponseParameterRecord,
  P2VAR(uint32, AUTOMATIC, DCM_VAR) transferResponseParameterRecordSize,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

#else

/* !LINKSTO Dcm.EB.RequestTransferExit.AUTOSAR430,1 */
/** \brief Callout for termination of an upload or download process.
 **
 ** This callout allows the user to terminate the data transfer between the ECU and the tester.
 **
 ** \param[in]    OpStatus Operation status
 ** \param[in]    transferRequestParameterRecord Pointer to vehicle/manufacturer specific data
 ** \param[in]    transferRequestParameterRecordSize Length of ParameterRecord in bytes
 ** \param[inout] transferResponseParameterRecordSize When the function is called this parameter
 **                                                   contains the maximum number of data bytes
 **                                                   that can be written to the buffer. The
 **                                                  function returns the actual number of written
 **                                                  data bytes in transferResponseParameterRecord
 ** \param[out]   transferResponseParameterRecord Pointer to vehicle-manufacturer-specific data
 ** \param[out]   ErrorCode NRC if the request for termination fails.
 **
 ** \return E_OK, E_NOT_OK or DCM_E_PENDING.
 **
 ** \ServiceID{::DCM_SVCID_REQUESTTRANSFEREXIT}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_APPL_CODE) Dcm_ProcessRequestTransferExit(
  Dcm_OpStatusType  OpStatus,
  P2CONST(uint8, AUTOMATIC, DCM_VAR) transferRequestParameterRecord,
  uint32 transferRequestParameterRecordSize,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) transferResponseParameterRecord,
  P2VAR(uint32, AUTOMATIC, DCM_VAR) transferResponseParameterRecordSize,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

#endif /*(DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)*/
#endif /*(DCM_DSP_USE_SERVICE_0X37 == STD_ON)*/

/** \brief Callout for storing the program conditions.
 **
 ** This callout allows the user to store the programming environment in a
 ** non-volatile memory before jumping to bootloader.
 **
 ** \param[in]  ProgConditions Pointer to the structure which holds the program conditions
 **
 ** \return E_OK, E_NOT_OK or DCM_E_PENDING.
 **
 ** \ServiceID{::DCM_SVCID_SETPROGCONDITIONS}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_SetProgConditions(
  P2VAR(Dcm_ProgConditionsType, AUTOMATIC, DCM_VAR) ProgConditions);

/** \brief Callout for retrieving the stored program conditions.
 **
 ** This callout allows the user to retrieve the stored programming environment from the
 ** non-volatile memory after jumping from bootloader.
 **
 ** \param[out]  ProgConditions Pointer to the structure which holds the program conditions
 **
 ** \return DCM_COLD_START or DCM_WARM_START.
 **
 ** \ServiceID{::DCM_SVCID_GETPROGCONDITIONS}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Dcm_EcuStartModeType, DCM_CODE) Dcm_GetProgConditions(
  P2VAR(Dcm_ProgConditionsType, AUTOMATIC, DCM_VAR) ProgConditions);

/** \brief Send a negative response instead of a positive response
 ** The negative response code is taken from the first call - duplicate calls are allowed but the
 ** errorCode transmitted is ignored. The transmission of the negative response will be performed
 ** after emiting PROC_DONE.
 **
 ** \param[in] pMsgContext Message-related information for one diagnostic protocol identifier.
 **
 ** \param[in] errorCode Error code to be set. This error code will be taken as negative response
 ** code (NRC) (see description).(::DCM_E_GENERALREJECT, ::DCM_E_SERVICENOTSUPPORTED,
 ** ::DCM_E_SUBFUNCTIONNOTSUPPORTED, ::DCM_E_REQUESTCORRECTLYRECEIVEDRESPONSEPENDING, ...)
 **
 ** \ServiceID{::DCM_SERVID_SETNEGRESPONSE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DCM_CODE) Dcm_ExternalSetNegResponse(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_APPL_DATA) pMsgContext,
  Dcm_NegativeResponseCodeType errorCode);

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
/** \brief Confirm (un)successful transmission
 **
 ** This function is provided by the upper layer (application).
 **
 ** \param[in] ServiceID Service ID of diagnostic request which triggered the response transmission.
 ** \param[in] MsgContext Pointer to the MessageContext of the service handler which triggered
 ** the response transmission.
 ** \param[in] ResponseConfirmationType The type of response which was confirmed.
 ** \param[in] ResponseSuppressed TRUE if the response was suppressed (either as a result of the
 ** suppressPositiveresponse bit being set or otherwise).FALSE otherwise.
 ** \param[in] TransmissionSuccessful TRUE if the confirmation of transmission was positive. FALSE otherwise.
 **
 ** \ServiceID{::DCM_SERVID_APPLICATIONTRANSMISSIONCONFIRMATION}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
/* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Dcm_ApplicationTransmissionConfirmation,1 */
extern FUNC(void, DCM_APPL_CODE) Dcm_ApplicationTransmissionConfirmation(
  Dcm_ServiceIDType ServiceID, Dcm_MsgContextPtrType MsgContext,
  Dcm_ResponseConfirmationTypeType ResponseConfirmationType,
  boolean ResponseSuppressed, boolean TransmissionSuccessful);
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
/** \brief Function called on reception of UDS Service  ReadDataByIdentifier (0x22)
 ** for an unsupported DID when DcmDspRoutineEnableDefaultInterfaces configuration parameter is set to TRUE.
 **
 ** Default operation for reading unconfigured DIDs. Unconfigured DIDs shall be considered as being composed of
 ** a single asynchronously-handled signal. The Application may perform the operation for the requested DID or return an
 ** adequate NegativeResponseCode. For ReadDataByIdentifier (0x22) requests comprising more than one DID, returning the
 ** RequestOutOfRange (0x31) NRC shall advance reading to the next DID if existing, as with regular DIDs.
 **
 ** This callout allows the user to define the functionality of the ReadDataByIdentifier (0x22)
 **
 ** \param[in] OpStatus Operation Status: DCM_INITIAL - Initial call.
 **                                       DCM_PENDING - Subsequent call after a previous DCM_E_PENDING return.
 **                                       DCM_CANCEL - Final call for cancellation of operation. The Dcm ignores the
 **                                                    return value for this call.
 ** \param[in] DID 2-byte DID identifier.
 ** \param[out] DestinationBuffer Pointer to the location in the output buffer where DID data is written.
 ** \param[out] Length pointer to which the application shall write the amount of data, in bytes, which
 ** was written to the DestinationBuffer pointer.
 ** \param[out] Nrc pointer to which the application shall write the NRC to be returned or whether
 ** the DID is supported or not. Valid only if the return value is E_NOT_OK.
 **
 ** \return Permission of application
 **         - ::E_OK: Request was successful.
 **         - ::E_NOT_OK: Request was not successful.
 **         - ::DCM_E_PENDING: Request is not yet finished. Further call(s) required to finish.
 **
 ** \ServiceID{::DCM_SERVID_DEFAULTDIDREAD_DEFAULTINTERFACE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
/* !LINKSTO Dcm.Dsn.IB.DidServices.ReadDid.DefaultRead,1 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DefaultDIDRead(
  Dcm_OpStatusType OpStatus,
  Dcm_DIDIDType DID,
  uint8 *DestinationBuffer,
  uint32 *Length,
  Dcm_NegativeResponseCodeType *Nrc);

/** \brief Function called on reception of UDS Service  WriteDataByIdentifier (0x2E)
 ** for an unsupported DID when DcmDspRoutineEnableDefaultInterfaces configuration parameter is set to TRUE.
 **
 ** Default operation for writing unconfigured DIDs. Unconfigured DIDs shall be considered as being composed of
 ** a single asynchronously-handled signal. The Application may perform the operation for the requested DID or return an
 ** adequate NegativeResponseCode.
 **
 ** This callout allows the user to define the functionality of the WriteDataByIdentifier (0x2E)
 **
 ** \param[in] OpStatus Operation Status: DCM_INITIAL - Initial call.
 **                                       DCM_PENDING - Subsequent call after a previous DCM_E_PENDING return.
 **                                       DCM_CANCEL - Final call for cancellation of operation. The Dcm ignores the
 **                                                    return value for this call.
 ** \param[in] DID 2-byte DID identifier.
 ** \param[in] Length The valid length, in bytes, of the SourceBuffer. Represents the length, in bytes, of the
 ** received dataRecord from the WriteDataByIdentifier (0x2E) service request.
 ** \param[out] SourceBuffer Pointer to the location in the input buffer where DID data is read from.
 ** \param[out] Nrc Pointer to which the application shall write the NRC to be returned or whether the DID is supported
 ** or not. Valid only if the return value is E_NOT_OK.
 **
 ** \return Permission of application
 **         - ::E_OK: Request was successful.
 **         - ::E_NOT_OK: Request was not successful.
 **         - ::DCM_E_PENDING: Request is not yet finished. Further call(s) required to finish.
 **
 ** \ServiceID{::DCM_SERVID_DEFAULTDIDWRITE_DEFAULTINTERFACE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
/* !LINKSTO Dcm.Dsn.IB.DidServices.WriteDid.DefaultWrite,1 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DefaultDIDWrite(
  Dcm_OpStatusType OpStatus,
  Dcm_DIDIDType DID,
  uint8 *SourceBuffer,
  uint32 Length,
  Dcm_NegativeResponseCodeType *Nrc);

/** \brief Function called on reception of UDS Service InputOutputControlByIdentifier (0x2F)
 ** with InputOutputControlParameter equal to resetToDefault, when DcmDspRoutineEnableDefaultInterfaces configuration
 ** parameter is set to TRUE.
 **
 ** Default operation for resetting unconfigured DIDs to default control. Unconfigured DIDs shall be
 ** considered as being composed of a single asynchronously-handled signal. The Application may perform the opertation
 ** for the requested DID or return an adequate NegativeResponseCode.
 **
 ** This callout allows the user to define the functionality of the resetToDefault sub-service.
 ** \param[in] OpStatus Operation Status: DCM_INITIAL - Initial call.
 **                                       DCM_PENDING - Subsequent call after a previous DCM_E_PENDING return.
 **                                       DCM_CANCEL - Final call for cancellation of operation. The Dcm ignores the
 **                                                    return value for this call.
 ** \param[in] DID 2-byte DID identifier.
 ** \param[out] Nrc Pointer to which the application shall write the NRC to be returned or whether the DID is supported
 ** or not. Valid only if the return value is E_NOT_OK.
 **
 ** \return Permission of application
 **         - ::E_OK: Request was successful.
 **         - ::E_NOT_OK: Request was not successful.
 **         - ::DCM_E_PENDING: Request is not yet finished. Further call(s) required to finish.
 **
 ** \ServiceID{::DCM_SERVID_DEFAULTDIDRESETTODEFAULT_DEFAULTINTERFACE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DefaultDIDResetToDefault(
  Dcm_OpStatusType OpStatus,
  Dcm_DIDIDType DID,
  Dcm_NegativeResponseCodeType *Nrc);

/** \brief Function called on reception of UDS Service InputOutputControlByIdentifier (0x2F)
 ** with InputOutputControlParameter equal to returnControlToECU, when DcmDspRoutineEnableDefaultInterfaces configuration
 ** parameter is set to TRUE.
 **
 ** This callout allows the user to define the functionality of the returnControlToECU sub-service.
 **
 ** Default operation for returning unconfigured DIDs to ECU control. Unconfigured DIDs shall be considered as
 ** being composed of a single asynchronously-handled signal. The Application may perform the opertation for the requested
 ** DID or return an adequate NegativeResponseCode.
 **
 ** \param[in] OpStatus Operation Status: DCM_INITIAL - Initial call.
 **                                       DCM_PENDING - Subsequent call after a previous DCM_E_PENDING return.
 **                                       DCM_CANCEL - Final call for cancellation of operation. The Dcm ignores the
 **                                                    return value for this call.
 ** \param[in] DID 2-byte DID identifier.
 ** \param[out] Nrc Pointer to which the application shall write the NRC to be returned or whether the DID is supported
 ** or not. Valid only if the return value is E_NOT_OK.
 **
 ** \return Permission of application
 **         - ::E_OK: Request was successful.
 **         - ::E_NOT_OK: Request was not successful.
 **         - ::DCM_E_PENDING: Request is not yet finished. Further call(s) required to finish.
 **
 ** \ServiceID{::DCM_SERVID_DEFAULTDIDRETURNCONTROLTOECU_DEFAULTINTERFACE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DefaultDIDReturnControlToECU(
  Dcm_OpStatusType OpStatus,
  Dcm_DIDIDType DID,
  Dcm_NegativeResponseCodeType *Nrc);

/** \brief Return Control to ECU for All Input Output Controls.
 **
 ** This callout allows the user to define the functionality of the ReturnControlToECUForAll functionality.
 **
 ** Default operation for returning all unconfigured DIDs to ECU control. To be used for the ReturnControlToECU
 ** operation at Session switch to DefaultSession since the Dcm cannot decide which unconfigured DIDs are under control.
 **
 ** \param[in] OpStatus Operation Status: DCM_INITIAL - Initial call.
 **                                       DCM_PENDING - Subsequent call after a previous DCM_E_PENDING return.
 **                                       DCM_CANCEL - Final call for cancellation of operation. The Dcm ignores the
 **                                                    return value for this call.
 **
 ** \return Permission of application
 **         - ::E_OK: Request was successful.
 **         - ::E_NOT_OK: Request was not successful.
 **         - ::DCM_E_PENDING: Request is not yet finished. Further call(s) required to finish.
 **
 ** \ServiceID{::DCM_SERVID_DEFAULTDIDRETURNCONTROLTOECUFORALL_DEFAULTINTERFACE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DefaultDIDReturnControlToECUForAll(
  Dcm_OpStatusType OpStatus);

/** \brief Function called on reception of UDS Service InputOutputControlByIdentifier (0x2F)
 ** with InputOutputControlParameter equal to freezeCurrentState, when DcmDspRoutineEnableDefaultInterfaces
 ** configuration parameter is set to TRUE.
 **
 ** Default operation for freezing unconfigured DIDs to current control state. Unconfigured DIDs shall be
 ** considered as being composed of a single asynchronously-handled signal. The Application may perform
 ** the opertation for the requested DataIdentifier (DID) or return an adequate Negative Response Code.
 **
 ** This callout allows the user to define the functionality of the freezeCurrentState sub-service.
 **
 ** \param[in] OpStatus Operation Status: DCM_INITIAL - Initial call.
 **                                       DCM_PENDING - Subsequent call after a previous DCM_E_PENDING return.
 **                                       DCM_CANCEL - Final call for cancellation of operation. The Dcm ignores the
 **                                                    return value for this call.
 ** \param[in] DID 2-byte DID identifier.
 ** \param[out] Nrc Pointer to which the application shall write the NRC to be returned or whether the DID is supported
 ** or not. Valid only if the return value is E_NOT_OK.
 **
 ** \return Permission of application
 **         - ::E_OK: Request was successful.
 **         - ::E_NOT_OK: Request was not successful.
 **         - ::DCM_E_PENDING: Request is not yet finished. Further call(s) required to finish.
 **
 ** \ServiceID{::DCM_SERVID_DEFAULTDIDFREEZECURRENTSTATE_DEFAULTINTERFACE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DefaultDIDFreezeCurrentState(
  Dcm_OpStatusType OpStatus,
  Dcm_DIDIDType DID,
  Dcm_NegativeResponseCodeType *Nrc);

/** \brief Function called on reception of UDS Service InputOutputControlByIdentifier (0x2F)
 ** with InputOutputControlParameter equal to shortTermAdjustment, when DcmDspRoutineEnableDefaultInterfaces
 ** configuration parameter is set to TRUE.
 **
 ** This callout allows the user to define the functionality of the shortTermAdjustment sub-service.
 **
 ** Default operation for short term adjustment of control state of unconfigured DIDs. Unconfigured DIDs
 ** shall be considered as being composed of a single asynchronously-handled signal. The Application may perform the
 ** opertation for the requested DID or return an adequate NegativeResponseCode.
 **
 ** \param[in] OpStatus Operation Status: DCM_INITIAL - Initial call.
 **                                       DCM_PENDING - Subsequent call after a previous DCM_E_PENDING return.
 **                                       DCM_CANCEL - Final call for cancellation of operation. The Dcm ignores the
 **                                                    return value for this call.
 ** \param[in] DID 2-byte DID identifier.
 ** \param[in] Length - the valid length, in bytes, of the controlState. Represents the length, in bytes, of the
 ** received controlOptionRecord and controlMask from the InputOutputControlByIdentifier
 ** (0x2F) service request.
 ** \param[out] controlState Pointer to the location in the input buffer where the controlState (containing the
 ** controlOptionRecord and controlMask) is located.
 ** \param[out] Nrc Pointer to which the application shall write the NRC to be returned or whether the DID is supported
 ** or not. Valid only if the return value is E_NOT_OK.
 **
 ** \return Permission of application
 **         - ::E_OK: Request was successful.
 **         - ::E_NOT_OK: Request was not successful.
 **         - ::DCM_E_PENDING: Request is not yet finished. Further call(s) required to finish.
 **
 ** \ServiceID{::DCM_SERVID_DEFAULTDIDSHORTTERMADJUSTMENT_DEFAULTINTERFACE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DefaultDIDShortTermAdjustment(
  Dcm_OpStatusType OpStatus,
  Dcm_DIDIDType DID,
  uint8 *controlState,
  uint32 Length,
  Dcm_NegativeResponseCodeType *Nrc);
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

#define DCM_STOP_SEC_CALLOUT_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_H */
/*==================[end of file]================================================================*/
