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

#ifndef FRNM_HSM_H
#define FRNM_HSM_H

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

#include <FrNm_HsmCfg.h>         /* state machine user's configuration */

/*==================[macros]================================================*/

/** \brief Special value for the data type ::FrNm_HsmStateIdxType
 *
 * Pseudo state index used for non-existend states. E.g. for the parent
 * state of the top state or the init substates of leaf states. */
#if (defined FRNM_HSM_SIDX_INVALID)
#error FRNM_HSM_SIDX_INVALID already defined
#endif
#define FRNM_HSM_SIDX_INVALID            0xFFU
/** \brief Special value for the data type ::FrNm_HsmActionIdxType
 *
 * Used for non-existend actions. */
#if (defined FRNM_HSM_AIDX_INVALID)
#error FRNM_HSM_AIDX_INVALID already defined
#endif
#define FRNM_HSM_AIDX_INVALID            0xFFU
/** \brief Special value for the data type ::FrNm_HsmGuardIdxType
 *
 * Used for non-existend guards. */
#if (defined FRNM_HSM_GIDX_INVALID)
#error FRNM_HSM_GIDX_INVALID already defined
#endif
#define FRNM_HSM_GIDX_INVALID            0xFFU

/** \brief Special value for the data type ::FrNm_HsmEventType
 *
 * Invalid event */
#if (defined FRNM_HSM_EVENT_INVALID)
#error FRNM_HSM_EVENT_INVALID already defined
#endif
#define FRNM_HSM_EVENT_INVALID           0xFFU

#if (defined FRNM_PL_SF)
#error FRNM_PL_SF already defined
#endif

#if (defined FRNM_PDL_SF)
#error FRNM_PDL_SF already defined
#endif

#if (defined FRNM_HSMINITINST)
#error FRNM_HSMINITINST already defined
#endif

#if (defined FRNM_HSMEMITINST)
#error FRNM_HSMEMITINST already defined
#endif

#if (defined FRNM_HSMEMITTOSELFINST)
#error FRNM_HSMEMITTOSELFINST already defined
#endif

#if (defined FRNM_HSMMAININST)
#error FRNM_HSMMAININST already defined
#endif

#if (defined FRNM_INST_ACCESS)
#error FRNM_INST_ACCESS already defined
#endif

#if (defined FRNM_INST)
#error FRNM_INST already defined
#endif

/* Helper macros to enable building the sources with and without multiple
 * instances support */
#if (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
/* parameter list for action, guard, entry, exit state functions */
#define FRNM_PL_SF(a)                   a
/* parameter definition list for action, guard, entry, exit state functions */
#define FRNM_PDL_SF(a)                  a
#define FRNM_HSMINITINST(a,b)           FrNm_HsmInitInst(a,b)
#define FRNM_HSMEMITINST(a,b,c)         FrNm_HsmEmitInst(a,b,c)
#define FRNM_HSMEMITTOSELFINST(a,b,c)   FrNm_HsmEmitToSelfInst(a,b,c)
#define FRNM_HSMMAININST(a,b)           FrNm_HsmMainInst(a,b)
/* instance access macro */
#define FRNM_INST_ACCESS(array,instIdx) ((array)[(instIdx)])
#define FRNM_INST(instIdx)              (instIdx)
#else
#define FRNM_PL_SF(a)                   /* nothing */
#define FRNM_PDL_SF(a)                  void
#define FRNM_HSMINITINST(a,b)           FrNm_HsmInitInst(a)
#define FRNM_HSMEMITINST(a,b,c)         FrNm_HsmEmitInst(a,c)
#define FRNM_HSMEMITTOSELFINST(a,b,c)   FrNm_HsmEmitToSelfInst(a,c)
#define FRNM_HSMMAININST(a,b)           FrNm_HsmMainInst(a)
#define FRNM_INST_ACCESS(array,instIdx) ((array)[0U])
#define FRNM_INST(instIdx)              0U
#endif

#if (defined FRNM_HSM_TRACE)
#error FRNM_HSM_TRACE already defined
#endif
/* enable tracing capabilities only on windows */
#if defined(TS_WINDOWS) && (TS_ARCH_FAMILY == TS_WINDOWS)
#define FRNM_HSM_TRACE                  STD_ON
#else
#define FRNM_HSM_TRACE                  STD_OFF
#endif

/*==================[type definitions]======================================*/

/* ** Events ** */

/** \brief Type to encode single events.
 *
 * Event numbers are generated, they are zero-based and consecutive. */
typedef uint8  FrNm_HsmEventType;

/** \brief Type to encode event masks.
 *
 * This type describes a bit encoded set of events. The event ev of type
 * ::FrNm_HsmEventType corresponds to the (1 << ev) bit in the
 * ::FrNm_HsmEventMaskType. */
typedef uint32 FrNm_HsmEventMaskType;

/** \brief State index type definition */
typedef uint8  FrNm_HsmStateIdxType;
/** \brief Action index type definition */
typedef uint8  FrNm_HsmActionIdxType;
/** \brief Guard index type definition */
typedef uint8  FrNm_HsmGuardIdxType;

/** \brief Hsm instance
 *
 * Struct for an instance of state machine, placed in NOINIT RAM */
struct FrNm_HsmInstStruct
{
  /** \brief Id of the currently active state */
  FrNm_HsmStateIdxType   stateId;

  /** \brief Id of source state during a transition */
  FrNm_HsmStateIdxType   sourceId;

  /** \brief Number of events filled up in the queue */
  uint8                 evQueueFill;

  /** \brief Index into queue where events emitted to self have to be
   * inserted */
  uint8                 evQueueInsertEvNo;
};

/** \brief Type for state machine instance data */
typedef struct FrNm_HsmInstStruct FrNm_HsmInstType;

/** \brief Function pointer type for entry/exit/transition action functions */
/* Deviation MISRAC2012-2 */
typedef P2FUNC(void, FRNM_CODE, FrNm_HsmActionType)(
  FRNM_PDL_SF(const uint8 instIdx));

/** \brief Function pointer type for guard functions */
/* Deviation MISRAC2012-2 */
typedef P2FUNC(boolean, FRNM_CODE, FrNm_HsmGuardType)(
  FRNM_PDL_SF(const uint8 instIdx));

/* forward declaration */
struct FrNm_HsmTranStruct;
/** \brief Type for state transition description */
typedef struct FrNm_HsmTranStruct FrNm_HsmTranType;

/** \brief Structure describing a single state of a state chart
 *
 * Type describing states and their relation in the statechart. State
 * descriptions are stored in ROM */
struct FrNm_HsmStateStruct
{
  /** \brief Mask of non-deferred and non-ignored events
   *
   * Events related to real actions */
  FrNm_HsmEventMaskType                         actionEventMask;
  /** \brief Mask of ignored events */
  FrNm_HsmEventMaskType                         ignoredEventMask;

  /** \brief Pointer to array of transitions from this state */
  P2CONST(FrNm_HsmTranType, TYPEDEF, FRNM_CONST) trans;

  /** \brief Parent state ID */
  FrNm_HsmStateIdxType                          parentId;
  /** \brief Init substate, if initId == ::FRNM_HSM_SIDX_INVALID there is no
   * substate */
  FrNm_HsmStateIdxType                          initId;
  /** \brief Function pointer of entry handler function */
  FrNm_HsmActionIdxType                         entryFnIdx;

  /** \brief Function pointer of exit handler function */
  FrNm_HsmActionIdxType                         exitFnIdx;

  /** \brief Function pointer of do handler function */
  FrNm_HsmActionIdxType                         doFnIdx;

  /** \brief Number of transitions originating from this state */
  uint8                                        numTrans;
};

/** \brief Type for state description */
typedef struct FrNm_HsmStateStruct FrNm_HsmStateType;

/** \brief State chart description data
 *
 * This struct is placed in ROM */
struct FrNm_HsmStatechartStruct
{
  /** \brief Pointer to state description array */
  P2CONST(FrNm_HsmStateType, TYPEDEF, FRNM_CONST)     states;

  /** \brief Pointer to array containing the Hsm instance working data in
   * RAM */
  P2VAR(FrNm_HsmInstType, TYPEDEF, FRNM_VAR_CLEARED)   instances;

  /** \brief Pointer to array of all entry/exit/transition function
   * pointers */
  P2CONST(FrNm_HsmActionType, TYPEDEF, FRNM_CONST)    actions;

  /** \brief Pointer to array of all guard function pointers */
  P2CONST(FrNm_HsmGuardType, TYPEDEF, FRNM_CONST)     guards;

  /** \brief Pointer to event counter array
   *
   * Total array size equals to (numInstances * numEvents).
   *
   *  for instance #n are located in array fields events[n*numEvents]
   * .. events[(n+1)*numEvents-1] */
  P2VAR(uint8, TYPEDEF, FRNM_VAR_CLEARED)             evCounters;

  /** \brief Pointer to event queue for this hsm instance
   *
   * Total array size equals to (numInstances * numEvents).
   *
   * Events for instance #n are located in array fields events[n*numEvents]
   * .. events[(n+1)*numEvents-1] */
  P2VAR(FrNm_HsmEventType, TYPEDEF, FRNM_VAR_CLEARED)  events;

#if defined(TS_WINDOWS) && (TS_ARCH_FAMILY == TS_WINDOWS)
  /* name of the HSM (for debugging on Windows) */
  /* Deviation MISRAC2012-1 */
  P2CONST(char, FRNM_CONST, FRNM_CONST)                                hsmName;
  /* Deviation MISRAC2012-1, MISRAC2012-2 <+4> */
  P2CONST(P2CONST(char, FRNM_CONST, FRNM_CONST), FRNM_CONST, FRNM_CONST) stateNames;
  P2CONST(P2CONST(char, FRNM_CONST, FRNM_CONST), FRNM_CONST, FRNM_CONST) actionNames;
  P2CONST(P2CONST(char, FRNM_CONST, FRNM_CONST), FRNM_CONST, FRNM_CONST) guardNames;
  P2CONST(P2CONST(char, FRNM_CONST, FRNM_CONST), FRNM_CONST, FRNM_CONST) eventNames;
#endif

  /** \brief Number of available events in the state chart */
  uint8                                             numEvents;

  /** \brief Id of the state chart */
  uint8                                             stateChartId;

  /** \brief Number of hsm instances */
  uint8                                             numInstances;
  /** \brief ID of the top state */
  FrNm_HsmStateIdxType                               topStateId;
};

/** \brief Type for state chart description */
typedef struct FrNm_HsmStatechartStruct FrNm_HsmStatechartType;

/** \brief Pointer type to state chart description */
typedef P2CONST(FrNm_HsmStatechartType, TYPEDEF, FRNM_CONST)
  FrNm_HsmStatechartPtrType;


/** \brief State transition
 *
 * Struct type describing a transition in the statechart. Transition
 * descriptions are stored in ROM */
struct FrNm_HsmTranStruct
{
  /** \brief Pointer to array of transition step indices */
  P2CONST(FrNm_HsmActionIdxType, TYPEDEF, FRNM_CONST) steps;

  /** \brief Number of transition steps */
  uint8                                             numSteps;

  /** \brief Triggering event for transition */
  FrNm_HsmEventType                                  event;

  /** \brief Index of transition guard function */
  FrNm_HsmGuardIdxType                               guardFnIdx;

  /** \brief ID of the target state */
  FrNm_HsmStateIdxType                               target;
};

/*==================[external function declarations]========================*/

#define FRNM_START_SEC_CODE
#include <FrNm_MemMap.h>

/*------------------ init functions ----------------------------------------*/

#if (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Init all hsm instances of single state chart
 **
 ** \param sc pointer to state chart which shall be initialized */
extern FUNC(void, FRNM_CODE) FrNm_HsmInit(const FrNm_HsmStatechartPtrType sc);
#else
/** \brief Init hsm instance of single state chart
 **
 ** \param sc pointer to state chart which shall be initialized */
#if (defined FrNm_HsmInit)
#error FrNm_HsmInit already defined
#endif
#define FrNm_HsmInit(sc) FRNM_HSMINITINST((sc),0U)
#endif

/** \brief Init single instance of one hsm
 **
 ** \param sc pointer to state chart which shall be initialized
 ** \param instIdx index of target instance */
extern FUNC(void, FRNM_CODE) FRNM_HSMINITINST(
  const FrNm_HsmStatechartPtrType sc,
  const uint8                    instIdx);

/*------------------ emit functions ----------------------------------------*/

#if (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Send an event to all instances of a state chart
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to all instances of the state machine. */
extern FUNC(void, FRNM_CODE) FrNm_HsmEmit(
  const FrNm_HsmStatechartPtrType sc,
  const FrNm_HsmEventType         event);

/** \brief Send an event to all instances of a state chart
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to all instances of the state machine. */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_HsmEmitToSelf(
  const FrNm_HsmStatechartPtrType sc,
  const FrNm_HsmEventType         event);
#else
/** \brief Send an event to a state chart
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to the only instance of the state machine. */
#if (defined FrNm_HsmEmit)
#error FrNm_HsmEmit already defined
#endif
#define FrNm_HsmEmit(sc,event)       FrNm_HsmEmitInst((sc),(event))

/** \brief Send an event to a state chart from
 ** within the state machine functions
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to the only instance of the state machine. */
#if (defined FrNm_HsmEmitToSelf)
#error FrNm_HsmEmitToSelf already defined
#endif
#define FrNm_HsmEmitToSelf(sc,event) FrNm_HsmEmitToSelfInst((sc),(event))

#endif

/** \brief Send an event to a specific instance of the state machine
 ** \param sc target state chart
 ** \param instIdx index of target instance
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow */
extern FUNC(void, FRNM_CODE) FRNM_HSMEMITINST(
  const FrNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const FrNm_HsmEventType         event);

/** \brief Send an event to a specific instance of the state machine from
 ** within the state machine functions
 ** \param sc target state chart
 ** \param instIdx index of target instance
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow */
extern FUNC(void, FRNM_CODE) FRNM_HSMEMITTOSELFINST(
  const FrNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const FrNm_HsmEventType         event);

/*------------------ main functions ----------------------------------------*/

#if (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Process events for one state chart
 ** \param sc pointer to state charts
 **
 ** This function calls FrNm_HsmMainInst() for all instances of the state
 ** machine described in sc until all events are processed. */
extern FUNC(void, FRNM_CODE) FrNm_HsmMain(const FrNm_HsmStatechartPtrType sc);
#else
/** \brief Process events for one state chart
 ** \param sc pointer to state charts
 **
 ** This function calls FrNm_HsmMainInst() for the only instance of the state
 ** machine described in sc. */
#if (defined FrNm_HsmMain)
#error FrNm_HsmMain already defined
#endif
#define FrNm_HsmMain(sc) ((void)FRNM_HSMMAININST((sc),0U))
#endif

/** \brief Process events of a state chart in one state machine instance
 ** \param sc state chart
 ** \param instIdx index of target instance
 ** \return Event processing status
 ** \retval TRUE at least one event was processed which may have lead to an
 ** action or a state change
 ** \retval FALSE No action or state change performed due to event
 ** processing */
extern FUNC(boolean, FRNM_CODE) FRNM_HSMMAININST(
    const FrNm_HsmStatechartPtrType sc,
    const uint8                    instIdx);

/*------------------ helper functions --------------------------------------*/

#if (FRNM_HSM_TRACE == STD_ON)
/** \brief Switches tracing on or off
 ** \param newValue new tracing, TRUE for on and FALSE for off */
extern FUNC(void, FRNM_CODE) FrNm_HsmSetTracing(const boolean newValue);
#endif

#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif
/*==================[end of file]===========================================*/
