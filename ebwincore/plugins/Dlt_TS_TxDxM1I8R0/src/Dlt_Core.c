/**
 * \file
 *
 * \brief AUTOSAR Dlt
 *
 * This file contains the implementation of the AUTOSAR
 * module Dlt.
 *
 * \version 1.8.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/* MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 *     "A conversion should not be performed from pointer to void into pointer to object."
 *
 *     Reason:
 *     The callbacks used to write into the NvM buffer are optimized to
 *     copy the data byte by byte. This conversion does not create alignment issues.
 */

/*==================[inclusions]============================================*/

#include <Dlt_Cfg.h>

#if (DLT_ENABLE_BSW_DISTRIBUTION == STD_ON)
#include <SchM_Dlt.h>                             /* SchM interface for Dlt */
#endif

#include <Dlt_UserCallouts.h>                 /* DLT user callout functions */
#include <Dlt_Cbk.h>
#include <Dlt_Trace.h>                        /* Dbg related macros for Dlt */

#if (DLT_INCLUDE_RTE == STD_ON) && (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431)
#include <Dlt_SessionMapping.h>
#endif /* (DLT_INCLUDE_RTE == STD_ON) && (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */

#include <Dlt_Int.h>                           /* Module internal interface */
#include <Dlt.h>                 /* Module public and internal declarations */

#include <PduR.h>
#include <PduR_Dlt.h>

#if (DLT_IMPLEMENT_NV_RAM_STORAGE == STD_ON)
#include <NvM.h>                                          /* NvM module APIs */
#endif
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
#include <TSMem.h>
#endif
/*==================[macros]================================================*/

#if(defined DLT_RESET_STORE_INDEX)
#error DLT_RESET_STORE_INDEX already defined
#endif
#define DLT_RESET_STORE_INDEX()                          do{ \
                                                            Dlt_AppToCtxRefIdx = 0U;\
                                                            Dlt_AppIdIdx = 0U;\
                                                          }while(0);

#if(defined DLT_NVM_STORED_DATA_UPDATED_BIT_MASK)
#error DLT_NVM_STORED_DATA_UPDATED_BIT_MASK already defined
#endif
#define DLT_NVM_STORED_DATA_UPDATED_BIT_MASK 0x01U

#if(defined DLT_NVM_OS_CORE_ID_BIT_MASK)
#error DLT_NVM_OS_CORE_ID_BIT_MASK already defined
#endif
#define DLT_NVM_OS_CORE_ID_BIT_MASK          0x1EU

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (DLT_IMPLEMENT_NV_RAM_STORAGE == STD_ON)

#define DLT_START_SEC_CODE
#include <Dlt_MemMap.h>

/** \brief Function to read a 32 bit data from byte array
 **
 ** Use this function to read specified 32 bit data from
 ** specified destination.
 **
 ** \param[in]      Dword       Data source
 ** \param[out]     DesPtr      Address of the data destination */
STATIC FUNC(void, DLT_CODE) Dlt_ReadDwordFromByteArray
(
  P2VAR(uint32, AUTOMATIC, NVM_APPL_DATA) PtrDestDword,
  P2CONST(uint8, AUTOMATIC, NVM_APPL_CONST) SrcPtr
);

#define DLT_STOP_SEC_CODE
#include <Dlt_MemMap.h>
/*==================[internal data]=========================================*/

#define DLT_START_SEC_VAR_CLEARED_8
#include <Dlt_MemMap.h>

/** \brief Flag used in order to signal if the NvMInitCbk for DataSet blocks is used */
STATIC VAR(boolean, DLT_VAR) Dlt_NvMInitCbkDataSetFlag;
/** \brief Flag used in order to signal if the NvMInitCbk for Native blocks is used */
STATIC VAR(boolean, DLT_VAR) Dlt_NvMInitCbkNativeFlag;

#define DLT_STOP_SEC_VAR_CLEARED_8
#include <Dlt_MemMap.h>

#define DLT_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dlt_MemMap.h>

/** \brief index to go through Application IDs stored in Dlt_AppIdTable
 ** in order to make the transfer of data from Dlt to Nv RAM*/
STATIC VAR(Dlt_AppIdTableIdxType, DLT_VAR) Dlt_AppIdIdx;
/** \brief Variable to hold the number of calls for noOfCtxPerApp*/
STATIC VAR(Dlt_AppToContextIdTableIdxType, DLT_VAR) Dlt_AppToCtxRefIdx;

#define DLT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dlt_MemMap.h>

/*==================[external data]=========================================*/
/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external function definitions]=========================*/
/*------------------[interface Dlt <--> NvM]--------------------------------*/
#define DLT_START_SEC_CODE
#include <Dlt_MemMap.h>

/*-----------------------------Dlt_NvMWriteRamBlockToNvMDataSetCbk----------*/
FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMWriteRamBlockToNvMDataSetCbk(
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvMBuffer)
{
  Std_ReturnType result = E_NOT_OK;
  /* Deviation MISRAC2012-1 */
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) NvMBufferPtr = (uint8*)NvMBuffer;

  uint32 accessContextTableIdx;

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
#if (DLT_ENABLE_BSW_DISTRIBUTION == STD_ON)
  CoreIdType CurrentCoreId = GetCoreID();
  Dlt_BSWSatelliteContextPtrType SatelliteContextPtr = (CurrentCoreId != DLT_MASTER_CORE) ?
      Dlt_GetSatelliteContextPtr() : NULL_PTR;
  Dlt_BSWDistributionCommonType LocalCoreContext = (CurrentCoreId != DLT_MASTER_CORE) ?
      SatelliteContextPtr->CoreContext : Dlt_BSWMasterContextPtr->CoreContext;
#else
  Dlt_BSWDistributionCommonType LocalCoreContext = Dlt_BSWMasterContextPtr->CoreContext;
#endif
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  DBG_DLT_NVMWRITERAMBLOCKTONVDATASETCBK_ENTRY(NvMBuffer);
#if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (NvMBufferPtr == NULL_PTR)
  {
    DLT_DET_REPORT_ERROR(DLT_SID_InternalWriteToDataSetApiId, DLT_E_PARAM_POINTER);
  }
  else
#endif
  {
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
    Dlt_ChnIdTableIdxType chnIdx;
#endif
    /* ENTER critical section */
    Dlt_EnterExclusiveAreaAnyCoreContext();

    if (Dlt_NvMInitCbkDataSetFlag == TRUE)
    {
      /* retrieve information related to the last registered pair of applId and ctxId if the NvMInitCbk
         is called */
      Dlt_GetLastRegisteredIndex(&Dlt_AppIdIdx, &Dlt_AppToCtxRefIdx);
    }
    else
    {
      /* to update the index of tables from where the information is retrieved*/
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
      if (Dlt_AppToCtxRefIdx >= LocalCoreContext.AppToContextIdTableIdx[Dlt_AppIdIdx])
#else
      if (Dlt_AppToCtxRefIdx >= Dlt_AppToContextIdTableIdx[Dlt_AppIdIdx])
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */
      {
        Dlt_AppToCtxRefIdx = 0U;
        Dlt_AppIdIdx++;
      }
    }
    /* If this function is erroneously called many times, the application ID index may get larger
     * than the maximum application IDs configured, causing out-of-bounds accesses.
     * As such, a E_NOT_OK return value provides protection against this type of behavior. */
    if (Dlt_AppIdIdx >= DLT_MAX_COUNT_APP_IDS)
    {
      /* !LINKSTO Dlt_StorePersistent.NvMWriteRamBlockToNvMDataSetCbk_Protection, 1 */
      result = E_NOT_OK;
    }
    else
    {

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
      accessContextTableIdx = LocalCoreContext.AppToContextIdTable[Dlt_AppIdIdx].ContextIdTableIdx[Dlt_AppToCtxRefIdx];
#else
      accessContextTableIdx = Dlt_AppToContextIdTable[Dlt_AppIdIdx].ContextIdTableIdx[Dlt_AppToCtxRefIdx];
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */
      /* Store data to Nv RAM */
      /* Store the AppId from Dlt RAM to the data set block in
      ** NvM[0U] = LSB1 of AppId
      ** NvM[1U] = LSB2 of AppId
      ** NvM[2U] = MSB1 of AppId
      ** NvM[3U] = MSB2 of AppId
      ** */
      /* !LINKSTO EB_Ref_SWS_Dlt_00073, 1 */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
      Dlt_WriteDwordToByteArray
        (&NvMBufferPtr[DLT_NVM_APPID_INDEX], LocalCoreContext.AppIdTable[Dlt_AppIdIdx].AppId);
#else
      Dlt_WriteDwordToByteArray
        (&NvMBufferPtr[DLT_NVM_APPID_INDEX], Dlt_AppIdTable[Dlt_AppIdIdx].AppId);
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */
      /* Store the ContextId from Dlt RAM to data set block in
      ** NvM[4U] = LSB1 of ContextId
      ** NvM[5U] = LSB2 of ContextId
      ** NvM[6U] = MSB1 of ContextId
      ** NvM[7U] = MSB2 of ContextId
      ** */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
      Dlt_WriteDwordToByteArray
        (&NvMBufferPtr[DLT_NVM_CTXID_INDEX], LocalCoreContext.ContextIdTable[accessContextTableIdx].ContextId);
      NvMBufferPtr[DLT_NVM_LOGLEVEL_INDEX] = LocalCoreContext.ContextIdTable[accessContextTableIdx].LogLevel;
      NvMBufferPtr[DLT_NVM_TRACESTATUS_INDEX] = LocalCoreContext.ContextIdTable[accessContextTableIdx].TraceStatus;
#else
      /* !LINKSTO EB_Ref_SWS_Dlt_00073, 1 */
      Dlt_WriteDwordToByteArray
        (&NvMBufferPtr[DLT_NVM_CTXID_INDEX], Dlt_BSWMasterContextPtr->CoreContext.ContextIdTable[accessContextTableIdx].ContextId);
      /* Store the LogLevel of current context from Dlt RAM to data set block in NvMBuffer[8U]*/
      /* !LINKSTO Dlt.DltPersistentStorage.BlockTypeDataSet, 2 */
      NvMBufferPtr[DLT_NVM_LOGLEVEL_INDEX] = Dlt_BSWMasterContextPtr->CoreContext.ContextIdTable[accessContextTableIdx].LogLevel;
      /* Store the TraceStatus of current context from Dlt RAM to data set block in NvMBuffer[9U]*/
      /* !LINKSTO Dlt.DltPersistentStorage.BlockTypeDataSet , 2 */
      NvMBufferPtr[DLT_NVM_TRACESTATUS_INDEX] = Dlt_BSWMasterContextPtr->CoreContext.ContextIdTable[accessContextTableIdx].TraceStatus;
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

      /* Store the SessionID from Dlt RAM to data set block in
      ** NvM[10U] = LSB1 of SessionID
      ** NvM[11U] = LSB2 of SessionID
      ** NvM[12U] = MSB1 of SessionID
      ** NvM[13U] = MSB2 of SessionID
      ** */

#if (DLT_SWC_SESSION_REGISTERED > 0)
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)

      Dlt_WriteDwordToByteArray
        (&NvMBufferPtr[DLT_NVM_SESSIONID_INDEX],
            LocalCoreContext.SessionIdTable[LocalCoreContext.ContextIdTable[accessContextTableIdx].SessionIdTableIdx].SessionId);

#else
      /* Store the session Ids provided by SW-Cs. If sessionID is provided by SW-C than the value
         stored in the location of the SessionId index must be different than DLT_NO_SESSION_ID */
      if(Dlt_BSWMasterContextPtr->CoreContext.ContextIdTable[accessContextTableIdx].SessionIdTableIdx != DLT_NO_SESSION_ID)
      {
        /* !LINKSTO Dlt.DltPersistentStorage.BlockTypeDataSet,2 */
        Dlt_WriteDwordToByteArray
          (&NvMBufferPtr[DLT_NVM_SESSIONID_INDEX],
              Dlt_SessionIdTable[Dlt_BSWMasterContextPtr->CoreContext.ContextIdTable[accessContextTableIdx].SessionIdTableIdx].SessionId);
      }
      else
      {
        /* SessionId is not provided by SW-C. Store a default value for the sessionID */
        Dlt_WriteDwordToByteArray(&NvMBufferPtr[DLT_NVM_SESSIONID_INDEX], DLT_NVM_DEFAULT_SESSIONID);
      }
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */
#endif

      /* NvM[14U] - 0000 000X - one bit storing the update state of the data */
      NvMBufferPtr[DLT_NVM_UPDATE_INDEX] = DLT_STORED_DATA_UPDATED;
#if ( (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) && (DLT_ENABLE_BSW_DISTRIBUTION == STD_ON) )
      /* NvM[14U] - 000X XXX0 - four bits storing the current OsCoreId */
      NvMBufferPtr[DLT_NVM_UPDATE_INDEX] |= LocalCoreContext.ContextIdTable[accessContextTableIdx].OsCoreId << 1U;
#endif /* #if ( (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) && (DLT_ENABLE_BSW_DISTRIBUTION == STD_ON) ) */

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
      /* NvM[15U] - store verbose mode status */
      NvMBufferPtr[DLT_NVM_VERBOSEMODE_INDEX] = LocalCoreContext.ContextIdTable[accessContextTableIdx].VerboseMode;
      /* NvM[16U] - store log channel assignment */
      for (chnIdx = 0; chnIdx < DLT_TXPDU_NO; chnIdx ++)
      {
        NvMBufferPtr[DLT_NVM_CHANNEL_NAME_INDEX + chnIdx] = LocalCoreContext.ContextIdTable[accessContextTableIdx].ChannelsIdx[chnIdx];
      }
#else
      NvMBufferPtr[DLT_NVM_VERBOSEMODE_INDEX] = Dlt_BSWMasterContextPtr->CoreContext.ContextIdTable[accessContextTableIdx].VerboseMode;
#endif
      if (Dlt_NvMInitCbkDataSetFlag == FALSE)
      {
        ++Dlt_AppToCtxRefIdx;
      }
      /* Update status of function execution */
      result = E_OK;
    }
    /* exit critical section */
    Dlt_ExitExclusiveAreaAnyCoreContext();
  }

  DBG_DLT_NVMWRITERAMBLOCKTONVDATASETCBK_EXIT(NvMBuffer, result);
  return result;
}

/*-----------------------------Dlt_NvMWriteRamBlockToNvMNativeCbk----------*/
FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMWriteRamBlockToNvMNativeCbk(
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvMBuffer)
{
  Std_ReturnType result = E_NOT_OK;
  /* Deviation MISRAC2012-1 */
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) NvMBufferPtr = (uint8*)NvMBuffer;

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
#if (DLT_ENABLE_BSW_DISTRIBUTION == STD_ON)
  CoreIdType CurrentCoreId = GetCoreID();
  Dlt_BSWSatelliteContextPtrType SatelliteContextPtr = (CurrentCoreId != DLT_MASTER_CORE) ?
      Dlt_GetSatelliteContextPtr() : NULL_PTR;
  Dlt_BSWDistributionCommonType LocalCoreContext = (CurrentCoreId != DLT_MASTER_CORE) ?
      SatelliteContextPtr->CoreContext : Dlt_BSWMasterContextPtr->CoreContext;
#else
  Dlt_BSWDistributionCommonType LocalCoreContext = Dlt_BSWMasterContextPtr->CoreContext;
#endif
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  DBG_DLT_NVMWRITERAMBLOCKTONVNATIVECBK_ENTRY(NvMBuffer);

#if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (NvMBuffer == NULL_PTR)
  {
    DLT_DET_REPORT_ERROR(DLT_SID_InternalWriteToNativeApiId, DLT_E_PARAM_POINTER);
  }
  else
#endif
  {
    /* Store persistent by copying data one by one from Dlt module's RAM into Nv native data set */

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
    NvMBufferPtr[DLT_NVM_FILTER_MSG_NATIVE_IDX] = *LocalCoreContext.FilterMessages;
    NvMBufferPtr[DLT_NVM_DEFAULT_MAX_LOG_LEVEL_NATIVE_IDX] = *LocalCoreContext.DefaultLogLevel;
#else
    /* !LINKSTO Dlt.DltPersistentStorage.DltConfigParam, 1,
        Dlt.swdd.DltPersistentStorage.InfoInNativeBlock, 1    */
    NvMBufferPtr[DLT_NVM_FILTER_MSG_NATIVE_IDX] = *Dlt_BSWMasterContextPtr->CoreContext.FilterMessages;
    NvMBufferPtr[DLT_NVM_DEFAULT_MAX_LOG_LEVEL_NATIVE_IDX] = *Dlt_BSWMasterContextPtr->CoreContext.DefaultLogLevel;
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */
#if (DLT_IMPLEMENT_TIMESTAMP != STD_OFF)
    NvMBufferPtr[DLT_NVM_HDR_USE_TIMESTAMP_NATIVE_IDX] = Dlt_HeaderUseTimestamp;
#endif /* DLT_IMPLEMENT_TIMESTAMP != STD_OFF */
    NvMBufferPtr[DLT_NVM_HDR_USE_ECUID_NATIVE_IDX] = Dlt_HeaderUseEcuId;
    NvMBufferPtr[DLT_NVM_HDR_USE_EXTENDEDHDR_NATIVE_IDX] = Dlt_HeaderUseExtendedHeader;
    NvMBufferPtr[DLT_NVM_HDR_USE_SESSIONID_NATIVE_IDX] = Dlt_HeaderUseSessionID;
    NvMBufferPtr[DLT_NVM_HDR_USE_VERBOSEMODE_NATIVE_IDX] = Dlt_HeaderUseVerboseMode;

    /* When the "Dlt_VfbTraceLogLevel" runtime variable will be available,
     * the following line of code must be uncommented. This line is now added for completeness reasons */
    /* NvMBuffer[DLT_NVM_VFB_TRACELOGLEVEL_NATIVE_IDX] = Dlt_VfbTraceLogLevel; */

    /* ENTER critical section */
    Dlt_EnterExclusiveAreaAnyCoreContext();

    NvMBufferPtr[DLT_NVM_BANDWIDTH_COM_NATIVE_IDX]     = (uint8)(Dlt_BandwidthForComModule & 0xffU);
    NvMBufferPtr[DLT_NVM_BANDWIDTH_COM_NATIVE_IDX+1U]  = (uint8)((uint32)(Dlt_BandwidthForComModule >>  8U) & 0xffU);
    NvMBufferPtr[DLT_NVM_BANDWIDTH_COM_NATIVE_IDX+2U]  = (uint8)((uint32)(Dlt_BandwidthForComModule >> 16U) & 0xffU);
    NvMBufferPtr[DLT_NVM_BANDWIDTH_COM_NATIVE_IDX+3U]  = (uint8)((uint32)(Dlt_BandwidthForComModule >> 24U) & 0xffU);

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
    NvMBufferPtr[DLT_NVM_DEFAULT_TRACE_STATUS_NATIVE_IDX] = *LocalCoreContext.DefaultTraceStatus;
    /* Store the runtime information into the NvM buffer */
    TS_MemCpy(&NvMBufferPtr[DLT_NVM_LOG_CHANNELS_TRACE_NATIVE_IDX],
        &Dlt_LogChannelThresholdInfo[0U],
        sizeof(Dlt_LogChannelThresholdInfo));
#else
    NvMBufferPtr[DLT_NVM_DEFAULT_TRACE_STATUS_NATIVE_IDX] = *Dlt_BSWMasterContextPtr->CoreContext.DefaultTraceStatus;
#endif

    /* Exit critical section */
    Dlt_ExitExclusiveAreaAnyCoreContext();

    NvMBufferPtr[DLT_NVM_GLOBAL_LOG_STATUS_NATIVE_IDX] = Dlt_GlobalLogStatus;

    /* Update status of function execution */
    result = E_OK;
  }

  DBG_DLT_NVMWRITERAMBLOCKTONVNATIVECBK_EXIT(NvMBuffer, result);
  return result;
}
/*-----------------------------Dlt_NvMInitDataSetBlockCbk----------*/
FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMInitDataSetBlockCbk(void)
{
  Std_ReturnType result = E_NOT_OK;
  uint8 data_set_index = 0U;

  DBG_DLT_NVMINITDATASETBLOCKCBK_ENTRY();

  /* get the current dataset index
   * the only return value at this point is E_OK since
   *  - block Id is correctly given as input and the block is a dataset block
   *  - currentDataSetIndex has a valid address
   *  - NvM at this point is initialized
   */
   /* !LINKSTO Dlt.swdd.DltPersistentStorage.BlockIdForDataSetBlock, 1 */
  (void)NvM_GetDataIndex(DLT_STORE_DATASET_NVRAM_BLOCK_ID, &data_set_index);
#if(DLT_STORE_DATASET_NVRAM_BLOCK_NUMBER > 0U)
  /* get the configured factory default */
  if (data_set_index < DLT_STORE_DATASET_NVRAM_BLOCK_NUMBER)
  {
    /* internal data to store the wanted configuration */
    Dlt_SessionIDType session_id = 0U;
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
    Dlt_ApplicationIDType app_id = {0U};
    Dlt_ContextIDType context_id = {0U};
    Dlt_Internal_ApplicationIDType app_id_Internal = 0U;
    Dlt_Internal_ContextIDType context_id_Internal = 0U;
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
    Dlt_ApplicationIDType app_id = 0U;
    Dlt_ContextIDType context_id = 0U;
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */
    Dlt_MessageTraceStatusType trace_status = 0U;

    Dlt_BSWDistributionCommonType localCoreContext;
    Dlt_ContextIdTableType localCoreContextTable;

    localCoreContext.ContextIdTable     = &localCoreContextTable;
    localCoreContext.DefaultLogLevel    = &Dlt_DefaultLogLevel;
    localCoreContext.DefaultTraceStatus = &Dlt_DefaultTraceStatus;
    localCoreContext.FilterMessages     = &Dlt_FilterMessages;

    /* !LINKSTO Dlt.swdd.DltPersistentStorage.GeneralRestoreOfTheDatasetBlock, 1 */
#if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431)
    localCoreContextTable.LogLevel = DLT_FACTORY_DEFAULT_MAX_LOG_LEVEL;
#endif
    localCoreContextTable.VerboseMode = Dlt_HeaderUseVerboseMode;

    /* !LINKSTO Dlt.swdd.DltPersistentStorage.TraceStatusFactoryDefaultForRegisteredContexts, 1,
                Dlt.swdd.DltPersistentStorage.FactoryDefaultTraceLogAndSessionForRegContexts, 1 */
    if (DLT_APPL_OK == Dlt_ApplGetConfigFactoryDefault
         (data_set_index, &session_id, &app_id, &context_id, &trace_status) )
    {
      localCoreContextTable.TraceStatus = trace_status;
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
      /* !LINKSTO SWS_Dlt_00348,1 */
      app_id_Internal = DLT_UINT32_CONVERT_TUPLE(app_id);
      context_id_Internal = DLT_UINT32_CONVERT_TUPLE(context_id);
      /* register context with data provided by the user */
      if (DLT_E_OK == Dlt_InternalRegisterContext
         (session_id,
          app_id_Internal,
          context_id_Internal,
          &localCoreContext,
          NULL_PTR,
          0U,
          TRUE))
#else
        if (DLT_E_OK == Dlt_InternalRegisterContext
           (session_id,
            app_id,
            context_id,
            &localCoreContext,
            NULL_PTR,
            0U,
            TRUE))
#endif
      {
        result = E_OK;
        Dlt_NvMInitCbkDataSetFlag = TRUE;
      }
    }
    else
    {
      /* Nothing to do because all datasets have been processed */
    }
  }
#endif

  DBG_DLT_NVMINITDATASETBLOCKCBK_EXIT(result);
  return result;
}
/*-----------------------------Dlt_NvMInitNativeBlockCbk----------*/
FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMInitNativeBlockCbk(void)
{
  Std_ReturnType result = E_NOT_OK;

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
#if (DLT_ENABLE_BSW_DISTRIBUTION == STD_ON)
  CoreIdType CurrentCoreId = GetCoreID();
  Dlt_BSWSatelliteContextPtrType SatelliteContextPtr = (CurrentCoreId != DLT_MASTER_CORE) ?
      Dlt_GetSatelliteContextPtr() : NULL_PTR;
  Dlt_BSWDistributionCommonType LocalCoreContext = (CurrentCoreId != DLT_MASTER_CORE) ?
      SatelliteContextPtr->CoreContext : Dlt_BSWMasterContextPtr->CoreContext;
#else
  Dlt_BSWDistributionCommonType LocalCoreContext = Dlt_BSWMasterContextPtr->CoreContext;
#endif
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  DBG_DLT_NVMINITNATIVECBK_ENTRY();

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  /* !LINKSTO Dlt.swdd.DltPersistentStorage.FactoryDefaultForCfgParameters, 1 */
  *LocalCoreContext.FilterMessages = DLT_FILTER_MESSAGES;
  *LocalCoreContext.DefaultLogLevel = DLT_DEFAULT_MAX_LOG_LEVEL;
  *LocalCoreContext.DefaultTraceStatus = DLT_DEFAULT_TRACE_STATUS;
#else
  *Dlt_BSWMasterContextPtr->CoreContext.FilterMessages = DLT_FILTER_MESSAGES;
  *Dlt_BSWMasterContextPtr->CoreContext.DefaultLogLevel = DLT_DEFAULT_MAX_LOG_LEVEL;
  *Dlt_BSWMasterContextPtr->CoreContext.DefaultTraceStatus = DLT_DEFAULT_TRACE_STATUS;
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

#if (DLT_IMPLEMENT_TIMESTAMP != STD_OFF)
  Dlt_HeaderUseTimestamp = DLT_HEADER_CONFIG_TIMESTAMP_BIT;
#endif /* DLT_IMPLEMENT_TIMESTAMP != STD_OFF */
  Dlt_HeaderUseEcuId = DLT_HEADER_CONFIG_ECU_ID_BIT;
  Dlt_HeaderUseExtendedHeader = DLT_HEADER_CONFIG_EXTENDED_HEADER_BIT;
  Dlt_HeaderUseSessionID = DLT_HEADER_CONFIG_SESSION_ID_BIT;
  Dlt_HeaderUseVerboseMode = DLT_USE_VERBOSE_MODE;
  Dlt_GlobalLogStatus = DLT_LOGGING_ENABLED;

  Dlt_EnterExclusiveAreaAnyCoreContext();
  Dlt_BandwidthForComModule = DLT_BANDWIDTH_FOR_COM_MODULE;
  Dlt_ExitExclusiveAreaAnyCoreContext();

  Dlt_NvMInitCbkNativeFlag = TRUE;

  result = E_OK;

  DBG_DLT_NVMINITNATIVECBK_EXIT(result);
  return result;
}
/*-----------------------------Dlt_NvMReadRamBlockFromNvMDataSetCbk----------*/
FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMReadRamBlockFromNvMDataSetCbk(
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvMBuffer)
{
  Std_ReturnType result = E_NOT_OK;

  /* Deviation MISRAC2012-1 */
  P2CONST(uint8, AUTOMATIC, NVM_CONST) NvMBufferPtr = (const uint8*)NvMBuffer;

  DBG_DLT_NVMREADRAMBLOCKFROMNVMDATASETCBK_ENTRY(NvMBuffer);

#if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (NvMBufferPtr == NULL_PTR)
  {
    /* !LINKSTO Dlt_NvMReadRamBlockFromNvM.NvMBufferNULLPTR,1 */
    /* throw Det error with DLT_E_PARAM_POINTER*/
    DLT_DET_REPORT_ERROR( DLT_SID_InternalReadFromDataSetApiId, DLT_E_PARAM_POINTER);
    /* result shall remain E_NOT_OK */
  }
  else
#endif
  {
    Dlt_MessageLogLevelType DefaultLogLevel;
    Dlt_MessageTraceStatusType DefaultTraceStatus;
    Dlt_FilterMessagesType FilterMessages;
    Dlt_ContextIdTableType ContextIdTable;
    Dlt_BSWDistributionCommonType localContext;

    localContext.ContextIdTable = &ContextIdTable;
    localContext.DefaultLogLevel = &DefaultLogLevel;
    localContext.DefaultTraceStatus = &DefaultTraceStatus;
    localContext.FilterMessages = &FilterMessages;
    /*
     * ENTER critical section to protect values of log level and
     * trace status for each pair of ApplicationID and ContextId which have
     * been explicitly registered by Dlt_registerContext()
     * which shall be restored from data set NvM block
     */
    Dlt_EnterExclusiveAreaAnyCoreContext();
    /* copy data stored in Nv data set block to Dlt -> restore information from NvM block to Dlt */
    if (((NvMBufferPtr[DLT_NVM_UPDATE_INDEX] & DLT_NVM_STORED_DATA_UPDATED_BIT_MASK) != 0U) &&
        (Dlt_NvMInitCbkDataSetFlag == FALSE))
    {
      Dlt_Internal_ApplicationIDType Dlt_NvMApplicationID;
      Dlt_Internal_ContextIDType Dlt_NvMContextID;
      Dlt_SessionIDType Dlt_NvMSessionId;
      /* NvMBuffer is the source of information and Dlt_NvMApplicationID is the storing location*/
      Dlt_ReadDwordFromByteArray(&Dlt_NvMApplicationID, &NvMBufferPtr[DLT_NVM_APPID_INDEX]);
      Dlt_ReadDwordFromByteArray(&Dlt_NvMContextID, &NvMBufferPtr[DLT_NVM_CTXID_INDEX]);
      ContextIdTable.LogLevel = NvMBufferPtr[DLT_NVM_LOGLEVEL_INDEX];
      ContextIdTable.TraceStatus = NvMBufferPtr[DLT_NVM_TRACESTATUS_INDEX];
      ContextIdTable.VerboseMode = NvMBufferPtr[DLT_NVM_VERBOSEMODE_INDEX];
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
      ContextIdTable.ContextDescription.CtxtDescLen = 0U;
      TS_MemBZero(&ContextIdTable.ContextDescription.CtxtDesc[0U], 0xFFU);
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */
      Dlt_ReadDwordFromByteArray(&Dlt_NvMSessionId, &NvMBufferPtr[DLT_NVM_SESSIONID_INDEX]);
#if ( (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) && (DLT_ENABLE_BSW_DISTRIBUTION == STD_ON) )
      ContextIdTable.OsCoreId = (NvMBufferPtr[DLT_NVM_UPDATE_INDEX] & DLT_NVM_OS_CORE_ID_BIT_MASK) >> 1U;
#endif /* #if ( (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) && (DLT_ENABLE_BSW_DISTRIBUTION == STD_ON) ) */
      /* The session Id passed will always be set to 0U
       * The return value can be ignored in this case as the only possible return
       * value is E_OK. Reason: we know for a fact that information which was
       * previously written in the NvM is valid and correct and therefore when
       * copying that data back to the DLT RAM, the Dlt_InternalRegisterContext uses
       * valid and correct data to register information */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
      {
        Dlt_ChnIdTableIdxType chnIdx;

        for (chnIdx = 0; chnIdx < DLT_TXPDU_NO; chnIdx ++)
        {
          localContext.ContextIdTable->ChannelsIdx[chnIdx] = NvMBufferPtr[DLT_NVM_CHANNEL_NAME_INDEX + chnIdx];
        }
      }
#endif
      /* !LINKSTO Dlt_NvMReadRamBlockFromNvMDataSetCbk.RestoreOverwrite,1 */
      (void)Dlt_InternalRegisterContext(Dlt_NvMSessionId,
                                        Dlt_NvMApplicationID,
                                        Dlt_NvMContextID,
                                        &localContext,
                                        NULL_PTR,
                                        0U,
                                        TRUE);

    }

    result = E_OK;
    /* EXIT critical section */
    Dlt_ExitExclusiveAreaAnyCoreContext();
  }

  DBG_DLT_NVMREADRAMBLOCKFROMNVMDATASETCBK_EXIT(NvMBuffer, result);
  return result;
}
/*-----------------------------Dlt_NvMReadRamBlockFromNvMNativeCbk----------*/
FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMReadRamBlockFromNvMNativeCbk(
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvMBuffer)
{
  Std_ReturnType result = E_NOT_OK;
  /* Deviation MISRAC2012-1 */
  P2CONST(uint8, AUTOMATIC, NVM_CONST) NvMBufferPtr = (const uint8*)NvMBuffer;

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
#if (DLT_ENABLE_BSW_DISTRIBUTION == STD_ON)
  CoreIdType CurrentCoreId = GetCoreID();
  Dlt_BSWSatelliteContextPtrType SatelliteContextPtr = (CurrentCoreId != DLT_MASTER_CORE) ?
      Dlt_GetSatelliteContextPtr() : NULL_PTR;
  Dlt_BSWDistributionCommonType LocalCoreContext = (CurrentCoreId != DLT_MASTER_CORE) ?
      SatelliteContextPtr->CoreContext : Dlt_BSWMasterContextPtr->CoreContext;
#else
  Dlt_BSWDistributionCommonType LocalCoreContext = Dlt_BSWMasterContextPtr->CoreContext;
#endif
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  DBG_DLT_NVMREADRAMBLOCKFROMNVMNATIVECBK_ENTRY(NvMBuffer);

#if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (NvMBufferPtr == NULL_PTR)
  {
    /* !LINKSTO Dlt_NvMReadRamBlockFromNvM.NvMBufferNULLPTR,1 */
    /* throw Det error with DLT_E_PARAM_POINTER*/
    DLT_DET_REPORT_ERROR(DLT_SID_InternalReadFromNativeApiId, DLT_E_PARAM_POINTER);
    /* result shall remain E_NOT_OK */
  }
  else
#endif
  {
    /*
     * ENTER critical section to protect runtime configurable variables which
     * shall be restored from native NvM block
     */
    Dlt_EnterExclusiveAreaAnyCoreContext();

    if (Dlt_NvMInitCbkNativeFlag == FALSE)
    {
      /* copy data stored in Nv native block to Dlt -> restore information from NvM block to Dlt */

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
      *LocalCoreContext.FilterMessages = NvMBufferPtr[DLT_NVM_FILTER_MSG_NATIVE_IDX];
      *LocalCoreContext.DefaultLogLevel = NvMBufferPtr[DLT_NVM_DEFAULT_MAX_LOG_LEVEL_NATIVE_IDX];
#else
      *Dlt_BSWMasterContextPtr->CoreContext.FilterMessages = NvMBufferPtr[DLT_NVM_FILTER_MSG_NATIVE_IDX];
      *Dlt_BSWMasterContextPtr->CoreContext.DefaultLogLevel = NvMBufferPtr[DLT_NVM_DEFAULT_MAX_LOG_LEVEL_NATIVE_IDX];
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */
#if (DLT_IMPLEMENT_TIMESTAMP != STD_OFF)
      Dlt_HeaderUseTimestamp = NvMBufferPtr[DLT_NVM_HDR_USE_TIMESTAMP_NATIVE_IDX];
#endif /* DLT_IMPLEMENT_TIMESTAMP != STD_OFF */
      Dlt_HeaderUseEcuId = NvMBufferPtr[DLT_NVM_HDR_USE_ECUID_NATIVE_IDX];
      Dlt_HeaderUseExtendedHeader = NvMBufferPtr[DLT_NVM_HDR_USE_EXTENDEDHDR_NATIVE_IDX];
      Dlt_HeaderUseSessionID = NvMBufferPtr[DLT_NVM_HDR_USE_SESSIONID_NATIVE_IDX];
      /* !LINKSTO Dlt.DltProtocol.UseVerboseMode.InitialValueNvM, 1 */
      Dlt_HeaderUseVerboseMode = NvMBufferPtr[DLT_NVM_HDR_USE_VERBOSEMODE_NATIVE_IDX];

      Dlt_BandwidthForComModule  = (uint32)NvMBufferPtr[DLT_NVM_BANDWIDTH_COM_NATIVE_IDX];
      Dlt_BandwidthForComModule |= (uint32)(((uint32)NvMBufferPtr[DLT_NVM_BANDWIDTH_COM_NATIVE_IDX+1U]) <<  8U);
      Dlt_BandwidthForComModule |= (uint32)(((uint32)NvMBufferPtr[DLT_NVM_BANDWIDTH_COM_NATIVE_IDX+2U]) << 16U);
      Dlt_BandwidthForComModule |= (uint32)(((uint32)NvMBufferPtr[DLT_NVM_BANDWIDTH_COM_NATIVE_IDX+3U]) << 24U);

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
      *LocalCoreContext.DefaultTraceStatus = NvMBufferPtr[DLT_NVM_DEFAULT_TRACE_STATUS_NATIVE_IDX];
      TS_MemCpy(&Dlt_LogChannelThresholdInfo[0U],
          &NvMBufferPtr[DLT_NVM_LOG_CHANNELS_TRACE_NATIVE_IDX],
          sizeof(Dlt_LogChannelThresholdInfo));
#else
      *Dlt_BSWMasterContextPtr->CoreContext.DefaultTraceStatus = NvMBufferPtr[DLT_NVM_DEFAULT_TRACE_STATUS_NATIVE_IDX];
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */
      Dlt_GlobalLogStatus = NvMBufferPtr[DLT_NVM_GLOBAL_LOG_STATUS_NATIVE_IDX];
    }
      /* EXIT critical section */
    Dlt_ExitExclusiveAreaAnyCoreContext();
    result = E_OK;
  }

  DBG_DLT_NVMREADRAMBLOCKFROMNVMNATIVECBK_EXIT(NvMBuffer, result);

  return result;
}
/*-----------------------------Dlt_NvMSingleBlockCallbackDataSet----------*/
/* !LINKSTO Dlt.swdd.DltPersistentStorage.SingleBlockCallbackForDataSetBlockFinish,1 */
FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMSingleBlockCallbackDataSet(
  uint8                 ServiceId,
  NvM_RequestResultType JobResult)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 currentDataSetIndex = 0U;
  boolean Dlt_ResetIdxForWrite = FALSE;
  boolean Dlt_SetModuleToPersistent = FALSE;

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
#if (DLT_ENABLE_BSW_DISTRIBUTION == STD_ON)
  CoreIdType CurrentCoreId = GetCoreID();
  Dlt_BSWSatelliteContextPtrType SatelliteContextPtr = (CurrentCoreId != DLT_MASTER_CORE) ?
      Dlt_GetSatelliteContextPtr() : NULL_PTR;
  Dlt_BSWDistributionCommonType LocalCoreContext = (CurrentCoreId != DLT_MASTER_CORE) ?
      SatelliteContextPtr->CoreContext : Dlt_BSWMasterContextPtr->CoreContext;
#else
  Dlt_BSWDistributionCommonType LocalCoreContext = Dlt_BSWMasterContextPtr->CoreContext;
#endif
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  DBG_DLT_NVMSINGLEBLOCKCALLBACKDATASET_ENTRY(ServiceId, JobResult);

  /* In case the JobResult is NVM_REQ_NOT_OK and serviceID = WRITE_BLOCK increment
     the index of the corresponding applicationId to contextId reference */
  if ((JobResult != NVM_REQ_OK) && (JobResult != NVM_REQ_RESTORED_DEFAULTS))
  {
    if (ServiceId == DLT_NVM_WRITE_BLOCK_API_ID)
    {
      Dlt_EnterExclusiveAreaAnyCoreContext();
      Dlt_AppToCtxRefIdx++;
      Dlt_ExitExclusiveAreaAnyCoreContext();
    }
  }

  /* get the current dataset index
   * the only return value at this point is E_OK since
   *  - block Id is correctly given as input and the block is a dataset block
   *  - currentDataSetIndex has a valid address
   *  - NvM at this point is intialized
   */
   /* !LINKSTO Dlt.swdd.DltPersistentStorage.BlockIdForDataSetBlock, 1 */
  (void)NvM_GetDataIndex(DLT_STORE_DATASET_NVRAM_BLOCK_ID, &currentDataSetIndex);
  /* Next dataset shall be processed */
  currentDataSetIndex++;
#if(DLT_STORE_DATASET_NVRAM_BLOCK_NUMBER > 0U)
  if (currentDataSetIndex < DLT_STORE_DATASET_NVRAM_BLOCK_NUMBER)
  {
    Std_ReturnType async_job_accepted = E_NOT_OK;

    /* the only return value at this point is E_OK for this API is since
     *  - block Id is correctly given as input and the block type is a dataset
     *  - currentDataSetIndex is checked to be in configured range
     *  - NvM at this point is intialized
     *  - it is not possible to have another request for this block pending at this point
    */
    /* !LINKSTO Dlt.swdd.DltPersistentStorage.BlockIdForDataSetBlock, 1 */
    (void)NvM_SetDataIndex(DLT_STORE_DATASET_NVRAM_BLOCK_ID, currentDataSetIndex);

    /* trigger request */
    if (ServiceId == DLT_NVM_WRITE_BLOCK_API_ID)
    {
      /* check if there are any registered contexts left */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
      /* !LINKSTO Dlt.swdd.DltPersistentStorage.SingleBlockCallbackForDataSetBlockFunctionality,2 */
      if (currentDataSetIndex < *LocalCoreContext.ContextIdTableIdx)
#else
      if (currentDataSetIndex < *Dlt_BSWMasterContextPtr->CoreContext.ContextIdTableIdx)
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */
      {
        async_job_accepted = NvM_WriteBlock(DLT_STORE_DATASET_NVRAM_BLOCK_ID, NULL_PTR);
      }
      else
      {
        Dlt_ResetIdxForWrite = TRUE;
      }
    }
    else if (ServiceId == DLT_NVM_READ_BLOCK_API_ID)
    {
      async_job_accepted = NvM_ReadBlock(DLT_STORE_DATASET_NVRAM_BLOCK_ID, NULL_PTR);
    }
    else
    {
      /* ignore request */
    }

    /* check if the asynchronous job has been accepted by NvM */
    if (async_job_accepted == E_NOT_OK)
    {
#if (DLT_DEV_ERROR_DETECT == STD_ON)
      /* report DET to notify user that the processing of the persistent storage has stopped */
      DLT_DET_REPORT_ERROR(DLT_SID_SingleBlockCallbackDataSet, DLT_E_REQUEST_NOT_ACCEPTED);
#endif
      Dlt_ResetIdxForWrite = TRUE;
      Dlt_SetModuleToPersistent = TRUE;
      RetVal = E_NOT_OK;
    }
    else
    {
      /* the new request is accepted */
      RetVal = E_OK;
    }
  }
  else
#endif
  {
    /* all the datasets are processed */
    Dlt_ResetIdxForWrite = TRUE;
    Dlt_SetModuleToPersistent = TRUE;
    Dlt_NvMInitCbkNativeFlag = FALSE;
    if(Dlt_NvMInitCbkDataSetFlag == TRUE)
    {
      Dlt_EnterExclusiveAreaAnyCoreContext();
      Dlt_AppToCtxRefIdx = 0U;
      Dlt_AppIdIdx = 0U;
      Dlt_ExitExclusiveAreaAnyCoreContext();
      Dlt_NvMInitCbkDataSetFlag = FALSE;
    }
    RetVal = E_OK;
  }

  /* Check if the module's state needs to be updated to PERSISTENT */
  if ((ServiceId == DLT_NVM_READ_BLOCK_API_ID) && (Dlt_SetModuleToPersistent == TRUE))
  {
    Dlt_InitStateMaster = DLT_INITIALIZED_PERSISTENT;

#if (DLT_ENABLE_BSW_DISTRIBUTION == STD_ON)
    Dlt_SendTablesToSatellites(DLT_ALL_SATELLITES);
#endif
  }

  /* check if a reset for the variables used to access the register context is needed */
  if ((ServiceId == DLT_NVM_WRITE_BLOCK_API_ID) && (Dlt_ResetIdxForWrite == TRUE))
  {
    /* Update variables to access the registered contexts */
    DLT_RESET_STORE_INDEX();
  }

  DBG_DLT_NVMSINGLEBLOCKCALLBACKDATASET_EXIT(ServiceId, JobResult, RetVal);
  return RetVal;
}
/*-----------------------------Dlt_NvMSingleBlockCallbackNative-------------------------*/
FUNC(Std_ReturnType, DLT_CODE) Dlt_NvMSingleBlockCallbackNative(
  uint8 ServiceId,
  NvM_RequestResultType JobResult)
{
  Std_ReturnType result = E_NOT_OK;

  DBG_DLT_NVMSINGLEBLOCKCALLBACKNATIVE_ENTRY(ServiceId, JobResult);

  TS_PARAM_UNUSED(ServiceId);

  /* Check ServiceId and JobResult */
  /* !LINKSTO Dlt.swdd.DltPersistentStorage.SingleBlockCallbackForNativeBlockJobResult,1 */
  if ((JobResult != NVM_REQ_OK) && (JobResult != NVM_REQ_RESTORED_DEFAULTS))
  {
#if (DLT_DEV_ERROR_DETECT == STD_ON)
    DLT_DET_REPORT_ERROR(DLT_SID_NvMSingleBlockCallbackNative, DLT_E_REQUEST_NOT_ACCEPTED);
#endif
  }
  else
  {
    result = E_OK;
  }

  DBG_DLT_NVMSINGLEBLOCKCALLBACKNATIVE_EXIT(ServiceId, JobResult, result);
  /* returns status of execution E_OK always */
  return result;
}

/*==================[internal function definitions]=========================*/

/*-------------------Dlt_ReadDwordFromByteArray-------------------------*/
STATIC FUNC(void, DLT_CODE) Dlt_ReadDwordFromByteArray
(
  P2VAR(uint32, AUTOMATIC, NVM_APPL_DATA) PtrDestDword,
  P2CONST(uint8, AUTOMATIC, NVM_APPL_CONST) SrcPtr
)
{

  DBG_DLT_READDWORDFROMBYTEARRAY_ENTRY(PtrDestDword, SrcPtr);
  *PtrDestDword = (uint32)(((uint32)SrcPtr[DLT_UINT32_SB0_IDX]) |
                          (((uint32)SrcPtr[DLT_UINT32_SB1_IDX]) << 8U) |
                          (((uint32)SrcPtr[DLT_UINT32_SB2_IDX]) << 16U) |
                          (((uint32)SrcPtr[DLT_UINT32_SB3_IDX]) << 24U));

  DBG_DLT_READDWORDFROMBYTEARRAY_EXIT(PtrDestDword, SrcPtr);

}

#define DLT_STOP_SEC_CODE
#include <Dlt_MemMap.h>

#endif /*if (DLT_IMPLEMENT_NV_RAM_STORAGE == STD_ON)*/
/*==================[end of file]===========================================*/
