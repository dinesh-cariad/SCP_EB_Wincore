 /***************************************************************************
 *
 * \file Dccm_Validation.c
 *
 * \brief Dccm Validation
 *
 * Copyright 2020 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/
#include <Dccm_Validation.h>

/*==================[internal constants]=====================================*/

/*==================[internal function declarations]=========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateISOServices
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_DiagnosticAndCommunicationManagementServices
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_DataTransmissionServices
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_UploadDownloadServices
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateDiagnosticSessionControlBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateECUResetBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateSecurityAccessBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateCommunicationControlBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateTesterPresentBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateAccessTimingParameterBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateControlDTCSettingBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateLinkControlBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadDataByIdentifierBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadScalingDataByIdentifierBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateDynamicallyDefineDataIdentifierBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadDTCInformationBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadDTCInformationBasedOnRequest_1
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadDTCInformationBasedOnRequest_2
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadDTCInformationBasedOnRequest_3
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadDTCInformationBasedOnRequest_4
(
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateWriteDataByIdentifierBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateWriteMemoryByAddressBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateInputOutputControlByIdentifierBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateRoutineControlBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateTransferDataBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateRequestFileTransferBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateSecuredDataTransmissionBasedOnRequest
(
  uint32 RequestDataLength,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadMemoryByAddressBasedOnRequest
(
  uint32 RequestDataLength,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateClearDiagnosticInformationBasedOnRequest
(
  uint32 RequestDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateRequestDownloadBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateRequestUploadBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateLengthRequestFixedResponseMinumum
(
  uint32 RequestDataLength,
  uint32 LengthRequest,
  uint32 ResponseDataLength,
  uint32 LengthResponse
);

#define DCCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[external function definitions]==========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateRespBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  /* check if all parameters are valid */
  if (ResponseDataLength == DCCM_LENGTH_0)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  else if((RequestBuffer != NULL_PTR) && (RequestDataLength != DCCM_LENGTH_0) && (ResponseBuffer != NULL_PTR))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;

    /* if the response is negative */
    if(ResponseBuffer[0U] == NEGATIVE_RESPONSE_SID)
    {
      /* check the length, should be 3 bytes */
      if (ResponseDataLength != NEGATIVE_RESPONSE_LENGTH)
      {
        RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
      }
      else
      {
        /* verify if the SID from response (second byte) is the same as the SID from request (first byte) */
        if(ResponseBuffer[1U] == RequestBuffer[0U])
        {
          RetVal = E_OK;
        }
      }
    }
    else /* is not a negative response */
    {
      /* check if the SID from response is request + 40 */
      if (ResponseBuffer[0U] != (RequestBuffer[0U] + 0x40U))
      {
        RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
      }
      else
      {
        RetVal = Dccm_ValidateISOServices(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
      }
    }
  }
  else
  {
    /* nothing to do here */
  }
  return RetVal;
}
#define DCCM_STOP_SEC_CODE
#include <MemMap.h>


/*==================[internal function definitions]==========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateISOServices
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;


  switch(ResponseBuffer[0])
  {
    case SID_DIAGNOSTIC_SESSION_CONTROL_RSP:
    case SID_ECU_RESET_RSP:
    case SID_SECURITY_ACCESS_RSP:
    case SID_COMMUNICATION_CONTROL_RSP:
    case SID_TESTER_PRESENT_RSP:
    case SID_ACCESS_TIMING_PARAMETER_RSP:
    case SID_SECURED_DATA_TRANSMISSION_RSP:
    case SID_CONTROL_DTC_SETTING_RSP:
    case SID_LINK_CONTROL_RSP:
        RetVal = Dccm_DiagnosticAndCommunicationManagementServices(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
        break;

    case SID_READ_DATA_BY_IDENTIFIER_RSP:
    case SID_READ_MEMORY_BY_ADDRESS_RSP:
    case SID_READ_SCALING_DATA_BY_IDENTIFIER_RSP:
    case SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER_RSP:
    case SID_WRITE_DATA_BY_IDENTIFIER_RSP:
    case SID_WRITE_MEMORY_BY_ADDRESS_RSP:
        RetVal = Dccm_DataTransmissionServices(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
        break;

    case SID_CLEAR_DIAGNOSTIC_INFORMATION_RSP:
        RetVal = Dccm_ValidateClearDiagnosticInformationBasedOnRequest(RequestDataLength);
        break;

    case SID_READ_DTC_INFORMATION_RSP:
        RetVal = Dccm_ValidateReadDTCInformationBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
        break;

    case SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER_RSP:
        RetVal = Dccm_ValidateInputOutputControlByIdentifierBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
        break;

    case SID_ROUTINE_CONTROL_RSP:
        RetVal = Dccm_ValidateRoutineControlBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
        break;

    case SID_REQUEST_DOWNLOAD_RSP:
    case SID_REQUEST_UPLOAD_RSP:
    case SID_TRANSFER_DATA_RSP:
    case SID_FILE_TRANSFER_RSP:
        RetVal = Dccm_UploadDownloadServices(RequestBuffer,RequestDataLength, ResponseBuffer, ResponseDataLength);
        break;

    case SID_RESPONSE_ON_EVENT_RSP:
    case SID_READ_DATA_BY_PERIODIC_IDENTIFIER_RSP:
    case SID_REQUEST_TRANSFER_EXIT_RSP:
        /* ResponseOnEvent and ReadDataByPeriodicIdentifier are not supported by Dccm */

        /* Only for ReadDataByPeriodicIdentifier service: */
        /* it's not needed any additional verification */
        /* the minimum response is 1 byte that corresponds to the SID of the response and is already being checked */

        /* Only for RequestTransferExit service: */
        /* it's not needed any additional verification */
        /* the minimum response is 1 byte that corresponds to the SID of the response and is already being checked */
        /* the minimum request is 1 byte that corresponds to the SID of the request and is already being checked */
        break;

    default:
       RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
       break;
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateDiagnosticSessionControlBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* the length of the response must be 6 bytes */
  if (ResponseDataLength != DCCM_LENGTH_6)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* the length of the request is fixed to 2 bytes */
  else if(RequestDataLength != DCCM_LENGTH_2)
  {
    RetVal = E_NOT_OK;
  }
  /* sub-function from request shall be the same as the sub-function from response */
  else if ((RequestBuffer[1] & DCCM_SUBFUNCTION_MASK) != (ResponseBuffer[1] & DCCM_SUBFUNCTION_MASK))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  /* The suppressPosRspMsgIndicationBit from the sub-function parameter byte is not echoed and it value in response is set to zero */
  else if ((ResponseBuffer[1] & DCCM_SUPPRESSBIT_MASK) != 0x00U)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateECUResetBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* the length of the request is fixed to 2 bytes */
  if (RequestDataLength != DCCM_LENGTH_2)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response depends by the value of sub-function */
  /* if the sub-function parameter is set to the enableRapidPowerShutDown value (0x04) then the length must be 3 */
  else if ((ResponseBuffer[1] == DCCM_SERVICE_ECU_RESET_SUBFUNCTION_ENABLE_RAPID_POWER_SHUTDOWN) && (ResponseDataLength != DCCM_LENGTH_3))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* if the sub-function has another value the length shall be 2 */
  else if ((ResponseBuffer[1] != DCCM_SERVICE_ECU_RESET_SUBFUNCTION_ENABLE_RAPID_POWER_SHUTDOWN) && (ResponseDataLength != DCCM_LENGTH_2))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* sub-function from request shall be the same as the sub-function from response */
  else if ((RequestBuffer[1] & DCCM_SUBFUNCTION_MASK) != (ResponseBuffer[1] & DCCM_SUBFUNCTION_MASK))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  /* The suppressPosRspMsgIndicationBit from the sub-function parameter byte is not echoed and it value in response is set to zero */
  else if ((ResponseBuffer[1] & DCCM_SUPPRESSBIT_MASK) != 0x00U)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateSecurityAccessBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* at least 2 bytes shall be present in the request */
  if (RequestDataLength < DCCM_LENGTH_2)
  {
    RetVal = E_NOT_OK;
  }
  /* at least 2 bytes shall be present in the response */
  else if (ResponseDataLength < DCCM_LENGTH_2)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* sub-function from request shall be the same as the sub-function from response */
  else if ((RequestBuffer[1] & DCCM_SUBFUNCTION_MASK) != (ResponseBuffer[1] & DCCM_SUBFUNCTION_MASK))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  /* The suppressPosRspMsgIndicationBit from the sub-function parameter byte is not echoed and it value in response is set to zero */
  else if ((ResponseBuffer[1] & DCCM_SUPPRESSBIT_MASK) != 0x00U)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateCommunicationControlBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* at least 3 bytes shall be present in the request */
  if (RequestDataLength < DCCM_LENGTH_3)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response is fixed to 2 bytes */
  else if (ResponseDataLength != DCCM_LENGTH_2)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* sub-function from request shall be the same as the sub-function from response */
  else if ((RequestBuffer[1] & DCCM_SUBFUNCTION_MASK) != (ResponseBuffer[1] & DCCM_SUBFUNCTION_MASK))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  /* The suppressPosRspMsgIndicationBit from the sub-function parameter byte is not echoed and it value in response is set to zero */
  else if ((ResponseBuffer[1] & DCCM_SUPPRESSBIT_MASK) != 0x00U)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateTesterPresentBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;


  /* the length of the request is fixed to 2 bytes */
  if (RequestDataLength != DCCM_LENGTH_2)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response is fixed to 2 bytes */
  else if (ResponseDataLength != DCCM_LENGTH_2)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* The suppressPosRspMsgIndicationBit from the sub-function parameter byte is not echoed and it value in response is set to zero */
  else if ((ResponseBuffer[1] & DCCM_SUPPRESSBIT_MASK) != 0x00U)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  /* the second byte from the response (sub-function) shall be 0x00 */
  /* sub-function from request shall be the same as the sub-function from response */
  else if ((ResponseBuffer[1] != DCCM_ZERO_SUBFUNCTION) || ((RequestBuffer[1] & DCCM_SUBFUNCTION_MASK) != (ResponseBuffer[1] & DCCM_SUBFUNCTION_MASK)))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateAccessTimingParameterBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* at least 2 bytes shall be present in the request */
  if (RequestDataLength < DCCM_LENGTH_2)
  {
    RetVal = E_NOT_OK;
  }
  /* at least 2 bytes shall be present in the response */
  else if (ResponseDataLength < DCCM_LENGTH_2)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* the sub-function from request (timingParameterAccessType) shall be the same as the sub-function from response */
  else if ((RequestBuffer[1] & DCCM_SUBFUNCTION_MASK) != (ResponseBuffer[1] & DCCM_SUBFUNCTION_MASK))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  /* if sub-function is not readExtendedTimingParameterSet or readCurrentlyActiveTimingParameters */
  /* the response will not contain TimingParameterResponseRecord */
  else if((RequestBuffer[1] != DCCM_READ_EXTEND_TIMING_PARAMETER_SET) && (RequestBuffer[1] != DCCM_READ_CURRENTLY_ACTIVE_TIMING_PARAMETERS) && (ResponseDataLength != DCCM_LENGTH_2))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* The suppressPosRspMsgIndicationBit from the sub-function parameter byte is not echoed and it value in response is set to zero */
  else if ((ResponseBuffer[1] & DCCM_SUPPRESSBIT_MASK) != 0x00U)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  /* TimingParameterResponseRecord is only present if timingParameterAccessType = readExtendedTimingParameterSet or readCurrentlyActiveTimingParameters */
  else if (((RequestBuffer[1] == DCCM_READ_EXTEND_TIMING_PARAMETER_SET) || (RequestBuffer[1] == DCCM_READ_CURRENTLY_ACTIVE_TIMING_PARAMETERS)) && (ResponseDataLength < DCCM_LENGTH_3))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateControlDTCSettingBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* at least 2 bytes shall be present in the request */
  if (RequestDataLength < DCCM_LENGTH_2)
  {
   RetVal = E_NOT_OK;
  }
  /* the length of the response is fixed to 2 bytes */
  else if (ResponseDataLength != DCCM_LENGTH_2)
  {
   RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* sub-function from request shall be the same as the sub-function from response */
  else if ((RequestBuffer[1] & DCCM_SUBFUNCTION_MASK) != (ResponseBuffer[1] & DCCM_SUBFUNCTION_MASK))
  {
   RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  /* The suppressPosRspMsgIndicationBit from the sub-function parameter byte is not echoed and it value in response is set to zero */
  else if ((ResponseBuffer[1] & DCCM_SUPPRESSBIT_MASK) != 0x00U)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateLinkControlBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* the length of the request must be at least 2 bytes */
  if (RequestDataLength < DCCM_LENGTH_2)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response is fixed to 2 bytes */
  else if (ResponseDataLength != DCCM_LENGTH_2)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* sub-function from request shall be the same as the sub-function from response */
  else if ((RequestBuffer[1] & DCCM_SUBFUNCTION_MASK) != (ResponseBuffer[1] & DCCM_SUBFUNCTION_MASK))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  /* The suppressPosRspMsgIndicationBit from the sub-function parameter byte is not echoed and it value in response is set to zero */
  else if ((ResponseBuffer[1] & DCCM_SUPPRESSBIT_MASK) != 0x00U)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadDataByIdentifierBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;
  uint32 count;
  boolean found = FALSE;

  /* the length of the request must be at least 3 bytes */
  if (RequestDataLength < DCCM_LENGTH_3)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response must be at least 4 bytes */
  else if (ResponseDataLength < DCCM_LENGTH_4)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  else /* dataIdentifier from response shall be found in the list of dataIdentifiers from request */
  {
    for(count = 1U; count < RequestDataLength; count = count + 2U)
    {
      if ((RequestBuffer[count] == ResponseBuffer[1]) && (RequestBuffer[count + 1U] == ResponseBuffer[2]))
      {
        found = TRUE;
        break;
      }
    }

    if(found == FALSE)
    {
      RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
    }
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadScalingDataByIdentifierBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;
  uint8 highNibble = 0U;
  uint8 lowNibble = 0U;

  /* the length of the request is fixed to 3 bytes */
  if (RequestDataLength != DCCM_LENGTH_3)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response must be at least 4 bytes */
  else if (ResponseDataLength < DCCM_LENGTH_4)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* dataIdentifier from request shall be the same as the dataIdentifier from response */
  else if ((RequestBuffer[1] != ResponseBuffer[1]) || (RequestBuffer[2] != ResponseBuffer[2]))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else
  {
    /* scalingByte parameter (High Nibble, bits 7-4) */
    highNibble = ResponseBuffer[3] >> BITS_TO_SHIFT_4;
    /* scalingByte parameter (Low Nibble, bits 3-0 ) - represents the length of scalingByteExtension parameter */
    lowNibble = ResponseBuffer[3] & BITS3210_BIT_MASK;

    /* validate scalingByte if the high nibble is encoded as formula, unit/format, or bitMappedReportedWithOutMask*/
    switch (highNibble)
    {
      case DCCM_BIT_MAPPED_REPORTED_WITH_OUT_MASK:

        /* it is mandatory to have at least one byte in scalingByteExtension parameter */
        if (lowNibble < DCCM_LENGTH_1)
        {
          RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
        }
        /* verify that at least the length announced by lowNibble for the first scalingByte has been received */
        else if ((ResponseDataLength - DCCM_LENGTH_4) < lowNibble)
        {
          RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
        }
        else
        {
          /* Nothing to do here */
        }

        break;

      case DCCM_SCALING_BYTE_FORMULA:

        /* it is mandatory to have at least 3 bytes in scalingByteExtension parameter */
        if (lowNibble < DCCM_LENGTH_3)
        {
          RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
        }
        /* verify that at least the length announced by lowNibble for the first scalingByte has been received */
        else if ((ResponseDataLength - DCCM_LENGTH_4) < lowNibble)
        {
          RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
        }
        else
        {
          /* Nothing to do here */
        }

        break;

      case DCCM_SCALING_BYTE_UNIT_FORMAT:

        /* it is mandatory to have at least 1 byte in scalingByteExtension parameter */
        if (lowNibble < DCCM_LENGTH_1)
        {
          RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
        }
        /* verify that at least the length announced by lowNibble for the first scalingByte has been received */
        else if ((ResponseDataLength - DCCM_LENGTH_4) < lowNibble)
        {
          RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
        }
        else
        {
          /* Nothing to do here */
        }

        break;

      default:
        /* Nothing to do here */
        break;
    }
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateDynamicallyDefineDataIdentifierBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* the length of the request must be at least 2 bytes */
  if (RequestDataLength < DCCM_LENGTH_2)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response must be at least 2 bytes */
  else if (ResponseDataLength < DCCM_LENGTH_2)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* sub-function from request (definitionType) shall be the same as the sub-function from response */
  else if ((RequestBuffer[1] & DCCM_SUBFUNCTION_MASK) != (ResponseBuffer[1] & DCCM_SUBFUNCTION_MASK))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  /* The suppressPosRspMsgIndicationBit from the sub-function parameter byte is not echoed and it value in response is set to zero */
  else if ((ResponseBuffer[1] & DCCM_SUPPRESSBIT_MASK) != 0x00U)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else if ((RequestBuffer[1] == DCCM_DEFINE_BY_MEMORY_ADDRESS) || (RequestBuffer[1] == DCCM_DEFINE_BY_IDENTIFYER))
  {
    if ((ResponseDataLength < DCCM_LENGTH_4) || (RequestDataLength < DCCM_LENGTH_4))
    {
      RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
    }
    /* dynamicallyDefinedDataIdentifier parameter from request shall be present in response */
    else if ((RequestBuffer[2] != ResponseBuffer[2]) || (RequestBuffer[3] != ResponseBuffer[3]))
    {
      RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
    }
    /* byte MSB from dynamicallyDefinedDataIdentifier shall be 0xF2U or 0xF3U */
    else if ((ResponseBuffer[2] != 0xF2U) && (ResponseBuffer[2] != 0xF3U))
    {
      RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
    }
    else
    {
      /* nothing to do here */
    }
  }
  /* if sub-function = clearDynamicallyDefinedDataIdentifier and it has dynamicallyDefinedDataIdentifier parameter */
  else if((RequestBuffer[1] == DCCM_CLEAR_DYNAMICALLY_DEFINED_DATA_IDENTIFIER) && (RequestDataLength == DCCM_LENGTH_4))
  {
    /* the response shall contain dynamicallyDefinedDataIdentifier parameter */
    if(ResponseDataLength != DCCM_LENGTH_4)
    {
      RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
    }
    /* dynamicallyDefinedDataIdentifier parameter from request shall be present in response */
    else if ((RequestBuffer[2] != ResponseBuffer[2]) || (RequestBuffer[3] != ResponseBuffer[3]))
    {
      RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
    }
    /* byte MSB from dynamicallyDefinedDataIdentifier shall be 0xF2U or 0xF3U */
    else if ((ResponseBuffer[2] != 0xF2U) && (ResponseBuffer[2] != 0xF3U))
    {
      RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
    }
    else
    {
      /* nothing to do here */
    }
  }
  else
  {
    /* nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateSecuredDataTransmissionBasedOnRequest
(
  uint32 RequestDataLength,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* the length of the request must be at least 2 bytes */
  if (RequestDataLength < DCCM_LENGTH_2)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response must be at least 2 bytes */
  else if (ResponseDataLength < DCCM_LENGTH_2)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadMemoryByAddressBasedOnRequest
(
  uint32 RequestDataLength,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* the length of the request must be at least 4 bytes */
  if (RequestDataLength < DCCM_LENGTH_4)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response must be at least 2 bytes */
  else if (ResponseDataLength < DCCM_LENGTH_2)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateClearDiagnosticInformationBasedOnRequest
(
  uint32 RequestDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* the length of the request must be 4 bytes */
  if (RequestDataLength != DCCM_LENGTH_4)
  {
    RetVal = E_NOT_OK;
  }
  /* the minimum response is 1 byte that corresponds to the SID of the response and is already being checked */
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateRequestDownloadBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;
  /* parameters for LengthFormatIdentifier */
  uint8 LFI_Bits7654Of8Bits = 0U;
  uint8 LFI_Bits3210Of8Bits = 0U;
  /* parameters for addressAndLengthFormatIdentifier */
  uint8 AALFI_Bits7654Of8Bits = 0U;
  uint8 AALFI_Bits3210Of8Bits = 0U;
  uint8 LengthAnnouncedInParametersRequest = 0U;

  /* the length of the request must be at least 5 bytes */
  if (RequestDataLength < DCCM_LENGTH_5)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response must be at least 3 bytes */
  else if (ResponseDataLength < DCCM_LENGTH_3)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  else
  {
    /* Validation for Request */
    /* split the parameter addressAndLengthFormatIdentifier */
    /* bit 7 - 4: Length (number of bytes) of the maxNumberOfBlockLength parameter.
       bit 3 - 0: reserved by ISO document, to be set to '0'.*/
    AALFI_Bits7654Of8Bits = RequestBuffer[2] >> BITS_TO_SHIFT_4;
    AALFI_Bits3210Of8Bits = RequestBuffer[2] & BITS3210_BIT_MASK;

    /* check if memoryAddress + memorySize respect the announced size */
    LengthAnnouncedInParametersRequest = AALFI_Bits7654Of8Bits + AALFI_Bits3210Of8Bits;
    if ((RequestDataLength - DCCM_LENGTH_3) != LengthAnnouncedInParametersRequest)
    {
      RetVal = E_NOT_OK;
    }

    /* Validation for Response */
    /* split the parameter LengthFormatIdentifier */
    /* bit 7 - 4: Length (number of bytes) of the maxNumberOfBlockLength parameter.
       bit 3 - 0: reserved by ISO document, to be set to '0'.*/
    LFI_Bits7654Of8Bits = ResponseBuffer[1] >> BITS_TO_SHIFT_4;
    LFI_Bits3210Of8Bits = ResponseBuffer[1] & BITS3210_BIT_MASK;

    /* check if bit 3 - 0 is set to '0' */
    if(LFI_Bits3210Of8Bits != 0U)
    {
      RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
    }
    else
    {
      /* check for maxNumberOfBlockLength parameter */
      /* from the entire response length remove 2 bytes (SID and lengthFormatIdentifier) */
      /* what is left is the length of the maxNumberOfBlockLength parameter */
      if ((ResponseDataLength - DCCM_LENGTH_2) != LFI_Bits7654Of8Bits)
      {
        RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
      }
    }
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateRequestUploadBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;
  /* parameters for LengthFormatIdentifier */
  uint8 LFI_Bits7654Of8Bits = 0U;
  uint8 LFI_Bits3210Of8Bits = 0U;
  /* parameters for addressAndLengthFormatIdentifier */
  uint8 AALFI_Bits7654Of8Bits = 0U;
  uint8 AALFI_Bits3210Of8Bits = 0U;
  uint8 LengthAnnouncedInParametersRequest = 0U;

  /* the length of the request must be at least 5 bytes */
  if (RequestDataLength < DCCM_LENGTH_5)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response must be at least 3 bytes */
  else if (ResponseDataLength < DCCM_LENGTH_3)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  else
  {
    /* Validation for Request */
    /* split the parameter addressAndLengthFormatIdentifier */
    /* bit 7 - 4: Length (number of bytes) of the maxNumberOfBlockLength parameter.
       bit 3 - 0: reserved by ISO document, to be set to '0'.*/
    AALFI_Bits7654Of8Bits = RequestBuffer[2] >> BITS_TO_SHIFT_4;
    AALFI_Bits3210Of8Bits = RequestBuffer[2] & BITS3210_BIT_MASK;

    /* check if memoryAddress + memorySize respect the announced size */
    LengthAnnouncedInParametersRequest = AALFI_Bits7654Of8Bits + AALFI_Bits3210Of8Bits;
    if ((RequestDataLength - DCCM_LENGTH_3) != LengthAnnouncedInParametersRequest)
    {
      RetVal = E_NOT_OK;
    }

    /* Validation for Response */
    /* split the parameter LengthFormatIdentifier */
    /* bit 7 - 4: Length (number of bytes) of the maxNumberOfBlockLength parameter.
       bit 3 - 0: reserved by ISO document, to be set to '0'.*/
    LFI_Bits7654Of8Bits = ResponseBuffer[1] >> BITS_TO_SHIFT_4;
    LFI_Bits3210Of8Bits = ResponseBuffer[1] & BITS3210_BIT_MASK;

    /* check if bit 3 - 0 is set to '0' */
    if(LFI_Bits3210Of8Bits != 0U)
    {
      RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
    }
    else
    {
      /* check for maxNumberOfBlockLength parameter */
      /* from the entire response length remove 2 bytes (SID and lengthFormatIdentifier) */
      /* what is left is the length of the maxNumberOfBlockLength parameter */
      if ((ResponseDataLength - DCCM_LENGTH_2) != LFI_Bits7654Of8Bits)
      {
        RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
      }
    }
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateWriteDataByIdentifierBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* the length of the request must be at least 4 bytes */
  if (RequestDataLength < DCCM_LENGTH_4)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response is fixed to 3 bytes */
  else if (ResponseDataLength != DCCM_LENGTH_3)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* dataIdentifier from request shall be the same as the dataIdentifier from response */
  else if ((RequestBuffer[1] != ResponseBuffer[1]) || (RequestBuffer[2] != ResponseBuffer[2]))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateWriteMemoryByAddressBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;
  /* parameters for addressAndLengthFormatIdentifier */
  uint8 AALFI_Bits7654Of8Bits = 0U;
  uint8 AALFI_Bits3210Of8Bits = 0U;
  uint8 LengthAnnouncedInParametersRequest = 0U;
  uint8 LengthAnnouncedInParametersResponse = 0U;
  uint32 count = 0U;

  /* the length of the request must be at least 5 bytes */
  if (RequestDataLength < DCCM_LENGTH_5)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response must be at least 4 bytes */
  else if (ResponseDataLength < DCCM_LENGTH_4)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* addressAndLengthFormatIdentifier from request shall be the same as the addressAndLengthFormatIdentifier from response */
  else if (RequestBuffer[1] != ResponseBuffer[1])
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else
  {
    /* Validation for Request */
    /* split the parameter addressAndLengthFormatIdentifier */
    /* addressAndLengthFormatIdentifier contain the length of memoryAddress + memorySize */
    /* bit 7 - 4: Length (number of bytes) of the memorySize parameter. */
    /* bit 3 - 0: Length (number of bytes) of the memoryAddress parameter.*/
    AALFI_Bits7654Of8Bits = RequestBuffer[1] >> BITS_TO_SHIFT_4;
    AALFI_Bits3210Of8Bits = RequestBuffer[1] & BITS3210_BIT_MASK;

    /* length of memorySize + memoryAddress + SID + addressAndLengthFormatIdentifier + at least 1 byte of dataRecord */
    LengthAnnouncedInParametersRequest = AALFI_Bits7654Of8Bits + AALFI_Bits3210Of8Bits + DCCM_LENGTH_3;
    /* length of memorySize + memoryAddress + SID + addressAndLengthFormatIdentifier */
    LengthAnnouncedInParametersResponse = AALFI_Bits7654Of8Bits + AALFI_Bits3210Of8Bits + DCCM_LENGTH_2;

    /* check that the request has at least the announced length */
    if (LengthAnnouncedInParametersRequest < RequestDataLength)
    {
      RetVal = E_NOT_OK;
    }
    /* check that the response has the announced length */
    else if (LengthAnnouncedInParametersResponse != ResponseDataLength)
    {
      RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
    }
    else
    {
      /* check for the echoed parameter memoryAddress */
      for (count = 0U; count < AALFI_Bits3210Of8Bits; count++)
      {
        if (RequestBuffer[count + 2U] != ResponseBuffer[count + 2U])
        {
          RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
          break;
        }
      }

      /* check for the echoed parameter memorySize */
      for (count = 0U; count < AALFI_Bits7654Of8Bits; count++)
      {
        if (RequestBuffer[count + 2U + AALFI_Bits3210Of8Bits] != ResponseBuffer[count + 2U + AALFI_Bits3210Of8Bits])
        {
          RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
          break;
        }
      }
    }

  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateInputOutputControlByIdentifierBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* the length of the request must be at least 4 bytes */
  if (RequestDataLength < DCCM_LENGTH_4)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response must be at least 4 bytes */
  else if (ResponseDataLength < DCCM_LENGTH_4)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* dataIdentifier from request shall be the same as the dataIdentifier from response */
  else if ((RequestBuffer[1] != ResponseBuffer[1]) || (RequestBuffer[2] != ResponseBuffer[2]))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateRoutineControlBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* the length of the request must be at least 4 bytes */
  if (RequestDataLength < DCCM_LENGTH_4)
  {
    RetVal = E_NOT_OK;
  }
  /* the length of the response must be at least 4 bytes */
  else if (ResponseDataLength < DCCM_LENGTH_4)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* sub-function (second byte, index 1) from request shall be the same as the routineControlType (second byte, index 1) from response */
  /* dataIdentifier from request (bytes 3 and 4, indexes 2 and 3) shall be the same as the dataIdentifier from response  (bytes 3 and 4, indexes 2 and 3) */
  else if (((RequestBuffer[1] & DCCM_SUBFUNCTION_MASK) != (ResponseBuffer[1] & DCCM_SUBFUNCTION_MASK)) || (RequestBuffer[2] != ResponseBuffer[2]) || (RequestBuffer[3] != ResponseBuffer[3]))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  /* The suppressPosRspMsgIndicationBit from the sub-function parameter byte is not echoed and it value in response is set to zero */
  else if ((ResponseBuffer[1] & DCCM_SUPPRESSBIT_MASK) != 0x00U)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateTransferDataBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* at least 2 bytes shall be present in the request */
  if (RequestDataLength < DCCM_LENGTH_2)
  {
    RetVal = E_NOT_OK;
  }
  /* at least 2 bytes shall be present in the response */
  else if (ResponseDataLength < DCCM_LENGTH_2)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* the second byte from request (blockSequenceCounter) shall be the same as the second byte from response */
  else if (RequestBuffer[1] != ResponseBuffer[1])
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateRequestFileTransferBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;
  uint8 indexDataFormatIdentifier_Response = 0U;
  uint8 indexDataFormatIdentifier_Request = 0U;
  uint16 sizeOfFilePathAndName = 0U;
  uint8 LengthAnnouncedInParametersResponse = 0U;
  uint8 LengthAnnouncedInParametersRequest = 0U;

  /* at least 5 bytes shall be present in the request */
  if (RequestDataLength < DCCM_LENGTH_5)
  {
    RetVal = E_NOT_OK;
  }
  /* at least 2 bytes shall be present in the response */
  else if (ResponseDataLength < DCCM_LENGTH_2)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* the second byte from request (modeOfOperation) shall be the same as the second byte from response */
  else if (RequestBuffer[1] != ResponseBuffer[1])
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  /* modeOfOperation shall contain only values from 0x01U to 0x05U */
  else if ((RequestBuffer[1] < 0x01U) || (RequestBuffer[1] > 0x05U))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  /* If the modeOfOperation parameter equals to 0x02 (DeleteFile) dataFormatIdentifier parameter */
  /* shall not be included in the response message.*/
  else if ((ResponseBuffer[1] != DCCM_MODE_OF_OPERATION_DELETE_FILE) && (ResponseDataLength > DCCM_LENGTH_3))
  {
    /* maxNumberOfBlockLength + SID + modeOfOperation + lengthFormatIdentifier + DataFormatIdentifier */
    LengthAnnouncedInParametersResponse = ResponseBuffer[2] + DCCM_LENGTH_4;
    if (LengthAnnouncedInParametersResponse > ResponseDataLength )
    {
      RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
    }
    else
    {

      /* find the index of DataFormatIdentifier parameter in response */
      /* value of lengthFormatIdentifier parameter + SID + modeOfOperation + lengthFormatIdentifier */
      indexDataFormatIdentifier_Response = ResponseBuffer[2] + DCCM_LENGTH_3;

      if (RequestBuffer[1] == DCCM_MODE_OF_OPERATION_READ_DIR)
      {

        /* If the modeOfOperation parameter equals to 0x05 (ReadDir) the value of dataFormatIdentifier parameter shall be equal to 0x00 */
        if (ResponseBuffer[indexDataFormatIdentifier_Response] != 0x00U)
        {
          RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
        }
      }
      else
      {
        /* calculate FilePathAndName size from filePathAndNameLength MSB and filePathAndNameLength LSB */
        sizeOfFilePathAndName = (((uint16)(RequestBuffer[2])) << BITS_TO_SHIFT_8) | RequestBuffer[3];

        /* filePathAndName + ( SID + modeOfOperation + filePathAndNameLength MSB + filePathAndNameLength LSB + dataFormatIdentifier) */
        LengthAnnouncedInParametersRequest = sizeOfFilePathAndName + DCCM_LENGTH_5;
        if (LengthAnnouncedInParametersRequest > RequestDataLength)
        {
          RetVal = E_NOT_OK;
        }
        else
        {
          /* find the index of DataFormatIdentifier parameter in request */
          /* value of filePathAndName + (SID + modeOfOperation + filePathAndNameLength MSB + LSB) */
          indexDataFormatIdentifier_Request = sizeOfFilePathAndName + DCCM_LENGTH_4;

          /* check for the echo DataFormatIdentifier parameter */
          if (ResponseBuffer[indexDataFormatIdentifier_Response] != RequestBuffer[indexDataFormatIdentifier_Request])
          {
            RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
          }
        }
      }
    }
  }
  else
  {
  /* Nothing to do here */
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadDTCInformationBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  /* The response size and format varies, depending on sub-function */
  /* at least 2 bytes shall be present in the response to have the byte with sub-function*/
  if (ResponseDataLength < DCCM_LENGTH_2)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  /* The suppressPosRspMsgIndicationBit from the sub-function parameter byte is not echoed and it value in response is set to zero */
  else if ((ResponseBuffer[1] & DCCM_SUPPRESSBIT_MASK) != 0x00U)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  /* sub-function from request shall be the same as the sub-function from response */
  else if((RequestBuffer[1] & DCCM_SUBFUNCTION_MASK) != (ResponseBuffer[1] & DCCM_SUBFUNCTION_MASK))
  {
    RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
  }
  else
  {
    switch (ResponseBuffer[1])
    {
        case 0x02U:
        case 0x04U:
        case 0x05U:
        case 0x08U:
        case 0x0FU:
        case 0x13U:
        case 0x16U:
        case 0x19U:
            RetVal = Dccm_ValidateReadDTCInformationBasedOnRequest_1(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
            break;

        case 0x09U:
        case 0x0BU:
        case 0x0CU:
        case 0x0DU:
        case 0x0EU:
        case 0x17U:
        case 0x42U:
        case 0x55U:
          RetVal = Dccm_ValidateReadDTCInformationBasedOnRequest_2(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
          break;

        case 0x03U:
        case 0x07U:
        case 0x11U:
        case 0x12U:
        case 0x14U:
        case 0x18U:
          RetVal = Dccm_ValidateReadDTCInformationBasedOnRequest_3(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
          break;

        case 0x01U:
        case 0x06U:
        case 0x0AU:
        case 0x10U:
        case 0x15U:
            RetVal = Dccm_ValidateReadDTCInformationBasedOnRequest_4(RequestDataLength, ResponseBuffer, ResponseDataLength);
            break;

        default: /* the sub-function is not valid */
          RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
          break;
    }
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadDTCInformationBasedOnRequest_1
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

 if (ResponseBuffer[1] == 0x16U)
 {
    /* RequestDataLength shall have 3 bytes and ResponseDataLength at least 3 bytes */
    RetVal = Dccm_ValidateLengthRequestFixedResponseMinumum(RequestDataLength, DCCM_LENGTH_3, ResponseDataLength, DCCM_LENGTH_3);

    if (E_OK == RetVal)
    {
      /* DTCExtDataRecordNumber shall contain only values between 0x00U and 0xEFU */
      if(ResponseBuffer[2] > DCCM_MAX_DTC_EXT_DATA_RECORD_NR_16)
      {
        RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
      }
    }
  }
  else if ((ResponseBuffer[1] == 0x02U) || (ResponseBuffer[1] == 0x05U) || (ResponseBuffer[1] == 0x0FU) || (ResponseBuffer[1] == 0x13U))
  {
    /* RequestDataLength shall have 3 bytes and ResponseDataLength at least 3 bytes */
    RetVal = Dccm_ValidateLengthRequestFixedResponseMinumum(RequestDataLength, DCCM_LENGTH_3, ResponseDataLength, DCCM_LENGTH_3);
  }
  else if (ResponseBuffer[1] == 0x04U)
  {
    if (RequestDataLength < DCCM_LENGTH_2)
    {
      RetVal = E_NOT_OK;
    }
    else
    {
      if (ResponseDataLength < DCCM_LENGTH_6)
      {
         RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
      }
    }
  }
  else if (ResponseBuffer[1] == 0x19U)
  {
    /* RequestDataLength shall have 7 bytes and ResponseDataLength at least 7 bytes */
    RetVal = Dccm_ValidateLengthRequestFixedResponseMinumum(RequestDataLength, DCCM_LENGTH_7, ResponseDataLength, DCCM_LENGTH_7);

    if( E_OK == RetVal)
    {
      /* check for the echo parameter MemorySelection */
      if(ResponseBuffer[2] != RequestBuffer[6])
      {
        RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
      }
      else
      {
        /* if the 8th byte is present */
        /* DTCExtDataRecordNumber shall contain only values between 0x00U and 0xFE */
        if((ResponseDataLength >= DCCM_LENGTH_8) && (ResponseBuffer[7] > DCCM_MAX_DTC_EXT_DATA_RECORD_NR_19))
        {
          RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
        }
      }
    }
  }
  else
  {
    /* ResponseBuffer[1] == 0x08U */
    /* RequestDataLength shall have 4 bytes and ResponseDataLength at least 3 bytes */
    RetVal = Dccm_ValidateLengthRequestFixedResponseMinumum(RequestDataLength, DCCM_LENGTH_4, ResponseDataLength, DCCM_LENGTH_3);
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadDTCInformationBasedOnRequest_2
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  if ((ResponseBuffer[1] == 0x0BU) || (ResponseBuffer[1] == 0x0CU) || (ResponseBuffer[1] == 0x0DU) || (ResponseBuffer[1] == 0x0EU))
  {
    /* RequestDataLength shall have 2 bytes and ResponseDataLength at least 3 bytes */
    RetVal = Dccm_ValidateLengthRequestFixedResponseMinumum(RequestDataLength, DCCM_LENGTH_2, ResponseDataLength, DCCM_LENGTH_3);
  }
  else if (ResponseBuffer[1] == 0x09U)
  {
    /* RequestDataLength shall have 5 bytes and ResponseDataLength at least 3 bytes */
    RetVal = Dccm_ValidateLengthRequestFixedResponseMinumum(RequestDataLength, DCCM_LENGTH_5, ResponseDataLength, DCCM_LENGTH_3);
  }
  else if (ResponseBuffer[1] == 0x17U)
  {
    /* RequestDataLength shall have 4 bytes and ResponseDataLength at least 4 bytes */
    RetVal = Dccm_ValidateLengthRequestFixedResponseMinumum(RequestDataLength, DCCM_LENGTH_4, ResponseDataLength, DCCM_LENGTH_4);

    if (E_OK == RetVal)
    {
      /* check for the echo parameter MemorySelection */
      if(ResponseBuffer[2] != RequestBuffer[3])
      {
        RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
      }
    }
  }
  else if (ResponseBuffer[1] == 0x42U)
  {
    /* RequestDataLength shall have 5 bytes and ResponseDataLength at least 6 bytes */
    RetVal = Dccm_ValidateLengthRequestFixedResponseMinumum(RequestDataLength, DCCM_LENGTH_5, ResponseDataLength, DCCM_LENGTH_6);

    if (E_OK == RetVal)
    {
      if((ResponseBuffer[5] != DCCM_DTCFORMAT_2) && (ResponseBuffer[5] != DCCM_DTCFORMAT_4))
      {
        RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
      }
    }
  }
  else
  {
    /* ResponseBuffer[1] == 0x55U */
    /* RequestDataLength shall have 3 bytes and ResponseDataLength at least 5 bytes */
    RetVal = Dccm_ValidateLengthRequestFixedResponseMinumum(RequestDataLength, DCCM_LENGTH_3, ResponseDataLength, DCCM_LENGTH_5);

    if (E_OK == RetVal)
    {
      if((ResponseBuffer[4] != DCCM_DTCFORMAT_2) && (ResponseBuffer[4] != DCCM_DTCFORMAT_4))
      {
        RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
      }
    }
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadDTCInformationBasedOnRequest_3
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;
  uint32 counter = 0U;

  if ((ResponseBuffer[1] == 0x11U) || (ResponseBuffer[1] == 0x12U))
  {
    if (RequestDataLength != DCCM_LENGTH_3)
    {
      RetVal = E_NOT_OK;
    }
    else if (ResponseDataLength != DCCM_LENGTH_6)
    {
      RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
    }
    else
    {
      if (ResponseBuffer[3] > 0x04U)
      {
        RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
      }
    }
  }
  else if (ResponseBuffer[1] == 0x14U)
  {
    if (RequestDataLength != DCCM_LENGTH_2)
    {
      RetVal = E_NOT_OK;
    }
    else
    {
      /* if DTCFaultDetectionCounterRecord is present */
      if(ResponseDataLength > DCCM_LENGTH_5)
      {
        /* the 4th parameter, DTCFaultDetectionCounter, shall contain values between 0x01-0xFF*/
        for(counter = 5U; counter < ResponseDataLength; counter = counter + 4U)
        {
          if(ResponseBuffer[counter] == 0x0U)
          {
            RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
            break;
          }
        }
      }
    }
  }
  else if (ResponseBuffer[1] == 0x07U)
  {
    if (RequestDataLength != DCCM_LENGTH_4)
    {
      RetVal = E_NOT_OK;
    }
    else if (ResponseDataLength != DCCM_LENGTH_6)
    {
      RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
    }
    else
    {
      if (ResponseBuffer[3] > 0x04U)
      {
        RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
      }
    }
  }
  else if (ResponseBuffer[1] == 0x18U)
  {
    /* RequestDataLength shall have 7 bytes and ResponseDataLength at least 7 bytes */
    RetVal = Dccm_ValidateLengthRequestFixedResponseMinumum(RequestDataLength, DCCM_LENGTH_7, ResponseDataLength, DCCM_LENGTH_7);

    if (E_OK == RetVal)
    {
      /* check for the echo parameter MemorySelection */
      if(ResponseBuffer[2] != RequestBuffer[6])
      {
        RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
      }
    }
  }
  else
  {
     /* ResponseBuffer[1] == 0x03U */
     if (RequestDataLength < DCCM_LENGTH_2)
     {
        RetVal = E_NOT_OK;
     }
  }

  return RetVal;
}


STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateReadDTCInformationBasedOnRequest_4
(
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  if ((ResponseBuffer[1] == 0x06U) || (ResponseBuffer[1] == 0x10U))
  {
    /* RequestDataLength shall have 6 bytes and ResponseDataLength at least 6 bytes */
    RetVal = Dccm_ValidateLengthRequestFixedResponseMinumum(RequestDataLength, DCCM_LENGTH_6, ResponseDataLength, DCCM_LENGTH_6);

    if (E_OK == RetVal)
    {
       /* if the 7th byte is present */
       if(ResponseDataLength >= DCCM_LENGTH_7)
       {
         /* DTCExtDataRecordNumber value must be between 0x00U and 0xFDU */
         if (ResponseBuffer[6] > DCCM_MAX_DTC_EXT_DATA_RECORD_NR_6_10)
         {
           RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
         }
       }
    }
  }
  else if ((ResponseBuffer[1] == 0x0AU) || (ResponseBuffer[1] == 0x15U))
  {
    /* RequestDataLength shall have 2 bytes and ResponseDataLength at least 3 bytes */
    RetVal = Dccm_ValidateLengthRequestFixedResponseMinumum(RequestDataLength, DCCM_LENGTH_2, ResponseDataLength, DCCM_LENGTH_3);
  }
  else
  {
    /* ResponseBuffer[1] == 0x01U */
    if (RequestDataLength != DCCM_LENGTH_3)
    {
      RetVal = E_NOT_OK;
    }
    else if (ResponseDataLength != DCCM_LENGTH_6)
    {
      RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
    }
    else
    {
      if (ResponseBuffer[3] > 0x04U)
      {
        RetVal = DCCM_E_INVALID_RESPONSE_FORMAT;
      }
    }
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_DiagnosticAndCommunicationManagementServices
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  if (ResponseBuffer[0] == SID_DIAGNOSTIC_SESSION_CONTROL_RSP)
  {
    RetVal = Dccm_ValidateDiagnosticSessionControlBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }
  else if (ResponseBuffer[0] == SID_ECU_RESET_RSP)
  {
    RetVal = Dccm_ValidateECUResetBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }
  else if (ResponseBuffer[0] == SID_SECURITY_ACCESS_RSP)
  {
    RetVal = Dccm_ValidateSecurityAccessBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }
  else if (ResponseBuffer[0] == SID_COMMUNICATION_CONTROL_RSP)
  {
    RetVal = Dccm_ValidateCommunicationControlBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }
  else if (ResponseBuffer[0] == SID_TESTER_PRESENT_RSP)
  {
    RetVal = Dccm_ValidateTesterPresentBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }
  else if (ResponseBuffer[0] == SID_ACCESS_TIMING_PARAMETER_RSP)
  {
    RetVal = Dccm_ValidateAccessTimingParameterBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }
  else if (ResponseBuffer[0] == SID_SECURED_DATA_TRANSMISSION_RSP)
  {
    RetVal = Dccm_ValidateSecuredDataTransmissionBasedOnRequest(RequestDataLength, ResponseDataLength);
  }
  else if (ResponseBuffer[0] == SID_CONTROL_DTC_SETTING_RSP)
  {
    RetVal = Dccm_ValidateControlDTCSettingBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }
  else
  {
    /* ResponseBuffer[0] == SID_CONTROL_DTC_SETTING_RSP */
    RetVal = Dccm_ValidateLinkControlBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_DataTransmissionServices
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  if (ResponseBuffer[0] == SID_READ_DATA_BY_IDENTIFIER_RSP)
  {
    RetVal = Dccm_ValidateReadDataByIdentifierBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }
  else if (ResponseBuffer[0] == SID_READ_MEMORY_BY_ADDRESS_RSP)
  {
    RetVal = Dccm_ValidateReadMemoryByAddressBasedOnRequest(RequestDataLength, ResponseDataLength);
  }
  else if (ResponseBuffer[0] == SID_READ_SCALING_DATA_BY_IDENTIFIER_RSP)
  {
    RetVal = Dccm_ValidateReadScalingDataByIdentifierBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }
  else if (ResponseBuffer[0] == SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER_RSP)
  {
    RetVal = Dccm_ValidateDynamicallyDefineDataIdentifierBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }
  else if (ResponseBuffer[0] == SID_WRITE_DATA_BY_IDENTIFIER_RSP)
  {
    RetVal = Dccm_ValidateWriteDataByIdentifierBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }
  else
  {
    /* ResponseBuffer[0] == SID_WRITE_MEMORY_BY_ADDRESS_RSP */
    RetVal = Dccm_ValidateWriteMemoryByAddressBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_UploadDownloadServices
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
)
{
  Std_ReturnType RetVal = E_OK;

  if (ResponseBuffer[0] == SID_REQUEST_DOWNLOAD_RSP)
  {
    RetVal = Dccm_ValidateRequestDownloadBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }
  else if (ResponseBuffer[0] == SID_REQUEST_UPLOAD_RSP)
  {
    RetVal = Dccm_ValidateRequestUploadBasedOnRequest(RequestBuffer,RequestDataLength, ResponseBuffer, ResponseDataLength);
  }
  else if (ResponseBuffer[0] == SID_TRANSFER_DATA_RSP)
  {
    RetVal = Dccm_ValidateTransferDataBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }
  else
  {
    /* ResponseBuffer[0] == SID_FILE_TRANSFER_RSP */
    RetVal = Dccm_ValidateRequestFileTransferBasedOnRequest(RequestBuffer, RequestDataLength, ResponseBuffer, ResponseDataLength);
  }

  return RetVal;
}
/** \brief This function is used to validate using an pattern.
 **
 ** The request shall have a fixed value. The response shall have at least a specific value.
 **
 **/
STATIC FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateLengthRequestFixedResponseMinumum
(
  uint32 RequestDataLength,
  uint32 LengthRequest,
  uint32 ResponseDataLength,
  uint32 LengthResponse
)
{
  Std_ReturnType RetVal = E_OK;

  if (RequestDataLength != LengthRequest)
  {
    RetVal = E_NOT_OK;
  }
  else if (ResponseDataLength < LengthResponse)
  {
    RetVal = DCCM_E_INVALID_RESPONSE_LENGTH;
  }
  else
  {
    /* Nothing to do here */
  }
  return RetVal;
}
#define DCCM_STOP_SEC_CODE
#include <MemMap.h>
