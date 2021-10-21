/**
 * \file
 *
 * \brief AUTOSAR KeyM
 *
 * This file contains the implementation of the AUTOSAR
 * module KeyM.
 *
 * \version 1.2.11
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[deviations]==================================================================*/

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W588
 * W588: dead assignment to "KeyM_Initialized" eliminated
 *
 * Reason:
 * Setting "KeyM_Initialized" to false is needed to prevent functions being called if
 * reinitialization is interrupted.
 *
 * TASKING-2) Deviated Rule: W557
 * W557: possible infinite loop
 *
 * Reason:
 * Waiting state for CsmCallback which is called in asynchronous processing from different tasks.
 *
 * TASKING-3) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * This is a bug in the tasking compiler and should be fixed with v6.1r1.
 */

/*==================[includes]====================================================================*/

/* !LINKSTO KeyM.Dsn.Files/KeyM.c/0001,1 */

#include <TSAutosar.h>
#include <KeyM_Externals.h>
#include <KeyM_Int_Cfg.h>
#include <Csm.h>
#include <SchM_KeyM.h>
#include <KeyM.h>
#include <KeyM_X509.h>
#include <TSMem.h>

#if (KEYM_API_ENABLED_NVM == STD_ON)
#include <NvM.h>
#endif

#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
#include <Det.h>
#endif

/*==================[macros]======================================================================*/

#if (defined KEYM_CERT)
#error KEYM_CERT is already defined
#endif
/** \brief  Macro to shorten name lengths of certificates. **/
#define KEYM_CERT(id) (KeyM_Config.certificates[(id)])

#if (defined KEYM_KEY)
#error KEYM_KEY is already defined
#endif
/** \brief  Macro to shorten name lengths of keys. **/
#define KEYM_KEY(id) (KeyM_Config.keys[(id)])

#if (defined KEYM_UINT16_MAX)
#error KEYM_UINT16_MAX is already defined
#endif
/** \brief  Maximum value of uint16. **/
#define KEYM_UINT16_MAX 65535U

/*==================[type definitions]============================================================*/

/** \brief  Structure definition for stack.
 **
 ** \var  stack
 ** \brief  Stores the certificate chain for further processing.
 **
 ** \var  top
 ** \brief  Stores the array index of the last entry in the chain.
 **/
typedef struct
{
  uint16                 top;
  KeyM_CertificateIdType stack[KEYM_CERT_MAX_DEPTH_CHAIN];
}
KeyM_StackType;

/*==================[internal function declarations]==============================================*/

/** \brief  Internal certificate set function.
 **
 ** This function handles the setting of a certificate.
 **
 ** \param[in]  certificateId   The identifier of the certificate the data shall be stored in.
 ** \param[in]  certDataPtr     A pointer to the data that shall be stored.
 ** \param[in]  certDataLength  The length of the data that shall be stored.
 **
 ** \returns  Result of the setting of the certificate.
 **
 ** \retval  E_OK                           Certificate was accepted.
 ** \retval  KEYM_E_PARAMETER_MISMATCH      Parameter do not match with expected value.
 ** \retval  KEYM_E_KEY_CERT_SIZE_MISMATCH  The referenced certificate is to small to
 **                                         store the given data.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
static FUNC(Std_ReturnType, KEYM_CODE) KeyM_InternalSetCertificate
(
          KeyM_CertificateIdType            certificateId,
  P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) certDataPtr,
          uint32                            certDataLength
);

/** \brief  Internal certificate verify function.
 **
 ** This function handles the verification of certificate.
 **
 ** \param[in]  certificateId  The identifier of the certificate the data shall be stored in.
 **
 ** \returns  Result of the verification of the certificate.
 **
 ** \retval  E_OK                                Certificate verification request was accepted.
 **                                              Processing will be done in the background function.
 ** \retval  KEYM_E_PARAMETER_MISMATCH           Certificate ID is invalid.
 ** \retval  KEYM_E_KEY_CERT_EMPTY               One of the certificates of the chain is empty.
 ** \retval  KEYM_E_CERT_INVALID_CHAIN_OF_TRUST  An upper certificate is not valid.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
static FUNC(Std_ReturnType, KEYM_CODE) KeyM_InternalVerifyCertificate
(
  KeyM_CertificateIdType certificateId
);

/** \brief  Compares two buffers of a given length.
 **
 ** This function compares the two given buffers. The lengths are also compared.
 **
 ** \param[in]  left         Pointer to the first input buffer.
 ** \param[in]  leftLength   Length of the first input buffer.
 ** \param[in]  right        Pointer to the second input buffer.
 ** \param[in]  rightLength  Length of the second input buffer.
 **
 ** \returns  Result of comparison.
 **
 ** \retval  E_OK      Buffers have the same content and the same length.
 ** \retval  E_NOT_OK  Buffers do not have the same content or length is different.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
static FUNC(Std_ReturnType, KEYM_CODE) KeyM_BufferCmp
(
  P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) left,
          uint32                            leftLength,
  P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) right,
          uint32                            rightLength
);

/** \brief  Compares two x509_names.
 **
 ** This function compares the two given x509 names. The compared parts are country, organization,
 ** organization unit, state, locality, common name and serialNumber.
 **
 ** \param[in]  leftPtr      Pointer to the first x509 name.
 ** \param[in]  leftLength   Length of the first x509 name.
 ** \param[in]  rightPtr     Pointer to the second x509 name.
 ** \param[in]  rightLength  Length of the second x509 name.
 **
 ** \returns  Result of comparison.
 **
 ** \retval  E_OK      Names are equal.
 ** \retval  E_NOT_OK  Names are not equal.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
static FUNC(Std_ReturnType, KEYM_CODE) KeyM_X509NameCmp
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) leftPtr,
        uint32                              leftLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) rightPtr,
        uint32                              rightLength
);

/** \brief  Verify a certificate.
 **
 ** This function checks if the issuer of the certificate is equal to the subject of the upper
 ** layer certificate and then verifies the signature.
 **
 ** \param[in]  certificateId  Identifier of the certificate to verify.
 **
 ** \returns  Result of verification.
 **
 ** \retval  E_OK      Verification successful.
 ** \retval  E_NOT_OK  Verification not successful.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON)
static FUNC(Std_ReturnType, KEYM_CODE) KeyM_CompareAndVerify
(
  KeyM_CertificateIdType certificateId
);
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON) */

/** \brief  Push next item on the stack.
 **
 ** This functions pushes the id of a certificate to the stack. This is needed to iterate
 ** through the certificate chain.
 **
 ** \param[in]  item  Identifier of the certificate to push on the stack.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
static FUNC(void, KEYM_CODE) KeyM_Push
(
  KeyM_CertificateIdType item
);

/** \brief  Pop last item from the stack.
 **
 ** This functions gets the id of the certificate that was pushed on the stack last.
 **
 ** \param[out]  item  Pointer to the location where the id of the certificate shall be stored.
 **
 ** \returns  Result of poping off stack.
 **
 ** \retval  E_OK      Id of certificate was successfully poped from stack.
 ** \retval  E_NOT_OK  Stack is empty.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
static FUNC(Std_ReturnType, KEYM_CODE) KeyM_Pop
(
  P2VAR(KeyM_CertificateIdType, AUTOMATIC, CRYPTO_APPL_DATA) item
);

/** \brief  Helper function of the MainBackgroundFunction for initialization. **/
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON)
static FUNC(void, KEYM_CODE) KeyM_InitHelper
(
  void
);
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON) */

/** \brief  Helper function of the MainBackgroundFunction for verification. **/
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON)
static FUNC(void, KEYM_CODE) KeyM_VerifyHelper
(
  void
);
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON) */

/** \brief Helper function for MainFunction to perform initial read **/
#if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON))
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_NvMRead
(
  void
);
#endif /* #if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON)) */

/** \brief Helper function for MainFunction to check NvM read error status **/
#if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON))
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_NvMReadErrorStatus
(
  void
);
#endif /* #if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON)) */

/** \brief Helper function for MainFunction to store certificates to NvM **/
#if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON))
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyStoreNvM
(
  void
);
#endif /* #if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON)) */

/** \brief Helper function for MainFunction to check NvM write error status **/
#if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON))
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_NvMWriteErrorStatus
(
  void
);
#endif /* #if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON)) */

/** \brief Helper function for MainFunction to initialize the verification steps **/
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF)
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyInit
(
  void
);
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) */

/** \brief Helper function for MainFunction to perform parsing inside a requested verification **/
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF)
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyParse
(
  void
);
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) */

/** \brief Helper function for MainFunction to prepare the verification of a root certificate **/
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF)
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyPrepRoot
(
  void
);
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) */

/** \brief Helper function for MainFunction to perform the verification **/
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF)
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyVerify
(
  void
);
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) */

/** \brief Helper function for MainFunction to process errors in verification **/
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF)
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyError
(
  void
);
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) */

/** \brief Helper function for MainFunction to store the public key **/
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF)
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyStorePublicKey
(
  void
);
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) */

/** \brief Helper function for MainFunction to wait for a Csm callback **/
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF)
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyWaitCsmCallback
(
  void
);
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) */

/** \brief Helper function for MainFunction to finalize the verification **/
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF)
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyFinish
(
  void
);
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) */

/** \brief Helper function for MainFunction to perform a initial parsing **/
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF)
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_PreparseParse
(
  void
);
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) */

/** \brief  Helper function of the Main(Background)Function for parsing. **/
static FUNC(void, KEYM_CODE) KeyM_ParseHelper
(
  KeyM_CertificateIdType certificateId
);

/** \brief  Helper function for verification of a certificate against the configured certificate
 **         rules.
 **
 ** \param[in]  certificateId  Identifier of the certificate to verify.
 **
 ** \returns  Result of the verification.
 **
 ** \retval  TRUE   Verification successful.
 ** \retval  FALSE  Verification not successful.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
static FUNC(boolean, KEYM_CODE) KeyM_CertElemVerification_Helper
(
  KeyM_CertificateIdType certificateId
);

/*==================[internal constants]==========================================================*/

/*==================[internal data]===============================================================*/

#define KEYM_START_SEC_VAR_INIT_BOOLEAN
#include <KeyM_MemMap.h>

#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
/** \brief  Variable that represents whether the KeyM module is fully initialized. **/
/* !LINKSTO KeyM.Dsn.Api/Datas/0001,1 */
static VAR(boolean, KEYM_VAR) KeyM_Initialized        = FALSE;
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */

/** \brief  Flag if the Csm Callback was called. **/
static VAR(boolean, KEYM_VAR) KeyM_CsmCallback        = FALSE;

/** \brief Flag indicates if verification was requested. **/
static VAR(boolean, KEYM_VAR) KeyM_FlgVerifyRequested = FALSE;

#define KEYM_STOP_SEC_VAR_INIT_BOOLEAN
#include <KeyM_MemMap.h>

#define KEYM_START_SEC_VAR_INIT_UNSPECIFIED
#include <KeyM_MemMap.h>

/** \brief  The State of the KeyM. **/
/* !LINKSTO KeyM.Dsn.Api/Datas/0004,1 */
static VAR(KeyM_StateType, KEYM_VAR) KeyM_State = KEYM_STATE_UNINIT;

/** \brief  Stores the certificate chain for further processing. **/
static VAR(KeyM_StackType, KEYM_VAR) KeyM_Stack = { 0U, { 0U } };

/** \brief Stores the current id which has to be verified by the KeyM_MainFunction. **/
/* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00050,1 */
static VAR(KeyM_CertificateIdType, KEYM_VAR) KeyM_CertVrfyIdCur      = 0U;

/** \brief Stores the id of the certificate which has to be parsed next inside the chain
 **        by the KeyM_MainFunction.
 **/
/* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00051,1 */
static VAR(KeyM_CertificateIdType, KEYM_VAR) KeyM_CertVrfyChainIdCur = 0U;

/** \brief Stores the current id which has to be parsed by the KeyM_MainFunction. **/
/* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00150,1 */
static VAR(KeyM_CertificateIdType, KEYM_VAR) KeyM_CertParseIdCur     = 0U;

/** \brief Global callback value used for async callbacks in KeyM_MainFunction. **/
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF)
static VAR(KeyM_ResultType, KEYM_VAR) KeyM_CallbackValue                 = KEYM_RT_NOT_OK;

/** \brief Verification result stored in between several states of KeyM_MainFunction. **/
static VAR(Crypto_VerifyResultType, KEYM_VAR) KeyM_SignatureVerifyResult = CRYPTO_E_VER_NOT_OK;

/** \brief  The current state of the KeyM_MainFunction. **/
/* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00002,1 */
#if (KEYM_API_ENABLED_NVM == STD_ON)
static VAR(KeyM_MainFunction_StateType, KEYM_VAR) KeyM_MainFunction_State = KEYM_MFUNC_STATE_NVMREAD;
#else /* #if (KEYM_API_ENABLED_NVM == STD_ON) */
static VAR(KeyM_MainFunction_StateType, KEYM_VAR) KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_INIT;
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) */

#define KEYM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <KeyM_MemMap.h>

/*==================[external constants]==========================================================*/

/*==================[external data]===============================================================*/

/*==================[external function definitions]===============================================*/

#define KEYM_START_SEC_CODE
#include <KeyM_MemMap.h>

/* --- KeyM_Init -------------------------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00043,1 */

FUNC(void, KEYM_CODE) KeyM_Init
(
  P2CONST(KeyM_ConfigType, AUTOMATIC, KEYM_APPL_DATA) ConfigPtr
)
{
  uint32  i;
  boolean doInit = FALSE;

  TS_PARAM_UNUSED(ConfigPtr);

  /* !LINKSTO KeyM.Dsn.Api/ExAreas/0001,1 */
  SchM_Enter_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();
  if (KEYM_STATE_INIT != KeyM_State)
  {
    doInit     = TRUE;
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_Init/00002,1 */
    KeyM_State = KEYM_STATE_INIT;
  }
  SchM_Exit_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();

  if (doInit)
  {
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
    /* Deviation TASKING-1 */
    KeyM_Initialized        = FALSE;
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
    KeyM_FlgVerifyRequested = FALSE;
    KeyM_CertVrfyIdCur      = 0U;
    KeyM_CertVrfyChainIdCur = 0U;
    KeyM_CertParseIdCur     = 0U;
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF)
/* !LINKSTO KeyM.Dsn.Alg/KeyM_Init_Async/00001,1 */
/* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00002,1 */
#if (KEYM_API_ENABLED_NVM == STD_ON)
    KeyM_MainFunction_State = KEYM_MFUNC_STATE_NVMREAD;
#else /* #if (KEYM_API_ENABLED_NVM == STD_ON) */
    KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_INIT;
    KeyM_State              = KEYM_STATE_IDLE;
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) */

    /* !LINKSTO KeyM.Dsn.Alg/KeyM_Init/00001,1 */
    /* Deviation TASKING-3 */
    for (i = 0U; i < KEYM_CERT_COUNT; i++)
    {
      KEYM_CERT(i).data->certStatus        = KEYM_CERTIFICATE_NOT_AVAILABLE;
#if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON))
      KEYM_CERT(i).data->certNvMReadStatus = KEYM_CERTIFICATE_NVM_READ_NOTSET;
#endif /* #if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON)) */
    }
    KeyM_Stack.top = KEYM_UINT16_MAX;
    /* initialization is done in KeyM_MainBackgroundFunction/KeyM_MainFunction */

#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
    KeyM_Initialized = TRUE;
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  }
}

/* --- KeyM_Deinit ------------------------------------------------------------------------------ */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00047,1 */

FUNC(void, KEYM_CODE) KeyM_Deinit
(
  void
)
{
  uint32  i;
  uint32  j;
  uint8   k;
  boolean doDeinit = FALSE;

#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_DEINIT,
                           KEYM_E_UNINIT
                          );
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO KeyM.Dsn.Api/ExAreas/0001,1 */
    SchM_Enter_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();
    /* CHECK: NOPARSE */
    /* State is only set within this method internally, in a local static variable. It is */
    /* not possible to modify this state from outside that method. */
    if (KEYM_STATE_DEINIT != KeyM_State)
    {
      doDeinit   = TRUE;
      KeyM_State = KEYM_STATE_DEINIT;
    }
    SchM_Exit_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();
    if (doDeinit)
    {
    /* CHECK: PARSE */
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
      KeyM_Initialized = FALSE;
#endif
      /* Clean all certificate data */
      /* !LINKSTO SWS_KeyM_00048,2 */
      /* Deviation TASKING-3 */
      for (i = 0U; i < KEYM_CERT_COUNT; i++)
      {
        TS_MemBZero(KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).cnfg->keyData,
                    KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).data->keyLength
                   );
        for (j = 0U; j < KEYM_CERT(i).cnfg->certElementsNumber; j++)
        {
          k = 0U;
          do
          {
            KEYM_CERT(i).cnfg->certElements[j].data[k].dataLength = 0U;
            KEYM_CERT(i).cnfg->certElements[j].data[k].dataPtr    = NULL_PTR;
            k++;
          } while (k < KEYM_CERT(i).cnfg->certElements[j].cnfg->maxIterations);
        }
        KEYM_CERT(i).cnfg->certElementDataIssuer->dataLength         = 0U;
        KEYM_CERT(i).cnfg->certElementDataIssuer->dataPtr            = NULL_PTR;
        KEYM_CERT(i).cnfg->certElementDataSubject->dataLength        = 0U;
        KEYM_CERT(i).cnfg->certElementDataSubject->dataPtr           = NULL_PTR;
        KEYM_CERT(i).cnfg->certElementDataSignatureValue->dataLength = 0U;
        KEYM_CERT(i).cnfg->certElementDataSignatureValue->dataPtr    = NULL_PTR;
        KEYM_CERT(i).cnfg->certElementDataPublicKey->dataLength      = 0U;
        KEYM_CERT(i).cnfg->certElementDataPublicKey->dataPtr         = NULL_PTR;
        KEYM_CERT(i).cnfg->certTbsCertificate->dataLength            = 0U;
        KEYM_CERT(i).cnfg->certTbsCertificate->dataPtr               = NULL_PTR;
        KEYM_CERT(i).data->certStatus                                = KEYM_CERTIFICATE_NOT_AVAILABLE;
#if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON))
        KEYM_CERT(i).data->certNvMReadStatus                         = KEYM_CERTIFICATE_NVM_READ_NOTSET;
#endif
        KEYM_CERT(i).data->flgServiceRequested                       = FALSE;
        KEYM_CERT(i).data->flgParsed                                 = FALSE;
      }
      KeyM_Stack.top = KEYM_UINT16_MAX;
      KeyM_State     = KEYM_STATE_UNINIT;
    }
  }
}

/* --- KeyM_GetVersionInfo ---------------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00049,1 */

FUNC(void, KEYM_CODE) KeyM_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, KEYM_APPL_DATA) VersionInfo
)
{
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (NULL_PTR == VersionInfo)
  {
    /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00146,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_GETVERSIONINFO,
                           KEYM_E_PARAM_POINTER
                          );
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    VersionInfo->vendorID         = KEYM_VENDOR_ID;
    VersionInfo->moduleID         = KEYM_MODULE_ID;
    VersionInfo->sw_major_version = KEYM_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = KEYM_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = KEYM_SW_PATCH_VERSION;
  }
}

/* --- KeyM_Start ------------------------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00050,1 */
/* !LINKSTO SWS_KeyM_00001,1 */

#if (KEYM_API_ENABLED_START == STD_ON)
FUNC(Std_ReturnType, KEYM_CODE) KeyM_Start
(
          KeyM_StartType                     StartType,
  P2CONST(uint8,  AUTOMATIC, KEYM_APPL_DATA) RequestData,
          uint16                             RequestDataLength,
    P2VAR(uint8,  AUTOMATIC, KEYM_APPL_DATA) ResponseData,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) ResponseDataLength
)
{
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_START,
                           KEYM_E_UNINIT
                          );
  }
  else
  if (   (NULL_PTR == RequestData       )
      || (NULL_PTR == ResponseData      )
      || (NULL_PTR == ResponseDataLength)
     )
  {
    /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00146,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_START,
                           KEYM_E_PARAM_POINTER
                          );
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    TS_PARAM_UNUSED(StartType);
    TS_PARAM_UNUSED(RequestData);
    TS_PARAM_UNUSED(RequestDataLength);
    TS_PARAM_UNUSED(ResponseData);
    TS_PARAM_UNUSED(ResponseDataLength);
  }
  return E_NOT_OK;
}
#endif /* #if (KEYM_API_ENABLED_START == STD_ON) */

/* --- KeyM_Prepare ----------------------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00051,1 */
/* !LINKSTO SWS_KeyM_00001,1 */

#if (KEYM_API_ENABLED_PREPARE == STD_ON)
FUNC(Std_ReturnType, KEYM_CODE) KeyM_Prepare
(
  P2CONST(uint8,  AUTOMATIC, KEYM_APPL_DATA) RequestData,
          uint16                             RequestDataLength,
    P2VAR(uint8,  AUTOMATIC, KEYM_APPL_DATA) ResponseData,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) ResponseDataLength
)
{
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_PREPARE,
                           KEYM_E_UNINIT
                          );
  }
  else
  if (   (NULL_PTR == RequestData       )
      || (NULL_PTR == ResponseData      )
      || (NULL_PTR == ResponseDataLength)
     )
  {
    /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00146,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_PREPARE,
                           KEYM_E_PARAM_POINTER
                          );
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    TS_PARAM_UNUSED(RequestData);
    TS_PARAM_UNUSED(RequestDataLength);
    TS_PARAM_UNUSED(ResponseData);
    TS_PARAM_UNUSED(ResponseDataLength);
  }
  return E_NOT_OK;
}
#endif /* #if (KEYM_API_ENABLED_PREPARE == STD_ON) */

/* --- KeyM_Update ------------------------------------------------------------------------------ */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00052,1 */
/* !LINKSTO SWS_KeyM_00001,1 */

#if (KEYM_API_ENABLED_UPDATE == STD_ON)
FUNC(Std_ReturnType, KEYM_CODE) KeyM_Update
(
  P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) KeyNamePtr,
          uint16                            KeyNameLength,
  P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) RequestDataPtr,
          uint16                            RequestDataLength,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) ResultDataPtr,
          uint16                            ResultDataMaxLength
)
{
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_UPDATE,
                           KEYM_E_UNINIT
                          );
  }
  else
  if (   (NULL_PTR == KeyNamePtr    )
      || (NULL_PTR == RequestDataPtr)
      || (NULL_PTR == ResultDataPtr )
     )
  {
    /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00146,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_UPDATE,
                           KEYM_E_PARAM_POINTER
                          );
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    TS_PARAM_UNUSED(KeyNamePtr);
    TS_PARAM_UNUSED(KeyNameLength);
    TS_PARAM_UNUSED(RequestDataPtr);
    TS_PARAM_UNUSED(RequestDataLength);
    TS_PARAM_UNUSED(ResultDataPtr);
    TS_PARAM_UNUSED(ResultDataMaxLength);
  }
  return E_NOT_OK;
}
#endif /* #if (KEYM_API_ENABLED_UPDATE == STD_ON) */

/* --- KeyM_Finalize ---------------------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00053,1 */
/* !LINKSTO SWS_KeyM_00001,1 */

#if (KEYM_API_ENABLED_FINALIZE == STD_ON)
FUNC(Std_ReturnType, KEYM_CODE) KeyM_Finalize
(
  P2CONST(uint8,  AUTOMATIC, KEYM_APPL_DATA) RequestDataPtr,
          uint16                             RequestDataLength,
    P2VAR(uint8,  AUTOMATIC, KEYM_APPL_DATA) ResponseDataPtr,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) ResponseMaxDataLength
)
{
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_FINALIZE,
                           KEYM_E_UNINIT
                          );
  }
  else
  if (   (NULL_PTR == RequestDataPtr       )
      || (NULL_PTR == ResponseDataPtr      )
      || (NULL_PTR == ResponseMaxDataLength)
     )
  {
    /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00146,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_FINALIZE,
                           KEYM_E_PARAM_POINTER
                          );
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    TS_PARAM_UNUSED(RequestDataPtr);
    TS_PARAM_UNUSED(RequestDataLength);
    TS_PARAM_UNUSED(ResponseDataPtr);
    TS_PARAM_UNUSED(ResponseMaxDataLength);
  }
  return E_NOT_OK;
}
#endif /* #if (KEYM_API_ENABLED_FINALIZE == STD_ON) */

/* --- KeyM_Verify ------------------------------------------------------------------------------ */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00054,1 */
/* !LINKSTO SWS_KeyM_00001,1 */

#if (KEYM_API_ENABLED_VERIFY == STD_ON)
FUNC(Std_ReturnType, KEYM_CODE) KeyM_Verify
(
  P2CONST(uint8,  AUTOMATIC, KEYM_APPL_DATA) KeyNamePtr,
          uint16                             KeyNameLength,
  P2CONST(uint8,  AUTOMATIC, KEYM_APPL_DATA) RequestData,
          uint16                             RequestDataLength,
    P2VAR(uint8,  AUTOMATIC, KEYM_APPL_DATA) ResponseData,
    P2VAR(uint16, AUTOMATIC, KEYM_APPL_DATA) ResponseDataLength
)
{
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_VERIFY,
                           KEYM_E_UNINIT
                          );
  }
  else
  if (   (NULL_PTR == KeyNamePtr        )
      || (NULL_PTR == RequestData       )
      || (NULL_PTR == ResponseData      )
      || (NULL_PTR == ResponseDataLength)
     )
  {
    /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00146,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_VERIFY,
                           KEYM_E_PARAM_POINTER
                          );
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    TS_PARAM_UNUSED(KeyNamePtr);
    TS_PARAM_UNUSED(KeyNameLength);
    TS_PARAM_UNUSED(RequestData);
    TS_PARAM_UNUSED(RequestDataLength);
    TS_PARAM_UNUSED(ResponseData);
    TS_PARAM_UNUSED(ResponseDataLength);
  }
  return E_NOT_OK;
}
#endif /* #if (KEYM_API_ENABLED_VERIFY == STD_ON) */

/* --- KeyM_ServiceCertificate ------------------------------------------------------------------ */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00056,1 */
/* !LINKSTO SWS_KeyM_00021,1 */
/* !LINKSTO SWS_KeyM_00110,1 */
/* !LINKSTO SWS_KeyM_00001,1 */

#if (KEYM_API_ENABLED_SERVICECERTIFICATE == STD_ON)
FUNC(Std_ReturnType, KEYM_CODE) KeyM_ServiceCertificate
(
          KeyM_ServiceCertificateType       Service,
  P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) CertNamePtr,
          uint16                            CertNameLength,
  P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) RequestData,
          uint16                            RequestDataLength,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_DATA) ResponseData,
          uint16                            ResponseDataLength
)
{
  /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00002,1 */
  Std_ReturnType retVal               = E_NOT_OK;
  boolean        doServiceCertificate = FALSE;
  uint16         i                    = 0U;

#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_SERVICECERTIFICATE,
                           KEYM_E_UNINIT
                          );
  }
  else
  if (   (NULL_PTR == CertNamePtr )
      || (NULL_PTR == RequestData )
      || (NULL_PTR == ResponseData)
     )
  {
    /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00146,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_SERVICECERTIFICATE,
                           KEYM_E_PARAM_POINTER
                          );
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00015,1 */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00016,1 */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00017,1 */
    retVal = KEYM_E_PARAMETER_MISMATCH;
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    TS_PARAM_UNUSED(ResponseData);
    TS_PARAM_UNUSED(ResponseDataLength);

    /* !LINKSTO KeyM.Dsn.Api/ExAreas/0001,1 */
    SchM_Enter_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00013,2 */
    if (KEYM_STATE_IDLE != KeyM_State)
    {
      retVal = KEYM_E_BUSY;
    }
    else
    {
      doServiceCertificate = TRUE;
      KeyM_State           = KEYM_STATE_SERVICE_CERTIFICATE;
    }
    SchM_Exit_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();

    if (doServiceCertificate)
    {
      /* Search for certificate */
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00001,1 */
      /* Deviation TASKING-3 */
      while (KEYM_CERT_COUNT > i)
      {
        if (E_OK == KeyM_BufferCmp(CertNamePtr,
                                   CertNameLength,
                                   KEYM_CERT(i).cnfg->certName,
                                   KEYM_CERT(i).cnfg->certNameLength
                                  )
           )
        {
          /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00003,1 */
          if (   (   (KEYM_SERVICE_CERT_SET_ROOT     == Service                           )
                  && (i                              == KEYM_CERT(i).cnfg->certUpperCertId)
                  && (KEYM_CERTIFICATE_NOT_AVAILABLE == KEYM_CERT(i).data->certStatus     )
                 )
              || (   (KEYM_SERVICE_CERT_UPDATE_ROOT  == Service                           )
                  && (i                              == KEYM_CERT(i).cnfg->certUpperCertId)
                  && (KEYM_CERTIFICATE_NOT_AVAILABLE != KEYM_CERT(i).data->certStatus     )
                 )
              || (   (KEYM_SERVICE_CERT_SET_INTERMEDIATE == Service                           )
                  && (i                                  != KEYM_CERT(i).cnfg->certUpperCertId)
                  && (KEYM_CERTIFICATE_NOT_AVAILABLE     == KEYM_CERT(i).data->certStatus     )
                 )
              || (   (KEYM_SERVICE_CERT_UPDATE_INTERMEDIATE == Service                           )
                  && (i                                     != KEYM_CERT(i).cnfg->certUpperCertId)
                  && (KEYM_CERTIFICATE_NOT_AVAILABLE        != KEYM_CERT(i).data->certStatus     )
                 )
             )
          {
            /* First set certificate */
            /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00006,1 */
            retVal = KeyM_InternalSetCertificate(i, RequestData, RequestDataLength);
            if (0U != RequestDataLength)
            {
              /* This is the only error that it possible in this case. */
              /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00005,1 */
              if (KEYM_E_KEY_CERT_SIZE_MISMATCH == retVal)
              {
                retVal = KEYM_E_PARAMETER_MISMATCH;
              }
              else
              {
                /* validate */
                retVal = KeyM_InternalVerifyCertificate(i);
              }
              if (E_OK == retVal)
              {
                /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00012,1 */
                KEYM_CERT(i).data->flgServiceRequested = TRUE;
              }
              else
              {
                /* reset state */
                KeyM_State = KEYM_STATE_IDLE;
              }
            }
            else
            {
              /* reset state */
              KeyM_State = KEYM_STATE_IDLE;
            }
          }
          else
          {
            /* reset state */
            KeyM_State = KEYM_STATE_IDLE;
          }
          break;
        }
        i++;
      }
      if (KEYM_CERT_COUNT == i)
      {
        /* reset state */
        KeyM_State = KEYM_STATE_IDLE;
      }
    }
  }
  return retVal;
}
#endif /* #if (KEYM_API_ENABLED_SERVICECERTIFICATE == STD_ON) */

/* --- KeyM_SetCertificate ---------------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00057,1 */

FUNC(Std_ReturnType, KEYM_CODE) KeyM_SetCertificate
(
          KeyM_CertificateIdType                        CertId,
  P2CONST(KeyM_CertDataType, AUTOMATIC, KEYM_APPL_DATA) CertificateDataPtr
)
{
  Std_ReturnType retVal           = E_NOT_OK;
  boolean        doSetCertificate = FALSE;

#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_SETCERTIFICATE,
                           KEYM_E_UNINIT
                          );
  }
  else
  if (NULL_PTR == CertificateDataPtr)
  {
    /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00146,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_SETCERTIFICATE,
                           KEYM_E_PARAM_POINTER
                          );
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_SetCertificate/00002,2 */
    retVal = KEYM_E_PARAMETER_MISMATCH;
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO KeyM.Dsn.Api/ExAreas/0001,1 */
    SchM_Enter_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_SetCertificate/00006,2 */
    if (KEYM_STATE_IDLE != KeyM_State)
    {
      retVal = KEYM_E_BUSY;
    }
    else
    {
      doSetCertificate = TRUE;
      KeyM_State       = KEYM_STATE_SET_CERTIFICATE;
    }
    SchM_Exit_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();

    if (doSetCertificate)
    {
      /* !LINKSTO KeyM.Req.Gen/SWS_KeyM_00057_NvM,1 */
      if (   (CertId < KEYM_CERT_COUNT)
          && (KEYM_STORAGE_IN_NVM == KEYM_KEY(KEYM_CERT(CertId).cnfg->keymStorageKey).cnfg->keyStorage)
         )
      {
        retVal = E_NOT_OK;
      }
      else
      {
        retVal = KeyM_InternalSetCertificate(CertId,
                                             CertificateDataPtr->certData,
                                             CertificateDataPtr->certDataLength
                                            );
      }

      KeyM_State = KEYM_STATE_IDLE;
    }
  }
  return retVal;
}

/* --- KeyM_GetCertificate ---------------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00058,1 */

FUNC(Std_ReturnType, KEYM_CODE) KeyM_GetCertificate
(
        KeyM_CertificateIdType                        CertId,
  P2VAR(KeyM_CertDataType, AUTOMATIC, KEYM_APPL_DATA) CertificateDataPtr
)
{
  /* !LINKSTO SWS_KeyM_00117,1 */
  Std_ReturnType retVal           = E_NOT_OK;
  boolean        doGetCertificate = FALSE;

#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_GETCERTIFICATE,
                           KEYM_E_UNINIT
                          );
  }
  else
  if (NULL_PTR == CertificateDataPtr)
  {
    /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00146,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_GETCERTIFICATE,
                           KEYM_E_PARAM_POINTER
                          );
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_GetCertificate/00006,2 */
    retVal = KEYM_E_PARAMETER_MISMATCH;
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO KeyM.Dsn.Api/ExAreas/0001,1 */
    SchM_Enter_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_GetCertificate/00005,2 */
    if (KEYM_STATE_IDLE != KeyM_State)
    {
      retVal = KEYM_E_BUSY;
    }
    else
    {
      doGetCertificate = TRUE;
      KeyM_State       = KEYM_STATE_GET_CERTIFICATE;
    }
    SchM_Exit_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();

    if (doGetCertificate)
    {
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_GetCertificate/00001,1 */
      if (KEYM_CERT_COUNT <= CertId)
      {
        retVal = KEYM_E_PARAMETER_MISMATCH;
      }
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_GetCertificate/00002,1 */
      else
      if (KEYM_CERTIFICATE_NOT_AVAILABLE == KEYM_CERT(CertId).data->certStatus)
      {
        retVal = KEYM_E_KEY_CERT_EMPTY;
      }
      else
      {
        /* Get CryptoKey Buffer */
        /* !LINKSTO KeyM.Dsn.Alg/KeyM_GetCertificate/00003,1 */
        if (  CertificateDataPtr->certDataLength
            < KEYM_KEY(KEYM_CERT(CertId).cnfg->keymStorageKey).data->keyLength
           )
        {
          retVal = KEYM_E_KEY_CERT_SIZE_MISMATCH;
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
          /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00145,1 */
          (void) Det_ReportError(KEYM_MODULE_ID,
                                 KEYM_INSTANCE_ID,
                                 KEYM_SID_GETCERTIFICATE,
                                 KEYM_E_SMALL_BUFFER
                                );
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
        }
        else
        {
          /* !LINKSTO KeyM.Dsn.Alg/KeyM_GetCertificate/00004,1 */
          TS_MemCpy(CertificateDataPtr->certData,
                    KEYM_KEY(KEYM_CERT(CertId).cnfg->keymStorageKey).cnfg->keyData,
                    KEYM_KEY(KEYM_CERT(CertId).cnfg->keymStorageKey).data->keyLength
                   );
          CertificateDataPtr->certDataLength = KEYM_KEY(KEYM_CERT(CertId).cnfg->keymStorageKey).data->keyLength;
          retVal                             = E_OK;
        }
      }

      KeyM_State = KEYM_STATE_IDLE;
    }
  }
  return retVal;
}

/* --- KeyM_VerifyCertificates ------------------------------------------------------------------ */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00059,1 */

FUNC(Std_ReturnType, KEYM_CODE) KeyM_VerifyCertificates
(
  KeyM_CertificateIdType CertId,
  KeyM_CertificateIdType CertUpperId
)
{
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_VERIFYCERTIFICATES,
                           KEYM_E_UNINIT
                          );
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    TS_PARAM_UNUSED(CertId);
    TS_PARAM_UNUSED(CertUpperId);
  }
  return E_NOT_OK;
}

/* --- KeyM_VerifyCertificate ------------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00060,1 */

FUNC(Std_ReturnType, KEYM_CODE) KeyM_VerifyCertificate
(
  KeyM_CertificateIdType CertId
)
{
  Std_ReturnType retVal              = E_NOT_OK;
  boolean        doVerifyCertificate = FALSE;

#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_VERIFYCERTIFICATE,
                           KEYM_E_UNINIT
                          );
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO KeyM.Dsn.Api/ExAreas/0001,1 */
    SchM_Enter_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_VerifyCertificate/00007,2 */
    if (KEYM_STATE_IDLE != KeyM_State)
    {
      retVal = KEYM_E_BUSY;
    }
    else
    {
      doVerifyCertificate = TRUE;
      KeyM_State          = KEYM_STATE_VERIFY_CERTIFICATE;
    }
    SchM_Exit_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();

    if (doVerifyCertificate)
    {
       retVal = KeyM_InternalVerifyCertificate(CertId);

       if (E_OK != retVal)
       {
         KeyM_State = KEYM_STATE_IDLE;
       }
    }
  }

  return retVal;
}

/* --- KeyM_VerifyCertificateChain -------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00061,1 */

FUNC(Std_ReturnType, KEYM_CODE) KeyM_VerifyCertificateChain
(
             KeyM_CertificateIdType                        CertId,
  CONSTP2VAR(KeyM_CertDataType, AUTOMATIC, KEYM_APPL_DATA) certChainData,
             uint8                                         NumberOfCertificates
)
{
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_VERIFYCERTIFICATECHAIN,
                           KEYM_E_UNINIT
                          );
  }
  else
  if (NULL_PTR == certChainData)
  {
    /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00146,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_VERIFYCERTIFICATECHAIN,
                           KEYM_E_PARAM_POINTER
                          );
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    TS_PARAM_UNUSED(CertId);
    TS_PARAM_UNUSED(certChainData);
    TS_PARAM_UNUSED(NumberOfCertificates);
  }
  return E_NOT_OK;
}

/* --- KeyM_CertElementGet ---------------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00063,1 */

FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertElementGet
(
        KeyM_CertificateIdType             CertId,
        KeyM_CertElementIdType             CertElementId,
  P2VAR(uint8,  AUTOMATIC, KEYM_APPL_DATA) CertElementData,
  P2VAR(uint32, AUTOMATIC, KEYM_APPL_DATA) CertElementDataLength
)
{
  Std_ReturnType retVal = E_NOT_OK;
  boolean        doCertElementGet = FALSE;

#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_CERTELEMENTGET,
                           KEYM_E_UNINIT
                          );
  }
  else
  if (   (NULL_PTR == CertElementData      )
      || (NULL_PTR == CertElementDataLength)
     )
  {
    /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00146,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_CERTELEMENTGET,
                           KEYM_E_PARAM_POINTER
                          );
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGet/00008,1 */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGet/00009,1 */
    retVal = KEYM_E_PARAMETER_MISMATCH;
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO KeyM.Dsn.Api/ExAreas/0001,1 */
    SchM_Enter_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGet/00001,1 */
    if (KEYM_STATE_IDLE != KeyM_State)
    {
      retVal = KEYM_E_BUSY;
    }
    else
    {
      doCertElementGet = TRUE;
      KeyM_State       = KEYM_STATE_CERT_ELEMENT_GET;
    }
    SchM_Exit_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();

    if (doCertElementGet)
    {
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGet/00002,1 */
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGet/00003,1 */
      if ((CertId < KEYM_CERT_COUNT) && (CertElementId < KEYM_CERT(CertId).cnfg->certElementsNumber))
      {
        /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGet/00007,2 */
        if (KEYM_CERT(CertId).cnfg->certElements[CertElementId].cnfg->hasIterations)
        {
          /* Element with iterations not supported in this function. */
          retVal = E_NOT_OK;
        }
        else
        if (   (KEYM_CERTIFICATE_VALID == KEYM_CERT(CertId).data->certStatus)
            || (KEYM_CERTIFICATE_PARSED_NOT_VALIDATED == KEYM_CERT(CertId).data->certStatus)
           )
        {
          if (NULL_PTR != KEYM_CERT(CertId).cnfg->certElements[CertElementId].data->dataPtr)
          {
            /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGet/00004,1 */
            if (*CertElementDataLength >= KEYM_CERT(CertId).cnfg->certElements[CertElementId].data->dataLength)
            {
              /* !LINKSTO SWS_KeyM_00127,2 */
              TS_MemCpy(CertElementData,
                        KEYM_CERT(CertId).cnfg->certElements[CertElementId].data->dataPtr,
                        KEYM_CERT(CertId).cnfg->certElements[CertElementId].data->dataLength
                       );
              *CertElementDataLength = KEYM_CERT(CertId).cnfg->certElements[CertElementId].data->dataLength;
              retVal = E_OK;
            }
            else
            {
              /* Provided buffer for the certificate element too small. */
              retVal = KEYM_E_KEY_CERT_SIZE_MISMATCH;
            }
          }
          else
          {
            /* Element data not found. */
            retVal = E_NOT_OK;
          }
        }
        else
        /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGet/00005,1 */
        if (KEYM_CERTIFICATE_NOT_AVAILABLE == KEYM_CERT(CertId).data->certStatus)
        {
          /* No certificate data available, the certificate slot is empty. */
          retVal = KEYM_E_KEY_CERT_EMPTY;
        }
        else
        /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGet/00006,2 */
        {
          /* The certificate is not valid. */
          retVal = KEYM_E_KEY_CERT_INVALID;
        }
      }
      else
      {
        /* Certificate ID or certificate element ID invalid. */
        retVal = KEYM_E_PARAMETER_MISMATCH;
      }

      KeyM_State = KEYM_STATE_IDLE;
    }
    if (E_OK != retVal)
    {
      *CertElementDataLength = 0U;
    }
  }
  return retVal;
}

/* --- KeyM_CertElementGetFirst ----------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00064,1 */

FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertElementGetFirst
(
        KeyM_CertificateIdType                                   CertId,
        KeyM_CertElementIdType                                   CertElementId,
  P2VAR(KeyM_CertElementIteratorType, AUTOMATIC, KEYM_APPL_DATA) CertElementIterator,
  P2VAR(uint8,                        AUTOMATIC, KEYM_APPL_DATA) CertElementData,
  P2VAR(uint32,                       AUTOMATIC, KEYM_APPL_DATA) CertElementDataLength
)
{
  Std_ReturnType retVal = E_NOT_OK;
  boolean        doCertElementGetFirst = FALSE;

#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_CERTELEMENTGETFIRST,
                           KEYM_E_UNINIT
                          );
  }
  else
  if (   (NULL_PTR == CertElementIterator  )
      || (NULL_PTR == CertElementData      )
      || (NULL_PTR == CertElementDataLength)
     )
  {
    /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00146,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_CERTELEMENTGETFIRST,
                           KEYM_E_PARAM_POINTER
                          );
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetFirst/00006,1 */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetFirst/00010,1 */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetFirst/00011,1 */
    retVal = KEYM_E_PARAMETER_MISMATCH;
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO KeyM.Dsn.Api/ExAreas/0001,1 */
    SchM_Enter_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetFirst/00001,1 */
    if (KEYM_STATE_IDLE != KeyM_State)
    {
      retVal = KEYM_E_BUSY;
    }
    else
    {
      doCertElementGetFirst = TRUE;
      KeyM_State            = KEYM_STATE_CERT_ELEMENT_GET;
    }
    SchM_Exit_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();

    if (doCertElementGetFirst)
    {
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetFirst/00002,1 */
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetFirst/00003,1 */
      if ((CertId < KEYM_CERT_COUNT) && (CertElementId < KEYM_CERT(CertId).cnfg->certElementsNumber))
      {
        /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetFirst/00007,2 */
        if (   (KEYM_CERTIFICATE_VALID == KEYM_CERT(CertId).data->certStatus)
            || (KEYM_CERTIFICATE_PARSED_NOT_VALIDATED == KEYM_CERT(CertId).data->certStatus)
           )
        {
          if (NULL_PTR != KEYM_CERT(CertId).cnfg->certElements[CertElementId].data[0U].dataPtr)
          {
            /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetFirst/00005,1 */
            if (*CertElementDataLength >= KEYM_CERT(CertId).cnfg->certElements[CertElementId].data[0U].dataLength)
            {
              /* !LINKSTO SWS_KeyM_00128,1 */
              TS_MemCpy(CertElementData,
                        KEYM_CERT(CertId).cnfg->certElements[CertElementId].data[0U].dataPtr,
                        KEYM_CERT(CertId).cnfg->certElements[CertElementId].data[0U].dataLength
                       );
              *CertElementDataLength = KEYM_CERT(CertId).cnfg->certElements[CertElementId].data[0U].dataLength;

              /* Initialize iterator */
              /* !LINKSTO SWS_KeyM_00129,1 */
              CertElementIterator->certId = CertId;
              CertElementIterator->certElementId = CertElementId;
              CertElementIterator->index = 1U;

              retVal = E_OK;
            }
            else
            {
              /* Provided buffer for the certificate element too small. */
              retVal = KEYM_E_KEY_CERT_SIZE_MISMATCH;
            }
          }
          else
          {
            /* Element data not found. */
            retVal = E_NOT_OK;
          }
        }
        else
        /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetFirst/00004,1 */
        if (KEYM_CERTIFICATE_NOT_AVAILABLE == KEYM_CERT(CertId).data->certStatus)
        {
          /* No certificate data available, the certificate slot is empty. */
          retVal = KEYM_E_KEY_CERT_EMPTY;
        }
        /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGet/00006,2 */
        else
        {
          /* The certificate is not valid. */
          retVal = KEYM_E_KEY_CERT_INVALID;
        }
      }
      else
      {
        /* Certificate ID or certificate element ID invalid. */
        retVal = KEYM_E_PARAMETER_MISMATCH;
      }

      KeyM_State = KEYM_STATE_IDLE;
    }

    if (E_OK != retVal)
    {
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetFirst/00008,1 */
      CertElementIterator->index = 0U;
      *CertElementDataLength = 0U;
    }
  }
  return retVal;
}

/* --- KeyM_CertElementGetNext ------------------------------------------------------------------ */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00065,1 */

FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertElementGetNext
(
  P2VAR(KeyM_CertElementIteratorType, AUTOMATIC, KEYM_APPL_DATA) CertElementIterator,
  P2VAR(uint8,                        AUTOMATIC, KEYM_APPL_DATA) CertElementData,
  P2VAR(uint32,                       AUTOMATIC, KEYM_APPL_DATA) CertElementDataLength
)
{
  Std_ReturnType retVal = E_NOT_OK;
  boolean        doCertElementGetNext = FALSE;

#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_CERTELEMENTGETNEXT,
                           KEYM_E_UNINIT
                          );
  }
  else
  if (   (NULL_PTR == CertElementIterator  )
      || (NULL_PTR == CertElementData      )
      || (NULL_PTR == CertElementDataLength)
     )
  {
    /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00146,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_CERTELEMENTGETNEXT,
                           KEYM_E_PARAM_POINTER
                          );
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetNext/00006,1 */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetNext/00010,1 */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetNext/00011,1 */
    retVal = KEYM_E_PARAMETER_MISMATCH;
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO KeyM.Dsn.Api/ExAreas/0001,1 */
    SchM_Enter_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetNext/00001,1 */
    if (KEYM_STATE_IDLE != KeyM_State)
    {
      retVal = KEYM_E_BUSY;
    }
    else
    {
      doCertElementGetNext = TRUE;
      KeyM_State           = KEYM_STATE_CERT_ELEMENT_GET;
    }
    SchM_Exit_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();

    if (doCertElementGetNext)
    {
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetNext/00009,1 */
      if (0U == CertElementIterator->index)
      {
        retVal = E_NOT_OK;
      }
      else
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetNext/00002,1 */
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetNext/00003,1 */
      if (   (CertElementIterator->certId < KEYM_CERT_COUNT)
          && (CertElementIterator->certElementId < KEYM_CERT(CertElementIterator->certId).cnfg->certElementsNumber)
         )
      {
        /* !LINKSTO KeyM.Req.Gen/SWS_KeyM_00148,1 */
        if (!KEYM_CERT(CertElementIterator->certId).cnfg->certElements[CertElementIterator->certElementId].cnfg->hasIterations)
        {
          /* Element without iterations not supported in this function. */
          retVal = E_NOT_OK;
        }
        else
        /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetNext/00007,2 */
        if (   (KEYM_CERTIFICATE_VALID == KEYM_CERT(CertElementIterator->certId).data->certStatus)
            || (KEYM_CERTIFICATE_PARSED_NOT_VALIDATED == KEYM_CERT(CertElementIterator->certId).data->certStatus)
           )
        {
          if (NULL_PTR != KEYM_CERT(CertElementIterator->certId).cnfg->certElements[CertElementIterator->certElementId].data[CertElementIterator->index].dataPtr)
          {
            /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetNext/00005,1 */
            if (*CertElementDataLength >= KEYM_CERT(CertElementIterator->certId).cnfg->certElements[CertElementIterator->certElementId].data[CertElementIterator->index].dataLength)
            {
              /* !LINKSTO SWS_KeyM_00132,1 */
              TS_MemCpy(CertElementData,
                        KEYM_CERT(CertElementIterator->certId).cnfg->certElements[CertElementIterator->certElementId].data[CertElementIterator->index].dataPtr,
                        KEYM_CERT(CertElementIterator->certId).cnfg->certElements[CertElementIterator->certElementId].data[CertElementIterator->index].dataLength
                       );
              *CertElementDataLength = KEYM_CERT(CertElementIterator->certId).cnfg->certElements[CertElementIterator->certElementId].data[CertElementIterator->index].dataLength;

              /* Increase iterator, uint8 overflow is on purpose */
              (CertElementIterator->index)++;

              retVal = E_OK;
            }
            else
            {
              /* Provided buffer for the certificate element too small. */
              retVal = KEYM_E_KEY_CERT_SIZE_MISMATCH;
            }
          }
          else
          {
            /* Element data not found. */
            retVal = E_NOT_OK;
          }
        }
        else
        /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetNext/00004,1 */
        if (KEYM_CERTIFICATE_NOT_AVAILABLE == KEYM_CERT(CertElementIterator->certId).data->certStatus)
        {
          /* No certificate data available, the certificate slot is empty. */
          retVal = KEYM_E_KEY_CERT_EMPTY;
        }
        /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGet/00006,2 */
        else
        {
          /* The certificate is not valid. */
          retVal = KEYM_E_KEY_CERT_INVALID;
        }
      }
      else
      {
        /* Certificate ID or certificate element ID invalid. */
        retVal = KEYM_E_PARAMETER_MISMATCH;
      }

      KeyM_State = KEYM_STATE_IDLE;
    }

    if (E_OK != retVal)
    {
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertElementGetNext/00008,1 */
      CertElementIterator->index = 0U;
      *CertElementDataLength = 0U;
    }

  }
  return retVal;
}

/* --- KeyM_CertGetStatus ----------------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00066,1 */

FUNC(Std_ReturnType, KEYM_CODE) KeyM_CertGetStatus
(
        KeyM_CertificateIdType                                 CertId,
  P2VAR(KeyM_CertificateStatusType, AUTOMATIC, KEYM_APPL_DATA) Status
)
{
  Std_ReturnType retVal          = E_NOT_OK;
  boolean        doCertGetStatus = FALSE;

#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (!KeyM_Initialized)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_CERTGETSTATUS,
                           KEYM_E_UNINIT
                          );
  }
  else
  if (NULL_PTR == Status)
  {
    /* !LINKSTO SWS_KeyM_00036,1, SWS_KeyM_00146,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_CERTGETSTATUS,
                           KEYM_E_PARAM_POINTER
                          );
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertGetStatus/00004,1 */
    retVal = KEYM_E_PARAMETER_MISMATCH;
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO KeyM.Dsn.Api/ExAreas/0001,1 */
    SchM_Enter_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertGetStatus/00002,2 */
    if (KEYM_STATE_IDLE != KeyM_State)
    {
      retVal = KEYM_E_BUSY;
    }
    else
    {
      doCertGetStatus = TRUE;
      KeyM_State      = KEYM_STATE_CERT_GET_STATUS;
    }
    SchM_Exit_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();

    if (doCertGetStatus)
    {
      if (KEYM_CERT_COUNT >= CertId)
      {
        /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertGetStatus/00001,1 */
        /* !LINKSTO SWS_KeyM_00133,1 */
        *Status = KEYM_CERT(CertId).data->certStatus;
        retVal  = E_OK;
      }

      KeyM_State = KEYM_STATE_IDLE;
    }
  }

  return retVal;
}

/* --- KeyM_MainFunction ------------------------------------------------------------------------ */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00074,1 */

FUNC(void, KEYM_CODE) KeyM_MainFunction
(
  void
)
{
/* !LINKSTO KeyM.Dsn.Alg/KeyM_General/00001,2 */
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF)
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (KEYM_STATE_UNINIT == KeyM_State)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_MAINFUNCTION,
                           KEYM_E_UNINIT
                          );
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    switch(KeyM_MainFunction_State)
    {
#if (KEYM_API_ENABLED_NVM == STD_ON)
      case KEYM_MFUNC_STATE_NVMREAD:
      {
        KeyM_MainFunction_State_NvMRead();
        break;
      }

      case KEYM_MFUNC_STATE_NVMREADERRORSTATUS:
      {
        KeyM_MainFunction_State_NvMReadErrorStatus();
        break;
      }
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */

      case KEYM_MFUNC_STATE_VRFY_INIT:
      {
        KeyM_MainFunction_State_VrfyInit();
        break;
      }

      case KEYM_MFUNC_STATE_VRFY_PARSE:
      {
        KeyM_MainFunction_State_VrfyParse();
        break;
      }

      /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00070,1 */
      case KEYM_MFUNC_STATE_VRFY_PREPROOT:
      {
        KeyM_MainFunction_State_VrfyPrepRoot();
        break;
      }

      case KEYM_MFUNC_STATE_VRFY_VERIFY:
      {
        KeyM_MainFunction_State_VrfyVerify();
        break;
      }

      /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00140,1 */
      case KEYM_MFUNC_STATE_VRFY_ERROR:
      {
        KeyM_MainFunction_State_VrfyError();
        break;
      }

      case KEYM_MFUNC_STATE_VRFY_WAITCSMCALLBACK:
      {
        KeyM_MainFunction_State_VrfyWaitCsmCallback();
        break;
      }

      case KEYM_MFUNC_STATE_VRFY_STOREPUBLICKEY:
      {
        KeyM_MainFunction_State_VrfyStorePublicKey();
        break;
      }

#if (KEYM_API_ENABLED_NVM == STD_ON)
      case KEYM_MFUNC_STATE_VRFY_STORE_NVM:
      {
        KeyM_MainFunction_State_VrfyStoreNvM();
        break;
      }

      /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00120,1 */
      case KEYM_MFUNC_STATE_NVMWRITEERRORSTATUS:
      {
        KeyM_MainFunction_State_NvMWriteErrorStatus();
        break;
      }
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */

      /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00130,1 */
      case KEYM_MFUNC_STATE_VRFY_FINISH:
      {
        KeyM_MainFunction_State_VrfyFinish();
        break;
      }

      /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00160,1 */
      case KEYM_MFUNC_STATE_PREPARSE_PARSE:
      {
        KeyM_MainFunction_State_PreparseParse();
        break;
      }

      /* CHECK: NOPARSE */
      /* Default case can not occur in test environment */
      default:
      {
        /* Should be never reached. */
        break;
      }
      /* CHECK: PARSE */
    }
  }
#else /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) */
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (KEYM_STATE_UNINIT == KeyM_State)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_MAINFUNCTION,
                           KEYM_E_UNINIT
                          );
  }
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF */
}

/* --- KeyM_MainFunction_State_NvMRead ---------------------------------------------------------- */

#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF)
#if (KEYM_API_ENABLED_NVM == STD_ON)
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_NvMRead
(
  void
)
{
  uint32 i = 0U;
  for (i = 0U; i < KEYM_CERT_COUNT; i++)
  {
    if (KEYM_STORAGE_IN_NVM == KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).cnfg->keyStorage)
    {
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00030,1 */
      if (E_OK == NvM_ReadBlock(KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).cnfg->nvmBlockDescriptorId,
                                KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).cnfg->keyData))
      {
        /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00031,2 */
        KEYM_CERT(i).data->certNvMReadStatus = KEYM_CERTIFICATE_NVM_READ_REQUESTED;
      }
      else
      {
        /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00031,2 */
        KEYM_CERT(i).data->certNvMReadStatus = KEYM_CERTIFICATE_NVM_READ_FAILED;

        /* Reset buffer and keep status not available. */
        TS_MemBZero(KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).cnfg->keyData,
                    KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).data->keyLength
                   );
      }
    }
  }
  /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00032,1 */
  KeyM_MainFunction_State = KEYM_MFUNC_STATE_NVMREADERRORSTATUS;
}
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */

/* --- KeyM_MainFunction_State_NvMReadErrorStatus ----------------------------------------------- */

#if (KEYM_API_ENABLED_NVM == STD_ON)
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_NvMReadErrorStatus
(
  void
)
{
  uint32                    i                     = 0U;
  Std_ReturnType            retVal;
  boolean                   isAllNvMStatesChecked = TRUE;
  NvM_RequestResultType     requestResult         = NVM_REQ_NOT_OK;

  for (i = 0U; i < KEYM_CERT_COUNT; i++)
  {
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00040,1 */
    if (KEYM_CERTIFICATE_NVM_READ_REQUESTED == KEYM_CERT(i).data->certNvMReadStatus)
    {
      /* Poll read status */
      retVal = NvM_GetErrorStatus(KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).cnfg->nvmBlockDescriptorId,
                                  &requestResult
                                 );

      if ((E_OK == retVal) && (NVM_REQ_PENDING == requestResult))
      {
        isAllNvMStatesChecked = FALSE;
      }
      else
      if (NVM_REQ_OK == requestResult)
      {
        KEYM_CERT(i).data->certNvMReadStatus = KEYM_CERTIFICATE_NVM_READ_SUCCESSFUL;
        KEYM_CERT(i).data->certStatus        = KEYM_CERTIFICATE_NOT_PARSED;
      }
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_Init_Async/00004,1 */
      else
      {
        /* Read not successful. Retry handling should have been already done within NvM,
        if configured for dedicated block. */
        KEYM_CERT(i).data->certNvMReadStatus = KEYM_CERTIFICATE_NVM_READ_FAILED;

        /* Reset buffer and keep status not available. */
        TS_MemBZero(KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).cnfg->keyData,
                    KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).data->keyLength
                   );
      }
    }
  }

  /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00041,2 */
  if (isAllNvMStatesChecked)
  {
    KeyM_State                = KEYM_STATE_IDLE;

    /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00042,2 */
    KeyM_MainFunction_State = KEYM_MFUNC_STATE_PREPARSE_PARSE;
  }
}
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */

/* --- KeyM_MainFunction_State_VrfyStoreNvM ----------------------------------------------------- */

#if (KEYM_API_ENABLED_NVM == STD_ON)
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyStoreNvM
(
  void
)
{
  /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00110,1 */
  /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00014,1 */
  if (E_OK == NvM_WriteBlock(KEYM_KEY(KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymStorageKey).cnfg->nvmBlockDescriptorId,
                             KEYM_KEY(KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymStorageKey).cnfg->keyData
                            )
     )
  {
    KeyM_MainFunction_State = KEYM_MFUNC_STATE_NVMWRITEERRORSTATUS;
  }
  else
  {
    KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_FINISH;
  }
}
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */

/* --- KeyM_MainFunction_State_NvMWriteErrorStatus ---------------------------------------------- */

#if (KEYM_API_ENABLED_NVM == STD_ON)
static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_NvMWriteErrorStatus
(
  void
)
{
  NvM_RequestResultType requestResult = NVM_REQ_NOT_OK;
  Std_ReturnType        retVal;

  /* Get write status */
  retVal = NvM_GetErrorStatus(KEYM_KEY(KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymStorageKey).cnfg->nvmBlockDescriptorId,
                              &requestResult
                             );
  if ((E_OK == retVal) && (NVM_REQ_PENDING == requestResult))
  {
    /* Stay in this state as long as NvM request is pending */
    KeyM_MainFunction_State = KEYM_MFUNC_STATE_NVMWRITEERRORSTATUS;
  }
  else
  if (NVM_REQ_OK == requestResult)
  {
    KeyM_CallbackValue      = KEYM_RT_OK;
    KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_FINISH;
  }
  else
  {
    KeyM_CallbackValue      = KEYM_RT_KEY_CERT_WRITE_FAIL;
    KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_FINISH;
  }
}
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */

/* --- KeyM_MainFunction_State_VrfyInit --------------------------------------------------------- */

static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyInit
(
  void
)
{
  KeyM_CallbackValue         = KEYM_RT_NOT_OK;
  KeyM_SignatureVerifyResult = CRYPTO_E_VER_NOT_OK;

  if (FALSE == KeyM_FlgVerifyRequested)
  {
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00052,2 */
    /* No verification requests pending, check if preparsing is necessary */
    KeyM_MainFunction_State = KEYM_MFUNC_STATE_PREPARSE_PARSE;
  }
  else
  {
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00053,2 */
    if (   (KEYM_E_CERTIFICATE_INVALID_FORMAT  == KEYM_CERT(KeyM_CertVrfyIdCur).data->certStatus)
        || (KEYM_E_CERTIFICATE_INVALID_CONTENT == KEYM_CERT(KeyM_CertVrfyIdCur).data->certStatus)
        || (KEYM_E_CERTIFICATE_INVALID_TYPE    == KEYM_CERT(KeyM_CertVrfyIdCur).data->certStatus)
      )
    {
      KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_ERROR;
      KeyM_CallbackValue      = KEYM_RT_KEY_CERT_INVALID;
    }
    else
    {
      KeyM_CertVrfyChainIdCur = KeyM_CertVrfyIdCur;

      KeyM_Push(KeyM_CertVrfyIdCur);
      /* check if current cert is root cert and stop. */
      while (KeyM_CertVrfyChainIdCur != KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certUpperCertId)
      {
        KeyM_CertVrfyChainIdCur = KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certUpperCertId;
        KeyM_Push(KeyM_CertVrfyChainIdCur);
      }
      (void)KeyM_Pop(&KeyM_CertVrfyChainIdCur);

      /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00054,2 */
      if (!KEYM_CERT(KeyM_CertVrfyChainIdCur).data->flgParsed)
      {
        KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_PARSE;
      }
      else
      {
        KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_PREPROOT;
      }
    }
  }
}

/* --- KeyM_MainFunction_State_VrfyParse -------------------------------------------------------- */

static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyParse
(
  void
)
{
  /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00060,1 */
  KeyM_ParseHelper(KeyM_CertVrfyChainIdCur);
  if (KEYM_CERTIFICATE_PARSED_NOT_VALIDATED == KEYM_CERT(KeyM_CertVrfyChainIdCur).data->certStatus)
  {
    if (KeyM_CertVrfyChainIdCur == KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certUpperCertId)
    {
      KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_PREPROOT;
    }
    else
    {
      KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_VERIFY;
    }
  }
  else
  {
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00061,1 */
    if (KeyM_CertVrfyChainIdCur == KeyM_CertVrfyIdCur)
    {
      KeyM_CallbackValue = KEYM_RT_KEY_CERT_INVALID;
    }
    else
    {
      KeyM_CallbackValue = KEYM_RT_CERT_INVALID_CHAIN_OF_TRUST;
    }

    KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_ERROR;
  }
}

/* --- KeyM_MainFunction_State_VrfyPrepRoot ----------------------------------------------------- */

static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyPrepRoot
(
  void
)
{
  Std_ReturnType retVal;
  retVal = Csm_KeyElementSet(KEYM_CERT(KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certUpperCertId).cnfg->csmSignatureVerifyKeyId,
                             1U,
                             KEYM_CERT(KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certUpperCertId).cnfg->certElementDataPublicKey->dataPtr,
                             KEYM_CERT(KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certUpperCertId).cnfg->certElementDataPublicKey->dataLength
                            );
  if (E_OK == retVal)
  {
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_Verify_Async/00002,1 */
    retVal = Csm_KeySetValid(KEYM_CERT(KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certUpperCertId).cnfg->csmSignatureVerifyKeyId);
  }
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  else
  {
    /* !LINKSTO KeyM.Dsn.Gen/KEYM_E_CSMKEYELEMENTSET_FAILED/00002,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_MAINFUNCTION,
                           KEYM_E_CSMKEYELEMENTSET_FAILED
                          );
  }
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  if (E_OK == retVal)
  {
    KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_VERIFY;
  }
  else
  {
    KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_ERROR;
  }
}

/* --- KeyM_MainFunction_State_VrfyVerify ------------------------------------------------------- */

static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyVerify
(
  void
)
{
  Std_ReturnType retVal;
  retVal = KeyM_X509NameCmp((KEYM_CERT(KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certUpperCertId).cnfg->certElementDataSubject->dataPtr),
                            (KEYM_CERT(KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certUpperCertId).cnfg->certElementDataSubject->dataLength),
                            (KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certElementDataIssuer->dataPtr),
                            (KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certElementDataIssuer->dataLength)
                           );
  if (E_OK == retVal)
  {
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_Verify_Async/00003,2 */
    retVal = Csm_SignatureVerify(KEYM_CERT(KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certUpperCertId).cnfg->csmSignatureVerifyJobId,
                                 CRYPTO_OPERATIONMODE_SINGLECALL,
                                 KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certTbsCertificate->dataPtr,
                                 KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certTbsCertificate->dataLength,
                                 KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certElementDataSignatureValue->dataPtr,
                                 KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certElementDataSignatureValue->dataLength,
                                 &KeyM_SignatureVerifyResult
                                );
  }

  if (E_OK == retVal)
  {
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00080,1 */
    if (CRYPTO_PROCESSING_ASYNC == KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->csmSignatureVerifyJobProcessingType)
    {
      KeyM_CsmCallback        = FALSE;
      KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_WAITCSMCALLBACK;
    }
    else
    {
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00082,1 */
      if (CRYPTO_E_VER_OK == KeyM_SignatureVerifyResult)
      {
        KEYM_CERT(KeyM_CertVrfyChainIdCur).data->certStatus = KEYM_CERTIFICATE_VALID;
        KeyM_CallbackValue                                  = KEYM_RT_OK;
        KeyM_MainFunction_State                             = KEYM_MFUNC_STATE_VRFY_STOREPUBLICKEY;
      }
      else
      {
        /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00081,1 */
        /* !LINKSTO SWS_KeyM_00034,1 */
        KEYM_CERT(KeyM_CertVrfyChainIdCur).data->certStatus = KEYM_E_CERTIFICATE_SIGNATURE_FAIL;
        KeyM_CallbackValue                                  = KEYM_RT_KEY_CERT_INVALID;
        KeyM_MainFunction_State                             = KEYM_MFUNC_STATE_VRFY_ERROR;
      }
    }
  }
  else
  {
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00081,1 */
    KEYM_CERT(KeyM_CertVrfyChainIdCur).data->certStatus = KEYM_E_CERTIFICATE_SIGNATURE_FAIL;
    KeyM_CallbackValue                                  = KEYM_RT_KEY_CERT_INVALID;
    KeyM_MainFunction_State                             = KEYM_MFUNC_STATE_VRFY_ERROR;
  }
}

/* --- KeyM_MainFunction_State_VrfyError -------------------------------------------------------- */

static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyError
(
  void
)
{
  while (E_OK == KeyM_Pop(&KeyM_CertVrfyChainIdCur))
  {
    KEYM_CERT(KeyM_CertVrfyChainIdCur).data->certStatus = KEYM_E_CERTIFICATE_INVALID_CHAIN_OF_TRUST;
  }

#if (KEYM_API_ENABLED_NVM == STD_ON)
  if (                  (TRUE == KEYM_CERT(KeyM_CertVrfyIdCur).data->flgServiceRequested)
      && (KEYM_STORAGE_IN_NVM == KEYM_KEY(KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymStorageKey).cnfg->keyStorage)
     )
  {
    KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_STORE_NVM;
  }
  else
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */
  {
    KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_FINISH;
  }
}

/* --- KeyM_MainFunction_State_VrfyWaitCsmCallback ---------------------------------------------- */

static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyWaitCsmCallback
(
  void
)
{
  if(TRUE == KeyM_CsmCallback)
  {
    KeyM_CsmCallback = FALSE;

    /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00090,1 */
    if (CRYPTO_E_VER_OK == KeyM_SignatureVerifyResult)
    {
      KEYM_CERT(KeyM_CertVrfyChainIdCur).data->certStatus = KEYM_CERTIFICATE_VALID;
      KeyM_MainFunction_State                             = KEYM_MFUNC_STATE_VRFY_STOREPUBLICKEY;
    }
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00091,1 */
    else
    {
      /* !LINKSTO SWS_KeyM_00034,1 */
      KEYM_CERT(KeyM_CertVrfyChainIdCur).data->certStatus = KEYM_E_CERTIFICATE_SIGNATURE_FAIL;
      KeyM_CallbackValue                                  = KEYM_RT_KEY_CERT_INVALID;
      KeyM_MainFunction_State                             = KEYM_MFUNC_STATE_VRFY_ERROR;
    }
  }
}

/* --- KeyM_MainFunction_State_VrfyStorePublicKey ----------------------------------------------- */

static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyStorePublicKey
(
  void
)
{
  Std_ReturnType retVal;
  retVal = Csm_KeyElementSet(KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->csmSignatureVerifyKeyId,
                             1U,
                             KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certElementDataPublicKey->dataPtr,
                             KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certElementDataPublicKey->dataLength
                            );
  if (E_OK == retVal)
  {
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_Verify_Async/00002,1 */
    retVal = Csm_KeySetValid(KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->csmSignatureVerifyKeyId);
  }
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  else
  {
    /* !LINKSTO KeyM.Dsn.Gen/KEYM_E_CSMKEYELEMENTSET_FAILED/00002,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_MAINFUNCTION,
                           KEYM_E_CSMKEYELEMENTSET_FAILED
                          );
  }
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  if (E_OK == retVal)
  {
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00100,1 */
    if (KeyM_CertVrfyChainIdCur != KeyM_CertVrfyIdCur)
    {
      (void)KeyM_Pop(&KeyM_CertVrfyChainIdCur);
      if (FALSE == KEYM_CERT(KeyM_CertVrfyChainIdCur).data->flgParsed)
      {
        KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_PARSE;
      }
      else
      {
        KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_VERIFY;
      }
    }
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00101,1 */
    else
    {
#if (KEYM_API_ENABLED_NVM == STD_ON)
      if (   (TRUE == KEYM_CERT(KeyM_CertVrfyIdCur).data->flgServiceRequested)
          && (KEYM_STORAGE_IN_NVM == KEYM_KEY(KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymStorageKey).cnfg->keyStorage)
         )
      {
        KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_STORE_NVM;
      }
      else
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */
      {
        KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_FINISH;
      }
    }
  }
  else
  {
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_MainFunction/00102,2 */
    KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_ERROR;
    KeyM_CallbackValue      = KEYM_RT_CERT_INVALID_CHAIN_OF_TRUST;
  }
}

/* --- KeyM_MainFunction_State_VrfyFinish ------------------------------------------------------- */

static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_VrfyFinish
(
  void
)
{
  /* !LINKSTO SWS_KeyM_00152,1 */
  if (TRUE == KEYM_CERT(KeyM_CertVrfyIdCur).data->flgServiceRequested)
  {
    KEYM_CERT(KeyM_CertVrfyIdCur).data->flgServiceRequested = FALSE;
    if (NULL_PTR != KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymCbkServiceCertificate)
    {
      /* !LINKSTO SWS_KeyM_00149,1 */
      KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymCbkServiceCertificate(KeyM_CertVrfyIdCur, KeyM_CallbackValue, 0U, NULL_PTR);
    }
  }
  else
  {
    /* !LINKSTO SWS_KeyM_00153,1 */
    if (NULL_PTR != KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymCbkCertificateVerify)
    {
      /* !LINKSTO SWS_KeyM_00153,1 */
      (void) KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymCbkCertificateVerify(KeyM_CertVrfyIdCur,
                                                                      KEYM_CERT(KeyM_CertVrfyIdCur).data->certStatus
                                                                    );
    }
  }
  KeyM_FlgVerifyRequested = FALSE;
  KeyM_State              = KEYM_STATE_IDLE;
  KeyM_MainFunction_State = KEYM_MFUNC_STATE_PREPARSE_PARSE;
}

/* --- KeyM_MainFunction_State_PreparseParse ---------------------------------------------------- */

static FUNC(void, KEYM_CODE) KeyM_MainFunction_State_PreparseParse
(
  void
)
{
  KeyM_CertificateIdType tmpParseId = KeyM_CertParseIdCur;

  do
  {
    if (   (FALSE                          == KEYM_CERT(tmpParseId).data->flgParsed)
        && (KEYM_CERTIFICATE_NOT_AVAILABLE != KEYM_CERT(tmpParseId).data->certStatus)
      )
    {
      /* Call ParseHelper to parse certificate */
      KeyM_ParseHelper(tmpParseId);

      /* Store potential next parsing candidate */
      KeyM_CertParseIdCur = (tmpParseId + 1U) % KEYM_CERT_COUNT;
    }
    tmpParseId = (tmpParseId + 1U) % KEYM_CERT_COUNT;
  }
  while (tmpParseId != KeyM_CertParseIdCur);

  /* Preparsing finished, check if verification request is pending */
  KeyM_MainFunction_State = KEYM_MFUNC_STATE_VRFY_INIT;
}
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) */

/* --- KeyM_MainBackgroundFunction -------------------------------------------------------------- */

/* !LINKSTO KeyM.Dsn.SWS_KeyM_00075,1 */

FUNC(void, KEYM_CODE) KeyM_MainBackgroundFunction
(
  void
)
{
/* !LINKSTO KeyM.Dsn.Alg/KeyM_General/00001,2 */
#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON)
  boolean                doInit     = FALSE;
  KeyM_CertificateIdType tmpParseId = KeyM_CertParseIdCur;

#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (KEYM_STATE_UNINIT == KeyM_State)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_MAINBACKGROUNDFUNCTION,
                           KEYM_E_UNINIT
                          );
  }
  else
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  {
    /* !LINKSTO KeyM.Dsn.Api/ExAreas/0001,1 */
    SchM_Enter_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();
    if (KEYM_STATE_INIT == KeyM_State)
    {
      doInit = TRUE;
    }
    SchM_Exit_KeyM_SCHM_KEYM_EXCLUSIVE_AREA_0();

    if (doInit)
    {
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_Init_Async/00001,1 */
      KeyM_InitHelper();
    }
    else
    {
      KeyM_VerifyHelper();

      do
      {
        if (   (FALSE                          == KEYM_CERT(tmpParseId).data->flgParsed)
            && (KEYM_CERTIFICATE_NOT_AVAILABLE != KEYM_CERT(tmpParseId).data->certStatus)
          )
        {
          /* Call ParseHelper to parse certificate */
          KeyM_ParseHelper(tmpParseId);

          /* Store potential next parsing candidate */
          KeyM_CertParseIdCur = (tmpParseId + 1U) % KEYM_CERT_COUNT;
        }
        tmpParseId = (tmpParseId + 1U) % KEYM_CERT_COUNT;
      }
      while (tmpParseId != KeyM_CertParseIdCur);
    }
  }
#else /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON) */
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
  if (KEYM_STATE_UNINIT == KeyM_State)
  {
    /* !LINKSTO SWS_KeyM_00036,1, KeyM.Req.Gen/SWS_KeyM_00144,1 */
    (void) Det_ReportError(KEYM_MODULE_ID,
                           KEYM_INSTANCE_ID,
                           KEYM_SID_MAINBACKGROUNDFUNCTION,
                           KEYM_E_UNINIT
                          );
  }
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON) */
}

#define KEYM_STOP_SEC_CODE
#include <KeyM_MemMap.h>

/*==================[internal function definitions]===============================================*/

#define KEYM_START_SEC_CODE
#include <KeyM_MemMap.h>

/* --- KeyM_InternalSetCertificate -------------------------------------------------------------- */

static FUNC(Std_ReturnType, KEYM_CODE) KeyM_InternalSetCertificate
(
          KeyM_CertificateIdType            certificateId,
  P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) certDataPtr,
          uint32                            certDataLength
)
{
  Std_ReturnType retVal;
  uint32         i;
  uint8          j;

  /* !LINKSTO KeyM.Dsn.Alg/KeyM_SetCertificate/00001,1 */
  if (certificateId >= KEYM_CERT_COUNT)
  {
    retVal = KEYM_E_PARAMETER_MISMATCH;
  }
  else
  {
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_SetCertificate/00004,1 */
    if (   (certDataLength > KEYM_CERT(certificateId).cnfg->certMaxLength)
        || (  certDataLength
            > KEYM_KEY(KEYM_CERT(certificateId).cnfg->keymStorageKey).cnfg->keyMaxLength
           )
       )
    {
      /* Not enough place in the local buffer to copy the certificate */
      retVal = KEYM_E_KEY_CERT_SIZE_MISMATCH;
    }
    else
    {
      /* reset certificate */
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00004,1 */
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_SetCertificate/00003,1 */
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_SetCertificate/00005,1 */
      TS_MemBZero(KEYM_KEY(KEYM_CERT(certificateId).cnfg->keymStorageKey).cnfg->keyData,
                  KEYM_KEY(KEYM_CERT(certificateId).cnfg->keymStorageKey).data->keyLength
                 );
      for (i = 0U; i < KEYM_CERT(certificateId).cnfg->certElementsNumber; i++)
      {
        j = 0U;
        do
        {
          KEYM_CERT(certificateId).cnfg->certElements[i].data[j].dataLength = 0U;
          KEYM_CERT(certificateId).cnfg->certElements[i].data[j].dataPtr    = NULL_PTR;
          j++;
        } while (j < KEYM_CERT(certificateId).cnfg->certElements[i].cnfg->maxIterations);
      }
      KEYM_CERT(certificateId).cnfg->certElementDataIssuer->dataLength         = 0U;
      KEYM_CERT(certificateId).cnfg->certElementDataIssuer->dataPtr            = NULL_PTR;
      KEYM_CERT(certificateId).cnfg->certElementDataSubject->dataLength        = 0U;
      KEYM_CERT(certificateId).cnfg->certElementDataSubject->dataPtr           = NULL_PTR;
      KEYM_CERT(certificateId).cnfg->certElementDataSignatureValue->dataLength = 0U;
      KEYM_CERT(certificateId).cnfg->certElementDataSignatureValue->dataPtr    = NULL_PTR;
      KEYM_CERT(certificateId).cnfg->certElementDataPublicKey->dataLength      = 0U;
      KEYM_CERT(certificateId).cnfg->certElementDataPublicKey->dataPtr         = NULL_PTR;
      KEYM_CERT(certificateId).cnfg->certTbsCertificate->dataLength            = 0U;
      KEYM_CERT(certificateId).cnfg->certTbsCertificate->dataPtr               = NULL_PTR;
#if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON))
      KEYM_CERT(certificateId).data->certNvMReadStatus                         = KEYM_CERTIFICATE_NVM_READ_NOTSET;
#endif /* #if ((KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_OFF) && (KEYM_API_ENABLED_NVM == STD_ON)) */
      /* !LINKSTO SWS_KeyM_00026,1 */
      KEYM_CERT(certificateId).data->flgParsed = FALSE;
      if (0U == certDataLength)
      {
        /* !LINKSTO SWS_KeyM_00140,1 */
        /* !LINKSTO SWS_KeyM_00141,1 */
        KEYM_CERT(certificateId).data->certStatus = KEYM_CERTIFICATE_NOT_AVAILABLE;
      }
      else
      {
        /* Copy certificate into local buffer */
        /* !LINKSTO SWS_KeyM_00115,1 */
        TS_MemCpy(KEYM_KEY(KEYM_CERT(certificateId).cnfg->keymStorageKey).cnfg->keyData,
                  certDataPtr,
                  certDataLength
                 );
        /* Update certificate size in local buffer */
        KEYM_KEY(KEYM_CERT(certificateId).cnfg->keymStorageKey).data->keyLength = certDataLength;
        /* !LINKSTO SWS_KeyM_00138,1 */
        KEYM_CERT(certificateId).data->certStatus = KEYM_CERTIFICATE_NOT_PARSED;
      }
      /* !LINKSTO SWS_KeyM_00116,1 */
      retVal = E_OK;
    }
  }
  return retVal;
}

/* --- KeyM_InternalVerifyCertificate ----------------------------------------------------------- */

static FUNC(Std_ReturnType, KEYM_CODE) KeyM_InternalVerifyCertificate
(
  KeyM_CertificateIdType certificateId
)
{
  Std_ReturnType         retVal;
  KeyM_CertificateIdType tempCertId;

  if (certificateId >= KEYM_CERT_COUNT)
  {
    retVal = KEYM_E_PARAMETER_MISMATCH;
  }
  else
  if (KEYM_CERTIFICATE_NOT_AVAILABLE == KEYM_CERT(certificateId).data->certStatus)
  {
    retVal = KEYM_E_KEY_CERT_EMPTY;
  }
  else
  {
    /* !LINKSTO SWS_KeyM_00120,1 */
    /* !LINKSTO KeyM.Req.Gen/SWS_KeyM_00029,2 */
    /* build chain */
    tempCertId = certificateId;
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00007,1 */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_VerifyCertificate/00001,1 */
    KeyM_Push(tempCertId);
    while (tempCertId != KEYM_CERT(tempCertId).cnfg->certUpperCertId)
    {
      tempCertId = KEYM_CERT(tempCertId).cnfg->certUpperCertId;
      KeyM_Push(tempCertId);
    }
    retVal = E_OK;
    /* check chain */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00008,1 */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_VerifyCertificate/00002,1 */
    while (E_OK == KeyM_Pop(&tempCertId))
    {
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00009,1 */
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_VerifyCertificate/00003,1 */
      if (KEYM_CERTIFICATE_NOT_AVAILABLE == KEYM_CERT(tempCertId).data->certStatus)
      {
        if (E_OK == retVal)
        {
          retVal = KEYM_E_KEY_CERT_EMPTY;
        }
      }
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00010,1 */
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_VerifyCertificate/00004,1 */
      else
      if (   (KEYM_E_CERTIFICATE_INVALID_FORMAT  == KEYM_CERT(tempCertId).data->certStatus)
          || (KEYM_E_CERTIFICATE_INVALID_CONTENT == KEYM_CERT(tempCertId).data->certStatus)
         )
      {
        if (tempCertId != certificateId)
        {
          if (E_OK == retVal)
          {
            /* !LINKSTO SWS_KeyM_00030,1 */
            retVal = KEYM_E_CERT_INVALID_CHAIN_OF_TRUST;
          }
        }
      }
      else
      if (E_OK != retVal)
      {
        KEYM_CERT(tempCertId).data->certStatus = KEYM_E_CERTIFICATE_INVALID_CHAIN_OF_TRUST;
      }
      else
      {
        /* Needed for MISRA. Nothing to do here */
      }
    }
    /* asynchronous verification is only started if no errors are found */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00011,1 */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_VerifyCertificate/00005,1 */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_VerifyCertificate/00006,1 */
    /* !LINKSTO SWS_KeyM_00121,1 */
    if (E_OK == retVal)
    {
      KeyM_FlgVerifyRequested = TRUE;
      KeyM_CertVrfyIdCur      = certificateId;
      KeyM_CertVrfyChainIdCur = certificateId;
    }
  }
  return retVal;
}

/* --- KeyM_CompareAndVerify -------------------------------------------------------------------- */

#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON)
static FUNC(Std_ReturnType, KEYM_CODE) KeyM_CompareAndVerify
(
  KeyM_CertificateIdType certificateId
)
{
  Std_ReturnType          retVal                     = E_NOT_OK;
  Crypto_VerifyResultType KeyM_SignatureVerifyResult = CRYPTO_E_VER_NOT_OK;

  /* if current cert is root the key has to be set */
  if (certificateId == KEYM_CERT(certificateId).cnfg->certUpperCertId)
  {
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_Verify_Async/00001,2 */
    retVal = Csm_KeyElementSet(KEYM_CERT(certificateId).cnfg->csmSignatureVerifyKeyId,
                               1U,
                               KEYM_CERT(certificateId).cnfg->certElementDataPublicKey->dataPtr,
                               KEYM_CERT(certificateId).cnfg->certElementDataPublicKey->dataLength
                              );
    if (E_OK == retVal)
    {
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_Verify_Async/00002,1 */
      retVal = Csm_KeySetValid(KEYM_CERT(certificateId).cnfg->csmSignatureVerifyKeyId);
    }
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
    else
    {
      /* !LINKSTO KeyM.Dsn.Gen/KEYM_E_CSMKEYELEMENTSET_FAILED/00002,1 */
      (void) Det_ReportError(KEYM_MODULE_ID,
                             KEYM_INSTANCE_ID,
                             KEYM_SID_MAINBACKGROUNDFUNCTION,
                             KEYM_E_CSMKEYELEMENTSET_FAILED
                            );
    }
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  }
  else
  {
    retVal = E_OK;
  }
  /* Validate certificate */
  if (E_OK == retVal)
  {
    /* Check that subject field of the upper certificate matches
     * the issuer field of the certificate to verify.
     */
    retVal = KeyM_X509NameCmp((KEYM_CERT(KEYM_CERT(certificateId).cnfg->certUpperCertId).cnfg->certElementDataSubject->dataPtr),
                              (KEYM_CERT(KEYM_CERT(certificateId).cnfg->certUpperCertId).cnfg->certElementDataSubject->dataLength),
                              (KEYM_CERT(certificateId).cnfg->certElementDataIssuer->dataPtr),
                              (KEYM_CERT(certificateId).cnfg->certElementDataIssuer->dataLength)
                             );
    if (E_OK == retVal)
    {
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_Verify_Async/00003,2 */
      retVal = Csm_SignatureVerify(KEYM_CERT(KEYM_CERT(certificateId).cnfg->certUpperCertId).cnfg->csmSignatureVerifyJobId,
                                   CRYPTO_OPERATIONMODE_SINGLECALL,
                                   KEYM_CERT(certificateId).cnfg->certTbsCertificate->dataPtr,
                                   KEYM_CERT(certificateId).cnfg->certTbsCertificate->dataLength,
                                   KEYM_CERT(certificateId).cnfg->certElementDataSignatureValue->dataPtr,
                                   KEYM_CERT(certificateId).cnfg->certElementDataSignatureValue->dataLength,
                                   &KeyM_SignatureVerifyResult
                                  );
      if (   (E_OK                    == retVal                                                            )
          && (CRYPTO_PROCESSING_ASYNC == KEYM_CERT(certificateId).cnfg->csmSignatureVerifyJobProcessingType)
         )
      {
        /* CHECK: NOPARSE */
        /* Test system is not capable of calling callbacks asynchronous, so this
         * path is not coverable */
        /* Deviation TASKING-2 */
        while (FALSE == KeyM_CsmCallback)
        {
          /* wait for Csm to finish */
        }
         /* CHECK: PARSE */
        KeyM_CsmCallback = FALSE;
      }
    }

    if (CRYPTO_E_VER_OK == KeyM_SignatureVerifyResult)
    {
      retVal = E_OK;
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  if (E_OK == retVal)
  {
    /* !LINKSTO SWS_KeyM_00134,1 */
    KEYM_CERT(certificateId).data->certStatus = KEYM_CERTIFICATE_VALID;
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_Verify_Async/00001,2 */
    retVal = Csm_KeyElementSet(KEYM_CERT(certificateId).cnfg->csmSignatureVerifyKeyId,
                               1U,
                               KEYM_CERT(certificateId).cnfg->certElementDataPublicKey->dataPtr,
                               KEYM_CERT(certificateId).cnfg->certElementDataPublicKey->dataLength
                              );
    if (E_OK == retVal)
    {
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_Verify_Async/00002,1 */
      retVal = Csm_KeySetValid(KEYM_CERT(certificateId).cnfg->csmSignatureVerifyKeyId);
    }
#if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON)
    else
    {
      /* !LINKSTO KeyM.Dsn.Gen/KEYM_E_CSMKEYELEMENTSET_FAILED/00002,1 */
      (void) Det_ReportError(KEYM_MODULE_ID,
                             KEYM_INSTANCE_ID,
                             KEYM_SID_MAINBACKGROUNDFUNCTION,
                             KEYM_E_CSMKEYELEMENTSET_FAILED
                            );
    }
#endif /* #if (KEYM_API_ENABLED_DEVERRORDETECT == STD_ON) */
  }
  else
  {
    /* !LINKSTO SWS_KeyM_00034,1 */
    KEYM_CERT(certificateId).data->certStatus = KEYM_E_CERTIFICATE_SIGNATURE_FAIL;
  }
  return retVal;
}
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON) */

/* --- KeyM_CsmSignatureVerifyCallback ---------------------------------------------------------------- */

/* Called by Csm after finishing signature verification */
/* !LINKSTO KeyM.Req.Gen/CsmSignatureVerifyCallback,1 */
#if (KEYM_API_ENABLED_CSMCALLBACK == STD_ON)
FUNC(void, KEYM_CODE) KeyM_CsmSignatureVerifyCallback
(
  P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job,
          Std_ReturnType                            result
)
{
  TS_PARAM_UNUSED(job);
  TS_PARAM_UNUSED(result);
  KeyM_CsmCallback = TRUE;
}
#endif /* #if (KEYM_API_ENABLED_CSMCALLBACK == STD_ON) */

/* --- KeyM_Push -------------------------------------------------------------------------------- */

static FUNC(void, KEYM_CODE) KeyM_Push
(
  KeyM_CertificateIdType item
)
{
  /* Stack size is checked during generation. This function will only be called with a chain
   * that fits in the stack, hence this function can not fail.
   */
  KeyM_Stack.top                   = KeyM_Stack.top + 1U;
  KeyM_Stack.stack[KeyM_Stack.top] = item;
}

/* --- KeyM_Pop --------------------------------------------------------------------------------- */

static FUNC(Std_ReturnType, KEYM_CODE) KeyM_Pop
(
  KeyM_CertificateIdType* item
)
{
  Std_ReturnType retVal;

  /* comparing to uint16 max catches the overflow which means the stack is empty. */
  if (KEYM_UINT16_MAX == KeyM_Stack.top)
  {
    /* stack empty */
    retVal = E_NOT_OK;
  }
  else
  {
    *item          = KeyM_Stack.stack[KeyM_Stack.top];
    /* overflow is done on purpose here. */
    KeyM_Stack.top = KeyM_Stack.top - 1U;
    retVal         = E_OK;
  }
  return retVal;
}

/* --- KeyM_BufferCmp --------------------------------------------------------------------------- */

static FUNC(Std_ReturnType, KEYM_CODE) KeyM_BufferCmp
(
  P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) left,
          uint32                            leftLength,
  P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) right,
          uint32                            rightLength
)
{
  Std_ReturnType retVal = E_NOT_OK;

  if (leftLength == rightLength)
  {
    retVal = TS_MemCmp(left, right, leftLength);
  }
  return retVal;
}

/* --- KeyM_X509NameCmp ------------------------------------------------------------------------- */

static FUNC(Std_ReturnType, KEYM_CODE) KeyM_X509NameCmp
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) leftPtr,
        uint32                              leftLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_APPL_DATA) rightPtr,
        uint32                              rightLength
)
{
  Std_ReturnType retVal;

  retVal = KeyM_BufferCmp(leftPtr,
                          leftLength,
                          rightPtr,
                          rightLength
                         );
  return retVal;
}

/* --- KeyM_InitHelper -------------------------------------------------------------------------- */

#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON)
static FUNC(void, KEYM_CODE) KeyM_InitHelper
(
  void
)
{
#if (KEYM_API_ENABLED_NVM == STD_ON)
  Std_ReturnType        retVal;
  uint32                i;
  NvM_RequestResultType requestResult = NVM_REQ_NOT_OK;

  for (i = 0U; i < KEYM_CERT_COUNT; i++)
  {
    /* !LINKSTO SWS_KeyM_00022,1 */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_Init_Async/00003,1 */
    if (KEYM_STORAGE_IN_NVM == KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).cnfg->keyStorage)
    {
      /* read block */
      if (E_OK == NvM_ReadBlock(KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).cnfg->nvmBlockDescriptorId,
                                KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).cnfg->keyData
                               )
         )
      {
        /* Poll read status */
        do
        {
          retVal = NvM_GetErrorStatus(KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).cnfg->nvmBlockDescriptorId,
                                      &requestResult
                                     );
        }
        while ((E_OK == retVal) && (NVM_REQ_PENDING == requestResult));
      }
      if (NVM_REQ_OK == requestResult)
      {
        KEYM_CERT(i).data->certStatus = KEYM_CERTIFICATE_NOT_PARSED;
      }
      /* !LINKSTO KeyM.Dsn.Alg/KeyM_Init_Async/00004,1 */
      else
      if (NVM_REQ_RESTORED_FROM_ROM == requestResult)
      {
        /* this is not a valid case. Reset buffer and keep status not available. */
        TS_MemBZero(KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).cnfg->keyData,
                    KEYM_KEY(KEYM_CERT(i).cnfg->keymStorageKey).data->keyLength
                   );
      }
      else
      {
        /* block could not be restored. Keep status not available. */
      }
    }
  }
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */
  /* !LINKSTO KeyM.Dsn.Alg/KeyM_Init_Async/00002,1 */
  KeyM_State = KEYM_STATE_IDLE;
}
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON) */

/* --- KeyM_VerifyHelper ------------------------------------------------------------------------ */

#if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON)
static FUNC(void, KEYM_CODE) KeyM_VerifyHelper
(
  void
)
{
  Std_ReturnType         retVal         = E_NOT_OK;
  boolean                chainIsInvalid = FALSE;
  KeyM_ResultType        keyMRetVal     = KEYM_RT_NOT_OK;
#if (KEYM_API_ENABLED_NVM == STD_ON)
  NvM_RequestResultType  requestResult  = NVM_REQ_NOT_OK;
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */

  /* check if verifying is requested */
  if (KeyM_FlgVerifyRequested)
  {
    if (   (KEYM_E_CERTIFICATE_INVALID_FORMAT  == KEYM_CERT(KeyM_CertVrfyIdCur).data->certStatus)
        || (KEYM_E_CERTIFICATE_INVALID_CONTENT == KEYM_CERT(KeyM_CertVrfyIdCur).data->certStatus)
        || (KEYM_E_CERTIFICATE_INVALID_TYPE    == KEYM_CERT(KeyM_CertVrfyIdCur).data->certStatus)
       )
    {
      /* this path can only be reached if verify is called. */
      KeyM_FlgVerifyRequested = FALSE;
      if (NULL_PTR != KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymCbkCertificateVerify)
      {
        /* !LINKSTO SWS_KeyM_00153,1 */
        (void) KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymCbkCertificateVerify(KeyM_CertVrfyIdCur, KEYM_CERT(KeyM_CertVrfyIdCur).data->certStatus);
      }
      KeyM_State = KEYM_STATE_IDLE;
    }
    else
    {
      KeyM_CertVrfyChainIdCur = KeyM_CertVrfyIdCur;
      KeyM_Push(KeyM_CertVrfyIdCur);
      /* check if current cert is root cert and stop. */
      while (KeyM_CertVrfyChainIdCur != KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certUpperCertId)
      {
        KeyM_CertVrfyChainIdCur = KEYM_CERT(KeyM_CertVrfyChainIdCur).cnfg->certUpperCertId;
        KeyM_Push(KeyM_CertVrfyChainIdCur);
      }
      while (E_OK == KeyM_Pop(&KeyM_CertVrfyChainIdCur))
      {
        if (chainIsInvalid)
        {
          /* this path can only be reached if the error is in the chain itself. Checking for
            * errors describing the certificate is not needed here.
            */
          KEYM_CERT(KeyM_CertVrfyChainIdCur).data->certStatus = KEYM_E_CERTIFICATE_INVALID_CHAIN_OF_TRUST;
        }
        else
        {
          /* no certificate of the chain is empty so this check is enough. */
          if (!KEYM_CERT(KeyM_CertVrfyChainIdCur).data->flgParsed)
          {
            /* parse certificate */
            KeyM_ParseHelper(KeyM_CertVrfyChainIdCur);
            if (KEYM_CERTIFICATE_PARSED_NOT_VALIDATED != KEYM_CERT(KeyM_CertVrfyChainIdCur).data->certStatus)
            {
              chainIsInvalid = TRUE;
              if (KeyM_CertVrfyChainIdCur == KeyM_CertVrfyIdCur)
              {
                keyMRetVal = KEYM_RT_KEY_CERT_INVALID;
              }
              else
              {
                keyMRetVal = KEYM_RT_CERT_INVALID_CHAIN_OF_TRUST;
              }
              continue;
            }
          }
          retVal = KeyM_CompareAndVerify(KeyM_CertVrfyChainIdCur);
          if(E_OK == retVal)
          {
            keyMRetVal = KEYM_RT_OK;
          }
          else
          {
            keyMRetVal                         = KEYM_RT_CERT_INVALID_CHAIN_OF_TRUST;
            chainIsInvalid                     = TRUE;
          }
        }
      }
#if (KEYM_API_ENABLED_NVM == STD_ON)
      if (TRUE == KEYM_CERT(KeyM_CertVrfyIdCur).data->flgServiceRequested)
      {
        if (   KEYM_STORAGE_IN_NVM
            == KEYM_KEY(KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymStorageKey).cnfg->keyStorage
            )
        {
          /* !LINKSTO KeyM.Dsn.Alg/KeyM_ServiceCertificate/00014,1 */
          if (E_OK == NvM_WriteBlock(KEYM_KEY(KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymStorageKey).cnfg->nvmBlockDescriptorId,
                                      KEYM_KEY(KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymStorageKey).cnfg->keyData
                                    )
              )
          {
            /* Poll write status */
            do
            {
              retVal = NvM_GetErrorStatus(KEYM_KEY(KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymStorageKey).cnfg->nvmBlockDescriptorId,
                                          &requestResult
                                         );
            }
            while ((E_OK == retVal) && (NVM_REQ_PENDING == requestResult));
          }
          if (NVM_REQ_OK != requestResult)
          {
            keyMRetVal = KEYM_RT_KEY_CERT_WRITE_FAIL;
          }
        }
      }
#endif /* #if (KEYM_API_ENABLED_NVM == STD_ON) */
      /* check which callback to call */
      /* !LINKSTO SWS_KeyM_00152,1 */
      if (TRUE == KEYM_CERT(KeyM_CertVrfyIdCur).data->flgServiceRequested)
      {
        KEYM_CERT(KeyM_CertVrfyIdCur).data->flgServiceRequested = FALSE;
        if (NULL_PTR != KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymCbkServiceCertificate)
        {
          /* !LINKSTO SWS_KeyM_00149,1 */
          KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymCbkServiceCertificate(KeyM_CertVrfyIdCur, keyMRetVal, 0U, NULL_PTR);
        }
      }
      else
      {
        if (NULL_PTR != KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymCbkCertificateVerify)
        {
          /* !LINKSTO SWS_KeyM_00153,1 */
          (void) KEYM_CERT(KeyM_CertVrfyIdCur).cnfg->keymCbkCertificateVerify(KeyM_CertVrfyIdCur, KEYM_CERT(KeyM_CertVrfyIdCur).data->certStatus);
        }
      }
      /* !LINKSTO SWS_KeyM_00153,1 */
      KeyM_FlgVerifyRequested = FALSE;
      KeyM_State         = KEYM_STATE_IDLE;
    }
  }
}
#endif /* #if (KEYM_API_ENABLED_BACKGROUND_MAINFUNCTION == STD_ON) */

/* --- KeyM_ParseHelper ------------------------------------------------------------------------- */

static FUNC(void, KEYM_CODE) KeyM_ParseHelper
(
  KeyM_CertificateIdType certificateId
)
{
  KeyM_ParseResultType retVal;

  /* parse certificate */
  retVal = KeyM_GetX509Certificate(KEYM_KEY(KEYM_CERT(certificateId).cnfg->keymStorageKey).cnfg->keyData,
                                   KEYM_KEY(KEYM_CERT(certificateId).cnfg->keymStorageKey).data->keyLength,
                                   KEYM_CERT(certificateId).cnfg
                                  );
  if(KEYM_PARSERESULT_OK == retVal)
  {
    if(KeyM_CertElemVerification_Helper(certificateId))
    {
      /* !LINKSTO SWS_KeyM_00137,1 */
      KEYM_CERT(certificateId).data->certStatus = KEYM_CERTIFICATE_PARSED_NOT_VALIDATED;
    }
    else
    {
      /* !LINKSTO KeyM.Req.Xdm/KeyMCertificateElementRule/General/00002,1 */
      KEYM_CERT(certificateId).data->certStatus = KEYM_E_CERTIFICATE_INVALID_CONTENT;
    }
  }
  else
  if (KEYM_PARSERESULT_INVALID_TYPE == retVal)
  {
    KEYM_CERT(certificateId).data->certStatus = KEYM_E_CERTIFICATE_INVALID_TYPE;
  }
  else
  if (KEYM_PARSERESULT_INVALID_FORMAT == retVal)
  {
    /* !LINKSTO SWS_KeyM_00032,2 */
    /* !LINKSTO KeyM.Dsn.Alg/KeyM_CertGetStatus/00003,1 */
    KEYM_CERT(certificateId).data->certStatus = KEYM_E_CERTIFICATE_INVALID_FORMAT;
  }
  /* CHECK: NOPARSE */
  else
  if (KEYM_PARSERESULT_INVALID_CONTENT == retVal)
  {
    /* !LINKSTO SWS_KeyM_00031,2 */
    KEYM_CERT(certificateId).data->certStatus = KEYM_E_CERTIFICATE_INVALID_CONTENT;
  }
  else
  {
    /* Caution: this else is required by MISRA-C:2012 */
  }
  /* CHECK: PARSE */
  KEYM_CERT(certificateId).data->flgParsed = TRUE;
}

/* --- KeyM_CertElemVerification_Helper --------------------------------------------------------- */

static FUNC(boolean, KEYM_CODE) KeyM_CertElemVerification_Helper
(
  KeyM_CertificateIdType certificateId
)
{
  uint16  i;
  boolean retVal = TRUE;
  for(i = 0U; ((i < KEYM_CERT(certificateId).cnfg->certElementRulesNumber) && (TRUE == retVal)); i++)
  {
    retVal = (KEYM_CERT(certificateId).cnfg->certElementRules)[i]();
  }
  return retVal;
}

#define KEYM_STOP_SEC_CODE
#include <KeyM_MemMap.h>

/*==================[end of file]=================================================================*/

