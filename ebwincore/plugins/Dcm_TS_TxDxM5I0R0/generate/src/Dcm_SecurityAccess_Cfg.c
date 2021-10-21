/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO Dcm.Dsn.File.SecurityAccess.ConfigC,1 */

/*==================[inclusions]=================================================================*/
[!AUTOSPACING!]
#include <Dcm_Int.h>
#include <Dcm_API_Cfg.h>                                  /*  Configurable function declarations */
#include <Dcm_DspExternal.h>                          /* Callbacks provided by Central Diag SwC  */

#include <Dcm_SecurityAccess_Cfg.h>
[!INDENT "0"!]
/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[internal constants]=========================================================*/

[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmServiceAPI/DcmDefaultASRServiceAPI = 'AUTOSAR_42'"!][!//
  [!VAR "DefaultASRVersion" = "'DCM_USE_AUTOSAR_42'"!][!//
[!ELSE!][!//
  [!VAR "DefaultASRVersion" = "'DCM_USE_AUTOSAR_40'"!][!//
[!ENDIF!][!//
[!//
[!VAR "DcmTaskTime" = "DcmConfigSet/*[1]/DcmGeneral/DcmTaskTime"!][!//
[!//
/*==================[internal data]==============================================================*/

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

[!IF "node:exists( DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityRow/*)"!]
  /* Array of configuration information for diagnostic security levels */
  CONST(Dcm_SecurityLevelEntryConfigType, DCM_CONST) Dcm_SecurityLevelEntryConfig[[!"num:integer( count( DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityRow/*))"!]] =
  {
    [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityRow/*"!][!//
      /* !LINKSTO Dcm.EB.ServiceAPI.SecurityAccess,1 */
      [!IF "$DefaultASRVersion = 'DCM_USE_AUTOSAR_42'"!][!//
        [!VAR "RteConnectedASRVersion" = "'DCM_USE_AUTOSAR_42'"!][!//
      [!ELSE!][!//
        [!VAR "RteConnectedASRVersion" = "'DCM_USE_AUTOSAR_40'"!][!//
      [!ENDIF!][!//
      [!WS "2"!]{
      [!INDENT "4"!]
        [!/* Generate only if the delay timers feature is enabled for security access */!][!//
        [!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityRow/*[(DcmDspSecurityDelayTime != 0) or (DcmDspSecurityDelayTimeOnBoot != 0)])"!][!//
          [!"num:integer((DcmDspSecurityDelayTime) div $DcmTaskTime)"!]U, /* Delay time */
          [!"num:integer((DcmDspSecurityDelayTimeOnBoot) div $DcmTaskTime)"!]U, /* Delay on boot */
        [!ENDIF!]
        [!"DcmDspSecuritySeedSize"!]U, /* Seed size */
        [!"DcmDspSecurityKeySize"!]U, /* Key size */
        [!IF "node:exists(DcmDspSecurityADRSize)"!]
          [!"DcmDspSecurityADRSize"!]U, /* ADR Size */
        [!ELSE!]
          0U,
        [!ENDIF!]
        [!/*---GetSeedFnc------*/!][!//
        [!IF "DcmDspSecurityADRSize > 0"!][!//
        /* !LINKSTO SWS_Dcm_91003.Port.ADR.USE_SYNCH_FNC,1 */
          [!IF "DcmDspSecurityUsePort = 'USE_SECURITY_SYNCH_FNC'"!][!//
            [!IF "node:exists(DcmDspSecurityGetSeedFnc)"!][!//
              &[!"DcmDspSecurityGetSeedFnc"!],
              NULL_PTR,
              NULL_PTR,
              NULL_PTR,
            [!ELSE!][!//
              NULL_PTR,
              NULL_PTR,
              NULL_PTR,
              NULL_PTR,
            [!ENDIF!][!//
          /* !LINKSTO SWS_Dcm_91003.Port.ADR.USE_ASYNCH_FNC,1 */
          [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_ASYNCH_FNC'"!][!//
            [!IF "node:exists(DcmDspSecurityGetSeedFnc)"!][!//
              NULL_PTR,
              &[!"DcmDspSecurityGetSeedFnc"!],
              NULL_PTR,
              NULL_PTR,
            [!ELSE!][!//
              NULL_PTR,
              NULL_PTR,
              NULL_PTR,
              NULL_PTR,
            [!ENDIF!][!//
          /* !LINKSTO SWS_Dcm_91003.Port.ADR.USE_SYNCH_CS,1 */
          [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_SYNCH_CLIENT_SERVER'"!]
            &Rte_Call_SecurityAccess_[!"node:name(.)"!]_GetSeed,
            NULL_PTR,
            NULL_PTR,
            NULL_PTR,
          /* !LINKSTO SWS_Dcm_91003.Port.ADR.USE_ASYNCH_CS,1 */
          [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_ASYNCH_CLIENT_SERVER'"!]
            NULL_PTR,
            &Rte_Call_SecurityAccess_[!"node:name(.)"!]_GetSeed,
            NULL_PTR,
            NULL_PTR,
          [!ELSE!][!//
            NULL_PTR,
            NULL_PTR,
            NULL_PTR,
            NULL_PTR,
          [!ENDIF!][!//
        [!ELSE!][!//
          /* !LINKSTO SWS_Dcm_91003.Port.NO_ADR.USE_SYNCH_FNC,1 */
          [!IF "DcmDspSecurityUsePort = 'USE_SECURITY_SYNCH_FNC'"!][!//
            [!IF "node:exists(DcmDspSecurityGetSeedFnc)"!][!//
              NULL_PTR,
              NULL_PTR,
              &[!"DcmDspSecurityGetSeedFnc"!],
              NULL_PTR,
            [!ELSE!][!//
              NULL_PTR,
              NULL_PTR,
              NULL_PTR,
              NULL_PTR,
            [!ENDIF!][!//
          /* !LINKSTO SWS_Dcm_91003.Port.NO_ADR.USE_ASYNCH_FNC,1 */
          [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_ASYNCH_FNC'"!][!//
            [!IF "node:exists(DcmDspSecurityGetSeedFnc)"!][!//
              NULL_PTR,
              NULL_PTR,
              NULL_PTR,
              &[!"DcmDspSecurityGetSeedFnc"!],
            [!ELSE!][!//
              NULL_PTR,
              NULL_PTR,
              NULL_PTR,
              NULL_PTR,
            [!ENDIF!][!//
          /* !LINKSTO SWS_Dcm_91003.Port.NO_ADR.USE_SYNCH_CS,1 */
          [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_SYNCH_CLIENT_SERVER'"!]
            NULL_PTR,
            NULL_PTR,
            &Rte_Call_SecurityAccess_[!"node:name(.)"!]_GetSeed,
            NULL_PTR,
          /* !LINKSTO SWS_Dcm_91003.Port.NO_ADR.USE_ASYNCH_CS,1 */
          [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_ASYNCH_CLIENT_SERVER'"!]
            NULL_PTR,
            NULL_PTR,
            NULL_PTR,
            &Rte_Call_SecurityAccess_[!"node:name(.)"!]_GetSeed,
          [!ELSE!][!//
            NULL_PTR,
            NULL_PTR,
            NULL_PTR,
            NULL_PTR,
          [!ENDIF!][!//
        [!ENDIF!][!//
        [!/*---CompareKeyFnc------*/!][!//
        [!IF "DcmDspSecurityUsePort = 'USE_SECURITY_SYNCH_FNC'"!][!//
          [!IF "node:exists(DcmDspSecurityCompareKeyFnc)"!][!//
            [!IF "$RteConnectedASRVersion = 'DCM_USE_AUTOSAR_42'"!][!//
            /* !LINKSTO SWS_Dcm_91004.AS_42.Port.USE_SYNCH_FNC,1 */
              NULL_PTR,
              &[!"DcmDspSecurityCompareKeyFnc"!],
            [!ELSE!][!//
            /* !LINKSTO SWS_Dcm_91004.AS_40.Port.USE_SYNCH_FNC,1 */
              &[!"DcmDspSecurityCompareKeyFnc"!],
              NULL_PTR,
            [!ENDIF!][!//
            NULL_PTR,
            NULL_PTR,
          [!ELSE!][!//
            NULL_PTR,
            NULL_PTR,
            NULL_PTR,
            NULL_PTR,
          [!ENDIF!][!//
        [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_ASYNCH_FNC'"!][!//
          [!IF "node:exists(DcmDspSecurityCompareKeyFnc)"!][!//
            NULL_PTR,
            NULL_PTR,
            [!IF "$RteConnectedASRVersion = 'DCM_USE_AUTOSAR_42'"!][!//
            /* !LINKSTO SWS_Dcm_91004.AS_42.Port.USE_ASYNCH_FNC,1 */
              NULL_PTR,
              &[!"DcmDspSecurityCompareKeyFnc"!],
            [!ELSE!][!//
            /* !LINKSTO SWS_Dcm_91004.AS_40.Port.USE_ASYNCH_FNC,1 */
              &[!"DcmDspSecurityCompareKeyFnc"!],
              NULL_PTR,
            [!ENDIF!][!//
          [!ELSE!][!//
            NULL_PTR,
            NULL_PTR,
            NULL_PTR,
            NULL_PTR,
          [!ENDIF!][!//
        [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_SYNCH_CLIENT_SERVER'"!]
          [!IF "$RteConnectedASRVersion = 'DCM_USE_AUTOSAR_42'"!][!//
            /* !LINKSTO SWS_Dcm_91004.AS_42.Port.USE_SYNCH_CS,1 */
            NULL_PTR,
            &Rte_Call_SecurityAccess_[!"node:name(.)"!]_CompareKey,
          [!ELSE!][!//
            /* !LINKSTO SWS_Dcm_91004.AS_40.Port.USE_SYNCH_CS,1 */
            &Rte_Call_SecurityAccess_[!"node:name(.)"!]_CompareKey,
            NULL_PTR,
          [!ENDIF!][!//
          NULL_PTR,
          NULL_PTR,
        [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_ASYNCH_CLIENT_SERVER'"!]
          NULL_PTR,
          NULL_PTR,
          [!IF "$RteConnectedASRVersion = 'DCM_USE_AUTOSAR_42'"!][!//
            /* !LINKSTO SWS_Dcm_91004.AS_42.Port.USE_ASYNCH_CS,1 */
            NULL_PTR,
            &Rte_Call_SecurityAccess_[!"node:name(.)"!]_CompareKey,
          [!ELSE!][!//
            /* !LINKSTO SWS_Dcm_91004.AS_40.Port.USE_ASYNCH_CS,1 */
            &Rte_Call_SecurityAccess_[!"node:name(.)"!]_CompareKey,
            NULL_PTR,
          [!ENDIF!][!//
        [!ELSE!][!//
          NULL_PTR,
          NULL_PTR,
          NULL_PTR,
          NULL_PTR,
        [!ENDIF!][!//
        [!/* Generate only if the delay timers feature is enabled for security access */!][!//
        [!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityRow/*[(DcmDspSecurityDelayTime != 0) or (DcmDspSecurityDelayTimeOnBoot != 0)])"!][!//
          [!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityRow/*[DcmDspSecurityAttemptCounterEnabled = 'true'])"!]
            [!IF "DcmDspSecurityAttemptCounterEnabled = 'true'"!][!//
              /* !LINKSTO SWS_Dcm_01041.Port.USE_SYNCH_INTERFACE,1 */
              /* !LINKSTO SWS_Dcm_00685.Interface.USE_SYNCH_INTERFACE,1 */
              [!/*---GetSecurityAttemptCounterFnc------*/!][!//
              [!IF "DcmDspSecurityUsePort = 'USE_SECURITY_SYNCH_FNC'"!][!//
                [!IF "node:exists(DcmDspSecurityGetAttemptCounterFnc)"!][!//
                  &[!"DcmDspSecurityGetAttemptCounterFnc"!],
                  NULL_PTR,
                [!ELSE!][!//
                  NULL_PTR,
                  NULL_PTR,
                [!ENDIF!][!//
              /* !LINKSTO SWS_Dcm_01041.Port.USE_ASYNCH_INTERFACE,1 */
              /* !LINKSTO SWS_Dcm_00685.Interface.AttemptCounterEnabled.USE_ASYNCH_INTERFACE,1 */
              [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_ASYNCH_FNC'"!][!//
                [!IF "node:exists(DcmDspSecurityGetAttemptCounterFnc)"!][!//
                  NULL_PTR,
                  &[!"DcmDspSecurityGetAttemptCounterFnc"!],
                [!ELSE!][!//
                  NULL_PTR,
                  NULL_PTR,
                [!ENDIF!][!//
              [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_SYNCH_CLIENT_SERVER'"!]
                &Rte_Call_SecurityAccess_[!"node:name(.)"!]_GetSecurityAttemptCounter,
                NULL_PTR,
              [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_ASYNCH_CLIENT_SERVER'"!]
                NULL_PTR,
                &Rte_Call_SecurityAccess_[!"node:name(.)"!]_GetSecurityAttemptCounter,
              [!ELSE!][!//
                NULL_PTR,
                NULL_PTR,
              [!ENDIF!][!//
              [!/*---SetSecurityAttemptCounter------*/!][!//
              [!IF "DcmDspSecurityUsePort = 'USE_SECURITY_SYNCH_FNC'"!][!//
                [!IF "node:exists(DcmDspSecuritySetAttemptCounterFnc)"!][!//
                  &[!"DcmDspSecuritySetAttemptCounterFnc"!],
                  NULL_PTR,
                [!ELSE!][!//
                  NULL_PTR,
                  NULL_PTR,
                [!ENDIF!][!//
              [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_ASYNCH_FNC'"!][!//
                [!IF "node:exists(DcmDspSecuritySetAttemptCounterFnc)"!][!//
                  NULL_PTR,
                  &[!"DcmDspSecuritySetAttemptCounterFnc"!],
                [!ELSE!][!//
                  NULL_PTR,
                  NULL_PTR,
                [!ENDIF!][!//
              [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_SYNCH_CLIENT_SERVER'"!]
                &Rte_Call_SecurityAccess_[!"node:name(.)"!]_SetSecurityAttemptCounter,
                NULL_PTR,
              [!ELSEIF "DcmDspSecurityUsePort = 'USE_SECURITY_ASYNCH_CLIENT_SERVER'"!]
                NULL_PTR,
                &Rte_Call_SecurityAccess_[!"node:name(.)"!]_SetSecurityAttemptCounter,
              [!ELSE!][!//
                NULL_PTR,
                NULL_PTR,
              [!ENDIF!][!//
            [!ELSE!][!//
              NULL_PTR,
              NULL_PTR,
              NULL_PTR,
              NULL_PTR,
            [!ENDIF!][!//
          [!ENDIF!][!//
          [!"DcmDspSecurityNumAttDelay"!]U, /* Attempts until delay */
          [!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityRow/*[DcmDspSecurityAttemptCounterEnabled = 'true'])"!]
              /* !LINKSTO SWS_Dcm_00685.Interface.AttemptCounterDisabled.USE_ASYNCH_INTERFACE,1 */
            [!IF "DcmDspSecurityAttemptCounterEnabled = 'true'"!]
              TRUE, /* Ext. handling of attempt counter */
            [!ELSE!]
              FALSE, /* Ext. handling of attempt counter */
            [!ENDIF!]
          [!ENDIF!]
          [!IF "DcmDspSecurityDelayTimeOnBootOverride = 'true'"!][!//
            TRUE,  /* Force delay on boot */
          [!ELSE!]
            FALSE, /* Do not force delay on boot */
          [!ENDIF!]
          [!IF "DcmDspSecurityDelayTimeInfinite = 'true'"!][!//
            TRUE, /* Force infinite delay time */
          [!ELSE!]
            FALSE, /* Do not force infinite delay time */
          [!ENDIF!]
        [!ENDIF!]
        [!/*---SecAccessInterface------*/!][!//
        DCM_[!"DcmDspSecurityUsePort"!],
        [!"$RteConnectedASRVersion"!],
        [!"DcmDspSecurityLevel"!]U, /* Security level */
      [!ENDINDENT!]
      [!WS "2"!]},
    [!ENDLOOP!][!//
  };
[!ENDIF!]

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/
[!ENDINDENT!]

/*==================[end of file]================================================================*/
