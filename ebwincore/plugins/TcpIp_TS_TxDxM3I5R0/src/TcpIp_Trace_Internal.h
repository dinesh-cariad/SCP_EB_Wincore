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
#ifndef TCPIP_TRACE_INTERNAL_H
#define TCPIP_TRACE_INTERNAL_H

/* give test files a chance to disable the internal trace macros completely */
#ifndef TCPIP_DISABLE_INTERNAL_TRACE_MACROS_FOR_TESTING
/*==================[inclusions]============================================*/

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

/*==================[macros]================================================*/
/* Place internal debug instrumentation macros in this file, i.e. all
   macros that are not generated for the public api instrumentation.
*/

#ifndef DBG_TCPIP_INIT_ENTRY
/** \brief Entry point of function TcpIp_Init() */
#define DBG_TCPIP_INIT_ENTRY(a)
#endif

#ifndef DBG_TCPIP_INIT_EXIT
/** \brief Exit point of function TcpIp_Init() */
#define DBG_TCPIP_INIT_EXIT(a)
#endif

#ifndef DBG_TCPIP_MAINFUNCTION_ENTRY
/** \brief Entry point of function TcpIp_MainFunction() */
#define DBG_TCPIP_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_TCPIP_MAINFUNCTION_EXIT
/** \brief Exit point of function TcpIp_MainFunction() */
#define DBG_TCPIP_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_TCPIP_MAINFUNCTIONTX_ENTRY
/** \brief Entry point of function TcpIp_MainFunctionTx() */
#define DBG_TCPIP_MAINFUNCTIONTX_ENTRY()
#endif

#ifndef DBG_TCPIP_MAINFUNCTIONTX_EXIT
/** \brief Exit point of function TcpIp_MainFunctionTx() */
#define DBG_TCPIP_MAINFUNCTIONTX_EXIT()
#endif

#ifndef DBG_TCPIP_REQUESTCOMMODE_ENTRY
/** \brief Entry point of function TcpIp_RequestComMode() */
#define DBG_TCPIP_REQUESTCOMMODE_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_REQUESTCOMMODE_EXIT
/** \brief Exit point of function TcpIp_RequestComMode() */
#define DBG_TCPIP_REQUESTCOMMODE_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_GETVERSIONINFO_ENTRY
/** \brief Entry point of function TcpIp_GetVersionInfo() */
#define DBG_TCPIP_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_TCPIP_GETVERSIONINFO_EXIT
/** \brief Exit point of function TcpIp_GetVersionInfo() */
#define DBG_TCPIP_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_TCPIP_GETANDRESETMEASUREMENTDATA_ENTRY
/** \brief Entry point of function TcpIp_GetAndResetMeasurementData() */
#define DBG_TCPIP_GETANDRESETMEASUREMENTDATA_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_GETANDRESETMEASUREMENTDATA_EXIT
/** \brief Exit point of function TcpIp_GetAndResetMeasurementData() */
#define DBG_TCPIP_GETANDRESETMEASUREMENTDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_RXINDICATION_ENTRY
/** \brief Entry point of function TcpIp_RxIndication() */
#define DBG_TCPIP_RXINDICATION_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_TCPIP_RXINDICATION_EXIT
/** \brief Exit point of function TcpIp_RxIndication() */
#define DBG_TCPIP_RXINDICATION_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_TCPIP_IP_REACHCONFIRMATION_ENTRY
/** \brief Entry point of function TcpIp_Ip_reachConfirmation() */
#define DBG_TCPIP_IP_REACHCONFIRMATION_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IP_REACHCONFIRMATION_EXIT
/** \brief Exit point of function TcpIp_Ip_reachConfirmation() */
#define DBG_TCPIP_IP_REACHCONFIRMATION_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IP_ISVALIDTCPREMOTEADDRESS_ENTRY
/** \brief Entry point of function TcpIp_Ip_isValidTcpRemoteAddress() */
#define DBG_TCPIP_IP_ISVALIDTCPREMOTEADDRESS_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IP_ISVALIDTCPREMOTEADDRESS_EXIT
/** \brief Exit point of function TcpIp_Ip_isValidTcpRemoteAddress() */
#define DBG_TCPIP_IP_ISVALIDTCPREMOTEADDRESS_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_SOCKET_GETSOCKET_ENTRY
/** \brief Entry point of function TcpIp_Socket_getSocket() */
#define DBG_TCPIP_SOCKET_GETSOCKET_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_SOCKET_GETSOCKET_EXIT
/** \brief Exit point of function TcpIp_Socket_getSocket() */
#define DBG_TCPIP_SOCKET_GETSOCKET_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_BIND_ENTRY
/** \brief Entry point of function TcpIp_Bind() */
#define DBG_TCPIP_BIND_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_BIND_EXIT
/** \brief Exit point of function TcpIp_Bind() */
#define DBG_TCPIP_BIND_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_TCPCONNECT_ENTRY
/** \brief Entry point of function TcpIp_TcpConnect() */
#define DBG_TCPIP_TCPCONNECT_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_TCPCONNECT_EXIT
/** \brief Exit point of function TcpIp_TcpConnect() */
#define DBG_TCPIP_TCPCONNECT_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_TCPLISTEN_ENTRY
/** \brief Entry point of function TcpIp_TcpListen() */
#define DBG_TCPIP_TCPLISTEN_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_TCPLISTEN_EXIT
/** \brief Exit point of function TcpIp_TcpListen() */
#define DBG_TCPIP_TCPLISTEN_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_CLOSE_ENTRY
/** \brief Entry point of function TcpIp_Close() */
#define DBG_TCPIP_CLOSE_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_CLOSE_EXIT
/** \brief Exit point of function TcpIp_Close() */
#define DBG_TCPIP_CLOSE_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_UDPTRANSMIT_ENTRY
/** \brief Entry point of function TcpIp_UdpTransmit() */
#define DBG_TCPIP_UDPTRANSMIT_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_UDPTRANSMIT_EXIT
/** \brief Exit point of function TcpIp_UdpTransmit() */
#define DBG_TCPIP_UDPTRANSMIT_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_TCPTRANSMIT_ENTRY
/** \brief Entry point of function TcpIp_TcpTransmit() */
#define DBG_TCPIP_TCPTRANSMIT_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_TCPTRANSMIT_EXIT
/** \brief Exit point of function TcpIp_TcpTransmit() */
#define DBG_TCPIP_TCPTRANSMIT_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_CHANGEPARAMETER_ENTRY
/** \brief Entry point of function TcpIp_ChangeParameter() */
#define DBG_TCPIP_CHANGEPARAMETER_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_CHANGEPARAMETER_EXIT
/** \brief Exit point of function TcpIp_ChangeParameter() */
#define DBG_TCPIP_CHANGEPARAMETER_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_TCPRECEIVED_ENTRY
/** \brief Entry point of function TcpIp_TcpReceived() */
#define DBG_TCPIP_TCPRECEIVED_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_TCPRECEIVED_EXIT
/** \brief Exit point of function TcpIp_TcpReceived() */
#define DBG_TCPIP_TCPRECEIVED_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_REQUESTIPADDRASSIGNMENT_ENTRY
/** \brief Entry point of function TcpIp_RequestIpAddrAssignment() */
#define DBG_TCPIP_REQUESTIPADDRASSIGNMENT_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_REQUESTIPADDRASSIGNMENT_EXIT
/** \brief Exit point of function TcpIp_RequestIpAddrAssignment() */
#define DBG_TCPIP_REQUESTIPADDRASSIGNMENT_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_RELEASEIPADDRASSIGNMENT_ENTRY
/** \brief Entry point of function TcpIp_ReleaseIpAddrAssignment() */
#define DBG_TCPIP_RELEASEIPADDRASSIGNMENT_ENTRY(a)
#endif

#ifndef DBG_TCPIP_RELEASEIPADDRASSIGNMENT_EXIT
/** \brief Exit point of function TcpIp_ReleaseIpAddrAssignment() */
#define DBG_TCPIP_RELEASEIPADDRASSIGNMENT_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_ICMPTRANSMIT_ENTRY
/** \brief Entry point of function TcpIp_IcmpTransmit() */
#define DBG_TCPIP_ICMPTRANSMIT_ENTRY(a,b,c,d,e,f,g)
#endif

#ifndef DBG_TCPIP_ICMPTRANSMIT_EXIT
/** \brief Exit point of function TcpIp_IcmpTransmit() */
#define DBG_TCPIP_ICMPTRANSMIT_EXIT(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_TCPIP_DHCPREADOPTION_ENTRY
/** \brief Entry point of function TcpIp_DhcpReadOption() */
#define DBG_TCPIP_DHCPREADOPTION_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_ICMPTRANSMITERROR_ENTRY
/** \brief Entry point of function TcpIp_IcmpTransmitError() */
#define DBG_TCPIP_ICMPTRANSMITERROR_ENTRY(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_TCPIP_ICMPTRANSMITERROR_EXIT
/** \brief Exit point of function TcpIp_IcmpTransmitError() */
#define DBG_TCPIP_ICMPTRANSMITERROR_EXIT(a,b,c,d,e,f,g,h,i)
#endif

#ifndef DBG_TCPIP_DHCPREADOPTION_EXIT
/** \brief Exit point of function TcpIp_DhcpReadOption() */
#define DBG_TCPIP_DHCPREADOPTION_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_DHCPWRITEOPTION_ENTRY
/** \brief Entry point of function TcpIp_DhcpWriteOption() */
#define DBG_TCPIP_DHCPWRITEOPTION_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_DHCPWRITEOPTION_EXIT
/** \brief Exit point of function TcpIp_DhcpWriteOption() */
#define DBG_TCPIP_DHCPWRITEOPTION_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_GETIPADDR_ENTRY
/** \brief Entry point of function TcpIp_GetIpAddr() */
#define DBG_TCPIP_GETIPADDR_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_GETIPADDR_EXIT
/** \brief Exit point of function TcpIp_GetIpAddr() */
#define DBG_TCPIP_GETIPADDR_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_GETPHYSADDR_ENTRY
/** \brief Entry point of function TcpIp_GetPhysAddr() */
#define DBG_TCPIP_GETPHYSADDR_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_GETPHYSADDR_EXIT
/** \brief Exit point of function TcpIp_GetPhysAddr() */
#define DBG_TCPIP_GETPHYSADDR_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_ISCONNECTIONREADY_ENTRY
/** \brief Entry point of function TcpIp_IsConnectionReady() */
#define DBG_TCPIP_ISCONNECTIONREADY_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_ISCONNECTIONREADY_EXIT
/** \brief Exit point of function TcpIp_IsConnectionReady() */
#define DBG_TCPIP_ISCONNECTIONREADY_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPADDRM_INIT_ENTRY
/** \brief Entry point of function TcpIp_IpAddrM_init() */
#define DBG_TCPIP_IPADDRM_INIT_ENTRY()
#endif

#ifndef DBG_TCPIP_IPADDRM_INIT_EXIT
/** \brief Exit point of function TcpIp_IpAddrM_init() */
#define DBG_TCPIP_IPADDRM_INIT_EXIT()
#endif

#ifndef DBG_TCPIP_IPADDRM_MAINFUNCTION_ENTRY
/** \brief Entry point of function TcpIp_IpAddrM_mainFunction() */
#define DBG_TCPIP_IPADDRM_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_TCPIP_IPADDRM_MAINFUNCTION_EXIT
/** \brief Exit point of function TcpIp_IpAddrM_mainFunction() */
#define DBG_TCPIP_IPADDRM_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_TCPIP_IPADDRM_GETLOCALADDRINFO_ENTRY
/** \brief Entry point of function TcpIp_IpAddrM_getLocalAddrInfo() */
#define DBG_TCPIP_IPADDRM_GETLOCALADDRINFO_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPADDRM_GETLOCALADDRINFO_EXIT
/** \brief Exit point of function TcpIp_IpAddrM_getLocalAddrInfo() */
#define DBG_TCPIP_IPADDRM_GETLOCALADDRINFO_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPADDRM_GETANYIPLOCALADDRID_ENTRY
/** \brief Entry point of function TcpIp_IpAddrM_getAnyIpLocalAddrId() */
#define DBG_TCPIP_IPADDRM_GETANYIPLOCALADDRID_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPADDRM_GETANYIPLOCALADDRID_EXIT
/** \brief Exit point of function TcpIp_IpAddrM_getAnyIpLocalAddrId() */
#define DBG_TCPIP_IPADDRM_GETANYIPLOCALADDRID_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPADDRM_LOCKIFONLINE_ENTRY
/** \brief Entry point of function TcpIp_IpAddrM_lockIfOnline() */
#define DBG_TCPIP_IPADDRM_LOCKIFONLINE_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPADDRM_LOCKIFONLINE_EXIT
/** \brief Exit point of function TcpIp_IpAddrM_lockIfOnline() */
#define DBG_TCPIP_IPADDRM_LOCKIFONLINE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPADDRM_LOCKIFONLINESHUTDOWN_ENTRY
/** \brief Entry point of function TcpIp_IpAddrM_lockIfOnlineShutdown() */
#define DBG_TCPIP_IPADDRM_LOCKIFONLINESHUTDOWN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPADDRM_LOCKIFONLINESHUTDOWN_EXIT
/** \brief Exit point of function TcpIp_IpAddrM_lockIfOnlineShutdown() */
#define DBG_TCPIP_IPADDRM_LOCKIFONLINESHUTDOWN_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPADDRM_LOCKIFSTARTUPONLINE_ENTRY
/** \brief Entry point of function TcpIp_IpAddrM_lockIfStartupOnline() */
#define DBG_TCPIP_IPADDRM_LOCKIFSTARTUPONLINE_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPADDRM_LOCKIFSTARTUPONLINE_EXIT
/** \brief Exit point of function TcpIp_IpAddrM_lockIfStartupOnline() */
#define DBG_TCPIP_IPADDRM_LOCKIFSTARTUPONLINE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPADDRM_LOCKIFSTARTUPONLINESHUTDOWN_ENTRY
/** \brief Entry point of function TcpIp_IpAddrM_lockIfStartupOnlineShutdown() */
#define DBG_TCPIP_IPADDRM_LOCKIFSTARTUPONLINESHUTDOWN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPADDRM_LOCKIFSTARTUPONLINESHUTDOWN_EXIT
/** \brief Exit point of function TcpIp_IpAddrM_lockIfStartupOnlineShutdown() */
#define DBG_TCPIP_IPADDRM_LOCKIFSTARTUPONLINESHUTDOWN_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPADDRM_UNLOCKIF_ENTRY
/** \brief Entry point of function TcpIp_IpAddrM_unlockIf() */
#define DBG_TCPIP_IPADDRM_UNLOCKIF_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPADDRM_UNLOCKIF_EXIT
/** \brief Exit point of function TcpIp_IpAddrM_unlockIf() */
#define DBG_TCPIP_IPADDRM_UNLOCKIF_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPADDRM_GETIPADDRESS_ENTRY
/** \brief Entry point of function TcpIp_IpAddrM_getIpAddress() */
#define DBG_TCPIP_IPADDRM_GETIPADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPADDRM_GETIPADDRESS_EXIT
/** \brief Exit point of function TcpIp_IpAddrM_getIpAddress() */
#define DBG_TCPIP_IPADDRM_GETIPADDRESS_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPADDRM_GETIPCONFIG_ENTRY
/** \brief Entry point of function TcpIp_IpAddrM_getIpConfig() */
#define DBG_TCPIP_IPADDRM_GETIPCONFIG_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPADDRM_GETIPCONFIG_EXIT
/** \brief Exit point of function TcpIp_IpAddrM_getIpConfig() */
#define DBG_TCPIP_IPADDRM_GETIPCONFIG_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPADDRM_GETIFSTATE_ENTRY
/** \brief Entry point of function TcpIp_IpAddrM_getIfState() */
#define DBG_TCPIP_IPADDRM_GETIFSTATE_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPADDRM_GETIFSTATE_EXIT
/** \brief Exit point of function TcpIp_IpAddrM_getIfState() */
#define DBG_TCPIP_IPADDRM_GETIFSTATE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPADDRM_SOCKETSCLOSED_ENTRY
/** \brief Entry point of function TcpIp_IpAddrM_socketsClosed() */
#define DBG_TCPIP_IPADDRM_SOCKETSCLOSED_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPADDRM_SOCKETSCLOSED_EXIT
/** \brief Exit point of function TcpIp_IpAddrM_socketsClosed() */
#define DBG_TCPIP_IPADDRM_SOCKETSCLOSED_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPADDRM_GETIPADDRSTATE_ENTRY
/** \brief Entry point of function TcpIp_IpAddrM_getIpAddrState() */
#define DBG_TCPIP_IPADDRM_GETIPADDRSTATE_ENTRY(p1)
#endif

#ifndef DBG_TCPIP_IPADDRM_GETIPADDRSTATE_EXIT
/** \brief Exit point of function TcpIp_IpAddrM_getIpAddrState() */
#define DBG_TCPIP_IPADDRM_GETIPADDRSTATE_EXIT(retval, p1)
#endif

#ifndef DBG_TCPIP_IPV4_INIT_ENTRY
/** \brief Entry point of function TcpIp_IpV4_init() */
#define DBG_TCPIP_IPV4_INIT_ENTRY()
#endif

#ifndef DBG_TCPIP_IPV4_INIT_EXIT
/** \brief Exit point of function TcpIp_IpV4_init() */
#define DBG_TCPIP_IPV4_INIT_EXIT()
#endif

#ifndef DBG_TCPIP_IPV4_ARPANNOUNCE_ENTRY
/** \brief Entry point of function TcpIp_IpV4_ArpAnnounce() */
#define DBG_TCPIP_IPV4_ARPANNOUNCE_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_IPV4_ARPANNOUNCE_EXIT
/** \brief Exit point of function TcpIp_IpV4_ArpAnnounce() */
#define DBG_TCPIP_IPV4_ARPANNOUNCE_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_IPV4_ARPPROBE_ENTRY
/** \brief Entry point of function TcpIp_IpV4_ArpProbe() */
#define DBG_TCPIP_IPV4_ARPPROBE_ENTRY(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_TCPIP_IPV4_ARPPROBE_EXIT
/** \brief Exit point of function TcpIp_IpV4_ArpProbe() */
#define DBG_TCPIP_IPV4_ARPPROBE_EXIT(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_TCPIP_IPV4_ADDRCONFLDETECT_CONFLICTCALLOUT_ENTRY
/** \brief Entry point of function TcpIp_IpV4_AddrConflDetect_conflictCallout() */
#define DBG_TCPIP_IPV4_ADDRCONFLDETECT_CONFLICTCALLOUT_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_ADDRCONFLDETECT_CONFLICTCALLOUT_EXIT
/** \brief Exit point of function TcpIp_IpV4_AddrConflDetect_conflictCallout() */
#define DBG_TCPIP_IPV4_ADDRCONFLDETECT_CONFLICTCALLOUT_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_ARP_GETREMOTEPHYSADDR_ENTRY
/** \brief Entry point of function TcpIp_IpV4_Arp_getRemotePhysAddr() */
#define DBG_TCPIP_IPV4_ARP_GETREMOTEPHYSADDR_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_ARP_GETREMOTEPHYSADDR_EXIT
/** \brief Exit point of function TcpIp_IpV4_Arp_getRemotePhysAddr() */
#define DBG_TCPIP_IPV4_ARP_GETREMOTEPHYSADDR_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_IPV4_ARP_SETREMOTEPHYSADDR_ENTRY
/** \brief Entry point of function TcpIp_IpV4_Arp_setRemotePhysAddr() */
#define DBG_TCPIP_IPV4_ARP_SETREMOTEPHYSADDR_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_ARP_SETREMOTEPHYSADDR_EXIT
/** \brief Exit point of function TcpIp_IpV4_Arp_setRemotePhysAddr() */
#define DBG_TCPIP_IPV4_ARP_SETREMOTEPHYSADDR_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_IPV4_ARP_GETPHYSADDRPTR_ENTRY
/** \brief Entry point of function TcpIp_IpV4_Arp_getPhysAddrPtr() */
#define DBG_TCPIP_IPV4_ARP_GETPHYSADDRPTR_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_ARP_GETPHYSADDRPTR_EXIT
/** \brief Exit point of function TcpIp_IpV4_Arp_getPhysAddrPtr() */
#define DBG_TCPIP_IPV4_ARP_GETPHYSADDRPTR_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV4_RXINDICATIONIP_ENTRY
/** \brief Entry point of function TcpIp_IpV4_rxIndicationIp() */
#define DBG_TCPIP_IPV4_RXINDICATIONIP_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_IPV4_RXINDICATIONIP_EXIT
/** \brief Exit point of function TcpIp_IpV4_rxIndicationIp() */
#define DBG_TCPIP_IPV4_RXINDICATIONIP_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_TCPIP_IPV4_GETMINIMUMPAYLOAD_ENTRY
/** \brief Entry point of function TcpIp_IpV4_getMinimumPayload() */
#define DBG_TCPIP_IPV4_GETMINIMUMPAYLOAD_ENTRY()
#endif

#ifndef DBG_TCPIP_IPV4_GETMINIMUMPAYLOAD_EXIT
/** \brief Exit point of function TcpIp_IpV4_getMinimumPayload() */
#define DBG_TCPIP_IPV4_GETMINIMUMPAYLOAD_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPV4_GETMAXIMUMPAYLOAD_ENTRY
/** \brief Entry point of function TcpIp_IpV4_getMaximumPayload() */
#define DBG_TCPIP_IPV4_GETMAXIMUMPAYLOAD_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_GETMAXIMUMPAYLOAD_EXIT
/** \brief Exit point of function TcpIp_IpV4_getFragmentSize() */
#define DBG_TCPIP_IPV4_GETMAXIMUMPAYLOAD_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_IPV4_GETFRAGMENTSIZE_ENTRY
/** \brief Entry point of function TcpIp_IpV4_getFragmentSize() */
#define DBG_TCPIP_IPV4_GETFRAGMENTSIZE_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_GETFRAGMENTSIZE_EXIT
/** \brief Exit point of function TcpIp_IpV4_getMaximumPayload() */
#define DBG_TCPIP_IPV4_GETFRAGMENTSIZE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_PROVIDETXBUFFER_ENTRY
/** \brief Entry point of function TcpIp_IpV4_provideTxBuffer() */
#define DBG_TCPIP_IPV4_PROVIDETXBUFFER_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_IPV4_PROVIDETXBUFFER_EXIT
/** \brief Exit point of function TcpIp_IpV4_provideTxBuffer() */
#define DBG_TCPIP_IPV4_PROVIDETXBUFFER_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_TCPIP_IPV4_GETREMOTEADDRHANDLE_ENTRY
/** \brief Entry point of function TcpIp_IpV4_getRemoteAddrHandle() */
#define DBG_TCPIP_IPV4_GETREMOTEADDRHANDLE_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV4_GETREMOTEADDRHANDLE_EXIT
/** \brief Exit point of function TcpIp_IpV4_getRemoteAddrHandle() */
#define DBG_TCPIP_IPV4_GETREMOTEADDRHANDLE_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_TRANSMIT_ENTRY
/** \brief Entry point of function TcpIp_IpV4_transmit() */
#define DBG_TCPIP_IPV4_TRANSMIT_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_TCPIP_IPV4_TRANSMIT_EXIT
/** \brief Exit point of function TcpIp_IpV4_transmit() */
#define DBG_TCPIP_IPV4_TRANSMIT_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_TCPIP_IPV4_UPDATEHEADER_ENTRY
/** \brief Entry point of function TcpIp_IpV4_updateHeader() */
#define DBG_TCPIP_IPV4_UPDATEHEADER_ENTRY(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_TCPIP_IPV4_UPDATEHEADER_EXIT
/** \brief Exit point of function TcpIp_IpV4_updateHeader() */
#define DBG_TCPIP_IPV4_UPDATEHEADER_EXIT(a,b,c,d,e,f,g,h,i)
#endif

#ifndef DBG_TCPIP_IPV4_RXINDICATIONARP_ENTRY
/** \brief Entry point of function TcpIp_IpV4_rxIndicationArp() */
#define DBG_TCPIP_IPV4_RXINDICATIONARP_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_RXINDICATIONARP_EXIT
/** \brief Exit point of function TcpIp_IpV4_rxIndicationArp() */
#define DBG_TCPIP_IPV4_RXINDICATIONARP_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_IPV4_ICMPTRANSMIT_ENTRY
/** \brief Entry point of function TcpIp_IpV4_icmpTransmit() */
#define DBG_TCPIP_IPV4_ICMPTRANSMIT_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_TCPIP_IPV4_ICMPTRANSMIT_EXIT
/** \brief Exit point of function TcpIp_IpV4_icmpTransmit() */
#define DBG_TCPIP_IPV4_ICMPTRANSMIT_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_TCPIP_IPV4_ICMP_UPDATEHEADER_ENTRY
/** \brief Entry point of function TcpIp_IpV4_Icmp_updateHeader() */
#define DBG_TCPIP_IPV4_ICMP_UPDATEHEADER_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_ICMP_UPDATEHEADER_EXIT
/** \brief Exit point of function TcpIp_IpV4_Icmp_updateHeader() */
#define DBG_TCPIP_IPV4_ICMP_UPDATEHEADER_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_MAINFUNCTION_ENTRY
/** \brief Entry point of function TcpIp_IpV4_mainFunction() */
#define DBG_TCPIP_IPV4_MAINFUNCTION_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_MAINFUNCTION_EXIT
/** \brief Exit point of function TcpIp_IpV4_mainFunction() */
#define DBG_TCPIP_IPV4_MAINFUNCTION_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPV4_GETTXLOCALADDRID_BYREMOTEIP_ENTRY
/** \brief Entry point of function TcpIp_IpV4_getTxLocalAddrId_byRemoteIp() */
#define DBG_TCPIP_IPV4_GETTXLOCALADDRID_BYREMOTEIP_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_GETTXLOCALADDRID_BYREMOTEIP_EXIT
/** \brief Exit point of function TcpIp_IpV4_getTxLocalAddrId_byRemoteIp() */
#define DBG_TCPIP_IPV4_GETTXLOCALADDRID_BYREMOTEIP_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_GETTXLOCALADDRID_BYCTRLIDX_ENTRY
/** \brief Entry point of function TcpIp_IpV4_getTxLocalAddrId_byCtrlIdx() */
#define DBG_TCPIP_IPV4_GETTXLOCALADDRID_BYCTRLIDX_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_GETTXLOCALADDRID_BYCTRLIDX_EXIT
/** \brief Exit point of function TcpIp_IpV4_getTxLocalAddrId_byCtrlIdx() */
#define DBG_TCPIP_IPV4_GETTXLOCALADDRID_BYCTRLIDX_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV4_GETIFSTATE_ENTRY
/** \brief Entry point of function TcpIp_IpV4_getIfState() */
#define DBG_TCPIP_IPV4_GETIFSTATE_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_GETIFSTATE_EXIT
/** \brief Exit point of function TcpIp_IpV4_getIfState() */
#define DBG_TCPIP_IPV4_GETIFSTATE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_REACHCONFIRMATION_ENTRY
/** \brief Entry point of function TcpIp_IpV4_reachConfirmation() */
#define DBG_TCPIP_IPV4_REACHCONFIRMATION_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_REACHCONFIRMATION_EXIT
/** \brief Exit point of function TcpIp_IpV4_reachConfirmation() */
#define DBG_TCPIP_IPV4_REACHCONFIRMATION_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_ISVALIDTCPREMOTEADDRESS_ENTRY
/** \brief Entry point of function TcpIp_IpV4_isValidTcpRemoteAddress() */
#define DBG_TCPIP_IPV4_ISVALIDTCPREMOTEADDRESS_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_ISVALIDTCPREMOTEADDRESS_EXIT
/** \brief Exit point of function TcpIp_IpV4_isValidTcpRemoteAddress() */
#define DBG_TCPIP_IPV4_ISVALIDTCPREMOTEADDRESS_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_GETREMOTEADDRHANDLE_ENTRY
/** \brief Entry point of function TcpIp_IpV4_getRemoteAddrHandle() */
#define DBG_TCPIP_IPV4_GETREMOTEADDRHANDLE_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV4_GETREMOTEADDRHANDLE_EXIT
/** \brief Exit point of function TcpIp_IpV4_getRemoteAddrHandle() */
#define DBG_TCPIP_IPV4_GETREMOTEADDRHANDLE_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_ADDRCONFLDETECT_ACTIVATE_ENTRY
/** \brief Entry point of function TcpIp_IpV4_AddrConflDetect_activate() */
#define DBG_TCPIP_IPV4_ADDRCONFLDETECT_ACTIVATE_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV4_ADDRCONFLDETECT_ACTIVATE_EXIT
/** \brief Exit point of function TcpIp_IpV4_AddrConflDetect_activate() */
#define DBG_TCPIP_IPV4_ADDRCONFLDETECT_ACTIVATE_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_UDP_UPDATEHEADER_ENTRY
/** \brief Entry point of function TcpIp_Udp_UpdateHeader() */
#define DBG_TCPIP_UDP_UPDATEHEADER_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_UDP_UPDATEHEADER_EXIT
/** \brief Exit point of function TcpIp_Udp_UpdateHeader() */
#define DBG_TCPIP_UDP_UPDATEHEADER_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_UDP_COPYDATA_ENTRY
/** \brief Entry point of function TcpIp_Udp_copyData() */
#define DBG_TCPIP_UDP_COPYDATA_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_UDP_COPYDATA_EXIT
/** \brief Exit point of function TcpIp_Udp_copyData() */
#define DBG_TCPIP_UDP_COPYDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_UDP_RXINDICATION_ENTRY
/** \brief Entry point of function TcpIp_Udp_rxIndication() */
#define DBG_TCPIP_UDP_RXINDICATION_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_TCPIP_UDP_RXINDICATION_EXIT
/** \brief Exit point of function TcpIp_Udp_rxIndication() */
#define DBG_TCPIP_UDP_RXINDICATION_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_TCPIP_UDP_CLOSE_ENTRY
/** \brief Entry point of function TcpIp_Udp_close() */
#define DBG_TCPIP_UDP_CLOSE_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_UDP_CLOSE_EXIT
/** \brief Exit point of function TcpIp_Udp_close() */
#define DBG_TCPIP_UDP_CLOSE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_UDP_CHANGESOCKETTODEFAULT_ENTRY
/** \brief Entry point of function TcpIp_Udp_changeSocketToDefault() */
#define DBG_TCPIP_UDP_CHANGESOCKETTODEFAULT_ENTRY(a)
#endif

#ifndef DBG_TCPIP_UDP_CHANGEPARAMETER_ENTRY
/** \brief Entry point of function TcpIp_Udp_changeParameter() */
#define DBG_TCPIP_UDP_CHANGEPARAMETER_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_UDP_CHANGEPARAMETER_EXIT
/** \brief Exit point of function TcpIp_Udp_changeParameter() */
#define DBG_TCPIP_UDP_CHANGEPARAMETER_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_UDP_CHANGESOCKETTODEFAULT_EXIT
/** \brief Exit point of function TcpIp_Udp_changeSocketToDefault() */
#define DBG_TCPIP_UDP_CHANGESOCKETTODEFAULT_EXIT(a)
#endif

#ifndef DBG_TCPIP_UDP_GETANDRESETINVALIDSOCKETCOUNT_ENTRY
/** \brief Entry point of function TcpIp_Udp_GetAndResetInvalidSocketCount() */
#define DBG_TCPIP_UDP_GETANDRESETINVALIDSOCKETCOUNT_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_UDP_GETANDRESETINVALIDSOCKETCOUNT_EXIT
/** \brief Exit point of function TcpIp_Udp_GetAndResetInvalidSocketCount() */
#define DBG_TCPIP_UDP_GETANDRESETINVALIDSOCKETCOUNT_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_UDP_GETPORTS_ENTRY
/** \brief Entry point of function TcpIp_Udp_getPort() */
#define DBG_TCPIP_UDP_GETPORTS_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_UDP_GETPORTS_EXIT
/** \brief Entry point of function TcpIp_Udp_getPort() */
#define DBG_TCPIP_UDP_GETPORTS_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_SOCKET_CHECKSOCKETPROTOCOLFCT_ENTRY
/** \brief Entry point of function TcpIp_Socket_checkSocketProtocolFct() */
#define DBG_TCPIP_SOCKET_CHECKSOCKETPROTOCOLFCT_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_SOCKET_CHECKSOCKETPROTOCOLFCT_EXIT
/** \brief Exit point of function TcpIp_Socket_checkSocketProtocolFct() */
#define DBG_TCPIP_SOCKET_CHECKSOCKETPROTOCOLFCT_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_SOCKET_CHECKSOCKETID_ENTRY
/** \brief Entry point of function TcpIp_Socket_checkSocketId() */
#define DBG_TCPIP_SOCKET_CHECKSOCKETID_ENTRY(a)
#endif

#ifndef DBG_TCPIP_SOCKET_CHECKSOCKETID_EXIT
/** \brief Exit point of function TcpIp_Socket_checkSocketId() */
#define DBG_TCPIP_SOCKET_CHECKSOCKETID_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_SOCKET_CHECKDOMAIN_ENTRY
/** \brief Entry point of function TcpIp_Socket_checkDomain() */
#define DBG_TCPIP_SOCKET_CHECKDOMAIN_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_SOCKET_CHECKDOMAIN_EXIT
/** \brief Exit point of function TcpIp_Socket_checkDomain() */
#define DBG_TCPIP_SOCKET_CHECKDOMAIN_EXIT(a,b,c)
#endif


#ifndef DBG_TCPIP_SOCKET_INIT_ENTRY
/** \brief Entry point of function TcpIp_Socket_init() */
#define DBG_TCPIP_SOCKET_INIT_ENTRY()
#endif

#ifndef DBG_TCPIP_SOCKET_INIT_EXIT
/** \brief Exit point of function TcpIp_Socket_init() */
#define DBG_TCPIP_SOCKET_INIT_EXIT()
#endif

#ifndef DBG_TCPIP_SOCKET_AUTOBINDUDP_ENTRY
/** \brief Entry point of function TcpIp_Socket_autoBindUdp() */
#define DBG_TCPIP_SOCKET_AUTOBINDUDP_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_SOCKET_AUTOBINDUDP_EXIT
/** \brief Exit point of function TcpIp_Socket_autoBindUdp() */
#define DBG_TCPIP_SOCKET_AUTOBINDUDP_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_SOCKET_LOOKUPUDP_ENTRY
/** \brief Entry point of function TcpIp_Socket_lookupUdp() */
#define DBG_TCPIP_SOCKET_LOOKUPUDP_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_TCPIP_SOCKET_LOOKUPUDP_EXIT
/** \brief Exit point of function TcpIp_Socket_lookupUdp() */
#define DBG_TCPIP_SOCKET_LOOKUPUDP_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_TCPIP_SOCKET_AUTOBINDTCP_ENTRY
/** \brief Entry point of function TcpIp_Socket_autoBindTcp() */
#define DBG_TCPIP_SOCKET_AUTOBINDTCP_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_SOCKET_AUTOBINDTCP_EXIT
/** \brief Exit point of function TcpIp_Socket_autoBindTcp() */
#define DBG_TCPIP_SOCKET_AUTOBINDTCP_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_SOCKET_ACTIVATETCP_ENTRY
/** \brief Entry point of function TcpIp_Socket_activateTcp() */
#define DBG_TCPIP_SOCKET_ACTIVATETCP_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_SOCKET_ACTIVATETCP_EXIT
/** \brief Exit point of function TcpIp_Socket_activateTcp() */
#define DBG_TCPIP_SOCKET_ACTIVATETCP_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_SOCKET_LOOKUPTCP_ENTRY
/** \brief Entry point of function TcpIp_Socket_lookupTcp() */
#define DBG_TCPIP_SOCKET_LOOKUPTCP_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_SOCKET_LOOKUPTCP_EXIT
/** \brief Exit point of function TcpIp_Socket_lookupTcp() */
#define DBG_TCPIP_SOCKET_LOOKUPTCP_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_SOCKET_TCP_CLONELISTENSOCKET_ENTRY
/** \brief Entry point of function TcpIp_Socket_Tcp_cloneListenSocket() */
#define DBG_TCPIP_SOCKET_TCP_CLONELISTENSOCKET_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_SOCKET_TCP_CLONELISTENSOCKET_EXIT
/** \brief Exit point of function TcpIp_Socket_Tcp_cloneListenSocket() */
#define DBG_TCPIP_SOCKET_TCP_CLONELISTENSOCKET_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_TCPIP_SOCKET_TCP_ESTABLISHCONNECTION_ENTRY
/** \brief Entry point of function TcpIp_Socket_Tcp_establishConnection() */
#define DBG_TCPIP_SOCKET_TCP_ESTABLISHCONNECTION_ENTRY(a)
#endif

#ifndef DBG_TCPIP_SOCKET_TCP_ESTABLISHCONNECTION_EXIT
/** \brief Exit point of function TcpIp_Socket_Tcp_establishConnection() */
#define DBG_TCPIP_SOCKET_TCP_ESTABLISHCONNECTION_EXIT(a)
#endif

#ifndef DBG_TCPIP_SOCKET_TCP_SETHASHDATA_ENTRY
/** \brief Entry point of function TcpIp_Socket_Tcp_setHashData() */
#define DBG_TCPIP_SOCKET_TCP_SETHASHDATA_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_SOCKET_TCP_SETHASHDATA_EXIT
/** \brief Exit point of function TcpIp_Socket_Tcp_setHashData() */
#define DBG_TCPIP_SOCKET_TCP_SETHASHDATA_EXIT(a,b,c,d,e)
#endif


#ifndef DBG_TCPIP_SOCKET_GETLISTENSOCKET_ENTRY
/** \brief Entry point of function TcpIp_Socket_getListenSocket() */
#define DBG_TCPIP_SOCKET_GETLISTENSOCKET_ENTRY(a)
#endif

#ifndef DBG_TCPIP_SOCKET_GETLISTENSOCKET_EXIT
/** \brief Exit point of function TcpIp_Socket_getListenSocket() */
#define DBG_TCPIP_SOCKET_GETLISTENSOCKET_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_SOCKET_CLOSETCP_ENTRY
/** \brief Entry point of function TcpIp_Socket_closeTcp() */
#define DBG_TCPIP_SOCKET_CLOSETCP_ENTRY(a)
#endif

#ifndef DBG_TCPIP_SOCKET_CLOSETCP_EXIT
/** \brief Exit point of function TcpIp_Socket_closeTcp() */
#define DBG_TCPIP_SOCKET_CLOSETCP_EXIT(a)
#endif

#ifndef DBG_TCPIP_SOCKET_MAINFUNCTION_ENTRY
/** \brief Entry point of function TcpIp_Socket_mainFunction() */
#define DBG_TCPIP_SOCKET_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_TCPIP_SOCKET_MAINFUNCTION_EXIT
/** \brief Exit point of function TcpIp_Socket_mainFunction() */
#define DBG_TCPIP_SOCKET_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_TCPIP_SOCKET_MAINFUNCTIONTX_ENTRY
/** \brief Entry point of function TcpIp_Socket_mainFunctionTx() */
#define DBG_TCPIP_SOCKET_MAINFUNCTIONTX_ENTRY()
#endif

#ifndef DBG_TCPIP_SOCKET_MAINFUNCTIONTX_EXIT
/** \brief Exit point of function TcpIp_Socket_mainFunctionTx() */
#define DBG_TCPIP_SOCKET_MAINFUNCTIONTX_EXIT()
#endif

#ifndef DBG_TCPIP_SOCKET_GETSOCKETPTR_ENTRY
/** \brief Entry point of function TcpIp_Socket_getSocketPtr() */
#define DBG_TCPIP_SOCKET_GETSOCKETPTR_ENTRY(a)
#endif

#ifndef DBG_TCPIP_SOCKET_GETSOCKETPTR_EXIT
/** \brief Exit point of function TcpIp_Socket_getSocketPtr() */
#define DBG_TCPIP_SOCKET_GETSOCKETPTR_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_SOCKET_CLOSESOCKETS_ENTRY
/** \brief Entry point of function TcpIp_Socket_closeSockets() */
#define DBG_TCPIP_SOCKET_CLOSESOCKETS_ENTRY(a)
#endif

#ifndef DBG_TCPIP_SOCKET_CLOSESOCKETS_EXIT
/** \brief Exit point of function TcpIp_Socket_closeSockets() */
#define DBG_TCPIP_SOCKET_CLOSESOCKETS_EXIT(a)
#endif

#ifndef DBG_TCPIP_SOCKET_TERMINATESOCKETS_ENTRY
/** \brief Entry point of function TcpIp_Socket_terminateSockets() */
#define DBG_TCPIP_SOCKET_TERMINATESOCKETS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_SOCKET_TERMINATESOCKETS_EXIT
/** \brief Exit point of function TcpIp_Socket_terminateSockets() */
#define DBG_TCPIP_SOCKET_TERMINATESOCKETS_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function TcpIp_IsValidConfig() */
#define DBG_TCPIP_ISVALIDCONFIG_ENTRY(a)
#endif

#ifndef DBG_TCPIP_ISVALIDCONFIG_EXIT
/** \brief Exit point of function TcpIp_IsValidConfig() */
#define DBG_TCPIP_ISVALIDCONFIG_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_SOCKET_LISTENTCP_ENTRY
/** \brief Entry point of function TcpIp_Socket_ListenTcp() */
#define DBG_TCPIP_SOCKET_LISTENTCP_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_SOCKET_LISTENTCP_EXIT
/** \brief Exit point of function TcpIp_Socket_ListenTcp() */
#define DBG_TCPIP_SOCKET_LISTENTCP_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_TCP_INIT_ENTRY
/** \brief Entry point of function TcpIp_Tcp_init() */
#define DBG_TCPIP_TCP_INIT_ENTRY()
#endif

#ifndef DBG_TCPIP_TCP_INIT_EXIT
/** \brief Exit point of function TcpIp_Tcp_init() */
#define DBG_TCPIP_TCP_INIT_EXIT()
#endif

#ifndef DBG_TCPIP_TCP_COPYDATA_ENTRY
/** \brief Entry point of function TcpIp_Tcp_copyData() */
#define DBG_TCPIP_TCP_COPYDATA_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_TCP_COPYDATA_EXIT
/** \brief Exit point of function TcpIp_Tcp_copyData() */
#define DBG_TCPIP_TCP_COPYDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_TCP_UPDATEHEADER_ENTRY
/** \brief Entry point of function TcpIp_Tcp_updateHeader() */
#define DBG_TCPIP_TCP_UPDATEHEADER_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_TCP_UPDATEHEADER_EXIT
/** \brief Exit point of function TcpIp_Tcp_updateHeader() */
#define DBG_TCPIP_TCP_UPDATEHEADER_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_TCP_RXINDICATION_ENTRY
/** \brief Entry point of function TcpIp_Tcp_rxIndication() */
#define DBG_TCPIP_TCP_RXINDICATION_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_TCPIP_TCP_RXINDICATION_EXIT
/** \brief Exit point of function TcpIp_Tcp_rxIndication() */
#define DBG_TCPIP_TCP_RXINDICATION_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_TCPIP_TCP_RX_GETPORT_ENTRY
/** \brief Entry point of function TcpIp_Tcp_rx_getPort() */
#define DBG_TCPIP_TCP_RX_GETPORT_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_TCP_RX_GETPORT_EXIT
/** \brief Exit point of function TcpIp_Tcp_rx_getPort() */
#define DBG_TCPIP_TCP_RX_GETPORT_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_TCP_CHANGESOCKETTODEFAULT_ENTRY
/** \brief Entry point of function TcpIp_Tcp_changeSocketToDefault() */
#define DBG_TCPIP_TCP_CHANGESOCKETTODEFAULT_ENTRY(a)
#endif

#ifndef DBG_TCPIP_TCP_CHANGESOCKETTODEFAULT_EXIT
/** \brief Exit point of function TcpIp_Tcp_changeSocketToDefault() */
#define DBG_TCPIP_TCP_CHANGESOCKETTODEFAULT_EXIT(a)
#endif

#ifndef DBG_TCPIP_TCP_PROCESSTCPSOCKET_ENTRY
/** \brief Entry point of function TcpIp_Tcp_processTcpSocket() */
#define DBG_TCPIP_TCP_PROCESSTCPSOCKET_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_TCP_PROCESSTCPSOCKET_EXIT
/** \brief Exit point of function TcpIp_Tcp_processTcpSocket() */
#define DBG_TCPIP_TCP_PROCESSTCPSOCKET_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_TCP_PROCESSTCPSOCKETTX_ENTRY
/** \brief Entry point of function TcpIp_Tcp_processTcpSocketTx() */
#define DBG_TCPIP_TCP_PROCESSTCPSOCKETTX_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_TCP_PROCESSTCPSOCKETTX_EXIT
/** \brief Exit point of function TcpIp_Tcp_processTcpSocketTx() */
#define DBG_TCPIP_TCP_PROCESSTCPSOCKETTX_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_TCP_CLOSE_ENTRY
/** \brief Entry point of function TcpIp_Tcp_close() */
#define DBG_TCPIP_TCP_CLOSE_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_TCP_CLOSE_EXIT
/** \brief Exit point of function TcpIp_Tcp_close() */
#define DBG_TCPIP_TCP_CLOSE_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_TCP_ABORT_ENTRY
/** \brief Entry point of function TcpIp_Tcp_abort() */
#define DBG_TCPIP_TCP_ABORT_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_TCP_ABORT_EXIT
/** \brief Exit point of function TcpIp_Tcp_abort() */
#define DBG_TCPIP_TCP_ABORT_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_TCP_TERMINATE_ENTRY
/** \brief Entry point of function TcpIp_Tcp_terminate() */
#define DBG_TCPIP_TCP_TERMINATE_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_TCP_TERMINATE_EXIT
/** \brief Exit point of function TcpIp_Tcp_terminate() */
#define DBG_TCPIP_TCP_TERMINATE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_TCP_MAINFUNCTION_ENTRY
/** \brief Entry point of function TcpIp_Tcp_mainfunction() */
#define DBG_TCPIP_TCP_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_TCPIP_TCP_MAINFUNCTION_EXIT
/** \brief Exit point of function TcpIp_Tcp_mainfunction() */
#define DBG_TCPIP_TCP_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_TCPIP_TCP_CHANGEPARAMETER_ENTRY
/** \brief Entry point of function TcpIp_Tcp_changeParameter() */
#define DBG_TCPIP_TCP_CHANGEPARAMETER_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_TCP_CHANGEPARAMETER_EXIT
/** \brief Exit point of function TcpIp_Tcp_changeParameter() */
#define DBG_TCPIP_TCP_CHANGEPARAMETER_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_TCP_GETANDRESETINVALIDSOCKETCOUNT_ENTRY
/** \brief Entry point of function TcpIp_Tcp_GetAndResetInvalidSocketCount() */
#define DBG_TCPIP_TCP_GETANDRESETINVALIDSOCKETCOUNT_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_TCP_GETANDRESETINVALIDSOCKETCOUNT_EXIT
/** \brief Exit point of function TcpIp_Tcp_GetAndResetInvalidSocketCount() */
#define DBG_TCPIP_TCP_GETANDRESETINVALIDSOCKETCOUNT_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_DHCPV4_INIT_ENTRY
/** \brief Entry point of function TcpIp_DhcpV4_init() */
#define DBG_TCPIP_DHCPV4_INIT_ENTRY(a)
#endif

#ifndef DBG_TCPIP_DHCPV4_INIT_EXIT
/** \brief Exit point of function TcpIp_DhcpV4_init() */
#define DBG_TCPIP_DHCPV4_INIT_EXIT(a)
#endif

#ifndef DBG_TCPIP_DHCPV4_STARTASSIGN_ENTRY
/** \brief Entry point of function TcpIp_DhcpV4_startAssign() */
#define DBG_TCPIP_DHCPV4_STARTASSIGN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_DHCPV4_STARTASSIGN_EXIT
/** \brief Exit point of function TcpIp_DhcpV4_startAssign() */
#define DBG_TCPIP_DHCPV4_STARTASSIGN_EXIT(a)
#endif

#ifndef DBG_TCPIP_DHCPV4_STOPASSIGN_ENTRY
/** \brief Entry point of function TcpIp_DhcpV4_stopAssign() */
#define DBG_TCPIP_DHCPV4_STOPASSIGN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_DHCPV4_STOPASSIGN_EXIT
/** \brief Exit point of function TcpIp_DhcpV4_stopAssign() */
#define DBG_TCPIP_DHCPV4_STOPASSIGN_EXIT(a)
#endif

#ifndef DBG_TCPIP_DHCPV4_RXINDICATION_ENTRY
/** \brief Entry point of function TcpIp_DhcpV4_rxIndication() */
#define DBG_TCPIP_DHCPV4_RXINDICATION_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_DHCPV4_RXINDICATION_EXIT
/** \brief Exit point of function TcpIp_DhcpV4_rxIndication() */
#define DBG_TCPIP_DHCPV4_RXINDICATION_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_DHCPV4_LOCALIPADDRASSIGNMENTCHG_ENTRY
/** \brief Entry point of function TcpIp_DhcpV4_LocalIpAddrAssignmentChg() */
#define DBG_TCPIP_DHCPV4_LOCALIPADDRASSIGNMENTCHG_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_DHCPV4_LOCALIPADDRASSIGNMENTCHG_EXIT
/** \brief Exit point of function TcpIp_DhcpV4_LocalIpAddrAssignmentChg() */
#define DBG_TCPIP_DHCPV4_LOCALIPADDRASSIGNMENTCHG_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_DHCPV4_COPYTXDATA_ENTRY
/** \brief Entry point of function TcpIp_DhcpV4_CopyTxData() */
#define DBG_TCPIP_DHCPV4_COPYTXDATA_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_DHCPV4_COPYTXDATA_EXIT
/** \brief Exit point of function TcpIp_DhcpV4_CopyTxData() */
#define DBG_TCPIP_DHCPV4_COPYTXDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_DHCPV4_MAINFUNCTION_ENTRY
/** \brief Entry point of function TcpIp_DhcpV4_mainfunction() */
#define DBG_TCPIP_DHCPV4_MAINFUNCTION_ENTRY(a)
#endif

#ifndef DBG_TCPIP_DHCPV4_MAINFUNCTION_EXIT
/** \brief Exit point of function TcpIp_DhcpV4_mainfunction() */
#define DBG_TCPIP_DHCPV4_MAINFUNCTION_EXIT(a)
#endif

#ifndef DBG_TCPIP_DHCPV4_GETIPCONFIG_ENTRY
/** \brief Entry point of function TcpIp_DhcpV4_getIpConfig() */
#define DBG_TCPIP_DHCPV4_GETIPCONFIG_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_DHCPV4_GETIPCONFIG_EXIT
/** \brief Exit point of function TcpIp_DhcpV4_getIpConfig() */
#define DBG_TCPIP_DHCPV4_GETIPCONFIG_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_DHCPV4_GETIPADDRESS_ENTRY
/** \brief Entry point of function TcpIp_DhcpV4_getIpAddress() */
#define DBG_TCPIP_DHCPV4_GETIPADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_DHCPV4_GETIPADDRESS_EXIT
/** \brief Exit point of function TcpIp_DhcpV4_getIpAddress() */
#define DBG_TCPIP_DHCPV4_GETIPADDRESS_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_DHCPV4_ISASSIGNED_ENTRY
/** \brief Entry point of function TcpIp_DhcpV4_isAssigned() */
#define DBG_TCPIP_DHCPV4_ISASSIGNED_ENTRY(a)
#endif

#ifndef DBG_TCPIP_DHCPV4_ISASSIGNED_EXIT
/** \brief Exit point of function TcpIp_DhcpV4_isAssigned() */
#define DBG_TCPIP_DHCPV4_ISASSIGNED_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_DHCPV4_ASSIGNMENTACTIVE_ENTRY
/** \brief Entry point of function TcpIp_DhcpV4_assignmentActive() */
#define DBG_TCPIP_DHCPV4_ASSIGNMENTACTIVE_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_DHCPV4_ASSIGNMENTACTIVE_EXIT
/** \brief Exit point of function TcpIp_DhcpV4_assignmentActive() */
#define DBG_TCPIP_DHCPV4_ASSIGNMENTACTIVE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_DHCPV4_GETSTATUS_ENTRY
/** \brief Entry point of function TcpIp_DhcpV4_getStatus() */
#define DBG_TCPIP_DHCPV4_GETSTATUS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_DHCPV4_GETSTATUS_EXIT
/** \brief Exit point of function TcpIp_DhcpV4_getStatus() */
#define DBG_TCPIP_DHCPV4_GETSTATUS_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_INIT_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_AUTOIP_INIT_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_INIT_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_AUTOIP_INIT_EXIT(a)
#endif


#ifndef DBG_TCPIP_IPV4_AUTOIP_DOIP_INIT_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_AUTOIP_DOIP_INIT_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_DOIP_INIT_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_AUTOIP_DOIP_INIT_EXIT(a)
#endif


#ifndef DBG_TCPIP_IPV4_AUTOIP_STARTASSIGN_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_AUTOIP_STARTASSIGN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_STARTASSIGN_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_AUTOIP_STARTASSIGN_EXIT(a)
#endif


#ifndef DBG_TCPIP_IPV4_AUTOIP_DOIP_STARTASSIGN_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_AUTOIP_DOIP_STARTASSIGN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_DOIP_STARTASSIGN_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_AUTOIP_DOIP_STARTASSIGN_EXIT(a)
#endif


#ifndef DBG_TCPIP_IPV4_AUTOIP_STOPASSIGN_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_AUTOIP_STOPASSIGN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_STOPASSIGN_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_AUTOIP_STOPASSIGN_EXIT(a)
#endif


#ifndef DBG_TCPIP_IPV4_AUTOIP_DOIP_STOPASSIGN_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_AUTOIP_DOIP_STOPASSIGN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_DOIP_STOPASSIGN_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_AUTOIP_DOIP_STOPASSIGN_EXIT(a)
#endif


#ifndef DBG_TCPIP_IPV4_AUTOIP_ASSIGNMENTACTIVE_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_AUTOIP_ASSIGNMENTACTIVE_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_ASSIGNMENTACTIVE_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_AUTOIP_ASSIGNMENTACTIVE_EXIT(a,b)
#endif


#ifndef DBG_TCPIP_IPV4_AUTOIP_DOIP_ASSIGNMENTACTIVE_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_AUTOIP_DOIP_ASSIGNMENTACTIVE_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_DOIP_ASSIGNMENTACTIVE_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_AUTOIP_DOIP_ASSIGNMENTACTIVE_EXIT(a,b)
#endif


#ifndef DBG_TCPIP_IPV4_AUTOIP_ISASSIGNED_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_AUTOIP_ISASSIGNED_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_ISASSIGNED_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_AUTOIP_ISASSIGNED_EXIT(a,b)
#endif


#ifndef DBG_TCPIP_IPV4_AUTOIP_DOIP_ISASSIGNED_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_AUTOIP_DOIP_ISASSIGNED_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_DOIP_ISASSIGNED_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_AUTOIP_DOIP_ISASSIGNED_EXIT(a,b)
#endif


#ifndef DBG_TCPIP_IPV4_AUTOIP_GETIPCONFIG_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_AUTOIP_GETIPCONFIG_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_GETIPCONFIG_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_AUTOIP_GETIPCONFIG_EXIT(a,b,c,d)
#endif


#ifndef DBG_TCPIP_IPV4_AUTOIP_DOIP_GETIPCONFIG_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_AUTOIP_DOIP_GETIPCONFIG_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_DOIP_GETIPCONFIG_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_AUTOIP_DOIP_GETIPCONFIG_EXIT(a,b,c,d)
#endif


#ifndef DBG_TCPIP_IPV4_AUTOIP_GETIPADDRESS_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_AUTOIP_GETIPADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_GETIPADDRESS_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_AUTOIP_GETIPADDRESS_EXIT(a,b)
#endif


#ifndef DBG_TCPIP_IPV4_AUTOIP_DOIP_GETIPADDRESS_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_AUTOIP_DOIP_GETIPADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_AUTOIP_DOIP_GETIPADDRESS_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_AUTOIP_DOIP_GETIPADDRESS_EXIT(a,b)
#endif


#ifndef DBG_TCPIP_IPV4_LINKLOCAL_MAINFUNCTION_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPV4_LINKLOCAL_MAINFUNCTION_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_LINKLOCAL_MAINFUNCTION_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPV4_LINKLOCAL_MAINFUNCTION_EXIT(a)
#endif


#ifndef DBG_TCPIP_IPV4_STATICIP_INIT_ENTRY
/** \brief Entry point of function TcpIp_IpV4_StaticIp_init() */
#define DBG_TCPIP_IPV4_STATICIP_INIT_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_INIT_EXIT
/** \brief Exit point of function TcpIp_IpV4_StaticIp_init() */
#define DBG_TCPIP_IPV4_STATICIP_INIT_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_STARTASSIGN_ENTRY
/** \brief Entry point of function TcpIp_IpV4_StaticIp_startAssign() */
#define DBG_TCPIP_IPV4_STATICIP_STARTASSIGN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_STARTASSIGN_EXIT
/** \brief Exit point of function TcpIp_IpV4_StaticIp_startAssign() */
#define DBG_TCPIP_IPV4_STATICIP_STARTASSIGN_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_STOPASSIGN_ENTRY
/** \brief Entry point of function TcpIp_IpV4_StaticIp_stopAssign() */
#define DBG_TCPIP_IPV4_STATICIP_STOPASSIGN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_STOPASSIGN_EXIT
/** \brief Exit point of function TcpIp_IpV4_StaticIp_stopAssign() */
#define DBG_TCPIP_IPV4_STATICIP_STOPASSIGN_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_GETIPCONFIG_ENTRY
/** \brief Entry point of function TcpIp_IpV4_StaticIp_getIpConfig() */
#define DBG_TCPIP_IPV4_STATICIP_GETIPCONFIG_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_GETIPCONFIG_EXIT
/** \brief Exit point of function TcpIp_IpV4_StaticIp_getIpConfig() */
#define DBG_TCPIP_IPV4_STATICIP_GETIPCONFIG_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_REQUESTIPADDRESS_ENTRY
/** \brief Entry point of function TcpIp_IpV4_StaticIp_requestIpAddress() */
#define DBG_TCPIP_IPV4_STATICIP_REQUESTIPADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_REQUESTIPADDRESS_EXIT
/** \brief Exit point of function TcpIp_IpV4_StaticIp_requestIpAddress() */
#define DBG_TCPIP_IPV4_STATICIP_REQUESTIPADDRESS_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_GETIPADDRESS_ENTRY
/** \brief Entry point of function TcpIp_IpV4_StaticIp_getIpAddress() */
#define DBG_TCPIP_IPV4_STATICIP_GETIPADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_GETIPADDRESS_EXIT
/** \brief Exit point of function TcpIp_IpV4_StaticIp_getIpAddress() */
#define DBG_TCPIP_IPV4_STATICIP_GETIPADDRESS_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_ISASSIGNED_ENTRY
/** \brief Entry point of function TcpIp_IpV4_StaticIp_isAssigned() */
#define DBG_TCPIP_IPV4_STATICIP_ISASSIGNED_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_ISASSIGNED_EXIT
/** \brief Exit point of function TcpIp_IpV4_StaticIp_isAssigned() */
#define DBG_TCPIP_IPV4_STATICIP_ISASSIGNED_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_ASSIGNMENTACTIVE_ENTRY
/** \brief Entry point of function TcpIp_IpV4_StaticIp_assignmentActive() */
#define DBG_TCPIP_IPV4_STATICIP_ASSIGNMENTACTIVE_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_STATICIP_ASSIGNMENTACTIVE_EXIT
/** \brief Exit point of function TcpIp_IpV4_StaticIp_assignmentActive() */
#define DBG_TCPIP_IPV4_STATICIP_ASSIGNMENTACTIVE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPV4_GETANDRESETCOUNTER_ENTRY
/** \brief Entry point of function TcpIp_IpV4_GetAndResetCounter() */
#define DBG_TCPIP_IPV4_GETANDRESETCOUNTER_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV4_GETANDRESETCOUNTER_EXIT
/** \brief Exit point of function TcpIp_IpV4_GetAndResetCounter() */
#define DBG_TCPIP_IPV4_GETANDRESETCOUNTER_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV4_ISADDRLIMBROADCAST_ENTRY
/** \brief Entry point of function TcpIp_IpV4_isAddrLimBroadcast() */
#define DBG_TCPIP_IPV4_ISADDRLIMBROADCAST_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV4_ISADDRLIMBROADCAST_EXIT
/** \brief Exit point of function TcpIp_IpV4_isAddrLimBroadcast() */
#define DBG_TCPIP_IPV4_ISADDRLIMBROADCAST_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_GETCTRLADDRRANGE_ENTRY
/** \brief Entry point of function TcpIp_IpV4_getCtrlAddrRange() */
#define DBG_TCPIP_IPV4_GETCTRLADDRRANGE_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV4_GETCTRLADDRRANGE_EXIT
/** \brief Exit point of function TcpIp_IpV4_getCtrlAddrRange() */
#define DBG_TCPIP_IPV4_GETCTRLADDRRANGE_EXIT(a,b,c)
#endif


#ifndef DBG_TCPIP_IPADDRM_ASSIGNSM_ASSIGNED_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPADDRM_ASSIGNSM_ASSIGNED_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPADDRM_ASSIGNSM_ASSIGNED_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPADDRM_ASSIGNSM_ASSIGNED_EXIT(a,b)
#endif


#ifndef DBG_TCPIP_IPADDRM_ASSIGNSM_UNASSIGNED_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IPADDRM_ASSIGNSM_UNASSIGNED_ENTRY(a,b)
#endif


#ifndef DBG_TCPIP_IPADDRM_ASSIGNSM_UNASSIGNED_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IPADDRM_ASSIGNSM_UNASSIGNED_EXIT(a,b)
#endif


#ifndef DBG_TCPIP_MEMORY_INIT_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_MEMORY_INIT_ENTRY()
#endif


#ifndef DBG_TCPIP_MEMORY_INIT_EXIT
/** \brief Exit point */
#define DBG_TCPIP_MEMORY_INIT_EXIT()
#endif


#ifndef DBG_TCPIP_MEMORY_NEW_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_MEMORY_NEW_ENTRY(a)
#endif


#ifndef DBG_TCPIP_MEMORY_NEW_EXIT
/** \brief Exit point */
#define DBG_TCPIP_MEMORY_NEW_EXIT(a,b)
#endif


#ifndef DBG_TCPIP_MEMORY_GETLINEARMEMORYADDRESS_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_MEMORY_GETLINEARMEMORYADDRESS_ENTRY(a,b,c)
#endif


#ifndef DBG_TCPIP_MEMORY_GETLINEARMEMORYADDRESS_EXIT
/** \brief Exit point */
#define DBG_TCPIP_MEMORY_GETLINEARMEMORYADDRESS_EXIT(a,b,c,d)
#endif


#ifndef DBG_TCPIP_MEMORY_GET_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_MEMORY_GET_ENTRY(a,b,c,d)
#endif


#ifndef DBG_TCPIP_MEMORY_GET_EXIT
/** \brief Exit point */
#define DBG_TCPIP_MEMORY_GET_EXIT(a,b,c,d)
#endif


#ifndef DBG_TCPIP_MEMORY_INCREASE_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_MEMORY_INCREASE_ENTRY(a,b)
#endif


#ifndef DBG_TCPIP_MEMORY_INCREASE_EXIT
/** \brief Exit point */
#define DBG_TCPIP_MEMORY_INCREASE_EXIT(a,b,c)
#endif


#ifndef DBG_TCPIP_MEMORY_DECREASE_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_MEMORY_DECREASE_ENTRY(a,b)
#endif


#ifndef DBG_TCPIP_MEMORY_DECREASE_EXIT
/** \brief Exit point */
#define DBG_TCPIP_MEMORY_DECREASE_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_MEMORY_NVM_START_ENTRY
/** \brief Entry point of function TcpIp_Memory_NvM_start() */
#define DBG_TCPIP_MEMORY_NVM_START_ENTRY()
#endif

#ifndef DBG_TCPIP_MEMORY_NVM_START_EXIT
/** \brief Exit point of function TcpIp_Memory_NvM_start() */
#define DBG_TCPIP_MEMORY_NVM_START_EXIT()
#endif

#ifndef DBG_TCPIP_MEMORY_NVM_STOP_ENTRY
/** \brief Entry point of function TcpIp_Memory_NvM_stop() */
#define DBG_TCPIP_MEMORY_NVM_STOP_ENTRY()
#endif

#ifndef DBG_TCPIP_MEMORY_NVM_STOP_EXIT
/** \brief Exit point of function TcpIp_Memory_NvM_stop() */
#define DBG_TCPIP_MEMORY_NVM_STOP_EXIT()
#endif

#ifndef DBG_TCPIP_MEMORY_NVM_SETIPADDRESS_ENTRY
/** \brief Entry point of function TcpIp_Memory_NvM_setIpAddress() */
#define DBG_TCPIP_MEMORY_NVM_SETIPADDRESS_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_MEMORY_NVM_SETIPADDRESS_EXIT
/** \brief Exit point of function TcpIp_Memory_NvM_setIpAddress() */
#define DBG_TCPIP_MEMORY_NVM_SETIPADDRESS_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_MEMORY_NVM_GETIPADDRESS_ENTRY
/** \brief Entry point of function TcpIp_Memory_NvM_getIpAddress() */
#define DBG_TCPIP_MEMORY_NVM_GETIPADDRESS_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_MEMORY_NVM_GETIPADDRESS_EXIT
/** \brief Exit point of function TcpIp_Memory_NvM_getIpAddress() */
#define DBG_TCPIP_MEMORY_NVM_GETIPADDRESS_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IP_INIT_ENTRY
/** \brief Entry point  */
#define DBG_TCPIP_IP_INIT_ENTRY()
#endif

#ifndef DBG_TCPIP_IP_INIT_EXIT
/** \brief Exit point  */
#define DBG_TCPIP_IP_INIT_EXIT()
#endif

#ifndef DBG_TCPIP_IP_MAINFUNCTION_ENTRY
/** \brief Entry point  */
#define DBG_TCPIP_IP_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_TCPIP_IP_MAINFUNCTION_EXIT
/** \brief Exit point  */
#define DBG_TCPIP_IP_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_TCPIP_IP_FRAG_RETURNBUFFER_ENTRY
/** \brief Entry point  */
#define DBG_TCPIP_IP_FRAG_RETURNBUFFER_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IP_FRAG_RETURNBUFFER_EXIT
/** \brief Exit point  */
#define DBG_TCPIP_IP_FRAG_RETURNBUFFER_EXIT(a)
#endif

#ifndef DBG_TCPIP_IP_FRAG_REASSEMBLE_ENTRY
/** \brief Entry point  */
#define DBG_TCPIP_IP_FRAG_REASSEMBLE_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IP_FRAG_REASSEMBLE_EXIT
/** \brief Exit point  */
#define DBG_TCPIP_IP_FRAG_REASSEMBLE_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IP_TRANSMIT_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IP_TRANSMIT_ENTRY(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_TCPIP_IP_TRANSMIT_EXIT
/** \brief Exit point  */
#define DBG_TCPIP_IP_TRANSMIT_EXIT(a,b,c,d,e,f,g,h,i)
#endif

#ifndef DBG_TCPIP_IP_TRANSMITIP_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IP_TRANSMITIP_ENTRY(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_TCPIP_IP_TRANSMITIP_EXIT
/** \brief Exit point  */
#define DBG_TCPIP_IP_TRANSMITIP_EXIT(a,b,c,d,e,f,g,h,i)
#endif

#ifndef DBG_TCPIP_IP_GETMAXIMUMPAYLOAD_ENTRY
/** \brief Entry point  */
#define DBG_TCPIP_IP_GETMAXIMUMPAYLOAD_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IP_GETMAXIMUMPAYLOAD_EXIT
/** \brief Exit point  */
#define DBG_TCPIP_IP_GETMAXIMUMPAYLOAD_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_IP_GETMINIMUMPAYLOAD_ENTRY
/** \brief Entry point  */
#define DBG_TCPIP_IP_GETMINIMUMPAYLOAD_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IP_GETMINIMUMPAYLOAD_EXIT
/** \brief Exit point  */
#define DBG_TCPIP_IP_GETMINIMUMPAYLOAD_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IP_GETIFSTATE_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IP_GETIFSTATE_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IP_GETIFSTATE_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IP_GETIFSTATE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IP_GETIPCONFIG_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IP_GETIPCONFIG_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IP_GETIPCONFIG_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IP_GETIPCONFIG_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IP_GETTXLOCALADDRID_BYREMOTEIP_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IP_GETTXLOCALADDRID_BYREMOTEIP_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IP_GETTXLOCALADDRID_BYREMOTEIP_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IP_GETTXLOCALADDRID_BYREMOTEIP_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IP_GETTXLOCALADDRID_BYLOCALADDRID_ENTRY
/** \brief Entry point */
#define DBG_TCPIP_IP_GETTXLOCALADDRID_BYLOCALADDRID_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IP_GETTXLOCALADDRID_BYLOCALADDRID_EXIT
/** \brief Exit point */
#define DBG_TCPIP_IP_GETTXLOCALADDRID_BYLOCALADDRID_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_INIT_ENTRY
/** \brief Entry point of function TcpIp_IPV6_StaticIp_init() */
#define DBG_TCPIP_IPV6_STATICIP_INIT_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_INIT_EXIT
/** \brief Exit point of function TcpIp_IPV6_StaticIp_init() */
#define DBG_TCPIP_IPV6_STATICIP_INIT_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_STARTASSIGN_ENTRY
/** \brief Entry point of function TcpIp_IPV6_StaticIp_startAssign() */
#define DBG_TCPIP_IPV6_STATICIP_STARTASSIGN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_STARTASSIGN_EXIT
/** \brief Exit point of function TcpIp_IPV6_StaticIp_startAssign() */
#define DBG_TCPIP_IPV6_STATICIP_STARTASSIGN_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_STOPASSIGN_ENTRY
/** \brief Entry point of function TcpIp_IPV6_StaticIp_stopAssign() */
#define DBG_TCPIP_IPV6_STATICIP_STOPASSIGN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_STOPASSIGN_EXIT
/** \brief Exit point of function TcpIp_IPV6_StaticIp_stopAssign() */
#define DBG_TCPIP_IPV6_STATICIP_STOPASSIGN_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_GETIPCONFIG_ENTRY
/** \brief Entry point of function TcpIp_IPV6_StaticIp_getIpConfig() */
#define DBG_TCPIP_IPV6_STATICIP_GETIPCONFIG_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_GETIPCONFIG_EXIT
/** \brief Exit point of function TcpIp_IPV6_StaticIp_getIpConfig() */
#define DBG_TCPIP_IPV6_STATICIP_GETIPCONFIG_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_REQUESTIPADDRESS_ENTRY
/** \brief Entry point of function TcpIp_IPV6_StaticIp_requestIpAddress() */
#define DBG_TCPIP_IPV6_STATICIP_REQUESTIPADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_REQUESTIPADDRESS_EXIT
/** \brief Exit point of function TcpIp_IPV6_StaticIp_requestIpAddress() */
#define DBG_TCPIP_IPV6_STATICIP_REQUESTIPADDRESS_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_GETIPADDRESS_ENTRY
/** \brief Entry point of function TcpIp_IPV6_StaticIp_getIpAddress() */
#define DBG_TCPIP_IPV6_STATICIP_GETIPADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_GETIPADDRESS_EXIT
/** \brief Exit point of function TcpIp_IPV6_StaticIp_getIpAddress() */
#define DBG_TCPIP_IPV6_STATICIP_GETIPADDRESS_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_ASSIGNMENTACTIVE_ENTRY
/** \brief Entry point of function TcpIp_IPV6_StaticIp_assignmentActive() */
#define DBG_TCPIP_IPV6_STATICIP_ASSIGNMENTACTIVE_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_ASSIGNMENTACTIVE_EXIT
/** \brief Exit point of function TcpIp_IPV6_StaticIp_assignmentActive() */
#define DBG_TCPIP_IPV6_STATICIP_ASSIGNMENTACTIVE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_GETIPADDRSTATE_ENTRY
/** \brief Entry point of function TcpIp_IpV6_StaticIp_getIpAddrState() */
#define DBG_TCPIP_IPV6_STATICIP_GETIPADDRSTATE_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV6_STATICIP_GETIPADDRSTATE_EXIT
/** \brief Exit point of function TcpIp_IpV6_StaticIp_getIpAddrState() */
#define DBG_TCPIP_IPV6_STATICIP_GETIPADDRSTATE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_INIT_ENTRY
/** \brief Entry point of function TcpIp_IPV6_LinkLocal_init() */
#define DBG_TCPIP_IPV6_LINKLOCAL_INIT_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_INIT_EXIT
/** \brief Exit point of function TcpIp_IPV6_LinkLocal_init() */
#define DBG_TCPIP_IPV6_LINKLOCAL_INIT_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_STARTASSIGN_ENTRY
/** \brief Entry point of function TcpIp_IPV6_LinkLocal_startAssign() */
#define DBG_TCPIP_IPV6_LINKLOCAL_STARTASSIGN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_STARTASSIGN_EXIT
/** \brief Exit point of function TcpIp_IPV6_LinkLocal_startAssign() */
#define DBG_TCPIP_IPV6_LINKLOCAL_STARTASSIGN_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_STOPASSIGN_ENTRY
/** \brief Entry point of function TcpIp_IPV6_LinkLocal_stopAssign() */
#define DBG_TCPIP_IPV6_LINKLOCAL_STOPASSIGN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_STOPASSIGN_EXIT
/** \brief Exit point of function TcpIp_IPV6_LinkLocal_stopAssign() */
#define DBG_TCPIP_IPV6_LINKLOCAL_STOPASSIGN_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_GETIPCONFIG_ENTRY
/** \brief Entry point of function TcpIp_IPV6_LinkLocal_getIpConfig() */
#define DBG_TCPIP_IPV6_LINKLOCAL_GETIPCONFIG_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_GETIPCONFIG_EXIT
/** \brief Exit point of function TcpIp_IPV6_LinkLocal_getIpConfig() */
#define DBG_TCPIP_IPV6_LINKLOCAL_GETIPCONFIG_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_REQUESTIPADDRESS_ENTRY
/** \brief Entry point of function TcpIp_IPV6_LinkLocal_requestIpAddress() */
#define DBG_TCPIP_IPV6_LINKLOCAL_REQUESTIPADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_REQUESTIPADDRESS_EXIT
/** \brief Exit point of function TcpIp_IPV6_LinkLocal_requestIpAddress() */
#define DBG_TCPIP_IPV6_LINKLOCAL_REQUESTIPADDRESS_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_GETIPADDRESS_ENTRY
/** \brief Entry point of function TcpIp_IPV6_LinkLocal_getIpAddress() */
#define DBG_TCPIP_IPV6_LINKLOCAL_GETIPADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_GETIPADDRESS_EXIT
/** \brief Exit point of function TcpIp_IPV6_LinkLocal_getIpAddress() */
#define DBG_TCPIP_IPV6_LINKLOCAL_GETIPADDRESS_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_ASSIGNMENTACTIVE_ENTRY
/** \brief Entry point of function TcpIp_IPV6_LinkLocal_assignmentActive() */
#define DBG_TCPIP_IPV6_LINKLOCAL_ASSIGNMENTACTIVE_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_ASSIGNMENTACTIVE_EXIT
/** \brief Exit point of function TcpIp_IPV6_LinkLocal_assignmentActive() */
#define DBG_TCPIP_IPV6_LINKLOCAL_ASSIGNMENTACTIVE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_GETIPADDRSTATE_ENTRY
/** \brief Entry point of function TcpIp_IpV6_LinkLocal_getIpAddrState() */
#define DBG_TCPIP_IPV6_LINKLOCAL_GETIPADDRSTATE_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPV6_LINKLOCAL_GETIPADDRSTATE_EXIT
/** \brief Exit point of function TcpIp_IpV6_LinkLocal_getIpAddrState() */
#define DBG_TCPIP_IPV6_LINKLOCAL_GETIPADDRSTATE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_DHCPV6_RXINDICATION_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6_rxIndication() */
#define DBG_TCPIP_DHCPV6_RXINDICATION_ENTRY(p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_DHCPV6_RXINDICATION_EXIT
/** \brief Exit point of function TcpIp_DhcpV6_rxIndication() */
#define DBG_TCPIP_DHCPV6_RXINDICATION_EXIT(p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_DHCPV6_LOCALIPADDRASSIGNMENTCHG_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6_LocalIpAddrAssignmentChg() */
#define DBG_TCPIP_DHCPV6_LOCALIPADDRASSIGNMENTCHG_ENTRY(p1, p2)
#endif

#ifndef DBG_TCPIP_DHCPV6_LOCALIPADDRASSIGNMENTCHG_EXIT
/** \brief Exit point of function TcpIp_DhcpV6_LocalIpAddrAssignmentChg() */
#define DBG_TCPIP_DHCPV6_LOCALIPADDRASSIGNMENTCHG_EXIT(p1, p2)
#endif

#ifndef DBG_TCPIP_DHCPV6_COPYTXDATA_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6_CopyTxData() */
#define DBG_TCPIP_DHCPV6_COPYTXDATA_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_DHCPV6_COPYTXDATA_EXIT
/** \brief Exit point of function TcpIp_DhcpV6_CopyTxData() */
#define DBG_TCPIP_DHCPV6_COPYTXDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_DHCPV6_INIT_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6_init() */
#define DBG_TCPIP_DHCPV6_INIT_ENTRY(p1)
#endif

#ifndef DBG_TCPIP_DHCPV6_INIT_EXIT
/** \brief Exit point of function TcpIp_DhcpV6_init() */
#define DBG_TCPIP_DHCPV6_INIT_EXIT(p1)
#endif

#ifndef DBG_TCPIP_DHCPV6_STARTASSIGN_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6_startAssign() */
#define DBG_TCPIP_DHCPV6_STARTASSIGN_ENTRY(p1)
#endif

#ifndef DBG_TCPIP_DHCPV6_STARTASSIGN_EXIT
/** \brief Exit point of function TcpIp_DhcpV6_startAssign() */
#define DBG_TCPIP_DHCPV6_STARTASSIGN_EXIT(p1)
#endif

#ifndef DBG_TCPIP_DHCPV6_STOPASSIGN_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6_stopAssign() */
#define DBG_TCPIP_DHCPV6_STOPASSIGN_ENTRY(p1)
#endif

#ifndef DBG_TCPIP_DHCPV6_STOPASSIGN_EXIT
/** \brief Exit point of function TcpIp_DhcpV6_stopAssign() */
#define DBG_TCPIP_DHCPV6_STOPASSIGN_EXIT(p1)
#endif

#ifndef DBG_TCPIP_DHCPV6_MAINFUNCTION_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6_mainFunction() */
#define DBG_TCPIP_DHCPV6_MAINFUNCTION_ENTRY(p1)
#endif

#ifndef DBG_TCPIP_DHCPV6_MAINFUNCTION_EXIT
/** \brief Exit point of function TcpIp_DhcpV6_mainFunction() */
#define DBG_TCPIP_DHCPV6_MAINFUNCTION_EXIT(p1)
#endif

#ifndef DBG_TCPIP_DHCPV6_GETIPCONFIG_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6_getIpConfig() */
#define DBG_TCPIP_DHCPV6_GETIPCONFIG_ENTRY(p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_DHCPV6_GETIPCONFIG_EXIT
/** \brief Exit point of function TcpIp_DhcpV6_getIpConfig() */
#define DBG_TCPIP_DHCPV6_GETIPCONFIG_EXIT(p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_DHCPV6_GETIPADDRESS_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6_getIpAddress() */
#define DBG_TCPIP_DHCPV6_GETIPADDRESS_ENTRY(p1, p2)
#endif

#ifndef DBG_TCPIP_DHCPV6_GETIPADDRESS_EXIT
/** \brief Exit point of function TcpIp_DhcpV6_getIpAddress() */
#define DBG_TCPIP_DHCPV6_GETIPADDRESS_EXIT(p1, p2)
#endif

#ifndef DBG_TCPIP_DHCPV6_ASSIGNMENTACTIVE_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6_assignmentActive() */
#define DBG_TCPIP_DHCPV6_ASSIGNMENTACTIVE_ENTRY(p1, p2)
#endif

#ifndef DBG_TCPIP_DHCPV6_ASSIGNMENTACTIVE_EXIT
/** \brief Exit point of function TcpIp_DhcpV6_assignmentActive() */
#define DBG_TCPIP_DHCPV6_ASSIGNMENTACTIVE_EXIT(p1, p2)
#endif

#ifndef DBG_TCPIP_DHCPV6_REQUESTIPADDRESS_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6_requestIpAddress() */
#define DBG_TCPIP_DHCPV6_REQUESTIPADDRESS_ENTRY(p1, p2)
#endif

#ifndef DBG_TCPIP_DHCPV6_REQUESTIPADDRESS_EXIT
/** \brief Exit point of function TcpIp_DhcpV6_requestIpAddress() */
#define DBG_TCPIP_DHCPV6_REQUESTIPADDRESS_EXIT(retval, p1, p2)
#endif

#ifndef DBG_TCPIP_DHCPV6_GETIPADDRSTATE_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6_getIpAddrState() */
#define DBG_TCPIP_DHCPV6_GETIPADDRSTATE_ENTRY(p1)
#endif

#ifndef DBG_TCPIP_DHCPV6_GETIPADDRSTATE_EXIT
/** \brief Exit point of function TcpIp_DhcpV6_getIpAddrState() */
#define DBG_TCPIP_DHCPV6_GETIPADDRSTATE_EXIT(retval, p1)
#endif

#ifndef DBG_TCPIP_DHCPV6READOPTION_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6ReadOption() */
#define DBG_TCPIP_DHCPV6READOPTION_ENTRY(p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_DHCPV6READOPTION_EXIT
/** \brief Exit point of function TcpIp_DhcpV6ReadOption() */
#define DBG_TCPIP_DHCPV6READOPTION_EXIT(retval, p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_DHCPV6WRITEOPTION_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6WriteOption() */
#define DBG_TCPIP_DHCPV6WRITEOPTION_ENTRY(p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_DHCPV6WRITEOPTION_EXIT
/** \brief Exit point of function TcpIp_DhcpV6WriteOption() */
#define DBG_TCPIP_DHCPV6WRITEOPTION_EXIT(retval, p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_DHCPV6_GETSTATUS_ENTRY
/** \brief Entry point of function TcpIp_DhcpV6_getStatus() */
#define DBG_TCPIP_DHCPV6_GETSTATUS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_DHCPV6_GETSTATUS_EXIT
/** \brief Exit point of function TcpIp_DhcpV6_getStatus() */
#define DBG_TCPIP_DHCPV6_GETSTATUS_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV6_GETTXLOCALADDRID_BYCTRLIDX_ENTRY
/** \brief Entry point of function TcpIp_IpV6_getTxLocalAddrId_byCtrlIdx() */
#define DBG_TCPIP_IPV6_GETTXLOCALADDRID_BYCTRLIDX_ENTRY(p1, p2)
#endif

#ifndef DBG_TCPIP_IPV6_GETTXLOCALADDRID_BYCTRLIDX_EXIT
/** \brief Exit point of function TcpIp_IpV6_getTxLocalAddrId_byCtrlIdx() */
#define DBG_TCPIP_IPV6_GETTXLOCALADDRID_BYCTRLIDX_EXIT(retval, p1, p2)
#endif

#ifndef DBG_TCPIP_IPV6_GETTXLOCALADDRID_BYREMOTEIP_ENTRY
/** \brief Entry point of function TcpIp_IpV6_getTxLocalAddrId_byRemoteIp() */
#define DBG_TCPIP_IPV6_GETTXLOCALADDRID_BYREMOTEIP_ENTRY(p1)
#endif

#ifndef DBG_TCPIP_IPV6_GETTXLOCALADDRID_BYREMOTEIP_EXIT
/** \brief Exit point of function TcpIp_IpV6_getTxLocalAddrId_byRemoteIp() */
#define DBG_TCPIP_IPV6_GETTXLOCALADDRID_BYREMOTEIP_EXIT(retval, p1)
#endif

#ifndef DBG_TCPIP_IPV6_GETIPCONFIG_ENTRY
/** \brief Entry point of function TcpIp_IpV6_getIpConfig() */
#define DBG_TCPIP_IPV6_GETIPCONFIG_ENTRY(p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_IPV6_GETIPCONFIG_EXIT
/** \brief Exit point of function TcpIp_IpV6_getIpConfig() */
#define DBG_TCPIP_IPV6_GETIPCONFIG_EXIT(p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_IPV6_GETIFSTATE_ENTRY
/** \brief Entry point of function TcpIp_IpV6_getIfState() */
#define DBG_TCPIP_IPV6_GETIFSTATE_ENTRY(p1)
#endif

#ifndef DBG_TCPIP_IPV6_GETIFSTATE_EXIT
/** \brief Exit point of function TcpIp_IpV6_getIfState() */
#define DBG_TCPIP_IPV6_GETIFSTATE_EXIT(retval, p1)
#endif

#ifndef DBG_TCPIP_IPV6_PROVIDETXBUFFER_ENTRY
/** \brief Entry point of function TcpIp_IpV6_provideTxBuffer() */
#define DBG_TCPIP_IPV6_PROVIDETXBUFFER_ENTRY(p1, p2, p3, p4, p5)
#endif

#ifndef DBG_TCPIP_IPV6_PROVIDETXBUFFER_EXIT
/** \brief Exit point of function TcpIp_IpV6_provideTxBuffer() */
#define DBG_TCPIP_IPV6_PROVIDETXBUFFER_EXIT(retval, p1, p2, p3, p4, p5)
#endif

#ifndef DBG_TCPIP_IPV6_GETREMOTEADDRHANDLE_ENTRY
/** \brief Entry point of function TcpIp_IpV6_getRemoteAddrHandle() */
#define DBG_TCPIP_IPV6_GETREMOTEADDRHANDLE_ENTRY(p1, p2, p3)
#endif

#ifndef DBG_TCPIP_IPV6_GETREMOTEADDRHANDLE_EXIT
/** \brief Exit point of function TcpIp_IpV6_getRemoteAddrHandle() */
#define DBG_TCPIP_IPV6_GETREMOTEADDRHANDLE_EXIT(retval, p1, p2, p3)
#endif

#ifndef DBG_TCPIP_IPV6_UNLOCKENTRY_ENTRY
/** \brief Entry point of function TcpIp_IpV6_unlockEntry() */
#define DBG_TCPIP_IPV6_UNLOCKENTRY_ENTRY(p1, p2)
#endif

#ifndef DBG_TCPIP_IPV6_UNLOCKENTRY_EXIT
/** \brief Exit point of function TcpIp_IpV6_unlockEntry() */
#define DBG_TCPIP_IPV6_UNLOCKENTRY_EXIT(p1, p2)
#endif

#ifndef DBG_TCPIP_IPV6_TRANSMIT_ENTRY
/** \brief Entry point of function TcpIp_IpV6_transmit() */
#define DBG_TCPIP_IPV6_TRANSMIT_ENTRY(p1, p2, p3, p4, p5, p6)
#endif

#ifndef DBG_TCPIP_IPV6_TRANSMIT_EXIT
/** \brief Exit point of function TcpIp_IpV6_transmit() */
#define DBG_TCPIP_IPV6_TRANSMIT_EXIT(retval, p1, p2, p3, p4, p5, p6)
#endif

#ifndef DBG_TCPIP_IPV6_UPDATEHEADER_ENTRY
/** \brief Entry point of function TcpIp_IpV6_updateHeader() */
#define DBG_TCPIP_IPV6_UPDATEHEADER_ENTRY(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_TCPIP_IPV6_UPDATEHEADER_EXIT
/** \brief Exit point of function TcpIp_IpV6_updateHeader() */
#define DBG_TCPIP_IPV6_UPDATEHEADER_EXIT(a,b,c,d,e,f,g,h,i)
#endif

#ifndef DBG_TCPIP_IPV6_ICMP_TRANSMIT_ENTRY
/** \brief Entry point of function TcpIp_IpV6_Icmp_transmit() */
#define DBG_TCPIP_IPV6_ICMP_TRANSMIT_ENTRY(p1, p2, p3, p4, p5, p6)
#endif

#ifndef DBG_TCPIP_IPV6_ICMP_TRANSMIT_EXIT
/** \brief Exit point of function TcpIp_IpV6_Icmp_transmit() */
#define DBG_TCPIP_IPV6_ICMP_TRANSMIT_EXIT(retval, p1, p2, p3, p4, p5, p6)
#endif

#ifndef DBG_TCPIP_IPV6_ICMP_UPDATEHEADER_ENTRY
/** \brief Entry point of function TcpIp_IpV6_Icmp_updateHeader() */
#define DBG_TCPIP_IPV6_ICMP_UPDATEHEADER_ENTRY(p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_IPV6_ICMP_UPDATEHEADER_EXIT
/** \brief Exit point of function TcpIp_IpV6_Icmp_updateHeader() */
#define DBG_TCPIP_IPV6_ICMP_UPDATEHEADER_EXIT(p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_IPV6_RXINDICATIONIP_ENTRY
/** \brief Entry point of function TcpIp_IpV6_rxIndicationIp() */
#define DBG_TCPIP_IPV6_RXINDICATIONIP_ENTRY(p1, p2, p3, p4, p5)
#endif

#ifndef DBG_TCPIP_IPV6_RXINDICATIONIP_EXIT
/** \brief Exit point of function TcpIp_IpV6_rxIndicationIp() */
#define DBG_TCPIP_IPV6_RXINDICATIONIP_EXIT(retval, p1, p2, p3, p4, p5)
#endif

#ifndef DBG_TCPIP_IPV6_GETMINIMUMPAYLOAD_ENTRY
/** \brief Entry point of function TcpIp_IpV6_getMinimumPayload() */
#define DBG_TCPIP_IPV6_GETMINIMUMPAYLOAD_ENTRY()
#endif

#ifndef DBG_TCPIP_IPV6_GETMINIMUMPAYLOAD_EXIT
/** \brief Exit point of function TcpIp_IpV6_getMinimumPayload() */
#define DBG_TCPIP_IPV6_GETMINIMUMPAYLOAD_EXIT(retval)
#endif

#ifndef DBG_TCPIP_IPV6_GETMAXIMUMPAYLOAD_ENTRY
/** \brief Entry point of function TcpIp_IpV6_getMaximumPayload() */
#define DBG_TCPIP_IPV6_GETMAXIMUMPAYLOAD_ENTRY(p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_IPV6_GETMAXIMUMPAYLOAD_EXIT
/** \brief Exit point of function TcpIp_IpV6_getMaximumPayload() */
#define DBG_TCPIP_IPV6_GETMAXIMUMPAYLOAD_EXIT(retval, p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_IPV6_GETFRAGMENTSIZE_ENTRY
/** \brief Entry point of function TcpIp_IpV6_getFragmentSize() */
#define DBG_TCPIP_IPV6_GETFRAGMENTSIZE_ENTRY(p1)
#endif

#ifndef DBG_TCPIP_IPV6_GETFRAGMENTSIZE_EXIT
/** \brief Exit point of function TcpIp_IpV6_getFragmentSize() */
#define DBG_TCPIP_IPV6_GETFRAGMENTSIZE_EXIT(retval, p1)
#endif

#ifndef DBG_TCPIP_IPV6_ISVALIDTCPREMOTEADDRESS_ENTRY
/** \brief Entry point of function TcpIp_IpV6_isValidTcpRemoteAddress() */
#define DBG_TCPIP_IPV6_ISVALIDTCPREMOTEADDRESS_ENTRY(p1)
#endif

#ifndef DBG_TCPIP_IPV6_ISVALIDTCPREMOTEADDRESS_EXIT
/** \brief Exit point of function TcpIp_IpV6_isValidTcpRemoteAddress() */
#define DBG_TCPIP_IPV6_ISVALIDTCPREMOTEADDRESS_EXIT(retval, p1)
#endif

#ifndef DBG_TCPIP_IPV6_ICMP_INTTRANSMIT_ENTRY
/** \brief Entry point of function TcpIp_IpV6_Icmp_intTransmit() */
#define DBG_TCPIP_IPV6_ICMP_INTTRANSMIT_ENTRY(p1, p2, p3, p4, p5, p6, p7, p8)
#endif

#ifndef DBG_TCPIP_IPV6_ICMP_INTTRANSMIT_EXIT
/** \brief Exit point of function TcpIp_IpV6_Icmp_intTransmit() */
#define DBG_TCPIP_IPV6_ICMP_INTTRANSMIT_EXIT(retval, p1, p2, p3, p4, p5, p6, p7, p8)
#endif

#ifndef DBG_TCPIP_IPV6_GETLOCALADDRID_BYIP_ENTRY
/** \brief Entry point of function TcpIp_IpV6_getLocalAddrId_byIp() */
#define DBG_TCPIP_IPV6_GETLOCALADDRID_BYIP_ENTRY(p1, p2)
#endif

#ifndef DBG_TCPIP_IPV6_GETLOCALADDRID_BYIP_EXIT
/** \brief Exit point of function TcpIp_IpV6_getLocalAddrId_byIp() */
#define DBG_TCPIP_IPV6_GETLOCALADDRID_BYIP_EXIT(retval, p1, p2)
#endif

#ifndef DBG_TCPIP_IPV6_GETANDRESETCOUNTER_ENTRY
/** \brief Entry point of function TcpIp_IpV6_GetAndResetCounter() */
#define DBG_TCPIP_IPV6_GETANDRESETCOUNTER_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV6_GETANDRESETCOUNTER_EXIT
/** \brief Exit point of function TcpIp_IpV6_GetAndResetCounter() */
#define DBG_TCPIP_IPV6_GETANDRESETCOUNTER_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV6_ISADDRALLNODESMULTICAST_ENTRY
/** \brief Entry point of function TcpIp_IpV6_isAddrAllNodesMulticast() */
#define DBG_TCPIP_IPV6_ISADDRALLNODESMULTICAST_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV6_ISADDRALLNODESMULTICAST_EXIT
/** \brief Exit point of function TcpIp_IpV6_isAddrAllNodesMulticast() */
#define DBG_TCPIP_IPV6_ISADDRALLNODESMULTICAST_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV6_GETCTRLADDRRANGE_ENTRY
/** \brief Entry point of function TcpIp_IpV6_getCtrlAddrRange() */
#define DBG_TCPIP_IPV6_GETCTRLADDRRANGE_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV6_GETCTRLADDRRANGE_EXIT
/** \brief Exit point of function TcpIp_IpV6_getCtrlAddrRange() */
#define DBG_TCPIP_IPV6_GETCTRLADDRRANGE_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPV6_CALCPARTIALCHECKSUM_ENTRY
/** \brief Entry point of function TcpIp_IpV6_CalcPartialChecksum() */
#define DBG_TCPIP_IPV6_CALCPARTIALCHECKSUM_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV6_CALCPARTIALCHECKSUM_EXIT
/** \brief Exit point of function TcpIp_IpV6_CalcPartialChecksum() */
#define DBG_TCPIP_IPV6_CALCPARTIALCHECKSUM_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_NDP_INIT_ENTRY
/** \brief Entry point of function TcpIp_Ndp_init() */
#define DBG_TCPIP_NDP_INIT_ENTRY()
#endif

#ifndef DBG_TCPIP_NDP_INIT_EXIT
/** \brief Exit point of function TcpIp_Ndp_init() */
#define DBG_TCPIP_NDP_INIT_EXIT()
#endif

#ifndef DBG_TCPIP_NDP_RXINDICATION_ENTRY
/** \brief Entry point of function TcpIp_Ndp_rxIndication() */
#define DBG_TCPIP_NDP_RXINDICATION_ENTRY(p1, p2, p3, p4, p5, p6, p7)
#endif

#ifndef DBG_TCPIP_NDP_RXINDICATION_EXIT
/** \brief Exit point of function TcpIp_Ndp_rxIndication() */
#define DBG_TCPIP_NDP_RXINDICATION_EXIT(p1, p2, p3, p4, p5, p6, p7)
#endif

#ifndef DBG_TCPIP_NDP_MAINFUNCTION_ENTRY
/** \brief Entry point of function TcpIp_Ndp_mainFunction() */
#define DBG_TCPIP_NDP_MAINFUNCTION_ENTRY(p1)
#endif

#ifndef DBG_TCPIP_NDP_MAINFUNCTION_EXIT
/** \brief Exit point of function TcpIp_Ndp_mainFunction() */
#define DBG_TCPIP_NDP_MAINFUNCTION_EXIT(p1)
#endif

#ifndef DBG_TCPIP_NDP_NCACHE_LOOKUPANDLOCK_ENTRY
/** \brief Entry point of function TcpIp_Ndp_NCache_lookup() */
#define DBG_TCPIP_NDP_NCACHE_LOOKUPANDLOCK_ENTRY(p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_NDP_NCACHE_LOOKUPANDLOCK_EXIT
/** \brief Exit point of function TcpIp_Ndp_NCache_lookup() */
#define DBG_TCPIP_NDP_NCACHE_LOOKUPANDLOCK_EXIT(retval, p1, p2, p3, p4)
#endif

#ifndef DBG_TCPIP_NDP_NCACHE_GETPHYSADDRPTR_ENTRY
/** \brief Entry point of function TcpIp_Ndp_NCache_getPhysAddrPtr() */
#define DBG_TCPIP_NDP_NCACHE_GETPHYSADDRPTR_ENTRY(p1, p2)
#endif

#ifndef DBG_TCPIP_NDP_NCACHE_GETPHYSADDRPTR_EXIT
/** \brief Exit point of function TcpIp_Ndp_NCache_getPhysAddrPtr() */
#define DBG_TCPIP_NDP_NCACHE_GETPHYSADDRPTR_EXIT(retval, p1, p2)
#endif

#ifndef DBG_TCPIP_NDP_NCACHE_GETREMOTEPHYSADDR_ENTRY
/** \brief Entry point of function TcpIp_Ndp_NCache_getRemotePhysAddr() */
#define DBG_TCPIP_NDP_NCACHE_GETREMOTEPHYSADDR_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_NDP_NCACHE_GETREMOTEPHYSADDR_EXIT
/** \brief Exit point of function TcpIp_Ndp_NCache_getRemotePhysAddr() */
#define DBG_TCPIP_NDP_NCACHE_GETREMOTEPHYSADDR_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_NDP_NCACHE_SETREMOTEPHYSADDR_ENTRY
/** \brief Entry point of function TcpIp_Ndp_NCache_setRemotePhysAddr() */
#define DBG_TCPIP_NDP_NCACHE_SETREMOTEPHYSADDR_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_NDP_NCACHE_SETREMOTEPHYSADDR_EXIT
/** \brief Exit point of function TcpIp_Ndp_NCache_setRemotePhysAddr() */
#define DBG_TCPIP_NDP_NCACHE_SETREMOTEPHYSADDR_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_NDP_NCACHE_UNLOCK_ENTRY
/** \brief Entry point of function TcpIp_Ndp_NCache_unlock() */
#define DBG_TCPIP_NDP_NCACHE_UNLOCK_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_NDP_NCACHE_UNLOCK_EXIT
/** \brief Exit point of function TcpIp_Ndp_NCache_unlock() */
#define DBG_TCPIP_NDP_NCACHE_UNLOCK_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_NDP_NCACHE_REACHCONFIRMATION_ENTRY
/** \brief Entry point of function TcpIp_Ndp_NCache_reachConfirmation() */
#define DBG_TCPIP_NDP_NCACHE_REACHCONFIRMATION_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_NDP_NCACHE_REACHCONFIRMATION_EXIT
/** \brief Exit point of function TcpIp_Ndp_NCache_reachConfirmation() */
#define DBG_TCPIP_NDP_NCACHE_REACHCONFIRMATION_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_INIT_ENTRY
/** \brief Entry point of function TcpIp_Ndp_Rpd_RouterAssign_init() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_INIT_ENTRY(a)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_INIT_EXIT
/** \brief Exit point of function TcpIp_Ndp_Rpd_RouterAssign_init() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_INIT_EXIT(a)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_STARTASSIGN_ENTRY
/** \brief Entry point of function TcpIp_Ndp_Rpd_RouterAssign_startAssign() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_STARTASSIGN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_STARTASSIGN_EXIT
/** \brief Exit point of function TcpIp_Ndp_Rpd_RouterAssign_startAssign() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_STARTASSIGN_EXIT(a)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_STOPASSIGN_ENTRY
/** \brief Entry point of function TcpIp_Ndp_Rpd_RouterAssign_stopAssign() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_STOPASSIGN_ENTRY(a)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_STOPASSIGN_EXIT
/** \brief Exit point of function TcpIp_Ndp_Rpd_RouterAssign_stopAssign() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_STOPASSIGN_EXIT(a)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPCONFIG_ENTRY
/** \brief Entry point of function TcpIp_Ndp_Rpd_RouterAssign_getIpConfig() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPCONFIG_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPCONFIG_EXIT
/** \brief Exit point of function TcpIp_Ndp_Rpd_RouterAssign_getIpConfig() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPCONFIG_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPADDRESS_ENTRY
/** \brief Entry point of function TcpIp_Ndp_Rpd_RouterAssign_getIpAddress() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPADDRESS_EXIT
/** \brief Exit point of function TcpIp_Ndp_Rpd_RouterAssign_getIpAddress() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPADDRESS_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_ASSIGNMENTACTIVE_ENTRY
/** \brief Entry point of function TcpIp_Ndp_Rpd_RouterAssign_assignmentActive() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_ASSIGNMENTACTIVE_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_ASSIGNMENTACTIVE_EXIT
/** \brief Exit point of function TcpIp_Ndp_Rpd_RouterAssign_assignmentActive() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_ASSIGNMENTACTIVE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_REQUESTIPADDRESS_ENTRY
/** \brief Entry point of function TcpIp_Ndp_Rpd_RouterAssign_requestIpAddress() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_REQUESTIPADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_REQUESTIPADDRESS_EXIT
/** \brief Exit point of function TcpIp_Ndp_Rpd_RouterAssign_requestIpAddress() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_REQUESTIPADDRESS_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPADDRSTATE_ENTRY
/** \brief Entry point of function TcpIp_Ndp_Rpd_RouterAssign_getIpAddrState() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPADDRSTATE_ENTRY(a)
#endif

#ifndef DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPADDRSTATE_EXIT
/** \brief Exit point of function TcpIp_Ndp_Rpd_RouterAssign_getIpAddrState() */
#define DBG_TCPIP_NDP_RPD_ROUTERASSIGN_GETIPADDRSTATE_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_NDP_DESTCACHE_GETNEXTHOPADDRANDLOCK_ENTRY
/** \brief Entry point of function TcpIp_Ndp_DestCache_getNextHopAddrAndLock() */
#define DBG_TCPIP_NDP_DESTCACHE_GETNEXTHOPADDRANDLOCK_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_TCPIP_NDP_DESTCACHE_GETNEXTHOPADDRANDLOCK_EXIT
/** \brief Exit point of function TcpIp_Ndp_DestCache_getNextHopAddrAndLock() */
#define DBG_TCPIP_NDP_DESTCACHE_GETNEXTHOPADDRANDLOCK_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_TCPIP_NDP_DESTCACHE_UNLOCK_ENTRY
/** \brief Entry point of function TcpIp_Ndp_DestCache_unlock() */
#define DBG_TCPIP_NDP_DESTCACHE_UNLOCK_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_NDP_DESTCACHE_UNLOCK_EXIT
/** \brief Exit point of function TcpIp_Ndp_DestCache_unlock() */
#define DBG_TCPIP_NDP_DESTCACHE_UNLOCK_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_NDP_RPD_GETDEFAULTROUTERADDRESS_ENTRY
/** \brief Entry point of function TcpIp_Ndp_Rpd_getDefaultRouterAddress() */
#define DBG_TCPIP_NDP_RPD_GETDEFAULTROUTERADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_NDP_RPD_GETDEFAULTROUTERADDRESS_EXIT
/** \brief Exit point of function TcpIp_Ndp_Rpd_getDefaultRouterAddress() */
#define DBG_TCPIP_NDP_RPD_GETDEFAULTROUTERADDRESS_EXIT(a,b)
#endif

#ifndef DBG_TCPIP_NDP_COPYDATA_ENTRY
/** \brief Entry point of function TcpIp_Ndp_copyData() */
#define DBG_TCPIP_NDP_COPYDATA_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_NDP_COPYDATA_EXIT
/** \brief Exit point of function TcpIp_Ndp_copyData() */
#define DBG_TCPIP_NDP_COPYDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IP_CALCPARTIALCHECKSUM_ENTRY
/** \brief Entry point of function TcpIp_Ip_CalcPartialChecksum() */
#define DBG_TCPIP_IP_CALCPARTIALCHECKSUM_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IP_CALCPARTIALCHECKSUM_EXIT
/** \brief Exit point of function TcpIp_Ip_CalcPartialChecksum() */
#define DBG_TCPIP_IP_CALCPARTIALCHECKSUM_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_IP_COMPAREIPADDR_ENTRY
/** \brief Entry point of function TcpIp_Ip_compareIpAddr() */
#define DBG_TCPIP_IP_COMPAREIPADDR_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IP_COMPAREIPADDR_EXIT
/** \brief Exit point of function TcpIp_Ip_compareIpAddr() */
#define DBG_TCPIP_IP_COMPAREIPADDR_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IP_ISADDRLIMBROADCASTALLNODESMULTICAST_ENTRY
/** \brief Entry point of function TcpIp_Ip_isAddrLimBroadcastAllNodesMulticast() */
#define DBG_TCPIP_IP_ISADDRLIMBROADCASTALLNODESMULTICAST_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IP_ISADDRLIMBROADCASTALLNODESMULTICAST_EXIT
/** \brief Exit point of function TcpIp_Ip_isAddrLimBroadcastAllNodesMulticast() */
#define DBG_TCPIP_IP_ISADDRLIMBROADCASTALLNODESMULTICAST_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_IP_GETCTRLADDRRANGE_ENTRY
/** \brief Entry point of function TcpIp_Ip_getCtrlAddrRange() */
#define DBG_TCPIP_IP_GETCTRLADDRRANGE_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IP_GETCTRLADDRRANGE_EXIT
/** \brief Exit point of function TcpIp_Ip_getCtrlAddrRange() */
#define DBG_TCPIP_IP_GETCTRLADDRRANGE_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_CALCPARTIALCHECKSUM_ENTRY
/** \brief Entry point of function TcpIp_IpV4_CalcPartialChecksum() */
#define DBG_TCPIP_IPV4_CALCPARTIALCHECKSUM_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPV4_CALCPARTIALCHECKSUM_EXIT
/** \brief Exit point of function TcpIp_IpV4_CalcPartialChecksum() */
#define DBG_TCPIP_IPV4_CALCPARTIALCHECKSUM_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_DHCPGETSTATUS_ENTRY
/** \brief Entry point of function TcpIp_DhcpGetStatus() */
#define DBG_TCPIP_DHCPGETSTATUS_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_DHCPGETSTATUS_EXIT
/** \brief Exit point of function TcpIp_DhcpGetStatus() */
#define DBG_TCPIP_DHCPGETSTATUS_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPSECDB_INIT_ENTRY
/** \brief Entry point of function TcpIp_IpSecDb_init() */
#define DBG_TCPIP_IPSECDB_INIT_ENTRY()
#endif

#ifndef DBG_TCPIP_IPSECDB_INIT_EXIT
/** \brief Exit point of function TcpIp_IpSecDb_init() */
#define DBG_TCPIP_IPSECDB_INIT_EXIT()
#endif

#ifndef DBG_TCPIP_IPSECDB_UPDATESEQNUM_ENTRY
/** \brief Entry point of function TcpIp_IpSecDb_UpdateSeqNum() */
#define DBG_TCPIP_IPSECDB_UPDATESEQNUM_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IPSECDB_UPDATESEQNUM_EXIT
/** \brief Exit point of function TcpIp_IpSecDb_UpdateSeqNum() */
#define DBG_TCPIP_IPSECDB_UPDATESEQNUM_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPSECDB_GETSECURITYRXASSOCIATION_ENTRY
/** \brief Entry point of function TcpIp_IpSecDb_getSecurityRxAssociation() */
#define DBG_TCPIP_IPSECDB_GETSECURITYRXASSOCIATION_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_TCPIP_IPSECDB_GETSECURITYRXASSOCIATION_EXIT
/** \brief Exit point of function TcpIp_IpSecDb_getSecurityRxAssociation() */
#define DBG_TCPIP_IPSECDB_GETSECURITYRXASSOCIATION_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_TCPIP_IPSECDB_STOP_ENTRY
/** \brief Entry point of function TcpIp_IpSecDb_stop() */
#define DBG_TCPIP_IPSECDB_STOP_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPSECDB_STOP_EXIT
/** \brief Exit point of function TcpIp_IpSecDb_stop() */
#define DBG_TCPIP_IPSECDB_STOP_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPSECDB_START_ENTRY
/** \brief Entry point of function TcpIp_IpSecDb_start() */
#define DBG_TCPIP_IPSECDB_START_ENTRY(a)
#endif

#ifndef DBG_TCPIP_IPSECDB_START_EXIT
/** \brief Exit point of function TcpIp_IpSecDb_start() */
#define DBG_TCPIP_IPSECDB_START_EXIT(a)
#endif

#ifndef DBG_TCPIP_IPSECDB_GETSECURITYTXASSOCIATION_ENTRY
/** \brief Entry point of function TcpIp_IpSecDb_getSecurityTxAssociation() */
#define DBG_TCPIP_IPSECDB_GETSECURITYTXASSOCIATION_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IPSECDB_GETSECURITYTXASSOCIATION_EXIT
/** \brief Exit point of function TcpIp_IpSecDb_getSecurityTxAssociation() */
#define DBG_TCPIP_IPSECDB_GETSECURITYTXASSOCIATION_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPSECDB_LOGERROR_ENTRY
/** \brief Entry point of function TcpIp_IpSecDb_LogError() */
#define DBG_TCPIP_IPSECDB_LOGERROR_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IPSECDB_LOGERROR_EXIT
/** \brief Exit point of function TcpIp_IpSecDb_LogError() */
#define DBG_TCPIP_IPSECDB_LOGERROR_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPSECDB_CHECKSECURITYPOLICYADDR_ENTRY
/** \brief Entry point of function TcpIp_IpSecDb_checkSecurityPolicyAddr() */
#define DBG_TCPIP_IPSECDB_CHECKSECURITYPOLICYADDR_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IPSECDB_CHECKSECURITYPOLICYADDR_EXIT
/** \brief Exit point of function TcpIp_IpSecDb_checkSecurityPolicyAddr() */
#define DBG_TCPIP_IPSECDB_CHECKSECURITYPOLICYADDR_EXIT(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPSECDB_CHECKSECURITYRXPOLICY_ENTRY
/** \brief Entry point of function TcpIp_IpSecDb_checkSecurityRxPolicy() */
#define DBG_TCPIP_IPSECDB_CHECKSECURITYRXPOLICY_ENTRY(a,b,c,d,e,f,g)
#endif

#ifndef DBG_TCPIP_IPSECDB_CHECKSECURITYRXPOLICY_EXIT
/** \brief Exit point of function TcpIp_IpSecDb_checkSecurityRxPolicy() */
#define DBG_TCPIP_IPSECDB_CHECKSECURITYRXPOLICY_EXIT(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_TCPIP_IPSECDB_CHECKSECURITYTXPOLICY_ENTRY
/** \brief Entry point of function TcpIp_IpSecDb_checkSecurityTxPolicy() */
#define DBG_TCPIP_IPSECDB_CHECKSECURITYTXPOLICY_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_IPSECDB_CHECKSECURITYTXPOLICY_EXIT
/** \brief Exit point of function TcpIp_IpSecDb_checkSecurityTxPolicy() */
#define DBG_TCPIP_IPSECDB_CHECKSECURITYTXPOLICY_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_TCPIP_REQUESTIPSECMODE_ENTRY
/** \brief Entry point of function TcpIp_RequestIpSecMode() */
#define DBG_TCPIP_REQUESTIPSECMODE_ENTRY(a,b)
#endif

#ifndef DBG_TCPIP_REQUESTIPSECMODE_EXIT
/** \brief Exit point of function TcpIp_RequestIpSecMode() */
#define DBG_TCPIP_REQUESTIPSECMODE_EXIT(a,b,c)
#endif

#ifndef DBG_TCPIP_IPSEC_INIT_ENTRY
/** \brief Entry point of function TcpIp_IpSec_init() */
#define DBG_TCPIP_IPSEC_INIT_ENTRY()
#endif

#ifndef DBG_TCPIP_IPSEC_INIT_EXIT
/** \brief Exit point of function TcpIp_IpSec_init() */
#define DBG_TCPIP_IPSEC_INIT_EXIT()
#endif

#ifndef DBG_TCPIP_IPSEC_MAINFUNCTION_ENTRY
/** \brief Entry point of function TcpIp_IpSec_mainFunction() */
#define DBG_TCPIP_IPSEC_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_TCPIP_IPSEC_MAINFUNCTION_EXIT
/** \brief Exit point of function TcpIp_IpSec_mainFunction() */
#define DBG_TCPIP_IPSEC_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_TCPIP_IPSEC_AHRXINDICATION_ENTRY
/** \brief Entry point of function TcpIp_IpSec_ahRxIndication() */
#define DBG_TCPIP_IPSEC_AHRXINDICATION_ENTRY(a,b,c,d,e,f,g)
#endif

#ifndef DBG_TCPIP_IPSEC_AHRXINDICATION_EXIT
/** \brief Exit point of function TcpIp_IpSec_ahRxIndication() */
#define DBG_TCPIP_IPSEC_AHRXINDICATION_EXIT(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_TCPIP_IPSEC_BUILDAH_ENTRY
/** \brief Entry point of function TcpIp_IpSec_buildAh() */
#define DBG_TCPIP_IPSEC_BUILDAH_ENTRY(a,b,c,d)
#endif

#ifndef DBG_TCPIP_IPSEC_BUILDAH_EXIT
/** \brief Exit point of function TcpIp_IpSec_buildAh() */
#define DBG_TCPIP_IPSEC_BUILDAH_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_TCPIP_IPSECDB_GETANDRESETPOLICYCOUNT_ENTRY
/** \brief Entry point of function TcpIp_IpSecDb_GetAndResetPolicyCount() */
#define DBG_TCPIP_IPSECDB_GETANDRESETPOLICYCOUNT_ENTRY(a,b,c)
#endif

#ifndef DBG_TCPIP_IPSECDB_GETANDRESETPOLICYCOUNT_EXIT
/** \brief Exit point of function TcpIp_IpSecDb_GetAndResetPolicyCount() */
#define DBG_TCPIP_IPSECDB_GETANDRESETPOLICYCOUNT_EXIT(a,b,c)
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#else /* TCPIP_DISABLE_INTERNAL_TRACE_MACROS_FOR_TESTING */
#define TCPIP_TRACE_INTERNAL_H_DISABLED
#endif /* TCPIP_DISABLE_INTERNAL_TRACE_MACROS_FOR_TESTING */
#endif /* ifndef TCPIP_TRACE_INTERNAL_H */
/*==================[end of file]===========================================*/
