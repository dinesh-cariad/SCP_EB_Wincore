#ifndef ECUM_BOOTTARGET_H
#define ECUM_BOOTTARGET_H

/**
 * \file
 *
 * \brief AUTOSAR EcuM
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuM.
 *
 * \version 5.15.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* ATTENTION: If you are including the EcuM_BootTarget.h header file from the
 * boot manager or the boot loader files you have to write the EcuM_Cfg.h file
 * by hand and store it at a location where it can be found by the compiler.
 * EcuM_Cfg.h must contain these definitions for the preprocessor symbols
 * - ECUM_USE_BOOTTARGET_SEC = STD_ON or STD_OFF
 * - ECUM_DEV_ERROR_DETECT   = STD_OFF
 * - ECUM_INCLUDE_RTE        = STD_OFF
 *
 * If ECUM_USE_BOOTTARGET_SEC = STD_ON is used the variable EcuM_BootTarget is
 * put into the memory section SEC_ECUM_BOOTTARGET and in the memory class
 * ECUM_VAR_BOOTTARGET. The linker file must map this section to the same
 * physical address for the appication and the bootloader part. */

/*==================[inclusions]============================================*/

#include <EcuM_BSW_Types.h>     /* static module types */

#include <EcuM_Cfg.h>           /* definition of ECUM_USE_BOOTTARGET_SEC */

/*==================[macros]================================================*/

#ifndef ECUM_USE_BOOTTARGET_SEC
#error ECUM_USE_BOOTTARGET_SEC is not defined
#endif

/* define default memory class for EcuM_BootTarget */
#ifndef ECUM_VAR_BOOTTARGET
#define ECUM_VAR_BOOTTARGET ECUM_VAR_CLEARED
#endif

/*------------------[macros for EcuM_BootTargetType]------------------------*/

/* define symbols otherwise defined in Rte_Type.h if RTE is available */

#ifndef ECUM_BOOT_TARGET_APP
/** \brief Value for EcuM_BootTargetType
 **
 **  The ECU will boot into the application. */
#define ECUM_BOOT_TARGET_APP         0U
#endif

#ifndef ECUM_BOOT_TARGET_OEM_BOOTLOADER
/** \brief Value for EcuM_BootTargetType
 **
 ** The ECU will boot into the OEM bootloader. */
#define ECUM_BOOT_TARGET_OEM_BOOTLOADER  1U
#endif

#if ((ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) || \
     (ECUM_EB_DEFAULT_ASR_SERVICE_API == ECUM_EB_SERVICE_API_ASR32))
#ifndef ECUM_BOOT_TARGET_BOOTLOADER
/** \brief Value for EcuM_BootTargetType
 **
 ** The ECU will boot into the bootloader. */
#define ECUM_BOOT_TARGET_BOOTLOADER  1U
#endif
#endif

#ifndef ECUM_BOOT_TARGET_SYS_BOOTLOADER
/** \brief Value for EcuM_BootTargetType
 **
 ** The ECU will boot into the system supplier bootloader. */
#define ECUM_BOOT_TARGET_SYS_BOOTLOADER  2U
#endif

#if (defined ECUM_BOOT_TARGET_OEM_BOOTLOADER_INTERNAL_0)
#error ECUM_BOOT_TARGET_OEM_BOOTLOADER_INTERNAL_0 already defined
#endif
/** \brief Value for 1st byte of EcuM_BootTargetInternalType.
 **
 ** The ECU will boot into the OEM bootloader. This internal value is written
 ** to memory to make reading of the values more reliable. */
#define ECUM_BOOT_TARGET_OEM_BOOTLOADER_INTERNAL_0   0x00FFA55AU

#if (defined ECUM_BOOT_TARGET_OEM_BOOTLOADER_INTERNAL_1)
#error ECUM_BOOT_TARGET_OEM_BOOTLOADER_INTERNAL_1 already defined
#endif
/** \brief Value for 2nd byte of EcuM_BootTargetInternalType */
#define ECUM_BOOT_TARGET_OEM_BOOTLOADER_INTERNAL_1   0x2342EB6CU

#if (defined ECUM_BOOT_TARGET_SYS_BOOTLOADER_INTERNAL_0)
#error ECUM_BOOT_TARGET_SYS_BOOTLOADER_INTERNAL_0 already defined
#endif
/** \brief Value for 1st byte of EcuM_BootTargetInternalType.
 **
 ** The ECU will boot into the system supplier bootloader. This internal value
 ** is written to memory to make reading of the values more reliable. */
#define ECUM_BOOT_TARGET_SYS_BOOTLOADER_INTERNAL_0   0xEB15C001U

#if (defined ECUM_BOOT_TARGET_SYS_BOOTLOADER_INTERNAL_1)
#error ECUM_BOOT_TARGET_SYS_BOOTLOADER_INTERNAL_1 already defined
#endif
/** \brief Value for 2nd byte of EcuM_BootTargetInternalType */
#define ECUM_BOOT_TARGET_SYS_BOOTLOADER_INTERNAL_1   0x1BADBABEU

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define ECUM_START_SEC_CODE
#include <EcuM_MemMap.h>

#if ((ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (ECUM_EB_DEFAULT_ASR_SERVICE_API == ECUM_EB_SERVICE_API_ASR32))
/* !LINKSTO EcuM.EB.ASR32.EcuM102,1 */
/** \brief Select boot target using AUTOSAR 4.0 API.
 **
 ** This function sets the target which is used after reboot.
 **
 ** \param[in] target The selected boot target.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK The new  boot target was not accepted by EcuM.
 **
 ** \ServiceID{18}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType,ECUM_CODE) EcuM_ASR32_SelectBootTarget
(
    EcuM_ASR32_BootTargetType target
);

/* !LINKSTO EcuM.EB.ASR32.EcuM102,1 */
/** \brief Get boot target using AUTOSAR 4.0 API.
 **
 ** This function gets the currently selected  boot target.
 **
 ** \param[out] target The currently selected boot target.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors (Always).
 **
 ** \ServiceID{19}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType,ECUM_CODE) EcuM_ASR32_GetBootTarget
(
    P2VAR(EcuM_ASR32_BootTargetType, AUTOMATIC, RTE_APPL_DATA) target
);
#endif /* ((ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) ... */

/* !LINKSTO EcuM.EB.ASR32.EcuM101,1 */
/** \brief Select boot target using AUTOSAR 4.0 API.
 **
 ** This function sets the target which is used after reboot.
 **
 ** \param[in] target The selected boot target.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK The new  boot target was not accepted by EcuM.
 **
 ** \ServiceID{18}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType,ECUM_CODE) EcuM_ASR40_SelectBootTarget
(
    EcuM_ASR40_BootTargetType target
);

/* !LINKSTO EcuM.EB.ASR32.EcuM101,1 */
/** \brief Get boot target using AUTOSAR 4.0 API.
 **
 ** This function gets the currently selected  boot target.
 **
 ** \param[out] target The currently selected boot target.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors (Always).
 **
 ** \ServiceID{19}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType,ECUM_CODE) EcuM_ASR40_GetBootTarget
(
    P2VAR(EcuM_ASR40_BootTargetType, AUTOMATIC, RTE_APPL_DATA) target
);

#define ECUM_STOP_SEC_CODE
#include <EcuM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#if (ECUM_USE_BOOTTARGET_SEC == STD_ON)
#define ECUM_START_SEC_ECUM_BOOTTARGET
#else
#define ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#endif
#include <EcuM_MemMap.h>

/* Boot target - internal 64 Bit type */
extern VAR(EcuM_BootTargetInternalType, ECUM_VAR_BOOTTARGET) EcuM_BootTarget;

#if (ECUM_USE_BOOTTARGET_SEC == STD_ON)
#define ECUM_STOP_SEC_ECUM_BOOTTARGET
#else
#define ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#endif
#include <EcuM_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef ECUM_BOOTTARGET_H */

