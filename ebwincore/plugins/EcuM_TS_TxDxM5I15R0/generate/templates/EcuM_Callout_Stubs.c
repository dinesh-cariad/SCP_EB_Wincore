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
[!AUTOSPACING!]

[!VAR "MULTICORE_ENABLED" = "node:exists(as:modconf('Os')[1]/OsOS/OsNumberOfCores) and node:value(as:modconf('Os')/OsOS/OsNumberOfCores) > 1 and node:value(as:modconf('EcuM')/EcuMGeneral/EcuMEnableMulticore) = 'true'"!]
[!IF "$MULTICORE_ENABLED"!]
[!VAR "MULTICORE_NUM_CORES" = "node:value(as:modconf('Os')[1]/OsOS/OsNumberOfCores)"!]
[!ELSE!][!//
[!VAR "MULTICORE_NUM_CORES" = "1"!]
[!ENDIF!]

[!IF "node:exists(as:modconf('EcuM')/EcuMGeneral/EcuMMasterCoreId)"!][!//
[!VAR "ECUM_MASTER_CORE_ID" = "node:value(as:modconf('EcuM')/EcuMGeneral/EcuMMasterCoreId)"!][!//
[!ELSE!][!//
[!VAR "ECUM_MASTER_CORE_ID" = "'0'"!][!//
[!ENDIF!][!//

/*==================[inclusions]============================================*/
/* !LINKSTO EcuM2990,1 */
#include <TSAutosar.h>           /* EB specific standard types */

#include <EcuM.h>
#include <EcuM_Trace.h>
#include <EcuM_PBcfg.h>

/*==================[macros]================================================*/

#if(defined ECUM_GET_CORE_ID)
#error ECUM_GET_CORE_ID already defined
#endif
#if (ECUM_MULTICORE_ENABLED == TRUE)
/** \brief Returns the core ID that is currently active. */
#define ECUM_GET_CORE_ID() GetCoreID()
#else
#define ECUM_GET_CORE_ID() 0U
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */

#ifndef OS_CORE_ID_MASTER
#define OS_CORE_ID_MASTER 0U
typedef uint8 EcuMCoreIdType;
#else
typedef CoreIdType EcuMCoreIdType;
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define ECUM_START_SEC_CODE
#include <EcuM_MemMap.h>

/*------------------[Callouts from STARTUP state]---------------------------*/
[!SELECT "EcuMConfiguration/*[1]/EcuMCommonConfiguration"!][!//

[!IF "node:exists(EcuMDriverInitListZero)"!][!//
FUNC(void, ECUM_APPL_CODE) EcuM_AL_DriverInitZero(void)
{
 /* EcuM_DefaultInitListZero() provides the default implementation of the
  * EcuMDriverInitListZero.  When this function is called, the initialization of
  * the configured EcuMDriverInitItems within the EcuM configuration container
  * EcuMDriverInitListZero will be carried out.
  */
  EcuM_DefaultInitListZero();
}
[!ELSE!][!//
/* No content for EcuM_AL_DriverInitZero() configured */
[!ENDIF!][!//

[!IF "node:exists(EcuMDriverInitListOne)"!][!//
/* Configured Initialization List One items */
FUNC(void, ECUM_APPL_CODE) EcuM_DriverInitListOne
(
  P2CONST(EcuM_ConfigType, AUTOMATIC, ECUM_CONST) ConfigPtr
)
{
  TS_PARAM_UNUSED(ConfigPtr);
  /* EcuM_DefaultInitListOne() provides the default implementation of the
   * EcuMDriverInitListOne.  When this function is called, the initialization of
   * the configured EcuMDriverInitItems within the EcuM configuration container
   * EcuMDriverInitListOne will be carried out.
   */
  EcuM_DefaultInitListOne();
}

FUNC(void, ECUM_APPL_CODE) EcuM_AL_DriverInitOne
(
  P2CONST(EcuM_ConfigType, AUTOMATIC, ECUM_CONST) ConfigPtr
)
{
  /* Call Initialization List One. This initializes all the configured
   * modules in 'EcuMDriverInitListOne'
   */
  EcuM_DriverInitListOne(ConfigPtr);
}
[!ENDIF!][!//

/*------------------[Callouts from SLEEP state]--------------------------*/
[!IF "node:exists(EcuMDriverRestartList)"!][!//
/* Configured Driver Restart List items */
FUNC(void, ECUM_CODE) EcuM_DriverRestartList
(
  P2CONST(EcuM_ConfigType, AUTOMATIC, ECUM_CONST) ConfigPtr
)
{
  TS_PARAM_UNUSED(ConfigPtr);
  /* EcuM_DefaultRestartList() provides the default implementation of the
   * EcuMDriverRestartList.  When this function is called, the initialization of
   * the configured EcuMDriverInitItems defined within the EcuM configuration
   * container EcuMDriverRestartList will be carried out.
   */
  EcuM_DefaultRestartList();
}

FUNC(void, ECUM_APPL_CODE) EcuM_AL_DriverRestart
(
  P2CONST(EcuM_ConfigType, AUTOMATIC, ECUM_CONST) ConfigPtr
)
{
  /* Call Driver Restart List. This restart all the configured
   * modules in 'EcuMDriverRestartList'
   */
  EcuM_DriverRestartList(ConfigPtr);
}
[!ENDIF!][!//
[!ENDSELECT!][!//

/*------------------[Generic callouts]--------------------------------------*/

FUNC(void,ECUM_APPL_CODE) EcuM_ErrorHook
(
  Std_ReturnType reason
)
{
  TS_PARAM_UNUSED(reason); /* remove if parameter is used */

  /* Implement the routine to handle the production error ECUM_RAM_CHECK_FAILED
   * with in this callout function.
   *
   * EcuM calls EcuM_ErrorHook with parameter ECUM_RAM_CHECK_FAILED when RAM Check
   * fails on wakeup.
   *
   * Integrator may define additional error codes to be passed as the 'reason'
   * parameter. This code shall not conflict with the development and production error
   * codes defined in SWS. Also it shall not conflict with standard error codes E_OK and
   * E_NOT_OK.
   * Note: The ECU Manager module can invoke EcuM_ErrorHook in all phases
   *
   *  Implementation hint:
   *
   *  FUNC(void,ECUM_APPL_CODE) EcuM_ErrorHook (Std_ReturnType reason)
   *  {
   *    if(reason == ECUM_RAM_CHECK_FAILED)
   *    {
   *      Mcu_PerformReset();
   *    }
   *  }
   */
}

/*------------------[Callouts from STARTUP phase]---------------------------*/

FUNC(void,ECUM_APPL_CODE) EcuM_AL_SetProgrammableInterrupts(void)
{
  DBG_ECUM_AL_SETPROGRAMMABLEINTERRUPTS_ENTRY();
  /* Implement the routine to set interrupts on ECUs with programmable interrupts
   * with in this callout function.
   *
   * On ECUs with programmable interrupt priorities, these priorities must be set before
   * the OS is started. */

  DBG_ECUM_AL_SETPROGRAMMABLEINTERRUPTS_EXIT();
}

FUNC(EcuM_DeterminePbConfigurationRetType, ECUM_APPL_CODE)
  EcuM_DeterminePbConfiguration(void)
{
  EcuM_DeterminePbConfigurationRetType PbConfig = &([!"name(EcuMConfiguration/*[1])"!]);

  DBG_ECUM_DETERMINEPBCONFIGURATION_ENTRY();

  /* Implement the routine to return EcuM configuration structure
   * which shall be activated on start up in this callout function.
   *
   * Integrator may select the post-build configuration after evaluating
   * some condition like status of a port pin or NVRAM value. */

  DBG_ECUM_DETERMINEPBCONFIGURATION_EXIT(PbConfig);
  return PbConfig;
}

/*------------------[Callouts from SHUTDOWN phase]--------------------------*/

FUNC(void, ECUM_APPL_CODE) EcuM_OnGoOffOne(void)
{
  DBG_ECUM_ONGOOFFONE_ENTRY();

  /* Ecu Manager module invokes this callout on entry actions of OffPreOS
   * sequence. */

  DBG_ECUM_ONGOOFFONE_EXIT();
}

FUNC(void, ECUM_APPL_CODE) EcuM_OnGoOffTwo(void)
{
  DBG_ECUM_ONGOOFFTWO_ENTRY();

  /* Ecu Manager module invokes this callout on entry actions of OffPostOS
   * sequence. */

  DBG_ECUM_ONGOOFFTWO_EXIT();
}

FUNC(void,ECUM_APPL_CODE) EcuM_EnableWakeupSources
(
  EcuM_WakeupSourceType wakeupSource
)
{
  DBG_ECUM_ENABLEWAKEUPSOURCES_ENTRY(wakeupSource);

  TS_PARAM_UNUSED(wakeupSource); /* remove if parameter is used */

  /* Implement the routine to notify the wakeup sources defined in the wakeupSource
   * bitfield that SLEEP will be entered and enable the wakeup functionality accordingly
   * in this callout function.
   *
   *  Implementation hint:
   *
   *  Enable GPT Wakeup:
   *
   *  FUNC(void,ECUM_APPL_CODE) EcuM_EnableWakeupSources (EcuM_WakeupSourceType wakeupSource)
   *  {
   *    if (wakeupSource is assigned to GPT channel)
   *    {
   *      Gpt_EnableWakeup(Gpt_ChannelType);
   *
   *      Gpt_StartTimer(Gpt_ChannelType, Gpt_ValueType);
   *
   *      Gpt_SetMode(Gpt_ModeType);
   *    }
   *  }
   *  Enable CAN Wakeup:
   *
   *  void EcuM_EnableWakeupSources (EcuM_WakeupSourceType wakeupSource)
   *  {
   *    if (wakeupSource is assigned to CAN channel)
   *    {
   *      CanIf_SetTrcvWakeupMode(TransceiverId, TrcvWakeupMode);
   *    }
   *    else if (wakeupSource is assigned to ICU channel)
   *    {
   *      Icu_EnableWakeup(Icu_ChannelType);
   *    }
   *  }
   */

  DBG_ECUM_ENABLEWAKEUPSOURCES_EXIT(wakeupSource);
}

FUNC(void,ECUM_APPL_CODE) EcuM_GenerateRamHash(void)
{
  DBG_ECUM_GENERATERAMHASH_ENTRY();

  /* Implement the routine to check RAM memories integrity with in this callout
   * function.
   *
   * The ECU Manager module invokes EcuM_GenerateRamHash callout
   * before halting the micro controller and check the RAM integrity using
   * the callout EcuM_CheckRamHash after the processor returns from halt.
   *
   * Ram memory may become corrupted when an ECU is held in sleep mode for a
   * long time. The RAM memories integrity should therefore be checked to prevent
   * unforeseen behavior. The system designer may choose an adequate checksum
   * algorithm to perform the check. */

  DBG_ECUM_GENERATERAMHASH_EXIT();
}

FUNC(void, ECUM_APPL_CODE) EcuM_AL_SwitchOff(void)
{
  DBG_ECUM_AL_SWITCHOFF_ENTRY();

  /* The ECU Manager module invokes EcuM_AL_SwitchOff as the last activity
   * in the OffPostOS Sequence.
   *
   * In some cases of HW/SW concurrency, it may happen that during the power
   * down in EcuM_AL_SwitchOff(endless loop) some hardware(eg. CAN transciver)
   * switches on the ECU again.
   *
   * In this case the Ecu may be in deadlock until the hardware watchdog resets
   * the ECU. To reduce the time until the hardware  watchdog fixes this deadlock,
   * the integrator code in EcuM_AL_SwitchOff as last action can limit the endless
   * loop and after a sufficient long time reset the Ecu using Mcu_PerformReset(). */

  DBG_ECUM_AL_SWITCHOFF_EXIT();
}

FUNC(void, ECUM_APPL_CODE) EcuM_AL_Reset(EcuM_ResetType reset)
{
  DBG_ECUM_AL_RESET_ENTRY(reset);

  TS_PARAM_UNUSED(reset); /* remove if parameter is used */

  /* Implement the routine to reset the ECU with in this callout function.
   *
   * When the shutdown target is RESET, the ECU Manager module invokes
   * this callout. This callout shall take the code for resetting the ECU.
   * The parameter 'reset' tells which type of reset to be done in Ecu.
   *
   * EcuMConf_EcuMResetMode_ECUM_RESET_MCU - Perform Microcontroller reset via Mcu_PerformReset
   * EcuMConf_EcuMResetMode_ECUM_RESET_WDG - Perform Watchdog reset via WdgM_PerformReset
   * EcuMConf_EcuMResetMode_ECUM_RESET_IO  - Perform Reset by toggeling an I/O line
   *
   * This values can be extended by configuration and the integrator shall decide
   * the type of reset to be performed with respect to the 'reset' received.
   *
   *  Implementation hint:
   *  FUNC(void, ECUM_APPL_CODE) EcuM_AL_Reset(EcuM_ResetType reset)
   *  {
   *    if(reset == EcuMConf_EcuMResetMode_ECUM_RESET_MCU )
   *    {
   *      Mcu_PerformReset();
   *    }
   *    else if(reset == EcuMConf_EcuMResetMode_ECUM_RESET_WDG )
   *    {
   *      WdgM_PerformReset();
   *    }
   *    if(reset == EcuMConf_EcuMResetMode_ECUM_RESET_IO )
   *    {
   *
   *    }
   *  }
   *
   */

  DBG_ECUM_AL_RESET_EXIT(reset);
}

/*------------------[Callouts from WAKEUP state]----------------------------*/

FUNC(uint8, ECUM_APPL_CODE) EcuM_CheckRamHash(void)
{
  uint8 RAMTest = 1U;

  DBG_ECUM_CHECKRAMHASH_ENTRY();

  /* Implement the routine to check RAM memories integrity with in this callout
   * function.
   *
   * The goal of this test is to ensure that after a long SLEEP duration, RAM
   * contents is still consistent. The check does not need to be exhaustive since
   * this would consume quite some processing time during wakeups. A well
   * designed check will execute quickly and detect RAM integrity defects
   * with a sufficient probability.
   */

  /* 0: RAM integrity test failed
     else: RAM integrity test passed */

  DBG_ECUM_CHECKRAMHASH_EXIT(RAMTest);
  return RAMTest;
}

FUNC(void,ECUM_APPL_CODE) EcuM_DisableWakeupSources
(
  EcuM_WakeupSourceType wakeupSource
)
{
  DBG_ECUM_DISABLEWAKEUPSOURCES_ENTRY(wakeupSource);

  TS_PARAM_UNUSED(wakeupSource); /* remove if parameter is used */

  /* Implement the routine to disable all wakeup sources in this callout function.
   *
   * The ECU Manager Module calls EcuM_DisableWakeupSources to set the wakeup
   * source(s) defined in the wakeupSource bitfield so that they are not able
   * to wake the ECU up and will be invoked in the WakeupRestart Sequence.
   *
   * The ECU Manager module derives the wakeup sources to be checked
   * (and used as the wakeupSource parameter) from the internal pending events
   * variable (NOT operation). The integration code used for this callout must
   * determine which wakeup sources must be checked.
   *
   *  Implementation hint:
   *
   *  Disable GPT Wakeup:
   *  FUNC(void,ECUM_APPL_CODE) EcuM_DisableWakeupSources ( EcuM_WakeupSourceType wakeupSource)
   *  {
   *    if (wakeupSource is assigned to GPT channel)
   *    {
   *      Gpt_DisableWakeup(Gpt_ChannelType);
   *
   *      Gpt_SetMode(Gpt_ModeType);
   *    }
   *  }
   */

  DBG_ECUM_DISABLEWAKEUPSOURCES_EXIT(wakeupSource);
}

/*------------------[Callouts from UP phase]----------------------------*/

FUNC(void,ECUM_APPL_CODE) EcuM_StartWakeupSources
(
  EcuM_WakeupSourceType wakeupSource
)
{
  DBG_ECUM_STARTWAKEUPSOURCES_ENTRY(wakeupSource);

  TS_PARAM_UNUSED(wakeupSource); /* remove if parameter is used */

  /* Implement the routine to start the given wakeup source(s) with in this callout function,
   * so that they are ready to perform wakeup validation
   *
   * Implementation hint:
   *
   * Can start wakeup :
   * FUNC(void,ECUM_APPL_CODE) EcuM_StartWakeupSources ( EcuM_WakeupSourceType wakeupSource )
   *  {
   *    if (wakeupSource is assigned to CAN channel)
   *    {
   *      CanIf_SetTrcvMode(uint8 TransceiverId, CanIf_TrcvModeType TransceiverMode);
   *
   *      CanIf_SetControllerMode( uint8 ControllerId, CanIf_ControllerModeType ControllerMode);
   *    }
   *  }
   */

  DBG_ECUM_STARTWAKEUPSOURCES_EXIT(wakeupSource);
}

FUNC(void,ECUM_APPL_CODE) EcuM_CheckValidation
(
  EcuM_WakeupSourceType wakeupSource
)
{
  DBG_ECUM_CHECKVALIDATION_ENTRY(wakeupSource);

  TS_PARAM_UNUSED(wakeupSource); /* remove if parameter is used */

  /* Implement the routine to validate a wakeup source with in this callout function.
   *
   * If a valid wakeup has been detected, it shall be reported to EcuM
   * via EcuM_ValidateWakeupEvent().
   * This callout will be invoked by EcuM in the Wakeup Validation Sequence.
   *
   * Implementation hint :
   *
   * Can Wakeup Validation :
   *  FUNC(void,ECUM_APPL_CODE) EcuM_CheckValidation(EcuM_WakeupSourceType wakeupSource)
   *    {
   *      if (wakeupSource is assigned to CAN channel)
   *      {
   *        CanIf_CheckValidation(WakeupSource);
   *      }
   *    }
   */

  DBG_ECUM_CHECKVALIDATION_EXIT(wakeupSource);
}

FUNC(void,ECUM_APPL_CODE) EcuM_StopWakeupSources
(
  EcuM_WakeupSourceType wakeupSource
)
{
  DBG_ECUM_STOPWAKEUPSOURCES_ENTRY(wakeupSource);

  TS_PARAM_UNUSED(wakeupSource); /* remove if parameter is used */

  /* Implement the routine to stop the given wakeup sources after
   * unsuccessful wakeup validation, with in this callout function.
   *
   * This callout will be invoked by EcuM in Wakeup Validation sequence.
   *
   * Implementation hint:
   *
   * Stop CanIf wakeup:
   * FUNC(void,ECUM_APPL_CODE) EcuM_StopWakeupSources (EcuM_WakeupSourceType wakeupSource)
   * {
   *   Std_ReturnType CanIf_SetControllerMode
   *                   ( uint8 ControllerId, CanIf_ControllerModeType ControllerMode );
   * }
   */

  DBG_ECUM_STOPWAKEUPSOURCES_EXIT(wakeupSource);
}

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
FUNC(void,ECUM_APPL_CODE) EcuM_OnEnterRun(void)
{
  /* Ecu Manager module invokes this callout on entry to the RUN state. */
}

FUNC(void,ECUM_APPL_CODE) EcuM_OnExitRun(void)
{

  /* Ecu Manager module invokes this callout on exit of the RUN state. */
}

FUNC(void,ECUM_APPL_CODE) EcuM_OnExitPostRun(void)
{
  /* Ecu Manager module invokes this callout on exit of the POSTRUN state. */
}
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

/*------------------[Callouts from SLEEP phase]-----------------------------*/

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
FUNC(void,ECUM_APPL_CODE) EcuM_OnGoSleep(void)
{
  /* This callout is invoked by EcuM on entry of SLEEP state. */
}
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

FUNC(void,ECUM_APPL_CODE) EcuM_SleepActivity(void)
{
  DBG_ECUM_SLEEPACTIVITY_ENTRY();

  /* EcuM invokes this callout function in Poll sequence in a blocking loop at
   * maximum frequency. The callout implementation must ensure by other means, if
   * callout code shall be executed with a lower period. The integrator may choose
   * any method to control this, e.g. with the help of OS counters, OS alarms
   * or Gpt timers. */

  DBG_ECUM_SLEEPACTIVITY_EXIT();
}

FUNC(void,ECUM_APPL_CODE) EcuM_CheckWakeup
(
  EcuM_WakeupSourceType wakeupSource
)
{
  DBG_ECUM_CHECKWAKEUP_ENTRY(wakeupSource);

  TS_PARAM_UNUSED(wakeupSource); /* remove if parameter is used */

  /* Implement the routine to poll for a wakeup source in this callout function.
   *
   * It may also be called by the ISR of a wakeup source to set up the PLL and set
   * other wakeup sources that may be connected to the same interrupt.
   *
   * The Ecu Manager module invokes EcuM_CheckWakeup() periodically during the Poll
   * sequence(See section 7.5.3 Activities in the Poll Sequence) if the MCU is not halted
   * or when handling a wakeup interrupt.
   * If called from the Poll sequence, EcuM calls this callout functions in a blocking
   * loop at maximum frequency. The implementation must ensure by other means if callout
   * code shall be executed with a lower period. Choose any method to control the
   * execution period. eg. with the help of OS counters, OS alarms or GPT timers.
   *
   *  Implementation hint:
   *
   *  FUNC(void,ECUM_APPL_CODE) EcuM_CheckWakeup (EcuM_WakeupSourceType wakeupSource)
   *  {
   *    <Module>_CheckWakeup(wakeupSource);
   *  }
   */

  DBG_ECUM_CHECKWAKEUP_EXIT(wakeupSource);
}

FUNC(void, ECUM_APPL_CODE) EcuM_PreHalt(void)
{
  DBG_ECUM_PREHALT_ENTRY();

  /* Implement the routine to disable all the interrupts which lead to ECU wakeup. */

  DBG_ECUM_PREHALT_EXIT();
}

FUNC(void, ECUM_APPL_CODE) EcuM_PostHalt(void)
{
  DBG_ECUM_POSTHALT_ENTRY();

  /* Implement the routine to enable all the interrupts, that were disabled by
   * EcuM_PreHalt. */

  DBG_ECUM_POSTHALT_EXIT();
}

/*-------------[Callouts for EcuM Multi-Core synchronization]---------------*/

/* !LINKSTO EcuM.Impl.EcuM_OnCoreSync,1 */
#if (ECUM_MULTICORE_ENABLED == TRUE)
FUNC(EcuM_SyncActionType, ECUM_APPL_CODE) EcuM_OnCoreSync(void)
{
  const EcuMCoreIdType coreID = ECUM_GET_CORE_ID();
  EcuM_SyncActionType syncValue = ECUM_CONTINUE_SYNC;

  switch(coreID)
  {
    case [!"num:i($ECUM_MASTER_CORE_ID)"!]:
    /* Function blocking on the Master-Core on entry to Sleep(EcuM_GoHalt) or Shutdown(EcuM_GoDown) on EcuM_SlaveCoreSync(). */
    {
      /* If you want to continue waiting for Slave-Cores to synchronize, return ECUM_CONTINUE_SYNC.
       * If you want to abort synchronization with the Slave-Cores, return ECUM_ABORT_SYNC.
       *
       * if (shutdownTarget == ECUM_STATE_SLEEP)
       * {
       * The Master-Core is waiting for the Slave-Cores to signal that they have entered the sleep state.  The
       * Master-Core must synchronize here so that the Ram is not modified while the Master-Core calculates the
       * Ram hash.
       *
       * If this function returns ECUM_ABORT_SYNC and the selected shutdown target is ECUM_STATE_SLEEP, the
       * Master-Core will not enter the sleep state, because the Ram hash would likely not be correct, which
       * would lead to an ECU reset.
       * }
       * else if (shutdownTarget == (ECUM_STATE_OFF || ECUM_STATE_RESET))
       * {
       * The Master-Core is waiting for the Slave-Cores to signal that they have entered the shutdown state.  The
       * Master-Core must synchronize here so that the Slave-Cores are not stopped before they have finished de-initialization.
       *
       * If this function returns ECUM_ABORT_SYNC and the selected shutdown target is ECUM_STATE_OFF, the
       * Master-Core will not enter the shutdown state. The BswM on the Master-Core must then restart the
       * shutdown transition for all cores.
       * }
       */
      break;
    }
  [!FOR "Index" = "0" TO "$MULTICORE_NUM_CORES - 1"!]
  [!IF "$Index != $ECUM_MASTER_CORE_ID"!]
    case [!"num:i($Index)"!]:
    /* Function blocking on Slave-Core ID [!"num:i($Index)"!] on exit from Sleep (EcuM_GoHalt or EcuM_GoPoll) on EcuM_MasterCoreSync(). */
    {
     /* If you want to continue waiting for synchronization with the Master-Core, return ECUM_CONTINUE_SYNC.
      * If you want to abort synchronization with the Master-Core, return ECUM_ABORT_SYNC.
      *
      * The Slave-Core ID [!"num:i($Index)"!] is waiting for the Master-Core to signal that it has completed
      * the verification of the Ram hash (in case the selected sleep mode is halt) and that the other cores are
      * ready to transition into the UP state.
      *
      * If a wakeup event has occured only on a Slave-Core, this callout function can be used to trigger a wakeup
      * event on the other cores (for example, by setting an OsEvent which triggers a user-defined function that
      * sets an EcuM wakeup event on the core that the OsEvent is allocated to).
      *
      * If this function returns ECUM_ABORT_SYNC, the Slave-Core will immediately continue transition into the UP state.  If the
      * Master-Core is still verifying the Ram hash, and the Slave-Core modifies a section of memory that is being verified, the
      * Ram validation may fail, leading to an Ecu reset. The Ram hash is only calculated in the halt sleep mode.
      */
      break;
    }
  [!ENDIF!]
  [!ENDFOR!]
    default:
    {
      break;
    }
  }

  return syncValue;
}
#endif /* ECUM_MULTICORE_ENABLED == TRUE */

#define ECUM_STOP_SEC_CODE
#include <EcuM_MemMap.h>

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

