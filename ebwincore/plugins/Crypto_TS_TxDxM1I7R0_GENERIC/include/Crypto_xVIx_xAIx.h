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

#ifndef CRYPTO_XVIX_XAIX_H
#define CRYPTO_XVIX_XAIX_H

/* !LINKSTO SWS_Crypto_00007, 1 */

/*==[Misra deviations]============================================================================*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 05.6 (required)
 * type Crypto_xVIx_xAIx_QueueElementType conflicts with the typedef name Crypto_xVIx_xAIx_QueueElementType.
 *
 * Reason:
 * A type can not be self referenced if the type is not already defined.
 */

/*==[Includes]====================================================================================*/

/* !LINKSTO SWS_Crypto_00042, 1, SWS_Crypto_00009, 1, SWS_Crypto_00043, 1, SWS_Crypto_00010, 1 */
#include <Csm_Types.h>
#include <Crypto_xVIx_xAIx_Cfg.h>
#include <Crypto_xVIx_xAIx_KeyManagement.h>

/*==[Macros]======================================================================================*/

/*--[Development error codes]---------------------------------------------------------------------*/

/* !LINKSTO SWS_Crypto_00040, 1 */
#if (defined CRYPTO_XVIX_XAIX_E_UNINIT)
#error CRYPTO_XVIX_XAIX_E_UNINIT is already defined
#endif
/** \brief Crypto driver is not initialized. */
#define CRYPTO_XVIX_XAIX_E_UNINIT 0x00U

#if (defined CRYPTO_XVIX_XAIX_E_INIT_FAILED)
#error CRYPTO_XVIX_XAIX_E_INIT_FAILED is already defined
#endif
/** \brief Initialization of crypto driver failed. */
#define CRYPTO_XVIX_XAIX_E_INIT_FAILED 0x01U

#if (defined CRYPTO_XVIX_XAIX_E_PARAM_POINTER)
#error CRYPTO_XVIX_XAIX_E_PARAM_POINTER is already defined
#endif
/** \brief Pointer parameter has an invalid value. */
#define CRYPTO_XVIX_XAIX_E_PARAM_POINTER 0x02U

#if (defined CRYPTO_XVIX_XAIX_E_PARAM_HANDLE)
#error CRYPTO_XVIX_XAIX_E_PARAM_HANDLE is already defined
#endif
/** \brief Handle parameter has an invalid value. */
#define CRYPTO_XVIX_XAIX_E_PARAM_HANDLE 0x04U

#if (defined CRYPTO_XVIX_XAIX_E_PARAM_VALUE)
#error CRYPTO_XVIX_XAIX_E_PARAM_VALUE is already defined
#endif
/** \brief Value parameter has an invalid value. */
#define CRYPTO_XVIX_XAIX_E_PARAM_VALUE 0x05U

/*--[Service IDs]---------------------------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_SID_INIT)
#error CRYPTO_XVIX_XAIX_SID_INIT is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_Init. */
#define CRYPTO_XVIX_XAIX_SID_INIT 0x00U

#if (defined CRYPTO_XVIX_XAIX_SID_GETVERSIONINFO)
#error CRYPTO_XVIX_XAIX_SID_GETVERSIONINFO is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_GetVersionInfo. */
#define CRYPTO_XVIX_XAIX_SID_GETVERSIONINFO 0x01U

#if (defined CRYPTO_XVIX_XAIX_SID_PROCESSJOB)
#error CRYPTO_XVIX_XAIX_SID_PROCESSJOB is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_ProcessJob. */
#define CRYPTO_XVIX_XAIX_SID_PROCESSJOB 0x03U

#if (defined CRYPTO_XVIX_XAIX_SID_CANCELJOB)
#error CRYPTO_XVIX_XAIX_SID_CANCELJOB is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_CancelJob. */
#define CRYPTO_XVIX_XAIX_SID_CANCELJOB 0x0EU

#if (defined CRYPTO_XVIX_XAIX_SID_KEYELEMENTSET)
#error CRYPTO_XVIX_XAIX_SID_KEYELEMENTSET is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_KeyElementSet. */
#define CRYPTO_XVIX_XAIX_SID_KEYELEMENTSET 0x04U

#if (defined CRYPTO_XVIX_XAIX_SID_KEYVALIDSET)
#error CRYPTO_XVIX_XAIX_SID_KEYVALIDSET is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_KeyValidSet. */
#define CRYPTO_XVIX_XAIX_SID_KEYVALIDSET 0x05U

#if (defined CRYPTO_XVIX_XAIX_SID_KEYELEMENTGET)
#error CRYPTO_XVIX_XAIX_SID_KEYELEMENTGET is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_KeyElementGet. */
#define CRYPTO_XVIX_XAIX_SID_KEYELEMENTGET 0x06U

#if (defined CRYPTO_XVIX_XAIX_SID_KEYELEMENTCOPY)
#error CRYPTO_XVIX_XAIX_SID_KEYELEMENTCOPY is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_KeyElementCopy. */
#define CRYPTO_XVIX_XAIX_SID_KEYELEMENTCOPY 0x0FU

#if (defined CRYPTO_XVIX_XAIX_SID_KEYCOPY)
#error CRYPTO_XVIX_XAIX_SID_KEYCOPY is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_KeyCopy. */
#define CRYPTO_XVIX_XAIX_SID_KEYCOPY 0x10U

#if (defined CRYPTO_XVIX_XAIX_SID_KEYELEMENTIDSGET)
#error CRYPTO_XVIX_XAIX_SID_KEYELEMENTIDSGET is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_KeyElementIdsGet. */
#define CRYPTO_XVIX_XAIX_SID_KEYELEMENTIDSGET 0x11U

#if (defined CRYPTO_XVIX_XAIX_SID_RANDOMSEED)
#error CRYPTO_XVIX_XAIX_SID_RANDOMSEED is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_RandomSeed. */
#define CRYPTO_XVIX_XAIX_SID_RANDOMSEED 0x0DU

#if (defined CRYPTO_XVIX_XAIX_SID_KEYGENERATE)
#error CRYPTO_XVIX_XAIX_SID_KEYGENERATE is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_KeyGenerate. */
#define CRYPTO_XVIX_XAIX_SID_KEYGENERATE 0x07U

#if (defined CRYPTO_XVIX_XAIX_SID_KEYDERIVE)
#error CRYPTO_XVIX_XAIX_SID_KEYDERIVE is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_KeyDerive. */
#define CRYPTO_XVIX_XAIX_SID_KEYDERIVE 0x08U

#if (defined CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCPUBVAL)
#error CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCPUBVAL is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_KeyExchangeCalcPubVal. */
#define CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCPUBVAL 0x09U

#if (defined CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCSECRET)
#error CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCSECRET is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_KeyExchangeCalcSecret. */
#define CRYPTO_XVIX_XAIX_SID_KEYEXCHANGECALCSECRET 0x0AU

#if (defined CRYPTO_XVIX_XAIX_SID_CERTIFICATEPARSE)
#error CRYPTO_XVIX_XAIX_SID_CERTIFICATEPARSE is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_CertificateParse. */
#define CRYPTO_XVIX_XAIX_SID_CERTIFICATEPARSE 0x0BU

#if (defined CRYPTO_XVIX_XAIX_SID_CERTIFICATEVERIFY)
#error CRYPTO_XVIX_XAIX_SID_CERTIFICATEVERIFY is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_CertificateVerifiy. */
#define CRYPTO_XVIX_XAIX_SID_CERTIFICATEVERIFY 0x12U

#if (defined CRYPTO_XVIX_XAIX_SID_MAINFUNCTION)
#error CRYPTO_XVIX_XAIX_SID_MAINFUNCTION is already defined
#endif
/** \brief AUTOSAR API service ID for Crypto_xVIx_xAIx_MainFunction. */
#define CRYPTO_XVIX_XAIX_SID_MAINFUNCTION 0x0CU

/*--[Crypto_xVIx_xAIx_DriverObjectStateType]------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE)
#error CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE is already defined
#endif
/** \brief Crypto driver object is idle. */
#define CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE 0x00U

#if (defined CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_BUSY)
#error CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_BUSY is already defined
#endif
/** \brief Crypto driver object is busy. */
#define CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_BUSY 0x01U

/*--[Crypto_xVIx_xAIx_KeyStateType]---------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_KEY_STATE_INVALID)
#error CRYPTO_XVIX_XAIX_KEY_STATE_INVALID is already defined
#endif
/** \brief Key is invalid. */
#define CRYPTO_XVIX_XAIX_KEY_STATE_INVALID 0x00U

#if (defined CRYPTO_XVIX_XAIX_KEY_STATE_VALID)
#error CRYPTO_XVIX_XAIX_KEY_STATE_VALID is already defined
#endif
/** \brief Key is valid. */
#define CRYPTO_XVIX_XAIX_KEY_STATE_VALID 0x01U

/*--[Crypto_xVIx_xAIx_ReadAccessType]-------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_RA_DENIED)
#error CRYPTO_XVIX_XAIX_RA_DENIED is already defined
#endif
/** \brief The key element can not be read from outside the crypto driver. */
#define CRYPTO_XVIX_XAIX_RA_DENIED 0x01U

#if (defined CRYPTO_XVIX_XAIX_RA_INTERNAL_COPY)
#error CRYPTO_XVIX_XAIX_RA_INTERNAL_COPY is already defined
#endif
/** \brief Key element can be copied within the same crypto driver. */
#define CRYPTO_XVIX_XAIX_RA_INTERNAL_COPY 0x02U

#if (defined CRYPTO_XVIX_XAIX_RA_ALLOWED)
#error CRYPTO_XVIX_XAIX_RA_ALLOWED is already defined
#endif
/** \brief Key element can be read in plaintext. */
#define CRYPTO_XVIX_XAIX_RA_ALLOWED 0x03U

#if (defined CRYPTO_XVIX_XAIX_RA_ENCRYPTED)
#error CRYPTO_XVIX_XAIX_RA_ENCRYPTED is already defined
#endif
/** \brief Key element can be read encrypted. */
#define CRYPTO_XVIX_XAIX_RA_ENCRYPTED 0x04U

/*--[Crypto_xVIx_xAIx_WriteAccessType]------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_WA_DENIED)
#error CRYPTO_XVIX_XAIX_WA_DENIED is already defined
#endif
/** \brief The key element can not be written from outside the crypto driver. */
#define CRYPTO_XVIX_XAIX_WA_DENIED 0x01U

#if (defined CRYPTO_XVIX_XAIX_WA_INTERNAL_COPY)
#error CRYPTO_XVIX_XAIX_WA_INTERNAL_COPY is already defined
#endif
/** \brief Key element within the same crypto driver can be copied to the key element. */
#define CRYPTO_XVIX_XAIX_WA_INTERNAL_COPY 0x02U

#if (defined CRYPTO_XVIX_XAIX_WA_ALLOWED)
#error CRYPTO_XVIX_XAIX_WA_ALLOWED is already defined
#endif
/** \brief Key element can be written in plaintext. */
#define CRYPTO_XVIX_XAIX_WA_ALLOWED 0x03U

#if (defined CRYPTO_XVIX_XAIX_WA_ENCRYPTED)
#error CRYPTO_XVIX_XAIX_WA_ENCRYPTED is already defined
#endif
/** \brief Key element can be written encrypted. */
#define CRYPTO_XVIX_XAIX_WA_ENCRYPTED 0x04U

/*--[Runtime Error Types]------------------------------------------------------------*/

/* !LINKSTO SWS_Crypto_00194, 1 */
#if (defined CRYPTO_XVIX_XAIX_E_RE_SMALL_BUFFER)
#error CRYPTO_XVIX_XAIX_E_RE_SMALL_BUFFER is already defined
#endif
#define CRYPTO_XVIX_XAIX_E_RE_SMALL_BUFFER       0x00U

#if (defined CRYPTO_XVIX_XAIX_E_RE_KEY_NOT_AVAILABLE)
#error CRYPTO_XVIX_XAIX_E_RE_KEY_NOT_AVAILABLE is already defined
#endif
#define CRYPTO_XVIX_XAIX_E_RE_KEY_NOT_AVAILABLE  0x01U

#if (defined CRYPTO_XVIX_XAIX_E_RE_KEY_READ_FAIL)
#error CRYPTO_XVIX_XAIX_E_RE_KEY_READ_FAIL is already defined
#endif
#define CRYPTO_XVIX_XAIX_E_RE_KEY_READ_FAIL      0x02U

#if (defined CRYPTO_XVIX_XAIX_E_RE_ENTROPY_EXHAUSTED)
#error CRYPTO_XVIX_XAIX_E_RE_ENTROPY_EXHAUSTED is already defined
#endif
#define CRYPTO_XVIX_XAIX_E_RE_ENTROPY_EXHAUSTED  0x03U


/*==[Types]=======================================================================================*/

/** \brief Pointer to a process function defined in the process layer. */
typedef Std_ReturnType (* Crypto_xVIx_xAIx_ProcessFuncPtr_t)(uint32 objectId, Crypto_JobType* job);

/** \brief Pointer to a cancel function defined in the process layer. */
typedef Std_ReturnType (* Crypto_xVIx_xAIx_CancelFuncPtr_t)(uint32 objectId, Crypto_JobType* job);

/** \brief Different states of the crypto driver object. */
typedef uint8 Crypto_xVIx_xAIx_DriverObjectStateType;

/* !LINKSTO SWS_Crypto_00038, 1 */
/** \brief Different states of a crypto key. */
typedef boolean Crypto_xVIx_xAIx_KeyStateType;

/** \brief Different read access types for a key element. */
typedef uint8 Crypto_xVIx_xAIx_ReadAccessType;

/** \brief Different write access types for a key element. */
typedef uint8 Crypto_xVIx_xAIx_WriteAccessType;


/* CHECK: RULE 505 OFF (Type definitions should end in '_t' or 'Type') */
/* This produces a CheckC warning. The type definition is according to the Autosar specification and can not be changed. */
/** \brief Structure of a crypto key element */
typedef struct
{
  /* Id of key element. */
  uint32 Id;
  /* Indicates whether the key element can be read or written with size smaller than the size of
      the element. */
  boolean AllowPartialAccess;
  /* The initial value of the key element. */
  CONSTP2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_CONST) InitValue;
  /* Indicates whether the key element is persisted in non-volatile memory. */
  boolean Persist;
  /* Defines the read access rights. */
  CONST(Crypto_xVIx_xAIx_ReadAccessType, AUTOMATIC) ReadAccess;
  /* Maximum size of the key element in byte.s */
  uint32 Size;
  /* Defines the write access rights. */
  CONST(Crypto_xVIx_xAIx_WriteAccessType, AUTOMATIC) WriteAccess;
} Crypto_xVIx_xAIx_KeyElement;
/* CHECK: RULE 505 ON */

/** \brief Pointer to a crypto key element. */
typedef P2VAR(Crypto_xVIx_xAIx_KeyElement, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)
  Crypto_xVIx_xAIx_KeyElementPtr;

/* CHECK: RULE 505 OFF (Type definitions should end in '_t' or 'Type') */
/* This produces a CheckC warning. The type definition is according to the Autosar specification and can not be changed. */
/** \brief Structure of a crypto key. */
typedef struct
{
  /* Iterations used in key derivation process. */
  CONST(uint32, AUTOMATIC) KeyDeriveIterations;
  /* Number of key elements referenced by the key. */
  CONST(uint32, AUTOMATIC) KeyElements;
  /* Reference to the key type structure. */
  CONSTP2CONST(Crypto_xVIx_xAIx_KeyElementPtr, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyType;
  /* Current state of the key. */
  Crypto_xVIx_xAIx_KeyStateType KeyState;
} Crypto_xVIx_xAIx_Key;
/* CHECK: RULE 505 ON */

#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
/** \brief Structure of a queue element */
/* Deviation MISRAC2012-1 */
typedef struct Crypto_xVIx_xAIx_QueueElementType
{
  /* Job that is referenced by the queue element. */
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) Job;
  /* Reference to the next element in the queue. */
  P2VAR(struct Crypto_xVIx_xAIx_QueueElementType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Next;
  /* Function that shall be used to further process the job. */
  Crypto_xVIx_xAIx_ProcessFuncPtr_t ProcessFunction;
} Crypto_xVIx_xAIx_QueueElementType;
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */

#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
/** \brief Pointer to a crypto key element */
typedef P2VAR(Crypto_xVIx_xAIx_QueueElementType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) Crypto_xVIx_xAIx_QueueElementPtr;
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */

#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
/** \brief Meta data structure of a queue */
typedef struct
{
  /* Current size of the queue. */
  uint32 CurrentSize;
  /* Maximum number of elements that can be stored in the queue. */
  CONST(uint32, AUTOMATIC) MaxSize;
  /* Pointer to the first element of the queue. (highest priority) */
  P2VAR(Crypto_xVIx_xAIx_QueueElementType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Head;
  /* Pointer to the data array that stores the queue elements. */
  CONSTP2VAR(Crypto_xVIx_xAIx_QueueElementType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) Data;
} Crypto_xVIx_xAIx_QueueType;
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */

/* CHECK: RULE 505 OFF (Type definitions should end in '_t' or 'Type') */
/* This produces a CheckC warning. The type definition is according to the Autosar specification and can not be changed. */
/** \brief Structure of a crypto driver object. */
typedef struct
{

  /* Flag that defines if the periodic main function shall be skipped */
  boolean SkipPeriodicMainFunction;
  /* Current state of the crypto driver object */
  Crypto_xVIx_xAIx_DriverObjectStateType DriverObjectState;
  /* Job that is currently processed by the crypto driver object */
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) CurrentJob;
#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
  /* Reference to the queue of the crypto driver object */
  CONSTP2VAR(Crypto_xVIx_xAIx_QueueType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA) Queue;
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */
} Crypto_xVIx_xAIx_DriverObject;
/* CHECK: RULE 505 ON */





/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_BOOLEAN
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Module initialization status. */
extern VAR(boolean, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_Initialized;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_BOOLEAN
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Array of driver objects. */
extern VAR(Crypto_xVIx_xAIx_DriverObject, CRYPTO_xVIx_xAIx_VAR)
  Crypto_xVIx_xAIx_DriverObjects[CRYPTO_XVIX_XAIX_DRIVER_OBJECT_COUNT];

/* !LINKSTO EB_Crypto_01018,1 */
#if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON)
/** \brief Array of keys. */
extern VAR(Crypto_xVIx_xAIx_Key, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_Keys[CRYPTO_XVIX_XAIX_KEY_COUNT];
#endif /* #if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON) */

/** \brief Array of key elements */
extern VAR(Crypto_xVIx_xAIx_KeyElement, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_KeyElements[CRYPTO_XVIX_XAIX_KEY_ELEMENT_COUNT];

#if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON)
/** \brief Array of queues */
extern VAR(Crypto_xVIx_xAIx_QueueType, AUTOMATIC) Crypto_xVIx_xAIx_Queues[CRYPTO_XVIX_XAIX_DRIVER_OBJECT_COUNT];
#endif /* #if (CRYPTO_XVIX_XAIX_QUEUE == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Declaration of functions with external linkage]==============================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Init function of the Crypto Driver.
 *
 * This function initializes the AUTOSAR layer and forwards the initialization request to the
 * process layer.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
#ifndef RTE_SCHM_SWCBSW_CRYPTO_XVIX_XAIX_INIT
#define RTE_SCHM_SWCBSW_CRYPTO_XVIX_XAIX_INIT
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_Init
(
  void
);
#endif /* #ifndef RTE_SCHM_SWCBSW_CRYPTO_XVIX_XAIX_INIT */

/** \brief Retrieve version info for the crypto driver module.
 *
 * This function copies the version information to the passed VersionInfo struct.
 *
 * \param[out] versioninfo Pointer to the VersionInfo structure that shall be filled with data.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) versioninfo
);

/** \brief Process function of the crypto driver.
 *
 * This function accepts the process requests. It checks the parameters for correctness and
 * dispatches according to service, algorithm family and algorithm mode of the requested job. If
 * the requested driver object supports the requested primitive, the function passes the request to
 * the process layer, if the crypto driver object is not busy. If the driver object is busy, the
 * function indicates an error (synchronous processing) or attempts to put the job into the queue.
 * (asynchronous processing)
 *
 * \param[in] objectId Identifier of the crypto driver object that shall process the job.
 * \param[in] job      Reference to the job that shall be processed.
 *
 * \returns Result of the job processing.
 * \retval E_OK                        Job has been put into the queue or successfully processed.
 * \retval E_NOT_OK                    Job could not be processed.
 * \retval CRYPTO_E_BUSY               Request failed, the crypto driver object is busy.
 * \retval CRYPTO_E_KEY_NOT_VALID      Request failed, the key to be used is not valid.
 * \retval CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, a key element has the wrong size.
 * \retval CRYPTO_E_QUEUE_FULL         Request failed, the queue of the crypto driver object
 *                                     is full.
 * \retval CRYPTO_E_ENTROPY_EXHAUSTION Request failed, the entropy is exhausted.
 * \retval CRYPTO_E_SMALL_BUFFER       Request failed, the provided buffer is too small to store
 *                                     the result.
 * \retval CRYPTO_E_JOB_CANCELED       Request failed, the synchronous job was canceled.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ProcessJob
(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) job
);

/** \brief Process function of the crypto driver.
 *
 * This function accepts the process requests. It checks the parameters for correctness and
 * dispatches according to service, algorithm family and algorithm mode of the requested job. If
 * the requested driver object supports the requested primitive, the function passes the request to
 * the process layer, if the crypto driver object is not busy. If the driver object is busy, the
 * function indicates an error (synchronous processing) or attempts to put the job into the queue.
 * (asynchronous processing)
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_ProcessJob_Dispatch
(
  uint32 objectId,
  Std_ReturnType RetVal,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) job
);




/** \brief  Cancel function of the crypto driver.
 *
 * This function is used to cancel a requested job. If the job is currently in the queue of the
 * passed driver object, this function will attempt to remove the job from the queue. If the job is
 * currently processed by the crypto engine, the function will pass the cancellation request down
 * to the process layer.
 *
 * \param[in] objectId Identifier of the crypto driver object that processes the job.
 * \param[in] job      Reference to the job that shall be cancelled.
 *
 * \returns Result of the job cancellation attempt.
 * \retval E_OK              Job has been removed from the queue.
 * \retval E_NOT_OK          Job could not be cancelled.
 * \retval CRYPTO_E_CANCELED Active job has been successfully canceled.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CancelJob
(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) job
);

/** \brief Cyclic main function of the crypto driver.
 *
 * This function checks all available queues for jobs to be processed.
 * If a driver object is idle and has jobs in its queue, they are passed to the process layer.
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 *
 */
#if (CRYPTO_XVIX_XAIX_CRYPTO_MAIN_FUNCTION_EXISTS == STD_ON)
#ifndef RTE_SCHM_SWCBSW_CRYPTO_XVIX_XAIX_MAINFUNCTION
#define RTE_SCHM_SWCBSW_CRYPTO_XVIX_XAIX_MAINFUNCTION
extern FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_MainFunction
(
  void
);
#endif /* #ifndef RTE_SCHM_SWCBSW_CRYPTO_XVIX_XAIX_MAINFUNCTION */
#endif /* #if (CRYPTO_XVIX_XAIX_CRYPTO_MAIN_FUNCTION_EXISTS == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #ifndef CRYPTO_XVIX_XAIX_H */

/*==[End of file]=================================================================================*/
