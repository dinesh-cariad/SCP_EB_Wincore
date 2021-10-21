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

/* !LINKSTO Dcm.dsn.Dsp.Pid.IfReadData,1 */
/* This file contains the implementation of the PidServices. */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 14.6 (required)
 *     "For any iteration statement there shall be at most one break statement used
 *     for loop termination"
 *
 *     Reason:
 *     The iteration needs to be terminated when some particular conditions are satisfied.
 *     Adapting such algorithm optimizes the code.
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 15.4 (advisory)
  *     "There should be no more than one break or goto statement used to terminate any iteration
  *     statement."
  *
  *     Reason:
  *     The iteration needs to be terminated when some particular conditions are satisfied.
  *     Adapting such algorithm optimizes the code.
  */
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
 */
/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <TSMem.h>                                               /* EB specific memory functions */
#include <Dcm_Dsp_PidServices.h>
#include <Dcm_Int.h>
#include <Dcm_Dsp_DemFacade.h>
#include <Dcm_Dsl_Supervisor.h>

#if (DCM_DEM_CLIENT_REF_USED == STD_ON)
#include <Dem.h>
#endif /* #if (DCM_DEM_CLIENT_REF_USED == STD_ON) */

#if ((DCM_DSP_USE_DIDSERVICES == STD_ON) && (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON))
#include <Dcm_Dsp_EndiannessInterpretation.h>
#endif

/*===========================[macro definitions]=================================================*/

#if (defined DCM_OBD_PID_AVAILDATALEN)
#error DCM_OBD_PID_AVAILDATALEN already defined
#endif
/** \brief Length of a single supported Pid response */
#define DCM_OBD_PID_AVAILDATALEN 4U /* DATA_A, ... DATA_D */

#if (defined DCM_PID_ID_WIDTH)
#error DCM_PID_ID_WIDTH already defined
#endif
/** \brief Width of the Pid Id in service response */
#define DCM_PID_ID_WIDTH 1U

/*===========================[type definitions]==================================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if (((DCM_DSP_USE_SERVICE_0X01 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X02 == STD_ON)) && \
     (DCM_NUM_PID_CFG > 0U))
/** \brief The function gets availability PID" or "non-availability PID"
 **
 ** \param[in] ServiceId Service identifier (0x01 or 0x02)
 ** \param[in] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE      Operation completed
 ** \retval DCM_E_ABORT     Operation failed
 **
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ObdService_CommonPid_Func(
  Dcm_ServiceIdType ServiceId,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* (((DCM_DSP_USE_SERVICE_0X01 == STD_ON) || \
          (DCM_DSP_USE_SERVICE_0X02 == STD_ON)) && \
          (DCM_NUM_PID_CFG > 0U)) */

#if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_NUM_PID_CFG > 0U) )
/** \brief The function calculates the availability PIDs
 **
 ** \param[in] Dcm_ServiceIdType Service Identifier
 ** \param[in] RequestedAvailabilityPidId Requested Availability PID identifier
 ** \param[in] AvailabilityData Buffer where to set the Availability PIDs
 **
 ** \retval E_OK                At least one bit is set in AvailabilityData
 ** \retval E_NOT_OK            No bit is set in AvailabilityData
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_PidGetAvailabilityPid(
  Dcm_ServiceIdType ServiceId,
  Dcm_DspPidIdentifierType RequestedAvailabilityPidId,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) availabilityData,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);
#endif /* #if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_NUM_PID_CFG > 0U) ) */

#if (DCM_NUM_PID_CFG > 0U)
/** \brief Loops through all PID configurations, fetches the data and adds padding
 **
 ** \param[in] ServiceId Service identifier (0x01 or 0x02)
 ** \param[in] PidId Pid Identifier
 ** \param[in] DestBuffer Buffer where to put the read data
 ** \param[in] Bufsize Destination Buffer size
 **
 ** \retval E_OK                Operation Success
 ** \retval E_NOT_OK            Operation Failed
 **
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_PidGetNonAvailabilityPid(
  Dcm_ServiceIdType ServiceId,
  Dcm_DspPidIdentifierType PidId,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DestBuffer,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) Bufsize,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);
#endif /* #if (DCM_NUM_PID_CFG > 0U) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

#if (DCM_NUM_PID_CFG > 0U)
/* !LINKSTO Dcm.Dsn.OBDPidServices.PidDataBuffer,1 */
/** \brief Buffer which stores each read DcmDspPidData data until it is copied in the
 ** service response  at the correct location.
 */
VAR(uint8, DCM_VAR) Dcm_PidDataBuffer[DCM_PID_DATA_MAX_SIZE];
#endif

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_NUM_PID_CFG > 0U) )
/* !LINKSTO Dcm.dsn.OBDPidServices.IB.PidGetAvailablePids,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_PidGetAvailabilityPid(
  Dcm_ServiceIdType ServiceId,
  Dcm_DspPidIdentifierType RequestedAvailabilityPidId,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) availabilityData,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc)
{
  Std_ReturnType ReturnVal = DCM_E_NOT_OK;

  /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_a1,1 */
  TS_MemSet(availabilityData, 0U, DCM_OBD_PID_AVAILDATALEN);

#if (DCM_NUM_PID_CFG > 0U)
  {
    Std_ReturnType UpdateAvailableInfoWithIDRetVal = DCM_E_NOT_OK;
    uint8_least Index = 0U;

    P2CONST(Dcm_DspPidConfigType, AUTOMATIC, DCM_CONST) curPidConfig = &Dcm_DspPidConfig[0];

    /* !LINKSTO Dcm.dsn.Dsp.PidService.PidGetAvailabilityPid,1 */
    /* !LINKSTO Dcm.dsn.OBDPidServices.IB.DspInternal_DidAvailabiltiyChecks,1 */
    /* Deviation TASKING-1 */
    for (Index=0U; Index<DCM_NUM_PID_CFG; Index++)
    {
      curPidConfig = &Dcm_DspPidConfig[Index];

      if (DCM_UINT8_IS_BIT_SET(Dcm_DspPidUsed[Index/8U], Index%8U) &&
          (((Dcm_DspPidBitFieldType)(curPidConfig->BitField & ServiceId))!=0x00U))
      {
        /* !LINKSTO Dcm.ISO-15031-5_8.1.2.2_1,1 */
        /* !LINKSTO SAE_J1979-DA_TABLE_A1.01,1 */
        /* !LINKSTO SAE_J1979-DA_TABLE_A1.02,1 */
        UpdateAvailableInfoWithIDRetVal =
          Dcm_OBDSupport_UpdateAvailableInfoWithID(curPidConfig->PidIdentifier,
            RequestedAvailabilityPidId, availabilityData);

        if (DCM_E_OK == UpdateAvailableInfoWithIDRetVal)
        {
          ReturnVal = DCM_E_OK;
        }
      }
    }

#if (DCM_DSP_USE_SERVICE_0X02 == STD_ON)
    /* Set Pid 02 as supported if OBD service 0x02 with Pid 0x00 is requested */
    /* !LINKSTO Dcm.EB.OBD.RequestPowertrainFreezeFrameData.AvailabilityOfPID2,1 */
    if ((ServiceId == DCM_SERVICE_02) && (RequestedAvailabilityPidId == 0x00U))
    {
      UpdateAvailableInfoWithIDRetVal = Dcm_OBDSupport_UpdateAvailableInfoWithID(
        0x02U, RequestedAvailabilityPidId, availabilityData);
    }
    if (DCM_E_OK == UpdateAvailableInfoWithIDRetVal)
    {
      ReturnVal = DCM_E_OK;
    }
#endif /* #if (DCM_DSP_USE_SERVICE_0X02 == STD_ON) */
  }
#else
  /* This case can be reached only when there is no configured PID related to link
     between UDS and OBD service 0x01. All other cases related to OBD services 0x01
     and 0x02 are filtered out from service handlers */
  DCM_PRECONDITION_ASSERT( ((ServiceId == DCM_SERVICE_01) && \
                            (RequestedAvailabilityPidId == 0x00U) ), DCM_INTERNAL_API_ID);
  ReturnVal = DCM_E_OK;
#endif /* #if (DCM_NUM_PID_CFG > 0U) */

  if (ReturnVal == DCM_E_NOT_OK)
  {
    /* If no PID is configured in the req range
       which is used and supports Service 01 -
       set NRC in case PID is called by Service 0x22 */
    *Nrc = DCM_E_REQUESTOUTOFRANGE;
  }

  return ReturnVal;
}
#endif /* #if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_NUM_PID_CFG > 0U) ) */

#if (DCM_NUM_PID_CFG > 0U)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_PidGetNonAvailabilityPid(
  Dcm_ServiceIdType ServiceId,
  Dcm_DspPidIdentifierType PidId,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DestBuffer,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) Bufsize,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc)
{
  Std_ReturnType ReturnVal = DCM_E_NOT_OK;

  uint8_least Index = 0U;
  P2CONST(Dcm_DspPidConfigType, AUTOMATIC, DCM_CONST) curPidConfig = Dcm_DspPidConfig;
  uint8 ValidPidFound = 1U;

#if (DCM_DSP_USE_SERVICE_0X02 == STD_ON)
  if (!((ServiceId == DCM_SERVICE_02) && (PidId == 0x02U)))
#endif
  {
    /* Search Pid in config */
    /* !LINKSTO Dcm.dsn.OBDPidServices.IB.DspInternal_DidAvailabiltiyChecks,1 */
    /* Deviation TASKING-1 */
    for (Index=0U; Index<DCM_NUM_PID_CFG; Index++)
    {
      curPidConfig = &Dcm_DspPidConfig[Index];
      /*If PID found in configuration*/
      if (PidId == curPidConfig->PidIdentifier)
      {
        const uint8 PidUsedByte = (uint8)(Index/0x08U);
        const uint8 PidUsedBit = (uint8)(Index%0x08U);

        /* If PID not supporting requested service or is not used */
        if (((ServiceId == DCM_SERVICE_01) &&
             (((uint8)(curPidConfig->BitField & DCM_DSP_SERVICE01_MASK)) == 0U)) ||
            ((ServiceId == DCM_SERVICE_02) &&
             (((uint8)(curPidConfig->BitField & DCM_DSP_SERVICE02_MASK)) == 0U )) ||
            (!DCM_UINT8_IS_BIT_SET(Dcm_DspPidUsed[PidUsedByte], PidUsedBit)))
        {
          /* just say 'PID not found' */
          ValidPidFound = 0U;
        }
        break;
      }
    }
    if (((Index == DCM_NUM_PID_CFG) || (ValidPidFound == 0U))
         && (ServiceId == DCM_SERVICE_01))
    {
      /* If requested PID is not configured or doesn't support service 01,
         set NRC in case PID is called by Service 0x22 */
      *Nrc = DCM_E_REQUESTOUTOFRANGE;
    }
  }

  /*If PID found in configuration and PID is used*/
  /* !LINKSTO Dcm.dsn.OBDPidServices.IB.DspInternal_PidReadData,1 */
  if ((Index < DCM_NUM_PID_CFG) && (ValidPidFound == 1U))
  {
    uint32_least PidDataConfigIndex;
    uint8 PidSize = curPidConfig->PidSize;
#if (DCM_DSP_PID_SUPINFO_CONFIG_NUM > 0)
    uint16_least SupInfoConfigIndex;
#endif
    uint8 DestBufferOffset = 0U;

#if (DCM_DSP_USE_SERVICE_0X02 == STD_ON)
    if (ServiceId == DCM_SERVICE_02)
    {
      DestBufferOffset = 1U; /* FF */
      /* !LINKSTO Dcm.EB.OBD.RequestPowertrainFreezeFrameData.PID2,1 */
      if (PidId == 0x02U)
      {
        PidSize = 2U;
      }
    }
#endif
    /* !LINKSTO Dcm.EB.OBD.TxBufferSizeToLow.Suppress,1 */
    if (PidSize <= (*Bufsize - DestBufferOffset))
    {

#if (DCM_DSP_USE_SERVICE_0X02 == STD_ON)
      if (ServiceId == DCM_SERVICE_02)
      {
        DestBuffer[0U] = 0U; /* freeze frame*/
      }
#endif
      /* Init DestBuffer with 0 */
      /* !LINKSTO Dcm623,2, SWS_Dcm_00944,1, SWS_Dcm_00973,1, SWS_Dcm_00974,1, SWS_Dcm_01061,1 */
      /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_e1,1, Dcm.ISO-15031-5_6.2.4.3.7_Table11_f1,1 */
      TS_MemSet(&DestBuffer[DestBufferOffset], 0x00U, PidSize);

#if (DCM_DSP_USE_SERVICE_0X02 == STD_ON)
      /* !LINKSTO Dcm.EB.OBD.RequestPowertrainFreezeFrameData.PID2,1 */
      if ((ServiceId == DCM_SERVICE_02) && (PidId == 0x02U))
      {
        uint32 DTCNumber;

        /* !LINKSTO Dcm279,2, Dcm.ISO-15031-5_6.2.4.3.7_Table11_e1,1 */
        /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_f1,1 */
        if(Dcm_Dsp_DemFacade_GetDTCOfOBDFreezeFrame(&DTCNumber) == DCM_E_OK)
        {
          /* 2-byte DTC is packed into the 4-byte data insert in DestBuffer
           * (see [SWS_Dem_00277])*/
          DestBuffer[1U] = (uint8)(((uint32)DTCNumber)>>16U); /* High byte */
          DestBuffer[2U] = (uint8)(((uint32)DTCNumber)>>8U); /* low byte */
        }

        /* !LINKSTO Dcm279,2, SWS_Dcm_01061,1, Dcm.ISO-15031-5_6.2.4.3.7_Table11_e1,1 */
        /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_f1,1 */
        ReturnVal = DCM_E_OK;
      }
      else
#endif
      {
        /*Read all Pid data from Application (service 0x01) or Dem (Service 0x02)*/
        for (PidDataConfigIndex=curPidConfig->DataStartIndex;
             PidDataConfigIndex<(curPidConfig->DataStartIndex+curPidConfig->NumOfData);
             PidDataConfigIndex++)
        {
          /*curPidDataConfig goes through all PID Data of a PID
           * in PidDataConfig array -> from DataStartIndex to NumOfData*/
          CONSTP2CONST(Dcm_DspPidDataConfigType, AUTOMATIC, DCM_CONST) curPidDataConfig =
              &Dcm_DspPidDataConfig[PidDataConfigIndex];

          /* !LINKSTO Dcm.dsn.Dsp.PidService.PidReadData_0x01,1 */
          if (ServiceId == DCM_SERVICE_01)
          {
            /* !LINKSTO Dcm408,2 */
            /* !LINKSTO Dcm.EB.OBD.RequestCurrentPowertrainDiagnosticData.ReadDataNotOk,1 */
            if (DCM_E_OK !=
                curPidDataConfig->Service0x01ReadFnc(Dcm_PidDataBuffer))
            {
              ReturnVal = DCM_E_NO_DATA;
              break;
            }
          }
#if (DCM_DSP_USE_SERVICE_0X02 == STD_ON)
          /* !LINKSTO Dcm.dsn.Dsp.PidService.PidReadData_0x02,1 */
          if (ServiceId == DCM_SERVICE_02)
          {
            /* Local copy of Bufsize used as in/out parameter for
            *  Dcm_Dsp_DemFacade_ReadDataOfOBDFreezeFrame()
             * Dcm_Dsp_DemFacade_ReadDataOfOBDFreezeFrame receives the available num of bytes and
             * returns in the same parameter the actual num of written bytes*/
            uint16 UsedBufsize = (uint16)(curPidDataConfig->DataSize+7U)/8U;
            uint8 PidDataConfigRelIndex =
                (uint8)(PidDataConfigIndex - curPidConfig->DataStartIndex);

            /* !LINKSTO Dcm286,2, Dcm.ISO-15031-5_6.2.4.3.7_Table11_e3,1 */
            if (DCM_E_OK != Dcm_Dsp_DemFacade_ReadDataOfOBDFreezeFrame(PidId, PidDataConfigRelIndex,
                Dcm_PidDataBuffer, &UsedBufsize))
            {
              /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_e3,1 */
              ReturnVal = DCM_E_NO_DATA;
              /* Deviation MISRAC2012-1 */
              /* Deviation MISRA-1 */
              break;
            }
          }
#endif
          /*copy data to DestBuffer*/
          /* !LINKSTO Dcm287,1 */
          Dcm_DspInternal_CopySignalData(Dcm_PidDataBuffer,
              &DestBuffer[DestBufferOffset], 0u, curPidDataConfig->DataPos,
              curPidDataConfig->DataSize,
              DCM_COPY_TO_OUTBUFFER_OPAQUE,
              DCM_BYTE_DIRECTION_INCREMENT);
        }
        /* !LINKSTO Dcm.EB.OBD.RequestCurrentPowertrainDiagnosticData.ReadDataNotOk,1 */
        if (ReturnVal != DCM_E_NO_DATA)
        {
#if (DCM_DSP_PID_SUPINFO_CONFIG_NUM > 0)
          /*curPidSupInfoConfig goes through all Sup Info Config of a PID
           * in SupInfoConfig array -> from SupInfoStartIndex to NumOfSupInfo*/
          /* !LINKSTO Dcm621,2 */
          for (SupInfoConfigIndex=curPidConfig->SupInfoStartIndex;
               (uint16)SupInfoConfigIndex < \
                 (curPidConfig->SupInfoStartIndex+curPidConfig->NumOfSupInfo);
               SupInfoConfigIndex++)
          {
            CONSTP2CONST(Dcm_DspPidSupInfoConfigType, AUTOMATIC, DCM_CONST) curPidSupInfoConfig =
                &Dcm_DspPidSupInfoConfig[SupInfoConfigIndex];
            CONSTP2CONST(uint8, AUTOMATIC, DCM_CONST) curPidSupInfoData =
                &Dcm_DspPidSupInfoData[curPidSupInfoConfig->SupInfoDataStartIndex];

            /* Copy all support info data starting from
             * SupInfoDataStartIndex with the length SupInfoLen */
            TS_MemCpy(&DestBuffer[curPidSupInfoConfig->SupInfoPos + DestBufferOffset],
                      curPidSupInfoData, curPidSupInfoConfig->SupInfoLen);
          }
#endif
          ReturnVal = DCM_E_OK;
        }
        /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_e1,1 */
        else
        {
          ReturnVal = DCM_E_NOT_OK;
        }
      }
      /*The function gives back the num of written bytes*/
      *Bufsize = (Dcm_MsgLenType)DestBufferOffset + (Dcm_MsgLenType)PidSize;
    }
  }
  return ReturnVal;
}
#endif /* (DCM_NUM_PID_CFG > 0U)) */

#if (((DCM_DSP_USE_SERVICE_0X01 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X02 == STD_ON)) && \
     (DCM_NUM_PID_CFG > 0U))
/* !LINKSTO Dcm.dsn.Dsp.OBDPidCommonScvH,1, Dcm.dsn.Dsp.CommonPidScvH,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ObdService_CommonPid_Func(
  Dcm_ServiceIdType ServiceId,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Dcm_MsgLenType RequestLen = pMsgContext->reqDataLen;

  /* Variable needed as parameter to
   * Dcm_DspInternal_PidGetAvailabilityPid and
   * Dcm_DspInternal_PidGetNonAvailabilityPid */
  Dcm_NegativeResponseCodeType Nrc = DCM_E_POSITIVERESPONSE;

  /*Suppress the Pos Response. Pos Response is enabled when a valid request is received.*/
  pMsgContext->msgAddInfo.suppressPosResponse = TRUE;

  /*The request message may contain up to 6 PIDs for Service 01 and 3 PIDs for Service 02*/
  /* !LINKSTO Dcm.ISO-15031-5_8.1.1_1,1, Dcm.ISO-15031-5_8.2.1_1,1 */
  /* !LINKSTO Dcm.ISO-15031-5_8.2.1_1_InvalidRequestLength,1 */
  /* !LINKSTO Dcm.ISO-15031-5_8.2.1_1_MinimumRequestLength,1 */
  if( ((ServiceId == DCM_SERVICE_01)&&(RequestLen > DCM_OBD_MAX_REQ_PID_SERVICE_01))||
      ((ServiceId == DCM_SERVICE_02)&&
      ((RequestLen > (DCM_OBD_MAX_REQ_PID_SERVICE_02 * 2U)) || ((RequestLen % 2U) != 0U)))||
      (RequestLen == 0U))
  {
    /* send no response */
    /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_b,1, Dcm.ISO-15031-5_6.2.4.3.7_Table11_h,1 */
    pMsgContext->msgAddInfo.suppressPosResponse = TRUE;
  }
  else
  {

#if ((DCM_COMMON_BUFF_CONFIGURED == STD_ON) || (DCM_DSP_USE_SERVICE_0X02 == STD_ON))
    /* Dcm_ReadPidBuffer is used to store the req Pids,
     * if a single buffer is configured as Rx and Tx buffer.
     * Size of buffer is the max number of PIDs that can be
     * requested in Services 01 and 02 */
    Dcm_MsgItemType Dcm_ReqPidBuffer[DCM_OBD_MAX_REQ_PID_SERVICE_01];
#endif
    P2VAR(uint8, AUTOMATIC, DCM_VAR) ReqPidList;

#if (DCM_DSP_USE_SERVICE_0X02 == STD_ON)
    if (ServiceId == DCM_SERVICE_02)
    {
      uint8_least Index;
      uint8_least IndexWrite = 0U;
      for (Index=0U;Index<(RequestLen-1U);Index+=2U)
      {
        Dcm_ReqPidBuffer[IndexWrite] = pMsgContext->reqData[Index];
        IndexWrite++;
        /* If a freeze frame different than 0 is requested,
           ignore the response for that PID */
        /* !LINKSTO Dcm409,1 */
        if (pMsgContext->reqData[(Index+1U)]!=0U)
        {
          IndexWrite--;
        }
      }
      ReqPidList = Dcm_ReqPidBuffer;
      RequestLen = IndexWrite;
    }
    else
#endif
    {
#if (DCM_COMMON_BUFF_CONFIGURED == STD_ON)
      /*If RX and TX buffer is common*/
      if(pMsgContext->reqData == pMsgContext->resData)
      {
        /*Copy the Pids to the local buffer*/
        TS_MemCpy(Dcm_ReqPidBuffer, pMsgContext->reqData, pMsgContext->reqDataLen);
        ReqPidList = Dcm_ReqPidBuffer;
      }
      else
#endif
      {
        ReqPidList = pMsgContext->reqData;
      }
    }

    if (DCM_E_OK == Dcm_OBDSupport_CheckRequestCombinedIds(ReqPidList, RequestLen))
    {
      uint8_least Index = 0U;
      /* response position */
      Dcm_MsgLenType ResponsePos = 0U;
      /* Remaining buffer size */
      Dcm_MsgLenType RemainingBufSize = pMsgContext->resMaxDataLen;

      /* !LINKSTO Dcm.ISO-15031-5_8.1.1_2,1, Dcm.ISO-15031-5_8.2_2,1 */
      for(Index=0U; Index<RequestLen; Index++)
      {
        /* Check only the first PID in the request.
         * It is checked above that all PIDs have the same type.*/
        /* !LINKSTO Dcm407,1, Dcm284,2 */
        if(DCM_IS_AVAILABILITY_OBDSUBID(ReqPidList[0U]) == TRUE)
        {
          uint8 ResponsePosOffset;
          if (ServiceId == DCM_SERVICE_01)
          {
            ResponsePosOffset = 1U;
          }
          else
          {
            ResponsePosOffset = 2U;
          }
          /*Availability buffer has 4 bytes*/
          /* !LINKSTO Dcm.EB.OBD.TxBufferSizeToLow.Suppress,1 */
          if ((ResponsePos + 4U + ResponsePosOffset) <= pMsgContext->resMaxDataLen)
          {
            const uint8 currReqPid = ReqPidList[Index];

            /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_e2,1, Dcm.ISO-15031-5_8.1.2.2_1,1 */
            if ((DCM_E_OK == Dcm_DspInternal_PidGetAvailabilityPid(ServiceId,
                currReqPid, &(pMsgContext->resData[ResponsePos+ResponsePosOffset]), &Nrc)) ||
                /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_a1,1 */
                (currReqPid == 0x00U))
            {
              /*add the Pid in response*/
              pMsgContext->resData[ResponsePos] = currReqPid;
              /*add the FF 0 in response*/
              if (ServiceId == DCM_SERVICE_02)
              {
                pMsgContext->resData[ResponsePos + 1U] = 0U;
              }
              /* !LINKSTO SWS_Dcm_00973,1, Dcm.ISO-15031-5_6.2.4.3.7_Table11_b,1 */
              /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_f2,1, Dcm.ISO-15031-5_6.2.4.3.7_Table11_h,1 */
              ResponsePos += 4U + (Dcm_MsgLenType)ResponsePosOffset;
              /* !LINKSTO Dcm.ISO-15031-5_8.1.2.4_1,1 */
              pMsgContext->msgAddInfo.suppressPosResponse = FALSE;
            }
          }
        }
        else
        {
          /* write Pid data to response buffer */
          if(DCM_E_OK == Dcm_PidGetNonAvailabilityPid(ServiceId, ReqPidList[Index],
            &(pMsgContext->resData[ResponsePos + DCM_PID_ID_WIDTH]), &RemainingBufSize, &Nrc))
          {
            /* if successful */
            /* add Pid id in front */
            pMsgContext->resData[ResponsePos] = ReqPidList[Index];

            /* !LINKSTO SWS_Dcm_00973,1, Dcm.ISO-15031-5_6.2.4.3.7_Table11_b,1 */
            /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_f2,1, Dcm.ISO-15031-5_6.2.4.3.7_Table11_h,1 */
            ResponsePos += RemainingBufSize + DCM_PID_ID_WIDTH;
            /* !LINKSTO Dcm.ISO-15031-5_8.1.2.4_1,1 */
            pMsgContext->msgAddInfo.suppressPosResponse = FALSE;
          }
        }

        RemainingBufSize = pMsgContext->resMaxDataLen - ResponsePos;
      }
      pMsgContext->resDataLen =  ResponsePos;
    }
    else
    {
      /*It's not allowed to request a combination of PID types*/
      /* !LINKSTO SWS_Dcm_00943,1, SWS_Dcm_00972,1 */
      pMsgContext->msgAddInfo.suppressPosResponse = TRUE;
    }
  }

  /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_c,1, Dcm.ISO-15031-5_6.2.4.3.7_Table11_g,1 */
  return DCM_E_DONE;
}
#endif /* (DCM_NUM_PID_CFG > 0U)) */

#if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_NUM_PID_CFG > 0U) )
/* !LINKSTO Dcm.dsn.OBDPidServices.IB.ReadDataOfSingleDid_Execute,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_PidServices_Execute(
  Dcm_GenericUint8IdType PidId,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfg)
{
  Std_ReturnType retVal = DCM_E_NOT_OK;
  Dcm_MsgLenType Size = BufferCfg->Size - BufferCfg->Occupied;

  DBG_DCM_DSP_PIDSERVICES_EXECUTE_ENTRY(PidId, Nrc, BufferCfg);

  if (DCM_IS_AVAILABILITY_OBDSUBID(PidId) == TRUE)
  {

    if(Size < DCM_OBD_PID_AVAILDATALEN)
    {
      /* Buffer is too small for the response data; Set the Negative Response Code. */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.NRC0x14,1 */
      *Nrc = DCM_E_RESPONSETOOLONG;
      retVal = E_NOT_OK;
    }
    else
    {
      /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.PID,1 */
      retVal = Dcm_DspInternal_PidGetAvailabilityPid(
                 DCM_SERVICE_01, PidId, &(BufferCfg->BufferPtr[BufferCfg->Occupied]), Nrc);
      if (retVal == E_OK)
      {
        Size = DCM_OBD_PID_AVAILDATALEN;
      }
    }

  }
  else
  {
#if (DCM_NUM_PID_CFG > 0U)
    retVal = Dcm_PidGetNonAvailabilityPid(
               DCM_SERVICE_01, PidId,
               &(BufferCfg->BufferPtr[BufferCfg->Occupied]), &Size, Nrc);
#endif /* #if (DCM_NUM_PID_CFG > 0U) */
  }
  if (retVal == E_OK)
  {
    BufferCfg->Occupied += Size;
  }

  DBG_DCM_DSP_PIDSERVICES_EXECUTE_EXIT(retVal, PidId, Nrc, BufferCfg);

  return retVal;
}

#endif /* #if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_NUM_PID_CFG > 0U) ) */

#if(DCM_DSP_USE_SERVICE_0X01 == STD_ON)
/* !LINKSTO Dcm243,1, Dcm.dsn.Dsp.OBDServiceScvH.0x01,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestCurrentPowertrainDiagnosticData_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  DBG_DCM_DSP_REQUESTCURRENTPOWERTRAINDIAGNOSTICDATA_SVCH_ENTRY(OpStatus, pMsgContext);

  if ((OpStatus != DCM_CONFIRMED_OK) && (OpStatus != DCM_CONFIRMED_NOK))
  {
#if (DCM_NUM_PID_CFG == 0U)
    /*Suppress the Pos Response. Pos Response is enabled when a valid request is received.*/
    pMsgContext->msgAddInfo.suppressPosResponse = TRUE;
    /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_a1,1 */
    if ((pMsgContext->reqDataLen==1U) && (pMsgContext->reqData[0U]==0U) &&
        (pMsgContext->resMaxDataLen>=5U /* PID + DATA_A..DATA_D */))
    {
      pMsgContext->msgAddInfo.suppressPosResponse = FALSE;
      TS_MemSet(pMsgContext->resData, 0x00U, 5U);
      pMsgContext->resDataLen = 5U;
    }
#else
    (void)Dcm_DspInternal_ObdService_CommonPid_Func(DCM_SERVICE_01, pMsgContext);
#endif
  }

  DBG_DCM_DSP_REQUESTCURRENTPOWERTRAINDIAGNOSTICDATA_SVCH_EXIT(serviceProcessStatus,
                                                               OpStatus, pMsgContext);

  return DCM_E_DONE;
}
#endif /*(DCM_DSP_USE_SERVICE_0X01 == STD_ON) */

#if(DCM_DSP_USE_SERVICE_0X02 == STD_ON)
/* !LINKSTO Dcm244,1, Dcm.dsn.Dsp.OBDServiceScvH.0x02,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestPowertrainFreezeFrameData_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);
#endif

  DBG_DCM_DSP_REQUESTPOWERTRAINFREEZEFRAMEDATA_SVCH_ENTRY(OpStatus, pMsgContext);

  if ((OpStatus != DCM_CONFIRMED_OK) && (OpStatus != DCM_CONFIRMED_NOK))
  {

/* !LINKSTO Dcm.EB.RequestPowerTrainFreezeFrameData.DemFacade.Locking,1 */
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
        /* Lock DemFacade resources
         * A potential request for service 0x19 0x0E that might be concurrent with
         * this hereby request for service 0x02 is cancelled as soon as it is received so
         * the DemFacade lock is always found free
         * Moreover, since the operations for fetching PID data are synchronous, they cannot be
         * interrupted by the internal ROE-generated 0x19 0x0E request.
         */
        (void)Dcm_Dsp_DemFacade_LockClient(DemClientId) ;
#endif

#if (DCM_NUM_PID_CFG == 0U)
    /*Suppress the Pos Response. Pos Response is enabled when a valid request is received.*/
    pMsgContext->msgAddInfo.suppressPosResponse = TRUE;
    if (pMsgContext->reqDataLen==2U)
    {
      /* !LINKSTO Dcm.EB.OBD.RequestPowertrainFreezeFrameData.AvailabilityOfPID2,1 */
      if ((pMsgContext->reqData[0U]==0U) && (pMsgContext->reqData[1U]==0U) &&
          (pMsgContext->resMaxDataLen>=6U /* PID + FRNO_ + DATA_A..DATA_D */))
      {
        /* Pid 0x00 */
        pMsgContext->msgAddInfo.suppressPosResponse = FALSE;
        TS_MemSet(pMsgContext->resData, 0x00U, 6U);
        pMsgContext->resData[2U] = 0x40U; /* PID 2 */
        pMsgContext->resDataLen = 6U;
      }
      /* !LINKSTO Dcm.EB.OBD.RequestPowertrainFreezeFrameData.PID2,1 */
      if ((pMsgContext->reqData[0U]==2U) && (pMsgContext->reqData[1U]==0U) &&
          (pMsgContext->resMaxDataLen>=3U /* PID + FRNO_ + DATA_A..DATA_B */))
      {
        /* Pid 0x02 */
        uint32 DTCNumber;

        pMsgContext->msgAddInfo.suppressPosResponse = FALSE;
        pMsgContext->resData[0U] = 2U; /* PID 2 */
        pMsgContext->resData[1U] = 0U; /* FRNO_ */

        /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_e1,1, Dcm.ISO-15031-5_6.2.4.3.7_Table11_f1,1 */
        if(Dcm_Dsp_DemFacade_GetDTCOfOBDFreezeFrame(&DTCNumber) == DCM_E_OK)
        {
          /* 2-byte DTC is packed into the 4-byte data insert in DestBuffer
           * (see [SWS_Dem_00277])*/
          pMsgContext->resData[2U] = (uint8)(((uint32)DTCNumber)>>16U); /* High byte */
          pMsgContext->resData[3U] = (uint8)(((uint32)DTCNumber)>>8U); /* low byte */
        }
        /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_f1,1 */
        else
        {
          pMsgContext->resData[2U] = 0x00U; /* High byte */
          pMsgContext->resData[3U] = 0x00U; /* low byte */
        }

        pMsgContext->resDataLen = 4U;
      }
    }
#else
    (void)Dcm_DspInternal_ObdService_CommonPid_Func(DCM_SERVICE_02, pMsgContext);
#endif
  }

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
        /* Unlock DemFacade
         * This service handler function is synchronous, i.e. provides its result in one single run,
         * therefore by this time operations with the facade are always completed and the lock
         * may be released */
        Dcm_Dsp_DemFacade_UnlockClient(DemClientId);
#endif

  DBG_DCM_DSP_REQUESTPOWERTRAINFREEZEFRAMEDATA_SVCH_EXIT(serviceProcessStatus,
                                                         OpStatus, pMsgContext);


  return DCM_E_DONE;
}
#endif /*(DCM_DSP_USE_SERVICE_0X02 == STD_ON)*/

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[end of file]================================================================*/
