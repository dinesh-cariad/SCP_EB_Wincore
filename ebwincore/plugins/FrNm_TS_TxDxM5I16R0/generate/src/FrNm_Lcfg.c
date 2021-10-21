/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to
 * void into pointer to object.
 *
 * Reason:
 * The memory routines are optimized for dealing with aligned
 * memory sections.
 *
 */
/*==================[inclusions]============================================*/

#include <TSAutosar.h> /* EB specific standard types */
#include <FrNm_Lcfg.h> /* Generated configuration */
#include <FrNm_Lcfg_Static.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/* start data section declaration */
#define FRNM_START_SEC_CONST_32
#include <FrNm_MemMap.h>

/* value used to validate post build configuration against link time configuration */
CONST(uint32, FRNM_CONST) FrNm_LcfgSignature = [!"asc:getConfigSignature(as:modconf('FrNm')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!]U;

/* stop data section declaration */
#define FRNM_STOP_SEC_CONST_32
#include <FrNm_MemMap.h>

/* start data section declaration */
#define FRNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrNm_MemMap.h>

/** \brief Internal memory statically allocated upon link-time. Size depends on
 ** post build configuration. Memory requirements of post build configuration
 ** must be smaller than COM_RAM_SIZE_MAX in order to avoid buffer
 ** overflows. Memory is aligned to the most stringent alignment requirement
 ** of any simple data type available on the respective platform, e.g. uint32.
 */
STATIC TS_DefMaxAlignedByteArray(FRNM_RX_TX_BUFFER_Local, FRNM, VAR_CLEARED, FRNM_DATA_MEM_SIZE);

/* stop data section declaration */
#define FRNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrNm_MemMap.h>

/*==================[external data]=========================================*/
/* start data section declaration */
#define FRNM_START_SEC_CONST_UNSPECIFIED
#include <FrNm_MemMap.h>

/** \brief Pointer to statically allocated RAM.
 */
/* Deviation MISRAC2012-1 <+3> */
CONSTP2VAR(uint8, FRNM_CONST, COM_VAR_CLEARED) FrNm_Rx_Tx_Buffer =
  (P2VAR(uint8, FRNM_CONST, COM_VAR_CLEARED))
  ((P2VAR(void, FRNM_CONST, COM_VAR_CLEARED)) FRNM_RX_TX_BUFFER_Local);

/* stop data section declaration */
#define FRNM_STOP_SEC_CONST_UNSPECIFIED
#include <FrNm_MemMap.h>


/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
