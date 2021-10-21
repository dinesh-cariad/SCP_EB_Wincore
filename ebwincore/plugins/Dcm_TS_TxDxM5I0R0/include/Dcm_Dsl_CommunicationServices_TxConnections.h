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

/* !LINKSTO Dcm.Dsn.File.LowerLayerInterfaces.TxConnection.PrivateApi,1 */
/* This file contains the data types, macro definitions and provides external function declarations used by TxConnections software unit. */

#ifndef DCM_DSL_COMMUNICATIONSERVICES_TXCONNECTIONS_H
#define DCM_DSL_COMMUNICATIONSERVICES_TXCONNECTIONS_H

/*==================[inclusions]=================================================================*/

#include <Dcm.h>
#include <Dcm_Cfg.h>                                     /* Dcm configuration header file */

#include <ComStack_Types.h>                              /* AUTOSAR standard types */
#include <Dcm_Dsl_Cfg.h>                                 /* Dcm Dsl Configuration  */
#include <Dcm_Dsl_CommunicationServices.h>               /* Dcm Dsl Communication Services header file */
#include <Dcm_Dsl_ComMChannelStateManager.h>             /* Dcm Dsl ComMChannelStateManager header file */
#include <Dcm_Dsl_CommunicationServices_RxConnections.h> /* Dcm Dsl RxConnection header file */

#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
#include <Dcm_Dsl_CommunicationServices_MetaData.h>      /* Dcm Dsl Metadata header file */
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)*/

#if (DCM_DEV_ERROR_DETECT == STD_ON)
/* Development Error Tracer (DCM_DET_REPORT_ERROR() macros) */
#include <Det.h>
#endif

/*==================[macros]=====================================================================*/

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.TxPduId,2 */
#if (defined DCM_NUM_TX_CONNECTIONS)
  #error "DCM_NUM_TX_CONNECTIONS is already defined"
#endif
#define DCM_NUM_TX_CONNECTIONS DCM_NUM_TX_PDU_ID

#if (defined DCM_TX_CONNECTION_ID_LOW)
  #error "DCM_TX_CONNECTION_ID_LOW is already defined"
#endif
#define DCM_TX_CONNECTION_ID_LOW (Dcm_BufferUserType)((DCM_RX_CONNECTION_ID_HIGH) + 1U)

#if (defined DCM_TX_CONNECTION_ID_HIGH)
  #error "DCM_TX_CONNECTION_ID_HIGH is already defined"
#endif
#define DCM_TX_CONNECTION_ID_HIGH \
  (Dcm_BufferUserType)((DCM_TX_CONNECTION_ID_LOW) + \
  (Dcm_BufferUserType)(DCM_NUM_TX_CONNECTIONS) - 1U)

#if (defined DCM_TX_CONNECTION_ID_FROM_NRC_BUFFER_ID)
  #error "DCM_TX_CONNECTION_ID_FROM_NRC_BUFFER_ID is already defined"
#endif
#define DCM_TX_CONNECTION_ID_FROM_NRC_BUFFER_ID(NRCBufferId) \
  (Dcm_BufferUserType)((NRCBufferId) - (DCM_BUFFER_NRC_BUFFERS_ID_LOW) + (DCM_TX_CONNECTION_ID_LOW))

#if (defined DCM_TX_CONNECTION_ID_FROM_INDEX)
  #error "DCM_TX_CONNECTION_ID_FROM_INDEX is already defined"
#endif
#define DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex) \
  (Dcm_BufferUserType)((TxConnectionIndex) + (DCM_TX_CONNECTION_ID_LOW))

#if (defined DCM_TX_CONNECTION_INDEX_FROM_ID)
  #error "DCM_TX_CONNECTION_INDEX_FROM_ID is already defined"
#endif
#define DCM_TX_CONNECTION_INDEX_FROM_ID(TxConnectionId) \
  (Dcm_TxConnectionIndexType)((TxConnectionId) - (DCM_TX_CONNECTION_ID_LOW))

#if (defined DCM_TX_VIRTUAL_TX_CONNECTION_ID_LOW)
  #error "DCM_TX_VIRTUAL_TX_CONNECTION_ID_LOW is already defined"
#endif
#define DCM_TX_VIRTUAL_TX_CONNECTION_ID_LOW \
  (Dcm_BufferUserType)((Dcm_BufferUserType)(DCM_TX_CONNECTION_ID_LOW) + \
  (Dcm_BufferUserType)(DCM_TOTAL_NUMBER_OF_REAL_TX_CONNECTIONS))

  /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.VirtualTxConnection,1 */
#if (defined DCM_TX_CONNECTION_IS_VIRTUAL)
  #error "DCM_TX_CONNECTION_IS_VIRTUAL is already defined"
#endif
#define DCM_TX_CONNECTION_IS_VIRTUAL(TxConnectionId) \
  ( (DCM_VIRTUAL_TX_CONNECTIONS_PRESENT == STD_ON)&& \
    (DCM_TX_VIRTUAL_TX_CONNECTION_ID_LOW <= (Dcm_BufferUserType)(TxConnectionId)) && \
    ((TxConnectionId) < (Dcm_BufferUserType)(DCM_TX_CONNECTION_ID_LOW + DCM_NUM_TX_CONNECTIONS)) )
#if (defined DCM_INVALID_TX_CONNECTION_INDEX)
  #error "DCM_INVALID_TX_CONNECTION_INDEX is already defined"
#endif
#define DCM_INVALID_TX_CONNECTION_INDEX (DCM_NUM_TX_CONNECTIONS)

/** \brief The TxConnection is not transmitting data. */
#if (defined DCM_TX_CONNECTION_IDLE)
  #error "DCM_TX_CONNECTION_IDLE is already defined"
#endif
#define DCM_TX_CONNECTION_IDLE 0U

/** \brief The TxConnection is waiting for the ComMChannel associated to the RxPduId on which the
 *   request to which this transmission is a result of arrived to enter FullCommunicationMode. */
#if (defined DCM_TX_CONNECTION_WAITING_FOR_CHANNEL)
  #error "DCM_TX_CONNECTION_WAITING_FOR_CHANNEL is already defined"
#endif
#define DCM_TX_CONNECTION_WAITING_FOR_CHANNEL 1U

/** \brief The TxConnection is waiting for next page (Buffer) of data from the Requester in the
 *         context of Page-Buffered transmission. */
#if (defined DCM_TX_CONNECTION_WAITING_FOR_DATA)
  #error "DCM_TX_CONNECTION_WAITING_FOR_DATA is already defined"
#endif
#define DCM_TX_CONNECTION_WAITING_FOR_DATA 2U

/** \brief The TxConnection is transmitting data, i.e. there is data available to be copied to the
 *         lower layer. */
#if (defined DCM_TX_CONNECTION_TRANSMITTING)
  #error "DCM_TX_CONNECTION_TRANSMITTING is already defined"
#endif
#define DCM_TX_CONNECTION_TRANSMITTING 3U

/** \brief This status is used in order to instruct the TxConnection to
 *         request a new buffer of data from the Dsp in the next Dcm_MainFunction()
 *         cycle. This provides a delay mechanism for suppressed Page-buffered transmission
 *         so as not to continuously process the service handler function, thus leading to
 *         surpassing the DcmTaskTime time budget. */
#if (defined DCM_TX_CONNECTION_REQUEST_DATA)
  #error "DCM_TX_CONNECTION_REQUEST_DATA is already defined"
#endif
#define DCM_TX_CONNECTION_REQUEST_DATA 4U

/** \brief This TxConnection has had its transmission operation cancelled. A TxConnection
 *         may be canceled by either:
 *         - An error while copying data to the lower layer
 *         - An error during providing of the next buffer of data
 *         - A protocol preemption cancelling the transmission */
#if (defined DCM_TX_CONNECTION_CANCELLED)
  #error "DCM_TX_CONNECTION_CANCELLED is already defined"
#endif
#define DCM_TX_CONNECTION_CANCELLED 5U

/** \brief This TxConnection has had errors.*/
#if (defined DCM_TX_CONNECTION_ERROR)
  #error "DCM_TX_CONNECTION_ERROR is already defined"
#endif
#define DCM_TX_CONNECTION_ERROR 6U

/** \brief This TxConnection has timed out.*/
#if (defined DCM_TX_CONNECTION_TIMED_OUT)
  #error "DCM_TX_CONNECTION_TIMED_OUT is already defined"
#endif
#define DCM_TX_CONNECTION_TIMED_OUT 7U

/** \brief This type holds result of either:
 *         - The starting of a transmission operation (starting of the TxConnection state machine);
 *         - The transmission operation.
 */

/** \brief Starting this transmission failed or transmission failed as a result of errors. */
#if (defined DCM_TX_FAILED)
  #error "DCM_TX_FAILED is already defined"
#endif
#define DCM_TX_FAILED 0U

/** \brief This transmission was started or completed successfully. */
#if (defined DCM_TX_OK)
  #error "DCM_TX_OK is already defined"
#endif
#define DCM_TX_OK 1U

/** \brief Starting this transmission is delayed. */
#if (defined DCM_TX_DELAYED)
  #error "DCM_TX_DELAYED is already defined"
#endif
#define DCM_TX_DELAYED 2U

/** \brief Starting this transmission has timed out waiting for the corresponding ComMChannel
 *         to enter FullCommunicationMode. */
#if (defined DCM_TX_TIMED_OUT)
  #error "DCM_TX_TIMED_OUT is already defined"
#endif
#define DCM_TX_TIMED_OUT 3U

/*==================[type definitions]===========================================================*/

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.ConfigurationDataTypes,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.RuntimeDataTypes,1 */

/** \brief The index of a TxConnection in the Dcm_TxConnectionConfig[] arary.
 */
typedef uint16 Dcm_TxConnectionIndexType;

/** \brief This type expresses, in number of Dcm_MainFunction() cycles, the amount of time that
 *         a TxConnection shall wait for either:
 *         - The ComMChannel associated to the RxPduId on which the request to which this
 *           transmission is a result of arrived to enter FullCommunicationMode;
 *         - A new page of data to be available from the Requester in the context of Page-Buffered
 *           transmission.
 */
typedef uint32 Dcm_TransmissionTimeoutType;

/** \brief This type expresses the status of an TxConnection.
 */

typedef uint8 Dcm_TxConnectionStatusType;

/* Note: TxConnections are indexed by ConfirmationTxPduId (as this is a unique identifier). */
/** \brief This type holds the state variables of an TxConnection.
 */
typedef struct
{
  /** \brief Information about the TxPduId necessary in the PduR_DcmTransmit call
   */
  PduInfoType TxPduInfo;

  /** \brief The information of the buffer that needs to go out on the bus.
   */
  Dcm_BufferType Buffer;

  /** \brief The total amount of data left to copy to the lower layer in this session. This may
   *         actually be greater than the amount of data contained in the current buffer, in
   *         case of Page-Buffered transmission.
   */
  PduLengthType AmountLeftToCopy;

  /** \brief Holds the index of valid data in bytes to be transmitted from this Buffer.
   *         This also represents the amount of data which has already been copied to the lower
   *         layer from the current buffer, albeit not confirmed.
   */
  PduLengthType BufferDataMark;

  /** \brief Holds the index of Confirmed data in bytes transmitted from this Buffer.
   *         This also represents the amount of data which has already been copied to the lower
   *         layer from the current buffer and the transmission of which has been confirmed
   *         by the lower layer.
   */
  PduLengthType BufferConfirmedDataMark;

  /** \brief Represents the amount of time that the TxConnection shall wait for either:
   *         - The ComMChannel associated to the RxPduId on which the request to which this
   *           transmission is a result of arrived to enter FullCommunicationMode;
   *         - A new page of data to be available from the Requester in the context of Page-Buffered
   *           transmission.
   */
  Dcm_TransmissionTimeoutType TransmissionTimeout;

  /** \brief Holds the ID of the entity which requested this transmission, and which will
   *         receive the acknowledgment for the transmission.
   */
  Dcm_BufferUserType Requester;

  /** \brief The current status of this TxConnection.
   */
  Dcm_TxConnectionStatusType Status;

  /** \brief  The index of the ComMChannel associated to the RxPduId on which the request
   *          to which this transmission is a result of arrived; Indexes the array
   *          Dcm_RxComMChannelIDs[].
   */
  Dcm_RxComMChannelIDsIndexType ContextComMChannelIdIndex;

  /** \brief  Controls whether the transmission performed by this TxConnection is simulated
   *          (suppressed, i.e. no data is actually passed to the lower layer) or not.
   */
  boolean SimulateTransmission;
}
Dcm_TxConnectionType;

/** \brief This type holds the result of an TxConnection.
 */
typedef uint8 Dcm_TxResultType;

/*==================[external function declarations]=============================================*/

#if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U))
/** \brief Given a TxConnection's Id, determines whether this TxConnection belongs to a
 *         configured TYPE2 DcmPeriodicConnection.
 *
 *  \param TxConnectionId [in] Unique ID of the TxConnection BufferUser entity which handles the
 *                             transmission.
 *
 *  \return boolean
 *  \retval TRUE                   The TxConnection belongs to a TYPE2 DcmPeriodicConnection.
 *  \retval FALSE                  The TxConnection does not belong to a TYPE2
 *                                 DcmPeriodicConnection.
 **/
extern FUNC(boolean, DCM_CODE) Dcm_Dsl_IsTYPE2PeriodicTxConnection(
  Dcm_BufferUserType TxConnectionId
);
#endif /* #if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U)) */

/** \brief Instructs a TxConnection to begin a transmission.
 *
 *  \param TxConnectionId [in]        Unique ID of the TxConnection BufferUser entity which is to
 *                                    handle the transmission.
 *  \param ContextComMChannelId [in]  The index of the ComMChannel associated to the RxPduId on
 *                                    which the request to which this transmission is a result of
 *                                    arrived; Indexes the array Dcm_RxComMChannelIDs[].
 *  \param Requester [in]             Unique ID of the BufferUser entity which wishes to initiate
 *                                    the transmission. This may be either a Protocol or the
 *                                    Supervisor.
 *  \param SuppressTransmission [in]  Controls whether the transmission performed by this
 *                                    TxConnection is simulated (suppressed, i.e. no data is
 *                                    actually passed to the lower layer) or not.
 *  \param AmountToTransmit [in]      The total amount of data (in bytes) that the Requester wishes
 *                                    to transmit in this transmission session.
 *  \param TransmissionTimeout [in]   The amount of time that the TxConnection shall wait for
 *                                    the ComMChannel associated to the RxPduId on which the
 *                                    request to which this transmission is a result of arrived
 *                                    to enter FullCommunicationMode.
 *  \param Buffer [in]                A Buffer element containing the first packet of data to be
 *                                    transmitted. This does not necessarily have to contain
 *                                    the full payload if Page-Bufferred transmission is used.
 *  \return none
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_TxConnectionTransmit(
  Dcm_BufferUserType TxConnectionId,
  NetworkHandleType ContextComMChannelId,
  Dcm_BufferUserType Requester,
  boolean SuppressTransmission,
  Dcm_MsgLenType AmountToTransmit,
  Dcm_TransmissionTimeoutType TransmissionTimeout,
  Dcm_BufferType Buffer
);


/** \brief Cancels any ongoing transmission on an TxConnection.
 *
 *  \param TxConnectionId [in]      Unique ID of the TxConnection the transmission on which is to
 *                                  be cancelled.
 *
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_TxConnectionCancelTransmit(
  Dcm_BufferUserType TxConnectionId
);

/** \brief Provides a TxConnection with the next page of data (within a Buffer element) in the
 *         context of a Page-Bufferred transmission.
 *
 *  \param TxConnectionId [in]      Unique ID of the TxConnection to which the next page of data
 *                                  is provided.
 *  \param Buffer [in]              A Buffer element containing the next page of data to be
 *                                  transmitted.
 *
 *  \return none
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_TxConnectionProcessNextTxBuffer(
  Dcm_BufferUserType TxConnectionId,
  Dcm_BufferType Buffer
);

/** \brief Processes any actions that a  TxConnection may need to retry / may have delayed. This
 *         is necessary in case a TxConnection:
 *         - is waiting for a ComMChannel to enter FullCommunicationMode;
 *         - is waiting for one Dcm_MainFunction() cycle to pass in order to request the next page
 *           of data from a BufferUser entity in case of a Page-Buffered transmission.
 *
 *  \return none
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_TxConnectionProcessTxConnections(void);


/** \brief Explicitly changes the ownership of a TxConnection.
 *         Only the current requester of a TxConnection may transfer ownership to another
 *         BufferUser.
 *
 *  \param Provider [in]           Unique ID of the BufferUser entity which wishes to change
 *                                 the ownership of the TxConnection. This has to be the same ID as
 *                                 the current owner of the TxConnection.
 *  \param Receiver [in]           Unique ID of the BufferUser entity which shall become the
 *                                 new owner of this TxConnection.
 *  \param TxConnectionId [in]     ID of the TxConnection changing its owner.
 *
 *  \return Std_ReturnType
 *  \retval E_OK                   The TxConnection owner was changed successfully.
 *  \retval E_NOT_OK               The TxConnection cannot have its owner changed as the
 *                                 BufferUser entity requesting the ownership change is not the current
 *                                 owner.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_TxConnectionGive(
  Dcm_BufferUserType Provider,
  Dcm_BufferUserType Receiver,
  Dcm_BufferUserType TxConnectionId
);

/** \brief Get the state of a TxConnection
 *
 *  \param TxConnectionId [in]  Id of the TxConnection
 *
 *  \return Dcm_TxConnectionStatusType
 *  \retval DCM_TX_CONNECTION_IDLE                TxConnection is not transmitting data
 *  \retval DCM_TX_CONNECTION_WAITING_FOR_CHANNEL TxConnection is waiting for the ComMChannel
 *  \retval DCM_TX_CONNECTION_WAITING_FOR_DATA    TxConnection is waiting for the next page of data
 *  \retval DCM_TX_CONNECTION_TRANSMITTING        TxConnection is transmitting data
 *  \retval DCM_TX_CONNECTION_REQUEST_DATA        TxConnection requested a new buffer of data
 *  \retval DCM_TX_CONNECTION_CANCELLED           TxConnection has had its transmission operation
 *                                                cancelled
 */
extern FUNC(Dcm_TxConnectionStatusType, DCM_CODE) Dcm_Dsl_TxConnectionGetState(
  Dcm_BufferUserType TxConnectionId
);

/** \brief Confirms that a transmission session has ended to the Requester BufferUser entity
 *         that triggered this transmission.
 *
 *  \param Requester [in]               The unique BufferUser ID of the entity which requested this
 *                                      transmission session.
 *  \param Result [in]                  The result of the transmission (successful or failed)
 *  \param TxConnectionId [in]          The unique BufferUser ID of the TxConnection requesting the
 *                                      next transmission Buffer to be provided
 *
 *  \return none
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_ConfirmTransmissionToRequester(
  Dcm_BufferUserType Requester,
  NotifResultType Result,
  Dcm_BufferUserType TxConnectionId
);

/** \brief Initializes the state variables of a single TxConnection entity.
 *
 *  \param TxConnectionIndex [in] Index of the TxConnection to be initialized. This is the same as
 *                                the corresponding ConfirmationTxPduId.
 *
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_TxConnectionInitSingleTxConnection(
  Dcm_TxConnectionIndexType TxConnectionIndex
);

/** \brief Requests the next Buffer of data in a Paged Buffer transmission context from the
 *         Requester BufferUser entity of the transmission. This function is a wrapper for the
 *         corresponding _ProvideNextTxBuffer() API of the Protocol stack as only Protocols may
 *         execute page-buffered transmissions.
 *
 *  \param Requester [in]               The unique BufferUser ID of the entity which requested this
 *                                      transmission session.
 *  \param TxConnectionId [in]          The unique BufferUser ID of the TxConnection requesting the
 *                                      next transmission Buffer to be provided
 *
 *  \return Std_ReturnType
 *  \retval DCM_E_OK                    The Buffer containing a further page of data to be
 *                                      will be supplied by the Requester transmitted.
 *  \retval DCM_E_NOT_OK                An error has occurred (such as the TxConnection
 *                                      requesting more data than the Requester entity may
 *                                      ever supply)
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_ProvideNextTxBuffer(
  Dcm_BufferUserType Requester,
  Dcm_BufferUserType TxConnectionId
);

/** \brief Initializes all TxConnections status variables.
 *
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_TxConnectionInit(void);

/** \brief Adjusts the data mark indexes of a transmission buffer according to the
 *         information of the RetryInfoPtr received in the Dcm_CopyTxData() call if
 *         this pointer contains valid information.
 *
 *  \param RetryInfoPtr [in]            Pointer to retry information given by the lower layer
 *                                      in the context of the Dcm_CopyTxData() API call.
 *  \param BufferDataMark [in]          Index within the transmission buffer where the next byte
 *                                      to be copied to the lower layer is located.
 *  \param BufferConfirmedDataMark [in] Index within the transmission buffer up to which all the
 *                                      previous bytes have been confirmed as successfully copied
 *                                      by the lower layer.
 *
 *  \return Std_ReturnType
 *  \retval DCM_E_OK                    Either the RetryInfoPtr is a NULL pointer or points to
 *                                      valid data. The data marks have been successfully updated.
 *  \retval DCM_E_NOT_OK                The RetryInfoPtr points to data which is incompatible to
 *                                      current transmission session.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_ProcessRetryInformation(
  P2VAR(RetryInfoType, AUTOMATIC, DCM_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) BufferDataMark,
  P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) BufferConfirmedDataMark
);

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Contains the state variables of all the TxConnection entities.
 */
extern VAR(Dcm_TxConnectionType, DCM_VAR) Dcm_TxConnections[DCM_NUM_TX_CONNECTIONS];

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/** \brief Controls whether there is at least one TxConnection which needs to have its current
 *         state serviced during the next Dcm_MainFunction() cycle.
 */
extern VAR (boolean, DCM_VAR) Dcm_ProcessTxConnectionsOnMainFunction;

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSL_COMMUNICATIONSERVICES_TXCONNECTIONS_H */
/*==================[end of file]================================================================*/
