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

/* this file contains definitions of OBD related APIs provided by Dem to the
 * Dcm. */

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
/* !LINKSTO dsn.Dem.IncludeStr,1 */

#include <TSAutosar.h>                        /* EB specific standard types */
#include <Std_Types.h>                            /* AUTOSAR standard types */
#include <TSMem.h>                            /* EB specific memory functions */

#include <Dem_Int.h>             /* module public and internal declarations */
#include <Dem_Trace.h>                        /* Dbg related macros for Dem */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (DEM_OBD_Support == STD_ON)

#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

/** \brief Updates the content of PID 01 for an event
 *
 *  \param[in] EventId         Event to be checked
 *  \param[in] ReadinessGroup  Readiness group to be updated
 *  \param[in] ReadinessGroupChecked  Flag if this group has been checked
 *                                    before
 *  \param[inout]  PID01value  Pointer to the PID01 to be updated
 *
 *  This function updates the given PID01 depending on the other parameters,
 *  e.g. event status.
 *
 *  \return boolean  Returns if the given readiness group is now checked.
 */
STATIC FUNC(boolean, DEM_CODE) Dem_UpdatePID01value(
  const Dem_EventIdType EventId,
  const uint16 ReadinessGroup,
  const boolean ReadinessGroupChecked,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID01value);

#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>

#endif /* #if (DEM_OBD_Support == STD_ON) */

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/


#if (DEM_INCLUDE_RTE == STD_ON)
#define Dem_START_SEC_CODE
#else
#define DEM_START_SEC_CODE
#endif
#include <Dem_MemMap.h>

#if (DEM_OBD_Support == STD_ON)
/*----------------[Dem_SetEventDisabled]------------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_SetEventDisabled(Dem_EventIdType EventId)
{
  /* !LINKSTO dsn.Dem.OBDPidSupport.Dem_SetEventDisabled_01,1 */
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_SETEVENTDISABLED_ENTRY(EventId);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_SetEventDisabled, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_SetEventDisabled, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (EventId == DEM_EVENT_ID_INVALID)
  {
    DEM_REPORT_ERROR(DEM_SID_SetEventDisabled, DEM_E_PARAM_DATA);
  }
  else if (EventId > DEM_MAX_EVENTID)
  {
    DEM_REPORT_ERROR(DEM_SID_SetEventDisabled, DEM_E_WRONG_CONFIGURATION);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
#if (DEM_EVENT_DISABLING_SUPPORT == STD_ON)
  if (Dem_GbiEvAvailable(EventId) == FALSE)
  {
    /* event is disabled/not available so the disabling
     * of event shall be skipped */
    /* E_NOT_OK return value remains */
  }
  else
#endif
  {
#if (DEM_MAX_EVENTS_PER_READINESS_GROUP == 0U)
    /* Get the readiness group of the event */
    const Dem_EventOBDReadinessGroupType ReadinessGroup = Dem_GbiOBDReadinessGroup(EventId);

    if (ReadinessGroup < DEM_OBD_RDY_PID01PID41_CNT)
    {
      /*
       * ENTER critical section to protect ReadinessEnableStatus
       *       call-context: SWC / CDD
       */
      /* !LINKSTO dsn.Dem.ReadinessEnableStatus.CriticalSection,1 */
      DEM_ENTER_EXCLUSIVE_AREA();

      /* !LINKSTO Dem348,1, dsn.Dem.OBDPidSupport.Dem_ReadinessEnableStatus_01,1 */
      /* Set the readiness group of the event disabled */
      DEM_SET_READINESS_GROUP_DISABLED(ReadinessGroup, Dem_NvData.ReadinessEnableStatus);

      /*
       * LEAVE critical section
       */
      DEM_EXIT_EXCLUSIVE_AREA();
    }
#else
    uint8 RdyGrpIdx;
    uint8 EvtIdx;

    for (RdyGrpIdx = 0U; RdyGrpIdx < DEM_NUM_OBD_RDY_GROUPS; RdyGrpIdx++)
    {
      /* Deviation TASKING-1 */
      for (EvtIdx = 0U; EvtIdx < DEM_MAX_EVENTS_PER_READINESS_GROUP; EvtIdx++)
      {
        if (Dem_OBDReadinessGroup[RdyGrpIdx][EvtIdx] == EventId)
        {
          /* get the readiness group of the event */
          const Dem_EventOBDReadinessGroupType ReadinessGroup =
              Dem_OBDReadinessGroupMap[RdyGrpIdx];

          if (ReadinessGroup < DEM_OBD_RDY_PID01PID41_CNT)
          {
            /*
             * ENTER critical section to protect ReadinessEnableStatus
             *       call-context: SWC / CDD
             */
            /* !LINKSTO dsn.Dem.ReadinessEnableStatus.CriticalSection,1 */
            DEM_ENTER_EXCLUSIVE_AREA();

            /* !LINKSTO Dem348,1 */
            /* !LINKSTO dsn.Dem.OBDPidSupport.Dem_ReadinessEnableStatus_01,1 */
            /* set the readiness group of the event disabled */
            DEM_SET_READINESS_GROUP_DISABLED(
              ReadinessGroup, Dem_NvData.ReadinessEnableStatus);

            /*
             * LEAVE critical section
             */
            DEM_EXIT_EXCLUSIVE_AREA();
          }

          /* stop searching in this readiness group */
          break;
        }
      }
    }
#endif /* (DEM_MAX_EVENTS_PER_READINESS_GROUP == 0U) */
    Result = E_OK;
  }

  DBG_DEM_SETEVENTDISABLED_EXIT(Result, EventId);
  return Result;
}
#endif /* if (DEM_OBD_Support == STD_ON) */


/*----------------[Dem_SetPtoStatus]----------------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_SetPtoStatus(boolean PtoStatus)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_SETPTOSTATUS_ENTRY(PtoStatus);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_SetPtoStatus, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_SetPtoStatus, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if ( (PtoStatus != TRUE) && (PtoStatus != FALSE) )
  {
    DEM_REPORT_ERROR(DEM_SID_SetPtoStatus, DEM_E_PARAM_DATA);
  }
  else
#else /* DEM_DEV_ERROR_DETECT */
  TS_PARAM_UNUSED(PtoStatus);
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* function NOT fully implemented yet.
     * implementation is scheduled for future releases. */
  }

  DBG_DEM_SETPTOSTATUS_EXIT(Result, PtoStatus);
  return Result;
}

/* !LINKSTO Dem_OBD_0001,2 */
#if (DEM_OBD_Support == STD_ON)
/*----------------[Dem_SetPfcCycleQualified]--------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_SetPfcCycleQualified(void)
{
  Std_ReturnType Result = E_OK;

  DBG_DEM_SETPFCCYCLEQUALIFIED_ENTRY();

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_SetPfcCycleQualified, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_SetPfcCycleQualified, DEM_E_WRONG_CONTEXT);
  }
#endif
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* set permanent fault status */
    /* !LINKSTO Dem_OBD_0028,1 */
    /* !LINKSTO Dem_OBD_0005,1 */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U)
    DEM_PFC_STATUS = TRUE;
#endif
  }

  DBG_DEM_SETPFCCYCLEQUALIFIED_EXIT(Result);
  return Result;
}

/*----------------[Dem_GetPfcCycleQualified]--------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_GetPfcCycleQualified(
  P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA) isqualified)
{
  Std_ReturnType Result = E_OK;

  DBG_DEM_GETPFCCYCLEQUALIFIED_ENTRY(isqualified);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_GetPfcCycleQualified, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_GetPfcCycleQualified, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (isqualified == NULL_PTR)
  {
    /* !LINKSTO Dem_OBD_0079,1 */
    DEM_REPORT_ERROR(DEM_SID_GetPfcCycleQualified, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* get permanent fault status */
    /* !LINKSTO Dem_OBD_0078,1 */
    /* !LINKSTO Dem_OBD_0005,1 */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U)
    *isqualified = DEM_PFC_STATUS;
#else
    *isqualified = FALSE;
#endif
  }

  DBG_DEM_GETPFCCYCLEQUALIFIED_EXIT(Result, isqualified);
  return Result;
}

/*----------------[Dem_ResetReadiness]--------------------------------------*/

FUNC(void, DEM_CODE) Dem_ResetReadiness(void)
{
  DBG_DEM_RESETREADINESS_ENTRY();

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO dsn.Dem.OBDPidSupport.Dem_ResetReadiness_01,1 */
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_ResetReadiness, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_ResetReadiness, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
#if (DEM_MAX_EVENTS_PER_READINESS_GROUP == 0U)
    Dem_EventIdType EvId;

    /* Deviation TASKING-1 */
    for (EvId = 1U; EvId <= DEM_MAX_EVENTID; ++EvId)
    {
      const Dem_EventOBDReadinessGroupType ReadinessGroup = Dem_GbiOBDReadinessGroup(EvId);

      if (ReadinessGroup < DEM_OBD_RDY_PID01PID41_CNT)
      {
        Dem_DTCStatusMaskType OldDTCStatusMask;
        Dem_DTCStatusMaskType NewDTCStatusMask;
        /* calculate Status Mask place outside the critical section */
        P2VAR(Dem_DTCStatusMaskType, AUTOMATIC, DEM_VAR) EventDTCStatusPtr =
          &DEM_DTC_STATUS_MASKS[EvId];

        /*
         * ENTER critical section to protect consistency of UDS Status Byte
         */
        DEM_ENTER_EXCLUSIVE_AREA();

        OldDTCStatusMask = *EventDTCStatusPtr;
        NewDTCStatusMask = OldDTCStatusMask;

        if ( (!DEM_IS_ST_BIT_CDTC_SET(OldDTCStatusMask)) &&
             (!DEM_IS_ST_BIT_PDTC_SET(OldDTCStatusMask)) )
        {
          DEM_SET_MASK(NewDTCStatusMask, DEM_UDS_STATUS_TNCSLC);
          *EventDTCStatusPtr = NewDTCStatusMask;
        }

        /*
         * LEAVE critical section
         */
        DEM_EXIT_EXCLUSIVE_AREA();

#if (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON)
        if (OldDTCStatusMask != NewDTCStatusMask)
        {
          /* notify application if event status has changed */
          Dem_CbTriggerOnEventStatus(EvId, OldDTCStatusMask, NewDTCStatusMask);
        }
#endif /* DEM_USE_CB_TRIG_ON_STATUS_CHANGE */
      }
    }
#else /* DEM_MAX_EVENTS_PER_READINESS_GROUP == 0U */
    uint8 RdyGrpIdx;

    for (RdyGrpIdx = 0U; RdyGrpIdx < DEM_NUM_OBD_RDY_GROUPS; RdyGrpIdx++)
    {
      uint8 EvtIdx;

      /* Deviation TASKING-1 */
      for (EvtIdx = 0U; EvtIdx < DEM_MAX_EVENTS_PER_READINESS_GROUP; EvtIdx++)
      {
        const Dem_EventIdType EventId =
          Dem_OBDReadinessGroup[RdyGrpIdx][EvtIdx];
        Dem_DTCStatusMaskType OldDTCStatusMask;
        Dem_DTCStatusMaskType NewDTCStatusMask;
        /* calculate Status Mask place outside the critical section */
        P2VAR(Dem_DTCStatusMaskType, AUTOMATIC, DEM_VAR) EventDTCStatusPtr =
          &DEM_DTC_STATUS_MASKS[EventId];

        if (DEM_EVENT_ID_INVALID == EventId)
        {
          /* end of list of events; stop searching this readiness group */
          break;
        }

        /*
         * ENTER critical section to protect consistency of UDS Status Byte
         */
        DEM_ENTER_EXCLUSIVE_AREA();

        OldDTCStatusMask = *EventDTCStatusPtr;
        NewDTCStatusMask = OldDTCStatusMask;

        if ( (!DEM_IS_ST_BIT_CDTC_SET(OldDTCStatusMask)) &&
             (!DEM_IS_ST_BIT_PDTC_SET(OldDTCStatusMask)) )
        {
          DEM_SET_MASK(NewDTCStatusMask, DEM_UDS_STATUS_TNCSLC);
          *EventDTCStatusPtr = NewDTCStatusMask;
        }

        /*
         * LEAVE critical section
         */
        DEM_EXIT_EXCLUSIVE_AREA();

#if (DEM_USE_CB_TRIG_ON_STATUS_CHANGE == STD_ON)
        if (OldDTCStatusMask != NewDTCStatusMask)
        {
          /* notify application if event status has changed */
          Dem_CbTriggerOnEventStatus(EventId, OldDTCStatusMask, NewDTCStatusMask);
        }
#endif /* DEM_USE_CB_TRIG_ON_STATUS_CHANGE */
      }
    }
#endif /* (DEM_MAX_EVENTS_PER_READINESS_GROUP == 0U) */
  }

  DBG_DEM_RESETREADINESS_EXIT();
}

#endif /* #if (DEM_OBD_Support == STD_ON) */

#if (DEM_INCLUDE_RTE == STD_ON)
#define Dem_STOP_SEC_CODE
#else
#define DEM_STOP_SEC_CODE
#endif
#include <Dem_MemMap.h>

#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

#if ((DEM_IUMPR_FUNCTIONS_ENABLED == STD_ON) && (DEM_OBD_Support == STD_ON))
/*----------------[Dem_RepIUMPRFaultDetect]---------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_RepIUMPRFaultDetect(
  Dem_RatioIdType RatioID)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_REPIUMPRFAULTDETECT_ENTRY(RatioID);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_RepIUMPRFaultDetect, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_RepIUMPRFaultDetect, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else
#endif /* (DEM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* function NOT fully implemented yet.
     * implementation is scheduled for future releases. */
    TS_PARAM_UNUSED(RatioID);
  }

  DBG_DEM_REPIUMPRFAULTDETECT_EXIT(Result, RatioID);
  return Result;
}

/*----------------[Dem_RepIUMPRDenLock]-------------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_RepIUMPRDenLock(Dem_RatioIdType RatioID)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_REPIUMPRDENLOCK_ENTRY(RatioID);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_RepIUMPRDenLock, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_RepIUMPRDenLock, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else
#endif
  {
    /* function NOT fully implemented yet.
     * implementation is scheduled for future releases. */
    TS_PARAM_UNUSED(RatioID);
  }

  DBG_DEM_REPIUMPRDENLOCK_EXIT(Result, RatioID);
  return Result;
}

/*----------------[Dem_RepIUMPRDenRelease]----------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_RepIUMPRDenRelease(Dem_RatioIdType RatioID)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_REPIUMPRDENRELEASE_ENTRY(RatioID);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_RepIUMPRDenRelease, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_RepIUMPRDenRelease, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else
#endif /* (DEM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* function NOT fully implemented yet.
     * implementation is scheduled for future releases. */
    TS_PARAM_UNUSED(RatioID);
   }

  DBG_DEM_REPIUMPRDENRELEASE_EXIT(Result, RatioID);
  return Result;
}

#endif /* (DEM_IUMPR_FUNCTIONS_ENABLED == STD_ON) && (DEM_OBD_Support == STD_ON) */

#if ((DEM_DCM_ENABLED == STD_ON) && (DEM_OBD_Support == STD_ON))
/*----------------[Dem_DcmGetInfoTypeValue08]-------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetInfoTypeValue08(
  Dcm_OpStatusType                       OpStatus,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata08,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata08BufferSize)
{
  /* the API is implemented only as stub with return value always E_NOT_OK */
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_DCMGETINFOTYPEVALUE08_ENTRY(OpStatus, Iumprdata08,
                                      Iumprdata08BufferSize);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmGetInfoTypeValue08, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmGetInfoTypeValue08, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (DCM_INITIAL != OpStatus)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmGetInfoTypeValue08, DEM_E_PARAM_DATA);
  }
  else if ((Iumprdata08 == NULL_PTR) || (Iumprdata08BufferSize == NULL_PTR))
  {
    DEM_REPORT_ERROR(DEM_SID_DcmGetInfoTypeValue08, DEM_E_PARAM_POINTER);
  }
  else
#else /* DEM_DEV_ERROR_DETECT */
  TS_PARAM_UNUSED(Iumprdata08);
  TS_PARAM_UNUSED(Iumprdata08BufferSize);
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* function NOT fully implemented yet.
     * implementation is scheduled for future releases. */
    TS_PARAM_UNUSED(OpStatus);
  }

  DBG_DEM_DCMGETINFOTYPEVALUE08_EXIT(Result, OpStatus, Iumprdata08,
                                     Iumprdata08BufferSize);
  return Result;
}

/*----------------[Dem_DcmGetInfoTypeValue0B]----------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetInfoTypeValue0B(
  Dcm_OpStatusType                       OpStatus,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata0B,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Iumprdata0BBufferSize)
{
  /* the API is implemented only as stub with return value always E_NOT_OK */
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_DCMGETINFOTYPEVALUE0B_ENTRY(OpStatus, Iumprdata0B,
                                      Iumprdata0BBufferSize);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmGetInfoTypeValue0B, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmGetInfoTypeValue0B, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (DCM_INITIAL != OpStatus)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmGetInfoTypeValue0B, DEM_E_PARAM_DATA);
  }
  else if ((Iumprdata0B == NULL_PTR) || (Iumprdata0BBufferSize == NULL_PTR))
  {
    DEM_REPORT_ERROR(DEM_SID_DcmGetInfoTypeValue0B, DEM_E_PARAM_POINTER);
  }
  else
#else /* DEM_DEV_ERROR_DETECT */
  TS_PARAM_UNUSED(Iumprdata0B);
  TS_PARAM_UNUSED(Iumprdata0BBufferSize);
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* function NOT fully implemented yet.
     * implementation is scheduled for future releases. */
    TS_PARAM_UNUSED(OpStatus);
  }

  DBG_DEM_DCMGETINFOTYPEVALUE0B_EXIT(Result, OpStatus, Iumprdata0B,
                                     Iumprdata0BBufferSize);
  return Result;
}
#endif /* (DEM_DCM_ENABLED == STD_ON) && (DEM_OBD_Support == STD_ON) */

#if (DEM_OBD_Support == STD_ON)

/*----------------[Dem_DcmReadDataOfPID01]----------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID01(
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID01value)
{
  Std_ReturnType Result = E_OK;

  DBG_DEM_DCMREADDATAOFPID01_ENTRY(PID01value);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID01, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID01, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (PID01value == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID01, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* array to store the info if at least one event in a group is confirmed */
    boolean ReadinessGroupChecked[DEM_OBD_RDY_PID01PID41_CNT];
    Dem_EventOBDReadinessGroupType ReadinessGroup;
    Dem_EventIdType EventId;
    uint8 PID01[4] = {0x00U};
    const uint8 ObdReadinessEngineTypeMask = 0x08U;

#if (DEM_MAX_EVENTS_PER_READINESS_GROUP > 0U)
    uint8 RdyGrpIdx;
#endif

    /* preset engine type bit,
     * will be mask with configured engine type in
     * Dem_ObdReadinessForceCompleteMask */
    PID01[1] = ObdReadinessEngineTypeMask;

    /* mark all ReadinessGroups as not checked */
    for (ReadinessGroup = 0U;
         ReadinessGroup < DEM_OBD_RDY_PID01PID41_CNT;
         ReadinessGroup++)
    {
      ReadinessGroupChecked[ReadinessGroup] = FALSE;
    }

#if (defined DEM_MIL_ID)
    /* !LINKSTO Dem352,1 */
    if (Dem_InternalGetIndicatorStatus(DEM_MIL_ID, TRUE) != DEM_INDICATOR_OFF)
    {
      /* write the MIL Status corresponding bit of PID01 */
      DEM_SET_MIL_STATUS_PID01(PID01[0]);
    }
#endif

    /* !LINKSTO Dem355_Implicit1,1 */
    /* calculate readiness status of groups */
#if (DEM_MAX_EVENTS_PER_READINESS_GROUP == 0U)
    /* Deviation TASKING-1 */
    for (EventId = 1U; EventId <= DEM_MAX_EVENTID; ++EventId)
    {
#if (DEM_EVENT_DISABLING_SUPPORT == STD_ON)
      if (Dem_GbiEvAvailable(EventId) == TRUE)
#endif
      {
        /* get the readiness group of the event */
        ReadinessGroup = Dem_GbiOBDReadinessGroup(EventId);

        if (ReadinessGroup < DEM_OBD_RDY_PID01PID41_CNT)
        {
            ReadinessGroupChecked[ReadinessGroup] =
                Dem_UpdatePID01value(EventId,
                  ReadinessGroup,
                  ReadinessGroupChecked[ReadinessGroup],
                  PID01);
        }
      } /* else: event disabled so it shall be skipped */
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

#if (DEM_EVENT_DISABLING_SUPPORT == STD_ON)
          if (Dem_GbiEvAvailable(EventId) == TRUE)
#endif
          {
              ReadinessGroupChecked[ReadinessGroup] =
                  Dem_UpdatePID01value(EventId,
                      ReadinessGroup,
                      ReadinessGroupChecked[ReadinessGroup],
                      PID01);
          } /* else: event disabled so it shall be skipped */
        }
      }
    }
#endif /* (DEM_MAX_EVENTS_PER_READINESS_GROUP == 0U) */

    /* !LINKSTO Dem351,1 */
    /* count number of confirmed OBD DTCs by searching through _all_ events.
     * if event combination is enabled, a particular DTC is counted only once per
     * all associated combined events which refer its DTC Class, because only one
     * event can have CDTC bit set within the group of events. */
    /* Deviation TASKING-1 */
    for (EventId = 1U; EventId <= DEM_MAX_EVENTID; ++EventId)
    {
      /* check if the event is not disabled and has an OBD Dtc */
      const uint32 ObdDtc = Dem_GetVisibleObdDTC(EventId);

      if ((ObdDtc != DEM_NO_DTC) && DEM_IS_EV_ST_BIT_CDTC_SET(EventId))
      {
        /* event is configured to have an OBD Dtc and is confirmed -> count it */
        if ((PID01[0] & 0x7FU) < 127U)
        {
          PID01[0]++;
        }
      }
    }

    /* !LINKSTO dsn.Dem.OBDPidSupport.Dem_ReadinessForceStatus_02,1 */
    /* force specific complete bits depend on config */
    PID01[1] &= Dem_ObdReadinessForceCompleteMask;

    /* copy content of the PID into the destination buffer */
    TS_MemCpy(PID01value, &PID01[0], 4U);
  }

  DBG_DEM_DCMREADDATAOFPID01_EXIT(Result, PID01value);
  return Result;
}
#endif /* #if (DEM_OBD_Support == STD_ON) */

/*----------------[Dem_DcmReadDataOfPID21]----------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID21(
  /* Deviation MISRA-1 */
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID21value)
{

  DBG_DEM_DCMREADDATAOFPID21_ENTRY(PID21value);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID21, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID21, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (PID21value == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID21, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT == STD_ON */
  {
#if (DEM_OBD_CENTRALIZED_PID21_HANDLING == STD_ON)
    uint32 TicksTraveled;

    /*
     * ENTER critical section to protect
     *       Dem_NvData.TicksTraveledWhileMILIsActivated -- 32 bit read
     */
    DEM_ENTER_EXCLUSIVE_AREA();

    TicksTraveled = Dem_NvData.TicksTraveledWhileMILIsActivated;

    /*
     * LEAVE critical section
     */
    DEM_EXIT_EXCLUSIVE_AREA();

    /* !LINKSTO DemChap7.8.3_Implicit1,1 */
    /* !LINKSTO Dem.OBD.PID$21.DistanceTraveled.DataAvailability,1 */
    /* !LINKSTO Dem.OBD.PID$21.DistanceTraveled.Scaling,1 */
    /* !LINKSTO Dem.OBD.PID$21.DistanceTraveled.Measurement,1 */
    /* !LINKSTO SWS_Dem_01098,1 */
    PID21value[0] = (uint8)(DEM_CONVERT_TICKS_TO_KILOMETERS(TicksTraveled) >> 8);
    PID21value[1] = (uint8) DEM_CONVERT_TICKS_TO_KILOMETERS(TicksTraveled);
#else
    /* !LINKSTO SWS_Dem_01096,1 */
    /* !LINKSTO Dem.OBD.DistanceTraveled.InternalPID21Calculation.NotAvailable,1 */
    PID21value[0] = 0U;
    PID21value[1] = 0U;
#endif /* (DEM_OBD_CENTRALIZED_PID21_HANDLING == STD_ON) */
  }

  DBG_DEM_DCMREADDATAOFPID21_EXIT(E_OK, PID21value);
  return E_OK;
}
/*----------------[Dem_DcmReadDataOfPID30]----------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID30(
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID30value)
{
  /* !LINKSTO Dem320,2, Dem.Pid.SupportedPid_5,1 */
  Std_ReturnType Result = E_OK;

  DBG_DEM_DCMREADDATAOFPID30_ENTRY(PID30value);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID30, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID30, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (PID30value == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID30, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
#if (defined DEM_OPCYC_WARMUP_ID)
    PID30value[0] = Dem_NvData.OBDWarmupCycleCtr;
#else
    /* !LINKSTO Dem.EB.OBD.PID30,1 */
    PID30value[0] = 0U;
#endif
  }

  DBG_DEM_DCMREADDATAOFPID30_EXIT(Result, PID30value);
  return Result;
}

/*----------------[Dem_DcmReadDataOfPID31]----------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID31(
  /* !LINKSTO Dem321,2, Dem.Pid.SupportedPid_6,1 */
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID31value)
{

  DBG_DEM_DCMREADDATAOFPID31_ENTRY(PID31value);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID31, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID31, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (PID31value == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID31, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
#if (DEM_OBD_CENTRALIZED_PID31_HANDLING == STD_ON)
    uint32 TicksTraveled;

    /*
     * ENTER critical section to protect
     *       Dem_NvData.TicksTraveledSinceDTCsCleared -- 32 bit read
     */
    DEM_ENTER_EXCLUSIVE_AREA();

    TicksTraveled = Dem_NvData.TicksTraveledSinceDTCsCleared;

    /*
     * LEAVE critical section
     */
    DEM_EXIT_EXCLUSIVE_AREA();

    PID31value[0] = (uint8)(DEM_CONVERT_TICKS_TO_KILOMETERS(TicksTraveled) >> 8);
    PID31value[1] = (uint8) DEM_CONVERT_TICKS_TO_KILOMETERS(TicksTraveled);
#else
    /* !LINKSTO Dem.OBD.PID31,1 */
    PID31value[0] = 0U;
    PID31value[1] = 0U;
#endif /* (DEM_OBD_CENTRALIZED_PID31_HANDLING == STD_ON) */
  }

  DBG_DEM_DCMREADDATAOFPID31_EXIT(E_OK, PID31value);
  return E_OK;
}

#if (DEM_OBD_Support == STD_ON)
/*----------------[Dem_DcmReadDataOfPID41]----------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID41(
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID41value)
{
  /* !LINKSTO Dem322,2, Dem.Pid.SupportedPid_7,1 */
  Std_ReturnType Result = E_OK;

  DBG_DEM_DCMREADDATAOFPID41_ENTRY(PID41value);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID41, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID41, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (PID41value == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID41, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    uint8 PID41[4] = {0x0U};
#if (DEM_MAX_EVENTS_PER_READINESS_GROUP == 0U)
    Dem_EventIdType EvId;
#else
    uint8 RdyGrpIdx;
#endif

    /* initialize PID41 */
    PID41[1] = Dem_ObdReadinessPID41ByteB;

#if (DEM_MAX_EVENTS_PER_READINESS_GROUP == 0U)
    /* !LINKSTO VCC_DEM_006_Req141v1,1 */
    /* Deviation TASKING-1 */
    for (EvId = 1U; EvId <= DEM_MAX_EVENTID; ++EvId)
    {
#if (DEM_EVENT_DISABLING_SUPPORT == STD_ON)
      if (Dem_GbiEvAvailable(EvId) != FALSE)
#endif
      {
        /* get the readiness group of the event */
        const Dem_EventOBDReadinessGroupType ReadinessGroup = Dem_GbiOBDReadinessGroup(EvId);

        /*
         * ENTER critical section to protect Dem_ReadinessEnableStatus
         *       call-context: Dcm (Task)
         */
        /* !LINKSTO dsn.Dem.ReadinessEnableStatus.CriticalSection,1 */
        DEM_ENTER_EXCLUSIVE_AREA();

        if (ReadinessGroup < DEM_OBD_RDY_PID01PID41_CNT)
        {
          if (DEM_MONITOR_IS_ENABLED(ReadinessGroup, Dem_NvData.ReadinessEnableStatus))
          {
            /* !LINKSTO Dem356,1 */
            DEM_SET_RDY_SUPPORTED(ReadinessGroup, PID41);
          }

          /* !LINKSTO Dem355,1 */
          /* !LINKSTO dsn.Dem.DcmReadDataOfPID41.CompletionStatusOfDisabledGroup,1 */
          /* check if event was not completed monitoring cycle - independent
           * of the disable status of the group */
          if (DEM_IS_EV_ST_BIT_TNCTOC_SET(EvId))
          {
            /* the completion status bit is set to 1 in PID$41 */
            DEM_SET_RDY_NOT_COMPLETE(ReadinessGroup, PID41);
          }
        }

        /*
         * LEAVE critical section
         */
        DEM_EXIT_EXCLUSIVE_AREA();
      }
    }
#else
    for (RdyGrpIdx = 0U; RdyGrpIdx < DEM_NUM_OBD_RDY_GROUPS; RdyGrpIdx++)
    {
      uint8 EvtIdx;
      /* get the current readiness group */
      const Dem_EventOBDReadinessGroupType ReadinessGroup =
          Dem_OBDReadinessGroupMap[RdyGrpIdx];

      if (ReadinessGroup < DEM_OBD_RDY_PID01PID41_CNT)
      {
        /* Deviation TASKING-1 */
        for (EvtIdx = 0U; EvtIdx < DEM_MAX_EVENTS_PER_READINESS_GROUP; EvtIdx++)
        {
          const Dem_EventIdType EventId = Dem_OBDReadinessGroup[RdyGrpIdx][EvtIdx];

          if (DEM_EVENT_ID_INVALID == EventId)
          {
            /* end of list of events; stop searching this readiness group */
            break;
          }

#if (DEM_EVENT_DISABLING_SUPPORT == STD_ON)
          if (Dem_GbiEvAvailable(EventId) != FALSE)
#endif
          {
            /*
             * ENTER critical section to protect ReadinessEnableStatus
             *       call-context: Dcm (Task)
             */
            /* !LINKSTO dsn.Dem.ReadinessEnableStatus.CriticalSection,1 */
            DEM_ENTER_EXCLUSIVE_AREA();

            if (DEM_MONITOR_IS_ENABLED(ReadinessGroup, Dem_NvData.ReadinessEnableStatus))
            {
              /* !LINKSTO Dem356,1 */
              DEM_SET_RDY_SUPPORTED(ReadinessGroup, PID41);
            }

            /* !LINKSTO Dem355,1 */
            /* !LINKSTO dsn.Dem.DcmReadDataOfPID41.CompletionStatusOfDisabledGroup,1 */
            /* check if event was not completed monitoring cycle - independent
             * of the disable status of the group */
            if (DEM_IS_EV_ST_BIT_TNCTOC_SET(EventId))
            {
              /* the completion status bit is set to 1 in PID$41 */
              DEM_SET_RDY_NOT_COMPLETE(ReadinessGroup, PID41);
            }

            /*
             * LEAVE critical section
             */
            DEM_EXIT_EXCLUSIVE_AREA();
          }
        }
      }
    }
#endif /* (DEM_MAX_EVENTS_PER_READINESS_GROUP == 0U) */

    /* !LINKSTO dsn.Dem.OBDPidSupport.Dem_ReadinessForceStatus_03,1 */
    /* force specific complete bits depend on config */
    PID41[1] &= Dem_ObdReadinessForceCompleteMask;

    /* copy content of the PID into the destination buffer */
    TS_MemCpy(PID41value, &PID41[0], 4U);
  }

  DBG_DEM_DCMREADDATAOFPID41_EXIT(Result, PID41value);
  return Result;
}
#endif /* #if (DEM_OBD_Support == STD_ON) */
/*----------------[Dem_DcmReadDataOfPID4D]----------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID4D(
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID4Dvalue)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_DCMREADDATAOFPID4D_ENTRY(PID4Dvalue);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID4D, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID4D, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (PID4Dvalue == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID4D, DEM_E_PARAM_POINTER);
  }
  else
#else /* DEM_DEV_ERROR_DETECT */
  TS_PARAM_UNUSED(PID4Dvalue);
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* function NOT fully implemented yet.
     * implementation is scheduled for future releases. */
  }

  DBG_DEM_DCMREADDATAOFPID4D_EXIT(Result, PID4Dvalue);
  return Result;
}

/*----------------[Dem_DcmReadDataOfPID4E]----------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID4E(
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID4Evalue)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_DCMREADDATAOFPID4E_ENTRY(PID4Evalue);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID4E, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID4E, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (PID4Evalue == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID4E, DEM_E_PARAM_POINTER);
  }
  else
#else /* DEM_DEV_ERROR_DETECT */
  TS_PARAM_UNUSED(PID4Evalue);
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* function NOT fully implemented yet.
     * implementation is scheduled for future releases. */
  }

  DBG_DEM_DCMREADDATAOFPID4E_EXIT(Result, PID4Evalue);
  return Result;
}

/*----------------[Dem_DcmReadDataOfPID1C]----------------------------------*/

#if (DEM_OBD_COMPLIANCY_ENABLED == STD_ON)
FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID1C(
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID1Cvalue)
{
  /* !LINKSTO Dem325,3, Dem.Pid.SupportedPid_3,2 */
  Std_ReturnType Result = E_OK;

  DBG_DEM_DCMREADDATAOFPID1C_ENTRY(PID1Cvalue);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID1C, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID1C, DEM_E_WRONG_CONTEXT);
  }
#endif/* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (PID1Cvalue == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID1C, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
    PID1Cvalue[0] = Dem_ObdCompliancyPID1CByte;
  }

  DBG_DEM_DCMREADDATAOFPID1C_EXIT(Result, PID1Cvalue);
  return Result;
}
#endif /* #if (DEM_OBD_COMPLIANCY_ENABLED == STD_ON) */

/* !LINKSTO Dem_OBD_0001,2 */
#if (DEM_OBD_Support == STD_ON)
/*----------------[Dem_DcmReadDataOfPID91]----------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfPID91(
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID91value)
{
  /* the API is implemented only as stub with return value always E_NOT_OK */
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_DCMREADDATAOFPID91_ENTRY(PID91value);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID91, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID91, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (PID91value == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfPID91, DEM_E_PARAM_POINTER);
  }
  else
#else /* DEM_DEV_ERROR_DETECT */
  TS_PARAM_UNUSED(PID91value);
#endif /* DEM_DEV_ERROR_DETECT */
  {
    /* function existing only as stub. */
  }

  DBG_DEM_DCMREADDATAOFPID91_EXIT(Result, PID91value);
  return Result;
}

/*----------------[Dem_DcmReadDataOfOBDFreezeFrame]----------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmReadDataOfOBDFreezeFrame(
  uint8                                   PID,
  uint8                                   DataElementIndexOfPID,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  DestBuffer,
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) BufSize)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_DCMREADDATAOFOBDFREEZEFRAME_ENTRY(
    PID, DataElementIndexOfPID, DestBuffer, BufSize);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfOBDFreezeFrame, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfOBDFreezeFrame, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (DestBuffer == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfOBDFreezeFrame, DEM_E_PARAM_POINTER);
  }
  else if (BufSize == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmReadDataOfOBDFreezeFrame, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT  */
  {
#if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)
#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
    /* local variable to protect concurrent read access to stored event Id */
    Dem_EventIdType StoredOBDFFEventId = DEM_EVENT_ID_INVALID;
#elif (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)
    /* local variable to hold the event entry index from multiple OBD FF */
    Dem_SizeEvMemEntryType SelectedEntryIndex = 0U;
    boolean                EntryFound = FALSE;
#endif
    /*
     * ENTER critical section
     */
    DEM_ENTER_EXCLUSIVE_AREA();

#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)

    StoredOBDFFEventId = DEM_OBDFF_STORED_EVENT_ID;
#elif (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)

    EntryFound = Dem_SelectEntryOfOBDFreezeFrameEvent(&SelectedEntryIndex, TRUE);
#endif
    /*
     * LEAVE critical section
     */
    DEM_EXIT_EXCLUSIVE_AREA();

#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
    /* !LINKSTO dsn.Dem.OBD.ReadDataOfOBDFreezeFrame.EventId,1 */
    /* check whether any OBD freeze frame is stored */
    if (StoredOBDFFEventId != DEM_EVENT_ID_INVALID)
    {
#if ( (DEM_EVENT_DISABLING_SUPPORT == STD_ON) || \
      (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION) )
      if (Dem_GetVisibleCmbDTC(DEM_DTC_FORMAT_OBD, StoredOBDFFEventId) == DEM_NO_DTC)
      {
        /* stored OBD freeze frame event is disabled via calibration or DTC is
         * not visible */
        /* result remains E_NOT_OK */
      }
      else
#endif /* (DEM_EVENT_DISABLING_SUPPORT == STD_ON) || \
          (DEM_SUPPRESSION_SUPPORT == DEM_DTC_SUPPRESSION) */
#elif (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)
    if (EntryFound == TRUE)
#endif
      {
        uint8_least PidClassIterator;
        Dem_SizeEntryDataType StartByte = 0U;

        /* find dedicated PID in OBD freeze frame */
        /* Deviation TASKING-1 */
        for (PidClassIterator = 0U;
             PidClassIterator < DEM_NUM_OF_PIDS_IN_OBDFF;
             PidClassIterator++)
        {
          if ( (Dem_PidClass[PidClassIterator].ParameterId == PID) &&
               (Dem_PidClass[PidClassIterator].ParameterId != DEM_PID_UNUSED) )
          {
            /* if PID is found in PID class, check whether parameter
             * DataElementIndexOfPID is valid
             */
            if (DataElementIndexOfPID < Dem_PidClass[PidClassIterator]
                                              .NumSegDataElements)
            {
              break;
            }
          }

          /* calculate start position index of the PID */
          StartByte += (Dem_SizeEntryDataType)Dem_PidClass[PidClassIterator].Size;
        }

        /* !LINKSTO dsn.Dem.OBD.ReadDataOfOBDFreezeFrame.Pid,1 */
        /* only perform further actions if PID was found and parameter
         * DataElementIndexOfPID is valid. Otherwise return E_NOT_OK */
        if (PidClassIterator < DEM_NUM_OF_PIDS_IN_OBDFF)
        {
          /* !LINKSTO dsn.Dem.OBD.ReadDataOfOBDFreezeFrame.BufSize,1 */
          /* check whether given BufSize has the right size,
           * otherwise return E_NOT_OK */
          if (*BufSize >= Dem_PidClass[PidClassIterator]
                                .ObdFFPidDataElementSize[DataElementIndexOfPID])
          {
            P2VAR(Dem_EntryDataType, AUTOMATIC, DEM_VAR_CLEARED) OBDFFStoredData;
            /* calculate index of OBDFFData[]
             * --> pos. of PID in FF + index of data element in PID */
            /* !LINKSTO Dem.ASR431.SWS_Dem_00597,1 */
            const Dem_SizeEntryDataType ObdFFDataIdx = StartByte + Dem_PidClass[
              PidClassIterator].ObdFFPidDataElementOffset[DataElementIndexOfPID];

            /* !LINKSTO dsn.Dem.OBD.ReadDataOfOBDFreezeFrame.CriticalSection,1 */
            /*
             * ENTER critical section to protect the event memory
             *       call-context: Dem (Task)
             */
            DEM_ENTER_EXCLUSIVE_AREA();

#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)
            OBDFFStoredData = DEM_OBDFF_ENTRY_FFDATA_LINK(SelectedEntryIndex);
#elif (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
            OBDFFStoredData = DEM_OBDFF_STORED_DATA;
#endif
            /* !LINKSTO dsn.Dem.OBD.ReadDataOfOBDFreezeFrame.DestBuffer,1
             */
            /* !LINKSTO Dem.ASR431.SWS_Dem_00596,1 */
            /* write dedicated data element into destination buffer */
            TS_MemCpy(DestBuffer,
              &OBDFFStoredData[ObdFFDataIdx],
              Dem_PidClass[PidClassIterator].ObdFFPidDataElementSize[DataElementIndexOfPID]);

            /*
             * LEAVE critical section
             */
            DEM_EXIT_EXCLUSIVE_AREA();

            /* !LINKSTO dsn.Dem.OBD.ReadDataOfOBDFreezeFrame.BufsizeOut,1 */
            /* return size of data written into DestBuffer */
            *BufSize = Dem_PidClass[PidClassIterator]
                                .ObdFFPidDataElementSize[DataElementIndexOfPID];

            Result = E_OK;
          }
        } /* return E_NOT_OK, if no OBD freeze frame stored,
           * or PID not found in OBD freeze frame
           * or provided BufSize is to small
           * or PID is DEM_PID_UNUSED */
      }
#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
    }
#endif /* DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE) */
#else
  TS_PARAM_UNUSED(PID);
  TS_PARAM_UNUSED(DataElementIndexOfPID);
  TS_PARAM_UNUSED(DestBuffer);
  TS_PARAM_UNUSED(BufSize);
#endif /* (DEM_NUM_OF_PIDS_IN_OBDFF > 0U) */
  }

  DBG_DEM_DCMREADDATAOFOBDFREEZEFRAME_EXIT(
    Result, PID, DataElementIndexOfPID, DestBuffer, BufSize);
  return Result;
}

/*----------------[Dem_DcmGetDTCOfOBDFreezeFrame]------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetDTCOfOBDFreezeFrame(
  uint8                                   FrameNumber,
  P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) DTC,
  Dem_DTCFormatType                       DTCFormat)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_DCMGETDTCOFOBDFREEZEFRAME_ENTRY(FrameNumber, DTC, DTCFormat);

#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmGetDTCOfOBDFreezeFrame, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmGetDTCOfOBDFreezeFrame, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (DTC == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmGetDTCOfOBDFreezeFrame, DEM_E_PARAM_POINTER);
  }
  else if ( (DTCFormat != DEM_DTC_FORMAT_OBD) &&
            (DTCFormat != DEM_DTC_FORMAT_UDS) &&
            (DTCFormat != DEM_DTC_FORMAT_J1939) )
  {
    DEM_REPORT_ERROR(DEM_SID_DcmGetDTCOfOBDFreezeFrame, DEM_E_PARAM_DATA);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT */
  {
#if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U)
    /* !LINKSTO dsn.Dem.OBD.GetDTCOfOBDFreezeFrame.FrameNumber,1 */
    /* check whether provided FrameNumber == '0', otherwise return E_NOT_OK */
    if (FrameNumber == DEM_OBD_FF_RECORD_NUM)
    {
      if ( (DTCFormat == DEM_DTC_FORMAT_OBD) || (DTCFormat == DEM_DTC_FORMAT_UDS) )
      {
        /* local variable to protect concurrent read access to stored event Id */
        Dem_EventIdType StoredOBDFFEventId = DEM_EVENT_ID_INVALID;
        boolean         EntryFound = FALSE;

#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
        /*
         * ENTER critical section
         */
        DEM_ENTER_EXCLUSIVE_AREA();

        StoredOBDFFEventId = DEM_OBDFF_STORED_EVENT_ID;
        /*
         * LEAVE critical section
         */
        DEM_EXIT_EXCLUSIVE_AREA();

        /* !LINKSTO dsn.Dem.OBD.GetDTCOfOBDFreezeFrame.EventId,1 */
        /* check whether any OBD freeze frame is stored */
        if (StoredOBDFFEventId != DEM_EVENT_ID_INVALID)
        {
          EntryFound = TRUE;
        }
#elif (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)
        Dem_SizeEvMemEntryType SelectedEntryIndex = 0U;

        /*
         * ENTER critical section
         */
        DEM_ENTER_EXCLUSIVE_AREA();

        EntryFound = Dem_SelectEntryOfOBDFreezeFrameEvent(&SelectedEntryIndex, TRUE);

        if (EntryFound == TRUE)
        {
          /* get selected entry event Id from primary memory */
          StoredOBDFFEventId =
          Dem_EventMem[(DEM_DTC_ORIGIN_PRIMARY_MEMORY - 1U)][SelectedEntryIndex].EventId;
        }

        /*
         * LEAVE critical section
         */
        DEM_EXIT_EXCLUSIVE_AREA();
#endif /* (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE) */
        if (EntryFound == TRUE)
        {
          /* For storing temporarily DTC value */
          uint32 DtcTempValue = DEM_NO_DTC;

          /* !LINKSTO dsn.Dem.OBD.GetDTCOfOBDFreezeFrame.GetVisibleCmbDTC,1 */
          /* !LINKSTO dsn.Dem.OBD.GetDTCOfOBDFreezeFrame.Dtc,1 */
          /* !LINKSTO dsn.Dem.OBD.GetDTCOfOBDFreezeFrame.NonExistingDTCFormat,1 */
          /* !LINKSTO Dem.ASR431.SWS_Dem_00623,1 */
          /* !LINKSTO Dem.Pid.SupportedPid_2,1 */
          /* OBD freeze frames will only be stored if the event has an OBD DTC
           * configured (see: Dem_ProcessEventEntry()), otherwise it's no OBD event.
           * Nevertheless the result needs to be checked because the DTC might not be
           * visible (suppressed) or the event might be unavailable or the UDS format is
           * requested but the event is not assigned with a UDS DTC. */
          DtcTempValue = Dem_GetVisibleCmbDTC(DTCFormat, StoredOBDFFEventId);

          if (DtcTempValue != DEM_NO_DTC)
          {
            *DTC = DtcTempValue;
            Result = E_OK;
          }
        }
      }
      else
      {
        /* J1939 format is not supported, the function will exit with E_NOT_OK */
      }
    }
#else
  TS_PARAM_UNUSED(FrameNumber);
  TS_PARAM_UNUSED(DTC);
  TS_PARAM_UNUSED(DTCFormat);
#endif /* #if (DEM_NUM_OF_PIDS_IN_OBDFF > 0U) */
  } /* return E_NOT_OK if FrameNumber != 0 or EventId == DEM_EVENT_ID_INVALID */

  DBG_DEM_DCMGETDTCOFOBDFREEZEFRAME_EXIT(Result, FrameNumber, DTC, DTCFormat);
  return Result;
}

/*----------------[Dem_DcmGetAvailableOBDMIDs]------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetAvailableOBDMIDs(
  uint8                                   Obdmid,
  P2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) Obdmidvalue)
{
  /* function NOT fully implemented yet and will always return E_NOT_OK.
   * implementation is scheduled for future releases. */
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_DCMGETAVAILABLEOBDIDS_ENTRY(Obdmid, Obdmidvalue);
#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmGetAvailableOBDMIDs, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmGetAvailableOBDMIDs, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (Obdmidvalue == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmGetAvailableOBDMIDs, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT  */
  {
    TS_PARAM_UNUSED(Obdmid);
    TS_PARAM_UNUSED(Obdmidvalue);
  }
  DBG_DEM_DCMGETAVAILABLEOBDIDS_EXIT(Result, Obdmid, Obdmidvalue);

  return Result;
}

/*----------------[Dem_DcmGetNumTIDsOfOBDMID]-------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetNumTIDsOfOBDMID(
  uint8                                   Obdmid,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  numberOfTIDs)
{
  /* function NOT fully implemented yet and will always return E_NOT_OK.
   * implementation is scheduled for future releases. */
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_DCMGETNUMTIDSOFOBDMID_ENTRY(Obdmid, numberOfTIDs);
#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmGetNumTIDsOfOBDMID, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR(DEM_SID_DcmGetNumTIDsOfOBDMID, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if (numberOfTIDs == NULL_PTR)
  {
    DEM_REPORT_ERROR(DEM_SID_DcmGetNumTIDsOfOBDMID, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT  */
  {
    TS_PARAM_UNUSED(Obdmid);
    TS_PARAM_UNUSED(numberOfTIDs);
  }
  DBG_DEM_DCMGETNUMTIDSOFOBDMID_EXIT(Result, Obdmid, numberOfTIDs);

  return Result;
}

/*----------------[Dem_DcmGetDTRData]---------------------------------------*/

FUNC(Std_ReturnType, DEM_CODE) Dem_DcmGetDTRData(
  uint8                                   Obdmid,
  uint8                                   TIDindex,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  TIDvalue,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA)  UaSID,
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Testvalue,
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Lowlimvalue,
  P2VAR(uint16, AUTOMATIC, DEM_APPL_DATA) Upplimvalue)
{
  /* function NOT fully implemented yet and will always return E_NOT_OK.
   * implementation is scheduled for future releases. */
  Std_ReturnType Result = E_NOT_OK;

  DBG_DEM_DCMGETDTRDATA_ENTRY(
    Obdmid, TIDindex, TIDvalue, UaSID, Testvalue, Lowlimvalue, Upplimvalue);
#if (DEM_DEV_ERROR_DETECT == STD_ON)
  if (Dem_InitializationState != DEM_INITIALIZED)
  {
    DEM_REPORT_ERROR( DEM_SID_DcmGetDTRData, DEM_E_UNINIT);
  }
#if (DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON)
  else if (GetCoreID() != DEM_MASTER_INSTANCE)
  {
    /* !LINKSTO Dem.BSWDistribution.ContextCheckForNonDistributedFunctionalities,1 */
    DEM_REPORT_ERROR( DEM_SID_DcmGetDTRData, DEM_E_WRONG_CONTEXT);
  }
#endif /* DEM_BSW_DISTRIBUTION_FOR_BSW_EVENT_REPORTING == STD_ON */
  else if ( (TIDvalue    == NULL_PTR) ||
            (UaSID       == NULL_PTR) ||
            (Testvalue   == NULL_PTR) ||
            (Lowlimvalue == NULL_PTR) ||
            (Upplimvalue == NULL_PTR) )
  {
    DEM_REPORT_ERROR( DEM_SID_DcmGetDTRData, DEM_E_PARAM_POINTER);
  }
  else
#endif /* DEM_DEV_ERROR_DETECT  */
  {
    TS_PARAM_UNUSED(Obdmid);
    TS_PARAM_UNUSED(TIDindex);
    TS_PARAM_UNUSED(TIDvalue);
    TS_PARAM_UNUSED(UaSID);
    TS_PARAM_UNUSED(Testvalue);
    TS_PARAM_UNUSED(Lowlimvalue);
    TS_PARAM_UNUSED(Upplimvalue);
  }
  DBG_DEM_DCMGETDTRDATA_EXIT(Result,
    Obdmid, TIDindex, TIDvalue, UaSID, Testvalue, Lowlimvalue, Upplimvalue);

  return Result;
}

/*==================[internal function definitions]=========================*/

/*----------------[Dem_UpdatePID01value]----------------------------------*/
STATIC FUNC(boolean, DEM_CODE) Dem_UpdatePID01value(
  const Dem_EventIdType EventId,
  const uint16 ReadinessGroup,
  const boolean ReadinessGroupChecked,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) PID01value)
{
  boolean Result = ReadinessGroupChecked;

  DBG_DEM_DEM_UPDATEPID01VALUE_ENTRY(EventId, ReadinessGroup,
                                     ReadinessGroupChecked, PID01value);

  /* check if ReadinessGroup is not yet checked */
  if (ReadinessGroupChecked == FALSE)
  {
    /* set supported bit of the readiness group */
    DEM_SET_RDY_SUPPORTED(ReadinessGroup, PID01value);

    /* !LINKSTO Dem354,1 */
    /* check if event was not completed since last clear OR if it is pending */
    if (DEM_IS_EV_ST_BIT_TNCSLC_SET(EventId) ||
        DEM_IS_EV_ST_BIT_PDTC_SET(EventId))
    {
      /* set ReadinessGroup to NOT COMPLETE */
      DEM_SET_RDY_NOT_COMPLETE(ReadinessGroup, PID01value);

      /* mark ReadinessGroup as checked */
      Result = TRUE;
    }
  }

  /* check if event is confirmed (CDTC bit is set) */
  if (DEM_IS_EV_ST_BIT_CDTC_SET(EventId))
  {
    /* the check that ReadinessGroup is reported in PID01
     * (ReadinessGroup < DEM_OBD_RDY_PID01PID41_CNT)
     * is done in the upper function call (Dem_DcmReadDataOfPID01) */
    /* set ReadinessGroup to COMPLETE */
    DEM_SET_RDY_COMPLETE(ReadinessGroup, PID01value);

    /* if at least one event in this group is confirmed,
     * mark it as checked, so it can no more set to NOT COMPLETE */
    Result = TRUE;
  }

  DBG_DEM_DEM_UPDATEPID01VALUE_EXIT(ReadinessGroupChecked, EventId,
                                    ReadinessGroup, ReadinessGroupChecked,
                                    PID01value);

  return Result;
}

#endif /* #if (DEM_OBD_Support == STD_ON) */

#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>

/*==================[end of file]===========================================*/
