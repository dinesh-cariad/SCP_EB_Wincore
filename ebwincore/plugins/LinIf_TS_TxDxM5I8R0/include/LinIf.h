/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef LINIF_H
#define LINIF_H

/*==================[inclusions]=============================================*/

#include <ComStack_Types.h>         /* communication types, e.g. PduIdType */

#include <LinIf_Cfg.h>              /* DET + TP ON/OFF configuration */
#include <LinIf_Version.h>
#include <LinIf_Types.h>            /* LinIf basic types */
#include <LinTp_Types.h>            /* LinTp basic types */

#ifdef LINIF_WAKEUP_REV2_NOT_USED
#include <EcuM.h>                   /* EcuM_WakeupSourceType */
#endif /* LINIF_WAKEUP_REV2_NOT_USED */

#if (LINIF_PBCFGM_SUPPORT_ENABLED == STD_ON)

#define PBCFGM_NO_CFG_REQUIRED
#include <PbcfgM.h>                 /* Post build configuration manager */

#else /* (LINIF_PBCFGM_SUPPORT_ENABLED == STD_ON) */

#ifndef LINIF_NO_PBCFG_REQUIRED
#include <LinIf_PBcfg.h>            /* Post build time configurable parameters */
#endif /*(!define LINIF_NO_PBCFG_REQUIRED) */

#endif /* (LINIF_PBCFGM_SUPPORT_ENABLED == STD_ON) */


#if (LINIF_TP_SUPPORTED == STD_ON)

#if (LINTP_PBCFGM_SUPPORT_ENABLED == STD_ON)

#ifndef PBCFGM_NO_CFG_REQUIRED
#define PBCFGM_NO_CFG_REQUIRED
#endif /* PBCFGM_NO_CFG_REQUIRED */
#include <PbcfgM.h>                 /* Post build configuration manager */

#else /* (LINTP_PBCFGM_SUPPORT_ENABLED == STD_ON) */

#ifndef LINIF_NO_PBCFG_REQUIRED
#include <LinTp_PBcfg.h>
#endif /*(!define LINIF_NO_PBCFG_REQUIRED) */

#endif /* (LINTP_PBCFGM_SUPPORT_ENABLED == STD_ON) */

#endif /* (LINIF_TP_SUPPORTED == STD_ON) */


/*==================[macros]=================================================*/

/** \brief Null schedule identification */
#if (defined LINIF_NULL_SCHEDULE)
#error LINIF_NULL_SCHEDULE already defined!
#endif /* #if (defined LINIF_NULL_SCHEDULE)*/
#define LINIF_NULL_SCHEDULE   0U

/*==================[external function definitions]=========================*/

#define LINIF_START_SEC_CODE
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

#if (LINIF_VERSION_INFO_API == STD_ON)
/** \brief Return version Information
 **
 ** \param[out] versioninfo Version information are written to this variable.
 **
 ** \ServiceID{0x03}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, LINIF_CODE) LinIf_GetVersionInfo
(
    P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_DATA) versioninfo
);
#endif


/** \brief Validate configuration
 **
 ** Checks if the post build configuration fits to the link time configuration part.
 **
 ** \return E_OK if the given module configurations is valid otherwise E_NOT_OK.
 **
 ** \ServiceID{0x62}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_IsValidConfig
(
    P2CONST(void, AUTOMATIC, LINIF_APPL_CONST) voidConfigPtr
);


/** \brief Initialize module
 **
 ** This function initializes the LIN Interface
 **
 ** \param[in] ConfigPtr Not used.
 **
 ** \ServiceID{0x01}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, LINIF_CODE) LinIf_Init
(
    P2CONST(LinIf_ConfigType, AUTOMATIC, LINIF_APPL_CONST) ConfigPtr
);


/** \brief Schedule transmission of a sporadic frame
 **
 ** This function schedules sporadic frames for transmission.
 **
 ** \param[in] LinTxPduId The PDU Id of the sporadic frame to be sent.
 ** \param[in] PduInfoPtr Not used.
 **
 ** \return Result of the operation
 ** \retval E_OK Transmit request has been accepted
 ** \retval E_NOT_OK Transmit request has been rejected
 **
 ** \ServiceID{0x04}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_Transmit
(
    PduIdType LinTxPduId,
    P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA) PduInfoPtr
);

#if (LINIF_MASTER_SUPPORTED == STD_ON)

/** \brief Request schedule table for execution
 **
 ** This function schedules a schedule table for execution. Note that when
 ** the NULL_SCHEDULE is requested, all previous requests are deleted.
 **
 ** \param[in] Channel The LIN channel to operate on.
 ** \param[in] ScheduleTable The Id of the schedule requested.
 **
 ** \return Result of the operation
 ** \retval E_OK Schedule table request has been accepted
 ** \retval E_NOT_OK Schedule table request has been rejected
 **
 ** \ServiceID{0x05}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_ScheduleRequest
(
    NetworkHandleType Channel,
    LinIf_SchHandleType ScheduleTable
);
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

/** \brief Set channel to sleep
 **
 ** This function schedules a sleep request for execution. The channel will
 ** not enter sleep mode before the next schedule entry is due.
 **
 ** \param[in] Channel The LIN channel to operate on.
 **
 ** \return Result of the request
 ** \retval E_OK Request has been accepted or sleep transition is already in
 **              progress
 ** \retval E_NOT_OK Request has not been accepted
 **
 ** \ServiceID{0x06}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_GotoSleep
(
    NetworkHandleType Channel
);


/** \brief Wake up channel
 **
 ** This function wakes up a LIN channel.
 **
 ** \param[in] Channel The LIN channel to operate on.
 **
 ** \return Result of the operation
 ** \retval E_OK Wakeup request has been accepted
 ** \retval E_NOT_OK Wakeup request has been rejected
 **
 ** \ServiceID{0x07}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_Wakeup
(
    NetworkHandleType Channel
);


#if (LINIF_CHECKWAKEUP_SUPPORTED == STD_ON)
#ifndef LINIF_USE_LINAPI_REV2
/** \brief Check wakeup function
 **
 ** Will be called when the EcuM has been notified about a wakeup on a
 ** specific LIN channel.
 **
 ** \param[in] WakeupSource  Source device, which initiated the wake up event:
 **                          LIN controller or LIN transceiver
 ** \return Result of the operation
 ** \retval E_OK No error has occurred during execution of the API
 ** \retval E_NOT_OK An error has occurred during execution of the API
 **
 ** \ServiceID{0x60}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_CheckWakeup
(
    EcuM_WakeupSourceType WakeupSource
);
#else /* ifndef LINIF_USE_LINAPI_REV2 */
/** \brief Check wakeup function
 **
 ** Will be called when the EcuM has been notified about a wakeup on a
 ** specific LIN channel.
 **
 ** \param[in] Channel LIN channel
 **
 ** \ServiceID{0x60}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, LINIF_CODE) LinIf_CheckWakeup
(
    NetworkHandleType Channel
);
#endif /* ifndef LINIF_USE_LINAPI_REV2 */
#endif /* LINIF_CHECKWAKEUP_SUPPORTED == STD_ON */


#if (LINIF_TRCV_SUPPORTED == STD_ON)
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetTrcvMode
(
    NetworkHandleType Channel,
    LinTrcv_TrcvModeType TransceiverMode
);


#if (LINIF_MIRROR_ON_MULTICORE_SUPPORT_ENABLE == STD_OFF)
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetTrcvMode
(
    NetworkHandleType Channel,
    P2VAR(LinTrcv_TrcvModeType, AUTOMATIC, AUTOMATIC) TransceiverModePtr
);
#endif /* LINIF_MIRROR_ON_MULTICORE_SUPPORT_ENABLE == STD_OFF */


extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetTrcvWakeupReason
(
    NetworkHandleType Channel,
    P2VAR(LinTrcv_TrcvWakeupReasonType, AUTOMATIC, AUTOMATIC) TrcvWuReasonPtr
);


extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetTrcvWakeupMode
(
    NetworkHandleType Channel,
    LinTrcv_TrcvWakeupModeType LinTrcvWakeupMode
);
#endif /* LINIF_TRCV_SUPPORTED == STD_ON */

#if (LINIF_SLAVE_SUPPORTED == STD_ON)

/**
 ** \brief PID Table retrieval
 **
 ** Retrieves all assigned PID values. The order is congruent to the LIN frame index.
 ** Only applicable for LIN slave nodes.
 **
 ** \param[in]     Channel         LinIf Channel ID
 ** \param[in,out] PidBuffer       Pointer to existing buffer to which the current
 **                                assigned PID values are copied to
 ** \param[in,out] PidBufferLength Pointer to actual length of provided buffer.
 **                                After successful return, it contains
 **                                the number of copied PID values.
 **
 ** \return Std_ReturnType
 ** \retval E_OK     Request has been accepted
 ** \retval E_NOT_OK Request has not been accepted, development or production
 **                  error occurred
 ** \ServiceID{0x72}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetPIDTable
(
    NetworkHandleType Channel,
    P2VAR(Lin_FramePidType, AUTOMATIC, AUTOMATIC) PidBuffer,
    P2VAR(uint8, AUTOMATIC, AUTOMATIC) PidBufferLength
);


/**
 ** \brief PID Table assignment
 **
 ** Sets all assigned PID values. The order is congruent to the LIN frame index.
 ** Only applicable for LIN slave nodes.
 **
 ** \param[in] Channel         LinIf Channel ID
 ** \param[in] PidBuffer       Pointer to buffer which contains the PID values
 **                            to configure.
 ** \param[in] PidBufferLength Number of PID values in the provided buffer
 **
 ** \return Std_ReturnType
 ** \retval E_OK     Request has been accepted
 ** \retval E_NOT_OK Request has not been accepted, development or production
 **                  error occurred
 ** \ServiceID{0x73}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetPIDTable
(
    NetworkHandleType Channel,
    P2VAR(Lin_FramePidType, AUTOMATIC, AUTOMATIC) PidBuffer,
    uint8 PidBufferLength
);


/**
 ** \brief Configured NAD retrieval
 **
 ** Reports the current configured NAD.
 ** Only applicable for LIN slave nodes.
 **
 ** \param[in]  Channel LinIf Channel ID
 ** \param[out] Nad     Configured NAD of slave
 **
 ** \return Std_ReturnType
 ** \retval E_OK     Request has been accepted
 ** \retval E_NOT_OK Request has not been accepted, development or production
 **                  error occurred
 ** \ServiceID{0x70}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant}
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetConfiguredNAD
(
    NetworkHandleType Channel,
    P2VAR(uint8, AUTOMATIC, AUTOMATIC) Nad
);


/**
 ** \brief Configured NAD assignment
 **
 ** Sets the current configured NAD.
 ** Only applicable for LIN slave nodes.
 **
 ** \param[in] Channel LinIf Channel ID
 ** \param[in] Nad     Configured NAD to set as new slave NAD
 **
 ** \return Std_ReturnType
 ** \retval E_OK     Request has been accepted
 ** \retval E_NOT_OK Request has not been accepted, development or production
 **                  error occurred
 ** \ServiceID{0x71}
 ** \Synchronicity{Synchronous}
 ** \Reentrancy{Reentrant}
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetConfiguredNAD
(
    NetworkHandleType Channel,
    uint8 Nad
);

/**
 ** \brief Header Indication function
 **
 ** This service is called by the LIN Driver to report a received LIN header.
 ** Only applicable for LIN slave nodes.
 **
 ** \param[in]     Channel LinIf Channel ID
 ** \param[in,out] PduPtr  Pointer to PDU providing the received PID and
 **                        pointer to the SDU data buffer as in parameter.
 **                        Upon return, the length, checksum type and frame
 **                        response type are received as out parameter. If the
 **                        frame response type is LIN_FRAMERESPONSE_TX, then the
 **                        SDU data buffer contains the transmission data.
 **
 ** \return Std_ReturnType
 ** \retval E_OK     Request has been accepted
 ** \retval E_NOT_OK Request has not been accepted, development or production
 **                  error occurred
 ** \ServiceID{0x78}
 ** \Reentrancy{Reentrant for different Channels}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_HeaderIndication
(
  NetworkHandleType Channel,
  P2VAR(Lin_PduType, AUTOMATIC, AUTOMATIC) PduPtr
);

/**
 ** \brief Reception Indication function
 **
 ** This service is called by the LIN Driver to report a successfully received
 ** response and reception data.
 ** Only applicable for LIN slave nodes.
 **
 ** \param[in] Channel    LinIf Channel ID
 ** \param[in] Lin_SduPtr Pointer to a shadow buffer or memory mapped
 **                       LIN Hardware receive buffer where the current SDU
 **                       is stored. This pointer is only valid if the response
 **                       is received.
 **
 ** \ServiceID{0x79}
 ** \Reentrancy{Reentrant for different Channels}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, LINIF_CODE) LinIf_RxIndication
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) Lin_SduPtr
);

/**
 ** \brief Transmission Confirmation function
 **
 ** This service is called by the LIN Driver to report a successfully
 ** transmitted response.
 ** Only applicable for LIN slave nodes.
 **
 ** \param[in] Channel LinIf Channel ID
 **
 ** \ServiceID{0x7A}
 ** \Reentrancy{Reentrant for different Channels}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, LINIF_CODE) LinIf_TxConfirmation
(
  NetworkHandleType Channel
);

/**
 ** \brief Error Indication function
 **
 ** This service is called by the LIN Driver to report a detected error event
 ** during header or response processing.
 ** Only applicable for LIN slave nodes
 **
 ** \param[in] Channel     LinIf Channel ID
 ** \param[in] ErrorStatus Type of detected error
 **
 ** \ServiceID{0x7B}
 ** \Reentrancy{Reentrant for different Channels}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, LINIF_CODE) LinIf_LinErrorIndication
(
  NetworkHandleType Channel,
  Lin_SlaveErrorType ErrorStatus
);

#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

/** \brief LIN Interface main processing function.
 **
 ** This function performs nearly everything the LIN Interface has to
 ** handle. All access to the LIN bus happens here.
 **
 ** \ProductionError{LINIF_E_RX_CHECKSUM_ERROR, thrown\, if a checksum error is detected.}
 ** \ProductionError{LINIF_E_RX_NO_RESPONSE_ERROR, thrown\, if a slave not responding error is detected.}
 ** \ProductionError{LINIF_E_TX_BIT_ERROR, thrown\, if a bit error is detected.}
 **
 ** \ServiceID{0x80}
 */
extern FUNC(void, LINIF_CODE) LinIf_MainFunction
(
    void
);

#if (LINIF_TP_SUPPORTED == STD_ON)
#if (LINTP_VERSION_INFO_API == STD_ON)
/** \brief Return version Information
 **
 ** \param[out] versioninfo Version information are written to this variable.
 **
 ** \ServiceID{0x42}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, LINIF_CODE) LinTp_GetVersionInfo
(
    P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_DATA) versioninfo
);
#endif /* LINTP_VERSION_INFO_API */


/** \brief Validate configuration
 **
 ** Checks if the post build configuration fits to the link time configuration part.
 **
 ** \return E_OK if the given module configurations is valid otherwise E_NOT_OK.
 **
 ** \ServiceID{0x48}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, LINIF_CODE) LinTp_IsValidConfig
(
    P2CONST(void, AUTOMATIC, LINIF_APPL_CONST) voidConfigPtr
);

/** \brief Initialize TP
 **
 ** This function initializes the LIN Transport Layer
 **
 ** \param[in] ConfigPtr Not used.
 **
 ** \ServiceID{0x40}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, LINIF_CODE) LinTp_Init
(
    P2CONST(LinTp_ConfigType, AUTOMATIC, LINIF_APPL_CONST) ConfigPtr
);

/** \brief Start a TP transmission
 **
 ** This function starts a LinTP-Transmission if there is currently no other
 ** transmission ongoing on the channel identified by the PDU Id.
 **
 ** \param[in] LinTpTxSduId The PDU Id of the message to be sent
 ** \param[in] LinTpTxInfoPtr A PduInfoType to pass the length of the message
 **
 ** \return Result of the operation
 ** \retval E_OK Transmit request has been accepted
 ** \retval E_NOT_OK Transmit request has been rejected
 **
 ** \ServiceID{0x41}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinTp_Transmit
(
    PduIdType LinTpTxSduId,
    P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA) LinTpTxInfoPtr
);


/** \brief Cancel transmit
 **
 ** This function is defined for the upper layer to have a cancel transmit
 ** function. It does nothing else than checking the LinTp state if development
 ** error detection is enabled and always returns E_NOT_OK. This is a dummy
 ** method introduced for interface compatibility.
 **
 ** \param[in] LinTpTxSduId LIN N-SDU identifier
  **
 ** \return Result of the operation
 ** \retval E_NOT_OK: Cancellation request of the transfer of the specified
 **  Lin N-SDU is rejected
 **
 ** \ServiceID{0x46}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinTp_CancelTransmit
(
    PduIdType LinTpTxSduId
);


/** \brief Cancel receive
 **
 ** This function requests the cancellation of a segmented reception of the
 ** given Rx N-SDU. The cancellation itself will be performed during the next
 ** LinIf_MainFunction() call.
 **
 ** \param[in] LinTpRxSduId - This parameter contains the LinTP
 ** instance unique identifier of the Lin N-SDU reception of which has to be
 ** canceled.
 **
 ** \return Result of the operation
 ** \retval E_OK The cancellation request was accepted.
 ** \retval E_NOT_OK: Cancellation request of the reception of the specified
 **  Lin N-SDU is rejected
 **
 ** \ServiceID{0x47}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinTp_CancelReceive
(
    PduIdType LinTpRxSduId
);

/** \brief Change parameter
 **
 ** This function is defined for the upper layer to have a change parameter
 ** request function.This service is used to request the change of reception
 ** parameter STmin for a specified N-SDU.
 **
 ** \param[in] id - Identifier of the received N-SDU on which the reception
 **                 parameter has to be changed.
 ** \param[in] parameter - The selected parameter that the request shall
 **                        change (STmin).
 ** \param[in] value - The new value of the parameter.
 **
 ** \return Result of the operation
 ** \retval E_NOT_OK: request is not accepted
 **
 ** \ServiceID{0x44}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinTp_ChangeParameter
(
    PduIdType id,
    TPParameterType parameter,
    uint16 value
);

#endif /* (LINIF_TP_SUPPORTED == STD_ON) */

#if ((LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON) && (LINIF_MIRROR_ON_MULTICORE_SUPPORT_ENABLE == STD_OFF))

/** \brief This function indicates the channels that are enabled
 *  for bus mirroring.
 *
 *  \param Channel            Channel which is currently processed
 *  \param MirroringActive    The state of the channel - if it is
 *                            enabled for bus mirroring or not
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinIf_EnableBusMirroring
(
    NetworkHandleType Channel,
    boolean MirroringActive
);

#endif /* (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)  && (LINIF_MIRROR_ON_MULTICORE_SUPPORT_ENABLE == STD_OFF) */

#define LINIF_STOP_SEC_CODE
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

#undef LINIF_NO_PBCFG_REQUIRED

#endif /* ifndef LINIF_H */
/*==================[end of file]============================================*/
