/**
 * \file
 *
 * \brief Autosar LinIf
 *
 * This file contains the implementation of the Autosar
 * module LinIf.
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2012 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!IF "LinIfGeneral/LinIfTpSupported = 'true'"!]

/*==================[inclusions]============================================*/

#include <LinTp_Lcfg.h>   /* Generated configuration */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/* start data section declaration */
#define LINIF_START_SEC_CONST_32
#include <LinIf_MemMap.h>

/* value used to validate post build configuration against link time configuration */
CONST(uint32, LINIF_CONST) LinTp_LcfgSignature = [!"asc:getConfigSignature(as:modconf('LinTp')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!]U;

/* stop data section declaration */
#define LINIF_STOP_SEC_CONST_32
#include <LinIf_MemMap.h>


/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/
[!ELSE!]
#include <TSCompiler.h>
TS_PREVENTEMPTYTRANSLATIONUNIT
[!ENDIF!]

/*==================[end of file]===========================================*/
