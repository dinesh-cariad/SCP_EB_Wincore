/**
 * \file
 *
 * \brief AUTOSAR CanNm
 *
 * This file contains the implementation of the AUTOSAR
 * module CanNm.
 *
 * \version 6.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO CANNM304,1 */
#ifndef CANNM_CFG_H
#define CANNM_CFG_H

/*------------------[Defensive programming]---------------------------------*/
[!SELECT "CanNmDefensiveProgramming"!][!//

#if (defined CANNM_DEFENSIVE_PROGRAMMING_ENABLED)
#error CANNM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define CANNM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmDevErrorDetect = 'true') and (CanNmDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_PRECONDITION_ASSERT_ENABLED)
#error CANNM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define CANNM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmDevErrorDetect = 'true') and (CanNmDefProgEnabled = 'true') and (CanNmPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_POSTCONDITION_ASSERT_ENABLED)
#error CANNM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define CANNM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmDevErrorDetect  = 'true') and (CanNmDefProgEnabled = 'true') and (CanNmPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error CANNM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define CANNM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmDevErrorDetect  = 'true') and (CanNmDefProgEnabled = 'true') and (CanNmUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_INVARIANT_ASSERT_ENABLED)
#error CANNM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define CANNM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmDevErrorDetect  = 'true') and (CanNmDefProgEnabled = 'true') and (CanNmInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_STATIC_ASSERT_ENABLED)
#error CANNM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define CANNM_STATIC_ASSERT_ENABLED           [!//
[!IF "(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmDevErrorDetect  = 'true') and (CanNmDefProgEnabled = 'true') and (CanNmStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

[!/*  select first of the multiple config containers */!][!//
[!SELECT "CanNmGlobalConfig/*[1]"!][!//

/*==================[includes]==============================================*/

#include <TSAutosar.h>    /* EB specific standard types      */
/* !LINKSTO CanNm.EB.CanNmNodeIdCallbackHeaderName,1 */
[!IF "node:exists(CanNmNodeIdCallback)"!]
#include <[!"CanNmNodeIdCallbackHeader"!]>  /* CanNm node Id callback header */
[!ENDIF!]

#ifndef CANNM_NO_CFGCLASSMIX_REQUIRED
#include <CanNm_SymbolicNames_PBcfg.h>
#endif /* CANNM_NO_CFGCLASSMIX_REQUIRED */

#undef CANNM_NO_CFGCLASSMIX_REQUIRED

[!INCLUDE "../../generate_macros/CanNm_Macros.m"!][!//

/*==================[macros]================================================*/
#if (defined CANNM_CFG_SIGNATURE)
#error CANNM_CFG_SIGNATURE is already defined
#endif

#define CANNM_CFG_SIGNATURE [!"asc:getConfigSignature(as:modconf('CanNm')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U /* Compile time verification value */

#if (defined CANNM_PUBLIC_INFO_SIGNATURE)
#error CANNM_PUBLIC_INFO_SIGNATURE is already defined
#endif

#define CANNM_PUBLIC_INFO_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('CanNm')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('CanNm')[1]/CommonPublishedInformation/Release))"!]U /* PublicInfoSignature */

#if (defined CANNM_PBCFGM_SUPPORT_ENABLED)
#error CANNM_PBCFGM_SUPPORT_ENABLED is already defined
#endif
/* !LINKSTO CanNm.Impl.PbCfgM,1 */
#define CANNM_PBCFGM_SUPPORT_ENABLED [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('CanNm')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* ----- Pre-processor switch to enable/diable relocateable postbuild config ----- */
#if (defined CANNM_RELOCATABLE_CFG_ENABLE)
#error CANNM_RELOCATABLE_CFG_ENABLE is already defined
#endif

/* !LINKSTO CanNm.Impl.Relocatable,1 */
#if (CANNM_PBCFGM_SUPPORT_ENABLED == STD_ON)
#define CANNM_RELOCATABLE_CFG_ENABLE [!IF "node:exists(as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable) and (as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#else
#define CANNM_RELOCATABLE_CFG_ENABLE [!IF "as:modconf('CanNm')[1]/CanNmGeneral/CanNmRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#endif

#if (defined CANNM_VERSION_INFO_API)
#error CANNM_VERSION_INFO_API is already defined
#endif
/* !LINKSTO CANNM003_Conf,1 */
#define CANNM_VERSION_INFO_API            [!//
[!IF "CanNmVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_COM_CONTROL_ENABLED)
#error CANNM_COM_CONTROL_ENABLED is already defined
#endif
/* !LINKSTO CANNM013_Conf,1, CANNM168,1 */
#define CANNM_COM_CONTROL_ENABLED         [!//
[!IF "CanNmComControlEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_STATE_CHANGE_IND_ENABLED)
#error CANNM_STATE_CHANGE_IND_ENABLED is already defined
#endif
/* !LINKSTO CANNM012_Conf,1 */
#define CANNM_STATE_CHANGE_IND_ENABLED    [!//
[!IF "CanNmStateChangeIndEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_PASSIVE_MODE_ENABLED)
#error CANNM_PASSIVE_MODE_ENABLED is already defined
#endif
/* !LINKSTO CANNM010_Conf,1, CANNM161,1 */
#define CANNM_PASSIVE_MODE_ENABLED        [!//
[!IF "CanNmPassiveModeEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_PDU_RX_INDICATION_ENABLED)
#error CANNM_PDU_RX_INDICATION_ENABLED is already defined
#endif
/* !LINKSTO CANNM011_Conf,1 */
#define CANNM_PDU_RX_INDICATION_ENABLED   [!//
[!IF "CanNmPduRxIndicationEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_IMMEDIATE_RESTART_ENABLED)
#error CANNM_IMMEDIATE_RESTART_ENABLED is already defined
#endif
/* !LINKSTO CANNM009_Conf,1 */
#define CANNM_IMMEDIATE_RESTART_ENABLED   [!//
[!IF "CanNmImmediateRestartEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_IMMEDIATE_TXCONF_ENABLED)
#error CANNM_IMMEDIATE_TXCONF_ENABLED is already defined
#endif
/* !LINKSTO CANNM041_Conf,1, CANNM071,1 */
#define CANNM_IMMEDIATE_TXCONF_ENABLED    [!//
[!IF "CanNmImmediateTxconfEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_USER_DATA_ENABLED)
#error CANNM_USER_DATA_ENABLED is already defined
#endif
/* !LINKSTO CANNM004_Conf,1, CANNM158,1 */
#define CANNM_USER_DATA_ENABLED           [!//
[!IF "CanNmUserDataEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_COM_USER_DATA_SUPPORT)
#error CANNM_COM_USER_DATA_SUPPORT is already defined
#endif
/* !LINKSTO CANNM044_Conf,1 */
#define CANNM_COM_USER_DATA_SUPPORT       [!//
[!IF "CanNmComUserDataSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_REMOTE_SLEEP_IND_ENABLED)
#error CANNM_REMOTE_SLEEP_IND_ENABLED is already defined
#endif
/* !LINKSTO CANNM055_Conf,1, CANNM149,1 */
#define CANNM_REMOTE_SLEEP_IND_ENABLED    [!//
[!IF "CanNmRemoteSleepIndEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_NODE_ID_ENABLED)
#error CANNM_NODE_ID_ENABLED is already defined
#endif

/* !LINKSTO CanNm.EB.CanNmNodeIdEnabled,1 */
#define CANNM_NODE_ID_ENABLED             [!//
[!IF "count(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmChannelConfig/*[CanNmNodeIdEnabled='true']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined CANNM_REPEAT_MSG_IND_ENABLED)
#error CANNM_REPEAT_MSG_IND_ENABLED is already defined
#endif
/* !LINKSTO CANNM005_Conf,1 */
#define CANNM_REPEAT_MSG_IND_ENABLED      [!//
[!IF "count(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmChannelConfig/*[CanNmRepeatMsgIndEnabled='true']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_NODE_DETECTION_ENABLED)
#error CANNM_NODE_DETECTION_ENABLED is already defined
#endif
/* !LINKSTO CANNM007_Conf,1 */
#define CANNM_NODE_DETECTION_ENABLED      [!//
[!IF "count(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmChannelConfig/*[CanNmNodeDetectionEnabled='true']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_BUS_SYNCHRONIZATION_ENABLED)
#error CANNM_BUS_SYNCHRONIZATION_ENABLED is already defined
#endif
/* !LINKSTO CANNM006_Conf,1, CANNM185,1 */
#define CANNM_BUS_SYNCHRONIZATION_ENABLED [!//
[!IF "CanNmBusSynchronizationEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_BUS_LOAD_REDUCTION_ENABLED)
#error CANNM_BUS_LOAD_REDUCTION_ENABLED is already defined
#endif
/* !LINKSTO CANNM040_Conf,1, CANNM052,1 */
#define CANNM_BUS_LOAD_REDUCTION_ENABLED  [!//
[!IF "CanNmBusLoadReductionEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]




#if (defined CANNM_IMMEDIATE_TRANSMISSION)
#error CANNM_IMMEDIATE_TRANSMISSION is already defined
#endif
#define CANNM_IMMEDIATE_TRANSMISSION  [!//
[!IF "num:i(count(CanNmChannelConfig/*[CanNmImmediateNmTransmissions > 0])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_DEV_ERROR_DETECT)
#error CANNM_DEV_ERROR_DETECT is already defined
#endif
/** \brief Switch for DET usage */
/* !LINKSTO CANNM002_Conf,1, CANNM188,1 */
#define CANNM_DEV_ERROR_DETECT            [!//
[!IF "CanNmDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_NUMBER_OF_CHANNELS)
#error CANNM_NUMBER_OF_CHANNELS is already defined
#endif
/* !LINKSTO CANNM014_Conf,1 */
/** \brief Maximum number of channels */
#define CANNM_NUMBER_OF_CHANNELS               [!//
[!"num:i(count(CanNmChannelConfig/*))"!]U

#if (defined CANNM_INDEXFROMNMCHANNELHANDLE_NUM)
#error CANNM_INDEXFROMNMCHANNELHANDLE_NUM is already defined
#endif
/** \brief Number of elements in array CanNm_IndexFromComMChannelHandle[] */
#define CANNM_INDEXFROMNMCHANNELHANDLE_NUM     [!//
[!"num:i(num:max(node:refs(CanNmChannelConfig/*/CanNmComMNetworkHandleRef)/ComMChannelId)+1)"!]U

#if (defined CANNM_ACTIVE_WAKEUP_BIT_ENABLED)
#error CANNM_ACTIVE_WAKEUP_BIT_ENABLED is already defined
#endif
/* !LINKSTO CanNm.CanNmActiveWakeupBitEnabled,1 */
/** \brief Macro to indicate if Active wake up bit is present in CBV */
#define CANNM_ACTIVE_WAKEUP_BIT_ENABLED        [!//
[!IF "CanNmActiveWakeupBitEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANNM_NODEID_CALLBACK_ENABLED)
#error CANNM_NODEID_CALLBACK_ENABLED is already defined
#endif
/* !LINKSTO CanNm.EB.CanNmNodeIdCallbackName,1 */
/** \brief Macro to indicate if Node Id callback is enabled */
#define CANNM_NODEID_CALLBACK_ENABLED          [!//
[!IF "node:exists(CanNmNodeIdCallback)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "node:exists(CanNmNodeIdCallback)"!]
#if (defined CANNM_NODEID_CALLBACK_NAME)
#error CANNM_NODEID_CALLBACK_NAME is already defined
#endif
/** \brief Name of Node Id callback function */
#define CANNM_NODEID_CALLBACK_NAME   [!"CanNmNodeIdCallback"!]
[!ENDIF!]

#if (defined CANNM_CARWAKEUP_RX_ENABLED)
#error CANNM_CARWAKEUP_RX_ENABLED is already defined
#endif

#define CANNM_CARWAKEUP_RX_ENABLED        [!//
[!IF "count(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmChannelConfig/*[CanNmCarWakeUpRxEnabled='true']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/*------------------[PN Extensions]-------------------------------------*/
[!IF "as:modconf('CanNm')[1]/CanNmGeneral/CanNmPnSupported = 'true'"!]
/* !LINKSTO CANNM415,1 */
/* !LINKSTO ECUC_CanNm_00061,1 */
#if (defined CANNM_PN_INFO_LENGTH)
#error CANNM_PN_INFO_LENGTH is already defined
#endif
#define CANNM_PN_INFO_LENGTH    [!"CanNmPnInfo/CanNmPnInfoLength"!]U

/* Size of array for mapping PN EIRA timer */
[!//
[!// Calculate size of array required for EIRA timer
[!IF "((node:exists(CanNmPnEiraCalcEnabled)) and (CanNmPnEiraCalcEnabled = 'true')) or  num:i(count(CanNmChannelConfig/*[(node:exists(CanNmPnEraCalcEnabled)) and (CanNmPnEraCalcEnabled='true')])) > 0"!][!//
[!NOCODE!][!//
[!VAR "EiraMaxTimerSize" = "0"!][!//
[!IF "node:exists(as:modconf('CanNm')[1]/CanNmGeneral/CanNmMaxPn)"!][!//
[!VAR "EiraMaxTimerSize" = "node:value(as:modconf('CanNm')[1]/CanNmGeneral/CanNmMaxPn)"!][!//
[!ELSE!][!//
[!VAR "EiraMaxTimerSize" = "node:value(CanNmPnInfo/CanNmPnInfoLength) * 8"!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

#if (defined CANNM_EIRA_MAX_TIMER_SIZE)
#error CANNM_EIRA_MAX_TIMER_SIZE is already defined
#endif

#define CANNM_EIRA_MAX_TIMER_SIZE    [!"num:i($EiraMaxTimerSize)"!]U  /* CanNm Eira Maximum Timer array size */
[!ENDIF!]

/* !LINKSTO CANNM060_Conf_Refine,1 */
#if (defined CANNM_PN_INFO_OFFSET)
#error CANNM_PN_INFO_OFFSET is already defined
#endif
#define CANNM_PN_INFO_OFFSET     [!//
              [!"CanNmPnInfo/CanNmPnInfoOffset"!]U

/** CanNmPnEiraCalcEnabled **/
/* !LINKSTO CANNM070_Conf,1 */
#if (defined CANNM_PN_EIRA_CALC_ENABLED)
#error CANNM_PN_EIRA_CALC_ENABLED is already defined
#endif
#define CANNM_PN_EIRA_CALC_ENABLED             [!//
[!IF "(node:exists(CanNmPnEiraCalcEnabled)) and (CanNmPnEiraCalcEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** CanNmPnEraCalcEnabled **/
/* !LINKSTO CANNM067_Conf,1 */
#if (defined CANNM_PN_ERA_CALC_ENABLED)
#error CANNM_PN_ERA_CALC_ENABLED is already defined
#endif

#define CANNM_PN_ERA_CALC_ENABLED              [!//
[!IF "num:i(count(CanNmChannelConfig/*[CanNmPnEraCalcEnabled='true'])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** CanNmPnResetTime **/
#if (defined CANNM_PN_RESET_TIME)
#error CANNM_PN_RESET_TIME is already defined
#endif
#define CANNM_PN_RESET_TIME [!//
    [!"num:i((CanNmPnResetTime * 1000) div (CanNmMainFunctionPeriod * 1000))"!]U

[!/* get Eira PduId by EcuC reference from PduR, existence is checked in xdm */!][!//
[!IF "CanNmPnEiraCalcEnabled = 'true'"!][!//
#if (defined CANNM_PN_EIRA_PDUID)
#error CANNM_PN_EIRA_PDUID is already defined
#endif
#define CANNM_PN_EIRA_PDUID  [!//
    [!"as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/PduRSrcPdu[PduRSrcPduRef=node:current()/CanNmPnEiraRxNSduRef
]/PduRSourcePduHandleId"!]U/* CanNmPnEraRx Id*/
[!ENDIF!]

#define CANNM_PN_SUPPORTED  STD_ON
[!ELSE!]
#if (defined CANNM_PN_SUPPORTED)
#error CANNM_PN_SUPPORTED is already defined
#endif
#define CANNM_PN_SUPPORTED  STD_OFF

#if (defined CANNM_PN_EIRA_CALC_ENABLED)
#error CANNM_PN_EIRA_CALC_ENABLED is already defined
#endif
#define CANNM_PN_EIRA_CALC_ENABLED STD_OFF

#if (defined CANNM_PN_ERA_CALC_ENABLED)
#error CANNM_PN_ERA_CALC_ENABLED is already defined
#endif
#define CANNM_PN_ERA_CALC_ENABLED STD_OFF
[!ENDIF!]

/** \brief Enable multicore support*/
#if (defined CANNM_MULTICORE_ENABLED)
#error CANNM_MULTICORE_ENABLED already defined
#endif

/* !LINKSTO CanNm.Config.CanNmMultiCoreSupport,1 */
#define CANNM_MULTICORE_ENABLED           [!//
[!IF "as:modconf('CanNm')[1]/CanNmGeneral/CanNmMultiCoreSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/*------------------[Symbolic Name for CanNmRxPduId]------------------------*/
[!LOOP "CanNmChannelConfig/*"!][!/*
*/!]
/* !LINKSTO CanNm.EB.SymbolicName.CanNmRxPduId.NoShortName,1, CanNm.EB.SymbolicName.ecuc_sws_2108_3,1 */
/* It is possible to change SHORT-NAME of containers with multiplicity
 * of one within EB tresos Studio. So, the symbolic name generation shall be
 * according to requirement [ecuc_sws_2108] if a SHORT-NAME for the container
 * is specified and the macro is defined as follows:
 *   #define CanNmConf_CanNmRxPdu_<CTR_SHORT_NAME>
 *
 * If a SHORT-NAME is not specified the macro is defined as follows:
 *   #define CanNmConf_CanNmChannelConfig_<CHANNEL_NAME>_<CTR_NAME>
 */
[!LOOP "CanNmRxPdu/*"!][!//
[!CALL "GetSymbolNameRx","ShortNameRef"="'.'"!]
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined
#endif

/** \brief Symbolic name for the CanNm channel "[!"name(../..)"!]" */
#define [!"$SymbolName"!]  [!"CanNmRxPduId"!]U

#if (defined CANNM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined [!"name(../..)"!]_[!"name(.)"!])
#error [!"name(../..)"!]_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) [!"CanNmRxPduId"!] */
#define [!"name(../..)"!]_[!"name(.)"!] [!"CanNmRxPduId"!]U

#if (defined CanNm_[!"name(../..)"!]_[!"name(.)"!])
#error CanNm_[!"name(../..)"!]_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value with module abbreviation as prefix
 ** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define CanNm_[!"name(../..)"!]_[!"name(.)"!] [!"CanNmRxPduId"!]U
#endif /* CANNM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//
[!ENDLOOP!][!//

/*------------[Symbolic Name for CanNmTxConfirmationPduId]-------------------*/
[!LOOP "CanNmChannelConfig/*/CanNmTxPdu"!][!/*
*/!]
/* !LINKSTO CanNm.EB.SymbolicName.ecuc_sws_2108_1,2, CanNm.EB.SymbolicName.CanNmTxConfirmationPduId.NoShortName,1 */
/* It is possible to change short name values of containers with multiplicity
 * of one within EB tresos Studio. So, the symbolic name generation shall be
 * according to requirement [ecuc_sws_2108] if a SHORT-NAME for the container
 * is specified and the macro is defined as follows:
 *   #define CanNmConf_CanNmTxPdu_<CTR_SHORT_NAME>
 *
 * If a SHORT-NAME is not specified the macro is defined as follows:
 *   #define CanNmConf_CanNmChannelConfig_<CHANNEL_NAME>_<CTR_NAME>
 */
[!/* The LOOP over node-set excludes OPTIONAL containers from processing thus
   * the symbolic name macros should just be generated in case of their presence.
   */!]
[!CALL "GetSymbolNameTx","ShortNameRef"="'.'""!]
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined
#endif

/** \brief Symbolic name for the CanNm channel "[!"name(..)"!]" */
#define [!"$SymbolName"!]  [!"CanNmTxConfirmationPduId"!]U

#if (defined CANNM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined [!"name(..)"!]_[!"name(.)"!])
#error [!"name(..)"!]_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) [!"CanNmTxConfirmationPduId"!] */
#define [!"name(..)"!]_[!"name(.)"!]  [!"CanNmTxConfirmationPduId"!]U

#if (defined CanNm_[!"name(..)"!]_[!"name(.)"!])
#error CanNm_[!"name(..)"!]_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value with module abbreviation as prefix
 ** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define CanNm_[!"name(..)"!]_[!"name(.)"!]  [!"CanNmTxConfirmationPduId"!]U
#endif /* CANNM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//

/*-------------[Symbolic Name for CanNmTxUserDataPduId]--------------------*/
[!LOOP "CanNmChannelConfig/*/CanNmUserDataTxPdu"!][!/*
*/!]
/* !LINKSTO CanNm.EB.SymbolicName.ecuc_sws_2108_2,1 */
/* !LINKSTO CanNm.EB.SymbolicName.CanNmTxUserDataPduId.NoShortName,1 */
/* It is possible to change short name values of containers with multiplicity
 * of one within EB tresos Studio. So, the symbolic name generation shall be
 * according to requirement [ecuc_sws_2108] if a SHORT-NAME for the container
 * is specified and the macro is defined as follows:
 *   #define CanNmConf_CanNmUserDataTxPdu_<CTR_SHORT_NAME>
 *
 * If a SHORT-NAME is not specified the macro is defined as follows:
 *   #define CanNmConf_CanNmChannelConfig_<CHANNEL_NAME>_<CTR_NAME>
 */
[!/* The LOOP over node-set excludes OPTIONAL containers from processing thus
   * the symbolic name macros should just be generated in case of their presence.
   */!]
[!CALL "GetSymbolNameTx","ShortNameRef"="'.'""!]
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined
#endif

/** \brief Symbolic name for the CanNm channel "[!"name(..)"!]" */
#define [!"$SymbolName"!]  [!"CanNmTxUserDataPduId"!]U

#if (defined CANNM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined [!"name(..)"!]_[!"name(.)"!])
#error [!"name(..)"!]_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) [!"CanNmTxUserDataPduId"!] */
#define [!"name(..)"!]_[!"name(.)"!]  [!"CanNmTxUserDataPduId"!]U

#if (defined CanNm_[!"name(..)"!]_[!"name(.)"!])
#error CanNm_[!"name(..)"!]_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value with module abbreviation as prefix
 ** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define CanNm_[!"name(..)"!]_[!"name(.)"!]  [!"CanNmTxUserDataPduId"!]U
#endif /* CANNM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//

/*------------------[Hsm configuration]-------------------------------------*/

#if (defined CANNM_HSM_INST_MULTI_ENABLED)
#error CANNM_HSM_INST_MULTI_ENABLED already defined
#endif

#if (CANNM_NUMBER_OF_CHANNELS > 1U)
#define CANNM_HSM_INST_MULTI_ENABLED STD_ON
#else
#define CANNM_HSM_INST_MULTI_ENABLED STD_OFF
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

[!ENDSELECT!][!//
#endif /* ifndef CANNM_CFG_H */
/*==================[end of file]===========================================*/
