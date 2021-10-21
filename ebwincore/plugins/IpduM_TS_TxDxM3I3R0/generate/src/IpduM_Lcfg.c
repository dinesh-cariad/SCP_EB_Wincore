/**
 * \file
 *
 * \brief AUTOSAR IpduM
 *
 * This file contains the implementation of the AUTOSAR
 * module IpduM.
 *
 * \version 3.3.40
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <TSAutosar.h>           /* EB specific standard types */
#include <IpduM_Lcfg.h>          /* Generated configuration */
#include <IpduM_Lcfg_Static.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/* start data section declaration */
#define IPDUM_START_SEC_CONST_32
#include <IpduM_MemMap.h>

/**
 * Constant with size of post-build RAM in units of bytes
 */
/* !LINKSTO IPDUM.ASR40.IPDUM075,1 */
CONST(uint32, IPDUM_CONST) IpduM_GDataMemSize = IPDUM_DATA_MEM_SIZE;

/* value used to validate post build configuration against link time configuration */
CONST(uint32, IPDUM_CONST) IpduM_LcfgSignature = [!"asc:getConfigSignature(as:modconf('IpduM')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!]U;

/* stop data section declaration */
#define IPDUM_STOP_SEC_CONST_32
#include <IpduM_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/
#define IPDUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <IpduM_MemMap.h>

/**
 * Internal memory statically allocated upon link-time. - Size depends on
 * post build configuration => memory requirements of post build configuration
 * must be smaller than IPDUM_DATA_MEM_SIZE in order to avoid buffer
 * overflows.
 *
 * Note: This array itself is private. - Access to this array is done via
 *       IpduM_GDataMemPtr.
 */
STATIC TS_DefMaxAlignedByteArray(IpduM_DataMem, IPDUM, VAR_CLEARED, IPDUM_DATA_MEM_SIZE);

#define IPDUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <IpduM_MemMap.h>


/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/* start data section declaration */
#define IPDUM_START_SEC_CONST_UNSPECIFIED
#include <IpduM_MemMap.h>

/**
 * Exported pointer to post build memory which is statically allocated upon link-time
 *
 * Although the memory itself (i.e., the start address of the memory) is 32 bit aligned, a uint8
 * pointer is used for access to facilitate accesses as byte array with byte offsets as array indices
 */
/* !LINKSTO IPDUM.ASR40.IPDUM075,1 */
CONSTP2VAR(uint8, AUTOMATIC, IPDUM_VAR_CLEARED) IpduM_GDataMemPtr = (P2VAR(uint8, IPDUM_CONST, IPDUM_VAR_CLEARED)) IpduM_DataMem;

/* stop data section declaration */
#define IPDUM_STOP_SEC_CONST_UNSPECIFIED
#include <IpduM_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
