#ifndef NM_CBK_H
#define NM_CBK_H

/**
 * \file
 *
 * \brief AUTOSAR Nm
 *
 * This file contains the implementation of the AUTOSAR
 * module Nm.
 *
 * \version 5.12.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>   /* AUTOSAR Communication Stack types */

#include <Nm_Version.h>       /* Module's version declaration */
#include <Nm_Cfg.h>           /* Module configuration */

#if ((NM_DEV_ERROR_DETECT == STD_OFF)                   \
  && (NM_COORDINATOR_SUPPORT_ENABLED == STD_OFF))
/* !LINKSTO Nm124_ComM_Nm,1 */
#include <ComM_Nm.h>           /* ComM callback API for direct mapping */
#endif

/*==================[macros]================================================*/


#include <SchM_Nm.h>            /* Header of Schedule Manager for Nm */

#if ((NM_COORDINATOR_SUPPORT_ENABLED == STD_ON) || \
     (NM_PROVIDE_REMOTE_SLEEP_CALLBACKS == STD_ON))
#if (defined Nm_RemoteSleepCancelation)
#error Nm_RemoteSleepCancelation already defined
#endif
/** \brief Work around for typos in the AUTOSAR SWS documents */
#define Nm_RemoteSleepCancelation(nmNetworkHandle)      \
  Nm_RemoteSleepCancellation(nmNetworkHandle)

#endif

#if ((NM_DEV_ERROR_DETECT == STD_OFF)                   \
  && (NM_COORDINATOR_SUPPORT_ENABLED == STD_OFF)        \
  && (NM_MULTICORE_ENABLED == STD_OFF))

/* Mapping of Network Management callback API calls
 *
 * The set of macros map the Network Management callback API to the callback
 * API of the Communication Manager Module */

/** \brief Map Nm_NetworkMode() to ComM_Nm_NetworkMode()
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel. */
#if (defined Nm_NetworkMode)
#error Nm_NetworkMode already defined
#endif
#define Nm_NetworkMode(nmNetworkHandle)         \
  ComM_Nm_NetworkMode(nmNetworkHandle)

/** \brief Map Nm_NetworkStartIndication() to ComM_Nm_NetworkStartIndication()
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel. */
#if (defined Nm_NetworkStartIndication)
#error Nm_NetworkStartIndication already defined
#endif
#define Nm_NetworkStartIndication(nmNetworkHandle)      \
  ComM_Nm_NetworkStartIndication(nmNetworkHandle)

/** \brief Map Nm_BusSleepMode() to ComM_Nm_BusSleepMode()
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel. */
#if (defined Nm_BusSleepMode)
#error Nm_BusSleepMode already defined
#endif
#define Nm_BusSleepMode(nmNetworkHandle)        \
  ComM_Nm_BusSleepMode(nmNetworkHandle)

/** \brief Map Nm_PrepareBusSleepMode() to ComM_Nm_PrepareBusSleepMode()
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel. */
#if (defined Nm_PrepareBusSleepMode)
#error Nm_PrepareBusSleepMode already defined
#endif
#define Nm_PrepareBusSleepMode(nmNetworkHandle) \
  ComM_Nm_PrepareBusSleepMode(nmNetworkHandle)

#endif /* ((NM_DEV_ERROR_DETECT == STD_OFF)                    \
        && (NM_COORDINATOR_SUPPORT_ENABLED == STD_OFF))        \
        && (NM_MULTICORE_ENABLED == STD_OFF)) */

#if ((NM_COORDINATOR_SUPPORT_ENABLED == STD_OFF) && \
     (NM_PROVIDE_REMOTE_SLEEP_CALLBACKS == STD_OFF))

/** \brief Notification that all other nodes are ready to sleep.
 **
 ** If the Nm coordinator support is disabled this API function is implemented
 ** as empty function-like macro, as it provides no functionality. */
#if (defined Nm_RemoteSleepIndication)
#error Nm_RemoteSleepIndication already defined
#endif
#define Nm_RemoteSleepIndication(nmNetworkHandle) TS_PARAM_UNUSED(nmNetworkHandle)

/** \brief Notification that not all other nodes are ready to sleep.
 **
 ** If the Nm coordinator support is disabled this callback function is
 ** implemented as function-like macro. */
#if (defined Nm_RemoteSleepCancellation)
#error Nm_RemoteSleepCancellation already defined
#endif
#define Nm_RemoteSleepCancellation(nmNetworkHandle) TS_PARAM_UNUSED(nmNetworkHandle)

#endif

/*==================[external function declarations]======================*/

#define NM_START_SEC_CODE
#include <Nm_MemMap.h>

#if ((NM_DEV_ERROR_DETECT == STD_ON)                    \
  || (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)         \
  || (NM_MULTICORE_ENABLED == STD_ON))

#ifndef RTE_SCHM_SWCBSW_NM_NETWORKMODE
#define RTE_SCHM_SWCBSW_NM_NETWORKMODE
/** \brief Notifies that the network management has entered Network Mode.
 **
 ** This function gives a notification that the network management has entered
 ** Network Mode. The callback function enables transmission of application
 ** messages.
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **   initialized for this channel (checked).
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel.
 **
 ** \ServiceID{30}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO Nm.EB.NmMultiCoreSupport_ON_2,1 */
extern FUNC(void, NM_CODE) Nm_NetworkMode
(
  NetworkHandleType nmNetworkHandle
);
#endif /* RTE_SCHM_SWCBSW_NM_NETWORKMODE */
#ifndef RTE_SCHM_SWCBSW_NM_PREPAREBUSSLEEPMODE
#define RTE_SCHM_SWCBSW_NM_PREPAREBUSSLEEPMODE
/** \brief Notification that the network management has entered Prepare
 ** Bus-Sleep Mode.
 **
 ** This function provides a notification that the network management has
 ** entered Prepare Bus-Sleep Mode. The callback function disables
 ** transmission of application messages.
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **   initialized for this channel (checked).
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel.
 **
 ** \ServiceID{31}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
/* !LINKSTO Nm.EB.NmMultiCoreSupport_ON_4,1 */
extern FUNC(void, NM_CODE) Nm_PrepareBusSleepMode
(
  NetworkHandleType nmNetworkHandle
);
#endif /* RTE_SCHM_SWCBSW_NM_PREPAREBUSSLEEPMODE */

#ifndef RTE_SCHM_SWCBSW_NM_BUSSLEEPMODE
#define RTE_SCHM_SWCBSW_NM_BUSSLEEPMODE
/** \brief Notifies that Bus Sleep Mode has been entered.
 **
 ** This function provides a notification that the network management has
 ** entered Bus-Sleep Mode.
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **   initialized for this channel (checked).
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel.
 **
 ** \ServiceID{32}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO Nm.EB.NmMultiCoreSupport_ON_3,1 */
extern FUNC(void, NM_CODE) Nm_BusSleepMode
(
   NetworkHandleType nmNetworkHandle
);
#endif /* RTE_SCHM_SWCBSW_NM_BUSSLEEPMODE */

#ifndef RTE_SCHM_SWCBSW_NM_NETWORKSTARTINDICATION
#define RTE_SCHM_SWCBSW_NM_NETWORKSTARTINDICATION
/** \brief Notifies that Network Mode has been entered.
 **
 ** This function provides a notification that a NM-message has been received
 ** in the Bus-Sleep Mode, which indicates that some nodes in the network have
 ** already entered the Network Mode. The callback function starts the network
 ** management state machine.
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **   initialized for this channel (checked).
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel.
 **
 ** \ServiceID{33}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO Nm.EB.NmMultiCoreSupport_ON_1,1 */
extern FUNC(void, NM_CODE) Nm_NetworkStartIndication
(
  NetworkHandleType nmNetworkHandle
);
#endif

#endif /* ((NM_DEV_ERROR_DETECT == STD_ON)                      \
        * || (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)         \
        * || (NM_MULTICORE_ENABLED == STD_ON)) */


#if ((NM_COORDINATOR_SUPPORT_ENABLED == STD_ON) || \
     (NM_PROVIDE_REMOTE_SLEEP_CALLBACKS == STD_ON))
#ifndef RTE_SCHM_SWCBSW_NM_REMOTESLEEPINDICATION
#define RTE_SCHM_SWCBSW_NM_REMOTESLEEPINDICATION
/** \brief Notification that all other nodes are ready to sleep.
 **
 ** This function provides a notification that the network management has
 ** detected that all other nodes are ready to sleep. The NM gateway checks
 ** if the Bus is still required.
 **
 ** If the Nm coordinator support is disabled this API function is implemented
 ** as empty function-like macro, as it provides no functionality.
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel */
extern FUNC(void, NM_CODE) Nm_RemoteSleepIndication
(
  NetworkHandleType nmNetworkHandle
);
#endif /* RTE_SCHM_SWCBSW_NM_REMOTESLEEPINDICATION */
#ifndef RTE_SCHM_SWCBSW_NM_REMOTESLEEPCANCELLATION
#define RTE_SCHM_SWCBSW_NM_REMOTESLEEPCANCELLATION
/** \brief Notification that not all other nodes are ready to sleep.
 **
 ** Notification that the network management has detected that not all other
 ** nodes on the network are longer ready to enter Bus-Sleep Mode.
 **
 ** If the Nm coordinator support is disabled this callback function is
 ** implemented as function-like macro.
 **
 ** This function is not specified in AUTOSAR R3.x Nm SWS but in the R4.0
 ** draft SWS.
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel */
extern FUNC(void, NM_CODE) Nm_RemoteSleepCancellation
(
  NetworkHandleType nmNetworkHandle
);
#endif /* RTE_SCHM_SWCBSW_NM_REMOTESLEEPCANCELLATION */
#endif

#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
#ifndef RTE_SCHM_SWCBSW_NM_SYNCHRONIZATIONPOINT
#define RTE_SCHM_SWCBSW_NM_SYNCHRONIZATIONPOINT
/** \brief Notification that this is a suitable point in time to
 **        initiate the coordination algorithm on
 **
 ** Notification NM Coordinator functionality that this is a
 ** suitable point in time to initiate the coordination algorithm on
 **
 ** If the Nm coordinator support is disabled this callback function is
 ** implemented as function-like macro.
 **
 ** This function is not specified in AUTOSAR R3.x Nm SWS but in the R4.0
 ** draft SWS.
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel */
extern FUNC(void, NM_CODE) Nm_SynchronizationPoint
(
  NetworkHandleType nmNetworkHandle
);
#endif /* RTE_SCHM_SWCBSW_NM_SYNCHRONIZATIONPOINT */
#endif

#ifndef RTE_SCHM_SWCBSW_NM_TXTIMEOUTEXCEPTION
#define RTE_SCHM_SWCBSW_NM_TXTIMEOUTEXCEPTION
/** \brief Notification service to indicate that an attempt to send a NM message
 **        failed.
 **
 ** This function provides an indication that an attempt to send an NM message
 ** failed.
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel */
extern FUNC(void, NM_CODE) Nm_TxTimeoutException
(
  NetworkHandleType nmNetworkHandle
);
#endif /* RTE_SCHM_SWCBSW_NM_TXTIMEOUTEXCEPTION */
#if (NM_BUS_SYNCHRONIZATION_ENABLED == STD_ON)
#ifndef RTE_SCHM_SWCBSW_NM_COORDREADYTOSLEEPINDICATION
#define RTE_SCHM_SWCBSW_NM_COORDREADYTOSLEEPINDICATION
/** \brief Indicate Ready to Sleep
 **
 ** Sets an indication, when the NM Coordinator Sleep Ready
 ** bit in the Control Bit Vector is set
 **
 ** The functionality needs to be implemented by the customer, as it is not
 ** specified by AUTOSAR.
 **
 ** \param[in] nmChannelHandle Identification of the NM-channel */
extern FUNC(void, NM_CODE) Nm_CoordReadyToSleepIndication
(
 NetworkHandleType nmChannelHandle
);
#endif /* RTE_SCHM_SWCBSW_NM_COORDREADYTOSLEEPINDICATION */
#endif

#if (NM_STATE_CHANGE_IND_ENABLED == STD_ON)
#ifndef RTE_SCHM_SWCBSW_NM_STATECHANGENOTIFICATION
#define RTE_SCHM_SWCBSW_NM_STATECHANGENOTIFICATION
/** \brief Notification that an Nm changed state
 **
 ** This function provides a notification that a network management has
 ** changed state.
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel */
extern FUNC(void, NM_CODE) Nm_StateChangeNotification
(
  NetworkHandleType nmNetworkHandle,
  Nm_StateType nmPreviousState,
  Nm_StateType nmCurrentState
);
#endif /* RTE_SCHM_SWCBSW_NM_STATECHANGENOTIFICATION */
#endif

#if (NM_PDU_RX_INDICATION_ENABLED == STD_ON)
#ifndef RTE_SCHM_SWCBSW_NM_PDURXINDICATION
#define RTE_SCHM_SWCBSW_NM_PDURXINDICATION
/** \brief Indication that an Rx Pdu was received
 **
 ** Indication that a bus network management has received an NM message.
 **
 ** The functionality needs to be implemented by the customer, as it is not
 ** specified by AUTOSAR.
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel */
extern FUNC(void, NM_CODE) Nm_PduRxIndication
(
  NetworkHandleType nmNetworkHandle
);
#endif /* RTE_SCHM_SWCBSW_NM_PDURXINDICATION */
#endif

#if (NM_REPEAT_MESSAGE_INDICATION == STD_ON)
#ifndef RTE_SCHM_SWCBSW_NM_REPEATMESSAGEINDICATION
#define RTE_SCHM_SWCBSW_NM_REPEATMESSAGEINDICATION
/** \brief NM message with set Repeat Message Request Bit has been received.
 **
 ** Indication that a NM message with set Repeat Message Request Bit has been
 ** received.
 **
 ** The functionality needs to be implemented by the customer, as it is not
 ** specified by AUTOSAR.
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel */
extern FUNC(void, NM_CODE) Nm_RepeatMessageIndication
(
  NetworkHandleType nmNetworkHandle
);
#endif /* RTE_SCHM_SWCBSW_NM_REPEATMESSAGEINDICATION */
#endif

#if (NM_STATE_CHANGE_NOTIFICATION_CALLOUT == STD_ON)

/** \brief User specific callout function which will be called from
 **        callback function Nm_StateChangeNotification() to notify about the
 **        state changes within the BusNm modules.
 **
 ** The functionality needs to be implemented by the customer, as it is not
 ** specified by AUTOSAR.
 **
 ** \param[in] nmNetworkHandle Identification of the NM channel
 **            nmPreviousState Previous state of the NM channel
 **            nmCurrentState  Current(new) state of the NM channel
 */
extern FUNC(void, NM_CODE) Nm_StateChangeNotificationCallout
(
  NetworkHandleType nmNetworkHandle,
  Nm_StateType nmPreviousState,
  Nm_StateType nmCurrentState
);

#endif

#if (NM_CAR_WAKEUPRX_INDICATION == STD_ON)
#ifndef RTE_SCHM_SWCBSW_NM_CARWAKEUPINDICATION
#define RTE_SCHM_SWCBSW_NM_CARWAKEUPINDICATION
/** \brief Callback function to indicate car wakeup
 **
 ** This function is responsible to manage the Car Wake Up (CWU)
 ** request and distribute the Request to other Nm channels
 **
 ** The functionality needs to be implemented by the customer.
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel */
extern FUNC(void, NM_CODE) Nm_CarWakeUpIndication
(
  NetworkHandleType nmChannelHandle
);
#endif /* RTE_SCHM_SWCBSW_NM_CARWAKEUPINDICATION */
#endif

#define NM_STOP_SEC_CODE
#include <Nm_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef NM_CBK_H */
