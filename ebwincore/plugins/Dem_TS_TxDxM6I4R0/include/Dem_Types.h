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
#ifndef DEM_TYPES_H
#define DEM_TYPES_H

/* This file provides the type definitions of the AUTOSAR module Dem.
 * Note: Some types are fixed and others depending on the configuration.
 */

/*==================[inclusions]============================================*/
/* !LINKSTO dsn.Dem.IncludeStr,1 */

#include <Std_Types.h>                            /* AUTOSAR standard types */
#include <TSAutosar.h>                        /* EB specific standard types */

#include <Dem_Cfg.h>                     /* Module public generated symbols */
#include <Dem_BSW_Types.h>                        /* Defines for RTE usage. */

/* Rte_Dem_Type.h is included indirectly via Dem_Cfg.h */

/*==================[macros]================================================*/

/*------------------[Dem error values used for Det Reportings]---------------*/

#if (defined DEM_E_NODATAAVAILABLE)
#error DEM_E_NODATAAVAILABLE already defined
#endif
/** \brief The requested event data is not currently stored
 ** (but the request was valid) */
#define DEM_E_NODATAAVAILABLE    0x30U /* 48 */

/*------------------[Dem-specific Std_ReturnType values]--------------------*/
/* !LINKSTO Dem.ASR431.StdReturnType_Implicit1,1 */
#if (defined DEM_PENDING)
#error DEM_PENDING already defined
#endif
/** \brief The requested operation is currently in progress */
#define DEM_PENDING              0x04U

#if (defined DEM_CLEAR_BUSY)
#error DEM_CLEAR_BUSY already defined
#endif
/** \brief Another client is currently clearing DTCs. The requested operation
 ** will not be started */
#define DEM_CLEAR_BUSY           0x05U

#if (defined DEM_CLEAR_MEMORY_ERROR)
#error DEM_CLEAR_MEMORY_ERROR already defined
#endif
/** \brief An error occurred during erasing a memory location */
#define DEM_CLEAR_MEMORY_ERROR   0x06U

#if (defined DEM_CLEAR_FAILED)
#error DEM_CLEAR_FAILED already defined
#endif
/** \brief DTC not cleared */
#define DEM_CLEAR_FAILED         0x07U

#if (defined DEM_WRONG_DTC)
#error DEM_WRONG_DTC already defined
#endif
/** \brief Selected DTC value in selected format does
 ** not exist or clearing is restricted */
#define DEM_WRONG_DTC            0x08U

#if (defined DEM_WRONG_DTCORIGIN)
#error DEM_WRONG_DTCORIGIN already defined
#endif
/** \brief Selected DTCOrigin does not exist */
#define DEM_WRONG_DTCORIGIN      0x09U

#if (defined DEM_E_NO_DTC_AVAILABLE)
#error DEM_E_NO_DTC_AVAILABLE already defined
#endif
/** \brief There is no DTC configured in the requested format
 **
 ** Dem-specific ::Std_ReturnType value used by Dem_GetDTCOfEvent()
 */
#if(DEM_ENABLE_ASR43_SERVICE_API == STD_ON)
  #define DEM_E_NO_DTC_AVAILABLE 0xAU /* 10 */
#else
  #define DEM_E_NO_DTC_AVAILABLE 0x2U /* 2 */
#endif /* (DEM_ENABLE_ASR43_SERVICE_API == STD_ON) */

#if (defined DEM_E_NO_FDC_AVAILABLE)
#error DEM_E_NO_FDC_AVAILABLE already defined
#endif
/** \brief  There is no fault detection counter available for the requested event */
#define DEM_E_NO_FDC_AVAILABLE   0xEU /* 14 */

#if (defined DEM_BUFFER_TOO_SMALL)
#error DEM_BUFFER_TOO_SMALL already defined
#endif
/** \brief The provided buffer size is too small */
#define DEM_BUFFER_TOO_SMALL     0x15U /* 21 */

#if (defined DEM_BUSY)
#error DEM_BUSY already defined
#endif
/** \brief Another Dem_SelectDTC or Dem_SelectDTC dependent operation
 ** of this client is currently in progress */
#define DEM_BUSY                 0x16U /* 22 */

#if (defined DEM_NO_SUCH_ELEMENT)
#error DEM_NO_SUCH_ELEMENT already defined
#endif
/** \brief The requested record number is not supported by the event */
#define DEM_NO_SUCH_ELEMENT      0x30U /* 48 */

/*------------------[Additional R4.2 Dem-specific Std_ReturnType values]------------------*/

#if(DEM_ENABLE_ASR43_SERVICE_API == STD_OFF)

#if (defined DEM_E_WRONG_BUFFERSIZE)
#error DEM_E_WRONG_BUFFERSIZE already defined
#endif
/** \brief The provided buffer size is too small */
#define DEM_E_WRONG_BUFFERSIZE     0x15U /* 21 */

/* note: these are used by Dem_ASR42_GetEventFreezeFrameData() */

#if (defined DEM_E_WRONG_RECORDNUMBER)
#error DEM_E_WRONG_RECORDNUMBER already defined
#endif
/** \brief The requested record number is not supported by the event */
#define DEM_E_WRONG_RECORDNUMBER  0x31U /* 49 */

#if (defined DEM_E_WRONG_DIDNUMBER)
#error DEM_E_WRONG_DIDNUMBER already defined
#endif
/** \brief The requested DID is not supported by the freeze frame */
#define DEM_E_WRONG_DIDNUMBER     0x32U /* 50 */

#endif /* (DEM_ENABLE_ASR43_SERVICE_API == STD_OFF) */

/*------------------[Development Freeze Frame]------------------------------*/
#if (defined DEM_NUM_OF_DEVFF_KIND)
#error DEM_NUM_OF_DEVFF_KIND already defined
#endif
/** \brief Maximal number of configurable development freeze frames kinds */
#define DEM_NUM_OF_DEVFF_KIND 2U

/*-----------------------------[Event Aging]--------------------------------*/
#if (defined DEM_SIZE_OF_AGING_COUNTER)
#error DEM_SIZE_OF_AGING_COUNTER already defined
#endif
/** \brief Size of Aging Counter */
#define DEM_SIZE_OF_AGING_COUNTER 1U

/*-----------------------------[Dynamic Event Availability]--------------------------------*/
#if (defined DEM_NO_EVENT_AVAILABLE)
#error DEM_NO_EVENT_AVAILABLE already defined
#endif
/** \brief UDS status of a disabled event */
#define DEM_NO_EVENT_AVAILABLE 0x00U

/*==================[type definitions]======================================*/

/*------------------[>Dem data types<]--------------------------------------*/

/*------------------[Dem_ConfigType]----------------------------------------*/

/* Dem_ConfigType is defined within Dem_Cfg.h
   This is to support the post-build time multiple configuration data
   structure. Configuration structure of type pointer to Dem_ConfigType is
   passed to Dem_PreInit(). Although this is not used during pre-compile
   configuration, EcuM always calls Dem_PreInit() with the name of a
   referenced DemConfigSet container.
*/

#if (DEM_INCLUDE_RTE == STD_OFF)

/*------------------[Dem_EventIdType]---------------------------------------*/

/* Dem_EventIdType is defined within Dem_Cfg.h and related EventId symbols
 * are defined in Dem_IntEvtId.h and Dem_IntErrId.h */

/*------------------[Dem_EventStatusType]-----------------------------------*/
#ifndef RTE_TYPE_Dem_EventStatusType
#define RTE_TYPE_Dem_EventStatusType
/** \brief This type contains all monitor test result values, which can be
 ** reported via Dem_ReportErrorStatus() and Dem_SetEventStatus().
 **
 ** bits 0x04..0xFF: reserved
 */
typedef uint8 Dem_EventStatusType;
#endif

#ifndef DEM_EVENT_STATUS_PASSED
/** \brief Monitor reports qualified test result passed */
#define DEM_EVENT_STATUS_PASSED      0x00U
#endif

#ifndef DEM_EVENT_STATUS_FAILED
/** \brief Monitor reports qualified test result failed */
#define DEM_EVENT_STATUS_FAILED      0x01U
#endif

#ifndef DEM_EVENT_STATUS_PREPASSED
/** \brief Monitor reports non-qualified test result pre-passed
 ** (debounced Dem-internally) */
#define DEM_EVENT_STATUS_PREPASSED   0x02U
#endif

#ifndef DEM_EVENT_STATUS_PREFAILED
/** \brief Monitor reports non-qualified test result pre-failed
 ** (debounced Dem-internally) */
#define DEM_EVENT_STATUS_PREFAILED   0x03U
#endif

/*------------------[Dem_DebounceResetStatusType]---------------------------*/
#ifndef RTE_TYPE_Dem_DebounceResetStatusTyp
#define RTE_TYPE_Dem_DebounceResetStatusTyp
/** \brief This type contains all monitor test result values, which can be
 ** reported via Dem_ResetEventDebounceStatus().
 **
 ** bits 0x02..0xFF: reserved
 */
typedef uint8 Dem_DebounceResetStatusType;
#endif

/* !LINKSTO SWS_Dem_00927,1
 */
#ifndef DEM_DEBOUNCE_STATUS_FREEZE
/** \brief Freeze status value for event debouncing */
#define DEM_DEBOUNCE_STATUS_FREEZE            0x00U
#endif

#ifndef DEM_DEBOUNCE_STATUS_RESET
/** \brief Reset status value for event debouncing */
#define DEM_DEBOUNCE_STATUS_RESET             0x01U
#endif

/*------------------[Dem_EventStatusExtendedType]---------------------------*/
#ifndef RTE_TYPE_Dem_EventStatusExtendedType
#define RTE_TYPE_Dem_EventStatusExtendedType
/** \brief In this data-type each bit has an individual meaning. The bit is
 ** set to 1 when the condition holds. For example, if the 2nd bit (0x02)
 ** is set to 1, this means that the test failed this operation cycle.
 ** If the bit is set to 0, it has not yet failed this cycle.
 **
 ** Used in Dem_GetEventStatus() and Xxx_TriggerOnEventStatus().
 ** - Bit0: ::DEM_UDS_STATUS_TF
 ** - Bit1: ::DEM_UDS_STATUS_TFTOC
 ** - Bit2: ::DEM_UDS_STATUS_PDTC
 ** - Bit3: ::DEM_UDS_STATUS_CDTC
 ** - Bit4: ::DEM_UDS_STATUS_TNCSLC
 ** - Bit5: ::DEM_UDS_STATUS_TFSLC
 ** - Bit6: ::DEM_UDS_STATUS_TNCTOC
 ** - Bit7: ::DEM_UDS_STATUS_WIR */
typedef uint8 Dem_EventStatusExtendedType;
#endif

#ifndef DEM_UDS_STATUS_TF
/** \brief Test Failed status-bit */
#define DEM_UDS_STATUS_TF     0x01U
#endif

#ifndef DEM_UDS_STATUS_TFTOC
/** \brief Test Failed This Operation Cycle status-bit */
#define DEM_UDS_STATUS_TFTOC  0x02U
#endif

#ifndef DEM_UDS_STATUS_PDTC
/** \brief Pending DTC status-bit */
#define DEM_UDS_STATUS_PDTC   0x04U
#endif

#ifndef DEM_UDS_STATUS_CDTC
/** \brief Confirmed DTC status-bit */
#define DEM_UDS_STATUS_CDTC   0x08U
#endif

#ifndef DEM_UDS_STATUS_TNCSLC
/** \brief Test Not Completed Since Last Clear status-bit */
#define DEM_UDS_STATUS_TNCSLC 0x10U
#endif

#ifndef DEM_UDS_STATUS_TFSLC
/** \brief Test Failed Since Last Clear status-bit */
#define DEM_UDS_STATUS_TFSLC  0x20U
#endif

#ifndef DEM_UDS_STATUS_TNCTOC
/** \brief Test Not Completed This Operation Cycle status-bit */
#define DEM_UDS_STATUS_TNCTOC 0x40U
#endif

#ifndef DEM_UDS_STATUS_WIR
/** \brief Warning Indicator Requested status-bit */
#define DEM_UDS_STATUS_WIR    0x80U
#endif

/*------------------[Dem_OperationCycleIdType]------------------------------*/
#ifndef RTE_TYPE_Dem_OperationCycleIdType
#define RTE_TYPE_Dem_OperationCycleIdType
/** \brief Used to select the individual operation cycle in the API
 ** Dem_SetOperationCycleState()
 **
 ** value: The OperationCycleId, e.g. the ignition cycle is assigned to an ID
 **
 ** \note This type has been replaced with compatible base type (uint8) in
 ** AR3.1 Dem SWS, however it has been retained here for compatibility. */
typedef uint8 Dem_OperationCycleIdType;
#endif

/*------------------[Dem_OperationCycleStateType]---------------------------*/
#ifndef RTE_TYPE_Dem_OperationCycleStateType
#define RTE_TYPE_Dem_OperationCycleStateType
/** \brief This type contains operation cycle state values, which can be
 ** reported via Dem_SetOperationCycleState() */
typedef uint8 Dem_OperationCycleStateType;
#endif

#ifndef DEM_CYCLE_STATE_START
/** \brief Start/restart the operation cycle */
#define DEM_CYCLE_STATE_START 0x00U
#endif

#ifndef DEM_CYCLE_STATE_END
/** \brief End of operation cycle */
#define DEM_CYCLE_STATE_END   0x01U
#endif

/*------------------[Dem_IndicatorIdType]-----------------------------------*/
#ifndef RTE_TYPE_Dem_IndicatorIdType
#define RTE_TYPE_Dem_IndicatorIdType
/** \brief Used to request a indicator type
 **
 ** value: indicator lamp ID
 **
 ** \note This type has been replaced with compatible base type (uint8) in
 ** AR3.1 Dem SWS, however it has been retained here for compatibility. */
typedef uint8 Dem_IndicatorIdType;
#endif

/*------------------[Dem_IndicatorStatusType]-------------------------------*/
#ifndef RTE_TYPE_Dem_IndicatorStatusType
#define RTE_TYPE_Dem_IndicatorStatusType
/** \brief Used to return the status of Dem_GetIndicatorStatus() */
typedef uint8 Dem_IndicatorStatusType;
#endif

#ifndef DEM_INDICATOR_OFF
/** \brief Indicator off mode */
#define DEM_INDICATOR_OFF        0x00U
#endif

#ifndef DEM_INDICATOR_CONTINUOUS
/** \brief Indicator continuously on mode */
#define DEM_INDICATOR_CONTINUOUS 0x01U
#endif

#ifndef DEM_INDICATOR_BLINKING
/** \brief Indicator blinking mode */
#define DEM_INDICATOR_BLINKING   0x02U
#endif

#ifndef DEM_INDICATOR_BLINK_CONT
/** \brief Indicator blinking or continuously on mode. The SW-C for indicator
 ** is responsible to decide if the indicator is blinking or continuously on. */
#define DEM_INDICATOR_BLINK_CONT 0x03U
#endif

#ifndef DEM_INDICATOR_SLOW_FLASH
/** \brief Indicator slow flash mode */
#define DEM_INDICATOR_SLOW_FLASH 0x04U
#endif

#ifndef DEM_INDICATOR_FAST_FLASH
/** \brief Indicator fast flash mode */
#define DEM_INDICATOR_FAST_FLASH 0x05U
#endif

/*-----------------[Dem_DebouncingStateType]--------------------------*/
#ifndef RTE_TYPE_Dem_DebouncingStateType
#define RTE_TYPE_Dem_DebouncingStateType
/** \brief Type for debouncing state */
typedef uint8 Dem_DebouncingStateType;
#endif

#endif /* DEM_INCLUDE_RTE == STD_OFF */

/*------------------[Dem_DTCGroupType]--------------------------------------*/

/** \brief Used to define the group of DTCs
 **
 ** The user can add further groups. Unused bytes shall be filled with 00.
 */
typedef uint32 Dem_DTCGroupType;

/* !LINKSTO Dem.DataTypes_Implicit7,1 */
#if (defined DEM_DTC_GROUP_EMISSION_REL_DTCS)
#error DEM_DTC_GROUP_EMISSION_REL_DTCS already defined
#endif
/** \brief Emission related DTCs */
#define DEM_DTC_GROUP_EMISSION_REL_DTCS  0x000000U

/* !LINKSTO Dem.DataTypes_Implicit7,1 */
#if (defined DEM_DTC_GROUP_ALL_DTCS)
#error DEM_DTC_GROUP_ALL_DTCS already defined
#endif
/** \brief All DTCs */
#define DEM_DTC_GROUP_ALL_DTCS          0xFFFFFFU

/* further DEM_DTC_GROUP_<GROUP_NAME> defines are generated in Dem_Cfg.h */

/*------------------[Dem_DTCKindType]---------------------------------------*/

/** \brief Used to define the DTC kind (type) */
typedef uint8 Dem_DTCKindType;

#if (defined DEM_DTC_KIND_ALL_DTCS)
#error DEM_DTC_KIND_ALL_DTCS already defined
#endif
/** \brief Select all DTCs */
#define DEM_DTC_KIND_ALL_DTCS          0x01U

#if (defined DEM_DTC_KIND_EMISSION_REL_DTCS)
#error DEM_DTC_KIND_EMISSION_REL_DTCS already defined
#endif
/** \brief Select OBD-relevant DTCs */
#define DEM_DTC_KIND_EMISSION_REL_DTCS 0x02U

/*------------------[Dem_DTCFormatType]-------------------------------------*/
/* !LINKSTO Dem.ASR431.SWS_Dem_00933,1 */
#if (DEM_INCLUDE_RTE == STD_OFF)

#ifndef RTE_TYPE_Dem_DTCFormatType
#define RTE_TYPE_Dem_DTCFormatType
/** \brief Selects/specifies the format of the DTC value */
typedef uint8 Dem_DTCFormatType;
#endif

#ifndef DEM_DTC_FORMAT_OBD
/** \brief selects the 2-byte OBD DTC format (refer to configuration parameter
 ** DemObdDTC) */
#define DEM_DTC_FORMAT_OBD             0x00U
#endif

#ifndef DEM_DTC_FORMAT_UDS
/** \brief selects the 3-byte UDS DTC format (refer to configuration parameter
 ** DemUdsDTC) */
#define DEM_DTC_FORMAT_UDS             0x01U
#endif

#ifndef DEM_DTC_FORMAT_J1939
/** \brief selects the merged SPN + FMI to 3-byte J1939 DTC format */
#define DEM_DTC_FORMAT_J1939           0x02U
#endif

#else /* DEM_INCLUDE_RTE == STD_OFF */
/* ClientId based Dcm interface functions are based on ASR4.3.1 and hence require
 * DTCformat DEM_DTC_FORMAT_J1939 even if ASR40 service API is configured. */
#if ( (DEM_DEFAULT_ASR_SERVICE_API == DEM_SERVICE_API_ASR40) && \
      (DEM_ENABLE_ASR42_SERVICE_API == STD_OFF) )
#ifndef DEM_DTC_FORMAT_J1939
/** \brief selects the merged SPN + FMI to 3-byte J1939 DTC format */
#define DEM_DTC_FORMAT_J1939           0x02U
#endif
#endif

#endif /* DEM_INCLUDE_RTE == STD_ON */

/*------------------[Dem_DTCOriginType]-------------------------------------*/
/* !LINKSTO Dem.ASR403.DTCOriginType,1 */
/* !LINKSTO Dem.ASR431.DTCOriginType,1 */
#if (DEM_INCLUDE_RTE == STD_OFF)

#ifndef RTE_TYPE_Dem_DTCOriginType
#define RTE_TYPE_Dem_DTCOriginType
/** \brief Enum used to define the location of the events/DTC
 **
 ** The ::Dem_DTCOriginType is used to differ between the different event
 ** memories. The definition and use of the different memory types is OEM
 ** specific.
 */
typedef uint16 Dem_DTCOriginType;
#endif

#ifndef DEM_DTC_ORIGIN_PRIMARY_MEMORY
/** \brief Event information located in the primary memory */
#define DEM_DTC_ORIGIN_PRIMARY_MEMORY       0x01U
#endif

#ifndef DEM_DTC_ORIGIN_MIRROR_MEMORY
/** \brief Event information located in the mirror memory */
#define DEM_DTC_ORIGIN_MIRROR_MEMORY        0x02U
#endif

#ifndef DEM_DTC_ORIGIN_PERMANENT_MEMORY
/** \brief Event information located in the permanent memory */
#define DEM_DTC_ORIGIN_PERMANENT_MEMORY     0x03U
#endif

#ifndef DEM_DTC_ORIGIN_SECONDARY_MEMORY
/** \brief Event information located in the secondary memory */
#define DEM_DTC_ORIGIN_SECONDARY_MEMORY     0x04U
#endif
#endif /* DEM_INCLUDE_RTE == STD_OFF */

#ifndef DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY
/** \brief Event information located in the OBD relevant memory */
#define DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY  0x04U
#endif

/*------------------[Dem_DataKindType]-------------------------------------*/

/* !LINKSTO Dem.Dem_DataKindType.Type,1 */
/** \brief Used to define the data kind (type) */
typedef uint8 Dem_DataKindType;

#if (defined DEM_DATAKIND_FREEZEFRAME)
#error DEM_DATAKIND_FREEZEFRAME already defined
#endif
/** \brief Select Freeze Frame data kind */
#define DEM_DATAKIND_FREEZEFRAME      0x00U

#if (defined DEM_DATAKIND_EXTENDEDDATA)
#error DEM_DATAKIND_EXTENDEDDATA already defined
#endif
/** \brief Select Extended Data kind */
#define DEM_DATAKIND_EXTENDEDDATA     0x01U

/*----------------[Dem_DTCRequestType]--------------------------------------*/
#if (DEM_GET_DTCBYOCCURRENCETIME_SUPPORT == STD_ON)

/** \brief Request type for Dem_GetDTCByOccurrenceTime() */
typedef uint8 Dem_DTCRequestType;

#if (defined DEM_FIRST_FAILED_DTC)
#error DEM_FIRST_FAILED_DTC already defined
#endif
/** \brief First failed DTC requested */
#define DEM_FIRST_FAILED_DTC           0x01U

#if (defined DEM_MOST_RECENT_FAILED_DTC)
#error DEM_MOST_RECENT_FAILED_DTC already defined
#endif
/** \brief Most recent failed DTC requested */
#define DEM_MOST_RECENT_FAILED_DTC     0x02U

#if (defined DEM_FIRST_DET_CONFIRMED_DTC)
#error DEM_FIRST_DET_CONFIRMED_DTC already defined
#endif
/** \brief First detected confirmed DTC requested */
#define DEM_FIRST_DET_CONFIRMED_DTC    0x03U

#if (defined DEM_MOST_REC_DET_CONFIRMED_DTC)
#error DEM_MOST_REC_DET_CONFIRMED_DTC already defined
#endif
/** \brief Most recently detected confirmed DTC requested */
#define DEM_MOST_REC_DET_CONFIRMED_DTC 0x04U

#if (defined DEM_MAXNUM_OCCURRENCE_KINDS)
#error DEM_MAXNUM_OCCURRENCE_KINDS already defined
#endif
/** \brief Size of array used to store the EventIds corresponding
 **  to the occurrence kinds.
 **  This macro is not AUTOSAR specific but used for Dem_NvData structure.
 **  The macro definition is in this file because it belongs to
 **  the Dem_GetDTCByOccurrenceTime() functionality.
 */
#define DEM_MAXNUM_OCCURRENCE_KINDS           0x04U

#endif /* #if (DEM_GET_DTCBYOCCURRENCETIME_SUPPORT == STD_ON) */

/*------------------[Dem_DTCTranslationFormatType]--------------------------*/

/** \brief DTC translation format as defined in ISO14229-1 Service 0x19
 ** returned by Dem_GetTranslationType().
 **
 ** \note This type has been replaced with compatible base type (uint8) in
 ** AR3.1 Dem SWS, however it has been retained here for compatibility.
 */
typedef uint8 Dem_DTCTranslationFormatType;

#if (defined DEM_DTC_TRANSLATION_ISO15031_6)
#error DEM_DTC_TRANSLATION_ISO15031_6 already defined
#endif
/** \brief DTCs of ISO15031-6 */
#define DEM_DTC_TRANSLATION_ISO15031_6     0x00U

#if (defined DEM_DTC_TRANSLATION_ISO14229_1)
#error DEM_DTC_TRANSLATION_ISO14229_1 already defined
#endif
/** \brief DTCs of ISO14229-1 */
#define DEM_DTC_TRANSLATION_ISO14229_1     0x01U

#if (defined DEM_DTC_TRANSLATION_SAEJ1939_73)
#error DEM_DTC_TRANSLATION_SAEJ1939_73 already defined
#endif
/** \brief DTCs of SAEJ1939-73 */
#define DEM_DTC_TRANSLATION_SAEJ1939_73    0x02U

#if (defined DEM_DTC_TRANSLATION_ISO11992_4)
#error DEM_DTC_TRANSLATION_ISO11992_4 already defined
#endif
/** \brief DTCs of ISO11992_4 */
#define DEM_DTC_TRANSLATION_ISO11992_4     0x03U

#if (defined DEM_DTC_TRANSLATION_J2012DA_FORMAT_04)
#error DEM_DTC_TRANSLATION_J2012DA_FORMAT_04 already defined
#endif
/** \brief DTCs of SAE_J2012-DA_DTCFormat_04 */
#define DEM_DTC_TRANSLATION_J2012DA_FORMAT_04     0x04U

/*----------------[Dem_DTCSeverityType]-------------------------------------*/

/** \brief Defines the type of a DTCSeverityMask according to ISO14229-1 */
typedef uint8 Dem_DTCSeverityType;

#if (defined DEM_SEVERITY_NO_SEVERITY)
#error DEM_SEVERITY_NO_SEVERITY already defined
#endif
/** \brief No severity information available */
#define DEM_SEVERITY_NO_SEVERITY                  0x00U

#if (defined DEM_SEVERITY_WWHOBD_CLASS_NO_CLASS)
#error DEM_SEVERITY_WWHOBD_CLASS_NO_CLASS already defined
#endif
/** \brief No class information */
#define DEM_SEVERITY_WWHOBD_CLASS_NO_CLASS        0x01U

#if (defined DEM_SEVERITY_WWHOBD_CLASS_A)
#error DEM_SEVERITY_WWHOBD_CLASS_A already defined
#endif
/** \brief WWH-OBD Class A */
#define DEM_SEVERITY_WWHOBD_CLASS_A               0x02U

#if (defined DEM_SEVERITY_WWHOBD_CLASS_B1)
#error DEM_SEVERITY_WWHOBD_CLASS_B1 already defined
#endif
/** \brief WWH-OBD Class B1 */
#define DEM_SEVERITY_WWHOBD_CLASS_B1              0x04U

#if (defined DEM_SEVERITY_WWHOBD_CLASS_B2)
#error DEM_SEVERITY_WWHOBD_CLASS_B2 already defined
#endif
/** \brief WWH-OBD Class B2 */
#define DEM_SEVERITY_WWHOBD_CLASS_B2              0x08U

#if (defined DEM_SEVERITY_WWHOBD_CLASS_C)
#error DEM_SEVERITY_WWHOBD_CLASS_C already defined
#endif
/** \brief WWH-OBD Class C */
#define DEM_SEVERITY_WWHOBD_CLASS_C               0x10U

#if (defined DEM_SEVERITY_MAINTENANCE_ONLY)
#error DEM_SEVERITY_MAINTENANCE_ONLY already defined
#endif
/** \brief Maintenance required */
#define DEM_SEVERITY_MAINTENANCE_ONLY             0x20U

#if (defined DEM_SEVERITY_CHECK_AT_NEXT_HALT)
#error DEM_SEVERITY_CHECK_AT_NEXT_HALT already defined
#endif
/** \brief Check at next halt */
#define DEM_SEVERITY_CHECK_AT_NEXT_HALT           0x40U

#if (defined DEM_SEVERITY_CHECK_IMMEDIATELY)
#error DEM_SEVERITY_CHECK_IMMEDIATELY already defined
#endif
/** \brief Check immediately */
#define DEM_SEVERITY_CHECK_IMMEDIATELY            0x80U

#if ( (DEM_INCLUDE_RTE == STD_OFF) || \
      (DEM_DEFAULT_ASR_SERVICE_API == DEM_SERVICE_API_ASR40) )

/*------------------[Dem_UdsStatusByteType]---------------------------------*/

/* !LINKSTO Dem.ASR431.SWS_Dem_00928,1 */
/** \brief Used to set the current status
 **
 ** Lower limit 0x00 Elements
 ** Lower limit 0xFF Elements
 **
 ** In this data-type each bit has an individual meaning. The bit is set to
 ** 1 when the condition holds. For example, if the 2nd bit (0x02) is set
 ** to 1, this means that the test failed this operation cycle. If the bit is
 ** set to 0, it has not yet failed this cycle.
 ** Used in Dem_GetDTCStatusAvailabilityMask().
 ** - Bit0: ::DEM_UDS_STATUS_TF
 ** - Bit1: ::DEM_UDS_STATUS_TFTOC
 ** - Bit2: ::DEM_UDS_STATUS_PDTC
 ** - Bit3: ::DEM_UDS_STATUS_CDTC
 ** - Bit4: ::DEM_UDS_STATUS_TNCSLC
 ** - Bit5: ::DEM_UDS_STATUS_TFSLC
 ** - Bit6: ::DEM_UDS_STATUS_TNCTOC
 ** - Bit7: ::DEM_UDS_STATUS_WIR */
typedef uint8 Dem_UdsStatusByteType;
#endif

#if ((DEM_J1939_SUPPORT == STD_ON) && (DEM_J1939_READINESS1_SUPPORT == STD_ON))
/*------------------[Dem_J1939DcmDiagnosticReadiness1Type]------------------------------*/
/* !LINKSTO Dem.ASR422.SWS_Dem_00949,1 */
/** \brief This structure represents all data elemets of the DM05 message.*/
typedef struct
{
  uint16             NonContinuouslyMonitoredSystemsSupport;
       /**< Identifies the noncontinuously monitored systems support */
  uint16             NonContinuouslyMonitoredSystemsStatus;
       /**< Identifies the noncontinuously monitored systems status */
  uint8             ActiveTroubleCodes;   /**< Number of active DTCs */
  uint8             PreviouslyActiveDiagnosticTroubleCodes;
                               /**< Number of previously active DTCs */
  uint8             OBDCompliance;               /**< OBD Compliance */
  uint8             ContinuouslyMonitoredSystemsSupport_Status;
 /**< Identifies the continuously monitored system support and status */

} Dem_J1939DcmDiagnosticReadiness1Type;

#endif

#if ((DEM_J1939_SUPPORT == STD_ON) && (DEM_J1939_READINESS2_SUPPORT == STD_ON))
/*------------------[Dem_J1939DcmDiagnosticReadiness2Type]------------------------------*/
/* !LINKSTO Dem.ASR422.SWS_Dem_00950,1 */
/** \brief This structure represents all data elemets of the DM05 message.*/
typedef struct
{
  uint16             DistanceTraveledWhileMILisActivated;
       /**< The kilometers accumulated while the MIL is activated */
  uint16             DistanceSinceDTCsCleared;
       /**< Distance accumulated since emission related DTCs were cleared */
  uint16             MinutesRunbyEngineWhileMILisActivated;
       /**< Accumulated count (in minutes) while the MIL is activated (on) */
  uint16             TimeSinceDiagnosticTroubleCodesCleared;
       /**< Engine running time accumulated since emission related DTCs were cleared */
} Dem_J1939DcmDiagnosticReadiness2Type;

#endif

#if ((DEM_J1939_SUPPORT == STD_ON) && (DEM_J1939_READINESS3_SUPPORT == STD_ON))
/*------------------[Dem_J1939DcmDiagnosticReadiness3Type]------------------------------*/
/* !LINKSTO Dem.ASR422.SWS_Dem_00951,1 */
/** \brief This structure represents all data elemets of the DM05 message */
typedef struct
{
  uint16             TimeSinceEngineStart;
       /**< Time since key-on that the engine has been running */
  uint16             NonContinuouslyMonitoredSystemsEnableStatus;
       /**< Enable status of noncontinuous monitors this monitoring cycle */
  uint16             NonContinuouslyMonitoredSystems;
       /**< Completion status of noncontinuous monitors this monitoring cycle */
  uint8              NumberofWarmupsSinceDTCsCleared;
       /**< Number of OBD warmup cycles since all DTCs were cleared */
  uint8             ContinuouslyMonitoredSystemsEnableCompletedStatus;
       /**< Identifies the continuously monitored system enable/completed support and status */
} Dem_J1939DcmDiagnosticReadiness3Type;

#endif

/*------------------[Dem_J1939DcmDTCStatusFilterType]---------------------------------*/
#if (DEM_J1939_SUPPORT == STD_ON)
/** \brief The type to distinguish which DTCs should be filtered */
/* !LINKSTO Dem.ASR422.SWS_Dem_00945,1 */
typedef uint8 Dem_J1939DcmDTCStatusFilterType;

#if (defined DEM_J1939DTC_ACTIVE)
#error DEM_J1939DTC_ACTIVE already defined
#endif
/** \brief Active DTCs filter */
#define DEM_J1939DTC_ACTIVE  0x00U

#if (defined DEM_J1939DTC_PREVIOUSLY_ACTIVE)
#error DEM_J1939DTC_PREVIOUSLY_ACTIVE already defined
#endif
/** \brief Previously active DTCs filter */
#define DEM_J1939DTC_PREVIOUSLY_ACTIVE   0x01U

#if (defined DEM_J1939DTC_PENDING)
#error DEM_J1939DTC_PENDING already defined
#endif
/** \brief Pending DTCs filter */
#define DEM_J1939DTC_PENDING   0x02U

#if (defined DEM_J1939DTC_PERMANENT)
#error DEM_J1939DTC_PERMANENT already defined
#endif
/** \brief Permanent DTCs filter */
#define DEM_J1939DTC_PERMANENT   0x03U

#if (defined DEM_J1939DTC_CURRENTLY_ACTIVE)
#error DEM_J1939DTC_CURRENTLY_ACTIVE already defined
#endif
/** \brief Currently active DTCs filter */
#define DEM_J1939DTC_CURRENTLY_ACTIVE   0x04U

/*------------------[Dem_J1939DcmLampStatusType]---------------------------------*/
/** \brief The J1939 status of the 4 lamps MIL, RSI, AWL and protect */
/* !LINKSTO Dem.ASR422.SWS_Dem_00948,1 */
typedef uint16 Dem_J1939DcmLampStatusType;

#if (defined DEM_J1939MIL_ACTIVE)
#error DEM_J1939MIL_ACTIVE already defined
#endif
/** \brief MIL is active */
#define DEM_J1939MIL_ACTIVE  0x4000U

#if (defined DEM_J1939MIL_FAST_FLASH)
#error DEM_J1939MIL_FAST_FLASH already defined
#endif
/** \brief MIL is fast flashing */
#define DEM_J1939MIL_FAST_FLASH  0x0040U

#if (defined DEM_J1939MIL_CONT_UNAVAILABLE)
#error DEM_J1939MIL_CONT_UNAVAILABLE already defined
#endif
/** \brief MIL is continuously lit or unavailable */
#define DEM_J1939MIL_CONT_UNAVAILABLE  0x00C0U

#if (defined DEM_J1939RSIL_ACTIVE)
#error DEM_J1939RSIL_ACTIVE already defined
#endif
/** \brief Red stop indicator lamp active */
#define DEM_J1939RSIL_ACTIVE  0x1000U

#if (defined DEM_J1939RSIL_FAST_FLASH)
#error DEM_J1939RSIL_FAST_FLASH already defined
#endif
/** \brief Red stop indicator lamp is fast flashing */
#define DEM_J1939RSIL_FAST_FLASH  0x0010U

#if (defined DEM_J1939RSIL_CONT_UNAVAILABLE)
#error DEM_J1939RSIL_CONT_UNAVAILABLE already defined
#endif
/** \brief Red stop indicator lamp is continuously lit or unavailable */
#define DEM_J1939RSIL_CONT_UNAVAILABLE  0x0030U

#if (defined DEM_J1939AWIL_ACTIVE)
#error DEM_J1939AWIL_ACTIVE already defined
#endif
/** \brief Amber warning indicator lamp active */
#define DEM_J1939AWIL_ACTIVE  0x0400U

#if (defined DEM_J1939AWIL_FAST_FLASH)
#error DEM_J1939AWIL_FAST_FLASH already defined
#endif
/** \brief Amber warning indicator lamp is fast flashing */
#define DEM_J1939AWIL_FAST_FLASH  0x0004U

#if (defined DEM_J1939AWIL_CONT_UNAVAILABLE)
#error DEM_J1939AWIL_CONT_UNAVAILABLE already defined
#endif
/** \brief Amber warning indicator lamp is continuously lit or unavailable */
#define DEM_J1939AWIL_CONT_UNAVAILABLE  0x000CU

#if (defined DEM_J1939PIL_ACTIVE)
#error DEM_J1939PIL_ACTIVE already defined
#endif
/** \brief Protection indicator lamp active */
#define DEM_J1939PIL_ACTIVE  0x0100U

#if (defined DEM_J1939PIL_FAST_FLASH)
#error DEM_J1939PIL_FAST_FLASH already defined
#endif
/** \brief Protection indicator lamp is fast flashing */
#define DEM_J1939PIL_FAST_FLASH  0x0001U

#if (defined DEM_J1939PIL_CONT_UNAVAILABLE)
#error DEM_J1939PIL_CONT_UNAVAILABLE already defined
#endif
/** \brief Protection indicator lamp is continuously lit or unavailable */
#define DEM_J1939PIL_CONT_UNAVAILABLE  0x0003U

/*-------------[Dem_J1939DcmSetClearFilterType]-----------------------------*/
/** \brief The type to distinguish which DTCs gets cleared */
/* !LINKSTO Dem.ASR422.SWS_Dem_00946,1 */
typedef uint8 Dem_J1939DcmSetClearFilterType;

#if (defined DEM_J1939DTC_CLEAR_ALL)
#error DEM_J1939DTC_CLEAR_ALL already defined
#endif
/** \brief active DTCs to be cleared */
#define DEM_J1939DTC_CLEAR_ALL  0x00U

#if (defined DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE)
#error DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE already defined
#endif
/** \brief previously active DTCs to be cleared */
#define DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE  0x01U

/*-------------[Dem_J1939DcmSetFreezeFrameFilterType]-----------------------------*/
/* !LINKSTO Dem.ASR422.SWS_Dem_00947,1 */
typedef uint8 Dem_J1939DcmSetFreezeFrameFilterType;

#if (defined DEM_J1939DCM_FREEZEFRAME)
#error DEM_J1939DCM_FREEZEFRAME already defined
#endif
/** \brief FreezeFrame (DM04) */
#define DEM_J1939DCM_FREEZEFRAME  0x00U

#if (defined DEM_J1939DCM_EXPANDED_FREEZEFRAME)
#error DEM_J1939DCM_EXPANDED_FREEZEFRAME already defined
#endif
/** \brief ExpandedFreezeFrame(DM25) */
#define DEM_J1939DCM_EXPANDED_FREEZEFRAME  0x01U

#if (defined DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME)
#error DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME already defined
#endif
/** \brief SPNs in Expanded-FreezeFrame (DM24) */
#define DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME  0x02U

#endif /* (DEM_J1939_SUPPORT == STD_ON) */

#if (DEM_INCLUDE_RTE == STD_OFF)

/*------------------[Dem_DTCStatusMaskType]---------------------------------*/
#ifndef RTE_TYPE_Dem_DTCStatusMaskType
#define RTE_TYPE_Dem_DTCStatusMaskType
/** \brief Used to set the current status
 **
 ** value: - 0x00 ... 0xFF: Match DTCStatusMask as defined in ISO14229-1
 **
 ** \note This type has been replaced with compatible base type (uint8) in
 ** AR3.1 Dem SWS, however it has been retained here for compatibility. */
typedef uint8 Dem_DTCStatusMaskType;
#endif

/*------------------[Dem_RatioIdType]---------------------------------------*/
#ifndef RTE_TYPE_Dem_RatioIdType
#define RTE_TYPE_Dem_RatioIdType
/** \brief OBD specific ratio Id (related to a specific event, a FID, and an
 ** IUMPR group). This type depends on the Dem configuration.
 **
 ** Note: The size of this type is implementation specific and
 ** can be uint8 or uint16. */
typedef uint8 Dem_RatioIdType;
#endif

/*------------------[Dem_InitMonitorReasonType]-----------------------------*/
#ifndef RTE_TYPE_Dem_InitMonitorReasonType
#define RTE_TYPE_Dem_InitMonitorReasonType
/** \brief (Re-)Initialization reason returned by the
 ** callback @<Module@>_DemInitMonitorFor@<EventName@>() */
typedef uint8 Dem_InitMonitorReasonType;
#endif

#ifndef DEM_INIT_MONITOR_CLEAR
/** \brief Event was cleared and all internal values and states are reset */
#define DEM_INIT_MONITOR_CLEAR    0x01U
#endif

#ifndef DEM_INIT_MONITOR_RESTART
/** \brief Operation cycle of the event was (re-)started */
#define DEM_INIT_MONITOR_RESTART  0x02U
#endif

#if (DEM_INIT_MONITOR_REENABLED_SUPPORT == STD_ON)
#ifndef DEM_INIT_MONITOR_REENABLED
/** \brief Enable condition or DTC setting re-enabled */
#define DEM_INIT_MONITOR_REENABLED          0x03U
#endif

#ifndef DEM_INIT_MONITOR_STORAGE_REENABLED
/** \brief Storage condition re-enabled */
#define DEM_INIT_MONITOR_STORAGE_REENABLED  0x04U
#endif
#endif

#endif /* DEM_INCLUDE_RTE == STD_OFF */

/*------------------[Dem_DataElementDataType]-----------------------------------------*/

/** \brief Used to specify the data type used for External Data Elements */
typedef uint8 Dem_DataElementDataType;

#ifndef DEM_READ_DATA_CS_PROTOTYPE_BASIC
/** \brief uint8 data type used for CS interfaces */
#define DEM_READ_DATA_CS_PROTOTYPE_BASIC   0x00U
#endif

#ifndef DEM_READ_DATA_CS_PROTOTYPE_WITH_EVENTID
/** \brief data type used for CS interfaces with EventId */
#define DEM_READ_DATA_CS_PROTOTYPE_WITH_EVENTID 0x01U
#endif

#ifndef DEM_READ_DATA_SR_DATA_TYPE_UINT8
/** \brief uint8 data type used for SR interfaces */
#define DEM_READ_DATA_SR_DATA_TYPE_UINT8        0x02U
#endif

#ifndef DEM_READ_DATA_SR_DATA_TYPE_UINT16
/** \brief uint16 data type used for SR interfaces */
#define DEM_READ_DATA_SR_DATA_TYPE_UINT16       0x03U
#endif

#ifndef DEM_READ_DATA_SR_DATA_TYPE_UINT32
/** \brief uint32 data type used for SR interfaces */
#define DEM_READ_DATA_SR_DATA_TYPE_UINT32       0x04U
#endif

#if (DEM_SR_SUPPORT == STD_ON)

/*------------------[Dem_SRDataElementEndiannessType]--------------------------*/

/* !LINKSTO dsn.Dem.SRDataElementClass.Endianness.Macro,1 */
/** \brief Used to specify the endianness used for SR interfaces */
typedef uint8 Dem_SRDataElementEndiannessType;

#ifndef DEM_BIG_ENDIAN
/** \brief big endian endianness type used for SR interfaces */
#define DEM_BIG_ENDIAN              0x00U
#endif

#ifndef DEM_LITTLE_ENDIAN
/** \brief little endian endianness type used for SR interfaces */
#define DEM_LITTLE_ENDIAN           0x01U
#endif

#ifndef DEM_OPAQUE
/** \brief opaque endianness type used for SR interfaces */
#define DEM_OPAQUE                  0x02U
#endif
#endif /* DEM_SR_SUPPORT == STD_ON */

/*------------------[>Dem return types<]------------------------------------*/

/*------------------[Dem_ReturnSetFilterType]-------------------------------*/

/** \brief Used to return the status of (re-)setting a specific filter */
typedef uint8 Dem_ReturnSetFilterType;

#if (defined DEM_FILTER_ACCEPTED)
#error DEM_FILTER_ACCEPTED already defined
#endif
/** \brief Filter was accepted */
#define DEM_FILTER_ACCEPTED         0x00U

#if (defined DEM_WRONG_FILTER)
#error DEM_WRONG_FILTER already defined
#endif
/** \brief Wrong filter selected */
#define DEM_WRONG_FILTER            0x01U

#if (DEM_J1939_SUPPORT == STD_ON)
/*------------------[Dem_ReturnGetNextFilteredElementType]----------------------*/

/* !LINKSTO SWS_Dem_00957,1 */
/** \brief Used to return the status of the Dem_J1939DcmGetNextFilteredDTC<...> interfaces */
typedef uint8 Dem_ReturnGetNextFilteredElementType;

#if (defined DEM_FILTERED_OK)
#error DEM_FILTERED_OK already defined
#endif
/** \brief Returned next filtered element */
#define DEM_FILTERED_OK                0x00U

#if (defined DEM_FILTERED_NO_MATCHING_ELEMENT)
#error DEM_FILTERED_NO_MATCHING_ELEMENT already defined
#endif
/** \brief No further element (matching the filter criteria) found */
#define DEM_FILTERED_NO_MATCHING_ELEMENT   0x01U

#if (defined DEM_FILTERED_PENDING)
#error DEM_FILTERED_PENDING already defined
#endif
/** \brief The requested value is calculated asynchronously and currently not
 ** available. The caller can retry later.
 ** Only used by asynchronous interfaces.
 */
#define DEM_FILTERED_PENDING           0x02U

#if (defined DEM_FILTERED_BUFFER_TOO_SMALL)
#error DEM_FILTERED_BUFFER_TOO_SMALL already defined
#endif
/** \brief Buffer in the BufSize parameter is not huge enough */
#define DEM_FILTERED_BUFFER_TOO_SMALL   0x03U
#endif /* DEM_J1939_SUPPORT == STD_ON */

/*-------------------[Dem_ReturnGetNumberOfFilteredDTCType]-----------------*/

/** \brief Used to return the status of Dem_GetNumberOfFilteredDTC() */
typedef uint8 Dem_ReturnGetNumberOfFilteredDTCType;

#if (defined DEM_NUMBER_OK)
#error DEM_NUMBER_OK already defined
#endif
/** \brief Getting number of filtered DTCs was successful */
#define DEM_NUMBER_OK                0x00U

#if (defined DEM_NUMBER_FAILED)
#error DEM_NUMBER_FAILED already defined
#endif
/** \brief Getting number of filtered DTCs failed */
#define DEM_NUMBER_FAILED            0x01U

#if (defined DEM_NUMBER_PENDING)
#error DEM_NUMBER_PENDING already defined
#endif
/** \brief Getting number of filtered DTCs is pending */
#define DEM_NUMBER_PENDING           0x02U

/*------------------[Dem_ReturnClearDTCType]--------------------------------*/

/** \brief Used to return the status of Dem_ClearDTC() */
typedef uint8 Dem_ReturnClearDTCType;

#if (defined DEM_CLEAR_OK)
#error DEM_CLEAR_OK already defined
#endif
/** \brief DTC successfully cleared */
#define DEM_CLEAR_OK                  0x00U

#if (defined DEM_CLEAR_WRONG_DTC)
#error DEM_CLEAR_WRONG_DTC already defined
#endif
/** \brief DTC value not existing (in this format) */
#define DEM_CLEAR_WRONG_DTC           0x01U

#if (defined DEM_CLEAR_WRONG_DTCORIGIN)
#error DEM_CLEAR_WRONG_DTCORIGIN already defined
#endif
/** \brief Wrong DTC origin */
#define DEM_CLEAR_WRONG_DTCORIGIN     0x02U

#if (defined DEM_ASR42_CLEAR_FAILED)
#error DEM_ASR42_CLEAR_FAILED already defined
#endif
/** \brief DTC clearing failed */
#define DEM_ASR42_CLEAR_FAILED        0x03U

#if (defined DEM_CLEAR_PENDING)
#error DEM_CLEAR_PENDING already defined
#endif
/** \brief Clearing of DTC is pending */
#define DEM_CLEAR_PENDING             0x04U

#if (defined DEM_ASR42_CLEAR_BUSY)
#error DEM_ASR42_CLEAR_BUSY already defined
#endif
/** \brief Another client is currently clearing DTCs. The requested operation
 ** will not be started */
#define DEM_ASR42_CLEAR_BUSY          0x05U

#if (defined DEM_ASR42_CLEAR_MEMORY_ERROR)
#error DEM_ASR42_CLEAR_MEMORY_ERROR already defined
#endif
/** \brief An error occurred during erasing a memory location */
#define DEM_ASR42_CLEAR_MEMORY_ERROR  0x06U

/*------------------[>NvRAM-related types<]---------------------------------*/

/*---------[Dem_FFIdxType]--------------------------------------------------*/

/** \brief Number of freeze frames type
 **
 ** Note: The size of this type is defined by ISO14229-1.
 ** (FF RecordNumber = 0x01 ... 0xFE)
 **
 ** NumFF and MaxNumFF variables are also of this type.
 */
typedef uint8 Dem_FFIdxType;

/*------------------[definitions for Entry Types]---------------------------*/

/** \brief Entry data type, dynamic part of the error memory */
typedef uint8 Dem_EntryDataType;

/** \brief Entry data size type */
typedef uint16 Dem_SizeEntryDataType;

#if (DEM_OBD_Support == STD_ON)
/** \brief Data type for state of the readiness group */
typedef uint32 Dem_ReadinessEnableStatusType;
#endif

/*------------------[Dem_IndicatorCycleCounterType]-------------------------*/

/** \brief Indicator cycle counter Type */
typedef uint8 Dem_IndicatorCycleCounterType;

/*------------------[Dem_FailureCycleCounterType]---------------------------*/

/** \brief Type for value of Event failure cycles */
typedef uint8 Dem_FailureCycleCounterType;

/*------------------[Dem_StatusCounterType]---------------------------------*/

/** \brief Type for value of Status counters */
typedef uint8 Dem_StatusCounterType;

/*-----------------[Dem_DebounceCounterStatusType]--------------------------*/

/** \brief Internal counter value for counter based debouncing algorithm */
typedef sint16 Dem_DebounceCounterStatusType;

/*------------------[Dem_EntryStatusType]-----------------------------------*/

/** \brief Type concerning immediate storage */
typedef enum
{
  /** \brief entry was not changed and needs not to be stored persistently */
  DEM_ENTRY_UNCHANGED,
  /** \brief entry was changed and has to be stored persistently during shutdown */
  DEM_ENTRY_CHANGED,
  /** \brief entry was changed and has to be stored persistent immediately */
  DEM_ENTRY_STORE_IMMEDIATELY,
  /** \brief entry was cleared and has to be stored persistent immediately */
  DEM_ENTRY_CLEAR_IMMEDIATELY
} Dem_EntryStatusType;

/*---------------------[Dem_OccOrderType]-----------------------------------*/

/** \brief Type concerning occurrence order which is used for event
 ** displacement */
typedef uint16 Dem_OccOrderType;

/*---------------------[Dem_EvSignificanceType]-----------------------------------*/

/** \brief Type concerning the significance level per event which
 ** can be mapped as an external data */
typedef uint8 Dem_EvSignificanceType;

#if (defined DEM_EVENT_SIGNIFICANCE_OCCURRENCE)
#error DEM_EVENT_SIGNIFICANCE_OCCURRENCE already defined
#endif
/** \brief Offset of DTC value in 1st element of event desc */
#define DEM_EVENT_SIGNIFICANCE_OCCURRENCE           0U

#if (defined DEM_EVENT_SIGNIFICANCE_FAULT)
#error DEM_EVENT_SIGNIFICANCE_FAULT already defined
#endif
/** \brief Offset of DTC value in 1st element of event desc */
#define DEM_EVENT_SIGNIFICANCE_FAULT                1U

/*------------------[Dem_EventMemoryEntryType]------------------------------*/

/** \brief Event memory entry type, static part of the error memory */
typedef struct
{
  /* this structure's members ordering wasn't updated to comply to the
   * BSWM_C_0005 rule ("Members of structs SHALL be sorted in descending
   * alignment requirement order"), due to backward compatibility issue;
   * Dem_NvDataType structure has members of this type.*/
  Dem_EventIdType             EventId;           /**< Event Id of the entry */
#if (DEM_MULTI_EVENT_TRIGGERING_COUNT > 0U)
  Dem_EventIdType             RootId;             /**< Root Id of the entry */
#endif
  uint8                       OccCtr;
                               /**< Occurrence of event also used for NumFF */
  Dem_OccOrderType            OccOrder;
                     /**< Occurrence order of the event in the event memory */
#if (DEM_EXTENDED_DATA_CAPTURE == DEM_TRIGGER_ON_FDC_THRESHOLD)
  uint8                       SI30;              /**< DTC status indicators */
  uint8                       FDC12;
                      /**< Maximum fault detection counter since last clear */
  uint8                       OCC1;
                    /**< operation cycles since last unconfirmedDTC counter */
  uint8                       OCC2;
                                          /**< unconfirmedDTC aging counter */
  uint8                       OCC3;
                   /**< operation cycles since first unconfirmedDTC counter */
  uint8                       OCC4;
                                /**< unconfirmedDTC operation cycle counter */
  /* !LINKSTO dsn.Dem.OBD.OCC5.DemEventMemoryEntryType,1 */
#if ( DEM_VCC_OCC5_USED == STD_ON )
  uint8                       OCC5;
       /**< number of WCUs cycles where event is reported as active counter */
#endif
  uint8                       OCC6;
                            /**< consecutive failed operation cycle counter */
  uint8                       OCC7;
                      /**< qualified/unconfirmedDTC operation cycle counter */
#endif
#if (DEM_OPCYCLE_CTR_CSLF_SUPPORT == STD_ON)
  /* !LINKSTO Dem.OperationCycleCounters.CSLF.CounterSize,1 */
  uint8                       CSLF;
                  /**< operation cycles since FDC last reached +127 counter */
#endif
#if (DEM_OPCYCLE_CTR_CSFF_SUPPORT == STD_ON)
  /* !LINKSTO Dem.OperationCycleCounters.CSFF.CounterSize,1 */
  uint8                       CSFF;
                 /**< operation cycles since FDC first reached +127 counter */
#endif
#if (DEM_NUM_FAILURECYCLES > 0U)
  Dem_FailureCycleCounterType EventFailureCycleCounter;
                                        /**< Failure cycle counter of Event */
#endif
  Dem_SizeEntryDataType       EntryDataPos;
                               /**< Start of extended and freeze frame data */
#if (DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON)
  Dem_EntryStatusType         EntryStatus;
                                    /**< Entry status for immediate storage */
#endif
                                /**< Start of development freeze frame data */
#if (DEM_DEV_FREEZE_FRAME_USED == STD_ON)
  /* !LINKSTO dsn.Dem.DevFF.DevFFEntryIdx,1 */
  uint8                       DevFFEntryIdx[DEM_NUM_OF_DEVFF_KIND];
#endif
} Dem_EventMemoryEntryType;

#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
/* !LINKSTO dsn.Dem.OBD.Dem_SingleOBDEventMemoryEntryType,1 */
/** \brief Single Event memory entry type, static part of the error memory */
typedef struct
{
  Dem_EventIdType             EventId;
  /**< event Id of the entry */
  Dem_EntryDataType           OBDFFData[DEM_SIZE_OBDFF_DATA];
  /* !LINKSTO dsn.Dem.OBD.OBDFFData,1 */
  /**< size in bytes of all data elements contained in the OBD FF */
} Dem_SingleOBDEventMemoryEntryType;
#endif /* (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE) */

#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)
/** \brief Multiple Event memory entry type, static part of the error memory */
typedef struct
{
  Dem_OccOrderType            OBDOccOrder;
  /**< occurrence order of the OBD event in the primary event memory */
  Dem_EntryDataType           OBDFFData[DEM_SIZE_OBDFF_DATA];
  /**< size in bytes of all data elements contained in the OBD FF */
} Dem_MultipleOBDEventMemoryEntryType;
#endif /* (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE) */

/*------------------[types resulting into NVRAM blocks]---------------------*/

/* !LINKSTO dsn.Dem.NvMDataStructure,1 */
/** \brief Structure containing all non-volatile data
 **
 ** If no immediate storage is used, NvData consists of DTCStatusMask
 ** and event memory entries.
 ** If immediate storage is used, NvData consists of DTCStatusMask, but no
 ** event memory entries. The event memory data will be stored separately
 ** via gate-entry if immediate storage is enabled for any event.
 **
 ** Immediate storage = OFF:
 ** - NvM block DEM_NVM_BLOCK_ID_DEFAULT stores all non-volatile data
 **
 ** Immediate storage = ON:
 ** - NvM block DEM_NVM_BLOCK_ID_DEFAULT stores DTCStatusMask
 ** - NvM block DEM_NVM_BLOCK_ID_PRIMARY stores entry-related data of primary
 ** memory
 ** - NvM block DEM_NVM_BLOCK_ID_SECONDARY stores entry-related data of secondary
 ** memory
 ** - NvM block DEM_NVM_BLOCK_ID_MIRROR stores entry-related data of mirror
 ** memory
 ** - NvM block DEM_NVM_BLOCK_ID_PERMANENT stores entry-related data of permanent
 ** memory
 **
 ** \note configuration dependent type
 */
typedef struct
{
  /* this structure's members ordering wasn't updated to comply to the
   * BSWM_C_0005 rule ("Members of structs SHALL be sorted in descending
   * alignment requirement order"), due to backward compatibility issue */
#if (DEM_NVRAM_CONFIG_SIGNATURE_USED == STD_ON)
  uint32 NvRamBlockSignature;
    /**< Signature value of the configuration of the non-volatile data.
         This value is compared with the signature value of the configuration
         of the execution code stored in the macro DEM_CONFIG_NVRAM_SIGNATURE */
#endif

  Dem_DTCStatusMaskType    DTCStatusMask[DEM_NUMBER_OF_EVENTS];
    /**< Status of each DTC. In case Nv storage of TF bit is disabled TF bit 0
         shall not be used */
  uint8                    EvMemOvfIndFlags;
    /**< Bit-field to store the overflow indication status of all event
         memorys (primary, secondary, mirror, permanent). One bit will be used
         per memory. */

  /* !LINKSTO Dem_OBD_0005,1 */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U)
  /* !LINKSTO Dem_OBD_0013,1 */
  uint8                    PfcStatus;             /**< Permanent fault code */
#endif

#if (DEM_NUM_INDICATOR_LINKS > 0U)
  Dem_IndicatorCycleCounterType
    IndicatorCycleCounter[DEM_INDICATOR_MULTIPLICITY]
                         [DEM_NUM_INDICATOR_COUNTER];
                                               /**< Indicator cycle counter */
  uint8 IndicatorCycleCounterRole[DEM_INDICATOR_MULTIPLICITY]
                                 [DEM_NUM_INDICATOR_COUNTER_BIT];
                        /**< Flags to indicate indicator cycle counter role */
  uint8 IndicatorCycleCounterWIRStatus[DEM_INDICATOR_MULTIPLICITY]
                                      [DEM_NUM_INDICATOR_COUNTER_BIT];
                  /**< Flags to indicate indicator cycle counter WIR status */
#endif
#if (DEM_OPCYC_NVSTORAGE == STD_ON)
  Dem_OperationCycleStateType OpCycleState[DEM_NUM_OPCYCLES];
                                                /**< Operation Cycle states */
#if (DEM_NUM_DEBOUNCE_COUNTER_PERM > 0U)
  Dem_DebounceCounterStatusType
    DebounceCounterStatus[DEM_NUM_DEBOUNCE_COUNTER_PERM];
      /**< Debouncing info for persistently stored counter debounced events */
#endif
#endif
#if (DEM_USE_IMMEDIATE_NV_STORAGE == STD_OFF)

  Dem_EventMemoryEntryType EventMemoryPrimary[
    DEM_MAX_NUMBER_EVENT_ENTRY_PRI];              /**< Primary event memory */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_SEC != 0U)
  /* !LINKSTO Dem162_Implicit1,1 */
  Dem_EventMemoryEntryType EventMemorySecondary[
    DEM_MAX_NUMBER_EVENT_ENTRY_SEC];            /**< Secondary event memory */
#endif
#if (DEM_MAX_NUMBER_EVENT_ENTRY_MIR != 0U)
  /* !LINKSTO Dem162_Implicit1,1 */
  Dem_EventMemoryEntryType EventMemoryMirror[
    DEM_MAX_NUMBER_EVENT_ENTRY_MIR];               /**< Mirror event memory */
#endif
#if (DEM_SIZE_ENTRY_DATA_PRIMARY != 0U)
  Dem_EntryDataType        EntryDataPrimary[DEM_SIZE_ENTRY_DATA_PRIMARY];
                                               /**< Primary entry data area */
  Dem_SizeEntryDataType    EntryDataFreePosPrimary;
                                      /**< Primary entry data free position */
#endif
#if (DEM_SIZE_ENTRY_DATA_SECONDARY != 0U)
  Dem_EntryDataType        EntryDataSecondary[DEM_SIZE_ENTRY_DATA_SECONDARY];
                                             /**< Secondary entry data area */
  Dem_SizeEntryDataType    EntryDataFreePosSecondary;
                                    /**< Secondary entry data free position */
#endif
#if (DEM_SIZE_ENTRY_DATA_MIRROR != 0U)
  Dem_EntryDataType        EntryDataMirror[DEM_SIZE_ENTRY_DATA_MIRROR];
                                                /**< Mirror entry data area */
  Dem_SizeEntryDataType    EntryDataFreePosMirror;
                                       /**< Mirror entry data free position */
#endif
#endif /* DEM_USE_IMMEDIATE_NV_STORAGE == STD_OFF */

#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_SINGLE)
/* !LINKSTO Dem291,1 */
/* !LINKSTO dsn.Dem.OBD.OBDFreezeFrame.GlobalOBDFF,1 */
/* !LINKSTO dsn.Dem.OBD.SingleOBDEventMemoryEntry.Dem_NvDataType,1 */
  Dem_SingleOBDEventMemoryEntryType SingleOBDEventMemoryEntry;

#elif (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)

#if (DEM_USE_IMMEDIATE_NV_STORAGE == STD_OFF)
/* !LINKSTO dsn.Dem.OBD.OBDFreezeFrame.MultipleOBDFF,1 */
  Dem_MultipleOBDEventMemoryEntryType EntryOBDFF[DEM_MAX_NUMBER_EVENT_ENTRY_PRI];
  /**< Array containing the OBD Freeze Frame data and the occurrence order of the event
    entries from the primary memory */
#endif /* (DEM_USE_IMMEDIATE_NV_STORAGE == STD_OFF) */
#endif /* (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE) */

#if (DEM_OBD_CENTRALIZED_PID21_HANDLING == STD_ON)
  uint32 TicksTraveledWhileMILIsActivated;
                              /**< distance traveled while MIL is activated */
  uint8 NrWUCsWhileMILInactive;
  /**< number of warm-up cycles that happened since the MIL was deactivated */
  Dem_IndicatorStatusType MILPrevStatusOnlyOnOffRelevance;
                                            /**< previous status of the MIL */
#endif

#if (DEM_OBD_CENTRALIZED_PID31_HANDLING == STD_ON)
  uint32 TicksTraveledSinceDTCsCleared;
      /**< Distance traveled since all DTCs were cleared (used by PID $31) */
#endif

#if (defined DEM_OPCYC_WARMUP_ID)
  uint8 OBDWarmupCycleCtr;
              /**< Number of OBD warm-up cycles since all DTCs were cleared */
#endif

#if (DEM_OBD_Support == STD_ON)
  /* !LINKSTO dsn.Dem.OBDPidSupport.Dem_ReadinessEnableStatus_01,1
   */
  /* !LINKSTO dsn.Dem.OBDPidSupport.Dem_ReadinessEnableStatus_02,2
   */
  Dem_ReadinessEnableStatusType ReadinessEnableStatus;
                              /**< Readiness status of the Readiness Groups */
#endif

/* !LINKSTO dsn.Dem.DevFF.NvM,1 */
#if (DEM_MAX_NUMBER_DEVFFA_ENTRY > 0U)
  /* !LINKSTO dsn.Dem.DevFF.DevFFXEventId,1 */
  Dem_EventIdType DevFFAEventId[DEM_MAX_NUMBER_DEVFFA_ENTRY];
                        /**< EventIds of stored Development Freeze Frames A */

  /* !LINKSTO dsn.Dem.DevFF.DevFFAData,2 */
  uint8 DevFFAData[DEM_MAX_NUMBER_DEVFFA_ENTRY * DEM_DEV_FFA_MAX_CLASS_SIZE];
                            /**< Data of stored Development Freeze Frames A */
#endif

#if (DEM_MAX_NUMBER_DEVFFB_ENTRY > 0U)
  /* !LINKSTO dsn.Dem.DevFF.DevFFXEventId,1 */
  Dem_EventIdType DevFFBEventId[DEM_MAX_NUMBER_DEVFFB_ENTRY];
                        /**< EventIds of stored Development Freeze Frames B */

  /* !LINKSTO dsn.Dem.DevFF.DevFFBData,2 */
  uint8 DevFFBData[DEM_MAX_NUMBER_DEVFFB_ENTRY * DEM_DEV_FFB_MAX_CLASS_SIZE];
                            /**< Data of stored Development Freeze Frames B */
#endif

#if ((DEM_NUM_CMB_DTCS > 0U) && (DEM_NUM_FAILURECYCLES > 0U))
  Dem_FailureCycleCounterType CmbEvFailureCycleCounter[DEM_NUM_CMB_EVENTS];
                         /**< FailureCycle counters for all combined events */
#endif

#if (DEM_RESET_CONFIRMED_BIT_ON_OVERFLOW_SUPPORT_FAULT_CONFIRMATION == STD_ON)
  Dem_StatusCounterType StatusCounter[DEM_STATUS_COUNTER_SIZE];
                                     /**< Failure/aging counters for events */
#endif

#if (DEM_GET_DTCBYOCCURRENCETIME_SUPPORT == STD_ON)
  Dem_EventIdType OccurKindEventId[DEM_MAXNUM_OCCURRENCE_KINDS];
                              /**< Stored EventIds of DTC by OccurrenceTime */
#endif

#if (DEM_OPCYC_OBD_DCY_USED == STD_ON)
  /* !LINKSTO Dem.ASR440.SWS_Dem_01317,1 */
  boolean Dem_OpCycleDCYIsQualified;
                              /**< qualification status of driving cycle */
#endif
} Dem_NvDataType;

/*------------------[fragments to build up further gate-entry types]--------*/

/* These fragment-type definitions are used to build up similar types based on
 * a class-concept, but are not intended to be used by the implementation
 * directly. */

/** \brief gate-entry base fragment structure. This type definition represents a
 ** "fragment-type" of the full type Dem_GateEntryBaseType. This type definition
 ** shall only be used locally to define full-types. This type definition shall
 ** not be used in source code as defining a full-type variable definition. */
typedef struct
{
  /* this structure's members ordering wasn't updated to comply to the
   * BSWM_C_0005 rule ("Members of structs SHALL be sorted in descending
   * alignment requirement order"), due to backward compatibility issue */
  Dem_EventIdType             EventId;           /**< Event Id of the entry */
#if (DEM_MULTI_EVENT_TRIGGERING_COUNT > 0U)
  Dem_EventIdType             RootId;             /**< Root Id of the entry */
#endif
  Dem_FFIdxType               OccCtr;
                               /**< Occurrence of Event also used for NumFF */
  Dem_OccOrderType            OccOrder;
                     /**< Occurrence order of the event in the event memory */
#if (DEM_EXTENDED_DATA_CAPTURE == DEM_TRIGGER_ON_FDC_THRESHOLD)
  uint8                       SI30;              /**< DTC status indicators */
  uint8                       FDC12;
                      /**< Maximum fault detection counter since last clear */
  uint8                       OCC1;
                    /**< operation cycles since last unconfirmedDTC counter */
  uint8                       OCC2;
                                          /**< unconfirmedDTC aging counter */
  uint8                       OCC3;
                   /**< operation cycles since first unconfirmedDTC counter */
  uint8                       OCC4;
                                /**< unconfirmedDTC operation cycle counter */
  /* !LINKSTO dsn.Dem.OBD.OCC5.DemGateEntryBaseFragment,1 */
#if ( DEM_VCC_OCC5_USED == STD_ON )
  uint8                       OCC5;
       /**< number of WCUs cycles where event is reported as active counter */
#endif
  uint8                       OCC6;
                            /**< consecutive failed operation cycle counter */
  uint8                       OCC7;
                      /**< qualified/unconfirmedDTC operation cycle counter */
#endif
#if (DEM_NUM_FAILURECYCLES > 0U)
  Dem_FailureCycleCounterType EventFailureCycleCounter;
                                        /**< Failure cycle counter of Event */
#endif
#if (DEM_OPCYCLE_CTR_CSLF_SUPPORT == STD_ON)
  /* !LINKSTO Dem.OperationCycleCounters.CSLF.CounterSize,1 */
  uint8                       CSLF;
                  /**< operation cycles since FDC last reached +127 counter */
#endif
#if (DEM_OPCYCLE_CTR_CSFF_SUPPORT == STD_ON)
  /* !LINKSTO Dem.OperationCycleCounters.CSFF.CounterSize,1 */
  uint8                       CSFF;
                 /**< operation cycles since FDC first reached +127 counter */
#endif
#if (DEM_OPCYC_OBD_DCY_USED == STD_ON) && (DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON)
  boolean                     CDTCFlag;            /**< Confirmation status */
#endif
} Dem_GateEntryBaseFragmentType;

#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)
/** \brief gate-entry OBD freeze frame primary fragment structure. This type definition represents a
 ** "fragment-type" of the full type Dem_NvGateEntryPrimaryDataType. This type definition
 ** shall only be used locally to define full-types. This type definition shall
 ** not be used in source code as defining a full-type variable definition. */
typedef struct
{
  Dem_MultipleOBDEventMemoryEntryType  OBDFF;
  /**< OBD Freeze Frame entry structure */
} Dem_GateEntryOBDFFPrimaryFragmentType;
#endif

/** \brief gate-entry primary data fragment structure. This type definition represents a
 ** "fragment-type" of the full type Dem_NvGateEntryPrimaryDataType. This type definition
 ** shall only be used locally to define full-types. This type definition shall
 ** not be used in source code as defining a full-type variable definition. */
typedef struct
{
  Dem_EntryDataType EntryData[DEM_SIZE_GATE_ENTRY_DATA_PRIMARY];
    /**< Entry data for gate-entry consisting of IntVal area, extended data ,
         freeze frame data and J1939 Freeze frame */
} Dem_GateEntryPrimaryFragmentType;

/** \brief gate-entry secondary data fragment structure. This type definition represents a
 ** "fragment-type" of the full type Dem_NvGateEntrySecondaryDataType. This type
 ** definition shall only be used locally to define full-types. This type definition shall
 ** not be used in source code as defining a full-type variable definition. */
typedef struct
{
  Dem_EntryDataType EntryData[DEM_SIZE_GATE_ENTRY_DATA_SECONDARY];
    /**< Entry data for gate-entry consisting of IntVal area, extended data ,
         freeze frame data and J1939 Freeze frame */
} Dem_GateEntrySecondaryFragmentType;

/** \brief gate-entry mirror data fragment structure. This type definition represents a
 ** "fragment-type" of the full type Dem_NvGateEntryMirrorDataType. This type definition
 ** shall only be used locally to define full-types. This type definition shall
 ** not be used in source code as defining a full-type variable definition. */
typedef struct
{
  Dem_EntryDataType EntryData[DEM_SIZE_GATE_ENTRY_DATA_MIRROR];
   /**< Entry data for gate-entry consisting of IntVal area, extended data ,
         freeze frame data and J1939 Freeze frame */
} Dem_GateEntryMirrorFragmentType;

/*------------------[gate-entry type inheritance structure]-----------------*/

/** \brief gate-entry data void type - used for pointers to gate-entry data.
 ** This type definition represents a "fragment-type" of the full type Dem_GateEntryDataPtrVoidType.
 ** This type definition shall only be used locally to define full-types.
 ** This type definition shall not be used in source code as defining a
 ** full-type variable definition. */
typedef void Dem_GateEntryDataVoidType;

/** \brief gate-entry data void pointer type */
typedef CONSTP2VAR(Dem_GateEntryDataVoidType, DEM_CONST, DEM_VAR_CLEARED)
  Dem_GateEntryDataPtrVoidType;

/** \brief gate-entry base type - used for basic access */
typedef struct
{
  Dem_GateEntryBaseFragmentType      EntryBase;      /**< Basic information */
} Dem_GateEntryBaseType;

/*------------------[types resulting into NVRAM blocks]---------------------*/

/** \brief gate-entry void type - used for pointers to gate entries */
typedef void Dem_NvGateEntryDataVoidType;

/** \brief gate-entry void pointer type */
typedef CONSTP2VAR(Dem_NvGateEntryDataVoidType, DEM_CONST, DEM_VAR_CLEARED)
  Dem_NvGateEntryDataPtrVoidType;

/** \brief Structure containing all non-volatile primary event memory data
 ** for one gate-entry
 **
 ** \note configuration dependent type, derived from GateEntryBaseType
 **/
typedef struct
{
  Dem_GateEntryBaseFragmentType      EntryBase;     /**< Basic information */
  Dem_GateEntryPrimaryFragmentType   EntryPrimary;  /**< Primary entry data */
#if (DEM_OBDFF_SUPPORT_KIND == DEM_OBDFF_MULTIPLE)
  Dem_GateEntryOBDFFPrimaryFragmentType OBDFFEntry; /**< OBDFF information */
#endif
} Dem_NvGateEntryPrimaryDataType;

/** \brief Structure containing all non-volatile secondary event memory data
 ** for one gate-entry
 **
 ** \note configuration dependent type, derived from GateEntryBaseType
 **/
typedef struct
{
  Dem_GateEntryBaseFragmentType      EntryBase;      /**< Basic information */
  Dem_GateEntrySecondaryFragmentType EntrySecondary; /**< Secondary entry data */
} Dem_NvGateEntrySecondaryDataType;

/** \brief Structure containing all non-volatile mirror event memory data
 ** for one gate-entry
 **
 ** \note configuration dependent type, derived from GateEntryBaseType
 **/
typedef struct
{
  Dem_GateEntryBaseFragmentType      EntryBase;      /**< Basic information */
  Dem_GateEntryMirrorFragmentType    EntryMirror;    /**< Mirror entry data */
} Dem_NvGateEntryMirrorDataType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef DEM_TYPES_H */
/*==================[end of file]===========================================*/
