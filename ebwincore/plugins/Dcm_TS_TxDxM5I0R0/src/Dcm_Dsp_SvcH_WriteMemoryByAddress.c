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

/* !LINKSTO Dcm.Dsn.File.WMBA.Impl,1 */
/* This file contains the implementation of the Diagnostic Service WriteMemoryByAddress */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 15.2 (required)
 *     "An unconditional break statement shall terminate every non-empty switch clause."
 *
 *     Reason:
 *     The following actions have to be executed in the same state so the break
 *     statement is removed in order to have shorter code and avoid duplicated code.
 *
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 16.3 (required)
  *     "An unconditional break statement shall terminate every switch-clause."
  *
  *     Reason:
  *     The following actions have to be executed in the same state so the break
  *     statement is removed in order to have shorter code and avoid duplicated code.
  *
  *  MISRAC2012-2) Deviated Rule: 8.9 (advisory)
  *     "An object should be defined at block scope if its identifier only
  *     appears in a single function."
  *
  *     Reason:
  *     The object is defined in this way because the values that are stored
  *     in them are required for the next call of this function and should be
  *     hold in the data segment memory.
  *
  *  MISRAC2012-3) Deviated Rule: 16.1 (required)
  *     "All switch statements shall be well-formed"
  *
  *     Reason:
  *     This violation is present because rule 16.3 is violated and justified inside the affected
  *     switch statement, as noted under rule 16.1:
  *     some of the restrictions imposed on switch statements by this rule are expounded in the
  *     15.3, 16.2, 16.3, 16.4, 16.5, 16.6 rules, which are violated inside the current switch statement.
  */

/*===============================[includes]======================================================*/

#include <Dcm_Dsp_SvcH_WriteMemoryByAddress.h>

/* The functions provided and implemented by this unit are exclusively used for
 * UDS service 0x3D handling. */
#if (DCM_DSP_USE_SERVICE_0X3D == STD_ON)

#include <Dcm_Dsp_MemoryServices.h>
#include <Dcm_Trace.h>

/*==================[macro definitions]==========================================================*/

/*==================[type definitions]===========================================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants declarations]============================================*/

/*==================[internal constants declarations]============================================*/

/*==================[external data declarations]=================================================*/

/*==================[internal data declarations]=================================================*/

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/** \brief Global variable holding the length (number of bytes) of the memoryAddress parameter */
/* Deviation MISRAC2012-2 */
STATIC VAR(uint8, DCM_VAR) Dcm_WriteMemAddLength = 0U;

/** \brief Global variable holding the length (number of bytes) of the memorySize parameter */
/* Deviation MISRAC2012-2 */
STATIC VAR(uint8, DCM_VAR) Dcm_WriteMemSizeLength = 0U;

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Global variable holding the context information for executing a writing of a memory
 **        address element.
 */
/* Deviation MISRAC2012-2 */
STATIC VAR(Dcm_WriteMemoryContextType, DCM_VAR) Dcm_WriteMemoryAddressContext;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* This is the generic service handler for UDS service 0x3D */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_WriteMemoryByAddress_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType retVal = DCM_E_NOT_OK;
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType nrc;
  boolean enableResponseCheck = TRUE;

#if (DCM_NUM_PROTOCOL > 1U)
   const uint8 InstIdx = Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[
      pMsgContext->dcmRxPduId].MainConnectionIndex].ProtocolIndex;
#endif
  P2VAR(Dcm_WriteMemoryContextType, AUTOMATIC, DCM_VAR) pWriteMemoryContext =
    &Dcm_WriteMemoryAddressContext;

  DBG_DCM_DSP_WRITEMEMORYBYADDRESS_SVCH_ENTRY(OpStatus,pMsgContext);
  /* Deviation MISRAC2012-3 */
  switch (OpStatus)
  {
    /* First call to the service function */
    case DCM_INITIAL:
    {
      /* Variable holding the memory address to be written to */
      uint32 writeMemAddress = 0U;
      /* Variable holding the size of the memory (number of bytes) to be written */
      uint32 writeMemSize = 0U;

      /* Check if addressAndLengthFormatIdentifier + min memoryAddress + min memorySize +
       * min dataRecord parameters are present in the request */
      if ((pMsgContext->reqDataLen) >= 4U)
      {
        /* The data received should be addressAndLengthFormatIdentifier ->(1byte),
           memoryAddress[], memorySize[] ->
           (Dcm_WriteMemAddLength + Dcm_WriteMemSizeLength bytes) dataRecord[] */
        /* Holds the length (number of bytes) of the memoryAddress parameter */
/* !LINKSTO Dcm.EB.0x3D.AddressAndLengthFormatIdentifier.NotPresent,1 */
#if (DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION == STD_ON)
        /* Verify that the AddressLengthFormatIdentifier is valid or not */
        retVal = Dcm_Dsp_MemoryServices_AddressLengthAndFormat_Verification(pMsgContext->reqData[0U]);
        if(retVal != DCM_E_OK)
        {
          /* Send Nrc 0x31 */
          /* !LINKSTO Dcm.EB.0x3D.AddressAndLengthFormatIdentifier.Present,1 */
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
          enableResponseCheck = FALSE;
          serviceProcessStatus = DCM_E_DONE;
        }
        else
#endif /* (DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION == STD_ON) */
        {
          Dcm_WriteMemAddLength = (pMsgContext->reqData[0U]) & DCM_MASK_NIBBLE;
          /* Holds the length (number of bytes) of the memorySize parameter */
          Dcm_WriteMemSizeLength = (uint8)((pMsgContext->reqData[0U]) >> DCM_NIBBLE_BITS);

          /* ISO Sequence Check: Minimum length shall be equal to: addressAndLengthFormatIdentifier (1 byte)+
             number of memoryAddress bytes (Dcm_WriteMemAddLength) + number of memorySize bytes (Dcm_WriteMemSizeLength).*/
          /* !LINKSTO Dcm.EB.WriteMemoryByAddress.AddressAndLengthFormatIdentifier.MinimalLengthCheck,1 */
          if ((pMsgContext->reqDataLen) >= ((Dcm_MsgLenType)Dcm_WriteMemAddLength +
                                            (Dcm_MsgLenType)Dcm_WriteMemSizeLength +
                                            (Dcm_MsgLenType)DCM_ADDRESSANDLENGTHFORMATID_SIZE))
          {
            /* Get the Memory address and the size of memory to be write */
            retVal =
              Dcm_Dsp_MemoryServices_GetMemoryAddressAndSize(
                &(pMsgContext->reqData[0U]),
                Dcm_WriteMemAddLength,
                Dcm_WriteMemSizeLength,
                &writeMemAddress,
                &writeMemSize);
            if(retVal == DCM_E_OK)
            {
              /* ISO Sequence Check: Total length shall be equal to: addressAndLengthFormatIdentifier (1 byte ) +
                 number of memoryAddress bytes (Dcm_WriteMemAddLength) + number of memorySize
                 bytes (Dcm_WriteMemSizeLength) + number of dataRecord bytes (writeMemSize).*/
              /* !LINKSTO Dcm.EB.WriteMemoryByAddress.AddressAndLengthFormatIdentifier.TotalLengthCheck,1 */
              if ((pMsgContext->reqDataLen) !=
                  (writeMemSize + Dcm_WriteMemAddLength +
                    Dcm_WriteMemSizeLength + DCM_ADDRESSANDLENGTHFORMATID_SIZE))
              {
                Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
                enableResponseCheck = FALSE;
                serviceProcessStatus = DCM_E_DONE;
              }
              else
              {
                /* calculate required response data length */
                const uint32 RequiredBufSize = (uint32)(pMsgContext->resDataLen)
                                                 + Dcm_WriteMemAddLength
                                                 + writeMemSize
                                                 + 1U;

                /* Check if adequate buffer is configured to write the data */
                if(RequiredBufSize <= (uint32)(DCM_HSM_INFO(InstIdx).TxBuffer.Size))
                {
                  Dcm_Dsp_MemoryServices_WriteMemory_Setup(
                    pWriteMemoryContext,
                    writeMemAddress,
                    writeMemSize,
                    &(pMsgContext->reqData[Dcm_WriteMemAddLength + Dcm_WriteMemSizeLength + 1]));

                  serviceProcessStatus = Dcm_Dsp_MemoryServices_WriteMemory_Execute(
                                           pWriteMemoryContext);
                }
                else
                {
  #if (DCM_DEV_ERROR_DETECT == STD_ON)
                  DCM_DET_REPORT_ERROR(DCM_SVCID_WRITEMEMORY, DCM_E_INTERFACE_BUFFER_OVERFLOW);
  #endif
                  Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
                  enableResponseCheck = FALSE;
                  serviceProcessStatus = DCM_E_DONE;
                }
              }
            }
            else
            {
              Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
              enableResponseCheck = FALSE;
              serviceProcessStatus = DCM_E_DONE;
            }
          }
          else
          {
            Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
            serviceProcessStatus = DCM_E_DONE;
          }
        }
      }
      else
      {
        /* !LINKSTO Dcm.EB.WriteMemoryByAddress.NRC0x13,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
        enableResponseCheck = FALSE;
        serviceProcessStatus = DCM_E_DONE;
      }
    }
      break;

    /* !LINKSTO Dcm.EB.WriteMemoryByAddress.WriteMemory.CallRCRRP,1 */
    /* Subsequent call to the service function after FORCE_RCRRP, DCM_PENDING or DCM_CANCEL */
    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-1 */
    case DCM_FORCE_RCRRP_OK:
    {
       Dcm_Dsp_MemoryServices_WriteMemory_ConfirmForcedRCRRP(pWriteMemoryContext);
      /* Drop to the next case in order to continue processing */
    }
    /* no break */

    case DCM_PENDING:
      serviceProcessStatus = Dcm_Dsp_MemoryServices_WriteMemory_Execute(pWriteMemoryContext);
      break;

    case DCM_CANCEL:
      (void)Dcm_Dsp_MemoryServices_WriteMemory_Cancel(pWriteMemoryContext);
      enableResponseCheck = FALSE;
      break;

    default:
      serviceProcessStatus = DCM_E_DONE;
      enableResponseCheck = FALSE;
      break;
  }

  if(enableResponseCheck == TRUE)
  {
    if ((DCM_E_PENDING != serviceProcessStatus) &&
        (DCM_E_RESOURCE_LOCKED != serviceProcessStatus) &&
        (DCM_E_FORCE_RCRRP != serviceProcessStatus))
    {
      /* operation has been completed; either successfully or unsuccessfully */
      serviceProcessStatus = Dcm_Dsp_MemoryServices_WriteMemory_GetResults(
                                                  pWriteMemoryContext->NrcConditions, &nrc);

      if (DCM_E_OK == serviceProcessStatus)
      {
        uint8_least resDataIdx = 0U;
        pMsgContext->resDataLen += (Dcm_MsgLenType)Dcm_WriteMemAddLength +
                                   (Dcm_MsgLenType)Dcm_WriteMemSizeLength +
                                   (Dcm_MsgLenType)DCM_ADDRESSANDLENGTHFORMATID_SIZE;

        for(resDataIdx = 0U;
            resDataIdx <= (uint8)(Dcm_WriteMemAddLength + Dcm_WriteMemSizeLength);
            resDataIdx++)
        {
            pMsgContext->resData[resDataIdx] = pMsgContext->reqData[resDataIdx];
        }
      }
      else
      {
        Dcm_ExternalSetNegResponse(pMsgContext, nrc);
      }

      serviceProcessStatus = DCM_E_DONE;
    } /* else: do nothing; wait for next cycle or nrc transmission */
  }

  /* If resources are not available to perform the write try again next cycle */
  if (serviceProcessStatus == DCM_E_RESOURCE_LOCKED)
  {
    serviceProcessStatus = DCM_E_PENDING;
  }

  DBG_DCM_DSP_WRITEMEMORYBYADDRESS_SVCH_EXIT(serviceProcessStatus,OpStatus,pMsgContext);
  return serviceProcessStatus;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/




#endif /* #if(DCM_DSP_USE_SERVICE_0X3D == STD_ON) */

/*==================[end of file]================================================================*/
