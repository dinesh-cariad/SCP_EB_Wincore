/**
 * \file
 *
 * \brief AUTOSAR Sd
 *
 * This file contains the implementation of the AUTOSAR
 * module Sd.
 *
 * \version 1.4.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef SD_TRACE_H
#define SD_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <Sd_Trace_Internal.h>

#include <Sd_Cfg.h>             /* module static configuration */

#if (SD_DEV_ERROR_DETECT == STD_ON)




/*------------------------[Defensive programming]----------------------------*/

#if (defined SD_PRECONDITION_ASSERT)
#error SD_PRECONDITION_ASSERT is already defined
#endif

#if (defined SD_PRECONDITION_ASSERT_NO_EVAL)
#error SD_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (SD_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define SD_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), SD_MODULE_ID, SD_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define SD_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), SD_MODULE_ID, SD_INSTANCE_ID, (ApiId))
#else
#define SD_PRECONDITION_ASSERT(Condition, ApiId)
#define SD_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined SD_POSTCONDITION_ASSERT)
#error SD_POSTCONDITION_ASSERT is already defined
#endif

#if (defined SD_POSTCONDITION_ASSERT_NO_EVAL)
#error SD_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (SD_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define SD_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), SD_MODULE_ID, SD_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define SD_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), SD_MODULE_ID, SD_INSTANCE_ID, (ApiId))
#else
#define SD_POSTCONDITION_ASSERT(Condition, ApiId)
#define SD_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined SD_INVARIANT_ASSERT)
#error SD_INVARIANT_ASSERT is already defined
#endif

#if (defined SD_INVARIANT_ASSERT_NO_EVAL)
#error SD_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (SD_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define SD_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), SD_MODULE_ID, SD_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define SD_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), SD_MODULE_ID, SD_INSTANCE_ID, (ApiId))
#else
#define SD_INVARIANT_ASSERT(Condition, ApiId)
#define SD_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined SD_STATIC_ASSERT)
# error SD_STATIC_ASSERT is already defined
#endif
#if (SD_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define SD_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define SD_STATIC_ASSERT(expr)
#endif

#if (defined SD_UNREACHABLE_CODE_ASSERT)
#error SD_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (SD_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define SD_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(SD_MODULE_ID, SD_INSTANCE_ID, (ApiId))
#else
#define SD_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined SD_INTERNAL_API_ID)
#error SD_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define SD_INTERNAL_API_ID DET_INTERNAL_API_ID


#endif /* SD_DEV_ERROR_DETECT == STD_ON */

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Sd_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_GETVERSIONINFO_EXIT
/** \brief Exit point of function Sd_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function Sd_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_ISVALIDCONFIG_ENTRY(voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_ISVALIDCONFIG_EXIT
/** \brief Exit point of function Sd_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SD_ISVALIDCONFIG_EXIT(Retval,voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_INIT_ENTRY
/** \brief Entry point of function Sd_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_INIT_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_INIT_EXIT
/** \brief Exit point of function Sd_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_INIT_EXIT(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_SERVERSERVICESETSTATE_ENTRY
/** \brief Entry point of function Sd_ServerServiceSetState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_SERVERSERVICESETSTATE_ENTRY(ServiceServiceHandleId,ServerServiceState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_SERVERSERVICESETSTATE_EXIT
/** \brief Exit point of function Sd_ServerServiceSetState()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SD_SERVERSERVICESETSTATE_EXIT(Retval,ServiceServiceHandleId,ServerServiceState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_CLIENTSERVICESETSTATE_ENTRY
/** \brief Entry point of function Sd_ClientServiceSetState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_CLIENTSERVICESETSTATE_ENTRY(ClientServiceHandleId,ClientServiceState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_CLIENTSERVICESETSTATE_EXIT
/** \brief Exit point of function Sd_ClientServiceSetState()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SD_CLIENTSERVICESETSTATE_EXIT(Retval,ClientServiceHandleId,ClientServiceState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_CONSUMEDEVENTGROUPSETSTATE_ENTRY
/** \brief Entry point of function Sd_ConsumedEventGroupSetState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_CONSUMEDEVENTGROUPSETSTATE_ENTRY(ConsumedEventGroupHandleId,ConsumedEventGroupState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_CONSUMEDEVENTGROUPSETSTATE_EXIT
/** \brief Exit point of function Sd_ConsumedEventGroupSetState()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SD_CONSUMEDEVENTGROUPSETSTATE_EXIT(Retval,ConsumedEventGroupHandleId,ConsumedEventGroupState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_MAINFUNCTION_ENTRY
/** \brief Entry point of function Sd_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_MAINFUNCTION_EXIT
/** \brief Exit point of function Sd_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_STARTRANDOM_ENTRY
/** \brief Entry point of function Sd_StartRandom()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_STARTRANDOM_ENTRY(Seed)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_STARTRANDOM_EXIT
/** \brief Exit point of function Sd_StartRandom()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_STARTRANDOM_EXIT(Seed)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_RXINDICATION_ENTRY
/** \brief Entry point of function Sd_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_RXINDICATION_ENTRY(RxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_RXINDICATION_EXIT
/** \brief Exit point of function Sd_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_RXINDICATION_EXIT(RxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_LOCALIPADDRASSIGNMENTCHG_ENTRY
/** \brief Entry point of function Sd_LocalIpAddrAssignmentChg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_LOCALIPADDRASSIGNMENTCHG_ENTRY(SoConId,State)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SD_LOCALIPADDRASSIGNMENTCHG_EXIT
/** \brief Exit point of function Sd_LocalIpAddrAssignmentChg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SD_LOCALIPADDRASSIGNMENTCHG_EXIT(SoConId,State)
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

#endif /* ifndef SD_TRACE_H */
/*==================[end of file]===========================================*/
