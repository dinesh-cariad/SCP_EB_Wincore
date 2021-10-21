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

/* !LINKSTO NVM076,1 */
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

#include <TSMem.h>                /* Autosar Module  Base */
/* !LINKSTO SWS_NvM_00554,2 */
#include <MemIf.h>                /* for MemIf_SetMode() */

/* !LINKSTO SWS_NvM_00554,2 */
#include <NvM.h>                  /* Extern NVRAM Manager API definitions   */

#include <NvM_FunctionIntern.h>   /* NvM functions which are used           */
                                  /* only within the NvM module.            */
#include <NvM_DataIntern.h>       /* NvM data which is used only            */
                                  /* within the NvM module.                 */
#include <NvM_Queue.h>            /* queue external definitions.            */
#include <NvM_CalcCrc.h>
#if ( NVM_USE_COMPILED_CONFIGID == STD_OFF )
#include <NvM_UsrHdr.h>           /* indirect incl. of usr supplied header  */
#endif

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
/* !LINKSTO NVM556,1 */
#include <Det.h>                  /* API of module Det.                     */
#endif

/* if any DEM event is switched on, include Dem.h */
#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DEM)
/* !LINKSTO SWS_NvM_00554,2 */
#include <Dem.h>
#endif

/* if multi block job status reporting is switched on, include BswM_NvM.h */
#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_ON)
#include <BswM_NvM.h>
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

/** \brief Forces a block to be written on request NvM_WriteAll()
 **
 **        The status bits of a block are manipulated so that this block is written
 **        on the next call of NvM_WriteAll().
 **/
STATIC FUNC(void,NVM_CODE) NvM_ForceForWriteAll(void);

/** \brief Evaluates Configuration Id which is read from NV memory.
 **/
STATIC FUNC(void,NVM_CODE) NvM_ReadAll_Async_State1(void);

#if (NVM_NUMBER_OF_SELECT_BLOCK_FOR_READALL_BLOCKS > 0U) && \
     (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) &&\
     (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U)

/** \brief Intermediate state to trigger CRC re-calculation on the existing data.
           For blocks with permanent RAM the existing RAM data is used.For blocks
           with explicit sync, the callback is called to copy the data to internal
           buffer.If the explicit sync functions return a negative response and if
           NVM_REPEAT_MIRROR_OPERATIONS is configured, then the explicit sync will
           be retry for the configured number of time.

 **/
STATIC FUNC(void,NVM_CODE) NvM_ReadAll_ReCalcCrc(void);
#endif

/** \brief Third state of the asynchronous state machine of API function
 **        NvM_ReadAll. Finishes read operation of current block.
 **/
STATIC FUNC(void,NVM_CODE) NvM_ReadAll_Async_State6(void);

#if ((NVM_NUMBER_OF_SELECT_BLOCK_FOR_READALL_BLOCKS > 0U) && \
    (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) &&\
    (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
/** \brief Compares calculated CRC vs. CRC stored in RAM Block.
 **/
STATIC FUNC(void,NVM_CODE) NvM_ReadAll_Async_State7(void);
#endif

#if ( NVM_NUMBER_OF_SELECT_BLOCK_FOR_READALL_BLOCKS > 0U )
/** \brief Checks if ROM default data or NVM data must be copied
 **        to the RAM Block and starts copy process.
 **/
STATIC FUNC(void,NVM_CODE) NvM_ReadAll_Async_State8(void);

#endif /* NVM_NUMBER_OF_SELECT_BLOCK_FOR_READALL_BLOCKS > 0U */

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#if (NVM_NUMBER_OF_SELECT_BLOCK_FOR_READALL_BLOCKS > 0U) && \
     (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) &&\
     (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U)

STATIC FUNC(void,NVM_CODE) NvM_ReadAll_ReCalcCrc(void)
{
#if (NVM_SYNC_MECHANISM == STD_ON)
  /* Result of the mirror copy callback */
  Std_ReturnType ReturnVal = E_OK;
#endif
  /* local variable to store pointer to user data */
  NvM_PtrToApplDataType BufferAddr;

  /* get the data address */
  BufferAddr = NvM_GetUserRamBlockAddress();

  /* Set global error status to NVM_REQ_NOT_OK */
  NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;

  /* if explicit synch is enabled call Mirror callback to copy
     the data from NvM user */
#if (NVM_SYNC_MECHANISM == STD_ON)
  if (NVM_CHECK_WORKING_STATUS(NVM_DYN_MIRROR_MASK))
  {
    /* !LINKSTO NvM.SetRamBlockStatus.ExplicitSynch.CrcCalculation,1 */
    ReturnVal = NvM_MirrorCopy(NULL_PTR,
          NvM_CurrentBlockDescriptorPtr->writeRamToNvCallback, BufferAddr);
  }

  if (E_OK != ReturnVal)
  {
    if (NvM_MirrorRetryCount > 0)
    {
      /* Update NvM_CurrentFunctionPtr for next mirror operation retry */
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel]
          = &NvM_ReadAll_ReCalcCrc;
    }
    else
    {
      /*Data could not be retrieved so we consider the situation like a Read failure.*/
      NvM_GlobalCallLevel++;
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadAll_Async_State5;
      /* Set block result status and dynamic status*/
      NvM_PostService_Function();
    }
  }
  else
#endif
  {
    /* Mirror Operation OK - update NvM_CurrentFunctionPtr */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadAll_Async_State7;
    /* Call CRC calculation function */
    NvM_CalculateCrc(BufferAddr);
  }
}
#endif /* NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U */

FUNC(void,NVM_CODE) NvM_ReadAll(void)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_NVM_READALL_ENTRY();

/*                        ( NVM_DEV_ERROR_DETECT == STD_ON )                    */
#if ( NVM_DEV_ERROR_DETECT == STD_ON )

  /* Check if module is initialized by NvM_Init() */
  if ( NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE( NVM_GENERIC_INIT_FLAG_MASK ) )
  {
    NVM_DET_REPORT_ERROR( NVM_READ_ALL_API_ID, NVM_E_NOT_INITIALIZED );
  }
  else if ( NvM_AdminBlockTable[0U].NvMResult == NVM_REQ_PENDING )
  {
    NVM_DET_REPORT_ERROR( NVM_READ_ALL_API_ID, NVM_E_BLOCK_PENDING );
  }
  else
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
    /* protect concurrent access to the standard queue */
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

    if (NvM_AdminBlockTable[0U].NvMResult != NVM_REQ_PENDING)
    {
      /* Insert the request in the queue.
       * BlockId 0 for Multi Block Request; NULL_PTR as data address
       */
      Result = NvM_Queue_InsertStandard(0U, NULL_PTR, &NvM_ReadAll_Async);
    }
    if (E_OK == Result)
    {
      /* Insert the API ID into NvM_AdminBlockTable.
       * The information shall be available in case the request has been interrupted
       * by an immediate write request.
       */
      NvM_AdminBlockTable[0U].NvMCurrentServiceId = NVM_READ_ALL_API_ID;
    }

    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();

    /* Check if standard queue is already full */
    if (E_OK != Result)
    {
#if (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DEM)
#if (NVM_QUEUE_OVERFLOW_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID )
      NVM_DEM_REPORT_ERROR(NVM_QUEUE_OVERFLOW_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_DET)
      NVM_DET_REPORT_ERROR(NVM_READ_ALL_API_ID, NVM_E_DEMTODET_QUEUE_OVERFLOW);
#elif (NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW == TS_PROD_ERR_REP_TO_USER)
      NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NVM_BLOCK_MULTI, NVM_READ_ALL_API_ID, NVM_E_USER_CALLOUT_QUEUE_OVERFLOW);
#endif
    }

    /* Check if read-all queued */
    if (E_OK == Result)
    {
#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO == STD_ON)
      /* ReadAll request staus has changed, report the request status to BswM */
      BswM_NvM_CurrentJobMode(NVM_READ_ALL_API_ID, NVM_REQ_PENDING);
#endif
    }
  }

  DBG_NVM_READALL_EXIT();
}

/*==================[internal function definitions]==========================*/

STATIC FUNC(void,NVM_CODE) NvM_ForceForWriteAll(void)
{
  /* set bit 1 to 0: WriteProtectionFlag  ( WPF )           */
  /* set bit 5 to 0: WriteProtectionPermanentFlag ( WPPF )  */
  NvM_GlobalWorkingStatus &=
    ( NvM_DynamicStatusType )(~( NvM_DynamicStatusType )
      ( NVM_DYN_WPF_MASK|NVM_DYN_WPPF_MASK ) );

  /* set bit 6 to 1: ExtentedRuntimeBlockFlag ( ERBF )     */

  DBG_NVM_GLOBALWORKINGSTATUS((NvM_GlobalWorkingStatus),
                              ((NvM_GlobalWorkingStatus)|(NVM_ERBF_MASK)));
  NvM_GlobalWorkingStatus |= NVM_ERBF_MASK;

  /* set block to valid and changed */
  NvM_SetPermanentRamBlockValid();
  NvM_SetPermanentRamBlockChanged();

  /* NOTE: the high byte of 'NvM_GlobalWorkingStatus gets lost. The  */
  /*       high byte contains information which is only required     */
  /*       while processing the request und need not be stored       */
  /*       permanently in the 'NvM_AdminBlockTable'.                 */
  NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMDynStatus
   = NvM_GlobalWorkingStatus;
}


FUNC(void,NVM_CODE) NvM_ReadAll_Async(void)
{

  /* Since the processing of NvM_ReadAll() has
   * been started, NvM_CurrentServiceId needs to be set
   */
  NvM_CurrentServiceId = NVM_READ_ALL_API_ID;

  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadAll_Async_State0;
}

FUNC(void,NVM_CODE) NvM_ReadAll_Async_State0(void)
{
  /* Restore the function pointer and block number in the queue entry because
   * NvM_ReadAll() might have been interrupted by an immediate write request.
   */
  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
  NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMRequestAsyncPtr = &NvM_ReadAll_Async;
  NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMBlockDescriptorIndex = NVM_BLOCK_MULTI;
  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
  /* Switch to fast mode if configured */
#if ( NVM_DRV_MODE_SWITCH == STD_ON )

  MemIf_SetMode( MEMIF_MODE_FAST );

#endif

  /* Read Configuration Id from block 1 */
  NvM_GlobalBlockNumber = NVM_BLOCK_CONFIG_ID;
  /* !LINKSTO NvM.NvMSoftwareChangeCallout.Activation,1 */
#if ( NVM_USE_COMPILED_CONFIGID == STD_OFF )
  /* There is no compiled Configuration Id used. User must provide one. */
  TS_MemCpy( &NvM_CompiledConfigurationId[0],
               NVM_SOFTWARE_CHANGE_CALLOUT(),
               NVM_CONFIGURATIONID_SIZE
             );
#endif

  /* NvM_PreService_Function must be called after the NvM_GlobalBlockNumber
   * is set. Else the initialization of the global variables is incorrect. */
  NvM_PreService_Function();

  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadAll_Async_State1;

  DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),((NvM_GlobalCallLevel)+1U));
  NvM_GlobalCallLevel++;
  /* Set the status of Block 1 in  NvM_AdminBlockTable to pending */
  NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NVM_REQ_PENDING;
  /* Queue the read request for block 1 */
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State0;
}

STATIC FUNC(void,NVM_CODE) NvM_ReadAll_Async_State1(void)
{
  /* Evaluate result of Configuration Id read operation */

  boolean RestoreBlockDefaultsIndicator = FALSE;

  /* check if block 1 is invalid */
  if ( (NvM_GlobalErrorStatus == NVM_REQ_NV_INVALIDATED ) ||
      /* or if the block is empty - no CRC error has been detected in NvM */
      ((NvM_GlobalErrorStatus == NVM_REQ_INTEGRITY_FAILED) &&
          (!NVM_CHECK_WORKING_STATUS(NVM_DYN_CCU_MASK))))
  {
    /* !LINKSTO NVM673,1 */
    /* !LINKSTO NvM.NvMSoftwareChangeCallout.SWS_NvM_00673,1 */
    /* According to NVM673, in case Dynamic Configuration is enabled and the Configuration ID
     * block is reported as INVALIDATED or empty, Block1 RAM is updated with the new compiled configuration ID,
     and normal runtime preparation will be performed
     */
    RestoreBlockDefaultsIndicator = TRUE;
    NVM_CLEAR_GLOBAL_GENERIC_STATUS( NVM_GENERIC_DC_MASK );
    DBG_NVM_GLOBALGENERICSTATUS((NvM_GlobalGenericStatus),
        (NvM_GlobalGenericStatus & (uint16)((uint16)(~(NVM_GENERIC_DC_MASK)) & 0xFFFFU)));
  }
  else
  {

    if ((NvM_GlobalErrorStatus == NVM_REQ_OK) ||
        (NvM_GlobalErrorStatus == NVM_REQ_REDUNDANCY_FAILED))
    {
      /* !LINKSTO NVM246,1 */
      /* !LINKSTO NvM.NvMSoftwareChangeCallout.SWS_NvM_00246,1 */
      /* !LINKSTO NVM073,1 */
      /* !LINKSTO NvM.NvMSoftwareChangeCallout.SWS_NvM_00073,1 */
      /* Compare read Configuration Id versus Compiled Configuration Id or versus user's Configuration Id
      returned by the callout.*/
      /*(const uint8*)&NvM_CompiledConfigurationId this is either the address of an array either the address of a variable*/
      if (E_NOT_OK == TS_MemCmp(&NvM_ConfigurationId[0], (const uint8*)&NvM_CompiledConfigurationId, NVM_CONFIGURATIONID_SIZE))
      {
        /* The Configuration Ids does NOT match --> set error/status to
         * to NVM_REQ_NOT_OK ( NVM307 )
         */

        DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_NOT_OK));
        NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;
#if ( NVM_DYNAMIC_CONFIGURATION != STD_ON )
        /* NVM248: Configuration ID mismatch and
         *         NvM_DynamicConfiguration == FALSE --> proceed with
         *         normal behaviour --> don't write Compiled
         *         Configuration Id automatically --> set block 1
         *         to unchanged.
         */
        NvM_SetPermanentRamBlockUnchanged();
#endif  /* NVM_DYNAMIC_CONFIGURATION != STD_ON */
      }
      else
      {
        /* NVM313: The Configuration Ids are equal --> the Compiled
         *         Configuration Id must not be written --> set block to
         *         unchanged ( NVM313 )
         */
        NvM_SetPermanentRamBlockUnchanged();
      }

    } /* if ( NvM_GlobalErrorStatus == NVM_REQ_OK ) */
    else
    {
      /* NVM305: Configuration cannot be read because of an error
       *         was detetected by the underlying SW layers.
       */

      DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_INTEGRITY_FAILED));
      NvM_GlobalErrorStatus = NVM_REQ_INTEGRITY_FAILED;
    }
#if ( NVM_DYNAMIC_CONFIGURATION == STD_ON )
    if ((NvM_GlobalErrorStatus != NVM_REQ_OK) &&
        (NvM_GlobalErrorStatus != NVM_REQ_REDUNDANCY_FAILED))
    {

      /* Indicate that dynamic configuration must be applied to all
       * blocks processed by NvM_ReadAll.
       */
      NVM_SET_GLOBAL_GENERIC_STATUS( NVM_GENERIC_DC_MASK );
      DBG_NVM_GLOBALGENERICSTATUS((NvM_GlobalGenericStatus),
          (NvM_GlobalGenericStatus | NVM_GENERIC_DC_MASK));
    /* By setting the variable on TRUE NvM will update the configuration ID RAM block
    according to the new configuration ID, and mark the block for WriteAll */
    /* !LINKSTO NVM310,1 */
    /* !LINKSTO NvM.NvMSoftwareChangeCallout.SWS_NvM_00310,1 */
      RestoreBlockDefaultsIndicator = TRUE;
    }
    else
#endif
    {
       /* Indicate that normal processing of all blocks must be
        * applied.
        */
       NVM_CLEAR_GLOBAL_GENERIC_STATUS( NVM_GENERIC_DC_MASK );
       DBG_NVM_GLOBALGENERICSTATUS((NvM_GlobalGenericStatus),
              (NvM_GlobalGenericStatus & (uint16)((uint16)(~(NVM_GENERIC_DC_MASK)) & 0xFFFFU)));
    }

  }/* else - if ( NvM_GlobalErrorStatus == NVM_REQ_NV_INVALIDATED ) */

  /* Write error status of Configuration Id block */
  NvM_PostService_Function();

#if ( NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK > 0U )
  /* !LINKSTO NVM176,1 */
  /* Invoke block specific callback function */
  NvM_InvokeSingleBlockCallback(NvM_GlobalBlockNumber,
                                NvM_CurrentServiceId,
                                NvM_GlobalErrorStatus);
#endif

  /* When an error occured while processing the ConfigurationId Block the
   * the multi block job result must be set to NVM_REQ_NOT_OK ( NVM301 ).
   */
#if (NVM_RESULT_ERASED_BLOCKS == NVM_ERASED_BLOCK_INCONSISTENT)
  if ( ( NvM_GlobalErrorStatus != NVM_REQ_OK ) &&
       ( NvM_GlobalErrorStatus != NVM_REQ_REDUNDANCY_FAILED ) &&
       ( NvM_GlobalErrorStatus != NVM_REQ_NV_INVALIDATED ) &&
       ( ( NvM_GlobalErrorStatus != NVM_REQ_INTEGRITY_FAILED ) ||
         ( NVM_CHECK_WORKING_STATUS(NVM_DYN_CCU_MASK)) )
     )
#else
  if ( ( NvM_GlobalErrorStatus != NVM_REQ_OK ) &&
       ( NvM_GlobalErrorStatus != NVM_REQ_REDUNDANCY_FAILED ) &&
       ( ( NvM_GlobalErrorStatus != NVM_REQ_INTEGRITY_FAILED ) ||
         ( NVM_CHECK_WORKING_STATUS(NVM_DYN_CCU_MASK)) )
     )
#endif
  {
    NVM_REQUEST_INTERIM_RESULT = NVM_REQ_NOT_OK;
  }
  else
  {
    NVM_REQUEST_INTERIM_RESULT = NVM_REQ_OK;
  }
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadAll_Async_State5;
  if ( RestoreBlockDefaultsIndicator == TRUE )
  {
    /* Copy Compiled Configuration Id to Configuration Id RAM buffer
     * and set valid and changed bits so that it is written by
     * next NvM_WriteAll().
     */
    NvM_ForceForWriteAll();

    DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),((NvM_GlobalCallLevel)+1U));
    NvM_GlobalCallLevel++;
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_RestoreBlockDefaults_AscSt0;
  }
}

FUNC(void,NVM_CODE) NvM_ReadAll_Async_State5(void)
{
  NvM_BlockIdType TempBlockId;

  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
  TempBlockId = NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMBlockDescriptorIndex;
  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
  /* check if there are more blocks to process */
  if ( NvM_GlobalBlockNumber < ( NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS - 1U ) )
  {
    /* In case NvM_ReadAll() was interrupted by an immediate write job, the multi-block
     * request shall be resumed from the current block number. Otherwise NvM_ReadAll()
     * must proceed with the next block.
     */
    if (TempBlockId == NVM_BLOCK_MULTI)
    {
      /* proceed with next block */
      NvM_GlobalBlockNumber++;
    }

    /* NOTE: NvM_PreService_Function() must be called before
     *       NVM_CHECK_CONST_BLOCK_STATUS is used else the
     *       BlockDescriptorPointer is not set correctly.
     */
    NvM_PreService_Function();

    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadAll_Async_State6;

#if ( NVM_NUMBER_OF_SELECT_BLOCK_FOR_READALL_BLOCKS > 0U )

    /* Process only blocks who have a permanent RAM block or explicit
     * synchronization mechanism enabled (SWS_NvM_00356, SWS_NvM_00718),
     * and configuration parameter 'SelectBlockForReadAll' is enabled
     * ( SWS_NvM_00118 ).
     * All block management types are processed.
     */
#if (NVM_SYNC_MECHANISM == STD_ON)
    if (
          ((NvM_CurrentBlockDescriptorPtr->ramBlockDataAddress != NULL_PTR)
            ||
           (NVM_BD_USEMIRROR(NvM_CurrentBlockDescriptorPtr->blockDesc))
          ) &&
         (NVM_BD_READALL(NvM_CurrentBlockDescriptorPtr->blockDesc))
       )
#else
    if (
          ((NvM_CurrentBlockDescriptorPtr->ramBlockDataAddress != NULL_PTR)
          ) &&
          (NVM_BD_READALL(NvM_CurrentBlockDescriptorPtr->blockDesc))
       )
#endif /* NVM_SYNC_MECHANISM = STD_ON */
    {
      /* Check if another job is pending or cancelled for the processing
       * block ID and set the Multi Request Keep Job Pending flag and
       * Block request Canceled Flag accordingly. */
      NvM_CheckBlockStatus();

      /* Status of currently processed block must be set to
       * NVM_REQ_PENDING.
       */
      NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMResult = NVM_REQ_PENDING;
      NvM_GlobalErrorStatus = NVM_REQ_NOT_OK;

#if ((NVM_DYNAMIC_CONFIGURATION == STD_ON) && (NVM_NUMBER_OF_NON_RESISTANT_TO_CHANGED_SW_BLOCKS > 0U))
        /* !LINKSTO NVM249,1 */
        /*  If the block is configured with NvMResistantToChangedSw = FALSE and
         *  NvMDynamicConfiguration is set to TRUE extended runtime preparation
         *  shall be processed
         */
        if ( ( NVM_BD_RESISTANTTOCHANGE_BIT_FALSE(NvM_CurrentBlockDescriptorPtr->blockDesc) ) &&
             ( NVM_CHECK_GLOBAL_GENERIC_STATUS( NVM_GENERIC_DC_MASK ) )
            )
        {
          NvM_SetPermanentRamBlockInvalid();
        }
        else
        {
          /* !LINKSTO NVM247,1*/
          /* Normal runtime preparation shall be processed. */
        }
#endif  /* (NVM_DYNAMIC_CONFIGURATION == STD_ON) &&
           (NVM_NUMBER_OF_NON_RESISTANT_TO_CHANGED_SW_BLOCKS > 0U)*/

#if (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON)
      /* !LINKSTO SWS_NvM_00128,1, NVM133,1, SWS_NvM_00345,1 */
      if ((NVM_BD_USERAMBLOCKSTATUS(NvM_CurrentBlockDescriptorPtr->blockDesc) == FALSE) ||
          (NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMValid != NVM_BLOCK_VALID ))
      {
        /* Current block RAMBlockStatus is INVALID
         * --> copy data from NVM Block or ROM Block ( SWS_NvM_00362,NVM363 ).
         */
        NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadAll_Async_State8;

      }/* if ( NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMValid .. */
      else
      {
#if (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U)
         if ((NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMExtendedStatus &
                NVM_ES_RAM_BLOCK_CRC_CALC_MASK) == NVM_ES_RAM_BLOCK_CRC_CALC_MASK)
         {
           /* Reset the RAM block CRC calculation flag */
           NvM_CalcCrc_RemoveElement();
           /* Ram block CRC will be recalculated again */
         }

        /* Current block RAMBlockStatus is VALID
         * --> Check if CRC must be recalculated and checked
         */
        if(NVM_BD_CALCRAMBLOCKCRC(NvM_CurrentBlockDescriptorPtr->blockDesc))
        {
          /* parameter NVM_CALC_RAM_BLOCK_CRC is TRUE.
           * --> recalculate CRC and verify with CRC stored in block
           *     specific RAM CRC buffer ( SWS_NvM_00362 ).
           */
          NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadAll_ReCalcCrc;

        }/* if ( NVM_CHECK_CONST_BLOCK_STATUS( NVM_CALCRAMBLOCKCRC_MASK ) ) */
        else
        {
          /* RAM Block Status is valid and CRC need not to be
           * recalculated
           * --> The NvM data must not be loaded by NvM_ReadAll()
           * --> Set specific block result to NVM_REQ_BLOCK_SKIPPED
           *     ( NVM287 ).
           */
          NvM_GlobalErrorStatus = NVM_REQ_BLOCK_SKIPPED;
        }/* else-if ( NVM_CHECK_CONST_BLOCK_STATUS( NVM_CALCRAMBLOCKCRC_MASK ) ) */
#endif/* NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U*/
      }/* else-if ( NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMValid */
#else
      /* !LINKSTO SWS_NvM_00345,1 */
      /* NvM_SetRamBlockStatus() API is disabled, NvM shall load NV data for every block
       * configured to be processed during NvM_ReadAll(), regardless of it's RAM block status
       */
      NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadAll_Async_State8;
#endif /* NVM_SET_RAM_BLOCK_STATUS_API == STD_ON */

    }/* end of if ( current block is processed ) */
    else
#endif /* NVM_NUMBER_OF_SELECT_BLOCK_FOR_READALL_BLOCKS > 0U */
    {
      /* Check if another job is pending or cancelled for the processing
       * block ID and set the Multi Request Keep Job Pending flag and
       * Block request Canceled Flag accordingly. */
      NvM_CheckBlockStatus();

      /* Current block must not be processed --> set error status
       * to NVM_REQ_BLOCK_SKIPPED
       */
      if (NVM_BD_READALL(NvM_CurrentBlockDescriptorPtr->blockDesc))
      {
        DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_BLOCK_SKIPPED));
        NvM_GlobalErrorStatus = NVM_REQ_BLOCK_SKIPPED;
      }
    }
  }/* if ( NvM_GlobalBlockNumber < ( NvM_TotalNumberOfNvramBlocks - 1 ) ) */
  else
  {
    /* All blocks finished */
    /* Set final result of multi block request */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQUEST_INTERIM_RESULT));
    NvM_GlobalErrorStatus = NVM_REQUEST_INTERIM_RESULT;
    NvM_MultiPostService_Function();
  }/* else-if ( NvM_GlobalBlockNumber < ( NvM_TotalNumberOfNvramBlocks - 1 ) ) */

  /* Restore the function pointer and block number in the queue entry because
   * NvM_ReadAll() might have been interrupted by an immediate write request.
   */
  SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
  NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMRequestAsyncPtr = &NvM_ReadAll_Async;
  NvM_Queue_Standard[NvM_Queue_IndexStandard].NvMBlockDescriptorIndex = NVM_BLOCK_MULTI;
  SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
}

STATIC FUNC(void,NVM_CODE) NvM_ReadAll_Async_State6(void)
{
  /* Set error status of finished block and internal management data. */
  if (NVM_BD_READALL(NvM_CurrentBlockDescriptorPtr->blockDesc))
  {
    NvM_MultiRequestFinishedSingleBlock( &NvM_ReadAll_Async_State5 );
  }
  else
  {
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
    if ( (!NVM_CHECK_GLOBAL_GENERIC_STATUS(NVM_GENERIC_CANCEL_MASK)) &&
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
      /* Nothing to do (MISRA) */
    }
    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
    /* set pointer to next state */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadAll_Async_State5;
  }
}

#if ((NVM_NUMBER_OF_SELECT_BLOCK_FOR_READALL_BLOCKS > 0U) && \
    (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) &&\
    (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))

STATIC FUNC(void,NVM_CODE) NvM_ReadAll_Async_State7(void)
{
  /* Verify calculated CRC vs. CRC stored in user RAM CRC Buffer */
  NvM_GlobalErrorStatus = NvM_CheckCrc( NvM_GetStoredRamCRCAddress(),
                         (NvM_PtrToApplDataType)(&NvM_CalcCrc_CalcBuffer),
                         NvM_CurrentBlockDescriptorPtr->blockDesc );

  if ( NvM_GlobalErrorStatus == NVM_REQ_OK )
  {
    /* CRC verification successfull
     * --> RAMBlockStatus is already valid and consistent
     * --> Don't copy NVM or ROM Block ( NVM364 ).
     */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadAll_Async_State6;
  }
  else
  {
    /* CRC verification failed
     * --> RAMBlockStatus is valid and inconsistent
     * --> Copy NVM or ROM Block ( NVM363 ).
     */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadAll_Async_State8;
  }/* if ( NvM_GlobalErrorStatus == NVM_REQ_OK ) */
}
#endif

#if (NVM_NUMBER_OF_SELECT_BLOCK_FOR_READALL_BLOCKS > 0U)
STATIC FUNC(void,NVM_CODE) NvM_ReadAll_Async_State8(void)
{
  /* Set return state of the NvM_ReadAll state machine */
  NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadAll_Async_State6;
  NvM_GlobalCallLevel++;

  /* Check whether NvM Dynamic Configuration is enabled and block
   * address has changed.
   */
#if ( NVM_NUMBER_OF_NON_RESISTANT_TO_CHANGED_SW_BLOCKS > 0U )
  if ( ( NVM_BD_RESISTANTTOCHANGE_BIT_FALSE(NvM_CurrentBlockDescriptorPtr->blockDesc) ) &&
       ( NVM_CHECK_GLOBAL_GENERIC_STATUS( NVM_GENERIC_DC_MASK ) )
     )
  {
    /* NvM Dynamic Configuration is enabled AND block address has
     * changed. --->
     * Extended runtime preparation. Copy block default data from
     * ROM to RAM using NvM_RestoreBlockDefaults(). Also mark block
     * to be written by next NvM_WriteAll().
     */
    NvM_ForceForWriteAll();
    /*Set WRITEONCE Read flag so it can be later written*/
#if (NVM_WRITE_BLOCK_ONCE == STD_ON)
    NVM_SET_WORKING_STATUS(NVM_DYN_WBORF_MASK);
#endif
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_RestoreBlockDefaults_AscSt0;
  }
  else
#endif /* NVM_NUMBER_OF_NON_RESISTANT_TO_CHANGED_SW_BLOCKS > 0U */
  {
    /* NvM Dynamic Configuration is disabled OR block address if fix
     * --->
     * Normal runtime preparation. Copy data from Non Volatile Memory
     * to RAM using NvM_ReadBlock().
     */
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_ReadBlock_Async_State0;
  }
}

#endif /* NVM_NUMBER_OF_SELECT_BLOCK_FOR_READALL_BLOCKS > 0U */

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[end of file]============================================*/
