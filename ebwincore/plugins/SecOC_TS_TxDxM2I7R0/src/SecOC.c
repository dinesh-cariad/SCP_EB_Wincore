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

/*==================[deviations]============================================================*/

/*==================[includes]====================================================================*/

/* !LINKSTO SecOC.Dsn.BasicSWStructure.RxTx.SecOCc,5 */
#include <SecOC_Int.h>

#if (STD_ON == SECOC_INCLUDE_RTE)
#include <Rte_SecOC.h>
#endif

#if (STD_ON == SECOC_DET_ENABLED)
#include <Det.h>
#endif

#if (STD_ON == SECOC_PBCFGM_SUPPORT_ENABLED)
#include <PbcfgM.h>
#endif

/*==================[macros]======================================================================*/

/** \brief Defined value which indicates that the PDU ID is not corresponding to SecOC
 */
#if (defined SECOC_UNKNOWN_PDU)
#error SECOC_UNKNOWN_PDU already defined
#endif
#define SECOC_UNKNOWN_PDU      0U

/** \brief Defined value which indicates that the PDU ID corresponds
 **        to a configured authentic PDU ID
 */
#if (defined SECOC_AUTH_PDU)
#error SECOC_AUTH_PDU already defined
#endif
#define SECOC_AUTH_PDU  1U

/** \brief Defined value which indicates that the PDU ID corresponds
 **        to a configured cryptographic PDU ID
 */
#if (defined SECOC_CRYPTO_PDU)
#error SECOC_CRYPTO_PDU already defined
#endif
#define SECOC_CRYPTO_PDU    2U

/** \brief Defined value which indicates that the PDU ID corresponds
 **        to a configured secured PDU ID
 */
#if (defined SECOC_SECURED_PDU)
#error SECOC_SECURED_PDU already defined
#endif
#define SECOC_SECURED_PDU    3U

#if (SECOC_NUMBER_RX_PDUS > 0)
#if (STD_ON == SECOC_PDU_SECURED_AREA)
/** \brief Macro to calculate the sum of the Rx minimum required authentic data length
 *         that is comprised of the offset position and the length of the secured area,
 *         when the secured area is used
 */
#if (defined SECOC_RX_SECAREA_MIN_AUTHDATALEN)
#error SECOC_RX_SECAREA_MIN_AUTHDATALEN already defined
#endif
#define SECOC_RX_SECAREA_MIN_AUTHDATALEN(Id) ((uint32)(SECOC_GET_RX_SECURED_AREA_LEN((Id))) + \
                                              (uint32)(SECOC_GET_RX_SECURED_AREA_OFFSET((Id))))
#endif /* #if (STD_ON == SECOC_PDU_SECURED_AREA) */

#if (STD_ON == SECOC_RX_USE_MESS_LINKER)
/** \brief Macro to calculate the sum of the Rx minimum required authentic data length
 *         that is comprised of the offset position and the length of the message link,
 *         when the message link is used
 */
#if (defined SECOC_RX_MSGLINK_MIN_AUTHDATALEN)
#error SECOC_RX_MSGLINK_MIN_AUTHDATALEN already defined
#endif
#define SECOC_RX_MSGLINK_MIN_AUTHDATALEN(Id) ((uint32)SecOC_RxConfig[(Id)].RxMessageLinkLength + \
                                              (uint32)SecOC_RxConfig[(Id)].RxMessageLinkPos)
#endif /* #if (STD_ON == SECOC_RX_USE_MESS_LINKER) */
#endif /* #if (SECOC_NUMBER_RX_PDUS > 0) */

#if (SECOC_NUMBER_TX_PDUS > 0)
#if (STD_ON == SECOC_PDU_SECURED_AREA)
/** \brief Macro to calculate the sum of the Tx minimum required authentic data length
 *         that is comprised of the offset position and the length of the secured area,
 *         when the secured area is used
 */
#if (defined SECOC_TX_SECAREA_MIN_AUTHDATALEN)
#error SECOC_TX_SECAREA_MIN_AUTHDATALEN already defined
#endif
#define SECOC_TX_SECAREA_MIN_AUTHDATALEN(Id) ((uint32)(SECOC_GET_TX_SECURED_AREA_LEN((Id))) + \
                                              (uint32)(SECOC_GET_TX_SECURED_AREA_OFFSET((Id))))
#endif /* #if (STD_ON == SECOC_PDU_SECURED_AREA) */

#if (STD_ON == SECOC_TX_USE_MESS_LINKER)
/** \brief Macro to calculate the sum of the Tx minimum required authentic data length
 *         that is comprised of the offset position and the length of the message link,
 *         when the message link is used
 */
#if (defined SECOC_TX_MSGLINK_MIN_AUTHDATALEN)
#error SECOC_TX_MSGLINK_MIN_AUTHDATALEN already defined
#endif
#define SECOC_TX_MSGLINK_MIN_AUTHDATALEN(Id) ((uint32)SecOC_TxConfig[(Id)].TxMessageLinkLength + \
                                              (uint32)SecOC_TxConfig[(Id)].TxMessageLinkPos)
#endif /* #if (STD_ON == SECOC_TX_USE_MESS_LINKER) */
#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */

/*==================[type definitions]============================================================*/

/*==================[internal function declarations]==============================================*/

#if (SECOC_NUMBER_RX_PDUS > 0)

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

/** \brief Function to determine if the external received PDU ID corresponds to an internal
 *         equivalent index, converts the external ID into the internal equivalent Index
 *         and checks whether the received ID is for a secured/authentic/cryptographic PDU.
 * \param[in,out] rxPduIdx    in: received PDU ID  out: internal equivalent Index
 * \param[out]    rxPduType   Contains the PDU type (eq. authentic, cryptographic)
 * \returns whether the external PDU has an internal equivalent Index
 * \retval TRUE   an internal equivalent Index was found
 * \retval FALSE  an internal equivalent Index was not found
 */
static FUNC(boolean, SECOC_CODE) SecOC_RxCheckForPduIdx
(
  P2VAR(uint16, AUTOMATIC, SECOC_APPL_DATA) rxPduIdx,
  P2VAR(uint8,  AUTOMATIC, SECOC_APPL_DATA) rxPduType
);

#if (STD_ON == SECOC_RX_USE_SAME_BUFFER)
/** \brief Function used to determine whether the incoming PDU can be stored or not,
 *         i.e. whether the required buffer to store the incoming PDU is free or not.
 * \param[in]        pduIdx     Contains the PDU ID
 * \returns whether the required buffer is free or not
 * \retval TRUE      the buffer it is free, can be used
 * \retval FALSE     the buffer it isn't free
 */
static FUNC(boolean, SECOC_CODE) SecOC_RxCheckBufferAvailability
(
  uint16 pduIdx
);
#endif

/** \brief Function to determine whether the incoming PDU is valid based on its length,
 *         i.e. whether its actual length corresponds to the required length.
 * \param[in] pduLength  The length of the incoming PDU that needs to be verified
 * \param[in] pduIdx     Contains the PDU ID
 * \param[in] pduType    Contains the PDU type (e.q. authentic, cryptographic)
 * \returns whether the external PDU has the required length
 * \retval BUFREQ_OK        the length of the PDU is the required one
 * \retval BUFREQ_E_NOT_OK  the length of the PDU is not the required one (e.g. too small to extract
 *                          the message link, secured area)
 * \retval BUFREQ_E_OVFL    the incoming PDU is too big to store it internally
 */
static FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_RxCheckIncomingPdu
(
  uint32 pduLength,
  uint16 pduIdx,
  uint8  pduType
);

/** \brief Function to determine whether the incoming secured PDU is valid based on its length,
 *         i.e. whether its actual length corresponds to the required length.
 * \param[in] pduLength  The length of the incoming PDU that needs to be verified
 * \param[in] pduIdx     Contains the PDU ID
 * \returns whether the external PDU has the required length
 * \retval BUFREQ_OK        the length of the PDU is the required one
 * \retval BUFREQ_E_NOT_OK  the length of the PDU is not the required one (e.g. too small to extract
 *                          secured area)
 * \retval BUFREQ_E_OVFL    the incoming PDU is too big to store it internally
 */
static FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_RxCheckIncomingSecuredPdu
(
  uint32 pduLength,
  uint16 pduIdx
);

#if (SECOC_RX_USE_MESS_LINKER == STD_ON)
/** \brief Function to determine if the message linker values are matching
 *
 * The function checks if the message link from the cryptographic PDU is matching
 * with the respective part from the authentic PDU
 *
 * \param[in]   bufferAData   Pointer to the location where the first buffer is stored
 * \param[in]   bufferABitPos Start position from where the data must match
 * \param[in]   bufferBData   Pointer to the location where the second buffer is stored
 * \param[in]   bufferBBitPos Start position from where the data must match
 * \param[in]   bitLength     The data length on which the checking will be performed
 *
 * \returns Whether data from this two buffers is matching or not
 * \retval E_OK      The data from this two buffers is matching
 * \retval E_NOT_OK  The data from this two buffers is not matching
 */
static FUNC(Std_ReturnType, SECOC_CODE) SecOC_RxCheckMessageLink
(
  P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) bufferAData,
          uint32                             bufferABitPos,
  P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) bufferBData,
          uint32                             bufferBBitPos,
          uint32                             bitLength
);
#endif

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

#endif /* #if (SECOC_NUMBER_RX_PDUS > 0) */

#if (SECOC_NUMBER_TX_PDUS > 0)
#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

/** \brief Function used to determine whether the incoming PDU can be stored or not,
 *         i.e. whether the required buffer to store the incoming PDU is free or not.
 * \param[in]        pduIdx     Contains the PDU ID
 * \returns whether the required buffer is free or not
 * \retval TRUE      the buffer it is free, can be used
 * \retval FALSE     the buffer it isn't free
 */
static FUNC(uint8, SECOC_CODE) SecOC_TxCheckBufferAvailability
(
  uint16 pduIdx
);

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

#endif
#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */

/*==================[internal constants]==========================================================*/

/*==================[internal data]===============================================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[variables for Rx side]=======================================================*/

/*==================[variables for Tx side]=======================================================*/

#if (SECOC_NUMBER_TX_PDUS > 0)
#if (SECOC_BYPASS_AUTHENTICATION_ROUTINE == STD_ON)

#define SECOC_START_SEC_VAR_CLEARED_BOOLEAN
#include <SecOC_MemMap.h>

/**
 * \brief Flag which indicates the bypass mechanism state (enabled or disabled)
 */
VAR(boolean, SECOC_VAR) SecOC_TxBypassAuthRoutineState;

#define SECOC_STOP_SEC_VAR_CLEARED_BOOLEAN
#include <SecOC_MemMap.h>

#endif /* #if (SECOC_BYPASS_AUTHENTICATION_ROUTINE == STD_ON) */
#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */

/*==================[variables for Rx and Tx side]================================================*/

#define SECOC_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <SecOC_MemMap.h>

P2CONST(SecOC_ConfigType, AUTOMATIC, SECOC_APPL_CONST) SecOC_ConfigPtr;

#define SECOC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <SecOC_MemMap.h>

#define SECOC_START_SEC_VAR_INIT_8
#include <SecOC_MemMap.h>

/**
 * \brief Initialization status of the SecOC. It is declared as volatile to
 *        prevent the compiler from reordering accesses to this variable because
 *        we rely on the sequence of assignments in SecOC_Init() and SecOC_DeInit()
 */
volatile VAR(SecOC_StateType, SECOC_VAR) SecOC_InitStatus = SECOC_UNINIT;

#define SECOC_STOP_SEC_VAR_INIT_8
#include <SecOC_MemMap.h>

/*==================[external function definitions]===============================================*/

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

FUNC(void, SECOC_CODE) SecOC_ShiftLeft
(
  P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) array,
        uint32                             bitLength,
        uint8                              shift
)
{
  uint16 arrayIdx;

  /* length of complete bytes after the shift */
  const uint32 byteLength_shifted = (bitLength - shift) / 8U;

  for (arrayIdx = 0U; arrayIdx < byteLength_shifted; arrayIdx++)
  {
    array[arrayIdx] = ((uint8) (array[arrayIdx]      << shift       )) |
                          ((uint8) (array[arrayIdx + 1U] >> (8U - shift)));
  }

  if (((bitLength % 8U) >= shift) || (0U == (bitLength % 8U)))
  {
    array[byteLength_shifted] = array[byteLength_shifted] << shift;
  }
  else
  {
    array[byteLength_shifted] = ((uint8) (array[byteLength_shifted] << shift)  ) |
                                (array[byteLength_shifted + 1U] >> (8U - shift));
  }
}

/* !LINKSTO SecOC.Dsn.interface.RxTx.SecOC_Init,2, SWS_SecOC_RxTx_00106,1 */
FUNC(void, SECOC_CODE) SecOC_Init
(
  P2CONST(SecOC_ConfigType, AUTOMATIC, SECOC_APPL_CONST) config
)
{
  uint16                pduIdx;
  P2CONST(SecOC_ConfigType, AUTOMATIC, SECOC_APPL_CONST) LocalConfigPtr = config;

  /* Post Build Initializations */
  /* !LINKSTO SecOC.PB.Impl.PbCfgM,1 */
#if ( SECOC_PBCFGM_SUPPORT_ENABLED == STD_ON )
  if (LocalConfigPtr == NULL_PTR)
  {
    PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;

    if (E_OK == PbcfgM_GetConfig(SECOC_MODULE_ID, SECOC_INSTANCE_ID, &ModuleConfig))
    {
      LocalConfigPtr = (P2CONST(SecOC_ConfigType, AUTOMATIC, SECOC_APPL_CONST)) ModuleConfig;
    }
  }
#endif

  if(SecOC_IsValidConfig(LocalConfigPtr) == E_OK)
  {
    if (
        (SECOC_PB_PDU_MEM_SIZE >= LocalConfigPtr->PduBufferSize) &&
        (SECOC_PB_INT_MEM_SIZE >= LocalConfigPtr->IntBufferSize)
       )
    {
      /* Set the pointer to the post-build configuration */
      SecOC_ConfigPtr = LocalConfigPtr;

      TS_MemBZero
      (
        SecOC_PB_PduBuffer,
        SECOC_PB_PDU_MEM_SIZE
      );
      TS_MemBZero
      (
        SecOC_PB_IntBuffer,
        SECOC_PB_INT_MEM_SIZE
      );

      /* !LINKSTO SWS_SecOC_RxTx_00054,1 */
#if (SECOC_NUMBER_RX_PDUS > 0)
      /* Initialize the Rx state machine, events and the global buffers */

      for (pduIdx = 0U; pduIdx < SECOC_NUMBER_RX_PDUS; pduIdx++)
      {
        /* Reset the state machine */
        SecOC_RxData[pduIdx].RxState     = SECOC_RX_STATE_IDLE;

        /* Clear the events */

        /* Clear the events which are indicating functions calls like SecOC_StartOfReception */
        SecOC_RxData[pduIdx].RxPduREvent = SECOC_RXEVENT_NONE;
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
        /* Clear the events which are indicating functions calls like SecOC_TpRxIndication */
        SecOC_RxData[pduIdx].RxPduRIndicationForAuth = SECOC_RXEVENT_NONE;
        SecOC_RxData[pduIdx].RxPduRIndicationForCrypto = SECOC_RXEVENT_NONE;
#endif
#if (SECOC_RX_CSM_ASYNC == STD_ON)
        /* Clear the events which are indicating functions calls like SecOC_CsmCallbackNotification */
        SecOC_RxData[pduIdx].RxVerEvent = SECOC_RXEVENT_NONE;
#endif
        /* Clear the flag which is indicating if there is an ongoing verification procedure */
        SecOC_RxData[pduIdx].RxPduInProcess = FALSE;
        /* Clear the flag used for continues processing */
        SecOC_RxData[pduIdx].ProcessingVerification = FALSE;
        /* Reset the verification result */
        SecOC_RxData[pduIdx].CryptoVerificationResult = CRYPTO_E_VER_NOT_OK;

        /* !LINKSTO SWS_SecOC_00235,1 */
        /* Clear the counters used for authentication build and verify retries */
        SecOC_RxData[pduIdx].AuthAttempts = 0U;
        SecOC_RxData[pduIdx].FreshnessValueSyncAttempts = 0U;
        /* Clear the verification result */
        SecOC_RxData[pduIdx].VerificationResult = SECOC_VERIFICATIONFAILURE;
        /* Clear the variables used for overwriting the verification status */
        SecOC_RxData[pduIdx].VerifyStatusOverride = SECOC_OVERRIDESTATUS_CANCEL;
        SecOC_RxData[pduIdx].NumMsgToOverride = 0U;
#if (SECOC_VERIFICATION_STATUS_PROP_EB == SECOC_VERIFICATION_STATUS_PROP_ENABLED)
        SecOC_RxData[pduIdx].VerificationReturn = E_NOT_OK;
#endif
#if (STD_ON == SECOC_OVERRIDE_VERIFYSTATUS_PASS)
        /* Clear the flag which is indicating if the verification status was already overridden */
        SecOC_RxData[pduIdx].RenewedVerStatus = FALSE;
#endif
#if (STD_ON == SECOC_RX_USETP)
        /* Clear the flag which is indicating if PduR_SecOCStartOfReception was already called */
        SecOC_RxData[pduIdx].UpperLayerCallFlag = FALSE;
#endif
#if (STD_ON == SECOC_RX_USE_SAME_BUFFER)
        /* Clear the flags which is indicating if the same buffer is currently being used */
        if (SecOC_RxData[pduIdx].RxBufferUsed != NULL_PTR)
        {
          *SecOC_RxData[pduIdx].RxBufferUsed = FALSE;
        }
#endif

        /* Set pointers to buffers, used for storing the PDUs and for internal processing */
        SecOC_RxData[pduIdx].RxReceivedSecuredPduBuffer = SECOC_GET_RX_RECVSECURED_PDU_ADR(pduIdx);
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
        if (SecOC_RxConfig[pduIdx].UseSecPduCollection == TRUE)
        {
          SecOC_RxData[pduIdx].RxReceivedAuthPduBuffer = SECOC_GET_RX_RECVAUTH_PDU_ADR(pduIdx);
          SecOC_RxData[pduIdx].RxCryptoPduBuffer = SECOC_GET_RX_CRYPTO_PDU_ADR(pduIdx);
        }
        else
        {
          SecOC_RxData[pduIdx].RxReceivedAuthPduBuffer = NULL_PTR;
          SecOC_RxData[pduIdx].RxCryptoPduBuffer = NULL_PTR;
        }
#endif
        SecOC_RxData[pduIdx].RxSecuredPduBuffer = SECOC_GET_RX_SECURED_PDU_ADR(pduIdx);
        SecOC_RxData[pduIdx].RxAuthenticator = SECOC_GET_RX_AUTHENTICATOR_ADR(pduIdx);
        SecOC_RxData[pduIdx].RxDataToAuthenticator = SECOC_GET_RX_DATATOAUTH_ADR(pduIdx);

        /* Clear the dynamic length of the received data */
        SecOC_RxData[pduIdx].RxSecuredDataLength = 0U;
        SecOC_RxData[pduIdx].RxDataLength = 0U;
        SecOC_RxData[pduIdx].ReceivedTpSecuredDataLength = 0U;
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
        SecOC_RxData[pduIdx].ReceivedTpAuthDataLength = 0U;
        SecOC_RxData[pduIdx].ReceivedTpCryptoDataLength = 0U;
#endif

#if (SECOC_USE_CSMJOBREF_CALLOUT == STD_ON)
        /* !LINKSTO  SecOC_EB_Rx_xxx22,1, SecOC_EB_Rx_xxx23,1 */
        /* Get the new Csm job ID based on the configured one */
        SecOC_RxData[pduIdx].CsmJobIdPostDefined =
          SECOC_CSMJOBREF_CALLOUT(SecOC_RxConfig[pduIdx].CsmJobId);
#endif
      }
#endif /* #if (SECOC_NUMBER_RX_PDUS > 0) */

#if (SECOC_NUMBER_TX_PDUS > 0)
      /* Initialize the Tx state machine, events and the global buffers */

#if (SECOC_BYPASS_AUTHENTICATION_ROUTINE == STD_ON)
      SecOC_TxBypassAuthRoutineState = FALSE;
#endif

      for (pduIdx = 0U; pduIdx < SECOC_NUMBER_TX_PDUS; pduIdx++)
      {
        /* Reset the state machine */
        SecOC_TxData[pduIdx].TxState = SECOC_TX_STATE_IDLE;

        /* Clear the events */

        /* Clear the events which are indicating functions calls like SecOC_CancelTransmit */
        SecOC_TxData[pduIdx].TxPduREvent     = SECOC_TXEVENT_NONE;
#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
        /* Clear the events which are indicating functions calls like SecOC_TxConfirmation */
        SecOC_TxData[pduIdx].TxPduRConfirmationForAuth = SECOC_TXEVENT_NONE;
        SecOC_TxData[pduIdx].TxPduRConfirmationForCrypto = SECOC_TXEVENT_NONE;
#endif
        /* Clear the events which are indicating functions calls like SecOC_Transmit */
        SecOC_TxData[pduIdx].TxTransmitEvent = SECOC_TXEVENT_NONE;
#if (SECOC_TX_CSM_ASYNC == STD_ON)
        /* Clear the events which are indicating functions calls like SecOC_CsmCallbackNotification */
        SecOC_TxData[pduIdx].TxAuthEvent = SECOC_TXEVENT_NONE;
#endif
#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)
        /* Clear the buffer busy flags */
        SECOC_TX_SET_BUFFER_LVL1_STATUS(pduIdx, SECOC_BUFFER_FREE);
        SECOC_TX_SET_BUFFER_LVL2_STATUS(pduIdx, SECOC_BUFFER_FREE);
#endif
        /* Clear the flag used for continuous processing */
        SecOC_TxData[pduIdx].ProcessingAuthentication = FALSE;

        /* !LINKSTO SWS_SecOC_00226,1 */
        /* Clear the counters used for authentication build retries */
        SecOC_TxData[pduIdx].AuthAttempts = 0U;
#if (STD_ON == SECOC_MACGENERATE_STATUS_PROP_ENABLED)
        /* Clear the MAC generate result */
        SecOC_TxData[pduIdx].MacGenerateResult = E_NOT_OK;
#endif
#if (SECOC_USE_TX_CONFIRMATION_TIMEOUT == STD_ON)
        /* Clear the timeout counter used for skipping the call of SecOC_TpTxConfirmation */
        SecOC_TxData[pduIdx].TxConfirmationTimeoutCounter = 0U;
#endif

        /* Set pointers to buffers, used for storing the PDUs and for internal processing */
        SecOC_TxData[pduIdx].TxReceivedAuthPduBuffer = SECOC_GET_TX_ULAUTH_PDU_ADR(pduIdx);
        SecOC_TxData[pduIdx].TxSecuredPduBuffer = SECOC_GET_TX_SECURED_PDU_ADR(pduIdx);
        SecOC_TxData[pduIdx].TxAuthenticator = SECOC_GET_TX_AUTHENTICATOR_ADR(pduIdx);
        SecOC_TxData[pduIdx].TxDataToAuthenticator = SECOC_GET_TX_DATATOAUTH_ADR(pduIdx);

        TS_MemSet(&SecOC_TxData[pduIdx].TxSecuredPdu, 0U, sizeof(SecOC_TxData[pduIdx].TxSecuredPdu));
#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
        TS_MemSet(&SecOC_TxData[pduIdx].TxSentAuthPdu, 0U, sizeof(SecOC_TxData[pduIdx].TxSentAuthPdu));
        TS_MemSet(&SecOC_TxData[pduIdx].TxCryptoPdu, 0U, sizeof(SecOC_TxData[pduIdx].TxCryptoPdu));
#endif

        /* Clear the dynamic length of the received data */
        SecOC_TxData[pduIdx].TxDataLength = 0U;
        SecOC_TxData[pduIdx].TxReceivedDataLength = 0U;
        SecOC_TxData[pduIdx].CopiedDataFromSecLength = 0U;
#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
        SecOC_TxData[pduIdx].CopiedDataFromAuthLength = 0U;
        SecOC_TxData[pduIdx].CopiedDataFromCryptoLength = 0U;
#endif

#if (SECOC_USE_CSMJOBREF_CALLOUT == STD_ON)
        /* !LINKSTO  SecOC_EB_Tx_xxx32,1, SecOC_EB_Tx_xxx33,1 */
        /* Get the new Csm job ID based on the configured one */
        SecOC_TxData[pduIdx].CsmJobIdPostDefined =
          SECOC_CSMJOBREF_CALLOUT(SecOC_TxConfig[pduIdx].CsmJobId);
#endif
      }

#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */

      /* !LINKSTO SWS_SecOC_RxTx_00106,1 */
      SecOC_InitStatus = SECOC_INIT;
    }
  }
}

/* !LINKSTO SWS_SecOC_RxTx_00161,1 */
FUNC(void, SECOC_CODE) SecOC_DeInit
(
  void
)
{
  uint16 pduIdx;

  /* !LINKSTO SWS_SecOC_RxTx_00161,1 */
  SecOC_InitStatus = SECOC_UNINIT;

#if (SECOC_NUMBER_RX_PDUS > 0)
  /* DeInitialize the Rx state machine and the global buffers */

  /* !LINKSTO SWS_SecOC_RxTx_00157,1 */
  for (pduIdx = 0U; pduIdx < SECOC_NUMBER_RX_PDUS; pduIdx++)
  {
    /* Release pointers to buffers, used for storing the PDUs and for internal processing */

    SecOC_RxData[pduIdx].RxReceivedSecuredPduBuffer = NULL_PTR;
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
    if (SecOC_RxConfig[pduIdx].UseSecPduCollection == TRUE)
    {
      SecOC_RxData[pduIdx].RxReceivedAuthPduBuffer = NULL_PTR;
      SecOC_RxData[pduIdx].RxCryptoPduBuffer = NULL_PTR;
    }
#endif
    SecOC_RxData[pduIdx].RxSecuredPduBuffer = NULL_PTR;
    SecOC_RxData[pduIdx].RxAuthenticator = NULL_PTR;
    SecOC_RxData[pduIdx].RxDataToAuthenticator = NULL_PTR;

    /* Clear the dynamic length of the received data */
    SecOC_RxData[pduIdx].RxSecuredDataLength = 0U;
    SecOC_RxData[pduIdx].RxDataLength = 0U;
    SecOC_RxData[pduIdx].ReceivedTpSecuredDataLength = 0U;
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
    SecOC_RxData[pduIdx].ReceivedTpAuthDataLength = 0U;
    SecOC_RxData[pduIdx].ReceivedTpCryptoDataLength = 0U;
#endif
  }
#endif /* #if (SECOC_NUMBER_RX_PDUS > 0) */

#if (SECOC_NUMBER_TX_PDUS > 0)
  /* DeInitialize the Tx state machine and the global buffers */

  /* !LINKSTO SecOC.Dsn.interface.Tx.SecOC_DeInitBuffer,1, SWS_SecOC_RxTx_00157,1 */
  for (pduIdx = 0U; pduIdx < SECOC_NUMBER_TX_PDUS; pduIdx++)
  {
    /* Release pointers to buffers, used for storing the PDUs and for internal processing */
    SecOC_TxData[pduIdx].TxReceivedAuthPduBuffer = NULL_PTR;
    SecOC_TxData[pduIdx].TxSecuredPduBuffer = NULL_PTR;
    SecOC_TxData[pduIdx].TxAuthenticator = NULL_PTR;
    SecOC_TxData[pduIdx].TxDataToAuthenticator = NULL_PTR;

    /* Clear the dynamic length of the received data */
    SecOC_TxData[pduIdx].TxDataLength = 0U;
    SecOC_TxData[pduIdx].TxReceivedDataLength = 0U;
    SecOC_TxData[pduIdx].CopiedDataFromSecLength = 0U;
#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
    SecOC_TxData[pduIdx].CopiedDataFromAuthLength = 0U;
    SecOC_TxData[pduIdx].CopiedDataFromCryptoLength = 0U;
#endif

    TS_MemSet(&SecOC_TxData[pduIdx].TxSecuredPdu, 0U, sizeof(SecOC_TxData[pduIdx].TxSecuredPdu));
#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
    TS_MemSet(&SecOC_TxData[pduIdx].TxSentAuthPdu, 0U, sizeof(SecOC_TxData[pduIdx].TxSentAuthPdu));
    TS_MemSet(&SecOC_TxData[pduIdx].TxCryptoPdu, 0U, sizeof(SecOC_TxData[pduIdx].TxCryptoPdu));
#endif
  }

#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */

}

#if (STD_ON == SECOC_VERSION_INFO_API)
/* !LINKSTO SWS_SecOC_RxTx_00107,1, SWS_SecOC_RxTx_00103,2 */
FUNC(void, SECOC_CODE) SecOC_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, SECOC_APPL_DATA) versioninfo
)
{
  if (NULL_PTR != versioninfo)
  {
    versioninfo->vendorID         = SECOC_VENDOR_ID;
    versioninfo->moduleID         = SECOC_MODULE_ID;
    versioninfo->sw_major_version = SECOC_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = SECOC_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = SECOC_SW_PATCH_VERSION;
  }
}
#endif /* #if (STD_ON == SECOC_VERSION_INFO_API) */

FUNC(Std_ReturnType, SECOC_CODE) SecOC_IsValidConfig
(
  P2CONST(void, AUTOMATIC, SECOC_APPL_CONST) voidConfigPtr
)
{
  /* Assume an invalid configuration */
  Std_ReturnType retVal = E_NOT_OK;
  P2CONST(SecOC_ConfigType, AUTOMATIC, SECOC_APPL_CONST) ConfigPtr = voidConfigPtr;

  if(ConfigPtr != NULL_PTR)
  {
    /* Check if the configuration fits to the platform */
    if(TS_PlatformSigIsValid(ConfigPtr->PlatformSignature) == TRUE )
    {
      /* Validate the post build configuration against the compile time configuration */
      if(SECOC_CFG_SIGNATURE == ConfigPtr->CfgSignature)
      {
        /* Validate the post build configuration against the link time configuration */
        if(SecOC_LcfgSignature == ConfigPtr->LcfgSignature)
        {
          /* Validate the Published information in post build configuration against the
           * Published information in compile time configuration
           */
          if(SECOC_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
          {
            /* Indicate that the configuration is valid */
            retVal = E_OK;
          }
        }
      }
    }
  }

  return retVal;
}

/* !LINKSTO SWS_SecOC_Tx_00112,1 */
FUNC(Std_ReturnType, SECOC_CODE) SecOC_Transmit
(
          PduIdType                                id,
  P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if (SECOC_NUMBER_TX_PDUS > 0)
  uint16 pduIdx       = (uint16) id;

  /* !LINKSTO SWS_SecOC_Tx_00108,1 */
  if (
      (SECOC_INIT           == SecOC_InitStatus                 ) &&
      (NULL_PTR             != info                             ) &&
      (SECOC_NUMBER_TX_PDUS >  id                               ) &&
      (info->SduLength      <= SECOC_GET_TX_AUTH_PDU_LEN(pduIdx))
     )
  {
#if (STD_ON == SECOC_PDU_SECURED_AREA)
    /* !LINKSTO SWS_SecOC_Tx_00313,1 */
    if (info->SduLength >= SECOC_TX_SECAREA_MIN_AUTHDATALEN(pduIdx))
#endif /* #if (STD_ON == SECOC_PDU_SECURED_AREA) */
    {
#if (SECOC_TX_USE_MESS_LINKER == STD_ON)
      if (
          (SecOC_TxConfig[pduIdx].UseSecPduCollection == FALSE) ||
          (
           info->SduLength >= SECOC_REQUIREDBYTES(SECOC_TX_MSGLINK_MIN_AUTHDATALEN(pduIdx))
          )
         )
#endif
      {
        SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)
        if (SECOC_BUFFER_FREE == SecOC_TxCheckBufferAvailability(pduIdx))
#endif
        {
#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)
          /* Set the buffer used flag to true, to prevent reception of other authentic PDUs
           * that are using the same buffer
           */
          SECOC_TX_SET_BUFFER_LVL1_STATUS(pduIdx, SECOC_BUFFER_USED);
#endif
          /* !LINKSTO SecOC_EB_Tx_xxx06,1 */
          SecOC_TxData[pduIdx].TxReceivedDataLength = info->SduLength;
          /* !LINKSTO SWS_SecOC_Tx_00110,1 */
          TS_MemCpy(SecOC_TxData[pduIdx].TxReceivedAuthPduBuffer, info->SduDataPtr, info->SduLength);

          SecOC_TxData[pduIdx].TxTransmitEvent = SECOC_TXEVENT_TRANSMIT;

          retVal = E_OK;
        }
        SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
      }
    }

    /* !LINKSTO SecOC_EB_Tx_xxx24,1 */
#if (SECOC_TX_SYNC_PDU_PROCESSING == STD_ON)
    /* !LINKSTO SecOC_EB_Tx_xxx21,1 */
    if (TRUE == SecOC_TxConfig[pduIdx].UseTxSyncProcessing)
    {
      SecOC_Tx_ProcessPduStates(pduIdx);
    }
#endif /* #if (SECOC_TX_SYNC_PDU_PROCESSING == STD_ON) */
  }
#else
  SECOC_PARAM_UNUSED(id);
  SECOC_PARAM_UNUSED(info);
#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */

  return retVal;
}

/* !LINKSTO SWS_SecOC_Tx_00113,1 */
FUNC(Std_ReturnType, SECOC_CODE) SecOC_CancelTransmit
(
  PduIdType  id
)
{
  Std_ReturnType retVal = E_NOT_OK;
#if (SECOC_NUMBER_TX_PDUS > 0)
  uint16         pduIdx = (uint16) id;
  if ((SECOC_INIT == SecOC_InitStatus) && (SECOC_NUMBER_TX_PDUS > pduIdx))
  {
    /* !LINKSTO SWS_SecOC_Tx_00076,1, SWS_SecOC_Tx_00137,4 */
#if (STD_ON == SECOC_PDUR_SECOCCANCELTRANSMIT)
    retVal = PduR_SecOCCancelTransmit((PduIdType) SECOC_GET_TX_SEC_PDU_ID_AT_PDUR(pduIdx));
#else
    retVal = E_OK;
#endif

    SecOC_TxData[pduIdx].TxPduREvent = SECOC_TXEVENT_CANCEL;
    SecOC_TxData[pduIdx].TxTransmitEvent = SECOC_TXEVENT_NONE;

#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)
    SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
    SECOC_TX_SET_BUFFER_LVL2_STATUS(pduIdx, SECOC_BUFFER_FREE);
    SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
#endif
  }
#else
  SECOC_PARAM_UNUSED(id);
#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */
  return retVal;
}


/* !LINKSTO SWS_SecOC_Tx_00127,1 */
FUNC(Std_ReturnType, SECOC_CODE) SecOC_TriggerTransmit
(
        PduIdType                                TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if (SECOC_NUMBER_TX_PDUS > 0U)

  if (
      (NULL_PTR   != PduInfoPtr      ) &&
      (SECOC_INIT == SecOC_InitStatus)
     )
  {
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) bufferPtr = NULL_PTR;
    PduLengthType bufferLength = 0U;
    uint16 pduIdx;
    boolean idFound = FALSE;

    for (pduIdx = 0U; pduIdx < SECOC_NUMBER_TX_PDUS; pduIdx++)
    {
#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
      if (SecOC_TxConfig[pduIdx].UseSecPduCollection == TRUE)
      {
        if (TxPduId == SecOC_TxConfig[pduIdx].TxSentAuthLayerPduIdAtSecOC)
        {
          bufferPtr = SecOC_TxData[pduIdx].TxSentAuthPdu.SduDataPtr;
          bufferLength = SecOC_TxData[pduIdx].TxSentAuthPdu.SduLength;
          idFound = TRUE;
        }
        else if (TxPduId == SecOC_TxConfig[pduIdx].TxCryptoLayerPduIdAtSecOC)
        {
          bufferPtr = SecOC_TxData[pduIdx].TxCryptoPdu.SduDataPtr;
          bufferLength = SecOC_TxData[pduIdx].TxCryptoPdu.SduLength;
          idFound = TRUE;
        }
        else
        {
          /* Do nothing as the received ID is not for SecOC */
        }
      }
      else
#endif
      {
        if (TxPduId == SecOC_TxConfig[pduIdx].TxSecuredLayerPduIdAtSecOC)
        {
          bufferPtr = SecOC_TxData[pduIdx].TxSecuredPdu.SduDataPtr;
          bufferLength = SecOC_TxData[pduIdx].TxSecuredPdu.SduLength;
          idFound = TRUE;
        }
      }


      /* !LINKSTO SWS_SecOC_Tx_00150,1 */
      if (
          (idFound               == TRUE              )&&
          (SecOC_TxData[pduIdx].TxState == SECOC_TX_STATE_WAITFORCONFIRMATION ) &&
          (PduInfoPtr->SduLength >= bufferLength      )
         )
      {
        /* !LINKSTO SWS_SecOC_Tx_00068,1 */
        /* Copy the Secured I-PDU in the lower layer's buffer */
        TS_MemCpy(PduInfoPtr->SduDataPtr,
                  bufferPtr,
                  bufferLength
                 );

        /* Provide the length of the copied data */
        PduInfoPtr->SduLength = bufferLength;

        retVal = E_OK;
        break;
      }
    }
  }
#else
  SECOC_PARAM_UNUSED(TxPduId);
  SECOC_PARAM_UNUSED(PduInfoPtr);
#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */

  return retVal;
}


/* !LINKSTO SWS_SecOC_Tx_00126,1 */
FUNC(void, SECOC_CODE) SecOC_TxConfirmation
(
  PduIdType id
)
{
#if (SECOC_NUMBER_TX_PDUS > 0)
  uint16 pduIdx;

  if (SECOC_INIT == SecOC_InitStatus)
  {
    boolean idFound = FALSE;

    for (pduIdx = 0U; pduIdx < SECOC_NUMBER_TX_PDUS; pduIdx++)
    {
#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
      if (SecOC_TxConfig[pduIdx].UseSecPduCollection == TRUE)
      {
        if (id == SecOC_TxConfig[pduIdx].TxSentAuthLayerPduIdAtSecOC)
        {
          idFound = TRUE;

          if (SecOC_TxData[pduIdx].TxPduRConfirmationForCrypto == SECOC_TXEVENT_TXCONFIRMATION_OK)
          {
            /* !LINKSTO SWS_SecOC_Tx_00063,1 */
            PduR_SecOCTxConfirmation(SECOC_GET_TX_ULAUTH_PDU_ID_AT_PDUR(pduIdx));
            SecOC_TxData[pduIdx].TxPduRConfirmationForCrypto = SECOC_TXEVENT_NONE;
            SecOC_TxData[pduIdx].TxPduREvent = SECOC_TXEVENT_TXCONFIRMATION_OK;

#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)
            SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
            SECOC_TX_SET_BUFFER_LVL2_STATUS(pduIdx, SECOC_BUFFER_FREE);
            SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
#endif
          }
          else
          {
            SecOC_TxData[pduIdx].TxPduRConfirmationForAuth = SECOC_TXEVENT_TXCONFIRMATION_OK;
          }
        }
        else if (id == SecOC_TxConfig[pduIdx].TxCryptoLayerPduIdAtSecOC)
        {
          idFound = TRUE;

          if (SecOC_TxData[pduIdx].TxPduRConfirmationForAuth == SECOC_TXEVENT_TXCONFIRMATION_OK)
          {
            /* !LINKSTO SWS_SecOC_Tx_00063,1 */
            PduR_SecOCTxConfirmation(SECOC_GET_TX_ULAUTH_PDU_ID_AT_PDUR(pduIdx));
            SecOC_TxData[pduIdx].TxPduRConfirmationForAuth = SECOC_TXEVENT_NONE;
            SecOC_TxData[pduIdx].TxPduREvent = SECOC_TXEVENT_TXCONFIRMATION_OK;

#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)
            SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
            SECOC_TX_SET_BUFFER_LVL2_STATUS(pduIdx, SECOC_BUFFER_FREE);
            SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
#endif
          }
          else
          {
            SecOC_TxData[pduIdx].TxPduRConfirmationForCrypto = SECOC_TXEVENT_TXCONFIRMATION_OK;
          }
        }
        else
        {
          /* Do nothing as the received ID is not related to this secured PDU collection */
        }
      }
      else
#endif /* #if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON) */
      {
        if (id == SecOC_TxConfig[pduIdx].TxSecuredLayerPduIdAtSecOC)
        {
          idFound = TRUE;

          /* !LINKSTO SWS_SecOC_Tx_00063,1, SWS_SecOC_Tx_00137,4 */
          PduR_SecOCTxConfirmation(SECOC_GET_TX_ULAUTH_PDU_ID_AT_PDUR(pduIdx));
          SecOC_TxData[pduIdx].TxPduREvent = SECOC_TXEVENT_TXCONFIRMATION_OK;

#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)
          SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
          SECOC_TX_SET_BUFFER_LVL2_STATUS(pduIdx, SECOC_BUFFER_FREE);
          SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
#endif
        }
      }

      if (idFound == TRUE)
      {
        break;
      }
    }
  }
#else
  SECOC_PARAM_UNUSED(id);
#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */
}


#if (STD_ON == SECOC_TX_USETP)
/* !LINKSTO SWS_SecOC_Tx_00129,1, SWS_SecOC_RxTx_00103,2 */
FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_CopyTxData
(
        PduIdType                                  id,
  P2VAR(PduInfoType,   AUTOMATIC, SECOC_APPL_DATA) info,
  P2VAR(RetryInfoType, AUTOMATIC, SECOC_APPL_DATA) retry,
  P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) availableDataPtr
)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;

#if (SECOC_NUMBER_TX_PDUS > 0)

  if ((SECOC_INIT == SecOC_InitStatus) && (NULL_PTR != info) && (NULL_PTR != availableDataPtr))
  {
    P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) copiedDataLengthPtr = NULL_PTR;
    P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) bufferPtr = NULL_PTR;
    PduLengthType totalPduLength = 0U;
    uint16 pduIdx;
    boolean idFound = FALSE;


    for (pduIdx = 0U; pduIdx < SECOC_NUMBER_TX_PDUS; pduIdx++)
    {
#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
      if (SecOC_TxConfig[pduIdx].UseSecPduCollection == TRUE)
      {
        if (id == SecOC_TxConfig[pduIdx].TxSentAuthLayerPduIdAtSecOC)
        {
          copiedDataLengthPtr = &SecOC_TxData[pduIdx].CopiedDataFromAuthLength;
          totalPduLength = SecOC_TxData[pduIdx].TxSentAuthPdu.SduLength;
          bufferPtr = SecOC_TxData[pduIdx].TxSentAuthPdu.SduDataPtr;
          idFound = TRUE;
        }
        else if (id == SecOC_TxConfig[pduIdx].TxCryptoLayerPduIdAtSecOC)
        {
          copiedDataLengthPtr = &SecOC_TxData[pduIdx].CopiedDataFromCryptoLength;
          totalPduLength = SecOC_TxData[pduIdx].TxCryptoPdu.SduLength;
          bufferPtr = SecOC_TxData[pduIdx].TxCryptoPdu.SduDataPtr;
          idFound = TRUE;
        }
        else
        {
          /* Do nothing as the received ID is not for SecOC */
        }
      }
      else
#endif
      {
        if (id == SecOC_TxConfig[pduIdx].TxSecuredLayerPduIdAtSecOC)
        {
          copiedDataLengthPtr = &SecOC_TxData[pduIdx].CopiedDataFromSecLength;
          totalPduLength = SecOC_TxData[pduIdx].TxSecuredPdu.SduLength;
          bufferPtr = SecOC_TxData[pduIdx].TxSecuredPdu.SduDataPtr;
          idFound = TRUE;
        }
      }

      if (idFound == TRUE)
      {
        if (SECOC_TX_STATE_WAITFORCONFIRMATION == SecOC_TxData[pduIdx].TxState)
        {
          PduLengthType remainingBytes;

          retVal = BUFREQ_OK;

          if ( (NULL_PTR != retry) && (TP_DATARETRY  == retry->TpDataState))
          {
            /* copy previously copied data again. */

            if (retry->TxTpDataCnt < *copiedDataLengthPtr)
            {
              *copiedDataLengthPtr -= retry->TxTpDataCnt;
            }
            else
            {
              retVal = BUFREQ_E_NOT_OK;
            }
          }

          remainingBytes = totalPduLength - *copiedDataLengthPtr;

          if (remainingBytes < info->SduLength)
          {
            /* !LINKSTO SWS_SecOC_Tx_00129,1 */
            retVal = BUFREQ_E_BUSY;
            *availableDataPtr = remainingBytes;
          }

          if (BUFREQ_OK == retVal)
          {
            /* !LINKSTO SWS_SecOC_Tx_00073,1, SWS_SecOC_Tx_00129,1 */
            if ((info->SduLength > 0U) && (info->SduDataPtr == NULL_PTR))
            {
              retVal = BUFREQ_E_NOT_OK;
            }
            else
            {
              TS_MemCpy
              (
                  info->SduDataPtr,
                  &bufferPtr[*copiedDataLengthPtr],
                  info->SduLength
              );
              *copiedDataLengthPtr += info->SduLength;
              *availableDataPtr = (remainingBytes - info->SduLength);
            }
          }
        }
        break;
      }
    }
  }
#else /* #if (SECOC_NUMBER_TX_PDUS > 0) */
  SECOC_PARAM_UNUSED(id);
  SECOC_PARAM_UNUSED(info);
  SECOC_PARAM_UNUSED(retry);
  SECOC_PARAM_UNUSED(availableDataPtr);
#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */
  return retVal;
}


/* !LINKSTO SWS_SecOC_Tx_00152,1 */
FUNC(void, SECOC_CODE) SecOC_TpTxConfirmation
(
  PduIdType      id,
  Std_ReturnType result
)
{
#if (SECOC_NUMBER_TX_PDUS > 0)
  uint16 pduIdx;

  if (SECOC_INIT == SecOC_InitStatus)
  {
    boolean idFound = FALSE;

    for (pduIdx = 0U; pduIdx < SECOC_NUMBER_TX_PDUS; pduIdx++)
    {
#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
      if (SecOC_TxConfig[pduIdx].UseSecPduCollection == TRUE)
      {
        if (id == SecOC_TxConfig[pduIdx].TxSentAuthLayerPduIdAtSecOC)
        {
          idFound = TRUE;

          /* !LINKSTO SWS_SecOC_Tx_00077,1 */
          if (E_OK == result)
          {
            SecOC_TxData[pduIdx].TxPduRConfirmationForAuth = SECOC_TXEVENT_TXCONFIRMATION_OK;
          }
          else
          {
            SecOC_TxData[pduIdx].TxPduRConfirmationForAuth = SECOC_TXEVENT_TXCONFIRMATION_NOT_OK;
          }
        }
        else if (id == SecOC_TxConfig[pduIdx].TxCryptoLayerPduIdAtSecOC)
        {
          idFound = TRUE;

          /* !LINKSTO SWS_SecOC_Tx_00077,1 */
          if (E_OK == result)
          {
            SecOC_TxData[pduIdx].TxPduRConfirmationForCrypto = SECOC_TXEVENT_TXCONFIRMATION_OK;
          }
          else
          {
            SecOC_TxData[pduIdx].TxPduRConfirmationForCrypto = SECOC_TXEVENT_TXCONFIRMATION_NOT_OK;
          }
        }
        else
        {
          /* Do nothing as the received ID is not for SecOC */
        }

        if (TRUE == idFound)
        {
          if ((SecOC_TxData[pduIdx].TxPduRConfirmationForAuth != SECOC_TXEVENT_NONE)  &&
              (SecOC_TxData[pduIdx].TxPduRConfirmationForCrypto != SECOC_TXEVENT_NONE)
             )
          {
            if ((SecOC_TxData[pduIdx].TxPduRConfirmationForAuth == SECOC_TXEVENT_TXCONFIRMATION_OK)  &&
                (SecOC_TxData[pduIdx].TxPduRConfirmationForCrypto == SECOC_TXEVENT_TXCONFIRMATION_OK)
               )
            {
              /* !LINKSTO SWS_SecOC_Tx_00074,2, SWS_SecOC_Tx_00137,4 */
              PduR_SecOCTpTxConfirmation
                (SECOC_GET_TX_ULAUTH_PDU_ID_AT_PDUR(pduIdx), E_OK);
            }
            else
            {
              /* !LINKSTO SWS_SecOC_Tx_00074,2, SWS_SecOC_Tx_00137,4 */
              PduR_SecOCTpTxConfirmation
                (SECOC_GET_TX_ULAUTH_PDU_ID_AT_PDUR(pduIdx), E_NOT_OK);
            }
            SecOC_TxData[pduIdx].TxPduRConfirmationForAuth = SECOC_TXEVENT_NONE;
            SecOC_TxData[pduIdx].TxPduRConfirmationForCrypto = SECOC_TXEVENT_NONE;
            SecOC_TxData[pduIdx].TxPduREvent = SECOC_TXEVENT_TXCONFIRMATION_OK;

#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)
            SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
            SECOC_TX_SET_BUFFER_LVL2_STATUS(pduIdx, SECOC_BUFFER_FREE);
            SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
#endif
          }
        }
      }
      else
#endif /* #if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON) */
      {
        if (id == SecOC_TxConfig[pduIdx].TxSecuredLayerPduIdAtSecOC)
        {
          idFound = TRUE;

          /* !LINKSTO SWS_SecOC_Tx_00077,1, SWS_SecOC_Tx_00074,2, SWS_SecOC_Tx_00137,4 */
          PduR_SecOCTpTxConfirmation
            (SECOC_GET_TX_ULAUTH_PDU_ID_AT_PDUR(pduIdx), result);

          SecOC_TxData[pduIdx].TxPduREvent = SECOC_TXEVENT_TXCONFIRMATION_OK;

#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)
          SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
          SECOC_TX_SET_BUFFER_LVL2_STATUS(pduIdx, SECOC_BUFFER_FREE);
          SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
#endif
        }
      }

      if (idFound == TRUE)
      {

        break;
      }
    }
  }
#else
  SECOC_PARAM_UNUSED(id);
  SECOC_PARAM_UNUSED(result);
#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */
}
#endif /* (STD_ON == SECOC_TX_USETP) */

#if (STD_ON == SECOC_RX_USETP)
/* !LINKSTO SWS_SecOC_Rx_00130,1, SecOC_EB_Rx_00001,1 */
FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_StartOfReception
(
        PduIdType                                  id,
#if (STD_OFF == SECOC_ASR403_API)
  P2VAR(PduInfoType,   AUTOMATIC, SECOC_APPL_DATA) info,
#endif
        PduLengthType                              TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) bufferSizePtr
)
{
  BufReq_ReturnType bufReqRetVal = BUFREQ_E_NOT_OK;

#if (SECOC_NUMBER_RX_PDUS > 0)
  /* !LINKSTO SWS_SecOC_Rx_00109,1 */
  if (
      (SECOC_INIT == SecOC_InitStatus) &&
      (NULL_PTR   != bufferSizePtr   )
     )
  {
    uint16 pduIdx = (uint16) id;
    uint8 pduType = SECOC_UNKNOWN_PDU;


    if (TRUE == SecOC_RxCheckForPduIdx(&pduIdx, &pduType))
    {
      SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();

      /* !LINKSTO SWS_SecOC_Rx_00214,1, SWS_SecOC_Rx_00215,1 */
      if (
          (FALSE                     == SecOC_RxData[pduIdx].RxPduInProcess     ) ||
          (SECOC_REPLACE_CURRENT_PDU == SecOC_RxConfig[pduIdx].ReceptionStrategy)
         )
      {
        /* !LINKSTO SWS_SecOC_00181,1 */
        bufReqRetVal = SecOC_RxCheckIncomingPdu(TpSduLength, pduIdx, pduType);

        if (BUFREQ_OK == bufReqRetVal)
        {
          P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) receivedDataLenPtr;
#if (STD_OFF == SECOC_ASR403_API)
          P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) localBufferPtr;
#endif
          PduLengthType localBufferSize;
          PduLengthType inputBufferSize = *bufferSizePtr;

#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
          if (pduType == SECOC_AUTH_PDU)
          {
#if (STD_OFF == SECOC_ASR403_API)
            localBufferPtr = SecOC_RxData[pduIdx].RxReceivedAuthPduBuffer;
#endif
            localBufferSize = SECOC_GET_RX_AUTH_PDU_LEN(pduIdx);
            receivedDataLenPtr = &SecOC_RxData[pduIdx].ReceivedTpAuthDataLength;
          }
          else if (pduType == SECOC_CRYPTO_PDU)
          {
#if (STD_OFF == SECOC_ASR403_API)
            localBufferPtr = SecOC_RxData[pduIdx].RxCryptoPduBuffer;
#endif
            localBufferSize = SECOC_GET_RX_CRYPTO_PDU_LEN(pduIdx);
            receivedDataLenPtr = &SecOC_RxData[pduIdx].ReceivedTpCryptoDataLength;
          }
          else
#endif /* #if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON) */
          {
#if (STD_OFF == SECOC_ASR403_API)
            localBufferPtr = SecOC_RxData[pduIdx].RxReceivedSecuredPduBuffer;
#endif
            localBufferSize = SECOC_GET_RX_SECURED_PDU_LEN(pduIdx);
            receivedDataLenPtr = &SecOC_RxData[pduIdx].ReceivedTpSecuredDataLength;
          }


#if (STD_OFF == SECOC_ASR403_API)
          if (NULL_PTR != info)
          {
            if (info->SduLength <= localBufferSize)
            {
              *receivedDataLenPtr = info->SduLength;

              /* secured PDU */

              /* !LINKSTO SWS_SecOC_Rx_00111,1 */
              TS_MemCpy(localBufferPtr, info->SduDataPtr, info->SduLength);
            }
            else
            {
              bufReqRetVal = BUFREQ_E_OVFL;
            }
          }
          else
#endif /* #if (STD_OFF == SECOC_ASR403_API) */
          {
            *receivedDataLenPtr = 0U;
          }

          inputBufferSize =
              localBufferSize - *receivedDataLenPtr;

#if (STD_OFF == SECOC_ASR403_API)
          if (bufReqRetVal == BUFREQ_OK)
#endif
          {
            PduLengthType bufferSizeUpperLayer = 0U;
            PduLengthType reqBufferSizeUpperLayer;

            if (
                (TRUE == SecOC_RxData[pduIdx].UpperLayerCallFlag) &&
                ((SECOC_AUTH_PDU == pduType) || (SECOC_SECURED_PDU == pduType))
               )
            {
              PduR_SecOCTpRxIndication(SECOC_GET_RX_AUTH_PDU_ID_AT_PDUR(pduIdx), E_NOT_OK);
            }
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
            if (pduType == SECOC_AUTH_PDU)
            {
              reqBufferSizeUpperLayer = TpSduLength;
            }
            else if (pduType == SECOC_CRYPTO_PDU)
            {
              if (
                  ( SecOC_RxData[pduIdx].RxPduRIndicationForAuth == SECOC_RXEVENT_RXINDICATION   ) &&
                  ( SecOC_RxData[pduIdx].RxPduRIndicationForCrypto == SECOC_RXEVENT_RXINDICATION )
                 )
              {
                reqBufferSizeUpperLayer = SecOC_RxData[pduIdx].ReceivedTpAuthDataLength;
              }
              else
              {
                reqBufferSizeUpperLayer = 0U;
              }
            }
            else
#endif
            {
              reqBufferSizeUpperLayer =
              TpSduLength - (uint16)(SECOC_REQUIREDBYTES(SECOC_RX_AUTHENTICATION_INFO_LEN(pduIdx)));
            }

            /* !LINKSTO SWS_SecOC_Rx_00082,2 */
            if (reqBufferSizeUpperLayer != 0U)
            {
              BufReq_ReturnType upperBufReqRetVal = PduR_SecOCStartOfReception
                                                    (
                                                      SECOC_GET_RX_AUTH_PDU_ID_AT_PDUR(pduIdx),
#if (STD_OFF == SECOC_ASR403_API)
                                                      NULL_PTR,
#endif
                                                      reqBufferSizeUpperLayer,
                                                      &bufferSizeUpperLayer
                                                    );

              if (BUFREQ_OK == upperBufReqRetVal)
              {
                *bufferSizePtr = inputBufferSize;
                SecOC_RxData[pduIdx].UpperLayerCallFlag = TRUE;
                SecOC_RxData[pduIdx].RxPduREvent = SECOC_RXEVENT_STARTRECEPTION;
              }
              else
              {
                *receivedDataLenPtr = 0U;
                bufReqRetVal = BUFREQ_E_NOT_OK;
              }
            }
          }
        }
      }
      else
      {
        /* !LINKSTO SWS_SecOC_Rx_00215,1 */
        bufReqRetVal = BUFREQ_E_OVFL;
      }

      SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();
    }
  }

#else /* #if (SECOC_NUMBER_RX_PDUS > 0) */
  SECOC_PARAM_UNUSED(id);
#if (STD_OFF == SECOC_ASR403_API)
  SECOC_PARAM_UNUSED(info);
#endif
  SECOC_PARAM_UNUSED(TpSduLength);
  SECOC_PARAM_UNUSED(bufferSizePtr);
#endif /* #if (SECOC_NUMBER_RX_PDUS > 0) #else */


  return bufReqRetVal;
}

/* !LINKSTO SWS_SecOC_Rx_00128,1, SWS_SecOC_RxTx_00103,2 */
FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_CopyRxData
(
          PduIdType                                  id,
  P2CONST(PduInfoType,   AUTOMATIC, SECOC_APPL_DATA) info,
    P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) bufferSizePtr
)
{
  BufReq_ReturnType bufReqRetVal = BUFREQ_E_NOT_OK;

#if (SECOC_NUMBER_RX_PDUS > 0)
  uint16 pduIdx = (uint16) id;

  /* !LINKSTO SWS_SecOC_Rx_00109,1 */
  if (
      (SECOC_INIT == SecOC_InitStatus) &&
      (NULL_PTR   != info            ) &&
      (NULL_PTR   != bufferSizePtr   )
     )
  {
    uint8 pduType = SECOC_UNKNOWN_PDU;
    boolean idFound = FALSE;

    idFound = SecOC_RxCheckForPduIdx(&pduIdx, &pduType);


    if (
        (TRUE                         == idFound                         ) &&
        (SECOC_RXEVENT_STARTRECEPTION == SecOC_RxData[pduIdx].RxPduREvent)
       )
    {
      SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();

      /* CHECK: NOPARSE */
      /* Defensive programming: if this function is called, the SecOC_RxData[pduIdx].RxPduInProcess
       * will be TRUE when PduR event is SECOC_RXEVENT_STARTRECEPTION or
       * when is FALSE the overflow strategy is set to REPLACE.
       */
      if (
          (FALSE                     == SecOC_RxData[pduIdx].RxPduInProcess     ) ||
          (SECOC_REPLACE_CURRENT_PDU == SecOC_RxConfig[pduIdx].ReceptionStrategy)
         )
      /* CHECK: PARSE */
      {
        P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) receivedDataLenPtr;
        P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) localBufferPtr;
        PduLengthType localBufferSize = 0U;

#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
        if (pduType == SECOC_AUTH_PDU)
        {
          localBufferPtr = SecOC_RxData[pduIdx].RxReceivedAuthPduBuffer;
          localBufferSize = SECOC_GET_RX_AUTH_PDU_LEN(pduIdx);
          receivedDataLenPtr = &SecOC_RxData[pduIdx].ReceivedTpAuthDataLength;
        }
        else if (pduType == SECOC_CRYPTO_PDU)
        {
          localBufferPtr = SecOC_RxData[pduIdx].RxCryptoPduBuffer;
          localBufferSize = SECOC_GET_RX_CRYPTO_PDU_LEN(pduIdx);
          receivedDataLenPtr = &SecOC_RxData[pduIdx].ReceivedTpCryptoDataLength;
        }
        else
#endif
        {
          localBufferPtr = SecOC_RxData[pduIdx].RxReceivedSecuredPduBuffer;
          localBufferSize = SECOC_GET_RX_SECURED_PDU_LEN(pduIdx);
          receivedDataLenPtr = &SecOC_RxData[pduIdx].ReceivedTpSecuredDataLength;
        }

        if (localBufferSize >= (*receivedDataLenPtr + info->SduLength))
        {
          /* !LINKSTO SWS_SecOC_Rx_00128,1 */
          bufReqRetVal = BUFREQ_OK;

          /* !LINKSTO SWS_SecOC_Rx_00111,1, SWS_SecOC_Rx_00083,1, SWS_SecOC_Rx_00205,1 */
          TS_MemCpy
          (
            &localBufferPtr[*receivedDataLenPtr],
            info->SduDataPtr,
            info->SduLength
          );

          /* !LINKSTO SecOC_EB_Rx_xxx03,1 */
          *receivedDataLenPtr += info->SduLength;

        }
        else
        {
          bufReqRetVal = BUFREQ_E_OVFL;
        }
        /* !LINKSTO SWS_SecOC_Rx_00128,1 */
        *bufferSizePtr = localBufferSize - *receivedDataLenPtr;
      }
      else
      {
        /* we need to wait until the buffers a free */
        *bufferSizePtr = 0U;
      }

      SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();
    }
  }
#else /* #if (SECOC_NUMBER_RX_PDUS > 0) */
  SECOC_PARAM_UNUSED(id);
  SECOC_PARAM_UNUSED(info);
  SECOC_PARAM_UNUSED(bufferSizePtr);
#endif /* #if (SECOC_NUMBER_RX_PDUS > 0) #else */

  return bufReqRetVal;
}


/* !LINKSTO SWS_SecOC_Rx_00125,1 */
FUNC(void, SECOC_CODE) SecOC_TpRxIndication
(
  PduIdType      id,
  Std_ReturnType result
)
{
#if (SECOC_NUMBER_RX_PDUS > 0)

  /* !LINKSTO SWS_SecOC_Rx_00084,1 */
  if (SECOC_INIT == SecOC_InitStatus)
  {
    uint16 pduIdx = (uint16) id;
    uint8 pduType = SECOC_UNKNOWN_PDU;
    boolean idFound = FALSE;

    idFound = SecOC_RxCheckForPduIdx(&pduIdx, &pduType);

    if (
        (SECOC_RXEVENT_STARTRECEPTION  == SecOC_RxData[pduIdx].RxPduREvent) &&
        (idFound                       == TRUE                            )
       )
    {
      SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();

      if (result == E_OK)
      {
        if (pduType == SECOC_SECURED_PDU)
        {
          SecOC_RxData[pduIdx].RxPduREvent = SECOC_RXEVENT_RXINDICATION;
        }
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
        else
        {
#if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_ON)
          if(
              (SECOC_OVERRIDESTATUS_SKIP_UNTIL_NOTICE == SecOC_RxData[pduIdx].VerifyStatusOverride)
              &&
              (pduType == SECOC_AUTH_PDU)
            )
          {
            SecOC_RxData[pduIdx].RxPduREvent = SECOC_RXEVENT_RXINDICATION;
            /* Clear both events to ensure a clean start for the next receiving procedure */
            SecOC_RxData[pduIdx].RxPduRIndicationForAuth   = SECOC_RXEVENT_NONE;
            SecOC_RxData[pduIdx].RxPduRIndicationForCrypto = SECOC_RXEVENT_NONE;
          }
          else
#endif
          {
            if (pduType == SECOC_AUTH_PDU)
            {
              SecOC_RxData[pduIdx].RxPduRIndicationForAuth = SECOC_RXEVENT_RXINDICATION;
            }
            else
            {
              SecOC_RxData[pduIdx].RxPduRIndicationForCrypto = SECOC_RXEVENT_RXINDICATION;
            }

            if (
                ( SecOC_RxData[pduIdx].RxPduRIndicationForAuth == SECOC_RXEVENT_RXINDICATION   ) &&
                ( SecOC_RxData[pduIdx].RxPduRIndicationForCrypto == SECOC_RXEVENT_RXINDICATION )
               )
            {
#if (SECOC_RX_USE_MESS_LINKER == STD_ON)
              if (SecOC_RxConfig[pduIdx].RxMessageLinkLength != 0U)
              {
                /* Check if the message link from the cryptographic Pdu is matching the corresponding
                   part from the received authentic Pdu */
                /* !LINKSTO SWS_SecOC_Rx_00211,1 */
                if (
                     E_OK == SecOC_RxCheckMessageLink(
                                                       SecOC_RxData[pduIdx].RxReceivedAuthPduBuffer,
                                                       SecOC_RxConfig[pduIdx].RxMessageLinkPos,
                                                       SecOC_RxData[pduIdx].RxCryptoPduBuffer,
                                                       SECOC_RX_AUTHENTICATION_INFO_LEN(pduIdx),
                                                       SecOC_RxConfig[pduIdx].RxMessageLinkLength
                                                     )
                   )
                {
                  /* Set the PduR event to indicate that both PDUs were received and verification
                   * procedure can start
                   */
                  SecOC_RxData[pduIdx].RxPduREvent = SECOC_RXEVENT_RXINDICATION;

                  /* Clear both events to ensure a clean start for the next receiving procedure */
                  SecOC_RxData[pduIdx].RxPduRIndicationForAuth   = SECOC_RXEVENT_NONE;
                  SecOC_RxData[pduIdx].RxPduRIndicationForCrypto = SECOC_RXEVENT_NONE;
                }
                else
                {
                  /* Clear the main event but leave the indications events to ensure that if the
                     cryptographic or authentic PDU is received again the verification procedure is
                     immediately restarted
                   */
                  SecOC_RxData[pduIdx].RxPduREvent = SECOC_RXEVENT_NONE;

                  SecOC_RxData[pduIdx].UpperLayerCallFlag = FALSE;

                  PduR_SecOCTpRxIndication(SECOC_GET_RX_AUTH_PDU_ID_AT_PDUR(pduIdx), E_NOT_OK);
                }
              }
              else
#endif /* #if (SECOC_RX_USE_MESS_LINKER == STD_ON) */
              {
                /* Set the PduR event to indicate that both PDUs were received and verification
                 * procedure can start
                 */
                SecOC_RxData[pduIdx].RxPduREvent = SECOC_RXEVENT_RXINDICATION;

                /* Clear both events to ensure a clean start for the next receiving procedure */
                SecOC_RxData[pduIdx].RxPduRIndicationForAuth = SECOC_RXEVENT_NONE;
                SecOC_RxData[pduIdx].RxPduRIndicationForCrypto = SECOC_RXEVENT_NONE;
              }
            }
          }
        }
#endif /* #if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON) */
      }
      else
      {
        if ((pduType == SECOC_AUTH_PDU) || (pduType == SECOC_SECURED_PDU))
        {
          /* !LINKSTO SWS_SecOC_Rx_00089,1, SWS_SecOC_Rx_00087_3,1 */
          SecOC_RxData[pduIdx].RxPduREvent = SECOC_RXEVENT_NONE;

          SecOC_RxData[pduIdx].UpperLayerCallFlag = FALSE;

          PduR_SecOCTpRxIndication(SECOC_GET_RX_AUTH_PDU_ID_AT_PDUR(pduIdx), E_NOT_OK);
        }
      }

      SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();

/* !LINKSTO SecOC_EB_Rx_xxx18,1 */
#if (SECOC_RX_SYNC_PDU_PROCESSING == STD_ON)
      /* !LINKSTO SecOC_EB_Rx_xxx15,1 */
      if (TRUE == SecOC_RxConfig[pduIdx].UseRxSyncProcessing)
      {
        SecOC_Rx_ProcessPduStates(pduIdx);
      }
#endif /* #if (SECOC_RX_SYNC_PDU_PROCESSING == STD_ON) */
    }
  }
#else
  SECOC_PARAM_UNUSED(id);
  SECOC_PARAM_UNUSED(result);
#endif /* #if (SECOC_NUMBER_RX_PDUS > 0) */
}
#endif /* #if (STD_ON == SECOC_RX_USETP) */


/* !LINKSTO SWS_SecOC_Rx_00124,1, SecOC_EB_Rx_00002,1 */
FUNC(void, SECOC_CODE) SecOC_RxIndication
(
          PduIdType                                id,
#if (STD_ON == SECOC_ASR403_API)
    P2VAR(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info
#else
  P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info
#endif
)
{
#if (SECOC_NUMBER_RX_PDUS > 0)

  if (
      (SECOC_INIT == SecOC_InitStatus) &&
      (NULL_PTR   != info            )
     )
  {
    uint16 pduIdx = (uint16) id;
    uint8 pduType = SECOC_UNKNOWN_PDU;

    if (TRUE  == SecOC_RxCheckForPduIdx(&pduIdx, &pduType))
    {
      SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();

#if (STD_ON == SECOC_RX_USE_SAME_BUFFER)
      if (TRUE == SecOC_RxCheckBufferAvailability(pduIdx))
#endif
      {
        /* !LINKSTO SWS_SecOC_Rx_00214,1, SWS_SecOC_Rx_00215,1 */
        if (
            (FALSE                     == SecOC_RxData[pduIdx].RxPduInProcess     ) ||
            (SECOC_REPLACE_CURRENT_PDU == SecOC_RxConfig[pduIdx].ReceptionStrategy)
           )
        {
          BufReq_ReturnType bufReqRetVal;

          /* Check if the incoming PDU has a valid length */
          bufReqRetVal = SecOC_RxCheckIncomingPdu(info->SduLength, pduIdx, pduType);

          if (bufReqRetVal == BUFREQ_OK)
          {
            P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) localBufferPtr = NULL_PTR;
#if (STD_ON == SECOC_RX_USE_SAME_BUFFER)
            if (SecOC_RxData[pduIdx].RxBufferUsed != NULL_PTR)
            {
              *SecOC_RxData[pduIdx].RxBufferUsed = TRUE;
            }
#endif
            if (pduType == SECOC_SECURED_PDU)
            {
              SecOC_RxData[pduIdx].RxPduREvent = SECOC_RXEVENT_RXINDICATION;
              /* Store internally the PDU length for later processing */
              SecOC_RxData[pduIdx].ReceivedTpSecuredDataLength = info->SduLength;
              /* Set the proper buffer to store the incoming PDU */
              localBufferPtr = SecOC_RxData[pduIdx].RxReceivedSecuredPduBuffer;
            }
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
            else if (pduType == SECOC_AUTH_PDU)
            {
              SecOC_RxData[pduIdx].RxPduRIndicationForAuth = SECOC_RXEVENT_RXINDICATION;
              /* Store internally the PDU length for later processing */
              SecOC_RxData[pduIdx].ReceivedTpAuthDataLength = info->SduLength;
              /* Set the proper buffer to store the incoming PDU */
              localBufferPtr = SecOC_RxData[pduIdx].RxReceivedAuthPduBuffer;
            }
            else
            {
              SecOC_RxData[pduIdx].RxPduRIndicationForCrypto = SECOC_RXEVENT_RXINDICATION;
              /* Store internally the PDU length for later processing */
              SecOC_RxData[pduIdx].ReceivedTpCryptoDataLength = info->SduLength;
              /* Set the proper buffer to store the incoming PDU */
              localBufferPtr = SecOC_RxData[pduIdx].RxCryptoPduBuffer;
            }
#endif /* #if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON) */

            /* !LINKSTO SWS_SecOC_Rx_00111,1, SWS_SecOC_Rx_00078,1, SWS_SecOC_Rx_00205,1 */
            TS_MemCpy(localBufferPtr, info->SduDataPtr, info->SduLength);

#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
#if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_ON)
            if(
                (SECOC_OVERRIDESTATUS_SKIP_UNTIL_NOTICE == SecOC_RxData[pduIdx].VerifyStatusOverride)
                &&
                (pduType == SECOC_AUTH_PDU)
              )
            {
              SecOC_RxData[pduIdx].RxPduREvent = SECOC_RXEVENT_RXINDICATION;
              /* Clear both events to ensure a clean start for the next receiving procedure */
              SecOC_RxData[pduIdx].RxPduRIndicationForAuth = SECOC_RXEVENT_NONE;
              SecOC_RxData[pduIdx].RxPduRIndicationForCrypto = SECOC_RXEVENT_NONE;
            }
            else
#endif
            {
              if (
                    ( SecOC_RxData[pduIdx].RxPduRIndicationForAuth == SECOC_RXEVENT_RXINDICATION   ) &&
                    ( SecOC_RxData[pduIdx].RxPduRIndicationForCrypto == SECOC_RXEVENT_RXINDICATION )
                 )
              {
#if (SECOC_RX_USE_MESS_LINKER == STD_ON)
                if (SecOC_RxConfig[pduIdx].RxMessageLinkLength != 0U)
                {
                  /* Check if the message link from the cryptographic Pdu is matching the corresponding
                     part from the received authentic Pdu */
                  /* !LINKSTO SWS_SecOC_Rx_00211,1 */
                  if (
                       E_OK == SecOC_RxCheckMessageLink(
                                                         SecOC_RxData[pduIdx].RxReceivedAuthPduBuffer,
                                                         SecOC_RxConfig[pduIdx].RxMessageLinkPos,
                                                         SecOC_RxData[pduIdx].RxCryptoPduBuffer,
                                                         SECOC_RX_AUTHENTICATION_INFO_LEN(pduIdx),
                                                         SecOC_RxConfig[pduIdx].RxMessageLinkLength
                                                       )
                     )
                  {
                    /* Set the PduR event to indicate that both PDUs were received and verification
                     * procedure can start
                     */
                    SecOC_RxData[pduIdx].RxPduREvent = SECOC_RXEVENT_RXINDICATION;

                    /* Clear both events to ensure a clean start for the next receiving procedure */
                    SecOC_RxData[pduIdx].RxPduRIndicationForAuth = SECOC_RXEVENT_NONE;
                    SecOC_RxData[pduIdx].RxPduRIndicationForCrypto = SECOC_RXEVENT_NONE;
                  }
#if (STD_ON == SECOC_RX_USE_SAME_BUFFER)
                  else
                  {
                    if (SecOC_RxData[pduIdx].RxBufferUsed != NULL_PTR)
                    {
                      *SecOC_RxData[pduIdx].RxBufferUsed = FALSE;
                    }
                  }
#endif
                }
                else
#endif /* #if (SECOC_RX_USE_MESS_LINKER == STD_ON) */
                {
                  /* Set the PduR event to indicate that both PDUs were received and verification
                   * procedure can start
                   */
                  SecOC_RxData[pduIdx].RxPduREvent = SECOC_RXEVENT_RXINDICATION;

                  /* Clear both events to ensure a clean start for the next receiving procedure */
                  SecOC_RxData[pduIdx].RxPduRIndicationForAuth   = SECOC_RXEVENT_NONE;
                  SecOC_RxData[pduIdx].RxPduRIndicationForCrypto = SECOC_RXEVENT_NONE;
                }
              }
            }
#endif /* #if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON) */
          }
        }
      }

      SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();

/* !LINKSTO SecOC_EB_Rx_xxx18,1 */
#if (SECOC_RX_SYNC_PDU_PROCESSING == STD_ON)
      /* !LINKSTO SecOC_EB_Rx_xxx15,1 */
      if (TRUE == SecOC_RxConfig[pduIdx].UseRxSyncProcessing)
      {
        SecOC_Rx_ProcessPduStates(pduIdx);
      }
#endif /* #if (SECOC_RX_SYNC_PDU_PROCESSING == STD_ON) */
    }
  }
#else
  SECOC_PARAM_UNUSED(id);
  SECOC_PARAM_UNUSED(info);
#endif /* #if (SECOC_NUMBER_RX_PDUS > 0) */
}


/* !LINKSTO SWS_SecOC_00171,1 */
FUNC(void, SECOC_CODE) SecOC_MainFunctionRx
(
  void
)
{
#if (SECOC_NUMBER_RX_PDUS > 0)
  /* !LINKSTO SWS_SecOC_00172,1 */
  if (SECOC_INIT == SecOC_InitStatus)
  {
    uint16 pduIdx;

    /* !LINKSTO SWS_SecOC_00174,1 */
    for (pduIdx = 0U; SECOC_NUMBER_RX_PDUS > pduIdx; pduIdx++)
    {
/* !LINKSTO SecOC_EB_Rx_xxx18,1 */
#if (SECOC_RX_SYNC_PDU_PROCESSING == STD_ON)
      /* !LINKSTO SecOC_EB_Rx_xxx17,1 */
      if (FALSE == SecOC_RxConfig[pduIdx].UseRxSyncProcessing)
#endif /* #if (SECOC_RX_SYNC_PDU_PROCESSING == STD_ON) */
      {
        /* !LINKSTO SecOC_EB_Rx_xxx01,1 */
        SecOC_Rx_ProcessPduStates(pduIdx);
      }
    }
  }
#endif /* #if (SECOC_NUMBER_RX_PDUS > 0) */
}

/* !LINKSTO SWS_SecOC_00176,1 */
FUNC(void, SECOC_CODE) SecOC_MainFunctionTx
(
  void
)
{
#if (SECOC_NUMBER_TX_PDUS > 0)
  /* !LINKSTO SWS_SecOC_00177,1 */
  if (SECOC_INIT == SecOC_InitStatus)
  {
    uint16 pduIdx;

    /* !LINKSTO SWS_SecOC_00179,1 */
    for (pduIdx = 0U; SECOC_NUMBER_TX_PDUS > pduIdx; pduIdx++)
    {
/* !LINKSTO SecOC_EB_Tx_xxx24,1 */
#if (SECOC_TX_SYNC_PDU_PROCESSING == STD_ON)
      /* !LINKSTO SecOC_EB_Tx_xxx23,2 */
      if (
          (SecOC_TxConfig[pduIdx].UseTxSyncProcessing == FALSE                             ) ||
          (SecOC_TxData[pduIdx].TxState               == SECOC_TX_STATE_WAITFORCONFIRMATION)
         )
#endif /* #if (SECOC_TX_SYNC_PDU_PROCESSING == STD_ON) */
      {
        /* !LINKSTO SecOC_EB_Tx_xxx01,1 */
        SecOC_Tx_ProcessPduStates(pduIdx);
      }
    }
  }
#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */
}

#if ((SECOC_RX_CSM_ASYNC == STD_ON) || (SECOC_TX_CSM_ASYNC == STD_ON))
/* !LINKSTO SecOC.Dsn.interface.RxTx.SecOC_CsmCallbackNotification,1 */
FUNC(void, SECOC_CODE) SecOC_CsmCallbackNotification
(
  P2CONST(Crypto_JobType, AUTOMATIC, SECOC_APPL_DATA) job,
          Std_ReturnType                              result
)
{
  uint16         pduIdx;


  if (SECOC_INIT == SecOC_InitStatus)
  {
#if ((SECOC_NUMBER_TX_PDUS > 0) && (SECOC_TX_CSM_ASYNC == STD_ON))
    /* Check if the Csm callback is for the authentication service */
    for (pduIdx = 0U; pduIdx < SECOC_NUMBER_TX_PDUS; pduIdx++)
    {
      if ((SECOC_TX_CSM_JOB_ID(pduIdx) == job->jobId                                   ) &&
          (SECOC_TX_STATE_WAITFORAUTHENTICATIONCALLBACK == SecOC_TxData[pduIdx].TxState)
         )
      {
        if (E_OK == result)
        {
          SecOC_TxData[pduIdx].TxAuthEvent = SECOC_TXEVENT_AUTHCALLBACK_OK;
        }
        else
        {
          SecOC_TxData[pduIdx].TxAuthEvent = SECOC_TXEVENT_AUTHCALLBACK_NOT_OK;

#if (STD_ON == SECOC_MACGENERATE_STATUS_PROP_ENABLED)
          SecOC_TxData[pduIdx].MacGenerateResult = result;
#endif
        }
        break;
      }
    }
#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */

#if ((SECOC_NUMBER_RX_PDUS > 0) && (SECOC_RX_CSM_ASYNC == STD_ON))
    /* Check if the Csm callback is for the verification service */
    for (pduIdx = 0U; pduIdx < SECOC_NUMBER_RX_PDUS; pduIdx++)
    {
      if ((SECOC_RX_CSM_JOB_ID(pduIdx) == job->jobId                                 ) &&
          (SECOC_RX_STATE_WAITFORVERIFICATIONCALLBACK == SecOC_RxData[pduIdx].RxState)
         )
      {
        if (E_OK == result)
        {
          SecOC_RxData[pduIdx].RxVerEvent = SECOC_RXEVENT_VERCALLBACK_OK;
        }
        else
        {
          SecOC_RxData[pduIdx].RxVerEvent = SECOC_RXEVENT_VERCALLBACK_NOT_OK;
        }
#if (SECOC_VERIFICATION_STATUS_PROP_EB == SECOC_VERIFICATION_STATUS_PROP_ENABLED)
        SecOC_RxData[pduIdx].VerificationReturn = result;
#endif
        break;
      }
    }
#endif /* #if (SECOC_NUMBER_RX_PDUS > 0) */
  }
}
#endif /* #if ((SECOC_RX_CSM_ASYNC == STD_ON) || (SECOC_TX_CSM_ASYNC == STD_ON)) */

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

#if (SECOC_NUMBER_RX_PDUS > 0)

#if (STD_ON == SECOC_INCLUDE_RTE)
#define SecOC_START_SEC_CODE
#else
#define SECOC_START_SEC_CODE
#endif
#include <SecOC_MemMap.h>

/* !LINKSTO SWS_SecOC_Rx_00122,3 */
FUNC(Std_ReturnType, SECOC_CODE) SecOC_VerifyStatusOverride
(
  uint16  freshnessValueId,
  uint8   overrideStatus,
  uint8   numberOfMessagesToOverride
)
{
  uint16         pduIdx  = 0U;
  Std_ReturnType idFound = E_NOT_OK;

  if (SECOC_INIT == SecOC_InitStatus)
  {
#if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_OFF)
    if (SECOC_OVERRIDESTATUS_CANCEL >= overrideStatus)
#else
    if (
        (SECOC_OVERRIDESTATUS_SKIP_UNTIL_NOTICE == overrideStatus) ||
        (SECOC_OVERRIDESTATUS_PASS_MSG_NUMBER   == overrideStatus) ||
        (SECOC_OVERRIDESTATUS_CANCEL            >= overrideStatus)
       )
#endif
    {
      /* !LINKSTO SecOC_EB_Rx_xxx06,1 */
      for (pduIdx = 0U; pduIdx < SECOC_NUMBER_RX_PDUS; pduIdx++)
      {
        if (freshnessValueId == SECOC_GET_RX_FRESHNESS_ID(pduIdx))
        {
          SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();

#if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_OFF)
          if (SECOC_OVERRIDESTATUS_FAIL_MSG_NUMBER == overrideStatus)
#else
          if ((SECOC_OVERRIDESTATUS_PASS_MSG_NUMBER == overrideStatus)||
              (SECOC_OVERRIDESTATUS_FAIL_MSG_NUMBER == overrideStatus)
             )
#endif
          {
            SecOC_RxData[pduIdx].NumMsgToOverride = numberOfMessagesToOverride;
          }
          SecOC_RxData[pduIdx].VerifyStatusOverride = overrideStatus;

          SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();

          idFound = E_OK;
        }
      }
    }
  }
  return idFound;
}

#if (STD_ON == SECOC_INCLUDE_RTE)
#define SecOC_STOP_SEC_CODE
#else
#define SECOC_STOP_SEC_CODE
#endif
#include <SecOC_MemMap.h>

#endif /* #if (SECOC_NUMBER_RX_PDUS > 0) */

#if ((SECOC_NUMBER_TX_PDUS > 0) && (SECOC_BYPASS_AUTHENTICATION_ROUTINE == STD_ON))

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

/* !LINKSTO SecOC_EB_Tx_xxx29,1 */
FUNC(void, SECOC_CODE) SecOC_BypassAuthRoutine
(
  boolean state
)
{
  SecOC_TxBypassAuthRoutineState = state;
}

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

#endif /* #if ((SECOC_NUMBER_TX_PDUS > 0) && (SECOC_BYPASS_AUTHENTICATION_ROUTINE == STD_ON)) */

/*==================[internal function definitions]===============================================*/

#if (SECOC_NUMBER_RX_PDUS > 0)

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

static FUNC(boolean, SECOC_CODE) SecOC_RxCheckForPduIdx
(
  P2VAR(uint16, AUTOMATIC, SECOC_APPL_DATA) rxPduIdx,
  P2VAR(uint8,  AUTOMATIC, SECOC_APPL_DATA) rxPduType
)
{
  boolean idFound = FALSE;
  uint16 confPduIdx;

  /* Change the external PDU ID into the equivalent internal configuration PDU ID */
  for (confPduIdx = 0U; ((confPduIdx < SECOC_NUMBER_RX_PDUS) && (idFound == FALSE)); confPduIdx++)
  {
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
    if (SecOC_RxConfig[confPduIdx].UseSecPduCollection == TRUE)
    {
      if (SecOC_RxConfig[confPduIdx].RxCryptoLayerPduIdAtSecOC == *rxPduIdx)
      {
        *rxPduIdx = confPduIdx;
        *rxPduType = SECOC_CRYPTO_PDU;
        idFound = TRUE;
      }
      else if (SecOC_RxConfig[confPduIdx].RxReceivedAuthLayerPduIdAtSecOC == *rxPduIdx)
      {
        *rxPduIdx = confPduIdx;
        *rxPduType = SECOC_AUTH_PDU;
        idFound = TRUE;
      }
      else
      {
        /* Do nothing as the received ID is not for SecOC */
      }
    }
    else
#endif
    {
      if (SecOC_RxConfig[confPduIdx].RxSecuredLayerPduIdAtSecOC == *rxPduIdx)
      {
        *rxPduIdx = confPduIdx;
        *rxPduType = SECOC_SECURED_PDU;
        idFound = TRUE;
      }
    }
  }

  if (idFound == FALSE)
  {
    *rxPduIdx = 0U;
  }

  return idFound;
}


#if (STD_ON == SECOC_RX_USE_SAME_BUFFER)
static FUNC(boolean, SECOC_CODE) SecOC_RxCheckBufferAvailability
(
  uint16 pduIdx
)
{
  boolean bufferAvailable = FALSE;

  if (NULL_PTR != SecOC_RxData[pduIdx].RxBufferUsed)
  {
#if (STD_ON == SECOC_RX_USE_SECURED_PDU_COL)
    if (TRUE == SecOC_RxConfig[pduIdx].UseSecPduCollection)
    {
      if (
          (FALSE == *SecOC_RxData[pduIdx].RxBufferUsed) ||
          (
            ( SecOC_RxData[pduIdx].RxPduRIndicationForAuth == SECOC_RXEVENT_RXINDICATION   ) ||
            ( SecOC_RxData[pduIdx].RxPduRIndicationForCrypto == SECOC_RXEVENT_RXINDICATION )
          )
        )
      {
        bufferAvailable = TRUE;
      }
    }
    else
#endif /* #if (STD_ON == SECOC_RX_USE_SECURED_PDU_COL) */
    {
      if (FALSE == *SecOC_RxData[pduIdx].RxBufferUsed)
      {
        bufferAvailable = TRUE;
      }
    }
  }
  else
  {
    bufferAvailable = TRUE;
  }

  return bufferAvailable;
}
#endif /* #if (STD_ON == SECOC_RX_USE_SAME_BUFFER) */


static FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_RxCheckIncomingPdu
(
  uint32 pduLength,
  uint16 pduIdx,
  uint8  pduType
)
{
  BufReq_ReturnType bufReqRetVal = BUFREQ_E_NOT_OK;

  /* Is the incoming PDU the secured PDU? */
  if (pduType == SECOC_SECURED_PDU)
  {
    bufReqRetVal = SecOC_RxCheckIncomingSecuredPdu(pduLength, pduIdx);
  }
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
  /* Is the incoming PDU the authentic PDU? */
  else if (pduType == SECOC_AUTH_PDU)
  {
#if (SECOC_RX_USE_MESS_LINKER == STD_ON)
    /* Can the message link be extracted from the authentic PDU? (enough data) */
    if (pduLength >= SECOC_REQUIREDBYTES(SECOC_RX_MSGLINK_MIN_AUTHDATALEN(pduIdx)))
#endif
    {
#if (STD_ON == SECOC_PDU_SECURED_AREA)
      /* !LINKSTO SWS_SecOC_00314,1 */
      /* Can the secured authentic data be extracted from the authentic PDU? (enough data) */
      if (SECOC_RX_SECAREA_MIN_AUTHDATALEN(pduIdx) <= pduLength)
#endif /* #if (STD_ON == SECOC_PDU_SECURED_AREA) */
      {
        /* Do we have enough space to store the incoming authentic PDU? */
        if (pduLength <= SECOC_GET_RX_AUTH_PDU_LEN(pduIdx))
        {
          /* Enough space to store the incoming PDU */
          bufReqRetVal = BUFREQ_OK;
        }
        else
        {
          /* Not enough space to store the incoming authentic PDU */
          bufReqRetVal = BUFREQ_E_OVFL;
        }
      }
    }
  }
  /* The incoming PDU is a cryptographic PDU */
  else
  {
    /* Does the incoming cryptographic PDU has the required length */
    if (pduLength == SECOC_GET_RX_CRYPTO_PDU_LEN(pduIdx))
    {
      bufReqRetVal = BUFREQ_OK;
    }
  }
#endif /* #if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON) */

  return bufReqRetVal;
}


static FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_RxCheckIncomingSecuredPdu
(
  uint32 pduLength,
  uint16 pduIdx
)
{
  BufReq_ReturnType bufReqRetVal = BUFREQ_E_NOT_OK;
  const uint32 authInfoLen = SECOC_REQUIREDBYTES(SECOC_RX_AUTHENTICATION_INFO_LEN(pduIdx));

  /* Does the incoming secured PDU contains an authentic part? (enough data) */
  if (pduLength > authInfoLen)
  {
#if (STD_ON == SECOC_PDU_SECURED_AREA)
    /* !LINKSTO SWS_SecOC_00314,1 */
    /* Can the secured authentic data be extracted from the secured PDU? (enough data) */
    if (
        SECOC_RX_SECAREA_MIN_AUTHDATALEN(pduIdx) <=
        (pduLength - authInfoLen)
       )
#endif /* #if (STD_ON == SECOC_PDU_SECURED_AREA) */
    {
      /* Do we have enough space to store the incoming secured PDU? */
      if (pduLength <= SECOC_GET_RX_SECURED_PDU_LEN(pduIdx))
      {
        /* Enough space to store the incoming PDU */
        bufReqRetVal = BUFREQ_OK;
      }
      else
      {
        /* Not enough space to store the incoming secured PDU */
        bufReqRetVal = BUFREQ_E_OVFL;
      }
    }
  }

  return bufReqRetVal;
}


#if (SECOC_RX_USE_MESS_LINKER == STD_ON)
static FUNC(Std_ReturnType, SECOC_CODE) SecOC_RxCheckMessageLink
(
  P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) bufferAData,
          uint32                             bufferABitPos,
  P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) bufferBData,
          uint32                             bufferBBitPos,
          uint32                             bitLength
)
{
  uint32 bufferAIndex = bufferABitPos / 8U;
  uint32 bufferBIndex = bufferBBitPos / 8U;

  uint8 bufferAShift = bufferABitPos % 8U;
  uint8 bufferBShift = bufferBBitPos % 8U;

  uint32 intBitLength = bitLength;
  uint32 compareIndex = 0U;
  uint8 nextAIndex = 0U;
  uint8 nextBIndex = 0U;

  boolean isEqual = TRUE;
  Std_ReturnType compareResult = E_NOT_OK;


  if (bufferAShift != 0U)
  {
    nextAIndex = 1U;
  }

  if (bufferBShift != 0U)
  {
    nextBIndex = 1U;
  }

  while((isEqual == TRUE) && (((compareIndex + 1U) * 8U) <= intBitLength))
  {
    if
    (
      (uint8)(
               (bufferAData[bufferAIndex + compareIndex             ] <<       bufferAShift ) |
               (bufferAData[bufferAIndex + compareIndex + nextAIndex] >> (8U - bufferAShift))
             ) !=
      (uint8)(
               (bufferBData[bufferBIndex + compareIndex             ] <<       bufferBShift ) |
               (bufferBData[bufferBIndex + compareIndex + nextBIndex] >> (8U - bufferBShift))
             )
    )
    {
      isEqual = FALSE;
    }

    compareIndex++;
  }

  intBitLength -= (compareIndex * 8U);

  if((intBitLength > 0U) && (isEqual == TRUE))
  {
    uint8 bufferALastCompareByte;
    uint8 bufferBLastCompareByte;

    uint8 i;

    bufferALastCompareByte = (uint8)(bufferAData[bufferAIndex + compareIndex] << bufferAShift);

    if((intBitLength + bufferAShift) > 8U)
    {
      bufferALastCompareByte |=
        (uint8)(bufferAData[bufferAIndex + compareIndex + 1U] >> (8U - bufferAShift));
    }

    bufferBLastCompareByte = (uint8)(bufferBData[bufferBIndex + compareIndex] << bufferBShift);

    if((intBitLength + bufferBShift) > 8U)
    {
      bufferBLastCompareByte |=
        (uint8)(bufferBData[bufferBIndex + compareIndex + 1U] >> (8U - bufferBShift));
    }

    for(i = 0U; i < intBitLength; i++)
    {
      if(((bufferALastCompareByte << i) & 0x80U) != ((bufferBLastCompareByte << i) & 0x80U))
      {
        isEqual = FALSE;
      }
    }
  }

  if (isEqual == TRUE)
  {
    compareResult = E_OK;
  }
  else
  {
    compareResult = E_NOT_OK;
  }

  return compareResult;
}
#endif /* #if (SECOC_RX_USE_MESS_LINKER == STD_ON) */

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

#endif /* #if (SECOC_NUMBER_RX_PDUS > 0) */

#if (SECOC_NUMBER_TX_PDUS > 0)
#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

static FUNC(uint8, SECOC_CODE) SecOC_TxCheckBufferAvailability
(
  uint16 pduIdx
)
{
  uint8 bufferStatus;

  SECOC_TX_GET_BUFFER_LVL1_STATUS(pduIdx, bufferStatus);

  if (
      (SECOC_BUFFER_FREE == bufferStatus) ||
      (
        (SECOC_BUFFER_USED      == bufferStatus                        ) &&
        (SECOC_TXEVENT_TRANSMIT == SecOC_TxData[pduIdx].TxTransmitEvent)
      )
     )
  {
    bufferStatus = SECOC_BUFFER_FREE;
  }

  return bufferStatus;
}

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

#endif /* #if (STD_ON == SECOC_TX_USE_SAME_BUFFER) */
#endif /* #if (SECOC_NUMBER_TX_PDUS > 0) */

/*==================[end of file]=================================================================*/
