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

#ifndef UDPNM_HSM_H
#define UDPNM_HSM_H

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

#include <UdpNm_HsmCfg.h>         /* state machine user's configuration */

/*==================[macros]================================================*/

/** \brief Special value for the data type ::UdpNm_HsmStateIdxType
 *
 * Pseudo state index used for non-existend states. E.g. for the parent
 * state of the top state or the init substates of leaf states. */
#if (defined UDPNM_HSM_SIDX_INVALID)
#error UDPNM_HSM_SIDX_INVALID already defined
#endif
#define UDPNM_HSM_SIDX_INVALID            0xFFU
/** \brief Special value for the data type ::UdpNm_HsmActionIdxType
 *
 * Used for non-existend actions. */
#if (defined UDPNM_HSM_AIDX_INVALID)
#error UDPNM_HSM_AIDX_INVALID already defined
#endif
#define UDPNM_HSM_AIDX_INVALID            0xFFU
/** \brief Special value for the data type ::UdpNm_HsmGuardIdxType
 *
 * Used for non-existend guards. */
#if (defined UDPNM_HSM_GIDX_INVALID)
#error UDPNM_HSM_GIDX_INVALID already defined
#endif
#define UDPNM_HSM_GIDX_INVALID            0xFFU

/** \brief Special value for the data type ::UdpNm_HsmEventType
 *
 * Invalid event */
#if (defined UDPNM_HSM_EVENT_INVALID)
#error UDPNM_HSM_EVENT_INVALID already defined
#endif
#define UDPNM_HSM_EVENT_INVALID           0xFFU

#if (defined UDPNM_PL_SF)
#error UDPNM_PL_SF already defined
#endif

#if (defined UDPNM_PDL_SF)
#error UDPNM_PDL_SF already defined
#endif

#if (defined UDPNM_HSMINITINST)
#error UDPNM_HSMINITINST already defined
#endif

#if (defined UDPNM_HSMEMITINST)
#error UDPNM_HSMEMITINST already defined
#endif

#if (defined UDPNM_HSMEMITTOSELFINST)
#error UDPNM_HSMEMITTOSELFINST already defined
#endif

#if (defined UDPNM_HSMMAININST)
#error UDPNM_HSMMAININST already defined
#endif

#if (defined UDPNM_INST_ACCESS)
#error UDPNM_INST_ACCESS already defined
#endif

#if (defined UDPNM_INST)
#error UDPNM_INST already defined
#endif

/* Helper macros to enable building the sources with and without multiple
 * instances support */
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
/* parameter list for action, guard, entry, exit state functions */
#define UDPNM_PL_SF(a)                   a
/* parameter definition list for action, guard, entry, exit state functions */
#define UDPNM_PDL_SF(a)                  a
#define UDPNM_HSMINITINST(a,b)           UdpNm_HsmInitInst(a,b)
#define UDPNM_HSMEMITINST(a,b,c)         UdpNm_HsmEmitInst(a,b,c)
#define UDPNM_HSMEMITTOSELFINST(a,b,c)   UdpNm_HsmEmitToSelfInst(a,b,c)
#define UDPNM_HSMMAININST(a,b)           UdpNm_HsmMainInst(a,b)
/* instance access macro */
#define UDPNM_INST_ACCESS(array,instIdx) ((array)[(instIdx)])
#define UDPNM_INST(instIdx)              (instIdx)
#else
#define UDPNM_PL_SF(a)                   /* nothing */
#define UDPNM_PDL_SF(a)                  void
#define UDPNM_HSMINITINST(a,b)           UdpNm_HsmInitInst(a)
#define UDPNM_HSMEMITINST(a,b,c)         UdpNm_HsmEmitInst(a,c)
#define UDPNM_HSMEMITTOSELFINST(a,b,c)   UdpNm_HsmEmitToSelfInst(a,c)
#define UDPNM_HSMMAININST(a,b)           UdpNm_HsmMainInst(a)
#define UDPNM_INST_ACCESS(array,instIdx) ((array)[0U])
#define UDPNM_INST(instIdx)              0U
#endif

#if (defined UDPNM_HSM_TRACE)
#error UDPNM_HSM_TRACE already defined
#endif
/* enable tracing capabilities only on windows */
#if defined(TS_WINDOWS) && (TS_ARCH_FAMILY == TS_WINDOWS)
#define UDPNM_HSM_TRACE                  STD_ON
#else
#define UDPNM_HSM_TRACE                  STD_OFF
#endif

/*==================[type definitions]======================================*/

/* ** Events ** */

/** \brief Type to encode single events.
 *
 * Event numbers are generated, they are zero-based and consecutive. */
typedef uint8  UdpNm_HsmEventType;

/** \brief Type to encode event masks.
 *
 * This type describes a bit encoded set of events. The event ev of type
 * ::UdpNm_HsmEventType corresponds to the (1 << ev) bit in the
 * ::UdpNm_HsmEventMaskType. */
typedef uint32 UdpNm_HsmEventMaskType;

/** \brief State index type definition */
typedef uint8  UdpNm_HsmStateIdxType;
/** \brief Action index type definition */
typedef uint8  UdpNm_HsmActionIdxType;
/** \brief Guard index type definition */
typedef uint8  UdpNm_HsmGuardIdxType;

/** \brief Hsm instance
 *
 * Struct for an instance of state machine, placed in NOINIT RAM */
struct UdpNm_HsmInstStruct
{
  /** \brief Id of the currently active state */
  UdpNm_HsmStateIdxType   stateId;

  /** \brief Id of source state during a transition */
  UdpNm_HsmStateIdxType   sourceId;

  /** \brief Number of events filled up in the queue */
  uint8                 evQueueFill;

  /** \brief Index into queue where events emitted to self have to be
   * inserted */
  uint8                 evQueueInsertEvNo;
};

/** \brief Type for state machine instance data */
typedef struct UdpNm_HsmInstStruct UdpNm_HsmInstType;

/** \brief Function pointer type for entry/exit/transition action functions */
/* Deviation MISRAC2012-2 */
typedef P2FUNC(void, UDPNM_CODE, UdpNm_HsmActionType)(
  UDPNM_PDL_SF(const uint8 instIdx));

/** \brief Function pointer type for guard functions */
/* Deviation MISRAC2012-2 */
typedef P2FUNC(boolean, UDPNM_CODE, UdpNm_HsmGuardType)(
  UDPNM_PDL_SF(const uint8 instIdx));

/* forward declaration */
struct UdpNm_HsmTranStruct;
/** \brief Type for state transition description */
typedef struct UdpNm_HsmTranStruct UdpNm_HsmTranType;

/** \brief Structure describing a single state of a state chart
 *
 * Type describing states and their relation in the statechart. State
 * descriptions are stored in ROM */
struct UdpNm_HsmStateStruct
{
  /** \brief Mask of non-deferred and non-ignored events
   *
   * Events related to real actions */
  UdpNm_HsmEventMaskType                         actionEventMask;
  /** \brief Mask of ignored events */
  UdpNm_HsmEventMaskType                         ignoredEventMask;

  /** \brief Pointer to array of transitions from this state */
  P2CONST(UdpNm_HsmTranType, TYPEDEF, UDPNM_CONST) trans;

  /** \brief Parent state ID */
  UdpNm_HsmStateIdxType                          parentId;
  /** \brief Init substate, if initId == ::UDPNM_HSM_SIDX_INVALID there is no
   * substate */
  UdpNm_HsmStateIdxType                          initId;
  /** \brief Function pointer of entry handler function */
  UdpNm_HsmActionIdxType                         entryFnIdx;

  /** \brief Function pointer of exit handler function */
  UdpNm_HsmActionIdxType                         exitFnIdx;

  /** \brief Function pointer of do handler function */
  UdpNm_HsmActionIdxType                         doFnIdx;

  /** \brief Number of transitions originating from this state */
  uint8                                        numTrans;
};

/** \brief Type for state description */
typedef struct UdpNm_HsmStateStruct UdpNm_HsmStateType;

/** \brief State chart description data
 *
 * This struct is placed in ROM */
struct UdpNm_HsmStatechartStruct
{
  /** \brief Pointer to state description array */
  P2CONST(UdpNm_HsmStateType, TYPEDEF, UDPNM_CONST)     states;

  /** \brief Pointer to array containing the Hsm instance working data in
   * RAM */
  P2VAR(UdpNm_HsmInstType, TYPEDEF, UDPNM_VAR_CLEARED)   instances;

  /** \brief Pointer to array of all entry/exit/transition function
   * pointers */
  P2CONST(UdpNm_HsmActionType, TYPEDEF, UDPNM_CONST)    actions;

  /** \brief Pointer to array of all guard function pointers */
  P2CONST(UdpNm_HsmGuardType, TYPEDEF, UDPNM_CONST)     guards;

  /** \brief Pointer to event counter array
   *
   * Total array size equals to (numInstances * numEvents).
   *
   *  for instance #n are located in array fields events[n*numEvents]
   * .. events[(n+1)*numEvents-1] */
  P2VAR(uint8, TYPEDEF, UDPNM_VAR_CLEARED)             evCounters;

  /** \brief Pointer to event queue for this hsm instance
   *
   * Total array size equals to (numInstances * numEvents).
   *
   * Events for instance #n are located in array fields events[n*numEvents]
   * .. events[(n+1)*numEvents-1] */
  P2VAR(UdpNm_HsmEventType, TYPEDEF, UDPNM_VAR_CLEARED)  events;

#if defined(TS_WINDOWS) && (TS_ARCH_FAMILY == TS_WINDOWS)
  /* name of the HSM (for debugging on Windows) */
  /* Deviation MISRAC2012-1 */
  P2CONST(char, UDPNM_CONST, UDPNM_CONST)                                hsmName;
  /* Deviation MISRAC2012-1, MISRAC2012-2 <+4> */
  P2CONST(P2CONST(char, UDPNM_CONST, UDPNM_CONST), UDPNM_CONST, UDPNM_CONST) stateNames;
  P2CONST(P2CONST(char, UDPNM_CONST, UDPNM_CONST), UDPNM_CONST, UDPNM_CONST) actionNames;
  P2CONST(P2CONST(char, UDPNM_CONST, UDPNM_CONST), UDPNM_CONST, UDPNM_CONST) guardNames;
  P2CONST(P2CONST(char, UDPNM_CONST, UDPNM_CONST), UDPNM_CONST, UDPNM_CONST) eventNames;
#endif

  /** \brief Number of available events in the state chart */
  uint8                                             numEvents;

  /** \brief Id of the state chart */
  uint8                                             stateChartId;

  /** \brief Number of hsm instances */
  uint8                                             numInstances;
  /** \brief ID of the top state */
  UdpNm_HsmStateIdxType                               topStateId;
};

/** \brief Type for state chart description */
typedef struct UdpNm_HsmStatechartStruct UdpNm_HsmStatechartType;

/** \brief Pointer type to state chart description */
typedef P2CONST(UdpNm_HsmStatechartType, TYPEDEF, UDPNM_CONST)
  UdpNm_HsmStatechartPtrType;


/** \brief State transition
 *
 * Struct type describing a transition in the statechart. Transition
 * descriptions are stored in ROM */
struct UdpNm_HsmTranStruct
{
  /** \brief Pointer to array of transition step indices */
  P2CONST(UdpNm_HsmActionIdxType, TYPEDEF, UDPNM_CONST) steps;

  /** \brief Number of transition steps */
  uint8                                             numSteps;

  /** \brief Triggering event for transition */
  UdpNm_HsmEventType                                  event;

  /** \brief Index of transition guard function */
  UdpNm_HsmGuardIdxType                               guardFnIdx;

  /** \brief ID of the target state */
  UdpNm_HsmStateIdxType                               target;
};

/*==================[external function declarations]========================*/

#define UDPNM_START_SEC_CODE
#include <UdpNm_MemMap.h>

/*------------------ init functions ----------------------------------------*/

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Init all hsm instances of single state chart
 **
 ** \param sc pointer to state chart which shall be initialized */
extern FUNC(void, UDPNM_CODE) UdpNm_HsmInit(const UdpNm_HsmStatechartPtrType sc);
#else
/** \brief Init hsm instance of single state chart
 **
 ** \param sc pointer to state chart which shall be initialized */
#if (defined UdpNm_HsmInit)
#error UdpNm_HsmInit already defined
#endif
#define UdpNm_HsmInit(sc) UDPNM_HSMINITINST((sc),0U)
#endif

/** \brief Init single instance of one hsm
 **
 ** \param sc pointer to state chart which shall be initialized
 ** \param instIdx index of target instance */
extern FUNC(void, UDPNM_CODE) UDPNM_HSMINITINST(
  const UdpNm_HsmStatechartPtrType sc,
  const uint8                    instIdx);

/*------------------ emit functions ----------------------------------------*/

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Send an event to all instances of a state chart
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to all instances of the state machine. */
extern FUNC(void, UDPNM_CODE) UdpNm_HsmEmit(
  const UdpNm_HsmStatechartPtrType sc,
  const UdpNm_HsmEventType         event);

/** \brief Send an event to all instances of a state chart
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to all instances of the state machine. */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_HsmEmitToSelf(
  const UdpNm_HsmStatechartPtrType sc,
  const UdpNm_HsmEventType         event);
#else
/** \brief Send an event to a state chart
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to the only instance of the state machine. */
#if (defined UdpNm_HsmEmit)
#error UdpNm_HsmEmit already defined
#endif
#define UdpNm_HsmEmit(sc,event)       UdpNm_HsmEmitInst((sc),(event))

/** \brief Send an event to a state chart from
 ** within the state machine functions
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to the only instance of the state machine. */
#if (defined UdpNm_HsmEmitToSelf)
#error UdpNm_HsmEmitToSelf already defined
#endif
#define UdpNm_HsmEmitToSelf(sc,event) UdpNm_HsmEmitToSelfInst((sc),(event))

#endif

/** \brief Send an event to a specific instance of the state machine
 ** \param sc target state chart
 ** \param instIdx index of target instance
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow */
extern FUNC(void, UDPNM_CODE) UDPNM_HSMEMITINST(
  const UdpNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const UdpNm_HsmEventType         event);

/** \brief Send an event to a specific instance of the state machine from
 ** within the state machine functions
 ** \param sc target state chart
 ** \param instIdx index of target instance
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow */
extern FUNC(void, UDPNM_CODE) UDPNM_HSMEMITTOSELFINST(
  const UdpNm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const UdpNm_HsmEventType         event);

/*------------------ main functions ----------------------------------------*/

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Process events for one state chart
 ** \param sc pointer to state charts
 **
 ** This function calls UdpNm_HsmMainInst() for all instances of the state
 ** machine described in sc until all events are processed. */
extern FUNC(void, UDPNM_CODE) UdpNm_HsmMain(const UdpNm_HsmStatechartPtrType sc);
#else
/** \brief Process events for one state chart
 ** \param sc pointer to state charts
 **
 ** This function calls UdpNm_HsmMainInst() for the only instance of the state
 ** machine described in sc. */
#if (defined UdpNm_HsmMain)
#error UdpNm_HsmMain already defined
#endif
#define UdpNm_HsmMain(sc) ((void)UDPNM_HSMMAININST((sc),0U))
#endif

/** \brief Process events of a state chart in one state machine instance
 ** \param sc state chart
 ** \param instIdx index of target instance
 ** \return Event processing status
 ** \retval TRUE at least one event was processed which may have lead to an
 ** action or a state change
 ** \retval FALSE No action or state change performed due to event
 ** processing */
extern FUNC(boolean, UDPNM_CODE) UDPNM_HSMMAININST(
    const UdpNm_HsmStatechartPtrType sc,
    const uint8                    instIdx);

/*------------------ helper functions --------------------------------------*/

#if (UDPNM_HSM_TRACE == STD_ON)
/** \brief Switches tracing on or off
 ** \param newValue new tracing, TRUE for on and FALSE for off */
extern FUNC(void, UDPNM_CODE) UdpNm_HsmSetTracing(const boolean newValue);
#endif

#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif
/*==================[end of file]===========================================*/
