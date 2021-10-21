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
#ifndef IPDUM_LCFG_H
#define IPDUM_LCFG_H

[!AUTOSPACING!]
/*==================[includes]==============================================*/

[!LOOP "variant:all()"!][!//
#include <IpduM_[!"."!]Lcfg.h>
[!ENDLOOP!][!//

/*==================[macros]================================================*/

/**
 * Size of internal IpduM data in units of bytes (static memory
 * allocation) - Memory required by post-build configuration must be smaller
 * than this constant
 */
#if (defined IPDUM_DATA_MEM_SIZE)
#error IPDUM_DATA_MEM_SIZE already defined!
#endif
[!NOCODE!]
[!VAR "compMacro" = "''"!]
[!FOR "v1" = "1" TO "variant:size()"!]
  [!VAR "vrnt1" = "concat('_', variant:all()[position() = $v1])"!]
  [!VAR "compMacro"!][!"$compMacro"!][!"node:when($v1 != 1, '#el', '#')"!]if([!ENDVAR!]
  [!VAR "iter" = "variant:size() - 1"!]
  [!FOR "v2" = "1" TO "variant:size()"!]
    [!VAR "vrnt2" = "concat('_', variant:all()[position() = $v2])"!]
    [!VAR "compMacro"!][!"$compMacro"!][!ENDVAR!]
    [!IF "$vrnt1 != $vrnt2"!]
      [!VAR "compMacro"!][!"$compMacro"!] (IPDUM_DATA_MEM_SIZE[!"$vrnt1"!] >= IPDUM_DATA_MEM_SIZE[!"$vrnt2"!])[!"node:when($iter > 1, ' && \', ' )')"!][!CR!][!ENDVAR!]
      [!VAR "iter" = "$iter - 1"!]
    [!ENDIF!]
  [!ENDFOR!]
  [!VAR "compMacro"!][!"$compMacro"!][!CR!]#define IPDUM_DATA_MEM_SIZE IPDUM_DATA_MEM_SIZE[!"$vrnt1"!][!CR!][!CR!][!ENDVAR!]
[!ENDFOR!]
[!VAR "compMacro"!][!"$compMacro"!]#endif[!ENDVAR!]
[!ENDNOCODE!]

[!"$compMacro"!]


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef IPDUM_LCFG_H */
/*==================[end of file]===========================================*/

