/**
 * \file
 *
 * \brief AUTOSAR PbcfgM
 *
 * This file contains the implementation of the AUTOSAR
 * module PbcfgM.
 *
 * \version 1.2.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef PBCFGM_TYPES_H
#define PBCFGM_TYPES_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>         /* EB specific standard types */
#include <PbcfgM_Cfg.h>        /* Module pre-compile time config */

#if (defined TS_RELOCATABLE_CFG_ENABLE)
#error TS_RELOCATABLE_CFG_ENABLE already defined
#endif
#define TS_RELOCATABLE_CFG_ENABLE PBCFGM_RELOCATABLE_CFG_ENABLE

#if (defined TS_PB_CFG_PTR_CLASS)
#error TS_PB_CFG_PTR_CLASS already defined
#endif
#define TS_PB_CFG_PTR_CLASS PBCFGM_APPL_CONST

#include <TSPBConfig_Types.h> /* Post Build Config Helpers */

/*==================[macros]================================================*/

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined PBCFGM_INSTANCE_ID)
#error PBCFGM_INSTANCE_ID already defined
#endif
/** \brief Id of instance of PbcfgM */
#define PBCFGM_INSTANCE_ID  0U

/*------------------[macros for error codes]--------------------------------*/

#if (defined PBCFGM_E_NULL_POINTER ) /* to prevent double declaration */
#error PBCFGM_E_NULL_POINTER  already defined
#endif
/** \brief Development Error Code
 **
 ** API service called with a NULL pointer.
 */
#define PBCFGM_E_NULL_POINTER   0x11U


#if (defined PBCFGM_E_UNINIT) /* to prevent double declaration */
#error PBCFGM_E_UNINIT already defined
#endif
/** \brief Development Error Code
 **
 ** API service called without module initialization.
 */
#define PBCFGM_E_UNINIT  0x20U

/*------------------[macros for service IDs]--------------------------------*/

#if (defined PBCFGM_SID_INIT)
#error PBCFGM_SID_INIT already defined
#endif
/** \brief Definition of service ID for PbcfgM_Init. */
#define PBCFGM_SID_INIT  0x01U

#if (defined PBCFGM_SID_GET_CONFIG)
#error PBCFGM_SID_GET_CONFIG already defined
#endif
/** \brief Definition of service ID for PbcfgM_GetConfig. */
#define PBCFGM_SID_GET_CONFIG  0x02U


/*==================[type definitions]======================================*/

/** \brief Generalized pointer to any module configuration **/
typedef P2CONST(void, AUTOMATIC, PBCFGM_APPL_CONST) PbcfgM_ModuleConfigPtrType;

/** \brief Function pointer to 'Is Valid Configuration' function **/
typedef FUNC(Std_ReturnType, PBCFGM_APPL_CODE) (PbcfgM_IsValidConfigFuncType)
(
   P2CONST(void, AUTOMATIC, PBCFGM_APPL_CONST) ConfigPtr
);

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#undef TS_RELOCATABLE_CFG_ENABLE
#undef TS_PB_CFG_PTR_CLASS

#endif /* ifndef PBCFGM_TYPES_H */
/*==================[end of file]===========================================*/
