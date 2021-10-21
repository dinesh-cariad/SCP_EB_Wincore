/**
 * \file
 *
 * \brief AUTOSAR PbcfgM
 *
 * This file contains the implementation of the AUTOSAR
 * module PbcfgM.
 *
 * \version 1.2.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/*==================[inclusions]============================================*/
#include <PbcfgM_Trace.h>
#include <Std_Types.h>
#include <PbcfgM_Version.h>
#include <PbcfgM_Int.h>
#include <PbcfgM_Types.h>
#include <PbcfgM_Api.h>
#include <PbcfgM_Lcfg.h>
#if (PBCFGM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif

#if (defined TS_RELOCATABLE_CFG_ENABLE)
#error TS_RELOCATABLE_CFG_ENABLE already defined
#endif
#define TS_RELOCATABLE_CFG_ENABLE PBCFGM_RELOCATABLE_CFG_ENABLE

#include <TSPBConfig_Init.h>
#include <TSPBConfig_Access.h>
#include <TSPBConfig_Signature.h>

/*==================[macros]================================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef PBCFGM_VENDOR_ID /* configuration check */
#error PBCFGM_VENDOR_ID must be defined
#endif

#if (PBCFGM_VENDOR_ID != 1U) /* vendor check */
#error PBCFGM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef PBCFGM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error PBCFGM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (PBCFGM_AR_RELEASE_MAJOR_VERSION != 4U)
#error PBCFGM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef PBCFGM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error PBCFGM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (PBCFGM_AR_RELEASE_MINOR_VERSION != 0U )
#error PBCFGM_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef PBCFGM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error PBCFGM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (PBCFGM_AR_RELEASE_REVISION_VERSION != 3U )
#error PBCFGM_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef PBCFGM_SW_MAJOR_VERSION /* configuration check */
#error PBCFGM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (PBCFGM_SW_MAJOR_VERSION != 1U)
#error PBCFGM_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef PBCFGM_SW_MINOR_VERSION /* configuration check */
#error PBCFGM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (PBCFGM_SW_MINOR_VERSION < 2U)
#error PBCFGM_SW_MINOR_VERSION wrong (< 2U)
#endif

#ifndef PBCFGM_SW_PATCH_VERSION /* configuration check */
#error PBCFGM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (PBCFGM_SW_PATCH_VERSION < 22U)
#error PBCFGM_SW_PATCH_VERSION wrong (< 22U)
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define PBCFGM_START_SEC_CODE
#include <PbcfgM_MemMap.h>

static FUNC(Std_ReturnType, PBCFGM_CODE) PbcfgM_AreTheModuleConfigsValid(
    P2CONST(PbcfgM_ConfigType, AUTOMATIC, PBCFGM_APPL_CONST) PbcfgM_ConfigPtr
);

#define PBCFGM_STOP_SEC_CODE
#include <PbcfgM_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define PBCFGM_START_SEC_VAR_FAST_INIT_8
#include <PbcfgM_MemMap.h>

#if (PBCFGM_DEV_ERROR_DETECT == STD_ON)
/** \brief PbcfgM_InitStatus
 * global variable for the actual initialization status of the PbcfgM */
STATIC VAR(PbcfgM_StatusType, PBCFGM_VAR_FAST) PbcfgM_InitStatus = PBCFGM_UNINIT;
#endif

#define PBCFGM_STOP_SEC_VAR_FAST_INIT_8
#include <PbcfgM_MemMap.h>

#define PBCFGM_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include <PbcfgM_MemMap.h>

/** \brief PbcfgM_ConfigPtr
 * global variable for the configuration pointer of the PbcfgM */
STATIC P2CONST(PbcfgM_ConfigType, PBCFGM_VAR_FAST, PBCFGM_APPL_CONST) PbcfgM_GConfigPtr = NULL_PTR;

#define PBCFGM_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include <PbcfgM_MemMap.h>

/*==================[external function definitions]=========================*/

#define PBCFGM_START_SEC_CODE
#include <PbcfgM_MemMap.h>

FUNC(void, PBCFGM_CODE) PbcfgM_Init
(
    P2CONST(PbcfgM_ConfigType, AUTOMATIC, PBCFGM_APPL_CONST) PbcfgM_ConfigPtr
)
{
    DBG_PBCFGM_INIT_ENTRY(PbcfgM_ConfigPtr);
#if (PBCFGM_DEV_ERROR_DETECT == STD_ON)
    /* Check whether parameter PbcfgM_ConfigPtr is a NULL_PTR */
    if(PbcfgM_ConfigPtr == NULL_PTR)
    {
        /* Found a NULL_PTR, report to DET */
        (void)Det_ReportError(PBCFGM_MODULE_ID,
                              PBCFGM_INSTANCE_ID,  /* PbcfgM instance ID parameter */
                              PBCFGM_SID_INIT,
                              PBCFGM_E_NULL_POINTER
                             );
    }
    else
#endif /* PBCFGM_DEV_ERROR_DETECT */
    {
        /* Copy the the configuration to global pointer */
        PbcfgM_GConfigPtr = PbcfgM_ConfigPtr;

#if (PBCFGM_DEV_ERROR_DETECT == STD_ON)
        /* Tag module as initialized */
        /* !LINKSTO PbcfgM.PbcfgMTransition.T1,1 */
        PbcfgM_InitStatus = PBCFGM_INIT;
#endif
    }

    DBG_PBCFGM_INIT_EXIT(PbcfgM_ConfigPtr);
}

/*--------------------------------------------------------------------------*/
FUNC(Std_ReturnType, PBCFGM_CODE) PbcfgM_GetConfig(
        uint16 PbcfgM_ModuleId,
        uint16 PbcfgM_InstanceId,
        P2VAR(PbcfgM_ModuleConfigPtrType, AUTOMATIC, PBCFGM_APPL_DATA) PbcfgM_ModuleConfigPtr)
{
    Std_ReturnType Result = E_NOT_OK;

    DBG_PBCFGM_GETCONFIG_ENTRY(PbcfgM_ModuleId,PbcfgM_InstanceId,PbcfgM_ModuleConfigPtr);

#if (PBCFGM_DEV_ERROR_DETECT == STD_ON)
    /* Check if PbcfgM is initialized */
    if(PbcfgM_InitStatus == PBCFGM_UNINIT)
    {
        /* Reported to Det in development mode */
        (void)Det_ReportError(PBCFGM_MODULE_ID,
                              PBCFGM_INSTANCE_ID,  /* PBCFGM Instance ID parameter */
                              PBCFGM_SID_GET_CONFIG,
                              PBCFGM_E_UNINIT
                             );

    }
    else if (PbcfgM_ModuleConfigPtr == NULL_PTR)
    {
        /* reported to Det in development mode */
        (void)Det_ReportError(PBCFGM_MODULE_ID,
                              PBCFGM_INSTANCE_ID,  /* PBCFGM instance ID parameter */
                              PBCFGM_SID_GET_CONFIG,
                              PBCFGM_E_NULL_POINTER
                             );
    }
    else
#endif /* #if (PBCFGM_DEV_ERROR_DETECT == STD_ON) */
    {
        uint16_least Index;
        CONSTP2CONST(PbcfgM_ModuleConfigEntryType, AUTOMATIC, PBCFGM_APPL_CONST) pConfigEntries =
                TS_UNCHECKEDGETCFG(PbcfgM_GConfigPtr, PbcfgM_ModuleConfigEntryType, PBCFGM, PbcfgM_GConfigPtr->ConfigEntryRef);

        for(Index = 0U ; Index <  PbcfgM_GConfigPtr->numConfigEntry ; Index++)
        {
            if (pConfigEntries[Index].ModuleId == PbcfgM_ModuleId)
            {
                if (pConfigEntries[Index].InstanceId == PbcfgM_InstanceId)
                {
                    *PbcfgM_ModuleConfigPtr =
                            TS_CHECKEDGETCFG(PbcfgM_GConfigPtr, void, PBCFGM, pConfigEntries[Index].ConfigRef);
                    Result = E_OK;
                    break;
                }
            }
        }
    }

    DBG_PBCFGM_GETCONFIG_EXIT(Result,PbcfgM_ModuleId,PbcfgM_InstanceId,PbcfgM_ModuleConfigPtr);
    return Result;
}
/*--------------------------------------------------------------------------*/
FUNC(Std_ReturnType, PBCFGM_CODE) PbcfgM_IsValidConfig
(
    P2CONST(PbcfgM_ConfigType, AUTOMATIC, PBCFGM_APPL_CONST) PbcfgM_ConfigPtr
)
{
    /* Assume an invalid configuration */
    Std_ReturnType RetVal = E_NOT_OK;

    DBG_PBCFGM_ISVALIDCONFIG_ENTRY(PbcfgM_ConfigPtr);

    if (PbcfgM_ConfigPtr != NULL_PTR)
    {
        /* Check if the configuration fits to the platform */
        if (TS_PlatformSigIsValid(PbcfgM_ConfigPtr->PlatformSignature) == TRUE)
        {
            /* Validate the Published information in post build configuration against the
               Published information in compile time configuration */
            if (PBCFGM_PUBLIC_INFO_SIGNATURE == PbcfgM_ConfigPtr->PublicInfoSignature)
            {
                /* Validate the post build configuration against the compile time configuration */
                if (PBCFGM_CFG_SIGNATURE == PbcfgM_ConfigPtr->CfgSignature)
                {
                    /* Validate the post build configuration against the link time configuration */
                    if (PbcfgM_LcfgSignature == PbcfgM_ConfigPtr->LcfgSignature)
                    {
                        RetVal = PbcfgM_AreTheModuleConfigsValid(PbcfgM_ConfigPtr);
                    }
                }
            }
        }
    }

    DBG_PBCFGM_ISVALIDCONFIG_EXIT(RetVal,PbcfgM_ConfigPtr);
    return RetVal;
}

/*--------------------------------------------------------------------------*/

#define PBCFGM_STOP_SEC_CODE
#include <PbcfgM_MemMap.h>

/*==================[internal function definitions]=========================*/

#define PBCFGM_START_SEC_CODE
#include <PbcfgM_MemMap.h>

/*--------------------------------------------------------------------------*/
/* Function checks if all referenced configurations are valid.
 * If one of the configurations is invalid the function returns with E_NOT_OK.
 * If no configuration is referenced the function returns with E_OK.
 * If all configurations are valid the function returns with E_OK */
static FUNC(Std_ReturnType, PBCFGM_CODE) PbcfgM_AreTheModuleConfigsValid
(
    P2CONST(PbcfgM_ConfigType, AUTOMATIC, PBCFGM_APPL_CONST) PbcfgM_ConfigPtr
)
{
    /* Assume valid configurations */
    Std_ReturnType RetVal = E_OK;

    uint16_least Index = 0U;

    while (PbcfgM_IsValidCfgFuncPtr[Index] != NULL_PTR)
    {
        /* Get the pointer to the first configuration entry */
        CONSTP2CONST(PbcfgM_ModuleConfigEntryType, AUTOMATIC, PBCFGM_APPL_CONST) ConfigEntriesPtr =
        TS_UNCHECKEDGETCFG(PbcfgM_ConfigPtr, PbcfgM_ModuleConfigEntryType, PBCFGM,
                PbcfgM_ConfigPtr->ConfigEntryRef);

        /* Get pointer to the module configuration */
        CONSTP2CONST(void, AUTOMATIC, PBCFGM_APPL_CONST) PbcfgMModuleConfigPtr=
                TS_CHECKEDGETCFG(PbcfgM_ConfigPtr, void, PBCFGM, ConfigEntriesPtr[Index].ConfigRef);

        /* Call the 'Is Valid Function' of the module */
        if (PbcfgM_IsValidCfgFuncPtr[Index](PbcfgMModuleConfigPtr) == E_NOT_OK)
        {
            /* Indicate that not all configurations are valid */
            RetVal = E_NOT_OK;
            break;
        }
        Index++;
    }

    return RetVal;
}

#define PBCFGM_STOP_SEC_CODE
#include <PbcfgM_MemMap.h>

/*==================[end of file]===========================================*/
