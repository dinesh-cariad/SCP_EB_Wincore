/**
 * \file
 *
 * \brief AUTOSAR CanIf
 *
 * This file contains the implementation of the AUTOSAR
 * module CanIf.
 *
 * \version 6.10.15
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!CODE!]
[!AUTOSPACING!]
[!INCLUDE "../../generate_macros/CanIf_PreCompileMacros.m"!]
[!IF "$canifMFSupport"!]
[!INCLUDE "../../generate_macros/CanIf_DecoupledProcessing.m"!]
[!ELSE!]
[!VAR "decoupledRxSupported" = "false()"!]
[!VAR "txDecoupledSupport" = "false()"!]
[!ENDIF!]

#ifndef CANIF_CFG_H
#define CANIF_CFG_H


/*==================[includes]===============================================*/
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* Autosar standard types */


/* It is required by AUTOSAR include the headerfile that contains the post-build/link-time symbolic IDs here */
/* This mixture of config classes can be omitted for module internal purposes */
#ifndef CANIF_NO_CFGCLASSMIX_REQUIRED
#include <CanIf_SymbolicNames_PBcfg.h>
#endif /* CANIF_NO_CFGCLASSMIX_REQUIRED */

#undef CANIF_NO_CFGCLASSMIX_REQUIRED

/*==================[macros]=================================================*/

/** \brief switch reports that PbcfgM uses CanIf */
[!WS "3"!][!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('CanIf')) = 'true'"!]
#if (defined CANIF_PBCFGM_SUPPORT_ENABLED)
#error CANIF_PBCFGM_SUPPORT_ENABLED already defined!
#endif /* #if( defined CANIF_PBCFGM_SUPPORT_ENABLED)*/
#define CANIF_PBCFGM_SUPPORT_ENABLED STD_ON


/** \brief switch reports that CanIf_IsValidConfig function shall be mapped to memory section CANIF_CODE_CC_BLOCK
*/
[!WS "3"!][!IF "node:exists(as:modconf('PbcfgM')[1]/PbcfgMGeneral/PbcfgMMapIsValidFunctionToMemSection) and as:modconf('PbcfgM')[1]/PbcfgMGeneral/PbcfgMMapIsValidFunctionToMemSection = 'true'"!]
#if (defined CANIF_ISVALIDCONFIG_MMAP_ENABLED)
#error CANIF_ISVALIDCONFIG_MMAP_ENABLED already defined!
#endif /* #if( defined CANIF_ISVALIDCONFIG_MMAP_ENABLED)*/
#define CANIF_ISVALIDCONFIG_MMAP_ENABLED STD_ON
[!ELSE!]
#if (defined CANIF_ISVALIDCONFIG_MMAP_ENABLED)
#error CANIF_ISVALIDCONFIG_MMAP_ENABLED already defined!
#endif /* #if( defined CANIF_ISVALIDCONFIG_MMAP_ENABLED)*/
#define CANIF_ISVALIDCONFIG_MMAP_ENABLED STD_OFF
[!ENDIF!]

[!ELSE!]
#if (defined CANIF_PBCFGM_SUPPORT_ENABLED)
#error CANIF_PBCFGM_SUPPORT_ENABLED already defined!
#endif /* #if( defined CANIF_PBCFGM_SUPPORT_ENABLED)*/
#define CANIF_PBCFGM_SUPPORT_ENABLED     STD_OFF

/** \brief switch reports that CanIf_IsValidConfig function shall be mapped to memory section CANIF_CODE_CC_BLOCK
*/
#if (defined CANIF_ISVALIDCONFIG_MMAP_ENABLED)
#error CANIF_ISVALIDCONFIG_MMAP_ENABLED already defined!
#endif /* #if( defined CANIF_ISVALIDCONFIG_MMAP_ENABLED)*/
#define CANIF_ISVALIDCONFIG_MMAP_ENABLED STD_OFF
[!ENDIF!]


/** \brief Switch for DET usage */
#if (defined CANIF_DEV_ERROR_DETECT)
#error CANIF_DEV_ERROR_DETECT already defined!
#endif /* #if( defined CANIF_DEV_ERROR_DETECT)*/
#define CANIF_DEV_ERROR_DETECT          [!WS "3"!][!IF "$canifDetEnabled"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Multiple driver support */
#if (defined CANIF_MULTIPLE_DRV_SUPPORT)
#error CANIF_MULTIPLE_DRV_SUPPORT already defined!
#endif /* #if( defined CANIF_MULTIPLE_DRV_SUPPORT)*/
#define CANIF_MULTIPLE_DRV_SUPPORT      [!WS "3"!][!IF "$canIfPublicMultipleDrvSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Support of CanIf_GetVersionInfo()
 **
 ** This constant defines whether the CanIf_GetVersionInfo is
 ** provided.
 ** STD_ON    Function is provided
 ** STD_OFF   Function is not provided
 */
#if (defined CANIF_VERSION_INFO_API)
#error CANIF_VERSION_INFO_API already defined!
#endif /* #if( defined CANIF_VERSION_INFO_API)*/
#define CANIF_VERSION_INFO_API          [!WS "3"!][!IF "CanIfPublicCfg/CanIfPublicVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of CanIf_MainFunctionRx() and/or CanIf_MainFunctionTx()
 **
 ** This constant defines whether the CanIf_MainFunctionRx and/or
 ** CanIf_MainFunctionTx is provided.
 ** STD_ON    Functions are provided
 ** STD_OFF   Functions are not provided
 */
#if (defined CANIF_DECOUPLED_PROCESSING_SUPPORT)
#error CANIF_DECOUPLED_PROCESSING_SUPPORT already defined!
#endif
#define CANIF_DECOUPLED_PROCESSING_SUPPORT   [!WS "3"!][!IF "$canifMFSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of Rx decoupled processing measurement
 **
 ** This constant defines whether the CDD is notified with the number of 
 ** queued Rx Pdus/ number of Rx Pdus that could not be added in the queue.
 ** STD_ON    Handling is provided
 ** STD_OFF   Handling is not provided
 */
#if (defined CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT)
#error CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT already defined!
#endif
#define CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT   [!WS "3"!][!IF "$canifRxDecMeasureSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of Tx decoupled processing measurement
 **
 ** This constant defines whether the CDD is notified with the number of 
 ** queued Tx Pdus/ number of Tx Pdus that could not be added in the queue.
 ** STD_ON    Handling is provided
 ** STD_OFF   Handling is not provided
 */
#if (defined CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT)
#error CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT already defined!
#endif
#define CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT   [!WS "3"!][!IF "$canifTxDecMeasureSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of CanIf_MainFunctionTx()
 **
 ** This constant defines whether the CanIf_MainFunctionTx is provided.
 ** STD_ON    Function is provided
 ** STD_OFF   Function is not provided
 */
#if (defined CANIF_DECOUPLED_TXPROCESSING_SUPPORT)
#error CANIF_DECOUPLED_TXPROCESSING_SUPPORT already defined!
#endif
#define CANIF_DECOUPLED_TXPROCESSING_SUPPORT   [!WS "3"!][!IF "$txDecoupledSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of CanIf_MainFunctionRx()
 **
 ** This constant defines whether the CanIf_MainFunctionRx is provided.
 ** STD_ON    Function is provided
 ** STD_OFF   Function is not provided
 */
#if (defined CANIF_DECOUPLED_RXPROCESSING_SUPPORT)
#error CANIF_DECOUPLED_RXPROCESSING_SUPPORT already defined!
#endif
#define CANIF_DECOUPLED_RXPROCESSING_SUPPORT   [!WS "3"!][!IF "$decoupledRxSupported = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief support of API CanIf_SetBaudrate()
 **
 ** This constant defines whether the API function CanIf_SetBaudrate() is
 ** provided.
 ** STD_ON    Function is provided
 ** STD_OFF   Function is not provided
 */
#if (defined CANIF_SET_BAUDRATE_API)
#error CANIF_SET_BAUDRATE_API already defined!
#endif /* #if( defined CANIF_SET_BAUDRATE_API)*/
#define CANIF_SET_BAUDRATE_API   [!WS "3"!][!IF "CanIfPublicCfg/CanIfSetBaudrateApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief pre-compile switch defining whether CanIfPublicCanIdTypeEnum is set to UINT32 or not.
 **
 ** This constant defines whether the API function CanIf_SetBaudrate() is
 ** provided.
 ** STD_ON    CanIfPublicCanIdTypeEnum is UINT32
 ** STD_OFF   CanIfPublicCanIdTypeEnum is UINT16
 */
#if (defined CANIF_CANIDTYPE_UINT32)
#error CANIF_CANIDTYPE_UINT32 already defined!
#endif /* #if( defined CANIF_CANIDTYPE_UINT32)*/
#define CANIF_CANIDTYPE_UINT32  [!WS "3"!][!IF "as:modconf('CanIf')[1]/CanIfPublicCfg/CanIfPublicCanIdTypeEnum = 'UINT32'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief support of API CanIf_CancelTransmit()
 **
 ** This constant defines whether the API function CanIf_CancelTransmit() is
 ** provided.
 ** STD_ON    Function is provided
 ** STD_OFF   Function is not provided
 */
#if (defined CANIF_CANCEL_TRANSMIT_SUPPORT)
#error CANIF_CANCEL_TRANSMIT_SUPPORT already defined!
#endif /* #if( defined CANIF_CANCEL_TRANSMIT_SUPPORT)*/
#define CANIF_CANCEL_TRANSMIT_SUPPORT   [!WS "3"!][!IF "CanIfPublicCfg/CanIfPublicCancelTransmitSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of API CanIf_GetTxConfirmationState()
 **
 ** This constant defines whether the API function CanIf_GetTxConfirmationState()
 ** is provided.
 ** STD_ON    Function is provided
 ** STD_OFF   Function is not provided
 */
#if (defined CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT)
#error CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT already defined!
#endif /* #if( defined CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT)*/
#define CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT    [!WS "3"!][!IF "CanIfPublicCfg/CanIfPublicTxConfirmPollingSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Enable sending the result within Tx Confirmation
 **
 ** This constant defines whether the Tx confirmation will provide the result
 ** as function parameter, or the Tx confirmation will be sent in case of 
 ** successful cases only for all upper layers
 ** STD_ON    Send the result as a parameter (E_OK/E_NOT_OK)
 ** STD_OFF   Tx Confirmation in case of successful transmission only
 */
#if (defined CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED)
#error CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED already defined!
#endif /* #if( defined CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED)*/
#define CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED    [!WS "3"!][!IF "$ulTxConfirmationResutSupported"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of CanIf_CancelTxConfirmation() callback function
 **
 ** This constant defines whether the callback CanIf_CancelTxConfirmation()
 ** is provided.
 ** STD_ON    Function is provided
 ** STD_OFF   Function is not provided
 */
#if (defined CANIF_CANCEL_TX_CONFIRMATION_CB)
#error CANIF_CANCEL_TX_CONFIRMATION_CB already defined!
#endif /* #if( defined CANIF_CANCEL_TX_CONFIRMATION_CB)*/
[!IF "($canDriverCompatibility = 'ASR402') or ($canDriverCompatibility = 'ASR403')"!]
#define CANIF_CANCEL_TX_CONFIRMATION_CB           [!WS "3"!][!IF "node:containsValue(CanIfCtrlDrvCfg/*/CanIfCtrlDrvTxCancellation, 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ELSE!]
#define CANIF_CANCEL_TX_CONFIRMATION_CB           STD_OFF
[!ENDIF!]


/** \brief Support of wakeup API
 **
 ** This constant defines whether the wakeup APIs are provided.
 ** STD_ON    Function is provided
 ** STD_OFF   Function is not provided
 */
#if (defined CANIF_WAKEUP_SUPPORT_API)
#error CANIF_WAKEUP_SUPPORT_API already defined!
#endif /* #if( defined CANIF_WAKEUP_SUPPORT_API)*/
#define CANIF_WAKEUP_SUPPORT_API                  [!WS "3"!][!IF "$wakeupNotificationCalled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of wakeup validation API
 **
 ** This constant defines whether wakeup validation API is provided.
 ** STD_ON    Function is provided
 ** STD_OFF   Function is not provided
 */
#if (defined CANIF_WAKEUP_VALIDATION_API)
#error CANIF_WAKEUP_VALIDATION_API already defined!
#endif /* #if( defined CANIF_WAKEUP_VALIDATION_API)*/
#define CANIF_WAKEUP_VALIDATION_API               [!WS "3"!][!IF "$wakeupValidationCalled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of wakeup validation only when controller is started
 **
 ** This constant defines whether the wakeup validation is performed only based on the controller
 ** state and/or the wakeup flag value.
 ** STD_ON    Wakeup validation is performed only when the corresponding controller
              is in CAN_CS_STARTED mode (handling according to ASR 4.3.1).
 ** STD_OFF   Wakeup validation is performed only when the controller is in
              CANIF_CS_SLEEP mode or when the wakeup flag was previously set.
 */
#if (defined CANIF_VALIDATE_WAKEUP_ON_STARTED_CTRL_ONLY_SUPPORT)
#error CANIF_VALIDATE_WAKEUP_ON_STARTED_CTRL_ONLY_SUPPORT already defined!
#endif
#define CANIF_VALIDATE_WAKEUP_ON_STARTED_CTRL_ONLY_SUPPORT   [!WS "3"!][!IF "$canifValidateWakeupOnStartedCtrlSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of function CanIf_SetDynamicTxId()
 **
 ** This constant defines whether CanIf_SetDynamicTxId() is supported.
 ** STD_ON    Function is provided
 ** STD_OFF   Function is not provided
 */
#if (defined CANIF_SETDYNAMICTXID_API)
#error CANIF_SETDYNAMICTXID_API already defined!
#endif /* #if( defined CANIF_SETDYNAMICTXID_API)*/
#define CANIF_SETDYNAMICTXID_API                  [!WS "3"!][!IF "$canifSetDynTxIdSupported"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of function CanIf_ReadRxPduData()
 **
 ** This constant defines whether CanIf_ReadRxPduData() is supported.
 ** STD_ON    Function is provided
 ** STD_OFF   Function is not provided
 */
#if (defined CANIF_READRXPDU_DATA_API)
#error CANIF_READRXPDU_DATA_API already defined!
#endif /* #if( defined CANIF_READRXPDU_DATA_API)*/
#define CANIF_READRXPDU_DATA_API                  [!WS "3"!][!IF "$canifReadRxPduDataApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of function CanIf_ReadRxNotifStatus()
 **
 ** This constant defines whether CanIf_ReadRxNotifStatus() is supported.
 ** STD_ON    Function is provided
 ** STD_OFF   Function is not provided
 */
#if (defined CANIF_READRXPDU_NOTIF_STATUS_API)
#error CANIF_READRXPDU_NOTIF_STATUS_API already defined!
#endif /* #if( defined CANIF_READRXPDU_NOTIF_STATUS_API)*/
#define CANIF_READRXPDU_NOTIF_STATUS_API          [!WS "3"!][!IF "$canifReadRxNotifStatusApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of function CanIf_ReadTxNotifStatus()
 **
 ** This constant defines whether CanIf_ReadTxNotifStatus() is supported.
 ** STD_ON    Function is provided
 ** STD_OFF   Function is not provided
 */
#if (defined CANIF_READTXPDU_NOTIF_STATUS_API)
#error CANIF_READTXPDU_NOTIF_STATUS_API already defined!
#endif /* #if( defined CANIF_READTXPDU_NOTIF_STATUS_API)*/
#define CANIF_READTXPDU_NOTIF_STATUS_API          [!WS "3"!][!IF "$canifReadTxNotifStatusApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of range reception
 **
 ** This constant defines if range reception is configured and used.
 ** STD_ON    Range reception is used
 ** STD_OFF   No reception range is used
 */
#if (defined CANIF_RANGE_RECEPTION)
#error CANIF_RANGE_RECEPTION already defined!
#endif /* #if( defined CANIF_RANGE_RECEPTION)*/
#define CANIF_RANGE_RECEPTION                  [!WS "3"!][!IF "$rangeReceptionUsed"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Support of CanTrcv
 **
 ** This constant defines if transceivers are supported
 ** this configuration.
 */
#if (defined CANIF_TRCV_SUPPORT)
#error CANIF_TRCV_SUPPORT already defined!
#endif /* #if( defined CANIF_TRCV_SUPPORT)*/
#define CANIF_TRCV_SUPPORT                     [!WS "3"!][!IF "$canTrcvSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief CanIf partial networking support
 **
 ** This macro defines if API functions for partial networking are exported.
 */
#if (defined CANIF_PUBLIC_PN_SUPPORT)
#error CANIF_PUBLIC_PN_SUPPORT already defined!
#endif /* #if( defined CANIF_PUBLIC_PN_SUPPORT)*/
#define CANIF_PUBLIC_PN_SUPPORT            [!WS "3"!][!IF "$canifPNSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief CanIfTxOfflineActiveSupport
 **
 ** This macro defines if CanIfTxOfflineActiveSupport is enabled or not.
 */
#define CANIF_TX_OFFLINE_ACTIVE_SUPPORT    [!WS "3"!][!IF "CanIfPublicCfg/CanIfTxOfflineActiveSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief CanIf tx buffering support
 **
 ** This macro defines if tx buffering is enabled or not.
 */
#if (defined CANIF_PUBLIC_TX_BUFFERING)
#error CANIF_PUBLIC_TX_BUFFERING already defined!
#endif /* #if( defined CANIF_PUBLIC_TX_BUFFERING)*/
#define CANIF_PUBLIC_TX_BUFFERING          [!WS "3"!][!IF "$publicTxBufferingSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief DLC check switch
 **
 ** This constant defines whether the DLC check is used or not.
 ** STD_ON    DLC check is enabled
 ** STD_OFF   DLC check is disabled
 */
#if (defined CANIF_DLC_CHECK_ENABLED)
#error CANIF_DLC_CHECK_ENABLED already defined!
#endif /* #if( defined CANIF_DLC_CHECK_ENABLED)*/
#define CANIF_DLC_CHECK_ENABLED            [!WS "3"!][!IF "CanIfPrivateCfg/CanIfPrivateDlcCheck"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Single CAN controller optimization switch
 **
 ** This constant defines, if the CanIf single controller optimization is
 ** switched on.
 */
#if (defined CANIF_SINGLE_CAN_CTRL_OPT)
#error CANIF_SINGLE_CAN_CTRL_OPT already defined!
#endif /* #if( defined CANIF_SINGLE_CAN_CTRL_OPT)*/
#define CANIF_SINGLE_CAN_CTRL_OPT          [!WS "3"!][!IF "$canifSingleCtrlOpt"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief HOH translation optimization
 **
 ** This constant defines, if the HOH translation optimization is enabled.
 ** It is enabled, if the HOH IDs are zero-based and consecutive and all HRH
 ** IDs are lower than the HTH IDs.
 */
#if (defined CANIF_HOH_TRANSLATION_OPT)
#error CANIF_HOH_TRANSLATION_OPT already defined!
#endif /* #if( defined CANIF_HOH_TRANSLATION_OPT)*/
#define CANIF_HOH_TRANSLATION_OPT             [!WS "3"!][!IF "$canifHOHTransOpt"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Switch for <UL>_RxIndication support
 **
 ** This switch defines, if CanIf uses the <UL>_RxIndcation function of any of the upper layer
 */
#if (defined CANIF_UL_RX_INDICATION_SUPPORT)
#error CANIF_UL_RX_INDICATION_SUPPORT already defined!
#endif /* #if( defined CANIF_UL_RX_INDICATION_SUPPORT)*/
#define CANIF_UL_RX_INDICATION_SUPPORT      [!WS "3"!][!IF "$ulRxIndicationSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Switch for <Cdd>_RxIndication support
 **
 ** This switch defines, if CanIf uses the <Cdd>_RxIndcation function of any of the upper layer
 */
#if (defined CANIF_CDD_RX_INDICATION_SUPPORT)
#error CANIF_CDD_RX_INDICATION_SUPPORT already defined!
#endif /* #if( defined CANIF_CDD_RX_INDICATION_SUPPORT)*/
#define CANIF_CDD_RX_INDICATION_SUPPORT      [!WS "3"!][!IF "$cddRxIndicationSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Switch for <UL>_TxConfirmation support
 **
 ** This switch defines, if CanIf uses the <UL>_TxConfirmation function of any of the upper layer
 */
#if (defined CANIF_TX_CONFIRMATION_SUPPORT)
#error CANIF_TX_CONFIRMATION_SUPPORT already defined!
#endif /* #if( defined CANIF_TX_CONFIRMATION_SUPPORT)*/
#define CANIF_TX_CONFIRMATION_SUPPORT      [!WS "3"!][!IF "$ulTxConfirmationSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Switch for DLC error notification support
 **
 ** This switch defines, if DLC error notification functions are supported.
 */
#if (defined CANIF_DLC_ERROR_NOTIF_SUPPORT)
#error CANIF_DLC_ERROR_NOTIF_SUPPORT already defined!
#endif /* #if( defined CANIF_DLC_ERROR_NOTIF_SUPPORT)*/
#define CANIF_DLC_ERROR_NOTIF_SUPPORT      [!WS "3"!][!IF "$dlcErrorNotifSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Switch for DLC check passed notification support
 **
 ** This switch defines, if DLC check passed notification functions are
 ** supported.
 */
#if (defined CANIF_DLC_PASSED_NOTIF_SUPPORT)
#error CANIF_DLC_PASSED_NOTIF_SUPPORT already defined!
#endif /* #if( defined CANIF_DLC_PASSED_NOTIF_SUPPORT)*/
#define CANIF_DLC_PASSED_NOTIF_SUPPORT     [!WS "3"!][!IF "$dlcPassedNotifSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Any callback function of any upper layer used
 **
 ** This macro defines if at least one upper layer of the CanIf
 ** shall be notified via any callback function (which are
 ** <UL>_RxIndication, <CDD>_RxIndication, <UL>_TxConfirmation,
 ** DLC error and DLC passed).
 */
#if (defined CANIF_ANY_UL_CBK_SUPPORT)
#error CANIF_ANY_UL_CBK_SUPPORT already defined!
#endif /* #if( defined CANIF_ANY_UL_CBK_SUPPORT)*/
#define CANIF_ANY_UL_CBK_SUPPORT     [!WS "3"!][!IF "($ulRxIndicationSupport = 'true') or ($cddRxIndicationSupport = 'true') or ($ulTxConfirmationSupport = 'true') or ($dlcErrorNotifSupport = 'true') or ($dlcPassedNotifSupport = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Switch for wakeup validation by NM
 **
 ** This switch defines, if wakeup validation is allowed by each Rx PDU (value
 ** STD_OFF) or if wakeup validation is only performed for (Can)Nm PDUs (value
 ** STD_ON).
 */
#if (defined CANIF_WAKEUP_CHECK_VALID_BY_NM)
#error CANIF_WAKEUP_CHECK_VALID_BY_NM already defined!
#endif /* #if( defined CANIF_WAKEUP_CHECK_VALID_BY_NM)*/
#define CANIF_WAKEUP_CHECK_VALID_BY_NM     [!WS "3"!][!IF "CanIfPublicCfg/CanIfPublicWakeupCheckValidByNM = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Index of Nm as upper layer
 **
 ** This symbol defines the index of the module CanNm as upper layer in the upper
 ** layer configuration map.
 */
#if (defined CANIF_NM_UPPER_LAYER_INDEX)
#error CANIF_NM_UPPER_LAYER_INDEX already defined!
#endif /* #if( defined CANIF_NM_UPPER_LAYER_INDEX)*/
#define CANIF_NM_UPPER_LAYER_INDEX         [!WS "3"!][!IF "(CanIfPublicCfg/CanIfPublicWakeupCheckValidByNM = 'true') and (count(CanIfUpperLayerConfig/*[@name = 'CAN_NM']) > 0)"!][!"num:i(node:pos(CanIfUpperLayerConfig/*[@name = 'CAN_NM']))"!]U[!ELSE!]CANIF_NO_UPPER_LAYER_CALLBACK[!ENDIF!]


/** \brief CAN controller wakeup support
 **
 ** This macro defines if wakeup by CAN controllers is supported.
 */
#if (defined CANIF_CAN_WAKEUP_SUPPORT)
#error CANIF_CAN_WAKEUP_SUPPORT already defined!
#endif /* #if( defined CANIF_CAN_WAKEUP_SUPPORT)*/
#define CANIF_CAN_WAKEUP_SUPPORT           [!WS "3"!][!IF "$canWakeupSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief CAN transceiver wakeup support
 **
 ** This macro defines if wakeup by CAN transceivers is supported.
 */
#if (defined CANIF_CANTRCV_WAKEUP_SUPPORT)
#error CANIF_CANTRCV_WAKEUP_SUPPORT already defined!
#endif /* #if( defined CANIF_CANTRCV_WAKEUP_SUPPORT)*/
#define CANIF_CANTRCV_WAKEUP_SUPPORT       [!WS "3"!][!IF "$canTrcvWakeupSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief CAN driver compatibility
 **
 ** This macro is defined based on which CAN ASR version, CanIf is compatible with.
 ** Currently, CanIf is configured to be compatible with [!"$canDriverCompatibility"!].
 */
#if (defined CANIF_CAN_[!"$canDriverCompatibility"!]_COMPATIBILITY)
#error CANIF_CAN_[!"$canDriverCompatibility"!]_COMPATIBILITY already defined!
#endif /* #if( defined CANIF_CAN_[!"$canDriverCompatibility"!]_COMPATIBILITY)*/
#define CANIF_CAN_[!"$canDriverCompatibility"!]_COMPATIBILITY    STD_ON


/** \brief CAN Metadata support
 **
 ** This macro defines if CanIf provides Metadata support.
 */
#if (defined CANIF_METADATA_SUPPORT)
#error CANIF_METADATA_SUPPORT already defined!
#endif /* #if( defined CANIF_METADATA_SUPPORT)*/
#define CANIF_METADATA_SUPPORT           [!WS "3"!][!IF "$CanIfMetaDataSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief CAN Software Filtering support
 **
 ** This macro defines if CanIf supports software filtering
 ** for any configured HRH.
 */
#if (defined CANIF_SW_FILTER)
#error CANIF_SW_FILTER already defined!
#endif /* #if( defined CANIF_SW_FILTER)*/
#define CANIF_SW_FILTER  [!WS "3"!][!IF "$CanIfSWFilter = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro for bus-off callback function
 **
 ** This macro shall be used for calling the bus-off notification function.
 */
#if (defined CANIF_BUSOFF_CALLBACK)
#error CANIF_BUSOFF_CALLBACK already defined!
#endif /* #if( defined CANIF_BUSOFF_CALLBACK)*/
#define CANIF_BUSOFF_CALLBACK( Controller ) \
[!IF "node:exists(CanIfDispatchCfg/CanIfDispatchUserCtrlBusOffName)"!]
  [!"CanIfDispatchCfg/CanIfDispatchUserCtrlBusOffName"!]( (Controller) )
[!ELSE!][!//
    CanSM_ControllerBusOff( (Controller) )
[!ENDIF!][!//


/** \brief Macro for controller mode indication callback function
**
** This macro shall be used for calling the controller mode indication function.
*/
#if (defined CANIF_CTRLMODEINDICATION_CALLBACK)
#error CANIF_CTRLMODEINDICATION_CALLBACK already defined!
#endif /* #if( defined CANIF_CTRLMODEINDICATION_CALLBACK)*/
#define CANIF_CTRLMODEINDICATION_CALLBACK( Controller , ControllerMode) \
[!IF "node:exists(CanIfDispatchCfg/CanIfDispatchUserCtrlModeIndicationName)"!]
[!"CanIfDispatchCfg/CanIfDispatchUserCtrlModeIndicationName"!]( (Controller), (ControllerMode) )
[!ELSE!][!//
    CanSM_ControllerModeIndication( (Controller), (ControllerMode) )
[!ENDIF!][!//


/** \brief Macro for transceiver mode indication callback function
**
** This macro shall be used for calling the transceiver mode indication function.
*/
#if (defined CANIF_TRCVMODEINDICATION_CALLBACK)
#error CANIF_TRCVMODEINDICATION_CALLBACK already defined!
#endif /* #if( defined CANIF_TRCVMODEINDICATION_CALLBACK)*/
#define CANIF_TRCVMODEINDICATION_CALLBACK( Transceiver , TransceiverMode) \
[!IF "node:exists(CanIfDispatchCfg/CanIfDispatchUserTrcvModeIndicationName)"!]
[!"CanIfDispatchCfg/CanIfDispatchUserTrcvModeIndicationName"!]( (Transceiver), (TransceiverMode) )
[!ELSE!][!//
    CanSM_TransceiverModeIndication( (Transceiver), (TransceiverMode) )
[!ENDIF!][!//


#if CANIF_PUBLIC_PN_SUPPORT == STD_ON
/** \brief Macro for confirm PN availability callback function
**
** This macro shall be used for calling the confirm PN availability function.
*/
#if (defined CANIF_CONFIRMPNAVAILABILITY_CALLBACK)
#error CANIF_CONFIRMPNAVAILABILITY_CALLBACK already defined!
#endif /* #if( defined CANIF_CONFIRMPNAVAILABILITY_CALLBACK)*/
#define CANIF_CONFIRMPNAVAILABILITY_CALLBACK( Transceiver) \
  [!IF "CanIfDispatchCfg/CanIfDispatchUserConfirmPnAvailabilityUL = 'CAN_SM'"!]
    CanSM_ConfirmPnAvailability( Transceiver )
  [!ELSE!][!//
    [!"CanIfDispatchCfg/CanIfDispatchUserConfirmPnAvailabilityName"!]( Transceiver )
  [!ENDIF!][!//


[!IF "node:exists(CanIfDispatchCfg/CanIfDispatchUserClearTrcvWufFlagIndicationUL)"!]
/** \brief Macro for clear transceiver WUF flag indication callback function
**
** This macro shall be used for calling the clear transceiver WUF flag indication function
*/
#if (defined CANIF_CLEARTRCVWUFFLAGINDICATION_CALLBACK)
#error CANIF_CLEARTRCVWUFFLAGINDICATION_CALLBACK already defined!
#endif /* #if( defined CANIF_CLEARTRCVWUFFLAGINDICATION_CALLBACK)*/
#define CANIF_CLEARTRCVWUFFLAGINDICATION_CALLBACK( Transceiver) \
    [!IF "CanIfDispatchCfg/CanIfDispatchUserClearTrcvWufFlagIndicationUL = 'CAN_SM'"!]
    CanSM_ClearTrcvWufFlagIndication( Transceiver )
    [!ELSE!][!//
      [!IF "node:exists(CanIfDispatchCfg/CanIfDispatchUserClearTrcvWufFlagIndicationName)"!]
    [!"CanIfDispatchCfg/CanIfDispatchUserClearTrcvWufFlagIndicationName"!]( Transceiver )
      [!ENDIF!][!//
    [!ENDIF!][!//

[!ENDIF!][!//


  [!IF "node:exists(CanIfDispatchCfg/CanIfDispatchUserCheckTrcvWakeFlagIndicationUL)"!]
/** \brief Macro for check transceiver wake flag indication callback function
**
** This macro shall be used for calling the check transceiver wake flag indication function
*/
#if (defined CANIF_CHECKTRCVWAKEFLAGINDICATION_CALLBACK)
#error CANIF_CHECKTRCVWAKEFLAGINDICATION_CALLBACK already defined!
#endif /* #if( defined CANIF_CHECKTRCVWAKEFLAGINDICATION_CALLBACK)*/
#define CANIF_CHECKTRCVWAKEFLAGINDICATION_CALLBACK( Transceiver) \
    [!IF "CanIfDispatchCfg/CanIfDispatchUserCheckTrcvWakeFlagIndicationUL = 'CAN_SM'"!]
    CanSM_CheckTransceiverWakeFlagIndication( Transceiver )
    [!ELSE!][!//
      [!IF "node:exists(CanIfDispatchCfg/CanIfDispatchUserCheckTrcvWakeFlagIndicationName)"!]
    [!"CanIfDispatchCfg/CanIfDispatchUserCheckTrcvWakeFlagIndicationName"!]( Transceiver )
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
#endif /* CANIF_PUBLIC_PN_SUPPORT */


/** \brief Macro for calling the upper layer wakeup notification function
 **
 ** This macro shall be used for calling the configured upper layer wakeup
 ** notification function.
 */
[!IF "($wakeupNotificationCalled = 'true') and ($canDriverCompatibility = 'ASR402')"!]
  [!IF "node:exists(CanIfDispatchCfg/CanIfDispatchUserSetWakeupEventName)"!]
#if (defined CANIF_SET_WAKEUP_EVENT)
#error CANIF_SET_WAKEUP_EVENT already defined!
#endif /* #if( defined CANIF_SET_WAKEUP_EVENT)*/
#define CANIF_SET_WAKEUP_EVENT( Sources )   [!"CanIfDispatchCfg/CanIfDispatchUserSetWakeupEventName"!]( (Sources) );
  [!ELSE!]
#if (defined CANIF_SET_WAKEUP_EVENT)
#error CANIF_SET_WAKEUP_EVENT already defined!
#endif /* #if( defined CANIF_SET_WAKEUP_EVENT)*/
#define CANIF_SET_WAKEUP_EVENT( Sources )   EcuM_SetWakeupEvent( (Sources) );
  [!ENDIF!]
[!ELSE!]
#if (defined CANIF_SET_WAKEUP_EVENT)
#error CANIF_SET_WAKEUP_EVENT already defined!
#endif /* #if( defined CANIF_SET_WAKEUP_EVENT)*/
#define CANIF_SET_WAKEUP_EVENT( Sources )
[!ENDIF!]


/** \brief Macro for calling the upper layer wakeup validation function
 **
 ** This macro shall be used for calling the configured upper layer wakeup
 ** validation function.
 */
[!IF "$wakeupValidationCalled = 'true'"!]
  [!IF "node:exists(CanIfDispatchCfg/CanIfDispatchUserValidateWakeupEventName)"!]
#if (defined CANIF_VALIDATE_WAKEUP_EVENT)
#error CANIF_VALIDATE_WAKEUP_EVENT already defined!
#endif /* #if( defined CANIF_VALIDATE_WAKEUP_EVENT)*/
#define CANIF_VALIDATE_WAKEUP_EVENT( Sources )   [!"CanIfDispatchCfg/CanIfDispatchUserValidateWakeupEventName"!]( (Sources) );
  [!ELSE!]
#if (defined CANIF_VALIDATE_WAKEUP_EVENT)
#error CANIF_VALIDATE_WAKEUP_EVENT already defined!
#endif /* #if( defined CANIF_VALIDATE_WAKEUP_EVENT)*/
#define CANIF_VALIDATE_WAKEUP_EVENT( Sources )   EcuM_ValidateWakeupEvent( (Sources) );
  [!ENDIF!]
[!ELSE!]
#if (defined CANIF_VALIDATE_WAKEUP_EVENT)
#error CANIF_VALIDATE_WAKEUP_EVENT already defined!
#endif /* #if( defined CANIF_VALIDATE_WAKEUP_EVENT)*/
#define CANIF_VALIDATE_WAKEUP_EVENT( Sources )
[!ENDIF!]


/** \brief Macro for calling the CDD Rx measurement function
 **
 ** This macro shall be used for calling the configured CDD for Rx decoupled processing
 ** measurement function when a Rx Pdu is added in the queue.
 */
[!IF "$canifRxDecMeasureSupport = 'true'"!]
#if (defined CANIF_ENQUEUED_RXPDUS_CALLBACK)
#error CANIF_ENQUEUED_RXPDUS_CALLBACK already defined!
#endif /* #if( defined CANIF_ENQUEUED_RXPDUS_CALLBACK)*/
#define CANIF_ENQUEUED_RXPDUS_CALLBACK( MainFuncId, NoOfPdus )   [!"CanIfDecoupledMeasurementSupport/CanIfNumberOfEnqueuedRxPdusApiName"!]( (MainFuncId), (NoOfPdus) );
[!ENDIF!]


/** \brief Macro for calling the CDD Tx measurement function
 **
 ** This macro shall be used for calling the configured CDD for Tx decoupled processing
 ** measurement function when a Tx Pdu is added in the queue.
 */
[!IF "$canifTxDecMeasureSupport = 'true'"!]
#if (defined CANIF_ENQUEUED_TXPDUS_CALLBACK)
#error CANIF_ENQUEUED_TXPDUS_CALLBACK already defined!
#endif /* #if( defined CANIF_ENQUEUED_TXPDUS_CALLBACK)*/
#define CANIF_ENQUEUED_TXPDUS_CALLBACK( MainFuncId, NoOfPdus )   [!"CanIfDecoupledMeasurementSupport/CanIfNumberOfEnqueuedTxPdusApiName"!]( (MainFuncId), (NoOfPdus) );
[!ENDIF!]


/** \brief Macro for calling the CDD Rx measurement function
 **
 ** This macro shall be used for calling the configured CDD for Rx decoupled processing
 ** measurement function when the queue is full.
 */
[!IF "$canifRxDecMeasureSupport = 'true'"!]
#if (defined CANIF_RX_EXCEED_QUEUE_CALLBACK)
#error CANIF_RX_EXCEED_QUEUE_CALLBACK already defined!
#endif /* #if( defined CANIF_RX_EXCEED_QUEUE_CALLBACK)*/
#define CANIF_RX_EXCEED_QUEUE_CALLBACK( MainFuncId, NoOfPdus )   [!"CanIfDecoupledMeasurementSupport/CanIfNumberOfRxPdusExceedingQueueApiName"!]( (MainFuncId), (NoOfPdus) );
[!ENDIF!]


/** \brief Macro for calling the CDD Tx measurement function
 **
 ** This macro shall be used for calling the configured CDD for Tx decoupled processing
 ** measurement function when the queue is full.
 */
[!IF "$canifTxDecMeasureSupport = 'true'"!]
#if (defined CANIF_TX_EXCEED_QUEUE_CALLBACK)
#error CANIF_TX_EXCEED_QUEUE_CALLBACK already defined!
#endif /* #if( defined CANIF_TX_EXCEED_QUEUE_CALLBACK)*/
#define CANIF_TX_EXCEED_QUEUE_CALLBACK( MainFuncId, NoOfPdus )   [!"CanIfDecoupledMeasurementSupport/CanIfNumberOfTxPdusExceedingQueueApiName"!]( (MainFuncId), (NoOfPdus) );
[!ENDIF!]


/** \def CANIF_TL_TX_CAN_ID_SUPPORT
 **
 ** \brief Switch for Tx CAN ID translation
 **
 ** This switch defines, if Tx CAN ID translation is supported.
 */

/** \def CANIF_TRANSLATE_TX_CAN_ID(CanId)
 **
 ** \brief Translate Tx CAN ID
 **
 ** This macro is used to translate the configured CAN ID of a Tx PDU into the
 ** CAN ID that shall really be used for the transmission.
 **
 ** \param[in] CanId CAN ID to be translated
 **
 ** \return Translated CAN ID
 */
[!IF "node:exists(CanIfDispatchCfg/CanIfTranslateTxCanIdFunc)"!]
#if (defined CANIF_TL_TX_CAN_ID_SUPPORT)
#error CANIF_TL_TX_CAN_ID_SUPPORT already defined!
#endif /* #if( defined CANIF_TL_TX_CAN_ID_SUPPORT)*/
#define CANIF_TL_TX_CAN_ID_SUPPORT        STD_ON

#if (defined CANIF_TRANSLATE_TX_CAN_ID)
#error CANIF_TRANSLATE_TX_CAN_ID already defined!
#endif /* #if( defined CANIF_TRANSLATE_TX_CAN_ID)*/
#define CANIF_TRANSLATE_TX_CAN_ID(CanId)  [!"CanIfDispatchCfg/CanIfTranslateTxCanIdFunc"!](CanId)

[!ELSE!]

#if (defined CANIF_TL_TX_CAN_ID_SUPPORT)
#error CANIF_TL_TX_CAN_ID_SUPPORT already defined!
#endif /* #if( defined CANIF_TL_TX_CAN_ID_SUPPORT)*/
#define CANIF_TL_TX_CAN_ID_SUPPORT        STD_OFF

#if (defined CANIF_TRANSLATE_TX_CAN_ID)
#error CANIF_TRANSLATE_TX_CAN_ID already defined!
#endif /* #if( defined CANIF_TRANSLATE_TX_CAN_ID)*/
#define CANIF_TRANSLATE_TX_CAN_ID(CanId)  (CanId)
[!ENDIF!]



/** \def CANIF_TL_RX_CAN_ID_SUPPORT
 **
 ** \brief Switch for Rx CAN ID translation
 **
 ** This switch defines, if Rx CAN ID translation is supported.
 */

/** \def CANIF_TRANSLATE_RX_CAN_ID(CanId)
 **
 ** \brief Translate Rx CAN ID
 **
 ** This macro is used to translate the CAN ID of a received message into the
 ** CAN ID, that was previously configured for the associated Rx PDU.
 **
 ** \param[in] CanId CAN ID to be translated
 **
 ** \return Translated CAN ID
 */
[!IF "node:exists(CanIfDispatchCfg/CanIfTranslateRxCanIdFunc)"!]
#if (defined CANIF_TL_RX_CAN_ID_SUPPORT)
#error CANIF_TL_RX_CAN_ID_SUPPORT already defined!
#endif /* #if( defined CANIF_TL_RX_CAN_ID_SUPPORT)*/
#define CANIF_TL_RX_CAN_ID_SUPPORT        STD_ON

#if (defined CANIF_TRANSLATE_RX_CAN_ID)
#error CANIF_TRANSLATE_RX_CAN_ID already defined!
#endif /* #if( defined CANIF_TRANSLATE_RX_CAN_ID)*/
#define CANIF_TRANSLATE_RX_CAN_ID(CanId)  [!"CanIfDispatchCfg/CanIfTranslateRxCanIdFunc"!](CanId)

[!ELSE!]

#if (defined CANIF_TL_RX_CAN_ID_SUPPORT)
#error CANIF_TL_RX_CAN_ID_SUPPORT already defined!
#endif /* #if( defined CANIF_TL_RX_CAN_ID_SUPPORT)*/
#define CANIF_TL_RX_CAN_ID_SUPPORT        STD_OFF

#if (defined CANIF_TRANSLATE_RX_CAN_ID)
#error CANIF_TRANSLATE_RX_CAN_ID already defined!
#endif /* #if( defined CANIF_TRANSLATE_RX_CAN_ID)*/
#define CANIF_TRANSLATE_RX_CAN_ID(CanId)  (CanId)
[!ENDIF!]


[!IF "$canifMFSupport"!]
  [!VAR "MaxQueuedTxPdusSize" = "num:i(0)"!]
  [!VAR "MaxQueuedRxPdusSize" = "num:i(0)"!]
  [!LOOP "CanIfPublicCfg/CanIfTxProcessing/*"!]
    [!VAR "MaxQueuedTxPdusSize" = "num:i($MaxQueuedTxPdusSize + CanIfTxPduQueueSize)"!]
  [!ENDLOOP!]

  [!LOOP "CanIfPublicCfg/CanIfRxProcessing/*"!]
    [!VAR "MaxQueuedRxPdusSize" = "num:i($MaxQueuedRxPdusSize + CanIfRxPduQueueSize)"!]
  [!ENDLOOP!]

#if (defined CANIF_MAX_QUEUED_TXPDUS_SIZE)
#error CANIF_MAX_QUEUED_TXPDUS_SIZE already defined!
#endif /* #if(defined CANIF_MAX_QUEUED_TXPDUS_SIZE) */
#define CANIF_MAX_QUEUED_TXPDUS_SIZE [!"$MaxQueuedTxPdusSize"!]U

#if (defined CANIF_MAX_QUEUED_RXPDUS_SIZE)
#error CANIF_MAX_QUEUED_RXPDUS_SIZE already defined!
#endif /* #if(defined CANIF_MAX_QUEUED_RXPDUS_SIZE) */
#define CANIF_MAX_QUEUED_RXPDUS_SIZE [!"$MaxQueuedRxPdusSize"!]U

#if (defined CANIF_RX_MF_SIZE)
#error CANIF_RX_MF_SIZE already defined!
#endif /* #if(defined CANIF_RX_MF_SIZE) */
#define CANIF_RX_MF_SIZE [!"num:i(count(CanIfPublicCfg/CanIfRxProcessing/*))"!]U

#if (defined CANIF_TX_MF_SIZE)
#error CANIF_TX_MF_SIZE already defined!
#endif /* #if(defined CANIF_TX_MF_SIZE) */
#define CANIF_TX_MF_SIZE [!"num:i(count(CanIfPublicCfg/CanIfTxProcessing/*))"!]U

[!ENDIF!]

#if (defined CANIF_BUS_MIRRORING_SUPPORT)
#error CANIF_BUS_MIRRORING_SUPPORT already defined !
#endif /* #if (defined CANIF_BUS_MIRRORING_SUPPORT) */
#define CANIF_BUS_MIRRORING_SUPPORT STD_[!"node:when($canIfBusMirrorSupport = 'true', 'ON', 'OFF')"!]

[!IF "$canIfBusMirrorSupport = 'true'"!]
#if (defined CANIF_CAN_DRV_TX_ERR_CNT_SUPPORTED)
#error CANIF_CAN_DRV_TX_ERR_CNT_SUPPORTED already defined !
#endif /* #if (defined CANIF_CAN_DRV_TX_ERR_CNT_SUPPORTED) */
#define CANIF_CAN_DRV_TX_ERR_CNT_SUPPORTED STD_[!"node:when($canIfCanTxErrCntSupported = 'true', 'ON', 'OFF')"!]

#if (defined CANIF_CAN_DRV_TX_ERR_CNT_DUMMY)
#error CANIF_CAN_DRV_TX_ERR_CNT_DUMMY already defined !
#endif /* #if (defined CANIF_CAN_DRV_TX_ERR_CNT_DUMMY) */
#define CANIF_CAN_DRV_TX_ERR_CNT_DUMMY [!"$canIfCanDummyTxErrorCount"!]U

#if (defined CANIF_CAN_GET_CTRL_STATE_SUPPORTED)
#error CANIF_CAN_GET_CTRL_STATE_SUPPORTED
#endif /* #if (defined CANIF_CAN_GET_CTRL_STATE_SUPPORTED) */
#define CANIF_CAN_GET_CTRL_STATE_SUPPORTED STD_[!"node:when($canIfCanGetCntrlStateSupported = 'true', 'ON', 'OFF')"!]

#if (defined CANIF_CAN_ERROR_STATE_DUMMY)
#error CANIF_CAN_ERROR_STATE_DUMMY already defined !
#endif /* #if (defined CANIF_CAN_ERROR_STATE_DUMMY) */
#define CANIF_CAN_ERROR_STATE_DUMMY [!"$canIfCanDummyErrorState"!]

[!ENDIF!][!/* $canIfBusMirrorSupport = 'true' */!]

#if (defined CANIF_HOOK_ON_RECEPTION_SUPPORT)
#error CANIF_HOOK_ON_RECEPTION_SUPPORT already defined !
#endif /* #if (defined CANIF_HOOK_ON_RECEPTION_SUPPORT) */
#define CANIF_HOOK_ON_RECEPTION_SUPPORT STD_[!"node:when($canIfHookOnReceptionSupport = 'true', 'ON', 'OFF')"!]

[!IF "$canIfHookOnReceptionSupport = 'true'"!]

#if (defined CANIF_HOOK_ON_RECEPTION_FUNCTION_NAME)
#error CANIF_HOOK_ON_RECEPTION_FUNCTION_NAME already defined !
#endif /* #if (defined CANIF_HOOK_ON_RECEPTION_FUNCTION_NAME) */
#define CANIF_HOOK_ON_RECEPTION_FUNCTION_NAME [!"CanIfHookOnRxSupport/CanIfHookOnReceptionFunctionName"!]

[!ENDIF!][!/* $canIfHookOnReceptionSupport = 'true' */!]

/* ----- Pre-processor switch to enable/diable relocatable postbuild config ----- */
#if (CANIF_PBCFGM_SUPPORT_ENABLED == STD_ON)
#if (defined CANIF_RELOCATABLE_CFG_ENABLE)
#error CANIF_RELOCATABLE_CFG_ENABLE already defined!
#endif /* #if( defined CANIF_RELOCATABLE_CFG_ENABLE)*/
#define CANIF_RELOCATABLE_CFG_ENABLE   [!WS "3"!][!//
[!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!/*
*/!]
#else
#if (defined CANIF_RELOCATABLE_CFG_ENABLE)
#error CANIF_RELOCATABLE_CFG_ENABLE already defined!
#endif /* #if( defined CANIF_RELOCATABLE_CFG_ENABLE)*/
#define CANIF_RELOCATABLE_CFG_ENABLE   [!WS "3"!][!//
[!IF "CanIfPublicCfg/CanIfRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!/*
*/!]
#endif

/** \brief Compile time verification value */
#if (defined CANIF_CFG_SIGNATURE)
#error CANIF_CFG_SIGNATURE already defined!
#endif /* #if( defined CANIF_CFG_SIGNATURE)*/
#define CANIF_CFG_SIGNATURE [!"asc:getConfigSignature(as:modconf('CanIf')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U

/** \brief Compile time Published information verfication value */
#if (defined CANIF_PUBLIC_INFO_SIGNATURE)
#error CANIF_PUBLIC_INFO_SIGNATURE already defined!
#endif /* #if( defined CANIF_PUBLIC_INFO_SIGNATURE)*/
#define CANIF_PUBLIC_INFO_SIGNATURE  [!"asc:getConfigSignature(node:difference(as:modconf('CanIf')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('CanIf')[1]/CommonPublishedInformation/Release))"!]U

/* ----- defensive programming macros ----- */





/*------------------[Defensive programming]---------------------------------*/
[!SELECT "CanIfDefensiveProgramming"!][!//

#if (defined CANIF_DEFENSIVE_PROGRAMMING_ENABLED)
#error CANIF_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define CANIF_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../CanIfPublicCfg/CanIfPublicDevErrorDetect  = 'true') and (CanIfDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANIF_PRECONDITION_ASSERT_ENABLED)
#error CANIF_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define CANIF_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../CanIfPublicCfg/CanIfPublicDevErrorDetect  = 'true') and (CanIfDefProgEnabled = 'true') and (CanIfPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANIF_POSTCONDITION_ASSERT_ENABLED)
#error CANIF_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define CANIF_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../CanIfPublicCfg/CanIfPublicDevErrorDetect  = 'true') and (CanIfDefProgEnabled = 'true') and (CanIfPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANIF_UNREACHABLE_CODE_ASSERT_ENABLED)
#error CANIF_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define CANIF_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../CanIfPublicCfg/CanIfPublicDevErrorDetect  = 'true') and (CanIfDefProgEnabled = 'true') and (CanIfUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANIF_INVARIANT_ASSERT_ENABLED)
#error CANIF_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define CANIF_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../CanIfPublicCfg/CanIfPublicDevErrorDetect  = 'true') and (CanIfDefProgEnabled = 'true') and (CanIfInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined CANIF_STATIC_ASSERT_ENABLED)
#error CANIF_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define CANIF_STATIC_ASSERT_ENABLED           [!//
[!IF "(../CanIfPublicCfg/CanIfPublicDevErrorDetect  = 'true') and (CanIfDefProgEnabled = 'true') and (CanIfStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//


/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef CANIF_CFG_H */
/*==================[end of file]============================================*/
[!ENDCODE!]
