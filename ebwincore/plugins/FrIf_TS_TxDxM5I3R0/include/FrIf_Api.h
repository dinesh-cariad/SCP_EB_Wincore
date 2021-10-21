/**
 * \file
 *
 * \brief AUTOSAR FrIf
 *
 * This file contains the implementation of the AUTOSAR
 * module FrIf.
 *
 * \version 5.3.25
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


#ifndef FRIF_API_H
#define FRIF_API_H

/*======================[Include Section]=====================================*/

#include <ComStack_Types.h>     /* get AUTOSAR ComStack data types          */
#include <Fr_GeneralTypes.h>    /* get AUTOSAR Fr data types                */
#include <FrIf_Types.h>         /* get AUTOSAR FrIf data types              */
#include <FrIf_Cfg.h>           /* get AUTOSAR pre-compile configuration    */
#include <FrIf_Version.h>       /* get AUTOSAR FrIf version declaration     */

#if (FRIF_PBCFGM_SUPPORT_ENABLED == STD_ON)
#include <PbcfgM_Api.h>         /* Post build configuration manager */
#endif /* FRIF_PBCFGM_SUPPORT_ENABLED */

/*======================[Global Macros]=======================================*/

#if (defined FRIF_INSTANCE_ID)
#error FRIF_INSTANCE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define FRIF_INSTANCE_ID         0U

/*
 * Service identifiers for all API functions of the FrTp module
 */

/*
 * Service identifiers for Frif_AbortCommunication API function
 */
#define FRIF_ABORTCOMMUNICATION_SERVICE_ID (0x06U)

/*
 * Service identifiers for FrIf_AckAbsoluteTimerIRQ API function
 */
#define FRIF_ACKABSOLUTETIMERIRQ_SERVICE_ID (0x21U)

/*
 * Service identifiers for FrIf_AckIRQ API function
 */
#define FRIF_ACKIRQ_SERVICE_ID (0x86U)

/*
 * Service identifiers for FrIf_AckRelativeTimerIRQ API function
 */
#define FRIF_ACKRELATIVETIMERIRQ_SERVICE_ID (0x97U)

/*
 * Service identifiers for FrIf_AllowColdstart API function
 */
#define FRIF_ALLOWCOLDSTART_SERVICE_ID (0x10U)

/*
 * Service identifiers for FrIf_AllSlots API function
 */
#define FRIF_ALLSLOTS_SERVICE_ID (0x33U)

/*
 * Service identifiers for FrIf_CancelAbsoluteTimer API function
 */
#define FRIF_CANCELABSOLUTETIMER_SERVICE_ID (0x1BU)

/*
 * Service identifiers for FrIf_CancelRelativeTimer API function
 */
#define FRIF_CANCELRELATIVETIMER_SERVICE_ID (0x94U)

/*
 * Service identifiers for FrIf_CheckMTS API function
 */
#define FRIF_CHECKMTS_SERVICE_ID (0x92U)

/*
 * Service identifiers for FrIf_CheckWakeupByTransceiver API function
 */
#define FRIF_CHECKWAKEUPBYTRANSCEIVER_SERVICE_ID (0x39U)

/*
 * Service identifiers for FrIf_ClearTransceiverWakeup API function
 */
#define FRIF_CLEARTRANSCEIVERWAKEUP_SERVICE_ID (0x18U)

/*
 * Service identifiers for FrIf_ControllerInit API function
 */
#define FRIF_CONTROLLERINIT_SERVICE_ID (0x03U)

/*
 * Service identifiers for FrIf_DisableAbsoluteTimerIRQ API function
 */
#define FRIF_DISABLEABSOLUTETIMERIRQ_SERVICE_ID (0x23U)

/*
 * Service identifiers for FrIf_DisableIRQ API function
 */
#define FRIF_DISABLEIRQ_SERVICE_ID (0x88U)

/*
 * Service identifiers for FrIf_DisableLPdu API function
 */
#define FRIF_DISABLELPDU_SERVICE_ID (0x28U)

/*
 * Service identifiers for FrIf_DisableRelativeTimerIRQ API function
 */
#define FRIF_DISABLERELATIVETIMERIRQ_SERVICE_ID (0x98U)

/*
 * Service identifiers for FrIf_DisableTransceiverBranch API function
 */
#define FRIF_DISABLETRANSCEIVERBRANCH_SERVICE_ID (0x37U)

/*
 * Service identifiers for FrIf_EnableAbsoluteTimerIRQ API function
 */
#define FRIF_ENABLEABSOLUTETIMERIRQ_SERVICE_ID (0x1DU)

/*
 * Service identifiers for FrIf_EnableIRQ API function
 */
#define FRIF_ENABLEIRQ_SERVICE_ID (0x87U)

/*
 * Service identifiers for FrIf_EnableRelativeTimerIRQ API function
 */
#define FRIF_ENABLERELATIVETIMERIRQ_SERVICE_ID (0x95U)

/*
 * Service identifiers for FrIf_EnableTransceiverBranch API function
 */
#define FRIF_ENABLETRANSCEIVERBRANCH_SERVICE_ID (0x36U)

/*
 * Service identifiers for FrIf_GetAbsoluteTimerIRQStatus API function
 */
#define FRIF_GETABSOLUTETIMERIRQSTATUS_SERVICE_ID (0x1FU)

/*
 * Service identifiers for FrIf_GetChannelStatus API function
 */
#define FRIF_GETCHANNELSTATUS_SERVICE_ID (0x26U)

/*
 * Service identifiers for FrIf_GetClockCorrection API function
 */
#define FRIF_GETCLOCKCORRECTION_SERVICE_ID (0x29U)

/*
 * Service identifiers for FrIf_GetControllerErrorStatus API function
 */
#define FRIF_GETCONTROLLERERRORSTATUS_SERVICE_ID (0x83U)

/*
 * Service identifiers for FrIf_GetCycleLength API function
 */
#define FRIF_GETCYCLELENGTH_SERVICE_ID (0x3AU)

/*
 * Service identifiers for FrIf_GetGlobalTime API function
 */
#define FRIF_GETGLOBALTIME_SERVICE_ID (0x0EU)

/*
 * Service identifiers for FrIf_GetIRQStatus API function
 */
#define FRIF_GETIRQSTATUS_SERVICE_ID (0x89U)

/*
 * Service identifiers for FrIf_GetNmVector API function
 */
#define FRIF_GETNMVECTOR_SERVICE_ID (0x0FU)

/*
 * Service identifiers for FrIf_GetNumOfStartupFrames API function
 */
#define FRIF_GETNUMOFSTARTUPFRAMES_SERVICE_ID (0x34U)

/*
 * Service identifiers for FrIf_GetPOCStatus API function
 */
#define FRIF_GETPOCSTATUS_SERVICE_ID (0x0DU)

/*
 * Service identifiers for FrIf_GetRelativeTimerIRQStatus API function
 */
#define FRIF_GETRELATIVETIMERIRQSTATUS_SERVICE_ID (0x96U)

/*
 * Service identifiers for FrIf_GetSyncFrameList API function
 */
#define FRIF_GETSYNCFRAMELIST_SERVICE_ID (0x2AU)

/*
 * Service identifiers for FrIf_GetSyncState API function
 */
#define FRIF_GETSYNCSTATE_SERVICE_ID (0x99U)

/*
 * Service identifiers for FrIf_GetTransceiverError API function
 */
#define FRIF_GETTRANSCEIVERERROR_SERVICE_ID (0x35U)

/*
 * Service identifiers for FrIf_GetTransceiverMode API function
 */
#define FRIF_GETTRANSCEIVERMODE_SERVICE_ID (0x14U)

/*
 * Service identifiers for FrIf_GetTransceiverWUReason API function
 */
#define FRIF_GETTRANSCEIVERWUREASON_SERVICE_ID (0x15U)

/*
 * Service identifiers for FrIf_GetVersionInfo API function
 */
#define FRIF_GETVERSIONINFO_SERVICE_ID (0x01U)

/*
 * Service identifiers for FrIf_GetWakeupRxStatus API function
 */
#define FRIF_GETWAKEUPRXSTATUS_SERVICE_ID (0x2BU)

/*
 * Service identifiers for FrIf_HaltCommunication API function
 */
#define FRIF_HALTCOMMUNICATION_SERVICE_ID (0x05U)

/*
 * Service identifiers for FrIf_Init API function
 */
#define FRIF_INIT_SERVICE_ID (0x02U)

/*
 * Service identifiers for FrIf_JobListExec_<ClstIdx> API function
 */
#define FRIF_JOBLISTEXECUTION_SERVICE_ID (0x32U)

/*
 * Service identifiers for FrIf_GetMacroticksPerCycle API function
 */
#define FRIF_GETMACROTICKSPERCYCLE_SERVICE_ID (0x11U)

/*
 * Service identifiers for FrIf_EnableBusMirroring API function
 */
#define FRIF_ENABLEBUSMIRRORING_SERVICE_ID (0x4BU)

/*
 * Service identifiers for FrIf_ReadCCConfig API function
 */
#define FRIF_READCCCONFIG_SERVICE_ID (0x3BU)

/*
 * Service identifiers for FrIf_ReconfigLPdu API function
 */
#define FRIF_RECONFIGLPDU_SERVICE_ID (0x00U)

/*
 * Service identifiers for FrIf_SendMTS API function
 */
#define FRIF_SENDMTS_SERVICE_ID (0x90U)

/*
 * Service identifiers for FrIf_StopMTS API function
 */
#define FRIF_STOPMTS_SERVICE_ID (0x91U)

/*
 * Service identifiers for FrIf_SendWUP API function
 */
#define FRIF_SENDWUP_SERVICE_ID (0x0AU)

/*
 * Service identifiers for FrIf_SetAbsoluteTimer API function
 */
#define FRIF_SETABSOLUTETIMER_SERVICE_ID (0x19U)

/*
 * Service identifiers for FrIf_SetExtSync API function
 */
#define FRIF_SETEXTSYNC_SERVICE_ID (0x9AU)

/*
 * Service identifiers for FrIf_SetRelativeTimer API function
 */
#define FRIF_SETRELATIVETIMER_SERVICE_ID (0x93U)

/*
 * Service identifiers for FrIf_SetTransceiverMode API function
 */
#define FRIF_SETTRANSCEIVERMODE_SERVICE_ID (0x13U)

/*
 * Service identifiers for FrIf_SetWakeupChannel API function
 */
#define FRIF_SETWAKEUPCHANNEL_SERVICE_ID (0x09U)

/*
 * Service identifiers for FrIf_StartCommunication API function
 */
#define FRIF_STARTCOMMUNICATION_SERVICE_ID (0x04U)

/*
 * Service identifiers for FrIf_MainFunction API function
 */
#define FRIF_MAINFUNCTION_SERVICE_ID (0x27U)

/*
 * Service identifiers for FrIf_GetState API function
 */
#define FRIF_GETSTATE_SERVICE_ID (0x07U)

/*
 * Service identifiers for FrIf_SetState API function
 */
#define FRIF_SETSTATE_SERVICE_ID (0x08U)

/*
 * Service identifiers for FrIf_GetMacrotickDuration API function
 */
#define FRIF_GETMACROTICKDURATION_SERVICE_ID (0x31U)

/*
 * Service identifiers for FrIf_Transmit API function
 */
#define FRIF_TRANSMIT_SERVICE_ID (0x12U)

/*
 * Service identifiers for API functions without specific SID
 */
#define FRIF_UNKNOWN_SERVICE_ID (0xFFU)

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef FRIF_VENDOR_ID /* configuration check */
#error FRIF_VENDOR_ID must be defined
#endif

#if (FRIF_VENDOR_ID != 1U) /* vendor check */
#error FRIF_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef FRIF_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error FRIF_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (FRIF_AR_RELEASE_MAJOR_VERSION != 4U)
#error FRIF_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef FRIF_AR_RELEASE_MINOR_VERSION /* configuration check */
#error FRIF_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (FRIF_AR_RELEASE_MINOR_VERSION != 0U )
#error FRIF_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef FRIF_AR_RELEASE_REVISION_VERSION /* configuration check */
#error FRIF_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (FRIF_AR_RELEASE_REVISION_VERSION != 3U )
#error FRIF_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef FRIF_SW_MAJOR_VERSION /* configuration check */
#error FRIF_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (FRIF_SW_MAJOR_VERSION != 5U)
#error FRIF_SW_MAJOR_VERSION wrong (!= 5U)
#endif

#ifndef FRIF_SW_MINOR_VERSION /* configuration check */
#error FRIF_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (FRIF_SW_MINOR_VERSION < 3U)
#error FRIF_SW_MINOR_VERSION wrong (< 3U)
#endif

#ifndef FRIF_SW_PATCH_VERSION /* configuration check */
#error FRIF_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (FRIF_SW_PATCH_VERSION < 25U)
#error FRIF_SW_PATCH_VERSION wrong (< 25U)
#endif

/* Development error codes according to Fr SWS */
/**
 * \brief DET error code
 *
 * Invalid pointer (NULL_PTR) passed to service.
 */
#define FRIF_E_INV_POINTER        (0x01U)
/**
 * \brief DET error code
 *
 * Invalid controller index passed to service.
 */
#define FRIF_E_INV_CTRL_IDX       (0x02U)
/**
 * \brief DET error code
 *
 * Invalid cluster index passed to service.
 */
#define FRIF_E_INV_CLST_IDX       (0x03U)
/**
 * \brief DET error code
 *
 * Invalid channel index passed to service.
 */
#define FRIF_E_INV_CHNL_IDX       (0x04U)
/**
 * \brief DET error code
 *
 * Invalid timer index offset passed to service.
 */
#define FRIF_E_INV_TIMER_IDX      (0x05U)
/**
 * \brief DET error code
 *
 * Invalid tx pdu index offset passed to service.
 */
#define FRIF_E_INV_TXPDUID        (0x06U)
/**
 * \brief DET error code
 *
 * Invalid LPdu index passed to service.
 * (Specified but not used)
 */
#define FRIF_E_INV_LPDU_IDX       (0x07U)
/**
 * \brief DET error code
 *
 * Service was called although module was not initialized.
 */
#define FRIF_E_NOT_INITIALIZED    (0x08U)
/**
 * \brief DET error code
 *
 * Invalid job index passed to service.
 */
#define FRIF_E_INV_JOB_IDX        (0x80U)
/**
 * \brief DET error code
 *
 * Invalid cycle vale passed to service.
 */
#define FRIF_E_INV_CYCLE          (0x81U)

#include <FrIf_Extension.h>   /* get Module specific vendor extensions     */


/*======== macro and pre-compile-time consistency check section start ========*/

/* check for defined STD_ON macro value */
#ifndef STD_ON
#error STD_ON not defined
#endif  /* STD_ON */

/* check for defined STD_OFF macro value */
#ifndef STD_OFF
#error STD_OFF not defined
#endif  /* STD_OFF */

/* check for correctly defined switch FRIF_VERSION_INFO_API */
#ifndef FRIF_VERSION_INFO_API
#error macro FRIF_VERSION_INFO_API not defined
#endif /* FRIF_VERSION_INFO_API */

/* test for valid value of FRIF_RELATIVE_TIMER_ENABLE */
#if (FRIF_VERSION_INFO_API != STD_ON) && \
    (FRIF_VERSION_INFO_API != STD_OFF)
#error macro FRIF_VERSION_INFO_API has an invalid value
#endif /* FRIF_VERSION_INFO_API */

/* check for correctly defined switch FRIF_DEV_ERROR_DETECT */
#ifndef FRIF_DEV_ERROR_DETECT
#error macro FRIF_DEV_ERROR_DETECT not defined
#endif /* FRIF_DEV_ERROR_DETECT */

/* test for valid value of FRIF_DEV_ERROR_DETECT */
#if (FRIF_DEV_ERROR_DETECT != STD_ON) && \
    (FRIF_DEV_ERROR_DETECT != STD_OFF)
#error macro FRIF_DEV_ERROR_DETECT has an invalid value
#endif /* FRIF_DEV_ERROR_DETECT */


/* test for valid macro definition of FRIF_GETCHANNELSTATUS_API_ENABLE */
#ifndef FRIF_GETCHANNELSTATUS_API_ENABLE
#error FRIF_GETCHANNELSTATUS_API_ENABLE not defined
#endif  /* FRIF_GETCHANNELSTATUS_API_ENABLE */

/* test for valid value of FRIF_GETCHANNELSTATUS_API_ENABLE */
#if (FRIF_GETCHANNELSTATUS_API_ENABLE != STD_ON) && \
    (FRIF_GETCHANNELSTATUS_API_ENABLE != STD_OFF)
#error macro FRIF_GETCHANNELSTATUS_API_ENABLE has an invalid value
#endif /* FRIF_GETCHANNELSTATUS_API_ENABLE */


/*========= macro and pre-compile-time consistency check section end =========*/

/*======================[Global Data Types]===================================*/

/*======================[Global Data]=========================================*/

/*======================[Global Function Declarations]========================*/

/* start code section declaration */
#define FRIF_START_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

#if (FRIF_VERSION_INFO_API == STD_ON)
/* FrIf_GetVersionInfo() API function is enabled */

/**
 * \brief Get version information of the FlexRay Interface.
 *
 * This service returns the version information of this module. The version
 * information includes:
 * - Module Id
 * - Vendor Id
 * - Vendor specific version numbers
 *
 * If DET is enabled and VersionInfoPtr is NULL_PTR,
 * FRIF_E_INV_POINTER is reported to DET.
 *
 * \param[out] FrIf_VersionInfoPtr Pointer where to store the version
 *                                 information of this module.
 *
 * \ServiceID{0x01}
 * \Reentrancy{Re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void,FRIF_CODE) FrIf_GetVersionInfo
    (
        P2VAR(Std_VersionInfoType,AUTOMATIC,FRIF_APPL_DATA) FrIf_VersionInfoPtr
    );

#else /* FRIF_VERSION_INFO_API */
/* FrIf_GetVersionInfo() API function is disabled -
 * provide macro implementation
 */

/**
 * \brief Get version information of the FlexRay Interface.
 *
 * This service returns the version information of this module. The version
 * information includes:
 * - Module Id
 * - Vendor Id
 * - Vendor specific version numbers
 *
 * \param[out] FrIf_VersionInfoPtr Pointer where to store the version
 *                                 information of this module.
 *
 * \ServiceID{0x01}
 * \Reentrancy{Re-entrant}
 * \Synchronicity{Synchronous}
 */

#define FrIf_GetVersionInfo(FrIf_VersionInfoPtr)                            \
    do                                                                      \
    {                                                                       \
        (FrIf_VersionInfoPtr)->vendorID = FRIF_VENDOR_ID;                   \
        (FrIf_VersionInfoPtr)->moduleID = FRIF_MODULE_ID;                   \
        (FrIf_VersionInfoPtr)->sw_major_version = FRIF_SW_MAJOR_VERSION;    \
        (FrIf_VersionInfoPtr)->sw_minor_version = FRIF_SW_MINOR_VERSION;    \
        (FrIf_VersionInfoPtr)->sw_patch_version = FRIF_SW_PATCH_VERSION;    \
    }while(0)                                                 \

#endif /* FRIF_VERSION_INFO_API */

/** \brief Validate configuration
 **
 ** Checks if the post build configuration fits to the link time configuration part.
 **
 ** \return E_OK if the given module configurations is valid otherwise E_NOT_OK.
 **
 ** \ServiceID{}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, FRIF_CODE) FrIf_IsValidConfig
(
    P2CONST(void, AUTOMATIC, FRIF_APPL_CONST) voidConfigPtr
);

/**
 * \brief Initializes the module.
 *
 * This service initializes the module and registers the post-build-time configuration passed as
 * argument for usage by other service of this module.
 *
 * If FrIf_ConfigPtr is NULL_PTR, FRIF_E_INV_POINTER is reported to DET.
 *
 * \param   FrIf_ConfigPtr (in)     Address of module post-build-time configuration
 *
 * \ServiceID{0x02}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void,FRIF_CODE) FrIf_Init
    (
        P2CONST(FrIf_ConfigType,AUTOMATIC,FRIF_APPL_CONST) FrIf_ConfigPtr
    );


/**
 * \brief  Initializes FlexRay controller
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx             Index of FlexRay controller to initialize.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x03}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_ControllerInit
    (
        uint8 FrIf_CtrlIdx
    );


/**
 * \brief   Invokes CHI command 'RUN'.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x04}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_StartCommunication
    (
        uint8 FrIf_CtrlIdx
    );


/**
 * \brief   Invokes CHI command 'HALT'.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x05}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_HaltCommunication
    (
        uint8 FrIf_CtrlIdx
    );


/**
 * \brief   Invokes CHI command 'FREEZE'.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x06}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_AbortCommunication
    (
        uint8 FrIf_CtrlIdx
    );

#if (FRIF_ALLOWCOLDSTART_API_ENABLE == STD_ON)
/**
 * \brief   Invokes CHI command 'ALLOW_COLDSTART'.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x10}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_AllowColdstart
    (
        uint8 FrIf_CtrlIdx
    );
#endif /* (FRIF_ALLOWCOLDSTART_API_ENABLE == STD_ON) */

/**
 * \brief  Sets the cluster state
 *
 * This service sets the FrIf state (FRIF_STATE_ONLINE, FRIF_STATE_OFFLINE) of the respective FrIf
 * cluster.
 *
 * In FRIF_STATE_ONLINE the joblist executes all communication operations.
 *
 * In FRIF_STATE_OFFLINE the joblist executes only the communication operation PREPARE_LPDU.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIF_ClstIdx contains an invalid value
 * FRIF_E_INV_CLST_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_StateTransition    Requested transition.
 * \param[in] FrIf_ClstIdx            FlexRay cluster index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x08}
 * \Reentrancy{Non Reentrant for same FlexRay controller}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_SetState
    (
        uint8 FrIf_ClstIdx,
        FrIf_StateTransitionType FrIf_StateTransition
    );


/**
 * \brief  Reads the cluster's state
 *
 * This service returns the current state of the FrIf state machine of a cluster.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_ClstIdx contains an invalid value
 * FRIF_E_INV_CLST_IDX is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_StatePtr is NULL_PTR,
 * FRIF_E_INV_POINTER is reported to DET and E_NOT_OK returned.
 *
 * \param[in]  FrIf_ClstIdx         FlexRay cluster index.
 * \param[out] FrIf_StatePtr        Address the current state is written to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x07}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetState
    (
        uint8 FrIf_ClstIdx,
        P2VAR(FrIf_StateType,AUTOMATIC,FRIF_APPL_DATA) FrIf_StatePtr
    );

#if (FRIF_WAKEUP_API_ENABLE == STD_ON)
/**
 * \brief   Selects a channel for wakeup pattern transmission.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 * \param[in] FrIf_ChnlIdx        Channel the wakeup pattern should be transmitted on.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x09}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_SetWakeupChannel
    (
        uint8 FrIf_CtrlIdx,
        Fr_ChannelType FrIf_ChnlIdx
    );
#endif /* (FRIF_WAKEUP_API_ENABLE == STD_ON) */

#if (FRIF_WAKEUP_API_ENABLE == STD_ON)
/**
 * \brief   Invokes CHI command 'WAKEUP'.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x0A}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_SendWUP
    (
        uint8 FrIf_CtrlIdx
    );
#endif /* (FRIF_WAKEUP_API_ENABLE == STD_ON) */

/**
 * \brief   Reads FlexRay CC POC-status.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx             FlexRay controller index.
 * \param[out] FrIf_POCStatusPtr       Address the POC-status is written to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x0D}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetPOCStatus
    (
        uint8 FrIf_CtrlIdx,
        P2VAR(Fr_POCStatusType,AUTOMATIC,FRIF_APPL_DATA) FrIf_POCStatusPtr
    );


/**
 * \brief   Reads the FlexRay clusters global time.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx            FlexRay controller index.
 * \param[out] FrIf_CyclePtr          Address to write the current cycle counter value to.
 * \param[out] FrIf_MacroTickPtr      Address to write the current macrotick counter value to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x0E}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetGlobalTime
    (
        uint8 FrIf_CtrlIdx,
        P2VAR(uint8,AUTOMATIC,FRIF_APPL_DATA) FrIf_CyclePtr,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) FrIf_MacroTickPtr
    );

#if (FRIF_GETNMVECTOR_API_ENABLE == STD_ON)
/**
 * \brief   Reads the FlexRay NM-Vector of the last FlexRay cycle.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the driver service Fr_GetNmVector().
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_NmVectorPtr is NULL_PTR,
 * FRIF_E_INV_POINTER is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx            FlexRay controller index.
 * \param[out] FrIf_NmVectorPtr       Address to write the Nm-Vector to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x0F}
 * \Reentrancy{Non Reentrant for same FlexRay controller}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetNmVector
    (
        uint8 FrIf_CtrlIdx,
        P2VAR(uint8,AUTOMATIC,FRIF_APPL_DATA) FrIf_NmVectorPtr
    );
#endif /* (FRIF_GETNMVECTOR_API_ENABLE == STD_ON) */

/**
 * \brief  Get macrotick duration
 *
 * This service returns the configured macrotick time of the cluster that the controller requested
 * is part of. The time returned is in units of nanoseconds.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and 0 returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and 0 returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 *
 * \return  Macrotick length in units of nanoseconds
 *
 * \ServiceID{0x31}
 * \Reentrancy{Re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(uint16,FRIF_CODE) FrIf_GetMacrotickDuration
    (
        uint8 FrIf_CtrlIdx
    );

/**
 * \brief  This API returns the configured time of the configuration parameter "GdCycle"
 *         in nanoseconds for the FlexRay controller with index FrIf_CtrlIdx.
 *
 * \param FrIf_CtrlIdx (in)     Index of the FlexRay CC to address.
 *
 * \retval                      Time in unit of nanoseconds
 *
 * \ServiceID{0x3A}
 * \Reentrancy{Re-entrant for different values of FrIf_TxPduId only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(uint32,FRIF_CODE) FrIf_GetCycleLength
    (
        uint8 FrIf_CtrlIdx
    );

#if (FRIF_GETCHANNELSTATUS_API_ENABLE == STD_ON)
/**
 * \brief   Returns the FlexRay aggregated channel status.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in]  FrIf_CtrlIdx           FlexRay controller index.
 * \param[out] FrIf_ChannelStatusPtr  Address to write the aggregated channel status to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x26}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetChannelStatus
(
    uint8 FrIf_CtrlIdx,
    P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) FrIf_ChannelAStatusPtr,
    P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) FrIf_ChannelBStatusPtr
);
#endif /* (FRIF_GETCHANNELSTATUS_API_ENABLE == STD_ON) */

#if (FRIF_GETNUMOFSTARTUPFRAMES_API_ENABLE == STD_ON)
/**
 * \brief  Wraps the FlexRay Driver API function Fr_GetNumOfStartupFrames and gets
 *         the current number of startup frames seen on the cluster.
 *
 * \param FrIf_CtrlIdx (in)                  FlexRay controller index.
 * \param FrIf_NumOfStartupFramesPtr (out)   FlexRay active star branch index.
 *
 * \retval  E_OK                Function serviced successfully.
 * \retval  E_NOT_OK            Function execution failed.
 *
 * \ServiceID{0x34}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetNumOfStartupFrames
    (
        uint8 FrIf_CtrlIdx,
        P2VAR(uint8,AUTOMATIC,FRIF_APPL_DATA) FrIf_NumOfStartupFramesPtr
    );
#endif /* FRIF_GETNUMOFSTARTUPFRAMES_API_ENABLE == STD_ON */


/**
 * \brief   Sets an absolute timer.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 * \param[in] FrIf_AbsTimerIdx    Absolute timer index.
 * \param[in] FrIf_Cycle          Communication Cycle the alarm should elapse.
 * \param[in] FrIf_Offset         Macrotick offset the alarm should elapse.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x19}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_SetAbsoluteTimer
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_AbsTimerIdx,
        uint8 FrIf_Cycle,
        uint16 FrIf_Offset
    );

/**
 * \brief   Cancels the absolute timer.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 * \param[in] FrIf_AbsTimerIdx       Absolute timer index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x1B}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_CancelAbsoluteTimer
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_AbsTimerIdx
    );

/**
 * \brief   Enables the absolute timer IRQ.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 * \param[in] FrIf_AbsTimerIdx       Absolute timer index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x1D}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_EnableAbsoluteTimerIRQ
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_AbsTimerIdx
    );

#if (FRIF_ENABLETRANSCEIVERBRANCH_API_ENABLE == STD_ON)
/**
 * \brief  Wraps the FlexRay Transceiver Driver API function FrTrcv_EnableTransceiverBranch.
 *
 * \param FrIf_CtrlIdx (in)     FlexRay controller index.
 * \param FrIf_ChnlIdx (in)     FlexRay channel index.
 * \param FrIf_BranchIdx (in)   FlexRay active star branch index.
 *
 * \retval  E_OK                Function serviced successfully.
 * \retval  E_NOT_OK            Function execution failed.
 *
 * \ServiceID{0x36}
 * \Reentrancy{Re-entrant}
 * \Synchronicity{Synchronous}
 */
FUNC(Std_ReturnType,FRIF_CODE) FrIf_EnableTransceiverBranch
    (
        uint8 FrIf_CtrlIdx,
        Fr_ChannelType FrIf_ChnlIdx,
        uint8 FrIf_BranchIdx
    );
#endif /* FRIF_ENABLETRANSCEIVERBRANCH_API_ENABLE == STD_ON */

#if (FRIF_GETIRQSTATUS_API_ENABLE == STD_ON)
/**
 * \brief   Reads the absolute timer IRQ status.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 * \param[in] FrIf_AbsTimerIdx       Absolute timer index.
 * \param[out] FrIf_IRQStatusPtr    Address the timer IRQ status is stored to
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x1F}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetAbsoluteTimerIRQStatus
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_AbsTimerIdx,
        P2VAR(boolean,AUTOMATIC,FRIF_APPL_DATA) FrIf_IRQStatusPtr
    );
#endif /* FRIF_GETIRQSTATUS_API_ENABLE == STD_ON */

/**
 * \brief   Acknowledges the absolute timer IRQ.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 * \param[in] FrIf_AbsTimerIdx       Absolute timer index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x21}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_AckAbsoluteTimerIRQ
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_AbsTimerIdx
    );

#if (FRIF_DISABLEIRQ_API_ENABLE == STD_ON)
/**
 * \brief   Disables the absolute timer IRQ.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 * \param[in] FrIf_AbsTimerIdx       Absolute timer index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x23}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_DisableAbsoluteTimerIRQ
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_AbsTimerIdx
    );
#endif /* (FRIF_DISABLEIRQ_API_ENABLE == STD_ON) */

#if (FRIF_DISABLETRANSCEIVERBRANCH_API_ENABLE == STD_ON)
/**
 * \brief  Wraps the FlexRay Transceiver Driver API function FrTrcv_DisableTransceiverBranch.
 *
 * \param FrIf_CtrlIdx (in)     FlexRay controller index.
 * \param FrIf_ChnlIdx (in)     FlexRay channel index.
 * \param FrIf_BranchIdx (in)   FlexRay active star branch index.
 *
 * \retval  E_OK                Function serviced successfully.
 * \retval  E_NOT_OK            Function execution failed.
 *
 * \ServiceID{0x37}
 * \Reentrancy{Re}
 * \Synchronicity{Synchronous}
 */
FUNC(Std_ReturnType,FRIF_CODE) FrIf_DisableTransceiverBranch
    (
        uint8 FrIf_CtrlIdx,
        Fr_ChannelType FrIf_ChnlIdx,
        uint8 FrIf_BranchIdx
    );
#endif /* FRIF_DISABLETRANSCEIVERBRANCH_API_ENABLE == STD_ON */


/**
 * \brief  Transmits a Pdu
 *
 * Tjis service allows upper layers to request the transmission of of Pdus via the FlexRay
 * communication system.
 *
 * In case of an immediate transmission (single Pdu per Frame, no Update-Bit), the Pdu payload data
 * is immediately passed to the FlexRay drivers transmit service.
 *
 * In case of decoupled transmission this service just remembers the transmission request and carries out
 * the transmission within th econtext of the joblist execution.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_TxPduId contains an invalid value
 * FRIF_E_INV_TXPDUID is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and the Pdu to transmit is an immediate Tx-Pdu and FrIf_PduInfoPtr or
 * FrIf_PduInfoPtr->SduDataPtr is NULL_PTR,
 * FRIF_E_INV_POINTER is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and the Pdu to transmit is dynamic length Pdu and FrIf_PduInfoPtr
 * is NULL_PTR, FRIF_E_INV_POINTER is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_TxPduId          Id of Pdu to be transmitted.
 * \param[in] FrIf_PduInfoPtr       Description of Pdu content to be transmitted.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x12}
 * \Reentrancy{Re-entrant for different values of FrIf_TxPduId only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_Transmit
    (
        PduIdType FrIf_TxPduId,
        P2CONST(PduInfoType,AUTOMATIC,FRIF_APPL_DATA) FrIf_PduInfoPtr
    );


/**
 * \brief  Sets the transceiver's mode.
 *
 * This service translates the FrIf_CtrlIdx and FrIf_ChnlIdx to the configured FlexRay transceiver
 * driver and FlexRay transceiver driver transceiver index and calls the equivalent transceiver driver
 * service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_ChnlIdx contains an invalid value
 * FRIF_E_INV_CHNL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx      FlexRay controller index.
 * \param[in] FrIf_ChnlIdx      FlexRay channel index.
 * \param[in] FrIf_TrcvMode     Transceiver Mode to set.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x13}
 * \Reentrancy{Re-entrant for different value pairs of FrIf_CtrlIdx/FrIf_ChnlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_SetTransceiverMode
    (
        uint8 FrIf_CtrlIdx,
        Fr_ChannelType FrIf_ChnlIdx,
        FrTrcv_TrcvModeType FrIf_TrcvMode
    );

#if (FRIF_GETTRANSCEIVERMODE_API_ENABLE == STD_ON)
/**
 * \brief  Receive the transceiver's current mode.
 *
 * This service translates the FrIf_CtrlIdx and FrIf_ChnlIdx to the configured FlexRay transceiver
 * driver and FlexRay transceiver driver transceiver index and calls the equivalent transceiver driver
 * service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_ChnlIdx contains an invalid value
 * FRIF_E_INV_CHNL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx      FlexRay controller index.
 * \param[in] FrIf_ChnlIdx      FlexRay channel index.
 * \param[out] FrIf_TrcvModePtr     Address to write the transceiver mode to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x14}
 * \Reentrancy{Re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetTransceiverMode
    (
        uint8 FrIf_CtrlIdx,
        Fr_ChannelType FrIf_ChnlIdx,
        P2VAR(FrTrcv_TrcvModeType,AUTOMATIC,FRIF_APPL_DATA) FrIf_TrcvModePtr
    );
#endif /* (FRIF_GETTRANSCEIVERMODE_API_ENABLE == STD_ON) */

#if (FRIF_GETTRANSCEIVERWUREASON_API_ENABLE == STD_ON)
/**
 * \brief  Returns the transceivers wakeup reason.
 *
 * This service translates the FrIf_CtrlIdx and FrIf_ChnlIdx to the configured FlexRay transceiver
 * driver and FlexRay transceiver driver transceiver index and calls the equivalent transceiver driver
 * service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_ChnlIdx contains an invalid value
 * FRIF_E_INV_CHNL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx      FlexRay controller index.
 * \param[in] FrIf_ChnlIdx      FlexRay channel index.
 * \param[out] FrIf_TrcvWUReasonPtr     Address to write the transceiver WU Reason to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x15}
 * \Reentrancy{Re-entrant for different value pairs of FrIf_CtrlIdx/FrIf_ChnlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetTransceiverWUReason
    (
        uint8 FrIf_CtrlIdx,
        Fr_ChannelType FrIf_ChnlIdx,
        P2VAR(FrTrcv_TrcvWUReasonType,AUTOMATIC,FRIF_APPL_DATA) FrIf_TrcvWUReasonPtr
    );
#endif /* (FRIF_GETTRANSCEIVERWUREASON_API_ENABLE == STD_ON) */

#if (FRIF_WAKEUPCONTROL_API_ENABLE == STD_ON)
/**
 * \brief  Clears the transceiver driver's wakeup information.
 *
 * This service translates the FrIf_CtrlIdx and FrIf_ChnlIdx to the configured FlexRay transceiver
 * driver and FlexRay transceiver driver transceiver index and calls the equivalent transceiver driver
 * service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_ChnlIdx contains an invalid value
 * FRIF_E_INV_CHNL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx      FlexRay controller index.
 * \param[in] FrIf_ChnlIdx      FlexRay channel index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x18}
 * \Reentrancy{Re-entrant for different value pairs of FrIf_CtrlIdx/FrIf_ChnlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_ClearTransceiverWakeup
    (
        uint8 FrIf_CtrlIdx,
        Fr_ChannelType FrIf_ChnlIdx
    );
#endif /* (FRIF_WAKEUPCONTROL_API_ENABLE == STD_ON) */

#if (FRIF_CHECKWAKEUPBYTRANSCEIVER_API_ENABLE == STD_ON)
/**
 * \brief  Service checks for wakeup events by the transceiver
 *
 * This service translates the FrIf_CtrlIdx and FrIf_ChnlIdx to the configured FlexRay transceiver
 * driver and FlexRay transceiver driver transceiver index and calls the equivalent transceiver driver
 * service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_ChnlIdx contains an invalid value
 * FRIF_E_INV_CHNL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx      FlexRay controller index.
 * \param[in] FrIf_ChnlIdx      FlexRay channel index.
 *
 * \ServiceID{0x39}
 * \Reentrancy{Re-entrant for different value pairs of FrIf_CtrlIdx/FrIf_ChnlIdx}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void,FRIF_CODE) FrIf_CheckWakeupByTransceiver
    (
        uint8 FrIf_CtrlIdx,
        Fr_ChannelType FrIf_ChnlIdx
    );
#endif /* (FRIF_CHECKWAKEUPBYTRANSCEIVER_API_ENABLE == STD_ON) */

/**
 * \brief Dispatches the communication operations within a communication job
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_JobIdx contains an invalid value
 * FRIF_E_INV_JOB_IDX is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_ClstIdx contains an invalid value
 * FRIF_E_INV_CLST_IDX is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_Cycle contains an invalid value
 * FRIF_E_INV_CYCLE is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_ClstIdx            FlexRay cluster index.
 * \param[in] FrIf_JobIdx             FlexRay job index.
 * \param[in] FrIf_Cycle              FlexRay cycle number.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \Reentrancy{Non Reentrant for same FlexRay cluster}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_DispatchComOps
    (
        uint8 FrIf_ClstIdx,
        uint16 FrIf_JobIdx,
        uint8 FrIf_Cycle
    );

#if (FRIF_READCCCONFIG_API_ENABLE == STD_ON)
/**
 * \brief  Wraps the FlexRay Driver API function Fr_ReadCCConfig().
 *
 * \param FrIf_CtrlIdx (in)                 FlexRay controller index.
 * \param FrIf_ConfigParamIdx (in)          FlexRay configuration parameter to read.
 * \param FrIf_ConfigParamValuePtr (out)    Pointer to the localtion where output value
 *                                          will be stored.
 *
 * \retval  E_OK                Function serviced successfully.
 * \retval  E_NOT_OK            Function execution failed.
 *
 * \ServiceID{0x3B}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_ReadCCConfig
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_ConfigParamIdx,
        P2VAR(uint32,AUTOMATIC,FRIF_APPL_DATA) FrIf_ConfigParamValuePtr
    );
#endif /* FRIF_READCCCONFIG_API_ENABLE */

#if (FRIF_ALLSLOTS_API_ENABLE == STD_ON)
/**
 * \brief   Invokes CHI command 'ALL_SLOTS'.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x33}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_AllSlots
(
    uint8 FrIf_CtrlIdx
);
#endif /* (FRIF_ALLSLOTS_API_ENABLE == STD_ON) */

#if (FRIF_RECONFIGLPDU_API_ENABLE == STD_ON)
/**
 * \brief   Dynamically reconfigures a LPdu.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx         FlexRay controller index.
 * \param[in] FrIf_LPduIdx         LPdu index.
 * \param[in] FrIf_FrameId         FlexRay frame ID.
 * \param[in] FrIf_ChnlIdx         FlexRay Channel.
 * \param[in] FrIf_CycleRepetition Cycle Repetition pattern.
 * \param[in] FrIf_CycleOffset     Cycle Offset part of the cycle filter mechanism FrIf_LPdu shall
 *                                 be configured to.
 * \param[in] FrIf_PayloadLength   Payload length in units of bytes.
 * \param[in] FrIf_HeaderCRC       FlexRay Header CRC.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x00}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_ReconfigLPdu
(
    uint8 FrIf_CtrlIdx,
    uint16 FrIf_LPduIdx,
    uint16 FrIf_FrameId,
    Fr_ChannelType FrIf_ChnlIdx,
    uint8 FrIf_CycleRepetition,
    uint8 FrIf_CycleOffset,
    uint8 FrIf_PayloadLength,
    uint16 FrIf_HeaderCRC
);
#endif /* (FRIF_RECONFIGLPDU_API_ENABLE == STD_ON) */

#if (FRIF_DISABLELPDU_API_ENABLE == STD_ON)
/**
 * \brief   Dynamically disables a LPdu.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx         FlexRay controller index.
 * \param[in] FrIf_LPduIdx         LPdu index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x28}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_DisableLPdu
(
    uint8 FrIf_CtrlIdx,
    uint16 FrIf_LPduIdx
);
#endif /* (FRIF_DISABLELPDU_API_ENABLE == STD_ON) */

#if (FRIF_GETWAKEUPRXSTATUS_API_ENABLE == STD_ON)
/**
 * \brief   Reads the wakeup rx status of the FlexRay controller.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx            FlexRay controller index.
 * \param[out] FrIf_WakeupRxStatusPtr       Address to write the wakeup rx status to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x2B}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetWakeupRxStatus
(
    uint8 FrIf_CtrlIdx,
    P2VAR(uint8,AUTOMATIC,FRIF_APPL_DATA) FrIf_WakeupRxStatusPtr
);
#endif /* (FRIF_GETWAKEUPRXSTATUS_API_ENABLE == STD_ON) */

#if (FRIF_GETCLOCKCORRECTION_API_ENABLE == STD_ON)
/**
 * \brief   Returns the FlexRay clock correction terms.
 *
 * This service reads the FlexRay clock correction terms and writes them
 * into *FrIf_RateCorrectionPtr and *FrIf_RateCorrectionPtr.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx                 FlexRay controller index.
 * \param[out] FrIf_RateCorrectionPtr      Address to write the rate correction value to.
 * \param[out] FrIf_OffsetCorrectionPtr    Address to write the offset correction value to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x29}
 * \Reentrancy{Non Reentrant for same FlexRay controller}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetClockCorrection
    (
        uint8 FrIf_CtrlIdx,
        P2VAR(sint16,AUTOMATIC,FRIF_APPL_DATA) FrIf_RateCorrectionPtr,
        P2VAR(sint32,AUTOMATIC,FRIF_APPL_DATA) FrIf_OffsetCorrectionPtr
    );
#endif /* (FRIF_GETCLOCKCORRECTION_API_ENABLE == STD_ON) */

#if (FRIF_GETSYNCFRAMELIST_API_ENABLE == STD_ON)
/**
 * \brief   Returns a list of sync frames.
 *
 * This service writes a list of sync frames observed in the last even/odd communication cycle couple
 * into *FrIf_ChannelAEvenListPtr, *FrIf_ChannelBEvenListPtr, *FrIf_ChannelAOddListPtr and
 * *FrIf_ChannelBOddListPtr
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx                 FlexRay controller index.
 * \param[in] FrIf_ListSize                Size of list passed to the output pointers.
 * \param[out] FrIf_ChannelAEvenListPtr    Address to write the list of even sync frames of channel A.
 * \param[out] FrIf_ChannelBEvenListPtr    Address to write the list of even sync frames of channel B.
 * \param[out] FrIf_ChannelAOddListPtr     Address to write the list of odd sync frames of channel A.
 * \param[out] FrIf_ChannelBOddListPtr     Address to write the list of odd sync frames of channel B.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x2A}
 * \Reentrancy{Non Reentrant for same FlexRay controller}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetSyncFrameList
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_ListSize,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) FrIf_ChannelAEvenListPtr,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) FrIf_ChannelBEvenListPtr,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) FrIf_ChannelAOddListPtr,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) FrIf_ChannelBOddListPtr
    );
#endif /* (FRIF_GETSYNCFRAMELIST_API_ENABLE == STD_ON) */

#if (FRIF_GETTRANSCEIVERERROR_API_ENABLE == STD_ON)
/**
 * \brief  Returns the transceivers error status.
 *
 * This service returns the FlexRay transceiver's error status.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_ChnlIdx contains an invalid value
 * FRIF_E_INV_CHNL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in]   FrIf_CtrlIdx        FlexRay controller index.
 * \param[in]   FrIf_ChnlIdx        FlexRay channel index.
 * \param[in]   FrIf_BranchIdx      FlexRay branch index (active star).
 * \param[out]  FrIf_BusErrorState  Address to write the transceiver error status to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x35}
 * \Reentrancy{Non Reentrant for same transceiver}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetTransceiverError
    (
        uint8 FrIf_CtrlIdx,
        Fr_ChannelType FrIf_ChnlIdx,
        uint8 FrIf_BranchIdx,
        P2VAR(uint32,AUTOMATIC,FRIF_APPL_DATA) FrIf_BusErrorState
    );
#endif /* (FRIF_GETTRANSCEIVERERROR_API_ENABLE == STD_ON) */


/**
 * \brief   Retunrs macroticks per communication cycle.
 *
 * This service returns the number of macroticks a single FlexRay communication cycle of the FlexRay
 * cluster the requested controller is joining consists of.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx            FlexRay controller index.
 *
 * \return Duration of a communication cycle in units of macroticks.
 *
 * \ServiceID{0x11}
 * \Reentrancy{Non Reentrant for same FlexRay controller}
 * \Synchronicity{Synchronous}
 */
extern FUNC(uint16,FRIF_CODE) FrIf_GetMacroticksPerCycle
    (
        uint8 FrIf_CtrlIdx
    );

#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
/**
 * \brief  Enables or disables mirroring for all FlexRay controllers connected to the addressed FlexRay cluster.
 *
 * \param FrIf_ClstIdx (in)                 Index of the FlexRay cluster to address.
 * \param FrIf_MirroringActive (in)         TRUE: Mirror_ReportFlexRayFrame will be called for each
 *                                                frame received or transmitted on the addressed FlexRay CC.
 *                                          FALSE: Mirror_ReportFlexRayFrame will not be called for
 *                                                 the addressed FlexRay CC.
 *
 * \retval  E_OK                Mirroring mode was changed.
 * \retval  E_NOT_OK            Wrong FrIf_CtrlIdx, or mirroring is globally disabled (see FrIfBusMirroringSupport).
 *
 * \ServiceID{0x4B}
 * \Reentrancy{Re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_EnableBusMirroring
    (
        uint8 FrIf_ClstIdx,
        boolean FrIf_MirroringActive
    );
#endif /* FRIF_BUSMIRRORINGSUPPORT_ENABLE */


/* start code section declaration */
#define FRIF_STOP_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */


#endif /* FRIF_API_H */

