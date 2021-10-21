/**
 * \file
 *
 * \brief AUTOSAR FrSM
 *
 * This file contains the implementation of the AUTOSAR
 * module FrSM.
 *
 * \version 5.3.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!INCLUDE "../include/FrSM_Variables.m"!][!//
#ifndef FRSM_CFG_H
#define FRSM_CFG_H

/*==================[includes]==============================================*/

#include <ComStack_Types.h>
#include <TSAutosar.h>          /* TS_PROD_ERR_* defines */
#include <FrSM_Version.h>

/*==================[macros]================================================*/




/*------------------[Defensive programming]---------------------------------*/
[!SELECT "FrSMDefensiveProgramming"!][!//

#if (defined FRSM_DEFENSIVE_PROGRAMMING_ENABLED)
#error FRSM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define FRSM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../FrSMGeneral/FrSMDevErrorDetect  = 'true') and (FrSMDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRSM_PRECONDITION_ASSERT_ENABLED)
#error FRSM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define FRSM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../FrSMGeneral/FrSMDevErrorDetect  = 'true') and (FrSMDefProgEnabled = 'true') and (FrSMPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRSM_POSTCONDITION_ASSERT_ENABLED)
#error FRSM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define FRSM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../FrSMGeneral/FrSMDevErrorDetect  = 'true') and (FrSMDefProgEnabled = 'true') and (FrSMPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRSM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error FRSM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define FRSM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../FrSMGeneral/FrSMDevErrorDetect  = 'true') and (FrSMDefProgEnabled = 'true') and (FrSMUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRSM_INVARIANT_ASSERT_ENABLED)
#error FRSM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define FRSM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../FrSMGeneral/FrSMDevErrorDetect  = 'true') and (FrSMDefProgEnabled = 'true') and (FrSMInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRSM_STATIC_ASSERT_ENABLED)
#error FRSM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define FRSM_STATIC_ASSERT_ENABLED           [!//
[!IF "(../FrSMGeneral/FrSMDevErrorDetect  = 'true') and (FrSMDefProgEnabled = 'true') and (FrSMStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//



/* standard SWS pre-compile time configuration parameters */

[!CALL "GuardedDefine", "Comment"="'Development error detection enable switch'",
  "Name"="'FRSM_DEV_ERROR_DETECT'"!] [!IF "$VarFrSMDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Version info API enable switch'",
  "Name"="'FRSM_VERSION_INFO_API'"!] [!IF "$VarFrSMVersionInfoApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'<Cdd>_SyncLossErrorIndication function enable switch'",
  "Name"="'FRSM_SYNC_LOSS_ERROR_INDICATION_ENABLE'"!] [!IF "$VarFrSMSyncLossIndicationEnable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "$VarFrSMSyncLossIndicationEnable"!][!//
[!CALL "GuardedDefine", "Comment"="'<Cdd>_SyncLossErrorIndication function name'",
  "Name"="'FRSM_SYNC_LOSS_ERROR_INDICATION_NAME'"!] [!"FrSMGeneral/FrSMSyncLossErrorIndicationName"!]
[!ENDIF!][!//

/* vendor specific pre-compile time configuration parameters */

[!CALL "GuardedDefine", "Comment"="'EB-specific switch to disable usage of FlexRay transceiver driver'",
  "Name"="'FRSM_FRTRCV_CONTROL_ENABLE'"!] [!IF "$VarFrSMFrTrcvControlEnable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'EB-specific switch to enable optimizations if a single FlexRay cluster is used'",
  "Name"="'FRSM_SINGLE_CLST_OPT_ENABLE'"!] [!IF "$VarFrSMSingleClstOptEnable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'EB-specific switch to disable mode indications from FrSM to ComM'",
  "Name"="'FRSM_COMM_INDICATION_ENABLE'"!] [!IF "$VarFrSMComMIndicationEnable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'EB-specific switch to disable mode indications from FrSM to BswM'",
  "Name"="'FRSM_REPORT_TO_BSMW_ENABLE'"!] [!IF "$VarFrSMReportToBswMEnable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'EB-specific switch to disable API to set transceivers to passive/active mode'",
  "Name"="'FRSM_SETECUPASSIVE_ENABLE'"!] [!IF "$VarFrSMSetEcuPassiveEnable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'EB-specific switch to disable call of \a FrNm_StartupError()'",
  "Name"="'FRSM_FRNM_STARTUPERROR_ENABLE'"!] [!IF "$VarFrSMFrNmStartupErrorEnable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'EB-specific switch to disable support for single slot mode'",
  "Name"="'FRSM_KEYSLOTONLYMODE_ENABLE'"!] [!IF "$VarFrSMKeySlotOnlyModeEnable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'EB-specific switch to enable support for multicore mode.'",
  "Name"="'FRSM_MULTI_CORE_SUPPORT_ENABLE'"!] [!IF "$VarFrSMMultiCoreSupportEnable"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "$VarFrSMSyncLossIndicationHeaderEnable"!][!//
[!CALL "GuardedDefine", "Comment"="'<Cdd>_SyncLossErrorIndication header file name'",
  "Name"="'FRSM_SYNC_LOSS_ERROR_INDICATION_HEADER'"!] <[!"FrSMGeneral/FrSMSyncLossErrorIndicationHeaderName"!]>
[!ENDIF!][!//

[!CALL "GuardedDefine", "Comment"="'Switch for DEM to DET reporting for production error FRSM_E_CLUSTER_STARTUP'",
  "Name"="'FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP'"!]   [!//
[!IF "ReportToDem/FrSMClusterStartupReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/FrSMClusterStartupReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "ReportToDem/FrSMClusterStartupReportToDem = 'DET'"!][!//
[!CALL "GuardedDefine", "Comment"="'Det error ID, if DEM to DET reporting is enabled for FRSM_E_CLUSTER_STARTUP'",
  "Name"="'FRSM_E_DEMTODET_CLUSTER_STARTUP'"!]          [!"ReportToDem/FrSMClusterStartupReportToDemDetErrorId"!]U
[!ENDIF!][!//


[!CALL "GuardedDefine", "Comment"="'Switch for DEM to DET reporting for production error FRSM_E_CLUSTER_SYNC_LOSS'",
  "Name"="'FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS'"!] [!//
[!IF "ReportToDem/FrSMClusterSyncLossReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/FrSMClusterSyncLossReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "ReportToDem/FrSMClusterSyncLossReportToDem = 'DET'"!][!//
[!CALL "GuardedDefine", "Comment"="'Det error ID, if DEM to DET reporting is enabled for FRSM_E_CLUSTER_SYNC_LOSS'",
  "Name"="'FRSM_E_DEMTODET_CLUSTER_SYNC_LOSS'"!]        [!"ReportToDem/FrSMClusterSyncLossReportToDemDetErrorId"!]U
[!ENDIF!][!//

#if (FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_ON)
[!CALL "GuardedDefine", "Comment"="'Multi Core support enabled --> use SchM Call to indicate actual bus mode'",
  "Name"="'FRSM_CALL_COMM_BUSSM_MODEINDICATION'", "Params"="'(FrIfClstIdx, Channel, ComMode)'"!]        FrSM_ComM_BusSM_ModeIndicationFpList[(FrIfClstIdx)]((Channel), (ComMode))
#else
[!CALL "GuardedDefine", "Comment"="'Multi Core support disabled --> use direct ComM Call to indicate actual bus mode'",
  "Name"="'FRSM_CALL_COMM_BUSSM_MODEINDICATION'", "Params"="'(Channel, ComMode)'"!]        ComM_BusSM_ModeIndication((Channel), (ComMode))
#endif /* FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_ON */

#endif /* FRSM_CFG_H */

/*==================[end of file]===========================================*/
