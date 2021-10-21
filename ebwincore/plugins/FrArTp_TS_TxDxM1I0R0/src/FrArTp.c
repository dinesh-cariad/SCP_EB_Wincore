/**
 * \file
 *
 * \brief AUTOSAR FrArTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrArTp.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/*==================[inclusions]============================================*/

#include <FrArTp_Int.h>   /* module internal headerfile */
#include <FrArTp_SM_Int.h>    /* SW-unit SM interface */
#include <FrArTp_Encode_Int.h>    /* SW-unit SM interface */
#include <FrArTp_CfgAccess_Int.h>    /* SW-unit SM interface */


/*==================[version check]=========================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef FRARTP_VENDOR_ID /* configuration check */
#error FRARTP_VENDOR_ID must be defined
#endif

#if (FRARTP_VENDOR_ID != 1U) /* vendor check */
#error FRARTP_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef FRARTP_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error FRARTP_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (FRARTP_AR_RELEASE_MAJOR_VERSION != 4U)
#error FRARTP_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef FRARTP_AR_RELEASE_MINOR_VERSION /* configuration check */
#error FRARTP_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (FRARTP_AR_RELEASE_MINOR_VERSION != 2U )
#error FRARTP_AR_RELEASE_MINOR_VERSION wrong (!= 2U)
#endif

#ifndef FRARTP_AR_RELEASE_REVISION_VERSION /* configuration check */
#error FRARTP_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (FRARTP_AR_RELEASE_REVISION_VERSION != 2U )
#error FRARTP_AR_RELEASE_REVISION_VERSION wrong (!= 2U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef FRARTP_SW_MAJOR_VERSION /* configuration check */
#error FRARTP_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (FRARTP_SW_MAJOR_VERSION != 1U)
#error FRARTP_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef FRARTP_SW_MINOR_VERSION /* configuration check */
#error FRARTP_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (FRARTP_SW_MINOR_VERSION < 0U)
#error FRARTP_SW_MINOR_VERSION wrong (< 0U)
#endif

#ifndef FRARTP_SW_PATCH_VERSION /* configuration check */
#error FRARTP_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (FRARTP_SW_PATCH_VERSION < 8U)
#error FRARTP_SW_PATCH_VERSION wrong (< 8U)
#endif

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

#if (FRARTP_VERSION_INFO_API == STD_ON)

#define FRARTP_START_SEC_CONST_UNSPECIFIED
#include <FrArTp_MemMap.h>

/** \brief Describes the used version of the module */
STATIC CONST(Std_VersionInfoType, FRARTP_CONST) FrArTp_VersionInfo =
{
   FRARTP_VENDOR_ID,
   FRARTP_MODULE_ID,
   FRARTP_SW_MAJOR_VERSION,
   FRARTP_SW_MINOR_VERSION,
   FRARTP_SW_PATCH_VERSION
};

#define FRARTP_STOP_SEC_CONST_UNSPECIFIED
#include <FrArTp_MemMap.h>

#endif /* FRARTP_VERSION_INFO_API == STD_ON */

/*==================[external data]=========================================*/

#define FRARTP_START_SEC_VAR_INIT_8
#include <FrArTp_MemMap.h>

VAR(boolean, FRARTP_VAR) FrArTp_InitStatus = FALSE;

#define FRARTP_STOP_SEC_VAR_INIT_8
#include <FrArTp_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define FRARTP_START_SEC_CODE
#include <FrArTp_MemMap.h>

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00147,1 */
FUNC(void, FRARTP_CODE) FrArTp_Init
(
  P2CONST(FrArTp_ConfigType, AUTOMATIC, FRARTP_APPL_CONST) ConfigPtr
)
{
  DBG_FRARTP_INIT_ENTRY(ConfigPtr);

  if(FrArTp_CfgAccess_init(ConfigPtr) == E_OK)
  {
    FrArTp_SM_init();

    FrArTp_Encode_init();

    /* FrArTp stack finished initialization */
    FrArTp_InitStatus = TRUE;
  }

  DBG_FRARTP_INIT_EXIT(ConfigPtr);
}

#if (FRARTP_VERSION_INFO_API == STD_ON)
/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00215,1 */
FUNC(void, FRARTP_CODE) FrArTp_GetVersionInfo
(
   CONSTP2VAR(Std_VersionInfoType, AUTOMATIC, FRARTP_APPL_DATA) versioninfo
)
{
  DBG_FRARTP_GETVERSIONINFO_ENTRY(versioninfo);

#if(FRARTP_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00215.E_PARAM_POINTER,1 */
  if(versioninfo == NULL_PTR)
  {
    FRARTP_REPORT_ERROR(FRARTP_GETVERSIONINFO_SERVICE_ID, FRARTP_E_PARAM_POINTER);
  }
  else
#endif /* FRARTP_DEV_ERROR_DETECT == STD_ON */
  {
    *versioninfo = FrArTp_VersionInfo;
  }

  DBG_FRARTP_GETVERSIONINFO_EXIT(versioninfo);
}
#endif /* FRARTP_VERSION_INFO_API == STD_ON */

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00162,1 */
FUNC(void,FRARTP_CODE) FrArTp_MainFunction(void)
{
  DBG_FRARTP_MAINFUNCTION_ENTRY();

  if(FrArTp_InitStatus != FALSE)
  {
    /* Execute the main function of the state machine */
    FrArTp_SM_mainFunction();

    /* Execute the main function of the encode unit */
    FrArTp_Encode_mainFunction();
  }

  DBG_FRARTP_MAINFUNCTION_EXIT();
}

#define FRARTP_STOP_SEC_CODE
#include <FrArTp_MemMap.h>

/*==================[end of file]===========================================*/
