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

/*  Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 8.9 (Advisory)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason:
 * The AUTOSAR memory mapping requires that functions are mapped to SEC_CODE memory sections.
 * Objects at block scope require a different mapping, e.g. to a SEC_CONST section,
 * which leads to nested memory sections, which is not supported by some compilers.
 *
 */

/*==================[includes]====================================================================*/
/* !LINKSTO SecOC.Dsn.BasicSWStructure.Tx.SecOC_Txc,1 */
#include <SecOC_Tx.h>

#if (STD_ON == SECOC_INCLUDE_RTE)
#include <Rte_SecOC.h>
#endif

/*==================[macros]======================================================================*/

#if (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_RTE)

#if (SECOC_TX_FRESHNESS == STD_ON)

/** \brief Macro, which defines the name of the callout function
 *         used to obtain the freshness value
 */
#if (defined SECOC_GET_TX_FRESHNESS_FUNC)
#error SECOC_GET_TX_FRESHNESS_FUNC already defined
#endif
#define SECOC_GET_TX_FRESHNESS_FUNC              Rte_Call_PS_TxFreshnessManagement_GetTxFreshness

#endif /* #if (SECOC_TX_FRESHNESS == STD_ON) */

#if (SECOC_TX_FRESHNESS_TRUNC_DATA == STD_ON)

/** \brief Macro, which defines the name of the callout function
 *         used to obtain the freshness value and the truncated freshness value
 */
#if (defined SECOC_GET_TX_FRESHNESS_TRUNCDATA_FUNC)
#error SECOC_GET_TX_FRESHNESS_TRUNCDATA_FUNC already defined
#endif
#define SECOC_GET_TX_FRESHNESS_TRUNCDATA_FUNC    Rte_Call_PS_TxFreshnessManagement_GetTxFreshnessTruncData

#endif /* #if (SECOC_TX_FRESHNESS_TRUNC_DATA == STD_ON) */

/** \brief Macro, which defines the name of the callout function
 *         used to obtain confirmation that the PDU was sent
 */
#if (defined SECOC_SPDUTXCONFIRMATION_FUNC)
#error SECOC_SPDUTXCONFIRMATION_FUNC already defined
#endif
#define SECOC_SPDUTXCONFIRMATION_FUNC            Rte_Call_PS_TxFreshnessManagement_SPduTxConfirmation

#elif (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_CFUNC)

#if (SECOC_TX_FRESHNESS == STD_ON)

/** \brief Macro, which defines the structure that holds the pointer to function
 *         used to obtain the freshness value
 */
#if (defined SECOC_GET_TX_FRESHNESS_FUNC)
#error SECOC_GET_TX_FRESHNESS_FUNC already defined
#endif
#define SECOC_GET_TX_FRESHNESS_FUNC              SecOC_TxConfig[instId].GetTxFreshness

#endif /* #if (SECOC_TX_FRESHNESS == STD_ON) */


#if (SECOC_TX_FRESHNESS_TRUNC_DATA == STD_ON)

/** \brief Macro, which defines the structure that holds the pointer to function
 *         used to obtain the freshness value and the truncated freshness value
 */
#if (defined SECOC_GET_TX_FRESHNESS_TRUNCDATA_FUNC)
#error SECOC_GET_TX_FRESHNESS_TRUNCDATA_FUNC already defined
#endif
#define SECOC_GET_TX_FRESHNESS_TRUNCDATA_FUNC    SecOC_TxConfig[instId].GetTxFreshnessTruncData

#endif /* #if (SECOC_TX_FRESHNESS_TRUNC_DATA == STD_ON) */

/** \brief Macro, which defines the structure that holds the pointer to function
 *         used to confirm that the PDU was sent
 */
#if (defined SECOC_SPDUTXCONFIRMATION_FUNC)
#error SECOC_SPDUTXCONFIRMATION_FUNC already defined
#endif
#define SECOC_SPDUTXCONFIRMATION_FUNC            SecOC_TxConfig[instId].SPduTxConfirmation

#endif /* #if (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_CFUNC) */

/** \brief Macro to calculate the sum of the Tx authentication information length that is comprised
 *         of the truncated authenticator value and the truncated freshness value
 */
#if (defined SECOC_TX_AUTHENTICATION_INFO_LEN)
#error SECOC_TX_AUTHENTICATION_INFO_LEN already defined
#endif
#define SECOC_TX_AUTHENTICATION_INFO_LEN(Id) ((uint32)(SECOC_GET_TX_AUTHINFOTX_LEN((Id))) + \
                                              (uint32)(SECOC_GET_TX_FRESHNESSTX_LEN((Id))))

/*==================[type definitions]============================================================*/

/*==================[internal function declarations]==============================================*/

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

/** \brief Idle state of the SecOC Tx state machine
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Tx_StateIdle(uint16 instId);

/** \brief State of the SecOC Tx state machine, which gets the freshness value
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Tx_StateGetFreshness(uint16 instId);

/** \brief State of the SecOC Tx state machine, which starts an authentication
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Tx_StateAuthentication(uint16 instId);

#if (SECOC_TX_CSM_ASYNC == STD_ON)
/** \brief State of the SecOC Tx state machine, which waits
 *         for the callback and evaluates the result
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Tx_StateWaitForAuthenticationCallback(uint16 instId);
#endif

/** \brief State of the SecOC Tx state machine, which constructs the secured Pdu
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Tx_StateConstructPdu(uint16 instId);

/** \brief State of the SecOC Tx state machine, which transmits the secured Pdu
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Tx_StateBroadcastPdu(uint16 instId);

/** \brief State of the SecOC Tx state machine, where CanTp/PduR can copy the secured Pdu
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Tx_StateWaitForConfirmation(uint16 instId);

/** \brief Function used to reset the state machine
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Tx_ResetSm(uint16 instId);

/** \brief Function used to handle the authentication retry
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Tx_CheckForCsmRetry(uint16 instId);

/** \brief Function used to copy the freshness value to the secured Pdu
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Tx_CopyFreshnessToSecuredPdu(uint16 instId);

/** \brief Function used to copy the MAC to the secured Pdu
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Tx_CopyMacToSecuredPdu(uint16 instId);

#if (SECOC_TX_SHAPE_SEC_PDU == STD_ON)
/** \brief Function used to update the secured Pdu layout to a fix one
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Tx_UpdateLayoutSecuredPdu(uint16 instId);
#endif

#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
/** \brief Function used to broadcast the cryptographic PDU to the lower layer
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(Std_ReturnType, SECOC_CODE) SecOC_Tx_BroadcastCryptoPdu(uint16 instId);

/** \brief Function used to broadcast the authentic PDU to the lower layer
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(Std_ReturnType, SECOC_CODE) SecOC_Tx_BroadcastAuthenticPdu(uint16 instId);

#if (SECOC_TX_USE_MESS_LINKER == STD_ON)
/** \brief Function used to copy the message link to the cryptographic Pdu
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Tx_CopyMessageLinkToCryptoPdu(uint16 instId);
#endif
#endif

#if (STD_ON == SECOC_MACGENERATE_STATUS_PROP_ENABLED)
/** \brief Function which propagates the MAC Generate status to the configured callout
 *         functions
 * \param[in] instId ID of the state machine instance (i.e. TX PDU)
 *
 */
static FUNC(void, SECOC_CODE) SecOC_Tx_PropagateMacGenerateStatus
(
  uint16  instId
);
#endif

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

/*==================[internal constants]==========================================================*/

#define SECOC_START_SEC_CONST_UNSPECIFIED
#include <SecOC_MemMap.h>

/**
 * \brief Variable which holds the states of the SecOC Tx state machine
 */
/* Deviation MISRAC2012-1 */
static CONST(SecOC_SmStateType, SECOC_CONST) SecOC_TxStateMachine[SECOC_NUMBER_OF_TXSTATES] =
{
  &SecOC_Tx_StateIdle,
  &SecOC_Tx_StateGetFreshness,
  &SecOC_Tx_StateAuthentication,
  &SecOC_Tx_StateConstructPdu,
  &SecOC_Tx_StateBroadcastPdu,
  &SecOC_Tx_StateWaitForConfirmation,
#if (SECOC_TX_CSM_ASYNC == STD_ON)
  &SecOC_Tx_StateWaitForAuthenticationCallback
#endif
};

#define SECOC_STOP_SEC_CONST_UNSPECIFIED
#include <SecOC_MemMap.h>

/*==================[internal data]===============================================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function definitions]===============================================*/

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

FUNC(void, SECOC_CODE) SecOC_Tx_ProcessPduStates
(
  uint16 pduIdx
)
{
  /* !LINKSTO SecOC_EB_Tx_xxx26,1 */
  do
  {
    SecOC_TxData[pduIdx].ProcessingAuthentication = FALSE;
    SecOC_TxStateMachine[SecOC_TxData[pduIdx].TxState](pduIdx);
  }
  while (TRUE == SecOC_TxData[pduIdx].ProcessingAuthentication);
}

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

/*==================[internal function definitions]===============================================*/

/*==================[state machine states]========================================================*/

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

static FUNC(void, SECOC_CODE) SecOC_Tx_StateIdle(uint16 instId)
{
  if (SECOC_TXEVENT_TRANSMIT == SecOC_TxData[instId].TxTransmitEvent)
  {
#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)
    uint8 bufferStatus;
    SECOC_TX_GET_BUFFER_LVL2_STATUS(instId, bufferStatus);

    if (bufferStatus == SECOC_BUFFER_FREE)
#endif
    {
      SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)
      SECOC_TX_SET_BUFFER_LVL2_STATUS(instId, SECOC_BUFFER_USED);
#endif
      SecOC_TxData[instId].TxDataLength = SecOC_TxData[instId].TxReceivedDataLength;
      TS_MemCpy
      (
        SecOC_TxData[instId].TxSecuredPduBuffer,
        SecOC_TxData[instId].TxReceivedAuthPduBuffer,
        SecOC_TxData[instId].TxDataLength
      );

      SecOC_TxData[instId].TxTransmitEvent = SECOC_TXEVENT_NONE;
      SecOC_TxData[instId].TxPduREvent     = SECOC_TXEVENT_NONE;

#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)
      SECOC_TX_SET_BUFFER_LVL1_STATUS(instId, SECOC_BUFFER_FREE);
#endif
      SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();

  #if (SECOC_BYPASS_AUTHENTICATION_ROUTINE == STD_ON)
      /* !LINKSTO SecOC_EB_Tx_xxx30,1 */
      if (TRUE == SecOC_TxBypassAuthRoutineState)
      {
        TS_MemSet
        (
          &SecOC_TxData[instId].TxSecuredPduBuffer[SecOC_TxData[instId].TxDataLength],
          SECOC_BYPASS_DEFAULT_AUTHINFO_VALUE,
          SECOC_REQUIREDBYTES(SECOC_TX_AUTHENTICATION_INFO_LEN(instId))
        );

        if ((SECOC_TX_AUTHENTICATION_INFO_LEN(instId) % 8) != 0)
        {
          SecOC_TxData[instId].TxSecuredPduBuffer
            [
              SecOC_TxData[instId].TxDataLength +
              SECOC_REQUIREDBYTES(SECOC_TX_AUTHENTICATION_INFO_LEN(instId)) - 1U
            ] &= ~((1U << (8U - (SECOC_TX_AUTHENTICATION_INFO_LEN(instId) % 8))) - 1U);
        }

        SecOC_TxData[instId].TxState = SECOC_TX_STATE_CONSTRUCTSECUREDPDU;
      }
      else
  #endif
      {
        /* !LINKSTO SWS_SecOC_Tx_00031,1, SWS_SecOC_Tx_00034_1,2, SWS_SecOC_Tx_00034_2,1,
               SecOC_EB_RxTx_xxx03,1*/
        /* DataId */
        {
          uint8 index;
          for (index = 0U; index < SECOC_DATAID_LENGTH; index++)
          {
           SecOC_TxData[instId].TxDataToAuthenticator[SECOC_DATAID_LENGTH - index - 1U] =
           (uint8) (SECOC_GET_TX_DATA_ID(instId) >> (index * 8U));
          }
        }

        /* Authentic data or the secured authentic data part */
  #if (STD_ON == SECOC_PDU_SECURED_AREA)
        /* !LINKSTO SWS_SecOC_00311,1, SWS_SecOC_00312,1 */
        if (TRUE == SECOC_GET_TX_USE_SECURED_AREA(instId))
        {
          /* Copy the secured authentic part from the secured PDU */
          TS_MemCpy
          (
            &SecOC_TxData[instId].TxDataToAuthenticator[SECOC_DATAID_LENGTH],
            &SecOC_TxData[instId].TxSecuredPduBuffer[SECOC_GET_TX_SECURED_AREA_OFFSET(instId)],
            SECOC_GET_TX_SECURED_AREA_LEN(instId)
          );
        }
        else
  #endif /* #if (STD_ON == SECOC_PDU_SECURED_AREA) */
        {
          /* Authentic data */
          TS_MemCpy
          (
            &SecOC_TxData[instId].TxDataToAuthenticator[SECOC_DATAID_LENGTH],
            SecOC_TxData[instId].TxSecuredPduBuffer,
            SecOC_TxData[instId].TxDataLength
          );
        }

        SecOC_TxData[instId].TxState = SECOC_TX_STATE_GETFRESHNESS;
      }

      /* !LINKSTO SWS_SecOC_00226,1 */
      SecOC_TxData[instId].AuthAttempts = 0U;

      /* !LINKSTO SecOC_EB_Tx_xxx01,1 */
      SecOC_TxData[instId].ProcessingAuthentication = TRUE;
    }
  }
}


static FUNC(void, SECOC_CODE) SecOC_Tx_StateGetFreshness(uint16 instId)
{
  if (SECOC_TXEVENT_CANCEL == SecOC_TxData[instId].TxPduREvent)
  {
    SecOC_Tx_ResetSm(instId);
  }
  else
  {
#if (SECOC_QUERY_FRESHNESS_VALUE != SECOC_FRESHNESS_NONE)
    Std_ReturnType retVal = E_NOT_OK;
#endif
#if ((SECOC_TX_FRESHNESS_TRUNC_DATA == STD_ON) && (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_RTE))
    uint8 tempFreshnessValueBuffer[8U] = { 0U };
#endif

    /* !LINKSTO SecOC_EB_Tx_xxx01,1 */
    SecOC_TxData[instId].ProcessingAuthentication = TRUE;

    /* !LINKSTO SWS_SecOC_Tx_00031,1, SWS_SecOC_Tx_00033,1, SWS_SecOC_Tx_00061,1,
                SWS_SecOC_Tx_00066,1, SWS_SecOC_Tx_00071,1 */
    /* !LINKSTO SecOC_EB_Tx_xxx07,1 */

#if (SECOC_QUERY_FRESHNESS_VALUE != SECOC_FRESHNESS_NONE)
    /* !LINKSTO SecOC.Dsn.interface.Tx.TxFreshnessManagement_FreshnessLengthZero,3 */
    if (0U != SECOC_GET_TX_FRESHNESS_LEN(instId))
    {
      uint32 freshnessValueLength = SECOC_GET_TX_FRESHNESS_LEN(instId);
      P2VAR(uint8, AUTOMATIC, SECOC_VAR) freshnessValuePtr = NULL_PTR;

#if (STD_ON == SECOC_PDU_SECURED_AREA)
      if (TRUE == SECOC_GET_TX_USE_SECURED_AREA(instId))
      {
        freshnessValuePtr = &SecOC_TxData[instId].TxDataToAuthenticator
                            [SECOC_DATAID_LENGTH + SECOC_GET_TX_SECURED_AREA_LEN(instId)];
      }
      else
#endif /* #if (STD_ON == SECOC_PDU_SECURED_AREA) */
      {
        freshnessValuePtr = &SecOC_TxData[instId].TxDataToAuthenticator
                            [SECOC_DATAID_LENGTH + SecOC_TxData[instId].TxDataLength];
      }

      /* !LINKSTO SWS_SecOC_Tx_00070,2 */
      switch(SecOC_TxConfig[instId].GetTxFreshnessFuncType)
      {
#if (SECOC_TX_FRESHNESS == STD_ON)
        case SECOC_GET_TX_FRESHNESS_FUNC_TYPE:
        {
          /* !LINKSTO SWS_SecOC_Tx_xx004,3 */
          /* !LINKSTO SWS_SecOC_Tx_xx006,3 */
          retVal = SECOC_GET_TX_FRESHNESS_FUNC
                   (
                     SECOC_GET_TX_FRESHNESS_ID(instId),
                     freshnessValuePtr,
                     &freshnessValueLength
                   );
        }
        break;
#endif /* #if (SECOC_TX_FRESHNESS == STD_ON) */
#if (SECOC_TX_FRESHNESS_TRUNC_DATA == STD_ON)
        case SECOC_GET_TX_FRESHNESS_TRUNCDATA_FUNC_TYPE:
        {
          uint32 freshnessValueTxLength = SECOC_GET_TX_FRESHNESSTX_LEN(instId);

          /* !LINKSTO SWS_SecOC_Tx_00221,1 */
          /* !LINKSTO SWS_SecOC_Tx_00223,1 */
          retVal = SECOC_GET_TX_FRESHNESS_TRUNCDATA_FUNC
                   (
                     SECOC_GET_TX_FRESHNESS_ID(instId),
                     freshnessValuePtr,
                     &freshnessValueLength,
#if (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_RTE)
                    tempFreshnessValueBuffer,
#else
                    &SecOC_TxData[instId].TxSecuredPduBuffer[SecOC_TxData[instId].TxDataLength],
#endif
                     &freshnessValueTxLength
                   );
        }
        break;
#endif /* #if (SECOC_TX_FRESHNESS_TRUNC_DATA == STD_ON) */
        /* CHECK: NOPARSE */
        default:
        {
          /* Nothing to do. */
        }
        break;
        /* CHECK: PARSE */
      }

      SECOC_PARAM_UNUSED(freshnessValuePtr);
      SECOC_PARAM_UNUSED(freshnessValueLength);
    }
    else
    {
      retVal = E_OK;
    }

    if (retVal != E_OK)
    {
      if (SecOC_TxConfig[instId].MaxAuthAttempts > SecOC_TxData[instId].AuthAttempts)
      {
        /* !LINKSTO SWS_SecOC_Tx_xx009,1 */
        SecOC_TxData[instId].AuthAttempts++;

#if (SECOC_TX_SYNC_PDU_PROCESSING == STD_ON)
        if (FALSE == SecOC_TxConfig[instId].UseTxSyncProcessing)
#endif /* #if (SECOC_TX_SYNC_PDU_PROCESSING == STD_ON) */
        {
          /* !LINKSTO SecOC_EB_Tx_xxx02,2 */
          SecOC_TxData[instId].ProcessingAuthentication = FALSE;
        }
      }
      else
      {
        /* !LINKSTO SWS_SecOC_Tx_00155,1 */
        SecOC_Tx_ResetSm(instId);
      }
    }
    else
#endif /* #if (SECOC_QUERY_FRESHNESS_VALUE != SECOC_FRESHNESS_NONE) */
    {
#if ((SECOC_TX_FRESHNESS_TRUNC_DATA == STD_ON) && (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_RTE))
      if (SECOC_GET_TX_FRESHNESS_TRUNCDATA_FUNC_TYPE == SecOC_TxConfig[instId].GetTxFreshnessFuncType)
      {
        TS_MemCpy
        (
          &SecOC_TxData[instId].TxSecuredPduBuffer[SecOC_TxData[instId].TxDataLength],
          tempFreshnessValueBuffer,
          SECOC_REQUIREDBYTES(SECOC_GET_TX_FRESHNESSTX_LEN(instId))
        );
      }
#endif
      SecOC_TxData[instId].TxState = SECOC_TX_STATE_AUTHENTICATION;
    }
  }
}


static FUNC(void, SECOC_CODE) SecOC_Tx_StateAuthentication(uint16 instId)
{
  if (SECOC_TXEVENT_CANCEL == SecOC_TxData[instId].TxPduREvent)
  {
    SecOC_Tx_ResetSm(instId);
  }
  else
  {
    Std_ReturnType retVal = E_NOT_OK;
    uint32 dataLength = 0U;

    /* !LINKSTO SecOC_EB_Tx_xxx01,1 */
    SecOC_TxData[instId].ProcessingAuthentication = TRUE;

    /* Length of the data based on which the MAC shall be calculate */
#if (STD_ON == SECOC_PDU_SECURED_AREA)
    if (TRUE == SECOC_GET_TX_USE_SECURED_AREA(instId))
    {
      dataLength =
      (
       (uint32) SECOC_GET_TX_SECURED_AREA_LEN(instId)          +
       (uint32) SECOC_DATAID_LENGTH                            +
       SECOC_REQUIREDBYTES(SECOC_GET_TX_FRESHNESS_LEN(instId))
      );
    }
    else
#endif /* #if (STD_ON == SECOC_PDU_SECURED_AREA) */
    {
      dataLength =
      (
       (uint32) SecOC_TxData[instId].TxDataLength              +
       (uint32) SECOC_DATAID_LENGTH                            +
       SECOC_REQUIREDBYTES(SECOC_GET_TX_FRESHNESS_LEN(instId))
      );
    }

#if (SECOC_TX_CSM_ASYNC == STD_ON)
    if (TRUE == SecOC_TxConfig[instId].UseCsmAsync)
    {
      /* Set the next execution state */
      SecOC_TxData[instId].TxState = SECOC_TX_STATE_WAITFORAUTHENTICATIONCALLBACK;
      /* Prepare the Csm callback event flag */
      SecOC_TxData[instId].TxAuthEvent = SECOC_TXEVENT_NONE;
    }
    else
    {
      /* Set the next execution state */
      SecOC_TxData[instId].TxState = SECOC_TX_STATE_CONSTRUCTSECUREDPDU;
    }
#else
    /* Set the next execution state */
    SecOC_TxData[instId].TxState = SECOC_TX_STATE_CONSTRUCTSECUREDPDU;
#endif /* #if (SECOC_TX_CSM_ASYNC == STD_ON) */

    SecOC_TxData[instId].TxAuthenticatorLengthResult =
                                       SECOC_REQUIREDBYTES(SECOC_GET_TX_AUTHINFOTX_LEN(instId));

    /* !LINKSTO SWS_SecOC_Tx_00060,1, SWS_SecOC_Tx_00065,1, SWS_SecOC_Tx_00070,2 */
    /* !LINKSTO SWS_SecOC_Tx_00035,2, SWS_SecOC_Tx_00031,1,
                SecOC.Dsn.interface.RxTx.SecOC_CsmSingleCalls,1 */
    retVal = Csm_MacGenerate
             (
           (Csm_ConfigIdType)SECOC_TX_CSM_JOB_ID(instId),
              CRYPTO_OPERATIONMODE_SINGLECALL,
              SecOC_TxData[instId].TxDataToAuthenticator,
              dataLength,
              SecOC_TxData[instId].TxAuthenticator,
              &SecOC_TxData[instId].TxAuthenticatorLengthResult
             );

#if (STD_ON == SECOC_MACGENERATE_STATUS_PROP_ENABLED)
    SecOC_TxData[instId].MacGenerateResult = retVal;
#endif

    if (E_OK != retVal)
    {
      /* !LINKSTO SWS_SecOC_00228,1 */
      SecOC_Tx_CheckForCsmRetry(instId);
    }
  }
}

#if (SECOC_TX_CSM_ASYNC == STD_ON)
static FUNC(void, SECOC_CODE) SecOC_Tx_StateWaitForAuthenticationCallback(uint16 instId)
{
  /* Set the authentication procedure status */
  /* !LINKSTO SecOC_EB_Tx_xxx01,1 */
  SecOC_TxData[instId].ProcessingAuthentication = TRUE;

  if (SECOC_TXEVENT_AUTHCALLBACK_OK == SecOC_TxData[instId].TxAuthEvent)
  {
    /* Clear authentication result */
    SecOC_TxData[instId].TxAuthEvent = SECOC_TXEVENT_NONE;

    /* Trigger a transition from state Wait to ConstructSecuredPdu */
    SecOC_TxData[instId].TxState = SECOC_TX_STATE_CONSTRUCTSECUREDPDU;
  }
  else if (SECOC_TXEVENT_AUTHCALLBACK_NOT_OK == SecOC_TxData[instId].TxAuthEvent)
  {
    /* Clear authentication result */
    SecOC_TxData[instId].TxAuthEvent = SECOC_TXEVENT_NONE;

    /* !LINKSTO SWS_SecOC_00228,1 */
    SecOC_Tx_CheckForCsmRetry(instId);
  }
  else
  {
    /* Wait for the Csm callback */
    /* !LINKSTO SecOC_EB_Tx_xxx02,2 */
    SecOC_TxData[instId].ProcessingAuthentication = FALSE;
  }
}
#endif /* #if (SECOC_TX_CSM_ASYNC == STD_ON) */


static FUNC(void, SECOC_CODE) SecOC_Tx_StateConstructPdu(uint16 instId)
{
  if (SECOC_TXEVENT_CANCEL == SecOC_TxData[instId].TxPduREvent)
  {
    SecOC_Tx_ResetSm(instId);
  }
  else
  {
    /* Set the authentication procedure status */
    /* !LINKSTO SecOC_EB_Tx_xxx01,1 */
    SecOC_TxData[instId].ProcessingAuthentication = TRUE;

    /* Set the next execution state */
    SecOC_TxData[instId].TxState = SECOC_TX_STATE_BROADCASTSECUREDPDU;

#if (STD_ON == SECOC_BYPASS_AUTHENTICATION_ROUTINE)
    if (FALSE == SecOC_TxBypassAuthRoutineState)
#endif
    {
      /* Copy the (truncated) freshness value to the secured PDU */
      SecOC_Tx_CopyFreshnessToSecuredPdu(instId);

      /* Copy the (truncated) MAC to the secured PDU */
      SecOC_Tx_CopyMacToSecuredPdu(instId);
    }

#if (SECOC_TX_USE_MESS_LINKER == STD_ON)
    if (0U != SecOC_TxConfig[instId].TxMessageLinkLength)
    {
      /* !LINKSTO SWS_SecOC_Tx_00209,1 */
      SecOC_Tx_CopyMessageLinkToCryptoPdu(instId);

      SecOC_TxData[instId].TxSecuredPdu.SduLength =
        SecOC_TxData[instId].TxDataLength + SECOC_GET_TX_CRYPTO_PDU_LEN(instId);
    }
    else
#endif
    {
      SecOC_TxData[instId].TxSecuredPdu.SduLength =
        SecOC_TxData[instId].TxDataLength +
        (uint16)SECOC_REQUIREDBYTES(SECOC_TX_AUTHENTICATION_INFO_LEN(instId));
    }

    SecOC_TxData[instId].TxSecuredPdu.SduDataPtr = SecOC_TxData[instId].TxSecuredPduBuffer;

#if (SECOC_TX_SHAPE_SEC_PDU == STD_ON)
    SecOC_Tx_UpdateLayoutSecuredPdu(instId);
#endif
  }
}


static FUNC(void, SECOC_CODE) SecOC_Tx_StateBroadcastPdu(uint16 instId)
{
  /* CHECK: NOPARSE */
  if (SECOC_TXEVENT_CANCEL == SecOC_TxData[instId].TxPduREvent)
  {
    SecOC_Tx_ResetSm(instId);
  }
  /* CHECK: PARSE */
  else
  {
    Std_ReturnType retVal = E_NOT_OK;

    /* Set the next execution state */
    SecOC_TxData[instId].TxState = SECOC_TX_STATE_WAITFORCONFIRMATION;

#if (SECOC_BYPASS_AUTHENTICATION_ROUTINE == STD_ON)
    if (FALSE == SecOC_TxBypassAuthRoutineState)
#endif
    {
#if (STD_ON == SECOC_MACGENERATE_STATUS_PROP_ENABLED)
      SecOC_Tx_PropagateMacGenerateStatus(instId);
#endif
    }

#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
    /* !LINKSTO SWS_SecOC_Tx_00202,1 */
    if (TRUE == SecOC_TxConfig[instId].UseSecPduCollection)
    {
      /* Transmit the authentic and cryptographic PDUs to the lower layer */

      retVal = SecOC_Tx_BroadcastAuthenticPdu(instId);

      if (E_OK == retVal)
      {
        retVal = SecOC_Tx_BroadcastCryptoPdu(instId);
      }
    }
    else
#endif /* #if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON) */
    {
      SecOC_TxData[instId].CopiedDataFromSecLength = 0U;

      /* Broadcast secured Pdu */
#if (STD_ON == SECOC_TX_USETP)
      if (TRUE == SecOC_TxConfig[instId].UseTp)
      {
        /* !LINKSTO SWS_SecOC_00180,1, SWS_SecOC_Tx_00031,1, SWS_SecOC_Tx_00072,1 */
        retVal = PduR_SecOCTpTransmit
                             (
                               (PduIdType) SECOC_GET_TX_SEC_PDU_ID_AT_PDUR(instId),
                               &SecOC_TxData[instId].TxSecuredPdu
                             );
      }
      else
#endif /* if (STD_ON == SECOC_TX_USETP) */
      {
        /* !LINKSTO SWS_SecOC_00180,1, SWS_SecOC_Tx_00031,1, SWS_SecOC_Tx_00062,1,
                    SWS_SecOC_Tx_00067,2, SWS_SecOC_Tx_00137,4 */
        retVal = PduR_SecOCTransmit
                             (
                               (PduIdType) SECOC_GET_TX_SEC_PDU_ID_AT_PDUR(instId),
                               &SecOC_TxData[instId].TxSecuredPdu
                             );
      }
    }

    if (E_OK == retVal)
    {
      /* !LINKSTO SecOC.Dsn.interface.Tx.TxFreshnessManagement_FreshnessLengthZero,3 */
      if (0U != SECOC_GET_TX_FRESHNESS_LEN(instId))
      {
#if (SECOC_BYPASS_AUTHENTICATION_ROUTINE == STD_ON)
        if (FALSE == SecOC_TxBypassAuthRoutineState)
#endif
        {
        /* !LINKSTO SWS_SecOC_Tx_xx014,2 */
        /* !LINKSTO SWS_SecOC_Tx_xx015,2 */
#if (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_RTE)
          (void)SECOC_SPDUTXCONFIRMATION_FUNC(SECOC_GET_TX_FRESHNESS_ID(instId));
#elif (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_CFUNC)
          SECOC_SPDUTXCONFIRMATION_FUNC(SECOC_GET_TX_FRESHNESS_ID(instId));
#endif
        }
      }
    }
    else
    {
      /* !LINKSTO SWS_SecOC_Tx_xx011,1 */
      /* !LINKSTO SWS_SecOC_Tx_00078,1 */
      SecOC_Tx_ResetSm(instId);
    }
  }
}


static FUNC(void, SECOC_CODE) SecOC_Tx_StateWaitForConfirmation(uint16 instId)
{
  /* !LINKSTO SWS_SecOC_Tx_00063,1, SWS_SecOC_Tx_00064,1, SWS_SecOC_Tx_00075,1 */
  if (
      (SECOC_TXEVENT_TXCONFIRMATION_OK == SecOC_TxData[instId].TxPduREvent) ||
      (SECOC_TXEVENT_CANCEL            == SecOC_TxData[instId].TxPduREvent)
     )
  {
#if (SECOC_USE_TX_CONFIRMATION_TIMEOUT == STD_ON)
    SecOC_TxData[instId].TxConfirmationTimeoutCounter = 0U;
#endif
    SecOC_TxData[instId].TxState     = SECOC_TX_STATE_IDLE;
    SecOC_TxData[instId].TxPduREvent = SECOC_TXEVENT_NONE;
    /* !LINKSTO SecOC_EB_Tx_xxx03,1 */
    if (SecOC_TxData[instId].TxTransmitEvent == SECOC_TXEVENT_TRANSMIT)
    {
      SecOC_TxData[instId].ProcessingAuthentication = TRUE;
    }
  }
#if (SECOC_USE_TX_CONFIRMATION_TIMEOUT == STD_ON)
  else
  {
    if (SecOC_TxConfig[instId].TxConfirmationTimeoutThreshold > 0U)
    {
      SecOC_TxData[instId].TxConfirmationTimeoutCounter++;

      /* !LINKSTO SecOC_EB_Tx_xxx05,1 */
      if (
          SecOC_TxData[instId].TxConfirmationTimeoutCounter ==
          SecOC_TxConfig[instId].TxConfirmationTimeoutThreshold
         )
      {
        SecOC_TxData[instId].TxConfirmationTimeoutCounter = 0U;
        SecOC_Tx_ResetSm(instId);
      }
    }
  }
#endif /* #if (SECOC_USE_TX_CONFIRMATION_TIMEOUT == STD_ON) */
}

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

/*==================[helper functions]============================================================*/

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

static FUNC(void, SECOC_CODE) SecOC_Tx_ResetSm(uint16 instId)
{
  SecOC_TxData[instId].TxState     = SECOC_TX_STATE_IDLE;
  SecOC_TxData[instId].TxPduREvent = SECOC_TXEVENT_NONE;
  TS_MemBZero(SecOC_TxData[instId].TxSecuredPduBuffer, SECOC_GET_TX_SECURED_PDU_LEN(instId));
  /* !LINKSTO SecOC_EB_Tx_xxx02,2 */
  SecOC_TxData[instId].ProcessingAuthentication = FALSE;
  /* Set the buffer used flag to false, to be able to receive authentic PDUs */
#if (STD_ON == SECOC_TX_USE_SAME_BUFFER)
  SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
  SECOC_TX_SET_BUFFER_LVL2_STATUS(instId, SECOC_BUFFER_FREE);
  SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_1();
#endif
}


static FUNC(void, SECOC_CODE) SecOC_Tx_CheckForCsmRetry(uint16 instId)
{
  /* !LINKSTO SWS_SecOC_00228,1 */
  if (SecOC_TxConfig[instId].MaxAuthAttempts > SecOC_TxData[instId].AuthAttempts)
  {
    /* !LINKSTO SWS_SecOC_Tx_xx009,1 */
    SecOC_TxData[instId].AuthAttempts++;
    SecOC_TxData[instId].TxState = SECOC_TX_STATE_AUTHENTICATION;

#if (SECOC_TX_SYNC_PDU_PROCESSING == STD_ON)
    if (FALSE == SecOC_TxConfig[instId].UseTxSyncProcessing)
#endif /* #if (SECOC_TX_SYNC_PDU_PROCESSING == STD_ON) */
    {
      /* Set the authentication procedure status */
      /* !LINKSTO SecOC_EB_Tx_xxx02,2 */
      SecOC_TxData[instId].ProcessingAuthentication = FALSE;
    }
  }
#if (SECOC_USE_DEFAULT_AUTH == STD_OFF)
  else
  {
#if (STD_ON == SECOC_MACGENERATE_STATUS_PROP_ENABLED)
    SecOC_Tx_PropagateMacGenerateStatus(instId);
#endif

    /* !LINKSTO SWS_SecOC_Tx_00155,1 */
    /* !LINKSTO SWS_SecOC_Tx_xx011,1 */
    SecOC_Tx_ResetSm(instId);
  }
#else /* #if (SECOC_USE_DEFAULT_AUTH == STD_OFF) */
  else
  {
    /* !LINKSTO SecOC_EB_Tx_xxx17,1 */
    TS_MemSet
    (
      SecOC_TxData[instId].TxAuthenticator,
      SECOC_DEFAULT_AUTH_VALUE,
      SECOC_REQUIREDBYTES(SECOC_GET_TX_AUTHINFOTX_LEN(instId))
     );

    /* Set the next execution state */
    SecOC_TxData[instId].TxState = SECOC_TX_STATE_CONSTRUCTSECUREDPDU;
  }
#endif /* #if (SECOC_USE_DEFAULT_AUTH == STD_OFF) */
}


static FUNC(void, SECOC_CODE) SecOC_Tx_CopyFreshnessToSecuredPdu
(
  uint16 instId
)
{
#if (SECOC_TX_FRESHNESS_TRUNC_DATA == STD_ON)
  /* If the GetTxFreshnessTruncData is used then there is no need to move and shift
   * the freshness value to the secured PDU because this is done by this API
   */
  if (SECOC_GET_TX_FRESHNESS_TRUNCDATA_FUNC_TYPE != SecOC_TxConfig[instId].GetTxFreshnessFuncType)
#endif
  {
    const uint32 freshnessTxBits = SECOC_GET_TX_FRESHNESSTX_LEN(instId);

    /* least significant Bits of Freshness Value */
    /* !LINKSTO SWS_SecOC_Tx_00094,1 */
    if (0U < freshnessTxBits)
    {
      const uint32 freshnessDiffBits = (uint32)SECOC_GET_TX_FRESHNESS_LEN(instId) - freshnessTxBits;
      const uint32 freshnessDiffBytes = freshnessDiffBits / 8U;
      uint32 freshnessStartPos = 0U;

#if (STD_ON == SECOC_PDU_SECURED_AREA)
      if (TRUE == SECOC_GET_TX_USE_SECURED_AREA(instId))
      {
        freshnessStartPos = SECOC_DATAID_LENGTH +
                            (uint32) SECOC_GET_TX_SECURED_AREA_LEN(instId) +
                            freshnessDiffBytes;
      }
      else
#endif /* #if (STD_ON == SECOC_PDU_SECURED_AREA) */
      {
        freshnessStartPos = SECOC_DATAID_LENGTH +
                            (uint32) SecOC_TxData[instId].TxDataLength +
                            freshnessDiffBytes;
      }

      /* !LINKSTO SWS_SecOC_Tx_00037,1, SWS_SecOC_Tx_00061,1, SWS_SecOC_Tx_00066,1 */
      {
        P2VAR(uint8, AUTOMATIC, SECOC_VAR) da =
            &SecOC_TxData[instId].TxSecuredPduBuffer[SecOC_TxData[instId].TxDataLength];

        uint32 copyLen = SECOC_REQUIREDBYTES(freshnessTxBits);

        if (0U != (freshnessDiffBits % 8U))
        {
          /* FreshnessValueTxLength starts from Bit position 0 of a byte in the secured Pdu */
          SecOC_ShiftLeft
          (
            &SecOC_TxData[instId].TxDataToAuthenticator[freshnessStartPos],
            (SECOC_GET_TX_FRESHNESS_LEN(instId) - (freshnessDiffBytes * 8U)),
            (uint8) (freshnessDiffBits % 8U)
          );
        }

        TS_MemCpy
        (
          da,
          &SecOC_TxData[instId].TxDataToAuthenticator[freshnessStartPos],
          copyLen
        );

        if (0U != (freshnessTxBits % 8U))
        {
          da[copyLen - 1U] =
             (uint8)(da[copyLen- 1U] >> (8U - (freshnessTxBits % 8U)))
                                     << (8U - (freshnessTxBits % 8U));
        }
      }
    }
  }
}


static FUNC(void, SECOC_CODE) SecOC_Tx_CopyMacToSecuredPdu
(
  uint16 instId
)
{
  const uint32 freshnessRemainingBitsLen = (uint32)SECOC_GET_TX_FRESHNESSTX_LEN(instId) % 8U;
  const uint32 macStartPos = (uint32)(SecOC_TxData[instId].TxDataLength +
                                           ((uint32)SECOC_GET_TX_FRESHNESSTX_LEN(instId) / 8U));
  const uint32 authBitsLen = SECOC_GET_TX_AUTHINFOTX_LEN(instId);
  uint8 *authPtr = SecOC_TxData[instId].TxAuthenticator;
  uint8 *secPduPtr = SecOC_TxData[instId].TxSecuredPduBuffer;

  /* Clear unused bits from the authenticator (MAC) */
  authPtr[authBitsLen / 8U] &= 0xFFU << (8U - (authBitsLen % 8U));

  if (0U == freshnessRemainingBitsLen)
  {
    /* !LINKSTO SWS_SecOC_Tx_00031,1, SWS_SecOC_Tx_00037,1, SWS_SecOC_Tx_00061,1,
                SWS_SecOC_Tx_00066,1, SWS_SecOC_Tx_00071,1 */
    TS_MemCpy
    (
      &secPduPtr[macStartPos],
      authPtr,
      SECOC_REQUIREDBYTES(authBitsLen)
    );
  }
  else
  {
    /* !LINKSTO SWS_SecOC_Tx_00031,1, SWS_SecOC_Tx_00037,1, SWS_SecOC_Tx_00061,1,
                SWS_SecOC_Tx_00066,1, SWS_SecOC_Tx_00071,1 */
    /* construct the 'merged' byte (least bits of Freshness Value, First Bits of Authenticator) */
    secPduPtr[macStartPos] |= (authPtr[0U] >> freshnessRemainingBitsLen);

    if (authBitsLen > (8U - freshnessRemainingBitsLen))
    {
      /* !LINKSTO SWS_SecOC_Tx_00036,1 */
      /* The authenticator and therefore also AuthInfoTxLength starts from the LSB */
      SecOC_ShiftLeft
      (
        authPtr,
        (uint32)authBitsLen,
        (uint8) (8U - freshnessRemainingBitsLen)
      );

      TS_MemCpy
      (
        &secPduPtr[
                   SecOC_TxData[instId].TxDataLength +
                   SECOC_REQUIREDBYTES(SECOC_GET_TX_FRESHNESSTX_LEN(instId))
                  ],
        authPtr,
        SECOC_REQUIREDBYTES(authBitsLen - (8U - freshnessRemainingBitsLen))
      );
    }
  }
}

#if (SECOC_TX_SHAPE_SEC_PDU == STD_ON)
static FUNC(void, SECOC_CODE) SecOC_Tx_UpdateLayoutSecuredPdu(uint16 instId)
{
  if (TRUE == SecOC_TxConfig[instId].TxShapeSecuredPdu)
  {
    PduLengthType secPduLen = SECOC_GET_TX_SECURED_PDU_LEN(instId);
    PduLengthType authInfo;

#if (SECOC_TX_USE_MESS_LINKER == STD_ON)
    if (0U != SecOC_TxConfig[instId].TxMessageLinkLength)
    {
      authInfo = SECOC_GET_TX_CRYPTO_PDU_LEN(instId);
    }
    else
#endif
    {
      authInfo = (PduLengthType)SECOC_REQUIREDBYTES(SECOC_TX_AUTHENTICATION_INFO_LEN(instId));
    }

    /* Add the padding to the secured PDU */
    /* !LINKSTO SecOC_EB_Tx_xxx10,2 */
    SECOC_TX_SHAPE_FUNC
    (
      instId,
      SecOC_TxData[instId].TxSecuredPdu.SduDataPtr,
      &SecOC_TxData[instId].TxSecuredPdu.SduLength,
      &secPduLen,
      authInfo
    );

    SecOC_TxData[instId].TxSecuredPdu.SduLength = secPduLen;

    SecOC_TxData[instId].TxDataLength = SecOC_TxData[instId].TxSecuredPdu.SduLength - authInfo;
  }
}
#endif

#if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON)
static FUNC(Std_ReturnType, SECOC_CODE) SecOC_Tx_BroadcastAuthenticPdu(uint16 instId)
{
  Std_ReturnType retVal = E_NOT_OK;

#if (SECOC_TX_SHAPE_SEC_PDU == STD_ON)
  if (TRUE == SecOC_TxConfig[instId].TxShapeSecuredPdu)
  {
    SecOC_TxData[instId].TxSentAuthPdu.SduLength = SECOC_GET_TX_AUTH_PDU_LEN(instId);
  }
  else
#endif
  {
    SecOC_TxData[instId].TxSentAuthPdu.SduLength = SecOC_TxData[instId].TxDataLength;
  }

  SecOC_TxData[instId].TxSentAuthPdu.SduDataPtr = SecOC_TxData[instId].TxSecuredPduBuffer;

  SecOC_TxData[instId].CopiedDataFromAuthLength = 0U;

#if (STD_ON == SECOC_TX_USETP)
  if (TRUE == SecOC_TxConfig[instId].UseTp)
  {
    /* !LINKSTO SWS_SecOC_Tx_00201,1 */
    retVal = PduR_SecOCTpTransmit
             (
               (PduIdType) SECOC_GET_TX_SENT_AUTH_PDU_ID_AT_PDUR(instId),
               &SecOC_TxData[instId].TxSentAuthPdu
             );
  }
  else
#endif /* if (STD_ON == SECOC_TX_USETP) */
  {
    /* !LINKSTO SWS_SecOC_Tx_00201,1 */
    retVal = PduR_SecOCTransmit
             (
               (PduIdType) SECOC_GET_TX_SENT_AUTH_PDU_ID_AT_PDUR(instId),
               &SecOC_TxData[instId].TxSentAuthPdu
             );
  }

  return retVal;
}


static FUNC(Std_ReturnType, SECOC_CODE) SecOC_Tx_BroadcastCryptoPdu(uint16 instId)
{
  Std_ReturnType retVal = E_NOT_OK;

  SecOC_TxData[instId].TxCryptoPdu.SduLength = SECOC_GET_TX_CRYPTO_PDU_LEN(instId);

  SecOC_TxData[instId].TxCryptoPdu.SduDataPtr =
    &SecOC_TxData[instId].TxSecuredPduBuffer[SecOC_TxData[instId].TxDataLength];

  SecOC_TxData[instId].CopiedDataFromCryptoLength = 0U;

#if (STD_ON == SECOC_TX_USETP)
  if (TRUE == SecOC_TxConfig[instId].UseTp)
  {
    /* !LINKSTO SWS_SecOC_Tx_00201,1 */
    retVal = PduR_SecOCTpTransmit
             (
               (PduIdType) SECOC_GET_TX_CRYPTO_PDU_ID_AT_PDUR(instId),
               &SecOC_TxData[instId].TxCryptoPdu
             );
  }
  else
#endif /* if (STD_ON == SECOC_TX_USETP) */
  {
    /* !LINKSTO SWS_SecOC_Tx_00201,1 */
    retVal = PduR_SecOCTransmit
             (
               (PduIdType) SECOC_GET_TX_CRYPTO_PDU_ID_AT_PDUR(instId),
               &SecOC_TxData[instId].TxCryptoPdu
             );
  }

  return retVal;
}

#if (SECOC_TX_USE_MESS_LINKER == STD_ON)
static FUNC(void, SECOC_CODE) SecOC_Tx_CopyMessageLinkToCryptoPdu
(
  uint16 instId
)
{
  P2CONST(uint8, AUTOMATIC, SECOC_VAR) src = &SecOC_TxData[instId].TxSecuredPduBuffer[SecOC_TxConfig[instId].TxMessageLinkPos / 8U];
  P2VAR(uint8, AUTOMATIC, SECOC_VAR) dst = &SecOC_TxData[instId].TxSecuredPduBuffer[SecOC_TxData[instId].TxDataLength + (SECOC_TX_AUTHENTICATION_INFO_LEN(instId) / 8U)];
  uint32 size = SecOC_TxConfig[instId].TxMessageLinkLength;

  uint8 srcMaskBitsReq = 0x00U;
  uint8 srcDataMask = 0x00U;
  uint8 dstBitsSpace = 0x00U;

  uint8 srcBitPos = (uint8)(SecOC_TxConfig[instId].TxMessageLinkPos % 8U);
  uint32 srcBytePos = 0U;

  uint8 dstBitPos = (uint8)(SECOC_TX_AUTHENTICATION_INFO_LEN(instId) % 8U);
  uint32 dstBytePos = 0U;


  /* !LINKSTO SWS_SecOC_Tx_00210,1 */
  while (size > 0U)
  {
    /* Set the numbers of bits to be transferred this while cycle */
    if (size < ((uint32)8U - dstBitPos))
    {
      srcMaskBitsReq = (uint8)size;
    }
    else
    {
      srcMaskBitsReq = 8U - dstBitPos;
    }

    /* Clear unused bits from the source */
    srcDataMask = src[srcBytePos] << srcBitPos;

    /* Check if we must take other bits from the next byte in the source buffer */
    if (srcMaskBitsReq > (8U - srcBitPos))
    {
      srcDataMask |= (uint8)(src[srcBytePos + 1U] >> (8U - srcBitPos));
    }

    /* Check the current destination bit position is the start of the current destination byte */
    if (dstBitPos == 0U)
    {
      /* Check whether the number of bits to transfer is more than byte size */
      if (size >= 8U)
      {
        /* Transfer one full byte */
        dst[dstBytePos] = srcDataMask;
        /* Update the byte positions */
        srcBytePos++;
        dstBytePos++;

        /* Decrement the number of bits to be copied by 8 */
        size = size - 8U;
      }
      else
      {
        /* Transfer the required bits and clear the unused bits */
        dst[dstBytePos] = srcDataMask & (0xFFU << (8U - size));

        /* Nothing left to transfer */
        size = 0U;
      }
    }
    else
    {
      /* Count space in current destination byte to data storage */
      dstBitsSpace = 8U - dstBitPos;

      if (size <= dstBitsSpace)
      {
        /* Clear the bits that has to transfer in destination buffer */
        dst[dstBytePos] &= (uint8)(0xFFU << dstBitsSpace);
        /* Transfer the number of remaining bits to current destination */
        dst[dstBytePos] |= (uint8)(srcDataMask >> dstBitPos) & (0xFFU << (dstBitsSpace - size));
        /* Nothing left to transfer */
        size = 0U;
      }
      else
      {
        /* Clear the bits that has to transfer in destination buffer */
        dst[dstBytePos] &= (uint8)(0xFFU << (8U - dstBitPos));
        /* Fill the current byte of dst using srcDataMask*/
        dst[dstBytePos] |= (uint8)(srcDataMask >> dstBitPos);

        /* Update the source Byte and Bit Positions accordingly */
        if (dstBitsSpace > (8U - srcBitPos))
        {
          srcBytePos++;
          srcBitPos -= dstBitPos;
        }
        else if (dstBitsSpace < (8U - srcBitPos))
        {
          srcBitPos += dstBitsSpace;
        }
        else
        {
          srcBytePos++;
          srcBitPos = 0U;
        }

        /* Updated the Destination Byte and Bit Positions accordingly */
        dstBytePos++;
        dstBitPos = 0U;
        /* Decrement the number of bits to be copied by dstBitsSpace */
        size -= dstBitsSpace;
      }
    }
  }
}
#endif /* #if (SECOC_TX_USE_MESS_LINKER == STD_ON) */
#endif /* #if (SECOC_TX_USE_SECURED_PDU_COL == STD_ON) */


#if (STD_ON == SECOC_MACGENERATE_STATUS_PROP_ENABLED)
static FUNC(void, SECOC_CODE) SecOC_Tx_PropagateMacGenerateStatus
(
  uint16 instId
)
{
  /* Variable used for holding the MAC Generate status to propagate */
  SecOC_MacGenerateStatusType SecOC_TxMacGenerateStatus;
#if (SECOC_NUMBER_FUNC_MACGENERATE_STATUS_PROP > 0)
  /* Variable used for holding the index of the pointer to MAC Generate status callout function */
  uint32 calloutIdx;
#endif

  /* !LINKSTO SecOC_EB_Tx_xxx13,2 */
  if (
      (SECOC_STATUS_PROP_BOTH == SecOC_TxConfig[instId].MacGenerateStatusPropagationMode) ||
      (
        (SECOC_STATUS_PROP_FAILURE_ONLY == SecOC_TxConfig[instId].MacGenerateStatusPropagationMode) &&
        (E_OK                           != SecOC_TxData[instId].MacGenerateResult)
      )
     )
  {
    SecOC_TxMacGenerateStatus.freshnessValueID    = SECOC_GET_TX_FRESHNESS_ID(instId);
    SecOC_TxMacGenerateStatus.macGenerateStatus = SecOC_TxData[instId].MacGenerateResult;
    SecOC_TxMacGenerateStatus.secOCDataId         = SECOC_GET_TX_DATA_ID(instId);

#if (SECOC_NUMBER_FUNC_MACGENERATE_STATUS_PROP > 0)
    /* !LINKSTO SecOC_EB_Tx_xxx13,2 */
    for (calloutIdx = 0U; calloutIdx < SECOC_NUMBER_FUNC_MACGENERATE_STATUS_PROP; calloutIdx++)
    {
      SecOC_TxMacGenerateStatusCallout[calloutIdx](SecOC_TxMacGenerateStatus);
    }
#endif

    /* call the Rte, if configured */
#if (STD_ON == SECOC_INCLUDE_RTE)
    /* !LINKSTO SecOC.Dsn.interface.Tx.MacGenerateStatusCall,2 */
    (void) Rte_Write_PS_MacGenerateStatus_macGenerateStatus(&SecOC_TxMacGenerateStatus);
#endif
  }
}
#endif /* #if (STD_ON == SECOC_MACGENERATE_STATUS_PROP_ENABLED) */

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

/*==================[end of file]=================================================================*/
