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
#include <LinIf_Int.h>        /* LinIf internal data */

#if (LINIF_VERSION_INFO_API == STD_ON)

#define LINIF_NO_PBCFG_REQUIRED
#include <LinIf.h>            /* Module library API */

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
FUNC(void, LINIF_CODE) LinIf_GetVersionInfo
(
    P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_DATA) versioninfo
)
{
    DBG_LINIF_GETVERSIONINFO_ENTRY(versioninfo);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    /* DET checks first */

    if ( versioninfo == NULL_PTR )
    {
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINIF_API_GETVERSIONINFO,
                              LINIF_E_PARAMETER_POINTER
                             );
    }
    else

#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
    {
        versioninfo->vendorID         = LINIF_VENDOR_ID;
        versioninfo->moduleID         = LINIF_MODULE_ID;
        versioninfo->sw_major_version = LINIF_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = LINIF_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = LINIF_SW_PATCH_VERSION;
    }

    DBG_LINIF_GETVERSIONINFO_EXIT(versioninfo);
}

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

#else /* (LINIF_VERSION_INFO_API == STD_ON) */

/* prevent empty translation unit in case LinIf_Merged.c is not used */
#if (TS_MERGED_COMPILE == STD_OFF)

#include <TSCompiler.h>
TS_PREVENTEMPTYTRANSLATIONUNIT

#endif /* TS_MERGED_COMPILE */

#endif /* (LINIF_VERSION_INFO_API == STD_ON) */

/*==================[end of file]============================================*/
