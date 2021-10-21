/**
 * \file
 *
 * \brief AUTOSAR Mirror
 *
 * This file contains the implementation of the AUTOSAR
 * module Mirror.
 *
 * \version 1.0.17
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef MIRROR_H
#define MIRROR_H

/*==================[inclusions]============================================*/

#include <Mirror_Cfg.h>         /* Generated configuration */
#include <Mirror_Types.h>       /* Specific Mirror types */
#include <Mirror_Int.h>    /* Mirror internal macros and declarations*/

/*==================[macros]================================================*/

/*------------------[service Ids]----------------------------------------*/

/** \brief Module instance ID
 *
 * Defines the instance number of this module. Since multiple instances are not supported this ID
 * is always zero.
 **/
#if (defined MIRROR_INSTANCE_ID)
#error MIRROR_INSTANCE_ID already defined!
#endif /* #if (defined MIRROR_INSTANCE_ID)*/
#define MIRROR_INSTANCE_ID 0U

/** \brief Defines API id of function Mirror_Init(). */
#if (defined MIRROR_SID_INIT)
#error MIRROR_SID_INIT already defined!
#endif /* #if (defined MIRROR_SID_INIT)*/
#define MIRROR_SID_INIT 0x01U

/** \brief Defines API id of function Mirror_DeInit(). */
#if (defined MIRROR_SID_DEINIT)
#error MIRROR_SID_DEINIT already defined!
#endif /* #if (defined MIRROR_SID_DEINIT)*/
#define MIRROR_SID_DEINIT 0x02U

/** \brief Defines API id of function Mirror_GetVersionInfo(). */
#if (defined MIRROR_SID_GETVERSIONINFO)
#error MIRROR_SID_GETVERSIONINFO already defined!
#endif /* #if (defined MIRROR_SID_GETVERSIONINFO)*/
#define MIRROR_SID_GETVERSIONINFO 0x03U

/** \brief Defines API id of function Mirror_MainFunction(). */
#if (defined MIRROR_SID_MAINFUNCTION)
#error MIRROR_SID_MAINFUNCTION already defined!
#endif /* #if (defined MIRROR_SID_MAINFUNCTION)*/
#define MIRROR_SID_MAINFUNCTION 0x04U

/** \brief Defines API id of function Mirror_StartAllSourceNetworks(). */
#if (defined MIRROR_SID_STARTALLSOURCENETWORKS)
#error MIRROR_SID_STARTALLSOURCENETWORKS already defined!
#endif /* #if (defined MIRROR_SID_STARTALLSOURCENETWORKS)*/
#define MIRROR_SID_STARTALLSOURCENETWORKS 0x08U

/** \brief Defines API id of function Mirror_StopAllSourceNetworks(). */
#if (defined MIRROR_SID_STOPALLSOURCENETWORKS)
#error MIRROR_SID_STOPALLSOURCENETWORKS already defined!
#endif /* #if (defined MIRROR_SID_STOPALLSOURCENETWORKS)*/
#define MIRROR_SID_STOPALLSOURCENETWORKS 0x09U

/** \brief Defines API id of function Mirror_StartSourceNetwork(). */
#if (defined MIRROR_SID_STARTSOURCENETWORK)
#error MIRROR_SID_STARTSOURCENETWORK already defined!
#endif /* #if (defined MIRROR_SID_STARTSOURCENETWORK)*/
#define MIRROR_SID_STARTSOURCENETWORK 0x10U

/** \brief Defines API id of function Mirror_StopSourceNetwork(). */
#if (defined MIRROR_SID_STOPSOURCENETWORK)
#error MIRROR_SID_STOPSOURCENETWORK already defined!
#endif /* #if (defined MIRROR_SID_STOPSOURCENETWORK)*/
#define MIRROR_SID_STOPSOURCENETWORK 0x11U

/** \brief Defines API id of function Mirror_Offline(). */
#if (defined MIRROR_SID_OFFLINE)
#error MIRROR_SID_OFFLINE already defined!
#endif /* #if (defined MIRROR_SID_OFFLINE)*/
#define MIRROR_SID_OFFLINE 0x13U

/** \brief Defines API id of function Mirror_IsMirrorActive(). */
#if (defined MIRROR_SID_ISMIRRORACTIVE)
#error MIRROR_SID_ISMIRRORACTIVE already defined!
#endif /* #if (defined MIRROR_SID_ISMIRRORACTIVE)*/
#define MIRROR_SID_ISMIRRORACTIVE 0x20U

/** \brief Defines API id of function Mirror_GetDestNetwork(). */
#if (defined MIRROR_SID_GETDESTINATIONNETWORK)
#error MIRROR_SID_GETDESTINATIONNETWORK already defined!
#endif /* #if (defined MIRROR_SID_GETDESTINATIONNETWORK)*/
#define MIRROR_SID_GETDESTINATIONNETWORK 0x21U

/** \brief Defines API id of function Mirror_IsSourceNetworkStarted(). */
#if (defined MIRROR_SID_ISSOURCENETWORKSTARTED)
#error MIRROR_SID_ISSOURCENETWORKSTARTED already defined!
#endif /* #if (defined MIRROR_SID_ISSOURCENETWORKSTARTED)*/
#define MIRROR_SID_ISSOURCENETWORKSTARTED 0x22U

/** \brief Defines API id of function Mirror_GetNetworkType(). */
#if (defined MIRROR_SID_GETNETWORKTYPE)
#error MIRROR_SID_GETNETWORKTYPE already defined!
#endif /* #if (defined MIRROR_SID_GETNETWORKTYPE)*/
#define MIRROR_SID_GETNETWORKTYPE 0x24U

/** \brief Defines API id of function Mirror_GetNetworkId(). */
#if (defined MIRROR_SID_GETNETWORKID)
#error MIRROR_SID_GETNETWORKID already defined!
#endif /* #if (defined MIRROR_SID_GETNETWORKID)*/
#define MIRROR_SID_GETNETWORKID 0x25U

/** \brief Defines API id of function Mirror_GetNetworkHandle(). */
#if (defined MIRROR_SID_GETNETWORKHANDLE)
#error MIRROR_SID_GETNETWORKHANDLE already defined!
#endif /* #if (defined MIRROR_SID_GETNETWORKHANDLE)*/
#define MIRROR_SID_GETNETWORKHANDLE 0x26U

/** \brief Defines API id of function Mirror_ReportCanFrame(). */
#if (defined MIRROR_SID_REPORTCANFRAME)
#error MIRROR_SID_REPORTCANFRAME already defined!
#endif /* #if (defined MIRROR_SID_REPORTCANFRAME)*/
#define MIRROR_SID_REPORTCANFRAME 0x50U

/** \brief Defines API id of function Mirror_ReportLinFrame(). */
#if (defined MIRROR_SID_REPORTLINFRAME)
#error MIRROR_SID_REPORTLINFRAME already defined!
#endif /* #if (defined MIRROR_SID_REPORTLINFRAME)*/
#define MIRROR_SID_REPORTLINFRAME 0x51U

/** \brief Defines API id of function Mirror_ReportFlexRayFrame(). */
#if (defined MIRROR_SID_REPORTFLEXRAYFRAME)
#error MIRROR_SID_REPORTFLEXRAYFRAME already defined!
#endif /* #if (defined MIRROR_SID_REPORTFLEXRAYFRAME)*/
#define MIRROR_SID_REPORTFLEXRAYFRAME 0x52U

/** \brief Defines API id of function Mirror_ReportFlexRayChannelStatus(). */
#if (defined MIRROR_SID_REPORTFLEXRAYCHANNELSTATUS)
#error MIRROR_SID_REPORTFLEXRAYCHANNELSTATUS already defined!
#endif /* #if (defined MIRROR_SID_REPORTFLEXRAYCHANNELSTATUS)*/
#define MIRROR_SID_REPORTFLEXRAYCHANNELSTATUS 0x53U

/*------------------[Det error codes]----------------------------------------*/

/*------------------[Develepment Errors]-----------------*/

/** \brief Definition of DET error code MIRROR_E_UNINIT */
/* !LINKSTO Mirror.ASR44.SWS_Mirror_00007,1 */
#if (defined MIRROR_E_UNINIT)
#error MIRROR_E_UNINIT already defined!
#endif /* #if (defined MIRROR_E_UNINIT)*/
#define MIRROR_E_UNINIT 0x01U /** MIRROR module not initialized */

/** \brief Definition of DET error code MIRROR_E_REINIT */
/* !LINKSTO Mirror.ASR44.SWS_Mirror_00007,1 */
#if (defined MIRROR_E_REINIT)
#error MIRROR_E_REINIT already defined!
#endif /* #if (defined MIRROR_E_REINIT)*/
#define MIRROR_E_REINIT 0x02U /** The init API was called twice */

/** \brief Definition of DET error code MIRROR_E_INIT_FAILED */
/* !LINKSTO Mirror.ASR44.SWS_Mirror_00007,1 */
#if (defined MIRROR_E_INIT_FAILED)
#error MIRROR_E_INIT_FAILED already defined!
#endif /* #if (defined MIRROR_E_INIT_FAILED)*/
#define MIRROR_E_INIT_FAILED 0x03U /** Mirror_Init was called with an invalid configuration pointer */

/** \brief Definition of DET error code MIRROR_E_PARAM_POINTER */
/* !LINKSTO Mirror.ASR44.SWS_Mirror_00007,1 */
#if (defined MIRROR_E_PARAM_POINTER)
#error MIRROR_E_PARAM_POINTER already defined!
#endif /* #if (defined MIRROR_E_PARAM_POINTER)*/
#define MIRROR_E_PARAM_POINTER 0x10 /** An API service was called with a NULL pointer */

/** \brief Definition of DET error code MIRROR_E_INVALID_PDU_SDU_ID */
/* !LINKSTO Mirror.ASR44.SWS_Mirror_00007,1 */
#if (defined MIRROR_E_INVALID_PDU_SDU_ID)
#error MIRROR_E_INVALID_PDU_SDU_ID already defined!
#endif /* #if (defined MIRROR_E_INVALID_PDU_SDU_ID)*/
#define MIRROR_E_INVALID_PDU_SDU_ID 0x11 /** An API service was called with a wrong ID */

/** \brief Definition of DET error code MIRROR_E_INVALID_NETWORK_ID */
/* !LINKSTO Mirror.ASR44.SWS_Mirror_00007,1 */
#if (defined MIRROR_E_INVALID_NETWORK_ID)
#error MIRROR_E_INVALID_NETWORK_ID already defined!
#endif /* #if (defined MIRROR_E_INVALID_NETWORK_ID)*/
#define MIRROR_E_INVALID_NETWORK_ID 0x12 /** An API service was called with wrong network handle */

/** \brief Definition of DET error code MIRROR_E_INVALID_CALL */
#if (defined MIRROR_E_INVALID_CALL)
#error MIRROR_E_INVALID_CALL already defined!
#endif /* #if (defined MIRROR_E_INVALID_CALL)*/
#define MIRROR_E_INVALID_CALL 0x13 /** A dummy API was called */

/** \brief Definition of DET error code MIRROR_E_INVALID_PARAM */
#if (defined MIRROR_E_INVALID_PARAM)
#error MIRROR_E_INVALID_PARAM already defined!
#endif /* #if (defined MIRROR_E_INVALID_PARAM)*/
#define MIRROR_E_INVALID_PARAM 0x14 /** A function was called with invalid parameter */

/** \brief Definition of DET error code MIRROR_E_INVALID_STATUS */
#if (defined MIRROR_E_INVALID_STATUS)
#error MIRROR_E_INVALID_PARAM already defined!
#endif /* #if (defined MIRROR_E_INVALID_STATUS)*/
#define MIRROR_E_INVALID_STATUS 0x15 /** A function was called with invalid status */

/** \brief Definition of DET error code MIRROR_E_INVALID_CONTROLLER_ID */
#if (defined MIRROR_E_INVALID_CONTROLLER_ID)
#error MIRROR_E_INVALID_CONTROLLER_ID already defined!
#endif /* #if (defined MIRROR_E_INVALID_CONTROLLER_ID)*/
#define MIRROR_E_INVALID_CONTROLLER_ID 0x16 /** An API service was called with wrong controller ID */

/** \brief Definition of DET error code MIRROR_E_INVALID_CLUSTER_ID */
#if (defined MIRROR_E_INVALID_CLUSTER_ID)
#error MIRROR_E_INVALID_CLUSTER_ID already defined!
#endif /* #if (defined MIRROR_E_INVALID_CLUSTER_ID)*/
#define MIRROR_E_INVALID_CLUSTER_ID 0x17 /** An API service was called with wrong cluster ID */

/** \brief Definition of DET error code MIRROR_E_INVALID_CHANNEL */
#if (defined MIRROR_E_INVALID_CHANNEL)
#error MIRROR_E_INVALID_CHANNEL already defined!
#endif /* #if (defined MIRROR_E_INVALID_CHANNEL)*/
#define MIRROR_E_INVALID_CHANNEL 0x18U /** A function was called with invalid channel */

/*------------------[Runtime Errors]-------------------*/

/** \brief Definition of DET error code MIRROR_E_QUEUE_OVERRUN */
/* !LINKSTO Mirror.ASR44.SWS_Mirror_00008,1 */
#if (defined MIRROR_E_QUEUE_OVERRUN)
#error MIRROR_E_QUEUE_OVERRUN already defined!
#endif /* #if (defined MIRROR_E_QUEUE_OVERRUN)*/
#define MIRROR_E_QUEUE_OVERRUN 0x40U /** A message could not be stored in the queue */

/** \brief Definition of DET error code MIRROR_E_TRANSMIT_FAILED */
/* !LINKSTO Mirror.ASR44.SWS_Mirror_00008,1 */
#if (defined MIRROR_E_TRANSMIT_FAILED)
#error MIRROR_E_TRANSMIT_FAILED already defined!
#endif /* #if (defined MIRROR_E_TRANSMIT_FAILED)*/
#define MIRROR_E_TRANSMIT_FAILED 0x41U /** A message could not be transmitted */

/** \brief Definition of DET error code MIRROR_E_INTERMEDIATE_BUFFER_OVERRUN */
/* !LINKSTO Mirror.ASR44.SWS_Mirror_00008,1 */
#if (defined MIRROR_E_INTERMEDIATE_BUFFER_OVERRUN)
#error MIRROR_E_INTERMEDIATE_BUFFER_OVERRUN already defined!
#endif /* #if (defined MIRROR_E_INTERMEDIATE_BUFFER_OVERRUN)*/
#define MIRROR_E_INTERMEDIATE_BUFFER_OVERRUN 0x42U /** A message could not be stored in the intermediate elements buffer */

/** \brief Definition of DET error code MIRROR_E_NESTED_REPORT_FRAMES */
#if (defined MIRROR_E_NESTED_REPORT_FRAMES)
#error MIRROR_E_NESTED_REPORT_FRAMES already defined!
#endif /* #if (defined MIRROR_E_NESTED_REPORT_FRAMES)*/
#define MIRROR_E_NESTED_REPORT_FRAMES 0x43U /** A report frame function was preempted by another call of it */

/*------------------[Det Reporting functions]-------------------*/

#if(MIRROR_DEV_ERROR_DETECT == STD_ON)
#if (defined MIRROR_DET_REPORT_ERROR)
#error MIRROR_DET_REPORT_ERROR already defined!
#endif /* #if (defined MIRROR_DET_REPORT_ERROR)*/
#define MIRROR_DET_REPORT_ERROR(ApiId, ErrorId) \
  (void) Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID, (ApiId), (ErrorId))
#endif /* MIRROR_DEV_ERROR_DETECT == STD_ON */

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define MIRROR_START_SEC_CODE
#include <Mirror_MemMap.h>

/**
 * \brief Initializes the Mirror module.
 *
 * This service initializes the Mirror module. It shall be the first function of the module to be
 * called.
 *
 * \param[in] config - Base pointer to the configuration structure of the Mirror module.
 *
 * \ServiceID{0x01}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, MIRROR_CODE) Mirror_Init
(
  P2CONST(Mirror_ConfigType, AUTOMATIC, MIRROR_APPL_CONST) config
);

/**
 * \brief Deinitializes the Mirror module.
 *
 * This function resets the Bus Mirroring module to the uninitialized state.
 * It could be called after the intiallization of the module.
 *
 * \ServiceID{0x02}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, MIRROR_CODE) Mirror_DeInit
(
  void
);

#if(MIRROR_VERSION_INFO_API == STD_ON)
/**
 * \brief API to get the version information of Mirror module.
 *
 * This service returns the version information of this module.
 *
 * \param[out] VersionInfo - Pointer to where to store the version information of this module.
 *
 * \ServiceID{0x03}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */

extern FUNC(void, MIRROR_CODE) Mirror_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, MIRROR_APPL_DATA) VersionInfo
);
#endif /* MIRROR_VERSION_INFO_API */

/**
 * \brief Returns the global mirroring state.
 *
 * This service returns true if the Mirror is active and false if the Mirror is inactive.
 *
 ** \retval TRUE: Mirror is active.
 ** \retval FALSE: Mirror is inactive.
 *
 * \ServiceID{0x20}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(boolean, MIRROR_CODE) Mirror_IsMirrorActive
(
  void
);

/**
 * \brief Completely disables any mirroring activities.
 *
 * This service resets source buses are to disabled, queued messages are purged,
 * and the destination bus is reset to the default destination.
 *
 * \ServiceID{0x13}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, MIRROR_CODE) Mirror_Offline
(
  void
);

/**
 * \brief Returns the currently selected destination bus.
 *
 * This service returns the currently active destination network.
 *
 * \retval NetworkHandleType: The currently selected destination bus.
 *
 * \ServiceID{0x21}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(NetworkHandleType, MIRROR_CODE) Mirror_GetDestNetwork
(
  void
);

/**
 * \brief Returns the state of a source bus.
 *
 * This service returns true if the source bus is started and false if not started.
 *
 * \param[in] network - The ComM channel Id of the source network.
 *
 ** \retval TRUE: Source bus is active.
 ** \retval FALSE: Source bus is inactive.
 *
 * \ServiceID{0x22}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(boolean, MIRROR_CODE) Mirror_IsSourceNetworkStarted
(
  VAR(NetworkHandleType, TYPEDEF) network
);

/**
 * \brief Activates all source bus.
 *
 * \ServiceID{0x08}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, MIRROR_CODE) Mirror_StartAllSourceNetworks
(
  void
);

/**
 * \brief Dectivates all source bus.
 *
 * \ServiceID{0x09}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, MIRROR_CODE) Mirror_StopAllSourceNetworks
(
  void
);

/**
 * \brief Activates a source bus.
 *
 * This service return E_OK if the activation of the source bus is successful and E_NOT_OK if the activation failed.
 *
 * \param[in] network - The ComM channel Id of the destination network.
 *
 ** \retval E_OK: Source bus was activated.
 ** \retval E_NOT_OK: Function was called with invalid parameters.
 *
 * \ServiceID{0x10}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, MIRROR_CODE) Mirror_StartSourceNetwork
(
  VAR(NetworkHandleType, TYPEDEF) network
);

/**
 * \brief Deactivates a source bus.
 *
 * This service return E_OK if the deactivation of the source bus is successful and E_NOT_OK if the deactivation failed.
 *
 * \param[in] network - The ComM channel Id of the destination network.
 *
 ** \retval E_OK: Source bus was deactivated.
 ** \retval E_NOT_OK: Function was called with invalid parameters.
 *
 * \ServiceID{0x11}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, MIRROR_CODE) Mirror_StopSourceNetwork
(
  VAR(NetworkHandleType, TYPEDEF) network
);

/**
 * \brief Returns the network type of the given network.
 *
 * This service returns the network type of the given network and it would be LIN, CAN or IP network type
 *
 * \param[in] network - The ComM channel Id of source or destination network.
 *
 ** \retval MIRROR_NT_INVALID if the bus is not configured.
 ** \retval MIRROR_NT_CAN: if the network is CAN type.
 ** \retval MIRROR_NT_LIN: if the network is LIN type.
 ** \retval MIRROR_NT_ETHERNET: if the network is Ethernet type.
 *
 * \ServiceID{0x24}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Mirror_NetworkType, MIRROR_CODE) Mirror_GetNetworkType
(
  VAR(NetworkHandleType, TYPEDEF) network
);

/**
 * \brief Returns the Mirror network ID of the given network.
 *
 * This service returns the Mirror network ID of the given network and if the network entered isn't valid it returns 0xFF
 *
 * \param[in] network - The ComM channel Id of source or destination network.
 *
 ** \retval The Mirror network ID corresponding to the entered ComM network ID .
 ** \retval 0xFF: if the ComM network ID is invalid.
 *
 * \ServiceID{0x25}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(uint8, MIRROR_CODE) Mirror_GetNetworkId
(
  VAR(NetworkHandleType, TYPEDEF) network
);

/**
 * \brief Returns the network type of the given network.
 *
 * This service returns the ComM network Id of the given Mirror network ID and the Mirror_NetworkType.
 *
 * \param[in] networkType - The type of the Mirror network.
 * \param[in] networkId - ComM channel Mirror network ID
 *
 ** \retval MIRROR_INVALID_NETWORK: if no configured network corresponds to the given combination.
 ** \retval NetworkHandleType: The ComM handle ID if the network corresponds to the given combination.
 *
 * \ServiceID{0x26}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(NetworkHandleType, MIRROR_CODE) Mirror_GetNetworkHandle
(
  VAR(Mirror_NetworkType, TYPEDEF) networkType,
  VAR(uint8, TYPEDEF) networkId
);

/**
 * \brief Mirror module main function.
 *
 * This function performs the processing of the AUTOSAR MIRROR module's destination network activities.
 *
 * \ServiceID{0x04}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, MIRROR_CODE) Mirror_MainFunction
(
  void
);

#define MIRROR_STOP_SEC_CODE
#include <Mirror_MemMap.h>

#if(MIRROR_CAN_SOURCENETWORK == STD_ON)

#define MIRROR_START_SEC_CAN_CODE
#include <Mirror_MemMap.h>

/**
 * \brief Mirror module Report Can frame callback function.
 *
 * This callback is called by the lower layer of the CanIf to Report an incoming frame
 *
 * \param[in] controllerId - ID of the CAN controller that received or transmitted the frame.
 * \param[in] canId - CAN ID of the CAN frame.
 * \param[in] length - length Length of the CAN frame.
 * \param[in] payload - Content of the CAN frame.
 *
 * \ServiceID{0x50}
 * \Reentrancy{Non reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, MIRROR_CODE) Mirror_ReportCanFrame
(
  VAR(uint8, TYPEDEF) controllerId,
  VAR(Can_IdType, TYPEDEF) canId,
  VAR(uint8, TYPEDEF) length,
  P2CONST(uint8, AUTOMATIC, MIRROR_APPL_DATA) payload
);

#define MIRROR_STOP_SEC_CAN_CODE
#include <Mirror_MemMap.h>

#endif /* MIRROR_CAN_SOURCENETWORK == STD_ON */

#if(MIRROR_LIN_SOURCENETWORK == STD_ON)

#define MIRROR_START_SEC_LIN_CODE
#include <Mirror_MemMap.h>

/**
 * \brief Mirror module Report Lin frame callback function.
 *
 * This callback is called by the lower layer of the LinIf to Report an incoming frame
 *
 * \param[in] network - ComM channel associated with the LIN channel on
 *                      which the frame was received or transmitted.
 * \param[in] pid - Protected ID of the LIN frame.
 * \param[in] pdu - Content of the LIN frame.
 * \param[in] status - Rx/Tx status of the frame access through the LIN driver.
 *
 * \ServiceID{0x51}
 * \Reentrancy{Non reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, MIRROR_CODE) Mirror_ReportLinFrame
(
  VAR(NetworkHandleType, TYPEDEF) network,
  VAR(Lin_FramePidType, TYPEDEF) pid,
  P2CONST(PduInfoType, AUTOMATIC, MIRROR_APPL_DATA) pdu,
  VAR(Lin_StatusType, TYPEDEF) status
);

#define MIRROR_STOP_SEC_LIN_CODE
#include <Mirror_MemMap.h>

#endif /* MIRROR_LIN_SOURCENETWORK == STD_ON */

#if(MIRROR_FLEXRAY_SOURCENETWORK == STD_ON)

#define MIRROR_START_SEC_FLEXRAY_CODE
#include <Mirror_MemMap.h>

/**
 * \brief Mirror module Report FlexRay frame callback function.
 *
 * This callback is called by the lower layer of the FrIf to Report an incoming frame
 *
 * \param[in] controllerId - FlexRay controller that received/transmitted the frame.
 * \param[in] slotId - ID of the slot in which the received/transmitted frame is located.
 * \param[in] cycle - Cycle in which the reception/transmission takes place.
 * \param[in] frChannel - FlexRay channel(s) on which the reception/transmission takes place.
 * \param[in] frame - Content of the FlexRay frame, or NULL when a txConflict is reported..
 * \param[in] txConflict - TRUE in case a txConflict has been detected, FALSE otherwise.
 *
 * \ServiceID{0x52}
 * \Reentrancy{Non reentrant}
 * \Synchronicity{Non Synchronous}
 */
extern FUNC(void, MIRROR_CODE) Mirror_ReportFlexRayFrame
(
  VAR(uint8, TYPEDEF) controllerId,
  VAR(uint16, TYPEDEF) slotId,
  VAR(uint8, TYPEDEF) cycle,
  VAR(Fr_ChannelType, TYPEDEF) frChannel,
  P2CONST(PduInfoType, AUTOMATIC, MIRROR_APPL_DATA) frame,
  VAR(boolean, TYPEDEF) txConflict
);

/**
 * \brief Mirror module Report FlexRay channel status callback function.
 *
 * This callback is called by the lower layer of the FrIf to Report an incoming frame
 *
 * \param[in] uint8 clusterId, - FlexRay cluster for which the status is reported.
 * \param[in] channelAStatus - Status of FlexRay channel A.
 * \param[in] channelBStatus - Status of FlexRay channel B.
 *
 * \ServiceID{0x53}
 * \Reentrancy{Reentrant for different clusterIds. Non reentrant for the same clusterId}
 * \Synchronicity{Non Synchronous}
 */
extern FUNC(void, MIRROR_CODE) Mirror_ReportFlexRayChannelStatus
(
  VAR(uint8, TYPEDEF) clusterId,
  VAR(uint16, TYPEDEF) channelAStatus,
  VAR(uint16, TYPEDEF) channelBStatus
);

#define MIRROR_STOP_SEC_FLEXRAY_CODE
#include <Mirror_MemMap.h>

#endif /* MIRROR_FLEXRAY_SOURCENETWORK == STD_ON */

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef MIRROR_H */
/*==================[end of file]===========================================*/
