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
#ifndef TCPIP_NDP_API_H
#define TCPIP_NDP_API_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>       /* AUTOSAR comstack types */
#include <TcpIp_Api.h>            /* general TcpIp type definitions */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/** \brief Specifies an entry in the NDP cache */
typedef struct
{
  /** \brief IPv6 address in network byte order */
  uint32 Inet6Addr[4];
  /** \brief physical address in network byte order */
  uint8 PhysAddr[6];
  /** \brief state of the address entry  */
  uint8 State;
} TcpIp_NdpCacheEntryType ;

/*==================[external function declarations]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Retrieve all valid physical addresses from NDP cache
 *
 * This service copies entries from the physical address cache of the IPv6 instance that is active
 * on the EthIf controller specified by ctrlIdx into a user provided buffer. The function will copy
 * all or numberOfElements into the output list. If input value of numberOfElements is 0 the
 * function will not copy any data but only return the number of valid entries in the cache.
 * EntryListPtr may be NULL_PTR in this case.
 *
 * \param[in]      ctrlIdx           EthIf controller index to identify the related NDP table.
 * \param[in,out]  numberOfElements  In: Maximum number of entries that can be stored in
 *                                       output entryListPtr.
 *                                   Out: Number of entries written to output entryListPtr (Number
 *                                        of all entries in the cache if input value is 0).
 * \param[out]     entryListPtr      Pointer to memory where the list of cache entries shall
 *                                   be stored.
 *
 * \return Result of operation
 * \retval E_OK      physical address cache could be read.
 * \retval E_NOT_OK  physical address cache could not be read (i.e. no IPv6 instance active on
 *                   this controller)
 *
 * \ServiceID{0x1d}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetNdpCacheEntries
(
  uint8 ctrlIdx,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) numberOfElements,
  P2VAR(TcpIp_NdpCacheEntryType, AUTOMATIC, TCPIP_APPL_DATA) entryListPtr
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* ifndef TCPIP_NDP_API_H */
/*==================[end of file]===========================================*/
