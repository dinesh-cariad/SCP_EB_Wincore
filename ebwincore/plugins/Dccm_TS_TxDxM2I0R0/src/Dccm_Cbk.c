 /***************************************************************************
 *
 * \file Dccm_Cbk.c
 *
 * \brief Diagnostic Client Communication Manager implementation of PduR Callback functions
 *
 * Copyright 2020 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 */

/*==================[inclusions]=============================================*/

#include <Dccm_Internal.h>
#include <Dccm_Cbk.h>

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[internal function declarations]=========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_CopyTxData_VerifyParameters
(
  P2VAR(Dccm_ProtocolIdType, AUTOMATIC, DCCM_APPL_DATA) ProtocolIdPtr,
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCCM_APPL_DATA) PduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, DCCM_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DCCM_APPL_DATA) AvailableDataPtr
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ProcessRetryInformation
(
  P2VAR(Dccm_ProtocolIdType, AUTOMATIC, DCCM_APPL_DATA) ProtocolId,
  P2VAR(RetryInfoType, AUTOMATIC, DCCM_APPL_DATA) RetryInfoPtr,
  PduLengthType SduLength
);

#define DCCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

FUNC(BufReq_ReturnType, DCCM_CODE) Dccm_StartOfReception
(
  PduIdType RxPduId,
  PduLengthType TpTotalLength,
  P2VAR(PduLengthType, AUTOMATIC, DCCM_APPL_DATA) RxBufferSizePtr
)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  Dccm_ProtocolIdType ProtocolId = 0U;

  if (RxPduId >= MaxServers)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_PDUID);
  }
  else if (Dccm_GetDiagnosticProtocolByRxPduId (RxPduId, &ProtocolId) == E_NOT_OK)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_PROTOCOL_ID);
  }
  else if (DCCM_DIAGNOSTIC_PROTOCOL_STATUS_RECEIVE == Dccm_DiagnosticProtocols[ProtocolId].Status)
  {

    if (RxBufferSizePtr == NULL_PTR)
    {
      DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_NULL_POINTER);
      retVal = BUFREQ_E_NOT_OK;
    }
    else if ( TpTotalLength > Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.Length )
    {
      DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_BUFFER_LENGTH);
      retVal = BUFREQ_E_NOT_OK;
    }
    else
    {
      /* everything is OK */

#if (DCCM_P2CLIENT_ENABLED == STD_ON)
      SchM_Enter_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
      /* stop the P2ClientConfigurationValue */
      Dccm_DiagnosticProtocols[ProtocolId].Timeouts.Timeout = DCCM_TIMEOUT_OFF;
      SchM_Exit_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
#endif

#if (DCCM_P6CLIENT_ENABLED == STD_OFF)
      SchM_Enter_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
      /* if P6Client is not enabled then start InternalTimeout*/
      if (Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeoutConfigurationValue != DCCM_TIMEOUT_OFF)
      {
         Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeout = Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeoutConfigurationValue + DCCM_EXE_INTERVAL;
      }
      SchM_Exit_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
#endif

      if (TpTotalLength != 0U)
      {
        Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TpTotalLength = TpTotalLength;
        Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength = 0U;
        *RxBufferSizePtr = Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.Length;

        retVal = BUFREQ_OK;
      }
      else
      {
        /* TpTotalLength == 0U */
        /* the server ask to provide the currently available maximum buffer size */
        *RxBufferSizePtr = Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.Length;

        retVal = BUFREQ_OK;
      }
    }
  }
  else
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_STATE);
  }

  return retVal;
}

FUNC(BufReq_ReturnType, DCCM_CODE) Dccm_CopyRxData
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCCM_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DCCM_APPL_DATA) RxBufferSizePtr
)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint16 count = 0U;
  Dccm_ProtocolIdType ProtocolId = 0;

  if (RxPduId >= MaxServers)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_PDUID);
  }
  else if (Dccm_GetDiagnosticProtocolByRxPduId (RxPduId, &ProtocolId) == E_NOT_OK)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_PROTOCOL_ID);
  }
  else if ( (PduInfoPtr == NULL_PTR) || (RxBufferSizePtr == NULL_PTR) )
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_NULL_POINTER);
    retVal = BUFREQ_E_NOT_OK;
  }
  else if ( DCCM_DIAGNOSTIC_PROTOCOL_STATUS_RECEIVE != Dccm_DiagnosticProtocols[ProtocolId].Status )
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_STATE);
    retVal = BUFREQ_E_NOT_OK;
  }
  else
  {
    /* Check if service request fits in reception(Rx) buffer
    * If not: return error code */
    /* length (what was received now) + length (what was received until now) < length (receiving buffer) */
    if ((PduInfoPtr->SduLength + Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength) > (PduLengthType) Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.Length)
    {
      DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_BUFFER_OVERFLOW);
      retVal = BUFREQ_E_NOT_OK;
    }
    else if (PduInfoPtr->SduLength == 0U)
    {
      /* just update *RxBufferSizePtr with the number of unused bytes from the receive buffer.*/
      *RxBufferSizePtr = Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.Length - Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength;
      retVal = BUFREQ_OK;
    }
    else
    {
      /* length (what was received now) + length (what was received until now) <= length (what was announced that it will be sent) */
      if ((PduInfoPtr->SduLength + Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength) <= Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TpTotalLength)
      {
        /* copy data to Rx buffer */
        for (count = 0U; count < PduInfoPtr->SduLength; count++)
        {
          Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduDataPtr[count + Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength] = PduInfoPtr->SduDataPtr[count];
        }

        /* DataLength -> length must be set zero in StartOfReception */
        Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength += PduInfoPtr->SduLength;

        /* update *RxBufferSizePtr with the number of unused bytes from the receive buffer.*/
        *RxBufferSizePtr = Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.Length - Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength;

        retVal = BUFREQ_OK;
      }
      else
      {
        DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_BUFFER_LENGTH);
        retVal = BUFREQ_E_NOT_OK;
      }
    }
  }

  return retVal;
}

FUNC(void, DCCM_CODE) Dccm_RxIndication
(
  PduIdType RxPduId,
  NotifResultType Result
)
{
  boolean ret = FALSE;
  Dccm_ProtocolIdType ProtocolId = 0;
  boolean SendNotification = TRUE;

  if (RxPduId >= MaxServers)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_PDUID);
  }
  else if (Dccm_GetDiagnosticProtocolByRxPduId (RxPduId, &ProtocolId) == E_NOT_OK)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_PROTOCOL_ID);
  }
  else if (DCCM_DIAGNOSTIC_PROTOCOL_STATUS_RECEIVE == Dccm_DiagnosticProtocols[ProtocolId].Status)
  {

#if (DCCM_P6CLIENT_ENABLED == STD_OFF)
    /* if P6Client is not enabled then stop InternalTimeout counter*/
    SchM_Enter_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
    Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeout = DCCM_TIMEOUT_OFF;
    SchM_Exit_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
#else
    /* if P6Client is enabled then stop Timeout counter*/
    SchM_Enter_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
    Dccm_DiagnosticProtocols[ProtocolId].Timeouts.Timeout = DCCM_TIMEOUT_OFF;
    SchM_Exit_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
#endif

    switch(Result)
    {
      case NTFRSLT_OK:
        /* reception successful */
        /* was it response pending */
        if (Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength > 0U)
        {
          ret = Dccm_CheckResponsePending(Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength, Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduDataPtr);
          if (TRUE == ret)
          {
            /* it was response pending -> do not inform Dccm_MainFunction about it */
            SendNotification = FALSE;

            /* check the second byte of the response, the byte corresponding with SID */
            /* if it's not the same as the SID for which the request was made return error */
            if (Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduDataPtr[1U] != Dccm_DiagnosticProtocols[ProtocolId].RequestInfo.ServiceID)
            {
               Dccm_DiagnosticProtocols[ProtocolId].ResponseCode = DCCM_RSP_INVALID_RESPONSE_PENDING_FORMAT;
               DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_INVALID_RESPONSE_PENDING_FORMAT);
               Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_NOTIFY;
            }
            else
            {

              /* it was response pending -> start the Timeout counter */
              /* depending on the configuration the Timeout counter will be loaded with P2StarClient or P6StarClient*/
              SchM_Enter_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
#if (DCCM_P2CLIENT_ENABLED == STD_ON)
              Dccm_DiagnosticProtocols[ProtocolId].Timeouts.Timeout = Dccm_DiagnosticProtocols[ProtocolId].Timeouts.P2StarClientConfigurationValue + DCCM_EXE_INTERVAL;
#elif (DCCM_P6CLIENT_ENABLED == STD_ON)
              Dccm_DiagnosticProtocols[ProtocolId].Timeouts.Timeout = Dccm_DiagnosticProtocols[ProtocolId].Timeouts.P6StarClientConfigurationValue + DCCM_EXE_INTERVAL;
#endif
              SchM_Exit_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
            }
          }
          else
          {
            if (Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength != Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TpTotalLength)
            {
              /* report error if the receiving of data was finished with RxIndication(),
               * but the length of the data package received is smaller than the size
               * of the data package that was announced using StartOfReception() */
              Dccm_DiagnosticProtocols[ProtocolId].ResponseCode = DCCM_RSP_RX_FAILED;
              DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_FAILED);
            }
            else
            {
              Dccm_DiagnosticProtocols[ProtocolId].ResponseCode = DCCM_RSP_OK;
            }
          }
        }
        else
        {
          Dccm_DiagnosticProtocols[ProtocolId].ResponseCode = DCCM_RSP_WRONG_BUFFER_SIZE;
        }

        if (SendNotification == TRUE)
        {
          Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_NOTIFY;

          /* notify the caller of the Dccm_SendRequest() about the length of the actual data received from the server;
           * because DataLengthPtr is a pointer, this update will be visible to the caller of the Dccm_SendRequest()*/
          *(Dccm_DiagnosticProtocols[ProtocolId].DataLengthPtr) = Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength;
        }
        break;

      case NTFRSLT_E_TIMEOUT_A:
      case NTFRSLT_E_TIMEOUT_BS:
      case NTFRSLT_E_TIMEOUT_CR:
        /* reception timed out */
        Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_NOTIFY;
        Dccm_DiagnosticProtocols[ProtocolId].ResponseCode = DCCM_RSP_TIMEOUT;
        break;

      default:
        /* reception failed */
        Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_NOTIFY;
        Dccm_DiagnosticProtocols[ProtocolId].ResponseCode = DCCM_RSP_RX_FAILED;
        DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_FAILED_2);
        break;
    }
  }
  else
  {
    /* Dccm_RxIndication call found the module in an unexpected state */
    DCCM_DET_REPORT_ERROR(DCCM_SID_RX, DCCM_E_STATE);
  }
}

FUNC(BufReq_ReturnType, DCCM_CODE) Dccm_CopyTxData
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCCM_APPL_DATA) PduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, DCCM_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DCCM_APPL_DATA) AvailableDataPtr
)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint16 count = 0U;
  Dccm_ProtocolIdType ProtocolId = 0;
  Std_ReturnType CheckInputParameters;

  CheckInputParameters = Dccm_CopyTxData_VerifyParameters(&ProtocolId, TxPduId, PduInfoPtr, RetryInfoPtr, AvailableDataPtr);

  if (CheckInputParameters == E_NOT_OK)
  {
    /* nothing to do here, input parameters are not valid */
  }
  else
  {
  /* If no buffer is available to contain the TesterPresent request (eg. the TesterPresent was disabled in the meantime) */
    if ((Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.Buffer == NULL_PTR) &&
        (Dccm_DiagnosticProtocols[ProtocolId].TesterPresentInitiatedByDccm == TRUE))
    {
      /* do nothing */
    }
    else if (Dccm_DiagnosticProtocols[ProtocolId].Status == DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT_REQUEST_NEXT_BUFFER)
    {
      retVal = BUFREQ_E_BUSY;
    }
    else /* (Dccm_DiagnosticProtocols[ProtocolId].Status == DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT) */
    {
      Std_ReturnType RetryInformationProcessing = Dccm_ProcessRetryInformation(&ProtocolId, RetryInfoPtr, PduInfoPtr->SduLength);

      if (RetryInformationProcessing == E_OK)
      {
        /* at first call CanTp is asking if more data is available. */
        if ((PduInfoPtr->SduLength == 0U))
        {
          /* update *AvailableDataPtr with the remaining number of bytes that still have to be transferred */
#if (DCCM_BUFFER_STREAMING == STD_OFF)
          *AvailableDataPtr = Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength - Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TpTotalLength;
#else
          *AvailableDataPtr = *(Dccm_DiagnosticProtocols[ProtocolId].DataLengthPtr) - Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TpTotalLength;
#endif

          retVal = BUFREQ_OK;
        }
        /* at second call copy data to provided buffer */
        else
        {
#if (DCCM_BUFFER_STREAMING == STD_OFF)
          if ((Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TpTotalLength + PduInfoPtr->SduLength) >  Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength /* the length of the actual data bytes from the buffer / (*DataLengthPtr) */)
          {
            DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_INVALID_PARAMETER_3);
            retVal = BUFREQ_E_BUSY;
          }
          else
          {
            /* if SID was not saved, save it */
            if (Dccm_DiagnosticProtocols[ProtocolId].RequestInfo.isServiceIDsaved == FALSE)
            {
              Dccm_DiagnosticProtocols[ProtocolId].RequestInfo.ServiceID = Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduDataPtr[0];
              Dccm_DiagnosticProtocols[ProtocolId].RequestInfo.isServiceIDsaved = TRUE;
            }

            for (count = 0U; count < PduInfoPtr->SduLength; count++)
            {
              PduInfoPtr->SduDataPtr[count] = Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduDataPtr[count + Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TpTotalLength];
            }

            Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TpTotalLength += PduInfoPtr->SduLength;
            /* update *AvailableDataPtr with the remaining number of bytes that still have to be transferred */
            *AvailableDataPtr = Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength - Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TpTotalLength;
            retVal = BUFREQ_OK;
          }
#else          /*#if (DCCM_BUFFER_STREAMING == STD_ON) ================*/
          /* report error for the following use-case:
           * 10 bytes to send, 4 bytes buffer, and transfer of 4+4+3(instead of 2)=11 bytes */
            if ( (Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TpTotalLength + PduInfoPtr->SduLength)
              >  Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength)
          { /* all what was sent until now                              + what is sending now*/
            /* > (is more than) the whole data that should be sent ? */
            DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_INVALID_PARAMETER_4);
            retVal = BUFREQ_E_BUSY;
          }
          else
          {
            /* if SID was not saved, save it */
            if (Dccm_DiagnosticProtocols[ProtocolId].RequestInfo.isServiceIDsaved == FALSE)
            {
              Dccm_DiagnosticProtocols[ProtocolId].RequestInfo.ServiceID = Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduDataPtr[0];
              Dccm_DiagnosticProtocols[ProtocolId].RequestInfo.isServiceIDsaved = TRUE;
            }

            for (count = 0U; count < PduInfoPtr->SduLength; count++)
              {
                PduInfoPtr->SduDataPtr[count] = Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduDataPtr[count/* + Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TpTotalLength*/];
              }
              /* if not all data transmitted */
              if ( *(Dccm_DiagnosticProtocols[ProtocolId].DataLengthPtr)
                  > (Dccm_DiagnosticProtocols[ProtocolId].StartLocation
                      + PduInfoPtr->SduLength/*Dccm_DiagnosticProtocols[ProtocolId].AvailableData*/
                    )
              )
              {
                /* adjust StartLocation */
                Dccm_DiagnosticProtocols[ProtocolId].StartLocation =
                    Dccm_DiagnosticProtocols[ProtocolId].StartLocation
                    + PduInfoPtr->SduLength/*Dccm_DiagnosticProtocols[ProtocolId].AvailableData*/;
                /* ask more */
                if (RetryInfoPtr == NULL_PTR)
                {
                  Dccm_DiagnosticProtocols[ProtocolId].RetryInformation = (uint8)DCCM_RETRY_INFO_NULL;
                }
                else
                {
                  Dccm_DiagnosticProtocols[ProtocolId].RetryInformation = (uint8)(RetryInfoPtr->TpDataState);
                }
                Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT_REQUEST_NEXT_BUFFER;
              }
              Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TpTotalLength += PduInfoPtr->SduLength/*Dccm_DiagnosticProtocols[ProtocolId].AvailableData*/;
              /* update *AvailableDataPtr with the remaining number of bytes that still have to be transferred */
              *AvailableDataPtr = *(Dccm_DiagnosticProtocols[ProtocolId].DataLengthPtr) - Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TpTotalLength;
              retVal = BUFREQ_OK;
            }/*else*/
#endif          /*#endif (DCCM_BUFFER_STREAMING == STD_OFF / ON) ================*/

        }/* at second call copy data to provided buffer */
      } /* if (RetryInformationProcessing == E_OK) */
#if (DCCM_BUFFER_STREAMING == STD_ON)
      else if (RetryInformationProcessing == DCCM_ASK_MORE_DATA)
      {
        retVal = BUFREQ_E_BUSY;
      }
#endif
      else
      {
        if (RetryInformationProcessing == BUFREQ_E_BUSY)
        {
            retVal = BUFREQ_E_BUSY;
        }
      }
    } /* if (Dccm_DiagnosticProtocols[ProtocolId].Status == DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT) */
  }

  return retVal;
}

FUNC(void, DCCM_CODE) Dccm_TxConfirmation
(
  PduIdType       TxPduId,
  NotifResultType Result
)
{
  Dccm_ProtocolIdType ProtocolId = 0;

  if (TxPduId >= MaxServers)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_PDUID);
  }
  else if (Dccm_GetDiagnosticProtocolByTxPduId (TxPduId, &ProtocolId) == E_NOT_OK)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_PROTOCOL_ID);
  }
  else if (Dccm_DiagnosticProtocols[ProtocolId].Status != DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_STATE);
  }
  else
  {
      switch(Result)
      {
        case NTFRSLT_OK:
          /* transmission successful */

          /* stop timeout counter */
          SchM_Enter_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
          Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeout = DCCM_TIMEOUT_OFF;
          SchM_Exit_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();

          /* check if suppress positive response message indication bit is set */
          if ( Dccm_CheckBufferSuppressBit(Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.Length, Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.Buffer)
#if (DCCM_FUNCTIONAL_COMM_NO_RESPONSE_EXPECTED == STD_ON)
              ||
              /* or the use-case is with functional communication, and
               * Dccm was configured to suppress the response
               * for all functional communication */
              (Dccm_DiagnosticProtocols[ProtocolId].AddressingType == FUNCTIONAL_ADDRESSING)
#endif
          )
          {
            /* reset buffer length */
            /*(Dccm_DiagnosticProtocols[ProtocolId].Dccm_ReqData.DataLengthPtr) = 0U;*/

            /* set response code */
            Dccm_DiagnosticProtocols[ProtocolId].ResponseCode = DCCM_TX_CONFIRMATION_OK;
            Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_NOTIFY;
            /* Dccm_TxConfirmation(): (suppressPosRspMsgIndicationBit == TRUE) && ((server reports negative response) OR (server reports requestCorrectlyReceived-ResponsePending + positive response))
             * the use-case is: "suppress positive response message indication bit is set"
             * is it possible that error messages can be received from the server?
             * how to notify the client about these error messages?
             * a possibility of implementation:
             *  wait for a response from the server for a period (eg. DCCM_TIMEOUT_P2CLIENT):
             *    1) if no answer comes from the server,
             *      Dccm can assume that all was ok: can notify the client that all was ok
             *    2) or, if a negative response comes from the server,
             *      Dccm can notify the client about the negative response
             * */
          }
          else
          {
            /* start timeout counter */
            /* depending on the configuration the Timeout counter will be loaded with P2Client or P6Client*/
            SchM_Enter_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
#if (DCCM_P2CLIENT_ENABLED == STD_ON)
            Dccm_DiagnosticProtocols[ProtocolId].Timeouts.Timeout = Dccm_DiagnosticProtocols[ProtocolId].Timeouts.P2ClientConfigurationValue + DCCM_EXE_INTERVAL;
#elif (DCCM_P6CLIENT_ENABLED == STD_ON)
            Dccm_DiagnosticProtocols[ProtocolId].Timeouts.Timeout = Dccm_DiagnosticProtocols[ProtocolId].Timeouts.P6ClientConfigurationValue + DCCM_EXE_INTERVAL;
#endif
            SchM_Exit_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
            Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_RECEIVE;
          }
          break;

        case NTFRSLT_E_TIMEOUT_A:
        case NTFRSLT_E_TIMEOUT_BS:
        case NTFRSLT_E_TIMEOUT_CR:
          /* transmission timed out */
          Dccm_DiagnosticProtocols[ProtocolId].ResponseCode = DCCM_RSP_TIMEOUT;
          Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_NOTIFY;
          break;

        default:
          /* transmission failed */
          DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_FAILED);
          Dccm_DiagnosticProtocols[ProtocolId].ResponseCode = DCCM_RSP_TX_FAILED;
          Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_NOTIFY;
          break;
      }
  }
}

FUNC(Std_ReturnType, DCCM_CODE) Dccm_GetDiagnosticProtocolByRxPduId
(
 PduIdType RxPduId,
 P2VAR(Dccm_ProtocolIdType, AUTOMATIC, DCCM_APPL_DATA) ProtocolId
)
{
  Std_ReturnType retVal = E_NOT_OK;
  Dccm_ProtocolIdType i = 0;
  *ProtocolId = DCCM_INVALID_PROTOCOL_ID;
  for(i = 0; i < DCCM_MAX_DIAGNOSTIC_PROTOCOLS; i++)
  {
    if((Dccm_DiagnosticProtocols[i].RxPduId == RxPduId) &&
        (Dccm_DiagnosticProtocols[i].Status != DCCM_DIAGNOSTIC_PROTOCOL_STATUS_UNUSED))
    {
      *ProtocolId = i;
      retVal = E_OK;
      break;
    }
  }
  return retVal;
}

FUNC(Std_ReturnType, DCCM_CODE) Dccm_GetDiagnosticProtocolByTxPduId
(
 PduIdType TxPduId,
 P2VAR(Dccm_ProtocolIdType, AUTOMATIC, DCCM_APPL_DATA) ProtocolId
)
{
  Std_ReturnType retVal = E_NOT_OK;
  Dccm_ProtocolIdType i = 0;
  *ProtocolId = DCCM_INVALID_PROTOCOL_ID;
  for(i = 0; i < DCCM_MAX_DIAGNOSTIC_PROTOCOLS; i++)
  {
    if((Dccm_DiagnosticProtocols[i].TxPduId == TxPduId )&&
        (Dccm_DiagnosticProtocols[i].Status != DCCM_DIAGNOSTIC_PROTOCOL_STATUS_UNUSED))
    {
      *ProtocolId = i;
      retVal = E_OK;
        break;
    }
  }
  return retVal;
}

#define DCCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function definitions]==========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

/**
 * \brief Check if parameters used for copying request data are right set
 */
STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_CopyTxData_VerifyParameters
(
  P2VAR(Dccm_ProtocolIdType, AUTOMATIC, DCCM_APPL_DATA) ProtocolIdPtr,
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCCM_APPL_DATA) PduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, DCCM_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DCCM_APPL_DATA) AvailableDataPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

  if (TxPduId >= MaxServers)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_PDUID);
  }
  else if (Dccm_GetDiagnosticProtocolByTxPduId (TxPduId, ProtocolIdPtr) == E_NOT_OK)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_PROTOCOL_ID);
  }
  else if ((Dccm_DiagnosticProtocols[*ProtocolIdPtr].Status != DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT)
      && (Dccm_DiagnosticProtocols[*ProtocolIdPtr].Status != DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT_REQUEST_NEXT_BUFFER))
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_STATE);
  }
  else if ((PduInfoPtr == NULL_PTR) ||
      (AvailableDataPtr == NULL_PTR) ||
      ( (PduInfoPtr->SduDataPtr == NULL_PTR) &&
        (PduInfoPtr->SduLength != 0U)
      )
    )
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_NULL_POINTER);
  }
  else if((RetryInfoPtr != NULL_PTR) &&
          (RetryInfoPtr->TpDataState != TP_CONFPENDING) &&
          (RetryInfoPtr->TpDataState != TP_DATARETRY) &&
          (RetryInfoPtr->TpDataState != TP_DATACONF))
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_INVALID_PARAMETER);
  }
  else if (PduInfoPtr->SduLength > Dccm_DiagnosticProtocols[*ProtocolIdPtr].Dccm_PduInfo.Length)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_INVALID_PARAMETER_2);
  }
  else
  {
    retVal = E_OK;
  }

  return retVal;
}

/** \brief Adjusts the data mark indexes of a transmission buffer according to the
 *         information of the RetryInfoPtr received in the Dccm_CopyTxData() call if
 *         this pointer contains valid information.
 *
 *  \param RetryInfoPtr [in]            Pointer to retry information given by the lower layer
 *                                      in the context of the Dccm_CopyTxData() API call.
 *  \return Std_ReturnType
 *  \retval     E_OK                    Either the RetryInfoPtr is a NULL pointer or points to
 *                                      valid data. The data marks have been successfully updated.
 *  \retval     E_NOT_OK                The RetryInfoPtr points to data which is incompatible to
 *                                      current transmission session.
 *  \retval     DCCM_ASK_MORE_DATA      The function Dccm_ProcessRetryInformation() instructs the caller to
 *                                      ask more data from the client application.
 *                                      This macro is needed only for streaming use-case.
 *  \retval     BUFREQ_E_BUSY           Some part of the data was already sent;
 *                                      after this, request a retransmission, but
 *                                      what was sent - offset + what is asked > all data that must be sent;
 *                                      The transfer is not completely failed, but this particular call to
 *                                      CopyTxData() can not be completed due to wrong parameters.
 *                                      If the caller of CopyTxData() calls again CopyTxData()
 *                                      with the correct parameters, the transfer can be completed successfully.
 */
STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ProcessRetryInformation
(
  P2VAR(Dccm_ProtocolIdType, AUTOMATIC, DCCM_APPL_DATA) ProtocolId,
  P2VAR(RetryInfoType, AUTOMATIC, DCCM_APPL_DATA) RetryInfoPtr,
  PduLengthType SduLength
)
{
  Std_ReturnType RetryInformationProcessing = E_NOT_OK;

  /* Lower layer has requested data to be copied. */
  if(RetryInfoPtr != NULL_PTR)
  {
    switch (RetryInfoPtr->TpDataState)
    {
      case TP_DATARETRY:
        /* If RetryInfoPtr->TpDataState is TP_RETRY, and RetryInfoPtr->TxTpDataCnt is larger
         * than the available payload data prior to the current transmitted data mark
         * Dccm_CopyTxData() shall return BUFREQ_E_NOT_OK.
         */
        if(RetryInfoPtr->TxTpDataCnt > (Dccm_DiagnosticProtocols[*ProtocolId].Dccm_PduInfo.TpTotalLength - Dccm_DiagnosticProtocols[*ProtocolId].Dccm_PduInfo.TxConfirmedLength))
        {
          DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_INVALID_PARAMETER);
        }
#if (DCCM_BUFFER_STREAMING == STD_OFF)
        else
        if ((Dccm_DiagnosticProtocols[*ProtocolId].Dccm_PduInfo.TpTotalLength + SduLength - RetryInfoPtr->TxTpDataCnt) >  Dccm_DiagnosticProtocols[*ProtocolId].Dccm_PduInfo.PduInfo.SduLength /* the length of the actual data bytes from the buffer / (*DataLengthPtr) */)
        {
          DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_INVALID_PARAMETER_2);
          RetryInformationProcessing = BUFREQ_E_BUSY;
        }
#else /*#if (DCCM_BUFFER_STREAMING == STD_ON) ================*/
        /* some part of the data was already sent; after this, request a retransmission, but
         * what was sent - offset + what is asked > all data that must be sent */
        else
        if ( (Dccm_DiagnosticProtocols[*ProtocolId].Dccm_PduInfo.TpTotalLength + SduLength - RetryInfoPtr->TxTpDataCnt)
            >  Dccm_DiagnosticProtocols[*ProtocolId].Dccm_PduInfo.PduInfo.SduLength)
        {
          DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_INVALID_PARAMETER_3);
          RetryInformationProcessing = BUFREQ_E_BUSY;
        }
#endif
        else
        {
          /* Rewind the buffer to the point where the retry was requested */
          Dccm_DiagnosticProtocols[*ProtocolId].Dccm_PduInfo.TpTotalLength -= RetryInfoPtr->TxTpDataCnt;
          /* All data before this location is implicitly considered as confirmed. */
          Dccm_DiagnosticProtocols[*ProtocolId].Dccm_PduInfo.TxConfirmedLength = Dccm_DiagnosticProtocols[*ProtocolId].Dccm_PduInfo.TpTotalLength;
#if (DCCM_BUFFER_STREAMING == STD_OFF)
          RetryInformationProcessing = E_OK;
#else /*#if (DCCM_BUFFER_STREAMING == STD_ON) ================*/
          /* Ask from the upper layer (client application) data starting from the point where the retry was requested */
          Dccm_DiagnosticProtocols[*ProtocolId].StartLocation = Dccm_DiagnosticProtocols[*ProtocolId].Dccm_PduInfo.TpTotalLength;
          Dccm_DiagnosticProtocols[*ProtocolId].RetryInformation = (uint8)TP_DATARETRY;
          Dccm_DiagnosticProtocols[*ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT_REQUEST_NEXT_BUFFER;
          RetryInformationProcessing = DCCM_ASK_MORE_DATA;
#endif
        }

        break;

      case TP_DATACONF:
        /* Lower layer has confirmed the already transferred data
         * and will expect the next chunk of data to be transferred */
        Dccm_DiagnosticProtocols[*ProtocolId].Dccm_PduInfo.TxConfirmedLength = Dccm_DiagnosticProtocols[*ProtocolId].Dccm_PduInfo.TpTotalLength;
        RetryInformationProcessing = E_OK;
        break;

      case TP_CONFPENDING:
        /* The lower layer requests not to confirm any already copied data yet */
        RetryInformationProcessing = E_OK;
        break;

      /* CHECK: NOPARSE */
      default:
        /* Unreachable code as this error condition was captured in the preamble
           of the Dccm_CopyTxData() function . This is required by MISRA.*/
        DCCM_DET_REPORT_ERROR(DCCM_SID_TX, DCCM_E_INVALID_PARAMETER_4);
        break;
      /* CHECK: PARSE */
    }
  }
  else
  {
    /* No retry information to process. All data before this location
       is implicitly considered as confirmed. This is also the approach in asc_Dcm and asc_Dlt.*/
    Dccm_DiagnosticProtocols[*ProtocolId].Dccm_PduInfo.TxConfirmedLength = Dccm_DiagnosticProtocols[*ProtocolId].Dccm_PduInfo.TpTotalLength;
    RetryInformationProcessing = E_OK;
  }

  return RetryInformationProcessing;
}

#define DCCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[end of file]============================================*/
