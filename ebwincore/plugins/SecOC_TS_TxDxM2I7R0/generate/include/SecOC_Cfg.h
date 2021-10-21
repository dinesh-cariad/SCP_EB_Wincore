/**
 * \file
 *
 * \brief AUTOSAR SecOC
 *
 * This file contains the implementation of the AUTOSAR
 * module SecOC.
 *
 * \version 2.7.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef SECOC_CFG_H
#define SECOC_CFG_H

/*==================[deviations]============================================================*/

/*==================[includes]====================================================================*/

[!// This file contains the verification of the PDU IDs uniqueness
[!INCLUDE "SecOC_Checks.m"!][!//
[!//
/* !LINKSTO SecOC.Dsn.BasicSWStructure.RxTx.SecOC_Cfgh,2 */
[!IF "'true' = node:value(as:modconf('SecOC')/SecOCGeneral/SecOCRteUsage)"!][!//
#include <Rte_SecOC_Type.h>
[!ENDIF!][!//

#include <SecOC_Types.h>
#include <Csm.h>

/*==================[macros]======================================================================*/

#if (defined SECOC_PB_PDU_MEM_SIZE)
#error SECOC_PB_PDU_MEM_SIZE already defined
#endif
#define SECOC_PB_PDU_MEM_SIZE [!"node:value(as:modconf('SecOC')/SecOCGeneral/SecOCMaxPduBufferSize)"!]U

#if (defined SECOC_PB_INT_MEM_SIZE)
#error SECOC_PB_INT_MEM_SIZE already defined
#endif
#define SECOC_PB_INT_MEM_SIZE [!"node:value(as:modconf('SecOC')/SecOCGeneral/SecOCMaxIntBufferSize)"!]U

/* !LINKSTO SecOC.PB.Impl.Hash,1 */
#if (defined SECOC_CFG_SIGNATURE)
#error SECOC_CFG_SIGNATURE already defined
#endif
#define SECOC_CFG_SIGNATURE [!"asc:getConfigSignature(as:modconf('SecOC')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U

#if (defined SECOC_PUBLIC_INFO_SIGNATURE)
#error SECOC_PUBLIC_INFO_SIGNATURE already defined
#endif
#define SECOC_PUBLIC_INFO_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('SecOC')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('SecOC')[1]/CommonPublishedInformation/Release))"!]U

/* !LINKSTO SecOC.PB.Impl.PbCfgM,1 */
#if (defined SECOC_PBCFGM_SUPPORT_ENABLED)
#error SECOC_PBCFGM_SUPPORT_ENABLED already defined
#endif
#define SECOC_PBCFGM_SUPPORT_ENABLED [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('SecOC')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* ----- Pre-processor switch to enable/disable relocatable post build configuration ----- */

#if (defined SECOC_RELOCATABLE_CFG_ENABLE)
#error SECOC_RELOCATABLE_CFG_ENABLE already defined
#endif
#if (SECOC_PBCFGM_SUPPORT_ENABLED == STD_ON)
/* !LINKSTO SecOC.PB.Impl.PbcfgM.Relocatable,1 */
#define SECOC_RELOCATABLE_CFG_ENABLE [!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#else
/* !LINKSTO SecOC.PB.Impl.NoPbcfgM.Relocatable,1 */
#define SECOC_RELOCATABLE_CFG_ENABLE [!IF "as:modconf('SecOC')[1]/SecOCGeneral/SecOCRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#endif

[!SELECT "as:modconf('SecOC')"!][!//

#if (defined SECOC_RX_SYNC_PDU_PROCESSING)
#error SECOC_RX_SYNC_PDU_PROCESSING already defined
#endif
/** \brief Macro which defines whether there is at least one rx Pdu using synchronous processing or not.
*         STD_ON: One or more rx pdus are using synchronous processing
*         STD_OFF: Synchronous processing is not used by any rx Pdu
*/
/* !LINKSTO SecOC_EB_Rx_xxx18,1 */
/* !LINKSTO SecOC_EB_Rx_xxx19,1 */
#define SECOC_RX_SYNC_PDU_PROCESSING [!IF "num:i(count(SecOCRxPduProcessing/*[node:exists(SecOCRxSyncPduProcessing)][node:value(SecOCRxSyncPduProcessing) = 'true' and node:value(SecOcCsmMode) = 'SYNCHRONOUS'])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro which defines whether the GetRxFreshnessAuthData function shall be used or not.
  *        STD_ON: The GetRxFreshnessAuthData function shall be used based on the configuration
  *        STD_OFF: The GetRxFreshnessAuthData function is not used
  */
#if (defined SECOC_RX_AUTH_DATA_FRESHNESS)
  #error SECOC_RX_AUTH_DATA_FRESHNESS is already defined
#endif
#define  SECOC_RX_AUTH_DATA_FRESHNESS [!IF "(num:i(count(SecOCRxPduProcessing/*[node:value(SecOCUseAuthDataFreshness) = 'true'])) > 0) and (node:value(as:modconf('SecOC')/SecOCGeneral/SecOCQueryFreshnessValue) != 'NONE')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro which defines whether the GetRxFreshness function shall be used or not.
  *        STD_ON: The GetRxFreshness function shall be used based on the configuration
  *        STD_OFF: The GetRxFreshness function is not used
  */
#if (defined SECOC_RX_FRESHNESS)
  #error SECOC_RX_FRESHNESS is already defined
#endif
#define  SECOC_RX_FRESHNESS [!IF "(num:i(count(SecOCRxPduProcessing/*[node:value(SecOCUseAuthDataFreshness) = 'false'])) > 0) and (node:value(as:modconf('SecOC')/SecOCGeneral/SecOCQueryFreshnessValue) != 'NONE')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro which defines whether the GetTxFreshnessTruncData function shall be used or not.
  *        STD_ON: The GetTxFreshnessTruncData function shall be used based on the configuration
  *        STD_OFF: The GetTxFreshnessTruncData function is not used
  */
#if (defined SECOC_TX_FRESHNESS_TRUNC_DATA)
  #error SECOC_TX_FRESHNESS_TRUNC_DATA is already defined
#endif
#define  SECOC_TX_FRESHNESS_TRUNC_DATA [!IF "(num:i(count(SecOCTxPduProcessing/*[node:value(SecOCProvideTxTruncatedFreshnessValue) = 'true'])) > 0) and (node:value(as:modconf('SecOC')/SecOCGeneral/SecOCQueryFreshnessValue) != 'NONE')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro which defines whether the GetTxFreshness function shall be used or not.
  *        STD_ON: The GetTxFreshness function shall be used based on the configuration
  *        STD_OFF: The GetTxFreshness function is not used
  */
#if (defined SECOC_TX_FRESHNESS)
  #error SECOC_TX_FRESHNESS is already defined
#endif
#define  SECOC_TX_FRESHNESS [!IF "(num:i(count(SecOCTxPduProcessing/*[node:value(SecOCProvideTxTruncatedFreshnessValue) = 'false'])) > 0) and (node:value(as:modconf('SecOC')/SecOCGeneral/SecOCQueryFreshnessValue) != 'NONE')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined SECOC_TX_SYNC_PDU_PROCESSING)
#error SECOC_TX_SYNC_PDU_PROCESSING already defined
#endif
/** \brief Macro which defines whether there is at least one tx Pdu using synchronous processing or not.
*         STD_ON: One or more tx pdus are using synchronous processing
*         STD_OFF: Synchronous processing is not used by any tx Pdu
*/
/* !LINKSTO SecOC_EB_Tx_xxx24,1 */
/* !LINKSTO SecOC_EB_Tx_xxx25,1 */
#define SECOC_TX_SYNC_PDU_PROCESSING [!IF "num:i(count(SecOCTxPduProcessing/*[node:exists(SecOCTxSyncPduProcessing)][node:value(SecOCTxSyncPduProcessing) = 'true' and node:value(SecOcCsmMode) = 'SYNCHRONOUS'])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro, which defines the length in bytes of the Data ID element
 */
#if (defined SECOC_DATAID_LENGTH)
#error SECOC_DATAID_LENGTH already defined
#endif
#define SECOC_DATAID_LENGTH [!IF "node:value(SecOCGeneral/SecOCDataIdLength) = 'UINT8'"!]1U[!ELSEIF "node:value(SecOCGeneral/SecOCDataIdLength) = 'UINT16'"!]2U[!ELSE!]4U[!ENDIF!]


/** \brief Macro, which defines whether SecOC sends secured PDUS when the MAC could not be
 *         generated and is replaced by the configured default MAC.
 *
 * STD_ON, the secured PDUS are sent with the default configured MAC when the MAC could not be generated.
 * STD_OFF, the secured PDUS are not sent when the MAC could not be generated.
 */
#if (defined SECOC_USE_DEFAULT_AUTH)
#error SECOC_USE_DEFAULT_AUTH already defined
#endif
#define SECOC_USE_DEFAULT_AUTH [!IF "'true' = node:exists(SecOCGeneral/SecOCDefaultAuthenticatorValue)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "'true' = node:exists(SecOCGeneral/SecOCDefaultAuthenticatorValue)"!][!//
/** \brief Macro, which defines the default MAC value that shall be used to create the secured PDU
 *         when the MAC could not be generated.
 */
#if (defined SECOC_DEFAULT_AUTH_VALUE)
#error SECOC_DEFAULT_AUTH_VALUE already defined
#endif
#define SECOC_DEFAULT_AUTH_VALUE [!"num:i(SecOCGeneral/SecOCDefaultAuthenticatorValue)"!]U
[!ENDIF!][!//

/** \brief Macro, which defines whether the secured PDU collection is used or not.
 *
 * STD_ON, if at least one Rx PDU is configured with the secured PDU collection.
 * STD_OFF, if no Rx PDU is configured with the secured PDU collection.
 */
#if (defined SECOC_RX_USE_SECURED_PDU_COL)
#error SECOC_RX_USE_SECURED_PDU_COL already defined
#endif
#define SECOC_RX_USE_SECURED_PDU_COL [!IF "num:i(count(SecOCRxPduProcessing/*[SecOCRxSecuredPduLayer = 'SecOCRxSecuredPduCollection'])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro, which defines whether the message linker in the cryptographic Rx
 *         PDU is used or not.
 *
 * STD_ON, if at least one cryptographic PDU is configured to contain the message linker.
 * STD_OFF, if no cryptographic PDU is configured to contain the message linker.
 */
#if (defined SECOC_RX_USE_MESS_LINKER)
#error SECOC_RX_USE_MESS_LINKER already defined
#endif
#define SECOC_RX_USE_MESS_LINKER [!IF "num:i(count(SecOCRxPduProcessing/*/SecOCRxSecuredPduLayer/SecOCUseMessageLink)) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro, which defines whether the secured PDU collection is used or not.
 *
 * STD_ON, if at least one Tx PDU is configured with the secured PDU collection.
 * STD_OFF, if no Tx PDU is configured with the secured PDU collection.
 */
#if (defined SECOC_TX_USE_SECURED_PDU_COL)
#error SECOC_TX_USE_SECURED_PDU_COL already defined
#endif
#define SECOC_TX_USE_SECURED_PDU_COL [!IF "num:i(count(SecOCTxPduProcessing/*[SecOCTxSecuredPduLayer = 'SecOCTxSecuredPduCollection'])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro, which defines whether the message linker in the cryptographic Tx PDU
 *         is used or not.
 *
 * STD_ON, if at least one cryptographic PDU is configured to contain the message linker.
 * STD_OFF, if no cryptographic PDU is configured to contain the message linker.
 */
#if (defined SECOC_TX_USE_MESS_LINKER)
#error SECOC_TX_USE_MESS_LINKER already defined
#endif
#define SECOC_TX_USE_MESS_LINKER [!IF "num:i(count(SecOCTxPduProcessing/*/SecOCTxSecuredPduLayer/SecOCUseMessageLink)) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro, which defines whether the shaping of the secured PDU on the Rx side
 *         will be performed or not
 *
 * STD_ON, if the shaping of the secured PDU will be performed.
 * STD_OFF, if the shaping of the secured PDU will not be performed.
 */
#if (defined SECOC_RX_SHAPE_SEC_PDU)
#error SECOC_RX_SHAPE_SEC_PDU already defined
#endif
#define SECOC_RX_SHAPE_SEC_PDU [!IF "'true' = node:exists(SecOCGeneral/SecOCRxShapeFuncName)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro, which defines whether the shaping of the secured PDU on the Tx side
 *         will be performed or not
 *
 * STD_ON, if the shaping of the secured PDU will be performed.
 * STD_OFF, if the shaping of the secured PDU will not be performed.
 */
#if (defined SECOC_TX_SHAPE_SEC_PDU)
#error SECOC_TX_SHAPE_SEC_PDU already defined
#endif
#define SECOC_TX_SHAPE_SEC_PDU [!IF "'true' = node:exists(SecOCGeneral/SecOCTxShapeFuncName)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "'true' = node:exists(SecOCGeneral/SecOCRxShapeFuncName)"!]
/** \brief Macro, which defines the name of the shaping function on the Rx side
 */
#if (defined SECOC_RX_SHAPE_FUNC)
#error SECOC_RX_SHAPE_FUNC already defined
#endif
#define SECOC_RX_SHAPE_FUNC [!"SecOCGeneral/SecOCRxShapeFuncName"!]
[!ENDIF!]

[!IF "'true' = node:exists(SecOCGeneral/SecOCTxShapeFuncName)"!]
/** \brief Macro, which defines the name of the shaping function on the Tx side
 */
#if (defined SECOC_TX_SHAPE_FUNC)
#error SECOC_TX_SHAPE_FUNC already defined
#endif
#define SECOC_TX_SHAPE_FUNC [!"SecOCGeneral/SecOCTxShapeFuncName"!]
[!ENDIF!]

/** \brief Macro, which defines whether the Csm services for the Rx side are used
 *         also in asynchronous mode.
 *
 * STD_ON, if asynchronous mode is used for at least one configured PDU.
 * STD_OFF, if asynchronous mode is not used.
 */
#if (defined SECOC_RX_CSM_ASYNC)
#error SECOC_RX_CSM_ASYNC already defined
#endif
#define SECOC_RX_CSM_ASYNC [!IF "num:i(count(SecOCRxPduProcessing/*[SecOcCsmMode = 'ASYNCHRONOUS'])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro, which defines whether the Csm services for the Tx side are used
 *         also in asynchronous mode.
 *
 * STD_ON, if asynchronous mode is used for at least one configured PDU.
 * STD_OFF, if asynchronous mode is not used.
 */
#if (defined SECOC_TX_CSM_ASYNC)
#error SECOC_TX_CSM_ASYNC already defined
#endif
#define SECOC_TX_CSM_ASYNC [!IF "num:i(count(SecOCTxPduProcessing/*[SecOcCsmMode = 'ASYNCHRONOUS'])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro, which defines whether the MAC verification shall be performed on Secured I-PDU
 *
 * STD_ON, if verification is not performed.
 * STD_OFF, if verification is performed.
 */
#if (defined SECOC_SKIP_RX_PDU_VERIFICATION)
#error SECOC_SKIP_RX_PDU_VERIFICATION already defined
#endif
#define SECOC_SKIP_RX_PDU_VERIFICATION [!IF "num:i(count(SecOCRxPduProcessing/*[SecOCRxSecuredPduLayer/SecOCSecuredRxPduVerification = 'false'])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro, which defines whether the function SecOC_VerifyStatusOverride provides the ability
 *         to override the VerifyStatus with "pass" or not.
 *
 * STD_ON, if SecOC_VerifyStatusOverride provides the ability to set the result to "pass".
 * STD_OFF, if SecOC_VerifyStatusOverride does not provide the ability to set the result to "pass".
 */
#if (defined SECOC_OVERRIDE_VERIFYSTATUS_PASS)
#error SECOC_OVERRIDE_VERIFYSTATUS_PASS defined
#endif
#define SECOC_OVERRIDE_VERIFYSTATUS_PASS [!IF "'true' = node:value(SecOCGeneral/SecOCEnableForcedPassOverride)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro, which defines whether the Tx confirmation timeout is used or not.
 *
 * STD_ON, if Tx confirmation timeout is used.
 * STD_OFF, if Tx confirmation timeout is not used.
 */
#if (defined SECOC_USE_TX_CONFIRMATION_TIMEOUT)
#error SECOC_USE_TX_CONFIRMATION_TIMEOUT already defined
#endif
#define SECOC_USE_TX_CONFIRMATION_TIMEOUT [!IF "num:i(count(SecOCTxPduProcessing/*[SecOCTxConfirmationTimeout > 0])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro, which defines whether Autosar 4.0.3 APIs or 4.2.1 APIs of the PduR shall be used.
 *
 * STD_ON, if Autosar 4.0.3 APIs are used.
 * STD_OFF, if Autosar 4.2.1 APIs are used.
 */
#if (defined SECOC_ASR403_API)
#error SECOC_ASR403_API already defined
#endif
#define SECOC_ASR403_API [!IF "'true' = node:value(SecOCGeneral/SecOCASR403)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* !LINKSTO SecOC.Dsn.interface.RxTx.SECOC_INCLUDE_RTE, 1 */
/** \brief Macro, which defines whether Rte usage is configured for the SecOC.
 *
 * STD_ON, if Rte usage is enabled.
 * STD_OFF, if Rte usage is disabled.
 */
#if (defined SECOC_INCLUDE_RTE)
#error SECOC_INCLUDE_RTE already defined
#endif
#define SECOC_INCLUDE_RTE [!IF "'true' = node:value(SecOCGeneral/SecOCRteUsage)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro, which defines whether Det usage is configured for the SecOC.
 *
 * STD_ON, if development error detection is enabled.
 * STD_OFF, if development error detection is disabled.
 */
#if (defined SECOC_DET_ENABLED)
#error SECOC_DET_ENABLED already defined
#endif
#define SECOC_DET_ENABLED STD_OFF

/* !LINKSTO SecOC.Dsn.interface.Rx.SECOC_IGNORE_VER_RESULT,1 */
/** \brief Macro, which defines whether the verification result of the secured I-PDU shall be
 *         ignored.
 *
 * STD_ON,  verification result is ignored and PduR_SecOCRxIndication is called for secured I-PDU.
 * STD_OFF  only for verified secured I-PDU the PduR_SecOCRxIndication is called.
 */
#if (defined SECOC_IGNORE_VER_RESULT)
#error SECOC_IGNORE_VER_RESULT already defined
#endif
#define SECOC_IGNORE_VER_RESULT [!IF "'true' = node:value(SecOCGeneral/SecOcIgnoreVerificationResult)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro, which defines whether the SecOC provides the API SecOC_GetVersionInfo()
 *
 * STD_ON, if SecOC_GetVersionInfo() is available.
 * STD_OFF, if SecOC_GetVersionInfo() is not available.
 */
#if (defined SECOC_VERSION_INFO_API)
#error SECOC_VERSION_INFO_API already defined
#endif
#define SECOC_VERSION_INFO_API [!IF "'true' = node:value(SecOCGeneral/SecOCVersionInfoApi)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro which defines whether the authenticator length is passed to the cryptographic
 *         algorithm in bits or in bytes
 *         STD_ON:  Authenticator is passed in bits to the cryptographic algorithm
 *         STD_OFF: Authenticator is passed in bytes to the cryptographic algorithm
 */
#if (defined SECOC_AUTH_LENGTH_BITS)
#error SECOC_AUTH_LENGTH_BITS already defined
#endif
#define SECOC_AUTH_LENGTH_BITS [!IF "'true' = node:value(SecOCGeneral/SecOCCryptoBitLength)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro which defines whether the secured area within a PDU shall be used or not.
  *        STD_ON: Secured area within a PDU is used, the secured part of the authentic PDU will be sent authenticator
  *        STD_OFF: Secured area within a PDU is not used, the complete authentic PDU will be sent to the authenticator
  */
#if (defined SECOC_PDU_SECURED_AREA)
  #error SECOC_PDU_SECURED_AREA is already defined
#endif
#define  SECOC_PDU_SECURED_AREA [!IF "'true' = node:value(SecOCGeneral/SecOCUseSecuredArea)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Number of configured Rx Pdus, which shall be verified by the SecOC module
 */
#if (defined SECOC_NUMBER_RX_PDUS)
#error SECOC_NUMBER_RX_PDUS already defined
#endif
#define SECOC_NUMBER_RX_PDUS [!"num:i(count(SecOCRxPduProcessing/*))"!]U

/** \brief Number of configured Tx Pdus, which shall be authenticated by the SecOC module
 */
#if (defined SECOC_NUMBER_TX_PDUS)
#error SECOC_NUMBER_TX_PDUS already defined
#endif
#define SECOC_NUMBER_TX_PDUS [!"num:i(count(SecOCTxPduProcessing/*))"!]U

/** \brief Flag to indicate if PduR_SecOCCancelTransmit is available
 */
#if (defined SECOC_PDUR_SECOCCANCELTRANSMIT)
#error SECOC_PDUR_SECOCCANCELTRANSMIT already defined
#endif
#define SECOC_PDUR_SECOCCANCELTRANSMIT [!//
[!IF "'true' = node:value(as:modconf('PduR')/PduRBswModules/SecOC/PduRTransportProtocol) and
      'true' = node:value(as:modconf('PduR')/PduRBswModules/SecOC/PduRCancelTransmit)"!] [!//
STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief  How the current freshness value shall be determined.
 */
[!IF "node:value(as:modconf('SecOC')/SecOCGeneral/SecOCQueryFreshnessValue) = 'RTE'"!][!//
#if (defined SECOC_QUERY_FRESHNESS_VALUE)
#error SECOC_QUERY_FRESHNESS_VALUE already defined
#endif
#define SECOC_QUERY_FRESHNESS_VALUE SECOC_FRESHNESS_RTE
[!ELSEIF "node:value(as:modconf('SecOC')/SecOCGeneral/SecOCQueryFreshnessValue) = 'CFUNC'"!][!//
#if (defined SECOC_QUERY_FRESHNESS_VALUE)
#error SECOC_QUERY_FRESHNESS_VALUE already defined
#endif
#define SECOC_QUERY_FRESHNESS_VALUE SECOC_FRESHNESS_CFUNC
[!ELSE!][!//
#if (defined SECOC_QUERY_FRESHNESS_VALUE)
#error SECOC_QUERY_FRESHNESS_VALUE already defined
#endif
#define SECOC_QUERY_FRESHNESS_VALUE SECOC_FRESHNESS_NONE
[!ENDIF!][!//

/** \brief Configuration switch indicating whether the verification status shall be propagated for any Rx PDU
 */
#if (defined SECOC_VERIFICATION_STATUS_PROP_ENABLED)
#error SECOC_VERIFICATION_STATUS_PROP_ENABLED already defined
#endif
[!IF "node:value(as:modconf('SecOC')/SecOCGeneral/SecOCPropagateVerificationStatus) = 'AUTOSAR'"!][!//
#define SECOC_VERIFICATION_STATUS_PROP_ENABLED                  SECOC_VERIFICATION_STATUS_PROP_AUTOSAR
[!ELSEIF "node:value(as:modconf('SecOC')/SecOCGeneral/SecOCPropagateVerificationStatus) = 'EB_CUSTOM'"!][!//
#define SECOC_VERIFICATION_STATUS_PROP_ENABLED                  SECOC_VERIFICATION_STATUS_PROP_EB
[!ELSE!][!//
#define SECOC_VERIFICATION_STATUS_PROP_ENABLED                  SECOC_VERIFICATION_STATUS_PROP_NONE
[!ENDIF!][!//

/** \brief Configuration switch indicating whether the MAC Generate status shall be propagated for any Tx PDU
 */
#if (defined SECOC_MACGENERATE_STATUS_PROP_ENABLED)
#error SECOC_MACGENERATE_STATUS_PROP_ENABLED already defined
#endif
#define SECOC_MACGENERATE_STATUS_PROP_ENABLED [!IF "count(SecOCTxPduProcessing/*[node:value(SecOCMacGenerateStatusPropagationMode) != 'NONE']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro which defines the number of callout functions that propagates the verification status
 */
#if (defined SECOC_NUMBER_FUNC_VERIFICATION_STATUS_PROP)
#error SECOC_NUMBER_FUNC_VERIFICATION_STATUS_PROP already defined
#endif
#define SECOC_NUMBER_FUNC_VERIFICATION_STATUS_PROP [!"num:i(count(SecOCGeneral/SecOCVerificationStatusCallout/*))"!]U

/** \brief Macro which defines the number of callout functions that propagates the MAC Generate status
 */
#if (defined SECOC_NUMBER_FUNC_MACGENERATE_STATUS_PROP)
#error SECOC_NUMBER_FUNC_MACGENERATE_STATUS_PROP already defined
#endif
#define SECOC_NUMBER_FUNC_MACGENERATE_STATUS_PROP [!"num:i(count(SecOCGeneral/SecOCMacGenerateStatusCallout/*))"!]U

/** \brief Macro switch for TP Interface for transmission
 */
#if (defined SECOC_TX_USETP)
#error SECOC_TX_USETP already defined
#endif
#define SECOC_TX_USETP [!IF "count(SecOCTxPduProcessing/*[node:value(./SecOCTxAuthenticPduLayer/SecOCPduType)='SECOC_TPPDU']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro switch for TP Interface for reception
 */
#if (defined SECOC_RX_USETP)
#error SECOC_RX_USETP already defined
#endif
#define SECOC_RX_USETP [!IF "count(SecOCRxPduProcessing/*[node:value(./SecOCRxAuthenticPduLayer/SecOCPduType)='SECOC_TPPDU']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Configuration switch indicating whether the "same buffer PDU collection" shall be used or not on the Rx side
 */
#if (defined SECOC_RX_USE_SAME_BUFFER)
#error SECOC_RX_USE_SAME_BUFFER already defined
#endif
#define SECOC_RX_USE_SAME_BUFFER [!IF "count(SecOCRxPduProcessing/*[node:exists(./SecOCSameBufferPduRef) = true()]) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Configuration switch indicating whether the "same buffer PDU collection" shall be used or not on the Tx side
 */
#if (defined SECOC_TX_USE_SAME_BUFFER)
#error SECOC_TX_USE_SAME_BUFFER already defined
#endif
#define SECOC_TX_USE_SAME_BUFFER [!IF "count(SecOCTxPduProcessing/*[node:exists(./SecOCSameBufferPduRef) = true()]) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro, which defines whether SecOC provides the ability to bypass the authentication routine or not
 *
 * STD_ON, the bypass mechanism is available
 * STD_OFF, the bypass mechanism is not available
 */
#if (defined SECOC_BYPASS_AUTHENTICATION_ROUTINE)
#error SECOC_BYPASS_AUTHENTICATION_ROUTINE already defined
#endif
#define SECOC_BYPASS_AUTHENTICATION_ROUTINE [!IF "'true' = node:exists(SecOCGeneral/SecOCBypassAuthenticationRoutine)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "'true' = node:exists(SecOCGeneral/SecOCBypassAuthenticationRoutine)"!][!//
/** \brief Macro, which defines the default authentication information value that shall be used
 *         to create the secured PDU when the bypass mechanism is enabled.
 */
#if (defined SECOC_BYPASS_DEFAULT_AUTHINFO_VALUE)
#error SECOC_BYPASS_DEFAULT_AUTHINFO_VALUE already defined
#endif
#define SECOC_BYPASS_DEFAULT_AUTHINFO_VALUE [!"num:i(SecOCGeneral/SecOCBypassAuthenticationRoutine/SecOCDefaultAuthenticationInfoValue)"!]U
[!ENDIF!][!//

/** \brief Macro, which defines if SecOC calls during the initialization phase for every PDU an
 *         external API defined by SecOCCsmJobRefCallout to obtain the Csm job ID that shall be
 *         used together with the Csm services
 *
 * STD_ON, the callout will be executed to obtain the Csm job ID
 * STD_OFF, no callout shall be done
 */
#if (defined SECOC_USE_CSMJOBREF_CALLOUT)
#error SECOC_USE_CSMJOBREF_CALLOUT already defined
#endif
#define SECOC_USE_CSMJOBREF_CALLOUT [!IF "'true' = node:exists(SecOCGeneral/SecOCCsmJobRefCallout)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "'true' = node:exists(SecOCGeneral/SecOCCsmJobRefCallout)"!][!//
/** \brief Macro, which defines the name of the callout function
 *         used to obtain the Csm job ID
 */
#if (defined SECOC_CSMJOBREF_CALLOUT)
#error SECOC_CSMJOBREF_CALLOUT already defined
#endif
#define SECOC_CSMJOBREF_CALLOUT     [!"node:value(SecOCGeneral/SecOCCsmJobRefCallout)"!]
[!ENDIF!][!//

/*==================[type definitions]============================================================*/

/** \brief Length type of the Data ID element.
 */
[!IF "node:value(SecOCGeneral/SecOCDataIdLength) = 'UINT8'"!][!//
typedef uint8 SecOC_DataIdLengthType;
[!ELSE!][!//
[!IF "node:value(SecOCGeneral/SecOCDataIdLength) = 'UINT16'"!][!//
typedef uint16 SecOC_DataIdLengthType;
[!ELSE!][!//
typedef uint32 SecOC_DataIdLengthType;
[!ENDIF!][!//
[!ENDIF!][!//

#ifndef RTE_TYPE_SecOC_VerificationStatusType
/** \cond SECOC_DOC_INTERNAL_MACROS */
#define RTE_TYPE_SecOC_VerificationStatusType
/** \endcond */
/* !LINKSTO SWS_SecOC_Rx_00160,4 */
/**
 * \struct SecOC_VerificationStatusType
 * \brief Data structure to bundle the status of a verification attempt for a specific Freshness
 *        Value and Data ID.
 *//**
 * \var SecOC_VerificationStatusType::freshnessValueID
 * \brief Identifier of the Freshness Value which resulted in the Verification Result
 *//**
 * \var SecOC_VerificationStatusType::verificationStatus
 * \brief Result of verification attempt.
 *//**
 * \var SecOC_VerificationStatusType::secOCDataId
 * \brief Identifier for the Secured I-PDU.
 *//**
 * \var SecOC_VerificationStatusType::verificationReturn
 * \brief Result of verification attempt.
 */
typedef struct
{
  uint16                       freshnessValueID;
  SecOC_VerificationResultType verificationStatus;
  SecOC_DataIdLengthType       secOCDataId;
#if (SECOC_VERIFICATION_STATUS_PROP_EB == SECOC_VERIFICATION_STATUS_PROP_ENABLED)
  Std_ReturnType               verificationReturn;
#endif
}
SecOC_VerificationStatusType;
#endif

#ifndef RTE_TYPE_SecOC_MacGenerateStatusType
/** \cond SECOC_DOC_INTERNAL_MACROS */
#define RTE_TYPE_SecOC_MacGenerateStatusType
/** \endcond */
/* !LINKSTO SecOC_EB_Tx_xxx14,2 */
/**
 * \struct SecOC_MacGenerateStatusType
 * \brief Data structure to bundle the status of a MAC generate attempt for a specific Freshness
 *        Value and Data ID.
 *//**
 * \var SecOC_MacGenerateStatusType::freshnessValueID
 * \brief Identifier of the Freshness Value which resulted in the Verification Result
 *//**
 * \var SecOC_MacGenerateStatusType::macGenerateStatus
 * \brief Result of the MAC Generate procedure.
 *//**
 * \var SecOC_MacGenerateStatusType::secOCDataId
 * \brief Identifier for the Secured I-PDU.
 */
typedef struct
{
  uint16                 freshnessValueID;
  Std_ReturnType         macGenerateStatus;
  SecOC_DataIdLengthType secOCDataId;
}
SecOC_MacGenerateStatusType;
#endif

[!IF "(count(SecOCGeneral/SecOCVerificationStatusCallout/*) > 0) and (node:value(SecOCGeneral/SecOCPropagateVerificationStatus) != 'NONE')"!][!//
/**
 * \brief Pointer to verification status callout function type
 */
typedef P2FUNC(void, TYPEDEF, SecOC_VerificationStatusCalloutType) (SecOC_VerificationStatusType verificationStatus);
[!ENDIF!][!//

[!IF "(count(SecOCGeneral/SecOCMacGenerateStatusCallout/*) > 0) and (count(SecOCTxPduProcessing/*[node:value(SecOCMacGenerateStatusPropagationMode) != 'NONE']) > 0)"!][!//
/**
 * \brief Pointer to MAC Generate status callout function type
 */
typedef P2FUNC(void, TYPEDEF, SecOC_MacGenerateStatusCalloutType) (SecOC_MacGenerateStatusType macGenerateStatus);
[!ENDIF!][!//

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

#define SECOC_START_SEC_VAR_CLEARED_8
#include <SecOC_MemMap.h>
[!IF "num:i(count(SecOCRxPduProcessing/*[node:exists(SecOCSameBufferPduRef) = true()])) > 0"!][!//
/**
 * \brief Flags indicating, if the buffer is currently being used
 */
extern VAR(uint8, SECOC_VAR) SecOC_RxBufferUsedFlags[[!"num:i(count(node:refs(SecOCRxPduProcessing/*[node:exists(./SecOCSameBufferPduRef) = true()]/SecOCSameBufferPduRef)))"!]];
[!ENDIF!][!//

[!IF "num:i(count(SecOCTxPduProcessing/*[node:exists(SecOCSameBufferPduRef) = true()])) > 0"!][!//
/**
 * \brief Flags indicating, if the buffer is currently being used
 */
extern VAR(uint8, SECOC_VAR) SecOC_TxBufferUsedFlags[[!"num:i(count(node:refs(SecOCTxPduProcessing/*[node:exists(./SecOCSameBufferPduRef) = true()]/SecOCSameBufferPduRef)))"!]];
[!ENDIF!][!//

#define SECOC_STOP_SEC_VAR_CLEARED_8
#include <SecOC_MemMap.h>

/*==================[external function declarations]==============================================*/

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

/* !LINKSTO  SecOC_EB_RxTx_xxx12,1 */
[!IF "'true' = node:exists(SecOCGeneral/SecOCCsmJobRefCallout)"!][!//
/** \brief This function is used by the SecOC to obtain the Csm job ID which shall be used
 *         when calling the Csm services. The call will be made during the initialization phase
 *         for every Rx and Tx PDU configured
 *
 * \param[in]       JobId          the Csm job ID extracted from service references,
 *                                 set during the pre-compile time, during the configuration phase
 *
 */
extern FUNC(uint32, SECOC_APPL_CODE) [!"SecOCGeneral/SecOCCsmJobRefCallout"!]
(
   uint32 JobId
);
[!ENDIF!][!//

[!IF "'true' = node:exists(SecOCGeneral/SecOCRxShapeFuncName)"!]
/** \brief This interface is used by the SecOC to remove the padding within the secured PDU.
 *
 * \param[in]       SecOCPduID          Holds the identifier of the secured PDU
 *                                      or the identifier of the received authentic PDU,
                                        when the Secured PDU Collection is used, at SecOC.
 * \param[in,out]   SecPdu              Holds the secured PDU.
 * \param[in]       SrcSecPduLength     Holds the length of the received secured PDU.
 * \param[in,out]   DstSecPduLength     in: Holds the maximum length of the secured PDU.
 *                                      out:Holds the length of the secured PDU without the padding.
 * \param[in]       AuthenticatorLength Holds the length of the authenticator.
 *
 */
extern FUNC(void, SECOC_APPL_CODE) [!"SecOCGeneral/SecOCRxShapeFuncName"!]
(
          PduIdType                                  SecOCPduID,
    P2VAR(uint8,         AUTOMATIC, SECOC_APPL_DATA) SecPdu,
  P2CONST(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) SrcSecPduLength,
    P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) DstSecPduLength,
          uint32                                     AuthenticatorLength
);
[!ENDIF!][!//

[!IF "'true' = node:exists(SecOCGeneral/SecOCTxShapeFuncName)"!]
/** \brief This interface is used by the SecOC to to add the required padding within
 *         the secured PDU to maintain a fixed layout.
 *
 * \param[in]       SecOCPduID          Holds the identifier of the received authentic PDU at SecOC.
 * \param[in,out]   SecPdu              Holds the secured PDU.
 * \param[in]       SrcSecPduLength     Holds the length of the generated secured PDU without
 *                                      the required padding.
 * \param[in,out]   DstSecPduLength     in:  Holds the maximum length of the secured PDU.
 *                                      out: Holds the length of the secured PDU with the padding.
 * \param[in]       AuthenticatorLength Holds the length of the authenticator.
 *
 */
extern FUNC(void, SECOC_APPL_CODE) [!"SecOCGeneral/SecOCTxShapeFuncName"!]
(
          PduIdType                                  SecOCPduID,
    P2VAR(uint8,         AUTOMATIC, SECOC_APPL_DATA) SecPdu,
  P2CONST(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) SrcSecPduLength,
    P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) DstSecPduLength,
          uint32                                     AuthenticatorLength
);
[!ENDIF!][!//

[!IF "node:value(SecOCGeneral/SecOCPropagateVerificationStatus) != 'NONE'"!][!//
[!LOOP "SecOCGeneral/SecOCVerificationStatusCallout/*"!][!//

/* !LINKSTO SWS_SecOC_Rx_00119,1 */
/** \brief Function which reports the status of the verification procedure
 *
 * \param[in] verificationStatus
 *
 */
extern FUNC(void, SECOC_APPL_CODE) [!"node:value(.)"!]
(
  SecOC_VerificationStatusType verificationStatus
);

[!ENDLOOP!][!//
[!ENDIF!][!//
[!//
[!IF "count(SecOCTxPduProcessing/*[node:value(SecOCMacGenerateStatusPropagationMode) != 'NONE']) > 0"!][!//
[!LOOP "SecOCGeneral/SecOCMacGenerateStatusCallout/*"!][!//

/* !LINKSTO SecOC_EB_Tx_xxx13,2 */
/** \brief Function which reports the status of the MAC Generate service
 *
 * \param[in] macGenerateStatus
 *
 */
extern FUNC(void, SECOC_APPL_CODE) [!"node:value(.)"!]
(
  SecOC_MacGenerateStatusType macGenerateStatus
);

[!ENDLOOP!][!//
[!ENDIF!][!//
[!//
[!IF "node:value(SecOCGeneral/SecOCQueryFreshnessValue) = 'CFUNC'"!][!//
[!IF "(count(SecOCTxPduProcessing/*) > 0)"!][!//
[!VAR "lastFuncName" = "''"!][!//
[!LOOP "node:order(SecOCTxPduProcessing/*[SecOCProvideTxTruncatedFreshnessValue = 'false'], './SecOCFreshnessValueFuncName')"!][!//
[!IF "$lastFuncName != node:value(./SecOCFreshnessValueFuncName)"!][!//
/* !LINKSTO SWS_SecOC_Tx_xx013,3 */
/**
 * \brief This interface is used by the SecOC to obtain the current freshness value.
 *
 * \param[in]    SecOCFreshnessValueID     Holds the identifier of the freshness value.
 * \param[out]   SecOCFreshnessValue       Holds the current freshness value.
 * \param[in]    SecOCFreshnessValueLength Holds the length of the required freshness value in bits.
 *
 * \returns whether the request was successful or not.
 * \retval E_OK     Request successful.
 * \retval E_NOT_OK Request failed, a freshness value cannot be provided due to general issues for
 *                  freshness or this FreshnessValueId.
 * \retval E_BUSY   The freshness information can temporarily not be provided.
 *
 */
extern FUNC(Std_ReturnType, SECOC_CODE) [!"node:value(./SecOCFreshnessValueFuncName)"!]
(
        uint16                              SecOCFreshnessValueID,
  P2VAR(uint8,  AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValue,
  P2VAR(uint32, AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValueLength
);

[!VAR "lastFuncName" = "node:value(./SecOCFreshnessValueFuncName)"!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!VAR "lastFuncName" = "''"!][!//
[!LOOP "node:order(SecOCTxPduProcessing/*[SecOCProvideTxTruncatedFreshnessValue = 'true'], './SecOCFreshnessValueFuncName')"!][!//
[!IF "$lastFuncName != node:value(./SecOCFreshnessValueFuncName)"!][!//
/* !LINKSTO SWS_SecOC_Tx_xx016,1 */
/**
 * \brief This interface is used by the SecOC to obtain the current freshness value.
 *
 * \param[in]    SecOCFreshnessValueID              Holds the identifier of the freshness value.
 * \param[in]    SecOCFreshnessValueLength          Holds the length of the required freshness value
 *                                                  in bits.
 * \param[in]    SecOCTruncatedFreshnessValueLength Holds the length of the required truncated
 *                                                  freshness value in bits.
 * \param[out]   SecOCFreshnessValue                Holds the current freshness value.
 * \param[out]   SecOCTruncatedFreshnessValue       Holds the current truncated freshness value.
 *
 * \returns whether the request was successful or not.
 * \retval E_OK     Request successful.
 * \retval E_NOT_OK Request failed, a freshness value cannot be provided due to general issues for
 *                  freshness or this FreshnessValueId.
 * \retval E_BUSY   The freshness information can temporarily not be provided.
 *
 */
extern FUNC(Std_ReturnType, SECOC_CODE) [!"node:value(./SecOCFreshnessValueFuncName)"!]
(
        uint16                              SecOCFreshnessValueID,
  P2VAR(uint8,  AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValue,
  P2VAR(uint32, AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValueLength,
  P2VAR(uint8,  AUTOMATIC, SECOC_APPL_DATA) SecOCTruncatedFreshnessValue,
  P2VAR(uint32, AUTOMATIC, SECOC_APPL_DATA) SecOCTruncatedFreshnessValueLength
);

[!VAR "lastFuncName" = "node:value(./SecOCFreshnessValueFuncName)"!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!VAR "lastFuncName" = "''"!][!//
[!LOOP "node:order(SecOCTxPduProcessing/*, './SecOCSecuredPDUTransmittedFuncName')"!][!//
[!IF "$lastFuncName != node:value(./SecOCSecuredPDUTransmittedFuncName)"!][!//
/* !LINKSTO SWS_SecOC_Tx_xx014,2 */
/**
 * \brief This interface is used by the SecOC to indicate that the Secured I-PDU has been initiated
 *        for transmission.
 *
 * \param[in]    SecOCFreshnessValueID     Holds the identifier of the freshness value.
 */
extern FUNC(void, SECOC_CODE) [!"node:value(./SecOCSecuredPDUTransmittedFuncName)"!]
(
  uint16 SecOCFreshnessValueID
);

[!VAR "lastFuncName" = "node:value(./SecOCSecuredPDUTransmittedFuncName)"!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDIF!][!//
[!//
[!IF "(count(SecOCRxPduProcessing/*) > 0)"!][!//
[!//
[!VAR "lastFuncName" = "''"!][!//
[!LOOP "node:order(SecOCRxPduProcessing/*[SecOCUseAuthDataFreshness = 'false'], './SecOCFreshnessValueFuncName')"!][!//
[!IF "$lastFuncName != node:value(./SecOCFreshnessValueFuncName)"!][!//
/* !LINKSTO SWS_SecOC_Rx_xx028,3 */
/**
 * \brief This interface is used by the SecOC to obtain the current freshness value.
 *
 * \param[in]    SecOCFreshnessValueID              Holds the identifier of the freshness value.
 * \param[in]    SecOCTruncatedFreshnessValue       Holds the truncated freshness value that was
 *                                                  contained in the Secured I-PDU.
 * \param[in]    SecOCTruncatedFreshnessValueLength Holds the length in bits of the truncated
 *                                                  freshness value.
 * \param[in]    SecOCCounterSyncAttempts           Holds the number of authentication verify
 *                                                  attempts of this PDU since the last reception.
 *                                                  The value is 0 for the first attempt and
 *                                                  incremented on every unsuccessful verification
 *                                                  attempt.
 * \param[out]   SecOCFreshnessValue                Holds the freshness value to be used for the
 *                                                  calculation of the authenticator.
 * \param[in]    SecOCFreshnessValueLength          Holds the length in bits of the freshness value.
 *
 * \returns whether the request was successful or not.
 * \retval E_OK     Request successful.
 * \retval E_NOT_OK Request failed, a freshness value cannot be provided due to general issues for
 *                  freshness or this FreshnessValueId.
 * \retval E_BUSY   The freshness information can temporarily not be provided.
 *
 */
extern FUNC(Std_ReturnType, SECOC_CODE) [!"node:value(./SecOCFreshnessValueFuncName)"!]
(
          uint16                              SecOCFreshnessValueID,
  P2CONST(uint8,  AUTOMATIC, SECOC_APPL_DATA) SecOCTruncatedFreshnessValue,
          uint32                              SecOCTruncatedFreshnessValueLength,
          uint16                              SecOCCounterSyncAttempts,
    P2VAR(uint8,  AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValue,
    P2VAR(uint32, AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValueLength
);

[!VAR "lastFuncName" = "node:value(./SecOCFreshnessValueFuncName)"!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//

[!VAR "lastFuncName" = "''"!][!//
[!LOOP "node:order(SecOCRxPduProcessing/*[SecOCUseAuthDataFreshness = 'true'], './SecOCFreshnessValueFuncName')"!][!//
[!IF "$lastFuncName != node:value(./SecOCFreshnessValueFuncName)"!][!//
/* !LINKSTO SWS_SecOC_Rx_91006,1 */
/**
 * \brief This interface is used by the SecOC to obtain the current freshness value.
 *
 * \param[in]    SecOCFreshnessValueID              Holds the identifier of the freshness value.
 * \param[in]    SecOCTruncatedFreshnessValue       Holds the truncated freshness value that was
 *                                                  contained in the Secured I-PDU.
 * \param[in]    SecOCTruncatedFreshnessValueLength Holds the length in bits of the truncated
 *                                                  freshness value.
 * \param[in]    SecOCAuthDataFreshnessValue        The parameter holds a part of the
 *                                                  received, not yet authenticated PDU.
 * \param[in]    SecOCAuthDataFreshnessValueLength  This is the length value in bits that holds
 *                                                  the freshness from the authentic PDU.
 * \param[in]    SecOCAuthVerifyAttempts            Holds the number of authentication verify
 *                                                  attempts of this PDU since the last reception.
 *                                                  The value is 0 for the first attempt and
 *                                                  incremented on every unsuccessful verification
 *                                                  attempt.
 * \param[out]   SecOCFreshnessValue                Holds the freshness value to be used for the
 *                                                  calculation of the authenticator.
 * \param[in]    SecOCFreshnessValueLength          Holds the length in bits of the freshness value.
 *
 * \returns whether the request was successful or not.
 * \retval E_OK     Request successful.
 * \retval E_NOT_OK Request failed, a freshness value cannot be provided due to general issues for
 *                  freshness or this FreshnessValueId.
 * \retval E_BUSY   The freshness information can temporarily not be provided.
 *
 */
extern FUNC(Std_ReturnType, SECOC_CODE) [!"node:value(./SecOCFreshnessValueFuncName)"!]
(
          uint16                              SecOCFreshnessValueID,
  P2CONST(uint8,  AUTOMATIC, SECOC_APPL_DATA) SecOCTruncatedFreshnessValue,
          uint32                              SecOCTruncatedFreshnessValueLength,
  P2CONST(uint8,  AUTOMATIC, SECOC_APPL_DATA) SecOCAuthDataFreshnessValue,
          uint16                              SecOCAuthDataFreshnessValueLength,
          uint16                              SecOCAuthVerifyAttempts,
    P2VAR(uint8,  AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValue,
    P2VAR(uint32, AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValueLength
);

[!VAR "lastFuncName" = "node:value(./SecOCFreshnessValueFuncName)"!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDIF!][!//
[!ENDIF!][!//
#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>
[!ENDSELECT!][!//

#endif /* #ifndef SECOC_CFG_H */

/*==================[end of file]=================================================================*/
