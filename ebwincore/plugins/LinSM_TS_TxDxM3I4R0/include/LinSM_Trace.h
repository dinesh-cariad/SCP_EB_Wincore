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

#ifndef LINSM_TRACE_H
#define LINSM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <LinSM_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_INIT_ENTRY
/** \brief Entry point of function LinSM_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_INIT_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_INIT_EXIT
/** \brief Exit point of function LinSM_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_INIT_EXIT(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_SCHEDULEREQUEST_ENTRY
/** \brief Entry point of function LinSM_ScheduleRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_SCHEDULEREQUEST_ENTRY(network,schedule)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_SCHEDULEREQUEST_EXIT
/** \brief Exit point of function LinSM_ScheduleRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINSM_SCHEDULEREQUEST_EXIT(Retval,network,schedule)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function LinSM_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_GETVERSIONINFO_EXIT
/** \brief Exit point of function LinSM_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_GETCURRENTCOMMODE_ENTRY
/** \brief Entry point of function LinSM_GetCurrentComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_GETCURRENTCOMMODE_ENTRY(network,mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_GETCURRENTCOMMODE_EXIT
/** \brief Exit point of function LinSM_GetCurrentComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINSM_GETCURRENTCOMMODE_EXIT(Retval,network,mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_REQUESTCOMMODE_ENTRY
/** \brief Entry point of function LinSM_RequestComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_REQUESTCOMMODE_ENTRY(network,mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_REQUESTCOMMODE_EXIT
/** \brief Exit point of function LinSM_RequestComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINSM_REQUESTCOMMODE_EXIT(Retval,network,mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_MAINFUNCTION_ENTRY
/** \brief Entry point of function LinSM_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_MAINFUNCTION_EXIT
/** \brief Exit point of function LinSM_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_SCHEDULEREQUESTCONFIRMATION_ENTRY
/** \brief Entry point of function LinSM_ScheduleRequestConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_SCHEDULEREQUESTCONFIRMATION_ENTRY(network,schedule)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_SCHEDULEREQUESTCONFIRMATION_EXIT
/** \brief Exit point of function LinSM_ScheduleRequestConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_SCHEDULEREQUESTCONFIRMATION_EXIT(network,schedule)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_WAKEUPCONFIRMATION_ENTRY
/** \brief Entry point of function LinSM_WakeupConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_WAKEUPCONFIRMATION_ENTRY(network,success)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_WAKEUPCONFIRMATION_EXIT
/** \brief Exit point of function LinSM_WakeupConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_WAKEUPCONFIRMATION_EXIT(network,success)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_GOTOSLEEPCONFIRMATION_ENTRY
/** \brief Entry point of function LinSM_GotoSleepConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_GOTOSLEEPCONFIRMATION_ENTRY(network,success)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_GOTOSLEEPCONFIRMATION_EXIT
/** \brief Exit point of function LinSM_GotoSleepConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_GOTOSLEEPCONFIRMATION_EXIT(network,success)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_GOTOSLEEPINDICATION_ENTRY
/** \brief Entry point of function LinSM_GotoSleepIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_GOTOSLEEPINDICATION_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINSM_GOTOSLEEPINDICATION_EXIT
/** \brief Exit point of function LinSM_GotoSleepIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINSM_GOTOSLEEPINDICATION_EXIT(Channel)
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

#endif /* ifndef LINSM_TRACE_H */
/*==================[end of file]===========================================*/
