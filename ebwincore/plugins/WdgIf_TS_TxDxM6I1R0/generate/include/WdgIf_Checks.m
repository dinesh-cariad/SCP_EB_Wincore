[!/**
 * \file
 *
 * \brief AUTOSAR WdgIf
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgIf.
 *
 * \version 6.1.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]

[!//*** multiple inclusion protection ***
[!IF "not(var:defined('WDGIF_CHECKS_M'))"!]
[!VAR "WDGIF_CHECKS_M"="'true'"!]
[!SELECT "as:modconf('WdgIf')[1]"!]

[!VAR "VendorList" = "''"!]
[!VAR "VendorApiInfixCnt" = "num:i(0)"!]

[!IF "WdgIfGeneral/WdgIfDriverAPIInfixEnable = 'true'"!][!//
[!LOOP "WdgIfDevice/*"!][!//
    [!IF "node:exists(WdgIfDrvBswImplementationRef)"!][!//
    [!/* In case WdgIfDrvBswImplementationRef is set the VendorApiInfix and VendorID
       * shall be taken from the Wdg BSWMD.
       */!][!//
        [!VAR "VendorApiInfix" = "asc:getVendorApiInfix(as:path(node:ref(WdgIfDrvBswImplementationRef)))"!][!//
        [!VAR "VendorId" = "asc:getVendorId(as:path(node:ref(WdgIfDrvBswImplementationRef)))"!][!//
    [!ELSE!][!//
        [!VAR "VendorApiInfix" = "as:ref(WdgIfDriverRef)/../CommonPublishedInformation/VendorApiInfix"!][!//
        [!VAR "VendorId" = "as:ref(WdgIfDriverRef)/../CommonPublishedInformation/VendorId"!][!//
    [!ENDIF!][!//

    [!//
    [!/* check how many VendorApiInfix are configured */!][!//
    [!IF "$VendorApiInfix=''"!][!//
        [!/* An empty VendorApiInfix is allowed for at most one WdgIfDevice. If more then one then signal an error  */!][!//
        [!VAR "VendorApiInfixCnt" = "num:i(num:i($VendorApiInfixCnt) + num:i(1))"!]
    [!ELSE!][!//
        [!VAR "VendorList" = "concat($VendorList, $VendorApiInfix, $VendorId, ':')"!][!//
    [!ENDIF!][!//

[!ENDLOOP!][!//
[!ELSE!][!//

[!ENDIF!][!//

[!/* If there are more than one referenced drivers at most one VendorApiInfixCnt can be empty */!][!//
[!IF "(num:i($VendorApiInfixCnt) > 1)"!]
[!ERROR!]The driver specific API infix of the referenced Wdg driver is not available and more than one Wdg driver without API infix is configured. There can be at most one Wdg driver without API infix in one project.[!ENDERROR!]
[!ELSE!][!//

[!LOOP "text:split($VendorList, ':' )"!][!//
[!IF "not(text:uniq(text:split($VendorList, ':' ), (.)))"!][!//
[!ERROR!]The referenced Wdg drivers must have a unique combination of VendorId and API infix. At least one Wdg driver violates this rule.[!ENDERROR!]
[!ELSE!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!//

[!ENDIF!]

[!// - EOF ----------------------
[!ENDSELECT!]
[!ENDIF!]
[!ENDNOCODE!]