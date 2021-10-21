/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
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

#define FRNM_NO_CFGCLASSMIX_REQUIRED
#ifndef FRNM_NO_PBCFG_REQUIRED
#define FRNM_NO_PBCFG_REQUIRED
#endif

#include <FrNm_Trace.h>

#include <SchM_FrNm.h>           /* SchM symbols for crit. sections */

#include <FrNm_Hsm.h>            /* public API of FrNm_Hsm.c */

#include <FrNm_Int.h>

/* design debugging output */
#if (FRNM_HSM_TRACE == STD_ON)
/* CHECK: RULE 501 OFF (macro needed in std headers) */
/* Deviation MISRAC2012-1 */
#define __NO_ISOCEXT
#include <stdlib.h>
#include <stdio.h>
/* CHECK: RULE 501 ON */
#endif

/*==================[macros]================================================*/

#if (defined FRNM_HSM_DEL_FROM_QUEUE)
#error FRNM_HSM_DEL_FROM_QUEUE already defined
#endif

#if (defined FRNM_HSM_TRAN)
#error FRNM_HSM_TRAN already defined
#endif

#if (defined FRNM_HSM_INIT_SUBSTATES)
#error FRNM_HSM_INIT_SUBSTATES already defined
#endif

#if (defined FRNM_HSM_FIND_EV_TO_DISPATCH)
#error FRNM_HSM_FIND_EV_TO_DISPATCH already defined
#endif

#if (defined FRNM_HSM_DISPATCH_EVENT)
#error FRNM_HSM_DISPATCH_EVENT already defined
#endif

#if (defined FRNM_EV_QUEUE_ENTRY)
#error FRNM_EV_QUEUE_ENTRY already defined
#endif

#if (defined FRNM_EV_COUNTER_ENTRY)
#error FRNM_EV_COUNTER_ENTRY already defined
#endif

#if (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
#define FRNM_HSM_DEL_FROM_QUEUE(a,b,c) FrNm_HsmDelFromQueue(a,b,c)
#define FRNM_HSM_TRAN(a,b,c)           FrNm_HsmTran(a,b,c)
#define FRNM_HSM_INIT_SUBSTATES(a,b)   FrNm_HsmInitSubstates(a,b)
#define FRNM_HSM_FIND_EV_TO_DISPATCH(a,b,c,d) FrNm_HsmFindEvToDispatch(a,b,c,d)
#define FRNM_HSM_DISPATCH_EVENT(a,b,c) FrNm_HsmDispatchEvent(a,b,c)

/** \brief Access entry in event queue of a specific hsm instance */
#define FRNM_EV_QUEUE_ENTRY(sc,instIdx,entryIdx) \
  ((sc)->events[((instIdx)*((sc)->numEvents))+(entryIdx)])
#define FRNM_EV_COUNTER_ENTRY(sc,instIdx,entryIdx) \
  ((sc)->evCounters[((instIdx)*((sc)->numEvents))+(entryIdx)])
#else
#define FRNM_HSM_DEL_FROM_QUEUE(a,b,c) FrNm_HsmDelFromQueue(a,c)
#define FRNM_HSM_TRAN(a,b,c)           FrNm_HsmTran(a,c)
#define FRNM_HSM_INIT_SUBSTATES(a,b)   FrNm_HsmInitSubstates(a)
#define FRNM_HSM_FIND_EV_TO_DISPATCH(a,b,c,d) FrNm_HsmFindEvToDispatch(a,c,d)
#define FRNM_HSM_DISPATCH_EVENT(a,b,c) FrNm_HsmDispatchEvent(a,c)
#define FRNM_EV_QUEUE_ENTRY(sc,instIdx,entryIdx) \
  ((sc)->events[(entryIdx)])
#define FRNM_EV_COUNTER_ENTRY(sc,instIdx,entryIdx) \
  ((sc)->evCounters[(entryIdx)])
#endif

#if (defined FRNM_HSM_PRINT1)
#error FRNM_HSM_PRINT1 already defined
#endif

#if (defined FRNM_HSM_PRINT2)
#error FRNM_HSM_PRINT2 already defined
#endif

#if (defined FRNM_HSM_PRINT3)
#error FRNM_HSM_PRINT3 already defined
#endif

#if (defined FRNM_HSM_PRINT4)
#error FRNM_HSM_PRINT4 already defined
#endif

#if (defined FRNM_HSM_PRINT5)
#error FRNM_HSM_PRINT5 already defined
#endif

#if (FRNM_HSM_TRACE == STD_ON)
#define FRNM_HSM_PRINT1(a)                            \
  do                                                  \
  {                                                   \
    if (FrNm_HsmTraceEnabled == TRUE)                 \
    {                                                 \
      (void)printf("HsmTrace: " a);                   \
    }                                                 \
  } while (0U)
#define FRNM_HSM_PRINT2(a,b)                          \
  do                                                  \
  {                                                   \
    if (FrNm_HsmTraceEnabled == TRUE)                 \
    {                                                 \
      (void)printf("HsmTrace: " a,(b));               \
    }                                                 \
  } while (0U)
/* Deviation MISRAC2012-2 */
#define FRNM_HSM_PRINT3(a,b,c)                        \
  do                                                  \
  {                                                   \
    if (FrNm_HsmTraceEnabled == TRUE)                 \
    {                                                 \
      (void)printf("HsmTrace: " a,(b),(c));           \
    }                                                 \
  } while (0U)
#define FRNM_HSM_PRINT4(a,b,c,d)                      \
  do                                                  \
  {                                                   \
    if (FrNm_HsmTraceEnabled == TRUE)                 \
    {                                                 \
      (void)printf("HsmTrace: " a,(b),(c),(d));       \
    }                                                 \
  } while (0U)
#define FRNM_HSM_PRINT5(a,b,c,d,e)                    \
  do                                                  \
  {                                                   \
    if (FrNm_HsmTraceEnabled == TRUE)                 \
    {                                                 \
      (void)printf("HsmTrace: " a,(b),(c),(d),(e));   \
    }                                                 \
  } while (0U)
#else
#define FRNM_HSM_PRINT1(a)          /* nothing */
#define FRNM_HSM_PRINT2(a,b)        /* nothing */
#define FRNM_HSM_PRINT3(a,b,c)      /* nothing */
#define FRNM_HSM_PRINT4(a,b,c,d)    /* nothing */
#define FRNM_HSM_PRINT5(a,b,c,d,e)  /* nothing */
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define FRNM_START_SEC_CODE
#include <FrNm_MemMap.h>

/** \brief Delete entry from event queue
 ** \param sc pointer to state chart
 ** \param instIdx index of state machine instance to work on
 ** \param queueEntryNo entry in queue to delete
 **
 ** This function must be called inside of a critical section which prevents
 ** the concurrent access to the queue.
 **/
STATIC FUNC(void, FRNM_CODE) FRNM_HSM_DEL_FROM_QUEUE(
  const FrNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const uint8                    queueEntryNo);

/** \brief Execute a transition
 ** \param sc pointer to state chart
 ** \param instIdx index of state machine instance to work on
 ** \param tran Pointer to the transition description struct */
STATIC FUNC(void, FRNM_CODE) FRNM_HSM_TRAN(
  const FrNm_HsmStatechartPtrType                      sc,
  const uint8                                         instIdx,
  CONSTP2CONST(FrNm_HsmTranType, AUTOMATIC, FRNM_CONST) tran);

/** \brief Initialize all substates for the current state
 ** \param sc pointer to state chart to work on
 ** \param instIdx index of state machine instance to work on */
STATIC FUNC(void, FRNM_CODE) FRNM_HSM_INIT_SUBSTATES(
  const FrNm_HsmStatechartPtrType sc,
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
STATIC FUNC(boolean, FRNM_CODE) FRNM_HSM_FIND_EV_TO_DISPATCH(
  const FrNm_HsmStatechartPtrType                     sc,
  const uint8                                        instIdx,
  CONSTP2VAR(FrNm_HsmEventType, AUTOMATIC, AUTOMATIC) event,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)            queueEntryNo);

/** \brief Dispatch event from event queue
 ** \param sc pointer to state chart to work on
 ** \param instIdx index of state machine instance to work on
 ** \param event identified event from queue
 ** \return Transition fired indicator
 ** \retval TRUE the event dispatching resulted in an fired transition
 ** \retval FALSE else */
STATIC FUNC(boolean, FRNM_CODE) FRNM_HSM_DISPATCH_EVENT(
  const FrNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const FrNm_HsmEventType         event);

#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#if (FRNM_HSM_TRACE == STD_ON)

#define FRNM_START_SEC_VAR_INIT_8
#include <FrNm_MemMap.h>

STATIC VAR(boolean, FRNM_VAR) FrNm_HsmTraceEnabled = FALSE;

#define FRNM_STOP_SEC_VAR_INIT_8
#include <FrNm_MemMap.h>

#endif

/*==================[external function definitions]=========================*/

#define FRNM_START_SEC_CODE
#include <FrNm_MemMap.h>

/*------------------ init functions ----------------------------------------*/

#if (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
FUNC(void, FRNM_CODE) FrNm_HsmInit(const FrNm_HsmStatechartPtrType sc)
{
  uint8 instIdx;
  for (instIdx = 0U; instIdx < sc->numInstances; ++instIdx)
  {
    FRNM_HSMINITINST(sc, instIdx);
  }
}
/* else: function is implemented as macro and maps to FrNm_HsmInitInst() */
#endif

FUNC(void, FRNM_CODE) FRNM_HSMINITINST(
  const FrNm_HsmStatechartPtrType sc,
  const uint8                    instIdx)
{
  CONSTP2VAR(FrNm_HsmInstType, AUTOMATIC, FRNM_VAR_CLEARED) inst
    = &FRNM_INST_ACCESS(sc->instances, instIdx);
  uint8 i;

  FRNM_HSM_PRINT3("%s %d Init\n", sc->hsmName, FRNM_INST(instIdx));

  /* init all fields because the struct is placed in a NOINIT memory
   * section */
  inst->stateId  = sc->topStateId;
  inst->sourceId = FRNM_HSM_SIDX_INVALID;

  for (i = 0U; i < sc->numEvents; ++i)
  {
    FRNM_EV_QUEUE_ENTRY(sc, instIdx, i) = 0U;
    FRNM_EV_COUNTER_ENTRY(sc, instIdx, i) = 0U;
  }
  inst->evQueueFill       = 0U;
  inst->evQueueInsertEvNo = 0U;

/* CHECK: NOPARSE */
/* No parse have been added because with the current state machine
 * configuration (eap file) top state has an entry function
 */
  if (sc->states[inst->stateId].entryFnIdx != FRNM_HSM_AIDX_INVALID)
  {
    /* Deviation MISRAC2012-2 */
    FRNM_HSM_PRINT4(
      "%s %d %s\n",
      sc->hsmName, FRNM_INST(instIdx), sc->actionNames[inst->stateId]);

    /* enter top state */
    sc->actions[sc->states[inst->stateId].entryFnIdx](FRNM_PL_SF(instIdx));
  }
/* CHECK: PARSE */
  /* init all substates */
  FRNM_HSM_INIT_SUBSTATES(sc, instIdx);
}

/*------------------ emit functions ----------------------------------------*/

/* CHECK: NOPARSE */
/* No parse have been added because this function is not used.
 */
#if (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
FUNC(void, FRNM_CODE) FrNm_HsmEmit(
  const FrNm_HsmStatechartPtrType sc,
  const FrNm_HsmEventType         event)
{
  uint8 instIdx;

  for (instIdx = 0U; instIdx < sc->numInstances; ++instIdx)
  {
    FRNM_HSMEMITINST(sc, instIdx, event);
  }
}
/* else: function is implemented as macro */
#endif
/* CHECK: PARSE */
FUNC(void, FRNM_CODE) FRNM_HSMEMITINST(
  const FrNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const FrNm_HsmEventType         event)
{
  CONSTP2VAR(FrNm_HsmInstType, AUTOMATIC, FRNM_VAR_CLEARED) inst
    = &FRNM_INST_ACCESS(sc->instances, instIdx);

  SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
  /* in case the event has already been queued don't add it again to the queue*/
  if ( FRNM_EV_COUNTER_ENTRY(sc, instIdx, event) < 1U)
  {
    FRNM_EV_QUEUE_ENTRY(sc, instIdx, inst->evQueueFill) = event;
    inst->evQueueFill++;
    /* leave evQueueInsertEvNo unchanged, this pointer will be updated on
     * insertion or event dispatching */
    ++FRNM_EV_COUNTER_ENTRY(sc, instIdx, event);
  }
  SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

  /* Deviation MISRAC2012-2 */
  FRNM_HSM_PRINT4(
    "%s %d event %s enqueued\n",
    sc->hsmName, FRNM_INST(instIdx), sc->eventNames[event]);

}

FUNC(void, FRNM_CODE) FRNM_HSMEMITTOSELFINST(
  const FrNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const FrNm_HsmEventType         event)
{
  CONSTP2VAR(FrNm_HsmInstType, AUTOMATIC, FRNM_VAR_CLEARED) inst
    = &FRNM_INST_ACCESS(sc->instances, instIdx);

  SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
/* CHECK: NOPARSE */
/* EmitToSelfInst is used only for FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED event.
 * Event FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED cannot be in the queue twice
 */
  if ( FRNM_EV_COUNTER_ENTRY(sc, instIdx, event) < 1U)
  {
    /* make room for new entry */
    uint8 j;
    for (j = inst->evQueueFill; j > inst->evQueueInsertEvNo; --j)
    {
      FRNM_EV_QUEUE_ENTRY(sc, instIdx, j) = FRNM_EV_QUEUE_ENTRY(sc, instIdx, j-1U);
    }
    ++inst->evQueueFill;
    ++FRNM_EV_COUNTER_ENTRY(sc, instIdx, event);
    FRNM_EV_QUEUE_ENTRY(sc, instIdx, inst->evQueueInsertEvNo) = event;
    inst->evQueueInsertEvNo++;
  }
/* CHECK: PARSE */
  SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

  /* Deviation MISRAC2012-2 */
  FRNM_HSM_PRINT4(
    "%s %d event %s enqueued internally\n",
    sc->hsmName, FRNM_INST(instIdx), sc->eventNames[event]);
}

/*------------------ main functions ----------------------------------------*/
/* CHECK: NOPARSE */
/* No parse have been added because this function is not used.
 */
#if (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
FUNC(void, FRNM_CODE) FrNm_HsmMain(const FrNm_HsmStatechartPtrType sc)
{
  boolean transitionFired;

  do
  {
    uint8 instIdx;
    transitionFired = FALSE;
    /* for all instances */
    for (instIdx = 0U; instIdx < sc->numInstances; ++instIdx)
    {
      const boolean retVal = FrNm_HsmMainInst(sc, instIdx);
      transitionFired = (retVal || transitionFired);
    }
  } while (transitionFired);
}
/* else: function is implemented as macro and maps to FrNm_HsmMainInst() */
#endif
/* CHECK: PARSE */
FUNC(boolean, FRNM_CODE) FRNM_HSMMAININST(
  const FrNm_HsmStatechartPtrType sc,
  const uint8                    instIdx)
{
  CONSTP2VAR(FrNm_HsmInstType, AUTOMATIC, FRNM_VAR_CLEARED) inst
    = &FRNM_INST_ACCESS(sc->instances,instIdx);
  /* return value, flag indicating if at least one transition fired */
  boolean          transitionFired = FALSE;
  FrNm_HsmEventType event;
  uint8            queueEntryNo;
  boolean          eventPending;

  /* Deviation MISRAC2012-2 */
  FRNM_HSM_PRINT3(
    "%s %d Main\n",
    sc->hsmName, FRNM_INST(instIdx));

  /* reset event insertion pointer to an invalid value, this will be set to a
   * valid location if the first event is being dispatched */
  inst->evQueueInsertEvNo = 0U;

  /* check if there are is at least one pending event */
  eventPending
    = FRNM_HSM_FIND_EV_TO_DISPATCH(sc, instIdx, &event, &queueEntryNo);

  /* dispatch all pending events from event queue */
  while (TRUE == eventPending)
  {
    boolean retVal;
    /* delete event from queue */
    SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
    FRNM_HSM_DEL_FROM_QUEUE(sc, instIdx, queueEntryNo);
    SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

    /* dispatch event */
    retVal = FRNM_HSM_DISPATCH_EVENT(sc, instIdx, event);
    transitionFired = (retVal || transitionFired);

    if (FrNm_DoActFkp[FRNM_INST(instIdx)] != NULL_PTR)
    {
      FrNm_DoActFkp[FRNM_INST(instIdx)](FRNM_INST(instIdx));
    }

    eventPending
      = FRNM_HSM_FIND_EV_TO_DISPATCH(sc, instIdx, &event, &queueEntryNo);
  }


  return transitionFired;
}

#if (FRNM_HSM_TRACE == STD_ON)
FUNC(void, FRNM_CODE) FrNm_HsmSetTracing(const boolean newValue)
{
  FrNm_HsmTraceEnabled = newValue;
}
#endif

#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>

/*==================[internal function definitions]=========================*/

#define FRNM_START_SEC_CODE
#include <FrNm_MemMap.h>

STATIC FUNC(void, FRNM_CODE) FRNM_HSM_DEL_FROM_QUEUE(
  const FrNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const uint8                    queueEntryNo)
{
  /* delete event no queueEntryNo from queue */
  CONSTP2VAR(FrNm_HsmInstType, AUTOMATIC, FRNM_VAR_CLEARED) inst
    = &FRNM_INST_ACCESS(sc->instances, instIdx);
  uint8 j;

  /* reduce the occurrence counter of the respective event */
  FrNm_HsmEventType event = FRNM_EV_QUEUE_ENTRY(sc, instIdx, queueEntryNo);
  --FRNM_EV_COUNTER_ENTRY(sc, instIdx, event);

  /* Deviation MISRAC2012-3 */
  for (j = queueEntryNo; (j + 1U) < inst->evQueueFill; ++j)
  {
    FRNM_EV_QUEUE_ENTRY(sc, instIdx, j)
      = FRNM_EV_QUEUE_ENTRY(sc, instIdx, j + 1U);
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

STATIC FUNC(void, FRNM_CODE) FRNM_HSM_TRAN(
  const FrNm_HsmStatechartPtrType                      sc,
  const uint8                                         instIdx,
  CONSTP2CONST(FrNm_HsmTranType, AUTOMATIC, FRNM_CONST) tran)
{
  P2CONST(FrNm_HsmStateType, AUTOMATIC, FRNM_CONST) state;
  CONSTP2VAR(FrNm_HsmInstType, AUTOMATIC, FRNM_VAR_CLEARED) inst
      = &FRNM_INST_ACCESS(sc->instances, instIdx);

  /* perform simplified transition for internal transitions */
  if (tran->target == FRNM_HSM_SIDX_INVALID)
  {
    /* Deviation MISRAC2012-2 */
    FRNM_HSM_PRINT4(
      "%s %d %s (internal transition)\n",
      sc->hsmName, FRNM_INST(instIdx), sc->actionNames[tran->steps[0U]]);
    sc->actions[tran->steps[0U]](FRNM_PL_SF(instIdx));
  }
  else /* external transitions */
  {
    uint8_least i;

    /* "source" may be a superstate of "state".  exit all states from "state"
     * until "source" is reached */
    while (inst->stateId != inst->sourceId)
    {
      state = &sc->states[inst->stateId];

      if (state->exitFnIdx != FRNM_HSM_AIDX_INVALID)
      {
        /* Deviation MISRAC2012-2 */
        FRNM_HSM_PRINT4(
          "%s %d %s\n",
          sc->hsmName, FRNM_INST(instIdx), sc->actionNames[state->exitFnIdx]);
        sc->actions[state->exitFnIdx](FRNM_PL_SF(instIdx));
      }

      inst->stateId = state->parentId;
    }

    /* execute exit/transiton/entry actions on transition path */
    for (i = 0U; i < tran->numSteps; ++i)
    {
      /* Deviation MISRAC2012-2 */
      FRNM_HSM_PRINT4(
        "%s %d %s\n",
        sc->hsmName, FRNM_INST(instIdx), sc->actionNames[tran->steps[i]]);
      sc->actions[tran->steps[i]](FRNM_PL_SF(instIdx));
    }

    /* set the new current state and perform init steps of substates */
    inst->stateId = tran->target; /* current state is last entered state */
    FRNM_HSM_INIT_SUBSTATES(sc, instIdx);

    /* Deviation MISRAC2012-2 */
    FRNM_HSM_PRINT4(
      "%s %d transition to state %s finished\n",
      sc->hsmName, FRNM_INST(instIdx), sc->stateNames[inst->stateId]);
  }

}

STATIC FUNC(void, FRNM_CODE) FRNM_HSM_INIT_SUBSTATES(
  const FrNm_HsmStatechartPtrType sc,
  const uint8                    instIdx)
{
  CONSTP2VAR(FrNm_HsmInstType, AUTOMATIC, FRNM_VAR_CLEARED) inst
    = &FRNM_INST_ACCESS(sc->instances,instIdx);

  FrNm_HsmStateIdxType stateIdx = sc->states[inst->stateId].initId;

  while (stateIdx != FRNM_HSM_SIDX_INVALID) /* init state if necessary */
  {
    P2CONST(FrNm_HsmStateType, AUTOMATIC, FRNM_CONST) state
      = &sc->states[stateIdx];

    inst->stateId = stateIdx; /* set new current state */
    /* enter the substate */
    /* CHECK: NOPARSE */
    /* No parse have been added because with the current state machine
     * configuration (eap file) statement (state->entryFnIdx != FRNM_HSM_AIDX_INVALID)
     * is never evaluated to FALSE
     */
    if (state->entryFnIdx != FRNM_HSM_AIDX_INVALID)
    {
      /* enter the substate */
      /* Deviation MISRAC2012-2 */
      FRNM_HSM_PRINT4(
        "%s %d %s\n",
        sc->hsmName, FRNM_INST(instIdx), sc->actionNames[state->entryFnIdx]);
      sc->actions[state->entryFnIdx](FRNM_PL_SF(instIdx));
    }
    /* CHECK: PARSE */
    /* get next init state */
    stateIdx = state->initId;
  }
}

STATIC FUNC(boolean, FRNM_CODE) FRNM_HSM_FIND_EV_TO_DISPATCH(
  const FrNm_HsmStatechartPtrType                     sc,
  const uint8                                        instIdx,
  CONSTP2VAR(FrNm_HsmEventType, AUTOMATIC, AUTOMATIC) event,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)            queueEntryNo)
{
    CONSTP2VAR(FrNm_HsmInstType, AUTOMATIC, FRNM_VAR_CLEARED) inst
      = &FRNM_INST_ACCESS(sc->instances,instIdx);
    CONSTP2CONST(FrNm_HsmStateType, AUTOMATIC, FRNM_CONST) state
      = &sc->states[inst->stateId];
    boolean          foundEventToDispatch = FALSE;
    FrNm_HsmEventType ev                   = FRNM_HSM_EVENT_INVALID;
    uint8            entryNo              = 0U;

    /* lock event queue access */
    SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
    while ((entryNo < inst->evQueueFill)
           && (FALSE == foundEventToDispatch))
    {
      FrNm_HsmEventMaskType eventMask;
      ev        = FRNM_EV_QUEUE_ENTRY(sc, instIdx, entryNo);
      eventMask = (FrNm_HsmEventMaskType)1UL << ev;
/* CHECK: NOPARSE */
/* No parse have been added because:
 *  - if (inst->evQueueInsertEvNo <= entryNo) is never evaluated to FALSE
 *    since FRNM_HSMEMITTOSELFINST function is not used.
 *  - statement if (eventMask & state->actionEventMask) != 0U)
 *    is never evaluated to FALSE since there are no differed events.
 */

      if ((eventMask & state->ignoredEventMask) != 0U)
      {
        /* Deviation MISRAC2012-2 */
        FRNM_HSM_PRINT4(
          "%s %d event %s ignored\n",
          sc->hsmName, FRNM_INST(instIdx), sc->eventNames[ev]);

        /* delete event no 'entryNo' from queue */
        FRNM_HSM_DEL_FROM_QUEUE(sc, instIdx, entryNo);
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
        FRNM_HSM_PRINT4(
          "%s %d event %s deferred\n",
          sc->hsmName, FRNM_INST(instIdx), sc->eventNames[ev]);

        /* found only deferred event, try next one in queue */
        ++entryNo;
      }
/* CHECK: PARSE */
    }
    SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

    *event        = ev;
    *queueEntryNo = entryNo;
    return foundEventToDispatch;
}

STATIC FUNC(boolean, FRNM_CODE) FRNM_HSM_DISPATCH_EVENT(
  const FrNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const FrNm_HsmEventType         event)
{
  CONSTP2VAR(FrNm_HsmInstType, AUTOMATIC, FRNM_VAR_CLEARED) inst
    = &FRNM_INST_ACCESS(sc->instances,instIdx);
  P2CONST(FrNm_HsmTranType, AUTOMATIC, FRNM_CONST) trans = NULL_PTR;
  /* return value, flag indicating if at least one transition fired */
  boolean transitionFired = FALSE;
  /* flag for the trigger search algorithm */
  boolean triggerFound    = FALSE;

  /* *** search for transition for non-deferred and non-ignored event and
   * fire the transition if found *** */

  /* Deviation MISRAC2012-2 */
  FRNM_HSM_PRINT5(
    "%s %d dispatching event %s in state %s\n",
    sc->hsmName, FRNM_INST(instIdx), sc->eventNames[event],
    sc->stateNames[inst->stateId]);

  /* try current leaf state as src of transitions */
  inst->sourceId = inst->stateId;

  /* start searching for a triggering event */
  do
  {
    uint8_least i;          /* index in transition array of state */

    CONSTP2CONST(FrNm_HsmStateType, AUTOMATIC, FRNM_CONST) state
      = &sc->states[inst->sourceId];

    /* check all possible transitions of state */
    for (i = 0U; (i < state->numTrans) && (FALSE == triggerFound); ++i)
    {
      trans = &state->trans[i]; /* test next transition */

      if (event == trans->event)
      {
        /* evaluate guard condition */
        boolean guardCond = TRUE;

        if (trans->guardFnIdx != FRNM_HSM_GIDX_INVALID)
        {
          guardCond = sc->guards[trans->guardFnIdx](FRNM_PL_SF(instIdx));
          /* Deviation MISRAC2012-2 */
          FRNM_HSM_PRINT5(
            "%s %d %s evaluates to %s\n",
            sc->hsmName, FRNM_INST(instIdx),
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
           && (FRNM_HSM_SIDX_INVALID != inst->sourceId));

  /* a triggering event was found and transition is ready to be executed or
   * the top state is reached the queued event must be ignored
   *
   * we have to check if the top state is not yet reached at runtime because
   * there may still be non-ignored and not handled events due to false
   * guard conditions */

  if (TRUE == triggerFound)
  {
    FRNM_HSM_TRAN(sc, instIdx, trans); /* execute the transition */
  }
  else
  {
    /* Deviation MISRAC2012-2 */
    FRNM_HSM_PRINT4(
      "%s %d event %s ignored at top state\n",
      sc->hsmName, FRNM_INST(instIdx), sc->eventNames[event]);
  }

  return transitionFired;
}

#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>

/*==================[end of file]===========================================*/
