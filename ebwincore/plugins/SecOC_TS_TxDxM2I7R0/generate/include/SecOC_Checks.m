[!/**
 * \file
 *
 * \brief AUTOSAR SecOC
 *
 * This file contains the implementation of the AUTOSAR
 * module SecOC.
 *
 * \version 2.7.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//

[!NOCODE!]
[!IF "not(var:defined('SECOC_CHECKS.M'))"!][!//
  [!VAR "SECOC_CHECKS.M" = "'true'"!][!//
    [!VAR "NumberOfRxPduWithCollection" = "num:i(count(as:modconf('SecOC')/SecOCRxPduProcessing/*[SecOCRxSecuredPduLayer = 'SecOCRxSecuredPduCollection']))"!][!//
    [!LOOP "node:order(as:modconf('SecOC')/SecOCRxPduProcessing/*)"!][!//
      [!IF "$NumberOfRxPduWithCollection = 0"!][!//
        [!IF "SecOCRxSecuredPduLayer = 'SecOCRxSecuredPdu'"!][!//
          [!IF "node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCRxSecuredPduLayer/SecOCRxSecuredLayerPduId, ./SecOCRxSecuredPduLayer/SecOCRxSecuredLayerPduId) = true()"!][!//
            [!ERROR!]For [!"node:name(.)"!] the SecOCRxSecuredLayerPduId has to be unique.[!ENDERROR!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ELSE!][!//
        [!IF "SecOCRxSecuredPduLayer = 'SecOCRxSecuredPdu'"!][!//
          [!IF "(node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCRxSecuredPduLayer/SecOCRxSecuredLayerPduId, ./SecOCRxSecuredPduLayer/SecOCRxSecuredLayerPduId) or 
                 node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCRxSecuredPduLayer/SecOCRxAuthenticPdu/SecOCRxAuthenticPduId, ./SecOCRxSecuredPduLayer/SecOCRxSecuredLayerPduId) or
                 node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCRxSecuredPduLayer/SecOCRxCryptographicPdu/SecOCRxCryptographicPduId, ./SecOCRxSecuredPduLayer/SecOCRxSecuredLayerPduId)) = true()"!][!//
              [!ERROR!]For [!"node:name(.)"!] the SecOCRxSecuredLayerPduId has to be unique.[!ENDERROR!][!//
          [!ENDIF!][!//
        [!ELSE!][!//
          [!IF "(node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCRxSecuredPduLayer/SecOCRxSecuredLayerPduId, ./SecOCRxSecuredPduLayer/SecOCRxAuthenticPdu/SecOCRxAuthenticPduId) or
                 node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCRxSecuredPduLayer/SecOCRxAuthenticPdu/SecOCRxAuthenticPduId, ./SecOCRxSecuredPduLayer/SecOCRxAuthenticPdu/SecOCRxAuthenticPduId) or
                 node:containsValue(../*/SecOCRxSecuredPduLayer/SecOCRxCryptographicPdu/SecOCRxCryptographicPduId, ./SecOCRxSecuredPduLayer/SecOCRxAuthenticPdu/SecOCRxAuthenticPduId)) = true()"!][!//
              [!ERROR!]For [!"node:name(.)"!] the SecOCRxAuthenticPduId has to be unique.[!ENDERROR!][!//
          [!ENDIF!][!//
          [!IF "(node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCRxSecuredPduLayer/SecOCRxSecuredLayerPduId, ./SecOCRxSecuredPduLayer/SecOCRxCryptographicPdu/SecOCRxCryptographicPduId) or
                 node:containsValue(../*/SecOCRxSecuredPduLayer/SecOCRxAuthenticPdu/SecOCRxAuthenticPduId, ./SecOCRxSecuredPduLayer/SecOCRxCryptographicPdu/SecOCRxCryptographicPduId) or
                 node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCRxSecuredPduLayer/SecOCRxCryptographicPdu/SecOCRxCryptographicPduId, ./SecOCRxSecuredPduLayer/SecOCRxCryptographicPdu/SecOCRxCryptographicPduId)) = true()"!][!//
              [!ERROR!]For [!"node:name(.)"!] the SecOCRxCryptographicPduId has to be unique.[!ENDERROR!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//

    [!VAR "NumberOfTxPduWithCollection" = "num:i(count(as:modconf('SecOC')/SecOCTxPduProcessing/*[SecOCTxSecuredPduLayer = 'SecOCTxSecuredPduCollection']))"!][!//
    [!LOOP "node:order(as:modconf('SecOC')/SecOCTxPduProcessing/*)"!][!//
      [!IF "$NumberOfTxPduWithCollection = 0"!][!//
        [!IF "SecOCTxSecuredPduLayer = 'SecOCTxSecuredPdu'"!][!//
          [!IF "node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCTxSecuredPduLayer/SecOCTxSecuredLayerPduId, ./SecOCTxSecuredPduLayer/SecOCTxSecuredLayerPduId) = true()"!][!//
            [!ERROR!]For [!"node:name(.)"!] the SecOCTxSecuredLayerPduId has to be unique.[!ENDERROR!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ELSE!][!//
        [!IF "SecOCTxSecuredPduLayer = 'SecOCTxSecuredPdu'"!][!//
          [!IF "(node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCTxSecuredPduLayer/SecOCTxSecuredLayerPduId, ./SecOCTxSecuredPduLayer/SecOCTxSecuredLayerPduId) or 
                 node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCTxSecuredPduLayer/SecOCTxAuthenticPdu/SecOCTxAuthenticPduId, ./SecOCTxSecuredPduLayer/SecOCTxSecuredLayerPduId) or
                 node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCTxSecuredPduLayer/SecOCTxCryptographicPdu/SecOCTxCryptographicPduId, ./SecOCTxSecuredPduLayer/SecOCTxSecuredLayerPduId)) = true()"!][!//
              [!ERROR!]For [!"node:name(.)"!] the SecOCTxSecuredLayerPduId has to be unique.[!ENDERROR!][!//
          [!ENDIF!][!//
        [!ELSE!][!//
          [!IF "(node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCTxSecuredPduLayer/SecOCTxSecuredLayerPduId, ./SecOCTxSecuredPduLayer/SecOCTxAuthenticPdu/SecOCTxAuthenticPduId) or
                 node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCTxSecuredPduLayer/SecOCTxAuthenticPdu/SecOCTxAuthenticPduId, ./SecOCTxSecuredPduLayer/SecOCTxAuthenticPdu/SecOCTxAuthenticPduId) or
                 node:containsValue(../*/SecOCTxSecuredPduLayer/SecOCTxCryptographicPdu/SecOCTxCryptographicPduId, ./SecOCTxSecuredPduLayer/SecOCTxAuthenticPdu/SecOCTxAuthenticPduId)) = true()"!][!//
              [!ERROR!]For [!"node:name(.)"!] the SecOCTxAuthenticPduId has to be unique.[!ENDERROR!][!//
          [!ENDIF!][!//
          [!IF "(node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCTxSecuredPduLayer/SecOCTxSecuredLayerPduId, ./SecOCTxSecuredPduLayer/SecOCTxCryptographicPdu/SecOCTxCryptographicPduId) or
                 node:containsValue(../*/SecOCTxSecuredPduLayer/SecOCTxAuthenticPdu/SecOCTxAuthenticPduId, ./SecOCTxSecuredPduLayer/SecOCTxCryptographicPdu/SecOCTxCryptographicPduId) or
                 node:containsValue(../*[node:name(node:current()) != node:name(.)]/SecOCTxSecuredPduLayer/SecOCTxCryptographicPdu/SecOCTxCryptographicPduId, ./SecOCTxSecuredPduLayer/SecOCTxCryptographicPdu/SecOCTxCryptographicPduId)) = true()"!][!//
              [!ERROR!]For [!"node:name(.)"!] the SecOCTxCryptographicPduId has to be unique.[!ENDERROR!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDIF!][!//
[!ENDNOCODE!]