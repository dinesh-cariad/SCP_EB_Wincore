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

/* set macro that enables merged compilation macros */
#define TS_MERGED_COMPILE STD_ON

/* include all foreign includes before inclusion of TcpIp_guards.h
 * Special cases can be excluded in merged_included_ignore and must be handled here.
 * e.g. guarded includes like Det.h & PbcfgM.h
 * All APIs (content of /include/ directory must be added to merged_includes_foreign,
 * in order to include them here (Module prefix is inconclusive). */
#include <TcpIp_merged_includes.h>

/* manual handling of special cases (conditional includes) */
#include <TcpIp_Cfg.h>          /* defines TCPIP_PBCFGM_SUPPORT_ENABLED */
#if (TCPIP_PBCFGM_SUPPORT_ENABLED == STD_ON)
#define PBCFGM_NO_PBCFG_REQUIRED
  #include <PbcfgM.h>           /* Post build configuration manager API */
#endif /* TCPIP_PBCFGM_SUPPORT_ENABLED */

#if TCPIP_DEV_ERROR_DETECT == STD_ON
  #include <Det.h>                  /* Det function declaration */
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
  #include <Csm.h>
#endif /*(TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

#if TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON
  #include <StbM.h>
#endif /* TCPIP_TCP_SYN_COOKIES_ENABLED == STD_ON */

#if TCPIP_NVM_STORING_ENABLED == STD_ON
  #include <NvM.h>
#endif /* TCPIP_NVM_STORING_ENABLED == STD_ON */

/* Now guard the collected internal macros against unsolicited spam defines from outside */
#include <TcpIp_guards.h>

#include <TcpIp_IpAddrM.c>
#include <TcpIp_IpAddrM_CtrlSM.c>
#include <TcpIp_IpAddrM_LocalAddrSM.c>
#include <TcpIp_IpAddrM_AssignSM.c>
#include <TcpIp_Ip.c>
#include <TcpIp_Ip_Reass.c>
#include <TcpIp_Ip_Frag.c>
#include <TcpIp_IpV4.c>
#include <TcpIp_IpV4_Arp.c>
#include <TcpIp_IpV4_Icmp.c>
#include <TcpIp_IpV4_StaticIp.c>
#include <TcpIp_IpV4_LinkLocal.c>
#include <TcpIp_IpV4_AddrConflDetect.c>

#include <TcpIp_IpV6.c>
#include <TcpIp_Ndp.c>
#include <TcpIp_Ndp_Rpd.c>
#include <TcpIp_Ndp_Dad.c>
#include <TcpIp_Ndp_NCache.c>
#include <TcpIp_IpV6_Icmp.c>
#include <TcpIp_IpV6_StaticIp.c>
#include <TcpIp_IpV6_LinkLocal.c>
#include <TcpIp_Udp.c>
#include <TcpIp_Socket_UDP.c>
#include <TcpIp_Socket_TCP.c>
#include <TcpIp_Socket.c> /* must be the last of the 'Socket*' files due to reduced inline fn IF */
#include <TcpIp_Tcp.c>
#include <TcpIp_Tcp_rxData.c>
#include <TcpIp_Tcp_txData.c>
#include <TcpIp_Tcp_SM.c>
#include <TcpIp_Tcp_rxOutOfOrder.c>
#include <TcpIp_Memory.c>
#include <TcpIp_DhcpV4.c>
#include <TcpIp_DhcpV6.c>
#include <TcpIp_Dhcp.c>
#include <TcpIp.c>
#include <TcpIp_IpSec.c>
#include <TcpIp_IpSecDb.c>
#include <TcpIp_UpLDummy.c>
#include <TcpIp_Crypto.c>
