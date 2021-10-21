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
/* !LINKSTO SecOC.Dsn.BasicSWStructure.Rx.SecOC_Rxc,1 */
#include <SecOC_Rx.h>

#if (STD_ON == SECOC_INCLUDE_RTE)
#include <Rte_SecOC.h>
#endif

/*==================[macros]======================================================================*/

#if (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_RTE)

#if (SECOC_RX_FRESHNESS == STD_ON)

/** \brief Macro, which defines the name of the callout function
 *         used to obtain the freshness value
 */
#if (defined SECOC_GET_RX_FRESHNESS_FUNC)
#error SECOC_GET_RX_FRESHNESS_FUNC already defined
#endif
#define SECOC_GET_RX_FRESHNESS_FUNC             Rte_Call_PS_RxFreshnessManagement_GetRxFreshness

#endif /* #if (SECOC_RX_FRESHNESS == STD_ON) */

#if (SECOC_RX_AUTH_DATA_FRESHNESS == STD_ON)

/** \brief Macro, which defines the name of the callout function
 *         used to obtain the freshness value
 */
#if (defined SECOC_GET_RX_FRESHNESS_AUTHDATA_FUNC)
#error SECOC_GET_RX_FRESHNESS_AUTHDATA_FUNC already defined
#endif
#define SECOC_GET_RX_FRESHNESS_AUTHDATA_FUNC    Rte_Call_PS_RxFreshnessManagement_GetRxFreshnessAuthData

#endif /* #if (SECOC_RX_AUTH_DATA_FRESHNESS == STD_ON) */

#elif (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_CFUNC)

#if (SECOC_RX_FRESHNESS == STD_ON)

/** \brief Macro, which defines the structure that holds the pointer to function
 *         used to obtain the freshness value
 */
#if (defined SECOC_GET_RX_FRESHNESS_FUNC)
#error SECOC_GET_RX_FRESHNESS_FUNC already defined
#endif
#define SECOC_GET_RX_FRESHNESS_FUNC             SecOC_RxConfig[instId].GetRxFreshness

#endif /* #if (SECOC_RX_FRESHNESS == STD_ON) */

#if (SECOC_RX_AUTH_DATA_FRESHNESS == STD_ON)

/** \brief Macro, which defines the structure that holds the pointer to function
 *         used to obtain the freshness value
 */
#if (defined SECOC_GET_RX_FRESHNESS_AUTHDATA_FUNC)
#error SECOC_GET_RX_FRESHNESS_AUTHDATA_FUNC already defined
#endif
#define SECOC_GET_RX_FRESHNESS_AUTHDATA_FUNC    SecOC_RxConfig[instId].GetRxFreshnessAuthData

#endif /* #if (SECOC_RX_AUTH_DATA_FRESHNESS == STD_ON) */

#endif /* #elif (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_CFUNC) */

/*==================[type definitions]============================================================*/

/*==================[internal function declarations]==============================================*/

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

/** \brief Idle state of the SecOC Rx state machine
 * \param[in] instId ID of the state machine instance (i.e. RX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Rx_StateIdle(uint16 instId);

/** \brief State of the SecOC Rx state machine, which constructs the data to authenticator
 * \param[in] instId ID of the state machine instance (i.e. RX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Rx_StateConstructDataToAuthenticator(uint16 instId);

/** \brief State of the SecOC Rx state machine, which starts a verification
 * \param[in] instId ID of the state machine instance (i.e. RX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Rx_StateVerification(uint16 instId);

#if (SECOC_RX_CSM_ASYNC == STD_ON)
/** \brief State of the SecOC Rx state machine, which waits
 *         for the callback and evaluates the result
 * \param[in] instId ID of the state machine instance (i.e. RX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Rx_StateWaitForVerificationCallback(uint16 instId);
#endif

/** \brief State of the SecOC Rx state machine which evaluates the verification result
 * \param[in] instId ID of the state machine instance (i.e. RX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Rx_StateCheckVerificationResult(uint16 instId);

/** \brief State of the SecOC Rx state machine which propagates the authenticated PDU
 *         to the upper layer
 * \param[in] instId ID of the state machine instance (i.e. RX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Rx_StateBroadcastPdu(uint16 instId);

/** \brief State of the SecOC Rx state machine which retries to obtain a new freshness value
 *         to retry to authenticate the secured PDU
 * \param[in] instId ID of the state machine instance (i.e. RX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Rx_StateRetryToObtainFreshness(uint16 instId);

#if (SECOC_RX_SHAPE_SEC_PDU == STD_ON)
/** \brief Function used to prepare and call the Rx side shaping function
 * \param[in] instId ID of the state machine instance (i.e. RX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Rx_CallShapeFunc(uint16 instId);
#endif

#if ((STD_ON == SECOC_RX_AUTH_DATA_FRESHNESS) && (SECOC_QUERY_FRESHNESS_VALUE != SECOC_FRESHNESS_NONE))
/** \brief Function used to extract the secured PDU part that will be sent to the freshness SWC
 * \param[in] instId ID of the state machine instance (i.e. RX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Rx_ExtractAuthDataFreshnessValue(uint16 instId);
#endif

/** \brief Function used to check the retVal of the query for freshness
 * \param[in] instId ID of the state machine instance (i.e. RX PDU)
 * \param[in] retVal The return value of the freshness value SWC APIs
 */
static FUNC(void, SECOC_CODE) SecOC_Rx_CheckFreshnessFuncResult
(
  uint16         instId,
  Std_ReturnType retVal
);

/** \brief Function used to renew the verify status override
 * \param[in]     instId    ID of the state machine instance (i.e. RX PDU)
 * \param[in,out] passData  verification result
 */
static FUNC(void, SECOC_CODE) SecOC_Rx_RenewVerifyStatusOverride
(
  uint16 instId,
  P2VAR(boolean, AUTOMATIC, SECOC_APPL_DATA) passData
);

#if (SECOC_VERIFICATION_STATUS_PROP_NONE != SECOC_VERIFICATION_STATUS_PROP_ENABLED)
/** \brief Function which propagates the verification attempt success to the configured callout
 *         functions
 * \param[in] instId ID of the state machine instance (i.e. RX PDU)
 *
 */
static FUNC(void, SECOC_CODE) SecOC_Rx_PropagateVerificationStatus
(
  uint16  instId
);
#endif

/** \brief Function used to handle the verification retry
 * \param[in] instId ID of the state machine instance (i.e. RX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Rx_CheckForCsmRetry(uint16 instId);

/** \brief Resets the Rx state machine and frees the buffers
 * \param[in] instId ID of the state machine instance (i.e. RX PDU)
 */
static FUNC(void, SECOC_CODE) SecOC_Rx_ResetSm(uint16 instId);

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

/*==================[internal constants]==========================================================*/

#define SECOC_START_SEC_CONST_UNSPECIFIED
#include <SecOC_MemMap.h>

/**
 * \brief Variable which holds the state of the SecOC Rx state machine
 */
/* Deviation MISRAC2012-1 */
static CONST(SecOC_SmStateType, SECOC_CONST) SecOC_RxStateMachine[SECOC_NUMBER_OF_RXSTATES] =
{
  &SecOC_Rx_StateIdle,
  &SecOC_Rx_StateConstructDataToAuthenticator,
  &SecOC_Rx_StateVerification,
  &SecOC_Rx_StateCheckVerificationResult,
  &SecOC_Rx_StateBroadcastPdu,
  &SecOC_Rx_StateRetryToObtainFreshness,
#if (SECOC_RX_CSM_ASYNC == STD_ON)
  &SecOC_Rx_StateWaitForVerificationCallback
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

FUNC(void, SECOC_CODE) SecOC_Rx_ProcessPduStates
(
  uint16 pduIdx
)
{
  /* !LINKSTO SecOC_EB_Rx_xxx20,1 */
  do
  {
    SecOC_RxData[pduIdx].ProcessingVerification = FALSE;

    SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();
    SecOC_RxStateMachine[SecOC_RxData[pduIdx].RxState](pduIdx);
    SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();
  }
  while (TRUE == SecOC_RxData[pduIdx].ProcessingVerification);
}

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

/*==================[internal function definitions]===============================================*/

/*==================[state machine states]========================================================*/

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

/* !LINKSTO SecOC_EB_Rx_xxx16,1 */
static FUNC(void, SECOC_CODE) SecOC_Rx_StateIdle(uint16 instId)
{
  /* !LINKSTO SWS_SecOC_Rx_00085,1, SWS_SecOC_Rx_00203,1 */
  if (SECOC_RXEVENT_RXINDICATION == SecOC_RxData[instId].RxPduREvent)
  {
    /* Set the verification procedure status */
    /* !LINKSTO SecOC_EB_Rx_xxx01,1 */
    SecOC_RxData[instId].ProcessingVerification = TRUE;

    /* the Pdu is now processed and the buffers in use */
    SecOC_RxData[instId].RxPduInProcess = TRUE;
    SecOC_RxData[instId].RxPduREvent = SECOC_RXEVENT_NONE;

#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
    /* Check if SecuredPduCollection is configured for this PDU */
    if (SecOC_RxConfig[instId].UseSecPduCollection == TRUE)
    {
      /* !LINKSTO SWS_SecOC_Rx_00206,1 */
      /* Reconstruct the secured PDU from the received authentic and cryptographic PDUs */
      TS_MemCpy
      (
        SecOC_RxData[instId].RxSecuredPduBuffer,
        SecOC_RxData[instId].RxReceivedAuthPduBuffer,
        SecOC_RxData[instId].ReceivedTpAuthDataLength
      );

      TS_MemCpy
      (
        &SecOC_RxData[instId].RxSecuredPduBuffer[SecOC_RxData[instId].ReceivedTpAuthDataLength],
        SecOC_RxData[instId].RxCryptoPduBuffer,
        SECOC_GET_RX_CRYPTO_PDU_LEN(instId)
      );

      SecOC_RxData[instId].RxSecuredDataLength = SecOC_RxData[instId].ReceivedTpAuthDataLength +
                                                 SECOC_GET_RX_CRYPTO_PDU_LEN(instId);
    }
    else
#endif
    {
      if (SECOC_REPLACE_CURRENT_PDU == SecOC_RxConfig[instId].ReceptionStrategy)
      {
        TS_MemCpy
        (
          SecOC_RxData[instId].RxSecuredPduBuffer,
          SecOC_RxData[instId].RxReceivedSecuredPduBuffer,
          SecOC_RxData[instId].ReceivedTpSecuredDataLength
        );
      }

      SecOC_RxData[instId].RxSecuredDataLength = SecOC_RxData[instId].ReceivedTpSecuredDataLength;
    }

#if (SECOC_RX_SHAPE_SEC_PDU == STD_ON)
    SecOC_Rx_CallShapeFunc(instId);
#endif

#if (SECOC_RX_USE_MESS_LINKER == STD_ON)
    if (SecOC_RxConfig[instId].RxMessageLinkLength != 0U)
    {
      /* !LINKSTO SecOC_EB_Rx_xxx04,1 */
      SecOC_RxData[instId].RxDataLength = SecOC_RxData[instId].RxSecuredDataLength -
                                          SECOC_GET_RX_CRYPTO_PDU_LEN(instId);
    }
    else
#endif
    {
      /* !LINKSTO SecOC_EB_Rx_xxx04,1 */
      SecOC_RxData[instId].RxDataLength = SecOC_RxData[instId].RxSecuredDataLength -
                                          (uint16)SECOC_REQUIREDBYTES
                                            (SECOC_RX_AUTHENTICATION_INFO_LEN(instId));
    }
    /* !LINKSTO SWS_SecOC_Rx_00040,1, SWS_SecOC_Rx_00042,1 */
    /* !LINKSTO SWS_SecOC_Rx_00265,1 */
#if (SECOC_SKIP_RX_PDU_VERIFICATION == STD_OFF)
    if (SECOC_OVERRIDESTATUS_SKIP_UNTIL_NOTICE == SecOC_RxData[instId].VerifyStatusOverride)
#else
    if (
        (SECOC_OVERRIDESTATUS_SKIP_UNTIL_NOTICE == SecOC_RxData[instId].VerifyStatusOverride) ||
        (TRUE                                   == SecOC_RxConfig[instId].SkipVerification)
       )
#endif
    {
      SecOC_RxData[instId].CryptoVerificationResult = CRYPTO_E_VER_OK;
      SecOC_RxData[instId].RxState     = SECOC_RX_STATE_CHECKVERIFICATIONRESULT;
    }
    else
    {
      /* next state. if everything is ok */
      SecOC_RxData[instId].RxState = SECOC_RX_STATE_CONSTRUCTDATATOAUTHENTICATOR;
    }
  }
}


static FUNC(void, SECOC_CODE) SecOC_Rx_StateConstructDataToAuthenticator(uint16 instId)
{
  if (
      (SECOC_RXEVENT_RXINDICATION   == SecOC_RxData[instId].RxPduREvent) ||
      (SECOC_RXEVENT_STARTRECEPTION == SecOC_RxData[instId].RxPduREvent)
     )
  {
    SecOC_RxData[instId].RxState = SECOC_RX_STATE_IDLE;
    SecOC_RxData[instId].RxPduInProcess = FALSE;
    /* !LINKSTO SWS_SecOC_00235,1 */
    SecOC_RxData[instId].AuthAttempts = 0U;
    /* Set the verification procedure status */
    SecOC_RxData[instId].ProcessingVerification = TRUE;
  }
  else
  {
    Std_ReturnType retVal = SECOC_E_NOT_OK;

    if(SecOC_RxData[instId].AuthAttempts == 0U)
    {
      /* !LINKSTO SWS_SecOC_RxTx_00011,1, SWS_SecOC_Rx_00046_1,1,
                  SWS_SecOC_Rx_00046_2,1, SWS_SecOC_Rx_00040,1,
                  SecOC_EB_RxTx_xxx03,1*/

      /* DataID loop */
      {
        uint8 index;
        for (index = 0U; index < SECOC_DATAID_LENGTH; index++)
        {
          SecOC_RxData[instId].RxDataToAuthenticator[SECOC_DATAID_LENGTH - index - 1U] =
          (uint8) (SECOC_GET_RX_DATA_ID(instId) >> (index * 8U));
        }
      }
      /* !LINKSTO SWS_SecOC_00311,1, SWS_SecOC_00312,1 */
      /* Copy the Secured authentic part from the secured PDU */
#if (STD_ON == SECOC_PDU_SECURED_AREA)
      if (TRUE == SECOC_GET_RX_USE_SECURED_AREA(instId))
      {
        TS_MemCpy
        (
          &SecOC_RxData[instId].RxDataToAuthenticator[SECOC_DATAID_LENGTH],
          &SecOC_RxData[instId].RxSecuredPduBuffer[SECOC_GET_RX_SECURED_AREA_OFFSET(instId)],
          SECOC_GET_RX_SECURED_AREA_LEN(instId)
        );
      }
      else
#endif /* #if (STD_ON == SECOC_PDU_SECURED_AREA) */
      {
        TS_MemCpy
        (
          &SecOC_RxData[instId].RxDataToAuthenticator[SECOC_DATAID_LENGTH],
          SecOC_RxData[instId].RxSecuredPduBuffer,
          SecOC_RxData[instId].RxDataLength
        );
      }

      /* !LINKSTO SWS_SecOC_00235,1 */
      SecOC_RxData[instId].FreshnessValueSyncAttempts = 0U;

      /* The data length is dynamic, therefore the start position of the RxFreshnessVerifyValue
       * changes also.
       */
#if ((STD_ON == SECOC_RX_AUTH_DATA_FRESHNESS) && (SECOC_QUERY_FRESHNESS_VALUE != SECOC_FRESHNESS_RTE))
      if (0U != SECOC_GET_RX_FRESHNESS_LEN(instId))
      {
#endif

#if (STD_ON == SECOC_PDU_SECURED_AREA)
        if (TRUE == SECOC_GET_RX_USE_SECURED_AREA(instId))
        {
          SecOC_RxData[instId].RxFreshnessVerifyValue =
            &(SecOC_RxData[instId].RxDataToAuthenticator)[
                                                          SECOC_DATAID_LENGTH               +
                                                          SECOC_GET_RX_SECURED_AREA_LEN(instId)
                                                         ];
        }
        else
#endif /* #if (STD_ON == SECOC_PDU_SECURED_AREA) */
        {
          SecOC_RxData[instId].RxFreshnessVerifyValue =
            &(SecOC_RxData[instId].RxDataToAuthenticator)[
                                                          SECOC_DATAID_LENGTH               +
                                                          SecOC_RxData[instId].RxDataLength
                                                         ];
        }
#if ((STD_ON == SECOC_RX_AUTH_DATA_FRESHNESS) && (SECOC_QUERY_FRESHNESS_VALUE != SECOC_FRESHNESS_RTE))
      }
      else
      {
        SecOC_RxData[instId].RxFreshnessVerifyValue = NULL_PTR;
      }
#endif
    }

#if (SECOC_QUERY_FRESHNESS_VALUE != SECOC_FRESHNESS_NONE)
    /* !LINKSTO SecOC.Dsn.interface.Rx.RxFreshnessManagement_FreshnessLengthZero, 3,
                SecOC.Dsn.interface.Rx.RxFreshnessManagement_FreshnessLengthZero_2, 1 */
#if (STD_ON == SECOC_RX_AUTH_DATA_FRESHNESS)
    if (
        (0U != SECOC_GET_RX_FRESHNESS_LEN(instId)                 ) ||
        (0U != SecOC_RxConfig[instId].AuthDataFreshnessValueLength)
       )
#else
    if (0U != SECOC_GET_RX_FRESHNESS_LEN(instId))
#endif
    {
      uint32 freshnessValueLength = SECOC_GET_RX_FRESHNESS_LEN(instId);

#if (STD_ON == SECOC_RX_AUTH_DATA_FRESHNESS)
      /* Extract the secured PDU part that will be sent to the freshness SWC */
      SecOC_Rx_ExtractAuthDataFreshnessValue(instId);
#endif
      /* !LINKSTO SWS_SecOC_00220,1 */
      switch(SecOC_RxConfig[instId].GetRxFreshnessFuncType)
      {
#if (STD_ON == SECOC_RX_FRESHNESS)
        case SECOC_GET_RX_FRESHNESS_FUNC_TYPE:
        {
          SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();
          /* !LINKSTO SWS_SecOC_Rx_xx023,3 */
          /* !LINKSTO SWS_SecOC_Rx_xx025,3 */
          retVal = SECOC_GET_RX_FRESHNESS_FUNC
                   (
                     SECOC_GET_RX_FRESHNESS_ID(instId),
                     &SecOC_RxData[instId].RxSecuredPduBuffer[SecOC_RxData[instId].RxDataLength],
                     SECOC_GET_RX_FRESHNESSTX_LEN(instId),
                     SecOC_RxData[instId].FreshnessValueSyncAttempts,
                     SecOC_RxData[instId].RxFreshnessVerifyValue,
                     &freshnessValueLength
                   );
          SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();
        }
        break;
#endif /* (STD_ON == SECOC_RX_FRESHNESS) */
#if (STD_ON == SECOC_RX_AUTH_DATA_FRESHNESS)
        case SECOC_GET_RX_FRESHNESS_AUTHDATA_FUNC_TYPE:
        {
          SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();
          /* !LINKSTO SWS_SecOC_Rx_00219,1 */
          /* !LINKSTO SWS_SecOC_Rx_00244,2 */
          /* !LINKSTO SWS_SecOC_Rx_00246,2 */
          retVal = SECOC_GET_RX_FRESHNESS_AUTHDATA_FUNC
                   (
                     SECOC_GET_RX_FRESHNESS_ID(instId),
                     &SecOC_RxData[instId].RxSecuredPduBuffer[SecOC_RxData[instId].RxDataLength],
                     SECOC_GET_RX_FRESHNESSTX_LEN(instId),
                     SecOC_RxData[instId].AuthDataFreshnessValueBuffer,
                     SecOC_RxConfig[instId].AuthDataFreshnessValueLength,
                     SecOC_RxData[instId].FreshnessValueSyncAttempts,
                     SecOC_RxData[instId].RxFreshnessVerifyValue,
                     &freshnessValueLength
                   );
        SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();
        }
        break;
#endif /* (STD_ON == SECOC_RX_AUTH_DATA_FRESHNESS) */
        /* CHECK: NOPARSE */
        default:
        {
          /* Nothing to do. */
        }
        break;
        /* CHECK: PARSE */
      }
    }
    else
#endif /* #if (SECOC_QUERY_FRESHNESS_VALUE == SECOC_FRESHNESS_NONE) */
    {
      retVal = SECOC_E_OK;
    }

    SecOC_Rx_CheckFreshnessFuncResult(instId, retVal);
  }
}


static FUNC(void, SECOC_CODE) SecOC_Rx_StateVerification(uint16 instId)
{
  if (
      (SECOC_RXEVENT_RXINDICATION   == SecOC_RxData[instId].RxPduREvent) ||
      (SECOC_RXEVENT_STARTRECEPTION == SecOC_RxData[instId].RxPduREvent)
     )
  {
    SecOC_RxData[instId].RxState = SECOC_RX_STATE_IDLE;
    SecOC_RxData[instId].RxPduInProcess = FALSE;
    /* !LINKSTO SWS_SecOC_00235,1 */
    SecOC_RxData[instId].AuthAttempts = 0U;
    /* Set the verification procedure status */
    SecOC_RxData[instId].ProcessingVerification = TRUE;
  }
  else
  {
    Std_ReturnType retVal = E_NOT_OK;
    uint32 dataLength;

    /* Length of the authentic data */
#if (STD_ON == SECOC_PDU_SECURED_AREA)
    if (TRUE == SECOC_GET_RX_USE_SECURED_AREA(instId))
    {
      dataLength =
      (
       (uint32)SECOC_GET_RX_SECURED_AREA_LEN(instId)                    +
       (uint32)SECOC_DATAID_LENGTH                                      +
       SECOC_REQUIREDBYTES(SECOC_GET_RX_FRESHNESS_LEN(instId))
      );
    }
    else
#endif /* #if (STD_ON == SECOC_PDU_SECURED_AREA) */
    {
      dataLength =
      (
       (uint32)SecOC_RxData[instId].RxDataLength                        +
       (uint32)SECOC_DATAID_LENGTH                                      +
       SECOC_REQUIREDBYTES(SECOC_GET_RX_FRESHNESS_LEN(instId))
      );
    }

#if (SECOC_RX_CSM_ASYNC == STD_ON)
    if (TRUE == SecOC_RxConfig[instId].UseCsmAsync)
    {
      /* Set the next execution state */
      SecOC_RxData[instId].RxState = SECOC_RX_STATE_WAITFORVERIFICATIONCALLBACK;
      /* Prepare the Csm callback event flag */
      SecOC_RxData[instId].RxVerEvent = SECOC_RXEVENT_NONE;
    }
    else
    {
      /* Set the next execution state */
      SecOC_RxData[instId].RxState = SECOC_RX_STATE_CHECKVERIFICATIONRESULT;
    }
#else
    /* Set the next execution state */
    SecOC_RxData[instId].RxState = SECOC_RX_STATE_CHECKVERIFICATIONRESULT;
#endif /* #if (SECOC_RX_CSM_ASYNC == STD_ON) */

    /* Set the verification procedure status */
    /* !LINKSTO SecOC_EB_Rx_xxx01,1 */
    SecOC_RxData[instId].ProcessingVerification = TRUE;

    SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();

    /* !LINKSTO SWS_SecOC_Rx_00040,1, SWS_SecOC_Rx_00047,2, SWS_SecOC_Rx_00079,1,
                SecOC.Dsn.interface.RxTx.SecOC_CsmSingleCalls,1 */
    retVal = Csm_MacVerify
             (
              (Csm_ConfigIdType)SECOC_RX_CSM_JOB_ID(instId),
              CRYPTO_OPERATIONMODE_SINGLECALL,
              SecOC_RxData[instId].RxDataToAuthenticator,
              dataLength,
              SecOC_RxData[instId].RxAuthenticator,
              /* !LINKSTO SWS_SecOC_EB_RxTx_00003,1 */
#if (SECOC_AUTH_LENGTH_BITS == STD_ON)
              SECOC_GET_RX_AUTHINFOTX_LEN(instId),
#else
              SECOC_REQUIREDBYTES(SECOC_GET_RX_AUTHINFOTX_LEN(instId)),
#endif
              /* !LINKSTO SWS_SecOC_00242,1 */
              &SecOC_RxData[instId].CryptoVerificationResult
             );

    SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();

#if (SECOC_VERIFICATION_STATUS_PROP_EB == SECOC_VERIFICATION_STATUS_PROP_ENABLED)
    SecOC_RxData[instId].VerificationReturn = retVal;
#endif

    /* !LINKSTO SWS_SecOC_Tx_00151,1 */
    if (E_OK != retVal)
    {
#if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_ON)
      boolean pass = FALSE;

      /* Check if the verification status can be overridden to pass */
      if (SECOC_OVERRIDESTATUS_PASS_MSG_NUMBER == SecOC_RxData[instId].VerifyStatusOverride)
      {
        /* Try to override the verification status */
        SecOC_Rx_RenewVerifyStatusOverride(instId, &pass);
      }

      if (TRUE == pass)
      {
        SecOC_RxData[instId].CryptoVerificationResult = CRYPTO_E_VER_OK;

        /* Set the next execution state as the verification status was successfully overridden */
        SecOC_RxData[instId].RxState = SECOC_RX_STATE_CHECKVERIFICATIONRESULT;
      }
      else
#endif
      {
        /* !LINKSTO SWS_SecOC_00238,1 */
        SecOC_Rx_CheckForCsmRetry(instId);
      }
    }
  }
}

#if (SECOC_RX_CSM_ASYNC == STD_ON)
static FUNC(void, SECOC_CODE) SecOC_Rx_StateWaitForVerificationCallback(uint16 instId)
{
  /* Set the verification procedure status */
  /* !LINKSTO SecOC_EB_Rx_xxx01,1 */
  SecOC_RxData[instId].ProcessingVerification = TRUE;

  if (SECOC_RXEVENT_VERCALLBACK_OK == SecOC_RxData[instId].RxVerEvent)
  {
    /* Clear verification result */
    SecOC_RxData[instId].RxVerEvent = SECOC_RXEVENT_NONE;

    /* Trigger a transition from state Wait to VerificationResult */
    SecOC_RxData[instId].RxState = SECOC_RX_STATE_CHECKVERIFICATIONRESULT;
  }
  else if (SECOC_RXEVENT_VERCALLBACK_NOT_OK == SecOC_RxData[instId].RxVerEvent)
  {
    /* Clear verification result */
    SecOC_RxData[instId].RxVerEvent = SECOC_RXEVENT_NONE;

#if (SECOC_IGNORE_VER_RESULT == STD_ON)
    /* Set the verification result to a negative one */
    SecOC_RxData[instId].CryptoVerificationResult = CRYPTO_E_VER_NOT_OK;

    /* Trigger a transition to the next state even if the verification result is a negative one */
    SecOC_RxData[instId].RxState = SECOC_RX_STATE_CHECKVERIFICATIONRESULT;
#else
    {
#if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_ON)
      boolean pass = FALSE;

      /* Check if the verification status can be overridden to pass */
      if (SECOC_OVERRIDESTATUS_PASS_MSG_NUMBER == SecOC_RxData[instId].VerifyStatusOverride)
      {
        /* Try to override the verification status */
        SecOC_Rx_RenewVerifyStatusOverride(instId, &pass);
      }

      /* Check if the verification status was overridden */
      if (TRUE == pass)
      {
        SecOC_RxData[instId].CryptoVerificationResult = CRYPTO_E_VER_OK;

        /* Proceed with the execution */
        SecOC_RxData[instId].RxState = SECOC_RX_STATE_CHECKVERIFICATIONRESULT;
      }
      else
#endif
      {
        /* !LINKSTO SWS_SecOC_00238,1 */
        SecOC_Rx_CheckForCsmRetry(instId);
      }
    }
#endif /* #if (SECOC_IGNORE_VER_RESULT == STD_ON) */
  }
  else
  {
    /* Wait for the Csm callback */
    /* !LINKSTO SecOC_EB_Rx_xxx02,2 */
    SecOC_RxData[instId].ProcessingVerification = FALSE;
  }
}
#endif /* #if (SECOC_RX_CSM_ASYNC == STD_ON) */

static FUNC(void, SECOC_CODE) SecOC_Rx_StateCheckVerificationResult(uint16 instId)
{
  if (
      (SECOC_RXEVENT_RXINDICATION   == SecOC_RxData[instId].RxPduREvent) ||
      (SECOC_RXEVENT_STARTRECEPTION == SecOC_RxData[instId].RxPduREvent)
     )
  {
    SecOC_RxData[instId].RxState = SECOC_RX_STATE_IDLE;
    SecOC_RxData[instId].RxPduInProcess = FALSE;
    /* !LINKSTO SWS_SecOC_00235,1 */
    SecOC_RxData[instId].AuthAttempts = 0U;
    /* Set the verification procedure status */
    SecOC_RxData[instId].ProcessingVerification = TRUE;
  }
  else
  {
    boolean passData = FALSE;

    if (SecOC_RxData[instId].CryptoVerificationResult == CRYPTO_E_VER_OK)
    {
      SecOC_RxData[instId].VerificationResult = SECOC_VERIFICATIONSUCCESS;
    }
    else
    {
      SecOC_RxData[instId].VerificationResult = SECOC_VERIFICATIONFAILURE;
    }

#if (SECOC_VERIFICATION_STATUS_PROP_EB == SECOC_VERIFICATION_STATUS_PROP_ENABLED)
    SecOC_RxData[instId].VerificationReturn = E_OK;
#endif

#if (SECOC_IGNORE_VER_RESULT == STD_ON)
    /* !LINKSTO SWS_SecOC_Rx_00081,1 */
    passData = TRUE;
#else
    if (SECOC_VERIFICATIONSUCCESS == SecOC_RxData[instId].VerificationResult)
    {
      passData = TRUE;
    }
#endif /* #if (SECOC_IGNORE_VER_RESULT == STD_OFF) */

#if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_ON)
    /* Check that the verification status was not renewed already */
    if (SecOC_RxData[instId].RenewedVerStatus == FALSE)
#endif
    {
      SecOC_Rx_RenewVerifyStatusOverride(instId, &passData);
    }

#if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_ON)
    /* Set the flag back to FALSE for the next secured PDU that will be received */
    SecOC_RxData[instId].RenewedVerStatus = FALSE;

    if (SECOC_OVERRIDESTATUS_SKIP_UNTIL_NOTICE == SecOC_RxData[instId].VerifyStatusOverride)
    {
      SecOC_RxData[instId].VerificationResult = SECOC_NO_VERIFICATION;
    }
#endif

    /* !LINKSTO SWS_SecOC_Rx_00048_2,1 */
#if (SECOC_VERIFICATION_STATUS_PROP_NONE != SECOC_VERIFICATION_STATUS_PROP_ENABLED)
#if (SECOC_SKIP_RX_PDU_VERIFICATION == STD_ON)
    if (FALSE == SecOC_RxConfig[instId].SkipVerification)
#endif
    {
      /* Report authentication status */
      /* !LINKSTO SWS_SecOC_Rx_00048_1,1 */
      SecOC_Rx_PropagateVerificationStatus(instId);
    }
#endif /* #if (SECOC_VERIFICATION_STATUS_PROP_NONE != SECOC_VERIFICATION_STATUS_PROP_ENABLED) */

    if (TRUE == passData)
    {
      SecOC_RxData[instId].ProcessingVerification = TRUE;
      SecOC_RxData[instId].RxState = SECOC_RX_STATE_BROADCASTPDU;
    }
    else
    {
      /* Data authentication failed. -> Try again if Max attempts not reached */
      /* !LINKSTO SecOC.Dsn.interface.Rx.RxFreshnessManagement_MacVerFailed_FreshnessLengthZero,1 */
      if (
          (SecOC_RxConfig[instId].FreshnessValueMaxSyncAttempts >
           SecOC_RxData[instId].FreshnessValueSyncAttempts
          ) &&
          (SECOC_OVERRIDESTATUS_CANCEL == SecOC_RxData[instId].VerifyStatusOverride)
            &&
          (0U != SECOC_GET_RX_FRESHNESS_LEN(instId))
         )
      {
        SecOC_RxData[instId].ProcessingVerification = TRUE;
        SecOC_RxData[instId].RxState = SECOC_RX_STATE_RETRYTOOBTAINFRESHNESS;
      }
      else
      {
#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
        /* Verification of the secured PDU failed, prepare the state machine to restart the procedure
         * immediately after receiving new data for the authentic or cryptographic PDU
         */
        /* !LINKSTO SWS_SecOC_Rx_00208,1 */
        SecOC_RxData[instId].RxPduRIndicationForAuth   = SECOC_RXEVENT_RXINDICATION;
        SecOC_RxData[instId].RxPduRIndicationForCrypto = SECOC_RXEVENT_RXINDICATION;
#endif

        /* Drop Message */
        /* !LINKSTO SWS_SecOC_Rx_00087_2,1 */
        /* !LINKSTO SWS_SecOC_Rx_xx039,1 */
        SecOC_Rx_ResetSm(instId);
      }
    }
  }
}


static FUNC(void, SECOC_CODE) SecOC_Rx_StateBroadcastPdu(uint16 instId)
{
  /* CHECK: NOPARSE */
  /* It is not possible to stop here the state machine execution */
  if (
      (SECOC_RXEVENT_RXINDICATION   == SecOC_RxData[instId].RxPduREvent) ||
      (SECOC_RXEVENT_STARTRECEPTION == SecOC_RxData[instId].RxPduREvent)
     )
  {
    SecOC_RxData[instId].RxState = SECOC_RX_STATE_IDLE;
    SecOC_RxData[instId].RxPduInProcess = FALSE;
    /* !LINKSTO SWS_SecOC_00235,1 */
    SecOC_RxData[instId].AuthAttempts = 0U;
    /* Set the verification procedure status */
    SecOC_RxData[instId].ProcessingVerification = TRUE;
  }
  /* CHECK: PARSE */
  else
  {
    PduInfoType SecOC_RxPduInfo;

    SecOC_RxPduInfo.SduLength  = SecOC_RxData[instId].RxDataLength;
    SecOC_RxPduInfo.SduDataPtr = SecOC_RxData[instId].RxSecuredPduBuffer;

    SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();

    /* Pass Data to 'upper layer' */
      /* !LINKSTO SWS_SecOC_Rx_00040,1, SWS_SecOC_Rx_00050,1, SWS_SecOC_Rx_00080,1 */
      /* !LINKSTO SWS_SecOC_Rx_00086,2, SWS_SecOC_00175,1, SWS_SecOC_Rx_00137,2    */
      /* !LINKSTO  SWS_SecOC_Rx_00XX1,1, SWS_SecOC_Rx_00088,2                      */
#if (STD_ON == SECOC_RX_USETP)
    if (TRUE == SecOC_RxConfig[instId].UseTp)
    {
      if(
          BUFREQ_OK ==
          PduR_SecOCTpCopyRxData(
                                  SECOC_GET_RX_AUTH_PDU_ID_AT_PDUR(instId),
                                  &SecOC_RxPduInfo,
                                  &SecOC_RxPduInfo.SduLength
                                )
        )
      {
        PduR_SecOCTpRxIndication(SECOC_GET_RX_AUTH_PDU_ID_AT_PDUR(instId), E_OK);
      }
      else
      {
        PduR_SecOCTpRxIndication(SECOC_GET_RX_AUTH_PDU_ID_AT_PDUR(instId), E_NOT_OK);
      }

      SecOC_RxData[instId].UpperLayerCallFlag = FALSE;
    }
    else
#endif
    {
      /* !LINKSTO SWS_SecOC_Rx_00081,1 */
      PduR_SecOCRxIndication(SECOC_GET_RX_AUTH_PDU_ID_AT_PDUR(instId), &SecOC_RxPduInfo);
    }

    SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();

#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
    /* Clear the internal buffers where the authentic and cryptographic PDUs are stored */
    if (TRUE == SecOC_RxConfig[instId].UseSecPduCollection)
    {
      /* !LINKSTO SWS_SecOC_Rx_00207,1 */
      TS_MemBZero(SecOC_RxData[instId].RxReceivedAuthPduBuffer, SECOC_GET_RX_AUTH_PDU_LEN(instId));
      TS_MemBZero(SecOC_RxData[instId].RxCryptoPduBuffer, SECOC_GET_RX_CRYPTO_PDU_LEN(instId));
    }
#endif /* #if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON) */

    /* !LINKSTO SWS_SecOC_Rx_00087_1,2 */
    SecOC_RxData[instId].RxPduInProcess = FALSE;
    SecOC_RxData[instId].RxState = SECOC_RX_STATE_IDLE;
#if (STD_ON == SECOC_RX_USE_SAME_BUFFER)
    if (SecOC_RxData[instId].RxBufferUsed != NULL_PTR)
    {
      *SecOC_RxData[instId].RxBufferUsed = FALSE;
    }
#endif
  }
}


static FUNC(void, SECOC_CODE) SecOC_Rx_StateRetryToObtainFreshness(uint16 instId)
{
  /* CHECK: NOPARSE */
  /* It is not possible to stop here the state machine execution */
  if (
      (SECOC_RXEVENT_RXINDICATION   == SecOC_RxData[instId].RxPduREvent) ||
      (SECOC_RXEVENT_STARTRECEPTION == SecOC_RxData[instId].RxPduREvent)
     )
  {
    SecOC_RxData[instId].RxState = SECOC_RX_STATE_IDLE;
    SecOC_RxData[instId].RxPduInProcess = FALSE;
    /* !LINKSTO SWS_SecOC_00235,1 */
    SecOC_RxData[instId].AuthAttempts = 0U;
    /* Set the verification procedure status */
    SecOC_RxData[instId].ProcessingVerification = TRUE;
  }
  /* CHECK: PARSE */
  else
  {
    Std_ReturnType retVal = SECOC_E_NOT_OK;
#if (SECOC_QUERY_FRESHNESS_VALUE != SECOC_FRESHNESS_NONE)
    uint32 freshnessValueLength = SECOC_GET_RX_FRESHNESS_LEN(instId);

    /* The freshness interface is called with sync attempts + 1 because
     * the sync attempts counter will be incremented only if the call return value is SECOC_E_OK.
     * It shall be avoided, that the FreshnessValueSyncAttempts counter is incremented more than
     * once in case the freshness function returns SECOC_E_BUSY.
     */

    /* !LINKSTO SWS_SecOC_00220,1 */
    switch(SecOC_RxConfig[instId].GetRxFreshnessFuncType)
    {
#if (STD_ON == SECOC_RX_FRESHNESS)
      case SECOC_GET_RX_FRESHNESS_FUNC_TYPE:
      {
        SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();

        /* !LINKSTO SWS_SecOC_Rx_xx023,3 */
        /* !LINKSTO SWS_SecOC_Rx_xx025,3 */
        retVal = SECOC_GET_RX_FRESHNESS_FUNC
                 (
                   SECOC_GET_RX_FRESHNESS_ID(instId),
                   &SecOC_RxData[instId].RxSecuredPduBuffer[SecOC_RxData[instId].RxDataLength],
                   SECOC_GET_RX_FRESHNESSTX_LEN(instId),
                   (SecOC_RxData[instId].FreshnessValueSyncAttempts + 1U),
                   SecOC_RxData[instId].RxFreshnessVerifyValue,
                   &freshnessValueLength
                 );

        SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();
      }
      break;
#endif /* (STD_ON == SECOC_RX_FRESHNESS) */
#if (STD_ON == SECOC_RX_AUTH_DATA_FRESHNESS)
      case SECOC_GET_RX_FRESHNESS_AUTHDATA_FUNC_TYPE:
      {
        SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();
        /* !LINKSTO SWS_SecOC_Rx_00219,1 */
        /* !LINKSTO SWS_SecOC_Rx_00244,2 */
        /* !LINKSTO SWS_SecOC_Rx_00246,2 */
        retVal = SECOC_GET_RX_FRESHNESS_AUTHDATA_FUNC
                 (
                   SECOC_GET_RX_FRESHNESS_ID(instId),
                   &SecOC_RxData[instId].RxSecuredPduBuffer[SecOC_RxData[instId].RxDataLength],
                   SECOC_GET_RX_FRESHNESSTX_LEN(instId),
                   SecOC_RxData[instId].AuthDataFreshnessValueBuffer,
                   SecOC_RxConfig[instId].AuthDataFreshnessValueLength,
                   (SecOC_RxData[instId].FreshnessValueSyncAttempts + 1U),
                   SecOC_RxData[instId].RxFreshnessVerifyValue,
                   &freshnessValueLength
                 );
        SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();
      }
      break;
#endif /* (STD_ON == SECOC_RX_AUTH_DATA_FRESHNESS) */
      /* CHECK: NOPARSE */
      default:
      {
        /* Nothing to do. */
      }
      break;
      /* CHECK: PARSE */
    }
#endif /* #if (SECOC_QUERY_FRESHNESS_VALUE != SECOC_FRESHNESS_NONE) */

#if (SECOC_VERIFICATION_STATUS_PROP_EB == SECOC_VERIFICATION_STATUS_PROP_ENABLED)
    SecOC_RxData[instId].VerificationReturn = retVal;
#endif

    if (SECOC_E_BUSY == retVal)
    {
      if (SecOC_RxConfig[instId].AuthenticationBuildAttempts > SecOC_RxData[instId].AuthAttempts)
      {
        /* !LINKSTO SWS_SecOC_Rx_xx018,1 */
        /* !LINKSTO SWS_SecOC_Rx_xx026_BUSY,1 */
        SecOC_RxData[instId].AuthAttempts++;

        /* !LINKSTO SWS_SecOC_00238,1 */
        /* try again, no change on SecOC_RxData[instId].RxState */
      }
      else
      {
        /* Report authentication status */
        /* !LINKSTO SWS_SecOC_00240,1 */
        SecOC_RxData[instId].VerificationResult = SECOC_AUTHENTICATIONBUILDFAILURE;
#if (SECOC_VERIFICATION_STATUS_PROP_NONE != SECOC_VERIFICATION_STATUS_PROP_ENABLED)
        SecOC_Rx_PropagateVerificationStatus(instId);
#endif
        SecOC_Rx_ResetSm(instId);
      }
    }
    else if (SECOC_E_OK == retVal)
    {
      SecOC_RxData[instId].RxState = SECOC_RX_STATE_VERIFICATION;
      /* !LINKSTO SWS_SecOC_00239,1 */
      SecOC_RxData[instId].FreshnessValueSyncAttempts++;

      /* !LINKSTO SWS_SecOC_00239,1 */
      SecOC_RxData[instId].AuthAttempts = 0U;
#if (SECOC_RX_SYNC_PDU_PROCESSING == STD_ON)
      if (TRUE == SecOC_RxConfig[instId].UseRxSyncProcessing)
      {
        SecOC_RxData[instId].ProcessingVerification = TRUE;
      }
#endif /* #if (SECOC_RX_SYNC_PDU_PROCESSING == STD_ON) */
    }
    else
    {
      /* !LINKSTO SWS_SecOC_Rx_xx026_NOT_OK,1 */
      SecOC_RxData[instId].VerificationResult = SECOC_FRESHNESSFAILURE;
#if (SECOC_VERIFICATION_STATUS_PROP_NONE != SECOC_VERIFICATION_STATUS_PROP_ENABLED)
      SecOC_Rx_PropagateVerificationStatus(instId);
#endif
      /* Drop Message */
      /* !LINKSTO SWS_SecOC_Rx_00087_2,1 */
      /* !LINKSTO SWS_SecOC_00240,1 */
      SecOC_Rx_ResetSm(instId);
    }
  }
}
#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

/*==================[helper functions]============================================================*/

#define SECOC_START_SEC_CODE
#include <SecOC_MemMap.h>

static FUNC(void, SECOC_CODE) SecOC_Rx_ResetSm(uint16 instId)
{
  SecOC_RxData[instId].RxState = SECOC_RX_STATE_IDLE;
  SecOC_RxData[instId].RxPduInProcess = FALSE;
  /* !LINKSTO SWS_SecOC_00235,1 */
  SecOC_RxData[instId].AuthAttempts = 0U;
  /* !LINKSTO SWS_SecOC_Rx_00213,1 */
#if (STD_ON == SECOC_RX_USETP)
  if (TRUE == SecOC_RxConfig[instId].UseTp)
  {
    SecOC_RxData[instId].UpperLayerCallFlag = FALSE;

    SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();
    PduR_SecOCTpRxIndication(SECOC_GET_RX_AUTH_PDU_ID_AT_PDUR(instId), E_NOT_OK);
    SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();
  }
#endif

#if (STD_ON == SECOC_RX_USE_SAME_BUFFER)
  if (SecOC_RxData[instId].RxBufferUsed != NULL_PTR)
  {
    *SecOC_RxData[instId].RxBufferUsed = FALSE;
  }
#endif
}

static FUNC(void, SECOC_CODE) SecOC_Rx_CheckForCsmRetry(uint16 instId)
{
  if (
      SecOC_RxConfig[instId].FreshnessValueMaxSyncAttempts >
      SecOC_RxData[instId].FreshnessValueSyncAttempts
     )
  {
    /* !LINKSTO SWS_SecOC_Rx_xx019,1 */
    SecOC_RxData[instId].FreshnessValueSyncAttempts++;

    /* !LINKSTO SWS_SecOC_Rx_00121,2 */
    /* try again */
    SecOC_RxData[instId].RxState = SECOC_RX_STATE_VERIFICATION;
  }
  else
  {
    SecOC_Rx_ResetSm(instId);
  }

#if (SECOC_RX_SYNC_PDU_PROCESSING == STD_ON)
  if (FALSE == SecOC_RxConfig[instId].UseRxSyncProcessing)
#endif /* #if (SECOC_RX_SYNC_PDU_PROCESSING == STD_ON) */
  {
    /* Set the verification procedure status */
    /* !LINKSTO SecOC_EB_Rx_xxx02,2 */
    SecOC_RxData[instId].ProcessingVerification = FALSE;
  }

#if (SECOC_VERIFICATION_STATUS_PROP_EB == SECOC_VERIFICATION_STATUS_PROP_ENABLED)
  /* EB custom error notification required by FvM to identify that the MAC service was rejected */
  SecOC_RxData[instId].VerificationResult = SECOC_MACSERVICEFAILURE;
#else
  /* Autosar compliant error notification */
  SecOC_RxData[instId].VerificationResult = SECOC_FRESHNESSFAILURE;
#endif


#if (SECOC_VERIFICATION_STATUS_PROP_NONE != SECOC_VERIFICATION_STATUS_PROP_ENABLED)
  SecOC_Rx_PropagateVerificationStatus(instId);
#endif /* #if (SECOC_VERIFICATION_STATUS_PROP_NONE != SECOC_VERIFICATION_STATUS_PROP_ENABLED) */
}


#if (SECOC_RX_SHAPE_SEC_PDU == STD_ON)
static FUNC(void, SECOC_CODE) SecOC_Rx_CallShapeFunc(uint16 instId)
{
  if (TRUE == SecOC_RxConfig[instId].RxShapeSecuredPdu)
  {
    PduLengthType secPduLen = SECOC_GET_RX_SECURED_PDU_LEN(instId);
    uint32 authInfo;
    PduIdType pduIdx;

#if (SECOC_RX_USE_SECURED_PDU_COL == STD_ON)
    if (TRUE == SecOC_RxConfig[instId].UseSecPduCollection)
    {
      pduIdx = SecOC_RxConfig[instId].RxReceivedAuthLayerPduIdAtSecOC;
      authInfo = SECOC_GET_RX_CRYPTO_PDU_LEN(instId);
    }
    else
#endif
    {
      pduIdx = SecOC_RxConfig[instId].RxSecuredLayerPduIdAtSecOC;
      authInfo = SECOC_REQUIREDBYTES(SECOC_RX_AUTHENTICATION_INFO_LEN(instId));
    }

    /* Remove the padding from the secured PDU */
    /* !LINKSTO SecOC_EB_Rx_xxx09,2 */
    SECOC_RX_SHAPE_FUNC
    (
      pduIdx,
      SecOC_RxData[instId].RxSecuredPduBuffer,
      &SecOC_RxData[instId].RxSecuredDataLength,
      &secPduLen,
      authInfo
    );

    SecOC_RxData[instId].RxSecuredDataLength = secPduLen;
  }
}
#endif /* #if (SECOC_RX_SHAPE_SEC_PDU == STD_ON) */


#if ((STD_ON == SECOC_RX_AUTH_DATA_FRESHNESS) && (SECOC_QUERY_FRESHNESS_VALUE != SECOC_FRESHNESS_NONE))
static FUNC(void, SECOC_CODE) SecOC_Rx_ExtractAuthDataFreshnessValue
(
  uint16 instId
)
{
  if (NULL_PTR != SecOC_RxData[instId].AuthDataFreshnessValueBuffer)
  {
    TS_MemCpy
    (
      SecOC_RxData[instId].AuthDataFreshnessValueBuffer,
      &SecOC_RxData[instId].RxSecuredPduBuffer
        [SecOC_RxConfig[instId].AuthDataFreshnessValueStartPosition / 8U],
      SECOC_REQUIREDBYTES
      (
        (uint32)SecOC_RxConfig[instId].AuthDataFreshnessValueLength +
        ((uint32)SecOC_RxConfig[instId].AuthDataFreshnessValueStartPosition % 8U)
      )
    );

    if (0U != (SecOC_RxConfig[instId].AuthDataFreshnessValueStartPosition % 8U))
    {
      SecOC_ShiftLeft
      (
        SecOC_RxData[instId].AuthDataFreshnessValueBuffer,
        (
          (uint32)SecOC_RxConfig[instId].AuthDataFreshnessValueLength +
          ((uint32)SecOC_RxConfig[instId].AuthDataFreshnessValueStartPosition % 8U)
        ),
        (uint8) (SecOC_RxConfig[instId].AuthDataFreshnessValueStartPosition % 8U)
      );
    }
  }
}
#endif /* #if (STD_ON == SECOC_RX_AUTH_DATA_FRESHNESS) */

static FUNC(void, SECOC_CODE) SecOC_Rx_CheckFreshnessFuncResult
(
  uint16         instId,
  Std_ReturnType retVal
)
{
#if (SECOC_VERIFICATION_STATUS_PROP_EB == SECOC_VERIFICATION_STATUS_PROP_ENABLED)
  SecOC_RxData[instId].VerificationReturn = retVal;
#endif

  if (SECOC_E_BUSY == retVal)
  {
    if (SecOC_RxConfig[instId].AuthenticationBuildAttempts > SecOC_RxData[instId].AuthAttempts)
    {
      /* !LINKSTO SWS_SecOC_Rx_xx018,1 */
      /* !LINKSTO SWS_SecOC_Rx_xx026_BUSY,1 */
      SecOC_RxData[instId].AuthAttempts++;

      /* !LINKSTO SWS_SecOC_Rx_00121,2 */
      /* !LINKSTO SWS_SecOC_00238,1 */
      /* try again */
      SecOC_RxData[instId].RxState = SECOC_RX_STATE_CONSTRUCTDATATOAUTHENTICATOR;
    }
    else
    {
      /* Report authentication status */
      /* !LINKSTO SWS_SecOC_00240,1 */
      SecOC_RxData[instId].VerificationResult = SECOC_AUTHENTICATIONBUILDFAILURE;
#if (SECOC_VERIFICATION_STATUS_PROP_NONE != SECOC_VERIFICATION_STATUS_PROP_ENABLED)
      SecOC_Rx_PropagateVerificationStatus(instId);
#endif /* #if (SECOC_VERIFICATION_STATUS_PROP_NONE != SECOC_VERIFICATION_STATUS_PROP_ENABLED) */
      SecOC_Rx_ResetSm(instId);
    }

#if (SECOC_RX_SYNC_PDU_PROCESSING == STD_ON)
    if (TRUE == SecOC_RxConfig[instId].UseRxSyncProcessing)
    {
      /* Set the verification procedure status */
      /* !LINKSTO SecOC_EB_Rx_xxx02,2 */
      SecOC_RxData[instId].ProcessingVerification = TRUE;
    }
    else
#endif /* #if (SECOC_RX_SYNC_PDU_PROCESSING == STD_ON) */
    {
      /* Set the verification procedure status */
      /* !LINKSTO SecOC_EB_Rx_xxx02,2 */
      SecOC_RxData[instId].ProcessingVerification = FALSE;
    }
  }
  else
  {
    if (SECOC_E_OK == retVal)
    {
      const uint32 freshnessRemainingBitsLen = (uint32)SECOC_GET_RX_FRESHNESSTX_LEN(instId) % 8U;
      /* !LINKSTO SWS_SecOC_00235,1 */
      SecOC_RxData[instId].AuthAttempts = 0U;

      /* authenticator */
      /* !LINKSTO SWS_SecOC_Rx_00040,1 */
      TS_MemCpy
      (
        SecOC_RxData[instId].RxAuthenticator,
        &SecOC_RxData[instId].RxSecuredPduBuffer
          [SecOC_RxData[instId].RxDataLength + ((PduLengthType)SECOC_GET_RX_FRESHNESSTX_LEN(instId) / 8U)],
        SECOC_REQUIREDBYTES(((uint32) SECOC_GET_RX_AUTHINFOTX_LEN(instId) + freshnessRemainingBitsLen))
      );

      if (0U !=freshnessRemainingBitsLen)
      {
        /* the length passed to the Shift function must be the length of the authenticator,
         * but additional with the offset,
         * which is caused by a bit length of the FreshnessValueTxLength
         */
        SecOC_ShiftLeft
        (
          SecOC_RxData[instId].RxAuthenticator,
          (uint32)SECOC_GET_RX_AUTHINFOTX_LEN(instId) + freshnessRemainingBitsLen,
          (uint8) freshnessRemainingBitsLen
        );
      }
      /* Set the verification procedure status */
      SecOC_RxData[instId].ProcessingVerification = TRUE;

      SecOC_RxData[instId].RxState = SECOC_RX_STATE_VERIFICATION;
    }
    else
    {
      /* !LINKSTO SWS_SecOC_00240,1 */
      SecOC_Rx_ResetSm(instId);
      /* Report authentication status */
      /* !LINKSTO SWS_SecOC_Rx_xx026_NOT_OK,1 */
      SecOC_RxData[instId].VerificationResult = SECOC_FRESHNESSFAILURE;
#if (SECOC_VERIFICATION_STATUS_PROP_NONE != SECOC_VERIFICATION_STATUS_PROP_ENABLED)
      SecOC_Rx_PropagateVerificationStatus(instId);
#endif
      /* Set the verification procedure status */
      /* !LINKSTO SecOC_EB_Rx_xxx02,2 */
      SecOC_RxData[instId].ProcessingVerification = FALSE;
    }
  }
}

static FUNC(void, SECOC_CODE) SecOC_Rx_RenewVerifyStatusOverride
(
  uint16 instId,
  P2VAR(boolean, AUTOMATIC, SECOC_APPL_DATA) passData
)
{
  uint16 pduIdx;

#if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_OFF)
  if (
      (SECOC_OVERRIDESTATUS_FAIL_MSG_NUMBER == SecOC_RxData[instId].VerifyStatusOverride) &&
      (0U                                   == SecOC_RxData[instId].NumMsgToOverride    )
     )
#else
  if (
      (
       (SECOC_OVERRIDESTATUS_FAIL_MSG_NUMBER == SecOC_RxData[instId].VerifyStatusOverride)  ||
       (SECOC_OVERRIDESTATUS_PASS_MSG_NUMBER == SecOC_RxData[instId].VerifyStatusOverride)
      ) &&
      (0U                                    == SecOC_RxData[instId].NumMsgToOverride    )
     )
#endif /* #if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_OFF) */
  {
    /* !LINKSTO SecOC_EB_Rx_xxx06,1 */
    /* Check all freshness ID from the configuration */
    for (pduIdx = 0U; pduIdx < SECOC_NUMBER_RX_PDUS; pduIdx++)
    {
      /* Check if the freshness ID for the given PDU is equal with other PDUs freshness ID */
      if (SECOC_GET_RX_FRESHNESS_ID(instId)== SECOC_GET_RX_FRESHNESS_ID(pduIdx))
      {
        SecOC_RxData[pduIdx].VerifyStatusOverride = SECOC_OVERRIDESTATUS_CANCEL;
      }
    }
  }

  /* Check if the overrideStatus option is enabled */
  if (SECOC_OVERRIDESTATUS_CANCEL != SecOC_RxData[instId].VerifyStatusOverride)
  {
    *passData = FALSE;

    /* Check overrideStatus is set to fail for a given number of messages */
    if (SECOC_OVERRIDESTATUS_FAIL_MSG_NUMBER == SecOC_RxData[instId].VerifyStatusOverride)
    {
      /* !LINKSTO SecOC_EB_Rx_xxx05,1 */
      /* Check all freshness ID from the configuration */
      for (pduIdx = 0U; pduIdx < SECOC_NUMBER_RX_PDUS; pduIdx++)
      {
        /* Check if the freshness ID for the given PDU is equal with other PDUs freshness ID */
        if (SECOC_GET_RX_FRESHNESS_ID(instId) == SECOC_GET_RX_FRESHNESS_ID(pduIdx))
        {
          /* Decrease the number of messages for which the verification result will be overridden */
          SecOC_RxData[pduIdx].NumMsgToOverride--;
        }
      }
    }
#if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_ON)
    /* Check overrideStatus is set to pass for a given number of messages */
    else if (SECOC_OVERRIDESTATUS_PASS_MSG_NUMBER == SecOC_RxData[instId].VerifyStatusOverride)
    {
      /* This will indicate that the status was already renewed once for the current secured PDU */
      SecOC_RxData[instId].RenewedVerStatus = TRUE;

      /* !LINKSTO SecOC_EB_Rx_xxx05,1 */
      /* Check all freshness ID from the configuration */
      for (pduIdx = 0U; pduIdx < SECOC_NUMBER_RX_PDUS; pduIdx++)
      {
        /* Check if the freshness ID for the given PDU is equal with other PDUs freshness ID */
        if (SECOC_GET_RX_FRESHNESS_ID(instId) == SECOC_GET_RX_FRESHNESS_ID(pduIdx))
        {
          /* Decrease the number of messages for which the verification result will be overridden */
          SecOC_RxData[pduIdx].NumMsgToOverride--;
        }
      }

      *passData = TRUE;
    }
    else if (SECOC_OVERRIDESTATUS_SKIP_UNTIL_NOTICE == SecOC_RxData[instId].VerifyStatusOverride)
    {
      *passData = TRUE;
    }
    else
    {
      /* Do nothing for SECOC_OVERRIDESTATUS_FAIL as the passData is already set to FALSE */
    }
#endif /* #if (SECOC_OVERRIDE_VERIFYSTATUS_PASS == STD_ON) */
  }
}


#if (SECOC_VERIFICATION_STATUS_PROP_NONE != SECOC_VERIFICATION_STATUS_PROP_ENABLED)
static FUNC(void, SECOC_CODE) SecOC_Rx_PropagateVerificationStatus
(
  uint16  instId
)
{
  /* Variable used for holding the verification status to propagate */
  SecOC_VerificationStatusType SecOC_RxVerificationStatus;
#if (SECOC_NUMBER_FUNC_VERIFICATION_STATUS_PROP > 0)
  /* Variable used for holding the index of the pointer to verification status callout function */
  uint32 calloutIdx;
#endif

  /* !LINKSTO SWS_SecOC_Rx_00119,1 */
  if (
      (SECOC_STATUS_PROP_BOTH          == SECOC_GET_RX_VERSTATUS_PROP_MODE(instId)) ||
      (
       (SECOC_STATUS_PROP_FAILURE_ONLY == SECOC_GET_RX_VERSTATUS_PROP_MODE(instId)) &&
       (SECOC_VERIFICATIONSUCCESS      != SecOC_RxData[instId].VerificationResult)
      )
     )
  {

    SecOC_RxVerificationStatus.freshnessValueID   = SECOC_GET_RX_FRESHNESS_ID(instId);
    SecOC_RxVerificationStatus.verificationStatus = SecOC_RxData[instId].VerificationResult;
    SecOC_RxVerificationStatus.secOCDataId        = SECOC_GET_RX_DATA_ID(instId);
#if (SECOC_VERIFICATION_STATUS_PROP_EB == SECOC_VERIFICATION_STATUS_PROP_ENABLED)
    SecOC_RxVerificationStatus.verificationReturn = SecOC_RxData[instId].VerificationReturn;
#endif

    SchM_Exit_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();

#if (SECOC_NUMBER_FUNC_VERIFICATION_STATUS_PROP > 0)
    /* !LINKSTO SWS_SecOC_Rx_00119,1 */
    for (calloutIdx = 0U; calloutIdx < SECOC_NUMBER_FUNC_VERIFICATION_STATUS_PROP; calloutIdx++)
    {
      SecOC_RxVerificationStatusCallout[calloutIdx](SecOC_RxVerificationStatus);
    }
#endif

    /* call the Rte, if configured */
#if (STD_ON == SECOC_INCLUDE_RTE)
    /* !LINKSTO SecOC.Dsn.interface.Rx.VerificationStatusCall,3 */
    (void) Rte_Write_PS_VerificationStatus_verificationStatus(&SecOC_RxVerificationStatus);
#endif

    SchM_Enter_SecOC_SCHM_SECOC_EXCLUSIVE_AREA_0();
  }
}
#endif /* #if (STD_ON == SECOC_VERIFICATION_STATUS_PROP_ENABLED) */

#define SECOC_STOP_SEC_CODE
#include <SecOC_MemMap.h>

/*==================[end of file]=================================================================*/
