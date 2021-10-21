/**
 * \file
 *
 * \brief AUTOSAR DoIP
 *
 * This file contains the implementation of the AUTOSAR
 * module DoIP.
 *
 * \version 1.1.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DOIP_H
#define DOIP_H
/* !LINKSTO DoIP.ASR41.SWS_DoIP_00158, 1 */
/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 20.5 (advisory)
 * '#undef' should not be used.
 *
 * Reason:
 * In order to modify the behavior of the included file, enclosing the file inclusion by a special
 * macro definition is required.
 *
 */

/*==================[includes]==============================================*/
#include <TSAutosar.h>            /* EB specific standard types */
#include <ComStack_Types.h>
/* !LINKSTO DoIP.ASR41.SWS_DoIP_00157, 1 */
#include <DoIP_Cfg.h>             /* Module configuration */
#include <DoIP_Types.h>           /* Module configuration */
#include <DoIP_Version.h>         /* Module version declarations */
#include <DoIP_Types_Int.h>

/* Exclude post-build-time config include file if requested to do so. */
#if (!defined DOIP_NO_PBCFG_REQUIRED) && (!defined DOIP_NO_CFG_REQUIRED)
#if (DOIP_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <DoIP_PBcfg.h>           /* Post-build-time configuration. */
#endif /* DOIP_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* DOIP_NO_PBCFG_REQUIRED */

/* Deviation MISRAC2012-1 */
#undef DOIP_NO_LCFG_REQUIRED
#undef DOIP_NO_PBCFG_REQUIRED
#undef DOIP_NO_CFG_REQUIRED

/*==================[macros]================================================*/

#if (defined DOIP_INSTANCE_ID)
#error DOIP_INSTANCE_ID is already defined
#endif
/** \brief Module instance ID
 *
 * Defines the instance number of this module. Since multiple instances are not supported this ID
 * is always zero.
 **/
#define DOIP_INSTANCE_ID 0U

/* === Module Det error codes === */
#if (defined DOIP_E_UNINIT)
#error DOIP_E_UNINIT is already defined
#endif
#define DOIP_E_UNINIT 0x01U /**< DET error code: Module not initialized */

#if (defined DOIP_E_PARAM_POINTER)
#error DOIP_E_PARAM_POINTER is already defined
#endif
#define DOIP_E_PARAM_POINTER 0x02U /**< DET error code: Invalid pointer (NULL_PTR) */

#if (defined DOIP_E_INVALID_PDU_SDU_ID)
#error DOIP_E_INVALID_PDU_SDU_ID is already defined
#endif
#define DOIP_E_INVALID_PDU_SDU_ID 0x03U /**< DET error code: Invalid ID */

#if (defined DOIP_E_INVALID_PARAMETER)
#error DOIP_E_INVALID_PARAMETER is already defined
#endif
#define DOIP_E_INVALID_PARAMETER 0x04U /**< DET error code: Invalid parameter */

/* === API service IDs === */
#if (defined DOIP_SID_GETVERSIONINFO)
#error DOIP_SID_GETVERSIONINFO is already defined
#endif
#define DOIP_SID_GETVERSIONINFO 0x00U /**< SID for DoIP_GetVersionInfo() */

#if (defined DOIP_SID_INIT)
#error DOIP_SID_INIT is already defined
#endif
#define DOIP_SID_INIT 0x01U /**< SID for DoIP_Init() */

#if (defined DOIP_SID_MAINFUNCTION)
#error DOIP_SID_MAINFUNCTION is already defined
#endif
#define DOIP_SID_MAINFUNCTION 0x02U  /**< SID for DoIP_MainFunction() */

#if (defined DOIP_SID_TPTRANSMIT)
#error DOIP_SID_TPTRANSMIT is already defined
#endif
#define DOIP_SID_TPTRANSMIT 0x49U /**< SID for DoIP_TpTransmit() */

#if (defined DOIP_SID_CANCELTRANSMIT)
#error DOIP_SID_CANCELTRANSMIT is already defined
#endif
#define DOIP_SID_CANCELTRANSMIT 0x04U /**< SID for DoIP_CancelTransmit() */

#if (defined DOIP_SID_CANCELRECEIVE)
#error DOIP_SID_CANCELRECEIVE is already defined
#endif
#define DOIP_SID_CANCELRECEIVE 0x05U /**< SID for DoIP_CancelReceive() */

#if (defined DOIP_SID_SOADTPCOPYTXDATA)
#error DOIP_SID_SOADTPCOPYTXDATA is already defined
#endif
#define DOIP_SID_SOADTPCOPYTXDATA 0x06U /**< SID for DoIP_SoAdTpCopyTxData() */

#if (defined DOIP_SID_SOADTPTXCONFIRMATION)
#error DOIP_SID_SOADTPTXCONFIRMATION is already defined
#endif
#define DOIP_SID_SOADTPTXCONFIRMATION 0x07U /**< SID for DoIP_SoAdTpTxConfirmation() */

#if (defined DOIP_SID_SOADTPCOPYRXDATA)
#error DOIP_SID_SOADTPCOPYRXDATA is already defined
#endif
#define DOIP_SID_SOADTPCOPYRXDATA 0x08U /**< SID for DoIP_SoAdTpCopyRxData() */

#if (defined DOIP_SID_SOADTPSTARTOFRECEPTION)
#error DOIP_SID_SOADTPSTARTOFRECEPTION is already defined
#endif
#define DOIP_SID_SOADTPSTARTOFRECEPTION 0x09U /**< SID for DoIP_SoAdTpStartOfReception() */

#if (defined DOIP_SID_SOADTPRXINDICATION)
#error DOIP_SID_SOADTPRXINDICATION is already defined
#endif
#define DOIP_SID_SOADTPRXINDICATION 0x0AU /**< SID for DoIP_SoAdTpRxIndication() */

#if (defined DOIP_SID_SOCONMODECHG)
#error DOIP_SID_SOCONMODECHG is already defined
#endif
#define DOIP_SID_SOCONMODECHG 0x0BU /**< SID for DoIP_SoConModeChg() */

#if (defined DOIP_SID_LOCALIPADDRASSIGNMENTCHG)
#error DOIP_SID_LOCALIPADDRASSIGNMENTCHG is already defined
#endif
#define DOIP_SID_LOCALIPADDRASSIGNMENTCHG 0x0CU /**< SID for DoIP_LocalIpAddrAssignmentChg() */

#if (defined DOIP_SID_ACTIVATIONLINESWITCH)
#error DOIP_SID_ACTIVATIONLINESWITCH is already defined
#endif
#define DOIP_SID_ACTIVATIONLINESWITCH 0x0FU /**< SID for DoIP_ActivationLineSwitch() */

#if (defined DOIP_SID_SOADIFTXCONFIRMATION)
#error DOIP_SID_SOADIFTXCONFIRMATION is already defined
#endif
#define DOIP_SID_SOADIFTXCONFIRMATION 0x40U /**< SID for DoIP_SoAdIfTxConfirmation() */

#if (defined DOIP_SID_SOADIFRXINDICATION)
#error DOIP_SID_SOADIFRXINDICATION is already defined
#endif
#define DOIP_SID_SOADIFRXINDICATION 0x42U /**< SID for DoIP_SoAdIfRxIndication() */

#if (defined DOIP_SID_IFTRANSMIT)
#error DOIP_SID_IFTRANSMIT is already defined
#endif
#define DOIP_SID_IFTRANSMIT 0x49U /**< SID for DoIP_IfTransmit() */

#if (defined DOIP_SID_MAINFUNCTION_TX)
#error DOIP_SID_MAINFUNCTION_TX is already defined
#endif
#define DOIP_SID_MAINFUNCTION_TX 0xF1U /**< SID for DoIP_MainFunctionTx() */

#if (defined DOIP_SID_GETANDRESETMEASUREMENTDATA)
#error DOIP_SID_GETANDRESETMEASUREMENTDATA is already defined
#endif
/** \brief Defines API id of function DoIP_GetAndResetMeasurementData()
 **/
#define DOIP_SID_GETANDRESETMEASUREMENTDATA    0x50U

/*==================[type definitions]======================================*/


/*==================[external function declarations]========================*/

#if DOIP_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define DOIP_START_SEC_CODE_CC_BLOCK
#else
#define DOIP_START_SEC_CODE
#endif
#include <DoIP_MemMap.h>

/** \brief Checks compatibility of the post-build-time configuration
*
* This service checks the compatibility of the post-build-time configuration
*
* \param[in] voidConfigPtr Pointer to the configuration data of the DoIP module.
*
* \retval E_OK Provided configuration is compatible.
* \retval E_NOT_OK Provided configuration is not compatible.
*
* \Reentrancy{Reentrant}
* \Synchronicity{Synchronous}
*/
extern FUNC(Std_ReturnType, DOIP_CODE) DoIP_IsValidConfig
(
 P2CONST(void, AUTOMATIC, DOIP_APPL_CONST) voidConfigPtr
);

#if DOIP_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define DOIP_STOP_SEC_CODE_CC_BLOCK
#include <DoIP_MemMap.h>
#define DOIP_START_SEC_CODE
#include <DoIP_MemMap.h>
#endif

#if(DOIP_VERSION_INFO_API == STD_ON)
/* DoIP_GetVersionInfo() API function is enabled */

/** \brief API to get the module version information
 *
 * This service returns the version information of this module.
 *
 * \param[out] VersionInfo - Pointer to return the module version information.
 *
 * \ServiceID{0x00}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
extern FUNC(void, DOIP_CODE) DoIP_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, DOIP_APPL_DATA) VersionInfo
);
#elif (DOIP_VERSION_INFO_API == STD_OFF)

#if (defined DoIP_GetVersionInfo)        /* To prevent double declaration */
#error DoIP_GetVersionInfo is already defined
#endif /* (defined DoIP_GetVersionInfo) */
/*
 * DoIP_GetVersionInfo() API function is disabled - provide macro implementation
 */
#define DoIP_GetVersionInfo(versionInfo)                           \
    do                                                             \
    {                                                              \
        (versionInfo)->vendorID = DOIP_VENDOR_ID;                  \
        (versionInfo)->moduleID = DOIP_MODULE_ID;                  \
        (versionInfo)->sw_major_version = DOIP_SW_MAJOR_VERSION;   \
        (versionInfo)->sw_minor_version = DOIP_SW_MINOR_VERSION;   \
        (versionInfo)->sw_patch_version = DOIP_SW_PATCH_VERSION;   \
    } while(0)

#endif /* DOIP_VERSION_INFO_API */

/** \brief API to initialize the module
 *
 * This service initializes the modules shared variables.
 *
 * \param[in] DoIP_ConfigPtr - Pointer to the module post build configuration.
 *
 * \ServiceID{0x01}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
*/
extern FUNC(void, DOIP_CODE) DoIP_Init
(
  P2CONST(DoIP_ConfigType, AUTOMATIC, DOIP_APPL_CONST) DoIP_ConfigPtr
);

#if(DOIP_ENABLE_MAIN_FUNCTION_TX == STD_ON)
/** \brief Transmit part of the main function if enabled to be external
 *
 * \ServiceID{0xF1}
*/
extern FUNC(void, DOIP_CODE) DoIP_MainFunctionTx(void) ;
#endif

/** \brief Main function API
 *
 * This function executes module tasks periodically.
 *
 * \ServiceID{0x02}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 * \Timing fixed
*/
extern FUNC(void, DOIP_CODE) DoIP_MainFunction(void);

/** \brief API to request Tp data transfer from the PduR to the SoAd.
 *
 * \param[in] DoIPPduRTxId - DoIP handle ID to be used for DoIP API calls from PduR.
 * \param[in] DoIPPduRTxInfoPtr - Pointer to  Tx PDU information structure which contains
 *            the length of the message to transmit.
 *
 * \return Std_ReturnType
 * \retval E_OK - Request accepted
 * \retval E_NOT_OK - Request not accepted
 *
 * \ServiceID{0x03}
 * \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 * \Synchronicity{Asynchronous}
*/
extern FUNC(Std_ReturnType, DOIP_CODE) DoIP_TpTransmit
(
  PduIdType DoIPPduRTxId,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_CONST) DoIPPduRTxInfoPtr
);

/** \brief API to cancel a transmission from PduR to SoAd.
 *
 * \param[in] DoIPPduRTxId - DoIP handle ID to be used for DoIP API calls from PduR.
 *
 * \return Std_ReturnType
 * \retval E_OK - Request is accepted.
 * \retval E_NOT_OK - Request is rejected.
 *
 * \ServiceID{0x04}
 * \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 * \Synchronicity{Synchronous}
*/
extern FUNC(Std_ReturnType, DOIP_CODE) DoIP_CancelTransmit
(
  PduIdType DoIPPduRTxId
);


/** \brief API to cancel a reception from SoAd to PduR
 *
 * \param[in] DoIPPduRRxId - DoIP handle ID to be used for DoIP API calls from PduR.
 *
 * \return Std_ReturnType
 * \retval E_OK - Request is accepted.
 * \retval E_NOT_OK - Request is rejected.
 *
 * \ServiceID{0x05}
 * \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId}
 * \Synchronicity{Synchronous}
*/
extern FUNC(Std_ReturnType, DOIP_CODE) DoIP_CancelReceive
(
  PduIdType DoIPPduRRxId
);

#if (DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
/**
 * \brief API to read and reset measurement data.
 *
 * This service allows to read and reset detailed measurement data for diagnostic purposes.
 * Get all MeasurementIdx's at once is not supported. DOIP_MEAS_ALL shall only be used to reset
 * all MeasurementIdx's at once. A NULL_PTR shall be provided for MeasurementDataPtr in this case.
 *
 * \param[in] MeasurementIdx         Index to select specific measurement data:
 *                                   DOIP_MEAS_DROP_GENHDR (0x01) - Measurement index of dropped
 *                                   messages caused by invalid generic header.
 *                                   DOIP_MEAS_DROP_DIAGMSG (0x02) - Measurement index of dropped
 *                                   diag messages.
 *                                   DOIP_MEAS_ALL (0xFF) - Represents all measurement indexes.
 * \param[in] MeasurementResetNeeded Flag to trigger a reset of the measurement data.
 * \param[out] MeasurementDataPtr    Pointer to data buffer, where to copy measurement data.
 *
 * \returns Std_ReturnType
 * \retval  E_OK     The function has been successfully executed.
 * \retval  E_NOT_OK The function could not be successfully executed.
 * \ServiceID{0x50}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) DoIP_GetAndResetMeasurementData
(
  DoIP_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, DOIP_APPL_DATA) MeasurementDataPtr
);
#endif /* DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

/*==================[internal function declarations]========================*/

#define DOIP_STOP_SEC_CODE
#include <DoIP_MemMap.h>
/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/



#endif /* DOIP_H */
/*==================[end of file]===========================================*/

