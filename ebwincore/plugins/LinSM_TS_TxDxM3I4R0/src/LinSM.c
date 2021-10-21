/**
 * \file
 *
 * \brief AUTOSAR LinSM
 *
 * This file contains the implementation of the AUTOSAR
 * module LinSM.
 *
 * \version 3.4.19
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <LinSM_Internal.h>

/*==================[macros]================================================*/

/* -----------------[version checks]----------------------------------------*/
/* !LINKSTO LinSM.ASR40.LINSM208,1 */
/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef LINSM_VENDOR_ID /* configuration check */
#error LINSM_VENDOR_ID must be defined
#endif

#if (LINSM_VENDOR_ID != 1U) /* vendor check */
#error LINSM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef LINSM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error LINSM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (LINSM_AR_RELEASE_MAJOR_VERSION != 4U)
#error LINSM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef LINSM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error LINSM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (LINSM_AR_RELEASE_MINOR_VERSION != 0U )
#error LINSM_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef LINSM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error LINSM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (LINSM_AR_RELEASE_REVISION_VERSION != 3U )
#error LINSM_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef LINSM_SW_MAJOR_VERSION /* configuration check */
#error LINSM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (LINSM_SW_MAJOR_VERSION != 3U)
#error LINSM_SW_MAJOR_VERSION wrong (!= 3U)
#endif

#ifndef LINSM_SW_MINOR_VERSION /* configuration check */
#error LINSM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (LINSM_SW_MINOR_VERSION < 4U)
#error LINSM_SW_MINOR_VERSION wrong (< 4U)
#endif

#ifndef LINSM_SW_PATCH_VERSION /* configuration check */
#error LINSM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (LINSM_SW_PATCH_VERSION < 19U)
#error LINSM_SW_PATCH_VERSION wrong (< 19U)
#endif

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#define LINSM_START_SEC_VAR_INIT_8
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

VAR(LinSM_GlobalStateType, LINSM_VAR) LinSM_GlobalState = LINSM_GLOBAL_UNINIT;

#define LINSM_STOP_SEC_VAR_INIT_8
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

