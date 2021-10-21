/**
 * \file
 *
 * \brief AUTOSAR IpduM
 *
 * This file contains the implementation of the AUTOSAR
 * module IpduM.
 *
 * \version 3.3.40
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


#ifndef IPDUM_TYPECHECK_H
#define IPDUM_TYPECHECK_H

[!AUTOSPACING!]
/*==================[inclusions]============================================*/

/*==================[macros]================================================*/
[!NOCODE!]
[!IF "node:fallback("->as:modconf('IpduM')/IpduMDefensiveProgramming/IpduMStaticAssertEnabled = 'true'", false())"!]
[!INCLUDE "../../generate_macros/IpduM_Types.m"!]
[!INCLUDE "../../generate_macros/IpduM_Macros.m"!]
[!CALL "IpduMRegisterTypes", "TypeStrName" = "'IpduMTypeRegistry_RAM'"!]
[!CALL "IpduMEstablishTypes", "TypeStr" = "$IpduMTypeRegistry_RAM_ENABLEDTYPES", "isVar" = "true()"!]
[!LOOP "text:split($IpduMTypeRegistry_RAM_ENABLEDTYPES, ';')"!]
  [!CALL "IpduMGetVar", "VarName" = "concat(., '_SIZE')"!]
[!CODE!]TS_STATIC_ASSERT(sizeof([!"."!]) == [!"$VarVal"!], [!"."!]_type_size_mismatch);[!CR!][!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
/* Static Assertions disabled, empty header */
[!ENDIF!]
[!ENDNOCODE!]
/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* IPDUM_TYPECHECK_H */
/*==================[end of file]===========================================*/
