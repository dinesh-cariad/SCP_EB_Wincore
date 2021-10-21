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
 * MISRAC2012-1) Deviated Rule: 8.5 (required)
 * An external object or function shall be declared once in one and only one file.
 *
 * Reason:
 * In AUTOSAR main functions might be declared by RTE generated code or the module
 * header file.
 *
 */

/*
 * mgcc62 Deviation List
 *
 * MGCC62-1) Deviated Rule: redundant-decls
 * warning: redundant redeclaration [-Wredundant-decls]
 *
 * Reason:
 * In AUTOSAR main functions might be declared by RTE generated code or the module
 * header file.
 *
 */

#ifndef TCPIP_API_H
#define TCPIP_API_H

/*==================[inclusions]============================================*/
#ifndef TCPIP_NO_CFGCLASSMIX_REQUIRED
#define TCPIP_NO_CFGCLASSMIX_REQUIRED
#endif

#include <ComStack_Types.h>       /* AUTOSAR comstack types */
#include <TcpIp_Cfg.h>            /* TcpIp pre-compile-time configuration */
#include <TcpIp_CfgTypes.h>       /* TcpIp pre-compile-time dependent configuration types */
#include <TcpIp_Version.h>        /* this module's version declaration */
#include <Eth_GeneralTypes.h>     /* Ethernet general public types */


#include <TcpIp_Api.guards>
/*==================[macros]================================================*/

/*------------------[DET error IDs]-----------------------------------------*/

/** \brief Det error Id TCPIP_E_NOTINIT */
#define TCPIP_E_NOTINIT                  0x01U

/** \brief Det error Id TCPIP_E_PARAM_POINTER */
#define TCPIP_E_PARAM_POINTER            0x02U

/** \brief Det error Id TCPIP_E_INV_ARG */
#define TCPIP_E_INV_ARG                  0x03U

/** \brief Det error Id TCPIP_E_NOBUFS */
#define TCPIP_E_NOBUFS                   0x04U

/** \brief Det error Id TCPIP_E_INV_SOCKADDR */
#define TCPIP_E_INV_SOCKADDR             0x05U

/** \brief Det error Id TCPIP_E_DESTADDRREQ */
#define TCPIP_E_DESTADDRREQ              0x06U

/** \brief Det error Id TCPIP_E_MSGSIZE */
#define TCPIP_E_MSGSIZE                  0x07U

/** \brief Det error Id TCPIP_E_PROTOTYPE */
#define TCPIP_E_PROTOTYPE                0x08U

/** \brief Det error Id TCPIP_E_ADDRINUSE */
#define TCPIP_E_ADDRINUSE                0x09U

/** \brief Det error Id TCPIP_E_ADDRNOTAVAIL */
#define TCPIP_E_ADDRNOTAVAIL             0x0aU

/** \brief Det error Id TCPIP_E_ISCONN */
#define TCPIP_E_ISCONN                   0x0bU

/** \brief Det error Id TCPIP_E_NOTCONN */
#define TCPIP_E_NOTCONN                  0x0cU

/** \brief Det error Id TCPIP_E_NOPROTOOPT */
#define TCPIP_E_NOPROTOOPT               0x0dU

/** \brief Det error Id TCPIP_E_AFNOSUPPORT */
#define TCPIP_E_AFNOSUPPORT              0x0eU


/*==================[type definitions]=========================*/

/** \brief operation completed successfully */
#define TCPIP_OK                0U
/** \brief operation failed */
#define TCPIP_E_NOT_OK          1U
/** \brief operation failed because of an ARP cache miss */
#define TCPIP_E_PHYS_ADDR_MISS  2U
/** \brief operation failed, ignore the packet */
#define TCPIP_E_DROP            3U
/** \brief operation failed, illegal request */
#define TCPIP_E_NOT_PERMITTED   4U
/** \brief operation failed, out of memory */
#define TCPIP_E_MEMORY          5U
/** \brief operation failed, no local address assigned */
#define TCPIP_E_ADDRNOTASSIGNED 6U
/** \brief operation failed temporarily, dependent operation not yet concluded */
#define TCPIP_E_PENDING         7U

/** \brief TcpIp specific return type. */
typedef uint8 TcpIp_ReturnType;


/** \brief Represents the invalid value for function arguments of type TcpIp_SocketIdType */
#define TCPIP_SOCKETID_INVALID 0xFFFFU


/** \brief Handle identifier
 *
 * Type definition of the handle identifier used in the CopyData functor interface.
 */
typedef uint16 TcpIp_Ip_HandleIdType;

/** \brief Socket identifier
 *
 * Type definition of socket identifier
 */
typedef TcpIp_Ip_HandleIdType TcpIp_SocketIdType;


/** \brief Value identifies IP version 4 for TcpIp_DomainType */
#define TCPIP_AF_INET   0x02U

/** \brief Value identifies IP version 6 for TcpIp_DomainType */
#define TCPIP_AF_INET6  0x1cU

/** \brief Value identifies unspecified address family for TcpIp_DomainType */
#define TCPIP_AF_UNSPEC 0x00U

/** \brief Generic address structure
 *
 * This structure defines a generic address type to pass on the API.
 * This generic type abstract IPv4 and IPv6 addresses and shall be use to
 * typecast TcpIp_SockAddrInetType and TcpIp_SockAddrInet6Type.
 * The value of domain defines the underlying structure.
 * Note: The member data secures the correct alignment of this structure and
 * shall not be used in any way.
 */
typedef struct
{
  /** \brief This is the code for the address format of this address */
  TcpIp_DomainType domain;
  /** \brief Secures alignment and shall not be used. */
  uint32 data[1];
} TcpIp_SockAddrType;

/** \brief Pointer to structure of type TcpIp_SockAddrType */
typedef P2VAR(TcpIp_SockAddrType,TYPEDEF,TCPIP_APPL_DATA) TcpIp_SockAddrPtrType;

/*==================[external function declarations]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief Checks compatibility of the post-build-time configuration
 *
 * This service checks the compatibility of the post-build-time configuration
 * against the source code.
 *
 * \param[in] voidConfigPtr Pointer to the configuration data of the TcpIp module.
 *
 * \return Result of compatibility check
 * \retval E_OK      Provided configuration is compatible.
 * \retval E_NOT_OK  Provided configuration is notcompatible.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IsValidConfig
(
  P2CONST(void,AUTOMATIC,TCPIP_APPL_CONST) voidConfigPtr
);

/** \brief Initializes the TcpIp stack
 *
 * This service initializes the TCP/IP Stack.
 * The call of this service is mandatory before using the TcpIp instance for further processing.
 *
 * \param[in] ConfigPtr Address of the post-build time configuration data of the TcpIp module.
 *
 * \ServiceID{0x01}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, TCPIP_CODE) TcpIp_Init(P2CONST(TcpIp_ConfigType, AUTOMATIC, TCPIP_APPL_CONST) ConfigPtr);

/** \brief TcpIp main function
 *
 * This function performs the periodic actions of the TcpIp module (e.g. timer handling, state machine triggering).
 *
 * \ServiceID{0x15}
 * \Reentrancy{Non-Reentrant}
 * \Synchronicity{Synchronous}
 */
/* Deviation MISRAC2012-1, MGCC62-1 */
extern FUNC(void, TCPIP_CODE) TcpIp_MainFunction(void);

#if (TCPIP_MAINFUNCTIONTX_ENABLED == STD_ON)
/** \brief MainFunction for socket transmission
 *
 *  This function performs transmission tasks. e.g. transmission of TCP segments.
 */
extern FUNC(void, TCPIP_CODE) TcpIp_MainFunctionTx
(
  void
);
#endif /* (TCPIP_MAINFUNCTIONTX_ENABLED == STD_ON) */

#if (TCPIP_VERSION_INFO_API == STD_ON)
/** \brief Get version information of the TcpIp module.
 *
 * This service returns the version information of this module. The version
 * information includes:
 *  - Module Id
 *  - Vendor Id
 *  - Vendor specific version numbers
 *
 * \param[out] versioninfo Pointer to where to store the version
 * information of this module.
 *
 * \ServiceID{0x02}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, TCPIP_CODE) TcpIp_GetVersionInfo(P2VAR(Std_VersionInfoType,AUTOMATIC, TCPIP_APPL_DATA) versioninfo);
#endif /* TCPIP_VERSION_INFO_API == STD_ON */

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
/**
 * \brief   Reads and resets measurement data for diagnostic purposes.
 *
 * This service allows to read and reset detailed measurement data for diagnostic purposes.
 * Get all MeasurementIdx's at once is not supported. TCPIP_MEAS_ALL shall only be used to reset
 * all MeasurementIdx's at once. A NULL_PTR shall be provided for MeasurementDataPtr in this case.
 *
 * \param[in] MeasurementIdx         Index to select specific measurement data:
 *                                   TCPIP_MEAS_DROP_TCP (0x01) - Measurement index of dropped
 *                                   PDUs caused by invalid destination TCP-Port.
 *                                   TCPIP_MEAS_DROP_UDP (0x02) - Measurement index of dropped
 *                                   PDUs caused by invalid destination UDP-Port.
 *                                   TCPIP_MEAS_DROP_IPV4 (0x03) - Measurement index of dropped
 *                                   datagrams caused by invalid IPv4 address
 *                                   TCPIP_MEAS_DROP_IPV6 (0x04) - Measurement index of dropped
 *                                   datagrams caused by invalid IPv6 address
 *                                   TCPIP_MEAS_RESERVED_1 (0x05-0x7F) - Reserved by AUTOSAR.
 *                                   TCPIP_MEAS_RESERVED_2 (0x80-0xEF) - Vendor specific range.
 *                                   TCPIP_MEAS_RESERVED_3 (0xF0-0xFE) - Reserved by AUTOSAR
 *                                                                       (future use).
 *                                   TCPIP_MEAS_ALL (0xFF) - Represents all measurement indexes.
 * \param[in] MeasurementResetNeeded Flag to trigger a reset of the measurement data.
 * \param[out] MeasurementDataPtr    Pointer to data buffer, where to copy measurement data.
 *
 * \returns Std_ReturnType
 * \retval  E_OK     The function has been successfully executed.
 * \retval  E_NOT_OK The function could not be successfully executed.
 * \ServiceID{0x45}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetAndResetMeasurementData
(
  TcpIp_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
);
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#if  (TCPIP_DHCPGETSTATUS_API == STD_ON)

/** \brief Read DHCP status
 *
 *  This functions returns the status of the Dhcp address assignment
 *
 * \param[in]  LocalAddressId    Address Id of the DHCP address assignment
 * \param[in]  StatusPtr         Pointer to a status variable. Contains the state information.
 *
 * StatusPtr value       DhcpV4         DhcpV6
 *      0                INACTIVE       INACTIVE        port closed
 *      1                START_DELAY    START_DELAY     waiting for limited broadcast address(v4)/Link local address (v6)
 *      2                INIT           INIT            Startup state
 *      3                SELECTING      SOLICIT         Searching for server  (DISCOVER/SOLICITATIONS are sent)
 *      4                REQUESTING     REQUESTING      Requesting specific lease  (REQUESTS are sent)
 *      5                BOUND          BOUND           Lease has been assigned, is in use
 *      6                RENEWING       RENEWING        Renewing a lease
 *      7                REBINDING      REBINDING       Requesting lease extension from any server
 *      8                   /           REASSIGNING     (DhcpV6 only) Requested specific lease in response to NoBinding status code
 *      9
 *
 * \returns Std_ReturnType
 * \retval  E_OK                 Status was successfully retrieved
 * \retval  E_NOT_OK             No Dhcp address assignment was found
 * \ServiceID{0xF1}

 */
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpGetStatus
(
  TcpIp_LocalAddrIdType LocalAddressId,
  P2VAR(uint8,AUTOMATIC, TCPIP_APPL_DATA) StatusPtr
);

#endif /* (TCPIP_DHCPGETSTATUS_API == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>


#endif /* ifndef TCPIP_API_H */
/*==================[end of file]===========================================*/
