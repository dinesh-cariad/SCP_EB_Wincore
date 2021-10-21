/**
 * \file
 *
 * \brief AUTOSAR CanNm
 *
 * This file contains the implementation of the AUTOSAR
 * module CanNm.
 *
 * \version 6.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* This file contains the implementation of the hierarchical state machine
 * driver. */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 21.1 (required)
 * #define and #undef shall not be used on a reserved identifier or reserved macro name.
 *
 * Reason:
 * Printf(), which is defined inside library <stdio.h> is used in this file, for the debugging
 * purpose only.
 *
 * MISRAC2012-2) Deviated Rule: 21.6 (required)
 * The Standard Library input/output functions shall not be used.
 *
 * Reason:
 * Printf(), which is defined inside library <stdio.h> is used in this file, for the debugging
 * purpose only.
 *
 * MISRAC2012-3) Deviated Rule: 14.2 (required)
 * A for loop shall be well-formed.
 *
 * Reason:
 * Violations of rule 14.2 is false positive, rearranging the conditioncheck would lead towards
 * unexpected result.
 */

/*==================[inclusions]============================================*/

#define CANNM_NO_CFGCLASSMIX_REQUIRED
#ifndef CANNM_NO_PBCFG_REQUIRED
#define CANNM_NO_PBCFG_REQUIRED
#endif

#include <CanNm_Trace.h>

#include <SchM_CanNm.h>                 /* SchM symbols for crit. sections */

#include <CanNm_Hsm.h>                  /* public API of CanNm_Hsm.c */
#include <CanNm_Int.h>

#if (CANNM_DEV_ERROR_DETECT == STD_ON)
/* !LINKSTO CANNM308,1 */
#include <Det.h>                        /* Det API */
#endif

/* design debugging output */
#if (CANNM_HSM_TRACE == STD_ON)
/* CHECK: RULE 501 OFF (macro needed in std headers) */
/* Deviation MISRAC2012-1 */
#define __NO_ISOCEXT
#include <stdlib.h>
#include <stdio.h>
/* CHECK: RULE 501 ON */
#endif

/*==================[macros]================================================*/

#if (defined CANNM_HSM_DEL_FROM_QUEUE)
#error CANNM_HSM_DEL_FROM_QUEUE already defined
#endif

#if (defined CANNM_HSM_TRAN)
#error CANNM_HSM_TRAN already defined
#endif

#if (defined CANNM_HSM_INIT_SUBSTATES)
#error CANNM_HSM_INIT_SUBSTATES already defined
#endif

#if (defined CANNM_HSM_FIND_EV_TO_DISPATCH)
#error CANNM_HSM_FIND_EV_TO_DISPATCH already defined
#endif

#if (defined CANNM_HSM_DISPATCH_EVENT)
#error CANNM_HSM_DISPATCH_EVENT already defined
#endif

#if (defined CANNM_EV_QUEUE_ENTRY)
#error CANNM_EV_QUEUE_ENTRY already defined
#endif

#if (defined CANNM_EV_COUNTER_ENTRY)
#error CANNM_EV_COUNTER_ENTRY already defined
#endif

#if (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
#define CANNM_HSM_DEL_FROM_QUEUE(a,b,c) CanNm_HsmDelFromQueue(a,b,c)
#define CANNM_HSM_TRAN(a,b,c)           CanNm_HsmTran(a,b,c)
#define CANNM_HSM_INIT_SUBSTATES(a,b)   CanNm_HsmInitSubstates(a,b)
#define CANNM_HSM_FIND_EV_TO_DISPATCH(a,b,c,d) CanNm_HsmFindEvToDispatch(a,b,c,d)
#define CANNM_HSM_DISPATCH_EVENT(a,b,c) CanNm_HsmDispatchEvent(a,b,c)

/** \brief Access entry in event queue of a specific hsm instance */
#define CANNM_EV_QUEUE_ENTRY(sc,instIdx,entryIdx) \
  ((sc)->events[((instIdx)*((sc)->numEvents))+(entryIdx)])
#define CANNM_EV_COUNTER_ENTRY(sc,instIdx,entryIdx) \
  ((sc)->evCounters[((instIdx)*((sc)->numEvents))+(entryIdx)])
#else
#define CANNM_HSM_DEL_FROM_QUEUE(a,b,c) CanNm_HsmDelFromQueue(a,c)
#define CANNM_HSM_TRAN(a,b,c)           CanNm_HsmTran(a,c)
#define CANNM_HSM_INIT_SUBSTATES(a,b)   CanNm_HsmInitSubstates(a)
#define CANNM_HSM_FIND_EV_TO_DISPATCH(a,b,c,d) CanNm_HsmFindEvToDispatch(a,c,d)
#define CANNM_HSM_DISPATCH_EVENT(a,b,c) CanNm_HsmDispatchEvent(a,c)
#define CANNM_EV_QUEUE_ENTRY(sc,instIdx,entryIdx) \
  ((sc)->events[(entryIdx)])
#define CANNM_EV_COUNTER_ENTRY(sc,instIdx,entryIdx) \
  ((sc)->evCounters[(entryIdx)])
#endif

#if (defined CANNM_HSM_PRINT1)
#error CANNM_HSM_PRINT1 already defined
#endif

#if (defined CANNM_HSM_PRINT2)
#error CANNM_HSM_PRINT2 already defined
#endif

#if (defined CANNM_HSM_PRINT3)
#error CANNM_HSM_PRINT3 already defined
#endif

#if (defined CANNM_HSM_PRINT4)
#error CANNM_HSM_PRINT4 already defined
#endif

#if (defined CANNM_HSM_PRINT5)
#error CANNM_HSM_PRINT5 already defined
#endif

#if (CANNM_HSM_TRACE == STD_ON)
#define CANNM_HSM_PRINT1(a)                          \
  do                                                 \
  {                                                  \
    if (CanNm_HsmTraceEnabled == TRUE)               \
    {                                                \
      (void)printf("HsmTrace: " a);                  \
    }                                                \
  } while (0U)
#define CANNM_HSM_PRINT2(a,b)                        \
  do                                                 \
  {                                                  \
    if (CanNm_HsmTraceEnabled == TRUE)               \
    {                                                \
      (void)printf("HsmTrace: " a,(b));              \
    }                                                \
  } while (0U)
/* Deviation MISRAC2012-2 */
#define CANNM_HSM_PRINT3(a,b,c)                      \
  do                                                 \
  {                                                  \
    if (CanNm_HsmTraceEnabled == TRUE)               \
    {                                                \
      (void)printf("HsmTrace: " a,(b),(c));          \
    }                                                \
  } while (0U)
#define CANNM_HSM_PRINT4(a,b,c,d)                    \
  do                                                 \
  {                                                  \
    if (CanNm_HsmTraceEnabled == TRUE)               \
    {                                                \
      (void)printf("HsmTrace: " a,(b),(c),(d));      \
    }                                                \
  } while (0U)
#define CANNM_HSM_PRINT5(a,b,c,d,e)                  \
  do                                                 \
  {                                                  \
    if (CanNm_HsmTraceEnabled == TRUE)               \
    {                                                \
      (void)printf("HsmTrace: " a,(b),(c),(d),(e));  \
    }                                                \
  } while (0U)
#else
#define CANNM_HSM_PRINT1(a)          /* nothing */
#define CANNM_HSM_PRINT2(a,b)        /* nothing */
#define CANNM_HSM_PRINT3(a,b,c)      /* nothing */
#define CANNM_HSM_PRINT4(a,b,c,d)    /* nothing */
#define CANNM_HSM_PRINT5(a,b,c,d,e)  /* nothing */
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define CANNM_START_SEC_CODE
#include <CanNm_MemMap.h>

/** \brief Delete entry from event queue
 ** \param sc pointer to state chart
 ** \param instIdx index of state machine instance to work on
 ** \param queueEntryNo entry in queue to delete
 **
 ** This function must be called inside of a critical section which prevents
 ** the concurrent access to the queue.
 **/
STATIC FUNC(void, CANNM_CODE) CANNM_HSM_DEL_FROM_QUEUE(
  const CanNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const uint8                    queueEntryNo);

/** \brief Execute a transition
 ** \param sc pointer to state chart
 ** \param instIdx index of state machine instance to work on
 ** \param tran Pointer to the transition description struct */
STATIC FUNC(void, CANNM_CODE) CANNM_HSM_TRAN(
  const CanNm_HsmStatechartPtrType                      sc,
  const uint8                                         instIdx,
  CONSTP2CONST(CanNm_HsmTranType, AUTOMATIC, CANNM_CONST) tran);

/** \brief Initialize all substates for the current state
 ** \param sc pointer to state chart to work on
 ** \param instIdx index of state machine instance to work on */
STATIC FUNC(void, CANNM_CODE) CANNM_HSM_INIT_SUBSTATES(
  const CanNm_HsmStatechartPtrType sc,
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
STATIC FUNC(boolean, CANNM_CODE) CANNM_HSM_FIND_EV_TO_DISPATCH(
  const CanNm_HsmStatechartPtrType                     sc,
  const uint8                                        instIdx,
  CONSTP2VAR(CanNm_HsmEventType, AUTOMATIC, AUTOMATIC) event,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)            queueEntryNo);

/** \brief Dispatch event from event queue
 ** \param sc pointer to state chart to work on
 ** \param instIdx index of state machine instance to work on
 ** \param event identified event from queue
 ** \return Transition fired indicator
 ** \retval TRUE the event dispatching resulted in an fired transition
 ** \retval FALSE else */
STATIC FUNC(boolean, CANNM_CODE) CANNM_HSM_DISPATCH_EVENT(
  const CanNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const CanNm_HsmEventType         event);

#define CANNM_STOP_SEC_CODE
#include <CanNm_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#if (CANNM_HSM_TRACE == STD_ON)

#define CANNM_START_SEC_VAR_INIT_8
#include <CanNm_MemMap.h>

STATIC VAR(boolean, CANNM_VAR) CanNm_HsmTraceEnabled = FALSE;

#define CANNM_STOP_SEC_VAR_INIT_8
#include <CanNm_MemMap.h>

#endif

/*==================[external function definitions]=========================*/

#define CANNM_START_SEC_CODE
#include <CanNm_MemMap.h>

/*------------------ init functions ----------------------------------------*/

#if (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
FUNC(void, CANNM_CODE) CanNm_HsmInit(const CanNm_HsmStatechartPtrType sc)
{
  uint8 instIdx;
  for (instIdx = 0U; instIdx < sc->numInstances; ++instIdx)
  {
    CANNM_HSMINITINST(sc, instIdx);
  }
}
/* else: function is implemented as macro and maps to CanNm_HsmInitInst() */
#endif

FUNC(void, CANNM_CODE) CANNM_HSMINITINST(
  const CanNm_HsmStatechartPtrType sc,
  const uint8                    instIdx)
{
  CONSTP2VAR(CanNm_HsmInstType, AUTOMATIC, CANNM_VAR_CLEARED) inst
    = &CANNM_INST_ACCESS(sc->instances, instIdx);
  uint8 i;

  CANNM_HSM_PRINT3("%s %d Init\n", sc->hsmName, CANNM_INST(instIdx));

  /* init all fields because the struct is placed in a NOINIT memory
   * section */
  inst->stateId  = sc->topStateId;
  inst->sourceId = CANNM_HSM_SIDX_INVALID;

  for (i = 0U; i < sc->numEvents; ++i)
  {
    CANNM_EV_QUEUE_ENTRY(sc, instIdx, i) = 0U;
    CANNM_EV_COUNTER_ENTRY(sc, instIdx, i) = 0U;
  }
  inst->evQueueFill       = 0U;
  inst->evQueueInsertEvNo = 0U;

/* CHECK: NOPARSE */
/* No parse has been added because with the current state machine
 * configuration (eap file) top state has an entry function
 */
  if (sc->states[inst->stateId].entryFnIdx != CANNM_HSM_AIDX_INVALID)
  {
    /* Deviation MISRAC2012-2 */
    CANNM_HSM_PRINT4(
      "%s %d %s\n",
      sc->hsmName, CANNM_INST(instIdx), sc->actionNames[inst->stateId]);

    /* enter top state */
    sc->actions[sc->states[inst->stateId].entryFnIdx](CANNM_PL_SF(instIdx));
  }
/* CHECK: PARSE */
  /* init all substates */
  CANNM_HSM_INIT_SUBSTATES(sc, instIdx);
}

/*------------------ emit functions ----------------------------------------*/
/* CHECK: NOPARSE */
/* No parse has been added because function is not used
 */
#if (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
FUNC(void, CANNM_CODE) CanNm_HsmEmit(
  const CanNm_HsmStatechartPtrType sc,
  const CanNm_HsmEventType         event)
{
  uint8 instIdx;

  for (instIdx = 0U; instIdx < sc->numInstances; ++instIdx)
  {
    CANNM_HSMEMITINST(sc, instIdx, event);
  }
}
/* else: function is implemented as macro */
#endif
/* CHECK: PARSE */

FUNC(void, CANNM_CODE) CANNM_HSMEMITINST(
  const CanNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const CanNm_HsmEventType         event)
{
  CONSTP2VAR(CanNm_HsmInstType, AUTOMATIC, CANNM_VAR_CLEARED) inst
    = &CANNM_INST_ACCESS(sc->instances, instIdx);

  SchM_Enter_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();
  /* in case the event has already been queued don't add it again to the queue*/
  /* !LINKSTO CanNm.EB.EventQueue,1 */
  if ( CANNM_EV_COUNTER_ENTRY(sc, instIdx, event) < 1U)
  {
    CANNM_EV_QUEUE_ENTRY(sc, instIdx, inst->evQueueFill) = event;
    inst->evQueueFill++;
    /* leave evQueueInsertEvNo unchanged, this pointer will be updated on
     * insertion or event dispatching */
    ++CANNM_EV_COUNTER_ENTRY(sc, instIdx, event);
  }
  SchM_Exit_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();

  /* Deviation MISRAC2012-2 */
  CANNM_HSM_PRINT4(
    "%s %d event %s enqueued\n",
    sc->hsmName, CANNM_INST(instIdx), sc->eventNames[event]);

}

FUNC(void, CANNM_CODE) CANNM_HSMEMITTOSELFINST(
  const CanNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const CanNm_HsmEventType         event)
{
  CONSTP2VAR(CanNm_HsmInstType, AUTOMATIC, CANNM_VAR_CLEARED) inst
    = &CANNM_INST_ACCESS(sc->instances, instIdx);

  SchM_Enter_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();
  if ( CANNM_EV_COUNTER_ENTRY(sc, instIdx, event) < 1U)
  {
    /* make room for new entry */
    uint8 j;
    for (j = inst->evQueueFill; j > inst->evQueueInsertEvNo; --j)
    {
      CANNM_EV_QUEUE_ENTRY(sc, instIdx, j) = CANNM_EV_QUEUE_ENTRY(sc, instIdx, j-1U);
    }
    ++inst->evQueueFill;
    ++CANNM_EV_COUNTER_ENTRY(sc, instIdx, event);
    CANNM_EV_QUEUE_ENTRY(sc, instIdx, inst->evQueueInsertEvNo) = event;
    inst->evQueueInsertEvNo++;
  }
  SchM_Exit_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();

  /* Deviation MISRAC2012-2 */
  CANNM_HSM_PRINT4(
    "%s %d event %s enqueued internally\n",
    sc->hsmName, CANNM_INST(instIdx), sc->eventNames[event]);
}

/*------------------ main functions ----------------------------------------*/

#if (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
FUNC(void, CANNM_CODE) CanNm_HsmMain(const CanNm_HsmStatechartPtrType sc)
{
  boolean transitionFired;

  do
  {
    uint8 instIdx;
    transitionFired = FALSE;
    /* for all instances */
    for (instIdx = 0U; instIdx < sc->numInstances; ++instIdx)
    {
      const boolean retVal = CanNm_HsmMainInst(sc, instIdx);
      transitionFired = (retVal || transitionFired);
    }
  } while (transitionFired);
}
/* else: function is implemented as macro and maps to CanNm_HsmMainInst() */
#endif
FUNC(boolean, CANNM_CODE) CANNM_HSMMAININST(
  const CanNm_HsmStatechartPtrType sc,
  const uint8                    instIdx)
{
  CONSTP2VAR(CanNm_HsmInstType, AUTOMATIC, CANNM_VAR_CLEARED) inst
    = &CANNM_INST_ACCESS(sc->instances,instIdx);
  /* return value, flag indicating if at least one transition fired */
  boolean          transitionFired = FALSE;
  CanNm_HsmEventType event;
  uint8            queueEntryNo;
  boolean          eventPending;

  /* Deviation MISRAC2012-2 */
  CANNM_HSM_PRINT3(
    "%s %d Main\n",
    sc->hsmName, CANNM_INST(instIdx));

  /* reset event insertion pointer to an invalid value, this will be set to a
   * valid location if the first event is being dispatched */
  inst->evQueueInsertEvNo = 0U;

  /* check if there are is at least one pending event */
  eventPending
    = CANNM_HSM_FIND_EV_TO_DISPATCH(sc, instIdx, &event, &queueEntryNo);

  /* dispatch all pending events from event queue */
  while (TRUE == eventPending)
  {
    boolean retVal;
    /* delete event from queue */
    SchM_Enter_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();
    CANNM_HSM_DEL_FROM_QUEUE(sc, instIdx, queueEntryNo);
    SchM_Exit_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();

    /* dispatch event */
    retVal = CANNM_HSM_DISPATCH_EVENT(sc, instIdx, event);
    transitionFired = (retVal || transitionFired);



    eventPending
      = CANNM_HSM_FIND_EV_TO_DISPATCH(sc, instIdx, &event, &queueEntryNo);
  }


  return transitionFired;
}

#if (CANNM_HSM_TRACE == STD_ON)
FUNC(void, CANNM_CODE) CanNm_HsmSetTracing(const boolean newValue)
{
  CanNm_HsmTraceEnabled = newValue;
}
#endif

#define CANNM_STOP_SEC_CODE
#include <CanNm_MemMap.h>

/*==================[internal function definitions]=========================*/

#define CANNM_START_SEC_CODE
#include <CanNm_MemMap.h>

STATIC FUNC(void, CANNM_CODE) CANNM_HSM_DEL_FROM_QUEUE(
  const CanNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const uint8                    queueEntryNo)
{
  /* delete event no queueEntryNo from queue */
  CONSTP2VAR(CanNm_HsmInstType, AUTOMATIC, CANNM_VAR_CLEARED) inst
    = &CANNM_INST_ACCESS(sc->instances, instIdx);
  uint8 j;

  /* reduce the occurrence counter of the respective event */
  CanNm_HsmEventType event = CANNM_EV_QUEUE_ENTRY(sc, instIdx, queueEntryNo);
  --CANNM_EV_COUNTER_ENTRY(sc, instIdx, event);

  /* Deviation MISRAC2012-3 */
  for (j = queueEntryNo; (j + 1U) < inst->evQueueFill; ++j)
  {
    CANNM_EV_QUEUE_ENTRY(sc, instIdx, j)
      = CANNM_EV_QUEUE_ENTRY(sc, instIdx, j + 1U);
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

STATIC FUNC(void, CANNM_CODE) CANNM_HSM_TRAN(
  const CanNm_HsmStatechartPtrType                      sc,
  const uint8                                         instIdx,
  CONSTP2CONST(CanNm_HsmTranType, AUTOMATIC, CANNM_CONST) tran)
{
  P2CONST(CanNm_HsmStateType, AUTOMATIC, CANNM_CONST) state;
  CONSTP2VAR(CanNm_HsmInstType, AUTOMATIC, CANNM_VAR_CLEARED) inst
      = &CANNM_INST_ACCESS(sc->instances, instIdx);

  /* perform simplified transition for internal transitions */
  if (tran->target == CANNM_HSM_SIDX_INVALID)
  {
    /* Deviation MISRAC2012-2 */
    CANNM_HSM_PRINT4(
      "%s %d %s (internal transition)\n",
      sc->hsmName, CANNM_INST(instIdx), sc->actionNames[tran->steps[0U]]);
    sc->actions[tran->steps[0U]](CANNM_PL_SF(instIdx));
  }
  else /* external transitions */
  {
    uint8_least i;

    /* "source" may be a superstate of "state".  exit all states from "state"
     * until "source" is reached */
    while (inst->stateId != inst->sourceId)
    {
      state = &sc->states[inst->stateId];

      if (state->exitFnIdx != CANNM_HSM_AIDX_INVALID)
      {
        /* Deviation MISRAC2012-2 */
        CANNM_HSM_PRINT4(
          "%s %d %s\n",
          sc->hsmName, CANNM_INST(instIdx), sc->actionNames[state->exitFnIdx]);
        sc->actions[state->exitFnIdx](CANNM_PL_SF(instIdx));
      }

      inst->stateId = state->parentId;
    }

    /* execute exit/transiton/entry actions on transition path */
    for (i = 0U; i < tran->numSteps; ++i)
    {
      /* Deviation MISRAC2012-2 */
      CANNM_HSM_PRINT4(
        "%s %d %s\n",
        sc->hsmName, CANNM_INST(instIdx), sc->actionNames[tran->steps[i]]);
      sc->actions[tran->steps[i]](CANNM_PL_SF(instIdx));
    }

    /* set the new current state and perform init steps of substates */
    inst->stateId = tran->target; /* current state is last entered state */
    CANNM_HSM_INIT_SUBSTATES(sc, instIdx);

    /* Deviation MISRAC2012-2 */
    CANNM_HSM_PRINT4(
      "%s %d transition to state %s finished\n",
      sc->hsmName, CANNM_INST(instIdx), sc->stateNames[inst->stateId]);
  }

}

STATIC FUNC(void, CANNM_CODE) CANNM_HSM_INIT_SUBSTATES(
  const CanNm_HsmStatechartPtrType sc,
  const uint8                    instIdx)
{
  CONSTP2VAR(CanNm_HsmInstType, AUTOMATIC, CANNM_VAR_CLEARED) inst
    = &CANNM_INST_ACCESS(sc->instances,instIdx);

  CanNm_HsmStateIdxType stateIdx = sc->states[inst->stateId].initId;

  while (stateIdx != CANNM_HSM_SIDX_INVALID) /* init state if necessary */
  {
    P2CONST(CanNm_HsmStateType, AUTOMATIC, CANNM_CONST) state
      = &sc->states[stateIdx];

    inst->stateId = stateIdx; /* set new current state */
    /* enter the substate */
/* CHECK: NOPARSE */
/* No parse has been added because with the current state machine
 * configuration (eap file), statement (state->entryFnIdx != CANNM_HSM_AIDX_INVALID)
 * is never evaluated to FALSE
 */
    if (state->entryFnIdx != CANNM_HSM_AIDX_INVALID)
    {
      /* enter the substate */
      /* Deviation MISRAC2012-2 */
      CANNM_HSM_PRINT4(
        "%s %d %s\n",
        sc->hsmName, CANNM_INST(instIdx), sc->actionNames[state->entryFnIdx]);
      sc->actions[state->entryFnIdx](CANNM_PL_SF(instIdx));
    }
/* CHECK: PARSE */
    /* get next init state */
    stateIdx = state->initId;
  }
}

STATIC FUNC(boolean, CANNM_CODE) CANNM_HSM_FIND_EV_TO_DISPATCH(
  const CanNm_HsmStatechartPtrType                     sc,
  const uint8                                        instIdx,
  CONSTP2VAR(CanNm_HsmEventType, AUTOMATIC, AUTOMATIC) event,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)            queueEntryNo)
{
    CONSTP2VAR(CanNm_HsmInstType, AUTOMATIC, CANNM_VAR_CLEARED) inst
      = &CANNM_INST_ACCESS(sc->instances,instIdx);
    CONSTP2CONST(CanNm_HsmStateType, AUTOMATIC, CANNM_CONST) state
      = &sc->states[inst->stateId];
    boolean          foundEventToDispatch = FALSE;
    CanNm_HsmEventType ev                   = CANNM_HSM_EVENT_INVALID;
    uint8            entryNo              = 0U;

    /* lock event queue access */
    SchM_Enter_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();
    while ((entryNo < inst->evQueueFill)
           && (FALSE == foundEventToDispatch))
    {
      CanNm_HsmEventMaskType eventMask;
      ev        = CANNM_EV_QUEUE_ENTRY(sc, instIdx, entryNo);
      eventMask = (CanNm_HsmEventMaskType)1UL << ev;
/* CHECK: NOPARSE */
/* No parse has been added because:
 *  - statement if (eventMask & state->actionEventMask) != 0U)
 *    is never evaluated to FALSE since there are no deferred events.
 */
      if ((eventMask & state->ignoredEventMask) != 0U)
      {
        /* Deviation MISRAC2012-2 */
        CANNM_HSM_PRINT4(
          "%s %d event %s ignored\n",
          sc->hsmName, CANNM_INST(instIdx), sc->eventNames[ev]);

        /* delete event no 'entryNo' from queue */
        CANNM_HSM_DEL_FROM_QUEUE(sc, instIdx, entryNo);
        /* do not increment entryNo */
      }
      else if ((eventMask & state->actionEventMask) != 0U)
      {
        if (inst->evQueueInsertEvNo <= entryNo)
        {
          /* if the insertion location is still unset (or pointing to an
           * invalid location) then set the insertion location to just after
           * the current event to be dispatched. */
          inst->evQueueInsertEvNo = entryNo + 1U;
        }
        /* leave while loop and dispatch event no entryNo */
        foundEventToDispatch = TRUE;
      }
      else
      {
        /* Deviation MISRAC2012-2 */
        CANNM_HSM_PRINT4(
          "%s %d event %s deferred\n",
          sc->hsmName, CANNM_INST(instIdx), sc->eventNames[ev]);

        /* found only deferred event, try next one in queue */
        ++entryNo;
      }
/* CHECK: PARSE */
    }
    SchM_Exit_CanNm_SCHM_CANNM_EXCLUSIVE_AREA_0();

    *event        = ev;
    *queueEntryNo = entryNo;
    return foundEventToDispatch;
}

STATIC FUNC(boolean, CANNM_CODE) CANNM_HSM_DISPATCH_EVENT(
  const CanNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const CanNm_HsmEventType         event)
{
  CONSTP2VAR(CanNm_HsmInstType, AUTOMATIC, CANNM_VAR_CLEARED) inst
    = &CANNM_INST_ACCESS(sc->instances,instIdx);
  P2CONST(CanNm_HsmTranType, AUTOMATIC, CANNM_CONST) trans = NULL_PTR;
  /* return value, flag indicating if at least one transition fired */
  boolean transitionFired = FALSE;
  /* flag for the trigger search algorithm */
  boolean triggerFound    = FALSE;

  /* *** search for transition for non-deferred and non-ignored event and
   * fire the transition if found *** */

  /* Deviation MISRAC2012-2 */
  CANNM_HSM_PRINT5(
    "%s %d dispatching event %s in state %s\n",
    sc->hsmName, CANNM_INST(instIdx), sc->eventNames[event],
    sc->stateNames[inst->stateId]);

  /* try current leaf state as src of transitions */
  inst->sourceId = inst->stateId;

  /* start searching for a triggering event */
  do
  {
    uint8_least i;          /* index in transition array of state */

    CONSTP2CONST(CanNm_HsmStateType, AUTOMATIC, CANNM_CONST) state
      = &sc->states[inst->sourceId];

    /* check all possible transitions of state */
    for (i = 0U; (i < state->numTrans) && (FALSE == triggerFound); ++i)
    {
      trans = &state->trans[i]; /* test next transition */

      if (event == trans->event)
      {
        /* evaluate guard condition */
        boolean guardCond = TRUE;

        if (trans->guardFnIdx != CANNM_HSM_GIDX_INVALID)
        {
          guardCond = sc->guards[trans->guardFnIdx](CANNM_PL_SF(instIdx));
          /* Deviation MISRAC2012-2 */
          CANNM_HSM_PRINT5(
            "%s %d %s evaluates to %s\n",
            sc->hsmName, CANNM_INST(instIdx),
            sc->guardNames[trans->guardFnIdx], (guardCond?"TRUE":"FALSE"));
        }

        if (TRUE == guardCond)
        {
          /* exit for loop and middle do-while loop in any case because we
           * have found at least one enabled transition for this event. */
          triggerFound    = TRUE;

          /* remember that some real event processing e.g. a state machine
           * action happened. In this case a subsequent call of this or other
           * dispatching functions may be necessary */
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
           && (CANNM_HSM_SIDX_INVALID != inst->sourceId));

  /* a triggering event was found and transition is ready to be executed or
   * the top state is reached the queued event must be ignored
   *
   * we have to check if the top state is not yet reached at runtime because
   * there may still be non-ignored and not handled events due to false
   * guard conditions */

  if (TRUE == triggerFound)
  {
    CANNM_HSM_TRAN(sc, instIdx, trans); /* execute the transition */
  }
  else
  {
    /* Deviation MISRAC2012-2 */
    CANNM_HSM_PRINT4(
      "%s %d event %s ignored at top state\n",
      sc->hsmName, CANNM_INST(instIdx), sc->eventNames[event]);
  }

  return transitionFired;
}

#define CANNM_STOP_SEC_CODE
#include <CanNm_MemMap.h>

/*==================[end of file]===========================================*/
