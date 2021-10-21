#ifndef FR_GENERAL_TYPES
/* !LINKSTO Base.ASR403.FR499,1 */
#define FR_GENERAL_TYPES

/**
 * \file
 *
 * \brief AUTOSAR Base
 *
 * This file contains the implementation of the AUTOSAR
 * module Base.
 *
 * \version 5.0.31
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO Base.ASR403.FR117,1, Base.ASR403.FR455,1, Base.ASR403.FR102,1, Base.ASR403.FR657,1, Base.ASR403.FR500,1 */

/*==================[inclusions]=============================================*/

#include <Std_Types.h>      /* basic AUTOSAR type definitions */

/*==================[macros]=================================================*/

/** \brief map FR_SLOTMODE_SINGLE to FR_SLOTMODE_KEYSLOT for ASR3.x compliance */
/* !LINKSTO Base.ASR403.FR599,1 */
#if (defined FR_SLOTMODE_SINGLE)
#error FR_SLOTMODE_SINGLE is already defined
#endif
#define FR_SLOTMODE_SINGLE FR_SLOTMODE_KEYSLOT

#if (defined FR_CIDX_GDCYCLE)
#error FR_CIDX_GDCYCLE is already defined
#endif
/** \brief Index for config parameter FrIfGdCycle */
#define FR_CIDX_GDCYCLE                         0U

#if (defined FR_CIDX_PMICROPERCYCLE)
#error FR_CIDX_PMICROPERCYCLE is already defined
#endif
/** \brief Index for config parameter FrPMicroPerCycle */
#define FR_CIDX_PMICROPERCYCLE                  1U

#if (defined FR_CIDX_PDLISTENTIMEOUT)
#error FR_CIDX_PDLISTENTIMEOUT is already defined
#endif
/** \brief Index for config parameter FrPdListenTimeout */
#define FR_CIDX_PDLISTENTIMEOUT                 2U

#if (defined FR_CIDX_GMACROPERCYCLE)
#error FR_CIDX_GMACROPERCYCLE is already defined
#endif
/** \brief Index for config parameter FrIfGMacroPerCycle */
#define FR_CIDX_GMACROPERCYCLE                  3U

#if (defined FR_CIDX_GDMACROTICK)
#error FR_CIDX_GDMACROTICK is already defined
#endif
/** \brief Index for config parameter FrIfGdMacrotick */
#define FR_CIDX_GDMACROTICK                     4U

#if (defined FR_CIDX_GNUMBEROFMINISLOTS)
#error FR_CIDX_GNUMBEROFMINISLOTS is already defined
#endif
/** \brief Index for config parameter FrIfGNumberOfMinislots */
#define FR_CIDX_GNUMBEROFMINISLOTS              5U

#if (defined FR_CIDX_GNUMBEROFSTATICSLOTS)
#error FR_CIDX_GNUMBEROFSTATICSLOTS is already defined
#endif
/** \brief Index for config parameter FrIfGNumberOfStaticSlots */
#define FR_CIDX_GNUMBEROFSTATICSLOTS            6U

#if (defined FR_CIDX_GDNIT)
#error FR_CIDX_GDNIT is already defined
#endif
/** \brief Index for config parameter FrIfGdNit */
#define FR_CIDX_GDNIT                           7U

#if (defined FR_CIDX_GDSTATICSLOT)
#error FR_CIDX_GDSTATICSLOT is already defined
#endif
/** \brief Index for config parameter FrIfGdStaticSlot */
#define FR_CIDX_GDSTATICSLOT                    8U

#if (defined FR_CIDX_GDWAKEUPRXWINDOW)
#error FR_CIDX_GDWAKEUPRXWINDOW is already defined
#endif
/** \brief Index for config parameter FrIfGdWakeupRxWindow */
#define FR_CIDX_GDWAKEUPRXWINDOW                9U

#if (defined FR_CIDX_PKEYSLOTID)
#error FR_CIDX_PKEYSLOTID is already defined
#endif
/** \brief Index for config parameter FrPKeySlotId */
#define FR_CIDX_PKEYSLOTID                      10U

#if (defined FR_CIDX_PLATESTTX)
#error FR_CIDX_PLATESTTX is already defined
#endif
/** \brief Index for config parameter FrPLatestTx */
#define FR_CIDX_PLATESTTX                       11U

#if (defined FR_CIDX_POFFSETCORRECTIONOUT)
#error FR_CIDX_POFFSETCORRECTIONOUT is already defined
#endif
/** \brief Index for config parameter FrPOffsetCorrectionOut */
#define FR_CIDX_POFFSETCORRECTIONOUT            12U

#if (defined FR_CIDX_POFFSETCORRECTIONSTART)
#error FR_CIDX_POFFSETCORRECTIONSTART is already defined
#endif
/** \brief Index for config parameter FrPOffsetCorrectionStart */
#define FR_CIDX_POFFSETCORRECTIONSTART          13U

#if (defined FR_CIDX_PRATECORRECTIONOUT)
#error FR_CIDX_PRATECORRECTIONOUT is already defined
#endif
/** \brief Index for config parameter FrPRateCorrectionOut */
#define FR_CIDX_PRATECORRECTIONOUT              14U

#if (defined FR_CIDX_PSECONDKEYSLOTID)
#error FR_CIDX_PSECONDKEYSLOTID is already defined
#endif
/** \brief Index for config parameter FrPSecondKeySlotId */
#define FR_CIDX_PSECONDKEYSLOTID                15U

#if (defined FR_CIDX_PDACCEPTEDSTARTUPRANGE)
#error FR_CIDX_PDACCEPTEDSTARTUPRANGE is already defined
#endif
/** \brief Index for config parameter FrPdAcceptedStartupRange */
#define FR_CIDX_PDACCEPTEDSTARTUPRANGE          16U

#if (defined FR_CIDX_GCOLDSTARTATTEMPTS)
#error FR_CIDX_GCOLDSTARTATTEMPTS is already defined
#endif
/** \brief Index for config parameter FrIfGColdStartAttempts */
#define FR_CIDX_GCOLDSTARTATTEMPTS              17U

#if (defined FR_CIDX_GCYCLECOUNTMAX)
#error FR_CIDX_GCYCLECOUNTMAX is already defined
#endif
/** \brief Index for config parameter FrIfGCycleCountMax */
#define FR_CIDX_GCYCLECOUNTMAX                  18U

#if (defined FR_CIDX_GLISTENNOISE)
#error FR_CIDX_GLISTENNOISE is already defined
#endif
/** \brief Index for config parameter FrIfGListenNoise */
#define FR_CIDX_GLISTENNOISE                    19U

#if (defined FR_CIDX_GMAXWITHOUTCLOCKCORRECTFATAL)
#error FR_CIDX_GMAXWITHOUTCLOCKCORRECTFATAL is already defined
#endif
/** \brief Index for config parameter FrIfGMaxWithoutClockCorrectFatal */
#define FR_CIDX_GMAXWITHOUTCLOCKCORRECTFATAL    20U

#if (defined FR_CIDX_GMAXWITHOUTCLOCKCORRECTPASSIVE)
#error FR_CIDX_GMAXWITHOUTCLOCKCORRECTPASSIVE is already defined
#endif
/** \brief Index for config parameter FrIfGMaxWithoutClockCorrectPassive */
#define FR_CIDX_GMAXWITHOUTCLOCKCORRECTPASSIVE  21U

#if (defined FR_CIDX_GNETWORKMANAGEMENTVECTORLENGTH)
#error FR_CIDX_GNETWORKMANAGEMENTVECTORLENGTH is already defined
#endif
/** \brief Index for config parameter FrIfGNetworkManagementVectorLength */
#define FR_CIDX_GNETWORKMANAGEMENTVECTORLENGTH  22U

#if (defined FR_CIDX_GPAYLOADLENGTHSTATIC)
#error FR_CIDX_GPAYLOADLENGTHSTATIC is already defined
#endif
/** \brief Index for config parameter FrIfGPayloadLengthStatic */
#define FR_CIDX_GPAYLOADLENGTHSTATIC            23U

#if (defined FR_CIDX_GSYNCFRAMEIDCOUNTMAX)
#error FR_CIDX_GSYNCFRAMEIDCOUNTMAX is already defined
#endif
/** \brief Index for config parameter FrIfGSyncFrameIDCountMax */
#define FR_CIDX_GSYNCFRAMEIDCOUNTMAX            24U

#if (defined FR_CIDX_GDACTIONPOINTOFFSET)
#error FR_CIDX_GDACTIONPOINTOFFSET is already defined
#endif
/** \brief Index for config parameter FrIfGdActionPointOffset */
#define FR_CIDX_GDACTIONPOINTOFFSET             25U

#if (defined FR_CIDX_GDBIT)
#error FR_CIDX_GDBIT is already defined
#endif
/** \brief Index for config parameter FrIfGdBit */
#define FR_CIDX_GDBIT                           26U

#if (defined FR_CIDX_GDCASRXLOWMAX)
#error FR_CIDX_GDCASRXLOWMAX is already defined
#endif
/** \brief Index for config parameter FrIfGdCasRxLowMax */
#define FR_CIDX_GDCASRXLOWMAX                   27U

#if (defined FR_CIDX_GDDYNAMICSLOTIDLEPHASE)
#error FR_CIDX_GDDYNAMICSLOTIDLEPHASE is already defined
#endif
/** \brief Index for config parameter FrIfGdDynamicSlotIdlePhase */
#define FR_CIDX_GDDYNAMICSLOTIDLEPHASE          28U

#if (defined FR_CIDX_GDMINISLOTACTIONPOINTOFFSET)
#error FR_CIDX_GDMINISLOTACTIONPOINTOFFSET is already defined
#endif
/** \brief Index for config parameter FrIfGdMiniSlotActionPointOffset */
#define FR_CIDX_GDMINISLOTACTIONPOINTOFFSET     29U

#if (defined FR_CIDX_GDMINISLOT)
#error FR_CIDX_GDMINISLOT is already defined
#endif
/** \brief Index for config parameter FrIfGdMinislot */
#define FR_CIDX_GDMINISLOT                      30U

#if (defined FR_CIDX_GDSAMPLECLOCKPERIOD)
#error FR_CIDX_GDSAMPLECLOCKPERIOD is already defined
#endif
/** \brief Index for config parameter FrIfGdSampleClockPeriod */
#define FR_CIDX_GDSAMPLECLOCKPERIOD             31U

#if (defined FR_CIDX_GDSYMBOLWINDOW)
#error FR_CIDX_GDSYMBOLWINDOW is already defined
#endif
/** \brief Index for config parameter FrIfGdSymbolWindow */
#define FR_CIDX_GDSYMBOLWINDOW                  32U

#if (defined FR_CIDX_GDSYMBOLWINDOWACTIONPOINTOFFSET)
#error FR_CIDX_GDSYMBOLWINDOWACTIONPOINTOFFSET is already defined
#endif
/** \brief Index for config parameter FrIfGdSymbolWindowActionPointOffset */
#define FR_CIDX_GDSYMBOLWINDOWACTIONPOINTOFFSET 33U

#if (defined FR_CIDX_GDTSSTRANSMITTER)
#error FR_CIDX_GDTSSTRANSMITTER is already defined
#endif
/** \brief Index for config parameter FrIfGdTssTransmitter */
#define FR_CIDX_GDTSSTRANSMITTER                34U

#if (defined FR_CIDX_GDWAKEUPRXIDLE)
#error FR_CIDX_GDWAKEUPRXIDLE is already defined
#endif
/** \brief Index for config parameter FrIfGdWakeupRxIdle */
#define FR_CIDX_GDWAKEUPRXIDLE                  35U

#if (defined FR_CIDX_GDWAKEUPRXLOW)
#error FR_CIDX_GDWAKEUPRXLOW is already defined
#endif
/** \brief Index for config parameter FrIfGdWakeupRxLow */
#define FR_CIDX_GDWAKEUPRXLOW                   36U

#if (defined FR_CIDX_GDWAKEUPTXACTIVE)
#error FR_CIDX_GDWAKEUPTXACTIVE is already defined
#endif
/** \brief Index for config parameter FrIfGdWakeupTxActive */
#define FR_CIDX_GDWAKEUPTXACTIVE                37U

#if (defined FR_CIDX_GDWAKEUPTXIDLE)
#error FR_CIDX_GDWAKEUPTXIDLE is already defined
#endif
/** \brief Index for config parameter FrIfGdWakeupTxIdle */
#define FR_CIDX_GDWAKEUPTXIDLE                  38U

#if (defined FR_CIDX_PALLOWPASSIVETOACTIVE)
#error FR_CIDX_PALLOWPASSIVETOACTIVE is already defined
#endif
/** \brief Index for config parameter FrPAllowPassiveToActive */
#define FR_CIDX_PALLOWPASSIVETOACTIVE           39U

#if (defined FR_CIDX_PCHANNELS)
#error FR_CIDX_PCHANNELS is already defined
#endif
/** \brief Index for config parameter FrPChannels */
#define FR_CIDX_PCHANNELS                       40U

#if (defined FR_CIDX_PCLUSTERDRIFTDAMPING)
#error FR_CIDX_PCLUSTERDRIFTDAMPING is already defined
#endif
/** \brief Index for config parameter FrPClusterDriftDamping */
#define FR_CIDX_PCLUSTERDRIFTDAMPING            41U

#if (defined FR_CIDX_PDECODINGCORRECTION)
#error FR_CIDX_PDECODINGCORRECTION is already defined
#endif
/** \brief Index for config parameter FrPDecodingCorrection */
#define FR_CIDX_PDECODINGCORRECTION             42U

#if (defined FR_CIDX_PDELAYCOMPENSATIONA)
#error FR_CIDX_PDELAYCOMPENSATIONA is already defined
#endif
/** \brief Index for config parameter FrPDelayCompensationA */
#define FR_CIDX_PDELAYCOMPENSATIONA             43U

#if (defined FR_CIDX_PDELAYCOMPENSATIONB)
#error FR_CIDX_PDELAYCOMPENSATIONB is already defined
#endif
/** \brief Index for config parameter FrPDelayCompensationB */
#define FR_CIDX_PDELAYCOMPENSATIONB             44U

#if (defined FR_CIDX_PMACROINITIALOFFSETA)
#error FR_CIDX_PMACROINITIALOFFSETA is already defined
#endif
/** \brief Index for config parameter FrPMacroInitialOffsetA */
#define FR_CIDX_PMACROINITIALOFFSETA            45U

#if (defined FR_CIDX_PMACROINITIALOFFSETB)
#error FR_CIDX_PMACROINITIALOFFSETB is already defined
#endif
/** \brief Index for config parameter FrPMacroInitialOffsetB */
#define FR_CIDX_PMACROINITIALOFFSETB            46U

#if (defined FR_CIDX_PMICROINITIALOFFSETA)
#error FR_CIDX_PMICROINITIALOFFSETA is already defined
#endif
/** \brief Index for config parameter FrPMicroInitialOffsetA */
#define FR_CIDX_PMICROINITIALOFFSETA            47U

#if (defined FR_CIDX_PMICROINITIALOFFSETB)
#error FR_CIDX_PMICROINITIALOFFSETB is already defined
#endif
/** \brief Index for config parameter FrPMicroInitialOffsetB */
#define FR_CIDX_PMICROINITIALOFFSETB            48U

#if (defined FR_CIDX_PPAYLOADLENGTHDYNMAX)
#error FR_CIDX_PPAYLOADLENGTHDYNMAX is already defined
#endif
/** \brief Index for config parameter FrPPayloadLengthDynMax */
#define FR_CIDX_PPAYLOADLENGTHDYNMAX            49U

#if (defined FR_CIDX_PSAMPLESPERMICROTICK)
#error FR_CIDX_PSAMPLESPERMICROTICK is already defined
#endif
/** \brief Index for config parameter FrPSamplesPerMicrotick */
#define FR_CIDX_PSAMPLESPERMICROTICK            50U

#if (defined FR_CIDX_PWAKEUPCHANNEL)
#error FR_CIDX_PWAKEUPCHANNEL is already defined
#endif
/** \brief Index for config parameter FrPWakeupChannel */
#define FR_CIDX_PWAKEUPCHANNEL                  51U

#if (defined FR_CIDX_PWAKEUPPATTERN)
#error FR_CIDX_PWAKEUPPATTERN is already defined
#endif
/** \brief Index for config parameter FrPWakeupPattern */
#define FR_CIDX_PWAKEUPPATTERN                  52U

#if (defined FR_CIDX_PDMICROTICK)
#error FR_CIDX_PDMICROTICK is already defined
#endif
/** \brief Index for config parameter FrPdMicrotick */
#define FR_CIDX_PDMICROTICK                     53U

#if (defined FR_CIDX_GDIGNOREAFTERTX)
#error FR_CIDX_GDIGNOREAFTERTX is already defined
#endif
/** \brief Index for config parameter FrIfGdIgnoreAfterTx */
#define FR_CIDX_GDIGNOREAFTERTX                 54U

#if (defined FR_CIDX_PALLOWHALTDUETOCLOCK)
#error FR_CIDX_PALLOWHALTDUETOCLOCK is already defined
#endif
/** \brief Index for config parameter FrPAllowHaltDueToClock */
#define FR_CIDX_PALLOWHALTDUETOCLOCK            55U

#if (defined FR_CIDX_PEXTERNALSYNC)
#error FR_CIDX_PEXTERNALSYNC is already defined
#endif
/** \brief Index for config parameter FrPExternalSync */
#define FR_CIDX_PEXTERNALSYNC                   56U

#if (defined FR_CIDX_PFALLBACKINTERNAL)
#error FR_CIDX_PFALLBACKINTERNAL is already defined
#endif
/** \brief Index for config parameter FrPFallBackInternal */
#define FR_CIDX_PFALLBACKINTERNAL               57U

#if (defined FR_CIDX_PKEYSLOTONLYENABLED)
#error FR_CIDX_PKEYSLOTONLYENABLED is already defined
#endif
/** \brief Index for config parameter FrPKeySlotOnlyEnabled */
#define FR_CIDX_PKEYSLOTONLYENABLED             58U

#if (defined FR_CIDX_PKEYSLOTUSEDFORSTARTUP)
#error FR_CIDX_PKEYSLOTUSEDFORSTARTUP is already defined
#endif
/** \brief Index for config parameter FrPKeySlotUsedForStartup */
#define FR_CIDX_PKEYSLOTUSEDFORSTARTUP          59U

#if (defined FR_CIDX_PKEYSLOTUSEDFORSYNC)
#error FR_CIDX_PKEYSLOTUSEDFORSYNC is already defined
#endif
/** \brief Index for config parameter FrPKeySlotUsedForSync */
#define FR_CIDX_PKEYSLOTUSEDFORSYNC             60U

#if (defined FR_CIDX_PNMVECTOREARLYUPDATE)
#error FR_CIDX_PNMVECTOREARLYUPDATE is already defined
#endif
/** \brief Index for config parameter FrPNmVectorEarlyUpdate */
#define FR_CIDX_PNMVECTOREARLYUPDATE            61U

#if (defined FR_CIDX_PTWOKEYSLOTMODE)
#error FR_CIDX_PTWOKEYSLOTMODE is already defined
#endif
/** \brief Index for config parameter FrPTwoKeySlotMode */
#define FR_CIDX_PTWOKEYSLOTMODE                 62U

/*==================[type definitions]=======================================*/

/* !LINKSTO Base.ASR403.FR110,1, Base.ASR403.FR077,1 */

/** \brief This enumerator type represents the FlexRay controller POC states. */
/* !LINKSTO Base.ASR403.FR505,1 */
typedef enum
{
    FR_POCSTATE_CONFIG = 0,     /**< config state */
    FR_POCSTATE_DEFAULT_CONFIG, /**< default config state */
    FR_POCSTATE_HALT,           /**< halt state */
    FR_POCSTATE_NORMAL_ACTIVE,  /**< normal active state */
    FR_POCSTATE_NORMAL_PASSIVE, /**< normal passive state */
    FR_POCSTATE_READY,          /**< ready state */
    FR_POCSTATE_STARTUP,        /**< startup state */
    FR_POCSTATE_WAKEUP          /**< wakeup state */
} Fr_POCStateType;

/** \brief This enumerator type represents the FlexRay controller slotmodes. */
/* !LINKSTO Base.ASR403.FR506,1 */
typedef enum
{
    FR_SLOTMODE_KEYSLOT = 0,    /**< key slot mode */
    FR_SLOTMODE_ALL_PENDING,   /**< all peding slot mode */
    FR_SLOTMODE_ALL            /**< all slot mode */
} Fr_SlotModeType;

/** \brief This enumerator type represents the FlexRay controller errormode. */
/* !LINKSTO Base.ASR403.FR507,1 */
typedef enum
{
    FR_ERRORMODE_ACTIVE = 0,    /**< acvtive errormode */
    FR_ERRORMODE_PASSIVE,       /**< passive errormode */
    FR_ERRORMODE_COMM_HALT      /**< communication halted errormode */
} Fr_ErrorModeType;

/** \brief This enumerator type represents the wakeup status type. */
/* !LINKSTO Base.ASR403.FR508,1 */
typedef enum
{
    FR_WAKEUP_UNDEFINED = 0,    /**< wakeup state "undefined" */
    FR_WAKEUP_RECEIVED_HEADER,  /**< wakeup state "received header" */
    FR_WAKEUP_RECEIVED_WUP,     /**< wakeup state "received wakeup pattern" */
    FR_WAKEUP_COLLISION_HEADER, /**< wakeup state "collsision header" */
    FR_WAKEUP_COLLISION_WUP,    /**< wakeup state "collision wakeup pattern" */
    FR_WAKEUP_COLLISION_UNKNOWN,/**< wakeup state "collision unknown" */
    FR_WAKEUP_TRANSMITTED       /**< wakeup state "transmitted" */
} Fr_WakeupStatusType;

/** \brief This enumerator type represents the startup status type. */
/* !LINKSTO Base.ASR403.FR509,1 */
typedef enum
{
    FR_STARTUP_UNDEFINED = 0,                   /**< startup state "undefined" */
    FR_STARTUP_COLDSTART_LISTEN,                /**< startup state "coldstart listen" */
    FR_STARTUP_INTEGRATION_COLDSTART_CHECK,     /**< startup state "integration coldstart check" */
    FR_STARTUP_COLDSTART_JOIN,                  /**< startup state "coldstart join" */
    FR_STARTUP_COLDSTART_COLLISION_RESOLUTION,  /**< startup state "collision resolution" */
    FR_STARTUP_COLDSTART_CONSISTENCY_CHECK,     /**< startup state "consistency check" */
    FR_STARTUP_INTEGRATION_LISTEN,              /**< startup state "integration listen" */
    FR_STARTUP_INITIALIZE_SCHEDULE,             /**< startup state "initialize schedule" */
    FR_STARTUP_INTEGRATION_CONSISTENCY_CHECK,   /**< startup state "integration consistency
                                                 *   check" */
    FR_STARTUP_COLDSTART_GAP,                   /**< startup state "coldstart gap" */
    FR_STARTUP_EXTERNAL_STARTUP                 /**< startup state "external" */
} Fr_StartupStateType;

/** \brief This structure contains the POC-Status information. */
/* !LINKSTO Base.ASR403.FR510,1 */
typedef struct
{
    VAR(Fr_ErrorModeType, TYPEDEF) ErrorMode;       /**<  CC - error mode */
    VAR(Fr_SlotModeType, TYPEDEF) SlotMode;         /**<  CC - slot mode */
    VAR(Fr_StartupStateType, TYPEDEF) StartupState; /**<  CC - startup state */
    VAR(Fr_POCStateType, TYPEDEF) State;            /**<  CC - POC state */
    VAR(Fr_WakeupStatusType, TYPEDEF) WakeupStatus; /**<  CC - wakeup state */
    VAR(boolean, TYPEDEF) CHIHaltRequest;           /**<  CC - CHI Halt request bit */
    VAR(boolean, TYPEDEF) ColdstartNoise;           /**<  CC - Coldstart noise bit */
    VAR(boolean, TYPEDEF) Freeze;                   /**<  CC - Freeze bit */
    VAR(boolean, TYPEDEF) CHIReadyRequest;          /**<  CC - CHI Ready request bit */
} Fr_POCStatusType;

/** \brief This enumerator type represents the LSdu tx status. */
/* !LINKSTO Base.ASR403.FR511,1,Base.ASR430.SWS_Fr_00511,1 */
typedef enum
{
    FR_TRANSMITTED = 0,     /**< LSdu was transmitted */
    FR_NOT_TRANSMITTED,     /**< LSdu was not transmitted */
    FR_TRANSMITTED_CONFLICT /**< A transmission conflict has occurred. */
} Fr_TxLPduStatusType;

/** \brief This enumerator type represents the LSdu rx status. */
/* !LINKSTO Base.ASR403.FR512,1 */
typedef enum
{
    FR_RECEIVED = 0,                  /**< LSdu was received */
    FR_NOT_RECEIVED,                  /**< LSdu was not received */
    FR_RECEIVED_MORE_DATA_AVAILABLE   /**< LPdu has been received. More instances of
                                       *   this LPdu are available (FIFO usage). */
} Fr_RxLPduStatusType;

/** \brief This enumerator type represents the FlexRay channels. */
/* !LINKSTO Base.ASR403.FR514,1 */
typedef enum
{
    FR_CHANNEL_A = 0,     /**< FlexRay channel A */
    FR_CHANNEL_B,         /**< FlexRay channel B */
    FR_CHANNEL_AB         /**< FlexRay channel A and B */
} Fr_ChannelType;

/** \brief This enumerator type represents the FlexRay transceiver driver
 * modes. */
typedef enum
{
    FRTRCV_TRCVMODE_NORMAL = 0,  /**< FrTrcv mode "Normal" */
    FRTRCV_TRCVMODE_STANDBY,     /**< FrTrcv mode "Standby" */
    FRTRCV_TRCVMODE_SLEEP,       /**< FrTrcv mode "Sleep" */
    FRTRCV_TRCVMODE_RECEIVEONLY  /**< FrTrcv mode "Receive Only" */
} FrTrcv_TrcvModeType;

/** \brief This enumerator type represents the FlexRay transceiver
 * wakeup-reasons. */
typedef enum
{
    FRTRCV_WU_NOT_SUPPORTED = 0,  /**< FrTrcv wakeup not supported */
    FRTRCV_WU_BY_BUS,             /**< FrTrcv wakeup by bus */
    FRTRCV_WU_BY_PIN,             /**< FrTrcv wakeup by pin */
    FRTRCV_WU_INTERNALLY,         /**< FrTrcv internally wakeup */
    FRTRCV_WU_RESET,              /**< FrTrcv reset wakeup */
    FRTRCV_WU_POWER_ON            /**< FrTrcv power on wakeup */
} FrTrcv_TrcvWUReasonType;

/** \brief This structure contains information about the assignment of a
 * FlexRay frame to a cycle and a slot ID. */
typedef struct
{
   uint8 Cycle;
   uint16 SlotId;
   Fr_ChannelType channelId;
} Fr_SlotAssignmentType;

/* EB-specific type definitions for backward compatibility with Autosar 3.1 Fr modules */

/** \brief This enumerator type is used for the FlexRay controller
 * synchronization state. */
typedef enum
{
    FR_ASYNC = 0,   /**< FlexRay controller is not synchronized to any cluster */
    FR_SYNC         /**< FlexRay controller is synchronized to some cluster */
} Fr_SyncStateType;

/** \brief This enumerator type contains the possible offset correction
 * operations. */
typedef enum
{
    FR_OFFSET_INC = 0,  /**< apply an incrementing offset correction */
    FR_OFFSET_DEC,      /**< apply an decrementing offset correction */
    FR_OFFSET_NOCHANGE  /**< don't apply any offset correction       */
} Fr_OffsetCorrectionType;

/** \brief This enumerator type contains the possible rate correction
 * operations. */
typedef enum
{
    FR_RATE_INC = 0,    /**< apply an incrementing rate correction */
    FR_RATE_DEC,        /**< apply an decrementing rate correction */
    FR_RATE_NOCHANGE    /**< don't apply any rate correction       */
} Fr_RateCorrectionType;

/** \brief This enumerator type represents the FlexRay MTS status. */
typedef enum
{
    FR_MTS_RCV = 0,               /**< MTS received */
    FR_MTS_RCV_SYNERR,            /**< MTS received and syntax error detected */
    FR_MTS_RCV_BVIO,              /**< MTS received and boundary violation detected */
    FR_MTS_RCV_SYNERR_BVIO,       /**< MTS received and syntax error and boundary violation
                                   *   detected */
    FR_MTS_NOT_RCV,               /**< no MTS received */
    FR_MTS_NOT_RCV_SYNERR,        /**< no MTS received and syntax error detected */
    FR_MTS_NOT_RCV_BVIO,          /**< no MTS received and boundary violation detected */
    FR_MTS_NOT_RCV_SYNERR_BVIO    /**< no MTS received and syntax error and boundary violation
                                   *   detected */
} Fr_MTSStatusType;

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
#endif /* FR_GENERAL_TYPES */
