/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRNM_LCFG_STATIC_H_
#define FRNM_LCFG_STATIC_H_

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
#define FRNM_START_SEC_CONST_UNSPECIFIED
#include <FrNm_MemMap.h>

/**
 * \brief Pointer to statically allocated RAM.
 */
extern CONSTP2VAR(uint8, FRNM_CONST, FRNM_VAR_CLEARED) FrNm_Rx_Tx_Buffer;

/* stop data section declaration */
#define FRNM_STOP_SEC_CONST_UNSPECIFIED
#include <FrNm_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* FRNM_LCFG_STATIC_H_ */
