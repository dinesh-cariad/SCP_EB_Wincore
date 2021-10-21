/**
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
 */
#ifndef WDGIF_WDGDRIVERS_H
#define WDGIF_WDGDRIVERS_H

/*==================[inclusions]=============================================*/
[!NOCODE!]

[!AUTOSPACING!]
[!INCLUDE "WdgIf_Checks.m"!][!//
[!CODE!][!//
/* Driver specific header files */
[!ENDCODE!][!//

************************************************************************
*** loop over watchdog references and generate driver specific #includes:
*** generate includes for driver specific API files of format
*** Wdg_<VendorId>_<VendorApiInfix>.h
************************************************************************
[!CODE!][!//
[!IF "WdgIfGeneral/WdgIfDriverAPIInfixEnable = 'true'"!][!//
[!LOOP "WdgIfDevice/*"!][!//
[!/* Since EB tresos Studio 2008.b-sr.1, the parameter VendorApiInfix
   * (part of CommonPublishedInformation) is always available.
   * So it is not necessary to differentiate whether or not this parameter exists.
   */!][!//
[!IF "node:exists(WdgIfDrvBswImplementationRef)"!][!//
[!/* In case WdgIfDrvBswImplementationRef is set the VendorApiInfix and VendorID
   * shall be taken from the Wdg BSWMD.
   */!][!//
[!VAR "WDGIF_VENDORAPIINFIX"!][!//
[!"asc:getVendorApiInfix(as:path(node:ref(WdgIfDrvBswImplementationRef)))"!][!//
[!ENDVAR!][!//
[!VAR "WDGIF_VENDORID"!][!//
[!"asc:getVendorId(as:path(node:ref(WdgIfDrvBswImplementationRef)))"!][!//
[!ENDVAR!][!//
[!ELSE!][!//
[!VAR "WDGIF_VENDORAPIINFIX"!][!//
[!"as:ref(WdgIfDriverRef)/../CommonPublishedInformation/VendorApiInfix"!][!//
[!ENDVAR!][!//
[!VAR "WDGIF_VENDORID"!][!//
[!"as:ref(WdgIfDriverRef)/../CommonPublishedInformation/VendorId"!][!//
[!ENDVAR!][!//
[!ENDIF!][!//
[!//
[!/* check content of VendorApiInfix */!][!//
#include [!//
[!IF "$WDGIF_VENDORAPIINFIX=''"!][!//
[!/* VendorApiInfix is empty; this implies count(WdgIfDevice/*) == 1 */!][!//
<Wdg.h> [!//
[!ELSE!][!//
<Wdg_[!"$WDGIF_VENDORID"!]_[!"$WDGIF_VENDORAPIINFIX"!].h> [!//
[!ENDIF!][!//
/* Watchdog driver API defines */
[!ENDLOOP!][!//
[!ELSE!][!//
#include <Wdg.h> [!//
/* Watchdog driver API defines */
[!ENDIF!][!//
[!ENDCODE!][!//
[!ENDNOCODE!]

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef WDGIF_WDGDRIVERS_H */
/*==================[end of file]============================================*/
