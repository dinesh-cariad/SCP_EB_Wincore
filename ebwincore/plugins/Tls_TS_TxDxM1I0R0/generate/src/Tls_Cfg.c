/**
 * \file
 *
 * \brief AUTOSAR Tls
 *
 * This file contains the implementation of the AUTOSAR
 * module Tls.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*===============================[includes]======================================================*/
#include <Tls_Int.h>
#include <Tls_Cfg.h>
#if (TLS_CERTIFICATE_SUPPORT == STD_ON)
#include <KeyM.h>
#endif /* (TLS_CERTIFICATE_SUPPORT == STD_ON) */

/*===========================[macro definitions]=================================================*/

#define TLS_CFG_DUMMY_VAL_UINT8 0xFFU
#define TLS_CFG_DUMMY_VAL_UINT16 0xFFFFU
#define TLS_CFG_DUMMY_VAL_UINT32 0xFFFFFFFFU

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

#define TLS_START_SEC_VAR_CLEARED_8
#include <Tls_MemMap.h>

/* Tls buffers */
[!LOOP "node:order(TlsConnection/*, 'node:value(./TlsConnectionId)')"!]

[!// Calculate minimum tx buffer size
[!IF "(./TlsDtls = 'true')"!][!//
[!VAR "headerSize" = "25"!][!//
[!VAR "minMessageSize" = "331"!][!//
[!ELSE!][!//
[!VAR "headerSize" = "9"!][!//
[!VAR "minMessageSize" = "75"!][!//
[!ENDIF!][!//

[!IF "(./TlsEndpoint = 'TLS_CLIENT')"!][!//
[!VAR "identitySize" = "2+num:i(string-length(node:value(./TlsClientIdentity)))"!][!//
[!ELSE!][!//
[!VAR "identitySize" = "2+num:i(string-length(node:value(./TlsServerIdentityHint)))"!][!//
[!ENDIF!][!//

[!VAR "maxSize" = "num:max($minMessageSize | $identitySize)"!][!//

[!VAR "totalSize" = "$headerSize + $maxSize"!][!//

[!VAR "txBufferSize" = "num:max(node:value(./TlsTransmitBufferSize) | $totalSize)"!][!//
[!// End of: Calculate minimum tx buffer size

STATIC uint8 Tls_TransmitBuffer_[!"node:value(./TlsConnectionId)"!][[!"num:i($txBufferSize)"!]];
STATIC uint8 Tls_ReceiveBuffer_[!"node:value(./TlsConnectionId)"!][[!"node:value(./TlsReceiveBufferSize)"!]];
STATIC uint8 Tls_DecryptBuffer_[!"node:value(./TlsConnectionId)"!][[!"node:value(./TlsMaxRecordSize)"!]];
STATIC uint8 Tls_DtlsEncryptBuffer_[!"node:value(./TlsConnectionId)"!][[!"node:value(./TlsMaxRecordSize)"!]];
[!ENDLOOP!]

[!IF "node:exists(TlsConfig/TlsExtensionsDetectionConfig)"!][!//
[!//
/** \brief Function pointer for ExtensionsDetection */
CONST(Tls_ExtensionDetectionFpType, TCPIP_APPL_CONST) Tls_ExtensionsUsedAPI =
  &[!"Tls/Tls/TlsExtensionsDetectionConfig/TlsExtensionsDetectionCalloutName"!];
[!//
[!ENDIF!][!//


#define TLS_STOP_SEC_VAR_CLEARED_8
#include <Tls_MemMap.h>

#define TLS_START_SEC_CONST_UNSPECIFIED
#include <Tls_MemMap.h>

[!// Iterate local certificates
[!LOOP "node:order(TlsConnection/*, 'node:value(./TlsConnectionId)')"!][!//
[!IF "node:refvalid(./TlsKeyMLocalCertRef)"!][!//
STATIC CONST(KeyM_CertificateIdType, TLS_CONST) Tls_LocalCertificateChainIds_[!"node:value(./TlsConnectionId)"!][[!"node:value(./TlsKeyMLocalCertChainLength)"!]] = {[!//
[!VAR "cert" = "node:path(node:ref(./TlsKeyMLocalCertRef))"!][!//
[!FOR "i" = "1" TO "node:value(./TlsKeyMLocalCertChainLength)"!][!//
[!"node:value(concat($cert,'/KeyMCertificateId'))"!]U,[!//
[!VAR "cert" = "node:path(node:ref(node:ref(concat($cert,'/KeyMCertUpperHierarchicalCertRef'))))"!][!//
[!ENDFOR!][!//
};
[!ENDIF!][!//
[!ENDLOOP!][!//

[!// Iterate remote certificates
[!LOOP "node:order(TlsConnection/*, 'node:value(./TlsConnectionId)')"!]
[!IF "node:refvalid(./TlsKeyMRemoteCertRef)"!][!//
STATIC CONST(KeyM_CertificateIdType, TLS_CONST) Tls_RemoteCertificateChainIds_[!"node:value(./TlsConnectionId)"!][[!"node:value(./TlsKeyMRemoteCertChainLength)"!]] = {[!//
[!VAR "cert" = "node:path(node:ref(./TlsKeyMRemoteCertRef))"!][!//
[!FOR "i" = "1" TO "node:value(./TlsKeyMRemoteCertChainLength)"!][!//
[!"node:value(concat($cert,'/KeyMCertificateId'))"!]U,[!//
[!VAR "cert" = "node:path(node:ref(node:ref(concat($cert,'/KeyMCertUpperHierarchicalCertRef'))))"!][!//
[!ENDFOR!][!//
};
[!ENDIF!][!//
[!ENDLOOP!][!//

CONST(Tls_GlobalConfigType, TLS_CONST) Tls_Config =
{
  {
  /* TLS connections */
[!LOOP "node:order(TlsConnection/*, 'node:value(./TlsConnectionId)')"!][!//
  {
    /*cipher*/[!"node:value(./TlsCipherSuite)"!],
[!IF "(./TlsDtls = 'true')"!][!//
    /*DTLS*/TRUE,
[!ELSE!][!//
    /*DTLS*/FALSE,
[!ENDIF!][!//
    /*entity*/[!"node:value(./TlsEndpoint)"!],
[!IF "(./TlsEndpoint = 'TLS_CLIENT')"!][!//
    /*identity*/(const uint8*)"[!"node:value(./TlsClientIdentity)"!]", [!"num:i(string-length(node:value(./TlsClientIdentity)))"!],
[!ELSE!][!//
    /*identity*/(const uint8*)"[!"node:value(./TlsServerIdentityHint)"!]", [!"num:i(string-length(node:value(./TlsServerIdentityHint)))"!],
[!ENDIF!][!//
[!IF "(as:modconf('Tls')[1]/TlsGeneral/TlsCsmKeyDeriveSupport = 'false')"!][!//
[!VAR "csmPath" = "node:path(node:ref(CsmPRFPreMasterSecretJobRef))"!][!//
    /*PRFPreMasterSecretJobId*/[!"node:value(concat($csmPath,'/CsmJobId'))"!],
[!ELSE!][!//
    /*PreMasterSecretJobId*/TLS_CFG_DUMMY_VAL_UINT32,
[!ENDIF!][!//
[!VAR "csmPath" = "node:path(node:ref(CsmPRFMasterSecretJobRef))"!][!//
    /*PRFMasterSecretJobId*/[!"node:value(concat($csmPath,'/CsmJobId'))"!],
[!VAR "csmPath" = "node:path(node:ref(CsmPreMasterSecretKeyRef))"!][!//
    /*PreMasterSecretKeyId*/[!"node:value(concat($csmPath,'/CsmKeyId'))"!],
[!IF "(as:modconf('Tls')[1]/TlsGeneral/TlsCsmKeyDeriveSupport = 'false')"!][!//
[!VAR "csmPath" = "node:path(node:ref(CsmMasterSecretKeyRef))"!][!//
    /*MasterSecretKeyId*/[!"node:value(concat($csmPath,'/CsmKeyId'))"!],
[!ELSE!][!//
    /*MasterSecretKeyId*/TLS_CFG_DUMMY_VAL_UINT32,
[!ENDIF!][!//
[!IF "node:refvalid(./CsmMACGenerateJobRef)"!][!//
[!VAR "csmPath" = "node:path(node:ref(CsmMACGenerateJobRef))"!][!//
    /*MACGenerateJobId*/[!"node:value(concat($csmPath,'/CsmJobId'))"!],
[!ELSE!][!//
    /*MACGenerateJobId*/TLS_CFG_DUMMY_VAL_UINT32,
[!ENDIF!][!//
[!IF "node:refvalid(./CsmMACVerifyJobRef)"!][!//
[!VAR "csmPath" = "node:path(node:ref(CsmMACVerifyJobRef))"!][!//
    /*MACVerifyJobId*/[!"node:value(concat($csmPath,'/CsmJobId'))"!],
[!ELSE!][!//
    /*MACVerifyJobId*/TLS_CFG_DUMMY_VAL_UINT32,
[!ENDIF!][!//
[!IF "node:refvalid(./CsmMACGenerateKeyRef)"!][!//
[!VAR "csmPath" = "node:path(node:ref(CsmMACGenerateKeyRef))"!][!//
    /*MACGenerateKeyId*/[!"node:value(concat($csmPath,'/CsmKeyId'))"!],
[!ELSE!][!//
    /*MACGenerateKeyId*/TLS_CFG_DUMMY_VAL_UINT32,
[!ENDIF!][!//
[!IF "node:refvalid(./CsmMACVerifyKeyRef)"!][!//
[!VAR "csmPath" = "node:path(node:ref(CsmMACVerifyKeyRef))"!][!//
    /*MACVerifyKeyId*/[!"node:value(concat($csmPath,'/CsmKeyId'))"!],
[!ELSE!][!//
    /*MACVerifyKeyId*/TLS_CFG_DUMMY_VAL_UINT32,
[!ENDIF!][!//
[!VAR "csmPath" = "node:path(node:ref(CsmHashJobRef))"!][!//
    /*hashJobId*/[!"node:value(concat($csmPath,'/CsmJobId'))"!],
[!VAR "csmPath" = "node:path(node:ref(CsmRandomJobRef))"!][!//
    /*randomJobId*/[!"node:value(concat($csmPath,'/CsmJobId'))"!],
[!IF "node:refvalid(./CsmEncryptJobRef)"!][!//
[!VAR "csmPath" = "node:path(node:ref(CsmEncryptJobRef))"!][!//
    /*encryptJobId*/[!"node:value(concat($csmPath,'/CsmJobId'))"!],
[!ELSE!][!//
    /*encryptJobId*/TLS_CFG_DUMMY_VAL_UINT32,
[!ENDIF!][!//
[!IF "node:refvalid(./CsmDecryptJobRef)"!][!//
[!VAR "csmPath" = "node:path(node:ref(CsmDecryptJobRef))"!][!//
    /*decryptJobId*/[!"node:value(concat($csmPath,'/CsmJobId'))"!],
[!ELSE!][!//
    /*decryptJobId*/TLS_CFG_DUMMY_VAL_UINT32,
[!ENDIF!][!//
[!IF "node:refvalid(./CsmEncryptKeyRef)"!][!//
[!VAR "csmPath" = "node:path(node:ref(CsmEncryptKeyRef))"!][!//
    /*encryptKeyId*/[!"node:value(concat($csmPath,'/CsmKeyId'))"!],
[!ELSE!][!//
    /*encryptKeyId*/TLS_CFG_DUMMY_VAL_UINT32,
[!ENDIF!][!//
[!IF "node:refvalid(./CsmDecryptKeyRef)"!][!//
[!VAR "csmPath" = "node:path(node:ref(CsmDecryptKeyRef))"!][!//
    /*decryptKeyId*/[!"node:value(concat($csmPath,'/CsmKeyId'))"!],
[!ELSE!][!//
    /*decryptKeyId*/TLS_CFG_DUMMY_VAL_UINT32,
[!ENDIF!][!//
    /*connectionId*/[!"node:value(./TlsConnectionId)"!],
    /*transmitBufferPtr*/Tls_TransmitBuffer_[!"node:value(./TlsConnectionId)"!],
    /*receiveBufferPtr*/Tls_ReceiveBuffer_[!"node:value(./TlsConnectionId)"!],
    /*decryptBufferPtr*/Tls_DecryptBuffer_[!"node:value(./TlsConnectionId)"!],
    /*dtlsEncryptBufferPtr*/Tls_DtlsEncryptBuffer_[!"node:value(./TlsConnectionId)"!],
    /*transmitBufferSize*/[!"node:value(./TlsTransmitBufferSize)"!],
    /*receiveBufferSize*/[!"node:value(./TlsReceiveBufferSize)"!],
    /*decryptBufferSize*/[!"node:value(./TlsMaxRecordSize)"!],
    /*dtlsEncryptBufferSize*/[!"node:value(./TlsMaxRecordSize)"!],
    /*maxRecordSize*/[!"node:value(./TlsMaxRecordSize)"!],
[!IF "(as:modconf('Tls')[1]/TlsGeneral/TlsEnableCertificates = 'true')"!][!//
[!IF "node:refvalid(./TlsKeyMLocalCertRef)"!][!//
    /*localCertChainIdPtr*/Tls_LocalCertificateChainIds_[!"node:value(./TlsConnectionId)"!],
    /*localCertChainLength*/[!"node:value(./TlsKeyMLocalCertChainLength)"!],
[!VAR "csmPath" = "node:path(node:ref(TlsCsmLocalCertSigGenerateJobRef))"!][!//
    /*localCertSigGenerateJobId*/[!"node:value(concat($csmPath,'/CsmJobId'))"!],
[!ELSE!][!//
    /*localCertChainIdPtr*/NULL_PTR,
    /*localCertChainLength*/TLS_CFG_DUMMY_VAL_UINT8,
    /*localCertSigGenerateJobId*/TLS_CFG_DUMMY_VAL_UINT32,
[!ENDIF!][!//
[!IF "node:exists(./TlsLocalCaDistinguishedName)"!][!//
    /*localCaCommonName*/(const uint8*)"[!"node:value(./TlsLocalCaDistinguishedName)"!]", [!"num:i(string-length(node:value(./TlsLocalCaDistinguishedName)))"!]U,
[!ELSE!][!//
    /*localCaCommonName*/(const uint8*)"", 0U,
[!ENDIF!][!//
[!IF "node:refvalid(./TlsKeyMRemoteCertRef)"!][!//
    /*remoteCertChainIdPtr*/Tls_RemoteCertificateChainIds_[!"node:value(./TlsConnectionId)"!],
    /*remoteCertChainLength*/[!"node:value(./TlsKeyMRemoteCertChainLength)"!],
[!VAR "csmPath" = "node:path(node:ref(node:ref(TlsKeyMRemoteCertRef)/KeyMCertCsmSignatureVerifyJobRef))"!][!//
    /*remoteCertSigVerifyJobId*/[!"node:value(concat($csmPath,'/CsmJobId'))"!],
[!ELSE!][!//
    /*remoteCertChainIdPtr*/NULL_PTR,
    /*remoteCertChainLength*/TLS_CFG_DUMMY_VAL_UINT8,
    /*remoteCertSigVerifyJobId*/TLS_CFG_DUMMY_VAL_UINT32,
[!ENDIF!][!//
[!ENDIF!][!//
[!IF "node:refvalid(./TlsCsmKeyExchangeEphemeralGenerateKeyRef)"!][!//
[!VAR "csmPath" = "node:path(node:ref(TlsCsmKeyExchangeEphemeralGenerateKeyRef))"!][!//
    /*keyExchangeEphemeralGenerateKeyId*/[!"node:value(concat($csmPath,'/CsmKeyId'))"!],
[!ELSE!][!//
    /*keyExchangeEphemeralGenerateKeyId*/TLS_CFG_DUMMY_VAL_UINT32,
[!ENDIF!][!//
[!IF "node:refvalid(./TlsCsmKeyExchangeEphemeralExchangeKeyRef)"!][!//
[!VAR "csmPath" = "node:path(node:ref(TlsCsmKeyExchangeEphemeralExchangeKeyRef))"!][!//
    /*keyExchangeEphemeralExchangeKeyId*/[!"node:value(concat($csmPath,'/CsmKeyId'))"!],
[!ELSE!][!//
    /*keyExchangeEphemeralExchangeKeyId*/TLS_CFG_DUMMY_VAL_UINT32,
[!ENDIF!][!//
    [!IF "(./TcpIpTlsUseSecurityExtensionRecordSizeLimit = 'true')"!][!//
    /*RecordSizeLimitExtensionEnabled*/TRUE,
    [!ELSE!][!//
    /*RecordSizeLimitExtensionEnabled*/FALSE,
    [!ENDIF!][!//
  },
[!ENDLOOP!]
  }
};

#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include <Tls_MemMap.h>
