/**
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
 */

/*==================[inclusions]=============================================*/

/* !LINKSTO SecOC.Dsn.BasicSWStructure.RxTx.SecOC_Lcfgc,1 */
#include <TSAutosar.h>         /* EB specific standard types */
#include <SecOC_Lcfg.h> /* Generated configuration */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/* start data section declaration */
#define SECOC_START_SEC_CONST_32
#include <SecOC_MemMap.h>

/* value used to validate post build configuration against link time configuration */
CONST(uint32, SECOC_CONST) SecOC_LcfgSignature = [!"asc:getConfigSignature(as:modconf('SecOC')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!]U;

/* stop data section declaration */
#define SECOC_STOP_SEC_CONST_32
#include <SecOC_MemMap.h>

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/

