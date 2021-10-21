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
#ifndef DEM_INT_CFG_H
#define DEM_INT_CFG_H

/* This file contains the all module internal configuration declarations for
 * the AUTOSAR module Dem that are target independent. */

[!AUTOSPACING!]
[!INCLUDE "../../generate_common/Dem_Include.m"!]
[!//
/*==================[includes]==============================================*/
/* !LINKSTO dsn.Dem.IncludeStr,1 */

/*------------------[callback header file inclusions]-----------------------*/

[!LOOP "DemGeneral/DemHeaderFileInclusion/*"!]
  [!WS "0"!]#include <[!"."!]>
[!ENDLOOP!]

/*==================[macros]================================================*/

/*------------------[General]-----------------------------------------------*/

#if (defined DEM_USE_AGING)
#error DEM_USE_AGING already defined
#endif
/** \brief Macro for Enabling/Disabling aging functionality */
[!IF "count(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemAgingAllowed = 'true']) > 0"!]
  [!WS "0"!]#define DEM_USE_AGING   STD_ON
[!ELSE!][!//
  [!WS "0"!]#define DEM_USE_AGING   STD_OFF
[!ENDIF!]

#if (defined DEM_USE_INT_VAL_AREA)
#error DEM_USE_INT_VAL_AREA already defined
#endif
/** \brief Macro for Enabling/Disabling internal value area
 **
 ** In the current implementation the existance of the internal value area
 ** depends on aging only. */
#define DEM_USE_INT_VAL_AREA   DEM_USE_AGING

#if (defined DEM_USE_MAPPED_INT_DATA_ELEMENTS)
#error DEM_USE_MAPPED_INT_DATA_ELEMENTS already defined
#endif
/** \brief Macro for Enabling/Disabling internal data elements mapping */
[!/* note: internal data elements can not be configured for FFs */!][!//
[!IF "count(node:refs(node:refs(DemGeneral/DemExtendedDataClass/*/DemExtendedDataRecordClassRef/*)/DemDataElementClassRef/*)[. = 'DemInternalDataElementClass']) > 0"!]
  [!WS "0"!]#define DEM_USE_MAPPED_INT_DATA_ELEMENTS    STD_ON
[!ELSE!][!//
  [!WS "0"!]#define DEM_USE_MAPPED_INT_DATA_ELEMENTS    STD_OFF
[!ENDIF!]




/*------------------[Defensive programming]---------------------------------*/
[!SELECT "DemDefensiveProgramming"!][!//

#if (defined DEM_DEFENSIVE_PROGRAMMING_ENABLED)
#error DEM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define DEM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../DemGeneral/DemDevErrorDetect  = 'true') and (DemDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DEM_PRECONDITION_ASSERT_ENABLED)
#error DEM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define DEM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../DemGeneral/DemDevErrorDetect  = 'true') and (DemDefProgEnabled = 'true') and (DemPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DEM_POSTCONDITION_ASSERT_ENABLED)
#error DEM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define DEM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../DemGeneral/DemDevErrorDetect  = 'true') and (DemDefProgEnabled = 'true') and (DemPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DEM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error DEM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define DEM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../DemGeneral/DemDevErrorDetect  = 'true') and (DemDefProgEnabled = 'true') and (DemUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DEM_INVARIANT_ASSERT_ENABLED)
#error DEM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define DEM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../DemGeneral/DemDevErrorDetect  = 'true') and (DemDefProgEnabled = 'true') and (DemInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DEM_STATIC_ASSERT_ENABLED)
#error DEM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define DEM_STATIC_ASSERT_ENABLED           [!//
[!IF "(../DemGeneral/DemDevErrorDetect  = 'true') and (DemDefProgEnabled = 'true') and (DemStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//


[!IF "DemGeneral/DemRteUsage = 'false'"!]
/** \brief This macro specifies the user-defined DTCOrigin value to be used by Dcm or
 **  SW-Cs for accessing the Dem internal secondary event memory */
  [!IF "node:exists(DemGeneral/DemUserDefMemoryId)"!]
    [!WS "0"!]#define DEM_DTC_ORIGIN_USERDEFINED_MEMORY_MAPPED_TO_SECONDARY [!"translate(num:inttohex((DemGeneral/DemUserDefMemoryId + 256), 3), 'abcdef', 'ABCDEF')"!]U
  [!ELSE!][!//
    [!WS "0"!]#define DEM_DTC_ORIGIN_USERDEFINED_MEMORY_MAPPED_TO_SECONDARY 0U
  [!ENDIF!]
[!ENDIF!]

/*------------------[Callback macros]---------------------------------------*/
[!VAR "GenericCtr" = "count(DemConfigSet/*/DemEventParameter/*/DemCallbackInitMForE)"!]

#if (defined DEM_CB_TABLE_INIT_MONITOR_SIZE)
#error DEM_CB_TABLE_INIT_MONITOR_SIZE already defined
#endif
/** \brief Size of the generic callback function pointer table for InitMonitor */
#define DEM_CB_TABLE_INIT_MONITOR_SIZE [!"num:integer($GenericCtr)"!]U

#if (defined DEM_LOOKUP_TABLE_INIT_MONITOR_SIZE)
#error DEM_LOOKUP_TABLE_INIT_MONITOR_SIZE already defined
#endif
/** \brief Size of the generic lookup table for InitMonitor */
#define DEM_LOOKUP_TABLE_INIT_MONITOR_SIZE [!"num:integer($GenericCtr)"!]U

#if (defined DEM_USE_CB_INIT_MONITOR)
#error DEM_USE_CB_INIT_MONITOR already defined
#endif
/** \brief Macro for Enabling/Disabling init monitor callback functionality */
#if (DEM_CB_TABLE_INIT_MONITOR_SIZE > 0U)
#define DEM_USE_CB_INIT_MONITOR STD_ON
#else
#define DEM_USE_CB_INIT_MONITOR STD_OFF
#endif

[!VAR "CbNameList" = "'#'"!]
[!VAR "CCbCtr" = "0"!]
[!VAR "CLookupCtr" = "0"!]
[!VAR "RteCtr" = "0"!]
[!LOOP "DemConfigSet/*/DemEventParameter/*/DemCallbackEventStatusChanged/*"!]
  [!IF "not(node:exists(DemCallbackEventStatusChangedFnc))"!]
    [!VAR "RteCtr" = "$RteCtr + 1"!]
  [!ELSE!]
    [!VAR "CLookupCtr" = "$CLookupCtr + 1"!]
    [!IF "not(contains($CbNameList, concat('#', DemCallbackEventStatusChangedFnc, '#')))"!]
      [!VAR "CCbCtr" = "$CCbCtr + 1"!]
      [!VAR "CbNameList" = "concat($CbNameList, DemCallbackEventStatusChangedFnc, '#')"!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!]

#if (defined DEM_C_CALLBACK_TABLE_TRIG_ON_EVST_SIZE)
#error DEM_C_CALLBACK_TABLE_TRIG_ON_EVST_SIZE already defined
#endif
/** \brief Size of the C-callback function pointer table for TriggerOnEventStatus */
#define DEM_C_CALLBACK_TABLE_TRIG_ON_EVST_SIZE [!"num:integer($CCbCtr)"!]U

#if (defined DEM_RTE_CALLBACK_TABLE_TRIG_ON_EVST_SIZE)
#error DEM_RTE_CALLBACK_TABLE_TRIG_ON_EVST_SIZE already defined
#endif
/** \brief Size of the RTE-callback function pointer table for TriggerOnEventStatus */
#define DEM_RTE_CALLBACK_TABLE_TRIG_ON_EVST_SIZE [!"num:integer($RteCtr)"!]U

#if (defined DEM_C_LOOKUP_TABLE_TRIG_ON_EVST_SIZE)
#error DEM_C_LOOKUP_TABLE_TRIG_ON_EVST_SIZE already defined
#endif
/** \brief Size of the C-lookup table for TriggerOnEventStatus */
#define DEM_C_LOOKUP_TABLE_TRIG_ON_EVST_SIZE [!"num:integer($CLookupCtr)"!]U

#if (defined DEM_RTE_LOOKUP_TABLE_TRIG_ON_EVST_SIZE)
#error DEM_RTE_LOOKUP_TABLE_TRIG_ON_EVST_SIZE already defined
#endif
/** \brief Size of the RTE-lookup table for TriggerOnEventStatus */
#define DEM_RTE_LOOKUP_TABLE_TRIG_ON_EVST_SIZE [!"num:integer($RteCtr)"!]U

[!VAR "GenericCtr" = "count(DemGeneral/DemCallbackDTCStatusChanged/*)"!]

#if (defined DEM_CB_TABLE_TRIG_ON_DTCST_SIZE)
#error DEM_CB_TABLE_TRIG_ON_DTCST_SIZE already defined
#endif
/** \brief Size of the C-callback function pointer table for TriggerOnDTCEventStatus */
#define DEM_CB_TABLE_TRIG_ON_DTCST_SIZE [!"num:integer($GenericCtr)"!]U

#if (defined DEM_USE_CB_TRIG_ON_STATUS_CHANGE)
#error DEM_USE_CB_TRIG_ON_STATUS_CHANGE already defined
#endif
/** \brief Macro for Enabling/Disabling trigger on event status or DTC event callback
 ** functionality */
[!IF "(($RteCtr > 0) or ($CCbCtr > 0) or (DemGeneral/DemTriggerFiMReports = 'true')) or (($GenericCtr > 0) or (DemGeneral/DemTriggerDcmReports = 'true'))"!]
#define DEM_USE_CB_TRIG_ON_STATUS_CHANGE STD_ON
[!ELSE!]
#define DEM_USE_CB_TRIG_ON_STATUS_CHANGE STD_OFF
[!ENDIF!]

[!VAR "CCbCtr" = "0"!]
[!VAR "CLookupCtr" = "0"!]
[!VAR "RteCtr" = "0"!]
[!VAR "CbNameList" = "'#'"!]
[!LOOP "DemConfigSet/*/DemEventParameter/*/DemCallbackEventDataChanged"!]
  [!IF "not(node:exists(DemCallbackEventDataChangedFnc))"!]
    [!VAR "RteCtr" = "$RteCtr + 1"!]
  [!ELSE!]
    [!VAR "CLookupCtr" = "$CLookupCtr + 1"!]
    [!IF "not(contains($CbNameList, concat('#', DemCallbackEventDataChangedFnc, '#')))"!]
      [!VAR "CCbCtr" = "$CCbCtr + 1"!]
      [!VAR "CbNameList" = "concat($CbNameList, DemCallbackEventDataChangedFnc, '#')"!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!]

#if (defined DEM_C_CB_TABLE_TRIG_ON_EVDAT_SIZE )
#error DEM_C_CB_TABLE_TRIG_ON_EVDAT_SIZE  already defined
#endif
/** \brief Size of the C-callback function pointer table for TriggerOnEventDataChanged */
#define DEM_C_CB_TABLE_TRIG_ON_EVDAT_SIZE  [!"num:integer($CCbCtr)"!]U

#if (defined DEM_RTE_CB_TABLE_TRIG_ON_EVDAT_SIZE)
#error DEM_RTE_CB_TABLE_TRIG_ON_EVDAT_SIZE already defined
#endif
/** \brief Size of the RTE-callback function pointer table for TriggerOnEventDataChanged */
#define DEM_RTE_CB_TABLE_TRIG_ON_EVDAT_SIZE [!"num:integer($RteCtr)"!]U

#if (defined DEM_C_LOOKUP_TABLE_TRIG_ON_EVDAT_SIZE)
#error DEM_C_LOOKUP_TABLE_TRIG_ON_EVDAT_SIZE already defined
#endif
/** \brief Size of the C-lookup table for TriggerOnEventDataChanged */
#define DEM_C_LOOKUP_TABLE_TRIG_ON_EVDAT_SIZE [!"num:integer($CLookupCtr)"!]U

#if (defined DEM_RTE_LOOKUP_TABLE_TRIG_ON_EVDAT_SIZE)
#error DEM_RTE_LOOKUP_TABLE_TRIG_ON_EVDAT_SIZE already defined
#endif
/** \brief Size of the RTE-lookup table for TriggerOnEventDataChanged */
#define DEM_RTE_LOOKUP_TABLE_TRIG_ON_EVDAT_SIZE [!"num:integer($RteCtr)"!]U

#if (defined DEM_USE_CB_TRIG_ON_EVDAT)
#error DEM_USE_CB_TRIG_ON_EVDAT already defined
#endif
/** \brief Macro for Enabling/Disabling trigger on event data changed callback functionality */
#if ( (DEM_RTE_CB_TABLE_TRIG_ON_EVDAT_SIZE > 0U) || \
      (DEM_C_CB_TABLE_TRIG_ON_EVDAT_SIZE > 0U) )
#define DEM_USE_CB_TRIG_ON_EVDAT STD_ON
#else
#define DEM_USE_CB_TRIG_ON_EVDAT STD_OFF
#endif

[!VAR "CCbCtr" = "0"!]
[!VAR "CLookupCtr" = "0"!]
[!VAR "RteCtr" = "0"!]
[!VAR "CbNameList" = "'#'"!]
[!LOOP "DemConfigSet/*/DemEventParameter/*[not(node:exists(DemEvtCmbCommonParamMaster)) or (DemEvtCmbCommonParamMaster = 'true')]/DemCallbackClearEventAllowed"!]
  [!IF "not(node:exists(DemCallbackClearEventAllowedFnc))"!]
    [!VAR "RteCtr" = "$RteCtr + 1"!]
  [!ELSE!]
    [!VAR "CLookupCtr" = "$CLookupCtr + 1"!]
    [!IF "not(contains($CbNameList, concat('#', DemCallbackClearEventAllowedFnc, '#')))"!]
      [!VAR "CCbCtr" = "$CCbCtr + 1"!]
      [!VAR "CbNameList" = "concat($CbNameList, DemCallbackClearEventAllowedFnc, '#')"!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!]

#if (defined DEM_C_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE)
#error DEM_C_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE  already defined
#endif
/** \brief Size of the C-callback function pointer table for ClearEventAllowed */
#define DEM_C_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE  [!"num:integer($CCbCtr)"!]U

#if (defined DEM_RTE_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE)
#error DEM_RTE_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE already defined
#endif
/** \brief Size of the RTE-callback function pointer table for ClearEventAllowed */
#define DEM_RTE_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE [!"num:integer($RteCtr)"!]U

#if (defined DEM_C_LOOKUP_TABLE_CLEAR_EVENT_ALLOWED_SIZE)
#error DEM_C_LOOKUP_TABLE_CLEAR_EVENT_ALLOWED_SIZE already defined
#endif
/** \brief Size of the C-lookup table for ClearEventAllowed */
#define DEM_C_LOOKUP_TABLE_CLEAR_EVENT_ALLOWED_SIZE [!"num:integer($CLookupCtr)"!]U

#if (defined DEM_RTE_LOOKUP_TABLE_CLEAR_EVENT_ALLOWED_SIZE)
#error DEM_RTE_LOOKUP_TABLE_CLEAR_EVENT_ALLOWED_SIZE already defined
#endif
/** \brief Size of the RTE-lookup table for ClearEventAllowed */
#define DEM_RTE_LOOKUP_TABLE_CLEAR_EVENT_ALLOWED_SIZE [!"num:integer($RteCtr)"!]U

#if (defined DEM_USE_CB_CLEAR_EVENT_ALLOWED)
#error DEM_USE_CB_CLEAR_EVENT_ALLOWED already defined
#endif
/** \brief Macro for Enabling/Disabling ClearEventAllowed callback functionality */
#if ( (DEM_RTE_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE > 0U) || \
      (DEM_C_CB_TABLE_CLEAR_EVENT_ALLOWED_SIZE > 0U) )
#define DEM_USE_CB_CLEAR_EVENT_ALLOWED STD_ON
#else
#define DEM_USE_CB_CLEAR_EVENT_ALLOWED STD_OFF
#endif

[!VAR "GenericCtr" = "count(DemConfigSet/*/DemEventParameter/*/DemEventClass/DemDebounceAlgorithmClass[. = 'DemDebounceMonitorInternal']/DemCallbackGetFDC)"!]

#if (defined DEM_CB_TABLE_GET_FDC_SIZE)
#error DEM_CB_TABLE_GET_FDC_SIZE already defined
#endif
/** \brief Size of the generic callback function pointer table for GetFaultDetectionCounter */
#define DEM_CB_TABLE_GET_FDC_SIZE [!"num:integer($GenericCtr)"!]U

#if (defined DEM_LOOKUP_TABLE_GET_FDC_SIZE)
#error DEM_LOOKUP_TABLE_GET_FDC_SIZE already defined
#endif
/** \brief Size of the generic lookup table for GetFaultDetectionCounter */
#define DEM_LOOKUP_TABLE_GET_FDC_SIZE [!"num:integer($GenericCtr)"!]U

#if (defined DEM_USE_CB_GET_FDC)
#error DEM_USE_CB_GET_FDC already defined
#endif
/** \brief Macro for Enabling/Disabling fault detection counter callback
 ** functionality */
#if (DEM_CB_TABLE_GET_FDC_SIZE > 0U)
#define DEM_USE_CB_GET_FDC STD_ON
#else
#define DEM_USE_CB_GET_FDC STD_OFF
#endif

/*------------------[Client configuration]----------------------------------*/
[!VAR "DemClientCnt" = "count(DemGeneral/DemClient/*)"!]
#if (defined DEM_NUMBER_OF_CLIENTS)
#error DEM_NUMBER_OF_CLIENTS is already defined
#endif
/** \brief Number of configured clients
 **/
#define DEM_NUMBER_OF_CLIENTS      [!"num:integer($DemClientCnt)"!]U

#if (defined DEM_NUM_CLIENTS_SIZE)
#error DEM_NUM_CLIENTS_SIZE is already defined
#endif
/** \brief Array size of clients filter set flag
 **/
#define DEM_NUM_CLIENTS_SIZE [!"num:integer(($DemClientCnt + 7) div 8)"!]U

[!VAR "VirtualClientIdx" = "0"!]
[!IF "DemGeneral/DemRteUsage = 'true' and
      (DemGeneral/DemServiceAPI/DemDefaultASRServiceAPI = 'AUTOSAR_42' or
       DemGeneral/DemServiceAPI/DemDefaultASRServiceAPI = 'AUTOSAR_43')"!]
#if (defined DEM_VIRTUAL_CLIENT_CDD)
#error DEM_VIRTUAL_CLIENT_CDD is already defined
#endif
/** \brief Virtual client ID assigned to the CDD interface
 **/
#define DEM_VIRTUAL_CLIENT_CDD (DEM_NUMBER_OF_CLIENTS + [!"num:integer($VirtualClientIdx)"!]U)
[!VAR "VirtualClientIdx" = "$VirtualClientIdx + 1"!]
[!ENDIF!]

[!IF "(DemGeneral/DemJ1939Support = 'true')"!]
#if (defined DEM_VIRTUAL_CLIENT_J1939)
#error DEM_VIRTUAL_CLIENT_J1939 is already defined
#endif
/** \brief Virtual client ID to address the J1939 event memory
 **/
#define DEM_VIRTUAL_CLIENT_J1939 (DEM_NUMBER_OF_CLIENTS + [!"num:integer($VirtualClientIdx)"!]U)
[!VAR "VirtualClientIdx" = "$VirtualClientIdx + 1"!]
[!ENDIF!]

#if (defined DEM_NUMBER_OF_VIRTUAL_CLIENTS)
#error DEM_NUMBER_OF_VIRTUAL_CLIENTS already defined
#endif
/** \brief Number of virtual clients
 **/
#define DEM_NUMBER_OF_VIRTUAL_CLIENTS [!"num:integer($VirtualClientIdx)"!]U

/*------------------[DTC groups configuration]------------------------------*/

/* Symbolic names of internal DTC group indexes */
[!VAR "GroupIdx" = "0"!]
[!LOOP "node:order(DemGeneral/DemGroupOfDTC/*[(name(.) != 'DEM_DTC_GROUP_EMISSION_REL_DTCS')], './DemGroupDTCs')"!]
  [!VAR "GroupName" = "substring(name(.), 15)"!]
  [!INDENT "0"!]

    #if (defined DEM_DTCGRP_IDX_[!"$GroupName"!])
    #error DEM_DTCGRP_IDX_[!"$GroupName"!] already defined
    #endif
    /** \brief Symbolic name of DTC group [!"$GroupName"!] index */
    #define DEM_DTCGRP_IDX_[!"$GroupName"!][!CALL "Indent", "Length" = "31 - string-length($GroupName)"!] [!"num:integer($GroupIdx)"!]U
    [!VAR "GroupIdx" = "$GroupIdx + 1"!]
  [!ENDINDENT!]
[!ENDLOOP!]

#if (defined DEM_DTCGRP_IDX_NO_DTC)
#error DEM_DTCGRP_IDX_NO_DTC already defined
#endif
/** \brief Symbolic name of DTC group index for events without a DTC */
#define DEM_DTCGRP_IDX_NO_DTC  ([!"num:integer($GroupIdx)"!]U)

#if (defined DEM_NUM_DTC_GROUPS)
#error DEM_NUM_DTC_GROUPS already defined
#endif
/** \brief Number of DTC groups used to define a size of the Dem_DTCGroups[] array */
#define DEM_NUM_DTC_GROUPS [!"num:integer(count(DemGeneral/DemGroupOfDTC/*[name(.) != 'DEM_DTC_GROUP_EMISSION_REL_DTCS']))"!]U

#if (defined DEM_DTCGRP_IDX_INVALID)
#error DEM_DTCGRP_IDX_INVALID already defined
#endif
/** \brief Symbolic name of invalid DTC group index */
#define DEM_DTCGRP_IDX_INVALID  (DEM_NUM_DTC_GROUPS + 1U)

#if (defined DEM_DTCGRP_IDX_ALL_DTC)
#error DEM_DTCGRP_IDX_ALL_DTC already defined
#endif
/** \brief Symbolic name of unassigned DTC group index */
#define DEM_DTCGRP_IDX_ALL_DTC  (DEM_NUM_DTC_GROUPS + 2U)

/*------------------[Freeze frame configuration]----------------------------*/
[!//
[!// Maximum size of freeze frame class will be checked concerning Dem_GetSizeOfFreezeFrameSelection()
[!// and DEM_SIZE_ENTRY_DATA_<...> in Dem_Cfg.h
[!VAR "MaxFFSize" = "0"!]
[!LOOP "DemGeneral/DemFreezeFrameClass/*"!]
  [!VAR "FFSize" = "0"!]
  [!LOOP "DemDidClassRef/*"!]
    [!LOOP "as:ref(.)/DemDidDataElementClassRef/*"!]
      [!VAR "FFSize" = "$FFSize + as:ref(.)/DemDataElementDataSize"!]
    [!ENDLOOP!]
  [!ENDLOOP!]
  [!WS "0"!]/* Size of the freeze frame class [!"name(.)"!] in bytes: [!"num:integer($FFSize)"!] */
  [!IF "$MaxFFSize < $FFSize"!]
    [!VAR "MaxFFSize" = "$FFSize"!]
  [!ENDIF!]
[!ENDLOOP!]

#if (defined DEM_MAXSIZE_FFCLS)
#error DEM_MAXSIZE_FFCLS already defined
#endif
/** \brief Maximum size of a freeze frame class in bytes */
#define DEM_MAXSIZE_FFCLS [!"num:integer($MaxFFSize)"!]U

/* Symblic names of internal freeze frame class indexes */
[!LOOP "DemGeneral/DemFreezeFrameClass/*"!]
  [!INDENT "0"!]
    #if (defined DEM_FFCLS_IDX_[!"name(.)"!])
    #error DEM_FFCLS_IDX_[!"name(.)"!] already defined
    #endif
    /** \brief Symbolic name of freeze frame class [!"name(.)"!] index */
    #define DEM_FFCLS_IDX_[!"name(.)"!][!CALL "Indent", "Length" = "31 - string-length(name(.))"!] [!"num:integer(@index)"!]U
  [!ENDINDENT!]
[!ENDLOOP!]
[!//
[!VAR "NumFFCls" = "count(DemGeneral/DemFreezeFrameClass/*)"!]
[!INDENT "0"!]

  #if (defined DEM_FFCLS_NULL_IDX)
  #error DEM_FFCLS_NULL_IDX already defined
  #endif
  /** \brief Symbolic name of freeze frame class null index */
  #define DEM_FFCLS_NULL_IDX                             [!"num:integer($NumFFCls)"!]U
[!ENDINDENT!]
[!VAR "NumFFCls" = "$NumFFCls + 1"!]

#if (defined DEM_NUM_FFCLS)
#error DEM_NUM_FFCLS already defined
#endif
/** \brief Number of different freeze frame classes */
#define DEM_NUM_FFCLS [!"num:integer($NumFFCls)"!]U

#if (defined DEM_NUM_FFSEGS)
#error DEM_NUM_FFSEGS already defined
#endif
/** \brief Number of different freeze frame segments */
#define DEM_NUM_FFSEGS [!"num:integer(count(DemGeneral/DemDidClass/*))"!]U

/*------------------[Freeze frame record number configuration]--------------*/
[!//
[!IF "DemGeneral/DemTypeOfFreezeFrameRecordNumeration = 'DEM_FF_RECNUM_CONFIGURED' and count(DemGeneral/DemFreezeFrameRecNumClass/*) > 0"!]

/* Symblic names of internal freeze frame record number class indexes */
  [!INDENT "0"!]
    [!LOOP "DemGeneral/DemFreezeFrameRecNumClass/*"!]
      #if (defined DEM_FFRECNUMCLS_IDX_[!"name(.)"!])
      #error DEM_FFRECNUMCLS_IDX_[!"name(.)"!] already defined
      #endif
      /** \brief Symbolic name of freeze frame record number class [!"name(.)"!] index */
      #define DEM_FFRECNUMCLS_IDX_[!"name(.)"!][!CALL "Indent", "Length" = "31 - string-length(name(.))"!] [!"num:integer(@index)"!]U
    [!ENDLOOP!]

    #if (defined DEM_NUM_FFRECNUMCLS)
    #error DEM_NUM_FFRECNUMCLS already defined
    #endif
    /** \brief Number of different freeze frame record number class */
    #define DEM_NUM_FFRECNUMCLS [!"num:integer(count(DemGeneral/DemFreezeFrameRecNumClass/*) + 1)"!]U

    #if (defined DEM_FFRECNUMCLS_NULL)
    #error DEM_FFRECNUMCLS_NULL already defined
    #endif
    /** \brief Index of NULL freeze frame record number class */
    #define DEM_FFRECNUMCLS_NULL (DEM_NUM_FFRECNUMCLS - 1U)
  [!ENDINDENT!]
[!ENDIF!]

/*---------------------[Common Freeze Frame]--------------------------------*/

#if (defined DEM_COMMON_FF_CLASS_IDX)
#error DEM_COMMON_FF_CLASS_IDX already defined
#endif
/** \brief Defines the index of the regular common freeze frame class */
[!IF "(node:refexists(DemGeneral/DemCommonFFDataClassRef))"!]
  [!WS "0"!]#define DEM_COMMON_FF_CLASS_IDX   DEM_FFCLS_IDX_[!"name(as:ref(DemGeneral/DemCommonFFDataClassRef))"!]
[!ELSE!][!//
  [!WS "0"!]#define DEM_COMMON_FF_CLASS_IDX   DEM_FFCLS_NULL_IDX
[!ENDIF!]

/*------------------[Development Freeze Frame]------------------------------*/

/* !LINKSTO dsn.Dem.DevFF.DemCommonFFDataDevAClassRef,1 */
#if (defined DEM_COMMON_DEVFFA_CLASS_IDX)
#error DEM_COMMON_DEVFFA_CLASS_IDX already defined
#endif
/** \brief Switch, indicating if Development Freeze Frames are enabled */
[!IF "(DemGeneral/DemDevFreezeFrameSupport = 'true') and
      node:exists(DemGeneral/DemCommonFFDataDevAClassRef)"!]
  [!WS "0"!]#define DEM_COMMON_DEVFFA_CLASS_IDX   DEM_FFCLS_IDX_[!"name(as:ref(DemGeneral/DemCommonFFDataDevAClassRef))"!]
[!ELSE!][!//
  [!WS "0"!]#define DEM_COMMON_DEVFFA_CLASS_IDX   DEM_FFCLS_NULL_IDX
[!ENDIF!]

/*------------------[Prestored freeze frame configuration]------------------*/

#if (defined DEM_PFF_ZERO_END)
#error DEM_PFF_ZERO_END already defined
#endif
/** \brief Zero value to be added to each size, whose involved patterns could
 ** be none */
#define DEM_PFF_ZERO_END                  0U

#if (defined DEM_SIZE_PFF_ENTRY_DATA)
#error DEM_SIZE_PFF_ENTRY_DATA already defined
#endif
/** \brief Size of dynamic PFF entry data
 **
 ** All FF sizes of events with enabled prestorage are added.
 **/
#define DEM_SIZE_PFF_ENTRY_DATA \
  ([!//
  [!IF "$CalibrationSupport = 'true'"!]
    [!VAR "MaxFFSizePrimary" = "-1"!]
    [!VAR "MaxFFSizeSecondary" = "-1"!]
    [!VAR "MaxFFSizeMirror" = "-1"!]
    [!IF "node:exists(DemGeneral/DemMaxSizeFreezeFrameEntryPrimary)"!]
      [!VAR "MaxFFSizePrimary" = "DemGeneral/DemMaxSizeFreezeFrameEntryPrimary"!]
    [!ENDIF!]
    [!IF "node:exists(DemGeneral/DemMaxSizeFreezeFrameEntrySecondary)"!]
      [!VAR "MaxFFSizeSecondary" = "DemGeneral/DemMaxSizeFreezeFrameEntrySecondary"!]
    [!ENDIF!]
    [!IF "node:exists(DemGeneral/DemMaxSizeFreezeFrameEntryMirror)"!]
      [!VAR "MaxFFSizeMirror" = "DemGeneral/DemMaxSizeFreezeFrameEntryMirror"!]
    [!ENDIF!]
  [!ENDIF!]
[!/* Size of common freeze frame */!][!//
  [!VAR "CommonFFSize" = "0"!]
  [!IF "node:refexists(DemGeneral/DemCommonFFDataClassRef)"!]
    [!LOOP "as:ref(DemGeneral/DemCommonFFDataClassRef)/DemDidClassRef/*"!]
      [!LOOP "as:ref(.)/DemDidDataElementClassRef/*"!]
[!/*      internal data elements can not be configured for FFs */!][!//
        [!VAR "CommonFFSize" = "$CommonFFSize + as:ref(.)/DemDataElementDataSize"!]
      [!ENDLOOP!]
    [!ENDLOOP!]
  [!ENDIF!]
[!/* Calculate size of freeze frame only for events with enabled prestorage support */!][!//
  [!LOOP "DemConfigSet/*/DemEventParameter/*[DemEventClass/DemFFPrestorageSupported = 'true']"!]
    [!VAR "FFSize" = "0"!]
    [!LOOP "as:ref(DemFreezeFrameClassRef)/DemDidClassRef/*"!]
      [!LOOP "as:ref(.)/DemDidDataElementClassRef/*"!]
        [!VAR "FFSize" = "$FFSize + as:ref(.)/DemDataElementDataSize"!]
      [!ENDLOOP!]
    [!ENDLOOP!]
[!/* Size of common part is added only if event has individual part defined */!][!//
    [!VAR "FFSize" = "$CommonFFSize + $FFSize"!]
    [!IF "$CalibrationSupport = 'true'"!]
      [!VAR "MaxFFSize" = "0"!]
      [!IF "DemEventClass/DemEventDestination = 'DEM_DTC_ORIGIN_PRIMARY_MEMORY'"!]
        [!VAR "MaxFFSize" = "$MaxFFSizePrimary"!]
      [!ENDIF!]
      [!IF "DemEventClass/DemEventDestination = 'DEM_DTC_ORIGIN_SECONDARY_MEMORY'"!]
        [!VAR "MaxFFSize" = "$MaxFFSizeSecondary"!]
      [!ENDIF!]
      [!IF "DemEventClass/DemEventDestination = 'DEM_DTC_ORIGIN_MIRROR_MEMORY'"!]
        [!VAR "MaxFFSize" = "$MaxFFSizeMirror"!]
      [!ENDIF!]
      [!IF "$FFSize > $MaxFFSize"!]
[!/* Size error is reported in Dem_Cfg.h */!][!//
      [!ELSE!]
[!/* Use configured maximum freeze frame size */!][!//
        [!VAR "FFSize" = "$MaxFFSize"!]
      [!ENDIF!]
    [!ENDIF!]
    [!WS "0"!][!"num:integer($FFSize)"!]U + [!//
  [!ENDLOOP!]
  DEM_PFF_ZERO_END)

/*------------------[Extended data configuration]---------------------------*/
[!//
[!// Maximum size of extended data class will be checked concerning
[!// and DEM_SIZE_ENTRY_DATA_<...> in Dem_Cfg.h
[!VAR "MaxEDSize" = "0"!]
[!VAR "MaxEDStartByte" = "0"!]
[!LOOP "DemGeneral/DemExtendedDataClass/*"!]
  [!VAR "EDSize" = "0"!]
  [!LOOP "DemExtendedDataRecordClassRef/*"!]
    [!IF "node:islast()"!][!/* last element is omitted for start byte calculation */!][!//
      [!IF "$MaxEDStartByte < $EDSize"!]
        [!VAR "MaxEDStartByte" = "$EDSize"!]
      [!ENDIF!]
    [!ENDIF!]
    [!LOOP "as:ref(.)/DemDataElementClassRef/*"!]
      [!VAR "EDSize" = "$EDSize + as:ref(.)/DemDataElementDataSize"!]
    [!ENDLOOP!]
  [!ENDLOOP!]
  [!WS "0"!]/* Size of the extended data class [!"name(.)"!] in bytes: [!"num:integer($EDSize)"!] */
  [!IF "$MaxEDSize < $EDSize"!]
    [!VAR "MaxEDSize" = "$EDSize"!]
  [!ENDIF!]
[!ENDLOOP!]

#if (defined DEM_MAXSIZE_EDCLS)
#error DEM_MAXSIZE_EDCLS already defined
#endif
/** \brief Maximum size of a extended data class in bytes */
#define DEM_MAXSIZE_EDCLS [!"num:integer($MaxEDSize)"!]U

/* Symbolic names of internal extended data class indexes */
[!LOOP "DemGeneral/DemExtendedDataClass/*"!]
  [!INDENT "0"!]

    #if (defined DEM_EDCLS_IDX_[!"name(.)"!])
    #error DEM_EDCLS_IDX_[!"name(.)"!] already defined
    #endif
    /** \brief Symbolic name of extended data class [!"name(.)"!] index */
    #define DEM_EDCLS_IDX_[!"name(.)"!][!CALL "Indent", "Length" = "31 - string-length(name(.))"!] [!"num:integer(@index)"!]U
  [!ENDINDENT!]
[!ENDLOOP!]
[!//
[!VAR "NumEDCls" = "count(DemGeneral/DemExtendedDataClass/*)"!]
[!INDENT "0"!]

  #if (defined DEM_EDCLS_NULL_IDX)
  #error DEM_EDCLS_NULL_IDX already defined
  #endif
  /** \brief Symbolic name of extended data class null index */
  #define DEM_EDCLS_NULL_IDX                             [!"num:integer($NumEDCls)"!]U
[!ENDINDENT!]
[!VAR "NumEDCls" = "$NumEDCls + 1"!]

#if (defined DEM_NUM_EDCLS)
#error DEM_NUM_EDCLS already defined
#endif
/** \brief Number of different extended data classes */
#define DEM_NUM_EDCLS [!"num:integer($NumEDCls)"!]U

#if (defined DEM_NUM_EDSEGS)
#error DEM_NUM_EDSEGS already defined
#endif
/** \brief Number of different extended data segments */
#define DEM_NUM_EDSEGS [!"num:integer(count(DemGeneral/DemExtendedDataRecordClass/*))"!]U

/*------------------[Data elements configuration]---------------------------*/

/*
 * IMPORTANT to synchronize the order with implementation in function
 * Dem_AssembleEDSegIntData(). Macro order is important to the algorithm
 * in the assamble function.
 *
 * Up to 35 internal data values reserved for internal use
 * based on maximum value of configuration parameter DemDataElementClass.
 */

#if (defined DEM_NUM_EXT_CS_DATAELEMENTS)
#error DEM_NUM_EXT_CS_DATAELEMENTS already defined
#endif
/** \brief Number of different data elements of type DemExternalCSDataElementClass */
#define DEM_NUM_EXT_CS_DATAELEMENTS [!"num:integer(count(DemGeneral/DemDataElementClass/*[. = 'DemExternalCSDataElementClass']))"!]U

#if (defined DEM_NUM_EXT_SR_DATAELEMENTS)
#error DEM_NUM_EXT_SR_DATAELEMENTS already defined
#endif
/** \brief Number of different data elements of type DemExternalSRDataElementClass */
#define DEM_NUM_EXT_SR_DATAELEMENTS [!"num:integer(count(DemGeneral/DemDataElementClass/*[. = 'DemExternalSRDataElementClass']))"!]U

#if (defined DEM_NUM_EXT_DATAELEMENTS)
#error DEM_NUM_EXT_DATAELEMENTS already defined
#endif
/** \brief Number of different external data elements both CS and SR */
#define DEM_NUM_EXT_DATAELEMENTS (DEM_NUM_EXT_CS_DATAELEMENTS + DEM_NUM_EXT_SR_DATAELEMENTS)

#if (defined DEM_INT_VAL_IDX_OCCCTR)
#error DEM_INT_VAL_IDX_OCCCTR already defined
#endif
/** \brief Macro for internal data element 'Occurrence counter' */
#define DEM_INT_VAL_IDX_OCCCTR    DEM_NUM_EXT_DATAELEMENTS

#if (defined DEM_INT_VAL_IDX_AGECTRUP)
#error DEM_INT_VAL_IDX_AGECTRUP already defined
#endif
/** \brief Macro for internal data element 'Aging counter' */
#define DEM_INT_VAL_IDX_AGECTRUP    (DEM_NUM_EXT_DATAELEMENTS + 1U)

#if (defined DEM_INT_VAL_IDX_AGECTRDWN)
#error DEM_INT_VAL_IDX_AGECTRDWN already defined
#endif
/** \brief Macro for internal data element 'Aging counter' */
#define DEM_INT_VAL_IDX_AGECTRDWN  (DEM_NUM_EXT_DATAELEMENTS + 2U)

#if (defined DEM_INT_VAL_IDX_OVFIND)
#error DEM_INT_VAL_IDX_OVFIND already defined
#endif
/** \brief Macro for internal data element 'Overflow indication' */
#define DEM_INT_VAL_IDX_OVFIND    (DEM_NUM_EXT_DATAELEMENTS + 3U)

#if (defined DEM_INT_VAL_IDX_EVSIGNIF)
#error DEM_INT_VAL_IDX_EVSIGNIF already defined
#endif
/** \brief Macro for internal data element 'Event significance' */
#define DEM_INT_VAL_IDX_EVSIGNIF  (DEM_NUM_EXT_DATAELEMENTS + 4U)

#if (defined DEM_INT_VAL_IDX_PRIO)
#error DEM_INT_VAL_IDX_PRIO already defined
#endif
/** \brief Macro for internal data element 'Event priority' */
#define DEM_INT_VAL_IDX_PRIO      (DEM_NUM_EXT_DATAELEMENTS + 5U)

#if (defined DEM_INT_VAL_IDX_EVENT_ID)
#error DEM_INT_VAL_IDX_EVENT_ID already defined
#endif
/** \brief Macro for internal data element 'EventID */
#define DEM_INT_VAL_IDX_EVENT_ID  (DEM_NUM_EXT_DATAELEMENTS + 6U)
#if (defined DEM_INT_VAL_IDX_CSLF)
#error DEM_INT_VAL_IDX_CSLF already defined
#endif

/** \brief Macro for internal data element 'number of operation cycles since the
 ** DTC fault detection counter last reached its maximum value +127' */
#define DEM_INT_VAL_IDX_CSLF        (DEM_NUM_EXT_DATAELEMENTS + 7U)
#if (defined DEM_INT_VAL_IDX_CSFF)

#error DEM_INT_VAL_IDX_CSFF already defined
#endif
/** \brief Macro for internal data element 'number of operation cycles since the
 ** DTC fault detection counter first reached its maximum value of +127' */
#define DEM_INT_VAL_IDX_CSFF        (DEM_NUM_EXT_DATAELEMENTS + 8U)

#if (defined DEM_INT_VAL_IDX_CURRENT_FDC)
#error DEM_INT_VAL_IDX_CURRENT_FDC already defined
#endif
/** \brief Macro for internal data element 'Fault detection counter' */
#define DEM_INT_VAL_IDX_CURRENT_FDC (DEM_NUM_EXT_DATAELEMENTS + 9U)

#if (defined DEM_INT_VAL_IDX_FDC12)
#error DEM_INT_VAL_IDX_FDC12 already defined
#endif
/** \brief Macro for internal data element 'Maximum fault detection
 ** counter since last clear' */
#define DEM_INT_VAL_IDX_FDC12      (DEM_NUM_EXT_DATAELEMENTS + 10U)

#if (defined DEM_INT_VAL_IDX_SI30)
#error DEM_INT_VAL_IDX_SI30 already defined
#endif
/** \brief Macro for internal data element 'DTC status indicators' */
#define DEM_INT_VAL_IDX_SI30      (DEM_NUM_EXT_DATAELEMENTS + 11U)

#if (defined DEM_INT_VAL_IDX_OCC1)
#error DEM_INT_VAL_IDX_OCC1 already defined
#endif
/** \brief Macro for internal data element 'Operation cycles since
 ** last unconfirmedDTC counter' */
#define DEM_INT_VAL_IDX_OCC1      (DEM_NUM_EXT_DATAELEMENTS + 12U)

#if (defined DEM_INT_VAL_IDX_OCC2)
#error DEM_INT_VAL_IDX_OCC2 already defined
#endif
/** \brief Macro for internal data element 'UnconfirmedDTC aging counter' */
#define DEM_INT_VAL_IDX_OCC2       (DEM_NUM_EXT_DATAELEMENTS + 13U)

#if (defined DEM_INT_VAL_IDX_OCC3)
#error DEM_INT_VAL_IDX_OCC3 already defined
#endif
/** \brief Macro for internal data element 'Operation cycles since first
 ** unconfirmedDTC counter' */
#define DEM_INT_VAL_IDX_OCC3      (DEM_NUM_EXT_DATAELEMENTS + 14U)

#if (defined DEM_INT_VAL_IDX_OCC4)
#error DEM_INT_VAL_IDX_OCC4 already defined
#endif
/** \brief Macro for internal data element 'UnconfirmedDTC operation cycle
 ** counter' */
#define DEM_INT_VAL_IDX_OCC4      (DEM_NUM_EXT_DATAELEMENTS + 15U)

#if (defined DEM_INT_VAL_IDX_OCC6)
#error DEM_INT_VAL_IDX_OCC6 already defined
#endif
/** \brief Macro for internal data element 'Consecutive failed operation cycle
 ** counter' */
#define DEM_INT_VAL_IDX_OCC6      (DEM_NUM_EXT_DATAELEMENTS + 16U)

#if (defined DEM_INT_VAL_IDX_OCC7)
#error DEM_INT_VAL_IDX_OCC7 already defined
#endif
/** \brief Macro for internal data element 'Qualified/unconfirmedDTC operation
 ** cycle counter' */
#define DEM_INT_VAL_IDX_OCC7      (DEM_NUM_EXT_DATAELEMENTS + 17U)

#if (defined DEM_INT_VAL_IDX_OCC5)
#error DEM_INT_VAL_IDX_OCC5 already defined
#endif
/** \brief Macro for internal data element 'Warm-up cycles (WUC) where event
 **  is reported as active until it becomes confirmed counter' */
#define DEM_INT_VAL_IDX_OCC5      (DEM_NUM_EXT_DATAELEMENTS + 18U)

#if (defined DEM_INT_VAL_IDX_ROOT_ID)
#error DEM_INT_VAL_IDX_ROOT_ID already defined
#endif
#define DEM_INT_VAL_IDX_ROOT_ID    (DEM_NUM_EXT_DATAELEMENTS + 19U)

#if (defined DEM_INT_VAL_UNUSED)
#error DEM_INT_VAL_UNUSED already defined
#endif
/** \brief Macro for no internal data element used for the
 **  Dem_CheckIntDataElementUsage() internal API */
#define DEM_INT_VAL_UNUSED        (DEM_NUM_EXT_DATAELEMENTS + 20U)

/*------------------[Events configuration]----------------------------------*/
[!CALL "CREATE_DTCCLASS_LIST_FROM_EVENTS"!]
[!CALL "CALC_EVENT_DESC"!]
/* 1st 32 bits of event description layout */

#if (defined DEM_UDS_DTC_OFFSET)
#error DEM_UDS_DTC_OFFSET already defined
#endif
/** \brief Offset of UDS DTC in 1st element of event desc */
#define DEM_UDS_DTC_OFFSET[!"'            '"!][!"num:i($CalUdsDTCOffset)"!]U

#if (defined DEM_DTCORIGIN_OFFSET)
#error DEM_DTCORIGIN_OFFSET already defined
#endif
/** \brief Offset of DTC origin in 1st element of event desc */
#define DEM_DTCORIGIN_OFFSET[!"'            '"!][!"num:i($CalDTCOriginOffset)"!]U

#if (defined DEM_DTCSEVERITY_OFFSET)
#error DEM_DTCSEVERITY_OFFSET already defined
#endif
/** \brief Offset of DTC severity in 1st element of event desc */
#define DEM_DTCSEVERITY_OFFSET[!"'            '"!][!"num:i($CalDTCSeverityOffset)"!]U

#if (DEM_MAX_NUMBER_PRESTORED_FF > 0U)
#if (defined DEM_ENABLEPRESTORAGE_OFFSET)
#error DEM_ENABLEPRESTORAGE_OFFSET already defined
#endif
/** \brief Offset of Prestorage status in 1st element of event desc */
#define DEM_ENABLEPRESTORAGE_OFFSET[!"'            '"!][!"num:i($CalEnablePrestorageOffset)"!]U
#endif

#if (defined DEM_EVENTKIND_OFFSET)
#error DEM_EVENTKIND_OFFSET already defined
#endif
/** \brief Offset of Event kind in 1st element of event desc */
#define DEM_EVENTKIND_OFFSET[!"'            '"!][!"num:i($CalEventKindOffset)"!]U

#if (DEM_NUM_INDICATOR_LINKS > 0U)
#if (defined DEM_INDICATORUSED_OFFSET)
#error DEM_INDICATORUSED_OFFSET already defined
#endif
/** \brief Offset of indicator used status in 1st element of event desc */
#define DEM_INDICATORUSED_OFFSET[!"'            '"!][!"num:i($CalIndicatorOffset)"!]U
#endif

#if (defined DEM_IMMEDIATESTORAGE_OFFSET)
#error DEM_IMMEDIATESTORAGE_OFFSET already defined
#endif
/** \brief Offset of Immediate storage status in 1st element of event desc */
#define DEM_IMMEDIATESTORAGE_OFFSET[!"'            '"!][!"num:i($CalImmediateStorageOffset)"!]U

#if (defined DEM_UDS_DTC_WIDTH)
#error DEM_UDS_DTC_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of DTC */
#define DEM_UDS_DTC_WIDTH              24U

#if (defined DEM_DTCORIGIN_WIDTH)
#error DEM_DTCORIGIN_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of DTC origin */
#define DEM_DTCORIGIN_WIDTH            2U

#if (defined DEM_DTCGROUP_WIDTH)
#error DEM_DTCGROUP_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of DTC group */
#define DEM_DTCGROUP_WIDTH[!"'             '"!][!"num:i($CalEvDescDTCGroupWidth)"!]U

#if (defined DEM_DTCGROUP_OFFSET)
#error DEM_DTCGROUP_OFFSET already defined
#endif
#define DEM_DTCGROUP_OFFSET[!"'            '"!][!"num:i($CalEvDescDTCGroupOffset)"!]U

[!IF "DemGeneral/DemEventCombinationSupport = 'DEM_EVCOMB_TYPE1'"!]
#if (defined DEM_DTCCONFIDX_WIDTH)
#error DEM_DTCCONFIDX_WIDTH already defined
#endif
#define DEM_DTCCONFIDX_WIDTH[!"'           '"!][!"num:i($CalEvDescDTCConfIdxWidth)"!]U

#if (defined DEM_DTCCONFIDX_OFFSET)
#error DEM_DTCCONFIDX_OFFSET already defined
#endif
#define DEM_DTCCONFIDX_OFFSET[!"'          '"!][!"num:i($CalEvDescDTCConfIdxOffset)"!]U
[!ENDIF!]

#if (defined DEM_IMMEDIATESTORAGE_WIDTH)
#error DEM_IMMEDIATESTORAGE_WIDTH already defined
#endif
/** \brief Number of bits to hold the status of Immediate storage */
#define DEM_IMMEDIATESTORAGE_WIDTH     1U

#if (defined DEM_DTCSEVERITY_WIDTH)
#error DEM_DTCSEVERITY_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of DTC severity */
#define DEM_DTCSEVERITY_WIDTH          2U

#if (DEM_MAX_NUMBER_PRESTORED_FF > 0U)
#if (defined DEM_ENABLEPRESTORAGE_WIDTH)
#error DEM_ENABLEPRESTORAGE_WIDTH already defined
#endif
/** \brief Number of bits to hold the status of Prestorage */
#define DEM_ENABLEPRESTORAGE_WIDTH     1U
#endif

#if (defined DEM_DEBOUNCEALGO_WIDTH)
#error DEM_DEBOUNCEALGO_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of Debounce algorithm */
#define DEM_DEBOUNCEALGO_WIDTH[!"'         '"!][!"num:i($CalEvDescDebounceAlgoWidth)"!]U

#if (defined DEM_DEBOUNCEALGO_OFFSET)
#error DEM_DEBOUNCEALGO_OFFSET already defined
#endif
/** \brief Offset of Debounce algorithm in 3rd element of event desc */
#define DEM_DEBOUNCEALGO_OFFSET[!"'        '"!][!"num:i($CalEvDescDebounceAlgoOffset)"!]U

#if (defined DEM_EVENTSIGNIFICANCE_WIDTH)
#error DEM_EVENTSIGNIFICANCE_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of Event Significance */
#define DEM_EVENTSIGNIFICANCE_WIDTH[!"'            '"!][!"num:i($CalEvDescEvSignWidth)"!]U

#if (defined DEM_EVENTSIGNIFICANCE_OFFSET)
#error DEM_EVENTSIGNIFICANCE_OFFSET already defined
#endif
/** \brief Number of bits to hold the value of Freeze frame class index */
#define DEM_EVENTSIGNIFICANCE_OFFSET[!"'            '"!][!"num:i($CalEvDescEvSignOffset)"!]U

#if (defined DEM_FFCLASSIDX_WIDTH)
#error DEM_FFCLASSIDX_WIDTH already defined
#endif
/** \brief  Offset of Event Significance in 2nd element of event desc */
#define DEM_FFCLASSIDX_WIDTH[!"'           '"!][!"num:i($CalEvDescFFClassIdxWidth)"!]U

#if (defined DEM_FFCLASSIDX_OFFSET)
#error DEM_FFCLASSIDX_OFFSET already defined
#endif
/** \brief Offset of FF class index in 2nd element of event desc */
#define DEM_FFCLASSIDX_OFFSET[!"'          '"!][!"num:i($CalEvDescFFClassIdxOffset)"!]U

#if (defined DEM_FFRECINFO_WIDTH)
#error DEM_FFRECINFO_WIDTH already defined
#endif
[!INDENT "0"!]
  [!IF "DemGeneral/DemTypeOfFreezeFrameRecordNumeration = 'DEM_FF_RECNUM_CALCULATED'"!]
    /** \brief Number of bits to hold the value of maximum number of freeze frames */
  [!ELSE!]
    /** \brief Number of bits to hold the value of Freeze frame record numeration class index */
  [!ENDIF!]
[!ENDINDENT!]
#define DEM_FFRECINFO_WIDTH[!"'            '"!][!"num:i($CalEvDescFFRecInfoWidth)"!]U

#if (defined DEM_FFRECINFO_OFFSET)
#error DEM_FFRECINFO_OFFSET already defined
#endif
/** \brief Offset of maximum FF number (if record numeration type is calculated)
 ** or index to freeze frame record numeration class (if record numeration
 ** type is configured) in 2nd element of event desc
 */
#define DEM_FFRECINFO_OFFSET[!"'           '"!][!"num:i($CalEvDescFFRecInfoOffset)"!]U

#if (defined DEM_EDCLASSIDX_WIDTH)
#error DEM_EDCLASSIDX_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of Extended data class index */
#define DEM_EDCLASSIDX_WIDTH[!"'           '"!][!"num:i($CalEvDescEDClassIdxWidth)"!]U

#if (defined DEM_EDCLASSIDX_OFFSET)
#error DEM_EDCLASSIDX_OFFSET already defined
#endif
/** \brief Offset of ED class index in 2nd element of event desc */
#define DEM_EDCLASSIDX_OFFSET[!"'          '"!][!"num:i($CalEvDescEDClassIdxOffset)"!]U

#if (defined DEM_OPCYCLEIDX_WIDTH)
#error DEM_OPCYCLEIDX_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of Operation cycle index */
#define DEM_OPCYCLEIDX_WIDTH[!"'           '"!][!"num:i($CalEvDescOpCycleIdxWidth)"!]U

#if (defined DEM_OPCYCLEIDX_OFFSET)
#error DEM_OPCYCLEIDX_OFFSET already defined
#endif
/** \brief Offset of Operation cycle index in 2nd element of event desc */
#define DEM_OPCYCLEIDX_OFFSET[!"'          '"!][!"num:i($CalEvDescOpCycleIdxOffset)"!]U

#if (defined DEM_AGINGCTRTHRESHOLD_WIDTH)
#error DEM_AGINGCTRTHRESHOLD_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of number of Aging cycles */
#define DEM_AGINGCTRTHRESHOLD_WIDTH[!"'          '"!][!"num:i($CalEvDescAgingCtrThresholdWidth)"!]U

#if (defined DEM_AGINGCTRTHRESHOLD_OFFSET)
#error DEM_AGINGCTRTHRESHOLD_OFFSET already defined
#endif
/** \brief Offset of Aging cycle in 2nd element of event desc */
#define DEM_AGINGCTRTHRESHOLD_OFFSET[!"'         '"!][!"num:i($CalEvDescAgingCtrThresholdOffset)"!]U

/* Configurational value width in bits used for Dem_EventDescType */

#if (defined DEM_EVENTCLASSDESCIDX_WIDTH)
#error DEM_EVENTCLASSDESCIDX_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of event class index */
#define DEM_EVENTCLASSDESCIDX_WIDTH[!"'    '"!][!"num:i($CalEvDescEventClassIdxWidth)"!]U

[!IF "DemGeneral/DemEventCombinationSupport = 'DEM_EVCOMB_DISABLED'"!]
#if (defined DEM_EVENTCLASSDESCIDX_OFFSET)
#error DEM_EVENTCLASSDESCIDX_OFFSET already defined
#endif
/** \brief Offset of event class idx in 2nd element of event desc if calibration is enabled */
#define DEM_EVENTCLASSDESCIDX_OFFSET[!"'   '"!][!"num:i($CalEvDescEventClassIdxOffset)"!]U
[!ENDIF!]

#if (defined DEM_EVENTKIND_WIDTH)
#error DEM_EVENTKIND_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of Event kind */
#define DEM_EVENTKIND_WIDTH            1U

#if (defined DEM_EVENTFAILURECLASSIDX_WIDTH)
#error DEM_EVENTFAILURECLASSIDX_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of Event failure class index */
#define DEM_EVENTFAILURECLASSIDX_WIDTH[!"' '"!][!"num:i($CalEvDescEventFailureClassIdxWidth)"!]U

#if (defined DEM_EVENTFAILURECLASSIDX_OFFSET)
#error DEM_EVENTFAILURECLASSIDX_OFFSET already defined
#endif
/** \brief Offset of Event failure class index in 3rd element of event desc */
#define DEM_EVENTFAILURECLASSIDX_OFFSET  [!"num:i($CalEvDescEventFailureClassIdxOffset)"!]U

#if (defined DEM_ENCONDGRPIDX_WIDTH)
#error DEM_ENCONDGRPIDX_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of Enable condition group index */
#define DEM_ENCONDGRPIDX_WIDTH[!"'            '"!][!"num:i($CalEvDescEnCondGrpIdxWidth)"!]U

#if (defined DEM_ENCONDGRPIDX_OFFSET)
#error DEM_ENCONDGRPIDX_OFFSET already defined
#endif
/** \brief Offset of Enable condition group index in 3rd element of event desc */
#define DEM_ENCONDGRPIDX_OFFSET[!"'           '"!][!"num:i($CalEvDescEnCondGrpIdxOffset)"!]U

[!IF "($EventClassSupport = 'false') and ($NumMultiEventTriggering > 0)"!]
#if (defined DEM_MULTIEVENTTRIGGERINGIDX_WIDTH)
#error DEM_MULTIEVENTTRIGGERINGIDX_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of multi event triggering class index */
#define DEM_MULTIEVENTTRIGGERINGIDX_WIDTH[!"'    '"!][!"num:i($CalEvDescMultiEvTrigClassIdxWidth)"!]U

#if (defined DEM_MULTIEVENTTRIGGERINGIDX_OFFSET)
#error DEM_MULTIEVENTTRIGGERINGIDX_OFFSET already defined
#endif
/** \brief Offset of multi event triggering class index element of event desc */
#define DEM_MULTIEVENTTRIGGERINGIDX_OFFSET[!"'   '"!][!"num:i($CalEvDescMultiEvTrigClassIdxOffset)"!]U
[!ENDIF!]

#if (DEM_NUM_INDICATOR_LINKS > 0U)
#if (defined DEM_INDICATORUSED_WIDTH)
#error DEM_INDICATORUSED_WIDTH already defined
#endif
/** \brief Number of bits to hold the status of Indicators */
#define DEM_INDICATORUSED_WIDTH        1U
#endif

#if (defined DEM_PRIORITY_WIDTH)
#error DEM_PRIORITY_WIDTH already defined
#endif
[!/* negative number will lead to WIDTH of zero */!][!//
/** \brief Number of bits to hold the value of Internal priority */
#define DEM_PRIORITY_WIDTH[!"'             '"!][!"num:i($CalEvDescPriorityWidth)"!]U

#if (defined DEM_PRIORITY_OFFSET)
#error DEM_PRIORITY_OFFSET already defined
#endif
/** \brief Offset of Internal priority in 3rd element of event desc */
#define DEM_PRIORITY_OFFSET[!"'            '"!][!"num:i($CalEvDescPriorityOffset)"!]U

#if (defined DEM_DEBOUNCEIDX_WIDTH)
#error DEM_DEBOUNCEIDX_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of Debounce index */
#define DEM_DEBOUNCEIDX_WIDTH[!"'          '"!][!"num:i($CalEvDescDebounceIdxWidth)"!]U

#if (defined DEM_DEBOUNCEIDX_OFFSET)
#error DEM_DEBOUNCEIDX_OFFSET already defined
#endif
/** \brief Offset of Debounce algorithm index in 3rd element of event desc */
#define DEM_DEBOUNCEIDX_OFFSET[!"'         '"!][!"num:i($CalEvDescDebounceIdxOffset)"!]U

#if (defined DEM_DTCFUNCTIONALUNIT_WIDTH)
#error DEM_DTCFUNCTIONALUNIT_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of DTC functional unit */
#define DEM_DTCFUNCTIONALUNIT_WIDTH[!"'    '"!][!"num:i($CalEvDescDTCFuncUnitWidth)"!]U

#if (defined DEM_DTCFUNCTIONALUNIT_OFFSET)
#error DEM_DTCFUNCTIONALUNIT_OFFSET already defined
#endif
/** \brief Offset of DTC functional unit in 3rd element of event desc */
#define DEM_DTCFUNCTIONALUNIT_OFFSET[!"'   '"!][!"num:i($CalEvDescDTCFuncUnitOffset)"!]

#if (defined DEM_AGINGCYCLEIDX_WIDTH)
#error DEM_AGINGCYCLEIDX_WIDTH already defined
#endif
/** \brief Number of bits to hold the value of relative Aging cycle index
 *
 * The invalid value :: DEM_NUM_AGINGCYCLES is also included in the width. */
#define DEM_AGINGCYCLEIDX_WIDTH[!"'        '"!][!"num:i($CalEvDescAgeCycleIdxWidth)"!]U

#if (defined DEM_AGINGCYCLEIDX_OFFSET)
#error DEM_AGINGCYCLEIDX_OFFSET already defined
#endif
/** \brief Offset of Aging cycle index in 3rd element of event desc */
#define DEM_AGINGCYCLEIDX_OFFSET[!"'       '"!][!"num:i($CalEvDescAgeCycleIdxOffset)"!]U

/*----------------------[OBD configuration offsets]-------------------------*/

[!CALL "OBD_EVENT_OFFSET"!]
[!IF "DemGeneral/DemOBDSupport = 'true'"!]
#if (defined DEM_OBD_READINESS_OFFSET)
#error DEM_OBD_READINESS_OFFSET already defined
#endif
/** \brief Offset of ReadinessGroup */
#define DEM_OBD_READINESS_OFFSET[!"'       '"!][!"num:i($CalOBDReadinessOffset)"!]U

#if (defined DEM_OBD_AGING_CYC_CNT_THRESHOLD_OFFSET)
#error DEM_OBD_AGING_CYC_CNT_THRESHOLD_OFFSET already defined
#endif
/** \brief Offset of DemOBDAgingCycleCounterThreshold in Dem_OBDConfig.Dem_OBDEvConf1 */
#define DEM_OBD_AGING_CYC_CNT_THRESHOLD_OFFSET[!"'       '"!][!"num:i($CalOBDAgingCycCntThresholdOffset)"!]U
[!ENDIF!]

/*------------------[Warning indicators configuration]----------------------*/

[!VAR "HealList"="'#'"!]
[!VAR "HealCtr"="0"!][!/* number of unique healing cycle and threshold pair */!][!//
[!LOOP "DemConfigSet/*[1]/DemEventParameter/*"!]
  [!LOOP "./DemEventClass/DemIndicatorAttribute/*"!]
    [!IF "$CommonOperationCycleEnabled = 'false'"!]
      [!VAR "HealingCycle" = "node:name(as:ref(./DemIndicatorHealingCycleRef))"!]
    [!ELSE!]
      [!VAR "HealingCycle" = "node:name(as:ref(../../DemOperationCycleRef))"!]
    [!ENDIF!]
    [!VAR "HealingCycleCounter" = "node:value(./DemIndicatorHealingCycleCounterThreshold)"!]
    [!IF "not(contains($HealList, concat('#', $HealingCycle,'*',$HealingCycleCounter, '#')))"!]
      [!VAR "HealList" = "concat($HealList, $HealingCycle,'*',$HealingCycleCounter, '#')"!]
      [!VAR "HealCtr" = "$HealCtr + 1"!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDLOOP!]
#if (defined DEM_NUM_HEALINGCYC_COUNTER_INFO_ELEMENTS)
#error DEM_NUM_HEALINGCYC_COUNTER_INFO_ELEMENTS already defined
#endif
/** \brief Number of healing cycle-counter information elements */
#define DEM_NUM_HEALINGCYC_COUNTER_INFO_ELEMENTS [!"num:integer($HealCtr)"!]

[!VAR "FailList"="'#'"!]
[!VAR "FailCtr"="0"!][!/* number of unique failure cycle and threshold pair */!][!//
[!VAR "FailureCycle" = "''"!]
[!LOOP "DemConfigSet/*[1]/DemEventParameter/*"!]
  [!LOOP "./DemEventClass/DemIndicatorAttribute/*"!]
    [!IF "./DemIndicatorFailureCycleSource ='DEM_FAILURE_CYCLE_INDICATOR'"!]
      [!IF "$CommonOperationCycleEnabled = 'false'"!]
        [!VAR "FailureCycle" = "node:name(as:ref(./DemIndicatorFailureCycleRef))"!]
      [!ENDIF!]
      [!VAR "FailureThreshold" = "node:value(./DemIndicatorFailureCycleCounterThreshold)"!]
    [!ELSE!]
      [!IF "$CommonOperationCycleEnabled = 'false'"!]
        [!/* check if event is combined */!][!//
        [!IF "node:exists(../../../DemEvtCmbCommonParamMaster)"!][!//
          [!VAR "FailureCycle" = "node:name(as:ref(../../../../*[(DemDTCClassRef=node:current()/../../../DemDTCClassRef) and (DemEvtCmbCommonParamMaster = 'true')]/DemEventClass/DemEventFailureCycleRef))"!][!//
        [!ELSE!][!//
          [!VAR "FailureCycle" = "node:name(as:ref(../../DemEventFailureCycleRef))"!]
        [!ENDIF!][!//
      [!ENDIF!]
      [!/* check if event is combined */!][!//
      [!IF "node:exists(../../../DemEvtCmbCommonParamMaster)"!][!//
        [!VAR "FailureThreshold" = "node:value(../../../../*[(DemDTCClassRef=node:current()/../../../DemDTCClassRef) and (DemEvtCmbCommonParamMaster = 'true')]/DemEventClass/DemEventFailureCycleCounterThreshold)"!][!//
      [!ELSE!][!//
        [!VAR "FailureThreshold" = "node:value(../../DemEventFailureCycleCounterThreshold)"!]
      [!ENDIF!][!//
    [!ENDIF!]
    [!IF "$CommonOperationCycleEnabled = 'true'"!]
      [!VAR "FailureCycle" = "node:name(as:ref(../../DemOperationCycleRef))"!]
    [!ENDIF!]
    [!IF "not(contains($FailList, concat('#', $FailureCycle,'*', $FailureThreshold, '#')))"!]
      [!VAR "FailList" = "concat($FailList, $FailureCycle,'*', $FailureThreshold, '#')"!]
      [!VAR "FailCtr" = "$FailCtr + 1"!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDLOOP!]
#if (defined DEM_NUM_FAILURECYC_COUNTER_INFO_ELEMENTS)
#error DEM_NUM_FAILURECYC_COUNTER_INFO_ELEMENTS already defined
#endif
/** \brief Number of failure cycle-counter information elements */
#define DEM_NUM_FAILURECYC_COUNTER_INFO_ELEMENTS [!"num:integer($FailCtr)"!]

/*------------------[Enable conditions configuration]-----------------------*/

#if (defined DEM_NUM_ENCONDGROUPS)
#error DEM_NUM_ENCONDGROUPS already defined
#endif
/** \brief Number of enable condition groups, used for further calculation */
#define DEM_NUM_ENCONDGROUPS   [!"num:integer(count(DemGeneral/DemEnableConditionGroup/*))"!]U

#if (defined DEM_NUM_ENCONDGRP_SINGLEREF)
#error DEM_NUM_ENCONDGRP_SINGLEREF already defined
#endif
[!IF "num:integer(count(DemGeneral/DemEnableConditionGroup/*)) > 0"!]
  [!/* number of enable conditions referred by condition groups */!][!//
  [!VAR "NrOfEnCondRefByGrp" = "0"!]
  [!VAR "NrOfEnCondGrpStatCtrLinks" = "0"!]
  [!VAR "EnCondName" = "''"!]
  [!VAR "EnCondId" = "0"!]
  [!LOOP "node:order(DemGeneral/DemEnableCondition/*,'DemEnableConditionId')"!][!//
    [!VAR "EnCondName" = "name(.)"!][!//
    [!VAR "EnCondId" = "num:i(DemEnableConditionId)"!][!//
    [!/* helper to indicate whether a condition already was refereced by a group */!][!//
    [!VAR "EnCondAlreadyReferenced" = "0"!]
    [!LOOP "../../DemEnableConditionGroup/*/DemEnableConditionRef/*"!][!//
      [!IF "as:ref(.)/DemEnableConditionId = $EnCondId"!]
        [!/* incrementing nr of groups, that are referencing this condition */!][!//
        [!VAR "NrOfEnCondGrpStatCtrLinks" = "$NrOfEnCondGrpStatCtrLinks + 1"!]
        [!IF "$EnCondAlreadyReferenced = 0"!]
          [!VAR "NrOfEnCondRefByGrp" = "$NrOfEnCondRefByGrp + 1"!]
          [!VAR "EnCondAlreadyReferenced" = "1"!]
        [!ENDIF!]
      [!ENDIF!]
    [!ENDLOOP!][!//
  [!ENDLOOP!][!//
/** \brief This macro specifies the number of enable conditions referred by condition groups.
 **
 **        The value is determined in the following way: first a verification is made for the
 **        enable condition in order to checked whether it has been referenced already by a
 **        condition group or not. If the enable condition has not been referenced by a condition
 **        group then the number of enable conditions referred by condition groups is incremented
 **        by 1U. In this way it is ensured that multiple references to one enable condition
 **        is counted once.
 */
#define DEM_NUM_ENCONDGRP_SINGLEREF   [!"num:i($NrOfEnCondRefByGrp + 1)"!]U
[!ELSE!]
#define DEM_NUM_ENCONDGRP_SINGLEREF   0U
[!ENDIF!]

#if (defined DEM_NUM_ENCONDGRP_LINKS)
#error DEM_NUM_ENCONDGRP_LINKS already defined
#endif
[!IF "num:integer(count(DemGeneral/DemEnableConditionGroup/*)) > 0"!]
/** \brief Number of links to the dedicated enable condition group counters */
#define DEM_NUM_ENCONDGRP_LINKS   [!"num:i($NrOfEnCondGrpStatCtrLinks)"!]U
[!ELSE!]
/** \brief Number of enable conditions referenced by groups */
#define DEM_NUM_ENCONDGRP_LINKS   0U
[!ENDIF!]

/*------------------[Debouncing configuration]------------------------------*/

#if (defined DEM_NUM_DEBOUNCE_COUNTER_CLASSES)
#error DEM_NUM_DEBOUNCE_COUNTER_CLASSES already defined
#endif
[!CALL "DEBOUNCE_COUNTER_GET_CLASS_COUNT"!]
[!VAR "NumDebounceCounterBasedClasses" = "$DebounceCounterClassCount"!]

/** \brief Number of counter based debounce classes */
#define DEM_NUM_DEBOUNCE_COUNTER_CLASSES   [!"num:integer($NumDebounceCounterBasedClasses)"!]U

#if (defined DEM_NUM_DEBOUNCE_COUNTER)
#error DEM_NUM_DEBOUNCE_COUNTER already defined
#endif
[!VAR "NumDebounceCounterBased" = "count(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceCounterBased'])"!]
/** \brief Number of counter based debounced events */
#define DEM_NUM_DEBOUNCE_COUNTER   [!"num:integer($NumDebounceCounterBased)"!]U

#if (defined DEM_NUM_DEBOUNCE_TIME)
#error DEM_NUM_DEBOUNCE_TIME already defined
#endif
/** \brief Number of Time based debounced events */
#define DEM_NUM_DEBOUNCE_TIME      [!"num:integer(count(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceTimeBase']))"!]U

#if (defined DEM_NUM_DEBOUNCE_TIME_CLASSES)
#error DEM_NUM_DEBOUNCE_TIME_CLASSES already defined
#endif
[!CALL "DEBOUNCE_TIME_GET_CLASS_COUNT"!][!//
[!VAR "NumTimeDebounceBasedClasses" = "$DebounceTimeClassCount"!]

/** \brief Number of Time based debounce classes */
/* !LINKSTO Dem.Calibration.DemTimeBasedDebounceClassConcept,1 */
#define DEM_NUM_DEBOUNCE_TIME_CLASSES   [!"num:integer($NumTimeDebounceBasedClasses)"!]U

#if (defined DEM_NUM_DEBOUNCE_FREQUENCY)
#error DEM_NUM_DEBOUNCE_FREQUENCY already defined
#endif
/** \brief Number of counter based debounced events */
#define DEM_NUM_DEBOUNCE_FREQUENCY [!"num:integer(count(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceFrequencyBased']))"!]U

/*------------------[Fault confirmation configuration]----------------------*/

#if (defined DEM_NO_FAULT_CONFIRMATION)
#error DEM_NO_FAULT_CONFIRMATION already defined
#endif
/** \brief Symbolic name for failure class index if default configuration is used */
#define DEM_NO_FAULT_CONFIRMATION [!"num:integer($CalEvDescEventFailureClassIdx)"!]U

/*------------------[Immediate storage configuration]-----------------------*/

[!IF "(count(DemConfigSet/*/DemEventParameter/*[node:refexists(DemDTCClassRef) and as:ref(DemDTCClassRef)/DemImmediateNvStorage = 'true']) > 0) or
      (DemGeneral/DemImmediateStorageInternalEvents = 'true') or (DemGeneral/DemClearDTCBehavior != 'DEM_CLRRESP_VOLATILE')"!]
#if (defined DEM_STORAGEWAITNVMREADY)
#error DEM_STORAGEWAITNVMREADY already defined
#endif
/** \brief Macro for generic invocation of user-defined name of the Memory stack main function triggering callout function */
#define DEM_STORAGEWAITNVMREADY(BlockId) [!"DemGeneral/DemCallbackMemStackMainFuncTrigger"!](BlockId);
[!ENDIF!]

/*------------------[Dynamic DTC configuration]-----------------------------*/

[!IF "node:exists(DemGeneral/DemCalloutDynamicDTCFnc)"!]
#if (defined DEM_GET_DYNAMIC_DTC)
#error DEM_GET_DYNAMIC_DTC already defined
#endif
#define DEM_GET_DYNAMIC_DTC(EvId, DTC) [!"DemGeneral/DemCalloutDynamicDTCFnc"!](EvId, DTC);
[!ENDIF!]

/*------------------[J1939 Freeze frame definitions]--------------*/
[!VAR "NumJ1939FFCls" = "count(DemGeneral/DemJ1939FreezeFrameClass/*)"!]
[!INDENT "0"!]

#if (defined DEM_J1939FFCLS_NULL_IDX)
#error DEM_J1939FFCLS_NULL_IDX already defined
#endif
/** \brief Symbolic name of J1939 freeze frame class null index */
#define DEM_J1939FFCLS_NULL_IDX                             [!"num:integer($NumJ1939FFCls)"!]U
[!ENDINDENT!]
[!VAR "NumJ1939FFCls" = "$NumJ1939FFCls + 1"!]

#if (defined DEM_NUM_J1939FFCLS)
#error DEM_NUM_J1939FFCLS already defined
#endif
/** \brief Number of different J1939 freeze frame classes */
#define DEM_NUM_J1939FFCLS [!"num:integer($NumJ1939FFCls)"!]U

#if (defined DEM_NUM_J1939FFSPNS)
#error DEM_NUM_J1939FFSPNS already defined
#endif
/** \brief Number of different J1939 freeze frame SPNs */
#define DEM_NUM_J1939FFSPNS [!"num:integer(count(DemGeneral/DemSPNClass/*))"!]U

/*----------------------------[J1939 Det]-------------------------*/
[!IF "(DemGeneral/DemJ1939Support = 'true') and (DemGeneral/DemDevErrorDetect = 'true')"!]
[!VAR "ListNode" = "''"!]
[!VAR "Node" = "-1"!]
[!VAR "IndexJ1939" = "0"!][!// Get the max Node ID
  [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!][!//
    [!IF "node:exists(DemDTCClassRef) and node:exists(as:ref(DemDTCClassRef)/DemJ1939DTCValue)"!][!// Set the LastNode to the Maximum value to make sure that all other nodes have lower ID
      [!VAR "Node" = "node:value(as:ref(as:ref(./DemDTCClassRef)/DemJ1939NodeRef)/J1939NmNodeId)"!]
      [!IF "not(contains($ListNode,num:integer($Node)))"!][!//
        [!VAR "ListNode" = "concat('-',$ListNode,'')"!]
        [!VAR "ListNode" = "concat(num:integer($Node),$ListNode,'')"!]
#if (defined DEM_J1939_NODE_[!"num:integer($IndexJ1939)"!])
#error DEM_J1939_NODE_[!"num:integer($IndexJ1939)"!] already defined
#endif
/** \brief Node [!"num:integer($IndexJ1939)"!] */
#define DEM_J1939_NODE_[!"num:integer($IndexJ1939)"!] [!"num:integer($Node)"!]U
        [!VAR "IndexJ1939" = "$IndexJ1939 + 1"!]
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
  [!VAR "ExitIndex" = "0"!][!// Loop throught all the Nodes and create an Exist macro for each node
  [!FOR "ExistIndex" = "0" TO "$IndexJ1939 - 1"!][!//

#if (defined DEM_J1939_NODE_EXIST_[!"num:integer($ExistIndex)"!])
#error DEM_J1939_NODE_EXIST_[!"num:integer($ExistIndex)"!] already defined
#endif
/** \brief Is Exist for Node [!"num:integer($ExistIndex)"!] */
    [!VAR "NextIndex" = "$ExistIndex + 1"!][!// Create the Macro that is returns TRUE if the node exist but if does not call the exist for next node and see if it matches
#define DEM_J1939_NODE_EXIST_[!"num:integer($ExistIndex)"!](node) ((node == DEM_J1939_NODE_[!"num:integer($ExistIndex)"!])?TRUE:DEM_J1939_NODE_EXIST_[!"num:integer($NextIndex)"!](node))
    [!VAR "ExitIndex" = "$ExitIndex + 1"!]
  [!ENDFOR!][!//

#if (defined DEM_J1939_NODE_EXIST_[!"num:integer($ExitIndex)"!])
#error DEM_J1939_NODE_EXIST_[!"num:integer($ExitIndex)"!] already defined
#endif
[!// Create an Exist Macro with node Index after last node index that returns always false
/** \brief Is Exist for Node [!"num:integer($ExitIndex)"!] */
#define DEM_J1939_NODE_EXIST_[!"num:integer($ExitIndex)"!](node) (FALSE)

#if (defined DEM_J1939_NODE_EXIST)
#error DEM_J1939_NODE_EXIST already defined
#endif
[!// Entry point for checking if a specific Node Exists
/** \brief Node Index */
#define DEM_J1939_NODE_EXIST(node) DEM_J1939_NODE_EXIST_0(node)
[!ENDIF!][!//

/*------------------[Dem BSW distribution macros]---------------------------------*/
[!//
[!CALL "CALCULATE_MULTICORE_SETTINGS"!][!//

#if (defined DEM_MASTER_INSTANCE)
#error DEM_MASTER_INSTANCE already defined
#endif
/** \brief Macro defining the core configured as master core */
#define DEM_MASTER_INSTANCE [!"$MasterCoreId"!]U

#if (defined DEM_NUMBER_OF_CORES)
#error DEM_NUMBER_OF_CORES already defined
#endif
/** \brief Dem number of cores */
#define DEM_NUMBER_OF_CORES [!"num:i($NumberOfSatteliteCores + 1)"!]U

[!INDENT "0"!]
  [!IF "$DemBSWDistributionOfEventReports = 'true'"!]
    [!LOOP "(DemGeneral/DemBSWDistribution/DemSatelliteInstanceConfig/*)"!]
      [!VAR "SatelliteCoreId" = "num:i(DemSatelliteCoreId)"!]

      #if (defined DEM_SATELLITE_[!"$SatelliteCoreId"!]_LOCAL_BSW_EVENT_QUEUE_SIZE)
      #error DEM_SATELLITE_[!"$SatelliteCoreId"!]_LOCAL_BSW_EVENT_QUEUE_SIZE already defined
      #endif
      /** \brief BSW event queue size of satellite instance [!"$SatelliteCoreId"!] during pre-initialization */
      #define DEM_SATELLITE_[!"$SatelliteCoreId"!]_LOCAL_BSW_EVENT_QUEUE_SIZE [!"num:i(DemSatelliteInstanceLocalBswEventBufferSize)"!]U

      #if (defined DEM_SATELLITE_[!"$SatelliteCoreId"!]_CORE_ID)
      #error DEM_SATELLITE_[!"$SatelliteCoreId"!]_CORE_ID already defined
      #endif
      /** \brief Satellite instance core Id */
      #define DEM_SATELLITE_[!"$SatelliteCoreId"!]_CORE_ID [!"$SatelliteCoreId"!]U
    [!ENDLOOP!]
  [!ENDIF!]
[!ENDINDENT!]
[!//

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/
[!//
[!INDENT "0"!]
  [!IF "$DemBSWDistributionOfEventReports = 'true'"!]
    [!LOOP "(DemGeneral/DemBSWDistribution/DemSatelliteInstanceConfig/*)"!]
      [!VAR "SatelliteCoreId" = "num:i(DemSatelliteCoreId)"!]

      #define DEM_1_Core_[!"$SatelliteCoreId"!]_START_SEC_VAR_CLEARED_8
      #include <Dem_1_Core_[!"$SatelliteCoreId"!]_MemMap.h>

      /** \brief Index of the next entry in the FIFO queue belonging to the satellite instance [!"$SatelliteCoreId"!] to write into */
      extern VAR(uint8, DEM_VAR) Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueueWriteIdx;
      /** \brief Index of the next entry in the FIFO queue belonging to the satellite instance [!"$SatelliteCoreId"!] to read from */
      extern VAR(uint8, DEM_VAR) Dem_Core[!"$SatelliteCoreId"!]_LocalBswEventQueueReadIdx;

      #define DEM_1_Core_[!"$SatelliteCoreId"!]_STOP_SEC_VAR_CLEARED_8
      #include <Dem_1_Core_[!"$SatelliteCoreId"!]_MemMap.h>
    [!ENDLOOP!]
  [!ENDIF!]
[!ENDINDENT!]

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef DEM_INT_CFG_H */
/*==================[end of file]===========================================*/
