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

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.9 (required)
 * An object should be defined at block scope if its identifier only appears in a single function.

 * Reason:
 * The AUTOSAR memory mapping requires that the function are mapped to SEC_CODE memory section.
 * Objects at block scope require a different mapping, e.g. to a SEC_VAR section, which leads to
 * nested memory sections, which is not supported by some compilers. Also  Declaring them inside the
 *  block scope will increase stack usage of respective functions.
 *
 */

/*==[include directives]==========================================================================*/

#include <Std_Types.h>
#include <TSAutosar.h>
#include <Crypto_xVIx_xAIx.h>
#include <Crypto_xVIx_xAIx_AL_Cipher.h>
#include <Csm_Types.h>
#include <CryIf_Cbk.h>
#include <TSMem.h>
#include <Crypto_xVIx_xAIx_AL_Common.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>
#include <Crypto_xVIx_xAIx_KeyManagement.h>
#include <Crypto_xVIx_xAIx_AL_KeyManagement.h>
#include <SchM_Crypto_xVIx_xAIx.h>

#if ( (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) || \
      (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)    \
    )

/*==[Macro definitions]===========================================================================*/

#if (defined CRYPTO_XVIX_XAIX_CIPHER_DEFAULT_CURRENT_MODE)
#error CRYPTO_XVIX_XAIX_CIPHER_DEFAULT_CURRENT_MODE already defined
#endif
/** \brief The default mode that the primitive is in.*/
#define CRYPTO_XVIX_XAIX_CIPHER_DEFAULT_CURRENT_MODE 0x00U

#if (defined CRYPTO_XVIX_XAIX_AES_STATE_UNINITIALIZED)
#error CRYPTO_XVIX_XAIX_AES_STATE_UNINITIALIZED already defined
#endif
/** \brief Uninitialized State for the AES ECB */
#define CRYPTO_XVIX_XAIX_AES_STATE_UNINITIALIZED        0x00U

#if (defined CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED)
#error CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED already defined
#endif
/** \brief Initilized state for the AES ECB. */
#define CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED          0x02U

#if (defined CRYPTO_XVIX_XAIX_AES_STATE_CALCULATING)
#error CRYPTO_XVIX_XAIX_AES_STATE_CALCULATING already defined
#endif
/** \brief Calculating State for the AES ECB. During processing this state is active. */
#define CRYPTO_XVIX_XAIX_AES_STATE_CALCULATING          0x04U

#if (defined CRYPTO_XVIX_XAIX_AES_STATE_FINISH)
#error CRYPTO_XVIX_XAIX_AES_STATE_FINISH already defined
#endif
/** \brief Finish State for the AES ECB. This state comes after the Finish function is called. */
#define CRYPTO_XVIX_XAIX_AES_STATE_FINISH               0x05U

#if (defined CRYPTO_XVIX_XAIX_AES_STATE_ERROR)
#error CRYPTO_XVIX_XAIX_AES_STATE_ERROR already defined
#endif
/** \brief Error state, indicating an error during the processing of the primitve. */
#define CRYPTO_XVIX_XAIX_AES_STATE_ERROR                0x06U
/*---Crypto_xVIx_xAIx_CbcPkcs7EncryptStateType--------------------------------------------------------------*/
#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_UNINIT)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_UNINIT already defined
#endif
/** \brief State indicating that the primitive is uninitialized. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_UNINIT             0x00U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_ST)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_ST already defined
#endif
/** \brief State of the state machine after start. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_ST          0x01U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_ST)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_ST already defined
#endif
/** \brief State of the state machine after start. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_ST         0x02U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_INITIALIZED)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_INITIALIZED already defined
#endif
/** \brief State for the AES CBC Encrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_INITIALIZED        0x03U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_FROM_INP)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_FROM_INP already defined
#endif
/** \brief State for the AES CBC Encrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_FROM_INP        0x04U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_UPD)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_UPD already defined
#endif
/** \brief State for the AES CBC Encrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_UPD         0x05U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_UPD)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_UPD already defined
#endif
/** \brief State for the AES CBC Encrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_UPD        0x06U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_TO_OUTP)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_TO_OUTP already defined
#endif
/** \brief State for the AES CBC Encrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_TO_OUTP         0x07U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_PAD_LAST_BL)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_PAD_LAST_BL already defined
#endif
/** \brief State for the AES CBC Encrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_PAD_LAST_BL        0x08U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_F_REQ_SB_UPD)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_F_REQ_SB_UPD already defined
#endif
/** \brief State for the AES CBC Encrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_F_REQ_SB_UPD       0x09U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_F_WAIT_SB_UPD)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_F_WAIT_SB_UPD already defined
#endif
/** \brief State for the AES CBC Encrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_F_WAIT_SB_UPD      0x0AU

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_PAD)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_PAD already defined
#endif
/** \brief State for the AES CBC Encrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_PAD             0x0BU

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_FIN)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_FIN already defined
#endif
/** \brief State for the AES CBC Encrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_FIN         0x0CU

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_FIN)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_FIN already defined
#endif
/** \brief State for the AES CBC Encrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_FIN        0x0DU

/*---Crypto_xVIx_xAIx_CbcPkcs7DecryptStateType--------------------------------------------------------------*/

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_UNINIT)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_UNINIT already defined
#endif
/** \brief State for the AES CBC Decrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_UNINIT           0x00U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_ST)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_ST already defined
#endif
/** \brief State for the AES CBC Decrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_ST        0x01U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_ST)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_ST already defined
#endif
/** \brief State for the AES CBC Decrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_ST       0x02U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_INITIALIZED)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_INITIALIZED already defined
#endif
/** \brief State for the AES CBC Decrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_INITIALIZED      0x03U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_READ_WRITE)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_READ_WRITE already defined
#endif
/** \brief State for the AES CBC Decrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_READ_WRITE       0x04U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_UPD)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_UPD already defined
#endif
/** \brief State for the AES CBC Decrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_UPD       0x05U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_UPD)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_UPD already defined
#endif
/** \brief State for the AES CBC Decrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_UPD      0x06U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_XOR)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_XOR already defined
#endif
/** \brief State for the AES CBC Decrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_XOR              0x07U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_CP_LAST_BL)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_CP_LAST_BL already defined
#endif
/** \brief State for the AES CBC Decrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_CP_LAST_BL       0x08U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_FIN)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_FIN already defined
#endif
/** \brief State for the AES CBC Decrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_FIN       0x09U

#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_FIN)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_FIN already defined
#endif
/** \brief State for the AES CBC Decrypt state machine. */
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_FIN      0x0AU

/*--AES-ECB-COMMON--------------------------------------------------------------------------------*/

/* !LINKSTO EB_Crypto_CryptAlgos_0106,1 */
/** \brief Defines the maximum key length in bytes */
#if (defined CRYPTO_XVIX_XAIX_AES_KEYSEEDLENMAX)
#error CRYPTO_XVIX_XAIX_AES_KEYSEEDLENMAX already defined
#endif
#define  CRYPTO_XVIX_XAIX_AES_KEYSEEDLENMAX   32U

/* !LINKSTO EB_Crypto_CryptAlgos_0105,1 */
/** \brief Defines the length in bytes of the AES state */
#if (defined CRYPTO_XVIX_XAIX_AES_SZESTATE)
#error CRYPTO_XVIX_XAIX_AES_SZESTATE already defined
#endif
#define  CRYPTO_XVIX_XAIX_AES_SZESTATE        16U  /* Block size in 8-bit words; always 16 for AES */

/** \brief Defines the length of the SBox and SBoxInv lookup tables */
#if (defined CRYPTO_XVIX_XAIX_AES_SBOXLEN)
#error CRYPTO_XVIX_XAIX_AES_SBOXLEN already defined
#endif
#define  CRYPTO_XVIX_XAIX_AES_SBOXLEN        256U

/** \brief Defines the length of the Te/Td lookup tables */
#if (defined CRYPTO_XVIX_XAIX_AES_TETDLEN)
#error CRYPTO_XVIX_XAIX_AES_TETDLEN already defined
#endif
#define CRYPTO_XVIX_XAIX_AES_TETDLEN              256U

/* !LINKSTO EB_Crypto_CryptAlgos_0110,1 */
/** \brief Defines the length of the Round Constants table used by
 **        the fast implementation variant
 */
#if (defined CRYPTO_XVIX_XAIX_AES_RCONFASTLEN)
#error CRYPTO_XVIX_XAIX_AES_RCONFASTLEN already defined
#endif
#define CRYPTO_XVIX_XAIX_AES_RCONFASTLEN          10U

/** \brief Converts a byte array to an uint32 using big endian byte order */
#if (defined CRYPTO_XVIX_XAIX_AES_COMMON_BYTEARRAY_TO_UINT32)
#error CRYPTO_XVIX_XAIX_AES_COMMON_BYTEARRAY_TO_UINT32 already defined
#endif
#define CRYPTO_XVIX_XAIX_AES_COMMON_BYTEARRAY_TO_UINT32(src)  \
(uint32)(                                       \
          ((uint32)((src)[0]) << 24U) |         \
          ((uint32)((src)[1]) << 16U) |         \
          ((uint32)((src)[2]) <<  8U) |         \
          ((uint32)((src)[3]))                  \
        )

/** \brief Converts an uint32 to a byte array using big endian byte order */
#if (defined CRYPTO_XVIX_XAIX_AES_COMMON_UINT32_TO_BYTEARRAY)
#error CRYPTO_XVIX_XAIX_AES_COMMON_UINT32_TO_BYTEARRAY already defined
#endif
#define CRYPTO_XVIX_XAIX_AES_COMMON_UINT32_TO_BYTEARRAY(dst, src)  \
do                                                  \
{                                                   \
  (dst)[0] = (uint8)((src) >> 24U);                 \
  (dst)[1] = (uint8)((src) >> 16U);                 \
  (dst)[2] = (uint8)((src) >>  8U);                 \
  (dst)[3] = (uint8)((src)       );                 \
}                                                   \
while (0)

/** \brief Performs a left rotation by three bytes */
#if (defined CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3)
#error CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3 already defined
#endif
#define CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(word)       \
((uint32)((word) << 24U) | (uint32)((word) >>  8U))

/** \brief Performs a left rotation by two bytes */
#if (defined CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2)
#error CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2 already defined
#endif
#define CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(word)       \
((uint32)((word) << 16U) | (uint32)((word) >> 16U))

/** \brief Performs a left rotation by one byte */
#if (defined CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1)
#error CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1 already defined
#endif
#define CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(word)       \
((uint32)((word) <<  8U) | (uint32)((word) >> 24U))

/*--AES-CBC-ENCRYPT-------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON)
#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_ENCRYPT_BLOCK_LEN)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_ENCRYPT_BLOCK_LEN already defined
#endif
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_ENCRYPT_BLOCK_LEN 16U
#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON) */

/*--AES-CBC-DECRYPT-------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON)
#if (defined CRYPTO_XVIX_XAIX_CBC_PKCS7_DECRYPT_BLOCK_LEN)
#error CRYPTO_XVIX_XAIX_CBC_PKCS7_DECRYPT_BLOCK_LEN already defined
#endif
#define CRYPTO_XVIX_XAIX_CBC_PKCS7_DECRYPT_BLOCK_LEN 16U
#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON) */

/*==[Type declarations]===========================================================================*/

/*--AES-COMMON-TYPES------------------------------------------------------------------------------*/

/** \brief  Type of the configured AES algorithm.
 **/
typedef uint32 Crypto_xVIx_xAIx_AES_KeyType;

/** \brief  Configuration type for representing a configured AES algorithm.
 **/
typedef struct
{
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Job,
    uint8                                                         Error
  );
  Crypto_xVIx_xAIx_AES_KeyType                                    TypeKey;
  uint8                                                           ValuPrio;
  uint32                                                          ObjId;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)      Job;
}
Crypto_xVIx_xAIx_AES_ConfigType;

/*--AES-CBC-ENCRYPT-CFG---------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON)
typedef struct
{
  Csm_ConfigIdType                                                SymBlockEncryptCfgId;
  uint32                                                          ObjId;
  Crypto_ProcessingType                                           ProcessingType;
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Job,
    uint8                                                         Error
  );
}
Crypto_xVIx_xAIx_CbcPkcs7EncryptConfigType;
#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON) */

/*--AES-CBC-DECRYPT-CFG---------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON)
typedef struct
{
  Csm_ConfigIdType                                                SymBlockDecryptCfgId;
  uint32 ObjId;
  Crypto_ProcessingType                                           ProcessingType;
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Job,
    uint8                                                         Error
  );
}
Crypto_xVIx_xAIx_CbcPkcs7DecryptConfigType;
#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON) */

/*--AES-ECB-COMMON------------------------------------------------------------------------------*/

/** \brief  AES algorithm implementation state machine states.
 **/
typedef uint8 Crypto_xVIx_xAIx_AESStateType;

/*---[struct]-------------------------------------------------------------------------------------*/

/** \brief  AES algorithm implementation Context structure.
 **/
typedef struct
{
  Crypto_xVIx_xAIx_AESStateType                                   CtxState;
  Std_ReturnType                                                  CtxError;

  uint8                                                           AesState[CRYPTO_XVIX_XAIX_AES_SZESTATE]; /* AES state
  */

  uint8                                                           SzeKey; /* key size in 32-bit words;
  4, 6, 8 */
  uint32                                                          SzeTurn; /* number of turns 10, 12, 14.  */
  uint8                                                           CntTurn; /* counter of turns */
  uint8                                                           KeySeed[CRYPTO_XVIX_XAIX_AES_KEYSEEDLENMAX];
  uint32                                                          KeySeedLen;
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA )           KeyExpdFast;
  uint32                                                          KeyExpdLen;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA )          TxtIput;
  uint32                                                          TxtIputLen;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA )            TxtOput;
  uint32                                                          TxtOputLen;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)      Job;
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Result;
  boolean                                                         CancelFlag;
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Job,
    uint8                                                         Error
  );
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)            IsBusy;
  uint32                                                          ObjId;
  Crypto_OperationModeType                                        CurrentMode;
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA )           TmpOutputLengthPtr;
}
Crypto_xVIx_xAIx_AES_ContextType;

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON)

/** \brief  AES algorithm implementation state machine states.
 **/
typedef uint8 Crypto_xVIx_xAIx_CbcPkcs7EncryptStateType;

typedef struct
{
  Crypto_xVIx_xAIx_CbcPkcs7EncryptStateType                     CtxState;
  Csm_ConfigIdType                                              CfgId;
  Crypto_xVIx_xAIx_SymKeyType                                   KeyPtr;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)         IputTextPtr;
  uint32                                                        IputTextLength;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)           OputTextPtr;
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)          OputTextLengthPtr;
  uint32                                                        OputTextLength;
  uint32                                                        StartOputLength;
  uint32                                                        BlockFilled;
  uint8                                                         Block[CRYPTO_XVIX_XAIX_CBC_PKCS7_ENCRYPT_BLOCK_LEN];
  uint8                                                         LastCipherText[CRYPTO_XVIX_XAIX_CBC_PKCS7_ENCRYPT_BLOCK_LEN];
  uint32                                                        ObjId;
  Crypto_ProcessingType                                         IsSync;
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)          Job,
    uint8                                                                 Error
  );
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)         IsBusy;
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Result;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)    Job;
  Crypto_OperationModeType                                      CurrentMode;
  boolean                                                       CancelFlag;
  boolean                                                       CallbackFlag;
  Std_ReturnType                                                CallbackResult;
}
Crypto_xVIx_xAIx_CbcPkcs7EncryptContextType;

#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON) */

/*--AES-CBC-DECRYPT------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON)

/** \brief  AES algorithm implementation state machine states.
 **/
typedef uint8 Crypto_xVIx_xAIx_CbcPkcs7DecryptStateType;

typedef struct
{
  Crypto_xVIx_xAIx_CbcPkcs7DecryptStateType               CtxState;
  Csm_ConfigIdType                                        CfgId;
  Crypto_xVIx_xAIx_SymKeyType                             KeyPtr;
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   IputTextPtr;
  uint32                                                  IputTextLength;
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)     OputTextPtr;
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    OputTextLengthPtr;
  uint32                                                  OputTextLength;
  uint32                                                  StartOputLength;
  uint32                                                  BlockFilled;
  uint8                                                   Block[CRYPTO_XVIX_XAIX_CBC_PKCS7_DECRYPT_BLOCK_LEN];
  uint8                                                   LastCipherText[CRYPTO_XVIX_XAIX_CBC_PKCS7_DECRYPT_BLOCK_LEN];
  uint8                                                   PlainText[CRYPTO_XVIX_XAIX_CBC_PKCS7_DECRYPT_BLOCK_LEN];
  boolean                                                 BlockToWrite;
  boolean                                                 UpdateExecFlag;
  uint32                                                  ObjId;
  Crypto_ProcessingType                                   IsSync;
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Job,
    uint8                                                          Error
  );
  P2VAR(boolean, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)            IsBusy;
  P2VAR(Std_ReturnType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)     Result;
  P2VAR(Crypto_JobType, TYPEDEF, CRYPTO_xVIx_xAIx_APPL_DATA)       Job;
  Crypto_OperationModeType                               CurrentMode;
  boolean                                                CancelFlag;
  boolean                                                CallbackFlag;
  Std_ReturnType                                         CallbackResult;
}
Crypto_xVIx_xAIx_CbcPkcs7DecryptContextType;

#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON) */

/*==[Declarations of functions with internal linkage]=============================================*/

/*--AES-ECB-COMMON--------------------------------------------------------------------------------*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>
/** \brief  Rijndael's key expansion expands an 16/24/32 byte key into an 176/208/240 bytes key.
 **
 * \param[in,out]  Context  Pointer to Context structure.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESCommonCalculateKeySchedule
(
  P2VAR(Crypto_xVIx_xAIx_AES_ContextType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Context
);

/** \brief  Maps the input byte array stored in the AES state variable to the intermediate
 **         variables used to perform the calculation
 *
 * \param[out]  FirstCol   Pointer to the variable where the values corresponding
 *              to the first column will be stored.
 * \param[out]  SecondCol  Pointer to the variable where the values corresponding
 *              to the second column will be stored.
 * \param[out]  ThirdCol   Pointer to the variable where the values corresponding
 *              to the third column will be stored.
 * \param[out]  FourthCol  Pointer to the variable where the values corresponding
 *              to the fourth column will be stored.
 * \param[in]   Context    Pointer to Context structure.
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESCommonMapState
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                   FirstCol,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                   SecondCol,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                   ThirdCol,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                   FourthCol,
  P2CONST(Crypto_xVIx_xAIx_AES_ContextType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Context
);

/** \brief  Unmaps the output of the algorithm stored in the intermediate variables used to perform
 *         the calculation to the AES state variable
 *
 * \param[in]   FirstCol   Pointer to the variable where the values corresponding
 *              to the first column are stored.
 * \param[in]   SecondCol  Pointer to the variable where the values corresponding
 *              to the second column are stored.
 * \param[in]   ThirdCol   Pointer to the variable where the values corresponding
 *              to the third column are stored.
 * \param[in]   FourthCol  Pointer to the variable where the values corresponding
 *              to the fourth column are stored.
 * \param[out]  Context    Pointer to Context structure.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESCommonUnmapState
(
  P2CONST(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)               FirstCol,
  P2CONST(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)               SecondCol,
  P2CONST(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)               ThirdCol,
  P2CONST(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)               FourthCol,
  P2VAR(Crypto_xVIx_xAIx_AES_ContextType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Context
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--AES-ECB-ENCRYPT-------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>
/**  \brief  Crypto AL level Callback  for the AES Block encryption computation.
 *
 * This function is called at the end of Update (only if configured) and finish mode of the AES
 * Encrypt computation.
 *
 * \param[in]   Job     Pointer to the job which the primive computation belongs to.
 * \param[in]   Result  Result of the step (Update/Finish) for which the callback is called.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
);

/**  \brief  Start AES Block encryption computation.
 *
 * This function requests the start of the AES encryption computation for the given configuration.
 * The start is performed in Crypto_xVIx_xAIx_AES_Encrypt_MainFunction().
 *
 * \param[in]    CfgPtr  A pointer to the configuration for which the start of the AES encryption
 *                       is requested.
 * \param[in]    KeyPtr  A pointer to the key which should be used in the AES Block
 *                       encryption computation.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESEncryptStart
(
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        CfgPtr,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr
);

/** \brief  Update AES Block encryption computation.
 * This function requests the update of the AES Block encryption computation for the
 * given data.
 * The update is performed in Crypto_xVIx_xAIx_AES_Encrypt_MainFunction().
 *
 * \param[in]      IputTextPtr        A pointer to the start of an array which contains the
 *                                    constant plain text that shall be encrypted.
 * \param[in]      IputTextLength     Length of the constant plain text in bytes.
 * \param[out]     OputTextPtr        A pointer to the start of an array where the encrypted
 *                                    text will be stored.
 * \param[in,out]  OputTextLengthPtr  Holds a pointer to a memory location in which the length
 *                                    information in bytes is stored. On calling this function this
 *                                    parameter shall contain the size of the buffer provided
 *                                    by OputTextPtr. When the request has finished, the amount
 *                                    of data that has been encrypted shall be stored.
 *
 * \returns  void
 *
 * \retval   CRYPTO_E_BUSY    If the main function is currently processing a requested service.
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESEncryptUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) IputTextPtr,
  uint32                                                IputTextLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   OputTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  OputTextLengthPtr
);

/** \brief  Finish AES Block encryption computation.
 *
 * This function requests the finishing of the AES Block encryption computation and
 * the storing of the encrypted text in the given buffer.
 * The finish is performed in Crypto_xVIx_xAIx_AES_Encrypt_MainFunction().
 *
 * \returns  void
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESEncryptFinish(void);

/** \brief  Perform the AES Block encryption computation tasks.
 *
 * This function performs the actual AES Block encryption computation tasks
 * which have been requested by the service functions. The function calls the
 * main function of the configured primitive to perform the tasks.
 *
 * \param[in/out] Result        This parameter holds the result of the last called
 *   Crypto_xVIx_xAIx_AESEncryptMainFunction.
 * \param[in/out] Busy          This parameter holds the state of the primitive. This state is set
 * to FALSE after the primitive has performed Start or update or it has Finished.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESEncryptMainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

/** \brief  Calls Crypto_xVIx_xAIx_AESCommonCalculateKeySchedule to expand the key.
 **/
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_EncryptCalculateKeySchedule
(
  void
);

/** \brief  Encrypts the ciphertext based on the extended key and on lookup tables which combine
 *         the ByteSub, ShiftRow and MixColumn transformations.
 *
 * \param[in,out]  Context  Pointer to Context structure.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESEncryptCalculateRounds
(
  P2VAR(Crypto_xVIx_xAIx_AES_ContextType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Context
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) */

/*--AES-ECB-DECRYPT-------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>
/**  \brief  Crypto AL level Callback  for the AES Block Decryption computation.
 *
 * This function is called at the end of Update (only if configured) and finish mode of the AES
 * Encrypt computation.
 *
 * \param[in]   Job     Pointer to the job which the primive computation belongs to.
 * \param[in]   Result  Result of the step (Update/Finish) for which the callback is called.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
);

/**  \brief  Start AES Block decryption computation.
 *
 * This function requests the start of the AES decryption computation for the given configuration.
 * The start is performed in Crypto_xVIx_xAIx_AES_Decrypt_MainFunction().
 *
 * \param[in]    CfgPtr  A pointer to the configuration for which the start of the AES decryption
 *                       is requested.
 * \param[in]    KeyPtr  A pointer to the key which should be used in the AES Block
 *                       decryption computation.
 *
 * \returns  void
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 **/
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESDecryptStart
(
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        CfgPtr,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr
);

/** \brief  Update AES Block decryption computation.
 *
 * This function requests the update of the AES Block decryption computation for the
 * given data.
 * The update is performed in Crypto_xVIx_xAIx_AES_Decrypt_MainFunction().
 *
 * \param[in]      IputTextPtr        A pointer to the start of an array which contains the
 *                                    constant cipher text that shall be decrypted.
 * \param[in]      IputTextLength     Length of the constant cipher text in bytes.
 * \param[out]     OputTextPtr        A pointer to the start of an array where the decrypted
 *                                    text will be stored.
 * \param[in,out]  OputTextLengthPtr  Holds a pointer to a memory location in which the length
 *                                    information in bytes is stored. On calling this function this
 *                                    parameter shall contain the size of the buffer provided
 *                                    by OputTextPtr. When the request has finished, the amount
 *                                    of data that has been decrypted shall be stored.
 *
 * \returns  void
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESDecryptUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) IputTextPtr,
  uint32                                                IputTextLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   OputTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  OputTextLengthPtr
);

/** \brief  Finish AES Block decryption computation.
 *
 * This function requests the finishing of the AES Block decryption computation and
 * the storing of the decrypted text in the given buffer.
 * The finish is performed in Crypto_xVIx_xAIx_AES_Decrypt_MainFunction().
 *
 * \returns  void
 *
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESDecryptFinish(void);

/** \brief  Perform the AES Block decryption computation tasks.
 *
 * This function performs the actual AES Block decryption computation tasks
 * which have been requested by the service functions. The function calls the
 * main function of the configured primitive to perform the tasks.
 *
 * \param[in/out] Result        This parameter holds the result of the last called
 *   Crypto_xVIx_xAIx_AESDecryptMainFunction.
 * \param[in/out] Busy          This parameter holds the state of the primitive. This state is set
 * to FALSE after the primitive has performed Start or update or it has Finished.
 * \returns  void
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 **/
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESDecryptMainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

/** \brief  Calls Crypto_xVIx_xAIx_AESCommonCalculateKeySchedule to expand the key and applies the
 *         InvMixColumn transformation to the expanded key.
 *
 * \returns  void
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESDecryptCalculateKeySchedule
(
  void
);

/** \brief  Decrypts the ciphertext based on the extended key and on lookup tables which combine
 *         the InvByteSub, InvShiftRow and InvMixColumn transformations.
 * \returns  void
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESDecryptCalculateRounds
(
  void
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON) */
/*--AES-CBC-ENCRYPT-------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/**  \brief  Crypto CBC Encrypt Reset
 *
 *           This Function resets the Context of CBC Encrypt.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Reset
(
  void
);

/** \brief  Primitive level callback for the AES CBC Encrypt.This callback is called when the
 * intern calculations for a state are finished.
 *
 * \param[in]   Job     Pointer to the job which the primive computation belongs to.
 * \param[in]   Result  Result of the intern step/state for which the callback is called.
 *
 * \returns  void
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7EncryptCallback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
);

/** \brief  Crypto AL level callback for the AES CBC Encrypt.This callback is called when the
 * AES CBC primitive has completed the Update (if configured) or Finish mode.
 *
 * \param[in]   Job     Pointer to the job which the primive computation belongs to.
 * \param[in]   Result  Result of the intern step/state for which the callback is called.
 *
 * \returns  void
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Job,
  uint8                                                         Result
);

/** \brief  Start the symmetric encryption.
 *
 * This function starts the SymEncrypt service which will
 * encrypt blocks of data.
 *
 * \param[in]      CfgPtr            The service configuration.
 * \param[in]      KeyPtr            Pointer to the key to be used.
 * \param[in]      Job               Holds a pointer to the Job which the primitive computations
 *                                   belongs.
 * \param[in]      IvPtr             Holds the initialisation vector which has to be used during
 *                                   the symmetrical encryption computation.
 *
 * \returns  Error code
 *
 * \retval   E_OK      If the service can be started.
 * \retval   E_NOT_OK  If the key is invalid.
 * \retval   CRYPTO_E_BUSY    If another instance of this service is already
 *                         running.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(Std_ReturnType,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7EncryptStart
(
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        CfgPtr,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                Job,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) IvPtr
);

/** \brief  Stream data to be encrypted (Single-Shot only).
 *
 * This function streams data to be encrypted into the CSE.
 *
 * \param[in]      IputTextPtr        A pointer to the start of an array which contains the
 *                                    constant plain text that shall be encrypted.
 * \param[in]      IputTextLength     Length of the constant plain text in bytes.
 * \param[out]     OputTextPtr        A pointer to the start of an array where the encrypted
 *                                    text will be stored.
 * \param[in,out]  OputTextLengthPtr  Holds a pointer to a memory location in which the length
 *                                    information in bytes is stored. On calling this function this
 *                                    parameter shall contain the size of the buffer provided
 *                                    by OputTextPtr. When the request has finished, the amount
 *                                    of data that has been encrypted shall be stored.
 *
 * \returns  Error code
 *
 * \retval   E_OK            If the update was successfully requested.
 * \retval   E_NOT_OK        If the service has not been started.
 * \retval   CRYPTO_E_BUSY          If another instance of this service is already
 *                               running.
 * \retval   CRYPTO_E_SMALL_BUFFER  If the Result buffer is too small for the
 *                               encrypted data.
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(Std_ReturnType,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7EncryptUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) IputTextPtr,
  uint32                                                IputTextLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   OputTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  OputTextLengthPtr
);

/** \brief  Finish encrypt.
 *
 * This function finishes the encryption.
 *
 * \param[out]     OputTextPtr        A pointer to the start of an array where the encrypted
 *                                    text will be stored.
 * \param[in,out]  OputTextLengthPtr  Holds a pointer to a memory location in which the length
 *                                    information in bytes is stored. On calling this function this
 *                                    parameter shall contain the size of the buffer provided
 *                                    by OputTextPtr. When the request has finished, the amount
 *                                    of data that has been encrypted shall be stored.
 *
 * \returns  Error code
 *
 * \retval   E_OK            If the finish was successfully requested.
 * \retval   E_NOT_OK        If the service has not been started.
 * \retval   CRYPTO_E_BUSY          If another instance of this service is already
 *                               running.
 * \retval   CRYPTO_E_SMALL_BUFFER  If the Result buffer is too small for the
 *                               encrypted data.
 *
 *
 * \returns  void
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(Std_ReturnType,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7EncryptFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  OputTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) OputTextLengthPtr
);

/** \brief  Perform primitive tasks.
 *
 * This function performs periodic tasks of the primitive that may be
 * necessary (e.g. checking if a CSE command has to be sent or if a
 * CSE command has finished). This function is called by a
 * corresponding CSM main function.
 *
 * \param[in/out] Result        This parameter holds the result of the last called
 *   Crypto_xVIx_xAIx_CbcPkcs7EncryptMainFunction.
 * \param[in/out] Busy          This parameter holds the state of the primitive. This state is set
 * to FALSE after the primitive has performed Start or update or it has Finished.
 *
 * \returns  void
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7EncryptMainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

/**
 *
 * \brief State: Request start of symmetrical Block encryption
 *
 * \retval None
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStateReqSbSt
(
  void
);

/**
 *
 * \brief State: Copy input data for processing
 *
 * \retval None
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStateCpFromInp
(
  void
);

/**
 *
 * \brief State: Request update of symmetrical Block encryption
 *
 * \retval None
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStateReqSbUpd
(
  void
);

/**
 *
 * \brief State: Copy Result data to output
 *
 * \retval None
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStateCpToOutp
(
  void
);

/**
 *
 * \brief State: Pad the last Block for encryption
 *
 * \retval None
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStatePadLastBl
(
  void
);

/**
 *
 * \brief State: Request update of symmetrical Block encryption for the last Block
 *
 * \retval None
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStateFReqSbUpd
(
  void
);

/**
 *
 * \brief State: Copy input data for processing
 *
 * \retval None
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStateCpPad
(
  void
);

/**
 *
 * \brief State: Request finish of symmetrical Block encryption
 *
 * \retval None
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStateReqSbFin
(
  void
);

/** \brief Function to perform the callback and job cancellation.
 *
 *     This function is implemented in order to perform the job cancellation
 *     and callback functionality is a single call, instead of having it in the
 *     Crypto_xVIx_xAIx_CbcPkcs7EncryptMainFunction.
 *
 *
 * \Reentrancy {Nonreentrant}
 * \Synchronicity {Synchronous/Asynchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7EncCallbackCancel
(
  void
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON) */

/*--AES-CBC-DECRYPT-------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/**  \brief  Crypto CBC Decrypt Reset
 *
 *           This Function resets the Context of CBC Decrypt.
 *
 * \returns  void
 *
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Reset
(
  void
);

/** \brief  Primitive level callback for the AES CBC Decrypt.This callback is called when the
 * intern calculations for a state are finished.
 *
 * \param[in]   Job     Pointer to the job which the primive computation belongs to.
 * \param[in]   Result  Result of the intern step/state for which the callback is called.
 *
 * \returns  void
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7DecryptCallback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
);

/** \brief  Crypto AL level callback for the AES CBC Decrypt.This callback is called when the
 * AES CBC primitive has completed the Update (if configured) or Finish mode.
 *
 * \param[in]   Job     Pointer to the job which the primive computation belongs to.
 * \param[in]   Result  Result of the intern step/state for which the callback is called.
 *
 * \returns  void
 * \Reentrancy{Non-reentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
);

/** \brief  Start the symmetric decryption.
 *
 * This function starts the SymDecrypt service which will
 * decrypt blocks of data.
 *
 * \param[in]      CfgPtr            The service configuration.
 * \param[in]      KeyPtr            Pointer to the key to be used.
 * \param[in]      Job               Holds a pointer to the job to which the decryption
 *                                   computation belong.
 * \param[in]      IvPtr        Holds the initialisation vector which has to be used during the
 *                                   symmetrical decryption computation.
 *
 * \returns  Error code
 *
 * \retval   E_OK      If the service can be started.
 * \retval   E_NOT_OK  If the key is invalid.
 * \retval   CRYPTO_E_BUSY    If another instance of this service is already
 *                         running.
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(Std_ReturnType,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7DecryptStart
(
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        CfgPtr,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                Job,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) IvPtr
);

/** \brief  Stream data to be decrypted (Single-Shot only).
 *
 * This function streams data to be decrypted into the CSE.
 *
 * \param[in]      IputTextPtr        A pointer to the start of an array which contains the
 *                                    constant plain text that shall be decrypted.
 * \param[in]      IputTextLength     Length of the constant cipher text in bytes.
 * \param[out]     OputTextPtr        A pointer to the start of an array where the decrypted
 *                                    text will be stored.
 * \param[in,out]  OputTextLengthPtr  Holds a pointer to a memory location in which the length
 *                                    information in bytes is stored. On calling this function
 *                                    this parameter shall contain the size of the buffer provided
 *                                    by OputTextPtr. When the request has finished, the amount
 *                                    of data that has been decrypted shall be stored.
 *
 * \returns  Error code
 *
 * \retval   E_OK            If the update was successfully requested.
 * \retval   E_NOT_OK        If the service has not been started.
 * \retval   CRYPTO_E_BUSY          If another instance of this service is already
 *                               running.
 * \retval   CRYPTO_E_SMALL_BUFFER  If the Result buffer is too small for the
 *                               decrypted data.
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(Std_ReturnType,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7DecryptUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) IputTextPtr,
  uint32                                                IputTextLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   OputTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  OputTextLengthPtr
);

/** \brief  Finish Decrypt.
 *
 * This function finishes the decryption.
 *
 * \param[out]     OputTextPtr        A pointer to the start of an array where the decrypted
 *                                    text will be stored.
 * \param[in,out]  OputTextLengthPtr  Holds a pointer to a memory location in which the length
 *                                    information in bytes is stored. On calling this function
 *                                    this parameter shall contain the size of the buffer provided
 *                                    by OputTextPtr. When the request has finished, the amount
 *                                    of data that has been decrypted shall be stored.
 *
 * \returns  Error code
 *
 * \retval   E_OK            If the finish was successfully requested.
 * \retval   E_NOT_OK        If the service has not been started.
 * \retval   CRYPTO_E_BUSY          If another instance of this service is already
 *                               running.
 * \retval   CRYPTO_E_SMALL_BUFFER  If the Result buffer is too small for the
 *                               encrypted data.
 *
 *\Reentrancy{Nonreentrant}
 *\Synchronicity{Synchronous}
 */
static FUNC(Std_ReturnType,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7DecryptFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  OputTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) OputTextLengthPtr
);

/** \brief  Perform primitive tasks.
 *
 * This function performs periodic tasks of the primitive that may be
 * necessary (e.g. checking if a CSE command has to be sent or if a
 * CSE command has finished). This function is called by a
 * corresponding CSM main function.
 *
 * \param[in/out] Result        This parameter holds the result of the last called
 *   Crypto_xVIx_xAIx_CbcPkcs7DecryptMainFunction.
 * \param[in/out] Busy          This parameter holds the state of the primitive. This state is set
 * to FALSE after the primitive has performed Start or update or it has Finished.
 * \returns void
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7DecryptMainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
);

/**
 *
 * \brief State: Request start of symmetrical Block decryption
 *
 * \retval None
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7DecStateReqSbSt
(
  void
);

/**
 *
 * \brief State: Write decrypted data to output
 *
 * \retval None
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7DecStateReadWrite
(
  void
);

/**
 *
 * \brief State: Request update of symmetrical Block decryption
 *
 * \retval None
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7DecStateReqSbUpd
(
  void
);

/**
 *
 * \brief State: XOR of a Block of data
 *
 * \retval None
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7DecStateXOR
(
  void
);

/*
 *
 * \brief State: Write decrypted for last data Block to output
 *
 * \retval None
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7DecStateCpLastBl
(
  void
);

/**
 *
 * \brief State: Request finish of symmetrical Block decryption
 *
 * \retval None
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7DecStateReqSbFin
(
  void
);

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON) */

/*--AES-HELPER------------------------------------------------------------------------------------*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)

/** \brief Calls the Crypto primitives mainfunction in a loop for synchronous Job processing
 *
 * \pre    The primitives API function has returned with E_OK
 * \post   The primitives has finished its computation and invoked the mode specific callback
 * \return void
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESHelperDecryptSynchronousMainCall(void);
/** \brief Decrypt Helper Decrypt callback.
 *  This callback is called when the helper has completed the Start/ update or Finish calculations.
 *  In this callback the parent primitive is informed about the finishing of the helper primitive.
 *
 * \param[in]   Job     Pointer to the job which the primitive computation belongs to.
 * \param[in]   Result  Result of the intern step/state for which the callback is called.
 *
 * \return void
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESHelperDecryptCallbackNotification
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
);

#endif /*#if((CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)*/
#if(CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON)
/** \brief Calls the Crypto primitives mainfunction in a loop for synchronous Job processing
 *
 * \pre    The primitives API function has returned with E_OK
 * \post   The primitives has finished its computation and invoked the mode specific callback
 * \return void
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESHelperEncryptSynchronousMainCall(void);
/** \brief Decrypt Helper Encrypt callback.
 *  This callback is called when the helper has completed the Staer/ update or Finish calculations.
 *  In this callback the parent primitive is informed about the finishing of the helper primitive.
 *
 * \param[in]   Job     Pointer to the job which the primitive computation belongs to.
 * \param[in]   Result  Result of the intern step/state for which the callback is called.
 *
 * \return void
 *
 * \Reentrancy{Nonreentrant}
 * \Synchronicity{Synchronous}
 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Encrypt_CallbackNotification
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
);

#endif /* #if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with internal linkage]=============================================================*/

/*--AES-ECB-COMMON--------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_AES_SCHEDULE == CRYPTO_XVIX_XAIX_AES_SCHEDULE_STATIC)

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/* !LINKSTO EB_Crypto_CryptAlgos_0107,1 */
static CONST(uint8, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_AES_SboxNrm[CRYPTO_XVIX_XAIX_AES_SBOXLEN] =
{
  0x63U, 0x7cU, 0x77U, 0x7bU, 0xf2U, 0x6bU, 0x6fU, 0xc5U,
  0x30U, 0x01U, 0x67U, 0x2bU, 0xfeU, 0xd7U, 0xabU, 0x76U,
  0xcaU, 0x82U, 0xc9U, 0x7dU, 0xfaU, 0x59U, 0x47U, 0xf0U,
  0xadU, 0xd4U, 0xa2U, 0xafU, 0x9cU, 0xa4U, 0x72U, 0xc0U,
  0xb7U, 0xfdU, 0x93U, 0x26U, 0x36U, 0x3fU, 0xf7U, 0xccU,
  0x34U, 0xa5U, 0xe5U, 0xf1U, 0x71U, 0xd8U, 0x31U, 0x15U,
  0x04U, 0xc7U, 0x23U, 0xc3U, 0x18U, 0x96U, 0x05U, 0x9aU,
  0x07U, 0x12U, 0x80U, 0xe2U, 0xebU, 0x27U, 0xb2U, 0x75U,
  0x09U, 0x83U, 0x2cU, 0x1aU, 0x1bU, 0x6eU, 0x5aU, 0xa0U,
  0x52U, 0x3bU, 0xd6U, 0xb3U, 0x29U, 0xe3U, 0x2fU, 0x84U,
  0x53U, 0xd1U, 0x00U, 0xedU, 0x20U, 0xfcU, 0xb1U, 0x5bU,
  0x6aU, 0xcbU, 0xbeU, 0x39U, 0x4aU, 0x4cU, 0x58U, 0xcfU,
  0xd0U, 0xefU, 0xaaU, 0xfbU, 0x43U, 0x4dU, 0x33U, 0x85U,
  0x45U, 0xf9U, 0x02U, 0x7fU, 0x50U, 0x3cU, 0x9fU, 0xa8U,
  0x51U, 0xa3U, 0x40U, 0x8fU, 0x92U, 0x9dU, 0x38U, 0xf5U,
  0xbcU, 0xb6U, 0xdaU, 0x21U, 0x10U, 0xffU, 0xf3U, 0xd2U,
  0xcdU, 0x0cU, 0x13U, 0xecU, 0x5fU, 0x97U, 0x44U, 0x17U,
  0xc4U, 0xa7U, 0x7eU, 0x3dU, 0x64U, 0x5dU, 0x19U, 0x73U,
  0x60U, 0x81U, 0x4fU, 0xdcU, 0x22U, 0x2aU, 0x90U, 0x88U,
  0x46U, 0xeeU, 0xb8U, 0x14U, 0xdeU, 0x5eU, 0x0bU, 0xdbU,
  0xe0U, 0x32U, 0x3aU, 0x0aU, 0x49U, 0x06U, 0x24U, 0x5cU,
  0xc2U, 0xd3U, 0xacU, 0x62U, 0x91U, 0x95U, 0xe4U, 0x79U,
  0xe7U, 0xc8U, 0x37U, 0x6dU, 0x8dU, 0xd5U, 0x4eU, 0xa9U,
  0x6cU, 0x56U, 0xf4U, 0xeaU, 0x65U, 0x7aU, 0xaeU, 0x08U,
  0xbaU, 0x78U, 0x25U, 0x2eU, 0x1cU, 0xa6U, 0xb4U, 0xc6U,
  0xe8U, 0xddU, 0x74U, 0x1fU, 0x4bU, 0xbdU, 0x8bU, 0x8aU,
  0x70U, 0x3eU, 0xb5U, 0x66U, 0x48U, 0x03U, 0xf6U, 0x0eU,
  0x61U, 0x35U, 0x57U, 0xb9U, 0x86U, 0xc1U, 0x1dU, 0x9eU,
  0xe1U, 0xf8U, 0x98U, 0x11U, 0x69U, 0xd9U, 0x8eU, 0x94U,
  0x9bU, 0x1eU, 0x87U, 0xe9U, 0xceU, 0x55U, 0x28U, 0xdfU,
  0x8cU, 0xa1U, 0x89U, 0x0dU, 0xbfU, 0xe6U, 0x42U, 0x68U,
  0x41U, 0x99U, 0x2dU, 0x0fU, 0xb0U, 0x54U, 0xbbU, 0x16U
};

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/* Deviation MISRAC2012-1 */
static CONST(uint8, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_AES_SboxInv[CRYPTO_XVIX_XAIX_AES_SBOXLEN] =
{
  0x52U, 0x09U, 0x6aU, 0xd5U, 0x30U, 0x36U, 0xa5U, 0x38U,
  0xbfU, 0x40U, 0xa3U, 0x9eU, 0x81U, 0xf3U, 0xd7U, 0xfbU,
  0x7cU, 0xe3U, 0x39U, 0x82U, 0x9bU, 0x2fU, 0xffU, 0x87U,
  0x34U, 0x8eU, 0x43U, 0x44U, 0xc4U, 0xdeU, 0xe9U, 0xcbU,
  0x54U, 0x7bU, 0x94U, 0x32U, 0xa6U, 0xc2U, 0x23U, 0x3dU,
  0xeeU, 0x4cU, 0x95U, 0x0bU, 0x42U, 0xfaU, 0xc3U, 0x4eU,
  0x08U, 0x2eU, 0xa1U, 0x66U, 0x28U, 0xd9U, 0x24U, 0xb2U,
  0x76U, 0x5bU, 0xa2U, 0x49U, 0x6dU, 0x8bU, 0xd1U, 0x25U,
  0x72U, 0xf8U, 0xf6U, 0x64U, 0x86U, 0x68U, 0x98U, 0x16U,
  0xd4U, 0xa4U, 0x5cU, 0xccU, 0x5dU, 0x65U, 0xb6U, 0x92U,
  0x6cU, 0x70U, 0x48U, 0x50U, 0xfdU, 0xedU, 0xb9U, 0xdaU,
  0x5eU, 0x15U, 0x46U, 0x57U, 0xa7U, 0x8dU, 0x9dU, 0x84U,
  0x90U, 0xd8U, 0xabU, 0x00U, 0x8cU, 0xbcU, 0xd3U, 0x0aU,
  0xf7U, 0xe4U, 0x58U, 0x05U, 0xb8U, 0xb3U, 0x45U, 0x06U,
  0xd0U, 0x2cU, 0x1eU, 0x8fU, 0xcaU, 0x3fU, 0x0fU, 0x02U,
  0xc1U, 0xafU, 0xbdU, 0x03U, 0x01U, 0x13U, 0x8aU, 0x6bU,
  0x3aU, 0x91U, 0x11U, 0x41U, 0x4fU, 0x67U, 0xdcU, 0xeaU,
  0x97U, 0xf2U, 0xcfU, 0xceU, 0xf0U, 0xb4U, 0xe6U, 0x73U,
  0x96U, 0xacU, 0x74U, 0x22U, 0xe7U, 0xadU, 0x35U, 0x85U,
  0xe2U, 0xf9U, 0x37U, 0xe8U, 0x1cU, 0x75U, 0xdfU, 0x6eU,
  0x47U, 0xf1U, 0x1aU, 0x71U, 0x1dU, 0x29U, 0xc5U, 0x89U,
  0x6fU, 0xb7U, 0x62U, 0x0eU, 0xaaU, 0x18U, 0xbeU, 0x1bU,
  0xfcU, 0x56U, 0x3eU, 0x4bU, 0xc6U, 0xd2U, 0x79U, 0x20U,
  0x9aU, 0xdbU, 0xc0U, 0xfeU, 0x78U, 0xcdU, 0x5aU, 0xf4U,
  0x1fU, 0xddU, 0xa8U, 0x33U, 0x88U, 0x07U, 0xc7U, 0x31U,
  0xb1U, 0x12U, 0x10U, 0x59U, 0x27U, 0x80U, 0xecU, 0x5fU,
  0x60U, 0x51U, 0x7fU, 0xa9U, 0x19U, 0xb5U, 0x4aU, 0x0dU,
  0x2dU, 0xe5U, 0x7aU, 0x9fU, 0x93U, 0xc9U, 0x9cU, 0xefU,
  0xa0U, 0xe0U, 0x3bU, 0x4dU, 0xaeU, 0x2aU, 0xf5U, 0xb0U,
  0xc8U, 0xebU, 0xbbU, 0x3cU, 0x83U, 0x53U, 0x99U, 0x61U,
  0x17U, 0x2bU, 0x04U, 0x7eU, 0xbaU, 0x77U, 0xd6U, 0x26U,
  0xe1U, 0x69U, 0x14U, 0x63U, 0x55U, 0x21U, 0x0cU, 0x7dU
};

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_32
#include <Crypto_xVIx_xAIx_MemMap.h>

static CONST(uint32, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_AES_Td0[CRYPTO_XVIX_XAIX_AES_TETDLEN] =
{
  0x51f4a750U, 0x7e416553U, 0x1a17a4c3U, 0x3a275e96U,
  0x3bab6bcbU, 0x1f9d45f1U, 0xacfa58abU, 0x4be30393U,
  0x2030fa55U, 0xad766df6U, 0x88cc7691U, 0xf5024c25U,
  0x4fe5d7fcU, 0xc52acbd7U, 0x26354480U, 0xb562a38fU,
  0xdeb15a49U, 0x25ba1b67U, 0x45ea0e98U, 0x5dfec0e1U,
  0xc32f7502U, 0x814cf012U, 0x8d4697a3U, 0x6bd3f9c6U,
  0x038f5fe7U, 0x15929c95U, 0xbf6d7aebU, 0x955259daU,
  0xd4be832dU, 0x587421d3U, 0x49e06929U, 0x8ec9c844U,
  0x75c2896aU, 0xf48e7978U, 0x99583e6bU, 0x27b971ddU,
  0xbee14fb6U, 0xf088ad17U, 0xc920ac66U, 0x7dce3ab4U,
  0x63df4a18U, 0xe51a3182U, 0x97513360U, 0x62537f45U,
  0xb16477e0U, 0xbb6bae84U, 0xfe81a01cU, 0xf9082b94U,
  0x70486858U, 0x8f45fd19U, 0x94de6c87U, 0x527bf8b7U,
  0xab73d323U, 0x724b02e2U, 0xe31f8f57U, 0x6655ab2aU,
  0xb2eb2807U, 0x2fb5c203U, 0x86c57b9aU, 0xd33708a5U,
  0x302887f2U, 0x23bfa5b2U, 0x02036abaU, 0xed16825cU,
  0x8acf1c2bU, 0xa779b492U, 0xf307f2f0U, 0x4e69e2a1U,
  0x65daf4cdU, 0x0605bed5U, 0xd134621fU, 0xc4a6fe8aU,
  0x342e539dU, 0xa2f355a0U, 0x058ae132U, 0xa4f6eb75U,
  0x0b83ec39U, 0x4060efaaU, 0x5e719f06U, 0xbd6e1051U,
  0x3e218af9U, 0x96dd063dU, 0xdd3e05aeU, 0x4de6bd46U,
  0x91548db5U, 0x71c45d05U, 0x0406d46fU, 0x605015ffU,
  0x1998fb24U, 0xd6bde997U, 0x894043ccU, 0x67d99e77U,
  0xb0e842bdU, 0x07898b88U, 0xe7195b38U, 0x79c8eedbU,
  0xa17c0a47U, 0x7c420fe9U, 0xf8841ec9U, 0x00000000U,
  0x09808683U, 0x322bed48U, 0x1e1170acU, 0x6c5a724eU,
  0xfd0efffbU, 0x0f853856U, 0x3daed51eU, 0x362d3927U,
  0x0a0fd964U, 0x685ca621U, 0x9b5b54d1U, 0x24362e3aU,
  0x0c0a67b1U, 0x9357e70fU, 0xb4ee96d2U, 0x1b9b919eU,
  0x80c0c54fU, 0x61dc20a2U, 0x5a774b69U, 0x1c121a16U,
  0xe293ba0aU, 0xc0a02ae5U, 0x3c22e043U, 0x121b171dU,
  0x0e090d0bU, 0xf28bc7adU, 0x2db6a8b9U, 0x141ea9c8U,
  0x57f11985U, 0xaf75074cU, 0xee99ddbbU, 0xa37f60fdU,
  0xf701269fU, 0x5c72f5bcU, 0x44663bc5U, 0x5bfb7e34U,
  0x8b432976U, 0xcb23c6dcU, 0xb6edfc68U, 0xb8e4f163U,
  0xd731dccaU, 0x42638510U, 0x13972240U, 0x84c61120U,
  0x854a247dU, 0xd2bb3df8U, 0xaef93211U, 0xc729a16dU,
  0x1d9e2f4bU, 0xdcb230f3U, 0x0d8652ecU, 0x77c1e3d0U,
  0x2bb3166cU, 0xa970b999U, 0x119448faU, 0x47e96422U,
  0xa8fc8cc4U, 0xa0f03f1aU, 0x567d2cd8U, 0x223390efU,
  0x87494ec7U, 0xd938d1c1U, 0x8ccaa2feU, 0x98d40b36U,
  0xa6f581cfU, 0xa57ade28U, 0xdab78e26U, 0x3fadbfa4U,
  0x2c3a9de4U, 0x5078920dU, 0x6a5fcc9bU, 0x547e4662U,
  0xf68d13c2U, 0x90d8b8e8U, 0x2e39f75eU, 0x82c3aff5U,
  0x9f5d80beU, 0x69d0937cU, 0x6fd52da9U, 0xcf2512b3U,
  0xc8ac993bU, 0x10187da7U, 0xe89c636eU, 0xdb3bbb7bU,
  0xcd267809U, 0x6e5918f4U, 0xec9ab701U, 0x834f9aa8U,
  0xe6956e65U, 0xaaffe67eU, 0x21bccf08U, 0xef15e8e6U,
  0xbae79bd9U, 0x4a6f36ceU, 0xea9f09d4U, 0x29b07cd6U,
  0x31a4b2afU, 0x2a3f2331U, 0xc6a59430U, 0x35a266c0U,
  0x744ebc37U, 0xfc82caa6U, 0xe090d0b0U, 0x33a7d815U,
  0xf104984aU, 0x41ecdaf7U, 0x7fcd500eU, 0x1791f62fU,
  0x764dd68dU, 0x43efb04dU, 0xccaa4d54U, 0xe49604dfU,
  0x9ed1b5e3U, 0x4c6a881bU, 0xc12c1fb8U, 0x4665517fU,
  0x9d5eea04U, 0x018c355dU, 0xfa877473U, 0xfb0b412eU,
  0xb3671d5aU, 0x92dbd252U, 0xe9105633U, 0x6dd64713U,
  0x9ad7618cU, 0x37a10c7aU, 0x59f8148eU, 0xeb133c89U,
  0xcea927eeU, 0xb761c935U, 0xe11ce5edU, 0x7a47b13cU,
  0x9cd2df59U, 0x55f2733fU, 0x1814ce79U, 0x73c737bfU,
  0x53f7cdeaU, 0x5ffdaa5bU, 0xdf3d6f14U, 0x7844db86U,
  0xcaaff381U, 0xb968c43eU, 0x3824342cU, 0xc2a3405fU,
  0x161dc372U, 0xbce2250cU, 0x283c498bU, 0xff0d9541U,
  0x39a80171U, 0x080cb3deU, 0xd8b4e49cU, 0x6456c190U,
  0x7bcb8461U, 0xd532b670U, 0x486c5c74U, 0xd0b85742U
};

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_32
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON) */
#endif /* #if (CRYPTO_XVIX_XAIX_AES_SCHEDULE == CRYPTO_XVIX_XAIX_AES_SCHEDULE_STATIC) */

#if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_32
#include <Crypto_xVIx_xAIx_MemMap.h>

/* Deviation MISRAC2012-1 */
static CONST(uint32, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_AES_Te0[CRYPTO_XVIX_XAIX_AES_TETDLEN] =
{
  0xc66363a5U, 0xf87c7c84U, 0xee777799U, 0xf67b7b8dU,
  0xfff2f20dU, 0xd66b6bbdU, 0xde6f6fb1U, 0x91c5c554U,
  0x60303050U, 0x02010103U, 0xce6767a9U, 0x562b2b7dU,
  0xe7fefe19U, 0xb5d7d762U, 0x4dababe6U, 0xec76769aU,
  0x8fcaca45U, 0x1f82829dU, 0x89c9c940U, 0xfa7d7d87U,
  0xeffafa15U, 0xb25959ebU, 0x8e4747c9U, 0xfbf0f00bU,
  0x41adadecU, 0xb3d4d467U, 0x5fa2a2fdU, 0x45afafeaU,
  0x239c9cbfU, 0x53a4a4f7U, 0xe4727296U, 0x9bc0c05bU,
  0x75b7b7c2U, 0xe1fdfd1cU, 0x3d9393aeU, 0x4c26266aU,
  0x6c36365aU, 0x7e3f3f41U, 0xf5f7f702U, 0x83cccc4fU,
  0x6834345cU, 0x51a5a5f4U, 0xd1e5e534U, 0xf9f1f108U,
  0xe2717193U, 0xabd8d873U, 0x62313153U, 0x2a15153fU,
  0x0804040cU, 0x95c7c752U, 0x46232365U, 0x9dc3c35eU,
  0x30181828U, 0x379696a1U, 0x0a05050fU, 0x2f9a9ab5U,
  0x0e070709U, 0x24121236U, 0x1b80809bU, 0xdfe2e23dU,
  0xcdebeb26U, 0x4e272769U, 0x7fb2b2cdU, 0xea75759fU,
  0x1209091bU, 0x1d83839eU, 0x582c2c74U, 0x341a1a2eU,
  0x361b1b2dU, 0xdc6e6eb2U, 0xb45a5aeeU, 0x5ba0a0fbU,
  0xa45252f6U, 0x763b3b4dU, 0xb7d6d661U, 0x7db3b3ceU,
  0x5229297bU, 0xdde3e33eU, 0x5e2f2f71U, 0x13848497U,
  0xa65353f5U, 0xb9d1d168U, 0x00000000U, 0xc1eded2cU,
  0x40202060U, 0xe3fcfc1fU, 0x79b1b1c8U, 0xb65b5bedU,
  0xd46a6abeU, 0x8dcbcb46U, 0x67bebed9U, 0x7239394bU,
  0x944a4adeU, 0x984c4cd4U, 0xb05858e8U, 0x85cfcf4aU,
  0xbbd0d06bU, 0xc5efef2aU, 0x4faaaae5U, 0xedfbfb16U,
  0x864343c5U, 0x9a4d4dd7U, 0x66333355U, 0x11858594U,
  0x8a4545cfU, 0xe9f9f910U, 0x04020206U, 0xfe7f7f81U,
  0xa05050f0U, 0x783c3c44U, 0x259f9fbaU, 0x4ba8a8e3U,
  0xa25151f3U, 0x5da3a3feU, 0x804040c0U, 0x058f8f8aU,
  0x3f9292adU, 0x219d9dbcU, 0x70383848U, 0xf1f5f504U,
  0x63bcbcdfU, 0x77b6b6c1U, 0xafdada75U, 0x42212163U,
  0x20101030U, 0xe5ffff1aU, 0xfdf3f30eU, 0xbfd2d26dU,
  0x81cdcd4cU, 0x180c0c14U, 0x26131335U, 0xc3ecec2fU,
  0xbe5f5fe1U, 0x359797a2U, 0x884444ccU, 0x2e171739U,
  0x93c4c457U, 0x55a7a7f2U, 0xfc7e7e82U, 0x7a3d3d47U,
  0xc86464acU, 0xba5d5de7U, 0x3219192bU, 0xe6737395U,
  0xc06060a0U, 0x19818198U, 0x9e4f4fd1U, 0xa3dcdc7fU,
  0x44222266U, 0x542a2a7eU, 0x3b9090abU, 0x0b888883U,
  0x8c4646caU, 0xc7eeee29U, 0x6bb8b8d3U, 0x2814143cU,
  0xa7dede79U, 0xbc5e5ee2U, 0x160b0b1dU, 0xaddbdb76U,
  0xdbe0e03bU, 0x64323256U, 0x743a3a4eU, 0x140a0a1eU,
  0x924949dbU, 0x0c06060aU, 0x4824246cU, 0xb85c5ce4U,
  0x9fc2c25dU, 0xbdd3d36eU, 0x43acacefU, 0xc46262a6U,
  0x399191a8U, 0x319595a4U, 0xd3e4e437U, 0xf279798bU,
  0xd5e7e732U, 0x8bc8c843U, 0x6e373759U, 0xda6d6db7U,
  0x018d8d8cU, 0xb1d5d564U, 0x9c4e4ed2U, 0x49a9a9e0U,
  0xd86c6cb4U, 0xac5656faU, 0xf3f4f407U, 0xcfeaea25U,
  0xca6565afU, 0xf47a7a8eU, 0x47aeaee9U, 0x10080818U,
  0x6fbabad5U, 0xf0787888U, 0x4a25256fU, 0x5c2e2e72U,
  0x381c1c24U, 0x57a6a6f1U, 0x73b4b4c7U, 0x97c6c651U,
  0xcbe8e823U, 0xa1dddd7cU, 0xe874749cU, 0x3e1f1f21U,
  0x964b4bddU, 0x61bdbddcU, 0x0d8b8b86U, 0x0f8a8a85U,
  0xe0707090U, 0x7c3e3e42U, 0x71b5b5c4U, 0xcc6666aaU,
  0x904848d8U, 0x06030305U, 0xf7f6f601U, 0x1c0e0e12U,
  0xc26161a3U, 0x6a35355fU, 0xae5757f9U, 0x69b9b9d0U,
  0x17868691U, 0x99c1c158U, 0x3a1d1d27U, 0x279e9eb9U,
  0xd9e1e138U, 0xebf8f813U, 0x2b9898b3U, 0x22111133U,
  0xd26969bbU, 0xa9d9d970U, 0x078e8e89U, 0x339494a7U,
  0x2d9b9bb6U, 0x3c1e1e22U, 0x15878792U, 0xc9e9e920U,
  0x87cece49U, 0xaa5555ffU, 0x50282878U, 0xa5dfdf7aU,
  0x038c8c8fU, 0x59a1a1f8U, 0x09898980U, 0x1a0d0d17U,
  0x65bfbfdaU, 0xd7e6e631U, 0x844242c6U, 0xd06868b8U,
  0x824141c3U, 0x299999b0U, 0x5a2d2d77U, 0x1e0f0f11U,
  0x7bb0b0cbU, 0xa85454fcU, 0x6dbbbbd6U, 0x2c16163aU
};

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_32
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) */

/*==[Variables with internal linkage]=============================================================*/

/*--AES-CBC-DECRYPT-------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Conterxt variable for AES CBC Decrypt.
 */
static VAR(Crypto_xVIx_xAIx_CbcPkcs7DecryptContextType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_CbcPkcs7DecryptContext;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON) */

/*--AES-CBC-ENCRYPT--------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief Conterxt variable for AES CBC Encrypt.
 */
static VAR(Crypto_xVIx_xAIx_CbcPkcs7EncryptContextType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_CbcPkcs7EncryptContext;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON) */

/*--AES-ECB-ENCRYPT--------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_32
#include <Crypto_xVIx_xAIx_MemMap.h>

  /** \brief  Static array to hold the expanded key.
 **/

static VAR(uint32, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_AesEncryptExpKey[60U];

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_32
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief  The Context variable which is used to store the internal state of the AES computation.
 **/
static VAR(Crypto_xVIx_xAIx_AES_ContextType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_AESEncryptContext;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) */

/*--AES-ECB-DECRYPT-------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_32
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief  Static array to hold the expanded key.
 **/
 /* Deviation MISRAC2012-1 */
static VAR(uint32, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_AesDecryptExpKey[60U];

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_32
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/** \brief  The Context variable which is used to store the internal state of the AES computation.
 **/
static VAR(Crypto_xVIx_xAIx_AES_ContextType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_AESDecryptContext;

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON) */

/*--AES-HELPER------------------------------------------------------------------------------------*/

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_BOOLEAN
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)

static VAR(boolean, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperDecryptBusy = FALSE;

static volatile VAR(boolean, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperDecryptFlgCallbackNotification = FALSE;

#endif /* #if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON)

static VAR(boolean, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperEncryptBusy = FALSE;

static volatile VAR(boolean, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperEncryptFlgCallbackNotification = FALSE;

#endif /* #if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_BOOLEAN
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON || CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON)

/** \brief Local buffer for the encryption key */
/* Deviation MISRAC2012-1 */
static VAR(Crypto_xVIx_xAIx_SymKeyType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_AesKey = {{0x00U}, 0x00U};

#endif /* #if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON || CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)

static VAR(Std_ReturnType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperDecryptResult = E_OK;

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON)

/** \brief Local buffer for the decryption Iv. */
/* Deviation MISRAC2012-1 */
static VAR(Crypto_xVIx_xAIx_SymKeyType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_DecryptIv = {{0x00U}, 0x00U};

#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON) */
#endif /* #if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON)

static VAR(Std_ReturnType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_HelperEncryptResult = E_OK;

/* !LINKSTO EB_Crypto_01019,1 */
#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON)

/** \brief Local buffer for the encryption Iv. */
/* Deviation MISRAC2012-1 */
static VAR(Crypto_xVIx_xAIx_SymKeyType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_EncryptIv = {{0x00U}, 0x00U};

#endif /* #if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) */
#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)
static P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_xVIx_xAIx_ModeDecryptCallBack)
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                              Error
);
#endif /* #if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON) */

#if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON)
static P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_xVIx_xAIx_ModeEncryptCallBack)
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                              Error
);
#endif /* #if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Constants with external linkage]=============================================================*/

/*==[Variables with external linkage]=============================================================*/

/*==[Definition of functions with external linkage]===============================================*/

/*--AES-HELPER------------------------------------------------------------------------------------*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#if(CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Decrypt_Start
(
  CONST(uint32,CRYPTO_xVIx_xAIx_CONST)                           ObjId,
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
    uint8                                                        Error
  ),
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   Job,
  Crypto_ProcessingType                                          IsSync,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Crypto_xVIx_xAIx_AES_ConfigType Temp;

  if (CRYPTO_XVIX_XAIX_AES_KEY_128 == KeyPtr->length)
  {
    Temp.TypeKey = CRYPTO_XVIX_XAIX_AES_KEY_128;
    RetVal = E_OK;
  }
  else if (CRYPTO_XVIX_XAIX_AES_KEY_192 == KeyPtr->length)
  {
    Temp.TypeKey = CRYPTO_XVIX_XAIX_AES_KEY_192;
    RetVal = E_OK;
  }
  else if (CRYPTO_XVIX_XAIX_AES_KEY_256 == KeyPtr->length)
  {
    Temp.TypeKey = CRYPTO_XVIX_XAIX_AES_KEY_256;
    RetVal = E_OK;
  }
  else
  {
    /* Nothing to do. */
  }


  if (E_OK == RetVal)
  {
    Temp.Job = Job;
    Temp.ObjId = ObjId;
    Crypto_xVIx_xAIx_ModeDecryptCallBack = Crypto_Generic_Callback;
    if(CRYPTO_PROCESSING_ASYNC == IsSync)
    {
      Temp.Crypto_Generic_Callback = Crypto_Generic_Callback;
    }
    else
    {
      Temp.Crypto_Generic_Callback = &Crypto_xVIx_xAIx_AESHelperDecryptCallbackNotification;
    }
    Crypto_xVIx_xAIx_AESDecryptStart(&Temp, KeyPtr);
  }
  return RetVal;
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Decrypt_Update
(
  Crypto_ProcessingType                                 IsSync,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CipherTextPtr,
  uint32                                                CipherTextLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   PlainTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  PlainTextLengthPtr
)
{
  Crypto_xVIx_xAIx_AESDecryptUpdate
  (
    CipherTextPtr,
    CipherTextLength,
    PlainTextPtr,
    PlainTextLengthPtr
  );
  if(CRYPTO_PROCESSING_SYNC == IsSync)
  {
    Crypto_xVIx_xAIx_AESHelperDecryptSynchronousMainCall();
  }
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Decrypt_Finish
(
  Crypto_ProcessingType IsSync
)
{
  /* Finish should not change the output result if called from helper */
  Crypto_xVIx_xAIx_AESDecryptContext.TmpOutputLengthPtr =
    Crypto_xVIx_xAIx_AESDecryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr;
  Crypto_xVIx_xAIx_AESDecryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr =
    &Crypto_xVIx_xAIx_AESDecryptContext.KeySeedLen;
  Crypto_xVIx_xAIx_AESDecryptFinish();
  if (CRYPTO_PROCESSING_SYNC == IsSync)
  {
    Crypto_xVIx_xAIx_AESHelperDecryptSynchronousMainCall();
  }
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Decrypt_MainFunction(void)
{
  Crypto_xVIx_xAIx_AESDecryptMainFunction
  (
    &Crypto_xVIx_xAIx_HelperDecryptResult,
    &Crypto_xVIx_xAIx_HelperDecryptBusy
  );
}

#endif /* #if(CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON) */

#if(CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON)

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Encrypt_Start
(
  CONST(uint32,CRYPTO_xVIx_xAIx_CONST)                            ObjId,
  P2FUNC(void, CRYPTO_xVIx_xAIx_CODE, Crypto_Generic_Callback)
  (
    P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  Job,
    uint8                                                         Error
  ),
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)    Job,
  Crypto_ProcessingType                                           IsSync,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  KeyPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Crypto_xVIx_xAIx_AES_ConfigType Temp;
  if (CRYPTO_XVIX_XAIX_AES_KEY_128 == KeyPtr->length)
  {
    Temp.TypeKey = CRYPTO_XVIX_XAIX_AES_KEY_128;
    RetVal = E_OK;
  }
  else if (CRYPTO_XVIX_XAIX_AES_KEY_192 == KeyPtr->length)
  {
    Temp.TypeKey = CRYPTO_XVIX_XAIX_AES_KEY_192;
    RetVal = E_OK;
  }
  else if (CRYPTO_XVIX_XAIX_AES_KEY_256 == KeyPtr->length)
  {
    Temp.TypeKey = CRYPTO_XVIX_XAIX_AES_KEY_256;
    RetVal = E_OK;
  }
  else
  {
    /* Nothing to be done. */
  }

  if (E_OK == RetVal)
  {
    Temp.Job = Job;
    Temp.ObjId = ObjId;
    Crypto_xVIx_xAIx_ModeEncryptCallBack = Crypto_Generic_Callback;
    if(CRYPTO_PROCESSING_ASYNC == IsSync)
    {
      Temp.Crypto_Generic_Callback = Crypto_Generic_Callback;
    }
    else
    {
      Temp.Crypto_Generic_Callback = &Crypto_xVIx_xAIx_AES_Helper_Encrypt_CallbackNotification;
    }
    Crypto_xVIx_xAIx_AESEncryptStart(&Temp, KeyPtr);
  }
  return RetVal;
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Encrypt_Update
(
  Crypto_ProcessingType                                 IsSync,
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) CipherTextPtr,
  uint32                                                CipherTextLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   PlainTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  PlainTextLengthPtr
)
{
  Crypto_xVIx_xAIx_AESEncryptUpdate
  (
   CipherTextPtr,
   CipherTextLength,
   PlainTextPtr,
   PlainTextLengthPtr
  );

  if(CRYPTO_PROCESSING_SYNC == IsSync)
  {
   Crypto_xVIx_xAIx_AESHelperEncryptSynchronousMainCall();
  }
}
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Encrypt_Finish
(
  Crypto_ProcessingType IsSync
)
{
  /* Finish should not change the output result if called from helper */
  Crypto_xVIx_xAIx_AESEncryptContext.TmpOutputLengthPtr =
    Crypto_xVIx_xAIx_AESEncryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr;
  Crypto_xVIx_xAIx_AESEncryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr =
    &Crypto_xVIx_xAIx_AESEncryptContext.KeySeedLen;
  Crypto_xVIx_xAIx_AESEncryptFinish();
  if (CRYPTO_PROCESSING_SYNC == IsSync)
  {
    Crypto_xVIx_xAIx_AESHelperEncryptSynchronousMainCall();
  }
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Encrypt_SyncSingleCall
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)          InputPtr,
  uint32                                                         InputLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)            OutputPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)           OutputLengthPtr,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr
)
{
  Std_ReturnType                    RetVal = E_OK;
  Crypto_xVIx_xAIx_AES_ContextType  Context;

  TS_PARAM_UNUSED(InputLength);

  Context.KeyExpdFast = Crypto_xVIx_xAIx_AesEncryptExpKey;

  if (CRYPTO_XVIX_XAIX_AES_KEY_128 == KeyPtr->length)
  {
    Context.SzeKey = 4U;
    Context.SzeTurn = 10U;
  }
  else if (CRYPTO_XVIX_XAIX_AES_KEY_192 == KeyPtr->length)
  {
    Context.SzeKey = 6U;
    Context.SzeTurn = 12U;
  }
  else if (CRYPTO_XVIX_XAIX_AES_KEY_256 == KeyPtr->length)
  {
    Context.SzeKey = 8U;
    Context.SzeTurn = 14U;
  }
  else
  {
    RetVal = E_NOT_OK;
  }
  if (E_OK == RetVal)
  {
    Context.KeySeedLen = KeyPtr->length;
    TS_MemCpy
    (
        Context.KeySeed,
        KeyPtr->data,
        KeyPtr->length
    );

    TS_MemCpy
    (
        Context.AesState,
        InputPtr,
        CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH
    );
    Crypto_xVIx_xAIx_AESCommonCalculateKeySchedule(&Context);
    Crypto_xVIx_xAIx_AESEncryptCalculateRounds(&Context);
    /* !LINKSTO EB_Crypto_CryptAlgos_0210,1 */
    if(CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH >= *OutputLengthPtr)
    {

      TS_MemCpy
      (
          OutputPtr,
          Context.AesState,
          *OutputLengthPtr
      );
    }
    else
    {
      /* !LINKSTO EB_Crypto_CryptAlgos_0211,1 */
      TS_MemCpy
      (
          OutputPtr,
          Context.AesState,
          CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH
      );
      *OutputLengthPtr = CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH;
    }
  }
  return RetVal;
}

FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AES_EncryptPreCalc
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)          InputPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)            OutputPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)           OutputLengthPtr,
  uint32                                                         KeyLength,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)           expKey
)
{
  Std_ReturnType                    RetVal = E_OK;
  Crypto_xVIx_xAIx_AES_ContextType  Context;

  Context.KeyExpdLen = 60U;
  Context.KeyExpdFast = expKey;

  if (CRYPTO_XVIX_XAIX_AES_KEY_128 == KeyLength)
  {
    Context.SzeKey = 4U;
    Context.SzeTurn = 10U;
  }
  else if (CRYPTO_XVIX_XAIX_AES_KEY_192 == KeyLength)
  {
    Context.SzeKey = 6U;
    Context.SzeTurn = 12U;
  }
  else if (CRYPTO_XVIX_XAIX_AES_KEY_256 == KeyLength)
  {
    Context.SzeKey = 8U;
    Context.SzeTurn = 14U;
  }
  else
  {
    RetVal = E_NOT_OK;
  }

  if (E_OK == RetVal)
  {
    Context.KeySeedLen = KeyLength;

    TS_MemCpy
    (
        Context.AesState,
        InputPtr,
        CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH
    );
    Crypto_xVIx_xAIx_AESEncryptCalculateRounds(&Context);
    if(CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH >= *OutputLengthPtr)
    {
      TS_MemCpy
      (
          OutputPtr,
          Context.AesState,
          *OutputLengthPtr
      );
    }
    else
    {
      TS_MemCpy
      (
          OutputPtr,
          Context.AesState,
          CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH
      );
      *OutputLengthPtr = CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH;
    }
  }
  return RetVal;
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Encrypt_MainFunction
(
  void
)
{
  Crypto_xVIx_xAIx_AESEncryptMainFunction(
                                           &Crypto_xVIx_xAIx_HelperEncryptResult,
                                           &Crypto_xVIx_xAIx_HelperEncryptBusy
                                         );
}

#endif /* #if(CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) */

/*--CRYPTO-CIPHER-AES-ECB-DECRYPT-AL-LAYER-FUNCTIONS----------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Reset
(
  void
)
{
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  TS_MemBZero(&Crypto_xVIx_xAIx_AESDecryptContext, sizeof(Crypto_xVIx_xAIx_AESDecryptContext));
  SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
}

/* !LINKSTO EB_Crypto_CryptAlgos_1127, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Crypto_xVIx_xAIx_AES_ConfigType Crypto_xVIx_xAIx_AES_ECB_DECRYPT_Cfg =
  {
    NULL_PTR, CRYPTO_XVIX_XAIX_AES_KEY_128, 0U, 0U, NULL_PTR
  };
  /* !LINKSTO SWS_Crypto_00017, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
  Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Reset();
  RetVal = E_NOT_OK;
  Crypto_xVIx_xAIx_AesKey.length = Job->jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
  Crypto_xVIx_xAIx_AES_ECB_DECRYPT_Cfg.TypeKey = Crypto_xVIx_xAIx_AesKey.length;
  if (CRYPTO_XVIX_XAIX_AES_KEY_128 == Crypto_xVIx_xAIx_AesKey.length ||
      CRYPTO_XVIX_XAIX_AES_KEY_192 == Crypto_xVIx_xAIx_AesKey.length ||
      CRYPTO_XVIX_XAIX_AES_KEY_256 == Crypto_xVIx_xAIx_AesKey.length
    )
  {
    RetVal = E_OK;
  }

  if(E_OK == RetVal)
  {
    Crypto_xVIx_xAIx_AES_ECB_DECRYPT_Cfg.Crypto_Generic_Callback =
      &Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_CallBack;
    Crypto_xVIx_xAIx_AES_ECB_DECRYPT_Cfg.Job = Job;
    Crypto_xVIx_xAIx_AES_ECB_DECRYPT_Cfg.ObjId = ObjectId;
    RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet
             (
               Job->cryptoKeyId,
               CRYPTO_KE_CIPHER_KEY,
               Crypto_xVIx_xAIx_AesKey.data,
               &(Crypto_xVIx_xAIx_AesKey.length)
             );
    if(E_OK == RetVal)
    {
      Crypto_xVIx_xAIx_AESDecryptStart(
                                        &Crypto_xVIx_xAIx_AES_ECB_DECRYPT_Cfg,
                                        &Crypto_xVIx_xAIx_AesKey
                                      );
    }
  }
  return RetVal;
}
/* !LINKSTO EB_Crypto_CryptAlgos_1148, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);

  if( Crypto_xVIx_xAIx_AESDecryptContext.CtxState == CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED )
  {
    if (
         ( Job->jobPrimitiveInputOutput.inputLength == CRYPTO_XVIX_XAIX_AES_SZESTATE) &&
         (*(Job->jobPrimitiveInputOutput.outputLengthPtr) == CRYPTO_XVIX_XAIX_AES_SZESTATE)
       )
     {
       Crypto_xVIx_xAIx_AESDecryptUpdate
       (
         Job->jobPrimitiveInputOutput.inputPtr,Job->jobPrimitiveInputOutput.inputLength,
         Job->jobPrimitiveInputOutput.outputPtr,Job->jobPrimitiveInputOutput.outputLengthPtr
       );
       RetVal = E_OK;
     }
     else
     {
       RetVal = E_NOT_OK;
       /* reset state */
       /* Context state administration */
       Crypto_xVIx_xAIx_AESDecryptContext.CtxState = CRYPTO_XVIX_XAIX_AES_STATE_UNINITIALIZED;
     }
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}
/* !LINKSTO EB_Crypto_CryptAlgos_1169, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  TS_PARAM_UNUSED(Job);
  if (Crypto_xVIx_xAIx_AESDecryptContext.CtxState == CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED)
  {
    Crypto_xVIx_xAIx_AESDecryptFinish();
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1209, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* This state indicates, that the Primitive is currently waiting for an Update/Finish call */
  if(CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED == Crypto_xVIx_xAIx_AESDecryptContext.CtxState)
  {
    /* !LINKSTO SWS_Crypto_00144_CORRECTION, 1 */
    Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Reset();
    RetVal = E_OK;
  }
  else
  {
    /* job can not be canceled imideatly */
    Crypto_xVIx_xAIx_AESDecryptContext.CancelFlag = TRUE;
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00183, 1 */
    RetVal = CRYPTO_E_JOB_CANCELED;
  }
  TS_PARAM_UNUSED(Job);
  TS_PARAM_UNUSED(ObjectId);
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1230, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  Crypto_xVIx_xAIx_AESDecryptMainFunction(Result,Busy);
}
#endif /*#if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)*/
/*--CRYPTO-CIPHER-AES-ECB-ENCRYPT-AL-LAYER-FUNCTIONS----------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON)

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Reset
(
  void
)
{
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  TS_MemBZero(&Crypto_xVIx_xAIx_AESEncryptContext, sizeof(Crypto_xVIx_xAIx_AESEncryptContext));
  SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
}
/* !LINKSTO EB_Crypto_CryptAlgos_1128, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Crypto_xVIx_xAIx_AES_ConfigType Crypto_xVIx_xAIx_AES_ECB_ENCRYPT_Cfg =
  {
    NULL_PTR,
    CRYPTO_XVIX_XAIX_AES_KEY_128, 0U, 0U,
    NULL_PTR
  };
  /* !LINKSTO SWS_Crypto_00017, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
  Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Reset();
  RetVal = E_NOT_OK;
  Crypto_xVIx_xAIx_AesKey.length = Job->jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
  Crypto_xVIx_xAIx_AES_ECB_ENCRYPT_Cfg.TypeKey = Crypto_xVIx_xAIx_AesKey.length;
  if (CRYPTO_XVIX_XAIX_AES_KEY_128 == Crypto_xVIx_xAIx_AesKey.length ||
      CRYPTO_XVIX_XAIX_AES_KEY_192 == Crypto_xVIx_xAIx_AesKey.length ||
      CRYPTO_XVIX_XAIX_AES_KEY_256 == Crypto_xVIx_xAIx_AesKey.length
    )
  {
    RetVal = E_OK;
  }

  if(E_OK == RetVal)
  {
    Crypto_xVIx_xAIx_AES_ECB_ENCRYPT_Cfg.Crypto_Generic_Callback =
      &Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_CallBack;
    Crypto_xVIx_xAIx_AES_ECB_ENCRYPT_Cfg.Job = Job;
    Crypto_xVIx_xAIx_AES_ECB_ENCRYPT_Cfg.ObjId = ObjectId;
    RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet
             (
               Job->cryptoKeyId,
               CRYPTO_KE_CIPHER_KEY,
               Crypto_xVIx_xAIx_AesKey.data,
               &(Crypto_xVIx_xAIx_AesKey.length)
             );
    if(E_OK == RetVal)
    {
      Crypto_xVIx_xAIx_AESEncryptStart(
                                        &Crypto_xVIx_xAIx_AES_ECB_ENCRYPT_Cfg,
                                        &Crypto_xVIx_xAIx_AesKey
                                      );
    }
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1149, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  if ( Crypto_xVIx_xAIx_AESEncryptContext.CtxState == CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED )
  {
    if
    (
      (Job->jobPrimitiveInputOutput.inputLength == CRYPTO_XVIX_XAIX_AES_SZESTATE) &&
      (*(Job->jobPrimitiveInputOutput.outputLengthPtr) == CRYPTO_XVIX_XAIX_AES_SZESTATE)
    )
    {
      Crypto_xVIx_xAIx_AESEncryptUpdate
      (
        Job->jobPrimitiveInputOutput.inputPtr,
        Job->jobPrimitiveInputOutput.inputLength,
        Job->jobPrimitiveInputOutput.outputPtr,
        Job->jobPrimitiveInputOutput.outputLengthPtr
      );
      RetVal = E_OK;
    }
    else
    {
      RetVal = E_NOT_OK;
    }
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1170, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  TS_PARAM_UNUSED(Job);
  if (Crypto_xVIx_xAIx_AESEncryptContext.CtxState == CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED)
  {
    Crypto_xVIx_xAIx_AESEncryptFinish();
    RetVal = E_OK;
  }
  else
  {
    RetVal = CRYPTO_E_BUSY;
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1210, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* This state indicates, that the Primitive is currently waiting for an Update/Finish call */
  if(CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED == Crypto_xVIx_xAIx_AESEncryptContext.CtxState)
  {
    /* !LINKSTO SWS_Crypto_00144_CORRECTION, 1 */
    Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Reset();
    RetVal = E_OK;
  }
  else
  {
    /* job can not be canceled imideatly */
    Crypto_xVIx_xAIx_AESEncryptContext.CancelFlag = TRUE;
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00183, 1 */
    RetVal = CRYPTO_E_JOB_CANCELED;
  }
  TS_PARAM_UNUSED(Job);
  TS_PARAM_UNUSED(ObjectId);
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1231, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  Crypto_xVIx_xAIx_AESEncryptMainFunction(Result, Busy);
}

FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_AESPreCalculateKeySchedule
(
  uint8                                               SzeKey,
  uint32                                              SzeTurn,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeySeed,
  P2VAR(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  KeyExpdFastPtr
)
{
  uint8 Index;
  CONST(uint32, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_AES_Rcon_Fast[CRYPTO_XVIX_XAIX_AES_RCONFASTLEN] =
  {
    0x01000000U, 0x02000000U, 0x04000000U, 0x08000000U,
    0x10000000U, 0x20000000U, 0x40000000U, 0x80000000U,
    0x1B000000U, 0x36000000U
  };
  uint32 ExpandedKey[60U] = { 0U };

  /* Copy the first key elements */
  for (Index = 0U; Index < SzeKey; Index++)
  {
    ExpandedKey[Index] = CRYPTO_XVIX_XAIX_AES_COMMON_BYTEARRAY_TO_UINT32(&(KeySeed[Index * 4U]));
  }

  /* Calculate the rest of the subkeys */
  for (Index = SzeKey; Index < (4U * ((SzeTurn) + 1U)); Index++)
  {
    uint32 temp;
    temp = ExpandedKey[Index - 1U];

    if ((Index % SzeKey) == 0U)
    {
      /* Leftmost word (includes the g() function) */
      temp = (
               ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp >> 16U) & 0xFFU] << 24U) ^
               ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp >> 8U) & 0xFFU] << 16U) ^
               ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp ) & 0xFFU] << 8U) ^
               ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp >> 24U) & 0xFFU] )
             ) ^ Crypto_xVIx_xAIx_AES_Rcon_Fast[(Index / (SzeKey)) - 1U];
    }
    else if ((SzeKey > 6U) &&
        ((Index % (SzeKey)) == 4U))
    {
      /* Fifth word (includes the f() function), only used for 256 bit keys */
      temp = ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp >> 24U) & 0xFFU] << 24U) ^
             ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp >> 16U) & 0xFFU] << 16U) ^
             ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp >> 8U) & 0xFFU] << 8U) ^
             ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp ) & 0xFFU] );
    }
    else
    {
      /* Nothing to do (MISRA) */
    }

    ExpandedKey[Index] = ExpandedKey[Index - SzeKey] ^ temp;
  }
  /* copy local expanded key into KeyExpdFastPtr */
  TS_MemCpy(KeyExpdFastPtr, ExpandedKey, 240U);
}

#endif /* #if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) */

/*--CRYPTO-CIPHER-AES-CBC-ENCRYPT-AL-LAYER-FUNCTIONS----------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON)

/* !LINKSTO EB_Crypto_CryptAlgos_1130, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  Crypto_xVIx_xAIx_CbcPkcs7EncryptConfigType Crypto_xVIx_xAIx_AES_CBC_ENCRYPT_Cfg =
  {
    0U,
    FALSE,/*an xpath query to the csm to ask for the processing type of the curent primitive.
    It can be either SYNC or ASYNC*/
    0U,
    NULL_PTR
  };
  /* !LINKSTO SWS_Crypto_00017, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
  Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Reset();
  /*reset key and Iv*/
  Crypto_xVIx_xAIx_AesKey.length = Job->jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
  Crypto_xVIx_xAIx_EncryptIv.length = 16U;
  TS_MemBZero(Crypto_xVIx_xAIx_AesKey.data, sizeof(Crypto_xVIx_xAIx_AesKey.data));
  TS_MemBZero(Crypto_xVIx_xAIx_EncryptIv.data, sizeof(Crypto_xVIx_xAIx_AesKey.data));

  Crypto_xVIx_xAIx_AES_CBC_ENCRYPT_Cfg.Crypto_Generic_Callback =
    &Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_CallBack;
  Crypto_xVIx_xAIx_AES_CBC_ENCRYPT_Cfg.ObjId = ObjectId;
  Crypto_xVIx_xAIx_AES_CBC_ENCRYPT_Cfg.ProcessingType = Job->jobPrimitiveInfo->processingType;
  RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet(
                                    Job->cryptoKeyId,
                                    CRYPTO_KE_CIPHER_KEY,
                                    Crypto_xVIx_xAIx_AesKey.data,
                                    &(Crypto_xVIx_xAIx_AesKey.length)
                                  );

  if(E_OK == RetVal)
  {
    RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet(
                                      Job->cryptoKeyId,
                                      CRYPTO_KE_CIPHER_IV,
                                      Crypto_xVIx_xAIx_EncryptIv.data,
                                      &(Crypto_xVIx_xAIx_EncryptIv.length)
                                    );
  }

  if(E_OK == RetVal)
  {
    RetVal = Crypto_xVIx_xAIx_CbcPkcs7EncryptStart(
                                          &Crypto_xVIx_xAIx_AES_CBC_ENCRYPT_Cfg,
                                          &Crypto_xVIx_xAIx_AesKey,
                                          Job,
                                          &Crypto_xVIx_xAIx_EncryptIv
                                        );
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1150, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  RetVal = Crypto_xVIx_xAIx_CbcPkcs7EncryptUpdate
           (
             Job->jobPrimitiveInputOutput.inputPtr,
             Job->jobPrimitiveInputOutput.inputLength,
             Job->jobPrimitiveInputOutput.outputPtr,
             Job->jobPrimitiveInputOutput.outputLengthPtr
           );
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1171, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);

  if (
      (
        CRYPTO_PROCESSING_ASYNC ==
            Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsSync
      ) &&
      (
        NULL_PTR != Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job &&
        (
          (
            ((uint8)Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job->jobPrimitiveInputOutput.mode) &
            ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
        )
      )
     )
  {
    RetVal =
    Crypto_xVIx_xAIx_CbcPkcs7EncryptFinish
    (
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextPtr,
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLengthPtr
    );
  }
  else
  {
    /* !LINKSTO EB_Crypto_01029, 1 */
    /* !LINKSTO EB_Crypto_01030, 1 */
    RetVal =
    Crypto_xVIx_xAIx_CbcPkcs7EncryptFinish
    (
      Job->jobPrimitiveInputOutput.outputPtr,
      Job->jobPrimitiveInputOutput.outputLengthPtr
    );
  }

  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1211, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* This state indicates, that the Primitive is currently waiting for an Update/Finish call */
  if(CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_INITIALIZED == Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState)
  {
    /* !LINKSTO SWS_Crypto_00122_CORRECTION, 1 */
    Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Reset();
    /* !LINKSTO EB_Crypto_00063, 1 */
    RetVal = E_OK;
  }
  else
  {
    /* job can not be canceled imideatly */
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CancelFlag = TRUE;
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00183, 1 */
    RetVal = CRYPTO_E_JOB_CANCELED;
  }
  TS_PARAM_UNUSED(Job);
  TS_PARAM_UNUSED(ObjectId);
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1232, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  Crypto_xVIx_xAIx_CbcPkcs7EncryptMainFunction(Result, Busy);
}

#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON) */

/*--CRYPTO-CIPHER-AES-CBC-DECRYPT-AL-LAYER-FUNCTIONS----------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON)

/* !LINKSTO EB_Crypto_CryptAlgos_1129, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Start
(
  uint32                                                       ObjectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Crypto_xVIx_xAIx_CbcPkcs7DecryptConfigType Crypto_xVIx_xAIx_AES_CBC_DECRYPT_Cfg =
  {
    0U,
    FALSE,/*an xpath query to the csm to ask for the processing type of the curent primitive.
    It can be either SYNC or ASYNC*/
    0U,
    NULL_PTR
  };

  /*reset key and Iv*/
  Crypto_xVIx_xAIx_AesKey.length = Job->jobPrimitiveInfo->primitiveInfo->algorithm.keyLength;
  Crypto_xVIx_xAIx_DecryptIv.length = 16U;
  TS_MemBZero(Crypto_xVIx_xAIx_AesKey.data, sizeof(Crypto_xVIx_xAIx_AesKey.data));
  TS_MemBZero(Crypto_xVIx_xAIx_DecryptIv.data, sizeof(Crypto_xVIx_xAIx_AesKey.data));

  Crypto_xVIx_xAIx_AES_CBC_DECRYPT_Cfg.Crypto_Generic_Callback =
    &Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_CallBack;
  Crypto_xVIx_xAIx_AES_CBC_DECRYPT_Cfg.ObjId = ObjectId;
  Crypto_xVIx_xAIx_AES_CBC_DECRYPT_Cfg.ProcessingType = Job->jobPrimitiveInfo->processingType;
  /* !LINKSTO SWS_Crypto_00017, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Start2Start, 1 */
  /* !LINKSTO Crypto.Dsn.JobTransition.Update2Start, 1 */
  Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Reset();
  RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet(
                                    Job->cryptoKeyId,
                                    CRYPTO_KE_CIPHER_KEY,
                                    Crypto_xVIx_xAIx_AesKey.data,
                                    &(Crypto_xVIx_xAIx_AesKey.length)
                                  );

  if(E_OK == RetVal)
  {
    RetVal = Crypto_xVIx_xAIx_AL_KeyElementGet(
                                      Job->cryptoKeyId,
                                      CRYPTO_KE_CIPHER_IV,
                                      Crypto_xVIx_xAIx_DecryptIv.data,
                                      &(Crypto_xVIx_xAIx_DecryptIv.length)
                                    );
  }

  if(E_OK == RetVal)
  {
    RetVal = Crypto_xVIx_xAIx_CbcPkcs7DecryptStart(
                                          &Crypto_xVIx_xAIx_AES_CBC_DECRYPT_Cfg,
                                          &Crypto_xVIx_xAIx_AesKey,
                                          Job,
                                          &Crypto_xVIx_xAIx_DecryptIv
                                        );
  }
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1151, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Update
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);
  RetVal =
  Crypto_xVIx_xAIx_CbcPkcs7DecryptUpdate
  (
    Job->jobPrimitiveInputOutput.inputPtr,
    Job->jobPrimitiveInputOutput.inputLength,
    Job->jobPrimitiveInputOutput.outputPtr,
    Job->jobPrimitiveInputOutput.outputLengthPtr
  );
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1172, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Finish
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  TS_PARAM_UNUSED(ObjectId);

  /* !LINKSTO EB_Crypto_01029, 1 */
  /* !LINKSTO EB_Crypto_01030, 1 */
  RetVal =
  Crypto_xVIx_xAIx_CbcPkcs7DecryptFinish
  (
    Job->jobPrimitiveInputOutput.outputPtr,
    Job->jobPrimitiveInputOutput.outputLengthPtr
  );

  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1212, 1 */
FUNC(Std_ReturnType, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Cancel
(
  uint32                                                         ObjectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* This state indicates, that the Primitive is currently waiting for an Update/Finish call */
  if(
      CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_INITIALIZED ==
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState
    )
  {
    /* !LINKSTO SWS_Crypto_00122_CORRECTION, 1 */
    Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Reset();
    /* !LINKSTO EB_Crypto_00063, 1 */
    RetVal = E_OK;
  }
  else
  {
    /* job can not be canceled imideatly */
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CancelFlag = TRUE;
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00183, 1 */
    RetVal = CRYPTO_E_JOB_CANCELED;
  }
  TS_PARAM_UNUSED(Job);
  TS_PARAM_UNUSED(ObjectId);
  return RetVal;
}

/* !LINKSTO EB_Crypto_CryptAlgos_1233, 1 */
FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_MainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  Crypto_xVIx_xAIx_CbcPkcs7DecryptMainFunction(Result, Busy);
}

#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==[Definition of functions with internal linkage]===============================================*/

/*--AES-ECB-ENCRYPT-------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if (
         (
           CRYPTO_OPERATIONMODE_FINISH ==
           (
             CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_AESEncryptContext.CurrentMode
           )
         ) || (E_OK != Result)
       )
    {
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_AESEncryptContext.CurrentMode = CRYPTO_XVIX_XAIX_CIPHER_DEFAULT_CURRENT_MODE;
      /* !LINKSTO EB_Crypto_01050, 1 */
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_AESEncryptContext.ObjId].DriverObjectState =
          CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_AESEncryptContext.ObjId].CurrentJob =
        NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_AESEncryptContext.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Reset();
    }
    else if(
             (
               CRYPTO_OPERATIONMODE_UPDATE ==
               (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_AESEncryptContext.CurrentMode)
             ) &&
             (
               (
                 ((uint8)Crypto_xVIx_xAIx_AESEncryptContext.Job->jobPrimitiveInputOutput.mode) &
                 ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
               ) !=
               ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
             )
           )
    {
      CryIf_CallbackNotification(Job, Result);
    }
    else if(
             (
               (
                 (
                   ((uint8)Crypto_xVIx_xAIx_AESEncryptContext.Job->jobPrimitiveInputOutput.mode) &
                     ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                 ) == ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
               ) ||
               (
                 ((uint8)Crypto_xVIx_xAIx_AESEncryptContext.Job->jobPrimitiveInputOutput.mode) &
                   ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
               ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
             ) && CRYPTO_OPERATIONMODE_START ==
               (CRYPTO_OPERATIONMODE_START & Crypto_xVIx_xAIx_AESEncryptContext.CurrentMode)
           )
    {
      RetVal = Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Update(
                                                           Crypto_xVIx_xAIx_AESEncryptContext.ObjId,
                                                           Job
                                                         );
      if(E_OK != RetVal)
      {
        SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
        Crypto_xVIx_xAIx_AESEncryptContext.CurrentMode = CRYPTO_XVIX_XAIX_CIPHER_DEFAULT_CURRENT_MODE;
        /* !LINKSTO EB_Crypto_01050, 1 */
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_AESEncryptContext.ObjId].DriverObjectState =
            CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_AESEncryptContext.ObjId].CurrentJob = NULL_PTR;
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_AESEncryptContext.ObjId].SkipPeriodicMainFunction = TRUE;
        SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
        /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
        Job->jobState = CRYPTO_JOBSTATE_IDLE;
        CryIf_CallbackNotification(Job, RetVal);
        Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Reset();
      }
    }
    else
    {
      if(
          (
            (
              ((uint8)Crypto_xVIx_xAIx_AESEncryptContext.Job->jobPrimitiveInputOutput.mode) &
                ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
            ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) && CRYPTO_OPERATIONMODE_UPDATE ==
            (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_AESEncryptContext.CurrentMode)
        )
      {
        /* This function can not fail in this case */
        (void)Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Finish(
                                                          Crypto_xVIx_xAIx_AESEncryptContext.ObjId,
                                                          Job
                                                        );
      }
    }
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESEncryptStart
(
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        CfgPtr,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr
)
{
  P2CONST(Crypto_xVIx_xAIx_AES_ConfigType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LocCfgPtr;

  LocCfgPtr = (P2CONST(Crypto_xVIx_xAIx_AES_ConfigType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)) CfgPtr;


  Crypto_xVIx_xAIx_AESEncryptContext.Crypto_Generic_Callback = LocCfgPtr->Crypto_Generic_Callback;
  Crypto_xVIx_xAIx_AESEncryptContext.Job = LocCfgPtr->Job;
  Crypto_xVIx_xAIx_AESEncryptContext.ObjId = LocCfgPtr->ObjId;

  /* Context state administration */
  Crypto_xVIx_xAIx_AESEncryptContext.CurrentMode = CRYPTO_OPERATIONMODE_START;
  /* copy reinterpreted data of argument 'cfgPtr' to 'Crypto_xVIx_xAIx_AESEncryptContext' */
  switch ( LocCfgPtr->TypeKey )
  {
    case CRYPTO_XVIX_XAIX_AES_KEY_128:
    {
      Crypto_xVIx_xAIx_AESEncryptContext.SzeKey = 4U;
      Crypto_xVIx_xAIx_AESEncryptContext.SzeTurn = 10U;
    }
    break;
    case CRYPTO_XVIX_XAIX_AES_KEY_192:
    {
      Crypto_xVIx_xAIx_AESEncryptContext.SzeKey = 6U;
      Crypto_xVIx_xAIx_AESEncryptContext.SzeTurn = 12U;
    }
    break;
    case CRYPTO_XVIX_XAIX_AES_KEY_256:
    {
      Crypto_xVIx_xAIx_AESEncryptContext.SzeKey = 8U;
      Crypto_xVIx_xAIx_AESEncryptContext.SzeTurn = 14U;
    }
    break;
     /* CHECK: NOPARSE */
    /*All the possible values of TypeKey are addressed in the cases above. This function is only
      called through helper, which its self checks that the key type is valid.*/
    /* Caution: this 'default' is required by MISRA-C:2012 */
    default:
    {}
    break;
     /* CHECK: PARSE */
  }

  Crypto_xVIx_xAIx_AESEncryptContext.KeyExpdFast = Crypto_xVIx_xAIx_AesEncryptExpKey;
  /* copy reinterpretetd data of argument 'KeyPtr' to 'Crypto_xVIx_xAIx_AESEncryptContext' */
  Crypto_xVIx_xAIx_AESEncryptContext.KeySeedLen = KeyPtr->length;
  TS_MemCpy( Crypto_xVIx_xAIx_AESEncryptContext.KeySeed, KeyPtr->data, KeyPtr->length );
  /* set misc initial Context values */
  Crypto_xVIx_xAIx_AESEncryptContext.CtxError = E_OK;
  /*Main code start*/
  /* Context state administration */
  Crypto_xVIx_xAIx_AESEncryptContext.CtxState = CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED;

  /* initializations */
  Crypto_xVIx_xAIx_AESEncryptContext.CntTurn = 0U;

  /* callback */
  Crypto_xVIx_xAIx_AESEncryptContext.Crypto_Generic_Callback
  (
    Crypto_xVIx_xAIx_AESEncryptContext.Job,Crypto_xVIx_xAIx_AESEncryptContext.CtxError
  );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESEncryptUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) IputTextPtr,
  uint32                                                IputTextLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   OputTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  OputTextLengthPtr
)
{
  Crypto_xVIx_xAIx_AESEncryptContext.CurrentMode = CRYPTO_OPERATIONMODE_UPDATE;
  /* copy input arguments into Context variables */
  Crypto_xVIx_xAIx_AESEncryptContext.TxtIput = IputTextPtr;
  Crypto_xVIx_xAIx_AESEncryptContext.TxtIputLen = IputTextLength;
  Crypto_xVIx_xAIx_AESEncryptContext.TxtOput = OputTextPtr;
  Crypto_xVIx_xAIx_AESEncryptContext.TxtOputLen = *OputTextLengthPtr;

  /*Main code start*/
  /* Context state administration */
  Crypto_xVIx_xAIx_AESEncryptContext.CtxState = CRYPTO_XVIX_XAIX_AES_STATE_CALCULATING;
  /* !LINKSTO EB_Crypto_CryptAlgos_0108,1 */
  /* map */
  TS_MemCpy(
             Crypto_xVIx_xAIx_AESEncryptContext.AesState,
             Crypto_xVIx_xAIx_AESEncryptContext.TxtIput,
             16U
           );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESEncryptFinish(void)
{
  /* Context state administration */
  Crypto_xVIx_xAIx_AESEncryptContext.CtxState = CRYPTO_XVIX_XAIX_AES_STATE_FINISH;
  Crypto_xVIx_xAIx_AESEncryptContext.CurrentMode = CRYPTO_OPERATIONMODE_FINISH;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESEncryptMainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  if(!Crypto_xVIx_xAIx_CheckSkipMain(
                                      Result,
                                      Busy,
                                      Crypto_xVIx_xAIx_AESEncryptContext.ObjId
                                    )
    )
  {
    Crypto_xVIx_xAIx_AESEncryptContext.IsBusy = Busy;
    Crypto_xVIx_xAIx_AESEncryptContext.Result = Result;
    if((NULL_PTR != Result) && (NULL_PTR != Busy))
    {
      *Crypto_xVIx_xAIx_AESEncryptContext.IsBusy = TRUE;
      *Crypto_xVIx_xAIx_AESEncryptContext.Result = E_OK;
    }
    switch ( Crypto_xVIx_xAIx_AESEncryptContext.CtxState )
    {
      case CRYPTO_XVIX_XAIX_AES_STATE_UNINITIALIZED:
      break;
      /* Crypto_xVIx_xAIx_AESEncryptMainFunction > CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED  */
      case CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED:
      if(NULL_PTR != Busy)
      {
        *Crypto_xVIx_xAIx_AESEncryptContext.IsBusy = FALSE;
      }
      break;
      /* Crypto_xVIx_xAIx_AESEncryptMainFunction > CRYPTO_XVIX_XAIX_AES_STATE_CALCULATING */
      case CRYPTO_XVIX_XAIX_AES_STATE_CALCULATING:
      {
        Crypto_xVIx_xAIx_AES_EncryptCalculateKeySchedule();
        Crypto_xVIx_xAIx_AESEncryptCalculateRounds(&Crypto_xVIx_xAIx_AESEncryptContext);

        /* Context state administration */
        Crypto_xVIx_xAIx_AESEncryptContext.CtxState = CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED;

        TS_MemCpy(
                   Crypto_xVIx_xAIx_AESEncryptContext.TxtOput,
                   Crypto_xVIx_xAIx_AESEncryptContext.AesState,
                   16U
                 );
        Crypto_xVIx_xAIx_AESEncryptContext.TxtOputLen = CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH;

        /* callback */
        if(TRUE == Crypto_xVIx_xAIx_AESEncryptContext.CancelFlag)
        {
          Crypto_xVIx_xAIx_AESEncryptContext.Crypto_Generic_Callback
          (
            Crypto_xVIx_xAIx_AESEncryptContext.Job,
            CRYPTO_E_JOB_CANCELED
          );
          if(
              (NULL_PTR != Crypto_xVIx_xAIx_AESEncryptContext.Result) &&
              (NULL_PTR != Crypto_xVIx_xAIx_AESEncryptContext.IsBusy)
            )
          {
            *Crypto_xVIx_xAIx_AESEncryptContext.IsBusy = FALSE;
            *Crypto_xVIx_xAIx_AESEncryptContext.Result = CRYPTO_E_JOB_CANCELED;
          }
        }
        else
        {
          Crypto_xVIx_xAIx_AESEncryptContext.Crypto_Generic_Callback
          (
            Crypto_xVIx_xAIx_AESEncryptContext.Job,
            Crypto_xVIx_xAIx_AESEncryptContext.CtxError
          );
          if(
              (NULL_PTR != Crypto_xVIx_xAIx_AESEncryptContext.Result) &&
              (NULL_PTR != Crypto_xVIx_xAIx_AESEncryptContext.IsBusy)
            )
          {
            *Crypto_xVIx_xAIx_AESEncryptContext.IsBusy = FALSE;
            *Crypto_xVIx_xAIx_AESEncryptContext.Result = Crypto_xVIx_xAIx_AESEncryptContext.CtxError;
          }
        }
      }
      break;

      /* Crypto_xVIx_xAIx_AESEncryptMainFunction > CRYPTO_XVIX_XAIX_AES_STATE_FINISH */
      case CRYPTO_XVIX_XAIX_AES_STATE_FINISH:
      {
        if(NULL_PTR != Crypto_xVIx_xAIx_AESEncryptContext.IsBusy)
        {
          *Crypto_xVIx_xAIx_AESEncryptContext.IsBusy = TRUE;
        }
        if(
            (
              (
                ((uint8)Crypto_xVIx_xAIx_AESEncryptContext.Job->jobPrimitiveInputOutput.mode) &
                  ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
              ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
            ) && (
                   CRYPTO_PROCESSING_ASYNC ==
                   Crypto_xVIx_xAIx_AESEncryptContext.Job->jobPrimitiveInfo->processingType
                 )
          )
        {
          /* The result will always be a full block in singlecall. */
          *Crypto_xVIx_xAIx_AESEncryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr = 16U;
        }
        else
        {
          /* The result will always be empty in finish. */
          *Crypto_xVIx_xAIx_AESEncryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr = 0U;
        }
        /* reset to correct outputLengthPtr if needed */
        if(NULL_PTR != Crypto_xVIx_xAIx_AESEncryptContext.TmpOutputLengthPtr)
        {
          Crypto_xVIx_xAIx_AESEncryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr =
            Crypto_xVIx_xAIx_AESEncryptContext.TmpOutputLengthPtr;
        }
        /* Context state administration */
        Crypto_xVIx_xAIx_AESEncryptContext.CtxState = CRYPTO_XVIX_XAIX_AES_STATE_UNINITIALIZED;

        /* callback */
        if(TRUE == Crypto_xVIx_xAIx_AESEncryptContext.CancelFlag)
        {
          Crypto_xVIx_xAIx_AESEncryptContext.Crypto_Generic_Callback
          (
            Crypto_xVIx_xAIx_AESEncryptContext.Job,
            CRYPTO_E_JOB_CANCELED
          );
          if(
              (NULL_PTR != Crypto_xVIx_xAIx_AESEncryptContext.Result) &&
              (NULL_PTR != Crypto_xVIx_xAIx_AESEncryptContext.IsBusy)
            )
          {
            *Crypto_xVIx_xAIx_AESEncryptContext.IsBusy = FALSE;
            *Crypto_xVIx_xAIx_AESEncryptContext.Result = CRYPTO_E_JOB_CANCELED;
          }
        }
        else
        {
          Crypto_xVIx_xAIx_AESEncryptContext.Crypto_Generic_Callback
          (
            Crypto_xVIx_xAIx_AESEncryptContext.Job,Crypto_xVIx_xAIx_AESEncryptContext.CtxError
          );
          if(
              (NULL_PTR != Crypto_xVIx_xAIx_AESEncryptContext.Result) &&
              (NULL_PTR != Crypto_xVIx_xAIx_AESEncryptContext.IsBusy)
            )
          {
            *Crypto_xVIx_xAIx_AESEncryptContext.IsBusy = FALSE;
            *Crypto_xVIx_xAIx_AESEncryptContext.Result = Crypto_xVIx_xAIx_AESEncryptContext.CtxError;
          }
        }
      }
      break;
      /* Crypto_xVIx_xAIx_AESEncryptMainFunction > DEFAULT */
      default:
      {
        /* Context state administration */
        Crypto_xVIx_xAIx_AESEncryptContext.CtxState = CRYPTO_XVIX_XAIX_AES_STATE_ERROR;
        Crypto_xVIx_xAIx_AESEncryptContext.CtxError = E_NOT_OK;
      }
      break;
    }
  }
}

/* !LINKSTO EB_Crypto_CryptAlgos_0115,1 */
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_EncryptCalculateKeySchedule
(
  void
)
{
  /* Expand the key */
  Crypto_xVIx_xAIx_AESCommonCalculateKeySchedule(&Crypto_xVIx_xAIx_AESEncryptContext);
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESEncryptCalculateRounds
(
  P2VAR(Crypto_xVIx_xAIx_AES_ContextType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Context
)
{
  uint32 T0 = 0U;
  uint32 T1 = 0U;
  uint32 T2 = 0U;
  uint32 T3 = 0U;
  uint32 S0 = 0U;
  uint32 S1 = 0U;
  uint32 S2 = 0U;
  uint32 S3 = 0U;
  uint8 Index;

  /* Each loop calculates two turns, hence the number of loops is half of the number of turns */
  const uint8 numberOfLoops = (uint8) (Context->SzeTurn / 2U);
  /* Start Index of the last sub-key */
  const uint8 keyExpdFastOffset = (uint8) (Context->SzeTurn * 4U);

  /* Initial turn */
  /* Map the input byte array to the state */
  Crypto_xVIx_xAIx_AESCommonMapState(&S0, &S1, &S2, &S3, Context);

  /* !LINKSTO EB_Crypto_CryptAlgos_0116,1 */
  /* KeyAddition */
  S0 ^= Context->KeyExpdFast[0U];
  S1 ^= Context->KeyExpdFast[1U];
  S2 ^= Context->KeyExpdFast[2U];
  S3 ^= Context->KeyExpdFast[3U];

  /* !LINKSTO EB_Crypto_CryptAlgos_0111,1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_0112,1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_0113,1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_0114,1 */
  /* Main turns */
  for (Index = 0U; Index < numberOfLoops; Index++)
  {
    const uint8 keyExpdFastLoopOffset = Index * 8U;
    T0 = Crypto_xVIx_xAIx_AES_Te0[(S0 >> 24U) & 0xFFU]
        ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Te0[(S1 >> 16U) & 0xFFU]))
        ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Te0[(S2 >> 8U) & 0xFFU]))
        ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Te0[(S3) & 0xFFU]))
        ^ Context->KeyExpdFast[keyExpdFastLoopOffset + 4U];
    T1 = Crypto_xVIx_xAIx_AES_Te0[(S1 >> 24U) & 0xFFU]
        ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Te0[(S2 >> 16U) & 0xFFU]))
        ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Te0[(S3 >> 8U) & 0xFFU]))
        ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Te0[(S0) & 0xFFU]))
        ^ Context->KeyExpdFast[keyExpdFastLoopOffset + 5U];
    T2 = Crypto_xVIx_xAIx_AES_Te0[(S2 >> 24U) & 0xFFU]
        ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Te0[(S3 >> 16U) & 0xFFU]))
        ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Te0[(S0 >> 8U) & 0xFFU]))
        ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Te0[(S1) & 0xFFU]))
        ^ Context->KeyExpdFast[keyExpdFastLoopOffset + 6U];
    T3 = Crypto_xVIx_xAIx_AES_Te0[(S3 >> 24U) & 0xFFU]
        ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Te0[(S0 >> 16U) & 0xFFU]))
        ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Te0[(S1 >> 8U) & 0xFFU]))
        ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Te0[(S2) & 0xFFU]))
        ^ Context->KeyExpdFast[keyExpdFastLoopOffset + 7U];
    /* The last turn does not use the MixColumn layer */
    if (Index < (numberOfLoops - 1U))
    {
      S0 = Crypto_xVIx_xAIx_AES_Te0[(T0 >> 24U) & 0xFFU]
          ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Te0[(T1 >> 16U) & 0xFFU]))
          ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Te0[(T2 >> 8U) & 0xFFU]))
          ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Te0[(T3) & 0xFFU]))
          ^ Context->KeyExpdFast[keyExpdFastLoopOffset + 8U];
      S1 = Crypto_xVIx_xAIx_AES_Te0[(T1 >> 24U) & 0xFFU]
          ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Te0[(T2 >> 16U) & 0xFFU]))
          ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Te0[(T3 >> 8U) & 0xFFU]))
          ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Te0[(T0) & 0xFFU]))
          ^ Context->KeyExpdFast[keyExpdFastLoopOffset + 9U];
      S2 = Crypto_xVIx_xAIx_AES_Te0[(T2 >> 24U) & 0xFFU]
          ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Te0[(T3 >> 16U) & 0xFFU]))
          ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Te0[(T0 >> 8U) & 0xFFU]))
          ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Te0[(T1) & 0xFFU]))
          ^ Context->KeyExpdFast[keyExpdFastLoopOffset + 10U];
      S3 = Crypto_xVIx_xAIx_AES_Te0[(T3 >> 24U) & 0xFFU]
          ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Te0[(T0 >> 16U) & 0xFFU]))
          ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Te0[(T1 >> 8U) & 0xFFU]))
          ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Te0[(T2) & 0xFFU]))
          ^ Context->KeyExpdFast[keyExpdFastLoopOffset + 11U];
    }
  }
  /* !LINKSTO EB_Crypto_CryptAlgos_0117,1 */
  /* Final turn */
  S0 = ((uint32) ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T0 >> 24U) & 0xFFU] << 24U)
      ^ ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T1 >> 16U) & 0xFFU] << 16U)
      ^ ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T2 >> 8U) & 0xFFU] << 8U)
      ^ ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T3) & 0xFFU]))
      ^ Context->KeyExpdFast[keyExpdFastOffset + 0U];
  S1 = ((uint32) ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T1 >> 24U) & 0xFFU] << 24U)
      ^ ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T2 >> 16U) & 0xFFU] << 16U)
      ^ ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T3 >> 8U) & 0xFFU] << 8U)
      ^ ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T0) & 0xFFU]))
      ^ Context->KeyExpdFast[keyExpdFastOffset + 1U];
  S2 = ((uint32) ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T2 >> 24U) & 0xFFU] << 24U)
      ^ ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T3 >> 16U) & 0xFFU] << 16U)
      ^ ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T0 >> 8U) & 0xFFU] << 8U)
      ^ ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T1) & 0xFFU]))
      ^ Context->KeyExpdFast[keyExpdFastOffset + 2U];
  S3 = ((uint32) ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T3 >> 24U) & 0xFFU] << 24U)
      ^ ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T0 >> 16U) & 0xFFU] << 16U)
      ^ ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T1 >> 8U) & 0xFFU] << 8U)
      ^ ((uint32) Crypto_xVIx_xAIx_AES_SboxNrm[(T2) & 0xFFU]))
      ^ Context->KeyExpdFast[keyExpdFastOffset + 3U];

  /* Computation is finished, unmap the state to the byte array */
  Crypto_xVIx_xAIx_AESCommonUnmapState(&S0, &S1, &S2, &S3, Context);
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) */

/*--AES-ECB-DECRYPT-------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if (
         (
           CRYPTO_OPERATIONMODE_FINISH ==
           (
             CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_AESDecryptContext.CurrentMode
           )
         ) || (E_OK != Result)
       )
    {
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_AESDecryptContext.CurrentMode = CRYPTO_XVIX_XAIX_CIPHER_DEFAULT_CURRENT_MODE;
      /* !LINKSTO EB_Crypto_01050, 1 */
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_AESDecryptContext.ObjId].DriverObjectState =
          CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_AESDecryptContext.ObjId].CurrentJob = NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_AESDecryptContext.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Reset();
    }
    else if(
             (
               CRYPTO_OPERATIONMODE_UPDATE ==
               (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_AESDecryptContext.CurrentMode)
             ) &&
             (
               (
                 ((uint8)Crypto_xVIx_xAIx_AESDecryptContext.Job->jobPrimitiveInputOutput.mode) &
                 ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
               ) !=
               (
                 (uint8) CRYPTO_OPERATIONMODE_SINGLECALL
               )
             )
           )
    {
      CryIf_CallbackNotification(Job, Result);
    }
    else if(
             (
               (
                 (
                   ((uint8)Crypto_xVIx_xAIx_AESDecryptContext.Job->jobPrimitiveInputOutput.mode) &
                     ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                 ) == ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
               ) ||
               (
                 ((uint8)Crypto_xVIx_xAIx_AESDecryptContext.Job->jobPrimitiveInputOutput.mode) &
                   ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
               ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
             ) && CRYPTO_OPERATIONMODE_START ==
               (CRYPTO_OPERATIONMODE_START & Crypto_xVIx_xAIx_AESDecryptContext.CurrentMode)
           )
    {
      RetVal = Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Update(
                                                           Crypto_xVIx_xAIx_AESDecryptContext.ObjId,
                                                           Job
                                                         );
      if(E_OK != RetVal)
      {
        SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
        Crypto_xVIx_xAIx_AESDecryptContext.CurrentMode = CRYPTO_XVIX_XAIX_CIPHER_DEFAULT_CURRENT_MODE;
        /* !LINKSTO EB_Crypto_01050, 1 */
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_AESDecryptContext.ObjId].DriverObjectState =
            CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_AESDecryptContext.ObjId].CurrentJob = NULL_PTR;
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_AESDecryptContext.ObjId].SkipPeriodicMainFunction = TRUE;
        SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
        /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
        Job->jobState = CRYPTO_JOBSTATE_IDLE;
        CryIf_CallbackNotification(Job, RetVal);
        Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Reset();
      }
    }
    else
    {
      if(
          (
            (
              ((uint8)Crypto_xVIx_xAIx_AESDecryptContext.Job->jobPrimitiveInputOutput.mode) &
                ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
            ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) && CRYPTO_OPERATIONMODE_UPDATE ==
            (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_AESDecryptContext.CurrentMode)
        )
      {
        /* This function can not fail in this case */
        (void)Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Finish(
                                                          Crypto_xVIx_xAIx_AESDecryptContext.ObjId,
                                                          Job
                                                        );
      }
    }
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESDecryptStart
(
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        CfgPtr,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr
)
{
  P2CONST(Crypto_xVIx_xAIx_AES_ConfigType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LocCfgPtr;

  LocCfgPtr = (P2CONST(Crypto_xVIx_xAIx_AES_ConfigType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)) CfgPtr;

  Crypto_xVIx_xAIx_AESDecryptContext.Crypto_Generic_Callback = LocCfgPtr->Crypto_Generic_Callback;
  Crypto_xVIx_xAIx_AESDecryptContext.Job = LocCfgPtr->Job;
  Crypto_xVIx_xAIx_AESDecryptContext.ObjId = LocCfgPtr->ObjId;

  Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast = Crypto_xVIx_xAIx_AesDecryptExpKey;
  /* Context state administration */
  Crypto_xVIx_xAIx_AESDecryptContext.CurrentMode = CRYPTO_OPERATIONMODE_START;
  /* copy reinterpreted data of argument 'CfgPtr' to 'Crypto_xVIx_xAIx_AESDecryptContext' */
  switch ( LocCfgPtr->TypeKey )
  {
    case CRYPTO_XVIX_XAIX_AES_KEY_128:
    {
      Crypto_xVIx_xAIx_AESDecryptContext.SzeKey = 4U;
      Crypto_xVIx_xAIx_AESDecryptContext.SzeTurn = 10U;
    }
    break;
    case CRYPTO_XVIX_XAIX_AES_KEY_192:
    {
      Crypto_xVIx_xAIx_AESDecryptContext.SzeKey = 6U;
      Crypto_xVIx_xAIx_AESDecryptContext.SzeTurn = 12U;
    }
    break;
    case CRYPTO_XVIX_XAIX_AES_KEY_256:
    {
      Crypto_xVIx_xAIx_AESDecryptContext.SzeKey = 8U;
      Crypto_xVIx_xAIx_AESDecryptContext.SzeTurn = 14U;
    }
    break;
    /* CHECK: NOPARSE */
    /*All the possible values of TypeKey are addressed in the cases above. This function is only
      called through helper, which its self checks that the key type is valid.*/
    /* Caution: this 'default' is required by MISRA-C:2012 */
    default:
    {}
    break;
    /* CHECK: PARSE */
  }

  /* copy reinterpretetd data of argument 'KeyPtr' to 'Crypto_xVIx_xAIx_AESDecryptContext' */
  Crypto_xVIx_xAIx_AESDecryptContext.KeySeedLen = KeyPtr->length;
  TS_MemCpy( Crypto_xVIx_xAIx_AESDecryptContext.KeySeed, KeyPtr->data, KeyPtr->length );
  /* set misc initial Context values */
  Crypto_xVIx_xAIx_AESDecryptContext.CtxError = E_OK;

  /*Main Code Start*/

  /* Context state administration */
  Crypto_xVIx_xAIx_AESDecryptContext.CtxState = CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED;
  /* initializations */
  Crypto_xVIx_xAIx_AESDecryptContext.CntTurn = (uint8) Crypto_xVIx_xAIx_AESDecryptContext.SzeTurn;

  /* callback */
  Crypto_xVIx_xAIx_AESDecryptContext.Crypto_Generic_Callback
  (
    Crypto_xVIx_xAIx_AESDecryptContext.Job,Crypto_xVIx_xAIx_AESDecryptContext.CtxError
  );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESDecryptUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) IputTextPtr,
  uint32                                                IputTextLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   OputTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  OputTextLengthPtr
)
{
  Crypto_xVIx_xAIx_AESDecryptContext.CurrentMode = CRYPTO_OPERATIONMODE_UPDATE;
  /* copy input arguments into Context variables */
  Crypto_xVIx_xAIx_AESDecryptContext.TxtIput = IputTextPtr;
  Crypto_xVIx_xAIx_AESDecryptContext.TxtIputLen = IputTextLength;
  Crypto_xVIx_xAIx_AESDecryptContext.TxtOput = OputTextPtr;
  Crypto_xVIx_xAIx_AESDecryptContext.TxtOputLen = *OputTextLengthPtr;
  /*Main code start*/
  /* Context state administration */
  Crypto_xVIx_xAIx_AESDecryptContext.CtxState = CRYPTO_XVIX_XAIX_AES_STATE_CALCULATING;

  /* !LINKSTO EB_Crypto_CryptAlgos_0108,1 */
  /* map */
  TS_MemCpy
  (
    Crypto_xVIx_xAIx_AESDecryptContext.AesState,
    Crypto_xVIx_xAIx_AESDecryptContext.TxtIput,
    CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH
  );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESDecryptFinish(void)
{
  /* Context state administration */
  Crypto_xVIx_xAIx_AESDecryptContext.CtxState = CRYPTO_XVIX_XAIX_AES_STATE_FINISH;
  Crypto_xVIx_xAIx_AESDecryptContext.CurrentMode = CRYPTO_OPERATIONMODE_FINISH;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESDecryptMainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  if(!Crypto_xVIx_xAIx_CheckSkipMain(
                                      Result,
                                      Busy,
                                      Crypto_xVIx_xAIx_AESDecryptContext.ObjId
                                    )
    )
  {
    Crypto_xVIx_xAIx_AESDecryptContext.Result = Result;
    Crypto_xVIx_xAIx_AESDecryptContext.IsBusy = Busy;
    if(
        (NULL_PTR != Crypto_xVIx_xAIx_AESDecryptContext.Result) &&
        (NULL_PTR != Crypto_xVIx_xAIx_AESDecryptContext.IsBusy)
      )
    {
      *Crypto_xVIx_xAIx_AESDecryptContext.Result = E_OK;
      *Crypto_xVIx_xAIx_AESDecryptContext.IsBusy = TRUE;
    }
    switch ( Crypto_xVIx_xAIx_AESDecryptContext.CtxState )
    {
      /* Crypto_xVIx_xAIx_AESDecryptMainFunction > CRYPTO_XVIX_XAIX_AES_STATE_UNINITIALIZED */
      case CRYPTO_XVIX_XAIX_AES_STATE_UNINITIALIZED:
      break;

      /* Crypto_xVIx_xAIx_AESDecryptMainFunction > CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED */
      case CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED:
      if(NULL_PTR != Busy)
      {
        *Crypto_xVIx_xAIx_AESDecryptContext.IsBusy = FALSE;
      }
      break;

      /* Crypto_xVIx_xAIx_AESDecryptMainFunction > CRYPTO_XVIX_XAIX_AES_STATE_CALCULATING */
      case CRYPTO_XVIX_XAIX_AES_STATE_CALCULATING:
      {
        if(NULL_PTR != Crypto_xVIx_xAIx_AESDecryptContext.IsBusy)
        {
          *Crypto_xVIx_xAIx_AESDecryptContext.IsBusy = TRUE;
        }

        Crypto_xVIx_xAIx_AESDecryptCalculateKeySchedule();
        Crypto_xVIx_xAIx_AESDecryptCalculateRounds();

        /* Context state administration */
        Crypto_xVIx_xAIx_AESDecryptContext.CtxState = CRYPTO_XVIX_XAIX_AES_STATE_INITIALIZED;

        TS_MemCpy
        (
          Crypto_xVIx_xAIx_AESDecryptContext.TxtOput,
          Crypto_xVIx_xAIx_AESDecryptContext.AesState,
          CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH
        );
        Crypto_xVIx_xAIx_AESDecryptContext.TxtOputLen = CRYPTO_XVIX_XAIX_AES_BLOCK_LENGTH;

        /* callback */
        if(TRUE == Crypto_xVIx_xAIx_AESDecryptContext.CancelFlag)
        {
          Crypto_xVIx_xAIx_AESDecryptContext.Crypto_Generic_Callback
          (
            Crypto_xVIx_xAIx_AESDecryptContext.Job,
            CRYPTO_E_JOB_CANCELED
          );
          if(
              (NULL_PTR != Crypto_xVIx_xAIx_AESDecryptContext.Result) &&
              (NULL_PTR != Crypto_xVIx_xAIx_AESDecryptContext.IsBusy)
            )
          {
            *Crypto_xVIx_xAIx_AESDecryptContext.IsBusy = FALSE;
            *Crypto_xVIx_xAIx_AESDecryptContext.Result = CRYPTO_E_JOB_CANCELED;
          }
        }
        else
        {
          Crypto_xVIx_xAIx_AESDecryptContext.Crypto_Generic_Callback
          (
            Crypto_xVIx_xAIx_AESDecryptContext.Job,
            Crypto_xVIx_xAIx_AESDecryptContext.CtxError
          );
          if(
              (NULL_PTR != Crypto_xVIx_xAIx_AESDecryptContext.Result) &&
              (NULL_PTR != Crypto_xVIx_xAIx_AESDecryptContext.IsBusy)
            )
          {
            *Crypto_xVIx_xAIx_AESDecryptContext.Result = Crypto_xVIx_xAIx_AESDecryptContext.CtxError;
            *Crypto_xVIx_xAIx_AESDecryptContext.IsBusy = FALSE;
          }
        }
      }
      break;

      /* Crypto_xVIx_xAIx_AESDecryptMainFunction > CRYPTO_XVIX_XAIX_AES_STATE_FINISH */
      case CRYPTO_XVIX_XAIX_AES_STATE_FINISH:
      {
        if(NULL_PTR != Crypto_xVIx_xAIx_AESDecryptContext.IsBusy)
        {
          *Crypto_xVIx_xAIx_AESDecryptContext.IsBusy = TRUE;
        }

        if(
            (
              (
                ((uint8)Crypto_xVIx_xAIx_AESDecryptContext.Job->jobPrimitiveInputOutput.mode) &
                  ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
              ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
            ) && (
                   CRYPTO_PROCESSING_ASYNC ==
                   Crypto_xVIx_xAIx_AESDecryptContext.Job->jobPrimitiveInfo->processingType
                 )
          )
        {
          /* The result will always be a full block in singlecall. */
          *Crypto_xVIx_xAIx_AESDecryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr = 16U;
        }
        else
        {
          /* The result will always be empty in finish. */
          *Crypto_xVIx_xAIx_AESDecryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr = 0U;
        }
        /* reset to correct outputLengthPtr if needed */
        if(NULL_PTR != Crypto_xVIx_xAIx_AESDecryptContext.TmpOutputLengthPtr)
        {
          Crypto_xVIx_xAIx_AESDecryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr =
            Crypto_xVIx_xAIx_AESDecryptContext.TmpOutputLengthPtr;
        }
        /* Context state administration */
        Crypto_xVIx_xAIx_AESDecryptContext.CtxState = CRYPTO_XVIX_XAIX_AES_STATE_UNINITIALIZED;

        /* callback */
        if(TRUE == Crypto_xVIx_xAIx_AESDecryptContext.CancelFlag)
        {
          Crypto_xVIx_xAIx_AESDecryptContext.Crypto_Generic_Callback
          (
            Crypto_xVIx_xAIx_AESDecryptContext.Job,
            CRYPTO_E_JOB_CANCELED
          );
          if(
              (NULL_PTR != Crypto_xVIx_xAIx_AESDecryptContext.Result) &&
              (NULL_PTR != Crypto_xVIx_xAIx_AESDecryptContext.IsBusy)
            )
          {
            *Crypto_xVIx_xAIx_AESDecryptContext.IsBusy = FALSE;
            *Crypto_xVIx_xAIx_AESDecryptContext.Result = CRYPTO_E_JOB_CANCELED;
          }
        }
        else
        {
          Crypto_xVIx_xAIx_AESDecryptContext.Crypto_Generic_Callback
          (
            Crypto_xVIx_xAIx_AESDecryptContext.Job,
            Crypto_xVIx_xAIx_AESDecryptContext.CtxError
          );
          if(
              (NULL_PTR != Crypto_xVIx_xAIx_AESDecryptContext.Result) &&
              (NULL_PTR != Crypto_xVIx_xAIx_AESDecryptContext.IsBusy)
            )
          {
            *Crypto_xVIx_xAIx_AESDecryptContext.Result = Crypto_xVIx_xAIx_AESDecryptContext.CtxError;
            *Crypto_xVIx_xAIx_AESDecryptContext.IsBusy = FALSE;
          }
        }
      }
      break;
      /* *** Crypto_xVIx_xAIx_AESDecryptMainFunction > DEFAULT */
      default:
      {
        /* Context state administration */
        Crypto_xVIx_xAIx_AESDecryptContext.CtxState = CRYPTO_XVIX_XAIX_AES_STATE_ERROR;

        Crypto_xVIx_xAIx_AESDecryptContext.CtxError = E_NOT_OK;
      }
      break;
    }
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESDecryptCalculateKeySchedule
(
  void
)
{
  uint8 Index;
  /* Expand the key */
  Crypto_xVIx_xAIx_AESCommonCalculateKeySchedule(&Crypto_xVIx_xAIx_AESDecryptContext);

  /* Apply the InvMixColumn transformation to the expanded key */
  for (Index = 1U; Index < Crypto_xVIx_xAIx_AESDecryptContext.SzeTurn; Index++)
  {
    const uint8 loopOffset = Index * 4U;

    Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset + 0U] =
    Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[(Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset
        + 0U] >> 24U)] & 0xFFU]
    ^ CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(
        Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[(Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset
            + 0U] >> 16U) & 0xFFU] & 0xFFU]) ^
    CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[
        (Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset + 0U] >> 8U) & 0xFFU
        ] & 0xFFU]) ^
    CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[
        (Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset + 0U] ) & 0xFFU
        ] & 0xFFU]);
    Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset + 1U] =
    Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[(Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset
        + 1U] >> 24U)] & 0xFFU]
    ^ CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(
        Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[(Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset
            + 1U] >> 16U) & 0xFFU] & 0xFFU]) ^
    CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[
        (Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset + 1U] >> 8U) & 0xFFU
        ] & 0xFFU]) ^
    CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[
        (Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset + 1U] ) & 0xFFU
        ] & 0xFFU]);
    Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset + 2U] =
    Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[(Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset
        + 2U] >> 24U)] & 0xFFU]
    ^ CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(
        Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[(Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset
            + 2U] >> 16U) & 0xFFU] & 0xFFU]) ^
    CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[
        (Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset + 2U] >> 8U) & 0xFFU
        ] & 0xFFU]) ^
    CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[
        (Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset + 2U] ) & 0xFFU
        ] & 0xFFU]);
    Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset + 3U] =
    Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[(Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset
        + 3U] >> 24U)] & 0xFFU]
    ^ CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(
        Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[(Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset
            + 3U] >> 16U) & 0xFFU] & 0xFFU]) ^
    CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[
        (Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset + 3U] >> 8U) & 0xFFU
        ] & 0xFFU]) ^
    CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Td0[Crypto_xVIx_xAIx_AES_SboxNrm[
        (Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[loopOffset + 3U] ) & 0xFFU
        ] & 0xFFU]);
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESDecryptCalculateRounds
(
  void
)
{
  uint32 T0 = 0U;
  uint32 T1 = 0U;
  uint32 T2 = 0U;
  uint32 T3 = 0U;
  uint32 S0 = 0U;
  uint32 S1 = 0U;
  uint32 S2 = 0U;
  uint32 S3 = 0U;
  uint8 Index;
  /* Each loop calculates two turns, hence the number of loops is half of the number of turns */
  const uint8 numberOfLoops = (uint8) (Crypto_xVIx_xAIx_AESDecryptContext.SzeTurn / 2U);
  /* Start Index of the second to last sub-key */
  const uint8 keyExpdFastOffset = (uint8) (Crypto_xVIx_xAIx_AESDecryptContext.SzeTurn * 4U);

  /* Initial turn */
  /* Map the input byte array to the state */
  Crypto_xVIx_xAIx_AESCommonMapState(&S0, &S1, &S2, &S3, &Crypto_xVIx_xAIx_AESDecryptContext);

  /* !LINKSTO EB_Crypto_CryptAlgos_0116,1 */
  /* KeyAddition */
  S0 ^= Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[keyExpdFastOffset];
  S1 ^= Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[keyExpdFastOffset + 1U];
  S2 ^= Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[keyExpdFastOffset + 2U];
  S3 ^= Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[keyExpdFastOffset + 3U];

  /* !LINKSTO EB_Crypto_CryptAlgos_0111,1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_0112,1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_0113,1 */
  /* !LINKSTO EB_Crypto_CryptAlgos_0114,1 */
  for (Index = 0U; Index < numberOfLoops; Index++)
  {
    const uint8 keyExpdFastLoopOffset = (keyExpdFastOffset - (Index * 8U));

    T0 = Crypto_xVIx_xAIx_AES_Td0[(S0 >> 24U) & 0xFFU]
    ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Td0[(S3 >> 16U) & 0xFFU]))
    ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Td0[(S2 >> 8U) & 0xFFU]))
    ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Td0[(S1) & 0xFFU]))
    ^ Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[keyExpdFastLoopOffset - 4U];
    T1 = Crypto_xVIx_xAIx_AES_Td0[(S1 >> 24U) & 0xFFU]
    ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Td0[(S0 >> 16U) & 0xFFU]))
    ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Td0[(S3 >> 8U) & 0xFFU]))
    ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Td0[(S2) & 0xFFU]))
    ^ Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[keyExpdFastLoopOffset - 3U];
    T2 = Crypto_xVIx_xAIx_AES_Td0[(S2 >> 24U) & 0xFFU]
    ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Td0[(S1 >> 16U) & 0xFFU]))
    ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Td0[(S0 >> 8U) & 0xFFU]))
    ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Td0[(S3) & 0xFFU]))
    ^ Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[keyExpdFastLoopOffset - 2U];
    T3 = Crypto_xVIx_xAIx_AES_Td0[(S3 >> 24U) & 0xFFU]
    ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Td0[(S2 >> 16U) & 0xFFU]))
    ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Td0[(S1 >> 8U) & 0xFFU]))
    ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Td0[(S0) & 0xFFU]))
    ^ Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[keyExpdFastLoopOffset - 1U];

    /* The last turn does not use the InvMixColumn layer */
    if (Index < (numberOfLoops - 1U))
    {
      S0 = Crypto_xVIx_xAIx_AES_Td0[(T0 >> 24U) & 0xFFU]
      ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Td0[(T3 >> 16U) & 0xFFU]))
      ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Td0[(T2 >> 8U) & 0xFFU]))
      ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Td0[(T1) & 0xFFU]))
      ^ Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[keyExpdFastLoopOffset - 8U];
      S1 = Crypto_xVIx_xAIx_AES_Td0[(T1 >> 24U) & 0xFFU]
      ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Td0[(T0 >> 16U) & 0xFFU]))
      ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Td0[(T3 >> 8U) & 0xFFU]))
      ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Td0[(T2) & 0xFFU]))
      ^ Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[keyExpdFastLoopOffset - 7U];
      S2 = Crypto_xVIx_xAIx_AES_Td0[(T2 >> 24U) & 0xFFU]
      ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Td0[(T1 >> 16U) & 0xFFU]))
      ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Td0[(T0 >> 8U) & 0xFFU]))
      ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Td0[(T3) & 0xFFU]))
      ^ Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[keyExpdFastLoopOffset - 6U];
      S3 = Crypto_xVIx_xAIx_AES_Td0[(T3 >> 24U) & 0xFFU]
      ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL3(Crypto_xVIx_xAIx_AES_Td0[(T2 >> 16U) & 0xFFU]))
      ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL2(Crypto_xVIx_xAIx_AES_Td0[(T1 >> 8U) & 0xFFU]))
      ^ (CRYPTO_XVIX_XAIX_AES_COMMON_ROTL1(Crypto_xVIx_xAIx_AES_Td0[(T0) & 0xFFU]))
      ^ Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[keyExpdFastLoopOffset - 5U];
    }
  }

  /* !LINKSTO EB_Crypto_CryptAlgos_0117,1 */
  /* Final turn */
  S0 = (
         (uint32) ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T0 >> 24U) & 0xFFU] << 24U) ^
         ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T3 >> 16U) & 0xFFU] << 16U) ^
         ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T2 >> 8U) & 0xFFU] << 8U) ^
         ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T1) & 0xFFU])
       ) ^ Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[0U];
  S1 = (
         (uint32) ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T1 >> 24U) & 0xFFU] << 24U) ^
         ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T0 >> 16U) & 0xFFU] << 16U) ^
         ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T3 >> 8U) & 0xFFU] << 8U) ^
         ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T2) & 0xFFU])
       ) ^ Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[1U];
  S2 = (
         (uint32) ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T2 >> 24U) & 0xFFU] << 24U) ^
         ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T1 >> 16U) & 0xFFU] << 16U) ^
         ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T0 >> 8U) & 0xFFU] << 8U) ^
         ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T3) & 0xFFU])
       ) ^ Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[2U];
  S3 = (
         (uint32) ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T3 >> 24U) & 0xFFU] << 24U) ^
         ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T2 >> 16U) & 0xFFU] << 16U) ^
         ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T1 >> 8U) & 0xFFU] << 8U) ^
         ((uint32) Crypto_xVIx_xAIx_AES_SboxInv[(T0) & 0xFFU])
       ) ^ Crypto_xVIx_xAIx_AESDecryptContext.KeyExpdFast[3U];

  /* Computation is finished, unmap the state to the byte array */
  Crypto_xVIx_xAIx_AESCommonUnmapState(&S0, &S1, &S2, &S3, &Crypto_xVIx_xAIx_AESDecryptContext);
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON) */

/*--AES-CBC-ENCRYPT------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Reset
(
  void
)
{
  Crypto_xVIx_xAIx_AL_ENCRYPT_AES_ECB_Reset();
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  TS_MemBZero(
               &Crypto_xVIx_xAIx_CbcPkcs7EncryptContext,
               sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext)
             );
  SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                              Result
)
{
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if(
        (
          CRYPTO_OPERATIONMODE_FINISH ==
          (
            CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CurrentMode
          )
        ) || (E_OK != Result)
      )
    {
      /* !LINKSTO EB_Crypto_01050, 1 */
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CurrentMode = CRYPTO_XVIX_XAIX_CIPHER_DEFAULT_CURRENT_MODE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.ObjId].DriverObjectState =
        CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.ObjId].CurrentJob = NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Reset();
    }
    if(
        (
          CRYPTO_OPERATIONMODE_UPDATE ==
          (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CurrentMode)
        ) &&
        (
          (
            ((uint8)Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job->jobPrimitiveInputOutput.mode) &
            ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) !=
          (
            (uint8) CRYPTO_OPERATIONMODE_SINGLECALL
          )
        )
      )
    {
      CryIf_CallbackNotification(Job, Result);
    }
  }
}

/* !LINKSTO EB_Crypto_CryptAlgos_0961, 1 */
static FUNC(Std_ReturnType,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7EncryptStart
(
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        CfgPtr,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                Job,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) IvPtr
)
{
  P2CONST(Crypto_xVIx_xAIx_CbcPkcs7EncryptConfigType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LocCfgPtr;
  Std_ReturnType LocRetVal;

  LocCfgPtr = (P2CONST(Crypto_xVIx_xAIx_CbcPkcs7EncryptConfigType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)) CfgPtr;

  LocRetVal = E_NOT_OK;

  if(IvPtr->length == sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText))
  {

    LocRetVal = E_OK;
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CurrentMode = CRYPTO_OPERATIONMODE_START;

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_ST;

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job = Job;

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.ObjId = LocCfgPtr->ObjId;

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Crypto_Generic_Callback = LocCfgPtr->Crypto_Generic_Callback;

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsSync = LocCfgPtr->ProcessingType;

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CfgId = LocCfgPtr->SymBlockEncryptCfgId;

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.KeyPtr = *KeyPtr;

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.BlockFilled = 0;

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLength = 0;

    /* !LINKSTO EB_Crypto_CryptAlgos_0205,1 */
    /* copy Iv to lastCipherText to use in first block */
    TS_MemCpy
    (
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText,
      IvPtr->data,
      IvPtr->length
    );
  }

  return LocRetVal;
}

static FUNC(Std_ReturnType,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7EncryptUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) IputTextPtr,
  uint32                                                IputTextLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   OputTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  OputTextLengthPtr
)
{
  Std_ReturnType LocRetVal;
  LocRetVal = E_OK;

  if ( Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState == CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_INITIALIZED )
  {
    /* Context state administration */
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_FROM_INP;
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLength = 0U;

    /* copy input arguments into Context variables */
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IputTextPtr = IputTextPtr;
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IputTextLength = IputTextLength;
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextPtr = OputTextPtr;
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLengthPtr = OputTextLengthPtr;
  }
  else
  {
    LocRetVal = CRYPTO_E_BUSY;
  }

  return LocRetVal;
}

static FUNC(Std_ReturnType,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7EncryptFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  OputTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) OputTextLengthPtr
)
{
  Std_ReturnType LocRetVal;
  LocRetVal = E_OK;

  if ( Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState == CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_INITIALIZED )
  {
    /* Context state administration */
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_PAD_LAST_BL;

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextPtr = OputTextPtr;
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLengthPtr = OputTextLengthPtr;
  }
  else
  {
    LocRetVal = CRYPTO_E_BUSY;
  }
  return LocRetVal;
}

static FUNC(void,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7EncryptMainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  if(!Crypto_xVIx_xAIx_CheckSkipMain(
                                      Result,
                                      Busy,
                                      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.ObjId
                                    )
    )
  {
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsBusy = Busy;
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Result = Result;
    if((NULL_PTR != Result) && (NULL_PTR != Busy))
    {
      *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsBusy = TRUE;
      *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Result = E_OK;
    }
    switch ( Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState )
    {
      case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_UNINIT:
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_ST:
      {
        Crypto_xVIx_xAIx_CBCPkcs7EncStateReqSbSt();
      }
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_INITIALIZED:
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_FROM_INP:
      {
        Crypto_xVIx_xAIx_CBCPkcs7EncStateCpFromInp();
      }
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_UPD:
      {
        Crypto_xVIx_xAIx_CBCPkcs7EncStateReqSbUpd();
      }
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_TO_OUTP:
      {
        Crypto_xVIx_xAIx_CBCPkcs7EncStateCpToOutp();
      }
      break;

      /* Crypto_xVIx_xAIx_CbcPkcs7EncryptMainFunction > CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_PAD_LAST_BL */
      case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_PAD_LAST_BL:
      {
        Crypto_xVIx_xAIx_CBCPkcs7EncStatePadLastBl();
      }
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_F_REQ_SB_UPD:
      {
        Crypto_xVIx_xAIx_CBCPkcs7EncStateFReqSbUpd();
      }
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_PAD:
      {
        Crypto_xVIx_xAIx_CBCPkcs7EncStateCpPad();
      }
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_FIN:
      {
        Crypto_xVIx_xAIx_CBCPkcs7EncStateReqSbFin();
      }
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_ST:
      case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_UPD:
      case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_F_WAIT_SB_UPD:
      case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_FIN:
      {
        /* !LINKSTO EB_Crypto_CryptAlgos_0203,1 */
        Crypto_xVIx_xAIx_AES_Helper_Encrypt_MainFunction();
      }
      break;
      /*  Crypto_xVIx_xAIx_CbcPkcs7EncryptMainFunction > DEFAULT  */
      /* CHECK: NOPARSE */
      /*All the states that the state machine can have are addressed in the above cases.*/
      /* Defensive Programming - unreachable code. This default case is required by MISRA-C:2012 */
      default:
      {}
      break;
      /* CHECK: PARSE */
    }
    Crypto_xVIx_xAIx_CbcPkcs7EncCallbackCancel();
  }
}
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7EncCallbackCancel
(
  void
)
{
  /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00181, 1, SWS_Crypto_00144_CORRECTION, 1 */
  if(TRUE == Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CancelFlag)
  {
    if (
         (NULL_PTR != Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Result)
         && (NULL_PTR != Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsBusy)
       )
    {
      *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsBusy = FALSE;
      *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Result = CRYPTO_E_JOB_CANCELED;
      /* processingType is synchronous */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveCancel2Idle, 1 */
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job->jobState = CRYPTO_JOBSTATE_IDLE;
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.ObjId].DriverObjectState =
      CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.ObjId].CurrentJob =
      NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
    }
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Crypto_Generic_Callback
    (
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job,
      CRYPTO_E_JOB_CANCELED
    );
  }
  else
  {
    if(TRUE == Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackFlag)
    {
      if (
           (NULL_PTR != Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Result)
           && (NULL_PTR != Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsBusy)
         )
      {
        *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsBusy = FALSE;
        *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Result =
        Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackResult;
      }
      if(
          ( CRYPTO_PROCESSING_ASYNC ==
              Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job->jobPrimitiveInfo->processingType
          ) &&
          (
            (
              (
                ((uint8)Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job->jobPrimitiveInputOutput.mode) &
                ((uint8)CRYPTO_OPERATIONMODE_SINGLECALL)
              ) == ((uint8)CRYPTO_OPERATIONMODE_SINGLECALL)
            ) ||
            (
              (
                ((uint8)Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job->jobPrimitiveInputOutput.mode) &
                ((uint8)CRYPTO_OPERATIONMODE_STREAMSTART)
              ) == ((uint8)CRYPTO_OPERATIONMODE_STREAMSTART)
            )
          ) && (CRYPTO_OPERATIONMODE_START == Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CurrentMode)
        )
      {
        Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.StartOputLength =
            *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr;
        (void)Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Update
              (
                Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.ObjId,
                Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job
              );
      }
      else
      {
        if(
            (
              CRYPTO_PROCESSING_ASYNC ==
                  Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsSync
            ) &&
            (
              (
                (
                  ((uint8)Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job->jobPrimitiveInputOutput.mode) &
                  ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
              )
            ) && (CRYPTO_OPERATIONMODE_UPDATE == Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CurrentMode)
          )
        {
          /* !LINKSTO EB_Crypto_01029, 1 */
          Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job->jobPrimitiveInputOutput.outputPtr =
          &(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job->jobPrimitiveInputOutput.outputPtr[Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLength]);
          *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr =
              Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.StartOputLength - Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLength;

          (void)Crypto_xVIx_xAIx_AL_ENCRYPT_AES_CBC_Finish
                (
                  Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.ObjId,
                  Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job
                );
        }
      }
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Crypto_Generic_Callback
      (
        Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job,
        Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackResult
      );
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackFlag = FALSE;
    }
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7EncryptCallback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
)
{
  TS_PARAM_UNUSED(Job);

  switch(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState)
  {
    case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_ST:
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState =
      CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_INITIALIZED;
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackResult = Result;
    break;
    case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_UPD:
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState =
        CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_TO_OUTP;
    break;

    case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_F_WAIT_SB_UPD:
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState =
        CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_PAD;
    break;

    case CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_FIN:
    {
      if
      (
        (
          CRYPTO_PROCESSING_ASYNC ==
              Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job->jobPrimitiveInfo->processingType
        ) &&
        (
          (
            (
              ((uint8)Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job->jobPrimitiveInputOutput.mode) &
              ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
            ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          )
        )
      )
      {
      /*Set the actual length of the output buffer.*/
      *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLengthPtr += Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLength;
      }
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_UNINIT;
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackResult = Result;
    }
    break;
    /* CHECK: NOPARSE */
    /*All the states (wait states) that the state machine can have at the time of this callback
       are addressed in the cases above.*/
    /* Caution: this 'default' is required by MISRA-C:2012 */
    default:
    {
    }
    break;
    /* CHECK: PARSE */
  }
}
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStateReqSbSt
(
  void
)
{
  Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_ST;

  switch (
           Crypto_xVIx_xAIx_AES_Helper_Encrypt_Start
           (
             Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.ObjId,
             &Crypto_xVIx_xAIx_CbcPkcs7EncryptCallback,
             Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Job,
             Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsSync,
             &Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.KeyPtr
           )
         )
  {
    case E_OK:
    {
    }
    break;

    default:
    {
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_UNINIT;
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackResult = E_NOT_OK;
    }
    break;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStateCpFromInp
(
  void
)
{
  Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CurrentMode = CRYPTO_OPERATIONMODE_UPDATE;
  if (NULL_PTR != Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsBusy)
  {
    *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsBusy = TRUE;
  }
  if(
      (
        Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.BlockFilled + Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IputTextLength
      ) >= sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block)
    )
  {
    uint32 newLen;

    newLen = sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block) -
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.BlockFilled;

    TS_MemCpy
    (
      &Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block[Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.BlockFilled],
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IputTextPtr, newLen
    );

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IputTextPtr = &Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IputTextPtr[newLen];

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IputTextLength -= newLen;

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.BlockFilled = sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block);

    /* !LINKSTO EB_Crypto_CryptAlgos_0204,1 */
    Crypto_xVIx_xAIx_CommonXOR
    (
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block,
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText,
      sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block)
    );

    /* Context state administration */
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_UPD;
  }
  else
  {
    TS_MemCpy
    (
      &Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block[Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.BlockFilled],
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IputTextPtr,
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IputTextLength
    );

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.BlockFilled += Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IputTextLength;

    *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLengthPtr = Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLength;

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_INITIALIZED;

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackFlag = TRUE;
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackResult = E_OK;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStateReqSbUpd
(
  void
)
{
  Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_UPD;
  Crypto_xVIx_xAIx_AES_Helper_Encrypt_Update
  (
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsSync,
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block,
    sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block),
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText,
    &(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.BlockFilled)
  );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStateCpToOutp
(
  void
)
{
  if(
      (
        *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLengthPtr -
        Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLength
      ) >= sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText)
    )
  {
    TS_MemCpy
    (
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextPtr,
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText,
      sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText)
    );

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLength +=
    sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText);

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextPtr =
    &Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextPtr[
                                                sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText)
                                              ];

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.BlockFilled = 0;

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_CP_FROM_INP;
  }
  else
  {
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_UNINIT;
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackFlag = TRUE;
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackResult = CRYPTO_E_SMALL_BUFFER;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStatePadLastBl
(
  void
)
{
  uint32 Counter;
  if (NULL_PTR != Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsBusy)
  {
    *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsBusy = TRUE;
  }
  /* !LINKSTO EB_Crypto_CryptAlgos_0202, 2 */
  for(
       Counter = 0; Counter < (
                                sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block) -
                                Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.BlockFilled
                              ); Counter++
     )
  {
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block[Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.BlockFilled + Counter ] =
         (uint8)(
                  sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block) -
                  Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.BlockFilled
                );
  }

  Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.BlockFilled = sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block);

  Crypto_xVIx_xAIx_CommonXOR
  (
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block,
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText,
    sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block)
  );

  Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_F_REQ_SB_UPD;
  /* set current mode to finish to call Callback correctly */
  Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CurrentMode = CRYPTO_OPERATIONMODE_FINISH;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStateFReqSbUpd
(
  void
)
{
  Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_F_WAIT_SB_UPD;
  Crypto_xVIx_xAIx_AES_Helper_Encrypt_Update
  (
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsSync,
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block,
    sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.Block),
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText,
    &(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.BlockFilled)
  );

}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStateCpPad
(
  void
)
{
  if(
      *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLengthPtr
      >= sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText)
    )
  {
    TS_MemCpy
    (
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextPtr,
      Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText,
      sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText)
    );

    /* !LINKSTO EB_Crypto_01030, 1 */
    *Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.OputTextLengthPtr =
     sizeof(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.LastCipherText);

    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_REQ_SB_FIN;
  }
  else
  {
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_UNINIT;
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackFlag = TRUE;
    Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CallbackResult = CRYPTO_E_SMALL_BUFFER;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7EncStateReqSbFin
(
  void
)
{
  Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_S_WAIT_SB_FIN;
  Crypto_xVIx_xAIx_AES_Helper_Encrypt_Finish(Crypto_xVIx_xAIx_CbcPkcs7EncryptContext.IsSync);
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_ENC_ENABLED == STD_ON) */

/*--AES-CBC-DECRYPT------------------------------------------------------------------------------*/

#if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON)

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Reset
(
  void
)
{
  Crypto_xVIx_xAIx_AL_DECRYPT_AES_ECB_Reset();
  SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
  TS_MemBZero(
               &Crypto_xVIx_xAIx_CbcPkcs7DecryptContext,
               sizeof(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext)
             );
  SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_CallBack
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  uint8                                                        Result
)
{
  if(CRYPTO_PROCESSING_ASYNC == Job->jobPrimitiveInfo->processingType)
  {
    if(
        (
          CRYPTO_OPERATIONMODE_FINISH ==
          (
            CRYPTO_OPERATIONMODE_FINISH & Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CurrentMode
          )
        ) || (E_OK != Result)
      )
    {
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CurrentMode = CRYPTO_XVIX_XAIX_CIPHER_DEFAULT_CURRENT_MODE;
      /* !LINKSTO EB_Crypto_01050, 1 */
      SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.ObjId].DriverObjectState =
          CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.ObjId].CurrentJob = NULL_PTR;
      Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.ObjId].SkipPeriodicMainFunction = TRUE;
      SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      /* !LINKSTO Crypto.Dsn.JobTransition.Finish2Idle, 1 */
      /* !LINKSTO Crypto.Dsn.JobTransition.ActiveError2Idle, 1 */
      Job->jobState = CRYPTO_JOBSTATE_IDLE;
      CryIf_CallbackNotification(Job, Result);
      Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Reset();
    }
    if(
        (
          CRYPTO_OPERATIONMODE_UPDATE ==
          (CRYPTO_OPERATIONMODE_UPDATE & Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CurrentMode)
        ) &&
        (
          (
            ((uint8)Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job->jobPrimitiveInputOutput.mode) &
            ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          ) !=
          (
            (uint8) CRYPTO_OPERATIONMODE_SINGLECALL
          )
        )
      )
    {
      CryIf_CallbackNotification(Job, Result);
    }
  }
}

/* !LINKSTO EB_Crypto_CryptAlgos_0961, 1 */
static FUNC(Std_ReturnType,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7DecryptStart
(
  P2CONST(void, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                        CfgPtr,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) KeyPtr,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                Job,
  P2CONST(Crypto_xVIx_xAIx_SymKeyType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) IvPtr
)
{
  P2CONST(Crypto_xVIx_xAIx_CbcPkcs7DecryptConfigType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) LocCfgPtr;
  Std_ReturnType LocRetVal;

  LocCfgPtr = (P2CONST(Crypto_xVIx_xAIx_CbcPkcs7DecryptConfigType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)) CfgPtr;

  LocRetVal = E_NOT_OK;

  if(IvPtr->length == sizeof(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.LastCipherText))
  {
    LocRetVal = E_OK;
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_ST;
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CurrentMode = CRYPTO_OPERATIONMODE_START;
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job = Job;

    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.ObjId = LocCfgPtr->ObjId;

    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Crypto_Generic_Callback = LocCfgPtr->Crypto_Generic_Callback;

    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CfgId = LocCfgPtr->SymBlockDecryptCfgId;

    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.KeyPtr = *KeyPtr;

    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockFilled = 0U;

    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLength = 0U;

    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockToWrite = FALSE;

    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IsSync = LocCfgPtr->ProcessingType;

    /* !LINKSTO EB_Crypto_CryptAlgos_0207,1 */
    /* copy Iv to lastCipherText to use in first block */
    TS_MemCpy
    (
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.LastCipherText,
      IvPtr->data,
      IvPtr->length
    );
  }

  return LocRetVal;
}

static FUNC(Std_ReturnType,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7DecryptUpdate
(
  P2CONST(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) IputTextPtr,
  uint32                                                IputTextLength,
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)   OputTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  OputTextLengthPtr
)
{
  Std_ReturnType LocRetVal;
  LocRetVal = E_OK;

  if ( Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState == CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_INITIALIZED )
  {
    /* Context state administration */
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_READ_WRITE;
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLength = 0;

    /* copy input arguments into Context variables */
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IputTextPtr = IputTextPtr;
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IputTextLength = IputTextLength;
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextPtr = OputTextPtr;
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLengthPtr = OputTextLengthPtr;
  }
  else
  {
    LocRetVal = CRYPTO_E_BUSY;
  }

  return LocRetVal;
}

static FUNC(Std_ReturnType,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7DecryptFinish
(
  P2VAR(uint8, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)  OputTextPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) OputTextLengthPtr
)
{
  Std_ReturnType LocRetVal;
  LocRetVal = E_OK;

  if ( Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState == CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_INITIALIZED )
  {
    /* Context state administration */
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_CP_LAST_BL;

    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextPtr = OputTextPtr;
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLengthPtr = OputTextLengthPtr;
  }
  else
  {
    LocRetVal = CRYPTO_E_BUSY;
  }

  return LocRetVal;
}

static FUNC(void,CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7DecryptMainFunction
(
  P2VAR(Std_ReturnType,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Result,
  P2VAR(boolean,AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)        Busy
)
{
  if(!Crypto_xVIx_xAIx_CheckSkipMain(
                                      Result,
                                      Busy,
                                      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.ObjId
                                    )
    )
  {
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IsBusy = Busy;
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Result = Result;
    if((NULL_PTR != Result) && (NULL_PTR != Busy))
    {
      *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IsBusy = TRUE;
      *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Result = E_OK;
    }
    switch ( Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState )
    {
      case CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_UNINIT:
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_ST:
      {
        Crypto_xVIx_xAIx_CBCPkcs7DecStateReqSbSt();
      }
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_INITIALIZED:
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_READ_WRITE:
      {
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CurrentMode = CRYPTO_OPERATIONMODE_UPDATE;
        Crypto_xVIx_xAIx_CBCPkcs7DecStateReadWrite();
      }
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_UPD:
      {
        Crypto_xVIx_xAIx_CBCPkcs7DecStateReqSbUpd();
      }
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_XOR:
      {
        Crypto_xVIx_xAIx_CBCPkcs7DecStateXOR();
      }
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_CP_LAST_BL:
      {
        /* Set current mode to call Callback correctly */
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CurrentMode = CRYPTO_OPERATIONMODE_FINISH;
        Crypto_xVIx_xAIx_CBCPkcs7DecStateCpLastBl();
      }
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_FIN:
      {
        Crypto_xVIx_xAIx_CBCPkcs7DecStateReqSbFin();
      }
      break;

      case CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_ST:
      case CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_UPD:
      case CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_FIN:
      {
        /* !LINKSTO EB_Crypto_CryptAlgos_0203,1 */
        Crypto_xVIx_xAIx_AES_Helper_Decrypt_MainFunction();
      }
      break;

      /* Crypto_xVIx_xAIx_CbcPkcs7DecryptMainFunction > DEFAULT */
      /* CHECK: NOPARSE */
      /*All the states that the state machine can have are already addressed in the cases above.*/
      /* Defensive Programming - unreachable code. This default case is required by MISRA-C:2012 */
      default:
      {}
      break;
      /* CHECK: PARSE */
    }
    /* !LINKSTO SWS_Crypto_00143_CORRECTION_SWS_Crypto_00181, 1, SWS_Crypto_00144_CORRECTION, 1 */
    if(TRUE == Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CancelFlag)
    {
      if (
           (NULL_PTR != Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Result)
           && (NULL_PTR != Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IsBusy)
         )
      {
        *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IsBusy = FALSE;
        *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Result = CRYPTO_E_JOB_CANCELED;
        /* processingType is synchronous */
        /* !LINKSTO Crypto.Dsn.JobTransition.ActiveCancel2Idle, 1 */
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job->jobState = CRYPTO_JOBSTATE_IDLE;
        SchM_Enter_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.ObjId].DriverObjectState =
        CRYPTO_XVIX_XAIX_DRIVER_OBJECT_STATE_IDLE;
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.ObjId].CurrentJob =
        NULL_PTR;
        Crypto_xVIx_xAIx_DriverObjects[Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.ObjId].SkipPeriodicMainFunction = TRUE;
        SchM_Exit_Crypto_xVIx_xAIx_SCHM_CRYPTO_XVIX_XAIX_EXCLUSIVE_AREA_DRIVEROBJECT();
      }
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Crypto_Generic_Callback
      (
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job,
        CRYPTO_E_JOB_CANCELED
      );
    }
    else
    {
      if(TRUE == Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackFlag)
      {
        if (
             (NULL_PTR != Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Result)
             && (NULL_PTR != Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IsBusy)
           )
        {
          *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IsBusy = FALSE;
          *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Result =
          Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackResult;
        }
        if(
            (
              CRYPTO_PROCESSING_ASYNC ==
              Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job->jobPrimitiveInfo->processingType
            ) &&
            (
              (
                (
                  ((uint8)Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job->jobPrimitiveInputOutput.mode) &
                    ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
              ) ||
              (
                (
                  ((uint8)Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job->jobPrimitiveInputOutput.mode) &
                  ((uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
                ) == ( (uint8) CRYPTO_OPERATIONMODE_STREAMSTART)
              )
            ) && (CRYPTO_OPERATIONMODE_START == Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CurrentMode)
          )
        {
          Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.StartOputLength =
              *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr;
          (void)Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Update
                (
                  Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.ObjId,
                  Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job
                );
        }
        else
        {
          if
          (
            (
              CRYPTO_PROCESSING_ASYNC ==
                  Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job->jobPrimitiveInfo->processingType
            ) &&
            (
              (
                (
                  ((uint8)Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job->jobPrimitiveInputOutput.mode) &
                  ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
                ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
              )
            ) && (CRYPTO_OPERATIONMODE_UPDATE == Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CurrentMode)
          )
          {
             Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job->jobPrimitiveInputOutput.outputPtr =
             &(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job->jobPrimitiveInputOutput.outputPtr[Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLength]);
             *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr =
                 Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.StartOputLength -
                 *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job->jobPrimitiveInputOutput.outputLengthPtr;

            (void)Crypto_xVIx_xAIx_AL_DECRYPT_AES_CBC_Finish
                  (
                    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.ObjId,
                    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job
                  );
          }
        }
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Crypto_Generic_Callback
        (
          Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job,
          Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackResult
        );
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackFlag = FALSE;
      }
    }
  }
}
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CbcPkcs7DecryptCallback
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType Result
)
{
  TS_PARAM_UNUSED(Job);
  switch(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState)
  {
    case CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_ST:
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState =
          CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_INITIALIZED;
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackResult = Result;
    break;
    case CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_UPD:
    {
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState =
        CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_XOR;
    }
    break;

    case CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_FIN:
    {
      if
      (
        (
          CRYPTO_PROCESSING_ASYNC ==
              Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job->jobPrimitiveInfo->processingType
        ) &&
        (
          (
            (
              ((uint8)Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job->jobPrimitiveInputOutput.mode) &
              ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
            ) == ((uint8) CRYPTO_OPERATIONMODE_SINGLECALL)
          )
        )
      )
      {
        /*Set the actual length of the output buffer.*/
        *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLengthPtr += Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLength;
      }
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_UNINIT;
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackResult = Result;
    }
    break;

    /* CHECK: NOPARSE */
    /*All the states that the state machine can have at the time when callback is called are already
      cover in the cases above.*/
    default:
    {
      /* Caution: this 'default' is required by MISRA-C:2012 */
    }
    break;
    /* CHECK: PARSE */
  }
}
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7DecStateReqSbSt
(
  void
)
{
  Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_ST;

  switch (
           Crypto_xVIx_xAIx_AES_Helper_Decrypt_Start
           (
             Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.ObjId,
             &Crypto_xVIx_xAIx_CbcPkcs7DecryptCallback, Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Job,
             Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IsSync,
             &Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.KeyPtr
           )
         )
  {
    case E_OK:
    {
    }
    break;

    default:
    {
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_UNINIT;
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackResult = E_NOT_OK;
    }
    break;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7DecStateReadWrite
(
  void
)
{
  if (NULL_PTR != Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IsBusy)
  {
    *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IsBusy = TRUE;
  }
  if (Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IputTextLength == 0U)
  {
    *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLengthPtr =
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLength;

    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_INITIALIZED;

    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackFlag = TRUE;
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackResult = E_OK;
  }
  else
  {
    Std_ReturnType writeResult;

    writeResult = E_OK;

    if (TRUE == Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockToWrite)
    {
      if(
          (*Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLengthPtr -
              Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLength
          ) >= sizeof(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.LastCipherText)
        )
      {
        TS_MemCpy
        (
          &Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextPtr[Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLength],
          Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.PlainText,
          sizeof(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.PlainText)
        );

        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLength +=
            sizeof(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.LastCipherText);

        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockFilled = 0U;

        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockToWrite = FALSE;
      }
      else
      {
        writeResult = CRYPTO_E_SMALL_BUFFER;
      }
    }

    if (E_OK != writeResult)
    {
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_UNINIT;

      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackFlag = TRUE;
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackResult = writeResult;
    }
    else
    {
      if(
          (Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockFilled + Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IputTextLength)
          >= sizeof(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Block)
        )
      {
        uint32 newLen;

        newLen = (
                   sizeof(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Block) -
                   Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockFilled
                 );

        TS_MemCpy
        (
          &Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Block[Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockFilled],
          Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IputTextPtr,
          newLen
        );

        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IputTextPtr =
        &Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IputTextPtr[newLen];

        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IputTextLength -= newLen;

        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockFilled =
        sizeof(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Block);

        /* Context state administration */
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_UPD;
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.UpdateExecFlag = TRUE;
      }
      else
      {
        TS_MemCpy
        (
          &Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Block[Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockFilled],
          Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IputTextPtr,
          Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IputTextLength
        );

        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockFilled +=
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IputTextLength;

        *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLengthPtr =
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLength;

        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState =
        CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_INITIALIZED;
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackFlag = TRUE;
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackResult = E_OK;
      }
    }
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7DecStateReqSbUpd
(
  void
)
{
  Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_UPD;
  Crypto_xVIx_xAIx_AES_Helper_Decrypt_Update
  (
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IsSync,
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Block,
    sizeof(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Block),
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.PlainText,
    &(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockFilled)
  );
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7DecStateXOR
(
  void
)
{
  Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockFilled = 0U;

  /* !LINKSTO EB_Crypto_CryptAlgos_0206,1 */
  Crypto_xVIx_xAIx_CommonXOR
  (
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.PlainText,
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.LastCipherText,
    sizeof(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.PlainText)
  );

  TS_MemCpy
  (
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.LastCipherText,
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Block,
    sizeof(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.Block)
  );

  Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockToWrite = TRUE;

  Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_READ_WRITE;
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7DecStateCpLastBl
(
  void
)
{
  uint8 Padding;
  uint8 LastPlainTextLength;
  if (NULL_PTR != Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IsBusy)
  {
    *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IsBusy = TRUE;
  }
  Padding =
  Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.PlainText[
                                           sizeof(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.PlainText) - 1U
                                         ];

  LastPlainTextLength = (uint8) (sizeof(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.PlainText) - Padding);

  if(
      (TRUE != Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.UpdateExecFlag) ||
      (Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.BlockFilled != 0) ||
      (Padding > sizeof(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.PlainText))
    )
  {
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_UNINIT;
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackFlag = TRUE;
    Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackResult = E_NOT_OK;
  }
  else
  {
    if(*Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLengthPtr < LastPlainTextLength)
    {
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_UNINIT;
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackFlag = TRUE;
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CallbackResult = CRYPTO_E_SMALL_BUFFER;
    }
    else
    {
      TS_MemCpy
      (
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextPtr,
        Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.PlainText,
        LastPlainTextLength
      );

      /* !LINKSTO EB_Crypto_01030, 1 */
      *Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.OputTextLengthPtr = LastPlainTextLength;

      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_REQ_SB_FIN;
      Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.UpdateExecFlag = FALSE;
    }
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_CBCPkcs7DecStateReqSbFin
(
  void
)
{
  Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.CtxState = CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_S_WAIT_SB_FIN;
  Crypto_xVIx_xAIx_AES_Helper_Decrypt_Finish(Crypto_xVIx_xAIx_CbcPkcs7DecryptContext.IsSync);
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#endif /* #if (CRYPTO_XVIX_XAIX_CBC_PKCS7_DEC_ENABLED == STD_ON) */

/*--AES-ECB-COMMON-------------------------------------------------------------------------------*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESCommonCalculateKeySchedule
(
  P2VAR(Crypto_xVIx_xAIx_AES_ContextType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Context
)
{
  uint8 Index;
  uint32 temp;
  CONST(uint32, CRYPTO_xVIx_xAIx_CONST) Crypto_xVIx_xAIx_AES_Rcon_Fast[CRYPTO_XVIX_XAIX_AES_RCONFASTLEN] =
  {
    0x01000000U, 0x02000000U, 0x04000000U, 0x08000000U,
    0x10000000U, 0x20000000U, 0x40000000U, 0x80000000U,
    0x1B000000U, 0x36000000U
  };
  /* Copy the first key elements */
  for (Index = 0U; Index < Context->SzeKey; Index++)
  {
    Context->KeyExpdFast[Index] =
        CRYPTO_XVIX_XAIX_AES_COMMON_BYTEARRAY_TO_UINT32(&(Context->KeySeed[Index * 4U]));
  }

  /* Calculate the rest of the subkeys */
  for (Index = Context->SzeKey;
      Index < (4U * ((Context->SzeTurn) + 1U));
      Index++
  )
  {
    temp = Context->KeyExpdFast[Index - 1U];

    if ((Index % Context->SzeKey) == 0U)
    {
      /* Leftmost word (includes the g() function) */
      temp = (
               ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp >> 16U) & 0xFFU] << 24U) ^
               ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp >> 8U) & 0xFFU] << 16U) ^
               ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp ) & 0xFFU] << 8U) ^
               ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp >> 24U) & 0xFFU] )
             ) ^ Crypto_xVIx_xAIx_AES_Rcon_Fast[(Index / (Context->SzeKey)) - 1U];
    }
    else if ((Context->SzeKey > 6U) &&
        ((Index % (Context->SzeKey)) == 4U))
    {
      /* Fifth word (includes the f() function), only used for 256 bit keys */
      temp = ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp >> 24U) & 0xFFU] << 24U) ^
             ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp >> 16U) & 0xFFU] << 16U) ^
             ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp >> 8U) & 0xFFU] << 8U) ^
             ((uint32)Crypto_xVIx_xAIx_AES_SboxNrm[(temp ) & 0xFFU] );
    }
    else
    {
      /* Nothing to do (MISRA) */
    }

    Context->KeyExpdFast[Index] = Context->KeyExpdFast[Index - Context->SzeKey] ^ temp;
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESCommonMapState
(
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                   FirstCol,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                   SecondCol,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                   ThirdCol,
  P2VAR(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)                   FourthCol,
  P2CONST(Crypto_xVIx_xAIx_AES_ContextType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Context
)
{
  *FirstCol = CRYPTO_XVIX_XAIX_AES_COMMON_BYTEARRAY_TO_UINT32(&(Context->AesState[ 0U]));
  *SecondCol = CRYPTO_XVIX_XAIX_AES_COMMON_BYTEARRAY_TO_UINT32(&(Context->AesState[ 4U]));
  *ThirdCol = CRYPTO_XVIX_XAIX_AES_COMMON_BYTEARRAY_TO_UINT32(&(Context->AesState[ 8U]));
  *FourthCol = CRYPTO_XVIX_XAIX_AES_COMMON_BYTEARRAY_TO_UINT32(&(Context->AesState[12U]));
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESCommonUnmapState
(
  P2CONST(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)               FirstCol,
  P2CONST(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)               SecondCol,
  P2CONST(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)               ThirdCol,
  P2CONST(uint32, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA)               FourthCol,
  P2VAR(Crypto_xVIx_xAIx_AES_ContextType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Context
)
{
  CRYPTO_XVIX_XAIX_AES_COMMON_UINT32_TO_BYTEARRAY(&(Context->AesState[ 0U]), *FirstCol);
  CRYPTO_XVIX_XAIX_AES_COMMON_UINT32_TO_BYTEARRAY(&(Context->AesState[ 4U]), *SecondCol);
  CRYPTO_XVIX_XAIX_AES_COMMON_UINT32_TO_BYTEARRAY(&(Context->AesState[ 8U]), *ThirdCol);
  CRYPTO_XVIX_XAIX_AES_COMMON_UINT32_TO_BYTEARRAY(&(Context->AesState[12U]), *FourthCol);
}

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

/*--AES-HELPER------------------------------------------------------------------------------*/

#define CRYPTO_xVIx_xAIx_START_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#if (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)
static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESHelperDecryptSynchronousMainCall(void)
{
  Crypto_xVIx_xAIx_HelperDecryptFlgCallbackNotification = FALSE;

  while(Crypto_xVIx_xAIx_HelperDecryptFlgCallbackNotification == FALSE)
  {
    Crypto_xVIx_xAIx_AESDecryptMainFunction(&Crypto_xVIx_xAIx_HelperDecryptResult, &Crypto_xVIx_xAIx_HelperDecryptBusy);
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AESHelperDecryptCallbackNotification
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
)
{
  Crypto_xVIx_xAIx_HelperDecryptFlgCallbackNotification = TRUE;
  Crypto_xVIx_xAIx_ModeDecryptCallBack(Job,Result);
}
#endif /*(CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)*/
#if(CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON)
static FUNC(void, CRYPTO_xVIx_xAIx_CODE)Crypto_xVIx_xAIx_AESHelperEncryptSynchronousMainCall(void)
{
  Crypto_xVIx_xAIx_HelperEncryptFlgCallbackNotification = FALSE;
  while(Crypto_xVIx_xAIx_HelperEncryptFlgCallbackNotification == FALSE)
  {
    Crypto_xVIx_xAIx_AESEncryptMainFunction(&Crypto_xVIx_xAIx_HelperEncryptResult, &Crypto_xVIx_xAIx_HelperEncryptBusy);
  }
}

static FUNC(void, CRYPTO_xVIx_xAIx_CODE) Crypto_xVIx_xAIx_AES_Helper_Encrypt_CallbackNotification
(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_xVIx_xAIx_APPL_DATA) Job,
  Std_ReturnType                                               Result
)
{
  Crypto_xVIx_xAIx_HelperEncryptFlgCallbackNotification = TRUE;
  Crypto_xVIx_xAIx_ModeEncryptCallBack(Job, Result);
}

#endif /* #if(CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_CODE
#include <Crypto_xVIx_xAIx_MemMap.h>

#else /*#if((CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)) */
typedef uint8 Crypto_xVIx_xAIx_AES_PrvtEmptyTranslationUnitType;
#endif /* #if((CRYPTO_XVIX_XAIX_AES_ENCRYPT_ENABLED == STD_ON) || (CRYPTO_XVIX_XAIX_AES_DECRYPT_ENABLED == STD_ON)) */

/*==[end of file]================================================================================*/

