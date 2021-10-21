/**
 * \file
 *
 * \brief AUTOSAR WdgIf
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgIf.
 *
 * \version 6.1.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <WdgIf_Trace.h>
#include <Std_Types.h>          /* AUTOSAR standard types */
#include <WdgIf.h>              /* WdgIf regular public API */

#if ((WDGIF_DEV_ERROR_DETECT == STD_ON) && (WDGIF_EB_NUMBER_OF_DEVICES > 1U))
#include <WdgIf_WdgDrivers.h>   /* generated file holding the inclusion of header files
                                   from the Wdg drivers to be applied. */
#endif

#if (WDGIF_DEV_ERROR_DETECT == STD_ON)
/* conditional include of error detection header */
#include <Det.h>
#endif

#if ((WDGIF_DEV_ERROR_DETECT == STD_OFF)        \
   && (WDGIF_EB_NUMBER_OF_DEVICES == 1U)           \
   && (WDGIF_VERSION_INFO_API == STD_OFF))
/* For the given configuration this translation unit is empty.  The following
 * code prevents compiler errors or warnings in this case */
typedef void TSPreventEmptyTranslationUnitType;
#endif

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef WDGIF_VENDOR_ID /* configuration check */
#error WDGIF_VENDOR_ID must be defined
#endif

#if (WDGIF_VENDOR_ID != 1U) /* vendor check */
#error WDGIF_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef WDGIF_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error WDGIF_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (WDGIF_AR_RELEASE_MAJOR_VERSION != 4U)
#error WDGIF_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef WDGIF_AR_RELEASE_MINOR_VERSION /* configuration check */
#error WDGIF_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (WDGIF_AR_RELEASE_MINOR_VERSION != 0U )
#error WDGIF_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef WDGIF_AR_RELEASE_REVISION_VERSION /* configuration check */
#error WDGIF_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (WDGIF_AR_RELEASE_REVISION_VERSION != 3U )
#error WDGIF_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef WDGIF_SW_MAJOR_VERSION /* configuration check */
#error WDGIF_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (WDGIF_SW_MAJOR_VERSION != 6U)
#error WDGIF_SW_MAJOR_VERSION wrong (!= 6U)
#endif

#ifndef WDGIF_SW_MINOR_VERSION /* configuration check */
#error WDGIF_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (WDGIF_SW_MINOR_VERSION < 1U)
#error WDGIF_SW_MINOR_VERSION wrong (< 1U)
#endif

#ifndef WDGIF_SW_PATCH_VERSION /* configuration check */
#error WDGIF_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (WDGIF_SW_PATCH_VERSION < 26U)
#error WDGIF_SW_PATCH_VERSION wrong (< 26U)
#endif

/*------------------[DET check macro]----------------------------------------*/
#if (WDGIF_DEV_ERROR_DETECT == STD_ON)
#if (defined WDGIF_DET_REPORT_ERROR)
#error WDGIF_DET_REPORT_ERROR already defined
#endif /* if (defined WDGIF_DET_REPORT_ERROR) */

/** \brief Define WDGIF_DET_REPORT_ERROR for simplified error reporting */
#define WDGIF_DET_REPORT_ERROR(ServiceId,ErrorCode)                     \
   (void)Det_ReportError(WDGIF_MODULE_ID, 0U, (ServiceId), (ErrorCode))

#endif /* if (WDGIF_DEV_ERROR_DETECT == STD_ON) */
/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

#if (WDGIF_EB_NUMBER_OF_DEVICES > 1U)

#define WDGIF_START_SEC_CONST_ASIL_D_UNSPECIFIED
#include <WdgIf_MemMap.h>

/* !LINKSTO WDGIF.EB.DesignDecision.InternalVariable1,1 */
/* function pointer array for SetMode functions */
CONST(WdgIf_SetModeFpType, WDGIF_CONST) WdgIf_SetModeFpArray[
   WDGIF_EB_NUMBER_OF_DEVICES] = WDGIF_SETMODEPTRS;

/* !LINKSTO WDGIF.EB.DesignDecision.InternalVariable2,1 */
/* function pointer array for Trigger functions */
CONST(WdgIf_SetTriggerCondFpType, WDGIF_CONST) WdgIf_TriggerFpArray[
   WDGIF_EB_NUMBER_OF_DEVICES] = WDGIF_TRIGGERPTRS;

#define WDGIF_STOP_SEC_CONST_ASIL_D_UNSPECIFIED
#include <WdgIf_MemMap.h>

#endif  /* WDGIF_EB_NUMBER_OF_DEVICES > 1 */

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#define WDGIF_START_SEC_CODE_ASIL_D
#include <WdgIf_MemMap.h>

#if ((WDGIF_DEV_ERROR_DETECT == STD_ON) && (WDGIF_EB_NUMBER_OF_DEVICES > 1U))

FUNC(void, WDGIF_CODE) WdgIf_SetTriggerCondition
(
  uint8  DeviceIndex,
  uint16 Timeout
)
{
  DBG_WDGIF_SETTRIGGERCONDITION_ENTRY(DeviceIndex,Timeout);

  if (DeviceIndex >= WDGIF_EB_NUMBER_OF_DEVICES)
  {
    WDGIF_DET_REPORT_ERROR(WDGIF_SID_SETTRIGGERCOND, WDGIF_E_PARAM_DEVICE);
  }
  else
  {
    /* select and call appropriate function */
    WdgIf_IntTrigger(DeviceIndex,Timeout);
  }

  DBG_WDGIF_SETTRIGGERCONDITION_EXIT(DeviceIndex,Timeout);
}

FUNC(Std_ReturnType, WDGIF_CODE) WdgIf_SetMode
(
  uint8          DeviceIndex,
  WdgIf_ModeType WdgMode
)
{
  Std_ReturnType ret;

  DBG_WDGIF_SETMODE_ENTRY(DeviceIndex,WdgMode);

  /* check DeviceIndex parameter for a valid range */
  if (DeviceIndex >= WDGIF_EB_NUMBER_OF_DEVICES)
  {
    WDGIF_DET_REPORT_ERROR(WDGIF_SID_SETMODE, WDGIF_E_PARAM_DEVICE);
    ret = E_NOT_OK;
  }
  /* note: no check for WdgMode: value is not important for us and Wdg driver
   *       will do its own check anyway */
  else
  {
    /* select and call appropriate function pointer */
    ret = WdgIf_IntSetMode(DeviceIndex,WdgMode);
  }

  DBG_WDGIF_SETMODE_EXIT(ret,DeviceIndex,WdgMode);
  return ret;
}

#endif  /* (WDGIF_DEV_ERROR_DETECT == STD_ON) && .. */


#if (WDGIF_VERSION_INFO_API == STD_ON)
FUNC(void, WDGIF_CODE) WdgIf_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, WDGIF_APPL_DATA) VersionInfoPtr
)
{
  DBG_WDGIF_GETVERSIONINFO_ENTRY(VersionInfoPtr);
#if (WDGIF_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == VersionInfoPtr)
  {
    WDGIF_DET_REPORT_ERROR(WDGIF_SID_GETVERSIONINFO, WDGIF_E_INV_POINTER);
  }
  else
#endif /* WDGIF_DEV_ERROR_DETECT */
  {
    VersionInfoPtr->vendorID         = WDGIF_VENDOR_ID;
    VersionInfoPtr->moduleID         = WDGIF_MODULE_ID;
    VersionInfoPtr->sw_major_version = WDGIF_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = WDGIF_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = WDGIF_SW_PATCH_VERSION;
  }

  DBG_WDGIF_GETVERSIONINFO_EXIT(VersionInfoPtr);
}
#endif

#define WDGIF_STOP_SEC_CODE_ASIL_D
#include <WdgIf_MemMap.h>

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
