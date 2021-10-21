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

#ifndef PBCFGM_TRACE_H
#define PBCFGM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <PbcfgM_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_PBCFGM_INIT_ENTRY
/** \brief Entry point of function PbcfgM_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_PBCFGM_INIT_ENTRY(PbcfgM_ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_PBCFGM_INIT_EXIT
/** \brief Exit point of function PbcfgM_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_PBCFGM_INIT_EXIT(PbcfgM_ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_PBCFGM_GETCONFIG_ENTRY
/** \brief Entry point of function PbcfgM_GetConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_PBCFGM_GETCONFIG_ENTRY(PbcfgM_ModuleId,PbcfgM_InstanceId,PbcfgM_ModuleConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_PBCFGM_GETCONFIG_EXIT
/** \brief Exit point of function PbcfgM_GetConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_PBCFGM_GETCONFIG_EXIT(Retval,PbcfgM_ModuleId,PbcfgM_InstanceId,PbcfgM_ModuleConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_PBCFGM_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function PbcfgM_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_PBCFGM_ISVALIDCONFIG_ENTRY(PbcfgM_ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_PBCFGM_ISVALIDCONFIG_EXIT
/** \brief Exit point of function PbcfgM_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_PBCFGM_ISVALIDCONFIG_EXIT(Retval,PbcfgM_ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef PBCFGM_TRACE_H */
/*==================[end of file]===========================================*/

