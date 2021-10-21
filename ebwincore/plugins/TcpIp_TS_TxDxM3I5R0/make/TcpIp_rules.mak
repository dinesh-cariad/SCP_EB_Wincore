#
# \file
#
# \brief AUTOSAR TcpIp
#
# This file contains the implementation of the AUTOSAR
# module TcpIp.
#
# \version 3.5.13
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

#################################################################
# REGISTRY

ifndef TS_MERGED_COMPILE
TS_MERGED_COMPILE := TRUE
endif

ifndef TS_TCPIP_MERGED_COMPILE
TS_TCPIP_MERGED_COMPILE := $(TS_MERGED_COMPILE)
endif

ifeq ($(TS_TCPIP_MERGED_COMPILE),TRUE)

TcpIp_src_FILES +=                              \
          $(TcpIp_CORE_PATH)/src/TcpIp_Merged.c \

else

TcpIp_src_FILES_Socket := $(TcpIp_CORE_PATH)/src/TcpIp_Socket.c \
                          $(TcpIp_CORE_PATH)/src/TcpIp_Socket_UDP.c \
                          $(TcpIp_CORE_PATH)/src/TcpIp_Socket_TCP.c \

TcpIp_src_FILES_IpAddrM := $(TcpIp_CORE_PATH)/src/TcpIp_IpAddrM.c \
                           $(TcpIp_CORE_PATH)/src/TcpIp_IpAddrM_CtrlSM.c \
                           $(TcpIp_CORE_PATH)/src/TcpIp_IpAddrM_LocalAddrSM.c \
                           $(TcpIp_CORE_PATH)/src/TcpIp_IpAddrM_AssignSM.c \

TcpIp_src_FILES_IpV4 := $(TcpIp_CORE_PATH)/src/TcpIp_IpV4.c \
                        $(TcpIp_CORE_PATH)/src/TcpIp_IpV4_Arp.c \
                        $(TcpIp_CORE_PATH)/src/TcpIp_IpV4_Icmp.c \
                        $(TcpIp_CORE_PATH)/src/TcpIp_IpV4_LinkLocal.c \
                        $(TcpIp_CORE_PATH)/src/TcpIp_IpV4_StaticIp.c \
                        $(TcpIp_CORE_PATH)/src/TcpIp_IpV4_AddrConflDetect.c \

TcpIp_src_FILES_IpV6 := $(TcpIp_CORE_PATH)/src/TcpIp_IpV6.c \
                        $(TcpIp_CORE_PATH)/src/TcpIp_IpV6_Icmp.c \
                        $(TcpIp_CORE_PATH)/src/TcpIp_IpV6_LinkLocal.c \
                        $(TcpIp_CORE_PATH)/src/TcpIp_IpV6_StaticIp.c \

TcpIp_src_FILES_Ndp := $(TcpIp_CORE_PATH)/src/TcpIp_Ndp.c \
                       $(TcpIp_CORE_PATH)/src/TcpIp_Ndp_Rpd.c \
                       $(TcpIp_CORE_PATH)/src/TcpIp_Ndp_NCache.c \
                       $(TcpIp_CORE_PATH)/src/TcpIp_Ndp_Dad.c \

TcpIp_src_FILES_Ip := $(TcpIp_CORE_PATH)/src/TcpIp_Ip.c \
                      $(TcpIp_CORE_PATH)/src/TcpIp_Ip_Reass.c \
                      $(TcpIp_CORE_PATH)/src/TcpIp_Ip_Frag.c \

TcpIp_src_FILES_Udp := $(TcpIp_CORE_PATH)/src/TcpIp_Udp.c \

TcpIp_src_FILES_Tcp := $(TcpIp_CORE_PATH)/src/TcpIp_Tcp.c \
                       $(TcpIp_CORE_PATH)/src/TcpIp_Tcp_SM.c \
                       $(TcpIp_CORE_PATH)/src/TcpIp_Tcp_rxData.c \
                       $(TcpIp_CORE_PATH)/src/TcpIp_Tcp_txData.c \
                       $(TcpIp_CORE_PATH)/src/TcpIp_Tcp_rxOutOfOrder.c \

TcpIp_src_FILES_DhcpV4 := $(TcpIp_CORE_PATH)/src/TcpIp_DhcpV4.c \
                          $(TcpIp_CORE_PATH)/src/TcpIp_Dhcp.c \

TcpIp_src_FILES_DhcpV6 := $(TcpIp_CORE_PATH)/src/TcpIp_DhcpV6.c \
                          $(TcpIp_CORE_PATH)/src/TcpIp_Dhcp.c \

TcpIp_src_FILES_Memory := $(TcpIp_CORE_PATH)/src/TcpIp_Memory.c \

TcpIp_src_FILES := $(TcpIp_CORE_PATH)/src/TcpIp.c \
                   $(TcpIp_CORE_PATH)/src/TcpIp_UpLDummy.c \
                   $(TcpIp_CORE_PATH)/src/TcpIp_Crypto.c

TcpIp_src_FILES_IpSec := $(TcpIp_CORE_PATH)/src/TcpIp_IpSec.c \

TcpIp_src_FILES_IpSecDb := $(TcpIp_CORE_PATH)/src/TcpIp_IpSecDb.c \

TcpIp_Units ?= Socket IpAddrM Ip IpV4 Udp Tcp Memory DhcpV4 IpV6 DhcpV6 Ndp IpSec IpSecDb
TcpIp_src_FILES += $(foreach unit,$(TcpIp_Units),$(TcpIp_src_FILES_$(unit)))

endif

TcpIp_src_FILES +=  $(TcpIp_OUTPUT_PATH)/src/TcpIp_Lcfg.c

LIBRARIES_TO_BUILD   += TcpIp_src

# Fill the list with post build configuration files needed to build the post build binary.
TcpIp_pbconfig_FILES := $(wildcard $(TcpIp_OUTPUT_PATH)/src/TcpIp_PBcfg.c)

ifneq ($(strip $(TcpIp_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += TcpIp_pbconfig
LIBRARIES_TO_BUILD += TcpIp_pbconfig
endif
