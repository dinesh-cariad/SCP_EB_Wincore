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

[!IF "var:defined('postBuildVariant')"!]
[!VAR "IPDUM_VARIANT" = "concat('_',$postBuildVariant)"!]
[!VAR "lcfgInclGuard" = "concat('IPDUM_', text:toupper($postBuildVariant), '_LCFG_H')"!]
[!ELSE!]
[!VAR "IPDUM_VARIANT" = "''"!]
[!VAR "lcfgInclGuard" = "'IPDUM_LCFG_H'"!]
[!ENDIF!]

#ifndef [!"$lcfgInclGuard"!]
#define [!"$lcfgInclGuard"!]

[!AUTOSPACING!]


[!INCLUDE "../../generate_macros/IpduM_Types.m"!]
[!INCLUDE "../../generate_macros/IpduM_Macros.m"!]

[!CALL "IpduMRegisterTypes", "TypeStrName" = "'IpduMTypeRegistry_RAM'"!]
[!CALL "IpduMEstablishTypes", "TypeStr" = "$IpduMTypeRegistry_RAM_ENABLEDTYPES", "isVar" = "true()"!]

/*==================[includes]==============================================*/
#include <TSAutosar.h>              /* global configuration */
#include <IpduM_Types.h>            /* Module types */
/*==================[macros]================================================*/
[!SELECT "IpduMConfig/*[1]"!]

/**
 * Size of internal IpduM data in units of bytes (static memory
 * allocation) - Memory required by post-build configuration must be smaller
 * than this constant
 */
#if (defined IPDUM_DATA_MEM_SIZE[!"$IPDUM_VARIANT"!])
#error IPDUM_DATA_MEM_SIZE[!"$IPDUM_VARIANT"!] already defined!
#endif
[!IF "node:exists(../../IpduMGeneral/IpduMDataMemSize)"!]
#define IPDUM_DATA_MEM_SIZE[!"$IPDUM_VARIANT"!] [!"../../IpduMGeneral/IpduMDataMemSize"!]
 [!ELSE!]
   [!CALL "GetDataMemSlices"!][!//
#define IPDUM_DATA_MEM_SIZE[!"$IPDUM_VARIANT"!] [!"num:i(node:when($IpduMDataMemMinLenCalc > 0, $IpduMDataMemMinLenCalc, 1))"!]
[!ENDIF!]

[!ENDSELECT!]

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef [!"$lcfgInclGuard"!] */
/*==================[end of file]===========================================*/
