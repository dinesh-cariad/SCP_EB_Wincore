/**
 * \file
 *
 * \brief AUTOSAR Sd
 *
 * This file contains the implementation of the AUTOSAR
 * module Sd.
 *
 * \version 1.4.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!CODE!]
[!AUTOSPACING!]


#ifndef SD_SYMBOLICNAMES_PBCFG_H
#define SD_SYMBOLICNAMES_PBCFG_H


/*==================[includes]===============================================*/

#include <ComStack_Types.h>        /* AUTOSAR standard types */
#include <Sd_Cfg.h>              /* Generated configuration */

[!LOOP "variant:all()"!][!//
#include <Sd_SymbolicNames[!"."!]_PBcfg.h>
[!ENDLOOP!][!//

/*==================[macros]=================================================*/

/*------------------[symbolic name values]----------------------------------*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef SD_SYMBOLICNAMES_PBCFG_H */
/*==================[end of file]============================================*/
[!ENDCODE!]
