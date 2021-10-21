/**
 * \file
 *
 * \brief AUTOSAR WdgM
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgM.
 *
 * \version 6.1.39
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef WDGM_BSW_LCFG_H
#define WDGM_BSW_LCFG_H
/*==================[inclusions]=================================================================*/
/* !LINKSTO WDGM.EB.Design.IncludeFileStructure,1 */
#include <WdgM_Int_Types.h>                                               /* Internal Data Types */
#include <WdgIf.h>                                                             /* WdgIf_ModeType */

/*==================[macros]=====================================================================*/
#if (defined WDGM_EB_DUMMY_SEID)
#error WDGM_EB_DUMMY_SEID is already defined
#endif

/** \brief dummy SEID in case some SE/CP has no predecessor */
#define WDGM_EB_DUMMY_SEID 255U

#if (defined WDGM_EB_DUMMY_CPID)
#error WDGM_EB_DUMMY_CPID is already defined
#endif

/** \brief dummy CPID in case some SE/CP has no predecessor */
#define WDGM_EB_DUMMY_CPID 255U

#if (defined WDGM_EB_DUMMY_FAILEDREFCYCLETOL)
#error WDGM_EB_DUMMY_FAILEDREFCYCLETOL is already defined
#endif

/** \brief dummy failed reference cycle tolerance in case a SE is not configured in some mode */
#define WDGM_EB_DUMMY_FAILEDREFCYCLETOL 0U

/*==================[type definitions]===========================================================*/

/*------------------[basic type definitions]-----------------------------------------------------*/


/** \brief result type of a checkpoint */
typedef uint8 WdgM_EB_ResultType;

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
/** \brief identifier of a logical supervision graph */
typedef uint16 WdgM_EB_GraphIdType;
#endif

#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
/** \brief identifier of a deadline supervision entity */
typedef uint16 WdgM_EB_DMIdType;
#endif

/** \brief identifier of a supervised entity
 *
 * This is a deviation from AUTOSAR and is fixed to uint8 in order to use atomic assignments */
typedef uint8 WdgM_EB_SEIdType;

/** \brief identifier of a checkpoint
 *
 * This is a deviation from AUTOSAR and is fixed to uint8 in order to use atomic assignments */
typedef uint8 WdgM_EB_CPIdType;

/** \brief alive counter data */
typedef uint16 WdgM_EB_AliveCounterType;

/** \brief the value of a trigger condition */
typedef uint16 WdgM_EB_TriggerConditionType;

/** \brief the id of a trigger */
typedef uint8 WdgM_EB_TriggerIdType;

/** \brief the id of a watchdog */
typedef uint8 WdgM_EB_WatchdogIdType;

#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
/** \brief a timestamp for deadline monitoring */
typedef uint32 WdgM_EB_TimestampType;
#endif

/** \brief holding a checkpoint */
typedef struct
{
  WdgM_EB_SEIdType SEId;
  WdgM_EB_CPIdType CPId;
} WdgM_EB_CPType;

/*------------------[data type definitions]------------------------------------------------------*/

#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable4,1 */
/** \brief data for a deadline supervision graph */
typedef struct
{
  WdgM_EB_TimestampType StartTimestampData;
  /**< time-stamp when the start Checkpoint was called */
  WdgM_EB_TimestampType StartTimestampDataInv;
  /**< bit-inverse time-stamp when the start Checkpoint was called */
  uint32 MaxCyclesData;
  /**<  maximum allowed main function cycles until deadline violation */
  uint16 IsActiveData;
  /**< active flag of the Deadline Monitoring including bit-inverse data */
} WdgM_EB_DMDataType;
#endif

/** \brief runtime data of a CP */
typedef struct
{
  /* !LINKSTO WDGM.EB.DesignDecision.InternalVariable3,1 */
  uint32 AliveCounterData; /**< alive counter and bit-inverse of it */
#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
  /* !LINKSTO WDGM.EB.DesignDecision.InternalVariable5,1 */
  uint16 DeadlineData; /**< deadline result and bit-inverse of it */
#endif
#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
  /* !LINKSTO WDGM.EB.DesignDecision.InternalVariable8,1 */
  uint16 LogicalData; /**< logical result and bit-inverse of it */
#endif
} WdgM_EB_CPDataType;

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
#if (WDGM_EB_MULTICORE_ENABLED == STD_OFF)
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable6,2,
            WDGM.EB.DesignDecision.InternalVariable7,2 */
/** \brief data for a logical supervision graph */
typedef struct
{
  uint32 LastCPData; /**< last checkpoint including bit-inverse data */
  /* !LINKSTO WDGM.EB.DesignDecision.InternalVariable7,2 */
  uint16 IsActiveData; /**< active flag of the graph including bit-inverse data */
} WdgM_EB_GraphDataType;
#else
typedef struct
{
  Atomic_t LastCPData; /**< last checkpoint including bit-inverse data */
  /* !LINKSTO WDGM.EB.DesignDecision.InternalVariable7,2 */
  Atomic_t IsActiveData; /**< active flag of the graph including bit-inverse data */
} WdgM_EB_GraphDataType;
#endif
#endif

#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
#if (WDGM_EB_MULTICORE_ENABLED == STD_OFF)
/** \brief the sequence counter for all CP in one graph
 *
 * Holding the counter to determine concurrent call of ChecpointReached function including bit-inverse data */
typedef uint16 WdgM_EB_ExtGraphDataSeqArrayType;
#else
typedef Atomic_t WdgM_EB_ExtGraphDataSeqArrayType;
#endif
#endif

/*------------------[configuration type definitions]---------------------------------------------*/

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
/** \brief configuration data for logical supervision of a CP */
typedef struct
{
  P2CONST(WdgM_EB_CPType,TYPEDEF,WDGM_CONST) PredCPs;
  /**< pointer to an array of allowed predecessor CPs. NULL_PTR if no predecessors are allowed. */
  WdgM_EB_GraphIdType GraphId; /**< id of the graph the CP belongs to */
  uint8 SESequenceIdx; /**< index in the Sequence Graph Array for the SE which contains this CP.
  All CPs, belonging to the graph, which are part of the same SE have the same index in the Array. */
  uint8 NumOfPredCPs; /**< number of allowed predecessor CPs. If 0, then this is an initial CP. */
  boolean IsFinal; /**< flag for being a final CP */
} WdgM_EB_CPLogicalCfgType;
#endif

/** \brief configuration data for alive supervision of a CP */
typedef struct
{
  uint16 ExpectedAliveIndication; /**< amount of expected alive indications */
  uint16 SupervisionReferenceCycle;
  /**< amount of supervision cycles needed until checking is performed */

  uint8 MaxMargin; /**< maximum number of additional indications */
  uint8 MinMargin; /**< maximum number of fewer indications */
} WdgM_EB_CPAliveCfgType;

#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
/** \brief configuration data for deadline monitoring of a CP */
typedef struct WdgM_EB_CPDeadlineCfgStructType
{
  WdgM_EB_TimestampType WdgMDeadlineMax; /**< maximum Deadline time */
  WdgM_EB_TimestampType WdgMDeadlineMin; /**< minimum Deadline time */
  uint16 DMMaxMainfunctionCycles; /**< maximum main function calls until deadline violation */
  P2CONST(struct WdgM_EB_CPDeadlineCfgStructType,TYPEDEF,WDGM_CONST) PrevCPDeadlineCfg;
  /**< pointer to deadline configuration in case a start checkpoint is a stop checkpoint
   * of a previous deadline supervision configuration */
  WdgM_EB_DMIdType DMId;
  /**< id of the Deadline Monitoring this CP belongs to in case of a Start CP,
   * in case of a Stop CP, this variable is retrieved via \a PrevCPDeadlineCfg. */
  WdgM_EB_CPIdType StartCPId; /**< Internal Checkpoint id of start checkpoint */
  WdgM_EB_CPIdType StopCPId; /**< Internal Checkpoint id of stop checkpoint */
} WdgM_EB_CPDeadlineCfgType;
#endif

/** \brief configuration data of a CP */
typedef struct
{
  P2CONST(WdgM_EB_CPAliveCfgType,TYPEDEF,WDGM_CONST) AliveCfgPtr[WDGM_EB_MODE_NUM];
  /**< array of pointers to alive supervision configuration */
#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
  P2CONST(WdgM_EB_CPDeadlineCfgType,TYPEDEF,WDGM_CONST) DeadlineCfgPtr[WDGM_EB_MODE_NUM];
  /**< array of pointers to deadline monitoring configuration */
#endif
#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
  P2CONST(WdgM_EB_CPLogicalCfgType,TYPEDEF,WDGM_CONST) IntLogicalCfgPtr[WDGM_EB_MODE_NUM];
  /**< array of pointers to internal logical supervision configuration */
  P2CONST(WdgM_EB_CPLogicalCfgType,TYPEDEF,WDGM_CONST) ExtLogicalCfgPtr[WDGM_EB_MODE_NUM];
  /**< array of pointers to external logical supervision configuration */
#endif
} WdgM_EB_CPCfgType;

/** \brief configuration data of a SE */
typedef struct
{
  P2CONST(WdgM_EB_CPCfgType,TYPEDEF,WDGM_CONST) CPCfg;
  /**< pointer to an array of length NumOfCPs of CP configurations */
  P2VAR(WdgM_EB_CPDataType,TYPEDEF,WDGM_VAR) CPData;
  /**< pointer to an array of length NumOfCPs of CP runtime data */
  WdgM_EB_SEIdType SEId;
  /**< identifier of the external SE */
  WdgM_EB_CPIdType NumOfCPs;
  /**< number of CPs in this SE */
  boolean ErrorRecoveryEnabled;
  /**< True, if Supervised Entity shall never enter state Expired for sake of error recovery */
  uint8 FailedAliveSupervisionRefCycleTol[WDGM_EB_MODE_NUM];
  /**< amount of reference cycles that are tolerable to fail in the given mode */
#if (WDGM_EB_MULTICORE_ENABLED == STD_ON)
  WdgM_EB_CoreIdType CoreID;
  /**< Core ID where the SE is executed */
#endif
} WdgM_EB_SECfgType;

/** \brief configuration data for a watchdog trigger */
typedef struct
{
  WdgIf_ModeType WatchdogMode; /**< watchdog mode to use for triggering */
  WdgM_EB_TriggerConditionType TriggerCondition; /**< the value that is used for triggering */
  WdgM_EB_WatchdogIdType WatchdogId; /**< the id of the watchdog to trigger */
} WdgM_EB_TriggerCfgType;

/** \brief configuration data for each mode */
typedef struct
{
  uint32 MaxSupervisionCycleCounter;
  /**< least common multiple of all reference cycle values */
  P2CONST(WdgM_EB_TriggerCfgType, TYPEDEF, WDGM_CONST) TriggerCfg;
  /**< pointer to an array of trigger configurations */
  uint16 ExpiredSupervisionCycleTol;
  /**< amount of supervision cycles to wait in state EXPIRED */
  WdgM_EB_TriggerIdType NumTriggers;
  /**< number of triggers configured for this mode */
} WdgM_EB_ModeCfgType;

/** \brief internal runtime data of a CP (only accessed from MainFunction) */
typedef struct
{
  WdgM_EB_AliveCounterType AliveCounterData; /**< alive counter */
} WdgM_EB_CPInternalDataType;

#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
/** \brief Externals Graph run-time data
 *
 * This type is used to represent the pointer to the run-time data for each
 * external graph to store the sequence number of checkpoint for each
 * supervised entity and the number of SE of the runtime-data
 */
typedef struct
{
  P2VAR(WdgM_EB_ExtGraphDataSeqArrayType, TYPEDEF, WDGM_VAR) GraphDataPtr; /**< pointer to an array of length NumOfSEperGraphId
  which contains the greatest sequence number for each SE in the external graph */
  uint8 NumOfSEperGraphId; /**< length of the array pointed above */
} WdgM_EB_ExtGraphDataSeqType;
#endif

/** \brief core instance data */
typedef struct
{
#if(WDGM_EB_MULTICORE_ENABLED == STD_ON)
  CONSTP2VAR(Atomic_t, AUTOMATIC, AUTOMATIC) WdgM_EB_SELocalStatus;
  /**< local statuses of all SE */
#else
  CONSTP2VAR(WdgM_EB_LocalStatusType, AUTOMATIC, AUTOMATIC) WdgM_EB_SELocalStatus;
  /**< local statuses of all SE */
#endif
  CONSTP2VAR(uint16, AUTOMATIC, AUTOMATIC) WdgM_EB_FailedAliveSupervisionRefCycleCnt;
  /**< failed reference cycle counter for the SE */
  CONSTP2VAR(WdgM_EB_CPInternalDataType, AUTOMATIC, AUTOMATIC) WdgM_EB_IntRAMData;
  /**< Internal RAM data holding a copy of the alive counter values for each checkpoint */
  VAR(uint32, WDGM_VAR) WdgM_EB_SupervisionCycleCnt;   /* !LINKSTO WDGM.ASR40.WDGM085.2,1 */
  /**< WdgM internal counter for supervision cycles */
  WdgM_EB_ModeType WdgM_EB_CurrentModeId;
  /**< WdgM_EB_CurrentModeId represents current mode id set by WdgM_SetMode */
  boolean WdgM_EB_MainFunctionActive;
  /**< WdgM internal flag for indicating an active MainFunction execution */
#if (WDGM_EB_CHECK_MAINFUNCTION_TIMING == STD_ON)
  VAR(WdgM_EB_TimestampType, WDGM_VAR) WdgM_EB_MainFunctionTimeStamp;
  /**< TimeStamp of the last WdgM MainFunction call */
#endif
  CONST(WdgM_EB_CoreIdType, WDGM_VAR) CoreID;
  /**< Core ID where the CoreInstance belongs to */
}WdgM_EB_CoreContextType;

/*==================[external function declarations]=============================================*/

/*==================[external constants]=========================================================*/

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_CONST_8BIT
#else
#define WDGM_START_SEC_CONST_ASIL_D_8
#endif
#include <WdgM_MemMap.h>

/** \brief configured initial mode id */
extern CONST(WdgM_ModeType,WDGM_CONST) WdgM_EB_InitialModeId;

#if (WDGM_EB_DEINIT_MODE_ENABLED == STD_ON)
/** \brief configured de-init mode id */
extern CONST(WdgM_ModeType,WDGM_CONST) WdgM_EB_DeInitModeId;
#endif

/** \brief array size of configuration data for a WdgMMode */
extern CONST(WdgM_ModeType,WDGM_CONST) WdgM_EB_ModeCfgSize;

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_CONST_8BIT
#else
#define WDGM_STOP_SEC_CONST_ASIL_D_8
#endif
#include <WdgM_MemMap.h>

#define WDGM_START_SEC_CONST_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>

/** \brief array of configuration data for a SE */
extern CONST(WdgM_EB_SECfgType,WDGM_CONST) WdgM_EB_SECfg[WDGM_EB_SE_NUM];

/** \brief array of configuration data for a WdgMMode */
extern CONST(WdgM_EB_ModeCfgType,WDGM_CONST) WdgM_EB_ModeCfg[WDGM_EB_MODE_NUM];

#define WDGM_STOP_SEC_CONST_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_CONST_16BIT
#else
#define WDGM_START_SEC_CONST_ASIL_D_16
#endif
#include <WdgM_MemMap.h>

/** \brief array of mapping SEId to SE internal ID */
extern CONST(WdgM_EB_SEIdType, WDGM_CONST) WdgM_EB_SEIntSEId[WDGM_EB_SE_NUM];

#if (WDGM_EB_CALLERIDS_ENABLED == STD_ON)
/** \brief List of configured caller IDs */
extern CONST(uint16, WDGM_CONST) WdgM_EB_CallerIds[WDGM_EB_CALLERIDS_NUM];
#endif

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_CONST_16BIT
#else
#define WDGM_STOP_SEC_CONST_ASIL_D_16
#endif
#include <WdgM_MemMap.h>

/*==================[external data]==============================================================*/

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_PARTITIONING_ENABLED == STD_ON)
#define WDGM_START_SEC_SHARED_VAR_CLEARED_UNSPECIFIED
#else
#define WDGM_START_SEC_VAR_CLEARED_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>


/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable4,1 */
#if (WDGM_EB_DEADLINE_SUPERVISION_ENABLED == STD_ON)
#if (WDGM_EB_DM_NUM > 0)
extern VAR(WdgM_EB_DMDataType,WDGM_VAR) WdgM_EB_DMData[WDGM_EB_DM_NUM];
#else
extern VAR(WdgM_EB_DMDataType,WDGM_VAR) WdgM_EB_DMData[1];
#endif
#endif

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_PARTITIONING_ENABLED == STD_ON)
#define WDGM_STOP_SEC_SHARED_VAR_CLEARED_UNSPECIFIED
#else
#define WDGM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#endif /* ifndef WDGM_BSW_LCFG_H */
/*==================[end of file]================================================================*/
