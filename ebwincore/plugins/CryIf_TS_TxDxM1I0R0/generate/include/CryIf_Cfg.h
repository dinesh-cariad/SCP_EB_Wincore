[!AUTOSPACING!]
[!/* ====================================================================================== */!][!//
[!INCLUDE "include/CryIf_Macros.m"!]
[!/* ====================================================================================== */!][!//
[!//
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
#ifndef CRYIF_CFG_H
#define CRYIF_CFG_H

/* !LINKSTO SWS_CryIf_00007, 1 */

/*==================[misra deviations]============================================================*/

/*==================[inclusions]==================================================================*/

/* !LINKSTO EB_CryIf_00011, 1 */
#include <Std_Types.h>
#include <Csm_Types.h>

[!LOOP "node:order(node:refs(./CryIfChannel/*/CryIfDriverObjectRef)/../../.. | node:refs(./CryIfKey/*/CryIfKeyRef)/../../.., 'node:value(./CommonPublishedInformation/VendorApiInfix)')"!][!//
#include <Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(.)"!].h>
[!ENDLOOP!][!//

/*==================[macros]======================================================================*/

/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (defined CRYIF_API_ENABLED_KEYMNGMNT)
#error CRYIF_API_ENABLED_KEYMNGMNT is already defined
#endif
/** \brief Key management API enabled/disabled info */
/* !LINKSTO CryIf.Dsn.Api/Macros/00002, 1 */
[!IF "num:i(count(CryIfKey/*)) > 0"!][!//
#define CRYIF_API_ENABLED_KEYMNGMNT STD_ON
[!ELSE!][!//
#define CRYIF_API_ENABLED_KEYMNGMNT STD_OFF
[!ENDIF!][!//

#if (defined CRYIF_API_ENABLED_DEVERRORDETECT)
#error CRYIF_API_ENABLED_DEVERRORDETECT is already defined
#endif
/** \brief Configuration parameter CryIfDevErrorDetection */
/* !LINKSTO CryIf.Dsn.Api/Macros/00001, 1 */
[!IF "(CryIfGeneral/CryIfDevErrorDetect = 'true')"!][!//
#define CRYIF_API_ENABLED_DEVERRORDETECT STD_ON
[!ELSE!][!//
#define CRYIF_API_ENABLED_DEVERRORDETECT STD_OFF
[!ENDIF!][!//

#if (defined CRYIF_API_ENABLED_VERSIONINFO)
#error CRYIF_API_ENABLED_VERSIONINFO is already defined
#endif
/** \brief Configuration parameter CryIfVersionInfoApi */
/* !LINKSTO CryIf.Dsn.Api/Macros/00005, 1 */
[!IF "(CryIfGeneral/CryIfVersionInfoApi = 'true')"!][!//
#define CRYIF_API_ENABLED_VERSIONINFO STD_ON
[!ELSE!][!//
#define CRYIF_API_ENABLED_VERSIONINFO STD_OFF
[!ENDIF!][!//

#if (defined CRYIF_CHANNEL_COUNT)
#error CRYIF_CHANNEL_COUNT is already defined
#endif
/** \brief Number of CryIf channels */
/* !LINKSTO CryIf.Dsn.Api/Macros/00004, 1 */
#define CRYIF_CHANNEL_COUNT [!"num:i(count(CryIfChannel/*))"!]U

[!LOOP "node:order(CryIfChannel/*, 'node:value(./CryIfChannelId)')"!][!//
#if (defined CryIfConf_CryIfChannel_[!"node:name(.)"!])
#error CryIfConf_CryIfChannel_[!"node:name(.)"!] is already defined
#endif
/** \brief CryIf channel [!"node:name(.)"!] */
#define CryIfConf_CryIfChannel_[!"node:name(.)"!] [!"node:value(./CryIfChannelId)"!]U

[!ENDLOOP!]

#if (defined CRYIF_KEY_COUNT)
#error CRYIF_KEY_COUNT is already defined
#endif
/** \brief Number of CryIf keys */
/* !LINKSTO CryIf.Dsn.Api/Macros/00009, 1 */
#define CRYIF_KEY_COUNT [!"num:i(count(CryIfKey/*))"!]U

[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
#if (defined CryIfConf_CryIfKey_[!"node:name(.)"!])
#error CryIfConf_CryIfKey_[!"node:name(.)"!] is already defined
#endif
/** \brief CryIf key [!"node:name(.)"!] */
#define CryIfConf_CryIfKey_[!"node:name(.)"!] [!"node:value(./CryIfKeyId)"!]U

[!ENDLOOP!]

[!//
[!VAR "maxKeyElements" = "0"!][!//
[!LOOP "CryIfKey/*"!][!//
[!SELECT "node:ref(./CryIfKeyRef)/CryptoKeyTypeRef"!][!//
[!SELECT "node:ref(.)"!][!//
[!IF "count(./CryptoKeyElementRef/*) > $maxKeyElements"!][!//
[!VAR "maxKeyElements" = "count(./CryptoKeyElementRef/*)"!][!//
[!ENDIF!][!//
[!ENDSELECT!][!//
[!ENDSELECT!][!//
[!ENDLOOP!][!//
#if (defined CRYIF_MAX_KEY_ELEMENTS_IN_KEY)
#error CRYIF_MAX_KEY_ELEMENTS_IN_KEY is already defined
#endif
/** \brief Maximum number of key elements in a key */
/* !LINKSTO CryIf.Dsn.Api/Macros/00006, 1 */
#define CRYIF_MAX_KEY_ELEMENTS_IN_KEY [!"num:i($maxKeyElements)"!]U

#if (defined CRYIF_MAX_KEY_ELEMNT_COPY_SIZE)
#error CRYIF_MAX_KEY_ELEMNT_COPY_SIZE is already defined
#endif
/** \brief Maximum key size for key or element copy in bytes */
/* !LINKSTO CryIf.Dsn.Api/Macros/00010, 1 */
#define CRYIF_MAX_KEY_ELEMNT_COPY_SIZE [!"num:i(CryIfGeneral/CryIfMaxKeyElementCopySize)"!]U

/* --- Misc --- */

/* !LINKSTO CryIf.Req.Api/CRYIF_API_VERSION_430/00001,1 */
#if (defined CRYIF_API_VERSION_430)
  #error CRYIF_API_VERSION_430 is already defined
#endif /* #if (defined CRYIF_API_VERSION_430) */
#define CRYIF_API_VERSION_430  0x00U

/* !LINKSTO CryIf.Req.Api/CRYIF_API_VERSION_431/00001,1 */
#if (defined CRYIF_API_VERSION_431)
  #error CRYIF_API_VERSION_431 is already defined
#endif /* #if (defined CRYIF_API_VERSION_431) */
#define CRYIF_API_VERSION_431  0x01U

/* !LINKSTO CryIf.Req.Api/CRYIF_API_VERSION_EB/00001,1 */
#if (defined CRYIF_API_VERSION_EB)
  #error CRYIF_API_VERSION_EB is already defined
#endif /* #if (defined CRYIF_API_VERSION_EB) */
#define CRYIF_API_VERSION_EB   0x02U

#if (defined CRYIF_KEY_EMPTY)
  #error CRYIF_KEY_EMPTY is already defined 
#endif /* #if (defined CRYIF_KEY_EMPTY) */
/** \brief Value of the empty key for services that to not need a Crypto key */
#define CRYIF_KEY_EMPTY 0xFFFFFFFFU

#if (defined CRYIF_API_VERSION)
#error CRYIF_API_VERSION is already defined
#endif
/** \brief  The API version that shall be supported by the CryIf. **/
/* !LINKSTO CryIf.Dsn.Api/Macros/00012,1 */
#define CRYIF_API_VERSION [!"node:value(./CryIfEbGeneral/CryIfEbMisc/CryIfEbAutosarApiVersion)"!]

/*==================[declaration of types]========================================================*/

/*==================[declaration of external constants]===========================================*/

/*==================[declaration of external variables]===========================================*/

/*==================[declaration of external functions]===========================================*/

/*================================================================================================*/

#endif /* #ifndef CRYIF_CFG_H */

/*==================[end of file]=================================================================*/
