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

/* !LINKSTO Dcm.Dsn.File.DslCommunicationServices.PublicApi,1 */
/* This file contains the Public APIs needed for the communication modules as well as data types
   and macro definitions used by these units. */

#ifndef DCM_DSL_COMMUNICATIONSERVICES_H
#define DCM_DSL_COMMUNICATIONSERVICES_H

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 8.9 (required)
 *     An identifier with external linkage shall have exactly one external definition.
 *
 *     Reason:
 *     The definition of the declared external variable is, for some unit tests, in the test source
 *     code as a dummy.
 *     The test source code is not included in the MISRA analysis and therefore it is
 *     not visible to the MISRA checker.
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 8.6 (required)
  *     "An identifier with external linkage shall have exactly one external definition."
  *
  *     Reason:
  *     The definition of the declared external variable is, for some unit tests, in the test source
  *     code as a dummy.
  *     The test source code is not included in the MISRA analysis and therefore it is
  *     not visible to the MISRA checker.
  */

/*==================[inclusions]=================================================================*/

#include <Dcm.h>
#include <Dcm_Cfg.h>                                     /* Dcm configuration header file */

#include <ComStack_Types.h>                              /* AUTOSAR standard types */
#include <Dcm_Dsl_Cfg.h>                                 /* Dcm Dsl Configuration  */

#include <Dcm_Dsl_ComMChannelStateManager.h>             /* Dcm Dsl CComMChannelStateManager header file */
#include <Dcm_Dsl_CommunicationServices_BufferManager.h> /* Dcm Dsl BufferManager header file */

#if (DCM_DEV_ERROR_DETECT == STD_ON)
/* Development Error Tracer (DCM_DET_REPORT_ERROR() macros) */
#include <Det.h>
#endif

/*==================[macros]=====================================================================*/

#if (defined DCM_PROTOCOL_ID_LOW)
  #error "DCM_PROTOCOL_ID_LOW is already defined"
#endif
#define DCM_PROTOCOL_ID_LOW (Dcm_BufferUserType)((DCM_TX_CONNECTION_ID_HIGH) + 1U)

#if (defined DCM_PROTOCOL_ID_HIGH)
  #error "DCM_PROTOCOL_ID_HIGH is already defined"
#endif
#define DCM_PROTOCOL_ID_HIGH \
  (Dcm_BufferUserType)((DCM_PROTOCOL_ID_LOW) + (Dcm_BufferUserType)(DCM_NUM_PROTOCOLS) - 1U)

#if (defined DCM_SUPERVISOR_ID)
  #error "DCM_SUPERVISOR_ID is already defined"
#endif
#define DCM_SUPERVISOR_ID (Dcm_BufferUserType)((DCM_PROTOCOL_ID_HIGH) + 1U)

#if (defined DCM_INVALID_USER_ID)
  #error "DCM_INVALID_USER_ID is already defined"
#endif
#define DCM_INVALID_USER_ID (Dcm_BufferUserType)((DCM_SUPERVISOR_ID) + 1U)

#if (defined DCM_PROTOCOL_ID_FROM_INDEX)
  #error "DCM_PROTOCOL_ID_FROM_INDEX is already defined"
#endif
#define DCM_PROTOCOL_ID_FROM_INDEX(ProtocolIndex) \
  (Dcm_BufferUserType)((Dcm_BufferUserType)(ProtocolIndex) + \
  (Dcm_BufferUserType)(DCM_PROTOCOL_ID_LOW))

#if (defined DCM_PROTOCOL_INDEX_FROM_ID)
  #error "DCM_PROTOCOL_INDEX_FROM_ID is already defined"
#endif
#define DCM_PROTOCOL_INDEX_FROM_ID(ProtocolId) \
  (Dcm_ProtocolIndexType)((ProtocolId) - (DCM_PROTOCOL_ID_LOW))

#if (defined DCM_INVALID_PROTOCOL_INDEX)
  #error "DCM_INVALID_PROTOCOL_INDEX is already defined"
#endif
#define DCM_INVALID_PROTOCOL_INDEX DCM_NUM_PROTOCOLS

#if (defined DCM_INVALID_MAINCONNECTION_INDEX)
  #error "DCM_INVALID_MAINCONNECTION_INDEX is already defined"
#endif
#define DCM_INVALID_MAINCONNECTION_INDEX DCM_NUM_MAIN_CONNECTIONS

#if (defined DCM_USER_IS_RX_CONNECTION)
  #error "DCM_USER_IS_RX_CONNECTION is already defined"
#endif
#define DCM_USER_IS_RX_CONNECTION(UserId) ((UserId) <= (DCM_RX_CONNECTION_ID_HIGH))

#if (defined DCM_USER_IS_TX_CONNECTION)
  #error "DCM_USER_IS_TX_CONNECTION is already defined"
#endif
#define DCM_USER_IS_TX_CONNECTION(UserId) \
  ((DCM_TX_CONNECTION_ID_LOW <= (UserId)) && ((UserId) <= (DCM_TX_CONNECTION_ID_HIGH)))

#if (defined DCM_USER_IS_PROTOCOL)
  #error "DCM_USER_IS_PROTOCOL is already defined"
#endif
#define DCM_USER_IS_PROTOCOL(UserId) \
  ((DCM_PROTOCOL_ID_LOW <= (UserId)) && ((UserId) <= (DCM_PROTOCOL_ID_HIGH)))

/* whether this request goes to a protocol or bypasses the
   Dsd/Dsp*/
#if (defined DCM_REQUEST_IS_DISPATCHABLE)
  #error "DCM_REQUEST_IS_DISPATCHABLE is already defined"
#endif
#define DCM_REQUEST_IS_DISPATCHABLE(RequestType) \
  (!((DCM_BUSYREPEATREQUEST_REQUEST == (RequestType)) || \
  (DCM_CONCURRENT_TESTERPRESENT_REQUEST == (RequestType)) || \
  (DCM_INVALID_REQUEST_TYPE == (RequestType))))

#if (defined DCM_IS_PHYSICAL_REQUEST)
  #error "DCM_IS_PHYSICAL_REQUEST is already defined"
#endif
#define DCM_IS_PHYSICAL_REQUEST(RequestType) \
  ( (DCM_NORMAL_PHYSICAL_REQUEST == (RequestType)) || \
    (DCM_ROE_TYPE1_PHYSICAL_REQUEST == (RequestType)) || \
    (DCM_ROE_TYPE2_PHYSICAL_REQUEST == (RequestType)) )

#if (defined DCM_IS_FUNCTIONAL_REQUEST)
  #error "DCM_IS_FUNCTIONAL_REQUEST is already defined"
#endif
#define DCM_IS_FUNCTIONAL_REQUEST(RequestType) \
  ( (DCM_NORMAL_FUNCTIONAL_REQUEST == (RequestType)) || \
    (DCM_ROE_TYPE1_FUNCTIONAL_REQUEST == (RequestType)) || \
    (DCM_ROE_TYPE2_FUNCTIONAL_REQUEST == (RequestType)) )

#if (defined DCM_IS_NORMAL_REQUEST)
  #error "DCM_IS_NORMAL_REQUEST is already defined"
#endif
#define DCM_IS_NORMAL_REQUEST(RequestType) ((DCM_NORMAL_PHYSICAL_REQUEST == (RequestType)) || \
                                            (DCM_NORMAL_FUNCTIONAL_REQUEST == (RequestType)))

#if (defined DCM_IS_ROE_REQUEST)
  #error "DCM_IS_ROE_REQUEST is already defined"
#endif
#define DCM_IS_ROE_REQUEST(RequestType) ((DCM_ROE_TYPE1_PHYSICAL_REQUEST == (RequestType)) || \
                                         (DCM_ROE_TYPE1_FUNCTIONAL_REQUEST == (RequestType)) || \
                                         (DCM_ROE_TYPE2_PHYSICAL_REQUEST == (RequestType)) || \
                                         (DCM_ROE_TYPE2_FUNCTIONAL_REQUEST == (RequestType)))

#if (defined DCM_IS_PERIODIC_REQUEST)
  #error "DCM_IS_PERIODIC_REQUEST is already defined"
#endif
#define DCM_IS_PERIODIC_REQUEST(RequestType) ((DCM_PERIODIC_REQUEST == (RequestType)) || \
                                              (DCM_PERIODIC_TYPE1_REQUEST == (RequestType)) || \
                                              (DCM_PERIODIC_TYPE2_REQUEST == (RequestType)))

#if (defined DCM_IS_JUMP_FROM_BL_RESPONSE_REQUEST)
  #error "DCM_IS_JUMP_FROM_BL_RESPONSE_REQUEST is already defined"
#endif
#define DCM_IS_JUMP_FROM_BL_RESPONSE_REQUEST(RequestType) \
  (DCM_JUMP_FROM_BL_RESPONSE_REQUEST == (RequestType))

#if (defined DCM_IS_JUMP_FROM_BL_NO_RESPONSE_REQUEST)
  #error "DCM_IS_JUMP_FROM_BL_NO_RESPONSE_REQUEST is already defined"
#endif
#define DCM_IS_JUMP_FROM_BL_NO_RESPONSE_REQUEST(RequestType) \
  (DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST == (RequestType))

#if (defined DCM_INVALID_PERIODIC_CONNECTION)
  #error "DCM_INVALID_PERIODIC_CONNECTION is already defined"
#endif
#define DCM_INVALID_PERIODIC_CONNECTION DCM_NUM_PERIODIC_CONNECTIONS

#if (defined DCM_INVALID_PERIODIC_TRANSMISSION)
  #error "DCM_INVALID_PERIODIC_TRANSMISSION is already defined"
#endif
#define DCM_INVALID_PERIODIC_TRANSMISSION DCM_NUM_PERIODIC_TRANSMISSIONS

#if (defined DCM_INVALID_ROE_CONNECTION)
  #error "DCM_INVALID_ROE_CONNECTION is already defined"
#endif
#define DCM_INVALID_ROE_CONNECTION DCM_NUM_ROE_CONNECTIONS

/** \brief Physical addressing. */
#if (defined DCM_PHYSICAL_ADDRESSING)
  #error "DCM_PHYSICAL_ADDRESSING is already defined"
#endif
#define DCM_PHYSICAL_ADDRESSING   0U

/** \brief Functional addressing. */
#if (defined DCM_FUNCTIONAL_ADDRESSING)
  #error "DCM_FUNCTIONAL_ADDRESSING is already defined"
#endif
#define DCM_FUNCTIONAL_ADDRESSING 1U
/** \brief This is the default, invalid origin of a request. */
#if (defined DCM_INVALID_ORIGIN)
  #error "DCM_INVALID_ORIGIN is already defined"
#endif
#define DCM_INVALID_ORIGIN 0U

/** \brief This is an externally originating request (coming from the lower layer). */
#if (defined DCM_EXTERNAL)
  #error "DCM_EXTERNAL is already defined"
#endif
#define DCM_EXTERNAL 1U

/** \brief This is an internally originating request coming from the ROE subsystem. */
#if (defined DCM_INTERNAL_ROE)
  #error "DCM_INTERNAL_ROE is already defined"
#endif
#define DCM_INTERNAL_ROE 2U

/** \brief This is an internally originating request coming from the PeriodicResponse subsystem. */
#if (defined DCM_INTERNAL_PERIODIC)
  #error "DCM_INTERNAL_PERIODIC is already defined"
#endif
#define DCM_INTERNAL_PERIODIC 3U

/** \brief This is an internally originating request coming from the PostBoot subsystem. */
#if (defined DCM_INTERNAL_POST_BOOT_NO_ANSWER)
  #error "DCM_INTERNAL_POST_BOOT_NO_ANSWER is already defined"
#endif
#define DCM_INTERNAL_POST_BOOT_NO_ANSWER 4U

/** \brief This is an internally originating request coming from the PostBoot subsystem. */
#if (defined DCM_INTERNAL_POST_BOOT_WITH_ANSWER)
  #error "DCM_INTERNAL_POST_BOOT_WITH_ANSWER is already defined"
#endif
#define DCM_INTERNAL_POST_BOOT_WITH_ANSWER 5U

/** \brief This type describes the current state of a protocol (whether it is
 *         active or not and whether it is currently processing data).
 */

/** \brief The Protocol is stopped (not the currently active protocol). */
#if (defined DCM_PROTOCOL_STOPPED)
  #error "DCM_PROTOCOL_STOPPED is already defined"
#endif
#define DCM_PROTOCOL_STOPPED 0U


/** \brief The Protocol is the currently active protocol and is processing something
 *  currently. */
#if (defined DCM_PROTOCOL_STARTED_RUNNING)
  #error "DCM_PROTOCOL_STARTED_RUNNING is already defined"
#endif
#define DCM_PROTOCOL_STARTED_RUNNING 2U

/* GENERATED SECTION BEGIN EA:{5F83F0AA-FFBD-4be4-BFC6-B50A1F694EDE} */

/* This type expresses the type of a request which is forwarded to the Supervisor. */

/** \brief Dcm_RequestType: Invalid request type. **/
#if (defined DCM_INVALID_REQUEST_TYPE)
  #error "DCM_INVALID_REQUEST_TYPE is already defined"
#endif
#define DCM_INVALID_REQUEST_TYPE ((uint8)0U)

/** \brief Dcm_RequestType: The request is an external request which arrived on a physical
 *  RxPduId. **/
#if (defined DCM_NORMAL_PHYSICAL_REQUEST)
  #error "DCM_NORMAL_PHYSICAL_REQUEST is already defined"
#endif
#define DCM_NORMAL_PHYSICAL_REQUEST ((uint8)1U)

/** \brief Dcm_RequestType: The request is an external request which arrived on a functional
 *  RxPduId. **/
#if (defined DCM_NORMAL_FUNCTIONAL_REQUEST)
  #error "DCM_NORMAL_FUNCTIONAL_REQUEST is already defined"
#endif
#define DCM_NORMAL_FUNCTIONAL_REQUEST ((uint8)2U)

/** \brief Dcm_RequestType: The request is an internally-generated Periodic response request. **/
#if (defined DCM_PERIODIC_REQUEST)
  #error "DCM_PERIODIC_REQUEST is already defined"
#endif
#define DCM_PERIODIC_REQUEST ((uint8)5U)

/** \brief Dcm_RequestType: The request is an external request which shall be responded to with
 *  a BusyRepeatRequest (0x21) NRC. **/
#if (defined DCM_BUSYREPEATREQUEST_REQUEST)
  #error "DCM_BUSYREPEATREQUEST_REQUEST is already defined"
#endif
#define DCM_BUSYREPEATREQUEST_REQUEST ((uint8)6U)

/** \brief Dcm_RequestType: The request is an external request which shall only be treated if
 *  it is a concurrent TesterPresent (0x3E 0x80) request. **/
#if (defined DCM_CONCURRENT_TESTERPRESENT_REQUEST)
  #error "DCM_CONCURRENT_TESTERPRESENT_REQUEST is already defined"
#endif
#define DCM_CONCURRENT_TESTERPRESENT_REQUEST ((uint8)7U)

/** \brief Dcm_RequestType: The request is an internally generated request to perform state
 *  variable settings and respond to a previous external request which resulted in a jump to
 *  the bootloader. **/
#if (defined DCM_JUMP_FROM_BL_RESPONSE_REQUEST)
  #error "DCM_JUMP_FROM_BL_RESPONSE_REQUEST is already defined"
#endif
#define DCM_JUMP_FROM_BL_RESPONSE_REQUEST ((uint8)8U)

/** \brief Dcm_RequestType: The request is an internally generated request to perform state
 *  variable settings but not respond to a previous external request which resulted in a jump
 *  to the bootloader. **/
#if (defined DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST)
  #error "DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST is already defined"
#endif
#define DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST ((uint8)9U)

#if (defined DCM_ROE_TYPE1_PHYSICAL_REQUEST)
  #error "DCM_ROE_TYPE1_PHYSICAL_REQUEST is already defined"
#endif
#define DCM_ROE_TYPE1_PHYSICAL_REQUEST ((uint8)10U)

#if (defined DCM_ROE_TYPE1_FUNCTIONAL_REQUEST)
  #error "DCM_ROE_TYPE1_FUNCTIONAL_REQUEST is already defined"
#endif
#define DCM_ROE_TYPE1_FUNCTIONAL_REQUEST ((uint8)11U)

#if (defined DCM_ROE_TYPE2_PHYSICAL_REQUEST)
  #error "DCM_ROE_TYPE2_PHYSICAL_REQUEST is already defined"
#endif
#define DCM_ROE_TYPE2_PHYSICAL_REQUEST ((uint8)12U)

#if (defined DCM_ROE_TYPE2_FUNCTIONAL_REQUEST)
  #error "DCM_ROE_TYPE2_FUNCTIONAL_REQUEST is already defined"
#endif
#define DCM_ROE_TYPE2_FUNCTIONAL_REQUEST ((uint8)13U)

#if (defined DCM_PERIODIC_TYPE1_REQUEST)
  #error "DCM_PERIODIC_TYPE1_REQUEST is already defined"
#endif
#define DCM_PERIODIC_TYPE1_REQUEST ((uint8)14U)

#if (defined DCM_PERIODIC_TYPE2_REQUEST)
  #error "DCM_PERIODIC_TYPE2_REQUEST is already defined"
#endif
#define DCM_PERIODIC_TYPE2_REQUEST ((uint8)15U)

/* GENERATED SECTION END   EA:{5F83F0AA-FFBD-4be4-BFC6-B50A1F694EDE} */

#if (defined DCM_POSTPONED_ROE_REQUEST)
  #error "DCM_POSTPONED_ROE_REQUEST is already defined"
#endif
#define DCM_POSTPONED_ROE_REQUEST  16U

#if (defined DCM_REJECTED_REQUEST)
  #error "DCM_REJECTED_REQUEST is already defined"
#endif
#define DCM_REJECTED_REQUEST  17U

/** \brief This type expresses the status of a MainConnection.
 */

/** \brief The MainConnection is idle. */
#if (defined DCM_MAIN_CONNECTION_IDLE)
  #error "DCM_MAIN_CONNECTION_IDLE is already defined"
#endif
#define DCM_MAIN_CONNECTION_IDLE 0U

/** \brief The MainConnection is processing (there is at least one TxConnection which belongs to
 *         this MainConnection that is currently transmitting data). */
#if (defined DCM_MAIN_CONNECTION_PROCESSING)
  #error "DCM_MAIN_CONNECTION_PROCESSING is already defined"
#endif
#define DCM_MAIN_CONNECTION_PROCESSING 1U

/** \brief This type is DcmDslProtocolID for periodic transmission. */
#if (defined DCM_PERIODIC_ON_CAN)
#error DCM_PERIODIC_ON_CAN already defined
#endif /* #if (defined DCM_PERIODIC_ON_CAN) */

/** \brief Periodic transmission on CAN */
#define DCM_PERIODIC_ON_CAN DCM_PERIODICTRANS_ON_CAN

#if (defined DCM_PERIODIC_ON_FLEXRAY)
#error DCM_PERIODIC_ON_FLEXRAY already defined
#endif /* #if (defined DCM_PERIODIC_ON_FLEXRAY) */

/** \brief Periodic transmission on FlexRay */
#define DCM_PERIODIC_ON_FLEXRAY DCM_PERIODICTRANS_ON_FLEXRAY

/*==================[type definitions]===========================================================*/

/* !LINKSTO Dcm.Dsn.DiagnosticSessionLayer.ConfigurationDataTypes,1 */
/* !LINKSTO Dcm.Dsn.DiagnosticSessionLayer.RuntimeDataTypes,1 */

/** \brief The priority of a Protocol.
 */
typedef uint8 Dcm_ProtocolPriorityType;

/** \brief The ECU address of a Protocol.
 */
typedef uint16 Dcm_ProtocolECUAddressType;

/** \brief The identification address of the tester communicating on this connection.
 */
typedef uint16 Dcm_TesterSourceAddressType;

/** \brief This type is used to hold the value of a P2/P2* adjustment timer.
 */
typedef uint16 Dcm_TimerAdjustType;

/** \brief This type expresses the type of reception permission a certain request to receive
 *         data (external, i.e. from the lower layer, or internal, i.e. from the ROE or Periodic
 *         Response subsystems) may get.
 */

typedef uint8 Dcm_ReceptionPermissionType;

/** \brief This type defines the origin of a request which is being handled by an RxConnection.
 */

typedef uint8 Dcm_RequestOriginType;

/** \brief The index of a ServiceTable to be used by a Protocol.
 */
typedef uint8 Dcm_ServiceTableIndexType;

/** \brief The index of an ROEConnection configuration in the
 *         Dcm_RoeConnectionConfig[] array.
 */
typedef uint8 Dcm_RoeConnectionIndexType;

/** \brief The index of a PeriodicTransmission configuration in the
 *         Dcm_PeriodicTransmissionConfig[] array.
 */
typedef uint8 Dcm_PeriodicTransmissionIndexType;

/** \brief The index of an PeriodicConnection configuration in the
 *         Dcm_PeriodicConnectionConfig[] array.
 */
typedef uint8 Dcm_PeriodicConnectionIndexType;

/** \brief The index of a GenericConnection configuration in the
 *         Dcm_PduInfo[] array.
 */
typedef uint8 Dcm_GenericConnectionIndexType;

/** \brief used to contain the number of PeriodicConnection configurations within
 *         one single PeriodicTransmission configuration.
 */
typedef uint8 Dcm_PeriodicConnectionNumberType;

/** \brief The index of a Protocol in the Dcm_ProtocolConfig[] and Dcm_HsmInfo[] arrays.
 */
typedef uint8 Dcm_ProtocolIndexType;

/** \brief Protocol Stack Number type */
typedef uint8 Dcm_ProtocolStackNumberType;

/** \brief The identifier of Dem Client */
typedef uint8 Dcm_DemClientIdType;

/** \brief This type contains the configuration information for
 *         DcmDslProtocol instances.
 */
typedef struct
{
  /** \brief This parameter is used to guarantee that the first response
   *         is available on the bus before reaching the P2 timer value; Expressed in number of
   *         Dcm_MainFunction() cycles.
   */
  Dcm_TimerAdjustType P2ServerAdjustTime;

  /** \brief This parameter is used to gurantee that the subsequent response
   *         is available on the bus before reaching P2* timer value; Expressed in number of
   *         Dcm_MainFunction() cycles.
   */
  Dcm_TimerAdjustType P2StarServerAdjustTime;

  /** \brief Protocol preemption timeout value; Expressed in number of Dcm_MainFunction() cycles.
   */
  uint16 PreemptionTimeout;

  /** \brief Index of associated NormalBuffer used for transmission by this Protocol;
   *         Indexes the arrays Dcm_BufferConfig[] and Dcm_BufferInfo[].
   */
  Dcm_BufferIndexType TxBufferIndex;

  /** \brief The type of this Protocol (UDS, OBD, etc...). */
  Dcm_ProtocolType Type;

  /** \brief The priority of this Protocol.
   */
  Dcm_ProtocolPriorityType Priority;

  Dcm_ProtocolStackNumberType StackNumber;
  /** \brief Index of the service table used by this Protocol; Indexes the array
   *         Dcm_SidTabConfig[].
   */
  Dcm_ServiceTableIndexType ServiceTableIndex;

  /** \brief Indicates whether EndiannessConversion is to be performed or not for
   *         data of requests to and responses from this Protocol.
   */
  boolean EndiannessConversion;

  /** \brief Specifying if the ECU should send a RequestCorrectlyReceivedResponsePending (0x78)
   *         NRC before transitioning to the Bootloader as a result of a DiagnosticSessionControl
   *         (0x10) service request.
   */
  boolean SendRespPendOnTransToBoot;

  /** \brief Specifying if the ECU should send a ServiceNotSupported (0x11)
   *         NRC when a request for an uncofigured OBD service is processed.
   */
  boolean SupressServiceNotSupportedNRC;
#if (DCM_DEM_CLIENT_REF_USED == STD_ON)
  /** \brief Indicates the Dem Client Identifier */
  Dcm_DemClientIdType DemClientId;
#endif /* #if (DCM_DEM_CLIENT_REF_USED == STD_ON) */
}
Dcm_ProtocolConfigType;

/** \brief Type definition for a constant pointer to a protocol configuration */
typedef CONSTP2VAR(Dcm_ProtocolConfigType, TYPEDEF, DCM_CONST) Dcm_ProtocolConfigPtrType;

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.ConfigurationDataTypes,1 */
/** \brief This type contains the configuration information for
 *         DcmDslMainConnection instances.
 */
typedef struct
{
  /** \brief The transmission PduID (TxPduId) used by this MainConnection for response
   *         transmission.
   */
  PduIdType TxPduId;

  /** \brief The transmission confirmation PduID (ConfirmationTxPduId) associated to the
   *         transmission PduID (TxPduId) used by this MainConnection for response
   *         transmission.
   */
  PduIdType ConfirmationTxPduId;

  /** \brief The PduID (TxEcuPduId) used by this MainConnection for obtaining
   *         MetaData information from EcuC.
   */
  PduIdType TxEcuCPduId;

  /** \brief The identification address of the tester communicating on this connection.
   */
  Dcm_TesterSourceAddressType TesterSourceAddress;

  /** \brief The index of the Protocol to which this MainConnection belongs; Indexes the array
   *         Dcm_ProtocolConfigType[].
   */
  Dcm_ProtocolIndexType ProtocolIndex;

#if (DCM_NUM_ROE_CONNECTIONS > 0U)
  /** \brief Index to the configuration of the transmission parameters used for TYPE2 Roe
   *         responses configured via requests to an RxConnection belonging to this
   *         MainConnection; Indexes the array Dcm_RoeConnectionConfig[].
   */
  Dcm_RoeConnectionIndexType RoeConnectionIndex;
#endif /* #if (DCM_NUM_ROE_CONNECTIONS > 0U) */

#if ((DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))
  /** \brief Index to the configuration of the transmission parameters used for TYPE2 Periodic
   *         responses configured via requests to an RxConnection belonging to this
   *         MainConnection; Indexes the array Dcm_PeriodicTransmissionConfig[].
   */
  Dcm_PeriodicTransmissionIndexType PeriodicTransmissionIndex;
#endif /* #if ((DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U)) */

  /** \brief The index of the GenericConnection to which this MainConnection belongs; Indexes the array
   *         Dcm_PduInfoType[].
   */
  Dcm_GenericConnectionIndexType GenericConnectionIndex;

  /** \brief The flag used by this MainConnection to check if MetaData is configured.
   */
  boolean MetaDataIsConfigured;

}
Dcm_MainConnectionConfigType;

/** \brief This type contains the configuration information for
 *         DcmDslRoeConnection instances.
 */
typedef struct
{
  /** \brief Indicates if this entry contains a valid TxPduId and ConfirmationTxPduId. There may be
   *         entries that are invalid and just exist to point to the protocol on which they are
   *         configure for the purpose of TYPE 1 processing on that protocol.
   */
  boolean Type2PduIdsPresent;

  /** \brief The transmission PduID (TxPduId) used by this connection
   */
  PduIdType TxPduId;

  /** \brief The transmission confirmation PduID (ConfirmationTxPduId) used by this connection.
   */
  PduIdType ConfirmationTxPduId;

  /** \brief The index of the GenericConnection to which this MainConnection belongs; Indexes the array
   *         Dcm_PduInfoType[].
   */
  Dcm_GenericConnectionIndexType GenericConnectionIndex;

  /** \brief The index of the Protocol to which this connection belongs; indexes the array
   *         Dcm_ProtocolConfig[].
   */
  Dcm_ProtocolIndexType ProtocolIndex;
}
Dcm_RoeConnectionConfigType;

/** \brief This type contains the configuration information for
 *         DcmDslPeriodicTransmission instances.
 */
typedef struct
{
  /** \brief The number of contained DcmDslPeriodicConnections.
   */
  Dcm_PeriodicConnectionIndexType NumberOfPeriodicConnections;

   /** \brief The index of the first DcmDslPeriodicConnection belonging
    *         to this DcmDslPeriodicTransmission in the DcmDslPeriodicConnection
    *         table, if this DcmDslPeriodicTransmission is referenced from any
    *         DcmDslMainConnection; Indexes the array Dcm_PeriodicConnectionConfig[].
   */
  Dcm_PeriodicConnectionIndexType IndexOfFirstPeriodicConnection;

  /** \brief The index of the Protocol to which this connection belongs; indexes the array
   *         Dcm_ProtocolConfig[].
   */
  Dcm_ProtocolIndexType ProtocolIndex;
}
Dcm_PeriodicTransmissionConfigType;

/** \brief This type contains the configuration information for
 *         DcmDslPeriodicConnection instances.
 */
typedef struct
{
  /** \brief The transmission PduID (TxPduId) used by this connection.
   */
  PduIdType TxPduId;

  /** \brief The transmission confirmation PduID (ConfirmationTxPduId) used by this connection.
   */
  PduIdType ConfirmationTxPduId;
  /** \brief The index of the GenericConnection to which this MainConnection belongs; Indexes the array
   *         Dcm_PduInfoType[].
   */
  Dcm_GenericConnectionIndexType GenericConnectionIndex;
}
Dcm_PeriodicConnectionConfigType;

/** \brief This type defines the MainConnection operation type.
 */
typedef uint8 Dcm_MainConnectionOperationType;

/** \brief This type holds the status variables of a MainConnection.
 */
typedef struct
{
  /** \brief The status of the MainConnection.
   */
  Dcm_MainConnectionOperationType Status;
}
Dcm_MainConnectionType;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Initializes the CommunicationServices Unit.
 *
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_Init(void);

/** \brief Get the ClientId of a protocol
 *
 *  \param ProtocolIndex [in]    Index of the protocol
 *
 *  \return Dcm_DemClientIdType  ClientId
 */
extern FUNC(Dcm_DemClientIdType, DCM_CODE) Dcm_Dsl_GetDemClientId(
  Dcm_ProtocolIndexType ProtocolIndex
);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Contains the configuration data pertaining to Protocols.
 */
/* Deviation MISRA-1 */
extern CONST(Dcm_ProtocolConfigType, DCM_CONST) Dcm_ProtocolConfig[DCM_NUM_PROTOCOLS];

#if ((STD_ON == DCM_DSP_USE_ROESERVICES) && (DCM_NUM_ROE_CONNECTIONS > 0U))
/** \brief Contains the configuration data pertaining to RoeConnections.
 */
extern CONST(Dcm_RoeConnectionConfigType, DCM_CONST) \
  Dcm_RoeConnectionConfig[DCM_NUM_ROE_CONNECTIONS];
#endif /* #if ((STD_ON == DCM_DSP_USE_ROESERVICES) && (DCM_NUM_ROE_CONNECTIONS > 0U)) */

#if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))
/** \brief Contains the configuration data pertaining to PeriodicTransmissions.
 */
extern CONST(Dcm_PeriodicTransmissionConfigType, DCM_CONST) \
  Dcm_PeriodicTransmissionConfig[DCM_NUM_PERIODIC_TRANSMISSIONS];
#endif /* #if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U)) */

#if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U))
/** \brief Contains the configuration data pertaining to PeriodicConnections.
 */
extern CONST(Dcm_PeriodicConnectionConfigType, DCM_CONST) \
  Dcm_PeriodicConnectionConfig[DCM_NUM_PERIODIC_CONNECTIONS];
#endif /* #if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U)) */

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSL_COMMUNICATIONSERVICES_H */
/*==================[end of file]================================================================*/
