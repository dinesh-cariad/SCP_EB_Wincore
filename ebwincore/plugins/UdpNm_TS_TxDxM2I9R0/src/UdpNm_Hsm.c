/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
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

#define UDPNM_NO_CFGCLASSMIX_REQUIRED
#ifndef UDPNM_NO_PBCFG_REQUIRED
#define UDPNM_NO_PBCFG_REQUIRED
#endif

#include <UdpNm_Trace.h>

#include <SchM_UdpNm.h>           /* SchM symbols for crit. sections */

#include <UdpNm_Hsm.h>            /* public API of UdpNm_Hsm.c */
#include <UdpNm_Int.h>

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
/* !LINKSTO EB_SWS_UdpNm_00082_5,1 */
#include <Det.h>                        /* Det API */
#endif

/* design debugging output */
#if (UDPNM_HSM_TRACE == STD_ON)
/* CHECK: RULE 501 OFF (macro needed in std headers) */
/* Deviation MISRAC2012-1 */
#define __NO_ISOCEXT
#include <stdlib.h>
#include <stdio.h>
/* CHECK: RULE 501 ON */
#endif

/*==================[macros]================================================*/

#if (defined UDPNM_HSM_DEL_FROM_QUEUE)
#error UDPNM_HSM_DEL_FROM_QUEUE already defined
#endif

#if (defined UDPNM_HSM_TRAN)
#error UDPNM_HSM_TRAN already defined
#endif

#if (defined UDPNM_HSM_INIT_SUBSTATES)
#error UDPNM_HSM_INIT_SUBSTATES already defined
#endif

#if (defined UDPNM_HSM_FIND_EV_TO_DISPATCH)
#error UDPNM_HSM_FIND_EV_TO_DISPATCH already defined
#endif

#if (defined UDPNM_HSM_DISPATCH_EVENT)
#error UDPNM_HSM_DISPATCH_EVENT already defined
#endif

#if (defined UDPNM_EV_QUEUE_ENTRY)
#error UDPNM_EV_QUEUE_ENTRY already defined
#endif

#if (defined UDPNM_EV_COUNTER_ENTRY)
#error UDPNM_EV_COUNTER_ENTRY already defined
#endif

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
#define UDPNM_HSM_DEL_FROM_QUEUE(a,b,c) UdpNm_HsmDelFromQueue(a,b,c)
#define UDPNM_HSM_TRAN(a,b,c)           UdpNm_HsmTran(a,b,c)
#define UDPNM_HSM_INIT_SUBSTATES(a,b)   UdpNm_HsmInitSubstates(a,b)
#define UDPNM_HSM_FIND_EV_TO_DISPATCH(a,b,c,d) UdpNm_HsmFindEvToDispatch(a,b,c,d)
#define UDPNM_HSM_DISPATCH_EVENT(a,b,c) UdpNm_HsmDispatchEvent(a,b,c)

/** \brief Access entry in event queue of a specific hsm instance */
#define UDPNM_EV_QUEUE_ENTRY(sc,instIdx,entryIdx) \
  ((sc)->events[((instIdx)*((sc)->numEvents))+(entryIdx)])
#define UDPNM_EV_COUNTER_ENTRY(sc,instIdx,entryIdx) \
  ((sc)->evCounters[((instIdx)*((sc)->numEvents))+(entryIdx)])
#else
#define UDPNM_HSM_DEL_FROM_QUEUE(a,b,c) UdpNm_HsmDelFromQueue(a,c)
#define UDPNM_HSM_TRAN(a,b,c)           UdpNm_HsmTran(a,c)
#define UDPNM_HSM_INIT_SUBSTATES(a,b)   UdpNm_HsmInitSubstates(a)
#define UDPNM_HSM_FIND_EV_TO_DISPATCH(a,b,c,d) UdpNm_HsmFindEvToDispatch(a,c,d)
#define UDPNM_HSM_DISPATCH_EVENT(a,b,c) UdpNm_HsmDispatchEvent(a,c)
#define UDPNM_EV_QUEUE_ENTRY(sc,instIdx,entryIdx) \
  ((sc)->events[(entryIdx)])
#define UDPNM_EV_COUNTER_ENTRY(sc,instIdx,entryIdx) \
  ((sc)->evCounters[(entryIdx)])
#endif

#if (defined UDPNM_HSM_PRINT1)
#error UDPNM_HSM_PRINT1 already defined
#endif

#if (defined UDPNM_HSM_PRINT2)
#error UDPNM_HSM_PRINT2 already defined
#endif

#if (defined UDPNM_HSM_PRINT3)
#error UDPNM_HSM_PRINT3 already defined
#endif

#if (defined UDPNM_HSM_PRINT4)
#error UDPNM_HSM_PRINT4 already defined
#endif

#if (defined UDPNM_HSM_PRINT5)
#error UDPNM_HSM_PRINT5 already defined
#endif

#if (UDPNM_HSM_TRACE == STD_ON)
#define UDPNM_HSM_PRINT1(a)                          \
  do                                                 \
  {                                                  \
    if (UdpNm_HsmTraceEnabled == TRUE)               \
    {                                                \
      (void)printf("HsmTrace: " a);                  \
    }                                                \
  } while (0U)
#define UDPNM_HSM_PRINT2(a,b)                        \
  do                                                 \
  {                                                  \
    if (UdpNm_HsmTraceEnabled == TRUE)               \
    {                                                \
      (void)printf("HsmTrace: " a,(b));              \
    }                                                \
  } while (0U)
/* Deviation MISRAC2012-2 */
#define UDPNM_HSM_PRINT3(a,b,c)                      \
  do                                                 \
  {                                                  \
    if (UdpNm_HsmTraceEnabled == TRUE)               \
    {                                                \
      (void)printf("HsmTrace: " a,(b),(c));          \
    }                                                \
  } while (0U)
#define UDPNM_HSM_PRINT4(a,b,c,d)                    \
  do                                                 \
  {                                                  \
    if (UdpNm_HsmTraceEnabled == TRUE)               \
    {                                                \
      (void)printf("HsmTrace: " a,(b),(c),(d));      \
    }                                                \
  } while (0U)
#define UDPNM_HSM_PRINT5(a,b,c,d,e)                  \
  do                                                 \
  {                                                  \
    if (UdpNm_HsmTraceEnabled == TRUE)               \
    {                                                \
      (void)printf("HsmTrace: " a,(b),(c),(d),(e));  \
    }                                                \
  } while (0U)
#else
#define UDPNM_HSM_PRINT1(a)          /* nothing */
#define UDPNM_HSM_PRINT2(a,b)        /* nothing */
#define UDPNM_HSM_PRINT3(a,b,c)      /* nothing */
#define UDPNM_HSM_PRINT4(a,b,c,d)    /* nothing */
#define UDPNM_HSM_PRINT5(a,b,c,d,e)  /* nothing */
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define UDPNM_START_SEC_CODE
#include <UdpNm_MemMap.h>

/** \brief Delete entry from event queue
 ** \param sc pointer to state chart
 ** \param instIdx index of state machine instance to work on
 ** \param queueEntryNo entry in queue to delete
 **
 ** This function must be called inside of a critical section which prevents
 ** the concurrent access to the queue.
 **/
STATIC FUNC(void, UDPNM_CODE) UDPNM_HSM_DEL_FROM_QUEUE(
  const UdpNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const uint8                    queueEntryNo);

/** \brief Execute a transition
 ** \param sc pointer to state chart
 ** \param instIdx index of state machine instance to work on
 ** \param tran Pointer to the transition description struct */
STATIC FUNC(void, UDPNM_CODE) UDPNM_HSM_TRAN(
  const UdpNm_HsmStatechartPtrType                      sc,
  const uint8                                         instIdx,
  CONSTP2CONST(UdpNm_HsmTranType, AUTOMATIC, UDPNM_CONST) tran);

/** \brief Initialize all substates for the current state
 ** \param sc pointer to state chart to work on
 ** \param instIdx index of state machine instance to work on */
STATIC FUNC(void, UDPNM_CODE) UDPNM_HSM_INIT_SUBSTATES(
  const UdpNm_HsmStatechartPtrType sc,
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
STATIC FUNC(boolean, UDPNM_CODE) UDPNM_HSM_FIND_EV_TO_DISPATCH(
  const UdpNm_HsmStatechartPtrType                     sc,
  const uint8                                        instIdx,
  CONSTP2VAR(UdpNm_HsmEventType, AUTOMATIC, AUTOMATIC) event,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)            queueEntryNo);

/** \brief Dispatch event from event queue
 ** \param sc pointer to state chart to work on
 ** \param instIdx index of state machine instance to work on
 ** \param event identified event from queue
 ** \return Transition fired indicator
 ** \retval TRUE the event dispatching resulted in an fired transition
 ** \retval FALSE else */
STATIC FUNC(boolean, UDPNM_CODE) UDPNM_HSM_DISPATCH_EVENT(
  const UdpNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const UdpNm_HsmEventType         event);

#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#if (UDPNM_HSM_TRACE == STD_ON)

#define UDPNM_START_SEC_VAR_INIT_8
#include <UdpNm_MemMap.h>

STATIC VAR(boolean, UDPNM_VAR) UdpNm_HsmTraceEnabled = FALSE;

#define UDPNM_STOP_SEC_VAR_INIT_8
#include <UdpNm_MemMap.h>

#endif

/*==================[external function definitions]=========================*/

#define UDPNM_START_SEC_CODE
#include <UdpNm_MemMap.h>

/*------------------ init functions ----------------------------------------*/

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
FUNC(void, UDPNM_CODE) UdpNm_HsmInit(const UdpNm_HsmStatechartPtrType sc)
{
  uint8 instIdx;
  for (instIdx = 0U; instIdx < sc->numInstances; ++instIdx)
  {
    UDPNM_HSMINITINST(sc, instIdx);
  }
}
/* else: function is implemented as macro and maps to UdpNm_HsmInitInst() */
#endif

FUNC(void, UDPNM_CODE) UDPNM_HSMINITINST(
  const UdpNm_HsmStatechartPtrType sc,
  const uint8                    instIdx)
{
  CONSTP2VAR(UdpNm_HsmInstType, AUTOMATIC, UDPNM_VAR_CLEARED) inst
    = &UDPNM_INST_ACCESS(sc->instances, instIdx);
  uint8 i;

  UDPNM_HSM_PRINT3("%s %d Init\n", sc->hsmName, UDPNM_INST(instIdx));

  /* init all fields because the struct is placed in a NOINIT memory
   * section */
  inst->stateId  = sc->topStateId;
  inst->sourceId = UDPNM_HSM_SIDX_INVALID;

  for (i = 0U; i < sc->numEvents; ++i)
  {
    UDPNM_EV_QUEUE_ENTRY(sc, instIdx, i) = 0U;
    UDPNM_EV_COUNTER_ENTRY(sc, instIdx, i) = 0U;
  }
  inst->evQueueFill       = 0U;
  inst->evQueueInsertEvNo = 0U;
/* CHECK: NOPARSE */
/* No parse have been added because with the current state machine
 * configuration (eap file) top state has an entry function
 */
  if (sc->states[inst->stateId].entryFnIdx != UDPNM_HSM_AIDX_INVALID)
  {
    /* Deviation MISRAC2012-2 */
    UDPNM_HSM_PRINT4(
      "%s %d %s\n",
      sc->hsmName, UDPNM_INST(instIdx), sc->actionNames[inst->stateId]);

    /* enter top state */
    sc->actions[sc->states[inst->stateId].entryFnIdx](UDPNM_PL_SF(instIdx));
  }
/* CHECK: PARSE */
  /* init all substates */
  UDPNM_HSM_INIT_SUBSTATES(sc, instIdx);
}

/*------------------ emit functions ----------------------------------------*/

/* CHECK: NOPARSE */
/* No parse have been added because function is not used
 */
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
FUNC(void, UDPNM_CODE) UdpNm_HsmEmit(
  const UdpNm_HsmStatechartPtrType sc,
  const UdpNm_HsmEventType         event)
{
  uint8 instIdx;

  for (instIdx = 0U; instIdx < sc->numInstances; ++instIdx)
  {
    UDPNM_HSMEMITINST(sc, instIdx, event);
  }
}
/* else: function is implemented as macro */
#endif
/* CHECK: PARSE */

FUNC(void, UDPNM_CODE) UDPNM_HSMEMITINST(
  const UdpNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const UdpNm_HsmEventType         event)
{
  CONSTP2VAR(UdpNm_HsmInstType, AUTOMATIC, UDPNM_VAR_CLEARED) inst
    = &UDPNM_INST_ACCESS(sc->instances, instIdx);


  SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
  /* in case the event has already been queued don't add it again to the queue*/
  /* !LINKSTO UdpNm.EB.EventQueue,1 */
  if ( UDPNM_EV_COUNTER_ENTRY(sc, instIdx, event) < 1U)
  {
    UDPNM_EV_QUEUE_ENTRY(sc, instIdx, inst->evQueueFill) = event;
    inst->evQueueFill++;
    /* leave evQueueInsertEvNo unchanged, this pointer will be updated on
     * insertion or event dispatching */
    ++UDPNM_EV_COUNTER_ENTRY(sc, instIdx, event);
  }
  SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

  /* Deviation MISRAC2012-2 */
  UDPNM_HSM_PRINT4(
    "%s %d event %s enqueued\n",
    sc->hsmName, UDPNM_INST(instIdx), sc->eventNames[event]);

}

FUNC(void, UDPNM_CODE) UDPNM_HSMEMITTOSELFINST(
  const UdpNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const UdpNm_HsmEventType         event)
{
  CONSTP2VAR(UdpNm_HsmInstType, AUTOMATIC, UDPNM_VAR_CLEARED) inst
    = &UDPNM_INST_ACCESS(sc->instances, instIdx);

  SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
  if ( UDPNM_EV_COUNTER_ENTRY(sc, instIdx, event) < 1U)
  {
    /* make room for new entry */
    uint8 j;

    for (j = inst->evQueueFill; j > inst->evQueueInsertEvNo; --j)
    {
      UDPNM_EV_QUEUE_ENTRY(sc, instIdx, j) = UDPNM_EV_QUEUE_ENTRY(sc, instIdx, j-1U);
    }

    ++inst->evQueueFill;
    ++UDPNM_EV_COUNTER_ENTRY(sc, instIdx, event);
    UDPNM_EV_QUEUE_ENTRY(sc, instIdx, inst->evQueueInsertEvNo) = event;
    inst->evQueueInsertEvNo++;
  }
  SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

  /* Deviation MISRAC2012-2 */
  UDPNM_HSM_PRINT4(
    "%s %d event %s enqueued internally\n",
    sc->hsmName, UDPNM_INST(instIdx), sc->eventNames[event]);
}

/*------------------ main functions ----------------------------------------*/
/* CHECK: NOPARSE */
/* No parse have been added because this function is not used.
 */
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
FUNC(void, UDPNM_CODE) UdpNm_HsmMain(const UdpNm_HsmStatechartPtrType sc)
{
  boolean transitionFired;

  do
  {
    uint8 instIdx;
    transitionFired = FALSE;
    /* for all instances */
    for (instIdx = 0U; instIdx < sc->numInstances; ++instIdx)
    {
      const boolean retVal = UdpNm_HsmMainInst(sc, instIdx);
      transitionFired = (retVal || transitionFired);
    }
  } while (transitionFired);
}
/* else: function is implemented as macro and maps to UdpNm_HsmMainInst() */
#endif
/* CHECK: PARSE */
FUNC(boolean, UDPNM_CODE) UDPNM_HSMMAININST(
  const UdpNm_HsmStatechartPtrType sc,
  const uint8                    instIdx)
{
  CONSTP2VAR(UdpNm_HsmInstType, AUTOMATIC, UDPNM_VAR_CLEARED) inst
    = &UDPNM_INST_ACCESS(sc->instances,instIdx);
  /* return value, flag indicating if at least one transition fired */
  boolean          transitionFired = FALSE;
  UdpNm_HsmEventType event;
  uint8            queueEntryNo;
  boolean          eventPending;

  /* Deviation MISRAC2012-2 */
  UDPNM_HSM_PRINT3(
    "%s %d Main\n",
    sc->hsmName, UDPNM_INST(instIdx));

  /* reset event insertion pointer to an invalid value, this will be set to a
   * valid location if the first event is being dispatched */
  inst->evQueueInsertEvNo = 0U;

  /* check if there are is at least one pending event */
  eventPending
    = UDPNM_HSM_FIND_EV_TO_DISPATCH(sc, instIdx, &event, &queueEntryNo);

  /* dispatch all pending events from event queue */
  while (TRUE == eventPending)
  {
    boolean retVal;
    /* delete event from queue */
    SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
    UDPNM_HSM_DEL_FROM_QUEUE(sc, instIdx, queueEntryNo);
    SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

    /* dispatch event */
    retVal = UDPNM_HSM_DISPATCH_EVENT(sc, instIdx, event);
    transitionFired = (retVal || transitionFired);



    eventPending
      = UDPNM_HSM_FIND_EV_TO_DISPATCH(sc, instIdx, &event, &queueEntryNo);
  }


  return transitionFired;
}

#if (UDPNM_HSM_TRACE == STD_ON)
FUNC(void, UDPNM_CODE) UdpNm_HsmSetTracing(const boolean newValue)
{
  UdpNm_HsmTraceEnabled = newValue;
}
#endif

#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>

/*==================[internal function definitions]=========================*/

#define UDPNM_START_SEC_CODE
#include <UdpNm_MemMap.h>

STATIC FUNC(void, UDPNM_CODE) UDPNM_HSM_DEL_FROM_QUEUE(
  const UdpNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const uint8                    queueEntryNo)
{
  /* delete event no queueEntryNo from queue */
  CONSTP2VAR(UdpNm_HsmInstType, AUTOMATIC, UDPNM_VAR_CLEARED) inst
    = &UDPNM_INST_ACCESS(sc->instances, instIdx);
  uint8 j;

  /* reduce the occurrence counter of the respective event */
  UdpNm_HsmEventType event = UDPNM_EV_QUEUE_ENTRY(sc, instIdx, queueEntryNo);
  --UDPNM_EV_COUNTER_ENTRY(sc, instIdx, event);

  /* Deviation MISRAC2012-3 */
  for (j = queueEntryNo; (j + 1U) < inst->evQueueFill; ++j)
  {
    UDPNM_EV_QUEUE_ENTRY(sc, instIdx, j)
      = UDPNM_EV_QUEUE_ENTRY(sc, instIdx, j + 1U);
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

STATIC FUNC(void, UDPNM_CODE) UDPNM_HSM_TRAN(
  const UdpNm_HsmStatechartPtrType                      sc,
  const uint8                                         instIdx,
  CONSTP2CONST(UdpNm_HsmTranType, AUTOMATIC, UDPNM_CONST) tran)
{
  P2CONST(UdpNm_HsmStateType, AUTOMATIC, UDPNM_CONST) state;
  CONSTP2VAR(UdpNm_HsmInstType, AUTOMATIC, UDPNM_VAR_CLEARED) inst
      = &UDPNM_INST_ACCESS(sc->instances, instIdx);

  /* perform simplified transition for internal transitions */
  if (tran->target == UDPNM_HSM_SIDX_INVALID)
  {
    /* Deviation MISRAC2012-2 */
    UDPNM_HSM_PRINT4(
      "%s %d %s (internal transition)\n",
      sc->hsmName, UDPNM_INST(instIdx), sc->actionNames[tran->steps[0U]]);
    sc->actions[tran->steps[0U]](UDPNM_PL_SF(instIdx));
  }
  else /* external transitions */
  {
    uint8_least i;

    /* "source" may be a superstate of "state".  exit all states from "state"
     * until "source" is reached */
    while (inst->stateId != inst->sourceId)
    {
      state = &sc->states[inst->stateId];

      if (state->exitFnIdx != UDPNM_HSM_AIDX_INVALID)
      {
        /* Deviation MISRAC2012-2 */
        UDPNM_HSM_PRINT4(
          "%s %d %s\n",
          sc->hsmName, UDPNM_INST(instIdx), sc->actionNames[state->exitFnIdx]);
        sc->actions[state->exitFnIdx](UDPNM_PL_SF(instIdx));
      }

      inst->stateId = state->parentId;
    }

    /* execute exit/transiton/entry actions on transition path */
    for (i = 0U; i < tran->numSteps; ++i)
    {
      /* Deviation MISRAC2012-2 */
      UDPNM_HSM_PRINT4(
        "%s %d %s\n",
        sc->hsmName, UDPNM_INST(instIdx), sc->actionNames[tran->steps[i]]);
      sc->actions[tran->steps[i]](UDPNM_PL_SF(instIdx));
    }

    /* set the new current state and perform init steps of substates */
    inst->stateId = tran->target; /* current state is last entered state */
    UDPNM_HSM_INIT_SUBSTATES(sc, instIdx);

    /* Deviation MISRAC2012-2 */
    UDPNM_HSM_PRINT4(
      "%s %d transition to state %s finished\n",
      sc->hsmName, UDPNM_INST(instIdx), sc->stateNames[inst->stateId]);
  }

}

STATIC FUNC(void, UDPNM_CODE) UDPNM_HSM_INIT_SUBSTATES(
  const UdpNm_HsmStatechartPtrType sc,
  const uint8                    instIdx)
{
  CONSTP2VAR(UdpNm_HsmInstType, AUTOMATIC, UDPNM_VAR_CLEARED) inst
    = &UDPNM_INST_ACCESS(sc->instances,instIdx);

  UdpNm_HsmStateIdxType stateIdx = sc->states[inst->stateId].initId;

  while (stateIdx != UDPNM_HSM_SIDX_INVALID) /* init state if necessary */
  {
    P2CONST(UdpNm_HsmStateType, AUTOMATIC, UDPNM_CONST) state
      = &sc->states[stateIdx];

    inst->stateId = stateIdx; /* set new current state */
    /* enter the substate */
/* CHECK: NOPARSE */
/* No parse have been added because with the current state machine
 * configuration (eap file) statement (state->entryFnIdx != UDPNM_HSM_AIDX_INVALID)
 * is never evaluated to FALSE
 */
    if (state->entryFnIdx != UDPNM_HSM_AIDX_INVALID)
    {
      /* enter the substate */
      /* Deviation MISRAC2012-2 */
      UDPNM_HSM_PRINT4(
        "%s %d %s\n",
        sc->hsmName, UDPNM_INST(instIdx), sc->actionNames[state->entryFnIdx]);
      sc->actions[state->entryFnIdx](UDPNM_PL_SF(instIdx));
    }
/* CHECK: PARSE */
    /* get next init state */
    stateIdx = state->initId;
  }
}

STATIC FUNC(boolean, UDPNM_CODE) UDPNM_HSM_FIND_EV_TO_DISPATCH(
  const UdpNm_HsmStatechartPtrType                     sc,
  const uint8                                        instIdx,
  CONSTP2VAR(UdpNm_HsmEventType, AUTOMATIC, AUTOMATIC) event,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)            queueEntryNo)
{
    CONSTP2VAR(UdpNm_HsmInstType, AUTOMATIC, UDPNM_VAR_CLEARED) inst
      = &UDPNM_INST_ACCESS(sc->instances,instIdx);
    CONSTP2CONST(UdpNm_HsmStateType, AUTOMATIC, UDPNM_CONST) state
      = &sc->states[inst->stateId];
    boolean          foundEventToDispatch = FALSE;
    UdpNm_HsmEventType ev                   = UDPNM_HSM_EVENT_INVALID;
    uint8            entryNo              = 0U;


    /* lock event queue access */
    SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
    while ((entryNo < inst->evQueueFill)
           && (FALSE == foundEventToDispatch))
    {
      UdpNm_HsmEventMaskType eventMask;
      ev        = UDPNM_EV_QUEUE_ENTRY(sc, instIdx, entryNo);
      eventMask = (UdpNm_HsmEventMaskType)1UL << ev;
/* CHECK: NOPARSE */
/* No parse have been added because:
 *  - statement if (eventMask & state->actionEventMask) != 0U)
 *    is never evaluated to FALSE since there are no differed events.
 */
      if ((eventMask & state->ignoredEventMask) != 0U)
      {
        /* Deviation MISRAC2012-2 */
        UDPNM_HSM_PRINT4(
          "%s %d event %s ignored\n",
          sc->hsmName, UDPNM_INST(instIdx), sc->eventNames[ev]);

        /* delete event no 'entryNo' from queue */
        UDPNM_HSM_DEL_FROM_QUEUE(sc, instIdx, entryNo);
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
        UDPNM_HSM_PRINT4(
          "%s %d event %s deferred\n",
          sc->hsmName, UDPNM_INST(instIdx), sc->eventNames[ev]);

        /* found only deferred event, try next one in queue */
        ++entryNo;
      }
/* CHECK: PARSE */
    }
    SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

    *event        = ev;
    *queueEntryNo = entryNo;
    return foundEventToDispatch;
}

STATIC FUNC(boolean, UDPNM_CODE) UDPNM_HSM_DISPATCH_EVENT(
  const UdpNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const UdpNm_HsmEventType         event)
{
  CONSTP2VAR(UdpNm_HsmInstType, AUTOMATIC, UDPNM_VAR_CLEARED) inst
    = &UDPNM_INST_ACCESS(sc->instances,instIdx);
  P2CONST(UdpNm_HsmTranType, AUTOMATIC, UDPNM_CONST) trans = NULL_PTR;
  /* return value, flag indicating if at least one transition fired */
  boolean transitionFired = FALSE;
  /* flag for the trigger search algorithm */
  boolean triggerFound    = FALSE;

  /* *** search for transition for non-deferred and non-ignored event and
   * fire the transition if found *** */

  /* Deviation MISRAC2012-2 */
  UDPNM_HSM_PRINT5(
    "%s %d dispatching event %s in state %s\n",
    sc->hsmName, UDPNM_INST(instIdx), sc->eventNames[event],
    sc->stateNames[inst->stateId]);

  /* try current leaf state as src of transitions */
  inst->sourceId = inst->stateId;

  /* start searching for a triggering event */
  do
  {
    uint8_least i;          /* index in transition array of state */

    CONSTP2CONST(UdpNm_HsmStateType, AUTOMATIC, UDPNM_CONST) state
      = &sc->states[inst->sourceId];

    /* check all possible transitions of state */
    for (i = 0U; (i < state->numTrans) && (FALSE == triggerFound); ++i)
    {
      trans = &state->trans[i]; /* test next transition */

      if (event == trans->event)
      {
        /* evaluate guard condition */
        boolean guardCond = TRUE;

        if (trans->guardFnIdx != UDPNM_HSM_GIDX_INVALID)
        {
          guardCond = sc->guards[trans->guardFnIdx](UDPNM_PL_SF(instIdx));
          /* Deviation MISRAC2012-2 */
          UDPNM_HSM_PRINT5(
            "%s %d %s evaluates to %s\n",
            sc->hsmName, UDPNM_INST(instIdx),
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
           && (UDPNM_HSM_SIDX_INVALID != inst->sourceId));

  /* a triggering event was found and transition is ready to be executed or
   * the top state is reached the queued event must be ignored
   *
   * we have to check if the top state is not yet reached at runtime because
   * there may still be non-ignored and not handled events due to false
   * guard conditions */

  if (TRUE == triggerFound)
  {
    UDPNM_HSM_TRAN(sc, instIdx, trans); /* execute the transition */
  }
  else
  {
    /* Deviation MISRAC2012-2 */
    UDPNM_HSM_PRINT4(
      "%s %d event %s ignored at top state\n",
      sc->hsmName, UDPNM_INST(instIdx), sc->eventNames[event]);
  }

  return transitionFired;
}

#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>

/*==================[end of file]===========================================*/
