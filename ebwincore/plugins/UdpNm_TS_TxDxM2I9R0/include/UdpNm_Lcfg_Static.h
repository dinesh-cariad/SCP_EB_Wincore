/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef UDPNM_LCFG_STATIC_H_
#define UDPNM_LCFG_STATIC_H_

/*==================[inclusions]============================================*/
#include <UdpNm_Adaptive.h>

#if (UDPNM_ADAPTIVE == STD_ON)
#include <AdaptiveUdpNm_Cfg.h>
#else
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
#define UDPNM_START_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>

/** \brief Pointer to statically allocated RAM.
 **/
extern CONSTP2VAR(uint8, UDPNM_CONST, UDPNM_VAR_CLEARED) UdpNm_Rx_Tx_Buffer;

/* stop data section declaration */
#define UDPNM_STOP_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif
#endif /* UDPNM_LCFG_STATIC_H_ */
