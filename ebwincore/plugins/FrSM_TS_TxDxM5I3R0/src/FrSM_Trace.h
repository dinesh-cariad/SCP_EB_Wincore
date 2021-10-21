/**
 * \file
 *
 * \brief AUTOSAR FrSM
 *
 * This file contains the implementation of the AUTOSAR
 * module FrSM.
 *
 * \version 5.3.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRSM_TRACE_H
#define FRSM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif

#include <FrSM_guards.h> /* After External/API/Cfg, before Trace/internal/private */
#include <FrSM_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRSM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function FrSM_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRSM_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRSM_GETVERSIONINFO_EXIT
/** \brief Exit point of function FrSM_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRSM_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRSM_INIT_ENTRY
/** \brief Entry point of function FrSM_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRSM_INIT_ENTRY(FrSM_ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRSM_INIT_EXIT
/** \brief Exit point of function FrSM_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRSM_INIT_EXIT(FrSM_ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRSM_REQUESTCOMMODE_ENTRY
/** \brief Entry point of function FrSM_RequestComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRSM_REQUESTCOMMODE_ENTRY(NetworkHandle,ComM_Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRSM_REQUESTCOMMODE_EXIT
/** \brief Exit point of function FrSM_RequestComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRSM_REQUESTCOMMODE_EXIT(Retval,NetworkHandle,ComM_Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRSM_GETCURRENTCOMMODE_ENTRY
/** \brief Entry point of function FrSM_GetCurrentComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRSM_GETCURRENTCOMMODE_ENTRY(NetworkHandle,ComM_ModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRSM_GETCURRENTCOMMODE_EXIT
/** \brief Exit point of function FrSM_GetCurrentComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRSM_GETCURRENTCOMMODE_EXIT(Retval,NetworkHandle,ComM_ModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRSM_SETECUPASSIVE_ENTRY
/** \brief Entry point of function FrSM_SetEcuPassive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRSM_SETECUPASSIVE_ENTRY(FrSM_Passive)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRSM_SETECUPASSIVE_EXIT
/** \brief Exit point of function FrSM_SetEcuPassive()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRSM_SETECUPASSIVE_EXIT(Retval,FrSM_Passive)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRSM_ALLSLOTS_ENTRY
/** \brief Entry point of function FrSM_AllSlots()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRSM_ALLSLOTS_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRSM_ALLSLOTS_EXIT
/** \brief Exit point of function FrSM_AllSlots()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRSM_ALLSLOTS_EXIT(Retval,NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRSM_MAINFUNCTION_ENTRY
/** \brief Entry point of function FrSM_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRSM_MAINFUNCTION_ENTRY(FrSM_ClstIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRSM_MAINFUNCTION_EXIT
/** \brief Exit point of function FrSM_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRSM_MAINFUNCTION_EXIT(FrSM_ClstIdx)
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

#endif /* ifndef FRSM_TRACE_H */
/*==================[end of file]===========================================*/
