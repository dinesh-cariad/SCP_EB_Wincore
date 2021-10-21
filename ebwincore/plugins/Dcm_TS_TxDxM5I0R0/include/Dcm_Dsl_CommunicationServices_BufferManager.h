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

/* !LINKSTO Dcm.Dsn.File.LowerLayerInterfaces.BufferManager.PrivateApi,1 */
/* This file contains the data types, macro definitions and provides external function declarations used by BufferManager software unit. */

#ifndef DCM_DSL_COMMUNICATIONSERVICES_BUFFERMANAGER_H
#define DCM_DSL_COMMUNICATIONSERVICES_BUFFERMANAGER_H

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
#include <Dcm_Cfg.h>                                  /* Dcm configuration header file */

#include <ComStack_Types.h>                           /* AUTOSAR standard types */
#include <Dcm_Dsl_Cfg.h>                              /* Dcm Dsl Configuration  */
#include <Dcm_Dsl_ComMChannelStateManager.h>          /* Dcm Dsl ComMChannelStateManager header file */

#if (DCM_DEV_ERROR_DETECT == STD_ON)
/* Development Error Tracer (DCM_DET_REPORT_ERROR() macros) */
#include <Det.h>
#endif

/*==================[macros]=====================================================================*/
#if (defined DCM_BUFFER_CONFIGURED_BUFFERS_ID_LOW)
  #error "DCM_BUFFER_CONFIGURED_BUFFERS_ID_LOW is already defined"
#endif
#define DCM_BUFFER_CONFIGURED_BUFFERS_ID_LOW 0

#if (defined DCM_BUFFER_CONFIGURED_BUFFERS_ID_HIGH)
  #error "DCM_BUFFER_CONFIGURED_BUFFERS_ID_HIGH is already defined"
#endif
#define DCM_BUFFER_CONFIGURED_BUFFERS_ID_HIGH \
  (Dcm_BufferIdType)((Dcm_BufferIdType)(DCM_NUM_CONFIGURED_BUFFERS) - 1U)

#if (defined DCM_BUFFER_DEFAULT_BUFFERS_ID_LOW)
  #error "DCM_BUFFER_DEFAULT_BUFFERS_ID_LOW is already defined"
#endif
#define DCM_BUFFER_DEFAULT_BUFFERS_ID_LOW \
  (Dcm_BufferIdType)((Dcm_BufferIdType)(DCM_BUFFER_CONFIGURED_BUFFERS_ID_HIGH) + 1U)

#if (defined DCM_BUFFER_DEFAULT_BUFFERS_ID_HIGH)
  #error "DCM_BUFFER_DEFAULT_BUFFERS_ID_HIGH is already defined"
#endif
#define DCM_BUFFER_DEFAULT_BUFFERS_ID_HIGH \
  (Dcm_BufferIdType)((Dcm_BufferIdType)(DCM_BUFFER_DEFAULT_BUFFERS_ID_LOW) + \
  (Dcm_BufferIdType)(DCM_NUM_DEFAULT_BUFFERS) - 1U)

#if (defined DCM_BUFFER_NRC_BUFFERS_ID_LOW)
  #error "DCM_BUFFER_NRC_BUFFERS_ID_LOW is already defined"
#endif
#define DCM_BUFFER_NRC_BUFFERS_ID_LOW \
  (Dcm_BufferIdType)((Dcm_BufferIdType)(DCM_BUFFER_DEFAULT_BUFFERS_ID_HIGH) + 1U)

#if (defined DCM_BUFFER_NRC_BUFFERS_ID_HIGH)
  #error "DCM_BUFFER_NRC_BUFFERS_ID_HIGH is already defined"
#endif
#define DCM_BUFFER_NRC_BUFFERS_ID_HIGH \
  (Dcm_BufferIdType)((Dcm_BufferIdType)(DCM_BUFFER_NRC_BUFFERS_ID_LOW) + \
  (Dcm_BufferIdType)(DCM_NUM_NRC_BUFFERS) - 1U)

#if (defined DCM_BUFFER_INVALID_ID)
  #error "DCM_BUFFER_INVALID_ID is already defined"
#endif
#define DCM_BUFFER_INVALID_ID (Dcm_BufferIdType)((DCM_BUFFER_NRC_BUFFERS_ID_HIGH) + 1U)

#if (defined DCM_NRC_BUFFER_ID)
  #error "DCM_NRC_BUFFER_ID is already defined"
#endif
#define DCM_NRC_BUFFER_ID(TxConnectionIndex) \
  (Dcm_BufferIdType)((Dcm_BufferIdType)(DCM_BUFFER_NRC_BUFFERS_ID_LOW) + \
  (Dcm_BufferIdType)(TxConnectionIndex))

#if (defined DCM_DEFAULT_BUFFER_ID_FROM_RX_CONNECTION_ID)
  #error "DCM_DEFAULT_BUFFER_ID_FROM_RX_CONNECTION_ID is already defined"
#endif
#define DCM_DEFAULT_BUFFER_ID_FROM_RX_CONNECTION_ID(RxPduId) \
  (Dcm_BufferIdType)((DCM_BUFFER_DEFAULT_BUFFERS_ID_LOW) + (RxPduId))

#if (defined DCM_BUFFER_IS_NORMAL_BUFFER)
  #error "DCM_BUFFER_IS_NORMAL_BUFFER is already defined"
#endif
#define DCM_BUFFER_IS_NORMAL_BUFFER(BufferId) \
  ((BufferId) <= (DCM_BUFFER_CONFIGURED_BUFFERS_ID_HIGH))

#if (defined DCM_BUFFER_IS_DEFAULT_BUFFER)
  #error "DCM_BUFFER_IS_DEFAULT_BUFFER is already defined"
#endif
#define DCM_BUFFER_IS_DEFAULT_BUFFER(BufferId) \
  ((((DCM_BUFFER_DEFAULT_BUFFERS_ID_LOW) <= (BufferId)) && \
  ((BufferId) <= (DCM_BUFFER_DEFAULT_BUFFERS_ID_HIGH))))

#if (defined DCM_BUFFER_IS_NRC_BUFFER)
  #error "DCM_BUFFER_IS_NRC_BUFFER is already defined"
#endif
#define DCM_BUFFER_IS_NRC_BUFFER(BufferId) \
  ((((DCM_BUFFER_NRC_BUFFERS_ID_LOW) <= (BufferId)) && \
  ((BufferId) <= (DCM_BUFFER_NRC_BUFFERS_ID_HIGH))))

/* These are all the possible owner entities of a buffer. They are identified by unique IDs*/

#if (defined DCM_TX_CONNECTION_INDEX_FROM_NRC_BUFFER_ID)
  #error "DCM_TX_CONNECTION_INDEX_FROM_NRC_BUFFER_ID is already defined"
#endif
#define DCM_TX_CONNECTION_INDEX_FROM_NRC_BUFFER_ID(NRCBufferId) \
  ((NRCBufferId) - (DCM_BUFFER_NRC_BUFFERS_ID_LOW))

#if (defined DCM_RX_CONNECTION_ID_FROM_DEFAULT_BUFFER_ID)
  #error "DCM_RX_CONNECTION_ID_FROM_DEFAULT_BUFFER_ID is already defined"
#endif
#define DCM_RX_CONNECTION_ID_FROM_DEFAULT_BUFFER_ID(DefaultBufferId) \
  ((DefaultBufferId) - (DCM_BUFFER_DEFAULT_BUFFERS_ID_LOW))

#if (defined DCM_RX_CONNECTION_INDEX_FROM_DEFAULT_BUFFER_ID)
  #error "DCM_RX_CONNECTION_INDEX_FROM_DEFAULT_BUFFER_ID is already defined"
#endif
#define DCM_RX_CONNECTION_INDEX_FROM_DEFAULT_BUFFER_ID(DefaultBufferId) \
  ((DefaultBufferId) - (DCM_BUFFER_DEFAULT_BUFFERS_ID_LOW))

/* While a buffer is being used for reception, it belongs to the RxConnection */
/* While a buffer is being written to it belongs to a Protocol entity */
/* While a buffer is being used for transmission, it belongs to the TxConnection */

#if (defined DCM_NRC_BUFFER_ID_FROM_TX_CONNECTION_ID)
  #error "DCM_NRC_BUFFER_ID_FROM_TX_CONNECTION_ID is already defined"
#endif
#define DCM_NRC_BUFFER_ID_FROM_TX_CONNECTION_ID(TxConnectionId) \
  (Dcm_BufferIdType)(((TxConnectionId) + (DCM_BUFFER_NRC_BUFFERS_ID_LOW)) - \
  (DCM_TX_CONNECTION_ID_LOW))

/** \brief An invalid size for a buffer allocation. */
#if (defined DCM_BUFFER_INVALID_SIZE)
  #error "DCM_BUFFER_INVALID_SIZE is already defined"
#endif
#define DCM_BUFFER_INVALID_SIZE 0U

/** \brief An invalid but to be ignored size for a buffer allocation. */
#if (defined DCM_BUFFER_IGNORE_BUFFER_SIZE)
  #error "DCM_BUFFER_IGNORE_BUFFER_SIZE is already defined"
#endif
#define DCM_BUFFER_IGNORE_BUFFER_SIZE 0U

/** \brief The default (2-byte) size of a DefaultBuffer for reception. */
#if (defined DCM_BUFFER_DEFAULT_BUFFER_SIZE)
  #error "DCM_BUFFER_DEFAULT_BUFFER_SIZE is already defined"
#endif
#define DCM_BUFFER_DEFAULT_BUFFER_SIZE 2U

/** \brief The default (3-byte) size of a NRCBuffer for transmission. */
#if (defined DCM_BUFFER_NRC_BUFFER_SIZE)
  #error "DCM_BUFFER_NRC_BUFFER_SIZE is already defined"
#endif
#define DCM_BUFFER_NRC_BUFFER_SIZE 3U

/** \brief Buffer is currently not in use */
#if (defined DCM_BUFFER_IDLE)
  #error "DCM_BUFFER_IDLE is already defined"
#endif
#define DCM_BUFFER_IDLE 0U

/** \brief Buffer is used for reception */
#if (defined DCM_BUFFER_RX_NORMAL_PENDING)
  #error "DCM_BUFFER_RX_NORMAL_PENDING is already defined"
#endif
#define DCM_BUFFER_RX_NORMAL_PENDING 1U

/** \brief Buffer contains a completed reception */
#if (defined DCM_BUFFER_RX_NORMAL_DONE)
  #error "DCM_BUFFER_RX_NORMAL_DONE is already defined"
#endif
#define DCM_BUFFER_RX_NORMAL_DONE 2U

/** \brief Buffer is used for ROE request simulation */
#if (defined DCM_BUFFER_RX_ROE_PENDING)
  #error "DCM_BUFFER_RX_ROE_PENDING is already defined"
#endif
#define DCM_BUFFER_RX_ROE_PENDING 3U

/** \brief Buffer contains a ROE request simulation */
#if (defined DCM_BUFFER_RX_ROE_DONE)
  #error "DCM_BUFFER_RX_ROE_DONE is already defined"
#endif
#define DCM_BUFFER_RX_ROE_DONE 4U

/** \brief Buffer is used for default 2 byte reception */
#if (defined DCM_BUFFER_RX_DEFAULT_PENDING)
  #error "DCM_BUFFER_RX_DEFAULT_PENDING is already defined"
#endif
#define DCM_BUFFER_RX_DEFAULT_PENDING 5U

/** \brief Buffer contains a completed default 2 byte reception */
#if (defined DCM_BUFFER_RX_DEFAULT_DONE)
  #error "DCM_BUFFER_RX_DEFAULT_DONE is already defined"
#endif
#define DCM_BUFFER_RX_DEFAULT_DONE 6U

/** \brief Buffer is used for normal transmission */
#if (defined DCM_BUFFER_TX_NORMAL_PENDING)
  #error "DCM_BUFFER_TX_NORMAL_PENDING is already defined"
#endif
#define DCM_BUFFER_TX_NORMAL_PENDING 7U

/** \brief Buffer has completed a normal transmission */
#if (defined DCM_BUFFER_TX_NORMAL_DONE)
  #error "DCM_BUFFER_TX_NORMAL_DONE is already defined"
#endif
#define DCM_BUFFER_TX_NORMAL_DONE 8U

/** \brief Buffer is used for ROE transmission */
#if (defined DCM_BUFFER_TX_ROE_PENDING)
  #error "DCM_BUFFER_TX_ROE_PENDING is already defined"
#endif
#define DCM_BUFFER_TX_ROE_PENDING 9U

/** \brief Buffer has completed a ROE transmission */
#if (defined DCM_BUFFER_TX_ROE_DONE)
  #error "DCM_BUFFER_TX_ROE_DONE is already defined"
#endif
#define DCM_BUFFER_TX_ROE_DONE 10U

/** \brief Buffer is used for a periodic transmission */
#if (defined DCM_BUFFER_TX_PERIODIC_PENDING)
  #error "DCM_BUFFER_TX_PERIODIC_PENDING is already defined"
#endif
#define DCM_BUFFER_TX_PERIODIC_PENDING 11U

/** \brief Buffer has completed a periodic transmission */
#if (defined DCM_BUFFER_TX_PERIODIC_DONE)
  #error "DCM_BUFFER_TX_PERIODIC_DONE is already defined"
#endif
#define DCM_BUFFER_TX_PERIODIC_DONE 12U

/** \brief Buffer is used for NRC transmission */
#if (defined DCM_BUFFER_TX_NRC_PENDING)
  #error "DCM_BUFFER_TX_NRC_PENDING is already defined"
#endif
#define DCM_BUFFER_TX_NRC_PENDING 13U

/** \brief Buffer has completed an NRC transmission */
#if (defined DCM_BUFFER_TX_NRC_DONE)
  #error "DCM_BUFFER_TX_NRC_DONE is already defined"
#endif
#define DCM_BUFFER_TX_NRC_DONE 14U

/** \brief This buffer is currently being filled with data by
           a normal protocol processing */
#if (defined DCM_BUFFER_PROCESSING_NORMAL)
  #error "DCM_BUFFER_PROCESSING_NORMAL is already defined"
#endif
#define DCM_BUFFER_PROCESSING_NORMAL 15U

/** \brief This buffer is currently being filled with data by
           a ROE processing */
#if (defined DCM_BUFFER_PROCESSING_ROE)
  #error "DCM_BUFFER_PROCESSING_ROE is already defined"
#endif
#define DCM_BUFFER_PROCESSING_ROE 16U

/** \brief This buffer is currently being filled with data by
           a periodic processing */
#if (defined DCM_BUFFER_PROCESSING_PERIODIC)
  #error "DCM_BUFFER_PROCESSING_PERIODIC is already defined"
#endif
#define DCM_BUFFER_PROCESSING_PERIODIC 17U

/** \brief This buffer is currently being filled with data by
           a NRC processing. This is never used as the NRC buffer's
           state and owner are never queried. */
#if (defined DCM_BUFFER_PROCESSING_NRC)
  #error "DCM_BUFFER_PROCESSING_NRC is already defined"
#endif
#define DCM_BUFFER_PROCESSING_NRC 18U

/** \brief Buffer is currently being used for a generic transmission. */
#if (defined DCM_BUFFER_TX)
  #error "DCM_BUFFER_TX is already defined"
#endif
#define DCM_BUFFER_TX 19U

/** \brief This type escribes a buffer's intended usage (the reason for which it is being
 *         allocated).
 */

/** \brief Buffer is currently not in use */
#if (defined DCM_BUFFER_NONE)
  #error "DCM_BUFFER_NONE is already defined"
#endif
#define DCM_BUFFER_NONE 0U

/** \brief Buffer is used for reception */
#if (defined DCM_BUFFER_RX_NORMAL)
  #error "DCM_BUFFER_RX_NORMAL is already defined"
#endif
#define DCM_BUFFER_RX_NORMAL 1U

/** \brief Buffer is used for ROE request simulation */
#if (defined DCM_BUFFER_RX_ROE)
  #error "DCM_BUFFER_RX_ROE is already defined"
#endif
#define DCM_BUFFER_RX_ROE 2U

/** \brief Buffer is used for default 2 byte reception */
#if (defined DCM_BUFFER_RX_DEFAULT)
  #error "DCM_BUFFER_RX_DEFAULT is already defined"
#endif
#define DCM_BUFFER_RX_DEFAULT 3U

/** \brief Buffer is used for normal transmission */
#if (defined DCM_BUFFER_TX_NORMAL)
  #error "DCM_BUFFER_TX_NORMAL is already defined"
#endif
#define DCM_BUFFER_TX_NORMAL 4U

/** \brief Buffer is used for ROE transmission */
#if (defined DCM_BUFFER_TX_ROE)
  #error "DCM_BUFFER_TX_ROE is already defined"
#endif
#define DCM_BUFFER_TX_ROE 5U

/** \brief Buffer is used for periodic transmission */
#if (defined DCM_BUFFER_TX_PERIODIC)
  #error "DCM_BUFFER_TX_PERIODIC is already defined"
#endif
#define DCM_BUFFER_TX_PERIODIC 6U

/** \brief Buffer is used for NRC transmission */
#if (defined DCM_BUFFER_TX_NRC)
  #error "DCM_BUFFER_TX_NRC is already defined"
#endif
#define DCM_BUFFER_TX_NRC 7U

/*==================[type definitions]===========================================================*/

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.BufferManager.RuntimeDataTypes,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.BufferManager.ConfigurationDataTypes,1 */

/** \brief The index of a NormalBuffer in the Dcm_BufferConfig[] and Dcm_BufferInfo[] arrays.
 */
typedef uint8 Dcm_BufferIndexType;

/** \brief The unique ID of a NormalBuffer, DefaultBuffer or NRCBuffer.
 */
typedef uint16 Dcm_BufferIdType;

/** \brief This type identifies a unique BufferUser entity. BufferUsers may be either:
 *         - RxConnections;
 *         - TxConnections;
 *         - Protocols;
 *         - The Supervisor unit.
 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Users,1 */
typedef uint16 Dcm_BufferUserType;

typedef uint32 Dcm_BufferSizeType;

/* !LINKSTO Dcm549,1 */
/** \brief This type describes the current state of a buffer in the Dcm module
 */
typedef uint8 Dcm_BufferStateType;

typedef uint8 Dcm_BufferUsageType;

/** \brief This type contains status information about a specific buffer.
 */
typedef struct
{
  /** \brief Current user of this buffer; */
  Dcm_BufferUserType User;

  /** \brief Current state of this buffer */
  Dcm_BufferStateType State;
}Dcm_BufferInfoType;

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.BufferManager.ConfigurationDataTypes,1 */
/** \brief This type contains the configuration information a buffer used for communication.
 */
typedef struct
{
  /** \brief Size of this buffer
   */
  Dcm_BufferSizeType Size;
  /** \brief location of buffer
   */
  Dcm_MsgType BufferPtr;
#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
  /** \brief Location of default buffer
   */
  Dcm_MsgType DefaultBufferPtr;
#endif
} Dcm_BufferConfigType;

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Type,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer,1 */
/** \brief This type contains the data necessary for usage of a buffer. */
typedef struct
{
  /** \brief Size of this buffer
   */
  Dcm_BufferSizeType Size;
  /** \brief The amount of valid data, in bytes, currently stored in this buffer
   */
  Dcm_BufferSizeType FilledLength;
  /** \brief id of the buffer
   */
  Dcm_BufferIdType BufferId;

  /** \brief location of buffer
   */
  Dcm_MsgType BufferPtr;
} Dcm_BufferType;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Allocates a Buffer to a Requester.
 *
 *  \param Requester [in]          Unique ID of the BufferUser entity requesting the Buffer.
 *  \param Use [in]                Intended usage of the Buffer.
 *  \param Size [in]               The minimum size requested for the Buffer.
 *  \param Buffer [out]            A Buffer element containing the information of the Buffer
 *                                 received from the Buffer Manager.
 *
 *  \return BufReq_ReturnType
 *  \retval BUFREQ_OK              The Buffer can be allocated.
 *  \retval BUFREQ_E_OVFL          The Buffer cannot be allocated as the configured buffer is
 *                                 too small for the requested size.
 *  \retval BUFREQ_E_BUSY          The Buffer cannot be allocated as it is currently used by
 *                                 another BufferUser entity.
 */
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_BufferAllocate(
  Dcm_BufferUserType Requester,
  Dcm_BufferUsageType Use,
  Dcm_BufferSizeType Size,
  P2VAR(Dcm_BufferType, AUTOMATIC, DCM_VAR) Buffer
);

/** \brief Wrapper for the Dcm_Dsl_BufferInitializeSingleBuffer(), which frees a single
 *         NormalBuffer.
 *
 *  \param Requester [in]          Unique ID of the BufferUser entity which wishes to free the
 *                                 Buffer. This has to be the same ID as the current owner of
 *                                 the Buffer for NormalBuffers.
 *  \param BufferID [in]           Unique ID of the Buffer element to free.
 *
 *  \return BufReq_ReturnType
 *  \retval BUFREQ_OK              The Buffer can be freed.
 *  \retval BUFREQ_E_NOT_OK        The Buffer cannot be freed as it is either:
 *                                 - not a NormalBuffer
 *                                 - owned by another BufferUser than the Requester.
 */
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_BufferFree(
  Dcm_BufferUserType Requester,
  Dcm_BufferIdType BufferId
);

/** \brief Explicitly changes the state of a NormalBuffer element. For a NormalBuffer,
 *         only the current user of a Buffer may change its state.
 *
 *  \param CurrentUser [in]         Unique ID of the BufferUser entity which wishes to change
 *                                 the state of the Buffer. This has to be the same ID as
 *                                 the current owner of the Buffer for NormalBuffers.
 *  \param NewState [in]           The new state the Buffer takes.
 *  \param BufferID [in]           Unique ID of the Buffer element to free.
 *
 *  \return BufReq_ReturnType
 *  \retval BUFREQ_OK              The Buffer state was changed successfully.
 *  \retval BUFREQ_E_NOT_OK        The Buffer cannot have its state changed as it is either:
 *                                 - not a NormalBuffer
 *                                 - owned by another BufferUser than the current user.
 */
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_BufferChangeState(
  Dcm_BufferUserType CurrentUser,
  Dcm_BufferStateType NewState,
  Dcm_BufferIdType BufferId
);

/** \brief Explicitly changes the ownership of a NormalBuffer element. For a NormalBuffer,
 *         only the current user of a Buffer may transfer ownership to another BufferUser.
 *
 *  \param Provider [in]           Unique ID of the BufferUser entity which wishes to change
 *                                 the ownership of the Buffer. This has to be the same ID as
 *                                 the current owner of the Buffer for NormalBuffers.
 *  \param Receiver [in]           Unique ID of the BufferUser entity which shall become the
 *                                 new owner of this Buffer.
 *  \param BufferId [in]           ID of the Buffer changing its owner.
 *
 *  \return BufReq_ReturnType
 *  \retval BUFREQ_OK              The Buffer owner was changed successfully.
 *  \retval BUFREQ_E_NOT_OK        The Buffer cannot have its owner changed as it is either:
 *                                 - not a NormalBuffer
 *                                 - owned by another BufferUser than the Provider.
 */
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_BufferGive(
  Dcm_BufferUserType Provider,
  Dcm_BufferUserType Receiver,
  Dcm_BufferIdType BufferId
);

/** \brief Initializes the Buffer Manager and all the Buffer state variables.
 *
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_BufferInit(void);

/** \brief Provides a Buffer element containing the information about the buffer assigned
 *         to a certain BufferUser entity for a specific use case. This functions like
 *         an allocation, only bypassing the permissions checking and not actually
 *         allocating the Buffer entity to the Requester.
 *
 *  \param Requester [in]          Unique ID of the BufferUser entity which wishes to
 *                                 receive information about the configured buffer
 *  \param Use [in]                The intended use case of the buffer.
 *  \param Buffer [out]            A Buffer element containing the information about the
 *                                 Buffer received from the Buffer Manager.
 *
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_BufferGetInfo(
  Dcm_BufferUserType Requester,
  Dcm_BufferUsageType Use,
  P2VAR(Dcm_BufferType, AUTOMATIC, DCM_VAR) Buffer
);

/** \brief Frees a NormalBuffer. For a NormalBuffer, only the the current user of a Buffer may
 *                               free it.
 *
 *  \param Requester  [in]          Unique ID of the BufferUser entity which wishes to free the
 *                                  Buffer. This has to be the same ID as the current owner of
 *                                  the Buffer for NormalBuffers.
 *  \param BufferIdRx  [in]         Unique ID of the Rx Buffer element to free.
 *  \param BufferIdTx  [in]         Unique ID of the Tx Buffer element to free.
 *  \param BufferIdNRC [in]         Unique ID of the NRC Buffer element to free.
 *
 *  \return void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_BufferFreeRxTxNRC(
  Dcm_BufferUserType Requester,
  Dcm_BufferIdType BufferIdRx,
  Dcm_BufferIdType BufferIdTx,
  Dcm_BufferIdType BufferIdNRC
);

/** \brief Frees a buffer.
 *
 *  \param Requester [in]          Unique ID of the BufferUser entity which wishes to free the
 *                                 Buffer.
 *  \param BufferID  [in]          Unique ID of the Buffer element to free.
 *
 *  \return BufReq_ReturnType
 *  \retval BUFREQ_OK              The Buffer can be freed.
 *  \retval BUFREQ_E_NOT_OK        The Buffer cannot be freed as it is either:
 *                                 - not a NormalBuffer
 *                                 - owned by another BufferUser than the Requester.
 */
extern FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_BufferInitializeSingleBuffer(
  Dcm_BufferUserType Requester,
  Dcm_BufferIdType BufferId
);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/** \brief Contains memory space allocated for all configured (NormalBuffer) buffer elements. */
/* Deviation MISRAC2012-1 */
/* Deviation MISRA-1 */
extern VAR(uint8, DCM_VAR) Dcm_Buffers[DCM_TOTAL_CONFIGURED_BUFFER_SIZE];

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
/** \brief Contains memory space allocate for all the DefaultBuffers which have the size
           of all used RxBuffers.
 */
extern VAR(uint8, DCM_VAR) Dcm_DefaultBuffers[DCM_TOTAL_RX_CONFIGURED_BUFFER_SIZE];
#else
/** \brief Contains memory space allocated for all the DefaultBuffers. There is one 2-byte
 *         DefaultBuffer per RxConnection.
 */
extern VAR(uint8, DCM_VAR) Dcm_DefaultBuffers[2 * DCM_NUM_DEFAULT_BUFFERS];
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Contains the configuration data pertaining to the configured NormalBuffers. */
extern CONST(Dcm_BufferConfigType, DCM_CONST) Dcm_BufferConfig[DCM_NUM_CONFIGURED_BUFFERS];

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSL_COMMUNICATIONSERVICES_BUFFERMANAGER_H */
/*==================[end of file]================================================================*/
