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
#ifndef TCPIP_H
#define TCPIP_H

/*  MISRA-C:2012 Deviation List
 *
 *  MISRA-1) Deviated Rule: 20.5 (advisory)
 *   #undef shall not be used.
 *
 *   Reason:
 *   Macro TCPIP_NO_PBCFG_REQUIRED and TCPIP_NO_CFG_REQUIRED may be defined in
 *   more than one instance which will cause compile
 *   warning.
 */

/*==================[inclusions]============================================*/

#include <TcpIp_Api.h>            /* general public API */
#include <TcpIp_IpAddrM_Api.h>    /* public API implemented by SW-unit IpAddrM */
#include <TcpIp_Socket_Api.h>     /* public API implemented by SW-unit Socket */
#include <TcpIp_Ip_Api.h>         /* public API implemented by SW-unit Ip */
#include <TcpIp_IpV4_Api.h>       /* public API implemented by SW-unit IpV4 */
#include <TcpIp_Memory_Api.h>     /* public API implemented by SW-unit Memory */
#include <TcpIp_Ndp_Api.h>        /* public API implemented by SW-unit Ndp */
#include <TcpIp_Udp_Api.h>        /* public API implemented by SW-unit Udp */
#include <TcpIp_Tcp_Api.h>        /* public API implemented by SW-unit Tcp */
#include <TcpIp_DhcpV4_Api.h>     /* public API implemented by SW-unit Dhcp */
#include <TcpIp_DhcpV6_Api.h>     /* public API implemented by SW-unit DhcpV6 */
#include <TcpIp_IpSec_Api.h>      /* public API implemented by SW-unit IpSec */
#include <TcpIp_Generic_Api.h>    /* generated TcpIp_<Up>GetSocket API */

/* Exclude post-build-time config include file if requested to do so */
#if (!defined TCPIP_NO_PBCFG_REQUIRED) && (!defined TCPIP_NO_CFG_REQUIRED)
#if (TCPIP_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <TcpIp_PBcfg.h>         /* get TcpIp post build configuration */
#endif /* TCPIP_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* TCPIP_NO_PBCFG_REQUIRED, TCPIP_NO_CFG_REQUIRED */


/* Deviation MISRA-1 */
#undef TCPIP_NO_PBCFG_REQUIRED
/* Deviation MISRA-1 */
#undef TCPIP_NO_CFG_REQUIRED

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef TCPIP_H */
/*==================[end of file]===========================================*/
