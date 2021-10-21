/**
 * \file
 *
 * \brief AUTOSAR NvM
 *
 * This file contains the implementation of the AUTOSAR
 * module NvM.
 *
 * \version 6.17.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#if (defined NVM_INTERNAL_USE)
#error NVM_INTERNAL_USE is already defined
#endif
/* prevent redundant declarations of RTE types */
#define NVM_INTERNAL_USE

#include <SchM_NvM.h>             /* module specific header for             */
                                  /* Schedule Manager services              */

#include <NvM_Trace.h>
#include <TSAutosar.h>            /* EB specific standard types             */

/* !LINKSTO SWS_NvM_00554,2 */
#include <NvM.h>                  /* Extern NVRAM Manager API definitions   */
#include <NvM_DataIntern.h>       /* NvM data which is used only            */
                                  /* within the NvM module.                 */
#include <NvM_FunctionIntern.h>   /* NvM functions which are used           */
                                  /* only within the NvM module.            */
#include <NvM_Queue.h>            /* Extern definitions of unit queue.      */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/** \brief ******************  variables normal uint8 buffer section ****************/

#define NVM_START_SEC_VAR_CLEARED_16
#include <NvM_MemMap.h>

VAR(uint16, NVM_VAR_CLEARED) NvM_Queue_IndexStandard;
VAR(uint16, NVM_VAR_CLEARED) NvM_Queue_CancelledStandardJobs;

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) )
VAR(uint16, NVM_VAR_CLEARED) NvM_Queue_IndexImmediate;
VAR(uint16, NVM_VAR_CLEARED) NvM_Queue_CountImmediate;
VAR(uint16, NVM_VAR_CLEARED) NvM_Queue_CancelledImmediateJobs;
#endif

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#define NVM_STOP_SEC_VAR_CLEARED_16
#include <NvM_MemMap.h>

#define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <NvM_MemMap.h>

VAR( NvM_QueueItemType, NVM_VAR ) NvM_Queue_Standard[NVM_SIZE_STANDARD_JOB_QUEUE];

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) )
VAR( NvM_QueueItemType, NVM_VAR ) NvM_Queue_Immediate[NVM_SIZE_IMMEDIATE_JOB_QUEUE];
#endif

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <NvM_MemMap.h>

/** \brief ****************  end of variables normal uint8 buffer section ***********/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

FUNC(uint8, NVM_CODE) Nvm_Queue_CheckEmpty(void)
{
  uint8 Result = NVM_QUEUE_REQUEST_QUEUED; /* Result of the function */

#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) )
  /* In case both queues are configured both queues must be empty. */
  if (NVM_STD_QUEUE_EMPTY())
  {
#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)
    if(NVM_IMMEDIATE_QUEUE_EMPTY())
#endif
    {
      /* queue is empty */
      Result = NVM_QUEUE_IS_EMPTY;
    }
  }
#else
  if (NVM_STD_QUEUE_EMPTY())
  {
    /* queue is empty */
    Result = NVM_QUEUE_IS_EMPTY;
  }
#endif

  return Result;
}/* end of function Nvm_Queue_CheckEmpty */

FUNC(Std_ReturnType, NVM_CODE) NvM_Queue_CheckFullStandard
(
  NvM_BlockIdType BlockId
)
{
  Std_ReturnType Result = E_NOT_OK; /* Result of the function */

  /* One slot in the queue must be reserved for the multiblock requests.
   * Hence, free space check in the queue depends on whether a multiblock request
   * is already queued or not. Therefore, as long as a multiblock request is
   * not already queued, there shall be atleast 2 free spaces in the queue for
   * a single block request to be queued.

   * If multiblock request is already queued, only one free space is required for
   * the single block request to be queued.
   */

   /* Note:
    * It is ensured that queue has a size of atleast 2.
    * Therefore, queue has 2 free spaces either when the queue is empty OR
    * if the Queue Index is less than Queue Size - 2)
    */

  if (NVM_STD_QUEUE_EMPTY())
  {
    Result = E_OK;
  }
#if ( NVM_SIZE_STANDARD_JOB_QUEUE > 2U )
  else if (NVM_STD_QUEUE_NOT_FULL())
  {
    Result = E_OK;
  }
#endif
  else
  {
    /* Either the queue is full or slot for a multi block request is free
     * - Queue Index is less than (Queue Size - 1)
     */
    if ( NVM_STD_QUEUE_NOT_FULL_MULTI() )
    {
      if ( BlockId == NVM_BLOCK_MULTI )
      {
        /* In case of a multiblock request, the request can be queued
         * at the slot reserved for multiblock requests.
         */
        Result = E_OK;
      }
      else
      {
        /* This is a single block request */
        if ( NvM_AdminBlockTable[NVM_BLOCK_MULTI].NvMResult == NVM_REQ_PENDING )
        {
          /* A multi block request is already queued.
           * So this single block request can be queued.
           */
          Result = E_OK;
        } /* if ( NvM_Queue_Standard[0U].NvMBlockDescriptorIndex == NVM_BLOCK_MULTI ) */
      } /* if ( BlockId == NVM_BLOCK_MULTI ) */
    } /* if ( NVM_STD_QUEUE_NOT_FULL_MULTI() ) */
  } /* if  ( ( NVM_STD_QUEUE_EMPTY() ) || ( NVM_STD_QUEUE_NOT_FULL() ) ) */

  return Result;

}/* end of function NvM_Queue_CheckFullStandard  */

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

#if ( NVM_JOB_PRIORITIZATION == STD_ON )
FUNC(Std_ReturnType, NVM_CODE) NvM_Queue_InsertImmediate
(
  uint16                        BlockIndex,
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) DataAddress,
  NvM_CurrentFunctionPtrType             NvMRequestAsyncPtr
)
{
  Std_ReturnType Result = E_NOT_OK;  /* Result of this function. */
  uint16 Index = 0U;

#if ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U )

  if ( NvM_BlockDescriptorTable[BlockIndex].blockJobPriority == NVM_BLOCK_IMMEDIATE_PRIORITY )
  {
    /* Check if a service for block BlockId is already pending or if the
     * queue is full. In both cases the request can not be inserted.
     */
#if ( NVM_SIZE_IMMEDIATE_JOB_QUEUE > 0 )
    if ( NVM_IMMEDIATE_QUEUE_NOT_FULL() )
    {
      NvM_AdminBlockTable[BlockIndex].NvMResult = NVM_REQ_PENDING;

/* Index is already 0 in case NVM_SIZE_IMMEDIATE_JOB_QUEUE is 1 */
#if ( NVM_SIZE_IMMEDIATE_JOB_QUEUE > 1 )
      /* Calculate the position in Immediate Queue
       * where the new request should be placed
       */
      Index = (NvM_Queue_IndexImmediate + NvM_Queue_CountImmediate) - NvM_Queue_CancelledImmediateJobs;
      /* Wrap around */
      Index %= NVM_SIZE_IMMEDIATE_JOB_QUEUE;
#endif

      /* insert API function parameters into the Immediate queue. */
      NvM_Queue_Immediate[Index].NvMBlockDescriptorIndex = BlockIndex;
      NvM_Queue_Immediate[Index].NvMRequestAsyncPtr = NvMRequestAsyncPtr;
      NvM_Queue_Immediate[Index].NvMTempRamBlockDataAddress = DataAddress;

      /* Advance Immediate Queue Count by 1 */
      NVM_IMMEDIATE_QUEUE_UP();

      Result = E_OK;
    }/* if ( ( NvM_AdminBlockTable[BlockId].NvMResult!= NVM_REQ_PENDING ) */
#endif

  }
  else
#endif /* #if ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) */
  {
    /* The immediate queue is requested but the block has only
     * standard priority. --> Insert block into Standard Queue.
     */
    Result = NvM_Queue_InsertStandard( BlockIndex, DataAddress, NvMRequestAsyncPtr );

  }/* if ( NvM_BlockDescriptorTable[BlockId].blockJobPriority */

#if ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS == 0U )
  TS_PARAM_UNUSED(Index);
#endif
  return Result;

}/* end of function NvM_Queue_InsertImmediate */
#endif /* #if ( NVM_JOB_PRIORITIZATION == STD_ON ) */

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

FUNC(Std_ReturnType, NVM_CODE) NvM_Queue_InsertStandard
(
  uint16                                 BlockIndex,
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) DataAddress,
  NvM_CurrentFunctionPtrType             NvMRequestAsyncPtr
)
{
  Std_ReturnType Result;   /*  Result of this function.  */

  uint16 Index;             /*  Index in the standard queue  */

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

  boolean Found = FALSE;   /*  Flag to indicate if a slot is found  */

#if ( NVM_JOB_PRIORITIZATION == STD_ON )
  uint8 Priority = 0U;     /*  Current block's standard priority  */
#endif
  uint16 CurBlockId;       /*  Current block's Block Identifer  */

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

  /* Check if the queue is full. If so, the request can not be inserted. */
  Result = NvM_Queue_CheckFullStandard(BlockIndex);

  if (E_OK == Result)
  {
    /* Set the current block's result to pending */
    NvM_AdminBlockTable[BlockIndex].NvMResult = NVM_REQ_PENDING;

    /* Increment current value of Standard Queue Index.
     * In case of uninitialized Index, set it to 0.
     */
    NVM_STANDARD_QUEUE_UP();

    /* Start from the value of Queue Index and search inwards */
    Index = NvM_Queue_IndexStandard - NvM_Queue_CancelledStandardJobs;

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

#if ( NVM_JOB_PRIORITIZATION == STD_ON )

    /* Fetch block's priority; Priority is set to 0 for Block 0 (multiblock requests) */
    Priority = NvM_BlockDescriptorTable[BlockIndex].blockJobPriority;

#endif

    /*
     * Queuing :

     * A multi-block job (BlockId=0) is queued always at the bottom
     * (NvM_Queue_IndexStandard=0) irrespective of whether job  prioritisation is
     * enabled or not so that it get executed only after all other jobs get
     * executed.

     * Irrespective of job prioritisation and irrespective of whether multi-block
     * job or single block job, an ongoing job shall always keep the top index of
     * the queue. The top index of the queue is being shared by NvM state machine
     * and hence always point to the job which is being currently executed.
     * And, single blocks jobs which are queued while executing multi-block job needs
     * to be executed only after job of the multi-block gets finished.

     * Irrespective of job prioritisation, multi-block job shall stay at the bottom
     * (NvM_Queue_IndexStandard=0)if it is not currently being executed and the new
     * single block job shall be queued in front of the multi-block job.

     * The queue order of single block jobs shall be based on whether job
     * prioritisation is enabled or not.
     * If job prioritisation is not enabled, it will be queued in FCFS order.
     * If job prioritisation is enabled, high priority jobs (having less value of
     * 'blockJobPriority') shall be queued in front of the low priority jobs
     * (having higher value of 'blockJobPriority').
     */

    while ( ( Found != TRUE ) && ( Index != 0U ) )
    {

      /* Get the block number of current request in queue */
      CurBlockId = NvM_Queue_Standard[Index - 1U].NvMBlockDescriptorIndex;

      if (
           /* Check if the new request is a multi-block request */
           ( BlockIndex == 0U ) ||
           /* Check if the request in the queue is being serviced */
           ( CurBlockId == NvM_GlobalBlockNumber ) ||
           /* Check if the request in the queue is a multi-block request
            * and is being processed. NvM_GlobalBlockNumber keep change during
            * job of the multi-block request, hence this check is required.
            */
           (
             ( CurBlockId == 0U ) &&
             (
               (( NVM_READ_ALL_API_ID == NvM_CurrentServiceId ) ||
                ( NVM_WRITE_ALL_API_ID == NvM_CurrentServiceId )) ||
                ( NVM_VALIDATE_ALL_API_ID == NvM_CurrentServiceId )
             )
           )
         )
      {
         /*  Move job in queue index to forward position. */
         NvM_Queue_Standard[Index] = NvM_Queue_Standard[Index - 1U];
         /* Compare with the next request in the queue */
         Index--;
      }
      else
      {

        /* Check if the request in the queue is a multi-block request
         * In such a case, the new single block request is placed in front it.
         */
        if (CurBlockId == 0U)
        {
          /* Stop the search */
          Found = TRUE;
        }
#if ( NVM_JOB_PRIORITIZATION == STD_ON )

        /* Priority of jobs shall be evaluated when job prioritisation is
         * enabled. Otherwise jobs shall be processed in serialized order.
         * If the request in the queue is not a multi-block request then
         * new single block request is placed in front of low priority
         * jobs in the queue.
         */
        else if (Priority < NvM_BlockDescriptorTable[CurBlockId].blockJobPriority)
        {
          /* Stop the search */
          Found = TRUE;
        }
 #endif   /* ( NVM_JOB_PRIORITIZATION == STD_ON ) */
        else
        {
          /*  Move job in queue index to forward position. */
          NvM_Queue_Standard[Index] = NvM_Queue_Standard[Index - 1U];
          /* Compare with the next request in the queue */
          Index--;
        }
      }
    }/* while ( ( Found != TRUE ) && ( Index != 0U ) ) */

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

    /* Insert the new request in the queue at the found position */
    NvM_Queue_Standard[Index].NvMBlockDescriptorIndex = BlockIndex;
    NvM_Queue_Standard[Index].NvMRequestAsyncPtr = NvMRequestAsyncPtr;
    NvM_Queue_Standard[Index].NvMTempRamBlockDataAddress = DataAddress;

    Result = E_OK;
  }/* if ( ( NvM_AdminBlockTable[BlockId].NvMResult != NVM_REQ_PENDING ) */

  return Result;

}/* end of function NvM_Queue_InsertStandard */

FUNC(void, NVM_CODE) NvM_Queue_RemoveFirstItem(void)
{
  /* Protect concurrent access to queue */
  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) )

  /* Check if the current service is queued in the Immediate Queue */
  if ( ( NVM_IMMEDIATE_QUEUE_BLOCKID == NvM_GlobalBlockNumber ) &&
       ( NVM_IMMEDIATE_QUEUE_NOT_EMPTY() )
     )
  {
    /* The current job must be removed from the immediate queue.
     * Increase current value of immediate Queue Front circularly.
     */
    NVM_IMMEDIATE_QUEUE_DOWN();
  } /* end of if ( ( NVM_IMMEDIATE_QUEUE_NOT_EMPTY() )*/
  else
#endif /* #if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) &&
                ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) ) */
  {
    /* The current job must be removed from the standard queue.
     * Decrement current value of standard Queue Index.
     * In case the value is 0, set it to uninitialized (0xFF).
     */
    NVM_STANDARD_QUEUE_DOWN();
  }
  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

}/* end of function NvM_Queue_RemoveFirstItem */


#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)
#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) )
FUNC(void, NVM_CODE) NvM_Queue_UpdateMultiblockQueueItem(void)
{
  /* Protect concurrent access to standard queue */
  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

  if (NVM_READ_ALL_API_ID == NvM_CurrentServiceId)
  {
    /* Check if NvM_ReadAll() was interrupted while processing the Configuration ID block */
    if (NVM_BLOCK_CONFIG_ID == NvM_GlobalBlockNumber)
    {
      NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMRequestAsyncPtr = &NvM_ReadAll_Async_State0;

      NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMBlockDescriptorIndex = NVM_BLOCK_CONFIG_ID;
    }
    else
    {
      NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMRequestAsyncPtr = &NvM_ReadAll_Async_State5;

      NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMBlockDescriptorIndex = NvM_GlobalBlockNumber;
    }
  }
  else if (NVM_WRITE_ALL_API_ID == NvM_CurrentServiceId)
  {
    NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMRequestAsyncPtr = &NvM_WriteAll_Async_State1;

    NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMBlockDescriptorIndex = NvM_GlobalBlockNumber;
  }
#if (NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS > 0)
  else if (NVM_FIRST_INIT_ALL_API_ID == NvM_CurrentServiceId)
  {
    NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMRequestAsyncPtr = &NvM_FirstInitAll_Async_State1;

    NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMBlockDescriptorIndex = NvM_GlobalBlockNumber;
  }
#endif
  else
  {
    /* NVM_VALIDATE_ALL_API_ID */
    NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMRequestAsyncPtr = &NvM_ValidateAll_Async_State0;

    NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMBlockDescriptorIndex = NvM_GlobalBlockNumber;
  }

  /* The multi-block request was interrupted by an immediate write job, so the
   * Multi Request Keep Job Pending and Block request Canceled flags need to be cleared.
   * When the multi-block request is resumed, the flags will be set accordingly.
   */
  if ( (!NVM_CHECK_GLOBAL_GENERIC_STATUS(NVM_GENERIC_CANCEL_MASK)) &&
       (NVM_CHECK_GLOBAL_GENERIC_STATUS( NVM_GENERIC_MRKJP_MASK ))
     )
  {
    DBG_NVM_GLOBALGENERICSTATUS((NvM_GlobalGenericStatus),
                (NvM_GlobalGenericStatus & (uint16)((uint16)(~(NVM_GENERIC_MRKJP_MASK)) & 0xFFFFU)));
    NVM_CLEAR_GLOBAL_GENERIC_STATUS( NVM_GENERIC_MRKJP_MASK );
  }
  else if (NVM_CHECK_GLOBAL_GENERIC_STATUS(NVM_GENERIC_CANCEL_MASK))
  {
    DBG_NVM_GLOBALGENERICSTATUS((NvM_GlobalGenericStatus),
                (NvM_GlobalGenericStatus & (uint16)((uint16)(~(NVM_GENERIC_CANCEL_MASK)) & 0xFFFFU)));
    NVM_CLEAR_GLOBAL_GENERIC_STATUS(NVM_GENERIC_CANCEL_MASK);
  }
  else
  {
    /* Nothing to do (MISRA) */
  }

  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
}
#endif /* (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */
#endif /* #if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) &&
                ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) ) */

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[end of file]===========================================*/
