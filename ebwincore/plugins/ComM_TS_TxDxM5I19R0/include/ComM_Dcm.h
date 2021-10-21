/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* !LINKSTO ComM463_Refine_ComM_Dcm,1, ComM266,1 */
#ifndef COMM_DCM_H
#define COMM_DCM_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>     /* AUTOSAR COM stack standard types */
#include <ComM_Version.h>       /* ComM module version information */
#include <ComM_Cfg.h>           /* ComM configuration parameters */

/*==================[macros]================================================*/
#if (COMM_DCM_ENABLED == STD_ON)

#if (defined COMM_SID_DCM_ACTIVE_DIAGNOSTIC)
#error COMM_SID_DCM_ACTIVE_DIAGNOSTIC already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_DCM_ActiveDiagnostic(). */
#define COMM_SID_DCM_ACTIVE_DIAGNOSTIC  31U

#if (defined COMM_SID_DCM_INACTIVE_DIAGNOSTIC)
#error COMM_SID_DCM_INACTIVE_DIAGNOSTIC already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_DCM_InactiveDiagnostic(). */
#define COMM_SID_DCM_INACTIVE_DIAGNOSTIC  32U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

#ifndef RTE_SCHM_SWCBSW_COMM_DCM_ACTIVEDIAGNOSTIC
#define RTE_SCHM_SWCBSW_COMM_DCM_ACTIVEDIAGNOSTIC
/** \brief Indicate an active diagnostic session by DCM.
 **
 ** This function provides an indication that DCM is currently
 ** handling an active diagnostic session on the channel given by
 ** ComM's configuration.
 **
 ** This implies that for the respective channels, network mode
 ** must be requested.
 **
 ** \param[in] Channel Network channel needed for Diagnostic
 **                    communication.
 **
 ** \ServiceID{::COMM_SID_DCM_ACTIVE_DIAGNOSTIC}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, COMM_CODE) ComM_DCM_ActiveDiagnostic
(
  NetworkHandleType Channel
);
#endif /* RTE_SCHM_SWCBSW_COMM_DCM_ACTIVEDIAGNOSTIC */

#ifndef RTE_SCHM_SWCBSW_COMM_DCM_INACTIVEDIAGNOSTIC
#define RTE_SCHM_SWCBSW_COMM_DCM_INACTIVEDIAGNOSTIC
/** \brief Indicate an inactive diagnostic session by DCM.
 **
 ** This function provides an indication that DCM is currently
 ** not handling an active diagnostic session on the channel
 ** given by ComM's configuration.
 **
 ** This implies that for the respective channels, network mode can
 ** be released when no other user requires the respective channel.
 **
 ** \param[in] Channel Network channel no longer needed for Diagnostic
 **                    communication.
 **
 ** \ServiceID{::COMM_SID_DCM_INACTIVE_DIAGNOSTIC}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, COMM_CODE) ComM_DCM_InactiveDiagnostic
(
  NetworkHandleType Channel
);
#endif /* RTE_SCHM_SWCBSW_COMM_DCM_INACTIVEDIAGNOSTIC */

#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/
#endif /* if(COMM_DCM_ENABLED == STD_ON) */

#endif /* ifndef COMM_DCM_H */
/*==================[end of file]===========================================*/
