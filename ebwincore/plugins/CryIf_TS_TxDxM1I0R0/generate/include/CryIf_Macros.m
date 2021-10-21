[!/**
 * \file
 *
 * \brief AUTOSAR CryIf
 *
 * This file contains the implementation of the AUTOSAR
 * module CryIf.
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2018 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
[!/* *** multiple inclusion protection *** */!][!//
[!IF "not(var:defined('CRYIF_MACROS_M'))"!][!//
[!VAR "CRYIF_MACROS_M" = "'true'"!][!//
[!INDENT "0"!][!//
[!//
[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
[!/*
****************************************************************************************************
* MACRO to get the module implementation prefix (MIP) of a Crypto module.
****************************************************************************************************
*/!][!//
[!//
[!MACRO "CryIf_CryptoMip", "ctr"!][!//
  [!SELECT "node:ref($ctr)"!][!//
    [!IF "node:exists(as:modconf('CryIf')/CryIfEbGeneral/CryIfEbGeneralBswmdImplementation)"!][!//
      [!IF "node:exists(as:modconf('CryIf')/CryIfEbGeneral/CryIfEbGeneralBswmdImplementation/CryIfEbGeneralBswmdImplementationRefs/*/CryIfCryptoRef[node:path(node:ref(.)) = $ctr])"!][!//
        [!/* !LINKSTO CryIf.Dsn.Tbcg/Mip/00003,1 */!][!//
        [!SELECT "as:modconf('CryIf')/CryIfEbGeneral/CryIfEbGeneralBswmdImplementation/CryIfEbGeneralBswmdImplementationRefs/*/CryIfCryptoRef[node:path(node:ref(.)) = $ctr]/.."!][!//
          [!VAR "vendorID" = "asc:getVendorId(as:path(node:ref(./CryIfCryptoBswImplementationRef)))"!][!//
          [!VAR "apiInfix" = "asc:getVendorApiInfix(as:path(node:ref(./CryIfCryptoBswImplementationRef)))"!][!//
        [!ENDSELECT!][!//
      [!ELSE!][!//
        [!/* !LINKSTO CryIf.Dsn.Tbcg/Mip/00004,1 */!][!//
        [!VAR "vendorID" = "./CommonPublishedInformation/VendorId"!][!//
        [!VAR "apiInfix" = "./CommonPublishedInformation/VendorApiInfix"!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
      [!/* !LINKSTO CryIf.Dsn.Tbcg/Mip/00002,1 */!][!//
      [!VAR "vendorID" = "./CommonPublishedInformation/VendorId"!][!//
      [!VAR "apiInfix" = "./CommonPublishedInformation/VendorApiInfix"!][!//
    [!ENDIF!][!//
    [!IF "(string-length($vendorID) > 0) and (string-length($apiInfix) > 0)"!][!//
_[!"$vendorID"!]_[!"$apiInfix"!][!//
    [!ELSE!][!//
      [!IF "(node:empty($vendorID)) and (node:empty($apiInfix))"!][!//
      [!ELSE!][!//
        [!ERROR "?concat('The vendorId or apiInfix are empty for ', node:path(node:ref(./CryIfCryptoRef)/../..))"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDSELECT!][!//
[!ENDMACRO!][!//
[!//
[!/* -------------------------------------------------------------------------------------- */!][!//
[!//
[!ENDINDENT!][!//
[!ENDIF!][!/* *** multiple inclusion protection *** */!][!//
