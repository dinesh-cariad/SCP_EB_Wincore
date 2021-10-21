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

#ifndef CANNM_HSM_H
#define CANNM_HSM_H

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

#include <CanNm_HsmCfg.h>         /* state machine user's configuration */

/*==================[macros]================================================*/

/** \brief Special value for the data type ::CanNm_HsmStateIdxType
 *
 * Pseudo state index used for non-existend states. E.g. for the parent
 * state of the top state or the init substates of leaf states. */
#if (defined CANNM_HSM_SIDX_INVALID)
#error CANNM_HSM_SIDX_INVALID already defined
#endif
#define CANNM_HSM_SIDX_INVALID            0xFFU
/** \brief Special value for the data type ::CanNm_HsmActionIdxType
 *
 * Used for non-existend actions. */
#if (defined CANNM_HSM_AIDX_INVALID)
#error CANNM_HSM_AIDX_INVALID already defined
#endif
#define CANNM_HSM_AIDX_INVALID            0xFFU
/** \brief Special value for the data type ::CanNm_HsmGuardIdxType
 *
 * Used for non-existend guards. */
#if (defined CANNM_HSM_GIDX_INVALID)
#error CANNM_HSM_GIDX_INVALID already defined
#endif
#define CANNM_HSM_GIDX_INVALID            0xFFU

/** \brief Special value for the data type ::CanNm_HsmEventType
 *
 * Invalid event */
#if (defined CANNM_HSM_EVENT_INVALID)
#error CANNM_HSM_EVENT_INVALID already defined
#endif
#define CANNM_HSM_EVENT_INVALID           0xFFU

#if (defined CANNM_PL_SF)
#error CANNM_PL_SF already defined
#endif

#if (defined CANNM_PDL_SF)
#error CANNM_PDL_SF already defined
#endif

#if (defined CANNM_HSMINITINST)
#error CANNM_HSMINITINST already defined
#endif

#if (defined CANNM_HSMEMITINST)
#error CANNM_HSMEMITINST already defined
#endif

#if (defined CANNM_HSMEMITTOSELFINST)
#error CANNM_HSMEMITTOSELFINST already defined
#endif

#if (defined CANNM_HSMMAININST)
#error CANNM_HSMMAININST already defined
#endif

#if (defined CANNM_INST_ACCESS)
#error CANNM_INST_ACCESS already defined
#endif

#if (defined CANNM_INST)
#error CANNM_INST already defined
#endif

/* Helper macros to enable building the sources with and without multiple
 * instances support */
#if (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
/* parameter list for action, guard, entry, exit state functions */
#define CANNM_PL_SF(a)                   a
/* parameter definition list for action, guard, entry, exit state functions */
#define CANNM_PDL_SF(a)                  a
#define CANNM_HSMINITINST(a,b)           CanNm_HsmInitInst(a,b)
#define CANNM_HSMEMITINST(a,b,c)         CanNm_HsmEmitInst(a,b,c)
#define CANNM_HSMEMITTOSELFINST(a,b,c)   CanNm_HsmEmitToSelfInst(a,b,c)
#define CANNM_HSMMAININST(a,b)           CanNm_HsmMainInst(a,b)
/* instance access macro */
#define CANNM_INST_ACCESS(array,instIdx) ((array)[(instIdx)])
#define CANNM_INST(instIdx)              (instIdx)
#else
#define CANNM_PL_SF(a)                   /* nothing */
#define CANNM_PDL_SF(a)                  void
#define CANNM_HSMINITINST(a,b)           CanNm_HsmInitInst(a)
#define CANNM_HSMEMITINST(a,b,c)         CanNm_HsmEmitInst(a,c)
#define CANNM_HSMEMITTOSELFINST(a,b,c)   CanNm_HsmEmitToSelfInst(a,c)
#define CANNM_HSMMAININST(a,b)           CanNm_HsmMainInst(a)
#define CANNM_INST_ACCESS(array,instIdx) ((array)[0U])
#define CANNM_INST(instIdx)              0U
#endif

#if (defined CANNM_HSM_TRACE)
#error CANNM_HSM_TRACE already defined
#endif
/* enable tracing capabilities only on windows */
#if defined(TS_WINDOWS) && (TS_ARCH_FAMILY == TS_WINDOWS)
#define CANNM_HSM_TRACE                  STD_ON
#else
#define CANNM_HSM_TRACE                  STD_OFF
#endif

/*==================[type definitions]======================================*/

/* ** Events ** */

/** \brief Type to encode single events.
 *
 * Event numbers are generated, they are zero-based and consecutive. */
typedef uint8  CanNm_HsmEventType;

/** \brief Type to encode event masks.
 *
 * This type describes a bit encoded set of events. The event ev of type
 * ::CanNm_HsmEventType corresponds to the (1 << ev) bit in the
 * ::CanNm_HsmEventMaskType. */
typedef uint32 CanNm_HsmEventMaskType;

/** \brief State index type definition */
typedef uint8  CanNm_HsmStateIdxType;
/** \brief Action index type definition */
typedef uint8  CanNm_HsmActionIdxType;
/** \brief Guard index type definition */
typedef uint8  CanNm_HsmGuardIdxType;

/** \brief Hsm instance
 *
 * Struct for an instance of state machine, placed in NOINIT RAM */
struct CanNm_HsmInstStruct
{
  /** \brief Id of the currently active state */
  CanNm_HsmStateIdxType   stateId;

  /** \brief Id of source state during a transition */
  CanNm_HsmStateIdxType   sourceId;

  /** \brief Number of events filled up in the queue */
  uint8                 evQueueFill;

  /** \brief Index into queue where events emitted to self have to be
   * inserted */
  uint8                 evQueueInsertEvNo;
};

/** \brief Type for state machine instance data */
typedef struct CanNm_HsmInstStruct CanNm_HsmInstType;

/** \brief Function pointer type for entry/exit/transition action functions */
/* Deviation MISRAC2012-2 */
typedef P2FUNC(void, CANNM_CODE, CanNm_HsmActionType)(
  CANNM_PDL_SF(const uint8 instIdx));

/** \brief Function pointer type for guard functions */
/* Deviation MISRAC2012-2 */
typedef P2FUNC(boolean, CANNM_CODE, CanNm_HsmGuardType)(
  CANNM_PDL_SF(const uint8 instIdx));

/* forward declaration */
struct CanNm_HsmTranStruct;
/** \brief Type for state transition description */
typedef struct CanNm_HsmTranStruct CanNm_HsmTranType;

/** \brief Structure describing a single state of a state chart
 *
 * Type describing states and their relation in the statechart. State
 * descriptions are stored in ROM */
struct CanNm_HsmStateStruct
{
  /** \brief Mask of non-deferred and non-ignored events
   *
   * Events related to real actions */
  CanNm_HsmEventMaskType                         actionEventMask;
  /** \brief Mask of ignored events */
  CanNm_HsmEventMaskType                         ignoredEventMask;

  /** \brief Pointer to array of transitions from this state */
  P2CONST(CanNm_HsmTranType, TYPEDEF, CANNM_CONST) trans;

  /** \brief Parent state ID */
  CanNm_HsmStateIdxType                          parentId;
  /** \brief Init substate, if initId == ::CANNM_HSM_SIDX_INVALID there is no
   * substate */
  CanNm_HsmStateIdxType                          initId;
  /** \brief Function pointer of entry handler function */
  CanNm_HsmActionIdxType                         entryFnIdx;

  /** \brief Function pointer of exit handler function */
  CanNm_HsmActionIdxType                         exitFnIdx;

  /** \brief Function pointer of do handler function */
  CanNm_HsmActionIdxType                         doFnIdx;

  /** \brief Number of transitions originating from this state */
  uint8                                        numTrans;
};

/** \brief Type for state description */
typedef struct CanNm_HsmStateStruct CanNm_HsmStateType;

/** \brief State chart description data
 *
 * This struct is placed in ROM */
struct CanNm_HsmStatechartStruct
{
  /** \brief Pointer to state description array */
  P2CONST(CanNm_HsmStateType, TYPEDEF, CANNM_CONST)     states;

  /** \brief Pointer to array containing the Hsm instance working data in
   * RAM */
  P2VAR(CanNm_HsmInstType, TYPEDEF, CANNM_VAR_CLEARED)   instances;

  /** \brief Pointer to array of all entry/exit/transition function
   * pointers */
  P2CONST(CanNm_HsmActionType, TYPEDEF, CANNM_CONST)    actions;

  /** \brief Pointer to array of all guard function pointers */
  P2CONST(CanNm_HsmGuardType, TYPEDEF, CANNM_CONST)     guards;

  /** \brief Pointer to event counter array
   *
   * Total array size equals to (numInstances * numEvents).
   *
   *  for instance #n are located in array fields events[n*numEvents]
   * .. events[(n+1)*numEvents-1] */
  P2VAR(uint8, TYPEDEF, CANNM_VAR_CLEARED)             evCounters;

  /** \brief Pointer to event queue for this hsm instance
   *
   * Total array size equals to (numInstances * numEvents).
   *
   * Events for instance #n are located in array fields events[n*numEvents]
   * .. events[(n+1)*numEvents-1] */
  P2VAR(CanNm_HsmEventType, TYPEDEF, CANNM_VAR_CLEARED)  events;

#if defined(TS_WINDOWS) && (TS_ARCH_FAMILY == TS_WINDOWS)
  /* name of the HSM (for debugging on Windows) */
  /* Deviation MISRAC2012-1 */
  P2CONST(char, CANNM_CONST, CANNM_CONST)                                hsmName;
  /* Deviation MISRAC2012-1, MISRAC2012-2 <+4> */
  P2CONST(P2CONST(char, CANNM_CONST, CANNM_CONST), CANNM_CONST, CANNM_CONST) stateNames;
  P2CONST(P2CONST(char, CANNM_CONST, CANNM_CONST), CANNM_CONST, CANNM_CONST) actionNames;
  P2CONST(P2CONST(char, CANNM_CONST, CANNM_CONST), CANNM_CONST, CANNM_CONST) guardNames;
  P2CONST(P2CONST(char, CANNM_CONST, CANNM_CONST), CANNM_CONST, CANNM_CONST) eventNames;
#endif

  /** \brief Number of available events in the state chart */
  uint8                                             numEvents;

  /** \brief Id of the state chart */
  uint8                                             stateChartId;

  /** \brief Number of hsm instances */
  uint8                                             numInstances;
  /** \brief ID of the top state */
  CanNm_HsmStateIdxType                               topStateId;
};

/** \brief Type for state chart description */
typedef struct CanNm_HsmStatechartStruct CanNm_HsmStatechartType;

/** \brief Pointer type to state chart description */
typedef P2CONST(CanNm_HsmStatechartType, TYPEDEF, CANNM_CONST)
  CanNm_HsmStatechartPtrType;


/** \brief State transition
 *
 * Struct type describing a transition in the statechart. Transition
 * descriptions are stored in ROM */
struct CanNm_HsmTranStruct
{
  /** \brief Pointer to array of transition step indices */
  P2CONST(CanNm_HsmActionIdxType, TYPEDEF, CANNM_CONST) steps;

  /** \brief Number of transition steps */
  uint8                                             numSteps;

  /** \brief Triggering event for transition */
  CanNm_HsmEventType                                  event;

  /** \brief Index of transition guard function */
  CanNm_HsmGuardIdxType                               guardFnIdx;

  /** \brief ID of the target state */
  CanNm_HsmStateIdxType                               target;
};

/*==================[external function declarations]========================*/

#define CANNM_START_SEC_CODE
#include <CanNm_MemMap.h>

/*------------------ init functions ----------------------------------------*/

#if (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Init all hsm instances of single state chart
 **
 ** \param sc pointer to state chart which shall be initialized */
extern FUNC(void, CANNM_CODE) CanNm_HsmInit(const CanNm_HsmStatechartPtrType sc);
#else
/** \brief Init hsm instance of single state chart
 **
 ** \param sc pointer to state chart which shall be initialized */
#if (defined CanNm_HsmInit)
#error CanNm_HsmInit already defined
#endif
#define CanNm_HsmInit(sc) CANNM_HSMINITINST((sc),0U)
#endif

/** \brief Init single instance of one hsm
 **
 ** \param sc pointer to state chart which shall be initialized
 ** \param instIdx index of target instance */
extern FUNC(void, CANNM_CODE) CANNM_HSMINITINST(
  const CanNm_HsmStatechartPtrType sc,
  const uint8                    instIdx);

/*------------------ emit functions ----------------------------------------*/

#if (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Send an event to all instances of a state chart
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to all instances of the state machine. */
extern FUNC(void, CANNM_CODE) CanNm_HsmEmit(
  const CanNm_HsmStatechartPtrType sc,
  const CanNm_HsmEventType         event);

/** \brief Send an event to all instances of a state chart
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to all instances of the state machine. */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_HsmEmitToSelf(
  const CanNm_HsmStatechartPtrType sc,
  const CanNm_HsmEventType         event);
#else
/** \brief Send an event to a state chart
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to the only instance of the state machine. */
#if (defined CanNm_HsmEmit)
#error CanNm_HsmEmit already defined
#endif
#define CanNm_HsmEmit(sc,event)       CanNm_HsmEmitInst((sc),(event))

/** \brief Send an event to a state chart from
 ** within the state machine functions
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to the only instance of the state machine. */
#if (defined CanNm_HsmEmitToSelf)
#error CanNm_HsmEmitToSelf already defined
#endif
#define CanNm_HsmEmitToSelf(sc,event) CanNm_HsmEmitToSelfInst((sc),(event))

#endif

/** \brief Send an event to a specific instance of the state machine
 ** \param sc target state chart
 ** \param instIdx index of target instance
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow */
extern FUNC(void, CANNM_CODE) CANNM_HSMEMITINST(
  const CanNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const CanNm_HsmEventType         event);

/** \brief Send an event to a specific instance of the state machine from
 ** within the state machine functions
 ** \param sc target state chart
 ** \param instIdx index of target instance
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow */
extern FUNC(void, CANNM_CODE) CANNM_HSMEMITTOSELFINST(
  const CanNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const CanNm_HsmEventType         event);

/*------------------ main functions ----------------------------------------*/

#if (CANNM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Process events for one state chart
 ** \param sc pointer to state charts
 **
 ** This function calls CanNm_HsmMainInst() for all instances of the state
 ** machine described in sc until all events are processed. */
extern FUNC(void, CANNM_CODE) CanNm_HsmMain(const CanNm_HsmStatechartPtrType sc);
#else
/** \brief Process events for one state chart
 ** \param sc pointer to state charts
 **
 ** This function calls CanNm_HsmMainInst() for the only instance of the state
 ** machine described in sc. */
#if (defined CanNm_HsmMain)
#error CanNm_HsmMain already defined
#endif
#define CanNm_HsmMain(sc) ((void)CANNM_HSMMAININST((sc),0U))
#endif

/** \brief Process events of a state chart in one state machine instance
 ** \param sc state chart
 ** \param instIdx index of target instance
 ** \return Event processing status
 ** \retval TRUE at least one event was processed which may have lead to an
 ** action or a state change
 ** \retval FALSE No action or state change performed due to event
 ** processing */
extern FUNC(boolean, CANNM_CODE) CANNM_HSMMAININST(
    const CanNm_HsmStatechartPtrType sc,
    const uint8                    instIdx);

/*------------------ helper functions --------------------------------------*/

#if (CANNM_HSM_TRACE == STD_ON)
/** \brief Switches tracing on or off
 ** \param newValue new tracing, TRUE for on and FALSE for off */
extern FUNC(void, CANNM_CODE) CanNm_HsmSetTracing(const boolean newValue);
#endif

#define CANNM_STOP_SEC_CODE
#include <CanNm_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif
/*==================[end of file]===========================================*/
