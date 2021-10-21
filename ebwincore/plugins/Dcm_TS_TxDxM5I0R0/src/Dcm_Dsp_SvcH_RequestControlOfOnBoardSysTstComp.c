/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*This file contains the implementation of the Diagnostic Service
  * Processing module in Dcm; all parts that relate to OBD Mode 8 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * This warnings is a false positive.
 * The compiler optimizes the 'for' loop (which has only 1 iteration) in the following way
 * - it prepares in advance the index for the next iteration
 * - if the index is equal to '0' it goes back to check the 'for' loop conditions, otherwise it just moves on
 * - since the index is already equal to '1', it never jumps back to check the exit condition
 *
 */
/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */

#include <Dcm_Dsp_SvcH_RequestControlOfOnBoardSysTstComp.h>

/* The functions provided and implemented by this unit are exclusively used for
 * OBD service 0x08 handling. */
#if(DCM_DSP_USE_SERVICE_0X08 == STD_ON)

#if (DCM_DEM_CLIENT_REF_USED == STD_ON)
#include <Dem.h>                                                      /* Declaration of Dem APIs */
#endif /* #if (DCM_DEM_CLIENT_REF_USED == STD_ON) */

#include <TSMem.h>                                               /* EB specific memory functions */

/*===========================[macro definitions]=================================================*/

#if (defined DCM_OBD_MAX_REQ_TID_SERVICE_08)
  #error "DCM_OBD_MAX_REQ_TID_SERVICE_08 is already defined"
#endif
#define DCM_OBD_MAX_REQ_TID_SERVICE_08 6U

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief The function calculates the availability TIDs
 **
 ** \param[in] ReqAvailTid        Requested Availability TID identifier
 ** \param[in] availabilityData   Buffer where to set the Availability TIDs
 **
 ** \retval E_OK                At least one bit is set in availabilityData
 ** \retval E_NOT_OK            No bit is set in availabilityData
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_GetAvailableReqCtrlTIDs(
  uint8 ReqAvailTid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) availabilityData);

/** \brief The function retrieves a non-availability TID for request control OBD service
 **
 ** \param[in]    InBuffer       Buffer containing the requested TID and its parameters
 ** \param[in]    InBufferLen    Length of requested TID plus parameters
 ** \param[out]   OutBuffer      Buffer to which the TID data is stored
 ** \param[out]   OutBufferLen   Maximum capacity of the buffer to which the TID data is stored
 ** \param[out]   ResponseLength Length of the constructed response message
 ** \param[out]   Nrc            Negative response code
 **
 ** \retval       E_OK           Processing done,  valid result in buffer
 **               E_NOT_OK       Processing done, error code in Nrc
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessReqCtrlNonAvailabilityTID(
  P2VAR(uint8, AUTOMATIC, DCM_VAR) InBuffer,
  Dcm_MsgLenType InBufferLen,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) OutBuffer,
  Dcm_MsgLenType OutBufferLen,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) ResponseLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);

/** \brief The function retrieves the availability data for one or more ranges
 **
 ** \param[in]    ReqTidList         Number of requested availability IDs in InBuffer
 ** \param[out]   ReqTidListLength   Length of the constructed response message
 ** \param[out]   OutBuffer          Buffer to which the availability record(s) are stored
 ** \param[out]   OutBufferLen       Maximum capacity of that buffer
 ** \param[out]   ResponseLength     Length of the constructed response message
 ** \param[out]   Nrc                Negative response code
 **
 ** \retval       E_OK               Processing done,  valid result in buffer
 **               E_NOT_OK           Processing done, error code in Nrc
 **
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessReqCtrlAvailabilityTID(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) ReqTidList,
  Dcm_MsgLenType ReqTidListLength,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) OutBuffer,
  Dcm_MsgLenType OutBufferLen,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) ResponseLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);

/** \brief The function gets availability TIDs or non-availability TID
 **        for request control service. It is called through arapters
 **        that handle OBD or UDS request and response message formats.
 **
 ** \param[in]    InBuffer       Buffer containing the requested TID and its parameters
 ** \param[in]    InBufferLen    Length of requested TID plus parameters
 ** \param[out]   OutBuffer      Buffer to which the TID data is stored
 ** \param[out]   OutBufferLen   Maximum capacity of the buffer to which the TID data is stored
 ** \param[out]   ResponseLength Length of the constructed response message
 ** \param[out]   Nrc            Negative response code
 **
 ** \retval DCM_E_DONE      Operation completed
 **
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessReqCtrl(
  P2VAR(uint8, AUTOMATIC, DCM_VAR) InBuffer,
  Dcm_MsgLenType InBufferLen,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) OutBuffer,
  Dcm_MsgLenType OutBufferLen,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) ResponseLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);

/** \brief An adaptor that takes care of the fact that the request
 **        and response messages are of OBD type
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 */
STATIC FUNC(void, DCM_CODE) Dcm_DspInternal_ReqCtrl_OBDShell(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/*==================[internal function definitions]=============================================*/

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_GetAvailableReqCtrlTIDs(
  uint8 ReqAvailTid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) availabilityData)
{
  Std_ReturnType ReturnVal = E_NOT_OK;
  uint8 idx = 0U;

  /* See if there are any configured and used TIDs within the requested availability range */
  /* Deviation TASKING-1 */
  for (idx=0U; idx<DCM_DSP_NUM_REQUESTCONTROL_CFG; idx++)
  {
    /* Only consider TIDs that are active and within the proper range */
    if ((Dcm_DspRequestControlUsedConfig[idx] == TRUE)
        && (Dcm_DspRequestControlConfig[idx].TestId > ReqAvailTid)
        && (Dcm_DspRequestControlConfig[idx].TestId <= (ReqAvailTid + DCM_OBD_AVAILABILITY_RANGE)))
    {
      /* Set the bit in the availability data as this TID is within range, available and used */
      (void)Dcm_OBDSupport_UpdateAvailableInfoWithID(Dcm_DspRequestControlConfig[idx].TestId,
                                                     ReqAvailTid, availabilityData);

      ReturnVal = E_OK;
    }
  }

  return ReturnVal;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessReqCtrlNonAvailabilityTID(
  P2VAR(uint8, AUTOMATIC, DCM_VAR) InBuffer,
  Dcm_MsgLenType InBufferLen,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) OutBuffer,
  Dcm_MsgLenType OutBufferLen,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) ResponseLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc)
{
  Dcm_NegativeResponseCodeType localNrc = DCM_E_POSITIVERESPONSE;
  Std_ReturnType retVal = E_NOT_OK;
  Dcm_MsgLenType localRespLen = 0U;
  uint8 idx = 0U;

  /* Look for the requested TID in the configuration array */
  /* Deviation TASKING-1 */
  for (idx=0U; idx<DCM_DSP_NUM_REQUESTCONTROL_CFG; idx++)
  {
    if (Dcm_DspRequestControlConfig[idx].TestId == InBuffer[0U])
    {
      /* Make sure the requested TID can be processed, the buffers are large enough and
       * the proper number of bytes are present to be passed to the configured function */
      /* !LINKSTO Dcm.EB.Config.Calibration.DcmDspRequestControlUsed,1 */
      if ((Dcm_DspRequestControlUsedConfig[idx] == TRUE)
          /* 1 byte to account for the TID */
          && (Dcm_DspRequestControlConfig[idx].InBufferSize == (InBufferLen - 1U))
          && (Dcm_DspRequestControlConfig[idx].OutBufferSize <= (OutBufferLen - 1U))
          && (Dcm_DspRequestControlConfig[idx].RequestControlFnc != NULL_PTR)
         )
      {
        /* Set the TID to first position in the response message */
        OutBuffer[0U] = InBuffer[0U];

        /* !LINKSTO Dcm419,1 */
        /* Call the SWC function to handle the RequestControl */
        if (Dcm_DspRequestControlConfig[idx].RequestControlFnc(&(OutBuffer[1U]), &(InBuffer[1U]))
            == E_OK)
        {
          /* !LINKSTO Dcm420,1 */
          /* Data to be send is the data generated by the handler plus the TID */
          localRespLen = (Dcm_MsgLenType)Dcm_DspRequestControlConfig[idx].OutBufferSize + 1U;

          /* Got a positive response */
          retVal = E_OK;
        }
        else
        {
          /* Indicate the TID is supported but the current request failed */
          localNrc = DCM_E_CONDITIONSNOTCORRECT;
        }
      }
      else
      {
        localNrc = DCM_E_REQUESTOUTOFRANGE;
      }

      /* Discontinue the loop as the requested TID was found */
      break;
    }
  }

  /* Report RequestOutOfRange if the requested TID was not found */
  if (idx >= DCM_DSP_NUM_REQUESTCONTROL_CFG)
  {
    localNrc = DCM_E_REQUESTOUTOFRANGE;
  }

  /* Update the externally visible results of this function */
  *Nrc = localNrc;
  *ResponseLength = localRespLen;

  return retVal;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessReqCtrlAvailabilityTID(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) ReqTidList,
  Dcm_MsgLenType ReqTidListLength,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) OutBuffer,
  Dcm_MsgLenType OutBufferLen,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) ResponseLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc)
{
  Std_ReturnType retVal = E_NOT_OK;
  Dcm_MsgLenType ResponsePos = 0U;

  /* Verify that there is enough space in response buffer to accomodate all requested
   * availability ID records. A record is 5 octets in size */
  if (OutBufferLen >= (ReqTidListLength * 5U))
  {
    uint8 reqIdx = 0U;

    /* Loop over all requested availability IDs. There can be up to 6 in a request. */
    for(reqIdx = 0U; reqIdx < ReqTidListLength; reqIdx++)
    {
      const uint8 currReqTid = ReqTidList[reqIdx];

      /* No mix of availablity and non-availability TIDs is allowed. */
      /* !LINKSTO SWS_Dcm_00947,1 */
      if (DCM_IS_AVAILABILITY_OBDSUBID(currReqTid) == FALSE)
      {
        /* If a non-availability TID is among availability ones consider the whole request
         * invalid */
        ResponsePos = 0U;
        break;
      }

      /* Clean the buffer before setting any availability bits in it */
      TS_MemSet(&(OutBuffer[ResponsePos]), 0U, 5U);
      /* !LINKSTO SWS_Dcm_00948,1 */

      /* Check and set the availability bits for the requested availability ID */
      if (Dcm_DspInternal_GetAvailableReqCtrlTIDs(currReqTid,
                                                  &(OutBuffer[ResponsePos+1U])) == E_OK)
      {
        /* As there were TIDs in the requested range, the range will be
         * reported. The first byte of an availability record is the
         * availability ID */
        OutBuffer[ResponsePos] = currReqTid;

        /* An availability data record is 5 octets. 1 octet is the availablity
         * ID and the remaing 4 are the the bit encoded TID availability record */
        ResponsePos += 5U;
      }
    }
  }

  /* Length of response message is the length of all used availability records */
  *ResponseLength = ResponsePos;

  /* If there is no availability record transmitted for any requested availability ID
   * this is considered a RequestOutOfRange in UDS terms */
  if (ResponsePos == 0U)
  {
    *Nrc = DCM_E_REQUESTOUTOFRANGE;
  }
  else
  {
    /* Got a positive response */
    retVal = E_OK;
  }

  return retVal;
}
/* !LINKSTO Dcm417,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessReqCtrl(
  P2VAR(uint8, AUTOMATIC, DCM_VAR) InBuffer,
  Dcm_MsgLenType InBufferLen,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) OutBuffer,
  Dcm_MsgLenType OutBufferLen,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) ResponseLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* The request message for service 8 may contain up to 6 availability IDs
   * or 1 TID and max 5 additonal parameters */
  if((InBufferLen >= 1U)
      && (InBufferLen <= DCM_OBD_MAX_REQ_TID_SERVICE_08))
  {
    /* Pointer to the array of TIDs. Can be either the requestMessage
     * or, in case a common buffer is used, a separate buffer to which the
     * content of the request message is copied before it is overwritten by
     * the reponse message */
    P2VAR(uint8, AUTOMATIC, DCM_VAR) ReqTidList;

#if (DCM_COMMON_BUFF_CONFIGURED == STD_ON)
    /* Dcm_ReadTidBuffer is used to store the req Tids,
     * if a single buffer is configured as Rx and Tx buffer.
     * Size of buffer is the max number of TIDs that can be
     * requested in service 08 i.e. 6 TIDS meaning 6 bytes */
    uint8 ReqTidBuffer[DCM_OBD_MAX_REQ_TID_SERVICE_08];

    /*If RX and TX buffer is common*/
    if(InBuffer == OutBuffer)
    {
      /*Copy the Tids to the local buffer*/
      TS_MemCpy(ReqTidBuffer, InBuffer, InBufferLen);

      /* Use the backup buffer for the request message */
      ReqTidList = ReqTidBuffer;
    }
    else
#endif
    {
      /* Use the original request message buffer */
      ReqTidList = InBuffer;
    }

    /* Discriminate between availability and non-availability TIDs */
    if (DCM_IS_AVAILABILITY_OBDSUBID(ReqTidList[0U]) == TRUE)
    {
        /* !LINKSTO Dcm418,1 */
        /* Handle one or more (max 6) availability TIDs */
        retVal = Dcm_DspInternal_ProcessReqCtrlAvailabilityTID(ReqTidList,
                                                               InBufferLen,
                                                               OutBuffer,
                                                               OutBufferLen,
                                                               ResponseLength,
                                                               Nrc);
    }
    else
    {
      /* Handle exactly one non-availability TID. */
      retVal = Dcm_DspInternal_ProcessReqCtrlNonAvailabilityTID(ReqTidList,
                                                                InBufferLen,
                                                                OutBuffer,
                                                                OutBufferLen,
                                                                ResponseLength,
                                                                Nrc);
    }
  }
  else
  {
    /* There is an issue with the request message length */
    *Nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
  }

  return retVal;
}

STATIC FUNC(void, DCM_CODE) Dcm_DspInternal_ReqCtrl_OBDShell(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* Generated negative response code, if any */
  Dcm_NegativeResponseCodeType Nrc = DCM_E_POSITIVERESPONSE;

  /* Length of the generated response message, if any */
  Dcm_MsgLenType ResponseLength = 0U;

  /* Suppress positive response by default. It is enabled before a valid
   * positive or negative reponse message is send */
  pMsgContext->msgAddInfo.suppressPosResponse = TRUE;

  /* Call the function to process the request control */
  retVal = Dcm_DspInternal_ProcessReqCtrl(pMsgContext->reqData,
                                          pMsgContext->reqDataLen,
                                          pMsgContext->resData,
                                          pMsgContext->resMaxDataLen,
                                          &ResponseLength,
                                          &Nrc);

  /* Send a positive response? */
  if (retVal == E_OK)
  {
    /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_u,1 */
    pMsgContext->resDataLen = ResponseLength;
    pMsgContext->msgAddInfo.suppressPosResponse = FALSE;
  }
  else
  {
    /* Only the ConditonsNotCorrect NRC will be sent in OBD mode. Other NRCs like
     * RequestOutOfRange will be suppressed for the OBD protocol, but used in UDS */

    /* Send a negative response with NRC 0x22 (Conditions not correct)? */
    /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_v,1 */
    if (Nrc == DCM_E_CONDITIONSNOTCORRECT)
    {
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
      pMsgContext->msgAddInfo.suppressPosResponse = FALSE;
    }
    /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_w,1 */
  }
}

/*==================[external function definitions]=============================================*/

FUNC(void, DCM_CODE) Dcm_DspInternal_ReqCtrlViaRoutineCtrl_UDSShell(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* Generated negative response code, if any */
  Dcm_NegativeResponseCodeType Nrc = DCM_E_POSITIVERESPONSE;

  /* Length of the generated response message, if any */
  Dcm_MsgLenType ResponseLength = 0U;

  DBG_DCM_DSPINTERNAL_REQCTRLVIAROUTINECTRL_UDSSHELL_ENTRY(pMsgContext);

  /* Check upper length boundary. Lower one has already been checked by the
   * calling function. Only  one availability request may be present in an
   * UDS request (1 byte subfunction, 1 byte RID high, 1 byte RID low) */
  if ((DCM_IS_AVAILABILITY_OBDSUBID(pMsgContext->reqData[3U]) == TRUE)
      && (pMsgContext->reqDataLen > 3U))
  {
    /* Bad request messasge length */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
  }
  else
  {
    /* By setting the in and out pointers behind the additonal data in the UDS
     * message (sub-function ID and upper 8 bits of the 16 bit routine identifier),
     * make the UDS message look like an OBD one and therefore allow to use the
     * existing OBD code to handle this message too. */

    /* Call the function to process the request control */
    retVal = Dcm_DspInternal_ProcessReqCtrl(&(pMsgContext->reqData[2U]),
                                            pMsgContext->reqDataLen - 2U,
                                            &(pMsgContext->resData[2U]),
                                            pMsgContext->resMaxDataLen - 2U,
                                            &ResponseLength,
                                            &Nrc);

    /* Send a positive response? */
    if (retVal == E_OK)
    {
      /* Set the subservice ID and the upper 8 bits of the RID. The other bytes were
       * already handled by the ProcessReqCtrl() function. */
      pMsgContext->resData[0U] = pMsgContext->reqData[0U];
      pMsgContext->resData[1U] = pMsgContext->reqData[1U];
      pMsgContext->resDataLen = ResponseLength + 2U;
    }
    else
    {
      /* As Dcm_DspInternal_ProcessReqCtrl() failed, an NRC must be set. */
      /* Send whatever NRC was set. No NRCs are not suppressed in UDS like in OBD */
      Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
    }
  }

  DBG_DCM_DSPINTERNAL_REQCTRLVIAROUTINECTRL_UDSSHELL_EXIT();
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_RequestControlOfOnBoardSysTstComp_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  DBG_DCM_DSPINTERNAL_REQUESTCONTROLOFONBOARDSYSTSTCOMP_SVCH_ENTRY(OpStatus, pMsgContext);

  if (DCM_INITIAL == OpStatus)
  {
    /* Call the generic handler for this service through an adaptor which takes care
     * of the fact that the request and response messages are of OBD type */
    Dcm_DspInternal_ReqCtrl_OBDShell(pMsgContext);
  }

  DBG_DCM_DSPINTERNAL_REQUESTCONTROLOFONBOARDSYSTSTCOMP_SVCH_EXIT(OpStatus, pMsgContext);

  /* this function NEVER fails and it has a return value only as a result of standardized function signatures */
  return DCM_E_DONE;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X08 == STD_ON) */

/*==================[end of file]================================================================*/

