/**
 * \file
 *
 * \brief AUTOSAR FrSM
 *
 * This file contains the implementation of the AUTOSAR
 * module FrSM.
 *
 * \version 5.3.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRSM_H
#define FRSM_H

/*======================[Include Section]=====================================*/

#include <ComStack_Types.h>     /* get AUTOSAR ComStack data types          */
#include <ComM.h>               /* get ComM_ModeType type definition        */
#include <FrSM_Cfg.h>   /* get FrSM post-build-time config declarations    */
#include <FrSM_Lcfg.h>    /* get FrSM link-time config declarations          */
#include <FrSM_Types.h>         /* get FrSM specific data types             */
#include <FrSM_CfgTypes.h>           /* get FrSM pre-compile-time configuration  */

/*======================[Global Macros]=======================================*/

/**
 * \brief Module information
 *
 * Vendor ID (EB) for the module FrSM.
 */
#define FRSM_VENDOR_ID 1U

/**
 * \brief Module information
 *
 * Module ID for module FrSM.
 */
#define FRSM_MODULE_ID 142U

/*
 * Development error codes according to FrSM SWS
 */

/**
 * \brief DET error code
 *
 * Invalid pointer in parameter list.
 */
#define FRSM_E_NULL_PTR      1U

/**
 * \brief DET error code
 *
 * Invalid network handle parameter.
 */
#define FRSM_E_INV_HANDLE       2U

/**
 * \brief DET error code
 *
 * FrSM module was not initialized.
 */
#define FRSM_E_UNINIT  3U

/**
 * \brief DET error code
 *
 * Invalid communication mode requested.
 */
#define FRSM_E_INV_MODE         4U

/**
 * \brief DET error code
 *
 * SchM_Call_ComM_BusSM_Modeindication failed.
 */
#define FRSM_E_MODEINDICATION         250U

/*======================[Global Data Types]===================================*/

/*======================[Global Data]=========================================*/

/*======================[Global Function Declarations]========================*/

#define FRSM_START_SEC_CODE
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

#if (FRSM_VERSION_INFO_API == STD_ON)
/* FrSM_GetVersionInfo() API function is enabled */

 /** \brief Get version information of the FlexRay State Manager.
  *
  * This service returns the version information of this module. The version
  * information includes:
  *  - Module Id
  *  - Vendor Id
  *  - Vendor specific version numbers
  *
  * \param[out] versioninfo Pointer where to store the version
  *                                 information of this module.
  *
  * \ServiceID{4}
  * \Reentrancy{Reentrant}
  * \Synchronicity{Synchronous}
 */
extern FUNC(void,FRSM_CODE) FrSM_GetVersionInfo
    (
        P2VAR(Std_VersionInfoType,AUTOMATIC,FRSM_APPL_DATA) versioninfo
    );
#endif /* FRSM_VERSION_INFO_API */


 /** \brief Initialization service for module FrSM.
  *
  * This service initializes the FlexRay State Manager local variables and the
  * state machine for each cluster (state FRSM_READY).
  *
  * It initially stores the post-build-time configuration passed as argument to enable
  * subsequent service calls to access the configuration.
  *
  * \param[in] FrSM_ConfigPtr Pointer to a selected configuration structure
  *
  * \ServiceID{1}
  * \Reentrancy{Non Reentrant}
  * \Synchronicity{Synchronous}
 */
extern FUNC(void,FRSM_CODE) FrSM_Init
    (
        P2CONST(FrSM_ConfigType,AUTOMATIC,FRSM_APPL_CONST) FrSM_ConfigPtr
    );

/* Avoid redundant redeclaration if Multicoresupport is enabled */
#if (FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF)
 /** \brief Communication mode change request service.
  *
  * This service requests a change of the communication mode from the FrSM.
  * This service doesn't necessarily change into the requested communication mode
  * synchronously but might store the request internally and process it via further
  * FrSM_MainFunction_#() invocations.
  *
  * Only COMM_FULL_COMMUNICATION and COMM_NO_COMMUNICATION may be requested.
  *
  * If COMM_SILENT_COMMUNICATION is requested, the service will return E_NOT_OK.
  *
  * \param[in] NetworkHandle This parameter identifies the FlexRay cluster for
               which a communication mode is requested.
  * \param[in] ComM_Mode This parameter holds the requested communication mode.
  * \return Std_ReturnType
  * \retval E_OK Request accepted
  * \retval E_NOT_OK Request not accepted
  *
  * \ServiceID{2}
  * \Reentrancy{Reentrant for different FlexRay clusters}
  * \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType,FRSM_CODE) FrSM_RequestComMode
    (
        NetworkHandleType NetworkHandle,
        ComM_ModeType ComM_Mode
    );

 /** \brief Service to receive the current communication mode.
  *
  * This service receives the current communication mode from the FrSM.
  *
  * Only COMM_FULL_COMMUNICATION or COMM_NO_COMMUNICATION will be received.
  *
  * \param[in] NetworkHandle Handle of communication network
  * \param[out] ComM_ModePtr Pointer to the memory location where the current communication mode
  * shall be stored
  * \return Std_ReturnType
  * \retval E_OK: Request accepted
  * \retval E_NOT_OK: Request was not accepted as the FrSM has not been initialized using FrSM_Init.
  *
  * \ServiceID{3}
  * \Reentrancy{Reentrant for different FlexRay clusters}
  * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRSM_CODE) FrSM_GetCurrentComMode
    (
        NetworkHandleType NetworkHandle,
        P2VAR(ComM_ModeType,AUTOMATIC,FRSM_APPL_DATA) ComM_ModePtr
    );
#endif

#if ( (FRSM_FRTRCV_CONTROL_ENABLE == STD_ON) && (FRSM_SETECUPASSIVE_ENABLE == STD_ON) )
/**
 * \brief Service to set all FlexRay clusters of the ECU to receive only mode.
 *
 * \param[in] FrSM_Passive   If true set to passive mode. If false set to active mode.
 * \return Std_ReturnType
 * \retval E_OK     Request accepted
 * \retval E_NOT_OK Request not accepted
 *
 * \ServiceID{6}
 * \Reentrancy{Non reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRSM_CODE) FrSM_SetEcuPassive
    (
        boolean FrSM_Passive
    );
#endif /* ( (FRSM_FRTRCV_CONTROL_ENABLE == STD_ON) && (FRSM_SETECUPASSIVE_ENABLE == STD_ON) ) */

#if (FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON)
/** \brief This API function can be used to leave the KeySlotOnlyMode.
 **
 ** This function calls FrIf_AllSlots for the controller of the FlexRay cluster.
 **
 ** \param[in] NetworkHandle This parameter identifies the FlexRay cluster for which a
 ** communication mode is requested.
 **
 ** \return Std_ReturnType
 ** \retval E_OK     Request accepted
 ** \retval E_NOT_OK Request not accepted
 **
 ** \ServiceID{5}
 ** \Reentrancy{Reentrant for different FlexRay clusters}
 ** \Synchronicity{Asynchronous}
 **/
extern FUNC(Std_ReturnType, FRSM_CODE) FrSM_AllSlots(
  NetworkHandleType NetworkHandle);
#endif /* FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON */

#define FRSM_STOP_SEC_CODE
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */


#endif /* FRSM_H */

