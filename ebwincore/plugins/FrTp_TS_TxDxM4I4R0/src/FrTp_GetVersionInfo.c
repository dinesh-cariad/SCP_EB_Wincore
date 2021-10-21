/**
 * \file
 *
 * \brief AUTOSAR FrTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTp.
 *
 * \version 4.4.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*======================[Include Section]====================================*/
#include <FrTp_guards.h> /* After foreign/API/Cfg, before internal/private */
#include <FrTp_Trace.h>
#include <FrTp_Priv.h>  /* data types */

/*======================[Local Macros]=======================================*/

/*=====================[Variable Definitions]================================*/

/*
 * Start code section declaration
 */
#define FRTP_START_SEC_CODE
#include <FrTp_MemMap.h>

/*=====================[Local Function Definitions]==========================*/

/*=====================[Function Definitions]================================*/


#if (FRTP_VERSION_INFO_API == STD_ON)

FUNC(void,FRTP_CODE) FrTp_GetVersionInfo
(
    P2VAR(Std_VersionInfoType, AUTOMATIC, FRTP_APPL_DATA) versioninfo
)
{
    DBG_FRTP_GETVERSIONINFO_ENTRY(versioninfo);

#if (FRTP_DEV_ERROR_DETECT == STD_ON)
    if (versioninfo == NULL_PTR)
    {
        (void)Det_ReportError(FRTP_MODULE_ID,
                              FRTP_DET_GENERIC_INSTANCE_ID,
                              FRTP_GETVERSIONINFO_SERVICE_ID,
                              FRTP_E_NULL_PTR
                             );
    }
    else
#endif
    {
        versioninfo->vendorID = FRTP_VENDOR_ID;
        versioninfo->moduleID = FRTP_MODULE_ID;
        versioninfo->sw_major_version = FRTP_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = FRTP_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = FRTP_SW_PATCH_VERSION;
    }

    DBG_FRTP_GETVERSIONINFO_EXIT(versioninfo);
}

#endif /* FRTP_VERSION_INFO_API */


/*
 * Stop code section declaration
 */
#define FRTP_STOP_SEC_CODE
#include <FrTp_MemMap.h>


