/**
 * \file
 *
 * \brief AUTOSAR TcpIp
 *
 * This file contains the implementation of the AUTOSAR
 * module TcpIp.
 *
 * \version 3.5.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * 1) Polymorphic (derived) data structures need an explicit pointer-to-base-class cast and
 * then another explicit pointer-to-derived-class cast in C.
 *
 * MISRAC2012-3) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses.
 *
 * Reason:
 * Parentheses defeat the purpose of a compile-time-if macro: e.g. it could not be used
 * within a parameter list, where the additional parenthesis are harmful.
 *
 */

/*==================[inclusions]============================================*/

#include <Std_Types.h>              /* AUTOSAR standard types */
#include <TSMem.h>                  /* TS_MemCpy, TS_MemCmp */
#include <TcpIp_Cfg.h>              /* TcpIp pre-compile-time configuration */

#if ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) || (TCPIP_TCP_CRYPTO_ENABLED == STD_ON))

#include <TcpIp_guards.h>     /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Crypto.h>    /* own SW-unit private header file */

/*==================[macros]================================================*/

/* maximal expected nonce size (salt + iv) for AES calculations */
#define TCPIP_CRYPTO_MAX_NONCE_SIZE                                    12U

/*==================[type definitions]======================================*/

typedef P2FUNC(Std_ReturnType, TCPIP_CODE, TcpIp_Crypto_GenerateFctPtrType)
(
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) outputPtr,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) outputLengthPtr
);

/* forward declaration */
struct TcpIp_Cypto_OperationFunctor;

typedef P2FUNC(Std_ReturnType, TCPIP_CODE, TcpIp_Crypto_OperationFctPtrType)
(
  /* Deviation MISRAC2012-3 */
  P2CONST(struct TcpIp_Cypto_OperationFunctor, AUTOMATIC, TCPIP_APPL_DATA) that,
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint32 dataLength
);

typedef struct TcpIp_Cypto_OperationFunctor
{
  TcpIp_Crypto_OperationFctPtrType operationFctPtr;
} TcpIp_Crypto_OperationFunctorType;

typedef struct
{
  TcpIp_Crypto_OperationFctPtrType operationFctPtr;
  TcpIp_Crypto_GenerateFctPtrType generateFctPtr;
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) outputPtr;
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) outputLengthPtr;
} TcpIp_Crypto_GenerateFunctorType;

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
typedef P2FUNC(Std_ReturnType, TCPIP_CODE, TcpIp_Crypto_VerifyFctPtrType)
(
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint32 dataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) outputPtr,
  uint32 macLength,
  P2VAR(Crypto_VerifyResultType,  AUTOMATIC, TCPIP_APPL_DATA) verifyPtr
);

typedef struct
{
  TcpIp_Crypto_OperationFctPtrType operationFctPtr;
  TcpIp_Crypto_VerifyFctPtrType verifyFctPtr;
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) referencePtr;
  uint32 referenceLength;
} TcpIp_Crypto_VerifyFunctorType;

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

/*==================[external function declarations]========================*/


/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE == STD_ON) ||  \
    (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY == STD_ON)     ||  \
    (TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY == STD_ON) ||  \
    (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE == STD_ON))

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Crypto_operation
(
  uint32 jobId,
  P2CONST(TcpIp_Crypto_OperationFunctorType, AUTOMATIC, TCPIP_APPL_DATA) operationFunctorPtr,
  P2CONST(TcpIp_Crypto_FragmentType, AUTOMATIC, TCPIP_APPL_DATA) dataFragsPtr,
  uint8 numOfDataFragments
);
#endif /* ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE == STD_ON) ||
           (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY == STD_ON)     ||
           (TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY == STD_ON) ||
           (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE == STD_ON)) */

#if ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE == STD_ON) ||  \
     (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE == STD_ON))
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Crypto_generate
(
  P2CONST(TcpIp_Crypto_OperationFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint32 dataLength
);
#endif /* ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE == STD_ON) ||
           (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE == STD_ON)) */

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
#if ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY == STD_ON) ||  \
     (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY == STD_ON))
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Crypto_verify
(
  P2CONST(TcpIp_Crypto_OperationFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint32 dataLength
);
#endif /* ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY == STD_ON) ||
           (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY == STD_ON)) */

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE == STD_ON) ||  \
     (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE == STD_ON))

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Crypto_MacGenerate
(
  uint32 jobId,
  P2CONST(TcpIp_Crypto_FragmentType, AUTOMATIC, TCPIP_APPL_DATA) dataFragsPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) macPtr,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) macLengthPtr,
  uint8 numOfDataFragments
)
{
  Std_ReturnType returnValue = E_NOT_OK;
  TcpIp_Crypto_GenerateFunctorType macGenerateFunctor;
  /* Deviation MISRAC2012-1 */
  P2VAR(TcpIp_Crypto_OperationFunctorType, AUTOMATIC, TCPIP_APPL_DATA) operationFunctorPtr =
    (P2VAR(TcpIp_Crypto_OperationFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) &macGenerateFunctor;

  macGenerateFunctor.generateFctPtr = &Csm_MacGenerate;
  macGenerateFunctor.operationFctPtr = &TcpIp_Crypto_generate;
  macGenerateFunctor.outputLengthPtr = macLengthPtr;
  macGenerateFunctor.outputPtr = macPtr;

  returnValue =
    TcpIp_Crypto_operation(jobId, operationFunctorPtr, dataFragsPtr, numOfDataFragments);

  return returnValue;
}

#endif /* ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE == STD_ON) ||
           (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE == STD_ON))*/

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
#if ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY == STD_ON) ||  \
     (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY == STD_ON))

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Crypto_MacVerify
(
  uint32 jobId,
  P2CONST(TcpIp_Crypto_FragmentType, AUTOMATIC, TCPIP_APPL_DATA) dataFragsPtr,
  uint8 numOfDataFragments,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) macPtr,
  uint8 macLength
)
{
  Std_ReturnType returnValue = E_NOT_OK;
  TcpIp_Crypto_VerifyFunctorType macVerifyFunctor;
  /* Deviation MISRAC2012-1 */
  CONSTP2CONST(TcpIp_Crypto_OperationFunctorType, AUTOMATIC, TCPIP_APPL_DATA) operationFunctorPtr =
    (CONSTP2CONST(TcpIp_Crypto_OperationFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) &macVerifyFunctor;

  macVerifyFunctor.verifyFctPtr = &Csm_MacVerify;
  macVerifyFunctor.operationFctPtr = &TcpIp_Crypto_verify;
  macVerifyFunctor.referencePtr = macPtr;
  macVerifyFunctor.referenceLength = (uint32) macLength * 8U; /* length in bits */

  returnValue =
    TcpIp_Crypto_operation(jobId, operationFunctorPtr, dataFragsPtr, numOfDataFragments);

  return returnValue;
}

#endif /* ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY == STD_ON) ||
           (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY == STD_ON)) */

#if ((TCPIP_IPSEC_GMAC_ENABLED == STD_ON))

#if (TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYELEMENTSET == STD_ON)

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Crypto_KeyElementSet
(
  uint32 keyId,
  uint32 keyElementId,
  P2CONST(uint8,  AUTOMATIC, TCPIP_APPL_DATA) keyPtr,
  uint32 keyLength
)
{
  Std_ReturnType result = Csm_KeyElementSet(keyId,
                                            keyElementId,
                                            keyPtr,
                                            keyLength
                                           );

  if(result == E_OK)
  {
    (void) Csm_KeySetValid(keyId);
  }

  return result;
}

#endif /* (TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYELEMENTSET == STD_ON) */

#endif /* ((TCPIP_IPSEC_GMAC_ENABLED == STD_ON)) */

#if ((TCPIP_IPSEC_GMAC_ENABLED == STD_ON))

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Crypto_setNonce
(
  uint32 keyId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) initializationVectorPtr,
  uint16 initializationVectorSize,
  uint8 saltSize
)
{
  Std_ReturnType returnValue = E_NOT_OK;
  /* contains salt + iv */
  uint8 nonce[TCPIP_CRYPTO_MAX_NONCE_SIZE] = {0U};
  uint32 nonceSize = ((uint32)saltSize + (uint32)initializationVectorSize);

  TCPIP_PRECONDITION_ASSERT(nonceSize <= TCPIP_CRYPTO_MAX_NONCE_SIZE, TCPIP_INTERNAL_API_ID);

  /* get the generated key for salt first */
  if(TcpIp_Crypto_KeyElementGet(keyId, CRYPTO_KE_CIPHER_IV, nonce, &nonceSize) == E_OK)
  {
    nonceSize = (uint32)saltSize + (uint32)initializationVectorSize;

    /* combine salt with random initialization vector */
    TS_MemCpy(&nonce[saltSize], initializationVectorPtr, initializationVectorSize);

    /* set the nonce */
    returnValue = TcpIp_Crypto_KeyElementSet(keyId, CRYPTO_KE_CIPHER_IV, nonce, nonceSize);
  }

  return returnValue;
}

#endif /* ((TCPIP_IPSEC_GMAC_ENABLED == STD_ON)) */
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

/*==================[internal function definitions]=========================*/

#if((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE == STD_ON) ||  \
    (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY == STD_ON)     ||  \
    (TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY == STD_ON) ||  \
    (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE == STD_ON))

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Crypto_operation
(
  uint32 jobId,
  P2CONST(TcpIp_Crypto_OperationFunctorType, AUTOMATIC, TCPIP_APPL_DATA) operationFunctorPtr,
  P2CONST(TcpIp_Crypto_FragmentType, AUTOMATIC, TCPIP_APPL_DATA) dataFragsPtr,
  uint8 numOfDataFragments
)
{
  Std_ReturnType returnValue = E_NOT_OK;
  uint8 i = 0U, j = 0U;
  uint8 numOfVerifyCalls = 0U;
  Crypto_OperationModeType operationMode;

  if(numOfDataFragments == 1U)
  {
    /* if data is not fragmented call mac verify a single time */
    operationMode = CRYPTO_OPERATIONMODE_SINGLECALL;
    numOfVerifyCalls = 1U;
  }
  else
  {
    /* if data is fragmented perform start, update, finish */
    operationMode = CRYPTO_OPERATIONMODE_START;
    /* add two more call one for start and one for finish */
    numOfVerifyCalls = 2U + numOfDataFragments;
  }

  for(i = 0U; (i < numOfVerifyCalls); i++)
  {
    /* call crypto operation for every data fragment */
    returnValue = operationFunctorPtr->operationFctPtr
                    (
                     operationFunctorPtr,
                     jobId,
                     operationMode,
                     dataFragsPtr[j].fragDataPtr,
                     dataFragsPtr[j].fragDataLength
                    );

    if(returnValue == E_OK)
    {
      if(i == 0U)
      {
        /* process next data with update */
        operationMode = CRYPTO_OPERATIONMODE_UPDATE;
      }
      else if(i >= (numOfVerifyCalls - 2U))
      {
        /* in last iteration do the finish call */
        operationMode = CRYPTO_OPERATIONMODE_FINISH;
      }
      else
      {
        /* process next data with update */
        j++;
      }
    }
    else
    {
      /* error occurred stop processing */
      break;
    }
  }

  return returnValue;
}

#endif /* ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE == STD_ON) ||
           (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY == STD_ON)     ||
           (TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY == STD_ON) ||
           (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE == STD_ON)) */

#if ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE == STD_ON) ||  \
     (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE == STD_ON))
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Crypto_generate
(
  P2CONST(TcpIp_Crypto_OperationFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint32 dataLength
)
{
  /* Deviation MISRAC2012-1 */
  P2CONST(TcpIp_Crypto_GenerateFunctorType, AUTOMATIC, TCPIP_APPL_DATA) this =
    (P2CONST(TcpIp_Crypto_GenerateFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) that;

  return this->generateFctPtr(jobId, mode, dataPtr, dataLength, this->outputPtr,  this->outputLengthPtr);
}
#endif /* ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE == STD_ON) ||
           (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE == STD_ON)) */

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
#if ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY == STD_ON) ||  \
     (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY == STD_ON))

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Crypto_verify
(
  P2CONST(TcpIp_Crypto_OperationFunctorType, AUTOMATIC, TCPIP_APPL_DATA) that,
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint32 dataLength
)
{
  Crypto_VerifyResultType VerifyResult;
  Std_ReturnType operationResult = E_NOT_OK, functionResult;

  /* Deviation MISRAC2012-1 */
  P2CONST(TcpIp_Crypto_VerifyFunctorType, AUTOMATIC, TCPIP_APPL_DATA) this =
    (P2CONST(TcpIp_Crypto_VerifyFunctorType, AUTOMATIC, TCPIP_APPL_DATA)) that;

  functionResult =
    this->verifyFctPtr(jobId,
                       mode,
                       dataPtr,
                       dataLength,
                       this->referencePtr,
                       this->referenceLength,
                       &VerifyResult);

  /* operation mode will always be CRYPTO_OPERATIONMODE_SINGLECALL for manual SA,
   * ESN is not used and no data is verified after the payload
  if((mode == CRYPTO_OPERATIONMODE_FINISH) || (mode == CRYPTO_OPERATIONMODE_SINGLECALL))*/
  {
    if((functionResult == E_OK) && (VerifyResult == CRYPTO_E_VER_OK))
    {
      /* the mac or signature is valid */
      operationResult = E_OK;
    }
  }
  /* not used for MANUAL only SA, needed for IKE and ESN
  else
  {
    operationResult = functionResult;
  }*/

  return operationResult;
}

#endif /* ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY == STD_ON) ||
           (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY == STD_ON)) */
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) || (TCPIP_TCP_CRYPTO_ENABLED == STD_ON)) */

/*==================[end of file]===========================================*/
