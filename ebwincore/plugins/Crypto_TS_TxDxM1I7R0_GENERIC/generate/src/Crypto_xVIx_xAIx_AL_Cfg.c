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

[!VAR "nvmNeeded" = "'false'"!][!//
 [!LOOP "node:order(CryptoKeys/CryptoKey/*)"!][!//
  [!IF "num:i(0) != num:i(count(CryptoKeyNvRamBlockIds/*))"!][!//
   [!VAR "nvmNeeded" = "'true'"!][!//
 [!ENDIF!][!//
[!ENDLOOP!][!//
[!/* ------------------------------------------------------------------ */!][!//
/*================================================================================================*/

/*==================[includes]====================================================================*/

#include <Crypto_xVIx_xAIx_AL_KeyManagement.h>
#include <Crypto_xVIx_xAIx_AL_Cfg.h>

/*==================[macros]======================================================================*/

/*==================[type definitions]============================================================*/

/*==================[external function declarations]==============================================*/

/*==================[internal function declarations]==============================================*/

/*==================[external constants]==========================================================*/

/*==================[internal constants]==========================================================*/

/*==================[internal data]===============================================================*/

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
/**
* \brief Keys elements data.
*/
          [!VAR "s" = "$s + 1"!]
        [!ENDIF!][!//
VAR(uint8, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_AL_KeyElemData_[!"num:i($i)"!]_[!"num:i($k)"!][CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(as:ref(.)))"!]_SIZE];
        [!VAR "k" = "$k + 1"!]
      [!ENDSELECT!]
    [!ENDLOOP!]
  [!ENDSELECT!]
  [!VAR "i" = "$i + 1"!]
[!ENDLOOP!]

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_CLEARED_8
#include <Crypto_xVIx_xAIx_MemMap.h>

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/**
* \brief Keys data.
*/
[!VAR "i" = "0"!]
[!LOOP "node:order(CryptoKeys/CryptoKey/*, 'node:value(CryptoKeyId)')"!]
   [!VAR "CryptoKeyId" = "node:value(CryptoKeyId)"!]
   [!SELECT "./CryptoKeyTypeRef"!]
static VAR(Crypto_xVIx_xAIx_AL_KeyDataType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_AL_KeyData_[!"num:i($i)"!][[!"num:i(count(as:ref(.)/CryptoKeyElementRef/*))"!]U] =
      [!VAR "k" = "0"!]
{
      [!LOOP "as:ref(.)/CryptoKeyElementRef/*"!]
         [!SELECT "node:current()"!]
            [!VAR "keyElem" = "node:name(as:ref(.))"!]
{
CRYPTO_XVIX_XAIX_KE_[!"text:toupper(node:name(as:ref(.)))"!]_SIZE,
&Crypto_xVIx_xAIx_AL_KeyElemData_[!"num:i($i)"!]_[!"num:i($k)"!][0U],
&Crypto_xVIx_xAIx_[!"text:toupper(node:name(as:ref(.)))"!]_INIT_VALUE[0U],

[!IF "$nvmNeeded = 'true'"!][!//
            [!VAR "j" = "$i + 1"!]
            [!VAR "nvramId" = "0"!]
            [!LOOP "../../../../../CryptoKeys/CryptoKey/*[num:i($j)]/CryptoKeyNvRamBlockIds/*"!]
               [!IF "node:name(as:ref(CryptoPersistKeyElement)) = string($keyElem)"!]
                  [!VAR "nvramId" = "node:value(as:ref(CryptoNvramBlockIdRef)/NvMNvramBlockIdentifier)"!]
                  [!VAR "nvramIdName" = "text:toupper(node:name(as:ref(CryptoNvramBlockIdRef)))"!]
               [!ENDIF!]
            [!ENDLOOP!]
            [!IF "num:i($nvramId) > 0"!]
[!"string($nvramIdName)"!]_ID
            [!ELSE!][!//
0U
            [!ENDIF!]
[!ENDIF!][!//
},
            [!VAR "k" = "$k + 1"!]
         [!ENDSELECT!]
      [!ENDLOOP!]
      [!VAR "i" = "$i + 1"!]
};
   [!ENDSELECT!]
[!ENDLOOP!]

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==================[external data]===============================================================*/

#define CRYPTO_xVIx_xAIx_START_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/**
 * \brief Array holding all keys data.
 */
/* !LINKSTO EB_Crypto_01018,1 */
#if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON)
VAR(Crypto_xVIx_xAIx_AL_KeysDataType, CRYPTO_xVIx_xAIx_VAR) Crypto_xVIx_xAIx_AL_Keys_Data[CRYPTO_XVIX_XAIX_KEY_COUNT] =
{
[!VAR "i" = "0"!]
[!LOOP "node:order(CryptoKeys/CryptoKey/*, 'node:value(CryptoKeyId)')"!]
  [!SELECT "./CryptoKeyTypeRef"!]
  {
    [!"num:i(count(as:ref(.)/CryptoKeyElementRef/*))"!]U,
    &Crypto_xVIx_xAIx_AL_KeyData_[!"num:i($i)"!][0U]
  },
  [!ENDSELECT!]
[!VAR "i" = "$i + 1"!]
[!ENDLOOP!]
};
#endif /* #if (CRYPTO_XVIX_XAIX_API_ENABLED_KEYMNGMNT == STD_ON) */

#define CRYPTO_xVIx_xAIx_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Crypto_xVIx_xAIx_MemMap.h>

/*==================[external function definitions]===============================================*/

/*==================[internal function definitions]===============================================*/

/*================================================================================================*/

/*==================[end of file]=================================================================*/

