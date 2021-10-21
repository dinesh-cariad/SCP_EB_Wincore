/**
 * \file
 *
 * \brief AUTOSAR LdCom
 *
 * This file contains the implementation of the AUTOSAR
 * module LdCom.
 *
 * \version 1.0.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef LDCOM_H
#define LDCOM_H


/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00002,1 */
#include <ComStack_Types.h>

/*==========================================================================*/
/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00002,1 */
#include <LdCom_Types.h>         /* Module public types */
/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00002,1 */
#include <LdCom_Cfg.h>           /* Module configurable parameters */
#include <LdCom_Cbk.h>           /* Module interface */
#include <LdCom_Version.h>       /* Module Version Info */

/* Exclude post-build-time configuration include file if requested to do so */
#if (!defined LDCOM_NO_PBCFG_REQUIRED) && (!defined LDCOM_NO_CFG_REQUIRED)
#if (LDCOM_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <LdCom_PBcfg.h>
#endif /* (LDCOM_PBCFGM_SUPPORT_ENABLED == STD_OFF) */
#endif /* (!defined LDCOM_NO_PBCFG_REQUIRED) && (!defined LDCOM_NO_CFG_REQUIRED) */

/*==================[macros]================================================*/

#if (defined LdCom_Transmit)
#error LdCom_Transmit already defined
#endif
/* Map the API LdCom_Transmit() to LdCom_IfTransmit():
   The mapping is made to go conform with the Rte module calling LdCom_Transmit with a signal ID
   related to communication interface I-PDUs. Rte calls of a signal ID related to a transport
   protocol I-PDU are made by dedicated TP-API LdCom_TpTransmit. */
#define LdCom_Transmit(Id,PduInfoPtr) LdCom_IfTransmit((Id),(PduInfoPtr))

#if (defined LDCOM_INSTANCE_ID)
#error LDCOM_INSTANCE_ID already defined
#endif
/* Id of instance of LdCom provided to Det_ReportError() */
#define LDCOM_INSTANCE_ID 0x00U

#if (defined LDCOM_UNINIT)
#error LDCOM_UNINIT already defined
#endif
/* UnInit state of LdCom_StateType */
#define LDCOM_UNINIT  0U

#if (defined LDCOM_ONLINE)
#error LDCOM_ONLINE already defined
#endif
/* Init state of LdCom_StateType */
#define LDCOM_ONLINE  1U


#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
/* API Service ID's */

#if (defined LDCOM_SID_INIT)
#error LDCOM_SID_INIT already defined
#endif
/** \brief API Service ID for LdCom_Init() */
#define LDCOM_SID_INIT                         0x01U

#if (defined LDCOM_SID_DEINIT)
#error LDCOM_SID_DEINIT already defined
#endif
/** \brief API Service ID for LdCom_DeInit() */
#define LDCOM_SID_DEINIT                       0x02U

#if (defined LDCOM_SID_GETVERSIONINFO)
#error LDCOM_SID_GETVERSIONINFO already defined
#endif
/** \brief API Service ID for LdCom_GetVersioninfo() */
#define LDCOM_SID_GETVERSIONINFO               0x03U

#if (defined LDCOM_SID_IFTRANSMIT)
#error LDCOM_SID_IFTRANSMIT already defined
#endif
/** \brief API Service ID for LdCom_IfTransmit() */
#define LDCOM_SID_IFTRANSMIT                   0x05U

#if (defined LDCOM_SID_STARTOFRECEPTION)
#error LDCOM_SID_STARTOFRECEPTION already defined
#endif
/** \brief API Service ID for LdCom_StartOfReception() */
#define LDCOM_SID_STARTOFRECEPTION             0x46U

#if (defined LDCOM_SID_COPYRXDATA)
#error LDCOM_SID_COPYRXDATA already defined
#endif
/** \brief API Service ID for LdCom_CopyRxData() */
#define LDCOM_SID_COPYRXDATA                   0x44U

#if (defined LDCOM_SID_TPRXINDICATION)
#error LDCOM_SID_TPRXINDICATION already defined
#endif
/** \brief API Service ID for LdCom_TpRxIndication() */
#define LDCOM_SID_TPRXINDICATION               0x45U

#if (defined LDCOM_SID_COPYTXDATA)
#error LDCOM_SID_COPYTXDATA already defined
#endif
/** \brief API Service ID for LdCom_CopyTxData() */
#define LDCOM_SID_COPYTXDATA                   0x43U

#if (defined LDCOM_SID_TPTXCONFIRMATION)
#error LDCOM_SID_TPTXCONFIRMATION already defined
#endif
/** \brief API Service ID for LdCom_TpTxConfirmation() */
#define LDCOM_SID_TPTXCONFIRMATION             0x48U

#if (defined LDCOM_SID_RXINDICATION)
#error LDCOM_SID_RXINDICATION already defined
#endif
/** \brief API Service ID for LdCom_RxIndication() */
#define LDCOM_SID_RXINDICATION                 0x42U

#if (defined LDCOM_SID_TRIGGERTRANSMIT)
#error LDCOM_SID_TRIGGERTRANSMIT already defined
#endif
/** \brief API Service ID for LdCom_TriggerTransmit() */
#define LDCOM_SID_TRIGGERTRANSMIT              0x41U

#if (defined LDCOM_SID_TXCONFIRMATION)
#error LDCOM_SID_TXCONFIRMATION already defined
#endif
/** \brief API Service ID for LdCom_TxConfirmation() */
#define LDCOM_SID_TXCONFIRMATION               0x40U

#if (defined LDCOM_SID_TPTRANSMIT)
#error LDCOM_SID_TPTRANSMIT already defined
#endif
/** \brief API Service ID for LDCOM_TpTransmit() */
#define LDCOM_SID_TPTRANSMIT                   0x04U
#endif /* (LDCOM_DEV_ERROR_DETECT == STD_ON) */

/*==================[Error codes]===========================================*/
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
/* Development error values of type uint8 */

#if (defined LDCOM_E_UNINIT)
#error LDCOM_E_UNINIT already defined
#endif
/** \brief Error code if any other API service, except LdCom_GetVersionInfo is called before the
 ** AUTOSAR LdCom module was initialized with LdCom_Init or after a call to LdCom_Deinit */
#define LDCOM_E_UNINIT              0x02U

#if (defined LDCOM_E_PARAM_POINTER)
#error LDCOM_E_PARAM_POINTER already defined
#endif
/** \brief API service called with a NULL pointer. In case of this error, the API service shall
 ** return immediately without any further action, except for reporting this development error. */
#define LDCOM_E_PARAM_POINTER       0x03U

#if (defined LDCOM_E_INVALID_PDU_SDU_ID)
#error LDCOM_E_INVALID_PDU_SDU_ID already defined
#endif
/** \brief API service called with wrong PDU-ID */
#define LDCOM_E_INVALID_PDU_SDU_ID  0x04U

#if (defined LDCOM_E_INVALID_SIGNAL_ID)
#error LDCOM_E_INVALID_SIGNAL_ID already defined
#endif
/** \brief API service called with wrong Signal-ID */
#define LDCOM_E_INVALID_SIGNAL_ID   0x05U

#if (defined LDCOM_E_INIT_FAILED)
#error LDCOM_E_INIT_FAILED already defined
#endif
/** \brief Invalid configuration set selection */
#define LDCOM_E_INIT_FAILED         0x06U

#endif /* (LDCOM_DEV_ERROR_DETECT == STD_ON) */

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define LDCOM_START_SEC_CODE
#include <LdCom_MemMap.h>

#if (LDCOM_VERSION_INFO_API == STD_ON)
/** \brief Get version information of the LdCom module.
**
** This service returns the version information of this module. The version
** information includes:
**  - Module Id
**  - Vendor Id
**  - Vendor specific version numbers
**
** \param[out] versioninfo Pointer to where to store the version
**                         information of this module.
**
** \ServiceID{0x03}
** \Reentrancy{Non-Reentrant}
** \Synchronicity{Synchronous}
*/
extern FUNC(void, LDCOM_CODE) LdCom_GetVersionInfo
(
P2VAR(Std_VersionInfoType, AUTOMATIC, LDCOM_APPL_DATA) versioninfo
);
#endif /* LDCOM_VERSION_INFO_API == STD_ON */

/** \brief Initialize the LdCom module.
**
** This service initializes internal and external interfaces and
** variables of the AUTOSAR LdCom module for further processing.
**
** \param[in] config Points to the implementation specific structure
**
** \ServiceID{0x01}
** \Reentrancy{Non-Reentrant}
** \Synchronicity{Synchronous}
*/
extern FUNC(void, LDCOM_CODE) LdCom_Init
(
P2CONST(LdCom_ConfigType, AUTOMATIC, LDCOM_APPL_CONST) config
);

/** \brief Deinitialize the LdCom module.
**
** With a call to LdCom_DeInit the AUTOSAR LdCom module is put into
** a de-initialized state.
**
** \ServiceID{0x02}
** \Reentrancy{Non-Reentrant}
** \Synchronicity{Synchronous}
*/
extern FUNC(void, LDCOM_CODE) LdCom_DeInit(void);

/** \brief Initiate a transmission of a signal to transport protocol modules.
**
** This function transmits the signal data given by \p Id and
** \p PduInfoPtr by replacing the Signal ID by the according PDU ID and
** invoking of PduR_LdComTpTransmit.
**
** \param[in]  Id           ID of the signal to be sent.
** \param[in]  PduInfoPtr   Length and pointer to the buffer of the Signal.
**
** \return Standard Return Code
** \retval E_OK Request is accepted by the destination module; transmission is continued.
** \retval E_NOT_OK Request is not accepted by the destination module; transmission is aborted.
**
** \ServiceID{0x05}
** \Reentrancy{Non Reentrant for the same HandleId, otherwise Reentrant}
** \Synchronicity{Asynchronous}
**
*/
extern FUNC(Std_ReturnType, LDCOM_CODE) LdCom_TpTransmit
(
PduIdType Id,
/* Remark: according to bugzilla# 63928; using pointer to const */
P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
);

/** \brief Initiate a transmission of a signal to communication interface modules.
**
** This function transmits the signal data given by \p Id and
** \p PduInfoPtr by replacing the Signal ID by the according PDU ID and
** invoking of PduR_LdComTransmit.
**
** \param[in]  Id           ID of the signal to be sent.
** \param[in]  PduInfoPtr   Length and pointer to the buffer of the Signal.
**
** \return Standard Return Code
** \retval E_OK Request is accepted by the destination module; transmission is continued.
** \retval E_NOT_OK Request is not accepted by the destination module; transmission is aborted.
**
** \ServiceID{0x05}
** \Reentrancy{Non Reentrant for the same HandleId, otherwise Reentrant}
** \Synchronicity{Synchronous}
**
*/
extern FUNC(Std_ReturnType, LDCOM_CODE) LdCom_IfTransmit
(
PduIdType Id,
/* Remark: according to bugzilla# 63928; using pointer to const */
P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
);


/** \brief Validate configuration
**
** Checks if the post build configuration is valid.
** A configuration is invalid if
** - the platform signature does not match.
** - the published information signature does not match.
** - the link time signature does not match.
** - the compile time signature does not match.
** - the function is called with a null pointer.
**
** \param[in] ConfigPtr Pointer to configuration structure that holds the
**                      LdCom module post-build-time configuration data.
**
** \return Function execution success status
** \retval E_OK  the provided module configuration is valid
** \retval E_NOT_OK  the provided module configuration is invalid
**
** \Reentrancy{Reentrant}
** \Synchronicity{Synchronous}
**/
extern FUNC(Std_ReturnType, LDCOM_CODE) LdCom_IsValidConfig
(
P2CONST(void, AUTOMATIC, LDCOM_APPL_CONST) ConfigPtr
);

#define LDCOM_STOP_SEC_CODE
#include <LdCom_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef LDCOM_H */
/*==================[end of file]===========================================*/
