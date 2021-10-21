/**
 * \file
 *
 * \brief AUTOSAR EthIf
 *
 * This file contains the implementation of the AUTOSAR
 * module EthIf.
 *
 * \version 1.9.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef ETHIF_TYPES_H
#define ETHIF_TYPES_H

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 20.5 (advisory)
 * '#undef' shall not be used.
 *
 * Reason:
 * In order to modify the behavior of the included file, enclosing the file
 * inclusion by a special macro definition is required.
 *
 */


/*==================[inclusions]============================================*/

#include <TSAutosar.h>         /* EB AUTOSAR standard types */
#include <EthIf_Cfg.h>         /* this module's version declaration */
/* !LINKSTO EthIf.ASR43.SWS_EthIf_00152, 1 */
/* !LINKSTO EthIf.ASR43.SWS_EthIf_00153, 1 */
/* !LINKSTO EthIf.ASR43.SWS_EthIf_91010, 1 */
/* !LINKSTO EthIf.ASR43.SWS_EthIf_91101, 1 */
#include <Eth_GeneralTypes.h> /* Ethernet general public types */

/** \brief Enable/disable relocateable config */
#if (defined TS_RELOCATABLE_CFG_ENABLE)
#error TS_RELOCATABLE_CFG_ENABLE is already defined
#endif
#define TS_RELOCATABLE_CFG_ENABLE    ETHIF_RELOCATABLE_CFG_ENABLE

#if (defined TS_PB_CFG_PTR_CLASS)
#error TS_PB_CFG_PTR_CLASS is already defined
#endif
#define TS_PB_CFG_PTR_CLASS          ETHIF_CONST

#include <TSPBConfig_Types.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/** \brief Defined type for pointer to pointer type definition without violating Misra rule 20.7
 **/
typedef P2VAR(uint8, TYPEDEF, ETH_APPL_DATA) EthIf_Uint8TypePtr;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/* Deviation MISRAC2012-1) */
#undef TS_RELOCATABLE_CFG_ENABLE
/* Deviation MISRAC2012-1) */
#undef TS_PB_CFG_PTR_CLASS

#endif /* ifndef ETHIF_TYPES_H */
/*==================[end of file]===========================================*/
