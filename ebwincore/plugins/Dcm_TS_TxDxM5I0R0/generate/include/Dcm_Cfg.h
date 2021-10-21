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
#ifndef DCM_CFG_H
#define DCM_CFG_H

/*==================[includes]===================================================================*/

[!AUTOSPACING!]
#include <Std_Types.h>

[!INCLUDE "Dcm_common.m"!]

/*==================[macros]=====================================================================*/

#if (defined DCM_CONTROL_MASK_EXTERNAL_ENABLED)
#error DCM_CONTROL_MASK_EXTERNAL_ENABLED is already defined
#endif

/** \brief Set to STD_ON if at least on DcmDspDidControlMask is set to DCM_CONTROLMASK_EXTERNAL */
[!IF "count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspDidInfo/*[DcmDspDidAccess/DcmDspDidControl/DcmDspDidControlMask = 'DCM_CONTROLMASK_EXTERNAL']) !=0"!]
#define DCM_CONTROL_MASK_EXTERNAL_ENABLED STD_ON
[!ELSE!]
#define DCM_CONTROL_MASK_EXTERNAL_ENABLED STD_OFF
[!ENDIF!]


#if (defined DCM_CONTROLMASK_EXTERNAL)
#error DCM_CONTROLMASK_EXTERNAL is already defined
#endif

/** \brief Definition of DCM_CONTROLMASK_EXTERNAL */
#define DCM_CONTROLMASK_EXTERNAL 0U

#if (defined DCM_CONTROLMASK_NOT_DEFINED)
#error DCM_CONTROLMASK_NOT_DEFINED is already defined
#endif

/** \brief Definition of DcmDspDidControlMask where DcmDspDidControlMask is set to something different then DCM_CONTROLMASK_EXTERNAL */
#define DCM_CONTROLMASK_NOT_DEFINED 1U

#if (defined DCM_CFG_SIGNATURE)
#error DCM_CFG_SIGNATURE is already defined
#endif
/* !LINKSTO Dcm.EB.Config.Id,1 */
/** \brief Sign a certain configuration with unique hash
 */
#define DCM_CFG_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('Dcm')[1]//
*[not(child::*) and(node:configclass() = 'PreCompile')],as:modconf('Dcm')[1]/DcmConfigSet/*))"!]U


[!INDENT "0"!][!//
[!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*"!][!//
  [!LOOP "DcmDslConnection/*"!][!//
    [!IF "node:exists(DcmDslProtocolTx/DcmDslTxConfirmationPduId)"!][!//
        [!/* Generate the symbol name. If the shortname is not unique this triggers a generation error. */!][!//
        [!CALL "GetSymbolName", "ShortNameRef"="'DcmDslProtocolTx'", "OldName"="name(./DcmDslProtocolTx)"!][!//
        /* !LINKSTO ecuc_sws_2108,1 */
        /** \brief Export symbolic name value for [!"../../@name"!]_[!"@name"!]_DcmDslProtocolTx */
#if (defined DcmConf_[!"$SymbolName"!])
  #error "DcmConf_[!"$SymbolName"!] is already defined"
#endif
        #define DcmConf_[!"$SymbolName"!]   [!"./DcmDslProtocolTx/DcmDslTxConfirmationPduId"!]U

    [!ENDIF!][!//
    [!LOOP "DcmDslPeriodicConnection/*"!][!//
        [!/* Generate the symbol name. If the shortname is not unique this triggers a generation error. */!][!//
        [!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(../.)"!][!//
        /* !LINKSTO ecuc_sws_2108,1 */
        /** \brief Export symbolic name value for [!"../../@name"!]_[!"@name"!]_DcmDslPeriodicConnection */
        #define DcmConf_[!"$SymbolName"!]   [!"./DcmDslPeriodicTxConfirmationPduId"!]U

    [!ENDLOOP!][!//
    [!IF "node:exists(DcmDslRoeTxConfirmationPduId)"!][!//
        [!/* Generate the symbol name. If the shortname is not unique this triggers a generation error. */!][!//
        [!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(.)"!][!//
        /* !LINKSTO ecuc_sws_2108,1 */
        /** \brief Export Symbolic Name value for parameter: DcmDslRoeTxConfirmationPduId */
        #define DcmConf_[!"$SymbolName"!]   [!"DcmDslRoeTxConfirmationPduId"!]U

    [!ENDIF!][!//
  [!ENDLOOP!][!//
  [!//
  [!VAR "ConnCount" = "0"!][!//
  [!LOOP "DcmDslConnection/*/DcmDslProtocolTx"!][!//
    #if (defined DCM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
    #if (defined Dcm_[!"name(../../../.)"!]_[!"name(.)"!]_[!"num:i($ConnCount)"!])
    #error Dcm_[!"name(../../../.)"!]_[!"name(.)"!]_[!"num:i($ConnCount)"!] already defined
    #endif
    /** \brief Export symbolic name value with module abbreviation as prefix only
     **        (AUTOSAR version <= AUTOSAR 4.0 rev2) */
    #define Dcm_[!"name(../../../.)"!]_[!"@name"!]_[!"num:i($ConnCount)"!] [!"num:i(./DcmDslTxConfirmationPduId)"!]
    #endif /* defined DCM_PROVIDE_LEGACY_SYMBOLIC_NAMES */

    [!VAR "ConnCount" = " $ConnCount +1"!][!//
  [!ENDLOOP!][!//
  [!//
  [!VAR "ConnCount" = "0"!][!//
  [!LOOP "DcmDslConnection/*[.='DcmDslPeriodicTransmission']/DcmDslPeriodicConnection/*"!][!//
    #if (defined DCM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
    #if (defined Dcm_[!"name(../../../.)"!]_[!"name(.)"!]_[!"num:i($ConnCount)"!])
    #error Dcm_[!"name(../../../.)"!]_[!"name(.)"!]_[!"num:i($ConnCount)"!] already defined
    #endif
    /** \brief Export symbolic name value with module abbreviation as prefix only
     **        (AUTOSAR version <= AUTOSAR 4.0 rev2) */
    #define Dcm_[!"name(../../../.)"!]_[!"@name"!]_[!"num:i($ConnCount)"!] [!"num:i(./DcmDslPeriodicTxConfirmationPduId)"!]
    #endif /* defined DCM_PROVIDE_LEGACY_SYMBOLIC_NAMES */

    [!VAR "ConnCount" = " $ConnCount +1"!][!//
  [!ENDLOOP!][!//
[!ENDLOOP!][!//
[!ENDINDENT!][!//

[!LOOP "DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*"!][!//
#if (defined DcmConf_[!"name(../../../../.)"!]_[!"./@name"!])
#error DcmConf_[!"name(../../../../.)"!]_[!"./@name"!] already defined
#endif
/** \brief Export symbolic name value */
#define DcmConf_[!"name(../../../../.)"!]_[!"@name"!] [!"./DcmDslProtocolRxPduId"!]

#if (defined DCM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined Dcm_[!"name(../../../../.)"!]_[!"name(.)"!])
#error Dcm_[!"name(../../../../.)"!]_[!"name(.)"!] already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 **        (AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Dcm_[!"name(../../../../.)"!]_[!"@name"!] [!"./DcmDslProtocolRxPduId"!]
#endif /* defined DCM_PROVIDE_LEGACY_SYMBOLIC_NAMES */

[!ENDLOOP!]
[!//
#if (defined DCM_NUM_PROTOCOL)       /* To prevent double declaration */
#error DCM_NUM_PROTOCOL already defined
#endif /* if (defined DCM_NUM_PROTOCOL) */

/** \brief Definition of number of protocols  */
#define DCM_NUM_PROTOCOL                     [!//
[!"num:i( count( DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*))"!]U

[!LOOP "DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*"!][!//
#if (defined DcmConf_[!"name(../../.)"!]_[!"./@name"!])
#error DcmConf_[!"name(../../.)"!]_[!"./@name"!] already defined
#endif
/** \brief Export symbolic name value */
#define DcmConf_[!"name(../../.)"!]_[!"@name"!] [!"./DcmDslProtocolID"!]

#if (defined DCM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined Dcm_[!"name(.)"!])
#error Dcm_[!"name(.)"!] already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only (AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Dcm_[!"@name"!] [!"./DcmDslProtocolID"!]
#endif /* defined DCM_PROVIDE_LEGACY_SYMBOLIC_NAMES */

[!ENDLOOP!]
[!//
#if (defined DCM_MAX_BUFFER_SIZE) /* To prevent double declaration */
#error DCM_MAX_BUFFER_SIZE already defined
#endif /* if (defined DCM_MAX_BUFFER_SIZE) */

/** \brief Definition of number of buffers */
#define DCM_MAX_BUFFER_SIZE                       [!//
[!"num:max(node:refs(DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*/DcmDslProtocolRxBufferID)/DcmDslBufferSize | node:refs(DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslProtocolTxBufferID)/DcmDslBufferSize)"!]U

#if (defined DCM_NUM_SID_TABLE) /* To prevent double declaration */
#error DCM_NUM_SID_TABLE already defined
#endif /* if (defined DCM_NUM_SID_TABLE) */

/** \brief Number Service ID table entries configured */
#define DCM_NUM_SID_TABLE                    [!//
[!"num:integer( count( DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*))"!]U

#if (defined DCM_ROE_ONDTCSTATUSCHANGE_SHORT_RESPONSE )  /* To prevent double declaration */
#error DCM_ROE_ONDTCSTATUSCHANGE_SHORT_RESPONSE already defined
#endif /* #if (defined DCM_ROE_ONDTCSTATUSCHANGE_SHORT_RESPONSE ) */
/** \brief Number of ROE Service that respond on DTC status change with a short message */
#define DCM_ROE_ONDTCSTATUSCHANGE_SHORT_RESPONSE      [!//
[!"num:i( count( as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId = num:hextoint('0x86')]/DcmDsdSubService/*[(DcmDsdSubServiceId = num:hextoint('0x01')) and (DcmROEonDtcStatusChangeShortResponse = 'true')] ) )"!]U

#if (defined DCM_NUM_SES_TAB_ENTRY) /* To prevent double declaration */
#error DCM_NUM_SES_TAB_ENTRY already defined
#endif /* if (defined DCM_NUM_SES_TAB_ENTRY) */

/** \brief Number of configured diagnostic sessions */
#define DCM_NUM_SES_TAB_ENTRY                [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspSession/*/DcmDspSessionRow/*))"!]U

[!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId = '47'])"!][!//
#if (defined DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT) /* To prevent double declaration */
#error DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT already defined
#endif /* if (defined DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT) */

/** \brief Processing time allowed for returncontroltoecu completion */
#define DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT                [!//
[!"num:i( DcmConfigSet/*[1]/DcmDsp/DcmDspReturnControlToEcuTimeout div DcmConfigSet/*[1]/DcmGeneral/DcmTaskTime)"!]U

#if (defined DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT_REPORT) /* To prevent double declaration */
#error DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT_REPORT already defined
#endif /* if (defined DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT_REPORT) */

/** \brief Det error report for returnControlToEcu is enables or not */
#define DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT_REPORT                [!//
[!IF "num:i(DcmConfigSet/*[1]/DcmDsp/DcmDspReturnControlToEcuTimeout) = '0'"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!]
[!ENDIF!][!//

[!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId = '17'])"!][!//
#if (defined DCM_DSP_POWER_DOWN_TIME) /* To prevent double declaration */
#error DCM_DSP_POWER_DOWN_TIME already defined
#endif /* if (defined DCM_DSP_POWER_DOWN_TIME) */

/** \brief Minimum stand-by time the server will remain in the power-down sequence */
#define DCM_DSP_POWER_DOWN_TIME                [!//
[!IF "node:exists( DcmConfigSet/*[1]/DcmDsp/DcmDspPowerDownTime)"!]
[!"num:i(DcmConfigSet/*[1]/DcmDsp/DcmDspPowerDownTime)"!]U
[!ELSE!][!//
0XFFU
[!ENDIF!][!//
[!ENDIF!][!//

#if (defined DCM_PAGEDBUFFER_ENABLED)   /* To prevent double declaration */
#error DCM_PAGEDBUFFER_ENABLED already defined
#endif /* if (defined DCM_PAGEDBUFFER_ENABLED) */

/** \brief Definition if paged buffer handling is enabled */
#define DCM_PAGEDBUFFER_ENABLED              [!//
[!IF "DcmConfigSet/*[1]/DcmPageBufferCfg/DcmPagedBufferEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DCM_PAGED_BUFFER_TIMEOUT) /* To prevent double declaration */
#error DCM_PAGED_BUFFER_TIMEOUT already defined
#endif /* if (defined DCM_PAGED_BUFFER_TIMEOUT) */

/** \brief Definition of Timeout for Paged buffer transmission.  This timeout
 ** is counted in units of DCM_TASK_TIME */
#define DCM_PAGED_BUFFER_TIMEOUT             [!//
[!IF "node:exists( DcmConfigSet/*[1]/DcmPageBufferCfg/DcmPagedBufferTimeout)"!][!//
[!"num:i( DcmConfigSet/*[1]/DcmPageBufferCfg/DcmPagedBufferTimeout div DcmConfigSet/*[1]/DcmGeneral/DcmTaskTime)"!]U
[!ELSE!][!//
0U
[!ENDIF!][!//

#if (defined DCM_DEV_ERROR_DETECT)
#error DCM_DEV_ERROR_DETECT already defined
#endif
/** \brief Switch, indicating if development error detection is activated for
 ** DCM */
#define DCM_DEV_ERROR_DETECT                 [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DCM_DIAG_SES_RESP_PEND_ENABLED )
#error DCM_DIAG_SES_RESP_PEND_ENABLED already defined
#endif /* #if (defined DCM_DIAG_SES_RESP_PEND_ENABLED ) */

/** \brief Enable if are any number of Pending responses provided per session */
#define DCM_DIAG_SES_RESP_PEND_ENABLED[!WS "6"!]
[!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspSession/*/DcmDspSessionRow/*/DcmDspDiagSesRespMaxNumRespPend)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DCM_DIAG_RESP_MAX_NUM_RESP_PEND )  /* To prevent double declaration */
#error DCM_DIAG_RESP_MAX_NUM_RESP_PEND already defined
#endif /* #if (defined DCM_DIAG_RESP_MAX_NUM_RESP_PEND ) */

/** \brief Maximum number of Pending responses possible */
#define DCM_DIAG_RESP_MAX_NUM_RESP_PEND      [!//
[!"num:i( DcmConfigSet/*[1]/DcmDsl/DcmDslDiagResp/DcmDslDiagRespMaxNumRespPend)"!]U

#if (defined DCM_DIAG_DSL_INFINITY_LIMIT_RESP_PEND ) /* To prevent double declaration */
#error DCM_DIAG_DSL_INFINITY_LIMIT_RESP_PEND already defined
#endif /* #if (defined DCM_DIAG_DSL_INFINITY_LIMIT_RESP_PEND) */
/** \brief Infinity limit (255 or 65535) for transmitting infinite ResponsePending NRC (0x78) */
#define DCM_DIAG_DSL_INFINITY_LIMIT_RESP_PEND      [!//
[!"num:i( DcmConfigSet/*[1]/DcmDsl/DcmDslDiagResp/DcmDslDiagRespMaxNumRespPendInfinityLimit)"!]U

#if (defined DCM_VERSION_INFO_API)
#error DCM_VERSION_INFO_API already defined
#endif
/** \brief Switch, indicating if Dcm_GetVersionInfo() API is activated for
 ** DCM */
#define DCM_VERSION_INFO_API                 [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DCM_INCLUDE_RTE)
#error DCM_INCLUDE_RTE already defined
#endif
/** \brief Switch, indicating if RTE is available and can be used from DCM */
#define DCM_INCLUDE_RTE                      [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmRteUsage = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DCM_RTETYPES_PROTECTION_AT_FILE_LEVEL)
#error DCM_RTETYPES_PROTECTION_AT_FILE_LEVEL already defined
#endif
/** \brief Switch, indicating if DCM uses the protection at file level */
#define DCM_RTETYPES_PROTECTION_AT_FILE_LEVEL         [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmRteTypesProtectionAtFileLevel = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DCM_SECURITYACCESS_STRICT)
#error DCM_SECURITYACCESS_STRICT already defined
#endif
/** \brief Switch, indicating whether the strict SecurityAccess mechanism is activated for the DCM */
#define DCM_SECURITYACCESS_STRICT            [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmRequestSeedAfterFailedCompareKey = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DCM_MODEDECLARATION_SUPPORT)
#error DCM_MODEDECLARATION_SUPPORT already defined
#endif
/** \brief Switch, indicating if Mode declaration support is enabled and SchM switch APIs
 ** are generated by RTE to be used for notifying mode switches to mode users */
#define DCM_MODEDECLARATION_SUPPORT          [!"$DCM_MODEDECLARATION_SUPPORT"!]

#if (defined DCM_MODE_DECLARATION_OEM_BL) /* To prevent double declaration */
#error DCM_MODE_DECLARATION_OEM_BL already defined
#endif /* if (defined DCM_MODE_DECLARATION_OEM_BL) */

/** \brief Indicates, if OEM boot support and mode declaration is enabled */
#define DCM_MODE_DECLARATION_OEM_BL           [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspSession/*/DcmDspSessionRow/*[DcmDspSessionForBoot = 'DCM_OEM_BOOT']) and (DcmConfigSet/*[1]/DcmGeneral/DcmModeDeclarationSupport = 'true')"!] STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_MODE_DECLARATION_SYS_BL) /* To prevent double declaration */
#error DCM_MODE_DECLARATION_SYS_BL already defined
#endif /* if (defined DCM_MODE_DECLARATION_SYS_BL) */

/** \brief Indicates, if SYS boot support and mode declaration is enabled */
#define DCM_MODE_DECLARATION_SYS_BL           [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspSession/*/DcmDspSessionRow/*[DcmDspSessionForBoot = 'DCM_SYS_BOOT']) and (DcmConfigSet/*[1]/DcmGeneral/DcmModeDeclarationSupport = 'true')"!] STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Switch, indicating availability of timing values in positive response of 0x10 and JumpFromBL */
#if (defined DCM_TIMING_VALUES_IN_POSITIVE_RESPONSE)
  #error "DCM_TIMING_VALUES_IN_POSITIVE_RESPONSE is already defined"
#endif
#define DCM_TIMING_VALUES_IN_POSITIVE_RESPONSE                      [!//
[!IF "DcmConfigSet/*[1]/DcmDsp/DcmDspSessionControlTiming = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DCM_ASYNC_SERVICE_HANDLER)
#error DCM_ASYNC_SERVICE_HANDLER already defined
#endif
/** \brief Switch, indicating if Async service handler is available */
#define DCM_ASYNC_SERVICE_HANDLER             [!//
[!IF "DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*/DcmAsyncServiceExecution ='true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DCM_ASYNC_DID_SERVICES)
#error DCM_ASYNC_DID_SERVICES already defined
#endif
/** \brief Switch, indicating if either of services 0x22, 0x2E or 0x2F use an Async service handler */
#define DCM_ASYNC_DID_SERVICES                [!//
[!IF "DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*/DcmAsyncServiceExecution[../DcmDsdSidTabServiceId=34 or ../DcmDsdSidTabServiceId=46  or ../DcmDsdSidTabServiceId=47] ='true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!/* readAccWithBlkIdInterface holds true if there is any DID with read access and atleast one
     signal of it with data access interface configured as block-id. This will be used for the
     generation of the macro DCM_READ_DID_BLOCK_ID_CONFIGURED */!][!//
[!VAR "readAccWithBlkIdInterface" = "'false'"!][!//
[!IF "$DCM_DSP_USE_SERVICE_0X22 = 'STD_ON'"!]
  [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[DcmDspDidUsed = 'true' and node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidRead]/DcmDspDidSignal/*[node:ref(DcmDspDidDataRef)/DcmDspDataUsePort = 'USE_BLOCK_ID']"!]
          [!VAR "readAccWithBlkIdInterface" = "'true'"!][!//
          [!BREAK!][!//
  [!ENDLOOP!][!//
[!ENDIF!][!//

#if (defined DCM_READ_DID_BLOCK_ID_CONFIGURED) /* To prevent double declaration */
#error DCM_READ_DID_BLOCK_ID_CONFIGURED already defined
#endif /* if (defined DCM_READ_DID_BLOCK_ID_CONFIGURED) */

/** \brief Indicates, if a DID is configured with read access and having signal with Data Access
 **  Interface as block-id */
#define DCM_READ_DID_BLOCK_ID_CONFIGURED           [!//
[!IF "($readAccWithBlkIdInterface)"!] STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_WRITE_DID_BLOCK_ID_CONFIGURED) /* To prevent double declaration */
#error DCM_WRITE_DID_BLOCK_ID_CONFIGURED already defined
#endif /* if (defined DCM_WRITE_DID_BLOCK_ID_CONFIGURED) */

[!/* writeAccWithBlkIdInterface holds true if there is any DID with write access and atleast one
     signal of it with data access interface configured as block-id. This will be used for the
     generation of the macro DCM_WRITE_DID_BLOCK_ID_CONFIGURED */!][!//
[!VAR "writeAccWithBlkIdInterface" = "'false'"!][!//
[!IF "$DCM_DSP_USE_SERVICE_0X2E = 'STD_ON'"!]
  [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[DcmDspDidUsed = 'true' and node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidWrite]/DcmDspDidSignal/*[node:ref(DcmDspDidDataRef)/DcmDspDataUsePort = 'USE_BLOCK_ID']"!]
          [!VAR "writeAccWithBlkIdInterface" = "'true'"!][!//
          [!BREAK!][!//
  [!ENDLOOP!][!//
[!ENDIF!][!//
[!//
/** \brief Indicates, if a DID is configured with write access and having signal with Data Access
 **  Interface as block-id */
#define DCM_WRITE_DID_BLOCK_ID_CONFIGURED           [!//
[!IF "($writeAccWithBlkIdInterface)"!] STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_DID_BLOCK_ID_CONFIGURED) /* To prevent double declaration */
#error DCM_DID_BLOCK_ID_CONFIGURED already defined
#endif /* if (defined DCM_DID_BLOCK_ID_CONFIGURED) */
/** \brief Indicates, if a DID is configured with read or write access and having signal with Data Access
 **  Interface as block-id */
#define DCM_DID_BLOCK_ID_CONFIGURED           [!//
[!IF "(($readAccWithBlkIdInterface = 'true') or ($writeAccWithBlkIdInterface = 'true'))"!] STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_NVM_USAGE_REQUIRED) /* To prevent double declaration */
#error DCM_NVM_USAGE_REQUIRED already defined
#endif /* if (defined DCM_NVM_USAGE_REQUIRED) */

/** \brief Indicates, if NvM.h is required */
#define DCM_NVM_USAGE_REQUIRED           [!//
[!IF "(($readAccWithBlkIdInterface = 'true') or ($writeAccWithBlkIdInterface = 'true') or (DcmConfigSet/*[1]/DcmGeneral/DcmDDDIDStorage = 'true') or (node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspRoe/DcmDspRoeBlockIdRef)) = 'true')"!] STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_JUMP_TO_BL_ENABLED) /* To prevent double declaration */
#error DCM_JUMP_TO_BL_ENABLED already defined
#endif /* if (defined DCM_JUMP_TO_BL_ENABLED) */

/** \brief Indicates, if SYS or OEM boot support is enabled */
#define DCM_JUMP_TO_BL_ENABLED           [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspSession/*/DcmDspSessionRow/*[DcmDspSessionForBoot = 'DCM_OEM_BOOT']) or node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspSession/*/DcmDspSessionRow/*[DcmDspSessionForBoot = 'DCM_SYS_BOOT'])"!] STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_ENDIANNESS_CONVERSION) /* To prevent double declaration */
#error DCM_ENDIANNESS_CONVERSION already defined
#endif /* if (defined DCM_ENDIANNESS_CONVERSION) */
/** \brief Indicates, if endianness conversion is enabled */
#define DCM_ENDIANNESS_CONVERSION           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*[DcmDslProtocolEndiannessConvEnabled = 'true'])"!] STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_DID_ENDIANNESS_CONVERSION) /* To prevent double declaration */
#error DCM_DID_ENDIANNESS_CONVERSION already defined
#endif /* if (defined DCM_DID_ENDIANNESS_CONVERSION) */
/** \brief Indicates if endianness conversion for DIDs is enabled */
#define DCM_DID_ENDIANNESS_CONVERSION           [!//
[!IF "(DcmConfigSet/*[1]/DcmGeneral/DcmDslProtocolDIDEndiannessConvEnabled = 'true')"!] STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_GET_SIZEOFEDRBYDTC_OPTIMIZATION)
#error DCM_GET_SIZEOFEDRBYDTC_OPTIMIZATION already defined
#endif
/** \brief Switch, for the optimized service handling of ReadDTCInformation(0x19),
 ** Subfunction 0x06 and 0x19.
 ** Handling of sub services 0x06 and 0x19 of service
 ** ReadDTCInformation (0x19) is optimized if Dem provides the size of UDS Extended data record
 ** number along with the extended data bytes size (according to AUTOSAR R4.1) */
#define DCM_GET_SIZEOFEDRBYDTC_OPTIMIZATION                 [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmGetSizeOfExtendedDataRecordByDTCOptimization  = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_RTE_TABLE_DID_SERVICES_SIZE)
#error DCM_RTE_TABLE_DID_SERVICES_SIZE already defined
#endif
/** \brief Did Services count */
#define DCM_RTE_TABLE_DID_SERVICES_SIZE   [!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*))"!]U

#if (defined DCM_CALLBACK_REQUEST_SIZE)
#error DCM_CALLBACK_REQUEST_SIZE already defined
#endif
/** \brief Callback Request count */
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmRteUsage = 'true'"!][!//
#define DCM_CALLBACK_REQUEST_SIZE   [!"num:i(count(DcmConfigSet/*[1]/DcmDsl/DcmDslCallbackDCMRequestService/*))"!]U
[!ELSE!][!//
#define DCM_CALLBACK_REQUEST_SIZE   0U
[!ENDIF!][!//

#if (defined DCM_TASK_TIME) /* To prevent double declaration */
#error DCM_TASK_TIME already defined
#endif /* if (defined DCM_TASK_TIME) */

/** \brief Definition for periodic cyclic task time in milliseconds */
#define DCM_TASK_TIME             [!//
[!"num:i(DcmConfigSet/*[1]/DcmGeneral/DcmTaskTime * 1000)"!]U

#if (defined DCM_DSP_DIDSERVICES_NUM_DIDS) /* To prevent double declaration */
#error DCM_DSP_DIDSERVICES_NUM_DIDS already defined
#endif /* if (defined DCM_DSP_DIDSERVICES_NUM_DIDS) */

/** \brief Number of enabled DIDs */
 /* !LINKSTO SWS_Dcm_01096,1 */
#define DCM_DSP_DIDSERVICES_NUM_DIDS          [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[DcmDspDidUsed = 'true']))"!]U

[!VAR "DCM_DSP_DIDSERVICES_NUM_DID_RANGES" = "num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspDidRange/*))"!]

#if (defined DCM_DSP_DIDSERVICES_NUM_DID_RANGES) /* To prevent double declaration */
#error DCM_DSP_DIDSERVICES_NUM_DID_RANGES already defined
#endif /* if (defined DCM_DSP_DIDSERVICES_NUM_DID_RANGES) */

/** \brief Number of DID ranges configured */
#define DCM_DSP_DIDSERVICES_NUM_DID_RANGES    [!"$DCM_DSP_DIDSERVICES_NUM_DID_RANGES"!]U

#if (defined DCM_DSP_USE_DIDRANGES) /* To prevent double declaration */
#error DCM_DSP_USE_DIDRANGES already defined
#endif /* if (defined DCM_DSP_USE_DIDRANGES) */

/** \brief STD_ON if at least one DID range is configured. STD_OFF otherwise */
#define DCM_DSP_USE_DIDRANGES                 [!//
[!IF "$DCM_DSP_DIDSERVICES_NUM_DID_RANGES > 0"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

[!/*---- Prepare the list of referenced DID Info containers  ----*/!][!//
[!VAR "refDidInfo" = "'#'"!][!//
[!VAR "DCM_NUM_DDDID_SRCTAB_ELEMENTS" = "'0'"!]
[!/*---- Loop through the enabled DIDs ----*/!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[DcmDspDidUsed = 'true']"!]
  [!IF "not(contains($refDidInfo, concat('#', node:pos(node:ref(DcmDspDidInfoRef)), '#')))"!]
    [!VAR "refDidInfo" = "concat($refDidInfo, node:pos(node:ref(DcmDspDidInfoRef)), '#')"!]
  [!ENDIF!]
  [!IF "(node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDDDIDMaxElements) and (node:ref(DcmDspDidInfoRef)/DcmDspDidDynamicallyDefined = 'true') )"!]
    [!VAR "DCM_NUM_DDDID_SRCTAB_ELEMENTS" = "num:i($DCM_NUM_DDDID_SRCTAB_ELEMENTS + node:ref(DcmDspDidInfoRef)/DcmDspDDDIDMaxElements)"!]
  [!ENDIF!]
[!ENDLOOP!]
[!/*---- Loop through the DID ranges ----*/!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspDidRange/*"!]
  [!IF "not(contains($refDidInfo, concat('#', node:pos(node:ref(DcmDspDidRangeInfoRef)), '#')))"!]
    [!VAR "refDidInfo" = "concat($refDidInfo, node:pos(node:ref(DcmDspDidRangeInfoRef)), '#')"!]
  [!ENDIF!]
[!ENDLOOP!]
[!//
#if (defined DCM_DSP_DIDSERVICES_NUM_DID_INFO) /* To prevent double declaration */
#error DCM_DSP_DIDSERVICES_NUM_DID_INFO already defined
#endif /* if (defined DCM_DSP_DIDSERVICES_NUM_DID_INFO) */

/** \brief Number of DID info containers configured */
#define DCM_DSP_DIDSERVICES_NUM_DID_INFO             [!//
[!"num:i(count(text:split($refDidInfo, '#')))"!]U

#if (defined DCM_DSP_DIDSERVICES_DIDRANGES_MAX_DATA_LENGTH) /* To prevent double declaration */
#error DCM_DSP_DIDSERVICES_DIDRANGES_MAX_DATA_LENGTH already defined
#endif /* if (defined DCM_DSP_DIDSERVICES_DIDRANGES_MAX_DATA_LENGTH) */

/** \brief The biggest maximum data length value of the configured DID ranges */
#define DCM_DSP_DIDSERVICES_DIDRANGES_MAX_DATA_LENGTH             [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspDidRange/*/DcmDspDidRangeMaxDataLength)"!][!//
[!"num:i(num:max(DcmConfigSet/*[1]/DcmDsp/DcmDspDidRange/*/DcmDspDidRangeMaxDataLength))"!]U
[!ELSE!][!//
0U
[!ENDIF!]

[!/*---- Calculate the array size for control/read/write/read scaling access functions ----*/!][!//
[!VAR "didControlAccessCount" = "0"!][!//
[!VAR "didReadAccessCount" = "0"!][!//
[!VAR "didWriteAccessCount" = "0"!][!//
[!VAR "didReadScalingAccessCount" = "0"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspDidInfo/*"!]
  [!IF "contains($refDidInfo, concat('#', node:pos(.), '#'))"!]
    [!IF "node:exists(DcmDspDidAccess/DcmDspDidControl)"!]
      [!VAR "didControlAccessCount" = "$didControlAccessCount + 1"!][!//
    [!ENDIF!]
    [!IF "node:exists(DcmDspDidAccess/DcmDspDidRead)"!]
      [!VAR "didReadAccessCount" = "$didReadAccessCount + 1"!][!//
    [!ENDIF!]
    [!IF "node:exists(DcmDspDidAccess/DcmDspDidWrite)"!]
      [!VAR "didWriteAccessCount" = "$didWriteAccessCount + 1"!][!//
    [!ENDIF!]
    [!IF "node:exists(DcmDspDidAccess/DcmDspDidReadScalingData)"!]
      [!VAR "didReadScalingAccessCount" = "$didReadScalingAccessCount + 1"!][!//
    [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!]
[!//
#if (defined DCM_DSP_DIDSERVICES_NUM_DID_READACCESS_INFO) /* To prevent double declaration */
#error DCM_DSP_DIDSERVICES_NUM_DID_READACCESS_INFO already defined
#endif /* if (defined DCM_DSP_DIDSERVICES_NUM_DID_READACCESS_INFO) */

/** \brief Number of DID read access info structures configured */
#define DCM_DSP_DIDSERVICES_NUM_DID_READACCESS_INFO             [!//
[!"num:i($didReadAccessCount)"!]U

#if (defined DCM_DSP_DIDSERVICES_NUM_DID_WRITEACCESS_INFO) /* To prevent double declaration */
#error DCM_DSP_DIDSERVICES_NUM_DID_WRITEACCESS_INFO already defined
#endif /* if (defined DCM_DSP_DIDSERVICES_NUM_DID_WRITEACCESS_INFO) */

/** \brief Number of DID write access info structures configured */
#define DCM_DSP_DIDSERVICES_NUM_DID_WRITEACCESS_INFO             [!//
[!"num:i($didWriteAccessCount)"!]U

#if (defined DCM_DSP_DIDSERVICES_NUM_DID_IOCTLACCESS_INFO) /* To prevent double declaration */
#error DCM_DSP_DIDSERVICES_NUM_DID_IOCTLACCESS_INFO already defined
#endif /* if (defined DCM_DSP_DIDSERVICES_NUM_DID_IOCTLACCESS_INFO) */

/** \brief Number of DID control access info structures configured */
#define DCM_DSP_DIDSERVICES_NUM_DID_IOCTLACCESS_INFO             [!//
[!"num:i($didControlAccessCount)"!]U

#if (defined DCM_DSP_DIDSERVICES_NUM_DID_READSCALINGACCESS_INFO) /* To prevent double declaration */
#error DCM_DSP_DIDSERVICES_NUM_DID_READSCALINGACCESS_INFO already defined
#endif /* if (defined DCM_DSP_DIDSERVICES_NUM_DID_READSCALINGACCESS_INFO) */

/** \brief Number of DID read scaling access info structures configured */
#define DCM_DSP_DIDSERVICES_NUM_DID_READSCALINGACCESS_INFO             [!//
[!"num:i($didReadScalingAccessCount)"!]U

/** \brief Return control to ECU is to be defined and executed only if both 0x2F
 ** service is enabled and if at least one DID list is configured with control access */
#if (defined DCM_RETURNCONTROLTOECU_ENABLED)
#error DCM_RETURNCONTROLTOECU_ENABLED already defined
#endif /* #if (defined  DCM_RETURNCONTROLTOECU_ENABLED) */

[!VAR "DcmRoutineDefaultInterface" = "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspDidEnableDefaultInterfaces"!][!//
#define DCM_RETURNCONTROLTOECU_ENABLED     [!//
[!IF "($DCM_DSP_USE_SERVICE_0X2F = 'STD_ON') and ((num:i($didControlAccessCount) > 0) or ($DcmRoutineDefaultInterface = 'true'))"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DCM_DSP_PRECONF_ADDRESS_LENGTH_FORMATIDENTIFIER)
#error DCM_DSP_PRECONF_ADDRESS_LENGTH_FORMATIDENTIFIER already defined
#endif /* if (defined DCM_DSP_PRECONF_ADDRESS_LENGTH_FORMATIDENTIFIER) */

/** \brief Indicates AddressAndLengthFormatIdentifier is configured or not */
#define DCM_DSP_PRECONF_ADDRESS_LENGTH_FORMATIDENTIFIER             [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspMemory/DcmDspAddressAndLengthFormatIdentifier)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspMemory/DcmDspAddressAndLengthFormatIdentifier)"!][!//
#if (defined DCM_NUM_ADDR_LENGTH_FORMAT_IDENTIFIER)
#error DCM_NUM_ADDR_LENGTH_FORMAT_IDENTIFIER already defined
#endif /* if (defined DCM_NUM_ADDR_LENGTH_FORMAT_IDENTIFIER) */

/** \brief number of AddressAndLengthFormatIdentifiers supported */
#define DCM_NUM_ADDR_LENGTH_FORMAT_IDENTIFIER [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspMemory/DcmDspAddressAndLengthFormatIdentifier/DcmDspSupportedAddressAndLengthFormatIdentifier/*))"!]U
[!ENDIF!]

#if (defined DCM_NUM_MEMORY_IDINFO)
#error DCM_NUM_MEMORY_IDINFO defined
#endif /* if (defined DCM_NUM_MEMORY_IDINFO) */

/** \brief Number of MemoryIdInfos configured */
#define DCM_NUM_MEMORY_IDINFO                 [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspMemory/DcmDspMemoryIdInfo/*))"!]U

#if (defined DCM_USE_MEMORY_ID)
#error DCM_USE_MEMORY_ID defined
#endif /* if (defined DCM_USE_MEMORY_ID) */

[!VAR "PreviousMaxWriteRanges" = "0"!]
[!VAR "PreviousMaxReadRanges" = "0"!]
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspMemory/DcmDspMemoryIdInfo/*"!]
  [!VAR "CurrentMaxWriteRanges" = "num:i(num:i(count(./DcmDspWriteMemoryRangeInfo/*)) + num:i(count(./DcmDspWriteMemoryRangeByLabelInfo/*)))"!]
  [!IF "$CurrentMaxWriteRanges > $PreviousMaxWriteRanges"!]
    [!VAR "PreviousMaxWriteRanges" = "$CurrentMaxWriteRanges"!][!//
  [!ENDIF!]
  [!VAR "CurrentMaxReadRanges" = "num:i(num:i(count(./DcmDspReadMemoryRangeInfo/*)) + num:i(count(./DcmDspReadMemoryRangeByLabelInfo/*)))"!]
  [!IF "$CurrentMaxReadRanges > $PreviousMaxReadRanges"!]
    [!VAR "PreviousMaxReadRanges" = "$CurrentMaxReadRanges"!][!//
  [!ENDIF!]
[!ENDLOOP!]
#if (defined DCM_MAX_NUM_READ_RANGES)
#error DCM_MAX_NUM_READ_RANGES defined
#endif /* if (defined DCM_MAX_NUM_READ_RANGES) */

/** \brief Max Number of Read Ranges configured on a memory table*/
#define DCM_MAX_NUM_READ_RANGES                 [!//
[!"num:i($PreviousMaxReadRanges)"!]U

#if (defined DCM_MAX_NUM_WRITE_RANGES)
#error DCM_MAX_NUM_WRITE_RANGES defined
#endif /* if (defined DCM_MAX_NUM_WRITE_RANGES) */

/** \brief Max Number of Read Ranges configured on a memory table*/
#define DCM_MAX_NUM_WRITE_RANGES                 [!//
[!"num:i($PreviousMaxWriteRanges)"!]U

/** \brief Defines whether Dcm shall use the MemoryIdentifier parameter */
#define DCM_USE_MEMORY_ID   [!//
[!IF "(DcmConfigSet/*[1]/DcmDsp/DcmDspMemory/DcmDspUseMemoryId = 'true')"!]TRUE[!ELSE!]FALSE[!ENDIF!]

#if (defined DCM_NUM_DID_DATA) /* To prevent double declaration */
#error DCM_NUM_DID_DATA already defined
#endif /* if (defined DCM_NUM_DID_DATA) */

[!IF "$DCM_DSP_USE_SERVICE_0X24 = 'STD_ON'"!][!//
/** \brief Number of DID Datas that have read scaling configured */
#define DCM_NUM_READSCALING_DID_DATA                 [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspData/*[node:ref(DcmDspDataInfoRef)/DcmDspDataScalingInfoSize]))"!]U
[!ENDIF!]

/** \brief Number of DID Datas configured */
#define DCM_NUM_DID_DATA                      [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspData/*))"!]U

#if (defined DCM_DID_REF_COUNT) /* To prevent double declaration */
#error DCM_DID_REF_COUNT already defined
#endif /* if (defined DCM_DID_REF_COUNT) */

/** \brief Total number of references of each DID */
#define DCM_DID_REF_COUNT                     [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[DcmDspDidUsed = 'true']/DcmDspDidRef/*))"!]U

#if (defined DCM_DID_SIGNALS_COUNT) /* To prevent double declaration */
#error DCM_DID_SIGNALS_COUNT already defined
#endif /* if (defined DCM_DID_SIGNALS_COUNT) */

/** \brief Total number of signals of each DID */
#define DCM_DID_SIGNALS_COUNT                 [!"$DCM_DID_SIGNALS_COUNT"!]U

#if (defined DCM_READ_DID_MAX) /* To prevent double declaration */
#error DCM_READ_DID_MAX already defined
#endif /* if (defined DCM_READ_DID_MAX) */

[!INDENT "0"!]
[!VAR "maxsize" = "0"!][!//
[!VAR "maxsizeRef" = "0"!][!//
[!VAR "maxsizeDid" = "0"!][!//
[!/* Check if PIDs are configured */!][!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspPid/*) or node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspVehInfo)"!]
  [!VAR "maxsize" = "4"!]
[!ENDIF!]
[!/* Check if MIDs are configured */!][!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspTestResultByObdmid/DcmDspTestResultObdmidTid)"!]
  [!VAR "maxsize" = "8"!]
[!ENDIF!]
[!/* First we loop through all static DIDs which are configured*/!][!//
[!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:ref(./DcmDspDidInfoRef)/DcmDspDidDynamicallyDefined = 'false']"!][!//
  [!VAR "maxsizeDid" = "0"!][!//
  [!LOOP " DcmDspDidSignal/*"!][!//
    [!VAR "CurrentSize" = "(num:i(./DcmDspDidDataPos) + num:i((as:ref(./DcmDspDidDataRef))/DcmDspDataSize))"!][!//
    [!IF "$maxsizeDid < $CurrentSize"!][!//
      [!VAR "maxsizeDid" = "$CurrentSize"!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
  [!/* For the current DID, this is the maximum size, in bytes*/!][!//
  [!VAR "maxsizeDid" = "num:i(($maxsizeDid + 7) div 8)"!][!//
  [!VAR "maxsizeRef" = "0"!][!//
  [!/* We then loop through the current DID's referenced DIDs */!][!//
  [!LOOP " DcmDspDidRef/*"!][!//
    [!VAR "maxsizeSingleRef" = "0"!][!//
    [!LOOP "node:ref(.)/DcmDspDidSignal/*"!][!//
      [!IF "$maxsizeSingleRef < (num:i(./DcmDspDidDataPos) + num:i((as:ref(./DcmDspDidDataRef))/DcmDspDataSize))"!][!//
        [!VAR "maxsizeSingleRef" = "(num:i(./DcmDspDidDataPos) + num:i((as:ref(./DcmDspDidDataRef))/DcmDspDataSize))"!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
    [!/* Similarly, we determine for each referenced DID its maximum possible size. Be aware that since
       * referenced DIDs are written along with their IDs, we add a further of 2 bytes to the maximum
       * determined DID size */!][!//
    [!VAR "maxsizeSingleRef" = "num:i(num:i(($maxsizeSingleRef + 7) div 8) + 2)"!][!//
    [!/* And make a sum of all these sizes, so that we know the maximum number of bytes obtained from referenced DIDs. */!][!//
    [!VAR "maxsizeRef" = "num:i($maxsizeRef + $maxsizeSingleRef)"!][!//
  [!ENDLOOP!][!//
  [!/* If for the current DID, we determine that the sum of the DID's maximum data length and the sum
     * of its references' maximum data lengths is greater than the previous one's, we've determined a
     * new maximum length */!][!//
  [!IF "$maxsizeDid + $maxsizeRef > $maxsize "!][!//
    [!VAR "maxsize" = "$maxsizeDid + $maxsizeRef"!][!//
  [!ENDIF!][!//
  [!IF "$maxsize >= 4294967295"!]
    [!ERROR!][!//
      DID [!"node:name(.)"!] is too large to fit within any buffer available in the module.[!//
    [!ENDERROR!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
/** \brief Size of the biggest DID configured including OBD MIDs/PIDs */
#if (defined DCM_DID_MAX_SIZE)
  #error "DCM_DID_MAX_SIZE is already defined"
#endif
#define DCM_DID_MAX_SIZE                      [!//
[!/* If no DID contains data, the maximum size must be at least 1 (array size of 0 not allowed). */!][!//
[!IF "$maxsize > 0"!][!//
  [!"num:i($maxsize)"!]U
[!ELSE!][!//
  1U
[!ENDIF!][!//
[!ENDINDENT!]

/** \brief Maximum number of Dids that can be requested in a single read */
#define DCM_READ_DID_MAX                      [!//
[!"num:i(DcmConfigSet/*[1]/DcmDsp/DcmDspMaxDidToRead)"!]U

#if (defined DCM_DID_DATA_TABLE_SIZE) /* To prevent double declaration */
#error DCM_DID_DATA_TABLE_SIZE already defined
#endif /* if (defined DCM_DID_DATA_TABLE_SIZE) */

[!INDENT "0"!][!//
[!VAR "DidDataArraySize" = "0"!][!//
[!VAR "SRInterfacesEnabled" = "'FALSE'"!][!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspData/*) and $DCM_DSP_USE_DIDSERVICES = 'STD_ON'"!][!//
  [!VAR "DcmDspDataReadAccessList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidRead)]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
  [!VAR "DcmDspDataWriteAccessList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidWrite)]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
  [!VAR "DcmDspDataControlAccessList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl) and ((as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidResetToDefault = 'true') or (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidFreezeCurrentState = 'true') or  (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidReturnControlToEcu = 'true') or (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidShortTermAdjustment = 'true'))]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
  [!VAR "DcmDspDataFreezeCurrentStateList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl) and (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidFreezeCurrentState = 'true')]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
  [!VAR "DcmDspDataResetToDefaultList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl) and (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidResetToDefault = 'true')]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
  [!VAR "DcmDspDataShortTermAdjList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl) and (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidShortTermAdjustment = 'true')]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
  [!VAR "DcmDspDataReadScalingDataAccessList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidReadScalingData)]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
  [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspData/*"!][!//
    [!VAR "ReadAccess" = "'FALSE'"!][!//
    [!VAR "WriteAccess" = "'FALSE'"!][!//
    [!VAR "ControlAccess" = "'FALSE'"!][!//
    [!VAR "ReadScalingDataAccess" = "'FALSE'"!][!//
    [!VAR "CurrentNode_aspath" = "as:path(.,true())"!][!//
    [!/* Check that DspData element is refered in a Did, which has the read access*/!][!//
    [!IF "text:match($DcmDspDataReadAccessList,'ASPath')"!][!//
      [!IF "text:contains(text:split($DcmDspDataReadAccessList), $CurrentNode_aspath)"!][!//
        [!VAR "ReadAccess" = "'TRUE'"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!/* Check that DspData element is refered in a Did, which has the write access*/!][!//
    [!IF "text:match($DcmDspDataWriteAccessList,'ASPath')"!][!//
      [!IF "text:contains(text:split($DcmDspDataWriteAccessList), $CurrentNode_aspath)"!][!//
        [!VAR "WriteAccess" = "'TRUE'"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!/* Check that DspData element is refered in a Did, which has the Control access*/!][!//
    [!IF "text:match($DcmDspDataControlAccessList,'ASPath')"!][!//
      [!IF "text:contains(text:split($DcmDspDataControlAccessList), $CurrentNode_aspath)"!][!//
        [!VAR "ControlAccess" = "'TRUE'"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!/* Check that DspData element is refered in a Did, which has the ReadScalingData access*/!][!//
    [!IF "text:match($DcmDspDataReadScalingDataAccessList,'ASPath')"!][!//
      [!IF "text:contains(text:split($DcmDspDataReadScalingDataAccessList), $CurrentNode_aspath)"!][!//
        [!VAR "ReadScalingDataAccess" = "'TRUE'"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!IF "(DcmDspDataUsePort = 'USE_DATA_SENDER_RECEIVER') or (DcmDspDataUsePort = 'USE_DATA_SENDER_RECEIVER_AS_SERVICE')"!][!//
      [!IF "$ReadAccess = 'TRUE'"!][!//
         [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
         [!VAR "SRInterfacesEnabled" = "'TRUE'"!][!//
      [!ENDIF!][!//
      [!IF "$WriteAccess = 'TRUE'"!][!//
         [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
         [!VAR "SRInterfacesEnabled" = "'TRUE'"!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
      [!/*---ConditionCheckRead------*/!][!//
      [!IF "(DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC') or (DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC')"!][!//
        [!IF "node:exists(DcmDspDataConditionCheckReadFnc) and DcmDspDataConditionCheckReadFncUsed = 'true'"!][!//
          [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
        [!ENDIF!][!//
      [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_SYNCH_CLIENT_SERVER') and (($ReadAccess = 'TRUE') or ($ControlAccess = 'TRUE')) and (DcmDspDataConditionCheckReadFncUsed = 'true')"!][!//
        [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
      [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_ASYNCH_CLIENT_SERVER') and (($ReadAccess = 'TRUE') or ($ControlAccess = 'TRUE')) and (DcmDspDataConditionCheckReadFncUsed = 'true')"!][!//
        [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
      [!ENDIF!][!//
      [!/*---ReadDataLength------*/!][!//
      [!IF "(DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC') or (DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC')"!][!//
        [!IF "node:exists(DcmDspDataReadDataLengthFnc)"!][!//
          [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
        [!ENDIF!][!//
      [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_SYNCH_CLIENT_SERVER') and (node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'false') and (($ReadAccess = 'TRUE') or ($ControlAccess = 'TRUE'))"!][!//
        [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
      [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_ASYNCH_CLIENT_SERVER') and (node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'false') and (($ReadAccess = 'TRUE') or ($ControlAccess = 'TRUE'))"!][!//
        [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
      [!ENDIF!][!//
      [!/*---ReadDataFnc------*/!][!//
      [!IF "DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC'"!][!//
        [!IF "node:exists(DcmDspDataReadFnc)"!][!//
          [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
        [!ENDIF!][!//
      [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_SYNCH_CLIENT_SERVER') and (($ReadAccess = 'TRUE') or ($ControlAccess = 'TRUE'))"!][!//
        [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!]
      [!ENDIF!][!//
      [!/*---CallReadData ------*/!][!//
      [!IF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
        [!IF "node:exists(DcmDspDataReadFnc)"!][!//
          [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
        [!ENDIF!][!//
      [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_ASYNCH_CLIENT_SERVER') and (($ReadAccess = 'TRUE') or ($ControlAccess = 'TRUE'))"!][!//
        [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
      [!ENDIF!][!//
      [!/*---WriteFixedData ------*/!][!//
      [!IF "DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC'"!][!//
        [!IF "node:exists(DcmDspDataWriteFnc)"!][!//
          [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
        [!ENDIF!][!//
      [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_SYNCH_CLIENT_SERVER') and ($WriteAccess = 'TRUE')"!][!//
        [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
      [!ENDIF!][!//
      [!/*---CallWriteFixedLength, CallWriteVarLength , ResultWriteData------*/!][!//
      [!IF "(DcmDspDataUsePort = 'USE_DATA_ASYNCH_CLIENT_SERVER')  and ($WriteAccess = 'TRUE')"!][!//
        [!IF "node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'true'"!][!//
          [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
        [!ELSE!][!//
          [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
        [!ENDIF!][!//
        [!/*--- Rte_Result_DataServices_[!"node:name(.)"!]_WriteData, -----*/!][!//
      [!ELSEIF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
        [!IF "node:exists(DcmDspDataWriteFnc)"!][!//
          [!IF "node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'true'"!][!//
            [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
          [!ELSE!][!//
            [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
      [!IF "$DCM_DSP_USE_SERVICE_0X24 = 'STD_ON'"!][!//
        [!IF "(DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC') or (DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC')"!][!//
          [!IF "(node:exists(DcmDspDataGetScalingInfoFnc))"!][!//
            [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
        [!IF "(((DcmDspDataUsePort = 'USE_DATA_SYNCH_CLIENT_SERVER') or (DcmDspDataUsePort = 'USE_DATA_ASYNCH_CLIENT_SERVER')) and ($ReadScalingDataAccess = 'TRUE'))"!][!//
          [!IF "node:exists(node:ref(DcmDspDataInfoRef)/DcmDspDataScalingInfoSize)"!][!//
            [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
      [!IF "$DCM_DSP_USE_SERVICE_0X2F = 'STD_ON'"!][!//
        [!VAR "DataUsePort" = "DcmDspDataUsePort"!][!//
        [!IF "text:match($DcmDspDataFreezeCurrentStateList,'ASPath')"!][!//
          [!IF "text:contains(text:split($DcmDspDataFreezeCurrentStateList), $CurrentNode_aspath)"!][!//
            [!IF "contains($DataUsePort, '_FNC') or contains($DataUsePort, '_CLIENT_SERVER')"!][!//
              [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
            [!ENDIF!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
        [!IF "text:match($DcmDspDataShortTermAdjList,'ASPath')"!][!//
          [!IF "text:contains(text:split($DcmDspDataShortTermAdjList), $CurrentNode_aspath)"!][!//
            [!IF "contains($DataUsePort, '_FNC') or contains($DataUsePort, '_CLIENT_SERVER')"!][!//
              [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
            [!ENDIF!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
        [!IF "text:match($DcmDspDataResetToDefaultList,'ASPath')"!][!//
          [!IF "text:contains(text:split($DcmDspDataResetToDefaultList), $CurrentNode_aspath)"!][!//
            [!IF "contains($DataUsePort, '_FNC') or contains($DataUsePort, '_CLIENT_SERVER')"!][!//
              [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
            [!ENDIF!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
        [!IF "text:match($DcmDspDataControlAccessList,'ASPath')"!][!//
          [!IF "text:contains(text:split($DcmDspDataControlAccessList), $CurrentNode_aspath)"!][!//
            [!IF "contains($DataUsePort, '_FNC') or contains($DataUsePort, '_CLIENT_SERVER')"!][!//
              [!VAR "DidDataArraySize" = "$DidDataArraySize + 1"!][!//
            [!ENDIF!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
[!ENDIF!][!//
[!ENDINDENT!][!//
/** \brief Size of Dcm_DidDataTable */
#define DCM_DID_DATA_TABLE_SIZE               [!//
[!"num:i($DidDataArraySize)"!]U

#if (defined DCM_DSP_USE_SENDERRECEIVER_INTERFACES) /* To prevent double declaration */
#error DCM_DSP_USE_SENDERRECEIVER_INTERFACES already defined
#endif /* if (defined DCM_DSP_USE_SENDERRECEIVER_INTERFACES) */

/** \brief Indicates if any DID which has read, write or control access has a DcmDspData with
  * DcmDspDataUsePort equal to USE_DATA_SENDER_RECEIVER or USE_DATA_SENDER_RECEIVER_AS_SERVICE */
#define DCM_DSP_USE_SENDERRECEIVER_INTERFACES                 [!//
[!IF "$SRInterfacesEnabled = 'TRUE'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//

/** \brief Maximum allowed periodicDIDs which can be read in a single "ReadDataByPeriodicIdentifier" request */
#if (defined DCM_MAX_PERIODIC_DID_READ)
  #error "DCM_MAX_PERIODIC_DID_READ is already defined"
#endif
#define DCM_MAX_PERIODIC_DID_READ             [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspMaxPeriodicDidToRead)"!]
  [!"num:i(DcmConfigSet/*[1]/DcmDsp/DcmDspMaxPeriodicDidToRead)"!]U
[!ELSE!][!//
  0U
[!ENDIF!]

/** \brief Maximum number of periodicDataIdentifiers that can be scheduled concurrently */
#if (defined DCM_MAX_PERIODIC_DID_SCHEDULER)
  #error "DCM_MAX_PERIODIC_DID_SCHEDULER is already defined"
#endif
#define DCM_MAX_PERIODIC_DID_SCHEDULER        [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspPeriodicDidTransmission/DcmDspMaxPeriodicDidScheduler)"!]
  [!"num:i(DcmConfigSet/*[1]/DcmDsp/DcmDspPeriodicDidTransmission/DcmDspMaxPeriodicDidScheduler)"!]U
[!ELSE!][!//
  0U
[!ENDIF!]

/** \brief Size of a single sampling bufer to which periodic DIDs are sampled */
#if (defined DCM_PERIODIC_DID_SAMPLING_BUFFER_SIZE)
  #error "DCM_PERIODIC_DID_SAMPLING_BUFFER_SIZE is already defined"
#endif
#define DCM_PERIODIC_DID_SAMPLING_BUFFER_SIZE [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspPeriodicDidTransmission/DcmDspPeriodicDidSamplingBufferSize)"!]
  [!"num:i(DcmConfigSet/*[1]/DcmDsp/DcmDspPeriodicDidTransmission/DcmDspPeriodicDidSamplingBufferSize)"!]U
[!ELSE!][!//
  0U
[!ENDIF!]

/** \brief Number of individual sampling buffers in the periodic DID transmission ring */
#if (defined DCM_PERIODIC_DID_TRANSMISSION_RING_ENTRIES)
  #error "DCM_PERIODIC_DID_TRANSMISSION_RING_ENTRIES is already defined"
#endif
#define DCM_PERIODIC_DID_TRANSMISSION_RING_ENTRIES    [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspPeriodicDidTransmission/DcmDspPeriodicDidTransmissionRingEntries)"!]
  [!"num:i(DcmConfigSet/*[1]/DcmDsp/DcmDspPeriodicDidTransmission/DcmDspPeriodicDidTransmissionRingEntries)"!]U
[!ELSE!][!//
  0U
[!ENDIF!]

/** \brief Maximum data length of periodic DDDID */
#if (defined DCM_PERIODIC_DDDID_MAX_LENGTH)
  #error "DCM_PERIODIC_DDDID_MAX_LENGTH is already defined"
#endif
#define DCM_PERIODIC_DDDID_MAX_LENGTH    [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspPeriodicDidTransmission/DcmDspPeriodicDDDIDMaxLength)"!]
  [!"num:i(DcmConfigSet/*[1]/DcmDsp/DcmDspPeriodicDidTransmission/DcmDspPeriodicDDDIDMaxLength)"!]U
[!ELSE!][!//
  0U
[!ENDIF!]

/* !LINKSTO Dcm.PeriodicDid.Dsn.GenTrRate,1 */

/** \brief Slow transmission rate for periodicDataIdentifiers */
#if (defined DCM_PERIODIC_TRANSMISSION_SLOW_RATE)
  #error "DCM_PERIODIC_TRANSMISSION_SLOW_RATE is already defined"
#endif
#define DCM_PERIODIC_TRANSMISSION_SLOW_RATE    [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspPeriodicTransmission/DcmDspPeriodicTransmissionSlowRate)"!]
  [!"num:i(DcmConfigSet/*[1]/DcmDsp/DcmDspPeriodicTransmission/DcmDspPeriodicTransmissionSlowRate div DcmConfigSet/*[1]/DcmGeneral/DcmTaskTime)"!]U
[!ELSE!][!//
  0U
[!ENDIF!]

/** \brief Medium transmission rate for periodicDataIdentifiers */
#if (defined DCM_PERIODIC_TRANSMISSION_MEDIUM_RATE)
  #error "DCM_PERIODIC_TRANSMISSION_MEDIUM_RATE is already defined"
#endif
#define DCM_PERIODIC_TRANSMISSION_MEDIUM_RATE    [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspPeriodicTransmission/DcmDspPeriodicTransmissionMediumRate)"!]
  [!"num:i(DcmConfigSet/*[1]/DcmDsp/DcmDspPeriodicTransmission/DcmDspPeriodicTransmissionMediumRate div DcmConfigSet/*[1]/DcmGeneral/DcmTaskTime)"!]U
[!ELSE!][!//
  0U
[!ENDIF!]

/** \brief Fast transmission rate for periodicDataIdentifiers */
#if (defined DCM_PERIODIC_TRANSMISSION_FAST_RATE)
  #error "DCM_PERIODIC_TRANSMISSION_FAST_RATE is already defined"
#endif
#define DCM_PERIODIC_TRANSMISSION_FAST_RATE    [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspPeriodicTransmission/DcmDspPeriodicTransmissionFastRate)"!]
  [!"num:i(DcmConfigSet/*[1]/DcmDsp/DcmDspPeriodicTransmission/DcmDspPeriodicTransmissionFastRate div DcmConfigSet/*[1]/DcmGeneral/DcmTaskTime)"!]U
[!ELSE!][!//
  0U
[!ENDIF!]



/* A uint32 buffer size is considered here since a union is created between arrays of all DID data types
 * to ensure 32 bit alignment for the signals buffer for safe casting from uint8* to 16/32*
 * The respective cast is needed for sender/receiver interfaces which require pointers to int16/32 as
 * parameters */
/** \brief Maximum size that a DID signal can occupy in a uint32 array */
[!VAR "BufMaxSize" = "0"!][!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspData/*)"!][!//
  [!IF "DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslProtocolEndiannessConvEnabled ='true'"!][!//
    [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspData/*"!]
      [!VAR "CurrentSize" = "DcmDspDataSize"!][!//
      [!VAR "WordSize" = "2"!][!//
      [!IF "((DcmDspDataType = 'SINT32') or (DcmDspDataType = 'UINT32') or (DcmDspDataType = 'SINT32_N') or (DcmDspDataType = 'UINT32_N'))"!][!//
        [!VAR "WordSize" = "4"!][!//
      [!ENDIF!]
      [!VAR "Bytes" = "$WordSize"!][!//
      [!IF "((num:i(($CurrentSize + 7) div 8)) mod $WordSize) > 0"!][!//
        [!VAR "Bytes" = "$Bytes - ((num:i(($CurrentSize + 7) div 8)) mod $WordSize)"!][!//
      [!ELSE!][!//
        [!VAR "Bytes" = "0"!][!//
      [!ENDIF!][!//
      [!VAR "TempSize" = "num:i((($CurrentSize + 7) div 8) + $Bytes)"!][!//
      [!IF "$TempSize > $BufMaxSize"!][!//
        [!VAR "BufMaxSize" = "$TempSize"!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!ELSE!][!//
    [!VAR "DcmDspDataMax" = "0"!][!//
    [!VAR "WordSize" = "1"!][!//
    [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspData/*"!]
      [!IF "DcmDspDataSize > $DcmDspDataMax"!][!//
        [!VAR "DcmDspDataMax" = "DcmDspDataSize"!][!//
        [!IF "contains(DcmDspDataType, '32')"!]
          [!VAR "WordSize" = "4"!][!//
        [!ELSEIF "contains(DcmDspDataType, '16')"!][!//
          [!VAR "WordSize" = "2"!][!//
        [!ENDIF!][!//
        [!VAR "BitLength" = "$WordSize*8"!][!//
        [!VAR "BufMaxSize" = "num:i((($DcmDspDataMax + $BitLength - 1) div $BitLength) * $WordSize)"!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!ENDIF!][!//
[!ENDIF!][!//

/** \brief Maximum size that a DID data defined as range can occupy in a uint32 array*/
[!VAR "BufRangeMaxSize" = "0"!][!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspDidRange/*/DcmDspDidRangeMaxDataLength)"!][!//
  [!VAR "BufRangeMaxSize" = "num:i(num:max(DcmConfigSet/*[1]/DcmDsp/DcmDspDidRange/*/DcmDspDidRangeMaxDataLength))"!][!//
[!ENDIF!][!//

/** \brief The maximum size that a ReadScaling DID needs. */
[!VAR "BufScalingMaxSize" = "0"!][!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspDataInfo/*/DcmDspDataScalingInfoSize)"!][!//
  [!VAR "BufScalingMaxSize"="num:max(DcmConfigSet/*[1]/DcmDsp/DcmDspDataInfo/*/DcmDspDataScalingInfoSize)"!]
[!ENDIF!][!//

[!IF "$BufRangeMaxSize > $BufMaxSize"!][!//
  [!VAR "BufMaxSize" = "$BufRangeMaxSize"!][!//
[!ENDIF!]

[!IF "$BufScalingMaxSize > $BufMaxSize"!][!//
  [!VAR "BufMaxSize" = "$BufScalingMaxSize"!][!//
[!ENDIF!]

[!/* BufMaxSize needs to be always different than zero, otherwise will have arrays with length zero */!][!//
[!VAR "BufMaxSize" = "(number($BufMaxSize = 0) * 1) + (number($BufMaxSize != 0) * $BufMaxSize)"!][!//

#if (defined DCM_DATA_MAX_SIZE_DWORD_ARRAY) /* To prevent double declaration */
#error DCM_DATA_MAX_SIZE_DWORD_ARRAY already defined
#endif /* if (defined DCM_DATA_MAX_SIZE_DWORD_ARRAY) */

#define DCM_DATA_MAX_SIZE_DWORD_ARRAY   [!"num:i((($BufMaxSize * 8) + 31) div 32)"!]U

/* Even if the value of this macro is not generated it is strictly dependent on a macro generated
 * above and has a similar meaning so for better readability/code understanding it is placed here */
#if (defined DCM_DATA_MAX_SIZE_WORD_ARRAY) /* To prevent double declaration */
#error DCM_DATA_MAX_SIZE_WORD_ARRAY already defined
#endif /* if (defined DCM_DATA_MAX_SIZE_WORD_ARRAY) */

#define DCM_DATA_MAX_SIZE_WORD_ARRAY    (2U * DCM_DATA_MAX_SIZE_DWORD_ARRAY)

/* Even if the value of this macro is not generated it is strictly dependent on a macro generated
 * above and has a similar meaning so for better readability/code understanding it is placed here
 * If the max length of DID data defined by ranges is bigger then max length of DID data
 * defined by signals then the buffer size need to be updated. */
#if (defined DCM_DATA_MAX_SIZE_BYTE_ARRAY) /* To prevent double declaration */
#error DCM_DATA_MAX_SIZE_BYTE_ARRAY already defined
#endif /* if (defined DCM_DATA_MAX_SIZE_BYTE_ARRAY) */

#define DCM_DATA_MAX_SIZE_BYTE_ARRAY    (4U * DCM_DATA_MAX_SIZE_DWORD_ARRAY)



#if (defined DCM_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED)
#error DCM_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED already defined
#endif
/** \brief Switch, indicating if Service request manufacturer notification is available */
#define DCM_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED       [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmRequestManufacturerNotificationEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DCM_REQUEST_SUPPLIER_NOTIFICATION_ENABLED)
#error DCM_REQUEST_SUPPLIER_NOTIFICATION_ENABLED already defined
#endif
/** \brief Switch, indicating if Service request supplier notification is available */
#define DCM_REQUEST_SUPPLIER_NOTIFICATION_ENABLED       [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmRequestSupplierNotificationEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DCM_RTE_TABLE_REQUEST_MANUFACTURER_SIZE)
#error DCM_RTE_TABLE_REQUEST_MANUFACTURER_SIZE already defined
#endif
/** \brief Service request manufacturer notification count */
#define DCM_RTE_TABLE_REQUEST_MANUFACTURER_SIZE   [!"num:i(count(DcmConfigSet/*[1]/DcmDsl/DcmDslServiceRequestManufacturerNotification/*))"!]U

#if (defined DCM_RTE_TABLE_REQUEST_SUPPLIER_SIZE)
#error DCM_RTE_TABLE_REQUEST_SUPPLIER_SIZE already defined
#endif
/** \brief Service request supplier notification count */
#define DCM_RTE_TABLE_REQUEST_SUPPLIER_SIZE   [!"num:i(count(DcmConfigSet/*[1]/DcmDsl/DcmDslServiceRequestSupplierNotification/*))"!]U

[!VAR "CommnBuff" = "'STD_OFF'"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*"!][!//
  [!VAR "TxBuff" = "node:value(DcmDslProtocolTxBufferID)"!][!//
  [!LOOP "DcmDslConnection/*/DcmDslProtocolRx/*"!][!//
    [!IF "$TxBuff = node:value(DcmDslProtocolRxBufferID)"!][!//
      [!VAR "CommnBuff" = "'STD_ON'"!][!//
      [!BREAK!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
[!ENDLOOP!][!//
[!//
/** \brief Indicates, if application will be notified of transmission confirmation */
#if (defined DCM_APPL_TXCONFIRMATION_NOTIFICATION) /* To prevent double declaration */
#error DCM_APPL_TXCONFIRMATION_NOTIFICATION already defined
#endif /* if defined DCM_APPL_TXCONFIRMATION_NOTIFICATION */
#define DCM_APPL_TXCONFIRMATION_NOTIFICATION                      [!//
[!IF "DcmConfigSet/*[1]/DcmDsl/DcmDslEnableTxConfirmationNotification = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro to check that if common buffer is used for Rx and Tx */
#if (defined DCM_COMMON_BUFF_CONFIGURED)
  #error "DCM_COMMON_BUFF_CONFIGURED is already defined"
#endif
#define DCM_COMMON_BUFF_CONFIGURED [!"$CommnBuff"!]

[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmModeDeclarationSupport = 'true'"!][!//
  [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspSession/*/DcmDspSessionRow/*"!][!//
    [!IF "./DcmDspSessionLevel = 1"!][!//
#if (defined DCM_RTE_DEFAULTSESSION_MODE) /* To prevent double declaration */
#error DCM_RTE_DEFAULTSESSION_MODE already defined
#endif
/** \brief  Definition if DCM_DEFAULT_SESSION has another name */
#define DCM_RTE_DEFAULTSESSION_MODE   RTE_MODE_DcmDiagnosticSessionControl_[!"./@name"!]

    [!ENDIF!][!//
  [!ENDLOOP!][!//
[!ENDIF!][!//
[!//


/** \brief Switch that enables/disables BusyRepeatRequest responses */
#if (defined DCM_RESPOND_WITH_BUSYREPEATREQUEST)
#error DCM_RESPOND_WITH_BUSYREPEATREQUEST already defined
#endif
/** \brief Switch, indicating if a BusyRepeastRequest response is send or not */
#define DCM_RESPOND_WITH_BUSYREPEATREQUEST       [!//
[!IF "DcmConfigSet/*[1]/DcmDsl/DcmDslDiagResp/DcmDslDiagRespOnSecondDeclinedRequest = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (DCM_INCLUDE_RTE == STD_OFF)

/*------------------[definitions of security levels]----------------------------------------------*/

[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspSecurity/DcmDspSecurityRow/*"!][!//
#if (defined DcmConf_[!"name(../../.)"!]_[!"./@name"!])
#error DcmConf_[!"name(../../.)"!]_[!"./@name"!] already defined
#endif
/** \brief Export symbolic name value */
#define DcmConf_[!"name(../../.)"!]_[!"@name"!] [!"num:i(./DcmDspSecurityLevel)"!]U

#if (defined DCM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined Dcm_[!"name(.)"!])
#error Dcm_[!"name(.)"!] already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 **        (AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Dcm_[!"@name"!] [!"num:i(./DcmDspSecurityLevel)"!]U
#endif /* defined DCM_PROVIDE_LEGACY_SYMBOLIC_NAMES */

[!ENDLOOP!][!//
[!//
#endif /* if (DCM_INCLUDE_RTE == STD_OFF) */

#if (defined DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS) /* To prevent double declaration */
#error DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS already defined
#endif /* if (defined DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS) */

/** \brief Indicates that the active diagnosis functionality was enabled */
#define DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS           [!//
[!IF "(DcmConfigSet/*[1]/DcmGeneral/DcmDslEnableSetActiveDiagnosis = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_COMM_MULTICORE_ENABLED) /* To prevent double declaration */
#error DCM_COMM_MULTICORE_ENABLED already defined
#endif /* if (defined DCM_COMM_MULTICORE_ENABLED) */

/** \brief Indicates that Dcm ComM inter-partition communication is enabled */
#define DCM_COMM_MULTICORE_ENABLED           [!//
[!IF "(DcmConfigSet/*[1]/DcmGeneral/DcmComMMultiCoreSupport = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_DDDID_PERSISTENCE) /* To prevent double declaration */
#error DCM_DDDID_PERSISTENCE already defined
#endif /* if (defined DCM_DDDID_PERSISTENCE) */

/** \brief Indicates if the DDDID persistence feature support is enabled */
#define DCM_DDDID_PERSISTENCE           [!//
[!IF "(DcmConfigSet/*[1]/DcmGeneral/DcmDDDIDStorage = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_DDDID_CHECK_SOURCES) /* To prevent double declaration */
#error DCM_DDDID_CHECK_SOURCES already defined
#endif /* if (defined DCM_DDDID_CHECK_SOURCES) */

/** \brief If the DDDID sources must be checked each time they are read */
#define DCM_DDDID_CHECK_SOURCES           [!//
[!IF "(DcmConfigSet/*[1]/DcmDsp/DcmDspDDDIDcheckPerSourceDID = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_NUM_DDDID_SRCTAB_ELEMENTS)
#error DCM_NUM_DDDID_SRCTAB_ELEMENTS already defined
#endif /* if (defined DCM_NUM_DDDID_SRCTAB_ELEMENTS) */

/** \brief Number of pre configured DDDID sources */
#define DCM_NUM_DDDID_SRCTAB_ELEMENTS   [!"$DCM_NUM_DDDID_SRCTAB_ELEMENTS"!]U

#if (defined DCM_DDDID_NVM_BLOCK_ID) /* To prevent double declaration */
#error DCM_DDDID_NVM_BLOCK_ID already defined
#endif /* if (defined DCM_DDDID_NVM_BLOCK_ID) */

/* !LINKSTO Dcm.EB.DcmDspDDDIDBlockIdRef,2 */
/** \brief Block Id of NvM to store DDDID Data for persistence */
#define DCM_DDDID_NVM_BLOCK_ID           [!//
[!IF "(DcmConfigSet/*[1]/DcmGeneral/DcmDDDIDStorage = 'true')"!][!//
[!"node:ref(DcmConfigSet/*[1]/DcmGeneral/DcmDspDDDIDBlockIdRef)/NvMNvramBlockIdentifier"!]U[!ELSE!]0xFFU[!ENDIF!]

[!/*------------------------------------------------------------------*/!][!//
[!/*------------------- ROE 4.2 START --------------------------------*/!][!//

/* !LINKSTO Dcm.ROE.Enabled,3 */
/* DcmDslResponseOnEvent is verified in xdm */
#if (defined DCM_DSP_USE_ROESERVICES)
  #error "DCM_DSP_USE_ROESERVICES is already defined"
#endif
#define DCM_DSP_USE_ROESERVICES       [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspRoe)"!][!//
STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspRoe)"!]

/** \brief Block Id of NvM to store ROE Data for persistence */
#if (defined DCM_ROE_NVM_BLOCK_ID)
  #error "DCM_ROE_NVM_BLOCK_ID is already defined"
#endif
#define DCM_ROE_NVM_BLOCK_ID           [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspRoe/DcmDspRoeBlockIdRef)"!][!//
[!"node:ref(DcmConfigSet/*[1]/DcmDsp/DcmDspRoe/DcmDspRoeBlockIdRef)/NvMNvramBlockIdentifier"!]U[!ELSE!]0xFFU[!ENDIF!]


/** \brief The minimum MainFunction cycles between two consecutive ROE transmissions */
#if (defined DCM_ROE_INTER_MSG_CYCLE_COUNT)
  #error "DCM_ROE_INTER_MSG_CYCLE_COUNT is already defined"
#endif
#define DCM_ROE_INTER_MSG_CYCLE_COUNT  [!//
[!"num:i(DcmConfigSet/*[1]/DcmDsp/DcmDspRoe/DcmDspRoeInterMessageTime div DcmConfigSet/*[1]/DcmGeneral/DcmTaskTime)"!]U

/** \brief Number of pre configured events */
#if (defined DCM_ROE_NUM_PRECONF_EVENT)
  #error "DCM_ROE_NUM_PRECONF_EVENT is already defined"
#endif
#define DCM_ROE_NUM_PRECONF_EVENT   [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspRoe/DcmDspRoeEvent/*))"!]U

/** \brief Number of pre configured eventWindowTimes */
#if (defined DCM_ROE_NUM_PRECONF_EVENT_WINDOW_TIMES)
  #error "DCM_ROE_NUM_PRECONF_EVENT_WINDOW_TIMES is already defined"
#endif
#define DCM_ROE_NUM_PRECONF_EVENT_WINDOW_TIMES   [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspRoe/DcmDspRoeEventWindowTime/*))"!]U

/** \brief If the ROE persistence feature support is enabled */
#if (defined DCM_ROE_PERSISTENCE)
  #error "DCM_ROE_PERSISTENCE is already defined"
#endif
#define DCM_ROE_PERSISTENCE           [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspRoe/DcmDspRoeBlockIdRef)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspRoe/DcmDspRoeEvent/*"!]
[!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(..)"!][!//
/* !LINKSTO ecuc_sws_2108,1 */
/** \brief Export Symbolic Name value for parameter: DcmDspRoeEventId */
#define DcmConf_[!"$SymbolName"!]   [!"DcmDspRoeEventId"!]U

[!ENDLOOP!]
[!ELSE!]
/* Define used for prototype generation in Dcm.h even if ROE is not present */
#define DCM_ROE_NUM_PRECONF_EVENT   0U
[!ENDIF!]

#if (defined DCM_ROE_NUM_ONCHANGEOFDATAIDENTIFIER_EVENTS) /* To prevent double declaration */
#error DCM_ROE_NUM_ONCHANGEOFDATAIDENTIFIER_EVENTS already defined
#endif /* if (defined DCM_ROE_NUM_ONCHANGEOFDATAIDENTIFIER_EVENTS) */

/** \brief Number of available OnChangeOfDataIdentifier ROE events */
#define DCM_ROE_NUM_ONCHANGEOFDATAIDENTIFIER_EVENTS [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspRoe/DcmDspRoeEvent/*[DcmDspRoeEventProperties = 'DcmDspRoeOnChangeOfDataIdentifier']))"!]U

#if (defined DCM_ROE_NUM_ROEONDTCSTATUSCHANGE) /* To prevent double declaration */
#error DCM_ROE_NUM_ROEONDTCSTATUSCHANGE already defined
#endif /* if (defined DCM_ROE_NUM_ROEONDTCSTATUSCHANGE) */

/** \brief Number of available RoeOnDTCStatusChange ROE events */
#define DCM_ROE_NUM_ROEONDTCSTATUSCHANGE   [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspRoe/DcmDspRoeEvent/*[DcmDspRoeEventProperties = 'DcmDspRoeOnDTCStatusChange']))"!]U

#if (defined DCM_HAS_ROE_TYPE2) /* To prevent double declaration */
#error DCM_HAS_ROE_TYPE2 already defined
#endif /* if (defined DCM_HAS_ROE_TYPE2) */

/** \brief Indicates if TYPE2 ROE are enabled */
#define DCM_HAS_ROE_TYPE2           [!//
[!IF "$DCM_DSP_USE_SERVICE_0X86 = 'STD_ON'
      and
        node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslMainConnection')
      and
        node:refvalid(./DcmDslROEConnectionRef)
      and
        node:refvalid(node:ref(DcmDslROEConnectionRef)/DcmDslRoeTxPduRef)
      and
        node:exists(node:ref(DcmDslROEConnectionRef)/DcmDslRoeTxConfirmationPduId)
        ])"!] STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!/*------------------- ROE 4.2 END ----------------------------------*/!][!//
[!/*------------------------------------------------------------------*/!][!//

#if (defined DCM_MAX_CONSEC_GET_EXT_DATA_REC) /* To prevent double declaration */
#error DCM_MAX_CONSEC_GET_EXT_DATA_REC already defined
#endif /* if (defined DCM_MAX_CONSEC_GET_EXT_DATA_REC) */

/** \brief The maximum number of consecutive Get Extended Data Record executions for service 0xAF */
#define DCM_MAX_CONSEC_GET_EXT_DATA_REC   [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspReadGenericInformation/DcmDspMaxDemGetDiagnosticDataConsecutiveExecutions)"!][!//
[!"num:i(DcmConfigSet/*[1]/DcmDsp/DcmDspReadGenericInformation/DcmDspMaxDemGetDiagnosticDataConsecutiveExecutions)"!]U[!ELSE!]0U[!ENDIF!]

#if (defined DCM_HAS_PERIODIC_RESPONSE_TYPE2) /* To prevent double declaration */
#error DCM_HAS_PERIODIC_RESPONSE_TYPE2 already defined
#endif /* if (defined DCM_HAS_PERIODIC_RESPONSE_TYPE2) */

/** \brief Indicates if TYPE2 periodic responses are enabled */
#define DCM_HAS_PERIODIC_RESPONSE_TYPE2           [!//
[!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslMainConnection')
                          and
                           node:refexists(./DcmDslPeriodicTransmissionConRef)
                          and
                           node:refvalid(./DcmDslPeriodicTransmissionConRef)
                          and
                           node:exists(node:ref(DcmDslPeriodicTransmissionConRef)/DcmDslPeriodicConnection/*)
                          ])"!] STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/*------------------[Services configured]--------------------------------------------------------*/

#if (defined DCM_DSP_USE_SERVICE_0XAF ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0XAF already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0XAF) */

#define DCM_DSP_USE_SERVICE_0XAF           [!"$DCM_DSP_USE_SERVICE_0XAF"!]

#if (defined DCM_DSP_USE_SERVICE_0X3E ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X3E already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X3E) */

#define DCM_DSP_USE_SERVICE_0X3E           [!"$DCM_DSP_USE_SERVICE_0X3E"!]

#if (defined DCM_DSP_USE_SERVICE_0X14 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X14 already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X14) */

#define DCM_DSP_USE_SERVICE_0X14           [!"$DCM_DSP_USE_SERVICE_0X14"!]

#if (defined DCM_DSP_USE_SERVICE_0X19 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X19 already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X19) */

#define DCM_DSP_USE_SERVICE_0X19           [!"$DCM_DSP_USE_SERVICE_0X19"!]

#if (defined DCM_DSP_USE_SERVICE_0X27 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X27 already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X27) */

#define DCM_DSP_USE_SERVICE_0X27           [!"$DCM_DSP_USE_SERVICE_0X27"!]

#if (defined DCM_DSP_USE_SERVICE_0X10 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X10  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X10 ) */

#define DCM_DSP_USE_SERVICE_0X10           [!"$DCM_DSP_USE_SERVICE_0X10"!]

#if (defined DCM_DSP_USE_SERVICE_0X22 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X22  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X22 ) */

#define DCM_DSP_USE_SERVICE_0X22           [!"$DCM_DSP_USE_SERVICE_0X22"!]

#if (defined DCM_DSP_SERVICE_0X22_ASYNC ) /* To prevent double declaration */
#error DCM_DSP_SERVICE_0X22_ASYNC  already defined
#endif /* if (defined DCM_DSP_SERVICE_0X22_ASYNC ) */

#define DCM_DSP_SERVICE_0X22_ASYNC         [!"$DCM_DSP_SERVICE_0X22_ASYNC"!]

#if (defined DCM_DSP_USE_SERVICE_0X24 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X24  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X24 ) */

#define DCM_DSP_USE_SERVICE_0X24           [!"$DCM_DSP_USE_SERVICE_0X24"!]

#if (defined DCM_DSP_USE_SERVICE_0X2A ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X2A  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X2A ) */

#define DCM_DSP_USE_SERVICE_0X2A           [!"$DCM_DSP_USE_SERVICE_0X2A"!]

#if (defined DCM_DSP_USE_SERVICE_0X23 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X23  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X23 ) */

#define DCM_DSP_USE_SERVICE_0X23           [!"$DCM_DSP_USE_SERVICE_0X23"!]

#if (defined DCM_DSP_USE_SERVICE_0X3D ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X3D  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X3D ) */

#define DCM_DSP_USE_SERVICE_0X3D           [!"$DCM_DSP_USE_SERVICE_0X3D"!]

#if (defined DCM_DSP_USE_SERVICE_0X2E ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X2E  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X2E ) */

#define DCM_DSP_USE_SERVICE_0X2E           [!"$DCM_DSP_USE_SERVICE_0X2E"!]

#if (defined DCM_DSP_SERVICE_0X2E_ASYNC ) /* To prevent double declaration */
#error DCM_DSP_SERVICE_0X2E_ASYNC  already defined
#endif /* if (defined DCM_DSP_SERVICE_0X2E_ASYNC ) */

#define DCM_DSP_SERVICE_0X2E_ASYNC          [!"$DCM_DSP_SERVICE_0X2E_ASYNC"!]

#if (defined DCM_DSP_USE_SERVICE_0X2F ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X2F  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X2F ) */

#define DCM_DSP_USE_SERVICE_0X2F           [!"$DCM_DSP_USE_SERVICE_0X2F"!]

#if (defined DCM_DSP_SERVICE_0X2F_ASYNC ) /* To prevent double declaration */
#error DCM_DSP_SERVICE_0X2F_ASYNC  already defined
#endif /* if (defined DCM_DSP_SERVICE_0X2F_ASYNC ) */

#define DCM_DSP_SERVICE_0X2F_ASYNC         [!"$DCM_DSP_SERVICE_0X2F_ASYNC"!]

#if (defined DCM_DSP_USE_SERVICE_0X31 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X31  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X31 ) */

#define DCM_DSP_USE_SERVICE_0X31           [!"$DCM_DSP_USE_SERVICE_0X31"!]

#if (defined DCM_DSP_USE_SERVICE_0X11 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X11  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X11 ) */

#define DCM_DSP_USE_SERVICE_0X11           [!"$DCM_DSP_USE_SERVICE_0X11"!]

#if (defined DCM_DSP_USE_SERVICE_0X34 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X34  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X34 ) */

#define DCM_DSP_USE_SERVICE_0X34           [!"$DCM_DSP_USE_SERVICE_0X34"!]

#if (defined DCM_DSP_USE_SERVICE_0X35 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X35  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X35 ) */

#define DCM_DSP_USE_SERVICE_0X35           [!"$DCM_DSP_USE_SERVICE_0X35"!]

#if (defined DCM_DSP_USE_SERVICE_0X36 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X36  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X36 ) */

#define DCM_DSP_USE_SERVICE_0X36           [!"$DCM_DSP_USE_SERVICE_0X36"!]

#if (defined DCM_DSP_USE_SERVICE_0X37 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X37  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X37 ) */

#define DCM_DSP_USE_SERVICE_0X37           [!"$DCM_DSP_USE_SERVICE_0X37"!]

#if (defined DCM_DSP_USE_SERVICE_0X28 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X28  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X28 ) */

#define DCM_DSP_USE_SERVICE_0X28           [!"$DCM_DSP_USE_SERVICE_0X28"!]

#if (defined DCM_DSP_USE_SERVICE_0X2C) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X2C  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X2C) */

#define DCM_DSP_USE_SERVICE_0X2C           [!"$DCM_DSP_USE_SERVICE_0X2C"!]

#if (defined DCM_DSP_USE_SERVICE_0X87 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X87  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X87 ) */

#define DCM_DSP_USE_SERVICE_0X87           [!"$DCM_DSP_USE_SERVICE_0X87"!]

#if (defined DCM_DSP_USE_SERVICE_0X86 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X86  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X86 ) */

#define DCM_DSP_USE_SERVICE_0X86           [!"$DCM_DSP_USE_SERVICE_0X86"!]

#if (defined DCM_DSP_USE_SERVICE_0X85 ) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X85  already defined
#endif /* if (defined DCM_DSP_USE_SERVICE_0X85 ) */

#define DCM_DSP_USE_SERVICE_0X85           [!"$DCM_DSP_USE_SERVICE_0X85"!]

/* begin define macros for OBD services */
#if (defined DCM_DSP_USE_OBDSERVICES) /* To prevent double declaration */
#error DCM_DSP_USE_OBDSERVICES  already defined
#endif /* #if(defined DCM_DSP_USE_OBDSERVICES) */

#define DCM_DSP_USE_OBDSERVICES            [!//
[!IF "($DCM_DSP_USE_SERVICE_0X01 = 'STD_ON') or ($DCM_DSP_USE_SERVICE_0X02 = 'STD_ON') or ($DCM_DSP_USE_SERVICE_0X03 = 'STD_ON') or ($DCM_DSP_USE_SERVICE_0X04 = 'STD_ON') or ($DCM_DSP_USE_SERVICE_0X06 = 'STD_ON') or ($DCM_DSP_USE_SERVICE_0X07 = 'STD_ON') or ($DCM_DSP_USE_SERVICE_0X08 = 'STD_ON') or ($DCM_DSP_USE_SERVICE_0X09 = 'STD_ON') or ($DCM_DSP_USE_SERVICE_0X0A = 'STD_ON')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* begin define macros for OBD data services (0x01, 0x02, 0x06, 0x08, 0x09) */
#if (defined DCM_DSP_USE_OBDDATASERVICES) /* To prevent double declaration */
#error DCM_DSP_USE_OBDDATASERVICES  already defined
#endif /* #if(defined DCM_DSP_USE_OBDDATASERVICES) */

#define DCM_DSP_USE_OBDDATASERVICES        [!//
[!IF "($DCM_DSP_USE_SERVICE_0X01 = 'STD_ON') or ($DCM_DSP_USE_SERVICE_0X02 = 'STD_ON') or ($DCM_DSP_USE_SERVICE_0X06 = 'STD_ON') or ($DCM_DSP_USE_SERVICE_0X08 = 'STD_ON') or ($DCM_DSP_USE_SERVICE_0X09 = 'STD_ON') or (node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspPid/*))"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_DSP_USE_SERVICE_0X01) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X01  already defined
#endif /* #if(defined DCM_DSP_USE_SERVICE_0X01) */

#define DCM_DSP_USE_SERVICE_0X01           [!"$DCM_DSP_USE_SERVICE_0X01"!]

#if (defined DCM_DSP_USE_SERVICE_0X02) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X02  already defined
#endif /* #if(defined DCM_DSP_USE_SERVICE_0X02) */

#define DCM_DSP_USE_SERVICE_0X02           [!"$DCM_DSP_USE_SERVICE_0X02"!]

#if (defined DCM_DSP_USE_SERVICE_0X03) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X03  already defined
#endif /* #if(defined DCM_DSP_USE_SERVICE_0X03) */

#define DCM_DSP_USE_SERVICE_0X03           [!"$DCM_DSP_USE_SERVICE_0X03"!]

#if (defined DCM_DSP_USE_SERVICE_0X04) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X04  already defined
#endif /* #if(defined DCM_DSP_USE_SERVICE_0X04) */

#define DCM_DSP_USE_SERVICE_0X04           [!"$DCM_DSP_USE_SERVICE_0X04"!]

#if (defined DCM_DSP_USE_SERVICE_0X06) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X06  already defined
#endif /* #if(defined DCM_DSP_USE_SERVICE_0X06) */

#define DCM_DSP_USE_SERVICE_0X06           [!"$DCM_DSP_USE_SERVICE_0X06"!]

#if (defined DCM_DSP_SERVICE_0X06_ASYNC ) /* To prevent double declaration */
#error DCM_DSP_SERVICE_0X06_ASYNC  already defined
#endif /* if (defined DCM_DSP_SERVICE_0X06_ASYNC ) */

#define DCM_DSP_SERVICE_0X06_ASYNC         [!"$DCM_DSP_SERVICE_0X06_ASYNC"!]

#if (defined DCM_DSP_USE_SERVICE_0X07) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X07  already defined
#endif /* #if(defined DCM_DSP_USE_SERVICE_0X07) */

#define DCM_DSP_USE_SERVICE_0X07           [!"$DCM_DSP_USE_SERVICE_0X07"!]

#if (defined DCM_DSP_USE_SERVICE_0X08) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X08  already defined
#endif /* #if(defined DCM_DSP_USE_SERVICE_0X08) */

#define DCM_DSP_USE_SERVICE_0X08           [!"$DCM_DSP_USE_SERVICE_0X08"!]

#if (defined DCM_DSP_USE_SERVICE_0X0A) /* To prevent double declaration */
#error DCM_DSP_USE_SERVICE_0X0A  already defined
#endif /* #if(defined DCM_DSP_USE_SERVICE_0X0A) */

#define DCM_DSP_USE_SERVICE_0X0A           [!"$DCM_DSP_USE_SERVICE_0X0A"!]

#if (defined DCM_DSP_USE_SERVICE_0X09)
  #error "DCM_DSP_USE_SERVICE_0X09 is already defined"
#endif
#define DCM_DSP_USE_SERVICE_0X09           [!"$DCM_DSP_USE_SERVICE_0X09"!]
/* end define macros for OBD services */

#if (defined DCM_EXTSVC_ENABLED ) /* To prevent double declaration */
#error DCM_EXTSVC_ENABLED  already defined
#endif /* if (defined DCM_EXTSVC_ENABLED ) */

/** \brief External handler is configured for any service or not */
#define DCM_EXTSVC_ENABLED           [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*/DcmDsdSidTabFnc)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined DCM_DSP_USE_DIDSERVICES ) /* To prevent double declaration */
#error DCM_DSP_USE_DIDSERVICES  already defined
#endif /* if (defined DCM_DSP_USE_DIDSERVICES ) */

/** \brief Indicates if there are internal service handlers enabled that need DID services */
#define DCM_DSP_USE_DIDSERVICES            [!"$DCM_DSP_USE_DIDSERVICES"!]

#if (defined DCM_DSP_USE_MEMORYSERVICES ) /* To prevent double declaration */
#error DCM_DSP_USE_MEMORYSERVICES  already defined
#endif /* if (defined DCM_DSP_USE_MEMORYSERVICES ) */

/** \brief Indicates if there are internal service handlers enabled that need DID services */
#define DCM_DSP_USE_MEMORYSERVICES         [!"$DCM_DSP_USE_MEMORYSERVICES"!]

#if (defined DCM_DSP_USE_DATATRANSFERSERVICES ) /* To prevent double declaration */
#error DCM_DSP_USE_DATATRANSFERSERVICES  already defined
#endif /* if (defined DCM_DSP_USE_DATATRANSFERSERVICES ) */

/** \brief Indicates if there are internal service handlers enabled that need DID services */
#define DCM_DSP_USE_DATATRANSFERSERVICES   [!"$DCM_DSP_USE_DATATRANSFERSERVICES"!]

#if (defined DCM_DSP_USE_DEMFACADE ) /* To prevent double declaration */
#error DCM_DSP_USE_DEMFACADE  already defined
#endif /* if (defined DCM_DSP_USE_DEMFACADE ) */

/** \brief Indicates if there are internal service handlers enabled that need DID services */
#define DCM_DSP_USE_DEMFACADE            [!"$DCM_DSP_USE_DEMFACADE"!]

#if (defined DCM_USE_API_CALLER_GENERIC_SVCH_IMPLEMENTATION) /* To prevent double declaration */
#error DCM_USE_API_CALLER_GENERIC_SVCH_IMPLEMENTATION  already defined
#endif /* if (defined DCM_USE_API_CALLER_GENERIC_SVCH_IMPLEMENTATION ) */

/** \brief Indicates if there are external service handlers enabled, that also have generic internal service handler enabled */
#define DCM_USE_API_CALLER_GENERIC_SVCH_IMPLEMENTATION [!WS '3'!][!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!][!//

/** \brief This is a Vendor Specific parameter of the Dcm.
  *        Defines whether the DCM Module shall call Application default interfaces for each unsupported (unconfigured or unused)
  *        DID (Service 0x22 - ReadDataByIdentifier, Service 0x2E - WriteDataByIdentifier, Service 0x2F - InputOutputControlByIdentifier).
  *        - TRUE: DIDs which are not configured are considered supported and they are handled by the supplier.
  *        - FALSE: DIDs which are not configured are considered not supported. */

 [!VAR "DcmRoutineDefaultInterface" = "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspDidEnableDefaultInterfaces"!][!//
 #define DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ [!WS '3'!][!//
 [!IF "($DcmRoutineDefaultInterface = 'false')"!]
 STD_OFF
 [!ELSE!][!//
 STD_ON
 [!ENDIF!][!//

/*------------------[Sub Services configured ]----------------------------------------------------*/

#if (defined DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x01 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x01  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x01 ) */

/** \brief Subservice 0x01 for 0x10 enabled or not */
#define DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x01           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='16']/*/*[DcmDsdSubServiceId='1' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x02 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x02  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x02 ) */

/** \brief Subservice 0x02 for 0x10 enabled or not */
#define DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x02           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='16']/*/*[DcmDsdSubServiceId='2' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x03 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x03  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x03 ) */

/** \brief Subservice 0x03 for 0x10 enabled or not */
#define DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x03           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='16']/*/*[DcmDsdSubServiceId='3' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x2B ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x2B  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x2B ) */

/** \brief Subservice 0x2B for 0x10 enabled or not */
#define DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x2B           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='16']/*/*[DcmDsdSubServiceId='43' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x2C ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x2C  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x2C ) */

/** \brief Subservice 0x2C for 0x10 enabled or not */
#define DCM_DSP_SUBSERVICE_DIAGNOSTICSESSIONCONTROL_0x2C           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='16']/*/*[DcmDsdSubServiceId='44' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_HARDRESET_0x01 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_HARDRESET_0x01  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_HARDRESET_0x01 ) */

/** \brief Subservice 0x01 for 0x11 enabled or not */
#define DCM_DSP_SUBSERVICE_HARDRESET_0x01           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='17']/*/*[DcmDsdSubServiceId='1' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_KEYOFFONRESET_0x02 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_KEYOFFONRESET_0x02  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_KEYOFFONRESET_0x02 ) */

/** \brief Subservice 0x02 for 0x11 enabled or not */
#define DCM_DSP_SUBSERVICE_KEYOFFONRESET_0x02           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='17']/*/*[DcmDsdSubServiceId='2' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_SOFTRESET_0x03 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_SOFTRESET_0x03  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_SOFTRESET_0x03 ) */

/** \brief Subservice 0x03 for 0x11 enabled or not */
#define DCM_DSP_SUBSERVICE_SOFTRESET_0x03           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='17']/*/*[DcmDsdSubServiceId='3' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_ENABLERAPIDPOWERSHUTDOWN_0x04 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_ENABLERAPIDPOWERSHUTDOWN_0x04  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_ENABLERAPIDPOWERSHUTDOWN_0x04 ) */

/** \brief Subservice 0x04 for 0x11 enabled or not */
#define DCM_DSP_SUBSERVICE_ENABLERAPIDPOWERSHUTDOWN_0x04           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='17']/*/*[DcmDsdSubServiceId='4' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_DISABLERAPIDPOWERSHUTDOWN_0x05 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_DISABLERAPIDPOWERSHUTDOWN_0x05  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_DISABLERAPIDPOWERSHUTDOWN_0x05 ) */

/** \brief Subservice 0x05 for 0x11 enabled or not */
#define DCM_DSP_SUBSERVICE_DISABLERAPIDPOWERSHUTDOWN_0x05           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='17']/*/*[DcmDsdSubServiceId='5' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_0X2C_0X01_SSVC_ENABLED) /* To prevent double declaration */
#error DCM_0X2C_0X01_SSVC_ENABLED already defined
#endif /* if (defined DCM_0X2C_0X01_SSVC_ENABLED) */

/** \brief Subservice 0x01 for 0x2C enabled or not */
#define DCM_0X2C_0X01_SSVC_ENABLED           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='44']/*/*[DcmDsdSubServiceId='1' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_0X2C_0X02_SSVC_ENABLED) /* To prevent double declaration */
#error DCM_0X2C_0X02_SSVC_ENABLED already defined
#endif /* if (defined DCM_0X2C_0X02_SSVC_ENABLED) */

/** \brief Subservice 0x02 for 0x2C enabled or not */
#define DCM_0X2C_0X02_SSVC_ENABLED           [!"$DCM_DSP_SUBSERVICE_DDDID_0x02"!]

#if (defined DCM_0X2C_0X03_SSVC_ENABLED) /* To prevent double declaration */
#error DCM_0X2C_0X03_SSVC_ENABLED already defined
#endif /* if (defined DCM_0X2C_0X03_SSVC_ENABLED) */

/** \brief Subservice 0x03 for 0x2C enabled or not */
#define DCM_0X2C_0X03_SSVC_ENABLED           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='44']/*/*[DcmDsdSubServiceId='3' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x01 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x01  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x01 ) */

/** \brief Subservice 0x01 for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x01           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='1' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x02 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x02  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x02 ) */

/** \brief Subservice 0x02 for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x02           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='2' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 ) */

/** \brief Subservice 0x03 for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='3' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x04 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x04  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x04 ) */

/** \brief Subservice 0x04 for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x04           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25' or DcmDsdSidTabServiceId='175']/*/*[DcmDsdSubServiceId='4' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x06 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x06  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x06 ) */

/** \brief Subservice 0x06 for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x06           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25' or DcmDsdSidTabServiceId='175']/*/*[DcmDsdSubServiceId='6' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 ) */

/** \brief Subservice 0x07 for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='7' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 ) */

/** \brief Subservice 0x08 for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='8' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 ) */

/** \brief Subservice 0x09 for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='9' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0A ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0A  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0A ) */

/** \brief Subservice 0x0A for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0A           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='10' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0B ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0B  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0B ) */

/** \brief Subservice 0x0B for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0B           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='11' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0C ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0C  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0C ) */

/** \brief Subservice 0x0C for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0C           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='12' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0D ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0D  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0D ) */

/** \brief Subservice 0x0D for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0D           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='13' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0E ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0E  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0E ) */

/** \brief Subservice 0x0E for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0E           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='14' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x13 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x13  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x13 ) */

/** \brief Subservice 0x13 for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x13           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='19' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x14 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x14  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x14 ) */

/** \brief Subservice 0x14 for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x14           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='20' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x15 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x15  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x15 ) */

/** \brief Subservice 0x15 for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x15           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='21' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x17 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x17  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x17 ) */

/** \brief Subservice 0x17 for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x17           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='23' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x18 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x18  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x18 ) */

/** \brief Subservice 0x18 for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x18           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='24' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x19 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x19  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x19 ) */

/** \brief Subservice 0x19 for 0x19 enabled or not */
#define DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x19           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/*/*[DcmDsdSubServiceId='25' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

/* !LINKSTO Dcm.EB.ReadDTC.ProcPageFunc,1 */
#if (defined DCM_DSP_SUBSERVICES_READDTCINFORMATION_EXTSVC_ENABLED ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICES_READDTCINFORMATION_EXTSVC_ENABLED  already defined
#endif /* if (defined DCM_DSP_SUBSERVICES_READDTCINFORMATION_EXTSVC_ENABLED ) */

/** \brief External handlers are enabled for all subservices for 0x19  */
#define DCM_DSP_SUBSERVICES_READDTCINFORMATION_EXTSVC_ENABLED           [!//
[!IF "$DCM_DSP_USE_SERVICE_0X19 = 'STD_ON'"!]
  [!VAR "SubServiceFncCount" = "0"!][!//
  [!LOOP "DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/DcmDsdSubService/*/DcmDsdSubServiceFnc"!]
    [!VAR "SubServiceFncCount" = "$SubServiceFncCount +1"!][!//
  [!ENDLOOP!]
  [!IF "num:i(count(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='25']/DcmDsdSubService/*)) = num:i($SubServiceFncCount)"!][!//
  STD_ON
  [!ELSE!][!//
  STD_OFF
  [!ENDIF!][!//
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05 ) */

/** \brief Subservice 0x05 for 0x86 enabled or not */
#define DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='134']/*/*[DcmDsdSubServiceId='5' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45 ) */

/** \brief Subservice 0x45 for 0x86 enabled or not */
#define DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='134']/*/*[DcmDsdSubServiceId='69' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00 ) */

/** \brief Subservice 0x00 for 0x86 enabled or not */
#define DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='134']/*/*[DcmDsdSubServiceId='0' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40 ) */

/** \brief Subservice 0x40 for 0x86 enabled or not */
#define DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='134']/*/*[DcmDsdSubServiceId='64' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01 ) */

/** \brief Subservice 0x01 for 0x86 enabled or not */
#define DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='134']/*/*[DcmDsdSubServiceId='1' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41 ) */

/** \brief Subservice 0x41 for 0x86 enabled or not */
#define DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='134']/*/*[DcmDsdSubServiceId='65' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x03 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x03  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x03 ) */

/** \brief Subservice 0x03 for 0x86 enabled or not */
#define DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x03           [!"$DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x03"!]

#if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x43 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x43  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x43 ) */

/** \brief Subservice 0x43 for 0x86 enabled or not */
#define DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x43           [!"$DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x43"!]

#if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x04 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x04  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x04 ) */

/** \brief Subservice 0x04 for 0x86 enabled or not */
#define DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x04           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='134']/*/*[DcmDsdSubServiceId='4' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x06 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x06  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x06 ) */

/** \brief Subservice 0x06 for 0x86 enabled or not */
#define DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x06           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='134']/*/*[DcmDsdSubServiceId='6' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_0X85_0X01_SSVC_ENABLED ) /* To prevent double declaration */
#error DCM_0X85_0X01_SSVC_ENABLED  already defined
#endif /* if (defined DCM_0X85_0X01_SSVC_ENABLED ) */

/** \brief Subservice 0x01 for 0x85 enabled or not */
#define DCM_0X85_0X01_SSVC_ENABLED           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='133']/*/*[DcmDsdSubServiceId='1' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_0X85_0X02_SSVC_ENABLED ) /* To prevent double declaration */
#error DCM_0X85_0X02_SSVC_ENABLED  already defined
#endif /* if (defined DCM_0X85_0X02_SSVC_ENABLED ) */

/** \brief Subservice 0x02 for 0x85 enabled or not */
#define DCM_0X85_0X02_SSVC_ENABLED           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='133']/*/*[DcmDsdSubServiceId='2' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_0X85_CONTROLOPTIONRECORD_ENABLED ) /* To prevent double declaration */
#error DCM_0X85_CONTROLOPTIONRECORD_ENABLED  already defined
#endif /* if (defined DCM_0X85_CONTROLOPTIONRECORD_ENABLED ) */

/** \brief ControlOptionRecord enabled or not */
#define DCM_0X85_CONTROLOPTIONRECORD_ENABLED           [!//
[!IF "(DcmConfigSet/*[1]/DcmDsp/DcmDspControlDTCSetting/DcmSupportDTCSettingControlOptionRecord = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_0X85_CONTROLOPTIONRECORD_SIZE ) /* To prevent double declaration */
#error DCM_0X85_CONTROLOPTIONRECORD_SIZE  already defined
#endif /* if (defined DCM_0X85_CONTROLOPTIONRECORD_SIZE ) */

/** \brief size of ControlOptionRecord in the request */
#define DCM_0X85_CONTROLOPTIONRECORD_SIZE           [!//
[!IF "(DcmConfigSet/*[1]/DcmDsp/DcmDspControlDTCSetting/DcmSupportDTCSettingControlOptionRecord = 'true')"!]3[!ELSE!]0[!ENDIF!]U

#if (defined DCM_DSP_SUBSERVICE_LINKCONTROL_0x01 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_LINKCONTROL_0x01  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_LINKCONTROL_0x01 ) */

#define DCM_DSP_SUBSERVICE_LINKCONTROL_0x01           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='135']/*/*[DcmDsdSubServiceId='1' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_LINKCONTROL_0x02 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_LINKCONTROL_0x02  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_LINKCONTROL_0x02 ) */

#define DCM_DSP_SUBSERVICE_LINKCONTROL_0x02           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='135']/*/*[DcmDsdSubServiceId='2' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_DSP_SUBSERVICE_LINKCONTROL_0x03 ) /* To prevent double declaration */
#error DCM_DSP_SUBSERVICE_LINKCONTROL_0x03  already defined
#endif /* if (defined DCM_DSP_SUBSERVICE_LINKCONTROL_0x03 ) */

#define DCM_DSP_SUBSERVICE_LINKCONTROL_0x03           [!//
[!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId='135']/*/*[DcmDsdSubServiceId='3' and not(node:exists(DcmDsdSubServiceFnc))])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_BSWM_API_ENABLED ) /* To prevent double declaration */
#error DCM_BSWM_API_ENABLED  already defined
#endif /* if defined (DCM_BSWM_API_ENABLED ) */

/** \brief BswM-Dcm interaction enabled or not */
#define DCM_BSWM_API_ENABLED           [!//
[!IF "node:exists(as:modconf('BswM')/BswMGeneral/BswMDcmEnabled) and as:modconf('BswM')/BswMGeneral/BswMDcmEnabled = 'true'"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_0XAF_EXT_SSVCH_ENABLED ) /* To prevent double declaration */
#error DCM_0XAF_EXT_SSVCH_ENABLED  already defined
#endif /* if (defined DCM_0XAF_EXT_SSVCH_ENABLED ) */

/** \brief External Subservice for 0xAF enabled or not */
#define DCM_0XAF_EXT_SSVCH_ENABLED           [!//
[!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*[1]/DcmDsdService/*[DcmDsdSidTabServiceId='175']/DcmDsdSubService/*[node:exists(DcmDsdSubServiceFnc)])"!]
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DATA_TRANSFER_SERVICES_AUTOSAR_403)
#error DATA_TRANSFER_SERVICES_AUTOSAR_403 already defined
#endif
/** \brief Macro for using ASR 4.0 for DataTransfer services */
#define DATA_TRANSFER_SERVICES_AUTOSAR_403 0U

#if (defined DATA_TRANSFER_SERVICES_AUTOSAR_422)
#error DATA_TRANSFER_SERVICES_AUTOSAR_422 already defined
#endif
/** \brief Macro for using ASR 4.2.2 for DataTransfer services */
#define DATA_TRANSFER_SERVICES_AUTOSAR_422 1U

#if (defined DATA_TRANSFER_SERVICES_AUTOSAR_430)
#error DATA_TRANSFER_SERVICES_AUTOSAR_430 already defined
#endif
/** \brief Macro for using ASR 4.3 for DataTransfer services */
#define DATA_TRANSFER_SERVICES_AUTOSAR_430 2U

#if (defined DCM_DATATRANSFER_SERVICES_ASR_VERSION)
#error DCM_DATATRANSFER_SERVICES_ASR_VERSION already defined
#endif
/** \brief ASR version compatibility for DataTransfer services */
#define DCM_DATATRANSFER_SERVICES_ASR_VERSION                   [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmDataTransferServicesASRVersion = 'AUTOSAR_43'"!]DATA_TRANSFER_SERVICES_AUTOSAR_430
[!ELSEIF "DcmConfigSet/*[1]/DcmGeneral/DcmDataTransferServicesASRVersion = 'AUTOSAR_422'"!]DATA_TRANSFER_SERVICES_AUTOSAR_422
[!ELSE!]DATA_TRANSFER_SERVICES_AUTOSAR_403
[!ENDIF!]

#if (defined DCM_ENABLE_DATA_TRANSFER_MEMRANGE_AND_SECURITY_CHECK)
#error DCM_ENABLE_DATA_TRANSFER_MEMRANGE_AND_SECURITY_CHECK already defined
#endif
/** \brief Define whether memory range and security checks
 ** are enabled for RequestDownload and RequestUpload */
#define DCM_ENABLE_DATA_TRANSFER_MEMRANGE_AND_SECURITY_CHECK                   [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmDataTransferServicesMemoryRangeAndSecurityCheck = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_ENABLE_DATA_TRANSFER_MEM_ID_CHECK)
#error DCM_ENABLE_DATA_TRANSFER_MEM_ID_CHECK already defined
#endif
/** \brief Define whether memory Id check is enabled for RequestDownload and RequestUpload */
#define DCM_ENABLE_DATA_TRANSFER_MEM_ID_CHECK                   [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmDataTransferServicesMemoryIdCheck = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/*------------------[ Service API ]----------------------------------------------------*/

#if (defined DCM_SERVICE_API_NONE)
#error DCM_SERVICE_API_NONE already defined
#endif
/** \brief Definition that no default service API is set */
#define DCM_SERVICE_API_NONE   0U

#if (defined DCM_SERVICE_API_ASR40)
#error DCM_SERVICE_API_ASR40 already defined
#endif
/** \brief Definition that AUTOSAR 4.0 service API is set as default */
#define DCM_SERVICE_API_ASR40 40U

#if (defined DCM_SERVICE_API_ASR42)
#error DCM_SERVICE_API_ASR42 already defined
#endif
/** \brief Definition that AUTOSAR 4.2 service API is set as default */
#define DCM_SERVICE_API_ASR42 42U

#if (defined DCM_ISO14229_2006)
#error DCM_ISO14229_2006 already defined
#endif
/** \brief Definition that ISO14229_2006 is set */
#define DCM_ISO14229_2006 6U

#if (defined DCM_ISO14229_2013)
#error DCM_ISO14229_2013 already defined
#endif
/** \brief Definition that ISO14229_2013 is set */
#define DCM_ISO14229_2013 13U

#if (defined DCM_ENABLE_ASR40_SERVICE_API)
#error DCM_ENABLE_ASR40_SERVICE_API already defined
#endif
/** \brief Definition whether AUTOSAR 4.0 service API for Dcm is enabled */
#define DCM_ENABLE_ASR40_SERVICE_API                   [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmServiceAPI/DcmEnableASR40ServiceAPI = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_ENABLE_ASR42_SERVICE_API)
#error DCM_ENABLE_ASR42_SERVICE_API already defined
#endif
/** \brief Definition whether AUTOSAR 4.2 service API for Dcm is enabled */
#define DCM_ENABLE_ASR42_SERVICE_API                   [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmServiceAPI/DcmEnableASR42ServiceAPI = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DCM_DEFAULT_ASR_SERVICE_API)
#error DCM_DEFAULT_ASR_SERVICE_API already defined
#endif
/** \brief Definition of default service API */
#define DCM_DEFAULT_ASR_SERVICE_API                    [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmServiceAPI/DcmDefaultASRServiceAPI = 'AUTOSAR_42'"!]DCM_SERVICE_API_ASR42
[!ELSE!]DCM_SERVICE_API_ASR40
[!ENDIF!]

#if (defined DCM_NRC_POLICY_ISO_VERSION)
#error DCM_NRC_POLICY_ISO_VERSION already defined
#endif
/** \brief Definition of ISO ISO14229_2006 or ISO14229_2013*/
#define DCM_NRC_POLICY_ISO_VERSION        [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmNRCPolicyISOVersion = 'ISO14229_2006'"!]DCM_ISO14229_2006
[!ELSE!]DCM_ISO14229_2013
[!ENDIF!]

#if (defined DCM_DSP_SRV_TABLES_USED_BUF_SIZE) /* To prevent double declaration */
#error DCM_DSP_SRV_TABLES_USED_BUF_SIZE already defined
#endif /* if (defined DCM_DSP_SRV_TABLES_USED_BUF_SIZE) */
/** \brief Size of Service Tables Used Buffer */
#define DCM_DSP_SRV_TABLES_USED_BUF_SIZE [!"num:i(count(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*))"!]U

#if (defined DCM_DSP_SERVICE_USED_BUF_SIZE) /* To prevent double declaration */
#error DCM_DSP_SERVICE_USED_BUF_SIZE already defined
#endif /* if (defined DCM_DSP_SERVICE_USED_BUF_SIZE) */
[!VAR "MaxValue" = "0"!][!//
[!LOOP "node:order(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*, 'DcmDsdSidTabId')"!][!//
  [!VAR "Count" = "count(DcmDsdService/*)"!][!//
  [!IF "$Count > $MaxValue"!][!//
    [!VAR "MaxValue" = "$Count"!]
  [!ENDIF!]
[!ENDLOOP!]
/** \brief Maximum Size of Service Used Buffer for a Service Table */
#define DCM_DSP_SERVICE_USED_BUF_SIZE [!"num:i(($MaxValue + 7) div 8)"!]U

/*------------------[ Service $01 ]----------------------------------------------------*/

#if (defined DCM_NUM_PID_CFG)
#error DCM_NUM_PID_CFG already defined
#endif /* if (defined DCM_NUM_PID_CFG) */

/** \brief Number of PIDs configured */
#define DCM_NUM_PID_CFG             [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspPid/*))"!]U

#if (defined DCM_DSP_PID_DATA_NUM) /* To prevent double declaration */
#error DCM_DSP_PID_DATA_NUM already defined
#endif /* if (defined DCM_DSP_PID_DATA_NUM) */

/** \brief Number of PID Data configured */
#define DCM_DSP_PID_DATA_NUM [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspPid/*/DcmDspPidData/*))"!]U

#if (defined DCM_DSP_PID_USED_BUF_SIZE) /* To prevent double declaration */
#error DCM_DSP_PID_USED_BUF_SIZE already defined
#endif /* if (defined DCM_DSP_PID_USED_BUF_SIZE) */

/** \brief Size of Pid Used Buffer */
#define DCM_DSP_PID_USED_BUF_SIZE [!"num:i((count(DcmConfigSet/*[1]/DcmDsp/DcmDspPid/*)+7) div 8)"!]U

#if (defined DCM_DSP_PID_SUPINFO_DATA_NUM) /* To prevent double declaration */
#error DCM_DSP_PID_SUPINFO_DATA_NUM already defined
#endif /* if (defined DCM_DSP_PID_SUPINFO_DATA_NUM) */

[!VAR "SupInfoDataBufSize" = "0"!][!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspPid/*/DcmDspPidData/*)"!]
  [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspPid/*"!]
    [!LOOP "DcmDspPidSupportInfo/*"!]
      [!VAR "SupInfoDataBufSize" = "$SupInfoDataBufSize + DcmDspPidSupportInfoLen"!]
    [!ENDLOOP!]
  [!ENDLOOP!]
[!ENDIF!]
/** \brief Number of Elements in SupportInfoData Array */
#define DCM_DSP_PID_SUPINFO_DATA_NUM [!//
[!"num:i($SupInfoDataBufSize)"!]U

#if (defined DCM_DSP_PID_SUPINFO_CONFIG_NUM) /* To prevent double declaration */
#error DCM_DSP_PID_SUPINFO_CONFIG_NUM already defined
#endif /* if (defined DCM_DSP_PID_SUPINFO_CONFIG_NUM) */

/** \brief Number of SupInfo Config */
#define DCM_DSP_PID_SUPINFO_CONFIG_NUM [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspPid/*/DcmDspPidSupportInfo/*))"!]U

#if (defined DCM_PID_DATA_MAX_SIZE) /* To prevent double declaration */
#error DCM_PID_DATA_MAX_SIZE already defined
#endif /* if (defined DCM_PID_DATA_MAX_SIZE) */

/* !LINKSTO Dcm.Dsn.OBDPidServices.PidDataBufferSize,1 */
/** \brief Maximum size of a PID Data configured in bytes */
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspPid/*/DcmDspPidData/*)"!][!//
  [!VAR "maxsizepiddata" = "num:max(DcmConfigSet/*[1]/DcmDsp/DcmDspPid/*/DcmDspPidData/*/DcmDspPidDataSize)"!][!//
  [!VAR "maxsizepiddata" = "num:i(($maxsizepiddata+7) div 8)"!][!//
[!ELSE!][!//
  [!VAR "maxsizepiddata" = "0"!][!//
[!ENDIF!][!//
#define DCM_PID_DATA_MAX_SIZE      [!//
[!"num:i($maxsizepiddata)"!]U

/** \brief OBD Service $08 Support */
#if (defined DCM_DSP_NUM_REQUESTCONTROL_CFG)
#error DCM_DSP_NUM_REQUESTCONTROL_CFG already defined
#endif /* if (defined DCM_DSP_NUM_REQUESTCONTROL_CFG) */

/** \brief Number of RequestControl items configured */
#define DCM_DSP_NUM_REQUESTCONTROL_CFG             [!//
[!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspRequestControl/*))"!]U

/*------------------[ Complex conditions ]----------------------------------------------------*/

#if (defined DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION ) /* To prevent double declaration */
#error DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION  already defined
#endif /* if (defined DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION ) */

/** \brief Indicates whether AddressLengthAndFormat_Verification is required or not  */
#define DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION           [!"$DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION"!]

#if (defined DCM_DSP_USE_GETMEMORYADDRESSANDSIZE ) /* To prevent double declaration */
#error DCM_DSP_USE_GETMEMORYADDRESSANDSIZE  already defined
#endif /* if (defined DCM_DSP_USE_GETMEMORYADDRESSANDSIZE ) */

/** \brief Indicates whether GetMemoryAddressAndSize is required or not  */
#define DCM_DSP_USE_GETMEMORYADDRESSANDSIZE           [!"$DCM_DSP_USE_GETMEMORYADDRESSANDSIZE"!]

/*-----------[  Dynamically defined DID initialization related configuration items ]--------------*/
[!IF "$DCM_DSP_USE_SERVICE_0X2C = 'STD_ON'"!]

#if (defined DCM_DDDID_CLEAR_ALWAYS)
  #error "DCM_DDDID_CLEAR_ALWAYS is already defined"
#endif
#define DCM_DDDID_CLEAR_ALWAYS                 10
#if (defined DCM_DDDID_CLEAR_ONLY_POWERON)
  #error "DCM_DDDID_CLEAR_ONLY_POWERON is already defined"
#endif
#define DCM_DDDID_CLEAR_ONLY_POWERON           11

#if (defined DCM_PRDI_PERIODIC_CLEAR_ALWAYS)
  #error "DCM_PRDI_PERIODIC_CLEAR_ALWAYS is already defined"
#endif
#define DCM_PRDI_PERIODIC_CLEAR_ALWAYS         12
#if (defined DCM_PRDI_PERIODIC_CLEAR_ONLY_POWERON)
  #error "DCM_PRDI_PERIODIC_CLEAR_ONLY_POWERON is already defined"
#endif
#define DCM_PRDI_PERIODIC_CLEAR_ONLY_POWERON   13
#if (defined DCM_PRDI_PERIODIC_CLEAR_NEVER)
  #error "DCM_PRDI_PERIODIC_CLEAR_NEVER is already defined"
#endif
#define DCM_PRDI_PERIODIC_CLEAR_NEVER          14

[!VAR "CurrentDDDIDInitPolicy" = "'STD_OFF'"!]
[!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmGeneral/DcmDynamicallyDefinedDIDInitPolicy)"!]
  [!VAR "CurrentDDDIDInitPolicy" = "node:value(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmGeneral/DcmDynamicallyDefinedDIDInitPolicy)"!]
  [!VAR "CurrentDDDIDInitPolicy" = "concat('DCM_',$CurrentDDDIDInitPolicy)"!]
[!ENDIF!]
[!//
[!VAR "CurrentPeriodicDDDIDInitPolicy" = "'STD_OFF'"!]
[!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmGeneral/DcmDynamicallyDefinedPeriodicDIDInitPolicy)"!]
  [!VAR "CurrentPeriodicDDDIDInitPolicy" = "node:value(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmGeneral/DcmDynamicallyDefinedPeriodicDIDInitPolicy)"!]
  [!VAR "CurrentPeriodicDDDIDInitPolicy" = "concat('DCM_',$CurrentPeriodicDDDIDInitPolicy)"!]
[!ENDIF!]
[!//
/** \brief Define if standard DDDID initialization is available. */
#if (defined DCM_DDDID_DO_NOT_INIT)
#error DCM_DDDID_DO_NOT_INIT already defined
#endif
#define DCM_DDDID_DO_NOT_INIT             [!//
[!IF "($CurrentDDDIDInitPolicy = 'DCM_DDDID_CLEAR_ONLY_POWERON') and ($CurrentPeriodicDDDIDInitPolicy = 'DCM_PRDI_PERIODIC_CLEAR_ONLY_POWERON')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Flag to indicate that normal DDDIDs shall be cleared upon startup */
#if (defined DCM_DDDID_POLICY_PERIODIC)
#error DCM_DDDID_POLICY_PERIODIC already defined
#endif
#define DCM_DDDID_POLICY_PERIODIC             [!"$CurrentPeriodicDDDIDInitPolicy"!]

/** \brief Flag to indicate that periodic DDDIDs shall be cleared upon startup */
#if (defined DCM_DDDID_POLICY_NONPERIODIC)
#error DCM_DDDID_POLICY_NONPERIODIC already defined
#endif
#define DCM_DDDID_POLICY_NONPERIODIC          [!"$CurrentDDDIDInitPolicy"!]

/** Find the index of the first and last dynamically defined DIDs in the DID config array */
[!VAR "CurrentDidIndex" = "0"!]
[!VAR "DDDIDInitStartIndex" = "65535"!]
[!VAR "DDDIDInitStopIndex" = "0"!]
[!LOOP "node:order(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[DcmDspDidUsed = 'true'], 'DcmDspDidIdentifier')"!]
  [!IF "node:ref(DcmDspDidInfoRef)/DcmDspDidDynamicallyDefined = 'true'"!]
    [!IF "$DDDIDInitStartIndex > $CurrentDidIndex"!]
      [!VAR "DDDIDInitStartIndex" = "$CurrentDidIndex"!]
    [!ENDIF!]
    [!VAR "DDDIDInitStopIndex" = "$CurrentDidIndex"!]
  [!ENDIF!]
  [!VAR "CurrentDidIndex" = "$CurrentDidIndex + 1"!]
[!ENDLOOP!]

/** \brief Index of the first dynamically defined DID in the DID config array */
#if (defined DCM_DDDID_INIT_START_INDEX)
#error DCM_DDDID_INIT_START_INDEX already defined
#endif
#define DCM_DDDID_INIT_START_INDEX       [!"num:i($DDDIDInitStartIndex)"!]

/** \brief Index of the last dynamically defined DID in the DID config array */
#if (defined DCM_DDDID_INIT_STOP_INDEX)
#error DCM_DDDID_INIT_STOP_INDEX already defined
#endif
#define DCM_DDDID_INIT_STOP_INDEX        [!"num:i($DDDIDInitStopIndex)"!]
[!ENDIF!]

/** \brief Defines the existence of configured S3 timeout notification callout function. */
#if (defined DCM_S3TIMEOUTNOTIFICATION)
  #error "DCM_S3TIMEOUTNOTIFICATION is already defined"
#endif
#define DCM_S3TIMEOUTNOTIFICATION [!WS "5"!][!//
[!IF "node:value(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslEnableS3TimeoutNotification) = 'true'"!][!//
STD_ON
[!ELSE!][!//
STD_OFF
[!ENDIF!]

#if (defined DCM_META_DATA_HANDLING_ENABLED)
  #error "DCM_META_DATA_HANDLING_ENABLED is already defined"
#endif
#define DCM_META_DATA_HANDLING_ENABLED         [!//
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmGenericConnectionUsage = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/*==================[type definitions]===========================================================*/
[!//
[!/*------------ generate read scaling size ----------------*/!][!//
[!IF "$BufScalingMaxSize < 255"!][!//
/** \brief Define the size of the read scaling info */
typedef uint8 Dcm_ReadScalingSignalSizeType;
[!ELSEIF "$BufScalingMaxSize < 65535"!][!//
/** \brief Define the size of the read scaling info */
typedef uint16 Dcm_ReadScalingSignalSizeType;
[!ELSE!][!//
/** \brief Define the size of the read scaling info */
typedef uint32 Dcm_ReadScalingSignalSizeType;
[!ENDIF!][!//

/*------------------[Dcm_ConfigType]----------------------------------------*/

/** \brief This type of the external data structure shall contain the post
 **  build initialization data for the Dcm.
 **
 ** \note Type is unused, as only pre-compile time support is implemented. */
typedef uint8 Dcm_ConfigType;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspSession/*/DcmDspSessionRow/*"!][!//
[!VAR "StrName"="''"!]
[!VAR "StrTemp"="substring-after(name(.),'DCM_')"!]
[!IF "$StrTemp = ''"!][!VAR "StrTemp"="name(.)"!][!ENDIF!]
[!LOOP "text:split($StrTemp, '_')"!]
[!VAR "StrName"="concat($StrName, concat(substring(., 1,1), text:tolower(substring(., 2))))"!]
[!ENDLOOP!]
[!//
[!IF "not(contains($StrName,'AllSessionLevel'))"!]
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_[!"$StrName"!]ModeEntry(void);

[!ENDIF!]
[!ENDLOOP!][!//
[!//
[!LOOP "DcmConfigSet/*/DcmProcessingConditions/DcmModeRule/*"!]
/** \brief Mode rule function declaration for [!"name(.)"!] */
extern FUNC(boolean, DCM_CODE) Dcm_ModeRule_[!"name(.)"!]_Result(P2VAR(uint8, AUTOMATIC, DCM_VAR) Nrc);

[!ENDLOOP!]
#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

#define DCM_START_SEC_CONST_8
#include <Dcm_MemMap.h>

/** \brief Configuration structure */
extern CONST(Dcm_ConfigType, DCM_CONST) [!"name(DcmConfigSet/*[1])"!];

#define DCM_STOP_SEC_CONST_8
#include <Dcm_MemMap.h>

/*==================[internal data]==============================================================*/
[!//
[!/*------------ generate structure of OBDMIDs from configuration ----------------*/!][!//
[!IF "($DCM_DSP_USE_SERVICE_0X06 = 'STD_ON') and (node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspTestResultByObdmid/DcmDspTestResultObdmidTid/*))"!][!//
#if (defined DCM_DSP_NR_OBDMID)       /* To prevent double declaration */
#error DCM_DSP_NR_OBDMID already defined
#endif

/** \brief Definition of number of OBDMIDs  */
#define DCM_DSP_NR_OBDMID [!//
[!"num:i(count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspTestResultByObdmid/DcmDspTestResultObdmidTid/*))"!]U

/** \brief Number of OBDMid used*/
#if (defined DCM_NUM_OBDMIDUSED_CFG)
#error DCM_NUM_OBDMIDUSED_CFG already defined
#endif /* if (defined DCM_NUM_OBDMIDUSED_CFG) */

/** \brief Number of OBDMid used*/
#define DCM_NUM_OBDMIDUSED_CFG             [!//
[!"num:i((count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspTestResultByObdmid/DcmDspTestResultObdmidTid/*)+7)div 8)"!]U

#if (defined DCM_NUM_OBDMID_CFG)
#error DCM_NUM_OBDMID_CFG already defined
#endif /* if (defined DCM_NUM_OBDMID_CFG) */

/** \brief Number of OBDMid used*/
#define DCM_NUM_OBDMID_CFG             [!//
[!"num:i(count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspTestResultByObdmid/DcmDspTestResultObdmidTid/*))"!]U

#if (defined DCM_NUM_OBDMIDTID_CFG)
#error DCM_NUM_OBDMIDTID_CFG already defined
#endif /* if (defined DCM_NUM_OBDMIDTID_CFG) */

/** \brief Number of OBDMidTid used*/
#define DCM_NUM_OBDMIDTID_CFG             [!//
[!"num:i(count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspTestResultByObdmid/DcmDspTestResultObdmidTid/*/DcmDspTestResultObdmidTids/*/DcmDspTestResultObdmidTidRefs/*))"!]U

/* !LINKSTO Dcm.dsn.Dsp.OBDMID.Config.OBDMIDIdentifier,1 */
/*
 * Dcm_DspOBDMIDConfigType shall contain the structure element OBDMIDIdentifier of type Dcm_DspOBDMIDIdentifierType
 * OBDMIDIdentifier shall contain the configured value of DcmDspTestResultObdmid of this DcmDspTestResultObdmidTid
 */
typedef uint8 Dcm_DspOBDMIDIdentifierType;

/* !LINKSTO Dcm.dsn.Dsp.OBDMID.Config.NumOfTid,1 */
/*
 * Dcm_DspOBDMIDConfigType shall contain the structure element NumOfTid of type Dcm_DspOBDMIDNumOfTidType
 * NumOfTid shall contain the number of element of Dcm_DspOBDMIDTidConfig[] assigned to this OBDMID
 */
typedef uint8 Dcm_DspOBDMIDNumOfTidType;

/* !LINKSTO Dcm.dsn.Dsp.OBDMID.Config.TidStartIndex,1 */
/*
 * Dcm_DspOBDMIDConfigType shall contain the structure element TidStartIndex of type Dcm_DspOBDMIDTidStartIndexType.
 * TidStartIndex shall refer to the first element of Dcm_DspOBDMIDTidConfig[] assigned to this OBDMID
 */
typedef uint16 Dcm_DspOBDMIDTidStartIndexType;

typedef uint8 Dcm_DspUsedType;

typedef struct
{
  P2CONST(Dcm_DspUsedType, AUTOMATIC, DCM_CONST) TidUsedPtr;
  Dcm_DspOBDMIDTidStartIndexType TidStartIndex;
  Dcm_DspOBDMIDIdentifierType OBDMIDIdentifier;
  Dcm_DspOBDMIDNumOfTidType NumOfTid;
} Dcm_DspOBDMIDConfigType;

/* !LINKSTO Dcm.dsn.Dsp.OBDMID.TidConfig.UaSid,1 */
/*
 * Dcm_DspOBDMIDTidType shall contain the structure element UaSid of type Dcm_DspOBDMIDTidUaSidType
 * UaSid shall contain the configured value of DcmDspTestResultObdmidTidUaSid of this DcmDspTestResultObdmidTids
 */
typedef uint8 Dcm_DspOBDMIDTidUaSidType;

/* !LINKSTO Dcm.dsn.Dsp.OBDMID.TidConfig.TidIdentifier,1 */
/*
 * Dcm_DspOBDMIDTidType shall contain the structure element TidIdentifier of type Dcm_DspOBDMIDTidIdentifierType
 * If TidIdentifier is set to 255 the TID is not supported in the current configuration
 * TidIdentifier shall contain the value of DcmDspTestResultTestId of the referred DcmDspTestResultObdmidTidRef
 */
typedef uint8 Dcm_DspOBDMIDTidIdentifierType;

/* !LINKSTO Dcm.dsn.Dsp.OBDMID.TidConfig.GetDTRValueFnc,1 */
/*
 * Dcm_DspOBDMIDTidType shall contain the structure element GetDTRValueFnc of type Dcm_DspGetDTRValueFunctionType
 * GetDTRValueFnc shall contain the pointer to the generated Xxx_GetDTRValue() operation of the refered DcmDspTestResultTid (DcmDspTestResultObdmidTidRef)
 */
typedef Std_ReturnType(*Dcm_DspGetDTRValueFunctionType)
(
  uint8 OpStatus,
  P2VAR(uint16, TYPEDEF, DCM_VAR) Testval,
  P2VAR(uint16, TYPEDEF, DCM_VAR) Minlimit,
  P2VAR(uint16, TYPEDEF, DCM_VAR) Maxlimit,
  P2VAR(uint8, TYPEDEF, DCM_VAR) Status
);

typedef struct
{
  Dcm_DspOBDMIDTidUaSidType UaSid;
  Dcm_DspOBDMIDTidIdentifierType TidIdentifier;
  Dcm_DspGetDTRValueFunctionType GetDTRValueFnc;
} Dcm_DspOBDMIDTidConfigType;

#define DCM_START_SEC_CALIB_8
#include <Dcm_MemMap.h>

[!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspTestResultByObdmid/DcmDspTestResultObdmidTid/*"!]
extern CONST(Dcm_DspUsedType, DCM_CONST) Dcm_DspTestResultTidUsed_[!"num:inttohex(node:value(./*), 2)"!][!//
[[!"num:i((count(./DcmDspTestResultObdmidTids/*/DcmDspTestResultObdmidTidRefs/*/DcmDspTestResultObdmidTidRef)+7)div 8)"!]U];

[!ENDLOOP!]
[!//

extern CONST(Dcm_DspUsedType, DCM_CONST) Dcm_DspOBDMIDUsed[DCM_NUM_OBDMIDUSED_CFG];

#define DCM_STOP_SEC_CALIB_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

extern CONST(Dcm_DspOBDMIDConfigType, DCM_CONST) Dcm_DspOBDMIDConfig[DCM_NUM_OBDMID_CFG];

extern CONST(Dcm_DspOBDMIDTidConfigType, DCM_CONST) Dcm_DspOBDMIDTidConfig[DCM_NUM_OBDMIDTID_CFG];

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>
[!ENDIF!]

#if (defined DCM_WRITEMIDBEFORETID_ENABLED)
#error DCM_WRITEMIDBEFORETID_ENABLED already defined
#endif /* if (defined DCM_WRITEMIDBEFORETID_ENABLED) */

/** \brief Flage indicates whether OBDMID shall be written before every TID,
 * in case it's requested via service 0x22. */
#define DCM_WRITEMIDBEFORETID_ENABLED             [!//
[!IF "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspTestResultByObdmid/DcmWriteMidBeforeTid = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!]


/* !LINKSTO Dcm.dsn.OBDMIDSupport,1 */
#if (defined DCM_OBDMID_SUPPORT_SWC)
#error DCM_OBDMID_SUPPORT_SWC already defined
#endif
/* define SWC support to obtain info for Obdmids */
#define DCM_OBDMID_SUPPORT_SWC                          [!//
[!IF "(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspTestResultByObdmid/DcmOBDMIDSupportType = 'DCM_OBDMID_SUPPORT_SWC')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/* This macro tells if the used Pdu router is supplied by Elektrobit or not. Elektrobit has vendorID '1' */
#if (defined DCM_USE_ELEKTROBIT_SUPPLIED_PDUR)
#error DCM_USE_ELEKTROBIT_SUPPLIED_PDUR already defined
#endif
#define DCM_USE_ELEKTROBIT_SUPPLIED_PDUR                [!//
[!IF "node:exists(as:modconf('PduR')[1]/CommonPublishedInformation/VendorId) and (as:modconf('PduR')[1]/CommonPublishedInformation/VendorId = 1)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief This macro is defined when the defined upper limits of the defined memory ranges are to be
* excluded from the defined range. */
#if (defined DCM_EXCLUDE_UPPER_LIMITS)
#error DCM_EXCLUDE_UPPER_LIMITS already defined
#endif

#define DCM_EXCLUDE_UPPER_LIMITS                          [!//
[!IF "((as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsp/DcmDspMemory/DcmDspMemoryRangeHighNotIncluded = 'true'))"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/*==================[external function definitions]==============================================*/

[!IF "node:value(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslEnableS3TimeoutNotification) = 'true'"!][!//
#define DCM_START_SEC_CALLOUT_CODE
#include <Dcm_MemMap.h>

/** \brief Callout function, to be notified when occurs the timeout of S3 timer.
 **
 ** \ServiceID{::DCM_SERVID_UNDEFINED}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
/* !LINKSTO Dcm.EB.SessionStateHandling.Enabled.Dcm_S3TimeoutNotification,1 */
extern FUNC(void, DCM_APPL_CODE) Dcm_S3TimeoutNotification(void);

#define DCM_STOP_SEC_CALLOUT_CODE
#include <Dcm_MemMap.h>
[!ENDIF!]

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_CFG_H */
/*==================[end of file]================================================================*/
