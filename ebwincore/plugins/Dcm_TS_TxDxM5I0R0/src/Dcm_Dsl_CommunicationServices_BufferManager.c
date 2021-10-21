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

/* !LINKSTO Dcm.Dsn.File.LowerLayerInterfaces.BufferManager.Impl,1 */
/* This file contains the implementation of the BufferManager software unit. */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * This warnings is a false positive.
 * The compiler optimizes the 'for' loop (which has only 1 iteration) in the following way
 * - it prepares in advance the index for the next iteration
 * - if the index is equal to '0' it goes back to check the 'for' loop conditions, otherwise it just moves on
 * - since the index is already equal to '1', it never jumps back to check the exit condition
 */

/*==================[inclusions]=================================================================*/

#include <ComStack_Types.h>                               /* AUTOSAR Communication Stack types */
#include <TSAutosar.h>
#include <TSMem.h>

#include <Dcm_Dsl_CommunicationServices_BufferManager.h>  /* Dcm Dsl BufferManager header file */
#include <Dcm_Dsl_CommunicationServices_RxConnections.h>  /* Dcm Dsl RxConnections header file */
#include <Dcm_Dsl_ComMChannelStateManager.h>              /* Dcm Dsl CComMChannelStateManager header
                                                             file */
#include <Dcm_Dsl_Supervisor.h>                           /* Dcm Dsl Supervisor header file */

#include <Dcm.h>                                          /* Definition of DCM_MODULE_ID */
#include <Dcm_Int.h>                                      /* Module intenal definitions */
#include <Dcm_HsmDcm.h>                                   /* public statechart model definitions */
#include <TSMem.h>                                        /* EB specific memory functions */
#include <SchM_Dcm.h>                                     /* SchM interface for Dcm */
#include <PduR_Dcm.h>                                     /* PduR interface for Dcm */
#include <ComM_Dcm.h>                                     /* ComM interface for Dcm */
#include <Dcm_Trace.h>
#include <Dcm_Cbk.h>

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Returns whether the requested Buffer can be assigned for the given Size, Use and
 *         by the Requester executing the call.
 *
 *  \param Requester [in]          ID of the BufferUser entity requesting the Buffer.
 *  \param Use [in]                Intended usage for which the Buffer is requested.
 *  \param Size [in]               The minimum size requested for the Buffer.
 *  \param BufferId [in]           ID of the buffer which is being requested.
 *
 *  \return BufReq_ReturnType
 *  \retval BUFREQ_OK              The Buffer can be allocated.
 *  \retval BUFREQ_E_OVFL          The Buffer cannot be allocated as it is too small for the
 *                                 requested size.
 *  \retval BUFREQ_E_BUSY          The Buffer cannot be allocated as it is currently used by
 *                                 another BufferUser entity.
 */
STATIC FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_BufferAllocationPermission(
  Dcm_BufferUserType Requester,
  Dcm_BufferUsageType Use,
  Dcm_BufferSizeType Size,
  Dcm_BufferIdType BufferId
);

/** \brief Initializes a single NormalBuffer's Dcm_BufferInfoType status entry
 *         with the default values.
 *
 *  \param[in] BufferIndex         The index of the NormalBuffer.
 *
 *  \return none
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_BufferInitSingleBuffer(
  Dcm_BufferIndexType BufferIndex
);

/** \brief Verifies whether the Requester may reserve a Buffer for the stated use case.
 *
 *  \param Requester [in]                 The unique ID of the requester BufferUser entity.
 *  \param Use [in]                       The usage for which the ID of the Buffer is requested.
 *
 *  \return boolean
 *  \retval TRUE                          The Requester may reserve the Buffer for the stated
 *                                        use case.
 *  \retval FALSE                         The Requester may not reserve the Buffer for the stated
 *                                        use case.
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsl_BufferUseValidForRequester(
  Dcm_BufferUserType Requester,
  Dcm_BufferUsageType Use
);

/** \brief Returns the ID of the NormalBuffer configured for the Requester entity for a
 *         given usage.
 *
 *  \param Requester [in]                 The unique ID of the requester BufferUser entity.
 *  \param Use [in]                       The usage for which the ID of the Buffer is requested.
 *
 *  \return Dcm_BufferIdType
 *  \retval 0..DCM_BUFFER_INVALID_ID-1    The unique BufferID of the Buffer entity reserved for
 *                                        the Requester for the given Use.
 *
 */
STATIC FUNC(Dcm_BufferIdType, DCM_CODE) Dcm_Dsl_BufferGetId(
  Dcm_BufferUserType Requester,
  Dcm_BufferUsageType Use
);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.BufferManager.RuntimeDataElements,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Types,2 */

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.NormalBuffers,1 */
/** \brief Contains memory space allocated for all configured (NormalBuffer) buffer elements.
           This has external linkage as it's referenced from the Configuration file.
 */
VAR(uint8, DCM_VAR) Dcm_Buffers[DCM_TOTAL_CONFIGURED_BUFFER_SIZE];

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/*==================[internal data]==============================================================*/
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>
/** \brief Contains the state variables of the configured NormalBuffers.
 */
STATIC VAR(Dcm_BufferInfoType, DCM_VAR) Dcm_BufferInfo[DCM_NUM_CONFIGURED_BUFFERS];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.FirstNRC0x78.SubsequentNRC0x78.SendNRCFromNRCBuffers,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.NRCBuffers,1 */
/* Each DcmDslMainConnection contains one NRC buffer*/
/** \brief Contains memory space allocated for all the NRCBuffers. There is one 3-byte
 *         NRCBuffer per TxConnection.
 */
STATIC VAR(uint8, DCM_VAR) Dcm_NRCBuffers[3 * DCM_NUM_NRC_BUFFERS];

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
VAR(uint8, DCM_VAR) Dcm_DefaultBuffers[DCM_TOTAL_RX_CONFIGURED_BUFFER_SIZE];
#else
  /* Each Rx connection has a default 2-byte buffer used in order to receive
     at most 2 bytes of data in the following circumstances:
     - The reception is Physical AND
       the corresponding DcmDslMainConnection is NOT busy (a BusyRepeatRequest NRC can be sent) AND
       the received request cannot be processed because a higher or equal priority job is being
       processed or
     - The reception is Functional AND either:
       - the corresponding DcmDslMainConnection is busy or
       - the request cannot be processed because a higher or equal priority job is
       being processed */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Types,2 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.DefaultBuffers,1 */
/** \brief Contains memory space allocated for all the DefaultBuffers. There is one 2-byte
 *         DefaultBuffer per RxConnection.
 */
VAR(uint8, DCM_VAR) Dcm_DefaultBuffers[2 * DCM_NUM_DEFAULT_BUFFERS];
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* Gets the information of a certain buffer without performing
   an allocation.

   WARNING: the BufferPtr shall be NULL as NO ALLOCATION takes place! */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.GetInfo,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_BufferGetInfo(
  Dcm_BufferUserType Requester,
  Dcm_BufferUsageType Use,
  P2VAR(Dcm_BufferType, AUTOMATIC, DCM_VAR) Buffer
)
{
  Dcm_BufferIdType BufferId;

  DBG_DCM_DSL_BUFFERGETINFO_ENTRY(Requester, Use, Buffer);

  Buffer->BufferPtr = NULL_PTR;

  BufferId = Dcm_Dsl_BufferGetId(Requester,
                                Use);

  if (DCM_BUFFER_IS_NORMAL_BUFFER(BufferId))
  {
    Buffer->Size = Dcm_BufferConfig[BufferId].Size;
  }
  else if (DCM_BUFFER_IS_DEFAULT_BUFFER(BufferId))
  {
#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
    Buffer->Size = Dcm_BufferConfig[Dcm_RxPduIdConfig[Requester].BufferIndex].Size;
#else
    Buffer->Size = DCM_BUFFER_DEFAULT_BUFFER_SIZE;
#endif
  }
  else if (DCM_BUFFER_IS_NRC_BUFFER(BufferId))
  {
    Buffer->Size = DCM_BUFFER_NRC_BUFFER_SIZE;
  }
  else
  {
    /* Basically an inexistent buffer */
    Buffer->Size = 0U;
  }

  Buffer->BufferId = BufferId;

  /* The amount of data in a buffer is of no interest to
     the buffer manager: only users of a buffer are interested
     in it. */

  Buffer->FilledLength = 0U;

  DBG_DCM_DSL_BUFFERGETINFO_EXIT(Requester, Use, Buffer);
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.State,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Owner,2 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Usage,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Allocation,1 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_BufferAllocate(
  Dcm_BufferUserType Requester,
  Dcm_BufferUsageType Use,
  Dcm_BufferSizeType Size,
  P2VAR(Dcm_BufferType, AUTOMATIC, DCM_VAR) Buffer
)
{
  BufReq_ReturnType AllocationPermission = BUFREQ_E_NOT_OK;
  Dcm_BufferStateType BufferState = DCM_BUFFER_IDLE;
  Dcm_BufferIdType BufferId;

  DBG_DCM_DSL_BUFFERALLOCATE_ENTRY(Requester, Use, Size, Buffer);

  /* Get the ID of the buffer we wish to allocate */
  BufferId = Dcm_Dsl_BufferGetId(Requester,
                                Use);

  if (DCM_BUFFER_INVALID_ID != BufferId)
  {
    /*
     * ENTER critical section
     */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    AllocationPermission = Dcm_Dsl_BufferAllocationPermission(
                             Requester,
                             Use,
                             Size,
                             BufferId);

    if (BUFREQ_OK == AllocationPermission)
    {
      switch(Use)
      {
        case DCM_BUFFER_RX_NORMAL:
          BufferState = DCM_BUFFER_RX_NORMAL_PENDING;
          break;
#if (STD_ON == DCM_DSP_USE_ROESERVICES)
        case DCM_BUFFER_RX_ROE:
          BufferState = DCM_BUFFER_RX_ROE_PENDING;
          break;
#endif /* #if (STD_ON == DCM_DSP_USE_ROESERVICES) */
        case DCM_BUFFER_RX_DEFAULT:
          /* DCM_BUFFER_RX_DEFAULT_PENDING but default buffers
             have only two possible uses, and the use is
             always given by the RxConnection as this buffer
             is not further dispatched. */
          break;
        case DCM_BUFFER_TX_NORMAL:
          BufferState = DCM_BUFFER_PROCESSING_NORMAL;
          break;
#if (STD_ON == DCM_DSP_USE_ROESERVICES)
        case DCM_BUFFER_TX_ROE:
          BufferState = DCM_BUFFER_PROCESSING_ROE;
          break;
#endif /* #if (STD_ON == DCM_DSP_USE_ROESERVICES) */
#if (STD_ON == DCM_PERIODIC_USED)
        case DCM_BUFFER_TX_PERIODIC:
          BufferState = DCM_BUFFER_PROCESSING_PERIODIC;
          break;
#endif /* #if (STD_ON == DCM_PERIODIC_USED) */
        case DCM_BUFFER_TX_NRC:
          /* DCM_BUFFER_PROCESSING_NRC but NRC buffers
             have only one possible use.*/
          break;

          /* CHECK: NOPARSE */
        default:
          /* Unreachable default case mandated by MISRA */
          DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
          break;
          /* CHECK: PARSE */
      }

      if (DCM_BUFFER_IS_NORMAL_BUFFER(BufferId))
      {
        Dcm_BufferInfo[BufferId].State = BufferState;
        Dcm_BufferInfo[BufferId].User = Requester;

        Buffer->BufferPtr = (uint8*)(Dcm_BufferConfig[BufferId].BufferPtr);
        Buffer->Size = Dcm_BufferConfig[BufferId].Size;
      }
      else if (DCM_BUFFER_IS_DEFAULT_BUFFER(BufferId))
      {
        const Dcm_BufferIdType RegularRxBufferId = Dcm_RxPduIdConfig[Requester].BufferIndex;
#if(DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
        DCM_PRECONDITION_ASSERT(
          Dcm_BufferConfig[RegularRxBufferId].DefaultBufferPtr != NULL_PTR, DCM_INTERNAL_API_ID);
        /* If TxConnectionNotification is enabled, then use the address of the DefaultBuffer
         * saved in Dcm_BufferConfig. Because the entire message context it must be
         * saved in order to be transmitted to the Dcm_ApplicationTransmissionConfirmation API
         */
        Buffer->BufferPtr = (uint8*)(Dcm_BufferConfig[RegularRxBufferId].DefaultBufferPtr);
#else
        /* If TxConfirmationNotification is disabled, then calculate the address of the
         * first byte from where copying will begin. The address of Dcm_DefaultBuffers is
         * determine by DefaultBufferSize which is 2 bytes in this case multiple by RxPduID. */
        Buffer->BufferPtr =
          (uint8*)&Dcm_DefaultBuffers[DCM_BUFFER_DEFAULT_BUFFER_SIZE *
                                      DCM_RX_CONNECTION_INDEX_FROM_DEFAULT_BUFFER_ID(BufferId)];
#endif
        Buffer->Size = Dcm_BufferConfig[RegularRxBufferId].Size;
      }
      else
      {
#if (DCM_PRECONDITION_ASSERT_ENABLED == STD_ON)
        boolean BuffIsNrcType = (DCM_BUFFER_IS_NRC_BUFFER(BufferId))?(TRUE):(FALSE);
#endif
        DCM_PRECONDITION_ASSERT((FALSE != BuffIsNrcType), DCM_INTERNAL_API_ID);
        Buffer->BufferPtr =
          (uint8*)&Dcm_NRCBuffers[DCM_BUFFER_NRC_BUFFER_SIZE *
                                  DCM_TX_CONNECTION_INDEX_FROM_NRC_BUFFER_ID(BufferId)];
        Buffer->Size = DCM_BUFFER_NRC_BUFFER_SIZE;
      }

      Buffer->BufferId = BufferId;
    }

    /*
     * EXIT critical section
     */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  }
  else
  {
    Buffer->BufferId = DCM_BUFFER_INVALID_ID;
    Buffer->Size = 0U;
    Buffer->BufferPtr = NULL_PTR;
  }

  DBG_DCM_DSL_BUFFERALLOCATE_EXIT(AllocationPermission, Requester, Use, Size, Buffer);

  return AllocationPermission;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Free,1 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_BufferFree(
  Dcm_BufferUserType Requester,
  Dcm_BufferIdType BufferId
)
{
  BufReq_ReturnType FreeResult = BUFREQ_E_NOT_OK;

  DBG_DCM_DSL_BUFFERFREE_ENTRY(Requester, BufferId);

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  FreeResult = Dcm_Dsl_BufferInitializeSingleBuffer(Requester, BufferId);

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  DBG_DCM_DSL_BUFFERFREE_EXIT(FreeResult, Requester, BufferId);

  return FreeResult;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.StateChange,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.StateChangeAllowed,1 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_BufferChangeState(
  Dcm_BufferUserType CurrentUser,
  Dcm_BufferStateType NewState,
  Dcm_BufferIdType BufferId
)
{
  BufReq_ReturnType StateChangeResult = BUFREQ_E_NOT_OK;

  DBG_DCM_DSL_BUFFERCHANGESTATE_ENTRY(CurrentUser,NewState, BufferId);

  if ((DCM_BUFFER_IS_NORMAL_BUFFER(BufferId)) &&
      (Dcm_BufferInfo[BufferId].User == CurrentUser))
  {
    Dcm_BufferInfo[BufferId].State = NewState;

    StateChangeResult = BUFREQ_OK;
  }
  else
  {
    StateChangeResult = BUFREQ_E_NOT_OK;
  }

  DBG_DCM_DSL_BUFFERCHANGESTATE_EXIT(StateChangeResult, CurrentUser,NewState, BufferId);

  return StateChangeResult;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.State,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Owner,2 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Usage,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Give,1 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_BufferGive(
  Dcm_BufferUserType Provider,
  Dcm_BufferUserType Receiver,
  Dcm_BufferIdType BufferId
)
{
  BufReq_ReturnType OwnerTransferResult = BUFREQ_E_NOT_OK;

  DBG_DCM_DSL_BUFFERGIVE_ENTRY(Provider, Receiver, BufferId);
  /* Only the current user of a buffer may change its owner it
     and only normal buffers are allowed to switch hands. */

  /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.TransferAllowed,1 */
  if ((DCM_BUFFER_IS_NORMAL_BUFFER(BufferId)) &&
      (Provider == Dcm_BufferInfo[BufferId].User))
  {
    Dcm_BufferInfo[BufferId].User = Receiver;

    OwnerTransferResult = BUFREQ_OK;
  }
  else
  {
    OwnerTransferResult = BUFREQ_E_NOT_OK;
  }

  DBG_DCM_DSL_BUFFERGIVE_EXIT(OwnerTransferResult, Provider, Receiver, BufferId);

  return OwnerTransferResult;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.State,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Owner,2 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Init,2 */
FUNC(void, DCM_CODE) Dcm_Dsl_BufferInit(void)
{
  uint16_least NormalBufferIndex;

  DBG_DCM_DSL_BUFFERINIT_ENTRY();
  /* Initialize the state of all NormalBuffers. */
  /* Deviation TASKING-1 */
  for (NormalBufferIndex = 0 ; NormalBufferIndex < DCM_NUM_CONFIGURED_BUFFERS; NormalBufferIndex++)
  {
    Dcm_Dsl_BufferInitSingleBuffer((Dcm_BufferIndexType)NormalBufferIndex);
  }

  /* DefaultBuffers and NRCBuffers don't have state variables and therefore they don't need to be
     initialized separately. */
  DBG_DCM_DSL_BUFFERINIT_EXIT();
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.BufferManager.BufferFreeRxTxNRC,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_BufferFreeRxTxNRC(
  Dcm_BufferUserType Requester,
  Dcm_BufferIdType BufferIdRx,
  Dcm_BufferIdType BufferIdTx,
  Dcm_BufferIdType BufferIdNRC)
{
  DBG_DCM_DSL_BUFFERFREERXTXNRC_ENTRY(Requester, BufferIdRx, BufferIdTx, BufferIdNRC);

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  (void)Dcm_Dsl_BufferInitializeSingleBuffer(Requester, BufferIdRx);
  (void)Dcm_Dsl_BufferInitializeSingleBuffer(Requester, BufferIdTx);
  (void)Dcm_Dsl_BufferInitializeSingleBuffer(Requester, BufferIdNRC);

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  DBG_DCM_DSL_BUFFERFREERXTXNRC_EXIT();
}


#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.BufferManager.BufferUseValidForRequester,1 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsl_BufferUseValidForRequester(
  Dcm_BufferUserType Requester,
  Dcm_BufferUsageType Use
)
{
  boolean UseIsValid = FALSE;

  if ((((DCM_BUFFER_RX_NORMAL == Use) ||
        (DCM_BUFFER_RX_ROE == Use) ||
        (DCM_BUFFER_RX_DEFAULT == Use)
       ) && (DCM_USER_IS_RX_CONNECTION(Requester))) ||
      (((DCM_BUFFER_TX_NORMAL == Use) ||
        (DCM_BUFFER_TX_ROE == Use) ||
        (DCM_BUFFER_TX_PERIODIC == Use)
       ) && (DCM_USER_IS_PROTOCOL(Requester))) ||
      (((DCM_BUFFER_TX_NRC == Use)
       && ((DCM_USER_IS_TX_CONNECTION(Requester))))))
  {
    UseIsValid = TRUE;
  }

  return UseIsValid;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.UniqueId,2 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Usage,1 */
STATIC FUNC(Dcm_BufferIdType, DCM_CODE) Dcm_Dsl_BufferGetId(
  Dcm_BufferUserType Requester,
  Dcm_BufferUsageType Use
)
{
  Dcm_BufferIdType BufferId = DCM_BUFFER_INVALID_ID;

  if (Dcm_Dsl_BufferUseValidForRequester(Requester, Use))
  {
    switch(Use)
    {
      case DCM_BUFFER_RX_NORMAL:
  #if (STD_ON == DCM_DSP_USE_ROESERVICES)
      case DCM_BUFFER_RX_ROE:
  #endif /* #if (STD_ON == DCM_DSP_USE_ROESERVICES) */
        BufferId = Dcm_RxPduIdConfig[Requester].BufferIndex;
        break;

      case DCM_BUFFER_RX_DEFAULT:
        BufferId = DCM_DEFAULT_BUFFER_ID_FROM_RX_CONNECTION_ID(Requester);
        break;

      case DCM_BUFFER_TX_NORMAL:
  #if (STD_ON == DCM_DSP_USE_ROESERVICES)
      case DCM_BUFFER_TX_ROE:
  #endif /* #if (STD_ON == DCM_DSP_USE_ROESERVICES) */
  #if (STD_ON == DCM_PERIODIC_USED)
      case DCM_BUFFER_TX_PERIODIC:
  #endif /* #if (STD_ON == DCM_PERIODIC_USED) */

        BufferId = Dcm_ProtocolConfig[DCM_PROTOCOL_INDEX_FROM_ID(Requester)].TxBufferIndex;
        break;

      case DCM_BUFFER_TX_NRC:
        BufferId = DCM_NRC_BUFFER_ID_FROM_TX_CONNECTION_ID(Requester);
        break;

        /* CHECK: NOPARSE */
      default:
        /* Unreachable default case mandated by MISRA */
        DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
        break;
        /* CHECK: PARSE */
    }
  }

  return BufferId;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.BufferManager.BufferAllocationPermission,1 */
STATIC FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_BufferAllocationPermission(
  Dcm_BufferUserType Requester,
  Dcm_BufferUsageType Use,
  Dcm_BufferSizeType Size,
  Dcm_BufferIdType BufferId
)
{
  BufReq_ReturnType AllocationPermission = BUFREQ_E_NOT_OK;

  /* First precondition is that the entity trying to assign the buffer actually
     has the right to do so.*/

  switch(Use)
  {
    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.AllocateNormalRx,1 */
    case DCM_BUFFER_RX_NORMAL:
#if (STD_ON == DCM_DSP_USE_ROESERVICES)
    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.AllocateRoeRx,1 */
    case DCM_BUFFER_RX_ROE:
#endif /* #if (STD_ON == DCM_DSP_USE_ROESERVICES) */
    /* Allocation is only successful if the buffer is:
       - free
       - greater or equal than the requested size. */

      if (Dcm_BufferConfig[BufferId].Size < Size)
      {
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.TpSduLengthTooLarge.RejectRx,1 */
        AllocationPermission = BUFREQ_E_OVFL;
      }
      else
      {
        if (DCM_BUFFER_IDLE != Dcm_BufferInfo[BufferId].State)
        {
          AllocationPermission = BUFREQ_E_BUSY;
        }
        else
        {
          AllocationPermission = BUFREQ_OK;
        }
      }

      break;

    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.AllocateDefaultRx,1 */
    case DCM_BUFFER_RX_DEFAULT:
      /* The buffer may be allocated only if:
         - the requested size is smaller than or equal to 2, the size of a DefaultBuffer.
         - the requested buffer is a default reception buffer. */
#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
      if (Dcm_BufferConfig[Dcm_RxPduIdConfig[Requester].BufferIndex].Size < Size)
#else
      if (DCM_BUFFER_DEFAULT_BUFFER_SIZE < Size)
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */
      {
        AllocationPermission = BUFREQ_E_OVFL;
      }
      else
      {
        AllocationPermission = BUFREQ_OK;
      }

      break;

    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.AllocateNormalTx,1 */
    case DCM_BUFFER_TX_NORMAL:
      /* The requested size is unknown for the moment,
         therefore the conditions are that the buffer is:
         - either:
           - free or
           - in use by the same user that tries to
             allocate it for transmission */

      if ((DCM_BUFFER_IDLE == Dcm_BufferInfo[BufferId].State) ||
          ((DCM_BUFFER_PROCESSING_NORMAL == Dcm_BufferInfo[BufferId].State) &&
           (Requester == Dcm_BufferInfo[BufferId].User)
          )
         )
      {
        AllocationPermission = BUFREQ_OK;
      }
      else
      {
        AllocationPermission = BUFREQ_E_BUSY;
      }

      break;

#if (STD_ON == DCM_DSP_USE_ROESERVICES)
    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.AllocateRoeTx,1 */
    case DCM_BUFFER_TX_ROE:

      if ((DCM_BUFFER_IDLE == Dcm_BufferInfo[BufferId].State) ||
          ((DCM_BUFFER_PROCESSING_ROE == Dcm_BufferInfo[BufferId].State) &&
           (Requester == Dcm_BufferInfo[BufferId].User)
          )
         )
      {
        AllocationPermission = BUFREQ_OK;
      }
      else
      {
        AllocationPermission = BUFREQ_E_BUSY;
      }

      break;
#endif /* #if (STD_ON == DCM_DSP_USE_ROESERVICES) */

#if (STD_ON == DCM_PERIODIC_USED)
    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.AllocatePeriodicTx,1 */
    case DCM_BUFFER_TX_PERIODIC:
      /* The buffer has to be:
         - free */

      if (DCM_BUFFER_IDLE == Dcm_BufferInfo[BufferId].State)
      {
        AllocationPermission = BUFREQ_OK;
      }
      else
      {
        AllocationPermission = BUFREQ_E_BUSY;
      }

      break;
#endif /* #if (STD_ON == DCM_PERIODIC_USED) */

    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.AllocateNrcTx,2 */
    case DCM_BUFFER_TX_NRC:

      if (DCM_BUFFER_NRC_BUFFER_SIZE < Size)
      {
        AllocationPermission = BUFREQ_E_OVFL;
      }
      else
      {
        AllocationPermission = BUFREQ_OK;
      }

      break;
      /* CHECK: NOPARSE */
    default:
      /* Unreachable default case mandated by MISRA */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
      /* CHECK: PARSE */
  }

  return AllocationPermission;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.State,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Owner,2 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Free,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.BufferManager.BufferInitializeSingleBuffer,1 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_BufferInitializeSingleBuffer(
  Dcm_BufferUserType Requester,
  Dcm_BufferIdType BufferId
)
{
  BufReq_ReturnType FreeResult = BUFREQ_E_NOT_OK;

  /* Only the current user of a buffer may free it */
  if (DCM_BUFFER_IS_NORMAL_BUFFER(BufferId))
  {
    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.FreeAllowed,1 */
    if (Requester == Dcm_BufferInfo[BufferId].User)
    {
      Dcm_BufferInfo[BufferId].State = DCM_BUFFER_IDLE;
      Dcm_BufferInfo[BufferId].User = DCM_INVALID_USER_ID;
      FreeResult = BUFREQ_OK;
    }
    else
    {
      /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.FreeNotAllowed,1 */
      FreeResult = BUFREQ_E_NOT_OK;
    }
  }
  else
  {
    /* Since other buffer types don't have current owners, freeing them is
       always successful. */
    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.FreeAllowed,1 */
      FreeResult = BUFREQ_OK;
  }

  return FreeResult;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.BufferManager.BufferInitSingleBuffer,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_BufferInitSingleBuffer(Dcm_BufferIndexType BufferIndex)
{
  Dcm_BufferInfo[BufferIndex].User = DCM_INVALID_USER_ID;
  Dcm_BufferInfo[BufferIndex].State = DCM_BUFFER_IDLE;

  /* clear the contents of the buffer */
  TS_MemSet(Dcm_BufferConfig[BufferIndex].BufferPtr, 0, Dcm_BufferConfig[BufferIndex].Size);
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[end of file]================================================================*/
