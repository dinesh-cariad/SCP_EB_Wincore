/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.5 (required)
 * An external object or function shall be declared in one and only one file.
 *
 * Reason:
 * The files SchM_UdpNm.h and UdpNm_Cfg.h contain declarations of UdpNm_MainFunction_x.
 * This is due to the issue reported in http://www.autosar.org/bugzilla/show_bug.cgi?id=53853
 * and should be fixed in a future version of the Rte.
 */

#ifndef UDPNM_CFG_H
#define UDPNM_CFG_H
/* !LINKSTO SWS_UdpNm_00201,1, EB_SWS_UdpNm_00044_3,1 */

#include <UdpNm_Adaptive.h>

#if (UDPNM_ADAPTIVE == STD_ON)
#include <AdaptiveUdpNm_Cfg.h>
#else /* (UDPNM_ADAPTIVE == STD_ON) */
/*------------------[Defensive programming]---------------------------------*/
[!SELECT "UdpNmDefensiveProgramming"!][!//

#if (defined UDPNM_DEFENSIVE_PROGRAMMING_ENABLED)
#error UDPNM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define UDPNM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmDevErrorDetect = 'true') and (UdpNmDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_PRECONDITION_ASSERT_ENABLED)
#error UDPNM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define UDPNM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmDevErrorDetect = 'true') and (UdpNmDefProgEnabled = 'true') and (UdpNmPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_POSTCONDITION_ASSERT_ENABLED)
#error UDPNM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define UDPNM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmDevErrorDetect  = 'true') and (UdpNmDefProgEnabled = 'true') and (UdpNmPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error UDPNM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define UDPNM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmDevErrorDetect  = 'true') and (UdpNmDefProgEnabled = 'true') and (UdpNmUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_INVARIANT_ASSERT_ENABLED)
#error UDPNM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define UDPNM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmDevErrorDetect  = 'true') and (UdpNmDefProgEnabled = 'true') and (UdpNmInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_STATIC_ASSERT_ENABLED)
#error UDPNM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define UDPNM_STATIC_ASSERT_ENABLED           [!//
[!IF "(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmDevErrorDetect  = 'true') and (UdpNmDefProgEnabled = 'true') and (UdpNmStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//


[!/*  select first of the multiple config containers */!][!//
[!SELECT "UdpNmGlobalConfig/*[1]"!][!//

/*==================[includes]==============================================*/

#include <TSAutosar.h>                      /* EB specific standard types */
/* !LINKSTO UdpNm.EB.UdpNmNodeIdCallbackHeaderName,1 */
[!IF "node:exists(UdpNmNodeIdCallback)"!]
#include <[!"UdpNmNodeIdCallbackHeader"!]>  /* UdpNm node Id callback header */
[!ENDIF!]

#ifndef UDPNM_NO_CFGCLASSMIX_REQUIRED
#include <UdpNm_SymbolicNames_PBcfg.h>
#endif

#undef UDPNM_NO_CFGCLASSMIX_REQUIRED

[!INCLUDE "../../generate_macros/UdpNm_Macros.m"!][!//

/*==================[macros]================================================*/

/* !LINKSTO UdpNm.Config.UdpNmMultiCoreSupport,1 */
#if (defined UDPNM_MULTICORE_SUPPORT)
#error UDPNM_MULTICORE_SUPPORT is already defined
#endif
/** \brief Enable multicore support */
#define UDPNM_MULTICORE_SUPPORT [!IF "as:modconf('UdpNm')[1]/UdpNmGeneral/UdpNmMultiCoreSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_CFG_SIGNATURE)
#error UDPNM_CFG_SIGNATURE is already defined
#endif
/* !LINKSTO UdpNm.EB.PBCFGM104,1 */
#define UDPNM_CFG_SIGNATURE [!"asc:getConfigSignature(as:modconf('UdpNm')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U /* Compile time verification value */

#if (defined UDPNM_PUBLIC_INFO_SIGNATURE)
#error UDPNM_PUBLIC_INFO_SIGNATURE is already defined
#endif
/* !LINKSTO UdpNm.EB.PBCFGM104_1,1 */
#define UDPNM_PUBLIC_INFO_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('UdpNm')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('UdpNm')[1]/CommonPublishedInformation/Release))"!]U /* PublicInfoSignature */

#if (defined UDPNM_PBCFGM_SUPPORT_ENABLED)
#error UDPNM_PBCFGM_SUPPORT_ENABLED is already defined
#endif
/* !LINKSTO UdpNm.Impl.PbCfgM,1, UdpNm.EB.PBCFGM107,1 */
#define UDPNM_PBCFGM_SUPPORT_ENABLED [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('UdpNm')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* ----- Pre-processor switch to enable/diable relocateable postbuild config ----- */
#if (defined UDPNM_RELOCATABLE_CFG_ENABLE)
#error UDPNM_RELOCATABLE_CFG_ENABLE is already defined
#endif
/* !LINKSTO UdpNm.Impl.Relocatable,1, UdpNm.EB.PbcfgM_General_3,1 */
#if (UDPNM_PBCFGM_SUPPORT_ENABLED == STD_ON)
#define UDPNM_RELOCATABLE_CFG_ENABLE [!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#else
#define UDPNM_RELOCATABLE_CFG_ENABLE [!IF "as:modconf('UdpNm')[1]/UdpNmGeneral/UdpNmRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#endif

#if (defined UDPNM_SOADIFRXINDICATION_ASR412)
#error UDPNM_SOADIFRXINDICATION_ASR412 is already defined
#endif
#define UDPNM_SOADIFRXINDICATION_ASR412 [!IF "as:modconf('UdpNm')[1]/UdpNmGeneral/UdpNmASR412APICompatibility = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_VERSION_INFO_API)
#error UDPNM_VERSION_INFO_API is already defined
#endif
#define UDPNM_VERSION_INFO_API            [!//
[!IF "UdpNmVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_COM_CONTROL_ENABLED)
#error UDPNM_COM_CONTROL_ENABLED is already defined
#endif
/* !LINKSTO SWS_UdpNm_00168,1 */
#define UDPNM_COM_CONTROL_ENABLED         [!//
[!IF "UdpNmComControlEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_STATE_CHANGE_IND_ENABLED)
#error UDPNM_STATE_CHANGE_IND_ENABLED is already defined
#endif
/* !LINKSTO ECUC_UdpNm_00012,1 */
#define UDPNM_STATE_CHANGE_IND_ENABLED    [!//
[!IF "UdpNmStateChangeIndEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_PASSIVE_MODE_ENABLED)
#error UDPNM_PASSIVE_MODE_ENABLED is already defined
#endif
/* !LINKSTO SWS_UdpNm_00161,1, ECUC_UdpNm_00010,1 */
#define UDPNM_PASSIVE_MODE_ENABLED        [!//
[!IF "UdpNmPassiveModeEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_PDU_RX_INDICATION_ENABLED)
#error UDPNM_PDU_RX_INDICATION_ENABLED is already defined
#endif
/* !LINKSTO ECUC_UdpNm_00011,1 */
#define UDPNM_PDU_RX_INDICATION_ENABLED   [!//
[!IF "UdpNmPduRxIndicationEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_IMMEDIATE_RESTART_ENABLED)
#error UDPNM_IMMEDIATE_RESTART_ENABLED is already defined
#endif
/* !LINKSTO ECUC_UdpNm_00009,2 */
#define UDPNM_IMMEDIATE_RESTART_ENABLED   [!//
[!IF "UdpNmImmediateRestartEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_USER_DATA_ENABLED)
#error UDPNM_USER_DATA_ENABLED is already defined
#endif
/* !LINKSTO SWS_UdpNm_00158,1, ECUC_UdpNm_00004,1 */
#define UDPNM_USER_DATA_ENABLED           [!//
[!IF "UdpNmUserDataEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_COM_USER_DATA_SUPPORT)
#error UDPNM_COM_USER_DATA_SUPPORT is already defined
#endif
/* !LINKSTO ECUC_UdpNm_00055,1 */
#define UDPNM_COM_USER_DATA_SUPPORT       [!//
[!IF "UdpNmComUserDataSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_REMOTE_SLEEP_IND_ENABLED)
#error UDPNM_REMOTE_SLEEP_IND_ENABLED is already defined
#endif
/* !LINKSTO ECUC_UdpNm_00005,1 */
#define UDPNM_REMOTE_SLEEP_IND_ENABLED    [!//
[!IF "UdpNmRemoteSleepIndEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_NODE_ID_ENABLED)
#error UDPNM_NODE_ID_ENABLED is already defined
#endif
#define UDPNM_NODE_ID_ENABLED             [!//
[!IF "count(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmChannelConfig/*[UdpNmNodeIdEnabled='true']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_REPEAT_MSG_IND_ENABLED)
#error UDPNM_REPEAT_MSG_IND_ENABLED is already defined
#endif
/* !LINKSTO ECUC_UdpNm_00015,1 */
#define UDPNM_REPEAT_MSG_IND_ENABLED      [!//
[!IF "count(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmChannelConfig/*[UdpNmRepeatMsgIndEnabled='true']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_NODE_DETECTION_ENABLED)
#error UDPNM_NODE_DETECTION_ENABLED is already defined
#endif
/* !LINKSTO ECUC_UdpNm_00007,1 */
#define UDPNM_NODE_DETECTION_ENABLED      [!//
[!IF "count(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmChannelConfig/*[UdpNmNodeDetectionEnabled='true']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_BUS_SYNCHRONIZATION_ENABLED)
#error UDPNM_BUS_SYNCHRONIZATION_ENABLED is already defined
#endif
/* !LINKSTO ECUC_UdpNm_00006,1 */
#define UDPNM_BUS_SYNCHRONIZATION_ENABLED [!//
[!IF "UdpNmBusSynchronizationEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_IMMEDIATE_TRANSMISSION)
#error UDPNM_IMMEDIATE_TRANSMISSION is already defined
#endif
#define UDPNM_IMMEDIATE_TRANSMISSION  [!//
[!IF "num:i(count(UdpNmChannelConfig/*[UdpNmImmediateNmTransmissions > 0])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_DEV_ERROR_DETECT)
#error UDPNM_DEV_ERROR_DETECT is already defined
#endif
/* !LINKSTO ECUC_UdpNm_00002,1 */
#define UDPNM_DEV_ERROR_DETECT            [!//
[!IF "UdpNmDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_NUMBER_OF_CHANNELS)
#error UDPNM_NUMBER_OF_CHANNELS is already defined
#endif
#define UDPNM_NUMBER_OF_CHANNELS               [!//
[!"num:i(count(UdpNmChannelConfig/*))"!]U

#if (defined UDPNM_INDEXFROMNMCHANNELHANDLE_NUM)
#error UDPNM_INDEXFROMNMCHANNELHANDLE_NUM is already defined
#endif
/** \brief Number of elements in array UdpNm_IndexFromComMChannelHandle[] */
#define UDPNM_INDEXFROMNMCHANNELHANDLE_NUM     [!//
[!"num:i(num:max(node:refs(UdpNmChannelConfig/*/UdpNmComMNetworkHandleRef)/ComMChannelId)+1)"!]U

#if (defined UDPNM_ACTIVE_WAKEUP_BIT_ENABLED)
#error UDPNM_ACTIVE_WAKEUP_BIT_ENABLED is already defined
#endif
/* !LINKSTO EB_ECUC_UdpNm_00074,1 */
/** \brief Macro to indicate if Active wake up bit is present in CBV */
#define UDPNM_ACTIVE_WAKEUP_BIT_ENABLED        [!//
[!IF "UdpNmActiveWakeupBitEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_NODEID_CALLBACK_ENABLED)
#error UDPNM_NODEID_CALLBACK_ENABLED is already defined
#endif
/* !LINKSTO UdpNm.EB.UdpNmNodeIdCallbackName,1 */
/** \brief Macro to indicate if Node Id callback is enabled */
#define UDPNM_NODEID_CALLBACK_ENABLED          [!//
[!IF "node:exists(UdpNmNodeIdCallback)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_NODEID_CALLBACK_NAME)
#error UDPNM_NODEID_CALLBACK_NAME is already defined
#endif
[!IF "node:exists(UdpNmNodeIdCallback)"!]
/* !LINKSTO UdpNm.EB.UdpNmNodeIdCallbackName,1 */
/** \brief Name of Node Id callback function */
#define UDPNM_NODEID_CALLBACK_NAME   [!"UdpNmNodeIdCallback"!]
[!ENDIF!]

#if (defined UDPNM_CARWAKEUP_RX_ENABLED)
#error UDPNM_CARWAKEUP_RX_ENABLED is already defined
#endif

#define UDPNM_CARWAKEUP_RX_ENABLED        [!//
[!IF "count(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmChannelConfig/*[UdpNmCarWakeUpRxEnabled='true']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/*------------------[PN Extensions]-------------------------------------*/
[!IF "as:modconf('UdpNm')[1]/UdpNmGeneral/UdpNmPnSupported = 'true'"!]

#if (defined UDPNM_PN_INFO_LENGTH)
#error UDPNM_PN_INFO_LENGTH is already defined
#endif
/* !LINKSTO ECUC_UdpNm_00069,1 */
#define UDPNM_PN_INFO_LENGTH    [!"UdpNmPnInfo/UdpNmPnInfoLength"!]U

/* !LINKSTO SWS_UdpNm_00358,2, SWS_UdpNm_00345,2 */
#if (defined UDPNM_EIRA_MAX_TIMER_SIZE)
#error UDPNM_EIRA_MAX_TIMER_SIZE is already defined
#endif
/* Size of array for mapping PN EIRA timer */
[!//
[!// Calculate size of array required for EIRA timer
[!IF "(node:exists(UdpNmPnEiraCalcEnabled) and (UdpNmPnEiraCalcEnabled = 'true')) or  num:i(count(UdpNmChannelConfig/*[(node:exists(UdpNmPnEraCalcEnabled)) and (UdpNmPnEraCalcEnabled='true')])) > 0"!][!//
[!NOCODE!][!//
[!VAR "EiraMaxTimerSize" = "0"!][!//
[!IF "node:exists(as:modconf('UdpNm')[1]/UdpNmGeneral/UdpNmMaxPn)"!][!//
  [!VAR "EiraMaxTimerSize" = "node:value(as:modconf('UdpNm')[1]/UdpNmGeneral/UdpNmMaxPn)"!][!//
[!ELSE!][!//
  [!VAR "EiraMaxTimerSize" = "node:value(UdpNmPnInfo/UdpNmPnInfoLength) * 8"!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
#define UDPNM_EIRA_MAX_TIMER_SIZE    [!"num:i($EiraMaxTimerSize)"!]U  /* UdpNm Eira Maximum Timer array size */
[!ENDIF!]

#if (defined UDPNM_PN_INFO_OFFSET)
#error UDPNM_PN_INFO_OFFSET is already defined
#endif
/* !LINKSTO ECUC_UdpNm_00068_Refine,1 */
#define UDPNM_PN_INFO_OFFSET    [!"UdpNmPnInfo/UdpNmPnInfoOffset"!]U

#if (defined UDPNM_PN_EIRA_CALC_ENABLED)
#error UDPNM_PN_EIRA_CALC_ENABLED is already defined
#endif
/* !LINKSTO ECUC_UdpNm_00066,1 */
#define UDPNM_PN_EIRA_CALC_ENABLED             [!//
[!IF "(node:exists(UdpNmPnEiraCalcEnabled)) and (UdpNmPnEiraCalcEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_PN_ERA_CALC_ENABLED)
#error UDPNM_PN_ERA_CALC_ENABLED is already defined
#endif
/** UdpNmPnEraCalcEnabled **/
#define UDPNM_PN_ERA_CALC_ENABLED              [!//
[!IF "num:i(count(UdpNmChannelConfig/*[(node:exists(UdpNmPnEraCalcEnabled)) and (UdpNmPnEraCalcEnabled='true')])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined UDPNM_PN_RESET_TIME)
#error UDPNM_PN_RESET_TIME is already defined
#endif
/* !LINKSTO ECUC_UdpNm_00065,1 */
#define UDPNM_PN_RESET_TIME [!//
[!"num:i((UdpNmPnResetTime * 1000) div (UdpNmMainFunctionPeriod * 1000))"!]U

#if (defined UDPNM_PN_EIRA_PDUID)
#error UDPNM_PN_EIRA_PDUID is already defined
#endif
[!/* get Eira PduId by EcuC reference from PduR, existence is checked in xdm */!][!//
/* !LINKSTO ECUC_UdpNm_00062,1 */
[!IF "(node:exists(UdpNmPnEiraCalcEnabled)) and (UdpNmPnEiraCalcEnabled = 'true')"!][!//
#define UDPNM_PN_EIRA_PDUID  [!//
    [!"as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/PduRSrcPdu[PduRSrcPduRef=node:current()/UdpNmPnEiraRxNSduRef
]/PduRSourcePduHandleId"!]U/* UdpNmPnEraRx Id*/
[!ENDIF!]

#if (defined UDPNM_PN_SUPPORTED)
#error UDPNM_PN_SUPPORTED is already defined
#endif
#define UDPNM_PN_SUPPORTED          STD_ON
[!ELSE!]
#if (defined UDPNM_PN_SUPPORTED)
#error UDPNM_PN_SUPPORTED is already defined
#endif
#define UDPNM_PN_SUPPORTED          STD_OFF

#if (defined UDPNM_PN_EIRA_CALC_ENABLED)
#error UDPNM_PN_EIRA_CALC_ENABLED is already defined
#endif
#define UDPNM_PN_EIRA_CALC_ENABLED  STD_OFF

#if (defined UDPNM_PN_ERA_CALC_ENABLED)
#error UDPNM_PN_ERA_CALC_ENABLED is already defined
#endif
#define UDPNM_PN_ERA_CALC_ENABLED   STD_OFF
[!ENDIF!]

/*------------------[Symbolic Name for UdpNmRxPduId]------------------------*/
[!LOOP "UdpNmChannelConfig/*"!][!/*
*/!]
/* It is possible to change SHORT-NAME of containers with multiplicity
 * of one within EB tresos Studio. So, the symbolic name generation shall be
 * according to requirement [ecuc_sws_2108] if a SHORT-NAME for the container
 * is specified and the macro is defined as follows:
 *   #define UdpNmConf_UdpNmRxPdu_<CTR_SHORT_NAME>
 *
 * If a SHORT-NAME is not specified the macro is defined as follows:
 *   #define UdpNmConf_UdpNmChannelConfig_<CHANNEL_NAME>_<CTR_NAME>
 */
[!LOOP "UdpNmRxPdu/*"!][!//
[!CALL "GetSymbolNameRx","ShortNameRef"="'.'"!]
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined
#endif

/** \brief Symbolic name for the UdpNm channel "[!"name(../..)"!]" */
#define [!"$SymbolName"!]  [!"UdpNmRxPduId"!]U

#if (defined UDPNM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined [!"name(../..)"!]_[!"name(.)"!])
#error [!"name(../..)"!]_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value without
 *         prefix (AUTOSAR version <= 3.1 rev4) [!"UdpNmRxPduId"!] */
#define [!"name(../..)"!]_[!"name(.)"!]  [!"UdpNmRxPduId"!]U

#if (defined UdpNm_[!"name(../..)"!]_[!"name(.)"!])
#error UdpNm_[!"name(../..)"!]_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value with module abbreviation as prefix
 **        only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define UdpNm_[!"name(../..)"!]_[!"name(.)"!]  [!"UdpNmRxPduId"!]U
#endif /* UDPNM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//
[!ENDLOOP!][!//

/*------------[Symbolic Name for UdpNmTxConfirmationPduId]-------------------*/
/* !LINKSTO UdpNm.EB.SymbolicName.UdpNmTxConfirmationPduId.NoShortName,1 */
[!LOOP "UdpNmChannelConfig/*/UdpNmTxPdu"!][!/*
*/!]
/* It is possible to change short name values of containers with multiplicity
 * of one within EB tresos Studio. So, the symbolic name generation shall be
 * according to requirement [ecuc_sws_2108] if a SHORT-NAME for the container
 * is specified and the macro is defined as follows:
 *   #define UdpNmConf_UdpNmTxPdu_<CTR_SHORT_NAME>
 *
 * If a SHORT-NAME is not specified the macro is defined as follows:
 *   #define UdpNmConf_UdpNmChannelConfig_<CHANNEL_NAME>_<CTR_NAME>
 */
[!/* The LOOP over node-set excludes OPTIONAL containers from processing thus
   * the symbolic name macros should just be generated in case of their presence.
   */!]
[!CALL "GetSymbolNameTx","ShortNameRef"="'.'""!]
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined
#endif

/** \brief Symbolic name for the UdpNm channel "[!"name(..)"!]" */
#define [!"$SymbolName"!]  [!"UdpNmTxConfirmationPduId"!]U

#if (defined UDPNM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined [!"name(..)"!]_[!"name(.)"!])
#error [!"name(..)"!]_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value without
 *         prefix (AUTOSAR version <= 3.1 rev4) [!"UdpNmTxConfirmationPduId"!] */
#define [!"name(..)"!]_[!"name(.)"!]  [!"UdpNmTxConfirmationPduId"!]U

#if (defined UdpNm_[!"name(..)"!]_[!"name(.)"!])
#error UdpNm_[!"name(..)"!]_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value with module abbreviation as prefix
 **        only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define UdpNm_[!"name(..)"!]_[!"name(.)"!]  [!"UdpNmTxConfirmationPduId"!]U
#endif /* UDPNM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//

/*-------------[Symbolic Name for UdpNmTxUserDataPduId]--------------------*/
/* !LINKSTO UdpNm.EB.SymbolicName.UdpNmTxUserDataPduId.NoShortName,1 */
[!LOOP "UdpNmChannelConfig/*/UdpNmUserDataTxPdu"!][!/*
*/!]
/* It is possible to change short name values of containers with multiplicity
 * of one within EB tresos Studio. So, the symbolic name generation shall be
 * according to requirement [ecuc_sws_2108] if a SHORT-NAME for the container
 * is specified and the macro is defined as follows:
 *   #define UdpNmConf_UdpNmUserDataTxPdu_<CTR_SHORT_NAME>
 *
 * If a SHORT-NAME is not specified the macro is defined as follows:
 *   #define UdpNmConf_UdpNmChannelConfig_<CHANNEL_NAME>_<CTR_NAME>
 */
[!/* The LOOP over node-set excludes OPTIONAL containers from processing thus
   * the symbolic name macros should just be generated in case of their presence.
   */!]
[!CALL "GetSymbolNameTx","ShortNameRef"="'.'""!]
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined
#endif

/** \brief Symbolic name for the UdpNm channel "[!"name(..)"!]" */
#define [!"$SymbolName"!]  [!"UdpNmTxUserDataPduId"!]U

#if (defined UDPNM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined [!"name(..)"!]_[!"name(.)"!])
#error [!"name(..)"!]_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value without
 *         prefix (AUTOSAR version <= 3.1 rev4) [!"UdpNmTxUserDataPduId"!] */
#define [!"name(..)"!]_[!"name(.)"!]  [!"UdpNmTxUserDataPduId"!]U

#if (defined UdpNm_[!"name(..)"!]_[!"name(.)"!])
#error UdpNm_[!"name(..)"!]_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value with module abbreviation as prefix
 **        only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define UdpNm_[!"name(..)"!]_[!"name(.)"!]  [!"UdpNmTxUserDataPduId"!]U
#endif /* UDPNM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//

#if (defined UDPNM_PDU_BYTE_0)
#error UDPNM_PDU_BYTE_0 already defined
#endif /* if (defined UDPNM_PDU_BYTE_0) */

/** \brief Definition of UDPNM_PDU_BYTE_0.
 */
#define UDPNM_PDU_BYTE_0                                   0U

#if (defined UDPNM_PDU_BYTE_1)
#error UDPNM_PDU_BYTE_1 already defined
#endif /* if (defined UDPNM_PDU_BYTE_1) */

/** \brief Definition of UDPNM_PDU_BYTE_1.
 */
#define UDPNM_PDU_BYTE_1                                   1U

#if (defined UDPNM_PDU_OFF)
#error UDPNM_PDU_OFF already defined
#endif /* if (defined UDPNM_PDU_OFF) */

/** \brief Definition of UDPNM_PDU_OFF.
 */
#define UDPNM_PDU_OFF                                      3U
/*------------------[Hsm configuration]-------------------------------------*/

#if (defined UDPNM_HSM_INST_MULTI_ENABLED)
#error UDPNM_HSM_INST_MULTI_ENABLED already defined
#endif
#if (UDPNM_NUMBER_OF_CHANNELS > 1U)
#define UDPNM_HSM_INST_MULTI_ENABLED STD_ON
#else
#define UDPNM_HSM_INST_MULTI_ENABLED STD_OFF
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define UDPNM_START_SEC_CODE
#include <UdpNm_MemMap.h>

[!FOR "I"="0" TO "num:i(count(UdpNmChannelConfig/*)) - 1"!]
#ifndef RTE_SCHM_SWCBSW_UDPNM_MAINFUNCTION_[!"num:i($I)"!]
#define RTE_SCHM_SWCBSW_UDPNM_MAINFUNCTION_[!"num:i($I)"!]
/** \brief Main function of the UdpNm.
 **
 ** Main function of the UdpNm which processes the algorithm describes in that document.
 **
 ** \ServiceID{0x013}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
  /* Deviation MISRAC2012-1 */
  extern FUNC(void, UDPNM_CODE) UdpNm_MainFunction_[!"num:i($I)"!](void);
#endif
[!ENDFOR!]
#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>

/*==================[internal function definitions]=========================*/

[!ENDSELECT!][!//
#endif /* (UDPNM_ADAPTIVE == STD_ON) */
#endif /* ifndef UDPNM_CFG_H */
/*==================[end of file]===========================================*/
