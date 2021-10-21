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
/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 15.4 (advisory)
 * There should be no more than one break or goto statement used to terminate any iteration statement.
 *
 * Reason:
 * In case UDP protocol is used there are two possible socket connections. The
 * implementation is similar for both connections and the loop has to terminate in both
 * cases.
 *
 * MISRAC2012-2) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type
 *
 * Reason:
 * The type of variable holding the address expected by SoAd_GetRemoteAddr and
 * SoAd_SetRemoteAddr functions is different from the type used in Xcp.
 *
 * MISRAC2012-3) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears in a single function
 *
 * Reason:
 * Using the variable at function scope would require making the variable static, which
 * would prevent allocation to a predefined memory section.
 *
 * MISRAC2012-4) Deviated Rule: 13.5 (required)
 * The right hand operand of a logical && or || operator shall not contain persistent side effects.
 *
 * Reason:
 * GetApplicationID() doesn't have any side effect.
 */

/*==================[inclusions]============================================*/

#include <Xcp.h>              /* Module public API */
#if ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) )

#include <TSAutosar.h>        /* EB specific standard types */
#include <TSAtomic_Assign.h>   /* EB specific standard types */
#include <Xcp_Trace.h>

#include <Xcp_Int.h>          /* Internal types */
#include <TSMem.h>            /* EB memory functions */
#include <Xcp_Cbk.h>          /* Callbacks and callouts of Xcp */
#if (XCP_RESOURCE_CAL_PAG == XCP_RESOURCE_CAL_PAG_MASK)
#include <Xcp_UserCallouts.h> /* User Callout Function declarations */
#endif

#include <SchM_Xcp.h>           /* Needed for exclusive area definition */

#if (XCP_MEMORY_ACCESS_AREAS == STD_ON)
#include <Os.h>                  /* Os counter is required */
#endif

/*==================[macros]================================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef XCP_VENDOR_ID /* configuration check */
#error XCP_VENDOR_ID must be defined
#endif

#if (XCP_VENDOR_ID != 1U) /* vendor check */
#error XCP_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef XCP_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error XCP_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (XCP_AR_RELEASE_MAJOR_VERSION != 4U)
#error XCP_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef XCP_AR_RELEASE_MINOR_VERSION /* configuration check */
#error XCP_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (XCP_AR_RELEASE_MINOR_VERSION != 0U )
#error XCP_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef XCP_AR_RELEASE_REVISION_VERSION /* configuration check */
#error XCP_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (XCP_AR_RELEASE_REVISION_VERSION != 3U )
#error XCP_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef XCP_SW_MAJOR_VERSION /* configuration check */
#error XCP_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (XCP_SW_MAJOR_VERSION != 2U)
#error XCP_SW_MAJOR_VERSION wrong (!= 2U)
#endif

#ifndef XCP_SW_MINOR_VERSION /* configuration check */
#error XCP_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (XCP_SW_MINOR_VERSION < 12U)
#error XCP_SW_MINOR_VERSION wrong (< 12U)
#endif

#ifndef XCP_SW_PATCH_VERSION /* configuration check */
#error XCP_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (XCP_SW_PATCH_VERSION < 0U)
#error XCP_SW_PATCH_VERSION wrong (< 0U)
#endif


#if (defined XCP_OVER_CAN)
#error XCP_OVER_CAN already defined
#endif
/** \brief macro defining that the lower communication layer is the Can */
#define XCP_OVER_CAN                                       0U

#if (defined XCP_OVER_FLEXRAY)
#error XCP_OVER_FLEXRAY already defined
#endif
/** \brief macro defining that the lower communication layer is the FlexRay */
#define XCP_OVER_FLEXRAY                                   1U

#if (defined XCP_OVER_ETHERNET)
#error XCP_OVER_ETHERNET already defined
#endif
/** \brief macro defining that the lower communication layer is the Ehternet */
#define XCP_OVER_ETHERNET                                  2U

#if (defined XCP_OVER_CDD)
#error XCP_OVER_CDD already defined
#endif
/** \brief macro defining that the lower communication layer is a CDD */
#define XCP_OVER_CDD                                       3U

#if (defined XCP_RX_PDU_DIRECTION)
#error XCP_RX_PDU_DIRECTION already defined
#endif
/** \brief macro defining that the PDU routing direction is Rx */
#define XCP_RX_PDU_DIRECTION                               0U

#if (defined XCP_TX_PDU_DIRECTION)
#error XCP_TX_PDU_DIRECTION already defined
#endif
/** \brief macro defining that the PDU routing direction is Tx */
#define XCP_TX_PDU_DIRECTION                               1U

/*==================[type definitions]======================================*/

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)

/** \brief Structure to hold the remote multicast and unicast addresses. The
 *         unicast address needs to be restored after a multicast transmit.
 * . */
typedef struct {
  /* Remote multicast address (only IPV4) */
  TcpIp_SockAddrInetType MulticastAddr;
  /* Remote unicast address (only IPV4) */
  TcpIp_SockAddrInetType UnicastAddr;
} Xcp_RemoteAddrData_t;

#endif /* XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON */

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

/** \brief Function used to retrieved the internal mapping route Pdu Id for the received Pdu Id.
 **
 ** \param[in]  CommunicationBusType  Communication bus type.
 ** \param[in]  PduDirection  Pdu direction(Tx or Rx).
 ** \param[in]  TLPduId  notified transport layer PDU-ID.
 ** \param[out] XcpInternalPduIdPtr  Internal Id which is mapped towards the TL one.
 ** \return     The operation result.
 **/
STATIC FUNC(Std_ReturnType,XCP_CODE) Xcp_GetPduInternalRouteId
(
  uint8 CommunicationBusType,
  uint8 PduDirection,
  PduIdType TLPduId,
  P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) XcpInternalPduIdPtr
);

/** \brief XCP PDU is received.
 **
 ** This function is called by the lower layer when an AUTOSAR XCP PDU has
 ** been received.
 **
 **
 ** \param[in]  CommunicationBusType  Communication bus type.
 ** \param[in]  XcpRxPduId  PDU-ID that has been received.
 ** \param[in]  XcpRxPduPtr  Contains the length (SduLength) of the received I-PDU and a pointer to
 **                          a buffer (SduDataPtr) containing the I-PDU.
 **
 ** \ServiceID{::XCP_SID_IF_RX_INDICATION}
 ** \Reentrancy{Reentrant for different XcpRxPduId. Non-reentrant for the same XcpRxPduId}
 ** \Synchronicity{Synchronous} */
STATIC FUNC(void,XCP_CODE) Xcp_CommonRxIndication
(
  uint8 CommunicationBusType,
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
);

/** \brief XCP PDU transmit confirmation.
 **
 ** This function is called by the lower layer when an AUTOSAR XCP PDU has
 ** been transmitted.
 **
 ** \param[in]  CommunicationBusType  Communication bus type.
 ** \param[in]  XcpTxPduId  PDU-ID that has been transmitted.
 **
 ** \ServiceID{::XCP_SID_IF_TX_CONFIRMATION}
 ** \Reentrancy{Reentrant for different XcpTxPduId. non-Reentrant for the same XcpTxPduId}
 ** \Synchronicity{Synchronous} */
STATIC FUNC(void, XCP_CODE) Xcp_CommonTxConfirmation
(
  uint8 CommunicationBusType,
  PduIdType XcpTxPduId
);

#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
/** \brief Call when an AUTOSAR XCP PDU shall be transmitted.
 **
 ** This function is called by the lower layer when an AUTOSAR XCP PDU
 ** shall be transmitted. The function Xcp_CommonTriggerTransmit is called by
 ** the lower layer Interface for requesting the I-PDU before transmission.
 ** Whether the function Xcp_CommonTriggerTransmit is called or not is
 ** statically configured for each I-PDU in the configuration.
 **
 ** \param[in]  CommunicationBusType  Communication bus type.
 ** \param[in]  XcpTxPduId  PDU-ID that has been requested to be transmitted.
 ** \param[out]  PduInfoPtr  Pointer to PduInfo SDU (pointer to SDU buffer and
 **                          SDU length).
 ** \return Function result
 ** \retval E_OK SDU has been copied and SduLength indicates the number of
 **              copied bytes.
 ** \retval E_NOT_OK No SDU has been copied. PduInfoPtr must not be used since
 **                  it may contain a NULL pointer or point to invalid data.
 **
 ** \ServiceID{::XCP_SID_IF_TRIGGER_TRANSMIT}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_CommonTriggerTransmit
(
  uint8 CommunicationBusType,
  PduIdType XcpTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr
);
#endif /* ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON)) */

#if (XCP_MAX_DAQ != 0U)
/** \brief Function used to initialize all DAQ lists
 **
 ** The function will initialize defaults data from ROM. The initialization is done only
 ** if the data was not initialized from the NV memory by the NvM_ReadAll() service (if DAQ storage
 ** is supported and requested).
 **
 ** \return Result of the operation.
 ** \retval E_OK if the DAQ lists were initialized with no error
 ** \retval E_NOT_OK if the DET error XCP_E_INVALID_NVM_BLOCK_LENGTH is thrown.
 **
 ** */
STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_InitializeDaqLists(void);
#endif

#if (XCP_SOAD_AUTO_OPEN_SO_CON == STD_ON)
/** \brief Function used to initialize SoAd connections
 **
 ** The function will initialize the SoAd connections, by opening
 ** the required ones.
 **
 ** \return Result of the operation.
 ** \retval E_OK if the initialization of required SoAd is successful
 ** \retval E_NOT_OK if the DET error XCP_E_SOCKET_CONNECTION_INIT is thrown.
 **
 ** */
STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_InitSoAdConnetions(void);
#endif

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define XCP_START_SEC_VAR_8
#include <Xcp_MemMap.h>

/* Initialization status of the XCP module */
VAR(boolean, XCP_VAR) Xcp_Initialized = FALSE;
/* Xcp_MainFunction processing status */
/* Deviation MISRAC2012-3 <START> */
STATIC VAR(boolean, XCP_VAR) Xcp_MainFunction_Running = FALSE;
/* Deviation MISRAC2012-3 <STOP> */


#if (XCP_SUPPRESS_TX_SUPPORT == STD_ON)
/* Channel (connection) Tx status (initially all are enabled) */
VAR(uint8, XCP_VAR) Xcp_TransmissionDisabled[XCP_CONNECTION_FLAG_SIZE] = {FALSE};
#endif

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
VAR(uint8, XCP_VAR) Xcp_MemoryProxyErrorPending;
VAR(uint8, XCP_VAR) Xcp_CyclicEventOverloaded;
#endif

#define XCP_STOP_SEC_VAR_8
#include <Xcp_MemMap.h>

#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

/* Connection status of XCP module */
VAR(Xcp_ConnectionStatusType, XCP_VAR) Xcp_ConnectionStatus =
{
  XCP_STATE_DISCONNECTED,
  XCP_INIT_CONNECTION_ID,
  0U
};

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
VAR(Atomic_t, XCP_VAR) Xcp_PartitionOfActiveConnection = ATOMICS_OBJECT_INITIALIZER(XCP_NUM_OF_OS_APPS);
VAR(Atomic_t, XCP_VAR) Xcp_ActiveConnectionId = ATOMICS_OBJECT_INITIALIZER(XCP_NO_ACTIVE_CONNECTION);
#endif

#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

/*==================[NOINIT external data]=========================================*/

#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>

#if (XCP_MAX_DAQ != 0U)

/* Queue for holding the DAQ Priority */
VAR(Xcp_DaqPrioQueueType, XCP_VAR_CLEARED) Xcp_DaqPrioQueue;

/* Mirror for holding the DTO data */
VAR(Xcp_AllDtoDataMirrorType, XCP_VAR_CLEARED) Xcp_DTOEntryMirror;

/* Buffer for holding Time stamp information and information for stimulation  */
VAR(Xcp_DaqRunTimeInfoType, XCP_VAR_CLEARED) Xcp_DaqRunTimeInfo[XCP_MAX_DAQ];
#endif

#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>

/*==================[internal data]=========================================*/

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>
/* Structure to hold the multicast and unicast remote addresses */
STATIC VAR(Xcp_RemoteAddrData_t, XCP_VAR_CLEARED) Xcp_RemoteAddrData;
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>
#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */

/*==================[external function definitions]=========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

#if (XCP_VERSION_INFO_API == STD_ON)
/*------------------[Xcp_GetVersionInfo]-------------------------------------*/

FUNC(void, XCP_CODE) Xcp_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, XCP_APPL_DATA) VersionInfoPtr
)
{
  DBG_XCP_GETVERSIONINFO_ENTRY();

#if (XCP_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == VersionInfoPtr)
  {
    XCP_DET_REPORT_ERROR(XCP_SID_GET_VERSION_INFO, XCP_E_INV_POINTER);
  }
  else
#endif
  {
    VersionInfoPtr->vendorID         = XCP_VENDOR_ID;
    VersionInfoPtr->moduleID         = XCP_MODULE_ID;
    VersionInfoPtr->sw_major_version = XCP_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = XCP_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = XCP_SW_PATCH_VERSION;
  }

  DBG_XCP_GETVERSIONINFO_EXIT(VersionInfoPtr);
}

#endif /* (XCP_VERSION_INFO_API == STD_ON) */

/*------------------[Xcp_Init]-----------------------------------------------*/

FUNC(void,XCP_CODE) Xcp_Init
(
  P2CONST(Xcp_ConfigType, AUTOMATIC, XCP_APPL_CONST) Xcp_ConfigPtr
)
{
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
  if ((uint8)GetApplicationID() == XCP_MASTER_APPLICATION)
  {
#if ((XCP_REMOTE_DESTINATIONS == STD_ON) && (XCP_NO_OF_CONNECTIONS == 1U))
    Xcp_RemoteDispatchType data;

    data.type = XCP_REMOTE_DISPATCH_INIT;
#endif

    Xcp_Init_Internal(Xcp_ConfigPtr);
#if ((XCP_REMOTE_DESTINATIONS == STD_ON) && (XCP_NO_OF_CONNECTIONS == 1U))
    (void) Xcp_RemoteDispatch_Handler(data);
#endif
  }
#else
  Xcp_Init_Internal(Xcp_ConfigPtr);
#endif
}

FUNC(void,XCP_CODE) Xcp_Init_Internal
(
  P2CONST(Xcp_ConfigType, AUTOMATIC, XCP_APPL_CONST) Xcp_ConfigPtr
)
{
  DBG_XCP_INIT_ENTRY(Xcp_ConfigPtr);

  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
    /* Present implementation doesn't have any post build parameters */
    TS_PARAM_UNUSED(Xcp_ConfigPtr);

#if (XCP_NUMBER_OF_FRAME_COUNTERS > 0U)
    /* Initialize frame transmit counter of XCP frame header */
    Xcp_InitFrameCtr();
#endif /* (XCP_NUMBER_OF_FRAME_COUNTERS > 0U) */

    /* Flush command queue */
    Xcp_FlushCMDQueue();
    /* Flush CTO Queue */
    Xcp_FlushCTOQueue();
  /* If event packet transmission enabled */
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
    /* Flush XCP events CTOs */
    Xcp_FlushEventCTOQueue();
#endif /* (XCP_EVENT_PACKET_ENABLED == STD_ON) */

    /* Initialize receive processor */
    Xcp_InitReceiveProcessor();
    /* Initialize command processor */
    Xcp_InitCommandProcessor();
    /* Initialize transmit processor */
    Xcp_InitTransmitProcessor();

    /* If CAL/PAG resource is supported */
#if (XCP_RESOURCE_CAL_PAG == XCP_RESOURCE_CAL_PAG_MASK)
    /* Initialize Calibration Segments and Pages */
    Xcp_ApplCalPagInit();
#endif /* (XCP_RESOURCE_CAL_PAG == XCP_RESOURCE_CAL_PAG_MASK) */

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
    Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_NO_USER_CALLOUT_REQUESTED);

#endif
    /* If we have DAQ lists configured */
#if (XCP_MAX_DAQ != 0U)

    /* Reset all event channels runtime information */
    Xcp_ResetEventChannels();
    /* Flush DTO Queue */
    Xcp_FlushDTORunTimeData();

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
    /* Reset all request data */
    Xcp_ResetRequestData();
#endif

    /* release first and second level lock */
    XCP_RELEASE_FIRST_LEVEL_LOCK();
    XCP_RELEASE_SECOND_LEVEL_LOCK();

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
    Atomics_Store(&Xcp_ActiveConnectionId, XCP_NO_ACTIVE_CONNECTION);
#endif
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
    Xcp_MemoryProxyErrorPending = FALSE;
    Xcp_CyclicEventOverloaded = FALSE;
#endif

    /* Initialize DAQ lists */
    if (Xcp_InitializeDaqLists() != E_OK)
    {
      /* Tag module as UnInitialized */
      Xcp_Initialized = FALSE;
    }
    else
#endif /* (XCP_MAX_DAQ != 0U) */
    {
#if (XCP_SOAD_AUTO_OPEN_SO_CON == STD_ON)
      if (Xcp_InitSoAdConnetions() != E_OK)
      {
        /* Tag module as UnInitialized */
        Xcp_Initialized = FALSE;
      }
      else
#endif /* (XCP_SOAD_AUTO_OPEN_SO_CON == STD_ON) */
      {
        /* Tag module as Initialized */
        Xcp_Initialized = TRUE;
      }
    }
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }

  DBG_XCP_INIT_EXIT();
}

#if ((XCP_BSW_DISTRIBUTION_ENABLED == STD_ON) && (XCP_NO_OF_CONNECTIONS == 1U))
FUNC(void,XCP_CODE) Xcp_Init_OnSlave
(
  void
)
{
  Xcp_InitRxPduFlags(0U);
}
#endif

/*------------------[Xcp_MainFunction]---------------------------------------*/

FUNC(void,XCP_CODE) Xcp_MainFunction(void)
{
  boolean ExecuteMainFunction = FALSE;   /* Indicates whether main function must be continued. */
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
  Atomic_value_t activeConnectionId = Atomics_Load(&Xcp_ActiveConnectionId);
#endif
  DBG_XCP_MAINFUNCTION_ENTRY();

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
  /* Deviation MISRAC2012-4 <START> */
  if (
       (activeConnectionId == XCP_NO_ACTIVE_CONNECTION)
       ||
       (
         (activeConnectionId != XCP_NO_ACTIVE_CONNECTION) &&
         (Xcp_ConnectionToPartitionMap[activeConnectionId] != (uint8)GetApplicationID())
       )
     )
  /* Deviation MISRAC2012-4 <STOP> */
  {
    return;
  }
#endif

  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    /* check if the Xcp_MainFunction is already being processed */
    if ((Xcp_MainFunction_Running == FALSE) &&
        (Xcp_Initialized == TRUE))
    {
      Xcp_MainFunction_Running = TRUE;
      ExecuteMainFunction = TRUE;
    }
    /* Exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    /* Check whether XCP is initialized  and the Xcp_MainFunction
     * is not already being processed */
    if (ExecuteMainFunction == TRUE)
    {
      boolean IsDisconnectTriggered;
      /* Process the Bus Monitor functionality
       * This function has to be called at the
       * beginning of the Xcp_MainFunction execution */
      Xcp_ProcessBusMonitor(&IsDisconnectTriggered);

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
#if (XCP_NO_OF_CONNECTIONS > 1U)
      /* If there is a disconnect triggered from Xcp_ProcessBusMonitor or if there is no message
       * on the bus or the message is different then FlexRay, we can continue */
      if (IsDisconnectTriggered ||
          (Xcp_ConnectionStatus.ConnectionId == XCP_INVALID_CONNECTION_ID) ||
          (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].ConnectionType != XCP_CONNECTION_OVER_FLEXRAY) ||
          (XCP_IS_FLAG_SET(XCP_FLEXRAY_BUFFERS_READY_FLAG, Xcp_ConnectionStatus.Flags, uint8) == TRUE))
#else
      /* continue processing only when the FlexRay Hardware Buffers are initialized */
      if (IsDisconnectTriggered ||
          (XCP_IS_FLAG_SET(XCP_FLEXRAY_BUFFERS_READY_FLAG, Xcp_ConnectionStatus.Flags, uint8) == TRUE))
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
#endif /* ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)) */
      {
        /* Execute Receive processor */
        Xcp_ProcessReception(XCP_SID_MAIN_FUNCTION);

        /* Execute Command processor */
        Xcp_ProcessCommandQueue();
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
        Xcp_ProcessTransmit(XCP_SID_MAIN_FUNCTION);
#endif
#if (XCP_MAX_CYCLIC_EVENT_CHANNEL > 0U) && (XCP_MAX_DAQ > 0U)
        /* Set all the Cyclic Events */
        Xcp_SetCyclicEvents();
#endif

#if (XCP_MAX_MF_PROCESSED_EVENTS > 0)
        /* Execute Event processor */
        Xcp_ProcessMFEvents();
#endif
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_OFF)
        /* Execute Transmit processor */
        Xcp_ProcessTransmit(XCP_SID_MAIN_FUNCTION);
#endif
      }

      /* Enter critical section */
      SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      /* reset the processing status of Xcp_MainFunction */
      Xcp_MainFunction_Running = FALSE;
      SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    }
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }
  DBG_XCP_MAINFUNCTION_EXIT();
}

#if ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON))
/*------------------[Xcp_CanIfRxIndication]----------------------------------*/


FUNC(void,XCP_CODE) Xcp_CanIfRxIndication
(
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
)

{
  DBG_XCP_CANIFRXINDICATION_ENTRY(XcpRxPduId,XcpRxPduPtr);

  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (Xcp_Initialized == FALSE)
    {
      XCP_DET_REPORT_ERROR(XCP_SID_IF_RX_INDICATION, XCP_E_NOT_INITIALIZED);
    }
    else
#endif
    {
     Xcp_CommonRxIndication(XCP_OVER_CAN, XcpRxPduId, XcpRxPduPtr);
    }
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }
  DBG_XCP_CANIFRXINDICATION_EXIT();
}

/*------------------[Xcp_CanIfTxConfirmation]--------------------------------*/

FUNC(void, XCP_CODE) Xcp_CanIfTxConfirmation
(
  PduIdType XcpTxPduId
)
{
  DBG_XCP_CANIFTXCONFIRMATION_ENTRY(XcpTxPduId);

  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (Xcp_Initialized == FALSE)
    {
      XCP_DET_REPORT_ERROR(XCP_SID_IF_TX_CONFIRMATION, XCP_E_NOT_INITIALIZED);
    }
    else
#endif
    {
      Xcp_CommonTxConfirmation(XCP_OVER_CAN, XcpTxPduId);
    }
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }
  DBG_XCP_CANIFTXCONFIRMATION_EXIT();
}

#endif /* ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON)) */

#if (XCP_ON_CDD_ENABLED == STD_ON)
/*------------------[Xcp_CddRxIndication]----------------------------------*/

FUNC(void, XCP_CODE) Xcp_CddRxIndication
(
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
)
{
  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (Xcp_Initialized == FALSE)
    {
      XCP_DET_REPORT_ERROR(XCP_SID_IF_RX_INDICATION, XCP_E_NOT_INITIALIZED);
    }
    else
#endif
    {
      Xcp_CommonRxIndication(XCP_OVER_CDD, XcpRxPduId, XcpRxPduPtr);
    }
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }
}

/*------------------[Xcp_CddTxConfirmation]--------------------------------*/

FUNC(void, XCP_CODE) Xcp_CddTxConfirmation
(
  PduIdType XcpTxPduId
)
{
  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (Xcp_Initialized == FALSE)
    {
      XCP_DET_REPORT_ERROR(XCP_SID_IF_TX_CONFIRMATION, XCP_E_NOT_INITIALIZED);
    }
    else
#endif
    {
      Xcp_CommonTxConfirmation(XCP_OVER_CDD, XcpTxPduId);
    }
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }
}

#if (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON)
/*------------------[Xcp_CddTriggerTransmit]--------------------------------*/

FUNC(Std_ReturnType, XCP_CODE) Xcp_CddTriggerTransmit
(
  PduIdType XcpTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == Xcp_Initialized)
    {
      XCP_DET_REPORT_ERROR(XCP_SID_IF_TRIGGER_TRANSMIT, XCP_E_NOT_INITIALIZED);
    }
    else
#endif
    {
      RetVal = Xcp_CommonTriggerTransmit(XCP_OVER_CDD, XcpTxPduId, PduInfoPtr);
    }
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }

  return RetVal;
}

#endif /* (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON) */
#endif /* (XCP_ON_CDD_ENABLED == STD_ON) */

#if (XCP_ON_FLEXRAY_ENABLED == STD_ON)

/*------------------[Xcp_FrIfRxIndication]-----------------------------------*/


FUNC(void,XCP_CODE) Xcp_FrIfRxIndication
(
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
)
{
  DBG_XCP_FRIFRXINDICATION_ENTRY(XcpRxPduId,XcpRxPduPtr);

  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (Xcp_Initialized == FALSE)
    {
      XCP_DET_REPORT_ERROR(XCP_SID_IF_RX_INDICATION, XCP_E_NOT_INITIALIZED);
    }
    else
#endif
    {
      Xcp_CommonRxIndication(XCP_OVER_FLEXRAY, XcpRxPduId, XcpRxPduPtr);
    }
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }
  DBG_XCP_FRIFRXINDICATION_EXIT();
}

/*------------------[Xcp_FrIfTxConfirmation]---------------------------------*/

FUNC(void,XCP_CODE) Xcp_FrIfTxConfirmation
(
  PduIdType XcpTxPduId
)
{
  DBG_XCP_FRIFTXCONFIRMATION_ENTRY(XcpTxPduId);

  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (Xcp_Initialized == FALSE)
    {
      XCP_DET_REPORT_ERROR(XCP_SID_IF_TX_CONFIRMATION, XCP_E_NOT_INITIALIZED);
    }
    else
#endif
    {
      Xcp_CommonTxConfirmation(XCP_OVER_FLEXRAY, XcpTxPduId);
    }
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }
  DBG_XCP_FRIFTXCONFIRMATION_EXIT();
}

#if (XCP_TRIGGER_TX_API_ENABLED == STD_ON)
/*------------------[Xcp_FrIfTriggerTransmit]--------------------------------*/

FUNC(Std_ReturnType,XCP_CODE) Xcp_FrIfTriggerTransmit
(
  PduIdType XcpTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_XCP_FRIFTRIGGERTRANSMIT_ENTRY(XcpTxPduId);

  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == Xcp_Initialized)
    {
      XCP_DET_REPORT_ERROR(XCP_SID_IF_TRIGGER_TRANSMIT, XCP_E_NOT_INITIALIZED);
    }
    else
#endif
    {
      RetVal = Xcp_CommonTriggerTransmit(XCP_OVER_FLEXRAY, XcpTxPduId, PduInfoPtr);
    }
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }

  DBG_XCP_FRIFTRIGGERTRANSMIT_EXIT(RetVal,PduInfoPtr);

  return RetVal;
}

#endif /* (XCP_TRIGGER_TX_API_ENABLED == STD_ON) */
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) */

#if (XCP_ON_ETHERNET_ENABLED == STD_ON)

/*------------------[Xcp_SoAdIfRxIndication]---------------------------------*/


FUNC(void,XCP_CODE) Xcp_SoAdIfRxIndication
(
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
)
{
  DBG_XCP_SOADIFRXINDICATION_ENTRY(XcpRxPduId,XcpRxPduPtr);

  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (Xcp_Initialized == FALSE)
    {
      XCP_DET_REPORT_ERROR(XCP_SID_IF_RX_INDICATION, XCP_E_NOT_INITIALIZED);
    }
    else
#endif
    {
      Xcp_CommonRxIndication(XCP_OVER_ETHERNET, XcpRxPduId, XcpRxPduPtr);
    }
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }
  DBG_XCP_SOADIFRXINDICATION_EXIT();
}

/*------------------[Xcp_SoAdIfTxConfirmation]-------------------------------*/

FUNC(void,XCP_CODE) Xcp_SoAdIfTxConfirmation
(
  PduIdType XcpTxPduId
)
{
  DBG_XCP_SOADIFTXCONFIRMATION_ENTRY(XcpTxPduId);

  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (Xcp_Initialized == FALSE)
    {
      XCP_DET_REPORT_ERROR(XCP_SID_IF_TX_CONFIRMATION, XCP_E_NOT_INITIALIZED);
    }
    else
#endif
    {
      Xcp_CommonTxConfirmation(XCP_OVER_ETHERNET, XcpTxPduId);
    }
  }
  else
  {
    /* Nothing to do */
  }

  DBG_XCP_SOADIFTXCONFIRMATION_EXIT();
}

#if (XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON)
/*------------------[Xcp_CopySoAdRemoteAddress]-----------------------------------*/

FUNC(void, XCP_CODE) Xcp_CopySoAdRemoteAddress
(
  SoAd_SoConIdType SrcSoConId,
  SoAd_SoConIdType DstSoConId,
  boolean ConnectCmd
)
{

  /* Variable to hold the status for get and set Remote Address */
  boolean RemoteAddrWrongSts = FALSE;

  /* We need a conversion to TcpIp_SockAddrInetType or TcpIp_SockAddrInet6Type
  in order to gain access to the port stored at the remote address  */

  /* Variable to hold the remote address received on the Rx socket */
  TcpIp_SockAddrInet6Type RemoteAddressV6;
  /* Variable to hold the remote address received on the Rx socket */
  TcpIp_SockAddrInetType RemoteAddress;


  /* Pointer to the remote address variable, of type expected by SoAd functions */
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_CODE) SoAdRemoteAddressPtr;

  boolean IsIPv6 = XCP_IS_BYTE_FLAG_SET(Xcp_ConnectionStatus.ConnectionId,
                                     &Xcp_Connections_IPv6[Xcp_ConnectionStatus.ConnectionId / 8U]);

  DBG_XCP_COPYSOADREMOTEADDRESS_ENTRY(SrcSoConId,DstSoConId,ClearPortID);

  /* Fill in the required domain */
  if (IsIPv6 == TRUE)
  {
    /* Deviation MISRAC2012-2 <START> */
    SoAdRemoteAddressPtr = (P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_CODE))&RemoteAddressV6;
    /* Deviation MISRAC2012-2 <STOP> */
    RemoteAddressV6.domain = (TcpIp_DomainType)TCPIP_AF_INET6;
  }
  else
  {
    /* Deviation MISRAC2012-2 <START> */
    SoAdRemoteAddressPtr = (P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_CODE))&RemoteAddress;
    /* Deviation MISRAC2012-2 <STOP> */
    RemoteAddress.domain = (TcpIp_DomainType)TCPIP_AF_INET;
  }

  if (ConnectCmd)
  {
    /* Get the current address for the Rx socket */
    if (SoAd_GetRemoteAddr(SrcSoConId, SoAdRemoteAddressPtr) == E_OK)
    {
      if (SoAd_SetRemoteAddr(DstSoConId, SoAdRemoteAddressPtr) != E_OK)
      {
        RemoteAddrWrongSts = TRUE;
      }
    }
    else
    {
      RemoteAddrWrongSts = TRUE;
    }
    DstSoConId = SrcSoConId;
  }
  /* Get the current address for the Rx socket */
  if (SoAd_GetRemoteAddr(SrcSoConId, SoAdRemoteAddressPtr) == E_OK)
  {
    /* We reconfigure the socket only if the Rx socket was being reconfigured to not accept all ports */
    if ((IsIPv6 == TRUE) && (RemoteAddressV6.port != 0U))
    {
      /* set the Rx socket to 0 (wild card, i.e. accept all ports)*/
      TS_AtomicAssignGeneric(RemoteAddressV6.port, 0U);
      if (SoAd_SetRemoteAddr(DstSoConId, SoAdRemoteAddressPtr) != E_OK)
      {
        RemoteAddrWrongSts = TRUE;
      }
    }
    else if ((IsIPv6 == FALSE) && (RemoteAddress.port != 0U))
    {
      TS_AtomicAssignGeneric(RemoteAddress.port, 0U);
      if (SoAd_SetRemoteAddr(DstSoConId, SoAdRemoteAddressPtr) != E_OK)
      {
        RemoteAddrWrongSts = TRUE;
      }
    }
    else
    {
      /* Nothing to do */
    }
  }
  else
  {
    RemoteAddrWrongSts = TRUE;
  }
  if (RemoteAddrWrongSts == TRUE)
  {
    /* enter critical section*/
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    /* the socket connection is not reliable and therefore the connection will be terminated */
    Xcp_PrepareForAutonomousDisconnect(TRUE);
    /* Exit critical section.*/
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  }
  DBG_XCP_COPYSOADREMOTEADDRESS_EXIT();
}

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)

/*------------------[Xcp_SetMulticastAddress]-----------------------------------*/

FUNC(void, XCP_CODE) Xcp_SetMulticastAddress
(
  P2VAR(uint16, AUTOMATIC, XCP_APPL_DATA) PortPtr,
  P2VAR(uint32, AUTOMATIC, XCP_APPL_DATA) AddressPtr
)
{
  DBG_XCP_SETMULTICASTADDRESS_ENTRY(PortPtr,AddressPtr);

  /* Only IPv4 multi-cast addresses are supported */
  Xcp_RemoteAddrData.MulticastAddr.domain = (TcpIp_DomainType)TCPIP_AF_INET;
  Xcp_RemoteAddrData.MulticastAddr.port = *PortPtr;
  Xcp_RemoteAddrData.MulticastAddr.addr[0] = *AddressPtr;

  DBG_XCP_SETMULTICASTADDRESS_EXIT(PortPtr, AddressPtr);
}

/*------------------[Xcp_SoAdChangeTxToMulticast]-----------------------------------*/

FUNC(Std_ReturnType, XCP_CODE) Xcp_SoAdChangeTxToMulticast(void)
{
  Std_ReturnType retVal;

  /* Pointer to the multicast address variable, of type expected by SoAd functions */
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_CODE) SoAdAddressPtr;

  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_CONST) ConnectionCfgPtr =
      &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];

  DBG_XCP_SOADCHANGETXTOMULTICAST_ENTRY();

  /* Deviation MISRAC2012-2 <START> */
  SoAdAddressPtr =
      (P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_VAR_CLEARED))&Xcp_RemoteAddrData.UnicastAddr;
  /* Deviation MISRAC2012-2 <STOP> */
  Xcp_RemoteAddrData.UnicastAddr.domain = (TcpIp_DomainType)TCPIP_AF_INET;

  /* Store unicast TX address */
  retVal = SoAd_GetRemoteAddr(ConnectionCfgPtr->SoAdTxConId, SoAdAddressPtr);
  /* Set Tx socket address and port to multicast */
  if (retVal == E_OK)
  {
    /* Deviation MISRAC2012-2 <START> */
    SoAdAddressPtr =
        (P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_VAR_CLEARED))&Xcp_RemoteAddrData.MulticastAddr;
    /* Deviation MISRAC2012-2 <STOP> */
    retVal = SoAd_SetRemoteAddr(ConnectionCfgPtr->SoAdTxConId, SoAdAddressPtr);
  }

  DBG_XCP_SOADCHANGETXTOMULTICAST_EXIT(retVal);

  return retVal;
}

/*------------------[Xcp_SoAdChangeTxToUnicast]-----------------------------------*/

FUNC(Std_ReturnType, XCP_CODE) Xcp_SoAdChangeTxToUnicast(void)
{
  Std_ReturnType retVal;

  /* Pointer to the remote address variable, of type expected by SoAd functions */
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_VAR_CLEARED) SoAdAddressPtr;

  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_CONST) ConnectionCfgPtr =
      &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];

  DBG_XCP_SOADCHANGETXTOUNICAST_ENTRY();

  /* Deviation MISRAC2012-2 <START> */
  SoAdAddressPtr =
      (P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_VAR_CLEARED))&Xcp_RemoteAddrData.UnicastAddr;
  /* Deviation MISRAC2012-2 <STOP> */

  retVal = SoAd_SetRemoteAddr(ConnectionCfgPtr->SoAdTxConId, SoAdAddressPtr);

  DBG_XCP_SOADCHANGETXTOUNICAST_EXIT(retVal);

  return retVal;
}

/*------------------[Xcp_SoAdGetLocalAddress]-----------------------------------*/

FUNC(Std_ReturnType, XCP_CODE) Xcp_SoAdGetLocalAddress
(
    P2VAR(TcpIp_SockAddrInetType, AUTOMATIC, XCP_APPL_DATA) AddressPtr
)
{
  /* Return value */
  Std_ReturnType RetVal;
  /* Pointer to the local address variable, of type expected by SoAd functions */
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_CODE) SoAdLocalAddrPtr;
  /* Pointer to the default router address variable, of type expected by SoAd functions */
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_CODE) SoAdRouterAddrPtr;
  /* Network mask of IPv4 address */
  uint8 Netmask;
  /* Structure where the IPv4 address of the default router (gateway) is stored */
  TcpIp_SockAddrInetType DefaultRouterAddr;

  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_CONST) ConnectionCfgPtr =
      &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];

  DBG_XCP_SOADGETLOCALADDRESS_ENTRY(AddressPtr);

  /* Deviation MISRAC2012-2 <START> */
  SoAdLocalAddrPtr = (P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_APPL_DATA))AddressPtr;
  SoAdRouterAddrPtr = (P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_APPL_DATA))&DefaultRouterAddr;
  /* Deviation MISRAC2012-2 <STOP> */
  SoAdLocalAddrPtr->domain = (TcpIp_DomainType)TCPIP_AF_INET;

  /* Get slave local IP address */
  RetVal = SoAd_GetLocalAddr(ConnectionCfgPtr->SoAdRxConId, SoAdLocalAddrPtr,
                             &Netmask, SoAdRouterAddrPtr);

  DBG_XCP_SOADGETLOCALADDRESS_EXIT(RetVal, AddressPtr);

  return RetVal;
}

#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */

#endif /* (XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON) */

#if (XCP_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON)
/*------------------[Xcp_SoAdSoConModeChg]-----------------------------------*/

FUNC(void,XCP_CODE) Xcp_SoAdSoConModeChg
(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode
)
{
  uint8 Iterator;

  DBG_XCP_SOADSOCONMODECHG_ENTRY(SoConId,Mode);

  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    /* Check whether XCP is initialized */
    if (Xcp_Initialized == FALSE)
    {
      XCP_DET_REPORT_ERROR(XCP_SID_SOAD_SO_CON_MODE_CHG, XCP_E_NOT_INITIALIZED);
    }
    else
#endif
    {
      /* If in XCP connected state */
      if (Xcp_ConnectionStatus.State == XCP_STATE_CONNECTED)
      {
        /* search throughout configured SoAd TcpIp connections */
        for(Iterator = 0U; Iterator < XCP_NO_OF_TCPIP_SOCKET_CON; Iterator++)
        {
          /* check if the Socket connection id belongs to any of the configured
           * Xcp connections over TcpIp. */
          if(Xcp_TcpIpSocketConMapCfg[Iterator].SoAdConId == SoConId)
          {
            /* check if the selected Xcp connection is active */
            if(Xcp_TcpIpSocketConMapCfg[Iterator].IntConnectionId ==
                Xcp_ConnectionStatus.ConnectionId)
            {
              /* If XCP socket connection is not ONLINE */
              if (Mode != SOAD_SOCON_ONLINE)
              {
                /* Enter critical section*/
                SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

                /* the connection has been terminated by the remote address
                 * and therefore, the Xcp connection will be terminated */
                Xcp_PrepareForAutonomousDisconnect(TRUE);

                /* Exit critical section.*/
                SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
              }
            }
            /* the Id is found, so the processing will be stopped */
            break;
          }
        }
      }
    }
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }

  DBG_XCP_SOADSOCONMODECHG_EXIT();
}

#endif /* (XCP_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON) */
#endif /* (XCP_ON_ETHERNET_ENABLED == STD_ON) */

#if (XCP_ENABLE_XCP_CONTROL_API == STD_ON)

/*----------------------------------[Xcp_Control]----------------------------*/

FUNC(void, XCP_CODE) Xcp_Control( uint8 command )
{
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
  Atomic_value_t partition = Atomics_Load(&Xcp_PartitionOfActiveConnection);
  if (XCP_MASTER_APPLICATION == partition)
  {
    Xcp_Control_Internal(command);
  }
  else
  {
#if (XCP_REMOTE_DESTINATIONS == STD_ON)
    Xcp_RemoteDispatchType data;
    data.type = XCP_REMOTE_DISPATCH_CONTROL;
    data.p8[0] = command;
    (void) Xcp_RemoteDispatch_Handler(data);
#endif
  }
#else
  Xcp_Control_Internal(command);
#endif
}

FUNC(void, XCP_CODE) Xcp_Control_Internal( uint8 command )
{
  Xcp_ConfigType Xcp_ConfigVar;

  DBG_XCP_CONTROL_ENTRY(command);

#if (XCP_DEV_ERROR_DETECT == STD_ON)
if ( (command != XCP_STATE_ACTIVE) && (command != XCP_STATE_INACTIVE) )
  {
    /* Report Error */
    XCP_DET_REPORT_ERROR(XCP_SID_STATE_CONTROL, XCP_E_INVALID_STATE_COMMAND);
  }
  else
#endif
   {
   /* Enter critical section.*/
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* Check if command to change the state of the module is different from the actual state that the module is in*/
    if ( Xcp_ControlStateOfXcpModule != command)
    {
      /* check if command is to disable Xcp */
      if (command == XCP_STATE_INACTIVE)
      {
        /* DISABLE XCP meaning the Xcp module will be inactive, code will be generated */
        /* but the suppressed functions will perform no action.*/
        Xcp_ControlStateOfXcpModule = XCP_STATE_INACTIVE;

        /* flush command queue */
        Xcp_FlushCMDQueue();

        /* if a connection is active terminate session */

#if (XCP_NO_OF_CONNECTIONS > 1U)
        if ((Xcp_ConnectionStatus.State != XCP_STATE_DISCONNECTED) ||
            (Xcp_ConnectionStatus.ConnectionId != XCP_INVALID_CONNECTION_ID))
#else
        if (Xcp_ConnectionStatus.State != XCP_STATE_DISCONNECTED)
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */

        {
          /* Terminate session */
          Xcp_TerminateSession();
        }
        else
        {
          /* Nothing to do */
        }

        /* If DAQ/STIM resource supported */
#if (XCP_MAX_DAQ != 0U)
        /* Reset DAQList runtime information */
        Xcp_ResetEventProcessor();
#endif
      }
      else
      {
        /* ENABLE XCP => full functionality of the Xcp module is resumed */
        Xcp_ControlStateOfXcpModule = XCP_STATE_ACTIVE;
        /* Initialize XCP */
        Xcp_Init(&Xcp_ConfigVar);
      }
    }
    else
    {
      /* Nothing to do */
    }

    /* Exit critical section.*/
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
   }

  DBG_XCP_CONTROL_EXIT(command);
}
#endif /* (XCP_ENABLE_XCP_CONTROL_API == STD_ON) */


/* !LINKSTO Xcp845,1 */
#if (XCP_SUPPRESS_TX_SUPPORT == STD_ON)
/*----------------------------------[Xcp_SetTransmissionMode]----------------------------*/

/* !LINKSTO Xcp844.SetTransMode,1 */
FUNC(void, XCP_CODE)  Xcp_SetTransmissionMode
(
  NetworkHandleType Channel,
  Xcp_TransmissionModeType Mode
)
{
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
  Atomic_value_t partition = Atomics_Load(&Xcp_PartitionOfActiveConnection);
  if (XCP_MASTER_APPLICATION == partition)
  {
    Xcp_SetTransmissionMode_Internal(Channel, Mode);
  }
  else
  {
#if (XCP_REMOTE_DESTINATIONS == STD_ON)
    Xcp_RemoteDispatchType data;
    data.type = XCP_REMOTE_DISPATCH_SETTRANSMISSIONMODE;
    data.p8[0] = Channel;
    data.p8[1] = Mode;
    (void) Xcp_RemoteDispatch_Handler(data);
#endif
  }
#else
  Xcp_SetTransmissionMode_Internal(Channel, Mode);
#endif
}

FUNC(void, XCP_CODE)  Xcp_SetTransmissionMode_Internal
(
  NetworkHandleType Channel,
  Xcp_TransmissionModeType Mode
)
{
  DBG_XCP_SETTRANSMISSIONMODE_ENTRY(Channel,Mode);
#if (XCP_DEV_ERROR_DETECT == STD_ON)
  /* Check whether XCP is initialized */
  if (Xcp_Initialized == FALSE)
  {
    XCP_DET_REPORT_ERROR(XCP_SID_SET_TRANSMISSION_MODE, XCP_E_NOT_INITIALIZED);
  }
  else if (Channel > XCP_NO_OF_CONNECTIONS)
  {
    XCP_DET_REPORT_ERROR(XCP_SID_SET_TRANSMISSION_MODE, XCP_E_PARAM_CHANNEL_INVALID);
  }
  else
#endif /* (XCP_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    if (Mode == XCP_TX_OFF)
    {
      /* Disable transmission for the selected channel */
      XCP_SET_BYTE_FLAG(Channel, Xcp_TransmissionDisabled);
      /* Note: Must use flags, TS_setatomic */
    }
    else
    {
      /* Enable transmission for the selected channel */
      XCP_CLEAR_BYTE_FLAG(Channel, Xcp_TransmissionDisabled);
    }
    /* Exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  }
  DBG_XCP_SETTRANSMISSIONMODE_EXIT();
}
#endif /* XCP_SUPPRESS_TX_SUPPORT == STD_ON */

/*==================[internal function definitions]=========================*/

/*-----------------------------[Xcp_GetPduInternalRouteId]------------------------*/

STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_GetPduInternalRouteId
(
  uint8 CommunicationBusType,
  uint8 PduDirection,
  PduIdType TLPduId,
  P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) XcpInternalPduIdPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
#if (XCP_PDU_ROUTING_TABLE == STD_ON)
  PduIdType Iterator;
  P2CONST(Xcp_LPduIdRouteConfType, AUTOMATIC, AUTOMATIC) LPduIdRouteConfPtr;
  DBG_XCP_GETPDUINTERNALROUTEID_ENTRY(CommunicationBusType,PduDirection,TLPduId);
  switch(CommunicationBusType)
  {
#if((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON))
    case XCP_OVER_CAN:
      if(PduDirection == XCP_RX_PDU_DIRECTION)
      {
        for(Iterator = 0U;
            ((Iterator < XCP_NO_OF_CANIF_RX_PDU_IDS) && (RetVal == E_NOT_OK));
            Iterator++)
        {
          LPduIdRouteConfPtr = &Xcp_CanIfRxLPduIdRouteConf[Iterator];
          if(LPduIdRouteConfPtr->LowerLayerPduId == TLPduId)
          {
            *XcpInternalPduIdPtr = LPduIdRouteConfPtr->XcpInternalPduId;
            RetVal = E_OK;
          }
        }
      }
      else
      {
        for(Iterator = 0U;
            ((Iterator < XCP_NO_OF_CANIF_TX_PDU_IDS) && (RetVal == E_NOT_OK));
            Iterator++)
        {
          LPduIdRouteConfPtr = &Xcp_CanIfTxLPduIdRouteConf[Iterator];
          if(LPduIdRouteConfPtr->LowerLayerPduId == TLPduId)
          {
            *XcpInternalPduIdPtr = LPduIdRouteConfPtr->XcpInternalPduId;
            RetVal = E_OK;
          }
        }
      }
      break;
#endif /* ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON)) */
#if(XCP_ON_CDD_ENABLED == STD_ON)
    case XCP_OVER_CDD:
      if(PduDirection == XCP_RX_PDU_DIRECTION)
      {
        for(Iterator = 0U;
            ((Iterator < XCP_NO_OF_CDD_RX_PDU_IDS) && (RetVal == E_NOT_OK));
            Iterator++)
        {
          LPduIdRouteConfPtr = &Xcp_CddRxLPduIdRouteConf[Iterator];
          if(LPduIdRouteConfPtr->LowerLayerPduId == TLPduId)
          {
            *XcpInternalPduIdPtr = LPduIdRouteConfPtr->XcpInternalPduId;
            RetVal = E_OK;
          }
        }
      }
      else
      {
        for(Iterator = 0U;
            ((Iterator < XCP_NO_OF_CDD_TX_PDU_IDS) && (RetVal == E_NOT_OK));
            Iterator++)
        {
          LPduIdRouteConfPtr = &Xcp_CddTxLPduIdRouteConf[Iterator];
          if(LPduIdRouteConfPtr->LowerLayerPduId == TLPduId)
          {
            *XcpInternalPduIdPtr = LPduIdRouteConfPtr->XcpInternalPduId;
            RetVal = E_OK;
          }
        }
      }
      break;
#endif /* (XCP_ON_CDD_ENABLED == STD_ON) */
#if(XCP_ON_FLEXRAY_ENABLED == STD_ON)
    case XCP_OVER_FLEXRAY:
      if(PduDirection == XCP_RX_PDU_DIRECTION)
      {
        for(Iterator = 0U;
            ((Iterator < XCP_NO_OF_FRIF_RX_PDU_IDS) && (RetVal == E_NOT_OK));
            Iterator++)
        {
          LPduIdRouteConfPtr = &Xcp_FrIfRxLPduIdRouteConf[Iterator];
          if(LPduIdRouteConfPtr->LowerLayerPduId == TLPduId)
          {
            *XcpInternalPduIdPtr = LPduIdRouteConfPtr->XcpInternalPduId;
            RetVal = E_OK;
          }
        }
      }
      else
      {
        for(Iterator = 0U;
            ((Iterator < XCP_NO_OF_FRIF_TX_PDU_IDS) && (RetVal == E_NOT_OK));
            Iterator++)
        {
          LPduIdRouteConfPtr = &Xcp_FrIfTxLPduIdRouteConf[Iterator];
          if(LPduIdRouteConfPtr->LowerLayerPduId == TLPduId)
          {
            *XcpInternalPduIdPtr = LPduIdRouteConfPtr->XcpInternalPduId;
            RetVal = E_OK;
          }
        }
      }
      break;
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) */
#if(XCP_ON_ETHERNET_ENABLED == STD_ON)
    case XCP_OVER_ETHERNET:
      if(PduDirection == XCP_RX_PDU_DIRECTION)
      {
        for(Iterator = 0U;
            ((Iterator < XCP_NO_OF_SOAD_RX_PDU_IDS) && (RetVal == E_NOT_OK));
            Iterator++)
        {
          LPduIdRouteConfPtr = &Xcp_SoAdRxLPduIdRouteConf[Iterator];
          if(LPduIdRouteConfPtr->LowerLayerPduId == TLPduId)
          {
            *XcpInternalPduIdPtr = LPduIdRouteConfPtr->XcpInternalPduId;
            RetVal = E_OK;
          }
        }
      }
      else
      {
        for(Iterator = 0U;
            ((Iterator < XCP_NO_OF_SOAD_TX_PDU_IDS) && (RetVal == E_NOT_OK));
            Iterator++)
        {
          LPduIdRouteConfPtr = &Xcp_SoAdTxLPduIdRouteConf[Iterator];
          if(LPduIdRouteConfPtr->LowerLayerPduId == TLPduId)
          {
            *XcpInternalPduIdPtr = LPduIdRouteConfPtr->XcpInternalPduId;
            RetVal = E_OK;
          }
        }
      }
      break;
#endif /* (XCP_ON_ETHERNET_ENABLED == STD_ON) */
    /* CHECK: NOPARSE */
    default:
      /* all possible cases have been explicitly covered in the cases above, the 'default'
         cannot be reached for the current implementation. The only possible option should
         be the CDD, but this is not supported for the current implementation yet */
      XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
      break;
    /* CHECK: PARSE */
  }
#else
  DBG_XCP_GETPDUINTERNALROUTEID_ENTRY(CommunicationBusType,PduDirection,TLPduId);
  if(PduDirection == XCP_RX_PDU_DIRECTION)
  {
    if (TLPduId < XCP_NO_OF_RX_PDU_IDS)
    {
      *XcpInternalPduIdPtr = TLPduId;
      RetVal = E_OK;
    }
  }
  else
  {
    if (TLPduId < XCP_NO_OF_TX_PDU_IDS)
    {
      *XcpInternalPduIdPtr = TLPduId;
      RetVal = E_OK;
    }
  }
  TS_PARAM_UNUSED(CommunicationBusType);
#endif /* (XCP_PDU_ROUTING_TABLE = STD_ON) */

  DBG_XCP_GETPDUINTERNALROUTEID_EXIT(RetVal,XcpInternalPduIdPtr);

  return RetVal;
}

/*-----------------------------[Xcp_CommonRxIndication]------------------------*/


STATIC FUNC(void,XCP_CODE) Xcp_CommonRxIndication
(
  uint8 CommunicationBusType,
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
)
{
#if(XCP_ON_FLEXRAY_ENABLED == STD_ON)
  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr;
  boolean IsPackageAccepted = TRUE;
#endif

  PduIdType XcpInternalPduId = XCP_IGNORED_PDU_ID;
#if (XCP_DEV_ERROR_DETECT == STD_ON)
  Std_ReturnType XcpInternalRouteSts;
#endif

  DBG_XCP_COMMONRXINDICATION_ENTRY(CommunicationBusType,XcpRxPduId,XcpRxPduPtr);

#if (XCP_DEV_ERROR_DETECT == STD_ON)
  XcpInternalRouteSts =
#else
  (void)
#endif
  Xcp_GetPduInternalRouteId(CommunicationBusType, XCP_RX_PDU_DIRECTION, XcpRxPduId, &XcpInternalPduId);

#if (XCP_DEV_ERROR_DETECT == STD_ON)
  if (XcpInternalRouteSts!= E_OK)
  {
    XCP_DET_REPORT_ERROR(XCP_SID_IF_RX_INDICATION, XCP_E_INVALID_PDUID);
  }
  else if ((XcpRxPduPtr == NULL_PTR)|| (XcpRxPduPtr->SduDataPtr == NULL_PTR))
  {
    XCP_DET_REPORT_ERROR(XCP_SID_IF_RX_INDICATION, XCP_E_NULL_POINTER);
  }
  else if (XcpRxPduPtr->SduLength > Xcp_GetPduLength(XcpInternalPduId))
  {
    XCP_DET_REPORT_ERROR(XCP_SID_IF_RX_INDICATION, XCP_E_INVALID_RX_PDU_LENGTH);
  }
  else
#endif/* (XCP_DEV_ERROR_DETECT == STD_ON) */
  {
    /* check if the package can be accepted */
#if(XCP_NO_OF_CONNECTIONS > 1U)
    if((Xcp_ConnectionStatus.ConnectionId !=
        Xcp_RxLPduIdConf[XcpInternalPduId].IntConnectionId) &&
      ((Xcp_ConnectionStatus.State == XCP_STATE_CONNECTED) ||
       (Xcp_ConnectionStatus.ConnectionId != XCP_INVALID_CONNECTION_ID)))
    {
#if(XCP_PROD_ERR_HANDLING_PDU_BUFFER_LOCKED!= TS_PROD_ERR_DISABLE)
      /* the connection resources are locked by another connection,
       * so, ignore notification */
      XCP_REPORT_PDU_BUFFER_LOCKED(XCP_SID_IF_RX_INDICATION);
#endif
    }
    else
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
    {
#if(XCP_ON_FLEXRAY_ENABLED == STD_ON)
      ConnectionCfgPtr = &(Xcp_ConnectionCfg[Xcp_RxLPduIdConf[XcpInternalPduId].IntConnectionId]);
#if(XCP_NO_OF_CONNECTIONS > 1U)
      if(ConnectionCfgPtr->ConnectionType == XCP_CONNECTION_OVER_FLEXRAY)
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
      {
        if(XcpRxPduPtr->SduDataPtr[0U] != ConnectionCfgPtr->FlexRayNax)
        {
          IsPackageAccepted = FALSE;
        }
      }

      if(IsPackageAccepted == TRUE)
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) */
      {
        /* process Rx Pdu data */
        Xcp_ReceiveRxData(XcpInternalPduId, XcpRxPduPtr);
      }
    }
  }
  DBG_XCP_COMMONRXINDICATION_EXIT();
}

#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))
/*-----------------------------[Xcp_CommonTriggerTransmit]------------------------*/

STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_CommonTriggerTransmit
(
  uint8 CommunicationBusType,
  PduIdType XcpTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  PduIdType XcpInternalPduId = XCP_IGNORED_PDU_ID;
#if (XCP_DEV_ERROR_DETECT == STD_ON)
  Std_ReturnType XcpInternalRouteSts;
#endif

  DBG_XCP_COMMONTRIGGERTRANSMIT_ENTRY(CommunicationBusType, XcpTxPduId);

#if (XCP_DEV_ERROR_DETECT == STD_ON)
  XcpInternalRouteSts =
#else
  (void)
#endif
  Xcp_GetPduInternalRouteId(CommunicationBusType, XCP_TX_PDU_DIRECTION, XcpTxPduId, &XcpInternalPduId);

#if (XCP_DEV_ERROR_DETECT == STD_ON)
  if (XcpInternalRouteSts!= E_OK)
  {
    XCP_DET_REPORT_ERROR(XCP_SID_IF_TRIGGER_TRANSMIT, XCP_E_INVALID_PDUID);
  }
  else if ((PduInfoPtr == NULL_PTR) || (PduInfoPtr->SduDataPtr == NULL_PTR))
  {
    XCP_DET_REPORT_ERROR(XCP_SID_IF_TRIGGER_TRANSMIT, XCP_E_INV_POINTER);
  }
  else
#endif
  {
#if(XCP_NO_OF_CONNECTIONS > 1U)
    if(Xcp_ConnectionStatus.ConnectionId !=
       Xcp_TxLPduIdConf[XcpInternalPduId].IntConnectionId)
    {
      /* the connection resources are locked by another connection,
       * so, ignore request and return error code. */
#if(XCP_PROD_ERR_HANDLING_PDU_BUFFER_LOCKED!= TS_PROD_ERR_DISABLE)
      XCP_REPORT_PDU_BUFFER_LOCKED(XCP_SID_IF_TRIGGER_TRANSMIT);
#endif
    }
    else
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
    {
      /* Get data to be transmitted */
      RetVal = Xcp_ProvideTxData(XcpInternalPduId, PduInfoPtr);
    }
  }

  DBG_XCP_COMMONTRIGGERTRANSMIT_EXIT(RetVal,PduInfoPtr);

  return RetVal;
}

#endif /* ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON)) */

/*-----------------------------[Xcp_CommonTxConfirmation]------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_CommonTxConfirmation
(
  uint8 CommunicationBusType,
  PduIdType XcpTxPduId
)
{
#if (XCP_DEV_ERROR_DETECT == STD_ON)
  Std_ReturnType XcpInternalRouteSts;
#endif
  PduIdType XcpInternalPduId = XCP_IGNORED_PDU_ID;

  DBG_XCP_COMMONTXCONFIRMATION_ENTRY(CommunicationBusType, XcpTxPduId);

#if (XCP_DEV_ERROR_DETECT == STD_ON)
  XcpInternalRouteSts =
#else
  (void)
#endif
  Xcp_GetPduInternalRouteId(CommunicationBusType, XCP_TX_PDU_DIRECTION, XcpTxPduId, &XcpInternalPduId);

#if (XCP_DEV_ERROR_DETECT == STD_ON)
  if (XcpInternalRouteSts != E_OK)
  {
    XCP_DET_REPORT_ERROR(XCP_SID_IF_TX_CONFIRMATION, XCP_E_INVALID_PDUID);
  }
  else
#endif
  {
#if(XCP_NO_OF_CONNECTIONS > 1U)
    if(Xcp_ConnectionStatus.ConnectionId != Xcp_TxLPduIdConf[XcpInternalPduId].IntConnectionId)
    {
      /* the connection resources are locked by another connection,
       * so, ignore notification */
#if(XCP_PROD_ERR_HANDLING_PDU_BUFFER_LOCKED!= TS_PROD_ERR_DISABLE)
      XCP_REPORT_PDU_BUFFER_LOCKED(XCP_SID_IF_TX_CONFIRMATION);
#endif
    }
    else
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
    {
      Xcp_IndicateTxConfirmation(XcpInternalPduId);
    }
  }

  DBG_XCP_COMMONTXCONFIRMATION_EXIT();
}

#if (XCP_MAX_DAQ != 0U)
/*-----------------------------[Xcp_InitializeDaqLists]------------------------*/

STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_InitializeDaqLists(void)
{
  /* The return value of this function*/
  Std_ReturnType RetValue = E_OK;
#if (XCP_STORE_DAQ_SUPPORTED == STD_ON)
  /* Variable to indicate if the DAQ lists were initialized from NV memory */
  boolean DaqListsInitializedFromNV = FALSE;
  /* The result of the NvM_ReadAll() service for the XCP NVM block descriptor */
  NvM_RequestResultType BlockResult;
#if (XCP_DEV_ERROR_DETECT == STD_ON)
  /* Variable to store the size of the DAQ lists runtime information */
  uint32 SizeOfDaqLists = (uint32)sizeof(Xcp_DaqLists);
#if ((XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON) && (XCP_NO_OF_NON_DYNAMIC_ODTENTRIES > 0))
  SizeOfDaqLists += sizeof(Xcp_OdtAddressTableRAM) + sizeof(Xcp_OdtLengthTableRAM);
#if (XCP_ENABLE_BIT_OFFSET == STD_ON)
  SizeOfDaqLists += sizeof(Xcp_OdtBitOffsetTableRAM);
#endif
#endif
#endif /* (XCP_DEV_ERROR_DETECT == STD_ON) */
#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) */

  DBG_XCP_INITIALIZEDAQLISTS_ENTRY();

#if (XCP_STORE_DAQ_SUPPORTED == STD_ON)
#if (XCP_DEV_ERROR_DETECT == STD_ON)

  /* It has to assured that the configured size for NvM block is not smaller than */
  /* the size of the data in the XCP structure. This can include fill bytes. */
  if (SizeOfDaqLists > XCP_STORE_DAQ_NVM_BLOCK_LENGTH)
  {
    /* Throw error */
    XCP_DET_REPORT_ERROR(XCP_SID_INIT, XCP_E_INVALID_NVM_BLOCK_LENGTH);
    RetValue = E_NOT_OK;
  }
  else
#endif /* (XCP_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Read the error status */
    if (NvM_GetErrorStatus(XCP_STORE_DAQ_NVM_BLOCK_ID, &BlockResult) == E_OK)
    {
      /* We consider the DAQ lists initialized only if NvM read successfully the DAQ lists from the NV
       * memory and the start up mode is STORE_DAQ (with or without resume mode). In case of CLEAR_DAQ,
       * we let the initialization routine to initialize the DAQ lists with default values. */
      if ((BlockResult == NVM_REQ_OK) &&
         ((Xcp_DaqLists.Xcp_StoreDaq.Mode == XCP_MASK_STORE_DAQ_REQ_RESUME) ||
          (Xcp_DaqLists.Xcp_StoreDaq.Mode == XCP_MASK_STORE_DAQ_REQ_NORESUME)))
      {
        /* If the desired start mode is resume we enable the immediate transfer start */
        if (Xcp_DaqLists.Xcp_StoreDaq.Mode == XCP_MASK_STORE_DAQ_REQ_RESUME)
        {
          Xcp_PrepareStoredDaqListsResume();
        }
        else
        {
          /* If the desired start mode is not resume, just prepare the DAQ lists accordingly */
          Xcp_PrepareStoredDaqLists();
        }

        /* mark as initialized */
        DaqListsInitializedFromNV = TRUE;
      }
    }
  }

  /* If the DAQ lists could not be loaded from the NV memory or the DAQ lists were cleared from NV,
   * or the storage of DAQ lists is not supported we need to restore/initialize the DAQ lists with
   * default data */
  if (DaqListsInitializedFromNV == FALSE)
#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) */
  {
#if (XCP_STORE_DAQ_SUPPORTED == STD_ON)
    /* Iterator */
    uint16 i;
    /* Clear the store DAQ administrative structure as it might have been erroneously filled by the
     * NvM stack */
    Xcp_DaqLists.Xcp_StoreDaq.ConfigurationSessionID = 0U;
#if(XCP_NO_OF_CONNECTIONS > 1U)
    /* Clear ConnectionId */
    Xcp_DaqLists.Xcp_StoreDaq.ConnectionId = XCP_INVALID_CONNECTION_ID;
#endif
#if (XCP_DAQ_COUNT > 0U)
    Xcp_DaqLists.Xcp_StoreDaq.NoOfConfiguredDynamicDaqLists = 0U;
#endif /* (XCP_DAQ_COUNT > 0U) */
    Xcp_DaqLists.Xcp_StoreDaq.Mode = 0U;
    for (i = 0U; i< XCP_NUMBER_OF_BYTES_SELECTED_DAQ; i++)
    {
      Xcp_DaqLists.Xcp_StoreDaq.SelectedDAQ[i] = 0U;
    }
#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) */

#if ((XCP_MAX_DAQ != 0U) && (XCP_MAX_DAQ > XCP_DAQ_COUNT)) /* If we have static or predefined DAQ lists */
    /* Initialize the DAQ lists with data from ROM */
    TS_MemCpy(&(Xcp_DaqLists.Xcp_NonDynamicDaqLists),&Xcp_DaqListsDefault, (sizeof(Xcp_DaqListsDefault)));
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON)
    TS_MemCpy(&Xcp_OdtAddressTableRAM[0], &Xcp_OdtAddressTableROM[0], sizeof(Xcp_OdtAddressTableRAM));
    TS_MemCpy(&Xcp_OdtLengthTableRAM[0], &Xcp_OdtLengthTableROM[0], sizeof(Xcp_OdtLengthTableRAM));
#if (XCP_ENABLE_BIT_OFFSET == STD_ON)
    TS_MemCpy(&Xcp_OdtBitOffsetTableRAM[0], &Xcp_OdtBitOffsetTableROM[0], sizeof(Xcp_OdtBitOffsetTableRAM));
#endif
#endif
#endif /* ((XCP_MAX_DAQ != 0U) && (XCP_MAX_DAQ > XCP_DAQ_COUNT)) */

#if (XCP_DAQ_COUNT > 0U) /* If we have dynamic DAQ lists */
    /* Reset the entire dynamic memory area to 0 */
    TS_MemBZero(Xcp_DaqLists.Xcp_DynamicArea, XCP_DYNAMIC_AREA_SIZE);
#endif /* (XCP_DAQ_COUNT > 0U) */
  }

  DBG_XCP_INITIALIZEDAQLISTS_EXIT(RetValue);
  return RetValue;
}
#endif /* (XCP_MAX_DAQ != 0U) */

#if (XCP_SOAD_AUTO_OPEN_SO_CON == STD_ON)
/*-----------------------------[Xcp_InitSoAdConnetions]------------------------*/

STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_InitSoAdConnetions(void)
{
  Std_ReturnType RetVal = E_OK;
  uint8 Iterator;
  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr;

  for(Iterator = 0U; Iterator < XCP_NO_OF_CONNECTIONS; Iterator++)
  {
    /* select connection */
    ConnectionCfgPtr = &Xcp_ConnectionCfg[Iterator];

    if (XCP_IS_FLAG_SET(XCP_AUTO_OPEN_SOCON_FLAG,
                        ConnectionCfgPtr->Properties,
                        uint8) == TRUE)
    {
      /* Open SoAd socket connection for XCP */
      if (SoAd_OpenSoCon(ConnectionCfgPtr->SoAdTxConId) != E_OK)
      {
        /* the socket connection cannot be open. */
        RetVal = E_NOT_OK;
        break;
      }
#if (XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON)
      else
      {
#if (XCP_NO_OF_CONNECTIONS > 1U)
        if(ConnectionCfgPtr->ConnectionType == XCP_CONNECTION_OVER_UDPIP)
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
        {
          /* Open a second SoAd socket connection for XCP */
          if (SoAd_OpenSoCon(ConnectionCfgPtr->SoAdRxConId) != E_OK)
          {

            /* the socket connection cannot be open. */
            RetVal = E_NOT_OK;
            /* Deviation MISRAC2012-1  */
            break;
          }
        }
      }
#endif /* (XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON) */
    }
  }
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
  /* Initialize Xcp_RemoteAddrData: domain: IPv4, port:0, address  = 0 */
  Xcp_RemoteAddrData.MulticastAddr.domain = (TcpIp_DomainType)TCPIP_AF_INET;
  Xcp_RemoteAddrData.MulticastAddr.port = 0U;
  Xcp_RemoteAddrData.MulticastAddr.addr[0] = 0U;
  Xcp_RemoteAddrData.UnicastAddr.domain = (TcpIp_DomainType)TCPIP_AF_INET;
  Xcp_RemoteAddrData.UnicastAddr.port = 0U;
  Xcp_RemoteAddrData.UnicastAddr.addr[0] = 0U;
#endif /* XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON */

  return RetVal;
}
#endif

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>
#endif /* (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) */

/*==================[end of file]===========================================*/
