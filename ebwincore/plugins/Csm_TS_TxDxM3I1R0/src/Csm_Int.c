/**
 * \file
 *
 * \brief AUTOSAR Csm
 *
 * This file contains the implementation of the AUTOSAR
 * module Csm.
 *
 * \version 3.1.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[misra deviations]============================================================*/

/*==================[includes]====================================================================*/

#include <Csm.h>
#include <Csm_Int.h>

/*==================[macros]======================================================================*/

/*==================[type definitions]============================================================*/

/*==================[internal function declarations]==============================================*/

/*==================[internal constants]==========================================================*/

/*==================[internal data]===============================================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function definitions]===============================================*/

#define CSM_START_SEC_CODE
#include <Csm_MemMap.h>

/* --- Csm_QueuePushJob ------------------------------------------------------------------------- */

FUNC(Std_ReturnType, CSM_CODE) Csm_QueuePushJob
(
  P2VAR(Csm_QueueType, AUTOMATIC, CSM_APPL_DATA) csmQueuePtr,
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) csmJobPtr
)
{
  Std_ReturnType retVal            = E_NOT_OK;
  uint32         queueElementIndex = 0U;
  P2VAR(Csm_QueueElementType, AUTOMATIC, CSM_APPL_DATA) current    = NULL_PTR;
  P2VAR(Csm_QueueElementType, AUTOMATIC, CSM_APPL_DATA) previous   = NULL_PTR;
  P2VAR(Csm_QueueElementType, AUTOMATIC, CSM_APPL_DATA) newElement = NULL_PTR;

  if (csmQueuePtr == NULL_PTR)
  {
    /* !LINKSTO CSM.Req.Gen/Queueing/00003,1 */
    retVal = E_NOT_OK;
  }
  else if (csmJobPtr == NULL_PTR)
  {
    /* !LINKSTO CSM.Req.Gen/Queueing/00003,1 */
    retVal = E_NOT_OK;
  }
  else if (csmQueuePtr->currentSize >= csmQueuePtr->maxSize)
  {
    /* !LINKSTO CSM.Req.Gen/Queueing/00005,1 */
    retVal = CRYPTO_E_QUEUE_FULL;
  }
  else
  {
    current = csmQueuePtr->head;
    if (current == NULL_PTR)
    {
      csmQueuePtr->data[0].job  = csmJobPtr;
      csmQueuePtr->data[0].next = NULL_PTR;
      csmQueuePtr->head         = &(csmQueuePtr->data[0]);
      csmQueuePtr->currentSize++;
      retVal = E_OK;
    }
    else
    {
      /* check for empty element */
      /* CHECK: NOPARSE */
      /* Defensive programming - unreachable code.
       * This loop has at least one cycle, because maxSize is at least 1
       * according to specification
       */
      for (queueElementIndex = 0; queueElementIndex < csmQueuePtr->maxSize; queueElementIndex++)
      /* CHECK: PARSE */
      {
        if (csmQueuePtr->data[queueElementIndex].job == NULL_PTR)
        {
          csmQueuePtr->data[queueElementIndex].job = csmJobPtr;
          newElement = &csmQueuePtr->data[queueElementIndex];
          break;
        }
      }

      /* !LINKSTO SWS_Csm_00944,1 */
      while ( (current != NULL_PTR                                                  ) &&
              (current->job->jobInfo->jobPriority >= csmJobPtr->jobInfo->jobPriority)
            )
      {
        previous = current;
        current = current->next;
      }

      if (previous == NULL_PTR)
      {
        csmQueuePtr->head = newElement;
      }
      else
      {
        previous->next = newElement;
      }

      newElement->next = current;
      csmQueuePtr->currentSize++;
      retVal = E_OK;
    }
  }
  return retVal;
}

/* --- Csm_QueueDelJob -------------------------------------------------------------------------- */

FUNC(Std_ReturnType, CSM_CODE) Csm_QueueDelJob
(
  P2VAR(Csm_QueueType, AUTOMATIC, CSM_APPL_DATA) csmQueuePtr,
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) csmJobPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;
  P2VAR(Csm_QueueElementType, AUTOMATIC, CSM_APPL_DATA) current = NULL_PTR;
  P2VAR(Csm_QueueElementType, AUTOMATIC, CSM_APPL_DATA) previous = NULL_PTR;

  if (csmQueuePtr == NULL_PTR)
  {
    /* !LINKSTO CSM.Req.Gen/Queueing/00003,1 */
    retVal = E_NOT_OK;
  }
  else if (csmJobPtr == NULL_PTR)
  {
    /* !LINKSTO CSM.Req.Gen/Queueing/00003,1 */
    retVal = E_NOT_OK;
  }
  else
  {
    current = csmQueuePtr->head;
    /* !LINKSTO CSM.Req.Gen/Queueing/00006,1 */
    while ( (current != NULL_PTR                                                  ) &&
            (current->job->jobInfo->jobPriority >= csmJobPtr->jobInfo->jobPriority)
          )
    {
      if (current->job == csmJobPtr)
      {
        if (current == csmQueuePtr->head)
        {
          csmQueuePtr->head = current->next;
        }
        else
        {
          previous->next = current->next;
        }
        current->job = NULL_PTR;
        current->next = NULL_PTR;
        csmQueuePtr->currentSize--;
        retVal = E_OK;
        break;
      }
      else
      {
        previous = current;
        current = current->next;
      }
    }
  }
  return retVal;
}

#define CSM_STOP_SEC_CODE
#include <Csm_MemMap.h>

/*==================[internal function definitions]===============================================*/

/*==================[end of file]=================================================================*/

