 /***************************************************************************
 *
 * \file Dccm_Service.c
 *
 * \brief Diagnostic Client Communication Manager implementation of Services
 *
 * Copyright 2020 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 2.7 (advisory)
 * There should be no unused parameters in functions. violated
 *
 * Reason:
 * This is a false positive. In all the reported test cases, the macros
 * DCCM_MAX_SERVERS_FUNCTIONAL_ADDRESSING and DCCM_MAX_SERVERS_PHYSICAL_ADDRESSING have value zero.
 * In this use-case, all the function parameters are handled with TS_PARAM_UNUSED() macro.
 *
 * MISRAC2012-2) Deviated Rule: Dir 4.5 (advisory)
 * Identifiers in the same name space with overlapping visibility should be typographically unambiguous. violated
 *
 * Reason:
 * This is a false positive. For the reported identifier, there is no overlapping visibility
 * in this function. There is no global variable with the same name.
 *
 */
/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * False positive, because in all the indicated TestSequences, DCCM_MAX_SERVERS_FUNCTIONAL_ADDRESSING is 1U.
 * This means that the condition is not always true.
 *
 * TASKING-2) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * False positive, because in all the indicated TestSequences, DCCM_MAX_SERVERS_PHYSICAL_ADDRESSING is 1U.
 * This means that the condition is not always true.
 */

/*==================[inclusions]=============================================*/

#include <Dccm_Internal.h>
#include <TSAutosar.h>

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[internal function declarations]=========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_GetNextFreeDiagnosticProtocol
(
    P2VAR(Dccm_ProtocolIdType, AUTOMATIC, DCCM_APPL_DATA) ProtocolId
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_VerifyTxRxPduIds
(
  PduIdType TxPduId,
  PduIdType RxPduId,
  uint8 AddressingType
);

#define DCCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/
#define DCCM_START_SEC_VAR_UNSPECIFIED
#include <MemMap.h>

/** \brief The number of ProtocolId's used for physical communication at some point */
VAR(uint8, DCCM_VAR) NrOfProtocolsAlreadyUsedForPhysicalCommunication = 0U;

#define DCCM_STOP_SEC_VAR_UNSPECIFIED
#include <MemMap.h>

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

FUNC(Std_ReturnType, DCCM_CODE) Dccm_GetDiagnosticProtocolStatus
(
  Dccm_ProtocolIdType ProtocolId,
  P2VAR(Dccm_DiagnosticProtocolStatusType, AUTOMATIC, DCCM_APPL_DATA) Status
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  if (Status == NULL_PTR)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_NULL_POINTER);
  }
  else if (ProtocolId >= DCCM_MAX_DIAGNOSTIC_PROTOCOLS)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_PROTOCOL_ID);
  }
  else
  {
    *Status = Dccm_DiagnosticProtocols[ProtocolId].Status;
    RetVal = E_OK;
  }
  return RetVal;
}

FUNC(Std_ReturnType, DCCM_CODE) Dccm_AllocateDiagnosticProtocol
(
  uint16 TxPduId,
  uint16 RxPduId,
  P2VAR(Dccm_ProtocolIdType, AUTOMATIC, DCCM_APPL_DATA) ProtocolId,
  uint8 AddressingType,
  Dccm_CallbackType Callback,
  Dccm_BufferStreamingCallbackType BufferStreamingCallback
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Dccm_ProtocolIdType TempProtocolId = DCCM_INVALID_PROTOCOL_ID;

  if (DccmWasInitialized == FALSE)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_STATE);
  }
  else if (ProtocolId == NULL_PTR)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_NULL_POINTER);
  }
  else
  {
#if (DCCM_BUFFER_STREAMING == STD_ON)
    if ((Callback == NULL_PTR)  || (BufferStreamingCallback == NULL_PTR))
#else
    if (Callback == NULL_PTR)
#endif
    {
      DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_NULL_POINTER_2);
    }
#if (DCCM_BUFFER_STREAMING == STD_OFF)
    else if (BufferStreamingCallback != NULL_PTR)
    {
      DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_NULL_POINTER_3);
    }
#endif
    else if ((AddressingType != PHYSICAL_ADDRESSING) && (AddressingType != FUNCTIONAL_ADDRESSING))
    {
      DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_INVALID_PARAMETER);
    }
    else if ((AddressingType == FUNCTIONAL_ADDRESSING) && (DCCM_MAX_SERVERS_FUNCTIONAL_ADDRESSING == 0U))
    {
      DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_INVALID_PARAMETER_2);
    }
    else if (Dccm_VerifyTxRxPduIds(TxPduId, RxPduId, AddressingType) == E_NOT_OK)
    {
      DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_INVALID_PARAMETER_4);
    }
    else
    {
      Std_ReturnType RetVal_GetByTxPduId = Dccm_GetDiagnosticProtocolByTxPduId(TxPduId, &TempProtocolId);
      Std_ReturnType RetVal_GetByRxPduId = Dccm_GetDiagnosticProtocolByRxPduId(RxPduId, &TempProtocolId);
      *ProtocolId = DCCM_INVALID_PROTOCOL_ID;
      if ((E_OK == RetVal_GetByTxPduId) || (E_OK == RetVal_GetByRxPduId))
      {
        /* if the TxPduId or RxPduId is already in the list, report error. */
        DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_PDUID);
      }
      else if (NrOfProtocolsAlreadyUsedForPhysicalCommunication >= DCCM_MAX_PHYSICAL_DIAGNOSTIC_PROTOCOLS )
      {
        DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_FAILED);
      }
      else
      {
        if (Dccm_GetNextFreeDiagnosticProtocol(ProtocolId) == E_OK)
        {
          /* the status of the protocol is already
           * DCCM_DIAGNOSTIC_PROTOCOL_STATUS_ALLOCATING
           * to prevent re-allocation of the same protocol */
          Dccm_DiagnosticProtocols[*ProtocolId].TxPduId = TxPduId;
          Dccm_DiagnosticProtocols[*ProtocolId].RxPduId = RxPduId;
          Dccm_DiagnosticProtocols[*ProtocolId].Callback = Callback;
          Dccm_DiagnosticProtocols[*ProtocolId].AddressingType = AddressingType;
          Dccm_DiagnosticProtocols[*ProtocolId].Timeouts.InternalTimeoutConfigurationValue = DCCM_TIMEOUT_INTERNAL;
#if (DCCM_P2CLIENT_ENABLED == STD_ON)
          Dccm_DiagnosticProtocols[*ProtocolId].Timeouts.P2ClientConfigurationValue = DCCM_TIMEOUT_P2CLIENT;
          Dccm_DiagnosticProtocols[*ProtocolId].Timeouts.P2StarClientConfigurationValue = DCCM_TIMEOUT_P2STARCLIENT;
#elif (DCCM_P6CLIENT_ENABLED == STD_ON)
          Dccm_DiagnosticProtocols[*ProtocolId].Timeouts.P6ClientConfigurationValue = DCCM_TIMEOUT_P6CLIENT;
          Dccm_DiagnosticProtocols[*ProtocolId].Timeouts.P6StarClientConfigurationValue = DCCM_TIMEOUT_P6STARCLIENT;
#endif

  #if (DCCM_BUFFER_STREAMING == STD_ON)
          Dccm_DiagnosticProtocols[*ProtocolId].BufferStreamingCallback = BufferStreamingCallback;
  #endif
          if(AddressingType == PHYSICAL_ADDRESSING)
          {
            NrOfProtocolsAlreadyUsedForPhysicalCommunication++;
          }
          Dccm_DiagnosticProtocols[*ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_READY;
          RetVal = E_OK;
        }
        else
        {
          RetVal = DCCM_STD_E_BUSY;
        }
      }
    }
  }/*if (ProtocolId != NULL_PTR)*/
  return RetVal;
}

FUNC(Std_ReturnType, DCCM_CODE) Dccm_ReleaseDiagnosticProtocol
(
  Dccm_ProtocolIdType ProtocolId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  if (ProtocolId >= DCCM_MAX_DIAGNOSTIC_PROTOCOLS)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_PDUID);
  }
  else if (DccmWasInitialized == FALSE)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_STATE);
  }
  else
  {
    if ((Dccm_DiagnosticProtocols[ProtocolId].Status != DCCM_DIAGNOSTIC_PROTOCOL_STATUS_UNUSED)&&
        (Dccm_DiagnosticProtocols[ProtocolId].Status != DCCM_DIAGNOSTIC_PROTOCOL_STATUS_RELEASE) &&
        (Dccm_DiagnosticProtocols[ProtocolId].Status != DCCM_DIAGNOSTIC_PROTOCOL_STATUS_ALLOCATING))
    {
      if (Dccm_DiagnosticProtocols[ProtocolId].TesterPresentInitiatedByDccm == TRUE)
      {
        /* TesterPresent sending ongoing: notify Dccm to disable tester present at a later point in time */
        (void) Dccm_DisableTesterPresent (ProtocolId);
      }
      else
      {
        Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_RELEASE;
        (void) Dccm_DisableTesterPresent (ProtocolId);

        if(Dccm_DiagnosticProtocols[ProtocolId].AddressingType == PHYSICAL_ADDRESSING)
        {
          NrOfProtocolsAlreadyUsedForPhysicalCommunication--;
        }
        /* the re-initialization of the protocol will transition the protocol to the state
         * DCCM_DIAGNOSTIC_PROTOCOL_STATUS_UNUSED */
        Dccm_DiagnosticProtocols[ProtocolId] = Dccm_DiagnosticProtocolDefault;
        RetVal = E_OK;
      }
    }
  }
  return RetVal;
}

FUNC(Std_ReturnType, DCCM_CODE) Dccm_ReleaseAllDiagnosticProtocols
(
void
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Dccm_ProtocolIdType ProtocolId = 0U;
  boolean CanReleaseAllProtocols = TRUE;

  if (DccmWasInitialized == FALSE)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_STATE);
  }
  else
  {
    SchM_Enter_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
    /* check if there is a protocol that can not be released */
    for(ProtocolId = 0U; ProtocolId < DCCM_MAX_DIAGNOSTIC_PROTOCOLS; ProtocolId++)
    {
      if ((Dccm_DiagnosticProtocols[ProtocolId].Status == DCCM_DIAGNOSTIC_PROTOCOL_STATUS_RELEASE) ||
         (Dccm_DiagnosticProtocols[ProtocolId].Status == DCCM_DIAGNOSTIC_PROTOCOL_STATUS_ALLOCATING) ||
         ((Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.isTesterPresentEnabled == TRUE) &&
         (Dccm_DiagnosticProtocols[ProtocolId].TesterPresentInitiatedByDccm == TRUE)))
      {
        CanReleaseAllProtocols = FALSE;
        break;
      }
    }

    if(CanReleaseAllProtocols == TRUE)
    {
      /* if the execution reaches this place, all protocols can be released with no problem */
      for(ProtocolId = 0U; ProtocolId < DCCM_MAX_DIAGNOSTIC_PROTOCOLS; ProtocolId++)
      {

        if (Dccm_DiagnosticProtocols[ProtocolId].Status != DCCM_DIAGNOSTIC_PROTOCOL_STATUS_UNUSED)
        {
          (void) Dccm_ReleaseDiagnosticProtocol(ProtocolId);
        }
      }
      RetVal = E_OK;
    }
   SchM_Exit_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
  }
  return RetVal;
}

/* Deviation MISRAC2012-2 <START> */
FUNC(Std_ReturnType, DCCM_CODE) Dccm_SendRequest
(
  Dccm_ProtocolIdType ProtocolId,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) Buffer,
  uint16 BufferLength,
  P2VAR(uint16, AUTOMATIC, DCCM_APPL_DATA) DataLengthPtr
)
{
  /* ATTENTION: the code might be in an exclusive area (disabled interrupts),
   * because of a call from Dccm_SendTesterPresent() */
  Std_ReturnType RetVal = E_NOT_OK;

  if ((Buffer == NULL_PTR) ||
      (DataLengthPtr == NULL_PTR))
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_NULL_POINTER);
  }
  else if (ProtocolId >= DCCM_MAX_DIAGNOSTIC_PROTOCOLS)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_PDUID);
  }
  else if (DccmWasInitialized == FALSE)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_STATE);
  }
#if (DCCM_BUFFER_STREAMING == STD_OFF)
  else if (BufferLength < *DataLengthPtr)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_BUFFER_LENGTH);
  }
#endif
  else
  {
    if (Dccm_DiagnosticProtocols[ProtocolId].Status != DCCM_DIAGNOSTIC_PROTOCOL_STATUS_READY)
    {
      RetVal = DCCM_STD_E_BUSY;
    }
    else
    {
      Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_SEND_RECEIVED;
      Dccm_DiagnosticProtocols[ProtocolId].DataLengthPtr = DataLengthPtr;

      Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduDataPtr = Buffer;
      Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.PduInfo.SduLength  = *DataLengthPtr;
      Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.Length             = BufferLength;
      Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.Buffer             = Buffer;
      Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TpTotalLength      = 0U;
      Dccm_DiagnosticProtocols[ProtocolId].Dccm_PduInfo.TxConfirmedLength  = 0U;
      Dccm_DiagnosticProtocols[ProtocolId].RequestInfo.ServiceID           = DCCM_NO_SERVICE;
      Dccm_DiagnosticProtocols[ProtocolId].RequestInfo.isServiceIDsaved    = FALSE;

#if (DCCM_FUNCTIONAL_COMM_NO_RESPONSE_EXPECTED == STD_ON)
      /* For functional communication, if the service has sub-functions,
       * overwrite the bit suppressPosRspMsgIndicationBit to TRUE */
      if ((Dccm_DiagnosticProtocols[ProtocolId].AddressingType == FUNCTIONAL_ADDRESSING)
          && (*DataLengthPtr >= 2U)
          && (BufferLength >= 2))
      {
        switch (Buffer[0])
        {
          case DCCM_SID_ROUTINE_CONTROL_REQUEST:
          case DCCM_SID_TESTER_PRESENT:
          case DCCM_SID_DIAGNOSTIC_SESSION_CONTROL:
          case DCCM_SID_ECU_RESET:
          case DCCM_SID_SECURITY_ACCESS:
          case DCCM_SID_COMMUNICATION_CONTROL:
          case DCCM_SID_ACCESS_TIMING_PARAMETER:
          case DCCM_SID_CONTROL_DTC_SETTING:
          case DCCM_SID_RESPONSE_ON_EVENT:
          case DCCM_SID_LINK_CONTROL:
          case DCCM_SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER:
          case DCCM_SID_READ_DTC_INFORMATION:
            Buffer[1] |= DCCM_SUPP_MSG_BIT;
            break;
          default:
            /* the service has no sub-function */
            break;
        }
      }
#endif

      RetVal = E_OK;

#if (DCCM_BUFFER_STREAMING == STD_ON)
      /* initialize the variables used for streaming */
      Dccm_DiagnosticProtocols[ProtocolId].StartLocation = 0U;
      Dccm_DiagnosticProtocols[ProtocolId].AvailableData = BufferLength;
      Dccm_DiagnosticProtocols[ProtocolId].RetryInformation = DCCM_RETRY_INFO_NULL;
#endif
      Dccm_DiagnosticProtocols[ProtocolId].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_READY_TO_TRANSMIT;
    }
  }
  return RetVal;
}
/* Deviation MISRAC2012-2 <STOP> */

/* Deviation MISRAC2012-2 <START> */
FUNC(boolean, DCCM_CODE) Dccm_CheckBufferSuppressBit
(
  uint16 DataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) Buffer
)
{
  boolean RetVal = FALSE;

  if (DataLength >= 2U)
  {
    switch (Buffer[0])
    {
      case DCCM_SID_ROUTINE_CONTROL_REQUEST:
      case DCCM_SID_TESTER_PRESENT:
      case DCCM_SID_DIAGNOSTIC_SESSION_CONTROL:
      case DCCM_SID_ECU_RESET:
      case DCCM_SID_SECURITY_ACCESS:
      case DCCM_SID_COMMUNICATION_CONTROL:
      case DCCM_SID_ACCESS_TIMING_PARAMETER:
      case DCCM_SID_CONTROL_DTC_SETTING:
      case DCCM_SID_RESPONSE_ON_EVENT:
      case DCCM_SID_LINK_CONTROL:
      case DCCM_SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER:
      case DCCM_SID_READ_DTC_INFORMATION:
        RetVal = (Buffer[1] < DCCM_SUPP_MSG_BIT) ? FALSE : TRUE;
        break;
      default:
        RetVal = FALSE;
        break;
    }
  }

  return RetVal;
}
/* Deviation MISRAC2012-2 <STOP> */

/* Deviation MISRAC2012-2 <START> */
FUNC(boolean, DCCM_CODE) Dccm_CheckResponsePending
(
  uint16 DataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) Buffer
)
{
  boolean RetVal = FALSE;

  if (DataLength >= 2U)
  {
    if ( (DCCM_NEGATIVE_RESPONSE_SID == Buffer[0]) &&
         (DCCM_E_RESPONSE_PENDING == Buffer[2]) )
    {
      RetVal = TRUE;
    }
  }

  return RetVal;
}
/* Deviation MISRAC2012-2 <STOP> */

FUNC(void, DCCM_CODE) Dccm_SetTesterPresentInterval
(
  uint16 Interval
)
{
  if (TesterPresentInterval != DCCM_TIMEOUT_OFF )
  {
    /* TesterPresent is already enabled;
     * the next sending of the "tester present message" will be the last
     * that will respect the interval that was set before the current call of this function.
     * Let the current count-down go to "zero"; after this, the TesterPresent message will be sent;
     * after this, the counter will be initialized with the value that is provided as Interval in the current function call.
     * */
    /* Just change the periodicity, and let the counter untouched. */
    TesterPresentInterval = Interval;
  }
  else
  {
    /* TesterPresent was disabled until now */
    TesterPresentInterval = Interval ;
    TesterPresentIntervalCounter = Interval;
  }
}

/* Deviation MISRAC2012-2 <START> */
FUNC(Std_ReturnType, DCCM_CODE) Dccm_EnableTesterPresent
(
    Dccm_ProtocolIdType ProtocolId,
    P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) Buffer,
    uint16 BufferLength,
    P2VAR(uint16, AUTOMATIC, DCCM_APPL_DATA) DataLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  if ((TesterPresentInterval == DCCM_TIMEOUT_OFF)
        || (DccmWasInitialized == FALSE)
     )
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_STATE);
  }
  else if (DCCM_MAX_SERVERS_FUNCTIONAL_ADDRESSING == 0U)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_FUNCTIONAL_ADDRESSING);
  }
  else if ((Buffer == NULL_PTR)
            || (DataLengthPtr == NULL_PTR)
          )
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_NULL_POINTER);
  }
  else if (ProtocolId >= DCCM_MAX_DIAGNOSTIC_PROTOCOLS)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_PDUID);
  }
  else if (BufferLength < DCCM_NEGATIVE_RESPONSE_LENGTH)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_BUFFER_LENGTH);
  }
  else if (Dccm_DiagnosticProtocols[ProtocolId].Status == DCCM_DIAGNOSTIC_PROTOCOL_STATUS_UNUSED)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_INVALID_PARAMETER);
  }
  else if (Dccm_DiagnosticProtocols[ProtocolId].AddressingType != FUNCTIONAL_ADDRESSING)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_INVALID_PARAMETER_2);
  }
  else
  {
    SchM_Enter_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
    if (Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.isTesterPresentEnabled == FALSE)
    {
      ProtocolIdsForTesterPresentMessages[NumberOfProtocolIdsForTesterPresentMessages] = ProtocolId;
      NumberOfProtocolIdsForTesterPresentMessages++;

      Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.isTesterPresentEnabled = TRUE;
      Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.DataLengthPtr = DataLengthPtr;
      Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.Length = BufferLength;
      Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.Buffer = Buffer;
    }
    Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.isMarkedForDisable = FALSE;

    RetVal = E_OK;
    SchM_Exit_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
  }
  return RetVal;
}
/* Deviation MISRAC2012-2 <STOP> */

FUNC(Std_ReturnType, DCCM_CODE) Dccm_DisableTesterPresent
(
    Dccm_ProtocolIdType ProtocolId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 i = 0;
  uint8 j = 0;
  boolean found = FALSE;

  if (ProtocolId >= DCCM_MAX_DIAGNOSTIC_PROTOCOLS)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_PDUID);
  }
  else if (Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.isTesterPresentEnabled == TRUE)
  {
    if (Dccm_DiagnosticProtocols[ProtocolId].TesterPresentInitiatedByDccm == TRUE)
    {
      /* TesterPresent sending is ongoing: disable TesterPresent after sending the message */
      Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.isMarkedForDisable = TRUE;
      RetVal = E_OK;
    }
    else
    {
      /* TesterPresent sending is not ongoing: disable TesterPresent now */
      SchM_Enter_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
      /* search the ProtocolId: the ProtocolId is in the list? */
      for (i = 0; i < NumberOfProtocolIdsForTesterPresentMessages; i++)
      {
        if (ProtocolIdsForTesterPresentMessages[i] == ProtocolId)
        {
          found = TRUE;
          break;
        }
      }
      if (found)
      {
        /* in the current protocol data structure, reset to default the values for the TesterPresent sub-structure */
        Dccm_DiagnosticProtocols[ProtocolId].TesterPresent = Dccm_TesterPresentDefault;

        for (j = i; j < (NumberOfProtocolIdsForTesterPresentMessages - 1); j++)
        {
          ProtocolIdsForTesterPresentMessages[j] = ProtocolIdsForTesterPresentMessages[j+1];
        }
        /* "j" is now the index of what was the last valid element in the array - put there an invalid value */
        ProtocolIdsForTesterPresentMessages[j] = DCCM_INVALID_PROTOCOL_ID;
        NumberOfProtocolIdsForTesterPresentMessages--;
        RetVal = E_OK;
      }
      SchM_Exit_Dccm_SCHM_DCCM_EXCLUSIVE_AREA_0();
    }
  }
  else
  {
  /* Nothing to do here */
  }
  return RetVal;
}

FUNC(Std_ReturnType, DCCM_CODE) Dccm_IsTesterPresentEnabled
(
    Dccm_ProtocolIdType ProtocolId,
    P2VAR(boolean, AUTOMATIC, DCCM_APPL_DATA) IsTesterPresentEnabled
)
{
  boolean RetVal = E_NOT_OK;

  if (ProtocolId >= DCCM_MAX_DIAGNOSTIC_PROTOCOLS)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_PDUID);
  }
  else if (IsTesterPresentEnabled == NULL_PTR)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_NULL_POINTER);
  }
  else if ((Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.isTesterPresentEnabled == TRUE)
            && (Dccm_DiagnosticProtocols[ProtocolId].TesterPresent.isMarkedForDisable == FALSE))
  {
    *IsTesterPresentEnabled = TRUE;
    RetVal = E_OK;
  }
  else
  {
    *IsTesterPresentEnabled = FALSE;
    RetVal = E_OK;
  }
  return RetVal;
}

FUNC(Std_ReturnType, DCCM_CODE) Dccm_SetCommunicationTimeoutParameters
(
  Dccm_ProtocolIdType ProtocolId,
#if (DCCM_P2CLIENT_ENABLED == STD_ON)
  Dccm_TimeoutType P2ClientConfigurationValue,
  Dccm_TimeoutType InternalTimeout,
  Dccm_TimeoutType P2StarClientConfigurationValue
#elif (DCCM_P6CLIENT_ENABLED == STD_ON)
  Dccm_TimeoutType P6ClientConfigurationValue,
  Dccm_TimeoutType InternalTimeout,
  Dccm_TimeoutType P6StarClientConfigurationValue
#endif
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  if (ProtocolId >= DCCM_MAX_DIAGNOSTIC_PROTOCOLS)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_PROTOCOL_ID);
  }
  else if (Dccm_DiagnosticProtocols[ProtocolId].Status != DCCM_DIAGNOSTIC_PROTOCOL_STATUS_READY)
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_STATE);
  }
#if (DCCM_P2CLIENT_ENABLED == STD_ON)
  else if ((P2ClientConfigurationValue > 4294967294UL) || (InternalTimeout > 4294967294UL) || (P2StarClientConfigurationValue > 4294967294UL))
#elif (DCCM_P6CLIENT_ENABLED == STD_ON)
  else if ((P6ClientConfigurationValue > 4294967294UL) || (InternalTimeout > 4294967294UL) || (P6StarClientConfigurationValue > 4294967294UL))
#endif
  {
    DCCM_DET_REPORT_ERROR(DCCM_SID_SERVICE, DCCM_E_INVALID_PARAMETER);
  }
  else
  {
    /* set value for InternalTimeout */
    if (InternalTimeout == 0UL)
    {
      Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeoutConfigurationValue = DCCM_TIMEOUT_OFF;
    }
    else
    {
      Dccm_DiagnosticProtocols[ProtocolId].Timeouts.InternalTimeoutConfigurationValue = InternalTimeout;
    }

#if (DCCM_P2CLIENT_ENABLED == STD_ON)
    /* set values for P2Client */
    Dccm_DiagnosticProtocols[ProtocolId].Timeouts.P2ClientConfigurationValue = P2ClientConfigurationValue;
    Dccm_DiagnosticProtocols[ProtocolId].Timeouts.P2StarClientConfigurationValue = P2StarClientConfigurationValue;
#elif (DCCM_P6CLIENT_ENABLED == STD_ON)
    /* set values for P6Client */
    Dccm_DiagnosticProtocols[ProtocolId].Timeouts.P6ClientConfigurationValue = P6ClientConfigurationValue;
    Dccm_DiagnosticProtocols[ProtocolId].Timeouts.P6StarClientConfigurationValue = P6StarClientConfigurationValue;
#endif

    RetVal = E_OK;
  }
  return RetVal;
}


#define DCCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function definitions]==========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

/**
 *
 * \brief Return a free Dccm diagnostic protocol.
 *
 * An interface to return a free Dccm diagnostic protocol.
 *
 * \param[out] ProtocolId The protocol ID that will be used by the client
 * of Dccm to communicate with the server. Must not be null.
 *
 * \retval E_OK Query accepted
 *
 * \retval E_NOT_OK All the Dccm diagnostic protocols are already in use.
 *
 */
STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_GetNextFreeDiagnosticProtocol
(
 P2VAR(Dccm_ProtocolIdType, AUTOMATIC, DCCM_APPL_DATA) ProtocolId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Dccm_ProtocolIdType i = 0;

  for(i = 0; i < DCCM_MAX_DIAGNOSTIC_PROTOCOLS; i++)
  {
    if(Dccm_DiagnosticProtocols[i].Status == DCCM_DIAGNOSTIC_PROTOCOL_STATUS_UNUSED)
    {
      Dccm_DiagnosticProtocols[i].Status = DCCM_DIAGNOSTIC_PROTOCOL_STATUS_ALLOCATING;
      *ProtocolId = i;
      RetVal = E_OK;
    break;
    }
  }
  return RetVal;
}

/**
 *
 * \brief Function to check if PduIds are found in the configuration.
 *
 * \param  TxPduId Pdu Id of the server.
 *
 * \param  RxPduId Pdu Id of the server.
 *
 * \param  AddressingType The protocol is allocated for physical or functional communication.
 *                        Expected values: PHYSICAL_ADDRESSING or FUNCTIONAL_ADDRESSING.
 *
 * \retval E_OK PduIds were found.
 *
 * \retval E_NOT_OK PduIds were not found.
 *
 */
/* Deviation MISRAC2012-1 <START> */
STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_VerifyTxRxPduIds
(
  PduIdType TxPduId,
  PduIdType RxPduId,
  uint8 AddressingType
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if ((DCCM_MAX_SERVERS_FUNCTIONAL_ADDRESSING > 0U) || (DCCM_MAX_SERVERS_PHYSICAL_ADDRESSING > 0U))
  uint32 i = 0U;
#endif

#if ((DCCM_MAX_SERVERS_FUNCTIONAL_ADDRESSING == 0U) && (DCCM_MAX_SERVERS_PHYSICAL_ADDRESSING == 0U))
    /* In this use-case the parameters are not used. */
    TS_PARAM_UNUSED(TxPduId);
    TS_PARAM_UNUSED(RxPduId);
    TS_PARAM_UNUSED(AddressingType);
#endif

#if (DCCM_MAX_SERVERS_FUNCTIONAL_ADDRESSING > 0U)
  if (AddressingType == FUNCTIONAL_ADDRESSING)
  {
/* Deviation TASKING-1 */
    for (i = 0; i < DCCM_MAX_SERVERS_FUNCTIONAL_ADDRESSING; i++)
    {
      if ((TxPduId == Dccm_FunctionalPduIdsArray[i].TxPduId) && (RxPduId == Dccm_FunctionalPduIdsArray[i].RxPduId))
      {
        RetVal = E_OK;
        break;
      }
    }
  }
#endif

#if (DCCM_MAX_SERVERS_PHYSICAL_ADDRESSING > 0U)
  if (AddressingType == PHYSICAL_ADDRESSING)
  {
/* Deviation TASKING-2 */
    for (i = 0; i < DCCM_MAX_SERVERS_PHYSICAL_ADDRESSING; i++)
    {
      if ((TxPduId == Dccm_PhysicalPduIdsArray[i].TxPduId) && (RxPduId == Dccm_PhysicalPduIdsArray[i].RxPduId))
      {
        RetVal = E_OK;
        break;
      }
    }
  }
#endif
  return RetVal;
}
/* Deviation MISRAC2012-1 <STOP> */


#define DCCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[end of file]============================================*/
