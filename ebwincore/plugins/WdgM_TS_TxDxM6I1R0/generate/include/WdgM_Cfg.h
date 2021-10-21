/**
 * \file
 *
 * \brief AUTOSAR WdgM
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgM.
 *
 * \version 6.1.39
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!INCLUDE "WdgM_Cfg.m"!][!//
[!CODE!][!//
#ifndef WDGM_CFG_H
#define WDGM_CFG_H

/*==================[includes]===================================================================*/
/* !LINKSTO WDGM.EB.Design.IncludeFileStructure,1 */
#include <WdgM_Callouts.h>                                                /* WdgM's callout APIs */

/*==================[macros]=====================================================================*/

/* ***************** Symbolic name values ****************************************************** */

/* List of watchdog modes */
[!LOOP "node:order(WdgMConfigSet/*[1]/WdgMMode/*, 'WdgMModeId')"!][!//

#if (defined WdgMConf_WdgMMode_[!"@name"!])
#error WdgMConf_WdgMMode_[!"@name"!] is already defined
#endif
/** \brief Export symbolic name value for watchdog mode */
#define WdgMConf_WdgMMode_[!"@name"!] [!"WdgMModeId"!]U

#if (defined WDGM_PROVIDE_LEGACY_SYMBOLIC_NAMES)

#if (defined WdgM_[!"@name"!])
#error WdgM_[!"@name"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 * (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define WdgM_[!"@name"!] [!"WdgMModeId"!]U
#endif /* defined WDGM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//

/* List of supervised entities and its checkpoints*/
[!IF "count(WdgMGeneral/WdgMSupervisedEntity/*) > 0"!][!//
[!LOOP "node:order(WdgMGeneral/WdgMSupervisedEntity/*, 'WdgMSupervisedEntityId')"!][!//

#if (defined WdgMConf_WdgMSupervisedEntity_[!"@name"!])
#error WdgMConf_WdgMSupervisedEntity_[!"@name"!] is already defined
#endif
/** \brief Export symbolic name value for supervised entity with ID [!"WdgMSupervisedEntityId"!] */
#define WdgMConf_WdgMSupervisedEntity_[!"@name"!] [!"WdgMSupervisedEntityId"!]U
[!IF "count(WdgMCheckpoint/*) > 0"!][!//
[!IF "($ASR40ServiceAPIEnabled = 'false') and ($DefaultServiceAPI != 'AUTOSAR_40')"!][!//
/* List of checkpoints of this supervised entities*/
[!LOOP "node:order(WdgMCheckpoint/*, 'WdgMCheckpointId')"!][!//

#if (defined WdgMConf_WdgMCheckpoint_[!"@name"!])
#error WdgMConf_WdgMCheckpoint_[!"@name"!] is already defined
#endif
/* \brief Export symbolic name value for checkpoint with ID [!"WdgMCheckpointId"!] */
#define WdgMConf_WdgMCheckpoint_[!"@name"!] [!"WdgMCheckpointId"!]U
[!ENDLOOP!][!//
[!ELSE!][!//
/* Checkpoints symbolic names are defined by RTE in Rte_WdgM_Type.h when ASR40 service is used */
[!ENDIF!][!//
[!ENDIF!][!//

#if (defined WDGM_PROVIDE_LEGACY_SYMBOLIC_NAMES)

#if (defined WdgM_[!"@name"!])
#error WdgM_[!"@name"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 * (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define WdgM_[!"@name"!] [!"WdgMSupervisedEntityId"!]U
[!IF "count(WdgMCheckpoint/*) > 0"!][!//
/* List of checkpoints of this supervised entities*/
[!LOOP "node:order(WdgMCheckpoint/*, 'WdgMCheckpointId')"!][!//

#if (defined WdgM_[!"@name"!])
#error WdgM_[!"@name"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 * (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define WdgM_[!"@name"!] [!"WdgMCheckpointId"!]U
[!ENDLOOP!][!//
[!ENDIF!][!//
#endif /* defined WDGM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//
[!ENDIF!][!//

/* List of watchdog devices */
[!/* Note: WdgMWatchdogName is derived from the Container name, therefore @name can be used */!][!//
[!LOOP "WdgMGeneral/WdgMWatchdog/*"!][!//

#if (defined WdgMConf_WdgMWatchdog_[!"WdgMWatchdogName"!])
#error WdgMConf_WdgMWatchdog_[!"WdgMWatchdogName"!] is already defined
#endif
/** \brief Export symbolic name value for watchdog mode */
#define WdgMConf_WdgMWatchdog_[!"WdgMWatchdogName"!] [!"as:ref(WdgMWatchdogDeviceRef)/WdgIfDeviceIndex"!]U

#if (defined WDGM_PROVIDE_LEGACY_SYMBOLIC_NAMES)

#if (defined WdgM_[!"WdgMWatchdogName"!])
#error WdgM_[!"WdgMWatchdogName"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 * (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define WdgM_[!"WdgMWatchdogName"!] [!"as:ref(WdgMWatchdogDeviceRef)/WdgIfDeviceIndex"!]U
#endif /* defined WDGM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//

/* !LINKSTO WDGM.ASR40.WDGM104,1 */
/* ***************** ASR specific macros ******************************************************* */

#if (defined WDGM_DEV_ERROR_DETECT)
#error WDGM_DEV_ERROR_DETECT already defined
#endif
/** \brief Macro for DET usage */
#define WDGM_DEV_ERROR_DETECT [!//
[!IF "$DevErrorDetectEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_VERSION_INFO_API)
#error WDGM_VERSION_INFO_API already defined
#endif
/** \brief Macro for DET usage */
#define WDGM_VERSION_INFO_API [!//
[!IF "$VersionInfoApiEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_DEM_ALIVE_SUPERVISION_REPORT)
#error WDGM_DEM_ALIVE_SUPERVISION_REPORT already defined
#endif
/** \brief reporting to diagnostic event manager **/
#define WDGM_DEM_ALIVE_SUPERVISION_REPORT [!//
[!IF "$DemStoppedSupervisionReportEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_IMMEDIATE_RESET)
#error WDGM_IMMEDIATE_RESET already defined
#endif
/** \brief immediate MCU reset in case of alive-supervision failure */
#define WDGM_IMMEDIATE_RESET [!//
[!IF "$ImmediateResetEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_OFF_MODE_ENABLED)
#error WDGM_OFF_MODE_ENABLED already defined
#endif
/** \brief "off mode" enabled/disabled */
#define WDGM_OFF_MODE_ENABLED [!//
[!IF "$OffModeEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_DEFENSIVE_BEHAVIOR)
#error WDGM_DEFENSIVE_BEHAVIOR already defined
#endif
/** \brief availability of Rte */
#define WDGM_DEFENSIVE_BEHAVIOR [!//
[!IF "$DefensiveBehaviorEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* ***************** General EB specific macros ************************************************ */

#if (defined WDGM_EB_WDG_INST_NUM)
#error WDGM_EB_WDG_INST_NUM already defined
#endif
/** \brief Number of watchdog drivers */
#define WDGM_EB_WDG_INST_NUM [!"$NumberOfWdgDrivers"!]U

#if (defined WDGM_EB_CALLERIDS_NUM)
#error WDGM_EB_CALLERIDS_NUM already defined
#endif
/** \brief Number of caller IDs */
#define WDGM_EB_CALLERIDS_NUM [!"$NumberOfCallerIds"!]U

#if (defined WDGM_EB_MODE_NUM)
#error WDGM_EB_MODE_NUM already defined
#endif
/** \brief Number of watchdog modes */
#define WDGM_EB_MODE_NUM [!"$NumberOfWdgModes"!]U

#if (defined WDGM_EB_INTERNAL_GRAPH_NUM)
#error WDGM_EB_INTERNAL_GRAPH_NUM already defined
#endif

/** \brief Number of graphs is sum of configured internal graphs */
#define WDGM_EB_INTERNAL_GRAPH_NUM [!"$NumberOfInternalGraphs"!]U

/** \brief Number of graphs is sum of configured external graphs */
#if (defined WDGM_EB_EXTERNAL_GRAPH_NUM)
#error WDGM_EB_EXTERNAL_GRAPH_NUM already defined
#endif

/** \brief Number of graphs is sum of configured external graphs */
#define WDGM_EB_EXTERNAL_GRAPH_NUM [!"$NumberOfExternalGraphs"!]U

#if (defined WDGM_EB_DM_NUM)
#error WDGM_EB_DM_NUM already defined
#endif
/** \brief Number of deadline supervision configurations */
#define WDGM_EB_DM_NUM [!"$NumberOfDMs"!]U

#if (defined WDGM_CONFIG_NAME)
#error WDGM_CONFIG_NAME already defined
#endif
/** \brief Name of the data structure storing configuration data
 * A pointer to this structure must be given to WdgM_Init(). */
#define WDGM_CONFIG_NAME [!"name(WdgMConfigSet/*[1])"!]

#if (defined WDGM_EB_SUPERVISION_DEM_EVENT_ID)
#error WDGM_EB_SUPERVISION_DEM_EVENT_ID already defined
#endif
/** \brief Supervision failure Dem event Id **/
#define WDGM_EB_SUPERVISION_DEM_EVENT_ID [!//
[!IF "node:exists(WdgMConfigSet/*[1]/WdgMDemEventParameterRefs/WDGM_E_MONITORING)"!][!//
[!"concat('DemConf_DemEventParameter_',node:ref(WdgMConfigSet/*[1]/WdgMDemEventParameterRefs/WDGM_E_MONITORING)/@name)"!][!//
[!ENDIF!]

#if (defined WDGM_EB_SET_MODE_DEM_EVENT_ID)
#error WDGM_EB_SET_MODE_DEM_EVENT_ID already defined
#endif
/** \brief Mode switch failure Dem event Id **/
#define WDGM_EB_SET_MODE_DEM_EVENT_ID [!//
[!IF "node:exists(WdgMConfigSet/*[1]/WdgMDemEventParameterRefs/WDGM_E_SET_MODE)"!][!//
[!"concat('DemConf_DemEventParameter_',node:ref(WdgMConfigSet/*[1]/WdgMDemEventParameterRefs/WDGM_E_SET_MODE)/@name)"!][!//
[!ENDIF!]

#if (defined WDGM_EB_IMPROPER_CALLER_DEM_EVENT_ID)
#error WDGM_EB_IMPROPER_CALLER_DEM_EVENT_ID already defined
#endif
/** \brief Improper caller Dem event Id **/
#define WDGM_EB_IMPROPER_CALLER_DEM_EVENT_ID [!//
[!IF "node:exists(WdgMConfigSet/*[1]/WdgMDemEventParameterRefs/WDGM_E_IMPROPER_CALLER)"!][!//
[!"concat('DemConf_DemEventParameter_',node:ref(WdgMConfigSet/*[1]/WdgMDemEventParameterRefs/WDGM_E_IMPROPER_CALLER)/@name)"!][!//
[!ENDIF!]

#if (defined WDGM_EB_MF_TIMINGVIOLATION_DEM_EVENT_ID)
#error WDGM_EB_MF_TIMINGVIOLATION_DEM_EVENT_ID already defined
#endif
/** \brief Mainfunction timing violation Dem event Id **/
#define WDGM_EB_MF_TIMINGVIOLATION_DEM_EVENT_ID [!//
[!IF "node:exists(WdgMConfigSet/*[1]/WdgMDemEventParameterRefs/WDGM_E_MF_TIMINGVIOLATION)"!][!//
[!"concat('DemConf_DemEventParameter_',node:ref(WdgMConfigSet/*[1]/WdgMDemEventParameterRefs/WDGM_E_MF_TIMINGVIOLATION)/@name)"!][!//
[!ENDIF!]

#if (defined WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID)
#error WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID already defined
#endif
/** \brief Data corruption Dem event Id **/
#define WDGM_EB_DATA_CORRUPTION_DEM_EVENT_ID [!//
[!IF "node:exists(WdgMConfigSet/*[1]/WdgMDemEventParameterRefs/WDGM_E_DATA_CORRUPTION)"!][!//
[!"concat('DemConf_DemEventParameter_',node:ref(WdgMConfigSet/*[1]/WdgMDemEventParameterRefs/WDGM_E_DATA_CORRUPTION)/@name)"!][!//
[!ENDIF!]
#if (defined WDGM_EB_ENABLE_SETMODE_SYNCHRON)
#error WDGM_EB_ENABLE_SETMODE_SYNCHRON already defined
#endif
/** \brief This macro enable WdgM_SetMode synchronously switch to the new mode. **/
#define WDGM_EB_ENABLE_SETMODE_SYNCHRON [!//
[!IF "$WdgMSetModeSynchronEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_PROD_ERR_REP_TO_DEM)
#error WDGM_PROD_ERR_REP_TO_DEM already defined
#endif

/** \brief definition of production error reporting defines - report to Dem
 *
 * The production error reporting could be configured to Dem, Det and off.
 * WDGM_PROD_ERR_REP_TO_DEM is used if the production error is reported
 * towards the Dem.
 */
#define WDGM_PROD_ERR_REP_TO_DEM 0U

#if (defined WDGM_PROD_ERR_REP_TO_DET)
#error WDGM_PROD_ERR_REP_TO_DET already defined
#endif

/** \brief definition of production error reporting defines - report to Det
 *
 * The production error reporting could be configured to Dem, Det and off.
 * WDGM_PROD_ERR_REP_TO_DET is used if the production error is reported
 * towards the Det.
 */
#define WDGM_PROD_ERR_REP_TO_DET 1U

#if (defined WDGM_PROD_ERR_DISABLE)
#error WDGM_PROD_ERR_DISABLE already defined
#endif

/** \brief definition of production error reporting defines - off
 *
 * The production error reporting could be configured to Dem, Det and off.
 * WDGM_PROD_ERR_DISABLE is used if the production error reporting is
 * switched off.
 */
#define WDGM_PROD_ERR_DISABLE 2U

/* ***************** EB specific macros regarding enhancements ********************************* */




/*------------------[Defensive programming]---------------------------------*/
[!SELECT "WdgMDefensiveProgramming"!][!//

#if (defined WDGM_DEFENSIVE_PROGRAMMING_ENABLED)
#error WDGM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define WDGM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../WdgMGeneral/WdgMDevErrorDetect  = 'true') and (WdgMDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_PRECONDITION_ASSERT_ENABLED)
#error WDGM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define WDGM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../WdgMGeneral/WdgMDevErrorDetect  = 'true') and (WdgMDefProgEnabled = 'true') and (WdgMPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_POSTCONDITION_ASSERT_ENABLED)
#error WDGM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define WDGM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../WdgMGeneral/WdgMDevErrorDetect  = 'true') and (WdgMDefProgEnabled = 'true') and (WdgMPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error WDGM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define WDGM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../WdgMGeneral/WdgMDevErrorDetect  = 'true') and (WdgMDefProgEnabled = 'true') and (WdgMUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_INVARIANT_ASSERT_ENABLED)
#error WDGM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define WDGM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../WdgMGeneral/WdgMDevErrorDetect  = 'true') and (WdgMDefProgEnabled = 'true') and (WdgMInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_STATIC_ASSERT_ENABLED)
#error WDGM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define WDGM_STATIC_ASSERT_ENABLED           [!//
[!IF "(../WdgMGeneral/WdgMDevErrorDetect  = 'true') and (WdgMDefProgEnabled = 'true') and (WdgMStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//


/*------------------[ASR compatibility flags]----------------------------------------------------*/

#if (defined WDGM_EB_BSW_COMPATIBILITY_ASR32)
#error WDGM_EB_BSW_COMPATIBILITY_ASR32 already defined
#endif
/** \brief Definition that the syntax of the WdgM BSW APIs are AUTOSAR 3.2 compliant **/
#define WDGM_EB_BSW_COMPATIBILITY_ASR32 32U

#if (defined WDGM_EB_BSW_COMPATIBILITY_ASR40)
#error WDGM_EB_BSW_COMPATIBILITY_ASR40 already defined
#endif
/** \brief Definition that the syntax of the WdgM BSW APIs are AUTOSAR 4.0 compliant **/
#define WDGM_EB_BSW_COMPATIBILITY_ASR40 40U

#if (defined WDGM_EB_BSW_COMPATIBILITY_ASR43)
#error WDGM_EB_BSW_COMPATIBILITY_ASR43 already defined
#endif
/** \brief Definition that the syntax of the WdgM BSW APIs are AUTOSAR 4.3 compliant **/
#define WDGM_EB_BSW_COMPATIBILITY_ASR43 43U

#if (defined WDGM_EB_BSW_COMPATIBILITY)
#error WDGM_EB_BSW_COMPATIBILITY already defined
#endif
/** \brief Definition of WdgM BSW API Compatibility mode **/
#define WDGM_EB_BSW_COMPATIBILITY                          [!//
[!IF "$BswCompatibilityMode = 'AUTOSAR_32'"!]WDGM_EB_BSW_COMPATIBILITY_ASR32
[!ELSEIF "$BswCompatibilityMode = 'AUTOSAR_40'"!]WDGM_EB_BSW_COMPATIBILITY_ASR40
[!ELSE!]WDGM_EB_BSW_COMPATIBILITY_ASR43
[!ENDIF!][!//

#if (defined WDGM_EB_SERVICE_API_NONE)
#error WDGM_EB_SERVICE_API_NONE already defined
#endif
/** \brief Definition that no default service API is set **/
#define WDGM_EB_SERVICE_API_NONE   0U

#if (defined WDGM_EB_SERVICE_API_ASR32)
#error WDGM_EB_SERVICE_API_ASR32 already defined
#endif
/** \brief Definition that AUTOSAR 3.2 service API is set as default **/
#define WDGM_EB_SERVICE_API_ASR32 32U

#if (defined WDGM_EB_SERVICE_API_ASR40)
#error WDGM_EB_SERVICE_API_ASR40 already defined
#endif
/** \brief Definition that AUTOSAR 4.0 service API is set as default **/
#define WDGM_EB_SERVICE_API_ASR40 40U

#if (defined WDGM_EB_SERVICE_API_ASR43)
#error WDGM_EB_SERVICE_API_ASR43 already defined
#endif
/** \brief Definition that AUTOSAR 4.3 service API is set as default **/
#define WDGM_EB_SERVICE_API_ASR43 43U

#if (defined WDGM_EB_ENABLE_ASR32_SERVICE_API)
#error WDGM_EB_ENABLE_ASR32_SERVICE_API already defined
#endif
/** \brief Definition whether AUTOSAR 3.2 service API for WdgM is enabled **/
#define WDGM_EB_ENABLE_ASR32_SERVICE_API                   [!//
[!IF "$ASR32ServiceAPIEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_ENABLE_ASR40_SERVICE_API)
#error WDGM_EB_ENABLE_ASR40_SERVICE_API already defined
#endif
/** \brief Definition whether AUTOSAR 4.0 service API for WdgM is enabled **/
#define WDGM_EB_ENABLE_ASR40_SERVICE_API                   [!//
[!IF "$ASR40ServiceAPIEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_ENABLE_ASR43_SERVICE_API)
#error WDGM_EB_ENABLE_ASR43_SERVICE_API already defined
#endif
/** \brief Definition whether AUTOSAR 4.3 service API for WdgM is enabled **/
#define WDGM_EB_ENABLE_ASR43_SERVICE_API                   [!//
[!IF "$ASR43ServiceAPIEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_DEFAULT_ASR_SERVICE_API)
#error WDGM_EB_DEFAULT_ASR_SERVICE_API already defined
#endif
/** \brief Definition of default service API **/
#define WDGM_EB_DEFAULT_ASR_SERVICE_API                    [!//
[!IF "$DefaultServiceAPI = 'AUTOSAR_32'"!]WDGM_EB_SERVICE_API_ASR32
[!ELSEIF "$DefaultServiceAPI = 'AUTOSAR_40'"!]WDGM_EB_SERVICE_API_ASR40
[!ELSEIF "$DefaultServiceAPI = 'AUTOSAR_43'"!]WDGM_EB_SERVICE_API_ASR43
[!ELSE!]WDGM_EB_SERVICE_API_NONE
[!ENDIF!][!//

#if (defined WDGM_EB_ENABLE_ASR32_ACTALIVESUPERV_API)
#error WDGM_EB_ENABLE_ASR32_ACTALIVESUPERV_API already defined
#endif
/** \brief Definition whether AUTOSAR 3.2 service API ActivateAliveSupervision is provided **/
#define WDGM_EB_ENABLE_ASR32_ACTALIVESUPERV_API            [!//
[!IF "$ActivateAliveSupervisionAPIEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_ASR32_ACTALIVESUPERV_API)
#error WDGM_EB_ASR32_ACTALIVESUPERV_API already defined
#endif
/** \brief Definition of callout API for AUTOSAR 3.2 service API ActivateAliveSupervision **/
#define WDGM_EB_ASR32_ACTALIVESUPERV_API                   [!"$ActivateAliveSupervisionAPIName"!]

#if (defined WDGM_EB_ENABLE_ASR32_DEACTALIVESUPERV_API)
#error WDGM_EB_ENABLE_ASR32_DEACTALIVESUPERV_API already defined
#endif
/** \brief Definition whether AUTOSAR 3.2 service API DeactivateAliveSupervision is provided **/
#define WDGM_EB_ENABLE_ASR32_DEACTALIVESUPERV_API          [!//
[!IF "$DeactivateAliveSupervisionAPIEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_ASR32_DEACTALIVESUPERV_API)
#error WDGM_EB_ASR32_DEACTALIVESUPERV_API already defined
#endif
/** \brief Definition of callout API for AUTOSAR 3.2 service API ActivateAliveSupervision **/
#define WDGM_EB_ASR32_DEACTALIVESUPERV_API                 [!"$DeactivateAliveSupervisionAPIName"!]

/*------------------[Error reporting flags]------------------------------------------------------*/

#if (defined WDGM_EB_PROD_ERR_HANDLING_SUPERVISION)
#error WDGM_EB_PROD_ERR_HANDLING_SUPERVISION already defined
#endif
/** \brief Switch for DEM to DET reporting */
#define WDGM_EB_PROD_ERR_HANDLING_SUPERVISION [!//
[!IF "node:exists(ReportToDem/WdgMSupervisionReportToDem) and (ReportToDem/WdgMSupervisionReportToDem = 'DEM')"!][!//
WDGM_PROD_ERR_REP_TO_DEM
[!ELSEIF "node:exists(ReportToDem/WdgMSupervisionReportToDem) and (ReportToDem/WdgMSupervisionReportToDem = 'DET')"!][!//
WDGM_PROD_ERR_REP_TO_DET
[!ELSE!][!//
WDGM_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "node:exists(ReportToDem/WdgMSupervisionReportToDem) and (ReportToDem/WdgMSupervisionReportToDem = 'DET')"!][!//
#if (defined WDGM_EB_E_DEMTODET_SUPERVISION)
#error WDGM_EB_E_DEMTODET_SUPERVISION already defined
#endif
/* !LINKSTO WdgM.EB.ReportToDem.Supervision.5,1 */
/** \brief Det error ID, if DEM to DET reporting is enabled */
#define WDGM_EB_E_DEMTODET_SUPERVISION [!"ReportToDem/WdgMSupervisionDemDetErrId"!]U
[!ENDIF!][!//

#if (defined WDGM_EB_PROD_ERR_HANDLING_SET_MODE)
#error WDGM_EB_PROD_ERR_HANDLING_SET_MODE already defined
#endif
/** \brief Switch for DEM to DET reporting */
#define WDGM_EB_PROD_ERR_HANDLING_SET_MODE                 [!//
[!IF "node:exists(ReportToDem/WdgMSetModeReportToDem) and (ReportToDem/WdgMSetModeReportToDem = 'DEM')"!][!//
WDGM_PROD_ERR_REP_TO_DEM
[!ELSEIF "node:exists(ReportToDem/WdgMSetModeReportToDem) and (ReportToDem/WdgMSetModeReportToDem = 'DET')"!][!//
WDGM_PROD_ERR_REP_TO_DET
[!ELSE!][!//
WDGM_PROD_ERR_DISABLE
[!ENDIF!][!//
[!IF "node:exists(ReportToDem/WdgMSetModeReportToDem) and (ReportToDem/WdgMSetModeReportToDem = 'DET')"!][!//
#if (defined WDGM_EB_E_DEMTODET_SET_MODE)
#error WDGM_EB_E_DEMTODET_SET_MODE already defined
#endif
/* !LINKSTO WdgM.EB.ReportToDem.SetMode.5,1 */
/** \brief Det error ID, if DEM to DET reporting is enabled */
#define WDGM_EB_E_DEMTODET_SET_MODE [!"ReportToDem/WdgMSetModeDemDetErrId"!]U
[!ENDIF!][!//

#if (defined WDGM_EB_PROD_ERR_HANDLING_IMPROPER_CALLER)
#error WDGM_EB_PROD_ERR_HANDLING_IMPROPER_CALLER already defined
#endif
/** \brief Switch for DEM to DET reporting */
#define WDGM_EB_PROD_ERR_HANDLING_IMPROPER_CALLER          [!//
[!IF "node:exists(ReportToDem/WdgMImproperCallerReportToDem) and (ReportToDem/WdgMImproperCallerReportToDem = 'DEM')"!][!//
WDGM_PROD_ERR_REP_TO_DEM
[!ELSEIF "node:exists(ReportToDem/WdgMImproperCallerReportToDem) and (ReportToDem/WdgMImproperCallerReportToDem = 'DET')"!][!//
WDGM_PROD_ERR_REP_TO_DET
[!ELSE!][!//
WDGM_PROD_ERR_DISABLE
[!ENDIF!][!//
[!IF "node:exists(ReportToDem/WdgMImproperCallerReportToDem) and (ReportToDem/WdgMImproperCallerReportToDem = 'DET')"!][!//
#if (defined WDGM_EB_E_DEMTODET_IMPROPER_CALLER)
#error WDGM_EB_E_DEMTODET_IMPROPER_CALLER already defined
#endif
/* !LINKSTO WdgM.EB.ReportToDem.ImproperCaller.5,1 */
/** \brief Det error ID, if DEM to DET reporting is enabled */
#define WDGM_EB_E_DEMTODET_IMPROPER_CALLER [!"ReportToDem/WdgMImproperCallerDemDetErrId"!]U
[!ENDIF!][!//

#if (defined WDGM_EB_PROD_ERR_HANDLING_MF_TIMINGVIOLATION)
#error WDGM_EB_PROD_ERR_HANDLING_MF_TIMINGVIOLATION already defined
#endif
/** \brief Switch for DEM to DET reporting */
#define WDGM_EB_PROD_ERR_HANDLING_MF_TIMINGVIOLATION       [!//
[!IF "node:exists(ReportToDem/WdgMMFTimingViolationReportToDem) and (ReportToDem/WdgMMFTimingViolationReportToDem = 'DEM')"!][!//
WDGM_PROD_ERR_REP_TO_DEM
[!ELSEIF "node:exists(ReportToDem/WdgMMFTimingViolationReportToDem) and (ReportToDem/WdgMMFTimingViolationReportToDem = 'DET')"!][!//
WDGM_PROD_ERR_REP_TO_DET
[!ELSE!][!//
WDGM_PROD_ERR_DISABLE
[!ENDIF!][!//
[!IF "node:exists(ReportToDem/WdgMMFTimingViolationReportToDem) and (ReportToDem/WdgMMFTimingViolationReportToDem = 'DET')"!][!//
#if (defined WDGM_EB_E_DEMTODET_MF_TIMINGVIOLATION)
#error WDGM_EB_E_DEMTODET_MF_TIMINGVIOLATION already defined
#endif
/* !LINKSTO WdgM.EB.ReportToDem.MfTimingViolation.5,1 */
/** \brief Det error ID, if DEM to DET reporting is enabled */
#define WDGM_EB_E_DEMTODET_MF_TIMINGVIOLATION [!"ReportToDem/WdgMMFTimingViolationDemDetErrId"!]U
[!ENDIF!][!//

#if (defined WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION)
#error WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION already defined
#endif
/** \brief Switch for DEM to DET reporting */
#define WDGM_EB_PROD_ERR_HANDLING_DATA_CORRUPTION          [!//
[!IF "node:exists(ReportToDem/WdgMDataCorruptionReportToDem) and (ReportToDem/WdgMDataCorruptionReportToDem = 'DEM')"!][!//
WDGM_PROD_ERR_REP_TO_DEM
[!ELSEIF "node:exists(ReportToDem/WdgMDataCorruptionReportToDem) and (ReportToDem/WdgMDataCorruptionReportToDem = 'DET')"!][!//
WDGM_PROD_ERR_REP_TO_DET
[!ELSE!][!//
WDGM_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "node:exists(ReportToDem/WdgMDataCorruptionReportToDem) and (ReportToDem/WdgMDataCorruptionReportToDem = 'DET')"!][!//
#if (defined WDGM_EB_E_DEMTODET_DATA_CORRUPTION)
#error WDGM_EB_E_DEMTODET_DATA_CORRUPTION already defined
#endif
/* !LINKSTO WdgM.EB.ReportToDem.DataCorruption.5,1 */
/** \brief Det error ID, if DEM to DET reporting is enabled */
#define WDGM_EB_E_DEMTODET_DATA_CORRUPTION [!"ReportToDem/WdgMDataCorruptionDemDetErrId"!]U
[!ENDIF!][!//

/*------------------[TimEPM specific flags]------------------------------------------------------*/

#if (defined WDGM_EB_ENABLE_SUPERVISOR_CALLOUTS)
#error WDGM_EB_ENABLE_SUPERVISOR_CALLOUTS already defined
#endif
/** \brief Definition whether WdgM shall poll information from the Supervisor. **/
#define WDGM_EB_ENABLE_SUPERVISOR_CALLOUTS                 [!//
[!IF "$SupervisorCalloutsEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED)
#error WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED already defined
#endif
/** \brief Defines whether an extern callout API shall be called. */
#define WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED [!//
[!IF "$GetExpectedInitStateCalloutEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_EXT_GET_EXPECTED_INIT_STATE)
#error WDGM_EB_EXT_GET_EXPECTED_INIT_STATE already defined
#endif
/** \brief Name of the external callout API for getting the expected (de-)init state */
#define WDGM_EB_EXT_GET_EXPECTED_INIT_STATE [!"$GetExpectedInitStateCalloutName"!]

#if (defined WDGM_EB_EXT_INIT_REDIRCALLOUT_ENABLED)
#error WDGM_EB_EXT_INIT_REDIRCALLOUT_ENABLED already defined
#endif
/** \brief Defines whether a call to WdgM_Init shall be redirected to a different API. */
#define WDGM_EB_EXT_INIT_REDIRCALLOUT_ENABLED [!//
[!IF "$InitRedirectionCalloutAPIEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_EXT_INIT_REDIRCALLOUT)
#error WDGM_EB_EXT_INIT_REDIRCALLOUT already defined
#endif
/** \brief Name of the API used for redirecting a call to WdgM_Init */
#define WDGM_EB_EXT_INIT_REDIRCALLOUT [!"$InitRedirectionCalloutAPIName"!]

#if (defined WDGM_EB_EXT_DEINIT_REDIRCALLOUT_ENABLED)
#error WDGM_EB_EXT_DEINIT_REDIRCALLOUT_ENABLED already defined
#endif
/** \brief Defines whether a call to WdgM_DeInit shall be redirected to a different API. */
#define WDGM_EB_EXT_DEINIT_REDIRCALLOUT_ENABLED [!//
[!IF "$DeInitRedirectionCalloutAPIEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_EXT_DEINIT_REDIRCALLOUT)
#error WDGM_EB_EXT_DEINIT_REDIRCALLOUT already defined
#endif
/** \brief Name of the API used for redirecting a call to WdgM_DeInit */
#define WDGM_EB_EXT_DEINIT_REDIRCALLOUT [!"$DeInitRedirectionCalloutAPIName"!]

#if (defined WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED)
#error WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED already defined
#endif
/** \brief Defines whether an extern callout API shall be called. */
#define WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED [!//
[!IF "$GetExpectedWdgMModeCalloutEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE)
#error WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE already defined
#endif
/** \brief Name of the external callout API for getting the expected WdgM mode */
#define WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE [!"$GetExpectedWdgMModeCalloutName"!]

#if (defined WDGM_EB_EXT_SETMODE_REDIRCALLOUT_ENABLED)
#error WDGM_EB_EXT_SETMODE_REDIRCALLOUT_ENABLED already defined
#endif
/** \brief Defines whether a call to WdgM_SetMode shall be redirected to a different API. */
#define WDGM_EB_EXT_SETMODE_REDIRCALLOUT_ENABLED [!//
[!IF "$SetModeRedirectionCalloutAPIEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_EXT_SETMODE_REDIRCALLOUT)
#error WDGM_EB_EXT_SETMODE_REDIRCALLOUT already defined
#endif
/** \brief Name of the API used for redirecting a call to WdgM_SetMode */
#define WDGM_EB_EXT_SETMODE_REDIRCALLOUT [!"$SetModeRedirectionCalloutAPIName"!]

#if (defined WDGM_EB_EXT_GET_TIME_ENABLED)
#error WDGM_EB_EXT_GET_TIME_ENABLED already defined
#endif
/** \brief Defines whether an extern callout API shall be called. */
#define WDGM_EB_EXT_GET_TIME_ENABLED [!//
[!IF "$GetTimeCalloutEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_EXT_GET_TIME)
#error WDGM_EB_EXT_GET_TIME already defined
#endif
/** \brief Name of the external callout API for retrieving the actual timer ticks */
#define WDGM_EB_EXT_GET_TIME [!"$GetTimeCalloutName"!]

#if (defined WDGM_EB_EXT_IS_PERFORM_RESET_ENABLED)
#error WDGM_EB_EXT_IS_PERFORM_RESET_ENABLED already defined
#endif
/** \brief Defines whether an extern callout API shall be called. */
#define WDGM_EB_EXT_IS_PERFORM_RESET_ENABLED [!//
[!IF "$IsPerformResetCalloutEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_EXT_IS_PERFORM_RESET)
#error WDGM_EB_EXT_IS_PERFORM_RESET already defined
#endif
/** \brief Name of the external callout API for retrieving the actual timer ticks */
#define WDGM_EB_EXT_IS_PERFORM_RESET [!"$IsPerformResetCalloutName"!]

#if (defined WDGM_EB_EXT_SUPERVISION_EXPIRED_ENABLED)
#error WDGM_EB_EXT_SUPERVISION_EXPIRED_ENABLED already defined
#endif
/** \brief Defines whether an extern callout API shall be called. */
#define WDGM_EB_EXT_SUPERVISION_EXPIRED_ENABLED [!//
[!IF "$SupervisionExpiredCalloutEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_EXT_SUPERVISION_EXPIRED)
#error WDGM_EB_EXT_SUPERVISION_EXPIRED already defined
#endif
/** \brief Name of the external callout API for retrieving the actual timer ticks */
#define WDGM_EB_EXT_SUPERVISION_EXPIRED [!"$SupervisionExpiredCalloutName"!]

#if (defined WDGM_EB_EXT_INDIVIDUAL_MODE_SWITCH_ENABLED)
#error WDGM_EB_EXT_INDIVIDUAL_MODE_SWITCH_ENABLED already defined
#endif
/** \brief Defines whether an extern callout API shall be called. */
#define WDGM_EB_EXT_INDIVIDUAL_MODE_SWITCH_ENABLED [!//
[!IF "$IndividualModeSwitchCalloutEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_EXT_INDIVIDUAL_MODE_SWITCH)
#error WDGM_EB_EXT_INDIVIDUAL_MODE_SWITCH already defined
#endif
/** \brief Name of the external callout API for retrieving the actual timer ticks */
#define WDGM_EB_EXT_INDIVIDUAL_MODE_SWITCH [!"$IndividualModeSwitchCalloutName"!]

#if (defined WDGM_EB_EXT_GLOBAL_MODE_SWITCH_ENABLED)
#error WDGM_EB_EXT_GLOBAL_MODE_SWITCH_ENABLED already defined
#endif
/** \brief Defines whether an extern callout API shall be called. */
#define WDGM_EB_EXT_GLOBAL_MODE_SWITCH_ENABLED [!//
[!IF "$GlobalModeSwitchCalloutEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_EXT_GLOBAL_MODE_SWITCH)
#error WDGM_EB_EXT_GLOBAL_MODE_SWITCH already defined
#endif
/** \brief Name of the external callout API for retrieving the actual timer ticks */
#define WDGM_EB_EXT_GLOBAL_MODE_SWITCH [!"$GlobalModeSwitchCalloutName"!]

#if (defined WDGM_EB_EXT_DET_ENABLED)
#error WDGM_EB_EXT_DET_ENABLED already defined
#endif
/** \brief Defines whether an extern callout API shall be called. */
#define WDGM_EB_EXT_DET_ENABLED [!//
[!IF "$DetCalloutEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_EXT_DET)
#error WDGM_EB_EXT_DET already defined
#endif
/** \brief Name of the external callout API for retrieving the actual timer ticks */
#define WDGM_EB_EXT_DET [!"$DetCalloutName"!]

#if (defined WDGM_EXT_GET_CORE_ID_ENABLED)
#error WDGM_EXT_GET_CORE_ID_ENABLED already defined
#endif
/** \brief Defines whether an extern callout API shall be called. */
#define WDGM_EXT_GET_CORE_ID_ENABLED [!//
[!IF "$GetCoreIdCalloutEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EXT_GET_CORE_ID)
#error WDGM_EXT_GET_CORE_ID already defined
#endif
/** \brief Name of the external callout API for retrieving the core Id */
#define WDGM_EXT_GET_CORE_ID() [!"$GetCoreIdCalloutName"!]

#if (defined WDGM_EB_PARTITIONING_ENABLED)
#error WDGM_EB_PARTITIONING_ENABLED already defined
#endif
/** \brief Definition whether memory partitioning is used or not **/
#define WDGM_EB_PARTITIONING_ENABLED [!//
[!IF "$WdgMPartitioningEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_DM_TIMEGRANULARITY)
#error WDGM_EB_DM_TIMEGRANULARITY already defined
#endif
/** \brief Real-time granularity in micro seconds for external GetElapsedTime API.
 * Equals 0 if Deadline Monitoring is not used. */
#define WDGM_EB_DM_TIMEGRANULARITY [!"num:i(round(num:f(num:mul($TimeGranularity, 1000000))))"!]UL

#if (defined WDGM_EB_DM_MAINFUNCTIONCYCLETIME)
#error WDGM_EB_DM_MAINFUNCTIONCYCLETIME already defined
#endif
/** \brief Time units between two Main Function calls in granularity stated in
 * macro WDGM_EB_DM_TIMEGRANULARITY.
 * Equals 0 if Deadline Monitoring is not used. */
#define WDGM_EB_DM_MAINFUNCTIONCYCLETIME [!"$MainFunctionCycleTime"!]U

#if (defined WDGM_EB_DM_MAINFUNCTIONTOLERANCETIME)
#error WDGM_EB_DM_MAINFUNCTIONTOLERANCETIME already defined
#endif
/** \brief Tolerance time which is allowed between two Main Function calls.
 * The time unit is with respect to the value of macro WDGM_EB_DM_TIMEGRANULARITY.
 * Equals 0 if Deadline Monitoring is not used. */
#define WDGM_EB_DM_MAINFUNCTIONTOLERANCETIME [!"$MainFunctionToleranceTime"!]U

/*------------------[WdgM Multi-Core implementation macros]-----------------------*/

#if (defined WDGM_EB_MULTICORE_ENABLED)
#error WDGM_EB_MULTICORE_ENABLED already defined
#endif

/** \brief Preprocessor macro verifying if the multicore functionality is enabled. */
#define WDGM_EB_MULTICORE_ENABLED [!//
[!IF "$WdgMMulticoreEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_NUMBER_OF_CORES)
#error WDGM_EB_NUMBER_OF_CORES  already defined
#endif
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020111_Conf,1 */
/** \brief Preprocessor macro defining the number of cores configured. */
#define WDGM_EB_NUMBER_OF_CORES  [!"$WdgMNumberOfCores"!]U

#if (defined WDGM_EB_MASTER_INSTANCE)
#error WDGM_EB_MASTER_INSTANCE already defined
#endif
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020113_Conf,1 */
/** \brief Preprocessor macro defining the core configured as master core. */
#define WDGM_EB_MASTER_INSTANCE [!"$WdgMMasterCoreId"!]U

#if (defined WDGM_EB_MASTER_WAIT_SLAVE_MODE_SWITCH)
#error WDGM_EB_MASTER_WAIT_SLAVE_MODE_SWITCH already defined
#endif
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020119_Conf,1 */
/** \brief   Preprocessor macro that defines the amount of time the WdgM Master Instance
* shall wait until WdgM Satellite Instances finish the mode switch or initialization.
* Unit: Number of master main function. */
#define WDGM_EB_MASTER_WAIT_SLAVE_MODE_SWITCH [!"$WdgMMasterWaitSlaveModeSwitch"!]U

[!IF "$WdgMMulticoreEnabled"!][!//
#if (defined WDGM_EB_SE_NUM)
#error WDGM_EB_SE_NUM already defined
#endif
/** \brief Overall number of supervised entities */
#define WDGM_EB_SE_NUM [!"$NumberOfSupervisedEntities"!]U
[!FOR "index_i" = "0" TO "num:i($WdgMNumberOfCores)-1"!][!/*
*/!][!VAR "SupEntExistPerCore" = "false()"!][!/*
*/!][!LOOP "as:modconf('WdgM')[1]/WdgMGeneral/WdgMSupervisedEntity/*"!][!/*
    */!][!IF "num:i(node:value(WdgMSupervisedEntityCoreId)) = num:i($index_i)"!][!/*
        */!][!VAR "SupEntExistPerCore" = "true()"!][!/*
    */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!IF "$SupEntExistPerCore"!]
#if (defined WDGM_EB_SE_NUM_CORE[!"$index_i"!])
#error WDGM_EB_SE_NUM_CORE[!"$index_i"!] already defined
#endif
/** \brief Number of supervised entities per core */
#define WDGM_EB_SE_NUM_CORE[!"$index_i"!] [!CALL "GetNumSEPerCore", "Core"="$index_i"!]U
[!ENDIF!][!/* IF $SupEntExistPerCore */!][!//
[!ENDFOR!][!/*

*/!][!ELSE!][!/* IF $WdgMMulticoreEnabled */!][!//
#if (defined WDGM_EB_SE_NUM)
#error WDGM_EB_SE_NUM already defined
#endif
/** \brief Overall number of supervised entities */
#define WDGM_EB_SE_NUM [!"$NumberOfSupervisedEntities"!]U[!//
[!ENDIF!][!/* IF $WdgMMulticoreEnabled */!][!//

[!IF "$WdgMMulticoreEnabled"!][!//
#if (defined WDGM_EB_CHECKPOINTS_NUM)
#error WDGM_EB_CHECKPOINTS_NUM already defined
#endif
/** \brief Overall number of configured checkpoints */
#define WDGM_EB_CHECKPOINTS_NUM [!"$NumberOfCheckpoints"!]U
[!FOR "index_i" = "0" TO "num:i($WdgMNumberOfCores)-1"!][!/*
*/!][!VAR "SupEntExistPerCore" = "false()"!][!/*
*/!][!LOOP "as:modconf('WdgM')[1]/WdgMGeneral/WdgMSupervisedEntity/*"!][!/*
    */!][!IF "num:i(node:value(WdgMSupervisedEntityCoreId)) = num:i($index_i)"!][!/*
        */!][!VAR "SupEntExistPerCore" = "true()"!][!/*
    */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!IF "$SupEntExistPerCore"!]
#if (defined WDGM_EB_CHECKPOINTS_NUM_CORE[!"$index_i"!])
#error WDGM_EB_CHECKPOINTS_NUM_CORE[!"$index_i"!] already defined
#endif
/** \brief Number of configured checkpoints per core */
#define WDGM_EB_CHECKPOINTS_NUM_CORE[!"$index_i"!] [!CALL "GetNumOfCheckpointsPerCore", "Core"="$index_i"!]U
[!ENDIF!][!/* IF $SupEntExistPerCore */!][!//
[!ENDFOR!][!//

#if (defined WDGM_EB_MASTER_STATUS_INIT)
#error WDGM_EB_MASTER_STATUS_INIT already defined
#endif
/** \brief WdgM master instance is initialized */
#define WDGM_EB_MASTER_STATUS_INIT       (Atomic_value_t)[!"$WdgMMasterPositionInGlobalInitStatus"!]U

#if (defined WDGM_EB_GLOBAL_STATUS_INIT)
#error WDGM_EB_GLOBAL_STATUS_INIT already defined
#endif
/** \brief all cores are initialized */
#define WDGM_EB_GLOBAL_STATUS_INIT       (Atomic_value_t)[!"$WdgMAllCoresInitialized"!]U[!//

[!ELSE!][!/* IF $WdgMMulticoreEnabled */!]
#if (defined WDGM_EB_CHECKPOINTS_NUM)
#error WDGM_EB_CHECKPOINTS_NUM already defined
#endif
/** \brief Overall number of configured checkpoints */
#define WDGM_EB_CHECKPOINTS_NUM [!"$NumberOfCheckpoints"!]U
[!ENDIF!][!/* IF $WdgMMulticoreEnabled */!][!//

#if (defined WDGM_EB_GLOBAL_STATUS_DEINIT)
#error WDGM_EB_GLOBAL_STATUS_DEINIT already defined
#endif
/** \brief all cores are de-initialized */
#define WDGM_EB_GLOBAL_STATUS_DEINIT           0U
[!VAR "index_i" = "0"!][!/*
*/!][!LOOP "node:order(as:modconf('WdgM')[1]/WdgMConfigSet/*[1]/WdgMMode/*, 'WdgMModeId')"!][!/*
  */!][!LOOP "WdgMExternalLogicalSupervision/*"!][!/*
    */!][!VAR "tmpText"!][!ENDVAR!][!/*
    */!][!LOOP "WdgMExternalTransition/*"!][!/*
      */!][!VAR "tmpText"!][!"$tmpText"!]:[!"num:i(as:ref(WdgMExternalTransitionSourceRef)/../../WdgMSupervisedEntityId)"!]:[!"num:i(as:ref(WdgMExternalTransitionDestRef)/../../WdgMSupervisedEntityId)"!][!ENDVAR!][!/*
    */!][!ENDLOOP!][!//
#if (defined WDGM_EB_SE_NUM_GRAPHID_[!"num:i($index_i)"!])
#error WDGM_EB_SE_NUM_GRAPHID_[!"num:i($index_i)"!] already defined
#endif
/** \brief Number of configured SE per external graph */
#define WDGM_EB_SE_NUM_GRAPHID_[!"num:i($index_i)"!] [!"num:i(count(text:order(text:split($tmpText,':'))))"!]U

[!/**/!][!VAR "index_i" = "$index_i + 1"!][!/*
  */!][!ENDLOOP!][!/*
*/!][!ENDLOOP!][!//

/*------------------[Optional internal mode switch during de-initialization]---------------------*/

#if (defined WDGM_EB_DEINIT_MODE_ENABLED)
#error WDGM_EB_DEINIT_MODE_ENABLED already defined
#endif
/** \brief DeInit mode */
#define WDGM_EB_DEINIT_MODE_ENABLED [!//
[!IF "$SleepModeEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/*------------------[WdgM Interface version check]-----------------------------------------------*/

#if (defined WDGM_EB_STATICIF_VERSION) /* to prevent double declaration */
#error WDGM_EB_STATICIF_VERSION already defined
#endif
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020202,1 */
/** \brief WdgM Static interface version */
#define WDGM_EB_STATICIF_VERSION          1

#if (defined WDGM_EB_GENIF_VERSION_REQ) /* to prevent double declaration */
#error WDGM_EB_GENIF_VERSION_REQ already defined
#endif
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020203,1 */
/** \brief WdgM Generated interface version required by component WdgM Static */
#define WDGM_EB_GENIF_VERSION_REQ         1

#if (defined WDGM_EB_GENIF_VERSION_CHECK) /* to prevent double declaration */
#error WDGM_EB_GENIF_VERSION_CHECK already defined
#endif

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020204,1 */
/** \brief Interface compatibility check macro for component WdgM Generated
 *
 * \param[in] staticif_req the required interface version of WdgM Static
 * \param[in] generatedif the interface version of WdgM Generated
 *
 * \retval TRUE The two components WdgM Static and WdgM Generated are compatible
 * \retval FALSE The two components WdgM Static and WdgM Generated are not compatible
 */
#define WDGM_EB_GENIF_VERSION_CHECK(staticif_req, generatedif)   \
    ((WDGM_EB_STATICIF_VERSION >= (staticif_req)) &&             \
     (WDGM_EB_GENIF_VERSION_REQ <= (generatedif)))

/* ***************** EB specific macros regarding optimizations ******************************** */

#if (defined WDGM_EB_INCLUDE_RTE)
#error WDGM_EB_INCLUDE_RTE already defined
#endif
/** \brief availability of Rte */
#define WDGM_EB_INCLUDE_RTE [!//
[!IF "$RteUsageEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_CALLERIDS_ENABLED)
#error WDGM_EB_CALLERIDS_ENABLED already defined
#endif
/** \brief Number of supervised entities */
#define WDGM_EB_CALLERIDS_ENABLED [!//
[!IF "$CallerIdsEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined WDGM_EB_LOGICAL_SUPERVISION_ENABLED)
#error WDGM_EB_LOGICAL_SUPERVISION_ENABLED already defined
#endif
/** \brief Usage of logical supervision */
#if (WDGM_EB_INTERNAL_GRAPH_NUM > 0U) || (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
#define WDGM_EB_LOGICAL_SUPERVISION_ENABLED STD_ON
#else
#define WDGM_EB_LOGICAL_SUPERVISION_ENABLED STD_OFF
#endif

#if (defined WDGM_EB_DEADLINE_SUPERVISION_ENABLED)
#error WDGM_EB_DEADLINE_SUPERVISION_ENABLED already defined
#endif
/** \brief Usage of deadline supervision */
#if (WDGM_EB_DM_NUM > 0U)
#define WDGM_EB_DEADLINE_SUPERVISION_ENABLED STD_ON
#else
#define WDGM_EB_DEADLINE_SUPERVISION_ENABLED STD_OFF
#endif

#if ((WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON) && \
     ((WDGM_EB_PROD_ERR_HANDLING_MF_TIMINGVIOLATION == WDGM_PROD_ERR_REP_TO_DEM) || \
      ((WDGM_EB_PROD_ERR_HANDLING_MF_TIMINGVIOLATION == WDGM_PROD_ERR_REP_TO_DET) && \
       (WDGM_DEV_ERROR_DETECT == STD_ON))))
/** \brief Check if MainFunction timing violations shall be detected */
#define WDGM_EB_CHECK_MAINFUNCTION_TIMING STD_ON
#else
#define WDGM_EB_CHECK_MAINFUNCTION_TIMING STD_OFF
#endif

/* ***************** EB specific macros regarding inter-core communication ******************************** */
[!IF "$WdgMMulticoreEnabled"!]

#if (defined WDGM_EB_ATOMIC_INIT)
#error WDGM_EB_ATOMIC_INIT already defined
#endif
/* !LINKSTO WDGM.EB.Design.InterCoreShareData1,1 */
/* Internal macro to initialize an atomic object at runtime in multicore systems */
#define WDGM_EB_ATOMIC_INIT(x,y) Atomics_Init(&(x), (Atomic_value_t)(y))

#if (defined WDGM_EB_ATOMIC_STORE)
#error WDGM_EB_ATOMIC_STORE already defined
#endif
/* !LINKSTO WDGM.EB.Design.InterCoreShareData2,1 */
/* Internal macro to store data atomically in multicore systems */
#define WDGM_EB_ATOMIC_STORE(x,y) Atomics_Store(&(x), (Atomic_value_t)(y))

#if (defined WDGM_EB_ATOMIC_LOAD)
#error WDGM_EB_ATOMIC_LOAD already defined
#endif
/* !LINKSTO WDGM.EB.Design.InterCoreShareData3,1 */
/* Internal macro to load data atomically in multicore systems */
#define WDGM_EB_ATOMIC_LOAD(x) Atomics_Load(&(x))

#if (defined WDGM_EB_ATOMIC_SET_BIT)
#error WDGM_EB_ATOMIC_SET_BIT already defined
#endif
/* !LINKSTO WDGM.EB.Design.InterCoreShareData4,1 */
/* Internal macro to set one bit atomically in multicore systems */
#define WDGM_EB_ATOMIC_SET_BIT(x,y) Atomics_FetchOr(&(x), (Atomic_value_t)(1U << (y)))

#if (defined WDGM_EB_ATOMIC_CLR_BIT)
#error WDGM_EB_ATOMIC_CLR_BIT already defined
#endif
/* !LINKSTO WDGM.EB.Design.InterCoreShareData5,1 */
/* Internal macro to clear one bit atomically in multicore systems */
#define WDGM_EB_ATOMIC_CLR_BIT(x,y) Atomics_ClearFlag(&(x), (Atomic_value_t)(1U << (y)))

#if (defined WDGM_EB_CHECK_INIT)
#error WDGM_EB_CHECK_INIT already defined
#endif
/* Internal macro to to check if the WdgM core instance is initialized in multicore systems */
#define WDGM_EB_CHECK_INIT(x,y) (WDGM_EB_ATOMIC_LOAD(x) & (1U<< (y)))

[!ELSE!][!/* IF $WdgMMulticoreEnabled */!][!//

/* Internal macro to get the specific run time data of the WdgM. */
#define WdgM_GetCoreInstance() ((WdgM_EB_CoreContextType *)&WdgM_EB_Context)

#if (defined WDGM_EB_ATOMIC_INIT)
#error WDGM_EB_ATOMIC_INIT already defined
#endif
/* Internal macro to initialize an atomic object at runtime in single core systems */
#define WDGM_EB_ATOMIC_INIT(x,y) (x) = (y)

#if (defined WDGM_EB_ATOMIC_STORE)
#error WDGM_EB_ATOMIC_STORE already defined
#endif
 /* Internal macro to store data atomically in single core systems */
#define WDGM_EB_ATOMIC_STORE(x,y) (x) = (y)

#if (defined WDGM_EB_ATOMIC_LOAD)
#error WDGM_EB_ATOMIC_LOAD already defined
#endif
 /* Internal macro to load data atomically in single core systems */
#define WDGM_EB_ATOMIC_LOAD(x) (x)

#if (defined WDGM_EB_ATOMIC_SET_BIT)
#error WDGM_EB_ATOMIC_SET_BIT already defined
#endif
/* Internal macro to set one bit atomically in single core systems */
#define WDGM_EB_ATOMIC_SET_BIT(x,y) ((x) = ((x) | (1U << (y))))

#if (defined WDGM_EB_ATOMIC_CLR_BIT)
#error WDGM_EB_ATOMIC_CLR_BIT already defined
#endif
/* Internal macro to clear one bit atomically in single core systems */
#define WDGM_EB_ATOMIC_CLR_BIT(x,y) ((x) = (x) & (~(1U << (y))))

#if (defined WDGM_EB_CHECK_INIT)
#error WDGM_EB_CHECK_INIT already defined
#endif
/* Internal macro to check if the WdgM is initialized in single core systems */
#define WDGM_EB_CHECK_INIT(x,y) ((x) & (1U << (y)))
[!ENDIF!][!//

#if (defined WDGM_EB_GET_CORE_ID)
#error WDGM_EB_GET_CORE_ID already defined
#endif
/* Internal macro for link external callout API for retrieving the core Id */
#define WDGM_EB_GET_CORE_ID() WDGM_EXT_GET_CORE_ID()

#define WDGM_INSTANCE_ID (WDGM_EB_GET_CORE_ID())

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

[!IF "($WdgMMulticoreEnabled = 'true')"!][!//
[!VAR "MULTICORE_NUM_CORES" = "node:value(as:modconf('WdgM')[1]/WdgMGeneral/WdgMGeneralMulticore/WdgMNumberOfCores)"!][!//
[!VAR "WDGM_MASTER_CORE_ID" = "node:value(as:modconf('WdgM')[1]/WdgMGeneral/WdgMGeneralMulticore/WdgMMasterCoreId)"!][!//
[!IF "$DefaultServiceAPI = 'NONE'"!][!//
[!FOR "Index" = "0" TO "$MULTICORE_NUM_CORES - 1"!][!//
[!IF "$Index != $WDGM_MASTER_CORE_ID"!][!//
FUNC(void, WDGM_1_CORE_[!"num:i($Index)"!]_CODE) WdgM_1_CORE_[!"num:i($Index)"!]_MainFunction(void);
[!ENDIF!][!//
[!ENDFOR!][!//
[!ENDIF!][!//
[!ENDIF!][!//


/*==================[external constants]=========================================================*/

/*==================[external data]==============================================================*/

#endif /* ifndef WDGM_CFG_H */
/*==================[end of file]================================================================*/
[!ENDCODE!][!//
