/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <CanTp_Trace.h>
#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */
#include <CanTp_Internal.h>    /* internal macros and variables */
#include <CanTp_Api.h>         /* CanTp API declarations */
#include <CanTp_Version.h>     /* CanTp version declarations */
#if(CANTP_JUMPTABLE_SUPPORT == STD_ON)
#include <CanTp_EntryTable.h>  /* CanTp entry jumptable */
#endif /* CANTP_JUMPTABLE_SUPPORT == STD_ON */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#if(CANTP_VERSION_INFO_API == STD_ON)

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON)

/** \brief Get version information.
 **
 ** This service returns the version information of this module.
 **
 ** \param[out]  VersionInfoPtr  Pointer to store the version information of
 **                              this module (Module Id, Vendor Id, Vendor
 **                              specific version numbers).
 */
FUNC(void, CANTP_CODE) CanTp_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CANTP_APPL_DATA) VersionInfoPtr
)
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
{
  DBG_CANTP_GETVERSIONINFO_ENTRY(VersionInfoPtr);
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
  /* check pointer - possible DET and return on error */
  if(VersionInfoPtr == NULL_PTR)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_GETVERSIONINFO,
                            CANTP_E_PARAM_POINTER
                          );
  }
  else
#endif
  {
    /* return version information */
    VersionInfoPtr->vendorID         = CANTP_VENDOR_ID;
    VersionInfoPtr->moduleID         = CANTP_MODULE_ID;
    VersionInfoPtr->sw_major_version = CANTP_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = CANTP_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = CANTP_SW_PATCH_VERSION;
  }

  DBG_CANTP_GETVERSIONINFO_EXIT(VersionInfoPtr);
}
#else /* => CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_CLIENT */
{
  DBG_CANTP_GETVERSIONINFO_ENTRY(VersionInfoPtr);
  (CanTp_EntryTablePtr->GetVersionInfo)(VersionInfoPtr);

  DBG_CANTP_GETVERSIONINFO_EXIT(VersionInfoPtr);
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

#endif /* CANTP_VERSION_INFO_API == STD_ON */

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
