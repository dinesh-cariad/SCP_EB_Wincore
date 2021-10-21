/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/
#include <Std_Types.h>        /* AUTOSAR standard types */
#include <TSAutosar.h>        /* EB specific standard types */
#include <Xcp_Cfg.h>
#include <Xcp_Version.h>      /* This modules' version declarations */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[external constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#ifndef XCP_WRAPPER_H
#define XCP_WRAPPER_H
[!AUTOSPACING!][!//
[!VAR "XcpEventChannelMFEnabled"="(XcpGeneral/XcpEventTriggerMainFunc = 'true') and (node:exists(XcpConfig/*[1]/XcpEventChannel)) and (num:i(count(XcpConfig/*[1]/XcpEventChannel/*)) > 0)"!]
[!VAR "XcpCrcMFEnabled"="XcpGeneral/XcpBuildChecksumMainFunctionSupport = 'true'"!]
[!VAR "XcpComMFEnabled"="node:exists(XcpGeneral/XcpSupportComMainFunctions) = 'true'"!]
[!VAR "XcpWrapperEnabled"="XcpGeneral/XcpServiceWrapper = 'true'"!]

[!IF "$XcpWrapperEnabled"!]
#define XCP_WRAPPER_START_SEC_WRAPPER_CODE
#include <Xcp_MemMap.h>

/*------------------[Scheduled functions]-----------------------------------*/
/** \brief Scheduled function of the XCP wrapper. This function calls
 ** calls Xcp_MainFunction only if the XCP module is enabled.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Scheduled function of the XCP module.
 **
 ** Precondition: None
 **
 ** \ServiceID{::255} */
extern FUNC(void,XCP_WRAPPER_CODE) Xcp_Wrapper_MainFunction(void);

[!IF "$XcpEventChannelMFEnabled"!]
  [!LOOP "XcpConfig/*[1]/XcpEventChannel/*"!]
    [!IF "XcpEventChannelTimeCycle > 0"!]
/** \brief Scheduled function of the XCP wrapper for processing the events. This function calls
 ** calls Xcp_MainFunction_EventName only if the XCP module is enabled.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Scheduled function of the XCP module.
 **
 ** Precondition: None
 **
 ** \ServiceID{::255} */
extern FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_MainFunction_[!"@name"!](void);
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDIF!]

[!IF "$XcpCrcMFEnabled"!]
/** \brief Scheduled function of the XCP wrapper for processing the CRC. This function calls
 ** calls Xcp_CrcMainFunction only if the XCP module is enabled.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Scheduled function of the XCP module.
 **
 ** Precondition: None
 **
 ** \ServiceID{::255} */
extern FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_CrcMainFunction(void);
[!ENDIF!]

[!IF "$XcpComMFEnabled = 'true'"!]
/** \brief Scheduled function of the XCP wrapper for processing the CRC. This function calls
 ** calls Xcp_TxMainFunction only if the XCP module is enabled.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Scheduled function of the XCP module.
 **
 ** Precondition: None
 **
 ** \ServiceID{::255} */
extern FUNC(void,XCP_WRAPPER_CODE) Xcp_Wrapper_TxMainFunction(void);

/** \brief Scheduled function of the XCP wrapper for processing the CRC. This function calls
 ** calls Xcp_RxMainFunction only if the XCP module is enabled
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Scheduled function of the XCP module.
 **
 ** Precondition: None
 **
 ** \ServiceID{::255} */
extern FUNC(void,XCP_WRAPPER_CODE) Xcp_Wrapper_RxMainFunction(void);
[!ENDIF!]

#if (XCP_VERSION_INFO_API == STD_ON)
/** \brief Wrapper function to return the XCP module version information
 **
 ** This function is a wrapper function to the Xcp interface that provides module vendor
 ** ID, module ID and software version major.minor.patch. If the XCP module is disabled
 ** Xcp_Wrapper_GetVersionInfo has no effect on the output parameter VersionInfoPtr.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Precondition: ::XCP_VERSION_INFO_API = ::STD_ON
 **
 ** \param[out]  VersionInfoPtr  Pointer to where to store the version
 **                              information of this module.
 **
 ** \ServiceID{::255}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, XCP_APPL_DATA) VersionInfoPtr
);
#endif

/** \brief Wrapper function that initializes the XCP.
 **
 ** This function represents the wrapper function for the initialization interface of the ** AUTOSAR XCP module.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Precondition: None.
 **
 ** \param[in]  Xcp_ConfigPtr  Pointer to a selected configuration structure.
 **
 ** \ServiceID{::255}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_Init
(
  P2CONST(Xcp_ConfigType, AUTOMATIC, XCP_APPL_CONST) Xcp_ConfigPtr
);

/** \brief Wrapper function that calls XCP interface  for setting an event.
 **
 ** This function only calls Xcp_SetEvent if XCP module is enabled.
 **
 ** Precondition: The event channel must be a non-cyclic event channel, i.e.
 ** the parameter XcpEventChannelTimeCycle is set to 0 for that event channel.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** \param[in]  EventId  XCP event channel ID.
 **
 ** \return Result of the operation.
 ** \retval XCP_OK     Setting the event was successful.
 ** \retval XCP_NOT_OK Setting the event failed (a development error
 **                    happened).
 ** \retval XCP_NOT_INITIALIZED Setting the event failed (module is not
 **                             initialized).
 ** \retval XCP_NOT_CONNECTED   Setting the event failed (slave is in the
 **                             state DISCONNECTED).
 ** \retval XCP_OVERLOAD        Setting the event failed (event is already
 **                             set).
 ** \retval XCP_NO_ACTIVE_LIST  Setting the event failed (no active list is
 **                             assigned to that event).
 **
 ** \ServiceID{::255}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Xcp_ReturnType, XCP_WRAPPER_CODE) Xcp_Wrapper_SetEvent
(
  uint16 EventId
);


/** \brief XCP PDU is received.
 **
 ** This function is called by the lower layer CanIf when an AUTOSAR XCP PDU has
 ** been received. If the XCP is enabled it calls Xcp_CanIfRxIndication, otherwise
 ** the function returns without any effect.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Precondition: ::XCP_ON_CAN_ENABLED = ::STD_ON
 ** Precondition: ::XCP_ON_CANFD_ENABLED = ::STD_ON
 **
 ** \param[in]  XcpRxPduId  PDU-ID that has been received.
 ** \param[in]  XcpRxPduPtr  Contains the length (SduLength) of the received I-PDU and a pointer to
 **                          a buffer (SduDataPtr) containing the I-PDU.
 **
 ** \ServiceID{::255}
 ** \Reentrancy{Reentrant for different XcpRxPduId. Non-reentrant for the same XcpRxPduId}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_CanIfRxIndication
(
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
);

/** \brief XCP PDU transmit confirmation.
 **
 ** This function is called by the lower layer CanIf when an AUTOSAR XCP PDU has
 ** been transmitted. If the XCP is enabled it calls Xcp_CanIfTxConfirmation,
 ** otherwise the function returns without any effect.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Precondition: ::XCP_ON_CAN_ENABLED = ::STD_ON
 ** Precondition: ::XCP_ON_CANFD_ENABLED = ::STD_ON
 **
 ** \param[in]  XcpTxPduId  PDU-ID that has been transmitted.
 **
 ** \ServiceID{::255}
 ** \Reentrancy{Reentrant for different XcpTxPduId. non-Reentrant for the same XcpTxPduId}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_CanIfTxConfirmation
(
  PduIdType XcpTxPduId
);

/** \brief XCP PDU is received.
 **
 ** This function is called by the lower layer Socket Adapter when an AUTOSAR
 ** XCP PDU has been received.If the XCP is enabled it calls Xcp_SoAdIfRxIndication,
 ** otherwise the function returns without any effect.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Precondition: ::XCP_ON_ETHERNET_ENABLED = ::STD_ON
 **
 ** \param[in]  XcpRxPduId  PDU-ID that has been received.
 ** \param[in]  XcpRxPduPtr  Pointer to SDU(Buffer of received payload).
 ** \ServiceID{::255}
 ** \Reentrancy{Reentrant for different XcpRxPduId, non-Reentrant for the same
 ** XcpRxPduId}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_SoAdIfRxIndication
(
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
);

/** \brief XCP PDU transmit confirmation.
 **
 ** This function is called by the lower layer Socket Adapter when an AUTOSAR
 ** XCP PDU has been transmitted. If the XCP is enabled it calls Xcp_SoAdIfTxConfirmation,
 ** otherwise the function returns without any effect.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Precondition: ::XCP_ON_ETHERNET_ENABLED = ::STD_ON
 **
 ** \param[in]  XcpTxPduId  PDU-ID that has been transmitted.
 ** \ServiceID{::255}
 ** \Reentrancy{Reentrant for different XcpTxPduId, non-Reentrant for the same
 ** XcpTxPduId}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_SoAdIfTxConfirmation
(
  PduIdType XcpTxPduId
);

#if (XCP_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON)
/** \brief Socket connection mode change notification.
 **
 ** This function is called by the lower layer Socket Adapter when the socket
 ** connection mode has changed. If the XCP is enabled it calls Xcp_SoAdSoConModeChg,
 ** otherwise the function returns without any effect.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** This function is available only for the TCP protocol as there is no UDP connection (UDP being
 ** a broadcasting protocol)
 **
 ** Precondition: ::XCP_ON_ETHERNET_ENABLED = ::STD_ON
 **
 ** \param[in]  SoConId  Socket connection ID of socket that has changed.
 ** \param[in]  Mode  Socket connection mode (ONLINE, OFFLINE or RECONNECT).
 ** \ServiceID{::255}
 ** \Reentrancy{Reentrant for different SoConIds. Non reentrant for the same
 ** SoConId}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_SoAdSoConModeChg
(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode
);

#endif /* (XCP_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON) */

#if (XCP_ON_FLEXRAY_ENABLED == STD_ON)
/** \brief XCP PDU is received.
 **
 ** This function is called by the lower layer FrIf when an AUTOSAR XCP PDU has
 ** been received. If the XCP module is enabled it calls Xcp_FrIfRxIndication,
 ** otherwise the function returns without any effect.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Precondition: ::XCP_ON_FLEXRAY_ENABLED = ::STD_ON
 **
 ** \param[in]  XcpRxPduId  PDU-ID that has been received.
 ** \param[in]  XcpRxPduPtr  Contains the length (SduLength) of the received I-PDU and a pointer to
 **                          a buffer (SduDataPtr) containing the I-PDU.
 **
 ** \ServiceID{::255}
 ** \Reentrancy{Reentrant for different XcpRxPduId. Non-reentrant for the same XcpRxPduId}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_FrIfRxIndication
(
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
);

/** \brief XCP PDU transmit confirmation.
 **
 ** This function is called by the lower layer FrIf when an AUTOSAR XCP PDU has
 ** been transmitted. If the XCP module is enabled it calls Xcp_FrIfTriggerTransmit,
 ** otherwise the function returns without any effect.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Precondition: ::XCP_ON_FLEXRAY_ENABLED = ::STD_ON
 **
 ** \param[in]  XcpTxPduId  PDU-ID that has been transmitted.
 **
 ** \ServiceID{::XCP_SID_IF_TX_CONFIRMATION}
 ** \Reentrancy{Reentrant for different XcpTxPduId. Non-reentrant for the same XcpTxPduId}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_FrIfTxConfirmation
(
  PduIdType XcpTxPduId
);

#if (XCP_TRIGGER_TX_API_ENABLED == STD_ON)
/** \brief Call when an AUTOSAR XCP PDU shall be transmitted.
 **
 ** This function is called by the lower layer FrIf when an AUTOSAR XCP PDU
 ** shall be transmitted. If the XCP module is enabled it calls Xcp_FrIfTriggerTransmit,
 ** for requesting the I-PDU, otherwise the functions returns without any effect and the,
 ** output parameter PduInfoPtr remains unchanged.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Precondition: ::XCP_ON_FLEXRAY_ENABLED = ::STD_ON
 **
 ** \param[in]  XcpTxPduId  PDU-ID that has been requested to be transmitted.
 ** \param[out]  PduInfoPtr  Pointer to PduInfo SDU (pointer to SDU buffer and
 **                          SDU length).
 ** \return Function result
 ** \retval E_OK SDU has been copied and SduLength indicates the number of
 **              copied bytes.
 ** \retval E_NOT_OK No SDU has been copied. PduInfoPtr must not be used since
 **                  it may contain a NULL pointer or point to invalid data.
 **
 ** \ServiceID{::255}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, XCP_WRAPPER_CODE) Xcp_Wrapper_FrIfTriggerTransmit
(
  PduIdType XcpTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr
);
#endif /* (XCP_TRIGGER_TX_API_ENABLED == STD_ON) */
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) */

#if (XCP_ON_CDD_ENABLED == STD_ON)
/** \brief XCP PDU is received.
 **
 ** This function is called by the lower layer Cdd when an AUTOSAR XCP PDU has
 ** been received. If the XCP module is enabled it calls Xcp_CddRxIndication,
 ** otherwise the function returns without any effect.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Precondition: ::XCP_ON_CDD_ENABLED = ::STD_ON
 **
 ** \param[in]  XcpRxPduId  PDU-ID that has been received.
 ** \param[in]  XcpRxPduPtr  Contains the length (SduLength) of the received I-PDU and a pointer to
 **                          a buffer (SduDataPtr) containing the I-PDU.
 **
 ** \ServiceID{::255}
 ** \Reentrancy{Reentrant for different XcpRxPduId. Non-reentrant for the same XcpRxPduId}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_CddRxIndication
(
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
);

/** \brief XCP PDU transmit confirmation.
 **
 ** This function is called by the lower layer Cdd when an AUTOSAR XCP PDU has
 ** been transmitted. If the XCP module is enabled it calls Xcp_CddTriggerTransmit,
 ** otherwise the function returns without any effect.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Precondition: ::XCP_ON_CDD_ENABLED = ::STD_ON
 **
 ** \param[in]  XcpTxPduId  PDU-ID that has been transmitted.
 **
 ** \ServiceID{::XCP_SID_IF_TX_CONFIRMATION}
 ** \Reentrancy{Reentrant for different XcpTxPduId. Non-reentrant for the same XcpTxPduId}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_CddTxConfirmation
(
  PduIdType XcpTxPduId
);

#if (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON)
/** \brief Call when an AUTOSAR XCP PDU shall be transmitted.
 **
 ** This function is called by the lower layer Cdd when an AUTOSAR XCP PDU
 ** shall be transmitted. If the XCP module is enabled it calls Xcp_CddTriggerTransmit,
 ** for requesting the I-PDU, otherwise the functions returns without any effect and the,
 ** output parameter PduInfoPtr remains unchanged.
 **
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** Precondition: ::XCP_ON_CDD_ENABLED = ::STD_ON
 **
 ** \param[in]  XcpTxPduId  PDU-ID that has been requested to be transmitted.
 ** \param[out]  PduInfoPtr  Pointer to PduInfo SDU (pointer to SDU buffer and
 **                          SDU length).
 ** \return Function result
 ** \retval E_OK SDU has been copied and SduLength indicates the number of
 **              copied bytes.
 ** \retval E_NOT_OK No SDU has been copied. PduInfoPtr must not be used since
 **                  it may contain a NULL pointer or point to invalid data.
 **
 ** \ServiceID{::255}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, XCP_WRAPPER_CODE) Xcp_Wrapper_CddTriggerTransmit
(
  PduIdType XcpTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr
);
#endif /* (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON) */
#endif /* (XCP_ON_CDD_ENABLED == STD_ON) */

#if (XCP_SUPPRESS_TX_SUPPORT == STD_ON)
/**
 ** \brief This API is used to turn on and off of the TX capabilities of used communication
 ** bus channel in XCP module.
 ** If Xcp_SetTransmissionMode(Channel, Mode) is called and parameter Mode
 ** equals XCP_TX_OFF, all TxPDUs which are assigned to Channel shall not be
 ** transmitted.
 ** 
 ** Mandatory interface: App_Xcp_Wrapper_IsXcpUsed().
 **
 ** If Xcp_SetTransmissionMode(Channel, Mode) is called and parameter Mode
 ** equals XCP_TX_ON, all TxPDUs which are assigned to Channel shall be able to be
 ** transmitted.
 **
 ** \param[in] Channel The Network channel for the used bus communication
 ** \param[in] Mode Enabled or disabled Transmission mode Parameters
 **
 ** \ServiceID{::XCP_SID_SET_TRANSMISSION_MODE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_CODE)  Xcp_Wrapper_SetTransmissionMode
(
  NetworkHandleType Channel,
  Xcp_TransmissionModeType Mode
);

#endif /* #if (XCP_SUPPRESS_TX_SUPPORT == STD_ON) */

#define XCP_WRAPPER_STOP_SEC_WRAPPER_CODE
#include <Xcp_MemMap.h>

[!ENDIF!]

#if (XCP_ON_CDD_ENABLED == STD_ON)

#define XCP_WRAPPER_START_SEC_WRAPPER_CODE
#include <Xcp_MemMap.h>

/** \brief Wrapper function to be called when an AUTOSAR XCP PDU shall be transmitted
 **        over a CDD connection.
 **
 ** Precondition: ::XCP_ON_CDD_ENABLED = ::STD_ON
 **
 ** \param[in]  XcpTxPduId   PDU-ID that has been requested to be transmitted.
 ** \param[out] PduInfoPtr  Pointer to PduInfo SDU (pointer to SDU buffer and
 **                          SDU length).
 ** \return Function result
 ** \retval E_OK SDU has been copied and SduLength indicates the number of
 **              copied bytes.
 ** \retval E_NOT_OK No SDU has been copied. PduInfoPtr must not be used since
 **                  it may contain a NULL pointer or point to invalid data.
 **
 ** \ServiceID{::255}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, XCP_WRAPPER_CODE) Xcp_Wrapper_CddTransmit
(
  PduIdType XcpTxPduId,
  P2CONST(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr
);

#define XCP_WRAPPER_STOP_SEC_WRAPPER_CODE
#include <Xcp_MemMap.h>

#endif /* (XCP_ON_CDD_ENABLED == STD_ON) */
/*==================[internal function definitions]=========================*/

#endif /* ifndef XCP_WRAPPER_H */

/*==================[end of file]===========================================*/
