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

#ifndef SECOC_INT_H
#define SECOC_INT_H

/*==================[deviations]============================================================*/

/*==================[includes]====================================================================*/

/* !LINKSTO SecOC.Dsn.BasicSWStructure.RxTx.SecOCInth,4 */
#include <SecOC.h>

#if (SECOC_NUMBER_RX_PDUS > 0)
#include <SecOC_Rx.h>
#endif
#if (SECOC_NUMBER_TX_PDUS > 0)
#include <SecOC_Tx.h>
#endif

#include <TSMem.h>
#include <SchM_SecOC.h>
#include <PduR_SecOC.h>

#if (defined TS_RELOCATABLE_CFG_ENABLE)
#error TS_RELOCATABLE_CFG_ENABLE already defined!
#endif
#define TS_RELOCATABLE_CFG_ENABLE SECOC_RELOCATABLE_CFG_ENABLE

#include <TSPBConfig_Access.h>
#include <TSPBConfig_Signature.h>

/*==================[macros]======================================================================*/

/* Relocatable post build elements access macros. */

#if (defined SECOC_PBCFG_ACCESS)
#error SECOC_PBCFG_ACCESS already defined
#endif
#define SECOC_PBCFG_ACCESS(type, element)   \
    (TS_UNCHECKEDGETCFG(SecOC_ConfigPtr, type, SECOC, SecOC_ConfigPtr->element))

#if (SECOC_NUMBER_RX_PDUS > 0)

#if (defined SECOC_GET_RXPDU_CFG)
#error SECOC_GET_RXPDU_CFG already defined
#endif
#define SECOC_GET_RXPDU_CFG(Index)   \
    (SECOC_PBCFG_ACCESS(SecOC_RxPduConfigType, RxPduConfigPtr)[(Index)])

#if (defined SECOC_GET_RX_DATA_ID)
#error SECOC_GET_RX_DATA_ID already defined
#endif
#define SECOC_GET_RX_DATA_ID(Index) (SECOC_GET_RXPDU_CFG(Index).DataId)

#if (defined SECOC_GET_RX_AUTH_PDU_ID_AT_PDUR)
#error SECOC_GET_RX_AUTH_PDU_ID_AT_PDUR already defined
#endif
#define SECOC_GET_RX_AUTH_PDU_ID_AT_PDUR(Index)   \
    (SECOC_GET_RXPDU_CFG(Index).AuthenticLayerPduIdAtPduR)

#if (STD_ON == SECOC_PDU_SECURED_AREA)
#if (defined SECOC_GET_RX_SECURED_AREA_LEN)
#error SECOC_GET_RX_SECURED_AREA_LEN already defined
#endif
#define SECOC_GET_RX_SECURED_AREA_LEN(Index) (SECOC_GET_RXPDU_CFG(Index).SecuredRxPduLength)

#if (defined SECOC_GET_RX_SECURED_AREA_OFFSET)
#error SECOC_GET_RX_SECURED_AREA_OFFSET already defined
#endif
#define SECOC_GET_RX_SECURED_AREA_OFFSET(Index) (SECOC_GET_RXPDU_CFG(Index).SecuredRxPduOffset)

#if (defined SECOC_GET_RX_USE_SECURED_AREA)
#error SECOC_GET_RX_USE_SECURED_AREA already defined
#endif
#define SECOC_GET_RX_USE_SECURED_AREA(Index) (SECOC_GET_RXPDU_CFG(Index).UseRxSecuredArea)
#endif /* #if (STD_ON == SECOC_PDU_SECURED_AREA) */

#if (defined SECOC_GET_RX_FRESHNESS_ID)
#error SECOC_GET_RX_FRESHNESS_ID already defined
#endif
#define SECOC_GET_RX_FRESHNESS_ID(Index) (SECOC_GET_RXPDU_CFG(Index).FreshnessValueId)

#if (defined SECOC_GET_RX_FRESHNESS_LEN)
#error SECOC_GET_RX_FRESHNESS_LEN already defined
#endif
#define SECOC_GET_RX_FRESHNESS_LEN(Index) (SECOC_GET_RXPDU_CFG(Index).FreshnessValueLength)

#if (defined SECOC_GET_RX_FRESHNESSTX_LEN)
#error SECOC_GET_RX_FRESHNESSTX_LEN already defined
#endif
#define SECOC_GET_RX_FRESHNESSTX_LEN(Index) (SECOC_GET_RXPDU_CFG(Index).FreshnessValueTxLength)

#if (defined SECOC_GET_RX_AUTHINFOTX_LEN)
#error SECOC_GET_RX_AUTHINFOTX_LEN already defined
#endif
#define SECOC_GET_RX_AUTHINFOTX_LEN(Index) (SECOC_GET_RXPDU_CFG(Index).AuthInfoTxLength)

#if (SECOC_VERIFICATION_STATUS_PROP_NONE != SECOC_VERIFICATION_STATUS_PROP_ENABLED)
#if (defined SECOC_GET_RX_VERSTATUS_PROP_MODE)
#error SECOC_GET_RX_VERSTATUS_PROP_MODE already defined
#endif
#define SECOC_GET_RX_VERSTATUS_PROP_MODE(Index)   \
    (SECOC_GET_RXPDU_CFG(Index).VerificationStatusPropagationMode)
#endif /* #if (STD_ON == SECOC_VERIFICATION_STATUS_PROP_ENABLED) */

#if (defined SECOC_GET_RX_RECVSECURED_PDU_ADR)
#error SECOC_GET_RX_RECVSECURED_PDU_ADR already defined
#endif
#define SECOC_GET_RX_RECVSECURED_PDU_ADR(Index)    \
    (&SecOC_PB_PduBuffer[SECOC_GET_RXPDU_CFG(Index).BuffersConfig.ReceivedSecuredPduBufferIndex])

#if (STD_ON == SECOC_RX_USE_SECURED_PDU_COL)
#if (defined SECOC_GET_RX_RECVAUTH_PDU_ADR)
#error SECOC_GET_RX_RECVAUTH_PDU_ADR already defined
#endif
#define SECOC_GET_RX_RECVAUTH_PDU_ADR(Index)   \
    (&SecOC_PB_PduBuffer[SECOC_GET_RXPDU_CFG(Index).BuffersConfig.ReceivedAuthPduBufferIndex])

#if (defined SECOC_GET_RX_CRYPTO_PDU_ADR)
#error SECOC_GET_RX_CRYPTO_PDU_ADR already defined
#endif
#define SECOC_GET_RX_CRYPTO_PDU_ADR(Index)     \
    (&SecOC_PB_PduBuffer[SECOC_GET_RXPDU_CFG(Index).BuffersConfig.CryptoPduBufferIndex])
#endif /* #if (STD_ON == SECOC_RX_USE_SECURED_PDU_COL) */

#if (defined SECOC_GET_RX_SECURED_PDU_ADR)
#error SECOC_GET_RX_SECURED_PDU_ADR already defined
#endif
#define SECOC_GET_RX_SECURED_PDU_ADR(Index)    \
    (&SecOC_PB_PduBuffer[SECOC_GET_RXPDU_CFG(Index).BuffersConfig.SecuredPduBufferIndex])

#if (defined SECOC_GET_RX_SECURED_PDU_LEN)
#error SECOC_GET_RX_SECURED_PDU_LEN already defined
#endif
#define SECOC_GET_RX_SECURED_PDU_LEN(Index)    \
    (SECOC_GET_RXPDU_CFG(Index).BuffersConfig.SecuredPduBufferLength)

#if (defined SECOC_GET_RX_AUTH_PDU_LEN)
#error SECOC_GET_RX_AUTH_PDU_LEN already defined
#endif
#define SECOC_GET_RX_AUTH_PDU_LEN(Index)       \
    (SECOC_GET_RXPDU_CFG(Index).BuffersConfig.AuthPduBufferLength)

#if (STD_ON == SECOC_RX_USE_SECURED_PDU_COL)
#if (defined SECOC_GET_RX_CRYPTO_PDU_LEN)
#error SECOC_GET_RX_CRYPTO_PDU_LEN already defined
#endif
#define SECOC_GET_RX_CRYPTO_PDU_LEN(Index)     \
    (SECOC_GET_RXPDU_CFG(Index).BuffersConfig.CryptoPduBufferLength)
#endif /* #if (STD_ON == SECOC_RX_USE_SECURED_PDU_COL) */

#if (defined SECOC_GET_RX_AUTHENTICATOR_ADR)
#error SECOC_GET_RX_AUTHENTICATOR_ADR already defined
#endif
#define SECOC_GET_RX_AUTHENTICATOR_ADR(Index)  \
    (&SecOC_PB_IntBuffer[SECOC_GET_RXPDU_CFG(Index).BuffersConfig.AuthenticatorIndex])

#if (defined SECOC_GET_RX_DATATOAUTH_ADR)
#error SECOC_GET_RX_DATATOAUTH_ADR already defined
#endif
#define SECOC_GET_RX_DATATOAUTH_ADR(Index)     \
    (&SecOC_PB_IntBuffer[SECOC_GET_RXPDU_CFG(Index).BuffersConfig.DataToAuthenticatorIndex])


/** \brief Macro to calculate the sum of the Rx authentication information length that is comprised
 *         of the truncated authenticator value and the truncated freshness value
 */
#if (defined SECOC_RX_AUTHENTICATION_INFO_LEN)
#error SECOC_RX_AUTHENTICATION_INFO_LEN already defined
#endif
#define SECOC_RX_AUTHENTICATION_INFO_LEN(Id) ((uint32)(SECOC_GET_RX_AUTHINFOTX_LEN((Id))) + \
                                              (uint32)(SECOC_GET_RX_FRESHNESSTX_LEN((Id))))

#endif /* #if (SECOC_NUMBER_RX_PDUS > 0) */



#if (SECOC_NUMBER_TX_PDUS > 0)

#if (defined SECOC_GET_TXPDU_CFG)
#error SECOC_GET_TXPDU_CFG already defined
#endif
#define SECOC_GET_TXPDU_CFG(Index)   \
    (SECOC_PBCFG_ACCESS(SecOC_TxPduConfigType, TxPduConfigPtr)[(Index)])

#if (defined SECOC_GET_TX_DATA_ID)
#error SECOC_GET_TX_DATA_ID already defined
#endif
#define SECOC_GET_TX_DATA_ID(Index) (SECOC_GET_TXPDU_CFG(Index).DataId)

#if (defined SECOC_GET_TX_SEC_PDU_ID_AT_PDUR)
#error SECOC_GET_TX_SEC_PDU_ID_AT_PDUR already defined
#endif
#define SECOC_GET_TX_SEC_PDU_ID_AT_PDUR(Index)  \
    (SECOC_GET_TXPDU_CFG(Index).SecuredLayerPduIdAtPduR)

#if (STD_ON == SECOC_TX_USE_SECURED_PDU_COL)
#if (defined SECOC_GET_TX_SENT_AUTH_PDU_ID_AT_PDUR)
#error SECOC_GET_TX_SENT_AUTH_PDU_ID_AT_PDUR already defined
#endif
#define SECOC_GET_TX_SENT_AUTH_PDU_ID_AT_PDUR(Index)   \
    (SECOC_GET_TXPDU_CFG(Index).SentAuthLayerPduIdAtPduR)

#if (defined SECOC_GET_TX_CRYPTO_PDU_ID_AT_PDUR)
#error SECOC_GET_TX_CRYPTO_PDU_ID_AT_PDUR already defined
#endif
#define SECOC_GET_TX_CRYPTO_PDU_ID_AT_PDUR(Index)   \
    (SECOC_GET_TXPDU_CFG(Index).CryptoLayerPduIdAtPduR)
#endif /* #if (STD_ON == SECOC_TX_USE_SECURED_PDU_COL) */

#if (defined SECOC_GET_TX_ULAUTH_PDU_ID_AT_PDUR)
#error SECOC_GET_TX_ULAUTH_PDU_ID_AT_PDUR already defined
#endif
#define SECOC_GET_TX_ULAUTH_PDU_ID_AT_PDUR(Index)  \
    (SECOC_GET_TXPDU_CFG(Index).ULAuthLayerPduIdAtPduR)

#if (STD_ON == SECOC_PDU_SECURED_AREA)
#if (defined SECOC_GET_TX_SECURED_AREA_LEN)
#error SECOC_GET_TX_SECURED_AREA_LEN already defined
#endif
#define SECOC_GET_TX_SECURED_AREA_LEN(Index) (SECOC_GET_TXPDU_CFG(Index).SecuredTxPduLength)

#if (defined SECOC_GET_TX_SECURED_AREA_OFFSET)
#error SECOC_GET_TX_SECURED_AREA_OFFSET already defined
#endif
#define SECOC_GET_TX_SECURED_AREA_OFFSET(Index) (SECOC_GET_TXPDU_CFG(Index).SecuredTxPduOffset)

#if (defined SECOC_GET_TX_USE_SECURED_AREA)
#error SECOC_GET_TX_USE_SECURED_AREA already defined
#endif
#define SECOC_GET_TX_USE_SECURED_AREA(Index) (SECOC_GET_TXPDU_CFG(Index).UseTxSecuredArea)
#endif /* #if (STD_ON == SECOC_PDU_SECURED_AREA) */

#if (defined SECOC_GET_TX_FRESHNESS_ID)
#error SECOC_GET_TX_FRESHNESS_ID already defined
#endif
#define SECOC_GET_TX_FRESHNESS_ID(Index) (SECOC_GET_TXPDU_CFG(Index).FreshnessValueId)

#if (defined SECOC_GET_TX_FRESHNESS_LEN)
#error SECOC_GET_TX_FRESHNESS_LEN already defined
#endif
#define SECOC_GET_TX_FRESHNESS_LEN(Index) (SECOC_GET_TXPDU_CFG(Index).FreshnessValueLength)

#if (defined SECOC_GET_TX_FRESHNESSTX_LEN)
#error SECOC_GET_TX_FRESHNESSTX_LEN already defined
#endif
#define SECOC_GET_TX_FRESHNESSTX_LEN(Index) (SECOC_GET_TXPDU_CFG(Index).FreshnessValueTxLength)

#if (defined SECOC_GET_TX_AUTHINFOTX_LEN)
#error SECOC_GET_TX_AUTHINFOTX_LEN already defined
#endif
#define SECOC_GET_TX_AUTHINFOTX_LEN(Index) (SECOC_GET_TXPDU_CFG(Index).AuthInfoTxLength)

#if (defined SECOC_GET_TX_SECURED_PDU_ADR)
#error SECOC_GET_TX_SECURED_PDU_ADR already defined
#endif
#define SECOC_GET_TX_SECURED_PDU_ADR(Index)    \
    (&SecOC_PB_PduBuffer[SECOC_GET_TXPDU_CFG(Index).BuffersConfig.SecuredPduBufferIndex])

#if (defined SECOC_GET_TX_ULAUTH_PDU_ADR)
#error SECOC_GET_TX_ULAUTH_PDU_ADR already defined
#endif
#define SECOC_GET_TX_ULAUTH_PDU_ADR(Index)   \
    (&SecOC_PB_PduBuffer[SECOC_GET_TXPDU_CFG(Index).BuffersConfig.ULAuthPduBufferIndex])

#if (defined SECOC_GET_TX_SECURED_PDU_LEN)
#error SECOC_GET_TX_SECURED_PDU_LEN already defined
#endif
#define SECOC_GET_TX_SECURED_PDU_LEN(Index)    \
    (SECOC_GET_TXPDU_CFG(Index).BuffersConfig.SecuredPduBufferLength)

#if (defined SECOC_GET_TX_AUTH_PDU_LEN)
#error SECOC_GET_TX_AUTH_PDU_LEN already defined
#endif
#define SECOC_GET_TX_AUTH_PDU_LEN(Index)       \
    (SECOC_GET_TXPDU_CFG(Index).BuffersConfig.AuthPduBufferLength)

#if (STD_ON == SECOC_TX_USE_SECURED_PDU_COL)
#if (defined SECOC_GET_TX_CRYPTO_PDU_LEN)
#error SECOC_GET_TX_CRYPTO_PDU_LEN already defined
#endif
#define SECOC_GET_TX_CRYPTO_PDU_LEN(Index)     \
    (SECOC_GET_TXPDU_CFG(Index).BuffersConfig.CryptoPduBufferLength)
#endif /* #if (STD_ON == SECOC_TX_USE_SECURED_PDU_COL) */

#if (defined SECOC_GET_TX_AUTHENTICATOR_ADR)
#error SECOC_GET_TX_AUTHENTICATOR_ADR already defined
#endif
#define SECOC_GET_TX_AUTHENTICATOR_ADR(Index)  \
    (&SecOC_PB_IntBuffer[SECOC_GET_TXPDU_CFG(Index).BuffersConfig.AuthenticatorIndex])

#if (defined SECOC_GET_TX_DATATOAUTH_ADR)
#error SECOC_GET_TX_DATATOAUTH_ADR already defined
#endif
#define SECOC_GET_TX_DATATOAUTH_ADR(Index)    \
    (&SecOC_PB_IntBuffer[SECOC_GET_TXPDU_CFG(Index).BuffersConfig.DataToAuthenticatorIndex])

#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */


/** \brief This macro is used to avoid compiler warnings for unused parameters.
 *
 * In some cases, parameters are specified by AUTOSAR for an interface but are not used.
 */
#if (defined SECOC_PARAM_UNUSED)
#error SECOC_PARAM_UNUSED already defined
#endif
#define SECOC_PARAM_UNUSED(x) ((void)(x))


#if (SECOC_NUMBER_RX_PDUS > 0)

/** \brief indicates that a VerifyStatus shall be overwritten with fail until further notice
 */
#if (defined SECOC_OVERRIDESTATUS_FAIL)
#error SECOC_OVERRIDESTATUS_FAIL already defined
#endif
#define SECOC_OVERRIDESTATUS_FAIL                0U

/** \brief indicates that a VerifyStatus shall be overwritten with fail until a number of messages
 *         is reached
 */
#if (defined SECOC_OVERRIDESTATUS_FAIL_MSG_NUMBER)
#error SECOC_OVERRIDESTATUS_FAIL_MSG_NUMBER already defined
#endif
#define SECOC_OVERRIDESTATUS_FAIL_MSG_NUMBER     1U

/** \brief indicates that a VerifyStatus shall not be overwritten any more
 */
#if (defined SECOC_OVERRIDESTATUS_CANCEL)
#error SECOC_OVERRIDESTATUS_CANCEL already defined
#endif
#define SECOC_OVERRIDESTATUS_CANCEL              2U

#if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_ON)
/** \brief indicates that a VerifyStatus shall be overwritten with pass until a number of messages
 *         is reached
 */
#if (defined SECOC_OVERRIDESTATUS_PASS_MSG_NUMBER)
#error SECOC_OVERRIDESTATUS_PASS_MSG_NUMBER already defined
#endif
#define SECOC_OVERRIDESTATUS_PASS_MSG_NUMBER     41U
#endif /* #if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_ON) */

/** \brief indicates that the verification will not be performed
 */
#if (defined SECOC_OVERRIDESTATUS_SKIP_UNTIL_NOTICE)
#error SECOC_OVERRIDESTATUS_SKIP_UNTIL_NOTICE already defined
#endif
#define SECOC_OVERRIDESTATUS_SKIP_UNTIL_NOTICE   43U

#endif /* #if (SECOC_NUMBER_RX_PDUS > 0) */

/** \brief indicates that the reception overflow strategy is to reject the incoming PDU
 *         because SecOC is busy processing another PDU with the same PDU ID
 */
#if (defined SECOC_REJECT_INCOMING_PDU)
#error SECOC_REJECT_INCOMING_PDU already defined
#endif
#define SECOC_REJECT_INCOMING_PDU                0U

/** \brief indicates that the reception overflow strategy is to replace the current PDU which is
 *         under verification (if there is any)
 */
#if (defined SECOC_REPLACE_CURRENT_PDU)
#error SECOC_REPLACE_CURRENT_PDU already defined
#endif
#define SECOC_REPLACE_CURRENT_PDU                1U

/** \brief Macro to calculate the number of bytes required for a number of bits
 */
#if (defined SECOC_REQUIREDBYTES)
#error SECOC_REQUIREDBYTES already defined
#endif
#define SECOC_REQUIREDBYTES(Bits)  (uint32) (((uint32)(Bits) / 8U) +        \
                                            ((((uint32)(Bits) % 8U) > 0U)? 1U : 0U))

/** \brief Macro, which defines what Rx internal data shall be used when calling the Csm service
 */
#if (defined SECOC_RX_CSM_JOB_ID)
#error SECOC_RX_CSM_JOB_ID already defined
#endif
#if (SECOC_USE_CSMJOBREF_CALLOUT == STD_ON)
#define SECOC_RX_CSM_JOB_ID(Id)                  SecOC_RxData[(Id)].CsmJobIdPostDefined
#else
#define SECOC_RX_CSM_JOB_ID(Id)                  SecOC_RxConfig[(Id)].CsmJobId
#endif

/** \brief Macro, which defines what Tx internal data shall be used when calling the Csm service
 */
#if (defined SECOC_TX_CSM_JOB_ID)
#error SECOC_TX_CSM_JOB_ID already defined
#endif
#if (SECOC_USE_CSMJOBREF_CALLOUT == STD_ON)
#define SECOC_TX_CSM_JOB_ID(Id)                  SecOC_TxData[(Id)].CsmJobIdPostDefined
#else
#define SECOC_TX_CSM_JOB_ID(Id)                  SecOC_TxConfig[(Id)].CsmJobId
#endif

#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)

/** \brief Macro used when some buffer is not used
 */
#if (defined SECOC_BUFFER_FREE)
#error SECOC_BUFFER_FREE already defined
#endif
#define SECOC_BUFFER_FREE           0x00U

/** \brief Macro used when some buffer is currently being used
 */
#if (defined SECOC_BUFFER_USED)
#error SECOC_BUFFER_USED already defined
#endif
#define SECOC_BUFFER_USED           0x01U

/** \brief Macro used to set the status of the Tx buffer used to store the upper layer authentic PDU
 */
#if (defined SECOC_TX_SET_BUFFER_LVL1_STATUS)
#error SECOC_TX_SET_BUFFER_LVL1_STATUS already defined
#endif
#define SECOC_TX_SET_BUFFER_LVL1_STATUS(Id, Value)  \
  do\
  { \
    if (SecOC_TxData[Id].TxBufferUsed != NULL_PTR) \
    { \
      if (Value == SECOC_BUFFER_FREE) \
      { \
        *SecOC_TxData[Id].TxBufferUsed &= ~(1U);\
      }\
      else\
      {\
        *SecOC_TxData[Id].TxBufferUsed |= 1U;\
      }\
    } \
  } while (0U)

/** \brief Macro used to set the status of the Tx buffer used to store the secured PDU
 */
#if (defined SECOC_TX_SET_BUFFER_LVL2_STATUS)
#error SECOC_TX_SET_BUFFER_LVL2_STATUS already defined
#endif
#define SECOC_TX_SET_BUFFER_LVL2_STATUS(Id, Value)  \
  do\
  { \
    if (SecOC_TxData[Id].TxBufferUsed != NULL_PTR) \
    { \
      if (Value == SECOC_BUFFER_FREE) \
      { \
        *SecOC_TxData[Id].TxBufferUsed &= ~(1U << 1U);\
      }\
      else\
      {\
        *SecOC_TxData[Id].TxBufferUsed |= (1U << 1U);\
      }\
    } \
  } while (0U)

/** \brief Macro used to get the status of the Tx buffer used to store the upper layer authentic PDU
 */
#if (defined SECOC_TX_GET_BUFFER_LVL1_STATUS)
#error SECOC_TX_GET_BUFFER_LVL1_STATUS already defined
#endif
#define SECOC_TX_GET_BUFFER_LVL1_STATUS(Id, Value)  \
  do\
  { \
    if (SecOC_TxData[Id].TxBufferUsed != NULL_PTR) \
    { \
      Value = *SecOC_TxData[Id].TxBufferUsed & 1U; \
    } \
    else \
    { \
      Value = SECOC_BUFFER_FREE; \
    } \
  } while (0U)

/** \brief Macro used to get the status of the Tx buffer used to store the secured PDU
 */
#if (defined SECOC_TX_GET_BUFFER_LVL2_STATUS)
#error SECOC_TX_GET_BUFFER_LVL2_STATUS already defined
#endif
#define SECOC_TX_GET_BUFFER_LVL2_STATUS(Id, Value)  \
  do\
  { \
    if (SecOC_TxData[Id].TxBufferUsed != NULL_PTR) \
    { \
      Value = (*SecOC_TxData[Id].TxBufferUsed >> 1U) & 1U; \
    } \
    else \
    { \
      Value = SECOC_BUFFER_FREE; \
    } \
  } while (0U)

#endif /* #if (STD_ON == SECOC_TX_USE_SAME_BUFFER) */

/*==================[type definitions]============================================================*/

/**
 * \brief state machine state type
 */
typedef P2FUNC(void, TYPEDEF, SecOC_SmStateType) (uint16 instId);

#if (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_CFUNC)

#if (SECOC_TX_FRESHNESS_TRUNC_DATA == STD_ON)
/**
 * \brief Pointer to the external GetTxFreshnessTruncData function.
 */
typedef P2FUNC(Std_ReturnType, SECOC_CODE, GetTxFreshnessTruncDataType)
(
        uint16                              SecOCFreshnessValueID,
  P2VAR(uint8,  AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValue,
  P2VAR(uint32, AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValueLength,
  P2VAR(uint8,  AUTOMATIC, SECOC_APPL_DATA) SecOCTruncatedFreshnessValue,
  P2VAR(uint32, AUTOMATIC, SECOC_APPL_DATA) SecOCTruncatedFreshnessValueLength
);
#endif

#if (SECOC_TX_FRESHNESS == STD_ON)
/**
 * \brief Pointer to the external GetTxFreshness function.
 */
typedef P2FUNC(Std_ReturnType, SECOC_CODE, GetTxFreshnessType)
(
        uint16                              SecOCFreshnessValueID,
  P2VAR(uint8,  AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValue,
  P2VAR(uint32, AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValueLength
);
#endif

/**
 * \brief Pointer to the external TxPduConfirmation function.
 */
typedef P2FUNC(void, SECOC_CODE, SPduTxConfirmationType)
(
  uint16 SecOCFreshnessValueID
);

#if (SECOC_RX_AUTH_DATA_FRESHNESS == STD_ON)
/**
 * \brief Pointer to the external GetRxFreshnessAuthData function.
 */
typedef P2FUNC(Std_ReturnType, SECOC_CODE, GetRxFreshnessAuthDataType)
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
#endif

#if (SECOC_RX_FRESHNESS == STD_ON)
/**
 * \brief Pointer to the external GetRxFreshness function.
 */
typedef P2FUNC(Std_ReturnType, SECOC_CODE, GetRxFreshnessType)
(
          uint16                              SecOCFreshnessValueID,
  P2CONST(uint8,  AUTOMATIC, SECOC_APPL_DATA) SecOCTruncatedFreshnessValue,
          uint32                              SecOCTruncatedFreshnessValueLength,
          uint16                              SecOCCounterSyncAttempts,
    P2VAR(uint8,  AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValue,
    P2VAR(uint32, AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValueLength
);
#endif

#endif /* #if (SECOC_QUERY_FRESHNESS_VALUE != SECOC_FRESHNESS_RTE) */

/* !LINKSTO SecOC.Dsn.interface.Rx.SecOC_RxConfigType,15 */
/**
 * \struct SecOC_RxConfigType
 * \brief SecOC Configuration type for Rx Pdus, which shall be verified by the SecOC module.
 */
typedef struct
{
  /* Csm Job ID */
  uint32                             CsmJobId;
#if (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_CFUNC)
#if (SECOC_RX_AUTH_DATA_FRESHNESS == STD_ON)
  /* Pointer to the GetRxFreshnessAuthData function */
  GetRxFreshnessAuthDataType         GetRxFreshnessAuthData;
#endif
#if (SECOC_RX_FRESHNESS == STD_ON)
  /* Pointer to the GetRxFreshness function */
  GetRxFreshnessType                 GetRxFreshness;
#endif
#endif
#if (SECOC_RX_USE_MESS_LINKER == STD_ON)
  /* Length of message link in the Pdu */
  uint16                             RxMessageLinkLength;
  /* Position of message linker inside the authentic I-PDU */
  uint16                             RxMessageLinkPos;
#endif
  /* ID of the secured Pdu at SecOC (used for e.g. SecOC_StartOfReception) */
  uint16                             RxSecuredLayerPduIdAtSecOC;
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
  /* ID of the received authentic Pdu at SecOC (used for e.g. SecOC_StartOfReception) */
  uint16                             RxReceivedAuthLayerPduIdAtSecOC;
  /* ID of the cryptographic Pdu at SecOC (used for e.g. SecOC_StartOfReception) */
  uint16                             RxCryptoLayerPduIdAtSecOC;
#endif
  /* Max allowed Freshness Value sync Attempts */
  uint16                             FreshnessValueMaxSyncAttempts;
  /* Max allowed of authentication build attempts that are to be carried out  */
  uint16                             AuthenticationBuildAttempts;
#if (SECOC_RX_AUTH_DATA_FRESHNESS == STD_ON)
  /* The length (in bits) of the area within the Pdu which will be provided to the freshness value manager */
  uint16                             AuthDataFreshnessValueLength;
  /* The start position (offset in bits) of the area within the Pdu which will be provided to the freshness value manager */
  uint16                             AuthDataFreshnessValueStartPosition;
#endif /* SECOC_RX_AUTH_DATA_FRESHNESS == STD_ON */
  /* Flag which indicates the reception overflow strategy */
  uint8                              ReceptionStrategy;
#if (SECOC_QUERY_FRESHNESS_VALUE != SECOC_FRESHNESS_NONE)
  /* Defines which freshness function should be called */
  uint8                              GetRxFreshnessFuncType;
#endif
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
  /* Flag which indicates whether the secured PDU collection is used or not */
  boolean                            UseSecPduCollection;
#endif
#if (SECOC_RX_SHAPE_SEC_PDU == STD_ON)
  /* Flag which indicates whether the functionality to shape the secured PDU is used or not */
  boolean                            RxShapeSecuredPdu;
#endif /* #if (SECOC_RX_SHAPE_SEC_PDU == STD_ON) */
#if (SECOC_RX_CSM_ASYNC == STD_ON)
  /* Flag which indicates whether the Csm service is used in asynchronous mode or not */
  boolean                            UseCsmAsync;
#endif /* #if (SECOC_RX_CSM_ASYNC == STD_ON) */
#if (SECOC_SKIP_RX_PDU_VERIFICATION == STD_ON)
  /* Flag which indicates whether the MAC verification will be skipped or performed */
  boolean                            SkipVerification;
#endif /* #if (SECOC_SKIP_RX_PDU_VERIFICATION == STD_ON) */
  /* Flag which indicates whether the TP interfaces (in the communication with PduR) will be used or not */
  boolean                            UseTp;
#if (SECOC_RX_SYNC_PDU_PROCESSING == STD_ON)
  /* Flag indicating whether synchronous processing will be used or not */
  boolean                            UseRxSyncProcessing;
#endif
}
SecOC_RxConfigType;

/* !LINKSTO SecOC.Dsn.interface.Tx.SecOC_TxConfigType,15 */
/**
 * \struct SecOC_TxConfigType
 * \brief SecOC Configuration type for Tx Pdus, which shall be authenticated by the SecOC module.
 */
typedef struct
{
#if (SECOC_USE_TX_CONFIRMATION_TIMEOUT == STD_ON)
  /* Timeout for TxConfirmation threshold */
  uint32                            TxConfirmationTimeoutThreshold;
#endif /* #if (SECOC_USE_TX_CONFIRMATION_TIMEOUT == STD_ON) */
  /* Csm Job ID */
  uint32                            CsmJobId;
#if (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_CFUNC)
#if (SECOC_TX_FRESHNESS_TRUNC_DATA == STD_ON)
  /* Pointer to the GetTxFreshnessTruncData function */
  GetTxFreshnessTruncDataType       GetTxFreshnessTruncData;
#endif
#if (SECOC_TX_FRESHNESS == STD_ON)
  /* Pointer to the GetTxFreshness function */
  GetTxFreshnessType                GetTxFreshness;
#endif
  /* Pointer to the SPduTxConfirmation function */
  SPduTxConfirmationType            SPduTxConfirmation;
#endif
  /* ID of the secured Pdu at SecOC (used for e.g. SecOC_TxConfirmation) */
  uint16                            TxSecuredLayerPduIdAtSecOC;
#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
  /* ID of the authentic Pdu at SecOC (used for e.g. SecOC_TxConfirmation) */
  uint16                            TxSentAuthLayerPduIdAtSecOC;
  /* ID of the cryptographic Pdu at SecOC (used for e.g. SecOC_TxConfirmation) */
  uint16                            TxCryptoLayerPduIdAtSecOC;
#endif
#if (SECOC_TX_USE_MESS_LINKER == STD_ON)
  /* Length of message link in the Pdu */
  uint16                            TxMessageLinkLength;
  /* Position of message linker inside the authentic I-PDU */
  uint16                            TxMessageLinkPos;
#endif
  /* Number of maximal allowed authentication attempts */
  uint16                            MaxAuthAttempts;
#if (STD_ON == SECOC_MACGENERATE_STATUS_PROP_ENABLED)
  /* Defines how the MAC Generate status shall be propagated */
  uint8                             MacGenerateStatusPropagationMode;
#endif
#if (SECOC_QUERY_FRESHNESS_VALUE != SECOC_FRESHNESS_NONE)
  /* Defines which freshness function should be called */
  uint8                             GetTxFreshnessFuncType;
#endif
#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
  /* Flag which indicates whether the secured PDU collection is used or not */
  boolean                           UseSecPduCollection;
#endif
#if (SECOC_TX_SHAPE_SEC_PDU == STD_ON)
  /* Flag which indicates whether the functionality to shape the secured PDU is used or not */
  boolean                           TxShapeSecuredPdu;
#endif /* #if (SECOC_TX_SHAPE_SEC_PDU == STD_ON) */
#if (SECOC_TX_CSM_ASYNC == STD_ON)
  /* Flag which indicates whether the Csm service is used in asynchronous mode or not */
  boolean                           UseCsmAsync;
#endif /* #if (SECOC_RX_CSM_ASYNC == STD_ON) */
  /* Flag which indicates whether the TP interfaces (in the communication with PduR) will be used or not */
  boolean                           UseTp;
#if (SECOC_TX_SYNC_PDU_PROCESSING == STD_ON)
  /* Flag indicating whether synchronous processing will be used or not */
  boolean                            UseTxSyncProcessing;
#endif
}
SecOC_TxConfigType;


/* !LINKSTO  SecOC.Dsn.interface.Rx.SecOC_RxDataType,10 */
/**
 * \struct SecOC_RxDataType
 * \brief Structure holding the Datas of a Rx Pdu
 */
typedef struct
{
#if (SECOC_USE_CSMJOBREF_CALLOUT == STD_ON)
  /* !LINKSTO  SecOC_EB_Rx_xxx21,1 */
  /* The Csm Job ID that shall be used (if SecOCCsmJobRefCallout enabled) */
  uint32                                   CsmJobIdPostDefined;
#endif
#if (SECOC_RX_USE_SAME_BUFFER == STD_ON)
  /* Flag which indicates if the buffer is available or used */
  CONSTP2VAR(uint8, TYPEDEF, SECOC_VAR)    RxBufferUsed;
#endif
  /* Pointer to buffer holding the received secured Pdu */
  P2VAR(uint8, TYPEDEF, SECOC_VAR)         RxReceivedSecuredPduBuffer;
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
  /* Pointer to buffer holding the received authentic Pdu */
  P2VAR(uint8, TYPEDEF, SECOC_VAR)         RxReceivedAuthPduBuffer;
  /* Pointer to buffer holding the cryptographic Pdu */
  P2VAR(uint8, TYPEDEF, SECOC_VAR)         RxCryptoPduBuffer;
#endif
  /* Pointer to buffer holding the secured Pdu */
  P2VAR(uint8, TYPEDEF, SECOC_VAR)         RxSecuredPduBuffer;
  /* Pointer to buffer holding the data to authenticator (used for eg. MAC services) */
  P2VAR(uint8, TYPEDEF, SECOC_VAR)         RxDataToAuthenticator;
  /* Pointer to buffer holding the complete freshness value */
  P2VAR(uint8, TYPEDEF, SECOC_VAR)         RxFreshnessVerifyValue;
  /* Pointer to buffer holding the authenticator (eg. MAC) */
  P2VAR(uint8, TYPEDEF, SECOC_VAR)         RxAuthenticator;
#if (SECOC_RX_AUTH_DATA_FRESHNESS == STD_ON)
  /* Pointer to buffer holding the part of the secured Pdu that will be sent to the freshness value manager */
  P2VAR(uint8, TYPEDEF, SECOC_VAR)         AuthDataFreshnessValueBuffer;
#endif
  /* The length of the secured PDU */
  PduLengthType                            RxSecuredDataLength;
  /* The length of the payload within the secured PDU (authentic data) */
  PduLengthType                            RxDataLength;
  /* Received data length from secured Pdu in bytes which has already been received via TP */
  PduLengthType                            ReceivedTpSecuredDataLength;
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
  /* Received data length from authentic Pdu in bytes which has already been received via TP */
  PduLengthType                            ReceivedTpAuthDataLength;
  /* Received data length from crypto Pdu in bytes which has already been received via TP */
  PduLengthType                            ReceivedTpCryptoDataLength;
#endif
  /* Number of authentication (build) attempts */
  uint16                                   AuthAttempts;
  /* Number of sync attempts */
  uint16                                   FreshnessValueSyncAttempts;
  /* The verification procedure result */
  SecOC_VerificationResultType             VerificationResult;
  /* The verification override status (on init, nothing shall be overridden) */
  uint8                                    VerifyStatusOverride;
  /* The number of messages to override (on init, nothing shall be overridden) */
  uint8                                    NumMsgToOverride;
  /* The index of the SecOC Rx state machine for each PDU configured */
  uint8                                    RxState;
  /* Rx state machine events emitted by APIs to the PduR */
  uint8                                    RxPduREvent;
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
  /* Rx state machine events emitted by APIs to the PduR */
  uint8                                    RxPduRIndicationForAuth;
  /* Tx state state machine events emitted by APIs to the PduR */
  uint8                                    RxPduRIndicationForCrypto;
#endif
#if (SECOC_RX_CSM_ASYNC == STD_ON)
  /* Rx state machine events emitted by APIs to the verifier */
  uint8                                    RxVerEvent;
#endif
  /* Variable to hold the verification result of the Csm */
  Crypto_VerifyResultType                  CryptoVerificationResult;
#if (SECOC_VERIFICATION_STATUS_PROP_ENABLED == SECOC_VERIFICATION_STATUS_PROP_EB)
  /* The return value of the MAC verification or get freshness procedure */
  Std_ReturnType                           VerificationReturn;
#endif
  /* Flag which indicates if the buffers corresponding to a Pdu are currently in use */
  boolean                                  RxPduInProcess;
  /* Flag which indicates if the verification of the Secured I-PDU is currently ongoing */
  boolean                                  ProcessingVerification;
#if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_ON)
  /* Flag which indicates if the SecOC_RxRenewVerifyStatusOverride was already called */
  boolean                                  RenewedVerStatus;
#endif
#if (SECOC_RX_USETP == STD_ON)
  /* Flag which indicates if the StartOfReception function was called in the upper layer */
  boolean                                  UpperLayerCallFlag;
#endif
}
SecOC_RxDataType;

/* !LINKSTO  SecOC.Dsn.interface.Tx.SecOC_TxDataType,9 */
/**
 * \struct SecOC_TxDataType
 * \brief Structure holding the Datas of a Tx Pdu
 */
typedef struct
{
#if (SECOC_USE_CSMJOBREF_CALLOUT == STD_ON)
  /* !LINKSTO  SecOC_EB_Tx_xxx31,1 */
  /* The Csm Job ID that shall be used (if SecOCCsmJobRefCallout enabled) */
  uint32                                   CsmJobIdPostDefined;
#endif
#if (SECOC_USE_TX_CONFIRMATION_TIMEOUT == STD_ON)
  /* Timeout counter for TxConfirmation */
  uint32                                   TxConfirmationTimeoutCounter;
#endif
  /* Variable passed to the authentication algorithm, where the length result is stored */
  uint32                                   TxAuthenticatorLengthResult;
#if (SECOC_TX_USE_SAME_BUFFER == STD_ON)
  /* Flag which indicates if the buffer is available or used */
  CONSTP2VAR(uint8, TYPEDEF, SECOC_VAR)    TxBufferUsed;
#endif
  /* Pointer to buffer holding the received authentic Pdu */
  P2VAR(uint8, TYPEDEF, SECOC_VAR)         TxReceivedAuthPduBuffer;
  /* Pointer to buffer holding the secured Pdu */
  P2VAR(uint8, TYPEDEF, SECOC_VAR)         TxSecuredPduBuffer;
  /* Pointer to buffer holding the data to authenticator (used for eg. MAC services) */
  P2VAR(uint8, TYPEDEF, SECOC_VAR)         TxDataToAuthenticator;
  /* Pointer to buffer holding the authenticator (eg. MAC) */
  P2VAR(uint8, TYPEDEF, SECOC_VAR)         TxAuthenticator;
  /* The length of the payload within the secured PDU (authentic data) */
  PduLengthType                            TxDataLength;
  /* Data length in bytes which has been received via SecOC_Transmit */
  PduLengthType                            TxReceivedDataLength;
  /* Copied data from the secured PDU length (copied via SecOC_CopyTxData) */
  PduLengthType                            CopiedDataFromSecLength;
#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
  /* Copied data from the sent authentic PDU length (copied via SecOC_CopyTxData) */
  PduLengthType                            CopiedDataFromAuthLength;
  /* Copied data from the cryptographic PDU length (copied via SecOC_CopyTxData) */
  PduLengthType                            CopiedDataFromCryptoLength;
#endif
  /* Variable to provide the secured Pdu to the lower layer */
  PduInfoType                              TxSecuredPdu;
#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
  /* Variable to provide the sent authentic Pdu to the lower layer */
  PduInfoType                              TxSentAuthPdu;
  /* Variable to provide the cryptographic Pdu to the lower layer */
  PduInfoType                              TxCryptoPdu;
#endif
  /* Number of authentication (build) attempts */
  uint16                                   AuthAttempts;
  /* The index of the SecOC Tx state machine for each PDU configured */
  uint8                                    TxState;
  /* Tx state machine events emitted by APIs to the PduR */
  uint8                                    TxPduREvent;
#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
  /* Tx state machine events emitted by APIs to the PduR */
  uint8                                    TxPduRConfirmationForAuth;
  /* Tx state machine events emitted by APIs to the PduR */
  uint8                                    TxPduRConfirmationForCrypto;
#endif
  /* Tx state machine events emitted by SecOC_Transmit */
  uint8                                    TxTransmitEvent;
#if (SECOC_TX_CSM_ASYNC == STD_ON)
 /* Tx state machine events emitted by APIs to the authenticator */
  uint8                                    TxAuthEvent;
#endif
#if (SECOC_MACGENERATE_STATUS_PROP_ENABLED == STD_ON)
  /* Status of the MAC Generate procedure */
  Std_ReturnType                           MacGenerateResult;
#endif
  /* Flag which indicates if the authentication of the I-PDU is currently ongoing */
  boolean                                  ProcessingAuthentication;
}
SecOC_TxDataType;

/*==================[external constants]==========================================================*/

#define SECOC_START_SEC_CONST_UNSPECIFIED
#include <SecOC_MemMap.h>

#if (SECOC_NUMBER_RX_PDUS > 0)
/** \brief Array holding the configuration of each configured RxPduProcessing
 */
extern CONST(SecOC_RxConfigType, SECOC_CONST) SecOC_RxConfig[SECOC_NUMBER_RX_PDUS];
#endif

#if (SECOC_NUMBER_TX_PDUS > 0)
/** \brief Array holding the configuration of each configured TxPduProcessing
 */
extern CONST(SecOC_TxConfigType, SECOC_CONST) SecOC_TxConfig[SECOC_NUMBER_TX_PDUS];
#endif

#define SECOC_STOP_SEC_CONST_UNSPECIFIED
#include <SecOC_MemMap.h>

#if (SECOC_NUMBER_FUNC_VERIFICATION_STATUS_PROP > 0)
#if (SECOC_VERIFICATION_STATUS_PROP_ENABLED != SECOC_VERIFICATION_STATUS_PROP_NONE)
#define SECOC_START_SEC_CONST_UNSPECIFIED
#include <SecOC_MemMap.h>

/**
 * \brief Variable which holds the configured callout functions for propagating the verification status
 */
extern CONST(SecOC_VerificationStatusCalloutType, SECOC_VAR) SecOC_RxVerificationStatusCallout[SECOC_NUMBER_FUNC_VERIFICATION_STATUS_PROP];

#define SECOC_STOP_SEC_CONST_UNSPECIFIED
#include <SecOC_MemMap.h>
#endif
#endif

#if (SECOC_NUMBER_FUNC_MACGENERATE_STATUS_PROP > 0)
#if (SECOC_MACGENERATE_STATUS_PROP_ENABLED == STD_ON)
#define SECOC_START_SEC_CONST_UNSPECIFIED
#include <SecOC_MemMap.h>

/**
 * \brief Variable which holds the configured callout functions for propagating the MAC Generate status
 */
extern CONST(SecOC_MacGenerateStatusCalloutType, SECOC_VAR) SecOC_TxMacGenerateStatusCallout[SECOC_NUMBER_FUNC_MACGENERATE_STATUS_PROP];

#define SECOC_STOP_SEC_CONST_UNSPECIFIED
#include <SecOC_MemMap.h>
#endif
#endif

/*==================[external data]===============================================================*/

#define SECOC_START_SEC_VAR_INIT_UNSPECIFIED
#include <SecOC_MemMap.h>

/*==================[variables for Rx side]=======================================================*/

#if (SECOC_NUMBER_RX_PDUS > 0)
/** \brief Data structure holding the Data for Rx Pdus, which shall be verified.
 */
extern VAR(SecOC_RxDataType, SECOC_VAR) SecOC_RxData[SECOC_NUMBER_RX_PDUS];
#endif

/*==================[variables for Tx side]=======================================================*/

#if (SECOC_NUMBER_TX_PDUS > 0)
/** \brief Data structure holding the Data for Tx Pdus, which shall be authenticated.
 */
extern VAR(SecOC_TxDataType, SECOC_VAR) SecOC_TxData[SECOC_NUMBER_TX_PDUS];
#endif

#define SECOC_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <SecOC_MemMap.h>

#if (SECOC_NUMBER_TX_PDUS > 0)
#if (SECOC_BYPASS_AUTHENTICATION_ROUTINE == STD_ON)

#define SECOC_START_SEC_VAR_CLEARED_BOOLEAN
#include <SecOC_MemMap.h>

/**
 * \brief Flag which indicates the bypass mechanism state (enabled or disabled)
 */
extern VAR(boolean, SECOC_VAR) SecOC_TxBypassAuthRoutineState;

#define SECOC_STOP_SEC_VAR_CLEARED_BOOLEAN
#include <SecOC_MemMap.h>

#endif /* #if (SECOC_BYPASS_AUTHENTICATION_ROUTINE == STD_ON) */
#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */

/*==================[variables for Rx and Tx side]================================================*/

#define SECOC_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <SecOC_MemMap.h>

/** \brief Pointer used to access the initialization structure that it is used during
 *         the init phase, when the SecOC_Init is called
 */
extern P2CONST(SecOC_ConfigType, AUTOMATIC, SECOC_APPL_CONST) SecOC_ConfigPtr;

#define SECOC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <SecOC_MemMap.h>

#define SECOC_START_SEC_VAR_INIT_8
#include <SecOC_MemMap.h>

/**
 * \brief Initialization status of the SecOC. It is declared as volatile to
 *        prevent the compiler from reordering accesses to this variable because
 *        we rely on the sequence of assignments in SecOC_Init() and SecOC_DeInit()
 */
extern volatile VAR(SecOC_StateType, SECOC_VAR) SecOC_InitStatus;

#define SECOC_STOP_SEC_VAR_INIT_8
#include <SecOC_MemMap.h>

#define SECOC_START_SEC_VAR_CLEARED_8
#include <SecOC_MemMap.h>

/** \brief Array used for holding the received and transmitted PDUs
 */
extern VAR(uint8, SECOC_VAR) SecOC_PB_PduBuffer[SECOC_PB_PDU_MEM_SIZE];

/** \brief Array used for holding the internal data that is required for processing the verification
 *         and the authentication
 */
extern VAR(uint8, SECOC_VAR) SecOC_PB_IntBuffer[SECOC_PB_INT_MEM_SIZE];

#define SECOC_STOP_SEC_VAR_CLEARED_8
#include <SecOC_MemMap.h>

/*==================[external function declarations]==============================================*/

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

/** \brief Function to shift all bits of an array left
 *
 * The function shifts all bits of an array left and fills
 * the LSB with zeros (arithmetic shift left)
 *
 * \param[in,out] array       pointer to the data which needs to be shifted
 * \param[in]     bitLength   length in bits starting from the LSB of the array
 * \param[in]     shift       number with which the shift will be done (must be smaller than 8)
 *
 */
extern FUNC(void, SECOC_CODE) SecOC_ShiftLeft
(
  P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) array,
        uint32                             bitLength,
        uint8                              shift
);

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

#undef TS_RELOCATABLE_CFG_ENABLE

#endif /* #ifndef SECOC_INT_H */

/*==================[end of file]=================================================================*/
