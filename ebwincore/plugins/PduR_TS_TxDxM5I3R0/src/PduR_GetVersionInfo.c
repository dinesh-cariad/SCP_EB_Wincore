/**
 * \file
 *
 * \brief AUTOSAR PduR
 *
 * This file contains the implementation of the AUTOSAR
 * module PduR.
 *
 * \version 5.3.46
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <PduR_Trace.h>
#include <TSAutosar.h>            /* EB specific standard types */
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <PduR_Internal_Static.h> /* Internal API (static part) */
#include <PduR_Api_Depend.h>      /* Public API (dependent part) */
#include <PduR_Version.h>         /* Version Information */

#if ((PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) && (PDUR_VERSION_INFO_API == STD_ON))
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/* AUTOSAR Memory Mapping - start section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_START_SEC_CODE
#include <PduR_MemMap.h>
#endif

FUNC(void, PDUR_CODE) PduR_GetVersionInfo
(
 P2VAR(Std_VersionInfoType, AUTOMATIC, PDUR_APPL_DATA) versionInfo
)
{
    DBG_PDUR_GETVERSIONINFO_ENTRY(versionInfo);
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
    if ( NULL_PTR == versionInfo )
    {
        PDUR_DET_REPORT_ERROR(PDUR_SID_GET_VER_INF, PDUR_E_NULL_POINTER);
    }
    else
#endif /* if (PDUR_DEV_ERROR_DETECT == STD_ON) */

    {
        /* get version info of this module */
        versionInfo->vendorID = PDUR_VENDOR_ID;
        versionInfo->moduleID = PDUR_MODULE_ID;
        versionInfo->sw_major_version = PDUR_SW_MAJOR_VERSION;
        versionInfo->sw_minor_version = PDUR_SW_MINOR_VERSION;
        versionInfo->sw_patch_version = PDUR_SW_PATCH_VERSION;
    }

    DBG_PDUR_GETVERSIONINFO_EXIT(versionInfo);
}


/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if ((PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) && (PDUR_VERSION_INFO_API == STD_ON)) */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT


/*==================[internal function definitions]=========================*/

#endif /* if ((PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) && (PDUR_VERSION_INFO_API == STD_ON)) */
/*==================[end of file]===========================================*/
