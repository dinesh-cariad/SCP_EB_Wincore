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

#ifndef CSM_INT_TYPES_H
#define CSM_INT_TYPES_H

/*==================[misra deviations]============================================================*/

/*  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 20.7 (required)
 *   "Expressions resulting from the expansion of macro parameters shall be enclosed in
 *    parentheses."
 *
 *   Reason:
 *   Enclosing parameter 'ptrtype' of compiler abstraction macro P2VAR for previous/next pointers
 *   of linked lists tends to faulty c code after preprocessing.
 */

/*==================[includes]====================================================================*/

/* !LINKSTO SWS_Csm_00068,1 */
#include <Std_Types.h>

#include <Csm_Types.h>

/*==================[macros]======================================================================*/

/** \brief  Csm job queue level 'CSM_QL_NONE'. The job is not queued.
 **/
#if (defined CSM_QL_NONE)
  #error CSM_QL_NONE is already defined
#endif /* #if (defined CSM_QL_NONE) */
#define  CSM_QL_NONE    0x0000U

/** \brief  Csm job queue level 'CSM_QL_CSM'. The job is queued in the Csm module.
 **/
#if (defined CSM_QL_CSM)
  #error CSM_QL_CSM is already defined
#endif /* #if (defined CSM_QL_CSM) */
#define  CSM_QL_CSM     0x0001U

/** \brief  Csm job queue level 'CSM_QL_CRYPTO'. The job is queued in the Crypto module.
 **/
#if (defined CSM_QL_CRYPTO)
  #error CSM_QL_CRYPTO is already defined
#endif /* #if (defined CSM_QL_CRYPTO) */
#define  CSM_QL_CRYPTO  0x0002U

/* !LINKSTO CSM.Req.Api/CSM_API_VERSION_430/00001,1 */
#if (defined CSM_API_VERSION_430)
  #error CSM_API_VERSION_430 is already defined
#endif /* #if (defined CSM_API_VERSION_430) */
#define CSM_API_VERSION_430  0x00U

/* !LINKSTO CSM.Req.Api/CSM_API_VERSION_431/00001,1 */
#if (defined CSM_API_VERSION_431)
  #error CSM_API_VERSION_431 is already defined
#endif /* #if (defined CSM_API_VERSION_431) */
#define CSM_API_VERSION_431  0x01U

/* !LINKSTO CSM.Req.Api/CSM_API_VERSION_EB/00001,1 */
#if (defined CSM_API_VERSION_EB)
  #error CSM_API_VERSION_EB is already defined
#endif /* #if (defined CSM_API_VERSION_EB) */
#define CSM_API_VERSION_EB   0x02U

/*==================[type definitions]============================================================*/

/**! \brief  Enumeration of the queue level (None, Csm, Crypto) a job is enqueued in.
 **/
/* !LINKSTO CSM.Dsn.Api/Types/00005,1 */
typedef uint8 Csm_JobQueueLevelType;

/**! \brief  Csm callback notifications function pointer
 **/
/* !LINKSTO CSM.Dsn.Api/Types/00006,1 */
typedef P2FUNC(void, CSM_APPL_CODE, Csm_CallbackNotificationType)
(
  P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job,
  Std_ReturnType result
);

/** \brief  Structure of a Csm queue element
 **/
/* !LINKSTO CSM.Dsn.Api/Types/00001,1 */
typedef struct Csm_QueueElement
{
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job;               /*!< Job that is referenced by the queue element */
  /* Deviation MISRAC2012-1 <+1> */
  P2VAR(struct Csm_QueueElement, AUTOMATIC, CSM_APPL_DATA) next;  /*!< Reference to the next element in the queue */
}
Csm_QueueElementType;

/** \brief  Meta data structure of a Csm queue
 **/
/* !LINKSTO CSM.Dsn.Api/Types/00003,1 */
typedef struct
{
  uint32  queueId;                                                        /*!< Numeric queue identifier. */
  boolean lockMainQueueDelJob;                                            /*!< Flag whether the processing of asyncronous jobs regarding this queue shall be stopped until next main function call, due to locking dequeuing jobs in Csm_MainFunction. */
  boolean lockMainProcessJob;                                             /*!< Flag whether the processing of job with highest priority (== head) shall be prevented in Csm_MainFunction, e.g. to prevent multiple processing due to preemption.       */
  uint32 cryIfChannelId;
  uint32 currentSize;                                                     /*!< Current size of the queue */
  CONST(uint32, CSM_CONST) maxSize;                                    /*!< Maximum number of elements that can be stored in the queue */
  P2VAR(Csm_QueueElementType, AUTOMATIC, CSM_APPL_DATA) head;       /*!< Pointer to the first element of the queue (highest priority) */
  CONSTP2VAR(Csm_QueueElementType, AUTOMATIC, CSM_APPL_DATA) data;  /*!< Pointer to the data array that stores the queue elements */
}
Csm_QueueType;

/**! \brief  Structure which contains additional job configurations.
 **/
/* !LINKSTO CSM.Dsn.Api/Types/00002,1 */
typedef struct
{
  P2VAR(Csm_QueueType, AUTOMATIC, CSM_APPL_DATA) csmQueue;
}
Crypto_JobExtraCnfgType;

/**! \brief  Structure which contains additional job datas.
 **/
/* !LINKSTO CSM.Dsn.Api/Types/00004,1 */
typedef struct
{
  Csm_JobQueueLevelType csmQueueLevel;
}
Crypto_JobExtraDataType;

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function declarations]==============================================*/

/*================================================================================================*/

#endif /* #ifndef CSM_INT_TYPES_H */

/*==================[end of file]=================================================================*/

