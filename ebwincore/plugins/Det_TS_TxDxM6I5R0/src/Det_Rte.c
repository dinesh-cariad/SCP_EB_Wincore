/**
 * \file
 *
 * \brief AUTOSAR Det
 *
 * This file contains the implementation of the AUTOSAR
 * module Det.
 *
 * \version 6.5.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=================================================================*/

#include <Det_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */

/* to prevent duplicate declarations of symbols from Rte_Det.h by our public
 * headers */

#if (defined DET_INTERNAL_USAGE)
#error DET_INTERNAL_USAGE is already defined
#endif

#define DET_INTERNAL_USAGE
#include <Det_BSW.h>                                              /* Declaration of Det BSW APIs */
#include <Det_Int.h>                                           /* internal config dependent file */

/* An exclusive area is only needed for internal logging within
 * the single-core implementation of the Det module */
#if (DET_MULTICORE_ENABLED == FALSE) && (DET_LOGMODE == DET_LOGMODE_INTERNAL)
# include <SchM_Det.h>          /* for the locking of the exclusive areas */
#endif

#if (DET_RTE_USAGE == STD_ON)
#if (DET_DEFAULT_ASR_SERVICE_API != DET_SERVICE_API_NONE)
# include <Rte_DevelopmentErrorTracer.h>
#endif
#endif


/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[internal function definitions]==============================================*/


#if (DET_MULTICORE_ENABLED == FALSE) && (DET_LOGMODE == DET_LOGMODE_INTERNAL)

#define DET_START_SEC_CODE
#include <Det_MemMap.h>

FUNC(void, DET_CODE) Det_Enter_SCHM_DET_EXCLUSIVE_AREA_0 (void)
{
      DBG_DET_ENTER_SCHM_DET_EXCLUSIVE_AREA_0_ENTRY();

      /* lock access to DET global variables */
      SchM_Enter_Det_SCHM_DET_EXCLUSIVE_AREA_0();

      DBG_DET_ENTER_SCHM_DET_EXCLUSIVE_AREA_0_EXIT();
}

FUNC(void, DET_CODE) Det_Exit_SCHM_DET_EXCLUSIVE_AREA_0 (void)
{
      DBG_DET_EXIT_SCHM_DET_EXCLUSIVE_AREA_0_ENTRY();

      /* lock access to DET global variables */
      SchM_Exit_Det_SCHM_DET_EXCLUSIVE_AREA_0();

      DBG_DET_EXIT_SCHM_DET_EXCLUSIVE_AREA_0_EXIT();
}

#define DET_STOP_SEC_CODE
#include <Det_MemMap.h>

#endif

/*==================[external function definitions]==============================================*/

#if (DET_RTE_USAGE == STD_ON)

#define DevelopmentErrorTracer_START_SEC_CODE
/* !LINKSTO Det.EB.req.swc_include,1 */
#include <DevelopmentErrorTracer_MemMap.h>

#if (DET_DEFAULT_ASR_SERVICE_API == DET_SERVICE_API_ASR32)
FUNC(void, DET_CODE) Det_Rte_ReportError(
   uint16 pdav0, /* Port defined argument for ModuleId */
   uint8  InstanceId,
   uint8  ApiId,
   uint8  ErrorId)
{
  DBG_DET_RTE_REPORTERROR_ENTRY(pdav0,InstanceId,ApiId,ErrorId);

  (void) Det_ASR32_ReportError(pdav0, InstanceId, ApiId, ErrorId);

  DBG_DET_RTE_REPORTERROR_EXIT(pdav0,InstanceId,ApiId,ErrorId);
}

#elif (DET_DEFAULT_ASR_SERVICE_API == DET_SERVICE_API_ASR40)
FUNC(Std_ReturnType, DET_CODE) Det_Rte_ReportError(
   uint16 pdav0, /* Port defined argument for ModuleId */
   uint8  InstanceId,
   uint8  ApiId,
   uint8  ErrorId)
{
  return Det_ASR40_ReportError(pdav0, InstanceId, ApiId, ErrorId);
}

#elif (DET_DEFAULT_ASR_SERVICE_API == DET_SERVICE_API_ASR43)
FUNC(Std_ReturnType, DET_CODE) Det_Rte_ReportError(
   uint16 pdav0, /* Port defined argument for ModuleId */
   uint8  pdav1, /* Port defined argument for InstanceId */
   uint8  ApiId,
   uint8  ErrorId)
{
  return Det_ASR40_ReportError(pdav0, pdav1, ApiId, ErrorId);
}

FUNC(Std_ReturnType, DET_CODE) Det_Rte_ReportRuntimeError(
   uint16 pdav0, /* Port defined argument for ModuleId */
   uint8  pdav1, /* Port defined argument for InstanceId */
   uint8  ApiId,
   uint8  ErrorId)
{
  return Det_ASR43_ReportRuntimeError(pdav0, pdav1, ApiId, ErrorId);
}
#endif /* (DET_DEFAULT_ASR_SERVICE_API == DET_SERVICE_API_ASR43) */

#define DevelopmentErrorTracer_STOP_SEC_CODE
/* !LINKSTO Det.EB.req.swc_include,1 */
#include <DevelopmentErrorTracer_MemMap.h>

#endif /* (DET_RTE_USAGE == STD_ON) */

/*==================[end of file]================================================================*/
