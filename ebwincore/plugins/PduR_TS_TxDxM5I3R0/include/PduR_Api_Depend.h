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
#ifndef PDUR_API_DEPEND_H
#define PDUR_API_DEPEND_H
/*==================[inclusions]============================================*/

#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* AUTOSAR Communication Stack types */
#include <PduR_Cfg.h>          /* Generated header of  PDU Router */
#include <PduR_Types.h>        /* types of PduR module */

/* Include zero cost header file if fully handled by zero cost operation.
   This comprises also cases like configuring solely communication interface modules or
   solely transport protocol modules. */
#if (PDUR_ZERO_COST_OPERATION_FULL == STD_ON)
#include <PduR_Zc_Cfg.h>
#else /* (PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) */
/*==================[macros]================================================*/

/* Use only interrupt locks in single core use case */
#if (PDUR_MULTICORE_SUPPORT == STD_OFF)

#define SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_1() \
  SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0()

#define SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_1() \
  SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0()

#define SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_2() \
  SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0()

#define SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_2() \
  SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0()

#endif /* (PDUR_MULTICORE_SUPPORT == STD_OFF) */

/*------------------[PDU Router 'Post Build Config']------------------------*/
#if (PDUR_CONST_CFG_ADDRESS_ENABLE == STD_ON)

#if (defined PduR_GConfigPtr)           /* To prevent double declaration */
#error PduR_GConfigPtr already defined
#endif /* if (defined PduR_GConfigPtr) */
/** \brief define PduR_GConfigPtr
 * root config pointer is replaced by absolute address that is given via
 * pre-compile-time switch PDUR_CONST_CONFIG_ADDRESS
 */
#define PduR_GConfigPtr \
    ((P2CONST(PduR_PBConfigType, PDUR_VAR_CLEARED, PDUR_APPL_CONST)) \
    PDUR_CONST_CFG_ADDRESS)

#endif /* PDUR_CONST_CFG_ADDRESS_ENABLE */


#if (defined PDUR_INVALID_CONFIGURATION_ID)           /* To prevent double declaration */
#error PDUR_INVALID_CONFIGURATION_ID already defined
#endif /* if (defined PDUR_INVALID_CONFIGURATION_ID) */
/** \brief Macro definition for the invalid configuration Id returned by
 * PduR_GetConfigurationId if the PduR is not initialized.
 */
#define PDUR_INVALID_CONFIGURATION_ID          0xFFFFU

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define PDUR_START_SEC_CODE
#include <PduR_MemMap.h>

/** \brief Get version information.
 **
 ** This service returns the version information of this module.
 **
 ** \param[out]  versionInfo  Pointer to where to store the version
 **                           information of this module (Module Id,
 **                           Vendor Id, Instance Id, Vendor specific version numbers)
 **
 ** \ServiceID{0x02}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
#if (PDUR_VERSION_INFO_API == STD_ON)
extern FUNC(void,PDUR_CODE) PduR_GetVersionInfo
(
   P2VAR(Std_VersionInfoType,AUTOMATIC,PDUR_APPL_DATA) versionInfo
);
#elif (PDUR_VERSION_INFO_API == STD_OFF)
/*
 * PduR_GetVersionInfo() API function is disabled -
 * provide macro implementation
 */
#if (defined PduR_GetVersionInfo)           /* To prevent double declaration */
#error PduR_GetVersionInfo already defined
#endif /* if (defined PduR_GetVersionInfo) */
#define PduR_GetVersionInfo(versionInfo) \
   do \
   { \
      /* get version info of PduR module */ \
      (versionInfo)->vendorID = PDUR_VENDOR_ID; \
      (versionInfo)->moduleID = PDUR_MODULE_ID; \
      (versionInfo)->sw_major_version = PDUR_SW_MAJOR_VERSION; \
      (versionInfo)->sw_minor_version = PDUR_SW_MINOR_VERSION; \
      (versionInfo)->sw_patch_version = PDUR_SW_PATCH_VERSION; \
   } while(0)
#endif /*  #if (PDUR_VERSION_INFO_API == STD_ON) */


/** \brief PduR_Init - Initializes the PDU Router.
 ** Function to initialize the PduR module. First function to be called of PduR.
 ** The module calling the function PduR_Init has to include PduR_PBcfg.h. The
 ** invocation of the PduR_Init function is
 ** PduR_Init(&(PDUR_CONFIG_NAME.PduR_RootConfig));
 **
 ** \param[in] ConfigPtr Pointer to post build configuration.
 **
 ** \ServiceID{0x01}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, PDUR_CODE) PduR_Init
(
   P2CONST(PduR_PBConfigType, AUTOMATIC, PDUR_APPL_CONST) ConfigPtr
);


/** \brief Get configuration ID
 **
 ** Returns the unique identifier of the post-build time configuration of the PDU Router.
 **
 ** \return Identifier of the post-build time configuration.
 **         For enabled DET the invalid configuration Id 'PDUR_INVALID_CONFIGURATION_ID'
 **         is returned.
 **
 ** \ServiceID{0x10}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(PduR_PBConfigIdType, PDUR_CODE) PduR_GetConfigurationId
(
   void
);


/** \brief Validate configuration
 **
 ** Checks if the post build configuration is valid.
 ** A configuration is invalid if
 ** - the platform signature does not match.
 ** - the published information signature does not match.
 ** - the link time signature does not match.
 ** - the compile time signature does not match.
 ** - the function is called with a null pointer.
 **
 ** \param[in] ConfigPtr Pointer to configuration structure that holds the
 **                      PduR module post-build-time configuration data.
 **
 ** \return Function execution success status
 ** \retval E_OK  the provided module configuration is valid
 ** \retval E_NOT_OK  the provided module configuration is invalid
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/

#if (PDUR_ISVALIDCONFIG_MMAP_ENABLED == STD_ON)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#define PDUR_START_SEC_CODE_CC_BLOCK
#include <PduR_MemMap.h>
#endif   /* if (PDUR_ISVALIDCONFIG_MMAP_ENABLED == STD_ON) */

extern FUNC(Std_ReturnType, PDUR_CODE) PduR_IsValidConfig
(
    P2CONST(void, AUTOMATIC, PDUR_APPL_CONST) ConfigPtr
);

#if (PDUR_ISVALIDCONFIG_MMAP_ENABLED == STD_ON)
#define PDUR_STOP_SEC_CODE_CC_BLOCK
#include <PduR_MemMap.h>
#define PDUR_START_SEC_CODE
#include <PduR_MemMap.h>
#endif   /* if (PDUR_ISVALIDCONFIG_MMAP_ENABLED == STD_ON) */

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
/**
 ** \brief AUTOSAR function to enable a routing path group
 **
 **  Generic PduR_EnableRouting function to enable a routing path group.
 **
 ** \param[in]  GroupId             Routing path group Id
 **
 ** \return void
 **
 **/
extern FUNC(void, PDUR_CODE) PduR_EnableRouting
(
   PduR_RoutingPathGroupIdType GroupId
);

/**
 ** \brief AUTOSAR function to disable a routing path group
 **
 ** Generic PduR_DisableRouting function to disable a routing path group.
 **
 ** \param[in]  GroupId             Routing path group Id
 **
 ** \return void
 **
 **/
extern FUNC(void, PDUR_CODE) PduR_DisableRouting
(
   PduR_RoutingPathGroupIdType GroupId
);
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */

#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/
#if (PDUR_CONST_CFG_ADDRESS_ENABLE == STD_OFF)
/*
 * if not a constant configuration address is used - define pointer variable
 */
#define PDUR_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <PduR_MemMap.h>

/** \brief PduR_GConfigPtr
 * global variable for the pointer to the config of PduR
 */
extern P2CONST(PduR_PBConfigType, PDUR_VAR_CLEARED, PDUR_APPL_CONST) PduR_GConfigPtr;


#define PDUR_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <PduR_MemMap.h>

#endif /* PDUR_CONST_CFG_ADDRESS_ENABLE == STD_OFF */
/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_ZERO_COST_OPERATION_FULL == STD_ON) */
#endif /* ifndef PDUR_API_DEPEND_H */
/*==================[end of file]===========================================*/
