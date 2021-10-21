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

[!INCLUDE "WdgM_Cfg.m"!][!//
[!CODE!][!//
#ifndef WDGM_LCFG_H
#define WDGM_LCFG_H

/*==================[inclusions]=================================================================*/
/* !LINKSTO WDGM.EB.Design.IncludeFileStructure,1 */
#include <WdgM_Cfg.h>
#include <WdgM_BSW_Lcfg.h>

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[external data]==============================================================*/

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_PARTITIONING_ENABLED == STD_ON)
#define WDGM_START_SEC_SHARED_VAR_CLEARED_UNSPECIFIED
#else
#define WDGM_START_SEC_VAR_CLEARED_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable3,1 */
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable5,1 */
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable8,1 */
[!LOOP "WdgMGeneral/WdgMSupervisedEntity/*"!][!//
/* Checkpoint individual run-time data for each SE */
extern VAR(WdgM_EB_CPDataType,WDGM_VAR) WdgM_EB_CPData_[!"@name"!][[!"num:i(count(WdgMCheckpoint/*))"!]];
[!ENDLOOP!][!//

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_PARTITIONING_ENABLED == STD_ON)
#define WDGM_STOP_SEC_SHARED_VAR_CLEARED_UNSPECIFIED
#else
#define WDGM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>
[!IF "$WdgMMulticoreEnabled"!]

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_32BIT
#else
#define WDGM_START_SEC_VAR_INIT_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

/* For multicore this variable is also used by the master to announce the satellites of the
 * result of the WdgMGetExpectedInitStateCallout(), so this is not under the
 * WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_OFF compiler switch. */
/** \brief Expected initialization state of the WdgM module. */
extern VAR(Atomic_t, WDGM_VAR) WdgM_EB_ExpectedInitState;

/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable2,1 */
/** \brief WdgM global supervision status */
extern VAR(Atomic_t, WDGM_VAR) WdgM_EB_GlobalStatus;

/** \brief WdgM global initialization status */
extern VAR(WdgM_EB_GlobalInitStatusType, WDGM_VAR) WdgM_EB_GlobalInitStatus;

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_32BIT
#else
#define WDGM_STOP_SEC_VAR_INIT_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_CONST_UNSPECIFIED
#else
#define WDGM_START_SEC_CONST_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
/** \brief array of pointers to the run-time data for each external graph and length of the pointed arrays */
extern CONST(WdgM_EB_ExtGraphDataSeqType,WDGM_CONST) WdgM_EB_ExtGraphDataSeqPtr[WDGM_EB_EXTERNAL_GRAPH_NUM];
#endif

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_CONST_UNSPECIFIED
#else
#define WDGM_STOP_SEC_CONST_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_CLEARED_32BIT
#else
#define WDGM_START_SEC_VAR_CLEARED_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

/** \brief through this variable, the WdgM master instance announces the WdgM satellite instances that the mode switch has been performed.
  * The WdgM satellite instances read the value of this variable to change and update their mode. */
extern VAR(Atomic_t, WDGM_VAR) WdgM_EB_CoreSwitchMode[WDGM_EB_NUMBER_OF_CORES];

/* !LINKSTO WDGM.ASR40.WDGM200,1 */
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable1,2 */
/** \brief defines the local statuses of all SE for the WdgM core instances */
extern VAR(Atomic_t, WDGM_VAR) WdgM_EB_SELocalStatus[WDGM_EB_SE_NUM];

/** \brief has the expired SEId been set? */
extern VAR(Atomic_t, WDGM_VAR) WdgM_EB_IsExpiredSEIdSet;

#if (WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED == STD_OFF)
extern VAR(Atomic_t, WDGM_VAR) WdgM_EB_ExpectedWdgMModeId;
#endif

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_CLEARED_32BIT
#else
#define WDGM_STOP_SEC_VAR_CLEARED_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_PARTITIONING_ENABLED == STD_ON)
#define WDGM_START_SEC_SHARED_VAR_CLEARED_GLOBAL_UNSPECIFIED
#elif (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_CLEARED_UNSPECIFIED
#else
#define WDGM_START_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable12,1,WDGM.EB.DesignDecision.InternalVariable13,1 */
extern VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_ExternalGraphData[WDGM_EB_EXTERNAL_GRAPH_NUM];
#else
extern VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_ExternalGraphData[1];
#endif

#if (WDGM_EB_INTERNAL_GRAPH_NUM > 0)
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable6,2,WDGM.EB.DesignDecision.InternalVariable7,2 */
extern VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_InternalGraphData[WDGM_EB_INTERNAL_GRAPH_NUM];
#else
extern VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_InternalGraphData[1];
#endif


/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_PARTITIONING_ENABLED == STD_ON)
#define WDGM_STOP_SEC_SHARED_VAR_CLEARED_GLOBAL_UNSPECIFIED
#elif (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#else
#define WDGM_STOP_SEC_VAR_CLEARED_GLOBAL_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#endif

#define WDGM_START_SEC_VAR_POWER_ON_CLEARED_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>

/** \brief the Supervised Entity Id which first expired */
extern VAR(Atomic_t, WDGM_VAR_POWER_ON_INIT) WdgM_EB_ExpiredSEID;

#define WDGM_STOP_SEC_VAR_POWER_ON_CLEARED_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>

[!/*
*/!][!VAR "SupEntExistPerCore" = "false()"!][!/*
  */!][!IF "node:exists(as:modconf('WdgM')[1]/WdgMGeneral/WdgMGeneralMulticore/WdgMNumberOfCores) and (as:modconf('WdgM')[1]/WdgMGeneral/WdgMGeneralMulticore/WdgMNumberOfCores > 1)"!][!/*
      */!][!VAR "WDGM_MASTER_CORE_ID" = "node:value(as:modconf('WdgM')[1]/WdgMGeneral/WdgMGeneralMulticore/WdgMMasterCoreId)"!][!/*
  */!][!ELSE!][!/*
      */!][!VAR "WDGM_MASTER_CORE_ID" = "'0'"!][!/*
  */!][!ENDIF!][!/*
  */!][!FOR "index_i" = "0" TO "num:i($WdgMNumberOfCores)-1"!][!/*
    */!][!LOOP "as:modconf('WdgM')[1]/WdgMGeneral/WdgMSupervisedEntity/*"!][!/*
      */!][!IF "num:i(node:value(WdgMSupervisedEntityCoreId)) = num:i($index_i)"!][!/*
        */!][!VAR "SupEntExistPerCore" = "true()"!][!/*
       */!][!ENDIF!][!/*
    */!][!ENDLOOP!][!/*
    */!][!IF "$SupEntExistPerCore"!][!//

[!IF "$index_i != $WDGM_MASTER_CORE_ID"!]
#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_1_CORE_[!"num:i($index_i)"!]_START_SEC_VAR_CLEARED_16BIT
#else
#define WDGM_1_CORE_[!"num:i($index_i)"!]_START_SEC_VAR_CLEARED_ASIL_D_16
#endif
#include <WdgM_1_CORE_[!"num:i($index_i)"!]_MemMap.h>
[!ELSE!][!//
#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_CLEARED_16BIT
#else
#define WDGM_START_SEC_VAR_CLEARED_ASIL_D_16
#endif
#include <WdgM_MemMap.h>
[!ENDIF!][!/* IF "$index_i != $WDGM_MASTER_CORE_ID" */!][!//

[!IF "$index_i != $WDGM_MASTER_CORE_ID"!]
#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_1_CORE_[!"num:i($index_i)"!]_STOP_SEC_VAR_CLEARED_16BIT
#else
#define WDGM_1_CORE_[!"num:i($index_i)"!]_STOP_SEC_VAR_CLEARED_ASIL_D_16
#endif
#include <WdgM_1_CORE_[!"num:i($index_i)"!]_MemMap.h>

[!ELSE!][!//
#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_CLEARED_16BIT
#else
#define WDGM_STOP_SEC_VAR_CLEARED_ASIL_D_16
#endif
#include <WdgM_MemMap.h>
[!ENDIF!][!/* IF "$index_i != $WDGM_MASTER_CORE_ID" */!][!/*
    */!][!ENDIF!][!/* IF $SupEntExistPerCore */!][!/*
    */!][!VAR "SupEntExistPerCore" = "false()"!][!//
  [!ENDFOR!][!//
[!ELSE!][!/* IF $WdgMMulticoreEnabled */!][!//

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_8BIT
#else
#define WDGM_START_SEC_VAR_INIT_ASIL_D_8
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_EXT_GET_EXPECTED_INIT_STATE_ENABLED == STD_OFF)
/** \brief Expected initialization state of the WdgM module */
extern VAR(WdgM_EB_InitStatusType, WDGM_VAR) WdgM_EB_ExpectedInitState;
#endif

/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable2,1 */
/** \brief WdgM global supervision status */
extern VAR(WdgM_GlobalStatusType, WDGM_VAR) WdgM_EB_GlobalStatus;

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_8BIT
#else
#define WDGM_STOP_SEC_VAR_INIT_ASIL_D_8
#endif
#include <WdgM_MemMap.h>

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_CONST_UNSPECIFIED
#else
#define WDGM_START_SEC_CONST_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
/** \brief array of pointers to the run-time data for each external graph and length of the pointed arrays */
extern CONST(WdgM_EB_ExtGraphDataSeqType,WDGM_CONST) WdgM_EB_ExtGraphDataSeqPtr[WDGM_EB_EXTERNAL_GRAPH_NUM];
#endif

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_CONST_UNSPECIFIED
#else
#define WDGM_STOP_SEC_CONST_ASIL_D_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_CLEARED_8BIT
#else
#define WDGM_START_SEC_VAR_CLEARED_ASIL_D_8
#endif
#include <WdgM_MemMap.h>

/* !LINKSTO WDGM.ASR40.WDGM200,1 */
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable1,2 */
/** \brief defines the local statuses of all SE*/
extern VAR(WdgM_LocalStatusType, WDGM_VAR) WdgM_EB_SELocalStatus[WDGM_EB_SE_NUM];

/** \brief has the expired SEId been set? */
extern VAR(boolean, WDGM_VAR) WdgM_EB_IsExpiredSEIdSet;

#if (WDGM_EB_EXT_GET_EXPECTED_WDGM_MODE_ENABLED == STD_OFF)
extern VAR(WdgM_EB_ModeType, WDGM_VAR) WdgM_EB_ExpectedWdgMModeId;
#endif

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_CLEARED_8BIT
#else
#define WDGM_STOP_SEC_VAR_CLEARED_ASIL_D_8
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_CLEARED_16BIT
#else
#define WDGM_START_SEC_VAR_CLEARED_ASIL_D_16
#endif
#include <WdgM_MemMap.h>

/** \brief Internal RAM data holding a copy of the alive counter values for each checkpoint */
extern VAR(WdgM_EB_CPInternalDataType, WDGM_VAR) WdgM_EB_IntRAMData[WDGM_EB_CHECKPOINTS_NUM];

/** \brief failed reference cycle counter for the SE  */
extern VAR(uint16, WDGM_VAR) WdgM_EB_FailedAliveSupervisionRefCycleCnt[WDGM_EB_SE_NUM];

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_CLEARED_16BIT
#else
#define WDGM_STOP_SEC_VAR_CLEARED_ASIL_D_16
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_LOGICAL_SUPERVISION_ENABLED == STD_ON)
/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_PARTITIONING_ENABLED == STD_ON)
#define WDGM_START_SEC_SHARED_VAR_CLEARED_UNSPECIFIED
#else
#define WDGM_START_SEC_VAR_CLEARED_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#if (WDGM_EB_EXTERNAL_GRAPH_NUM > 0U)
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable12,1,WDGM.EB.DesignDecision.InternalVariable13,1 */
extern VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_ExternalGraphData[WDGM_EB_EXTERNAL_GRAPH_NUM];
#else
extern VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_ExternalGraphData[1];
#endif

#if (WDGM_EB_INTERNAL_GRAPH_NUM > 0U)
/* !LINKSTO WDGM.EB.DesignDecision.InternalVariable6,2,WDGM.EB.DesignDecision.InternalVariable7,2 */
extern VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_InternalGraphData[WDGM_EB_INTERNAL_GRAPH_NUM];
#else
extern VAR(WdgM_EB_GraphDataType,WDGM_VAR) WdgM_EB_InternalGraphData[1];
#endif

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020304,1 */
#if (WDGM_EB_PARTITIONING_ENABLED == STD_ON)
#define WDGM_STOP_SEC_SHARED_VAR_CLEARED_UNSPECIFIED
#else
#define WDGM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#endif
#include <WdgM_MemMap.h>

#endif

#define WDGM_START_SEC_VAR_POWER_ON_INIT_ASIL_D_16
#include <WdgM_MemMap.h>

/** \brief the Supervised Entity Id which first expired */
extern VAR(WdgM_EB_ExpiredSEIDType, WDGM_VAR_POWER_ON_INIT) WdgM_EB_ExpiredSEID;

#define WDGM_STOP_SEC_VAR_POWER_ON_INIT_ASIL_D_16
#include <WdgM_MemMap.h>

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_START_SEC_VAR_32BIT
#else
#define WDGM_START_SEC_VAR_INIT_ASIL_D_32
#endif
#include <WdgM_MemMap.h>

/** \brief WdgM global initialization status */
extern VAR(WdgM_EB_GlobalInitStatusType, WDGM_VAR) WdgM_EB_GlobalInitStatus;

#if (WDGM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
#define WDGM_STOP_SEC_VAR_32BIT
#else
#define WDGM_STOP_SEC_VAR_INIT_ASIL_D_32
#endif
#include <WdgM_MemMap.h>

#define WDGM_START_SEC_VAR_INIT_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>

/** \brief The WdgM specific context data.
 ** This variable holds the specific run time data of the WdgM. */
extern VAR(WdgM_EB_CoreContextType, AUTOMATIC) WdgM_EB_Context;

#define WDGM_STOP_SEC_VAR_INIT_ASIL_D_UNSPECIFIED
#include <WdgM_MemMap.h>

[!ENDIF!][!/* IF $WdgMMulticoreEnabled */!][!//

/*==================[external function definitions]==============================================*/
[!IF "$WdgMMulticoreEnabled"!]
#define WDGM_START_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>

/** \brief Returns the core context data structure for the core which called the function.
 **/
extern FUNC(P2VAR(WdgM_EB_CoreContextType, WDGM_VAR, WDGM_APPL_DATA), WDGM_CODE) WdgM_GetCoreInstance(void);

#define WDGM_STOP_SEC_CODE_ASIL_D
#include <WdgM_MemMap.h>
[!ENDIF!]
#endif /* ifndef WDGM_LCFG_H */
/*==================[end of file]================================================================*/
[!ENDCODE!][!//
