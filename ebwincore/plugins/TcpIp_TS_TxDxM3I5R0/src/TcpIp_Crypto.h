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

#ifndef TCPIP_CRYPTO_H
#define TCPIP_CRYPTO_H

/*==================[inclusions]============================================*/

#if ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) || (TCPIP_TCP_CRYPTO_ENABLED == STD_ON))

#include <Csm.h>
#include <TcpIp_CustomCsmInterface.h> /* csm interface modifications */

#include <TcpIp_guards.h>     /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_CryptoTypes.h>        /* definition of crypto types */
#include <TcpIp_Int.h>                /* module internal headerfile */

/*==================[macros]================================================*/

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYGENERATE
#define TcpIp_Crypto_KeyGenerate Csm_KeyGenerate
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYGENERATE */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE
#define TcpIp_Crypto_Common_MacGenerate TcpIp_Crypto_MacGenerate
#define TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE STD_ON
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE */

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY
#define TcpIp_Crypto_Common_MacVerify TcpIp_Crypto_MacVerify
#define TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY STD_ON
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYEXCHANGECALCPUBVAL
#define TcpIp_Crypto_KeyExchangeCalcPubVal Csm_KeyExchangeCalcPubVal
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYEXCHANGECALCPUBVAL */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYELEMENTSET
#define TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYELEMENTSET STD_ON
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYELEMENTSET */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_RANDOMGENERATE
#define TcpIp_Crypto_RandomGenerate Csm_RandomGenerate
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_RANDOMGENERATE */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_AEADENCRYPT
#define TcpIp_Crypto_AEADEncrypt Csm_AEADEncrypt
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_AEADENCRYPT */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_AEADDENCRYPT
#define TcpIp_Crypto_AEADDecrypt Csm_AEADDecrypt
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_AEADDENCRYPT */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_ENCRYPT
#define TcpIp_Crypto_Encrypt Csm_Encrypt
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_ENCRYPT */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_DENCRYPT
#define TcpIp_Crypto_Decrypt Csm_Decrypt
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_DENCRYPT */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYELEMENTGET
#define TcpIp_Crypto_KeyElementGet Csm_KeyElementGet
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYELEMENTGET */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYELEMENTCOPY
#define TcpIp_Crypto_KeyElementCopy Csm_KeyElementCopy
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYELEMENTCOPY */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_CERTIFICATEVERIFY
#define TcpIp_Crypto_CertificateVerify Csm_CertificateVerify
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_CERTIFICATEVERIFY */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_HASH
#define TcpIp_Crypto_Hash Csm_Hash
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_HASH */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYEXCHANGECALCSECRET
#define TcpIp_Crypto_KeyExchangeCalcSecret Csm_KeyExchangeCalcSecret
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYEXCHANGECALCSECRET */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_CERTIFICATEPARSE
#define TcpIp_Crypto_CertificateParse Csm_CertificateParse
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_CERTIFICATEPARSE */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE
#define TcpIp_Crypto_Gmac_MacGenerate TcpIp_Crypto_MacGenerate
#define TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE STD_ON
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY
#define TcpIp_Crypto_Gmac_MacVerify TcpIp_Crypto_MacVerify
#define TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY STD_ON
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY */

#else /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE
#define TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE STD_OFF
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY
#define TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY STD_OFF
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY */

#ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY
#define TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY STD_OFF
#endif /* ifndef TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY */

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
/*==================[type definitions]======================================*/

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
typedef P2FUNC(Std_ReturnType, TCPIP_CODE, TcpIp_Crypto_MacVerifyFctPtrType)
(
  uint32 jobId,
  P2CONST(TcpIp_Crypto_FragmentType, AUTOMATIC, TCPIP_APPL_DATA) dataFragsPtr,
  uint8 numOfDataFragments,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) macPtr,
  uint8 macLength
);

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

typedef P2FUNC(Std_ReturnType, TCPIP_CODE, TcpIp_Crypto_MacGenerateFctPtrType)
(
  uint32 jobId,
  P2CONST(TcpIp_Crypto_FragmentType, AUTOMATIC, TCPIP_APPL_DATA) dataFragsPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) macPtr,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) macLengthPtr,
  uint8 numOfDataFragments
);

/*==================[external function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE == STD_ON) ||  \
     (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE == STD_ON))

TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Crypto_MacGenerate
(
  uint32 jobId,
  P2CONST(TcpIp_Crypto_FragmentType, AUTOMATIC, TCPIP_APPL_DATA) dataFragsPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) macPtr,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) macLengthPtr,
  uint8 numOfDataFragments
);

#endif /* ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACGENERATE == STD_ON) ||
           (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACGENERATE == STD_ON)) */

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)

#if ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY == STD_ON) ||  \
     (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY == STD_ON))

TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Crypto_MacVerify
(
  uint32 jobId,
  P2CONST(TcpIp_Crypto_FragmentType, AUTOMATIC, TCPIP_APPL_DATA) dataFragsPtr,
  uint8 numOfDataFragments,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) macPtr,
  uint8 macLength
);

#endif /* ((TCPIP_FUNCENABLED_TCPIP_CRYPTO_COMMON_MACVERIFY == STD_ON) ||
           (TCPIP_FUNCENABLED_TCPIP_CRYPTO_GMAC_MACVERIFY == STD_ON)) */

#if ((TCPIP_IPSEC_GMAC_ENABLED == STD_ON))

TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Crypto_setNonce
(
  uint32 keyId,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) initializationVectorPtr,
  uint16 initializationVectorSize,
  uint8 saltSize
);

#endif /* (TCPIP_IPSEC_GMAC_ENABLED == STD_ON) */

#if ((TCPIP_IPSEC_GMAC_ENABLED == STD_ON))

#if (TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYELEMENTSET == STD_ON)

TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Crypto_KeyElementSet
(
  uint32 keyId,
  uint32 keyElementId,
  P2CONST(uint8,  AUTOMATIC, TCPIP_APPL_DATA) keyPtr,
  uint32 keyLength
);

#endif /* (TCPIP_FUNCENABLED_TCPIP_CRYPTO_KEYELEMENTSET == STD_ON) */

#endif /* ((TCPIP_IPSEC_GMAC_ENABLED == STD_ON)) */

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/
#endif /* ((TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) || (TCPIP_TCP_CRYPTO_ENABLED == STD_ON)) */

#endif /* TCPIP_CRYPTO_H */
