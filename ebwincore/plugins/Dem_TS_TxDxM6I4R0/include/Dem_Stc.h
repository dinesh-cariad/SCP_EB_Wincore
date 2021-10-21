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
#ifndef DEM_STC_H
#define DEM_STC_H

/* This file includes all external static API declarations and type
 * definitions. */

/*==================[inclusions]============================================*/
/* !LINKSTO dsn.Dem.IncludeStr,1 */

#include <TSAutosar.h>                        /* EB specific standard types */
#include <Std_Types.h>                            /* AUTOSAR standard types */

#include <Dem_Version.h>                      /* Module version declaration */

#include <Dem_NvMCbk.h>                              /* Module callback API */

/*==================[macros]================================================*/

/*------------------[AUTOSAR event ID declaration]--------------------------*/

#if (defined DEM_EVENT_ID_INVALID)
#error DEM_EVENT_ID_INVALID already defined
#endif
/** \brief Symbolic name of invalid event ID */
#define DEM_EVENT_ID_INVALID 0U

#if (defined DEM_EVENT_ID_IGNORE)
#error DEM_EVENT_ID_IGNORE already defined
#endif
/** \brief Symbolic name of event ID that must be ignored during processing
 ** of the queue since it got invalidated */
#define DEM_EVENT_ID_IGNORE  0x1FFFU

/*-----------------------[Development error IDs]----------------------------*/

/* API service called with wrong parameter */

/* !LINKSTO Dem.DetReport.ParamConfig,4 */
#if (defined DEM_E_PARAM_CONFIG)
#error DEM_E_PARAM_CONFIG already defined
#endif
/** \brief DET error code for invalid configuration */
#define DEM_E_PARAM_CONFIG    0x10U

#if (defined DEM_E_WRONG_CONFIGURATION) /* to prevent double declaration */
#error DEM_E_WRONG_CONFIGURATION already defined
#endif
/** \brief DET error code for AR 4.3 in case an API function is called with
 **  a parameter value, which is not allowed by active configuration */
#define DEM_E_WRONG_CONFIGURATION  0x10U

#if (defined DEM_E_PARAM_POINTER)
#error DEM_E_PARAM_POINTER already defined
#endif
/** \brief DET error code for invalid address */
#define DEM_E_PARAM_POINTER   0x11U

#if (defined DEM_E_PARAM_DATA)
#error DEM_E_PARAM_DATA already defined
#endif
/** \brief DET error code for invalid data */
#define DEM_E_PARAM_DATA      0x12U

#if (defined DEM_E_PARAM_LENGTH)
#error DEM_E_PARAM_LENGTH already defined
#endif
/** \brief DET error code for invalid length */
#define DEM_E_PARAM_LENGTH    0x13U

/* API service called before Dem initialized */

#if (defined DEM_E_UNINIT)
#error DEM_E_UNINIT already defined
#endif
/** \brief  DET error code for wrong initialization status */
#define DEM_E_UNINIT          0x20U

/* No valid data available by the SW-C */

/* - DEM_E_NODATAAVAILABLE (0x30): DET error code for non-availabilty of data
 *   This value is also used as regular return value, refer to Dem_Types.h */

#if (defined DEM_E_INTEGRATION)
#error DEM_E_INTEGRATION already defined
#endif
/** \brief DET error code for unfulfilled return values from an SW-C */
#define DEM_E_INTEGRATION     0x41U

/* Required pre-condition for API call was not fulfilled */

#if (defined DEM_E_WRONG_CONDITION)
#error DEM_E_WRONG_CONDITION already defined
#endif
/** \brief DET error code for unfulfilled pre-conditions for an API call */
#define DEM_E_WRONG_CONDITION 0x40U

/* Calling context for API call was not fulfilled */

#if (defined DEM_E_WRONG_CONTEXT)
#error DEM_E_WRONG_CONTEXT already defined
#endif
/** \brief DET error code for wrong call-context in multi-core case */
#define DEM_E_WRONG_CONTEXT   0x42U

#if (defined DEM_E_INIT_FAILED) /* to prevent double declaration */
#error DEM_E_INIT_FAILED already defined
#endif
/** \brief DET error code for AR 4.3 in case Dem initialization fails */
#define DEM_E_INIT_FAILED  0x14U

#if (defined DEM_E_INVALID_OBDMID) /* to prevent double declaration */
#error DEM_E_INVALID_OBDMID already defined
#endif
/** \brief DET error code for AR 4.3 in case Dem_DcmGetAvailableOBDMIDs is
 **  called with invalid OBDMID */
#define DEM_E_INVALID_OBDMID  0x50U

/*==================[type definitions and related macros]===================*/

/*------------------[API function service IDs]------------------------------*/

#if (defined DEM_SID_GetVersionInfo)
#error DEM_SID_GetVersionInfo already defined
#endif
/** \brief Service Id for Dem_GetVersionInfo() */
#define DEM_SID_GetVersionInfo                                    0x00U

/* Interface ECU State Manager <--> Dem */

#if (defined DEM_SID_PreInit)
#error DEM_SID_PreInit already defined
#endif
/** \brief Service Id for Dem_PreInit() */
#define DEM_SID_PreInit                                           0x01U

#if (defined DEM_SID_Init)
#error DEM_SID_Init already defined
#endif
/** \brief Service Id for Dem_Init() */
#define DEM_SID_Init                                              0x02U

#if (defined DEM_SID_Shutdown)
#error DEM_SID_Shutdown already defined
#endif
/** \brief Service Id for Dem_Shutdown() */
#define DEM_SID_Shutdown                                          0x03U

/* Interface BSW modules / SW-Components <--> Dem */

#if (defined DEM_SID_ReportErrorStatus)
#error DEM_SID_ReportErrorStatus already defined
#endif
/** \brief Service Id for Dem_ReportErrorStatus() */
#define DEM_SID_ReportErrorStatus                                 0x0FU

#if (defined DEM_SID_SetEventStatus)
#error DEM_SID_SetEventStatus already defined
#endif
/** \brief Service Id for Dem_SetEventStatus() */
#define DEM_SID_SetEventStatus                                    0x04U

#if (defined DEM_SID_ResetEventStatus)
#error DEM_SID_ResetEventStatus already defined
#endif
/** \brief Service Id for Dem_ResetEventStatus() */
#define DEM_SID_ResetEventStatus                                  0x05U

#if (defined DEM_SID_PrestoreFreezeFrame)
#error DEM_SID_PrestoreFreezeFrame already defined
#endif
/** \brief Service Id for Dem_PrestoreFreezeFrame() */
#define DEM_SID_PrestoreFreezeFrame                               0x06U

#if (defined DEM_SID_ClearPrestoredFreezeFrame)
#error DEM_SID_ClearPrestoredFreezeFrame already defined
#endif
/** \brief Service Id for Dem_ClearPrestoredFreezeFrame() */
#define DEM_SID_ClearPrestoredFreezeFrame                         0x07U

#if (defined DEM_SID_SetOperationCycleState)
#error DEM_SID_SetOperationCycleState already defined
#endif
/** \brief Service Id for Dem_SetOperationCycleState() */
#define DEM_SID_SetOperationCycleState                            0x08U

#if (defined DEM_SID_ResetEventDebounceStatus)
#error DEM_SID_ResetEventDebounceStatus already defined
#endif
/** \brief Service Id for Dem_ResetEventDebounceStatus() */
#define DEM_SID_ResetEventDebounceStatus                          0x09U

#if (defined DEM_SID_SetAgingCycleState)
#error DEM_SID_SetAgingCycleState already defined
#endif
/** \brief Service Id for Dem_SetAgingCycleState() */
#define DEM_SID_SetAgingCycleState                                0x11U

#if (defined DEM_SID_SetAgingCycleCounterValue)
#error DEM_SID_SetAgingCycleCounterValue already defined
#endif
/** \brief Service Id for Dem_SetAgingCycleCounterValue() */
#define DEM_SID_SetAgingCycleCounterValue                         0x12U

#if (defined DEM_SID_GetEventStatus)
#error DEM_SID_GetEventStatus already defined
#endif
/** \brief Service Id for Dem_GetEventStatus() */
#define DEM_SID_GetEventStatus                                    0x0AU

#if (defined DEM_SID_GetEventFailed)
#error DEM_SID_GetEventFailed already defined
#endif
/** \brief Service Id for Dem_GetEventFailed() */
#define DEM_SID_GetEventFailed                                    0x0BU

#if (defined DEM_SID_GetEventTested)
#error DEM_SID_GetEventTested already defined
#endif
/** \brief Service Id for Dem_GetEventTested() */
#define DEM_SID_GetEventTested                                    0x0CU

#if (defined DEM_SID_GetDTCOfEvent)
#error DEM_SID_GetDTCOfEvent already defined
#endif
/** \brief Service Id for Dem_GetDTCOfEvent() */
#define DEM_SID_GetDTCOfEvent                                     0x0DU

#if (defined DEM_SID_SetEnableCondition)
#error DEM_SID_SetEnableCondition already defined
#endif
/** \brief Service Id for Dem_SetEnableCondition() */
#define DEM_SID_SetEnableCondition                                0x39U

#if (defined DEM_SID_SetStorageCondition)
#error DEM_SID_SetStorageCondition already defined
#endif
/** \brief Service Id for Dem_SetStorageCondition() */
#define DEM_SID_SetStorageCondition                               0x38U

#if (defined DEM_SID_GetFaultDetectionCounter)
#error DEM_SID_GetFaultDetectionCounter already defined
#endif
/** \brief Service Id for Dem_GetFaultDetectionCounter() */
#define DEM_SID_GetFaultDetectionCounter                          0x3EU

#if (defined DEM_SID_GetIndicatorStatus)
#error DEM_SID_GetIndicatorStatus already defined
#endif
/** \brief Service Id for Dem_GetIndicatorStatus() */
#define DEM_SID_GetIndicatorStatus                                0x29U

#if (defined DEM_SID_GetEventFreezeFrameData)
#error DEM_SID_GetEventFreezeFrameData already defined
#endif
/** \brief Service Id for Dem_GetEventFreezeFrameData() */
#define DEM_SID_GetEventFreezeFrameData                           0x31U

#if (defined DEM_SID_GetEventExtendedDataRecord)
#error DEM_SID_GetEventExtendedDataRecord already defined
#endif
/** \brief Service Id for Dem_GetEventExtendedDataRecord() */
#define DEM_SID_GetEventExtendedDataRecord                        0x30U

#if (defined DEM_SID_GetEventMemoryOverflow)
#error DEM_SID_GetEventMemoryOverflow already defined
#endif
/** \brief Service Id for Dem_GetEventMemoryOverflow() */
#define DEM_SID_GetEventMemoryOverflow                            0x32U

#if (defined DEM_SID_GetNumberOfEventMemoryEntries)
#error DEM_SID_GetNumberOfEventMemoryEntries already defined
#endif
/** \brief Service Id for Dem_GetNumberOfEventMemoryEntries() */
#define DEM_SID_GetNumberOfEventMemoryEntries                     0x35U

#if (defined DEM_SID_SetOperationCycleCntValue)
#error DEM_SID_SetOperationCycleCntValue already defined
#endif
/** \brief Service Id for Dem_SetOperationCycleCntValue() */
#define DEM_SID_SetOperationCycleCntValue                         0x36U

#if (defined DEM_SID_SetCycleQualified)
#error DEM_SID_SetCycleQualified already defined
#endif
/** \brief Service Id for Dem_SetCycleQualified() */
#define DEM_SID_SetCycleQualified                                 0x56U

#if (defined DEM_SID_GetSI30Status)
#error DEM_SID_GetSI30Status already defined
#endif
/** \brief Service Id for Dem_GetSI30Status() */
#define DEM_SID_GetSI30Status                                     0x83U

#if (defined DEM_SID_SetSI30Symptom)
#error DEM_SID_SetSI30Symptom already defined
#endif
/** \brief Service Id for Dem_SetSI30Symptom() */
#define DEM_SID_SetSI30Symptom                                    0x84U

#if (defined DEM_SID_SetSI30Notification)
#error DEM_SID_SetSI30Notification already defined
#endif
/** \brief Service Id for Dem_SetSI30Notification() */
#define DEM_SID_SetSI30Notification                               0x85U

#if (defined DEM_SID_SetDTCSuppression)
#error DEM_SID_SetDTCSuppression already defined
#endif
/** \brief Service Id for Dem_SetDTCSuppression() */
#define DEM_SID_SetDTCSuppression                                 0x33U

#if (defined DEM_SID_SetEventAvailable)
#error DEM_SID_SetEventAvailable already defined
#endif
/** \brief Service Id for Dem_SetEventAvailable() */
#define DEM_SID_SetEventAvailable                                 0x37U

#if (defined DEM_SID_SetWIRStatus)
#error DEM_SID_SetWIRStatus already defined
#endif
/** \brief Service Id for Dem_SetWIRStatus() */
#define DEM_SID_SetWIRStatus                                      0x7AU

#if (defined DEM_SID_SelectDTC)
#error DEM_SID_SelectDTC already defined
#endif
/** \brief Service Id for Dem_SelectDTC() */
#define DEM_SID_SelectDTC                                         0xB7U

#if (defined DEM_SID_GetDTCSelectionResult)
#error DEM_SID_GetDTCSelectionResult already defined
#endif
/** \brief Service Id for Dem_GetDTCSelectionResult() */
#define DEM_SID_GetDTCSelectionResult                             0xB8U

#if (defined DEM_SID_GetDTCSelectionResultForClearDTC)
#error DEM_SID_GetDTCSelectionResultForClearDTC already defined
#endif
/** \brief Service Id for Dem_GetDTCSelectionResultForClearDTC() */
#define DEM_SID_GetDTCSelectionResultForClearDTC                  0xBBU

/* Interface Dcm <--> Dem */

#if (defined DEM_SID_SetDTCFilter)
#error DEM_SID_SetDTCFilter already defined
#endif
/** \brief Service Id for Dem_SetDTCFilter() */
#define DEM_SID_SetDTCFilter                                      0x13U

#if (defined DEM_SID_SetFreezeFrameRecordFilter)
#error DEM_SID_SetFreezeFrameRecordFilter already defined
#endif
/** \brief Service Id for Dem_SetFreezeFrameRecordFilter() */
#define DEM_SID_SetFreezeFrameRecordFilter                        0x3FU

#if (defined DEM_SID_GetStatusOfDTC)
#error DEM_SID_GetStatusOfDTC already defined
#endif
/** \brief Service Id for Dem_GetStatusOfDTC() */
#define DEM_SID_GetStatusOfDTC                                    0x15U

#if (defined DEM_SID_GetDTCStatusAvailabilityMask)
#error DEM_SID_GetDTCStatusAvailabilityMask already defined
#endif
/** \brief Service Id for Dem_GetDTCStatusAvailabilityMask() */
#define DEM_SID_GetDTCStatusAvailabilityMask                      0x16U

#if (defined DEM_SID_GetNumberOfFilteredDTC)
#error DEM_SID_GetNumberOfFilteredDTC already defined
#endif
/** \brief Service Id for Dem_GetNumberOfFilteredDTC() */
#define DEM_SID_GetNumberOfFilteredDTC                            0x17U

#if (defined DEM_SID_GetNextFilteredDTC)
#error DEM_SID_GetNextFilteredDTC already defined
#endif
/** \brief Service Id for Dem_GetNextFilteredDTC() */
#define DEM_SID_GetNextFilteredDTC                                0x18U

#if (defined DEM_SID_GetDTCByOccurrenceTime)
#error DEM_SID_GetDTCByOccurrenceTime already defined
#endif
/** \brief Service Id for Dem_GetDTCByOccurrenceTime() */
#define DEM_SID_GetDTCByOccurrenceTime                            0x19U

#if (defined DEM_SID_GetNextFilteredRecord)
#error DEM_SID_GetNextFilteredRecord already defined
#endif
/** \brief Service Id for Dem_GetNextFilteredRecord() */
#define DEM_SID_GetNextFilteredRecord                             0x3AU

#if (defined DEM_SID_GetNextFilteredDTCAndFDC)
#error DEM_SID_GetNextFilteredDTCAndFDC already defined
#endif
/** \brief Service Id for Dem_GetNextFilteredDTCAndFDC() */
#define DEM_SID_GetNextFilteredDTCAndFDC                          0x3BU

#if (defined DEM_SID_GetNextFilteredDTCAndSeverity)
#error DEM_SID_GetNextFilteredDTCAndSeverity already defined
#endif
/** \brief Service Id for Dem_GetNextFilteredDTCAndSeverity() */
#define DEM_SID_GetNextFilteredDTCAndSeverity                     0x3DU

#if (defined DEM_SID_GetTranslationType)
#error DEM_SID_GetTranslationType already defined
#endif
/** \brief Service Id for Dem_GetTranslationType() */
#define DEM_SID_GetTranslationType                                0x3CU

#if (defined DEM_SID_GetSeverityOfDTC)
#error DEM_SID_GetSeverityOfDTC already defined
#endif
/** \brief Service Id for Dem_GetSeverityOfDTC() */
#define DEM_SID_GetSeverityOfDTC                                  0x0EU

#if (defined DEM_SID_GetFunctionalUnitOfDTC)
#error DEM_SID_GetFunctionalUnitOfDTC already defined
#endif
/** \brief Service Id for Dem_GetFunctionalUnitOfDTC() */
#define DEM_SID_GetFunctionalUnitOfDTC                            0x34U

#if (defined DEM_SID_DisableDTCRecordUpdate)
#error DEM_SID_DisableDTCRecordUpdate already defined
#endif
/** \brief Service Id for Dem_DisableDTCRecordUpdate() */
#define DEM_SID_DisableDTCRecordUpdate                            0x1AU

#if (defined DEM_SID_EnableDTCRecordUpdate)
#error DEM_SID_EnableDTCRecordUpdate already defined
#endif
/** \brief Service Id for Dem_EnableDTCRecordUpdate() */
#define DEM_SID_EnableDTCRecordUpdate                             0x1BU

#if (defined DEM_SID_GetNextFreezeFrameData)
#error DEM_SID_GetNextFreezeFrameData already defined
#endif
/** \brief Service Id for Dem_GetNextFreezeFrameData() */
#define DEM_SID_GetNextFreezeFrameData                            0x1DU

#if (defined DEM_SID_GetSizeOfFreezeFrameSelection)
#error DEM_SID_GetSizeOfFreezeFrameSelection already defined
#endif
/** \brief Service Id for Dem_GetSizeOfFreezeFrameSelection() */
#define DEM_SID_GetSizeOfFreezeFrameSelection                     0x1FU

#if (defined DEM_SID_GetNextExtendedDataRecord)
#error DEM_SID_GetNextExtendedDataRecord already defined
#endif
/** \brief Service Id for Dem_GetNextExtendedDataRecord() */
#define DEM_SID_GetNextExtendedDataRecord                         0x20U

#if (defined DEM_SID_GetSizeOfExtendedDataRecordSelection)
#error DEM_SID_GetSizeOfExtendedDataRecordSelection already defined
#endif
/** \brief Service Id for Dem_GetSizeOfExtendedDataRecordSelection() */
#define DEM_SID_GetSizeOfExtendedDataRecordSelection              0x21U

#if (defined DEM_SID_ClearDTC)
#error DEM_SID_ClearDTC already defined
#endif
/** \brief Service Id for Dem_ClearDTC() according with ASR42 or ASR431 */
#define DEM_SID_ClearDTC                                          0x23U

#if (defined DEM_SID_DisableDTCSetting)
#error DEM_SID_DisableDTCSetting already defined
#endif
/** \brief Service Id for Dem_DisableDTCSetting() */
#define DEM_SID_DisableDTCSetting                                 0x24U

#if (defined DEM_SID_EnableDTCSetting)
#error DEM_SID_EnableDTCSetting already defined
#endif
/** \brief Service Id for Dem_EnableDTCSetting() */
#define DEM_SID_EnableDTCSetting                                  0x25U

#if (defined DEM_SID_SelectFreezeFrameData)
#error DEM_SID_SelectFreezeFrameData already defined
#endif
/** \brief Service Id for Dem_SelectFreezeFrameData() */
#define DEM_SID_SelectFreezeFrameData                              0xB9U

#if (defined DEM_SID_SelectExtendedDataRecord)
#error DEM_SID_SelectExtendedDataRecord already defined
#endif
/** \brief Service Id for Dem_SelectExtendedDataRecord() */
#define DEM_SID_SelectExtendedDataRecord                           0xBAU

/* OBD-specific Interfaces */

#if (defined DEM_SID_SetEventDisabled)
#error DEM_SID_SetEventDisabled already defined
#endif
/** \brief Service Id for Dem_SetEventDisabled() */
#define DEM_SID_SetEventDisabled                                  0x51U

#if (defined DEM_SID_RepIUMPRFaultDetect)
#error DEM_SID_RepIUMPRFaultDetect already defined
#endif
/** \brief Service Id for Dem_RepIUMPRFaultDetect() */
#define DEM_SID_RepIUMPRFaultDetect                               0x73U

#if (defined DEM_SID_RepIUMPRDenLock)
#error DEM_SID_RepIUMPRDenLock already defined
#endif
/** \brief Service Id for Dem_RepIUMPRDenLock() */
#define DEM_SID_RepIUMPRDenLock                                   0x71U

#if (defined DEM_SID_RepIUMPRDenRelease)
#error DEM_SID_RepIUMPRDenRelease already defined
#endif
/** \brief Service Id for Dem_RepIUMPRDenRelease() */
#define DEM_SID_RepIUMPRDenRelease                                0x72U

#if (defined DEM_SID_DcmReadDataOfPID91)
#error DEM_SID_DcmReadDataOfPID91 already defined
#endif
/** \brief Service Id for Dem_DcmReadDataOfPID91() */
#define DEM_SID_DcmReadDataOfPID91                                0x6AU

#if (defined DEM_SID_DcmGetInfoTypeValue08)
#error DEM_SID_DcmGetInfoTypeValue08 already defined
#endif
/** \brief Service Id for Dem_DcmGetInfoTypeValue08() */
#define DEM_SID_DcmGetInfoTypeValue08                             0x6BU

#if (defined DEM_SID_DcmGetInfoTypeValue0B)
#error DEM_SID_DcmGetInfoTypeValue0B already defined
#endif
/** \brief Service Id for Dem_DcmGetInfoTypeValue0B() */
#define DEM_SID_DcmGetInfoTypeValue0B                             0x6CU

#if (defined DEM_SID_DcmReadDataOfPID01)
#error DEM_SID_DcmReadDataOfPID01 already defined
#endif
/** \brief Service Id for Dem_DcmReadDataOfPID01() */
#define DEM_SID_DcmReadDataOfPID01                                0x61U

#if (defined DEM_SID_DcmReadDataOfPID21)
#error DEM_SID_DcmReadDataOfPID21 already defined
#endif
/** \brief Service Id for Dem_DcmReadDataOfPID21() */
#define DEM_SID_DcmReadDataOfPID21                                0x64U

#if (defined DEM_SID_DcmReadDataOfPID30)
#error DEM_SID_DcmReadDataOfPID30 already defined
#endif
/** \brief Service Id for Dem_DcmReadDataOfPID30() */
#define DEM_SID_DcmReadDataOfPID30                                0x65U

#if (defined DEM_SID_DcmReadDataOfPID31)
#error DEM_SID_DcmReadDataOfPID31 already defined
#endif
/** \brief Service Id for Dem_DcmReadDataOfPID31() */
#define DEM_SID_DcmReadDataOfPID31                                0x66U

#if (defined DEM_SID_DcmReadDataOfPID41)
#error DEM_SID_DcmReadDataOfPID41 already defined
#endif
/** \brief Service Id for Dem_DcmReadDataOfPID41() */
#define DEM_SID_DcmReadDataOfPID41                                0x67U

#if (defined DEM_SID_DcmReadDataOfPID4D)
#error DEM_SID_DcmReadDataOfPID4D already defined
#endif
/** \brief Service Id for Dem_DcmReadDataOfPID4D() */
#define DEM_SID_DcmReadDataOfPID4D                                0x68U

#if (defined DEM_SID_DcmReadDataOfPID4E)
#error DEM_SID_DcmReadDataOfPID4E already defined
#endif
/** \brief Service Id for Dem_DcmReadDataOfPID4E() */
#define DEM_SID_DcmReadDataOfPID4E                                0x69U

#if (defined DEM_SID_DcmReadDataOfPID1C)
#error DEM_SID_DcmReadDataOfPID1C already defined
#endif
/** \brief Service Id for Dem_DcmReadDataOfPID1C() */
#define DEM_SID_DcmReadDataOfPID1C                                0x63U

#if (defined DEM_SID_DcmReadDataOfOBDFreezeFrame)
#error DEM_SID_DcmReadDataOfOBDFreezeFrame already defined
#endif
/** \brief Service Id for Dem_DcmReadDataOfOBDFreezeFrame() */
#define DEM_SID_DcmReadDataOfOBDFreezeFrame                       0x52U

#if (defined DEM_SID_DcmGetDTCOfOBDFreezeFrame)
#error DEM_SID_DcmGetDTCOfOBDFreezeFrame already defined
#endif
/** \brief Service Id for Dem_DcmGetDTCOfOBDFreezeFrame() */
#define DEM_SID_DcmGetDTCOfOBDFreezeFrame                         0x53U

#if (defined DEM_SID_SetPtoStatus)
#error DEM_SID_SetPtoStatus already defined
#endif
/** \brief Service Id for Dem_SetPtoStatus() */
#define DEM_SID_SetPtoStatus                                      0x79U

#if (defined DEM_SID_SetPfcCycleQualified)
#error DEM_SID_SetPfcCycleQualified already defined
#endif
/** \brief Service Id for Dem_SetPfcCycleQualified() */
#define DEM_SID_SetPfcCycleQualified                              0xAAU

#if (defined DEM_SID_GetPfcCycleQualified)
#error DEM_SID_GetPfcCycleQualified already defined
#endif
/** \brief Service Id for Dem_GetPfcCycleQualified() */
#define DEM_SID_GetPfcCycleQualified                              0xABU

#if (defined DEM_SID_ResetReadiness)
#error DEM_SID_ResetReadiness already defined
#endif
/** \brief Service Id for Dem_ResetReadiness() */
#define DEM_SID_ResetReadiness                                    0xACU

/* Vendor specific functions */
/* The values between 0xD0 ... 0xDF are reserved for non-ASR interfaces */
#if (defined DEM_SID_SetStoredDTCFilter)
#error DEM_SID_SetStoredDTCFilter already defined
#endif
/** \brief Service Id for Dem_DcmSetStoredDTCFilter() */
#define DEM_SID_SetStoredDTCFilter                                0xD0U

#if (defined DEM_SID_GetNextFilteredStoredDTC)
#error DEM_SID_GetNextFilteredStoredDTC already defined
#endif
/** \brief Service Id for Dem_DcmGetNextFilteredStoredDTC() */
#define DEM_SID_GetNextFilteredStoredDTC                          0xD1U

/* Scheduled functions */

#if (defined DEM_SID_MainFunction)
#error DEM_SID_MainFunction already defined
#endif
/** \brief Service Id for Dem_MainFunction() */
#define DEM_SID_MainFunction                                      0x55U

/* Callout functions */

#if (defined DEM_SID_CalloutDynamicDTCFnc)
#error DEM_SID_CalloutDynamicDTCFnc already defined
#endif
/** \brief Service Id for configured dynamic DTC callout function */
#define DEM_SID_CalloutDynamicDTCFnc                              0x80U

#if (defined DEM_SID_StoreImmediatelyNext)
#error DEM_SID_StoreImmediatelyNext already defined
#endif
/** \brief Service Id for Dem_StoreImmediatelyNext() */
#define DEM_SID_StoreImmediatelyNext                              0x81U

/* Debouncing functions */

#if (defined DEM_SID_ProcessUnconfirmedThreshold)
#error DEM_SID_ProcessUnconfirmedThreshold already defined
#endif
/** \brief Service Id for Dem_DebounceEventCounterBased() */
#define DEM_SID_ProcessUnconfirmedThreshold                       0x82U

/* Extended data assembly functions */

#if (defined DEM_SID_AssembleEDSegIntData)
#error DEM_SID_AssembleEDSegIntData already defined
#endif
/** \brief Service Id for Dem_AssembleEDSegIntData() */
#define DEM_SID_AssembleEDSegIntData                              0x86U

#if (defined DEM_SID_AssembleEDOnFDCThresholdData)
#error DEM_SID_AssembleEDOnFDCThresholdData already defined
#endif
/** \brief Service Id for Dem_AssembleEDOnFDCThresholdData() */
#define DEM_SID_AssembleEDOnFDCThresholdData                      0x87U

#if (defined DEM_SID_GetDebouncingOfEvent)
#error DEM_SID_GetDebouncingOfEvent already defined
#endif
/** \brief Service Id for Dem_GetDebouncingOfEvent() */
#define DEM_SID_GetDebouncingOfEvent                              0x9FU

#if (defined DEM_SID_GetEventExtendedDataRecordEx)
#error DEM_SID_GetEventExtendedDataRecordEx already defined
#endif
/** \brief Service Id for Dem_GetEventExtendedDataRecordEx() */
#define DEM_SID_GetEventExtendedDataRecordEx                      0x6DU

#if (defined DEM_SID_GetEventFreezeFrameDataEx)
#error DEM_SID_GetEventFreezeFrameDataEx already defined
#endif
/** \brief Service Id for Dem_GetEventFreezeFrameDataEx() */
#define DEM_SID_GetEventFreezeFrameDataEx                         0x6EU

#if (defined DEM_SID_J1939DcmSetDTCFilter)
#error DEM_SID_J1939DcmSetDTCFilter already defined
#endif
/** \brief Service Id for Dem_J1939DcmSetDTCFilter() */
#define DEM_SID_J1939DcmSetDTCFilter                               0x90U

#if (defined DEM_SID_J1939DcmGetNumberOfFilteredDTC)
#error DEM_SID_J1939DcmGetNumberOfFilteredDTC already defined
#endif
/** \brief Service Id for Dem_J1939DcmGetNumberOfFilteredDTC() */
#define DEM_SID_J1939DcmGetNumberOfFilteredDTC                     0x91U

#if (defined DEM_SID_J1939DcmGetNextFilteredDTC)
#error DEM_SID_J1939DcmGetNextFilteredDTC already defined
#endif
/** \brief Service Id for Dem_J1939DcmGetNextFilteredDTC() */
#define DEM_SID_J1939DcmGetNextFilteredDTC                         0x92U

#if (defined DEM_SID_J1939DcmClearDTC)
#error DEM_SID_J1939DcmClearDTC already defined
#endif
/** \brief Service Id for DEM_SID_J1939DcmClearDTC() */
#define DEM_SID_J1939DcmClearDTC                                   0x95U

#if (defined DEM_SID_J1939DcmSetFreezeFrameFilter)
#error DEM_SID_J1939DcmSetFreezeFrameFilter already defined
#endif
/** \brief Service Id for DEM_SID_J1939DcmSetFreezeFrameFilter() */
#define DEM_SID_J1939DcmSetFreezeFrameFilter                       0x96U

#if (defined DEM_SID_J1939DcmGetNextFreezeFrame)
#error DEM_SID_J1939DcmGetNextFreezeFrame already defined
#endif
/** \brief Service Id for DEM_SID_J1939DcmGetNextFreezeFrame() */
#define DEM_SID_J1939DcmGetNextFreezeFrame                         0x97U

#if (defined DEM_SID_J1939DcmSetRatioFilter)
#error DEM_SID_J1939DcmSetRatioFilter already defined
#endif
/** \brief Service Id for DEM_SID_J1939DcmSetRatioFilter() */
#define DEM_SID_J1939DcmSetRatioFilter                             0x99U

#if (defined DEM_SID_J1939DcmGetNextFilteredRatio)
#error DEM_SID_J1939DcmGetNextFilteredRatio already defined
#endif
/** \brief Service Id for DEM_SID_J1939DcmGetNextFilteredRatio() */
#define DEM_SID_J1939DcmGetNextFilteredRatio                       0x9AU

#if (defined DEM_SID_J1939DcmReadDiagnosticReadiness1)
#error DEM_SID_J1939DcmReadDiagnosticReadiness1 already defined
#endif
/** \brief Service Id for DEM_SID_J1939DcmReadDiagnosticReadiness1() */
#define DEM_SID_J1939DcmReadDiagnosticReadiness1                   0x9BU

#if (defined DEM_SID_J1939DcmReadDiagnosticReadiness2)
#error DEM_SID_J1939DcmReadDiagnosticReadiness2 already defined
#endif
/** \brief Service Id for DEM_SID_J1939DcmReadDiagnosticReadiness2() */
#define DEM_SID_J1939DcmReadDiagnosticReadiness2                   0x9CU

#if (defined DEM_SID_J1939DcmReadDiagnosticReadiness3)
#error DEM_SID_J1939DcmReadDiagnosticReadiness3 already defined
#endif
/** \brief Service Id for DEM_SID_J1939DcmReadDiagnosticReadiness3() */
#define DEM_SID_J1939DcmReadDiagnosticReadiness3                   0x9DU

#if (defined DEM_SID_DcmGetAvailableOBDMIDs)
#error DEM_SID_DcmGetAvailableOBDMIDs already defined
#endif
/** \brief Service Id for Dem_DcmGetAvailableOBDMIDs() */
#define DEM_SID_DcmGetAvailableOBDMIDs                             0xA3U

#if (defined DEM_SID_DcmGetNumTIDsOfOBDMID)
#error DEM_SID_DcmGetNumTIDsOfOBDMID already defined
#endif
/** \brief Service Id for Dem_DcmGetNumTIDsOfOBDMID() */
#define DEM_SID_DcmGetNumTIDsOfOBDMID                              0xA4U

#if (defined DEM_SID_DcmGetDTRData)
#error DEM_SID_DcmGetDTRData already defined
#endif
/** \brief Service Id for Dem_DcmGetDTRData() */
#define DEM_SID_DcmGetDTRData                                      0xA5U

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*------------------[Readiness groups]--------------------------------------*/

#if (defined DEM_NUM_OBD_RDY_GROUPS)
#error DEM_NUM_OBD_RDY_GROUPS already defined
#endif
#define DEM_NUM_OBD_RDY_GROUPS 0x0CU

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef DEM_STC_H */
/*==================[end of file]===========================================*/
