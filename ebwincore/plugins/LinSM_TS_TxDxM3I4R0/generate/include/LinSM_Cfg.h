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
#ifndef LINSM_CFG_H
#define LINSM_CFG_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* AUTOSAR communication stack types */
#include <LinSM_Version.h>     /* this module's version declaration */

/*==================[macros]================================================*/

#if (defined LINSM_DEV_ERROR_DETECT) /* to prevent double declaration */
#error LINSM_DEV_ERROR_DETECT already defined
#endif /* if (defined LINSM_DEV_ERROR_DETECT) */

/** \brief Switch for DET usage */
[!IF "LinSMGeneral/LinSMDevErrorDetect = 'true'"!][!//
#define LINSM_DEV_ERROR_DETECT   STD_ON
[!ELSE!][!//
#define LINSM_DEV_ERROR_DETECT   STD_OFF
[!ENDIF!][!//

#if (defined LINSM_VERSION_INFO_API) /* to prevent double declaration */
#error LINSM_VERSION_INFO_API already defined
#endif /* if (defined LINSM_VERSION_INFO_API) */

/** \brief Switch for version info API */
[!IF "LinSMGeneral/LinSMVersionInfoApi = 'true'"!][!//
#define LINSM_VERSION_INFO_API   STD_ON
[!ELSE!][!//
#define LINSM_VERSION_INFO_API   STD_OFF
[!ENDIF!][!//

#if (defined LINSM_MULTICORE_ENABLED) /* to prevent double declaration */
#error LINSM_MULTICORE_ENABLED already defined
#endif /* if (defined LINSM_MULTICORE_ENABLED) */

/** \brief Switch for multicore support API */
[!IF "$multiCoreSupport = 'true'"!][!//
#define LINSM_MULTICORE_ENABLED  STD_ON
[!ELSE!][!//
#define LINSM_MULTICORE_ENABLED  STD_OFF
[!ENDIF!][!//

#if (defined LINSM_SLAVE_NODES_ENABLED) /* to prevent double declaration */
#error LINSM_SLAVE_NODES_ENABLED already defined
#endif /* if (defined LINSM_SLAVE_NODES_ENABLED) */

/** \brief Switch for slave nodes support API and behavior */
[!IF "$numSlaveNodes > 0"!][!//
#define LINSM_SLAVE_NODES_ENABLED  STD_ON
[!ELSE!][!//
#define LINSM_SLAVE_NODES_ENABLED  STD_OFF
[!ENDIF!][!//

#if (defined LINSM_MASTER_NODES_ENABLED) /* to prevent double declaration */
#error LINSM_MASTER_NODES_ENABLED already defined
#endif /* if (defined LINSM_MASTER_NODES_ENABLED) */

/** \brief Switch for slave nodes support API and behavior */
[!IF "num:i(count(LinSMConfigSet/*[1]/LinSMChannel/*[LinSMNodeType = 'MASTER'])) > 0"!][!//
#define LINSM_MASTER_NODES_ENABLED  STD_ON
[!ELSE!][!//
#define LINSM_MASTER_NODES_ENABLED  STD_OFF
[!ENDIF!][!//

/** \brief Defensive programming */



/*------------------[Defensive programming]---------------------------------*/
[!SELECT "LinSMDefensiveProgramming"!][!//

#if (defined LINSM_DEFENSIVE_PROGRAMMING_ENABLED)
#error LINSM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define LINSM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../LinSMGeneral/LinSMDevErrorDetect  = 'true') and (LinSMDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LINSM_PRECONDITION_ASSERT_ENABLED)
#error LINSM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define LINSM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../LinSMGeneral/LinSMDevErrorDetect  = 'true') and (LinSMDefProgEnabled = 'true') and (LinSMPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LINSM_POSTCONDITION_ASSERT_ENABLED)
#error LINSM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define LINSM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../LinSMGeneral/LinSMDevErrorDetect  = 'true') and (LinSMDefProgEnabled = 'true') and (LinSMPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LINSM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error LINSM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define LINSM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../LinSMGeneral/LinSMDevErrorDetect  = 'true') and (LinSMDefProgEnabled = 'true') and (LinSMUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LINSM_INVARIANT_ASSERT_ENABLED)
#error LINSM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define LINSM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../LinSMGeneral/LinSMDevErrorDetect  = 'true') and (LinSMDefProgEnabled = 'true') and (LinSMInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LINSM_STATIC_ASSERT_ENABLED)
#error LINSM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define LINSM_STATIC_ASSERT_ENABLED           [!//
[!IF "(../LinSMGeneral/LinSMDevErrorDetect  = 'true') and (LinSMDefProgEnabled = 'true') and (LinSMStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//


/*------------------[symbolic name values]----------------------------------*/

#if (LINSM_MASTER_NODES_ENABLED == STD_ON)

#if (defined LinSMConf_LinSMSchedule_NULL_SCHEDULE) /* to prevent double declaration */
#error LinSMConf_LinSMSchedule_NULL_SCHEDULE already defined
#endif /* if (defined LinSMConf_LinSMSchedule_NULL_SCHEDULE) */

#define LinSMConf_LinSMSchedule_NULL_SCHEDULE 0U

[!LOOP "LinSMConfigSet/*[1]/LinSMChannel/*/LinSMSchedule/*"!][!//
[!IF "LinSMScheduleIndex = 0"!]

#if (defined LinSMConf_LinSMSchedule_[!"@name"!]) /* to prevent double declaration */
#error LinSMConf_LinSMSchedule_[!"@name"!] already defined
#endif /* if (defined LinSMConf_LinSMSchedule_[!"@name"!]) */

/** \brief Export symbolic name value for parameter LinIfScheduleIndex */
#define LinSMConf_LinSMSchedule_[!"@name"!] 0U

[!ELSE!]
[!IF "node:refvalid(LinSMScheduleIndexRef)='true'"!]
[!VAR "SymbolName" = "node:name(as:ref(LinSMScheduleIndexRef))"!]
/*---------------------------[!"@name"!]-------------------------------------*/

#if (defined LinSMConf_LinSMSchedule_[!"@name"!]) /* to prevent double declaration */
#error LinSMConf_LinSMSchedule_[!"@name"!] already defined
#endif /* if (defined LinSMConf_LinSMSchedule_[!"@name"!]) */

/** \brief Export symbolic name value for parameter LinIfScheduleIndex */
#define LinSMConf_LinSMSchedule_[!"@name"!] [!"concat('LinIfConf_LinIfScheduleTable_', $SymbolName)"!]

[!ENDIF!]
[!ENDIF!]
[!ENDLOOP!]

#endif /* (LINSM_MASTER_NODES_ENABLED == STD_ON) */

/*==================[type definitions]======================================*/

/** \brief Bogus definition, since post-build configuration is not supported */
typedef uint8 LinSM_ConfigType;

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#define LINSM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/** \brief Empty configuration structure to be passed to LinSM_Init() */
extern CONST(LinSM_ConfigType, LINSM_APPL_CONST) [!"LinSMConfigSet/*[1]/@name"!];

#define LINSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

/*==================[external function declarations]========================*/

#endif /* ifndef LINSM_CFG_H */
/*==================[end of file]===========================================*/

[!ENDCODE!]
