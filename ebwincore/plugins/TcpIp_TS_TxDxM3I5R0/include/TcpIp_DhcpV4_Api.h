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

#ifndef TCPIP_DHCPV4_API_H
#define TCPIP_DHCPV4_API_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>       /* AUTOSAR comstack types */
#include <TcpIp_Api.h>            /* general TcpIp type definitions */


#include <TcpIp_DhcpV4_Api.guards>
/*==================[macros]================================================*/

/** \brief Option TCPIP_DHCP_OPTION_FQDN on API TcpIp_DhcpReadOption() and
 **        TcpIp_DhcpWriteOption().
 **
 ** This option requests to read or write the Domain Name of the DHCP client.
 ** Precondition: none
 ** ParameterValue: Points to underlying type of size uint8.
 */
#define TCPIP_DHCP_OPTION_FQDN     81U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Retrieves value of a DHCP option
 *
 * By this API service the TCP/IP stack retrieves DHCP option data identified by
 * parameter option for already received DHCP options.
 *
 * \param[in]   LocalIpAddrId  IP address identifier representing the local IP address
 *                             and EthIf controller for which the DHCP option shall be read.
 * \param[in]     Option       DHCP option
 * \param[in]     DataPtr      Pointer to memory containing DHCP option data
 * \param[in,out] DataLength   in:  contains the length of the provided data buffer.
 *                             out: length of the actual data.
 *
 * \return Result of operation
 * \retval E_OK      requested data retrieved successfully.
 * \retval E_NOT_OK  requested data could not be retrieved
 *
 * \ServiceID{0x0D}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpReadOption
(
  TcpIp_LocalAddrIdType LocalIpAddrId,
  uint8 Option,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataLength,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr
);

/** \brief Write value of a DHCP option
 *
 * By this API service the TCP/IP stack writes the DHCP option data identified by
 * parameter option.
 *
 * \param[in]  LocalIpAddrId  IP address identifier representing the local IP address
 *                            and EthIf controller for which the DHCP option shall be
 *                            written.
 * \param[in]  Option         DHCP option, e.g. Host Name
 * \param[in]  DataLength     length of DHCP option data
 * \param[in]  DataPtr        Pointer to memory containing DHCP option data
 *
 * \return Result of operation
 * \retval E_OK      no error occurred.
 * \retval E_NOT_OK  DHCP option data could not be written.
 *
 * \ServiceID{0x0E}
 * \Reentrancy{None Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpWriteOption
(
  TcpIp_LocalAddrIdType LocalIpAddrId,
  uint8 Option,
  uint8 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* DHCPV4_API_H_ */
