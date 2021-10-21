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
#ifndef SECOC_LCFG_H
#define SECOC_LCFG_H

[!AUTOSPACING!]
/*==================[includes]==============================================*/

/* !LINKSTO SecOC.Dsn.BasicSWStructure.RxTx.SecOC_Lcfgh,1 */
#include <TSAutosar.h>              /* global configuration */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/* start data section declaration */
#define SECOC_START_SEC_CONST_32
#include <SecOC_MemMap.h>

/* value used to validate post build configuration against link time configuration */
extern CONST(uint32, SECOC_CONST) SecOC_LcfgSignature;

/* stop data section declaration */
#define SECOC_STOP_SEC_CONST_32
#include <SecOC_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef SECOC_LCFG_H */
/*==================[end of file]===========================================*/
