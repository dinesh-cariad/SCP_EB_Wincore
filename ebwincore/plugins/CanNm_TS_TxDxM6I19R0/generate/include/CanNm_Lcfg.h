/**
 * \file
 *
 * \brief AUTOSAR CanNm
 *
 * This file contains the implementation of the AUTOSAR
 * module CanNm.
 *
 * \version 6.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANNM_LCFG_H
#define CANNM_LCFG_H

[!AUTOSPACING!]
/*==================[includes]==============================================*/

#include <TSAutosar.h>              /* global configuration */

/*==================[macros]================================================*/
/* !LINKSTO CanNm.EB.PostBuildRamSize_1,1 */
#define CANNM_DATA_MEM_SIZE [!"num:i(as:modconf('CanNm')[1]/CanNmGlobalConfig/*[1]/CanNmPostBuildRamSize)"!]
/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/* start data section declaration */
#define CANNM_START_SEC_CONST_32
#include <CanNm_MemMap.h>

/* value used to validate post build configuration against link time configuration */
extern CONST(uint32, CANNM_CONST) CanNm_LcfgSignature;

/* stop data section declaration */
#define CANNM_STOP_SEC_CONST_32
#include <CanNm_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef CANNM_LCFG_H */
/*==================[end of file]===========================================*/
