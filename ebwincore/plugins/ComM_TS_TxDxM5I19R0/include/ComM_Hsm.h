/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef COMM_HSM_H
#define COMM_HSM_H

/* This file contains public interface for the hierarchical state machine
 * driver. */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: D4.6 (advisory)
 * typedefs that indicate size and signedness should be used in place of the basic numerical types.
 *
 * Reason:
 * Platform depended 'char' type is only used to define debugging strings on Windows.
 *
 * MISRAC2012-2) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses.
 *
 * Reason:
 * The macro is used in function parameter declarations and definitions where the number of
 * parentheses matter.
 */

/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* TS_ARCH_FAMILY */

#include <ComM_HsmCfg.h>         /* state machine user's configuration */

/*==================[macros]================================================*/

/** \brief Special value for the data type ::ComM_HsmStateIdxType
 *
 * Pseudo state index used for non-existend states. E.g. for the parent
 * state of the top state or the init substates of leaf states. */
#if (defined COMM_HSM_SIDX_INVALID)
#error COMM_HSM_SIDX_INVALID already defined
#endif
#define COMM_HSM_SIDX_INVALID            0xFFU
/** \brief Special value for the data type ::ComM_HsmActionIdxType
 *
 * Used for non-existend actions. */
#if (defined COMM_HSM_AIDX_INVALID)
#error COMM_HSM_AIDX_INVALID already defined
#endif
#define COMM_HSM_AIDX_INVALID            0xFFU
/** \brief Special value for the data type ::ComM_HsmGuardIdxType
 *
 * Used for non-existend guards. */
#if (defined COMM_HSM_GIDX_INVALID)
#error COMM_HSM_GIDX_INVALID already defined
#endif
#define COMM_HSM_GIDX_INVALID            0xFFU

/** \brief Special value for the data type ::ComM_HsmEventType
 *
 * Invalid event */
#if (defined COMM_HSM_EVENT_INVALID)
#error COMM_HSM_EVENT_INVALID already defined
#endif
#define COMM_HSM_EVENT_INVALID           0xFFU

#if (defined COMM_PL_SF)
#error COMM_PL_SF already defined
#endif

#if (defined COMM_PDL_SF)
#error COMM_PDL_SF already defined
#endif

#if (defined COMM_HSMINITINST)
#error COMM_HSMINITINST already defined
#endif

#if (defined COMM_HSMEMITINST)
#error COMM_HSMEMITINST already defined
#endif

#if (defined COMM_HSMEMITTOSELFINST)
#error COMM_HSMEMITTOSELFINST already defined
#endif

#if (defined COMM_HSMMAININST)
#error COMM_HSMMAININST already defined
#endif

#if (defined COMM_INST_ACCESS)
#error COMM_INST_ACCESS already defined
#endif

#if (defined COMM_INST)
#error COMM_INST already defined
#endif

/* Helper macros to enable building the sources with and without multiple
 * instances support */
#if (COMM_HSM_INST_MULTI_ENABLED == STD_ON)
/* parameter list for action, guard, entry, exit state functions */
#define COMM_PL_SF(a)                   a
/* parameter definition list for action, guard, entry, exit state functions */
#define COMM_PDL_SF(a)                  a
#define COMM_HSMINITINST(a,b)           ComM_HsmInitInst(a,b)
#define COMM_HSMEMITINST(a,b,c)         ComM_HsmEmitInst(a,b,c)
#define COMM_HSMEMITTOSELFINST(a,b,c)   ComM_HsmEmitToSelfInst(a,b,c)
#define COMM_HSMMAININST(a,b)           ComM_HsmMainInst(a,b)
/* instance access macro */
#define COMM_INST_ACCESS(array,instIdx) ((array)[(instIdx)])
#define COMM_INST(instIdx)              (instIdx)
#else
#define COMM_PL_SF(a)                   /* nothing */
#define COMM_PDL_SF(a)                  void
#define COMM_HSMINITINST(a,b)           ComM_HsmInitInst(a)
#define COMM_HSMEMITINST(a,b,c)         ComM_HsmEmitInst(a,c)
#define COMM_HSMEMITTOSELFINST(a,b,c)   ComM_HsmEmitToSelfInst(a,c)
#define COMM_HSMMAININST(a,b)           ComM_HsmMainInst(a)
#define COMM_INST_ACCESS(array,instIdx) ((array)[0U])
#define COMM_INST(instIdx)              0U
#endif

#if (defined COMM_HSM_TRACE)
#error COMM_HSM_TRACE already defined
#endif
/* enable tracing capabilities only on windows */
#if defined(TS_WINDOWS) && (TS_ARCH_FAMILY == TS_WINDOWS)
#define COMM_HSM_TRACE                  STD_ON
#else
#define COMM_HSM_TRACE                  STD_OFF
#endif

/*==================[type definitions]======================================*/

/* ** Events ** */

/** \brief Type to encode single events.
 *
 * Event numbers are generated, they are zero-based and consecutive. */
typedef uint8  ComM_HsmEventType;

/** \brief Type to encode event masks.
 *
 * This type describes a bit encoded set of events. The event ev of type
 * ::ComM_HsmEventType corresponds to the (1 << ev) bit in the
 * ::ComM_HsmEventMaskType. */
typedef uint32 ComM_HsmEventMaskType;

/** \brief State index type definition */
typedef uint8  ComM_HsmStateIdxType;
/** \brief Action index type definition */
typedef uint8  ComM_HsmActionIdxType;
/** \brief Guard index type definition */
typedef uint8  ComM_HsmGuardIdxType;

/** \brief Hsm instance
 *
 * Struct for an instance of state machine, placed in NOINIT RAM */
struct ComM_HsmInstStruct
{
  /** \brief Id of the currently active state */
  ComM_HsmStateIdxType   stateId;

  /** \brief Id of source state during a transition */
  ComM_HsmStateIdxType   sourceId;

  /** \brief Number of events filled up in the queue */
  uint8                 evQueueFill;

  /** \brief Index into queue where events emitted to self have to be
   * inserted */
  uint8                 evQueueInsertEvNo;
};

/** \brief Type for state machine instance data */
typedef struct ComM_HsmInstStruct ComM_HsmInstType;

/** \brief Function pointer type for entry/exit/transition action functions */
/* Deviation MISRAC2012-2 */
typedef P2FUNC(void, COMM_CODE, ComM_HsmActionType)(
  COMM_PDL_SF(const uint8 instIdx));

/** \brief Function pointer type for guard functions */
/* Deviation MISRAC2012-2 */
typedef P2FUNC(boolean, COMM_CODE, ComM_HsmGuardType)(
  COMM_PDL_SF(const uint8 instIdx));

/* forward declaration */
struct ComM_HsmTranStruct;
/** \brief Type for state transition description */
typedef struct ComM_HsmTranStruct ComM_HsmTranType;

/** \brief Structure describing a single state of a state chart
 *
 * Type describing states and their relation in the statechart. State
 * descriptions are stored in ROM */
struct ComM_HsmStateStruct
{
  /** \brief Mask of non-deferred and non-ignored events
   *
   * Events related to real actions */
  ComM_HsmEventMaskType                         actionEventMask;
  /** \brief Mask of ignored events */
  ComM_HsmEventMaskType                         ignoredEventMask;

  /** \brief Pointer to array of transitions from this state */
  P2CONST(ComM_HsmTranType, TYPEDEF, COMM_CONST) trans;

  /** \brief Parent state ID */
  ComM_HsmStateIdxType                          parentId;
  /** \brief Init substate, if initId == ::COMM_HSM_SIDX_INVALID there is no
   * substate */
  ComM_HsmStateIdxType                          initId;
  /** \brief Function pointer of entry handler function */
  ComM_HsmActionIdxType                         entryFnIdx;

  /** \brief Function pointer of exit handler function */
  ComM_HsmActionIdxType                         exitFnIdx;

  /** \brief Function pointer of do handler function */
  ComM_HsmActionIdxType                         doFnIdx;

  /** \brief Number of transitions originating from this state */
  uint8                                        numTrans;
};

/** \brief Type for state description */
typedef struct ComM_HsmStateStruct ComM_HsmStateType;

/** \brief State chart description data
 *
 * This struct is placed in ROM */
struct ComM_HsmStatechartStruct
{
  /** \brief Pointer to state description array */
  P2CONST(ComM_HsmStateType, TYPEDEF, COMM_CONST)     states;

  /** \brief Pointer to array containing the Hsm instance working data in
   * RAM */
  P2VAR(ComM_HsmInstType, TYPEDEF, COMM_VAR_CLEARED)   instances;

  /** \brief Pointer to array of all entry/exit/transition function
   * pointers */
  P2CONST(ComM_HsmActionType, TYPEDEF, COMM_CONST)    actions;

  /** \brief Pointer to array of all guard function pointers */
  P2CONST(ComM_HsmGuardType, TYPEDEF, COMM_CONST)     guards;

  /** \brief Pointer to event counter array
   *
   * Total array size equals to (numInstances * numEvents).
   *
   *  for instance #n are located in array fields events[n*numEvents]
   * .. events[(n+1)*numEvents-1] */
  P2VAR(uint8, TYPEDEF, COMM_VAR_CLEARED)             evCounters;

  /** \brief Pointer to event queue for this hsm instance
   *
   * Total array size equals to (numInstances * numEvents).
   *
   * Events for instance #n are located in array fields events[n*numEvents]
   * .. events[(n+1)*numEvents-1] */
  P2VAR(ComM_HsmEventType, TYPEDEF, COMM_VAR_CLEARED)  events;

#if defined(TS_WINDOWS) && (TS_ARCH_FAMILY == TS_WINDOWS)
  /* name of the HSM (for debugging on Windows) */
  /* Deviation MISRAC2012-1 */
  P2CONST(char, COMM_CONST, COMM_CONST)               hsmName;
  /* Deviation MISRAC2012-1, MISRAC2012-2 <+4> */
  P2CONST(P2CONST(char, COMM_CONST, COMM_CONST), COMM_CONST, COMM_CONST) stateNames;
  P2CONST(P2CONST(char, COMM_CONST, COMM_CONST), COMM_CONST, COMM_CONST) actionNames;
  P2CONST(P2CONST(char, COMM_CONST, COMM_CONST), COMM_CONST, COMM_CONST) guardNames;
  P2CONST(P2CONST(char, COMM_CONST, COMM_CONST), COMM_CONST, COMM_CONST) eventNames;
#endif

  /** \brief Number of available events in the state chart */
  uint8                                             numEvents;

  /** \brief Id of the state chart */
  uint8                                             stateChartId;

  /** \brief Number of hsm instances */
  uint8                                             numInstances;
  /** \brief ID of the top state */
  ComM_HsmStateIdxType                               topStateId;
};

/** \brief Type for state chart description */
typedef struct ComM_HsmStatechartStruct ComM_HsmStatechartType;

/** \brief Pointer type to state chart description */
typedef P2CONST(ComM_HsmStatechartType, TYPEDEF, COMM_CONST)
  ComM_HsmStatechartPtrType;


/** \brief State transition
 *
 * Struct type describing a transition in the statechart. Transition
 * descriptions are stored in ROM */
struct ComM_HsmTranStruct
{
  /** \brief Pointer to array of transition step indices */
  P2CONST(ComM_HsmActionIdxType, TYPEDEF, COMM_CONST) steps;

  /** \brief Number of transition steps */
  uint8                                             numSteps;

  /** \brief Triggering event for transition */
  ComM_HsmEventType                                  event;

  /** \brief Index of transition guard function */
  ComM_HsmGuardIdxType                               guardFnIdx;

  /** \brief ID of the target state */
  ComM_HsmStateIdxType                               target;
};

/*==================[external function declarations]========================*/

#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

/*------------------ init functions ----------------------------------------*/

#if (COMM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Init all hsm instances of single state chart
 **
 ** \param sc pointer to state chart which shall be initialized */
extern FUNC(void, COMM_CODE) ComM_HsmInit(const ComM_HsmStatechartPtrType sc);
#else
/** \brief Init hsm instance of single state chart
 **
 ** \param sc pointer to state chart which shall be initialized */
#define ComM_HsmInit(sc) COMM_HSMINITINST((sc),0U)
#endif

/** \brief Init single instance of one hsm
 **
 ** \param sc pointer to state chart which shall be initialized
 ** \param instIdx index of target instance */
extern FUNC(void, COMM_CODE) COMM_HSMINITINST(
  const ComM_HsmStatechartPtrType sc,
  const uint8                    instIdx);

/*------------------ emit functions ----------------------------------------*/

#if (COMM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Send an event to all instances of a state chart
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to all instances of the state machine. */
extern FUNC(void, COMM_CODE) ComM_HsmEmit(
  const ComM_HsmStatechartPtrType sc,
  const ComM_HsmEventType         event);

/** \brief Send an event to all instances of a state chart
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to all instances of the state machine. */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_HsmEmitToSelf(
  const ComM_HsmStatechartPtrType sc,
  const ComM_HsmEventType         event);
#else
/** \brief Send an event to a state chart
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to the only instance of the state machine. */
#if (defined ComM_HsmEmit)
#error ComM_HsmEmit already defined
#endif
#define ComM_HsmEmit(sc,event)       ComM_HsmEmitInst((sc),(event))

/** \brief Send an event to a state chart from
 ** within the state machine functions
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to the only instance of the state machine. */
#if (defined ComM_HsmEmitToSelf)
#error ComM_HsmEmitToSelf already defined
#endif
#define ComM_HsmEmitToSelf(sc,event) ComM_HsmEmitToSelfInst((sc),(event))

#endif

/** \brief Send an event to a specific instance of the state machine
 ** \param sc target state chart
 ** \param instIdx index of target instance
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow */
extern FUNC(void, COMM_CODE) COMM_HSMEMITINST(
  const ComM_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const ComM_HsmEventType         event);

/** \brief Send an event to a specific instance of the state machine from
 ** within the state machine functions
 ** \param sc target state chart
 ** \param instIdx index of target instance
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow */
extern FUNC(void, COMM_CODE) COMM_HSMEMITTOSELFINST(
  const ComM_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const ComM_HsmEventType         event);

/*------------------ main functions ----------------------------------------*/

#if (COMM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Process events for one state chart
 ** \param sc pointer to state charts
 **
 ** This function calls ComM_HsmMainInst() for all instances of the state
 ** machine described in sc until all events are processed. */
extern FUNC(void, COMM_CODE) ComM_HsmMain(const ComM_HsmStatechartPtrType sc);
#else
/** \brief Process events for one state chart
 ** \param sc pointer to state charts
 **
 ** This function calls ComM_HsmMainInst() for the only instance of the state
 ** machine described in sc. */
#if (defined ComM_HsmMain)
#error ComM_HsmMain already defined
#endif
#define ComM_HsmMain(sc) ((void)COMM_HSMMAININST((sc),0U))
#endif

/** \brief Process events of a state chart in one state machine instance
 ** \param sc state chart
 ** \param instIdx index of target instance
 ** \return Event processing status
 ** \retval TRUE at least one event was processed which may have lead to an
 ** action or a state change
 ** \retval FALSE No action or state change performed due to event
 ** processing */
extern FUNC(boolean, COMM_CODE) COMM_HSMMAININST(
    const ComM_HsmStatechartPtrType sc,
    const uint8                    instIdx);

/*------------------ helper functions --------------------------------------*/

#if (COMM_HSM_TRACE == STD_ON)
/** \brief Switches tracing on or off
 ** \param newValue new tracing, TRUE for on and FALSE for off */
extern FUNC(void, COMM_CODE) ComM_HsmSetTracing(const boolean newValue);
#endif

#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif
/*==================[end of file]===========================================*/
