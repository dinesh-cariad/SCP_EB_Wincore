/**
 * \file
 *
 * \brief AUTOSAR CryIf
 *
 * This file contains the implementation of the AUTOSAR
 * module CryIf.
 *
 * \version 1.0.24
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO SWS_CryIf_00004, 1 */

/*==================[misra deviations]============================================================*/

/*==================[inclusions]==================================================================*/

/* !LINKSTO EB_CryIf_00011, 1 */
#include <TSAutosar.h>                                              /* EB specific standard types */

#include <TSMem.h>

/* !LINKSTO SWS_CryIf_00008, 1 */
#include <CryIf.h>
#include <Csm_Cbk.h>
/* !LINKSTO EB_CryIf_00011, 1 */
#include <CryIf_Cbk.h>

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
#include <Det.h>                                              /* AUTOSAR development error tracer */
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */

/*==================[macros]======================================================================*/

/*==================[declaration of types]========================================================*/

/*==================[declaration of internal functions]===========================================*/

/*==================[definition of internal constants]============================================*/

/*==================[definition of internal variables]============================================*/

#define CRYIF_START_SEC_VAR_INIT_BOOLEAN
#include <CryIf_MemMap.h>

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
/** \brief Indicates whether the Crypto Interface module has been initialized or not */
/* !LINKSTO CryIf.Dsn.Api/Data/00001, 1 */
static VAR(boolean, CRYIF_VAR) CryIf_Initialized = FALSE;
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */

#define CRYIF_STOP_SEC_VAR_INIT_BOOLEAN
#include <CryIf_MemMap.h>

/*==================[definition of external constants]============================================*/

/*==================[definition of external variables]============================================*/

/*==================[definition of external functions]============================================*/

#define CRYIF_START_SEC_CODE
#include <CryIf_MemMap.h>

/* !LINKSTO SWS_CryIf_91000, 1 */
FUNC(void, CRYIF_CODE) CryIf_Init
(
  void
)
{
  #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00015, 1 */
  CryIf_Initialized = TRUE;
  #endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
}

#if (CRYIF_API_ENABLED_VERSIONINFO == STD_ON)
/* !LINKSTO SWS_CryIf_91001_CORRECTION, 1 */
FUNC(void, CRYIF_CODE) CryIf_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CRYIF_APPL_DATA) versioninfo
)
{
#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!CryIf_Initialized)
  {
    /* !LINKSTO SWS_CryIf_00016, 1 */
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_GETVERSIONINFO,
      CRYIF_E_UNINIT
    );
  }
  else if (versioninfo == NULL_PTR)
  {
    /* !LINKSTO SWS_CryIf_00017, 1 */
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_GETVERSIONINFO,
      CRYIF_E_PARAM_POINTER
    );
  }
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    versioninfo->vendorID         = CRYIF_VENDOR_ID;
    versioninfo->moduleID         = CRYIF_MODULE_ID;
    versioninfo->sw_major_version = CRYIF_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = CRYIF_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = CRYIF_SW_PATCH_VERSION;
  }
}
#endif /* #if (CRYIF_API_ENABLED_VERSIONINFO == STD_ON) */

/* !LINKSTO SWS_CryIf_91003, 1 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_ProcessJob
(
  uint32 channelId,
  P2VAR(Crypto_JobType , AUTOMATIC, CRYIF_APPL_DATA) job
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00027, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_PROCESSJOB,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00029, 1 */
  else if (job == NULL_PTR)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_PROCESSJOB,
      CRYIF_E_PARAM_POINTER
    );
  }
#if (CRYIF_CHANNEL_COUNT != 0U)
  /* !LINKSTO SWS_CryIf_00028, 1 */
  else if (channelId >= CRYIF_CHANNEL_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_PROCESSJOB,
      CRYIF_E_PARAM_HANDLE
    );
  }
#endif /* #if (CRYIF_CHANNEL_COUNT != 0U) */
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /*
     * Request if either the cryIfKeyId is < the configured amount of keys
     * or if an algorithm without the need for a keyId is chosen (Hash)
     */
    /* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
    /* !LINKSTO EB_CryIf_00009, 1 */
    if (
         (job->jobPrimitiveInfo->cryIfKeyId <  CRYIF_KEY_COUNT) ||
         (job->jobPrimitiveInfo->cryIfKeyId == CRYIF_KEY_EMPTY)
       )
     {
        /* !LINKSTO EB_CryIf_00008, 2 */
      if(
          (job->jobPrimitiveInfo->cryIfKeyId != CRYIF_KEY_EMPTY)
          &&(job->jobPrimitiveInfo->primitiveInfo->service != CRYPTO_HASH)
          &&(job->jobPrimitiveInfo->primitiveInfo->service != CRYPTO_SECCOUNTERREAD)
          &&(job->jobPrimitiveInfo->primitiveInfo->service != CRYPTO_SECCOUNTERINCREMENT)
        )
        {
          job->cryptoKeyId = CryIf_Keys[job->jobPrimitiveInfo->cryIfKeyId];
        }
        else
#else /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */
    {
#endif
      {
      /* Use default init value from Csm if cryptoKeyId cannot be obtained */
        job->cryptoKeyId = CRYIF_KEY_EMPTY;
      }
#if (CRYIF_CHANNEL_COUNT != 0U)
      /* !LINKSTO SWS_CryIf_00044, 1 */
      RetVal = CryIf_ProcessJobJumpTable[channelId](CryIf_Channels[channelId], job);
#else
      TS_PARAM_UNUSED(channelId);
#endif /* #if (CRYIF_CHANNEL_COUNT != 0U) */
    }
  }

  return RetVal;
}

/* !LINKSTO SWS_CryIf_91014, 1 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CancelJob
(
  uint32 channelId,
  P2VAR(Crypto_JobType , AUTOMATIC, CRYIF_APPL_DATA) job
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00129, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_CALLBACKNOTIFICATION,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00131, 1 */
  else if (job == NULL_PTR)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_PROCESSJOB,
      CRYIF_E_PARAM_POINTER
    );
  }
#if (CRYIF_CHANNEL_COUNT != 0U)
  /* !LINKSTO SWS_CryIf_00130, 1 */
  else if (channelId >= CRYIF_CHANNEL_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_PROCESSJOB,
      CRYIF_E_PARAM_HANDLE
    );
  }
#endif /* #if (CRYIF_CHANNEL_COUNT != 0U) */
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {

#if (   (CRYIF_API_VERSION == CRYIF_API_VERSION_430) \
     || (CRYIF_API_VERSION == CRYIF_API_VERSION_431) \
    )

#if (CRYIF_CHANNEL_COUNT != 0U)
      Crypto_JobInfoType ji = { 0U, 0U };

      TS_MemCpy(&ji, job->jobInfo, sizeof(Crypto_JobInfoType));

      /* !LINKSTO SWS_CryIf_00132, 1 */
      RetVal = CryIf_CancelJobJumpTable[channelId](CryIf_Channels[channelId], &ji);
#else
      TS_PARAM_UNUSED(channelId);
#endif /* #if (CRYIF_CHANNEL_COUNT != 0U) */

#endif /* #if (   (CRYIF_API_VERSION == CRYIF_API_VERSION_430) ... */

#if (CRYIF_API_VERSION == CRYIF_API_VERSION_EB)
    /*
     * Request if either the cryIfKeyId is < the configured amount of keys
     * or if an algorithm without the need for a keyId is chosen (Hash)
     */
    /* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
    /* !LINKSTO EB_CryIf_00010, 1 */
    if (
         (job->jobPrimitiveInfo->cryIfKeyId <  CRYIF_KEY_COUNT) ||
         (job->jobPrimitiveInfo->cryIfKeyId == CRYIF_KEY_EMPTY)
       )
     {
        /* !LINKSTO EB_CryIf_00008, 2 */
      if(
          (job->jobPrimitiveInfo->cryIfKeyId != CRYIF_KEY_EMPTY)
          &&(job->jobPrimitiveInfo->primitiveInfo->service != CRYPTO_HASH)
          &&(job->jobPrimitiveInfo->primitiveInfo->service != CRYPTO_SECCOUNTERREAD)
          &&(job->jobPrimitiveInfo->primitiveInfo->service != CRYPTO_SECCOUNTERINCREMENT)
        )
      {
        job->cryptoKeyId = CryIf_Keys[job->jobPrimitiveInfo->cryIfKeyId];
      }
      else
#else /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */
    {
#endif/* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */
        {
        /* Use default init value from Csm if cryptoKeyId cannot be obtained */
          job->cryptoKeyId = CRYIF_KEY_EMPTY;
        }
#if (CRYIF_CHANNEL_COUNT != 0U)
      /* !LINKSTO SWS_CryIf_00132, 1 */
      RetVal = CryIf_CancelJobJumpTable[channelId](CryIf_Channels[channelId], job);
#else
      TS_PARAM_UNUSED(channelId);
#endif /* #if (CRYIF_CHANNEL_COUNT != 0U) */
    }
#endif /* #if (CRYIF_API_VERSION == CRYIF_API_VERSION_EB) */

  }

  return RetVal;
}

/* !LINKSTO SWS_CryIf_91013, 1 */
FUNC(void, CRYIF_CODE) CryIf_CallbackNotification
(
  P2CONST(Crypto_JobType , AUTOMATIC, CRYIF_APPL_DATA) job,
  Std_ReturnType result
)
{
#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00107, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_CALLBACKNOTIFICATION,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00108, 1 */
  else if (job == NULL_PTR)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_CALLBACKNOTIFICATION,
      CRYIF_E_PARAM_POINTER
    );
  }
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_CryIf_00109, 1, SWS_CryIf_00002, 1 */
    Csm_CallbackNotification(job, result);
  }
}

/* !LINKSTO SWS_CryIf_91004, 1, CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementSet
(
  uint32 cryIfKeyId,
  uint32 keyElementId,
  P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) keyPtr,
  uint32 keyLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00049, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYELEMENTSET,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00050, 1 */
  else if (cryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYELEMENTSET,
      CRYIF_E_PARAM_HANDLE
    );
  }
  /* !LINKSTO SWS_CryIf_00052, 1 */
  else if (keyPtr == NULL_PTR)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYELEMENTSET,
      CRYIF_E_PARAM_POINTER
    );
  }
  /* !LINKSTO SWS_CryIf_00053, 1 */
  else if (keyLength == 0)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYELEMENTSET,
      CRYIF_E_PARAM_VALUE
    );
  }
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_CryIf_00055, 1 */
    RetVal = CryIf_KeyElementSetJumpTable[cryIfKeyId]
             (
               CryIf_Keys[cryIfKeyId],
               keyElementId,
               keyPtr,
               keyLength
             );
  }
  return RetVal;
}
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO SWS_CryIf_91005, 1, CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeySetValid
(
  uint32 cryIfKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00056, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYSETVALID,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00057, 1 */
  else if (cryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYSETVALID,
      CRYIF_E_PARAM_HANDLE
    );
  }
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_CryIf_00058, 1 */
    RetVal = CryIf_KeySetValidJumpTable[cryIfKeyId](CryIf_Keys[cryIfKeyId]);
  }
  return RetVal;
}
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO SWS_CryIf_91006, 1, CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
 FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementGet
(
  uint32 cryIfKeyId,
  uint32 keyElementId,
  P2VAR(uint8, AUTOMATIC, CRYIF_APPL_DATA) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYIF_APPL_DATA) resultLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00059, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYELEMENTGET,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00060, 1 */
  else if (cryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYELEMENTGET,
      CRYIF_E_PARAM_HANDLE
    );
  }
  /* !LINKSTO SWS_CryIf_00062, 1 */
  else if (resultPtr == NULL_PTR)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYELEMENTGET,
      CRYIF_E_PARAM_POINTER
    );
  }
  /* !LINKSTO SWS_CryIf_00063, 1 */
  else if (resultLengthPtr == NULL_PTR)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYELEMENTGET,
      CRYIF_E_PARAM_POINTER
    );
  }
  /* !LINKSTO SWS_CryIf_00064, 1 */
  else if (*resultLengthPtr == 0)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYELEMENTGET,
      CRYIF_E_PARAM_VALUE
    );
  }
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_CryIf_00065, 1 */
    RetVal = CryIf_KeyElementGetJumpTable[cryIfKeyId]
             (
               CryIf_Keys[cryIfKeyId],
               keyElementId,
               resultPtr,
               resultLengthPtr
             );
  }
  return RetVal;
}
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO SWS_CryIf_91015, 1, CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementCopy
(
  uint32 cryIfKeyId,
  uint32 keyElementId,
  uint32 targetCryIfKeyId,
  uint32 targetKeyElementId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  uint8 KeyElementBuffer[CRYIF_MAX_KEY_ELEMNT_COPY_SIZE] = {0U};
  uint32 KeyElementLength = CRYIF_MAX_KEY_ELEMNT_COPY_SIZE;

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00110, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYELEMENTCOPY,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00111, 1 */
  else if (cryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYELEMENTCOPY,
      CRYIF_E_PARAM_HANDLE
    );
  }
  /* !LINKSTO SWS_CryIf_00112, 1 */
  else if (targetCryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYELEMENTCOPY,
      CRYIF_E_PARAM_HANDLE
    );
  }
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_CryIf_00113, 1 */
    if (
         CryIf_KeyElementCopyJumpTable[cryIfKeyId] ==
         CryIf_KeyElementCopyJumpTable[targetCryIfKeyId]
       )
    {
      RetVal = CryIf_KeyElementCopyJumpTable[cryIfKeyId]
      (
        CryIf_Keys[cryIfKeyId],
        keyElementId,
        CryIf_Keys[targetCryIfKeyId],
        targetKeyElementId
      );
    }
    /* !LINKSTO SWS_CryIf_00114, 1 */
    else
    {
      RetVal = CryIf_KeyElementGetJumpTable[cryIfKeyId]
      (
        CryIf_Keys[cryIfKeyId],
        keyElementId,
        KeyElementBuffer,
        &KeyElementLength
      );
      if (RetVal == E_OK)
      {
        RetVal = CryIf_KeyElementSetJumpTable[targetCryIfKeyId]
        (
          CryIf_Keys[targetCryIfKeyId],
          targetKeyElementId,
          KeyElementBuffer,
          KeyElementLength
        );
      }
    }

    /* !LINKSTO SWS_CryIf_00115_CORRECTION, 1 */
    if (RetVal == CRYPTO_E_KEY_SIZE_MISMATCH)
    {
#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
      (void)Det_ReportError
      (
        CRYIF_MODULE_ID,
        CRYIF_INSTANCE_ID,
        CRYIF_SID_KEYELEMENTCOPY,
        CRYIF_E_KEY_SIZE_MISMATCH
      );
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
    }
  }
  return RetVal;
}
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO SWS_CryIf_91016, 1, CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
 FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyCopy
(
  uint32 cryIfKeyId,
  uint32 targetCryIfKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00116, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYCOPY,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00117, 1 */
  else if (cryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYCOPY,
      CRYIF_E_PARAM_HANDLE
    );
  }
  /* !LINKSTO SWS_CryIf_00118, 1 */
  else if (targetCryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYCOPY,
      CRYIF_E_PARAM_HANDLE
    );
  }
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
#if (CRYIF_MAX_KEY_ELEMENTS_IN_KEY > 0U)
    uint32 KeyElementIds[CRYIF_MAX_KEY_ELEMENTS_IN_KEY] = { 0 };
    uint32 KeyElementIdsLength = CRYIF_MAX_KEY_ELEMENTS_IN_KEY;
    uint8 KeyElementBuffer[CRYIF_MAX_KEY_ELEMNT_COPY_SIZE] = {0};
    uint32 KeyElementLength = 0;
    uint32 KeyElementTargetLength = 0;
    uint32 i = 0;

    /* !LINKSTO SWS_CryIf_00119, 1 */
    if (CryIf_KeyCopyJumpTable[cryIfKeyId] == CryIf_KeyCopyJumpTable[targetCryIfKeyId])
    {
      RetVal = CryIf_KeyCopyJumpTable[cryIfKeyId]
      (
          CryIf_Keys[cryIfKeyId],
          CryIf_Keys[targetCryIfKeyId]
      );
    }
    else
    {
      /* !LINKSTO SWS_CryIf_00120, 1 */
      RetVal = CryIf_KeyElementIdsGetJumpTable[cryIfKeyId]
      (
        CryIf_Keys[cryIfKeyId],
        &(KeyElementIds[0]),
        &KeyElementIdsLength
      );
      /* !LINKSTO EB_CryIf_00007, 2 */
      for (i = 0;( i < KeyElementIdsLength) && (RetVal == E_OK); i++)
      {
        KeyElementLength = CRYIF_MAX_KEY_ELEMNT_COPY_SIZE;
        /* Get the source keyElement */
        RetVal = CryIf_KeyElementGetJumpTable[cryIfKeyId]
        (
          CryIf_Keys[cryIfKeyId],
          KeyElementIds[i],
          KeyElementBuffer,
          &KeyElementLength
        );
        if (RetVal == E_OK)
        {
          KeyElementTargetLength = KeyElementLength;
          /* Get the target keyElement.
           * If the target KeyElement is larger than the source KeyElement and partial access
           * is not allowed, this will fail. */
          RetVal = CryIf_KeyElementGetJumpTable[targetCryIfKeyId]
          (
            CryIf_Keys[targetCryIfKeyId],
            KeyElementIds[i],
            KeyElementBuffer,
            &KeyElementTargetLength
          );
          /* If no other errors occure check if source and target key element have the same size.
           * If not, processing is canceled */
          if ((KeyElementTargetLength != KeyElementLength) && (RetVal == E_OK))
          {
            RetVal = CRYPTO_E_KEY_SIZE_MISMATCH;
          }
        }
      }
      /* Only if the testrun worked, start copying */
      for (i = 0;( i < KeyElementIdsLength) && (RetVal == E_OK); i++)
      {
        KeyElementLength = CRYIF_MAX_KEY_ELEMNT_COPY_SIZE;
        /* Return value ignored because of duplicate check in testrun */
        (void)CryIf_KeyElementGetJumpTable[cryIfKeyId]
        (
          CryIf_Keys[cryIfKeyId],
          KeyElementIds[i],
          KeyElementBuffer,
          &KeyElementLength
        );
        RetVal = CryIf_KeyElementSetJumpTable[targetCryIfKeyId]
        (
          CryIf_Keys[targetCryIfKeyId],
          KeyElementIds[i],
          KeyElementBuffer,
          KeyElementLength
        );
      }
    }
    /* !LINKSTO SWS_CryIf_00121_CORRECTION, 1 */
    if (RetVal == CRYPTO_E_KEY_SIZE_MISMATCH)
    {
#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
      (void)Det_ReportError
      (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYELEMENTCOPY,
      CRYIF_E_KEY_SIZE_MISMATCH
      );
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
    }
#endif /* #if (CRYIF_MAX_KEY_ELEMENTS_IN_KEY > 0U) */
  }

#if ((CRYIF_API_ENABLED_DEVERRORDETECT == STD_OFF) && (CRYIF_MAX_KEY_ELEMENTS_IN_KEY == 0U))
  TS_PARAM_UNUSED(cryIfKeyId);
  TS_PARAM_UNUSED(targetCryIfKeyId);
#endif /* #if ((CRYIF_API_ENABLED_DEVERRORDETECT == STD_OFF) && (CRYIF_MAX_KEY_ELEMENTS_IN_KEY == 0U)) */

  return RetVal;
}
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO SWS_CryIf_91007, 1, CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
 FUNC(Std_ReturnType, CRYIF_CODE) CryIf_RandomSeed
(
  uint32 cryIfKeyId,
  P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) seedPtr,
  uint32 seedLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00068, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_RANDOMSEED,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00069, 1 */
  else if (cryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_RANDOMSEED,
      CRYIF_E_PARAM_HANDLE
    );
  }
  /* !LINKSTO SWS_CryIf_00070, 1 */
  else if (seedPtr == NULL_PTR)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_RANDOMSEED,
      CRYIF_E_PARAM_POINTER
    );
  }
  /* !LINKSTO SWS_CryIf_00071, 1 */
  else if (seedLength == 0)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_RANDOMSEED,
      CRYIF_E_PARAM_VALUE
    );
  }
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_CryIf_00072, 1 */
    RetVal = CryIf_RandomSeedJumpTable[cryIfKeyId]
    (
      CryIf_Keys[cryIfKeyId],
      seedPtr,
      seedLength
    );
  }
  return RetVal;
}
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO SWS_CryIf_91008, 1, CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType,CRYIF_CODE) CryIf_KeyGenerate
(
  uint32 cryIfKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00073, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYGENERATE,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00074, 1 */
  else if (cryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYGENERATE,
      CRYIF_E_PARAM_HANDLE
    );
  }
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_CryIf_00075, 1 */
    RetVal = CryIf_KeyGenerateJumpTable[cryIfKeyId](CryIf_Keys[cryIfKeyId]);
  }
  return RetVal;
}
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO SWS_CryIf_91009, 1, CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyDerive
(
  uint32 cryIfKeyId,
  uint32 targetCryIfKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00076, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYDERIVE,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00077, 1 */
  else if (cryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYDERIVE,
      CRYIF_E_PARAM_HANDLE
    );
  }
  /* !LINKSTO SWS_CryIf_00122, 1 */
  else if (targetCryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYDERIVE,
      CRYIF_E_PARAM_HANDLE
    );
  }
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_CryIf_00081, 1 */
    /* Check if both keys are stored in the same Crypto driver */
    if (CryIf_KeyDeriveJumpTable[cryIfKeyId] == CryIf_KeyDeriveJumpTable[targetCryIfKeyId])
    {
      RetVal = CryIf_KeyDeriveJumpTable[cryIfKeyId]
      (
        CryIf_Keys[cryIfKeyId],
        CryIf_Keys[targetCryIfKeyId]
      );
    }
  }
  return RetVal;
}
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO SWS_CryIf_91010, 1, CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyExchangeCalcPubVal
(
  uint32 cryIfKeyId,
  P2VAR(uint8, AUTOMATIC, CRYIF_APPL_DATA) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYIF_APPL_DATA) publicValueLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00082, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYEXCHANGECALCPUBVAL,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00083, 1 */
  else if (cryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYEXCHANGECALCPUBVAL,
      CRYIF_E_PARAM_HANDLE
    );
  }
  /* !LINKSTO SWS_CryIf_00084, 1 */
  else if (publicValuePtr == NULL_PTR)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYEXCHANGECALCPUBVAL,
      CRYIF_E_PARAM_POINTER
    );
  }
  /* !LINKSTO SWS_CryIf_00085, 1 */
  else if (publicValueLengthPtr == NULL_PTR)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYEXCHANGECALCPUBVAL,
      CRYIF_E_PARAM_POINTER
    );
  }
  /* !LINKSTO SWS_CryIf_00086, 1 */
  else if (*publicValueLengthPtr == 0)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYEXCHANGECALCPUBVAL,
      CRYIF_E_PARAM_VALUE
    );
  }
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_CryIf_00087, 1 */
    RetVal = CryIf_KeyExchangeCalcPubValJumpTable[cryIfKeyId]
    (
      CryIf_Keys[cryIfKeyId],
      publicValuePtr,
      publicValueLengthPtr
    );
  }

  return RetVal;
}
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO SWS_CryIf_91011, 1, CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyExchangeCalcSecret
(
  uint32 cryIfKeyId,
  P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) partnerPublicValuePtr,
  uint32 partnerPublicValueLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00090, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYEXCHANGECALCSECRET,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00091, 1 */
  else if (cryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYEXCHANGECALCSECRET,
      CRYIF_E_PARAM_HANDLE
    );
  }
  /* !LINKSTO SWS_CryIf_00092, 1 */
  else if (partnerPublicValuePtr == NULL_PTR)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYEXCHANGECALCSECRET,
      CRYIF_E_PARAM_POINTER
    );
  }
  /* !LINKSTO SWS_CryIf_00094, 1 */
  else if (partnerPublicValueLength == 0)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_KEYEXCHANGECALCSECRET,
      CRYIF_E_PARAM_VALUE
    );
  }
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_CryIf_00095, 1 */
    RetVal = CryIf_KeyExchangeCalcSecretJumpTable[cryIfKeyId]
    (
      CryIf_Keys[cryIfKeyId],
      partnerPublicValuePtr,
      partnerPublicValueLength
    );
  }
  return RetVal;
}
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO SWS_CryIf_91012, 1, CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CertificateParse
(
  uint32 cryIfKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00098, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_CERTIFICATEPARSE,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00099, 1 */
  else if (cryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_CERTIFICATEPARSE,
      CRYIF_E_PARAM_HANDLE
    );
  }
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_CryIf_00104, 1 */
    RetVal = CryIf_CertificateParseJumpTable[cryIfKeyId]
    (
      CryIf_Keys[cryIfKeyId]
    );
  }
  return RetVal;
}
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO SWS_CryIf_91017, 1, CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CertificateVerify
(
  uint32 cryIfKeyId,
  uint32 verifyCryIfKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYIF_APPL_DATA) verifyPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON)
  /* !LINKSTO SWS_CryIf_00123, 1 */
  if (!CryIf_Initialized)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_CERTIFICATEVERIFY,
      CRYIF_E_UNINIT
    );
  }
  /* !LINKSTO SWS_CryIf_00124, 1 */
  else if (cryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_CERTIFICATEVERIFY,
      CRYIF_E_PARAM_HANDLE
    );
  }
  /* !LINKSTO SWS_CryIf_00125, 1 */
  else if (verifyCryIfKeyId >= CRYIF_KEY_COUNT)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_CERTIFICATEVERIFY,
      CRYIF_E_PARAM_HANDLE
    );
  }
  /* !LINKSTO SWS_CryIf_00127, 1 */
  else if (verifyPtr == NULL_PTR)
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_CERTIFICATEVERIFY,
      CRYIF_E_PARAM_POINTER
    );
  }
  /* !LINKSTO SWS_CryIf_00126, 1 */
  else if (
            CryIf_CertificateVerifyJumpTable[cryIfKeyId] !=
            CryIf_CertificateVerifyJumpTable[verifyCryIfKeyId]
          )
  {
    (void)Det_ReportError
    (
      CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID,
      CRYIF_SID_CERTIFICATEVERIFY,
      CRYIF_E_PARAM_HANDLE
    );
  }
  else
#endif /* #if (CRYIF_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO SWS_CryIf_00128, 1 */
    RetVal = CryIf_CertificateVerifyJumpTable[cryIfKeyId]
    (
      CryIf_Keys[cryIfKeyId],
      CryIf_Keys[verifyCryIfKeyId],
      verifyPtr
    );
  }
  return RetVal;
}
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#define CRYIF_STOP_SEC_CODE
#include <CryIf_MemMap.h>

/*==================[definition of internal functions]============================================*/

/*==================[end of file]=================================================================*/

