/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* !LINKSTO FRNM369,1 */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.5 (required)
 * An external object or function shall be declared in one and only one file.
 *
 * Reason:
 * The files SchM_FrNm.h and FrNm.h contain declarations of FrNm_MainFunction. This should be fixed
 * in a future version of the Rte.
 */

[!INCLUDE "../../generate_macros/FrNm_Macros.m"!][!//
#ifndef FRNM_CFG_H
#define FRNM_CFG_H

/*------------------[Defensive programming]---------------------------------*/
[!SELECT "FrNmDefensiveProgramming"!][!//

#if (defined FRNM_DEFENSIVE_PROGRAMMING_ENABLED)
#error FRNM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define FRNM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalProperties/FrNmDevErrorDetect = 'true') and (FrNmDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_PRECONDITION_ASSERT_ENABLED)
#error FRNM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define FRNM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalProperties/FrNmDevErrorDetect = 'true') and (FrNmDefProgEnabled = 'true') and (FrNmPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_POSTCONDITION_ASSERT_ENABLED)
#error FRNM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define FRNM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalProperties/FrNmDevErrorDetect  = 'true') and (FrNmDefProgEnabled = 'true') and (FrNmPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error FRNM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define FRNM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalProperties/FrNmDevErrorDetect  = 'true') and (FrNmDefProgEnabled = 'true') and (FrNmUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_INVARIANT_ASSERT_ENABLED)
#error FRNM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define FRNM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalProperties/FrNmDevErrorDetect  = 'true') and (FrNmDefProgEnabled = 'true') and (FrNmInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_STATIC_ASSERT_ENABLED)
#error FRNM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define FRNM_STATIC_ASSERT_ENABLED           [!//
[!IF "(as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalProperties/FrNmDevErrorDetect  = 'true') and (FrNmDefProgEnabled = 'true') and (FrNmStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

/*==================[includes]==============================================*/

#include <TSAutosar.h>    /* EB specific standard types */

#ifndef FRNM_NO_CFGCLASSMIX_REQUIRED
#include <FrNm_SymbolicNames_PBcfg.h>
#endif /* FRNM_NO_CFGCLASSMIX_REQUIRED */

#undef FRNM_NO_CFGCLASSMIX_REQUIRED

/*==================[macros]================================================*/

#if (defined FRNM_CFG_SIGNATURE)
#error FRNM_CFG_SIGNATURE is already defined
#endif
#define FRNM_CFG_SIGNATURE [!"asc:getConfigSignature(as:modconf('FrNm')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U /* Compile time verification value */

#if (defined FRNM_PUBLIC_INFO_SIGNATURE)
#error FRNM_PUBLIC_INFO_SIGNATURE is already defined
#endif
#define FRNM_PUBLIC_INFO_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('FrNm')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('FrNm')[1]/CommonPublishedInformation/Release))"!]U /* PublicInfoSignature */

/* !LINKSTO FrNm.Impl.PbCfgM,1 */
#if (defined FRNM_PBCFGM_SUPPORT_ENABLED)
#error FRNM_PBCFGM_SUPPORT_ENABLED is already defined
#endif
#define FRNM_PBCFGM_SUPPORT_ENABLED [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('FrNm')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* ----- Pre-processor switch to enable/diable relocateable postbuild config ----- */
/* !LINKSTO FrNm.Impl.Relocatable,1 */
#if (defined FRNM_RELOCATABLE_CFG_ENABLE)
#error FRNM_RELOCATABLE_CFG_ENABLE is already defined
#endif
#if (FRNM_PBCFGM_SUPPORT_ENABLED == STD_ON)
#define FRNM_RELOCATABLE_CFG_ENABLE [!IF "node:exists(as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable) and (as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#else
#define FRNM_RELOCATABLE_CFG_ENABLE [!IF "as:modconf('FrNm')[1]/FrNmGeneral/FrNmRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#endif

[!SELECT "FrNmGlobalConfig/FrNmGlobalProperties"!][!//
/** \brief Macro switch for version info API usage */
#if (defined FRNM_VERSION_INFO_API)
#error FRNM_VERSION_INFO_API is already defined
#endif
#define FRNM_VERSION_INFO_API            [!//
[!IF "FrNmVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Switch for DET usage */
#if (defined FRNM_DEV_ERROR_DETECT)
#error FRNM_DEV_ERROR_DETECT is already defined
#endif
#define FRNM_DEV_ERROR_DETECT            [!//
[!IF "FrNmDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_MAIN_ACROSS_FR_CYCLE)
#error FRNM_MAIN_ACROSS_FR_CYCLE is already defined
#endif
#define FRNM_MAIN_ACROSS_FR_CYCLE            [!//
[!IF "FrNmMainAcrossFrCycle = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

[!SELECT "FrNmGlobalConfig/FrNmGlobalFeatures"!][!//
#if (defined FRNM_STATE_CHANGE_INDICATION_ENABLED)
#error FRNM_STATE_CHANGE_INDICATION_ENABLED is already defined
#endif
#define FRNM_STATE_CHANGE_INDICATION_ENABLED    [!//
[!IF "FrNmStateChangeIndicationEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_PASSIVE_MODE_ENABLED)
#error FRNM_PASSIVE_MODE_ENABLED is already defined
#endif
#define FRNM_PASSIVE_MODE_ENABLED        [!//
[!IF "FrNmPassiveModeEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_PDU_RX_INDICATION_ENABLED)
#error FRNM_PDU_RX_INDICATION_ENABLED is already defined
#endif
#define FRNM_PDU_RX_INDICATION_ENABLED   [!//
[!IF "FrNmPduRxIndicationEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_USER_DATA_ENABLED)
#error FRNM_USER_DATA_ENABLED is already defined
#endif
#define FRNM_USER_DATA_ENABLED           [!//
[!IF "FrNmUserDataEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "FrNmPassiveModeEnabled = 'true'"!]
/* FrNm0040_Conf : If passive mode is enabled, then FrNmNodeDetectionEnabled is false.*/
#if (defined FRNM_NODE_DETECTION_ENABLED)
#error FRNM_NODE_DETECTION_ENABLED is already defined
#endif
#define FRNM_NODE_DETECTION_ENABLED STD_OFF

/* FrNm0040_Conf:Value of FrNmSourceNodeIdentifierEnabled is same as FrNmNodeDetectionEnabled.*/
#if (defined FRNM_SOURCE_NODE_IDENTIFIER_ENABLED)
#error FRNM_SOURCE_NODE_IDENTIFIER_ENABLED is already defined
#endif
#define FRNM_SOURCE_NODE_IDENTIFIER_ENABLED  STD_OFF

/*FrNm0040_Conf : FrNmControlBitVectorEnabled has dependency to FrNmNodeDetectionEnabled */
#if (defined FRNM_CONTROL_BIT_VECTOR_ENABLED)
#error FRNM_CONTROL_BIT_VECTOR_ENABLED is already defined
#endif
#define FRNM_CONTROL_BIT_VECTOR_ENABLED STD_OFF
[!ELSE!]
#if (defined FRNM_NODE_DETECTION_ENABLED)
#error FRNM_NODE_DETECTION_ENABLED is already defined
#endif
#define FRNM_NODE_DETECTION_ENABLED      [!//
[!IF "count(as:modconf('FrNm')[1]/FrNmChannelConfig/*/FrNmChannel/*[FrNmChannelIdentifiers/FrNmNodeDetectionEnabled = 'true']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_SOURCE_NODE_IDENTIFIER_ENABLED)
#error FRNM_SOURCE_NODE_IDENTIFIER_ENABLED is already defined
#endif
#define FRNM_SOURCE_NODE_IDENTIFIER_ENABLED   [!//
[!IF "count(as:modconf('FrNm')[1]/FrNmChannelConfig/*/FrNmChannel/*[FrNmChannelIdentifiers/FrNmSourceNodeIdentifierEnabled = 'true']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_CONTROL_BIT_VECTOR_ENABLED)
#error FRNM_CONTROL_BIT_VECTOR_ENABLED is already defined
#endif
#define FRNM_CONTROL_BIT_VECTOR_ENABLED      [!//
[!IF "FrNmControlBitVectorEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDIF!]
#if (defined FRNM_BUS_SYNCHRONIZATION_ENABLED)
#error FRNM_BUS_SYNCHRONIZATION_ENABLED is already defined
#endif
#define FRNM_BUS_SYNCHRONIZATION_ENABLED [!//
[!IF "FrNmBusSynchronizationEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_REPEAT_MSG_BIT_ENABLED)
#error FRNM_REPEAT_MSG_BIT_ENABLED is already defined
#endif
#define FRNM_REPEAT_MSG_BIT_ENABLED      [!//
[!IF "FrNmRepeatMessageBitEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_DUAL_CHANNEL_PDU_ENABLE)
#error FRNM_DUAL_CHANNEL_PDU_ENABLE is already defined
#endif
#define FRNM_DUAL_CHANNEL_PDU_ENABLE      [!//
[!IF "FrNmDualChannelPduEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_COM_USER_DATA_SUPPORT)
#error FRNM_COM_USER_DATA_SUPPORT is already defined
#endif
#define FRNM_COM_USER_DATA_SUPPORT      [!//
[!IF "FrNmComUserDataSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_CYCLE_COUNTER_EMULATION)
#error FRNM_CYCLE_COUNTER_EMULATION is already defined
#endif
#define FRNM_CYCLE_COUNTER_EMULATION      [!//
[!IF "FrNmCycleCounterEmulation = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_HW_VOTE_ENABLE)
#error FRNM_HW_VOTE_ENABLE is already defined
#endif
#define FRNM_HW_VOTE_ENABLE      [!//
[!IF "FrNmHwVoteEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_REMOTE_SLEEP_INDICATION_ENABLED)
#error FRNM_REMOTE_SLEEP_INDICATION_ENABLED is already defined
#endif
#define FRNM_REMOTE_SLEEP_INDICATION_ENABLED      [!//
[!IF "FrNmRemoteSleepIndicationEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_COORDINATOR_SYNC_SUPPORT)
#error FRNM_COORDINATOR_SYNC_SUPPORT is already defined
#endif
#define FRNM_COORDINATOR_SYNC_SUPPORT  [!//
[!IF "FrNmCoordinatorSyncSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* !LINKSTO FrNm.Config.FrNmMultiCoreSupport,1 */
/** \brief Enable multicore support */
#if (defined FRNM_MULTICORE_ENABLED)
#error FRNM_MULTICORE_ENABLED already defined
#endif
#define FRNM_MULTICORE_ENABLED           [!//
[!IF "as:modconf('FrNm')[1]/FrNmGeneral/FrNmMultiCoreSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** FrNmVoteBitValue **/
[!IF "node:exists(FrNmVoteBitValue)"!]
#if (defined FRNM_FORCE_VOTE_BIT)
#error FRNM_FORCE_VOTE_BIT is already defined
#endif
#define FRNM_FORCE_VOTE_BIT      STD_ON

#if (defined FRNM_VOTE_BIT)
#error FRNM_VOTE_BIT is already defined
#endif
#define FRNM_VOTE_BIT      [!//
  [!IF "num:i(FrNmVoteBitValue) = 0"!][!/*
  */!]0x00U[!//
  [!ELSE!][!/*
  */!]0x07U[!//
  [!ENDIF!]
[!ELSE!]
#if (defined FRNM_FORCE_VOTE_BIT)
#error FRNM_FORCE_VOTE_BIT is already defined
#endif
#define FRNM_FORCE_VOTE_BIT      STD_OFF
[!ENDIF!]

#if (defined FRNM_SYNCH_ERR_EXTENDED)
#error FRNM_SYNCH_ERR_EXTENDED is already defined
#endif
#define FRNM_SYNCH_ERR_EXTENDED  [!//
[!IF "(node:exists(FrNmSynchErrExtended)) and (FrNmSynchErrExtended = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

#if (defined FRNM_SYNCHRONIZATIONPOINT_ENABLED)
#error FRNM_SYNCHRONIZATIONPOINT_ENABLED is already defined
#endif
#define FRNM_SYNCHRONIZATIONPOINT_ENABLED   [!//
[!IF "num:i(count(FrNmChannelConfig/*/FrNmChannel/*/FrNmChannelIdentifiers[FrNmSynchronizationPointEnabled = 'true'])) != 0"!][!//
STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_REPEAT_MESSAGE_BIT_ACTIVE)
#error FRNM_REPEAT_MESSAGE_BIT_ACTIVE is already defined
#endif
#define FRNM_REPEAT_MESSAGE_BIT_ACTIVE  [!//
[!IF "num:i(count(FrNmChannelConfig/*/FrNmChannel/*/FrNmChannelIdentifiers[FrNmRepeatMessageBitActive = 'true'])) != 0"!][!//
STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_NUMBER_OF_CLUSTERS)
#error FRNM_NUMBER_OF_CLUSTERS is already defined
#endif
#define FRNM_NUMBER_OF_CLUSTERS   [!//
[!"num:i(FrNmGlobalConfig/FrNmGlobalConstants/FrNmNumberOfClusters)"!]U


/** \brief Maximum number of Active channels */
#if (defined FRNM_NUMBER_OF_CHANNELS)
#error FRNM_NUMBER_OF_CHANNELS is already defined
#endif
#define FRNM_NUMBER_OF_CHANNELS               [!//
[!"num:i(count(FrNmChannelConfig/*/FrNmChannel/*))"!]U

#if (defined FRNM_NUMBER_OF_RX_PDUS)
#error FRNM_NUMBER_OF_RX_PDUS is already defined
#endif
#define FRNM_NUMBER_OF_RX_PDUS   [!//
[!"num:i(count(FrNmChannelConfig/*/FrNmChannel/*/FrNmChannelIdentifiers/FrNmRxPdu/*))"!]U

#if (defined FRNM_NUMBER_OF_TX_PDUS)
#error FRNM_NUMBER_OF_TX_PDUS is already defined
#endif
#define FRNM_NUMBER_OF_TX_PDUS   [!//
[!"num:i(count(FrNmChannelConfig/*/FrNmChannel/*/FrNmChannelIdentifiers/FrNmTxPdu/*))"!]U

[!IF "num:i(count(FrNmChannelConfig/*/FrNmChannel/*/FrNmChannelIdentifiers/FrNmTxPdu/*)) !=0"!]
/* Maximum size of associated Pdus. This size is used to define local buffer when sending data */
#if (defined FRNM_TX_BUFFER_SIZE)
#error FRNM_TX_BUFFER_SIZE is already defined
#endif
#define FRNM_TX_BUFFER_SIZE  8U
[!ENDIF!]

[!LOOP "FrNmChannelConfig/*/FrNmChannel/*/*/FrNmRxPdu/*"!][!//

#if (defined FrNmConf_[!"name(..)"!]_[!"name(.)"!])
#error FrNmConf_[!"name(..)"!]_[!"name(.)"!] already defined
#endif

/** \brief Symbolic name of FrNmRxPdu with Id "[!"FrNmRxPduId"!]" for
 *  the channel [!"name(../../..)"!] */
#define FrNmConf_[!"name(..)"!]_[!"name(.)"!]   [!"FrNmRxPduId"!]U

#if (defined FRNM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined [!"name(.)"!])
#error [!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value without prefix
 * (AUTOSAR version <= 3.1 rev4) [!"FrNmRxPduId"!] */
#define [!"name(.)"!]                     [!"FrNmRxPduId"!]U

#if (defined FrNm_[!"name(.)"!])
#error FrNm_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value with module abbreviation as prefix
 ** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define FrNm_[!"name(.)"!]   [!"FrNmRxPduId"!]U
#endif /* FRNM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!]


[!LOOP "FrNmChannelConfig/*/FrNmChannel/*/*/FrNmTxPdu/*"!][!//

#if (defined FrNmConf_[!"name(..)"!]_[!"name(.)"!])
#error FrNmConf_[!"name(..)"!]_[!"name(.)"!] already defined
#endif

/** \brief Symbolic name of FrNmRxPdu with Id "[!"FrNmTxConfirmationPduId"!]"
 *  for the channel [!"name(../../..)"!] */
#define FrNmConf_[!"name(..)"!]_[!"name(.)"!]   [!"FrNmTxConfirmationPduId"!]U

#if (defined FRNM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined [!"name(.)"!])
#error [!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value without prefix
 * (AUTOSAR version <= 3.1 rev4) [!"FrNmTxConfirmationPduId"!] */
#define [!"name(.)"!]                     [!"FrNmTxConfirmationPduId"!]U

#if (defined FrNm_[!"name(.)"!])
#error FrNm_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value with module abbreviation as prefix
 ** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define FrNm_[!"name(.)"!]   [!"FrNmTxConfirmationPduId"!]U
#endif /* FRNM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!]

[!LOOP "FrNmChannelConfig/*/FrNmChannel/*/FrNmChannelIdentifiers/FrNmUserDataTxPdu"!][!//
[!CALL "GetSymbolName","ShortNameRef"="'.'"!][!//

#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined
#endif

/** \brief Symbolic name of FrNmTxUserDataPduId with Id
 * "[!"FrNmTxUserDataPduId"!]" for the channel [!"name(../..)"!]
 */
#define [!"$SymbolName"!]   [!"FrNmTxUserDataPduId"!]U

#if (defined FRNM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined [!"name(../..)"!]_FrNmUserDataTxPdu)
#error [!"name(../..)"!]_FrNmUserDataTxPdu already defined
#endif

/** \brief Export symbolic name value without prefix
 * (AUTOSAR version <= 3.1 rev4) [!"FrNmTxUserDataPduId"!]
 */
#define [!"name(../..)"!]_FrNmUserDataTxPdu           [!"FrNmTxUserDataPduId"!]U

#if (defined FrNm_[!"name(../..)"!]_FrNmUserDataTxPdu)
#error FrNm_[!"name(../..)"!]_FrNmUserDataTxPdu already defined
#endif

/** \brief Export symbolic name value with module abbreviation as prefix
 ** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2)
 */
#define FrNm_[!"name(../..)"!]_FrNmUserDataTxPdu   [!"FrNmTxUserDataPduId"!]U
#endif /* FRNM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!]

[!LOOP "FrNmChannelConfig/*/FrNmChannel/*/FrNmChannelIdentifiers/FrNmUserDataRxPdu"!][!//
[!CALL "GetSymbolName","ShortNameRef"="'.'"!][!//

#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined
#endif

/** \brief Symbolic name of FrNmRxUserDataPduId with Id
 * "[!"FrNmRxUserDataPduId"!]" for the channel [!"name(../..)"!]
 */
#define [!"$SymbolName"!]   [!"FrNmRxUserDataPduId"!]U

#if (defined FRNM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined [!"name(../..)"!]_FrNmUserDataRxPdu)
#error [!"name(../..)"!]_FrNmUserDataRxPdu already defined
#endif

/** \brief Export symbolic name value without prefix
 * (AUTOSAR version <= 3.1 rev4) [!"FrNmRxUserDataPduId"!]
 */
#define [!"name(../..)"!]_FrNmUserDataRxPdu           [!"FrNmRxUserDataPduId"!]U

#if (defined FrNm_[!"name(../..)"!]_FrNmUserDataRxPdu)
#error FrNm_[!"name(../..)"!]_FrNmUserDataRxPdu already defined
#endif

/** \brief Export symbolic name value with module abbreviation as prefix
 ** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2)
 */
#define FrNm_[!"name(../..)"!]_FrNmUserDataRxPdu   [!"FrNmRxUserDataPduId"!]U
#endif /* FRNM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!]

#if (defined FRNM_ACTIVE_WAKEUP_BIT_ENABLED)
#error FRNM_ACTIVE_WAKEUP_BIT_ENABLED is already defined
#endif
#define FRNM_ACTIVE_WAKEUP_BIT_ENABLED [!//
[!IF "num:i(count(as:modconf('FrNm')[1]/FrNmChannelConfig/*/FrNmChannel/*[(node:exists(FrNmChannelIdentifiers/FrNmActiveWakeupBitEnabled)) and (FrNmChannelIdentifiers/FrNmActiveWakeupBitEnabled = 'true')])) != 0"!][!//
STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRNM_CARWAKEUP_RX_ENABLED)
#error FRNM_CARWAKEUP_RX_ENABLED is already defined
#endif

#define FRNM_CARWAKEUP_RX_ENABLED        [!//
[!IF "count(as:modconf('FrNm')[1]/FrNmChannelConfig/*[1]/FrNmChannel/*[FrNmChannelIdentifiers/FrNmCarWakeUpRxEnabled = 'true']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/*------------------[PN Extensions]-------------------------------------*/
[!SELECT "FrNmGlobalConfig"!]
[!IF "../FrNmGeneral/FrNmPnSupported = 'true'"!]

/* !LINKSTO ECUC_FrNm_00063,1 */
#if (defined FRNM_PN_INFO_LENGTH)
#error FRNM_PN_INFO_LENGTH is already defined
#endif
#define FRNM_PN_INFO_LENGTH     [!//
              [!"FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoLength"!]U

#if (defined FRNM_PN_EIRA_TIMER_SIZE)
#error FRNM_PN_EIRA_TIMER_SIZE is already defined
#endif
#define FRNM_PN_EIRA_TIMER_SIZE  (FRNM_PN_INFO_LENGTH * 8U)

/* !LINKSTO FrNm0062_Conf_Refine,1 */
#if (defined FRNM_PN_INFO_OFFSET)
#error FRNM_PN_INFO_OFFSET is already defined
#endif
#define FRNM_PN_INFO_OFFSET     [!//
              [!"FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoOffset"!]U

/** FrNmPnEiraCalcEnabled **/
#if (defined FRNM_PN_EIRA_CALC_ENABLED)
#error FRNM_PN_EIRA_CALC_ENABLED is already defined
#endif
#define FRNM_PN_EIRA_CALC_ENABLED             [!//
[!IF "FrNmGlobalFeatures/FrNmPnEiraCalcEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** FrNmPnEraCalcEnabled **/
#if (defined FRNM_PN_ERA_CALC_ENABLED)
#error FRNM_PN_ERA_CALC_ENABLED is already defined
#endif
#define FRNM_PN_ERA_CALC_ENABLED             [!//
[!IF "count(as:modconf('FrNm')[1]/FrNmChannelConfig/*/FrNmChannel/*[FrNmChannelIdentifiers/FrNmPnEraCalcEnabled = 'true'])>0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** FrNmPnResetTime **/
#if (defined FRNM_PN_RESET_TIME)
#error FRNM_PN_RESET_TIME is already defined
#endif
#define FRNM_PN_RESET_TIME [!//
    [!"num:i(((FrNmGlobalFeatures/FrNmPnResetTime) * 1000) div ((num:min(../FrNmChannelConfig/*/FrNmChannel/*/FrNmChannelTiming/FrNmMainFunctionPeriod)) * 1000))"!]U

[!/* get Eira PduId by EcuC reference from PduR, existence is checked in xdm */!][!//
[!IF "FrNmGlobalFeatures/FrNmPnEiraCalcEnabled = 'true'"!][!//

#if (defined FRNM_PN_EIRA_PDUID)
#error FRNM_PN_EIRA_PDUID is already defined
#endif
#define FRNM_PN_EIRA_PDUID  [!//
    [!"as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/PduRSrcPdu[PduRSrcPduRef=node:current()/FrNmGlobalFeatures/FrNmPnEiraRxNSduRef
]/PduRSourcePduHandleId"!]U/* FrNmPnEiraRx Id*/
[!ENDIF!]

#if (defined FRNM_PN_SUPPORTED)
#error FRNM_PN_SUPPORTED is already defined
#endif
#define FRNM_PN_SUPPORTED  STD_ON
[!ELSE!]
#if (defined FRNM_PN_SUPPORTED)
#error FRNM_PN_SUPPORTED is already defined
#endif
#define FRNM_PN_SUPPORTED  STD_OFF

#if (defined FRNM_PN_ERA_CALC_ENABLED)
#error FRNM_PN_ERA_CALC_ENABLED is already defined
#endif
#define FRNM_PN_ERA_CALC_ENABLED STD_OFF

#if (defined FRNM_PN_EIRA_CALC_ENABLED)
#error FRNM_PN_EIRA_CALC_ENABLED is already defined
#endif
#define FRNM_PN_EIRA_CALC_ENABLED STD_OFF
[!ENDIF!]

[!ENDSELECT!][!//

[!IF "(FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnEiraCalcEnabled = 'true') or (count(as:modconf('FrNm')[1]/FrNmChannelConfig/*/FrNmChannel/*[FrNmChannelIdentifiers/FrNmPnEraCalcEnabled = 'true']) > 0)"!]
[!VAR "EiraMaxTimerSize" = "0"!][!//
[!IF "node:exists(FrNmGeneral/FrNmMaxPn)"!][!//
[!VAR "EiraMaxTimerSize" = "node:value(FrNmGeneral/FrNmMaxPn)"!][!//
[!ELSE!][!//
[!VAR "EiraMaxTimerSize" = "node:value(FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPnInfo/FrNmPnInfoLength) * 8"!][!//
[!ENDIF!][!//
#if (defined FRNM_EIRA_MAX_TIMER_SIZE)
#error FRNM_EIRA_MAX_TIMER_SIZE is already defined
#endif
#define FRNM_EIRA_MAX_TIMER_SIZE    [!"num:i($EiraMaxTimerSize)"!]U  /* FrNm Eira Maximum Timer array size */
[!ENDIF!]

/*------------------[Hsm configuration]-------------------------------------*/

#if (defined FRNM_HSM_INST_MULTI_ENABLED)
#error FRNM_HSM_INST_MULTI_ENABLED is already defined
#endif
#if (FRNM_NUMBER_OF_CHANNELS > 1U)
#define FRNM_HSM_INST_MULTI_ENABLED STD_ON
#else
#define FRNM_HSM_INST_MULTI_ENABLED STD_OFF
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define FRNM_START_SEC_CODE
#include <FrNm_MemMap.h>
/* Deviation MISRAC2012-1 */
[!FOR "I"="0" TO "num:i(FrNmGlobalConfig/FrNmGlobalConstants/FrNmNumberOfClusters)-1"!]
#ifndef RTE_SCHM_SWCBSW_FRNM_MAINFUNCTION_[!"num:i($I)"!]
#define RTE_SCHM_SWCBSW_FRNM_MAINFUNCTION_[!"num:i($I)"!]
extern FUNC(void, FRNM_CODE) FrNm_MainFunction_[!"num:i($I)"!](void);
#endif
[!ENDFOR!]
#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef FRNM_CFG_H */
/*==================[end of file]===========================================*/
