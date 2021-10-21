/**
 * \file
 *
 * \brief AUTOSAR ComXf
 *
 * This file contains the implementation of the AUTOSAR
 * module ComXf.
 *
 * \version 1.0.37
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef COMXF_API_STC_H
#define COMXF_API_STC_H

#include <ComXf_Cfg.h>          /* Pre-compile time configuration */
#include <ComXf_Version.h>      /* Module Version Info */

/*==================[macros]================================================*/

#if (defined COMXF_INSTANCE_ID) /* To prevent double declaration */
#error COMXF_INSTANCE_ID already defined
#endif /* #if (defined COMXF_INSTANCE_ID) */

/** \brief Id of instance of ComXf provided to Det_ReportError() **/
#define COMXF_INSTANCE_ID                      0x00U


/* API Service ID's */

#if (defined COMXF_SID_GETVERSIONINFO) /* To prevent double declaration */
#error COMXF_SID_GETVERSIONINFO already defined
#endif /* #if (defined COMXF_SID_GETVERSIONINFO) */

/** \brief API Service ID for ComXf_GetVersioninfo() */
#define COMXF_SID_GETVERSIONINFO               0x00U


#if (defined COMXF_SID_INIT) /* To prevent double declaration */
#error COMXF_SID_INIT already defined
#endif /* #if (defined COMXF_SID_INIT) */

/** \brief API Service ID for ComXf_Init() */
#define COMXF_SID_INIT                         0x01U


#if (defined COMXF_SID_DEINIT) /* To prevent double declaration */
#error COMXF_SID_DEINIT already defined
#endif /* #if (defined COMXF_SID_DEINIT) */

/** \brief API Service ID for ComXf_DeInit() */
#define COMXF_SID_DEINIT                       0x02U


#if (defined COMXF_SID_SR_TRANSFORMER) /* To prevent double declaration */
#error COMXF_SID_SR_TRANSFORMER already defined
#endif /* #if (defined COMXF_SID_SR_TRANSFORMER) */

/** \brief API Service ID for ComXf_transformerId() of Sender/Receiver communication */
#define COMXF_SID_SR_TRANSFORMER               0x03U


#if (defined COMXF_SID_SR_INV_TRANSFORMER) /* To prevent double declaration */
#error COMXF_SID_SR_INV_TRANSFORMER already defined
#endif /* #if (defined COMXF_SID_SR_INV_TRANSFORMER) */

/** \brief API Service ID for ComXf_Inv_transformerId() of Sender/Receiver communication */
#define COMXF_SID_SR_INV_TRANSFORMER           0x04U


/* Development error values of type uint8 */

#if (defined COMXF_E_UNINIT) /* To prevent double declaration */
#error COMXF_E_UNINIT already defined
#endif /* #if (defined COMXF_E_UNINIT) */

/** \brief Error code if any other API service, except GetVersionInfo is called before the
 ** transformer module was initialized with Init or after a call to DeInit */
#define COMXF_E_UNINIT                         0x01U


#if (defined COMXF_E_INIT_FAILED) /* To prevent double declaration */
#error COMXF_E_INIT_FAILED already defined
#endif /* #if (defined COMXF_E_INIT_FAILED) */

 /** \brief Error code if an invalid configuration set was selected */
#define COMXF_E_INIT_FAILED                    0x02U


#if (defined COMXF_E_PARAM) /* To prevent double declaration */
#error COMXF_E_PARAM already defined
#endif /* #if (defined COMXF_E_PARAM) */

/** \brief API Service called with wrong parameter */
#define COMXF_E_PARAM                          0x03U


#if (defined COMXF_E_PARAM_POINTER) /* To prevent double declaration */
#error COMXF_E_PARAM_POINTER already defined
#endif /* #if (defined COMXF_E_PARAM_POINTER) */

/** \brief API Service called with invalid pointer */
#define COMXF_E_PARAM_POINTER                  0x04U


/*==================[type definitions]======================================*/

/** \brief This is the type of the data structure containing the initialization data for
 ** the transformer. */
typedef VAR(uint8, TYPEDEF) ComXf_ConfigType;

/*==================[external function declarations]========================*/

#define COMXF_START_SEC_CODE
#include <ComXf_MemMap.h>

#if (COMXF_VERSION_INFO_API == STD_ON)

/** \brief Get version information of the ComXf module.
**
** This service returns the version information of this module. The version
** information includes:
**  - Module Id
**  - Vendor Id
**  - Vendor specific version numbers
**
** \param[out] VersionInfo Pointer to where to store the version
**                         information of this module.
**
** \ServiceID{0x00}
** \Reentrancy{Reentrant}
** \Synchronicity{Synchronous}
*/
extern FUNC(void, COMXF_CODE) ComXf_GetVersionInfo
(
   P2VAR(Std_VersionInfoType, AUTOMATIC, COMXF_APPL_DATA) VersionInfo
);
#endif /* COMXF_VERSION_INFO_API == STD_ON */


/** \brief Initialize the ComXf module.
**
** This function initializes the ComXf module.
**
** \param[in] config Points to the implementation specific structure
**
** \ServiceID{0x01}
** \Reentrancy{Non-Reentrant}
** \Synchronicity{Synchronous}
*/
extern FUNC(void, COMXF_CODE) ComXf_Init
(
   P2CONST(ComXf_ConfigType, AUTOMATIC, COMXF_APPL_CONST) config
);


/** \brief Deinitialize the ComXf module.
**
** This function deinitializes the ComXf module.
**
** \ServiceID{0x02}
** \Reentrancy{Non-Reentrant}
** \Synchronicity{Synchronous}
*/
extern FUNC(void, COMXF_CODE) ComXf_DeInit(void);


#define COMXF_STOP_SEC_CODE
#include <ComXf_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* COMXF_API_STC_H */
