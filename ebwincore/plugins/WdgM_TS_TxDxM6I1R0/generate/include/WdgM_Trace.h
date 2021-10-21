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
#ifndef WDGM_TRACE_H
#define WDGM_TRACE_H

/*==================[inclusions]============================================*/
[!IF "node:exists(as:modconf('Dbg'))"!]
/* !LINKSTO WDGM.EB.Design.IncludeFileStructure,1 */
/* !LINKSTO WDGM.EB.Dbg1,2 */
#include <Dbg.h>
[!ENDIF!]

/*==================[macros]================================================*/
[!IF "$WdgMMulticoreEnabled"!][!//
[!FOR "index_i" = "0" TO "num:i($WdgMNumberOfCores)-1"!][!//
[!VAR "SupEntExistPerCore" = "false()"!][!//
[!LOOP "as:modconf('WdgM')[1]/WdgMGeneral/WdgMSupervisedEntity/*"!][!//
    [!IF "num:i(node:value(WdgMSupervisedEntityCoreId)) = num:i($index_i)"!][!//
        [!VAR "SupEntExistPerCore" = "true()"!][!//
    [!ENDIF!][!//
[!ENDLOOP!][!//
[!IF "$SupEntExistPerCore"!]

#ifndef DBG_WDGM_ASR32_RTEINDIVIDUALMODESWITCH_CORE_[!"$index_i"!]_CORE_[!"$index_i"!]_ENTRY
/** \brief Entry point of function WdgM_ASR32_RteIndividualModeSwitch() */
#define DBG_WDGM_ASR32_RTEINDIVIDUALMODESWITCH_CORE_[!"$index_i"!]_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_ASR32_RTEINDIVIDUALMODESWITCH_CORE_[!"$index_i"!]_EXIT
/** \brief Exit point of function WdgM_ASR32_RteIndividualModeSwitch() */
#define DBG_WDGM_ASR32_RTEINDIVIDUALMODESWITCH_CORE_[!"$index_i"!]_EXIT(a,b)
#endif

#ifndef DBG_WDGM_ASR32_RTE_UPDATEALIVECOUNTER_CORE_[!"$index_i"!]_ENTRY
/** \brief Entry point of function WdgM_ASR32_Rte_UpdateAliveCounter() */
#define DBG_WDGM_ASR32_RTE_UPDATEALIVECOUNTER_CORE_[!"$index_i"!]_ENTRY(a)
#endif

#ifndef DBG_WDGM_ASR32_RTE_UPDATEALIVECOUNTER_CORE_[!"$index_i"!]_EXIT
/** \brief Exit point of function WdgM_ASR32_Rte_UpdateAliveCounter() */
#define DBG_WDGM_ASR32_RTE_UPDATEALIVECOUNTER_CORE_[!"$index_i"!]_EXIT(a,b)
#endif

#ifndef DBG_WDGM_ASR32_RTE_ACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_ENTRY
/** \brief Entry point of function WdgM_ASR32_Rte_ActivateAliveSupervision() */
#define DBG_WDGM_ASR32_RTE_ACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_ENTRY(a)
#endif

#ifndef DBG_WDGM_ASR32_RTE_ACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_EXIT
/** \brief Exit point of function WdgM_ASR32_Rte_ActivateAliveSupervision() */
#define DBG_WDGM_ASR32_RTE_ACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_EXIT(a,b)
#endif

#ifndef DBG_WDGM_ASR32_RTE_DEACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_ENTRY
/** \brief Entry point of function WdgM_ASR32_Rte_DeactivateAliveSupervision() */
#define DBG_WDGM_ASR32_RTE_DEACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_ENTRY(a)
#endif

#ifndef DBG_WDGM_ASR32_RTE_DEACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_EXIT
/** \brief Exit point of function WdgM_ASR32_Rte_DeactivateAliveSupervision() */
#define DBG_WDGM_ASR32_RTE_DEACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_EXIT(a,b)
#endif

#ifndef DBG_WDGM_ASR40_RTEINDIVIDUALMODESWITCH_CORE_[!"$index_i"!]_ENTRY
/** \brief Entry point of function WdgM_ASR40_RteIndividualModeSwitch() */
#define DBG_WDGM_ASR40_RTEINDIVIDUALMODESWITCH_CORE_[!"$index_i"!]_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_ASR40_RTEINDIVIDUALMODESWITCH_CORE_[!"$index_i"!]_EXIT
/** \brief Exit point of function WdgM_ASR40_RteIndividualModeSwitch() */
#define DBG_WDGM_ASR40_RTEINDIVIDUALMODESWITCH_CORE_[!"$index_i"!]_EXIT(a,b)
#endif

#ifndef DBG_WDGM_ASR40_RTE_UPDATEALIVECOUNTER_CORE_[!"$index_i"!]_ENTRY
/** \brief Entry point of function WdgM_ASR40_Rte_UpdateAliveCounter() */
#define DBG_WDGM_ASR40_RTE_UPDATEALIVECOUNTER_CORE_[!"$index_i"!]_ENTRY(a)
#endif

#ifndef DBG_WDGM_ASR40_RTE_UPDATEALIVECOUNTER_CORE_[!"$index_i"!]_EXIT
/** \brief Exit point of function WdgM_ASR40_Rte_UpdateAliveCounter() */
#define DBG_WDGM_ASR40_RTE_UPDATEALIVECOUNTER_CORE_[!"$index_i"!]_EXIT(a,b)
#endif

#ifndef DBG_WDGM_ASR40_RTE_CHECKPOINTREACHED_CORE_[!"$index_i"!]_ENTRY
/** \brief Entry point of function WdgM_ASR40_Rte_CheckpointReached() */
#define DBG_WDGM_ASR40_RTE_CHECKPOINTREACHED_CORE_[!"$index_i"!]_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_ASR40_RTE_CHECKPOINTREACHED_CORE_[!"$index_i"!]_EXIT
/** \brief Exit point of function WdgM_ASR40_Rte_CheckpointReached() */
#define DBG_WDGM_ASR40_RTE_CHECKPOINTREACHED_CORE_[!"$index_i"!]_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_RTE_UPDATEALIVECOUNTER_CORE_[!"$index_i"!]_ENTRY
/** \brief Entry point of function WdgM_Rte_UpdateAliveCounter() */
#define DBG_WDGM_RTE_UPDATEALIVECOUNTER_CORE_[!"$index_i"!]_ENTRY(a)
#endif

#ifndef DBG_WDGM_RTE_UPDATEALIVECOUNTER_CORE_[!"$index_i"!]_EXIT
/** \brief Exit point of function WdgM_Rte_UpdateAliveCounter() */
#define DBG_WDGM_RTE_UPDATEALIVECOUNTER_CORE_[!"$index_i"!]_EXIT(a,b)
#endif

#ifndef DBG_WDGM_RTE_ACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_ENTRY
/** \brief Entry point of function WdgM_Rte_ActivateAliveSupervision() */
#define DBG_WDGM_RTE_ACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_ENTRY(a)
#endif

#ifndef DBG_WDGM_RTE_ACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_EXIT
/** \brief Exit point of function WdgM_Rte_ActivateAliveSupervision() */
#define DBG_WDGM_RTE_ACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_EXIT(a,b)
#endif

#ifndef DBG_WDGM_RTE_DEACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_ENTRY
/** \brief Entry point of function WdgM_Rte_DeactivateAliveSupervision() */
#define DBG_WDGM_RTE_DEACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_ENTRY(a)
#endif

#ifndef DBG_WDGM_RTE_DEACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_EXIT
/** \brief Exit point of function WdgM_Rte_DeactivateAliveSupervision() */
#define DBG_WDGM_RTE_DEACTIVATEALIVESUPERVISION_CORE_[!"$index_i"!]_EXIT(a,b)
#endif

#ifndef DBG_WDGM_RTEINDIVIDUALMODESWITCH_CORE_[!"$index_i"!]_ENTRY
/** \brief Entry point of function WdgM_RteIndividualModeSwitch() */
#define DBG_WDGM_RTEINDIVIDUALMODESWITCH_CORE_[!"$index_i"!]_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_RTEINDIVIDUALMODESWITCH_CORE_[!"$index_i"!]_EXIT
/** \brief Exit point of function WdgM_RteIndividualModeSwitch() */
#define DBG_WDGM_RTEINDIVIDUALMODESWITCH_CORE_[!"$index_i"!]_EXIT(a,b)
#endif

#ifndef DBG_WDGM_RTE_CHECKPOINTREACHED_CORE_[!"$index_i"!]_ENTRY
/** \brief Entry point of function WdgM_Rte_CheckpointReached() */
#define DBG_WDGM_RTE_CHECKPOINTREACHED_CORE_[!"$index_i"!]_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_RTE_CHECKPOINTREACHED_CORE_[!"$index_i"!]_EXIT
/** \brief Exit point of function WdgM_Rte_CheckpointReached() */
#define DBG_WDGM_RTE_CHECKPOINTREACHED_CORE_[!"$index_i"!]_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_RTE_GETLOCALSTATUS_CORE_[!"$index_i"!]_ENTRY
/** \brief Entry point of function WdgM_Rte_GetLocalStatus() */
#define DBG_WDGM_RTE_GETLOCALSTATUS_CORE_[!"$index_i"!]_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_RTE_GETLOCALSTATUS_CORE_[!"$index_i"!]_EXIT
/** \brief Exit point of function WdgM_Rte_GetLocalStatus() */
#define DBG_WDGM_RTE_GETLOCALSTATUS_CORE_[!"$index_i"!]_EXIT(a,b,c)
#endif
[!ENDIF!][!//
[!ENDFOR!][!//
[!ENDIF!][!//
/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef WDGM_TRACE_H */
/*==================[end of file]===========================================*/
[!ENDCODE!][!//
