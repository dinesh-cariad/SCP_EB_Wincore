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

#ifndef CANNM_LCFG_STATIC_H_
#define CANNM_LCFG_STATIC_H_

/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
/*==================[version check]=========================================*/

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/* start data section declaration */
#define CANNM_START_SEC_CONST_UNSPECIFIED
#include <CanNm_MemMap.h>

/** \brief Pointer to statically allocated RAM.
 **/
extern CONSTP2VAR(uint8, CANNM_CONST, CANNM_VAR_CLEARED) CanNm_Rx_Tx_Buffer;

/* stop data section declaration */
#define CANNM_STOP_SEC_CONST_UNSPECIFIED
#include <CanNm_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* CANNM_LCFG_STATIC_H_ */
