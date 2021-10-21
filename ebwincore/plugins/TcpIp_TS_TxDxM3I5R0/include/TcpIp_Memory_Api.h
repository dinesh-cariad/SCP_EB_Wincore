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
#ifndef TCPIP_MEMORY_API_H
#define TCPIP_MEMORY_API_H

#if (TCPIP_NVM_STORING_ENABLED == STD_ON)
/*==================[inclusions]============================================*/

#include <ComStack_Types.h>       /* AUTOSAR comstack types */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external data declarations]============================*/

#define TCPIP_START_SEC_VAR_CLEARED_32
#include <TcpIp_MemMap.h>

/** \brief TcpIp NvM internal memory cache */
extern VAR(uint32,TCPIP_VAR_CLEARED) TcpIp_Memory_NvM_Ip_Memory[TCPIP_NVM_BLOCK_SIZE];

#define TCPIP_STOP_SEC_VAR_CLEARED_32
#include <TcpIp_MemMap.h>

/*==================[external function declarations]=========================*/

#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */

#endif /* ifndef TCPIP_MEMORY_API_H */
/*==================[end of file]===========================================*/
