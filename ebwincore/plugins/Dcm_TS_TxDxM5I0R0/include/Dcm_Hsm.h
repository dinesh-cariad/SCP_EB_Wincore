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

#ifndef DCM_HSM_H
#define DCM_HSM_H

/* This file contains public interface for the hierarchical state machine
 * driver. */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 6.3 (advisory)
 *     'typedefs' that indicate size and signedness should be used in place
 *     of the basic types.
 *
 *     Reason:
 *     Platform depended 'char' type is only used to define debugging
 *     strings on Windows.
 *
 *  MISRA-2) Deviated Rule: 19.10 (required)
 *     Parameter instance shall be enclosed in parentheses.
 *
 *     Reason:
 *     The macro is used in function parameter declarations and definitions
 *     where the number of parentheses matter.
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 20.7 (required)
  *     "Expressions resulting from the expansion of macro parameters shall
  *     be enclosed in parentheses."
  *
  *     Reason:
  *     The macro is used in function parameter declarations and definitions
  *     where the number of parentheses matter.
  *
  *  MISRAC2012-2) Deviated Rule: D4.6 (advisory)
  *     "typedefs that indicate size and signedness should be used in place of the basic numerical
  *     types."
  *
  *     Reason:
  *     Platform depended 'char' type is only used to define debugging
  *     strings on Windows.
  */

/*==================[inclusions]============================================*/

#include <Std_Types.h>          /* AUTOSAR standard types */
#include <TSAutosar.h>          /* TS_ARCH_FAMILY */

#include <Dcm_HsmCfg.h>         /* state machine user's configuration */

/*==================[macros]================================================*/

/** \brief Special value for the data type ::Dcm_HsmStateIdxType
 *
 * Pseudo state index used for non-existend states. E.g. for the parent
 * state of the top state or the init substates of leaf states. */
#if (defined DCM_HSM_SIDX_INVALID)
  #error "DCM_HSM_SIDX_INVALID is already defined"
#endif
#define DCM_HSM_SIDX_INVALID            0xFFU

/** \brief Special value for the data type ::Dcm_HsmActionIdxType
 *
 * Used for non-existend actions. */
#if (defined DCM_HSM_AIDX_INVALID)
  #error "DCM_HSM_AIDX_INVALID is already defined"
#endif
#define DCM_HSM_AIDX_INVALID            0xFFU

/** \brief Special value for the data type ::Dcm_HsmGuardIdxType
 *
 * Used for non-existend guards. */
#if (defined DCM_HSM_GIDX_INVALID)
  #error "DCM_HSM_GIDX_INVALID is already defined"
#endif
#define DCM_HSM_GIDX_INVALID            0xFFU

/** \brief Special value for the data type ::Dcm_HsmEventType
 *
 * Invalid event */
#if (defined DCM_HSM_EVENT_INVALID)
  #error "DCM_HSM_EVENT_INVALID is already defined"
#endif
#define DCM_HSM_EVENT_INVALID           0xFFU

/* Helper macros to enable building the sources with and without multiple
 * instances support */
#if (DCM_HSM_INST_MULTI_ENABLED == STD_ON)
/* parameter list for action, guard, entry, exit state functions */

/* Deviation MISRA-2 */
#if (defined DCM_PL_SF)
  #error "DCM_PL_SF is already defined"
#endif
#define DCM_PL_SF(a)                   (a)
/* parameter definition list for action, guard, entry, exit state functions */
#if (defined DCM_PDL_SF)
  #error "DCM_PDL_SF is already defined"
#endif
#define DCM_PDL_SF(a)                  a
#if (defined DCM_HSMINITINST)
  #error "DCM_HSMINITINST is already defined"
#endif
#define DCM_HSMINITINST(a,b)           Dcm_HsmInitInst(a,b)
#if (defined DCM_HSMEMITINST)
  #error "DCM_HSMEMITINST is already defined"
#endif
#define DCM_HSMEMITINST(a,b,c)         Dcm_HsmEmitInst(a,b,c)
#if (defined DCM_HSMEMITTOSELFINST)
  #error "DCM_HSMEMITTOSELFINST is already defined"
#endif
#define DCM_HSMEMITTOSELFINST(a,b,c)   Dcm_HsmEmitToSelfInst(a,b,c)
#if (defined DCM_HSMMAININST)
  #error "DCM_HSMMAININST is already defined"
#endif
#define DCM_HSMMAININST(a,b)           Dcm_HsmMainInst(a,b)
/* instance access macro */
/* Deviation MISRA-2 */
#if (defined DCM_INST_ACCESS)
  #error "DCM_INST_ACCESS is already defined"
#endif
#define DCM_INST_ACCESS(array,instIdx) (array[instIdx])
#if (defined DCM_INST)
  #error "DCM_INST is already defined"
#endif
#define DCM_INST(instIdx)              (instIdx)
#else
#if (defined DCM_PL_SF)
  #error "DCM_PL_SF is already defined"
#endif
#define DCM_PL_SF(a)                   /* nothing */
#if (defined DCM_PDL_SF)
  #error "DCM_PDL_SF is already defined"
#endif
#define DCM_PDL_SF(a)                  void
#if (defined DCM_HSMINITINST)
  #error "DCM_HSMINITINST is already defined"
#endif
#define DCM_HSMINITINST(a,b)           Dcm_HsmInitInst(a)
#if (defined DCM_HSMEMITINST)
  #error "DCM_HSMEMITINST is already defined"
#endif
#define DCM_HSMEMITINST(a,b,c)         Dcm_HsmEmitInst(a,c)
#if (defined DCM_HSMEMITTOSELFINST)
  #error "DCM_HSMEMITTOSELFINST is already defined"
#endif
#define DCM_HSMEMITTOSELFINST(a,b,c)   Dcm_HsmEmitToSelfInst(a,c)
#if (defined DCM_HSMMAININST)
  #error "DCM_HSMMAININST is already defined"
#endif
#define DCM_HSMMAININST(a,b)           Dcm_HsmMainInst(a)
/* Deviation MISRA-2 */
#if (defined DCM_INST_ACCESS)
  #error "DCM_INST_ACCESS is already defined"
#endif
#define DCM_INST_ACCESS(array,instIdx) (array[0])
#if (defined DCM_INST)
  #error "DCM_INST is already defined"
#endif
#define DCM_INST(instIdx)              0
#endif

#if (defined DCM_HSM_TRACE)
  #error "DCM_HSM_TRACE is already defined"
#endif
/* enable tracing capabilities only on windows */
#if defined(TS_WINDOWS) && (TS_ARCH_FAMILY == TS_WINDOWS)
#define DCM_HSM_TRACE                  STD_ON
#else
#define DCM_HSM_TRACE                  STD_OFF
#endif

/*==================[type definitions]======================================*/

/** \brief Type to encode single events.
 *
 * Event numbers are generated, they are zero-based and consecutive. */
typedef uint8  Dcm_HsmEventType;

/** \brief Type to encode event masks.
 *
 * This type describes a bit encoded set of events. The event ev of type
 * ::Dcm_HsmEventType corresponds to the (1 << ev) bit in the
 * ::Dcm_HsmEventMaskType. */
typedef uint32 Dcm_HsmEventMaskType;

/** \brief State index type definition */
typedef uint8  Dcm_HsmStateIdxType;
/** \brief Action index type definition */
typedef uint8  Dcm_HsmActionIdxType;
/** \brief Guard index type definition */
typedef uint8  Dcm_HsmGuardIdxType;

/** \brief Hsm instance
 *
 * Struct for an instance of state machine, placed in NOINIT RAM */
struct Dcm_HsmInstStruct
{
  /** \brief Id of the currently active state */
  Dcm_HsmStateIdxType   stateId;

  /** \brief Id of source state during a transition */
  Dcm_HsmStateIdxType   sourceId;

  /** \brief Number of events filled up in the queue */
  uint8                 evQueueFill;

  /** \brief Index into queue where events emitted to self have to be
   * inserted */
  uint8                 evQueueInsertEvNo;
};

/** \brief Type for state machine instance data */
typedef struct Dcm_HsmInstStruct Dcm_HsmInstType;

/** \brief Function pointer type for entry/exit/transition action functions */
/* Deviation MISRAC2012-1 <+2> */
typedef P2FUNC(void, DCM_CODE, Dcm_HsmActionType)(
  DCM_PDL_SF(const uint8 instIdx));

/** \brief Function pointer type for guard functions */
/* Deviation MISRAC2012-1 <+2> */
typedef P2FUNC(boolean, DCM_CODE, Dcm_HsmGuardType)(
  DCM_PDL_SF(const uint8 instIdx));

/* forward declaration */
struct Dcm_HsmTranStruct;
/** \brief Type for state transition description */
typedef struct Dcm_HsmTranStruct Dcm_HsmTranType;

/** \brief Structure describing a single state of a state chart
 *
 * Type describing states and their relation in the statechart. State
 * descriptions are stored in ROM */
typedef struct Dcm_HsmStateStruct
{
  /** \brief Mask of non-deferred and non-ignored events
   *
   * Events related to real actions */
  Dcm_HsmEventMaskType                         actionEventMask;
  /** \brief Mask of ignored events */
  Dcm_HsmEventMaskType                         ignoredEventMask;
  /** \brief Pointer to array of transitions from this state */
  P2CONST(Dcm_HsmTranType, TYPEDEF, DCM_CONST) trans;
  /** \brief Parent state ID */
  Dcm_HsmStateIdxType                          parentId;
  /** \brief Init substate, if initId == ::DCM_HSM_SIDX_INVALID there is no
   * substate */
  Dcm_HsmStateIdxType                          initId;
  /** \brief Function pointer of entry handler function */
  Dcm_HsmActionIdxType                         entryFnIdx;
  /** \brief Function pointer of exit handler function */
  Dcm_HsmActionIdxType                         exitFnIdx;
  /** \brief Number of transitions originating from this state */
  uint8                                        numTrans;
} Dcm_HsmStateType;

/** \brief Type for state description */

/** \brief State chart description data
 *
 * This struct is placed in ROM */
typedef struct Dcm_HsmStatechartStruct
{
  /** \brief Pointer to state description array */
  P2CONST(Dcm_HsmStateType, TYPEDEF, DCM_CONST)     states;

  /** \brief Pointer to array of all entry/exit/transition function
   * pointers */
  P2CONST(Dcm_HsmActionType, TYPEDEF, DCM_CONST)    actions;

  /** \brief Pointer to array of all guard function pointers */
  P2CONST(Dcm_HsmGuardType, TYPEDEF, DCM_CONST)     guards;

  /** \brief Pointer to array containing the Hsm instance working data in
   * RAM */
  P2VAR(Dcm_HsmInstType, TYPEDEF, DCM_VAR_CLEARED)   instances;

  /** \brief Pointer to event queue for this hsm instance
   *
   * Total array size equals to (numInstances * evQueueSize).
   *
   * Events for instance #n are located in array fields events[n*evQueueSize]
   * .. events[(n+1)*evQueueSize-1] */
  P2VAR(Dcm_HsmEventType, TYPEDEF, DCM_VAR_CLEARED)  events;

#if defined(TS_WINDOWS) && (TS_ARCH_FAMILY == TS_WINDOWS)
  /* name of the HSM (for debugging on Windows) */
  /* Deviation MISRAC2012-1 <+7> */
  /* Deviation MISRAC2012-2 <+6> */
  /* Deviation MISRA-1 <+5> */
  P2CONST(char, DCM_CONST, DCM_CONST)               hsmName;
  P2CONST(P2CONST(char, DCM_CONST, DCM_CONST), DCM_CONST, DCM_CONST) stateNames;
  P2CONST(P2CONST(char, DCM_CONST, DCM_CONST), DCM_CONST, DCM_CONST) actionNames;
  P2CONST(P2CONST(char, DCM_CONST, DCM_CONST), DCM_CONST, DCM_CONST) guardNames;
  P2CONST(P2CONST(char, DCM_CONST, DCM_CONST), DCM_CONST, DCM_CONST) eventNames;
#endif

  /** \brief Number of hsm instances */
  uint8                                             numInstances;
  /** \brief Number of event queue entries per instance */
  uint8                                             evQueueSize;
  /** \brief ID of the top state */
  Dcm_HsmStateIdxType                               topStateId;
    /** \brief Number of available events in the state chart */
  uint8                                             numEvents;

} Dcm_HsmStatechartType;


/** \brief Pointer type to state chart description */
typedef P2CONST(Dcm_HsmStatechartType, TYPEDEF, DCM_CONST)
  Dcm_HsmStatechartPtrType;


/** \brief State transition
 *
 * Struct type describing a transition in the statechart. Transition
 * descriptions are stored in ROM */
struct Dcm_HsmTranStruct
{
  /** \brief Pointer to array of transition step indices */
  P2CONST(Dcm_HsmActionIdxType, TYPEDEF, DCM_CONST) steps;

  /** \brief Number of transition steps */
  uint8                                             numSteps;

  /** \brief Triggering event for transition */
  Dcm_HsmEventType                                  event;

  /** \brief Index of transition guard function */
  Dcm_HsmGuardIdxType                               guardFnIdx;

  /** \brief ID of the target state */
  Dcm_HsmStateIdxType                               target;
};

/*==================[external function declarations]========================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/*------------------ init functions ----------------------------------------*/

#if (DCM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Init all hsm instances of single state chart
 **
 ** \param sc pointer to state chart which shall be initialized */
extern FUNC(void, DCM_CODE) Dcm_HsmInit(const Dcm_HsmStatechartPtrType sc);
#else
/** \brief Init hsm instance of single state chart
 **
 ** \param sc pointer to state chart which shall be initialized */
#if (defined Dcm_HsmInit)
  #error "Dcm_HsmInit is already defined"
#endif
#define Dcm_HsmInit(sc) DCM_HSMINITINST(sc,0)
#endif

/** \brief Init single instance of one hsm
 **
 ** \param sc pointer to state chart which shall be initialized
 ** \param instIdx index of target instance */
extern FUNC(void, DCM_CODE) DCM_HSMINITINST(
  const Dcm_HsmStatechartPtrType sc,
  const uint8                    instIdx);

/*------------------ emit functions ----------------------------------------*/

#if (DCM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Send an event to all instances of a state chart
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to all instances of the state machine. */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_HsmEmitToSelf(
  const Dcm_HsmStatechartPtrType sc,
  const Dcm_HsmEventType         event);
#else
/** \brief Send an event to a state chart from
 ** within the state machine functions
 ** \param sc Pointer to target state chart
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow
 **
 ** The event is sent to the only instance of the state machine. */
#if (defined Dcm_HsmEmitToSelf)
  #error "Dcm_HsmEmitToSelf is already defined"
#endif
#define Dcm_HsmEmitToSelf(sc,event) Dcm_HsmEmitToSelfInst((sc),(event))

#endif

/** \brief Send an event to a specific instance of the state machine
 ** \param sc target state chart
 ** \param instIdx index of target instance
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow */
extern FUNC(Std_ReturnType, DCM_CODE) DCM_HSMEMITINST(
  const Dcm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const Dcm_HsmEventType         event);

/** \brief Send an event to a specific instance of the state machine from
 ** within the state machine functions
 ** \param sc target state chart
 ** \param instIdx index of target instance
 ** \param ev event
 ** \return Success status of function
 ** \retval E_OK event was queued successfully
 ** \retval E_NOT_OK event queue overflow */
extern FUNC(Std_ReturnType, DCM_CODE) DCM_HSMEMITTOSELFINST(
  const Dcm_HsmStatechartPtrType sc,
  const uint8                    instIdx,
  const Dcm_HsmEventType         event);

/*------------------ main functions ----------------------------------------*/

#if (DCM_HSM_INST_MULTI_ENABLED == STD_ON)
/** \brief Process events for one state chart
 ** \param sc pointer to state charts
 **
 ** This function calls Dcm_HsmMainInst() for all instances of the state
 ** machine described in sc until all events are processed. */
extern FUNC(void, DCM_CODE) Dcm_HsmMain(const Dcm_HsmStatechartPtrType sc);
#else
/** \brief Process events for one state chart
 ** \param sc pointer to state charts
 **
 ** This function calls Dcm_HsmMainInst() for the only instance of the state
 ** machine described in sc. */
#if (defined Dcm_HsmMain)
  #error "Dcm_HsmMain is already defined"
#endif
#define Dcm_HsmMain(sc) ((void)DCM_HSMMAININST(sc,0))
#endif

/** \brief Process events of a state chart in one state machine instance
 ** \param sc state chart
 ** \param instIdx index of target instance
 ** \return Event processing status
 ** \retval TRUE at least one event was processed which may have lead to an
 ** action or a state change
 ** \retval FALSE No action or state change performed due to event
 ** processing */
extern FUNC(boolean, DCM_CODE) DCM_HSMMAININST(
    const Dcm_HsmStatechartPtrType sc,
    const uint8                    instIdx);

/*------------------ helper functions --------------------------------------*/

#if (DCM_HSM_TRACE == STD_ON)
/** \brief Switches tracing on or off
 ** \param newValue new tracing, TRUE for on and FALSE for off */
extern FUNC(void, DCM_CODE) Dcm_HsmSetTracing(const boolean newValue);
#endif

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif
/*==================[end of file]===========================================*/
