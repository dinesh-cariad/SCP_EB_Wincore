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

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 * The memory API defines the memory identifier handle as type of void* in order to
 * be generic from the possible implementations. The current implementation internally
 * decided to use the handle as index into an array, thus an integral datatype is needed.
 * Thus the handle pointer is converted to a handle integer.
 *
 */

#ifndef TCPIP_MEMORY_INT_H
#define TCPIP_MEMORY_INT_H

/*==================[inclusions]============================================*/
#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>          /* module internal headerfile */
#include <TcpIp_Memory_Api.h>   /* public API implemented by SW-unit Memory */

/*==================[macros]================================================*/

#if defined(TCPIP_MEMORY_NUM_TOTAL_BLOCKS) || (TCPIP_NVM_STORING_ENABLED == STD_ON)

/* Deviation MISRAC2012-1 */
#define TCPIP_MEMORY_IDPTR_INVALID ((P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA))((uint16_least)0xFFFFU))

#if (TCPIP_NVM_STORING_ENABLED == STD_ON)

/** \brief Number of IpV4 addresses to be stored in NvM */
#define TCPIP_NVM_BLOCK_NUM_OF_ENTRIES  TcpIp_PBcfgPtr->ipV4NvMAddrNum

/** \brief Returns the NvM memory index (0-based and dense) for the given local address Id and assignment method pair */
#define TcpIp_Memory_NvM_getCacheIdx(localAddrId,method) \
  ((TCPIP_CFG(localAddr,localAddrId)).assignmByMethod[(method)].intNvMMemoryIndex)

/* States of the Ip_NvM state machine */
/** \brief IDLE state of Ip_NvM state machine */
#define TCPIP_MEMORY_NVM_STATE_IDLE                            0U
/** \brief State of Ip_NvM state machine when Ip address can be set and get */
#define TCPIP_MEMORY_NVM_STATE_IP_AVAILABLE                    1U
/** \brief State of Ip_NvM state machine when Ip address can be only set */
#define TCPIP_MEMORY_NVM_STATE_IP_UNAVAILABLE                  2U

#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Initialize memory data structures
 *
 * By this API service the Memory SW-Unit gets initialized.
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Memory_init
(
  void
);

#ifdef TCPIP_MEMORY_NUM_TOTAL_BLOCKS
/** \brief Allocate dynamic memory
 *
 * This function dynamically allocates a chunk of memory and returns an identifier
 * which must be used for later access to this memory.
 * Note that the memory actually returned might be larger or smaller than the
 * requested memory size.
 *
 * param[in,out]      sizePtr   in: Size of requested memory in units of bytes.
 *                              out: Actual size of memory allocated in units of bytes.
 *
 * \return Identifier for allocated memory.
 * \retval NULL_PTR No memory has been allocated (e.g. out of memory).
 */
TS_MOD_PRIV_DECL FUNC_P2VAR(void, TCPIP_APPL_DATA, TCPIP_CODE) TcpIp_Memory_new
(
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) sizePtr
);

/** \brief Get pointer to linear memory chunk
 *
 * This function returns a pointer to a piece of linear allocated memory and the size of the linear chunk
 * identified by idPtr at location index.
 *
 * param[in]  idPtr    Pointer to dynamic memory identifier.
 * param[in]  index    Index of dynamic memory to return address of.
 * param[out]  size    Size of linear memory chunk available at index.
 *
 * \return Address of memory chunk.
 */
TS_MOD_PRIV_DECL FUNC_P2VAR(uint8, TCPIP_APPL_DATA, TCPIP_CODE) TcpIp_Memory_getLinearMemoryAddress
(
  P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA) idPtr,
  uint32 index,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) size
);

#if ((TCPIP_TCP_ENABLED == STD_ON))

/** \brief Get data from dynamic memory
 *
 * This function copies size bytes of data from dynamic memory identified by idPtr
 * at location index to destPtr.
 *
 * param[in]  idPtr    Pointer to dynamic memory identifier.
 * param[in]  destPtr  Pointer to to put the data to.
 * param[in]  index    Index into dynamic memory to read the data from.
 * param[in]  size     Size of data to get from dynamic memory in units of bytes.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Memory_get
(
  P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA) idPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) destPtr,
  uint32 index,
  uint32 size
);

#endif /* ((TCPIP_TCP_ENABLED == STD_ON)) */

#if (TCPIP_TCP_ENABLED == STD_ON)

/** \brief Increase dynamic memory size
 *
 * This function increases the size of a previously allocated memory.
 *
 * param[in]  idPtr   Pointer to dynamic memory identifier to increase.
 * param[in]  size    Number of bytes that shall be allocated
 *
 * \return Actual size of memory allocated in units of bytes.
 */
TS_MOD_PRIV_DECL FUNC(uint32, TCPIP_CODE) TcpIp_Memory_increase
(
  P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA) idPtr,
  uint32 size
);

#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

/** \brief Deallocate dynamic memory
 *
 * This function deallocates previously allocated memory thus enabling it's re-use for other
 * use-cases.
 *
 * param[in]  idPtr   Pointer to dynamic memory identifier to deallocate.
 * param[in]  size    Number of bytes that shall be freed at the beginning of the memory
 *
 * \return Remaining number of bytes that holds the memory
 * \retval 0   The memory is empty and shall not be used anymore, a new memory must be allocated
 * \retval >0  Number of remaining bytes
 */
TS_MOD_PRIV_DECL FUNC(uint32, TCPIP_CODE) TcpIp_Memory_decrease
(
  P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA) idPtr,
  uint32 size
);

#endif /* TCPIP_MEMORY_NUM_TOTAL_BLOCKS */

#if (TCPIP_NVM_STORING_ENABLED == STD_ON)

/** \brief Start the Memory_NvM state machine
 *
 * This function starts the Memory_NvM state machine so that the IP addresses
 * could be read and stored in Memory NvM local cache (depending on NvM).
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Memory_NvM_start(void);

/** \brief Stop the Memory_NvM state machine
 *
 * This function stops the Memory_NvM state machine so that the IP addresses
 * can't be read and stored in Memory NvM local cache and it is safe
 * for NvM to write the addresses.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Memory_NvM_stop(void);

/** \brief Set the IP address in Memory NvM local cache
 *
 * This function sets the address in Memory NvM local cache
 *
 * \param[in] localAddrId         Id of local IP address
 * \param[in] ipAddr              Ip address
 * \param[in] method              use TCPIP_IPADDRM_ASSIGN_LINKLOCALV4 or
 *                                TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP parameter set
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_Memory_NvM_setIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  uint32 ipAddr,
  uint8 method
);

/** \brief Get the IP address from Memory NvM local cache
 *
 * This function gets the address from Memory NvM local cache
 *
 * \param[in] localAddrId         Id of local IP address
 * \param[out] ipAddrPtr          Pointer for returning Ip address
 * \param[in] method              use TCPIP_IPADDRM_ASSIGN_LINKLOCALV4 or
 *                                TCPIP_IPADDRM_ASSIGN_LINKLOCAL_DOIP parameter set
 *
 * \return Result of the operation
 * \retval TRUE        Ip addresses was read successfully
 * \retval FALSE       Ip addresses either wasn't read successfully or wasn't available
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_Memory_NvM_getIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr,
  uint8 method
);
#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal function definitions]=========================*/
#endif /* defined(TCPIP_MEMORY_NUM_TOTAL_BLOCKS) || (TCPIP_NVM_STORING_ENABLED == STD_ON) */


#endif /* TCPIP_MEMORY_INT_H */
