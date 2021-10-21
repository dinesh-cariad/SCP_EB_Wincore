/**
 * \file
 *
 * \brief AUTOSAR E2EXf
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EXf.
 *
 * \version 1.0.36
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/*==================[inclusions]============================================*/
#include <Std_Types.h>      /* AUTOSAR standard types */
#include <E2EXf.h>          /* Module public API */

/*==================[version check]=========================================*/
/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef E2EXF_VENDOR_ID /* configuration check */
#error E2EXF_VENDOR_ID must be defined
#endif

#if (E2EXF_VENDOR_ID != 1U) /* vendor check */
#error E2EXF_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef E2EXF_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error E2EXF_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (E2EXF_AR_RELEASE_MAJOR_VERSION != 4U)
#error E2EXF_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef E2EXF_AR_RELEASE_MINOR_VERSION /* configuration check */
#error E2EXF_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (E2EXF_AR_RELEASE_MINOR_VERSION != 2U )
#error E2EXF_AR_RELEASE_MINOR_VERSION wrong (!= 2U)
#endif

#ifndef E2EXF_AR_RELEASE_REVISION_VERSION /* configuration check */
#error E2EXF_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (E2EXF_AR_RELEASE_REVISION_VERSION != 1U )
#error E2EXF_AR_RELEASE_REVISION_VERSION wrong (!= 1U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef E2EXF_SW_MAJOR_VERSION /* configuration check */
#error E2EXF_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (E2EXF_SW_MAJOR_VERSION != 1U)
#error E2EXF_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef E2EXF_SW_MINOR_VERSION /* configuration check */
#error E2EXF_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (E2EXF_SW_MINOR_VERSION < 0U)
#error E2EXF_SW_MINOR_VERSION wrong (< 0U)
#endif

#ifndef E2EXF_SW_PATCH_VERSION /* configuration check */
#error E2EXF_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (E2EXF_SW_PATCH_VERSION < 36U)
#error E2EXF_SW_PATCH_VERSION wrong (< 36U)
#endif

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define E2EXF_START_SEC_CODE
#include <E2EXf_MemMap.h>

/*--------------------- E2EXf_GetVersionInfo ----------------------------*/
#if (E2EXF_VERSION_INFO_API == STD_ON)
FUNC(void, E2EXF_CODE) E2EXf_GetVersionInfo
(
    P2VAR(Std_VersionInfoType, AUTOMATIC, E2EXF_APPL_DATA) VersionInfo
)
{
  if(VersionInfo != NULL_PTR)
  {
    /* assign logistic values to structure members */
    VersionInfo->vendorID = (uint16) E2EXF_VENDOR_ID;
    VersionInfo->moduleID = (uint16) E2EXF_MODULE_ID;
    VersionInfo->sw_major_version = (uint8) E2EXF_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = (uint8) E2EXF_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = (uint8) E2EXF_SW_PATCH_VERSION;
  }
}
#endif /* E2EXF_VERSION_INFO_API == STD_ON */

/*==================[internal function definitions]=========================*/

#define E2EXF_STOP_SEC_CODE
#include <E2EXf_MemMap.h>

/*==================[end of file]===========================================*/
