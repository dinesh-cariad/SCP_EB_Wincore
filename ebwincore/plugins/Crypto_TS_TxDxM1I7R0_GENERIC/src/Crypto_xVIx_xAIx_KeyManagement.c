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

/* !LINKSTO SWS_Crypto_00005, 1 */

/*==================[misra deviations]============================================================*/

/*==================[inclusions]==================================================================*/

#include <TSAutosar.h>
/* !LINKSTO SWS_Crypto_00009, 1, SWS_Crypto_00010, 1 */
#include <Crypto_xVIx_xAIx_KeyManagement.h>
#include <Crypto_xVIx_xAIx_AL_KeyManagement.h>
#include <CryIf_Cbk.h>
#include <Crypto_xVIx_xAIx_Int.h>
#include <Crypto_xVIx_xAIx_Version.h>
#include <SchM_Crypto_xVIx_xAIx.h>

/* !LINKSTO EB_Crypto_01019, 1 */
#if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON)

#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif


/*==================[macros]======================================================================*/

/*==================[declaration of types]========================================================*/

/*==================[declaration of internal functions]===========================================*/


#if  (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Checks the validity of a CV certificate current date.
 *  Given the KeyId, it gets the current time and checks the
 *  validity of its format.
 *
 * \param[in]  VerifyCryptoKeyId KeyId of the certificate to be verified.
 *
 * \retval E_OK     If the current date of the certificate has valid format.
 * \retval E_NOT_OK If the current date of the certificate has invalid format.
 */
static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CertificateCurrentDateVerify
(
  uint32 VerifyCryptoKeyId
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if  (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON) */

/*==================[definition of internal constants]============================================*/

/*==================[definition of internal variables]============================================*/

/*==================[definition of external constants]============================================*/

/*==================[definition of external variables]============================================*/

/*==================[definition of external constants]============================================*/

/*==================[definition of external variables]============================================*/

/*==================[definition of external functions]============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_KeyElementSet]------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO SWS_Crypto_91004, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyElementSet
(
  uint32                                                CryptoKeyId,
  uint32                                                KeyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr,
  uint32                                                KeyLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;
#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  boolean Found = FALSE;
#endif
  uint32 KeyElementIndex;
  uint32 KeyElementLength;
  uint32 ObjectId = 0U;
  Crypto_xVIx_xAIx_DriverObject* DriverObj = NULL_PTR;

#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  if (Crypto_xVIx_xAIx_Initialized == FALSE)
  {
    /* !LINKSTO SWS_Crypto_00075, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTSET,
     CRYPTO_XVIX_XAIX_E_UNINIT
    );
    RetVal = E_NOT_OK;
  }
  else if (CryptoKeyId >= CRYPTO_XVIX_XAIX_KEY_COUNT)
  {
    /* !LINKSTO SWS_Crypto_00076, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTSET,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else if (KeyPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Crypto_00078, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTSET,
     CRYPTO_XVIX_XAIX_E_PARAM_POINTER
    );
    RetVal = E_NOT_OK;
  }
  else if (KeyLength == 0U)
  {
    /* !LINKSTO SWS_Crypto_00079, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTSET,
     CRYPTO_XVIX_XAIX_E_PARAM_VALUE
    );
    RetVal = E_NOT_OK;
  }
  else
#endif
  {
    {
      /* Find referenced keyElement within the cryptoKey */
      for (
            KeyElementIndex = 0U;
            KeyElementIndex < Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements;
            KeyElementIndex++
          )
      {
        if (Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[KeyElementIndex]->Id == KeyElementId)
        {
#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
          Found = TRUE;
#endif
          break;
        }
      }

#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
      if (FALSE == Found)
      {
        /* Referenced keyElement is not contained within the cryptoKey */
        /* !LINKSTO SWS_Crypto_00077, 1 */
        (void) Det_ReportError
        (
         CRYPTO_XVIX_XAIX_MODULE_ID,
         CRYPTO_XVIX_XAIX_INSTANCE_ID,
         CRYPTO_XVIX_XAIX_SID_KEYELEMENTSET,
         CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
        );
        RetVal = E_NOT_OK;
      }
      else
#endif
      {
        /* Check if crypto driver is busy */
        /* !LINKSTO EB_Crypto_00008, 1 */
        DriverObj = &Crypto_xVIx_xAIx_DriverObjects[ObjectId];
        SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
          if (DriverObj->CurrentJob != NULL_PTR)
          {
            if (DriverObj->CurrentJob->cryptoKeyId == CryptoKeyId)
            {
              RetVal = CRYPTO_E_BUSY;
            }
          }
        SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_OFF)
        if (RetVal != CRYPTO_E_BUSY)
#else
        if (   (RetVal != CRYPTO_E_BUSY)
            && (TRUE   == Found        )
           )
#endif /* #if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_OFF) */
        {
          if (
               ( Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[KeyElementIndex]->WriteAccess ==
                 CRYPTO_XVIX_XAIX_WA_DENIED
               ) ||
               ( Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[KeyElementIndex]->WriteAccess ==
                 CRYPTO_XVIX_XAIX_WA_INTERNAL_COPY
               )
             )
          {
            /* Writing the keyElement is not allowed by the configuration */
            RetVal = CRYPTO_E_KEY_WRITE_FAIL;
          }
          else if (
                    (KeyLength != Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[KeyElementIndex]->Size) &&
                    (
                      Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[KeyElementIndex]->AllowPartialAccess ==
                      FALSE
                    )
                  )
          {
            /* !LINKSTO EB_Crypto_01090, 1 */
            /* !LINKSTO SWS_Crypto_00146, 1 */
            RetVal = CRYPTO_E_KEY_SIZE_MISMATCH;
          }
          else
          {
            if (KeyLength > Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[KeyElementIndex]->Size)
            {
              /* !LINKSTO EB_Crypto_01091, 1 */
              KeyElementLength = Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[KeyElementIndex]->Size;
            }
            else
            {
              KeyElementLength = KeyLength;
            }

            /* Set key state to invalid before forwarding the request */
            Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyState = CRYPTO_XVIX_XAIX_KEY_STATE_INVALID;

            /* !LINKSTO EB_Crypto_00007, 1 */
            RetVal = Crypto_xVIx_xAIx_PL_KeyElementSet(
                                                       CryptoKeyId,
                                                       KeyElementId,
                                                       KeyPtr,
                                                       KeyElementLength
                                                      );
          }
        }
      }
    }
  }

  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_KeyValidSet]--------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO SWS_Crypto_91005, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyValidSet
(
  uint32 CryptoKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  if (Crypto_xVIx_xAIx_Initialized == FALSE)
  {
    /* !LINKSTO SWS_Crypto_00082, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYVALIDSET,
     CRYPTO_XVIX_XAIX_E_UNINIT
    );
    RetVal = E_NOT_OK;
  }
  else if (CryptoKeyId >= CRYPTO_XVIX_XAIX_KEY_COUNT)
  {
    /* !LINKSTO SWS_Crypto_00083, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYVALIDSET,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else
#endif
  {
    /* !LINKSTO EB_Crypto_01080, 1 */
    RetVal = Crypto_xVIx_xAIx_PL_KeyValidSet(CryptoKeyId);
  }
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_KeyElementGet]------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO SWS_Crypto_91006, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyElementGet
(
  uint32                                               CryptoKeyId,
  uint32                                               KeyElementId,
  P2VAR(uint8,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) ResultLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 keyElementIndex;

#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  if (Crypto_xVIx_xAIx_Initialized == FALSE)
  {
    /* !LINKSTO SWS_Crypto_00085, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTGET,
     CRYPTO_XVIX_XAIX_E_UNINIT
    );
    RetVal = E_NOT_OK;
  }
  else if (CryptoKeyId >= CRYPTO_XVIX_XAIX_KEY_COUNT)
  {
    /* !LINKSTO SWS_Crypto_00086, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTGET,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else if (ResultPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Crypto_00088, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTGET,
     CRYPTO_XVIX_XAIX_E_PARAM_POINTER
    );
    RetVal = E_NOT_OK;
  }
  else if (ResultLengthPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Crypto_00089, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTGET,
     CRYPTO_XVIX_XAIX_E_PARAM_POINTER
    );
    RetVal = E_NOT_OK;
  }
  else if (*ResultLengthPtr == 0U)
  {
    /* !LINKSTO SWS_Crypto_00090, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTGET,
     CRYPTO_XVIX_XAIX_E_PARAM_VALUE
    );
    RetVal = E_NOT_OK;
  }
  else
#endif
  {
    {

      /* !LINKSTO EB_Crypto_01020, 1 */
      if(CRYPTO_XVIX_XAIX_KEY_STATE_INVALID != Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyState)
      {
        /* Find referenced keyElement within the cryptoKey */
        for (
             keyElementIndex = 0U;
             keyElementIndex < Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements;
             keyElementIndex++
            )
        {
          if (Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[keyElementIndex]->Id == KeyElementId)
          {
            break;
          }
        }

        if (keyElementIndex == Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements)
        {
          /* Referenced keyElement is not contained within the cryptoKey */
          /* !LINKSTO SWS_Crypto_00140, 2 */
    #if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
          (void) Det_ReportError
            (
             CRYPTO_XVIX_XAIX_MODULE_ID,
             CRYPTO_XVIX_XAIX_INSTANCE_ID,
             CRYPTO_XVIX_XAIX_SID_KEYELEMENTGET,
             CRYPTO_XVIX_XAIX_E_RE_KEY_NOT_AVAILABLE
            );
    #endif
          RetVal = CRYPTO_E_KEY_NOT_AVAILABLE;
        }
        else if (
                 (
                  *ResultLengthPtr <
                  Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[keyElementIndex]->Size
                 ) &&
                 (Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[keyElementIndex]->AllowPartialAccess ==
                  FALSE
                 )
                )
        {
          /* !LINKSTO SWS_Crypto_00093_CORRECTION, 1 */
    #if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
          (void) Det_ReportError
          (
           CRYPTO_XVIX_XAIX_MODULE_ID,
           CRYPTO_XVIX_XAIX_INSTANCE_ID,
           CRYPTO_XVIX_XAIX_SID_KEYELEMENTGET,
           CRYPTO_XVIX_XAIX_E_RE_SMALL_BUFFER
          );
    #endif
          RetVal = CRYPTO_E_SMALL_BUFFER;
        }
        else if (
                 (
                  Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[keyElementIndex]->ReadAccess ==
                  CRYPTO_XVIX_XAIX_RA_DENIED
                 ) ||
                 (
                  Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[keyElementIndex]->ReadAccess ==
                  CRYPTO_XVIX_XAIX_RA_INTERNAL_COPY
                 )
                )
        {
          /* Reading the keyElement is not allowed by the configuration */
          RetVal = CRYPTO_E_KEY_READ_FAIL;
        }
        else
        {
          /* !LINKSTO EB_Crypto_00009, 1
          */
          RetVal = Crypto_xVIx_xAIx_PL_KeyElementGet(
                                                     CryptoKeyId,
                                                     KeyElementId,
                                                     ResultPtr,
                                                     ResultLengthPtr
                                                    );
        }
      }
    }
  #if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
    if (RetVal == CRYPTO_E_KEY_NOT_AVAILABLE)
    {
      /* !LINKSTO SWS_Crypto_00140, 2
      */
      (void) Det_ReportError
      (
       CRYPTO_XVIX_XAIX_MODULE_ID,
       CRYPTO_XVIX_XAIX_INSTANCE_ID,
       CRYPTO_XVIX_XAIX_SID_KEYELEMENTGET,
       CRYPTO_XVIX_XAIX_E_RE_KEY_NOT_AVAILABLE
      );
    }
    if (RetVal == CRYPTO_E_KEY_READ_FAIL)
    {
      /* !LINKSTO SWS_Crypto_00139, 2
      */
      (void) Det_ReportError
      (
       CRYPTO_XVIX_XAIX_MODULE_ID,
       CRYPTO_XVIX_XAIX_INSTANCE_ID,
       CRYPTO_XVIX_XAIX_SID_KEYELEMENTGET,
       CRYPTO_XVIX_XAIX_E_RE_KEY_READ_FAIL
      );
    }
  #endif
  }

  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_KeyElementCopy]-----------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/


 /* !LINKSTO SWS_Crypto_00148, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyElementCopy
(
  uint32 CryptoKeyId,
  uint32 KeyElementId,
  uint32 TargetCryptoKeyId,
  uint32 TargetKeyElementId
)
{
  Std_ReturnType RetVal                    = E_NOT_OK;

  Crypto_xVIx_xAIx_DriverObject* DriverObj = NULL_PTR;
  uint32 ObjectId                          = 0U;
  uint32 SourceKeyElementIndex;
  uint32 TargetKeyElementIndex;

#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  if (Crypto_xVIx_xAIx_Initialized == FALSE)
  {
    /* !LINKSTO SWS_Crypto_00149, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTCOPY,
     CRYPTO_XVIX_XAIX_E_UNINIT
    );
    RetVal = E_NOT_OK;
  }
  else if (CryptoKeyId >= CRYPTO_XVIX_XAIX_KEY_COUNT)
  {
    /* !LINKSTO SWS_Crypto_00150, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTCOPY,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else if (TargetCryptoKeyId >= CRYPTO_XVIX_XAIX_KEY_COUNT)
  {
    /* !LINKSTO SWS_Crypto_00151, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTCOPY,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else
#endif
  {
    /* Check if crypto driver is busy */
    DriverObj = &Crypto_xVIx_xAIx_DriverObjects[ObjectId];
    SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    if (DriverObj->CurrentJob != NULL_PTR)
    {
      if (DriverObj->CurrentJob->cryptoKeyId == TargetCryptoKeyId)
      {
        RetVal = CRYPTO_E_BUSY;
      }
    }
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    if(CRYPTO_E_BUSY != RetVal)
    {
      /* Find referenced source keyElement within the cryptoKey */
      for(
           SourceKeyElementIndex = 0U;
           SourceKeyElementIndex < Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements;
           SourceKeyElementIndex++
         )
      {
        if (Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[SourceKeyElementIndex]->Id == KeyElementId)
        {
          break;
        }
      }

      if (SourceKeyElementIndex == Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements)
      {
#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO SWS_Crypto_00152, 1 */
        (void) Det_ReportError
        (
         CRYPTO_XVIX_XAIX_MODULE_ID,
         CRYPTO_XVIX_XAIX_INSTANCE_ID,
         CRYPTO_XVIX_XAIX_SID_KEYELEMENTCOPY,
         CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
        );
        RetVal = E_NOT_OK;
#else
        /* Referenced source keyElement is not contained within the cryptoKey */
        RetVal = CRYPTO_E_KEY_NOT_AVAILABLE;
#endif
      }
      else if(
               Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[SourceKeyElementIndex]->ReadAccess ==
               CRYPTO_XVIX_XAIX_RA_DENIED
             )
      {
        /* Reading the source keyElement is not allowed by the configuration */
        RetVal = CRYPTO_E_KEY_READ_FAIL;
      }
      else
      {
        /* Find referenced target keyElement within the cryptoKey */
        for(
             TargetKeyElementIndex = 0U;
             TargetKeyElementIndex < Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyElements;
             TargetKeyElementIndex++
           )
        {
          if(
              Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyType[TargetKeyElementIndex]->Id ==
              TargetKeyElementId
            )
          {
            break;
          }
        }

        if (TargetKeyElementIndex == Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyElements)
        {
#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
          /* !LINKSTO SWS_Crypto_00153, 1 */
          (void) Det_ReportError
          (
           CRYPTO_XVIX_XAIX_MODULE_ID,
           CRYPTO_XVIX_XAIX_INSTANCE_ID,
           CRYPTO_XVIX_XAIX_SID_KEYELEMENTCOPY,
           CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
          );
          RetVal = E_NOT_OK;
#else
          /* Referenced target keyElement is not contained within the cryptoKey */
          RetVal = CRYPTO_E_KEY_NOT_AVAILABLE;
#endif
        }
        else if
        (
          (
            Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyType[TargetKeyElementIndex]->WriteAccess ==
            CRYPTO_XVIX_XAIX_WA_DENIED
          ) ||
          (
            (
              Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[SourceKeyElementIndex]->ReadAccess ==
              CRYPTO_XVIX_XAIX_RA_INTERNAL_COPY
            ) &&
            (
              Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyType[TargetKeyElementIndex]->ReadAccess ==
              CRYPTO_XVIX_XAIX_RA_ALLOWED
            )
          )
        )
        {
          /* Writing the target keyElement is not allowed by the configuration */
          RetVal = CRYPTO_E_KEY_WRITE_FAIL;
        }
        else
        {
          /* !LINKSTO EB_Crypto_00010, 1, Crypto.Req.Adjustments/SWS_Crypto_00154,1 */
          RetVal = Crypto_xVIx_xAIx_PL_KeyElementCopy(
                                                      CryptoKeyId,
                                                      KeyElementId,
                                                      TargetCryptoKeyId,
                                                      TargetKeyElementId
                                                     );
        }
      }
    }
  }
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_KeyCopy]------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO SWS_Crypto_00155, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyCopy
(
  uint32 CryptoKeyId,
  uint32 TargetCryptoKeyId
)
{
  Std_ReturnType RetVal                    = E_NOT_OK;

  Crypto_xVIx_xAIx_DriverObject* DriverObj = NULL_PTR;
  uint32 ObjectId                          = 0U;
  uint32 i                                 = 0U;
  uint32 j                                 = 0U;
#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  if (Crypto_xVIx_xAIx_Initialized == FALSE)
  {
    /* !LINKSTO SWS_Crypto_00156, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYCOPY,
     CRYPTO_XVIX_XAIX_E_UNINIT
    );
    RetVal = E_NOT_OK;
  }
  else if (CryptoKeyId >= CRYPTO_XVIX_XAIX_KEY_COUNT)
  {
    /* !LINKSTO SWS_Crypto_00157, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYCOPY,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else if (TargetCryptoKeyId >= CRYPTO_XVIX_XAIX_KEY_COUNT)
  {
    /* !LINKSTO SWS_Crypto_00158, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYCOPY,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else
#endif
  {
    /* Check if crypto driver is busy */

    DriverObj = &Crypto_xVIx_xAIx_DriverObjects[ObjectId];
    SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    if (DriverObj->CurrentJob != NULL_PTR)
    {
      if (DriverObj->CurrentJob->cryptoKeyId == TargetCryptoKeyId)
      {
        RetVal = CRYPTO_E_BUSY;
      }
    }
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();

    if(CRYPTO_E_BUSY != RetVal)
    {
      RetVal = E_OK;
      /* check that all the elements in the source key are readable. */
      for (i = 0U; i < Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements; i++)
      {
        if (Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[i]->ReadAccess == CRYPTO_XVIX_XAIX_RA_DENIED)
        {
          /* Reading the source keyElement is not allowed by the configuration */
          RetVal = CRYPTO_E_KEY_READ_FAIL;
          break;
        }
        else
        {
          /* check that all the elements in the target key are writeable. */
          for (j = 0U; j < Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyElements; j++)
          {
            if (
                 Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[i]->Id ==
                 Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyType[j]->Id
               )
            {
              if(
                  (
                    Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyType[j]->WriteAccess ==
                    CRYPTO_XVIX_XAIX_WA_DENIED
                  ) ||
                  (
                    (
                      Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[i]->ReadAccess ==
                      CRYPTO_XVIX_XAIX_RA_INTERNAL_COPY
                    ) &&
                    (
                      Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyType[j]->ReadAccess ==
                      CRYPTO_XVIX_XAIX_RA_ALLOWED
                    )
                  )
                )
              {
                /* Writing the target keyElement is not allowed by the configuration */
                RetVal = CRYPTO_E_KEY_WRITE_FAIL;
              }
              break;
            }
          }
          /* source key element was not found in target key */
          if(j == Crypto_xVIx_xAIx_Keys[TargetCryptoKeyId].KeyElements)
          {
            RetVal = CRYPTO_E_KEY_NOT_AVAILABLE;
            break;
          }
        }
      }
      if(E_OK == RetVal)
      {
        /* !LINKSTO EB_Crypto_00011, 1, Crypto.Req.Adjustments/SWS_Crypto_00159,1 */
        RetVal = Crypto_xVIx_xAIx_PL_KeyCopy(CryptoKeyId, TargetCryptoKeyId);
      }
    }
  }
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_KeyElementIdsGet]---------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO SWS_Crypto_00160_CORRECTION, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyElementIdsGet
(
  uint32                                               CryptoKeyId,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyElementIdsPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyElementIdsLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 keyElementIndex = 0U;
#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  if (Crypto_xVIx_xAIx_Initialized == FALSE)
  {
    /* !LINKSTO SWS_Crypto_00161, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTIDSGET,
     CRYPTO_XVIX_XAIX_E_UNINIT
    );
    RetVal = E_NOT_OK;
  }
  else if (CryptoKeyId >= CRYPTO_XVIX_XAIX_KEY_COUNT)
  {
    /* !LINKSTO SWS_Crypto_00162, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTIDSGET,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else if (*KeyElementIdsLengthPtr < Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements)
  {
    /* !LINKSTO SWS_Crypto_00164_CORRECTION, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYELEMENTIDSGET,
     CRYPTO_XVIX_XAIX_E_RE_SMALL_BUFFER
    );
    RetVal = CRYPTO_E_SMALL_BUFFER;
  }
  else
#endif
  {
    /* Read the KeyElementIds that are referenced by the key */
    for (
         keyElementIndex = 0U;
         keyElementIndex < Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements;
         keyElementIndex++
        )
    {
      KeyElementIdsPtr[keyElementIndex] = Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyType[keyElementIndex]->Id;
    }
    *KeyElementIdsLengthPtr = Crypto_xVIx_xAIx_Keys[CryptoKeyId].KeyElements;
    RetVal = E_OK;
  }
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_RandomSeed]---------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO SWS_Crypto_91013, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_RandomSeed
(
  uint32                                                CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) EntropyPtr,
  uint32                                                EntropyLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  if (Crypto_xVIx_xAIx_Initialized == FALSE)
  {
    /* !LINKSTO SWS_Crypto_00128, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_RANDOMSEED,
     CRYPTO_XVIX_XAIX_E_UNINIT
    );
    RetVal = E_NOT_OK;
  }
  else if (CryptoKeyId >= CRYPTO_XVIX_XAIX_KEY_COUNT)
  {
    /* !LINKSTO SWS_Crypto_00129, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_RANDOMSEED,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else if (EntropyPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Crypto_00130_CORRECTION, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_RANDOMSEED,
     CRYPTO_XVIX_XAIX_E_PARAM_POINTER
    );
    RetVal = E_NOT_OK;
  }
  else if (EntropyLength == 0U)
  {
    /* !LINKSTO SWS_Crypto_00131_CORRECTION, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_RANDOMSEED,
     CRYPTO_XVIX_XAIX_E_PARAM_VALUE
    );
    RetVal = E_NOT_OK;
  }
  else
#endif
  {
    /* !LINKSTO EB_Crypto_00013, 1 */
    RetVal = Crypto_xVIx_xAIx_PL_RandomSeed(CryptoKeyId, EntropyPtr, EntropyLength);
  }
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_KeyGenerate]--------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/


/* !LINKSTO SWS_Crypto_91007, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyGenerate
(
  uint32 CryptoKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  if (Crypto_xVIx_xAIx_Initialized == FALSE)
  {
    /* !LINKSTO SWS_Crypto_00094, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYGENERATE,
     CRYPTO_XVIX_XAIX_E_UNINIT
    );
    RetVal = E_NOT_OK;
  }
  else if (CryptoKeyId >= CRYPTO_XVIX_XAIX_KEY_COUNT)
  {
    /* !LINKSTO SWS_Crypto_00095, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYGENERATE,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else
#endif
  {
    /* !LINKSTO EB_Crypto_00014, 1 */
    /* !LINKSTO Crypto.Req.Adjustments/SWS_Crypto_00165, 1 */
    RetVal = Crypto_xVIx_xAIx_PL_KeyGenerate(CryptoKeyId);
  }
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_KeyDerive]----------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO SWS_Crypto_91008, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyDerive
(
  uint32 CryptoKeyId,
  uint32 TargetCryptoKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  if (FALSE == Crypto_xVIx_xAIx_Initialized)
  {
    /* !LINKSTO SWS_Crypto_00097, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYDERIVE,
     CRYPTO_XVIX_XAIX_E_UNINIT
    );
    RetVal = E_NOT_OK;
  }
  else if (CryptoKeyId >= CRYPTO_XVIX_XAIX_KEY_COUNT)
  {
    /* !LINKSTO SWS_Crypto_00098, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYDERIVE,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else if (CRYPTO_XVIX_XAIX_KEY_COUNT <= TargetCryptoKeyId)
  {
    /* !LINKSTO SWS_Crypto_00180, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYDERIVE,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else
#endif
  {
    /* !LINKSTO EB_Crypto_00015, 1 */
    /* !LINKSTO Crypto.Req.Adjustments/SWS_Crypto_00166, 1 */
    RetVal = Crypto_xVIx_xAIx_PL_KeyDerive(CryptoKeyId, TargetCryptoKeyId);
  }
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_KeyExchangeCalcPubVal]----------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO SWS_Crypto_91009, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyExchangeCalcPubVal
(
  uint32                                               CryptoKeyId,
  P2VAR(uint8,  AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PublicValueLengthPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  if (Crypto_xVIx_xAIx_Initialized == FALSE)
  {
    /* !LINKSTO SWS_Crypto_00103, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCPUBVAL,
     CRYPTO_XVIX_XAIX_E_UNINIT
    );
    RetVal = E_NOT_OK;
  }
  else if (CryptoKeyId >= CRYPTO_XVIX_XAIX_KEY_COUNT)
  {
    /* !LINKSTO SWS_Crypto_00104, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCPUBVAL,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else if (PublicValuePtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Crypto_00105, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCPUBVAL,
     CRYPTO_XVIX_XAIX_E_PARAM_POINTER
    );
    RetVal = E_NOT_OK;
  }
  else if (PublicValueLengthPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Crypto_00106_CORRECTION, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCPUBVAL,
     CRYPTO_XVIX_XAIX_E_PARAM_POINTER
    );
    RetVal = E_NOT_OK;
  }
  else if (*PublicValueLengthPtr == 0U)
  {
    /* !LINKSTO SWS_Crypto_00107_CORRECTION, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCPUBVAL,
     CRYPTO_XVIX_XAIX_E_PARAM_VALUE
    );
    RetVal = E_NOT_OK;
  }
  else
#endif
  {
    /* !LINKSTO EB_Crypto_00016, 1 */
    /* !LINKSTO Crypto.Req.Adjustments/SWS_Crypto_00167, 1 */
    RetVal = Crypto_xVIx_xAIx_PL_KeyExchangeCalcPubVal(
                                                       CryptoKeyId,
                                                       PublicValuePtr,
                                                       PublicValueLengthPtr
                                                      );
  }

#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  if (RetVal == CRYPTO_E_SMALL_BUFFER)
  {
    /* !LINKSTO SWS_Crypto_00110, 2 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCPUBVAL,
     CRYPTO_XVIX_XAIX_E_RE_SMALL_BUFFER
    );
    RetVal = CRYPTO_E_SMALL_BUFFER;
  }
#endif
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_KeyExchangeCalcSecret]--------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/


/* !LINKSTO SWS_Crypto_91010, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_KeyExchangeCalcSecret
(
  uint32                                      CryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) PartnerPublicValuePtr,
  uint32                                      PartnerPublicValueLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  if (Crypto_xVIx_xAIx_Initialized == FALSE)
  {
    /* !LINKSTO SWS_Crypto_00111, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCSECRET,
     CRYPTO_XVIX_XAIX_E_UNINIT
    );
    RetVal = E_NOT_OK;
  }
  else if (CryptoKeyId >= CRYPTO_XVIX_XAIX_KEY_COUNT)
  {
    /* !LINKSTO SWS_Crypto_00112, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCSECRET,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else if (PartnerPublicValuePtr == NULL_PTR)
  {
    /* !LINKSTO SWS_Crypto_00113, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCSECRET,
     CRYPTO_XVIX_XAIX_E_PARAM_POINTER
    );
    RetVal = E_NOT_OK;
  }
  else if (PartnerPublicValueLength == 0U)
  {
    /* !LINKSTO SWS_Crypto_00115_CORRECTION, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCSECRET,
     CRYPTO_XVIX_XAIX_E_PARAM_VALUE
    );
    RetVal = E_NOT_OK;
  }
  else
#endif
  {
    /* !LINKSTO EB_Crypto_00017, 1 */
    RetVal = Crypto_xVIx_xAIx_PL_KeyExchangeCalcSecret(
                                                       CryptoKeyId,
                                                       PartnerPublicValuePtr,
                                                       PartnerPublicValueLength
                                                      );
  }
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_CertificateParse]---------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO SWS_Crypto_91011, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CertificateParse
(
  uint32 CryptoKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON)
  uint32 ObjectId = 0U;
  Crypto_xVIx_xAIx_DriverObject* DriverObj = NULL_PTR;
#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  if (FALSE == Crypto_xVIx_xAIx_Initialized)
  {
    /* !LINKSTO SWS_Crypto_00168, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_CERTIFICATEPARSE,
     CRYPTO_XVIX_XAIX_E_UNINIT
    );
    RetVal = E_NOT_OK;
  }
  else if (CRYPTO_XVIX_XAIX_KEY_COUNT <= CryptoKeyId)
  {
    /* !LINKSTO SWS_Crypto_00169, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_CERTIFICATEPARSE,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else
#endif /* #if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Check if crypto driver is busy */
    /* !LINKSTO SWS_Crypto_91011, 1
    */
    DriverObj = &Crypto_xVIx_xAIx_DriverObjects[ObjectId];
    SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    if (DriverObj->CurrentJob != NULL_PTR)
    {
      if (DriverObj->CurrentJob->cryptoKeyId == CryptoKeyId)
      {
        RetVal = CRYPTO_E_BUSY;
      }
    }
    SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();


    if (CRYPTO_E_BUSY != RetVal)
    {
      /* !LINKSTO EB_Crypto_00018, 1 */
      /* !LINKSTO SWS_Crypto_00170_CORRECTION, 1 */
      /* !LINKSTO Crypto.Req.Adjustments/SWS_Crypto_00170, 1 */
      RetVal = Crypto_xVIx_xAIx_PL_CertificateParse(CryptoKeyId);
    }
  }
#else
  TS_PARAM_UNUSED(CryptoKeyId);
#endif /* #if (CRYPTO_XVIX_XAIX_CERT_MANAGEMENT_ENABLED == STD_ON) */
  return RetVal;
}

/*------------------------------------------------------------------------------------------------*/

/*----[Crypto_xVIx_xAIx_CertificateVerify]--------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/

/* !LINKSTO SWS_Crypto_00171, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CertificateVerify
(
  uint32                                                                CryptoKeyId,
  uint32                                                                VerifyCryptoKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) VerifyPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if (CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT == STD_ON)
  if (FALSE == Crypto_xVIx_xAIx_Initialized)
  {
    /* !LINKSTO SWS_Crypto_00172, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_CERTIFICATEVERIFY,
     CRYPTO_XVIX_XAIX_E_UNINIT
    );
    RetVal = E_NOT_OK;
  }
  else if (
           (CRYPTO_XVIX_XAIX_KEY_COUNT <= CryptoKeyId      ) ||
           (CRYPTO_XVIX_XAIX_KEY_COUNT <= VerifyCryptoKeyId)
          )
  {
    /* !LINKSTO SWS_Crypto_00173, 1 */
    /* !LINKSTO SWS_Crypto_00174_CORRECTION, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_CERTIFICATEVERIFY,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
  else if (NULL_PTR == VerifyPtr)
  {
    /* !LINKSTO SWS_Crypto_00175, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_CERTIFICATEVERIFY,
     CRYPTO_XVIX_XAIX_E_PARAM_POINTER
    );
    RetVal = E_NOT_OK;
  }

#if (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON)
  else if (E_NOT_OK == Crypto_xVIx_xAIx_CertificateCurrentDateVerify(VerifyCryptoKeyId))
  {
    /* !LINKSTO SWS_Crypto_00176, 1 */
    (void) Det_ReportError
    (
     CRYPTO_XVIX_XAIX_MODULE_ID,
     CRYPTO_XVIX_XAIX_INSTANCE_ID,
     CRYPTO_XVIX_XAIX_SID_CERTIFICATEVERIFY,
     CRYPTO_XVIX_XAIX_E_PARAM_HANDLE
    );
    RetVal = E_NOT_OK;
  }
#endif

  else
#endif
  {
    /* !LINKSTO EB_Crypto_00019, 1 */
    /* !LINKSTO Crypto.Req.Adjustments/SWS_Crypto_00177, 1 */
    RetVal = Crypto_xVIx_xAIx_PL_CertificateVerify(CryptoKeyId, VerifyCryptoKeyId, VerifyPtr);
  }
  return RetVal;
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==================[internal function definitions]===============================================*/


#if  (CRYPTO_XVIX_XAIX_CERT_CURRENT_TIME_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CertificateCurrentDateVerify
(
  uint32 VerifyCryptoKeyId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8  CurrDate[CRYPTO_XVIX_XAIX_DATE_SIZE] = {0U,0U,0U,0U,0U,0U};
  uint32 CurrDateSize = CRYPTO_XVIX_XAIX_DATE_SIZE;
  uint32 Year  = 0U;
  uint32 Month = 0U;
  uint32 Day   = 0U;

  if(
      E_OK == Crypto_xVIx_xAIx_AL_KeyElementGet
              (
               VerifyCryptoKeyId,
               CRYPTO_KE_CERTIFICATE_CURRENT_TIME,
               CurrDate,
               &CurrDateSize
              )
   )
  {
    /*convert the year, month and day in integer value*/
    Year  = (uint32) (uint8) ((CurrDate[0U] * 10U) + CurrDate[1U]);
    Month = (uint32) (uint8) ((CurrDate[2U] * 10U) + CurrDate[3U]);
    Day   = (uint32) (uint8) ((CurrDate[4U] * 10U) + CurrDate[5U]);
    if(
        (99U >= Year) &&
        ((0U < Month) && (13U > Month)) &&
        ((0U < Day) && (32U > Day))
      )

    {
      RetVal = E_OK;
    }
  }
  return RetVal;
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif

/*================================================================================================*/

#endif /* #if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON) */

/*==================[end of file]=================================================================*/
