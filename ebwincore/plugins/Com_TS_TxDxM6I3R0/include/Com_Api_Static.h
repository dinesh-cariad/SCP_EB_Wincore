/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* mgcc62 Deviation List
 *
 * MGCC62-1) Deviated Rule: redundant-decls
 * warning: redundant redeclaration of 'Com_MainFunctionTx' [-Wredundant-decls]
 *
 * Reason:
 * AUTOSAR declares Com_MainFunctionTx also in SchM_Com.h
 *
 * MGCC62-2) Deviated Rule: redundant-decls
 * warning: redundant redeclaration of 'Com_MainFunctionRouteSignals' [-Wredundant-decls]
 *
 * Reason:
 * AUTOSAR declares Com_MainFunctionRouteSignals also in SchM_Com.h
 *
 * MGCC62-3) Deviated Rule: redundant-decls
 * warning: redundant redeclaration of 'Com_MainFunctionRx' [-Wredundant-decls]
 *
 * Reason:
 * AUTOSAR declares Com_MainFunctionRx also in SchM_Com.h
 */

#ifndef COM_API_STATIC_H
#define COM_API_STATIC_H


/*==================[inclusions]=============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <TSAtomic_Assign.h>    /* Header for atomic assignment macros */
#include <ComStack_Types.h>     /* Com-stack types */
#include <Com_Rules.h>          /* Generated pre-compile time config and derived rules */
#include <Com_Types.h>          /* types of configuration elements */
#include <Com_Int_Types.h>      /* types of configuration elements */

/*==================[macros]=================================================*/

/*----------- [ service Ids ] -------------*/


/** \brief Definition of constant COMServiceId_Init */
#if (defined COMServiceId_Init) /* To prevent double declaration */
#error COMServiceId_Init already defined
#endif /* if (defined COMServiceId_Init) */

/** \brief Define COMServiceId_Init */
#define COMServiceId_Init 0x01U


/** \brief Definition of constant COMServiceId_DeInit */
#if (defined COMServiceId_DeInit)       /* To prevent double declaration */
#error COMServiceId_DeInit already defined
#endif /* if (defined COMServiceId_DeInit) */

/** \brief Define COMServiceId_DeInit */
#define COMServiceId_DeInit 0x02U

/** \brief Definition of constant COMServiceId_IpduGroupControl */
#if (defined COMServiceId_IpduGroupControl)       /* To prevent double declaration */
#error COMServiceId_IpduGroupControl already defined
#endif /* if (defined COMServiceId_IpduGroupControl) */

/** \brief Define COMServiceId_IpduGroupControl */
#define COMServiceId_IpduGroupControl 0x03U

/** \brief Definition of constant COMServiceId_ReceptionDMControl */
#if (defined COMServiceId_ReceptionDMControl)    /* To prevent double declaration */
#error COMServiceId_ReceptionDMControl already defined
#endif /* if (defined COMServiceId_EnableReceptionDM) */

/** \brief Define COMServiceId_EnableReceptionDM */
#define COMServiceId_ReceptionDMControl 0x06U

/** \brief Definition of constant COMServiceId_GetStatus */
#if (defined COMServiceId_GetStatus)    /* To prevent double declaration */
#error COMServiceId_GetStatus already defined
#endif /* if (defined COMServiceId_GetStatus) */

/** \brief Define COMServiceId_GetStatus */
#define COMServiceId_GetStatus 0x07U

/** \brief Definition of constant COMServiceId_GetConfigurationId */
#if (defined COMServiceId_GetConfigurationId)   /* To prevent double declaration */
#error COMServiceId_GetConfigurationId already defined
#endif /* if (defined COMServiceId_GetConfigurationId) */

/** \brief Define COMServiceId_GetConfigurationId */
#define COMServiceId_GetConfigurationId 0x08U

/** \brief Definition of constant COMServiceId_GetVersionInfo */
#if (defined COMServiceId_GetVersionInfo)       /* To prevent double declaration */
#error COMServiceId_GetVersionInfo already defined
#endif /* if (defined COMServiceId_GetVersionInfo) */

/** \brief Define COMServiceId_GetVersionInfo */
#define COMServiceId_GetVersionInfo 0x09U

/** \brief Definition of constant COMServiceId_SendSignal */
#if (defined COMServiceId_SendSignal)   /* To prevent double declaration */
#error COMServiceId_SendSignal already defined
#endif /* if (defined COMServiceId_SendSignal) */

/** \brief Define COMServiceId_SendSignal */
#define COMServiceId_SendSignal 0x0AU

/** \brief Definition of constant COMServiceId_ReceiveSignal */
#if (defined COMServiceId_ReceiveSignal)        /* To prevent double declaration */
#error COMServiceId_ReceiveSignal already defined
#endif /* if (defined COMServiceId_ReceiveSignal) */

/** \brief Define COMServiceId_ReceiveSignal */
#define COMServiceId_ReceiveSignal 0x0BU

/** \brief Definition of constant COMServiceId_UpdateShadowSignal */
#if (defined COMServiceId_UpdateShadowSignal)   /* To prevent double declaration */
#error COMServiceId_UpdateShadowSignal already defined
#endif /* if (defined COMServiceId_UpdateShadowSignal) */

/** \brief Define COMServiceId_UpdateShadowSignal */
#define COMServiceId_UpdateShadowSignal 0x0CU

/** \brief Definition of constant COMServiceId_SendSignalGroup */
#if (defined COMServiceId_SendSignalGroup)      /* To prevent double declaration */
#error COMServiceId_SendSignalGroup already defined
#endif /* if (defined COMServiceId_SendSignalGroup) */

/** \brief Define COMServiceId_SendSignalGroup */
#define COMServiceId_SendSignalGroup 0x0DU

/** \brief Definition of constant COMServiceId_ReceiveSignalGroup */
#if (defined COMServiceId_ReceiveSignalGroup)   /* To prevent double declaration */
#error COMServiceId_ReceiveSignalGroup already defined
#endif /* if (defined COMServiceId_ReceiveSignalGroup) */

/** \brief Define COMServiceId_ReceiveSignalGroup */
#define COMServiceId_ReceiveSignalGroup 0x0EU

/** \brief Definition of constantDefinition of constant COMServiceId_ReceiveShadowSignal */
#if (defined COMServiceId_ReceiveShadowSignal)  /* To prevent double declaration */
#error COMServiceId_ReceiveShadowSignal already defined
#endif /* if (defined COMServiceId_ReceiveShadowSignal) */

/** \brief Define COMServiceId_ReceiveShadowSignal */
#define COMServiceId_ReceiveShadowSignal 0x0FU

/** \brief Definition of constant COMServiceId_InvalidateSignal */
#if (defined COMServiceId_InvalidateSignal)     /* To prevent double declaration */
#error COMServiceId_InvalidateSignal already defined
#endif /* if (defined COMServiceId_InvalidateSignal) */

/** \brief Define COMServiceId_InvalidateSignal */
#define COMServiceId_InvalidateSignal 0x10U

/** \brief Definition of constant COMServiceId_InvalidateShadowSignal */
#if (defined COMServiceId_InvalidateShadowSignal)       /* To prevent double declaration */
#error COMServiceId_InvalidateShadowSignal already defined
#endif /* if (defined COMServiceId_InvalidateShadowSignal) */

/** \brief Define COMServiceId_InvalidateShadowSignal */
#define COMServiceId_InvalidateShadowSignal 0x16U

/** \brief Definition of constant COMServiceId_TriggerIPDUSend */
#if (defined COMServiceId_TriggerIPDUSend)      /* To prevent double declaration */
#error COMServiceId_TriggerIPDUSend already defined
#endif /* if (defined COMServiceId_TriggerIPDUSend) */

/** \brief Define COMServiceId_TriggerIPDUSend */
#define COMServiceId_TriggerIPDUSend 0x17U

/** \brief Definition of constant COMServiceId_MainFunctionRx */
#if (defined COMServiceId_MainFunctionRx)       /* To prevent double declaration */
#error COMServiceId_MainFunctionRx already defined
#endif /* if (defined COMServiceId_MainFunctionRx) */

/** \brief Define COMServiceId_MainFunctionRx */
#define COMServiceId_MainFunctionRx 0x18U

/** \brief Definition of constant COMServiceId_MainFunctionTx */
#if (defined COMServiceId_MainFunctionTx)       /* To prevent double declaration */
#error COMServiceId_MainFunctionTx already defined
#endif /* if (defined COMServiceId_MainFunctionTx) */

/** \brief Define COMServiceId_MainFunctionTx */
#define COMServiceId_MainFunctionTx 0x19U

/** \brief Definition of constant COMServiceId_MainFunctionRouteSignals */
#if (defined COMServiceId_MainFunctionRouteSignals)     /* To prevent double declaration */
#error COMServiceId_MainFunctionRouteSignals already defined
#endif /* if (defined COMServiceId_MainFunctionRouteSignals) */

/** \brief Define COMServiceId_MainFunctionRouteSignals */
#define COMServiceId_MainFunctionRouteSignals 0x1AU

/** \brief Definition of constant COMServiceId_InvalidateSignalGroup */
#if (defined COMServiceId_InvalidateSignalGroup)      /* To prevent double declaration */
#error COMServiceId_InvalidateSignalGroup already defined
#endif /* if (defined COMServiceId_InvalidateSignalGroup) */

/** \brief Define COMServiceId_InvalidateSignalGroup */
#define COMServiceId_InvalidateSignalGroup 0x1BU

/** \brief Definition of constant COMServiceId_ClearIpduGroupVector */
#if (defined COMServiceId_ClearIpduGroupVector)     /* To prevent double declaration */
#error COMServiceId_ClearIpduGroupVector already defined
#endif /* if (defined COMServiceId_ClearIpduGroupVector) */

/** \brief Define COMServiceId_ClearIpduGroupVector */
#define COMServiceId_ClearIpduGroupVector 0x1CU

/** \brief Definition of constant COMServiceId_SetIpduGroup */
#if (defined COMServiceId_SetIpduGroup)     /* To prevent double declaration */
#error COMServiceId_SetIpduGroup already defined
#endif /* if (defined COMServiceId_SetIpduGroup) */

/** \brief Define COMServiceId_SetIpduGroup */
#define COMServiceId_SetIpduGroup 0x1DU

/** \brief Definition of constant COMServiceId_TpRxIndication */
#if (defined COMServiceId_TpRxIndication)     /* To prevent double declaration */
#error COMServiceId_TpRxIndication already defined
#endif /* if (defined COMServiceId_TpRxIndication) */

/** \brief Define COMServiceId_TpRxIndication */
#define COMServiceId_TpRxIndication 0x1EU

/** \brief Definition of constant COMServiceId_SendDynSignal */
#if (defined COMServiceId_SendDynSignal)     /* To prevent double declaration */
#error COMServiceId_SendDynSignal already defined
#endif /* if (defined COMServiceId_SendDynSignal) */

/** \brief Define COMServiceId_SendDynSignal */
#define COMServiceId_SendDynSignal 0x21

/** \brief Definition of constant COMServiceId_ReceiveDynSignal */
#if (defined COMServiceId_ReceiveDynSignal)     /* To prevent double declaration */
#error COMServiceId_ReceiveDynSignal already defined
#endif /* if (defined COMServiceId_ReceiveDynSignal) */

/** \brief Define COMServiceId_ReceiveDynSignal */
#define COMServiceId_ReceiveDynSignal 0x22

/** \brief Definition of constant COMServiceId_CopyRxData */
#if (defined COMServiceId_CopyRxData)     /* To prevent double declaration */
#error COMServiceId_CopyRxData already defined
#endif /* if (defined COMServiceId_CopyRxData) */

/** \brief Define COMServiceId_CopyRxData */
#define COMServiceId_CopyRxData 0x23

/** \brief Definition of constant COMServiceId_CopyRxData */
#if (defined COMServiceId_CopyTxData)     /* To prevent double declaration */
#error COMServiceId_CopyTxData already defined
#endif /* if (defined COMServiceId_CopyTxData) */

/** \brief Define COMServiceId_CopyTxData */
#define COMServiceId_CopyTxData 0x24

/** \brief Definition of constant COMServiceId_StartOfReception */
#if (defined COMServiceId_StartOfReception)     /* To prevent double declaration */
#error COMServiceId_StartOfReception already defined
#endif /* if (defined COMServiceId_StartOfReception) */

/** \brief Define COMServiceId_StartOfReception */
#define COMServiceId_StartOfReception 0x25

/** \brief Definition of constant COMServiceId_TpTxConfirmation */
#if (defined COMServiceId_TpTxConfirmation)     /* To prevent double declaration */
#error COMServiceId_TpTxConfirmation already defined
#endif /* if (defined COMServiceId_TpTxConfirmation) */

/** \brief Define COMServiceId_TpTxConfirmation */
#define COMServiceId_TpTxConfirmation 0x26

/** \brief Definition of constant COMServiceId_SwitchIpduTxMode */
#if (defined COMServiceId_SwitchIpduTxMode)      /* To prevent double declaration */
#error COMServiceId_SwitchIpduTxMode already defined
#endif /* if (defined COMServiceId_SwitchIpduTxMode) */

/** \brief Define COMServiceId_SwitchIpduTxMode */
#define COMServiceId_SwitchIpduTxMode 0x27U

/** \brief Definition of constant COMServiceId_TxConfirmation */
#if (defined COMServiceId_TxConfirmation)       /* To prevent double declaration */
#error COMServiceId_TxConfirmation already defined
#endif /* if (defined COMServiceId_TxConfirmation) */

/** \brief Define COMServiceId_TxConfirmation */
#define COMServiceId_TxConfirmation 0x40U

/** \brief Definition of constant COMServiceId_TriggerTransmit */
#if (defined COMServiceId_TriggerTransmit)      /* To prevent double declaration */
#error COMServiceId_TriggerTransmit already defined
#endif /* if (defined COMServiceId_TriggerTransmit) */

/** \brief Define COMServiceId_TriggerTransmit */
#define COMServiceId_TriggerTransmit 0x41U

/** \brief Definition of constant COMServiceId_RxIndication */
#if (defined COMServiceId_RxIndication) /* To prevent double declaration */
#error COMServiceId_RxIndication already defined
#endif /* if (defined COMServiceId_RxIndication) */

/** \brief Define COMServiceId_RxIndication */
#define COMServiceId_RxIndication 0x42U

/** \brief Definition of constant COMServiceId_SendSignalGroupArray */
#if (defined COMServiceId_SendSignalGroupArray)      /* To prevent double declaration */
#error COMServiceId_SendSignalGroupArray already defined
#endif /* if (defined COMServiceId_SendSignalGroupArray) */

/** \brief Define COMServiceId_SendSignalGroupArray */
#define COMServiceId_SendSignalGroupArray 0x43U

/** \brief Definition of constant COMServiceId_ReceiveSignalGroupArray */
#if (defined COMServiceId_ReceiveSignalGroupArray)      /* To prevent double declaration */
#error COMServiceId_SendSignalGroupArray already defined
#endif /* if (defined COMServiceId_ReceiveSignalGroupArray) */

/** \brief Define COMServiceId_ReceiveSignalGroupArray */
#define COMServiceId_ReceiveSignalGroupArray 0x44U

/** \brief Definition of constant COMServiceId_InternalAPI */
#if (defined COMServiceId_InternalAPI)      /* To prevent double declaration */
#error COMServiceId_InternalAPI already defined
#endif /* if (defined COMServiceId_InternalAPI) */

/** \brief Define COMServiceId_InternalAPI */
#define COMServiceId_InternalAPI 0xFFU

/** \brief Definition of constant COMServiceId_GetRxIPduBuffer */
#if (defined COMServiceId_GetRxIPduBuffer)      /* To prevent double declaration */
#error COMServiceId_GetRxIPduBuffer already defined
#endif /* if (defined COMServiceId_GetRxIPduBuffer) */

/** \brief Define COMServiceId_GetRxIPduBuffer */
#define COMServiceId_GetRxIPduBuffer 0xFEU

/*----------- [ DET error codes ] -------------*/
/** \brief Definition of error code COM_E_PARAM */
#if (defined COM_E_PARAM)       /* To prevent double declaration */
#error COM_E_PARAM already defined
#endif /* if (defined COM_E_PARAM) */

/** \brief Define COM_E_PARAM */
#define COM_E_PARAM 1U

/** \brief Definition of error code COM_E_UNINIT */
#if (defined COM_E_UNINIT)      /* To prevent double declaration */
#error COM_E_UNINIT already defined
#endif /* if (defined COM_E_UNINIT) */

/** \brief Define COM_E_UNINIT */
#define COM_E_UNINIT 2U

/** \brief Definition of error code COM_E_PARAM_POINTER */
#if (defined COM_E_PARAM_POINTER)      /* To prevent double declaration */
#error COM_E_PARAM_POINTER already defined
#endif /* if (defined COM_E_PARAM_POINTER) */

/** \brief Define COM_E_PARAM_POINTER */
#define COM_E_PARAM_POINTER 3U

#if (defined COM_E_SIGNAL_TOO_WIDE)     /* To prevent double declaration */
#error COM_E_SIGNAL_TOO_WIDE already defined
#endif /* if (defined COM_E_SIGNAL_TOO_WIDE) */

/** \brief Define COM_E_SIGNAL_TOO_WIDE */
#define COM_E_SIGNAL_TOO_WIDE 0x21U

#if (defined COM_INSTANCE_ID)     /* To prevent double declaration */
#error COM_INSTANCE_ID already defined
#endif /* if (defined COM_INSTANCE_ID) */

/** \brief Com instance ID */
#define COM_INSTANCE_ID 0U

/*==================[type definitions]======================================*/

/** \brief definition of the Com_StatusType */
typedef enum
{
   COM_UNINIT = 0,
   COM_INIT = 1
} Com_StatusType;

/** \brief definition of the Com_ServiceIdType */
typedef uint8 Com_ServiceIdType;

/** \brief definition of the Com_SignalIdType */
typedef uint16 Com_SignalIdType;

/** \brief definition of the Com_PduGroupIdType */
typedef uint8 Com_PduGroupIdType;

/** \brief definition of the Com_SignalGroupIdType */
typedef uint16 Com_SignalGroupIdType;

/* Check_C disabled. Com_IpduGroupVector must not be renamed due to Com specification. */
/* CHECK: RULE 505 OFF */
/** \brief definition of the Com_IpduGroupVector */
typedef uint8 Com_IpduGroupVector[COM_IPDU_GROUP_VECTOR_NUM_BYTES];
/* CHECK: RULE 505 ON */

/** \brief definition of the Com_IpduGroupIdType */
typedef uint16 Com_IpduGroupIdType;

/*==================[external function declarations]=========================*/

#define COM_START_SEC_CODE
#include <Com_MemMap.h>


/**
 * \brief Com_Init - Initializes the Com module
 *
 * Function to initialize the Com module. First function to be called of Com.
 * The module calling the function Com_Init has to include Com_PBcfg.h.
 * The invocation of the Com_Init function is without usage of PbcfgM
 * Com_Init(&<short name of Com configuration>); or
 * Com_Init(&Com_ConfigLayout.Com_RootConfig);
 * The invocation of the Com_Init function for usage of PbcfgM
 * Com_Init(NULL_PTR);
 *
 * \param[in] Com_ConfigPtr Pointer to configuration structure that holds the
 * Com module post-build-time configuration data.
 *
 * \ServiceID{0x01}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE) Com_Init
(
   P2CONST(Com_ConfigType, AUTOMATIC,COM_APPL_CONST) Com_ConfigPtr
);


#if COM_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#define COM_START_SEC_CODE_CC_BLOCK
#include <Com_MemMap.h>
#endif

/**
 * \brief Com_IsValidConfig - Checks validity of the post-build configuration
 *
 ** Checks if the post build configuration is valid.
 ** A configuration is invalid if
 ** - the platform signature does not match.
 ** - the published information signature does not match.
 ** - the link time signature does not match.
 ** - the compile time signature does not match.
 ** - the function is called with a null pointer.
 **
 ** \param[in] ConfigPtr Pointer to configuration structure that holds the
 **                      Com module post-build-time configuration data.
 **
 ** \return Function execution success status
 ** \retval E_OK  the provided module configuration is valid
 ** \retval E_NOT_OK  the provided module configuration is invalid
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */

extern FUNC(Std_ReturnType, COM_CODE) Com_IsValidConfig
(
   P2CONST(void, AUTOMATIC, COM_APPL_CONST) ConfigPtr
);

#if COM_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define COM_STOP_SEC_CODE_CC_BLOCK
#include <Com_MemMap.h>

#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif

#if (COM_VERSION_INFO_API == STD_ON)
/* Com_GetVersionInfo() API function is enabled */

/**
 * \brief Returns the module version information.
 *
 * This service returns the version information of this module.
 *
 * \param[out] versionInfoPtr Address the version information should be written to.
 *
 * \ServiceID{0x09}
 * \Reentrancy{Re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE) Com_GetVersionInfo
(
   P2VAR(Std_VersionInfoType, AUTOMATIC, COM_APPL_DATA) versionInfoPtr
);
#elif (COM_VERSION_INFO_API == STD_OFF)

#if (defined Com_GetVersionInfo)        /* To prevent double declaration */
#error Com_GetVersionInfo is already defined
#endif /* (defined Com_GetVersionInfo) */
/*
 * Com_GetVersionInfo() API function is disabled -
 * provide macro implementation
 */
#define Com_GetVersionInfo(versionInfoPtr)                            \
    do                                                                \
    {                                                                 \
        (versionInfoPtr)->vendorID = COM_VENDOR_ID;                   \
        (versionInfoPtr)->moduleID = COM_MODULE_ID;                   \
        (versionInfoPtr)->sw_major_version = COM_SW_MAJOR_VERSION;    \
        (versionInfoPtr)->sw_minor_version = COM_SW_MINOR_VERSION;    \
        (versionInfoPtr)->sw_patch_version = COM_SW_PATCH_VERSION;    \
    } while(0)

#endif /* COM_VERSION_INFO_API */



/**
 * \brief Com_DeInit - sets COM to de-initialized state
 *
 * Simply sets the global variable Com_Status to COM_UNINIT.
 *
 * \ServiceID{0x02}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE) Com_DeInit
(
   void
);


/**
 * \brief Com_IpduGroupControl - starts/stops  I-PDU
 *
 * Function to start/stop every Rx-Ipdu and Tx-Ipdu according to the passed
 * states of the ComIpduGroups in the parameter ipduGroupVector.
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] ipduGroupVector I-PDU group vector containing the activation
 * state (stopped = 0/ started = 1) for all IPdus.
 * \param[in] Initialize flag to request initialization of the I-PDUs which
 * are newly started
 *
 * \ServiceID{0x03}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE) Com_IpduGroupControl
(
   Com_IpduGroupVector ipduGroupVector,
   boolean Initialize
);


/**
 * \brief Com_ReceptionDMControl - enables or disables Rx I-PDU
 * Deadline Monitoring
 *
 * Function to enables/disables every RxDM of every Rx-IPdu according to the
 * passed states of the ComIpduGroups in the parameter ipduGroupVector.
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] ipduGroupVector I-PDU group vector containing the activation
 * of RxDM (disable = 0/ enable = 1) for all Rx-Pdus.
 *
 * \ServiceID{0x06}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE)  Com_ReceptionDMControl
(
   Com_IpduGroupVector ipduGroupVector
);


/**
 * \brief Com_ClearIpduGroupVector - sets all bits of the given
 * Com_IpduGroupVector to 0.
 *
 * \param[in] ipduGroupVector I-PDU group vector to be cleared
 *
 * \ServiceID{0x1c}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE)  Com_ClearIpduGroupVector
(
   Com_IpduGroupVector ipduGroupVector
);


/**
 * \brief Com_SetIpduGroup -  sets the value of a bit in an I-PDU group vector.
 *
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] ipduGroupVector I-PDU group vector to be modified
 * \param[in] ipduGroupId identifies the corresponding bit in the I-PDU
 * group vector
 * \param[in] bitval new value of the corresponding bit
 *
 * \ServiceID{0x1d}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE)  Com_SetIpduGroup
(
   Com_IpduGroupVector ipduGroupVector,
   Com_IpduGroupIdType ipduGroupId,
   boolean bitval
);


/**
 * \brief returns status of Com
 *
 * This function returns whether the module is initialized
 *
 * \return Result of init status
 * \retval COM_INIT the module is initialized
 * \retval COM_UNINIT the module is not initialized
 *
 * \ServiceID{0x07}
 * \Reentrancy{Re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Com_StatusType, COM_CODE) Com_GetStatus
(
   void
);


/**
 * \brief Com_GetConfiguratinoId - get post-build-time configuration ID
 *
 * \return post-build-time configuration ID
 *
 * \ServiceID{0x08}
 * \Reentrancy{Re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(uint32, COM_CODE) Com_GetConfigurationId
(
   void
);


/**
 * \brief Com_SendSignal - send a signal see COM197
 *
 * The service Com_SendSignal updates the signal object identified by SignalId
 * with the value referenced by the SignalDataPtr parameter.
 * Preconditions:
 * COM must be initialized.
 *
 * \param[in] SignalId ID of the signal to be sent
 * \param[in] SignalDataPtr place in memory to copy the data from
 * \return Result of operation
 * \retval E_OK success
 * \retval COM_SERVICE_NOT_AVAILABLE failure (Com not initialized,
 * corresponding I-PDU group was stopped or
 * service failed due to development error)
 * \retval COM_BUSY - The receive Tp buffer is actually not available
 *
 * \ServiceID{0x0A}
 * \Reentrancy{Re-entrant for different values of SignalId only.}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(uint8, COM_CODE) Com_SendSignal
(
   Com_SignalIdType SignalId,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);

/**
 * \brief Com_SendDynSignal - send a dynamic length signal
 *
 * The service Com_SendDynSignal updates the signal object identified by SignalId and
 * from signal type UINT8_DYN with the value referenced by the SignalDataPtr parameter.
 * Preconditions:
 * COM must be initialized.
 *
 * \param[in] SignalId ID of the signal to be sent
 * \param[in] SignalDataPtr place in memory to copy the data from
 * \param[in] Length Length of the dynamic length signal to be send
 * \return Result of operation
 * \retval E_OK success
 * \retval E_NOT_OK in case the Length is greater than the configured ComSignalLength
 * of this sent signal
 * \retval COM_SERVICE_NOT_AVAILABLE failure (Com not initialized,
 * corresponding I-PDU group was stopped or
 * service failed due to development error)
 * \retval COM_BUSY - The receive Tp buffer is actually not available
 *
 * \ServiceID{0x21}
 * \Reentrancy{Re-entrant for different values of SignalId only.}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(uint8, COM_CODE) Com_SendDynSignal
(
   Com_SignalIdType SignalId,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   uint16 Length
);


/**
 * \brief Com_ReceiveSignal - get a signal's actual value from COM
 *
 * This function returns the actual value of a signal.
 * This function is always available.
 * If the configuration parameter ComGeneratedRcvSigEnable is disabled,
 * this function maps to the generic implementation of the function.
 * If the configuration parameter ComGeneratedRcvSigEnable is enabled
 * and ComMapReceiveSignal == Com_ReceiveSignalGeneric,
 * this function maps to the generic implementation of the function.
 * If the configuration parameter ComGeneratedRcvSigEnable is enabled
 * and ComMapReceiveSignal == Com_ReceiveSignalGenerated,
 * this function maps to the generated implementation of the function.
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] SignalId ID of signal to receive
 * \param[out] SignalDataPtr piece of memory to copy data to
 * \return Result of operation
 * \retval E_OK success
 * \retval COM_SERVICE_NOT_AVAILABLE Com is disabled, no valid value passed over
 *
 * \ServiceID{0x0B}
 * \Reentrancy{Re-entrant for different values of SignalId only.}
 * \Synchronicity{Synchronous}
 */
extern FUNC(uint8, COM_CODE) Com_ReceiveSignal
(
   Com_SignalIdType SignalId,
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);


#if (COM_GENERATED_RCV_SIG_ENABLE == STD_ON)
#if (COM_MAP_RECEIVE_SIGNAL == COM_RECEIVE_SIGNAL_GENERATED)
/** \brief Com_ReceiveSignalGeneric - get a signal's actual value from COM
 * This function returns the actual value of a signal.
 * This function is only available if the configuration parameter
 * ComGeneratedRcvSigEnable is enabled and
 * ComMapReceiveSignal == Com_ReceiveSignalGenerated.
 * If available, this function maps to the generic implementation of the function.
 * Preconditions:
 * - COM must be initialized
 * \param[in] SignalId ID of signal to receive
 * \param[out] SignalDataPtr piece of memory to copy data to
 * \return Result of operation
 * \retval E_OK success
 * \retval COM_SERVICE_NOT_AVAILABLE Com is disabled, no valid value passed over
 */
FUNC(uint8, COM_CODE) Com_ReceiveSignalGeneric
(
   Com_SignalIdType SignalId,
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);

#else

/** \brief Com_ReceiveSignalGenerated - get a signal's actual value from COM
 * This function returns the actual value of a signal.
 * This function is only available if the configuration parameter
 * ComGeneratedRcvSigEnable is enabled and
 * ComMapReceiveSignal == Com_ReceiveSignalGeneric.
 * If available, this function maps to the generated implementation of the function.
 * Preconditions:
 * - COM must be initialized
 * \param[in] SignalId - ID of signal to receive
 * \param[in] SignalDataPtr - piece of memory to copy data to
 * \return Result of operation
 * \retval E_OK - success
 * \retval COM_SERVICE_NOT_AVAILABLE - Com is disabled, no valid value passed over
 */
FUNC(uint8, COM_CODE) Com_ReceiveSignalGenerated
(
   Com_SignalIdType SignalId,
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);

#endif /* (COM_MAP_RECEIVE_SIGNAL == COM_RECEIVE_SIGNAL_GENERATED) */
#endif /* (COM_GENERATED_RCV_SIG_ENABLE == STD_ON) */

/**
 * \brief Com_ReceiveDynSignal - get a dynamic length signal's actual value from COM
 *
 * Com_ReceiveDynSignal copies the data of the signal identified by SignalId to
 * the location specified by SignalDataPtr and stores the length of the
 * dynamic length signal at the position given by the Length parameter.
 *
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] SignalId ID of signal to receive
 * \param[in] SignalDataPtr piece of memory to copy data to
 * \param[in,out] LengthPtr in: maximum length that could be received
 *                          out: length of the dynamic length signal
 * \return Result of operation
 * \retval E_OK success
 * \retval E_NOT_OK the Length (as in-parameter) is smaller than the received
 *            length of the dynamic length signal
 * \retval COM_SERVICE_NOT_AVAILABLE corresponding I-PDU group was stopped
 * (or service failed due to development error)
 *
 * \ServiceID{0x22}
 * \Reentrancy{Re-entrant for different values of SignalId only.}
 * \Synchronicity{Synchronous}
 */
extern FUNC(uint8, COM_CODE) Com_ReceiveDynSignal
(
   Com_SignalIdType SignalId,
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint16, AUTOMATIC, COM_APPL_DATA) LengthPtr
);

#if (COM_TIMEBASE == STD_ON)
/**
 * \brief Com_MainFunctionTx - handle cyclic sending-related tasks
 *
 * This function handles cyclic sending-related tasks such as minimum delay
 * time and cyclic sending.
 * Preconditions:
 * - COM must be initialized
 *
 * \ServiceID{0x19}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
/* Deviation MGCC62-1 */
extern FUNC(void, COM_CODE) Com_MainFunctionTx
(
   void
);
/* AUTOSAR declares Com_MainFunctionTx also in SchM_Com.h which leads to
   "MISRA-C:2012 Rule 08.5" violation. */

/**
 * \brief Com_MainFunctionRouteSignals - handle cyclic Signal Gateway tasks
 *
 * This function handles cyclic receiving and sending (group)signals(group)
 * for the Signal Gateway functionality (SigGW).
 * Preconditions:
 * - COM must be initialized
 *
 * \ServiceID{0x1A}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
/* Deviation MGCC62-2 */
extern FUNC(void, COM_CODE) Com_MainFunctionRouteSignals
(
   void
);
/* AUTOSAR declares Com_MainFunctionRouteSignals also in SchM_Com.h which
   leads to "MISRA-C:2012 Rule 08.5" violation. */

#endif /* (COM_TIMEBASE == STD_ON) */

/**
 * \brief Com_UpdateShadowSignal - updates the data in the signal group
 *
 * The service Com_UpdateShadowSignal updates the group signal object
 * identified by SignalId with the value referenced by the SignalDataPtr
 * parameter.
 * Preconditions:
 * COM must be initialized.
 *
 * \param[in] SignalId ID of the group signal to be updated
 * \param[in] SignalDataPtr place in memory to copy the data from
 *
 * \ServiceID{0x0C}
 * \Reentrancy{Re-entrant for different values of SignalId only.}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE) Com_UpdateShadowSignal
(
   Com_SignalIdType SignalId,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);


/**
 * \brief Com_SendSignalGroup - send a signal group
 *
 * The service Com_SendSignalGroup updates the signal group object identified
 * by SignalGroupId
 * Preconditions:
 * COM must be initialized.
 *
 * \param[in] SignalGroupId ID of the signal group to be sent
 * \return Result of operation
 * \retval E_OK success
 * \retval COM_SERVICE_NOT_AVAILABLE failure (Com not initialized,
 * corresponding I-PDU group was stopped or
 * service failed due to development error)
 * \retval COM_BUSY - The receive Tp buffer is actually not available
 *
 * \ServiceID{0x0D}
 * \Reentrancy{Re-entrant for different values of SignalGroupId only.}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(uint8, COM_CODE) Com_SendSignalGroup
(
   Com_SignalGroupIdType SignalGroupId
);


/**
 * \brief Com_ReceiveSignalGroup - copies the actual value of a signal group
 * into the shadow buffer
 *
 * This function copies the value of a Rx signal group from its Rx-IPdu into
 * the its shadow buffer
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] SignalGroupId ID of signal group
 * \return Result of operation
 * \retval E_OK success
 * \retval COM_SERVICE_NOT_AVAILABLE Com is disabled, no valid value passed over
 *
 * \ServiceID{0x0E}
 * \Reentrancy{Re-entrant for different values of SignalGroupId only.}
 * \Synchronicity{Synchronous}
 */
extern FUNC(uint8, COM_CODE) Com_ReceiveSignalGroup
(
   Com_SignalGroupIdType SignalGroupId
);


/**
 * \brief Com_ReceiveShadowSignal - get a group signal's value from shadow buffer
 *
 * This function returns the value of a group signal from its shadow buffer of
 * the signal group
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] SignalId ID of group signal to receive
 * \param[in] SignalDataPtr piece of memory to copy data to
 *
 * \ServiceID{0x0F}
 * \Reentrancy{Re-entrant for different values of SignalId only.}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE) Com_ReceiveShadowSignal
(
   Com_SignalIdType SignalId,
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);


/**
 * \brief Com_SendSignalGroupArray - update and send a signal group
 *
 * The service Com_SendSignalGroupArray copies the content of the provided
 * SignalGroupArrayPtr to the associated I-PDU. The provided data shall correspond
 * to the array representation of the signal group.
 * Preconditions:
 * COM must be initialized.
 *
 * \param[in] SignalGroupId ID of the signal group to be sent
 * \param[in] SignalGroupArrayPtr Reference to the signal group array to be transmitted
 * \param[in] SignalGroupArrayLength Length of the signal group array
 * \return Result of operation
 * \retval E_OK success
 * \retval COM_SERVICE_NOT_AVAILABLE failure (Com not initialized,
 * corresponding I-PDU group was stopped or
 * service failed due to development error)
 * \retval COM_BUSY - The receive Tp buffer is actually not available
 *
 * \ServiceID{0x43}
 * \Reentrancy{Re-entrant for different values of SignalGroupId only.}
 * \Synchronicity{Asynchronous}
 */
extern FUNC(uint8, COM_CODE) Com_SendSignalGroupArray
(
   Com_SignalGroupIdType SignalGroupId,
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SignalGroupArrayPtr,
   uint16 SignalGroupArrayLength
);


/**
 * \brief Com_ReceiveSignalGroupArray - access signal group array
 *
 * The service Com_ReceiveSignalGroupArray copies the received signal group array
 * representation from the I-PDU to the SignalGroupArrayPtr.
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] SignalGroupId ID of signal group to be received.
 * \param[in] SignalGroupArrayLengthPtr reference to length of the signal group array
 * \param[out] SignalGroupArrayPtr reference to the location where the received
 *  signal group array shall be stored
 * \return Result of operation
 * \retval E_OK service has been accepted
 * \retval COM_SERVICE_NOT_AVAILABLE corresponding I-PDU group was stopped
 * (or service failed due to development error)
 *
 * \ServiceID{0x44}
 * \Reentrancy{Re-entrant for different values of SignalGroupId only.}
 * \Synchronicity{Synchronous}
 */
FUNC(uint8, COM_CODE) Com_ReceiveSignalGroupArray
(
   Com_SignalGroupIdType SignalGroupId,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) SignalGroupArrayPtr,
   P2VAR(uint16, AUTOMATIC, COM_APPL_DATA) SignalGroupArrayLengthPtr
);


/**
 * \brief Send an IPDU
 *
 * The function triggers sending of an IPDU
 * Preconditions:
 * - The COM must be initialized
 * - The function must only be called from callouts
 *
 * \param[in] ComTxPduId ID of the PDU to be sent
 *
 * \ServiceID{0x17}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE) Com_TriggerIPDUSend
(
   PduIdType ComTxPduId
);


/**
 * \brief Switch to a Transmission Mode
 *
 * The function  sets the transmission mode of the I-PDU
 * referenced by PduId to Mode
 * Preconditions:
 * - The COM must be initialized
 * - Should not be mixed with signal based TMS
 *
 * \param[in] PduId ID of the PDU to be sent
 * \param[in] Mode the transmission mode that shall be set
 *
 * \ServiceID{0x27}
 * \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE) Com_SwitchIpduTxMode
(
   PduIdType PduId,
   boolean Mode
);


/** \brief Com_GetRxIPduBuffer - returns Rx IPdu buffer reference
 * The service Com_GetRxIPduBuffer returns the buffer object identified by ComRxPduId
 * with the buffer referenced by the PduInfoPtr parameter.
 * Preconditions:
 * COM must be initialized.
 *
 * \param[in] ComRxPduId - ID of the Rx ComIPdu
 * \param[out] PduInfoPtr - Rx IPdu buffer reference
 * \return Function execution success status
 * \retval E_OK - success
 * \retval E_NOT_OK - failure (Com not initialized or
 * service failed due to development error)
 */
extern FUNC(uint8, COM_CODE) Com_GetRxIPduBuffer
(
    PduIdType ComRxPduId,
    P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr
);


#if (COM_TIMEBASE == STD_ON)
/**
 * \brief Com_MainFunctionRx - handle cyclic receiving-related tasks
 *
 * This function handles cyclic receiving-related tasks like reception deadline
 * monitoring.
 * Preconditions:
 * - COM must be initialized
 *
 * \ServiceID{0x18}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
/* Deviation MGCC62-3 */
extern FUNC(void, COM_CODE) Com_MainFunctionRx
(
   void
);
/* AUTOSAR declares Com_MainFunctionRx also in SchM_Com.h which leads to
   "MISRA-C:2012 Rule 08.5" violation. */
#endif /* (COM_TIMEBASE == STD_ON) */

#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef COM_API_STATIC_H */
/*==================[end of file]============================================*/
