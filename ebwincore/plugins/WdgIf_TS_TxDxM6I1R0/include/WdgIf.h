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
#ifndef WDGIF_H
#define WDGIF_H

/*==================[inclusions]=============================================*/
#include <Std_Types.h>      /* AUTOSAR standard type definitions */

#include <WdgIf_Version.h>  /* this module's version declaration */
#include <WdgIf_Types.h>    /* WdgIf type definitions */
#include <WdgIf_Cfg.h>      /* WdgIf pre-compile configuration */

#if ((WDGIF_DEV_ERROR_DETECT == STD_OFF) || (WDGIF_EB_NUMBER_OF_DEVICES == 1U))
/* For the given configuration the WdgIf APIs WdgIf_SetMode and WdgIf_SetTriggerCondition are
   considered as a macro implementation.
   Thus, Wdg<xxx>.h must be included in WdgIf.h instead. */
#include <WdgIf_WdgDrivers.h>   /* generated file holding the inclusion of header files
                                   from the Wdg drivers to be applied. */
#endif /* ((WDGIF_DEV_ERROR_DETECT == STD_OFF) || (WDGIF_EB_NUMBER_OF_DEVICES == 1U)) */

/*==================[macros]================================================*/

/*------------------[AUTOSAR API function IDs]--------------------------*/

#ifdef WDGIF_SID_SETMODE
#error WDGIF_SID_SETMODE already defined!
#endif
/** \brief API service id for WdgIf_SetMode() */
#define WDGIF_SID_SETMODE          0x01U


#ifdef WDGIF_SID_SETTRIGGERCOND
#error WDGIF_SID_SETTRIGGERCOND already defined!
#endif
/** \brief API service id for WdgIf_SetTriggerCondition() */
#define WDGIF_SID_SETTRIGGERCOND   0x02U


#ifdef WDGIF_SID_GETVERSIONINFO
#error WDGIF_SID_GETVERSIONINFO already defined!
#endif
/** \brief API service id for WdgIf_GetVersionInfo() */
#define WDGIF_SID_GETVERSIONINFO   0x03U

/*-----------------[AUTOSAR API module error codes]-------------------------*/

#ifdef WDGIF_E_PARAM_DEVICE
#error "WDGIF_E_PARAM_DEVICE macro already defined!"
#endif
/** \brief DET: API service called with wrong device index parameter */
#define WDGIF_E_PARAM_DEVICE    0x01U

#ifdef WDGIF_E_INV_POINTER
#error "WDGIF_E_INV_POINTER  macro already defined!"
#endif
/** \brief DET: API service called with null pointer parameter */
#define WDGIF_E_INV_POINTER     0x02U

/*-----------------[Macro implementation of API functions]------------------*/

#if ((WDGIF_DEV_ERROR_DETECT == STD_OFF) || (WDGIF_EB_NUMBER_OF_DEVICES == 1U))
#if (defined WdgIf_SetMode)
#error WdgIf_SetMode already defined
#else
/* Macro for mapping Wdg_SetMode() to the underlying watchdog device */
#define WdgIf_SetMode(DeviceIndex,WdgMode)    WdgIf_IntSetMode((DeviceIndex),(WdgMode))
#endif /* if (defined WdgIf_SetMode) */

#if (defined WdgIf_SetTriggerCondition)
#error WdgIf_SetTriggerCondition already defined
#else
/* Macro for mapping Wdg_SetTriggerCondition() to the underlying watchdog device */
#define WdgIf_SetTriggerCondition(DeviceIndex,Timeout)    WdgIf_IntTrigger((DeviceIndex),(Timeout))
#endif /* if (defined WdgIf_SetTriggerCondition) */

#endif  /* ((WDGIF_DEV_ERROR_DETECT == STD_OFF) || (WDGIF_EB_NUMBER_OF_DEVICES == 1U)) */

/*==================[type definitions]======================================*/

#if (WDGIF_EB_NUMBER_OF_DEVICES > 1U)

/** \brief Function pointer for setting the mode of an underlying watchdog
 ** device */
typedef P2FUNC(Std_ReturnType, WDGIF_CODE, WdgIf_SetModeFpType)(WdgIf_ModeType Mode);

/** \brief Function pointer for triggering an underlying watchdog device  */
typedef P2FUNC(void, WDGIF_CODE, WdgIf_SetTriggerCondFpType)(uint16 Timeout);

#endif

/*==================[external function declarations]========================*/

#define WDGIF_START_SEC_CODE_ASIL_D
#include <WdgIf_MemMap.h>

#if ((WDGIF_DEV_ERROR_DETECT == STD_ON) && (WDGIF_EB_NUMBER_OF_DEVICES > 1U))

/** \brief Set mode of the Watchdog driver
 **
 ** This function provides access to the mode switching services of the
 ** underlying watchdog drivers. The function is mapped to the service
 ** Wdg_SetMode().
 **
 ** This function is implemented as macro if development error detection is
 ** turned off.
 **
 ** \pre Parameter \a DeviceIndex is valid (the device ID exists in the configuration)
 **
 ** \param DeviceIndex index of requested watchdog driver
 ** \param WdgMode requested mode of watchdog driver
 **
 ** \return Std_ReturnType
 **
 ** \ServiceID{::WDGIF_SID_SETMODE}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, WDGIF_CODE) WdgIf_SetMode
(
  uint8          DeviceIndex,
  WdgIf_ModeType WdgMode
);

/** \brief Trigger the Watchdog driver
 **
 ** This service maps the service WdgIf_SetTriggerCondition to the service
 ** Wdg_SetTriggerCondition of the corresponding Watchdog Driver.
 **
 ** This function is implemented as macro if development error detection is
 ** turned off.
 **
 ** \pre Parameter \a DeviceIndex is valid (the device ID exists in the configuration)
 **
 ** \param[in] DeviceIndex   Identifies the Watchdog Driver instance.
 ** \param[in] Timeout       Timeout value (milliseconds) for setting the trigger counter.
 **
 ** \ServiceID{::WDGIF_SID_SETTRIGGERCOND}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, WDGIF_CODE) WdgIf_SetTriggerCondition
(
  uint8  DeviceIndex,
  uint16 Timeout
);

#endif  /* (WDGIF_DEV_ERROR_DETECT == STD_ON) && .. */


#if (WDGIF_VERSION_INFO_API == STD_ON)

/** \brief Get version information of the Watchdog Interface
 **
 ** This service returns the version information of this module.
 **
 ** \pre Parameter \a VersionInfoPtr is a valid pointer
 **
 ** \param[out] VersionInfoPtr Pointer to where to store the version
 **                            information of this module
 **
 ** \ServiceID{::WDGIF_SID_GETVERSIONINFO}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, WDGIF_CODE) WdgIf_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, WDGIF_APPL_DATA) VersionInfoPtr
);

#endif  /* WDGIF_VERSION_INFO_API == STD_ON */

#define WDGIF_STOP_SEC_CODE_ASIL_D
#include <WdgIf_MemMap.h>

/*==================[external constants]====================================*/

#if (WDGIF_EB_NUMBER_OF_DEVICES > 1U)

/* API functions WdgIf_SetMode() / WdgIf_SetTriggerCondition map Wdg function calls by
 * calling the requested function pointer within WdgIf_SetModeFpArray /
 * WdgIf_TriggerFpArray Array. */

#define WDGIF_START_SEC_CONST_ASIL_D_UNSPECIFIED
#include <WdgIf_MemMap.h>

/** \brief Array of function pointers to watchdog specific functions for
 ** setting the watchdog mode
 **
 ** The array contains function pointers for setting the mode of underlying
 ** watchdog devices. The Array is sized according to the number of configured
 ** watchdog devices.
 **/
extern CONST(WdgIf_SetModeFpType, WDGIF_CONST) WdgIf_SetModeFpArray[
   WDGIF_EB_NUMBER_OF_DEVICES];

/** \brief Array of function pointers to watchdog specific functions for
 ** triggering the watchdog
 **
 ** The array contains function pointers for triggering the underlying
 ** watchdog devices. The Array is sized according to the number of configured
 ** watchdog devices.
 **/
extern CONST(WdgIf_SetTriggerCondFpType, WDGIF_CONST) WdgIf_TriggerFpArray[
   WDGIF_EB_NUMBER_OF_DEVICES];

#define WDGIF_STOP_SEC_CONST_ASIL_D_UNSPECIFIED
#include <WdgIf_MemMap.h>

#endif  /* (WDGIF_EB_NUMBER_OF_DEVICES > 1) */

/*==================[external data]=========================================*/

#endif /* ifndef WDGIF_H */
/*==================[end of file]===========================================*/
