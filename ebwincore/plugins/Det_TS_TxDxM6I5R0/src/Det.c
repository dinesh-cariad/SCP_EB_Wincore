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

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 21.6 (Required)
 *    "The Standard Library input/output functions shall not be used"
 *
 *    Reason:
 *    The standard library function fprintf() is used for debugging purposes only.
 */

/*==================[inclusions]============================================*/

#include <Det.h>
#include <Det_Trace.h>

#if (defined DET_INTERNAL_USAGE)
#error DET_INTERNAL_USAGE is already defined
#endif

/* to prevent redeclarations of symbols from Rte_DevelopmentErrorTracer.h by
 * our public headers */
#define DET_INTERNAL_USAGE

#include <Det_Int.h>            /* internal config dependent file */
#include <Det_Int_Cfg.h>        /* Det generated internal configuration */

/* Rte and SchM includes are handled by the Rte specific comilation
 * units of Det */

#if ((DET_DEFENSIVE_PROGRAMMING_ENABLED == STD_ON)      \
     && (TS_ARCH_FAMILY == TS_WINDOWS))
#include <stdio.h>             /* for fprintf */
#endif

/*==================[macros]================================================*/

/* !LINKSTO DET036,1 */
/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef DET_VENDOR_ID /* configuration check */
#error DET_VENDOR_ID must be defined
#endif

#if (DET_VENDOR_ID != 1U) /* vendor check */
#error DET_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef DET_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error DET_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (DET_AR_RELEASE_MAJOR_VERSION != 4U)
#error DET_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef DET_AR_RELEASE_MINOR_VERSION /* configuration check */
#error DET_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (DET_AR_RELEASE_MINOR_VERSION != 0U )
#error DET_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef DET_AR_RELEASE_REVISION_VERSION /* configuration check */
#error DET_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (DET_AR_RELEASE_REVISION_VERSION != 3U )
#error DET_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef DET_SW_MAJOR_VERSION /* configuration check */
#error DET_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (DET_SW_MAJOR_VERSION != 6U)
#error DET_SW_MAJOR_VERSION wrong (!= 6U)
#endif

#ifndef DET_SW_MINOR_VERSION /* configuration check */
#error DET_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (DET_SW_MINOR_VERSION < 5U)
#error DET_SW_MINOR_VERSION wrong (< 5U)
#endif

#ifndef DET_SW_PATCH_VERSION /* configuration check */
#error DET_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (DET_SW_PATCH_VERSION < 6U)
#error DET_SW_PATCH_VERSION wrong (< 6U)
#endif

/*------------------[check the log mod configuration]-----------------------*/

/* check if value of DET_LOGMODE macro is valid */
#if !((DET_LOGMODE == DET_LOGMODE_INTERNAL)         \
      || (DET_LOGMODE == DET_LOGMODE_BREAKPOINT))
# error Det configuration error: invalid value for DET_LOGMODE
#endif

/*------------------[macros for state variable values]----------------------*/

#if (defined DET_STATE_UNINIT)
#error DET_STATE_UNINIT is already defined
#endif

/** \brief Det has not yet been initialized */
#define DET_STATE_UNINIT      0U

#if (defined DET_STATE_NOT_STARTED)
#error DET_STATE_NOT_STARTED is already defined
#endif

/** \brief Det is initialized but has not been started yet */
#define DET_STATE_NOT_STARTED 1U

#if (defined DET_STATE_IDLE)
#error DET_STATE_IDLE is already defined
#endif

/** \brief Det is idle and ready to send an error if necessary */
#define DET_STATE_IDLE        2U

#if (defined DET_STATE_SENDING)
#error DET_STATE_SENDING is already defined
#endif

/** \brief Det is sending an error and is waiting for the confirmation */
#define DET_STATE_SENDING     3U

/*==================[type definitions]======================================*/

/** \brief Type used for the DET internal state variable
 *
 * Allowed values:
 * - ::DET_STATE_UNINIT
 * - ::DET_STATE_NOT_STARTED
 * - ::DET_STATE_IDLE
 * - ::DET_STATE_SENDING
 */
typedef uint8 Det_StateType;

/** \brief Type for internal logging structure */
typedef struct
{
   uint16 ModuleId;             /**< Module ID of the calling module */
   uint8 InstanceId;           /**< Instance Id of the calling module */
   uint8 ApiId;                /**< ID of API in which error is detected */
   uint8 ErrorId;              /**< ID of detected development error */
} Det_ErrorBufferType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define DET_START_SEC_CODE
#include <Det_MemMap.h>

/** \brief Report errors.
 **
 ** This function reports errors. The error type is selected using the ErrorType
 ** parameter. This function is called from modules XXX for both development and runtime errors
 ** in case of activated preprocessor switch XXX_DEV_ERROR_DETECT. Production relevant errors
 ** shall be reported to the Diagnostics Event Manager (DEM). The type of reported errors depends
 ** on the ErrorType param[in].
 **
 ** \param[in] ErrorType The type of error to report.
 ** \param[in] ModuleId Module ID of calling module.
 ** \param[in] InstanceId Instance ID of calling module.
 ** \param[in] ApiId ID of API service in which error is detected.
 ** \param[in] ErrorId ID of detected runtime error.
 ** \return    See declaration in Det_BSW.h for each function
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(Std_ReturnType, DET_CODE) Det_ReportErrorStatus(
  uint8 ErrorType,
  uint16 ModuleId,
  uint8 InstanceId,
  uint8 ApiId,
  uint8 ErrorId
  );

#define DET_STOP_SEC_CODE
#include <Det_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#if (DET_LOGMODE == DET_LOGMODE_INTERNAL)

/* see declarations in Det.h */
#if (DET_MULTICORE_ENABLED == FALSE)
#define DET_START_SEC_VAR_CLEARED_16

/* !LINKSTO Det.EB.req.bsw_include,1 */
#include <Det_MemMap.h>
VAR(uint16, DET_VAR_CLEARED)              Det_WriteIndex_DevelopmentError;
VAR(uint16, DET_VAR_CLEARED)              Det_WriteIndex_RuntimeError;
VAR(uint16, DET_VAR_CLEARED)              Det_WriteIndex_TransientFault;
VAR(uint16, DET_VAR_CLEARED)              Det_UsedSlots_DevelopmentError;
VAR(uint16, DET_VAR_CLEARED)              Det_UsedSlots_RuntimeError;
VAR(uint16, DET_VAR_CLEARED)              Det_UsedSlots_TransientFault;
VAR(uint16, DET_VAR_CLEARED)              Det_ErrorLost_DevelopmentError;
VAR(uint16, DET_VAR_CLEARED)              Det_ErrorLost_RuntimeError;
VAR(uint16, DET_VAR_CLEARED)              Det_FaultLost_TransientFault;

#define DET_STOP_SEC_VAR_CLEARED_16
/* !LINKSTO Det.EB.req.bsw_include,1 */
#include <Det_MemMap.h>

#endif /* DET_MULTICORE_ENABLED == FALSE */
#define DET_START_SEC_VAR_CLEARED_UNSPECIFIED
/* !LINKSTO Det.EB.req.bsw_include,1 */
#include <Det_MemMap.h>

VAR(Det_ErrorBufferType, DET_VAR_CLEARED) Det_ErrorBuffer[DET_NR_OF_ERROR_TYPES][DET_BUFFERSIZE];

#if (DET_MULTICORE_ENABLED == TRUE)

VAR(Atomic_t, DET_VAR_CLEARED)              Det_WriteIndex_DevelopmentError;
VAR(Atomic_t, DET_VAR_CLEARED)              Det_WriteIndex_RuntimeError;
VAR(Atomic_t, DET_VAR_CLEARED)              Det_WriteIndex_TransientFault;
VAR(Atomic_t, DET_VAR_CLEARED)              Det_UsedSlots_DevelopmentError;
VAR(Atomic_t, DET_VAR_CLEARED)              Det_UsedSlots_RuntimeError;
VAR(Atomic_t, DET_VAR_CLEARED)              Det_UsedSlots_TransientFault;
VAR(Atomic_t, DET_VAR_CLEARED)              Det_ErrorLost_DevelopmentError;
VAR(Atomic_t, DET_VAR_CLEARED)              Det_ErrorLost_RuntimeError;
VAR(Atomic_t, DET_VAR_CLEARED)              Det_FaultLost_TransientFault;

#endif /* DET_MULTICORE_ENABLED == TRUE */

#define DET_STOP_SEC_VAR_CLEARED_UNSPECIFIED
/* !LINKSTO Det.EB.req.bsw_include,1 */
#include <Det_MemMap.h>

#endif

/*==================[internal data]=========================================*/

#define DET_START_SEC_VAR_INIT_8
/* !LINKSTO Det.EB.req.bsw_include,1 */
#include <Det_MemMap.h>

/** \brief State of the Det
 *
 * Value is ::DET_STATE_UNINIT until Det is initialized. When the state is
 * uninitialized the error reports are \e not stored internally.
 */
STATIC VAR(Det_StateType, DET_VAR) Det_State = DET_STATE_UNINIT;

#define DET_STOP_SEC_VAR_INIT_8
/* !LINKSTO Det.EB.req.bsw_include,1 */
#include <Det_MemMap.h>

/*==================[external function definitions]=========================*/

#define DET_START_SEC_CODE
/* !LINKSTO Det.EB.req.bsw_include,1 */
#include <Det_MemMap.h>

/* Service for initialization of the Development Error Tracer, see declaration
 * in Det.h */
/* !LINKSTO DET008,1 */
FUNC(void, DET_CODE) Det_Init(void)
{
#if (DET_LOGMODE == DET_LOGMODE_INTERNAL)
  /* Variable used to initialize the internal buffer
   * Det_ErrorBuffer[Det_ErrorType][Det_ErrorNumber] is the next field to be overwritten
   * Error type: DevelopmentError / RuntimeError / TransientFault */
  uint16 Det_ErrorNumber;
  uint8 Det_ErrorType;
#endif

  DBG_DET_INIT_ENTRY();

#if (DET_LOGMODE == DET_LOGMODE_INTERNAL)

  /* initialize all global variables */
  for(Det_ErrorType = 0U; Det_ErrorType < DET_NR_OF_ERROR_TYPES; ++Det_ErrorType)
  {
    for (Det_ErrorNumber = 0U; Det_ErrorNumber < DET_BUFFERSIZE; ++Det_ErrorNumber)
    {
      Det_ErrorBuffer[Det_ErrorType][Det_ErrorNumber].ModuleId   = 0U;
      Det_ErrorBuffer[Det_ErrorType][Det_ErrorNumber].InstanceId = 0U;
      Det_ErrorBuffer[Det_ErrorType][Det_ErrorNumber].ApiId      = 0U;
      Det_ErrorBuffer[Det_ErrorType][Det_ErrorNumber].ErrorId    = 0U;
    }
  }

#if (DET_MULTICORE_ENABLED == FALSE)
  Det_WriteIndex_DevelopmentError = 0U;
  Det_WriteIndex_RuntimeError     = 0U;
  Det_WriteIndex_TransientFault   = 0U;
  Det_ErrorLost_DevelopmentError  = 0U;
  Det_ErrorLost_RuntimeError      = 0U;
  Det_FaultLost_TransientFault    = 0U;
  Det_UsedSlots_DevelopmentError  = 0U;
  Det_UsedSlots_RuntimeError      = 0U;
  Det_UsedSlots_TransientFault    = 0U;
#else
/* !LINKSTO Det.EB.req.multicore.AtomicVarInit,1 */
  DET_ATOMIC_INIT(Det_WriteIndex_DevelopmentError,  0U);
  DET_ATOMIC_INIT(Det_WriteIndex_RuntimeError,      0U);
  DET_ATOMIC_INIT(Det_WriteIndex_TransientFault,    0U);
  DET_ATOMIC_INIT(Det_ErrorLost_DevelopmentError,   0U);
  DET_ATOMIC_INIT(Det_ErrorLost_RuntimeError,       0U);
  DET_ATOMIC_INIT(Det_FaultLost_TransientFault,     0U);
  DET_ATOMIC_INIT(Det_UsedSlots_DevelopmentError,   0U);
  DET_ATOMIC_INIT(Det_UsedSlots_RuntimeError,       0U);
  DET_ATOMIC_INIT(Det_UsedSlots_TransientFault,     0U);
#endif /* DET_MULTICORE_ENABLED == FALSE */

#else /* DET_LOGMODE == DET_LOGMODE_BREAKPOINT */

  /* nothing to initialize for breakpoint log mode */

#endif /* DET_LOGMODE switch */

  /* state change: UNINIT -> NOT_STARTED */
  Det_State = DET_STATE_NOT_STARTED;

  DBG_DET_INIT_EXIT();
}

/* Service to start the Development Error Tracer, see declaration in Det.h */
FUNC(void, DET_CODE) Det_Start(void)
{
  DBG_DET_START_ENTRY();

  if (Det_State == DET_STATE_NOT_STARTED)
  {
    /* for internal logging and breakpoint mode: NOT_STARTED -> IDLE */
    Det_State = DET_STATE_IDLE;
  }

  DBG_DET_START_EXIT();
}

#if (DET_MULTICORE_ENABLED == FALSE)
STATIC FUNC(Std_ReturnType, DET_CODE) Det_ReportErrorStatus(
  uint8 ErrorType,
  uint16 ModuleId,
  uint8 InstanceId,
  uint8 ApiId,
  uint8 ErrorId
  )
{
  Std_ReturnType transientFaultRetVal = E_OK;
#if (DET_LOGMODE == DET_LOGMODE_INTERNAL)
  P2VAR(uint16, AUTOMATIC, DET_APPL_DATA) Det_WriteIndex = NULL_PTR;
  P2VAR(uint16, AUTOMATIC, DET_APPL_DATA) Det_UsedSlots  = NULL_PTR;
  P2VAR(uint16, AUTOMATIC, DET_APPL_DATA) Det_ErrorLost  = NULL_PTR;
#endif

#if (DET_LOGMODE == DET_LOGMODE_BREAKPOINT)
  TS_PARAM_UNUSED(ErrorType);
  TS_PARAM_UNUSED(ModuleId);
  TS_PARAM_UNUSED(InstanceId);
  TS_PARAM_UNUSED(ApiId);
  TS_PARAM_UNUSED(ErrorId);
#endif

  if (Det_State != DET_STATE_UNINIT)
  {
    switch(ErrorType)
    {
      case DET_DEVELOPMENT_ERROR_TYPE:
      {
        /* call the notification functions which are configured */
        /* !LINKSTO EB_Ref.SWS_Det_00035.ReportError,1 */
        DET_NOTIFICATIONS_DEVELOPMENT_ERROR(ModuleId, InstanceId, ApiId, ErrorId);
#if (DET_LOGMODE == DET_LOGMODE_INTERNAL)
        Det_WriteIndex = &Det_WriteIndex_DevelopmentError;
        Det_UsedSlots  = &Det_UsedSlots_DevelopmentError;
        Det_ErrorLost  = &Det_ErrorLost_DevelopmentError;
#endif
        break;
      }
      case DET_RUNTIME_ERROR_TYPE:
      {
        /* call the notification functions which are configured */
        /* !LINKSTO EB_Ref.SWS_Det_00035.ReportRuntimeError,1 */
        /* !LINKSTO SWS_Det_00503,1 */
        DET_NOTIFICATIONS_RUNTIME_ERROR(ModuleId, InstanceId, ApiId, ErrorId);
#if (DET_LOGMODE == DET_LOGMODE_INTERNAL)
        Det_WriteIndex = &Det_WriteIndex_RuntimeError;
        Det_UsedSlots  = &Det_UsedSlots_RuntimeError;
        Det_ErrorLost  = &Det_ErrorLost_RuntimeError;
#endif
        break;
      }
      case DET_TRANSIENT_FAULT_TYPE:
      {
        /* call the notification functions which are configured */
        /* !LINKSTO EB_Ref.SWS_Det_00035.ReportTransientFault,1 */
        /* !LINKSTO SWS_Det_00502,1 */
        DET_NOTIFICATIONS_TRANSIENT_FAULT(ModuleId, InstanceId, ApiId, ErrorId);
#if (DET_LOGMODE == DET_LOGMODE_INTERNAL)
        Det_WriteIndex = &Det_WriteIndex_TransientFault;
        Det_UsedSlots  = &Det_UsedSlots_TransientFault;
        Det_ErrorLost  = &Det_FaultLost_TransientFault;
#endif
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
        /* Unreachable code. The unreachable code assert function can not be used by DET because
           it would violate  MISRA-C:2012 Rule 17.2 (Required) */
        break;
      }
      /* CHECK: PARSE */
    }

#if (DET_LOGMODE == DET_LOGMODE_INTERNAL)

    /* lock access to DET global variables */
    /* !LINKSTO Det.EB.req.errorbuffer.criticalsection.ReportError,1 */
    /* !LINKSTO Det.EB.req.errorbuffer.criticalsection.ReportRuntimeError,1 */
    /* !LINKSTO Det.EB.req.errorbuffer.criticalsection.ReportTransientFault,1 */
    Det_Enter_SCHM_DET_EXCLUSIVE_AREA_0();

    /* queue the reported error */
#if (DET_REPORTS_TO_KEEP == DET_KEEP_FIRST_REPORTS)
    /* Keep first errors reported to the Det */
    if (*Det_UsedSlots < DET_BUFFERSIZE)
    {
      Det_ErrorBuffer[ErrorType][*Det_WriteIndex].ModuleId   = ModuleId;
      Det_ErrorBuffer[ErrorType][*Det_WriteIndex].InstanceId = InstanceId;
      Det_ErrorBuffer[ErrorType][*Det_WriteIndex].ApiId      = ApiId;
      Det_ErrorBuffer[ErrorType][*Det_WriteIndex].ErrorId    = ErrorId;

      ++(*Det_WriteIndex); /* increase write index, overflow does not matter */
      ++(*Det_UsedSlots);  /* increment used slot counter */
    }
    else /* Det_UsedSlots >= DET_BUFFERSIZE */
    {
      if (*Det_ErrorLost < ((uint16)0xFFFFU))
      {
        ++(*Det_ErrorLost);       /* count lost error reports */
      }
    }
#else /* DET_REPORTS_TO_KEEP == DET_KEEP_LAST_REPORTS */
    /* Keep last errors reported to the Det */
    Det_ErrorBuffer[ErrorType][*Det_WriteIndex].ModuleId   = ModuleId;
    Det_ErrorBuffer[ErrorType][*Det_WriteIndex].InstanceId = InstanceId;
    Det_ErrorBuffer[ErrorType][*Det_WriteIndex].ApiId      = ApiId;
    Det_ErrorBuffer[ErrorType][*Det_WriteIndex].ErrorId    = ErrorId;

    ++(*Det_WriteIndex);                     /* increase write index */
    (*Det_WriteIndex) %= DET_BUFFERSIZE;     /* wrap around */

    if (*Det_UsedSlots < DET_BUFFERSIZE)
    {
      /* increase used slot counter but not beyond its maximum value */
      ++(*Det_UsedSlots);
    }
    else
    {
      if (*Det_ErrorLost < ((uint16)0xFFFFU))
      {
        ++(*Det_ErrorLost);       /* count lost error reports */
      }
    }
#endif /* DET_REPORTS_TO_KEEP == DET_KEEP_FIRST_REPORTS */

/* !LINKSTO EB_Ref.SWS_Det_00026.ReportError,1 */
#if (DET_REPORT_ERROR_STOP_EXECUTION == TRUE)
  Det_StopExecution(ErrorType);
#endif

    /* unlock access to DET global variables */
    Det_Exit_SCHM_DET_EXCLUSIVE_AREA_0();

#else /* DET_LOGMODE == DET_LOGMODE_BREAKPOINT */

    /* For breakpoint logging mode this function does nothing useful. The
     * user may set a breakpoint anywhere within this function and the stack
     * backtrace may be analyzed if the debugger/emulator halts at the
     * breakpoint */

#endif /* DET_LOGMODE switch */
  }
  /* else Det_State == DET_STATE_UNINIT: ignore the function call in the
   * uninitialized state. Other modules would report such an development
   * error to the Det but the Det cannot report this to itself. */

  return transientFaultRetVal;
}
#endif /* DET_MULTICORE_ENABLED == FALSE */

/* !LINKSTO Det.EB.req.multicore.DifferentImplementation,2 */
#if (DET_MULTICORE_ENABLED == TRUE)

/* Service for reporting errors, see declaration in Det_Int.h */
STATIC FUNC(Std_ReturnType, DET_CODE) Det_ReportErrorStatus(
  uint8 ErrorType,
  uint16 ModuleId,
  uint8 InstanceId,
  uint8 ApiId,
  uint8 ErrorId
  )
{
  Atomic_value_t writeIndex;
#if (DET_REPORTS_TO_KEEP == DET_KEEP_LAST_REPORTS)
  Atomic_value_t writeIndexInc;
  Atomic_value_t usedSlotsInc;
  Atomic_value_t usedSlots;
#endif

  Std_ReturnType transientFaultRetVal = E_OK;
#if (DET_LOGMODE == DET_LOGMODE_INTERNAL)
  P2VAR(Atomic_value_t, AUTOMATIC, DET_APPL_DATA) Det_WriteIndex = NULL_PTR;
  P2VAR(Atomic_value_t, AUTOMATIC, DET_APPL_DATA) Det_UsedSlots  = NULL_PTR;
  P2VAR(Atomic_value_t, AUTOMATIC, DET_APPL_DATA) Det_ErrorLost  = NULL_PTR;
#endif

#if (DET_LOGMODE == DET_LOGMODE_BREAKPOINT)
  TS_PARAM_UNUSED(ModuleId);
  TS_PARAM_UNUSED(InstanceId);
  TS_PARAM_UNUSED(ApiId);
  TS_PARAM_UNUSED(ErrorId);
#endif

  if (Det_State != DET_STATE_UNINIT)
  {
    switch(ErrorType)
    {
      case DET_DEVELOPMENT_ERROR_TYPE:
      {
        /* call the notification functions which are configured */
        /* !LINKSTO EB_Ref.SWS_Det_00035.ReportError,1 */
        DET_NOTIFICATIONS_DEVELOPMENT_ERROR(ModuleId, InstanceId, ApiId, ErrorId);
#if (DET_LOGMODE == DET_LOGMODE_INTERNAL)
        Det_WriteIndex = &Det_WriteIndex_DevelopmentError;
        Det_UsedSlots  = &Det_UsedSlots_DevelopmentError;
        Det_ErrorLost  = &Det_ErrorLost_DevelopmentError;
#endif
        break;
      }
      case DET_RUNTIME_ERROR_TYPE:
      {
        /* call the notification functions which are configured */
        /* !LINKSTO EB_Ref.SWS_Det_00035.ReportRuntimeError,1 */
        /* !LINKSTO SWS_Det_00503,1 */
        DET_NOTIFICATIONS_RUNTIME_ERROR(ModuleId, InstanceId, ApiId, ErrorId);
#if (DET_LOGMODE == DET_LOGMODE_INTERNAL)
        Det_WriteIndex = &Det_WriteIndex_RuntimeError;
        Det_UsedSlots  = &Det_UsedSlots_RuntimeError;
        Det_ErrorLost  = &Det_ErrorLost_RuntimeError;
#endif
        break;
      }
      case DET_TRANSIENT_FAULT_TYPE:
      {
        /* call the notification functions which are configured */
        /* !LINKSTO EB_Ref.SWS_Det_00035.ReportTransientFault,1 */
        /* !LINKSTO SWS_Det_00502,1 */
        DET_NOTIFICATIONS_TRANSIENT_FAULT(ModuleId, InstanceId, ApiId, ErrorId);
#if (DET_LOGMODE == DET_LOGMODE_INTERNAL)
        Det_WriteIndex = &Det_WriteIndex_TransientFault;
        Det_UsedSlots  = &Det_UsedSlots_TransientFault;
        Det_ErrorLost  = &Det_FaultLost_TransientFault;
#endif
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
        /* Unreachable code. The unreachable code assert function can not be used by DET because
           it would violate  MISRA-C:2012 Rule 17.2 (Required) */
        break;
      }
      /* CHECK: PARSE */
    }

#if (DET_LOGMODE == DET_LOGMODE_INTERNAL)
#if (DET_REPORTS_TO_KEEP == DET_KEEP_FIRST_REPORTS)
    /* !LINKSTO Det.EB.req.multicore.AtomicAccess,2 */
    writeIndex = Atomics_FetchAdd(Det_WriteIndex, 1U);

    if (writeIndex < DET_BUFFERSIZE)
    {
      Det_ErrorBuffer[ErrorType][writeIndex].ModuleId   = ModuleId;
      Det_ErrorBuffer[ErrorType][writeIndex].InstanceId = InstanceId;
      Det_ErrorBuffer[ErrorType][writeIndex].ApiId      = ApiId;
      Det_ErrorBuffer[ErrorType][writeIndex].ErrorId    = ErrorId;

      (void) Atomics_FetchAdd(Det_UsedSlots, 1U);
    }
    else
    {
      (void) Atomics_FetchAdd(Det_ErrorLost, 1U);
    }
#else /* DET_REPORTS_TO_KEEP == DET_KEEP_LAST_REPORTS */

    /* !LINKSTO Det.EB.req.multicore.AtomicAccess,2 */
    do
    {
      writeIndex = Atomics_Load(Det_WriteIndex);
      if ((writeIndex + 1U) == DET_BUFFERSIZE)
      {
        writeIndexInc = 0U;
      }
      else
      {
        writeIndexInc = writeIndex + 1U;
      }
    }while(!Atomics_CompareExchange(Det_WriteIndex, &writeIndex, writeIndexInc));

    Det_ErrorBuffer[ErrorType][writeIndex].ModuleId   = ModuleId;
    Det_ErrorBuffer[ErrorType][writeIndex].InstanceId = InstanceId;
    Det_ErrorBuffer[ErrorType][writeIndex].ApiId      = ApiId;
    Det_ErrorBuffer[ErrorType][writeIndex].ErrorId    = ErrorId;

    do
    {
      usedSlots = Atomics_Load(Det_UsedSlots);
      if ( usedSlots == DET_BUFFERSIZE)
      {
        usedSlotsInc = DET_BUFFERSIZE;
        (void) Atomics_FetchAdd(Det_ErrorLost, 1U);
      }
      else
      {
        usedSlotsInc = usedSlots + 1U;
      }
    }while(!Atomics_CompareExchange(Det_UsedSlots, &usedSlots, usedSlotsInc));

#endif /* DET_REPORTS_TO_KEEP == DET_KEEP_FIRST_REPORTS */

/* !LINKSTO EB_Ref.SWS_Det_00026.ReportError,1 */
#if (DET_REPORT_ERROR_STOP_EXECUTION == TRUE)
  Det_StopExecution(ErrorType);
#endif

#else /* DET_LOGMODE == DET_LOGMODE_BREAKPOINT */

    /* For breakpoint logging mode this function does nothing useful.  The
     * user may set a breakpoint anywhere within this function and the stack
     * backtrace may be analyzed if the debugger/emulator halts at the
     * breakpoint */

#endif /* DET_LOGMODE switch */
  }
  /* else Det_State == DET_STATE_UNINIT: ignore the function call in the
   * uninitialized state. Other modules would report such a development
   * error to the Det but the Det cannot report this to itself. */
  return transientFaultRetVal;
}

#endif /* DET_MULTICORE_ENABLED == TRUE */

/* Service for reporting of runtime errors, see declaration in Det_BSW.h */
FUNC(Std_ReturnType, DET_CODE) Det_ASR43_ReportTransientFault(
   uint16 ModuleId,
   uint8 InstanceId,
   uint8 ApiId,
   uint8 FaultId)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_DET_ASR43_REPORT_TRANSIENT_FAULT_ENTRY(ModuleId, InstanceId, ApiId, FaultId);

  retVal = Det_ReportErrorStatus(DET_TRANSIENT_FAULT_TYPE,
                                 ModuleId,
                                 InstanceId,
                                 ApiId,
                                 FaultId
                                 );

  DBG_DET_ASR43_REPORT_TRANSIENT_FAULT_EXIT(E_OK, ModuleId, InstanceId, ApiId, FaultId);

  return retVal;
}

/* Service for reporting runtime errors, see declaration in Det_BSW.h */
FUNC(Std_ReturnType, DET_CODE) Det_ASR43_ReportRuntimeError(
   uint16 ModuleId,
   uint8 InstanceId,
   uint8 ApiId,
   uint8 ErrorId)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_DET_ASR43_REPORT_RUNTIME_ERROR_ENTRY(ModuleId, InstanceId, ApiId, ErrorId);

  retVal = Det_ReportErrorStatus(DET_RUNTIME_ERROR_TYPE,
                                 ModuleId,
                                 InstanceId,
                                 ApiId,
                                 ErrorId
                                 );

  DBG_DET_ASR43_REPORT_RUNTIME_ERROR_EXIT(E_OK, ModuleId, InstanceId, ApiId, ErrorId);

  return retVal;
}

/* Service for reporting development errors, see declaration in Det_BSW.h */
FUNC(Std_ReturnType, DET_CODE) Det_ASR40_ReportError(
   uint16 ModuleId,
   uint8 InstanceId,
   uint8 ApiId,
   uint8 ErrorId)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_DET_ASR40_REPORTERROR_ENTRY(ModuleId, InstanceId, ApiId, ErrorId);

  retVal = Det_ReportErrorStatus(DET_DEVELOPMENT_ERROR_TYPE,
                                 ModuleId,
                                 InstanceId,
                                 ApiId,
                                 ErrorId
                                 );

  DBG_DET_ASR40_REPORTERROR_EXIT(E_OK, ModuleId, InstanceId, ApiId, ErrorId);

  return retVal;
}

#if ((DET_ENABLE_ASR32_SERVICE_API == STD_ON) || (DET_DEFAULT_ASR_SERVICE_API == DET_SERVICE_API_ASR32))
/* AUTOSAR 3.2 wrapper function for reporting development errors */
FUNC(void, DET_CODE) Det_ASR32_ReportError(
  uint16 ModuleId,
  uint8  InstanceId,
  uint8  ApiId,
  uint8  ErrorId)
{
  DBG_DET_ASR32_REPORTERROR_ENTRY(ModuleId,InstanceId,ApiId,ErrorId);

  /* Casting to void because according to AUTOSAR 3.2 there is no
   * return value*/
  (void) Det_ASR40_ReportError(ModuleId,InstanceId, ApiId, ErrorId);

  DBG_DET_ASR32_REPORTERROR_EXIT(ModuleId,InstanceId,ApiId,ErrorId);
}
#endif

#if (DET_VERSION_INFO_API == STD_ON)
FUNC(void, DET_CODE) Det_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, DET_APPL_DATA) VersionInfoPtr)
{
  DBG_DET_GETVERSIONINFO_ENTRY(VersionInfoPtr);
  /* !LINKSTO DET051,1 */
  if (VersionInfoPtr != NULL_PTR)
  {
    VersionInfoPtr->vendorID         = DET_VENDOR_ID;
    VersionInfoPtr->moduleID         = DET_MODULE_ID;
    VersionInfoPtr->sw_major_version = DET_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = DET_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = DET_SW_PATCH_VERSION;
  }
  else
  {
    /* !LINKSTO DET023,1 */
    /* !LINKSTO SWS_Det_00052,1 */
    /* call all notification functions which are configured */
    (void)Det_ASR40_ReportError( DET_MODULE_ID, 0U, DET_SID_GET_VERSION_INFO, DET_E_PARAM_POINTER );
  }
   DBG_DET_GETVERSIONINFO_EXIT(VersionInfoPtr);
}
#endif

/*------------------[defensive programming]---------------------------------*/
#if (TS_ARCH_FAMILY == TS_WINDOWS)

#if (DET_PRECONDITION_ASSERT_ENABLED == STD_ON)
FUNC(void, DET_CODE) Det_PreconditionAssertPrint(
  uint16 ModuleId, uint8 InstanceId, uint8 ApiId,
  const char *File, uint32 Line, const char *Condition)
{
  DBG_DET_PRECONDITIONASSERTPRINT_ENTRY(ModuleId,InstanceId,ApiId,File,Line,Condition);

  /* Deviation MISRAC2012-1 */
  (void)fprintf(
    stderr,
    "%s, %u: ApiId %u:\n"
    "ERROR: Precondition assertion \"%s\" failed in instance %u of module %u\n",
    File, (uint32)Line, ApiId, Condition, InstanceId, ModuleId);

  DBG_DET_PRECONDITIONASSERTPRINT_EXIT(ModuleId,InstanceId,ApiId,File,Line,Condition);
}
#endif

#if (DET_POSTCONDITION_ASSERT_ENABLED == STD_ON)
FUNC(void, DET_CODE) Det_PostconditionAssertPrint(
  uint16 ModuleId, uint8 InstanceId, uint8 ApiId,
  const char *File, uint32 Line, const char *Condition)
{
  DBG_DET_POSTCONDITIONASSERTPRINT_ENTRY(ModuleId,InstanceId,ApiId,File,Line,Condition);

  /* Deviation MISRAC2012-1 */
  (void)fprintf(
    stderr,
    "%s, %u: ApiId %u:\n"
    "ERROR: Postcondition assertion \"%s\" failed in instance %u of module %u\n",
    File, (uint32)Line, ApiId, Condition, InstanceId, ModuleId);

  DBG_DET_POSTCONDITIONASSERTPRINT_EXIT(ModuleId,InstanceId,ApiId,File,Line,Condition);
}
#endif

#if (DET_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
FUNC(void, DET_CODE) Det_UnreachableCodeAssertPrint(
  uint16 ModuleId, uint8 InstanceId, uint8 ApiId,
  const char *File, uint32 Line)
{
  DBG_DET_UNREACHABLECODEASSERTPRINT_ENTRY(ModuleId,InstanceId,ApiId,File,Line);

  /* Deviation MISRAC2012-1 */
  (void)fprintf(
    stderr,
    "%s, %u: ApiId %u:\n"
    "ERROR: Unreachable code assertion failed in instance %u of module %u\n",
    File, (uint32)Line, ApiId, InstanceId, ModuleId);

  DBG_DET_UNREACHABLECODEASSERTPRINT_EXIT(ModuleId,InstanceId,ApiId,File,Line);
}
#endif

#if (DET_INVARIANT_ASSERT_ENABLED == STD_ON)
FUNC(void, DET_CODE) Det_InvariantAssertPrint(
  uint16 ModuleId, uint8 InstanceId, uint8 ApiId,
  const char *File, uint32 Line, const char *Condition)
{
  DBG_DET_INVARIANTASSERTPRINT_ENTRY(ModuleId,InstanceId,ApiId,File,Line,Condition);

  /* Deviation MISRAC2012-1 */
  (void)fprintf(
    stderr,
    "%s, %u: ApiId %u:\n"
    "ERROR: Invariant assertion \"%s\" failed in instance %u of module %u\n",
    File, (uint32)Line, ApiId, Condition, InstanceId, ModuleId);

  DBG_DET_INVARIANTASSERTPRINT_EXIT(ModuleId,InstanceId,ApiId,File,Line,Condition);
}
#endif

#endif /* if (TS_ARCH_FAMILY == TS_WINDOWS) */

/*==================[internal function definitions]=========================*/

#define DET_STOP_SEC_CODE
/* !LINKSTO Det.EB.req.bsw_include,1 */
#include <Det_MemMap.h>

/*==================[end of file]=================================*/
