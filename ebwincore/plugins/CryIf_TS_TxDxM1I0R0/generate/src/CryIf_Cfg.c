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

/*==================[misra deviations]============================================================*/

/*==================[inclusions]==================================================================*/

/* !LINKSTO EB_CryIf_00011, 1 */
#include <CryIf.h>
/* !LINKSTO SWS_CryIf_00008, 1 */
#include <CryIf_Cfg.h>

/*==================[macros]======================================================================*/

/*==================[declaration of types]========================================================*/

/*==================[declaration of internal functions]===========================================*/

/*==================[definition of internal constants]============================================*/

/*==================[definition of internal variables]============================================*/

/*==================[definition of external constants]============================================*/

#define CRYIF_START_SEC_CONST_32
#include <CryIf_MemMap.h>

#if (CRYIF_CHANNEL_COUNT != 0U)
/* !LINKSTO CryIf.Dsn.Api/Consts/00017, 1 */
/* !LINKSTO CryIf.Req.Cfg/SymbolicNames/General/00001, 1 */
CONST(uint32, CRYIF_CONST) CryIf_Channels[CRYIF_CHANNEL_COUNT] =
{
[!LOOP "node:order(CryIfChannel/*, 'node:value(./CryIfChannelId)')"!][!//
#if defined(Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:refs(./CryIfDriverObjectRef)/../../..)"!]Conf_CryptoDriverObject_[!"node:name(node:path(node:ref(CryIfDriverObjectRef)))"!])
  Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:refs(./CryIfDriverObjectRef)/../../..)"!]Conf_CryptoDriverObject_[!"node:name(node:path(node:ref(CryIfDriverObjectRef)))"!],
#else
  [!"node:value(node:path(node:ref(CryIfDriverObjectRef)/CryptoDriverObjectId))"!]U,
#endif
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_CHANNEL_COUNT != 0U) */

/* !LINKSTO CryIf.Dsn.Api/Consts/00016, 1 */
/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
/* !LINKSTO CryIf.Req.Cfg/SymbolicNames/General/00001, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(uint32, CRYIF_CONST) CryIf_Keys[CRYIF_KEY_COUNT] =
{
[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
#if defined(Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:refs(./CryIfKeyRef)/../../..)"!]Conf_CryptoKey_[!"node:name(node:path(node:ref(CryIfKeyRef)))"!])
  Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:refs(./CryIfKeyRef)/../../..)"!]Conf_CryptoKey_[!"node:name(node:path(node:ref(CryIfKeyRef)))"!],
#else
  [!"node:value(node:path(node:ref(./CryIfKeyRef)/CryptoKeyId))"!]U,
#endif
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#define CRYIF_STOP_SEC_CONST_32
#include <CryIf_MemMap.h>

#define CRYIF_START_SEC_CONST_UNSPECIFIED
#include <CryIf_MemMap.h>

#if (CRYIF_CHANNEL_COUNT != 0U)
/* !LINKSTO CryIf.Dsn.Api/Consts/00001, 1 */
CONST(CryIf_ProcessJobPtrType, CRYIF_CONST) CryIf_ProcessJobJumpTable[CRYIF_CHANNEL_COUNT] =
{
[!LOOP "node:order(CryIfChannel/*, 'node:value(./CryIfChannelId)')"!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfDriverObjectRef)/../../..)"!]_ProcessJob,
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_CHANNEL_COUNT != 0U) */

#if (CRYIF_CHANNEL_COUNT != 0U)
/* !LINKSTO CryIf.Dsn.Api/Consts/00002, 1 */
CONST(CryIf_CancelJobPtrType, CRYIF_CONST) CryIf_CancelJobJumpTable[CRYIF_CHANNEL_COUNT] =
{
[!LOOP "node:order(CryIfChannel/*, 'node:value(./CryIfChannelId)')"!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfDriverObjectRef)/../../..)"!]_CancelJob,
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_CHANNEL_COUNT != 0U) */

/* !LINKSTO CryIf.Dsn.Api/Consts/00003, 1 */
/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(CryIf_KeyElementSetPtrType, CRYIF_CONST) CryIf_KeyElementSetJumpTable[CRYIF_KEY_COUNT] =
{
[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfKeyRef)/../../..)"!]_KeyElementSet,
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.Api/Consts/00004, 1 */
/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(CryIf_KeySetValidPtrType, CRYIF_CONST) CryIf_KeySetValidJumpTable[CRYIF_KEY_COUNT] =
{
[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
  [!IF "   node:value(./../../CryIfEbGeneral/CryIfEbMisc/CryIfEbAutosarApiVersion) = 'CRYIF_API_VERSION_430'
        or node:value(./../../CryIfEbGeneral/CryIfEbMisc/CryIfEbAutosarApiVersion) = 'CRYIF_API_VERSION_EB'"!]
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfKeyRef)/../../..)"!]_KeyValidSet,
  [!ELSE!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfKeyRef)/../../..)"!]_KeySetValid,
  [!ENDIF!][!//
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.Api/Consts/00005, 1 */
/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(CryIf_KeyElementGetPtrType, CRYIF_CONST) CryIf_KeyElementGetJumpTable[CRYIF_KEY_COUNT] =
{
[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfKeyRef)/../../..)"!]_KeyElementGet,
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.Api/Consts/00006, 1 */
/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(CryIf_KeyElementCopyPtrType, CRYIF_CONST) CryIf_KeyElementCopyJumpTable[CRYIF_KEY_COUNT] =
{
[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfKeyRef)/../../..)"!]_KeyElementCopy,
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.Api/Consts/00007, 1 */
/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(CryIf_KeyElementIdsPtrType, CRYIF_CONST) CryIf_KeyElementIdsGetJumpTable[CRYIF_KEY_COUNT] =
{
[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfKeyRef)/../../..)"!]_KeyElementIdsGet,
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.Api/Consts/00008, 1 */
/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(CryIf_RandomSeedPtrType, CRYIF_CONST) CryIf_RandomSeedJumpTable[CRYIF_KEY_COUNT] =
{
[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfKeyRef)/../../..)"!]_RandomSeed,
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.Api/Consts/00009, 1 */
/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(CryIf_KeyGeneratePtrType, CRYIF_CONST) CryIf_KeyGenerateJumpTable[CRYIF_KEY_COUNT] =
{
[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfKeyRef)/../../..)"!]_KeyGenerate,
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.Api/Consts/00010, 1 */
/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(CryIf_KeyDerivePtrType, CRYIF_CONST) CryIf_KeyDeriveJumpTable[CRYIF_KEY_COUNT] =
{
[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfKeyRef)/../../..)"!]_KeyDerive,
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.Api/Consts/00011, 1 */
/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(CryIf_KeyExchangeCalcPubValPtrType, CRYIF_CONST)
CryIf_KeyExchangeCalcPubValJumpTable[CRYIF_KEY_COUNT] =
{
[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfKeyRef)/../../..)"!]_KeyExchangeCalcPubVal,
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.Api/Consts/00012, 1 */
/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(CryIf_KeyExchangeCalcSecretPtrType, CRYIF_CONST)
CryIf_KeyExchangeCalcSecretJumpTable[CRYIF_KEY_COUNT] =
{
[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfKeyRef)/../../..)"!]_KeyExchangeCalcSecret,
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.Api/Consts/00013, 1 */
/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(CryIf_CertificateParsePtrType, CRYIF_CONST)
CryIf_CertificateParseJumpTable[CRYIF_KEY_COUNT] =
{
[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfKeyRef)/../../..)"!]_CertificateParse,
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.Api/Consts/00014, 1 */
/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(CryIf_CertificateVerifyPtrType, CRYIF_CONST)
CryIf_CertificateVerifyJumpTable[CRYIF_KEY_COUNT] =
{
[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfKeyRef)/../../..)"!]_CertificateVerify,
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

/* !LINKSTO CryIf.Dsn.Api/Consts/00015, 1 */
/* !LINKSTO CryIf.Dsn.NoCodeUnconfiguredKeyMgmt, 1 */
#if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON)
CONST(CryIf_KeyCopyPtrType, CRYIF_CONST) CryIf_KeyCopyJumpTable[CRYIF_KEY_COUNT] =
{
[!LOOP "node:order(CryIfKey/*, 'node:value(./CryIfKeyId)')"!][!//
  &Crypto[!CALL "CryIf_CryptoMip", "ctr" = "node:path(node:ref(./CryIfKeyRef)/../../..)"!]_KeyCopy,
[!ENDLOOP!][!//
};
#endif /* #if (CRYIF_API_ENABLED_KEYMNGMNT == STD_ON) */

#define CRYIF_STOP_SEC_CONST_UNSPECIFIED
#include <CryIf_MemMap.h>

/*==================[definition of external variables]============================================*/

/*==================[definition of external functions]============================================*/

/*==================[definition of internal functions]============================================*/

/*==================[end of file]=================================================================*/
