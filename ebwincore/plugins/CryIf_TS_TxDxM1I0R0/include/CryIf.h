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
#ifndef CRYIF_H
#define CRYIF_H

/* !LINKSTO SWS_CryIf_00005, 1 */

/*==================[misra deviations]============================================================*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.6 (required)
 * An identifier with external linkage shall have exactly one external definition.
 *
 * Reason:
 * Definition exists in generic file so a static test before the generation may cause a violation.
 */

/*==================[inclusions]==================================================================*/

/* !LINKSTO SWS_CryIf_00011, 1
   !LINKSTO SWS_CryIf_00008, 1
 */
#include <Csm_Types.h>
#include <CryIf_Cfg.h>
#include <CryIf_Version.h>

/*==================[macros]======================================================================*/

/* !LINKSTO CryIf.Dsn.Api/Macros/00007, 1 */
#if (defined CRYIF_INSTANCE_ID)
#error CRYIF_INSTANCE_ID already defined
#endif
/** \brief Instance ID of the Crypto Interface */
#define CRYIF_INSTANCE_ID             0x00U

/*------------------[Development error codes]-----------------------------------------------------*/
/* !LINKSTO SWS_CryIf_00009, 1 */
#if (defined CRYIF_E_UNINIT)
#error CRYIF_E_UNINIT already defined
#endif
/** \brief Error Code for uninitialized module */
#define CRYIF_E_UNINIT                0x00U

#if (defined CRYIF_E_INIT_FAILED)
#error CRYIF_E_INIT_FAILED already defined
#endif
/** \brief Error Code for init failed */
#define CRYIF_E_INIT_FAILED           0x01U

#if (defined CRYIF_E_PARAM_POINTER)
#error CRYIF_E_PARAM_POINTER already defined
#endif
/** \brief Error Code for invalid pointer */
#define CRYIF_E_PARAM_POINTER         0x02U

#if (defined CRYIF_E_PARAM_HANDLE)
#error CRYIF_E_PARAM_HANDLE already defined
#endif
/** \brief Error Code for invalid handle */
#define CRYIF_E_PARAM_HANDLE          0x03U

#if (defined CRYIF_E_PARAM_VALUE)
#error CRYIF_E_PARAM_VALUE already defined
#endif
/** \brief Error Code for invalid value */
#define CRYIF_E_PARAM_VALUE           0x04U

#if (defined CRYIF_E_KEY_SIZE_MISMATCH)
#error CRYIF_E_KEY_SIZE_MISMATCH already defined
#endif
/** \brief Error Code for key size mismatch */
#define CRYIF_E_KEY_SIZE_MISMATCH     0x05U

/*------------------[ Service ID ]----------------------------------------------------------------*/
/* !LINKSTO CryIf.Dsn.Api/Macros/00011, 1 */

#if (defined CRYIF_SID_INIT)
#error CRYIF_SID_INIT already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_Init */
#define CRYIF_SID_INIT                  0x00U

#if (defined CRYIF_SID_GETVERSIONINFO)
#error CRYIF_SID_GETVERSIONINFO already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_GetVersionInfo */
#define CRYIF_SID_GETVERSIONINFO        0x01U

#if (defined CRYIF_SID_PROCESSJOB)
#error CRYIF_SID_PROCESSJOB already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_ProcessJob */
#define CRYIF_SID_PROCESSJOB            0x03U

#if (defined CRYIF_SID_KEYELEMENTSET)
#error CRYIF_SID_KEYELEMENTSET already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_KeyElementSet */
#define CRYIF_SID_KEYELEMENTSET         0x04U

#if (defined CRYIF_SID_KEYSETVALID)
#error CRYIF_SID_KEYSETVALID already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_KeySetValid */
#define CRYIF_SID_KEYSETVALID           0x05U

#if (defined CRYIF_SID_KEYELEMENTGET)
#error CRYIF_SID_KEYELEMENTGET already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_KeyElementGet */
#define CRYIF_SID_KEYELEMENTGET         0x06U

#if (defined CRYIF_SID_KEYELEMENTCOPY)
#error CRYIF_SID_KEYELEMENTCOPY already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_KeyElementCopy */
#define CRYIF_SID_KEYELEMENTCOPY        0x0FU

#if (defined CRYIF_SID_KEYCOPY)
#error CRYIF_SID_KEYCOPY already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_KeyCopy */
#define CRYIF_SID_KEYCOPY               0x10U

#if (defined CRYIF_SID_RANDOMSEED)
#error CRYIF_SID_RANDOMSEED already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_RandomSeed */
#define CRYIF_SID_RANDOMSEED            0x07U

#if (defined CRYIF_SID_KEYGENERATE)
#error CRYIF_SID_KEYGENERATE already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_KeyGenerate */
#define CRYIF_SID_KEYGENERATE           0x08U

#if (defined CRYIF_SID_KEYDERIVE)
#error CRYIF_SID_KEYDERIVE already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_KeyDerive */
#define CRYIF_SID_KEYDERIVE             0x09U

#if (defined CRYIF_SID_KEYEXCHANGECALCPUBVAL)
#error CRYIF_SID_KEYEXCHANGECALCPUBVAL already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_KeyExchangeCalcPubVal */
#define CRYIF_SID_KEYEXCHANGECALCPUBVAL 0x0AU

#if (defined CRYIF_SID_KEYEXCHANGECALCSECRET)
#error CRYIF_SID_KEYEXCHANGECALCSECRET already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_KeyExchangeCalcSecret */
#define CRYIF_SID_KEYEXCHANGECALCSECRET 0x0BU

#if (defined CRYIF_SID_CERTIFICATEPARSE)
#error CRYIF_SID_CERTIFICATEPARSE already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_CertificateParse */
#define CRYIF_SID_CERTIFICATEPARSE      0x0CU

#if (defined CRYIF_SID_CERTIFICATEVERIFY)
#error CRYIF_SID_CERTIFICATEVERIFY already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_CertificateVerify */
#define CRYIF_SID_CERTIFICATEVERIFY     0x11U

#if (defined CRYIF_SID_CANCELJOB)
#error CRYIF_SID_CANCELJOB already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_CancelJob */
#define CRYIF_SID_CANCELJOB             0x0EU

#if (defined CRYIF_SID_CALLBACKNOTIFICATION)
#error CRYIF_SID_CALLBACKNOTIFICATION already defined
#endif
/** \brief AUTOSAR API service ID for CryIf_CallbackNotification */
#define CRYIF_SID_CALLBACKNOTIFICATION  0x0DU

/*==================[declaration of types]========================================================*/

/** \brief Function pointer type for CryIf_ProcessJob */
/* !LINKSTO CryIf.Dsn.Api/Types/00001, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_ProcessJobPtrType)
(
  uint32,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_DATA)
);

/** \brief Function pointer type for CryIf_CancelJob */
/* !LINKSTO CryIf.Dsn.Api/Types/00002, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_CancelJobPtrType)
(
  uint32,

#if (   (CRYIF_API_VERSION == CRYIF_API_VERSION_430) \
     || (CRYIF_API_VERSION == CRYIF_API_VERSION_431) \
    )
  P2VAR(Crypto_JobInfoType, AUTOMATIC, CRYIF_APPL_DATA)
#endif /* #if (   (CRYIF_API_VERSION == CRYIF_API_VERSION_430) ... */

#if (CRYIF_API_VERSION == CRYIF_API_VERSION_EB)
  P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_DATA)
#endif /* #if (CRYIF_API_VERSION == CRYIF_API_VERSION_EB) */

);

/** \brief Function pointer type for CryIf_KeyElementSet */
/* !LINKSTO CryIf.Dsn.Api/Types/00003, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_KeyElementSetPtrType)
(
  uint32,
  uint32,
  P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA),
  uint32
);

/** \brief Function pointer type for CryIf_KeySetValid */
/* !LINKSTO CryIf.Dsn.Api/Types/00004, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_KeySetValidPtrType)
(
  uint32
);

/** \brief Function pointer type for CryIf_KeyElementGet */
/* !LINKSTO CryIf.Dsn.Api/Types/00005, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_KeyElementGetPtrType)
(
  uint32,
  uint32,
  P2VAR(uint8, AUTOMATIC, CRYIF_APPL_DATA),
  P2VAR(uint32, AUTOMATIC, CRYIF_APPL_DATA)
);

/** \brief Function pointer type for CryIf_KeyElementCopy */
/* !LINKSTO CryIf.Dsn.Api/Types/00006, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_KeyElementCopyPtrType)
(
  uint32,
  uint32,
  uint32,
  uint32
);

/** \brief Function pointer type for CryIf_KeyElementIds */
/* !LINKSTO CryIf.Dsn.Api/Types/00007, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_KeyElementIdsPtrType)
(
  uint32,
  P2VAR(uint32, AUTOMATIC, CRYIF_APPL_DATA), P2VAR(uint32, AUTOMATIC, CRYIF_APPL_DATA)
);

/** \brief Function pointer type for CryIf_RandomSeed */
/* !LINKSTO CryIf.Dsn.Api/Types/00008, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_RandomSeedPtrType)
(
  uint32,
  P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA),
  uint32
);

/** \brief Function pointer type for CryIf_KeyGenerate */
/* !LINKSTO CryIf.Dsn.Api/Types/00009, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_KeyGeneratePtrType)
(
  uint32
);

/** \brief Function pointer type for CryIf_KeyDerive */
/* !LINKSTO CryIf.Dsn.Api/Types/00010, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_KeyDerivePtrType)
(
  uint32,
  uint32
);

/** \brief Function pointer type for CryIf_KeyExchangeCalcPubVal */
/* !LINKSTO CryIf.Dsn.Api/Types/00011, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_KeyExchangeCalcPubValPtrType)
(
  uint32,
  P2VAR(uint8, AUTOMATIC, CRYIF_APPL_DATA),
  P2VAR(uint32, AUTOMATIC, CRYIF_APPL_DATA)
);

/** \brief Function pointer type for CryIf_KeyExchangeCalcSecret */
/* !LINKSTO CryIf.Dsn.Api/Types/00012, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_KeyExchangeCalcSecretPtrType)
(
  uint32,
  P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) partnerPublicValuePtr,
  uint32
);

/** \brief Function pointer type for CryIf_CertificateParse */
/* !LINKSTO CryIf.Dsn.Api/Types/00013, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_CertificateParsePtrType)
(
  uint32
);

/** \brief Function pointer type for CryIf_CertificateVerify */
/* !LINKSTO CryIf.Dsn.Api/Types/00014, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_CertificateVerifyPtrType)
(
  uint32,
  uint32,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYIF_APPL_DATA)
);

/** \brief Function pointer type for CryIf_KeyCopy */
/* !LINKSTO CryIf.Dsn.Api/Types/00015, 1 */
typedef P2FUNC(Std_ReturnType, CRYIF_CODE, CryIf_KeyCopyPtrType)
(
  uint32,
  uint32
);

/*==================[declaration of external constants]===========================================*/

#define CRYIF_START_SEC_CONST_32
#include <CryIf_MemMap.h>

/* Deviation MISRAC2012-1 <START> */

/** \brief Container for the Crypto Channels */
#if (CRYIF_CHANNEL_COUNT != 0U)
extern CONST(uint32, CRYIF_CONST) CryIf_Channels[CRYIF_CHANNEL_COUNT];
#endif

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief Container to map Crypto Interface Keys to Crypto Driver Keys */
extern CONST(uint32, CRYIF_CONST) CryIf_Keys[CRYIF_KEY_COUNT];
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* Deviation MISRAC2012-1 <STOP> */

#define CRYIF_STOP_SEC_CONST_32
#include <CryIf_MemMap.h>

#define CRYIF_START_SEC_CONST_UNSPECIFIED
#include <CryIf_MemMap.h>

/* Deviation MISRAC2012-1 <START> */

/** \brief ProcessJob Jumptable for different Crypto Driver Objects */
#if (CRYIF_CHANNEL_COUNT != 0U)
extern CONST(CryIf_ProcessJobPtrType, CRYIF_CONST) CryIf_ProcessJobJumpTable[CRYIF_CHANNEL_COUNT];
#endif

/** \brief CancelJob Jumptable for different Crypto Driver Objects */
#if (CRYIF_CHANNEL_COUNT != 0U)
extern CONST(CryIf_CancelJobPtrType, CRYIF_CONST) CryIf_CancelJobJumpTable[CRYIF_CHANNEL_COUNT];
#endif

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief keyElementSet Jumptable for different Crypto Driver Objects */
extern CONST(CryIf_KeyElementSetPtrType, CRYIF_CONST) CryIf_KeyElementSetJumpTable[CRYIF_KEY_COUNT];
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief keySetValid Jumptable for different Crypto Driver Objects */
extern CONST(CryIf_KeySetValidPtrType, CRYIF_CONST) CryIf_KeySetValidJumpTable[CRYIF_KEY_COUNT];
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief keyElementGet Jumptable for different Crypto Driver Objects */
extern CONST(CryIf_KeyElementGetPtrType, CRYIF_CONST) CryIf_KeyElementGetJumpTable[CRYIF_KEY_COUNT];
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief keyElementCopy Jumptable for different Crypto Driver Objects */
extern CONST(CryIf_KeyElementCopyPtrType, CRYIF_CONST) CryIf_KeyElementCopyJumpTable[CRYIF_KEY_COUNT];
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief keyElementsIdGet Jumptable for different Crypto Driver Objects */
extern CONST(CryIf_KeyElementIdsPtrType, CRYIF_CONST) CryIf_KeyElementIdsGetJumpTable[CRYIF_KEY_COUNT];
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief KeyGenerate Jumptable for different Crypto Driver Objects */
extern CONST(CryIf_RandomSeedPtrType, CRYIF_CONST) CryIf_RandomSeedJumpTable[CRYIF_KEY_COUNT];
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief KeyGenerate Jumptable for different Crypto Driver Objects */
extern CONST(CryIf_KeyGeneratePtrType, CRYIF_CONST) CryIf_KeyGenerateJumpTable[CRYIF_KEY_COUNT];
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief KeyDerive Jumptable for different Crypto Driver Objects */
extern CONST(CryIf_KeyDerivePtrType, CRYIF_CONST) CryIf_KeyDeriveJumpTable[CRYIF_KEY_COUNT];
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief KeyExchangeCalcPubVal Jumptable for different Crypto Driver Objects */
extern CONST(CryIf_KeyExchangeCalcPubValPtrType, CRYIF_CONST) CryIf_KeyExchangeCalcPubValJumpTable[CRYIF_KEY_COUNT];
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief KeyExchangeCalcSecret Jumptable for different Crypto Driver Objects */
extern CONST(CryIf_KeyExchangeCalcSecretPtrType, CRYIF_CONST) CryIf_KeyExchangeCalcSecretJumpTable[CRYIF_KEY_COUNT];
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief CertificateParse Jumptable for different Crypto Driver Objects */
extern CONST(CryIf_CertificateParsePtrType, CRYIF_CONST) CryIf_CertificateParseJumpTable[CRYIF_KEY_COUNT];
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief CertificateVerify Jumptable for different Crypto Driver Objects */
extern CONST(CryIf_CertificateVerifyPtrType, CRYIF_CONST) CryIf_CertificateVerifyJumpTable[CRYIF_KEY_COUNT];
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief KeyCopy Jumptable for different Crypto Drivers */
extern CONST(CryIf_KeyCopyPtrType, CRYIF_CONST) CryIf_KeyCopyJumpTable[CRYIF_KEY_COUNT];
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* Deviation MISRAC2012-1 <STOP> */

#define CRYIF_STOP_SEC_CONST_UNSPECIFIED
#include <CryIf_MemMap.h>

/*==================[declaration of external variables]===========================================*/

/*==================[declaration of external functions]===========================================*/

#define CRYIF_START_SEC_CODE
#include <CryIf_MemMap.h>

/**
 * \brief Initializes the Crypto Interface module
 *
 * \ServiceID{::CRYIF_SID_INIT}
 * \Synchronicity{Synchronous}
 * \Reentrancy{Non Reentrant}
 */
extern FUNC(void, CRYIF_CODE) CryIf_Init
(
  void
);

/**
 * \brief Provides information about the version of the module
 *
 * \param  versioninfo     Pointer to a version info structure
 *
 * \ServiceID{::CRYIF_SID_GETVERSIONINFO}
 * \Synchronicity{Synchronous}
 * \Reentrancy{Reentrant}
 */
extern FUNC(void, CRYIF_CODE) CryIf_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CRYIF_APPL_DATA) versioninfo
);

/**
 * \brief   Processes a job received from the CSM
 *
 * \param[in]     channelId            Holds the identifier of the Crypto Channel
 * \param[in,out] job                  Holds a pointer to the job structure that shall be processed
 *
 * \return  Standard Return Value extended by the Crypto Stack
 * \retval  E_OK                       Request successful
 * \retval  E_NOT_OK                   Request failed
 * \retval  CRYPTO_E_SMALL_BUFFER      Provided buffer is too small to store the result
 * \retval  CRYPTO_E_QUEUE_FULL        Queue within the crypto driver is full
 *
 * \ServiceID{::CRYIF_SID_PROCESSJOB}
 * \Synchronicity{Sync or Async, depends on the configuration}
 * \Reentrancy{Reentrant}
 */
extern FUNC(Std_ReturnType, CRYIF_CODE) CryIf_ProcessJob
(
  uint32 channelId,
  P2VAR(Crypto_JobType , AUTOMATIC, CRYIF_APPL_DATA) job
);

/**
 * \brief   This interface dispatches the job cancellation function to the configured crypto
 *          driver object.
 *
 * \param[in]      channelId      Holds the identifier of the crypto channel.
 * \param[in,out]  job            Pointer to the configuration of the job. Contains structures
 *                                with user and primitive relevant information.
 *
 * \return  Standard Return Value extended by the Crypto Stack
 * \retval  E_OK                       Request successful
 * \retval  E_NOT_OK                   Request failed
 *
 * \ServiceID{::CRYIF_SID_CANCELJOB}
 * \Synchronicity{Synchronous}
 * \Reentrancy{Reentrant}
 */
extern FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CancelJob
(
  uint32 channelId,
  P2VAR(Crypto_JobType , AUTOMATIC, CRYIF_APPL_DATA) job
);

#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/**
 * \brief   This function shall dispatch the set key element function to the configured crypto
 *          driver object.
 *
 * \param[in]   cryIfKeyId     Holds the identifier of the key whose key element shall be set.
 * \param[in]   keyElementId   Holds the identifier of the key element which shall be set.
 * \param[in]   keyPtr         Holds the pointer to the key data which shall be set as key element.
 * \param[in]   keyLength      Contains the length of the key element in bytes.
 *
 * \return  Standard Return Value
 * \retval  E_OK                          Request successful
 * \retval  E_NOT_OK                      Request failed
 * \retval  CRYPTO_E_BUSY:                Request failed, Crypto Driver Object is busy
 * \retval  CRYPTO_E_KEY_WRITE_FAIL:      Request failed because write access was denied
 * \retval  CRYPTO_E_KEY_NOT_AVAILABLE:   Request failed because the key is not available.
 * \retval  CRYPTO_E_KEY_SIZE_MISMATCH:   Request failed, key element size does not match
 *                                        size of provided data.
 *
 * \ServiceID{::CRYIF_SID_KEYELEMENTSET}
 * \Synchronicity{Synchronous}
 * \Reentrancy{Non Reentrant}
 */
extern FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementSet
(
  uint32 cryIfKeyId,
  uint32 keyElementId,
  P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) keyPtr,
  uint32 keyLength
);
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/**
 * \brief   This function shall dispatch the set key valid function to the configured crypto driver
 *          object.
 *
 * \param[in]   cryIfKeyId             Identifier of the key that shall be set to valid
 *
 * \return  Standard Return Value
 * \retval  E_OK                       Request successful
 * \retval  E_NOT_OK                   Request failed
 * \retval  CRYPTO_E_BUSY              Request Failed, Crypro Driver Object is Busy
 *
 * \ServiceID{::CRYIF_SID_KEYSETVALID}
 * \Synchronicity{Synchronous}
 * \Reentrancy{Non Reentrant}
 */
extern FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeySetValid
(
  uint32 cryIfKeyId
);
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/**
 * \brief   This function shall dispatch the set key element function to the configured crypto
 *          driver object.
 *
 * \param[in]      cryIfKeyId       Holds the identifier of the key whose key element shall
 *                                  be returned.
 * \param[in]      keyElementId     Holds the identifier of the key element which shall be returned.
 * \param[out]     resultPtr        Holds the pointer of the buffer for the returned key element.
 * \param[in,out]  resultLengthPtr  Holds a pointer to a memory location in which the length
 *                                  information is stored. On calling this function this parameter
 *                                  shall contain the size of the buffer provided by resultPtr. If
 *                                  the key element is configured to allow partial access, this
 *                                  parameter contains the amount of data which should be read from
 *                                  the key element. The size may not be equal to the size of the
 *                                  provided buffer anymore. When the request has finished, the
 *                                  amount of data that has been stored shall be stored.
 *
 * \return  Standard Return Value
 * \retval  E_OK                          Request successful
 * \retval  E_NOT_OK                      Request failed
 * \retval  CRYPTO_E_BUSY                 Request failed, Crypto Driver Object is busy
 * \retval  CRYPTO_E_KEY_NOT_AVAILABLE    Request failed, the requested key element is not available
 * \retval  CRYPTO_E_KEY_READ_FAIL        Request failed because read access was denied
 * \retval  CRYPTO_E_SMALL_BUFFER         The provided buffer is too small to store the result
 *
 * \ServiceID{::CRYIF_SID_KEYELEMENTGET}
 * \Synchronicity{Synchronous}
 * \Reentrancy{Reentrant}
 */
extern FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementGet
(
  uint32 cryIfKeyId,
  uint32 keyElementId,
  P2VAR(uint8, AUTOMATIC, CRYIF_APPL_DATA) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYIF_APPL_DATA) resultLengthPtr
);
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/**
 * \brief   This function shall copy a key elements from one key to a target key.
 *
 * \param[in]  cryIfKeyId          Holds the identifier of the key whose key element shall be the
 *                                 source element.
 * \param[in]  keyElementId        Holds the identifier of the key element which shall be the source
 *                                 for the copy operation.
 * \param[in]  targetCryIfKeyId    Holds the identifier of the key whose key element shall be the
 *                                 destination element.
 * \param[in]  targetKeyElementId  Holds the identifier of the key element which shall be the
 *                                 destination for the copy operation.
 *
 * \return  Standard Return Value
 * \retval  E_OK                          Request successful
 * \retval  E_NOT_OK                      Request failed
 * \retval  CRYPTO_E_BUSY                 Request failed, Crypto Driver Object is busy
 * \retval  CRYPTO_E_KEY_EXTRACT_DENIED   Request failed, not allowed to extract key element
 * \retval  CRYPTO_E_KEY_READ_FAIL        Request failed, not allowed to extract key element
 * \retval  CRYPTO_E_KEY_WRITE_FAIL       Request failed, not allowed to write key element.
 * \retval  CRYPTO_E_KEY_SIZE_MISMATCH    Request failed, key element sizes are not compatible.
 *
 * \ServiceID{::CRYIF_SID_KEYELEMENTCOPY}
 * \Synchronicity{Synchronous}
 * \Reentrancy{Reentrant, but not for the same cryIfKeyId}
 */
extern FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementCopy
(
  uint32 cryIfKeyId,
  uint32 keyElementId,
  uint32 targetCryIfKeyId,
  uint32 targetKeyElementId
);
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/**
 * \brief   This function shall copy all key elements from the source key to a target key.
 *
 * \param[in]  cryIfKeyId          Holds the identifier of the key whose key element shall be the
 *                                 source element.
 * \param[in]  targetCryIfKeyId    Holds the identifier of the key whose key element shall be the
 *                                 destination element.
 *
 * \return  Standard Return Value
 * \retval  E_OK                          Request successful
 * \retval  E_NOT_OK                      Request failed
 * \retval  CRYPTO_E_BUSY                 Request failed, Crypto Driver Object is busy
 * \retval  CRYPTO_E_KEY_EXTRACT_DENIED   Request failed, not allowed to extract key element
 * \retval  CRYPTO_E_KEY_READ_FAIL        Request failed, not allowed to extract key element
 * \retval  CRYPTO_E_KEY_WRITE_FAIL       Request failed, not allowed to write key element.
 * \retval  CRYPTO_E_KEY_SIZE_MISMATCH    Request failed, key element sizes are not compatible.
 *
 * \ServiceID{::CRYIF_SID_KEYCOPY}
 * \Synchronicity{Synchronous}
 * \Reentrancy{Reentrant, but not for the same cryIfKeyId}
 */
extern FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyCopy
(
  uint32 cryIfKeyId,
  uint32 targetCryIfKeyId
);
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/**
 * \brief   This function shall dispatch the key generate function to the configured crypto driver
 *          object.
 *
 * \param[in]  cryIfKeyId        Holds the identifier of the key which is to be updated with the
 *                               generated value.
 *
 * \return  Standard Return Value
 * \retval  E_OK                          Request successful
 * \retval  E_NOT_OK                      Request failed
 * \retval  CRYPTO_E_BUSY                 Request failed, Crypto Driver Object is busy
 *
 * \ServiceID{::CRYIF_SID_KEYGENERATE}
 * \Synchronicity{Sync or Async, depends on the configuration}
 * \Reentrancy{Reentrant}
 */
extern FUNC(Std_ReturnType,CRYIF_CODE) CryIf_KeyGenerate
(
  uint32 cryIfKeyId
);
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/**
 * \brief   This function shall dispatch the key derive function to the configured crypto driver
 *          object.
 *
 * \param[in]  cryIfKeyId        Holds the identifier of the key which is used for key derivation.
 * \param[in]  targetCryIfKeyId  Holds the identifier of the key which is used to store the derived
 *                               key.
 *
 * \return  Standard Return Value
 * \retval  E_OK                          Request successful
 * \retval  E_NOT_OK                      Request failed
 *
 * \ServiceID{::CRYIF_SID_KEYDERIVE}
 * \Synchronicity{Synchronous}
 * \Reentrancy{Reentrant}
 */
extern FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyDerive
(
  uint32 cryIfKeyId,
  uint32 targetCryIfKeyId
);
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/**
 * \brief   This function shall dispatch the key exchange public value calculation function to the
 *          configured crypto driver object.
 *
 * \param[in]      cryIfKeyId            Holds the identifier of the key which shall be used for the
 *                                       key exchange protocol.
 * \param[in,out]  publicValueLengthPtr  Holds a pointer to the memory location in which the public
 *                                       value length information is stored. On calling this
 *                                       function, this parameter shall contain the size of the
 *                                       buffer provided by publicValuePtr. When the request has
 *                                       finished, the actual length of the returned value shall be
 *                                       stored.
 * \param[out]     publicValuePtr        Contains the pointer to the data where the public value
 *                                       shall be stored.
 *
 * \return  Standard Return Value
 * \retval  E_OK                          Request successful
 * \retval  E_NOT_OK                      Request failed
 * \retval  E_BUSY                        Request Failed, Crypto Driver Object is Busy
 * \retval  CRYPTO_E_SMALL_BUFFER         The provided buffer is too small to store the result
 *
 * \ServiceID{::CRYIF_SID_KEYEXCHANGECALCPUBVAL}
 * \Synchronicity{Synchronous}
 * \Reentrancy{Reentrant}
 */
extern FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyExchangeCalcPubVal
(
  uint32 cryIfKeyId,
  P2VAR(uint8, AUTOMATIC, CRYIF_APPL_DATA) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYIF_APPL_DATA) publicValueLengthPtr
);
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/**
 * \brief   This function shall dispatch the key exchange common shared secret calculation function
 *          to the configured crypto driver object.
 *
 * \param[in]  cryIfKeyId                Holds the identifier of the key which shall be used for the
 *                                       key exchange protocol.
 * \param[in]  partnerPublicValuePtr     Holds the pointer to the memory location which contains the
 *                                       partner's public value.
 * \param[in]  partnerPublicValueLength  Contains the length of the partner's public value in bytes.
 *
 * \return  Standard Return Value
 * \retval  E_OK                          Request successful
 * \retval  E_NOT_OK                      Request failed
 * \retval  E_BUSY                        Request Failed, Crypto Driver Object is Busy
 * \retval  CRYPTO_E_SMALL_BUFFER         The provided buffer is too small to store the result
 *
 * \ServiceID{::CRYIF_SID_KEYEXCHANGECALCSECRET}
 * \Synchronicity{Synchronous}
 * \Reentrancy{Reentrant}
 */
extern FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyExchangeCalcSecret
(
  uint32 cryIfKeyId,
  P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) partnerPublicValuePtr,
  uint32 partnerPublicValueLength
);
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/**
 * \brief   This function shall dispatch the certificate parse function to the configured crypto
 *          driver object.
 *
 * \param[in]  cryIfKeyId              Holds the identifier of the key which shall be parsed.
 *
 * \return  Standard Return Value
 * \retval  E_OK                          Request successful
 * \retval  E_NOT_OK                      Request failed
 * \retval  E_BUSY                        Request Failed, Crypto Driver Object is Busy
 *
 * \ServiceID{::CRYIF_SID_CERTIFICATEPARSE}
 * \Synchronicity{Synchronous}
 * \Reentrancy{Reentrant}
 */
extern FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CertificateParse
(
  uint32 cryIfKeyId
);
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/**
 * \brief   Verifies the certificate stored in the key referenced by verifyCryIfKeyId with the
 *          certificate stored in the key referenced by cryIfKeyId.
 *
 * \param[in]   cryIfKeyId              Holds the identifier of the key which shall be parsed.
 * \param[in]   verifyCryIfKeyId        Holds the identifier of the key containing the certificate
 *                                      to be verified.
 * \param[out]  verifyPtr               Holds a pointer to the memory location which will contain
 *                                      the result of the certificate verification.
 *
 * \return  Standard Return Value
 * \retval  E_OK                          Request successful
 * \retval  E_NOT_OK                      Request failed
 * \retval  E_BUSY                        Request Failed, Crypto Driver Object is Busy
 *
 * \ServiceID{::CRYIF_SID_CERTIFICATEVERIFY}
 * \Synchronicity{Synchronous}
 * \Reentrancy{Reentrant, but not for the same cryIfKeyId}
 */
extern FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CertificateVerify
(
  uint32 cryIfKeyId,
  uint32 verifyCryIfKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYIF_APPL_DATA) verifyPtr
);
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
/**
 * \brief   This function shall dispatch the random seed function to the configured crypto driver
 *          object.
 *
 * \param[in]  cryIfKeyId  Holds the identifier of the key for which a new seed shall be generated.
 * \param[in]  seedPtr     Holds a pointer to the memory location which contains the data to feed
 *                         the seed.
 * \param[in]  seedLength  Contains the length of the seed in bytes.
 *
 * \return  Standard Return Value
 * \retval  E_OK                          Request successful
 * \retval  E_NOT_OK                      Request failed
 *
 * \ServiceID{::CRYIF_SID_RANDOMSEED}
 * \Synchronicity{Sync or Async, depends on the configuration}
 * \Reentrancy{Reentrant}
 */
extern FUNC(Std_ReturnType, CRYIF_CODE) CryIf_RandomSeed
(
  uint32 cryIfKeyId,
  P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) seedPtr,
  uint32 seedLength
);
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#define CRYIF_STOP_SEC_CODE
#include <CryIf_MemMap.h>

/*================================================================================================*/

#endif /* #ifndef CRYIF_H */

/*==================[end of file]=================================================================*/

