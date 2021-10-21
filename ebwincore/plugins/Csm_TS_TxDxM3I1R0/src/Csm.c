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

#define CSM_SRC
/*==================[misra deviations]============================================================*/

/*  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 2.2 (required)
 *   There shall be no dead code.
 *
 *   Reason:
 *   The Csm SWS specifies unused API function parameters for interface compatibility reasons.
 *   That is why the TS_PARAM_UNUSED macro is used to prevent compiler warnings for unused
 *   variables.
 */

/*  Code-Metric Deviation List
 *
 *  CODEMETRIC-1) Deviated Rule: HIS_PARAM
 *   not 11 <= 8
 *
 *   Reason:
 *   The specification of Crypto Service Manager AUTOSAR CP Release 4.3.0 specifies API function
 *   Csm_AEADDecrypt (SWS_Csm_01026) with 11 parameters
 *
 *  CODEMETRIC-2) Deviated Rule: HIS_PARAM
 *   not 10 <= 8
 *
 *   Reason:
 *   The specification of Crypto Service Manager AUTOSAR CP Release 4.3.0 specifies API function
 *   Csm_AEADEncrypt (SWS_Csm_01023) with 10 parameters
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W588
 * W588: dead assignment to "Csm_Initialized" eliminated violated
 *
 * Reason:
 * Usage of variable "Csm_Initialized" depends on preprocessor settings that are configuration
 * specific
 *
 * TASKING-2) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * The Csm specifiaction demands that the configured number of jobs, CSM_JOB_COUNT, and the configured
 * configured number of queues, CSM_QUE_COUNT are always bigger than zero.
 */
/*==================[includes]====================================================================*/

#include <CryIf.h>

#include <Csm.h>
#include <Csm_Int.h>

#include <SchM_Csm.h>

#include <TSAutosar.h>

/* !LINKSTO CSM.Req.Gen/Det/00001,1 */
#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
/* !LINKSTO SWS_Csm_00485,1 */
#include <Det.h>
#endif

#if CSM_RTE_ENABLED == STD_ON
#include <Rte_Csm.h>
#endif

/*==================[macros]======================================================================*/

/*==================[type definitions]============================================================*/

/*==================[internal function declarations]==============================================*/

#define CSM_START_SEC_CODE
#include <Csm_MemMap.h>

/** \brief  Handle processing of queueing of asycronous job requests and forwarding of job requests
 **         to CryIf.
 **/
FUNC(Std_ReturnType, CSM_CODE) Csm_Process
(
  uint32 jobId,
  P2VAR(Crypto_JobPrimitiveInputOutputType, AUTOMATIC, CSM_APPL_DATA) jobPrimitiveInputOutput
);

#define CSM_STOP_SEC_CODE
#include <Csm_MemMap.h>

/*==================[internal constants]==========================================================*/

/*==================[internal data]===============================================================*/

#define CSM_START_SEC_VAR_INIT_BOOLEAN
#include <Csm_MemMap.h>

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
/** \brief  Variable that represents whether the Csm module is fully initialized.
 **/
/* !LINKSTO CSM.Dsn.Api/Datas/00001,1 */
static VAR(boolean, CSM_VAR) Csm_Initialized = FALSE;
#endif

#define CSM_STOP_SEC_VAR_INIT_BOOLEAN
#include <Csm_MemMap.h>

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function definitions]===============================================*/

#define CSM_START_SEC_CODE
#include <Csm_MemMap.h>

/* --- Csm_AEADDecrypt -------------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_01026,1 */

#if (CSM_API_ENABLED_SERVICE_AEADDECRYPT == STD_ON)
/* Deviation CODEMETRIC-1 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecrypt
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) ciphertextPtr,
          uint32                                              ciphertextLength,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) associatedDataPtr,
          uint32                                              associatedDataLength,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) tagPtr,
          uint32                                              tagLength,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) plaintextPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) plaintextLengthPtr,
    P2VAR(Crypto_VerifyResultType,  AUTOMATIC, CSM_APPL_DATA) verifyPtr
)
{
  Std_ReturnType                     result                  = E_NOT_OK;
  Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput = {
                                                                 NULL_PTR, 0U,                    /* inputPtr,           inputLength              */
                                                                 NULL_PTR, 0U,                    /* secondaryInputPtr,  secondaryInputLength     */
                                                                 NULL_PTR, 0U,                    /* tertiaryInputPtr,   tertiaryInputLength      */
                                                                 NULL_PTR, NULL_PTR,              /* outputPtr,          outputLengthPtr          */
                                                                 NULL_PTR, NULL_PTR,              /* secondaryOutputPtr, secondaryOutputLengthPtr */
#if (CSM_API_VERSION == CSM_API_VERSION_431)
                                                                 0U,                              /* input64                                      */
#endif /* (CSM_API_VERSION == CSM_API_VERSION_431) */
                                                                 NULL_PTR,                        /* verifyPtr                                    */
#if (CSM_API_VERSION != CSM_API_VERSION_EB)
                                                                 NULL_PTR,                        /* output64Ptr                                  */
#endif /* (CSM_API_VERSION != CSM_API_VERSION_EB) */
                                                                 CRYPTO_OPERATIONMODE_SINGLECALL  /* mode                                         */
                                                               };

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADDECRYPT,
                          CSM_E_UNINIT
                         );
  }
  else if (jobId >= CSM_JOB_COUNT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00002,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADDECRYPT,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (Csm_JobConfigurations[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_AEADDECRYPT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00003,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADDECRYPT,
                          CSM_E_SERVICE_NOT_IDENTICAL
                         );
  }
  else if (ciphertextPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADDECRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (associatedDataPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADDECRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (tagPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADDECRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (plaintextPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADDECRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (plaintextLengthPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADDECRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (verifyPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADDECRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    if ( (  (mode & CRYPTO_OPERATIONMODE_SINGLECALL) ==
              (CRYPTO_OPERATIONMODE_START|CRYPTO_OPERATIONMODE_FINISH)
         ) ||
         ( ((mode & CRYPTO_OPERATIONMODE_SINGLECALL) == 0U                             ) ||
           ( mode                                     > CRYPTO_OPERATIONMODE_SINGLECALL)
         )
       )
    {
      /* !LINKSTO SWS_Csm_01045,1, CSM.Req.Api/Csm_Service/00001,1, CSM.Req.Api/Csm_Service/00002,1 */
      result = E_NOT_OK;
    }
    else
    {
      /* !LINKSTO CSM.Req.Correction.SWS_Csm_01027,1 */
      jobPrimitiveInputOutput.mode                 = mode;
      jobPrimitiveInputOutput.inputPtr             = ciphertextPtr;
      jobPrimitiveInputOutput.inputLength          = ciphertextLength;
      jobPrimitiveInputOutput.secondaryInputPtr    = associatedDataPtr;
      jobPrimitiveInputOutput.secondaryInputLength = associatedDataLength;
      jobPrimitiveInputOutput.tertiaryInputPtr     = tagPtr;
      jobPrimitiveInputOutput.tertiaryInputLength  = tagLength;
      jobPrimitiveInputOutput.outputPtr            = plaintextPtr;
      jobPrimitiveInputOutput.outputLengthPtr      = plaintextLengthPtr;
      jobPrimitiveInputOutput.verifyPtr            = verifyPtr;

      result = Csm_Process(jobId, &jobPrimitiveInputOutput);
    }
  }

  return result;
}
#endif /* #if (CSM_API_ENABLED_SERVICE_AEADDECRYPT == STD_ON) */

/* --- Csm_AEADEncrypt -------------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_01023,1 */

#if (CSM_API_ENABLED_SERVICE_AEADENCRYPT == STD_ON)
/* Deviation CODEMETRIC-2 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncrypt
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) plaintextPtr,
          uint32                                              plaintextLength,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) associatedDataPtr,
          uint32                                              associatedDataLength,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) ciphertextPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) ciphertextLengthPtr,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) tagPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) tagLengthPtr
)
{
  Std_ReturnType                     result                  = E_NOT_OK;
  Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput = {
                                                                 NULL_PTR, 0U,                    /* inputPtr,           inputLength              */
                                                                 NULL_PTR, 0U,                    /* secondaryInputPtr,  secondaryInputLength     */
                                                                 NULL_PTR, 0U,                    /* tertiaryInputPtr,   tertiaryInputLength      */
                                                                 NULL_PTR, NULL_PTR,              /* outputPtr,          outputLengthPtr          */
                                                                 NULL_PTR, NULL_PTR,              /* secondaryOutputPtr, secondaryOutputLengthPtr */
#if (CSM_API_VERSION == CSM_API_VERSION_431)
                                                                 0U,                              /* input64                                      */
#endif /* (CSM_API_VERSION == CSM_API_VERSION_431) */
                                                                 NULL_PTR,                        /* verifyPtr                                    */
#if (CSM_API_VERSION != CSM_API_VERSION_EB)
                                                                 NULL_PTR,                        /* output64Ptr                                  */
#endif /* (CSM_API_VERSION != CSM_API_VERSION_EB) */
                                                                 CRYPTO_OPERATIONMODE_SINGLECALL  /* mode                                         */
                                                               };

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADENCRYPT,
                          CSM_E_UNINIT
                         );
  }
  else if (jobId >= CSM_JOB_COUNT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00002,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADENCRYPT,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (Csm_JobConfigurations[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_AEADENCRYPT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00003,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADENCRYPT,
                          CSM_E_SERVICE_NOT_IDENTICAL
                         );
  }
  else if (plaintextPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADENCRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (associatedDataPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADENCRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (ciphertextPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADENCRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (ciphertextLengthPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADENCRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (tagPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADENCRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (tagLengthPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_AEADENCRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    if ( (  (mode & CRYPTO_OPERATIONMODE_SINGLECALL) ==
              (CRYPTO_OPERATIONMODE_START|CRYPTO_OPERATIONMODE_FINISH)
         ) ||
         ( ((mode & CRYPTO_OPERATIONMODE_SINGLECALL) == 0U                             ) ||
           ( mode                                     > CRYPTO_OPERATIONMODE_SINGLECALL)
         )
       )
    {
      /* !LINKSTO SWS_Csm_01045,1, CSM.Req.Api/Csm_Service/00001,1, CSM.Req.Api/Csm_Service/00002,1 */
      result = E_NOT_OK;
    }
    else
    {
      /* !LINKSTO CSM.Req.Correction.SWS_Csm_01025,1 */
      jobPrimitiveInputOutput.mode                     = mode;
      jobPrimitiveInputOutput.inputPtr                 = plaintextPtr;
      jobPrimitiveInputOutput.inputLength              = plaintextLength;
      jobPrimitiveInputOutput.secondaryInputPtr        = associatedDataPtr;
      jobPrimitiveInputOutput.secondaryInputLength     = associatedDataLength;
      jobPrimitiveInputOutput.outputPtr                = ciphertextPtr;
      jobPrimitiveInputOutput.outputLengthPtr          = ciphertextLengthPtr;
      jobPrimitiveInputOutput.secondaryOutputPtr       = tagPtr;
      jobPrimitiveInputOutput.secondaryOutputLengthPtr = tagLengthPtr;

      result = Csm_Process(jobId, &jobPrimitiveInputOutput);
    }
  }

  return result;
}
#endif /* #if (CSM_API_ENABLED_SERVICE_AEADENCRYPT == STD_ON) */

/* --- Csm_CallbackNotification ----------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00970,1 */

FUNC(void, CSM_CODE) Csm_CallbackNotification
(
  P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job,
          Std_ReturnType                            result
)
{
#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_CALLBACKNOTIFICATION,
                          CSM_E_UNINIT
                         );
  }
  else if (job == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_CALLBACKNOTIFICATION,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    if (result != E_OK)
    {
#if (CSM_CBK_COUNT != 0)
      if ( job->jobPrimitiveInfo->callbackId != CSM_CBK_EMPTY)
      {
        Csm_Callbacks[job->jobPrimitiveInfo->callbackId](job, result);
      }
#endif /* #if (CSM_CBK_COUNT != 0) */
      Csm_JobExtraData[job->jobId].csmQueueLevel = CSM_QL_NONE;
    }
    else
    {
      /* !LINKSTO CSM.Req.Correction.SWS_Csm_01053,2 */
      if (((job->CSM_API_JOBPRIMITIVEINPUTOUTPUT.mode & CRYPTO_OPERATIONMODE_UPDATE    )
             != 0U
          ) &&
          ((job->CSM_API_JOBPRIMITIVEINPUTOUTPUT.mode & CRYPTO_OPERATIONMODE_SINGLECALL)
             != CRYPTO_OPERATIONMODE_SINGLECALL
          ) &&
          ( job->jobPrimitiveInfo->callbackId                 != CSM_CBK_EMPTY) &&
          ( job->jobPrimitiveInfo->callbackUpdateNotification == TRUE         )
         )
      {
#if (CSM_CBK_COUNT != 0U)
        /* !LINKSTO SWS_Csm_00039,1 */
        Csm_Callbacks[job->jobPrimitiveInfo->callbackId](job, result);
#endif /* #if (CSM_CBK_COUNT != 0U) */
      }
      /* !LINKSTO CSM.Req.Correction.SWS_Csm_01044,1 */
      if (((job->CSM_API_JOBPRIMITIVEINPUTOUTPUT.mode & CRYPTO_OPERATIONMODE_FINISH) != 0U           ) &&
          ( job->jobPrimitiveInfo->callbackId                                        != CSM_CBK_EMPTY)
         )
      {
#if (CSM_CBK_COUNT != 0U)
        Csm_Callbacks[job->jobPrimitiveInfo->callbackId](job, result);
#endif /* #if (CSM_CBK_COUNT != 0U) */
        if( Csm_JobExtraData[job->jobId].csmQueueLevel == CSM_QL_CRYPTO)
        {
          Csm_JobExtraData[job->jobId].csmQueueLevel = CSM_QL_NONE;
        }
      }
    }
  }
}

/* --- Csm_CancelJob ---------------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00968,1 */

#if (CSM_API_ENABLED_SERVICE_GENERAL == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJob
(
  uint32                   job,
  Crypto_OperationModeType mode
)
{
  Std_ReturnType result = E_NOT_OK;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_CANCELJOB,
                          CSM_E_UNINIT
                         );
  }
  else if (job >= CSM_JOB_COUNT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00002,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_CANCELJOB,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* Deviation MISRAC2012-1 <+1> */
    TS_PARAM_UNUSED(mode);

    if (Csm_JobConfigurations[job].jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_ASYNC)
    {
      /* !LINKSTO SWS_Csm_01021,1 */
      if (Csm_JobExtraData[job].csmQueueLevel == CSM_QL_CSM)
      {
        /* !LINKSTO CSM.Dsn.Api/ExAreas/00001,1, CSM.Dsn.Api/ExAreas/00002,1 */
        SchM_Enter_Csm_SCHM_CSM_EXCLUSIVE_AREA_0();
        result = Csm_QueueDelJob(Csm_JobExtraCnfg[job].csmQueue,
                                    &(Csm_JobConfigurations[job])
                                   );
        SchM_Exit_Csm_SCHM_CSM_EXCLUSIVE_AREA_0();
      }
      else
      if (Csm_JobExtraData[job].csmQueueLevel == CSM_QL_CRYPTO)
      {
        /* !LINKSTO SWS_Csm_00506,1, SWS_Csm_01021,1 */
        result = CryIf_CancelJob(Csm_JobExtraCnfg[job].csmQueue->cryIfChannelId,
                                 &(Csm_JobConfigurations[job])
                                );
      }
      else
      {
        /* intentionally empty */
      }

      if (result == E_OK)
      {
  #if (CSM_CBK_COUNT != 0U)
        if ((Csm_JobExtraData[job].csmQueueLevel == CSM_QL_CSM) &&
            (Csm_JobConfigurations[job].jobPrimitiveInfo->callbackId != CSM_CBK_EMPTY)
           )
        {
          /* !LINKSTO SWS_Csm_01030,1 */
          Csm_Callbacks[Csm_JobConfigurations[job].jobPrimitiveInfo->callbackId]
          (
            &(Csm_JobConfigurations[job]),
            CRYPTO_E_JOB_CANCELED
          );
        }
  #endif /* #if (CSM_CBK_COUNT != 0U) */
        Csm_JobExtraData[job].csmQueueLevel = CSM_QL_NONE;
      }
    }
    else
    {
      /* !LINKSTO SWS_Csm_00506,1 */
      result = CryIf_CancelJob(Csm_JobExtraCnfg[job].csmQueue->cryIfChannelId,
                               &(Csm_JobConfigurations[job])
                              );
    }
  }

  return result;
}
#endif /* #if (CSM_API_ENABLED_SERVICE_GENERAL == STD_ON) */

/* --- Csm_CertificateParse --------------------------------------------------------------------- */

/* !LINKSTO SWS_Csm_01036,1, CSM.Req.Gen/CsmKeys/00002,1 */

#if (CSM_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateParse
(
  uint32 keyId
)
{
  Std_ReturnType result = E_NOT_OK;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_CERTIFICATEPARSE,
                          CSM_E_UNINIT
                         );
  }
  else if (keyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_CERTIFICATEPARSE,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Csm_00506,1, SWS_Csm_01037,1 */
    result = CryIf_CertificateParse(Csm_Keys[keyId]);
  }

  return result;
}
#endif /* #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON) */

/* --- Csm_CertificateVerify -------------------------------------------------------------------- */

/* !LINKSTO SWS_Csm_01038,1, CSM.Req.Gen/CsmKeys/00002,1 */

#if (CSM_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateVerify
(
        uint32                                             keyId,
        uint32                                             verifyCryIfKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_DATA) verifyPtr
)
{
  Std_ReturnType result = E_NOT_OK;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_CERTIFICATEVERIFY,
                          CSM_E_UNINIT
                         );
  }
  else if (keyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_CERTIFICATEVERIFY,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (verifyCryIfKeyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_CERTIFICATEVERIFY,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (verifyPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_CERTIFICATEVERIFY,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Csm_00506,1, SWS_Csm_01040,1 */
    result = CryIf_CertificateVerify(Csm_Keys[keyId], Csm_Keys[verifyCryIfKeyId], verifyPtr);
  }

  return result;
}
#endif /* #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON) */

/* --- Csm_Decrypt ------------------------------------------------------------------------------ */

/* !LINKSTO CSM.Dsn.SWS_Csm_00989,1 */

#if (CSM_API_ENABLED_SERVICE_DECRYPT == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_Decrypt
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) dataPtr,
          uint32                                              dataLength,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) resultLengthPtr
)
{
  Std_ReturnType                     result                  = E_NOT_OK;
  Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput = {
                                                                 NULL_PTR, 0U,                    /* inputPtr,           inputLength              */
                                                                 NULL_PTR, 0U,                    /* secondaryInputPtr,  secondaryInputLength     */
                                                                 NULL_PTR, 0U,                    /* tertiaryInputPtr,   tertiaryInputLength      */
                                                                 NULL_PTR, NULL_PTR,              /* outputPtr,          outputLengthPtr          */
                                                                 NULL_PTR, NULL_PTR,              /* secondaryOutputPtr, secondaryOutputLengthPtr */
#if (CSM_API_VERSION == CSM_API_VERSION_431)
                                                                 0U,                              /* input64                                      */
#endif /* (CSM_API_VERSION == CSM_API_VERSION_431) */
                                                                 NULL_PTR,                        /* verifyPtr                                    */
#if (CSM_API_VERSION != CSM_API_VERSION_EB)
                                                                 NULL_PTR,                        /* output64Ptr                                  */
#endif /* (CSM_API_VERSION != CSM_API_VERSION_EB) */
                                                                 CRYPTO_OPERATIONMODE_SINGLECALL  /* mode                                         */
                                                               };

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_DECRYPT,
                          CSM_E_UNINIT
                         );
  }
  else if (jobId >= CSM_JOB_COUNT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00002,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_DECRYPT,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (Csm_JobConfigurations[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_DECRYPT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00003,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_DECRYPT,
                          CSM_E_SERVICE_NOT_IDENTICAL
                         );
  }
  else if (dataPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_DECRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (resultPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_DECRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (resultLengthPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_DECRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    if ( (  (mode & CRYPTO_OPERATIONMODE_SINGLECALL) ==
              (CRYPTO_OPERATIONMODE_START|CRYPTO_OPERATIONMODE_FINISH)
         ) ||
         ( ((mode & CRYPTO_OPERATIONMODE_SINGLECALL) == 0U                             ) ||
           ( mode                                     > CRYPTO_OPERATIONMODE_SINGLECALL)
         )
       )
    {
      /* !LINKSTO SWS_Csm_01045,1, CSM.Req.Api/Csm_Service/00001,1, CSM.Req.Api/Csm_Service/00002,1 */
      result = E_NOT_OK;
    }
    else
    {
      /* !LINKSTO SWS_Csm_00990,1 */
      jobPrimitiveInputOutput.mode            = mode;
      jobPrimitiveInputOutput.inputPtr        = dataPtr;
      jobPrimitiveInputOutput.inputLength     = dataLength;
      jobPrimitiveInputOutput.outputPtr       = resultPtr;
      jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr;

      result = Csm_Process(jobId, &jobPrimitiveInputOutput);
    }
  }
#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (result == CRYPTO_E_SMALL_BUFFER)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00830,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_DECRYPT,
                          CRYPTO_E_SMALL_BUFFER
                         );
  }
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */

  return result;
}
#endif /* #if (CSM_API_ENABLED_SERVICE_DECRYPT == STD_ON) */

/* --- Csm_Encrypt ------------------------------------------------------------------------------ */

/* !LINKSTO CSM.Dsn.SWS_Csm_00984,1 */

#if (CSM_API_ENABLED_SERVICE_ENCRYPT == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_Encrypt
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) dataPtr,
          uint32                                              dataLength,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) resultLengthPtr
)
{
  Std_ReturnType                     result                  = E_NOT_OK;
  Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput = {
                                                                 NULL_PTR, 0U,                    /* inputPtr,           inputLength              */
                                                                 NULL_PTR, 0U,                    /* secondaryInputPtr,  secondaryInputLength     */
                                                                 NULL_PTR, 0U,                    /* tertiaryInputPtr,   tertiaryInputLength      */
                                                                 NULL_PTR, NULL_PTR,              /* outputPtr,          outputLengthPtr          */
                                                                 NULL_PTR, NULL_PTR,              /* secondaryOutputPtr, secondaryOutputLengthPtr */
#if (CSM_API_VERSION == CSM_API_VERSION_431)
                                                                 0U,                              /* input64                                      */
#endif /* (CSM_API_VERSION == CSM_API_VERSION_431) */
                                                                 NULL_PTR,                        /* verifyPtr                                    */
#if (CSM_API_VERSION != CSM_API_VERSION_EB)
                                                                 NULL_PTR,                        /* output64Ptr                                  */
#endif /* (CSM_API_VERSION != CSM_API_VERSION_EB) */
                                                                 CRYPTO_OPERATIONMODE_SINGLECALL  /* mode                                         */
                                                               };

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_ENCRYPT,
                          CSM_E_UNINIT
                         );
  }
  else if (jobId >= CSM_JOB_COUNT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00002,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_ENCRYPT,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (Csm_JobConfigurations[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_ENCRYPT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00003,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_ENCRYPT,
                          CSM_E_SERVICE_NOT_IDENTICAL
                         );
  }
  else if (dataPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_ENCRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (resultPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_ENCRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (resultLengthPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_ENCRYPT,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    if ( (  (mode & CRYPTO_OPERATIONMODE_SINGLECALL) ==
              (CRYPTO_OPERATIONMODE_START|CRYPTO_OPERATIONMODE_FINISH)
         ) ||
         ( ((mode & CRYPTO_OPERATIONMODE_SINGLECALL) == 0U                             ) ||
           ( mode                                     > CRYPTO_OPERATIONMODE_SINGLECALL)
         )
       )
    {
      /* !LINKSTO SWS_Csm_01045,1, CSM.Req.Api/Csm_Service/00001,1, CSM.Req.Api/Csm_Service/00002,1 */
      result = E_NOT_OK;
    }
    else
    {
      /* !LINKSTO SWS_Csm_00986,1 */
      jobPrimitiveInputOutput.mode            = mode;
      jobPrimitiveInputOutput.inputPtr        = dataPtr;
      jobPrimitiveInputOutput.inputLength     = dataLength;
      jobPrimitiveInputOutput.outputPtr       = resultPtr;
      jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr;

      result = Csm_Process(jobId, &jobPrimitiveInputOutput);
    }
  }
#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (result == CRYPTO_E_SMALL_BUFFER)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00830,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_ENCRYPT,
                          CRYPTO_E_SMALL_BUFFER
                         );
  }
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */

  return result;
}
#endif /* #if (CSM_API_ENABLED_SERVICE_ENCRYPT == STD_ON) */

/* --- Csm_GetVersionInfo ----------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00705,1 */

#if (CSM_API_ENABLED_VERSIONINFO == STD_ON)
FUNC(void, CSM_CODE) Csm_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CSM_APPL_DATA) versioninfo
)
{
#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (versioninfo == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_GETVERSIONINFO,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    versioninfo->vendorID         = CSM_VENDOR_ID;
    versioninfo->moduleID         = CSM_MODULE_ID;
    versioninfo->sw_major_version = CSM_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = CSM_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = CSM_SW_PATCH_VERSION;
  }
}
#endif /* #if (CSM_API_ENABLED_VERSIONINFO == STD_ON) */

/* --- Csm_Hash --------------------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00980,1 */

#if (CSM_API_ENABLED_SERVICE_HASH == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_Hash
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) dataPtr,
          uint32                                              dataLength,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) resultLengthPtr
)
{
  Std_ReturnType                     result                  = E_NOT_OK;
  Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput = {
                                                                 NULL_PTR, 0U,                    /* inputPtr,           inputLength              */
                                                                 NULL_PTR, 0U,                    /* secondaryInputPtr,  secondaryInputLength     */
                                                                 NULL_PTR, 0U,                    /* tertiaryInputPtr,   tertiaryInputLength      */
                                                                 NULL_PTR, NULL_PTR,              /* outputPtr,          outputLengthPtr          */
                                                                 NULL_PTR, NULL_PTR,              /* secondaryOutputPtr, secondaryOutputLengthPtr */
#if (CSM_API_VERSION == CSM_API_VERSION_431)
                                                                 0U,                              /* input64                                      */
#endif /* (CSM_API_VERSION == CSM_API_VERSION_431) */
                                                                 NULL_PTR,                        /* verifyPtr                                    */
#if (CSM_API_VERSION != CSM_API_VERSION_EB)
                                                                 NULL_PTR,                        /* output64Ptr                                  */
#endif /* (CSM_API_VERSION != CSM_API_VERSION_EB) */
                                                                 CRYPTO_OPERATIONMODE_SINGLECALL  /* mode                                         */
                                                               };

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_HASH,
                          CSM_E_UNINIT
                         );
  }
  else if (jobId >= CSM_JOB_COUNT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00002,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_HASH,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (Csm_JobConfigurations[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_HASH)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00003,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_HASH,
                          CSM_E_SERVICE_NOT_IDENTICAL
                         );
  }
  else if (dataPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_HASH,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (resultPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_HASH,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (resultLengthPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_HASH,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    if ( (  (mode & CRYPTO_OPERATIONMODE_SINGLECALL) ==
              (CRYPTO_OPERATIONMODE_START|CRYPTO_OPERATIONMODE_FINISH)
         ) ||
         ( ((mode & CRYPTO_OPERATIONMODE_SINGLECALL) == 0U                             ) ||
           ( mode                                     > CRYPTO_OPERATIONMODE_SINGLECALL)
         )
       )
    {
      /* !LINKSTO SWS_Csm_01045,1, CSM.Req.Api/Csm_Service/00001,1, CSM.Req.Api/Csm_Service/00002,1 */
      result = E_NOT_OK;
    }
    else
    {
      /* !LINKSTO SWS_Csm_01015,1 */
      jobPrimitiveInputOutput.mode            = mode;
      jobPrimitiveInputOutput.inputPtr        = dataPtr;
      jobPrimitiveInputOutput.inputLength     = dataLength;
      jobPrimitiveInputOutput.outputPtr       = resultPtr;
      jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr;

      result = Csm_Process(jobId, &jobPrimitiveInputOutput);
    }
  }
#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (result == CRYPTO_E_SMALL_BUFFER)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00830,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_HASH,
                          CRYPTO_E_SMALL_BUFFER
                         );
  }
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */

  return result;
}
#endif /* #if (CSM_API_ENABLED_SERVICE_HASH == STD_ON) */

/* --- Csm_Init --------------------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00646,1 */

FUNC(void, CSM_CODE) Csm_Init
(
  void
)
{
  uint32 i = 0U;
  uint32 j = 0U;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* Deviation TASKING-1 */
  Csm_Initialized = FALSE;
#endif

  /* Deviation TASKING-2 */
  for (i=0U; i<CSM_JOB_COUNT; i++)
  {
    /* !LINKSTO CSM.Req.Api/Csm_Init/00001,1 */
    Csm_JobConfigurations[i].CSM_API_JOBSTATE                                         = CRYPTO_JOBSTATE_IDLE;
    Csm_JobConfigurations[i].cryptoKeyId                                              = CSM_KEY_EMPTY;
    Csm_JobConfigurations[i].CSM_API_JOBPRIMITIVEINPUTOUTPUT.inputPtr                 = NULL_PTR;
    Csm_JobConfigurations[i].CSM_API_JOBPRIMITIVEINPUTOUTPUT.inputLength              = 0U;
    Csm_JobConfigurations[i].CSM_API_JOBPRIMITIVEINPUTOUTPUT.secondaryInputPtr        = NULL_PTR;
    Csm_JobConfigurations[i].CSM_API_JOBPRIMITIVEINPUTOUTPUT.secondaryInputLength     = 0U;
    Csm_JobConfigurations[i].CSM_API_JOBPRIMITIVEINPUTOUTPUT.tertiaryInputPtr         = NULL_PTR;
    Csm_JobConfigurations[i].CSM_API_JOBPRIMITIVEINPUTOUTPUT.tertiaryInputLength      = 0U;
    Csm_JobConfigurations[i].CSM_API_JOBPRIMITIVEINPUTOUTPUT.outputPtr                = NULL_PTR;
    Csm_JobConfigurations[i].CSM_API_JOBPRIMITIVEINPUTOUTPUT.outputLengthPtr          = NULL_PTR;
    Csm_JobConfigurations[i].CSM_API_JOBPRIMITIVEINPUTOUTPUT.secondaryOutputPtr       = NULL_PTR;
    Csm_JobConfigurations[i].CSM_API_JOBPRIMITIVEINPUTOUTPUT.secondaryOutputLengthPtr = NULL_PTR;
    Csm_JobConfigurations[i].CSM_API_JOBPRIMITIVEINPUTOUTPUT.verifyPtr                = NULL_PTR;
    Csm_JobConfigurations[i].CSM_API_JOBPRIMITIVEINPUTOUTPUT.mode                     = CRYPTO_OPERATIONMODE_SINGLECALL;

    /* !LINKSTO CSM.Req.Api/Csm_Init/00003,1 */
    Csm_JobExtraCnfg[i].csmQueue->lockMainQueueDelJob = FALSE;
    Csm_JobExtraCnfg[i].csmQueue->lockMainProcessJob  = FALSE;
    Csm_JobExtraCnfg[i].csmQueue->currentSize         = 0U;
    Csm_JobExtraCnfg[i].csmQueue->head                = NULL_PTR;

    for (j=0U; j<Csm_JobExtraCnfg[i].csmQueue->maxSize; j++)
    {
      Csm_JobExtraCnfg[i].csmQueue->data[j].job  = NULL_PTR;
      Csm_JobExtraCnfg[i].csmQueue->data[j].next = NULL_PTR;
    }

    /* !LINKSTO CSM.Req.Api/Csm_Init/00002,1 */
    Csm_JobExtraData[i].csmQueueLevel = CSM_QL_NONE;
  }

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  Csm_Initialized = TRUE;

  /* CHECK: NOPARSE
   * Defensive Programming - unreachable code.
   */
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00659,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_INIT,
                          CSM_E_INIT_FAILED
                         );
  }
  /* CHECK: PARSE */
#endif
}

/* --- Csm_KeyCopy ------------------------------------------------------------------------------ */

/* !LINKSTO CSM.Dsn.SWS_Csm_01034,1, CSM.Req.Gen/CsmKeys/00002,1 */

#if (CSM_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyCopy
(
  uint32 keyId,
  uint32 targetKeyId
)
{
  Std_ReturnType result = E_NOT_OK;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYCOPY,
                          CSM_E_UNINIT
                         );
  }
  else if (keyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYCOPY,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (targetKeyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYCOPY,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Csm_00506,1, CSM.Req.Correction.SWS_Csm_01035,1 */
    result = CryIf_KeyCopy(Csm_Keys[keyId], Csm_Keys[targetKeyId]);
  }

  return result;
}
#endif /* #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON) */

/* --- Csm_KeyDerive ---------------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00956,1, CSM.Req.Gen/CsmKeys/00002,1 */

#if (CSM_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyDerive
(
  uint32 keyId,
  uint32 targetKeyId
)
{
  Std_ReturnType result = E_NOT_OK;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYDERIVE,
                          CSM_E_UNINIT
                         );
  }
  else if (keyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYDERIVE,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (targetKeyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYDERIVE,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Csm_00506,1, SWS_Csm_01018,1 */
    result = CryIf_KeyDerive(Csm_Keys[keyId], Csm_Keys[targetKeyId]);
  }

  return result;
}
#endif /* #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON) */

/* --- Csm_KeyElementCopy ----------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00969,1, CSM.Req.Gen/CsmKeys/00002,1 */

#if (CSM_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopy
(
  uint32 keyId,
  uint32 keyElementId,
  uint32 targetKeyId,
  uint32 targetKeyElementId
)
{
  Std_ReturnType result = E_NOT_OK;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYELEMENTCOPY,
                          CSM_E_UNINIT
                         );
  }
  else if (keyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYELEMENTCOPY,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (targetKeyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYELEMENTCOPY,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Csm_00506,1, SWS_Csm_01032,1 */
    result = CryIf_KeyElementCopy(Csm_Keys[keyId], keyElementId, Csm_Keys[targetKeyId], targetKeyElementId);
  }

  return result;
}
#endif /* #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON) */

/* --- Csm_KeyElementGet ------------------------------------------------------------------------ */

/* !LINKSTO CSM.Dsn.SWS_Csm_00959,1, CSM.Req.Gen/CsmKeys/00002,1 */

#if (CSM_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementGet
(
        uint32                            keyId,
        uint32                            keyElementId,
  P2VAR(uint8,  AUTOMATIC, CSM_APPL_DATA) keyPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) keyLengthPtr
)
{
  Std_ReturnType result = E_NOT_OK;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYELEMENTGET,
                          CSM_E_UNINIT
                         );
  }
  else if (keyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYELEMENTGET,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (keyPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYELEMENTGET,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (keyLengthPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYELEMENTGET,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Csm_00506,1, SWS_Csm_01004,1 */
    result = CryIf_KeyElementGet(Csm_Keys[keyId], keyElementId, keyPtr, keyLengthPtr);
  }
#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (result == CRYPTO_E_SMALL_BUFFER)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00830,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYELEMENTGET,
                          CRYPTO_E_SMALL_BUFFER
                         );
  }
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */

  return result;
}
#endif /* #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON) */

/* --- Csm_KeyElementSet ------------------------------------------------------------------------ */

/* !LINKSTO CSM.Dsn.SWS_Csm_00957,1, CSM.Req.Gen/CsmKeys/00002,1 */

#if (CSM_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementSet
(
          uint32                            keyId,
          uint32                            keyElementId,
  P2CONST(uint8,  AUTOMATIC, CSM_APPL_DATA) keyPtr,
          uint32                            keyLength
)
{
  Std_ReturnType result = E_NOT_OK;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYELEMENTSET,
                          CSM_E_UNINIT
                         );
  }
  else if (keyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYELEMENTSET,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (keyPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYELEMENTSET,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Csm_00506,1, SWS_Csm_01002,1 */
    result = CryIf_KeyElementSet(Csm_Keys[keyId], keyElementId, keyPtr, keyLength);
  }

  return result;
}
#endif /* #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON) */

/* --- Csm_KeyExchangeCalcPubVal ---------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00966,1, CSM.Req.Gen/CsmKeys/00002,1 */

#if (CSM_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcPubVal
(
        uint32                            keyId,
  P2VAR(uint8,  AUTOMATIC, CSM_APPL_DATA) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) publicValueLengthPtr
)
{
  Std_ReturnType result = E_NOT_OK;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYEXCHANGECALCPUBVAL,
                          CSM_E_UNINIT
                         );
  }
  else if (keyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYEXCHANGECALCPUBVAL,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (publicValuePtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYEXCHANGECALCPUBVAL,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (publicValueLengthPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYEXCHANGECALCPUBVAL,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Csm_00506,1, SWS_Csm_01020,1 */
    result = CryIf_KeyExchangeCalcPubVal(Csm_Keys[keyId], publicValuePtr, publicValueLengthPtr);
  }
#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (result == CRYPTO_E_SMALL_BUFFER)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00830,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYEXCHANGECALCPUBVAL,
                          CRYPTO_E_SMALL_BUFFER
                         );
  }
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */

  return result;
}
#endif /* #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON) */

/* --- Csm_KeyExchangeCalcSecret ---------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00967,1, CSM.Req.Gen/CsmKeys/00002,1 */

#if (CSM_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcSecret
(
          uint32                            keyId,
  P2CONST(uint8,  AUTOMATIC, CSM_APPL_DATA) partnerPublicValuePtr,
          uint32                            partnerPublicValueLength
)
{
  Std_ReturnType result = E_NOT_OK;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYEXCHANGECALCSECRET,
                          CSM_E_UNINIT
                         );
  }
  else if (keyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYEXCHANGECALCSECRET,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (partnerPublicValuePtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYEXCHANGECALCSECRET,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Csm_00506,1, SWS_Csm_01006,1 */
    result = CryIf_KeyExchangeCalcSecret(Csm_Keys[keyId], partnerPublicValuePtr, partnerPublicValueLength);
  }
#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (result == CRYPTO_E_SMALL_BUFFER)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00830,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYEXCHANGECALCSECRET,
                          CRYPTO_E_SMALL_BUFFER
                         );
  }
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */

  return result;
}
#endif /* #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON) */

/* --- Csm_KeyGenerate -------------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00955,1, CSM.Req.Gen/CsmKeys/00002,1 */

#if (CSM_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyGenerate
(
  uint32 keyId
)
{
  Std_ReturnType result = E_NOT_OK;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYGENERATE,
                          CSM_E_UNINIT
                         );
  }
  else if (keyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYGENERATE,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Csm_00506,1, SWS_Csm_01005,1 */
    result = CryIf_KeyGenerate(Csm_Keys[keyId]);
  }

  return result;
}
#endif /* #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON) */

/* --- Csm_KeySetValid -------------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00958,1, CSM.Req.Gen/CsmKeys/00002,1 */

#if (CSM_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_KeySetValid
(
  uint32 keyId
)
{
  Std_ReturnType result = E_NOT_OK;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYSETVALID,
                          CSM_E_UNINIT
                         );
  }
  else if (keyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_KEYSETVALID,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Csm_00506,1, SWS_Csm_01003,1 */
    result = CryIf_KeySetValid(Csm_Keys[keyId]);
  }

  return result;
}
#endif /* #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON) */

/* --- Csm_MacGenerate -------------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00982,1 */

#if (CSM_API_ENABLED_SERVICE_MACGENERATE == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerate
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) dataPtr,
          uint32                                              dataLength,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) macPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) macLengthPtr
)
{
  Std_ReturnType                     result                  = E_NOT_OK;
  Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput = {
                                                                 NULL_PTR, 0U,                    /* inputPtr,           inputLength              */
                                                                 NULL_PTR, 0U,                    /* secondaryInputPtr,  secondaryInputLength     */
                                                                 NULL_PTR, 0U,                    /* tertiaryInputPtr,   tertiaryInputLength      */
                                                                 NULL_PTR, NULL_PTR,              /* outputPtr,          outputLengthPtr          */
                                                                 NULL_PTR, NULL_PTR,              /* secondaryOutputPtr, secondaryOutputLengthPtr */
#if (CSM_API_VERSION == CSM_API_VERSION_431)
                                                                 0U,                              /* input64                                      */
#endif /* (CSM_API_VERSION == CSM_API_VERSION_431) */
                                                                 NULL_PTR,                        /* verifyPtr                                    */
#if (CSM_API_VERSION != CSM_API_VERSION_EB)
                                                                 NULL_PTR,                        /* output64Ptr                                  */
#endif /* (CSM_API_VERSION != CSM_API_VERSION_EB) */
                                                                 CRYPTO_OPERATIONMODE_SINGLECALL  /* mode                                         */
                                                               };

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_MACGENERATE,
                          CSM_E_UNINIT
                         );
  }
  else if (jobId >= CSM_JOB_COUNT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00002,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_MACGENERATE,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (Csm_JobConfigurations[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_MACGENERATE)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00003,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_MACGENERATE,
                          CSM_E_SERVICE_NOT_IDENTICAL
                         );
  }
  else if (dataPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_MACGENERATE,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (macPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_MACGENERATE,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (macLengthPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_MACGENERATE,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    if ( (  (mode & CRYPTO_OPERATIONMODE_SINGLECALL) ==
              (CRYPTO_OPERATIONMODE_START|CRYPTO_OPERATIONMODE_FINISH)
         ) ||
         ( ((mode & CRYPTO_OPERATIONMODE_SINGLECALL) == 0U                             ) ||
           ( mode                                     > CRYPTO_OPERATIONMODE_SINGLECALL)
         )
       )
    {
      /* !LINKSTO SWS_Csm_01045,1, CSM.Req.Api/Csm_Service/00001,1, CSM.Req.Api/Csm_Service/00002,1 */
      result = E_NOT_OK;
    }
    else
    {
      /* !LINKSTO SWS_Csm_01017,1 */
      jobPrimitiveInputOutput.mode            = mode;
      jobPrimitiveInputOutput.inputPtr        = dataPtr;
      jobPrimitiveInputOutput.inputLength     = dataLength;
      jobPrimitiveInputOutput.outputPtr       = macPtr;
      jobPrimitiveInputOutput.outputLengthPtr = macLengthPtr;

      result = Csm_Process(jobId, &jobPrimitiveInputOutput);
    }
  }
#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (result == CRYPTO_E_SMALL_BUFFER)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00830,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_MACGENERATE,
                          CRYPTO_E_SMALL_BUFFER
                         );
  }
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */

  return result;
}
#endif /* #if (CSM_API_ENABLED_SERVICE_MACGENERATE == STD_ON) */

/* --- Csm_MacVerify ---------------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_01050,1 */

#if (CSM_API_ENABLED_SERVICE_MACVERIFY == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerify
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) dataPtr,
          uint32                                              dataLength,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) macPtr,
          uint32                                              macLength,
    P2VAR(Crypto_VerifyResultType,  AUTOMATIC, CSM_APPL_DATA) verifyPtr
)
{
  Std_ReturnType                     result                  = E_NOT_OK;
  Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput = {
                                                                 NULL_PTR, 0U,                    /* inputPtr,           inputLength              */
                                                                 NULL_PTR, 0U,                    /* secondaryInputPtr,  secondaryInputLength     */
                                                                 NULL_PTR, 0U,                    /* tertiaryInputPtr,   tertiaryInputLength      */
                                                                 NULL_PTR, NULL_PTR,              /* outputPtr,          outputLengthPtr          */
                                                                 NULL_PTR, NULL_PTR,              /* secondaryOutputPtr, secondaryOutputLengthPtr */
#if (CSM_API_VERSION == CSM_API_VERSION_431)
                                                                 0U,                              /* input64                                      */
#endif /* (CSM_API_VERSION == CSM_API_VERSION_431) */
                                                                 NULL_PTR,                        /* verifyPtr                                    */
#if (CSM_API_VERSION != CSM_API_VERSION_EB)
                                                                 NULL_PTR,                        /* output64Ptr                                  */
#endif /* (CSM_API_VERSION != CSM_API_VERSION_EB) */
                                                                 CRYPTO_OPERATIONMODE_SINGLECALL  /* mode                                         */
                                                               };

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_MACVERIFY,
                          CSM_E_UNINIT
                         );
  }
  else if (jobId >= CSM_JOB_COUNT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00002,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_MACVERIFY,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (Csm_JobConfigurations[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_MACVERIFY)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00003,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_MACVERIFY,
                          CSM_E_SERVICE_NOT_IDENTICAL
                         );
  }
  else if (dataPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_MACVERIFY,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (macPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_MACVERIFY,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (verifyPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_MACVERIFY,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    if ( (  (mode & CRYPTO_OPERATIONMODE_SINGLECALL) ==
              (CRYPTO_OPERATIONMODE_START|CRYPTO_OPERATIONMODE_FINISH)
         ) ||
         ( ((mode & CRYPTO_OPERATIONMODE_SINGLECALL) == 0U                             ) ||
           ( mode                                     > CRYPTO_OPERATIONMODE_SINGLECALL)
         )
       )
    {
      /* !LINKSTO SWS_Csm_01045,1, CSM.Req.Api/Csm_Service/00001,1, CSM.Req.Api/Csm_Service/00002,1 */
      result = E_NOT_OK;
    }
    else
    {
      /* !LINKSTO SWS_Csm_01016,1 */
      jobPrimitiveInputOutput.mode                 = mode;
      jobPrimitiveInputOutput.inputPtr             = dataPtr;
      jobPrimitiveInputOutput.inputLength          = dataLength;
      jobPrimitiveInputOutput.secondaryInputPtr    = macPtr;
      jobPrimitiveInputOutput.secondaryInputLength = macLength;
      jobPrimitiveInputOutput.verifyPtr            = verifyPtr;

      result = Csm_Process(jobId, &jobPrimitiveInputOutput);
    }
  }

  return result;
}
#endif /* #if (CSM_API_ENABLED_SERVICE_MACVERIFY == STD_ON) */

/* --- Csm_MainFunction ------------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00479,1 */
/* !LINKSTO SWS_Csm_00019,1 */

#if (CSM_API_ENABLED_SERVICE_ASYNCHRONOUS == STD_ON)
FUNC(void, CSM_CODE) Csm_MainFunction
(
  void
)
{
  Std_ReturnType  result = E_NOT_OK;
  Crypto_JobType *job    = NULL_PTR;
  boolean         run    = FALSE;
  uint32          q;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_MAINFUNCTION,
                          CSM_E_UNINIT
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* Deviation TASKING-2 */
    for (q=0U; q<CSM_QUE_COUNT; q++)
    {
      job = NULL_PTR;

      if (Csm_Queues[q] != NULL_PTR)
      {
        /* !LINKSTO SWS_Csm_00037,1 */
        if (Csm_Queues[q]->lockMainQueueDelJob == FALSE)
        {
          /* !LINKSTO CSM.Dsn.Api/ExAreas/00001,1, CSM.Dsn.Api/ExAreas/00002,1 */
          SchM_Enter_Csm_SCHM_CSM_EXCLUSIVE_AREA_0();
          if (Csm_Queues[q]->head != NULL_PTR)
          {
            job = Csm_Queues[q]->head->job;
          }
          if ((Csm_Queues[q]->lockMainProcessJob == FALSE) &&
              (job != NULL_PTR)
             )
          {
            Csm_Queues[q]->lockMainProcessJob = TRUE;
            run                                  = TRUE;
          }
          SchM_Exit_Csm_SCHM_CSM_EXCLUSIVE_AREA_0();

          if (run == TRUE)
          {
            /* !LINKSTO SWS_Csm_00506,1 */
            result = CryIf_ProcessJob(Csm_Queues[q]->cryIfChannelId, job);

            if (!((result == CRYPTO_E_BUSY) || (result == CRYPTO_E_QUEUE_FULL)))
            {
              /* !LINKSTO CSM.Dsn.Api/ExAreas/00001,1, CSM.Dsn.Api/ExAreas/00002,1 */
              SchM_Enter_Csm_SCHM_CSM_EXCLUSIVE_AREA_0();
              (void) Csm_QueueDelJob(Csm_Queues[q], job);
              Csm_JobExtraData[job->jobId].csmQueueLevel = CSM_QL_CRYPTO;
              SchM_Exit_Csm_SCHM_CSM_EXCLUSIVE_AREA_0();
            }

            Csm_Queues[q]->lockMainProcessJob = FALSE;
            run                                  = FALSE;
          }
        }

        /* !LINKSTO SWS_Csm_00037,1 */
        Csm_Queues[q]->lockMainQueueDelJob = FALSE;
      }
    }
  }
}
#endif /* #if (CSM_API_ENABLED_SERVICE_ASYNCHRONOUS == STD_ON) */

/* --- Csm_RandomGenerate ----------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_01543,1 */

#if (CSM_API_ENABLED_SERVICE_RANDOMGENERATE == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerate
(
        uint32                            jobId,
  P2VAR(uint8,  AUTOMATIC, CSM_APPL_DATA) resultPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_DATA) resultLengthPtr
)
{
  Std_ReturnType                     result                  = E_NOT_OK;
  Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput = {
                                                                 NULL_PTR, 0U,                    /* inputPtr,           inputLength              */
                                                                 NULL_PTR, 0U,                    /* secondaryInputPtr,  secondaryInputLength     */
                                                                 NULL_PTR, 0U,                    /* tertiaryInputPtr,   tertiaryInputLength      */
                                                                 NULL_PTR, NULL_PTR,              /* outputPtr,          outputLengthPtr          */
                                                                 NULL_PTR, NULL_PTR,              /* secondaryOutputPtr, secondaryOutputLengthPtr */
#if (CSM_API_VERSION == CSM_API_VERSION_431)
                                                                 0U,                              /* input64                                      */
#endif /* (CSM_API_VERSION == CSM_API_VERSION_431) */
                                                                 NULL_PTR,                        /* verifyPtr                                    */
#if (CSM_API_VERSION != CSM_API_VERSION_EB)
                                                                 NULL_PTR,                        /* output64Ptr                                  */
#endif /* (CSM_API_VERSION != CSM_API_VERSION_EB) */
                                                                 CRYPTO_OPERATIONMODE_SINGLECALL  /* mode                                         */
                                                               };

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_RANDOMGENERATE,
                          CSM_E_UNINIT
                         );
  }
  else if (jobId >= CSM_JOB_COUNT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00002,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_RANDOMGENERATE,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (Csm_JobConfigurations[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_RANDOMGENERATE)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00003,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_RANDOMGENERATE,
                          CSM_E_SERVICE_NOT_IDENTICAL
                         );
  }
  else if (resultPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_RANDOMGENERATE,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (resultLengthPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_RANDOMGENERATE,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    {
      /* !LINKSTO CSM.Req.Correction.SWS_Csm_01001,1 */
      jobPrimitiveInputOutput.outputPtr       = resultPtr;
      jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr;

      result = Csm_Process(jobId, &jobPrimitiveInputOutput);
    }
  }

  return result;
}
#endif /* #if (CSM_API_ENABLED_SERVICE_RANDOMGENERATE == STD_ON) */

/* --- Csm_RandomSeed --------------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_01051,1, CSM.Req.Gen/CsmKeys/00002,1 */

#if (CSM_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomSeed
(
          uint32                            keyId,
  P2CONST(uint8,  AUTOMATIC, CSM_APPL_DATA) seedPtr,
          uint32                            seedLength
)
{
  Std_ReturnType result = E_NOT_OK;

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_RANDOMSEED,
                          CSM_E_UNINIT
                         );
  }
  else if (keyId >= CSM_KEY_COUNT)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_RANDOMSEED,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (seedPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_RANDOMSEED,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_Csm_00506,1, SWS_Csm_01052,1 */
    result = CryIf_RandomSeed(Csm_Keys[keyId], seedPtr, seedLength);
  }

  return result;
}
#endif /* #if (CSM_API_ENABLED_KEYMNGMNT == STD_ON) */

/* --- Csm_SignatureGenerate -------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00992,1 */

#if (CSM_API_ENABLED_SERVICE_SIGNATUREGENERATE == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerate
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) dataPtr,
          uint32                                              dataLength,
    P2VAR(uint8,                    AUTOMATIC, CSM_APPL_DATA) resultPtr,
    P2VAR(uint32,                   AUTOMATIC, CSM_APPL_DATA) resultLengthPtr
)
{
  Std_ReturnType                     result                  = E_NOT_OK;
  Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput = {
                                                                 NULL_PTR, 0U,                    /* inputPtr,           inputLength              */
                                                                 NULL_PTR, 0U,                    /* secondaryInputPtr,  secondaryInputLength     */
                                                                 NULL_PTR, 0U,                    /* tertiaryInputPtr,   tertiaryInputLength      */
                                                                 NULL_PTR, NULL_PTR,              /* outputPtr,          outputLengthPtr          */
                                                                 NULL_PTR, NULL_PTR,              /* secondaryOutputPtr, secondaryOutputLengthPtr */
#if (CSM_API_VERSION == CSM_API_VERSION_431)
                                                                 0U,                              /* input64                                      */
#endif /* (CSM_API_VERSION == CSM_API_VERSION_431) */
                                                                 NULL_PTR,                        /* verifyPtr                                    */
#if (CSM_API_VERSION != CSM_API_VERSION_EB)
                                                                 NULL_PTR,                        /* output64Ptr                                  */
#endif /* (CSM_API_VERSION != CSM_API_VERSION_EB) */
                                                                 CRYPTO_OPERATIONMODE_SINGLECALL  /* mode                                         */
                                                               };

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_SIGNATUREGENERATE,
                          CSM_E_UNINIT
                         );
  }
  else if (jobId >= CSM_JOB_COUNT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00002,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_SIGNATUREGENERATE,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (Csm_JobConfigurations[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_SIGNATUREGENERATE)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00003,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_SIGNATUREGENERATE,
                          CSM_E_SERVICE_NOT_IDENTICAL
                         );
  }
  else if (dataPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_SIGNATUREGENERATE,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (resultPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_SIGNATUREGENERATE,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (resultLengthPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_SIGNATUREGENERATE,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    if ( (  (mode & CRYPTO_OPERATIONMODE_SINGLECALL) ==
              (CRYPTO_OPERATIONMODE_START|CRYPTO_OPERATIONMODE_FINISH)
         ) ||
         ( ((mode & CRYPTO_OPERATIONMODE_SINGLECALL) == 0U                             ) ||
           ( mode                                     > CRYPTO_OPERATIONMODE_SINGLECALL)
         )
       )
    {
      /* !LINKSTO SWS_Csm_01045,1, CSM.Req.Api/Csm_Service/00001,1, CSM.Req.Api/Csm_Service/00002,1 */
      result = E_NOT_OK;
    }
    else
    {
      /* !LINKSTO SWS_Csm_00993,1 */
      jobPrimitiveInputOutput.mode            = mode;
      jobPrimitiveInputOutput.inputPtr        = dataPtr;
      jobPrimitiveInputOutput.inputLength     = dataLength;
      jobPrimitiveInputOutput.outputPtr       = resultPtr;
      jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr;

      result = Csm_Process(jobId, &jobPrimitiveInputOutput);
    }
  }
#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (result == CRYPTO_E_SMALL_BUFFER)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00830,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_SIGNATUREGENERATE,
                          CRYPTO_E_SMALL_BUFFER
                         );
  }
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */

  return result;
}
#endif /* #if (CSM_API_ENABLED_SERVICE_SIGNATUREGENERATE == STD_ON) */

/* --- Csm_SignatureVerify ---------------------------------------------------------------------- */

/* !LINKSTO CSM.Dsn.SWS_Csm_00996,1 */

#if (CSM_API_ENABLED_SERVICE_SIGNATUREVERIFY == STD_ON)
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerify
(
          uint32                                              jobId,
          Crypto_OperationModeType                            mode,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) dataPtr,
          uint32                                              dataLength,
  P2CONST(uint8,                    AUTOMATIC, CSM_APPL_DATA) signaturePtr,
          uint32                                              signatureLength,
    P2VAR(Crypto_VerifyResultType,  AUTOMATIC, CSM_APPL_DATA) verifyPtr
)
{
  Std_ReturnType                     result                  = E_NOT_OK;
  Crypto_JobPrimitiveInputOutputType jobPrimitiveInputOutput = {
                                                                 NULL_PTR, 0U,                    /* inputPtr,           inputLength              */
                                                                 NULL_PTR, 0U,                    /* secondaryInputPtr,  secondaryInputLength     */
                                                                 NULL_PTR, 0U,                    /* tertiaryInputPtr,   tertiaryInputLength      */
                                                                 NULL_PTR, NULL_PTR,              /* outputPtr,          outputLengthPtr          */
                                                                 NULL_PTR, NULL_PTR,              /* secondaryOutputPtr, secondaryOutputLengthPtr */
#if (CSM_API_VERSION == CSM_API_VERSION_431)
                                                                 0U,                              /* input64                                      */
#endif /* (CSM_API_VERSION == CSM_API_VERSION_431) */
                                                                 NULL_PTR,                        /* verifyPtr                                    */
#if (CSM_API_VERSION != CSM_API_VERSION_EB)
                                                                 NULL_PTR,                        /* output64Ptr                                  */
#endif /* (CSM_API_VERSION != CSM_API_VERSION_EB) */
                                                                 CRYPTO_OPERATIONMODE_SINGLECALL  /* mode                                         */
                                                               };

#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!Csm_Initialized)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00489,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_SIGNATUREVERIFY,
                          CSM_E_UNINIT
                         );
  }
  else if (jobId >= CSM_JOB_COUNT)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00002,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_SIGNATUREVERIFY,
                          CSM_E_PARAM_HANDLE
                         );
  }
  else if (Csm_JobConfigurations[jobId].jobPrimitiveInfo->primitiveInfo->service != CRYPTO_SIGNATUREVERIFY)
  {
    /* !LINKSTO CSM.Req.Gen/Det/00003,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_SIGNATUREVERIFY,
                          CSM_E_SERVICE_NOT_IDENTICAL
                         );
  }
  else if (dataPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_SIGNATUREVERIFY,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (signaturePtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_SIGNATUREVERIFY,
                          CSM_E_PARAM_POINTER
                         );
  }
  else if (verifyPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00539,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_SIGNATUREVERIFY,
                          CSM_E_PARAM_POINTER
                         );
  }
  else
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    if ( (  (mode & CRYPTO_OPERATIONMODE_SINGLECALL) ==
              (CRYPTO_OPERATIONMODE_START|CRYPTO_OPERATIONMODE_FINISH)
         ) ||
         ( ((mode & CRYPTO_OPERATIONMODE_SINGLECALL) == 0U                             ) ||
           ( mode                                     > CRYPTO_OPERATIONMODE_SINGLECALL)
         )
       )
    {
      /* !LINKSTO SWS_Csm_01045,1, CSM.Req.Api/Csm_Service/00001,1, CSM.Req.Api/Csm_Service/00002,1 */
      result = E_NOT_OK;
    }
    else
    {
      /* !LINKSTO SWS_Csm_00997,1 */
      jobPrimitiveInputOutput.mode                 = mode;
      jobPrimitiveInputOutput.inputPtr             = dataPtr;
      jobPrimitiveInputOutput.inputLength          = dataLength;
      jobPrimitiveInputOutput.secondaryInputPtr    = signaturePtr;
      jobPrimitiveInputOutput.secondaryInputLength = signatureLength;
      jobPrimitiveInputOutput.verifyPtr            = verifyPtr;

      result = Csm_Process(jobId, &jobPrimitiveInputOutput);
    }
  }
#if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (result == CRYPTO_E_SMALL_BUFFER)
  {
    /* !LINKSTO SWS_Csm_00485,1, SWS_Csm_00830,1 */
    (void)Det_ReportError(CSM_MODULE_ID,
                          CSM_INSTANCE_ID,
                          CSM_SID_SIGNATUREVERIFY,
                          CRYPTO_E_SMALL_BUFFER
                         );
  }
#endif /* #if (CSM_API_ENABLED_DEVERRORDETECT == STD_ON) */

  return result;
}
#endif /* #if (CSM_API_ENABLED_SERVICE_SIGNATUREVERIFY == STD_ON) */

#define CSM_STOP_SEC_CODE
#include <Csm_MemMap.h>

/*==================[internal function definitions]===============================================*/

#define CSM_START_SEC_CODE
#include <Csm_MemMap.h>

FUNC(Std_ReturnType, CSM_CODE) Csm_Process
(
  uint32 jobId,
  P2VAR(Crypto_JobPrimitiveInputOutputType, AUTOMATIC, CSM_APPL_DATA) jobPrimitiveInputOutput
)
{
  Std_ReturnType result = E_NOT_OK;

  if ( (Csm_JobConfigurations[jobId].CSM_API_JOBSTATE                == CRYPTO_JOBSTATE_ACTIVE) &&
       ((jobPrimitiveInputOutput->mode & CRYPTO_OPERATIONMODE_START) != 0U                    )
     )
  {
    /* !LINKSTO SWS_Csm_00017,1 */
    result = CRYPTO_E_BUSY;
  }
  else
  if (Csm_JobConfigurations[jobId].CSM_API_JOBSTATE == CRYPTO_JOBSTATE_ACTIVE)
  {
    Csm_JobConfigurations[jobId].CSM_API_JOBPRIMITIVEINPUTOUTPUT = *jobPrimitiveInputOutput;
    /* !LINKSTO SWS_Csm_00506,1 */
    result = CryIf_ProcessJob(Csm_JobExtraCnfg[jobId].csmQueue->cryIfChannelId,
                              &(Csm_JobConfigurations[jobId])
                             );
  }
  else
  {
    if (Csm_JobConfigurations[jobId].jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_ASYNC)
    {
      if ( ((jobPrimitiveInputOutput->mode & CRYPTO_OPERATIONMODE_START) != 0U) &&
           ((Csm_JobExtraData[jobId].csmQueueLevel == CSM_QL_NONE))
         )
      {
        if (Csm_JobExtraCnfg[jobId].csmQueue->currentSize
              >= Csm_JobExtraCnfg[jobId].csmQueue->maxSize
           )
        {
          result = CRYPTO_E_QUEUE_FULL;
        }
        else
        if (Csm_JobExtraCnfg[jobId].csmQueue->currentSize == 0U)
        {
          Csm_JobConfigurations[jobId].CSM_API_JOBPRIMITIVEINPUTOUTPUT = *jobPrimitiveInputOutput;
          /* !LINKSTO SWS_Csm_00506,1 */
          result = CryIf_ProcessJob(Csm_JobExtraCnfg[jobId].csmQueue->cryIfChannelId,
                                    &(Csm_JobConfigurations[jobId])
                                   );
          if (result == E_OK)
          {
            Csm_JobExtraData[jobId].csmQueueLevel = CSM_QL_CRYPTO;
          }
          else
          if ((result == CRYPTO_E_BUSY) || (result == CRYPTO_E_QUEUE_FULL))
          {
            /* !LINKSTO CSM.Dsn.Api/ExAreas/00001,1, CSM.Dsn.Api/ExAreas/00002,1 */
            SchM_Enter_Csm_SCHM_CSM_EXCLUSIVE_AREA_0();
            /* !LINKSTO CSM.Req.Gen/Queueing/00001,1 */
            result = Csm_QueuePushJob(Csm_JobExtraCnfg[jobId].csmQueue,
                                         &Csm_JobConfigurations[jobId]
                                        );
            /* CHECK: NOPARSE */
            /* Defensive programming - unreachable code.
             * 1. Csm_QueuePushJob never returns E_NOT_OK because it is ensured that no NULL_PTRs
             *    are passed.
             * 2. Csm_QueuePushJob never returns CRYPTO_E_QUEUE_FULL because this is already checked
             *    in the leading IF of the parental IF-ELSE structure.
             */
            if ( result == E_OK )
            /* CHECK: PARSE */
            {
              Csm_JobExtraData[jobId].csmQueueLevel = CSM_QL_CSM;
            }
            SchM_Exit_Csm_SCHM_CSM_EXCLUSIVE_AREA_0();
          }
          else
          {
            /* intentionally empty */
          }
        }
        else
        {
          Csm_JobConfigurations[jobId].CSM_API_JOBPRIMITIVEINPUTOUTPUT = *jobPrimitiveInputOutput;
          /* !LINKSTO CSM.Dsn.Api/ExAreas/00001,1, CSM.Dsn.Api/ExAreas/00002,1 */
          SchM_Enter_Csm_SCHM_CSM_EXCLUSIVE_AREA_0();
          /* !LINKSTO CSM.Req.Gen/Queueing/00001,1 */
          result = Csm_QueuePushJob(Csm_JobExtraCnfg[jobId].csmQueue,
                                       &Csm_JobConfigurations[jobId]
                                      );
          /* CHECK: NOPARSE */
          /* Defensive programming - unreachable code.
           * 1. Csm_QueuePushJob never returns E_NOT_OK because it is ensured that no NULL_PTRs
           *    are passed.
           * 2. Csm_QueuePushJob never returns CRYPTO_E_QUEUE_FULL because this is already checked
           *    in the leading IF of this IF-ELSE structure.
           */
          if (result == E_OK)
          /* CHECK: PARSE */
          {
            Csm_JobExtraData[jobId].csmQueueLevel = CSM_QL_CSM;
          }
          SchM_Exit_Csm_SCHM_CSM_EXCLUSIVE_AREA_0();
        }
      }
      else
      if ( ((jobPrimitiveInputOutput->mode & CRYPTO_OPERATIONMODE_START) == 0U) &&
           ((Csm_JobExtraData[jobId].csmQueueLevel != CSM_QL_NONE))
         )
      {
        /* !LINKSTO CSM.Req.Gen/Queueing/00004,1 */
        result = CRYPTO_E_BUSY;
      }
      else
      {
        /* !LINKSTO CSM.Req.Gen/Queueing/00002,1 */
        /* intentionally empty */
      }
    }
    else
    {
      if ((Csm_JobExtraCnfg[jobId].csmQueue->head == NULL_PTR) ||
          (Csm_JobConfigurations[jobId].jobInfo->jobPriority
             > Csm_JobExtraCnfg[jobId].csmQueue->head->job->jobInfo->jobPriority
          )
         )
      {
        /* !LINKSTO SWS_Csm_00037,1 */
        Csm_JobExtraCnfg[jobId].csmQueue->lockMainQueueDelJob = TRUE;

        Csm_JobConfigurations[jobId].CSM_API_JOBPRIMITIVEINPUTOUTPUT = *jobPrimitiveInputOutput;
        /* !LINKSTO SWS_Csm_00506,1 */
        result = CryIf_ProcessJob(Csm_JobExtraCnfg[jobId].csmQueue->cryIfChannelId,
                                  &(Csm_JobConfigurations[jobId])
                                 );
      }
      else
      {
        /* !LINKSTO SWS_Csm_91007,1 */
        result = CRYPTO_E_BUSY;
      }
    }
  }

  return result;
}

#define CSM_STOP_SEC_CODE
#include <Csm_MemMap.h>

/*==================[end of file]=================================================================*/

