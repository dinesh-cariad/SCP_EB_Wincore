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

#ifndef CSM_INT_H
#define CSM_INT_H

/*==================[misra deviations]============================================================*/

/*==================[includes]====================================================================*/

#include <Csm_Int_Cfg.h>

/*==================[macros]======================================================================*/

/*==================[type definitions]============================================================*/

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function declarations]==============================================*/

#define CSM_START_SEC_CODE
#include <Csm_MemMap.h>

/** \brief   Enqueue a job into the queue
 **
 ** This function stores the job within a queue element and stores the queue element within the
 ** queue according to the job priority. If the queue is full, this function will record an error.
 **
 ** \param[in]  csmQueuePtr          Queue in which the job shall be inserted
 ** \param[in]  csmJobPtr            Job that shall be put into the queue
 **
 ** \returns    Result of the queueing attempt
 **
 ** \retval     E_OK                 Job was successfully put into the queue
 ** \retval     E_NOT_OK             Request failed, Job was not put into the queue
 ** \retval     CRYPTO_E_QUEUE_FULL  Job could not be enqueued, because the queue is full
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_QueuePushJob
(
  P2VAR(Csm_QueueType, AUTOMATIC, CSM_APPL_DATA) csmQueuePtr,
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) csmJobPtr
);

/** \brief  Remove the referenced job from the queue
 **
 ** This function will search the referenced job in the queue. If it is part of the queue, the job
 ** is removed.
 **
 ** \param[in]  csmQueuePtr          Queue from which the job shall be removed
 ** \param[in]  csmJobPtr            Job that shall be removed
 **
 ** \returns    Result of the request
 **
 ** \retval     E_OK                 Job was successfully removed
 ** \retval     E_NOT_OK             Job is currently not enqueued within the referenced queue
 **/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_QueueDelJob
(
  P2VAR(Csm_QueueType, AUTOMATIC, CSM_APPL_DATA) csmQueuePtr,
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) csmJobPtr
);

#define CSM_STOP_SEC_CODE
#include <Csm_MemMap.h>

/*================================================================================================*/

#endif /* #ifndef CSM_INT_H */

/*==================[end of file]=================================================================*/

