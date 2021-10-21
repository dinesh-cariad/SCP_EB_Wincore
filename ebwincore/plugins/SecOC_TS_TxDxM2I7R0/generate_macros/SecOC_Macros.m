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
[!/*
*** multiple inclusion protection ***
*/!][!IF "not(var:defined('SECOC_MACROS_M'))"!][!//
[!VAR "SECOC_MACROS_M"="'true'"!][!/*

--------------------------------------------------------------------------
    Set the values of useful variables use in post-build config
--------------------------------------------------------------------------

*** Number of configured PDUs on the Rx side ***
*/!][!VAR "RxPduNum" = "num:i(count(SecOCRxPduProcessing/*))"!][!/*

*** Existence of configured PDUs on the Rx side with secured collection enabled ***
*/!][!IF "num:i(count(as:modconf('SecOC')/SecOCRxPduProcessing/*[SecOCRxSecuredPduLayer = 'SecOCRxSecuredPduCollection'])) > 0"!][!/*
*/!][!VAR "RxSecuredColEn" = "'true'"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "RxSecuredColEn" = "'false'"!][!/*
*/!][!ENDIF!][!/*

*** Number of configured PDUs on the Tx side ***
*/!][!VAR "TxPduNum" = "num:i(count(SecOCTxPduProcessing/*))"!][!/*

*** Existence of configured PDUs on the Tx side with secured collection enabled ***
*/!][!IF "num:i(count(as:modconf('SecOC')/SecOCTxPduProcessing/*[SecOCTxSecuredPduLayer = 'SecOCTxSecuredPduCollection'])) > 0"!][!/*
*/!][!VAR "TxSecuredColEn" = "'true'"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "TxSecuredColEn" = "'false'"!][!/*
*/!][!ENDIF!][!/*


*** Determine whether the propagation of the verification status is used or not ***
*/!][!IF "node:value(as:modconf('SecOC')/SecOCGeneral/SecOCPropagateVerificationStatus) != 'NONE'"!][!/*
*/!][!VAR "PropVerStatusEn" = "'true'"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "PropVerStatusEn" = "'false'"!][!/*
*/!][!ENDIF!][!/*

*** Determine the Data ID length in bytes ***
*/!][!IF "node:value(as:modconf('SecOC')/SecOCGeneral/SecOCDataIdLength) = 'UINT8'"!][!/*
*/!][!VAR "DataIdLen" = "1"!][!/*
*/!][!ELSEIF "node:value(as:modconf('SecOC')/SecOCGeneral/SecOCDataIdLength) = 'UINT16'"!][!/*
*/!][!VAR "DataIdLen" = "2"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "DataIdLen" = "4"!][!/*
*/!][!ENDIF!][!/*

*** Determine whether the secured area with a PDU is used or not ***
*/!][!VAR "SecuredAreaEn" = "node:value(SecOCGeneral/SecOCUseSecuredArea)"!][!/*


*** Determine whether the propagation of the reception overflow strategy is used or not ***
*/!][!IF "num:i(count(as:modconf('SecOC')/SecOCRxPduProcessing/*[node:value(./SecOCReceptionOverflowStrategy) = 'REPLACE'])) > 0"!][!/*
*/!][!VAR "RxRecvStrategyReplaceEn" = "'true'"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "RxRecvStrategyReplaceEn" = "'false'"!][!/*
*/!][!ENDIF!][!/*


*** Determine whether the Rx same buffer collection is used or not ***
*/!][!IF "num:i(count(as:modconf('SecOC')/SecOCRxPduProcessing/*[node:exists(./SecOCSameBufferPduRef) = 'true'])) > 0"!][!/*
*/!][!VAR "RxUseSameBufferPdu" = "'true'"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "RxUseSameBufferPdu" = "'false'"!][!/*
*/!][!ENDIF!][!/*

*** Determine whether the Tx same buffer collection is used or not ***
*/!][!IF "num:i(count(as:modconf('SecOC')/SecOCTxPduProcessing/*[node:exists(./SecOCSameBufferPduRef) = 'true'])) > 0"!][!/*
*/!][!VAR "TxUseSameBufferPdu" = "'true'"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "TxUseSameBufferPdu" = "'false'"!][!/*
*/!][!ENDIF!][!/*



*** End of file ***
*/!][!ENDIF!][!//