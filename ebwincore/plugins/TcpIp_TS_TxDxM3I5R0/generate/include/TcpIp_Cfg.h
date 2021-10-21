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

[!SELECT "as:modconf('TcpIp')[1]"!][!//
[!INCLUDE "../../generate_macros/TcpIp_Vars.m"!][!//
[!//
#ifndef TCPIP_CFG_H
#define TCPIP_CFG_H

/* It is required by AUTOSAR include the headerfile that contains the post-build/link-time symbolic IDs here */
/* This mixture of config classes can be omitted for module internal purposes */
#ifndef TCPIP_NO_CFGCLASSMIX_REQUIRED
#include <TcpIp_SymbolicNames_PBcfg.h>
#endif /* TCPIP_NO_CFGCLASSMIX_REQUIRED */

#undef TCPIP_NO_CFGCLASSMIX_REQUIRED

[!CALL "GuardedDefine", "Comment"="'Development error detection support'",
 "Name"="'TCPIP_DEV_ERROR_DETECT'"!][!//
[!IF "TcpIpGeneral/TcpIpDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'VersionInfo API support'",
  "Name"="'TCPIP_VERSION_INFO_API'"!][!//
[!IF "TcpIpGeneral/TcpIpVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'IPv4 support'",
  "Name"="'TCPIP_IPV4_ENABLED'"!][!//
[!IF "TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'IPv6 support'",
  "Name"="'TCPIP_IPV6_ENABLED'"!][!//
[!IF "TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'UDP support'",
  "Name"="'TCPIP_UDP_ENABLED'"!][!//
[!IF "TcpIpGeneral/TcpIpUdpEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'UDP limited broadcast and all nodes multicast reception'",
  "Name"="'TCPIP_UDP_BROADCAST_RECEPTION_ENABLED'"!][!//
[!IF "TcpIpGeneral/TcpIpUdpAllowImplicitBroadcastReception = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'TCP support'",
  "Name"="'TCPIP_TCP_ENABLED'"!][!//
[!IF "TcpIpGeneral/TcpIpTcpEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Get and Reset Measurement Data API support'",
  "Name"="'TCPIP_GETANDRESETMEASUREMENTDATA_API'"!][!//
[!IF "TcpIpGeneral/TcpIpGetAndResetMeasurementDataApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'TcpIpDhcpGetStatusApi API support'",
  "Name"="'TCPIP_DHCPGETSTATUS_API'"!][!//
[!IF "TcpIpGeneral/TcpIpDhcpGetStatusApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'TcpIpIcmpTransmitError API support'",
  "Name"="'TCPIP_ICMPTRANSMITERROR_API'"!][!//
[!IF "TcpIpGeneral/TcpIpIcmpTransmitErrorApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'TCP keep alive support'",
  "Name"="'TCPIP_TCP_KEEP_ALIVE_ENABLED'"!][!//
[!IF "TcpIpConfig/TcpIpTcpConfig/TcpIpTcpKeepAliveEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'TCP segment reception out-of-order support'",
  "Name"="'TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED'"!][!//
[!IF "TcpIpConfig/TcpIpTcpConfig/TcpIpTcpOutOfOrderReceptionEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "TcpIpConfig/TcpIpTcpConfig/TcpIpTcpOutOfOrderReceptionEnabled = 'true'"!][!//

[!CALL "GuardedDefine", "Comment"="'TCP number of buffers for out-of-order segment reception'",
  "Name"="'TCPIP_TCP_RINGBUFFER_COUNT'"!][!//
[!"num:i(TcpIpConfig/TcpIpTcpConfig/TcpIpTcpOutOfOrderReceptionBufferCount)"!]U

[!CALL "GuardedDefine", "Comment"="'TCP buffer size for out-of-order segment reception'",
  "Name"="'TCPIP_TCP_RINGBUFFER_SIZE'"!][!//
[!"num:i(TcpIpConfig/TcpIpTcpConfig/TcpIpTcpOutOfOrderReceptionBufferSize)"!]U

[!CALL "GuardedDefine", "Comment"="'TCP hole list size for out-of-order segment reception'",
  "Name"="'TCPIP_TCP_HOLE_LIST_SIZE'"!][!//
[!"num:i(TcpIpConfig/TcpIpTcpConfig/TcpIpTcpOutOfOrderReceptionHoleListSize)"!]U

[!ENDIF!][!//

[!CALL "GuardedDefine", "Comment"="'TCP unpredictable initial sequence number support'",
  "Name"="'TCPIP_TCP_UNPREDICTABLE_INIT_SEQ_NUM_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpTcpEnabled = 'true') and (node:exists(TcpIpConfig/TcpIpTcpConfig/TcpIpTcpUnpredictableSeqNumbers/TcpIpTcpUnpredictableSeqNumbersKeyGenerateJobId) = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'TCP segment reception out-of-order support'",
  "Name"="'TCPIP_TCP_SYN_COOKIES_ENABLED'"!][!//
[!IF "node:exists(TcpIpConfig/TcpIpTcpConfig/TcpIpTcpSynCookies/TcpIpTcpSynCookiesKey1GenerateJobId) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "node:exists(TcpIpConfig/TcpIpTcpConfig/TcpIpTcpSynCookies/TcpIpTcpSynCookiesKey1GenerateJobId) = 'true'"!][!//
[!CALL "GuardedDefine", "Comment"="'The timebase ID used to retrieve the current time from StbM for generatin TCP SYN cookie.'",
  "Name"="'TCPIP_TCP_SYN_COOKIES_TIMEBASE_ID'"!][!//
[!"num:i(node:ref(TcpIpConfig/TcpIpTcpConfig/TcpIpTcpSynCookies/TcpIpTcpSynCookiesTimebaseRef)/StbMSynchronizedTimeBaseIdentifier)"!]U[!//
[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'TCP copy window check'",
  "Name"="'TCPIP_TCP_COPY_WINDOW_CHECK_ENABLED'"!][!//
[!IF "TcpIpConfig/TcpIpTcpConfig/TcpIpTcpCopyWindowCheckEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'TCP RST segment ignored reception in state TIME-WAIT'",
  "Name"="'TCPIP_TCP_TIME_WAIT_RX_RST_IGNORE_ENABLED'"!][!//
[!IF "TcpIpConfig/TcpIpTcpConfig/TcpIpTcpTimeWaitRstIgnoreReceptionEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'IcmpV4 support'",
  "Name"="'TCPIP_ICMPV4_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true') and (TcpIpGeneral/TcpIpIpV4General/TcpIpIcmpEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'IcmpV6 support is mandatory in IPV6'",
  "Name"="'TCPIP_ICMPV6_ENABLED'"!]TCPIP_IPV6_ENABLED

[!CALL "GuardedDefine", "Comment"="'IpV4 link local address assignment support'",
  "Name"="'TCPIP_IPV4_LINK_LOCAL_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV4General/TcpIpAutoIpEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'IpV6 link local address assignment support'",
  "Name"="'TCPIP_IPV6_LINK_LOCAL_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and (TcpIpGeneral/TcpIpIpV6General/TcpIpSlaacLinkLocalEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'DHCP Client support'",
  "Name"="'TCPIP_DHCPV4_CLIENT_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true') and (TcpIpGeneral/TcpIpIpV4General/TcpIpDhcpClientEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'DHCP Simple Client support'",
  "Name"="'TCPIP_DHCPV4_SIMPLE_CLIENT_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true') and (TcpIpGeneral/TcpIpIpV4General/TcpIpDhcpSimpleClientEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'DHCPv6 Client support'",
  "Name"="'TCPIP_DHCPV6_CLIENT_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and (TcpIpGeneral/TcpIpIpV6General/TcpIpDhcpV6ClientEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

 [!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpArNudConfig/TcpIpNdpMaxNeighborCacheSize)"!][!//
[!CALL "GuardedDefine", "Comment"="'maximum value of Neighbor Cache Entries'",
  "Name"="'TCPIP_NDP_CACHESIZE_MAX'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpArNudConfig/TcpIpNdpMaxNeighborCacheSize)"!]U

[!ENDIF!][!//

/**\brief Duplicate address detection macros */
[!CALL "GuardedDefine", "Comment"="'Duplicate address detection support'",
   "Name"="'TCPIP_NDP_DAD_CONFIG_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and (TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpSlaacConfig/TcpIpNdpSlaacDadNumberOfTransmissions > 0)"!][!//
STD_ON /** TCPIP_NDP_DAD_CONFIG_ENABLED */

[!CALL "GuardedDefine", "Comment"="'Stateless auto assignment configuration support'",
  "Name"="'TCPIP_NDP_DAD_CONFIG_DELAY_ENABLED'"!][!//
[!IF "(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpSlaacConfig/TcpIpNdpSlaacDelayEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Optimistic duplicate address detection support'",
  "Name"="'TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED'"!][!//
[!IF "(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpSlaacConfig/TcpIpNdpSlaacOptimisticDadEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'DhcpV6 duplicate address detection support'",
  "Name"="'TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED'"!][!//
[!IF "(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpDhcpV6Config/*[1]/TcpIpDhcpV6SlaacDadEnabled) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'maximum value for the address configuration delay'",
  "Name"="'TCPIP_NDP_DAD_CONFIG_TIMEOUT'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpSlaacConfig/TcpIpNdpSlaacDadRetransmissionDelay * 10)"!]U

[!CALL "GuardedDefine", "Comment"="'maximum value of Neighbor Cache Entries'",
  "Name"="'TCPIP_NDP_DAD_CONFIG_NUM_OF_SOL'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpSlaacConfig/TcpIpNdpSlaacDadNumberOfTransmissions)"!]U

[!CALL "GuardedDefine", "Comment"="'Number IpV6 Unicast addresses'",
  "Name"="'TCPIP_NDP_DAD_NUM_OF_ENTRIES'"!][!//
[!"$IpV6NdpDadCount"!]

[!ELSE!][!//
STD_OFF

[!CALL "GuardedDefine", "Comment"="'Stateless auto assignment configuation support'",
  "Name"="'TCPIP_NDP_DAD_CONFIG_DELAY_ENABLED'"!] STD_OFF

[!CALL "GuardedDefine", "Comment"="'Optimistic duplicate address detection support'",
  "Name"="'TCPIP_NDP_DAD_CONFIG_OPTIMISTIC_DAD_ENABLED'"!] STD_OFF

[!CALL "GuardedDefine", "Comment"="'DhcpV6 duplicate address detection support'",
  "Name"="'TCPIP_DHCPV6_DAD_CONFIG_DAD_ENABLED'"!] STD_OFF

[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Duplicate Address Detection Callout'",
  "Name"="'TCPIP_DAD_FCT_ENABLE'"!][!//
[!IF "node:exists(TcpIpConfig/TcpIpDuplicateAddressDetectionConfig)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Router and Prefix Discovery support'",
  "Name"="'TCPIP_NDP_RPD_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and (TcpIpGeneral/TcpIpIpV6General/TcpIpNdpPrefixAndRouterDiscoveryEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Retransmission Solicitation Delay support'",
  "Name"="'TCPIP_NDP_RND_RTR_SOLICIT_DELAY_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and (TcpIpGeneral/TcpIpIpV6General/TcpIpNdpPrefixAndRouterDiscoveryEnabled = 'true') and
      (TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpPrefixRouterDiscoveryConfig/TcpIpNdpRndRtrSolicitationDelayEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

 [!IF "(node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpPrefixRouterDiscoveryConfig/TcpIpNdpPrefixListSize)) and
       (TcpIpGeneral/TcpIpIpV6General/TcpIpNdpPrefixAndRouterDiscoveryEnabled = 'true')"!][!//
[!CALL "GuardedDefine", "Comment"="'Maximum number of router address assignments supported'",
  "Name"="'TCPIP_NUM_IPV6_ROUTER_ASSIGN_MAX'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpPrefixRouterDiscoveryConfig/TcpIpNdpPrefixListSize)"!]U
[!ENDIF!][!//

 [!IF "(node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpPrefixRouterDiscoveryConfig/TcpIpNdpDefaultRouterListSize)) and
       (TcpIpGeneral/TcpIpIpV6General/TcpIpNdpPrefixAndRouterDiscoveryEnabled = 'true')"!][!//
[!CALL "GuardedDefine", "Comment"="'Maximum number of supported routers per controller'",
  "Name"="'TCPIP_NDP_DEFAULT_ROUTER_LIST_SIZE'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpPrefixRouterDiscoveryConfig/TcpIpNdpDefaultRouterListSize)"!]U
[!ENDIF!][!//

[!CALL "GuardedDefine", "Comment"="'Destination cache support'",
  "Name"="'TCPIP_NDP_DESTINATION_CACHE_ENABLED'"!][!//
[!IF "(node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpPrefixRouterDiscoveryConfig/TcpIpNdpDestinationCacheEnabled)) and
      (TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpPrefixRouterDiscoveryConfig/TcpIpNdpDestinationCacheEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

 [!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpPrefixRouterDiscoveryConfig/TcpIpNdpDestinationCacheSize)"!][!//
[!CALL "GuardedDefine", "Comment"="'Maximum number of entries per controller in the destination cache.'",
  "Name"="'TCPIP_NDP_DESTINATION_CACHE_SIZE'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpPrefixRouterDiscoveryConfig/TcpIpNdpDestinationCacheSize)"!]U
[!ENDIF!][!//

[!SELECT "TcpIpConfig/TcpIpIpConfig/TcpIpIpFragmentationConfig"!][!//
[!CALL "GuardedDefine", "Comment"="'IP RX fragmentation support'",
  "Name"="'TCPIP_IP_FRAGMENT_RX_ENABLED'"!][!//
[!IF "TcpIpIpFragmentationRxEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'IP TX fragmentation support'",
  "Name"="'TCPIP_IP_FRAGMENT_TX_ENABLED'"!][!//
[!IF "TcpIpIpFragmentationTxEnabled = 'OUTOFORDER'"!]TCPIP_IP_FRAGMENT_TX_OUTOFORDER[!//
[!ELSEIF "TcpIpIpFragmentationTxEnabled = 'INORDER'"!]TCPIP_IP_FRAGMENT_TX_INORDER[!//
[!ELSE!]STD_OFF[!//
[!ENDIF!]

[!CALL "GuardedDefine", "Name"="'TCPIP_IP_FRAGMENT_MEMRESERVED'"!][!//
[!"num:i(TcpIpIpFragMemReserved)"!]U
[!ENDSELECT!][!//

[!CALL "GuardedDefine", "Comment"="'mainfunction prescaler for 1s interval'",
  "Name"="'TCPIP_MAINFUNCTION_PRESCALER_1S'"!][!//
[!"num:i($TimerInterval_1s_ticks)"!]U

[!CALL "GuardedDefine", "Comment"="'mainfunction prescaler for 100ms interval'",
  "Name"="'TCPIP_MAINFUNCTION_PRESCALER_100MS'"!][!//
[!"num:i($TimerInterval_100ms_ticks)"!]U

[!CALL "GuardedDefine", "Comment"="'DHCP Fully Qualified Domain Name support'",
  "Name"="'TCPIP_DHCPV4_FQDN_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true') and (TcpIpGeneral/TcpIpIpV4General/TcpIpDhcpClientEnabled = 'true')"!][!//
[!IF "TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpFQDNOptionEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Dhcp options support'",
  "Name"="'TCPIP_DHCPV4_OPTIONS_ENABLED'"!][!//
[!IF "((TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpConfigurableOptionsEnabled) = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'DHCP Fully Qualified Domain Name support'",
  "Name"="'TCPIP_DHCPV6_FQDN_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and (TcpIpGeneral/TcpIpIpV6General/TcpIpDhcpV6ClientEnabled = 'true')"!][!//
[!IF "TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpDhcpV6Config/*[1]/TcpIpDhcpV6FQDNOptionEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ELSE!]STD_OFF[!//
[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'DhcpV6 options support'",
  "Name"="'TCPIP_DHCPV6_OPTIONS_ENABLED'"!][!//
[!IF "((TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpDhcpV6Config/*[1]/TcpIpDhcpV6ConfigurableOptionsEnabled) = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'ICMP message handler configuration'",
  "Name"="'TCPIP_ICMPV4_RX_FCT_ENABLED'"!][!//
[!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpIcmpConfig/TcpIpIcmpMsgHandler)"!][!//
STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'IPV4 ARP packet filter'",
  "Name"="'TCPIP_IPV4_ARP_PACKET_FILTER_ENABLE'"!][!//
[!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpIpV4ArpPacketFilter)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'TCP option filtering'",
  "Name"="'TCPIP_TCP_OPTION_FILTER_ENABLE'"!][!//
[!IF "(TcpIpConfig/TcpIpTcpConfig/TcpIpTcpOptionFilterEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'IPV6 extension header filtering'",
  "Name"="'TCPIP_IPV6_EXTENSIONHDR_FILTER_ENABLE'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6ExtensionHeaderFilterEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Physical address change handler configuration'",
  "Name"="'TCPIP_PHYSADDR_CHANGE_FCT_ENABLE'"!][!//
[!IF "node:exists(TcpIpConfig/TcpIpPhysAddrConfig/TcpIpPhysAddrChgHandler)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'maximum number overlapping socket instances possible during demultiplexing'",
  "Name"="'TCPIP_UDP_MAXIMUM_SOCKET_INSTANCES'"!][!//
[!"num:i(TcpIpGeneral/TcpIpUdpMaxMulticastRxInstances)"!]U

[!CALL "GuardedDefine", "Comment"="'enable/disable NvM support for storing IP addresses'",
  "Name"="'TCPIP_NVM_STORING_ENABLED'"!][!//
[!IF "node:exists(TcpIpConfig/TcpIpNvmBlock/TcpIpNvmBlockDescriptorRef) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "node:exists(TcpIpConfig/TcpIpNvmBlock/TcpIpNvmBlockDescriptorRef) = 'true'"!][!//
[!CALL "GuardedDefine", "Comment"="'The block identifier from NvM'",
  "Name"="'TCPIP_NVM_BLOCK_ID'"!][!//
[!"num:i(node:ref(TcpIpConfig/TcpIpNvmBlock/TcpIpNvmBlockDescriptorRef)/NvMNvramBlockIdentifier)"!]U[!//

[!CALL "GuardedDefine", "Comment"="'Size of internal NvM memory'",
  "Name"="'TCPIP_NVM_BLOCK_SIZE'"!][!"num:i(TcpIpConfig/TcpIpNvmBlock/TcpIpNvmBlockSize)"!]U

[!ENDIF!]

[!/*
socketMask, accessible as a C preprocessor macro via TCPIP_SOCKETPAGE_MASK, is a 4 bit mask
where the elements correspond to the configured socket pages in the following way from MSB to LSB [i3|i2|i1|i0]:
i0: 1 if IPv4 and UDP are enabled and more then 0 UDP IPv4 max sockets are configured, 0 otherwise.
i1: 1 if IPv6 and UDP are enabled and more then 0 UDP IPv6 max sockets are configured, 0 otherwise.
i2: 1 if IPv4 and TCP are enabled and more then 0 TCP IPv4 max sockets are configured, 0 otherwise.
i3: 1 if IPv6 and TCP are enabled and more then 0 TCP IPv6 max sockets are configured, 0 otherwise.
*/!][!//
[!VAR "socketMask"="num:i(0)"!][!//

[!/* socketMask += 1 if IPv4 and UDP enabled and more then 0 UDP IPv4 max sockets configured. */!][!//
[!CALL "GuardedDefine", "Comment"="'maximum number of IpV4 Udp sockets'",
  "Name"="'TCPIP_SOCKET_NUM_UDP_IPV4'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true') and (TcpIpGeneral/TcpIpUdpEnabled = 'true')"!][!/*
*/!][!IF "(TcpIpGeneral/TcpIpUdpSocketMax > 0)"!][!VAR "socketMask"="num:i(1)"!][!ENDIF!][!//
[!"num:i(TcpIpGeneral/TcpIpUdpSocketMax)"!]U[!ELSE!]0U[!ENDIF!]

[!/* socketMask += 4 if IPv4 and TCP enabled and more then 0 TCP IPv4 max sockets configured. */!][!//
[!CALL "GuardedDefine", "Comment"="'maximum number of IpV4 Tcp sockets'",
  "Name"="'TCPIP_SOCKET_NUM_TCP_IPV4'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true') and (TcpIpGeneral/TcpIpTcpEnabled = 'true')"!][!/*
*/!][!IF "(TcpIpGeneral/TcpIpTcpSocketMax > 0)"!][!VAR "socketMask"="bit:or($socketMask, num:i(4))"!][!ENDIF!][!//
[!"num:i(TcpIpGeneral/TcpIpTcpSocketMax)"!]U[!ELSE!]0U[!ENDIF!]

[!/* socketMask += 2 if IPv6 and UDP enabled and more then 0 UDP IPv6 max sockets configured. */!][!//
[!CALL "GuardedDefine", "Comment"="'maximum number of IpV6 Udp sockets'",
  "Name"="'TCPIP_SOCKET_NUM_UDP_IPV6'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and (TcpIpGeneral/TcpIpUdpEnabled = 'true')"!][!/*
*/!][!IF "(TcpIpGeneral/TcpIpUdpV6SocketMax > 0)"!][!VAR "socketMask"="bit:or($socketMask, num:i(2))"!][!ENDIF!][!//
[!"num:i(TcpIpGeneral/TcpIpUdpV6SocketMax)"!]U[!ELSE!]0U[!ENDIF!]

[!/* socketMask += 8 if IPv6 and TCP enabled and more then 0 TCP IPv6 max sockets configured. */!][!//
[!CALL "GuardedDefine", "Comment"="'maximum number of IpV6 Tcp sockets'",
  "Name"="'TCPIP_SOCKET_NUM_TCP_IPV6'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and (TcpIpGeneral/TcpIpTcpEnabled = 'true')"!][!/*
*/!][!IF "(TcpIpGeneral/TcpIpTcpV6SocketMax > 0)"!][!VAR "socketMask"="bit:or($socketMask, num:i(8))"!][!ENDIF!][!//
[!"num:i(TcpIpGeneral/TcpIpTcpV6SocketMax)"!]U[!ELSE!]0U[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'ICMPV6 message handler configuration'",
  "Name"="'TCPIP_ICMPV6_RX_FCT_ENABLED'"!][!//
[!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpIcmpV6Config/TcpIpIcmpV6MsgHandler)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'socket page mask adjusted to the enabled configurations'",
  "Name"="'TCPIP_SOCKETPAGE_MASK'"!][!//
[!"num:i($socketMask)"!]U

[!CALL "GuardedDefine", "Comment"="'number of configured socket pages'",
  "Name"="'TCPIP_SOCKETPAGE_NUM_VALID_CONFIGS'"!][!//
[!CALL "HammingWeight4", "mask"="$socketMask"!]U

[!/*
TCPIP_SOCKETPAGE_CONFIG_INDEX is an 8 bit lookup table used to map static socket page indices to configured ones.
The 8 bits from MSB to LSB [i7|i6|i5|i4|i3|i2|i1|i0] correspond to the configured socket pages in the following way:
Description:  Static socket page index => Configured socket page index
UDP IPv4:     0 =>                        [i1|i0] (always [0|0])
UDP IPv6:     1 =>                        [i3|i2]
TCP IPv4:     2 =>                        [i5|i4]
TCP IPv6:     3 =>                        [i7|i6]
The lookup/mapping is done in the implementation using TcpIp_Socket_getProtocolDomainDescriptorOffset.
It is only intended to be used with static socket page indices that are configured, i.e. it has to be checked via
TCPIP_SOCKET_ISVALIDSOCKETPAGE before!
*/!][!//
[!CALL "GuardedDefine", "Comment"="'socket page index adjusted to the enabled configurations'",
  "Name"="'TCPIP_SOCKETPAGE_CONFIG_INDEX'"!][!//
[!VAR "SocketPageUdpIpV6"!][!CALL "HammingWeight4", "mask"="bit:and($socketMask, 1)"!][!ENDVAR!][!//
[!VAR "SocketPageTcpIpV4"!][!CALL "HammingWeight4", "mask"="bit:and($socketMask, 3)"!][!ENDVAR!][!//
[!VAR "SocketPageTcpIpV6"!][!CALL "HammingWeight4", "mask"="bit:and($socketMask, 7)"!][!ENDVAR!][!//
[!"num:i(bit:or(bit:or(bit:shl($SocketPageUdpIpV6, 2), bit:shl($SocketPageTcpIpV4, 4)), bit:shl($SocketPageTcpIpV6, 6)))"!]U

/* ----- RX policy check ingress handler ----- */
[!CALL "GuardedDefine", "Comment"="'RX policy check ingress handler'",
  "Name"="'TCPIP_RX_POLICY_CHECK_INGRESS_FCT_ENABLED'"!][!//
[!IF "node:exists(TcpIpConfig/TcpIpRxPolicyCheckIngressHandlerConfig)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* ----- RX policy check routing handler ----- */
[!CALL "GuardedDefine", "Comment"="'RX policy check routing handler'",
  "Name"="'TCPIP_RX_POLICY_CHECK_ROUTING_FCT_ENABLED'"!][!//
[!IF "node:exists(TcpIpConfig/TcpIpRxPolicyCheckRoutingHandlerConfig)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* ----- RX packet post process handler ----- */
[!CALL "GuardedDefine", "Comment"="'RX packet post process handler'",
  "Name"="'TCPIP_RX_PACKET_POSTPROCESS_FCT_ENABLED'"!][!//
[!IF "node:exists(TcpIpConfig/TcpIpRxPacketPostProcessHandlerConfig)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* ----- RX packet drop handler ----- */
[!CALL "GuardedDefine", "Comment"="'RX packet drop handler'",
  "Name"="'TCPIP_RX_PACKET_DROP_FCT_ENABLED'"!][!//
[!IF "node:exists(TcpIpConfig/TcpIpRxPacketDropHandlerConfig)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* ----- Memory management ----- */
[!IF "(TcpIpGeneral/TcpIpTcpEnabled = 'true')"!]
[!CALL "GuardedDefine", "Comment"="'Total Number of configured memory blocks'",
  "Name"="'TCPIP_MEMORY_NUM_TOTAL_BLOCKS'"!][!"num:i(sum(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpMemoryConfig/TcpIpMemoryPool/*/TcpIpMemoryBlockCount))"!]U

[!VAR "TotalMemorySize"="num:i(0)"!][!//
[!LOOP "node:order(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpMemoryConfig/TcpIpMemoryPool/*)"!][!//
[!VAR "TotalMemorySize"="$TotalMemorySize + (TcpIpMemoryBlockSize * TcpIpMemoryBlockCount)"!][!//
[!ENDLOOP!][!//
[!CALL "GuardedDefine", "Comment"="'Total size of reserved memory'",
  "Name"="'TCPIP_MEMORY_TOTAL_SIZE'"!][!"num:i($TotalMemorySize)"!]U

[!IF "(node:exists(TcpIpConfig/TcpIpTcpConfig/TcpIpTcpUnpredictableSeqNumbers/TcpIpTcpUnpredictableSeqNumbersKeyGenerateJobId) = 'true') or (node:exists(TcpIpConfig/TcpIpTcpConfig/TcpIpTcpSynCookies/TcpIpTcpSynCookiesKey1GenerateJobId) = 'true')"!]
/* ----- Internal hash data array ----- */
[!CALL "GuardedDefine", "Comment"="'Size of internal hash data array, domain, remote ip address and port, local address id and port and it is also uint32 alligned'",
  "Name"="'TCPIP_TCP_INTERNAL_HASH_ADDR_PORT_SIZE'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true')"!][!"num:i(24)"!]U[!ELSE!][!"num:i(12)"!]U[!ENDIF!]
[!ENDIF!]

[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'ARP DAD enabled'",
  "Name"="'TCPIP_ARP_DAD_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV4General/TcpIpAutoIpEnabled = 'true') or (TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpArpProbingEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'DhcpV4 DAD enabled'",
  "Name"="'TCPIP_DHCPV4_CONFIG_DAD_ENABLED'"!][!//
[!IF "(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpArpProbingEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "(TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true')"!][!//

[!CALL "GuardedDefine", "Comment"="'Size of ARP table per ethernet controller'",
  "Name"="'TCPIP_ARP_TABLESIZE_MAX'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpArpConfig/*[1]/TcpIpArpTableSizeMax)"!]U

[!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpArpProbingEnabled)"!][!//

[!CALL "GuardedDefine", "Comment"="'Maximum number of ARP DAD entries'",
  "Name"="'TCPIP_NUM_ARP_DAD_ENTRIES'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV4General/TcpIpAutoIpEnabled = 'true') and (TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpArpProbingEnabled = 'true')"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpAutoIpConfig/*[1]/TcpIpAutoIpv4EntriesMax + TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpIpv4EntriesMax)"!]U
[!ELSEIF "(TcpIpGeneral/TcpIpIpV4General/TcpIpAutoIpEnabled = 'false') and (TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpArpProbingEnabled = 'true')"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpIpv4EntriesMax)"!]U
[!ELSEIF "(TcpIpGeneral/TcpIpIpV4General/TcpIpAutoIpEnabled = 'true') and (TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpArpProbingEnabled = 'false')"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpAutoIpConfig/*[1]/TcpIpAutoIpv4EntriesMax)"!]U
[!ELSE!][!//
[!"num:i(0)"!]U
[!ENDIF!][!//

[!ELSE!][!//

[!CALL "GuardedDefine", "Comment"="'Maximum number of ARP DAD entries'",
  "Name"="'TCPIP_NUM_ARP_DAD_ENTRIES'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV4General/TcpIpAutoIpEnabled = 'true')"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpAutoIpConfig/*[1]/TcpIpAutoIpv4EntriesMax)"!]U
[!ELSE!][!//
[!"num:i(0)"!]U
[!ENDIF!][!//

[!ENDIF!][!//

[!IF "(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpArpProbingEnabled = 'true')"!]
[!CALL "GuardedDefine", "Comment"="'DhcpV4 DAD probing parameter'",
  "Name"="'TCPIP_DHCPV4_DAD_CONFIG_DEFAULT_PROBING_PARAM'"!][!//
[!IF "(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpArpProbingType = 'PROBING_DEFAULT')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ENDIF!]

[!ENDIF!][!//

[!CALL "GuardedDefine", "Comment"="'Maximum number of local addrIds supported'",
  "Name"="'TCPIP_NUM_LOCALADDRENTRIES'"!]\
  (TCPIP_NUM_IPV4_LOCALADDRENTRIES + TCPIP_NUM_IPV6_LOCALADDRENTRIES)

[!CALL "GuardedDefine", "Comment"="'Maximum number of IPv4 local addrIds supported'",
  "Name"="'TCPIP_NUM_IPV4_LOCALADDRENTRIES'"!][!//
[!"num:i(TcpIpGeneral/TcpIpIpV4General/TcpIpLocalAddrIpv4EntriesMax)"!]U

[!CALL "GuardedDefine", "Comment"="'Maximum number of IPv6 local addrIds supported'",
  "Name"="'TCPIP_NUM_IPV6_LOCALADDRENTRIES'"!][!//
[!"num:i(TcpIpGeneral/TcpIpIpV6General/TcpIpLocalAddrIpv6EntriesMax)"!]U

[!CALL "GuardedDefine", "Comment"="'Maximum number of EthIfCtrl supported'",
  "Name"="'TCPIP_NUM_ETHIFCTRL'"!][!//
[!"num:i(TcpIpGeneral/TcpIpCtrlMax)"!]U

[!IF "(TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true') and (TcpIpGeneral/TcpIpIpV4General/TcpIpDhcpClientEnabled = 'true')"!][!//
[!CALL "GuardedDefine", "Comment"="'Maximum number of DHCP clients supported'",
  "Name"="'TCPIP_NUM_DHCPV4_CLIENTS'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpIpv4EntriesMax)"!]U

[!CALL "GuardedDefine", "Comment"="'Maximum size of the domain Name'",
  "Name"="'TCPIP_DHCPV4_DOMAINNAME_SIZE'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpIpv4DomainNameMaxSize)"!]U

[!IF "(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpConfigurableOptionsEnabled) = 'true'"!][!//
[!CALL "GuardedDefine", "Comment"="'number of TcpIpDhcpOption elements in config'",
  "Name"="'TCPIP_DHCPV4_MAX_OPTIONS'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpConfigurableOptionsEntriesMax)"!]U

[!CALL "GuardedDefine", "Comment"="'sum of all TcpIpDhcpOptionMaxLength elements in config'",
  "Name"="'TCPIP_DHCPV4_MAX_OPTION_DATA'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpConfigurableOptionsDataSizeMax)"!]U
[!ENDIF!][!//

[!ENDIF!][!//
[!IF "(TcpIpGeneral/TcpIpIpV4General/TcpIpAutoIpEnabled = 'true')"!][!//
[!CALL "GuardedDefine", "Comment"="'Maximum number of link local address assignments supported'",
  "Name"="'TCPIP_NUM_IPV4_LINK_LOCAL_ASSIGN'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpAutoIpConfig/*[1]/TcpIpAutoIpv4EntriesMax)"!]U

[!CALL "GuardedDefine", "Comment"="'netmask of link local address space'",
  "Name"="'TCPIP_IPV4_LINK_LOCAL_NETMASK'"!][!//
[!CALL "IpV4AddressToUINT32", "IpAddress" = "'255.255.0.0'", "endianCheck"="'true'"!]U

[!ENDIF!][!//
[!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and (TcpIpGeneral/TcpIpIpV6General/TcpIpSlaacLinkLocalEnabled = 'true')"!][!//
[!CALL "GuardedDefine", "Comment"="'Maximum number of link locasl address assignments supported'",
  "Name"="'TCPIP_NUM_IPV6_LINK_LOCAL_ASSIGN'"!][!//
[!"num:i(TcpIpGeneral/TcpIpIpV6General/TcpIpSlaacLinkLocalEntriesMax)"!]U

[!ENDIF!][!//
[!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and (TcpIpGeneral/TcpIpIpV6General/TcpIpDhcpV6ClientEnabled = 'true')"!][!//
[!CALL "GuardedDefine", "Comment"="'Maximum number of DHCPv6 clients supported'",
  "Name"="'TCPIP_NUM_DHCPV6_CLIENTS'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpDhcpV6Config/*[1]/TcpIpDhcpIpv6EntriesMax)"!]U

[!CALL "GuardedDefine", "Comment"="'Maximum size of the Server DUID'",
  "Name"="'TCPIP_DHCPV6_SERVER_DUID_SIZE'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpDhcpV6Config/*[1]/TcpIpDhcpIpv6ServerDuidMaxSize + 2)"!]U

[!CALL "GuardedDefine", "Comment"="'Maximum size of the domain Name'",
  "Name"="'TCPIP_DHCPV6_DOMAINNAME_SIZE'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpDhcpV6Config/*[1]/TcpIpDhcpIpv6DomainNameMaxSize)"!]U

[!IF "(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpDhcpV6Config/*[1]/TcpIpDhcpV6ConfigurableOptionsEnabled) = 'true'"!][!//
[!CALL "GuardedDefine", "Comment"="'number of TcpIpDhcpV6Option elements in config'",
  "Name"="'TCPIP_DHCPV6_MAX_OPTIONS'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpDhcpV6Config/*[1]/TcpIpDhcpV6ConfigurableOptionsEntriesMax)"!]U

[!CALL "GuardedDefine", "Comment"="'sum of all TcpIpDhcpV6OptionMaxLength elements in config'",
  "Name"="'TCPIP_DHCPV6_MAX_OPTION_DATA'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpDhcpV6Config/*[1]/TcpIpDhcpV6ConfigurableOptionsDataSizeMax)"!]U
[!ENDIF!][!//

[!ENDIF!][!//

[!IF "(node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpArNudConfig/TcpIpNdpDefensiveProcessing))"!][!//
[!CALL "GuardedDefine", "Comment"="'Ndp defensive processing supporting'",
  "Name"="'TCPIP_IPV6_NDP_DEFENSIVE_PROCESSING_ENABLED'"!][!//
[!IF "(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpArNudConfig/TcpIpNdpDefensiveProcessing = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ENDIF!][!//

[!CALL "GuardedDefine", "Comment"="'Compile time verification value'",
  "Name"="'TCPIP_CFG_SIGNATURE'"!][!//
[!"asc:getConfigSignature(as:modconf('TcpIp')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U

[!CALL "GuardedDefine", "Comment"="'PublicInfoSignature'",
  "Name"="'TCPIP_PUBLIC_INFO_SIGNATURE'"!][!//
[!"asc:getConfigSignature(node:difference(as:modconf('TcpIp')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('TcpIp')[1]/CommonPublishedInformation/Release))"!]U

[!CALL "GuardedDefine", "Comment"="'enable/disable relocateable postbuild config'",
  "Name"="'TCPIP_PBCFGM_SUPPORT_ENABLED'"!][!//
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('TcpIp')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'enable/disable relocateable config'",
  "Name"="'TCPIP_RELOCATABLE_CFG_ENABLE'"!][!//
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('TcpIp')) = 'true'"!][!//
[!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ELSE!][!IF "TcpIpGeneral/TcpIpRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'enable/disable TcpIp_MainFunctionTx'",
  "Name"="'TCPIP_MAINFUNCTIONTX_ENABLED'"!][!//
[!IF "TcpIpGeneral/TcpIpEnableMainFunctionTx = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* ----- IpSec ----- */
[!CALL "GuardedDefine", "Comment"="'Enables the availability to define bypass/discard policies'",
  "Name"="'TCPIP_FIREWALL'"!][!//
  (STD_ON + 1U)

[!CALL "GuardedDefine", "Comment"="'Enables the availability to define bypass/secured/discard policies and manual security associations'",
  "Name"="'TCPIP_STATIC_IPSEC'"!][!//
  ((TCPIP_FIREWALL) + 1U)

[!CALL "GuardedDefine", "Comment"="'Enables the availability to define bypass/secured/discard policies and manual/dynamic security associations'",
  "Name"="'TCPIP_IKEV2'"!][!//
  ((TCPIP_STATIC_IPSEC) + 1U)

[!CALL "GuardedDefine", "Comment"="'TcpIp security mode'",
  "Name"="'TCPIP_SECURITY_MODE'"!][!//
[!IF "TcpIpGeneral/TcpIpSecurityMode = 'FIREWALL'"!]TCPIP_FIREWALL[!//
[!ELSEIF "TcpIpGeneral/TcpIpSecurityMode = 'STATIC_IPSEC'"!]TCPIP_STATIC_IPSEC[!//
[!ELSEIF "TcpIpGeneral/TcpIpSecurityMode = 'IPSEC_IKE'"!]TCPIP_IKEV2[!//
[!ELSE!]STD_OFF[!//
[!ENDIF!]

[!IF "TcpIpGeneral/TcpIpSecurityMode = 'STATIC_IPSEC'"!][!//

[!CALL "GuardedDefine", "Comment"="'indicates the maximal generated ICV size'",
  "Name"="'TCPIP_IPSEC_MAX_ICV_LEN'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecGeneral/TcpIpIcvSizeMax)"!]U

[!CALL "GuardedDefine", "Comment"="'Number of security associations'",
  "Name"="'TCPIP_IPSEC_NUM_SEC_ASSOCIATIONS'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecGeneral/TcpIpMaxNumIpSecSecurityAssociation)"!]U

[!CALL "GuardedDefine", "Comment"="'Number of security associationsconfigurations'",
  "Name"="'TCPIP_IPSEC_NUM_SA_CONFIGS'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecGeneral/TcpIpMaxNumIpSecSecurityAssociationConfigurations)"!]U

[!CALL "GuardedDefine", "Comment"="'Maximal number of Ipsec connections'",
  "Name"="'TCPIP_IPSEC_MAX_NUM_CONNECTIONS'"!][!//
[!"num:i(TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecGeneral/TcpIpMaxNumIPsecConnections)"!]U

[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'enable/disable MAC filtering'",
  "Name"="'TCPIP_REMOTE_MAC_FILTERING_ENABLED'"!][!//
[!IF "(((TcpIpGeneral/TcpIpSecurityMode = 'STATIC_IPSEC') or (TcpIpGeneral/TcpIpSecurityMode = 'FIREWALL')) and (TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecGeneral/TcpIpIpSecRemotePhysAddrCheckEnable = 'true'))"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Indicates if GMAC is used for authentication in one or more configuration'",
 "Name"="'TCPIP_IPSEC_GMAC_ENABLED'"!][!//
[!IF "as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecGeneral/TcpIpIpSecGmacEnable = 'true'"!][!//
STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'IpSec error handler configuration'",
  "Name"="'TCPIP_IPSEC_ERROR_REPORT_FCT_ENABLED'"!][!//
[!IF "node:exists(TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecReportErrorHandler)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Traffic class value filtering'",
  "Name"="'TCPIP_IPV6_TRAFFIC_CLASS_FILTER_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and (TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6TrafficClassFilterEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'enable/disable Crypto for TCP syn cookies or unpredictable sequence numbers'",
  "Name"="'TCPIP_TCP_CRYPTO_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpTcpEnabled = 'true') and ((node:exists(TcpIpConfig/TcpIpTcpConfig/TcpIpTcpUnpredictableSeqNumbers/TcpIpTcpUnpredictableSeqNumbersKeyGenerateJobId) = 'true') or (node:exists(TcpIpConfig/TcpIpTcpConfig/TcpIpTcpSynCookies/TcpIpTcpSynCookiesKey1GenerateJobId)))"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


[!CALL "GuardedDefine", "Comment"="'Flow label value filtering'",
  "Name"="'TCPIP_IPV6_FLOW_LABEL_FILTER_ENABLED'"!][!//
[!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and (TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6FlowLabelFilterEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Indicates if counters for security policies are used'",
  "Name"="'TCPIP_IPSECDB_GETANDRESETMEASUREMENTDATA_ENABLE'"!][!//
[!IF "(((TcpIpGeneral/TcpIpSecurityMode = 'STATIC_IPSEC') or (TcpIpGeneral/TcpIpSecurityMode = 'FIREWALL')) and ((TcpIpGeneral/TcpIpGetAndResetMeasurementDataApi = 'true') or (node:exists(TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecReportErrorHandler))))"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* ----- defensive programming macros ----- */



/*------------------[Defensive programming]---------------------------------*/
[!SELECT "TcpIpDefensiveProgramming"!][!//

#if (defined TCPIP_DEFENSIVE_PROGRAMMING_ENABLED)
#error TCPIP_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define TCPIP_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../TcpIpGeneral/TcpIpDevErrorDetect  = 'true') and (TcpIpDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TCPIP_PRECONDITION_ASSERT_ENABLED)
#error TCPIP_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define TCPIP_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../TcpIpGeneral/TcpIpDevErrorDetect  = 'true') and (TcpIpDefProgEnabled = 'true') and (TcpIpPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TCPIP_POSTCONDITION_ASSERT_ENABLED)
#error TCPIP_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define TCPIP_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../TcpIpGeneral/TcpIpDevErrorDetect  = 'true') and (TcpIpDefProgEnabled = 'true') and (TcpIpPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TCPIP_UNREACHABLE_CODE_ASSERT_ENABLED)
#error TCPIP_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define TCPIP_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../TcpIpGeneral/TcpIpDevErrorDetect  = 'true') and (TcpIpDefProgEnabled = 'true') and (TcpIpUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TCPIP_INVARIANT_ASSERT_ENABLED)
#error TCPIP_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define TCPIP_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../TcpIpGeneral/TcpIpDevErrorDetect  = 'true') and (TcpIpDefProgEnabled = 'true') and (TcpIpInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TCPIP_STATIC_ASSERT_ENABLED)
#error TCPIP_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define TCPIP_STATIC_ASSERT_ENABLED           [!//
[!IF "(../TcpIpGeneral/TcpIpDevErrorDetect  = 'true') and (TcpIpDefProgEnabled = 'true') and (TcpIpStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif   /* TCPIP_CFG_H */
[!ENDSELECT!]
