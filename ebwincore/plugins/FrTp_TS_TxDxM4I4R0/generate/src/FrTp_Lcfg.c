/**
 * \file
 *
 * \brief AUTOSAR FrTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTp.
 *
 * \version 4.4.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <TSAutosar.h> /* EB specific standard types */
#include <ComStack_Types.h> /* Communication stack types from AUTOSAR */
#include <FrTp_Lcfg.h> /* Generated configuration */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/* start data section declaration */
#define FRTP_START_SEC_CONST_32
#include <FrTp_MemMap.h>

/* !LINKSTO FRTP210,1 */
/* value used to validate post build configuration against link time configuration */
CONST(uint32, FRTP_CONST) FrTp_LcfgSignature = [!"asc:getConfigSignature(as:modconf('FrTp')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!]U;

/* stop data section declaration */
#define FRTP_STOP_SEC_CONST_32
#include <FrTp_MemMap.h>


/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
