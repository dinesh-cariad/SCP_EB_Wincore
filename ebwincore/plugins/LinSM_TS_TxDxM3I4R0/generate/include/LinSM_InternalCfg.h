/**
 * \file
 *
 * \brief AUTOSAR LinSM
 *
 * This file contains the implementation of the AUTOSAR
 * module LinSM.
 *
 * \version 3.4.19
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!AUTOSPACING!]
[!INCLUDE "LinSM_Macros.m"!]
[!CODE!]
#ifndef LINSM_INTERNAL_CFG_H
#define LINSM_INTERNAL_CFG_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* AUTOSAR communication stack types */
#include <LinSM_Version.h>     /* this module's version declaration */

/*==================[macros]================================================*/

#if (defined LINSM_NUM_CHANNELS) /* to prevent double declaration */
#error LINSM_NUM_CHANNELS already defined
#endif /* if (defined LINSM_NUM_CHANNELS) */

#if (defined LINSM_ALL) /* to prevent double declaration */
#error LINSM_ALL already defined
#endif /* if (defined LINSM_ALL) */

#if (defined LINSM_NONE) /* to prevent double declaration */
#error LINSM_NONE already defined
#endif /* if (defined LINSM_NONE) */

#if (defined LINSM_MIXED) /* to prevent double declaration */
#error LINSM_MIXED already defined
#endif /* if (defined LINSM_MIXED) */

#if (defined LINSM_SLEEPSUPPORT_TYPE) /* to prevent double declaration */
#error LINSM_SLEEPSUPPORT_TYPE already defined
#endif /* if (defined LINSM_SLEEPSUPPORT_TYPE) */

#if (defined LINSM_COMM_NW_IS_VALID) /* to prevent double declaration */
#error LINSM_COMM_NW_IS_VALID already defined
#endif /* if (defined LINSM_COMM_NW_IS_VALID) */

#if (defined LINSM_COMM_NW_TO_ID) /* to prevent double declaration */
#error LINSM_COMM_NW_TO_ID already defined
#endif /* if (defined LINSM_COMM_NW_TO_ID) */

#if (defined LINSM_CHANNEL_LINIF_CHANNEL) /* to prevent double declaration */
#error LINSM_CHANNEL_LINIF_CHANNEL already defined
#endif /* if (defined LINSM_CHANNEL_LINIF_CHANNEL) */

#if (defined LINSM_CHANNEL_STATE) /* to prevent double declaration */
#error LINSM_CHANNEL_STATE already defined
#endif /* if (defined LINSM_CHANNEL_STATE) */

#if (defined LINSM_CHANNEL_TIMER) /* to prevent double declaration */
#error LINSM_CHANNEL_TIMER already defined
#endif /* if (defined LINSM_CHANNEL_TIMER) */

#if (LINSM_MASTER_NODES_ENABLED == STD_ON)

#if (defined LINSM_CHANNEL_LASTSCHEDULE) /* to prevent double declaration */
#error LINSM_CHANNEL_LASTSCHEDULE already defined
#endif /* if (defined LINSM_CHANNEL_LASTSCHEDULE) */

#if (defined LINSM_CHANNEL_NOTIFYLASTSCHEDULE) /* to prevent double declaration */
#error LINSM_CHANNEL_NOTIFYLASTSCHEDULE already defined
#endif /* if (defined LINSM_CHANNEL_NOTIFYLASTSCHEDULE) */

#if (defined LINSM_CHANNEL_SCHEDULEREQUESTACTIVE) /* to prevent double declaration */
#error LINSM_CHANNEL_SCHEDULEREQUESTACTIVE already defined
#endif /* if (defined LINSM_CHANNEL_SCHEDULEREQUESTACTIVE) */

#endif /* (LINSM_MASTER_NODES_ENABLED == STD_ON) */

#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)

#if (defined LINSM_CHANNEL_SLAVE_WAITNOCOMREQ) /* to prevent double declaration */
#error LINSM_CHANNEL_SLAVE_WAITNOCOMREQ already defined
#endif /* if (defined LINSM_CHANNEL_SLAVE_WAITNOCOMREQ) */

#if (defined LINSM_CHANNEL_SLAVE_WAITSLEEPCONF) /* to prevent double declaration */
#error LINSM_CHANNEL_SLAVE_WAITSLEEPCONF already defined
#endif /* if (defined LINSM_CHANNEL_SLAVE_WAITSLEEPCONF) */

#if (defined LINSM_CHANNEL_SLAVE_GOTOSLEEPINPROGRESS) /* to prevent double declaration */
#error LINSM_CHANNEL_SLAVE_GOTOSLEEPINPROGRESS already defined
#endif /* if (defined LINSM_CHANNEL_SLAVE_GOTOSLEEPINPROGRESS) */

#endif /* (LINSM_SLAVE_NODES_ENABLED == STD_ON) */

#if (defined LINSM_CHANNEL_STORED_COM) /* to prevent double declaration */
#error LINSM_CHANNEL_STORED_COM already defined
#endif /* if (defined LINSM_CHANNEL_STORED_COM) */

#if (defined LINSM_CHANNEL_REPETITION_TIME) /* to prevent double declaration */
#error LINSM_CHANNEL_REPETITION_TIME already defined
#endif /* if (defined LINSM_CHANNEL_REPETITION_TIME) */

#if (defined LINSM_CHANNEL_CONFTIMEOUT) /* to prevent double declaration */
#error LINSM_CHANNEL_CONFTIMEOUT already defined
#endif /* if (defined LINSM_CHANNEL_CONFTIMEOUT) */

#if (defined LINSM_CHANNEL_MODEREQUQSTREPETITIONTIME) /* to prevent double declaration */
#error LINSM_CHANNEL_MODEREQUQSTREPETITIONTIME already defined
#endif /* if (defined LINSM_CHANNEL_MODEREQUQSTREPETITIONTIME) */

#if (defined LINSM_CHANNEL_SLEEPSUPPORT) /* to prevent double declaration */
#error LINSM_CHANNEL_SLEEPSUPPORT already defined
#endif /* if (defined LINSM_CHANNEL_SLEEPSUPPORT) */

#if (defined LINSM_CHANNEL_COMM_NW) /* to prevent double declaration */
#error LINSM_CHANNEL_COMM_NW already defined
#endif /* if (defined LINSM_CHANNEL_COMM_NW) */

#if (defined LINSM_TRCV_SUPPORT_TYPE) /* to prevent double declaration */
#error LINSM_TRCV_SUPPORT_TYPE already defined
#endif /* #if (defined LINSM_TRCV_SUPPORT_TYPE) */

#if (defined LINSM_NOTIF_TRCV_NORMAL) /* to prevent double declaration */
#error LINSM_NOTIF_TRCV_NORMAL already defined
#endif /* #if (defined LINSM_NOTIF_TRCV_NORMAL) */

#if (defined LINSM_NOTIF_TRCV_SLEEP) /* to prevent double declaration */
#error LINSM_NOTIF_TRCV_SLEEP already defined
#endif /* #if (defined LINSM_NOTIF_TRCV_SLEEP) */

#if (defined LINSM_NOTIF_TRCV) /* to prevent double declaration */
#error LINSM_NOTIF_TRCV already defined
#endif /* #if (defined LINSM_NOTIF_TRCV) */

#if (defined LINSM_NOTIF_SETTRCV) /* to prevent double declaration */
#error LINSM_NOTIF_SETTRCV already defined
#endif /* #if (defined LINSM_NOTIF_SETTRCV) */

#if (defined LINSM_TRCV_ALL_PASSIVE) /* to prevent double declaration */
#error LINSM_TRCV_ALL_PASSIVE already defined
#endif /* #if (defined LINSM_TRCV_ALL_PASSIVE) */

#if (defined LINSM_TRCV_PASSIVE_NONE) /* to prevent double declaration */
#error LINSM_TRCV_PASSIVE_NONE already defined
#endif /* #if (defined LINSM_TRCV_PASSIVE_NONE) */

#if (defined LINSM_TRCV_NONE) /* to prevent double declaration */
#error LINSM_TRCV_NONE already defined
#endif /* #if (defined LINSM_TRCV_NONE) */

#if (defined LINSM_TRCV_MIXED) /* to prevent double declaration */
#error LINSM_TRCV_MIXED already defined
#endif /* #if (defined LINSM_TRCV_MIXED) */

#if (defined LINSM_CALL_SM_MODEINDICATION)      /* To prevent double declaration */
#error LINSM_CALL_SM_MODEINDICATION already defined!
#endif /* #if (defined LINSM_CALL_SM_MODEINDICATION) */

#if (defined LINSM_CALL_SM_BUSSLEEPMODE)      /* To prevent double declaration */
#error LINSM_CALL_SM_BUSSLEEPMODE already defined!
#endif /* #if (defined LINSM_CALL_SM_BUSSLEEPMODE) */


/*------------------[static defines]----------------------------------------*/

/** \brief number of channels handled */
#define LINSM_NUM_CHANNELS  [!"$numChannels"!]U


/* defines for the levels of API features used */

/** \brief all channels use a specific API feature */
#define LINSM_ALL    1U

/** \brief no channel uses a specific API feature */
#define LINSM_NONE   2U

/** \brief some channels use a specific API feature, others don't */
#define LINSM_MIXED  3U

/** \brief level of sleep support used in the configuration
 *
 * - setting LINSM_SLEEPSUPPORT_TYPE to LINSM_ALL signals that all channels
 *   use sleep support. In this case, code for channels not using sleep
 *   support can be omitted.
 *
 * - setting LINSM_SLEEPSUPPORT_TYPE to LINSM_NONE signals that no channel
 *   uses sleep support. In this case, code for the states LINSM_WAKEUP and
 *   LINSM_GOTOSLEEP can be omitted.
 *
 * - setting LINSM_SLEEPSUPPORT_TYPE to LINSM_MIXED signals that there are
 *   both channels with and without sleep support in the configuration.
 *   In this case, the according state transitions must be decided at runtime.
 */
[!IF "$sleepSupportType = 'none'"!]
#define LINSM_SLEEPSUPPORT_TYPE  LINSM_NONE
[!ELSEIF "$sleepSupportType = 'all'"!]
#define LINSM_SLEEPSUPPORT_TYPE  LINSM_ALL
[!ELSE!]
#define LINSM_SLEEPSUPPORT_TYPE  LINSM_MIXED
[!ENDIF!]

[!IF "$multiCoreSupport = 'true'"!][!//
/** \brief SchM_Call for ComM_BusSM_ModeIndication */
#define LINSM_CALL_SM_MODEINDICATION(nw,mode) \
  ((void)LinSM_ChannelConfig[LINSM_COMM_NW_TO_ID(nw)].ModeIndication((nw),(mode)))

/** \brief SchM_Call for ComM_BusSM_BusSleepMode */
#define LINSM_CALL_SM_BUSSLEEPMODE(nw) \
  ((void)LinSM_ChannelConfig[LINSM_COMM_NW_TO_ID(nw)].BusSleepMode((nw)))
[!ELSE!]
/** \brief ComM_Call for ModeIndication */
#define LINSM_CALL_SM_MODEINDICATION(nw,mode) \
   ComM_BusSM_ModeIndication((nw),(&mode))

/** \brief ComM_Call for BusSleepMode */
#define LINSM_CALL_SM_BUSSLEEPMODE(nw) \
   ComM_BusSM_BusSleepMode((nw))
[!ENDIF!]


/** \brief for invalid/master silence after wakeup reserved value */
#define LINSM_INVALID_SILENCE_AFTER_WAKEUP 65535U

/*------------------[network/channel ID lookup]-----------------------------*/

/* ComM networks -> LinSM channels */

[!IF "$LinSMIDMapComMType = 'LUT'"!]
  [!WARNING!]
Suboptimal configuration: ComM network handles (parameter ComMChannelId) [!//
referenced in the LinSM configuration (parameter LinSMComMNetworkHandleRef) [!//
are not consecutive. This will result in code size and runtime overhead.[!//
  [!ENDWARNING!]
[!//
/** \brief return true if the given ComM network is found in the configuration
 *
 * as ComM network IDs aren't consecutive, this is done by a lookup function.
 */
#define LINSM_COMM_NW_IS_VALID(network)  \
        ( LinSM_ComMNwToId(network) < [!"$numChannels"!]U )

/** \brief return the ComM network handle for a given LinSM channel ID
 *
 * as ComM network IDs are not consecutive, this is done using the configuration
 * structure. */
#define LINSM_CHANNEL_COMM_NW(id)  \
        (LinSM_ChannelConfig[(id)].ComMNetwork)

/** \brief return the LinSM channel ID for a given ComM network
 *
 * as ComM network IDs aren't consecutive, this is done by a lookup function.
 */
#define LINSM_COMM_NW_TO_ID(network)  \
        ( LinSM_ComMNwToId(network) )

[!ELSEIF "$LinSMIDMapComMType = 'OFFSET'"!]
/** \brief return true if the given ComM network is found in the configuration
 *
 * as ComM network IDs are consecutive, this is done by a range check. */
#define LINSM_COMM_NW_IS_VALID(network)  \
        (   ((network) >= [!"$LinSMIDComMMin"!]U) \
         && ((network) < [!"num:i($LinSMIDComMMin + $numChannels)"!]U) \
        )

/** \brief return the ComM network handle for a given LinSM channel ID
 *
 * as ComM network IDs are consecutive, this is done by just adding the ComM's
 * ID offset. */
#define LINSM_CHANNEL_COMM_NW(id)  ((id) + [!"$LinSMIDComMMin"!]U)

/** \brief return the LinSM channel ID for a given ComM network
 *
 * this is done by subtracting the ComM's ID offset. */
#define LINSM_COMM_NW_TO_ID(network)  ((network) - [!"$LinSMIDComMMin"!]U)

[!ELSE!][!// Mapped directly
/** \brief return true if the given ComM network is found in the configuration
 *
 * as ComM network IDs are mapped directly, this is done by a range check. */
#define LINSM_COMM_NW_IS_VALID(network)  \
        ((network) < [!"num:i($numChannels)"!]U)

/** \brief return the ComM network handle for a given LinSM channel ID
 *
 * as ComM network IDs are mapped directly, this is done by returning the same
 * id
 */
#define LINSM_CHANNEL_COMM_NW(id)  (id)

/** \brief return the LinSM channel ID for a given ComM network
 *
 * as ComMM network IDs are mapped directly, this is done by returning the same
 * id
 */
#define LINSM_COMM_NW_TO_ID(network)  (network)

[!ENDIF!][!// $LinSMIDMapComMType

[!IF "$LinSMIDMapLinIfType = 'LUT'"!]
  [!WARNING!]
Suboptimal configuration: LinIf channel handles (parameter LinIfChannelId) [!//
referenced in the LinSM configuration (parameter LinSMScheduleIndexRef) [!//
are not consecutive. This will result in code size and runtime overhead
  [!ENDWARNING!]
[!//
/** \brief return the LinIf channel ID for a given LinSM channel ID
 *
 * as LinIf channel IDs are not consecutive, this is done using the configuration
 * structure. */
#define LINSM_CHANNEL_LINIF_CHANNEL(id)  \
        (LinSM_ChannelConfig[(id)].LinIfChannel)

[!ELSEIF "$LinSMIDMapLinIfType = 'OFFSET'"!]
/** \brief return the LinIf channel ID for a given LinSM channel ID
 *
 * as LinIf channel IDs are consecutive, this is done by adding the LinIf's ID
 * offset */
#define LINSM_CHANNEL_LINIF_CHANNEL(id)  ((id) + [!"$LinSMIDLinIfMin"!]U)
[!ELSE!][!// Mapped directly
/** \brief return the LinIf channel ID for a given LinSM channel ID
 *
 * as LinIf channel IDs are mapped directly, this is done by returning the ID
 */
#define LINSM_CHANNEL_LINIF_CHANNEL(id)  (id)
[!ENDIF!][!// $LinSMIDMapLinIfType

[!IF "$numSlaveNodes > 0"!]
/** \brief return if LinSM channel is slave
 *
 * this is used to differenciate the behavior between MASTER and SLAVE nodes */
#define LINSM_CHANNEL_IS_SLAVE(id)  \
        (LinSM_ChannelConfig[(id)].ChIsSlave)
[!ENDIF!][!// $numSlaveNodes > 0

[!//
/*------------------[channel configuration access]--------------------------*/

/** \brief return the current state for a given LinSM channel ID
 *
 * this is also used to set the channel state; it therefore must be an
 * lvalue. */
#define LINSM_CHANNEL_STATE(id)  \
        (LinSM_ChannelData[(id)].State)

/** \brief return the current timer value for a given LinSM channel ID
 *
 * this is also used to set the channel's timer; it therefore must be an
 * lvalue. */
#define LINSM_CHANNEL_TIMER(id)  \
        (LinSM_ChannelData[(id)].Timer)

#if (LINSM_MASTER_NODES_ENABLED == STD_ON)

/** \brief return the last known schedule table for a given LinSM channel ID
 *
 * this is also used to set the schedule table; it therefore must be an
 * lvalue. */
#define LINSM_CHANNEL_LASTSCHEDULE(id)  \
        (LinSM_ChannelData[(id)].LastSchedule)

/** \brief indicates if the next LinSM main function invocation shall
 * call BswM_LinSM_CurrentSchedule with last known schedule table
 * for a given LinSM channel ID
 *
 * type: boolean
 *
 * this macro must be an lvalue. */
#define LINSM_CHANNEL_NOTIFYLASTSCHEDULE(id)  \
       (LinSM_ChannelData[(id)].NotifyLastSchedule)

/** \brief indicates that there is a ScheduleRequest in progress
 *
 * type: boolean
 *
 * this macro must be an lvalue. */
#define LINSM_CHANNEL_SCHEDULEREQUESTACTIVE(id)  \
       (LinSM_ChannelData[(id)].ScheduleRequestActive)

#endif /* (LINSM_MASTER_NODES_ENABLED == STD_ON) */

#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)

/** \brief indicates that there is a RequestComMode expected after BusSleepMode was called
 *
 * type: boolean
 *
 * this macro must be an lvalue. */
#define LINSM_CHANNEL_SLAVE_WAITNOCOMREQ(id)  \
       (LinSM_ChannelData[(id)].WaitNoComReq)

/** \brief indicates that there is a GoToSleep confirmation expected after BusSleepMode was called
 *
 * type: boolean
 *
 * this macro must be an lvalue. */
#define LINSM_CHANNEL_SLAVE_WAITSLEEPCONF(id)  \
       (LinSM_ChannelData[(id)].WaitGoToSleepConf)

/** \brief indicates that there is a GoToSleep is in progress so wakeup should not restart
 *
 * type: boolean
 *
 * this macro must be an lvalue. */
#define LINSM_CHANNEL_SLAVE_GOTOSLEEPINPROGRESS(id)  \
       (LinSM_ChannelData[(id)].GoToSleepInProgress)

#endif /* (LINSM_SLAVE_NODES_ENABLED == STD_ON) */

#define LINSM_CHANNEL_STORED_COM(id)  \
       (LinSM_ChannelData[(id)].StoredCom)

/** \brief return the repetition max value for a given LinSM channel ID */
#define LINSM_CHANNEL_REPETITION_TIME(id)  \
        (LinSM_ChannelData[(id)].RepetitionTime)

/** \brief return the confirmation timeout for a given LinSM channel ID */
#define LINSM_CHANNEL_CONFTIMEOUT(id)  \
        (LinSM_ChannelConfig[(id)].ConfirmationTimeout)


   /** \brief return if a given LinSM channel RepetitionTime */
#define LINSM_CHANNEL_MODEREQUQSTREPETITIONTIME(id)  \
        (LinSM_ChannelConfig[(id)].ModeRequestRepetitionTime)


/** \brief return the configured silence after wakeup timeout for a given LinSM channel ID */
#define LINSM_CHANNEL_SILENCEAFTERWUPTIMEOUT(id)  \
        (LinSM_ChannelConfig[(id)].SilenceAfterWakeupTimeout)

/** \brief return the value of the silence after wakeup timer */
#define LINSM_CHANNEL_SILENCE_TIME(id)  \
        (LinSM_ChannelData[(id)].SilenceTime)

[!IF "$sleepSupportType = 'mixed'"!]
/** \brief return if a given LinSM channel supports sleep mode */
#define LINSM_CHANNEL_SLEEPSUPPORT(id)  \
        (LinSM_ChannelConfig[(id)].SleepSupport & 0x01U)
[!ENDIF!]

/** \brief all channels are configured as passive (LinSMTransceiverPassiveMode = true) */
#define LINSM_TRCV_ALL_PASSIVE    1U

/** \brief all channels are configured as active (LinSMTransceiverPassiveMode = false) */
#define LINSM_TRCV_PASSIVE_NONE   2U

/** \brief no channels have LinSMTransceiverPassiveMode enabled */
#define LINSM_TRCV_NONE           3U

/** \brief some channels have LinSMTransceiverPassiveMode configured as true, others as false */
#define LINSM_TRCV_MIXED          4U

/** \brief level of support used in the configuration
 *
 * - setting LINSM_TRCV_SUPPORT_TYPE to LINSM_TRCV_NONE signals that no channel
 *   uses LinTrcv support. In this case code can be omitted.
 *
 * - setting LINSM_TRCV_SUPPORT_TYPE to LINSM_TRCV_ALL_PASSIVE or LINSM_TRCV_PASSIVE_NONE
 *   signals that all the channels are configured alike, lookup code can be omitted.
 *
 * - setting LINSM_TRCV_SUPPORT_TYPE to LINSM_TRCV_MIXED signals that there are
 *   channels with LinSMTransceiverPassiveMode configured as true and as false as well.
 *   In this case, the LinTrcv_TrcvModeType must be decided at runtime.
 */

[!IF "$trcvSupportType = 'none'"!]
#define LINSM_TRCV_SUPPORT_TYPE  LINSM_TRCV_NONE
#define LINSM_NOTIF_TRCV_NORMAL(id)    ((void)0U)
#define LINSM_NOTIF_TRCV_SLEEP(id)     ((void)0U)
[!ELSE!]
#define LINSM_NOTIF_SETTRCV(id, mode)  \
        ((void)LinIf_SetTrcvMode((LINSM_CHANNEL_LINIF_CHANNEL(id)), (mode)))
[!IF "$numChannels = $numChTrcv"!]
#define LINSM_NOTIF_TRCV(id, mode)     LINSM_NOTIF_SETTRCV((id), (mode))
[!ELSE!]
#define LINSM_NOTIF_TRCV(id, mode)  \
  do {                                                         \
    if((LinSM_ChannelConfig[(id)].SleepSupport & 0x02U) > 0U)  \
    {                                                          \
      LINSM_NOTIF_SETTRCV((id), (mode));                       \
    }                                                          \
  } while (0U)
[!ENDIF!]

#define LINSM_NOTIF_TRCV_NORMAL(id)  LINSM_NOTIF_TRCV((id), LINTRCV_TRCV_MODE_NORMAL)

[!IF "$trcvSupportType = 'allPassive'"!]
#define LINSM_TRCV_SUPPORT_TYPE      LINSM_TRCV_ALL_PASSIVE
#define LINSM_NOTIF_TRCV_SLEEP(id)   LINSM_NOTIF_TRCV((id), LINTRCV_TRCV_MODE_STANDBY)
[!ELSEIF "$trcvSupportType = 'passiveNone'"!]
#define LINSM_TRCV_SUPPORT_TYPE      LINSM_TRCV_PASSIVE_NONE
#define LINSM_NOTIF_TRCV_SLEEP(id)   LINSM_NOTIF_TRCV((id), LINTRCV_TRCV_MODE_SLEEP)
[!ELSE!]
#define LINSM_TRCV_SUPPORT_TYPE      LINSM_TRCV_MIXED
#define LINSM_NOTIF_TRCV_SLEEP(id)   \
  do {                                                         \
    if((LinSM_ChannelConfig[(id)].SleepSupport & 0x04U) > 0U)  \
    {                                                          \
      LINSM_NOTIF_SETTRCV((id), LINTRCV_TRCV_MODE_STANDBY);    \
    }                                                          \
    else                                                       \
    {                                                          \
      LINSM_NOTIF_TRCV((id), LINTRCV_TRCV_MODE_SLEEP);         \
    }                                                          \
  } while (0U)
[!ENDIF!]
[!ENDIF!]

/*==================[type definitions]======================================*/
[!IF "$multiCoreSupport = 'true'"!][!//
/** \brief Define function type for SchM call */
typedef P2FUNC( Std_ReturnType, LINSM_CODE, LinSM_Nw_CallsFctPtrType )
  (
    uint8 nw,
    uint8 ComMode
  );

typedef P2FUNC( Std_ReturnType, LINSM_CODE, LinSM_Nw_BusSleepFctPtrType )
  (
    uint8 Channel
  );
[!ELSE!]
typedef P2FUNC( void, LINSM_CODE, LinSM_Nw_CallsFctPtrType )
  (
    NetworkHandleType nw,
    ComM_ModeType ComMode
  );

typedef P2FUNC( void, LINSM_CODE, LinSM_Nw_BusSleepFctPtrType )
  (
    NetworkHandleType Channel
  );
[!ENDIF!]

/** \brief type big enough to hold our channel identifiers */
typedef NetworkHandleType LinSM_ChannelIdType;

/** \brief type to hold confirmation timeout value in ticks */
typedef uint16 LinSM_TimerValueType;

/** \brief type for static configuration of a channel */
typedef struct
{
[!IF "$multiCoreSupport = 'true'"!]
  LinSM_Nw_CallsFctPtrType  ModeIndication; /**< function pointer to SchM ModeIndication function */
[!IF "$numSlaveNodes > 0"!]
  LinSM_Nw_BusSleepFctPtrType  BusSleepMode; /**< function pointer to SchM BusSleepMode function */
[!ENDIF!]
[!ENDIF!]
  LinSM_TimerValueType  ConfirmationTimeout;
[!IF "$numSlaveNodes > 0"!]
  LinSM_TimerValueType  SilenceAfterWakeupTimeout;
[!ENDIF!]
[!IF "$LinSMIDMapComMType = 'LUT'"!]
  NetworkHandleType  ComMNetwork;
[!ENDIF!]
[!IF "$LinSMIDMapLinIfType = 'LUT'"!]
  NetworkHandleType  LinIfChannel;
[!ENDIF!]
[!IF "$sleepSupportType = 'mixed' or $trcvSupportType != 'none'"!]
  uint8  SleepSupport;
[!ENDIF!]
  uint8  ModeRequestRepetitionTime;
[!IF "$numSlaveNodes > 0"!]
  boolean  ChIsSlave;
[!ENDIF!]
} LinSM_ChannelConfigType;

/** \brief type for runtime data of a channel */
typedef struct
{
  LinSM_TimerValueType  Timer;
  LinSM_TimerValueType  SilenceTime;
  uint8  State;
  uint8  StoredCom;
  uint8  RepetitionTime;
#if (LINSM_MASTER_NODES_ENABLED == STD_ON)
  LinIf_SchHandleType  LastSchedule;
  boolean  NotifyLastSchedule;
  boolean  ScheduleRequestActive;
#endif /* (LINSM_MASTER_NODES_ENABLED == STD_ON) */
#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
  boolean  WaitNoComReq;
  boolean  WaitGoToSleepConf;
  boolean  GoToSleepInProgress;
#endif /* (LINSM_SLAVE_NODES_ENABLED == STD_ON) */
} LinSM_ChannelDataType;

/*==================[external constants]====================================*/

#define LINSM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/** \brief channel configuration */
extern CONST(LinSM_ChannelConfigType, LINSM_APPL_CONST) LinSM_ChannelConfig[[!"$numChannels"!]];

#define LINSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/*==================[external data]=========================================*/

#define LINSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/** \brief runtime channel data */
extern VAR(LinSM_ChannelDataType, LINSM_VAR_CLEARED) LinSM_ChannelData[[!"$numChannels"!]];

#define LINSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/*==================[external function declarations]========================*/

[!IF "$LinSMIDMapComMType = 'LUT'"!]
[!//
#define LINSM_START_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/** \brief maps ComM network handles to LinSM channel index
 *
 * \param[in] network  ComM network ID
 *
 * \retval < [!"$numChannels"!]U: LinSM channel index (channel found)
 * \retval >= [!"$numChannels"!]U: no matching channel in configuration
 */
extern FUNC(LinSM_ChannelIdType, LINSM_CODE) LinSM_ComMNwToId
(
  NetworkHandleType network
);

#define LINSM_STOP_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
[!ENDIF!][!// IF "$LinSMIDMapComMType = 'LUT'"
[!//
#endif /* ifndef LINSM_INTERNAL_CFG_H */
/*==================[end of file]===========================================*/

[!ENDCODE!]
