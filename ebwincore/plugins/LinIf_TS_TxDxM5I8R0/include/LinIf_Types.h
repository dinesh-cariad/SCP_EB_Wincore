/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef LINIF_TYPES_H
#define LINIF_TYPES_H

/*==================[inclusions]=============================================*/

#include <ComStack_Types.h>        /* AUTOSAR standard types */
#include <LinIf_Cfg.h>             /* module static configuration */
/* !LINKSTO dsn.LinIf.PostBuildConfigurationData,1 */
#include <LinIf_CfgTypes.h>        /* LinIf generated config types */

/*==================[macros]=================================================*/

/** \brief Run mode
 * LINIF_RUN_CONTINUOUS - Run continuous schedule table
 * LINIF_RUN_ONCE - Run once schedule table
 * */
/* Run mode type macros */
/** \brief Run the requested schedule table one time */
#if (defined LINIF_RUN_ONCE)
#error LINIF_RUN_ONCE already defined!
#endif /* #if (defined LINIF_RUN_ONCE)*/
#define LINIF_RUN_ONCE                       0U
/** \brief Run the requested schedule table continuously (restart) */
#if (defined LINIF_RUN_CONTINUOUS_RESTART)
#error LINIF_RUN_CONTINUOUS_RESTART already defined!
#endif /* #if (defined LINIF_RUN_CONTINUOUS_RESTART)*/
#define LINIF_RUN_CONTINUOUS_RESTART         1U
/** \brief Run the requested schedule table continuously (resume) */
#if (defined LINIF_RUN_CONTINUOUS_RESUME)
#error LINIF_RUN_CONTINUOUS_RESUME already defined!
#endif /* #if (defined LINIF_RUN_CONTINUOUS_RESUME)*/
#define LINIF_RUN_CONTINUOUS_RESUME          2U

/* Frame type macros */
#if (defined LINIF_FRM_TX)
#error LINIF_FRM_TX already defined!
#endif /* #if (defined LINIF_FRM_TX)*/
#define LINIF_FRM_TX                         0U

#if (defined LINIF_FRM_RX)
#error LINIF_FRM_RX already defined!
#endif /* #if (defined LINIF_FRM_RX)*/
#define LINIF_FRM_RX                         1U

#if (defined LINIF_FRM_MRF)
#error LINIF_FRM_MRF already defined!
#endif /* #if (defined LINIF_FRM_MRF)*/
#define LINIF_FRM_MRF                        2U

#if (defined LINIF_FRM_SRF)
#error LINIF_FRM_SRF already defined!
#endif /* #if (defined LINIF_FRM_SRF)*/
#define LINIF_FRM_SRF                        3U

#if (defined LINIF_FRM_EVENT_TRIGGERED)
#error LINIF_FRM_EVENT_TRIGGERED already defined!
#endif /* #if (defined LINIF_FRM_EVENT_TRIGGERED)*/
#define LINIF_FRM_EVENT_TRIGGERED            4U

#if (defined LINIF_FRM_CONFIG)
#error LINIF_FRM_CONFIG already defined!
#endif /* #if (defined LINIF_FRM_CONFIG)*/
#define LINIF_FRM_CONFIG                     5U

#if (defined LINIF_FRM_SPORADIC)
#error LINIF_FRM_SPORADIC already defined!
#endif /* #if (defined LINIF_FRM_SPORADIC)*/
#define LINIF_FRM_SPORADIC                   6U

#if (defined LINIF_FRM_SLAVE_TO_SLAVE)
#error LINIF_FRM_SLAVE_TO_SLAVE already defined!
#endif /* #if (defined LINIF_FRM_SLAVE_TO_SLAVE)*/
#define LINIF_FRM_SLAVE_TO_SLAVE             7U

#if (defined LINIF_FRM_EMPTY)
#error LINIF_FRM_EMPTY already defined!
#endif /* #if (defined LINIF_FRM_EMPTY)*/
#define LINIF_FRM_EMPTY                    255U

/* Checksum type macros */
#if (defined LINIF_CHECKSUM_CLASSIC)
#error LINIF_CHECKSUM_CLASSIC already defined!
#endif /* #if (defined LINIF_CHECKSUM_CLASSIC)*/
#define LINIF_CHECKSUM_CLASSIC            0x00U

#if (defined LINIF_CHECKSUM_ENHANCED)
#error LINIF_CHECKSUM_ENHANCED already defined!
#endif /* #if (defined LINIF_CHECKSUM_ENHANCED)*/
#define LINIF_CHECKSUM_ENHANCED           0x10U

/* state macros for LinIf StartupState */
/** \brief The state of the channel shall be CHANNEL_OPERATIONAL after startup */
#if (defined LINIF_STARTUP_NORMAL)
#error LINIF_STARTUP_NORMAL already defined!
#endif /* #if (defined LINIF_STARTUP_NORMAL)*/
#define LINIF_STARTUP_NORMAL                 1U

/** \brief  The state of the channel shall be CHANNEL_SLEEP after startup */
#if (defined LINIF_STARTUP_SLEEP)
#error LINIF_STARTUP_SLEEP already defined!
#endif /* #if (defined LINIF_STARTUP_SLEEP)*/
#define LINIF_STARTUP_SLEEP                  2U

/** \brief Invalid reference to EcuM_WakeupSourceId */
#if (defined LINIF_INVALIDECUMWAKEUPSOURCEID)
#error LINIF_INVALIDECUMWAKEUPSOURCEID already defined!
#endif /* #if (defined LINIF_INVALIDECUMWAKEUPSOURCEID)*/
#define LINIF_INVALIDECUMWAKEUPSOURCEID   0xFFU

/** \brief  Defines for invalid values
 * */
#if (defined LINIF_INVALID_ENTRY)
#error LINIF_INVALID_ENTRY already defined!
#endif /* #if (defined LINIF_INVALID_ENTRY)*/
#define   LINIF_INVALID_ENTRY           0xFFFFU

#if (defined LINIF_NULL_ENTRY)
#error LINIF_NULL_ENTRY already defined!
#endif /* #if (defined LINIF_NULL_ENTRY)*/
#define   LINIF_NULL_ENTRY                   0U

#if (defined LINIF_INVALID_SCHEDULE_TABLE)
#error LINIF_INVALID_SCHEDULE_TABLE already defined!
#endif /* #if (defined LINIF_INVALID_SCHEDULE_TABLE)*/
#define   LINIF_INVALID_SCHEDULE_TABLE     255U

#if (defined LINIF_UNUSED_VARIABLE)
#error LINIF_UNUSED_VARIABLE already defined!
#endif /* #if (defined LINIF_UNUSED_VARIABLE)*/
#define   LINIF_UNUSED_VARIABLE              0U

/** \brief Used in ComM channel mapping for non- Lin channels */
#if (defined LINIF_COMM_INVALIDCHANNELID)
#error LINIF_COMM_INVALIDCHANNELID already defined!
#endif /* #if (defined LINIF_COMM_INVALIDCHANNELID)*/
#define LINIF_COMM_INVALIDCHANNELID        255U

/** \brief Used for non LinTrcv referencing channels */
#if (defined LINIF_TRCV_INVALIDCHANNELID)
#error LINIF_TRCV_INVALIDCHANNELID already defined!
#endif /* #if (defined LINIF_TRCV_INVALIDCHANNELID)*/
#define LINIF_TRCV_INVALIDCHANNELID        255U

/*==================[type definitions]=======================================*/

/** \brief Schedule handle type
 **
 ** Variables of this type ares used to access schedule tables */
typedef uint8 LinIf_SchHandleType;

/** \brief Node configuration handler return type
 **
 ** Used to check whether the request is handled locally, by Tp or rejected */
typedef uint8 LinIf_NCHReturnType;

/* --- Lin Driver Configuration -------------------------------------------- */

/** \brief Pointer to function Lin_SendFrame()
 **
 ** This is a function pointer type for the Lin Driver function
 ** Lin_SendFrame().
 */
typedef P2FUNC( Std_ReturnType, LIN_CODE, LinIf_LinDriverSendFrameFctPtrType )
  (
    uint8 Channel,
    #ifdef LINIF_USE_LINAPI_REV431
    const Lin_PduType* PduInfoPtr
    #else
    Lin_PduType* PduInfoPtr
    #endif
  );

/** \brief Pointer to function Lin_GetStatus()
 **
 ** This is a function pointer type for the Lin Driver function
 ** Lin_GetStatus().
 */
typedef P2FUNC( Lin_StatusType, LIN_CODE, LinIf_LinDriverGetStatusFctPtrType )
  (
    uint8 Channel,
    uint8** Lin_SduPtr
  );

#if LINIF_CHECKWAKEUP_SUPPORTED == STD_ON
#ifndef LINIF_USE_LINAPI_REV2
/** \brief Pointer to function Lin_CheckWakeup()
 **
 ** This is a function pointer type for the Lin Driver function
 ** Lin_CheckWakeup().
 */
typedef P2FUNC( Std_ReturnType, LIN_CODE, LinIf_LinDriverCheckWakeupFctPtrType )
  (
    uint8 Channel
  );
#else /* LINIF_USE_LINAPI_REV2 */
/** \brief Pointer to function Lin_WakeupValidation()
 **
 ** This is a function pointer type for the Lin Driver function
 ** Lin_WakeupValidation().
 */
typedef P2FUNC( void, LIN_CODE, LinIf_LinDriverWakeupValidFctPtrType )
  (
    void
  );
#endif /* LINIF_USE_LINAPI_REV2 */
#endif /* LINIF_CHECKWAKEUP_SUPPORTED */

/** \brief Pointer to function Lin_GoToSleep()
 **
 ** This is a function pointer type for the Lin Driver function
 ** Lin_GoToSleep().
 */
typedef P2FUNC( Std_ReturnType, LIN_CODE, LinIf_LinDriverGoToSleepFctPtrType )
  (
    uint8 Channel
  );

/** \brief Pointer to function Lin_GoToSleepInternal()
 **
 ** This is a function pointer type for the Lin Driver function
 ** Lin_GoToSleepInternal().
 */
typedef P2FUNC( Std_ReturnType, LIN_CODE, LinIf_LinDriverGoToSleepIntFctPtrType )
  (
    uint8 Channel
  );

/** \brief Pointer to function Lin_Wakeup()
 **
 ** This is a function pointer type for the Lin Driver function
 ** Lin_Wakeup().
 */
typedef P2FUNC( Std_ReturnType, LIN_CODE, LinIf_LinDriverWakeupFctPtrType )
  (
    uint8 Channel
  );

#ifdef LINIF_USE_LINAPI_42
/** \brief Pointer to function Lin_WakeupInternal()
 **
 ** This is a function pointer type for the Lin Driver function
 ** Lin_WakeupInternal().
 */
typedef P2FUNC( Std_ReturnType, LIN_CODE, LinIf_LinDriverWakeupIntFctPtrType )
  (
    uint8 Channel
  );
#endif /* LINIF_USE_LINAPI_42 */

#if (LINIF_EOS_NOTIF_CALLOUT_SUPPORTED == STD_ON)
/** \brief Pointer to end-of-schedule notification callouts
 **
 ** Using the correct segment is problematic as the implementer
 ** of the callout doesn't include the type defined by LinIf
 ** nor does it hold any regard for the memory sections, also
 ** the Compiler module doesn't expose(define) CALLOUT_CODE,
 ** despite it being referred to.
 ** As such if it doesn't exist, alias it to APPL_CODE.
 **/
#ifndef LINIF_NOTIF_CALLOUT_CODE
#define LINIF_NOTIF_CALLOUT_CODE LINIF_APPL_CODE
#endif

typedef P2FUNC(void, LINIF_NOTIF_CALLOUT_CODE, LinIf_EndOfSchedNotifFctPtrType)
  (
    void
  );
#endif /* LINIF_EOS_NOTIF_CALLOUT_SUPPORTED */

#if (LINIF_UL_CDD_SUPPORTED == STD_ON)
/** \brief Function pointer type for state change confirmation functions
 **
 ** Type used for
 **   LinSM_GotoSleepConfirmation() and
 **   LinSM_WakeupConfirmation() as well as
 **   substitutes (Cdd)
 **/
typedef P2FUNC(void, LINIF_APPL_CODE, LinIf_SlpWuConfFuncPtrType)
  (
    NetworkHandleType network,
    boolean success
  );

#if (LINIF_SLAVE_SUPPORTED == STD_ON)
/** \brief Function pointer type for state change indication function
 **
 ** Type used for
 **   LinSM_GotoSleepIndication() and
 **   substitutes (Cdd)
 **/
 typedef P2FUNC(void, LINIF_APPL_CODE, LinIf_SlpIndicationFuncPtrType)
  (
    NetworkHandleType network
  );
#endif

/** \brief Function pointer type for schedule change confirmation functions
 **
 ** Type used for
 **    LinSM_ScheduleRequestConfirmation() and
 **    substitutes (Cdd)
 **/
typedef P2FUNC(void, LINIF_APPL_CODE, LinIf_SchedReqConfFuncPtrType)
  (
    NetworkHandleType network,
    LinIf_SchHandleType schedule
  );

/** \brief Function pointer type for UL RxIndication
 **
 ** Type used for
 **   PduR_LinIfRxIndication()
 **   UL Cdd RxIndication
 */
typedef P2FUNC(void, LINIF_APPL_CODE, LinIf_RxIndFuncPtrType)
  (
    PduIdType RxPduId,
    P2VAR(PduInfoType, AUTOMATIC, LINIF_APPL_DATA) PduInfoPtr
  );

/** \brief Function pointer used for UL TriggerTransmit
 **
 ** Type used for
 **   PduR_LinIfTriggerTransmit()
 **   UL Cdd TriggerTransmit
 */
typedef P2FUNC(Std_ReturnType, LINIF_APPL_CODE, LinIf_TrgTxFuncPtrType)
  (
    PduIdType TxPduId,
    P2VAR(PduInfoType, AUTOMATIC, LINIF_APPL_DATA) PduInfoPtr
  );

/** \brief Function pointer used for UL TxConfirmation
 **
 ** Type used for
 **   PduR_LinIfTxConfirmation
 **   UL Cdd TxConfirmation
 */
typedef P2FUNC(void, LINIF_APPL_DATA, LinIf_TxConfFuncPtrType)
  (
    PduIdType TxPduId
  );
#endif /* LINIF_UL_CDD_SUPPORTED */

/** \brief Lin Driver function configuration
 **
 ** This type is used for configuring the Lin Driver functions.
 */
typedef struct
{
#if (LINIF_MASTER_SUPPORTED == STD_ON)
  LinIf_LinDriverSendFrameFctPtrType sendFrame;             /**< send frame */
  LinIf_LinDriverGetStatusFctPtrType getStatus;             /**< get status */
#endif
#if LINIF_CHECKWAKEUP_SUPPORTED == STD_ON
#ifndef LINIF_USE_LINAPI_REV2
  LinIf_LinDriverCheckWakeupFctPtrType checkWakeup;         /**< check wakeup */
#else /* LINIF_USE_LINAPI_REV2 */
  LinIf_LinDriverWakeupValidFctPtrType wakeupValidation;    /**< wakeup validation */
#endif /* LINIF_USE_LINAPI_REV2 */
#endif /* LINIF_CHECKWAKEUP_SUPPORTED */
#if (LINIF_MASTER_SUPPORTED == STD_ON)
  LinIf_LinDriverGoToSleepFctPtrType goToSleep;             /**< go to sleep */
#endif
  LinIf_LinDriverGoToSleepIntFctPtrType goToSleepInt;       /**< go to sleep internal */
  LinIf_LinDriverWakeupFctPtrType wakeup;                   /**< wakeup */
#ifdef LINIF_USE_LINAPI_42
  LinIf_LinDriverWakeupIntFctPtrType wakeupInt;             /**< wakeup internal */
#endif /* LINIF_USE_LINAPI_42 */
} LinIf_LinDriverConfigType;

#endif /* ifndef LINIF_TYPES_H */
/*==================[end of file]============================================*/
