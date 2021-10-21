/**
 * \file
 *
 * \brief AUTOSAR PduR
 *
 * This file contains the implementation of the AUTOSAR
 * module PduR.
 *
 * \version 5.3.46
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef PDUR_API_STATIC_H
#define PDUR_API_STATIC_H
/*==================[inclusions]============================================*/

#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* AUTOSAR Communication Stack types */
#include <PduR_Types.h>        /* type definitions of the PduR module */

/*==================[macros]================================================*/

/*------------------[AUTOSAR instance identification]-------------------------*/

#if (defined PDUR_INSTANCE_ID) /* to prevent double declaration */
#error PDUR_INSTANCE_ID already defined
#endif /* if (defined PDUR_INSTANCE_ID) */

/** \brief Id of instance of PDU Router provided to Det_ReportError()
 **/
#define PDUR_INSTANCE_ID 0x00U


/*------------------[list of AUTOSAR Service Ids]---------------------------*/

#if (defined PDUR_SID_INIT)
#error PDUR_SID_INIT already defined
#endif /* if (defined PDUR_SID_INIT) */

/** \brief Definition of service ID for PduR_Init */
#define PDUR_SID_INIT 0x01U

#if (defined PDUR_SID_GET_VER_INF)
#error PDUR_SID_GET_VER_INF already defined
#endif /* if (defined PDUR_SID_GET_VER_INF) */

/** \brief Definition of service ID for PduR_GetVersionInfo */
#define PDUR_SID_GET_VER_INF 0x02U

#if (defined PDUR_SID_GET_CONF_ID)
#error PDUR_SID_GET_CONF_ID already defined
#endif /* if (defined PDUR_SID_GET_CONF_ID) */

/** \brief Definition of service ID for PduR_GetConfigurationId */
#define PDUR_SID_GET_CONF_ID 0x10U


#if (defined PDUR_SID_LO_TXCONF)
#error PDUR_SID_LO_TXCONF already defined
#endif /* if (defined PDUR_SID_LO_TXCONF) */

/** \brief Definition of service ID for PduR_LoTxConfirmation */
#define PDUR_SID_LO_TXCONF 0x40U

#if (defined PDUR_SID_LOTP_STRT_OF_RCPTN)
#error PDUR_SID_LOTP_STRT_OF_RCPTN already defined
#endif /* if (defined PDUR_SID_LOTP_STRT_OF_RCPTN) */

/** \brief Definition of service ID for PduR_LoTpStartOfReception */
#define PDUR_SID_LOTP_STRT_OF_RCPTN 0x34U


#if (defined PDUR_SID_LOTP_COPY_RX_DATA)
#error PDUR_SID_LOTP_COPY_RX_DATA already defined
#endif /* if (defined PDUR_SID_LOTP_COPY_RX_DATA) */

/** \brief Definition of service ID for PduR_LoTpCopyRxData */
#define PDUR_SID_LOTP_COPY_RX_DATA  0x32U


#if (defined PDUR_SID_LOTP_RXIND)
#error PDUR_SID_LOTP_RXIND already defined
#endif /* if (defined PDUR_SID_LOTP_RXIND) */

/** \brief Definition of service ID for PduR_LoTpRxIndication */
#define PDUR_SID_LOTP_RXIND 0x33U


#if (defined PDUR_SID_LOTP_COPY_TX_DATA)
#error PDUR_SID_LOTP_COPY_TX_DATA already defined
#endif /* if (defined PDUR_SID_LOTP_COPY_TX_DATA) */

/** \brief Definition of service ID for PduR_LoTpCopyTxData */
#define PDUR_SID_LOTP_COPY_TX_DATA 0x36U


#if (defined PDUR_SID_LOTP_TX_CONF)
#error PDUR_SID_LOTP_TX_CONF already defined
#endif /* if (defined PDUR_SID_LOTP_TX_CONF) */

/** \brief Definition of service ID for PduR_LoTpTxConfirmation */
#define PDUR_SID_LOTP_TX_CONF 0x37U


#if (defined PDUR_SID_UP_CANCELTXREQ)
#error PDUR_SID_UP_CANCELTXREQ already defined
#endif /* if (defined PDUR_SID_UP_CANCELTXREQ) */

/** \brief Definition of service ID for PduR_UpCancelTransmit */
#define PDUR_SID_UP_CANCELTXREQ 0x1CU


#if (defined PDUR_SID_UP_CHANGEPARAREQ)
#error PDUR_SID_UP_CHANGEPARAREQ already defined
#endif /* if (defined PDUR_SID_UP_CHANGEPARAREQ) */

/** \brief Definition of service ID for PduR_UpChangeParameter */
#define PDUR_SID_UP_CHANGEPARAREQ 0x1DU


#if (defined PDUR_SID_UP_CANCELRXREQ)
#error PDUR_SID_UP_CANCELRXREQ already defined
#endif /* if (defined PDUR_SID_UP_CANCELRXREQ) */

/** \brief Definition of service ID for PduR_UpCancelReceive  */
#define PDUR_SID_UP_CANCELRXREQ 0x21U


#if (defined PDUR_SID_LO_RXIND)
#error PDUR_SID_LO_RXIND already defined
#endif /* if (defined PDUR_SID_LO_RXIND) */

/** \brief Definition of service ID for PduR_LoRxIndication */
#define PDUR_SID_LO_RXIND 0x42U


#if (defined PDUR_SID_LO_TRIGTX)
#error PDUR_SID_LO_TRIGTX already defined
#endif /* if (defined PDUR_SID_LO_TRIGTX) */

/** \brief Definition of service ID for PduR_LoTriggerTransmit */
#define PDUR_SID_LO_TRIGTX 0x41U


#if (defined PDUR_SID_UP_TX)
#error PDUR_SID_UP_TX already defined
#endif /* if (defined PDUR_SID_UP_TX) */

/** \brief Definition of service ID for PduR_UpTransmit */
#define PDUR_SID_UP_TX 0x14U

#if (defined PDUR_SID_ENABLE_ROUTING)
#error PDUR_SID_ENABLE_ROUTING already defined
#endif /* if (defined PDUR_SID_ENABLE_ROUTING) */

/** \brief Definition of service ID for PduR_EnableRouting*/
#define PDUR_SID_ENABLE_ROUTING 0x50U

#if (defined PDUR_SID_DISABLE_ROUTING)
#error PDUR_SID_DISABLE_ROUTING already defined
#endif /* if (defined PDUR_SID_DISABLE_ROUTING) */

/** \brief Definition of service ID for PduR_DisableRouting */
#define PDUR_SID_DISABLE_ROUTING 0x51U

/*==================[SIDs for internal handler functions]===================*/
#if (defined PDUR_SID_IFGW_RXIND_DF)
#error PDUR_SID_IFGW_RXIND_DF already defined
#endif /* if (defined PDUR_SID_IFGW_RXIND_DF) */

/** \brief Definition of service ID for internal handler function PduR_GateIfRxIndicationDf(DynPyld) */
#define PDUR_SID_IFGW_RXIND_DF 0x55U

#if (defined PDUR_SID_IFGW_RXIND_TF)
#error PDUR_SID_IFGW_RXIND_TF already defined
#endif /* if (defined PDUR_SID_IFGW_RXIND_TF) */

/** \brief Definition of service ID for internal handler function PduR_GateIfRxIndicationTf(DynPyld) */
#define PDUR_SID_IFGW_RXIND_TF 0x56U


#if (defined PDUR_SID_IFGW_RXIND_SB)
#error PDUR_SID_IFGW_RXIND_SB already defined
#endif /* if (defined PDUR_SID_IFGW_RXIND_SB) */

/** \brief Definition of service ID for internal handler function PduR_GateIfRxIndicationSb(DynPyld) */
#define PDUR_SID_IFGW_RXIND_SB 0x57U


#if (defined PDUR_SID_IFGW_TRIGTX_TF)
#error PDUR_SID_IFGW_TRIGTX_TF already defined
#endif /* if (defined PDUR_SID_IFGW_TRIGTX_TF) */

/** \brief Definition of service ID for internal handler function PduR_GateIfTriggerTransmitTf(DynPyld) */
#define PDUR_SID_IFGW_TRIGTX_TF 0x58U


#if (defined PDUR_SID_IFGW_TRIGTX_SB)
#error PDUR_SID_IFGW_TRIGTX_SB already defined
#endif /* if (defined PDUR_SID_IFGW_TRIGTX_SB) */

/** \brief Definition of service ID for internal handler function PduR_GateIfTriggerTransmitSb(DynPyld) */
#define PDUR_SID_IFGW_TRIGTX_SB 0x59U

/*------------------[PDU Router Error Codes ]-------------------------------*/

#if (defined PDUR_E_CONFIG_PTR_INVALID)
#error PDUR_E_CONFIG_PTR_INVALID already defined
#endif /* if (defined PDUR_E_CONFIG_PTR_INVALID) */

/** \brief Error code for invalid configuration pointer.
 **/
#define PDUR_E_CONFIG_PTR_INVALID 0x00U


#if (defined PDUR_E_INVALID_REQUEST)
#error PDUR_E_INVALID_REQUEST already defined
#endif /* if (defined PDUR_E_INVALID_REQUEST) */

/** \brief Error code if API service used without module initialization or
 ** PduR_Init called in any state other than PDUR_UNINIT.
 **/
#define PDUR_E_INVALID_REQUEST 0x01U


#if (defined PDUR_E_PDU_ID_INVALID)
#error PDUR_E_PDU_ID_INVALID already defined
#endif /* if (defined PDUR_E_PDU_ID_INVALID) */

/** \brief Error code if invalid PDU identifier has been passed to a public API function.
 **/
#define PDUR_E_PDU_ID_INVALID 0x02U


#if (defined PDUR_E_TP_TX_REQ_REJECTED)
#error PDUR_E_TP_TX_REQ_REJECTED already defined
#endif /* if (defined PDUR_E_TP_TX_REQ_REJECTED) */

/** \brief Error code if TP module rejects a transmit request for a valid PDU identifier.
 **/
#define PDUR_E_TP_TX_REQ_REJECTED 0x03U

#if (defined PDUR_E_ROUTING_PATH_GROUP_ID_INVALID)
#error PDUR_E_ROUTING_PATH_GROUP_ID_INVALID already defined
#endif /* if (defined PDUR_ROUTINGPATH_GROUP_ID_INVALID) */

/** \brief Error code if invalid Routing Path Group identifier is passed to
** PduR_DisableRouting() and PduR_EnableRouting() functions.
 **/
#define PDUR_E_ROUTING_PATH_GROUP_ID_INVALID 0x08U


#if (defined PDUR_E_NULL_POINTER)
#error PDUR_E_NULL_POINTER already defined
#endif /* if (defined PDUR_E_NULL_POINTER) */

/** \brief Pointer parameter is null. Note that specific API calls may disable this error.
 **/
#define PDUR_E_NULL_POINTER 0x09U


#if (defined PDUR_E_PDU_INSTANCES_LOST)
#error PDUR_E_PDU_INSTANCES_LOST already defined
#endif /* if (defined PDUR_E_PDU_INSTANCES_LOST) */

/** \brief Loss of a PDU instance (FIFO flushed because of an overrun).
 **/
#define PDUR_E_PDU_INSTANCES_LOST 0x0AU


/*==================[type definitions]======================================*/


/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/
#define PDUR_START_SEC_VAR_INIT_UNSPECIFIED
#include <PduR_MemMap.h>

/** \brief Variable holding the State of the PDU Router **/
extern VAR(PduR_StateType, PDUR_VAR) PduR_State;

#define PDUR_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <PduR_MemMap.h>
/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef PDUR_API_STATIC_H */
/*==================[end of file]===========================================*/
