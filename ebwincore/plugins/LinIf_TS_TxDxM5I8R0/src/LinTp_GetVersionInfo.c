/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <LinIf_Trace.h>

#include <LinIf_Cfg.h>        /* Generated configuration */
#include <TSAutosar.h>        /* EB specific standard types */

#if (LINIF_TP_SUPPORTED == STD_ON)
#if (LINTP_VERSION_INFO_API == STD_ON)
#if (defined LINTP_USE_VERSIONINFO_API)
#error LINTP_USE_VERSIONINFO_API already defined!
#endif /* #if( defined LINTP_USE_VERSIONINFO_API)*/
#define LINTP_USE_VERSIONINFO_API STD_ON

#else /* (LINTP_VERSION_INFO_API == STD_ON) */

#if (defined LINTP_USE_VERSIONINFO_API)
#error LINTP_USE_VERSIONINFO_API already defined!
#endif /* #if( defined LINTP_USE_VERSIONINFO_API)*/
#define LINTP_USE_VERSIONINFO_API STD_OFF
#endif /* (LINTP_VERSION_INFO_API == STD_ON) */

#else /*  (LINIF_TP_SUPPORTED == STD_ON) */
#if (defined LINTP_USE_VERSIONINFO_API)
#error LINTP_USE_VERSIONINFO_API already defined!
#endif /* #if( defined LINTP_USE_VERSIONINFO_API)*/
#define LINTP_USE_VERSIONINFO_API STD_OFF

#endif /*  (LINIF_TP_SUPPORTED == STD_ON) */

#if (LINTP_USE_VERSIONINFO_API == STD_ON)

#define LINIF_NO_PBCFG_REQUIRED
#include <LinIf.h>            /* Module library API */
#include <LinIf_Int.h>        /* LinIf internal data */
#include <LinTp_Int.h>        /* LinTp internal data */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

/*==================[external function definitions]==========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

/** \brief Return version Information
 **
 ** \param[out] versioninfo Version information are written to this variable.
 **
 */
FUNC(void, LINIF_CODE) LinTp_GetVersionInfo
(
    P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_DATA) versioninfo
)
{

    DBG_LINTP_GETVERSIONINFO_ENTRY(versioninfo);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    /* DET checks first */

    if ( versioninfo == NULL_PTR )
    {
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINTP_API_GETVERSIONINFO,
                              LINIF_E_PARAMETER_POINTER
                             );
    }
    else

#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
    {
        /* LIN Transport Layer has no own version.
           Return version information of LIN Interface instead */
        versioninfo->vendorID   = LINIF_VENDOR_ID;
        versioninfo->moduleID   = LINIF_MODULE_ID;
        versioninfo->sw_major_version = LINIF_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = LINIF_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = LINIF_SW_PATCH_VERSION;
    }

    DBG_LINTP_GETVERSIONINFO_EXIT(versioninfo);
}

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

#else /* (LINTP_USE_VERSIONINFO_API == STD_ON) */

/* prevent empty translation unit in case LinIf_Merged.c is not used */
#if (TS_MERGED_COMPILE == STD_OFF)

#include <TSCompiler.h>
TS_PREVENTEMPTYTRANSLATIONUNIT

#endif /* TS_MERGED_COMPILE */

#endif /* (LINTP_USE_VERSIONINFO_API == STD_ON) */

/*==================[end of file]============================================*/
