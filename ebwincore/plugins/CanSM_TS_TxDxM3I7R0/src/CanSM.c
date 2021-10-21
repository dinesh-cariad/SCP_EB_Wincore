/**
 * \file
 *
 * \brief AUTOSAR CanSM
 *
 * This file contains the implementation of the AUTOSAR
 * module CanSM.
 *
 * \version 3.7.5
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>    /* AUTOSAR communication stack types */

#include <CanSM_Int_Stc.h>     /* Module internal static header */
#include <CanSM_Version.h>     /* Module version declarations */

/*==================[version check]=========================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef CANSM_VENDOR_ID /* configuration check */
#error CANSM_VENDOR_ID must be defined
#endif

#if (CANSM_VENDOR_ID != 1U) /* vendor check */
#error CANSM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef CANSM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error CANSM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (CANSM_AR_RELEASE_MAJOR_VERSION != 4U)
#error CANSM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef CANSM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error CANSM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (CANSM_AR_RELEASE_MINOR_VERSION != 0U )
#error CANSM_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef CANSM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error CANSM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (CANSM_AR_RELEASE_REVISION_VERSION != 3U )
#error CANSM_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef CANSM_SW_MAJOR_VERSION /* configuration check */
#error CANSM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (CANSM_SW_MAJOR_VERSION != 3U)
#error CANSM_SW_MAJOR_VERSION wrong (!= 3U)
#endif

#ifndef CANSM_SW_MINOR_VERSION /* configuration check */
#error CANSM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (CANSM_SW_MINOR_VERSION < 7U)
#error CANSM_SW_MINOR_VERSION wrong (< 7U)
#endif

#ifndef CANSM_SW_PATCH_VERSION /* configuration check */
#error CANSM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (CANSM_SW_PATCH_VERSION < 5U)
#error CANSM_SW_PATCH_VERSION wrong (< 5U)
#endif

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#define CANSM_START_SEC_VAR_INIT_UNSPECIFIED
#include <CanSM_MemMap.h>

VAR(ComM_ModeType, CANSM_VAR) CanSM_CommNoCommunication = COMM_NO_COMMUNICATION;
VAR(ComM_ModeType, CANSM_VAR) CanSM_CommSilentCommunication = COMM_SILENT_COMMUNICATION;
VAR(ComM_ModeType, CANSM_VAR) CanSM_CommFullCommunication = COMM_FULL_COMMUNICATION;

#define CANSM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <CanSM_MemMap.h>


#define CANSM_START_SEC_VAR_INIT_8
#include <CanSM_MemMap.h>

VAR(uint8, CANSM_VAR) CanSM_GlobalInit = CANSM_GLOBAL_UNINIT;

#define CANSM_STOP_SEC_VAR_INIT_8
#include <CanSM_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

