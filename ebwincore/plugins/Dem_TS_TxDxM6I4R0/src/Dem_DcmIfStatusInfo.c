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
 * the Dcm for the access of DTC status information. */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason:
 * The "ClientId" value depends on the input from the tests.
 * In other configurations, the code coverage reaches sections where the result is
 * different from the value Dem_StoredDTCFilterLockByClientId.
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

#include <TSAutosar.h>                        /* EB specific standard types */
#include <Std_Types.h>                            /* AUTOSAR standard types */

#include <Dem_Int.h>             /* module public and internal declarations */
#include <Dem_Trace.h>                        /* Dbg related macros for Dem */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

/** \brief Function to apply the DTC filter with the given event
 **
 **
 ** \param[in] ClientId
 ** \param[in] EventId  0 < EventId <= DEM_MAX_EVENTID
 ** \param[out] DTCStatus
 ** \param[out] FDC
 ** \param[out] DTC
 **
 ** \return Boolean value, if the filter matches the event configuration
 ** \retval TRUE  the filters match
 ** \retval FALSE the filters don't match
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(boolean, DEM_CODE) Dem_ApplyDTCFilter(
  uint8                                                          ClientId,
  Dem_EventIdType                                                EventId,
  P2VAR(Dem_DTCStatusMaskType, AUTOMATIC, DEM_APPL_DATA)         DTCStatus,
  P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA)                         FDC,
  P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC);

#if (DEM_ORDERED_DTC_REPORTING_USED == STD_ON)
/** \brief Function to memorize the current chronological order of stored DTCs
 **
 ** \param[in] ClientId client id for which the DTC order will be applied
 ** \param[in] DTCOrigin event memory source
 **
 ** \Reentrancy{Not reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(void, DEM_CODE) Dem_ApplyDTCOrder(uint8 ClientId,
                                              Dem_DTCOriginType DTCOrigin);
#endif

#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define DEM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dem_MemMap.h>

/** \brief Status of DTC-filter */
STATIC VAR(Dem_DTCFilterType, DEM_VAR_CLEARED) Dem_DTCFilter[DEM_NUMBER_OF_CLIENTS];

#define DEM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dem_MemMap.h>

#if (DEM_ORDERED_DTC_REPORTING_USED == STD_ON)

#define DEM_START_SEC_VAR_INIT_16
#include <Dem_MemMap.h>
/* !LINKSTO dsn.Dem.ReportingOrderDTC.DtcOrderImage,1 */
/* !LINKSTO dsn.Dem.ReportingOrderDTC.CurrentDTCReportingOrder,1 */
/* !LINKSTO Dem410_Implicit1,1, Dem410_Implicit2,1 */
/** \brief Image of chronologically ordered DTCs at call to set up a DTC filter */
STATIC VAR(Dem_EventIdType, DEM_VAR_CLEARED) Dem_OrderedEventId
  [DEM_NUMBER_OF_CLIENTS][DEM_MAX_NUM_ORDERED_EVENT_IDS] = { {DEM_EVENT_ID_INVALID} };

#define DEM_STOP_SEC_VAR_INIT_16
#include <Dem_MemMap.h>
#endif /* DEM_ORDERED_DTC_REPORTING_USED == STD_ON */

/*==================[internal data]=========================================*/

#define DEM_START_SEC_VAR_CLEARED_8
#include <Dem_MemMap.h>

/** \brief Status of freeze frame record filter
 **
 ** Defines the structure of the data element that holds the FF record
 ** filter attributes.
 */
STATIC VAR(Dem_RecordFilterType, DEM_VAR) Dem_RecordFilter[DEM_NUMBER_OF_CLIENTS];

/** \brief The client ID that locked the stored DTC filter */
STATIC VAR(uint8, DEM_VAR_CLEARED) Dem_StoredDTCFilterLockByClientId;

#if (DEM_NUMBER_OF_CLIENTS > 1U)
/** \brief Flag that indicates the read operation was finished by
 ** the previous call of Dem_DcmGetNextFilteredStoredDTC().
 */
STATIC VAR(boolean, DEM_VAR_CLEARED) Dem_ReadStoredDTCOpDone;
#endif

/** \brief Attributes and status of stored DTC filter */
STATIC VAR(Dem_StoredDTCFilterType, DEM_VAR_CLEARED) Dem_StoredDTCFilter;

#define DEM_STOP_SEC_VAR_CLEARED_8
#include <Dem_MemMap.h>

/*==================[external function definitions]=========================*/

#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

FUNC(void, DEM_CODE) Dem_InitDTCFilter(void)
{
  /* it should be noted that uint8 is used here because the max value
     of DEM_NUMBER_OF_CLIENTS is 255, if it's ever changed to 256 uint16 should
     be used */
  uint8_least Idx;
  /* Deviation TASKING-2 */
  for (Idx = 0U; Idx < DEM_NUMBER_OF_CLIENTS; Idx++)
  {
    Dem_DTCFilter[Idx].IsSet = FALSE;
  }
}

FUNC(void, DEM_CODE) Dem_InitRecordFilter(void)
{
  /* it should be noted that uint8 is used here because the max value
     of DEM_NUMBER_OF_CLIENTS is 255, if it's ever changed to 256 uint16 should
     be used */
  uint8_least Idx;
  /* Deviation TASKING-2 */
  for (Idx = 0U; Idx < DEM_NUMBER_OF_CLIENTS; Idx++)
  {
    Dem_RecordFilter[Idx].RecordFilterIsSet = FALSE;
  }
}

FUNC(void, DEM_CODE) Dem_InitDTCMemoryFilter(void)
{
  DBG_DEM_INITDTCMEMORYFILTER_ENTRY();

  Dem_StoredDTCFilterLockByClientId = DEM_CLIENT_ID_INVALID;
#if (DEM_NUMBER_OF_CLIENTS > 1U)
  Dem_ReadStoredDTCOpDone = FALSE;
#endif

  DBG_DEM_INITDTCMEMORYFILTER_EXIT();
}

/*----------------[Dem_SetDTCFilter]----------------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_SetDTCFilter(
  uint8                      ClientId,
  uint8                      DTCStatusMask,
  Dem_DTCFormatType          DTCFormat,
  Dem_DTCOriginType          DTCOrigin,
  boolean                    FilterWithSeverity,
  Dem_DTCSeverityType        DTCSeverityMask,
  boolean                    FilterForFaultDetectionCounter)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_SETDTCFILTER_ENTRY(
    ClientId, DTCStatusMask, DTCFormat, DTCOrigin,
    FilterWithSeverity, DTCSeverityMask, FilterForFaultDetectionCounter);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  /* check of Dem_InitializationState would be not necessary, but if this API
   * is called before full initialization, the system design is dubious
   * (and Dem_GetNextFilteredDTC[AndFDC]() is then not usable anyway) */
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_SetDTCFilter, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_SetDTCFilter, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId >= DEM_NUMBER_OF_CLIENTS)
  {
    DEM_REPORT_ERROR(DEM_SID_SetDTCFilter, DEM_E_WRONG_CONFIGURATION);
  }
  /* Dcm is responsible, that no status masks with unavailable bits are
   * requested (but this would not influence correctness of the filter) */
  /* no check with DEM_IS_UNMASKED_BIT_SET(DTCStatusMask, AVAILABILITY_MASK)
   * done, because all bits are supported */
  else if ( (DTCFormat != DEM_DTC_FORMAT_OBD) &&
            (DTCFormat != DEM_DTC_FORMAT_UDS)
          )
  {
    /* !LINKSTO Dem.ASR431.SetDTCFilter.InvalidDTCFormat,1 */
    DEM_REPORT_ERROR(DEM_SID_SetDTCFilter, DEM_E_PARAM_DATA);
  }
  else if ( (DTCOrigin > (Dem_DTCOriginType)0x01FFU) ||
            ( (DTCOrigin < (Dem_DTCOriginType)0x0100U) &&
              (DTCOrigin != DEM_DTC_ORIGIN_PRIMARY_MEMORY) &&
              (DTCOrigin != DEM_DTC_ORIGIN_MIRROR_MEMORY) &&
              (DTCOrigin != DEM_DTC_ORIGIN_PERMANENT_MEMORY) &&
              (DTCOrigin != DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY)
            )
          )
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01264,1 */
    DEM_REPORT_ERROR(DEM_SID_SetDTCFilter, DEM_E_PARAM_DATA);
  }
  /* this API is called with FilterWithSeverity = TRUE only when DTCFormat is
   * UDS (from UDS services 0x19 - sub-functions 0x07, 0x08 and 0x42). Thus,
   * no check is implemented regarding Dem.ASR431.DemDTCSeverity_Implicit1. */
  else if ( (FilterWithSeverity != TRUE) &&
            (FilterWithSeverity != FALSE)
          )
  {
    DEM_REPORT_ERROR(DEM_SID_SetDTCFilter, DEM_E_PARAM_DATA);
  }
  else if ( (DTCSeverityMask != DEM_SEVERITY_NO_SEVERITY) &&
            (DEM_IS_UNMASKED_BIT_SET(DTCSeverityMask,
               DEM_SEVERITY_MAINTENANCE_ONLY |
               DEM_SEVERITY_CHECK_AT_NEXT_HALT |
               DEM_SEVERITY_CHECK_IMMEDIATELY) )
          )
  {
    DEM_REPORT_ERROR(DEM_SID_SetDTCFilter, DEM_E_PARAM_DATA);
  }
  else if ( (FilterForFaultDetectionCounter != TRUE) &&
            (FilterForFaultDetectionCounter != FALSE)
          )
  {
    DEM_REPORT_ERROR(DEM_SID_SetDTCFilter, DEM_E_PARAM_DATA);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* !LINKSTO Dem.ASR431.SetDTCFilter.Attributes,1 */
    CONSTP2VAR(Dem_DTCFilterType, AUTOMATIC, DEM_VAR_CLEARED) ClientDTCFilter =
      &Dem_DTCFilter[ClientId];

    Dem_DTCOriginType MappedDTCOrigin = Dem_GetMappedDTCOrigin(
        DTCOrigin, DEM_DTC_ORIGIN_MAPPING_DCM_IF);

    /* reset event to read next */
    /* !LINKSTO dsn.Dem.ReportingOrderDTC.SetDtcFilter.InitNextItem,1 */
    ClientDTCFilter->NextItem = 1U;
    /* !LINKSTO Dem.ASR431.SetDTCFilter.Validity,1 */
    /* !LINKSTO Dem.ASR431.SetDTCFilter.UnsuccessfulCall,1 */
    ClientDTCFilter->IsSet = FALSE;

    /* Check if requested DTC_ORIGIN is mapped */
    if(MappedDTCOrigin != DEM_DTC_ORIGIN_INVALID)
    {

#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER == 0U)
      if (Dem_SizeEventMem[MappedDTCOrigin -1] == (Dem_SizeEvMemEntryType) 0U)
#else
      if ( (Dem_SizeEventMem[MappedDTCOrigin -1] == (Dem_SizeEvMemEntryType) 0U) &&
           (MappedDTCOrigin != DEM_DTC_ORIGIN_PERMANENT_MEMORY) )
#endif
      {
        /* exit with E_NOT_OK */
      }
      else
#if (DEM_OBD_Support == STD_OFF)
      if (DTCFormat == DEM_DTC_FORMAT_OBD)
      {
        /* exit with E_NOT_OK */
      }
      else
#endif
      {
        /* !LINKSTO Dem.SetDTCFilter.DTCStatusAvailabilityMask,1 */
        Dem_DTCStatusMaskType AvailableDTCStatusMask =
          (DEM_DTC_STATUS_AVAILABILITY_MASK & DTCStatusMask);

#if (DEM_ORDERED_DTC_REPORTING_USED == STD_ON)
        Dem_DTCStatusMaskType OccOrderingMask = 0U;

#if (DEM_RESET_PENDING_BIT_ON_OVERFLOW_SUPPORT == STD_ON)
        OccOrderingMask |= DEM_UDS_STATUS_PDTC;
#endif
#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW_SUPPORT == STD_ON)
        OccOrderingMask |= DEM_UDS_STATUS_CDTC;
#endif
        /* !LINKSTO dsn.Dem.ReportingOrderDTC.Condition, 1 */
        /* !LINKSTO Dem410_Implicit2, 1 */
        /* save info if:
         * 1. chronological ordered output is required in flag
         * 2. any bit of ordering mask is set, but no other bit is allowed to be set (equivalence check)
         * 3. valid DTCOrigin with size > 0 to support OccOrdering?
         */
        if (  ( (AvailableDTCStatusMask & OccOrderingMask) != 0x00U) &&
              ( (AvailableDTCStatusMask & (uint8)(~OccOrderingMask) ) == 0x00U) &&
              ( Dem_SizeEventMem[MappedDTCOrigin - 1U] > 0U )
           )
        {
          ClientDTCFilter->OccOrdering = TRUE;
        }
        else
        {
          ClientDTCFilter->OccOrdering = FALSE;
        }
#endif /* (DEM_ORDERED_DTC_REPORTING_USED == STD_ON) */
        /* !LINKSTO Dem.ASR431.SWS_Dem_01263,1 */
        /* save filter information in internal data structure */
        ClientDTCFilter->DTCStatusMask      = AvailableDTCStatusMask;
        /* !LINKSTO dsn.Dem.ASR431.DTCFilterType.DTCKind,1 */
        /* set the DTCKind according to the DTCOrigin request */
        ClientDTCFilter->DTCKind = (DTCOrigin == DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY) ?
          DEM_DTC_KIND_EMISSION_REL_DTCS : DEM_DTC_KIND_ALL_DTCS;
        ClientDTCFilter->DTCFormat          = DTCFormat;
        ClientDTCFilter->DTCOrigin          = MappedDTCOrigin;
        ClientDTCFilter->FilterWithSeverity = FilterWithSeverity;
        ClientDTCFilter->DTCSeverityMask    = DTCSeverityMask;
        ClientDTCFilter->FilterForFDC       = FilterForFaultDetectionCounter;

#if (DEM_ORDERED_DTC_REPORTING_USED == STD_ON)
        /* check if chronological order of DTCs is required */
        if (ClientDTCFilter->OccOrdering == TRUE)
        {
          /* !LINKSTO dsn.Dem.ReportingOrderDTC.DtcOrderImage,1 */
          /* take snapshot of current DTC order */

          Dem_ApplyDTCOrder(ClientId, MappedDTCOrigin);
        }
#endif

        /* reset event to read next */
        /* !LINKSTO dsn.Dem.ReportingOrderDTC.SetDtcFilter.InitNextItem,1 */
        ClientDTCFilter->NextItem = 1U;
        ClientDTCFilter->IsSet = TRUE;

        Result = E_OK;
      }
    }
  }
  DBG_DEM_SETDTCFILTER_EXIT(
    Result, ClientId, DTCStatusMask, DTCFormat, DTCOrigin,
    FilterWithSeverity, DTCSeverityMask, FilterForFaultDetectionCounter);
  return Result;
}

/*----------------[Dem_SetFreezeFrameRecordFilter]--------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_SetFreezeFrameRecordFilter(
  uint8                                   ClientId,
  Dem_DTCFormatType                       DTCFormat,
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredRecords)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_SETFREEZEFRAMERECORDFILTER_ENTRY(
    ClientId, DTCFormat, NumberOfFilteredRecords);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  /* check of Dem_InitializationState is necessary, as filter is reset in
   * Dem_Init() (and Dem_GetNextFilteredRecord() is then not usable anyway)
   * and also the event memory is accessed */
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_SetFreezeFrameRecordFilter, DEM_E_UNINIT);
  }
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_SetFreezeFrameRecordFilter, DEM_E_WRONG_CONFIGURATION);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_SetFreezeFrameRecordFilter, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if ( (DTCFormat != DEM_DTC_FORMAT_OBD) &&
            (DTCFormat != DEM_DTC_FORMAT_UDS) &&
            (DTCFormat != DEM_DTC_FORMAT_J1939)
          )
  {
    DEM_REPORT_ERROR(DEM_SID_SetFreezeFrameRecordFilter, DEM_E_PARAM_DATA);
  }
  else if (NumberOfFilteredRecords == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_SetFreezeFrameRecordFilter, DEM_E_PARAM_POINTER);
  }
  else
#else /* DEM_DEV_ERROR_DETECT */
/* !LINKSTO Dem_OBD_0001,2 */
#if (DEM_OBD_Support == STD_OFF)
  TS_PARAM_UNUSED(DTCFormat);
#endif
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* exit with E_NOT_OK if DTCFormat DEM_DTC_FORMAT_J1939 */
    if (DTCFormat != DEM_DTC_FORMAT_J1939)
    {
      CONSTP2VAR(Dem_RecordFilterType, AUTOMATIC, DEM_VAR_CLEARED) ClientFFRecordFilter =
        &Dem_RecordFilter[ClientId];
#if (DEM_NUM_FFSEGS > 0U)
      /* !LINKSTO Dem.FreezeFrameRecordFilter.Behavior,1 */
      /* filter belongs to primary memory */
      const Dem_SizeEvMemEntryType SizeEventMem =
        Dem_SizeEventMem[(DEM_DTC_ORIGIN_PRIMARY_MEMORY - 1U)];
      const Dem_EventMemoryEntryPtrType Entry =
        Dem_EventMem[(DEM_DTC_ORIGIN_PRIMARY_MEMORY - 1U)];
      Dem_SizeEvMemEntryType Index;

#if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)
      Dem_EventIdType StoredOBDFFEventId = DEM_EVENT_ID_INVALID;
#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)
      Dem_SizeEvMemEntryType SelectedEntryIdx = 0U;
#endif
#endif
#endif
      uint16 NumOfFilteredRecs = 0U;

      /*
       * ENTER critical section to protect the event memory entry
       *       call-context: Dcm (Task)
       */
      DEM_ENTER_EXCLUSIVE_AREA();
#if (DEM_NUM_FFSEGS > 0U)
#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)
      if (Dem_SelectEntryOfOBDFreezeFrameEvent(&SelectedEntryIdx, TRUE) == TRUE)
      {
        StoredOBDFFEventId =
          Dem_EventMem[(DEM_DTC_ORIGIN_PRIMARY_MEMORY - 1U)][SelectedEntryIdx].EventId;
      }
#elif (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
      StoredOBDFFEventId = DEM_OBDFF_STORED_EVENT_ID;
#endif /* (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE) */

      /* count number of currently stored freeze frames */
      for (Index = 0U; Index < SizeEventMem; ++Index)
      {
        const Dem_EventIdType EvId = Entry[Index].EventId;
        if (EvId != DEM_EVENT_ID_INVALID)
        {
          /* event in memory entry is only considered if DTC is assigned and not suppressed
           * (to match with Dem225) */
          /* event availability is checked implicitly */
          if (Dem_GetVisibleDTC(DTCFormat, EvId) != DEM_NO_DTC)
          {
#if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)
            if (EvId == StoredOBDFFEventId)
            {
              NumOfFilteredRecs++;
            }
#endif

          /* !LINKSTO dsn.Dem.Calibration.NoFFClass.PositiveResponse.SetFreezeFrameRecordFilter,1 */
#if (DEM_CALIBRATION_SUPPORT == STD_ON)
            if (DEM_REGULAR_FF_VALID_FFCLASS(EvId))
#endif
            {
              NumOfFilteredRecs +=
                (uint8)DEM_NUMFF(&Entry[Index], Dem_GbiMaxNumFFRecs(EvId));
            }

#if (DEM_DEV_FREEZE_FRAME_USED == STD_ON)
#if (DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U)
            if ( (Entry[Index].DevFFEntryIdx[DEM_DEVFF_A] != DEM_MAX_NUMBER_DEVFFA_ENTRY) &&
                  DEM_DEVFF_VALID(DEM_DTC_ORIGIN_PRIMARY_MEMORY, DEM_DEVFF_A,
                    EvId, Dem_EventDescDevFF[EvId][DEM_DEVFF_A].DevFFNum) )
            {
              NumOfFilteredRecs++;
            }
#endif
#if (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U)
            if ( (Entry[Index].DevFFEntryIdx[DEM_DEVFF_B] != DEM_MAX_NUMBER_DEVFFB_ENTRY) &&
                  DEM_DEVFF_VALID(DEM_DTC_ORIGIN_PRIMARY_MEMORY, DEM_DEVFF_B,
                    EvId, Dem_EventDescDevFF[EvId][DEM_DEVFF_B].DevFFNum) )
            {
              NumOfFilteredRecs++;
            }
#endif
#endif /* (DEM_DEV_FREEZE_FRAME_USED == STD_ON) */
          }
        }
      }
#endif /* (DEM_NUM_FFSEGS > 0U) */

      /*
       * LEAVE critical section
       */
      DEM_EXIT_EXCLUSIVE_AREA();

      /* !LINKSTO Dem.ASR431.FreezeFrameRecordFilter.Validity,1 */
      /* !LINKSTO Dem.ASR431.SWS_Dem_00650,1 */
      /* reset freeze frame record-filter */
      ClientFFRecordFilter->EntryIdx  = 0U;
      ClientFFRecordFilter->RecNumIdx = 0U;

#if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)
      ClientFFRecordFilter->ObdFFConsidered = FALSE;
#endif
#if (DEM_DEV_FREEZE_FRAME_USED == STD_ON)
#if ((DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U) || (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U))
      ClientFFRecordFilter->DevFFConsidered[DEM_DEVFF_A] = FALSE;
      ClientFFRecordFilter->DevFFConsidered[DEM_DEVFF_B] = FALSE;
#endif
#endif
/* !LINKSTO Dem.ASR431.FreezeFrameRecordFilter.SetFilterCriteria,1 */
/* !LINKSTO Dem_OBD_0001,2 */
#if (DEM_OBD_Support == STD_ON)
      ClientFFRecordFilter->DTCFormat = DTCFormat;
#else
      ClientFFRecordFilter->DTCFormat = DEM_DTC_FORMAT_UDS;
#endif
      ClientFFRecordFilter->RecordFilterIsSet = TRUE;
      *NumberOfFilteredRecords = NumOfFilteredRecs;
      Result = E_OK;
    }
  }

  DBG_DEM_SETFREEZEFRAMERECORDFILTER_EXIT(
    Result, ClientId, DTCFormat, NumberOfFilteredRecords);
  return Result;
}

/*----------------[Dem_GetStatusOfDTC]--------------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_GetStatusOfDTC(
  uint8                                                  ClientId,
  P2VAR(Dem_DTCStatusMaskType, AUTOMATIC, DEM_APPL_DATA) DTCStatus)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_GETSTATUSOFDTC_ENTRY(ClientId, DTCStatus);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetStatusOfDTC, DEM_E_UNINIT);
  }
  else if (DTCStatus == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_GetStatusOfDTC, DEM_E_PARAM_POINTER);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_GetStatusOfDTC, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    DEM_REPORT_ERROR(DEM_SID_GetStatusOfDTC, DEM_E_WRONG_CONFIGURATION);
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
      DEM_REPORT_ERROR(DEM_SID_GetStatusOfDTC, DEM_E_WRONG_CONDITION);
    }
    else
#endif /* DEM_DEV_ERROR_DETECT */
    {
      Dem_EventIdType EventId;

      switch(Dem_DoDTCSelection(ClientId, &EventId))
      {
        case DEM_DTC_SELECTOR_WRONG_DTC:
        case DEM_DTC_SELECTOR_DTC_GROUP:
          /* !LINKSTO Dem.ASR431.SWS_Dem_01259,1 */
          Result = DEM_WRONG_DTC;
          break;
        case DEM_DTC_SELECTOR_WRONG_DTC_ORIGIN:
          /* !LINKSTO Dem.ASR431.SWS_Dem_01260,1 */
          Result = DEM_WRONG_DTCORIGIN;
          break;
        case DEM_DTC_SELECTOR_SINGLE_DTC_EVID_CACHED:
          {
#if (DEM_NUM_CMB_DTCS > 0U)
            if (DEM_IS_EVENT_COMBINED(EventId))
            {
              /* if the event is combined the status will be retrieved from Dem_CmbDTCStatus */
              /* !LINKSTO Dem.ASR431.SWS_Dem_00059,1 */
              *DTCStatus =
                Dem_CmbDTCStatus[Dem_GbiDTCConfIdx(EventId)] &
                  DEM_DTC_STATUS_AVAILABILITY_MASK;
            }
            else
#endif
            {
              /* !LINKSTO Dem.ASR431.SWS_Dem_00059,1 */
              *DTCStatus =
                DEM_DTC_STATUS_MASKS[EventId] &
                  DEM_DTC_STATUS_AVAILABILITY_MASK;
            }

            /* the status of the DTC is correctly provided in the DTCStatus parameter */
            Result = E_OK;
          }
          break;
        /* CHECK: NOPARSE */
        default: /* should never be reached */
          DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_GetStatusOfDTC);
          /* Result remains E_NOT_OK */
          break;
        /* CHECK: PARSE */
      }
    }
  }
  DBG_DEM_GETSTATUSOFDTC_EXIT(Result, ClientId, DTCStatus);
  return Result;
}

/*----------------[Dem_GetDTCStatusAvailabilityMask]------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_GetDTCStatusAvailabilityMask(
  uint8 ClientId,
  P2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA) DTCStatusMask)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_GETDTCSTATUSAVAILABILITYMASK_ENTRY(ClientId, DTCStatusMask);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  /* check of Dem_InitializationState is not necessary:
   * it's allowed to get availability mask even if Dem is uninitialized */
  if (DTCStatusMask == NULL_PTR)
  {
    DEM_REPORT_ERROR(
      DEM_SID_GetDTCStatusAvailabilityMask, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    *DTCStatusMask = DEM_DTC_STATUS_AVAILABILITY_MASK;

    Result = E_OK;
  }

  TS_PARAM_UNUSED(ClientId);

  DBG_DEM_GETDTCSTATUSAVAILABILITYMASK_EXIT(Result, ClientId, DTCStatusMask);
  return Result;
}

/*----------------[Dem_GetNumberOfFilteredDTC]------------------------------*/

FUNC(Std_ReturnType, DEM_CODE)
  Dem_GetNumberOfFilteredDTC(
    uint8 ClientId,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredDTC)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_GETNUMBEROFFILTEREDDTC_ENTRY(ClientId, NumberOfFilteredDTC);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetNumberOfFilteredDTC, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNumberOfFilteredDTC, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId >= DEM_NUMBER_OF_CLIENTS)
  {
    DEM_REPORT_ERROR(
      DEM_SID_GetNumberOfFilteredDTC, DEM_E_WRONG_CONFIGURATION);
  }
  else if (NumberOfFilteredDTC == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_GetNumberOfFilteredDTC, DEM_E_PARAM_POINTER);
  }
  else if (Dem_DTCFilter[ClientId].IsSet != TRUE)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01300,1 */
    /* !LINKSTO Dem.ASR431.SetDTCFilter.UnsuccessfulCall,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNumberOfFilteredDTC, DEM_E_WRONG_CONDITION);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    Dem_DTCStatusMaskType IdleDTCStatus; /* status not needed this time */
    sint8 IdleFDC; /* FDC not needed this time */
    uint32 IdleDTC = DEM_NO_DTC;      /* DTC not needed this time */
    uint16 NrOfFilteredDTCs = 0U;       /* total number of counted DTCs */
    Dem_EvIdLeastType EvId;

    /* !LINKSTO Dem_OBD_0005,1 */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U)
    /* !LINKSTO dsn.Dem.OBD.DTCOrigin.Permanent.Dem_GetNumberOfFilteredDTC.0001,1 */
    if (Dem_DTCFilter[ClientId].DTCOrigin == DEM_DTC_ORIGIN_PERMANENT_MEMORY)
    {
      Dem_SizeEvMemEntryType EvMemEntryIdx;

      for (EvMemEntryIdx = 0U; EvMemEntryIdx < DEM_MAX_NUMBER_EVENT_ENTRY_PER; EvMemEntryIdx++)
      {
        const Dem_PermanentMemoryEntryType PermanentMemoryEntry =
            Dem_EventMemoryPermanent[EvMemEntryIdx];
        const Dem_EventIdType CheckedEventId =
            DEM_GET_PERMANENT_MEMORY_EVENT_ID(PermanentMemoryEntry);
        const uint8 NvStatus =
            Dem_EventMemoryPermanentNonVolatileStored[EvMemEntryIdx];

        /* !LINKSTO Dem_OBD_0058,1 */
        if ( (CheckedEventId != DEM_EVENT_ID_INVALID) &&
             (NvStatus == DEM_PERMANENT_MEMORY_NV_STORED) )
        {
          /* !LINKSTO Dem_OBD_0058,1 */
          if (Dem_ApplyDTCFilter(ClientId, CheckedEventId,
                &IdleDTCStatus, &IdleFDC, &IdleDTC) == TRUE)
          {
            /* new match found */
            ++NrOfFilteredDTCs;
          }
        }
      }
    }
    else
#endif /* #if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U) */
    {
#if (DEM_ORDERED_DTC_REPORTING_USED == STD_ON)
      /* !LINKSTO dsn.Dem.ReportingOrderDTC.NumberOfFilteredDTC,1 */
      /* check if chronological order of DTCs is required */
      if (Dem_DTCFilter[ClientId].OccOrdering == TRUE)
      {
        Dem_SizeEvMemEntryType EvIdx;
        const uint8 EventsInMemory = Dem_DTCFilter[ClientId].EventsInMemory;

        /* loop over all order DTCs */
        for (EvIdx = 0U; EvIdx < EventsInMemory; EvIdx++)
        {
          if (Dem_ApplyDTCFilter(ClientId,
                                 Dem_OrderedEventId[ClientId][EvIdx],
                                 &IdleDTCStatus,
                                 &IdleFDC,
                                 &IdleDTC) == TRUE)
          {
            /* new match found */
            ++NrOfFilteredDTCs;
          }
        }
      }
      else
#endif /* (DEM_ORDERED_DTC_REPORTING_USED == STD_ON) */
      {
        /* when calculating the number of filtered DTCs the event memory need not be
         * locked (by DcmAccessInProgress): when interrupting this routine while
         * looping over the events, modifications of events with Id smaller than
         * the current processed Id are neglected, while modifications of events
         * with event Id greater than the current processed Id are represented by
         * the result. Thus the result is never corrupt (but at most represents an
         * obsolete state of the event memory). */
        /* Deviation TASKING-2 */
        for (EvId = 1U; EvId <= DEM_MAX_EVENTID; ++EvId)
        {
#if (DEM_NUM_CMB_DTCS > 0U)
          /* for combined events, only apply DTC filter for the first event involved
           * into combination. Otherwise, the NumberOfFilteredDTC will be incremented
           * for each event combined to the DTC. */
          if ( (Dem_EventIdType)EvId == DEM_GET_MASTER_EVID((Dem_EventIdType)EvId) )
#endif
          {
            /* if status mask matches */
            if (Dem_ApplyDTCFilter(ClientId,
                                   (Dem_EventIdType)EvId,
                                   &IdleDTCStatus,
                                   &IdleFDC,
                                   &IdleDTC) != FALSE)
            {
              /* new match found */
              ++NrOfFilteredDTCs;
            }
          }
        }
      }
    }

    /* update the out parameter with the computed value */
    *NumberOfFilteredDTC = NrOfFilteredDTCs;
    Result = E_OK;
  }

  DBG_DEM_GETNUMBEROFFILTEREDDTC_EXIT(Result, ClientId, NumberOfFilteredDTC);
  return Result;
}

/*----------------[Dem_GetNextFilteredDTC]----------------------------------*/

/* !LINKSTO Dem.ASR431.SWS_Dem_00653,1 */
FUNC(Std_ReturnType, DEM_CODE) Dem_GetNextFilteredDTC(
  uint8                                                  ClientId,
  P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA)           DTC,
  P2VAR(Dem_DTCStatusMaskType, AUTOMATIC, DEM_APPL_DATA) DTCStatus)
{
  Std_ReturnType Result = E_NOT_OK;
  sint8 IdleFDC; /* FDC not needed this time */

  DBG_DEM_GETNEXTFILTEREDDTC_ENTRY(ClientId, DTC, DTCStatus);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredDTC, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredDTC, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId >= DEM_NUMBER_OF_CLIENTS)
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredDTC, DEM_E_WRONG_CONFIGURATION);
  }
  else if ( (DTCStatus == NULL_PTR) || (DTC == NULL_PTR) )
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredDTC, DEM_E_PARAM_POINTER);
  }
  else if (Dem_DTCFilter[ClientId].IsSet != TRUE)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01300,1 */
    /* !LINKSTO Dem.ASR431.SetDTCFilter.UnsuccessfulCall,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredDTC, DEM_E_WRONG_CONDITION);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    CONSTP2VAR(Dem_DTCFilterType, AUTOMATIC, DEM_VAR_CLEARED) ClientDTCFilter =
      &Dem_DTCFilter[ClientId];

    boolean FilterResult = FALSE;
    uint32 DTCValue = DEM_NO_DTC;
    Dem_DTCStatusMaskType DTCStatusValue = 0U;
    /* !LINKSTO Dem.ASR431.SWS_Dem_00216_Implicit1,1 */
    Result = DEM_NO_SUCH_ELEMENT;

    /* !LINKSTO Dem_OBD_0005,1 */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U)
    /* !LINKSTO dsn.Dem.OBD.DTCOrigin.Permanent.Dem_GetNextFilteredDTC.0001,1 */
    if (ClientDTCFilter->DTCOrigin == DEM_DTC_ORIGIN_PERMANENT_MEMORY)
    {
      while ( (FilterResult == FALSE) &&
              (ClientDTCFilter->NextItem <= DEM_MAX_NUMBER_EVENT_ENTRY_PER) )
      {
        /* even if NextEvMemEntryIdx is of smaller type than EventId, the cast
        * is OK because EventId lesser than DEM_MAX_NUMBER_EVENT_ENTRY_PER will
        * only be processed within the while loop */
        const Dem_SizeEvMemEntryType NextEvMemEntryIdx =
            (Dem_SizeEvMemEntryType)ClientDTCFilter->NextItem - 1U;
        const Dem_PermanentMemoryEntryType PermanentMemoryEntry =
            Dem_EventMemoryPermanent[NextEvMemEntryIdx];
        const Dem_EventIdType CheckedEventId =
            DEM_GET_PERMANENT_MEMORY_EVENT_ID(PermanentMemoryEntry);
        const uint8 NvStatus =
            Dem_EventMemoryPermanentNonVolatileStored[NextEvMemEntryIdx];

        if ( (CheckedEventId != DEM_EVENT_ID_INVALID) &&
            (NvStatus == DEM_PERMANENT_MEMORY_NV_STORED) )
        {
          /* !LINKSTO Dem_OBD_0068,1, Dem_OBD_0057,1 */
          FilterResult = Dem_ApplyDTCFilter(ClientId, CheckedEventId,
            &DTCStatusValue, &IdleFDC, &DTCValue);

          if (FilterResult == TRUE)
          {
            /* next matching DTC found */
            Result = E_OK;
          }
        }

        ClientDTCFilter->NextItem++;
      }
    }
    else
#endif /* #if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U) */
    {
      /* search for first DTC fulfilling the filter criterion */
#if (DEM_ORDERED_DTC_REPORTING_USED == STD_ON)
      while ( (ClientDTCFilter->NextItem <= DEM_MAX_EVENTID) &&
              (FilterResult == FALSE) &&
              ( (ClientDTCFilter->OccOrdering == FALSE) ||
                (ClientDTCFilter->NextItem <= ClientDTCFilter->EventsInMemory) ) )
#else
      while ( (ClientDTCFilter->NextItem <= DEM_MAX_EVENTID) &&
              (FilterResult == FALSE) )
#endif
      {
        Dem_EventIdType NextEventId;
#if (DEM_NUM_CMB_DTCS > 0U)
        boolean ProceedFiltering = TRUE;
#endif

#if (DEM_ORDERED_DTC_REPORTING_USED == STD_ON)
        if (ClientDTCFilter->OccOrdering == TRUE)
        {
          /* !LINKSTO dsn.Dem.ReportingOrderDTC.CurrentDTCReportingOrder.ApplyFilter,1 */
          NextEventId =
            Dem_OrderedEventId[ClientId][ClientDTCFilter->NextItem - 1U];
        }
        else
#endif
        {
          NextEventId = (Dem_EventIdType)ClientDTCFilter->NextItem;

#if (DEM_NUM_CMB_DTCS > 0U)
          /* only apply DTC filter for the first event involved in combination,
          * otherwise the cmb DTC might be returned for each cmb event linked to this DTC */
          if (NextEventId != DEM_GET_MASTER_EVID(NextEventId))
          {
            ProceedFiltering = FALSE;
          }
#endif
        }

#if (DEM_NUM_CMB_DTCS > 0U)
        if (ProceedFiltering == TRUE)
#endif
        {
          FilterResult = Dem_ApplyDTCFilter(ClientId,
            NextEventId, &DTCStatusValue, &IdleFDC, &DTCValue);

          /* !LINKSTO Dem.ReadDTCInformation.StatusOfDTC,1 */
          /* skip events not matching the filter criterion */
          if (FilterResult == TRUE)
          {
            /* mask out unsupported bits in DTC status */
            DTCStatusValue &= DEM_DTC_STATUS_AVAILABILITY_MASK;
            /* next matching DTC found */
            Result = E_OK;
          }
        }

        /* update NextItem for next call in filter */
        ClientDTCFilter->NextItem++;
      }
    }

    /* update the out parameters with the computed values */
    *DTC       = DTCValue;
    *DTCStatus = DTCStatusValue;
  }
  DBG_DEM_GETNEXTFILTEREDDTC_EXIT(Result, ClientId, DTC, DTCStatus);
  return Result;
}

#if (DEM_GET_DTCBYOCCURRENCETIME_SUPPORT == STD_ON)
/*----------------[Dem_GetDTCByOccurrenceTime]------------------------------*/

FUNC(Std_ReturnType, DEM_CODE)
  Dem_GetDTCByOccurrenceTime(
    uint8                                        ClientId,
    Dem_DTCRequestType                           DTCRequest,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA)      DTC)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_GETDTCBYOCCURRENCETIME_ENTRY(ClientId, DTCRequest, DTC);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetDTCByOccurrenceTime, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_GetDTCByOccurrenceTime, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_GetDTCByOccurrenceTime, DEM_E_WRONG_CONFIGURATION);
  }
  else if ( (DTCRequest != DEM_FIRST_FAILED_DTC) &&
            (DTCRequest != DEM_MOST_RECENT_FAILED_DTC) &&
            (DTCRequest != DEM_FIRST_DET_CONFIRMED_DTC) &&
            (DTCRequest != DEM_MOST_REC_DET_CONFIRMED_DTC) )
  {
    DEM_REPORT_ERROR(DEM_SID_GetDTCByOccurrenceTime, DEM_E_PARAM_DATA);
  }
  else if (DTC == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_GetDTCByOccurrenceTime, DEM_E_PARAM_POINTER);
  }
  else
#else /* DEM_DEV_ERROR_DETECT */
  TS_PARAM_UNUSED(ClientId);
#endif /* DEM_DEV_ERROR_DETECT */
  {
    uint32 OccurKindDTC;
    Dem_EventIdType EventId;

    /* !LINKSTO Dem.ASR431.SWS_Dem_00221,1 */
    /* presume that the requested data is not available */
    Result = DEM_NO_SUCH_ELEMENT;

    /*
     * ENTER critical section to protect access to Dem_NvData
     */
    DEM_ENTER_EXCLUSIVE_AREA();

    /* !LINKSTO Dem219,1 */
    /* identify event based on the request type received */
    EventId = DEM_OCCURKIND_STORED_EVENT_ID(DTCRequest);

    /*
     * LEAVE critical section
     */
    DEM_EXIT_EXCLUSIVE_AREA();

    /* check event for validity */
    if (EventId != DEM_EVENT_ID_INVALID)
    {
      /* identify DTC based on event ID */
      OccurKindDTC = Dem_GetVisibleUdsDTC(EventId);

      /* check if DTC is suppressed */
      if (OccurKindDTC != DEM_NO_DTC)
      {
        *DTC = OccurKindDTC;
        Result = E_OK;
      }
    }
  }

  DBG_DEM_GETDTCBYOCCURRENCETIME_EXIT(Result, ClientId, DTCRequest, DTC);
  return Result;
}
#endif /* (DEM_GET_DTCBYOCCURRENCETIME_SUPPORT == STD_ON) */

/*----------------[Dem_GetNextFilteredRecord]-------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_GetNextFilteredRecord(
    uint8                                   ClientId,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  RecordNumber)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_GETNEXTFILTEREDRECORD_ENTRY(ClientId, DTC, RecordNumber);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredRecord, DEM_E_UNINIT);
  }
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredRecord, DEM_E_WRONG_CONFIGURATION);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredRecord, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (DTC == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredRecord, DEM_E_PARAM_POINTER);
  }
  else if (RecordNumber == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredRecord, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    CONSTP2VAR(Dem_RecordFilterType, AUTOMATIC, DEM_VAR_CLEARED) ClientFFRecordFilter =
      &Dem_RecordFilter[ClientId];
    if (ClientFFRecordFilter->RecordFilterIsSet != FALSE)
    {
      const Dem_EventMemoryEntryPtrType Entry =
        Dem_EventMem[(DEM_DTC_ORIGIN_PRIMARY_MEMORY - 1U)];
#if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)
      Dem_EventIdType StoredOBDFFEventId = DEM_EVENT_ID_INVALID;
#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)
      Dem_SizeEvMemEntryType SelectedEntryIdx = 0U;
#endif
#endif
      Dem_FFIdxType MaxNumFF = 0U;
      /* quoting SWS: "This filter always belongs to primary memory." */
      const Dem_SizeEvMemEntryType SizeEventMem =
        Dem_SizeEventMem[(DEM_DTC_ORIGIN_PRIMARY_MEMORY - 1U)];
      Result = DEM_NO_SUCH_ELEMENT;

      /* if Dcm disables DTCRecordUpdate while retrieving all records by subsequent
       * API-calls, data can not get inconsistent */
      /*
       * ENTER critical section to protect the event memory entry
       *       call-context: Dcm (Task)
       */
      DEM_ENTER_EXCLUSIVE_AREA();

#if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)
#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)
        if (Dem_SelectEntryOfOBDFreezeFrameEvent(&SelectedEntryIdx, TRUE) == TRUE)
        {
          StoredOBDFFEventId =
            Dem_EventMem[(DEM_DTC_ORIGIN_PRIMARY_MEMORY - 1U)][SelectedEntryIdx].EventId;
        }
#elif (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
        StoredOBDFFEventId = DEM_OBDFF_STORED_EVENT_ID;
#endif /* (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE) */
#endif /* (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)  */

      /* search for event memory entry, which contains the next record */
      while (ClientFFRecordFilter->EntryIdx < SizeEventMem)
      {
        const Dem_EventMemoryEntryPtrType EvMemEntry =
              &Entry[ClientFFRecordFilter->EntryIdx];
        const Dem_EventIdType EventId = EvMemEntry->EventId;

        if (EventId != DEM_EVENT_ID_INVALID)
        {
          boolean FFRecordFound = FALSE;
          const uint32 DTCValue =
            Dem_GetVisibleDTC(ClientFFRecordFilter->DTCFormat, EventId);

#if (DEM_NUM_FFSEGS > 0U)
          MaxNumFF = Dem_GbiMaxNumFFRecs(EventId);
#endif
          if (DTCValue != DEM_NO_DTC)
          {
            Dem_FFIdxType NumStoredFFs = (Dem_FFIdxType)DEM_NUMFF(EvMemEntry, MaxNumFF);

            /* !LINKSTO dsn.Dem.SetFreezeFrameRecordFilter.AllStoredFFRecords,1 */
            /* !LINKSTO dsn.Dem.FreezeFrameRecordFilter.OrderOfRetrievingFF,1 */
            /* !LINKSTO Dem.ASR431.SWS_Dem_00225,1 */
#if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)
            if ( (StoredOBDFFEventId == EventId) &&
                 (FALSE == ClientFFRecordFilter->ObdFFConsidered) )
            {
              /* event memory entry with demanded freeze frame record reached */
              *DTC = DTCValue;
              /* OBDFF record number */
              *RecordNumber = 0x00U;

              /* set the flag */
              ClientFFRecordFilter->ObdFFConsidered = TRUE;

              FFRecordFound = TRUE;
            }
#endif /* (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)  */

#if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)
            if ( (NumStoredFFs > ClientFFRecordFilter->RecNumIdx) &&
                 (FALSE == FFRecordFound) )
#else
            if (NumStoredFFs > ClientFFRecordFilter->RecNumIdx)
#endif /* (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)  */
            {
#if (DEM_POSITIVE_RESPONSE_NO_FFCLASSES == STD_ON)
              /* !LINKSTO dsn.Dem.Calibration.NoFFClass.PositiveResponse.GetNextFilteredRecord,1 */
              if (DEM_REGULAR_FF_VALID_FFCLASS(EventId))
#endif
              {
                /* event memory entry with demanded freeze frame record reached */
                *DTC = DTCValue;

                /* calculate FF record number (relative addressing) */
#if (DEM_FREEZE_FRAME_REC_NUMERATION_TYPE == DEM_FF_RECNUM_CALCULATED)
                *RecordNumber = ClientFFRecordFilter->RecNumIdx + 1U;
#else
                *RecordNumber =
                  Dem_FFRecNumerationClass[Dem_GbiFFRecNumClassIdx(EventId)].
                    Dem_FFRecNumClassIdx[ClientFFRecordFilter->RecNumIdx];
#endif

                ClientFFRecordFilter->RecNumIdx++;

                FFRecordFound = TRUE;
              }
            }

#if (DEM_DEV_FREEZE_FRAME_USED == STD_ON)
#if ((DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U) || (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U))
            {
              uint8 IdxDevFFKind;

              for (IdxDevFFKind = DEM_DEVFF_A;
                   IdxDevFFKind <= DEM_DEVFF_B;
                   ++IdxDevFFKind)
              {
                /* !LINKSTO dsn.Dem.GetNextFilteredRecord.FFRecRecognizedFlags,2 */
                if ( (EvMemEntry->DevFFEntryIdx[IdxDevFFKind] != Dem_SizeDevFFMem[IdxDevFFKind]) &&
                     (FALSE == ClientFFRecordFilter->DevFFConsidered[IdxDevFFKind]) &&
                     (FALSE == FFRecordFound) &&
                     DEM_DEVFF_VALID(DEM_DTC_ORIGIN_PRIMARY_MEMORY, IdxDevFFKind,
                       EventId, Dem_EventDescDevFF[EventId][IdxDevFFKind].DevFFNum) )
                {
                  /* event memory entry with demanded freeze frame record reached */
                  *DTC = DTCValue;
                  /* get the DEVFF record number */
                  *RecordNumber = Dem_EventDescDevFF[EventId][IdxDevFFKind].DevFFNum;

                  /* set the flag */
                  ClientFFRecordFilter->DevFFConsidered[IdxDevFFKind] = TRUE;

                  FFRecordFound = TRUE;
                }
              }
            }
#endif /* (DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U) || (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U) */
#endif /* (DEM_DEV_FREEZE_FRAME_USED == STD_ON) */
          }

          if (TRUE == FFRecordFound)
          {
            Result = E_OK;
            break;
          }
        }
        /* !LINKSTO dsn.Dem.GetNextFilteredRecord.FFRecRecognizedFlags.Reset,2 */
#if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)
        ClientFFRecordFilter->ObdFFConsidered = FALSE;
#endif
#if (DEM_DEV_FREEZE_FRAME_USED == STD_ON)
#if ((DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U) || (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U))
        ClientFFRecordFilter->DevFFConsidered[DEM_DEVFF_A] = FALSE;
        ClientFFRecordFilter->DevFFConsidered[DEM_DEVFF_B] = FALSE;
#endif
#endif

        ClientFFRecordFilter->EntryIdx++;
        ClientFFRecordFilter->RecNumIdx = 0U;
      }

      /*
       * LEAVE critical section
       */
      DEM_EXIT_EXCLUSIVE_AREA();
    }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    else
    {
      /* !LINKSTO Dem.ASR431.FreezeFrameRecordFilter.WrongCondition,1 */
      DEM_REPORT_ERROR(DEM_SID_GetNextFilteredRecord, DEM_E_WRONG_CONDITION);
    }
#endif /* DEM_DEV_ERROR_DETECT */
  }

  DBG_DEM_GETNEXTFILTEREDRECORD_EXIT(Result, ClientId, DTC, RecordNumber);
  return Result;
}

/*----------------[Dem_GetNextFilteredDTCAndFDC]----------------------------*/
/* !LINKSTO Dem.ASR431.SWS_Dem_00228_Implicit1,1 */
FUNC(Std_ReturnType, DEM_CODE) Dem_GetNextFilteredDTCAndFDC(
  uint8 ClientId,
  P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
  P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA)  DTCFaultDetectionCounter)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_GETNEXTFILTEREDDTCANDFDC_ENTRY(
    ClientId, DTC, DTCFaultDetectionCounter);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredDTCAndFDC, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredDTCAndFDC, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId >= DEM_NUMBER_OF_CLIENTS)
  {
    DEM_REPORT_ERROR(
      DEM_SID_GetNextFilteredDTCAndFDC, DEM_E_WRONG_CONFIGURATION);
  }
  else if ( (DTC == NULL_PTR) || (DTCFaultDetectionCounter == NULL_PTR) )
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredDTCAndFDC, DEM_E_PARAM_POINTER);
  }
  else if (Dem_DTCFilter[ClientId].IsSet != TRUE)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01300,1 */
    /* !LINKSTO Dem.ASR431.SetDTCFilter.UnsuccessfulCall,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredDTCAndFDC, DEM_E_WRONG_CONDITION);
  }
  else if (Dem_DTCFilter[ClientId].FilterForFDC == FALSE)
  {
    /* API was called while FDC-filtering is inactive (not sensible)
     * -> this API is not functional by optimization, as FDC information would
     *    not be retrieved and be returned bogus by Dem_ApplyDTCFilter() */
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredDTCAndFDC, DEM_E_WRONG_CONDITION);
  }
/* !LINKSTO Dem_OBD_0005,1 */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U)
  /* !LINKSTO dsn.Dem.OBD.DTCOrigin.Permanent.Dem_GetNextFilteredDTCAndFDC.0001,3 */
  else if (Dem_DTCFilter[ClientId].DTCOrigin == DEM_DTC_ORIGIN_PERMANENT_MEMORY)
  {
    /* API was called with permanent memory as Origin
     * -> this API is not supported for permanent memory */
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredDTCAndFDC, DEM_E_WRONG_CONDITION);
  }
#endif
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    boolean FilterResult = FALSE;
    Result = DEM_NO_SUCH_ELEMENT;
    /* function NOT fully implemented yet.
     * filter criteria related to DTCFormat and DTCKind is not implemented
     * since there is no OBD support.
     * implementation is scheduled for future releases. */

    /* search for first DTC fulfilling the filter criterion */
    while ( (Dem_DTCFilter[ClientId].NextItem <= DEM_MAX_EVENTID) &&
            (FilterResult == FALSE)
          )
    {
      Dem_DTCStatusMaskType IdleDTCStatus; /* status not needed this time */
      const Dem_EventIdType NextEventId =
        (Dem_EventIdType)Dem_DTCFilter[ClientId].NextItem;

#if (DEM_NUM_CMB_DTCS > 0U)
      /* only apply DTC filter for the first event involved into
        * combination, otherwise the cmb DTC might be returned
        * for each cmb event linked to this DTC */
      if (NextEventId == DEM_GET_MASTER_EVID(NextEventId))
#endif
      {
        FilterResult = Dem_ApplyDTCFilter(
          ClientId,
          NextEventId,
          &IdleDTCStatus,
          DTCFaultDetectionCounter,
          DTC);

        /* skip events not matching the filter criterion */
        if (FilterResult == TRUE)
        {
          /* next matching DTC found */
          Result = E_OK;
        }
      }

      /* store event id for next call in filter */
      ++Dem_DTCFilter[ClientId].NextItem;
    }
  }
  DBG_DEM_GETNEXTFILTEREDDTCANDFDC_EXIT(
    Result, ClientId, DTC, DTCFaultDetectionCounter);
  return Result;
}

/*----------------[Dem_GetNextFilteredDTCAndSeverity]-----------------------*/

/* !LINKSTO Dem.ASR431.SWS_Dem_00287_Implicit1,1 */
FUNC(Std_ReturnType, DEM_CODE)
  Dem_GetNextFilteredDTCAndSeverity(
    uint8                                                ClientId,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA)              DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)               DTCStatus,
    P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_APPL_DATA) DTCSeverity,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)               DTCFunctionalUnit)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_GETNEXTFILTEREDDTCANDSEVERITY_ENTRY(
    ClientId, DTC, DTCStatus, DTCSeverity, DTCFunctionalUnit);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredDTCAndSeverity, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredDTCAndSeverity, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId >= DEM_NUMBER_OF_CLIENTS)
  {
    DEM_REPORT_ERROR(
      DEM_SID_GetNextFilteredDTCAndSeverity, DEM_E_WRONG_CONFIGURATION);
  }
  else if (DTC == NULL_PTR)
  {
    DEM_REPORT_ERROR(
      DEM_SID_GetNextFilteredDTCAndSeverity, DEM_E_PARAM_POINTER);
  }
  else if (DTCStatus == NULL_PTR)
  {
    DEM_REPORT_ERROR(
      DEM_SID_GetNextFilteredDTCAndSeverity, DEM_E_PARAM_POINTER);
  }
  else if (DTCSeverity == NULL_PTR)
  {
    DEM_REPORT_ERROR(
      DEM_SID_GetNextFilteredDTCAndSeverity, DEM_E_PARAM_POINTER);
  }
  else if (DTCFunctionalUnit == NULL_PTR)
  {
    DEM_REPORT_ERROR(
      DEM_SID_GetNextFilteredDTCAndSeverity, DEM_E_PARAM_POINTER);
  }
  else if (Dem_DTCFilter[ClientId].IsSet != TRUE)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01300,1 */
    /* !LINKSTO Dem.ASR431.SetDTCFilter.UnsuccessfulCall,1 */
    DEM_REPORT_ERROR(
      DEM_SID_GetNextFilteredDTCAndSeverity, DEM_E_WRONG_CONDITION);
  }
/* !LINKSTO Dem_OBD_0005,1 */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U)
  /* !LINKSTO dsn.Dem.OBD.DTCOrigin.Permanent.Dem_GetNextFilteredDTCAndSeverity.0001,5 */
  else if (Dem_DTCFilter[ClientId].DTCOrigin == DEM_DTC_ORIGIN_PERMANENT_MEMORY)
  {
    /* API was called with permanent memory as Origin
     * -> this API is not supported for permanent memory */
    DEM_REPORT_ERROR(
      DEM_SID_GetNextFilteredDTCAndSeverity, DEM_E_WRONG_CONDITION);
  }
#endif
  else if (Dem_DTCFilter[ClientId].FilterWithSeverity == FALSE)
  {
    /* API was called while severity-filtering is inactive
     * (functional, but not sensible) */
    DEM_REPORT_ERROR(
      DEM_SID_GetNextFilteredDTCAndSeverity, DEM_E_WRONG_CONDITION);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    boolean FilterResult = FALSE;
    Result = DEM_NO_SUCH_ELEMENT;

    /* function NOT fully implemented yet.
     * filter criteria related to DTCFormat is not implemented
     * since there is no OBD support.
     * implementation is scheduled for future releases. */

    /* search for first DTC fulfilling the filter criterion */
    while ( (Dem_DTCFilter[ClientId].NextItem <= DEM_MAX_EVENTID) &&
            (FilterResult == FALSE)
          )
    {
      sint8 IdleFDC; /* FDC not needed this time */
      const Dem_EventIdType NextEventId =
        (Dem_EventIdType)Dem_DTCFilter[ClientId].NextItem;

#if (DEM_NUM_CMB_DTCS > 0U)
      /* only apply DTC filter for the first event involved into combination,
      * otherwise the cmb DTC might be returned for each cmb event linked to this DTC */
      if (NextEventId == DEM_GET_MASTER_EVID(NextEventId))
#endif
      {
        uint8 DTCStatusValue = 0U;

        FilterResult = Dem_ApplyDTCFilter(
          ClientId, NextEventId, &DTCStatusValue, &IdleFDC, DTC);

        /* !LINKSTO Dem.ReadDTCInformation.StatusOfDTC,1 */
        /* skip events not matching the filter criterion */
        if (FilterResult == TRUE)
        {
          /* mask out unsupported bits in DTC status */
          DTCStatusValue &= DEM_DTC_STATUS_AVAILABILITY_MASK;
          /* !LINKSTO Dem.ASR431.SWS_Dem_01291,1 */
          /* !LINKSTO Dem.ASR431.SWS_Dem_01292,1 */
          /* get the severity mapped to event Id */
          *DTCSeverity = Dem_GbiDTCSeverity(NextEventId);
          /* !LINKSTO Dem.ASR431.SWS_Dem_01293,1 */
          /* get the functional unit mapped to event Id */
          *DTCFunctionalUnit = Dem_GbiDTCFunctionalUnit(NextEventId);

          /* next matching DTC found */
          Result = E_OK;
        }

        /* update out parameter for DTC status with computed values */
        *DTCStatus = DTCStatusValue;
      }

      /* store event id for next call in filter */
      ++Dem_DTCFilter[ClientId].NextItem;
    }
  }

  DBG_DEM_GETNEXTFILTEREDDTCANDSEVERITY_EXIT(
    Result, ClientId, DTC, DTCStatus, DTCSeverity, DTCFunctionalUnit);
  return Result;
}

/*----------------[Dem_GetTranslationType]----------------------------------*/

FUNC(Dem_DTCTranslationFormatType, DEM_CODE) Dem_GetTranslationType(
  uint8 ClientId)
{
  DBG_DEM_GETTRANSLATIONTYPE_ENTRY(ClientId);

  TS_PARAM_UNUSED(ClientId);

  DBG_DEM_GETTRANSLATIONTYPE_EXIT(ClientId, DEM_TYPE_OF_DTC_SUPPORTED);

  return DEM_TYPE_OF_DTC_SUPPORTED;
}

/*----------------[Dem_GetSeverityOfDTC]------------------------------------*/

/* !LINKSTO Dem.ASR431.DTCSelectorDependentAPIs,1 */
FUNC(Std_ReturnType, DEM_CODE)
  Dem_GetSeverityOfDTC(
    uint8                                                ClientId,
    P2VAR(Dem_DTCSeverityType, AUTOMATIC, DEM_APPL_DATA) DTCSeverity)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_GETSEVERITYOFDTC_ENTRY(ClientId, DTCSeverity);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetSeverityOfDTC, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_GetSeverityOfDTC, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_GetSeverityOfDTC, DEM_E_WRONG_CONFIGURATION);
  }
  else if (DTCSeverity == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_GetSeverityOfDTC, DEM_E_PARAM_POINTER);
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
      DEM_REPORT_ERROR(DEM_SID_GetSeverityOfDTC, DEM_E_WRONG_CONDITION);
    }
    else
#endif /* DEM_DEV_ERROR_DETECT */
    {
      Dem_EventIdType EventId;
      switch (Dem_DoDTCSelection(ClientId, &EventId))
      {
        case DEM_DTC_SELECTOR_SINGLE_DTC_EVID_CACHED:
          /* !LINKSTO Dem.ASR431.SWS_Dem_01291,1 */
          /* !LINKSTO Dem.ASR431.SWS_Dem_01292,1 */
          /* get the severity mapped to event Id */
          *DTCSeverity = Dem_GbiDTCSeverity(EventId);
          Result = E_OK;
          break;

        case DEM_DTC_SELECTOR_DTC_GROUP:
        case DEM_DTC_SELECTOR_WRONG_DTC:
          /* !LINKSTO Dem.ASR431.DTCSelectorWrongDTC,1 */
          /* !LINKSTO Dem.DTCSelector.DependentAPIs.SupportedDTCFormat,1 */
          /* !LINKSTO Dem.ASR431.DemDTCSeverity_Implicit1,1 */
          Result = DEM_WRONG_DTC;
          break;

        case DEM_DTC_SELECTOR_WRONG_DTC_ORIGIN:
          /* !LINKSTO Dem.ASR431.SWS_Dem_01262,1 */
          Result = DEM_WRONG_DTCORIGIN;
          break;

        /* CHECK: NOPARSE */
        default: /* should never be reached */
          DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_GetSeverityOfDTC);
          break;
        /* CHECK: PARSE */
      }
    }
  }

  DBG_DEM_GETSEVERITYOFDTC_EXIT(Result, ClientId, DTCSeverity);
  return Result;
}

/*------------------[Dem_GetFunctionalUnitOfDTC]----------------------------*/

/* !LINKSTO Dem.ASR431.DTCSelectorDependentAPIs,1 */
FUNC(Std_ReturnType, DEM_CODE)
  Dem_GetFunctionalUnitOfDTC(
    uint8                                  ClientId,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) DTCFunctionalUnit)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_GETFUNCTIONALUNITOFDTC_ENTRY(ClientId, DTCFunctionalUnit);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetFunctionalUnitOfDTC, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_GetFunctionalUnitOfDTC, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_GetFunctionalUnitOfDTC, DEM_E_WRONG_CONFIGURATION);
  }
  else if (DTCFunctionalUnit == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_GetFunctionalUnitOfDTC, DEM_E_PARAM_POINTER);
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
      DEM_REPORT_ERROR(DEM_SID_GetFunctionalUnitOfDTC, DEM_E_WRONG_CONDITION);
    }
    else
#endif /* DEM_DEV_ERROR_DETECT */
    {
      Dem_EventIdType EventId;
      switch (Dem_DoDTCSelection(ClientId, &EventId))
      {
        case DEM_DTC_SELECTOR_SINGLE_DTC_EVID_CACHED:
          /* !LINKSTO Dem.ASR431.SWS_Dem_01293,1 */
          /* get the functional unit mapped to event Id */
          *DTCFunctionalUnit = Dem_GbiDTCFunctionalUnit(EventId);
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
          DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_GetFunctionalUnitOfDTC);
          break;
        /* CHECK: PARSE */
      }
    }
  }

  DBG_DEM_GETFUNCTIONALUNITOFDTC_EXIT(Result, ClientId, DTCFunctionalUnit);
  return Result;
}

/*-------------------[Vendor specific APIs]---------------------------------*/

/*----------------[Dem_DcmSetStoredDTCFilter]-------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmSetStoredDTCFilter(
  uint8              ClientId,
  Dem_DTCOriginType  DTCOrigin,
  Dem_DataKindType   DataKind)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_DCMSETSTOREDDTCFILTER_ENTRY(ClientId, DTCOrigin, DataKind);

  /* !LINKSTO dsn.Dem.Dem_DcmSetStoredDTCFilter.Algorithm,2 */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_SetStoredDTCFilter, DEM_E_UNINIT);
  }
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_SetStoredDTCFilter, DEM_E_WRONG_CONFIGURATION);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_SetStoredDTCFilter, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if ( (DataKind != DEM_DATAKIND_FREEZEFRAME) &&
            (DataKind != DEM_DATAKIND_EXTENDEDDATA)
          )
  {
    /* !LINKSTO Dem.DetReport.Dem_DcmSetStoredDTCFilter.WrongDataKind,2 */
    DEM_REPORT_ERROR(DEM_SID_SetStoredDTCFilter, DEM_E_PARAM_DATA);
  }
  else if ( (DTCOrigin > (Dem_DTCOriginType)0x01FFU) ||
            ( (DTCOrigin < (Dem_DTCOriginType)0x0100U) &&
              (DTCOrigin != DEM_DTC_ORIGIN_PRIMARY_MEMORY) &&
              (DTCOrigin != DEM_DTC_ORIGIN_MIRROR_MEMORY) &&
              (DTCOrigin != DEM_DTC_ORIGIN_PERMANENT_MEMORY) &&
              (DTCOrigin != DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY)
            )
          )
  {
    /* !LINKSTO Dem.DetReport.Dem_DcmSetStoredDTCFilter.WrongDTCOrigin,2 */
    DEM_REPORT_ERROR(DEM_SID_SetStoredDTCFilter, DEM_E_PARAM_DATA);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* variable indicating that DTC_ORIGIN is valid */
    boolean ValidDTCOrigin = FALSE;
#if (DEM_NUMBER_OF_CLIENTS > 1U)
    /* variable indicating that service will be locked */
    boolean LockingStoredDTCFilter = FALSE;
#endif
    Dem_DTCOriginType MappedDTCOrigin = Dem_GetMappedDTCOrigin(
      DTCOrigin, DEM_DTC_ORIGIN_MAPPING_DCM_IF);

    /* check if requested DTC_ORIGIN is mapped */
    if(MappedDTCOrigin != DEM_DTC_ORIGIN_INVALID)
    {
      /* check if DTC origin is supported */
      if (Dem_SizeEventMem[MappedDTCOrigin - 1U] != (Dem_SizeEvMemEntryType)0U)
      {
        ValidDTCOrigin = TRUE;
      }
    }

#if (DEM_NUMBER_OF_CLIENTS > 1U)
    /*
     * ENTER critical section to protect Dem_StoredDTCFilterLockByClientId
     *       from concurrent access by multiple clients
     */
    DEM_ENTER_EXCLUSIVE_AREA();

    /* no client set a filter yet OR
     * the client owning the lock resets the filter OR
     * the read operation by the last client is finished */
    /* !LINKSTO Dem.Dem_DcmSetStoredDTCFilter.Attributes,2 */
    if ( (Dem_StoredDTCFilterLockByClientId == DEM_CLIENT_ID_INVALID) ||
         (Dem_StoredDTCFilterLockByClientId == ClientId) ||
         (Dem_ReadStoredDTCOpDone == TRUE)
       )
    {
      if (ValidDTCOrigin == TRUE)
      {
        /* lock the resource */
        Dem_StoredDTCFilterLockByClientId = ClientId;
        LockingStoredDTCFilter = TRUE;
      }
      else
      {
        /* release the lock */
        Dem_StoredDTCFilterLockByClientId = DEM_CLIENT_ID_INVALID;
      }
    }

    /*
     * LEAVE critical section
     */
    DEM_EXIT_EXCLUSIVE_AREA();

    if (LockingStoredDTCFilter == TRUE)
#else
    if (ValidDTCOrigin == TRUE)
#endif /* (DEM_NUMBER_OF_CLIENTS > 1U) */
    {
      /* save filter information in internal data structure */
      /* !LINKSTO Dem.Dem_DcmSetStoredDTCFilter.Validity,1 */
      Dem_StoredDTCFilter.DTCOriginIdx = (uint8)(MappedDTCOrigin - 1U);
      Dem_StoredDTCFilter.DataKind     = DataKind;

      /* reset next event memory entry index to the first
       * event memory entry index */
      Dem_StoredDTCFilter.NextEntryIdx = 0U;

#if (DEM_NUMBER_OF_CLIENTS > 1U)
      /* mark that read operation started */
      Dem_ReadStoredDTCOpDone = FALSE;
#else
      /* mark filter attributes valid */
      Dem_StoredDTCFilterLockByClientId = ClientId;
#endif
      Result = E_OK;
    }
#if ( (DEM_DEV_ERROR_DETECT == STD_ON) && (DEM_NUMBER_OF_CLIENTS > 1U) )
    else if (ValidDTCOrigin == TRUE)
    {
      /* the lock is already owned by another client OR
       * the read operation by the last client is not finished
       * Result is E_NOT_OK */
      /* !LINKSTO Dem.StoredDTCFilter.DetWrongCondition,1 */
      DEM_REPORT_ERROR(DEM_SID_SetStoredDTCFilter, DEM_E_WRONG_CONDITION);
    }
#endif /* (DEM_DEV_ERROR_DETECT == STD_ON) && (DEM_NUMBER_OF_CLIENTS > 1U) */
    else
    {
#if (DEM_NUMBER_OF_CLIENTS == 1U)
      /* invalidate filter attributes */
      Dem_StoredDTCFilterLockByClientId = DEM_CLIENT_ID_INVALID;
#endif
      /* !LINKSTO Dem.Dem_DcmSetStoredDTCFilter.Unavailable,2 */
      /* Result is E_NOT_OK */
    }
  }

  DBG_DEM_DCMSETSTOREDDTCFILTER_EXIT(Result, ClientId, DTCOrigin, DataKind);
  return Result;
}

/*----------------[Dem_DcmGetNextFilteredStoredDTC]-------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetNextFilteredStoredDTC(
    uint8                                   ClientId,
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_DCMGETNEXTFILTEREDSTOREDDTC_ENTRY(ClientId, DTC);

  /* !LINKSTO dsn.Dem.Dem_DcmGetNextFilteredStoredDTC.Algorithm,2 */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredStoredDTC, DEM_E_UNINIT);
  }
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredStoredDTC, DEM_E_WRONG_CONFIGURATION);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredStoredDTC, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (DTC == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredStoredDTC, DEM_E_PARAM_POINTER);
  }
  else if (Dem_StoredDTCFilterLockByClientId != ClientId)
  {
    /* !LINKSTO Dem.StoredDTCFilter.SequenceCheck,1 */
    /* !LINKSTO Dem.StoredDTCFilter.DetWrongCondition,1 */
    DEM_REPORT_ERROR(DEM_SID_GetNextFilteredStoredDTC, DEM_E_WRONG_CONDITION);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* Deviation TASKING-1 */
    if (Dem_StoredDTCFilterLockByClientId == ClientId)
    {
      uint32 StoredDTC = DEM_NO_DTC;
      Dem_EventIdType EventId;
      const Dem_SizeEvMemEntryType SizeEventMem =
        Dem_SizeEventMem[Dem_StoredDTCFilter.DTCOriginIdx];
      const Dem_EventMemoryEntryPtrType EventMem =
        Dem_EventMem[Dem_StoredDTCFilter.DTCOriginIdx];

      Result = DEM_NO_SUCH_ELEMENT;

      /*
       * ENTER critical section to prevent interruption of reading of
       *   two bytes of 'EventId'
       */
      /* !LINKSTO dsn.Dem.Dem_DcmGetNextFilteredStoredDTC.ExclusiveArea,1 */
      DEM_ENTER_EXCLUSIVE_AREA();

      /* search for event memory entry, which contains the next record */
      while ( (Dem_StoredDTCFilter.NextEntryIdx < SizeEventMem) &&
              (Result == DEM_NO_SUCH_ELEMENT)
            )
      {
        /* the access to EventMem[].EventId is protected
         * by critical section above */
        EventId = EventMem[Dem_StoredDTCFilter.NextEntryIdx].EventId;

        /* check if event memory entry is valid */
        if (EventId != DEM_EVENT_ID_INVALID)
        {
          StoredDTC = Dem_GetVisibleUdsDTC(EventId);

          /* check if DTC is assigned to event */
          if (StoredDTC != DEM_NO_DTC)
          {
            if (Dem_StoredDTCFilter.DataKind == DEM_DATAKIND_FREEZEFRAME)
            {
#if (DEM_COMMON_FREEZEFRAMES_USED == STD_ON)
              if (Dem_CommonPartFFClassIdx[DEM_REGULAR_FF] != DEM_FFCLS_NULL_IDX)
              {
                /* next matching DTC found */
                Result = E_OK;
              }
              else
#endif
              {
                if (Dem_GbiFFClassIdx(EventId) != DEM_FFCLS_NULL_IDX)
                {
                  /* next matching DTC found */
                  Result = E_OK;
                }
              }
            }
            else /* Dem_StoredDTCFilter.DataKind == DEM_DATAKIND_EXTENDEDDATA */
            /* Note: No additional check case needed, because
             * it is checked (DET) already in Dem_DcmSetStoredDTCFilter */
            {
              if (Dem_GbiEDClassIdx(EventId) != DEM_EDCLS_NULL_IDX)
              {
                /* next matching DTC found */
                Result = E_OK;
              }
            }
          }
        }

        /* next event in memory */
        ++Dem_StoredDTCFilter.NextEntryIdx;
      }

      /*
       * LEAVE critical section
       */
      DEM_EXIT_EXCLUSIVE_AREA();

      if (Result == E_OK)
      {
        *DTC = StoredDTC;
      }
#if (DEM_NUMBER_OF_CLIENTS > 1U)
      else
      {
        /* mark that read operation finished */
        Dem_ReadStoredDTCOpDone = TRUE;
      }
#endif
    }
  }

  DBG_DEM_DCMGETNEXTFILTEREDSTOREDDTC_EXIT(ClientId, Result, DTC);
  return Result;
}

#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>

/*==================[internal function definitions]=========================*/

#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

STATIC FUNC(boolean, DEM_CODE) Dem_ApplyDTCFilter(
  uint8                                                          ClientId,
  Dem_EventIdType                                                EventId,
  P2VAR(Dem_DTCStatusMaskType, AUTOMATIC, DEM_APPL_DATA)         DTCStatus,
  P2VAR(sint8, AUTOMATIC, DEM_APPL_DATA)                         FDC,
  P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA)                        DTC)
{
  boolean Result = FALSE;
  CONSTP2VAR(Dem_DTCFilterType, AUTOMATIC, DEM_VAR_CLEARED) ClientDTCFilter =
      &Dem_DTCFilter[ClientId];
  DBG_DEM_APPLYDTCFILTER_ENTRY(ClientId, EventId, DTCStatus, FDC, DTC);

  *DTC = Dem_GetVisibleCmbDTC(ClientDTCFilter->DTCFormat, EventId);
  /* check if a DTC is associated with the given event */
  if (*DTC != DEM_NO_DTC)
  {
    /* !LINKSTO Dem_OBD_0001,2 */
#if (DEM_OBD_Support == STD_ON)
    const Dem_DTCKindType DTCKind = Dem_GbiDTCKind(EventId);

    /* check if no DTC kind was filtered (DEM_DTC_KIND_ALL_DTCS)
     * or if kind filter matches */
    if ( (ClientDTCFilter->DTCKind == DEM_DTC_KIND_ALL_DTCS) ||
         (ClientDTCFilter->DTCKind == DTCKind) )
#endif
    {
      /* access DTC status only once centrally to ensure consistent value for
       * that filter services, which return this value */
#if (DEM_NUM_CMB_DTCS > 0U)
      boolean CmbEvent = FALSE;

      if (DEM_IS_EVENT_COMBINED(EventId))
      {
        CmbEvent = TRUE;
        *DTCStatus = Dem_CmbDTCStatus[Dem_GbiDTCConfIdx(EventId)];
      }
      else
#endif /* (DEM_NUM_CMB_DTCS > 0U) */
      {
        *DTCStatus = DEM_DTC_STATUS_MASKS[EventId];
      }

      /* apply all filter-criteria (Dem SWS 8.3.4.1.1) */

      /* filter for DTCStatusMask */
      if ( (ClientDTCFilter->DTCStatusMask == 0U) ||
           ( (ClientDTCFilter->DTCStatusMask & *DTCStatus) != 0U)
         )
      {
        /* filter for DTCOrigin */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U)
        if ( (ClientDTCFilter->DTCOrigin == Dem_GbiDTCOrigin(EventId)) ||
             /* !LINKSTO Dem_OBD_0005,1 */
             (ClientDTCFilter->DTCOrigin == DEM_DTC_ORIGIN_PERMANENT_MEMORY) )
#else
        if (ClientDTCFilter->DTCOrigin == Dem_GbiDTCOrigin(EventId))
#endif
        {
          Dem_DTCSeverityType DTCSeverity = 0U;

          /* check severity only if this condition is fulfilled, else next check is true,
           * no matter of severity. */
          if (ClientDTCFilter->FilterWithSeverity == TRUE)
          {
            DTCSeverity = Dem_GbiDTCSeverity(EventId);
          }

          /* filter for DTCSeverity as mask */
          if ( (ClientDTCFilter->FilterWithSeverity == FALSE) ||
                 /* to handle Severity filter inactive */
               ( (ClientDTCFilter->DTCSeverityMask == DTCSeverity) ||
                 ( (ClientDTCFilter->DTCSeverityMask & DTCSeverity) != 0U )
               )
             )
          {
            /* filter for fault detection counter */
            if (ClientDTCFilter->FilterForFDC == FALSE)
            {
              /* filter-criteria matched to event */
              Result = TRUE;
            }
            else
            {
              Std_ReturnType Ret = E_NOT_OK;

#if (DEM_NUM_CMB_DTCS > 0U)
              if (CmbEvent == TRUE)
              {
                /* if event is combined get the FDC of the combined DTC */
                Ret = Dem_GetFDCOfCmbDTC(EventId, FDC);
              }
              else
#endif
              {
#if (DEM_USE_CB_GET_FDC == STD_OFF)
                if (Dem_DebounceFctPtrTable[Dem_GbiDebounceAlgo(EventId)].
                      GetFDCFct != NULL_PTR)
#endif
                {
                  Ret = Dem_DebounceFctPtrTable[Dem_GbiDebounceAlgo(EventId)].
                    GetFDCFct(EventId, FDC);
                }
              }

              /* fault detection counter filter criteria:
               * FDC must between 1 and 0x7E (inclusive) - this means all
               * DTCs where debouncing is in PREFAILED (but not yet FAILED)
               * stage will be returned by the filter.
               * (Dem SWS 8.3.4.1.1, ISO 14229-1:2006 Table 258) */

              /* note that sint8 is signed! */
              if ( (Ret == E_OK) && ( (1 <= *FDC) && (*FDC <= 126) ) )
              {
                /* filter-criteria matched to event */
                Result = TRUE;
              }
            } /* FDC */
          } /* DTCSeverity */
        } /* DTCOrigin */
      } /* DTCStatus */
    } /* DTC-Kind*/
  } /* DTC and indirect DTCFormat */

  DBG_DEM_APPLYDTCFILTER_EXIT(Result, ClientId, EventId, DTCStatus, FDC, DTC);
  return Result;
}

#if (DEM_ORDERED_DTC_REPORTING_USED == STD_ON)
/* !LINKSTO Dem410_Implicit1,1 */
/* !LINKSTO dsn.Dem.ReportingOrderDTC.DtcOrderImage,1 */
STATIC FUNC(void, DEM_CODE) Dem_ApplyDTCOrder(uint8 ClientId,
                                              Dem_DTCOriginType DTCOrigin)
{
  const Dem_EventMemoryEntryPtrType EvMemEntry = Dem_EventMem[DTCOrigin - 1U];
  const Dem_SizeEvMemEntryType SizeEventMem    = Dem_SizeEventMem[DTCOrigin - 1U];

  /* this array is necessary temporary to store the OccOrder information of */
  /* every event in the DTCOrigin. The OccOrder is used later as criteria   */
  /* for sorting the events in the global array */
  Dem_OccOrderType EventMemOccOrder[DEM_MAX_NUM_ORDERED_EVENT_IDS];

  Dem_OccOrderType SwapOcc;
  Dem_EventIdType  SwapEvId;
  Dem_SizeEvMemEntryType EvIdx1;
  Dem_SizeEvMemEntryType EvIdx2;
  /* to reduce effort of resolving the address inside the critical section */
  CONSTP2VAR(uint8, AUTOMATIC, DEM_VAR_CLEARED) EventsInMemoryPtr =
    &(Dem_DTCFilter[ClientId].EventsInMemory);
  CONSTP2VAR(Dem_EventIdType, AUTOMATIC, DEM_VAR_CLEARED) ClientOrderedEventId =
    &Dem_OrderedEventId[ClientId][0U];

  DBG_DEM_APPLY_DTC_ORDER_ENTRY(ClientId, DTCOrigin);

  /* due to the preprocessing of Dem_SizeEventMem[DTCOrigin-1]
   * in the calling API function Dem_SetDtcFilter() SizeEventMem must be > 0 */
  DEM_PRECONDITION_ASSERT(SizeEventMem != 0U, DEM_INTERNAL_API_ID);

  /*
   * ENTER critical section to prevent interruption of determining current ordered
   * DTC output
   */
  /* !LINKSTO dsn.Dem.ReportingOrderDTC.SetDtcFilter.CriticalSection,1 */
  DEM_ENTER_EXCLUSIVE_AREA();

  /* reset the number of events in memory */
  *(EventsInMemoryPtr) = 0U;

  /* copy relevant stored event mem infos to global event id array and
   * temporary occurrence order array */
  /* events in array need not to be set because they will not be accessed
   * since the EventsInMemory member variable controls access to the array */
  for (EvIdx1 = 0U; EvIdx1 < SizeEventMem; ++EvIdx1)
  {
    ClientOrderedEventId[EvIdx1] = EvMemEntry[EvIdx1].EventId;

    if (ClientOrderedEventId[EvIdx1] != DEM_EVENT_ID_INVALID)
    {
      EventMemOccOrder[EvIdx1] = EvMemEntry[EvIdx1].OccOrder;
      *(EventsInMemoryPtr) += 1U;
    }
    else
    {
      /* set element to the lowest value to be sorted at the end
       * of the list in the algorithm below */
      EventMemOccOrder[EvIdx1] = 0U;
    }
  }

  /*
   * LEAVE critical section
   */
  DEM_EXIT_EXCLUSIVE_AREA();

  /* bubble sort to order event IDs in descending order of OccOrder */
  for (EvIdx1 = 0U; EvIdx1 < (SizeEventMem - 1U); ++EvIdx1)
  {
    for (EvIdx2 = 0U; EvIdx2 < (SizeEventMem - EvIdx1 - 1U); ++EvIdx2)
    {
      if (EventMemOccOrder[EvIdx2] < EventMemOccOrder[EvIdx2 + 1U])
      {
        SwapOcc = EventMemOccOrder[EvIdx2];
        SwapEvId = ClientOrderedEventId[EvIdx2];

        EventMemOccOrder[EvIdx2]   = EventMemOccOrder[EvIdx2 + 1U];
        ClientOrderedEventId[EvIdx2] = ClientOrderedEventId[EvIdx2 + 1U];

        EventMemOccOrder[EvIdx2 + 1U]   = SwapOcc;
        ClientOrderedEventId[EvIdx2 + 1U] = SwapEvId;
      }
    }
  }

  DBG_DEM_APPLY_DTC_ORDER_EXIT(ClientId, DTCOrigin);
}
#endif /* (DEM_ORDERED_DTC_REPORTING_USED == STD_ON) */

#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>

/*==================[end of file]===========================================*/
