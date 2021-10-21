/**
 * \file
 *
 * \brief AUTOSAR Det
 *
 * This file contains the implementation of the AUTOSAR
 * module Det.
 *
 * \version 6.5.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef DET_BSW_H
#define DET_BSW_H

/*==================[inclusions]============================================*/

#include <Det_Cfg.h>                   /* this module's public configuration */

/* we do not need to include Rte_Types.h here because the Rte does not define
 * any Det specific data types. */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define DET_START_SEC_CODE
#include <Det_MemMap.h>

/* Declaration of Det service API must _NOT_ be excluded in case of
 * internal usage because Det.c is decoupled from RTE. */

/* !LINKSTO EB_Req_Det_enableASR43.API.implementation.DetCFile,1 */
/** \brief Report transient faults.
 **
 ** This function reports transient faults. This function is called from modules XXX for transient
 ** faults in case of activated preprocessor switch XXX_DEV_ERROR_DETECT. Production relevant
 ** transient faults shall be reported to the Diagnostics Event Manager (DEM).
 **
 ** \param[in] ModuleId Module ID of calling module.
 ** \param[in] InstanceId Instance ID of calling module.
 ** \param[in] ApiId ID of API service in which transient fault is detected (defined in SWS of calling module)
 ** \param[in] FaultId ID of detected transient fault (defined in SWS of calling module).
 ** \return :  If no callout exists it shall return E_OK, otherwise it shall return the value of the
 **            configured callout. In case several callouts are configured the logical or (sum) of
 **            the callout return values shall be returned. Rationale: since E_OK=0, E_OK will be
 **            only returned if all are E_OK, and for multiple error codes there is a good chance
 **            to detect several of them.
 **
 ** \ServiceID{::DET_SID_REPORT_TRANSIENT_FAULT}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DET_CODE) Det_ASR43_ReportTransientFault(
   uint16 ModuleId,
   uint8 InstanceId,
   uint8 ApiId,
   uint8 FaultId);

/* !LINKSTO EB_Req_Det_enableASR43.API.implementation.DetCFile,1 */
/** \brief Report runtime errors.
 **
 ** This function reports runtime errors. This function is called from
 ** modules XXX for runtime errors in case of activated preprocessor
 ** switch XXX_DEV_ERROR_DETECT. Production relevant errors shall be reported
 ** to the Diagnostics Event Manager (DEM).
 **
 ** \param[in] ModuleId Module ID of calling module.
 ** \param[in] InstanceId Instance ID of calling module.
 ** \param[in] ApiId ID of API service in which error is detected.
 ** \param[in] ErrorId ID of detected runtime error.
 ** \return always ::E_OK
 **
 ** \ServiceID{::DET_SID_REPORT_RUNTIME_ERROR}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DET_CODE) Det_ASR43_ReportRuntimeError(
   uint16 ModuleId,
   uint8 InstanceId,
   uint8 ApiId,
   uint8 ErrorId);

/* !LINKSTO Det.EB.ASR32.Det103,1 */
/** \brief Report development errors.
 **
 ** This function reports development errors. This function is called from
 ** modules XXX for development errors in case of activated preprocessor
 ** switch XXX_DEV_ERROR_DETECT.  Production relevant errors shall be reported
 ** to the Diagnostics Event Manager (DEM).
 **
 ** \param[in] ModuleId Module ID of calling module.
 ** \param[in] InstanceId Instance ID of calling module.
 ** \param[in] ApiId ID of API service in which error is detected.
 ** \param[in] ErrorId ID of detected development error.
 ** \return always ::E_OK
 **
 ** \ServiceID{::DET_SID_REPORT_ERROR}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DET_CODE) Det_ASR40_ReportError(
   uint16 ModuleId,
   uint8 InstanceId,
   uint8 ApiId,
   uint8 ErrorId);

/* !LINKSTO Det.EB.ASR32.Det102,1 */
#if ((DET_ENABLE_ASR32_SERVICE_API == STD_ON) || (DET_DEFAULT_ASR_SERVICE_API == DET_SERVICE_API_ASR32))
/** \brief Report development errors.
 **
 ** This function is the AUTOSAR 3.2 specific implementation to report development errors.
 ** This function is called from modules XXX for development errors in case of activated
 ** preprocessor switch XXX_DEV_ERROR_DETECT.  Production relevant errors shall be reported
 ** to the Diagnostics Event Manager (DEM).
 **
 ** \param[in] ModuleId Module ID of calling module.
 ** \param[in] InstanceId Instance ID of calling module.
 ** \param[in] ApiId ID of API service in which error is detected.
 ** \param[in] ErrorId ID of detected development error.
 ** \return none
 **
 ** \ServiceID{::DET_SID_REPORT_ERROR}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DET_CODE) Det_ASR32_ReportError(
   uint16 ModuleId,
   uint8  InstanceId,
   uint8  ApiId,
   uint8  ErrorId);
#endif

#define DET_STOP_SEC_CODE
#include <Det_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef DET_BSW_H */
