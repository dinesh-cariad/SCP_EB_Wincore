/**
 * \file
 *
 * \brief AUTOSAR TcpIp
 *
 * This file contains the implementation of the AUTOSAR
 * module TcpIp.
 *
 * \version 3.5.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef TCPIP_CRYPTOTYPES_H_
#define TCPIP_CRYPTOTYPES_H_

/*==================[inclusions]============================================*/

#include <Std_Types.h>              /* TcpIp pre-compile-time configuration */
#include <Eth_GeneralTypes.h>       /* hardware handle type */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

typedef struct
{
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) fragDataPtr;
  uint16 fragDataLength;
  Eth_BufIdxType buffIdx;
} TcpIp_Crypto_FragmentType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* TCPIP_CRYPTOTYPES_H_ */
