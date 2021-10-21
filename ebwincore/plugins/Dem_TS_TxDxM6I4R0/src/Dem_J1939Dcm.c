/* --------{ EB Automotive C Source File }-------- */

/* This file contains definitions for interface functions provided by Dem to
 * the J1939Dcm and the general internal functions needed for the J1939Dcm
 * inteface and J1939 extensin core functionalites. */

/* MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 10.8 (required)
 *    "The value of a composite expression shall not be cast to a different essential
 *     type category or a wider essential type"
 *
 *  Reason:
 *    Needed to simplify the implementation.
 *
 *  MISRAC2012-2) Deviated Rule: 13.5 (required)
 *    "The right hand operand of a logical && or || operator shall not contain persistent
 *     side effects."
 *
 *  Reason:
 *    Required for the implementation.
 *
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason:
 * The compiler optimizes the 'for' loop (which has only 1 iteration) in the following way:
 * - it prepares in advance the index for the next iteration
 * - if the index is equal to '0' it goes back to check the 'for' loop conditions, otherwise it just moves on
 * - since the index is already equal to '1', it never jumps back to check the exit condition
 */

/*==================[inclusions]============================================*/
#include <TSAutosar.h>                        /* EB specific standard types */
#include <Std_Types.h>                            /* AUTOSAR standard types */

#include <Dem_Int.h>             /* Module public and internal declarations */
#include <Dem_Trace.h>                        /* Dbg related macros for Dem */
/*==================[macros]================================================*/
#define DEM_J1939_BYTE_MAX 250

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

#if (DEM_J1939_SUPPORT == STD_ON)
/** \brief Function to return the node of the DTC referenced by a specific event
 **
 **
 ** \param[in] EventId  0 < EventId <= DEM_MAX_EVENTID
 ** \param[out] Node
 **
 ** \return uint8 value, the value of the Node
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(uint8, DEM_CODE) Dem_InternalGetJ1939Node(Dem_EventIdType EventId);

#if ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_READINESS1_SUPPORT == STD_ON) || (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON))
/** \brief Function to apply the DTC filter with the given event
 **
 **
 ** \param[in] EventId  0 < EventId <= DEM_MAX_EVENTID
 ** \param[out] J1939Filter
 **
 ** \return Boolean value, if the filter matches the event configuration
 ** \retval TRUE  the filters match
 ** \retval FALSE the filters don't match
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(boolean, DEM_CODE) Dem_ApplyJ1939DTCFilter(
  Dem_EventIdType                                                EventId,
  Dem_J1939DcmDTCStatusFilterType                               J1939Filter
 );
#endif /* (DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_READINESS1_SUPPORT == STD_ON) || (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON) */

#if ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_READINESS1_SUPPORT == STD_ON))
/** \brief Gets the count of  J1939 DTCs.
 *
 * \param[in] DTCStatusFilter      set the filter type
 * \param[in] DTCKind  Defines the functional group of DTCs to be counted
 * \param[in] Node     J1939Nm node Id
 *
 * \return uint8 Returns DTC counts that match the input parameters
 */
STATIC FUNC(uint8, DEM_CODE) Dem_GetNumberJ1939DTCs(
       Dem_J1939DcmDTCStatusFilterType  DTCStatusFilter,
       Dem_DTCKindType                  DTCKind,
       uint8                            Node);
#endif
#if ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_READINESS1_SUPPORT == STD_ON) || (DEM_J1939_FREEZEFRAME_SUPPORT==STD_ON ) || (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON))
/** \brief Get DTC value from Dem_GetVisibleJ1939DTC() if J1939 support is enabled and
 ** if DTCFormat is Dem_GetVisibleJ1939DTC
 **
 ** \param[in] EventId    0 < EventId <= DEM_MAX_EVENTID
 **
 ** \return DEM_NO_DTC if no DTC is assigned to the event or if the DTC is
 **         suppressed, otherwise the DTC value
 */
STATIC FUNC(uint32, DEM_CODE) Dem_GetVisibleJ1939DTC(Dem_EventIdType EventId);

#endif /* ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_READINESS1_SUPPORT == STD_ON) || (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)) */

#if (DEM_J1939_READING_DTC_SUPPORT == STD_ON)
#if (defined DEM_MIL_ID)
/** \brief Internal function for reading the status of MIL indicator in case J1939 is enabled.
 **
 ** This function shall be used to read the MIL indicator-status derived from the
 ** event status as a summary of all assigned events.
 **
 ** \param[in] IndicatorId  Number of indicator
 **
 ** \return Status of the indicator, like on, off, blinking.
 ** \retval DEM_INDICATOR_OFF         Indicator off mode
 ** \retval DEM_INDICATOR_CONTINUOUS  Indicator continuously on mode
 ** \retval DEM_INDICATOR_SLOW_FLASH  Indicator slow flash mode
 ** \retval DEM_INDICATOR_FAST_FLASH  Indicator fast flash mode
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(Dem_IndicatorStatusType, DEM_CODE) Dem_J1939InternalGetMilStatus(void);
#endif /* (defined DEM_MIL_ID)*/

#if ((defined DEM_AWIL_ID) || (defined DEM_RSIL_ID) || (defined DEM_PIL_ID))
/** \brief Internal function for reading the status of Lamp indicators.
 **
 ** This function shall be used to read the Lamp indicator-status derived from the
 ** event status as a summary of all assigned events.
 **
 ** \param[in] IndicatorId  Number of indicator
 **
 ** \return Status of the indicator, like on, off, blinking.
 ** \retval DEM_INDICATOR_OFF         Indicator off mode
 ** \retval DEM_INDICATOR_CONTINUOUS  Indicator continuously on mode
 ** \retval DEM_INDICATOR_SLOW_FLASH  Indicator slow flash mode
 ** \retval DEM_INDICATOR_FAST_FLASH  Indicator fast flash mode
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(Dem_IndicatorStatusType, DEM_CODE) Dem_J1939InternalGetLampStatus(Dem_IndicatorIdType IndicatorId);
#endif /* ((defined DEM_AWIL_ID) || (defined DEM_RSIL_ID) || (defined DEM_PIL_ID))*/

/** \brief Internal function for setting the bytes of the indicator lamps.
 **
 ** This function shall be used to put the lamp status in the structure assigned to it.
 ** event status as a summary of all assigned events.
 **
 ** \param[in] None.
 ** \param[out] LampStatus  The status of the four lamps.
 **
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(void, DEM_CODE) Dem_J1939GetInternalLampStatusBytes(Dem_J1939DcmLampStatusType* LampStatus);
#endif /* (DEM_J1939_READING_DTC_SUPPORT == STD_ON) */

#if (DEM_J1939_READINESS1_SUPPORT == STD_ON)

/** \brief Updates the content of readiness1 for an event
 *
 *  \param[in] EventId         Event to be checked
 *  \param[in] ReadinessGroup  Readiness group to be updated
 *  \param[in] ReadinessGroupChecked  Flag if this group has been checked
 *                                    before
 *  \param[inout]  PID01value  Pointer to the PID01 to be updated
 *
 *  This function updates the given PID01 depending on the other parameters,
 *  e.g. event status.
 *  In case this function is called with calibration support disabled.
 *
 *  \return boolean  Returns if the given readiness group is now checked.
 */
STATIC FUNC(boolean, DEM_CODE) Dem_UpdateJ1939ReadinessValue(
  Dem_EventIdType EventId,
  uint16 ReadinessGroup,
  boolean ReadinessGroupChecked,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID01value);
#endif /* (DEM_J1939_READINESS1_SUPPORT == STD_ON) */

#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
/** \brief Process clearing of all events related to a node and matching the filter
 **
 ** \param[in]  DTCTypeFilter
 ** \param[in]  Node
 **
 ** \return Updated result of clear event process
 */
STATIC FUNC(Dem_ReturnClearDTCType, DEM_CODE) Dem_J1939DcmStartClearDTC(
  Dem_J1939DcmSetClearFilterType   DTCTypeFilter,
  uint8                                     Node);

#endif /* DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON */

#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>
/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
#define DEM_START_SEC_VAR_INIT_UNSPECIFIED
#include <Dem_MemMap.h>
#if (DEM_J1939_READING_DTC_SUPPORT == STD_ON)

/** \brief Status of DTC-filter */
STATIC VAR(Dem_J1939DTCFilterType, DEM_VAR) Dem_J1939DTCFilter =
{
  1U,                            /* index of event to read next */
  DEM_J1939DTC_ACTIVE,           /* J1939 DTC status filter */
  DEM_DTC_KIND_ALL_DTCS,         /* DTC kind */
  DEM_J1939_INVALID_NODE_ID,     /* J1939 NM Node */
};
#endif

#if (DEM_J1939_FREEZEFRAME_SUPPORT == STD_ON)

/** \brief Status of J1939 Freeze Frame Filter */
STATIC VAR(Dem_J1939FreezeFrameFilterType, DEM_VAR) Dem_J1939FreezeFrameFilter =
{
  1U,                            /* index of event to read next */
  DEM_J1939DCM_FREEZEFRAME,      /* Kind of J1939 Freeze Frame */
  DEM_J1939_INVALID_NODE_ID,     /* J1939 NM Node */
  FALSE,                         /* Initial value for validity */
};

#endif
#define DEM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Dem_MemMap.h>

#define DEM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dem_MemMap.h>
#if ((DEM_J1939_SUPPORT == STD_ON) && (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON))
  /** \brief Filter of an asynchronous J1939 ClearDTC processing */
  VAR(Dem_J1939ClearDTCFilterType, DEM_VAR_CLEARED) Dem_J1939ClearDTCFilter;
#endif
#define DEM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dem_MemMap.h>

/*==================[external function definitions]=========================*/
#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

#if (DEM_J1939_READING_DTC_SUPPORT == STD_ON)
/*------------------------[Dem_J1939DcmSetDTCFilter]------------------------*/

FUNC(Dem_ReturnSetFilterType, DEM_CODE) Dem_J1939DcmSetDTCFilter(
  Dem_J1939DcmDTCStatusFilterType                              DTCStatusFilter,
  Dem_DTCKindType                                              DTCKind,
  uint8                                                        node,
  P2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_APPL_DATA) LampStatus)
{
  Dem_ReturnSetFilterType Result = DEM_WRONG_FILTER;
  DBG_DEM_J1939DCMSETDTCFILTER_ENTER(DTCStatusFilter, DTCKind ,node, LampStatus);
#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmSetDTCFilter, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_J1939DcmSetDTCFilter, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
/* !LINKSTO dsn.Dem.J1939.J1939DTCFilter.J1939DcmSetDTCFilter.OBDPermanent,1 */
#if (DEM_OBD_Support == STD_ON)
  else if ((DTCStatusFilter != DEM_J1939DTC_ACTIVE) &&
            (DTCStatusFilter != DEM_J1939DTC_PREVIOUSLY_ACTIVE) &&
            (DTCStatusFilter != DEM_J1939DTC_PENDING) &&
            (DTCStatusFilter != DEM_J1939DTC_CURRENTLY_ACTIVE)
            &&(DTCStatusFilter != DEM_J1939DTC_PERMANENT))
#else
  else if ((DTCStatusFilter != DEM_J1939DTC_ACTIVE) &&
            (DTCStatusFilter != DEM_J1939DTC_PREVIOUSLY_ACTIVE) &&
            (DTCStatusFilter != DEM_J1939DTC_PENDING) &&
            (DTCStatusFilter != DEM_J1939DTC_CURRENTLY_ACTIVE))
#endif
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmSetDTCFilter, DEM_E_PARAM_DATA);
  }
/* !LINKSTO dsn.Dem.J1939.J1939DTCFilter.J1939DcmSetDTCFilter.OBDEmission,1 */
#if (DEM_OBD_Support == STD_ON)
  else if ((DTCKind != DEM_DTC_KIND_ALL_DTCS)
            && (DTCKind != DEM_DTC_KIND_EMISSION_REL_DTCS))
#else
  else if (DTCKind != DEM_DTC_KIND_ALL_DTCS)
#endif
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmSetDTCFilter, DEM_E_PARAM_DATA);
  }
  else if (FALSE == DEM_J1939_NODE_EXIST(node))
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmSetDTCFilter, DEM_E_PARAM_DATA);
  }
  else if (LampStatus == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmSetDTCFilter, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
      /* !LINKSTO Dem.ASR422.SWS_Dem_00855,1 */
      /* !LINKSTO Dem.ASR422.SWS_Dem_00856,1 */
      Dem_J1939DTCFilter.NextItem = 1U;
      Dem_J1939DTCFilter.J1939DTCStatusFilter = DTCStatusFilter;
      Dem_J1939DTCFilter.DTCKind = DTCKind;
      Dem_J1939DTCFilter.Node = node;
      /* !LINKSTO Dem.ASR422.SWS_Dem_00857,1 */
      Dem_J1939GetInternalLampStatusBytes(LampStatus);
      Result = DEM_FILTER_ACCEPTED;
  }

  DBG_DEM_J1939DCMSETDTCFILTER_EXIT(Result, DTCStatusFilter, DTCKind ,node, LampStatus);
  return Result;
}

/*-------------------[Dem_J1939DcmGetNumberOfFilteredDTC]-------------------*/

FUNC(Dem_ReturnGetNumberOfFilteredDTCType, DEM_CODE) Dem_J1939DcmGetNumberOfFilteredDTC(
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) NumberOfFilteredDTC)
{
  Dem_ReturnGetNumberOfFilteredDTCType Result = DEM_NUMBER_FAILED;

  DBG_DEM_J1939DCMGETNUMBEROFFILTEREDDTC_ENTER(NumberOfFilteredDTC);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmGetNumberOfFilteredDTC, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_J1939DcmGetNumberOfFilteredDTC, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (NumberOfFilteredDTC == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmGetNumberOfFilteredDTC, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNumberOfFilteredDTC.NumberFiltered,1 */
    *NumberOfFilteredDTC = Dem_GetNumberJ1939DTCs(Dem_J1939DTCFilter.J1939DTCStatusFilter, Dem_J1939DTCFilter.DTCKind, Dem_J1939DTCFilter.Node);

    /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNumberOfFilteredDTC.Ok,1 */
    Result = DEM_NUMBER_OK;
  }

  DBG_DEM_J1939DCMGETNUMBEROFFILTEREDDTC_EXIT(Result, NumberOfFilteredDTC);

  return Result;
}

/*---------------------[Dem_J1939DcmGetNextFilteredDTC]---------------------*/

FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_J1939DcmGetNextFilteredDTC(
  P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) J1939DTC,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) OccurenceCounter)
{
  /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFilteredDTC.NoMatchingRet,1 */
  Dem_ReturnGetNextFilteredElementType Result = DEM_FILTERED_NO_MATCHING_ELEMENT;
  DBG_DEM_J1939DCMGETNEXTFILTEREDDTC_ENTER(J1939DTC, OccurenceCounter);
#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmGetNextFilteredDTC, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_J1939DcmGetNextFilteredDTC, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if ((J1939DTC == NULL_PTR) || (OccurenceCounter == NULL_PTR))
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmGetNextFilteredDTC, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
      boolean FilterResult = FALSE;
      /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFilteredDTC.NoMatchingOut,1 */
      *J1939DTC = (uint32)DEM_NO_DTC;
      /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFilteredDTC.OccCntNotavl,1 */
      *OccurenceCounter = (uint8)127U;
      /* search for first DTC fulfilling the filter criterion */
      /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFilteredDTC.FilteredDTCs,1 */
      while ( (Dem_J1939DTCFilter.NextItem <= DEM_MAX_EVENTID) &&
              (FilterResult == FALSE)
            )
      {
        Dem_EventIdType NextEventId;
        uint32 GetFilter_J1939Dtc;
        NextEventId = (Dem_EventIdType)Dem_J1939DTCFilter.NextItem;
        GetFilter_J1939Dtc = Dem_GetVisibleJ1939DTC(NextEventId);
        if (GetFilter_J1939Dtc != DEM_NO_DTC)
        {
#if (DEM_OBD_Support == STD_ON)
          const Dem_DTCKindType DTCKind = Dem_GbiDTCKind(NextEventId);
#endif
          Dem_EventMemoryEntryPtrType EvMemEntryPtr;
          /* check if no DTC kind was filtered (DEM_DTC_KIND_ALL_DTCS)
           * or if kind filter matches */
#if (DEM_OBD_Support == STD_ON)
          if ((Dem_J1939DTCFilter.DTCKind == DEM_DTC_KIND_ALL_DTCS) ||
           (Dem_J1939DTCFilter.DTCKind == DTCKind) )
#endif
          {
            uint8 Node = Dem_InternalGetJ1939Node(NextEventId);
            if (Node == Dem_J1939DTCFilter.Node)
            {
              if (Dem_ApplyJ1939DTCFilter(NextEventId, Dem_J1939DTCFilter.J1939DTCStatusFilter) == TRUE)
              {
                FilterResult = TRUE;
                /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFilteredDTC.FilterRetOk,1 */
                Result = DEM_FILTERED_OK;
                /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFilteredDTC.FilterOk,1 */
                *J1939DTC = GetFilter_J1939Dtc;
                /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFilteredDTC.OccCnt,1 */
                if (Dem_SearchForEntry(NextEventId, &EvMemEntryPtr, NULL_PTR) == TRUE)
                {
                  if ((EvMemEntryPtr->OccCtr) >= 126)
                  {
                    *OccurenceCounter = 126;
                  }
                  else
                  {
                    *OccurenceCounter = EvMemEntryPtr->OccCtr;
                  }
                }
              }
            }
          }
        }

        /* update NextItem for next call in filter */
        Dem_J1939DTCFilter.NextItem++;
      }

  }

  DBG_DEM_J1939DCMGETNEXTFILTEREDDTC_EXIT(Result, J1939DTC, OccurenceCounter);
  return Result;
}

#endif /* (DEM_J1939_READING_DTC_SUPPORT == STD_ON) */

#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
/*--------------------------[Dem_J1939DcmClearDTC]--------------------------*/

FUNC(Dem_ReturnClearDTCType, DEM_CODE) Dem_J1939DcmClearDTC(
  Dem_J1939DcmSetClearFilterType   DTCTypeFilter,
  uint8                                     node)
{
  Dem_ReturnClearDTCType Result = DEM_ASR42_CLEAR_FAILED;

  DBG_DEM_J1939DCMCLEARDTC_ENTRY(DTCTypeFilter, node);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmClearDTC, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_J1939DcmClearDTC, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  /* !LINKSTO Dem.ASR42.J1939.SWS_Dem_00878.ClearDtc.delete,1 */
  else if ((DTCTypeFilter != DEM_J1939DTC_CLEAR_ALL) &&
            (DTCTypeFilter != DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE)
          )
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmClearDTC, DEM_E_PARAM_DATA);
  }
  else if (FALSE == DEM_J1939_NODE_EXIST(node))
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmClearDTC, DEM_E_PARAM_DATA);
    Result = DEM_CLEAR_WRONG_DTC;
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* check if an async ClearDTC was started */
    switch(Dem_ClearDtcAsyncStatus.RunState)
    {
      case DEM_CLEARDTC_STATE_PROCESSING_RAM:
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
      case DEM_CLEARDTC_STATE_PROCESSING_NVRAM:
#endif
      {
        if ((DEM_VIRTUAL_CLIENT_J1939 == Dem_ClearDtcAsyncStatus.ClientId) &&
            (Dem_J1939ClearDTCFilter.Dem_J1939Node == node) &&
            (Dem_J1939ClearDTCFilter.Dem_J1939DTCTypeFilter == DTCTypeFilter))
        {
          /* continue with asynchronous event clearing */
          Result = DEM_CLEAR_PENDING;
        }
        else
        {
          /* report Result BUSY */
          /* !LINKSTO SWS_Dem_00662,1, SWS_Dem_00663,1 */
          Result = DEM_CLEAR_BUSY;
        }
        break;
      }
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
      case DEM_CLEARDTC_STATE_FAILED:
#endif
      case DEM_CLEARDTC_STATE_FINISHED:
      {
        if ((DEM_VIRTUAL_CLIENT_J1939 == Dem_ClearDtcAsyncStatus.ClientId) &&
            (Dem_J1939ClearDTCFilter.Dem_J1939Node == node) &&
            (Dem_J1939ClearDTCFilter.Dem_J1939DTCTypeFilter == DTCTypeFilter))
        { /* !LINKSTO dsn.Dem.ClearDTCBehavior.NonvolatileFinish,3 */
          /* set the state back to IDLE */
          Dem_ClearDtcAsyncStatus.RunState = DEM_CLEARDTC_STATE_IDLE;
          /* report Result OK */
          Result = Dem_ClearDtcAsyncStatus.OperationResult;
        }
        else
        {
          /* report Result BUSY */
          /* !LINKSTO SWS_Dem_00662,1 */
          Result = DEM_CLEAR_BUSY;
        }
        break;
      }
      case DEM_CLEARDTC_STATE_IDLE:
      {
        Result = Dem_J1939DcmStartClearDTC(DTCTypeFilter, node);
        break;
      }
      /* CHECK: NOPARSE */
      default:
        DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_J1939DcmClearDTC);
        break;
      /* CHECK: PARSE */
    }
  }

  DBG_DEM_J1939DCMCLEARDTC_EXIT(Result, DTCTypeFilter, node);
  return Result;
}

#endif /*(DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON) */

#if (DEM_J1939_FREEZEFRAME_SUPPORT==STD_ON)

/*-------------------[Dem_J1939DcmSetFreezeFrameFilter]---------------------*/

FUNC(Dem_ReturnSetFilterType, DEM_CODE) Dem_J1939DcmSetFreezeFrameFilter(
    Dem_J1939DcmSetFreezeFrameFilterType FreezeFrameKind,
    uint8                                node)
{
  Dem_ReturnSetFilterType Result = DEM_WRONG_FILTER;

  DBG_DEM_J1939DCMSETFREEZEFRAMEFILTER_ENTRY(FreezeFrameKind, node);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmSetFreezeFrameFilter, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_J1939DcmSetFreezeFrameFilter, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (FALSE == DEM_J1939_NODE_EXIST(node) ||
          (FreezeFrameKind > DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME))
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmSetFreezeFrameFilter, DEM_E_PARAM_DATA);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
      /* !LINKSTO Dem.J1939.Dem_J1939DcmSetFreezeFrameFilter.DemJ1939DcmFF,1 */
      if (FreezeFrameKind == DEM_J1939DCM_FREEZEFRAME)
      {
          /* !LINKSTO Dem.J1939.Dem_J1939DcmSetFreezeFrameFilter.MaskAttributes,1 */
          /* DM04 Freeze Frame Kind */
          Dem_J1939FreezeFrameFilter.NextItem = 1U;
          Dem_J1939FreezeFrameFilter.FreezeFrameKind = FreezeFrameKind;
          Dem_J1939FreezeFrameFilter.Node = node;
          /* !LINKSTO Dem.J1939.Dem_J1939DcmSetFreezeFrameFilter.FilterAccepted_Validity,1 */
          Dem_J1939FreezeFrameFilter.IsFilterValid = TRUE;
          Result = DEM_FILTER_ACCEPTED;
      }
      else
      {
          /* Resetting the filter criteria when unsupported FreezeFrameKind is requested */
          Dem_J1939FreezeFrameFilter.NextItem = 1U;
          Dem_J1939FreezeFrameFilter.FreezeFrameKind = DEM_J1939DCM_FREEZEFRAME;
          Dem_J1939FreezeFrameFilter.Node = DEM_J1939_INVALID_NODE_ID;
          /* !LINKSTO Dem.J1939.Dem_J1939DcmSetFreezeFrameFilter.FilterAccepted_Validity,1 */
          Dem_J1939FreezeFrameFilter.IsFilterValid = FALSE;

          /* DM24 and DM25 are not implemented yet */
          Result = DEM_WRONG_FILTER;
      }

      DBG_DEM_J1939DCMSETFREEZEFRAMEFILTER_EXIT(Result, FreezeFrameKind, node);
  }
  return Result;
}

/*---------------------[Dem_J1939DcmGetNextFreezeFrame]---------------------*/

FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_J1939DcmGetNextFreezeFrame(
    P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) J1939DTC,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  OccurenceCounter,
    P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  DestBuffer,
    P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
{
    /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFreezeFrame.NoMatchedFreezeFrame,1 */
  Dem_ReturnGetNextFilteredElementType Result = DEM_FILTERED_NO_MATCHING_ELEMENT;

  DBG_DEM_J1939DCMGETNEXTFREEZEFRAME_ENTRY(J1939DTC, OccurenceCounter, DestBuffer, BufSize);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmGetNextFreezeFrame, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_J1939DcmGetNextFreezeFrame, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if ((J1939DTC == NULL_PTR) || (OccurenceCounter == NULL_PTR) ||
           (DestBuffer == NULL_PTR) || (BufSize == NULL_PTR))
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmGetNextFreezeFrame, DEM_E_PARAM_POINTER);
  }
  else if (((Dem_J1939FreezeFrameFilter.FreezeFrameKind != DEM_J1939DCM_FREEZEFRAME) &&
            (Dem_J1939FreezeFrameFilter.FreezeFrameKind != DEM_J1939DCM_EXPANDED_FREEZEFRAME)) ||
            (Dem_J1939FreezeFrameFilter.IsFilterValid == FALSE))
  {
    /* !LINKSTO Dem.DetReport.WrongConditionJ1939DcmGetNextFreezeFrame,1 */
    DEM_REPORT_ERROR(DEM_SID_J1939DcmGetNextFreezeFrame, DEM_E_WRONG_CONDITION);
  }
  else
#endif
  {
    /* !LINKSTO Dem.J1939.Dem_J1939DcmSetFreezeFrameFilter.FilterAccepted_Validity,1 */
    if (Dem_J1939FreezeFrameFilter.IsFilterValid != FALSE)
    {
      *J1939DTC = (uint32)DEM_NO_DTC;
      *OccurenceCounter = (uint8)127U;

      /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFreezeFrame.FilteredFreezeFrame,1 */
      /* search for first DTC fulfilling the filter criterion */
#if ( DEM_NUM_J1939FFSPNS > 0U )
      while ( (Dem_J1939FreezeFrameFilter.NextItem <= DEM_MAX_EVENTID) &&
            (Result == DEM_FILTERED_NO_MATCHING_ELEMENT)
          )
#else
      while (Dem_J1939FreezeFrameFilter.NextItem <= DEM_MAX_EVENTID)
#endif /* DEM_NUM_J1939FFSPNS > 0U */
      {
        Dem_EventIdType NextEventId;
#if (DEM_NUM_CMB_DTCS > 0U)
        Dem_EventIdType MasterEventId;
#endif
        uint32 CurrJ1939Dtc;
        NextEventId = (Dem_EventIdType)Dem_J1939FreezeFrameFilter.NextItem;
    #if (DEM_NUM_CMB_DTCS > 0U)
        MasterEventId=DEM_GET_MASTER_EVID(NextEventId);
    #endif
        CurrJ1939Dtc = Dem_GetVisibleJ1939DTC(NextEventId);
        /* check if current event has a configured J1939 DTC and, it is a master in case of combined DTC */
#if (DEM_NUM_CMB_DTCS > 0U)
        if ((CurrJ1939Dtc != DEM_NO_DTC) && (NextEventId == MasterEventId))
#else
        if (CurrJ1939Dtc != DEM_NO_DTC)
#endif
        {
          uint8 Node = Dem_InternalGetJ1939Node(NextEventId);
          /* check if current event belong to requested node, and J1939 Freeze frame requested */
          if ((Node == Dem_J1939FreezeFrameFilter.Node) && (Dem_J1939FreezeFrameFilter.FreezeFrameKind == DEM_J1939DCM_FREEZEFRAME) )
          {
#if ( DEM_NUM_J1939FFSPNS > 0U )
            Dem_EventMemoryEntryPtrType EvMemEntryPtr;
            /* retrieve J1939 FF data */
            Result = Dem_CollectJ1939FFInfo(NextEventId, &EvMemEntryPtr,BufSize, DestBuffer);

            if ( Result == DEM_FILTERED_OK)
            { /* data retrieved successfully */
              /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFreezeFrame.MatchingDTC,1 */
              *J1939DTC = CurrJ1939Dtc;
              /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFreezeFrame.MatchingOC,1 */
              if ((EvMemEntryPtr->OccCtr) >= 126)
              {
                /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFreezeFrame.OCCNotVal,1 */
                *OccurenceCounter = 126;
              }
              else
              {
                /* !LINKSTO Dem.J1939.Dem_J1939DcmGetNextFreezeFrame.MatchingDTC,1 */
                *OccurenceCounter = EvMemEntryPtr->OccCtr;
              }
            }
            /* DEM_FILTERED_PENDING case not implemented as function call is synchronous,
             * and J1939Dcm handle one request at a time */
#endif /* DEM_NUM_J1939FFSPNS > 0U */
          }
        }

        /* update NextItem for next call in filter */
        Dem_J1939FreezeFrameFilter.NextItem++;
      }
    }
    /* Result remains DEM_FILTERED_NO_MATCHING_ELEMENT because
     * FreezeFrameKind not valid */
  }
#if ((DEM_NUM_J1939FFSPNS == 0U) && (DEM_DEV_ERROR_DETECT == STD_OFF))
    TS_PARAM_UNUSED(DestBuffer);
    TS_PARAM_UNUSED(BufSize);
#endif

  DBG_DEM_J1939DCMGETNEXTFREEZEFRAME_EXIT(Result, J1939DTC, OccurenceCounter, DestBuffer, BufSize);
  return Result;
}
#endif /*(DEM_J1939_FREEZEFRAME_SUPPORT == STD_ON) */


#if (DEM_J1939_RATIO_SUPPORT == STD_ON)
/*-----------------------[Dem_J1939DcmSetRatioFilter]-----------------------*/
FUNC(Dem_ReturnSetFilterType, DEM_CODE) Dem_J1939DcmSetRatioFilter(
                                                   P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) IgnitionCycleCounter,
                                                   P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) OBDMonitoringConditionsEncountered,
                                                   uint8 node)
{
  Dem_ReturnSetFilterType Result = DEM_WRONG_FILTER;

  DBG_DEM_J1939DCMSETRATIOFILTER_ENTRY(IgnitionCycleCounter, OBDMonitoringConditionsEncountered, node);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmSetRatioFilter, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_J1939DcmSetRatioFilter, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if ((IgnitionCycleCounter == NULL_PTR) || (OBDMonitoringConditionsEncountered == NULL_PTR))
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmSetRatioFilter, DEM_E_PARAM_POINTER);
  }
  else if (FALSE == DEM_J1939_NODE_EXIST(node))
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmSetRatioFilter, DEM_E_PARAM_DATA);
  }
  else
#endif
  {
    /* API not yet implemented */
    TS_PARAM_UNUSED(IgnitionCycleCounter);
    TS_PARAM_UNUSED(OBDMonitoringConditionsEncountered);
    TS_PARAM_UNUSED(node);
  }

  DBG_DEM_J1939DCMSETRATIOFILTER_EXIT(Result, IgnitionCycleCounter, OBDMonitoringConditionsEncountered, node);

  return Result;
}

/*--------------------[Dem_J1939DcmGetNextFilteredRatio]--------------------*/
FUNC(Dem_ReturnGetNextFilteredElementType, DEM_CODE) Dem_J1939DcmGetNextFilteredRatio(
                                                              P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) SPN,
                                                              P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Numerator,
                                                              P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Denominator)
{
  Dem_ReturnGetNextFilteredElementType Result = DEM_FILTERED_NO_MATCHING_ELEMENT;

  DBG_DEM_J1939DCMGETNEXTFILTEREDRATIO_ENTRY(SPN, Numerator, Denominator);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmGetNextFilteredRatio, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_J1939DcmGetNextFilteredRatio, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if ((SPN == NULL_PTR) || (Numerator == NULL_PTR) || (Denominator == NULL_PTR))
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmGetNextFilteredRatio, DEM_E_PARAM_POINTER);
  }
  else
#endif
  {
    /* API not yet implemented */
    TS_PARAM_UNUSED(SPN);
    TS_PARAM_UNUSED(Numerator);
    TS_PARAM_UNUSED(Denominator);
  }

  DBG_DEM_J1939DCMGETNEXTFILTEREDRATIO_EXIT(Result, SPN, Numerator, Denominator);

  return Result;
}
#endif /* (DEM_J1939_RATIO_SUPPORT == STD_ON) */

#if (DEM_J1939_READINESS1_SUPPORT == STD_ON)
/*------------------[Dem_J1939DcmReadDiagnosticReadiness1]------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_J1939DcmReadDiagnosticReadiness1(
P2VAR(Dem_J1939DcmDiagnosticReadiness1Type, AUTOMATIC, AUTOMATIC) DataValue,
uint8 node
)
{
  Std_ReturnType Result = E_NOT_OK;
  /* array to store the info if at least one event in a group is confirmed */
  boolean ReadinessGroupChecked[DEM_OBD_RDY_PID01PID41_CNT];
  Dem_EventOBDReadinessGroupType ReadinessGroup;
  Dem_EventIdType EventId;
  uint32 J1939Dtc;
  uint8 EventNode;
  /* The variable PID01 contains the same order of the bits of OBD PID01 */
  uint8 PID01[3] = {0x00U};
#if ( (DEM_GENERAL_OBD == STD_ON) && (DEM_OBD_Support_Kind != DEM_OBD_NO_OBD_SUPPORT) )
#if (DEM_OBD_ENGINE_TYPE == DEM_IGNITION_COMPRESSION)
  uint8 loopIndex;
  uint8 mask = 0x01;
  uint16 PID01_map[2] = {0x0000U};
#endif
#endif /* DEM_GENERAL_OBD == STD_ON && DEM_OBD_Support_Kind != DEM_OBD_NO_OBD_SUPPORT */
#if (DEM_MAX_EVENTS_PER_READINESS_GROUP > 0U)
    uint8 RdyGrpIdx;
#endif

  DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS1_ENTRY(DataValue, node);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmReadDiagnosticReadiness1, DEM_E_UNINIT);
  }
  else if (FALSE == DEM_J1939_NODE_EXIST(node))
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmReadDiagnosticReadiness1, DEM_E_PARAM_DATA);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_J1939DcmReadDiagnosticReadiness1, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (DataValue == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmReadDiagnosticReadiness1, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* !LINKSTO Dem.ASR42.J1939.SWS_Dem_00909.ActiveTroubleCodes,1 */
    DataValue->ActiveTroubleCodes = Dem_GetNumberJ1939DTCs(DEM_J1939DTC_ACTIVE, DEM_DTC_KIND_ALL_DTCS, node);
    /* !LINKSTO Dem.ASR42.J1939.SWS_Dem_00909.PreviouslyActiveTroubleCodes,1 */
    DataValue->PreviouslyActiveDiagnosticTroubleCodes = Dem_GetNumberJ1939DTCs(DEM_J1939DTC_PREVIOUSLY_ACTIVE, DEM_DTC_KIND_ALL_DTCS, node);
    /* !LINKSTO Dem.ASR42.J1939.SWS_Dem_00909.OBD_Compliance,1 */
    DataValue->OBDCompliance=Dem_ObdCompliancyPID1CByte;


    /* mark all ReadinessGroups as not checked */
    for (ReadinessGroup = 0U;
         ReadinessGroup < DEM_OBD_RDY_PID01PID41_CNT;
         ReadinessGroup++)
    {
      ReadinessGroupChecked[ReadinessGroup] = FALSE;
    }
    /* !LINKSTO Dem.ASR42.J1939.SWS_Dem_00909.MonitoredSystems.calculation,1 */
    /* !LINKSTO Dem355_Implicit1,1 */
    /* Calculate Readiness Status of Groups */
#if (DEM_MAX_EVENTS_PER_READINESS_GROUP == 0U)
    for (EventId = 1U; EventId <= DEM_MAX_EVENTID; ++EventId)
    {
      EventNode = Dem_InternalGetJ1939Node(EventId);
      J1939Dtc = Dem_GetVisibleJ1939DTC(EventId);
         /* Check if the node is the requested node */
        if ((J1939Dtc != DEM_NO_DTC) && (EventNode == node))
        {
          /* get the readiness group of the event */
          ReadinessGroup = Dem_GbiOBDReadinessGroup(EventId);
          if (ReadinessGroup < DEM_OBD_RDY_PID01PID41_CNT)
          {
            if (Dem_UpdateJ1939ReadinessValue(EventId,
                                     ReadinessGroup,
                                     ReadinessGroupChecked[ReadinessGroup],
                                     PID01) == TRUE)
            {
              ReadinessGroupChecked[ReadinessGroup] = TRUE;
            }
          }
        } /* else: Node not requested so it shall be skipped */
    }
#else
    for (RdyGrpIdx = 0U; RdyGrpIdx < DEM_NUM_OBD_RDY_GROUPS; RdyGrpIdx++)
    {
      uint8 EvtIdx;

      /* get the current readiness group */
      ReadinessGroup = Dem_OBDReadinessGroupMap[RdyGrpIdx];

      if (ReadinessGroup < DEM_OBD_RDY_PID01PID41_CNT)
      {
        /* Deviation TASKING-1 */
        for (EvtIdx = 0U; EvtIdx < DEM_MAX_EVENTS_PER_READINESS_GROUP; EvtIdx++)
        {
          EventId = Dem_OBDReadinessGroup[RdyGrpIdx][EvtIdx];

          if (EventId == DEM_EVENT_ID_INVALID)
          {
            /* end of list of events; stop searching this readiness group */
            break;
          }
          EventNode = Dem_InternalGetJ1939Node(EventId);
          J1939Dtc = Dem_GetVisibleJ1939DTC(EventId);
          /* Check if the node is the requested node */
          if ((J1939Dtc != DEM_NO_DTC) && (EventNode == node) )
          {
            if (Dem_UpdateJ1939ReadinessValue(EventId,
                                     ReadinessGroup,
                                     ReadinessGroupChecked[ReadinessGroup],
                                     PID01) == TRUE)
            {
              ReadinessGroupChecked[ReadinessGroup] = TRUE;
            }
          } /* else: Node not requested so it shall be skipped */
        }
      }
    }

#endif /* (DEM_MAX_EVENTS_PER_READINESS_GROUP == 0U) */

    /* !LINKSTO dsn.Dem.OBDPidSupport.Dem_ReadinessForceStatus_02,1 */
    /* force specific complete bits depend on config */
    PID01[0] &= Dem_ObdReadinessForceCompleteMask;

    /* copy content of the PID01 into the destination buffer DataValue */
    DataValue->ContinuouslyMonitoredSystemsSupport_Status = (uint8)PID01[0];
    /* !LINKSTO Dem.ASR42.J1939.SWS_Dem_00909.MonitoredSystems.TestComplete,1 */
    /* !LINKSTO Dem.ASR42.J1939.SWS_Dem_00909.MonitoredSystems.SupportBit,1 */
#if ( (DEM_GENERAL_OBD == STD_ON) && (DEM_OBD_Support_Kind != DEM_OBD_NO_OBD_SUPPORT) )
#if (DEM_OBD_ENGINE_TYPE == DEM_IGNITION_SPARK)
    DataValue->NonContinuouslyMonitoredSystemsSupport= ( ((uint16)0x00FF) & ((uint16)PID01[1]) );
    DataValue->NonContinuouslyMonitoredSystemsStatus = ( ((uint16)0x00FF) & ((uint16)PID01[2]) );
#else /* DEM_OBD_ENGINE_TYPE == DEM_IGNITION_COMPRESSION */
    for(loopIndex=0; loopIndex<8; loopIndex++)
    {
      if (Dem_J1939ReadinessGroupCompressionMap[loopIndex] != DEM_OBD_J1939_RDY_NONE)
      {
        /* Deviation MISRAC2012-1 <+2> */
        PID01_map[0]= PID01_map[0] | ((uint16)(((PID01[1] & mask))<<(Dem_J1939ReadinessGroupCompressionMap[loopIndex]-loopIndex)));
        PID01_map[1]= PID01_map[1] | ((uint16)(((PID01[2] & mask))<<(Dem_J1939ReadinessGroupCompressionMap[loopIndex]-loopIndex)));
      }
      mask=mask<<1;

    }
    DataValue->NonContinuouslyMonitoredSystemsSupport= PID01_map[0];
    DataValue->NonContinuouslyMonitoredSystemsStatus = PID01_map[1];
#endif
#endif /* DEM_GENERAL_OBD == STD_ON && DEM_OBD_Support_Kind != DEM_OBD_NO_OBD_SUPPORT */

    Result = E_OK;
  }

  DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS1_EXIT(Result, DataValue, node);
  return Result;
}
#endif /* (DEM_J1939_READINESS1_SUPPORT == 0U) */

#if (DEM_J1939_READINESS2_SUPPORT == STD_ON)
/*------------------[Dem_J1939DcmReadDiagnosticReadiness2]------------------*/
FUNC(Std_ReturnType, DEM_CODE) Dem_J1939DcmReadDiagnosticReadiness2(
                                                  P2VAR(Dem_J1939DcmDiagnosticReadiness2Type, AUTOMATIC, DEM_APPL_DATA) DataValue,
                                                  uint8 node
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS2_ENTRY(DataValue, node);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmReadDiagnosticReadiness2, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_J1939DcmReadDiagnosticReadiness2, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (DataValue == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmReadDiagnosticReadiness2, DEM_E_PARAM_POINTER);
  }
  else if (FALSE == DEM_J1939_NODE_EXIST(node))
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmReadDiagnosticReadiness2, DEM_E_PARAM_DATA);
  }
  else
#endif
  {
    /* API not yet implemented */
    TS_PARAM_UNUSED(DataValue);
    TS_PARAM_UNUSED(node);
  }

  DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS2_EXIT(Result, DataValue, node);

  return Result;
}
#endif /* (DEM_J1939_READINESS2_SUPPORT == STD_ON) */

#if (DEM_J1939_READINESS3_SUPPORT == STD_ON)
/*------------------[Dem_J1939DcmReadDiagnosticReadiness3]------------------*/
FUNC(Std_ReturnType, DEM_CODE) Dem_J1939DcmReadDiagnosticReadiness3(
                                                  P2VAR(Dem_J1939DcmDiagnosticReadiness3Type, AUTOMATIC, DEM_APPL_DATA) DataValue,
                                                  uint8 node
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS3_ENTRY(DataValue, node);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmReadDiagnosticReadiness3, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_J1939DcmReadDiagnosticReadiness3, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (DataValue == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmReadDiagnosticReadiness3, DEM_E_PARAM_POINTER);
  }
  else if (FALSE == DEM_J1939_NODE_EXIST(node))
  {
    DEM_REPORT_ERROR(DEM_SID_J1939DcmReadDiagnosticReadiness3, DEM_E_PARAM_DATA);
  }
  else
#endif
  {
    /* API not yet implemented */
    TS_PARAM_UNUSED(DataValue);
    TS_PARAM_UNUSED(node);
  }

  DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS3_EXIT(Result, DataValue, node);

  return Result;
}
#endif /* (DEM_J1939_READINESS3_SUPPORT == STD_ON) */

#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>

/*==================[internal function definitions]=========================*/
#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

#if (DEM_J1939_READING_DTC_SUPPORT == STD_ON)

/*----------------------[Dem_J1939InternalGetMilStatus]---------------------*/
#if (defined DEM_MIL_ID)
STATIC FUNC(Dem_IndicatorStatusType, DEM_CODE) Dem_J1939InternalGetMilStatus(void)
{
  Dem_IndicatorIdType IndicatorId = DEM_MIL_ID;
  Dem_IndicatorStatusType Status = DEM_INDICATOR_OFF;

  DBG_DEM_J1939INTERNALGETMILSTATUS_ENTER();
#if (DEM_INDICATORCLASS_SUPPORT == STD_ON)
  {
    Dem_EventIdType EventId;

    for (EventId = 1U; EventId <= DEM_MAX_EVENTID; EventId++)
    {
#if (DEM_EVENT_DISABLING_SUPPORT == STD_ON)
      if (Dem_GbiEvAvailable(EventId) == FALSE)
      {
        /* event is disabled so the getting of status shall be skipped */
      }
      else
#endif
      {
        const Dem_IndicatorGrpIdxType IndicatorClassIdx =
          Dem_GbiIndicatorClassIndex(EventId);

        /* check for valid indicator index and prevent calibration error */
        if (IndicatorClassIdx < DEM_INDICATOR_UNUSED)
        {
          const Dem_IndicatorIdxType IndicatorClassIdxStart =
            Dem_IndicatorGroups[IndicatorClassIdx];
          const Dem_IndicatorIdxType IndicatorClassIdxEnd =
            Dem_IndicatorGroups[IndicatorClassIdx + 1U];
          Dem_IndicatorIdxType Idx;

          for (Idx = IndicatorClassIdxStart; Idx < IndicatorClassIdxEnd; Idx++)
          {
            CONSTP2CONST(Dem_IndicatorClassType, AUTOMATIC, DEM_CONST) IndicatorClass =
              &Dem_IndicatorClasses[Idx];

            if ((IndicatorClass->IndicatorId == IndicatorId) &&
                 (IndicatorClass->Enabled == TRUE) )
            {
              if (DEM_IS_BIT_SET_IN_ARRAY(
                     DEM_INDICATOR_CYC_COUNTER_WIR_STATUS[IndicatorClass->IndicatorId],
                     EventId) )
              {
                Dem_IndicatorStatusType Status_Temp;
                switch(IndicatorClass->Behaviour)
                {
                    case DEM_INDICATOR_BLINK_CONT:
                    {
                        Status_Temp = DEM_INDICATOR_FAST_FLASH;
                        break;
                    }
                    case DEM_INDICATOR_BLINKING:
                    {
                        Status_Temp = DEM_INDICATOR_SLOW_FLASH;
                        break;
                    }
                    default:
                    {
                        Status_Temp = IndicatorClass->Behaviour;
                        break;
                    }
                }
                if (Status == DEM_INDICATOR_OFF)
                {
                  /* set initial behaviour */
                  Status = Status_Temp;
                }
                else
                {
                   /* !LINKSTO Dem.ASR422.SWS_Dem_00859,1 */
                   /* !LINKSTO Dem.ASR422.SWS_Dem_00860,1 */
                   /* !LINKSTO Dem.ASR422.SWS_Dem_00861,1 */
                   if (Status_Temp > Status)
                   {
                     /* as different behaviours are requested, SW-C is responsible
                      * for behaviour selection */
                     Status = Status_Temp;
                   }
                }
              }
              /* else: no linked event requests its warning indicator
               * -> status remains DEM_INDICATOR_OFF */
              /* !LINKSTO Dem.ASR422.SWS_Dem_00858,1 */
            }
          }
        }
      }
    }
  }
/* calibration is disabled and event availability is not considered */
#else
  {
    P2CONST(Dem_IndicatorLinkType, AUTOMATIC, DEM_CONST) IndicatorLink;
    Dem_IndicatorIdxType LinkIdx;

    /* point to first indicator link of according data structures */
    IndicatorLink = &Dem_IndicatorLink[Dem_IndicatorDesc[IndicatorId].StartIdx];

    for (LinkIdx = 0U; LinkIdx < Dem_IndicatorDesc[IndicatorId].Number; ++LinkIdx)
    {
      const Dem_CounterDataIdxType CounterDataIdx =
        LinkIdx + Dem_IndicatorDesc[IndicatorId].StartIdx;

      if (DEM_IS_BIT_SET_IN_ARRAY(
             DEM_INDICATOR_CYC_COUNTER_WIR_STATUS[DEM_INDICATOR_DEFAULT_COUNTER],
             CounterDataIdx)
         )
      {
        Dem_IndicatorStatusType Status_Temp;
        switch(IndicatorLink[LinkIdx].Behaviour)
        {
            case DEM_INDICATOR_BLINK_CONT:
            {
                Status_Temp = DEM_INDICATOR_FAST_FLASH;
                break;
            }
            case DEM_INDICATOR_BLINKING:
            {
                Status_Temp = DEM_INDICATOR_SLOW_FLASH;
                break;
            }
            default:
            {
                Status_Temp = IndicatorLink[LinkIdx].Behaviour;
                break;
            }
        }
        if (Status == DEM_INDICATOR_OFF)
        {
          /* set initial behaviour */
          Status = Status_Temp;
        }
        else
        {
          /* !LINKSTO Dem.ASR422.SWS_Dem_00859,1 */
          /* !LINKSTO Dem.ASR422.SWS_Dem_00860,1 */
          /* !LINKSTO Dem.ASR422.SWS_Dem_00861,1 */
          if (Status_Temp > Status)
          {
            /* as different behaviours are requested, SW-C is responsible
             * for behaviour selection */
            Status = Status_Temp;
          }
        }
      }
      /* else: no linked event requests its warning indicator
       * -> status remains DEM_INDICATOR_OFF */
      /* !LINKSTO Dem.ASR422.SWS_Dem_00858,1 */
    }
  }
#endif

  DBG_DEM_J1939INTERNALGETMILSTATUS_EXIT(Status);
  /* return indicator status value */
  return Status;
}
#endif /* (defined DEM_MIL_ID) */

/*---------------------[Dem_J1939InternalGetLampStatus]---------------------*/
#if (((defined DEM_AWIL_ID) || (defined DEM_RSIL_ID) || (defined DEM_PIL_ID)))
STATIC FUNC(Dem_IndicatorStatusType, DEM_CODE) Dem_J1939InternalGetLampStatus(
  Dem_IndicatorIdType IndicatorId)
{
  Dem_IndicatorStatusType Status = DEM_INDICATOR_OFF;
  DBG_DEM_J1939INTERNALGETLAMPSTATUS_ENTER(IndicatorId);

  switch (IndicatorId)
  {
#ifdef DEM_AWIL_ID
    case DEM_AWIL_ID:
#endif
#ifdef DEM_RSIL_ID
    case DEM_RSIL_ID:
#endif
#ifdef DEM_PIL_ID
    case DEM_PIL_ID:
#endif
    {
#if (DEM_INDICATORCLASS_SUPPORT == STD_ON)
        {
          Dem_EventIdType EventId;

          for (EventId = 1U; EventId <= DEM_MAX_EVENTID; EventId++)
          {
#if (DEM_EVENT_DISABLING_SUPPORT == STD_ON)
            if (Dem_GbiEvAvailable(EventId) == FALSE)
            {
              /* event is disabled so the getting of status shall be skipped */
            }
            else
#endif
          {
            const Dem_IndicatorGrpIdxType IndicatorClassIdx =
              Dem_GbiIndicatorClassIndex(EventId);

              /* check for valid indicator index and prevent calibration error */
              if (IndicatorClassIdx < DEM_INDICATOR_UNUSED)
              {
                const Dem_IndicatorIdxType IndicatorClassIdxStart =
                  Dem_IndicatorGroups[IndicatorClassIdx];
                const Dem_IndicatorIdxType IndicatorClassIdxEnd =
                  Dem_IndicatorGroups[IndicatorClassIdx + 1U];
                Dem_IndicatorIdxType Idx;

                for (Idx = IndicatorClassIdxStart; Idx < IndicatorClassIdxEnd; Idx++)
                {
                  CONSTP2CONST(Dem_IndicatorClassType, AUTOMATIC, DEM_CONST) IndicatorClass =
                    &Dem_IndicatorClasses[Idx];
                  if ((IndicatorClass->IndicatorId == IndicatorId) &&
                       (IndicatorClass->Enabled == TRUE) )
                  {
                    if ( DEM_IS_BIT_SET_IN_ARRAY(
                           DEM_INDICATOR_CYC_COUNTER_WIR_STATUS[IndicatorClass->IndicatorId],
                           EventId) )
                    {
                      if (Status == DEM_INDICATOR_OFF)
                      {
                        /* set initial behaviour */
                        Status = IndicatorClass->Behaviour;
                      }
                      else
                      {
                        /* !LINKSTO Dem.ASR422.SWS_Dem_00863,1 */
                        /* !LINKSTO Dem.ASR422.SWS_Dem_00864,1 */
                        /* !LINKSTO Dem.ASR422.SWS_Dem_00865,1 */
                        /* !LINKSTO Dem.ASR422.SWS_Dem_00867,1 */
                        /* !LINKSTO Dem.ASR422.SWS_Dem_00868,1 */
                        /* !LINKSTO Dem.ASR422.SWS_Dem_00869,1 */
                        /* !LINKSTO Dem.ASR422.SWS_Dem_00871,1 */
                        /* !LINKSTO Dem.ASR422.SWS_Dem_00872,1 */
                        /* !LINKSTO Dem.ASR422.SWS_Dem_00873,1 */
                        if (IndicatorClass->Behaviour > Status)
                        {
                          /* as different behaviours are requested, SW-C is responsible
                           * for behaviour selection */
                          Status = IndicatorClass->Behaviour;
                        }
                      }
                    }
                    /* else: no linked event requests its warning indicator
                     * -> status remains DEM_INDICATOR_OFF */
                    /* !LINKSTO Dem.ASR422.SWS_Dem_00862,1 */
                    /* !LINKSTO Dem.ASR422.SWS_Dem_00866,1 */
                    /* !LINKSTO Dem.ASR422.SWS_Dem_00870,1 */
                  }
                }
              }
            }
          }
        }
        /* calibration is disabled and event availability is not considered */
#else
        {
          P2CONST(Dem_IndicatorLinkType, AUTOMATIC, DEM_CONST) IndicatorLink;
          Dem_IndicatorIdxType LinkIdx;

          /* point to first indicator link of according data structures */
          IndicatorLink = &Dem_IndicatorLink[Dem_IndicatorDesc[IndicatorId].StartIdx];

          for (LinkIdx = 0U; LinkIdx < Dem_IndicatorDesc[IndicatorId].Number; ++LinkIdx)
          {
            const Dem_CounterDataIdxType CounterDataIdx =
              LinkIdx + Dem_IndicatorDesc[IndicatorId].StartIdx;

            if (DEM_IS_BIT_SET_IN_ARRAY(
                   DEM_INDICATOR_CYC_COUNTER_WIR_STATUS[DEM_INDICATOR_DEFAULT_COUNTER],
                   CounterDataIdx)
               )
            {
              if (Status == DEM_INDICATOR_OFF)
              {
                /* set initial behaviour */
                Status = IndicatorLink[LinkIdx].Behaviour;
              }
              else
              {
                /* !LINKSTO Dem.ASR422.SWS_Dem_00863,1 */
                /* !LINKSTO Dem.ASR422.SWS_Dem_00864,1 */
                /* !LINKSTO Dem.ASR422.SWS_Dem_00865,1 */
                /* !LINKSTO Dem.ASR422.SWS_Dem_00867,1 */
                /* !LINKSTO Dem.ASR422.SWS_Dem_00868,1 */
                /* !LINKSTO Dem.ASR422.SWS_Dem_00869,1 */
                /* !LINKSTO Dem.ASR422.SWS_Dem_00871,1 */
                /* !LINKSTO Dem.ASR422.SWS_Dem_00872,1 */
                /* !LINKSTO Dem.ASR422.SWS_Dem_00873,1 */
                if (IndicatorLink[LinkIdx].Behaviour > Status)
                {
                  /* as different behaviours are requested, SW-C is responsible
                   * for behaviour selection */
                  Status = IndicatorLink[LinkIdx].Behaviour;
                }
              }
            }
            /* else: no linked event requests its warning indicator
             * -> status remains DEM_INDICATOR_OFF */
            /* !LINKSTO Dem.ASR422.SWS_Dem_00862,1 */
            /* !LINKSTO Dem.ASR422.SWS_Dem_00866,1 */
            /* !LINKSTO Dem.ASR422.SWS_Dem_00870,1 */
          }
        }
#endif
        break;
    }
    /* CHECK: NOPARSE */
    default:
      DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_J1939DcmSetDTCFilter);
      break;
    /* CHECK: PARSE */
  }

  DBG_DEM_J1939INTERNALGETLAMPSTATUS_EXIT(Status, IndicatorId);
  /* return lamp status value */
  return Status;
}
#endif /* ((defined DEM_AWIL_ID) || (defined DEM_RSIL_ID) || (defined DEM_PIL_ID))*/

/*-------------------[Dem_J1939GetInternalLampStatusBytes]------------------*/
STATIC FUNC(void, DEM_CODE) Dem_J1939GetInternalLampStatusBytes(Dem_J1939DcmLampStatusType* LampStatus)
{
  Dem_J1939DcmLampStatusType LampStatusTemp = 0U;

  DBG_DEM_J1939DCMGETINTERNALLAMPSTATUSBYTES_ENTRY(LampStatus);

#if (defined DEM_MIL_ID)
  switch (Dem_J1939InternalGetMilStatus())
  {
      case DEM_INDICATOR_OFF:
      /* !LINKSTO Dem.ASR422.SWS_Dem_00859,1 */
      /* The indicator is off */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939MIL_CONT_UNAVAILABLE;
      break;

      case DEM_INDICATOR_CONTINUOUS:
      /* The indicator is continuously on */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939MIL_ACTIVE;
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939MIL_CONT_UNAVAILABLE;
      break;

      case DEM_INDICATOR_SLOW_FLASH:
      /* The indicator is slow flahsing */
      /* The slow flash status is represented by zeros so no need for any operation. */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939MIL_ACTIVE;
      break;

      case DEM_INDICATOR_FAST_FLASH:
      /* The indicator is fast flahsing */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939MIL_ACTIVE;
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939MIL_FAST_FLASH;
      break;
      /* CHECK: NOPARSE */
      default:
      /* unreachable state. */
      DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_J1939DcmSetDTCFilter);
      break;
      /* CHECK: PARSE */
  }
#else
  LampStatusTemp = LampStatusTemp | (Dem_J1939DcmLampStatusType) DEM_J1939MIL_CONT_UNAVAILABLE;
#endif /* (defined DEM_MIL_ID) */

#if (defined DEM_AWIL_ID)
  switch (Dem_J1939InternalGetLampStatus(DEM_AWIL_ID))
  {
      case DEM_INDICATOR_OFF:
      /* !LINKSTO Dem.ASR422.SWS_Dem_00867,1 */
      /* The indicator is off */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939AWIL_CONT_UNAVAILABLE;
      break;

      case DEM_INDICATOR_CONTINUOUS:
      /* The indicator is continuously on */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939AWIL_ACTIVE;
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939AWIL_CONT_UNAVAILABLE;
      break;

      case DEM_INDICATOR_SLOW_FLASH:
      /* The indicator is slow flahsing */
      /* The slow flash status is represented by zeros so no need for any operation. */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939AWIL_ACTIVE;
      break;

      case DEM_INDICATOR_FAST_FLASH:
      /* The indicator is fast flahsing */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939AWIL_ACTIVE;
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939AWIL_FAST_FLASH;
      break;
      /* CHECK: NOPARSE */
      default:
      /* unreachable state. */
      DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_J1939DcmSetDTCFilter);
      break;
      /* CHECK: PARSE */
  }
#else
  LampStatusTemp = LampStatusTemp | (Dem_J1939DcmLampStatusType) DEM_J1939AWIL_CONT_UNAVAILABLE;
#endif /* (defined DEM_AWIL_ID) */

#if (defined DEM_RSIL_ID)
  switch (Dem_J1939InternalGetLampStatus(DEM_RSIL_ID))
  {
      case DEM_INDICATOR_OFF:
      /* !LINKSTO Dem.ASR422.SWS_Dem_00863,1 */
      /* The indicator is off */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939RSIL_CONT_UNAVAILABLE;
      break;

      case DEM_INDICATOR_CONTINUOUS:
      /* The indicator is continuously on */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939RSIL_ACTIVE;
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939RSIL_CONT_UNAVAILABLE;
      break;

      case DEM_INDICATOR_SLOW_FLASH:
      /* The indicator is slow flahsing */
      /* The slow flash status is represented by zeros so no need for any operation. */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939RSIL_ACTIVE;
      break;

      case DEM_INDICATOR_FAST_FLASH:
      /* The indicator is fast flahsing */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939RSIL_ACTIVE;
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939RSIL_FAST_FLASH;
      break;
      /* CHECK: NOPARSE */
      default:
      /* unreachable state. */
      DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_J1939DcmSetDTCFilter);
      break;
      /* CHECK: PARSE */
  }
#else
  LampStatusTemp = LampStatusTemp | (Dem_J1939DcmLampStatusType) DEM_J1939RSIL_CONT_UNAVAILABLE;
#endif /* (defined DEM_RSIL_ID) */

#if (defined DEM_PIL_ID)
  switch (Dem_J1939InternalGetLampStatus(DEM_PIL_ID))
  {
      case DEM_INDICATOR_OFF:
      /* !LINKSTO Dem.ASR422.SWS_Dem_00871,1 */
      /* The indicator is off */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939PIL_CONT_UNAVAILABLE;
      break;

      case DEM_INDICATOR_CONTINUOUS:
      /* The indicator is continuously on */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939PIL_ACTIVE;
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939PIL_CONT_UNAVAILABLE;
      break;

      case DEM_INDICATOR_SLOW_FLASH:
      /* The indicator is slow flahsing */
      /* The slow flash status is represented by zeros so no need for any operation. */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939PIL_ACTIVE;
      break;

      case DEM_INDICATOR_FAST_FLASH:
      /* The indicator is fast flahsing */
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939PIL_ACTIVE;
      LampStatusTemp |= (Dem_J1939DcmLampStatusType) DEM_J1939PIL_FAST_FLASH;
      break;
      /* CHECK: NOPARSE */
      default:
      /* unreachable state. */
      DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_J1939DcmSetDTCFilter);
      break;
      /* CHECK: PARSE */
  }
#else
  LampStatusTemp = LampStatusTemp | (Dem_J1939DcmLampStatusType) DEM_J1939PIL_CONT_UNAVAILABLE;
#endif /* (defined DEM_PIL_ID) */
  (*LampStatus) = LampStatusTemp;

  DBG_DEM_J1939DCMGETINTERNALLAMPSTATUSBYTES_EXIT(LampStatus);
}


#endif /* (DEM_J1939_READING_DTC_SUPPORT == STD_ON) */

#if ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_READINESS1_SUPPORT == STD_ON) || (DEM_J1939_FREEZEFRAME_SUPPORT==STD_ON))

/*-------------------------[Dem_GetVisibleJ1939DTC]-------------------------*/
STATIC FUNC(uint32, DEM_CODE) Dem_GetVisibleJ1939DTC(Dem_EventIdType EventId)
{
  /* Concerning the Format see Dem_277,1, SWS_Dem_00277,1 */
  uint32 DTC;

  DBG_DEM_GETVISIBLEJ1939DTC_ENTRY(EventId);

#if (DEM_EVENT_DISABLING_SUPPORT == STD_ON)
  if (FALSE == Dem_GbiEvAvailable(EventId))
  {
    DTC = DEM_NO_DTC;
  }
  else
#endif
  {
#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
    if (DEM_IS_BIT_SET_IN_ARRAY(Dem_J1939DTCSuppression, EventId))
    {
      DTC = DEM_NO_DTC;
    }
    else
#endif
    {
      DTC = Dem_InternalGetJ1939DTC(EventId);
    }
  }

  DBG_DEM_GETVISIBLEJ1939DTC_EXIT(DTC, EventId);
  return DTC;
}

#endif /* (DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_READINESS1_SUPPORT == STD_ON) || (DEM_J1939_FREEZEFRAME_SUPPORT==STD_ON) */

#if ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_READINESS1_SUPPORT == STD_ON) || (DEM_J1939_CLEAR_DTC_SUPPORT==STD_ON))

/*-------------------------[Dem_ApplyJ1939DTCFilter]------------------------*/
STATIC FUNC(boolean, DEM_CODE) Dem_ApplyJ1939DTCFilter(
  Dem_EventIdType                                                EventId,
  Dem_J1939DcmDTCStatusFilterType                                J1939Filter
 )
{
  boolean Result = FALSE;
  Dem_DTCStatusMaskType DTCStatus;

  DBG_DEM_APPLYJ1939DTCFILTER_ENTRY(EventId, J1939Filter);

  DTCStatus = DEM_GET_DTC_STATUS(EventId);
#if (DEM_NUM_CMB_DTCS > 0U)
  if (DEM_GET_MASTER_EVID(EventId) == EventId)
#endif
  {
    switch(J1939Filter)
    {
      case DEM_J1939DTC_ACTIVE:
      {
        /* !LINKSTO Dem_OBD_0005,1 */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U)
        if ((Dem_IsEventsMILIndicatorActive(EventId) == TRUE) ||
            (((DTCStatus & DEM_UDS_STATUS_CDTC) != 0U) && (DTCStatus & DEM_UDS_STATUS_TF) != 0))
#else
        if ((((DTCStatus & DEM_UDS_STATUS_CDTC) != 0U) && (DTCStatus & DEM_UDS_STATUS_TF) != 0))
#endif
        {
          Result = TRUE;
        }
        break;
      }
      case DEM_J1939DTC_PREVIOUSLY_ACTIVE:
      {
        /* !LINKSTO Dem_OBD_0005,1 */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U)
        if ((Dem_IsEventsMILIndicatorActive(EventId) == FALSE) &&
            ((DTCStatus & DEM_UDS_STATUS_CDTC) != 0U) && ((DTCStatus & DEM_UDS_STATUS_TF) == 0))
#else
        if (((DTCStatus & DEM_UDS_STATUS_CDTC) != 0U) && ((DTCStatus & DEM_UDS_STATUS_TF) == 0))
#endif
        {
          Result = TRUE;
        }
        break;
      }
      case DEM_J1939DTC_PENDING:
      {
      if ((DTCStatus & DEM_UDS_STATUS_PDTC) != 0)
        {
          Result = TRUE;
        }
        break;
      }
      /* !LINKSTO Dem_OBD_0001,2 */
#if (DEM_OBD_Support == STD_ON)
      case DEM_J1939DTC_PERMANENT:
      {
        /* !LINKSTO Dem_OBD_0005,1 */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U)
        Dem_SizeEvMemEntryType EntryIdx;
        for (EntryIdx = 0U; EntryIdx < DEM_MAX_NUMBER_EVENT_ENTRY_PER; EntryIdx++)
        {
          const Dem_PermanentMemoryEntryType Entry =
            Dem_EventMemoryPermanent[EntryIdx];

          if ( (DEM_GET_PERMANENT_MEMORY_EVENT_ID(Entry) == EventId) &&
               (Dem_EventMemoryPermanentNonVolatileStored[EntryIdx] ==
                  DEM_PERMANENT_MEMORY_NV_STORED) )
          {
            Result = TRUE;
            break;
          }
        }
#endif
        break;
      }
#endif
      case DEM_J1939DTC_CURRENTLY_ACTIVE:
      {
        if (0U != (DTCStatus & DEM_UDS_STATUS_TF))
        {
          Result = TRUE;
        }
        break;
      }
      /* CHECK: NOPARSE */
      default:
      /* Unreachable state */
      DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_J1939DcmGetNextFilteredDTC);
      break;
      /* CHECK: PARSE */
    }
  }

  DBG_DEM_APPLYJ1939DTCFILTER_EXIT(Result, EventId, J1939Filter);

  return Result;
}
#endif /* (DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_READINESS1_SUPPORT == STD_ON) || (DEM_J1939_CLEAR_DTC_SUPPORT==STD_ON) */

#if ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_READINESS1_SUPPORT == STD_ON))
/*-------------------------[Dem_GetNumberJ1939DTCs]-------------------------*/
STATIC FUNC(uint8, DEM_CODE) Dem_GetNumberJ1939DTCs(
       Dem_J1939DcmDTCStatusFilterType  DTCStatusFilter,
       Dem_DTCKindType                  DTCKind,
       uint8                            Node
)
{
  Dem_EventIdType EventId = 1;
  uint8 EventCnt = 0;
  uint32 J1939Dtc;
#if(DEM_OBD_Support == STD_ON)
  Dem_DTCKindType DTCKindVar;
#endif
  /* check the count value as the range of number of DTCS from 0 to 250 */
  while ((EventId <= DEM_MAX_EVENTID) && (EventCnt < DEM_J1939_BYTE_MAX))
  {
    J1939Dtc = Dem_GetVisibleJ1939DTC(EventId);
    if (J1939Dtc != DEM_NO_DTC)
    {
#if(DEM_OBD_Support == STD_ON)
      DTCKindVar = Dem_GbiDTCKind(EventId);
      /* check if no DTC kind was filtered (DEM_DTC_KIND_ALL_DTCS)
       * or if kind filter matches */
      if ((DTCKind == DEM_DTC_KIND_ALL_DTCS) ||
           ( DTCKind == DTCKindVar           ))
#else
      TS_PARAM_UNUSED(DTCKind);
#endif
      {
        if (Node == Dem_InternalGetJ1939Node(EventId))
        {
          if (Dem_ApplyJ1939DTCFilter(EventId, DTCStatusFilter) == TRUE)
          {
            EventCnt++;
          }
        }
      }
    }
    /* update EventId for next check */
    EventId++;
  }
  return EventCnt;
}

#endif /* ((DEM_J1939_READING_DTC_SUPPORT == STD_ON) || (DEM_J1939_READINESS1_SUPPORT == STD_ON) */

#if (DEM_J1939_READINESS1_SUPPORT == STD_ON)
/*----------------------[Dem_UpdateJ1939ReadinessValue]---------------------*/
STATIC FUNC(boolean, DEM_CODE) Dem_UpdateJ1939ReadinessValue(
  Dem_EventIdType EventId,
  uint16 ReadinessGroup,
  boolean ReadinessGroupChecked,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID01value)
{
  DBG_DEM_UPDATE_J1939_READINESS_VALUE_ENTRY(EventId, ReadinessGroup,
                                     ReadinessGroupChecked, PID01value);

  /* check if ReadinessGroup is reported in PID01 and not yet checked */
  if ((ReadinessGroup < DEM_OBD_RDY_PID01PID41_CNT) &&
      (ReadinessGroupChecked == FALSE))
  {
    /* set supported bit of the readiness group */
    DEM_J1939_SET_RDY_SUPPORTED(ReadinessGroup, PID01value);

    /* !LINKSTO Dem354,1 */
    /* check if event was not completed since last clear */
    if (DEM_IS_EV_ST_BIT_TNCSLC_SET(EventId) )
    {
      /* set ReadinessGroup to NOT COMPLETE */
      DEM_J1939_SET_RDY_NOT_COMPLETE(ReadinessGroup, PID01value);

      /* mark ReadinessGroup as checked */
      ReadinessGroupChecked = TRUE;
    }
  }

DBG_DEM_UPDATE_J1939_READINESS_VALUE_EXIT(ReadinessGroupChecked, EventId,
                                    ReadinessGroup, ReadinessGroupChecked,
                                    PID01value);
  return ReadinessGroupChecked;
}

#endif /* (DEM_J1939_READINESS1_SUPPORT == STD_ON) */

#if (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON)
/*------------------------[Dem_J1939DcmStartClearDTC]-----------------------*/
STATIC FUNC(Dem_ReturnClearDTCType, DEM_CODE) Dem_J1939DcmStartClearDTC(
  Dem_J1939DcmSetClearFilterType   DTCTypeFilter,
  uint8                                     Node)
{
  Dem_ReturnClearDTCType Result = DEM_CLEAR_PENDING;

  DBG_DEM_J1939DCMSTARTCLEARDTC_ENTRY(DTCTypeFilter, Node);

  /*
   * ENTER critical section to protect RunState RMW (service lock)
   *       call-context: Dcm (Task), Rte (Task)
  */
  DEM_ENTER_EXCLUSIVE_AREA();

  if (DEM_CLEARDTC_STATE_IDLE == Dem_ClearDtcAsyncStatus.RunState)
  {
    /* !LINKSTO Dem.ASR42.J1939.SWS_Dem_00878.ClearDtc.initialize,1 */
    Dem_ClearDtcAsyncStatus.RunState = DEM_CLEARDTC_STATE_PROCESSING_RAM;
    /* keep service Id to know which function started the clearing process (lock) */
    Dem_ClearDtcAsyncStatus.ClientId = DEM_VIRTUAL_CLIENT_J1939;
    Dem_J1939ClearDTCFilter.Dem_J1939DTCTypeFilter = DTCTypeFilter;
    Dem_J1939ClearDTCFilter.Dem_J1939Node = Node;
    /* event clearing is suspended, store current context */
    Dem_ClearDtcAsyncStatus.NextEventId = 1U;
#if (DEM_NUM_CMB_DTCS > 0U)
    Dem_ClearDtcAsyncStatus.NextCmbEvIdx = 0U;
#endif
    /* status modified flag is always present */
    Dem_ClearDtcAsyncStatus.AnyEventModified.EvSt = FALSE;
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
    /* data modified flag is only present for NONVOLATILE_FINISH */
    Dem_ClearDtcAsyncStatus.AnyEventModified.EvMemEntry = FALSE;
#endif
  }
  else
  {
    /* report Result BUSY */
    /* !LINKSTO SWS_Dem_00662,1 */
    Result = DEM_CLEAR_BUSY;
  }

  /*
   * LEAVE critical section
  */
  DEM_EXIT_EXCLUSIVE_AREA();

  DBG_DEM_J1939DCMSTARTCLEARDTC_EXIT(Result, DTCTypeFilter, Node);
  return Result;
}

/*----------------------[Dem_J1939DcmClearFilterCheck]----------------------*/
FUNC(boolean, DEM_CODE) Dem_J1939DcmClearFilterCheck(
  Dem_J1939DcmSetClearFilterType   DTCTypeFilter,
  uint8                                     Node,
  Dem_EventIdType                        EventId)
{
  boolean Result = FALSE;
  uint32 J1939Dtc;

  DBG_DEM_J1939DCMCLEARFILTERCHECK_ENTRY(DTCTypeFilter, Node, EventId);

  J1939Dtc = Dem_GetVisibleJ1939DTC(EventId);

  if (J1939Dtc != DEM_NO_DTC)
  {
    Dem_J1939DcmDTCStatusFilterType DTCStatusFilter = DEM_J1939DTC_ACTIVE;
    uint8 EventNode;
    boolean FilterResult = FALSE;

    switch (DTCTypeFilter)
    {
      case DEM_J1939DTC_CLEAR_ALL:
      DTCStatusFilter = DEM_J1939DTC_ACTIVE;
      break;
      case DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE:
      DTCStatusFilter = DEM_J1939DTC_PREVIOUSLY_ACTIVE;
      break;
      /* CHECK: NOPARSE */
      default:
      /* unreachable state. */
      DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_J1939DcmClearDTC);
      break;
      /* CHECK: PARSE */
    }

    EventNode = Dem_InternalGetJ1939Node(EventId);
    FilterResult = Dem_ApplyJ1939DTCFilter(EventId, DTCStatusFilter);

    if ((TRUE == FilterResult) && (EventNode == Node))
    {
      Result = TRUE;
    }
  }

  DBG_DEM_J1939DCMCLEARFILTERCHECK_EXIT(Result, DTCTypeFilter, Node, EventId);

  return Result;
}
#endif /* DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON */


/*-------------------------[Dem_InternalGetJ1939DTC]------------------------*/
FUNC(uint32, DEM_CODE) Dem_InternalGetJ1939DTC(Dem_EventIdType EventId)
{
  uint32 DTC;

  DTC = ((uint32)((uint32)Dem_J1939Config[DEM_DESC_J1939_IDX(EventId)].Dem_J1939DTCValue));

  return DTC;
}

/*------------------------[Dem_InternalGetJ1939Node]------------------------*/
STATIC FUNC(uint8, DEM_CODE) Dem_InternalGetJ1939Node(Dem_EventIdType EventId)
{
  uint8 Node;

  Node = ((uint8)((uint8)(Dem_J1939Config[DEM_DESC_J1939_IDX(EventId)].Dem_J1939NodeId)));

  return Node;
}

#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>

#endif /* (DEM_J1939_SUPPORT == STD_ON) */
