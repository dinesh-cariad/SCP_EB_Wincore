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

/* !LINKSTO Dcm.Dsn.File.RMBA.Impl,1 */
/* This file contains the implementation of the Diagnostic Service ReadMemoryByAddress */

/*===============================[includes]======================================================*/

#include <Dcm_Dsp_SvcH_ReadMemoryByAddress.h>

/* The functions provided and implemented by this unit are exclusively used for
 * UDS service 0x23 handling. */
#if (DCM_DSP_USE_SERVICE_0X23 == STD_ON)

#include <Dcm_Dsp_MemoryServices.h>
#include <Dcm_Trace.h>

/*==================[macro definitions]==========================================================*/

/*==================[type definitions]===========================================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants declarations]============================================*/

/*==================[internal constants declarations]============================================*/

/*==================[external data declarations]=================================================*/

/*==================[internal data declarations]=================================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Context variable holding information while processing reading of memory */
/* Deviation MISRAC2012-2 */
STATIC VAR(Dcm_ReadMemoryContextType, DCM_VAR) Dcm_ReadMemoryAddressContext;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadMemoryByAddress_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType retVal = DCM_E_NOT_OK;
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType nrc;
  uint32 AmountOfBufferOccupied;

  CONSTP2VAR(Dcm_ReadMemoryContextType, AUTOMATIC, DCM_VAR) pReadMemoryContext =
    &Dcm_ReadMemoryAddressContext;

  DBG_DCM_DSP_READMEMORYBYADDRESS_SVCH_ENTRY(OpStatus, pMsgContext);
  /* Deviation MISRAC2012-3 */
  switch (OpStatus)
  {
    /* First call to the service function */
    case DCM_INITIAL:
    {
      /* Variable holding the memory address to be read */
      uint32 readMemAddress = 0U;
      /* Variable holding the size of the memory (number of bytes) to be read */
      uint32 readMemSize = 0U;

      /* Check if addressAndLengthFormatIdentifier + min memoryAddress + min memorySize parameters
       * are present in the request */
      if ((pMsgContext->reqDataLen) >= 3U)
      {
/* !LINKSTO Dcm.EB.0x23.AddressAndLengthFormatIdentifier.NotPresent,1 */
#if (DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION == STD_ON)
        /* Verify that the AddressLengthFormatIdentifier is valid or not */
        retVal = Dcm_Dsp_MemoryServices_AddressLengthAndFormat_Verification(pMsgContext->reqData[0U]);
        if (retVal != DCM_E_OK)
        {
          /* Send Nrc 0x31 */
          /* !LINKSTO Dcm.EB.0x23.AddressAndLengthFormatIdentifier.Present,1 */
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
          serviceProcessStatus = DCM_E_DONE;
        }
        else
#endif /* (DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION == STD_ON) */
        {
          /* Holds the length (number of bytes) of the memoryAddress parameter */
          uint8 memAddLength = (pMsgContext->reqData[0U]) & DCM_MASK_NIBBLE;
          /* Holds the length (number of bytes) of the memorySize parameter */
          uint8 memSizeLength = (uint8)((pMsgContext->reqData[0U]) >> DCM_NIBBLE_BITS);

          /* !LINKSTO Dcm.EB.ReadMemoryByAddress.TotalLengthCheck,1 */
          /* The data received should be addressAndLengthFormatIdentifier ->(1byte),
             memoryAddress[], memorySize[] -> (memAddLength + memSizeLength bytes) */
          if ((pMsgContext->reqDataLen) != ((Dcm_MsgLenType)memAddLength +
                                            (Dcm_MsgLenType)memSizeLength +
                                            (Dcm_MsgLenType)DCM_ADDRESSANDLENGTHFORMATID_SIZE))
          {
            Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
            serviceProcessStatus = DCM_E_DONE;
          }
          else
          {
            /* Get the Memory address and the size of memory to be read */
            retVal = Dcm_Dsp_MemoryServices_GetMemoryAddressAndSize(&(pMsgContext->reqData[0U]),
                                                                    memAddLength,
                                                                    memSizeLength,
                                                                    &readMemAddress,
                                                                    &readMemSize);

            if (retVal == DCM_E_OK)
            {
              Dcm_Dsp_MemoryServices_ReadMemory_Setup(
                pReadMemoryContext,
                readMemAddress,
                readMemSize,
                pMsgContext->resData,
                pMsgContext->resDataLen,
                pMsgContext->resMaxDataLen);

              serviceProcessStatus
                = Dcm_Dsp_MemoryServices_ReadMemory_Execute(pReadMemoryContext);

              if ((DCM_E_PENDING != serviceProcessStatus)
                  && (DCM_E_FORCE_RCRRP != serviceProcessStatus)
                  && (DCM_E_RESOURCE_LOCKED != serviceProcessStatus)
                  )
              {
                /* operation has been completed; either successfully or unsuccessfully */
                serviceProcessStatus =
                  Dcm_Dsp_MemoryServices_ReadMemory_GetResults(pReadMemoryContext,
                                                               &nrc,
                                                               &AmountOfBufferOccupied);

                if (DCM_E_DONE == serviceProcessStatus)
                {
                  pMsgContext->resDataLen = (Dcm_MsgLenType)AmountOfBufferOccupied;
                }
                else
                {
                  Dcm_ExternalSetNegResponse(pMsgContext, nrc);
                }

                serviceProcessStatus = DCM_E_DONE;
              } /* else: do nothing; wait for next cycle or nrc transmission */
            }
            else
            {
              Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
              serviceProcessStatus = DCM_E_DONE;
            }
          }
        }
      }
      else
      {
        /* !LINKSTO Dcm.EB.ReadMemoryByAddress.NRC0x13,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
        serviceProcessStatus = DCM_E_DONE;
      }
    }
    break;

    /* Subsequent call to the service function after FORCE_RCRRP, DCM_PENDING or DCM_CANCEL */
    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-1 */
    /* !LINKSTO Dcm.EB.ReadMemoryByAddress.ReadMemory.CallRCRRP,1 */
    case DCM_FORCE_RCRRP_OK:
    {
      Dcm_Dsp_MemoryServices_ReadMemory_ConfirmForcedRCRRP(pReadMemoryContext);
      /* and drop to the next case in order to continue processing */
    }
    /* no break */

    case DCM_PENDING:
    {
      serviceProcessStatus = Dcm_Dsp_MemoryServices_ReadMemory_Execute(pReadMemoryContext);

      if ((DCM_E_PENDING == serviceProcessStatus)
         || (DCM_E_RESOURCE_LOCKED == serviceProcessStatus)
         )
      {
        /* Do nothing, just continue and call the operation again in the next Dcm cycle */
      }
      else if (DCM_E_FORCE_RCRRP == serviceProcessStatus)
      {
        /* Do nothing, just wait for the RCRRP NRC to be transmitted*/
      }
      else
      {
        /* All other cases signify that the operation has been completed,
         * either successfully or unsuccessfully */
        serviceProcessStatus =
          Dcm_Dsp_MemoryServices_ReadMemory_GetResults(pReadMemoryContext,
                                                       &nrc,
                                                       &AmountOfBufferOccupied);

        if (DCM_E_DONE == serviceProcessStatus)
        {
          pMsgContext->resDataLen = (Dcm_MsgLenType)AmountOfBufferOccupied;
        }
        else
        {
          Dcm_ExternalSetNegResponse(pMsgContext, nrc);
        }

        serviceProcessStatus = DCM_E_DONE;
      }
    }
    break;

    case DCM_CANCEL:
    {
      (void)Dcm_Dsp_MemoryServices_ReadMemory_Cancel(pReadMemoryContext);
    }
    break;

    default:
    {
      serviceProcessStatus = DCM_E_DONE;
    }
    break;
  }

  /* If resources are not available to perform the read try again next cycle */
  if (serviceProcessStatus == DCM_E_RESOURCE_LOCKED)
  {
    serviceProcessStatus = DCM_E_PENDING;
  }

  DBG_DCM_DSP_READMEMORYBYADDRESS_SVCH_EXIT(serviceProcessStatus, OpStatus, pMsgContext);
  return serviceProcessStatus;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#endif /* #if(DCM_DSP_USE_SERVICE_0X23 == STD_ON) */

/*==================[end of file]================================================================*/
