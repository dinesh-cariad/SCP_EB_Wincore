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

#ifndef TCPIP_DHCP_PRIV_H
#define TCPIP_DHCP_PRIV_H

/*==================[inclusions]============================================*/
#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>          /* module internal headerfile */
#include <TcpIp_Trace.h>        /* debug and trace macros; must be last */
#include <TcpIp_Ip_Int.h>       /* used SW-unit interface file */
#include <TcpIp_DhcpV6_Int.h>   /* DhcpV6 SW-unit interface file */
#include <TcpIp_DhcpV4_Int.h>   /* DhcpV4 SW-unit interface file */

/*==================[macros]================================================*/

/** \brief TcpIp_DhcpGetStatus() service ID */
#define TCPIP_SID_DHCPGETSTATUS                                      0xF1U

/** \brief Macro that subsumes, that both \a TCPIP_DHCPV6_CLIENT_ENABLED and
 * \a TCPIP_DHCPV4_CLIENT_ENABLED are \a STD_ON)*/
#if ((TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON) || (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON))
#define TCPIP_DHCP_DUALSTACK TCPIP_DHCP_IF_V4_V6_BOTH(STD_OFF, STD_OFF, STD_ON)
#else
#define TCPIP_DHCP_DUALSTACK STD_OFF
#endif  /*((TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON) || (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON))*/

#if ((TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON) && (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON))
#  define TCPIP_DHCP_IF_V4_V6_BOTH(v4, v6, both) both
#elif (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON)
#  define TCPIP_DHCP_IF_V4_V6_BOTH(v4, v6, both) v6
#elif (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON)
#  define TCPIP_DHCP_IF_V4_V6_BOTH(v4, v6, both) v4
#endif

/*==================[type definitions]======================================*/
#if (TCPIP_DHCPGETSTATUS_API == STD_ON)
#if (TCPIP_DHCP_DUALSTACK == STD_ON)
/** \brief function type of version specific TcpIp_DhcpGetStatus() implementation */
typedef P2FUNC(Std_ReturnType, TCPIP_CODE, TcpIp_Dhcp_GetStatus_FctPtrType)
(
    TcpIp_LocalAddrIdType localAddressId,
    uint8* statusPtr
);

/** \brief Set of function pointers for dual stack ip operations */
typedef struct
{
  TcpIp_Dhcp_GetStatus_FctPtrType TcpIp_Dhcp_GetStatusFctPtr;
} TcpIp_Dhcp_FctDispatchType;
#endif /* (TCPIP_DHCP_DUALSTACK == STD_ON) */
#endif /* (TCPIP_DHCPGETSTATUS_API == STD_ON) */

/*==================[external function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/*-------------------[SW-unit interface function declarations]---------------*/

/*-------------------[SW-unit private function declarations]---------------*/

#if ((TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON))

/** \brief Write Domain Name
 *
 * This functions defines the domain name of the requested DHCP client.
 *
 * \param[in]  DataLength  length of the domain name
 * \param[in]  DataPtr     pointer to the domain name
 * \param[in]  domain_name pointer to the domain name
 * \param[in]  domain_name_len     pointer to the domain name length
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Dhcp_writeOptionFQDN
(
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  P2VAR (uint8, AUTOMATIC, TCPIP_APPL_DATA) domain_name,
  P2VAR (uint8, AUTOMATIC, TCPIP_APPL_DATA) domain_name_len
);

/** \brief Read Domain Name
 *
 * This functions reads the domain name of the requested DHCP client.
 *
 * \param[in,out] DataLength     in: buffer length, out: length of the domain name
 * \param[out]    DataPtr        pointer to the domain name
 * \param[in]  domain_name       pointer to the domain name
 * \param[in]  domain_name_len   domain name length
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Dhcp_readOptionFQDN
(
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) DataLength,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  P2VAR (uint8, AUTOMATIC, TCPIP_APPL_DATA) domain_name,
  uint16 domain_name_len
);

#endif /* ((TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON)) */

#if ((TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) || (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON))

/** \brief Read Dhcp option
 *
 * This functions reads the option of the requested DHCP client.
 *
 * \param[in]  optionId          option id
 * \param[in,out] DataLength     in: buffer length, out: length of the option
 * \param[out]    DataPtr        pointer to buffer where data should be copied to
 * \param[in]  optionDataPtr     pointer to the option data
 * \param[in]  optionLengthPtr   pointer to the option length
 * \param[in]  dhcpOptionCfgPtr  pointer to the dhcp option configuration
 * \param[in]  optionNum         number of configured options
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Dhcp_readOption
(
  uint16 optionId,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) DataLength,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  P2VAR (uint8, AUTOMATIC, TCPIP_APPL_DATA) optionDataPtr,
  P2VAR (uint16, AUTOMATIC, TCPIP_APPL_DATA) optionLengthPtr,
  P2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOptionCfgPtr,
  uint8 optionNum
);

/** \brief Write Dhcp option
 *
 * This functions writes the option of the requested DHCP client.
 *
 * \param[in]  optionId          option id
 * \param[in]  DataLength        length of the option
 * \param[in]  DataPtr           pointer to buffer where data should be copied from
 * \param[in]  optionDataPtr     pointer to the option data
 * \param[in]  optionLengthPtr   pointer to the option length
 * \param[in]  dhcpOptionCfgPtr  pointer to the dhcp option configuration
 * \param[in]  optionNum         number of configured options
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Dhcp_writeOption
(
  uint16 optionId,
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  P2VAR (uint8, AUTOMATIC, TCPIP_APPL_DATA) optionDataPtr,
  P2VAR (uint16, AUTOMATIC, TCPIP_APPL_DATA) optionLengthPtr,
  P2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOptionCfgPtr,
  uint8 optionNum
);

/** \brief Write Dhcp option
 *
 * This functions writes the received configurable option.
 *
 * \param[in]  optionId          option id
 * \param[in]  DataLength        length of the option
 * \param[in]  DataPtr           pointer to buffer where data should be copied from
 * \param[in]  optionDataPtr     pointer to the option data
 * \param[in]  optionLengthPtr   pointer to the option length
 * \param[in]  dhcpOptionCfgPtr  pointer to the dhcp option configuration
 * \param[in]  optionNum         number of configured options
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Dhcp_writeOptionRxIndication
(
  uint16 optionId,
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  P2VAR (uint8, AUTOMATIC, TCPIP_APPL_DATA) optionDataPtr,
  P2VAR (uint16, AUTOMATIC, TCPIP_APPL_DATA) optionLengthPtr,
  P2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOptionCfgPtr,
  uint8 optionNum
);

#endif /* ((TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) || (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#endif /* ifndef TCPIP_DHCP_PRIV_H */
/*==================[end of file]===========================================*/
