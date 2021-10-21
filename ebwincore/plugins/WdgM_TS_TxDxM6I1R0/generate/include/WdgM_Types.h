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
#ifndef WDGM_TYPES_H
#define WDGM_TYPES_H

/*==================[inclusions]=================================================================*/
/* !LINKSTO WDGM.EB.Design.IncludeFileStructure,1 */
#include <Std_Types.h>
[!IF "($WdgMMulticoreEnabled = 'true')"!][!//
[!IF "$GetCoreIdCalloutEnabled = 'false'"!][!//
/* !LINKSTO WDGM.EB.WdgMMulticoreInclude.2,1 */
#include <Os.h>                                                                    /* CoreIdType */
[!ENDIF!][!//
/* !LINKSTO WDGM.EB.WdgMMulticoreInclude.1,1 */
#include <Atomics.h>                                              /* Atomic_t and Atomic_value_t */
[!ENDIF!][!//
[!IF "$DefaultServiceAPI != 'NONE'"!][!//
[!IF "($WdgMMulticoreEnabled = 'true')"!][!//
[!VAR "MULTICORE_NUM_CORES" = "node:value(as:modconf('WdgM')[1]/WdgMGeneral/WdgMGeneralMulticore/WdgMNumberOfCores)"!][!//
[!FOR "Index" = "0" TO "$MULTICORE_NUM_CORES - 1"!][!//
#include <Rte_WdgM_CORE_[!"num:i($Index)"!]_Type.h>
[!ENDFOR!][!//
[!ELSE!][!//
#include <Rte_WdgM_Type.h>
[!ENDIF!][!//
[!ENDIF!][!//

/*==================[macros]=====================================================================*/

[!/* macros are only defined for the ASR43 service. Deactivate generation if AUTOSAR 4.3 is the default service */!][!//
[!IF "$DefaultServiceAPI != 'AUTOSAR_43'"!][!//
/*------------------[WdgM_LocalStatusType constants]---------------------------------------------*/
#if (defined WDGM_LOCAL_STATUS_OK)
#error WDGM_LOCAL_STATUS_OK already defined
#endif
/** \brief Alive / Deadline / Logical Supervision fulfilled.
 *
 * Timing constraints for Alive Supervision are fulfilled within the configured margins. */
#define WDGM_LOCAL_STATUS_OK               0U

#if (defined WDGM_LOCAL_STATUS_FAILED)
#error WDGM_LOCAL_STATUS_FAILED already defined
#endif
/** \brief Alive Supervision not fulfilled, but Deadline and Logical Supervision are fulfilled.
 *
 * Timing constraints for Alive Supervision have been violated including the margins, but the amount
 * of failed supervision reference cycles has not been exceeded. */
#define WDGM_LOCAL_STATUS_FAILED           1U

#if (defined WDGM_LOCAL_STATUS_EXPIRED)
#error WDGM_LOCAL_STATUS_EXPIRED already defined
#endif
/** \brief One of Alive, Deadline, or Logical Supervision is not fulfilled.
 *
 * Timing constraints for Alive Supervision have been violated including the margins for more often
 * than the acceptable amount of failed supervision reference cycles. */
#define WDGM_LOCAL_STATUS_EXPIRED          2U

#if (defined WDGM_LOCAL_STATUS_DEACTIVATED)
#error WDGM_LOCAL_STATUS_DEACTIVATED already defined
#endif
/** \brief Supervision deactivated
 *
 * Alive, Deadline, and Logical Supervision is disabled for this Supervised Entity. */
#define WDGM_LOCAL_STATUS_DEACTIVATED      4U

/*------------------[WdgM_GlobalStatusType constants]--------------------------------------------*/

#if (defined WDGM_GLOBAL_STATUS_OK)
#error WDGM_GLOBAL_STATUS_OK already defined
#endif
/** \brief Alive / Deadline / Logical Supervision of all active Supervised Entities fulfilled. */
#define WDGM_GLOBAL_STATUS_OK              0U

#if (defined WDGM_GLOBAL_STATUS_FAILED)
#error WDGM_GLOBAL_STATUS_FAILED already defined
#endif
/** \brief At least one Supervised Entity has Local Supervision Status WDGM_LOCAL_STATUS_FAILED. */
#define WDGM_GLOBAL_STATUS_FAILED          1U

#if (defined WDGM_GLOBAL_STATUS_EXPIRED)
#error WDGM_GLOBAL_STATUS_EXPIRED already defined
#endif
/** \brief At least one Supervised Entity has Local Supervision Status WDGM_LOCAL_STATUS_EXPIRED
 * and the time limit to postpone the blocking of watchdog triggering is not yet exceeded. */
#define WDGM_GLOBAL_STATUS_EXPIRED         2U

#if (defined WDGM_GLOBAL_STATUS_STOPPED)
#error WDGM_GLOBAL_STATUS_STOPPED already defined
#endif
/** \brief At least one Supervised Entity has Local Supervision Status WDGM_LOCAL_STATUS_EXPIRED
 * and the time limit to postpone the blocking of watchdog triggering is exceeded. */
#define WDGM_GLOBAL_STATUS_STOPPED         3U

#if (defined WDGM_GLOBAL_STATUS_DEACTIVATED)
#error WDGM_GLOBAL_STATUS_DEACTIVATED already defined
#endif
/** \brief Supervision deactivated. */
#define WDGM_GLOBAL_STATUS_DEACTIVATED     4U
[!ENDIF!][!//

/*------------------[WdgM_EB_InitStatusType constants]-------------------------------------------*/

#if (defined WDGM_EB_INIT_STATUS_INIT)
#error WDGM_EB_INIT_STATUS_INIT already defined
#endif
/** \brief WdgM is / shall be initialized */
#define WDGM_EB_INIT_STATUS_INIT              0U

#if (defined WDGM_EB_INIT_STATUS_DEINIT)
#error WDGM_EB_INIT_STATUS_DEINIT already defined
#endif
/** \brief WdgM is / shall be de-initialized */
#define WDGM_EB_INIT_STATUS_DEINIT              0xFFU
/*==================[type definitions]===========================================================*/
[!/* These types are only defined in the default service is NONE. */!][!//
/* !LINKSTO WDGM.EB.ASR32.WDGM117,2 */
[!IF "$DefaultServiceAPI = 'NONE'"!][!//

/** \brief Supervised Entity Id Type
 *
 * This type identifies an individual Supervised Entity for the Watchdog
 * Manager.
 */
typedef uint16 WdgM_SupervisedEntityIdType;
[!ENDIF!][!//

/* !LINKSTO WDGM.EB.ASR32.WDGM118,2 */
[!/* Deactivate type generation if AUTOSAR 4.0/4.3 is the default service */!][!//
[!IF "($DefaultServiceAPI = 'NONE') or ($DefaultServiceAPI = 'AUTOSAR_32')"!][!//
/** \brief Checkpoint Id Type
 *
 * This type identifies a Checkpoint in the context of a Supervised Entity for the Watchdog Manager.
 * Note that an individual Checkpoint can only be identified by the pair of Supervised Entity ID and Checkpoint ID.
 */
typedef uint16 WdgM_CheckpointIdType;
[!ENDIF!][!//
/* !LINKSTO WDGM.EB.ASR43.WDGM127,1 */
[!/* Deactivate type generation if AUTOSAR 4.3 is the default service */!][!//
[!IF "$DefaultServiceAPI != 'AUTOSAR_43'"!][!//

 /** \brief Mode Type
 *
 * This type distinguishes the different modes that were configured for the
 * Watchdog Manager. */
typedef uint8 WdgM_ModeType;

/** \brief Local Status Type
 *
 * This type is used to represent the local supervision status of the
 * individual supervised entities.
 */
typedef uint8 WdgM_LocalStatusType;

/** \brief Global Status Type
 *
 * This type is used to represent the global supervision status
 * of the Watchdog Manager. */
typedef uint8 WdgM_GlobalStatusType;
[!ENDIF!][!//

/** \brief configuration of WdgM
 *
 * This is a dummy structure to hold configuration parameters.  As the WdgM is
 * not able to be link or post build time configurable this structure is not
 * used.  It is defined for compatibility only.  A pointer to this structure
 * is passed to the Watchdog Manager initialization function for
 * configuration. */
typedef uint8 WdgM_ConfigType;

/** \brief Holding the executing Core ID */
[!IF "$GetCoreIdCalloutEnabled = 'false'"!][!//
typedef CoreIdType WdgM_EB_CoreIdType;
[!ELSE!][!//
typedef uint16 WdgM_EB_CoreIdType;
[!ENDIF!][!//

/** \brief Init status
 *
 * This type is used to define the expected / actual initialization status.
 * The two possible states are:
 * WDGM_EB_INIT_STATUS_INIT ..... WdgM is / shall be initialized
 * WDGM_EB_INIT_STATUS_DEINIT ... WdgM is / shall be de-initialized
 */
[!IF "($WdgMMulticoreEnabled = 'true')"!][!//
typedef Atomic_value_t WdgM_EB_InitStatusType;
[!ELSE!][!//
typedef uint8 WdgM_EB_InitStatusType;
[!ENDIF!][!//
/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef WDGM_TYPES_H */
/*==================[end of file]================================================================*/
[!ENDCODE!][!//
