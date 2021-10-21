/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]
#ifndef CANTP_CFG_H
#define CANTP_CFG_H

/* CanTp_Cfg.h shall define constant and customizable data for module configuration
   at pre-compile time.
 */
/* !LINKSTO CanTp.ASR40.CANTP001, 1 */
/*==================[includes]===============================================*/
[!INCLUDE "../../generate_macros/CanTp_Precompile.m"!][!//
[!INCLUDE "../../generate_macros/CanTp_PostBuild.m"!][!//
#include <TSAutosar.h>      /* EB specific standard types */
#include <ComStack_Types.h> /* typedefs for AUTOSAR com stack */
[!IF "CanTpGeneral/CanTpGptUsageEnable = 'true'"!]
  [!IF "not(var:defined('postBuildVariant'))"!]
#include <CanTp_Gpt_Cbk.h>
  [!ELSE!][!//
    [!LOOP "variant:all()"!][!//
#include <CanTp_Gpt[!"."!]_Cbk.h>
    [!ENDLOOP!][!//
  [!ENDIF!]
[!ENDIF!]
/*==================[macros]=================================================*/

#if (defined CANTP_PBCFGM_SUPPORT_ENABLED) /* to prevent double declaration */
#error CANTP_PBCFGM_SUPPORT_ENABLED is already defined
#endif /* if (defined CANTP_PBCFGM_SUPPORT_ENABLED) */


#if (defined CANTP_ISVALIDCONFIG_MMAP_ENABLED) /* to prevent double declaration */
#error CANTP_ISVALIDCONFIG_MMAP_ENABLED is already defined
#endif /* if (defined CANTP_ISVALIDCONFIG_MMAP_ENABLED) */

/** \brief switch for supporting PbcgfM module */
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('CanTp')) = 'true'"!]

#define CANTP_PBCFGM_SUPPORT_ENABLED     STD_ON

/** \brief switch reports that CanTp_IsValidConfig function shall be mapped to memory section CANIF_CODE_CC_BLOCK
*/
[!IF "node:exists(as:modconf('PbcfgM')[1]/PbcfgMGeneral/PbcfgMMapIsValidFunctionToMemSection) and as:modconf('PbcfgM')[1]/PbcfgMGeneral/PbcfgMMapIsValidFunctionToMemSection = 'true'"!]
#define CANTP_ISVALIDCONFIG_MMAP_ENABLED STD_ON
[!ELSE!]
#define CANTP_ISVALIDCONFIG_MMAP_ENABLED STD_OFF
[!ENDIF!]

[!ELSE!]
#define CANTP_PBCFGM_SUPPORT_ENABLED     STD_OFF

/** \brief switch reports that CanTp_IsValidConfig function shall be mapped to memory section CANIF_CODE_CC_BLOCK
*/
#define CANTP_ISVALIDCONFIG_MMAP_ENABLED STD_OFF
[!ENDIF!]


#if (defined CANTP_PARALLEL_CHANNELS_ENABLED) /* to prevent double declaration */
#error CANTP_PARALLEL_CHANNELS_ENABLED is already defined
#endif /* if (defined CANTP_PARALLEL_CHANNELS_ENABLED) */

/** \brief switch for supporting parallel channels
*/
[!IF "node:exists(as:modconf('CanTp')[1]/CanTpGeneral/CanTpMaxParallelTxChannels) and node:exists(as:modconf('CanTp')[1]/CanTpGeneral/CanTpMaxParallelRxChannels)"!]
#define CANTP_PARALLEL_CHANNELS_ENABLED STD_ON
[!ELSE!]
#define CANTP_PARALLEL_CHANNELS_ENABLED STD_OFF
[!ENDIF!]

#if (defined CANTP_DEV_ERROR_DETECT) /* to prevent double declaration */
#error CANTP_DEV_ERROR_DETECT is already defined
#endif /* if (defined CANTP_DEV_ERROR_DETECT) */

/** \brief Switch for DET usage */

[!IF "CanTpGeneral/CanTpDevErrorDetect = 'true'"!][!//
#define CANTP_DEV_ERROR_DETECT            STD_ON
[!ELSE!][!//
#define CANTP_DEV_ERROR_DETECT            STD_OFF
[!ENDIF!][!//

#if (defined CANTP_VERSION_INFO_API) /* to prevent double declaration */
#error CANTP_VERSION_INFO_API is already defined
#endif /* if (defined CANTP_VERSION_INFO_API) */

/** \brief Switch for GetVersionInfo API */
[!IF "CanTpGeneral/CanTpVersionInfoApi = 'true'"!][!//
#define CANTP_VERSION_INFO_API            STD_ON
[!ELSE!][!//
#define CANTP_VERSION_INFO_API            STD_OFF
[!ENDIF!][!//

#if (defined CANTP_RX_CANCELLATION_API) /* to prevent double declaration */
#error CANTP_RX_CANCELLATION_API is already defined
#endif /* if (defined CANTP_RX_CANCELLATION_API) */

/** \brief Defines if receive cancellation API is available. */
[!IF "CanTpGeneral/CanTpCancelReceiveApi = 'true'"!][!//
#define CANTP_RX_CANCELLATION_API          STD_ON
[!ELSE!][!//
#define CANTP_RX_CANCELLATION_API          STD_OFF
[!ENDIF!][!//

#if (defined CANTP_TX_CANCELLATION_API) /* to prevent double declaration */
#error CANTP_TX_CANCELLATION_API is already defined
#endif /* if (defined CANTP_TX_CANCELLATION_API) */

/** \brief Defines if transmit cancellation API is available. */
[!IF "CanTpGeneral/CanTpCancelTransmitApi = 'true'"!][!//
#define CANTP_TX_CANCELLATION_API          STD_ON
[!ELSE!][!//
#define CANTP_TX_CANCELLATION_API          STD_OFF
[!ENDIF!][!//

/** \brief Handling of N_SA values
 **
 ** The macro CANTP_DYNAMIC_NSA_ENABLED is used to specify if N_SA values are fix values,
 ** which are specified at configuration time (normal Autosar behaviour) or
 ** it is possible to change the N_SA values for each
 ** Pdu at runtime. If enabled, change at runtime is possible.
 */
[!IF "CanTpGeneral/CanTpDynamicNSaEnabled = 'true'"!][!//

#if (defined CANTP_DYNAMIC_NSA_ENABLED) /* to prevent double declaration */
#error CANTP_DYNAMIC_NSA_ENABLED is already defined
#endif /* if (defined CANTP_DYNAMIC_NSA_ENABLED) */
#define CANTP_DYNAMIC_NSA_ENABLED   STD_ON

#if (defined CANTP_DYNAMIC_NSA_API) /* to prevent double declaration */
#error CANTP_DYNAMIC_NSA_API is already defined
#endif /* if (defined CANTP_DYNAMIC_NSA_API) */
#define CANTP_DYNAMIC_NSA_API       STD_ON

[!ELSE!][!//

#if (defined CANTP_DYNAMIC_NSA_ENABLED) /* to prevent double declaration */
#error CANTP_DYNAMIC_NSA_ENABLED is already defined
#endif /* if (defined CANTP_DYNAMIC_NSA_ENABLED) */
#define CANTP_DYNAMIC_NSA_ENABLED   STD_OFF

#if (defined CANTP_DYNAMIC_NSA_API) /* to prevent double declaration */
#error CANTP_DYNAMIC_NSA_API is already defined
#endif /* if (defined CANTP_DYNAMIC_NSA_API) */
#define CANTP_DYNAMIC_NSA_API       STD_OFF
[!ENDIF!][!//


#if (defined CANTP_CHANGE_PARAMETER_REQ_API) /* to prevent double declaration */
#error CANTP_CHANGE_PARAMETER_REQ_API is already defined
#endif /* if (defined CANTP_CHANGE_PARAMETER_REQ_API) */

/** \brief defines if CanTp_ChangeParameter() is enabled */
[!IF "CanTpGeneral/CanTpChangeParameterApi = 'true'"!][!//
#define CANTP_CHANGE_PARAMETER_REQ_API    STD_ON
[!ELSE!][!//
#define CANTP_CHANGE_PARAMETER_REQ_API    STD_OFF
[!ENDIF!][!//

#if (defined CANTP_CHANGE_TX_PARAMETER_REQ_API) /* to prevent double declaration */
#error CANTP_CHANGE_TX_PARAMETER_REQ_API is already defined
#endif /* if (defined CANTP_CHANGE_TX_PARAMETER_REQ_API) */

/** \brief defines if CanTp_ChangeTxParameter() and CanTp_ResetTxParameter are enabled */
[!IF "CanTpGeneral/CanTpChangeTxParameterApi = 'true'"!][!//
#define CANTP_CHANGE_TX_PARAMETER_REQ_API    STD_ON
[!ELSE!][!//
#define CANTP_CHANGE_TX_PARAMETER_REQ_API    STD_OFF
[!ENDIF!][!//


#if (defined CANTP_READ_PARAMETER_REQ_API)
#error CANTP_READ_PARAMETER_REQ_API is already defined
#endif /* #if (defined CANTP_READ_PARAMETER_REQ_API) */

/** \brief defines if CanTp_ReadParameter() is enabled */
[!IF "CanTpGeneral/CanTpReadParameterApi = 'true'"!][!//
#define CANTP_READ_PARAMETER_REQ_API     STD_ON
[!ELSE!][!//
#define CANTP_READ_PARAMETER_REQ_API    STD_OFF
[!ENDIF!][!//


#if (defined CANTP_N_BR_WAIT_SUPPORT) /* to prevent double declaration */
#error CANTP_N_BR_WAIT_SUPPORT is already defined
#endif /* if (defined CANTP_N_BR_WAIT_SUPPORT) */

/** \brief defines if N_BR WAIT support is enabled */
#define CANTP_N_BR_WAIT_SUPPORT           [!IF "CanTpGeneral/CanTpNbrWaitRepeatedSupport = 'false'"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!]

#if (defined CANTP_STALLHANDLING_SUPPORT) /* to prevent double declaration */
#error CANTP_STALLHANDLING_SUPPORT is already defined
#endif /* if (defined CANTP_STALLHANDLING_SUPPORT) */

/** \brief defines if STALLHANDLING support of received meassages is enabled */
#define CANTP_STALLHANDLING_SUPPORT           [!IF "CanTpGeneral/CanTpStallHandlingSupport = 'false'"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!]


/* jumptable related macros */

#if (defined CANTP_JUMPTABLE_OFF) /* to prevent double declaration */
#error CANTP_JUMPTABLE_OFF is already defined
#endif /* if (defined CANTP_JUMPTABLE_OFF) */

/** \brief Jumptable support is deactivated */
#define CANTP_JUMPTABLE_OFF             0U

#if (defined CANTP_JUMPTABLE_SERVER) /* to prevent double declaration */
#error CANTP_JUMPTABLE_SERVER is already defined
#endif /* if (defined CANTP_JUMPTABLE_SERVER) */

/** \brief Jumptable support is activated and module acts as server */
#define CANTP_JUMPTABLE_SERVER          1U

#if (defined CANTP_JUMPTABLE_CLIENT) /* to prevent double declaration */
#error CANTP_JUMPTABLE_CLIENT is already defined
#endif /* if (defined CANTP_JUMPTABLE_CLIENT) */

/** \brief Jumptable support is activated and module acts as client */
#define CANTP_JUMPTABLE_CLIENT          2U


#if (defined CANTP_JUMPTABLE_SUPPORT) /* to prevent double declaration */
#error CANTP_JUMPTABLE_SUPPORT is already defined
#endif /* if (defined CANTP_JUMPTABLE_SUPPORT) */

/** \brief General CanTp jumptable support */
#define CANTP_JUMPTABLE_SUPPORT           [!IF "CanTpJumpTable/CanTpJumpTableMode = 'OFF'"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!]

#if (defined CANTP_JUMPTABLE_MODE) /* to prevent double declaration */
#error CANTP_JUMPTABLE_MODE is already defined
#endif /* if (defined CANTP_JUMPTABLE_MODE) */

/** \brief CanTp jumptable mode
 **
 ** This macro defines the current jumptable mode (not supported, server,
 ** client).
 */
#define CANTP_JUMPTABLE_MODE              CANTP_JUMPTABLE_[!"CanTpJumpTable/CanTpJumpTableMode"!]


#if (defined CANTP_EXIT_JUMPTABLE_WITH_SCHM) /* to prevent double declaration */
#error CANTP_EXIT_JUMPTABLE_WITH_SCHM is already defined
#endif /* if (defined CANTP_EXIT_JUMPTABLE_WITH_SCHM) */

/** \brief CanTp jumptable support: SchM used as macro
 **
 ** This macro defines, if the SchM functions shall be accessed via function
 ** pointers in the exit jumptable (value STD_ON) or if macros are defined
 ** and used for them (value STD_OFF).
 */
[!IF "(CanTpJumpTable/CanTpJumpTableMode != 'OFF') and (CanTpJumpTable/CanTpUseSchMMacros = 'false')"!][!//
#define CANTP_EXIT_JUMPTABLE_WITH_SCHM    STD_ON
[!ELSE!][!//
#define CANTP_EXIT_JUMPTABLE_WITH_SCHM    STD_OFF
[!ENDIF!][!//


#if (defined CANTP_PROVIDE_JUMPTABLE) /* to prevent double declaration */
#error CANTP_PROVIDE_JUMPTABLE is already defined
#endif /* if (defined CANTP_PROVIDE_JUMPTABLE) */

/** \brief CanTp jumptable configuration
 **
 ** This macro defines if the current CanTp shall provide the entry jumptable
 ** to the application (value STD_ON) or if the CanTp functions are provided
 ** (value STD_OFF).
 */
[!IF "CanTpJumpTable/CanTpJumpTableMode = 'SERVER'"!][!//
#define CANTP_PROVIDE_JUMPTABLE           STD_ON
[!ELSE!][!//
#define CANTP_PROVIDE_JUMPTABLE           STD_OFF
[!ENDIF!][!//


#if (defined CANTP_PROVIDE_API_FUNCTIONS) /* to prevent double declaration */
#error CANTP_PROVIDE_API_FUNCTIONS is already defined
#endif /* if (defined CANTP_PROVIDE_API_FUNCTIONS) */

/** \brief Defines if the CanTp provides all of its API functions
 **
 ** This macro defines if the CanTp shall provide all of its API functions. If
 ** jumptable support is on and wrapper macros shall be used, this macro has
 ** the value STD_OFF. Otherwise it is STD_ON.
 */
[!IF "$CanTpProvideApiFunctions = 'true'"!][!//
#define CANTP_PROVIDE_API_FUNCTIONS       STD_ON
[!ELSE!][!//
#define CANTP_PROVIDE_API_FUNCTIONS       STD_OFF
[!ENDIF!][!//


#if (defined CANTP_USE_GPT) /* to prevent double declaration */
#error CANTP_USE_GPT is already defined
#endif /* if (defined CANTP_USE_GPT) */

/** \brief defines if general purpose timer is enabled for timeout handling of the channels */
[!IF "CanTpGeneral/CanTpGptUsageEnable = 'true'"!][!//
#define CANTP_USE_GPT  STD_ON
[!ELSE!][!//
#define CANTP_USE_GPT  STD_OFF
[!ENDIF!][!//

#if (defined CANTP_CANFD_ENABLED) /* to prevent double declaration */
#error CANTP_CANFD_ENABLED is already defined
#endif /* if (defined CANTP_CANFD_ENABLED) */

/** \brief Switch to define if CAN FD support is enabled. */
[!IF "CanTpGeneral/CanTpFlexibleDataRateSupport = 'true'"!][!//
#define CANTP_CANFD_ENABLED STD_ON
[!ELSE!][!//
#define CANTP_CANFD_ENABLED STD_OFF
[!ENDIF!][!//

#if (defined CANTP_DYNAMIC_ID_SUPPORT) /* to prevent double declaration */
#error CANTP_DYNAMIC_ID_SUPPORT is already defined
#endif /* if (defined CANTP_DYNAMIC_ID_SUPPORT) */

/** \brief Switch to define if dynamic ID via N-PDU MetaData support is enabled. */
[!IF "CanTpGeneral/CanTpDynIdSupport = 'true'"!][!//
#define CANTP_DYNAMIC_ID_SUPPORT STD_ON
[!ELSE!][!//
#define CANTP_DYNAMIC_ID_SUPPORT STD_OFF
[!ENDIF!][!//

#if (defined CANTP_GENERIC_CONNECTION_SUPPORT) /* to prevent double declaration */
#error CANTP_GENERIC_CONNECTION_SUPPORT is already defined
#endif /* if (defined CANTP_GENERIC_CONNECTION_SUPPORT) */

/** \brief Switch to define if generic connection using N-SDU MetaData support is enabled. */
[!IF "CanTpGeneral/CanTpGenericConnectionSupport = 'true'"!][!//
#define CANTP_GENERIC_CONNECTION_SUPPORT STD_ON
[!ELSE!][!//
#define CANTP_GENERIC_CONNECTION_SUPPORT STD_OFF
[!ENDIF!][!//

[!IF "node:exists(as:modconf('CanTp')[1]/CanTpGeneral/CanTpMaxParallelTxChannels) and node:exists(as:modconf('CanTp')[1]/CanTpGeneral/CanTpMaxParallelRxChannels)"!]
#if (defined CANTP_MAX_TX_CHANNELS) /* to prevent double declaration */
#error CANTP_MAX_TX_CHANNELS is already defined
#endif /* if (defined CANTP_MAX_TX_CHANNELS) */

/** \brief Maximum number of parallel TX channels supported.
*/
#define CANTP_MAX_TX_CHANNELS [!"num:i(as:modconf('CanTp')[1]/CanTpGeneral/CanTpMaxParallelTxChannels)"!]U


#if (defined CANTP_MAX_RX_CHANNELS) /* to prevent double declaration */
#error CANTP_MAX_RX_CHANNELS is already defined
#endif /* if (defined CANTP_MAX_RX_CHANNELS) */

/** \brief Maximum number of parallel RX channels supported.
 */
#define CANTP_MAX_RX_CHANNELS [!"num:i(as:modconf('CanTp')[1]/CanTpGeneral/CanTpMaxParallelRxChannels)"!]U


#if (defined CANTP_MAX_PARALLEL_CHANNELS) /* to prevent double declaration */
#error CANTP_MAX_PARALLEL_CHANNELS is already defined
#endif /* if (defined CANTP_MAX_PARALLEL_CHANNELS) */

/** \brief Maximum number of parallel Tx/Rx channels supported.
 */
#define CANTP_MAX_PARALLEL_CHANNELS [!"num:i(as:modconf('CanTp')[1]/CanTpGeneral/CanTpMaxParallelTxChannels + as:modconf('CanTp')[1]/CanTpGeneral/CanTpMaxParallelRxChannels)"!]U

[!ELSE!]

#if (defined CANTP_MAX_TX_CHANNELS) /* to prevent double declaration */
#error CANTP_MAX_TX_CHANNELS is already defined
#endif /* if (defined CANTP_MAX_TX_CHANNELS) */

/** \brief Maximum number TX channels supported.
 */
#define CANTP_MAX_TX_CHANNELS [!"num:i(as:modconf('CanTp')[1]/CanTpGeneral/CanTpMaxTxChannels)"!]U

#if (defined CANTP_MAX_RX_CHANNELS) /* to prevent double declaration */
#error CANTP_MAX_RX_CHANNELS is already defined
#endif /* if (defined CANTP_MAX_RX_CHANNELS) */

/** \brief Maximum number RX channels supported.
 */
#define CANTP_MAX_RX_CHANNELS [!"num:i(as:modconf('CanTp')[1]/CanTpGeneral/CanTpMaxRxChannels)"!]U
[!ENDIF!]


#if (defined CANTP_MAX_TX_NSDUS) /* to prevent double declaration */
#error CANTP_MAX_TX_NSDUS is already defined
#endif /* if (defined CANTP_MAX_TX_NSDUS) */

/** \brief Maximum number TX N-SDUs supported.
 */
#define CANTP_MAX_TX_NSDUS [!"num:i(as:modconf('CanTp')[1]/CanTpGeneral/CanTpMaxTxNSdus)"!]U

#if (defined CANTP_MAX_RX_NSDUS) /* to prevent double declaration */
#error CANTP_MAX_RX_NSDUS is already defined
#endif /* if (defined CANTP_MAX_RX_NSDUS) */

/** \brief Maximum number RX N-SDUs supported.
 */
#define CANTP_MAX_RX_NSDUS [!"num:i(as:modconf('CanTp')[1]/CanTpGeneral/CanTpMaxRxNSdus)"!]U

#if (defined CANTP_MAX_FC_PDUS) /* to prevent double declaration */
#error CANTP_MAX_FC_PDUS is already defined
#endif /* if (defined CANTP_MAX_FC_PDUS) */

/** \brief Maximum number flow control PDUs supported.
 */
#define CANTP_MAX_FC_PDUS [!"num:i(as:modconf('CanTp')[1]/CanTpGeneral/CanTpMaxFcPdus)"!]U

/* ----- Pre-processor switch to enable/disable relocateable postbuild config ----- */
#if(CANTP_PBCFGM_SUPPORT_ENABLED == STD_ON)

#if (defined CANTP_RELOCATABLE_CFG_ENABLE) /* to prevent double declaration */
#error CANTP_RELOCATABLE_CFG_ENABLE is already defined
#endif /* if (defined CANTP_RELOCATABLE_CFG_ENABLE) */

#define CANTP_RELOCATABLE_CFG_ENABLE  [!//
[!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!/*
*/!]

#else

#if (defined CANTP_RELOCATABLE_CFG_ENABLE) /* to prevent double declaration */
#error CANTP_RELOCATABLE_CFG_ENABLE is already defined
#endif /* if (defined CANTP_RELOCATABLE_CFG_ENABLE) */

#define CANTP_RELOCATABLE_CFG_ENABLE  [!//
[!IF "CanTpGeneral/CanTpRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!/*
*/!]
#endif


#if (defined CANTP_CFG_SIGNATURE) /* to prevent double declaration */
#error CANTP_CFG_SIGNATURE is already defined
#endif /* if (defined CANTP_CFG_SIGNATURE) */

/** \brief Compile time verification value */
#define CANTP_CFG_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('CanTp')[1]//*[not(child::*) and (node:configclass() = 'PreCompile')],as:modconf('CanTp')[1]/CanTpJumpTable/*))"!]U

#if (defined CANTP_PUBLIC_INFO_SIGNATURE) /* to prevent double declaration */
#error CANTP_PUBLIC_INFO_SIGNATURE is already defined
#endif /* if (defined CANTP_PUBLIC_INFO_SIGNATURE) */

/** \brief Compile time Published information configuration verification value */
#define CANTP_PUBLIC_INFO_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('CanTp')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('CanTp')[1]/CommonPublishedInformation/Release))"!]U




/*------------------[Defensive programming]---------------------------------*/
[!SELECT "CanTpDefensiveProgramming"!][!//

#if (defined CANTP_DEFENSIVE_PROGRAMMING_ENABLED)
#error CANTP_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define CANTP_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../CanTpGeneral/CanTpDevErrorDetect  = 'true') and (CanTpDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANTP_PRECONDITION_ASSERT_ENABLED)
#error CANTP_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define CANTP_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../CanTpGeneral/CanTpDevErrorDetect  = 'true') and (CanTpDefProgEnabled = 'true') and (CanTpPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANTP_POSTCONDITION_ASSERT_ENABLED)
#error CANTP_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define CANTP_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../CanTpGeneral/CanTpDevErrorDetect  = 'true') and (CanTpDefProgEnabled = 'true') and (CanTpPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANTP_UNREACHABLE_CODE_ASSERT_ENABLED)
#error CANTP_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define CANTP_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../CanTpGeneral/CanTpDevErrorDetect  = 'true') and (CanTpDefProgEnabled = 'true') and (CanTpUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANTP_INVARIANT_ASSERT_ENABLED)
#error CANTP_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define CANTP_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../CanTpGeneral/CanTpDevErrorDetect  = 'true') and (CanTpDefProgEnabled = 'true') and (CanTpInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANTP_STATIC_ASSERT_ENABLED)
#error CANTP_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define CANTP_STATIC_ASSERT_ENABLED           [!//
[!IF "(../CanTpGeneral/CanTpDevErrorDetect  = 'true') and (CanTpDefProgEnabled = 'true') and (CanTpStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//


/*====================[Symbolic Names]=======================================*/

/*==================[type definitions]=======================================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]=========================================*/

/*==================[external function declarations]=========================*/

/*==================[end of file]============================================*/
#endif /* ifndef CANTP_CFG_H */
[!ENDCODE!]
