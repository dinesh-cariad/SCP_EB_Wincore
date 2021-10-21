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


#ifndef IPDUM_ISVALIDCONFIG_H
#define IPDUM_ISVALIDCONFIG_H

/*==================[inclusions]============================================*/

#include <IpduM_Cfg.h>            /* Compile time configuration */
#include <TSAutosar.h>            /* EB specific standard types */
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <IpduM_Lcfg_Static.h>    /* Link time static symbols  */
#include <IpduM_Lcfg.h>           /* Link time configuration */
#include <TSPBConfig_Signature.h> /* Post build signature check */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/* AUTOSAR Memory Mapping - start section for code */

#define IPDUM_START_SEC_CODE
#include <IpduM_MemMap.h>

extern FUNC(Std_ReturnType, IPDUM_CODE) IpduM_IsValidConfig
(
    P2CONST(void, AUTOMATIC, IPDUM_APPL_CONST) ConfigPtr
);

#define IPDUM_STOP_SEC_CODE
#include <IpduM_MemMap.h>

#endif /* IPDUM_ISVALIDCONFIG_H */
/*==================[internal function definitions]=========================*/
/*==================[end of file]===========================================*/
