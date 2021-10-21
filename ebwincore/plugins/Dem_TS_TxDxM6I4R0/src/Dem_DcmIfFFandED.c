/**
 * \file
 *
 * \brief AUTOSAR Dem
 *
 * This file contains the implementation of the AUTOSAR
 * module Dem.
 *
 * \version 6.4.1
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* This file contains definitions for interface functions provided by Dem to
 * the Dcm for the access of extended data records and freeze frame data. */

/* MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 13.5 (required)
 *    "The right hand operand of a logical && or || operator shall not contain
 *    persistent side effects."
 *
 *    Reason:
 *    Call to getter functions which do not modify any global state.
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason:
 * "Result" remains E_NOT_OK only if Dem_ClientIdLockingDTCRecordUpdate != ClientId.
 * In other configurations, the code coverage reaches sections where the result is
 * different from the value E_OK.
 *
 * TASKING-2) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason:
 * The compiler optimizes the 'for' loop (which has only 1 iteration) in the following way:
 * - it prepares in advance the index for the next iteration
 * - if the index is equal to '0' it goes back to check the 'for' loop conditions, otherwise it just moves on
 * - since the index is already equal to '1', it never jumps back to check the exit condition
 */

/*==================[inclusions]============================================*/
/* !LINKSTO dsn.Dem.IncludeStr,1 */

#include <Std_Types.h>                            /* AUTOSAR standard types */
#include <TSAutosar.h>                        /* EB specific standard types */
#include <TSMem.h>                          /* EB specific memory functions */

#include <Dem_Int.h>             /* Module public and internal declarations */
#include <Dem_Trace.h>                        /* Dbg related macros for Dem */

/*==================[macros]================================================*/

#if (defined DEM_HIBYTE)
#error DEM_HIBYTE already defined
#endif
/** \brief Macro to get hi-byte of an uint16 */
#define DEM_HIBYTE(a) ((uint8)((a) >> 8U))

#if (defined DEM_LOBYTE)
#error DEM_LOBYTE already defined
#endif
/** \brief Macro to get low-byte of an uint16 */
#define DEM_LOBYTE(a) ((uint8)((a) & (uint16)0xFFU))

#if (defined DEM_SIZE_OBDFF)
#error DEM_SIZE_OBDFF already defined
#endif
/** \brief Macro to get whole size of OBD Freeze Frame */
#define DEM_SIZE_OBDFF (DEM_SIZE_OBDFF_DATA + (2U * DEM_NUM_OF_PIDS_IN_OBDFF) + 2U)

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

#if (DEM_NUM_FFSEGS > 0U)
/** \brief Function to copy all freeze frame segment data Ids and values
 ** into the destination buffer
 **
 ** \param[in] FFKind  Kind of freeze frame
 ** \param[in] IndividPartFFClass  Class of individual freeze frame part
 ** \param[in] FFEntryData  Freeze frame entry data source
 ** \param[out] DestBuffer  Destination buffer of overlying API
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(void, DEM_CODE) Dem_CopyFFInfo(
  Dem_FFKindType                                 FFKind,
  P2CONST(Dem_FFClassType, AUTOMATIC, DEM_CONST) IndividPartFFClass,
  Dem_EntryDataPtrConstType                      FFEntryData,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)         DestBuffer);

/** \brief Function to copy freeze frame data destination buffer
 **
 ** \param[in] EventId
 ** \param[in] FFKind Kind of freeze frame
 ** \param[in] RecordNumber
 ** \param[in] BufSize
 ** \param[out] DestBuffer
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(Std_ReturnType, DEM_CODE)
  Dem_CollectFFInfo(
    Dem_EventIdType                         EventId,
    Dem_FFKindType                          FFKind,
    uint8                                   RecordNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  DestBuffer);

/** \brief Function to calculate the size of the FF info data
 **
 ** \param[in] EventId - Event for witch to get FF size info
 ** \param[in] FFKind  - Kind of freeze frame
 ** \param[in] RecordNumber - RecordNumber is unique per DTC,
 **                           the value 0xFF is for overall size
 **
 ** \return Size of FF info kind
 ** size remains 0 if:
 **    - the record number is valid, but no entry
 **    - the FF record is not stored
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(uint16, DEM_CODE) Dem_SizeOfFFInfo(
  Dem_EventIdType                         EventId,
  Dem_FFKindType                          FFKind,
  uint8                                   RecordNumber);

/** \brief Gets the next freeze frame record for reporting to Dcm
 **
 ** This function searches the regular FF, DevFF A and DevFF B record numbers
 ** (if configured) of the event and returns the least record number that is
 ** equal to or greater than the record number being searched. This ensures
 ** that the freeze frame records are output in the numeric ascending order.
 ** The search is based only on the configured record numbers, without
 ** considering whether it is indeed stored or even a freeze frame class is
 ** configured.
 ** \param[in] EventId              Event whose freeze frame records shall be
 **                                 searched.
 ** \param[in] RecordNumberToSearch Freeze frame record number which shall be
 **                                 searched. If an exact match is not found,
 **                                 then the next highest record number, if
 **                                 available, is found.
 ** \param[out] FoundRecordNumber   Record number of the found freeze frame
 **                                 record.
 ** \param[out] FFKind              Kind of freeze frame that is found.
 **
 ** \return Result of the search
 ** \retval TRUE  Search successful, FoundRecordNumber and FFKind are valid.
 ** \retval FALSE Search failed, FoundRecordNumber and FFKind are invalid.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(boolean, DEM_CODE) Dem_FindNextFFRecForReporting(
  Dem_EventIdType                                 EventId,
  uint8                                           RecordNumberToSearch,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)          FoundRecordNumber,
  P2VAR(Dem_FFKindType, AUTOMATIC, DEM_APPL_DATA) FFKind);
#endif /* (DEM_NUM_FFSEGS > 0U) */

#if ((DEM_NUM_OF_PIDS_IN_OBDFF > 0U) && (DEM_NUM_FFSEGS > 0U))
/** \brief Function to copy OBD freeze frame data destination buffer
 **
 ** This function shall be called only for emission relevant events.
 **
 ** \param[in] EventId
 ** \param[in] RecordNumber
 ** \param[in] BufSize
 ** \param[out] DestBuffer
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(Std_ReturnType, DEM_CODE)
  Dem_CollectOBDFreezeFrameInfo(
    Dem_EventIdType                         EventId,
    uint8                                   RecordNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  DestBuffer);

/** \brief Function to calculate size of OBD freeze frame info
 **
 ** \param[in] EventId
 **
 ** \return Size of OBD FF info
 ** Size remains 0 if the record number is valid, but no entry exists for EventId.
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(uint16, DEM_CODE)
  Dem_SizeOfOBDFreezeFrameInfo(
    Dem_EventIdType                         EventId);

/** \brief Function to copy all OBD freeze frame PIDs and values
 **   into the destination buffer
 **
 ** If no buffer is provided the function calculates size and number of PID.
 **
 ** \param[out] DestBuffer Buffer for OBD freeze frame
 ** \param[out] BufSize
 ** \param[in] SelectedEntryIndex
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(uint8, DEM_CODE) Dem_CopyOBDFreezeFrameInfo(
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  DestBuffer,
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize,
  Dem_SizeEvMemEntryType                  SelectedEntryIdx);
#endif /* ((DEM_NUM_OF_PIDS_IN_OBDFF > 0U) && (DEM_NUM_FFSEGS > 0U)) */

#if (DEM_NUM_EDSEGS > 0U)
/** \brief Gets the next extended data record for reporting to Dcm
 **
 ** This function searches all the extended segments within the given extended
 ** data class and returns the segment with the least record number that is
 ** equal to or greater than the record number being searched. This ensures
 ** that the extended data records are output in the numeric ascending order.
 ** The search is based only on the configured record numbers, without
 ** considering whether it is indeed stored.
 **
 ** \param[in] EDClass              Pointer to the extended data class within
 **                                 which the search shall be performed.
 ** \param[in] RecordNumberToSearch Extended data record number which shall be
 **                                 searched. If an exact match is not found,
 **                                 then the segment with the next highest
 **                                 record number, if available, is found.
 ** \param[out] FoundEDSegment      Pointer to the found EDSegment.
 ** \param[out] EDStartByte         Start byte position of the FoundEDSegment.
 **
 ** \return Result of the search
 ** \retval TRUE  Search successful, FoundEDSegment and EDStartByte are valid.
 ** \retval FALSE Search failed, FoundEDSegment and EDStartByte are invalid.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(boolean, DEM_CODE) Dem_FindNextEDRecForReporting(
  P2CONST(Dem_EDClassType, AUTOMATIC, DEM_CONST)             EDClass,
  uint8                                                      RecordNumberToSearch,
  P2VAR(Dem_EDSegmentPtrConstType, AUTOMATIC, DEM_APPL_DATA) FoundEDSegment,
  P2VAR(Dem_EDStartByteType, AUTOMATIC, DEM_APPL_DATA)       EDStartByte);
#endif /* DEM_NUM_EDSEGS > 0U */

#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define DEM_START_SEC_VAR_CLEARED_16
#include <Dem_MemMap.h>

/** \brief Event corresponding to record update disabled DTC */
VAR(Dem_EventIdType, DEM_VAR_CLEARED) Dem_EvIdOfLockedMemoryEntry;

#define DEM_STOP_SEC_VAR_CLEARED_16
#include <Dem_MemMap.h>

/*==================[internal data]=========================================*/

#define DEM_START_SEC_VAR_CLEARED_8
#include <Dem_MemMap.h>

#if (DEM_NUMBER_OF_CLIENTS > 1U)
/** \brief ID of the diagnostic client which locked the DTC record update
 **
 **  The value DEM_CLIENT_ID_INVALID means the DTC record update is
 **  enabled (unlocked).
 */
STATIC VAR(uint8, DEM_VAR_CLEARED) Dem_ClientIdLockingDTCRecordUpdate;
#endif

/** \brief Array holding the FF/ED record selection related information of all
 ** the diagnostic clients. */
/* !LINKSTO Dem.ASR431.SWS_Dem_01251,1 */
STATIC VAR(Dem_RecordSelectionType, DEM_VAR_CLEARED)
  Dem_RecordSelection[DEM_NUMBER_OF_CLIENTS];

#define DEM_STOP_SEC_VAR_CLEARED_8
#include <Dem_MemMap.h>

/*==================[external function definitions]=========================*/

#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

/*---------------------[Dem_InitEventMemoryProcessor]-----------------------*/

FUNC(void, DEM_CODE) Dem_InitEventMemoryProcessor(void)
{
  DBG_DEM_INITEVENTMEMORYPROCESSOR_ENTRY();

#if (DEM_NUMBER_OF_CLIENTS > 1U)
  Dem_ClientIdLockingDTCRecordUpdate = DEM_CLIENT_ID_INVALID;
#endif
  Dem_EvIdOfLockedMemoryEntry = DEM_EVENT_ID_INVALID;

  DBG_DEM_INITEVENTMEMORYPROCESSOR_EXIT();
}

/*---------------------[Dem_DisableDTCRecordUpdate]-------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DisableDTCRecordUpdate(uint8 ClientId)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_DISABLEDTCRECORDUPDATE_ENTRY(ClientId);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DisableDTCRecordUpdate, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DisableDTCRecordUpdate, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_DisableDTCRecordUpdate, DEM_E_WRONG_CONFIGURATION);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    Dem_DTCSelectionType DTCSelection;
    Dem_GetDTCSelection(ClientId, &DTCSelection);

    /* check if Dem_SelectDTC() was called before calling this API */
    if (DTCSelection.SelectorStatus == DEM_DTC_SELECTOR_NO_REQUEST)
    {
      /* !LINKSTO Dem.ASR431.SWS_Dem_01299,1 */
      DEM_REPORT_ERROR(DEM_SID_DisableDTCRecordUpdate, DEM_E_WRONG_CONDITION);
    }
    else
#endif /* DEM_DEV_ERROR_DETECT */
    {
      Dem_EventIdType EventId;

      switch(Dem_DoDTCSelection(ClientId, &EventId))
      {
        case DEM_DTC_SELECTOR_WRONG_DTC:
        case DEM_DTC_SELECTOR_DTC_GROUP:
          /* a wrong DTC or group of DTCs was selected */
          Result = DEM_WRONG_DTC;
          break;
        case DEM_DTC_SELECTOR_WRONG_DTC_ORIGIN:
          /* !LINKSTO Dem.ASR431.SWS_Dem_01262,1 */
          Result = DEM_WRONG_DTCORIGIN;
          break;
        case DEM_DTC_SELECTOR_SINGLE_DTC_EVID_CACHED:
          {
            const Dem_EventIdType MasterEventId =
              DEM_GET_MASTER_EVID(EventId);

#if (DEM_NUMBER_OF_CLIENTS > 1U)
            /*
             * ENTER critical section to protect consistency of the two variables
             *       Dem_EvIdOfLockedMemoryEntry and Dem_ClientIdLockingDTCRecordUpdate
             *       call-context: Dcm (Task)
             */
            DEM_ENTER_EXCLUSIVE_AREA();

            if ( (Dem_EvIdOfLockedMemoryEntry != DEM_EVENT_ID_INVALID) &&
                 (Dem_ClientIdLockingDTCRecordUpdate != ClientId) )
            {
               /* another client disabled the DTC record update */
               /* Det error is reported outside of the critical section */
               /* Result remains E_NOT_OK */
            }
            else if ( (Dem_EvIdOfLockedMemoryEntry != MasterEventId) &&
                      (Dem_ClientIdLockingDTCRecordUpdate == ClientId) )
            {
              /* the current client disabled record update for another DTC */
              /* Det error is reported outside of the critical section */
              /* Result remains E_NOT_OK */
            }
            else
            {
              /* DTC record update is either not disabled or already disabled
               * by the current client and for the same DTC. */
              /* disable DTC record update */
              Dem_ClientIdLockingDTCRecordUpdate = ClientId;
              Dem_EvIdOfLockedMemoryEntry = MasterEventId;
              Result = E_OK;
            }

            /*
             * EXIT critical section
             */
            DEM_EXIT_EXCLUSIVE_AREA();
#else
            if ( (Dem_EvIdOfLockedMemoryEntry == DEM_EVENT_ID_INVALID) ||
                 (Dem_EvIdOfLockedMemoryEntry == MasterEventId) )
            {
              /* DTC record update is either not disabled or already disabled
               * for the same DTC selection. */
              /* disable DTC record update */
              Dem_EvIdOfLockedMemoryEntry = MasterEventId;
              Result = E_OK;
            }
#endif /* (DEM_NUMBER_OF_CLIENTS > 1U) */

#if (DEM_DEV_ERROR_DETECT == STD_ON)
            if (Result == E_NOT_OK)
            {
              /* !LINKSTO Dem.ASR431.SWS_Dem_00648,1 */
              /* !LINKSTO Dem.ASR431.DTCRecordUpdate.ClientIdLocksEventMemoryUpdate,1 */
              DEM_REPORT_ERROR(DEM_SID_DisableDTCRecordUpdate, DEM_E_WRONG_CONDITION);
            }
#endif
          }
          break;
        /* CHECK: NOPARSE */
        default: /* should never be reached */
          DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_DisableDTCRecordUpdate);
          /* Result remains E_NOT_OK */
          break;
        /* CHECK: PARSE */
      }
    }
  }

  DBG_DEM_DISABLEDTCRECORDUPDATE_EXIT(Result, ClientId);
  return Result;
}

/*---------------------[Dem_EnableDTCRecordUpdate]--------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_EnableDTCRecordUpdate(uint8 ClientId)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_ENABLEDTCRECORDUPDATE_ENTRY(ClientId);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_EnableDTCRecordUpdate, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_EnableDTCRecordUpdate, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_EnableDTCRecordUpdate, DEM_E_WRONG_CONFIGURATION);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    Dem_DTCSelectionType DTCSelection;
    Dem_GetDTCSelection(ClientId, &DTCSelection);

    /* check if Dem_SelectDTC() was called before calling this API */
    if (DTCSelection.SelectorStatus == DEM_DTC_SELECTOR_NO_REQUEST)
    {
      /* !LINKSTO Dem.ASR431.SWS_Dem_01299,1 */
      DEM_REPORT_ERROR(DEM_SID_EnableDTCRecordUpdate, DEM_E_WRONG_CONDITION);
    }
    else
#endif /* DEM_DEV_ERROR_DETECT */
    {
      Dem_EventIdType LockedEvId;

#if ( (DEM_USE_AGING == STD_ON) && \
      (DEM_EXTENDED_DATA_CAPTURE != DEM_TRIGGER_ON_FDC_THRESHOLD) )
      Dem_EventMemoryEntryPtrType EvMemEntry;
#if ( (DEM_NUM_CMB_DTCS > 0U) || (DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON) )
      boolean AgedEventFlag = FALSE;
#endif
#if (DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON)
      Dem_SizeEvMemEntryType EvMemEntryIdx;
#endif
#if ( (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON) || \
      (DEM_NUM_CMB_DTCS > 0U) )
      Dem_DTCStatusMaskType OldDTCStatusMask = 0U;
      Dem_DTCStatusMaskType NewDTCStatusMask = 0U;
#endif
#endif /* (DEM_USE_AGING == STD_ON) &&
          (DEM_EXTENDED_DATA_CAPTURE != DEM_TRIGGER_ON_FDC_THRESHOLD) */

      /*
       * ENTER critical section to protect consistency of the two variables
       *       Dem_EvIdOfLockedMemoryEntry and Dem_ClientIdLockingDTCRecordUpdate
       *       and to protect the event memory entry
       *       call-context: Dcm (Task)
       */
      DEM_ENTER_EXCLUSIVE_AREA();

      LockedEvId = Dem_EvIdOfLockedMemoryEntry;

      /* Due to SWS_Dcm_00371, it is expected that the DTC selection is not changed between
       * disable and enable of DTC record update.
       * However, for the purpose of defensive programming, the enabling of DTC record update
       * won't be conditioned by the validation steps performed on selector data. */
      if (DEM_EVENT_ID_INVALID == LockedEvId)
      {
        /* DTC record update is already enabled */
        Result = E_OK;
      }
#if (DEM_NUMBER_OF_CLIENTS > 1U)
      else if (Dem_ClientIdLockingDTCRecordUpdate != ClientId)
      {
        /* DTCRecordUpdate for the requested DTC was disabled by another Client.
         * Det error is reported outside of the critical section.
         * Result remains E_NOT_OK. */
      }
#endif
      else
      {
        /* this client has previously disabled the DTC record update and
         * therefore is allowed to enable it */
        /* enable DTC record update */
        /* !LINKSTO Dem271,3 */
        Dem_EvIdOfLockedMemoryEntry = DEM_EVENT_ID_INVALID;
#if (DEM_NUMBER_OF_CLIENTS > 1U)
        Dem_ClientIdLockingDTCRecordUpdate = DEM_CLIENT_ID_INVALID;
#else
#if (DEM_DEV_ERROR_DETECT == STD_OFF)
        TS_PARAM_UNUSED(ClientId);
#endif
#endif
#if ( (DEM_USE_AGING == STD_ON) && \
      (DEM_EXTENDED_DATA_CAPTURE != DEM_TRIGGER_ON_FDC_THRESHOLD) )
        /* clear disabled event from event memory if it was aged */
        if (DEM_AGING_ALLOWED(LockedEvId))
        {
#if (DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON)
          if (Dem_SearchForEntry(LockedEvId, &EvMemEntry, &EvMemEntryIdx) == TRUE)
#else
          if (Dem_SearchForEntry(LockedEvId, &EvMemEntry, NULL_PTR) == TRUE)
#endif
          {
            /* pointer to internal aging value */
            const Dem_EntryDataPtrType AgingCounter =
              Dem_IntValEntryData(EvMemEntry);

#if (DEM_NUM_CMB_DTCS > 0U)
            if (DEM_IS_EVENT_COMBINED(LockedEvId))
            {
              /* get the EventId of the stored event involved in combination */
              LockedEvId = EvMemEntry->EventId;
            }
#endif
            /* !LINKSTO SWS_Dem_01075,1 */
            if (*AgingCounter == 0U)
            {
#if ( (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON) || \
      (DEM_NUM_CMB_DTCS > 0U) )
              OldDTCStatusMask = DEM_DTC_STATUS_MASKS[LockedEvId];
#endif

#if ( (DEM_NUM_CMB_DTCS > 0U) || (DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON) )
              AgedEventFlag = TRUE;
#endif

              Dem_ClearAgedEventEntry(LockedEvId, EvMemEntry);

#if (DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON)
              /* 1st step: prepare the gate entry to transfer data to NvM */
              Dem_CheckForImmediateEntryStorage(LockedEvId,
                                                EvMemEntry,
                                                EvMemEntryIdx,
                                                DEM_NVM_NO_FORCE);
#endif

#if ( (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON) || \
      (DEM_NUM_CMB_DTCS > 0U) )
              NewDTCStatusMask = DEM_DTC_STATUS_MASKS[LockedEvId];
#endif
            }
          }
        }
#endif /* (DEM_USE_AGING == STD_ON) &&
          (DEM_EXTENDED_DATA_CAPTURE != DEM_TRIGGER_ON_FDC_THRESHOLD) */

        Result = E_OK;
      }

      /*
       * EXIT critical section
       */
      DEM_EXIT_EXCLUSIVE_AREA();

#if ( (DEM_USE_AGING == STD_ON) && \
      (DEM_EXTENDED_DATA_CAPTURE != DEM_TRIGGER_ON_FDC_THRESHOLD) )
      /* Deviation TASKING-1 */
      if (Result == E_OK)
      {
#if ( (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON) || \
      (DEM_NUM_CMB_DTCS > 0U) )
        const boolean CurrentEventStatusChanged =
          (OldDTCStatusMask != NewDTCStatusMask) ? TRUE : FALSE;
#if (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON)
        if (CurrentEventStatusChanged)
        {
          Dem_CbTriggerOnEventStatus(LockedEvId, OldDTCStatusMask, NewDTCStatusMask);
        }
#endif
#if (DEM_NUM_CMB_DTCS > 0U)
        if (DEM_IS_EVENT_COMBINED(LockedEvId) && (AgedEventFlag == TRUE))
        {
          Dem_ResetStatusForCmbEvents(LockedEvId, CurrentEventStatusChanged);
        }
#endif /* (DEM_NUM_CMB_DTCS > 0U) */
#endif /* (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON) || \
          (DEM_NUM_CMB_DTCS > 0U) */

#if (DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON)
        /* 2nd step: trigger the requests to the NvM module once
         * for all events for performance reasons
         * always called outside the critical section */
        if (AgedEventFlag == TRUE)
        {
          Dem_CheckForTriggerNvmWriteGateEntry();
        }
#endif /* DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON */
      }
#endif /* (DEM_USE_AGING == STD_ON) &&
          (DEM_EXTENDED_DATA_CAPTURE != DEM_TRIGGER_ON_FDC_THRESHOLD) */

#if (DEM_DEV_ERROR_DETECT == STD_ON)
      if (Result == E_NOT_OK)
      {
        /* !LINKSTO Dem.ASR431.DTCRecordUpdate.ClientIdLocksEventMemoryUpdate,1 */
        /* DTC record update was disabled by another client */
        DEM_REPORT_ERROR(DEM_SID_EnableDTCRecordUpdate, DEM_E_WRONG_CONDITION);
      }
#endif
    }
  }

  DBG_DEM_ENABLEDTCRECORDUPDATE_EXIT(Result, ClientId);
  return Result;
}

/*---------------------[Dem_InitDataRecordSelector]-------------------------*/

FUNC(void, DEM_CODE) Dem_InitDataRecordSelector(void)
{
  uint8 ClientId;

  DBG_DEM_INITDATARECORDSELECTOR_ENTRY();

  /* Deviation TASKING-2 */
  for (ClientId = 0U; ClientId < DEM_NUMBER_OF_CLIENTS; ClientId++)
  {
    Dem_RecordSelection[ClientId].Status = DEM_DATA_RECORD_SELECTOR_NONE;
  }

  DBG_DEM_INITDATARECORDSELECTOR_EXIT();
}

/*---------------------[Dem_ResetDataRecordSelector]------------------------*/

FUNC(void, DEM_CODE) Dem_ResetDataRecordSelector(uint8 ClientId)
{
  DBG_DEM_RESETDATARECORDSELECTOR_ENTRY(ClientId);

  Dem_RecordSelection[ClientId].Status = DEM_DATA_RECORD_SELECTOR_NONE;

  DBG_DEM_RESETDATARECORDSELECTOR_EXIT(ClientId);
}

/*---------------------[Dem_SelectFreezeFrameData]--------------------------*/

/* !LINKSTO Dem.ASR431.DTCSelectorDependentAPIs,1 */
FUNC(Std_ReturnType, DEM_CODE) Dem_SelectFreezeFrameData(
  uint8 ClientId,
  uint8 RecordNumber)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_SELECTFREEZEFRAMEDATA_ENTRY(ClientId, RecordNumber);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_SelectFreezeFrameData, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_SelectFreezeFrameData, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_SelectFreezeFrameData, DEM_E_WRONG_CONFIGURATION);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    Dem_DTCSelectionType DTCSelection;
    Dem_GetDTCSelection(ClientId, &DTCSelection);

    /* check if Dem_SelectDTC() was called before calling this API */
    if (DTCSelection.SelectorStatus == DEM_DTC_SELECTOR_NO_REQUEST)
    {
      /* !LINKSTO Dem.ASR431.SWS_Dem_01299,1 */
      DEM_REPORT_ERROR(DEM_SID_SelectFreezeFrameData, DEM_E_WRONG_CONDITION);
    }
    else
#endif /* DEM_DEV_ERROR_DETECT */
    {
      Dem_EventIdType EventId;

      switch (Dem_DoDTCSelection(ClientId, &EventId))
      {
        case DEM_DTC_SELECTOR_SINGLE_DTC_EVID_CACHED:
          /* !LINKSTO Dem.ASR431.SWS_Dem_01268,1 */
          Dem_RecordSelection[ClientId].SelectedRecNum = RecordNumber;

          Dem_RecordSelection[ClientId].NextRecNum =
            (RecordNumber == DEM_ALL_RECORD_NUM) ? DEM_MIN_FF_RECORD_NUM
                                                 : RecordNumber;

          Dem_RecordSelection[ClientId].Status = DEM_DATA_RECORD_SELECTOR_FF;

          Result = E_OK;
          break;

        case DEM_DTC_SELECTOR_WRONG_DTC:
        case DEM_DTC_SELECTOR_DTC_GROUP:
          /* !LINKSTO Dem.ASR431.DTCSelectorWrongDTC,1 */
          /* !LINKSTO Dem.DTCSelector.DependentAPIs.SupportedDTCFormat,1 */
          Result = DEM_WRONG_DTC;
          break;

        case DEM_DTC_SELECTOR_WRONG_DTC_ORIGIN:
          /* !LINKSTO Dem.ASR431.SWS_Dem_01262,1 */
          Result = DEM_WRONG_DTCORIGIN;
          break;

        /* CHECK: NOPARSE */
        default: /* should never be reached */
          DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_SelectFreezeFrameData);
          Result = DEM_WRONG_DTC;
          break;
        /* CHECK: PARSE */
      }
    }
  }

  DBG_DEM_SELECTFREEZEFRAMEDATA_EXIT(Result, ClientId, RecordNumber);
  return Result;
}

/*---------------------[Dem_SelectExtendedDataRecord]-----------------------*/

/* !LINKSTO Dem.ASR431.DTCSelectorDependentAPIs,1 */
FUNC(Std_ReturnType, DEM_CODE) Dem_SelectExtendedDataRecord(
  uint8 ClientId,
  uint8 ExtendedDataNumber)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_SELECTEXTENDEDDATARECORD_ENTRY(ClientId, ExtendedDataNumber);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_SelectExtendedDataRecord, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_SelectExtendedDataRecord, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_SelectExtendedDataRecord,
      DEM_E_WRONG_CONFIGURATION);
  }
  else if ( (ExtendedDataNumber < DEM_MIN_ED_RECORD_NUM) ||
            ( (ExtendedDataNumber > DEM_MAX_ED_RECORD_NUM) &&
              (ExtendedDataNumber < DEM_ALL_OBD_ED_RECORD_NUM) ) )
  {
    DEM_REPORT_ERROR(DEM_SID_SelectExtendedDataRecord, DEM_E_PARAM_DATA );
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    Dem_DTCSelectionType DTCSelection;
    Dem_GetDTCSelection(ClientId, &DTCSelection);

    /* check if Dem_SelectDTC() was called before calling this API */
    if (DTCSelection.SelectorStatus == DEM_DTC_SELECTOR_NO_REQUEST)
    {
      /* !LINKSTO Dem.ASR431.SWS_Dem_01299,1 */
      DEM_REPORT_ERROR(DEM_SID_SelectExtendedDataRecord,
        DEM_E_WRONG_CONDITION);
    }
    else
#endif /* DEM_DEV_ERROR_DETECT */
    {
      Dem_EventIdType EventId;
      switch (Dem_DoDTCSelection(ClientId, &EventId))
      {
        case DEM_DTC_SELECTOR_SINGLE_DTC_EVID_CACHED:
          /* !LINKSTO Dem.ASR431.SWS_Dem_01272,1 */
          Dem_RecordSelection[ClientId].SelectedRecNum = ExtendedDataNumber;

          if (ExtendedDataNumber == DEM_ALL_RECORD_NUM)
          {
            Dem_RecordSelection[ClientId].NextRecNum = DEM_MIN_ED_RECORD_NUM;
          }
          else if (ExtendedDataNumber == DEM_ALL_OBD_ED_RECORD_NUM)
          {
            Dem_RecordSelection[ClientId].NextRecNum =
              DEM_MIN_OBD_ED_RECORD_NUM;
          }
          else
          {
            Dem_RecordSelection[ClientId].NextRecNum = ExtendedDataNumber;
          }

          Dem_RecordSelection[ClientId].Status = DEM_DATA_RECORD_SELECTOR_ED;

          Result = E_OK;
          break;

        case DEM_DTC_SELECTOR_DTC_GROUP:
        case DEM_DTC_SELECTOR_WRONG_DTC:
          /* !LINKSTO Dem.ASR431.DTCSelectorWrongDTC,1 */
          /* !LINKSTO Dem.DTCSelector.DependentAPIs.SupportedDTCFormat,1 */
          Result = DEM_WRONG_DTC;
          break;

        case DEM_DTC_SELECTOR_WRONG_DTC_ORIGIN:
          /* !LINKSTO Dem.ASR431.SWS_Dem_01262,1 */
          Result = DEM_WRONG_DTCORIGIN;
          break;

        /* CHECK: NOPARSE */
        default: /* should never be reached */
          DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_SelectExtendedDataRecord);
          break;
        /* CHECK: PARSE */
      }
    }
  }

  DBG_DEM_SELECTEXTENDEDDATARECORD_EXIT(Result, ClientId, ExtendedDataNumber);
  return Result;
}

/*---------------------[Dem_GetNextFreezeFrameData]-------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_GetNextFreezeFrameData(
  uint8                                   ClientId,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  DestBuffer,
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_GETNEXTFREEZEFRAMEDATA_ENTRY(ClientId, DestBuffer, BufSize);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextFreezeFrameData, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNextFreezeFrameData, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId > DEM_NUMBER_OF_CLIENTS)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(
      DEM_SID_GetNextFreezeFrameData, DEM_E_WRONG_CONFIGURATION);
  }
  else if ((DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextFreezeFrameData, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    if (Dem_RecordSelection[ClientId].Status == DEM_DATA_RECORD_SELECTOR_FF)
    {
#if (DEM_NUM_FFSEGS > 0U)
      Dem_DTCSelectionType DTCSelection;
      Dem_EventIdType EventId;
#if ((DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U) || (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U))
      Dem_DTCOriginType DTCOrigin;
#endif
      uint8 SelectedRecNum;
      uint8 NextAvailableRecNum = DEM_OBD_FF_RECORD_NUM;

      /* get the event matching the selected DTC and its origin */
      Dem_GetDTCSelection(ClientId, &DTCSelection);
      EventId = DTCSelection.DTCInfo.EventId;
#if ((DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U) || (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U))
      DTCOrigin = DTCSelection.DTCInfo.DTCOrigin;
#endif

      /* !LINKSTO Dem.ASR431.SWS_Dem_01268,1 */
      SelectedRecNum = Dem_RecordSelection[ClientId].SelectedRecNum;

      Result = DEM_NO_SUCH_ELEMENT;

      /* !LINKSTO Dem.Dcm.GetNextFreezeFrameData.RecordNumber0xFF,1 */
      while (Dem_RecordSelection[ClientId].NextRecNum != DEM_END_OF_RECORD_SEARCH)
      {
        uint8 NextRecNum = Dem_RecordSelection[ClientId].NextRecNum;
        /* !LINKSTO Dem.ASR431.SWS_Dem_00576_Implicit1,1 */
#if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)
        /* !LINKSTO dsn.Dem.CheckRecordNumber.SearchForEntry,2 */
        /* !LINKSTO Dem.ASR431.SWS_Dem_00576,1 */
        if (NextRecNum == DEM_OBD_FF_RECORD_NUM)
        {
          if (Dem_GbiDTCKind(EventId) == DEM_DTC_KIND_EMISSION_REL_DTCS)
          {
            /* collect OBD FF record data header info and update result */
            /* !LINKSTO Dem.ASR431.SWS_Dem_00071,1 */
            /* !LINKSTO Dem.ASR431.SWS_Dem_00630,1 */
            Result = Dem_CollectOBDFreezeFrameInfo(EventId,
                                                   NextRecNum,
                                                   BufSize,
                                                   DestBuffer);
          }
        }
        else
#endif /* DEM_NUM_OF_PIDS_IN_OBDFF */
        {
          Dem_FFKindType FFKind = DEM_DEVFF_A;

          /* !LINKSTO dsn.Dem.CheckRecordNumber.SearchForEntry,2 */
          /* search for the next record number for reporting */
          if (FALSE != Dem_FindNextFFRecForReporting(EventId,
                                                     NextRecNum,
                                                     &NextAvailableRecNum,
                                                     &FFKind))
          {
            if ( (SelectedRecNum == DEM_ALL_RECORD_NUM) ||
                 (SelectedRecNum == NextAvailableRecNum) )
            {
#if ((DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U) || (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U))
              if ( ((FFKind == DEM_DEVFF_A) || (FFKind == DEM_DEVFF_B)) &&
                   (DTCOrigin == DEM_DTC_ORIGIN_PRIMARY_MEMORY)         &&
                   (DEM_DEVFF_VALID_FFCLASS(FFKind, EventId)) )
              {
                /* collect FF record data for DEVFF A or B */
                /* !LINKSTO Dem.ASR431.SWS_Dem_00071,1 */
                /* !LINKSTO Dem.ASR431.SWS_Dem_00630,1 */
                /* !LINKSTO dsn.Dem.DevFF.IB.GetNextFreezeFrameData,1 */
                Result = Dem_CollectFFInfo(EventId,
                                           FFKind,
                                           NextAvailableRecNum,
                                           BufSize,
                                           DestBuffer);
              }
              else
#endif /* (DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U) || (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U) */
              /* Deviation MISRAC2012-1 */
              if ( (FFKind == DEM_REGULAR_FF) &&
                   (DEM_REGULAR_FF_VALID_FFCLASS(EventId)) )
              {
                /* collect FF record data for regular FF */
                /* !LINKSTO Dem.ASR431.SWS_Dem_00071,1 */
                /* !LINKSTO Dem.ASR431.SWS_Dem_00630,1 */
                Result = Dem_CollectFFInfo(EventId,
                                           FFKind,
                                           NextAvailableRecNum,
                                           BufSize,
                                           DestBuffer);
              }
              else
              {
#if (DEM_POSITIVE_RESPONSE_NO_FFCLASSES == STD_ON)
                /* positive response no freezeframe classes */
                /* !LINKSTO dsn.Dem.Calibration.NoFFClass.PositiveResponse.GetNextFreezeFrameData,1 */
                *BufSize = 0U;
                Result = E_OK;
#endif
              }
            }
            else
            {
              /* result stays DEM_NO_SUCH_ELEMENT */
            }
          }
          else
          {
            /* no more records are left to be considered, mark end of search */
            Dem_RecordSelection[ClientId].NextRecNum =
                                               DEM_END_OF_RECORD_SEARCH;
            continue;
          }
        }

        if (SelectedRecNum != DEM_ALL_RECORD_NUM)
        {
          /* single record selected, mark end of search */
          Dem_RecordSelection[ClientId].NextRecNum = DEM_END_OF_RECORD_SEARCH;
        }
        else
        {
          if ((Result == E_OK) || (Result == DEM_NO_SUCH_ELEMENT))
          {
            Dem_RecordSelection[ClientId].NextRecNum =
                                               NextAvailableRecNum + 1U;
          }

          if (Result != DEM_NO_SUCH_ELEMENT)
          {
            break;
          }
        }
      }
#else
      TS_PARAM_UNUSED(DestBuffer);
      TS_PARAM_UNUSED(BufSize);
      Result = DEM_NO_SUCH_ELEMENT;
#endif /* DEM_NUM_FFSEGS */
    }
    else
    {
      /* result remains E_NOT_OK */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO Dem.ASR431.SWS_Dem_01271,1 */
      DEM_REPORT_ERROR(DEM_SID_GetNextFreezeFrameData, DEM_E_WRONG_CONDITION);
#endif
    }
  }

  DBG_DEM_GETNEXTFREEZEFRAMEDATA_EXIT(Result, ClientId, DestBuffer, BufSize);
  return Result;
}

/*---------------------[Dem_GetSizeOfFreezeFrameSelection]------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_GetSizeOfFreezeFrameSelection(
  uint8                                   ClientId,
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfFreezeFrame)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_GETSIZEOFFREEZEFRAMESELECTION_ENTRY(ClientId, SizeOfFreezeFrame);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetSizeOfFreezeFrameSelection, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(
      DEM_SID_GetSizeOfFreezeFrameSelection, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId > DEM_NUMBER_OF_CLIENTS)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(
      DEM_SID_GetSizeOfFreezeFrameSelection, DEM_E_WRONG_CONFIGURATION);
  }
  else if (SizeOfFreezeFrame == NULL_PTR)
  {
    DEM_REPORT_ERROR(
      DEM_SID_GetSizeOfFreezeFrameSelection, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    if (Dem_RecordSelection[ClientId].Status == DEM_DATA_RECORD_SELECTOR_FF)
    {
#if (DEM_NUM_FFSEGS > 0U)
      /*
                I) Single record
                    [Note: Size1 + ... + SizeN = Dem_SizeOfFF(FFClass)]
              |----------|---------------------|------|-------|---|------|-------|
      Format  |  RecNum  | NumberOfIdentifiers | DID1 | Data1 |...| DIDN | DataN |
              |----------|---------------------|------|-------|---|------|-------|
      Size in |----------|---------------------|------|-------|---|---- -|-------|
      bytes   |    1     |         1           |  2   | Size1 |...|  2   | SizeN |
              |----------|---------------------|------|-------|---|------|-------|

              \                                                                  /
               \________________________________________________________________/
                        |                    |                       |
                        |                    |                       |
                 II)  RecordNumber = 0xFF    |                       |
                    [Note: N = Number of stored FF records]          |
              |-------------------|-------------------|----|---------------------------|-----------|--------|
      Format  |       Record1     |       Record2     | .. |      RecordN              |   OBD FF  | DEV FF |
              |-------------------|-------------------|----|---------------------------|-----------|--------|
      Size in |-------------------|-------------------|----|---------------------------|-----------|--------|
       bytes  |  SizeOf(Record1)  |  SizeOf(Record2)  | .. |     SizeOf(RecordN)       | OBDFF Size|DEV Size|
              |-------------------|-------------------|----|------------------------------------------------|
      */

      Dem_DTCSelectionType DTCSelection;
#if ((DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U) || (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U))
      Dem_DTCOriginType DTCOrigin;
#endif
      Dem_EventIdType EventId;
      uint16 SizeOfFF = 0U;
      uint8 SelectedRecNum;

      Result = E_OK;

      Dem_GetDTCSelection(ClientId, &DTCSelection);
      EventId = DTCSelection.DTCInfo.EventId;
#if ((DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U) || (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U))
      DTCOrigin = DTCSelection.DTCInfo.DTCOrigin;
#endif

      /* !LINKSTO Dem.ASR431.SWS_Dem_01268,1 */
      SelectedRecNum = Dem_RecordSelection[ClientId].SelectedRecNum;

      /* !LINKSTO dsn.Dem.CheckRecordNumber.SearchForEntry,2 */
      /* !LINKSTO Dem.ASR431.SWS_Dem_00074,1 */
#if (DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U)
      /* reading the Freeze Frame A Class record number */
      if (DEM_DEVFF_VALID(DTCOrigin, DEM_DEVFF_A, EventId, SelectedRecNum))
      {
        /* !LINKSTO dsn.Dem.DevFF.IB.GetSizeOfFreezeFrameSelection,1 */
        SizeOfFF = Dem_SizeOfFFInfo(EventId, DEM_DEVFF_A, SelectedRecNum);
      }
      else
#endif
#if (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U)
      /* reading the Freeze Frame B Class record number */
      if (DEM_DEVFF_VALID(DTCOrigin, DEM_DEVFF_B, EventId, SelectedRecNum))
      {
        /* !LINKSTO dsn.Dem.DevFF.IB.GetSizeOfFreezeFrameSelection,1 */
        SizeOfFF = Dem_SizeOfFFInfo(EventId, DEM_DEVFF_B, SelectedRecNum);
      }
      else
#endif
#if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)
      if ( (Dem_GbiDTCKind(EventId) == DEM_DTC_KIND_EMISSION_REL_DTCS) &&
           (SelectedRecNum == DEM_OBD_FF_RECORD_NUM) )
      {
        /* size of OBD FF */
        SizeOfFF = Dem_SizeOfOBDFreezeFrameInfo(EventId);
      }
      else
#endif
      /* check if record number is valid for this DTC */
      /* Deviation MISRAC2012-1 */
      if (DEM_REGULAR_FF_VALID(EventId, SelectedRecNum))
      {
        /* regular FF only */
        SizeOfFF = Dem_SizeOfFFInfo(EventId, DEM_REGULAR_FF, SelectedRecNum);
      }
      else if (SelectedRecNum == DEM_ALL_RECORD_NUM)
      {
        /* !LINKSTO Dem.ASR431.SWS_Dem_00074_Implicit1,1 */
        /* !LINKSTO dsn.Dem.DevFF.IB.GetSizeOfFreezeFrameSelection,1 */
#if (DEM_DEV_FREEZE_FRAME_USED == STD_ON)
#if (DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U)
        /* reading the size of Freeze Frame A Class record number */
        SizeOfFF += Dem_SizeOfFFInfo(EventId, DEM_DEVFF_A, SelectedRecNum);
#endif
#if (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U)
        /* reading the size of Freeze Frame B Class record number */
        SizeOfFF += Dem_SizeOfFFInfo(EventId, DEM_DEVFF_B, SelectedRecNum);
#endif
#endif /* DEM_DEV_FREEZE_FRAME_USED == STD_ON */
#if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)
        if (Dem_GbiDTCKind(EventId) == DEM_DTC_KIND_EMISSION_REL_DTCS)
        {
          SizeOfFF += Dem_SizeOfOBDFreezeFrameInfo(EventId);
        }
#endif

        /* includes size of every regular FF */
        SizeOfFF += Dem_SizeOfFFInfo(EventId, DEM_REGULAR_FF, SelectedRecNum);
      }
#if (DEM_POSITIVE_RESPONSE_NO_FFCLASSES == STD_ON)
#if ((DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U) || (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U))
      /* !LINKSTO dsn.Dem.Calibration.NoFFClass.PositiveResponse.GetSizeOfFreezeFrameSelection,1 */
      else if ( (DEM_DEVFF_VALID_RECNUM(DTCOrigin,
                                        DEM_DEVFF_A,
                                        EventId,
                                        SelectedRecNum)) ||
                /* Deviation MISRAC2012-1 */
                (DEM_DEVFF_VALID_RECNUM(DTCOrigin,
                                        DEM_DEVFF_B,
                                        EventId,
                                        SelectedRecNum)) ||
                (DEM_REGULAR_FF_VALID_RECNUM(EventId, SelectedRecNum)) )
#else
      else if (DEM_REGULAR_FF_VALID_RECNUM(EventId, SelectedRecNum))
#endif /* (DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U) || (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U) */
      {
        /* result remains E_OK */
      }
#endif /* (DEM_POSITIVE_RESPONSE_NO_FFCLASSES == STD_ON) */
      else
      {
        /* one non-existing record requested */
        Result = DEM_NO_SUCH_ELEMENT;
      }

      *SizeOfFreezeFrame = SizeOfFF;
#else
      TS_PARAM_UNUSED(SizeOfFreezeFrame);
      Result = DEM_NO_SUCH_ELEMENT;
#endif /* (DEM_NUM_FFSEGS > 0U) */
    }
    else
    {
      /* result remains E_NOT_OK */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO Dem.ASR431.SWS_Dem_01271,1 */
      DEM_REPORT_ERROR(
        DEM_SID_GetSizeOfFreezeFrameSelection, DEM_E_WRONG_CONDITION);
#endif
    }
  }

  DBG_DEM_GETSIZEOFFREEZEFRAMESELECTION_EXIT(
    Result, ClientId, SizeOfFreezeFrame);

  return Result;
}

/*---------------------[Dem_GetNextExtendedDataRecord]----------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_GetNextExtendedDataRecord(
  uint8                                   ClientId,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  DestBuffer,
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_GETNEXTEXTENDEDDATARECORD_ENTRY(ClientId, DestBuffer, BufSize);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextExtendedDataRecord, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNextExtendedDataRecord, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNextExtendedDataRecord,
      DEM_E_WRONG_CONFIGURATION);
  }
  else if ( (DestBuffer == NULL_PTR) || (BufSize == NULL_PTR) )
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextExtendedDataRecord, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    if (Dem_RecordSelection[ClientId].Status == DEM_DATA_RECORD_SELECTOR_ED)
    {
#if (DEM_NUM_EDSEGS > 0U)
      Dem_EventMemoryEntryPtrType EvMemEntry;
      P2CONST(Dem_EDClassType, AUTOMATIC, DEM_CONST) EDClass;
      Dem_DTCSelectionType DTCSelection;
      Dem_EventIdType EventId;
      const uint8 SelectedRecNum =
        Dem_RecordSelection[ClientId].SelectedRecNum;

      Result = DEM_NO_SUCH_ELEMENT;

      Dem_GetDTCSelection(ClientId, &DTCSelection);
      EventId = DTCSelection.DTCInfo.EventId;
      /* get extended data class layout */
      EDClass = &Dem_EDClass[Dem_GbiEDClassIdx(EventId)];

      /* search till end of search is indicated */
      if (Dem_RecordSelection[ClientId].NextRecNum != DEM_END_OF_RECORD_SEARCH)
      {
        P2CONST(Dem_EDSegmentType, AUTOMATIC, DEM_CONST) EDSegment;
        Dem_EDStartByteType EDStartByte;

        /* search for next smallest valid record number */
        if (FALSE != Dem_FindNextEDRecForReporting(
                       EDClass,
                       Dem_RecordSelection[ClientId].NextRecNum,
                       &EDSegment,
                       &EDStartByte))
        {
          uint8 NextAvailableRecNum = EDSegment->RecNum;

          /* !LINKSTO dsn.Dem.CheckRecordNumber.SearchForEntry,2 */
          /* !LINKSTO Dem.Dcm.GetNextExtendedDataRecord.RecordNumber0xFE,1 */
          /* !LINKSTO Dem.Dcm.GetNextExtendedDataRecord.RecordNumber0xFF,1 */
          if ( (SelectedRecNum == DEM_ALL_RECORD_NUM) ||
               (SelectedRecNum == DEM_ALL_OBD_ED_RECORD_NUM) ||
               (SelectedRecNum == NextAvailableRecNum) )
          {
            /* NextAvailableRecNum is relevant for reporting */
            boolean EventMemEntryAvail;

            /*
             * ENTER critical section to protect event memory entry
             */
            DEM_ENTER_EXCLUSIVE_AREA();

            /* search for the event in event memory */
            EventMemEntryAvail =
              Dem_SearchForEntry(EventId, &EvMemEntry, NULL_PTR);

            /* !LINKSTO Dem.Dcm.ExtendedDataRecord.DataAlwaysAvailable,1 */
            if ( (EventMemEntryAvail != FALSE) ||
                 (EDSegment->AlwaysAvailable != FALSE) )
            {
              /* calculate needed buffer size
               * (start with 1 to account for the record number) */
              Dem_SizeEDType NeededSize = 1U;

#if (DEM_USE_MAPPED_INT_DATA_ELEMENTS == STD_ON)
              NeededSize += Dem_SizeOfMapIntValSegment(EDSegment);
#endif
              NeededSize += Dem_SizeOfEDSegment(EDSegment);

              if ((Dem_SizeEDType)*BufSize < NeededSize)
              {
                Result = DEM_BUFFER_TOO_SMALL;
              }
              else
              {
                /* !LINKSTO Dem.ASR431.SWS_Dem_00075,1 */
                /* write data for one record: */
                /* byte 0: record number */
                DestBuffer[0U] = NextAvailableRecNum;
                /* copy content of the record into the destination buffer */
                (void)Dem_AssembleExtendedDataSeg(EventId,
                                                  EDSegment,
                                                  EDStartByte,
                                                  &DestBuffer[1U],
                                                  EvMemEntry);

                /* store size of buffer */
                *BufSize = NeededSize;
                Result = E_OK;
              }
            }
            else
            {
              /* !LINKSTO Dem.Dcm.ExtendedDataRecord.DataAlwaysAvailable,1 */
              *BufSize = 0U;
              Result = E_OK;
            }

            /*
             * LEAVE critical section
             */
            DEM_EXIT_EXCLUSIVE_AREA();
          }

          if (SelectedRecNum <= DEM_MAX_ED_RECORD_NUM)
          {
            /* single ED record is selected */
            Dem_RecordSelection[ClientId].NextRecNum =
              DEM_END_OF_RECORD_SEARCH;
          }
          else
          {
            if (Result == E_OK)
            {
              Dem_RecordSelection[ClientId].NextRecNum =
                NextAvailableRecNum + 1U;
            }
          }
        }
        else
        {
          Dem_RecordSelection[ClientId].NextRecNum = DEM_END_OF_RECORD_SEARCH;
        }
      }
#else
      TS_PARAM_UNUSED(DestBuffer);
      TS_PARAM_UNUSED(BufSize);
      Result = DEM_NO_SUCH_ELEMENT;
#endif /* (DEM_NUM_EDSEGS > 0U) */
    }
    else
    {
      /* the API is not called in the correct sequence */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO Dem.ASR431.SWS_Dem_01274,1 */
      DEM_REPORT_ERROR(DEM_SID_GetNextExtendedDataRecord,
        DEM_E_WRONG_CONDITION);
#endif
      /*  Result stays E_NOT_OK */
    }
  }

  DBG_DEM_GETNEXTEXTENDEDDATARECORD_EXIT(
    Result, ClientId, DestBuffer, BufSize);
  return Result;
}

/*---------------------[Dem_GetSizeOfExtendedDataRecordSelection]-----------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_GetSizeOfExtendedDataRecordSelection(
  uint8                                   ClientId,
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SizeOfExtendedDataRecord)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_GETSIZEOFEXTENDEDDATARECORDSELECTION_ENTRY(
    ClientId, SizeOfExtendedDataRecord);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetSizeOfExtendedDataRecordSelection,
      DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_GetSizeOfExtendedDataRecordSelection,
      DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_GetSizeOfExtendedDataRecordSelection,
      DEM_E_WRONG_CONFIGURATION);
  }
  else if (SizeOfExtendedDataRecord == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_GetSizeOfExtendedDataRecordSelection,
      DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    if (Dem_RecordSelection[ClientId].Status == DEM_DATA_RECORD_SELECTOR_ED)
    {
      Dem_SizeEDType SizeOfEDR = 0U;
#if ((DEM_NUM_EDSEGS > 0U) || (DEM_OBD_Support == STD_OFF))
      uint8 SelectedRecNum = Dem_RecordSelection[ClientId].SelectedRecNum;
#endif

      Result = DEM_NO_SUCH_ELEMENT;

      /* !LINKSTO Dem.ISO-14229-1_11.3.1.7_1,1 */
#if (DEM_OBD_Support == STD_OFF)
      if (SelectedRecNum != DEM_ALL_OBD_ED_RECORD_NUM)
#endif
      {
#if (DEM_NUM_EDSEGS > 0U)
        P2CONST(Dem_EDClassType, AUTOMATIC, DEM_CONST) EDClass;
        Dem_DTCSelectionType DTCSelection;
        Dem_EventIdType EventId;
        Dem_EDSegIdxType SegIndex = 0xFFU;

        Dem_GetDTCSelection(ClientId, &DTCSelection);
        EventId = DTCSelection.DTCInfo.EventId;

        /* get extended data class layout */
        EDClass = &Dem_EDClass[Dem_GbiEDClassIdx(EventId)];

        /* check for valid record number only if a single record selected */
        if (SelectedRecNum <= DEM_MAX_ED_RECORD_NUM)
        {
          SegIndex = Dem_FindEDNumIndex(EDClass, SelectedRecNum);
        }

        /* check if record number is valid for the selected DTC */
        /* !LINKSTO dsn.Dem.CheckRecordNumber.SearchForEntry,2 */
        /* !LINKSTO Dem.ISO-14229-1_11.3.1.7_2,1 */
        if ( (SegIndex < EDClass->NumEDSegs)         ||
             (SelectedRecNum == DEM_ALL_OBD_ED_RECORD_NUM) ||
             (SelectedRecNum == DEM_ALL_RECORD_NUM) )
        {
          boolean EventMemEntryAvail;

          /*
           * ENTER critical section to protect event memory entry
           */
          DEM_ENTER_EXCLUSIVE_AREA();

          /* search for DTC in event memory */
          EventMemEntryAvail =
            Dem_SearchForEntry(EventId, NULL_PTR, NULL_PTR);

          /* search all EDRs to find records matching the requested number */
          for (SegIndex = 0U; (SegIndex < EDClass->NumEDSegs); ++SegIndex)
          {
            CONSTP2CONST(Dem_EDSegmentType, AUTOMATIC, DEM_CONST) EDSegment =
              &Dem_EDSegment[EDClass->EDSegIdx[SegIndex]];

            /* check if current EDR matches the requested number or group */
            /* !LINKSTO Dem.ASR431.SWS_Dem_00076_Implicit1,1 */
            /* !LINKSTO Dem.ASR431.SWS_Dem_00076_Implicit2,1 */
            if ( (SelectedRecNum == DEM_ALL_RECORD_NUM) ||
                 ( (SelectedRecNum == DEM_ALL_OBD_ED_RECORD_NUM) &&
                   (EDSegment->RecNum >= DEM_MIN_OBD_ED_RECORD_NUM) ) ||
                 (SelectedRecNum == EDSegment->RecNum) )
            {
              /* configuration does not allow record numbers >= 0xF0U */
              DEM_PRECONDITION_ASSERT(
                EDSegment->RecNum <= DEM_MAX_ED_RECORD_NUM,
                DEM_INTERNAL_API_ID);

              /* only add the size of this record if there is data */
              /* !LINKSTO Dem.Dcm.SizeOfExtendedDataRecord.DataAlwaysAvailable,1 */
              if ( (TRUE == EventMemEntryAvail) ||
                   (TRUE == EDSegment->AlwaysAvailable) )
              {
                /* add record number size */
                SizeOfEDR += 1U;

#if (DEM_USE_MAPPED_INT_DATA_ELEMENTS == STD_ON)
                SizeOfEDR += Dem_SizeOfMapIntValSegment(EDSegment);
#endif
                SizeOfEDR += Dem_SizeOfEDSegment(EDSegment);
              }
            }
          }

          /*
           * LEAVE critical section
           */
          DEM_EXIT_EXCLUSIVE_AREA();

          Result = E_OK;
        }
#endif /* DEM_NUM_EDSEGS > 0U */
        *SizeOfExtendedDataRecord = SizeOfEDR;
      }
    }
    else
    {
      /* the API is not called in the correct sequence */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO Dem.ASR431.SWS_Dem_01274,1 */
      DEM_REPORT_ERROR(DEM_SID_GetSizeOfExtendedDataRecordSelection,
        DEM_E_WRONG_CONDITION);
#endif
      /*  Result stays E_NOT_OK */
    }
  }

  DBG_DEM_GETSIZEOFEXTENDEDDATARECORDSELECTION_EXIT(
    Result, ClientId, SizeOfExtendedDataRecord);
  return Result;
}

/*==================[internal function definitions]=========================*/

#if (DEM_NUM_FFSEGS > 0U)
STATIC FUNC(boolean, DEM_CODE) Dem_FindNextFFRecForReporting(
  Dem_EventIdType                                 EventId,
  uint8                                           RecordNumberToSearch,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)          FoundRecordNumber,
  P2VAR(Dem_FFKindType, AUTOMATIC, DEM_APPL_DATA) FFKind)
{
#if (DEM_FREEZE_FRAME_REC_NUMERATION_TYPE == DEM_FF_RECNUM_CONFIGURED)
  P2CONST(Dem_FFIdxType, AUTOMATIC, DEM_CONST) FFRecNumClass;
  Dem_FFRecNumClassIdxType FFRecNumClassIdx;
  Dem_FFIdxType NumFF;
  uint8 FFRecNumIdx;
#endif
  boolean Result = FALSE;

  DBG_DEM_FINDNEXTFFRECFORREPORTING_ENTRY(
    EventId, RecordNumberToSearch, FoundRecordNumber, FFKind);

  *FoundRecordNumber = DEM_MAX_FF_RECORD_NUM + 1U;

#if (DEM_FREEZE_FRAME_REC_NUMERATION_TYPE == DEM_FF_RECNUM_CONFIGURED)
  FFRecNumClassIdx = Dem_GbiFFRecNumClassIdx(EventId);
  NumFF = Dem_FFRecNumerationClass[FFRecNumClassIdx].NumFFRecs;
  FFRecNumClass =
    Dem_FFRecNumerationClass[FFRecNumClassIdx].Dem_FFRecNumClassIdx;

  for (FFRecNumIdx = 0U; FFRecNumIdx < NumFF; FFRecNumIdx++)
  {
    uint8 RecNum = FFRecNumClass[FFRecNumIdx];
    /* check whether the requested record number is valid for the record
     * number class */
    if ((RecNum >= RecordNumberToSearch) && (RecNum < *FoundRecordNumber))
    {
      *FoundRecordNumber = RecNum;
      *FFKind = DEM_REGULAR_FF;
      Result = TRUE;
    }
  }
#else /* DEM_FREEZE_FRAME_REC_NUMERATION_TYPE == DEM_FF_RECNUM_CALCULATED */
  /* exclude DEM_OBD_FF_RECORD_NUM for this case, even though this function is
   * not expected to be called with it */
  if ( (RecordNumberToSearch <= Dem_GbiMaxNumFFRecs(EventId)) &&
       (RecordNumberToSearch != DEM_OBD_FF_RECORD_NUM)
     )
  {
    *FoundRecordNumber = RecordNumberToSearch;
    *FFKind = DEM_REGULAR_FF;
    Result = TRUE;
  }
#endif /* DEM_FREEZE_FRAME_REC_NUMERATION_TYPE */

#if (DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U)
  {
    uint8 DevFFRecNum = Dem_EventDescDevFF[EventId][DEM_DEVFF_A].DevFFNum;
    if ((DevFFRecNum >= RecordNumberToSearch) &&
        (DevFFRecNum < *FoundRecordNumber))
    {
      *FoundRecordNumber = DevFFRecNum;
      *FFKind = DEM_DEVFF_A;
      Result = TRUE;
    }
  }
#endif /* (DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U) */

#if (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U)
  {
    uint8 DevFFRecNum = Dem_EventDescDevFF[EventId][DEM_DEVFF_B].DevFFNum;
    if ((DevFFRecNum >= RecordNumberToSearch) &&
        (DevFFRecNum < *FoundRecordNumber))
    {
      *FoundRecordNumber = DevFFRecNum;
      *FFKind = DEM_DEVFF_B;
      Result = TRUE;
    }
  }
#endif /* (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U) */

  DBG_DEM_FINDNEXTFFRECFORREPORTING_EXIT(
    Result, EventId, RecordNumberToSearch, FoundRecordNumber, FFKind);

  return Result;
}

STATIC FUNC(Std_ReturnType, DEM_CODE)
  Dem_CollectFFInfo(
    Dem_EventIdType                         EventId,
    Dem_FFKindType                          FFKind,
    uint8                                   RecordNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  DestBuffer)
{
  Dem_EventMemoryEntryPtrType EvMemEntry;
  Std_ReturnType Result;
  boolean EntryExists;

  DBG_DEM_COLLECTFFINFO_ENTRY(EventId, FFKind, RecordNumber,
    BufSize, DestBuffer);

  /*
   * ENTER critical section to protect event memory entry
   */
  DEM_ENTER_EXCLUSIVE_AREA();

  /* search for entry in event memory */
  EntryExists = Dem_SearchForEntry(EventId, &EvMemEntry, NULL_PTR);

  if (EntryExists == TRUE)
  {
    Dem_FFIdxType       FFIndex;
    const Dem_FFIdxType MaxNumFF = Dem_GbiMaxNumFFRecs(EventId);
    boolean isDevFFIdxValid = FALSE;

#if (DEM_DEV_FREEZE_FRAME_USED == STD_ON)
    if ((FFKind == DEM_DEVFF_A) ||
        (FFKind == DEM_DEVFF_B))
    {
      if (EvMemEntry->DevFFEntryIdx[FFKind] < Dem_SizeDevFFMem[FFKind])
      {
        isDevFFIdxValid = TRUE;
      }
    }
#endif /* (DEM_DEV_FREEZE_FRAME_USED == STD_ON) */

    FFIndex = Dem_CheckFFRecNumStored(EvMemEntry, RecordNumber);

    if ( ( (FFIndex < DEM_NUMFF(EvMemEntry, MaxNumFF)) &&
           (FFKind == DEM_REGULAR_FF)) ||
         (isDevFFIdxValid == TRUE)
       )
    {
      P2CONST(Dem_FFClassType, AUTOMATIC, DEM_CONST) FFIndClass = NULL_PTR;
      Dem_EntryDataPtrConstType FFEntryData = NULL_PTR;
      Dem_SizeEntryDataType     NeededFFSize;
      Dem_NumFFSegsType         NumFFSegs;

#if (DEM_DEV_FREEZE_FRAME_USED == STD_ON)
      if ((FFKind == DEM_DEVFF_A) ||
          (FFKind == DEM_DEVFF_B))
      {
        FFEntryData = &Dem_DevFFData[FFKind][
          EvMemEntry->DevFFEntryIdx[FFKind] * DEM_DEV_FF_MAX_CLASS_SIZE(FFKind)];
        FFIndClass =
          &Dem_FFClass[Dem_EventDescDevFF[EventId][FFKind].DevFFClassIdx];
      }
      else
#endif /* (DEM_DEV_FREEZE_FRAME_USED == STD_ON) */
      {
        FFIndClass = &Dem_FFClass[Dem_GbiFFClassIdx(EventId)];
        FFEntryData = Dem_FFEntryData(EvMemEntry, FFIndClass, FFIndex);
      }

      NumFFSegs = Dem_GetNumFFSegs(FFKind, FFIndClass);
      NeededFFSize = ((uint16)Dem_SizeOfFF(FFKind, FFIndClass) +
        (2U * (uint16)NumFFSegs) + 2U);

      if (NeededFFSize <= *BufSize)
      {
        /* write data for one record: */
        /* byte 0: record number */
        DestBuffer[0] = RecordNumber;
        /*  byte 1: number of segments */
        DestBuffer[1] = NumFFSegs;

        Dem_CopyFFInfo(FFKind, FFIndClass, FFEntryData, &DestBuffer[2]);
        *BufSize = NeededFFSize;
        Result = E_OK;
      }
      else /* destination buffer is too small */
      {
        Result = DEM_BUFFER_TOO_SMALL;
      }
    }
    else
    {
      /* no freeze frame stored, return empty buffer */
      *BufSize = 0U;
      Result = E_OK;
    }
  }
  else
  {
    /* no freeze frame A stored, return empty buffer */
    /* !LINKSTO Dem.ASR431.SWS_Dem_00630,1 */
    /* return E_OK and BufSize 0 */
    *BufSize = 0U;
    Result = E_OK;
  }

  /*
   * EXIT critical section to protect event memory entry
   */
  DEM_EXIT_EXCLUSIVE_AREA();

  DBG_DEM_COLLECTFFINFO_EXIT(Result, EventId, FFKind, RecordNumber,
    BufSize, DestBuffer);

  return Result;
}

STATIC FUNC(uint16, DEM_CODE) Dem_SizeOfFFInfo(
  Dem_EventIdType                         EventId,
  Dem_FFKindType                          FFKind,
  uint8                                   RecordNumber)
{
  uint16 SizeOfFFKind = 0U;
  boolean EntryExists;
  /* search for DTC in event memory */
  Dem_EventMemoryEntryPtrType EvMemEntry;

  DBG_DEM_SIZEOFFFINFO_ENTRY(EventId, FFKind, RecordNumber);

  /*
   * ENTER critical section to protect event memory entry
   */
  DEM_ENTER_EXCLUSIVE_AREA();

  EntryExists =
    Dem_SearchForEntry(EventId, &EvMemEntry, NULL_PTR);

  if (EntryExists == TRUE)
  {
    Dem_FFIdxType       FFIndex;
    const Dem_FFIdxType MaxNumFF = Dem_GbiMaxNumFFRecs(EventId);
    /* variable used for handling both regular and development freeze frames */
    boolean FFIdxValid = FALSE;
    Dem_FFIdxType NumFF = DEM_NUMFF(EvMemEntry, MaxNumFF);

    if (FFKind == DEM_REGULAR_FF)
    {
      FFIndex = Dem_CheckFFRecNumStored(EvMemEntry, RecordNumber);

      /* check whether the overall size is requested or whether
       * the requested record number is stored */
      if ((RecordNumber == 0xFFU) || (FFIndex < NumFF))
      {
        FFIdxValid = TRUE;
      }
    }
    else
    {
      /* reading the size of Freeze Frame A and B Class record number */
#if (DEM_DEV_FREEZE_FRAME_USED == STD_ON)
      if (EvMemEntry->DevFFEntryIdx[FFKind] < Dem_SizeDevFFMem[FFKind])
      {
        FFIdxValid = TRUE;
      }
#endif
    }

    if (FFIdxValid == TRUE)
    {
      P2CONST(Dem_FFClassType, AUTOMATIC, DEM_CONST) FFIndClass = NULL_PTR;

#if (DEM_DEV_FREEZE_FRAME_USED == STD_ON)
      if ((FFKind == DEM_DEVFF_A) ||
          (FFKind == DEM_DEVFF_B))
      {
        FFIndClass =
          &Dem_FFClass[Dem_EventDescDevFF[EventId][FFKind].DevFFClassIdx];
      }
      else
#endif
      {
        FFIndClass = &Dem_FFClass[Dem_GbiFFClassIdx(EventId)];
      }

      SizeOfFFKind = 2U + Dem_SizeOfFF(FFKind, FFIndClass) +
                            (2U * (uint16)Dem_GetNumFFSegs(FFKind, FFIndClass));

      if ( (RecordNumber == 0xFFU) && (FFKind == DEM_REGULAR_FF) )
      {
        SizeOfFFKind *= (uint16)NumFF;
      }
    }
  }

  /*
   * EXIT critical section to protect event memory entry
   */
  DEM_EXIT_EXCLUSIVE_AREA();

  DBG_DEM_SIZEOFFFINFO_EXIT(Result, SizeOfFFKind, FFKind, RecordNumber);
  return SizeOfFFKind;
}

STATIC FUNC(void, DEM_CODE) Dem_CopyFFInfo(
  Dem_FFKindType                                 FFKind,
  P2CONST(Dem_FFClassType, AUTOMATIC, DEM_CONST) IndividPartFFClass,
  Dem_EntryDataPtrConstType                      FFEntryData,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)         DestBuffer)
{
  Dem_NumFFSegsType SegIdx;
  Dem_SizeEntryDataType Offset = 0U;
  Dem_SizeEntryDataType StartByte = 0U;

  uint8_least FFPartIdx;

  P2CONST(Dem_FFClassType, AUTOMATIC, DEM_CONST) FFPartClass[DEM_NUM_FFPARTS];
#if (DEM_COMMON_FREEZEFRAMES_USED == STD_ON)
  FFPartClass[0] = &Dem_FFClass[Dem_CommonPartFFClassIdx[FFKind]];
#endif
  FFPartClass[DEM_NUM_FFPARTS - 1U] = IndividPartFFClass;

  DBG_DEM_COPYFFINFO_ENTRY(FFKind, IndividPartFFClass, FFEntryData, DestBuffer);

#if (DEM_COMMON_FREEZEFRAMES_USED == STD_OFF)
  TS_PARAM_UNUSED(FFKind);
#endif

  /* Deviation TASKING-2 */
  for (FFPartIdx = 0U; FFPartIdx < DEM_NUM_FFPARTS; FFPartIdx++)
  {
    for (SegIdx = 0U; SegIdx < FFPartClass[FFPartIdx]->NumFFSegs; ++SegIdx)
    {
      /* copy one freeze frame segment data Ids and values into
         the destination buffer */

      const Dem_FFSegIdxType FFSegIdx = FFPartClass[FFPartIdx]->FFSegIdx[SegIdx];

      if (FFSegIdx != DEM_FFSEG_IDX_UNUSED)
      {
        const Dem_SizeFFType FFSegSize =
          Dem_SizeOfFFSegment(&Dem_FFSegment[FFSegIdx]);

        DestBuffer[Offset] =
          DEM_HIBYTE(Dem_FFSegment[FFSegIdx].DataId);
        DestBuffer[Offset + 1U] =
          DEM_LOBYTE(Dem_FFSegment[FFSegIdx].DataId);

        /* advance offset by 2 bytes */
        Offset += 2U;

        /* write content into response */
        TS_MemCpy(&DestBuffer[Offset], &FFEntryData[StartByte], FFSegSize);

        /* calculate startbyte for next FF segment */
        StartByte += (Dem_SizeEntryDataType)FFSegSize;

        /* advance offset by number of bytes in segment */
        Offset += (Dem_SizeEntryDataType)FFSegSize;
      }
    }
  }

  DBG_DEM_COPYFFINFO_EXIT(FFKind, IndividPartFFClass, FFEntryData, DestBuffer);
}

#if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)
STATIC FUNC(Std_ReturnType, DEM_CODE)
  Dem_CollectOBDFreezeFrameInfo(
    Dem_EventIdType                         EventId,
    uint8                                   RecordNumber,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  DestBuffer)
{
#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
  Dem_EventMemoryEntryPtrType EvMemEntry;
#endif
  Dem_EventIdType StoredOBDFFEventId;
  Std_ReturnType Result = E_OK;
  boolean EntryExists;
  Dem_SizeEvMemEntryType SelectedEntryIdx = 0U;

  DBG_DEM_COLLECTOBDFREEZEFRAMEINFO_ENTRY(
    EventId, RecordNumber, BufSize, DestBuffer);

  /*
   * ENTER critical section to protect event memory entry
   */
  DEM_ENTER_EXCLUSIVE_AREA();

#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)
  /* select entry for multiple OBD freeze frame */
  EntryExists = Dem_SelectEntryOfOBDFreezeFrameEvent(&SelectedEntryIdx, FALSE);

  StoredOBDFFEventId =
    Dem_EventMem[(DEM_DTC_ORIGIN_PRIMARY_MEMORY - 1U)][SelectedEntryIdx].EventId;
#elif (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
  /* search for DTC in event memory */
  /* for single OBD freeze frame there is no relation to event memory entry
   * consequently an explicit search for the event entry must be done */
  EntryExists = Dem_SearchForEntry(EventId, &EvMemEntry, NULL_PTR);

  StoredOBDFFEventId = DEM_OBDFF_STORED_EVENT_ID;
#endif /* (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE) */

  /* Deviation MISRAC2012-1 */
  if ( (EntryExists == TRUE) &&
       (DEM_GET_MASTER_EVID(EventId) == DEM_GET_MASTER_EVID(StoredOBDFFEventId)) )
  {
    /* check if the possible biggest OBD FF fits into the buffer,
     * do not care about disabled PIDs */
    if (*BufSize >= DEM_SIZE_OBDFF)
    {
      uint16 OdbFFSize = 0U;

      /* write data for one record: */
      /*  byte 0: record number */
      DestBuffer[0] = RecordNumber;
      /*  byte 1: number of segments is return by copy function */
      DestBuffer[1] = Dem_CopyOBDFreezeFrameInfo(&DestBuffer[2],
                                                 &OdbFFSize,
                                                 SelectedEntryIdx);

      /* add size for record number and segments */
      *BufSize = 2U + OdbFFSize;
    }
    else /* destination buffer is too small */
    {
      Result = DEM_BUFFER_TOO_SMALL;
    }
  }
  else
  {
    /* no OBD freeze frame stored, return empty buffer */
    *BufSize = 0U;
  }

  /*
   * EXIT critical section to protect event memory entry
   */
  DEM_EXIT_EXCLUSIVE_AREA();

  DBG_DEM_COLLECTOBDFREEZEFRAMEINFO_EXIT(Result, EventId, RecordNumber,
    BufSize, DestBuffer);

  return Result;
}

STATIC FUNC(uint16, DEM_CODE)
  Dem_SizeOfOBDFreezeFrameInfo(
    Dem_EventIdType                         EventId)
{
  uint16 ObdFFSize = 0U;
#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)
  Dem_SizeEvMemEntryType SelectedEntryIdx = 0U;
#endif
  boolean EntryExists = FALSE;
  Dem_EventIdType StoredOBDFFEventId;
#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
  Dem_EventMemoryEntryPtrType EvMemEntry;
#endif

  DBG_DEM_SIZEOFOBDFREEZEFRAMEINFO_ENTRY(EventId);

  /*
   * ENTER critical section to protect event memory entry
   */
  DEM_ENTER_EXCLUSIVE_AREA();

#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)
  EntryExists =
      Dem_SelectEntryOfOBDFreezeFrameEvent(&SelectedEntryIdx, FALSE);
  StoredOBDFFEventId =
    Dem_EventMem[0][SelectedEntryIdx].EventId;
#elif (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
  /* for single OBD freeze frame there is no relation to event memory entry
   * consequently an explicit search for the event entry must be done */
  EntryExists =
      Dem_SearchForEntry(EventId, &EvMemEntry, NULL_PTR);
  StoredOBDFFEventId = DEM_OBDFF_STORED_EVENT_ID;
#endif /* (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE) */

  /*
   * EXIT critical section to protect event memory entry
   */
  DEM_EXIT_EXCLUSIVE_AREA();

  /* OBD FF is available via $19 04 only if fault is in memory */
  /* Deviation MISRAC2012-1 */
  if ( (EntryExists == TRUE) &&
       (DEM_GET_MASTER_EVID(EventId) == DEM_GET_MASTER_EVID(StoredOBDFFEventId)) )
  {
#if (DEM_CALIBRATION_SUPPORT == STD_ON)
    uint8 ObdFFPidIterator;

    /* Deviation TASKING-2 */
    for (ObdFFPidIterator = 0U;
         ObdFFPidIterator < DEM_NUM_OF_PIDS_IN_OBDFF;
         ObdFFPidIterator++)
    {
      if (Dem_PidClass[ObdFFPidIterator].ParameterId != DEM_PID_UNUSED)
      {
        /* compute size of OBDFF */
        ObdFFSize += 2U + (uint16)Dem_PidClass[ObdFFPidIterator].Size;
      }
    }

    /* add size for record number and number of segments */
    ObdFFSize += 2U;
#else
    ObdFFSize = DEM_SIZE_OBDFF;
#endif /* (DEM_CALIBRATION_SUPPORT == STD_ON) */
  }

  DBG_DEM_SIZEOFOBDFREEZEFRAMEINFO_EXIT(ObdFFSize, EventId);
  return ObdFFSize;
}

STATIC FUNC(uint8, DEM_CODE) Dem_CopyOBDFreezeFrameInfo(
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  DestBuffer,
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize,
  Dem_SizeEvMemEntryType                  SelectedEntryIdx)
{
  uint16 Offset = 0U;
  uint8 NumberOfPids = 0U;
  uint8 ObdFFPidIterator;
  Dem_SizeEntryDataType StartByte = 0U;

#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
  CONSTP2VAR(Dem_EntryDataType, AUTOMATIC, AUTOMATIC) OBDFFStoredData =
    DEM_OBDFF_STORED_DATA;
#else
  CONSTP2VAR(Dem_EntryDataType, AUTOMATIC, AUTOMATIC) OBDFFStoredData =
    DEM_OBDFF_ENTRY_FFDATA_LINK(SelectedEntryIdx);
#endif

  DBG_DEM_COPYOBDFREEZEFRAMEINFO_ENTRY(DestBuffer, BufSize, SelectedEntryIdx);

#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
  TS_PARAM_UNUSED(SelectedEntryIdx);
#endif

  /* Deviation TASKING-2 */
  for (ObdFFPidIterator = 0U;
       ObdFFPidIterator < DEM_NUM_OF_PIDS_IN_OBDFF;
       ObdFFPidIterator++)
  {
#if (DEM_CALIBRATION_SUPPORT == STD_ON)
    if (Dem_PidClass[ObdFFPidIterator].ParameterId != DEM_PID_UNUSED)
#endif
    {
      /* map PID to DID according SWS_Dem_00686, DID --> 0xF4 + PID */
      DestBuffer[Offset] = 0xF4U;
      DestBuffer[Offset + 1U] = Dem_PidClass[ObdFFPidIterator].ParameterId;

      /* copy complete PID in DestBuffer */
      TS_MemCpy(&DestBuffer[Offset + 2U],
        &OBDFFStoredData[StartByte],
        Dem_PidClass[ObdFFPidIterator].Size);

      /* add size of copied PID */
      Offset += 2U + (uint16)Dem_PidClass[ObdFFPidIterator].Size;

      /* increment number of returned PIDs */
      NumberOfPids++;
    }

    /* calculate start position index of the PID */
    StartByte += (Dem_SizeEntryDataType)Dem_PidClass[ObdFFPidIterator].Size;
  }

  *BufSize = Offset;

  DBG_DEM_COPYOBDFREEZEFRAMEINFO_EXIT(NumberOfPids, DestBuffer, BufSize, SelectedEntryIdx);

  return NumberOfPids;
}
#endif /* (DEM_NUM_OF_PIDS_IN_OBDFF > 0U) */
#endif /* (DEM_NUM_FFSEGS > 0U) */

#if (DEM_NUM_EDSEGS > 0U)
STATIC FUNC(boolean, DEM_CODE) Dem_FindNextEDRecForReporting(
  P2CONST(Dem_EDClassType, AUTOMATIC, DEM_CONST)             EDClass,
  uint8                                                      RecordNumberToSearch,
  P2VAR(Dem_EDSegmentPtrConstType, AUTOMATIC, DEM_APPL_DATA) FoundEDSegment,
  P2VAR(Dem_EDStartByteType, AUTOMATIC, DEM_APPL_DATA)       EDStartByte)
{
  boolean Result = FALSE;
  Dem_EDSegIdxType SegIndex;
  Dem_EDSegIdxType FoundSegIndex = 0U;
  uint8 NextAvailableRecNum = DEM_MAX_ED_RECORD_NUM + 1U;

  DBG_DEM_FINDNEXTEDRECFORREPORTING_ENTRY(EDClass,
    RecordNumberToSearch, FoundEDSegment, EDStartByte);

  for (SegIndex = 0U; SegIndex < EDClass->NumEDSegs; ++SegIndex)
  {
    uint8 EDSegRecNum = Dem_EDSegment[EDClass->EDSegIdx[SegIndex]].RecNum;

    if ( (EDSegRecNum >= RecordNumberToSearch) &&
         (EDSegRecNum < NextAvailableRecNum) )
    {
      NextAvailableRecNum = EDSegRecNum;
      FoundSegIndex = SegIndex;
    }
  }

  if (NextAvailableRecNum <= DEM_MAX_ED_RECORD_NUM)
  {
    *EDStartByte = EDClass->StartByte[FoundSegIndex];
    *FoundEDSegment = &Dem_EDSegment[EDClass->EDSegIdx[FoundSegIndex]];
    Result = TRUE;
  }

  DBG_DEM_FINDNEXTEDRECFORREPORTING_EXIT(Result,
    EDClass, RecordNumberToSearch, FoundEDSegment, EDStartByte);
  return Result;
}
#endif /* (DEM_NUM_EDSEGS > 0U) */

#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>

/*==================[end of file]===========================================*/
