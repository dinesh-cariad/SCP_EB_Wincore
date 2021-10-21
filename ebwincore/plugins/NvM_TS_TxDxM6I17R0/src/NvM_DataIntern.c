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

/*  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 *    "A conversion should not be performed from pointer to void into pointer to object."
 *
 *    Reason: The portion of code is necessary and it has no alignment problems.
 *
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason: conditions depends on configuration, "false" case is covered by other tests
 *
 */
/*==================[inclusions]============================================*/

#if (defined NVM_INTERNAL_USE)
#error NVM_INTERNAL_USE already defined
#endif
/* prevent redundant declarations of RTE types */
#define NVM_INTERNAL_USE

#include <SchM_NvM.h>             /* module specific header for             */
                                  /* Schedule Manager services              */

#include <NvM_Trace.h>
#include <TSAutosar.h>            /* EB specific standard types             */
#include <TSMem.h>                /* Autosar Module Base */


/* !LINKSTO SWS_NvM_00554,2 */
#include <MemIf.h>                /* Contains extern definition of function */
                                  /* MemIf_Cancel().                        */

/* !LINKSTO SWS_NvM_00554,2 */
#include <NvM.h>                  /* Extern NVRAM Manager API definitions   */
#include <NvM_FunctionIntern.h>   /* NvM functions which are used           */
                                  /* only within the NvM module.            */
#include <NvM_DataIntern.h>       /* NvM data which is used only            */
                                  /* within the NvM module.                 */
#include <NvM_StateMachine.h>     /* External defintions for unit           */
                                  /* StateMachine                           */
#include <NvM_Queue.h>            /* Extern definitions of unit queue.      */
#include <NvM_CalcCrc.h>          /* Extern definitions of unit CalcCrc.    */

/* if any DEM event is switched on, include Dem.h */
#if (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DEM)        \
    ||                                                                  \
    (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DEM)
  /* !LINKSTO SWS_NvM_00554,2 */
#include <Dem.h>
#endif

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
/* !LINKSTO NVM556,1 */
#include <Det.h>               /* API of module Det. */
#endif

/* if single block job status reporting is switched on, include BswM_NvM.h */
#if (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON)
#include <BswM_NvM.h>
#endif
/*==================[macros]================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

/** \brief Function to be called when a fatal error is detected.
 **
 ** Currently this function does not handle errors, the function block
 ** is empty.
 **/
STATIC FUNC(void,NVM_CODE) NvM_FatalError(void);

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

#if (NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS > 0U)
#if ( defined(NVM_MULTICORE_ENABLED) )
#define NVM_START_SEC_MC_SHARED_VAR_POWER_ON_INIT_UNSPECIFIED
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_ADMINBLOCK_UNSPECIFIED
#include <NvM_MemMap.h>
#endif

/** \brief Stores the status of the NvM blocks.
 **/
/* !LINKSTO NVM134,1, NVM138,1, NVM135,1 */
VAR (NvM_AdminBlockType,NVM_VAR_POWER_ON_INIT) NvM_AdminBlockTable[NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS] = {{0U,0U,0U,0U,0U,0U}};

#if ( defined(NVM_MULTICORE_ENABLED) )
#define NVM_STOP_SEC_MC_SHARED_VAR_POWER_ON_INIT_UNSPECIFIED
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_ADMINBLOCK_UNSPECIFIED
#include <NvM_MemMap.h>
#endif /*MULTICORE ENABLED*/
#endif

/** \brief *************************  variables normal uint8 section ****************/


#define NVM_START_SEC_VAR_INIT_8
#include <NvM_MemMap.h>

VAR( uint8,NVM_APPL_DATA ) NvM_ConfigurationId[NVM_CONFIGURATIONID_PLUSCRC_SIZE] = {0U};

VAR( uint8,NVM_VAR ) NvM_GlobalErrorStatus = 0;

VAR( uint8,NVM_VAR ) NvM_GlobalCallLevel = 0;


#if (NVM_NUMBER_OF_WRITE_RETRY_BLOCKS > 0U)
VAR( uint8,NVM_VAR ) NvM_WriteRetryCount = 0U;
#endif

#if (NVM_NUMBER_OF_READ_RETRY_BLOCKS > 0U)
VAR( uint8,NVM_VAR ) NvM_ReadRetryCount = 0U;
#endif

#define NVM_STOP_SEC_VAR_INIT_8
#include <NvM_MemMap.h>

/** \brief *************************  variables normal uint16 section ****************/
#if ( defined(NVM_MULTICORE_ENABLED) )
#define NVM_START_SEC_MC_SHARED_VAR_INIT_16
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_VAR_INIT_16
#include <NvM_MemMap.h>
#endif

VAR( uint16,NVM_VAR ) NvM_GlobalGenericStatus = 0U;

#if ( defined(NVM_MULTICORE_ENABLED) )
#define NVM_STOP_SEC_MC_SHARED_VAR_INIT_16
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_VAR_INIT_16
#include <NvM_MemMap.h>
#endif /*MULTICORE ENABLED*/
/** \brief *************************  uninitialized variables uint8 section ****************/
#define NVM_START_SEC_VAR_CLEARED_8
#include <NvM_MemMap.h>

VAR( uint8,NVM_VAR_CLEARED ) NvM_CurrentServiceId;

#if (NVM_SYNC_MECHANISM == STD_ON)
VAR(uint8, NVM_VAR_CLEARED) NvM_MirrorRetryCount;
#endif

#if (NVM_CRYPTO_HOOKS == STD_ON)
VAR( uint8,NVM_VAR_CLEARED ) NvM_PostCryptoState;
#endif
#define NVM_STOP_SEC_VAR_CLEARED_8
#include <NvM_MemMap.h>

/** \brief *************************  initialized variables uint16 section ****************/
#if ( defined(NVM_MULTICORE_ENABLED) )
#define NVM_START_SEC_MC_SHARED_VAR_INIT_16
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_VAR_INIT_16
#include <NvM_MemMap.h>
#endif

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
VAR (uint16,NVM_VAR) NvM_CrcQueue[NVM_NUMBER_OF_CRC_BLOCKS] = {0U};
#endif

#if ( defined(NVM_MULTICORE_ENABLED) )
#define NVM_STOP_SEC_MC_SHARED_VAR_INIT_16
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_VAR_INIT_16
#include <NvM_MemMap.h>
#endif /*MULTICORE ENABLED*/

/** \brief *************************  uninitialized variables uint16 section ****************/
#if ( defined(NVM_MULTICORE_ENABLED) )
#define NVM_START_SEC_MC_SHARED_VAR_CLEARED_16
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_VAR_CLEARED_16
#include <NvM_MemMap.h>
#endif

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
VAR( uint16, NVM_VAR_CLEARED ) NvM_RamBlockCrcCalcCount;
#endif

#if ( defined(NVM_MULTICORE_ENABLED) )
#define NVM_STOP_SEC_MC_SHARED_VAR_CLEARED_16
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_VAR_CLEARED_16
#include <NvM_MemMap.h>
#endif /*MULTICORE ENABLED*/

/** \brief *************************  uninitialized variables unspecified section ****************/
#define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <NvM_MemMap.h>

VAR( NvM_DynamicStatusType,NVM_VAR_CLEARED ) NvM_GlobalWorkingStatus;

#define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <NvM_MemMap.h>

/** \brief ********************  variables user application data section ************/
#define NVM_START_SEC_VAR_APPL_DATA
#include <NvM_MemMap.h>

P2VAR(uint8, NVM_VAR_CLEARED, NVM_APPL_DATA) NvM_TempRamBlockAddress = NULL_PTR;

#if (NVM_INTERNAL_BUFFER_SIZE != 0U)
VAR(uint8, NVM_APPL_DATA) NvM_InternalBuffer[NVM_INTERNAL_BUFFER_SIZE] = {0U};
#endif

#define NVM_STOP_SEC_VAR_APPL_DATA
#include <NvM_MemMap.h>

#if ((NVM_WRITE_VERIFICATION == STD_ON) ||(NVM_PRE_WRITE_DATA_COMP == STD_ON) || (NVM_BLOCK_CHECK_ENABLED == STD_ON))
#define NVM_START_SEC_VAR_INIT_8
#include <NvM_MemMap.h>
/**  \brief Internal buffer used for intermediate read operations for pre and post write to NV memory.
 **         Internal buffer will be used in case of BlockCheck RedCopies compare to hold the data
 **         from the second redundant copy.
 **/
VAR( uint8, NVM_VAR ) NvM_IntermediateReadBuffer[NVM_WRITE_VERIFICATION_BUFFER_SIZE] = {0U};

#define NVM_STOP_SEC_VAR_INIT_8
#include <NvM_MemMap.h>
#endif/*(NVM_WRITE_VERIFICATION == STD_ON) ||(NVM_PRE_WRITE_DATA_COMP == STD_ON) || (NVM_BLOCK_CHECK_ENABLED == STD_ON)*/

/** \brief **********************  variables normal uint16 section ***************/
#define NVM_START_SEC_VAR_INIT_16
#include <NvM_MemMap.h>

VAR( uint16,NVM_VAR ) NvM_GlobalBlockNumber = 0U;

#define NVM_STOP_SEC_VAR_INIT_16
#include <NvM_MemMap.h>

/** \brief *************************  variables normal unspecified section **********/
#define NVM_START_SEC_VAR_INIT_UNSPECIFIED
#include <NvM_MemMap.h>

P2CONST( NvM_BlockDescriptorType,NVM_VAR,NVM_CONST )
  NvM_CurrentBlockDescriptorPtr = NULL_PTR;

VAR( NvM_CurrentFunctionPtrType,NVM_VAR ) NvM_CurrentFunctionPtr[6] =
  {NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR};

#define NVM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <NvM_MemMap.h>

/*==================[internal data]==========================================*/

#define NVM_START_SEC_VAR_INIT_8
#include <NvM_MemMap.h>

#if (NVM_CRYPTO_HOOKS == STD_ON)
/**  \brief Tells whether the Cripto function is called for the first time or not
 **         in a sequence of encryption calls. (Data encryption may be done in multiple steps)
 **/
STATIC VAR( boolean, NVM_VAR ) FirstCryptoExecution = TRUE;
#endif

#define NVM_STOP_SEC_VAR_INIT_8
#include <NvM_MemMap.h>

#define NVM_START_SEC_VAR_INIT_16
#include <NvM_MemMap.h>

#if (NVM_CRYPTO_HOOKS == STD_ON)
/**  \brief Encryption timeout that is incremented with each encryption call for a
 **         certain block data. The encryption is considered as failed if this variable
 **         reaches the maximum value.
 **/
STATIC VAR( uint16, NVM_VAR ) CryptoTimeoutCounter = 0U;
#endif

#define NVM_STOP_SEC_VAR_INIT_16
#include <NvM_MemMap.h>

/*==================[external function definitions]==========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

#if ( NVM_BLOCK_INDEX_SEARCH == STD_ON )
FUNC(uint16, NVM_CODE) NvM_SearchConfigTable
(
  const NvM_BlockIdType BlockId
)
{
    uint16 localReturnValue = NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS;
    uint16 localStartIndex;
    uint16 localMiddleIndex;
    uint16 localEndIndex;
    boolean localExitCondition = (boolean)FALSE;

    /* set index values for start of search (Divide et Impera algorithm) */
    localStartIndex = (uint16)0;
    localEndIndex = (uint16)(NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS - (uint16)1);
    localMiddleIndex = (uint16)(localEndIndex / (uint16)2);
    /* iterate through the configured block IDs to search for the received one */
    do
    {
      /* if the currently checked block ID is the same as the received parameter */
      if(NvM_BlockDescriptorTable[localMiddleIndex].nvBlockIdentifier == BlockId)
      {
        /* set the pointer that is the returned value */
        localReturnValue = localMiddleIndex;
        /* indicate that the while loop can exit */
        localExitCondition = (boolean)TRUE;
      }
      else
      {
        /* if the two margin indexes are no more than 1 appart */
        if(((uint16)(localEndIndex - localStartIndex)) <= (uint16)1)
        {
          /* set the exit condition to TRUE - the 2 remaining checks are done below */
          localExitCondition = (boolean)TRUE;
          /* check the ID for the End index */
          if(NvM_BlockDescriptorTable[localEndIndex].nvBlockIdentifier == BlockId)
          {
            /* set the pointer that is the returned value */
            localReturnValue = localEndIndex;
          }
          else
          {
            /* nothing to do here - block ID is not among the configured ones */
          }
        }
        else
        {
          /* if the currently checked block ID is smaller than the received parameter */
          if(NvM_BlockDescriptorTable[localMiddleIndex].nvBlockIdentifier < BlockId)
          {
            /* update start index so that the search is conducted in the upper half */
            localStartIndex = localMiddleIndex + (uint16)1;
          }
          else
          {
            /* update end index so that the search is conducted in the lower half */
            localEndIndex = (uint16)(localMiddleIndex - (uint16)1);
          }
          /* determine next middle index value for block ID comparison - searching must continue */
          localMiddleIndex = (uint16)(localEndIndex + localStartIndex);
          localMiddleIndex /= (uint16)2;
        }
      }
    /* end of "while" */
    }while(localExitCondition == (boolean)FALSE);

    return localReturnValue;
}
#endif

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
FUNC(uint16, NVM_CODE) NvM_GetBlockIndexForValidId
(
  const NvM_BlockIdType BlockId
)
{
  uint16 RetBlockIndex;
#if ( NVM_USE_REDIR_BLOCK_ARRAY == STD_ON )
  if(( BlockId >= 1U ) && ( BlockId <= NVM_BLOCK_ID_MAX ))
  {
    RetBlockIndex = NvM_RedirBlockId[BlockId];
  }
  else
  {
    RetBlockIndex = NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS;
  }
#else /* ( NVM_USE_REDIR_BLOCK_ARRAY == STD_ON ) */
#if ( NVM_BLOCK_INDEX_SEARCH == STD_ON )
  RetBlockIndex = NvM_SearchConfigTable(BlockId);
#else /* ( NVM_BLOCK_INDEX_SEARCH == STD_ON ) */
  if(( BlockId >= 1U ) && ( BlockId <= NVM_BLOCK_ID_MAX ))
  {
    RetBlockIndex = (uint16)BlockId;
  }
  else
  {
    RetBlockIndex = NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS;
  }
#endif /* ( NVM_BLOCK_INDEX_SEARCH == STD_ON ) */
#endif /* ( NVM_USE_REDIR_BLOCK_ARRAY == STD_ON ) */

  return RetBlockIndex;
}
#endif

FUNC(void,NVM_CODE) NvM_SetPermanentRamBlockInvalid(void)
{
  /* Check if the permanent RAM Block must be used by this service */
#if (NVM_SYNC_MECHANISM == STD_ON)
  if ( ( NVM_CHECK_WORKING_STATUS(NVM_DYN_UPRB_MASK) ) ||
       ( NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK) )
     )
#else
  if ( NVM_CHECK_WORKING_STATUS(NVM_DYN_UPRB_MASK) )
#endif /* NVM_SYNC_MECHANISM == STD_ON */
  {
    /* data must be copied to permanent RAM block -->  */
    /* set permanent RAM block invalid                 */
    NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMValid = NVM_BLOCK_INVALID;
  }
}

FUNC(void,NVM_CODE) NvM_SetPermanentRamBlockValid(void)
{
  /* Check if the permanent RAM Block must be used by this service */
#if (NVM_SYNC_MECHANISM == STD_ON)
  if ( ( NVM_CHECK_WORKING_STATUS(NVM_DYN_UPRB_MASK) ) ||
       ( NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK) )
     )
#else
  if ( NVM_CHECK_WORKING_STATUS(NVM_DYN_UPRB_MASK) )
#endif /* NVM_SYNC_MECHANISM == STD_ON */
  {
    /* data must be copied to permanent RAM block -->  */
    /* set permanent RAM block valid                   */
    NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMValid = NVM_BLOCK_VALID;
  }
}

FUNC(void,NVM_CODE) NvM_SetPermanentRamBlockUnchanged(void)
{
  /* Check if the permanent RAM Block must be used by this service */
#if (NVM_SYNC_MECHANISM == STD_ON)
  if ( ( NVM_CHECK_WORKING_STATUS(NVM_DYN_UPRB_MASK) ) ||
       ( NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK) )
     )
#else
  if ( NVM_CHECK_WORKING_STATUS(NVM_DYN_UPRB_MASK) )
#endif /* NVM_SYNC_MECHANISM == STD_ON */
  {
    /* Permanent RAM Block must be used.                          */
    /* Set block unchanged --> it's NOT written by NvM_WriteAll() */
    NVM_CLEAR_WORKING_STATUS(NVM_DYN_CHANGE_MASK);
  }
}

FUNC(void,NVM_CODE) NvM_SetPermanentRamBlockChanged(void)
{
  /* Check if the permanent RAM Block must be used by this service */
#if (NVM_SYNC_MECHANISM == STD_ON)
  if ( ( NVM_CHECK_WORKING_STATUS(NVM_DYN_UPRB_MASK) ) ||
       ( NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK) )
     )
#else
  if ( NVM_CHECK_WORKING_STATUS(NVM_DYN_UPRB_MASK) )
#endif /* NVM_SYNC_MECHANISM == STD_ON */
  {
    /* set block changed --> it's written by NvM_WriteAll() */
    NVM_SET_WORKING_STATUS(NVM_DYN_CHANGE_MASK);
  }
}/* end of function NvM_SetPermanentRamBlockChanged */

#if ( defined(NVM_MULTICORE_ENABLED) )

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>

FUNC(boolean, NVM_CODE) NvM_CheckSetMcFlag(uint16 AdminBlockIndex)
{
  boolean Result = TRUE;/*We set it to TRUE initially because we expect during runtime
  99.99% of the time will return TRUE so we can spare an assignment in exclusive area.*/
/* !LINKSTO NvM.MultiCore.LockMechanism,1 */
  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
  if(((uint8)(NvM_AdminBlockTable[AdminBlockIndex].NvMExtendedStatus & (NVM_ES_MULTICORE_LOCK_MASK)) == (NVM_ES_MULTICORE_LOCK_MASK)))
  {
    Result = FALSE;
  }
  else
  {
    NvM_AdminBlockTable[AdminBlockIndex].NvMExtendedStatus |= (uint8)(NVM_ES_MULTICORE_LOCK_MASK);
  }
  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
  return Result;
}
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

#endif

FUNC_P2VAR(uint8, AUTOMATIC, NVM_CODE) NvM_GetUserRamBlockAddress(void)
{
  NvM_PtrToApplDataType UserRamBlockAddress;

#if (NVM_SYNC_MECHANISM == STD_ON)
  if (NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK))
  {
    UserRamBlockAddress = NVM_GET_MIRROR_ADDR();
  }
  else
#endif
  {
    /* NOTE: if no permanent RAM buffer is defined and no temporary RAM      */
    /*       buffer is available then an fatal error occured                 */
    /*       Availability of temporary RAM block must be checked first       */
    /*       because if it is provided by the user it must be preferred to   */
    /*       the permanent RAM block.                                        */
    /* !LINKSTO SWS_NvM_00278,1, SWS_NvM_00280,1, SWS_NvM_00436,1 */
    if ( NvM_TempRamBlockAddress != NULL_PTR )
    {
      UserRamBlockAddress = NvM_TempRamBlockAddress;
    }
    else if ( NvM_CurrentBlockDescriptorPtr->ramBlockDataAddress != NULL_PTR)
    {
      /* return address of permanent RAM block */
     /* Deviation MISRAC2012-1 */
      UserRamBlockAddress =
        (NvM_PtrToApplDataType)NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].ramBlockDataAddress;
    }
    else
    {
      UserRamBlockAddress = NULL_PTR;

      /* no user data block available --> fatal error */
      /* This code must never be reached */
      /* NvM_ReadAll is performed only for blocks having permanent RAM block   */
      /* NvM_WriteAll is performed only for blocks having permanent RAM block  */
      /* Error recovery (write back to Nv memory) is perfomed only for blocks  */
      /* having permanent RAM block */
      /* NvM_WriteBlock, NvM_ReadBlock, and NvM_RestoreBlockDefaults APIs      */
      /* have DET check to detect error when there is no permanent or          */
      /* temporary RAM block available */
      /* NvM_SetRamBlockStatus() also checks for permanent RAM block */
      /* Therefore, this code shall not be reached unless user disables DET    */
      /* check and uses block without a permanent or temporary RAM block */

      NvM_FatalError();
    }
  }

  return UserRamBlockAddress;
}



FUNC(uint16,NVM_CODE) NvM_GetMergedBlockNumber(void)
{
  uint16 ret;

  ret = ( uint16 )( ( uint16 )
    (  NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockBaseNum )  |
    ( ( uint16 )NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex ) );
  return ret;
}

FUNC(void,NVM_CODE) NvM_ResetRedundantBlockDataIndex(void)
{
  if ( NVM_BD_BLOCKMNGTTYPE(NvM_CurrentBlockDescriptorPtr->blockDesc) ==
         NVM_BD_BLOCKMNGTTYPE_REDUNDANT
     )
  {
    /* start service with second block */
    NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDatasetBlockIndex = 0U;
  }
}

FUNC(void,NVM_CODE) NvM_MultiRequestFinishedSingleBlock( NvM_CurrentFunctionPtrType NextState )
{
  /* write status of finished block operation to block administration
   * table.
   */
  NvM_PostService_Function();

#if ( NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK > 0U )
  /* !LINKSTO NVM176,1 */
  /* Invoke block specific callback function */
  NvM_InvokeSingleBlockCallback(NvM_GlobalBlockNumber,
                                NvM_CurrentServiceId,
                                NvM_GlobalErrorStatus);
#endif

  if ((NvM_GlobalErrorStatus != NVM_REQ_OK) &&
      (NvM_GlobalErrorStatus != NVM_REQ_BLOCK_SKIPPED) &&
      (NvM_GlobalErrorStatus != NVM_REQ_NV_INVALIDATED)
     )
  {
    NVM_REQUEST_INTERIM_RESULT = NVM_REQ_NOT_OK;
  }

  /* set pointer to next state */
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = NextState;
}/* end of function NvM_MultiRequestFinishedSingleBlock() */

FUNC(void,NVM_CODE) NvM_PreService_Function(void)
{
  NvM_CurrentBlockDescriptorPtr = &( NvM_BlockDescriptorTable[NvM_GlobalBlockNumber] );


  DBG_NVM_GLOBALWORKINGSTATUS((NvM_GlobalWorkingStatus),
                ((uint16)NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDynStatus));
  NvM_GlobalWorkingStatus = ( uint16 )NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDynStatus;
  /* clear dynamic status bits except 'WriteProtectionFlag',     */
  /* 'WriteProtectionPermanentFlag', 'Block Locked' flag and the changed flags          */
  NVM_CLEAR_WORKING_STATUS(
     (NvM_DynamicStatusType)(~(NvM_DynamicStatusType)( NVM_DYN_WPF_MASK    |
                                                       NVM_DYN_WPPF_MASK   |
                                                       NVM_DYN_CHANGE_MASK |
                                                       NVM_DYN_SRBSC_MASK  |
                                                       NVM_DYN_LOCK_MASK   |
                                                       NVM_DYN_MIRROR_MASK |
                                                       NVM_DYN_WBID_MASK |
                                                       NVM_DYN_REDUNDANCY_MASK |
                                                       NVM_DYN_WBORF_MASK
                                                     )
                             )
                     );
  /* Check if a temporary RAM has been provided with the user request.
     If so the temporary RAM shall be used as it shall have priority over
     explicit synchronization mechanism or permanent RAM */
  /* !LINKSTO SWS_NvM_00196,1, SWS_NvM_00210,1, SWS_NvM_00435,1 */
  if (NvM_TempRamBlockAddress != NULL_PTR)
  {
    NVM_CLEAR_WORKING_STATUS(NVM_DYN_UPRB_MASK);
  }
  else
  {
#if (NVM_SYNC_MECHANISM == STD_ON)
    /* Check if explicit synchronization shall be used for the current block.
       If so the explicit synchronization shall be used as it has priority over
       the permanent RAM */
    if (NVM_BD_USEMIRROR(NvM_CurrentBlockDescriptorPtr->blockDesc))
    {
      NVM_SET_WORKING_STATUS(NVM_DYN_MIRROR_MASK);
      NVM_CLEAR_WORKING_STATUS(NVM_DYN_UPRB_MASK);
    }
    else
#endif  /* NVM_SYNC_MECHANISM == STD_ON */
    {
      /* Check if  permanent RAM is configured for the current block */
      if(NvM_CurrentBlockDescriptorPtr->ramBlockDataAddress != NULL_PTR)
      {
        NVM_SET_WORKING_STATUS( NVM_DYN_UPRB_MASK );
      }
    }
  }
}

FUNC(void,NVM_CODE) NvM_PostService_Function(void)
{
  /* Protect concurrent access to administrative data */
  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

  /* the following data must always be set when processing of job is */
  /* interrupted.                                                    */
  NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDynStatus
    = (( uint16 )NvM_GlobalWorkingStatus) & (NVM_DYN_CHANGE_MASK |
                                             NVM_DYN_WPF_MASK |
                                             NVM_DYN_BRVE_MASK |
                                             NVM_DYN_LOCK_MASK |
                                             NVM_DYN_RDL_MASK |
                                             NVM_DYN_WPPF_MASK |
                                             NVM_ERBF_MASK |
                                             NVM_DYN_REDUNDANCY_MASK |
                                             NVM_DYN_WBORF_MASK);
  /* If this block was not written due to CancelWriteAll restore flag so it may be written later */
  if( NVM_CHECK_GLOBAL_GENERIC_STATUS(NVM_WRITE_ALL_PREVENTED_MASK) )
  {
    NVM_SET_DYN_STATUS(NvM_GlobalBlockNumber, (NVM_DYN_CHANGE_MASK | NVM_DYN_SRBSC_MASK));
    NVM_CLEAR_GLOBAL_GENERIC_STATUS(NVM_WRITE_ALL_PREVENTED_MASK);
  }
  if ( (!NVM_CHECK_GLOBAL_GENERIC_STATUS(NVM_GENERIC_CANCEL_MASK))&&
       (NVM_CHECK_GLOBAL_GENERIC_STATUS( NVM_GENERIC_MRKJP_MASK ))
     )
  {
    /* Another request for this block was inserted in the queue.
     * Therefore the ErrorStatus must remain NVM_REQ_PENDING
     * after the block was processed by NvM_WriteAll() or NvM_ReadAll().
     */
    NVM_CLEAR_GLOBAL_GENERIC_STATUS( NVM_GENERIC_MRKJP_MASK );
    DBG_NVM_GLOBALGENERICSTATUS((NvM_GlobalGenericStatus),
              (NvM_GlobalGenericStatus & (uint16)((uint16)(~(NVM_GENERIC_MRKJP_MASK)) & 0xFFFFU)));
    NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NVM_REQ_PENDING;
  }
  else if (NVM_CHECK_GLOBAL_GENERIC_STATUS(NVM_GENERIC_CANCEL_MASK))
  {
    /* A single block request for the same block is present
     * in the queue, but it must be cancelled. Therefore the ErrorStatus
     * must remain NVM_REQ_CANCELED after the block was processed by
     * NvM_WriteAll() or NvM_ReadAll().
     */
    NVM_CLEAR_GLOBAL_GENERIC_STATUS(NVM_GENERIC_CANCEL_MASK);
    DBG_NVM_GLOBALGENERICSTATUS((NvM_GlobalGenericStatus),
              (NvM_GlobalGenericStatus & (uint16)((uint16)(~(NVM_GENERIC_CANCEL_MASK)) & 0xFFFFU)));
    NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NVM_REQ_CANCELED;
  }
  else
  {
    NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NvM_GlobalErrorStatus;
  }
  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

  /* Reset the temporary RAM block address */
  NvM_TempRamBlockAddress = NULL_PTR;

}/* end of function  NvM_PostService_Function */

FUNC(void,NVM_CODE) NvM_MultiPostService_Function(void)
{
#if ( NVM_DRV_MODE_SWITCH == STD_ON )

  MemIf_SetMode( MEMIF_MODE_SLOW );

#endif

  /* Block number of multi block request is zero.      */
  NvM_GlobalBlockNumber = 0x0U;
  NvM_CurrentBlockDescriptorPtr = &(NvM_BlockDescriptorTable[0x0]);

  NvM_StateMachine_TerminateCurrentLevel();
}

#if ( NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK > 0U )
FUNC(void,NVM_CODE) NvM_InvokeSingleBlockCallback
(
  uint16 BlockIndex,
  uint8 ServiceId,
  uint8 ErrorStatus
)
{
  if (NvM_BlockDescriptorTable[BlockIndex].singleBlockCallback != NULL_PTR)
  {
    /* Call block specific callback function, ignore return value
     * as we do not treat failures in this case
     */
    (void)NvM_BlockDescriptorTable[BlockIndex].singleBlockCallback(ServiceId, ErrorStatus);
  }
}
#endif /* NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK > 0U  */

FUNC(boolean, NVM_CODE) NvM_CheckWriteProtection(uint16 BlockIndex)
{
  boolean Result = FALSE;

  /* Check whether the write protection flag is set or the permanent write
   * protection flag is set  */
  if ((NVM_CHECK_DYN_STATUS(BlockIndex, NVM_DYN_WPF_MASK)) ||
      (NVM_CHECK_DYN_STATUS(BlockIndex, NVM_DYN_WPPF_MASK)))
  {
    /* block is write protected --> TRUE */
    Result = TRUE;
  }
/* !LINKSTO NVM150,1 */
#if (NVM_NUMBER_OF_DATASET_BLOCKS > 0U)
  /* Check whether a ROM Block of a Dataset Block is requested */
  else if (NvM_CheckDataSetRomBlock(BlockIndex) == TRUE)
  {
    /* block is write protected --> TRUE */
    Result = TRUE;
  }
  else
  {
    /* Nothing to do (MISRA) */
  }
#endif /* NVM_NUMBER_OF_DATASET_BLOCKS > 0U */
  return Result;
}

FUNC(boolean,NVM_CODE) NvM_CheckDataSetRomBlock(uint16 BlockIndex)
{
  boolean Result = FALSE;
/* !LINKSTO NVM150,1 */
#if ( NVM_NUMBER_OF_DATASET_BLOCKS > 0U )
  if ( ( NVM_BD_BLOCKMNGTTYPE(NvM_BlockDescriptorTable[BlockIndex].blockDesc) ==
         NVM_BD_BLOCKMNGTTYPE_DATASET
       ) &&
       ( NvM_AdminBlockTable[BlockIndex].NvMDatasetBlockIndex
          >= NvM_BlockDescriptorTable[BlockIndex].nvBlockNum
       )
     )
  {
    /* The specified NVRAM Block is a Dataset Block and the Dataset Index
     * specifies a ROM Data Block.
     */
    Result = TRUE;
  }
#else
    TS_PARAM_UNUSED(BlockIndex);
#endif /* NVM_NUMBER_OF_DATASET_BLOCKS > 0U */
  return Result;
}

FUNC(uint8, NVM_CODE) NvM_GetBlockCRCLength(uint32 BlockDesc)
{
  uint8 NvMCrcLength;

  /* If current block use 32bit crc */
  if(NVM_BD_CRCTYPE(BlockDesc) == NVM_BD_CRCTYPE_CRC32)
  {
    NvMCrcLength = 4U;
  }
  /* If current block use 16bit crc */
  else if(NVM_BD_CRCTYPE(BlockDesc) == NVM_BD_CRCTYPE_CRC16)
  {
    NvMCrcLength = 2U;
  }
  /* If current block use 8bit crc */
  else if(NVM_BD_CRCTYPE(BlockDesc) == NVM_BD_CRCTYPE_CRC8)
  {
    NvMCrcLength = 1U;
  }
  /* If current block dont use any crc */
  else
  {
    NvMCrcLength = 0U;
  }

  return NvMCrcLength;
}

FUNC(uint16, NVM_CODE) NvM_GetBlockLength(uint32 BlockDesc, uint16 BlkLength)
{
 uint16 Length = BlkLength;
#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
  /* If current block has static block id enabled */
  if (NVM_BD_USESTATICBLOCKID(BlockDesc))
  {
    Length += NVM_BLOCKID_LENGTH;
  }
#else
  TS_PARAM_UNUSED(BlockDesc);
#endif

  return Length;
}

FUNC_P2VAR(uint8, AUTOMATIC, NVM_CODE) NvM_GetBufferAddress(void)
{
  NvM_PtrToApplDataType DataBufPtr;

#if ((NVM_INTERNAL_BUFFER_SIZE != 0U) || (NVM_SYNC_MECHANISM == STD_ON))
  /* If 'Use internal buffer' or 'Use Mirror' flags are set */
  if ((NVM_BD_USEINTERNALBUFFER(NvM_CurrentBlockDescriptorPtr->blockDesc))||
      (NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK)))
  {
    DataBufPtr = NVM_GET_INTERNAL_BUFFER();
  }
  else
#endif /* ((NVM_INTERNAL_BUFFER_SIZE != 0U) || (NVM_SYNC_MECHANISM == STD_ON)) */
  {
    DataBufPtr = NvM_GetUserRamBlockAddress();
  }

  return DataBufPtr;
}

FUNC_P2VAR(uint8, AUTOMATIC, NVM_CODE) NvM_GetDataAddress(void)
{
  NvM_PtrToApplDataType DataBufPtr = NvM_GetBufferAddress();

#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
#if ((NVM_SYNC_MECHANISM == STD_ON) || (NVM_INTERNAL_BUFFER_SIZE != 0U))
  if ((NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK)) ||
      (NVM_BD_USEINTERNALBUFFER(NvM_CurrentBlockDescriptorPtr->blockDesc)))
  {
    /* If Static Block Id is enabled, exclude Block Header. */
    if (NVM_BD_USESTATICBLOCKID(NvM_CurrentBlockDescriptorPtr->blockDesc))
    {
      DataBufPtr = &(DataBufPtr[NVM_BLOCKID_LENGTH]);
    }
  }
#endif
#endif
  return DataBufPtr;
}

#if (NVM_SYNC_MECHANISM == STD_ON)
/* !LINKSTO NvM.ExplicitSynch.InternalMirror,1 */
FUNC(Std_ReturnType, NVM_CODE) NvM_MirrorCopy
(
  NvM_NvToRamCopyCallbackType ReadCallback,
  NvM_RamToNvCopyCallbackType WriteCallback,
  NvM_PtrToApplDataType DataBufPtr
)
{
  Std_ReturnType ReturnVal = E_NOT_OK;

  /* This function will be called only when explicit synchronization is used.
   * According to requirement NvM.ExplicitSynch.CallbackFunctions, it shall always
   * be assured in configuration phase that the callback functions are defined if
   * explicit synchronization is used.
   *
   * NvM_MirrorCopy is an internal function that will be called only by the NvM module.
   * At each function call, one of the function pointers passed as arguments will be NOT NULL.
   * Call block specific callback function in order to let the application
   * copy data between the RAM block and NvM module's mirror.
   */
  if (ReadCallback != NULL_PTR)
  {
    /* !LINKSTO NvM.ExplicitSynch.ReadCallback,1 */
    /* In case of ReadBlock request:
     * Let the user's callback function copy the data.
     */
    ReturnVal = ReadCallback(DataBufPtr);
  }
  else
  {
    /* !LINKSTO NvM.ExplicitSynch.WriteCallback,1 */
    /* In case of WriteBlock request:
     * Let the user's callback function copy the data.
     */
    ReturnVal = WriteCallback(DataBufPtr);
  }
  /* If the mirror copy operation returns E_NOT_OK, NVM performs the retry
   * NvMRepeatMirrorOperations times and afterwards set the result to NVM_REQ_NOT_OK
   * and reports production error */
  /* !LINKSTO SWS_NvM_00517,1, SWS_NvM_00579,1, SWS_NvM_00837,1, SWS_NvM_00838,1 */
  if (E_OK != ReturnVal)
  {
#if (NVM_REPEAT_MIRROR_OPERATIONS > 0U)
    /* increment retry counter  */
     NvM_MirrorRetryCount++;
    /* If the callback returns E_NOT_OK, the process has to be repeated
     * the configured number of times (by NvMRepeatMirrorOperations).
     */
    if (NvM_MirrorRetryCount < NVM_REPEAT_MIRROR_OPERATIONS)
    {
      /* Set the request as pending and retry in the next main processing
       * function cycle */
      /* !LINKSTO SWS_NvM_00839,1, SWS_NvM_00840,1 */
      NvM_GlobalErrorStatus = NVM_REQ_PENDING;
    }
    else
#endif
    {
      /* Maximum number of retries has been reached:
       * Report production error
       * Finish processing current request with result NVM_REQ_NOT_OK
       */
#if (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_REQ_FAILED_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
      NVM_DEM_REPORT_ERROR(NVM_REQ_FAILED_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DET)
      NVM_DET_REPORT_ERROR( NvM_CurrentServiceId, NVM_E_DEMTODET_REQ_FAILED );
#elif (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_USER)
      NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockIdentifier, NvM_CurrentServiceId, NVM_E_USER_CALLOUT_REQ_FAILED);
#endif
      /* Reset Mirror retry counter */
      NvM_MirrorRetryCount = 0U;
      /* Set the request as pending */
      NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;
      /* Check if a RAM CRC calculation has to be performed for this block
         Mirror_copy request is triggered by SetRamBlockStatus */
#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
      if (NvM_CurrentServiceId == NVM_SET_RAM_BLOCK_STATUS_API_ID)
      {
         /* Reset the RAM block CRC calculation flag */
         NvM_CalcCrc_RemoveElement();
         /* Ram block CRC will be recalculated again */
      }
#endif
      /* Terminate the request */
      NvM_StateMachine_TerminateCurrentLevel();
    }
  }
  else
  {
    /* If the callback returns E_OK, reset the counter value. */
    NvM_MirrorRetryCount = 0U;
#if (NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_REQ_FAILED_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID)
    NVM_DEM_REPORT_ERROR(NVM_REQ_FAILED_DEM_EVENT_ID, DEM_EVENT_STATUS_PASSED);
#endif
  #elif ((NVM_PROD_ERR_HANDLING_REQ_FAILED == TS_PROD_ERR_REP_TO_USER) && defined(NVM_PRODUCTION_PASSED_ERRORS_FUNCTION_CALLOUT))
    NVM_PRODUCTION_PASSED_ERRORS_FUNCTION_CALLOUT(NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockIdentifier, NvM_CurrentServiceId, NVM_E_USER_CALLOUT_REQ_FAILED);

#endif
  }

  return ReturnVal;
} /* NvM_PerformMirrorOperation */

#endif

FUNC(void, NVM_CODE) NvM_CheckBlockStatus(void)
{
  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
  if (NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult == NVM_REQ_PENDING)
  {
    /* Another request for this block was inserted in the queue.
     * Therefore the ErrorStatus must remain NVM_REQ_PENDING
     * after the block was written by NvM_ReadAll()/NvM_WriteAll().
     */
    NVM_SET_GLOBAL_GENERIC_STATUS(NVM_GENERIC_MRKJP_MASK);
    DBG_NVM_GLOBALGENERICSTATUS((NvM_GlobalGenericStatus),
                      (NvM_GlobalGenericStatus | NVM_GENERIC_MRKJP_MASK));
  }

  if (NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult == NVM_REQ_CANCELED)
  {
    /* A single block request for the same block is present
     * in the queue, but it must be cancelled. Therefore the ErrorStatus
     * must remain NVM_REQ_CANCELED after the block was processed by
     * NvM_ReadAll()/NvM_WriteAll().
     */
    NVM_SET_GLOBAL_GENERIC_STATUS(NVM_GENERIC_CANCEL_MASK);
    DBG_NVM_GLOBALGENERICSTATUS((NvM_GlobalGenericStatus),
                    (NvM_GlobalGenericStatus | NVM_GENERIC_CANCEL_MASK));
  }
  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
}

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)
FUNC(Std_ReturnType, NVM_CODE) NvM_InsertRestoreBlockJob(
    boolean                               PRAM,
    uint16                                BlockId,
    uint16                                BlockIndex,
    P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DestPtr)
{
  Std_ReturnType Result = E_NOT_OK;
  NvM_CurrentFunctionPtrType StateMachineFunction;
  uint8 CurrentServiceId;

#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
  boolean ReportQueueOverflow = FALSE; /* To check queue overflow DEM/DET error reporting */
#endif

  if(PRAM)
  {
    StateMachineFunction = NvM_RestorePRAMBlockDefaults_Asc;
    CurrentServiceId = NVM_RESTORE_PRAM_BLOCK_DEFAULTS_API_ID;
  }
  else
  {
    StateMachineFunction = NvM_RestoreBlockDefaults_Asc;
    CurrentServiceId = NVM_RESTORE_BLOCK_DEFAULTS_API_ID;
  }
    /* protect concurrent access to the standard queue and dataset index */
  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

/* !LINKSTO NVM150,1 */
#if( NVM_NUMBER_OF_DATASET_BLOCKS > 0U )
  /* In case block management type NVM_BLOCK_DATASET
   * RestoreBockDefaults shall return with E_NOT_OK if at least one
   * ROM block is configured and the data index points at a NV block.
   */
  /* !LINKSTO NVM353,1 */
  /* !LINKSTO SWS_NvM_00818,1 */
  if (
      ( NVM_BD_BLOCKMNGTTYPE(NvM_BlockDescriptorTable[BlockIndex].blockDesc) ==
          NVM_BD_BLOCKMNGTTYPE_DATASET
      )
      &&
      (NvM_BlockDescriptorTable[BlockIndex].romBlockNum > 0U)
      &&
      ( !( NVM_CHECK_DATASET_ROM_INDEX_SELECTED(BlockIndex)))
     )
  {
    /* Nothing to do: "Result" already E_NOT_OK */
  }
  else
#endif /* NVM_NUMBER_OF_DATASET_BLOCKS > 0U */
  {
    if (NvM_AdminBlockTable[BlockIndex].NvMResult != NVM_REQ_PENDING)
    {
      /* !LINKSTO SWS_NvM_00815,1 */
      /* !LINKSTO NVM224,1 */
      /* Deviation MISRAC2012-1 */
      Result = NvM_Queue_InsertStandard(
                        BlockIndex,
                        (NvM_PtrToApplDataType)NvM_DestPtr,
                        StateMachineFunction);

#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
      if (E_OK != Result)
      {
        /* Queue overflow DEM/DET error needs to be reported */
        ReportQueueOverflow = TRUE;
      }
#endif
      if (E_OK == Result)
      {
        /* Insert the API ID into NvM_AdminBlockTable.
         * The information shall be available in case the request has only been queued,
         * but the processing has not been started and a cancel request was issued.
         */
        NvM_AdminBlockTable[BlockIndex].NvMCurrentServiceId = CurrentServiceId;
      }
    }
  }

  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_DISABLE)
  if(ReportQueueOverflow == TRUE)
  {
#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_QUEUE_OVERFLOW_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
            NVM_DEM_REPORT_ERROR(NVM_QUEUE_OVERFLOW_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DET)
            NVM_DET_REPORT_ERROR(CurrentServiceId, NVM_E_DEMTODET_QUEUE_OVERFLOW);
#elif ( NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_USER )
        NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(BlockId, CurrentServiceId, NVM_E_USER_CALLOUT_QUEUE_OVERFLOW);
#endif
  }
#endif

#if (NVM_BSWM_BLOCK_STATUS_INFO == STD_ON)
  /* Check if block status changed and block status reporting to BswM is
   * enabled for this block */
  if ( (NVM_BD_BLOCKSTATUSINFO(NvM_BlockDescriptorTable[BlockIndex].blockDesc)) &&
       (E_OK == Result) )
  {
    /* Block status changed due to queuing restore block default
     * request, report the block status to BswM */
    BswM_NvM_CurrentBlockMode(BlockId, NVM_REQ_PENDING);
  }
#endif

#if ( defined(NVM_MULTICORE_ENABLED) )
  NvM_AdminBlockTable[BlockIndex].NvMExtendedStatus &= (uint8)(~(NVM_ES_MULTICORE_LOCK_MASK));
#endif

#if  ((NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW != TS_PROD_ERR_REP_TO_USER)  \
      &&                                                                 \
      (NVM_BSWM_BLOCK_STATUS_INFO != STD_ON))
  TS_PARAM_UNUSED(BlockId);
#endif

  return Result;
}
#endif

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

#if (defined(NVM_READBLOCK_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_READBLOCK_CALLOUT */

/*======================NvM_CheckCondReadBlock==========================*/
FUNC(uint16,NVM_CODE) NvM_CheckCondReadBlock
(
  NvM_BlockIdType BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DstPtr
)
{
  uint16 RetBlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
  boolean ConditionValid = FALSE;

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  /* Check if Block Id is within valid range */
  if ( RetBlockIndex == NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    NVM_DET_REPORT_ERROR( NVM_READ_BLOCK_API_ID, NVM_E_PARAM_BLOCK_ID );
  }
  /* Check if module is initialized by NvM_Init() */
  else if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    NVM_DET_REPORT_ERROR( NVM_READ_BLOCK_API_ID, NVM_E_NOT_INITIALIZED );
  }
  /* Check if an other request for block BlockId is already pending. */
  else if ( NvM_AdminBlockTable[RetBlockIndex].NvMResult == NVM_REQ_PENDING )
  {
    NVM_DET_REPORT_ERROR( NVM_READ_BLOCK_API_ID, NVM_E_BLOCK_PENDING );
  }
  /* Check if no permanent RAM block is defined, the explicit synchronization
   * mechanism is disabled for the block and NvM_DstPtr is equal to NULL.
   */
  /* !LINKSTO SWS_NvM_00616,1 */
#if (NVM_SYNC_MECHANISM == STD_ON)
  else if ( ( NvM_BlockDescriptorTable[RetBlockIndex].ramBlockDataAddress == NULL_PTR ) &&
            ( !NVM_BD_USEMIRROR(NvM_BlockDescriptorTable[RetBlockIndex].blockDesc) ) &&
            ( NvM_DstPtr == NULL_PTR )
          )
#else
  else if ( ( NvM_BlockDescriptorTable[RetBlockIndex].ramBlockDataAddress == NULL_PTR ) &&
            ( NvM_DstPtr == NULL_PTR )
          )
#endif /* NVM_SYNC_MECHANISM == STD_ON */
  {
    /* No Permanent RAM Data Block defined, no explicit synchronization
     * and also there is no Temporary RAM Data Block.
     * In this case, a NVM_E_PARAM_ADDRESS Det error must be reported.
     */
    NVM_DET_REPORT_ERROR( NVM_READ_BLOCK_API_ID,NVM_E_PARAM_ADDRESS );
  }
  else
#endif/*DET ENABLE*/
  {
#if ( defined(NVM_MULTICORE_ENABLED) )
#if (defined(NVM_READBLOCK_CALLOUT))
#if (( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) && ( NVM_SIZE_IMMEDIATE_JOB_QUEUE > 0 ))
    if ( NvM_BlockDescriptorTable[RetBlockIndex].blockJobPriority == NVM_BLOCK_IMMEDIATE_PRIORITY )
    {
      if(NVM_IMMEDIATE_QUEUE_NOT_FULL())
      {
        ConditionValid = TRUE;
      }
    }
    else
#endif /* #if (( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) && ( NVM_SIZE_IMMEDIATE_JOB_QUEUE > 0 )) */
    {
      if(E_OK == NvM_Queue_CheckFullStandard(RetBlockIndex))
      {
        ConditionValid = TRUE;
      }
    }
    if ( ConditionValid == TRUE )
#endif /* #if (defined(NVM_READBLOCK_CALLOUT)) */
    {
      /*Sets the multicore flag if not already set, returns FALSE if can't set*/
      ConditionValid = NvM_CheckSetMcFlag(RetBlockIndex);
#if (NVM_DEV_ERROR_DETECT == STD_ON)
      if ( ConditionValid == FALSE )
      {
        /* !LINKSTO NvM.MultiCore.Read_MultiCoreLock_DET,1 */
        NVM_DET_REPORT_ERROR( NVM_READ_BLOCK_API_ID, NVM_E_BLOCK_PENDING );
      }
#else
      TS_PARAM_UNUSED(NvM_DstPtr);
#endif/* NVM_DEV_ERROR_DETECT == STD_ON */
    }
#else
    ConditionValid = TRUE;
#if (NVM_DEV_ERROR_DETECT == STD_OFF)
    TS_PARAM_UNUSED(BlockId);
    TS_PARAM_UNUSED(NvM_DstPtr);
#endif/*NVM_DEV_ERROR_DETECT == STD_OFF*/
#endif /*defined(NVM_MULTICORE_ENABLED)*/
  }
  if(ConditionValid == FALSE)
  {
    RetBlockIndex = NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS;
  }
  return RetBlockIndex;
}

#if (defined(NVM_READBLOCK_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_READBLOCK_CALLOUT */

/*======================NvM_CheckCondWriteBlock==========================*/

#if (defined(NVM_WRITEBLOCK_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_WRITEBLOCK_CALLOUT */

FUNC(uint16,NVM_CODE) NvM_CheckCondWriteBlock
(
  NvM_BlockIdType BlockId,
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvM_SrcPtr
)
{
  uint16 RetBlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
  boolean ConditionValid = FALSE;

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  /* Check if Block Id is within valid range */
  if ( RetBlockIndex == NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    NVM_DET_REPORT_ERROR( NVM_WRITE_BLOCK_API_ID, NVM_E_PARAM_BLOCK_ID );
  }
  /* Check if module is initialized by NvM_Init() */
  else if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    NVM_DET_REPORT_ERROR( NVM_WRITE_BLOCK_API_ID, NVM_E_NOT_INITIALIZED );
  }
  /* Check if an other request for block RetBlockIndex is already pending. */
  else if ( NvM_AdminBlockTable[RetBlockIndex].NvMResult == NVM_REQ_PENDING )
  {
    NVM_DET_REPORT_ERROR( NVM_WRITE_BLOCK_API_ID, NVM_E_BLOCK_PENDING );
  }
  /* Check if block is locked. */
  else if (NVM_CHECK_DYN_STATUS(RetBlockIndex, NVM_DYN_LOCK_MASK))
  {
    NVM_DET_REPORT_ERROR(NVM_WRITE_BLOCK_API_ID, NVM_E_BLOCK_LOCKED);
  }
  /* Check if no permanent RAM block is defined, the explicit synchronization
   * mechanism is disabled for the block and NvM_SrcPtr is equal to NULL.
   */
  /* !LINKSTO SWS_NvM_00622,1 */
#if (NVM_SYNC_MECHANISM == STD_ON)
  else if ( ( NvM_BlockDescriptorTable[RetBlockIndex].ramBlockDataAddress == NULL_PTR ) &&
            ( !NVM_BD_USEMIRROR(NvM_BlockDescriptorTable[RetBlockIndex].blockDesc) ) &&
            ( NvM_SrcPtr == NULL_PTR )
          )
#else
  else if ( ( NvM_BlockDescriptorTable[RetBlockIndex].ramBlockDataAddress == NULL_PTR ) &&
            ( NvM_SrcPtr == NULL_PTR )
          )
#endif /* NVM_SYNC_MECHANISM == STD_ON */
  {
    /* No Permanent RAM Data Block defined, no explicit synchronization
     * and also there is no Temporary Data Block.
     * In this case, a NVM_E_PARAM_ADDRESS Det error must be reported.
     */
    NVM_DET_REPORT_ERROR( NVM_WRITE_BLOCK_API_ID, NVM_E_PARAM_ADDRESS );
  }
  else
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
#if ( defined(NVM_MULTICORE_ENABLED) )
#if (defined(NVM_WRITEBLOCK_CALLOUT))
#if (( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) && ( NVM_SIZE_IMMEDIATE_JOB_QUEUE > 0 ))
    if ( NvM_BlockDescriptorTable[RetBlockIndex].blockJobPriority == NVM_BLOCK_IMMEDIATE_PRIORITY )
    {
      if(NVM_IMMEDIATE_QUEUE_NOT_FULL())
      {
        ConditionValid = TRUE;
      }
    }
    else
#endif /* #if (( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) && ( NVM_SIZE_IMMEDIATE_JOB_QUEUE > 0 )) */
    {
      if(E_OK == NvM_Queue_CheckFullStandard(RetBlockIndex))
      {
        ConditionValid = TRUE;
      }
    }
    if ( ConditionValid == TRUE )
#endif /* #if (defined(NVM_WRITEBLOCK_CALLOUT)) */
    {
      /*Sets the multicore flag if not already set, returns FALSE if can't set*/
      ConditionValid = NvM_CheckSetMcFlag(RetBlockIndex);
#if (NVM_DEV_ERROR_DETECT == STD_ON)
      if ( ConditionValid == FALSE )
      {
        /* !LINKSTO NvM.MultiCore.Write_MultiCoreLock_DET,1 */
        NVM_DET_REPORT_ERROR( NVM_WRITE_BLOCK_API_ID, NVM_E_BLOCK_PENDING );
      }
#else /* NVM_DEV_ERROR_DETECT == STD_ON */
      TS_PARAM_UNUSED(NvM_SrcPtr);
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
    }
#else
    ConditionValid = TRUE;
#if (NVM_DEV_ERROR_DETECT == STD_OFF)
    TS_PARAM_UNUSED(BlockId);
    TS_PARAM_UNUSED(NvM_SrcPtr);
#endif/*NVM_DEV_ERROR_DETECT == STD_OFF*/
#endif /*defined(NVM_MULTICORE_ENABLED)*/
  }
  if(ConditionValid == FALSE)
  {
    RetBlockIndex = NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS;
  }
  return RetBlockIndex;
}

#if (defined(NVM_WRITEBLOCK_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_WRITEBLOCK_CALLOUT */

/*======================NvM_CheckCondRestoreBlockDefaults==========================*/

#if (defined(NVM_RESTOREBLOCKDEFAULTS_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_RESTOREBLOCKDEFAULTS_CALLOUT */

FUNC(uint16,NVM_CODE) NvM_CheckCondRestoreBlockDefaults
(
  NvM_BlockIdType BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DstPtr
)
{
  uint16 RetBlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
  boolean ConditionValid = FALSE;

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  /* Check if Block Id is within valid range */
  if ( RetBlockIndex == NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    NVM_DET_REPORT_ERROR( NVM_RESTORE_BLOCK_DEFAULTS_API_ID, NVM_E_PARAM_BLOCK_ID );
  }
  /* Check if module is initialized by NvM_Init() */
  else if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    NVM_DET_REPORT_ERROR( NVM_RESTORE_BLOCK_DEFAULTS_API_ID, NVM_E_NOT_INITIALIZED );
  }
  /* Check if an other request for block RetBlockIndex is already pending. */
  else if ( NvM_AdminBlockTable[RetBlockIndex].NvMResult == NVM_REQ_PENDING )
  {
    NVM_DET_REPORT_ERROR( NVM_RESTORE_BLOCK_DEFAULTS_API_ID, NVM_E_BLOCK_PENDING );
  }
  /* Check if no permanent RAM block is defined, the explicit synchronization
   * mechanism is disabled and NvM_DstPtr is equal to NULL.
   */
  /* !LINKSTO SWS_NvM_00629,1 */
#if (NVM_SYNC_MECHANISM == STD_ON)
  else if ( ( NvM_BlockDescriptorTable[RetBlockIndex].ramBlockDataAddress == NULL_PTR ) &&
            ( !NVM_BD_USEMIRROR(NvM_BlockDescriptorTable[RetBlockIndex].blockDesc) ) &&
            ( NvM_DstPtr == NULL_PTR )
          )
#else
  else if ( ( NvM_BlockDescriptorTable[RetBlockIndex].ramBlockDataAddress == NULL_PTR ) &&
            ( NvM_DstPtr == NULL_PTR )
          )
#endif /* NVM_SYNC_MECHANISM == STD_ON */
  {
    /* No Permanent RAM Data Block defined, no explicit synchronization
     * and also there is no Temporary RAM Data Block.
     * In this case, a NVM_E_PARAM_ADDRESS Det error must be reported.
     */
    NVM_DET_REPORT_ERROR( NVM_RESTORE_BLOCK_DEFAULTS_API_ID, NVM_E_PARAM_ADDRESS );
  }
  /* Check if no permanent ROM block and no InitBlockCallback is defined
   */
  /* !LINKSTO SWS_NvM_00883,1, SWS_NvM_00885,1 */
  else if ( ( NvM_BlockDescriptorTable[RetBlockIndex].romBlockDataAddress == NULL_PTR ) &&
            ( NvM_BlockDescriptorTable[RetBlockIndex].initBlockCallback == NULL_PTR )
          )
  {
    /* No ROM Block containig default data is defined and no
     * InitBlockCallback function.
     */
    NVM_DET_REPORT_ERROR( NVM_RESTORE_BLOCK_DEFAULTS_API_ID, NVM_E_BLOCK_WITHOUT_DEFAULTS );
  }
  else
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
#if ( NVM_DEV_ERROR_DETECT != STD_ON )
    if(
        ( NvM_BlockDescriptorTable[RetBlockIndex].romBlockDataAddress == NULL_PTR )
        &&
        ( NvM_BlockDescriptorTable[RetBlockIndex].initBlockCallback == NULL_PTR )
        )
    {
      /* No ROM Block containing default data is defined and no
       * InitBlockCallback function, => ConditionValid remains set to FALSE.
       */
    }
    else
#endif
    {
#if ( defined(NVM_MULTICORE_ENABLED) )
#if (defined(NVM_RESTOREBLOCKDEFAULTS_CALLOUT))
    if(E_OK == NvM_Queue_CheckFullStandard(RetBlockIndex))
    {
      ConditionValid = TRUE;
    }
    if ( ConditionValid == TRUE )
#endif /* #if (defined(NVM_RESTOREBLOCKDEFAULTS_CALLOUT)) */
    {
      /*Sets the multicore flag if not already set, returns FALSE if can't set*/
      ConditionValid = NvM_CheckSetMcFlag(RetBlockIndex);
#if (NVM_DEV_ERROR_DETECT == STD_ON)
      if ( ConditionValid == FALSE )
      {
        /* !LINKSTO NvM.MultiCore.RestoreBlockDefaults_MultiCoreLock_DET,1 */
        NVM_DET_REPORT_ERROR( NVM_RESTORE_BLOCK_DEFAULTS_API_ID, NVM_E_BLOCK_PENDING );
      }
#else
      TS_PARAM_UNUSED(NvM_DstPtr);
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
    }
#else
      ConditionValid = TRUE;
#if (NVM_DEV_ERROR_DETECT == STD_OFF)
      TS_PARAM_UNUSED(BlockId);
      TS_PARAM_UNUSED(NvM_DstPtr);
#endif/*NVM_DEV_ERROR_DETECT == STD_OFF*/
#endif /*defined(NVM_MULTICORE_ENABLED)*/
    }
  }
  if(ConditionValid == FALSE)
  {
    RetBlockIndex = NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS;
  }
  return RetBlockIndex;
}

#if (defined(NVM_RESTOREBLOCKDEFAULTS_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_RESTOREBLOCKDEFAULTS_CALLOUT */

/*======================NvM_CheckCondReadPRAMBlock==========================*/

#if (defined(NVM_READPRAMBLOCK_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_READPRAMBLOCK_CALLOUT */

FUNC(uint16,NVM_CODE) NvM_CheckCondReadPRAMBlock
(
  NvM_BlockIdType BlockId
)
{
  uint16 RetBlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
  boolean ConditionValid = FALSE;

#if ( NVM_DEV_ERROR_DETECT == STD_ON )

  /* Check if Block Id is within valid range */
  /* !LINKSTO SWS_NvM_00826,1 */
  if ( RetBlockIndex == NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    NVM_DET_REPORT_ERROR( NVM_READ_PRAM_BLOCK_API_ID, NVM_E_PARAM_BLOCK_ID );
  }
  /* Check if module is initialized by NvM_Init() */
  /* !LINKSTO SWS_NvM_00823,1 */
  else if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    NVM_DET_REPORT_ERROR( NVM_READ_PRAM_BLOCK_API_ID, NVM_E_NOT_INITIALIZED );
  }
  /* Check if an other request for block RetBlockIndex is already pending. */
  /* !LINKSTO SWS_NvM_00824,1 */
  else if ( NvM_AdminBlockTable[RetBlockIndex].NvMResult == NVM_REQ_PENDING )
  {
    NVM_DET_REPORT_ERROR( NVM_READ_PRAM_BLOCK_API_ID, NVM_E_BLOCK_PENDING );
  }
  /* Check if no permanent RAM block is defined or the explicit synchronization
   * mechanism is disabled.
   */
  /* !LINKSTO SWS_NvM_00825,1 */
#if (NVM_SYNC_MECHANISM == STD_ON)
  else if ( ( NvM_BlockDescriptorTable[RetBlockIndex].ramBlockDataAddress == NULL_PTR ) &&
            ( !NVM_BD_USEMIRROR(NvM_BlockDescriptorTable[RetBlockIndex].blockDesc) )
          )
#else
  else if ( NvM_BlockDescriptorTable[RetBlockIndex].ramBlockDataAddress == NULL_PTR )
#endif /* NVM_SYNC_MECHANISM == STD_ON */
  {
    /* No Permanent RAM Data Block defined, no explicit synchronization.
     * In this case, a NVM_E_PARAM_ADDRESS Det error must be reported.
     */
    NVM_DET_REPORT_ERROR( NVM_READ_PRAM_BLOCK_API_ID,NVM_E_PARAM_ADDRESS );
  }
  else
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
#if ( defined(NVM_MULTICORE_ENABLED) )
#if (defined(NVM_READPRAMBLOCK_CALLOUT))
#if (( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) && ( NVM_SIZE_IMMEDIATE_JOB_QUEUE > 0 ))
    if ( NvM_BlockDescriptorTable[RetBlockIndex].blockJobPriority == NVM_BLOCK_IMMEDIATE_PRIORITY )
    {
      if(NVM_IMMEDIATE_QUEUE_NOT_FULL())
      {
        ConditionValid = TRUE;
      }
    }
    else
#endif /* #if (( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) && ( NVM_SIZE_IMMEDIATE_JOB_QUEUE > 0 )) */
    {
      if(E_OK == NvM_Queue_CheckFullStandard(RetBlockIndex))
      {
        ConditionValid = TRUE;
      }
    }
    if ( ConditionValid == TRUE )
#endif /* #if (defined(NVM_READPRAMBLOCK_CALLOUT)) */
    {
      /*Sets the multicore flag if not already set, returns FALSE if can't set*/
      ConditionValid = NvM_CheckSetMcFlag(RetBlockIndex);
#if (NVM_DEV_ERROR_DETECT == STD_ON)
      if ( ConditionValid == FALSE )
      {
        /* !LINKSTO NvM.MultiCore.ReadPRAMBlock_MultiCoreLock_DET,1 */
        NVM_DET_REPORT_ERROR( NVM_READ_PRAM_BLOCK_API_ID, NVM_E_BLOCK_PENDING );
      }
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
    }
#else
    ConditionValid = TRUE;
#if (NVM_DEV_ERROR_DETECT == STD_OFF)
    TS_PARAM_UNUSED(BlockId);
#endif/*NVM_DEV_ERROR_DETECT == STD_OFF*/
#endif /*defined(NVM_MULTICORE_ENABLED)*/
  }
  if(ConditionValid == FALSE)
  {
    RetBlockIndex = NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS;
  }
  return RetBlockIndex;
}

#if (defined(NVM_READPRAMBLOCK_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_READPRAMBLOCK_CALLOUT */

/*======================NvM_CheckCondWritePRAMBlock==========================*/

#if (defined(NVM_WRITEPRAMBLOCK_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_WRITEPRAMBLOCK_CALLOUT */

FUNC(uint16,NVM_CODE) NvM_CheckCondWritePRAMBlock
(
  NvM_BlockIdType BlockId
)
{
  uint16 RetBlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
  boolean ConditionValid = FALSE;

#if ( NVM_DEV_ERROR_DETECT == STD_ON )

  /* Check if Block Id is within valid range */
  /* !LINKSTO SWS_NvM_00829,1 */
  if ( RetBlockIndex == NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    NVM_DET_REPORT_ERROR( NVM_WRITE_PRAM_BLOCK_API_ID, NVM_E_PARAM_BLOCK_ID );
  }
  /* Check if module is initialized by NvM_Init() */
  /* !LINKSTO SWS_NvM_00827,1 */
  else if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    NVM_DET_REPORT_ERROR( NVM_WRITE_PRAM_BLOCK_API_ID, NVM_E_NOT_INITIALIZED );
  }
  /* Check if an other request for block RetBlockIndex is already pending. */
  /* !LINKSTO SWS_NvM_00828,1 */
  else if ( NvM_AdminBlockTable[RetBlockIndex].NvMResult == NVM_REQ_PENDING )
  {
    NVM_DET_REPORT_ERROR( NVM_WRITE_PRAM_BLOCK_API_ID, NVM_E_BLOCK_PENDING );
  }
  /* Check if block is locked. */
  /* !LINKSTO SWS_NvM_00830,1 */
  else if (NVM_CHECK_DYN_STATUS(RetBlockIndex, NVM_DYN_LOCK_MASK))
  {
    NVM_DET_REPORT_ERROR(NVM_WRITE_PRAM_BLOCK_API_ID, NVM_E_BLOCK_LOCKED);
  }
  /* Check if no permanent RAM block is defined or the explicit synchronization
   * mechanism is disabled for the block.
   */
  /* !LINKSTO SWS_NvM_00893,1 */
#if (NVM_SYNC_MECHANISM == STD_ON)
  else if ( ( NvM_BlockDescriptorTable[RetBlockIndex].ramBlockDataAddress == NULL_PTR ) &&
            ( !NVM_BD_USEMIRROR(NvM_BlockDescriptorTable[RetBlockIndex].blockDesc) )
          )
#else
  else if ( NvM_BlockDescriptorTable[RetBlockIndex].ramBlockDataAddress == NULL_PTR )
#endif /* NVM_SYNC_MECHANISM == STD_ON */
  {
    /* No Permanent RAM Data Block defined, no explicit synchronization
     * and also there is no Temporary Data Block.
     * In this case, a NVM_E_PARAM_ADDRESS Det error must be reported.
     */
    NVM_DET_REPORT_ERROR( NVM_WRITE_PRAM_BLOCK_API_ID, NVM_E_PARAM_ADDRESS );
  }
  else
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
#if ( defined(NVM_MULTICORE_ENABLED) )
#if (defined(NVM_WRITEPRAMBLOCK_CALLOUT))
#if (( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) && ( NVM_SIZE_IMMEDIATE_JOB_QUEUE > 0 ))
    if ( NvM_BlockDescriptorTable[RetBlockIndex].blockJobPriority == NVM_BLOCK_IMMEDIATE_PRIORITY )
    {
      if(NVM_IMMEDIATE_QUEUE_NOT_FULL())
      {
        ConditionValid = TRUE;
      }
    }
    else
#endif /* #if (( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ) && ( NVM_SIZE_IMMEDIATE_JOB_QUEUE > 0 )) */
    {
      if(E_OK == NvM_Queue_CheckFullStandard(RetBlockIndex))
      {
        ConditionValid = TRUE;
      }
    }
    if ( ConditionValid == TRUE )
#endif /* #if (defined(NVM_WRITEPRAMBLOCK_CALLOUT)) */
    {
      /*Sets the multicore flag if not already set, returns FALSE if can't set*/
      ConditionValid = NvM_CheckSetMcFlag(RetBlockIndex);
#if (NVM_DEV_ERROR_DETECT == STD_ON)
      if ( ConditionValid == FALSE )
      {
        /* !LINKSTO NvM.MultiCore.WritePRAMBlock_MultiCoreLock_DET,1 */
        NVM_DET_REPORT_ERROR( NVM_WRITE_PRAM_BLOCK_API_ID, NVM_E_BLOCK_PENDING );
      }
#endif/* NVM_DEV_ERROR_DETECT == STD_ON */
    }
#else
    ConditionValid = TRUE;
#if (NVM_DEV_ERROR_DETECT == STD_OFF)
    TS_PARAM_UNUSED(BlockId);
#endif/*NVM_DEV_ERROR_DETECT == STD_OFF*/
#endif /*defined(NVM_MULTICORE_ENABLED)*/
  }
  if(ConditionValid == FALSE)
  {
    RetBlockIndex = NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS;
  }
  return RetBlockIndex;
}

#if (defined(NVM_WRITEPRAMBLOCK_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_WRITEPRAMBLOCK_CALLOUT */

/*======================NvM_CheckCondRestorePRAMBlockDefaults==========================*/

#if (defined(NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT */

FUNC(uint16,NVM_CODE) NvM_CheckCondRestorePRAMBlockDefaults
(
  NvM_BlockIdType BlockId
)
{
  uint16 RetBlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
  boolean ConditionValid = FALSE;

#if ( NVM_DEV_ERROR_DETECT == STD_ON )

  /* Check if Block Id is within valid range */
  /* !LINKSTO SWS_NvM_00834,1 */
  if ( RetBlockIndex == NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    NVM_DET_REPORT_ERROR( NVM_RESTORE_PRAM_BLOCK_DEFAULTS_API_ID, NVM_E_PARAM_BLOCK_ID );
  }
  /* Check if module is initialized by NvM_Init() */
  /* !LINKSTO SWS_NvM_00831,1 */
  else if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    NVM_DET_REPORT_ERROR( NVM_RESTORE_PRAM_BLOCK_DEFAULTS_API_ID, NVM_E_NOT_INITIALIZED );
  }
  /* Check if an other request for block RetBlockIndex is already pending. */
  /* !LINKSTO SWS_NvM_00832,1 */
  else if ( NvM_AdminBlockTable[RetBlockIndex].NvMResult == NVM_REQ_PENDING )
  {
    NVM_DET_REPORT_ERROR( NVM_RESTORE_PRAM_BLOCK_DEFAULTS_API_ID, NVM_E_BLOCK_PENDING );
  }
  /* Check if no permanent RAM block is defined or the explicit synchronization
   * mechanism is disabled.
   */
  /* !LINKSTO SWS_NvM_00894,1 */
#if (NVM_SYNC_MECHANISM == STD_ON)
  else if ( ( NvM_BlockDescriptorTable[RetBlockIndex].ramBlockDataAddress == NULL_PTR ) &&
            ( !NVM_BD_USEMIRROR(NvM_BlockDescriptorTable[RetBlockIndex].blockDesc) )
          )
#else
  else if ( NvM_BlockDescriptorTable[RetBlockIndex].ramBlockDataAddress == NULL_PTR )
#endif /* NVM_SYNC_MECHANISM == STD_ON */
  {
    /* No Permanent RAM Data Block defined, no explicit synchronization
     * and also there is no Temporary RAM Data Block.
     * In this case, a NVM_E_PARAM_ADDRESS Det error must be reported.
     */
    NVM_DET_REPORT_ERROR( NVM_RESTORE_PRAM_BLOCK_DEFAULTS_API_ID, NVM_E_PARAM_ADDRESS );
  }
  /* Check if no permanent ROM block and no InitBlockCallback is defined
   */
  /* !LINKSTO SWS_NvM_00886,1, SWS_NvM_00884,1 */
  else if ( ( NvM_BlockDescriptorTable[RetBlockIndex].romBlockDataAddress == NULL_PTR ) &&
            ( NvM_BlockDescriptorTable[RetBlockIndex].initBlockCallback == NULL_PTR )
          )
  {
    /* No ROM Block containig default data is defined and no
     * InitBlockCallback function.
     */
    NVM_DET_REPORT_ERROR( NVM_RESTORE_PRAM_BLOCK_DEFAULTS_API_ID, NVM_E_BLOCK_WITHOUT_DEFAULTS );
  }
  else
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
#if ( NVM_DEV_ERROR_DETECT != STD_ON )
    if(
        ( NvM_BlockDescriptorTable[RetBlockIndex].romBlockDataAddress == NULL_PTR )
        &&
        ( NvM_BlockDescriptorTable[RetBlockIndex].initBlockCallback == NULL_PTR )
        )
    {
      /* No ROM Block containing default data is defined and no
       * InitBlockCallback function, => ConditionValid remains set to FALSE.
       */
    }
    else
#endif
    {
#if ( defined(NVM_MULTICORE_ENABLED) )
#if (defined(NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT))
    if(E_OK == NvM_Queue_CheckFullStandard(RetBlockIndex))
    {
      ConditionValid = TRUE;
    }
    if ( ConditionValid == TRUE )
#endif /* #if (defined(NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT)) */
    {
      /*Sets the multicore flag if not already set, returns FALSE if can't set*/
      ConditionValid = NvM_CheckSetMcFlag(RetBlockIndex);
#if (NVM_DEV_ERROR_DETECT == STD_ON)
      if ( ConditionValid == FALSE )
      {
        /* !LINKSTO NvM.MultiCore.RestorePRAMBlockDefaults_MultiCoreLock_DET,1 */
        NVM_DET_REPORT_ERROR( NVM_RESTORE_PRAM_BLOCK_DEFAULTS_API_ID, NVM_E_BLOCK_PENDING );
      }
#endif/* NVM_DEV_ERROR_DETECT == STD_ON */
    }
#else
      ConditionValid = TRUE;
#if (NVM_DEV_ERROR_DETECT == STD_OFF)
      TS_PARAM_UNUSED(BlockId);
#endif/*NVM_DEV_ERROR_DETECT == STD_OFF*/
#endif /*defined(NVM_MULTICORE_ENABLED)*/
    }
  }

  if(ConditionValid == FALSE)
  {
    RetBlockIndex = NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS;
  }
  return RetBlockIndex;
}

#if (defined(NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT */

/*===============================NvM_CheckCondCancelJobs===================================*/

#if (defined(NVM_CANCELJOBS_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_CANCELJOBS_CALLOUT */

FUNC(uint16,NVM_CODE) NvM_CheckCondCancelJobs
(
  NvM_BlockIdType BlockId
)
{
  uint16 RetBlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
  boolean ConditionValid = FALSE;

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  /* Check if Block Id is within valid range */
  if ( RetBlockIndex == NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    NVM_DET_REPORT_ERROR(NVM_CANCEL_JOBS_API_ID, NVM_E_PARAM_BLOCK_ID);
  }
  /* Check if module is initialized by NvM_Init() */
  else if (NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE(NVM_GENERIC_INIT_FLAG_MASK))
  {
    NVM_DET_REPORT_ERROR(NVM_CANCEL_JOBS_API_ID, NVM_E_NOT_INITIALIZED);
  }
  else
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
#if ( defined(NVM_MULTICORE_ENABLED) )
    /*Sets the multicore flag is not already set, returns FALSE if can't set*/
    ConditionValid = NvM_CheckSetMcFlag(RetBlockIndex);
#if (NVM_DEV_ERROR_DETECT == STD_ON)
    if ( ConditionValid == FALSE )
    {
     /* !LINKSTO NvM.MultiCore.CancelJobs_MultiCoreLock_DET,1 */
      NVM_DET_REPORT_ERROR( NVM_CANCEL_JOBS_API_ID, NVM_E_BLOCK_PENDING );
    }
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
#else
    ConditionValid = TRUE;
#if (NVM_DEV_ERROR_DETECT == STD_OFF)
    TS_PARAM_UNUSED(BlockId);
#endif/*NVM_DEV_ERROR_DETECT == STD_OFF*/
#endif /*defined(NVM_MULTICORE_ENABLED)*/
  }
  if(ConditionValid == FALSE)
  {
    RetBlockIndex = NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS;
  }
  return RetBlockIndex;
}

#if (defined(NVM_CANCELJOBS_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_CANCELJOBS_CALLOUT */

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)

/*======================NvM_CheckCondInvalidateNvBlock==========================*/

#if (defined(NVM_INVALIDATENVBLOCK_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_INVALIDATENVBLOCK_CALLOUT */

FUNC(uint16,NVM_CODE) NvM_CheckCondInvalidateNvBlock
(
  NvM_BlockIdType BlockId
)
{
  uint16 RetBlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
  boolean ConditionValid = FALSE;

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  /* Check if Block Id is within valid range */
  if ( RetBlockIndex == NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    NVM_DET_REPORT_ERROR( NVM_INVALIDATE_NV_BLOCK_API_ID, NVM_E_PARAM_BLOCK_ID );
  }
  /* Check if module is initialized by NvM_Init() */
  else if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    NVM_DET_REPORT_ERROR( NVM_INVALIDATE_NV_BLOCK_API_ID, NVM_E_NOT_INITIALIZED );
  }
  /* Check if an other request for block RetBlockIndex is already pending. */
  else if ( NvM_AdminBlockTable[RetBlockIndex].NvMResult == NVM_REQ_PENDING )
  {
    NVM_DET_REPORT_ERROR( NVM_INVALIDATE_NV_BLOCK_API_ID, NVM_E_BLOCK_PENDING );
  }
  /* !LINKSTO SWS_NvM_00756,1 */
  else if (NVM_CHECK_DYN_STATUS(RetBlockIndex, NVM_DYN_LOCK_MASK))
  {
    NVM_DET_REPORT_ERROR(NVM_INVALIDATE_NV_BLOCK_API_ID, NVM_E_BLOCK_LOCKED);
  }
  else
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
#if ( defined(NVM_MULTICORE_ENABLED) )
#if (defined(NVM_INVALIDATENVBLOCK_CALLOUT))
    if(E_OK == NvM_Queue_CheckFullStandard(RetBlockIndex))
    {
      ConditionValid = TRUE;
    }
    if ( ConditionValid == TRUE )
#endif /* #if (defined(NVM_INVALIDATENVBLOCK_CALLOUT)) */
    {
      /*Sets the multicore flag if not already set, returns FALSE if can't set*/
      ConditionValid = NvM_CheckSetMcFlag(RetBlockIndex);
#if (NVM_DEV_ERROR_DETECT == STD_ON)
      if ( ConditionValid == FALSE )
      {
        /* !LINKSTO NvM.MultiCore.InvalidateNvBlock_MultiCoreLock_DET,1 */
        NVM_DET_REPORT_ERROR( NVM_INVALIDATE_NV_BLOCK_API_ID, NVM_E_BLOCK_PENDING );
      }
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
    }
#else
    ConditionValid = TRUE;
#if (NVM_DEV_ERROR_DETECT == STD_OFF)
    TS_PARAM_UNUSED(BlockId);
#endif/*NVM_DEV_ERROR_DETECT == STD_OFF*/
#endif /*defined(NVM_MULTICORE_ENABLED)*/
  }
  if(ConditionValid == FALSE)
  {
    RetBlockIndex = NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS;
  }
  return RetBlockIndex;
}

#if (defined(NVM_INVALIDATENVBLOCK_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_INVALIDATENVBLOCK_CALLOUT */

/*======================NvM_CheckCondEraseNvBlock==========================*/

#if (defined(NVM_ERASENVBLOCK_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_ERASENVBLOCK_CALLOUT */

FUNC(uint16,NVM_CODE) NvM_CheckCondEraseNvBlock
(
  NvM_BlockIdType BlockId
)
{
  uint16 RetBlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
/* Compile only if ConditionValid is used */
  #if (( NVM_DEV_ERROR_DETECT == STD_ON )||\
  (( NVM_JOB_PRIORITIZATION == STD_ON )&&(defined(NVM_MULTICORE_ENABLED))))
  boolean ConditionValid = TRUE;
  #endif

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  /* Check if Block Id is within valid range */
  if ( RetBlockIndex == NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    ConditionValid = FALSE;
    NVM_DET_REPORT_ERROR( NVM_ERASE_NV_BLOCK_API_ID, NVM_E_PARAM_BLOCK_ID );
  }
  /* Check if module is initialized by NvM_Init() */
  else if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    ConditionValid = FALSE;
    NVM_DET_REPORT_ERROR( NVM_ERASE_NV_BLOCK_API_ID, NVM_E_NOT_INITIALIZED );
  }
  /* Check if an other request for block RetBlockIndex is already pending. */
  else if ( NvM_AdminBlockTable[RetBlockIndex].NvMResult == NVM_REQ_PENDING )
  {
    ConditionValid = FALSE;
    NVM_DET_REPORT_ERROR( NVM_ERASE_NV_BLOCK_API_ID, NVM_E_BLOCK_PENDING );
  }
  /* !LINKSTO SWS_NvM_00757,1 */
  else if (NVM_CHECK_DYN_STATUS(RetBlockIndex, NVM_DYN_LOCK_MASK))
  {
    ConditionValid = FALSE;
    NVM_DET_REPORT_ERROR(NVM_ERASE_NV_BLOCK_API_ID, NVM_E_BLOCK_LOCKED);
  }
#if ( NVM_JOB_PRIORITIZATION == STD_ON )
  /* Check if the requested block has immediate priority */
  else if ( NvM_BlockDescriptorTable[RetBlockIndex].blockJobPriority != NVM_BLOCK_IMMEDIATE_PRIORITY )
  {
    ConditionValid = FALSE;
    /* The requested block has no immediate priority */
    NVM_DET_REPORT_ERROR( NVM_ERASE_NV_BLOCK_API_ID, NVM_E_BLOCK_CONFIG );
  }
  else
#else
  else
  {
    ConditionValid = FALSE;
    /* The requested block has no immediate priority */
    NVM_DET_REPORT_ERROR( NVM_ERASE_NV_BLOCK_API_ID, NVM_E_BLOCK_CONFIG );
  }
#endif /* NVM_JOB_PRIORITIZATION == STD_ON */
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
#if ( NVM_JOB_PRIORITIZATION == STD_ON )
#if ( defined(NVM_MULTICORE_ENABLED) )
#if (defined(NVM_ERASENVBLOCK_CALLOUT))
    if(E_NOT_OK == NvM_Queue_CheckFullStandard(RetBlockIndex))
    {
      ConditionValid = FALSE;
    }
    if ( ConditionValid == TRUE )
#endif /* #if (defined(NVM_ERASENVBLOCK_CALLOUT)) */
    {
      /*Sets the multicore flag if not already set, returns FALSE if can't set*/
      ConditionValid = NvM_CheckSetMcFlag(RetBlockIndex);
#if (NVM_DEV_ERROR_DETECT == STD_ON)
      if ( ConditionValid == FALSE )
      {
        /* !LINKSTO NvM.MultiCore.EraseNvBlock_MultiCoreLock_DET,1 */
        NVM_DET_REPORT_ERROR( NVM_ERASE_NV_BLOCK_API_ID, NVM_E_BLOCK_PENDING );
      }
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
    }
#elif (NVM_DEV_ERROR_DETECT == STD_OFF)/*defined(NVM_MULTICORE_ENABLED)*/
    TS_PARAM_UNUSED(BlockId);
#endif /* defined(NVM_MULTICORE_ENABLED) */
#elif (NVM_DEV_ERROR_DETECT == STD_OFF) /* NVM_JOB_PRIORITIZATION == STD_ON */
    TS_PARAM_UNUSED(BlockId);
#endif /* ( NVM_JOB_PRIORITIZATION == STD_ON ) */
  }
/* Compile only if ConditionValid is used */
  #if (( NVM_DEV_ERROR_DETECT == STD_ON )||\
  (( NVM_JOB_PRIORITIZATION == STD_ON )&&(defined(NVM_MULTICORE_ENABLED))))
  /* Deviation TASKING-1 */
  if(ConditionValid == FALSE)
  {
    RetBlockIndex = NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS;
  }
#endif
  return RetBlockIndex;
}

#if (defined(NVM_ERASENVBLOCK_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_ERASENVBLOCK_CALLOUT */

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */

#if (NVM_CRYPTO_HOOKS == STD_ON)
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
/*======================NvM_CryptoOperations_Async_State==========================*/
FUNC(void,NVM_CODE) NvM_CryptoOperations_Async_State(void)
{
  uint16 Length = NvM_GetBlockLength( NvM_CurrentBlockDescriptorPtr->blockDesc,
                                      NvM_CurrentBlockDescriptorPtr->nvBlockLength) +
                 (uint16)NvM_GetBlockCRCLength(NvM_CurrentBlockDescriptorPtr->blockDesc);/*maybe this can be optimized later to be done once before function call and saved somewhere*/
  MemIf_JobResultType CryptoResult;
  boolean CryptoError = FALSE;

  if(NvM_PostCryptoState == NvM_WriteState)
  {
    /* !LINKSTO NvM.CryptoHooks.CryptoWriteOperation,1 */
    CryptoResult = NVM_CRYPTO_WRITE_HOOK(NvM_CurrentBlockDescriptorPtr->nvBlockIdentifier,NvM_GetBufferAddress(),Length,NvM_CurrentBlockDescriptorPtr->CryptoExtraInfoSize,FirstCryptoExecution);
  }
  else
  {
    /* !LINKSTO NvM.CryptoHooks.CryptoReadOperation,1 */
    CryptoResult = NVM_CRYPTO_READ_HOOK(NvM_CurrentBlockDescriptorPtr->nvBlockIdentifier,NvM_GetBufferAddress(),Length,NvM_CurrentBlockDescriptorPtr->CryptoExtraInfoSize,FirstCryptoExecution);
  }

  FirstCryptoExecution = FALSE;
  if (CryptoResult == MEMIF_JOB_OK)
  {
    FirstCryptoExecution = TRUE;
    CryptoTimeoutCounter = 0;
    if(NvM_PostCryptoState == NvM_WriteState)
    {
      NvM_WriteBlock_ProcessRedundantBlock(&NvM_WriteBlock_Async_State3);
    }
    else
    {
      NvM_GlobalErrorStatus = NVM_REQ_OK;
    /*In case a Read state then just terminate current level*/
      NvM_StateMachine_TerminateCurrentLevel();
    }
  }
  else if ( CryptoResult == MEMIF_JOB_PENDING)
  {
    CryptoTimeoutCounter++;
    /* !LINKSTO NvM.CryptoHooks.HookTimeout,1 */
    if(CryptoTimeoutCounter < NVM_CRYPTO_TIMEOUT)
    {
      NvM_GlobalErrorStatus = NVM_REQ_PENDING;
    }
    else
    {
      CryptoError = TRUE;
    }

  }
  else
  {
    CryptoError = TRUE;
  }

  if(CryptoError == TRUE)
  {
    FirstCryptoExecution = TRUE;
    CryptoTimeoutCounter = 0;
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
    /* !LINKSTO NvM.CryptoHooks.ReadDet,1 */
    /* !LINKSTO NvM.CryptoHooks.WriteDet,1 */
    NVM_DET_REPORT_ERROR( NVM_CRYPTO_HOOKS_API_ID, NVM_E_CRYPTO_FAILED );
#endif
    NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;

    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_CryptoOperations_Async_Error;
  }
}/* end of function NvM_CryptoOperations_Async_State */

/*======================NvM_CryptoOperations_Async_Error==========================*/
FUNC(void,NVM_CODE) NvM_CryptoOperations_Async_Error(void)
{
  if(NvM_PostCryptoState == NvM_ReadState)
  {
    /* !LINKSTO NvM.CryptoHooks.CryptoFailDuringRead,1 */
    /* !LINKSTO NvM.CryptoHooks.CryptoFailDuringReadAll,1 */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_Retry;
  }
  /* !LINKSTO NvM.CryptoHooks.CryptoFailDuringWrite,1 */
  else if(NvM_CurrentServiceId == NVM_WRITE_ALL_API_ID)
  {
    /* !LINKSTO NvM.CryptoHooks.CryptoFailDuringWriteAll,1 */
    NvM_GlobalCallLevel = 1U;
  }
  else
  {
    NvM_GlobalCallLevel = 0U;
  }
}/* end of function NvM_CryptoOperations_Async_Error */
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* #if (NVM_CRYPTO_HOOKS == STD_ON) */

/*==================[internal function definitions]==========================*/
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
STATIC FUNC(void,NVM_CODE) NvM_FatalError(void)
{
  /* terminate complete request */
  /* terminate current job */
  NvM_GlobalCallLevel = 0x0U;
  /* set job as failed */
  NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;
}
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
/*==================[end of file]============================================*/
