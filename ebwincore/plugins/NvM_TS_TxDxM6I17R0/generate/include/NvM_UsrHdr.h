/**
 * \file
 *
 * \brief AUTOSAR NvM
 *
 * This file contains the implementation of the AUTOSAR
 * module NvM.
 *
 * \version 6.17.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef NVM_USR_HDR_H
#define NVM_USR_HDR_H

/* this just contains the inclusion directive for the user supplied header */

/*==================[inclusions]============================================*/

[!LOOP "NvMCommon/NvMUserHeader/*"!][!//
[!IF "normalize-space(.) != ''"!][!//
#include <[!"normalize-space(.)"!]>
[!ENDIF!][!//
[!ENDLOOP!][!//

/*==================[macros]================================================*/
#if (defined NVM_CRYPTO_HOOKS)
#error NVM_CRYPTO_HOOKS is already defined
#endif
/** \brief Defines whether the Crypto hooks are enabled
 **
 **        Range:
 **        STD_ON  : Crypto Hooks are enabled
 **        STD_OFF : Crypto Hooks are disabled
 **/
#define NVM_CRYPTO_HOOKS           [!//
[!IF "NvMCommon/NvMCommonCryptoSecurityParameters/NvMEnableCryptoSecurityHooks = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "NvMCommon/NvMCommonCryptoSecurityParameters/NvMEnableCryptoSecurityHooks = 'true'"!]
#if (defined NVM_CRYPTO_READ_HOOK)
#error NVM_CRYPTO_READ_HOOK is already defined
#endif
/** \brief Adaptation for the ReadHook API
 **
 **/
#define NVM_CRYPTO_READ_HOOK(BlockId,pbuffer,userdatasize,cryptoextrainfosize,firstcall)           [!"NvMCommon/NvMCommonCryptoSecurityParameters/NvMCryptoReadHook"!](BlockId,pbuffer,userdatasize,cryptoextrainfosize,firstcall)[!//

#if (defined NVM_CRYPTO_WRITE_HOOK)
#error NVM_CRYPTO_WRITE_HOOK is already defined
#endif
/** \brief Adaptation for the WriteHook API
 **
 **/
#define NVM_CRYPTO_WRITE_HOOK(BlockId,pbuffer,userdatasize,cryptoextrainfosize,firstcall)           [!"NvMCommon/NvMCommonCryptoSecurityParameters/NvMCryptoWriteHook"!](BlockId,pbuffer,userdatasize,cryptoextrainfosize,firstcall)[!//

[!ENDIF!][!//
/*==================[external function declarations]========================*/

/*==================[ end of external function declarations]================*/

#endif /* ifndef NVM_USR_HDR_H */
/*==================[end of file NvM_Cfg.h]==================================*/
