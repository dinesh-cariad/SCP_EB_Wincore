/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* This file contains the implementation of the hierarchical state machine
 * driver. */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 19.10 (required)
 *     Parameter instance shall be enclosed in parentheses.
 *
 *     Reason:
 *     The parameter is used in function parameter declarations, definitions
 *     or as structure members and enclosing it in parentheses results
 *     in compiler syntax error.
 *
 *  MISRA-2) Deviated Rule: 20.9 (required)
 *     The input/output library <stdio.h> shall not be used in production code.
 *
 *     Reason:
 *     Library function printf() which is defined inside library <stdio.h> is
 *     used in this file, for debugging purpose only.
 *
 *  MISRA-3) Deviated Rule: 13.5 (required)
 *     The three expressions of a for statement shall be concerned only with loop control.
 *
 *     Reason:
 *     MISRA rule 13.5 is not deviated. This is a 'false positive' in the MISRA checker.
 *
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 14.2 (required)
  *     "A for loop shall be well-formed."
  *
  *     Reason:
  *     This MISRA rule is not deviated. This is a 'false positive' in the MISRA checker.
  *
  *  MISRAC2012-2) Deviated Rule: 21.6 (required)
  *     "The Standard Library input/output functions shall not be used."
  *
  *     Reason:
  *     Library function printf() which is defined inside the standard library is
  *     used in this file, for debugging purpose only.
  *
  *  MISRAC2012-3) Deviated Rule: 21.1 (required)
  *     "#define and #undef shall not be used on a reserved identifier or reserved macro name."
  *
  *     Reason:
  *     This macro name is required to be set, to include only the basic functionality of
  *     the functions that are required from the standard libraries.
  */

/*==================[inclusions]============================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>          /* AUTOSAR standard types */

#include <Dcm_Int.h>            /* Module intenal definitions */
#include <SchM_Dcm.h>           /* SchM symbols for crit. sections */

#include <Dcm_Hsm.h>            /* public API of Dcm_Hsm.c */
/* design debugging output */
#if (DCM_HSM_TRACE == STD_ON)
/* __NO_ISOCEXT is a switch which activates some of options/definitions in HSM */
/* CHECK: RULE 501 OFF (macro needed in std headers) */
/* Deviation MISRAC2012-3 <START> */
#if (defined __NO_ISOCEXT)
  #error "__NO_ISOCEXT is already defined"
#endif
#define __NO_ISOCEXT
/* Deviation MISRAC2012-3 <STOP> */
# include <stdlib.h>
# include <stdio.h>
/* CHECK: RULE 501 ON */
#endif

/*==================[macros]================================================*/

#if (DCM_HSM_INST_MULTI_ENABLED == STD_ON)
#if (defined DCM_HSM_DEL_FROM_QUEUE)
  #error "DCM_HSM_DEL_FROM_QUEUE is already defined"
#endif
#define DCM_HSM_DEL_FROM_QUEUE(a,b,c) Dcm_HsmDelFromQueue(a,b,c)

#if (defined DCM_HSM_TRAN)
  #error "DCM_HSM_TRAN is already defined"
#endif
#define DCM_HSM_TRAN(a,b,c)           Dcm_HsmTran(a,b,c)

#if (defined DCM_HSM_INIT_SUBSTATES)
  #error "DCM_HSM_INIT_SUBSTATES is already defined"
#endif
#define DCM_HSM_INIT_SUBSTATES(a,b)   Dcm_HsmInitSubstates(a,b)

#if (defined DCM_HSM_FIND_EV_TO_DISPATCH)
  #error "DCM_HSM_FIND_EV_TO_DISPATCH is already defined"
#endif
#define DCM_HSM_FIND_EV_TO_DISPATCH(a,b,c,d) Dcm_HsmFindEvToDispatch(a,b,c,d)

/** \brief Access entry in event queue of a specific hsm instance */
/* Deviation MISRA-1 <+2> */
#if (defined DCM_EV_QUEUE_ENTRY)
  #error "DCM_EV_QUEUE_ENTRY is already defined"
#endif
#define DCM_EV_QUEUE_ENTRY(sc,instIdx,entryIdx) \
  (sc->events[((instIdx)*(sc->evQueueSize))+(entryIdx)])

#else
#if (defined DCM_HSM_DEL_FROM_QUEUE)
  #error "DCM_HSM_DEL_FROM_QUEUE is already defined"
#endif
#define DCM_HSM_DEL_FROM_QUEUE(a,b,c) Dcm_HsmDelFromQueue(a,c)

#if (defined DCM_HSM_TRAN)
  #error "DCM_HSM_TRAN is already defined"
#endif
#define DCM_HSM_TRAN(a,b,c)           Dcm_HsmTran(a,c)

#if (defined DCM_HSM_INIT_SUBSTATES)
  #error "DCM_HSM_INIT_SUBSTATES is already defined"
#endif
#define DCM_HSM_INIT_SUBSTATES(a,b)   Dcm_HsmInitSubstates(a)

#if (defined DCM_HSM_FIND_EV_TO_DISPATCH)
  #error "DCM_HSM_FIND_EV_TO_DISPATCH is already defined"
#endif
#define DCM_HSM_FIND_EV_TO_DISPATCH(a,b,c,d) Dcm_HsmFindEvToDispatch(a,c,d)
/* Deviation MISRA-1 <+2> */

#if (defined DCM_EV_QUEUE_ENTRY)
  #error "DCM_EV_QUEUE_ENTRY is already defined"
#endif
#define DCM_EV_QUEUE_ENTRY(sc,instIdx,entryIdx) \
  (sc->events[entryIdx])
#endif

#if (DCM_HSM_TRACE == STD_ON)
#if (defined DCM_HSM_PRINT1)
  #error "DCM_HSM_PRINT1 is already defined"
#endif
/* Deviation MISRAC2012-2 */
#define DCM_HSM_PRINT1(a)                       \
  do                                            \
  {                                             \
    if (Dcm_HsmTraceEnabled == TRUE)            \
    {                                           \
      /* Deviation MISRA-1 */                   \
      (void)printf("HsmTrace: " a);             \
    }                                           \
  } while (0U)

#if (defined DCM_HSM_PRINT2)
  #error "DCM_HSM_PRINT2 is already defined"
#endif
/* Deviation MISRAC2012-2 */
#define DCM_HSM_PRINT2(a,b)                     \
  do                                            \
  {                                             \
    if (Dcm_HsmTraceEnabled == TRUE)            \
    {                                           \
      /* Deviation MISRA-1 */                   \
      (void)printf("HsmTrace: " a,b);           \
    }                                           \
  } while (0U)

#if (defined DCM_HSM_PRINT3)
  #error "DCM_HSM_PRINT3 is already defined"
#endif
/* Deviation MISRAC2012-2 */
#define DCM_HSM_PRINT3(a,b,c)                   \
  do                                            \
  {                                             \
    if (Dcm_HsmTraceEnabled == TRUE)            \
    {                                           \
      /* Deviation MISRA-1 */                   \
      (void)printf("HsmTrace: " a,b,c);         \
    }                                           \
  } while (0U)

#if (defined DCM_HSM_PRINT4)
  #error "DCM_HSM_PRINT4 is already defined"
#endif
/* Deviation MISRAC2012-2 */
#define DCM_HSM_PRINT4(a,b,c,d)                 \
  do                                            \
  {                                             \
    if (Dcm_HsmTraceEnabled == TRUE)            \
    {                                           \
      /* Deviation MISRA-1 */                   \
      (void)printf("HsmTrace: " a,b,c,d);       \
    }                                           \
  } while (0U)

#if (defined DCM_HSM_PRINT5)
  #error "DCM_HSM_PRINT5 is already defined"
#endif
/* Deviation MISRAC2012-2 */
#define DCM_HSM_PRINT5(a,b,c,d,e)               \
  do                                            \
  {                                             \
    if (Dcm_HsmTraceEnabled == TRUE)            \
    {                                           \
      /* Deviation MISRA-1 */                   \
      (void)printf("HsmTrace: " a,b,c,d,e);     \
    }                                           \
  } while (0U)

#else
#if (defined DCM_HSM_PRINT1)
  #error "DCM_HSM_PRINT1 is already defined"
#endif
#define DCM_HSM_PRINT1(a)          /* nothing */

#if (defined DCM_HSM_PRINT2)
  #error "DCM_HSM_PRINT2 is already defined"
#endif
#define DCM_HSM_PRINT2(a,b)        /* nothing */

#if (defined DCM_HSM_PRINT3)
  #error "DCM_HSM_PRINT3 is already defined"
#endif
#define DCM_HSM_PRINT3(a,b,c)      /* nothing */

#if (defined DCM_HSM_PRINT4)
  #error "DCM_HSM_PRINT4 is already defined"
#endif
#define DCM_HSM_PRINT4(a,b,c,d)    /* nothing */

#if (defined DCM_HSM_PRINT5)
  #error "DCM_HSM_PRINT5 is already defined"
#endif
#define DCM_HSM_PRINT5(a,b,c,d,e)  /* nothing */
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Delete entry from event queue
 ** \param sc pointer to state chart
 ** \param instIdx index of state machine instance to work on
 ** \param queueEntryNo entry in queue to delete
 **
 ** This function must be called inside of a critical section which prevents
 ** the concurrent access to the queue.
 */
STATIC FUNC(void, DCM_CODE) DCM_HSM_DEL_FROM_QUEUE(
  const Dcm_HsmStatechartPtrType sc,
  const uint8              instIdx,
  const uint8              queueEntryNo);

/** \brief Execute a transition
 ** \param sc pointer to state chart
 ** \param instIdx index of state machine instance to work on
 ** \param tran Pointer to the transition description struct */
STATIC FUNC(void, DCM_CODE) DCM_HSM_TRAN(
  const Dcm_HsmStatechartPtrType                      sc,
  const uint8                                         instIdx,
  CONSTP2CONST(Dcm_HsmTranType, AUTOMATIC, DCM_CONST) tran);

/** \brief Initialize all substates for the current state
 ** \param sc pointer to state chart to work on
 ** \param instIdx index of state machine instance to work on */
STATIC FUNC(void, DCM_CODE) DCM_HSM_INIT_SUBSTATES(
  const Dcm_HsmStatechartPtrType sc,
  const uint8                    instIdx);

/** \brief Look for dispatchable event in event queue
 ** \param sc pointer to state chart to work on
 ** \param instIdx index of state machine instance to work on
 ** \param event identified event from queue
 ** \param queueEntryNo index in queue for identified event
 ** \return look up status
 ** \retval TRUE an event is found in the queue and it is not deferred
 ** and not ignored
 ** \retval FALSE no event is not found or one which is deferred or one
 ** which is ignored in the current state
 **
 ** Events which are marked to be ignored in the current state are deleted
 ** from the queue within this function. */
STATIC FUNC(boolean, DCM_CODE) DCM_HSM_FIND_EV_TO_DISPATCH(
  const Dcm_HsmStatechartPtrType                     sc,
  const uint8                                        instIdx,
  CONSTP2VAR(Dcm_HsmEventType, AUTOMATIC, AUTOMATIC) event,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)            queueEntryNo);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#if (DCM_HSM_TRACE == STD_ON)

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

STATIC VAR(boolean, DCM_VAR) Dcm_HsmTraceEnabled = FALSE;

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

#endif

/*==================[external function definitions]=========================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/*------------------ init functions ----------------------------------------*/

#if (DCM_HSM_INST_MULTI_ENABLED == STD_ON)
FUNC(void, DCM_CODE) Dcm_HsmInit(const Dcm_HsmStatechartPtrType sc)
{
  uint8 instIdx;

  DBG_DCM_HSMINIT_ENTRY(sc);
  for (instIdx = 0U; instIdx < sc->numInstances; ++instIdx)
  {
    DCM_HSMINITINST(sc, instIdx);
  }

  DBG_DCM_HSMINIT_EXIT(sc);
}
/* else: function is implemented as macro and maps to Dcm_HsmInitInst() */
#endif

FUNC(void, DCM_CODE) DCM_HSMINITINST(
  const Dcm_HsmStatechartPtrType sc,
  const uint8                    instIdx)
{
  CONSTP2VAR(Dcm_HsmInstType, AUTOMATIC, DCM_VAR_CLEARED) inst
    = &DCM_INST_ACCESS((sc->instances), (instIdx));
  uint8_least i;

  DBG_DCM_HSMINITINST_ENTRY(sc,DCM_INST(instIdx));
  /* Deviation MISRAC2012-2 */
  /* Deviation MISRA-2 */
  DCM_HSM_PRINT3("%s %d Init\n", sc->hsmName, DCM_INST(instIdx));

  /* init all fields because the struct is placed in a NOINIT memory
   * section */
  inst->stateId  = sc->topStateId;
  inst->sourceId = DCM_HSM_SIDX_INVALID;

  for (i = 0U; i < sc->evQueueSize; ++i)
  {
    DCM_EV_QUEUE_ENTRY(sc, instIdx, (uint8)i) = 0U;
  }
  inst->evQueueFill       = 0U;
  inst->evQueueInsertEvNo = 0U;

  if (sc->states[inst->stateId].entryFnIdx != DCM_HSM_AIDX_INVALID)
  {
  /* Deviation MISRAC2012-2 */
  /* Deviation MISRA-2 */
    DCM_HSM_PRINT4(
      "%s %d %s\n",
      sc->hsmName, DCM_INST(instIdx), sc->actionNames[inst->stateId]);

    /* enter top state */
    sc->actions[sc->states[inst->stateId].entryFnIdx](DCM_PL_SF(instIdx));
  }
  /* CHECK: NOPARSE */
  else
  {
    /* Unreachable code: Since the top state has an action, this code is unreachable. */
    DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
  }
  /* CHECK: PARSE */
  /* init all substates */
  DCM_HSM_INIT_SUBSTATES(sc, instIdx);

  DBG_DCM_HSMINITINST_EXIT(sc,DCM_INST(instIdx));
}

/*------------------ emit functions ----------------------------------------*/

FUNC(Std_ReturnType, DCM_CODE) DCM_HSMEMITINST(
  const Dcm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const Dcm_HsmEventType         event)
{
  Std_ReturnType RetVal = E_NOT_OK;

  CONSTP2VAR(Dcm_HsmInstType, AUTOMATIC, DCM_VAR_CLEARED) inst
    = &DCM_INST_ACCESS((sc->instances), (instIdx));

  DBG_DCM_HSMEMITINST_ENTRY(sc,DCM_INST(instIdx),event);

  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  if (inst->evQueueFill < sc->evQueueSize)
  {
    DCM_EV_QUEUE_ENTRY((sc), (instIdx), (inst->evQueueFill)) = event;
    inst->evQueueFill++;
    /* leave evQueueInsertEvNo unchanged, this pointer will be updated on
     * insertion or event dispatching */
    RetVal = E_OK;
  }
  /* CHECK: NOPARSE */
  else
  {
    /* Unreachable code: We don't expect to ever exceed the Dcm's event queue size */
    DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
  }
  /* CHECK: PARSE */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  /* Deviation MISRAC2012-2 */
  /* Deviation MISRA-2 */
  DCM_HSM_PRINT5(
    "%s %d event %s enqueued, status %d\n",
    sc->hsmName, DCM_INST(instIdx), sc->eventNames[event], RetVal);


  DBG_DCM_HSMEMITINST_EXIT(RetVal,sc,DCM_INST(instIdx),event);
  return RetVal;
}

FUNC(Std_ReturnType, DCM_CODE) DCM_HSMEMITTOSELFINST(
  const Dcm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const Dcm_HsmEventType         event)
{
  Std_ReturnType RetVal = E_NOT_OK;

  CONSTP2VAR(Dcm_HsmInstType, AUTOMATIC, DCM_VAR_CLEARED) inst
    = &DCM_INST_ACCESS((sc->instances), (instIdx));

  DBG_DCM_HSMEMITTOSELFINST_ENTRY(sc,DCM_INST(instIdx),event);

  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  if (inst->evQueueFill < sc->evQueueSize)
  {
    /* make room for new entry */
    uint8_least j;
    for (j = inst->evQueueFill; j > inst->evQueueInsertEvNo; --j)
    {
      DCM_EV_QUEUE_ENTRY(sc, instIdx, (uint8)j) = DCM_EV_QUEUE_ENTRY(sc, instIdx, (uint8)(j-1U));
    }
    ++inst->evQueueFill;
    DCM_EV_QUEUE_ENTRY((sc), (instIdx), (inst->evQueueInsertEvNo)) = event;
    inst->evQueueInsertEvNo++;
    RetVal = E_OK;
  }
  /* CHECK: NOPARSE */
  else
  {
    /* Unreachable code: We don't expect to ever exceed the Dcm's event queue size */
    DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
  }
  /* CHECK: PARSE */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  /* Deviation MISRAC2012-2 */
  /* Deviation MISRA-2 */
  DCM_HSM_PRINT5(
    "%s %d event %s enqueued internally, status %d\n",
    sc->hsmName, DCM_INST(instIdx), sc->eventNames[event], RetVal);


  DBG_DCM_HSMEMITTOSELFINST_EXIT(RetVal,sc,DCM_INST(instIdx),event);
  return RetVal;
}

/*------------------ main functions ----------------------------------------*/

#if (DCM_HSM_INST_MULTI_ENABLED == STD_ON)
FUNC(void, DCM_CODE) Dcm_HsmMain(const Dcm_HsmStatechartPtrType sc)
{
  boolean transitionFired;

  DBG_DCM_HSMMAIN_ENTRY(sc);

  do
  {
    uint8_least instIdx;
    transitionFired = FALSE;
    /* for all instances */
    for (instIdx = 0U; instIdx < sc->numInstances; ++instIdx)
    {
      const boolean retVal = Dcm_HsmMainInst(sc, (uint8)instIdx);
      transitionFired = (retVal || transitionFired);
    }
  } while (transitionFired);

  DBG_DCM_HSMMAIN_EXIT(sc);
}
/* else: function is implemented as macro and maps to Dcm_HsmMainInst() */
#endif

FUNC(boolean, DCM_CODE) DCM_HSMMAININST(
  const Dcm_HsmStatechartPtrType sc,
  const uint8                    instIdx)
{
  CONSTP2VAR(Dcm_HsmInstType, AUTOMATIC, DCM_VAR_CLEARED) inst
    = &DCM_INST_ACCESS((sc->instances), (instIdx));
  /* return value, flag indicating if at least one transition fired */
  boolean          transitionFired = FALSE;
  Dcm_HsmEventType event;
  uint8            queueEntryNo;

  DBG_DCM_HSMMAININST_ENTRY(sc,DCM_INST(instIdx));
  /* Deviation MISRAC2012-2 */
  /* Deviation MISRA-2 */
  DCM_HSM_PRINT3(
    "%s %d Main\n",
    sc->hsmName, DCM_INST(instIdx));

  /* reset event insertion pointer to an invalid value, this will be set to a
   * valid location if the first event is being dispatched */
  inst->evQueueInsertEvNo = 0U;

  while (TRUE
         == DCM_HSM_FIND_EV_TO_DISPATCH(sc, instIdx, &event, &queueEntryNo))
  {
    P2CONST(Dcm_HsmTranType, AUTOMATIC, DCM_CONST) trans = NULL_PTR;

    /* flag for the trigger search algorithm */
    boolean triggerFound = FALSE;

    /* *** search for transition for non-deferred and non-ignored event and
     * fire the transition if found *** */
    /* Deviation MISRAC2012-2 */
    /* Deviation MISRA-2 */
    DCM_HSM_PRINT5(
      "%s %d dispatching event %s in state %s\n",
      sc->hsmName, DCM_INST(instIdx), sc->eventNames[event],
      sc->stateNames[inst->stateId]);

    /* try current leaf state as src of transitions */
    inst->sourceId = inst->stateId;

    /* start searching for a triggering event */
    do
    {
      uint8_least i;          /* index in transition array of state */

      CONSTP2CONST(Dcm_HsmStateType, AUTOMATIC, DCM_CONST) state
        = &sc->states[inst->sourceId];

      /* check all possible transitions of state */
      for (i = 0U; (i < state->numTrans) && (FALSE == triggerFound); ++i)
      {
        trans = &state->trans[i]; /* test next transition */

        if (event == trans->event)
        {
          /* evaluate guard condition */
          boolean guardCond = TRUE;

          if (trans->guardFnIdx != DCM_HSM_GIDX_INVALID)
          {
            guardCond = sc->guards[trans->guardFnIdx](DCM_PL_SF(instIdx));
            /* Deviation MISRAC2012-2 */
            /* Deviation MISRA-2 */
            DCM_HSM_PRINT5(
              "%s %d %s evaluates to %s\n",
              sc->hsmName, DCM_INST(instIdx),
              sc->guardNames[trans->guardFnIdx], (guardCond?"TRUE":"FALSE"));
          }

          if (TRUE == guardCond)
          {
            /* exit for loop and middle do-while loop in any case because we
             * have found at least one enabled transition for this event. */
            triggerFound    = TRUE;

            /* remember that some real event processing e.g. a state machine
             * action has happens. In this case a subsequent call of this or
             * other dispatching functions may be necessary */
            transitionFired = TRUE;

            /* event+guard is OK: transition action is perfomed later after
             * loops are left and stack usage is lower */
          }
          /* else: if guard is false, then treat it as if the related
           * transition is not existing in the state chart model, "Practical
           * Statecharts", 2.ed, sect. 2.3.15  */
        }
      }

      if (FALSE == triggerFound)
      {
        /* we did not find the trigger in this state: try parent state as next
         * source state */
        inst->sourceId = state->parentId;
      }
    } while ((FALSE == triggerFound)
             && (DCM_HSM_SIDX_INVALID != inst->sourceId));

    /* a triggering event was found and transition is ready to be executed or
     * the top state is reached the queued event must be ignored
     *
     * we have to check if the top state is not yet reached at runtime because
     * there may still be non-ignored and not handled events due to false
     * guard conditions */

    if (TRUE == triggerFound)
    {
      DCM_HSM_TRAN(sc, instIdx, trans); /* execute the transition */
    }
    /* CHECK: NOPARSE */
    else
    {
      /* Unreachable code: This branch can be reached if in one of the state we have a transition
       * which is guarded and transitions with complementary guards are missing.
       * For the moment we do not have such transitions.
       */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
     /* Deviation MISRAC2012-2 */
     /* Deviation MISRA-2 */
      DCM_HSM_PRINT4(
        "%s %d event %s ignored at top state\n",
        sc->hsmName, DCM_INST(instIdx), sc->eventNames[event]);
    }
    /* CHECK: PARSE */
    /* delete dispatched event from queue */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
    DCM_HSM_DEL_FROM_QUEUE(sc, instIdx, queueEntryNo);
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  }

  DBG_DCM_HSMMAININST_EXIT(transitionFired,sc,DCM_INST(instIdx));
  return transitionFired;
}

#if (DCM_HSM_TRACE == STD_ON)
FUNC(void, DCM_CODE) Dcm_HsmSetTracing(const boolean newValue)
{
  DBG_DCM_HSMSETTRACING_ENTRY(newValue);

  Dcm_HsmTraceEnabled = newValue;

  DBG_DCM_HSMSETTRACING_EXIT(newValue);
}
#endif

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]=========================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

STATIC FUNC(void, DCM_CODE) DCM_HSM_DEL_FROM_QUEUE(
  const Dcm_HsmStatechartPtrType sc,
  const uint8              instIdx,
  const uint8              queueEntryNo)
{
  /* delete event no queueEntryNo from queue */
  CONSTP2VAR(Dcm_HsmInstType, AUTOMATIC, DCM_VAR_CLEARED) inst
    = &DCM_INST_ACCESS((sc->instances), (instIdx));
  uint8_least j;

  /* Deviation MISRAC2012-1 */
  /* Deviation MISRA-3 */
  for (j = queueEntryNo; (j + 1U) < inst->evQueueFill; ++j)
  {
    DCM_EV_QUEUE_ENTRY(sc, instIdx, (uint8)j)
      = DCM_EV_QUEUE_ENTRY(sc, instIdx, (uint8)(j + 1U));
  }
  --inst->evQueueFill;
  if (queueEntryNo < inst->evQueueInsertEvNo)
  {
    /* if this was an event emitted to self also decrement the insertion
     * pointer  */
    --inst->evQueueInsertEvNo;
  }
  /* no need to check for queueEntryNo >= inst->evQueueInsertEvNo here.  This
   * will be done before dispatching the next event from the queue */

}

STATIC FUNC(void, DCM_CODE) DCM_HSM_TRAN(
  const Dcm_HsmStatechartPtrType                      sc,
  const uint8                                   instIdx,
  CONSTP2CONST(Dcm_HsmTranType, AUTOMATIC, DCM_CONST) tran)
{
  P2CONST(Dcm_HsmStateType, AUTOMATIC, DCM_CONST) state;

  /* perform simplified transition for internal transitions */
  if (tran->target == DCM_HSM_SIDX_INVALID)
  {
    if (tran->numSteps == 1U)
    {
      /* Deviation MISRAC2012-2 */
      /* Deviation MISRA-2 */
      DCM_HSM_PRINT4(
        "%s %d %s (internal transition)\n",
        sc->hsmName, DCM_INST(instIdx), sc->actionNames[tran->steps[0U]]);
      sc->actions[tran->steps[0U]](DCM_PL_SF(instIdx));
    }
  }
  else /* external transitions */
  {
    CONSTP2VAR(Dcm_HsmInstType, AUTOMATIC, DCM_VAR_CLEARED) inst
      = &DCM_INST_ACCESS((sc->instances), (instIdx));
    uint8_least i;

    /* "source" may be a superstate of "state".  exit all states from "state"
     * until "source" is reached */
    while (inst->stateId != inst->sourceId)
    {
      state = &sc->states[inst->stateId];

      if (state->exitFnIdx != DCM_HSM_AIDX_INVALID)
      {
      /* Deviation MISRAC2012-2 */
      /* Deviation MISRA-2 */
        DCM_HSM_PRINT4(
          "%s %d %s\n",
          sc->hsmName, DCM_INST(instIdx), sc->actionNames[state->exitFnIdx]);
        sc->actions[state->exitFnIdx](DCM_PL_SF(instIdx));
      }

      inst->stateId = state->parentId;
    }

    /* execute exit/transiton/entry actions on transition path */
    for (i = 0U; i < tran->numSteps; ++i)
    {
      /* Deviation MISRAC2012-2 */
      /* Deviation MISRA-2 */
      DCM_HSM_PRINT4(
        "%s %d %s\n",
        sc->hsmName, DCM_INST(instIdx), sc->actionNames[tran->steps[i]]);
      sc->actions[tran->steps[i]](DCM_PL_SF(instIdx));
    }

    /* set the new current state and perform init steps of substates */
    inst->stateId = tran->target; /* current state is last entered state */
    DCM_HSM_INIT_SUBSTATES(sc, instIdx);

    /* Deviation MISRAC2012-2 */
    /* Deviation MISRA-2 */
    DCM_HSM_PRINT4(
      "%s %d transition to state %s finished\n",
      sc->hsmName, DCM_INST(instIdx), sc->stateNames[inst->stateId]);
  }

}

STATIC FUNC(void, DCM_CODE) DCM_HSM_INIT_SUBSTATES(
  const Dcm_HsmStatechartPtrType sc,
  const uint8                    instIdx)
{
  CONSTP2VAR(Dcm_HsmInstType, AUTOMATIC, DCM_VAR_CLEARED) inst
    = &DCM_INST_ACCESS((sc->instances), (instIdx));

  Dcm_HsmStateIdxType stateIdx = sc->states[inst->stateId].initId;

  while (stateIdx != DCM_HSM_SIDX_INVALID) /* init state if necessary */
  {
    P2CONST(Dcm_HsmStateType, AUTOMATIC, DCM_CONST) state
      = &sc->states[stateIdx];

    inst->stateId = stateIdx; /* set new current state */
    /* enter the substate */
    if (state->entryFnIdx != DCM_HSM_AIDX_INVALID)
    {
      /* Deviation MISRAC2012-2 */
      /* Deviation MISRA-2 */
      DCM_HSM_PRINT4(
        "%s %d %s\n",
        sc->hsmName, DCM_INST(instIdx), sc->actionNames[state->entryFnIdx]);
      sc->actions[state->entryFnIdx](DCM_PL_SF(instIdx));
    }
    /* get next init state */
    stateIdx = state->initId;
  }

}

STATIC FUNC(boolean, DCM_CODE) DCM_HSM_FIND_EV_TO_DISPATCH(
  const Dcm_HsmStatechartPtrType                     sc,
  const uint8                                        instIdx,
  CONSTP2VAR(Dcm_HsmEventType, AUTOMATIC, AUTOMATIC) event,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)            queueEntryNo)
{
  CONSTP2VAR(Dcm_HsmInstType, AUTOMATIC, DCM_VAR_CLEARED) inst
    = &DCM_INST_ACCESS((sc->instances), (instIdx));
  CONSTP2CONST(Dcm_HsmStateType, AUTOMATIC, DCM_CONST) state
    = &sc->states[inst->stateId];
  boolean          foundEventToDispatch = FALSE;
  Dcm_HsmEventType ev                   = DCM_HSM_EVENT_INVALID;
  uint8_least      entryNo              = 0U;

  /* lock event queue access */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  while ((entryNo < inst->evQueueFill)
         && (FALSE == foundEventToDispatch))
  {
    Dcm_HsmEventMaskType eventMask;
    ev        = DCM_EV_QUEUE_ENTRY(sc, instIdx, (uint8)entryNo);
    eventMask = (Dcm_HsmEventMaskType)(1UL << ev);

    if ((eventMask & state->ignoredEventMask) != 0U)
    {
      /* Deviation MISRAC2012-2 */
      /* Deviation MISRA-2 */
      DCM_HSM_PRINT4(
        "%s %d event %s ignored\n",
        sc->hsmName, DCM_INST(instIdx), sc->eventNames[ev]);

      /* delete event no 'entryNo' from queue */
      DCM_HSM_DEL_FROM_QUEUE(sc, instIdx, (uint8)entryNo);
      /* do not increment entryNo */
    }
    else if ((eventMask & state->actionEventMask) != 0U)
    {
      if (inst->evQueueInsertEvNo <= entryNo)
      {
        /* if the insertion location is still unset (or pointing to an
         * invalid location) then set the insertion location to just after
         * the current event to be dispatched. */
        inst->evQueueInsertEvNo = (uint8)(entryNo + 1U);
      }
      /* leave while loop and dispatch event no entryNo */
      foundEventToDispatch = TRUE;
    }
    else
    {
      /* Deviation MISRAC2012-2 */
      /* Deviation MISRA-2 */
      DCM_HSM_PRINT4(
        "%s %d event %s deferred\n",
        sc->hsmName, DCM_INST(instIdx), sc->eventNames[ev]);

      /* found only deferred event, try next one in queue */
      ++entryNo;
    }
  }
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  *event        = ev;
  *queueEntryNo = (uint8)entryNo;

  return foundEventToDispatch;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[end of file]===========================================*/
