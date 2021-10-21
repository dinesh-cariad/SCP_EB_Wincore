/***************************************************************************
 *
 * \file Dccm_MainFunction.c
 *
 * \brief Diagnostic Client Communication Manager implementation of MainFunction
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
#include <PduR_Dccm.h>
#include <Dccm_Cfg.h>                      /* Dccm configuration            */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[internal function declarations]=========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

STATIC FUNC(void, DCCM_CODE) Dccm_CheckingCommunicationTimeout(void);
STATIC FUNC(void, DCCM_CODE) Dccm_SendTesterPresent(void);

#define DCCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[external constants]=====================================*/

#define DCCM_START_SEC_CONST_UNSPECIFIED
#include <MemMap.h>

CONST(PduIdType, DCCM_CONST) MaxServers = DCCM_MAX_SERVERS_PHYSICAL_ADDRESSING + DCCM_MAX_SERVERS_FUNCTIONAL_ADDRESSING;

CONST(Dccm_DiagnosticProtocolType, DCCM_CONST) Dccm_DiagnosticProtocolDefault =
{
  0U,/* TxPduId */
  0U, /* RxPduId */
  DCCM_DIAGNOSTIC_PROTOCOL_STATUS_UNUSED, /* Status */
  NULL_PTR, /* DataLengthPtr */
  NULL_PTR, /* Callback */
  /* Dccm_PduInfoRxTxType Dccm_PduInfo:*/
  {
    /* PduInfoType PduInfo; */
    {
      NULL_PTR, /* SduDataPtr */
      0U, /* SduLength */
    },
    NULL_PTR, /* Buffer */
    0U, /* Length */
    0U, /* TpTotalLength */
    0U /* TxConfirmedLength */
  },
  DCCM_RSP_OK, /* ResponseCode */
  {/* Dccm_TimeoutsType Timeouts*/
    DCCM_TIMEOUT_OFF, /* Timeout */
    DCCM_TIMEOUT_OFF,/* InternalTimeout */
    DCCM_TIMEOUT_INTERNAL, /* InternalTimeoutConfigurationValue */
#if (DCCM_P2CLIENT_ENABLED == STD_ON)
    DCCM_TIMEOUT_P2CLIENT, /* P2ClientConfigurationValue */
    DCCM_TIMEOUT_P2STARCLIENT /* P2StarClientConfigurationValue */
#elif (DCCM_P6CLIENT_ENABLED == STD_ON)
    DCCM_TIMEOUT_P6CLIENT, /* P6ClientConfigurationValue */
    DCCM_TIMEOUT_P6STARCLIENT /* P6StarClientConfigurationValue */
#endif
  },
#if (DCCM_BUFFER_STREAMING == STD_ON)
  NULL_PTR, /*  BufferStreamingCallback */
  0U, /* StartLocation */
  0U, /* AvailableDataPtr */
  DCCM_RETRY_INFO_NULL, /* RetryInformation */
#endif
  /* Dccm_TesterPresentType TesterPresent */
  {
    FALSE, /* isTesterPresentEnabled */
    FALSE, /* isMarkedForDisable */
    NULL_PTR,/* DataLengthPtr */
    NULL_PTR,/* Buffer */
    0U, /* Length */
    FALSE, /* mustSendTesterPresentMessage */
  },
  PHYSICAL_ADDRESSING, /* AddressingType */
  /* RequestInfo */
  {
	DCCM_NO_SERVICE, /* ServiceID */
	FALSE, /* isServiceIDsaved */
  },
  FALSE, /* TesterPresentInitiatedByDccm */
};

CONST(Dccm_TesterPresentType, DCCM_CONST) Dccm_TesterPresentDefault =
{
    FALSE, /* isTesterPresentEnabled */
    FALSE, /* isMarkedForDisable */
    NULL_PTR,/* DataLengthPtr */
    NULL_PTR,/* Buffer */
    0U, /* Length */
    FALSE, /* mustSendTesterPresentMessage */
};

#define DCCM_STOP_SEC_CONST_UNSPECIFIED
#include <MemMap.h>

/*==================[internal constants]=====================================*/

#define DCCM_START_SEC_CONST_UNSPECIFIED
#include <MemMap.h>

#define DCCM_STOP_SEC_CONST_UNSPECIFIED
#include <MemMap.h>

/*==================[external data]==========================================*/

#define DCCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <MemMap.h>

/**
 * \brief Diagnostic protocol: A diagnostic protocol represent the whole communication between
 * one Diagnostic Client an one Diagnostic Server
 */
VAR(Dccm_DiagnosticProtocolType, DCCM_VAR) Dccm_DiagnosticProtocols[DCCM_MAX_DIAGNOSTIC_PROTOCOLS];
/**
 * \brief The ProtocolId's on which to send the TesterPresent message
 */
VAR(Dccm_ProtocolIdType, DCCM_VAR) ProtocolIdsForTesterPresentMessages[DCCM_MAX_DIAGNOSTIC_PROTOCOLS];

#define DCCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <MemMap.h>


#define DCCM_START_SEC_VAR_UNSPECIFIED
#include <MemMap.h>

/**
 * \brief The periodicity of the TesterPresent message (in milliseconds)
 */
VAR(Dccm_TimeoutType, DCCM_VAR) TesterPresentInterval = DCCM_TIMEOUT_OFF;
/**
 * \brief Track the time to the next sending of the TesterPresent message
 */
VAR(Dccm_TimeoutType, DCCM_VAR) TesterPresentIntervalCounter = DCCM_TIMEOUT_OFF;

/**
 * \brief The number of ProtocolId's on which to send the TesterPresent message
 */
VAR(uint8, DCCM_VAR) NumberOfProtocolIdsForTesterPresentMessages = 0U;

/**
 * \brief Did the client initialized Dccm with a call to the function Dccm_Init()?
 * If not, some functions will return error
 */
VAR(boolean, DCCM_VAR) DccmWasInitialized = FALSE;

#define DCCM_STOP_SEC_VAR_UNSPECIFIED
#include <MemMap.h>

/*==================[internal data]==========================================*/

#define DCCM_START_SEC_VAR_UNSPECIFIED
#include <MemMap.h>

STATIC VAR(boolean, DCCM_VAR) AllTesterPresentMessagesWereSent = TRUE;

#define DCCM_STOP_SEC_VAR_UNSPECIFIED
#include <MemMap.h>

/*==================[external function definitions]==========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

FUNC(void, DCCM_CODE) Dccm_Init(void)
{
  Dccm_ProtocolIdType ProtocolId;

  TesterPresentInterval = DCCM_TIMEOUT_OFF;
  NumberOfProtocolIdsForTesterPresentMessages = 0U;
  AllTesterPresentMessagesWereSent = TRUE;
  NrOfProtocolsAlreadyUsedForPhysicalCommunication = 0U;

  for(ProtocolId = 0; ProtocolId < DCCM_MAX_DIAGNOSTIC_PROTOCOLS; ProtocolId++)
  {
    Dccm_DiagnosticProtocols[ProtocolId] = Dccm_DiagnosticProtocolDefault;
    ProtocolIdsForTesterPresentMessages [ProtocolId] = DCCM_INVALID_PROTOCOL_ID;
  }

  DccmWasInitialized = TRUE;
}

FUNC(void, DCCM_CODE) Dccm_MainFunction(void)
{
  Std_ReturnType RetVal;
  Dccm_ProtocolIdType ProtocolId = 0;

#if (DCCM_BUFFER_STREAMING == STD_ON)
  PduLengthType HowManyBytesToAsk = 0U;
#endif

  if (DccmWasInitialized == TRUE)
   {
   /* should send Tester Present message for functional addressing?
    * only if (there is at least one functional address defined) AND (the TesterPresent is not disabled) */
   if ((NumberOfProtocolIdsForTesterPresentMessages > 0U) && (TesterPresentInterval != DCCM_TIMEOUT_OFF))
   {
     if (Dccm_DiagnosticProtocols[ProtocolIdsForTesterPresentMessages[ProtocolId]].TesterPresent.isMarkedForDisable == TRUE)
     {
       /* do nothing: if the TesterPresent was "marked for disable" for the current protocol,
        * no tracking is needed related with "when to send the next TesterPresent message". */
     }
     else
     {
       if (TesterPresentIntervalCounter > DCCM_EXE_INTERVAL)
       {
         TesterPresentIntervalCounter = TesterPresentIntervalCounter - DCCM_EXE_INTERVAL;

         /* the retry functionality for the TesterPresent message */
         if (AllTesterPresentMessagesWereSent == FALSE)
         {
           /* If there are TesterPresent messages waiting to be sent, send them now */
           Dccm_SendTesterPresent();
         }
       }
       else
       {
         /* record in all relevant protocols that the TesterPresent message must be sent */
         for (ProtocolId = 0; ProtocolId < NumberOfProtocolIdsForTesterPresentMessages; ProtocolId++)
         {
           Dccm_DiagnosticProtocols[ProtocolIdsForTesterPresentMessages[ProtocolId]].TesterPresent.mustSendTesterPresentMessage = TRUE;
         }
         Dccm_SendTesterPresent();
         TesterPresentIntervalCounter = TesterPresentInterval;
       }
     }
   }

  for(ProtocolId = 0; ProtocolId < DCCM_MAX_DIAGNOSTIC_PROTOCOLS; ProtocolId++)
  {
    if (Dccm_DiagnosticProtocols[ProtocolId].Status != DCCM_DIAGNOSTIC_PROTOCOL_STATUS_UNUSED)
    {
      /* check for communication timeouts */
      Dccm_CheckingCommunicationTimeout();

      switch (Dccm_DiagnosticProtocols[ProtocolId].Status)
      {
        case DCCM_DIAGNOSTIC_PROTOCOL_STATUS_READY_TO_TRANSMIT:
          /* trigger PduR transmit */
          RetVal = PduR_DccmTransmit(Dccm_DiagnosticProtocols[ProtocolId].TxPduId, &(Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo));

          SchM_Enter_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
          /* set internal timeout, to avoid endless loop if PduR does not
          * call Dccm_TxConfirmation */
          if (Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeoutConfigurationValue != DCCM_TIMEOUT_OFF)
          {
            Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeout = Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeoutConfigurationValue + DCCM_EXE_INTERVAL;
          }
          SchM_Exit_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();

          /* communication timeout will be set after successful
           * transmission (in Dccm_TxConfirmation)*/
          if (RetVal == E_OK)
          {
            Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT;
          }
          else
          {
            Dccm_DiagnosticProtocols[ProtocolId].ResponseCode = DCCM_RSP_TX_TRIG_FAILED;
            Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_NOTIFY;
          }
          break;

#if (DCCM_BUFFER_STREAMING == STD_ON)
        case DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT_REQUEST_NEXT_BUFFER:
          if ( *(Dccm_DiagnosticProtocols[ProtocolId].DataLengthPtr)
                < (Dccm_DiagnosticProtocols[ProtocolId].StartLocation + Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.Length)
          )
          {
            HowManyBytesToAsk = *(Dccm_DiagnosticProtocols[ProtocolId].DataLengthPtr) - Dccm_DiagnosticProtocols[ProtocolId].StartLocation;
          }
          else
          {
            HowManyBytesToAsk  = Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.Length;
          }
          RetVal = Dccm_DiagnosticProtocols[ProtocolId].BufferStreamingCallback
          (
              ProtocolId,
              Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.Buffer,
              Dccm_DiagnosticProtocols[ProtocolId].StartLocation,
              & HowManyBytesToAsk,
              Dccm_DiagnosticProtocols[ProtocolId].RetryInformation
          );
          if (RetVal == E_OK)
          {
            Dccm_DiagnosticProtocols[ProtocolId].AvailableData = HowManyBytesToAsk;
            Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_TRANSMIT;
          }
          else
          {
            /* let the protocol status unchanged */
          }
          break;
#endif

        case DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_NOTIFY:
          /* notify SW-Manager */
          Dccm_DiagnosticProtocols[ProtocolId].Callback(ProtocolId, Dccm_DiagnosticProtocols[ProtocolId].ResponseCode);

          Dccm_DiagnosticProtocols[ProtocolId].TesterPresentInitiatedByDccm = FALSE;

          if (Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.isMarkedForDisable == TRUE)
          {
             (void)Dccm_DisableTesterPresent(ProtocolId);
          }

          /* stop timeout counter */
          SchM_Enter_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
          Dccm_DiagnosticProtocols[ProtocolId].Timeouts.Timeout = DCCM_TIMEOUT_OFF;
          Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeout = DCCM_TIMEOUT_OFF;
          SchM_Exit_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();

          Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_READY;
          break;

        default:
          /* for all the other states we have nothing to do in the main function */
          break;
        }
      }
    }
  }
}

#define DCCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function definitions]==========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

/**
 * \brief Check for communication timeout
 */
STATIC FUNC(void, DCCM_CODE) Dccm_CheckingCommunicationTimeout
(
 void
)
{
  Dccm_ProtocolIdType ProtocolId = 0;

  SchM_Enter_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();

  /* P2Client and P2*Client */
  if (Dccm_DiagnosticProtocols[ProtocolId].Timeouts.Timeout != DCCM_TIMEOUT_OFF)
  {
    if (Dccm_DiagnosticProtocols[ProtocolId].Timeouts.Timeout >= DCCM_EXE_INTERVAL)
    {
      Dccm_DiagnosticProtocols[ProtocolId].Timeouts.Timeout = Dccm_DiagnosticProtocols[ProtocolId].Timeouts.Timeout - DCCM_EXE_INTERVAL;
    }
    else
    {
#if (DCCM_P2CLIENT_ENABLED == STD_ON)
      Dccm_DiagnosticProtocols[ProtocolId].ResponseCode = DCCM_RSP_TIMEOUT_P2CLIENT;
#elif (DCCM_P6CLIENT_ENABLED == STD_ON)
      Dccm_DiagnosticProtocols[ProtocolId].ResponseCode = DCCM_RSP_TIMEOUT_P6CLIENT;
#endif
      Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_NOTIFY;
    }
  }

  /* internal timeout*/
  if (Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeout != DCCM_TIMEOUT_OFF)
  {
    if (Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeout >= DCCM_EXE_INTERVAL)
    {
      Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeout = Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeout - DCCM_EXE_INTERVAL;
    }
    else
    {
      Dccm_DiagnosticProtocols[ProtocolId].ResponseCode = DCCM_RSP_TIMEOUT_INTERNAL;
      Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_NOTIFY;
    }
  }

  SchM_Exit_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
}

/**
 * \brief Send the TesterPresent message
 */
STATIC FUNC(void, DCCM_CODE) Dccm_SendTesterPresent
(
 void
)
{
  /* XXX is it possible that the client can delete / add a TesterPresent destination during the
   * execution of this function? if yes, then how to protect the code? */
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 i = 0U;
  Dccm_ProtocolIdType ProtocolId;

  AllTesterPresentMessagesWereSent = TRUE;

  /* Use an exclusive area to prevent the following use-case:
   * Dccm starts in an execution thread the function Dccm_SendTesterPresent() (this function)
   * (this means to copy the TesterPresent messages in the array with protocols)
   * but the client of Dccm, form a different thread of execution,
   * asks to add or delete a tester present destination
   * (using the functions Dccm_EnableTesterPresent() or Dccm_DisableTesterPresent()):
   * this would disrupt the array ProtocolIdsForTesterPresentMessages
   * and the variable NumberOfProtocolIdsForTesterPresentMessages during the execution of this function.
   *
   * The function Dccm_SendRequest() do not call code from outside of UsdC,
   * and always have a finite execution time - it is just putting data in a buffer.
   * */
  SchM_Enter_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();

  for (i=0; i < NumberOfProtocolIdsForTesterPresentMessages; i++)
  {
    ProtocolId = ProtocolIdsForTesterPresentMessages[i];
    if ((Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.mustSendTesterPresentMessage == TRUE) &&
        (Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.DataLengthPtr != NULL_PTR))
    {
      /* Keep-Alive-Logic is defined as the functionally addressed TesterPresent message with
         suppressPosRspMsgIndicationBit set to TRUE */
      /* initialize values */
      Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.Buffer[0] = DCCM_SID_TESTER_PRESENT;
      Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.Buffer[1] = DCCM_SUPP_MSG_BIT;
      *(Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.DataLengthPtr) = 2U;

      RetVal = Dccm_SendRequest ( ProtocolId,
          Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.Buffer,
          Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.Length,
          Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.DataLengthPtr );

      /* if the protocol is in use, the TesterPresent message can not be sent: Dccm will try again later */
      if (RetVal == E_OK)
      {
        /* because this function is called from MainFunction, the request was not sent yet. */
        Dccm_DiagnosticProtocols[ProtocolId].TesterPresentInitiatedByDccm = TRUE;
        Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.mustSendTesterPresentMessage = FALSE;
      }
      else
      {
        AllTesterPresentMessagesWereSent = FALSE;
      }
    }
  }

  SchM_Exit_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();

}

#define DCCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[end of file]============================================*/
