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

/*==================[misra deviations]============================================================*/

/*==================[includes]====================================================================*/

/* !LINKSTO SecOC.Dsn.BasicSWStructure.RxTx.SecOC_Cfgc,2 */
#include <SecOC_Cfg.h>
#include <SecOC_Int.h>
[!IF "'true' = node:value(as:modconf('SecOC')/SecOCGeneral/SecOCRteUsage)"!][!//
#include <Rte_SecOC.h>
[!ENDIF!][!//

/*==================[macros]======================================================================*/

[!SELECT "as:modconf('SecOC')"!][!//

[!VAR "RxBufferIndex" = "0"!][!//
[!LOOP "node:order(node:refs(SecOCRxPduProcessing/*[node:exists(./SecOCSameBufferPduRef) = true()]/SecOCSameBufferPduRef))"!][!//
/** \brief Macro which indicates the position of the "currently being used" flag for
 *         [!"node:name(.)"!] buffer inside the SecOC_RxBufferUsedFlags flags array
 */
#if (defined SECOC_RX_BUFFER_USED_FLAG_[!"text:toupper(node:name(.))"!])
#error SECOC_RX_BUFFER_USED_FLAG_[!"text:toupper(node:name(.))"!] already defined
#endif
#define SECOC_RX_BUFFER_USED_FLAG_[!"text:toupper(node:name(.))"!] [!"num:i($RxBufferIndex)"!]U

[!VAR "RxBufferIndex" = "$RxBufferIndex + 1"!][!//
[!ENDLOOP!][!//

[!VAR "TxBufferIndex" = "0"!][!//
[!LOOP "node:order(node:refs(SecOCTxPduProcessing/*[node:exists(./SecOCSameBufferPduRef) = true()]/SecOCSameBufferPduRef))"!][!//
/** \brief Macro which indicates the position of the "currently being used" flag for
 *         [!"node:name(.)"!] buffer inside the SecOC_TxBufferUsedFlags flags array
 */
#if (defined SECOC_TX_BUFFER_USED_FLAG_[!"text:toupper(node:name(.))"!])
#error SECOC_TX_BUFFER_USED_FLAG_[!"text:toupper(node:name(.))"!] already defined
#endif
#define SECOC_TX_BUFFER_USED_FLAG_[!"text:toupper(node:name(.))"!] [!"num:i($TxBufferIndex)"!]U

[!VAR "TxBufferIndex" = "$TxBufferIndex + 1"!][!//
[!ENDLOOP!][!//

/* Macro, which defines the default value for the dummy Csm job ID */
#if (defined SECOC_DUMMY_CSM_JOB_ID)
#error SECOC_DUMMY_CSM_JOB_ID already defined
#endif

#define SECOC_DUMMY_CSM_JOB_ID   4294967295U

/*==================[type definitions]============================================================*/

/*==================[internal function declarations]==============================================*/

/*==================[internal constants]==========================================================*/

/*==================[internal data]===============================================================*/

/*==================[external constants]==========================================================*/

#define SECOC_START_SEC_CONST_UNSPECIFIED
#include <SecOC_MemMap.h>

#if (SECOC_NUMBER_RX_PDUS > 0)

[!IF "(count(SecOCGeneral/SecOCVerificationStatusCallout/*) > 0) and (node:value(SecOCGeneral/SecOCPropagateVerificationStatus) != 'NONE')"!][!//
CONST(SecOC_VerificationStatusCalloutType, SECOC_VAR) SecOC_RxVerificationStatusCallout[SECOC_NUMBER_FUNC_VERIFICATION_STATUS_PROP] =
{
[!LOOP "SecOCGeneral/SecOCVerificationStatusCallout/*"!][!//
  &[!"node:value(.)"!],
[!ENDLOOP!][!//
};
[!ENDIF!][!//

[!IF "count(SecOCGeneral/SecOCMacGenerateStatusCallout/*) > 0"!][!//
[!IF "count(SecOCTxPduProcessing/*[node:value(SecOCMacGenerateStatusPropagationMode) != 'NONE']) > 0"!][!//
CONST(SecOC_MacGenerateStatusCalloutType, SECOC_VAR) SecOC_TxMacGenerateStatusCallout[SECOC_NUMBER_FUNC_MACGENERATE_STATUS_PROP] =
{
[!LOOP "SecOCGeneral/SecOCMacGenerateStatusCallout/*"!][!//
  &[!"node:value(.)"!],
[!ENDLOOP!][!//
};
[!ENDIF!][!//
[!ENDIF!][!//

CONST(SecOC_RxConfigType, SECOC_CONST) SecOC_RxConfig[SECOC_NUMBER_RX_PDUS] =
{
[!VAR "NumberOfPduWithAuthDataFreshness" = "num:i(count(SecOCRxPduProcessing/*[SecOCUseAuthDataFreshness = 'true']))"!][!//
[!VAR "NumberOfPduWithFreshness" = "num:i(count(SecOCRxPduProcessing/*[SecOCUseAuthDataFreshness = 'false']))"!][!//
[!VAR "NumberOfPduWithSkipVerification" = "num:i(count(SecOCRxPduProcessing/*[SecOCRxSecuredPduLayer/SecOCSecuredRxPduVerification = 'false']))"!][!//
[!VAR "NumberOfPduWithCsmAsync" = "num:i(count(SecOCRxPduProcessing/*[SecOcCsmMode = 'ASYNCHRONOUS']))"!][!//
[!VAR "NumberOfPduWithCollection" = "num:i(count(SecOCRxPduProcessing/*[SecOCRxSecuredPduLayer = 'SecOCRxSecuredPduCollection']))"!][!//
[!VAR "NumberOfPduWithMessLinker" = "num:i(count(SecOCRxPduProcessing/*/SecOCRxSecuredPduLayer/SecOCUseMessageLink/SecOCMessageLinkLen))"!][!//
[!VAR "NumberOfPduWithSyncProcessing" = "num:i(count(SecOCRxPduProcessing/*[node:exists(SecOCRxSyncPduProcessing)][node:value(SecOCRxSyncPduProcessing) = 'true' and node:value(SecOcCsmMode) = 'SYNCHRONOUS']))"!][!//
[!//
[!LOOP "node:order(SecOCRxPduProcessing/*)"!][!//
[!//
[!IF "$NumberOfPduWithAuthDataFreshness > 0"!][!//
[!IF "'true' = node:value(./SecOCUseAuthDataFreshness)"!][!//
[!VAR "AuthDataFreshnessLen" = "node:value(./SecOCAuthDataFreshnessLen)"!][!//
[!VAR "AuthDataFreshnessStartPosition" = "node:value(./SecOCAuthDataFreshnessStartPosition)"!][!//
[!ELSE!][!//
[!VAR "AuthDataFreshnessLen" = "0"!][!//
[!VAR "AuthDataFreshnessStartPosition" = "0"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!//
  /* [!"node:name(node:current())"!] */
  {
[!IF "node:value(./SecOCRxSecuredPduLayer/SecOCSecuredRxPduVerification) = 'true'"!][!//
    /* CsmJobId */
    CsmConf_CsmJob_[!"as:name(as:ref(./SecOCRxAuthServiceConfigRef))"!],
[!ELSE!][!//
    /* CsmJobId (dummy Csm job ID */
    SECOC_DUMMY_CSM_JOB_ID,
[!ENDIF!][!//
[!IF "($NumberOfPduWithAuthDataFreshness > 0) and (node:value(as:modconf('SecOC')/SecOCGeneral/SecOCQueryFreshnessValue) = 'CFUNC')"!][!//
[!IF "node:value(./SecOCUseAuthDataFreshness) = 'true'"!][!//
    /* GetRxFreshnessAuthData */
    &[!"node:value(./SecOCFreshnessValueFuncName)"!],
[!ELSE!][!//
    /* GetRxFreshnessAuthData */
    NULL_PTR,
[!ENDIF!][!//
[!ENDIF!][!//
[!IF "($NumberOfPduWithFreshness > 0) and (node:value(as:modconf('SecOC')/SecOCGeneral/SecOCQueryFreshnessValue) = 'CFUNC')"!][!//
[!IF "node:value(./SecOCUseAuthDataFreshness) = 'false'"!][!//
    /* GetRxFreshness */
    &[!"node:value(./SecOCFreshnessValueFuncName)"!],
[!ELSE!][!//
    /* GetRxFreshness */
    NULL_PTR,
[!ENDIF!][!//
[!ENDIF!][!//
[!IF "SecOCRxSecuredPduLayer = 'SecOCRxSecuredPdu'"!][!//
[!IF "($NumberOfPduWithCollection > 0) and ($NumberOfPduWithMessLinker > 0)"!][!//
    /* RxMessageLinkLength */
    0U,
    /* RxMessageLinkPos */
    0U,
[!ENDIF!][!//
[!ELSE!][!//
[!IF "$NumberOfPduWithMessLinker > 0"!][!//
[!IF "node:exists(./SecOCRxSecuredPduLayer/SecOCUseMessageLink/SecOCMessageLinkLen) = 'true'"!][!//
    /* RxMessageLinkLength */
    [!"node:value(./SecOCRxSecuredPduLayer/SecOCUseMessageLink/SecOCMessageLinkLen)"!]U,
    /* RxMessageLinkPos */
    [!"node:value(./SecOCRxSecuredPduLayer/SecOCUseMessageLink/SecOCMessageLinkPos)"!]U,
[!ELSE!][!//
    /* RxMessageLinkLength */
    0U,
    /* RxMessageLinkPos */
    0U,
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!IF "SecOCRxSecuredPduLayer = 'SecOCRxSecuredPdu'"!][!//
    /* RxSecuredLayerPduIdAtSecOC */
    [!"node:value(./SecOCRxSecuredPduLayer/SecOCRxSecuredLayerPduId)"!]U,
[!ELSE!][!//
    /* RxSecuredLayerPduIdAtSecOC */
    0U,
[!ENDIF!][!//
[!IF "$NumberOfPduWithCollection > 0"!][!//
[!IF "SecOCRxSecuredPduLayer = 'SecOCRxSecuredPduCollection'"!][!//
    /* RxReceivedAuthLayerPduIdAtSecOC */
    [!"node:value(./SecOCRxSecuredPduLayer/SecOCRxAuthenticPdu/SecOCRxAuthenticPduId)"!]U,
    /* RxCryptoLayerPduIdAtSecOC */
    [!"node:value(./SecOCRxSecuredPduLayer/SecOCRxCryptographicPdu/SecOCRxCryptographicPduId)"!]U,
[!ELSE!][!//
    /* RxReceivedAuthLayerPduIdAtSecOC */
    0U,
    /* RxCryptoLayerPduIdAtSecOC */
    0U,
[!ENDIF!][!//
[!ENDIF!][!//
    /* FreshnessValueMaxSyncAttempts */
    [!"node:value(./SecOCAuthenticationVerifyAttempts)"!]U,
    /* AuthenticationBuildAttempts */
    [!"node:value(./SecOCAuthenticationBuildAttempts)"!]U,
[!IF "$NumberOfPduWithAuthDataFreshness > 0"!][!//
    /* AuthDataFreshnessValueLength */
    [!"num:i($AuthDataFreshnessLen)"!]U,
    /* AuthDataFreshnessValueStartPosition */
    [!"num:i($AuthDataFreshnessStartPosition)"!]U,
[!ENDIF!][!//
    /* ReceptionStrategy */
    [!IF "node:value(./SecOCReceptionOverflowStrategy) = 'REJECT'"!]SECOC_REJECT_INCOMING_PDU[!ELSE!]SECOC_REPLACE_CURRENT_PDU[!ENDIF!],
[!IF "node:value(as:modconf('SecOC')/SecOCGeneral/SecOCQueryFreshnessValue) != 'NONE'"!][!//
    /* GetRxFreshnessFuncType */
    [!IF "node:value(./SecOCUseAuthDataFreshness) = 'true'"!]SECOC_GET_RX_FRESHNESS_AUTHDATA_FUNC_TYPE[!ELSE!]SECOC_GET_RX_FRESHNESS_FUNC_TYPE[!ENDIF!],
[!ENDIF!][!//
[!IF "$NumberOfPduWithCollection > 0"!][!//
    /* UseSecPduCollection */
    [!IF "'SecOCRxSecuredPduCollection' = SecOCRxSecuredPduLayer"!]TRUE[!ELSE!]FALSE[!ENDIF!],
[!ENDIF!][!//
[!IF "node:exists(as:modconf('SecOC')/SecOCGeneral/SecOCRxShapeFuncName) = 'true'"!][!//
    /* RxShapeSecuredPdu*/
    [!IF "node:value(./SecOCRxUseShapeFunc) = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],
[!ENDIF!][!//
[!IF "$NumberOfPduWithCsmAsync > 0"!][!//
    /* UseCsmAsync */
    [!IF "node:value(./SecOcCsmMode) = 'ASYNCHRONOUS'"!]TRUE[!ELSE!]FALSE[!ENDIF!],
[!ENDIF!][!//
[!IF "$NumberOfPduWithSkipVerification > 0"!][!//
    /* SkipVerification */
    [!IF "node:value(./SecOCRxSecuredPduLayer/SecOCSecuredRxPduVerification) = 'false'"!]TRUE[!ELSE!]FALSE[!ENDIF!],
[!ENDIF!][!//
    /* UseTp */
    [!IF "'SECOC_TPPDU' = node:value(./SecOCRxAuthenticPduLayer/SecOCPduType)"!]TRUE[!ELSE!]FALSE[!ENDIF!],
[!IF "$NumberOfPduWithSyncProcessing > 0"!][!//
    /* UseRxSyncProcessing */
    [!IF "node:value(./SecOcCsmMode) = 'SYNCHRONOUS' and (node:exists(./SecOCRxSyncPduProcessing) = 'true') and (node:value(./SecOCRxSyncPduProcessing) = 'true')"!]TRUE[!ELSE!]FALSE[!ENDIF!],
[!ENDIF!][!//
  },
[!ENDLOOP!][!//
};
#endif

#if (SECOC_NUMBER_TX_PDUS > 0)
CONST(SecOC_TxConfigType, SECOC_CONST) SecOC_TxConfig[SECOC_NUMBER_TX_PDUS] =
{
[!VAR "NumberOfPduWithFreshnessTruncData" = "num:i(count(SecOCTxPduProcessing/*[SecOCProvideTxTruncatedFreshnessValue = 'true']))"!][!//
[!VAR "NumberOfPduWithFreshness" = "num:i(count(SecOCTxPduProcessing/*[SecOCProvideTxTruncatedFreshnessValue = 'false']))"!][!//
[!VAR "NumberOfPduWithStatusPropEn" = "num:i(count(SecOCTxPduProcessing/*[node:value(SecOCMacGenerateStatusPropagationMode) != 'NONE']))"!][!//
[!VAR "NumberOfPduWithConfirmationTimeout" = "num:i(count(SecOCTxPduProcessing/*[SecOCTxConfirmationTimeout > 0]))"!][!//
[!VAR "NumberOfPduWithCsmAsync" = "num:i(count(SecOCTxPduProcessing/*[SecOcCsmMode = 'ASYNCHRONOUS']))"!][!//
[!VAR "NumberOfPduWithCollection" = "num:i(count(SecOCTxPduProcessing/*[SecOCTxSecuredPduLayer = 'SecOCTxSecuredPduCollection']))"!][!//
[!VAR "NumberOfPduWithMessLinker" = "num:i(count(SecOCTxPduProcessing/*/SecOCTxSecuredPduLayer/SecOCUseMessageLink/SecOCMessageLinkLen))"!][!//
[!VAR "NumberOfPduWithSyncProcessing" = "num:i(count(SecOCTxPduProcessing/*[node:exists(SecOCTxSyncPduProcessing)][node:value(SecOCTxSyncPduProcessing) = 'true' and node:value(SecOcCsmMode) = 'SYNCHRONOUS']))"!][!//
[!//
[!LOOP "node:order(SecOCTxPduProcessing/*, 'SecOCTxAuthenticPduLayer/SecOCTxAuthenticLayerPduId')"!][!//
[!//
  /* [!"node:name(node:current())"!] */
  {
[!IF "$NumberOfPduWithConfirmationTimeout > 0"!][!//
    /* TxConfirmationTimeoutThreshold */
    [!"num:i(node:value(./SecOCTxConfirmationTimeout) div node:value(./../../SecOCGeneral/SecOCMainFunctionPeriodRx))"!]U,
[!ENDIF!][!//
    /* CsmJobId */
    CsmConf_CsmJob_[!"as:name(as:ref(./SecOCTxAuthServiceConfigRef))"!],
[!IF "($NumberOfPduWithFreshnessTruncData > 0) and (node:value(as:modconf('SecOC')/SecOCGeneral/SecOCQueryFreshnessValue) = 'CFUNC')"!][!//
[!IF "node:value(./SecOCProvideTxTruncatedFreshnessValue) = 'true'"!][!//
    /* GetTxFreshnessTruncData */
    &[!"node:value(./SecOCFreshnessValueFuncName)"!],
[!ELSE!][!//
    /* GetTxFreshnessTruncData */
    NULL_PTR,
[!ENDIF!][!//
[!ENDIF!][!//
[!IF "($NumberOfPduWithFreshness > 0) and (node:value(as:modconf('SecOC')/SecOCGeneral/SecOCQueryFreshnessValue) = 'CFUNC')"!][!//
[!IF "node:value(./SecOCProvideTxTruncatedFreshnessValue) = 'false'"!][!//
    /* GetTxFreshness */
    &[!"node:value(./SecOCFreshnessValueFuncName)"!],
[!ELSE!][!//
    /* GetTxFreshness */
    NULL_PTR,
[!ENDIF!][!//
[!ENDIF!][!//
[!IF "node:value(as:modconf('SecOC')/SecOCGeneral/SecOCQueryFreshnessValue) = 'CFUNC'"!][!//
    /* SPduTxConfirmation */
    &[!"node:value(./SecOCSecuredPDUTransmittedFuncName)"!],
[!ENDIF!][!//
[!IF "SecOCTxSecuredPduLayer = 'SecOCTxSecuredPdu'"!][!//
    /* TxSecuredLayerPduIdAtSecOC */
    [!"node:value(./SecOCTxSecuredPduLayer/SecOCTxSecuredLayerPduId)"!]U,
[!IF "$NumberOfPduWithCollection > 0"!][!//
    /* TxSentAuthLayerPduIdAtSecOC */
    0U,
    /* TxCryptoLayerPduIdAtSecOC */
    0U,
[!ENDIF!][!//
[!ELSE!][!//
    /* TxSecuredLayerPduIdAtSecOC */
    0U,
    /* TxSentAuthLayerPduIdAtSecOC */
    [!"node:value(./SecOCTxSecuredPduLayer/SecOCTxAuthenticPdu/SecOCTxAuthenticPduId)"!]U,
    /* TxCryptoLayerPduIdAtSecOC */
    [!"node:value(./SecOCTxSecuredPduLayer/SecOCTxCryptographicPdu/SecOCTxCryptographicPduId)"!]U,
[!ENDIF!][!//
[!IF "SecOCTxSecuredPduLayer = 'SecOCTxSecuredPdu'"!][!//
[!IF "($NumberOfPduWithCollection > 0) and ($NumberOfPduWithMessLinker > 0)"!][!//
    /* TxMessageLinkLength */
    0U,
    /* TxMessageLinkPos */
    0U,
[!ENDIF!][!//
[!ELSE!][!//
[!IF "$NumberOfPduWithMessLinker > 0"!][!//
[!IF "node:exists(./SecOCTxSecuredPduLayer/SecOCUseMessageLink/SecOCMessageLinkLen) = 'true'"!][!//
    /* TxMessageLinkLength */
    [!"node:value(./SecOCTxSecuredPduLayer/SecOCUseMessageLink/SecOCMessageLinkLen)"!]U,
    /* TxMessageLinkPos */
    [!"node:value(./SecOCTxSecuredPduLayer/SecOCUseMessageLink/SecOCMessageLinkPos)"!]U,
[!ELSE!][!//
    /* TxMessageLinkLength */
    0U,
    /* TxMessageLinkPos */
    0U,
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDIF!][!//
    /* MaxAuthAttempts */
    [!"node:value(./SecOCAuthenticationBuildAttempts)"!]U,
[!IF "$NumberOfPduWithStatusPropEn > 0"!][!//
    /* MacGenerateStatusPropagationMode */
    SECOC_STATUS_PROP_[!"node:value(./SecOCMacGenerateStatusPropagationMode)"!],
[!ENDIF!][!//
[!IF "node:value(as:modconf('SecOC')/SecOCGeneral/SecOCQueryFreshnessValue) != 'NONE'"!][!//
    /* GetTxFreshnessFuncType */
    [!IF "node:value(./SecOCProvideTxTruncatedFreshnessValue) = 'true'"!]SECOC_GET_TX_FRESHNESS_TRUNCDATA_FUNC_TYPE[!ELSE!]SECOC_GET_TX_FRESHNESS_FUNC_TYPE[!ENDIF!],
[!ENDIF!][!//
[!IF "$NumberOfPduWithCollection > 0"!][!//
    /* UseSecPduCollection */
    [!IF "'SecOCTxSecuredPduCollection' = SecOCTxSecuredPduLayer"!]TRUE[!ELSE!]FALSE[!ENDIF!],
[!ENDIF!][!//
[!IF "node:exists(as:modconf('SecOC')/SecOCGeneral/SecOCTxShapeFuncName) = 'true'"!][!//
    /* TxShapeSecuredPdu */
    [!IF "node:value(./SecOCTxUseShapeFunc) = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!],
[!ENDIF!][!//
[!IF "$NumberOfPduWithCsmAsync > 0"!][!//
    /* UseCsmAsync */
    [!IF "node:value(./SecOcCsmMode) = 'ASYNCHRONOUS'"!]TRUE[!ELSE!]FALSE[!ENDIF!],
[!ENDIF!][!//
    /* UseTp */
    [!IF "'SECOC_TPPDU' = node:value(./SecOCTxAuthenticPduLayer/SecOCPduType)"!]TRUE[!ELSE!]FALSE[!ENDIF!],
[!IF "$NumberOfPduWithSyncProcessing > 0"!][!//
    /* UseTxSyncProcessing */
    [!IF "node:value(./SecOcCsmMode) = 'SYNCHRONOUS' and (node:exists(./SecOCTxSyncPduProcessing) = 'true') and (node:value(./SecOCTxSyncPduProcessing) = 'true')"!]TRUE[!ELSE!]FALSE[!ENDIF!],
[!ENDIF!][!//
  },
[!ENDLOOP!][!//
};
#endif

#define SECOC_STOP_SEC_CONST_UNSPECIFIED
#include <SecOC_MemMap.h>

/*==================[external data]===============================================================*/

#define SECOC_START_SEC_VAR_CLEARED_8
#include <SecOC_MemMap.h>

[!IF "num:i(count(SecOCRxPduProcessing/*[node:exists(SecOCSameBufferPduRef) = true()])) > 0"!][!//
VAR(uint8, SECOC_VAR) SecOC_RxBufferUsedFlags[[!"num:i(count(node:refs(SecOCRxPduProcessing/*[node:exists(./SecOCSameBufferPduRef) = true()]/SecOCSameBufferPduRef)))"!]];
[!ENDIF!][!//

[!IF "num:i(count(SecOCTxPduProcessing/*[node:exists(SecOCSameBufferPduRef) = true()])) > 0"!][!//
VAR(uint8, SECOC_VAR) SecOC_TxBufferUsedFlags[[!"num:i(count(node:refs(SecOCTxPduProcessing/*[node:exists(./SecOCSameBufferPduRef) = true()]/SecOCSameBufferPduRef)))"!]];
[!ENDIF!][!//

[!VAR "NumberOfPduWithAuthDataFreshness" = "num:i(count(SecOCRxPduProcessing/*[SecOCUseAuthDataFreshness = 'true']))"!][!//
[!LOOP "node:order(SecOCRxPduProcessing/*)"!][!//
[!//
[!IF "$NumberOfPduWithAuthDataFreshness > 0"!][!//
[!IF "'true' = node:value(./SecOCUseAuthDataFreshness)"!][!//
[!VAR "AuthDataFreshnessBufferLen" = "ceiling((node:value(./SecOCAuthDataFreshnessLen) + (node:value(./SecOCAuthDataFreshnessStartPosition) mod 8)) div 8)"!][!//
[!ELSE!][!//
[!VAR "AuthDataFreshnessBufferLen" = "0"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!//
[!IF "$NumberOfPduWithAuthDataFreshness > 0 and $AuthDataFreshnessBufferLen > 0"!][!//
VAR(uint8, SECOC_VAR) SecOC_RxAuthDataFreshnessValue_[!"node:name(.)"!][[!"num:i($AuthDataFreshnessBufferLen)"!]U];

[!ENDIF!][!//
[!ENDLOOP!][!//

VAR(uint8, SECOC_VAR) SecOC_PB_PduBuffer[SECOC_PB_PDU_MEM_SIZE];

VAR(uint8, SECOC_VAR) SecOC_PB_IntBuffer[SECOC_PB_INT_MEM_SIZE];

#define SECOC_STOP_SEC_VAR_CLEARED_8
#include <SecOC_MemMap.h>

#define SECOC_START_SEC_VAR_INIT_UNSPECIFIED
#include <SecOC_MemMap.h>
[!IF "count(SecOCRxPduProcessing/*) > 0"!][!//

VAR(SecOC_RxDataType, SECOC_VAR) SecOC_RxData[SECOC_NUMBER_RX_PDUS] =
{
[!VAR "NumberOfPduWithTp" = "num:i(count(SecOCRxPduProcessing/*[./SecOCRxAuthenticPduLayer/SecOCPduType = 'SECOC_TPPDU']))"!][!//
[!VAR "NumberOfPduWithAuthDataFreshness" = "num:i(count(SecOCRxPduProcessing/*[SecOCUseAuthDataFreshness = 'true']))"!][!//
[!VAR "NumberOfPduWithCollection" = "num:i(count(SecOCRxPduProcessing/*[SecOCRxSecuredPduLayer = 'SecOCRxSecuredPduCollection']))"!][!//
[!VAR "NumberOfPduWithCsmAsync" = "num:i(count(SecOCRxPduProcessing/*[SecOcCsmMode = 'ASYNCHRONOUS']))"!][!//
[!VAR "NumberOfPduWithSameBuffer" = "num:i(count(SecOCRxPduProcessing/*[node:exists(./SecOCSameBufferPduRef) = true()]))"!][!//
[!VAR "UseCsmJobIdPostDefined" = "node:exists(SecOCGeneral/SecOCCsmJobRefCallout)"!][!//
[!LOOP "node:order(SecOCRxPduProcessing/*)"!][!//
  /* [!"node:name(node:current())"!] */
  {
[!IF "$UseCsmJobIdPostDefined = 'true'"!][!//
    /* CsmJobIdPostDefined */
    0U,
[!ENDIF!][!//
[!IF "$NumberOfPduWithSameBuffer > 0"!][!//
[!IF "node:exists(./SecOCSameBufferPduRef) = true()"!][!//
    /* RxBufferUsed */
    &SecOC_RxBufferUsedFlags[SECOC_RX_BUFFER_USED_FLAG_[!"text:toupper(node:name(as:ref(./SecOCSameBufferPduRef)))"!]],
[!ELSE!][!//
    /* RxBufferUsed */
    NULL_PTR,
[!ENDIF!][!//
[!ENDIF!][!//
    /* RxReceivedSecuredPduBuffer */
    NULL_PTR,
[!IF "$NumberOfPduWithCollection > 0"!][!//
    /* RxReceivedAuthPduBuffer */
    NULL_PTR,
    /* RxCryptoPduBuffer */
    NULL_PTR,
[!ENDIF!][!//
    /* RxSecuredPduBuffer */
    NULL_PTR,
    /* RxDataToAuthenticator */
    NULL_PTR,
    /* RxFreshnessVerifyValue */
    NULL_PTR,
    /* RxAuthenticator */
    NULL_PTR,
[!IF "$NumberOfPduWithAuthDataFreshness > 0"!][!//
[!IF "node:value(./SecOCUseAuthDataFreshness) = 'true' and node:value(./SecOCAuthDataFreshnessLen) > 0"!][!//
    /* AuthDataFreshnessValueBuffer */
    SecOC_RxAuthDataFreshnessValue_[!"node:name(.)"!],
[!ELSE!][!//
    /* AuthDataFreshnessValueBuffer */
    NULL_PTR,
[!ENDIF!][!//
[!ENDIF!][!//
    /* RxSecuredDataLength */
    0U,
    /* RxDataLength */
    0U,
    /* ReceivedTpSecuredDataLength */
    0U,
[!IF "$NumberOfPduWithCollection > 0"!][!//
    /* ReceivedTpAuthDataLength*/
    0U,
    /* ReceivedTpCryptoDataLength */
    0U,
[!ENDIF!][!//
    /* AuthAttempts */
    0U,
    /* FreshnessValueSyncAttempts */
    0U,
    /* VerificationResult */
    SECOC_VERIFICATIONFAILURE,
    /* VerifyStatusOverride */
    SECOC_OVERRIDESTATUS_CANCEL,
    /* NumMsgToOverride */
    0U,
    /* RxState */
    0U,
    /* RxPduREvent */
    0U,
[!IF "$NumberOfPduWithCollection > 0"!][!//
    /* RxPduRIndicationForAuth */
    0U,
    /* RxPduRIndicationForCrypto */
    0U,
[!ENDIF!][!//
[!IF "$NumberOfPduWithCsmAsync > 0"!][!//
    /* RxVerEvent */
    0U,
[!ENDIF!][!//
    /* CryptoVerificationResult */
    0U,
[!IF "node:value(as:modconf('SecOC')/SecOCGeneral/SecOCPropagateVerificationStatus) = 'EB_CUSTOM'"!][!//
    /* VerificationReturn */
    E_NOT_OK,
[!ENDIF!][!//
    /* RxPduInProcess */
    FALSE,
    /* ProcessingVerification */
    FALSE,
[!IF "node:value(as:modconf('SecOC')/SecOCGeneral/SecOCEnableForcedPassOverride) = 'true'"!][!//
    /* RenewedVerStatus */
    FALSE,
[!ENDIF!][!//
[!IF "$NumberOfPduWithTp > 0"!][!//
    /* UpperLayerCallFlag */
    FALSE
[!ENDIF!][!//
  },
[!ENDLOOP!][!//
};

[!ENDIF!][!//
[!IF "count(SecOCTxPduProcessing/*) > 0"!][!//

VAR(SecOC_TxDataType, SECOC_VAR) SecOC_TxData[SECOC_NUMBER_TX_PDUS] =
{
[!VAR "NumberOfPduWithCollection" = "num:i(count(SecOCTxPduProcessing/*[SecOCTxSecuredPduLayer = 'SecOCTxSecuredPduCollection']))"!][!//
[!VAR "NumberOfPduWithStatusPropEn" = "num:i(count(SecOCTxPduProcessing/*[node:value(SecOCMacGenerateStatusPropagationMode) != 'NONE']))"!][!//
[!VAR "NumberOfPduWithConfirmationTimeout" = "num:i(count(SecOCTxPduProcessing/*[SecOCTxConfirmationTimeout > 0]))"!][!//
[!VAR "NumberOfPduWithCsmAsync" = "num:i(count(SecOCTxPduProcessing/*[SecOcCsmMode = 'ASYNCHRONOUS']))"!][!//
[!VAR "UseCsmJobIdPostDefined" = "node:exists(SecOCGeneral/SecOCCsmJobRefCallout)"!][!//
[!VAR "NumberOfPduWithSameBuffer" = "num:i(count(SecOCTxPduProcessing/*[node:exists(./SecOCSameBufferPduRef) = true()]))"!][!//
[!LOOP "node:order(SecOCTxPduProcessing/*, 'SecOCTxAuthenticPduLayer/SecOCTxAuthenticLayerPduId')"!][!//
  /* [!"node:name(node:current())"!] */
  {
[!IF "$UseCsmJobIdPostDefined = 'true'"!][!//
    /* CsmJobIdPostDefined */
    0U,
[!ENDIF!][!//
[!IF "$NumberOfPduWithConfirmationTimeout > 0"!][!//
    /* TxConfirmationTimeoutCounter */
    0U,
[!ENDIF!][!//
    /* TxAuthenticatorLengthResult */
    0U,
[!IF "$NumberOfPduWithSameBuffer > 0"!][!//
[!IF "node:exists(./SecOCSameBufferPduRef) = true()"!][!//
    /* TxBufferUsed */
    &SecOC_TxBufferUsedFlags[SECOC_TX_BUFFER_USED_FLAG_[!"text:toupper(node:name(as:ref(./SecOCSameBufferPduRef)))"!]],
[!ELSE!][!//
    /* TxBufferUsed */
    NULL_PTR,
[!ENDIF!][!//
[!ENDIF!][!//
    /* TxReceivedAuthPduBuffer */
    NULL_PTR,
    /* TxSecuredPduBuffer */
    NULL_PTR,
    /* TxDataToAuthenticator */
    NULL_PTR,
    /* TxAuthenticator */
    NULL_PTR,
    /* TxDataLength */
    0U,
    /* TxReceivedDataLength */
    0U,
    /* CopiedDataFromSecLength */
    0U,
[!IF "$NumberOfPduWithCollection > 0"!][!//
    /* CopiedDataFromAuthLength */
    0U,
    /* CopiedDataFromCryptoLength */
    0U,
[!ENDIF!][!//
    /* TxSecuredPdu */
    { NULL_PTR, 0U },
[!IF "$NumberOfPduWithCollection > 0"!][!//
    /* TxSentAuthPdu */
    { NULL_PTR, 0U },
    /* TxCryptoPdu */
    { NULL_PTR, 0U },
[!ENDIF!][!//
    /* AuthAttempts */
    0U,
    /* TxState */
    0U,
    /* TxPduREvent */
    0U,
[!IF "$NumberOfPduWithCollection > 0"!][!//
    /* TxPduRConfirmationForAuth */
    0U,
    /* TxPduRConfirmationForCrypto */
    0U,
[!ENDIF!][!//
    /* TxTransmitEvent */
    0U,
[!IF "$NumberOfPduWithCsmAsync > 0"!][!//
    /* TxAuthEvent */
    0U,
[!ENDIF!][!//
[!IF "$NumberOfPduWithStatusPropEn > 0"!][!//
    /* MacGenerateResult */
    E_NOT_OK,
[!ENDIF!][!//
    /* ProcessingAuthentication */
    FALSE
  },
[!ENDLOOP!][!//
};
[!ENDIF!][!//

#define SECOC_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <SecOC_MemMap.h>
[!ENDSELECT!][!//

/*==================[external function definitions]===============================================*/

/*==================[internal function definitions]===============================================*/

/*==================[end of file]=================================================================*/
