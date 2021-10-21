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

/* !LINKSTO Dcm.Dsn.File.LowerLayerInterfaces.RxConnection.PrivateApi,1 */
/* This file contains the data types, macro definitions and provides external function declarations used by RxConnections software unit. */

#ifndef DCM_DSL_COMMUNICATIONSERVICES_RXCONNECTIONS_H
#define DCM_DSL_COMMUNICATIONSERVICES_RXCONNECTIONS_H

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

/*==================[inclusions]=================================================================*/

#include <Dcm.h>
#include <Dcm_Cfg.h>                                  /* Dcm configuration header file */

#include <ComStack_Types.h>                           /* AUTOSAR standard types */
#include <Dcm_Dsl_Cfg.h>                              /* Dcm Dsl Configuration  */
#include <Dcm_Dsl_ComMChannelStateManager.h>          /* Dcm Dsl ComMChannelStateManager header file */
#include <Dcm_Dsl_CommunicationServices.h>            /* Dcm Dsl Communication Services header file */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
/* Development Error Tracer (DCM_DET_REPORT_ERROR() macros) */
#include <Det.h>
#endif

/*==================[macros]=====================================================================*/

#if (defined DCM_NUM_RX_CONNECTIONS)
  #error "DCM_NUM_RX_CONNECTIONS is already defined"
#endif
#define DCM_NUM_RX_CONNECTIONS DCM_NUM_RX_PDU_ID

#if (defined DCM_RX_CONNECTION_ID_LOW)
  #error "DCM_RX_CONNECTION_ID_LOW is already defined"
#endif
#define DCM_RX_CONNECTION_ID_LOW 0U

#if (defined DCM_RX_CONNECTION_ID_HIGH)
  #error "DCM_RX_CONNECTION_ID_HIGH is already defined"
#endif
#define DCM_RX_CONNECTION_ID_HIGH \
  (Dcm_BufferUserType)((Dcm_BufferUserType)(DCM_NUM_RX_CONNECTIONS) - 1U)

#if (defined DCM_RX_CONNECTION_ID_FROM_INDEX)
  #error "DCM_RX_CONNECTION_ID_FROM_INDEX is already defined"
#endif
#define DCM_RX_CONNECTION_ID_FROM_INDEX(RxConnectionIndex) (Dcm_BufferUserType)(RxConnectionIndex)

#if (defined DCM_RX_CONNECTION_ID_FROM_RXPDUID)
  #error "DCM_RX_CONNECTION_ID_FROM_RXPDUID is already defined"
#endif
#define DCM_RX_CONNECTION_ID_FROM_RXPDUID(RxPduID) (Dcm_BufferUserType)(RxPduID)

/* !LINKSTO Dcm.EB.Config.LowerLayerInterfaces.PduRInterface.DcmDslProtocolRxConnectionId,1*/
#if (defined DCM_RX_CONNECTION_INDEX_FROM_ID)
  #error "DCM_RX_CONNECTION_INDEX_FROM_ID is already defined"
#endif
#define DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId) (Dcm_RxConnectionIndexType)(RxConnectionId)

/** \brief This type expresses the type of reception permission a certain request to receive
 *         data (external, i.e. from the lower layer, or internal, i.e. from the ROE or Periodic
 *         Response subsystems) may get.
 */

/** \brief The reception shall be rejected. */
#if (defined DCM_RX_REJECT)
  #error "DCM_RX_REJECT is already defined"
#endif
#define DCM_RX_REJECT 0U

/** \brief The reception shall be accepted as a regular reception and held in the NormalBuffer
 *         which is configured for this RxConnection. */
#if (defined DCM_RX_ACCEPT)
  #error "DCM_RX_ACCEPT is already defined"
#endif
#define DCM_RX_ACCEPT 1U

/** \brief The reception shall be accepted as a potential concurrent TesterPresent (0x3E 0x80)
 *         reception and held in the DefaultBuffer of this RxConnection. */
#if (defined DCM_RX_ACCEPT_TESTERPRESENT)
  #error "DCM_RX_ACCEPT_TESTERPRESENT is already defined"
#endif
#define DCM_RX_ACCEPT_TESTERPRESENT 2U

/** \brief The reception shall be accepted as a request which will be responded to with a
 *         BusyRepeatRequest (0x21) NRC and held in the DefaultBuffer of this RxConnection. */
#if (defined DCM_RX_ACCEPT_BUSYREPEATREQUEST)
  #error "DCM_RX_ACCEPT_BUSYREPEATREQUEST is already defined"
#endif
#define DCM_RX_ACCEPT_BUSYREPEATREQUEST 3U

/** \brief The reception shall be temporarily declined. This permission shall further be
 *         changed to either DCM_RX_ACCEPT_TESTERPRESENT, DCM_RX_ACCEPT_BUSYREPEATREQUEST or
 *         DCM_RX_REJECT depending on the configuration and current system state. */
#if (defined DCM_RX_DECLINE)
  #error "DCM_RX_DECLINE is already defined"
#endif
#define DCM_RX_DECLINE 4U

/** \brief The reception shall be postponed due to an ongoing service request and should be
 *         retransmitted in the next cycle. */
#if (defined DCM_RX_POSTPONE)
  #error "DCM_RX_POSTPONE is already defined"
#endif
#define DCM_RX_POSTPONE 5U

/** \brief This type expresses the status of an RxConnection.
 */

/** \brief The RxConnection is not receiving data.
 *         Note: As soon as an RxConnection has finished a reception,
 *         it immediately goes into the IDLE state. */
#if (defined DCM_RX_CONNECTION_IDLE)
  #error "DCM_RX_CONNECTION_IDLE is already defined"
#endif
#define DCM_RX_CONNECTION_IDLE 0U

/** \brief The RxConnection is currently receiving data. */
#if (defined DCM_RX_CONNECTION_RECEIVING)
  #error "DCM_RX_CONNECTION_RECEIVING is already defined"
#endif
#define DCM_RX_CONNECTION_RECEIVING 1U

/** \brief The RxConnection has cancelled receiving data as a result of an error or of an explicit
           request to cancel receiving by the Supervisor. */
#if (defined DCM_RX_CONNECTION_CANCELLED)
  #error "DCM_RX_CONNECTION_CANCELLED is already defined"
#endif
#define DCM_RX_CONNECTION_CANCELLED 2U

/*==================[type definitions]===========================================================*/

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.RxConnection.ConfigurationDataTypes,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.RxConnection.RuntimeDataTypes,1 */

/** \brief This type describes the addressing type used in an RxConnection.
 */

typedef uint8 Dcm_AddressingTypeType;

/** \brief The index of a MainConnection in the Dcm_MainConnectionConfig[] arary.
 */
typedef uint8 Dcm_MainConnectionIndexType;

/** \brief The index of an RxConnection in the Dcm_RxConnectionConfig[] array.
 */
typedef uint16 Dcm_RxConnectionIndexType;

/** \brief The index of a ComMChannelId in the Dcm_RxComMChannelIDs[] array.
 */
typedef uint8 Dcm_RxComMChannelIDsIndexType;

typedef uint8 Dcm_RequestTypeType;

typedef uint8 Dcm_RxConnectionStatusType;

/** \brief This type holds the state variables of an RxConnection.
 */
typedef struct
{
  /** \brief The descriptor of the Buffer element into which this reception is taking place.
   */
  Dcm_BufferType Buffer;

  /** \brief Length of valid data in bytes stored in the buffer assigned to this connection.
   */
  PduLengthType ExpectedRxLength;

  /** \brief What this connection is receiving and how it shall be treated.
   */
  Dcm_RequestTypeType RequestType;

  /** \brief The status of the RxConnection.
   */
  Dcm_RxConnectionStatusType Status;
}
Dcm_RxConnectionType;

/** \brief This type aggregates the information pertaining to a single RxPduId
 */
typedef struct {
  /** \brief  The Rx buffer size
   */
  Dcm_BufferSizeType RxBufferSize;

  /** \brief The index of the RxConnection to which this RxPduId belongs. This is the same as
   *         the RxPduId; Indexes the array Dcm_RxConnections[].
   */
  Dcm_RxConnectionIndexType RxConnectionIndex;

  /** \brief The unique BufferUser ID of the RxConnection to which this RxPduId belongs. This
   *         is the same as the RxPduId.
   */
  Dcm_BufferUserType RxConnectionId;

  /** \brief The unique BufferUser ID of the Protocol to which this RxPduId belongs.
   */
  Dcm_BufferUserType ProtocolId;

  /** \brief The unique Buffer ID of the NormalBuffer assigned to this RxPduId.
   */
  Dcm_BufferIdType RxBufferId;

  /** \brief The unique Buffer ID of the DefautlBuffer assigned to this RxPduId.
   */
  Dcm_BufferIdType DefaultRxBufferId;

  /** \brief The index of the MainConnection to which this RxPduId belongs; Indexes the array
   *         Dcm_RxConnections[].
   */
  Dcm_MainConnectionIndexType MainConnectionIndex;

  /** \brief The index of the Protocol to which this RxPduId belongs; Indexes the arrays
   *         Dcm_ProtocolConfig[] and Dcm_HsmInfo[].
   */
  Dcm_ProtocolIndexType ProtocolIndex;

  /** \brief The addressing type configured for this RxPduId.
   */
  Dcm_AddressingTypeType AddressingType;

  /** \brief  The index of the ComMChannel associated to this RxPduId; Indexes the array
   *          Dcm_RxComMChannelIDs[].
   */
  Dcm_RxComMChannelIDsIndexType ChannelIdIndex;

  /** \brief The network id of the communication channel from Dcm_RxComMChannelIDs[] associated to
             this RxPduId.
   */
  NetworkHandleType ComMChannelId;

} Dcm_RxPduIdInfoType;

/** \brief This type contains the configuration information for each reception (Rx) PduID of the
 *         Dcm module.
 */
typedef struct
{
  /** \brief The PduID (RxEcuPduId) used by this MainConnection for obtaining
   *         MetaData information from EcuC.
   */
  PduIdType RxEcuCPduId;

  /** \brief Addressing Type for the RxPduId represented by this RxConnection
   */
  Dcm_AddressingTypeType AddressingType;

  /** \brief ID of associated buffer used for this RxConnection for physical receptions;
   *         Indexes the array Dcm_BufferConfig[].
   */
  Dcm_BufferIndexType BufferIndex;

  /** \brief ID of associated Main connection for this RxConnection;
   *         Indexes the array Dcm_MainConnectionConfig[].
   */
  Dcm_MainConnectionIndexType MainConnectionIndex;

  /** \brief Index of ComMChannel Identifier to which the RxPduId represented by this RxConnection
   *         belongs; Indexes the array Dcm_RxComMChannelIDs[].
   */
  /* NOTE: in AUTOSAR4.2.1, this index is given per MainConnection.
           Therefore, this parameter might become deprecated and may
           be relocated in later versions */
  Dcm_RxComMChannelIDsIndexType ChannelIdIndex;
}
Dcm_RxPduIdConfigType;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Provides a means to give a Buffer to an RxConnection so that the RxConnection may pass
 *         it on to the Supervisor module for processing. This is the mechanism by which ROE
 *         and Periodic response processing is performed in the context of the RxConnection which
 *         configured the behaviour.
 *
 *  \param RxConnectionId [in]     Unique ID of the RxConnection to which the Buffer is
 *                                 transferred.
 *  \param RequestType [in]        The type of request represented by the Buffer
 *  \param Buffer [in]             A Buffer element containing data defining the request to be
 *                                 passed on to the Supervisor for processing.
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_RxConnectionRxIndication(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType RequestType,
  Dcm_BufferType RequestBuffer
);
/** \brief Provides the information about a configured RxPduId
 *
 *  \param RxPduId [in]          The configured RxPduId.
 *  \param RxPduIdInfo [out]     A structure containing the information requested.
 *
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_RxConnectionGetRxPduIdInformation(
  PduIdType RxPduId,
  P2VAR(Dcm_RxPduIdInfoType, AUTOMATIC, DCM_VAR) RxPduIdInfo
);

/** \brief Cancels any ongoing reception on an RxConnection.
 *
 *  \param RxConnectionId [in]      Unique ID of the RxConnection the reception on which is to be
 *                                  cancelled.
 *
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_RxConnectionCancelReceive(
  Dcm_BufferUserType RxConnectionId
);

/** \brief Get the state of a RxConnection
 *
 *  \param RxConnectionId [in]  Id of the RxConnection
 *
 *  \return Dcm_RxConnectionStatusType
 *  \retval DCM_RX_CONNECTION_IDLE                RxConnection is not receiving data
 *  \retval DCM_RX_CONNECTION_RECEIVING           RxConnection is currently receiving data
 *  \retval DCM_RX_CONNECTION_CANCELLED           RxConnection has cancelled receiving data

 */
extern FUNC(Dcm_RxConnectionStatusType, DCM_CODE) Dcm_Dsl_RxConnectionGetState(
  Dcm_BufferUserType RxConnectionId
);

/** \brief Initializes all RxConnections status variables.
 *
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_RxConnectionInit(void);

/** \brief Sets up an RxConnection entity so that a data reception for a pre-determined
 *         request type and having a length advertised by the lower layer can be handled
 *         on the corresponding RxPduId, using a Buffer element previously allocated for
 *         this purpose.
 *
 *  \param RxConnectionIndex [in]  Index of the RxConnection which is to be set-up. This
 *                                 has the same value as the RxPduId and the ID of the
 *                                 RxConnection entity.
 *  \param RequestType [in]        The type of the request which the RxConnection shall
 *                                 be receiving. This is predetermined based on the
 *                                 permission of the Supervisor, the type of Buffer which
 *                                 could be allocated, the RxPduId and and the incoming
 *                                 size.
 *  \param Buffer [in]             The Buffer in which this reception shall be handled
 *  \param ExpectedRxLength [in]   The expected amount of data to be received.
 *
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_SetupRxConnection(
  Dcm_RxConnectionIndexType RxConnectionIndex,
  Dcm_RequestTypeType RequestType,
  Dcm_BufferType Buffer,
  PduLengthType ExpectedRxLength
);

/** \brief Initializes the state variables of a single RxConnection entity.
 *
 *  \param RxConnectionIndex [in] Index of the RxConnection to be initialized. This is the same as
 *                                the corresponding unique BufferUser ID and the corresponding
 *                                RxPduId.
 *
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_RxConnectionInitSingleRxConnection(
  Dcm_RxConnectionIndexType RxConnectionIndex
);

/** \brief Allocates a Buffer for reception depending on the current permissions
 *         of the system. Depending on the current situation of the Buffer elements
 *         within the Buffer Manager, this function can change the RxPermission
 *         given by the caller.
 *
 *  \param RxConnectionId [in]     Unique ID of the RxConnection requesting the Buffer.
 *  \param RequestedSize [in]      The minimum size requested for the Buffer.
 *  \param RxPermission [inout]    The permission previously calculated from the internal
 *                                 state of the RxConnections and the permission given by
 *                                 the Supervisor. The Buffer Manager is free to downgrade
 *                                 this permission to another type in case the configured
 *                                 Buffer cannot be allocated, in order to allow the
 *                                 reception to continue using a DefaultBuffer for a
 *                                 potential Concurrent TesterPresent (0x3E 0x80) request or
 *                                 in order to respond with the BusyRepeatRequest (0x21)
 *                                 NRC.
 *  \param Buffer [out]            A Buffer element containing the information of the Buffer
 *                                 received from the Buffer Manager.
 *
 *  \return BufReq_ReturnType
 *  \retval BUFREQ_OK              The Buffer can be allocated.
 *  \retval BUFREQ_E_OVFL          The Buffer cannot be allocated as it is too small for the
 *                                 requested size.
 *  \retval BUFREQ_E_BUSY          The Buffer cannot be allocated as it is currently used by
 *                                 another BufferUser entity.
 */
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_AllocateRxBuffer(
  Dcm_BufferUserType RxConnectionId,
  PduLengthType RequestedSize,
  P2VAR(Dcm_RequestTypeType, AUTOMATIC, DCM_VAR)  RequestType,
  P2VAR(Dcm_BufferType, AUTOMATIC, DCM_VAR) Buffer
);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Contains the configuration data pertaining to RxPduIds.
 */
/* Deviation MISRA-1 */
extern CONST(Dcm_RxPduIdConfigType, DCM_CONST) Dcm_RxPduIdConfig[DCM_NUM_RX_PDU_ID];

/** \brief Contains the configuration data pertaining to MainConnection.
 */
/* Deviation MISRA-1 */
extern CONST(Dcm_MainConnectionConfigType, DCM_CONST) \
  Dcm_MainConnectionConfig[DCM_NUM_MAIN_CONNECTIONS];

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSL_COMMUNICATIONSERVICES_RXCONNECTIONS_H */
/*==================[end of file]================================================================*/
