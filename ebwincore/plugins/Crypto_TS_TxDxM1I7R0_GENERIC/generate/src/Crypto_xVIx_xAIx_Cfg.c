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

[!AUTOSPACING!]
[!/* ------------------------------------------------------------------ */!][!//
[!VAR "PL_MainFunction" = "'false'"!][!//
[!IF "num:i(0) != num:i(count(CryptoDriverObjects/CryptoDriverObject/*/CryptoPrimitiveRef/*))"!][!//
  [!VAR "PL_MainFunction" = "'true'"!][!//
[!ENDIF!][!//
[!/* ------------------------------------------------------------------ */!][!//

/*==================[misra deviations]============================================================*/

/*==================[inclusions]==================================================================*/

#include <Crypto_xVIx_xAIx.h>
#include <Crypto_xVIx_xAIx_Int.h>
#include <Crypto_xVIx_xAIx_AL.h>
#include <Crypto_xVIx_xAIx_Cfg.h>
#include <TSAutosar.h>
#include <SchM_Crypto_xVIx_xAIx.h>

/*==================[macros]======================================================================*/

/*==================[declaration of types]========================================================*/

/*==================[declaration of internal functions]===========================================*/

/*==================[definition of internal constants]============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

[!LOOP "node:order(CryptoKeys/CryptoKey/*, 'node:value(CryptoKeyId)')"!]
  [!VAR "CryptoKeyId" = "node:value(CryptoKeyId)"!]
  [!SELECT "./CryptoKeyTypeRef"!]
/**
 * \brief Key Type for crypto key [!"node:name(.)"!].
 * Contains references to the key elements used by the key
 */
static CONSTP2VAR(Crypto_xVIx_xAIx_KeyElement, AUTOMATIC, AUTOMATIC) Crypto_xVIx_xAIx_KeyType[!"$CryptoKeyId"!][[!"num:i(count(node:ref(.)/CryptoKeyElementRef/*))"!]] =
{
    [!LOOP "node:ref(.)/CryptoKeyElementRef/*"!]
      [!SELECT "node:current()"!]
    &Crypto_xVIx_xAIx_KeyElements
     [CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(as:ref(.)))"!]_INDEX],
      [!ENDSELECT!]
  [!ENDLOOP!]
};
  [!ENDSELECT!]
[!ENDLOOP!]

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==================[definition of internal variables]============================================*/

/*==================[definition of external constants]============================================*/

/*==================[definition of external variables]============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

[!VAR "i"  = "0"!]
[!LOOP "node:order(CryptoDriverObjects/CryptoDriverObject/*,'node:value(CryptoDriverObjectId)')"!]
  [!IF "CryptoQueueSize > 0"!]
/** \brief Data Array for the queue of crypto driver [!"node:name(.)"!] */
VAR(Crypto_xVIx_xAIx_QueueElementType, AUTOMATIC) Crypto_xVIx_xAIx_QueueData[!"num:i($i)"!][CRYPTO_XVIX_XAIX_DO_[!"text:toupper(node:name(.))"!]_QUEUE_SIZE];

  [!ENDIF!]
  [!VAR "i" = "$i + 1"!]
[!ENDLOOP!]

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/**
 * \brief Data structure to store the queue information for each crypto driver object
 */

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
VAR(Crypto_xVIx_xAIx_QueueType, AUTOMATIC)
    Crypto_xVIx_xAIx_Queues[CRYPTO_XVIX_XAIX_DRIVER_OBJECT_COUNT] =
{
  [!VAR "i"  = "0"!]
  [!LOOP "node:order(CryptoDriverObjects/CryptoDriverObject/*,'node:value(CryptoDriverObjectId)')"!]
  {
    0U,
    CRYPTO_XVIX_XAIX_DO_[!"text:toupper(node:name(.))"!]_QUEUE_SIZE,
    NULL_PTR,
    [!IF "CryptoQueueSize > 0"!]
    Crypto_xVIx_xAIx_QueueData[!"num:i($i)"!]
    [!ELSE!][!//
    NULL_PTR
    [!ENDIF!]
  },
    [!VAR "i" = "$i + 1"!]
  [!ENDLOOP!]
};
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */

/**
 * \brief Data structure to store the configured crypto key elements
 */
VAR(Crypto_xVIx_xAIx_KeyElement, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_KeyElements[CRYPTO_XVIX_XAIX_KEY_ELEMENT_COUNT] =
{
  [!LOOP "CryptoKeyElements/CryptoKeyElement/*"!]
  {
    Crypto_xVIx_xAIxConf_CryptoKeyElement_[!"node:name(.)"!],
    CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_ALLOW_PARTIAL_ACCESS,
    Crypto_xVIx_xAIx_[!"text:toupper(node:name(.))"!]_INIT_VALUE,
    CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_PERSIST,
    CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_READ_ACCESS,
    CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_SIZE,
    CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_WRITE_ACCESS
  },
  [!ENDLOOP!]
};

/* !LINKSTO EB_Crypto_01018,1 */
#if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON)
/**
 * \brief Data structure to store the configured crypto keys
 */
VAR(Crypto_xVIx_xAIx_Key, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_Keys[CRYPTO_XVIX_XAIX_KEY_COUNT] =
{
  [!LOOP "node:order(CryptoKeys/CryptoKey/*, 'node:value(CryptoKeyId)')"!]
    [!VAR "CryptoKeyId"   = "node:value(CryptoKeyId)"!]
    [!VAR "CryptoKeyName" = "node:name(.)"!]     [!SELECT "./CryptoKeyTypeRef"!]
  {
    CRYPTO_XVIX_XAIX_KEY_[!"text:toupper($CryptoKeyName)"!]_DERIVE_ITERATIONS,
    [!"num:i(count(as:ref(.)/CryptoKeyElementRef/*))"!],
    Crypto_xVIx_xAIx_KeyType[!"$CryptoKeyId"!],
    CRYPTO_XVIX_XAIX_KEY_STATE_INVALID   },
    [!ENDSELECT!]
  [!ENDLOOP!]
};
#endif /* #if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO SWS_Crypto_00019, 1 */
/**
 * \brief Data structure to store the configured crypto driver objects
 */
VAR(Crypto_xVIx_xAIx_DriverObject, CRYPTO_xVIx_xAIx_VAR)
    Crypto_xVIx_xAIx_DriverObjects[CRYPTO_XVIX_XAIX_DRIVER_OBJECT_COUNT] =
{
  [!VAR "i"  = "0"!]
  [!LOOP "node:order(CryptoDriverObjects/CryptoDriverObject/*,'node:value(CryptoDriverObjectId)')"!]
  {

    TRUE,
    CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE,
    NULL_PTR,
#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
    &Crypto_xVIx_xAIx_Queues[[!"num:i($i)"!]]
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */
  },
    [!VAR "i" = "$i + 1"!]
  [!ENDLOOP!]
};

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==================[definition of external constants]============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

[!LOOP "CryptoKeyElements/CryptoKeyElement/*"!]
CONST(uint8, CRYPTO_xVIx_xAIx_CONST)
      Crypto_xVIx_xAIx_[!"text:toupper(node:name(.))"!]_INIT_VALUE
      [CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_SIZE]
      = CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_INIT_VALUE;
[!ENDLOOP!]

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==================[definition of external variables]============================================*/

/*==================[definition of external functions]============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>
[!VAR "RsaPssVrfyFound" = "0"!]
[!VAR "RsaPkcsVrfyFound" = "0"!]
[!VAR "RsaPkcsGenFound" = "0"!]
[!VAR "RsaesOaepEncFound" = "0"!]
[!VAR "RsaesOaepDecFound" = "0"!]
[!LOOP "CryptoDriverObjects/CryptoDriverObject/*/CryptoPrimitiveRef/*"!]
  [!IF "as:ref(.)/CryptoPrimitiveService = 'MAC_GENERATE'"!]
    [!VAR "Service" = "'MACGENERATE'"!]
    [!VAR "Output" = "'F'"!]
  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'MAC_VERIFY'"!]
    [!VAR "Service" = "'MACVERIFY'"!]
    [!VAR "Output" = "'None'"!]
  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'DECRYPT'"!]
    [!VAR "Service" = "'DECRYPT'"!]
    [!VAR "Output" = "'UF'"!]
  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'ENCRYPT'"!]
    [!VAR "Service" = "'ENCRYPT'"!]
    [!VAR "Output" = "'UF'"!]
  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'RANDOM'"!]
    [!VAR "Service" = "'RANDOMGENERATE'"!]
    [!VAR "Output" = "'F'"!]

  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'AEAD_DECRYPT'"!]
    [!VAR "Service" = "'AEADDECRYPT'"!]
    [!VAR "Output" = "'UF'"!]

  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'AEAD_ENCRYPT'"!]
    [!VAR "Service" = "'AEADENCRYPT'"!]
    [!VAR "Output" = "'UF'"!]

  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'HASH'"!]
    [!VAR "Service" = "'HASH'"!]
     [!VAR "Output" = "'F'"!]

  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'SIGNATURE_GENERATE'"!]
    [!VAR "Service" = "'SIGNATUREGENERATE'"!]
    [!VAR "Output" = "'F'"!]

  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'SIGNATURE_VERIFY'"!]
    [!VAR "Service" = "'SIGNATUREVERIFY'"!]
    [!VAR "Output" = "'None'"!]
  [!ENDIF!]
  [!LOOP "as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*"!]
    [!VAR "Family" = "node:value(.)"!]
    [!LOOP "../../CryptoPrimitiveAlgorithmMode/*"!]
      [!VAR "Mode" = "node:value(.)"!]

      [!IF "($Service = 'SIGNATUREVERIFY' and $Mode = 'CRYPTO_ALGOMODE_RSASSA_PSS' and $RsaPssVrfyFound = 1) or
            ($Service = 'SIGNATUREVERIFY' and $Mode = 'CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5' and $RsaPkcsVrfyFound = 1) or
            ($Service = 'SIGNATUREGENERATE' and $Mode = 'CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5' and $RsaPkcsGenFound = 1) or
            ($Service = 'ENCRYPT' and $Mode = 'CRYPTO_ALGOMODE_RSAES_OAEP' and $RsaesOaepEncFound = 1) or
            ($Service = 'DECRYPT' and $Mode = 'CRYPTO_ALGOMODE_RSAES_OAEP' and $RsaesOaepDecFound = 1)"!][!//
        [!BREAK!]
      [!ENDIF!]
      [!IF "$Service = 'SIGNATUREVERIFY' and $Mode = 'CRYPTO_ALGOMODE_RSASSA_PSS' and $RsaPssVrfyFound = 0"!]
        [!VAR "RsaPssVrfyFound" = "1"!]
      [!ELSEIF "$Service = 'SIGNATUREVERIFY' and $Mode = 'CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5' and $RsaPkcsVrfyFound = 0"!]
        [!VAR "RsaPkcsVrfyFound" = "1"!]
      [!ELSEIF "$Service = 'SIGNATUREGENERATE' and $Mode = 'CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5' and $RsaPkcsGenFound = 0"!]
        [!VAR "RsaPkcsGenFound" = "1"!]
      [!ELSEIF "$Service = 'ENCRYPT' and $Mode = 'CRYPTO_ALGOMODE_RSAES_OAEP' and $RsaesOaepEncFound = 0"!]
        [!VAR "RsaesOaepEncFound" = "1"!]
      [!ELSEIF "$Service = 'DECRYPT' and $Mode = 'CRYPTO_ALGOMODE_RSAES_OAEP' and $RsaesOaepDecFound = 0"!]
        [!VAR "RsaesOaepDecFound" = "1"!]
      [!ENDIF!]
/* !LINKSTO EB_Crypto_00024, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_Process
(
  uint32 ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
        [!IF "$Service != 'HASH'"!]
#if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON)
        [!ENDIF!]
  /* RetVal is intentional initialized with E_OK it has to be ensured that the Operation mode is
     always valid */
  Std_ReturnType RetVal = E_OK;

        [!IF "($Service != 'RANDOMGENERATE')"!]
  boolean Busy = TRUE;

        [!ENDIF!]
  Crypto_xVIx_xAIx_DriverObject* DriverObj = &Crypto_xVIx_xAIx_DriverObjects[ObjectId];
        [!IF "contains($Output, 'U')"!]
  /* !LINKSTO EB_Crypto_01030, 1 */
  uint32 CurrOutputLength = 0U;
  uint32 StartOutputLength = 0U;

  if (
       (CRYPTO_PROCESSING_SYNC == Job->jobPrimitiveInfo->processingType) &&
       (
         ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) ==
         (
          ((uint8) Job->jobPrimitiveInputOutput.mode) &
          ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL )
         )
       )
     )
  {
    StartOutputLength = *(Job->jobPrimitiveInputOutput.outputLengthPtr);
  }
        [!ENDIF!]

  /* !LINKSTO EB_Crypto_00027, 1 */

        [!IF "($Service = 'MACGENERATE' or $Service = 'MACVERIFY' or $Service = 'RANDOMGENERATE') and ($Mode != 'CRYPTO_ALGOMODE_HMAC')"!]
  if (

          [!IF "($Service != 'RANDOMGENERATE')"!]
      (CRYPTO_PROCESSING_SYNC == Job->jobPrimitiveInfo->processingType) &&

          [!ENDIF!]
      (
       ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) ==
       (((uint8) Job->jobPrimitiveInputOutput.mode) & ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL))
      )
     )
  {
    /* !LINKSTO EB_Crypto_00028, 1 */
    /* Perform synchronous singlecall operation and reset the driver object and Job afterwards */
    RetVal = Crypto_xVIx_xAIx_AL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_SyncSingleCall(ObjectId, Job);

      if ((RetVal == CRYPTO_E_KEY_NOT_AVAILABLE) || (RetVal == CRYPTO_E_KEY_READ_FAIL))
      {
        RetVal = E_NOT_OK;
      }

          [!IF "($Service = 'RANDOMGENERATE')"!]
    if (CRYPTO_PROCESSING_SYNC == Job->jobPrimitiveInfo->processingType)

          [!ENDIF!]
    {
      /* !LINKSTO EB_Crypto_01050, 1 */
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      DriverObj->DriverObjectState = CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      DriverObj->CurrentJob = NULL_PTR;

      DriverObj->SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
    }
  }
  else

        [!ENDIF!]
  {

        [!IF "($Service = 'RANDOMGENERATE')"!]
    {
      /* !LINKSTO EB_Crypto_01095, 1 */
      RetVal = E_NOT_OK;
    }
        [!ELSE!]
    if (
         /* !LINKSTO SWS_Crypto_00020, 1 */
         ((uint8) CRYPTO_OPERATIONMODE_START) ==
         (((uint8) Job->jobPrimitiveInputOutput.mode) & ((uint8) CRYPTO_OPERATIONMODE_START))
       )
    {
      /* !LINKSTO EB_Crypto_00029, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.InitialActive2Start, 1 */
      /* Perform start operation */
      RetVal = Crypto_xVIx_xAIx_AL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_Start(ObjectId, Job);

      if ((RetVal == CRYPTO_E_KEY_NOT_AVAILABLE) || (RetVal == CRYPTO_E_KEY_READ_FAIL))
      {
        RetVal = E_NOT_OK;
      }
[!/* This is intentionally not indented because AUTOSPACING does not work with Multiline IF */!][!//
[!IF "(
        ($Service != 'ENCRYPT' or $Family != 'CRYPTO_ALGOFAM_AES' or $Mode != 'CRYPTO_ALGOMODE_CFB') and
        ($Service != 'DECRYPT' or $Family != 'CRYPTO_ALGOFAM_AES' or $Mode != 'CRYPTO_ALGOMODE_CFB')
      )"!]
      /* !LINKSTO SWS_Crypto_00027, 1 */
      if ((CRYPTO_PROCESSING_SYNC == Job->jobPrimitiveInfo->processingType) &&
        /* !LINKSTO SWS_Crypto_00026, 1,
                    EB_Crypto_00032, 1,
                    EB_Crypto_00033, 1
        */
          (E_OK == RetVal))
      {
        /* Wait for result if request is synchronous */
        do
        {
          Crypto_xVIx_xAIx_AL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_MainFunction(&RetVal, &Busy);
        }
        while (CRYPTO_XVIX_XAIX_DRIVER_OBJECT_ACTIVE == Busy);
      }
          [!ENDIF!]
    }
    if (
        (E_OK == RetVal) &&
        (
         ((uint8) CRYPTO_OPERATIONMODE_UPDATE) ==
         (((uint8) Job->jobPrimitiveInputOutput.mode) & ((uint8) CRYPTO_OPERATIONMODE_UPDATE))
        )
       )
    {

      /* !LINKSTO EB_Crypto_00030, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.Start2Update, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.Update2Update, 1 */
      /* Perform update operation. Keep check on Async Single call and Async StreamStart. */
      if (
          (CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
 &&
          (
           ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART) !=
           (
            ((uint8) Job->jobPrimitiveInputOutput.mode) &
            ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
           )
          ) &&
          (
           ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) !=
           (((uint8) Job->jobPrimitiveInputOutput.mode) & ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL))
          )
         )
      {
        RetVal = Crypto_xVIx_xAIx_AL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_Update(ObjectId, Job);
      }
      /* !LINKSTO SWS_Crypto_00027, 1 */
      if (CRYPTO_PROCESSING_SYNC == Job->jobPrimitiveInfo->processingType)
      {
        /* Perform update operation in case of Sync Processing type*/
        RetVal = Crypto_xVIx_xAIx_AL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_Update(ObjectId, Job);
        /* !LINKSTO SWS_Crypto_00026, 1,
                    EB_Crypto_00032, 1,
                    EB_Crypto_00033, 1
        */
        if (E_OK == RetVal)
        {
          /* Wait for result if request is synchronous */
          do
          {
            Crypto_xVIx_xAIx_AL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_MainFunction(&RetVal, &Busy);
          }
          while (CRYPTO_XVIX_XAIX_DRIVER_OBJECT_ACTIVE == Busy);
        }
          [!IF "contains($Output, 'U')"!]
        /* !LINKSTO EB_Crypto_01030, 1 */
        CurrOutputLength += *(Job->jobPrimitiveInputOutput.outputLengthPtr);
          [!ENDIF!]
      }
    }
    if (
        (E_OK == RetVal) &&
        (
         ((uint8) CRYPTO_OPERATIONMODE_FINISH) ==
         (((uint8) Job->jobPrimitiveInputOutput.mode) & ((uint8) CRYPTO_OPERATIONMODE_FINISH))
        )
       )
    {

      /* !LINKSTO EB_Crypto_00031, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.Update2Finish, 1 */
      /* Perform finish operation */
      if (
          (CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
 &&
          (
           ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART) !=
           (
            ((uint8) Job->jobPrimitiveInputOutput.mode) &
            ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
           )
          ) &&
          (
           ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) !=
           (
            ((uint8) Job->jobPrimitiveInputOutput.mode) &
            ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL )
           )
          )
         )
      {
        RetVal = Crypto_xVIx_xAIx_AL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_Finish(ObjectId, Job);
      }
      /* !LINKSTO SWS_Crypto_00027, 1 */
      if (CRYPTO_PROCESSING_SYNC == Job->jobPrimitiveInfo->processingType)
      {
          [!IF "contains($Output, 'U')"!]
        if (
            ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) ==
            (
             ((uint8) Job->jobPrimitiveInputOutput.mode) &
             ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL )
            )
           )
        {
          RetVal = Crypto_xVIx_xAIx_PL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_Process_SyncUpdate([!IF "contains($Output, 'U')"!]StartOutputLength, [!ENDIF!]ObjectId, Busy, RetVal, Job);
        }
        else
          [!ENDIF!]
        {
          RetVal = Crypto_xVIx_xAIx_PL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_Process_SyncUpdate([!IF "contains($Output, 'U')"!]*(Job->jobPrimitiveInputOutput.outputLengthPtr), [!ENDIF!]ObjectId, Busy, RetVal, Job);
        }
          [!IF "contains($Output, 'U')"!]
        /* !LINKSTO EB_Crypto_01030, 1 */
        CurrOutputLength += *(Job->jobPrimitiveInputOutput.outputLengthPtr);
          [!ENDIF!]
        /* Reset driver object and job after finish operation was completed */
        /* !LINKSTO EB_Crypto_01050, 1 */
        SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
        DriverObj->DriverObjectState = CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;

        DriverObj->SkipPeriodicMainFunction = TRUE;
        DriverObj->CurrentJob = NULL_PTR;
        SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
        /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
        Job->jobState = CRYPTO_JOBSTATE_IDLE;
      }
    }
          [!IF "contains($Output, 'U')"!]
    if ((CRYPTO_PROCESSING_SYNC == Job->jobPrimitiveInfo->processingType) &&
        (0U != (((uint8) ~CRYPTO_OPERATIONMODE_START) &
                ((uint8) Job->jobPrimitiveInputOutput.mode)
               )
        )
       )
    {
      /* !LINKSTO EB_Crypto_01030, 1 */
      *(Job->jobPrimitiveInputOutput.outputLengthPtr) = CurrOutputLength;
    }
          [!ENDIF!]

        [!ENDIF!]
  }

  if (E_OK != RetVal)
  {
    /* If an error occured, reset the driver object and the Job */
    /* !LINKSTO EB_Crypto_01050, 1 */
    SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    DriverObj->DriverObjectState = CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
    DriverObj->CurrentJob = NULL_PTR;

    DriverObj->SkipPeriodicMainFunction = TRUE;
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    /* !LINKSTO SWS_Crypto_00025, 1 */
    /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
    Job->jobState = CRYPTO_JOBSTATE_IDLE;
  }

  /* !LINKSTO EB_Crypto_00034, 1 */
  return RetVal;
        [!IF "$Service != 'HASH'"!]
#else /* #if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON) */
  TS_PARAM_UNUSED(ObjectId);
  TS_PARAM_UNUSED(Job);
  return E_NOT_OK;
#endif /* #if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_OFF) */
        [!ENDIF!]
}


[!IF "not(contains($Service, 'RANDOMGENERATE'))"!]
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_Process_SyncUpdate
(
  [!IF "contains($Output, 'U')"!]uint32 StartOutputLength,[!ENDIF!]
  uint32 ObjectId,
  boolean Busy,
  Std_ReturnType RetVal,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  [!IF "contains($Output, 'U')"!]
  if (
      ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL) ==
      (((uint8) Job->jobPrimitiveInputOutput.mode) & ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL ))
     )
  {
    /* !LINKSTO EB_Crypto_01029, 1 */
    Job->jobPrimitiveInputOutput.outputPtr =
    &(Job->jobPrimitiveInputOutput.outputPtr[*(Job->jobPrimitiveInputOutput.outputLengthPtr)]);
    /* !LINKSTO EB_Crypto_01030, 1 */
    *(Job->jobPrimitiveInputOutput.outputLengthPtr) =
    StartOutputLength - *(Job->jobPrimitiveInputOutput.outputLengthPtr);
  }
  [!ENDIF!]

  /* Perform finish operation in case of Sync Processing type*/
  RetVal = Crypto_xVIx_xAIx_AL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_Finish(ObjectId, Job);
  /* !LINKSTO SWS_Crypto_00026, 1,
              EB_Crypto_00032, 1,
              EB_Crypto_00033, 1
  */
  if (E_OK == RetVal)
  {
    /* Wait for result if request is synchronous */
    do
    {
      Crypto_xVIx_xAIx_AL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_MainFunction(&RetVal, &Busy);
    }
    while (CRYPTO_XVIX_XAIX_DRIVER_OBJECT_ACTIVE == Busy);
  }
  return RetVal;
}

[!ENDIF!]

/* !LINKSTO EB_Crypto_00035, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_Cancel
(
  uint32 ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* !LINKSTO EB_Crypto_00036, 1 */
  RetVal = Crypto_xVIx_xAIx_AL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_Cancel(ObjectId, Job);
  return RetVal;
}
    [!ENDLOOP!]
  [!ENDLOOP!]
[!ENDLOOP!]

[!IF "$PL_MainFunction = 'true'"!][!//
#if CRYPTO_XVIX_XAIX_CRYPTO_MAIN_FUNCTION_EXISTS == STD_ON
/* !LINKSTO EB_Crypto_00056, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_PL_MainFunction
(
  void
)
{
[!VAR "RsaPssVrfyFound" = "0"!]
[!VAR "RsaPkcsVrfyFound" = "0"!]
[!VAR "RsaPkcsGenFound" = "0"!]
[!VAR "RsaesOaepEncFound" = "0"!]
[!VAR "RsaesOaepDecFound" = "0"!]
[!LOOP "CryptoDriverObjects/CryptoDriverObject/*/CryptoPrimitiveRef/*"!]
  [!IF "as:ref(.)/CryptoPrimitiveService = 'DECRYPT'"!]
    [!VAR "Service" = "'DECRYPT'"!]
  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'ENCRYPT'"!]
    [!VAR "Service" = "'ENCRYPT'"!]
  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'MAC_GENERATE'"!]
    [!VAR "Service" = "'MACGENERATE'"!]
  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'MAC_VERIFY'"!]
    [!VAR "Service" = "'MACVERIFY'"!]
  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'RANDOM'"!]
    [!VAR "Service" = "'RANDOMGENERATE'"!]

  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'AEAD_DECRYPT'"!]
    [!VAR "Service" = "'AEADDECRYPT'"!]

  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'AEAD_ENCRYPT'"!]
    [!VAR "Service" = "'AEADENCRYPT'"!]

  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'HASH'"!]
    [!VAR "Service" = "'HASH'"!]

  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'SIGNATURE_GENERATE'"!]
    [!VAR "Service" = "'SIGNATUREGENERATE'"!]

  [!ELSEIF "as:ref(.)/CryptoPrimitiveService = 'SIGNATURE_VERIFY'"!]
    [!VAR "Service" = "'SIGNATUREVERIFY'"!]
  [!ENDIF!]
  [!LOOP "as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*"!]
    [!VAR "Family" = "node:value(.)"!]
    [!LOOP "../../CryptoPrimitiveAlgorithmMode/*"!]
      [!VAR "Mode" = "node:value(.)"!]

      [!IF "($Service = 'SIGNATUREVERIFY' and $Mode = 'CRYPTO_ALGOMODE_RSASSA_PSS' and $RsaPssVrfyFound = 1) or
            ($Service = 'SIGNATUREVERIFY' and $Mode = 'CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5' and $RsaPkcsVrfyFound = 1) or
            ($Service = 'SIGNATUREGENERATE' and $Mode = 'CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5' and $RsaPkcsGenFound = 1) or
            ($Service = 'ENCRYPT' and $Mode = 'CRYPTO_ALGOMODE_RSAES_OAEP' and $RsaesOaepEncFound = 1) or
            ($Service = 'DECRYPT' and $Mode = 'CRYPTO_ALGOMODE_RSAES_OAEP' and $RsaesOaepDecFound = 1)"!][!//
        [!BREAK!]
      [!ENDIF!]
      [!IF "$Service = 'SIGNATUREVERIFY' and $Mode = 'CRYPTO_ALGOMODE_RSASSA_PSS' and $RsaPssVrfyFound = 0"!]
        [!VAR "RsaPssVrfyFound" = "1"!]
      [!ELSEIF "$Service = 'SIGNATUREVERIFY' and $Mode = 'CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5' and $RsaPkcsVrfyFound = 0"!]
        [!VAR "RsaPkcsVrfyFound" = "1"!]
      [!ELSEIF "$Service = 'SIGNATUREGENERATE' and $Mode = 'CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5' and $RsaPkcsGenFound = 0"!]
        [!VAR "RsaPkcsGenFound" = "1"!]
      [!ELSEIF "$Service = 'ENCRYPT' and $Mode = 'CRYPTO_ALGOMODE_RSAES_OAEP' and $RsaesOaepEncFound = 0"!]
        [!VAR "RsaesOaepEncFound" = "1"!]
      [!ELSEIF "$Service = 'DECRYPT' and $Mode = 'CRYPTO_ALGOMODE_RSAES_OAEP' and $RsaesOaepDecFound = 0"!]
        [!VAR "RsaesOaepDecFound" = "1"!]
      [!ENDIF!]
  Crypto_xVIx_xAIx_AL_[!"$Service"!]_[!"text:replace($Family,'CRYPTO_ALGOFAM_','')"!]_[!"text:replace($Mode,'CRYPTO_ALGOMODE_','')"!]_MainFunction(NULL_PTR, NULL_PTR);
    [!ENDLOOP!]
  [!ENDLOOP!]
[!ENDLOOP!]
}
#endif /* CRYPTO_XVIX_XAIX_CRYPTO_MAIN_FUNCTION_EXISTS == STD_ON */
[!ENDIF!][!//

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==================[definition of internal functions]============================================*/

/*==================[end of file]=================================================================*/
