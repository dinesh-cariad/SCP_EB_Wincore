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

[!VAR "NvmNeeded" = "'false'"!][!//
[!LOOP "node:order(CryptoKeys/CryptoKey/*)"!][!//
  [!IF "num:i(0) != num:i(count(CryptoKeyNvRamBlockIds/*))"!][!//
    [!VAR "NvmNeeded" = "'true'"!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!/* ------------------------------------------------------------------ */!][!//
[!VAR "exclusiveAreaQueue" = "'false'"!][!//
[!LOOP "node:order(CryptoDriverObjects/CryptoDriverObject/*,'node:value(CryptoDriverObjectId)')"!][!//
 [!IF "num:i(0) != num:i(node:value(CryptoQueueSize))"!][!//
   [!VAR "exclusiveAreaQueue" = "'true'"!][!//
 [!ENDIF!][!//
[!ENDLOOP!][!//
[!/* ------------------------------------------------------------------ */!][!//
#ifndef CRYPTO_XVIX_XAIX_CFG_H
#define CRYPTO_XVIX_XAIX_CFG_H

/* !LINKSTO SWS_Crypto_00008, 1 */

/*==================[misra deviations]============================================================*/

/*==================[inclusions]==================================================================*/

#include <Csm_Types.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>





/*==================[macros]======================================================================*/

#if (defined CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT)
#error CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT is already defined
#endif
/** \brief Macro indicating whether the development error detection shall be enabled */
[!IF "(CryptoGeneral/CryptoDevErrorDetect = 'true')"!]
#define CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT  STD_ON
[!ELSE!]
#define CRYPTO_XVIX_XAIX_DEV_ERROR_DETECT  STD_OFF
[!ENDIF!]

#if (defined CRYPTO_XVIX_XAIX_VERSION_INFO_API)
#error CRYPTO_XVIX_XAIX_VERSION_INFO_API is already defined
#endif
/** \brief Macro indicating whether the version info API shall be enabled */
[!IF "(CryptoGeneral/CryptoVersionInfoApi = 'true')"!]
#define CRYPTO_XVIX_XAIX_VERSION_INFO_API  STD_ON
[!ELSE!]
#define CRYPTO_XVIX_XAIX_VERSION_INFO_API  STD_OFF
[!ENDIF!]

#if (defined CRYPTO_XVIX_XAIX_CRYPTO_MAIN_FUNCTION_EXISTS)
#error CRYPTO_XVIX_XAIX_CRYPTO_MAIN_FUNCTION_EXISTS is already defined
#endif
/** \brief Macro indicating if the MainFunction shall be scheduled */
[!IF "node:exists(CryptoGeneral/CryptoMainFunctionPeriod)"!]
#define CRYPTO_XVIX_XAIX_CRYPTO_MAIN_FUNCTION_EXISTS STD_ON
[!ELSE!]
#define CRYPTO_XVIX_XAIX_CRYPTO_MAIN_FUNCTION_EXISTS STD_OFF
[!ENDIF!]

#if (defined CRYPTO_XVIX_XAIX_INSTANCE_ID)
#error CRYPTO_XVIX_XAIX_INSTANCE_ID is already defined
#endif
/** \brief Macro indicating the Instance ID of the crypto driver. */
#define CRYPTO_XVIX_XAIX_INSTANCE_ID [!"num:i(node:value(CryptoGeneral/CryptoInstanceId))"!]U

#if (defined CRYPTO_XVIX_XAIX_DRIVER_OBJECT_COUNT)
#error CRYPTO_XVIX_XAIX_DRIVER_OBJECT_COUNT is already defined
#endif
/** \brief Number of configured crypto driver objects */
#define CRYPTO_XVIX_XAIX_DRIVER_OBJECT_COUNT [!"num:i(count(CryptoDriverObjects/CryptoDriverObject/*))"!]U

[!LOOP "node:order(CryptoDriverObjects/CryptoDriverObject/*,'node:value(CryptoDriverObjectId)')"!]
#if (defined Crypto_xVIx_xAIxConf_CryptoDriverObject_[!"node:name(.)"!])
#error Crypto_xVIx_xAIxConf_CryptoDriverObject_[!"node:name(.)"!] is already defined
#endif
/** \brief Identifier of crypto driver object [!"node:name(.)"!] */
#define Crypto_xVIx_xAIxConf_CryptoDriverObject_[!"node:name(.)"!] [!"node:value(CryptoDriverObjectId)"!]U

#if (defined CRYPTO_XVIX_XAIX_DO_[!"text:toupper(node:name(.))"!]_QUEUE_SIZE)
#error CRYPTO_XVIX_XAIX_DO_[!"text:toupper(node:name(.))"!]_QUEUE_SIZE is already defined
#endif
/** \brief Queue size of crypto driver object [!"text:toupper(node:name(.))"!] */
#define CRYPTO_XVIX_XAIX_DO_[!"text:toupper(node:name(.))"!]_QUEUE_SIZE [!"node:value(CryptoQueueSize)"!]U

[!ENDLOOP!]

/** \brief EXCLUSIVE AREA QUEUE enabled/disabled
 */
[!IF "$exclusiveAreaQueue = 'true'"!][!//
#define  CRYPTO_XVIX_XAIX_QUEUE  STD_ON
[!ELSE!][!//
#define  CRYPTO_XVIX_XAIX_QUEUE  STD_OFF
[!ENDIF!][!//
/** \brief Number of configured crypto keys
 */
#if (defined CRYPTO_XVIX_XAIX_KEY_COUNT)
#error CRYPTO_XVIX_XAIX_KEY_COUNT is already defined
#endif
/** \brief Number of configured crypto keys */
#define CRYPTO_XVIX_XAIX_KEY_COUNT [!"num:i(count(CryptoKeys/CryptoKey/*))"!]U

#if (defined CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT)
#error CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT is already defined
#endif
/** \brief  Key management APIs enabled/disabled infos */
[!IF "num:i(count(CryptoKeys/CryptoKey/*)) > 0"!]
#define CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT STD_ON
[!ELSE!]
#define CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT STD_OFF
[!ENDIF!]

#if (defined CRYPTO_XVIX_XAIX_ENABLED_NVM)
#error CRYPTO_XVIX_XAIX_ENABLED_NVM is already defined
#endif
/** \brief NvM module dependency enabled/disabled */

[!IF "$NvmNeeded = 'true'"!][!//
#define CRYPTO_XVIX_XAIX_ENABLED_NVM  STD_ON
[!ELSE!][!//
#define CRYPTO_XVIX_XAIX_ENABLED_NVM  STD_OFF
[!ENDIF!][!//

[!LOOP "node:order(CryptoKeys/CryptoKey/*, 'node:value(CryptoKeyId)')"!]
#if (defined Crypto_xVIx_xAIxConf_CryptoKey_[!"node:name(.)"!])
#error Crypto_xVIx_xAIxConf_CryptoKey_[!"node:name(.)"!] is already defined
#endif
/** \brief Identifier of crypto key [!"node:name(.)"!] */
#define Crypto_xVIx_xAIxConf_CryptoKey_[!"node:name(.)"!] [!"node:value(CryptoKeyId)"!]U

#if (defined CRYPTO_XVIX_XAIX_KEY_[!"text:toupper(node:name(.))"!]_DERIVE_ITERATIONS)
#error CRYPTO_XVIX_XAIX_KEY_[!"text:toupper(node:name(.))"!]_DERIVE_ITERATIONS is already defined
#endif
/** \brief Derive iterations for crypto key [!"text:toupper(node:name(.))"!] */
#define CRYPTO_XVIX_XAIX_KEY_[!"text:toupper(node:name(.))"!]_DERIVE_ITERATIONS [!"node:value(CryptoKeyDeriveIterations)"!]U

[!ENDLOOP!]

#if (defined CRYPTO_XVIX_XAIX_KEY_ELEMENT_COUNT)
#error CRYPTO_XVIX_XAIX_KEY_ELEMENT_COUNT is already defined
#endif
/** \brief Number of configured crypto key elements */
#define CRYPTO_XVIX_XAIX_KEY_ELEMENT_COUNT [!"num:i(count(CryptoKeyElements/CryptoKeyElement/*))"!]U

[!LOOP "node:order(CryptoKeyElements/CryptoKeyElement/*, 'node:value(CryptoKeyElementId)')"!]

#if (defined Crypto_xVIx_xAIxConf_CryptoKeyElement_[!"node:name(.)"!])
#error Crypto_xVIx_xAIxConf_CryptoKeyElement_[!"node:name(.)"!] is already defined
#endif
/** \brief Identifier of crypto key element [!"node:name(.)"!] */
#define Crypto_xVIx_xAIxConf_CryptoKeyElement_[!"node:name(.)"!] [!"node:value(CryptoKeyElementId)"!]U
[!ENDLOOP!][!//

[!VAR "i" = "0"!]
[!VAR "MaxSize" = "0"!]
[!LOOP "CryptoKeyElements/CryptoKeyElement/*"!]
#if (defined CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_INDEX)
#error CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_INDEX is already defined
#endif
/** \brief Index of crypto key element [!"text:toupper(node:name(.))"!] */
#define CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_INDEX [!"num:i($i)"!]U

#if (defined CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_ALLOW_PARTIAL_ACCESS)
#error CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_ALLOW_PARTIAL_ACCESS is already defined
#endif
/** \brief Macro indicating whether partial access is allowed for a certain key element */
#define CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_ALLOW_PARTIAL_ACCESS [!"text:toupper(node:value(CryptoKeyElementAllowPartialAccess))"!]

/* !LINKSTO EB_Crypto_01291,1 */
#if (defined CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_INIT_VALUE)
#error CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_INIT_VALUE is already defined
#endif
/** \brief Macro indicating the initial value of the key element */
  [!IF "node:empty(CryptoKeyElementInitValue)"!]
#define CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_INIT_VALUE { 0x00U }
  [!ELSE!]
    [!VAR "SplitLine" = "0"!]
#define CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_INIT_VALUE \
{ \
    [!LOOP "text:split(text:replaceAll(node:value(CryptoKeyElementInitValue), '\s+', ''), ',')"!]
      [!VAR "SplitLine" = "$SplitLine + 1"!]
      [!"."!]U, [!//
      [!IF "num:i($SplitLine) = 8"!]
  \
        [!VAR "SplitLine" = "0"!]
      [!ENDIF!]
    [!ENDLOOP!]
    [!IF "num:i($SplitLine) != 0"!]
  \
    [!ENDIF!]
}
  [!ENDIF!]

#if (defined CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_PERSIST)
#error CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_PERSIST is already defined
#endif
/** \brief Macro indicating whether the key element is persistent in non-volatile storage */
#define CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_PERSIST [!"text:toupper(node:value(CryptoKeyElementPersist))"!]

#if (defined CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_READ_ACCESS)
#error CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_READ_ACCESS is already defined
#endif
/** \brief Macro indicating the read access rights */
#define CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_READ_ACCESS [!"text:replace(node:value(CryptoKeyElementReadAccess), 'CRYPTO_', 'CRYPTO_XVIX_XAIX_')"!]

#if (defined CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_SIZE)
#error CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_SIZE is already defined
#endif
/** \brief Macro indicating the size of the key element */
#define CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_SIZE [!"node:value(CryptoKeyElementSize)"!]U
  [!IF "node:value(CryptoKeyElementPersist)"!]
    [!IF "num:i($MaxSize) < node:value(CryptoKeyElementSize)"!]
      [!VAR "MaxSize" = "node:value(CryptoKeyElementSize)"!]
    [!ENDIF!]
  [!ENDIF!]

#if (defined CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_WRITE_ACCESS)
#error CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_WRITE_ACCESS is already defined
#endif
/** \brief Macro indicating the write access rights */
#define CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_WRITE_ACCESS [!"text:replace(node:value(CryptoKeyElementWriteAccess), 'CRYPTO_', 'CRYPTO_XVIX_XAIX_')"!]
  [!VAR "i" = "$i + 1"!]
[!ENDLOOP!]

#if (defined CRYPTO_XVIX_XAIX_AL_MAX_KEY_PERS_ELEMENT_SIZE)
#error CRYPTO_XVIX_XAIX_AL_MAX_KEY_PERS_ELEMENT_SIZE is already defined
#endif
/** \brief Max size of key element data. */
#define CRYPTO_XVIX_XAIX_AL_MAX_KEY_PERS_ELEMENT_SIZE ([!"num:i($MaxSize)"!]U + 1U)

/* !LINKSTO Crypto.Req.Gen/NvM/01018,1 */
[!IF "$NvmNeeded = 'true'"!][!//
  [!LOOP "CryptoKeys/CryptoKey/*/CryptoKeyNvRamBlockIds/*"!]
    [!VAR "NvmBlock" = "node:value(as:ref(CryptoNvramBlockIdRef)/NvMRamBlockDataAddress)"!]
    [!VAR "KeyElem" = "node:name(as:ref(CryptoPersistKeyElement))"!]
    [!VAR "KeyPath" = "node:path(../..)"!]
    [!VAR "i" = "0"!]
    [!LOOP "../../../../../CryptoKeys/CryptoKey/*"!]
      [!VAR "k" = "0"!]
      [!VAR "KeyIds" = "node:path(.)"!]
      [!IF "$KeyPath = $KeyIds"!]
        [!LOOP "as:ref(node:value(CryptoKeyTypeRef))/CryptoKeyElementRef/*"!]
          [!VAR "Path" = "as:path(node:value(.))"!]
          [!VAR "RefElem" = "substring($Path,text:lastIndexOf(string($Path),'/')+2)"!]
          [!IF "$KeyElem = $RefElem"!]
            [!IF "substring($NvmBlock,1,1) = '&'"!]
#if (defined [!"substring($NvmBlock,2)"!])
#error [!"substring($NvmBlock,2)"!] is already defined
#endif
/** \brief Nv Ram mirror block [!"substring($NvmBlock,2)"!] data addresses. */
#define [!"substring($NvmBlock,2)"!] Crypto_xVIx_xAIx_AL_KeyElemData_[!"num:i($i)"!]_[!"num:i($k)"!]
            [!ELSE!]
              [!ERROR!]
 "The RamBlockDataAddress should always start with the address operator(&)."
              [!ENDERROR!]
            [!ENDIF!]
          [!ENDIF!]
          [!VAR "k" = "$k + 1"!]
        [!ENDLOOP!]
      [!ENDIF!]
      [!VAR "i" = "$i + 1"!]
    [!ENDLOOP!]
  [!ENDLOOP!]
  [!VAR "i" = "0"!]
  [!LOOP "node:order(CryptoKeys/CryptoKey/*, 'node:value(CryptoKeyId)')"!]
    [!VAR "k" = "0"!]
    [!SELECT "./CryptoKeyTypeRef"!]
      [!LOOP "node:ref(.)/CryptoKeyElementRef/*"!]
        [!SELECT "node:current()"!]
          [!VAR "KeyElem" = "node:name(as:ref(.))"!]
          [!VAR "j" = "$i + 1"!]
          [!VAR "NvramId" = "0"!]
          [!LOOP "../../../../../CryptoKeys/CryptoKey/*[num:i($j)]/CryptoKeyNvRamBlockIds/*"!]
            [!IF "node:name(as:ref(CryptoPersistKeyElement)) = string($KeyElem)"!]
              [!VAR "NvramId" = "node:value(as:ref(CryptoNvramBlockIdRef)/NvMNvramBlockIdentifier)"!]
              [!VAR "NvramIdName" = "text:toupper(node:name(as:ref(CryptoNvramBlockIdRef)))"!]
            [!ENDIF!]
          [!ENDLOOP!]
          [!IF "num:i($NvramId) > 0"!]
#if (defined [!"string($NvramIdName)"!]_ID)
#error [!"string($NvramIdName)"!]_ID is already defined
#endif
 /** \brief Nv Ram mirror [!"string($NvramIdName)"!] block ID. */
#define [!"string($NvramIdName)"!]_ID [!"num:i($NvramId)"!]U
          [!ENDIF!]
          [!VAR "k" = "$k + 1"!]
        [!ENDSELECT!]
      [!ENDLOOP!]
    [!ENDSELECT!]
    [!VAR "i" = "$i + 1"!]
  [!ENDLOOP!]
[!ENDIF!][!//

[!VAR "CRYPTO_XVIX_XAIX_RANDOM_RNG_CTRDRBG_NOT_SET_ENABLED"!]STD_OFF[!ENDVAR!]
[!LOOP "CryptoDriverObjects/CryptoDriverObject/*/CryptoPrimitiveRef/*"!][!//
[!//
   [!IF "($CRYPTO_XVIX_XAIX_RANDOM_RNG_CTRDRBG_NOT_SET_ENABLED != 'STD_ON')"!][!//
     [!VAR "CRYPTO_XVIX_XAIX_RANDOM_RNG_CTRDRBG_NOT_SET_ENABLED"!][!IF "[!//
       as:ref(.)/CryptoPrimitiveService =                              'RANDOM'              and [!//
       node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmFamiliy/*, 'CRYPTO_ALGOFAM_AES') and [!//
       node:containsValue(as:ref(.)/CryptoPrimitiveAlgorithmMode/*,    'CRYPTO_ALGOMODE_CTRDRBG')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
     [!ENDVAR!]
   [!ENDIF!]
[!ENDLOOP!][!//
[!VAR "SymMaxKeySize" = "num:i(0)"!][!//
[!VAR "KeyElemCount" = "num:i(count(node:filter(node:refs(node:refs(CryptoKeys/CryptoKey/*/CryptoKeyTypeRef)/CryptoKeyElementRef/*), '1 = ./CryptoKeyElementId')))"!][!//
[!IF "num:i(0) < num:i($KeyElemCount)"!][!//
  [!VAR "SymMaxKeySize" = "num:max(node:filter(node:refs(node:refs(CryptoKeys/CryptoKey/*/CryptoKeyTypeRef)/CryptoKeyElementRef/*), '1 = ./CryptoKeyElementId')/CryptoKeyElementSize)"!][!//
[!ENDIF!][!//

[!VAR "AsymMaxKeySize" = "num:i($SymMaxKeySize)"!][!//

[!IF "($CRYPTO_XVIX_XAIX_RANDOM_RNG_CTRDRBG_NOT_SET_ENABLED = 'STD_ON')"!][!//
  [!IF "(num:i($SymMaxKeySize) < num:i(32))"!][!//
    [!VAR "SymMaxKeySize" = "num:i(32)"!]
  [!ENDIF!][!//
[!ENDIF!][!//

#if(defined CRYPTO_XVIX_XAIX_AL_SYM_MAX_KEY_ELEMENT_SIZE)
#error CRYPTO_XVIX_XAIX_AL_SYM_MAX_KEY_ELEMENT_SIZE is already defined
#endif
#ifndef CSM_SYM_KEY_MAX_LENGTH
/** \brief Maximum size of a symmetrical key. */
[!IF "num:i($SymMaxKeySize) != num:i(0)"!]
#define CRYPTO_XVIX_XAIX_AL_SYM_MAX_KEY_ELEMENT_SIZE [!"num:i($SymMaxKeySize)"!]U
[!ELSE!]
#define CRYPTO_XVIX_XAIX_AL_SYM_MAX_KEY_ELEMENT_SIZE 1U
[!ENDIF!]
#else
#if (CSM_SYM_KEY_MAX_LENGTH < [!"num:i($SymMaxKeySize)"!])
[!IF "($CRYPTO_XVIX_XAIX_RANDOM_RNG_CTRDRBG_NOT_SET_ENABLED = 'STD_ON')"!][!//
#if (CSM_SYM_KEY_MAX_LENGTH > 32U)
#define CRYPTO_XVIX_XAIX_AL_SYM_MAX_KEY_ELEMENT_SIZE CSM_SYM_KEY_MAX_LENGTH
#else
#define CRYPTO_XVIX_XAIX_AL_SYM_MAX_KEY_ELEMENT_SIZE 32U
#endif
[!ELSE!]
#if (CSM_SYM_KEY_MAX_LENGTH > 0U)
#define CRYPTO_XVIX_XAIX_AL_SYM_MAX_KEY_ELEMENT_SIZE CSM_SYM_KEY_MAX_LENGTH
#else
#define CRYPTO_XVIX_XAIX_AL_SYM_MAX_KEY_ELEMENT_SIZE 1U
#endif
[!ENDIF!][!//
#else
[!IF "num:i($SymMaxKeySize) != num:i(0)"!]
#define CRYPTO_XVIX_XAIX_AL_SYM_MAX_KEY_ELEMENT_SIZE [!"num:i($SymMaxKeySize)"!]U
[!ELSE!]
#define CRYPTO_XVIX_XAIX_AL_SYM_MAX_KEY_ELEMENT_SIZE 1U
[!ENDIF!]
#endif
#endif

#if(defined CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE)
#error CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE is already defined
#endif
#ifndef CSM_ASYM_PRIVATE_KEY_MAX_LENGTH
/** \brief Maximum size of an asymmetrical private key. */
[!IF "num:i($AsymMaxKeySize) != num:i(0)"!]
#define CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE [!"num:i($AsymMaxKeySize)"!]U
[!ELSE!]
#define CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE 1U
[!ENDIF!]
#else
#if (CSM_ASYM_PRIVATE_KEY_MAX_LENGTH < [!"num:i($AsymMaxKeySize)"!])
#define CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE CSM_ASYM_PRIVATE_KEY_MAX_LENGTH
#else
[!IF "num:i($AsymMaxKeySize) != num:i(0)"!]
#define CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE [!"num:i($AsymMaxKeySize)"!]U
[!ELSE!]
#define CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE 1U
[!ENDIF!]
#endif
#endif

#if(defined CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE)
#error CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE is already defined
#endif
#ifndef CSM_ASYM_PUBLIC_KEY_MAX_LENGTH
/** \brief Maximum size of an asymmetrical public key. */
[!IF "num:i($AsymMaxKeySize) != num:i(0)"!]
#define CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE [!"num:i($AsymMaxKeySize)"!]U
[!ELSE!]
#define CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE 1U
[!ENDIF!]
#else
#if (CSM_ASYM_PUBLIC_KEY_MAX_LENGTH < [!"num:i($AsymMaxKeySize)"!])
#define CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE CSM_ASYM_PUBLIC_KEY_MAX_LENGTH
#else
[!IF "num:i($AsymMaxKeySize) != num:i(0)"!]
#define CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE [!"num:i($AsymMaxKeySize)"!]U
[!ELSE!]
#define CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE 1U
[!ENDIF!]
#endif
#endif

[!VAR "MaxSaltKeySize" = "num:i(0)"!][!//
[!VAR "KeyElementCount" = "num:i(count(node:filter(node:refs(node:refs(CryptoKeys/CryptoKey/*/CryptoKeyTypeRef)/CryptoKeyElementRef/*), '13 = ./CryptoKeyElementId')))"!][!//
[!IF "num:i(0) < num:i($KeyElementCount)"!][!//
  [!VAR "MaxSaltKeySize" = "num:max(node:filter(node:refs(node:refs(CryptoKeys/CryptoKey/*/CryptoKeyTypeRef)/CryptoKeyElementRef/*), '13 = ./CryptoKeyElementId')/CryptoKeyElementSize)"!][!//
[!ENDIF!][!//

#if(defined CRYPTO_XVIX_XAIX_MAX_SALT_SIZE)
#error CRYPTO_XVIX_XAIX_MAX_SALT_SIZE is already defined
#endif
/** \brief Maximum size of key element containing SALT. */
[!IF "num:i($MaxSaltKeySize) != num:i(0)"!]
#define CRYPTO_XVIX_XAIX_MAX_SALT_SIZE [!"num:i($MaxSaltKeySize)"!]U
[!ELSE!]
#define CRYPTO_XVIX_XAIX_MAX_SALT_SIZE 0U
[!ENDIF!]

/*==================[declaration of types]========================================================*/


/* !LINKSTO EB_Crypto_01061,1 */
/** \brief Typedef for the Crypto_xVIx_xAIx_SymKeyType if the Csm_SymKeyType does not exist due to
 * not configured max key size in CSM.
 */
typedef struct
{
  uint8 data[CRYPTO_XVIX_XAIX_AL_SYM_MAX_KEY_ELEMENT_SIZE];
  uint32 length;
} Crypto_xVIx_xAIx_SymKeyType;


/* !LINKSTO EB_Crypto_01062,1 */
/** \brief Typedef for the Crypto_xVIx_xAIx_AsymPrivateKeyType if the Csm_AsymPrivateKeyType does
 * not exist due to not configured max key size in CSM.
 */
typedef struct
{
  uint8 data[CRYPTO_XVIX_XAIX_AL_ASYM_PRIVATE_MAX_KEY_ELEMENT_SIZE];
  uint32 length;
} Crypto_xVIx_xAIx_AsymPrivateKeyType;


/* !LINKSTO EB_Crypto_01063,1 */
/** \brief Typedef for the Crypto_xVIx_xAIx_AsymPublicKeyType if the Csm_AsymPublicKeyType does not
 * exist due to not configured max key size in CSM.
 */
typedef struct
{
  uint8 data[CRYPTO_XVIX_XAIX_AL_ASYM_PUBLIC_MAX_KEY_ELEMENT_SIZE];
  uint32 length;
} Crypto_xVIx_xAIx_AsymPublicKeyType;

/*==================[declaration of external constants]===========================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

[!LOOP "CryptoKeyElements/CryptoKeyElement/*"!]
/** \brief array with init values for the Key element [!"text:toupper(node:name(.))"!]. */
extern CONST(uint8, CRYPTO_xVIx_xAIx_CONST)
       Crypto_xVIx_xAIx_[!"text:toupper(node:name(.))"!]_INIT_VALUE
       [CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(.))"!]_SIZE];
[!ENDLOOP!]

#define CRYPTO_xVIx_xAIx_STOP_SEC_CONST_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==================[declaration of external variables]===========================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_CLEARED_8
#include <Crypto_xVIx_xAIx_MemMap.h>

[!VAR "i" = "0"!]
[!VAR "s" = "0"!]
[!LOOP "node:order(CryptoKeys/CryptoKey/*, 'node:value(CryptoKeyId)')"!]
  [!VAR "k" = "0"!]
  [!SELECT "./CryptoKeyTypeRef"!]
    [!LOOP "node:ref(.)/CryptoKeyElementRef/*"!]
      [!SELECT "node:current()"!]
        [!IF "$s = 0"!][!//
          [!VAR "s" = "$s + 1"!]
        [!ENDIF!][!//
/** \brief extern data for Key element [!"text:toupper(node:name(as:ref(.)))"!]. */
extern VAR(uint8, CRYPTO_xVIx_xAIx_VAR)
       Crypto_xVIx_xAIx_AL_KeyElemData_[!"num:i($i)"!]_[!"num:i($k)"!]
       [CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(as:ref(.)))"!]_SIZE];
        [!VAR "k" = "$k + 1"!]
      [!ENDSELECT!]
    [!ENDLOOP!]
  [!ENDSELECT!]
  [!VAR "i" = "$i + 1"!]
[!ENDLOOP!]

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_8
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==================[declaration of external functions]===========================================*/

/*================================================================================================*/

#endif /* #ifndef CRYPTO_XVIX_XAIX_CFG_H */

/*==================[end of file]=================================================================*/
