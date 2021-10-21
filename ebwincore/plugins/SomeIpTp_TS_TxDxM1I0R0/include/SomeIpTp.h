/**
 * \file
 *
 * \brief AUTOSAR SomeIpTp
 *
 * This file contains the implementation of the AUTOSAR
 * module SomeIpTp.
 *
 * \version 1.0.28
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef SOMEIPTP_H
#define SOMEIPTP_H

/*==================[inclusions]============================================*/

#include <SomeIpTp_Cfg.h>         /* Generated configuration */
#include <SomeIpTp_Types.h>       /* Specific SomeIpTp types */

#include <Det.h>                  /* Module public API */

/*==================[macros]================================================*/

/*------------------[service Ids]----------------------------------------*/

/** \brief Module instance ID
 *
 * Defines the instance number of this module. Since multiple instances are not supported this ID
 * is always zero.
 **/
#if (defined SOMEIPTP_INSTANCE_ID)
#error SOMEIPTP_INSTANCE_ID already defined!
#endif /* #if (defined SOMEIPTP_INSTANCE_ID)*/
#define SOMEIPTP_INSTANCE_ID 0U

/** \brief Defines API id of function SomeIpTp_GetVersionInfo(). */
#if (defined SOMEIPTP_SID_GETVERSIONINFO)
#error SOMEIPTP_SID_GETVERSIONINFO already defined!
#endif /* #if (defined SOMEIPTP_SID_GETVERSIONINFO)*/
#define SOMEIPTP_SID_GETVERSIONINFO 0x01U

/** \brief Defines API id of function SomeIpTp_Init(). */
#if (defined SOMEIPTP_SID_INIT)
#error SOMEIPTP_SID_INIT already defined!
#endif /* #if (defined SOMEIPTP_SID_INIT)*/
#define SOMEIPTP_SID_INIT 0x02U

/** \brief Defines API id of function SomeIpTp_MainFunctionTx(). */
#if (defined SOMEIPTP_SID_MAINFUNCTIONTX)
#error SOMEIPTP_SID_MAINFUNCTIONTX already defined!
#endif /* #if (defined SOMEIPTP_SID_MAINFUNCTIONTX)*/
#define SOMEIPTP_SID_MAINFUNCTIONTX 0x03U

/** \brief Defines API id of function SomeIpTp_MainFunctionRx(). */
#if (defined SOMEIPTP_SID_MAINFUNCTIONRX)
#error SOMEIPTP_SID_MAINFUNCTIONRX already defined!
#endif /* #if (defined SOMEIPTP_SID_MAINFUNCTIONRX)*/
#define SOMEIPTP_SID_MAINFUNCTIONRX 0x04U

/** \brief Defines API id of function SomeIpTp_Transmit(). */
#if (defined SOMEIPTP_SID_TRANSMIT)
#error SOMEIPTP_SID_TRANSMIT already defined!
#endif /* #if (defined SOMEIPTP_SID_TRANSMIT)*/
#define SOMEIPTP_SID_TRANSMIT 0x49U

/** \brief Defines Dummy API id of function SomeIpTp_PduRIfTransmit(). */
#if (defined SOMEIPTP_SID_PDURIFTRANSMIT)
#error SOMEIPTP_SID_PDURIFTRANSMIT already defined!
#endif /* #if (defined SOMEIPTP_SID_PDURIFTRANSMIT)*/
#define SOMEIPTP_SID_PDURIFTRANSMIT 0xFF

/*------------------[Det error codes]----------------------------------------*/

/*------------------[Develepment Errors]-----------------*/

/** \brief Definition of DET error code SOMEIPTP_E_NOTINIT */
/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00052,1 */
#if (defined SOMEIPTP_E_NOTINIT)
#error SOMEIPTP_E_NOTINIT already defined!
#endif /* #if (defined SOMEIPTP_E_NOTINIT)*/
#define SOMEIPTP_E_NOTINIT 0x01U /** SOME/IP TP module not initialized */

/** \brief Definition of DET error code SOMEIPTP_E_PARAM_POINTER */
/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00052,1 */
#if (defined SOMEIPTP_E_PARAM_POINTER)
#error SOMEIPTP_E_PARAM_POINTER already defined!
#endif /* #if (defined SOMEIPTP_E_PARAM_POINTER)*/
#define SOMEIPTP_E_PARAM_POINTER 0x02U /** Null pointer has been passed as an argument */

/** \brief Definition of DET error code SOMEIPTP_E_PARAM */
/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00052,1 */
#if (defined SOMEIPTP_E_PARAM)
#error SOMEIPTP_E_PARAM already defined!
#endif /* #if (defined SOMEIPTP_E_PARAM)*/
#define SOMEIPTP_E_PARAM 0x03U /** Unknown parameter has been passed */

/** \brief Definition of DET error code SOMEIPTP_E_INVALID_CALL */
#if (defined SOMEIPTP_E_INVALID_CALL)
#error SOMEIPTP_E_INVALID_CALL already defined!
#endif /* #if (defined SOMEIPTP_E_INVALID_CALL)*/
#define SOMEIPTP_E_INVALID_CALL 0xFF /** Invalid call for this dummy function */

/*------------------[Runtime Errors]-------------------*/

/** \brief Definition of DET error code SOMEIPTP_E_MESSAGE_TYPE */
/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00065,1 */
#if (defined SOMEIPTP_E_MESSAGE_TYPE)
#error SOMEIPTP_E_MESSAGE_TYPE already defined!
#endif /* #if (defined SOMEIPTP_E_MESSAGE_TYPE)*/
#define SOMEIPTP_E_MESSAGE_TYPE 0x04U /** The TP-Flag (of Message Type) was set to '0' */

/** \brief Definition of DET error code SOMEIPTP_E_INCONSISTENT_SEQUENCE */
/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00065,1 */
#if (defined SOMEIPTP_E_INCONSISTENT_SEQUENCE)
#error SOMEIPTP_E_INCONSISTENT_SEQUENCE already defined!
#endif /* #if (defined SOMEIPTP_E_INCONSISTENT_SEQUENCE)*/
#define SOMEIPTP_E_INCONSISTENT_SEQUENCE 0x05U /** Inconsistent subsequent segment received */

/** \brief Definition of DET error code SOMEIPTP_E_INCONSISTENT_HEADER */
/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00065,1 */
#if (defined SOMEIPTP_E_INCONSISTENT_HEADER)
#error SOMEIPTP_E_INCONSISTENT_HEADER already defined!
#endif /* #if (defined SOMEIPTP_E_INCONSISTENT_HEADER)*/
#define SOMEIPTP_E_INCONSISTENT_HEADER 0x06U /** Inconsistent header received */

/** \brief Definition of DET error code SOMEIPTP_E_DISASSEMBLY_INTERRUPT */
/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00065,1 */
#if (defined SOMEIPTP_E_DISASSEMBLY_INTERRUPT)
#error SOMEIPTP_E_DISASSEMBLY_INTERRUPT already defined!
#endif /* #if (defined SOMEIPTP_E_DISASSEMBLY_INTERRUPT)*/
#define SOMEIPTP_E_DISASSEMBLY_INTERRUPT 0x07U /** Disassembly Interrupt due to the upper layer */

/** \brief Definition of DET error code SOMEIPTP_E_ASSEMBLY_INTERRUPT */
/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00065,1 */
#if (defined SOMEIPTP_E_ASSEMBLY_INTERRUPT)
#error SOMEIPTP_E_ASSEMBLY_INTERRUPT already defined!
#endif /* #if (defined SOMEIPTP_E_ASSEMBLY_INTERRUPT)*/
#define SOMEIPTP_E_ASSEMBLY_INTERRUPT 0x08U /** Assembly Interrupt due to the upper layer */

/** \brief Definition of DET error code SOMEIPTP_E_MISSING_SOMEIPTP_HEADER */
/* !LINKSTO SomeIpTp.EB.Check_SomeIpTp_Header.RunTime_Error,1 */
#if (defined SOMEIPTP_E_MISSING_SOMEIPTP_HEADER)
#error SOMEIPTP_E_MISSING_SOMEIPTP_HEADER already defined!
#endif /* #if (defined SOMEIPTP_E_MISSING_SOMEIPTP_HEADER)*/
#define SOMEIPTP_E_MISSING_SOMEIPTP_HEADER 0x09U /** Missing Tp Header while TP-Flag is set to "1" */

/** \brief Definition of DET error code SOMEIPTP_E_ASSEMBLY_INTERRUPT for start of reception */
#if (defined SOMEIPTP_E_ASSEMBLY_INTERRUPT_SOR)
#error SOMEIPTP_E_ASSEMBLY_INTERRUPT_SOR already defined!
#endif /* #if (defined SOMEIPTP_E_ASSEMBLY_INTERRUPT_SOR)*/
#define SOMEIPTP_E_ASSEMBLY_INTERRUPT_SOR 0x80U /** Assembly Interrupt due to the upper layer */

/*------------------[Det Reporting functions]-------------------*/

#if(SOMEIPTP_DEV_ERROR_DETECT == STD_ON)

#if (defined SOMEIPTP_DET_REPORT_ERROR)
#error SOMEIPTP_DET_REPORT_ERROR already defined!
#endif /* #if (defined SOMEIPTP_DET_REPORT_ERROR)*/
#define SOMEIPTP_DET_REPORT_ERROR(ApiId, ErrorId) \
  (void) Det_ReportError(SOMEIPTP_MODULE_ID, SOMEIPTP_INSTANCE_ID, (ApiId), (ErrorId))

#endif /* SOMEIPTP_DEV_ERROR_DETECT == STD_ON */

#if (defined SOMEIPTP_DET_REPORT_RUNTIME_ERROR)
#error SOMEIPTP_DET_REPORT_RUNTIME_ERROR already defined!
#endif /* #if (defined SOMEIPTP_DET_REPORT_RUNTIME_ERROR)*/
#define SOMEIPTP_DET_REPORT_RUNTIME_ERROR(ApiId, ErrorId) \
  (void) Det_ReportRuntimeError(SOMEIPTP_MODULE_ID, SOMEIPTP_INSTANCE_ID, (ApiId), (ErrorId))

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define SOMEIPTP_START_SEC_CODE
#include <SomeIpTp_MemMap.h>

#if(SOMEIPTP_VERSION_INFO_API == STD_ON)
/**
 * \brief API to get the version information of SomeIpTp module.
 *
 * This service returns the version information of this module.
 *
 * \param[out] VersionInfo - Pointer to where to store the version information of this module.
 *
 * \retval void
 *
 * \ServiceID{0x01}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */

extern FUNC(void, SOMEIPTP_CODE) SomeIpTp_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) VersionInfo
);
#endif /* SOMEIPTP_VERSION_INFO_API */

/**
 * \brief Initializes the SomeIpTp module.
 *
 * This service initializes the SomeIpTp module. It shall be the first function of the module to be
 * called.
 *
 * \param[in] config - Base pointer to the configuration structure of the SOME/IP TP module.
 *
 * \retval void
 *
 * \ServiceID{0x02}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, SOMEIPTP_CODE) SomeIpTp_Init
(
  P2CONST(SomeIpTp_ConfigType, AUTOMATIC, SOMEIPTP_APPL_CONST) config
);

/**
 * \brief SomeIpTp module main function for Transmission.
 *
 * This function performs the processing of the AUTOSAR SOME/IP TP module's transmission activities.
 *
 *
 * \retval void
 *
 * \ServiceID{0x03}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, SOMEIPTP_CODE) SomeIpTp_MainFunctionTx
(
  void
);

/**
 * \brief SomeIpTp module main function for reception.
 *
 * This function performs the processing of the AUTOSAR SOME/IP TP module's reception activities.
 *
 * \retval void
 *
 * \ServiceID{0x04}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, SOMEIPTP_CODE) SomeIpTp_MainFunctionRx
(
  void
);

/**
 * \brief SomeIpTp module Transmit request function.
 *
 * This service requests transmission of a PDU.
 *
 * \param[in] TxPduId - Identifier of the PDU to be transmitted.
 * \param[in] PduInfoPtr - Length of and pointer to the PDU data and pointer to MetaData.
 *
 * \retval Std_ReturnType E_OK if transmit request has been accepted.
 *                        E_NOT_OK if not accepted.
 *
 * \ServiceID{0x49}
 * \Reentrancy{  Reentrant for different PduIds
 *               Non Reentrant for the same PduId  }
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_Transmit
(
  PduIdType TxPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_CONST) PduInfoPtr
);


/*------------------[Dummy PduR functions]-----------------*/
/**
 * \brief Dummy funtion for PduR.
 *
 * This function shall not be called, and if so it shall report developement error.
 *
 * \ServiceID{0xFF}
 */
extern FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_PduRIfTransmit
(
  PduIdType TxPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_CONST) PduInfoPtr
);

#define SOMEIPTP_STOP_SEC_CODE
#include <SomeIpTp_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef SOMEIPTP_H */
/*==================[end of file]===========================================*/
