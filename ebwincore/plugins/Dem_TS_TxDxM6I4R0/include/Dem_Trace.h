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
#ifndef DEM_TRACE_H
#define DEM_TRACE_H

/*==================[inclusions]============================================*/


#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif

/*==================[macros]================================================*/

#ifndef DBG_DEM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Dem_GetVersionInfo() */
#define DBG_DEM_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_DEM_GETVERSIONINFO_EXIT
/** \brief Exit point of function Dem_GetVersionInfo() */
#define DBG_DEM_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_DEM_PREINIT_ENTRY
/** \brief Entry point of function Dem_PreInit() */
#define DBG_DEM_PREINIT_ENTRY(a)
#endif

#ifndef DBG_DEM_PREINIT_EXIT
/** \brief Exit point of function Dem_PreInit() */
#define DBG_DEM_PREINIT_EXIT(a)
#endif

#ifndef DBG_DEM_INIT_ENTRY
/** \brief Entry point of function Dem_Init() */
#define DBG_DEM_INIT_ENTRY()
#endif

#ifndef DBG_DEM_INIT_EXIT
/** \brief Exit point of function Dem_Init() */
#define DBG_DEM_INIT_EXIT()
#endif

#ifndef DBG_DEM_SHUTDOWN_ENTRY
/** \brief Entry point of function Dem_Shutdown() */
#define DBG_DEM_SHUTDOWN_ENTRY()
#endif

#ifndef DBG_DEM_SHUTDOWN_EXIT
/** \brief Exit point of function Dem_Shutdown() */
#define DBG_DEM_SHUTDOWN_EXIT()
#endif

#ifndef DBG_DEM_REPORTERRORSTATUS_ENTRY
/** \brief Entry point of function Dem_ReportErrorStatus() */
#define DBG_DEM_REPORTERRORSTATUS_ENTRY(a, b)
#endif

#ifndef DBG_DEM_REPORTERRORSTATUS_EXIT
/** \brief Exit point of function Dem_ReportErrorStatus() */
#define DBG_DEM_REPORTERRORSTATUS_EXIT(a, b)
#endif

#ifndef DBG_DEM_SETEVENTSTATUS_ENTRY
/** \brief Entry point of function Dem_SetEventStatus() */
#define DBG_DEM_SETEVENTSTATUS_ENTRY(a, b)
#endif

#ifndef DBG_DEM_SETEVENTSTATUS_EXIT
/** \brief Exit point of function Dem_SetEventStatus() */
#define DBG_DEM_SETEVENTSTATUS_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_RESETEVENTSTATUS_ENTRY
/** \brief Entry point of function Dem_ResetEventStatus() */
#define DBG_DEM_RESETEVENTSTATUS_ENTRY(a)
#endif

#ifndef DBG_DEM_RESETEVENTSTATUS_EXIT
/** \brief Exit point of function Dem_ResetEventStatus() */
#define DBG_DEM_RESETEVENTSTATUS_EXIT(a, b)
#endif

#ifndef DBG_DEM_RESETEVENTDEBOUNCESTATUS_ENTRY
/** \brief Entry point of function Dem_ResetEventDebounceStatus() */
#define DBG_DEM_RESETEVENTDEBOUNCESTATUS_ENTRY(a,b)
#endif

#ifndef DBG_DEM_RESETEVENTDEBOUNCESTATUS_EXIT
/** \brief Exit point of function Dem_ResetEventDebounceStatus() */
#define DBG_DEM_RESETEVENTDEBOUNCESTATUS_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_PRESTOREFREEZEFRAME_ENTRY
/** \brief Entry point of function Dem_PrestoreFreezeFrame() */
#define DBG_DEM_PRESTOREFREEZEFRAME_ENTRY(a)
#endif

#ifndef DBG_DEM_PRESTOREFREEZEFRAME_EXIT
/** \brief Exit point of function Dem_PrestoreFreezeFrame() */
#define DBG_DEM_PRESTOREFREEZEFRAME_EXIT(a, b)
#endif

#ifndef DBG_DEM_CLEARPRESTOREDFREEZEFRAME_ENTRY
/** \brief Entry point of function Dem_ClearPrestoredFreezeFrame() */
#define DBG_DEM_CLEARPRESTOREDFREEZEFRAME_ENTRY(a)
#endif

#ifndef DBG_DEM_CLEARPRESTOREDFREEZEFRAME_EXIT
/** \brief Exit point of function Dem_ClearPrestoredFreezeFrame() */
#define DBG_DEM_CLEARPRESTOREDFREEZEFRAME_EXIT(a, b)
#endif

#ifndef DBG_DEM_SETOPERATIONCYCLESTATE_ENTRY
/** \brief Entry point of function Dem_SetOperationCycleState() */
#define DBG_DEM_SETOPERATIONCYCLESTATE_ENTRY(a, b)
#endif

#ifndef DBG_DEM_SETOPERATIONCYCLESTATE_EXIT
/** \brief Exit point of function Dem_SetOperationCycleState() */
#define DBG_DEM_SETOPERATIONCYCLESTATE_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_SETCYCLEQUALIFIED_ENTRY
/** \brief Entry point of function Dem_SetCycleQualified() */
#define DBG_DEM_SETCYCLEQUALIFIED_ENTRY(a)
#endif

#ifndef DBG_DEM_SETCYCLEQUALIFIED_EXIT
/** \brief Exit point of function Dem_SetCycleQualified() */
#define DBG_DEM_SETCYCLEQUALIFIED_EXIT(a, b)
#endif

#ifndef DBG_DEM_PROCESSOCC2BASEDHEALING_ENTRY
/** \brief Entry point of function Dem_ProcessOCC2BasedHealing() */
#define DBG_DEM_PROCESSOCC2BASEDHEALING_ENTRY(a)
#endif

#ifndef DBG_DEM_PROCESSOCC2BASEDHEALING_EXIT
/** \brief Exit point of function Dem_ProcessOCC2BasedHealing() */
#define DBG_DEM_PROCESSOCC2BASEDHEALING_EXIT(a)
#endif

#ifndef DBG_DEM_OPCYCLEPROCESSCYCLESTART_ENTRY
/** \brief Entry point of function Dem_OpCycleProcessCycleStart() */
#define DBG_DEM_OPCYCLEPROCESSCYCLESTART_ENTRY(a)
#endif

#ifndef DBG_DEM_OPCYCLEPROCESSCYCLESTART_EXIT
/** \brief Exit point of function Dem_OpCycleProcessCycleStart() */
#define DBG_DEM_OPCYCLEPROCESSCYCLESTART_EXIT(a)
#endif

#ifndef DBG_DEM_OPCYCLEPROCESSEVENTCYCLESTART_ENTRY
/** \brief Entry point of function Dem_OpCycleProcessEventCycleStart() */
#define DBG_DEM_OPCYCLEPROCESSEVENTCYCLESTART_ENTRY(a)
#endif

#ifndef DBG_DEM_OPCYCLEPROCESSEVENTCYCLESTART_EXIT
/** \brief Exit point of function Dem_OpCycleProcessEventCycleStart() */
#define DBG_DEM_OPCYCLEPROCESSEVENTCYCLESTART_EXIT(a)
#endif

#ifndef DBG_DEM_OPCYCLEPROCESSCYCLEEND_ENTRY
/** \brief Entry point of function Dem_OpCycleProcessCycleEnd() */
#define DBG_DEM_OPCYCLEPROCESSCYCLEEND_ENTRY(a)
#endif

#ifndef DBG_DEM_OPCYCLEPROCESSCYCLEEND_EXIT
/** \brief Exit point of function Dem_OpCycleProcessCycleEnd() */
#define DBG_DEM_OPCYCLEPROCESSCYCLEEND_EXIT(a)
#endif

#ifndef DBG_DEM_OPCYCLEPROCESSEVENTCYCLEEND_ENTRY
/** \brief Entry point of function Dem_OpCycleProcessEventCycleEnd() */
#define DBG_DEM_OPCYCLEPROCESSEVENTCYCLEEND_ENTRY(a)
#endif

#ifndef DBG_DEM_OPCYCLEPROCESSEVENTCYCLEEND_EXIT
/** \brief Exit point of function Dem_OpCycleProcessEventCycleEnd() */
#define DBG_DEM_OPCYCLEPROCESSEVENTCYCLEEND_EXIT(a)
#endif

#ifndef DBG_DEM_SETOPERATIONCYCLECNTVALUE_ENTRY
/** \brief Entry point of function Dem_SetOperationCycleCntValue() */
#define DBG_DEM_SETOPERATIONCYCLECNTVALUE_ENTRY(a, b)
#endif

#ifndef DBG_DEM_SETOPERATIONCYCLECNTVALUE_EXIT
/** \brief Exit point of function Dem_SetOperationCycleCntValue() */
#define DBG_DEM_SETOPERATIONCYCLECNTVALUE_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_SETAGINGCYCLESTATE_ENTRY
/** \brief Entry point of function Dem_SetAgingCycleState() */
#define DBG_DEM_SETAGINGCYCLESTATE_ENTRY(a)
#endif

#ifndef DBG_DEM_SETAGINGCYCLESTATE_EXIT
/** \brief Exit point of function Dem_SetAgingCycleState() */
#define DBG_DEM_SETAGINGCYCLESTATE_EXIT(a, b)
#endif

#ifndef DBG_DEM_SETAGINGCYCLECOUNTERVALUE_ENTRY
/** \brief Entry point of function Dem_SetAgingCycleCounterValue() */
#define DBG_DEM_SETAGINGCYCLECOUNTERVALUE_ENTRY(a)
#endif

#ifndef DBG_DEM_SETAGINGCYCLECOUNTERVALUE_EXIT
/** \brief Exit point of function Dem_SetAgingCycleCounterValue() */
#define DBG_DEM_SETAGINGCYCLECOUNTERVALUE_EXIT(a, b)
#endif

#ifndef DBG_DEM_READEXTERNALDATAELEMENT_ENTRY
/** \brief Entry point of function Dem_ReadExternalDataElement() */
#define DBG_DEM_READEXTERNALDATAELEMENT_ENTRY(a, b, c);
#endif

#ifndef DBG_DEM_READEXTERNALDATAELEMENT_EXIT
/** \brief Exit point of function Dem_ReadExternalDataElement() */
#define DBG_DEM_READEXTERNALDATAELEMENT_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_GETEVENTSTATUS_ENTRY
/** \brief Entry point of function Dem_GetEventStatus() */
#define DBG_DEM_GETEVENTSTATUS_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETEVENTSTATUS_EXIT
/** \brief Exit point of function Dem_GetEventStatus() */
#define DBG_DEM_GETEVENTSTATUS_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETEVENTFAILED_ENTRY
/** \brief Entry point of function Dem_GetEventFailed() */
#define DBG_DEM_GETEVENTFAILED_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETEVENTFAILED_EXIT
/** \brief Exit point of function Dem_GetEventFailed() */
#define DBG_DEM_GETEVENTFAILED_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETEVENTTESTED_ENTRY
/** \brief Entry point of function Dem_GetEventTested() */
#define DBG_DEM_GETEVENTTESTED_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETEVENTTESTED_EXIT
/** \brief Exit point of function Dem_GetEventTested() */
#define DBG_DEM_GETEVENTTESTED_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETDTCOFEVENT_ENTRY
/** \brief Entry point of function Dem_GetDTCOfEvent() */
#define DBG_DEM_GETDTCOFEVENT_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_GETDTCOFEVENT_EXIT
/** \brief Exit point of function Dem_GetDTCOfEvent() */
#define DBG_DEM_GETDTCOFEVENT_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_SETENABLECONDITION_ENTRY
/** \brief Entry point of function Dem_SetEnableCondition() */
#define DBG_DEM_SETENABLECONDITION_ENTRY(a, b)
#endif

#ifndef DBG_DEM_SETENABLECONDITION_EXIT
/** \brief Exit point of function Dem_SetEnableCondition() */
#define DBG_DEM_SETENABLECONDITION_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_SETSTORAGECONDITION_ENTRY
/** \brief Entry point of function Dem_SetStorageCondition() */
#define DBG_DEM_SETSTORAGECONDITION_ENTRY(a, b)
#endif

#ifndef DBG_DEM_SETSTORAGECONDITION_EXIT
/** \brief Exit point of function Dem_SetStorageCondition() */
#define DBG_DEM_SETSTORAGECONDITION_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETFAULTDETECTIONCOUNTER_ENTRY
/** \brief Entry point of function Dem_GetFaultDetectionCounter() */
#define DBG_DEM_GETFAULTDETECTIONCOUNTER_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETFAULTDETECTIONCOUNTER_EXIT
/** \brief Exit point of function Dem_GetFaultDetectionCounter() */
#define DBG_DEM_GETFAULTDETECTIONCOUNTER_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETINDICATORSTATUS_ENTRY
/** \brief Entry point of function Dem_GetIndicatorStatus() */
#define DBG_DEM_GETINDICATORSTATUS_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETINDICATORSTATUS_EXIT
/** \brief Exit point of function Dem_GetIndicatorStatus() */
#define DBG_DEM_GETINDICATORSTATUS_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_SELECTFREEZEFRAMEDATA_ENTRY
/** \brief Entry point of function Dem_SelectFreezeFrameData() */
#define DBG_DEM_SELECTFREEZEFRAMEDATA_ENTRY(a, b)
#endif

#ifndef DBG_DEM_SELECTFREEZEFRAMEDATA_EXIT
/** \brief Exit point of function Dem_SelectFreezeFrameData() */
#define DBG_DEM_SELECTFREEZEFRAMEDATA_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_RESETDATARECORDSELECTOR_ENTRY
/** \brief Entry point of function Dem_ResetDataRecordSelector() */
#define DBG_DEM_RESETDATARECORDSELECTOR_ENTRY(a)
#endif

#ifndef DBG_DEM_RESETDATARECORDSELECTOR_EXIT
/** \brief Exit point of function Dem_ResetDataRecordSelector() */
#define DBG_DEM_RESETDATARECORDSELECTOR_EXIT(a)
#endif

#ifndef DBG_DEM_INITDATARECORDSELECTOR_ENTRY
/** \brief Entry point of function Dem_InitDataRecordSelector() */
#define DBG_DEM_INITDATARECORDSELECTOR_ENTRY()
#endif

#ifndef DBG_DEM_INITDATARECORDSELECTOR_EXIT
/** \brief Exit point of function Dem_InitDataRecordSelector() */
#define DBG_DEM_INITDATARECORDSELECTOR_EXIT()
#endif

#ifndef DBG_DEM_FINDNEXTFFRECFORREPORTING_ENTRY
/** \brief Entry point of function Dem_FindNextFFRecForReporting() */
#define DBG_DEM_FINDNEXTFFRECFORREPORTING_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_FINDNEXTFFRECFORREPORTING_EXIT
/** \brief Exit point of function Dem_FindNextFFRecForReporting() */
#define DBG_DEM_FINDNEXTFFRECFORREPORTING_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_FINDNEXTEDRECFORREPORTING_ENTRY
/** \brief Entry point of function FindNextEDRecForReporting() */
#define DBG_DEM_FINDNEXTEDRECFORREPORTING_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_FINDNEXTEDRECFORREPORTING_EXIT
/** \brief Exit point of function FindNextEDRecForReporting() */
#define DBG_DEM_FINDNEXTEDRECFORREPORTING_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_SELECTEXTENDEDDATARECORD_ENTRY
/** \brief Entry point of function Dem_SelectExtendedDataRecord() */
#define DBG_DEM_SELECTEXTENDEDDATARECORD_ENTRY(a, b)
#endif

#ifndef DBG_DEM_SELECTEXTENDEDDATARECORD_EXIT
/** \brief Exit point of function Dem_SelectExtendedDataRecord() */
#define DBG_DEM_SELECTEXTENDEDDATARECORD_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETEVENTFREEZEFRAMEDATA_ENTRY
/** \brief Entry point of function Dem_GetEventFreezeFrameData() */
#define DBG_DEM_GETEVENTFREEZEFRAMEDATA_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DEM_GETEVENTFREEZEFRAMEDATA_EXIT
/** \brief Exit point of function Dem_GetEventFreezeFrameData() */
#define DBG_DEM_GETEVENTFREEZEFRAMEDATA_EXIT(a, b, c, d, e, f)
#endif

#ifndef DBG_DEM_ASR42_GETEVENTFREEZEFRAMEDATA_ENTRY
/** \brief Entry point of function Dem_ASR42_GetEventFreezeFrameData() */
#define DBG_DEM_ASR42_GETEVENTFREEZEFRAMEDATA_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DEM_ASR42_GETEVENTFREEZEFRAMEDATA_EXIT
/** \brief Exit point of function Dem_ASR42_GetEventFreezeFrameData() */
#define DBG_DEM_ASR42_GETEVENTFREEZEFRAMEDATA_EXIT(a, b, c, d, e, f)
#endif

#ifndef DBG_DEM_GETEVENTEXTENDEDDATARECORD_ENTRY
/** \brief Entry point of function Dem_GetEventExtendedDataRecord() */
#define DBG_DEM_GETEVENTEXTENDEDDATARECORD_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_GETEVENTEXTENDEDDATARECORD_EXIT
/** \brief Exit point of function Dem_GetEventExtendedDataRecord() */
#define DBG_DEM_GETEVENTEXTENDEDDATARECORD_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_GETDEBOUNCINGOFEVENT_ENTRY
/** \brief Entry point of function Dem_GetDebouncingOfEvent() */
#define DBG_DEM_GETDEBOUNCINGOFEVENT_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETDEBOUNCINGOFEVENT_EXIT
/** \brief Exit point of function Dem_GetDebouncingOfEvent() */
#define DBG_DEM_GETDEBOUNCINGOFEVENT_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETEVENTEXTENDEDDATARECORDEX_ENTRY
/** \brief Entry point of function Dem_GetEventExtendedDataRecordEx() */
#define DBG_DEM_GETEVENTEXTENDEDDATARECORDEX_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_GETEVENTEXTENDEDDATARECORDEX_EXIT
/** \brief Exit point of function Dem_GetEventExtendedDataRecordEx() */
#define DBG_DEM_GETEVENTEXTENDEDDATARECORDEX_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_GETEVENTFREEZEFRAMEDATAEX_ENTRY
/** \brief Entry point of function Dem_GetEventFreezeFrameDataEx() */
#define DBG_DEM_GETEVENTFREEZEFRAMEDATAEX_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DEM_GETEVENTFREEZEFRAMEDATAEX_EXIT
/** \brief Exit point of function Dem_GetEventFreezeFrameDataEx() */
#define DBG_DEM_GETEVENTFREEZEFRAMEDATAEX_EXIT(a, b, c, d, e, f)
#endif

#ifndef DBG_DEM_GETEVENTMEMORYOVERFLOW_ENTRY
/** \brief Entry point of function Dem_GetEventMemoryOverflow() */
#define DBG_DEM_GETEVENTMEMORYOVERFLOW_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETEVENTMEMORYOVERFLOW_EXIT
/** \brief Exit point of function Dem_GetEventMemoryOverflow() */
#define DBG_DEM_GETEVENTMEMORYOVERFLOW_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETNUMBEROFEVENTMEMORYENTRIES_ENTRY
/** \brief Entry point of function Dem_GetNumberOfEventMemoryEntries() */
#define DBG_DEM_GETNUMBEROFEVENTMEMORYENTRIES_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETNUMBEROFEVENTMEMORYENTRIES_EXIT
/** \brief Exit point of function Dem_GetNumberOfEventMemoryEntries() */
#define DBG_DEM_GETNUMBEROFEVENTMEMORYENTRIES_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETSI30STATUS_ENTRY
/** \brief Entry point of function Dem_GetSI30Status() */
#define DBG_DEM_GETSI30STATUS_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETSI30STATUS_EXIT
/** \brief Exit point of function Dem_GetSI30Status() */
#define DBG_DEM_GETSI30STATUS_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_SETSI30SYMPTOM_ENTRY
/** \brief Entry point of function Dem_GetSI30Status() */
#define DBG_DEM_SETSI30SYMPTOM_ENTRY(a)
#endif

#ifndef DBG_DEM_SETSI30SYMPTOM_EXIT
/** \brief Exit point of function Dem_SetSI30Symptom() */
#define DBG_DEM_SETSI30SYMPTOM_EXIT(a, b)
#endif

#ifndef DBG_DEM_SETSI30NOTIFICATION_ENTRY
/** \brief Entry point of function Dem_GetSI30Status() */
#define DBG_DEM_SETSI30NOTIFICATION_ENTRY(a)
#endif

#ifndef DBG_DEM_SETSI30NOTIFICATION_EXIT
/** \brief Exit point of function Dem_SetSI30Notification() */
#define DBG_DEM_SETSI30NOTIFICATION_EXIT(a, b)
#endif

#ifndef DBG_DEM_SETDTCSUPPRESSION_ENTRY
/** \brief Entry point of function Dem_SetDTCSuppression() */
#define DBG_DEM_SETDTCSUPPRESSION_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_SETDTCSUPPRESSION_EXIT
/** \brief Exit point of function Dem_SetDTCSuppression() */
#define DBG_DEM_SETDTCSUPPRESSION_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_INTERNALSETDTCSUPPRESSION_ENTRY
/** \brief Entry point of function Dem_InternalSetDTCSuppression() */
#define DBG_DEM_INTERNALSETDTCSUPPRESSION_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_INTERNALSETDTCSUPPRESSION_EXIT
/** \brief Exit point of function Dem_InternalSetDTCSuppression() */
#define DBG_DEM_INTERNALSETDTCSUPPRESSION_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_INTERNALCHECKANDSETDTCSUPPRESSION_ENTRY
/** \brief Entry point of function Dem_InternalCheckAndSetDTCSuppression() */
#define DBG_DEM_INTERNALCHECKANDSETDTCSUPPRESSION_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_INTERNALCHECKANDSETDTCSUPPRESSION_EXIT
/** \brief Exit point of function Dem_InternalCheckAndSetDTCSuppression() */
#define DBG_DEM_INTERNALCHECKANDSETDTCSUPPRESSION_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_INTERNALCHECKANDSETOBDDTCSUPPRESSION_ENTER
/** \brief Entry point of function Dem_InternalCheckAndSetObdDTCSuppression() */
#define DBG_DEM_INTERNALCHECKANDSETOBDDTCSUPPRESSION_ENTER(a, b)
#endif

#ifndef DBG_DEM_INTERNALCHECKANDSETOBDDTCSUPPRESSION_EXIT
/** \brief Exit point of function Dem_InternalCheckAndSetObdDTCSuppression() */
#define DBG_DEM_INTERNALCHECKANDSETOBDDTCSUPPRESSION_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_EXISTENTRYOFOBDDTC_ENTRY
/** \brief Entry point of function Dem_ExistEntryOfObdDTC() */
#define DBG_DEM_EXISTENTRYOFOBDDTC_ENTRY(a)
#endif

#ifndef DBG_DEM_EXISTENTRYOFOBDDTC_EXIT
/** \brief Exit point of function Dem_ExistEntryOfObdDTC() */
#define DBG_DEM_EXISTENTRYOFOBDDTC_EXIT(a, b)
#endif

#ifndef DBG_DEM_CHECKDELETEEVENT_ENTRY
/** \brief Entry point of function Dem_CheckDeleteEvent() */
#define DBG_DEM_CHECKDELETEEVENT_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_CHECKDELETEEVENT_EXIT
/** \brief Exit point of function Dem_CheckDeleteEvent() */
#define DBG_DEM_CHECKDELETEEVENT_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_J1939CHECKDELETEEVENT_ENTRY
/** \brief Entry point of function Dem_J1939CheckDeleteEvent() */
#define DBG_DEM_J1939CHECKDELETEEVENT_ENTRY(a)
#endif

#ifndef DBG_DEM_J1939CHECKDELETEEVENT_EXIT
/** \brief Exit point of function Dem_J1939CheckDeleteEvent() */
#define DBG_DEM_J1939CHECKDELETEEVENT_EXIT(a, b)
#endif

#ifndef DBG_DEM_ISDTCGROUPCLEAR_ENTRY
/** \brief Entry point of function Dem_IsDTCGroupClear() */
#define DBG_DEM_ISDTCGROUPCLEAR_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_ISDTCGROUPCLEAR_EXIT
/** \brief Exit point of function Dem_IsDTCGroupClear() */
#define DBG_DEM_ISDTCGROUPCLEAR_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_SETEVENTAVAILABLE_ENTRY
/** \brief Entry point of function Dem_SetEventAvailable() */
#define DBG_DEM_SETEVENTAVAILABLE_ENTRY(a, b)
#endif

#ifndef DBG_DEM_SETEVENTAVAILABLE_EXIT
/** \brief Exit point of function Dem_SetEventAvailable() */
#define DBG_DEM_SETEVENTAVAILABLE_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_MAINFUNCTION_ENTRY
/** \brief Entry point of function Dem_MainFunction() */
#define DBG_DEM_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_DEM_MAINFUNCTION_EXIT
/** \brief Exit point of function Dem_MainFunction() */
#define DBG_DEM_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_DEM_TRIGGERDEBOUNCEALGORITHMBEHAVIOR_ENTRY
/** \brief Entry point of function Dem_TriggerDebounceAlgorithmBehavior() */
#define DBG_DEM_TRIGGERDEBOUNCEALGORITHMBEHAVIOR_ENTRY(a)
#endif

#ifndef DBG_DEM_TRIGGERDEBOUNCEALGORITHMBEHAVIOR_EXIT
/** \brief Exit point of function Dem_TriggerDebounceAlgorithmBehavior() */
#define DBG_DEM_TRIGGERDEBOUNCEALGORITHMBEHAVIOR_EXIT(a)
#endif

#ifndef DBG_DEM_NVMGETNVRAMBLOCKSTATUS_ENTRY
/** \brief Entry point of function Dem_NvMGetNvRAMBlockStatus() */
#define DBG_DEM_NVMGETNVRAMBLOCKSTATUS_ENTRY(a)
#endif

#ifndef DBG_DEM_NVMGETNVRAMBLOCKSTATUS_EXIT
/** \brief Exit point of function Dem_NvMGetNvRAMBlockStatus() */
#define DBG_DEM_NVMGETNVRAMBLOCKSTATUS_EXIT(a, b)
#endif

#ifndef DBG_DEM_RESETNVDATA_ENTRY
/** \brief Entry point of function Dem_ResetNvData() */
#define DBG_DEM_RESETNVDATA_ENTRY()
#endif

#ifndef DBG_DEM_RESETNVDATA_EXIT
/** \brief Exit point of function Dem_ResetNvData() */
#define DBG_DEM_RESETNVDATA_EXIT()
#endif

#ifndef DBG_DEM_RESETSINGLEOBDFREEZEFRAMENVDATA_ENTRY
/** \brief Entry point of function Dem_ResetSingleOBDFreezeFrameNvData() */
#define DBG_DEM_RESETSINGLEOBDFREEZEFRAMENVDATA_ENTRY()
#endif

#ifndef DBG_DEM_RESETSINGLEOBDFREEZEFRAMENVDATA_EXIT
/** \brief Exit point of function Dem_ResetSingleOBDFreezeFrameNvData() */
#define DBG_DEM_RESETSINGLEOBDFREEZEFRAMENVDATA_EXIT()
#endif

#ifndef DBG_DEM_RESTOREIMMEDIATEDATA_ENTRY
/** \brief Entry point of function Dem_RestoreImmediateData() */
#define DBG_DEM_RESTOREIMMEDIATEDATA_ENTRY(a)
#endif

#ifndef DBG_DEM_RESTOREIMMEDIATEDATA_EXIT
/** \brief Exit point of function Dem_RestoreImmediateData() */
#define DBG_DEM_RESTOREIMMEDIATEDATA_EXIT(a)
#endif

#ifndef DBG_DEM_PERMANENTMEMORYINIT_ENTRY
/** \brief Entry point of function Dem_PermanentMemoryInit() */
#define DBG_DEM_PERMANENTMEMORYINIT_ENTRY(a)
#endif

#ifndef DBG_DEM_PERMANENTMEMORYINIT_EXIT
/** \brief Exit point of function Dem_PermanentMemoryInit() */
#define DBG_DEM_PERMANENTMEMORYINIT_EXIT(a)
#endif

#ifndef DBG_DEM_WRITEROOTERRORQUEUEENTRY_ENTRY
/** \brief Entry point of function Dem_WriteRootErrorQueueEntry() */
#define DBG_DEM_WRITEROOTERRORQUEUEENTRY_ENTRY(a)
#endif

#ifndef DBG_DEM_WRITEROOTERRORQUEUEENTRY_EXIT
/** \brief Exit point of function Dem_WriteRootErrorQueueEntry() */
#define DBG_DEM_WRITEROOTERRORQUEUEENTRY_EXIT(a)
#endif

#ifndef DBG_DEM_READROOTERRORQUEUEENTRY_ENTRY
/** \brief Entry point of function Dem_ReadRootErrorQueueEntry() */
#define DBG_DEM_READROOTERRORQUEUEENTRY_ENTRY(a)
#endif

#ifndef DBG_DEM_READROOTERRORQUEUEENTRY_EXIT
/** \brief Exit point of function Dem_ReadRootErrorQueueEntry() */
#define DBG_DEM_READROOTERRORQUEUEENTRY_EXIT(a, b)
#endif

#ifndef DBG_DEM_INVALIDATEERRORQUEUEENTRY_ENTRY
/** \brief Entry point of function Dem_InvalidateErrorQueueEntry() */
#define DBG_DEM_INVALIDATEERRORQUEUEENTRY_ENTRY(a)
#endif

#ifndef DBG_DEM_INVALIDATEERRORQUEUEENTRY_EXIT
/** \brief Exit point of function Dem_InvalidateErrorQueueEntry() */
#define DBG_DEM_INVALIDATEERRORQUEUEENTRY_EXIT(a)
#endif

#ifndef DBG_DEM_READERRORQUEUEENTRY_ENTRY
/** \brief Entry point of function Dem_ReadErrorQueueEntry() */
#define DBG_DEM_READERRORQUEUEENTRY_ENTRY(a)
#endif

#ifndef DBG_DEM_READERRORQUEUEENTRY_EXIT
/** \brief Exit point of function Dem_ReadErrorQueueEntry() */
#define DBG_DEM_READERRORQUEUEENTRY_EXIT(a, b)
#endif

#ifndef DBG_DEM_WRITEERRORQUEUEENTRY_ENTRY
/** \brief Entry point of function Dem_WriteErrorQueueEntry() */
#define DBG_DEM_WRITEERRORQUEUEENTRY_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_WRITEERRORQUEUEENTRY_EXIT
/** \brief Exit point of function Dem_WriteErrorQueueEntry() */
#define DBG_DEM_WRITEERRORQUEUEENTRY_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_CHECKERRORQUEUEENTRY_ENTRY
/** \brief Entry point of function Dem_CheckErrorQueueEntry() */
#define DBG_DEM_CHECKERRORQUEUEENTRY_ENTRY(a, b)
#endif

#ifndef DBG_DEM_CHECKERRORQUEUEENTRY_EXIT
/** \brief Exit point of function Dem_CheckErrorQueueEntry() */
#define DBG_DEM_CHECKERRORQUEUEENTRY_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_PROCESSEVENTSTATUSUPDATE_ENTRY
/** \brief Entry point of function Dem_ProcessEventStatusUpdate() */
#define DBG_DEM_PROCESSEVENTSTATUSUPDATE_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_PROCESSEVENTSTATUSUPDATE_EXIT
/** \brief Exit point of function Dem_ProcessEventStatusUpdate() */
#define DBG_DEM_PROCESSEVENTSTATUSUPDATE_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_CHECKERRORREQUIRESINDUPDATE_ENTRY
/** \brief Entry point of function Dem_CheckErrorRequiresIndUpdate() */
#define DBG_DEM_CHECKERRORREQUIRESINDUPDATE_ENTRY(a, b)
#endif

#ifndef DBG_DEM_CHECKERRORREQUIRESINDUPDATE_EXIT
/** \brief Exit point of function Dem_CheckErrorRequiresIndUpdate() */
#define DBG_DEM_CHECKERRORREQUIRESINDUPDATE_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_CHECKERRORQUEUEENTRYFORSTATUS_ENTRY
/** \brief Entry point of function Dem_CheckErrorQueueEntryForStatus() */
#define DBG_DEM_CHECKERRORQUEUEENTRYFORSTATUS_ENTRY(a, b)
#endif

#ifndef DBG_DEM_CHECKERRORQUEUEENTRYFORSTATUS_EXIT
/** \brief Exit point of function Dem_CheckErrorQueueEntryForStatus() */
#define DBG_DEM_CHECKERRORQUEUEENTRYFORSTATUS_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETPFFENTRYDATAPTR_ENTRY
/** \brief Entry point of function Dem_GetPFFEntryDataPtr() */
#define DBG_DEM_GETPFFENTRYDATAPTR_ENTRY(a)
#endif

#ifndef DBG_DEM_GETPFFENTRYDATAPTR_EXIT
/** \brief Exit point of function Dem_GetPFFEntryDataPtr() */
#define DBG_DEM_GETPFFENTRYDATAPTR_EXIT(a, b)
#endif

#ifndef DBG_DEM_UPDATEAGINGCOUNTER_ENTRY
/** \brief Entry point of function Dem_UpdateAgingCounter() */
#define DBG_DEM_UPDATEAGINGCOUNTER_ENTRY(a)
#endif

#ifndef DBG_DEM_UPDATEAGINGCOUNTER_EXIT
/** \brief Exit point of function Dem_UpdateAgingCounter() */
#define DBG_DEM_UPDATEAGINGCOUNTER_EXIT(a, b)
#endif

#ifndef DBG_DEM_ISEVENTSMILINDICATORACTIVE_ENTRY
/** \brief Entry point of function Dem_IsEventsMILIndicatorActive() */
#define DBG_DEM_ISEVENTSMILINDICATORACTIVE_ENTRY(a)
#endif

#ifndef DBG_DEM_ISEVENTSMILINDICATORACTIVE_EXIT
/** \brief Exit point of function Dem_IsEventsMILIndicatorActive() */
#define DBG_DEM_ISEVENTSMILINDICATORACTIVE_EXIT(a, b)
#endif

#ifndef DBG_DEM_ISCMBEVENTSMILINDICATORACTIVE_ENTRY
/** \brief Entry point of function Dem_IsCmbEventsMILIndicatorActive() */
#define DBG_DEM_ISCMBEVENTSMILINDICATORACTIVE_ENTRY(a)
#endif

#ifndef DBG_DEM_ISCMBEVENTSMILINDICATORACTIVE_EXIT
/** \brief Exit point of function Dem_IsCmbEventsMILIndicatorActive() */
#define DBG_DEM_ISCMBEVENTSMILINDICATORACTIVE_EXIT(a, b)
#endif

#ifndef DBG_DEM_INTERNALGETUDSDTC_ENTRY
/** \brief Entry point of function Dem_InternalGetUdsDTC() */
#define DBG_DEM_INTERNALGETUDSDTC_ENTRY(a)
#endif

#ifndef DBG_DEM_INTERNALGETUDSDTC_EXIT
/** \brief Exit point of function Dem_InternalGetUdsDTC() */
#define DBG_DEM_INTERNALGETUDSDTC_EXIT(a, b)
#endif

#ifndef DBG_DEM_INTERNALGETOBDDTC_ENTRY
/** \brief Entry point of function Dem_InternalGetObdDTC() */
#define DBG_DEM_INTERNALGETOBDDTC_ENTRY(a)
#endif

#ifndef DBG_DEM_INTERNALGETOBDDTC_EXIT
/** \brief Exit point of function Dem_InternalGetObdDTC() */
#define DBG_DEM_INTERNALGETOBDDTC_EXIT(a, b)
#endif

#ifndef DBG_DEM_CLEARAGEDEVENTENTRY_ENTRY
/** \brief Entry point of function Dem_ClearAgedEventEntry() */
#define DBG_DEM_CLEARAGEDEVENTENTRY_ENTRY(a, b)
#endif

#ifndef DBG_DEM_CLEARAGEDEVENTENTRY_EXIT
/** \brief Exit point of function Dem_ClearAgedEventEntry() */
#define DBG_DEM_CLEARAGEDEVENTENTRY_EXIT(a, b)
#endif

#ifndef DBG_DEM_PROCESSEVENT_ENTRY
/** \brief Entry point of function Dem_ProcessEvent() */
#define DBG_DEM_PROCESSEVENT_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DEM_PROCESSEVENT_EXIT
/** \brief Exit point of function Dem_ProcessEvent() */
#define DBG_DEM_PROCESSEVENT_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_PROCESSEVENTENTRY_ENTRY
/** \brief Entry point of function Dem_ProcessEventEntry() */
#define DBG_DEM_PROCESSEVENTENTRY_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_PROCESSEVENTENTRY_EXIT
/** \brief Exit point of function Dem_ProcessEventEntry() */
#define DBG_DEM_PROCESSEVENTENTRY_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_PROCESSOBDFREEZEFRAME_ENTRY
/** \brief Entry point of function Dem_ProcessOBDFreezeFrame() */
#define DBG_DEM_PROCESSOBDFREEZEFRAME_ENTRY(a)
#endif

#ifndef DBG_DEM_PROCESSOBDFREEZEFRAME_EXIT
/** \brief Exit point of function Dem_ProcessOBDFreezeFrame() */
#define DBG_DEM_PROCESSOBDFREEZEFRAME_EXIT(a)
#endif

#ifndef DBG_DEM_PROCESSSINGLEOBDFREEZEFRAME_ENTRY
/** \brief Entry point of function Dem_ProcessSingleOBDFreezeFrame() */
#define DBG_DEM_PROCESSSINGLEOBDFREEZEFRAME_ENTRY(a)
#endif

#ifndef DBG_DEM_PROCESSSINGLEOBDFREEZEFRAME_EXIT
/** \brief Exit point of function Dem_ProcessSingleOBDFreezeFrame() */
#define DBG_DEM_PROCESSSINGLEOBDFREEZEFRAME_EXIT(a)
#endif

#ifndef DBG_DEM_PROCESSMULTIPLEOBDFREEZEFRAMEENTRY_ENTRY
/** \brief Entry point of function Dem_ProcessMultipleOBDFreezeFrameEntry() */
#define DBG_DEM_PROCESSMULTIPLEOBDFREEZEFRAMEENTRY_ENTRY(a, b)
#endif

#ifndef DBG_DEM_PROCESSMULTIPLEOBDFREEZEFRAMEENTRY_EXIT
/** \brief Exit point of function Dem_ProcessMultipleOBDFreezeFrameEntry() */
#define DBG_DEM_PROCESSMULTIPLEOBDFREEZEFRAMEENTRY_EXIT(a, b)
#endif

#ifndef DBG_DEM_SELECTENTRYOFOBDFREEZEFRAMEEVENT_ENTRY
/** \brief Entry point of function Dem_SelectEntryOfOBDFreezeFrameEvent() */
#define DBG_DEM_SELECTENTRYOFOBDFREEZEFRAMEEVENT_ENTRY(a, b)
#endif

#ifndef DBG_DEM_SELECTENTRYOFOBDFREEZEFRAMEEVENT_EXIT
/** \brief Exit point of function Dem_SelectEntryOfOBDFreezeFrameEvent() */
#define DBG_DEM_SELECTENTRYOFOBDFREEZEFRAMEEVENT_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_INITMAXOBDOCCORDER_ENTRY
/** \brief Entry point of function Dem_InitMaxOBDOccOrder() */
#define DBG_DEM_INITMAXOBDOCCORDER_ENTRY()
#endif

#ifndef DBG_DEM_INITMAXOBDOCCORDER_EXIT
/** \brief Exit point of function Dem_InitMaxOBDOccOrder() */
#define DBG_DEM_INITMAXOBDOCCORDER_EXIT()
#endif

#ifndef DBG_DEM_SETOBDFFOCCURRENCEORDER_ENTRY
/** \brief Entry point of function Dem_SetOBDFFOccurrenceOrder() */
#define DBG_DEM_SETOBDFFOCCURRENCEORDER_ENTRY(a)
#endif

#ifndef DBG_DEM_SETOBDFFOCCURRENCEORDER_EXIT
/** \brief Exit point of function Dem_SetOBDFFOccurrenceOrder() */
#define DBG_DEM_SETOBDFFOCCURRENCEORDER_EXIT(a, b)
#endif

#ifndef DBG_DEM_REARRANGEOBDOCCURRENCEORDER_ENTRY
/** \brief Entry point of function Dem_RearrangeOBDOccurrenceOrder() */
#define DBG_DEM_REARRANGEOBDOCCURRENCEORDER_ENTRY()
#endif

#ifndef DBG_DEM_REARRANGEOBDOCCURRENCEORDER_EXIT
/** \brief Exit point of function Dem_RearrangeOBDOccurrenceOrder() */
#define DBG_DEM_REARRANGEOBDOCCURRENCEORDER_EXIT()
#endif

#ifndef DBG_DEM_PROCESSEVENTNOTIFICATIONS_ENTRY
/** \brief Entry point of function Dem_ProcessEventEntryNotifications() */
#define DBG_DEM_PROCESSEVENTNOTIFICATIONS_ENTRY(a)
#endif

#ifndef DBG_DEM_PROCESSEVENTNOTIFICATIONS_EXIT
/** \brief Exit point of function Dem_ProcessEventEntryNotifications() */
#define DBG_DEM_PROCESSEVENTNOTIFICATIONS_EXIT(a)
#endif

#ifndef DBG_DEM_INCREMENTALLOCC5_ENTRY
/** \brief Entry point of function Dem_IncrementAllOcc5() */
#define DBG_DEM_INCREMENTALLOCC5_ENTRY()
#endif

#ifndef DBG_DEM_INCREMENTALLOCC5_EXIT
/** \brief Exit point of function Dem_IncrementAllOcc5() */
#define DBG_DEM_INCREMENTALLOCC5_EXIT()
#endif

#ifndef DBG_DEM_CHECKDEVFREEZEFRAMECONSISTENCY_ENTRY
/** \brief Entry point of function Dem_CheckDevFreezeFrameConsistency() */
#define DBG_DEM_CHECKDEVFREEZEFRAMECONSISTENCY_ENTRY()
#endif

#ifndef DBG_DEM_CHECKDEVFREEZEFRAMECONSISTENCY_EXIT
/** \brief Exit point of function Dem_CheckDevFreezeFrameConsistency() */
#define DBG_DEM_CHECKDEVFREEZEFRAMECONSISTENCY_EXIT()
#endif

#ifndef DBG_DEM_ISEVENTADVANCEDPASSIVE_ENTRY
/** \brief Entry point of function Dem_IsEventAdvancedPassive() */
#define DBG_DEM_ISEVENTADVANCEDPASSIVE_ENTRY(a, b)
#endif

#ifndef DBG_DEM_ISEVENTADVANCEDPASSIVE_EXIT
/** \brief Exit point of function Dem_Dem_IsEventAdvancedPassive() */
#define DBG_DEM_ISEVENTADVANCEDPASSIVE_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_ISDISPLACEOBDEVENTENTRYALLOWED_ENTRY
/** \brief Entry point of function Dem_IsDisplaceOBDEventEntryAllowed() */
#define DBG_DEM_ISDISPLACEOBDEVENTENTRYALLOWED_ENTRY(a)
#endif

#ifndef DBG_DEM_ISDISPLACEOBDEVENTENTRYALLOWED_EXIT
/** \brief Exit point of function Dem_IsDisplaceOBDEventEntryAllowed() */
#define DBG_DEM_ISDISPLACEOBDEVENTENTRYALLOWED_EXIT(a, b)
#endif

#ifndef DBG_DEM_FINDORALLOCATEEVENTENTRY_ENTRY
/** \brief Entry point of function Dem_FindOrAllocateEventEntry() */
#define DBG_DEM_FINDORALLOCATEEVENTENTRY_ENTRY(a, b, c, d ,e, f)
#endif

#ifndef DBG_DEM_FINDORALLOCATEEVENTENTRY_EXIT
/** \brief Exit point of function Dem_FindOrAllocateEventEntry() */
#define DBG_DEM_FINDORALLOCATEEVENTENTRY_EXIT(a, b, c, d, e, f, g)
#endif

#ifndef DBG_DEM_REPLACEEVENTENTRY_ENTRY
/** \brief Entry point of function Dem_ReplaceEventEntry() */
#define DBG_DEM_REPLACEEVENTENTRY_ENTRY(a, b, c, d, e, f)
#endif

#ifndef DBG_DEM_REPLACEEVENTENTRY_EXIT
/** \brief Exit point of function Dem_ReplaceEventEntry() */
#define DBG_DEM_REPLACEEVENTENTRY_EXIT(a, b, c, d, e, f, g)
#endif

#ifndef DBG_DEM_PROCESSEVENTENTRYFFDATA_ENTRY
/** \brief Entry point of function Dem_ProcessEventEntryFFData() */
#define DBG_DEM_PROCESSEVENTENTRYFFDATA_ENTRY(a, b)
#endif

#ifndef DBG_DEM_PROCESSEVENTENTRYFFDATA_EXIT
/** \brief Exit point of function Dem_ProcessEventEntryFFData() */
#define DBG_DEM_PROCESSEVENTENTRYFFDATA_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_PROCESSEVENTENTRYEDATA_ENTRY
/** \brief Entry point of function Dem_ProcessEventEntryEData() */
#define DBG_DEM_PROCESSEVENTENTRYEDATA_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_PROCESSEVENTENTRYEDATA_EXIT
/** \brief Exit point of function Dem_ProcessEventEntryEData() */
#define DBG_DEM_PROCESSEVENTENTRYEDATA_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_STOREFF_ENTRY
/** \brief Entry point of function Dem_StoreFF() */
#define DBG_DEM_STOREFF_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DEM_STOREFF_EXIT
/** \brief Exit point of function Dem_StoreFF() */
#define DBG_DEM_STOREFF_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_STOREDEVFFS_ENTRY
/** \brief Entry point of function Dem_StoreDevFFs() */
#define DBG_DEM_STOREDEVFFS_ENTRY(a, b)
#endif

#ifndef DBG_DEM_STOREDEVFFS_EXIT
/** \brief Exit point of function Dem_StoreDevFFs() */
#define DBG_DEM_STOREDEVFFS_EXIT(a, b)
#endif

#ifndef DBG_DEM_STOREOBDFREEZEFRAME_ENTRY
/** \brief Entry point of function Dem_StoreOBDFreezeFrame() */
#define DBG_DEM_STOREOBDFREEZEFRAME_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_STOREOBDFREEZEFRAME_EXIT
/** \brief Exit point of function Dem_StoreOBDFreezeFrame() */
#define DBG_DEM_STOREOBDFREEZEFRAME_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_PROCESSDISTANCERELATEDPIDS_ENTER
/** \brief Entry point of function Dem_ProcessDistanceRelatedPIDs() */
#define DBG_DEM_PROCESSDISTANCERELATEDPIDS_ENTER()
#endif

#ifndef DBG_DEM_PROCESSDISTANCERELATEDPIDS_EXIT
/** \brief Exit point of function Dem_ProcessDistanceRelatedPIDs() */
#define DBG_DEM_PROCESSDISTANCERELATEDPIDS_EXIT()
#endif

#ifndef DBG_DEM_INTERNALGETINDICATORSTATUS_ENTER
/** \brief Entry point of function Dem_InternalGetIndicatorStatus() */
#define DBG_DEM_INTERNALGETINDICATORSTATUS_ENTER(a)
#endif

#ifndef DBG_DEM_INTERNALGETINDICATORSTATUS_EXIT
/** \brief Exit point of function Dem_InternalGetIndicatorStatus() */
#define DBG_DEM_INTERNALGETINDICATORSTATUS_EXIT(a,b)
#endif

#ifndef DBG_DEM_STOREED_ENTRY
/** \brief Entry point of function Dem_StoreED() */
#define DBG_DEM_STOREED_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_STOREED_EXIT
/** \brief Exit point of function Dem_StoreED() */
#define DBG_DEM_STOREED_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_SEARCHFOREMPTYENTRY_ENTRY
/** \brief Entry point of function Dem_SearchForEmptyEntry() */
#define DBG_DEM_SEARCHFOREMPTYENTRY_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_SEARCHFOREMPTYENTRY_EXIT
/** \brief Exit point of function Dem_SearchForEmptyEntry() */
#define DBG_DEM_SEARCHFOREMPTYENTRY_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_SEARCHFORENTRYTOREPLACE_ENTRY
/** \brief Entry point of function Dem_SearchForEntryToReplace() */
#define DBG_DEM_SEARCHFORENTRYTOREPLACE_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DEM_SEARCHFORENTRYTOREPLACE_EXIT
/** \brief Exit point of function Dem_SearchForEntryToReplace() */
#define DBG_DEM_SEARCHFORENTRYTOREPLACE_EXIT(a, b, c, d, e, f)
#endif

#ifndef DBG_DEM_UPDATEOCCURRENCEORDER_ENTRY
/** \brief Entry point of function Dem_UpdateOccurrenceOrder() */
#define DBG_DEM_UPDATEOCCURRENCEORDER_ENTRY(a)
#endif

#ifndef DBG_DEM_UPDATEOCCURRENCEORDER_EXIT
/** \brief Exit point of function Dem_UpdateOccurrenceOrder() */
#define DBG_DEM_UPDATEOCCURRENCEORDER_EXIT(a, b)
#endif

#ifndef DBG_DEM_DEBOUNCECOUNTERWITHNVSTORAGEDURINGPREINIT_ENTRY
/** \brief Entry point of function Dem_DebounceCounterWithNvStorageDuringPreInit() */
#define DBG_DEM_DEBOUNCECOUNTERWITHNVSTORAGEDURINGPREINIT_ENTRY(a)
#endif

#ifndef DBG_DEM_DEBOUNCECOUNTERWITHNVSTORAGEDURINGPREINIT_EXIT
/** \brief Exit point of function Dem_DebounceCounterWithNvStorageDuringPreInit() */
#define DBG_DEM_DEBOUNCECOUNTERWITHNVSTORAGEDURINGPREINIT_EXIT(a, b)
#endif

#ifndef DBG_DEM_REARRANGEOCCURRENCEORDER_ENTRY
/** \brief Entry point of function Dem_RearrangeOccurrenceOrder() */
#define DBG_DEM_REARRANGEOCCURRENCEORDER_ENTRY(a)
#endif

#ifndef DBG_DEM_REARRANGEOCCURRENCEORDER_EXIT
/** \brief Exit point of function Dem_RearrangeOccurrenceOrder() */
#define DBG_DEM_REARRANGEOCCURRENCEORDER_EXIT(a)
#endif

#ifndef DBG_DEM_INITMAXOCCORDER_ENTRY
/** \brief Entry point of function Dem_InitMaxOccOrder() */
#define DBG_DEM_INITMAXOCCORDER_ENTRY()
#endif

#ifndef DBG_DEM_INITMAXOCCORDER_EXIT
/** \brief Exit point of function Dem_InitMaxOccOrder() */
#define DBG_DEM_INITMAXOCCORDER_EXIT()
#endif

#ifndef DBG_DEM_SETCDTCONCONFIRMATION_ENTRY
/** \brief Entry point of function Dem_SetCDTCOnConfirmation() */
#define DBG_DEM_SETCDTCONCONFIRMATION_ENTRY()
#endif

#ifndef DBG_DEM_SETCDTCONCONFIRMATION_EXIT
/** \brief Exit point of function Dem_SetCDTCOnConfirmation() */
#define DBG_DEM_SETCDTCONCONFIRMATION_EXIT()
#endif

#ifndef DBG_DEM_PROCESSFAILURECOUNTERRESET_ENTRY
/** \brief Entry point of function Dem_ProcessFailureCounterReset() */
#define DBG_DEM_PROCESSFAILURECOUNTERRESET_ENTRY(a, b)
#endif

#ifndef DBG_DEM_PROCESSFAILURECOUNTERRESET_EXIT
/** \brief Exit point of function Dem_ProcessFailureCounterReset() */
#define DBG_DEM_PROCESSFAILURECOUNTERRESET_EXIT(a, b)
#endif

#ifndef DBG_DEM_UPDATEFAILURECOUNTERCYCLESTART_ENTRY
/** \brief Entry point of function Dem_UpdateFailureCounterCycleStart() */
#define DBG_DEM_UPDATEFAILURECOUNTERCYCLESTART_ENTRY(a, b)
#endif

#ifndef DBG_DEM_UPDATEFAILURECOUNTERCYCLESTART_EXIT
/** \brief Exit point of function Dem_UpdateFailureCounterCycleStart() */
#define DBG_DEM_UPDATEFAILURECOUNTERCYCLESTART_EXIT(a, b)
#endif

#ifndef DBG_DEM_INCREMENTFAILURECOUNTER_ENTRY
/** \brief Entry point of function Dem_IncrementFailureCounter() */
#define DBG_DEM_INCREMENTFAILURECOUNTER_ENTRY(a, b)
#endif

#ifndef DBG_DEM_INCREMENTFAILURECOUNTER_EXIT
/** \brief Exit point of function Dem_IncrementFailureCounter() */
#define DBG_DEM_INCREMENTFAILURECOUNTER_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_CHECKINTDATAELEMENTUSAGE_ENTRY
/** \brief Entry point of function Dem_CheckIntDataElementUsage() */
#define DBG_DEM_CHECKINTDATAELEMENTUSAGE_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_CHECKINTDATAELEMENTUSAGE_EXIT
/** \brief Exit point of function Dem_CheckIntDataElementUsage() */
#define DBG_DEM_CHECKINTDATAELEMENTUSAGE_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_CBTRIGGERONOVFINDSET_ENTRY
/** \brief Entry point of function Dem_CbTriggerOnOvfIndSet() */
#define DBG_DEM_CBTRIGGERONOVFINDSET_ENTRY(a)
#endif

#ifndef DBG_DEM_CBTRIGGERONOVFINDSET_EXIT
/** \brief Exit point of function Dem_CbTriggerOnOvfIndSet() */
#define DBG_DEM_CBTRIGGERONOVFINDSET_EXIT(a)
#endif

#ifndef DBG_DEM_PROCESSINDICATOR_ENTRY
/** \brief Entry point of function Dem_ProcessIndicator() */
#define DBG_DEM_PROCESSINDICATOR_ENTRY(a)
#endif

#ifndef DBG_DEM_PROCESSINDICATOR_EXIT
/** \brief Exit point of function Dem_ProcessIndicator() */
#define DBG_DEM_PROCESSINDICATOR_EXIT(a)
#endif

#ifndef DBG_DEM_PROCESSINDICATOROFFCRITERIA_ENTRY
/** \brief Entry point of function Dem_ProcessIndicatorOffCriteria() */
#define DBG_DEM_PROCESSINDICATOROFFCRITERIA_ENTRY(a, b)
#endif

#ifndef DBG_DEM_PROCESSINDICATOROFFCRITERIA_EXIT
/** \brief Exit point of function Dem_ProcessIndicatorOffCriteria() */
#define DBG_DEM_PROCESSINDICATOROFFCRITERIA_EXIT(a, b)
#endif

#ifndef DBG_DEM_PROCESSINDICATORONCRITERIA_ENTRY
/** \brief Entry point of function Dem_ProcessIndicatorOnCriteria() */
#define DBG_DEM_PROCESSINDICATORONCRITERIA_ENTRY(a, b)
#endif

#ifndef DBG_DEM_PROCESSINDICATORONCRITERIA_EXIT
/** \brief Exit point of function Dem_ProcessIndicatorOnCriteria() */
#define DBG_DEM_PROCESSINDICATORONCRITERIA_EXIT(a, b)
#endif

#ifndef DBG_DEM_PROCESSINDICATORFAIURECYCLECOUNTER_ENTRY
/** \brief Entry point of function Dem_ProcessIndicatorFailureCycleCounter() */
#define DBG_DEM_PROCESSINDICATORFAIURECYCLECOUNTER_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_PROCESSINDICATORFAIURECYCLECOUNTER_EXIT
/** \brief Exit point of function Dem_ProcessIndicatorFailureCycleCounter() */
#define DBG_DEM_PROCESSINDICATORFAIURECYCLECOUNTER_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_CHECKWIROFFCRITERIAFULFILLED_ENTRY
/** \brief Entry point of function Dem_CheckWIROffCriteriaFulfilled() */
#define DBG_DEM_CHECKWIROFFCRITERIAFULFILLED_ENTRY(a)
#endif

#ifndef DBG_DEM_CHECKWIROFFCRITERIAFULFILLED_EXIT
/** \brief Exit point of function Dem_CheckWIROffCriteriaFulfilled() */
#define DBG_DEM_CHECKWIROFFCRITERIAFULFILLED_EXIT(a, b)
#endif

#ifndef DBG_DEM_ISMILEVENT_ENTRY
/** \brief Entry point of function Dem_IsMILEvent() */
#define DBG_DEM_ISMILEVENT_ENTRY(a, b)
#endif

#ifndef DBG_DEM_ISMILEVENT_EXIT
/** \brief Exit point of function Dem_IsMILEvent() */
#define DBG_DEM_ISMILEVENT_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_SETCYCLECOUNTERROLE_ENTRY
/** \brief Entry point of function Dem_SetCycleCounterRole() */
#define DBG_DEM_SETCYCLECOUNTERROLE_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_SETCYCLECOUNTERROLE_EXIT
/** \brief Exit point of function Dem_SetCycleCounterRole() */
#define DBG_DEM_SETCYCLECOUNTERROLE_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_PERMANENTMEMORYNOTIFYCLEAR_ENTRY
/** \brief Entry point of function Dem_PermanentMemoryNotifyClear() */
#define DBG_DEM_PERMANENTMEMORYNOTIFYCLEAR_ENTRY(a)
#endif

#ifndef DBG_DEM_PERMANENTMEMORYNOTIFYCLEAR_EXIT
/** \brief Exit point of function Dem_PermanentMemoryNotifyClear() */
#define DBG_DEM_PERMANENTMEMORYNOTIFYCLEAR_EXIT(a)
#endif

#ifndef DBG_DEM_PERMANENTMEMORYADDENTRY_ENTRY
/** \brief Entry point of function Dem_PermanentMemoryAddEntry() */
#define DBG_DEM_PERMANENTMEMORYADDENTRY_ENTRY(a)
#endif

#ifndef DBG_DEM_PERMANENTMEMORYADDENTRY_EXIT
/** \brief Exit point of function Dem_PermanentMemoryAddEntry() */
#define DBG_DEM_PERMANENTMEMORYADDENTRY_EXIT(a, b)
#endif

#ifndef DBG_DEM_PERMANENTMEMORYREMOVEENTRY_ENTRY
/** \brief Entry point of function Dem_PermanentMemoryRemoveEntry() */
#define DBG_DEM_PERMANENTMEMORYREMOVEENTRY_ENTRY(a)
#endif

#ifndef DBG_DEM_PERMANENTMEMORYREMOVEENTRY_EXIT
/** \brief Exit point of function Dem_PermanentMemoryRemoveEntry() */
#define DBG_DEM_PERMANENTMEMORYREMOVEENTRY_EXIT(a, b)
#endif

#ifndef DBG_DEM_PERMANENTMEMORYSTATUSUPDATE_ENTRY
/** \brief Entry point of function Dem_PermanentMemoryStatusUpdate() */
#define DBG_DEM_PERMANENTMEMORYSTATUSUPDATE_ENTRY()
#endif

#ifndef DBG_DEM_PERMANENTMEMORYSTATUSUPDATE_EXIT
/** \brief Exit point of function Dem_PermanentMemoryStatusUpdate() */
#define DBG_DEM_PERMANENTMEMORYSTATUSUPDATE_EXIT()
#endif

#ifndef DBG_DEM_DEBOUNCEFREQUENCYTIMERTICK_ENTRY
/** \brief Entry point of function Dem_DebounceFrequencyTimerTick() */
#define DBG_DEM_DEBOUNCEFREQUENCYTIMERTICK_ENTRY()
#endif

#ifndef DBG_DEM_DEBOUNCEFREQUENCYTIMERTICK_EXIT
/** \brief Exit point of function Dem_DebounceFrequencyTimerTick() */
#define DBG_DEM_DEBOUNCEFREQUENCYTIMERTICK_EXIT()
#endif

#ifndef DBG_DEM_DEBOUNCETIMETIMERTICK_ENTRY
/** \brief Entry point of function Dem_DebounceTimeTimerTick() */
#define DBG_DEM_DEBOUNCETIMETIMERTICK_ENTRY()
#endif

#ifndef DBG_DEM_DEBOUNCETIMETIMERTICK_EXIT
/** \brief Exit point of function Dem_DebounceTimeTimerTick() */
#define DBG_DEM_DEBOUNCETIMETIMERTICK_EXIT()
#endif

#ifndef DBG_DEM_GETVISIBLEUDSDTC_ENTRY
/** \brief Entry point of function Dem_GetVisibleUdsDTC() */
#define DBG_DEM_GETVISIBLEUDSDTC_ENTRY(a)
#endif

#ifndef DBG_DEM_GETVISIBLEUDSDTC_EXIT
/** \brief Exit point of function Dem_GetVisibleUdsDTC() */
#define DBG_DEM_GETVISIBLEUDSDTC_EXIT(a, b)
#endif

#ifndef DBG_DEM_GETVISIBLECMBDTC_ENTRY
/** \brief Entry point of function Dem_GetVisibleCmbDTC() */
#define DBG_DEM_GETVISIBLECMBDTC_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETVISIBLECMBDTC_EXIT
/** \brief Exit point of function Dem_GetVisibleCmbDTC() */
#define DBG_DEM_GETVISIBLECMBDTC_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETVISIBLEDTC_ENTRY
/** \brief Entry point of function Dem_GetVisibleDTC() */
#define DBG_DEM_GETVISIBLEDTC_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETVISIBLEDTC_EXIT
/** \brief Exit point of function Dem_GetVisibleDTC() */
#define DBG_DEM_GETVISIBLEDTC_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETVISIBLEOBDDTC_ENTRY
/** \brief Entry point of function Dem_GetVisibleObdDTC() */
#define DBG_DEM_GETVISIBLEOBDDTC_ENTRY(a)
#endif

#ifndef DBG_DEM_GETVISIBLEOBDDTC_EXIT
/** \brief Exit point of function Dem_GetVisibleObdDTC() */
#define DBG_DEM_GETVISIBLEOBDDTC_EXIT(a, b)
#endif

#ifndef DBG_DEM_GETUDSDTC_ENTRY
/** \brief Entry point of function Dem_GetUdsDTC() */
#define DBG_DEM_GETUDSDTC_ENTRY(a)
#endif

#ifndef DBG_DEM_GETUDSDTC_EXIT
/** \brief Exit point of function Dem_GetUdsDTC() */
#define DBG_DEM_GETUDSDTC_EXIT(a, b)
#endif

#ifndef DBG_DEM_GETOBDDTC_ENTRY
/** \brief Entry point of function Dem_GetObdDTC() */
#define DBG_DEM_GETOBDDTC_ENTRY(a)
#endif

#ifndef DBG_DEM_GETOBDDTC_EXIT
/** \brief Exit point of function Dem_GetObdDTC() */
#define DBG_DEM_GETOBDDTC_EXIT(a, b)
#endif

#ifndef DBG_DEM_GETJ1939DTC_ENTRY
/** \brief Entry point of function Dem_GetJ1939DTC() */
#define DBG_DEM_GETJ1939DTC_ENTRY(a)
#endif

#ifndef DBG_DEM_GETJ1939DTC_EXIT
/** \brief Exit point of function Dem_GetJ1939DTC() */
#define DBG_DEM_GETJ1939DTC_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIMINRATIOEVENT_ENTRY
/** \brief Entry point of function Dem_GbiMinRatioEvent() */
#define DBG_DEM_GBIMINRATIOEVENT_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIMINRATIOEVENT_EXIT
/** \brief Exit point of function Dem_GbiMinRatioEvent() */
#define DBG_DEM_GBIMINRATIOEVENT_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIFREEZEFRAMEUPDTATE_ENTRY
/** \brief Entry point of function Dem_GbiFreezeFrameUpdate() */
#define DBG_DEM_GBIFREEZEFRAMEUPDTATE_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIFREEZEFRAMEUPDTATE_EXIT
/** \brief Exit point of function Dem_GbiFreezeFrameUpdate() */
#define DBG_DEM_GBIFREEZEFRAMEUPDTATE_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIOBDREADINESSGROUP_ENTRY
/** \brief Entry point of function Dem_GbiOBDReadinessGroup() */
#define DBG_DEM_GBIOBDREADINESSGROUP_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIOBDREADINESSGROUP_EXIT
/** \brief Entry point of function Dem_GbiOBDReadinessGroup() */
#define DBG_DEM_GBIOBDREADINESSGROUP_EXIT(a,b)
#endif

#ifndef DBG_DEM_GBIDTCKIND_ENTRY
/** \brief Entry point of function Dem_GbiDTCKind() */
#define DBG_DEM_GBIDTCKIND_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIDTCKIND_EXIT
/** \brief Exit point of function Dem_GbiDTCKind() */
#define DBG_DEM_GBIDTCKIND_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIDTCORIGIN_ENTRY
/** \brief Entry point of function Dem_GbiDTCOrigin() */
#define DBG_DEM_GBIDTCORIGIN_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIDTCORIGIN_EXIT
/** \brief Exit point of function Dem_GbiDTCOrigin() */
#define DBG_DEM_GBIDTCORIGIN_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIDTCORIGINIDX_ENTRY
/** \brief Entry point of function Dem_GbiDTCOrigin() */
#define DBG_DEM_GBIDTCORIGINIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIDTCORIGINIDX_EXIT
/** \brief Exit point of function Dem_GbiDTCOrigin() */
#define DBG_DEM_GBIDTCORIGINIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIDTCGROUPIDX_ENTRY
/** \brief Entry point of function Dem_GbiDTCGroupIdx() */
#define DBG_DEM_GBIDTCGROUPIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIDTCGROUPIDX_EXIT
/** \brief Exit point of function Dem_GbiDTCGroupIdx() */
#define DBG_DEM_GBIDTCGROUPIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIIMMEDIATESTORAGE_ENTRY
/** \brief Entry point of function Dem_GbiImmediateStorage() */
#define DBG_DEM_GBIIMMEDIATESTORAGE_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIIMMEDIATESTORAGE_EXIT
/** \brief Exit point of function Dem_GbiImmediateStorage() */
#define DBG_DEM_GBIIMMEDIATESTORAGE_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIDTCSEVERITY_ENTRY
/** \brief Entry point of function Dem_GbiDTCSeverity() */
#define DBG_DEM_GBIDTCSEVERITY_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIDTCSEVERITY_EXIT
/** \brief Exit point of function Dem_GbiDTCSeverity() */
#define DBG_DEM_GBIDTCSEVERITY_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIDTCFUNCTIONALUNIT_ENTRY
/** \brief Entry point of function Dem_GbiDTCFunctionalUnit() */
#define DBG_DEM_GBIDTCFUNCTIONALUNIT_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIDTCFUNCTIONALUNIT_EXIT
/** \brief Exit point of function Dem_GbiDTCFunctionalUnit() */
#define DBG_DEM_GBIDTCFUNCTIONALUNIT_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIENABLEPRESTORAGE_ENTRY
/** \brief Entry point of function Dem_GbiEnablePrestorage() */
#define DBG_DEM_GBIENABLEPRESTORAGE_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIENABLEPRESTORAGE_EXIT
/** \brief Exit point of function Dem_GbiEnablePrestorage() */
#define DBG_DEM_GBIENABLEPRESTORAGE_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIEVSIGNIFICANCE_ENTRY
/** \brief Entry point of function Dem_GbiEvSignificance() */
#define DBG_DEM_GBIEVSIGNIFICANCE_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIEVSIGNIFICANCE_EXIT
/** \brief Exit point of function Dem_GbiEvSignificance() */
#define DBG_DEM_GBIEVSIGNIFICANCE_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIFFCLASSIDX_ENTRY
/** \brief Entry point of function Dem_GbiFFClassIdx() */
#define DBG_DEM_GBIFFCLASSIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIFFCLASSIDX_EXIT
/** \brief Exit point of function Dem_GbiFFClassIdx() */
#define DBG_DEM_GBIFFCLASSIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIMAXNUMFFRECS_ENTRY
/** \brief Entry point of function Dem_GbiMaxNumFFRecs() */
#define DBG_DEM_GBIMAXNUMFFRECS_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIMAXNUMFFRECS_EXIT
/** \brief Exit point of function Dem_GbiMaxNumFFRecs() */
#define DBG_DEM_GBIMAXNUMFFRECS_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIFFRECNUMCLASSIDX_ENTRY
/** \brief Entry point of function Dem_GbiFFRecNumClassIdx() */
#define DBG_DEM_GBIFFRECNUMCLASSIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIFFRECNUMCLASSIDX_EXIT
/** \brief Exit point of function Dem_GbiFFRecNumClassIdx() */
#define DBG_DEM_GBIFFRECNUMCLASSIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIEDCLASSIDX_ENTRY
/** \brief Entry point of function Dem_GbiEDClassIdx() */
#define DBG_DEM_GBIEDCLASSIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIEDCLASSIDX_EXIT
/** \brief Exit point of function Dem_GbiEDClassIdx() */
#define DBG_DEM_GBIEDCLASSIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIOPCYCLEIDX_ENTRY
/** \brief Entry point of function Dem_GbiOpCycleIdx() */
#define DBG_DEM_GBIOPCYCLEIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIOPCYCLEIDX_EXIT
/** \brief Exit point of function Dem_GbiOpCycleIdx() */
#define DBG_DEM_GBIOPCYCLEIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIINDICATORHEALINGCYCLEIDX_ENTRY
/** \brief Entry point of function Dem_GbiIndicatorHealingCycleIdx() */
#define DBG_DEM_GBIINDICATORHEALINGCYCLEIDX_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GBIINDICATORHEALINGCYCLEIDX_EXIT
/** \brief Exit point of function Dem_GbiIndicatorHealingCycleIdx() */
#define DBG_DEM_GBIINDICATORHEALINGCYCLEIDX_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GBIINDICATORFAILURECYCLEIDX_ENTRY
/** \brief Entry point of function Dem_GbiIndicatorFailureCycleIdx() */
#define DBG_DEM_GBIINDICATORFAILURECYCLEIDX_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GBIINDICATORFAILURECYCLEIDX_EXIT
/** \brief Exit point of function Dem_GbiIndicatorFailureCycleIdx() */
#define DBG_DEM_GBIINDICATORFAILURECYCLEIDX_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GBINUMAGINGCYCLES_ENTRY
/** \brief Entry point of function Dem_GbiNumAgingCycles() */
#define DBG_DEM_GBINUMAGINGCYCLES_ENTRY(a)
#endif

#ifndef DBG_DEM_GBINUMAGINGCYCLES_EXIT
/** \brief Exit point of function Dem_GbiNumAgingCycles() */
#define DBG_DEM_GBINUMAGINGCYCLES_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIAGINGCYCLEIDX_ENTRY
/** \brief Entry point of function Dem_GbiAgingCycleIdx() */
#define DBG_DEM_GBIAGINGCYCLEIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIAGINGCYCLEIDX_EXIT
/** \brief Exit point of function Dem_GbiAgingCycleIdx() */
#define DBG_DEM_GBIAGINGCYCLEIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIEVENTFAILURECYCLEIDX_ENTRY
/** \brief Entry point of function Dem_GbiEventFailureCycleIdx() */
#define DBG_DEM_GBIEVENTFAILURECYCLEIDX_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GBIEVENTFAILURECYCLEIDX_EXIT
/** \brief Exit point of function Dem_GbiEventFailureCycleIdx() */
#define DBG_DEM_GBIEVENTFAILURECYCLEIDX_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GBIENCONDGRPIDX_ENTRY
/** \brief Entry point of function Dem_GbiEnCondGrpIdx() */
#define DBG_DEM_GBIENCONDGRPIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIENCONDGRPIDX_EXIT
/** \brief Exit point of function Dem_GbiEnCondGrpIdx() */
#define DBG_DEM_GBIENCONDGRPIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GETMULTIEVENTTRIGGERINGMASTERIDX_ENTRY
/** \brief Entry point of function Dem_GetMultiEventTriggeringMasterIdx() */
#define DBG_DEM_GETMULTIEVENTTRIGGERINGMASTERIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GETMULTIEVENTTRIGGERINGMASTERIDX_EXIT
/** \brief Exit point of function Dem_GetMultiEventTriggeringMasterIdx() */
#define DBG_DEM_GETMULTIEVENTTRIGGERINGMASTERIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIMULTIEVENTTRIGGERINGIDX_ENTRY
/** \brief Entry point of function Dem_GbiMultiEventTriggeringIdx() */
#define DBG_DEM_GBIMULTIEVENTTRIGGERINGIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIMULTIEVENTTRIGGERINGIDX_EXIT
/** \brief Exit point of function Dem_GbiMultiEventTriggeringIdx() */
#define DBG_DEM_GBIMULTIEVENTTRIGGERINGIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GETMULTIEVENTTRIGGERING_ENTRY
/** \brief Entry point of function Dem_GetMultiEventTriggering() */
#define DBG_DEM_GETMULTIEVENTTRIGGERING_ENTRY(a)
#endif

#ifndef DBG_DEM_GETMULTIEVENTTRIGGERING_EXIT
/** \brief Exit point of function Dem_GetMultiEventTriggering() */
#define DBG_DEM_GETMULTIEVENTTRIGGERING_EXIT(a, b)
#endif

#ifndef DBG_DEM_FINDNEXTDEPENDENTEVENT_ENTRY
/** \brief Entry point of function Dem_FindNextDependentEvent() */
#define DBG_DEM_FINDNEXTDEPENDENTEVENT_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_FINDNEXTDEPENDENTEVENT_EXIT
/** \brief Exit point of function Dem_FindNextDependentEvent() */
#define DBG_DEM_FINDNEXTDEPENDENTEVENT_EXIT(a, b, c, d)
#endif

#if (DEM_NUM_INDICATOR_LINKS > 0U)
#ifndef DBG_DEM_GBIINDICATORUSED_ENTRY
/** \brief Entry point of function Dem_GbiIndicatorUsed() */
#define DBG_DEM_GBIINDICATORUSED_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIINDICATORUSED_EXIT
/** \brief Exit point of function Dem_GbiIndicatorUsed() */
#define DBG_DEM_GBIINDICATORUSED_EXIT(a, b)
#endif
#endif

#ifndef DBG_DEM_GBIINDICATORCLASSINDEX_ENTRY
/** \brief Entry point of function Dem_GbiIndicatorClassIndex() */
#define DBG_DEM_GBIINDICATORCLASSINDEX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIINDICATORCLASSINDEX_EXIT
/** \brief Exit point of function Dem_GbiIndicatorClassIndex() */
#define DBG_DEM_GBIINDICATORCLASSINDEX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIPRIORITY_ENTRY
/** \brief Entry point of function Dem_GbiPriority() */
#define DBG_DEM_GBIPRIORITY_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIPRIORITY_EXIT
/** \brief Exit point of function Dem_GbiPriority() */
#define DBG_DEM_GBIPRIORITY_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIDEBOUNCEALGO_ENTRY
/** \brief Entry point of function Dem_GbiDebounceAlgo() */
#define DBG_DEM_GBIDEBOUNCEALGO_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIDEBOUNCEALGO_EXIT
/** \brief Exit point of function Dem_GbiDebounceAlgo() */
#define DBG_DEM_GBIDEBOUNCEALGO_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIDEBOUNCECOUNTERCLASSIDX_ENTRY
/** \brief Entry point of function Dem_GbiDebounceCounterClassIdx() */
#define DBG_DEM_GBIDEBOUNCECOUNTERCLASSIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIDEBOUNCECOUNTERCLASSIDX_EXIT
/** \brief Exit point of function Dem_GbiDebounceCounterClassIdx() */
#define DBG_DEM_GBIDEBOUNCECOUNTERCLASSIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIDEBOUNCESTATUSIDX_ENTRY
/** \brief Entry point of function Dem_GbiDebounceStatusIdx() */
#define DBG_DEM_GBIDEBOUNCESTATUSIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIDEBOUNCESTATUSIDX_EXIT
/** \brief Exit point of function Dem_GbiDebounceStatusIdx() */
#define DBG_DEM_GBIDEBOUNCESTATUSIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIDEBOUNCEIDX_ENTRY
/** \brief Entry point of function Dem_GbiDebounceIdx() */
#define DBG_DEM_GBIDEBOUNCEIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIDEBOUNCEIDX_EXIT
/** \brief Exit point of function Dem_GbiDebounceIdx() */
#define DBG_DEM_GBIDEBOUNCEIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIDEBOUNCETIMECLASSIDX_ENTRY
/** \brief Entry point of function Dem_GbiDebounceTimeClassIdx() */
#define DBG_DEM_GBIDEBOUNCETIMECLASSIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIDEBOUNCETIMECLASSIDX_EXIT
/** \brief Exit point of function Dem_GbiDebounceTimeClassIdx() */
#define DBG_DEM_GBIDEBOUNCETIMECLASSIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIEVENTKIND_ENTRY
/** \brief Entry point of function Dem_GbiEventKind() */
#define DBG_DEM_GBIEVENTKIND_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIEVENTKIND_EXIT
/** \brief Exit point of function Dem_GbiEventKind() */
#define DBG_DEM_GBIEVENTKIND_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIEVENTFAILURECLASSIDX_ENTRY
/** \brief Entry point of function Dem_GbiEventFailureClassIdx() */
#define DBG_DEM_GBIEVENTFAILURECLASSIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIEVENTFAILURECLASSIDX_EXIT
/** \brief Exit point of function Dem_GbiEventFailureClassIdx() */
#define DBG_DEM_GBIEVENTFAILURECLASSIDX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIOBDAGINGCYCLECOUNTERTHRESHOLD_ENTRY
/** \brief Entry point of function Dem_GbiOBDAgingCycleCounterThreshold() */
#define DBG_DEM_GBIOBDAGINGCYCLECOUNTERTHRESHOLD_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIOBDAGINGCYCLECOUNTERTHRESHOLD_EXIT
/** \brief Entry point of function Dem_GbiOBDAgingCycleCounterThreshold() */
#define DBG_DEM_GBIOBDAGINGCYCLECOUNTERTHRESHOLD_EXIT(a,b)
#endif

#ifndef DBG_DEM_GETPFFINDEX_ENTRY
/** \brief Entry point of function Dem_GetPFFIndex() */
#define DBG_DEM_GETPFFINDEX_ENTRY(a)
#endif

#ifndef DBG_DEM_GETPFFINDEX_EXIT
/** \brief Exit point of function Dem_GetPFFIndex() */
#define DBG_DEM_GETPFFINDEX_EXIT(a, b)
#endif

#ifndef DBG_DEM_GBIEVAVAILABLE_ENTRY
/** \brief Entry point of function Dem_GbiEvAvailable() */
#define DBG_DEM_GBIEVAVAILABLE_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIEVAVAILABLE_EXIT
/** \brief Entry point of function Dem_GbiEvAvailable() */
#define DBG_DEM_GBIEVAVAILABLE_EXIT(a, b)
#endif

#ifndef DBG_DEM_ISANYEVAVAILABLE_ENTRY
/** \brief Entry point of function Dem_IsAnyEvAvailable() */
#define DBG_DEM_ISANYEVAVAILABLE_ENTRY(a)
#endif

#ifndef DBG_DEM_ISANYEVAVAILABLE_EXIT
/** \brief Entry point of function Dem_IsAnyEvAvailable() */
#define DBG_DEM_ISANYEVAVAILABLE_EXIT(a, b)
#endif

#ifndef DBG_DEM_SEARCHFORENTRY_ENTRY
/** \brief Entry point of function Dem_SearchForEntry() */
#define DBG_DEM_SEARCHFORENTRY_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_SEARCHFORENTRY_EXIT
/** \brief Exit point of function Dem_SearchForEntry() */
#define DBG_DEM_SEARCHFORENTRY_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_SETEVENTRYEXISTS_ENTRY
/** \brief Entry point of function Dem_SetEvEntryExists() */
#define DBG_DEM_SETEVENTRYEXISTS_ENTRY(a, b)
#endif

#ifndef DBG_DEM_SETEVENTRYEXISTS_EXIT
/** \brief Exit point of function Dem_SetEvEntryExists() */
#define DBG_DEM_SETEVENTRYEXISTS_EXIT(a, b)
#endif

#ifndef DBG_DEM_INITEVENTRYEXISTS_ENTRY
/** \brief Entry point of function InitEvEntryExists() */
#define DBG_DEM_INITEVENTRYEXISTS_ENTRY()
#endif

#ifndef DBG_DEM_INITEVENTRYEXISTS_EXIT
/** \brief Exit point of function Dem_InitEvEntryExists() */
#define DBG_DEM_INITEVENTRYEXISTS_EXIT()
#endif

#ifndef DBG_DEM_FINDFFSEGIDX_ENTRY
/** \brief Entry point of function Dem_FindFFSegIdx() */
#define DBG_DEM_FINDFFSEGIDX_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_FINDFFSEGIDX_EXIT
/** \brief Exit point of function Dem_FindFFSegIdx() */
#define DBG_DEM_FINDFFSEGIDX_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_FFENTRYDATA_ENTRY
/** \brief Entry point of function Dem_FFEntryData() */
#define DBG_DEM_FFENTRYDATA_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_FFENTRYDATA_EXIT
/** \brief Exit point of function Dem_FFEntryData() */
#define DBG_DEM_FFENTRYDATA_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_CHECKFFRECNUMVALID_ENTRY
/** \brief Entry point of function Dem_CheckFFRecNumValid() */
#define DBG_DEM_CHECKFFRECNUMVALID_ENTRY(a, b)
#endif

#ifndef DBG_DEM_CHECKFFRECNUMVALID_EXIT
/** \brief Exit point of function Dem_CheckFFRecNumValid() */
#define DBG_DEM_CHECKFFRECNUMVALID_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_CHECKFFRECNUMSTORED_ENTRY
/** \brief Entry point of function Dem_CheckFFRecNumStored() */
#define DBG_DEM_CHECKFFRECNUMSTORED_ENTRY(a, b)
#endif

#ifndef DBG_DEM_CHECKFFRECNUMSTORED_EXIT
/** \brief Exit point of function Dem_CheckFFRecNumStored() */
#define DBG_DEM_CHECKFFRECNUMSTORED_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_EDENTRYDATA_ENTRY
/** \brief Entry point of function Dem_EDEntryData() */
#define DBG_DEM_EDENTRYDATA_ENTRY(a)
#endif

#ifndef DBG_DEM_EDENTRYDATA_EXIT
/** \brief Exit point of function Dem_EDEntryData() */
#define DBG_DEM_EDENTRYDATA_EXIT(a, b)
#endif

#ifndef DBG_DEM_INTVALENTRYDATA_ENTRY
/** \brief Entry point of function Dem_IntValEntryData() */
#define DBG_DEM_INTVALENTRYDATA_ENTRY(a)
#endif

#ifndef DBG_DEM_INTVALENTRYDATA_EXIT
/** \brief Exit point of function Dem_IntValEntryData() */
#define DBG_DEM_INTVALENTRYDATA_EXIT(a, b)
#endif

#ifndef DBG_DEM_FINDEDNUMINDEX_ENTRY
/** \brief Entry point of function Dem_FindEDNumIndex() */
#define DBG_DEM_FINDEDNUMINDEX_ENTRY(a, b)
#endif

#ifndef DBG_DEM_FINDEDNUMINDEX_EXIT
/** \brief Exit point of function Dem_FindEDNumIndex() */
#define DBG_DEM_FINDEDNUMINDEX_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_CHECKPROCESSINCCONDITIONS_ENTRY
/** \brief Entry point of function Dem_CheckProcessingConditions() */
#define DBG_DEM_CHECKPROCESSINCCONDITIONS_ENTRY(a)
#endif

#ifndef DBG_DEM_CHECKPROCESSINCCONDITIONS_EXIT
/** \brief Exit point of function Dem_CheckProcessingConditions() */
#define DBG_DEM_CHECKPROCESSINCCONDITIONS_EXIT(a, b)
#endif

#ifndef DBG_DEM_INTERNALSETEVENTSTATUS_ENTRY
/** \brief Entry point of function Dem_InternalSetEventStatus() */
#define DBG_DEM_INTERNALSETEVENTSTATUS_ENTRY(a, b)
#endif

#ifndef DBG_DEM_INTERNALSETEVENTSTATUS_EXIT
/** \brief Exit point of function Dem_InternalSetEventStatus() */
#define DBG_DEM_INTERNALSETEVENTSTATUS_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_INTERNALPROCESSEVENTREPORT_ENTRY
/** \brief Entry point of function Dem_InternalProcessEventReport() */
#define DBG_DEM_INTERNALPROCESSEVENTREPORT_ENTRY(a, b)
#endif

#ifndef DBG_DEM_INTERNALPROCESSEVENTREPORT_EXIT
/** \brief Exit point of function Dem_InternalProcessEventReport() */
#define DBG_DEM_INTERNALPROCESSEVENTREPORT_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_UPDATEEVENTSTATUSONQUALIFICATION_ENTRY
/** \brief Entry point of function Dem_UpdateEventStatusOnQualification() */
#define DBG_DEM_UPDATEEVENTSTATUSONQUALIFICATION_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_UPDATEEVENTSTATUSONQUALIFICATION_EXIT
/** \brief Exit point of function Dem_UpdateEventStatusOnQualification() */
#define DBG_DEM_UPDATEEVENTSTATUSONQUALIFICATION_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_CLEAREVENTENTRY_ENTRY
/** \brief Entry point of function Dem_ClearEventEntry() */
#define DBG_DEM_CLEAREVENTENTRY_ENTRY(a)
#endif

#ifndef DBG_DEM_CLEAREVENTENTRY_EXIT
/** \brief Exit point of function Dem_ClearEventEntry() */
#define DBG_DEM_CLEAREVENTENTRY_EXIT(a)
#endif

#ifndef DBG_DEM_CLEARSINGLEOBDFREEZEFRAME_ENTRY
/** \brief Entry point of function Dem_ClearSingleOBDFreezeFrame() */
#define DBG_DEM_CLEARSINGLEOBDFREEZEFRAME_ENTRY(a)
#endif

#ifndef DBG_DEM_CLEARSINGLEOBDFREEZEFRAME_EXIT
/** \brief Exit point of function Dem_ClearSingleOBDFreezeFrame() */
#define DBG_DEM_CLEARSINGLEOBDFREEZEFRAME_EXIT(a)
#endif

#ifndef DBG_DEM_EVMEMENTRYSIZE_ENTRY
/** \brief Entry point of function Dem_EvMemEntrySize() */
#define DBG_DEM_EVMEMENTRYSIZE_ENTRY(a, b)
#endif

#ifndef DBG_DEM_EVMEMENTRYSIZE_EXIT
/** \brief Exit point of function Dem_EvMemEntrySize() */
#define DBG_DEM_EVMEMENTRYSIZE_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_NVMGATEENTRYPRIMARYBLOCKCALLBACK_ENTRY
/** \brief Entry point of function Dem_NvMGateEntryPrimaryBlockCallback() */
#define DBG_DEM_NVMGATEENTRYPRIMARYBLOCKCALLBACK_ENTRY(a, b)
#endif

#ifndef DBG_DEM_NVMGATEENTRYPRIMARYBLOCKCALLBACK_EXIT
/** \brief Exit point of function Dem_NvMGateEntryPrimaryBlockCallback() */
#define DBG_DEM_NVMGATEENTRYPRIMARYBLOCKCALLBACK_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_NVMGATEENTRYSECONDARYBLOCKCALLBACK_ENTRY
/** \brief Entry point of function Dem_NvMGateEntrySecondaryBlockCallback() */
#define DBG_DEM_NVMGATEENTRYSECONDARYBLOCKCALLBACK_ENTRY(a, b)
#endif

#ifndef DBG_DEM_NVMGATEENTRYSECONDARYBLOCKCALLBACK_EXIT
/** \brief Exit point of function Dem_NvMGateEntrySecondaryBlockCallback() */
#define DBG_DEM_NVMGATEENTRYSECONDARYBLOCKCALLBACK_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_NVMGATEENTRYMIRRORBLOCKCALLBACK_ENTRY
/** \brief Entry point of function Dem_NvMGateEntryMirrorBlockCallback() */
#define DBG_DEM_NVMGATEENTRYMIRRORBLOCKCALLBACK_ENTRY(a, b)
#endif

#ifndef DBG_DEM_NVMGATEENTRYMIRRORBLOCKCALLBACK_EXIT
/** \brief Exit point of function Dem_NvMGateEntryMirrorBlockCallback() */
#define DBG_DEM_NVMGATEENTRYMIRRORBLOCKCALLBACK_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_NVMWRITEFINISHEDPERMANENTMEMORY_ENTRY
/** \brief Entry point of function Dem_NvMWriteFinishedPermanentMemory() */
#define DBG_DEM_NVMWRITEFINISHEDPERMANENTMEMORY_ENTRY(a, b)
#endif

#ifndef DBG_DEM_NVMWRITEFINISHEDPERMANENTMEMORY_EXIT
/** \brief Exit point of function Dem_NvMWriteFinishedPermanentMemory() */
#define DBG_DEM_NVMWRITEFINISHEDPERMANENTMEMORY_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_NVMWRITECOPYPERMANENTMEMORY_ENTRY
/** \brief Entry point of function Dem_NvMWriteCopyPermanentMemory() */
#define DBG_DEM_NVMWRITECOPYPERMANENTMEMORY_ENTRY(a)
#endif

#ifndef DBG_DEM_NVMWRITECOPYPERMANENTMEMORY_EXIT
/** \brief Exit point of function Dem_NvMWriteCopyPermanentMemory() */
#define DBG_DEM_NVMWRITECOPYPERMANENTMEMORY_EXIT(a, b)
#endif

#ifndef DBG_DEM_NVMREADCOPYPERMANENTMEMORY_ENTRY
/** \brief Entry point of function Dem_NvMReadCopyPermanentMemory() */
#define DBG_DEM_NVMREADCOPYPERMANENTMEMORY_ENTRY(a)
#endif

#ifndef DBG_DEM_NVMREADCOPYPERMANENTMEMORY_EXIT
/** \brief Exit point of function Dem_NvMReadCopyPermanentMemory() */
#define DBG_DEM_NVMREADCOPYPERMANENTMEMORY_EXIT(a, b)
#endif

#ifndef DBG_DEM_NVMGATEENTRYBLOCKCALLBACK_ENTRY
/** \brief Entry point of function Dem_NvMGateEntryBlockCallback() */
#define DBG_DEM_NVMGATEENTRYBLOCKCALLBACK_ENTRY(a, b)
#endif

#ifndef DBG_DEM_NVMGATEENTRYBLOCKCALLBACK_EXIT
/** \brief Exit point of function Dem_NvMGateEntrytBlockCallback() */
#define DBG_DEM_NVMGATEENTRYBLOCKCALLBACK_EXIT(a, b)
#endif

#ifndef DBG_DEM_NVMREADCOPYEVENTSTATUSBLOCK_ENTRY
/** \brief Entry point of function Dem_NvMReadCopyEventStatusBlock() */
#define DBG_DEM_NVMREADCOPYEVENTSTATUSBLOCK_ENTRY(a)
#endif

#ifndef DBG_DEM_NVMREADCOPYEVENTSTATUSBLOCK_EXIT
/** \brief Exit point of function Dem_NvMReadCopyEventStatusBlock() */
#define DBG_DEM_NVMREADCOPYEVENTSTATUSBLOCK_EXIT(a, b)
#endif

#ifndef DBG_DEM_NVMWRITECOPYEVENTSTATUSBLOCK_ENTRY
/** \brief Entry point of function Dem_NvMWriteCopyEventStatusBlock() */
#define DBG_DEM_NVMWRITECOPYEVENTSTATUSBLOCK_ENTRY(a)
#endif

#ifndef DBG_DEM_NVMWRITECOPYEVENTSTATUSBLOCK_EXIT
/** \brief Exit point of function Dem_NvMWriteCopyEventStatusBlock() */
#define DBG_DEM_NVMWRITECOPYEVENTSTATUSBLOCK_EXIT(a, b)
#endif

#ifndef DBG_DEM_NVMEVENTSTATUSBLOCKCALLBACK_ENTRY
/** \brief Entry point of function Dem_NvMEventStatusBlockCallback() */
#define DBG_DEM_NVMEVENTSTATUSBLOCKCALLBACK_ENTRY(a, b)
#endif

#ifndef DBG_DEM_NVMEVENTSTATUSBLOCKCALLBACK_EXIT
/** \brief Exit point of function Dem_NvMEventStatusBlockCallback() */
#define DBG_DEM_NVMEVENTSTATUSBLOCKCALLBACK_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_CBINITMONITORFOREVENT_ENTRY
/** \brief Entry point of function Dem_CbInitMonitorForEvent() */
#define DBG_DEM_CBINITMONITORFOREVENT_ENTRY(a, b)
#endif

#ifndef DBG_DEM_CBINITMONITORFOREVENT_EXIT
/** \brief Exit point of function Dem_CbInitMonitorForEvent() */
#define DBG_DEM_CBINITMONITORFOREVENT_EXIT(a, b)
#endif

#ifndef DBG_DEM_CBTRIGGERONEVENTSTATUS_ENTRY
/** \brief Entry point of function Dem_CbTriggerOnEventStatus() */
#define DBG_DEM_CBTRIGGERONEVENTSTATUS_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_CBTRIGGERONEVENTSTATUS_EXIT
/** \brief Exit point of function Dem_CbTriggerOnEventStatus() */
#define DBG_DEM_CBTRIGGERONEVENTSTATUS_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_CBTRIGGERONEVENTDATA_ENTRY
/** \brief Entry point of function Dem_CbTriggerOnEventData() */
#define DBG_DEM_CBTRIGGERONEVENTDATA_ENTRY(a)
#endif

#ifndef DBG_DEM_CBTRIGGERONEVENTDATA_EXIT
/** \brief Exit point of function Dem_CbTriggerOnEventData() */
#define DBG_DEM_CBTRIGGERONEVENTDATA_EXIT(a)
#endif

#ifndef DBG_DEM_CLEARTHISCYCLEANDREADINESSSTATUS_ENTRY
/** \brief Entry point of function Dem_ClearThisCycleAndReadinessStatus() */
#define DBG_DEM_CLEARTHISCYCLEANDREADINESSSTATUS_ENTRY(a)
#endif

#ifndef DBG_DEM_CLEARTHISCYCLEANDREADINESSSTATUS_EXIT
/** \brief Exit point of function Dem_ClearThisCycleAndReadinessStatus() */
#define DBG_DEM_CLEARTHISCYCLEANDREADINESSSTATUS_EXIT(a)
#endif

#ifndef DBG_DEM_CLEARTHISCYCLEANDREADINESSSTATUSCMBEVENTS_ENTRY
/** \brief Entry point of function Dem_ClearThisCycleAndReadinessStatusCmbEvents() */
#define DBG_DEM_CLEARTHISCYCLEANDREADINESSSTATUSCMBEVENTS_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_CLEARTHISCYCLEANDREADINESSSTATUSCMBEVENTS_EXIT
/** \brief Exit point of function Dem_ClearThisCycleAndReadinessStatusCmbEvents() */
#define DBG_DEM_CLEARTHISCYCLEANDREADINESSSTATUSCMBEVENTS_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_CBCLEAREVENTALLOWED_ENTRY
/** \brief Entry point of function Dem_CbClearEventAllowed() */
#define DBG_DEM_CBCLEAREVENTALLOWED_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_CBCLEAREVENTALLOWED_EXIT
/** \brief Exit point of function Dem_CbClearEventAllowed() */
#define DBG_DEM_CBCLEAREVENTALLOWED_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_CBGETFAULTDETECTIONCOUNTER_ENTRY
/** \brief Entry point of function Dem_CbGetFaultDetectionCounter() */
#define DBG_DEM_CBGETFAULTDETECTIONCOUNTER_ENTRY(a, b)
#endif

#ifndef DBG_DEM_CBGETFAULTDETECTIONCOUNTER_EXIT
/** \brief Exit point of function Dem_CbGetFaultDetectionCounter() */
#define DBG_DEM_CBGETFAULTDETECTIONCOUNTER_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_SIZEOFFF_ENTRY
/** \brief Entry point of function Dem_SizeOfFF() */
#define DBG_DEM_SIZEOFFF_ENTRY(a, b)
#endif

#ifndef DBG_DEM_SIZEOFFF_EXIT
/** \brief Exit point of function Dem_SizeOfFF() */
#define DBG_DEM_SIZEOFFF_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_SIZEOFFFSEGMENT_ENTRY
/** \brief Entry point of function Dem_SizeOfFFSegment() */
#define DBG_DEM_SIZEOFFFSEGMENT_ENTRY(a)
#endif

#ifndef DBG_DEM_SIZEOFFFSEGMENT_EXIT
/** \brief Exit point of function Dem_SizeOfFFSegment() */
#define DBG_DEM_SIZEOFFFSEGMENT_EXIT(a, b)
#endif

#ifndef DBG_DEM_GETNUMFFSEGS_ENTRY
/** \brief Entry point of function Dem_GetNumFFSegs() */
#define DBG_DEM_GETNUMFFSEGS_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETNUMFFSEGS_EXIT
/** \brief Exit point of function Dem_GetNumFFSegs() */
#define DBG_DEM_GETNUMFFSEGS_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_SIZEOFED_ENTRY
/** \brief Entry point of function Dem_SizeOfED() */
#define DBG_DEM_SIZEOFED_ENTRY(a)
#endif

#ifndef DBG_DEM_SIZEOFED_EXIT
/** \brief Exit point of function Dem_SizeOfED() */
#define DBG_DEM_SIZEOFED_EXIT(a, b)
#endif

#ifndef DBG_DEM_SIZEOFEDSEGMENT_ENTRY
/** \brief Entry point of function Dem_SizeOfEDSegment() */
#define DBG_DEM_SIZEOFEDSEGMENT_ENTRY(a)
#endif

#ifndef DBG_DEM_SIZEOFEDSEGMENT_EXIT
/** \brief Exit point of function Dem_SizeOfEDSegment() */
#define DBG_DEM_SIZEOFEDSEGMENT_EXIT(a, b)
#endif

#ifndef DBG_DEM_SIZEOFMAPINTVALSEGMENT_ENTRY
/** \brief Entry point of function Dem_SizeOfMapIntValSegment() */
#define DBG_DEM_SIZEOFMAPINTVALSEGMENT_ENTRY(a)
#endif

#ifndef DBG_DEM_SIZEOFMAPINTVALSEGMENT_EXIT
/** \brief Exit point of function Dem_SizeOfMapIntValSegment() */
#define DBG_DEM_SIZEOFMAPINTVALSEGMENT_EXIT(a, b)
#endif

#ifndef DBG_DEM_ASSEMBLEEXTENDEDDATASEG_ENTRY
/** \brief Entry point of function Dem_AssembleExtendedDataSeg() */
#define DBG_DEM_ASSEMBLEEXTENDEDDATASEG_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DEM_ASSEMBLEEXTENDEDDATASEG_EXIT
/** \brief Exit point of function Dem_AssembleExtendedDataSeg() */
#define DBG_DEM_ASSEMBLEEXTENDEDDATASEG_EXIT(a, b, c, d, e, f)
#endif

#ifndef DBG_DEM_ASSEMBLEEDSEGINTDATA_ENTRY
/** \brief Entry point of function Dem_AssembleEDSegIntData() */
#define DBG_DEM_ASSEMBLEEDSEGINTDATA_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_ASSEMBLEEDSEGINTDATA_EXIT
/** \brief Exit point of function Dem_AssembleEDSegIntData() */
#define DBG_DEM_ASSEMBLEEDSEGINTDATA_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_SIZEOFINTVALAREA_ENTRY
/** \brief Entry point of function Dem_SizeOfIntValArea() */
#define DBG_DEM_SIZEOFINTVALAREA_ENTRY(a)
#endif

#ifndef DBG_DEM_SIZEOFINTVALAREA_EXIT
/** \brief Exit point of function Dem_SizeOfIntValArea() */
#define DBG_DEM_SIZEOFINTVALAREA_EXIT(a, b)
#endif

#ifndef DBG_DEM_SHIFTDATA_ENTRY
/** \brief Entry point of function Dem_ShiftData() */
#define DBG_DEM_SHIFTDATA_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_SHIFTDATA_EXIT
/** \brief Exit point of function Dem_ShiftData() */
#define DBG_DEM_SHIFTDATA_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_CHECKFORIMMEDIATEENTRYSTORAGE_ENTRY
/** \brief Entry point of function Dem_CheckForImmediateEntryStorage() */
#define DBG_DEM_CHECKFORIMMEDIATEENTRYSTORAGE_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_CHECKFORIMMEDIATEENTRYSTORAGE_EXIT
/** \brief Exit point of function Dem_CheckForImmediateEntryStorage() */
#define DBG_DEM_CHECKFORIMMEDIATEENTRYSTORAGE_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_STOREIMMEDIATELYNEXT_ENTRY
/** \brief Entry point of function Dem_StoreImmediatelyNext() */
#define DBG_DEM_STOREIMMEDIATELYNEXT_ENTRY(a)
#endif

#ifndef DBG_DEM_STOREIMMEDIATELYNEXT_EXIT
/** \brief Exit point of function Dem_StoreImmediatelyNext() */
#define DBG_DEM_STOREIMMEDIATELYNEXT_EXIT(a, b)
#endif

#ifndef DBG_DEM_TRIGGERNVMWRITEEVENTSTATUS_ENTRY
/** \brief Entry point of function Dem_TriggerNvmWriteEventStatus() */
#define DBG_DEM_TRIGGERNVMWRITEEVENTSTATUS_ENTRY()
#endif

#ifndef DBG_DEM_TRIGGERNVMWRITEEVENTSTATUS_EXIT
/** \brief Exit point of function Dem_TriggerNvmWriteEventStatus() */
#define DBG_DEM_TRIGGERNVMWRITEEVENTSTATUS_EXIT(a)
#endif

#ifndef DBG_DEM_CHECKFORTRIGGERNVMWRITEGATEENTRY_ENTRY
/** \brief Entry point of function Dem_CheckForTriggerNvmWriteGateEntry() */
#define DBG_DEM_CHECKFORTRIGGERNVMWRITEGATEENTRY_ENTRY()
#endif

#ifndef DBG_DEM_CHECKFORTRIGGERNVMWRITEGATEENTRY_EXIT
/** \brief Exit point of function Dem_CheckForTriggerNvmWriteGateEntry() */
#define DBG_DEM_CHECKFORTRIGGERNVMWRITEGATEENTRY_EXIT()
#endif

#ifndef DBG_DEM_CHECKFOREVENTWIRUPDATE_ENTRY
/** \brief Entry point of function Dem_CheckForEventWIRUpdate() */
#define DBG_DEM_CHECKFOREVENTWIRUPDATE_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_CHECKFOREVENTWIRUPDATE_EXIT
/** \brief Exit point of function Dem_CheckForEventWIRUpdate() */
#define DBG_DEM_CHECKFOREVENTWIRUPDATE_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_STOREIMMEDIATELY_ENTRY
/** \brief Entry point of function Dem_StoreImmediately() */
#define DBG_DEM_STOREIMMEDIATELY_ENTRY(a, b)
#endif

#ifndef DBG_DEM_STOREIMMEDIATELY_EXIT
/** \brief Exit point of function Dem_StoreImmediately() */
#define DBG_DEM_STOREIMMEDIATELY_EXIT(a, b)
#endif

#ifndef DBG_DEM_RESETDEBOUNCETIMEBASED_ENTRY
/** \brief Entry point of function Dem_ResetDebounceTimeBased() */
#define DBG_DEM_RESETDEBOUNCETIMEBASED_ENTRY(a)
#endif

#ifndef DBG_DEM_RESETDEBOUNCETIMEBASED_EXIT
/** \brief Exit point of function Dem_ResetDebounceTimeBased() */
#define DBG_DEM_RESETDEBOUNCETIMEBASED_EXIT(a)
#endif

#ifndef DBG_DEM_FREEZEDEBOUNCETIMEBASED_ENTRY
/** \brief Entry point of function Dem_FreezeDebounceTimeBased() */
#define DBG_DEM_FREEZEDEBOUNCETIMEBASED_ENTRY(a)
#endif

#ifndef DBG_DEM_FREEZEDEBOUNCETIMEBASED_EXIT
/** \brief Exit point of function Dem_FreezeDebounceTimeBased() */
#define DBG_DEM_FREEZEDEBOUNCETIMEBASED_EXIT(a)
#endif

#ifndef DBG_DEM_RESETDEBOUNCECOUNTERBASED_ENTRY
/** \brief Entry point of function Dem_ResetDebounceCounterBased() */
#define DBG_DEM_RESETDEBOUNCECOUNTERBASED_ENTRY(a)
#endif

#ifndef DBG_DEM_RESETDEBOUNCECOUNTERBASED_EXIT
/** \brief Exit point of function Dem_ResetDebounceCounterBased() */
#define DBG_DEM_RESETDEBOUNCECOUNTERBASED_EXIT(a)
#endif

#ifndef DBG_DEM_FREEZEDEBOUNCECOUNTERBASED_ENTRY
/** \brief Entry point of function Dem_FreezeDebounceCounterBased() */
#define DBG_DEM_FREEZEDEBOUNCECOUNTERBASED_ENTRY(a)
#endif

#ifndef DBG_DEM_FREEZEDEBOUNCECOUNTERBASED_EXIT
/** \brief Exit point of function Dem_FreezeDebounceCounterBased() */
#define DBG_DEM_FREEZEDEBOUNCECOUNTERBASED_EXIT(a)
#endif

#ifndef DBG_DEM_RESETDEBOUNCEFREQUENCYBASED_ENTRY
/** \brief Entry point of function Dem_ResetDebounceFrequencyBased() */
#define DBG_DEM_RESETDEBOUNCEFREQUENCYBASED_ENTRY(a)
#endif

#ifndef DBG_DEM_RESETDEBOUNCEFREQUENCYBASED_EXIT
/** \brief Exit point of function Dem_ResetDebounceFrequencyBased() */
#define DBG_DEM_RESETDEBOUNCEFREQUENCYBASED_EXIT(a)
#endif

#ifndef DBG_DEM_FREEZEDEBOUNCEFREQUENCYBASED_ENTRY
/** \brief Entry point of function Dem_FreezeDebounceFrequencyBased() */
#define DBG_DEM_FREEZEDEBOUNCEFREQUENCYBASED_ENTRY(a)
#endif

#ifndef DBG_DEM_FREEZEDEBOUNCEFREQUENCYBASED_EXIT
/** \brief Exit point of function Dem_FreezeDebounceFrequencyBased() */
#define DBG_DEM_FREEZEDEBOUNCEFREQUENCYBASED_EXIT(a)
#endif

#ifndef DBG_DEM_RESETDEBOUNCEMONITOR_ENTRY
/** \brief Entry point of function Dem_ResetDebounceMonitor() */
#define DBG_DEM_RESETDEBOUNCEMONITOR_ENTRY(a)
#endif

#ifndef DBG_DEM_RESETDEBOUNCEMONITOR_EXIT
/** \brief Exit point of function Dem_ResetDebounceMonitor() */
#define DBG_DEM_RESETDEBOUNCEMONITOR_EXIT(a)
#endif

#ifndef DBG_DEM_DEBOUNCEEVENTCOUNTERBASED_ENTRY
/** \brief Entry point of function Dem_DebounceEventCounterBased() */
#define DBG_DEM_DEBOUNCEEVENTCOUNTERBASED_ENTRY(a, b)
#endif

#ifndef DBG_DEM_DEBOUNCEEVENTCOUNTERBASED_EXIT
/** \brief Exit point of function Dem_DebounceEventCounterBased() */
#define DBG_DEM_DEBOUNCEEVENTCOUNTERBASED_EXIT(a, b)
#endif

#ifndef DBG_DEM_PROCESSUNCONFIRMEDTHRESHOLD_ENTRY
/** \brief Entry point of function Dem_ProcessUnconfirmedThreshold() */
#define DBG_DEM_PROCESSUNCONFIRMEDTHRESHOLD_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_PROCESSUNCONFIRMEDTHRESHOLD_EXIT
/** \brief Exit point of function Dem_ProcessUnconfirmedThreshold() */
#define DBG_DEM_PROCESSUNCONFIRMEDTHRESHOLD_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_DEBOUNCEEVENTFREQUENCYBASED_ENTRY
/** \brief Entry point of function Dem_DebounceEventFrequencyBased() */
#define DBG_DEM_DEBOUNCEEVENTFREQUENCYBASED_ENTRY(a, b)
#endif

#ifndef DBG_DEM_DEBOUNCEEVENTFREQUENCYBASED_EXIT
/** \brief Exit point of function Dem_DebounceEventFrequencyBased() */
#define DBG_DEM_DEBOUNCEEVENTFREQUENCYBASED_EXIT(a, b)
#endif

#ifndef DBG_DEM_DEBOUNCEEVENTTIMEBASED_ENTRY
/** \brief Entry point of function Dem_DebounceEventTimeBased() */
#define DBG_DEM_DEBOUNCEEVENTTIMEBASED_ENTRY(a, b)
#endif

#ifndef DBG_DEM_DEBOUNCEEVENTTIMEBASED_EXIT
/** \brief Exit point of function Dem_DebounceEventTimeBased() */
#define DBG_DEM_DEBOUNCEEVENTTIMEBASED_EXIT(a, b)
#endif

#ifndef DBG_DEM_GETFDCCOUNTERBASED_ENTRY
/** \brief Entry point of function Dem_GetFDCCounterBased() */
#define DBG_DEM_GETFDCCOUNTERBASED_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETFDCCOUNTERBASED_EXIT
/** \brief Exit point of function Dem_GetFDCCounterBased() */
#define DBG_DEM_GETFDCCOUNTERBASED_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETFDCFREQUENCYBASED_ENTRY
/** \brief Entry point of function Dem_GetFDCFrequencyBased() */
#define DBG_DEM_GETFDCFREQUENCYBASED_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETFDCFREQUENCYBASED_EXIT
/** \brief Exit point of function Dem_GetFDCFrequencyBased() */
#define DBG_DEM_GETFDCFREQUENCYBASED_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETFDCTIMEBASED_ENTRY
/** \brief Entry point of function Dem_GetFDCTimeBased() */
#define DBG_DEM_GETFDCTIMEBASED_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETFDCTIMEBASED_EXIT
/** \brief Exit point of function Dem_GetFDCTimeBased() */
#define DBG_DEM_GETFDCTIMEBASED_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETBEHAVIORCOUNTERBASED_ENTRY
/** \brief Entry point of function Dem_GetBehaviorCounterBased() */
#define DBG_DEM_GETBEHAVIORCOUNTERBASED_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETBEHAVIORCOUNTERBASED_EXIT
/** \brief Exit point of function Dem_GetBehaviorCounterBased() */
#define DBG_DEM_GETBEHAVIORCOUNTERBASED_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETBEHAVIORFREQUENCYBASED_ENTRY
/** \brief Entry point of function Dem_GetBehaviorFrequencyBased() */
#define DBG_DEM_GETBEHAVIORFREQUENCYBASED_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETBEHAVIORFREQUENCYBASED_EXIT
/** \brief Exit point of function Dem_GetBehaviorFrequencyBased() */
#define DBG_DEM_GETBEHAVIORFREQUENCYBASED_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETBEHAVIORTIMEBASED_ENTRY
/** \brief Entry point of function Dem_GetBehaviorTimeBased() */
#define DBG_DEM_GETBEHAVIORTIMEBASED_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETBEHAVIORTIMEBASED_EXIT
/** \brief Exit point of function Dem_GetBehaviorTimeBased() */
#define DBG_DEM_GETBEHAVIORTIMEBASED_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_UPDATEFAILURECOUNTERFIRSTFAIL_ENTRY
/** \brief Entry point of function Dem_UpdateFailureCounterFirstFail() */
#define DBG_DEM_UPDATEFAILURECOUNTERFIRSTFAIL_ENTRY(a, b)
#endif

#ifndef DBG_DEM_UPDATEFAILURECOUNTERFIRSTFAIL_EXIT
/** \brief Exit point of function Dem_UpdateFailureCounterFirstFail() */
#define DBG_DEM_UPDATEFAILURECOUNTERFIRSTFAIL_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_RESETSTATUSCOUNTER_ENTRY
/** \brief Entry point of function Dem_ResetStatusCounter() */
#define DBG_DEM_RESETSTATUSCOUNTER_ENTRY(a)
#endif

#ifndef DBG_DEM_RESETSTATUSCOUNTER_EXIT
/** \brief Exit point of function Dem_ResetStatusCounter() */
#define DBG_DEM_RESETSTATUSCOUNTER_EXIT(a)
#endif

#ifndef DBG_DEM_GETSTATUSCOUNTERREFERENCE_ENTRY
/** \brief Entry point of function Dem_GetStatusCounterReference() */
#define DBG_DEM_GETSTATUSCOUNTERREFERENCE_ENTRY(a)
#endif

#ifndef DBG_DEM_GETSTATUSCOUNTERREFERENCE_EXIT
/** \brief Exit point of function Dem_GetStatusCounterReference() */
#define DBG_DEM_GETSTATUSCOUNTERREFERENCE_EXIT(a, b)
#endif

#ifndef DBG_DEM_GETCALLBACKINDEX_ENTRY
/** \brief Entry point of function Dem_GetCallbackIndex() */
#define DBG_DEM_GETCALLBACKINDEX_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DEM_GETCALLBACKINDEX_EXIT
/** \brief Exit point of function Dem_GetCallbackIndex() */
#define DBG_DEM_GETCALLBACKINDEX_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_GETRTECALLBACKINDEX_ENTRY
/** \brief Entry point of function Dem_GetRteCallbackIndex() */
#define DBG_DEM_GETRTECALLBACKINDEX_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DEM_GETRTECALLBACKINDEX_EXIT
/** \brief Exit point of function Dem_GetRteCallbackIndex() */
#define DBG_DEM_GETRTECALLBACKINDEX_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_INTERNALCLEARDTC_ENTRY
/** \brief Entry point of function Dem_InternalClearDTC() */
#define DBG_DEM_INTERNALCLEARDTC_ENTRY(a)
#endif

#ifndef DBG_DEM_INTERNALCLEARDTC_EXIT
/** \brief Exit point of function Dem_InternalClearDTC() */
#define DBG_DEM_INTERNALCLEARDTC_EXIT(a, b)
#endif

#ifndef DBG_DEM_ASR42_CLEARDTC_ENTRY
/** \brief Entry point of function Dem_ASR42_ClearDTC() */
#define DBG_DEM_ASR42_CLEARDTC_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_ASR42_CLEARDTC_EXIT
/** \brief Exit point of function Dem_ASR42_ClearDTC() */
#define DBG_DEM_ASR42_CLEARDTC_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_CLEARDTC_ENTRY
/** \brief Entry point of wrapper function Dem_ClearDTC() */
#define DBG_DEM_CLEARDTC_ENTRY(a)
#endif

#ifndef DBG_DEM_CLEARDTC_EXIT
/** \brief Exit point of function Dem_ClearDTC() */
#define DBG_DEM_CLEARDTC_EXIT(a, b)
#endif

#ifndef DBG_DEM_STARTCLEARDTC_ENTRY
/** \brief Entry point of function Dem_StartClearDTC() */
#define DBG_DEM_STARTCLEARDTC_ENTRY(a)
#endif

#ifndef DBG_DEM_STARTCLEARDTC_EXIT
/** \brief Exit point of function Dem_StartClearDTC() */
#define DBG_DEM_STARTCLEARDTC_EXIT(a, b)
#endif

#ifndef DBG_DEM_PROCESSCLEARDTC_ENTRY
/** \brief Entry point of function Dem_ProcessClearDTC() */
#define DBG_DEM_PROCESSCLEARDTC_ENTRY()
#endif

#ifndef DBG_DEM_PROCESSCLEARDTC_EXIT
/** \brief Exit point of function Dem_ProcessClearDTC() */
#define DBG_DEM_PROCESSCLEARDTC_EXIT()
#endif

#if ( (DEM_OBD_CENTRALIZED_PID31_HANDLING == STD_ON) || \
      (DEM_OBD_CENTRALIZED_PID21_HANDLING == STD_ON) || \
      (defined DEM_OPCYC_WARMUP_ID) )
#ifndef DBG_DEM_RESETOBDCOUNTERS_ENTRY
/** \brief Entry point of function Dem_ResetOBDCounters() */
#define DBG_DEM_RESETOBDCOUNTERS_ENTRY()
#endif

#ifndef DBG_DEM_RESETOBDCOUNTERS_EXIT
/** \brief Exit point of function Dem_ResetOBDCounters() */
#define DBG_DEM_RESETOBDCOUNTERS_EXIT()
#endif
#endif /* (DEM_OBD_CENTRALIZED_PID31_HANDLING == STD_ON) ||
          (DEM_OBD_CENTRALIZED_PID21_HANDLING == STD_ON) ||
          (defined DEM_OPCYC_WARMUP_ID) */

#ifndef DBG_DEM_ISDTCGROUPVALUE_ENTRY
/** \brief Entry point of function Dem_IsDTCGroupValue() */
#define DBG_DEM_ISDTCGROUPVALUE_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_ISDTCGROUPVALUE_EXIT
/** \brief Exit point of function Dem_IsDTCGroupValue() */
#define DBG_DEM_ISDTCGROUPVALUE_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_TRIGGERNVMWRITE_ENTRY
/** \brief Entry point of function Dem_TriggerNvmWrite() */
#define DBG_DEM_TRIGGERNVMWRITE_ENTRY(a, b)
#endif

#ifndef DBG_DEM_TRIGGERNVMWRITE_EXIT
/** \brief Exit point of function Dem_TriggerNvmWrite() */
#define DBG_DEM_TRIGGERNVMWRITE_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_CHECKFOROLDCLEARIMMEDIATEENTRIES_ENTRY
/** \brief Entry point of function Dem_CheckForOldClearImmediateEntries() */
#define DBG_DEM_CHECKFOROLDCLEARIMMEDIATEENTRIES_ENTRY(a)
#endif

#ifndef DBG_DEM_CHECKFOROLDCLEARIMMEDIATEENTRIES_EXIT
/** \brief Exit point of function Dem_CheckForOldClearImmediateEntries() */
#define DBG_DEM_CHECKFOROLDCLEARIMMEDIATEENTRIES_EXIT(a, b)
#endif

#ifndef DBG_DEM_CLEAREVENTS_ENTRY
/** \brief Entry point of function Dem_ClearEvents() */
#if ( (DEM_NUM_CMB_DTCS > 0U) && \
      (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON) )
#define DBG_DEM_CLEAREVENTS_ENTRY(a, b, c, d)
#elif (DEM_NUM_CMB_DTCS > 0U)
#define DBG_DEM_CLEAREVENTS_ENTRY(a, b, c)
#else
#define DBG_DEM_CLEAREVENTS_ENTRY(a, b)
#endif
#endif

#ifndef DBG_DEM_CLEAREVENTS_EXIT
/** \brief Exit point of function Dem_ClearEvents() */
#if ( (DEM_NUM_CMB_DTCS > 0U) && \
      (DEM_USE_CB_CLEAR_EVENT_ALLOWED == STD_ON) )
#define DBG_DEM_CLEAREVENTS_EXIT(a, b, c, d, e)
#elif (DEM_NUM_CMB_DTCS > 0U)
#define DBG_DEM_CLEAREVENTS_EXIT(a, b, c, d)
#else
#define DBG_DEM_CLEAREVENTS_EXIT(a, b, c)
#endif
#endif

#ifndef DBG_DEM_CLEARCMBEVENTS_ENTRY
/** \brief Entry point of function Dem_ClearCmbEvents() */
#define DBG_DEM_CLEARCMBEVENTS_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DEM_CLEARCMBEVENTS_EXIT
/** \brief Exit point of function Dem_ClearCmbEvents() */
#define DBG_DEM_CLEARCMBEVENTS_EXIT(a, b, c, d, e, f)
#endif

#ifndef DBG_DEM_CLEAREVENT_ENTRY
/** \brief Entry point of function Dem_ClearEvent() */
#define DBG_DEM_CLEAREVENT_ENTRY(a, b)
#endif

#ifndef DBG_DEM_CLEAREVENT_EXIT
/** \brief Exit point of function Dem_ClearEvent() */
#define DBG_DEM_CLEAREVENT_EXIT(a, b)
#endif

#ifndef DBG_DEM_TRIGGERCBFORCLEAREDEVENT_ENTRY
/** \brief Entry point of function Dem_TriggerCbForClearedEvent() */
#define DBG_DEM_TRIGGERCBFORCLEAREDEVENT_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_TRIGGERCBFORCLEAREDEVENT_EXIT
/** \brief Exit point of function Dem_TriggerCbForClearedEvent() */
#define DBG_DEM_TRIGGERCBFORCLEAREDEVENT_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_DISABLEDTCSETTING_ENTRY
/** \brief Entry point of function Dem_DisableDTCSetting() */
#define DBG_DEM_DISABLEDTCSETTING_ENTRY(a)
#endif

#ifndef DBG_DEM_DISABLEDTCSETTING_EXIT
/** \brief Exit point of function Dem_DisableDTCSetting() */
#define DBG_DEM_DISABLEDTCSETTING_EXIT(a, b)
#endif

#ifndef DBG_DEM_ENABLEDTCSETTING_ENTRY
/** \brief Entry point of function Dem_EnableDTCSetting() */
#define DBG_DEM_ENABLEDTCSETTING_ENTRY(a)
#endif

#ifndef DBG_DEM_ENABLEDTCSETTING_EXIT
/** \brief Exit point of function Dem_EnableDTCSetting() */
#define DBG_DEM_ENABLEDTCSETTING_EXIT(a, b)
#endif

#ifndef DBG_DEM_INITEVENTREPORTVERIFIER_ENTRY
/** \brief Entry point of function Dem_InitEventReportVerifier() */
#define DBG_DEM_INITEVENTREPORTVERIFIER_ENTRY()
#endif

#ifndef DBG_DEM_INITEVENTREPORTVERIFIER_EXIT
/** \brief Exit point of function Dem_InitEventReportVerifier() */
#define DBG_DEM_INITEVENTREPORTVERIFIER_EXIT()
#endif

#ifndef DBG_DEM_NOTIFYNEWDTCSELECTION_ENTRY
/** \brief Entry point of function Dem_NotifyNewDTCSelection() */
#define DBG_DEM_NOTIFYNEWDTCSELECTION_ENTRY(a)
#endif

#ifndef DBG_DEM_NOTIFYNEWDTCSELECTION_EXIT
/** \brief Exit point of function Dem_NotifyNewDTCSelection() */
#define DBG_DEM_NOTIFYNEWDTCSELECTION_EXIT(a)
#endif

#ifndef DBG_DEM_INITDTCMEMORYFILTER_ENTRY
/** \brief Entry point of function Dem_InitDTCMemoryFilter() */
#define DBG_DEM_INITDTCMEMORYFILTER_ENTRY()
#endif

#ifndef DBG_DEM_INITDTCMEMORYFILTER_EXIT
/** \brief Exit point of function Dem_InitDTCMemoryFilter() */
#define DBG_DEM_INITDTCMEMORYFILTER_EXIT()
#endif

#ifndef DBG_DEM_DCMSETSTOREDDTCFILTER_ENTRY
/** \brief Entry point of function Dem_DcmSetStoredDTCFilter() */
#define DBG_DEM_DCMSETSTOREDDTCFILTER_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_DCMSETSTOREDDTCFILTER_EXIT
/** \brief Exit point of function Dem_DcmSetStoredDTCFilter() */
#define DBG_DEM_DCMSETSTOREDDTCFILTER_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_DCMGETNEXTFILTEREDSTOREDDTC_ENTRY
/** \brief Entry point of function Dem_DcmGetNextFilteredStoredDTC() */
#define DBG_DEM_DCMGETNEXTFILTEREDSTOREDDTC_ENTRY(a, b)
#endif

#ifndef DBG_DEM_DCMGETNEXTFILTEREDSTOREDDTC_EXIT
/** \brief Exit point of function Dem_DcmGetNextFilteredStoredDTC() */
#define DBG_DEM_DCMGETNEXTFILTEREDSTOREDDTC_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETIDXOFDTCGROUP_ENTRY
/** \brief Entry point of function Dem_GetIdxOfDTCGroup() */
#define DBG_DEM_GETIDXOFDTCGROUP_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETIDXOFDTCGROUP_EXIT
/** \brief Exit point of function Dem_GetIdxOfDTCGroup() */
#define DBG_DEM_GETIDXOFDTCGROUP_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_DISABLEDTCRECORDUPDATE_ENTRY
/** \brief Entry point of function Dem_DisableDTCRecordUpdate() */
#define DBG_DEM_DISABLEDTCRECORDUPDATE_ENTRY(a)
#endif

#ifndef DBG_DEM_DISABLEDTCRECORDUPDATE_EXIT
/** \brief Exit point of function Dem_DisableDTCRecordUpdate() */
#define DBG_DEM_DISABLEDTCRECORDUPDATE_EXIT(a, b)
#endif

#ifndef DBG_DEM_ENABLEDTCRECORDUPDATE_ENTRY
/** \brief Entry point of function Dem_EnableDTCRecordUpdate() */
#define DBG_DEM_ENABLEDTCRECORDUPDATE_ENTRY(a)
#endif

#ifndef DBG_DEM_ENABLEDTCRECORDUPDATE_EXIT
/** \brief Exit point of function Dem_EnableDTCRecordUpdate() */
#define DBG_DEM_ENABLEDTCRECORDUPDATE_EXIT(a, b)
#endif

#ifndef DBG_DEM_GETNEXTFREEZEFRAMEDATA_ENTRY
/** \brief Entry point of function Dem_GetNextFreezeFrameData() */
#define DBG_DEM_GETNEXTFREEZEFRAMEDATA_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_GETNEXTFREEZEFRAMEDATA_EXIT
/** \brief Exit point of function Dem_GetNextFreezeFrameData() */
#define DBG_DEM_GETNEXTFREEZEFRAMEDATA_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_GETSIZEOFFREEZEFRAMESELECTION_ENTRY
/** \brief Entry point of function Dem_GetSizeOfFreezeFrameSelection() */
#define DBG_DEM_GETSIZEOFFREEZEFRAMESELECTION_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETSIZEOFFREEZEFRAMESELECTION_EXIT
/** \brief Exit point of function Dem_GetSizeOfFreezeFrameSelection() */
#define DBG_DEM_GETSIZEOFFREEZEFRAMESELECTION_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETNEXTEXTENDEDDATARECORD_ENTRY
/** \brief Entry point of function Dem_GetNextExtendedDataRecord() */
#define DBG_DEM_GETNEXTEXTENDEDDATARECORD_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_GETNEXTEXTENDEDDATARECORD_EXIT
/** \brief Exit point of function Dem_GetNextExtendedDataRecord() */
#define DBG_DEM_GETNEXTEXTENDEDDATARECORD_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_GETSIZEOFEXTENDEDDATARECORDSELECTION_ENTRY
/** \brief Entry point of function Dem_GetSizeOfExtendedDataRecordSelection() */
#define DBG_DEM_GETSIZEOFEXTENDEDDATARECORDSELECTION_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETSIZEOFEXTENDEDDATARECORDSELECTION_EXIT
/** \brief Exit point of function Dem_GetSizeOfExtendedDataRecordSelection() */
#define DBG_DEM_GETSIZEOFEXTENDEDDATARECORDSELECTION_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_COLLECTFFINFO_ENTRY
/** \brief Entry point of function Dem_CollectFFInfo() */
#define DBG_DEM_COLLECTFFINFO_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DEM_COLLECTFFINFO_EXIT
/** \brief Exit point of function Dem_CollectFFInfo() */
#define DBG_DEM_COLLECTFFINFO_EXIT(a, b, c, d, e, f)
#endif

#ifndef DBG_DEM_COPYFFINFO_ENTRY
/** \brief Entry point of function Dem_CopyFFInfo() */
#define DBG_DEM_COPYFFINFO_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_COPYFFINFO_EXIT
/** \brief Exit point of function Dem_CopyFFInfo() */
#define DBG_DEM_COPYFFINFO_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_COLLECTOBDFREEZEFRAMEINFO_ENTRY
/** \brief Entry point of function Dem_CollectOBDFreezeFrameInfo() */
#define DBG_DEM_COLLECTOBDFREEZEFRAMEINFO_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_COLLECTOBDFREEZEFRAMEINFO_EXIT
/** \brief Exit point of function Dem_CollectOBDFreezeFrameInfo() */
#define DBG_DEM_COLLECTOBDFREEZEFRAMEINFO_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_COPYOBDFREEZEFRAMEINFO_ENTRY
/** \brief Entry point of function Dem_CopyOBDFreezeFrameInfo() */
#define DBG_DEM_COPYOBDFREEZEFRAMEINFO_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_SIZEOFFFINFO_ENTRY
/** \brief Entry point of function Dem_SizeOfFFInfo() */
#define DBG_DEM_SIZEOFFFINFO_ENTRY(a,b,c)
#endif
#ifndef DBG_DEM_SIZEOFFFINFO_EXIT
/** \brief Exit point of function Dem_SizeOfFFInfo() */
#define DBG_DEM_SIZEOFFFINFO_EXIT(a,b,c,d)
#endif

#ifndef DBG_DEM_SIZEOFOBDFREEZEFRAMEINFO_ENTRY
/** \brief Entry point of function Dem_SizeOfOBDFreezeFrameInfo() */
#define DBG_DEM_SIZEOFOBDFREEZEFRAMEINFO_ENTRY(a)
#endif
#ifndef DBG_DEM_SIZEOFOBDFREEZEFRAMEINFO_EXIT
/** \brief Exit point of function Dem_SizeOfOBDFreezeFrameInfo() */
#define DBG_DEM_SIZEOFOBDFREEZEFRAMEINFO_EXIT(a,b)
#endif

#ifndef DBG_DEM_COPYOBDFREEZEFRAMEINFO_EXIT
/** \brief Exit point of function Dem_CopyOBDFreezeFrameInfo() */
#define DBG_DEM_COPYOBDFREEZEFRAMEINFO_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_SETEVENTDISABLED_ENTRY
/** \brief Entry point of function Dem_SetEventDisabled() */
#define DBG_DEM_SETEVENTDISABLED_ENTRY(a)
#endif

#ifndef DBG_DEM_SETEVENTDISABLED_EXIT
/** \brief Exit point of function Dem_SetEventDisabled() */
#define DBG_DEM_SETEVENTDISABLED_EXIT(a, b)
#endif

#ifndef DBG_DEM_REPIUMPRFAULTDETECT_ENTRY
/** \brief Entry point of function Dem_RepIUMPRFaultDetect() */
#define DBG_DEM_REPIUMPRFAULTDETECT_ENTRY(a)
#endif

#ifndef DBG_DEM_REPIUMPRFAULTDETECT_EXIT
/** \brief Exit point of function Dem_RepIUMPRFaultDetect() */
#define DBG_DEM_REPIUMPRFAULTDETECT_EXIT(a, b)
#endif

#ifndef DBG_DEM_REPIUMPRDENLOCK_ENTRY
/** \brief Entry point of function Dem_RepIUMPRDenLock() */
#define DBG_DEM_REPIUMPRDENLOCK_ENTRY(a)
#endif

#ifndef DBG_DEM_REPIUMPRDENLOCK_EXIT
/** \brief Exit point of function Dem_RepIUMPRDenLock() */
#define DBG_DEM_REPIUMPRDENLOCK_EXIT(a, b)
#endif

#ifndef DBG_DEM_REPIUMPRDENRELEASE_ENTRY
/** \brief Entry point of function Dem_RepIUMPRDenRelease() */
#define DBG_DEM_REPIUMPRDENRELEASE_ENTRY(a)
#endif

#ifndef DBG_DEM_REPIUMPRDENRELEASE_EXIT
/** \brief Exit point of function Dem_RepIUMPRDenRelease() */
#define DBG_DEM_REPIUMPRDENRELEASE_EXIT(a, b)
#endif

#ifndef DBG_DEM_DCMGETINFOTYPEVALUE08_ENTRY
/** \brief Entry point of function Dem_DcmGetInfoTypeValue08() */
#define DBG_DEM_DCMGETINFOTYPEVALUE08_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_DCMGETINFOTYPEVALUE08_EXIT
/** \brief Exit point of function Dem_DcmGetInfoTypeValue08() */
#define DBG_DEM_DCMGETINFOTYPEVALUE08_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_DCMGETINFOTYPEVALUE0B_ENTRY
/** \brief Entry point of function Dem_DcmGetInfoTypeValue0B() */
#define DBG_DEM_DCMGETINFOTYPEVALUE0B_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_DCMGETINFOTYPEVALUE0B_EXIT
/** \brief Exit point of function Dem_DcmGetInfoTypeValue0B() */
#define DBG_DEM_DCMGETINFOTYPEVALUE0B_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID01_ENTRY
/** \brief Entry point of function Dem_DcmReadDataOfPID01() */
#define DBG_DEM_DCMREADDATAOFPID01_ENTRY(a)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID01_EXIT
/** \brief Exit point of function Dem_DcmReadDataOfPID01() */
#define DBG_DEM_DCMREADDATAOFPID01_EXIT(a, b)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID21_ENTRY
/** \brief Entry point of function Dem_DcmReadDataOfPID21() */
#define DBG_DEM_DCMREADDATAOFPID21_ENTRY(a)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID21_EXIT
/** \brief Exit point of function Dem_DcmReadDataOfPID21() */
#define DBG_DEM_DCMREADDATAOFPID21_EXIT(a, b)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID30_ENTRY
/** \brief Entry point of function Dem_DcmReadDataOfPID30() */
#define DBG_DEM_DCMREADDATAOFPID30_ENTRY(a)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID30_EXIT
/** \brief Exit point of function Dem_DcmReadDataOfPID30() */
#define DBG_DEM_DCMREADDATAOFPID30_EXIT(a, b)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID31_ENTRY
/** \brief Entry point of function Dem_DcmReadDataOfPID31() */
#define DBG_DEM_DCMREADDATAOFPID31_ENTRY(a)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID31_EXIT
/** \brief Exit point of function Dem_DcmReadDataOfPID31() */
#define DBG_DEM_DCMREADDATAOFPID31_EXIT(a, b)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID41_ENTRY
/** \brief Entry point of function Dem_DcmReadDataOfPID41() */
#define DBG_DEM_DCMREADDATAOFPID41_ENTRY(a)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID41_EXIT
/** \brief Exit point of function Dem_DcmReadDataOfPID41() */
#define DBG_DEM_DCMREADDATAOFPID41_EXIT(a, b)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID4D_ENTRY
/** \brief Entry point of function Dem_DcmReadDataOfPID4D() */
#define DBG_DEM_DCMREADDATAOFPID4D_ENTRY(a)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID4D_EXIT
/** \brief Exit point of function Dem_DcmReadDataOfPID4D() */
#define DBG_DEM_DCMREADDATAOFPID4D_EXIT(a, b)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID4E_ENTRY
/** \brief Entry point of function Dem_DcmReadDataOfPID4E() */
#define DBG_DEM_DCMREADDATAOFPID4E_ENTRY(a)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID4E_EXIT
/** \brief Exit point of function Dem_DcmReadDataOfPID4E() */
#define DBG_DEM_DCMREADDATAOFPID4E_EXIT(a, b)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID1C_ENTRY
/** \brief Entry point of function Dem_DcmReadDataOfPID1C() */
#define DBG_DEM_DCMREADDATAOFPID1C_ENTRY(a)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID1C_EXIT
/** \brief Exit point of function Dem_DcmReadDataOfPID1C() */
#define DBG_DEM_DCMREADDATAOFPID1C_EXIT(a, b)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID91_ENTRY
/** \brief Entry point of function Dem_DcmReadDataOfPID91() */
#define DBG_DEM_DCMREADDATAOFPID91_ENTRY(a)
#endif

#ifndef DBG_DEM_DCMREADDATAOFPID91_EXIT
/** \brief Exit point of function Dem_DcmReadDataOfPID91() */
#define DBG_DEM_DCMREADDATAOFPID91_EXIT(a, b)
#endif

#ifndef DBG_DEM_DCMREADDATAOFOBDFREEZEFRAME_ENTRY
/** \brief Entry point of function Dem_DcmReadDataOfOBDFreezeFrame() */
#define DBG_DEM_DCMREADDATAOFOBDFREEZEFRAME_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_DCMREADDATAOFOBDFREEZEFRAME_EXIT
/** \brief Exit point of function Dem_DcmReadDataOfOBDFreezeFrame() */
#define DBG_DEM_DCMREADDATAOFOBDFREEZEFRAME_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_DCMGETDTCOFOBDFREEZEFRAME_ENTRY
/** \brief Entry point of function Dem_DcmGetDTCOfOBDFreezeFrame() */
#define DBG_DEM_DCMGETDTCOFOBDFREEZEFRAME_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_DCMGETDTCOFOBDFREEZEFRAME_EXIT
/** \brief Exit point of function Dem_DcmGetDTCOfOBDFreezeFrame() */
#define DBG_DEM_DCMGETDTCOFOBDFREEZEFRAME_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_DCMGETAVAILABLEOBDIDS_ENTRY
/** \brief Entry point of function Dem_DcmGetAvailableOBDMIDs() */
#define DBG_DEM_DCMGETAVAILABLEOBDIDS_ENTRY(a, b)
#endif

#ifndef DBG_DEM_DCMGETAVAILABLEOBDIDS_EXIT
/** \brief Exit point of function Dem_DcmGetAvailableOBDMIDs() */
#define DBG_DEM_DCMGETAVAILABLEOBDIDS_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_DCMGETNUMTIDSOFOBDMID_ENTRY
/** \brief Entry point of function Dem_DcmGetNumTIDsOfOBDMID() */
#define DBG_DEM_DCMGETNUMTIDSOFOBDMID_ENTRY(a, b)
#endif

#ifndef DBG_DEM_DCMGETNUMTIDSOFOBDMID_EXIT
/** \brief Exit point of function Dem_DcmGetNumTIDsOfOBDMID() */
#define DBG_DEM_DCMGETNUMTIDSOFOBDMID_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_DCMGETDTRDATA_ENTRY
/** \brief Entry point of function Dem_DcmGetDTRData() */
#define DBG_DEM_DCMGETDTRDATA_ENTRY(a, b, c, d, e, f, g)
#endif

#ifndef DBG_DEM_DCMGETDTRDATA_EXIT
/** \brief Exit point of function Dem_DcmGetDTRData() */
#define DBG_DEM_DCMGETDTRDATA_EXIT(a, b, c, d, e, f, g, h)
#endif

#ifndef DBG_DEM_SETPTOSTATUS_ENTRY
/** \brief Entry point of function Dem_SetPtoStatus() */
#define DBG_DEM_SETPTOSTATUS_ENTRY(a)
#endif

#ifndef DBG_DEM_SETPTOSTATUS_EXIT
/** \brief Exit point of function Dem_SetPtoStatus() */
#define DBG_DEM_SETPTOSTATUS_EXIT(a, b)
#endif

#ifndef DBG_DEM_SETPFCCYCLEQUALIFIED_ENTRY
/** \brief Entry point of function Dem_SetPfcCycleQualified() */
#define DBG_DEM_SETPFCCYCLEQUALIFIED_ENTRY()
#endif

#ifndef DBG_DEM_SETPFCCYCLEQUALIFIED_EXIT
/** \brief Exit point of function Dem_SetPfcCycleQualified() */
#define DBG_DEM_SETPFCCYCLEQUALIFIED_EXIT(a)
#endif

#ifndef DBG_DEM_GETPFCCYCLEQUALIFIED_ENTRY
/** \brief Entry point of function Dem_GetPfcCycleQualified() */
#define DBG_DEM_GETPFCCYCLEQUALIFIED_ENTRY(a)
#endif

#ifndef DBG_DEM_GETPFCCYCLEQUALIFIED_EXIT
/** \brief Exit point of function Dem_GetPfcCycleQualified() */
#define DBG_DEM_GETPFCCYCLEQUALIFIED_EXIT(a,b)
#endif

#ifndef DBG_DEM_RESETREADINESS_ENTRY
/** \brief Entry point of function Dem_ResetReadiness() */
#define DBG_DEM_RESETREADINESS_ENTRY()
#endif

#ifndef DBG_DEM_RESETREADINESS_EXIT
/** \brief Exit point of function Dem_ResetReadiness() */
#define DBG_DEM_RESETREADINESS_EXIT()
#endif

#ifndef DBG_DEM_DEM_UPDATEPID01VALUE_ENTRY
/** \brief Entry point of function Dem_UpdatePID01value() */
#define DBG_DEM_DEM_UPDATEPID01VALUE_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_DEM_UPDATEPID01VALUE_EXIT
/** \brief Exit point of function Dem_UpdatePID01value() */
#define DBG_DEM_DEM_UPDATEPID01VALUE_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_SETDTCFILTER_ENTRY
/** \brief Entry point of function Dem_SetDTCFilter() */
#define DBG_DEM_SETDTCFILTER_ENTRY(a, b, c, d, e, f, g)
#endif

#ifndef DBG_DEM_SETDTCFILTER_EXIT
/** \brief Exit point of function Dem_SetDTCFilter() */
#define DBG_DEM_SETDTCFILTER_EXIT(a, b, c, d, e, f, g, h)
#endif

#ifndef DBG_DEM_SETFREEZEFRAMERECORDFILTER_ENTRY
/** \brief Entry point of function Dem_SetFreezeFrameRecordFilter() */
#define DBG_DEM_SETFREEZEFRAMERECORDFILTER_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_SETFREEZEFRAMERECORDFILTER_EXIT
/** \brief Exit point of function Dem_SetFreezeFrameRecordFilter() */
#define DBG_DEM_SETFREEZEFRAMERECORDFILTER_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_GETSTATUSOFDTC_ENTRY
/** \brief Entry point of function Dem_GetStatusOfDTC() */
#define DBG_DEM_GETSTATUSOFDTC_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETSTATUSOFDTC_EXIT
/** \brief Exit point of function Dem_GetStatusOfDTC() */
#define DBG_DEM_GETSTATUSOFDTC_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETDTCSTATUSAVAILABILITYMASK_ENTRY
/** \brief Entry point of function Dem_GetDTCStatusAvailabilityMask() */
#define DBG_DEM_GETDTCSTATUSAVAILABILITYMASK_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETDTCSTATUSAVAILABILITYMASK_EXIT
/** \brief Exit point of function Dem_GetDTCStatusAvailabilityMask() */
#define DBG_DEM_GETDTCSTATUSAVAILABILITYMASK_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETNUMBEROFFILTEREDDTC_ENTRY
/** \brief Entry point of function Dem_GetNumberOfFilteredDTC() */
#define DBG_DEM_GETNUMBEROFFILTEREDDTC_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETNUMBEROFFILTEREDDTC_EXIT
/** \brief Exit point of function Dem_GetNumberOfFilteredDTC() */
#define DBG_DEM_GETNUMBEROFFILTEREDDTC_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETNEXTFILTEREDDTC_ENTRY
/** \brief Entry point of function Dem_GetNextFilteredDTC() */
#define DBG_DEM_GETNEXTFILTEREDDTC_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_GETNEXTFILTEREDDTC_EXIT
/** \brief Exit point of function Dem_GetNextFilteredDTC() */
#define DBG_DEM_GETNEXTFILTEREDDTC_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_GETDTCBYOCCURRENCETIME_ENTRY
/** \brief Entry point of function Dem_GetDTCByOccurrenceTime() */
#define DBG_DEM_GETDTCBYOCCURRENCETIME_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_GETDTCBYOCCURRENCETIME_EXIT
/** \brief Exit point of function Dem_GetDTCByOccurrenceTime() */
#define DBG_DEM_GETDTCBYOCCURRENCETIME_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_GETNEXTFILTEREDRECORD_ENTRY
/** \brief Entry point of function Dem_GetNextFilteredRecord() */
#define DBG_DEM_GETNEXTFILTEREDRECORD_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_GETNEXTFILTEREDRECORD_EXIT
/** \brief Exit point of function Dem_GetNextFilteredRecord() */
#define DBG_DEM_GETNEXTFILTEREDRECORD_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_GETNEXTFILTEREDDTCANDFDC_ENTRY
/** \brief Entry point of function Dem_GetNextFilteredDTCAndFDC() */
#define DBG_DEM_GETNEXTFILTEREDDTCANDFDC_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_GETNEXTFILTEREDDTCANDFDC_EXIT
/** \brief Exit point of function Dem_GetNextFilteredDTCAndFDC() */
#define DBG_DEM_GETNEXTFILTEREDDTCANDFDC_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_GETNEXTFILTEREDDTCANDSEVERITY_ENTRY
/** \brief Entry point of function Dem_GetNextFilteredDTCAndSeverity() */
#define DBG_DEM_GETNEXTFILTEREDDTCANDSEVERITY_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DEM_GETNEXTFILTEREDDTCANDSEVERITY_EXIT
/** \brief Exit point of function Dem_GetNextFilteredDTCAndSeverity() */
#define DBG_DEM_GETNEXTFILTEREDDTCANDSEVERITY_EXIT(a, b, c, d, e, f)
#endif

#ifndef DBG_DEM_GETTRANSLATIONTYPE_ENTRY
/** \brief Entry point of function Dem_GetTranslationType() */
#define DBG_DEM_GETTRANSLATIONTYPE_ENTRY(a)
#endif

#ifndef DBG_DEM_GETTRANSLATIONTYPE_EXIT
/** \brief Exit point of function Dem_GetTranslationType() */
#define DBG_DEM_GETTRANSLATIONTYPE_EXIT(a, b)
#endif

#ifndef DBG_DEM_GETSEVERITYOFDTC_ENTRY
/** \brief Entry point of function Dem_GetSeverityOfDTC() */
#define DBG_DEM_GETSEVERITYOFDTC_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETSEVERITYOFDTC_EXIT
/** \brief Exit point of function Dem_GetSeverityOfDTC() */
#define DBG_DEM_GETSEVERITYOFDTC_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETFUNCTIONALUNITOFDTC_ENTRY
/** \brief Entry point of function Dem_GetFunctionalUnitOfDTC() */
#define DBG_DEM_GETFUNCTIONALUNITOFDTC_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETFUNCTIONALUNITOFDTC_EXIT
/** \brief Exit point of function Dem_GetFunctionalUnitOfDTC() */
#define DBG_DEM_GETFUNCTIONALUNITOFDTC_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_APPLYDTCFILTER_ENTRY
/** \brief Entry point of function Dem_ApplyDTCFilter() */
#define DBG_DEM_APPLYDTCFILTER_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DEM_APPLYDTCFILTER_EXIT
/** \brief Exit point of function Dem_ApplyDTCFilter() */
#define DBG_DEM_APPLYDTCFILTER_EXIT(a, b, c, d, e, f)
#endif

#ifndef DBG_DEM_DEBOUNCEEVENT_ENTRY
/** \brief Entry point of function Dem_DebounceEvent() */
#define DBG_DEM_DEBOUNCEEVENT_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_DEBOUNCEEVENT_EXIT
/** \brief Exit point of function Dem_DebounceEvent() */
#define DBG_DEM_DEBOUNCEEVENT_EXIT(a, b, c, d)
#endif

#ifndef DBG_DEM_INTERNALSETSWCEVENTSTATUS_ENTRY
/** \brief Entry point of function Dem_InternalSetSwcEventStatus() */
#define DBG_DEM_INTERNALSETSWCEVENTSTATUS_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_INTERNALSETSWCEVENTSTATUS_EXIT
/** \brief Exit point of function Dem_InternalSetSwcEventStatus() */
#define DBG_DEM_INTERNALSETSWCEVENTSTATUS_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_INTERNALSETBSWEVENTSTATUS_ENTRY
/** \brief Entry point of function Dem_InternalSetSwcEventStatus() */
#define DBG_DEM_INTERNALSETBSWEVENTSTATUS_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_INTERNALSETBSWEVENTSTATUS_EXIT
/** \brief Exit point of function Dem_InternalSetSwcEventStatus() */
#define DBG_DEM_INTERNALSETBSWEVENTSTATUS_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_APPLY_DTC_ORDER_ENTRY
/** \brief Entry point of function Dem_ApplyDTCOrder() */
#define DBG_DEM_APPLY_DTC_ORDER_ENTRY(a, b)
#endif

#ifndef DBG_DEM_APPLY_DTC_ORDER_EXIT
/** \brief Exit point of function Dem_ApplyDTCOrder() */
#define DBG_DEM_APPLY_DTC_ORDER_EXIT(a, b)
#endif

#ifndef DBG_DEM_UPDATEEVENTIDOCCURKINDS_ENTRY
/** \brief Entry point of function  Dem_UpdateEventIdOccurKinds() */
#define DBG_DEM_UPDATEEVENTIDOCCURKINDS_ENTRY(a, b, c)
#endif

#ifndef DBG_DEM_UPDATEEVENTIDOCCURKINDS_EXIT
/** \brief Exit point of function  Dem_UpdateEventIdOccurKinds() */
#define DBG_DEM_UPDATEEVENTIDOCCURKINDS_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_RESETEVENTIDOCCURKINDS_ENTRY
/** \brief Entry point of function Dem_ResetEventIdOccurKinds() */
#define DBG_DEM_RESETEVENTIDOCCURKINDS_ENTRY(a)
#endif

#ifndef DBG_DEM_RESETEVENTIDOCCURKINDS_EXIT
/** \brief Exit point of function Dem_ResetEventIdOccurKinds() */
#define DBG_DEM_RESETEVENTIDOCCURKINDS_EXIT(a)
#endif

#ifndef DBG_DEM_UPDATECMBDTCSTATUS_ENTRY
/** \brief Entry point of function Dem_UpdateCmbDTCStatus() */
#define DBG_DEM_UPDATECMBDTCSTATUS_ENTRY(a)
#endif

#ifndef DBG_DEM_UPDATECMBDTCSTATUS_EXIT
/** \brief Exit point of function Dem_UpdateCmbDTCStatus() */
#define DBG_DEM_UPDATECMBDTCSTATUS_EXIT(a)
#endif

#ifndef DBG_DEM_GBIDTCCONFIDX_ENTRY
/** \brief Entry point of function Dem_GbiDTCConfIdx() */
#define DBG_DEM_GBIDTCCONFIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIDTCCONFIDX_EXIT
/** \brief Exit point of function Dem_GbiDTCConfIdx() */
#define DBG_DEM_GBIDTCCONFIDX_EXIT(a,b)
#endif

#ifndef DBG_DEM_GBIUDSDTC_ENTRY
/** \brief Entry point of function Dem_GbiUdsDTC() */
#define DBG_DEM_GBIUDSDTC_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIUDSDTC_EXIT
/** \brief Exit point of function Dem_GbiUdsDTC() */
#define DBG_DEM_GBIUDSDTC_EXIT(a,b)
#endif

#ifndef DBG_DEM_GETFDCOFCMBDTC_ENTRY
/** \brief Entry point of function Dem_GetFDCOfCmbDTC() */
#define DBG_DEM_GETFDCOFCMBDTC_ENTRY(a,b)
#endif

#ifndef DBG_DEM_GETFDCOFCMBDTC_EXIT
/** \brief Exit point of function Dem_GetFDCOfCmbDTC() */
#define DBG_DEM_GETFDCOFCMBDTC_EXIT(a,b,c)
#endif

#ifndef DBG_DEM_RESETSTATUSFORCMBEVENTS_ENTRY
/** \brief Entry point of function Dem_ResetStatusForCmbEvents() */
#define DBG_DEM_RESETSTATUSFORCMBEVENTS_ENTRY(a,b)
#endif

#ifndef DBG_DEM_RESETSTATUSFORCMBEVENTS_EXIT
/** \brief Exit point of function Dem_ResetStatusForCmbEvents() */
#define DBG_DEM_RESETSTATUSFORCMBEVENTS_EXIT(a,b)
#endif

#ifndef DBG_DEM_INITFAILURECTROFCMBEVENTS_ENTRY
/** \brief Entry point of function Dem_InitFailureCounterOfCmbEvents() */
#define DBG_DEM_INITFAILURECTROFCMBEVENTS_ENTRY(a)
#endif

#ifndef DBG_DEM_INITFAILURECTROFCMBEVENTS_EXIT
/** \brief Exit point of function Dem_InitFailureCounterOfCmbEvents() */
#define DBG_DEM_INITFAILURECTROFCMBEVENTS_EXIT(a)
#endif

#ifndef DBG_DEM_INITALLCMBDTCSTATUS_ENTRY
/** \brief Entry point of function Dem_InitAllCmbDTCStatus() */
#define DBG_DEM_INITALLCMBDTCSTATUS_ENTRY()
#endif

#ifndef DBG_DEM_INITALLCMBDTCSTATUS_EXIT
/** \brief Exit point of function Dem_InitAllCmbDTCStatus() */
#define DBG_DEM_INITALLCMBDTCSTATUS_EXIT()
#endif

#ifndef DBG_DEM_GETFAILURECOUNTERIDXOFCMBEVENT_ENTRY
/** \brief Entry point of function Dem_GetFailureCounterIdxOfCmbEvent() */
#define DBG_DEM_GETFAILURECOUNTERIDXOFCMBEVENT_ENTRY(a)
#endif

#ifndef DBG_DEM_GETFAILURECOUNTERIDXOFCMBEVENT_EXIT
/** \brief Exit point of function Dem_GetFailureCounterIdxOfCmbEvent() */
#define DBG_DEM_GETFAILURECOUNTERIDXOFCMBEVENT_EXIT(a, b)
#endif

#ifndef DBG_DEM_CHECKCONFIRMEDDRIVINGCYCLE_ENTRY
/** \brief Entry point of function Dem_CheckConfirmedDrivingCycle() */
#define DBG_DEM_CHECKCONFIRMEDDRIVINGCYCLE_ENTRY(a)
#endif

#ifndef DBG_DEM_CHECKCONFIRMEDDRIVINGCYCLE_EXIT
/** \brief Exit point of function Dem_CheckConfirmedDrivingCycle() */
#define DBG_DEM_CHECKCONFIRMEDDRIVINGCYCLE_EXIT(a, b)
#endif

#ifndef DBG_DEM_CBTRIGGERONDTCSTATUS_ENTRY
/** \brief Entry point of function Dem_CbTriggerOnDTCStatus() */
#define DBG_DEM_CBTRIGGERONDTCSTATUS_ENTRY(a,b,c)
#endif

#ifndef DBG_DEM_CBTRIGGERONDTCSTATUS_EXIT
/** \brief Exit point of function Dem_CbTriggerOnDTCStatus() */
#define DBG_DEM_CBTRIGGERONDTCSTATUS_EXIT(a,b,c)
#endif

#ifndef DBG_DEM_SETWIRSTATUS_ENTRY
/** \brief Entry point of function Dem_SetWIRStatus() */
#define DBG_DEM_SETWIRSTATUS_ENTRY(a,b)
#endif

#ifndef DBG_DEM_SETWIRSTATUS_EXIT
/** \brief Exit point of function Dem_SetWIRStatus() */
#define DBG_DEM_SETWIRSTATUS_EXIT(a,b,c)
#endif

#ifndef DBG_DEM_RECEIVEBSWEVENTREPORTSONMASTER_ENTRY
/** \brief Entry point of function Dem_ReceiveBswEventReportsOnMaster() */
#define DBG_DEM_RECEIVEBSWEVENTREPORTSONMASTER_ENTRY()
#endif

#ifndef DBG_DEM_RECEIVEBSWEVENTREPORTSONMASTER_EXIT
/** \brief Exit point of function Dem_ReceiveBswEventReportsOnMaster() */
#define DBG_DEM_RECEIVEBSWEVENTREPORTSONMASTER_EXIT()
#endif

#ifndef DBG_DEM_REPORTERRORSTATUSPREINIT_ENTRY
/** \brief Entry point of function Dem_ReportErrorStatusPreInit() */
#define DBG_DEM_REPORTERRORSTATUSPREINIT_ENTRY(a,b)
#endif

#ifndef DBG_DEM_REPORTERRORSTATUSPREINIT_EXIT
/** \brief Exit point of function Dem_ReportErrorStatusPreInit() */
#define DBG_DEM_REPORTERRORSTATUSPREINIT_EXIT(a,b)
#endif

#ifndef DBG_DEM_REPORTERRORSTATUSCORE_ENTRY
/** \brief Entry point of function Dem_ReportErrorStatusCore() */
#define DBG_DEM_REPORTERRORSTATUSCORE_ENTRY(a,b,c)
#endif

#ifndef DBG_DEM_REPORTERRORSTATUSCORE_EXIT
/** \brief Exit point of function Dem_ReportErrorStatusCore() */
#define DBG_DEM_REPORTERRORSTATUSCORE_EXIT(a,b,c)
#endif

#ifndef DBG_DEM_SELECTDTC_ENTRY
/** \brief Entry point of function Dem_SelectDTC() */
#define DBG_DEM_SELECTDTC_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DEM_SELECTDTC_EXIT
/** \brief Exit point of function Dem_SelectDTC() */
#define DBG_DEM_SELECTDTC_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_GETDTCSELECTIONRESULT_ENTRY
/** \brief Entry point of function Dem_GetDTCSelectionResult() */
#define DBG_DEM_GETDTCSELECTIONRESULT_ENTRY(a)
#endif

#ifndef DBG_DEM_GETDTCSELECTIONRESULT_EXIT
/** \brief Exit point of function Dem_GetDTCSelectionResult() */
#define DBG_DEM_GETDTCSELECTIONRESULT_EXIT(a, b)
#endif

#ifndef DBG_DEM_GETDTCSELECTIONRESULTFORCLEARDTC_ENTRY
/** \brief Entry point of function Dem_GetDTCSelectionResultForClearDTC() */
#define DBG_DEM_GETDTCSELECTIONRESULTFORCLEARDTC_ENTRY(a)
#endif

#ifndef DBG_DEM_GETDTCSELECTIONRESULTFORCLEARDTC_EXIT
/** \brief Exit point of function Dem_GetDTCSelectionResultForClearDTC() */
#define DBG_DEM_GETDTCSELECTIONRESULTFORCLEARDTC_EXIT(a, b)
#endif

#ifndef DBG_DEM_INTERNALGETDTCSELECTIONRESULT_ENTRY
/** \brief Entry point of function Dem_InternalGetDTCSelectionResult() */
#define DBG_DEM_INTERNALGETDTCSELECTIONRESULT_ENTRY(a, b)
#endif

#ifndef DBG_DEM_INTERNALGETDTCSELECTIONRESULT_EXIT
/** \brief Exit point of function Dem_InternalGetDTCSelectionResult() */
#define DBG_DEM_INTERNALGETDTCSELECTIONRESULT_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_SETDTCSELECTIONCRITERIA_ENTRY
/** \brief Entry point of function Dem_SetDTCSelectionCriteria() */
#define DBG_DEM_SETDTCSELECTIONCRITERIA_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DEM_SETDTCSELECTIONCRITERIA_EXIT
/** \brief Exit point of function Dem_SetDTCSelectionCriteria() */
#define DBG_DEM_SETDTCSELECTIONCRITERIA_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DEM_DODTCSELECTION_ENTRY
/** \brief Entry point of function Dem_DoDTCSelection() */
#define DBG_DEM_DODTCSELECTION_ENTRY(a, b)
#endif

#ifndef DBG_DEM_DODTCSELECTION_EXIT
/** \brief Exit point of function Dem_DoDTCSelection() */
#define DBG_DEM_DODTCSELECTION_EXIT(a, b, c)
#endif

#ifndef DBG_DEM_GETDTCSELECTION_ENTRY
/** \brief Entry point of function Dem_GetDTCSelection() */
#define DBG_DEM_GETDTCSELECTION_ENTRY(a, b)
#endif

#ifndef DBG_DEM_GETDTCSELECTION_EXIT
/** \brief Exit point of function Dem_GetDTCSelection() */
#define DBG_DEM_GETDTCSELECTION_EXIT(a, b)
#endif

#ifndef DBG_DEM_INITDTCSELECTOR_ENTRY
/** \brief Entry point of function Dem_InitDTCSelector() */
#define DBG_DEM_INITDTCSELECTOR_ENTRY()
#endif

#ifndef DBG_DEM_INITDTCSELECTOR_EXIT
/** \brief Exit point of function Dem_InitDTCSelector() */
#define DBG_DEM_INITDTCSELECTOR_EXIT()
#endif

#ifndef DBG_DEM_INITEVENTMEMORYPROCESSOR_ENTRY
/** \brief Entry point of function Dem_InitEventMemoryProcessor() */
#define DBG_DEM_INITEVENTMEMORYPROCESSOR_ENTRY()
#endif

#ifndef DBG_DEM_INITEVENTMEMORYPROCESSOR_EXIT
/** \brief Exit point of function Dem_InitEventMemoryProcessor() */
#define DBG_DEM_INITEVENTMEMORYPROCESSOR_EXIT()
#endif

#ifndef DBG_DEM_GETMAPPEDDTCORIGIN_ENTRY
/** \brief Entry point of function Dem_GetMappedDTCOrigin() */
#define DBG_DEM_GETMAPPEDDTCORIGIN_ENTRY(a,b)
#endif

#ifndef DBG_DEM_GETMAPPEDDTCORIGIN_EXIT
/** \brief Exit point of function Dem_GetMappedDTCOrigin() */
#define DBG_DEM_GETMAPPEDDTCORIGIN_EXIT(a,b,c)
#endif

#ifndef DBG_DEM_J1939DCMSETDTCFILTER_ENTER
/** \brief Entry point of function Dem_J1939DcmSetDTCFilter() */
#define DBG_DEM_J1939DCMSETDTCFILTER_ENTER(a,b,c,d)
#endif

#ifndef DBG_DEM_J1939DCMSETDTCFILTER_EXIT
/** \brief Exit point of function Dem_J1939DcmSetDTCFilter() */
#define DBG_DEM_J1939DCMSETDTCFILTER_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_DEM_J1939DCMGETNUMBEROFFILTEREDDTC_ENTER
/** \brief Entry point of function Dem_J1939DcmGetNumberOfFilteredDTC() */
#define DBG_DEM_J1939DCMGETNUMBEROFFILTEREDDTC_ENTER(a)
#endif

#ifndef DBG_DEM_J1939DCMGETNUMBEROFFILTEREDDTC_EXIT
/** \brief Exit point of function Dem_J1939DcmGetNumberOfFilteredDTC() */
#define DBG_DEM_J1939DCMGETNUMBEROFFILTEREDDTC_EXIT(a,b)
#endif

#ifndef DBG_DEM_J1939DCMGETNEXTFILTEREDDTC_ENTER
/** \brief Entry point of function Dem_J1939DcmGetNextFilteredDTC() */
#define DBG_DEM_J1939DCMGETNEXTFILTEREDDTC_ENTER(a,b)
#endif

#ifndef DBG_DEM_J1939DCMGETNEXTFILTEREDDTC_EXIT
/** \brief Exit point of function Dem_J1939DcmGetNextFilteredDTC() */
#define DBG_DEM_J1939DCMGETNEXTFILTEREDDTC_EXIT(a,b,c)
#endif

#ifndef DBG_DEM_J1939DCMCLEARDTC_ENTRY
/** \brief Entry point of function Dem_J1939DcmClearDTC() */
#define DBG_DEM_J1939DCMCLEARDTC_ENTRY(a,b)
#endif

#ifndef DBG_DEM_J1939DCMCLEARDTC_EXIT
/** \brief Exit point of function Dem_J1939DcmClearDTC() */
#define DBG_DEM_J1939DCMCLEARDTC_EXIT(a,b,c)
#endif

#ifndef DBG_DEM_J1939DCMSTARTCLEARDTC_ENTRY
/** \brief Entry point of function Dem_J1939DcmStartClearDTC() */
#define DBG_DEM_J1939DCMSTARTCLEARDTC_ENTRY(a,b)
#endif

#ifndef DBG_DEM_J1939DCMSTARTCLEARDTC_EXIT
/** \brief Exit point of function Dem_J1939DcmStartClearDTC() */
#define DBG_DEM_J1939DCMSTARTCLEARDTC_EXIT(a,b,c)
#endif

#ifndef DBG_DEM_J1939INTERNALGETLAMPSTATUS_ENTER
/** \brief Entry point of function Dem_J1939InternalGetLampStatus() */
#define DBG_DEM_J1939INTERNALGETLAMPSTATUS_ENTER(a)
#endif

#ifndef DBG_DEM_J1939INTERNALGETLAMPSTATUS_EXIT
/** \brief Exit point of function Dem_J1939InternalGetLampStatus() */
#define DBG_DEM_J1939INTERNALGETLAMPSTATUS_EXIT(a,b)
#endif

#ifndef DBG_DEM_J1939INTERNALGETMILSTATUS_ENTER
/** \brief Entry point of function Dem_J1939InternalGetMilStatus() */
#define DBG_DEM_J1939INTERNALGETMILSTATUS_ENTER()
#endif

#ifndef DBG_DEM_J1939INTERNALGETMILSTATUS_EXIT
/** \brief Exit point of function Dem_J1939InternalGetMilStatus() */
#define DBG_DEM_J1939INTERNALGETMILSTATUS_EXIT(a)
#endif


#ifndef DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS1_ENTRY
/** \brief Entry point of function Dem_J1939DcmReadDiagnosticReadiness1() */
#define DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS1_ENTRY(a,b)
#endif

#ifndef DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS1_EXIT
/** \brief Exit point of function Dem_J1939DcmReadDiagnosticReadiness1() */
#define DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS1_EXIT(a,b,c)
#endif


#ifndef DBG_DEM_GETVISIBLEJ1939DTC_ENTRY
/** \brief Entry point of function Dem_GetVisibleJ1939DTC() */
#define DBG_DEM_GETVISIBLEJ1939DTC_ENTRY(a)
#endif

#ifndef DBG_DEM_GETVISIBLEJ1939DTC_EXIT
/** \brief Exit point of function Dem_GetVisibleJ1939DTC() */
#define DBG_DEM_GETVISIBLEJ1939DTC_EXIT(a,b)
#endif

#ifndef DBG_DEM_UPDATE_J1939_READINESS_VALUE_ENTRY
/** \brief Entry point of function Dem_Update_J1939_Readiness_value() */
#define DBG_DEM_UPDATE_J1939_READINESS_VALUE_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DEM_UPDATE_J1939_READINESS_VALUE_EXIT
/** \brief Exit point of function Dem_Update_J1939_Readiness_value() */
#define DBG_DEM_UPDATE_J1939_READINESS_VALUE_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_DEM_SIZEOFJ1939FFSPN_ENTRY
/** \brief Entry point of function Dem_SizeOfJ1939FFSPN() */
#define DBG_DEM_SIZEOFJ1939FFSPN_ENTRY(a)
#endif

#ifndef DBG_DEM_SIZEOFJ1939FFSPN_EXIT
/** \brief Exit point of function Dem_SizeOfJ1939FFSPN() */
#define DBG_DEM_SIZEOFJ1939FFSPN_EXIT(a,b)
#endif

#ifndef DBG_DEM_SIZEOFJ1939FF_ENTRY
/** \brief Entry point of function Dem_SizeOfJ1939FF() */
#define DBG_DEM_SIZEOFJ1939FF_ENTRY(a)
#endif

#ifndef DBG_DEM_SIZEOFJ1939FF_EXIT
/** \brief Exit point of function Dem_SizeOfJ1939FF() */
#define DBG_DEM_SIZEOFJ1939FF_EXIT(a,b)
#endif

#ifndef DBG_DEM_GBIJ1939FFCLASSIDX_ENTRY
/** \brief Entry point of function Dem_GbiJ1939FFClassIdx() */
#define DBG_DEM_GBIJ1939FFCLASSIDX_ENTRY(a)
#endif

#ifndef DBG_DEM_GBIJ1939FFCLASSIDX_EXIT
/** \brief Exit point of function Dem_GbiJ1939FFClassIdx() */
#define DBG_DEM_GBIJ1939FFCLASSIDX_EXIT(a,b)
#endif

#ifndef DBG_DEM_J1939FFENTRYDATA_ENTRY
/** \brief Entry point of function Dem_J1939FFEntryData() */
#define DBG_DEM_J1939FFENTRYDATA_ENTRY(a)
#endif

#ifndef DBG_DEM_J1939FFENTRYDATA_EXIT
/** \brief Exit point of function Dem_J1939FFEntryData() */
#define DBG_DEM_J1939FFENTRYDATA_EXIT(a,b)
#endif

#ifndef DBG_DEM_STOREJ1939FF_ENTRY
/** \brief Entry point of function Dem_StoreJ1939FF() */
#define DBG_DEM_STOREJ1939FF_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DEM_STOREJ1939FF_EXIT
/** \brief Exit point of function Dem_StoreJ1939FF() */
#define DBG_DEM_STOREJ1939FF_EXIT(a,b,c,d)
#endif

#ifndef DBG_DEM_PROCESSEVENTENTRYJ1939FFDATA_ENTRY
/** \brief Entry point of function Dem_ProcessEventEntryJ1939FFData() */
#define DBG_DEM_PROCESSEVENTENTRYJ1939FFDATA_ENTRY(a,b)
#endif

#ifndef DBG_DEM_PROCESSEVENTENTRYJ1939FFDATA_EXIT
/** \brief Exit point of function Dem_ProcessEventEntryJ1939FFData() */
#define DBG_DEM_PROCESSEVENTENTRYJ1939FFDATA_EXIT(a,b,c)
#endif

#ifndef DBG_DEM_COPYJ1939FFINFO_ENTRY
/** \brief Entry point of function Dem_CopyJ1939FFInfo() */
#define DBG_DEM_COPYJ1939FFINFO_ENTRY(a,b,c)
#endif

#ifndef DBG_DEM_COPYJ1939FFINFO_EXIT
/** \brief Exit point of function Dem_CopyJ1939FFInfo() */
#define DBG_DEM_COPYJ1939FFINFO_EXIT(a,b,c)
#endif

#ifndef DBG_DEM_COLLECTJ1939FFINFO_ENTRY
/** \brief Entry point of function Dem_CollectJ1939FFInfo() */
#define DBG_DEM_COLLECTJ1939FFINFO_ENTRY(a,b,c)
#endif

#ifndef DBG_DEM_COLLECTJ1939FFINFO_EXIT
/** \brief Exit point of function Dem_CollectJ1939FFInfo() */
#define DBG_DEM_COLLECTJ1939FFINFO_EXIT(a,b,c,d)
#endif

#ifndef DBG_DEM_J1939DCMSETFREEZEFRAMEFILTER_ENTRY
/** \brief Entry point of function Dem_J1939DcmSetFreezeFrameFilter() */
#define DBG_DEM_J1939DCMSETFREEZEFRAMEFILTER_ENTRY(a,b)
#endif

#ifndef DBG_DEM_J1939DCMSETFREEZEFRAMEFILTER_EXIT
/** \brief Exit point of function Dem_J1939DcmSetFreezeFrameFilter() */
#define DBG_DEM_J1939DCMSETFREEZEFRAMEFILTER_EXIT(a,b,c)
#endif

#ifndef DBG_DEM_J1939DCMGETNEXTFREEZEFRAME_ENTRY
/** \brief Entry point of function Dem_J1939DcmGetNextFreezeFrame() */
#define DBG_DEM_J1939DCMGETNEXTFREEZEFRAME_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DEM_J1939DCMGETNEXTFREEZEFRAME_EXIT
/** \brief Exit point of function Dem_J1939DcmGetNextFreezeFrame() */
#define DBG_DEM_J1939DCMGETNEXTFREEZEFRAME_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_DEM_CLEARCHECKEVENT_ENTRY
/** \brief Entry point of function Dem_ClearCheckEvent() */
#define DBG_DEM_CLEARCHECKEVENT_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DEM_CLEARCHECKEVENT_EXIT
/** \brief Exit point of function Dem_ClearCheckEvent() */
#define DBG_DEM_CLEARCHECKEVENT_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_DEM_APPLYJ1939DTCFILTER_ENTRY
/** \brief Entry point of function Dem_ApplyJ1939DTCFilter() */
#define DBG_DEM_APPLYJ1939DTCFILTER_ENTRY(a,b)
#endif

#ifndef DBG_DEM_APPLYJ1939DTCFILTER_EXIT
/** \brief Exit point of function Dem_ApplyJ1939DTCFilter() */
#define DBG_DEM_APPLYJ1939DTCFILTER_EXIT(a,b,c)
#endif

#ifndef DBG_DEM_J1939DCMSETRATIOFILTER_ENTRY
/** \brief Entry point of function Dem_J1939DcmSetRatioFilter() */
#define DBG_DEM_J1939DCMSETRATIOFILTER_ENTRY(a,b,c)
#endif

#ifndef DBG_DEM_J1939DCMSETRATIOFILTER_EXIT
/** \brief Exit point of function Dem_J1939DcmSetRatioFilter() */
#define DBG_DEM_J1939DCMSETRATIOFILTER_EXIT(a,b,c,d)
#endif

#ifndef DBG_DEM_J1939DCMGETNEXTFILTEREDRATIO_ENTRY
/** \brief Entry point of function Dem_J1939DcmGetNextFilteredRatio() */
#define DBG_DEM_J1939DCMGETNEXTFILTEREDRATIO_ENTRY(a,b,c)
#endif

#ifndef DBG_DEM_J1939DCMGETNEXTFILTEREDRATIO_EXIT
/** \brief Exit point of function Dem_J1939DcmGetNextFilteredRatio() */
#define DBG_DEM_J1939DCMGETNEXTFILTEREDRATIO_EXIT(a,b,c,d)
#endif

#ifndef DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS2_ENTRY
/** \brief Entry point of function Dem_J1939DcmReadDiagnosticReadiness2() */
#define DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS2_ENTRY(a,b)
#endif

#ifndef DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS2_EXIT
/** \brief Exit point of function Dem_J1939DcmReadDiagnosticReadiness2() */
#define DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS2_EXIT(a,b,c)
#endif

#ifndef DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS3_ENTRY
/** \brief Entry point of function Dem_J1939DcmReadDiagnosticReadiness3() */
#define DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS3_ENTRY(a,b)
#endif

#ifndef DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS3_EXIT
/** \brief Exit point of function Dem_J1939DcmReadDiagnosticReadiness3() */
#define DBG_DEM_J1939DCMREADDIAGNOSTICREADINESS3_EXIT(a,b,c)
#endif

#ifndef DBG_DEM_J1939DCMGETINTERNALLAMPSTATUSBYTES_ENTRY
/** \brief Entry point of function Dem_J1939GetInternalLampStatusBytes() */
#define DBG_DEM_J1939DCMGETINTERNALLAMPSTATUSBYTES_ENTRY(a)
#endif

#ifndef DBG_DEM_J1939DCMGETINTERNALLAMPSTATUSBYTES_EXIT
/** \brief Exit point of function Dem_J1939GetInternalLampStatusBytes() */
#define DBG_DEM_J1939DCMGETINTERNALLAMPSTATUSBYTES_EXIT(a)
#endif

#ifndef DBG_DEM_J1939DCMCLEARFILTERCHECK_ENTRY
/** \brief Entry point of function Dem_J1939DcmClearFilterCheck() */
#define DBG_DEM_J1939DCMCLEARFILTERCHECK_ENTRY(a,b,c)
#endif

#ifndef DBG_DEM_J1939DCMCLEARFILTERCHECK_EXIT
/** \brief Exit point of function Dem_J1939DcmClearFilterCheck() */
#define DBG_DEM_J1939DCMCLEARFILTERCHECK_EXIT(a,b,c,d)
#endif

#ifndef DBG_DEM_CBTRIGGERJ1939DCM_ENTRY
/** \brief Entry point of function Dem_CbTriggerJ1939Dcm() */
#define DBG_DEM_CBTRIGGERJ1939DCM_ENTRY(a)
#endif

#ifndef DBG_DEM_CBTRIGGERJ1939DCM_EXIT
/** \brief Exit point of function Dem_CbTriggerJ1939Dcm() */
#define DBG_DEM_CBTRIGGERJ1939DCM_EXIT(a)
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef DEM_TRACE_H */
/*==================[end of file]===========================================*/
