/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANTP_LCFG_H
#define CANTP_LCFG_H

[!AUTOSPACING!]
/* !LINKSTO CanTp.ASR40.CANTP160_1,1 */
/*==================[includes]==============================================*/

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
#define CANTP_START_SEC_CONST_32
#include <CanTp_MemMap.h>

/* value used to validate post build configuration against link time configuration */
extern CONST(uint32, CANTP_CONST) CanTp_LcfgSignature;

/* stop data section declaration */
#define CANTP_STOP_SEC_CONST_32
#include <CanTp_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef CANTP_LCFG_H */
/*==================[end of file]===========================================*/
