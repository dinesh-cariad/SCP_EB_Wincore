/**
 * \file
 *
 * \brief AUTOSAR FrIf
 *
 * This file contains the implementation of the AUTOSAR
 * module FrIf.
 *
 * \version 5.3.25
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*======================[Include Section]=====================================*/

#include <FrIf_guards.h> /* Include define guards */
#include <FrIf_Trace.h>
#include <FrIf_Priv.h>

/*======================[Local Macros]========================================*/

/*======================[Local Data Types]====================================*/

/*======================[Local Data]==========================================*/

/*======================[Local Functions]=====================================*/

/*======================[Global Data]=========================================*/

/*======================[Global Functions]====================================*/

#if (FRIF_VERSION_INFO_API == STD_ON)

/* start code section declaration */
#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_START_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */


/**
 * \brief   Returns the module version information.
 *
 * \param versioninfo (out)      Address the version information should be written to.
 *
 */

FUNC(void,FRIF_CODE) FrIf_GetVersionInfo
    (
        P2VAR(Std_VersionInfoType,AUTOMATIC,FRIF_APPL_DATA) FrIf_VersionInfoPtr
    )
{
    DBG_FRIF_GETVERSIONINFO_ENTRY(FrIf_VersionInfoPtr);

/* check if development error detection is enabled */
#if (FRIF_DEV_ERROR_DETECT == STD_ON)

    /* check if controller index is within supported bounds */
    if(FrIf_VersionInfoPtr == NULL_PTR)
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_GETVERSIONINFO_SERVICE_ID, FRIF_E_INV_POINTER);
    }
    else

#endif  /* FRIF_DEV_ERROR_DETECT */

    {
        /* assign logistic values to structure members */
        FrIf_VersionInfoPtr->vendorID = (uint16) FRIF_VENDOR_ID;
        FrIf_VersionInfoPtr->moduleID = (uint16) FRIF_MODULE_ID;
        FrIf_VersionInfoPtr->sw_major_version = (uint8) FRIF_SW_MAJOR_VERSION;
        FrIf_VersionInfoPtr->sw_minor_version = (uint8) FRIF_SW_MINOR_VERSION;
        FrIf_VersionInfoPtr->sw_patch_version = (uint8) FRIF_SW_PATCH_VERSION;
    }

    DBG_FRIF_GETVERSIONINFO_EXIT(FrIf_VersionInfoPtr);
}

/* stop code section declaration */
#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_STOP_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */

#endif /* FRIF_VERSION_INFO_API */

