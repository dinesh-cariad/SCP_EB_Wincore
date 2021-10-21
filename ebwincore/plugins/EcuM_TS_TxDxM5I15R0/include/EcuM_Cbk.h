#ifndef ECUM_CBK_H
#define ECUM_CBK_H

/**
 * \file
 *
 * \brief AUTOSAR EcuM
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuM.
 *
 * \version 5.15.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO EcuM2994,1 */

/* This header defines the callout and callback interface of the EcuM. */

/*==================[inclusions]============================================*/

#include <EcuM_Version.h>       /* static module version information */
#include <EcuM_Types.h>

/*==================[macros]================================================*/

/** \brief Error reason value for EcuM_ErrorHook()
 *
 * The RAM check routine returned a failure */
#define ECUM_RAM_CHECK_FAILED   0x02U
/* According to the AUTOSAR "Specification of Standard Types" only 0x00, 0x01
 * are reserved error codes; range of 0x02-0x3f can be used */

/*==================[type definitions]======================================*/

/** \brief Return type of callout function EcuM_DeterminePbConfiguration()
 *
 * This type definition is used to prevent problematic nested compiler
 * abstraction macros as in FUNC(P2CONST(), ..) in the declaration/definition
 * of EcuM_DeterminePbConfiguration(). */
typedef P2CONST(EcuM_ConfigType, AUTOMATIC, ECUM_CONST)
  EcuM_DeterminePbConfigurationRetType;

/*==================[external function declarations]========================*/

#define ECUM_START_SEC_CODE
#include <EcuM_MemMap.h>

/*--------------------------------------------------------------------------
 * Callback functions
 *--------------------------------------------------------------------------*/

/*------------------[callbacks from wakeup sources]-------------------------*/

/** \brief Set wakeup events.
 **
 ** This function sets the wakeup events for a set of sources and starts the
 ** wakeup validation timeout timer.
 **
 ** Development Error Detection
 ** - ::ECUM_E_UNKNOWN_WAKEUP_SOURCE: \a sources contained unknown wakeup
 **   sources.
 **
 ** \param[in] sources Events of wakeup sources to be set.
 **
 ** \ServiceID{::ECUM_SID_SET_WK_EV}
 ** \Reentrancy{Non-Reentrant Non-Interruptible}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_CODE) EcuM_SetWakeupEvent
(
    EcuM_WakeupSourceType sources
);

/** \brief Validate wakeup events.
 **
 ** This function validates wakeup events. After wakeup, the ECU State Manager
 ** will stop the process during the WAKEUP VALIDATION state to wait for
 ** validation of the wakeup event. The validation is carried out with a call
 ** to this API service.
 **
 ** Development Error Detection
 ** - ::ECUM_E_UNKNOWN_WAKEUP_SOURCE: \a sources contained unknown wakeup
 **   sources.
 **
 ** \param[in] sources Wakeup sources which events shall be validated.
 **
 ** \ServiceID{::ECUM_SID_VALIDATE_WK_EV}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_CODE) EcuM_ValidateWakeupEvent
(
    EcuM_WakeupSourceType sources
);

/*--------------------------------------------------------------------------
 * Callout functions
 *--------------------------------------------------------------------------*/

/*------------------[Generic callouts]--------------------------------------*/

/** \brief Callout on errors.
 **
 ** This function is used in unrecoverable error situations.  The ECU State
 ** Manager will call the error hook.
 **
 ** It is up to the system integrator to react accordingly (reset, halt,
 ** restart, safe state etc. ).
 **
 ** \param[in] reason Reason for calling the error hook.  Currently the EcuM
 ** only reports ::ECUM_RAM_CHECK_FAILED with the error hook function.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_ErrorHook
(
  Std_ReturnType reason
);

/*------------------[Callouts from STARTUP state]---------------------------*/

/** \brief Callout from the startup phase.
 **
 ** This callout shall set the interrupts on ECUs with programmable interrupts.
 **
 ** This callout is invoked by EcuM before Mcu is started.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_APPL_CODE) EcuM_AL_SetProgrammableInterrupts(void);

#if (ECUM_INITLIST_ZERO == STD_ON)
/** \brief Callout in startup sequence zero.
 **
 ** This callout provides driver initialization and other hardware-related
 ** startup activities in case of a power on reset.
 **
 ** This callout is invoked by EcuM before Mcu is started.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_AL_DriverInitZero(void);

/** \brief Default implementation of the EcuMDriverInitListZero
 **
 ** This function provides the default implementation of the
 ** EcuMDriverInitListZero.  When this function is called, the initialization of
 ** the configured EcuMDriverInitItems within the EcuM configuration container
 ** EcuMDriverInitListZero will be carried out.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_CODE) EcuM_DefaultInitListZero(void);
#endif

/** \brief Callout which determines post-build configuration.
 **
 ** In this callout the system integrator has to return the actual post-build
 ** configuration for the EcuM.
 **
 ** This callout is invoked after EcuM_AL_DriverInitZero()
 **
 ** \return pointer to post-build configuration
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(EcuM_DeterminePbConfigurationRetType, ECUM_APPL_CODE)
  EcuM_DeterminePbConfiguration(void);

#if (ECUM_INITLIST_ONE == STD_ON)
/** \brief Callout in startup sequence one.
 **
 ** This callout provides driver initialization and other hardware-related
 ** startup activities in case of a power on reset.
 **
 ** This callout is invoked by EcuM before Gpt and OS is started.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_AL_DriverInitOne
(
  P2CONST(EcuM_ConfigType, AUTOMATIC, ECUM_CONST) ConfigPtr
);

/** \brief Default implementation of the EcuMDriverInitListOne
 **
 ** This function provides the default implementation of the
 ** EcuMDriverInitListOne.  When this function is called, the initialization of
 ** the configured EcuMDriverInitItems within the EcuM configuration container
 ** EcuMDriverInitListOne will be carried out.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_CODE) EcuM_DefaultInitListOne(void);
#endif

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
/** \brief Callout on EnterRun
 **
 ** This call allows the system designer to notify that the RUN state is
 ** about to be entered.
 **
 ** This callout is invoked by EcuM on entry of RUN state.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_OnEnterRun(void);

/** \brief Callout on ExitRun
 **
 ** This call allows the system designer to notify that the RUN state is
 ** about to be left.
 **
 ** This callout is invoked by EcuM before leaving RUN state.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_OnExitRun(void);

/** \brief Callout on ExitPostRun
 **
 ** This call allows the system designer to notify that the POST_RUN state is
 ** about to be left.
 **
 ** This callout is invoked by EcuM before leaving POST_RUN state.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_OnExitPostRun(void);

/** \brief Callout on RTEStartup
 **
 ** This call allows the system designer to notify that the RTE is about to be
 ** started.
 **
 ** This callout is invoked by EcuM before starting the RTE.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_OnRTEStartup(void);
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */
/*------------------[Callouts from RUN state]-------------------------------*/

/*------------------[Callouts from SHUTDOWN state]--------------------------*/

/** \brief Callout on GO OFF 1.
 **
 ** This call allows the system designer to notify that the GO OFF I state is
 ** about to be entered.
 **
 ** This callout is invoked by EcuM on entry of GO OFF 1 state.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_OnGoOffOne(void);

/** \brief Callout on GO OFF 2.
 **
 ** This call allows the system designer to notify that the GO OFF II state is
 ** about to be entered.
 **
 ** This callout is invoked by EcuM on entry of GO OFF 2 state.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_OnGoOffTwo(void);
#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
/** \brief Callout on PrepShutdown
 **
 ** This call allows the system designer to notify that the PREP SHUTDOWN state is
 ** about to be entered.
 **
 ** This callout is invoked by EcuM on entry of PREP SHUTDOWN state.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_OnPrepShutdown(void);
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

/** \brief Callout which enables wakeup sources.
 **
 ** In this callout the system integrator enables the wakeup functionality of
 ** a wakeup source, which shall be activated in the current sleep mode.
 **
 ** This callout is invoked before EcuM enter in Sleep.
 **
 ** \param wakeupSource All wakeup sources, activated in the current sleep
 ** mode. */
extern FUNC(void,ECUM_APPL_CODE) EcuM_EnableWakeupSources
(
  EcuM_WakeupSourceType wakeupSource
);

/** \brief Callout for writing a RAM hash.
 **
 ** This callout is invoked by EcuM just before putting the ECU physically
 ** to sleep.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_APPL_CODE) EcuM_GenerateRamHash(void);

/** \brief Callout for switching off power.
 **
 ** This callout shuts off the power supply of the ECU.  If the ECU cannot
 ** unpower itself, a reset may be an adequate reaction.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_APPL_CODE) EcuM_AL_SwitchOff(void);

/** \brief Callout for resetting the ECU.
 **
 ** This callout shall take the code for resetting the ECU.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_APPL_CODE) EcuM_AL_Reset( EcuM_ResetType reset );

/*------------------[Callouts from WAKEUP state]----------------------------*/

/** \brief Callout for checking RAM integrity .
 **
 ** This callout provides a RAM integrity test to ensure that RAM contents is
 ** still consistent after a long SLEEP duration.
 **
 ** This callout is invoked by EcuM early in WAKEUP 1 state.
 **
 ** \return Result of Check
 ** \retval 0 RAM integrity test failed.
 ** \retval else RAM integrity test passed.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(uint8,ECUM_APPL_CODE) EcuM_CheckRamHash(void);

/** \brief Callout which disables pending wakeup sources.
 **
 ** In this callout the system integrator can disable the functionality of
 ** pending wakeup source to set a wakeup.
 **
 ** This callout is invoked after a wakeup event is detected.
 **
 ** \param wakeupSource All pending wakeup sources.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_DisableWakeupSources
(
  EcuM_WakeupSourceType wakeupSource
);

#if (ECUM_INITLIST_RESTART == STD_ON)
/** \brief Callout for restarting drivers.
 **
 ** This callout provides driver initialization and other hardware-related
 ** startup activities in the wakeup case.
 **
 ** This callout is invoked by EcuM early in WAKEUP 1 state.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_AL_DriverRestart
(
  P2CONST(EcuM_ConfigType, AUTOMATIC, ECUM_CONST) ConfigPtr
);

/** \brief Default implementation of the EcuMDriverRestartList
 **
 ** This function provides the default implementation of the
 ** EcuMDriverRestartList.  When this function is called, the initialization of
 ** the configured EcuMDriverInitItems defined within the EcuM configuration
 ** container EcuMDriverRestartList will be carried out.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_CODE) EcuM_DefaultRestartList(void);
#endif

/** \brief Callout which starts pending wakeup sources.
 **
 ** In this callout the system integrator has to set up the conditions, so
 ** that a wakeup source can validate a wakeup event. For example, set the
 ** tranceiver mode of a Can transceiver to STARTED.
 **
 ** This callout is invoked in wakeup state.
 **
 ** \param[in] wakeupSource All pending wakeup sources.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_StartWakeupSources
(
  EcuM_WakeupSourceType wakeupSource
);

/** \brief Callout which checks pending wakeup sources.
 **
 ** This callout is invoked in wakeup validation state.  In this callout the
 ** system integrator has to check the wakeup sources for validation of a
 ** pending wakeup event.
 **
 ** \param[in] wakeupSource All pending wakeup sources.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_CheckValidation
(
  EcuM_WakeupSourceType wakeupSource
);

/** \brief Callout which stops pending and expired wakeup sources.
 **
 ** In this callout the system integrator has to stop the wakeup sources,
 ** previously started in callout function EcuM_StartWakeupSources().
 **
 ** This callout is invoked in wakeup validation state after a failed
 ** validation of an wakeup event.
 **
 ** \param[in] wakeupSource All pending/expired wakeup sources.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_StopWakeupSources
(
  EcuM_WakeupSourceType wakeupSource
);

/*------------------[Callouts from SLEEP state]-----------------------------*/

/** \brief Callout during sleep state.
 **
 ** It is explicitly allowed to poll wakeup sources from this callout and to
 ** call wakeup notification functions to indicate the end of the sleep state
 ** to the ECU State Manager.
 **
 ** This callout is invoked by EcuM periodically in SLEEP state, but only if
 ** the CPU is not suspended (i.e. clock is reduced).
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_SleepActivity(void);

/** \brief Callout which checks wakeup sources.
 **
 ** In this callout the system integrator checks the wakeup sources for
 ** detected wakeup events. These wakeup events are reported using
 ** EcuM_SetWakeupEvent().
 **
 ** This callout is invoked during sleep state or is invoked by an ISR
 ** resulting from an wakeup event, in case the CPU is powered of during
 ** sleep.
 **
 ** \param wakeupSource All wakeup sources, activated in the current sleep
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_CheckWakeup
(
  EcuM_WakeupSourceType wakeupSource
);

/** \brief Callout during sleep state.
 **
 ** In this callout all the execution of interrupts leading to wakeup
 ** will be disabled. This will be called before setting up the ECU
 ** in to Halt mode. This prevents the ECU from neglecting the pending
 ** wakeup events, which occurred due to the execution of ISR before the call
 ** to Mcu_SetMode()
 **
 ** This callout is invoked by EcuM periodically in SLEEP state, but only if
 ** the CPU is not suspended (i.e. clock is reduced).
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_APPL_CODE) EcuM_PreHalt(void);

/** \brief Callout during sleep state.
 **
 ** In this callout all the interrupts will be enabled which
 ** are disabled by the EcuM_PreHalt(). This will be called
 ** after a wakeup event that will return the function Mcu_SetMode()
 **
 ** This callout is invoked by EcuM periodically in SLEEP state, but only if
 ** the CPU is not suspended (i.e. clock is reduced).
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, ECUM_APPL_CODE) EcuM_PostHalt(void);

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
/** \brief Callout on GoSleep
 **
 ** This call allows the system designer to notify that the SLEEP state is
 ** about to be entered.
 **
 ** This callout is invoked by EcuM on entry of SLEEP state.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void,ECUM_APPL_CODE) EcuM_OnGoSleep(void);
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

/*-------------[Callouts for EcuM Multi-Core synchronization]---------------*/

#if (ECUM_MULTICORE_ENABLED == TRUE)
/** \brief Determines if a core shall continue waiting to synchronize
 **
 ** This function is repeatedly called while the EcuM on a core is blocked waiting
 ** for another core to synchronize. This callout function shall service a
 ** watchdog timer when one exists.
 **
 ** \return ECUM_ABORT_SYNC when the active EcuM instance shall abort synchronization.
 ** with the other cores, otherwise ECUM_CONTINUE_SYNC.
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(EcuM_SyncActionType,ECUM_APPL_CODE) EcuM_OnCoreSync(void);
#endif /* ECUM_MULTICORE_ENABLED == TRUE */

#define ECUM_STOP_SEC_CODE
#include <EcuM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef ECUM_CBK_H */

