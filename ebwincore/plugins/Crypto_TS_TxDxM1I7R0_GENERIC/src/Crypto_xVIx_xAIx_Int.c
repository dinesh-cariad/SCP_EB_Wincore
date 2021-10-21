/**
 * \file
 *
 * \brief AUTOSAR Crypto
 *
 * This file contains the implementation of the AUTOSAR
 * module Crypto.
 *
 * \version 1.7.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO EB_Crypto_00000, 1
*/

/*==================[misra deviations]============================================================*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be
 * enclosed in parentheses.
 *
 * Reason:
 * The AUTOSAR compiler abstraction requires these definitions in this way
 * and the arguments cannot be enclosed in parentheses due to C syntax.
 *
 */

/*==================[inclusions]==================================================================*/

#include <Crypto_xVIx_xAIx_AL_KeyManagement.h>
#include <Crypto_xVIx_xAIx_AL.h>
#include <Crypto_xVIx_xAIx_Int.h>

#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif

/*==================[macros]======================================================================*/

/*==================[declaration of types]========================================================*/

/*==================[declaration of internal functions]===========================================*/

#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
/** \brief Function to check if a job is currently enqueued
 *
 *  This function checks if a job is currently enqueued in the Crypto queue.
 *
 *  \param[in]  JobId  The id of the job which shall be checked.
 *  \param[in]  Queue  A pointer to the Crypto queue which shall be checked.
 *
 *  \returns If the job is enqueued in the Crypto queue
 *  \retval TRUE   The job is enqueued in the Crypto queue.
 *  \retval FALSE  The job is not enqueued in the Crypto queue.
 *
 *  \Reentrancy{Nonreentrant}
 *  \Synchronicity{Synchronous}
 */
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_IsEnqueued
(
  uint32 JobId,
  CONSTP2VAR(Crypto_xVIx_xAIx_QueueType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Queue
);
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */

/*==================[definition of internal constants]============================================*/

/*==================[definition of internal variables]============================================*/

/*==================[definition of external constants]============================================*/

/*==================[definition of external variables]============================================*/

/*==================[definition of external constants]============================================*/

/*==================[definition of external functions]============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>


#if (STD_ON == CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT)

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_PL_Init]------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_00022, 1
*/
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_Init
(
  void
)
{
  /* Forward the initialization request to the abstraction layer */
  /* !LINKSTO EB_Crypto_00023, 1
  */
  Crypto_xVIx_xAIx_AL_Init();
}

#endif /* #if (STD_ON == CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT) */

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_PL_KeyElementSet]---------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_00043, 1, EB_Crypto_01019, 1 */
#if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyElementSet
(
  uint32 CryptoKeyId,
  uint32 KeyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr,
  uint32 KeyLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_AL_KeyElementSet(CryptoKeyId, KeyElementId, KeyPtr, KeyLength);
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_PL_KeyValidSet]-----------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_01081, 1, EB_Crypto_01019, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyValidSet
(
  uint32 CryptoKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_AL_KeyValidSet(CryptoKeyId);
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_PL_KeyElementGet]---------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_00045, 1, EB_Crypto_01019, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyElementGet
(
  uint32 CryptoKeyId,
  uint32 KeyElementId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet(CryptoKeyId, KeyElementId, ResultPtr, ResultLengthPtr);
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_PL_KeyElementCopy]--------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_00046, 1, EB_Crypto_01019, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyElementCopy
(
  uint32 CryptoKeyId,
  uint32 KeyElementId,
  uint32 TargetCryptoKeyId,
  uint32 TargetKeyElementId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_AL_KeyElementCopy(
                                               CryptoKeyId,
                                               KeyElementId,
                                               TargetCryptoKeyId,
                                               TargetKeyElementId
                                             );
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_PL_KeyCopy]---------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_00047, 1, EB_Crypto_01019, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyCopy
(
  uint32 CryptoKeyId,
  uint32 TargetCryptoKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_AL_KeyCopy(CryptoKeyId, TargetCryptoKeyId);
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_PL_RandomSeed]------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_00049, 1, EB_Crypto_01019, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_RandomSeed
(
  uint32 CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) EntropyPtr,
  uint32 EntropyLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_AL_RandomSeed(CryptoKeyId, EntropyPtr, EntropyLength);
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_PL_KeyGenerate]-----------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_00050, 1, EB_Crypto_01019, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyGenerate
(
  uint32 CryptoKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_AL_KeyGenerate(CryptoKeyId);
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_PL_KeyDerive]-------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_00051, 1, EB_Crypto_01019, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyDerive
(
  uint32 CryptoKeyId,
  uint32 TargetCryptoKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_AL_KeyDerive(CryptoKeyId, TargetCryptoKeyId);
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_PL_KeyExchangeCalcPubVal]-------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_00052, 1, EB_Crypto_01019, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyExchangeCalcPubVal
(
  uint32 CryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValueLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_AL_KeyExchangeCalcPubVal(
                                                      CryptoKeyId,
                                                      PublicValuePtr,
                                                      PublicValueLengthPtr
                                                    );
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_PL_KeyExchangeCalcSecret]-------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_00053, 1, EB_Crypto_01019, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_KeyExchangeCalcSecret
(
  uint32 CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PartnerPublicValuePtr,
  uint32 PartnerPublicValueLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_AL_KeyExchangeCalcSecret(
                                                      CryptoKeyId,
                                                      PartnerPublicValuePtr,
                                                      PartnerPublicValueLength
                                                    );
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_PL_CertificateParse]------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_00054, 1, EB_Crypto_01019, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_CertificateParse
(
  uint32 CryptoKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_AL_CertificateParse(CryptoKeyId);
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_PL_CertificateVerify]-----------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_00055, 1, EB_Crypto_01019, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_CertificateVerify
(
  uint32 CryptoKeyId,
  uint32 VerifyCryptoKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) VerifyPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  RetVal = Crypto_xVIx_xAIx_AL_CertificateVerify(CryptoKeyId, VerifyCryptoKeyId, VerifyPtr);
  return RetVal;
}
#endif /* #if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON) */

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_EnqueueJob]---------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_EnqueueJob
(
  P2VAR(Crypto_xVIx_xAIx_QueueType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Queue,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Crypto_xVIx_xAIx_ProcessFuncPtr_t ProcessFunction
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Crypto_xVIx_xAIx_QueueElementType* Current = NULL_PTR;
  Crypto_xVIx_xAIx_QueueElementType* Previous = NULL_PTR;
  Crypto_xVIx_xAIx_QueueElementType* NewElement = NULL_PTR;
  uint32 queueElementIndex = 0U;

  /* !LINKSTO EB_Crypto_5200, 1 */
  if (NULL_PTR == Queue)
  {
    RetVal = E_NOT_OK;
  }
  /* !LINKSTO EB_Crypto_5201, 1 */
  else if (NULL_PTR == Job)
  {
    RetVal = E_NOT_OK;
  }
  /* !LINKSTO EB_Crypto_5202, 1 */
  else if (NULL_PTR == ProcessFunction)
  {
    RetVal = E_NOT_OK;
  }
  /* !LINKSTO EB_Crypto_5203, 1 */
  else if (0U ==
           (((uint8) Job->jobPrimitiveInputOutput.mode) & ((uint8) CRYPTO_OPERATIONMODE_START))
          )
  {
    /* Only jobs with start bit set are enqueued */
    RetVal = E_NOT_OK;
  }
  /* !LINKSTO SWS_Crypto_00032_CORRECTION, 1 */
  else if (Queue->CurrentSize >= Queue->MaxSize)
  {
    /* Queue is full */
    RetVal = CRYPTO_E_QUEUE_FULL;
  }
  else
  {
    Current = Queue->Head;
    /* !LINKSTO EB_Crypto_5204, 1 */
    if (NULL_PTR == Current)
    {
      /* Queue is empty */
      Queue->Data[0U].Job = Job;
      Queue->Data[0U].ProcessFunction = ProcessFunction;
      Queue->Data[0U].Next = NULL_PTR;
      Queue->Head = &(Queue->Data[0U]);
      Queue->CurrentSize++;
      RetVal = E_OK;
    }
    else
    {
      /* !LINKSTO EB_Crypto_5205, 1 */
      if(FALSE == Crypto_xVIx_xAIx_IsEnqueued(Job->jobInfo->jobId, Queue))
      {
        /* Find empty space in data array */
        for (queueElementIndex = 0U; queueElementIndex < Queue->MaxSize; queueElementIndex++)
        {
          if (NULL_PTR == Queue->Data[queueElementIndex].Job)
          {
            Queue->Data[queueElementIndex].Job = Job;
            Queue->Data[queueElementIndex].ProcessFunction = ProcessFunction;
            NewElement = &Queue->Data[queueElementIndex];
            break;
          }
        }
        /* Link new element according to priority */
        /* !LINKSTO SWS_Crypto_00030, 1 */
        while ((NULL_PTR != Current) &&
               (Current->Job->jobInfo->jobPriority >= Job->jobInfo->jobPriority))
        {
          Previous = Current;
          Current = Current->Next;
        }

        if (NULL_PTR == Previous)
        {
          Queue->Head = NewElement;
        }
        else
        {
          Previous->Next = NewElement;
        }
        NewElement->Next = Current;
        Queue->CurrentSize++;
        RetVal = E_OK;
      }
      else
      {
        RetVal = CRYPTO_E_BUSY;
      }
    }
  }
  return RetVal;
}
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_DequeueElement]-----------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
/* Deviation MISRAC2012-1*/
FUNC(FUNC_P2VAR(Crypto_xVIx_xAIx_QueueElementType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA), CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_DequeueElement
(
  P2VAR(Crypto_xVIx_xAIx_QueueType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Queue
)
{
  Crypto_xVIx_xAIx_QueueElementType* RetVal = NULL_PTR;
  Crypto_xVIx_xAIx_QueueElementType* Current = NULL_PTR;
  /* !LINKSTO EB_Crypto_5300, 1 */
  if (NULL_PTR == Queue)
  {
    RetVal = NULL_PTR;
  }
  else
  {
    /* Unlink first element (highest priority) and set new head element */
    Current = Queue->Head;
    if (NULL_PTR != Current)
    {
      /* !LINKSTO EB_Crypto_5302, 1 */
      RetVal = Current;
      Queue->Head = Current->Next;
      Current->Next = NULL_PTR;
      Queue->CurrentSize--;
    }
    else
    {
      /* !LINKSTO EB_Crypto_5301, 1 */
      RetVal = NULL_PTR;
    }
  }

  return RetVal;
}
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_RemoveJob]----------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RemoveJob
(
  P2VAR(Crypto_xVIx_xAIx_QueueType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Queue,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  /* !LINKSTO EB_Crypto_5102, 1 */
  Std_ReturnType RetVal = E_NOT_OK;
  P2VAR(Crypto_xVIx_xAIx_QueueElementType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Current = NULL_PTR;
  P2VAR(Crypto_xVIx_xAIx_QueueElementType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Previous = NULL_PTR;
  /* !LINKSTO EB_Crypto_5100, 1 */
  if (NULL_PTR == Queue)
  {
    RetVal = E_NOT_OK;
  }
  /* !LINKSTO EB_Crypto_5101, 1 */
  else if (NULL_PTR == Job)
  {
    RetVal = E_NOT_OK;
  }
  else
  {
    Current = Queue->Head;
    /* Look for passed job within queue and remove it if found */
    while ((NULL_PTR != Current) &&
           (Current->Job->jobInfo->jobPriority >= Job->jobInfo->jobPriority)
          )
    {
      if (Current->Job == Job)
      {
        if (Current == Queue->Head)
        {
          Queue->Head = Current->Next;
        }
        else
        {
          Previous->Next = Current->Next;
        }
        /* !LINKSTO EB_Crypto_5103, 1 */
        Current->Job = NULL_PTR;
        Current->Next = NULL_PTR;
        Queue->CurrentSize--;
        RetVal = E_OK;
        break;
      }
      else
      {
        Previous = Current;
        Current = Current->Next;
      }
    }
  }
  return RetVal;
}
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */

/*==================[definition of internal functions]============================================*/

#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
static FUNC(boolean, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_IsEnqueued
(
  uint32 JobId,
  CONSTP2VAR(Crypto_xVIx_xAIx_QueueType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Queue
)
{
  boolean IsEnqueued = FALSE;
  uint32 i = 0U;
  for(i = 0U; i < Queue->MaxSize; i++)
  {
    if(NULL_PTR != Queue->Data[i].Job)
    {
      if(JobId == Queue->Data[i].Job->jobInfo->jobId)
      {
        IsEnqueued = TRUE;
        break;
      }
    }
  }
  return IsEnqueued;
}
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==================[end of file]=================================================================*/
