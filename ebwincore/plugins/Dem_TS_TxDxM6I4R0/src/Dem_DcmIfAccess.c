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
 * the Dcm for clearing and control of DTC setting. */

/* MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 8.3 (required)
 *    "All declarations of an object or function shall use the same names
 *    and type qualifiers"
 *
 *  Reason:
 *    Different data types names are generated for the Rte ASR42 and ASR40
 *    interfaces, having compatible types.
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason:
 * "FractionalNumEvents" is not equal with "0" in configurations where
 * DemMaxNumberClearEventsPerCycle is greater than 1, and the number of events
 * in an event combination is different from DemMaxNumberClearEventsPerCycle.
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

#include <Dem_Int.h>             /* Module public and internal declarations */
#include <Dem_Trace.h>                        /* Dbg related macros for Dem */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

/** \brief Start clearing of all events related to a DTC or DTC group
 **
 ** \param[in]  ClientId
 **
 ** \return Updated result of clear event process
 */
STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_StartClearDTC(
  uint8 ClientId);

/** \brief Notify about a new DTC selection
 **
 ** This function is used to notify the Clear DTC Processor about a new DTC
 ** selection of a client. If a ClearDTC process for this client is in
 ** progress, the result of the clearing operation will be discarded.
 **
 ** \param[in] ClientId  ID of the client which performed a new DTC selection.
 **
 ** \return none
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(void, DEM_CODE) Dem_NotifyNewDTCSelection(uint8 ClientId);

#if ( (DEM_OBD_CENTRALIZED_PID31_HANDLING == STD_ON) || \
      (DEM_OBD_CENTRALIZED_PID21_HANDLING == STD_ON) || \
      (defined DEM_OPCYC_WARMUP_ID) )
/** \brief Function to reset counters affected by a clearing operation
 **
 ** This function is called by Dem_StartClearDTC(), from within an exclusive
 ** area.
 */
STATIC FUNC(void, DEM_CODE) Dem_ResetOBDCounters(void);
#endif

#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
/** \brief Checks if there are earlier scheduled entries for clearing
 ** immediately
 **
 ** \param[in] OriginIdx  Origin index of the event memory to check
 **
 ** \return Boolean
 ** \retval TRUE  at least one event changed
 ** \retval FALSE  no events changed
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(boolean, DEM_CODE) Dem_CheckForOldClearImmediateEntries(
  uint8 OriginIdx);
#endif

/** \brief Process clearing of events related to a DTC or DTC group
 **
 ** With each call of this function, a limited number of events of a DTC
 ** or DTC group are cleared.
 **
 ** \param[inout] NextEventId  Identifies the next (master) event to be cleared
 ** \param[inout] NextCmbEvIdx  Identifies the next combined event
 ** \param[inout] ClearEventAllowed  Result of last ClearEventAllowed callback
 ** \param[out] AnyEventModified  Member EvSt/EvMemEntry flag is set to TRUE if
 **                               the event status/entry of any event was modified
 **
 ** \return Updated result of the clearing process
 ** \retval E_OK  Events of a DTC or group of DTCs are successfully cleared
 ** \retval DEM_WRONG_DTC  Single DTC not found
 ** \retval DEM_PENDING  Clear limit reached, clearing will be continued on the
                         next call of this function
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_ClearEvents(
  CONSTP2VAR(Dem_EventIdType, AUTOMATIC, DEM_VAR_CLEARED)     NextEventId,
#if (DEM_NUM_CMB_DTCS > 0U)
  CONSTP2VAR(Dem_CmbDTCEvLnkType, AUTOMATIC, DEM_VAR_CLEARED) NextCmbEvIdx,
#if (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON)
  CONSTP2VAR(Dem_ClearEventAllowedType, AUTOMATIC, AUTOMATIC) ClearEventAllowed,
#endif
#endif
  CONSTP2VAR(Dem_EventModifiedType, AUTOMATIC, AUTOMATIC)     AnyEventModified);

#if (DEM_NUM_CMB_DTCS > 0U)
/** \brief Process clearing of combined events
 **
 ** \param[in] EventId  Master event of the event combination
 ** \param[in] ForcedDelete  Flag to force(TRUE) the clear, even if the event is disabled
 **                          (not available) or the assigned DTC is suppressed
 ** \param[inout] RemainingEventsToProcess  Specifies the number of events to be cleared
 ** \param[inout] NextCmbEvIdx  Identifies the next combined event to be cleared. A value
 **                             of 0 starts the clearing with the first event of the
 **                             combination. After each call, NextCmbEvIdx is updated.
 **                             If all events of the event combination are cleared,
 **                             NextCmbEvIdx is set to 0.
 ** \param[out] AnyEventModified  Member EvSt/EvMemEntry flag is set to TRUE if
 **                               the event status/entry of any event was modified
 **
 ** \return Updated result of the clearing process
 ** \retval E_OK  Combined events are successfully cleared
 ** \retval DEM_PENDING  Clear limit is reached
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_ClearCmbEvents(
  Dem_EventIdType                                             EventId,
  boolean                                                     ForcedDelete,
  CONSTP2VAR(uint16, AUTOMATIC, AUTOMATIC)                    RemainingEventsToProcess,
  CONSTP2VAR(Dem_CmbDTCEvLnkType, AUTOMATIC, DEM_VAR_CLEARED) NextCmbEvIdx,
  CONSTP2VAR(Dem_EventModifiedType, AUTOMATIC, AUTOMATIC)     AnyEventModified);
#endif

#if (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON)
/** \brief Clears only this cycle and readiness bits for an event
 **
 ** \param[in] EventId  0 < EventId <= DEM_MAX_EVENTID
 **
 ** \return none
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(void, DEM_CODE) Dem_ClearThisCycleAndReadinessStatus(
  Dem_EventIdType EventId);

#if (DEM_NUM_CMB_DTCS > 0U)
/** \brief Clears only this cycle and readiness bits for an event and also
 ** for its combined events (if any)
 **
 ** \param[in] EventId  0 < EventId <= DEM_MAX_EVENTID
 ** \param[inout] NextCmbEvIdx  Identifies the next combined event for status clearing. A value
 **                             of 0 starts the status clearing with the first event of the
 **                             combination. After each call, NextCmbEvIdx is updated.
 **                             If all events of the event combination have status cleared,
 **                             NextCmbEvIdx is set to 0.
 ** \param[inout] RemainingEventsToProcess  Specifies the number of events that may be processed
 **                                         per cycle
 **
 ** \return Updated result of the status clearing process
 ** \retval E_OK  Status of all events is successfully cleared
 ** \retval DEM_PENDING  Clear limit is reached
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_ClearThisCycleAndReadinessStatusCmbEvents(
  Dem_EventIdType                                             EventId,
  CONSTP2VAR(Dem_CmbDTCEvLnkType, AUTOMATIC, DEM_VAR_CLEARED) NextCmbEvIdx,
  CONSTP2VAR(uint16, AUTOMATIC, AUTOMATIC)                    RemainingEventsToProcess);
#endif
#endif

/** \brief Reset status bits, clear an event entry and event related data
 ** (like healing counters)
 **
 ** \param[in]  EventId  Identifies the event to be cleared
 ** \param[out] AnyEventModified  Member EvSt/EvMemEntry flag is set to TRUE if
 **                               the event status/entry was modified
 **
 ** \return none
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(void, DEM_CODE) Dem_ClearEvent(
  Dem_EventIdType                                    EventId,
  P2VAR(Dem_EventModifiedType, AUTOMATIC, AUTOMATIC) AnyEventModified);

/** \brief Function to check if an event can be cleared
 **
 ** This function checks the conditions for clearing an event e.g. event availability,
 ** DTC suppression, matching single DTC or DTC group etc.
 **
 ** \param[in] DtcGroupRequest  Flag to indicate single(FALSE) or group(TRUE) request
 ** \param[in] GroupIdx  The group index in case of a group request
 ** \param[in] EventId  Identifies the event to check for clearing
 ** \param[in] ForcedDelete  Flag to force(TRUE) the clear, even if the event is disabled
 **                          (not available) or the assigned DTC is suppressed
 **
 ** \return  Result of the check
 ** \retval E_OK  Check passed; proceed with event clearing
 ** \retval E_NOT_OK  Check failed; one or more conditions prevent the clearing of the event
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_CheckDeleteEvent(
  boolean         DtcGroupRequest,
  uint8           GroupIdx,
  Dem_EventIdType EventId,
  boolean         ForcedDelete);

#if((DEM_J1939_SUPPORT == STD_ON) && (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON))
/** \brief Function to check if an event can be cleared
 **
 ** This function checks the conditions for clearing an event e.g. event availability,
 ** DTC suppression, matching single DTC or DTC group etc.
 **
 ** \param[in] EventId  Identifies the event to check for clearing
 **
 ** \return  Result of the check
 ** \retval E_OK  Check passed; proceed with event clearing
 ** \retval E_NOT_OK  Check failed; one or more conditions prevent the clearing of the event
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_J1939CheckDeleteEvent(
  Dem_EventIdType EventId);
#endif

/** \brief Function to check if the ClearDTC request is a group request
 **
 ** \param[in]  DTC
 ** \param[in]  DTCFormat
 ** \param[out] GroupIdx
 **
 ** \return Boolean value indication success
 ** \retval TRUE  DTC is a group from the clear perspective
 ** \retval FALSE DTC is not a group
 */
STATIC FUNC(boolean, DEM_CODE) Dem_IsDTCGroupClear(
  uint32                             DTC,
  Dem_DTCFormatType                  DTCFormat,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) GroupIdx);

#if (DEM_NUM_DTC_GROUPS > 0U)
/** \brief Function to convert an 24-bit DTC-group-value into the
 ** corresponding DTC-group-index
 **
 ** \param[in] DTCGroup
 ** \param[out] Idx
 **
 ** \return Boolean value indication success
 ** \retval TRUE  could find corresponding DTC-group-index
 ** \retval FALSE wrong DTC-group-value
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **
 */
STATIC FUNC(boolean, DEM_CODE) Dem_GetIdxOfDTCGroup(
  Dem_DTCGroupType                   DTCGroup,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) Idx);
#endif

/** \brief Process clearing of all DTCs or DTCs group
 **
 ** This function can be called by Dem_ClearDTC() and Dem_ASR42_ClearDTC().
 **
 ** \param[in]  ClientId
 **
 ** \return Updated result of ClearDTC process
 **/
STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_InternalClearDTC(
  uint8 ClientId);

/** \brief Helper function to wrap the functionality of retrieving the
 ** result of a DTC selection
 **
 ** This function is used by Dem_GetDTCSelectionResult() and
 ** Dem_GetDTCSelectionResultForClearDTC().
 **
 ** \param[in]  ServiceId
 ** \param[in]  ClientId
 **
 ** \return Result of the DTC selection
 **/
STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_InternalGetDTCSelectionResult(
  uint8 ClientId,
  uint8 ServiceId);

#if (DEM_GET_DTCBYOCCURRENCETIME_SUPPORT == STD_ON)
/** \brief Process clearing the information related to the DTC occurrence time kind
 **
 ** This function is called by Dem_ClearEvent(). The function checks whether the event
 ** belongs to primary memory, then it further checks if the event that is removed from
 ** event memory is stored as any of the occurrence kinds, and if that is the case, the
 ** location is cleared for a new EventId to be stored. If not the function simply exits
 ** without further actions.
 **
 ** \param[in]  EventId ID of event
 **/
STATIC FUNC(void, DEM_CODE) Dem_ResetEventIdOccurKinds(Dem_EventIdType EventId);
#endif

#if (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON)
/** \brief Wrapper-Function to call ClearEventAllowed functions
 **
 ** Dem_CbClearEventAllowed shall be enabled only if
 ** either RTE callback or C callback is configured for clear event allowed.
 **
 ** This must be done by a wrapper, because it depends on configuration.
 **
 ** \param[in] EventId  0 < EventId < DEM_NUMBER_OF_EVENTS
 ** \param[out] Behavior (DEM_ONLY_THIS_CYCLE_AND_READINESS or DEM_NO_STATUS_BYTE_CHANGE)
 ** \param[out] Allowed  Clearance of the event is allowed (TRUE)/not allowed (FALSE)
 **                      Parameter Allowed will be unchanged if E_NOT_OK is returned.
 **
 ** \return  Result of the ClearEventAllowed callback function
 ** \Retval E_OK  Operation was successful
 ** \Retval E_NOT_OK  Operation failed
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_CbClearEventAllowed(
  Dem_EventIdType                          EventId,
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA)  Behavior,
  P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) Allowed);
#endif

#if ( (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON) || \
      (DEM_USE_CB_INIT_MONITOR == STD_ON) || \
      ( (DEM_J1939_SUPPORT == STD_ON) && \
        (DEM_J1939_READING_DTC_SUPPORT == STD_ON) ) )
/** \brief Function to trigger the necessary callbacks on clearing of an event
 **
 ** \param[in]  EventId           ID of the event that is cleared
 ** \param[in]  OldDTCStatusMask  UDS status of the event before the clear
 ** \param[in]  NewDTCStatusMask  UDS status of the event after the clear
 ** \param[in]  J1939DcmCbNeeded  flag indicating whether J1939Dcm callback
 **                               should be triggered
 **
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 **/
STATIC FUNC(void, DEM_CODE) Dem_TriggerCbForClearedEvent(
  Dem_EventIdType       EventId,
  Dem_DTCStatusMaskType OldDTCStatusMask,
  Dem_DTCStatusMaskType NewDTCStatusMask,
  boolean               J1939DcmCbNeeded);
#endif /* ( (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON) ||
            (DEM_USE_CB_INIT_MONITOR == STD_ON) ||
            ( (DEM_J1939_SUPPORT == STD_ON) &&
              (DEM_J1939_READING_DTC_SUPPORT == STD_ON) ) ) */

#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
#define DEM_START_SEC_VAR_CLEARED_8
#include <Dem_MemMap.h>

#if (DEM_NUMBER_OF_CLIENTS > 1U)
/* The client ID which locked the DTC setting */
static VAR(uint8, DEM_VAR_CLEARED) Dem_DTCSettingLockByClientId;
#endif

/* Flag for DTC setting lock */
VAR(boolean, DEM_VAR_CLEARED) Dem_DTCSettingAllowed;

/* Flag that indicates the ClearDTC result can be discarded */
STATIC VAR(boolean, DEM_VAR_CLEARED) Dem_NewDTCSelection;

#define DEM_STOP_SEC_VAR_CLEARED_8
#include <Dem_MemMap.h>

#define DEM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dem_MemMap.h>

/* !LINKSTO Dem.ASR431.SWS_Dem_01251,1 */
/** \brief DTC selection related information of all diagnostic clients
 **
 ** The highest index positions are reserved for virtual clients, used
 ** for e.g. ASR42 ClearDtc() and J1939DcmClearDTC().
 */
STATIC VAR(Dem_DTCSelectionType, DEM_VAR_CLEARED)
  Dem_DTCSelection[DEM_NUMBER_OF_CLIENTS + DEM_NUMBER_OF_VIRTUAL_CLIENTS];

/** \brief DTC selection of the client which started the ClearDTC operation */
STATIC VAR(Dem_DTCSelectionType, DEM_VAR_CLEARED) Dem_ClearDTCSelection;

#if ( (DEM_INIT_MONITOR_REENABLED_SUPPORT == STD_ON) && \
      (DEM_USE_CB_INIT_MONITOR == STD_ON) )
/** \brief Controls the asynchronous handling of InitMonitorForEvent
 ** callbacks */
VAR(Dem_InitMonitorReenableStateType, DEM_VAR_CLEARED)
  Dem_InitMonitorReenableState;
#endif

#define DEM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dem_MemMap.h>
/*==================[external function definitions]=========================*/

#if (DEM_INCLUDE_RTE == STD_ON)
#define Dem_START_SEC_CODE
#else
#define DEM_START_SEC_CODE
#endif
#include <Dem_MemMap.h>

/*------------------[Dem_ASR42_ClearDTC]------------------------------------*/

#if (DEM_DEFAULT_ASR_SERVICE_API == DEM_SERVICE_API_ASR42)
/* Deviation MISRAC2012-1 */
FUNC(Dem_ReturnClearDTCType, DEM_CODE) Dem_ASR42_ClearDTC(
  uint32            DTC,
  Dem_DTCFormatType DTCFormat,
  Dem_DTCOriginType DTCOrigin)
{
  Dem_ReturnClearDTCType Result = DEM_ASR42_CLEAR_FAILED;

  DBG_DEM_ASR42_CLEARDTC_ENTRY(DTC, DTCFormat, DTCOrigin);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_ClearDTC, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_ClearDTC, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  /* check if the DTC is invalid */
#if (DEM_OBD_Support == STD_ON)
  else if ( (DTC > DEM_DTC_GROUP_ALL_DTCS) ||
            ( (DTCFormat == DEM_DTC_FORMAT_OBD) &&
              (DTC != (uint32) DEM_DTC_GROUP_ALL_DTCS) &&
              ((DTC & (uint32) 0x0000FFU) != (uint32) 0x000000U)
            )
          )
#else
  else if (DTC > DEM_DTC_GROUP_ALL_DTCS)
#endif
  {
    DEM_REPORT_ERROR(DEM_SID_ClearDTC, DEM_E_PARAM_DATA);
    Result = DEM_CLEAR_WRONG_DTC;
  }
  /* check if the DTCFormat is invalid */
  else if ( (DTCFormat != DEM_DTC_FORMAT_OBD) &&
            (DTCFormat != DEM_DTC_FORMAT_UDS) &&
            (DTCFormat != DEM_DTC_FORMAT_J1939) )
  {
    DEM_REPORT_ERROR(DEM_SID_ClearDTC, DEM_E_PARAM_DATA);
    Result = DEM_CLEAR_WRONG_DTC;
  }
  /* check if the DTCOrigin is invalid.
   * DTCOrigin values in the user-defined memory origin range 0x01XX
   * are not treated as invalid, because the common ASR4.3.1 based
   * implementation is backward compatible to ASR4.2. */
  else if ( (DTCOrigin > (Dem_DTCOriginType)0x01FFU) ||
            ( (DTCOrigin < (Dem_DTCOriginType)0x0100U) &&
              (DTCOrigin != DEM_DTC_ORIGIN_PRIMARY_MEMORY) &&
              (DTCOrigin != DEM_DTC_ORIGIN_MIRROR_MEMORY) &&
              (DTCOrigin != DEM_DTC_ORIGIN_PERMANENT_MEMORY) &&
              (DTCOrigin != DEM_DTC_ORIGIN_SECONDARY_MEMORY) )
          )
  {
    DEM_REPORT_ERROR(DEM_SID_ClearDTC, DEM_E_PARAM_DATA);
    Result = DEM_CLEAR_WRONG_DTCORIGIN;
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    Std_ReturnType InternalResult = E_NOT_OK;

    /* store DTC related information of the client to internal data structure */
    Dem_SetDTCSelectionCriteria(DEM_VIRTUAL_CLIENT_CDD,
                                DTC,
                                DTCFormat,
                                DTCOrigin,
                                DEM_DTC_ORIGIN_MAPPING_SWC_IF);

    InternalResult = Dem_InternalClearDTC(DEM_VIRTUAL_CLIENT_CDD);

    /* map ASR4.3.1 Std_ReturnType values to ASR4.2 Dem_ReturnClearDTCType values */
    switch (InternalResult)
    {
      case DEM_WRONG_DTC:
        Result = DEM_CLEAR_WRONG_DTC;
        break;
      case DEM_WRONG_DTCORIGIN:
        Result = DEM_CLEAR_WRONG_DTCORIGIN;
        break;
      default:
        Result = (Dem_ReturnClearDTCType)InternalResult;
        break;
    }
  }

  DBG_DEM_ASR42_CLEARDTC_EXIT(Result, DTC, DTCFormat, DTCOrigin);
  return Result;
}
#endif /* DEM_DEFAULT_ASR_SERVICE_API == DEM_SERVICE_API_ASR42 */

#if (DEM_INCLUDE_RTE == STD_ON)
#define Dem_STOP_SEC_CODE
#else
#define DEM_STOP_SEC_CODE
#endif
#include <Dem_MemMap.h>

#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

/*------------------[Dem_ClearDTC]------------------------------------------*/

/* !LINKSTO Dem.ASR431.DTCSelectorDependentAPIs,1 */
FUNC(Std_ReturnType, DEM_CODE) Dem_ClearDTC(
  uint8 ClientId)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_CLEARDTC_ENTRY(ClientId);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_ClearDTC, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_ClearDTC, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_ClearDTC,
      DEM_E_WRONG_CONFIGURATION);
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
      DEM_REPORT_ERROR(DEM_SID_ClearDTC, DEM_E_WRONG_CONDITION);
    }
    else
#endif /* DEM_DEV_ERROR_DETECT */
    {
      Result = Dem_InternalClearDTC(ClientId);
    }
  }

  DBG_DEM_CLEARDTC_EXIT(Result, ClientId);
  return Result;
}

/*------------------[Dem_ProcessClearDTC]-----------------------------------*/

FUNC(void, DEM_CODE) Dem_ProcessClearDTC(void)
{
  Std_ReturnType Result = DEM_PENDING;

  DBG_DEM_PROCESSCLEARDTC_ENTRY();

  /* clear events in RAM */
  Result = Dem_ClearEvents(&Dem_ClearDtcAsyncStatus.NextEventId,
#if (DEM_NUM_CMB_DTCS > 0U)
                           &Dem_ClearDtcAsyncStatus.NextCmbEvIdx,
#if (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON)
                           &Dem_ClearDtcAsyncStatus.ClearEventAllowed,
#endif
#endif
                           &Dem_ClearDtcAsyncStatus.AnyEventModified);

  if (DEM_PENDING != Result)
  {
    /* RAM clearing done; Result is either E_OK or DEM_WRONG_DTC */
    if (Result == E_OK)
    {
#if ( (DEM_INCLUDE_RTE == STD_ON) && \
      (DEM_OBD_Support_Kind == DEM_OBD_MASTER_ECU) )
    /* inform a software component about service $04 execution */
    /* !LINKSTO dsn.Dem.OBD.Mode04.Master.ConditionsForNotifyingSwC.0001, 1 */
      const uint32 DTC = Dem_ClearDTCSelection.DTCInfo.DTC;
      const Dem_DTCFormatType DTCFormat = Dem_ClearDTCSelection.DTCInfo.DTCFormat;
      const Dem_DTCOriginType DTCOrigin = Dem_ClearDTCSelection.DTCInfo.DTCOrigin;

      if ( (DTC == DEM_DTC_GROUP_ALL_DTCS) &&
           (DTCFormat == DEM_DTC_FORMAT_OBD) &&
           (DTCOrigin == DEM_DTC_ORIGIN_PRIMARY_MEMORY) )
      {
        /* !LINKSTO SWS_Dem_00721, 1 */
        (void)Rte_Call_SetClearDTC_master_SetClearDTC(DTC, DTCFormat, DTCOrigin);
      }
#endif /* (DEM_OBD_Support_Kind == DEM_OBD_MASTER_ECU) && \
          (DEM_INCLUDE_RTE == STD_ON) */

#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
      {
#if((DEM_J1939_SUPPORT == STD_ON) && (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON))
        if(Dem_ClearDtcAsyncStatus.ClientId != DEM_VIRTUAL_CLIENT_J1939)
#endif
        {
          const uint8 OriginIdx =
            (uint8)(Dem_ClearDTCSelection.DTCInfo.DTCOrigin - DEM_DTC_ORIGIN_PRIMARY_MEMORY);

          /* data modified flag is only present for NONVOLATILE_FINISH */
          if (FALSE == Dem_ClearDtcAsyncStatus.AnyEventModified.EvMemEntry)
          {
            Dem_ClearDtcAsyncStatus.AnyEventModified.EvMemEntry =
              Dem_CheckForOldClearImmediateEntries(OriginIdx);
          }
        }
      }
#endif

#if (DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON)
      /* trigger the requests to the NvM module once for all events for
       * performance reasons */
      Result = Dem_TriggerNvmWrite(&Dem_ClearDtcAsyncStatus.AnyEventModified,
                                   DEM_SID_MainFunction);
#endif
    }

    /*
     * ENTER critical section to ensure consistency
     *       between RunState and OperationResult
     */
    DEM_ENTER_EXCLUSIVE_AREA();

    /* check it the client is still waiting for the result */
    if (TRUE == Dem_NewDTCSelection)
    {
      /* !LINKSTO Dem.ASR431.SWS_Dem_01042, 1 */
      /* discard the result and release the clearing lock */
      Dem_ClearDtcAsyncStatus.RunState = DEM_CLEARDTC_STATE_IDLE;
    }
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
    else if (DEM_PENDING == Result)
    {
      /* RunState was set to PROCESSING_NVRAM by Dem_TriggerNvmWrite() */
    }
#endif
    else
    {
      /* set the OperationResult to be reported to the client */
      Dem_ClearDtcAsyncStatus.OperationResult = Result;
      Dem_ClearDtcAsyncStatus.RunState = DEM_CLEARDTC_STATE_FINISHED;
    }

    /*
     * EXIT critical section
     */
    DEM_EXIT_EXCLUSIVE_AREA();
  }

  DBG_DEM_PROCESSCLEARDTC_EXIT();
}

/*------------------[Dem_DisableDTCSetting]---------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DisableDTCSetting(uint8 ClientId)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_DISABLEDTCSETTING_ENTRY(ClientId);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DisableDTCSetting, DEM_E_UNINIT);
  }
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_DisableDTCSetting, DEM_E_WRONG_CONFIGURATION);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DisableDTCSetting, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
#if (DEM_NUMBER_OF_CLIENTS > 1U)
    uint8   LocalDTCSettingLock;
    boolean LocalDTCSettingAllowed;
#else
    boolean LocalDTCSettingAllowed = Dem_DTCSettingAllowed;

#if (DEM_DEV_ERROR_DETECT == STD_OFF)
    TS_PARAM_UNUSED(ClientId);
#endif
#endif

#if (DEM_NUMBER_OF_CLIENTS > 1U)
    /*
     * ENTER critical section to ensure both variables are used
     *   synchronously
     */
    DEM_ENTER_EXCLUSIVE_AREA();

    /* keep the global values locally to ensure synchronous concurrency
     * protection */
    LocalDTCSettingLock    = Dem_DTCSettingLockByClientId;
    LocalDTCSettingAllowed = Dem_DTCSettingAllowed;

    if (LocalDTCSettingAllowed == TRUE)
    {
      Dem_DTCSettingLockByClientId = ClientId;
      /* !LINKSTO Dem.ASR431.SWS_Dem_00079,1 */
      Dem_DTCSettingAllowed = FALSE;
    }
    /*
     * LEAVE critical section
     */
    DEM_EXIT_EXCLUSIVE_AREA();

#endif /* (DEM_NUMBER_OF_CLIENTS > 1U) */

    if (LocalDTCSettingAllowed == TRUE)
    {
#if (DEM_USE_TRIGGER_DEBOUNCE_BEHAVIOR == STD_ON)
      {
        Dem_EventIdType EventId;

        /* loop through all events */
        /* Deviation TASKING-2 */
        for (EventId = 1U; EventId <= DEM_MAX_EVENTID; ++EventId)
        {
          /* !LINKSTO Dem.ASR431.ControlDTCSetting.AffectedEvents,1 */
          if (DEM_NO_DTC != Dem_GbiUdsDTC(EventId))
          {
            Dem_TriggerDebounceAlgorithmBehavior(EventId);
          }
        }
      }
#endif

      Result = E_OK;
#if (DEM_NUMBER_OF_CLIENTS == 1U)
      /* !LINKSTO Dem.ASR431.SWS_Dem_00079,1 */
      Dem_DTCSettingAllowed = FALSE;
#endif
    }
    else
    {
#if (DEM_NUMBER_OF_CLIENTS > 1U)
      if (LocalDTCSettingLock == ClientId)
      {
        Result = E_OK;
      }
#if (DEM_DEV_ERROR_DETECT == STD_ON)
      else
      {
        /* Result = E_NOT_OK; - already set */
        /* !LINKSTO Dem.ASR431.ControlDTCSetting.DetWrongCondition,1 */
        DEM_REPORT_ERROR(DEM_SID_DisableDTCSetting, DEM_E_WRONG_CONDITION);
      }
#endif
#else
      Result = E_OK;
#endif /* (DEM_NUMBER_OF_CLIENTS > 1U) */
    }
  }

  DBG_DEM_DISABLEDTCSETTING_EXIT(Result, ClientId);
  return Result;
}

/*------------------[Dem_EnableDTCSetting]----------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_EnableDTCSetting(uint8 ClientId)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_ENABLEDTCSETTING_ENTRY(ClientId);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_EnableDTCSetting, DEM_E_UNINIT);
  }
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_EnableDTCSetting, DEM_E_WRONG_CONFIGURATION);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_EnableDTCSetting, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
#if (DEM_NUMBER_OF_CLIENTS > 1U)
    uint8   LocalDTCSettingLock;
    boolean LocalDTCSettingAllowed;
#else
    boolean LocalDTCSettingAllowed = Dem_DTCSettingAllowed;

#if (DEM_DEV_ERROR_DETECT == STD_OFF)
    TS_PARAM_UNUSED(ClientId);
#endif
#endif

#if ((DEM_NUMBER_OF_CLIENTS > 1U) || \
     ((DEM_INIT_MONITOR_REENABLED_SUPPORT == STD_ON) && \
      (DEM_USE_CB_INIT_MONITOR == STD_ON) ) )
    /*
     * ENTER critical section to ensure coherent update of the variables
     */
    DEM_ENTER_EXCLUSIVE_AREA();
#endif

#if (DEM_NUMBER_OF_CLIENTS > 1U)
    /* keep the global values locally to ensure synchronous concurrency
     * protection */
    LocalDTCSettingLock    = Dem_DTCSettingLockByClientId;
    LocalDTCSettingAllowed = Dem_DTCSettingAllowed;
#endif

    if (LocalDTCSettingAllowed == FALSE)
    {
#if (DEM_NUMBER_OF_CLIENTS > 1U)
      if (LocalDTCSettingLock == ClientId)
#endif
      {
#if ( (DEM_INIT_MONITOR_REENABLED_SUPPORT == STD_ON) && \
      (DEM_USE_CB_INIT_MONITOR == STD_ON) )
        /* indicate that the InitMonitorForEvent callbacks should be called */
        /* !LINKSTO Dem.ASR431.ControlDTCSetting.InitMonitorReason,1 */
        Dem_InitMonitorReenableState.DoProcess = TRUE;
        Dem_InitMonitorReenableState.NextCbIdx = 0U;
#endif

#if (DEM_NUMBER_OF_CLIENTS > 1U)
        /* update lock as DTC setting is enabled */
        Dem_DTCSettingLockByClientId = DEM_CLIENT_ID_INVALID;
#endif
        /* !LINKSTO Dem.ASR431.SWS_Dem_00080,1 */
        Dem_DTCSettingAllowed = TRUE;
      }
    }

#if ((DEM_NUMBER_OF_CLIENTS > 1U) || \
     ((DEM_INIT_MONITOR_REENABLED_SUPPORT == STD_ON) && \
      (DEM_USE_CB_INIT_MONITOR == STD_ON) ) )
    /*
     * LEAVE critical section to ensure coherent update of the variables
     */
    DEM_EXIT_EXCLUSIVE_AREA();
#endif

    if (LocalDTCSettingAllowed == FALSE)
    {
#if (DEM_NUMBER_OF_CLIENTS > 1U)
      if (LocalDTCSettingLock != ClientId)
      {
        /* DTC enabling attempted by a different client:
         * Result = E_NOT_OK; - already set */
#if (DEM_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Dem.ASR431.ControlDTCSetting.DetWrongCondition,1 */
        DEM_REPORT_ERROR(DEM_SID_EnableDTCSetting, DEM_E_WRONG_CONDITION);
#endif
      }
      else
#endif /* (DEM_NUMBER_OF_CLIENTS > 1U) */
      {
        Result = E_OK;
      }
    }
    else
    {
      /* DTC setting enabling without first disabling allowed
       * without any side effects */
      Result = E_OK;
    }
  }

  DBG_DEM_ENABLEDTCSETTING_EXIT(Result, ClientId);
  return Result;
}

/*------------------[Dem_InitEventReportVerifier]---------------------------*/

FUNC(void, DEM_CODE) Dem_InitEventReportVerifier(void)
{
  /* This function is called from Dem_Init always (after Dem is
   * pre-initialized as well as after post-initialized) */
  DBG_DEM_INITEVENTREPORTVERIFIER_ENTRY();

#if (DEM_NUMBER_OF_CLIENTS > 1U)
  Dem_DTCSettingLockByClientId = DEM_CLIENT_ID_INVALID;
#endif
  Dem_DTCSettingAllowed = TRUE;

#if ( (DEM_INIT_MONITOR_REENABLED_SUPPORT == STD_ON) && \
      (DEM_USE_CB_INIT_MONITOR == STD_ON) )
  /* Reset monitor reenable callback control mechanism to
   * stop processing of any triggers from the previous cycle */
  Dem_InitMonitorReenableState.DoProcess = FALSE;
#endif

  DBG_DEM_INITEVENTREPORTVERIFIER_EXIT();
}

/*------------------[Dem_ProcessInitMonitorReenable]------------------------*/

#if ( (DEM_INIT_MONITOR_REENABLED_SUPPORT == STD_ON) && \
      (DEM_USE_CB_INIT_MONITOR            == STD_ON) )
FUNC(void, DEM_CODE) Dem_ProcessInitMonitorReenable(void)
{
  Dem_CbIndexType NextCbIdx;
  Dem_CbIndexType LastCbIdx;

  /*
   * ENTER critical section to protect access to global data
   */
  DEM_ENTER_EXCLUSIVE_AREA();

  NextCbIdx = Dem_InitMonitorReenableState.NextCbIdx;
  /* calculate NextCbIdx for next cycle and update state */
  LastCbIdx = NextCbIdx + DEM_MAX_NUM_INIT_MONITOR_PER_CYCLE;

  if (LastCbIdx >= DEM_LOOKUP_TABLE_INIT_MONITOR_SIZE)
  {
    LastCbIdx = DEM_LOOKUP_TABLE_INIT_MONITOR_SIZE;

    /* processing done after calling the remaining CBs */
    Dem_InitMonitorReenableState.DoProcess = FALSE;
  }

  Dem_InitMonitorReenableState.NextCbIdx = LastCbIdx;

  /*
   * LEAVE critical section
   */
  DEM_EXIT_EXCLUSIVE_AREA();

  /* call at most DEM_MAX_NUM_INIT_MONITOR_PER_CYCLE callback functions */
  while (NextCbIdx < LastCbIdx)
  {
    Dem_EventIdType EventId = Dem_CbLookupTableInitMonitor[NextCbIdx];

    /* !LINKSTO Dem.ASR431.ControlDTCSetting.AffectedEvents,1 */
    if (DEM_NO_DTC != Dem_GbiUdsDTC(EventId))
    {
      /* !LINKSTO Dem.ASR431.ControlDTCSetting.InitMonitorReason,1 */
      (void)Dem_CbFuncPtrInitMonitor[NextCbIdx](DEM_INIT_MONITOR_REENABLED);
    }

    NextCbIdx++;
  }
}
#endif

/*------------------[Dem_InitDTCSelector]-----------------------------------*/

FUNC(void, DEM_CODE) Dem_InitDTCSelector(void)
{
  uint16_least Idx;

  DBG_DEM_INITDTCSELECTOR_ENTRY();

  /* Deviation TASKING-2 */
  for (Idx = 0U;
       Idx < (DEM_NUMBER_OF_CLIENTS + DEM_NUMBER_OF_VIRTUAL_CLIENTS);
       Idx++)
  {
    Dem_DTCSelection[Idx].DTCInfo.DTC       = DEM_DTC_GROUP_ALL_DTCS;
    Dem_DTCSelection[Idx].DTCInfo.EventId   = DEM_EVENT_ID_INVALID;
    Dem_DTCSelection[Idx].DTCInfo.DTCOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
    Dem_DTCSelection[Idx].DTCInfo.DTCFormat = DEM_DTC_FORMAT_UDS;
    Dem_DTCSelection[Idx].SelectorStatus    = DEM_DTC_SELECTOR_NO_REQUEST;
  }

  DBG_DEM_INITDTCSELECTOR_EXIT();
}

/*------------------[Dem_SelectDTC]-----------------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_SelectDTC(
  uint8             ClientId,
  uint32            DTC,
  Dem_DTCFormatType DTCFormat,
  Dem_DTCOriginType DTCOrigin)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_SELECTDTC_ENTRY(ClientId, DTC, DTCFormat, DTCOrigin);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_SelectDTC, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_SelectDTC, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  /* check if the ClientId is invalid */
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(DEM_SID_SelectDTC, DEM_E_WRONG_CONFIGURATION);
  }
  /* check if the DTC is invalid */
  else if (DTC > DEM_DTC_GROUP_ALL_DTCS)
  {
    DEM_REPORT_ERROR(DEM_SID_SelectDTC, DEM_E_PARAM_DATA);
  }
  /* check if the DTCFormat is invalid */
  else if ( (DTCFormat != DEM_DTC_FORMAT_OBD) &&
            (DTCFormat != DEM_DTC_FORMAT_UDS) &&
            (DTCFormat != DEM_DTC_FORMAT_J1939) )
  {
    DEM_REPORT_ERROR(DEM_SID_SelectDTC, DEM_E_PARAM_DATA);
  }
  /* check if the DTCOrigin is invalid */
  else if ( (DTCOrigin > (Dem_DTCOriginType)0x01FFU) ||
            ( (DTCOrigin < (Dem_DTCOriginType)0x0100U) &&
              (DTCOrigin != DEM_DTC_ORIGIN_PRIMARY_MEMORY) &&
              (DTCOrigin != DEM_DTC_ORIGIN_MIRROR_MEMORY) &&
              (DTCOrigin != DEM_DTC_ORIGIN_PERMANENT_MEMORY) &&
              (DTCOrigin != DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY) )
          )
  {
    DEM_REPORT_ERROR(DEM_SID_SelectDTC, DEM_E_PARAM_DATA);
  }
  /* !LINKSTO Dem.DTCSelector.InvalidSelectionData,1 */
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* reset current selection of FF/ED records, if any */
    Dem_ResetDataRecordSelector(ClientId);

    /* Store DTC related information of the client to internal data structure */
    Dem_SetDTCSelectionCriteria(ClientId,
                                DTC,
                                DTCFormat,
                                DTCOrigin,
                                DEM_DTC_ORIGIN_MAPPING_DCM_IF);
    Result = E_OK;
  }

  DBG_DEM_SELECTDTC_EXIT(Result, ClientId, DTC, DTCFormat, DTCOrigin);
  return Result;
}

/*------------------[Dem_GetDTCSelectionResult]-----------------------------*/

/* !LINKSTO Dem.ASR431.DTCSelectorDependentAPIs,1 */
FUNC(Std_ReturnType, DEM_CODE) Dem_GetDTCSelectionResult(uint8 ClientId)
{
  Std_ReturnType Result;

  DBG_DEM_GETDTCSELECTIONRESULT_ENTRY(ClientId);

  Result = Dem_InternalGetDTCSelectionResult(ClientId,
                                             DEM_SID_GetDTCSelectionResult);

  DBG_DEM_GETDTCSELECTIONRESULT_EXIT(Result, ClientId);
  return Result;
}

/*------------------[Dem_GetDTCSelectionResultForClearDTC]------------------*/

/* !LINKSTO Dem.ASR431.DTCSelectorDependentAPIs,1 */
FUNC(Std_ReturnType, DEM_CODE) Dem_GetDTCSelectionResultForClearDTC(uint8 ClientId)
{
  Std_ReturnType Result;

  DBG_DEM_GETDTCSELECTIONRESULTFORCLEARDTC_ENTRY(ClientId);

  Result = Dem_InternalGetDTCSelectionResult(ClientId,
                                             DEM_SID_GetDTCSelectionResultForClearDTC);

  DBG_DEM_GETDTCSELECTIONRESULTFORCLEARDTC_EXIT(Result, ClientId);
  return Result;
}

/*------------------[Dem_GetDTCSelection]-----------------------------------*/

FUNC(void, DEM_CODE) Dem_GetDTCSelection(
  uint8                                           ClientId,
  P2VAR(Dem_DTCSelectionType, AUTOMATIC, DEM_VAR) DTCSelection)
{
  DBG_DEM_GETDTCSELECTION_ENTRY(ClientId, DTCSelection);

  DTCSelection->DTCInfo.DTC       = Dem_DTCSelection[ClientId].DTCInfo.DTC;
  DTCSelection->DTCInfo.EventId   = Dem_DTCSelection[ClientId].DTCInfo.EventId;
  DTCSelection->DTCInfo.DTCOrigin = Dem_DTCSelection[ClientId].DTCInfo.DTCOrigin;
  DTCSelection->DTCInfo.DTCFormat = Dem_DTCSelection[ClientId].DTCInfo.DTCFormat;
  DTCSelection->SelectorStatus    = Dem_DTCSelection[ClientId].SelectorStatus;

  DBG_DEM_GETDTCSELECTION_EXIT(ClientId, DTCSelection);
}

/*------------------[Dem_SetDTCSelectionCriteria]---------------------------*/

FUNC(void, DEM_CODE) Dem_SetDTCSelectionCriteria(
  uint8                    ClientId,
  uint32                   DTC,
  Dem_DTCFormatType        DTCFormat,
  Dem_DTCOriginType        DTCOrigin,
  Dem_DTCOriginMappingType MappingVariant)
{
  Dem_DTCOriginType MappedDTCOrigin;

  DBG_DEM_SETDTCSELECTIONCRITERIA_ENTRY(
    ClientId, DTC, DTCFormat, DTCOrigin, MappingVariant);

  MappedDTCOrigin = Dem_GetMappedDTCOrigin(DTCOrigin, MappingVariant);

  /* !LINKSTO Dem.ASR431.SWS_Dem_01304,1 */
  /* Check if the selection has been changed */
  if ( (MappingVariant == DEM_DTC_ORIGIN_MAPPING_DCM_IF) ||
       (DTC != Dem_DTCSelection[ClientId].DTCInfo.DTC) ||
       (DTCFormat != Dem_DTCSelection[ClientId].DTCInfo.DTCFormat) ||
       (MappedDTCOrigin != Dem_DTCSelection[ClientId].DTCInfo.DTCOrigin) ||
       (DEM_DTC_SELECTOR_NO_REQUEST ==
         Dem_DTCSelection[ClientId].SelectorStatus) )
  {
    /* Notify that the client is not interested anymore
     * in the result of the clearing operation */
    Dem_NotifyNewDTCSelection(ClientId);

    /* store DTC related information of the client to internal data structure
     * and invalidate previously cached event Id, if any */
    Dem_DTCSelection[ClientId].DTCInfo.DTC       = DTC;
    Dem_DTCSelection[ClientId].DTCInfo.EventId   = DEM_EVENT_ID_INVALID;
    Dem_DTCSelection[ClientId].DTCInfo.DTCOrigin = MappedDTCOrigin;
    Dem_DTCSelection[ClientId].DTCInfo.DTCFormat = DTCFormat;

    /* update selector status to indicate a new DTC selection request */
    Dem_DTCSelection[ClientId].SelectorStatus = DEM_DTC_SELECTOR_NOT_PROCESSED;
  }

  DBG_DEM_SETDTCSELECTIONCRITERIA_EXIT(
    ClientId, DTC, DTCFormat, DTCOrigin, MappingVariant);
}

/*------------------[Dem_DoDTCSelection]------------------------------------*/

FUNC (Dem_SelectorStatusType, DEM_CODE) Dem_DoDTCSelection(
  uint8                                      ClientId,
  P2VAR(Dem_EventIdType, AUTOMATIC, DEM_VAR) EventId)
{
  Dem_SelectorStatusType NewSelectorStatus = Dem_DTCSelection[ClientId].SelectorStatus;

  DBG_DEM_DODTCSELECTION_ENTRY(ClientId, EventId);

  if (NewSelectorStatus == DEM_DTC_SELECTOR_NOT_PROCESSED)
  {
    /* extract DTC related information of client to local variables */
    const uint32            DTC       = Dem_DTCSelection[ClientId].DTCInfo.DTC;
    const Dem_DTCFormatType DTCFormat = Dem_DTCSelection[ClientId].DTCInfo.DTCFormat;
    const Dem_DTCOriginType DTCOrigin = Dem_DTCSelection[ClientId].DTCInfo.DTCOrigin;
    const uint8             OriginIdx = (uint8)(DTCOrigin - DEM_DTC_ORIGIN_PRIMARY_MEMORY);
    uint8 GroupIdx;

    /* check the DTC format */
#if (DEM_OBD_Support == STD_ON)
    if ( (DTCFormat != DEM_DTC_FORMAT_OBD) &&
         (DTCFormat != DEM_DTC_FORMAT_UDS) )
#else
    if (DTCFormat != DEM_DTC_FORMAT_UDS)
#endif
    {
      NewSelectorStatus = DEM_DTC_SELECTOR_WRONG_DTC;
    }
#if (DEM_EMISSION_REL_DTCS_GROUP_EXIST == STD_OFF)
    else if (DTC == DEM_DTC_GROUP_EMISSION_REL_DTCS)
    {
      NewSelectorStatus = DEM_DTC_SELECTOR_WRONG_DTC;
    }
#endif
    /* check the DTC origin */
    else if ( (DTCOrigin != DEM_DTC_ORIGIN_PRIMARY_MEMORY) &&
         (DTCOrigin != DEM_DTC_ORIGIN_MIRROR_MEMORY) &&
         (DTCOrigin != DEM_DTC_ORIGIN_PERMANENT_MEMORY) &&
         (DTCOrigin != DEM_DTC_ORIGIN_SECONDARY_MEMORY) )
    {
      NewSelectorStatus = DEM_DTC_SELECTOR_WRONG_DTC_ORIGIN;
    }
    /* check if DTC origin is supported */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER == 0U)
    else if (Dem_SizeEventMem[OriginIdx] == (Dem_SizeEvMemEntryType) 0U)
#else
    else if ( (Dem_SizeEventMem[OriginIdx] == (Dem_SizeEvMemEntryType) 0U) &&
              (DTCOrigin != DEM_DTC_ORIGIN_PERMANENT_MEMORY) )
#endif
    {
      NewSelectorStatus = DEM_DTC_SELECTOR_WRONG_DTC_ORIGIN;
    }
    else if (Dem_IsDTCGroupValue(DTC, DTCFormat, &GroupIdx) != FALSE)
    {
      NewSelectorStatus = DEM_DTC_SELECTOR_DTC_GROUP;
    }
#if (DEM_OBD_Support == STD_ON)
    else if (DTCFormat == DEM_DTC_FORMAT_OBD)
    {
      /* selection of a single DTC in OBD format is not supported by any
       * dependent APIs of Dem_SelectDTC() */
      NewSelectorStatus = DEM_DTC_SELECTOR_WRONG_DTC;
    }
#endif
    else /* single DTC in UDS format requested */
    {
      Dem_EventIdType EvId;

      /* presume that no matching event will be found */
      NewSelectorStatus = DEM_DTC_SELECTOR_WRONG_DTC;

      /* Deviation TASKING-2 */
      for (EvId = 1U; EvId <= DEM_MAX_EVENTID; EvId++)
      {
        if (DTC == Dem_GetVisibleUdsDTC(EvId))
        {
          /* check DTC origin */
          if (DTCOrigin == Dem_GbiDTCOrigin(EvId))
          {
            /* store the found EvId in Dem_DTCSelection[] for the client */
            Dem_DTCSelection[ClientId].DTCInfo.EventId = EvId;
            NewSelectorStatus = DEM_DTC_SELECTOR_SINGLE_DTC_EVID_CACHED;
          }
          /* else: DTC origin does not match, also applies for all other
           * events having same UDS DTC in case of event combination.
           * Selector status remains DEM_DTC_SELECTOR_WRONG_DTC */

          break;
        }
      }
    }

    /* update the new selector status for the client */
    Dem_DTCSelection[ClientId].SelectorStatus = NewSelectorStatus;
  }

  *EventId = Dem_DTCSelection[ClientId].DTCInfo.EventId;

  DBG_DEM_DODTCSELECTION_EXIT(NewSelectorStatus, ClientId, EventId);
  return NewSelectorStatus;
}

/*------------------[Dem_IsDTCGroupId]--------------------------------------*/

FUNC(boolean, DEM_CODE) Dem_IsDTCGroupValue(
  uint32                             DTC,
  Dem_DTCFormatType                  DTCFormat,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) GroupIdx)
{
  boolean Result = FALSE;

  DBG_DEM_ISDTCGROUPVALUE_ENTRY(DTC, DTCFormat, GroupIdx);

  if (((Dem_DTCGroupType)DTC == DEM_DTC_GROUP_ALL_DTCS) ||
      (DTC == DEM_DTC_GROUP_EMISSION_REL_DTCS)) /* emission related group requested */
  {
    Result = TRUE;
  }
#if (DEM_NUM_DTC_GROUPS > 0U)
  /* check and get configurable GroupIdx */
  else
  {
    if (DTCFormat == DEM_DTC_FORMAT_UDS)
    {
      Result = Dem_GetIdxOfDTCGroup((Dem_DTCGroupType) DTC, GroupIdx);
    }
  }
#else
  TS_PARAM_UNUSED(DTCFormat);
  TS_PARAM_UNUSED(GroupIdx);
#endif /* (DEM_NUM_DTC_GROUPS > 0U) */

  DBG_DEM_ISDTCGROUPVALUE_EXIT(Result, DTC, DTCFormat, GroupIdx);
  return Result;
}

/*==================[internal function definitions]=========================*/

/*------------------[Dem_InternalClearDTC]----------------------------------*/

STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_InternalClearDTC(
  uint8 ClientId)
{
  Std_ReturnType Result = E_NOT_OK;
  Dem_DTCSelectionType DTCSelection;
  uint8 OriginIdx;

  DBG_DEM_INTERNALCLEARDTC_ENTRY(ClientId);

  Dem_GetDTCSelection(ClientId, &DTCSelection);
  /* Get the index of memory origin for checking its size */
  OriginIdx = (uint8)(DTCSelection.DTCInfo.DTCOrigin - DEM_DTC_ORIGIN_PRIMARY_MEMORY);
  /* Check if the DTCOrigin is invalid */
  /* Request shall be rejected also in case permanent memory origin is requested */
  /* !LINKSTO Dem_OBD_0070,2 */
  if ( (DTCSelection.DTCInfo.DTCOrigin != DEM_DTC_ORIGIN_PRIMARY_MEMORY) &&
       (DTCSelection.DTCInfo.DTCOrigin != DEM_DTC_ORIGIN_MIRROR_MEMORY) &&
       (DTCSelection.DTCInfo.DTCOrigin != DEM_DTC_ORIGIN_SECONDARY_MEMORY) )
  {
    Result = DEM_WRONG_DTCORIGIN;
  }
  else if (Dem_SizeEventMem[OriginIdx] == (Dem_SizeEvMemEntryType) 0U)
  {
    Result = DEM_WRONG_DTCORIGIN;
  }
#if (DEM_EMISSION_REL_DTCS_GROUP_EXIST == STD_OFF)
  else if (DTCSelection.DTCInfo.DTC == DEM_DTC_GROUP_EMISSION_REL_DTCS)
  {
    Result = DEM_WRONG_DTC;
  }
#endif
  /* Check if the DTCFormat is invalid */
  /* !LINKSTO Dem_OBD_0001,2 */
#if (DEM_OBD_Support == STD_ON)
  else if ( (DTCSelection.DTCInfo.DTCFormat != DEM_DTC_FORMAT_OBD) &&
            (DTCSelection.DTCInfo.DTCFormat != DEM_DTC_FORMAT_UDS) )
#else
  else if (DTCSelection.DTCInfo.DTCFormat != DEM_DTC_FORMAT_UDS)
#endif
  {
    Result = DEM_WRONG_DTC;
  }
  else
  {
    switch (Dem_ClearDtcAsyncStatus.RunState)
    {
      case DEM_CLEARDTC_STATE_PROCESSING_RAM:
        /* report PENDING if called by the same client for the same DTC
         * selection as the current clearing operation, otherwise BUSY */
        if ( (ClientId == Dem_ClearDtcAsyncStatus.ClientId) &&
             (FALSE == Dem_NewDTCSelection) )
        {
          /* !LINKSTO SWS_Dem_00663,1 */
          /* report Result PENDING */
          Result = DEM_PENDING;
        }
        else
        {
          /* !LINKSTO Dem.ASR431.SWS_Dem_00664,1 */
          /* !LINKSTO SWS_Dem_00662,1 */
          /* report Result BUSY */
          Result = DEM_CLEAR_BUSY;
        }
        break;
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
      case DEM_CLEARDTC_STATE_PROCESSING_NVRAM:
      case DEM_CLEARDTC_STATE_FAILED:
#endif
      case DEM_CLEARDTC_STATE_FINISHED:
        if (ClientId == Dem_ClearDtcAsyncStatus.ClientId)
        {
          /* !LINKSTO SWS_Dem_00663,1 */
          /* Result may be E_OK, DEM_PENDING, DEM_WRONG_DTC or DEM_CLEAR_MEMORY_ERROR as propagated
           * from Dem_ClearEvents() or Dem_TriggerNvmWrite() */
          Result = Dem_ClearDtcAsyncStatus.OperationResult;
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
          if (DEM_CLEARDTC_STATE_PROCESSING_NVRAM != Dem_ClearDtcAsyncStatus.RunState)
#endif
          {
            /* !LINKSTO Dem.ASR431.SWS_Dem_01042,1 */
            /* set the state back to IDLE only for FAILED or FINISHED states */
            Dem_ClearDtcAsyncStatus.RunState = DEM_CLEARDTC_STATE_IDLE;
          }
        }
        else
        {
          /* !LINKSTO Dem.ASR431.SWS_Dem_00664,1 */
          /* !LINKSTO SWS_Dem_00662,1 */
          /* report Result BUSY */
          Result = DEM_CLEAR_BUSY;
        }
        break;
      case DEM_CLEARDTC_STATE_IDLE:

        /* Dem_StartClearDTC() may return DEM_PENDING or DEM_BUSY */
        Result = Dem_StartClearDTC(ClientId);
        break;
      /* CHECK: NOPARSE */
      default:
        DEM_UNREACHABLE_CODE_ASSERT(DEM_SID_ClearDTC);
        break;
      /* CHECK: PARSE */
    }
  }

  DBG_DEM_INTERNALCLEARDTC_EXIT(Result, ClientId);
  return Result;
}

/*------------------[Dem_StartClearDTC]-------------------------------------*/

STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_StartClearDTC(
  uint8 ClientId)
{
  Std_ReturnType Result = DEM_PENDING;

  DBG_DEM_STARTCLEARDTC_ENTRY(ClientId);

  /*
   * ENTER critical section to protect RunState RMW (client lock)
   *       call-context: Dcm (Task), Rte (Task)
   */
  DEM_ENTER_EXCLUSIVE_AREA();

  if (DEM_CLEARDTC_STATE_IDLE == Dem_ClearDtcAsyncStatus.RunState)
  {
    /* set the OperationResult to be reported by InternalClearDTC() */
    Dem_ClearDtcAsyncStatus.OperationResult = Result;

    /* !LINKSTO Dem.ASR431.SWS_Dem_00661,1 */
    Dem_ClearDtcAsyncStatus.RunState = DEM_CLEARDTC_STATE_PROCESSING_RAM;

    /* ClientId which successfully acquired ClearDTC lock */
    Dem_ClearDtcAsyncStatus.ClientId = ClientId;

    /* update flag as a new client successfully acquired ClearDTC lock */
    Dem_NewDTCSelection = FALSE;

    /* make a copy of DTC selection for further processing
     * in case operation is cancelled */
    Dem_ClearDTCSelection = Dem_DTCSelection[ClientId];

    /* initialize ClearDTCAsyncStatus */
    Dem_ClearDtcAsyncStatus.AnyEventModified.EvSt = FALSE;
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
    Dem_ClearDtcAsyncStatus.AnyEventModified.EvMemEntry = FALSE;
#endif
    Dem_ClearDtcAsyncStatus.NextEventId = 1U;
#if (DEM_NUM_CMB_DTCS > 0U)
    Dem_ClearDtcAsyncStatus.NextCmbEvIdx = 0U;
#endif

#if ( (DEM_OBD_CENTRALIZED_PID31_HANDLING == STD_ON) || \
      (DEM_OBD_CENTRALIZED_PID21_HANDLING == STD_ON) || \
      (defined DEM_OPCYC_WARMUP_ID) )
    /* reset counters when service 'ClearDTC' is requested
     * via external tester for all DTCs */
    if (DEM_DTC_GROUP_ALL_DTCS == Dem_ClearDTCSelection.DTCInfo.DTC)
    {
      Dem_ResetOBDCounters();
    }
#endif
  }
  else
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_00664,1 */
    /* !LINKSTO SWS_Dem_00662,1 */
    /* report Result BUSY */
    Result = DEM_CLEAR_BUSY;
  }

  /*
   * LEAVE critical section
   */
  DEM_EXIT_EXCLUSIVE_AREA();

  DBG_DEM_STARTCLEARDTC_EXIT(Result, ClientId);
  return Result;
}

/*------------------[Dem_NotifyNewDTCSelection]-----------------------------*/

STATIC FUNC(void, DEM_CODE) Dem_NotifyNewDTCSelection(uint8 ClientId)
{
  DBG_DEM_NOTIFYNEWDTCSELECTION_ENTRY(ClientId);

  if (DEM_CLEARDTC_STATE_IDLE != Dem_ClearDtcAsyncStatus.RunState)
  {
    /*
     * ENTER critical section to protect RunState RMW (client lock)
     *       call-context: Dcm (Task), Rte (Task)
     */
    DEM_ENTER_EXCLUSIVE_AREA();

    if (ClientId == Dem_ClearDtcAsyncStatus.ClientId)
    {
      /* !LINKSTO Dem.ASR431.SWS_Dem_01304,1 */
      /* Dem_NotifyNewDTCSelection notifies that the client is not interested anymore
       * in the result of the clearing operation
       * RAM clearing operation will continue asynchronously
       * Immediate storage mechanism can handle data management in the
       * background including subsequent ClearDTC requests and event status
       * reports */
      Dem_NewDTCSelection = TRUE;

#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
      if ( (DEM_CLEARDTC_STATE_PROCESSING_NVRAM == Dem_ClearDtcAsyncStatus.RunState) ||
           (DEM_CLEARDTC_STATE_FAILED == Dem_ClearDtcAsyncStatus.RunState) ||
           (DEM_CLEARDTC_STATE_FINISHED == Dem_ClearDtcAsyncStatus.RunState) )
#else
      if (DEM_CLEARDTC_STATE_FINISHED == Dem_ClearDtcAsyncStatus.RunState)
#endif
      {
        /* reset the ClientId to release lock of the clearing process */
        Dem_ClearDtcAsyncStatus.ClientId = DEM_CLIENT_ID_INVALID;

        /* !LINKSTO Dem.ASR431.SWS_Dem_01042,1 */
        /* set the state back to IDLE/release the lock */
        Dem_ClearDtcAsyncStatus.RunState = DEM_CLEARDTC_STATE_IDLE;
      }
    }

    /*
    * LEAVE critical section
    */
    DEM_EXIT_EXCLUSIVE_AREA();
  }

  DBG_DEM_NOTIFYNEWDTCSELECTION_EXIT(ClientId);
}

/*------------------[Dem_ResetOBDCounters]----------------------------------*/

#if ( (DEM_OBD_CENTRALIZED_PID31_HANDLING == STD_ON) || \
      (DEM_OBD_CENTRALIZED_PID21_HANDLING == STD_ON) || \
      (defined DEM_OPCYC_WARMUP_ID) )
STATIC FUNC(void, DEM_CODE) Dem_ResetOBDCounters(void)
{
  DBG_DEM_RESETOBDCOUNTERS_ENTRY();

#if (DEM_OBD_CENTRALIZED_PID31_HANDLING == STD_ON)
    /* Clear the counter for the distance traveled. The counter
     * can be read via PID $31 */
    Dem_NvData.TicksTraveledSinceDTCsCleared = 0U;
#endif

#if (DEM_OBD_CENTRALIZED_PID21_HANDLING == STD_ON)
  /* !LINKSTO Dem.OBD.PID$21.DistanceTraveled.ResetClearDiagInfo,1 */
  /* Check if was a Dcm service $04 request – Clear/reset emission-related
   * diagnostic information */
  if ((Dem_ClearDTCSelection.DTCInfo.DTCFormat == DEM_DTC_FORMAT_OBD) &&
      (Dem_ClearDTCSelection.DTCInfo.DTCOrigin == DEM_DTC_ORIGIN_PRIMARY_MEMORY))
  {
    Dem_NvData.TicksTraveledWhileMILIsActivated = 0U;
  }
#endif  /* DEM_OBD_CENTRALIZED_PID21_HANDLING */

#if (defined DEM_OPCYC_WARMUP_ID)
    /* Clear the warm-up cycle counter. The counter
     * can be read via PID $30 */
    Dem_NvData.OBDWarmupCycleCtr = 0U;
#endif

  DBG_DEM_RESETOBDCOUNTERS_EXIT();
}
#endif /* (DEM_OBD_CENTRALIZED_PID31_HANDLING == STD_ON) ||
          (DEM_OBD_CENTRALIZED_PID21_HANDLING == STD_ON) ||
          (defined DEM_OPCYC_WARMUP_ID) */

/*------------------[Dem_InternalGetDTCSelectionResult]---------------------*/

STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_InternalGetDTCSelectionResult(
  uint8 ClientId,
  uint8 ServiceId)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_INTERNALGETDTCSELECTIONRESULT_ENTRY(ClientId, ServiceId);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(ServiceId, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(ServiceId, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (ClientId > DEM_MAX_CLIENTID)
  {
    /* !LINKSTO Dem.ASR431.SWS_Dem_01252,1 */
    DEM_REPORT_ERROR(ServiceId, DEM_E_WRONG_CONFIGURATION);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    Dem_DTCSelectionType DTCSelection;
    Dem_GetDTCSelection(ClientId, &DTCSelection);
#if (DEM_DEV_ERROR_DETECT == STD_ON)
    /* check if Dem_SelectDTC() was called before calling the API
     * Dem_GetDTCSelectionResult() / Dem_GetDTCSelectionResultForClearDTC() */
    if (DTCSelection.SelectorStatus == DEM_DTC_SELECTOR_NO_REQUEST)
    {
      /* !LINKSTO Dem.ASR431.SWS_Dem_01299,1 */
      DEM_REPORT_ERROR(ServiceId, DEM_E_WRONG_CONDITION);
    }
    else
#endif /* DEM_DEV_ERROR_DETECT */
    {
      /* !LINKSTO Dem.ASR431.GetDTCSelectionResultForClearDTC.APIBehavior,1 */
      if ((DEM_SID_GetDTCSelectionResultForClearDTC == ServiceId) &&
          (DEM_DTC_ORIGIN_PERMANENT_MEMORY == DTCSelection.DTCInfo.DTCOrigin))
      {
        /* clearing DTCs from permanent memory is not allowed */
        Result = DEM_WRONG_DTCORIGIN;
      }
      else
      {
        Dem_EventIdType EventId;
        switch (Dem_DoDTCSelection(ClientId, &EventId))
        {
          case DEM_DTC_SELECTOR_DTC_GROUP:
          case DEM_DTC_SELECTOR_SINGLE_DTC_EVID_CACHED:
            /* !LINKSTO Dem.ASR431.SWS_Dem_01256,1 */
            Result = E_OK;
            break;
          case DEM_DTC_SELECTOR_WRONG_DTC:
            /* !LINKSTO Dem.ASR431.SWS_Dem_01257,1 */
            /* !LINKSTO Dem.ASR431.DTCSelectorWrongDTC,1 */
            /* !LINKSTO Dem.DTCSelector.DependentAPIs.SupportedDTCFormat,1 */
            Result = DEM_WRONG_DTC;
            break;
          case DEM_DTC_SELECTOR_WRONG_DTC_ORIGIN:
            /* !LINKSTO Dem.ASR431.SWS_Dem_01262,1 */
            /* !LINKSTO Dem.ASR431.SWS_Dem_01258,1 */
            Result = DEM_WRONG_DTCORIGIN;
            break;
          /* CHECK: NOPARSE */
          default: /* should never be reached */
            DEM_UNREACHABLE_CODE_ASSERT(ServiceId);
            break;
          /* CHECK: PARSE */
        }
      }
    }
  }

  DBG_DEM_INTERNALGETDTCSELECTIONRESULT_EXIT(Result, ClientId, ServiceId);
  return Result;
}

/*------------------[Dem_CheckForOldClearImmediateEntries]------------------*/

#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
STATIC FUNC(boolean, DEM_CODE) Dem_CheckForOldClearImmediateEntries(
  uint8 OriginIdx)
{
  boolean Result = FALSE;
  Dem_SizeEvMemEntryType EventEntryIdx;
  const Dem_EventMemoryEntryPtrType EventMem = Dem_EventMem[OriginIdx];
  const Dem_SizeEvMemEntryType SizeEventMem = Dem_SizeEventMem[OriginIdx];

  DBG_DEM_CHECKFOROLDCLEARIMMEDIATEENTRIES_ENTRY(OriginIdx);

  /*
   * ENTER critical section to protect the event memory
   *       call-context: Dcm (Task)
   */
  DEM_ENTER_EXCLUSIVE_AREA();

  /* search for entries that were scheduled to be cleared by earlier
   * ClearDTC request(s) */
  for (EventEntryIdx = 0U; EventEntryIdx < SizeEventMem; ++EventEntryIdx)
  {
    /* check if entry was scheduled to be cleared earlier */
    if (EventMem[EventEntryIdx].EntryStatus == DEM_ENTRY_CLEAR_IMMEDIATELY)
    {
      Dem_ClearDtcAsyncStatus.OriginIdx = OriginIdx;
      Dem_ClearDtcAsyncStatus.EventMemWrite = DEM_NVM_WAITING;

      Result = TRUE;

      /* earlier scheduled entry found: stop searching */
      break;
    }
  }

  /*
   * LEAVE critical section
   */
  DEM_EXIT_EXCLUSIVE_AREA();

  DBG_DEM_CHECKFOROLDCLEARIMMEDIATEENTRIES_EXIT(Result, OriginIdx);
  return Result;
}
#endif /* DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH */

/*------------------[Dem_ClearEvents]---------------------------------------*/

STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_ClearEvents(
  CONSTP2VAR(Dem_EventIdType, AUTOMATIC, DEM_VAR_CLEARED)     NextEventId,
#if (DEM_NUM_CMB_DTCS > 0U)
  CONSTP2VAR(Dem_CmbDTCEvLnkType, AUTOMATIC, DEM_VAR_CLEARED) NextCmbEvIdx,
#if (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON)
  CONSTP2VAR(Dem_ClearEventAllowedType, AUTOMATIC, AUTOMATIC) ClearEventAllowed,
#endif
#endif
  CONSTP2VAR(Dem_EventModifiedType, AUTOMATIC, AUTOMATIC)     AnyEventModified)
{
  /* Result is initially set to E_OK, the same value as in Dem_StartClearDTC() */
  Std_ReturnType Result = E_OK;
  const uint8 OriginIdx =
    (uint8)(Dem_ClearDTCSelection.DTCInfo.DTCOrigin - DEM_DTC_ORIGIN_PRIMARY_MEMORY);
  const uint32 DTC = Dem_ClearDTCSelection.DTCInfo.DTC;
  const Dem_DTCFormatType DTCFormat  = Dem_ClearDTCSelection.DTCInfo.DTCFormat;
  uint8 GroupIdx = DEM_DTCGRP_IDX_INVALID;
  boolean DtcGroupRequest;
  /* !LINKSTO VCC_DEM_006_Req145v1,1 */
#if ( (DEM_EVENT_DISABLING_SUPPORT == STD_ON) && (DEM_CLEAR_DTC_OF_DISABLED_EVENTS == STD_ON) )
  const boolean ForcedDelete =
    ((Dem_DTCGroupType) DTC == DEM_DTC_GROUP_ALL_DTCS) ? TRUE : FALSE;
#else
  const boolean ForcedDelete = FALSE;
#endif

  Dem_EventIdType EventId = *NextEventId;
  uint16 RemainingEventsToProcess = DEM_MAX_NUM_CLEAR_EVENTS_PER_CYCLE;
  const uint16 LastEventId =
    ((EventId + DEM_MAX_NUM_CHECK_CLEAR_EVENTS_PER_CYCLE) <= DEM_MAX_EVENTID) ?
    (EventId + DEM_MAX_NUM_CHECK_CLEAR_EVENTS_PER_CYCLE - 1U) : DEM_MAX_EVENTID;

#if ( (DEM_NUM_CMB_DTCS > 0U) && \
      (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON) )
  DBG_DEM_CLEAREVENTS_ENTRY(NextEventId, NextCmbEvIdx, ClearEventAllowed, AnyEventModified);
#elif (DEM_NUM_CMB_DTCS > 0U)
  DBG_DEM_CLEAREVENTS_ENTRY(NextEventId, NextCmbEvIdx, AnyEventModified);
#else
  DBG_DEM_CLEAREVENTS_ENTRY(NextEventId, AnyEventModified);
#endif

  DtcGroupRequest = Dem_IsDTCGroupClear(DTC, DTCFormat, &GroupIdx);

  for (; EventId <= LastEventId; ++EventId)
  {
    Std_ReturnType RetVal;

    /* !LINKSTO Dem.ASR431.SWS_Dem_01203,1 */
    /* perform checks for availability, suppression, DTC match with single/group, etc. */
#if ((DEM_J1939_SUPPORT == STD_ON) && (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON))
    if (DEM_VIRTUAL_CLIENT_J1939 == Dem_ClearDtcAsyncStatus.ClientId)
    {
      RetVal = Dem_J1939CheckDeleteEvent(EventId);
    }
    else
#endif
    {
      RetVal = Dem_CheckDeleteEvent(DtcGroupRequest, GroupIdx,
                                    EventId, ForcedDelete);
    }

    if (RetVal == E_OK)
    {
#if (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON)
      boolean Allowed = TRUE;
      uint16 Behavior = DEM_NO_STATUS_BYTE_CHANGE;
#if (DEM_NUM_CMB_DTCS > 0U)
      /* check for ClearEventAllowed only on first call of Dem_ClearEvents */
      /* if NextCmbEvIdx > 0, Allowed was already TRUE on first call */
      if (0U == *NextCmbEvIdx)
      {
        /* !LINKSTO SWS_Dem_00515,1 */
        RetVal = Dem_CbClearEventAllowed(EventId, &Behavior, &Allowed);
        /* store result of ClearEventAllowed for the next call of Dem_ClearEvents */
        ClearEventAllowed->Behavior = Behavior;
        ClearEventAllowed->Allowed = Allowed;
      }
      else
      {
        Behavior = ClearEventAllowed->Behavior;
        Allowed = ClearEventAllowed->Allowed;
      }
#else
      /* !LINKSTO SWS_Dem_00515,1 */
      RetVal = Dem_CbClearEventAllowed(EventId, &Behavior, &Allowed);
#endif /* (DEM_NUM_CMB_DTCS > 0U) */
      /* !LINKSTO Dem516,1 */
      if ( (RetVal != E_OK) || ( Allowed == TRUE) )
#endif /* (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON) */
      {
#if (DEM_NUM_CMB_DTCS > 0U)
        if (DEM_IS_EVENT_COMBINED(EventId))
        {
          /* clear events combined to this DTC */
          Result = Dem_ClearCmbEvents(EventId,
                                      ForcedDelete,
                                      &RemainingEventsToProcess,
                                      NextCmbEvIdx,
                                      AnyEventModified);
        }
        else
#endif
        {
          Dem_ClearEvent(EventId, AnyEventModified);
          RemainingEventsToProcess--;
        }
      }
#if (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON)
      else
      {
        /* !LINKSTO SWS_Dem_00668,1 */
        /* !LINKSTO SWS_Dem_00667,1 */
        if (DEM_ONLY_THIS_CYCLE_AND_READINESS == Behavior)
        {
          /* !LINKSTO SWS_Dem_00669,1 */
#if (DEM_NUM_CMB_DTCS > 0U)
          if (DEM_IS_EVENT_COMBINED(EventId))
          {
            Result =
              Dem_ClearThisCycleAndReadinessStatusCmbEvents(EventId,
                                                            NextCmbEvIdx,
                                                            &RemainingEventsToProcess);
          }
          else
#endif
          {
            Dem_ClearThisCycleAndReadinessStatus(EventId);
            RemainingEventsToProcess--;
          }
        }
      }
#endif /* (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON) */

      /* !LINKSTO Dem.ClearDTCBurstReduction.ClearEventLimit,1 */
      /* suspend clearing if there is an event left to process but
       * the limit is reached */
      if ( (0U == RemainingEventsToProcess) &&
           (TRUE == DtcGroupRequest) &&
           (EventId < DEM_MAX_EVENTID) )
      {
        /* suspend processing by setting Result to DEM_PENDING */
        Result = DEM_PENDING;
      }

      if ( (FALSE == DtcGroupRequest) || (DEM_PENDING == Result) )
      {
        /* event for single DTC found or limit reached - stop searching */
        /* Result remains E_OK or DEM_PENDING */
        break;
      } /* else: group of DTC -> continue searching */
    } /* else: skip event since not all checks for clear event passed */
  }

  if (EventId > DEM_MAX_EVENTID)
  {
    /* check if a single DTC was not found */
    if (FALSE == DtcGroupRequest)
    {
      /* single DTC not found, wrong DTC */
      Result = DEM_WRONG_DTC;
    } /* else: status already set to E_OK */
  }
  else
  {
    if (EventId > LastEventId)
    {
      /* reached check limit */
      Result = DEM_PENDING;
      EventId = LastEventId;
    }

    if (DEM_PENDING == Result)
    {
#if (DEM_NUM_CMB_DTCS > 0U)
      /* only increment the (master) EventId when all combined events are cleared */
      if ( !DEM_IS_EVENT_COMBINED(EventId) || (*NextCmbEvIdx == 0U) )
#endif
      {
        EventId++;
      }
      /* store the next EventId to clear */
      *NextEventId = EventId;
    }
  }

  if (E_OK == Result)
  {
    /* clear overflow indication status of all event memories */
    if (DEM_DTC_GROUP_ALL_DTCS == (Dem_DTCGroupType)DTC)
    {
      DEM_ATOMIC_CLR_BIT_8(DEM_OVFIND_FLAGS, OriginIdx);
    }
  }

#if ( (DEM_NUM_CMB_DTCS > 0U) && \
      (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON) )
  DBG_DEM_CLEAREVENTS_EXIT(Result, NextEventId, NextCmbEvIdx, ClearEventAllowed, AnyEventModified);
#elif (DEM_NUM_CMB_DTCS > 0U)
  DBG_DEM_CLEAREVENTS_EXIT(Result, NextEventId, NextCmbEvIdx, AnyEventModified);
#else
  DBG_DEM_CLEAREVENTS_EXIT(Result, NextEventId, AnyEventModified);
#endif

  return Result;
}

/*------------------[Dem_ClearCmbEvents]------------------------------------*/

#if (DEM_NUM_CMB_DTCS > 0U)
STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_ClearCmbEvents(
  Dem_EventIdType                                             EventId,
  boolean                                                     ForcedDelete,
  CONSTP2VAR(uint16, AUTOMATIC, AUTOMATIC)                    RemainingEventsToProcess,
  CONSTP2VAR(Dem_CmbDTCEvLnkType, AUTOMATIC, DEM_VAR_CLEARED) NextCmbEvIdx,
  CONSTP2VAR(Dem_EventModifiedType, AUTOMATIC, AUTOMATIC)     AnyEventModified)
{
  Std_ReturnType Result = DEM_PENDING;
  const Dem_DTCConfIdxType DTCConfIdx = Dem_GbiDTCConfIdx(EventId);
  const uint16 NumCmbEvents =
    (uint16)(Dem_CmbDTCEvLnk[DTCConfIdx + 1U] - Dem_CmbDTCEvLnk[DTCConfIdx]);
  Dem_CmbDTCEvLnkType CmbEvIdx = Dem_CmbDTCEvLnk[DTCConfIdx];
  const uint8 FractionalNumEvents = NumCmbEvents % DEM_MAX_NUM_CLEAR_EVENTS_PER_CYCLE;
#if (DEM_EVENT_DISABLING_SUPPORT == STD_OFF)
  TS_PARAM_UNUSED(ForcedDelete);
#endif

  DBG_DEM_CLEARCMBEVENTS_ENTRY(
    EventId, ForcedDelete, RemainingEventsToProcess, NextCmbEvIdx, AnyEventModified);

  /* Deviation TASKING-1 */
  if ( (0U == *NextCmbEvIdx) &&
       ( (*RemainingEventsToProcess < FractionalNumEvents) ||
         ( (FractionalNumEvents == 0U) &&
           (*RemainingEventsToProcess != DEM_MAX_NUM_CLEAR_EVENTS_PER_CYCLE) )
       )
     )
  {
    /* !LINKSTO dsn.Dem.ClearDTCBurstReduction.EventCombinationMinimalCycles,1 */
    /* suspend, because the fractional portion could be handled in a single cycle,
     * but not in this cycle or
     * all combined events could be cleared in a single cycle, but not in this one. */
     *RemainingEventsToProcess = 0U;
     /* *NextCmbEvIdx must be set to a value different than 0 to avoid that
      * Dem_ClearEvents() continues with the processing of the next EventId */
     *NextCmbEvIdx = DEM_NUM_CMB_EVENTS;
  }
  else
  {
    const Dem_CmbDTCEvLnkType LastCmbEvIdx = Dem_CmbDTCEvLnk[DTCConfIdx + 1U] - 1U;

    if ( (0U != *NextCmbEvIdx) && (DEM_NUM_CMB_EVENTS != *NextCmbEvIdx) )
    {
      CmbEvIdx = *NextCmbEvIdx;
    }

    for (; CmbEvIdx <= LastCmbEvIdx; CmbEvIdx++)
    {
      Dem_EventIdType CurrentEventId = Dem_CmbDTCEvents[CmbEvIdx];
#if (DEM_EVENT_DISABLING_SUPPORT == STD_ON)
      boolean IsEventEnabled = Dem_GbiEvAvailable(CurrentEventId);
      if ( (ForcedDelete == TRUE) || (IsEventEnabled == TRUE) )
#endif
      {
        Dem_ClearEvent(CurrentEventId, AnyEventModified);

        /* update the combined status */
        Dem_UpdateCmbDTCStatus(EventId);

        /* !LINKSTO Dem.ClearDTCBurstReduction.ClearEventLimit,1 */
        (*RemainingEventsToProcess)--;
        if (0U == *RemainingEventsToProcess)
        {
          /* configured limit of cleared events reached */
          break;
        }
      }
    }

    if (CmbEvIdx < LastCmbEvIdx)
    {
      /* suspend event clearing */
      *NextCmbEvIdx = CmbEvIdx + 1U;
    }
    else
    {
      /* done with combined event clearing of this EventId */
      *NextCmbEvIdx = 0U;
      Result = E_OK;
    }
  }

  DBG_DEM_CLEARCMBEVENTS_EXIT(Result, EventId, ForcedDelete, RemainingEventsToProcess,
    NextCmbEvIdx, AnyEventModified);
  return Result;
}
#endif /* (DEM_NUM_CMB_DTCS > 0U) */

/*------------------[Dem_ClearThisCycleAndReadinessStatus]------------------*/

#if (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON)
STATIC FUNC(void, DEM_CODE) Dem_ClearThisCycleAndReadinessStatus(
  Dem_EventIdType EventId)
{
  DBG_DEM_CLEARTHISCYCLEANDREADINESSSTATUS_ENTRY(EventId);

#if (DEM_EVENT_DISABLING_SUPPORT == STD_ON)
  if (Dem_GbiEvAvailable(EventId) == TRUE)
#endif
#if (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON)
  {
    Dem_DTCStatusMaskType OldDTCStatusMask = 0U;
    Dem_DTCStatusMaskType NewDTCStatusMask = 0U;

    /*
     * ENTER critical section to protect the event memory
     *       call-context: Dcm (Task)
     */
    DEM_ENTER_EXCLUSIVE_AREA();

    /* get old status mask */
    OldDTCStatusMask = DEM_DTC_STATUS_MASKS[EventId];
    NewDTCStatusMask = OldDTCStatusMask;
    /* reset bits */
    DEM_SET_MASK(NewDTCStatusMask, (DEM_UDS_STATUS_TNCSLC | DEM_UDS_STATUS_TNCTOC));
    DEM_CLR_MASK_8(NewDTCStatusMask, (DEM_UDS_STATUS_TFSLC | DEM_UDS_STATUS_TFTOC));
    /* set new status */
    DEM_DTC_STATUS_MASKS[EventId] = NewDTCStatusMask;

    /*
     * LEAVE critical section
     */
    DEM_EXIT_EXCLUSIVE_AREA();

    /* report event change by callback */
    if (OldDTCStatusMask != NewDTCStatusMask)
    {
      /* notify application that event status has changed */
      Dem_CbTriggerOnEventStatus(EventId, OldDTCStatusMask, NewDTCStatusMask);
    }
  }
#else
  {
    /* reset bits */
    DEM_ATOMIC_CHNG_EV_ST_MASK(EventId,
                               (DEM_UDS_STATUS_TNCSLC | DEM_UDS_STATUS_TNCTOC),
                               (DEM_UDS_STATUS_TFSLC | DEM_UDS_STATUS_TFTOC));
  }
#endif /* (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON) */

  DBG_DEM_CLEARTHISCYCLEANDREADINESSSTATUS_EXIT(EventId);
}

/*--------------[Dem_ClearThisCycleAndReadinessStatusCmbEvents]-------------*/

#if (DEM_NUM_CMB_DTCS > 0U)
STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_ClearThisCycleAndReadinessStatusCmbEvents(
  Dem_EventIdType                                             EventId,
  CONSTP2VAR(Dem_CmbDTCEvLnkType, AUTOMATIC, DEM_VAR_CLEARED) NextCmbEvIdx,
  CONSTP2VAR(uint16, AUTOMATIC, AUTOMATIC)                    RemainingEventsToProcess)
{
  Std_ReturnType Result = E_OK;
  const Dem_DTCConfIdxType DTCConfIdx = Dem_GbiDTCConfIdx(EventId);
  const Dem_CmbDTCEvLnkType LastCmbEvIdx = Dem_CmbDTCEvLnk[DTCConfIdx + 1U] - 1U;
  Dem_CmbDTCEvLnkType CmbEvIdx = Dem_CmbDTCEvLnk[DTCConfIdx];

  DBG_DEM_CLEARTHISCYCLEANDREADINESSSTATUSCMBEVENTS_ENTRY(EventId, NextCmbEvIdx,
    RemainingEventsToProcess);

  if (0U != *NextCmbEvIdx)
  {
    CmbEvIdx = *NextCmbEvIdx;
  }

  /* process combined events */
  for (; CmbEvIdx <= LastCmbEvIdx; CmbEvIdx++)
  {
    Dem_ClearThisCycleAndReadinessStatus(Dem_CmbDTCEvents[CmbEvIdx]);
    /* !LINKSTO Dem.ClearDTCBurstReduction.ClearEventLimit,1 */
    (*RemainingEventsToProcess)--;
    if (0U == *RemainingEventsToProcess)
    {
      /* configured clear limit reached */
      break;
    }
  }

  /* update the combined status */
  Dem_UpdateCmbDTCStatus(EventId);

  if (CmbEvIdx < LastCmbEvIdx)
  {
    /* suspend event status clearing */
    *NextCmbEvIdx = CmbEvIdx + 1U;
    Result = DEM_PENDING;
  }
  else
  {
    /* done with status clearing of combined events of this EventId */
    *NextCmbEvIdx = 0U;
  }

  DBG_DEM_CLEARTHISCYCLEANDREADINESSSTATUSCMBEVENTS_EXIT(Result, EventId, NextCmbEvIdx,
    RemainingEventsToProcess);

  return Result;
}
#endif /* (DEM_NUM_CMB_DTCS > 0U) */
#endif /* (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON) */

/*------------------[Dem_ClearEvent]----------------------------------------*/

STATIC FUNC(void, DEM_CODE) Dem_ClearEvent(
  Dem_EventIdType                                    EventId,
  P2VAR(Dem_EventModifiedType, AUTOMATIC, AUTOMATIC) AnyEventModified)
{
#if (DEM_USER_CTRL_WIR_SUPPORT == STD_ON)
  /* !LINKSTO dsn.Dem.Indicator.UserControlled.ClearDTC,1 */
  const Dem_DTCStatusMaskType DTCStatusMask =
    DEM_IS_USER_CTRL_WIR_SET(EventId) ?
    (DEM_UDS_STATUS_TNCSLC | DEM_UDS_STATUS_TNCTOC | DEM_UDS_STATUS_WIR) :
    (DEM_UDS_STATUS_TNCSLC | DEM_UDS_STATUS_TNCTOC);
#else
  const Dem_DTCStatusMaskType DTCStatusMask =
    (DEM_UDS_STATUS_TNCSLC | DEM_UDS_STATUS_TNCTOC);
#endif
#if ( (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON) || \
      (DEM_USE_CB_INIT_MONITOR == STD_ON) || \
      ( (DEM_J1939_SUPPORT == STD_ON) && \
        (DEM_J1939_READING_DTC_SUPPORT == STD_ON) ) )
  /* value remains FALSE unless J1939 reading DTC functionality is enabled */
  boolean TriggerJ1939DcmCb = FALSE;
#endif
  Dem_DTCStatusMaskType OldDTCStatusMask;
  Dem_EventMemoryEntryPtrType EvMemEntry;
  Dem_SizeEvMemEntryType EvMemEntryIdx;

  DBG_DEM_CLEAREVENT_ENTRY(EventId, AnyEventModified);

  /*
   * ENTER critical section to protect the event memory
   *       call-context: Dcm (Task)
   */
  DEM_ENTER_EXCLUSIVE_AREA();

  /* get old status mask */
  OldDTCStatusMask = DEM_DTC_STATUS_MASKS[EventId];

  /* reset status mask for event */
#if ( (DEM_EVENT_DISABLING_SUPPORT == STD_ON) && \
      (DEM_CLEAR_DTC_OF_DISABLED_EVENTS == STD_ON) )
  /* !LINKSTO DemClearDTCDisabledEvents.UdsStatusResetValue,1 */
  if (Dem_GbiEvAvailable(EventId) == FALSE)
  {
    DEM_DTC_STATUS_MASKS[EventId] = DEM_NO_EVENT_AVAILABLE;
  }
  else
#endif
  {
#if ((DEM_J1939_SUPPORT == STD_ON) && (DEM_J1939_READING_DTC_SUPPORT == STD_ON))
    if (DEM_IS_EV_ST_BIT_TF_SET(EventId))
    {
      TriggerJ1939DcmCb = TRUE;
    }
#endif /* ((DEM_J1939_SUPPORT == STD_ON) && (DEM_J1939_READING_DTC_SUPPORT == STD_ON)) */
    DEM_DTC_STATUS_MASKS[EventId] = DTCStatusMask;
  }

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW_SUPPORT_FAULT_CONFIRMATION == STD_ON)
  /* reset failure counter independent of event memory entry existence */
  Dem_ResetStatusCounter(EventId);
#endif

#if (DEM_NUM_INDICATOR_LINKS > 0U)
  /* if an event status has been cleared, the healing cycle counter of the
   * related indicator has to be cleared, too */
#if (DEM_INDICATORCLASS_SUPPORT == STD_ON)
  {
    const Dem_IndicatorGrpIdxType IndicatorClassIdx =
      Dem_GbiIndicatorClassIndex(EventId);

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

        /* to keep data consistent, clear all data independent of indicator
         * class is enabled or disabled */

        /* !LINKSTO dsn.Dem.IndicatorCycleCounters.Clear,1 */
        /* clear indicator link-related data */
        DEM_CLR_BIT_IN_ARRAY(
          DEM_INDICATOR_CYC_COUNTER_WIR_STATUS[IndicatorClass->IndicatorId],
          EventId);

        DEM_INDICATOR_CYC_COUNTER[IndicatorClass->IndicatorId][EventId] = 0U;

        /* DEM_INDICATORCOUNTER_ROLE_FAILURE_CYCLE_COUNTER is not cleared */
      }
    }
  }
#else
  {
    Dem_IndicatorIdxType LinkIdx;

    /* Deviation TASKING-2 */
    for (LinkIdx = 0U; LinkIdx < DEM_NUM_INDICATOR_LINKS; ++LinkIdx)
    {
      if (EventId == Dem_IndicatorLink[LinkIdx].EventId)
      {
        const Dem_CounterDataIdxType CounterDataIdx =
            (Dem_CounterDataIdxType)LinkIdx;

        /* !LINKSTO dsn.Dem.IndicatorCycleCounters.Clear,1 */
        /* clear indicator link-related data */
        DEM_CLR_BIT_IN_ARRAY(
          DEM_INDICATOR_CYC_COUNTER_WIR_STATUS[DEM_INDICATOR_DEFAULT_COUNTER],
          CounterDataIdx);

        DEM_INDICATOR_CYC_COUNTER[DEM_INDICATOR_DEFAULT_COUNTER][CounterDataIdx] = 0U;

        /* DEM_INDICATORCOUNTER_ROLE_FAILURE_CYCLE_COUNTER is not cleared */
      }
    }
  }
#endif /* (DEM_INDICATORCLASS_SUPPORT == STD_ON) */

#endif /* DEM_NUM_INDICATOR_LINKS > 0U */

  /* !LINKSTO Dem_OBD_0005,1 */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U)
  /* !LINKSTO Dem_OBD_0082,1 */
  Dem_PermanentMemoryNotifyClear(EventId);
#endif

#if (DEM_MAX_NUMBER_PRESTORED_FF > 0U)
  /* release prestored freeze frame */
  if (Dem_GbiEnablePrestorage(EventId) == TRUE)
  {
    /* mark prestored freeze frame as unused */
    DEM_CLR_BIT_IN_ARRAY(Dem_PFFEntryValid, Dem_GetPFFIndex(EventId));
  }
#endif /* DEM_MAX_NUMBER_PRESTORED_FF > 0U */

  /* reset debouncing counters for event (Dem343) */
  Dem_DebounceFctPtrTable[Dem_GbiDebounceAlgo(EventId)].ResetFct(EventId);

  /* !LINKSTO dsn.Dem.ClearDTCBurstReduction.EventEntrySearch,1 */
#if (DEM_EXTENDED_DATA_CAPTURE != DEM_TRIGGER_ON_FDC_THRESHOLD)
  if (DEM_IS_BIT_SET(OldDTCStatusMask, DEM_UDS_STATUS_TFSLC))
#endif
  {
    /* search for DTC in event memory */
    if (Dem_SearchForEntry(EventId, &EvMemEntry, &EvMemEntryIdx) == TRUE)
    {
#if (DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON)
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_VOLATILE)
      /* CLRRESP_VOLATILE: updated on shutdown */
      const boolean NvMForceWrite = DEM_NVM_NO_FORCE;
#else
      /* CLRRESP_NONVOLATILE_<...>: write immediately */
      const boolean NvMForceWrite = DEM_NVM_FORCE;
#endif
#endif /* if (DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON) */

      /* !LINKSTO Dem.ASR422.SWS_Dem_00879,1 */
      Dem_ClearEventEntry(EvMemEntry);

#if (DEM_GET_DTCBYOCCURRENCETIME_SUPPORT == STD_ON)
      Dem_ResetEventIdOccurKinds(EventId);
#endif

#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
      /* event entry was modified */
      AnyEventModified->EvMemEntry = TRUE;
#endif

#if (DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON)
      /* event memory entry has been cleared: trigger immediate storage */
      Dem_CheckForImmediateEntryStorage(
        EventId,
        EvMemEntry,
        EvMemEntryIdx,
        NvMForceWrite);
#endif /* DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON */

#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
      /* entry is scheduled to be cleared */
      Dem_ClearDtcAsyncStatus.OriginIdx = Dem_GbiDTCOriginIdx(EventId);
      Dem_ClearDtcAsyncStatus.EventMemWrite = DEM_NVM_WAITING;
#endif
    }
  }

  if (OldDTCStatusMask != DTCStatusMask)
  {
    /* event status was modified: mark block as modified to trigger
     * a ClearDtc run state transition to RUNNING */
    AnyEventModified->EvSt = TRUE;
  }

  /* clear error queue */
  Dem_InvalidateErrorQueueEntry(EventId);

  /*
   * LEAVE critical section
   */
  DEM_EXIT_EXCLUSIVE_AREA();

#if ( (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON) || \
      (DEM_USE_CB_INIT_MONITOR == STD_ON) || \
      ( (DEM_J1939_SUPPORT == STD_ON) && \
        (DEM_J1939_READING_DTC_SUPPORT == STD_ON) ) )
  /* trigger all relevant callbacks */
  Dem_TriggerCbForClearedEvent(
    EventId, OldDTCStatusMask, DTCStatusMask, TriggerJ1939DcmCb);
#endif

  DBG_DEM_CLEAREVENT_EXIT(EventId, AnyEventModified);
}

/*----------------------[Dem_TriggerCbForClearedEvent]----------------------*/

#if ( (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON) || \
      (DEM_USE_CB_INIT_MONITOR == STD_ON) || \
      ( (DEM_J1939_SUPPORT == STD_ON) && \
        (DEM_J1939_READING_DTC_SUPPORT == STD_ON) ) )
STATIC FUNC(void, DEM_CODE) Dem_TriggerCbForClearedEvent(
  Dem_EventIdType       EventId,
  Dem_DTCStatusMaskType OldDTCStatusMask,
  Dem_DTCStatusMaskType NewDTCStatusMask,
  boolean               J1939DcmCbNeeded)
{
  DBG_DEM_TRIGGERCBFORCLEAREDEVENT_ENTRY(
    EventId, OldDTCStatusMask, NewDTCStatusMask, J1939DcmCbNeeded);

#if ( (DEM_J1939_SUPPORT == STD_ON) && \
      (DEM_J1939_READING_DTC_SUPPORT == STD_ON) )
  if (J1939DcmCbNeeded == TRUE)
  {
    Dem_CbTriggerJ1939Dcm(EventId);
  }
#else
  TS_PARAM_UNUSED(J1939DcmCbNeeded);
#endif /* ( (DEM_J1939_SUPPORT == STD_ON) &&
            (DEM_J1939_READING_DTC_SUPPORT == STD_ON) ) */

#if ( (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON) || \
      (DEM_USE_CB_INIT_MONITOR == STD_ON) )
  /* report event change by callback */
  if (OldDTCStatusMask != NewDTCStatusMask)
  {
#if (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON)
    /* notify application that event status has changed */
    Dem_CbTriggerOnEventStatus(EventId, OldDTCStatusMask, NewDTCStatusMask);
#endif

#if (DEM_USE_CB_INIT_MONITOR == STD_ON)
    /* Dem009: inform application about clearance */
    Dem_CbInitMonitorForEvent(EventId, DEM_INIT_MONITOR_CLEAR);
#endif
  }
#else
  TS_PARAM_UNUSED(OldDTCStatusMask);
  TS_PARAM_UNUSED(NewDTCStatusMask);
#endif /* ( (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON) ||
            (DEM_USE_CB_INIT_MONITOR == STD_ON) ) */

  DBG_DEM_TRIGGERCBFORCLEAREDEVENT_EXIT(
    EventId, OldDTCStatusMask, NewDTCStatusMask, J1939DcmCbNeeded);
}
#endif /* ( (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON) ||
            (DEM_USE_CB_INIT_MONITOR == STD_ON) ||
            ( (DEM_J1939_SUPPORT == STD_ON) &&
              (DEM_J1939_READING_DTC_SUPPORT == STD_ON) ) ) */

/*------------------[Dem_CheckDeleteEvent]----------------------------------*/

STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_CheckDeleteEvent(
  boolean           DtcGroupRequest,
  uint8             GroupIdx,
  Dem_EventIdType   EventId,
  boolean           ForcedDelete
  )
{
  Std_ReturnType Result = E_NOT_OK;
  const uint32 DTC = Dem_ClearDTCSelection.DTCInfo.DTC;
  const Dem_DTCOriginType OriginOfEvt = Dem_GbiDTCOrigin(EventId);

  DBG_DEM_CHECKDELETEEVENT_ENTRY(DtcGroupRequest, GroupIdx,
    EventId, ForcedDelete);

#if (DEM_SUPPRESSION_SUPPORT != DEM_DTC_SUPPRESSION)
  TS_PARAM_UNUSED(ForcedDelete);
#endif

  if (OriginOfEvt == Dem_ClearDTCSelection.DTCInfo.DTCOrigin)
  {
#if (DEM_NUM_CMB_DTCS > 0U)
    /* if event is combined, check if it is the master event */
    if (DEM_GET_MASTER_EVID(EventId) == EventId)
#endif
    {
#if (DEM_EVENT_DISABLING_SUPPORT == STD_ON)
#if (DEM_NUM_CMB_DTCS > 0U)
      boolean IsEventEnabled = Dem_IsAnyEvAvailable(EventId);
#else
      boolean IsEventEnabled = Dem_GbiEvAvailable(EventId);
#endif /* (DEM_NUM_CMB_DTCS > 0U) */
      if ( (IsEventEnabled == TRUE) || (ForcedDelete == TRUE ) )
#endif /* (DEM_EVENT_DISABLING_SUPPORT == STD_ON) */
      {
#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
        boolean IsSuppressed = FALSE;
#endif /* (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION) */
        boolean EntryDTC = FALSE;

#if (DEM_OBD_Support == STD_ON)
        if (Dem_ClearDTCSelection.DTCInfo.DTCFormat == DEM_DTC_FORMAT_OBD)
        {
#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
          IsSuppressed = DEM_IS_BIT_SET_IN_ARRAY(Dem_OBDDTCSuppression, EventId)? TRUE : FALSE;
#endif
          if (DtcGroupRequest == FALSE)
          {
            const uint32 DTCOfEvent = Dem_GetObdDTC(EventId);
            if (DTCOfEvent == DTC)
            {
              EntryDTC = TRUE;
            }
          }
        }
        else /* DTCFormat == DEM_DTC_FORMAT_UDS */
#endif /* (DEM_OBD_Support == STD_ON) */
        {
#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
          IsSuppressed = DEM_IS_BIT_SET_IN_ARRAY(Dem_UDSDTCSuppression, EventId)? TRUE : FALSE;
#endif
          if (DtcGroupRequest == FALSE)
          {
            const uint32 DTCOfEvent = Dem_GetUdsDTC(EventId);
            if (DTCOfEvent == DTC)
            {
              EntryDTC = TRUE;
            }
          }
        }
        {
/* !LINKSTO Dem_OBD_0001,2 */
#if (DEM_EMISSION_REL_DTCS_GROUP_EXIST == STD_ON) && (DEM_OBD_Support == STD_ON)
          const Dem_DTCKindType DTCKind = Dem_GbiDTCKind(EventId);
          const boolean EmissionRelDTCs =
            ( ((Dem_DTCGroupType) DTC == DEM_DTC_GROUP_EMISSION_REL_DTCS) &&
              (DTCKind == DEM_DTC_KIND_EMISSION_REL_DTCS) ) ? TRUE : FALSE;
#else
          const boolean EmissionRelDTCs = FALSE;
#endif
          const uint8 GroupIdxOfEvt = Dem_GbiDTCGroupIdx(EventId);

#if (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION)
          if ( ( (EntryDTC == TRUE) ||
                 (EmissionRelDTCs == TRUE) ||
                 (GroupIdxOfEvt == GroupIdx) ||
                 ((Dem_DTCGroupType) DTC == DEM_DTC_GROUP_ALL_DTCS)
               ) &&
               ( (IsSuppressed == FALSE) || (ForcedDelete == TRUE) )
             )
#else
            if ( (EntryDTC == TRUE) ||
                 (EmissionRelDTCs == TRUE) ||
                 (GroupIdxOfEvt == GroupIdx) ||
                 ((Dem_DTCGroupType) DTC == DEM_DTC_GROUP_ALL_DTCS)
               )
#endif /* (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION) */
          {
            Result = E_OK;
          }
        }
      }
    }
  }

  DBG_DEM_CHECKDELETEEVENT_EXIT(Result, DtcGroupRequest, GroupIdx,
    EventId, ForcedDelete);
  return Result;
}

/*------------------[Dem_J1939CheckDeleteEvent]-----------------------------*/

#if((DEM_J1939_SUPPORT == STD_ON) && (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON))
STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_J1939CheckDeleteEvent(
  Dem_EventIdType   EventId)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_J1939CHECKDELETEEVENT_ENTRY(EventId);
  /* part of all the combined events has already been processed */
#if (DEM_NUM_CMB_DTCS > 0U)
  if(Dem_ClearDtcAsyncStatus.NextCmbEvIdx != 0)
  {
    Result = E_OK;
  }
  else
#endif /* (DEM_NUM_CMB_DTCS > 0U) */
  {
    if(TRUE == Dem_J1939DcmClearFilterCheck(Dem_J1939ClearDTCFilter.Dem_J1939DTCTypeFilter, Dem_J1939ClearDTCFilter.Dem_J1939Node, EventId))
    {
      Result = E_OK;
    }
  }
#if (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH)
  if(E_OK == Result)
  {
    uint8 OriginIdx = Dem_GbiDTCOriginIdx(EventId);

    if(Dem_ClearDtcAsyncStatus.AnyEventModified.EvMemEntry == FALSE)
    {
      Dem_ClearDtcAsyncStatus.AnyEventModified.EvMemEntry = Dem_CheckForOldClearImmediateEntries(OriginIdx);
    }
  }
#endif

  DBG_DEM_J1939CHECKDELETEEVENT_EXIT(Result, EventId);
  return Result;
}
#endif

/*------------------[Dem_IsDTCGroupClear]-----------------------------------*/

STATIC FUNC(boolean, DEM_CODE) Dem_IsDTCGroupClear(
  uint32                             DTC,
  Dem_DTCFormatType                  DTCFormat,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) GroupIdx)
{
  boolean Result = FALSE;

  DBG_DEM_ISDTCGROUPCLEAR_ENTRY(DTC, DTCFormat, GroupIdx);

#if((DEM_J1939_SUPPORT == STD_ON) && (DEM_J1939_CLEAR_DTC_SUPPORT == STD_ON))
  if(Dem_ClearDtcAsyncStatus.ClientId == DEM_VIRTUAL_CLIENT_J1939)
  {
    Result = TRUE;
  }
  else
#endif
  {
    Result = Dem_IsDTCGroupValue(DTC, DTCFormat, GroupIdx);
  }

  DBG_DEM_ISDTCGROUPCLEAR_EXIT(Result, DTC, DTCFormat, GroupIdx);
  return Result;
}

/*------------------[Dem_GetIdxOfDTCGroup]----------------------------------*/

#if (DEM_NUM_DTC_GROUPS > 0U)
STATIC FUNC(boolean, DEM_CODE) Dem_GetIdxOfDTCGroup(
  Dem_DTCGroupType                   DTCGroup,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) Idx)
{
  boolean Result;
  uint8_least Index;

  DBG_DEM_GETIDXOFDTCGROUP_ENTRY(DTCGroup, Idx);

  /* for all DTCGroups */
  /* Deviation TASKING-2 */
  for (Index = 0U; Index < DEM_NUM_DTC_GROUPS; Index++)
  {
    if (DTCGroup == Dem_DTCGroups[Index])
    {
      break;
    }
  }

  if (Index < DEM_NUM_DTC_GROUPS)
  {
    *Idx   = (uint8)Index;
    Result = TRUE;
  }
  else
  {
    *Idx   = DEM_DTCGRP_IDX_INVALID;
    Result = FALSE;
  }

  DBG_DEM_GETIDXOFDTCGROUP_EXIT(Result, DTCGroup, Idx);
  return Result;
}
#endif /* DEM_NUM_DTC_GROUPS > 0U */

/*------------------[Dem_ResetEventIdOccurKinds]----------------------------*/

#if (DEM_GET_DTCBYOCCURRENCETIME_SUPPORT == STD_ON)
STATIC FUNC(void, DEM_CODE) Dem_ResetEventIdOccurKinds(Dem_EventIdType EventId)
{
  Dem_DTCRequestType OccurKindRequest;

  DBG_DEM_RESETEVENTIDOCCURKINDS_ENTRY(EventId);

  /* if the DTC assigned to the event is stored in PRIMARY memory */
  if (Dem_GbiDTCOrigin(EventId) == DEM_DTC_ORIGIN_PRIMARY_MEMORY)
  {
    for (OccurKindRequest = DEM_FIRST_FAILED_DTC;
          OccurKindRequest <= DEM_MOST_REC_DET_CONFIRMED_DTC; OccurKindRequest++)
    {
      if (DEM_OCCURKIND_STORED_EVENT_ID(OccurKindRequest) == EventId)
      {
        DEM_OCCURKIND_STORED_EVENT_ID(OccurKindRequest) = DEM_EVENT_ID_INVALID;
      }
    }
  }

  DBG_DEM_RESETEVENTIDOCCURKINDS_EXIT(EventId);
}
#endif /* (DEM_GET_DTCBYOCCURRENCETIME_SUPPORT == STD_ON) */

/*------------------[Dem_CbClearEventAllowed]-------------------------------*/

#if (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON)
STATIC FUNC(Std_ReturnType, DEM_CODE) Dem_CbClearEventAllowed(
  Dem_EventIdType                          EventId,
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA)  Behavior,
  P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) Allowed)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_DEM_CBCLEAREVENTALLOWED_ENTRY(EventId, Behavior, Allowed);

#if (DEM_RTE_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE > 0U)
  if ((Dem_CbProperty[EventId] & DEM_RTE_CB_CLEAR_EVENT_ALLOWED) != 0U)
  {
    Dem_CbIndexType RteCbIdx = 0U;

    while ( (RteCbIdx < DEM_RTE_LOOKUP_TABLE_CLEAR_EVENT_ALLOWED_SIZE) &&
            (DEM_CLEAREV_BEHAVIOR_GET_EVENT_ID(
               Dem_RteLookupTableClearEventAllowed[RteCbIdx]) != EventId) )
    {
      RteCbIdx++;
    }

    DEM_PRECONDITION_ASSERT((RteCbIdx < DEM_RTE_LOOKUP_TABLE_CLEAR_EVENT_ALLOWED_SIZE),
                            DEM_INTERNAL_API_ID);

    /* extract clear event behavior from packed event id */
    *Behavior = DEM_CLEAREV_BEHAVIOR_GET_BEHAVIOR(Dem_RteLookupTableClearEventAllowed[RteCbIdx]);

    /* same index in both arrays */
    RetVal = Dem_RteFuncPtrClearEventAllowed[RteCbIdx](Allowed);
  }
#endif /* (DEM_RTE_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE > 0U) */

#if (DEM_C_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE > 0U)
  if ((Dem_CbProperty[EventId] & DEM_C_CB_CLEAR_EVENT_ALLOWED) != 0U)
  {
    Dem_CbIndexType CCbIdx = 0U;

    while ( (CCbIdx < DEM_C_LOOKUP_TABLE_CLEAR_EVENT_ALLOWED_SIZE) &&
            (DEM_CLEAREV_BEHAVIOR_GET_EVENT_ID(
               Dem_CbLookupTableClearEventAllowed[CCbIdx].EventId) != EventId) )
    {
      CCbIdx++;
    }

    DEM_PRECONDITION_ASSERT((CCbIdx < DEM_C_LOOKUP_TABLE_CLEAR_EVENT_ALLOWED_SIZE),
                            DEM_INTERNAL_API_ID);

    /* extract clear event behavior from packed event id */
    *Behavior = DEM_CLEAREV_BEHAVIOR_GET_BEHAVIOR(
      Dem_CbLookupTableClearEventAllowed[CCbIdx].EventId);

    /* get index/link of callback function pointer */
    CCbIdx = Dem_CbLookupTableClearEventAllowed[CCbIdx].CallbackIdx;
    RetVal = Dem_CbFuncPtrClearEventAllowed[CCbIdx](Allowed);
  }
#endif /* (DEM_C_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE > 0U) */

  DBG_DEM_CBCLEAREVENTALLOWED_EXIT(RetVal, EventId, Behavior, Allowed);
  return RetVal;
}
#endif /* (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON) */

#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>

/*==================[end of file]===========================================*/
