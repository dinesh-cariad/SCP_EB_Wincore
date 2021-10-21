/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/
#include <UdpNm_Adaptive.h>
#if (UDPNM_ADAPTIVE == STD_OFF)
#include <TSAutosar.h> /* EB specific standard types */
#include <UdpNm_Lcfg.h> /* Generated configuration */
#include <UdpNm_Lcfg_Static.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/* start data section declaration */
#define UDPNM_START_SEC_CONST_32
#include <UdpNm_MemMap.h>

/* value used to validate post build configuration against link time configuration */
/* !LINKSTO UdpNm.EB.PBCFGM105,1 */
CONST(uint32, UDPNM_CONST) UdpNm_LcfgSignature = [!"asc:getConfigSignature(as:modconf('UdpNm')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!]U;

/* stop data section declaration */
#define UDPNM_STOP_SEC_CONST_32
#include <UdpNm_MemMap.h>


/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/* start data section declaration */
#define UDPNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>

/** \brief Internal memory statically allocated upon link-time. Size depends on
 *  post build configuration. Memory requirements of post build configuration
 *  must be smaller than COM_RAM_SIZE_MAX in order to avoid buffer
 *  overflows. Memory is aligned to the most stringent alignment requirement
 *  of any simple data type available on the respective platform, e.g. uint32.
 **/
STATIC TS_DefMaxAlignedByteArray(UDPNM_RX_TX_BUFFER_Local, UDPNM, VAR_CLEARED, UDPNM_DATA_MEM_SIZE);

/* stop data section declaration */
#define UDPNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>

/*==================[external data]=========================================*/
/* start data section declaration */
#define UDPNM_START_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>

/** \brief Pointer to statically allocated RAM.
 **/
CONSTP2VAR(uint8, UDPNM_CONST, COM_VAR_CLEARED) UdpNm_Rx_Tx_Buffer =
  (P2VAR(uint8, UDPNM_CONST, COM_VAR_CLEARED)) UDPNM_RX_TX_BUFFER_Local;

/* stop data section declaration */
#define UDPNM_STOP_SEC_CONST_UNSPECIFIED
#include <UdpNm_MemMap.h>
#endif /* (UDPNM_ADAPTIVE == STD_OFF) */
/*==================[external constants]====================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
