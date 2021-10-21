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

#include <LinSM_Trace.h>
#include <LinSM_Internal.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define LINSM_START_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
#endif /* TS_MERGED_COMPILE */

#if (LINSM_VERSION_INFO_API == STD_ON)

FUNC(void, LINSM_CODE) LinSM_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, LINSM_APPL_DATA) versioninfo
)
{
  DBG_LINSM_GETVERSIONINFO_ENTRY(versioninfo);
#if (LINSM_DEV_ERROR_DETECT == STD_ON)
  if ( versioninfo == NULL_PTR )
  {
    LINSM_DET(LINSM_SID_GETVERSIONINFO, LINSM_E_PARAMETER_POINTER);
  }
  else

#endif
  {
    versioninfo->vendorID         = LINSM_VENDOR_ID;
    versioninfo->moduleID         = LINSM_MODULE_ID;
    versioninfo->sw_major_version = LINSM_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = LINSM_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = LINSM_SW_PATCH_VERSION;
  }

  DBG_LINSM_GETVERSIONINFO_EXIT(versioninfo);
}

#endif /* (LINSM_VERSION_INFO_API == STD_ON) */

#if (TS_MERGED_COMPILE == STD_OFF)
#define LINSM_STOP_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
#endif /* TS_MERGED_COMPILE */

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

