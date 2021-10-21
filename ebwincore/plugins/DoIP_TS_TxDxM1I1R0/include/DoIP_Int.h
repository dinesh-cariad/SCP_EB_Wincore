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

#ifndef DOIP_INT_H
#define DOIP_INT_H

#include <TSAutosar.h> /* EB specific standard types */
#include <ComStack_Types.h>
#include <SoAd.h>
#include <DoIP_Types_Int.h> /* Module XGEN generated internal types file */

/*==================[includes]==============================================*/
#include <DoIP_Cfg.h>

#if (DOIP_RTE_USED == STD_ON)
#include <Rte_DoIP_Type.h>
#endif

#if (defined TS_RELOCATABLE_CFG_ENABLE)
#error TS_RELOCATABLE_CFG_ENABLE already defined!
#endif /* #if( defined TS_RELOCATABLE_CFG_ENABLE)*/
#define TS_RELOCATABLE_CFG_ENABLE DOIP_RELOCATABLE_CFG_ENABLE

#include <TSPBConfig_Access.h>
#include <TSPBConfig_Signature.h>           /* relocatable post-build macros */

/*==================[macro checks]==========================================*/

/*==================[macros]================================================*/

#if(DOIP_DEV_ERROR_DETECT == STD_ON)
#if (defined DOIP_DET_REPORT_ERROR)
#error DOIP_DET_REPORT_ERROR is already defined
#endif
#define DOIP_DET_REPORT_ERROR(ApiId, ErrorId) \
  (void) Det_ReportError(DOIP_MODULE_ID, DOIP_INSTANCE_ID, (ApiId), (ErrorId))
#endif /* DOIP_DEV_ERROR_DETECT == STD_ON */

#if (defined DOIP_MIN)
#error DOIP_MIN is already defined
#endif
#define DOIP_MIN(a, b) (((a)<(b))?(a):(b))

#if (defined DOIP_MAX)
#error DOIP_MAX is already defined
#endif
#define DOIP_MAX(a, b) (((a)>(b))?(a):(b))

#define DOIP_INVALID_TCP_CON_IDX (DOIP_MAX_PARALLEL_TESTER + 1U)

#if (defined DOIP_UINT32_MAX)
#error DOIP_UINT32_MAX is already defined
#endif
#define DOIP_UINT32_MAX 0xFFFFFFFFU

#if (defined DOIP_INVALID_TIMEOUT_VALUE)
#error DOIP_INVALID_TIMEOUT_VALUE is already defined
#endif
#define DOIP_INVALID_TIMEOUT_VALUE 0xFFFFFFFFU

#if (defined DOIP_MAX_MEAS_COUNTER)
#error DOIP_MAX_MEAS_COUNTER is already defined
#endif
#define DOIP_MAX_MEAS_COUNTER 0xFFFFFFFFU

/* === General macros === */
/* Protocol version */
#if (defined DOIP_PR_VERSION)
#error DOIP_PR_VERSION is already defined
#endif
#define DOIP_PR_VERSION 0x02U /* DoIP ISO 13400-2:2012 */

#if (defined DOIP_PR_INV_VERSION)
#error DOIP_PR_INV_VERSION is already defined
#endif
#define DOIP_PR_INV_VERSION 0xFDU /* DOIP_PR_VERSION xor 0xFF */

#if (defined DOIP_DEFAULT_PR_VERSION)
#error DOIP_DEFAULT_PR_VERSION is already defined
#endif
#define DOIP_DEFAULT_PR_VERSION 0xFFU /* DoIP ISO 13400-2:2012 */

#if (defined DOIP_GATEWAY)
#error DOIP_GATEWAY is already defined
#endif
#define DOIP_GATEWAY 0x00U

#if (defined DOIP_NODE)
#error DOIP_NODE is already defined
#endif
#define DOIP_NODE 0x01U

/* Macros used for DoIPResponseBeforeRoutingActivation */
/* Diag Nack code 0x06 will not be sent before Routing Activation */
#if (defined DOIP_DISABLE_DIAG_NACK_0x06)
#error DOIP_DISABLE_DIAG_NACK_0x06 is already defined
#endif
#define DOIP_DISABLE_DIAG_NACK_0x06 0x00U

/* Diag Nack messages will always be sent */
#if (defined DOIP_ENABLE_ALL_DIAG_NACK)
#error DOIP_ENABLE_ALL_DIAG_NACK is already defined
#endif
#define DOIP_ENABLE_ALL_DIAG_NACK 0x01U

#if (defined DOIP_MAX_TIMEOUT_VALUES)
#error DOIP_MAX_TIMEOUT_VALUES is already defined
#endif
#define DOIP_MAX_TIMEOUT_VALUES (                                                 \
                                  DOIP_MAX_UDP_SOCKET_CONNECTIONS +               \
                                  (DOIP_MAX_PARALLEL_TESTER + 1U) +               \
                                  (DOIP_MAX_PARALLEL_TESTER + 1U) +               \
                                  DOIP_MAX_VEHICLE_ANNOUNCEMENT_CON               \
                                )

/* === Inactivity Timer index start position in timer array. === */
#if (defined DOIP_INACTIVITY_TIMER_INDEX_START)
#error DOIP_INACTIVITY_TIMER_INDEX_START is already defined
#endif
#define DOIP_INACTIVITY_TIMER_INDEX_START DOIP_MAX_UDP_SOCKET_CONNECTIONS

/* === Alive check start position in timer array. === */
#if (defined DOIP_ALIVE_CHECK_TIMER_INDEX_START)
#error DOIP_ALIVE_CHECK_TIMER_INDEX_START is already defined
#endif
#define DOIP_ALIVE_CHECK_TIMER_INDEX_START (                                      \
                                             DOIP_INACTIVITY_TIMER_INDEX_START +  \
                                             (DOIP_MAX_PARALLEL_TESTER + 1U)      \
                                           )

/* === Vehicle announcement start in timer array. position === */
#if (defined DOIP_ANNOUNCEMENT_TIMER_INDEX_START)
#error DOIP_ANNOUNCEMENT_TIMER_INDEX_START is already defined
#endif
#define DOIP_ANNOUNCEMENT_TIMER_INDEX_START (                                      \
                                              DOIP_ALIVE_CHECK_TIMER_INDEX_START + \
                                              DOIP_MAX_PARALLEL_TESTER + 1U        \
                                            )

/* === Helper macros for finding corresponding index in TimeoutArr === */
#define DOIP_UDP_RELEASE_TIMER_INDEX(UdpConIdx) (UdpConIdx)
#define DOIP_INACTIVITY_TIMER_INDEX(TcpConIdx) (DOIP_INACTIVITY_TIMER_INDEX_START + TcpConIdx)
#define DOIP_ALIVE_CHECK_TIMER_INDEX(TcpConIdx) (DOIP_ALIVE_CHECK_TIMER_INDEX_START + TcpConIdx)
#define DOIP_ANNOUNCEMENT_TIMER_INDEX(AnnConIdx) (DOIP_ANNOUNCEMENT_TIMER_INDEX_START + AnnConIdx)


/* === Generic header size === */
#if (defined DOIP_GEN_HEADER_SIZE)
#error DOIP_GEN_HEADER_SIZE is already defined
#endif
#define DOIP_GEN_HEADER_SIZE ((uint16)8U)

/* === DHCP host name size === */
#if (defined DOIP_DHCP_MIN_HOSTNAME_SIZE)
#error DOIP_DHCP_MIN_HOSTNAME_SIZE is already defined
#endif
#define DOIP_DHCP_MIN_HOSTNAME_SIZE 5U /* Minimum length: ASCII string 'DoIP-' */

#if (defined DOIP_DHCP_MAX_HOSTNAME_SIZE)
#error DOIP_DHCP_MAX_HOSTNAME_SIZE is already defined
#endif
#define DOIP_DHCP_MAX_HOSTNAME_SIZE 22U /* Maximum length: ASCII string 'DoIP-' + 17 byte VIN */

/* === Byte position: Generic header position within SoAd payload === */
#if (defined DOIP_GEN_HEADER_B0_PR_VERSION)
#error DOIP_GEN_HEADER_B0_PR_VERSION is already defined
#endif
#define DOIP_GEN_HEADER_B0_PR_VERSION 0U

#if (defined DOIP_GEN_HEADER_B1_PR_INV_VERSION)
#error DOIP_GEN_HEADER_B1_PR_INV_VERSION is already defined
#endif
#define DOIP_GEN_HEADER_B1_PR_INV_VERSION 1U

#if (defined DOIP_GEN_HEADER_B2_TYPE_MSB)
#error DOIP_GEN_HEADER_B2_TYPE_MSB is already defined
#endif
#define DOIP_GEN_HEADER_B2_TYPE_MSB 2U

#if (defined DOIP_GEN_HEADER_B3_TYPE_LSB)
#error DOIP_GEN_HEADER_B3_TYPE_LSB is already defined
#endif
#define DOIP_GEN_HEADER_B3_TYPE_LSB 3U

#if (defined DOIP_GEN_HEADER_B4_LENGTH_3_MSB)
#error DOIP_GEN_HEADER_B4_LENGTH_3_MSB is already defined
#endif
#define DOIP_GEN_HEADER_B4_LENGTH_3_MSB 4U

#if (defined DOIP_GEN_HEADER_B5_LENGTH_2)
#error DOIP_GEN_HEADER_B5_LENGTH_2 is already defined
#endif
#define DOIP_GEN_HEADER_B5_LENGTH_2 5U

#if (defined DOIP_GEN_HEADER_B6_LENGTH_1)
#error DOIP_GEN_HEADER_B6_LENGTH_1 is already defined
#endif
#define DOIP_GEN_HEADER_B6_LENGTH_1 6U

#if (defined DOIP_GEN_HEADER_B7_LENGTH_0_LSB)
#error DOIP_GEN_HEADER_B7_LENGTH_0_LSB is already defined
#endif
#define DOIP_GEN_HEADER_B7_LENGTH_0_LSB 7U

#if (defined DOIP_GEN_HEADER_B8_PAYLOAD)
#error DOIP_GEN_HEADER_B8_PAYLOAD is already defined
#endif
#define DOIP_GEN_HEADER_B8_PAYLOAD 8U

/* === Generic header negative acknowledge code values === */
#if (defined DOIP_NACK_CODE_INCORRECT_PATTERN)
#error DOIP_NACK_CODE_INCORRECT_PATTERN is already defined
#endif
#define DOIP_NACK_CODE_INCORRECT_PATTERN 0x00U

#if (defined DOIP_NACK_CODE_UNKNOWN_PL_TYPE)
#error DOIP_NACK_CODE_UNKNOWN_PL_TYPE is already defined
#endif
#define DOIP_NACK_CODE_UNKNOWN_PL_TYPE 0x01U

#if (defined DOIP_NACK_CODE_MSG_TO_LONG)
#error DOIP_NACK_CODE_MSG_TO_LONG is already defined
#endif
#define DOIP_NACK_CODE_MSG_TO_LONG 0x02U

#if (defined DOIP_NACK_CODE_OUT_OF_MEMORY)
#error DOIP_NACK_CODE_OUT_OF_MEMORY is already defined
#endif
#define DOIP_NACK_CODE_OUT_OF_MEMORY 0x03U

#if (defined DOIP_NACK_CODE_INVALID_PL_LENGTH)
#error DOIP_NACK_CODE_INVALID_PL_LENGTH is already defined
#endif
#define DOIP_NACK_CODE_INVALID_PL_LENGTH 0x04U

/* === Payload type === */
#if (defined DOIP_PL_TYPE_GEN_HEADER_NACK)
#error DOIP_PL_TYPE_GEN_HEADER_NACK is already defined
#endif
#define DOIP_PL_TYPE_GEN_HEADER_NACK 0x0000U

#if (defined DOIP_PL_TYPE_VEHICLE_ID_REQ)
#error DOIP_PL_TYPE_VEHICLE_ID_REQ is already defined
#endif
#define DOIP_PL_TYPE_VEHICLE_ID_REQ 0x0001U

#if (defined DOIP_PL_TYPE_VEHICLE_ID_EID_REQ)
#error DOIP_PL_TYPE_VEHICLE_ID_EID_REQ is already defined
#endif
#define DOIP_PL_TYPE_VEHICLE_ID_EID_REQ 0x0002U

#if (defined DOIP_PL_TYPE_VEHICLE_ID_VIN_REQ)
#error DOIP_PL_TYPE_VEHICLE_ID_VIN_REQ is already defined
#endif
#define DOIP_PL_TYPE_VEHICLE_ID_VIN_REQ 0x0003U

#if (defined DOIP_PL_TYPE_VEHICLE_ID_RESP_ANN)
#error DOIP_PL_TYPE_VEHICLE_ID_RESP_ANN is already defined
#endif
#define DOIP_PL_TYPE_VEHICLE_ID_RESP_ANN 0x0004U

#if (defined DOIP_PL_TYPE_ROUTE_ACT_REQ)
#error DOIP_PL_TYPE_ROUTE_ACT_REQ is already defined
#endif
#define DOIP_PL_TYPE_ROUTE_ACT_REQ 0x0005U

#if (defined DOIP_PL_TYPE_ROUTE_ACT_RESP)
#error DOIP_PL_TYPE_ROUTE_ACT_RESP is already defined
#endif
#define DOIP_PL_TYPE_ROUTE_ACT_RESP 0x0006U

#if (defined DOIP_PL_TYPE_ALIVE_REQ)
#error DOIP_PL_TYPE_ALIVE_REQ is already defined
#endif
#define DOIP_PL_TYPE_ALIVE_REQ 0x0007U

#if (defined DOIP_PL_TYPE_ALIVE_RESP)
#error DOIP_PL_TYPE_ALIVE_RESP is already defined
#endif
#define DOIP_PL_TYPE_ALIVE_RESP 0x0008U


#if (defined DOIP_PL_TYPE_ENTITY_STATUS_REQ)
#error DOIP_PL_TYPE_ENTITY_STATUS_REQ is already defined
#endif
#define DOIP_PL_TYPE_ENTITY_STATUS_REQ 0x4001U

#if (defined DOIP_PL_TYPE_ENTITY_STATUS_RESP)
#error DOIP_PL_TYPE_ENTITY_STATUS_RESP is already defined
#endif
#define DOIP_PL_TYPE_ENTITY_STATUS_RESP 0x4002U

#if (defined DOIP_PL_TYPE_POWERMODE_REQ)
#error DOIP_PL_TYPE_POWERMODE_REQ is already defined
#endif
#define DOIP_PL_TYPE_POWERMODE_REQ 0x4003U

#if (defined DOIP_PL_TYPE_POWERMODE_RESP)
#error DOIP_PL_TYPE_POWERMODE_RESP is already defined
#endif
#define DOIP_PL_TYPE_POWERMODE_RESP 0x4004U


#if (defined DOIP_PL_TYPE_DIAG_MSG)
#error DOIP_PL_TYPE_DIAG_MSG is already defined
#endif
#define DOIP_PL_TYPE_DIAG_MSG 0x8001U

#if (defined DOIP_PL_TYPE_DIAG_ACK)
#error DOIP_PL_TYPE_DIAG_ACK is already defined
#endif
#define DOIP_PL_TYPE_DIAG_ACK 0x8002U

#if (defined DOIP_PL_TYPE_DIAG_NACK)
#error DOIP_PL_TYPE_DIAG_NACK is already defined
#endif
#define DOIP_PL_TYPE_DIAG_NACK 0x8003U

/* === Payload length and message size === */
#if (defined DOIP_PL_LENGTH_GEN_HEADER_NACK)
#error DOIP_PL_LENGTH_GEN_HEADER_NACK is already defined
#endif
#define DOIP_PL_LENGTH_GEN_HEADER_NACK 1U

#if (defined DOIP_PL_LENGTH_VEHICLE_ID_REQ)
#error DOIP_PL_LENGTH_VEHICLE_ID_REQ is already defined
#endif
#define DOIP_PL_LENGTH_VEHICLE_ID_REQ 0U

#if (defined DOIP_PL_LENGTH_VEHICLE_ID_EID_REQ)
#error DOIP_PL_LENGTH_VEHICLE_ID_EID_REQ is already defined
#endif
#define DOIP_PL_LENGTH_VEHICLE_ID_EID_REQ 6U

#if (defined DOIP_PL_LENGTH_VEHICLE_ID_VIN_REQ)
#error DOIP_PL_LENGTH_VEHICLE_ID_VIN_REQ is already defined
#endif
#define DOIP_PL_LENGTH_VEHICLE_ID_VIN_REQ 17U

#if (defined DOIP_PL_LENGTH_VEHICLE_ID_RESP_ANN)
#error DOIP_PL_LENGTH_VEHICLE_ID_RESP_ANN is already defined
#endif
#define DOIP_PL_LENGTH_VEHICLE_ID_RESP_ANN 32U

#if (defined DOIP_PL_LENGTH_VEHICLE_ID_RESP_ANN_EXT)
#error DOIP_PL_LENGTH_VEHICLE_ID_RESP_ANN_EXT is already defined
#endif
#define DOIP_PL_LENGTH_VEHICLE_ID_RESP_ANN_EXT 33U /* Extended VID response */

#if (defined DOIP_PL_LENGTH_ROUTE_ACT_REQ)
#error DOIP_PL_LENGTH_ROUTE_ACT_REQ is already defined
#endif
#define DOIP_PL_LENGTH_ROUTE_ACT_REQ 7U

#if (defined DOIP_PL_LENGTH_ROUTE_ACT_REQ_OEM)
#error DOIP_PL_LENGTH_ROUTE_ACT_REQ_OEM is already defined
#endif
#define DOIP_PL_LENGTH_ROUTE_ACT_REQ_OEM 11U

#if (defined DOIP_PL_LENGTH_ROUTE_ACT_RESP)
#error DOIP_PL_LENGTH_ROUTE_ACT_RESP is already defined
#endif
#define DOIP_PL_LENGTH_ROUTE_ACT_RESP 9U

#if (defined DOIP_PL_LENGTH_ROUTE_ACT_RESP_OEM)
#error DOIP_PL_LENGTH_ROUTE_ACT_RESP_OEM is already defined
#endif
#define DOIP_PL_LENGTH_ROUTE_ACT_RESP_OEM 13U

#if (defined DOIP_PL_LENGTH_ALIVE_REQ)
#error DOIP_PL_LENGTH_ALIVE_REQ is already defined
#endif
#define DOIP_PL_LENGTH_ALIVE_REQ 0U

#if (defined DOIP_PL_LENGTH_ALIVE_RESP)
#error DOIP_PL_LENGTH_ALIVE_RESP is already defined
#endif
#define DOIP_PL_LENGTH_ALIVE_RESP 2U


#if (defined DOIP_PL_LENGTH_ENTITY_STATUS_REQ)
#error DOIP_PL_LENGTH_ENTITY_STATUS_REQ is already defined
#endif
#define DOIP_PL_LENGTH_ENTITY_STATUS_REQ 0U

#if (defined DOIP_PL_LENGTH_POWERMODE_REQ)
#error DOIP_PL_LENGTH_POWERMODE_REQ is already defined
#endif
#define DOIP_PL_LENGTH_POWERMODE_REQ 0U

#if (defined DOIP_PL_LENGTH_POWERMODE_RESP)
#error DOIP_PL_LENGTH_POWERMODE_RESP is already defined
#endif
#define DOIP_PL_LENGTH_POWERMODE_RESP 1U


#if (defined DOIP_PL_LENGTH_DIAG_MIN)
#error DOIP_PL_LENGTH_DIAG_MIN is already defined
#endif
#define DOIP_PL_LENGTH_DIAG_MIN 5U /* Minimum diagnostic payload length. */

#if (defined DOIP_PL_LENGTH_DIAG_ACK_NACK)
#error DOIP_PL_LENGTH_DIAG_ACK_NACK is already defined
#endif
#define DOIP_PL_LENGTH_DIAG_ACK_NACK 5U

#if (defined DOIP_PL_LENGTH_DIAG_NACK)
#error DOIP_PL_LENGTH_DIAG_NACK is already defined
#endif
#define DOIP_PL_LENGTH_DIAG_NACK 5U

/* === Byte position: Vehicle identification response === */
#if (defined DOIP_VID_RESP_B8_VIN_16_MSB)
#error DOIP_VID_RESP_B8_VIN_16_MSB is already defined
#endif
#define DOIP_VID_RESP_B8_VIN_16_MSB 8U

#if (defined DOIP_VID_RESP_B24_VIN_0_LSB)
#error DOIP_VID_RESP_B24_VIN_0_LSB is already defined
#endif
#define DOIP_VID_RESP_B24_VIN_0_LSB 24U

#if (defined DOIP_VID_RESP_B25_LOG_ADDR_1_MSB)
#error DOIP_VID_RESP_B25_LOG_ADDR_1_MSB is already defined
#endif
#define DOIP_VID_RESP_B25_LOG_ADDR_1_MSB 25U

#if (defined DOIP_VID_RESP_B26_LOG_ADDR_0_LSB)
#error DOIP_VID_RESP_B26_LOG_ADDR_0_LSB is already defined
#endif
#define DOIP_VID_RESP_B26_LOG_ADDR_0_LSB 26U

#if (defined DOIP_VID_RESP_B27_EID_6_MSB)
#error DOIP_VID_RESP_B27_EID_6_MSB is already defined
#endif
#define DOIP_VID_RESP_B27_EID_6_MSB 27U

#if (defined DOIP_VID_RESP_B32_EID_0_LSB)
#error DOIP_VID_RESP_B32_EID_0_LSB is already defined
#endif
#define DOIP_VID_RESP_B32_EID_0_LSB 32U

#if (defined DOIP_VID_RESP_B33_GID_6_MSB)
#error DOIP_VID_RESP_B33_GID_6_MSB is already defined
#endif
#define DOIP_VID_RESP_B33_GID_6_MSB 33U

#if (defined DOIP_VID_RESP_B38_GID_0_LSB)
#error DOIP_VID_RESP_B38_GID_0_LSB is already defined
#endif
#define DOIP_VID_RESP_B38_GID_0_LSB 38U

#if (defined DOIP_VID_RESP_B39_FURTHER_ACTION)
#error DOIP_VID_RESP_B39_FURTHER_ACTION is already defined
#endif
#define DOIP_VID_RESP_B39_FURTHER_ACTION 39U

#if (defined DOIP_VID_RESP_B40_VIN_GID_STATUS)
#error DOIP_VID_RESP_B40_VIN_GID_STATUS is already defined
#endif
#define DOIP_VID_RESP_B40_VIN_GID_STATUS 40U

/* === Vehicle identification sizes === */
#if (defined DOIP_VIN_SIZE)
#error DOIP_VIN_SIZE is already defined
#endif
#define DOIP_VIN_SIZE 17U

#if (defined DOIP_EID_SIZE)
#error DOIP_EID_SIZE is already defined
#endif
#define DOIP_EID_SIZE 6U

#if (defined DOIP_GID_SIZE)
#error DOIP_GID_SIZE is already defined
#endif
#define DOIP_GID_SIZE 6U

/* === Vehicle identification response: VIN/GID sync status === */
#if (defined DOIP_VID_RESP_VIN_GID_STATUS_SYNC)
#error DOIP_VID_RESP_VIN_GID_STATUS_SYNC is already defined
#endif
#define DOIP_VID_RESP_VIN_GID_STATUS_SYNC 0x00U

#if (defined DOIP_VID_RESP_VIN_GID_STATUS_NOT_SYNC)
#error DOIP_VID_RESP_VIN_GID_STATUS_NOT_SYNC is already defined
#endif
#define DOIP_VID_RESP_VIN_GID_STATUS_NOT_SYNC 0x10U

/* === Vehicle identification response: Further action required === */
#if (defined DOIP_FURTHER_ACTION_NON)
#error DOIP_FURTHER_ACTION_NON is already defined
#endif
#define DOIP_FURTHER_ACTION_NON 0x00U

#if (defined DOIP_FURTHER_ACTION_ROUTE_ACT)
#error DOIP_FURTHER_ACTION_ROUTE_ACT is already defined
#endif
#define DOIP_FURTHER_ACTION_ROUTE_ACT 0x10U

/** \brief Specific value of routing activation number used for setting Further action byte. **/
#if (defined DOIP_FURTHER_ACTION_ROUTING_ACT_NUM)
#error DOIP_FURTHER_ACTION_ROUTING_ACT_NUM is already defined
#endif
#define DOIP_FURTHER_ACTION_ROUTING_ACT_NUM 0xE0

/* === Byte position: Routing activation request === */
#if (defined DOIP_ROU_ACT_REQ_B0_TESTER_ADDR_MSB)
#error DOIP_ROU_ACT_REQ_B0_TESTER_ADDR_MSB is already defined
#endif
#define DOIP_ROU_ACT_REQ_B0_TESTER_ADDR_MSB 0U

#if (defined DOIP_ROU_ACT_REQ_B1_TESTER_ADDR_LSB)
#error DOIP_ROU_ACT_REQ_B1_TESTER_ADDR_LSB is already defined
#endif
#define DOIP_ROU_ACT_REQ_B1_TESTER_ADDR_LSB 1U

#if (defined DOIP_ROU_ACT_REQ_B2_ACT_TYPE)
#error DOIP_ROU_ACT_REQ_B2_ACT_TYPE is already defined
#endif
#define DOIP_ROU_ACT_REQ_B2_ACT_TYPE 2U

#if (defined DOIP_ROU_ACT_REQ_B7_ISO_RESERVED)
#error DOIP_ROU_ACT_REQ_B7_ISO_RESERVED is already defined
#endif
#define DOIP_ROU_ACT_REQ_B7_ISO_RESERVED 3U

#if (defined DOIP_ROU_ACT_REQ_B7_OEM_SPECIFIC)
#error DOIP_ROU_ACT_REQ_B7_OEM_SPECIFIC is already defined
#endif
#define DOIP_ROU_ACT_REQ_B7_OEM_SPECIFIC 7U

/* === Byte position: Routing activation response === */
#if (defined DOIP_ROU_ACT_RESP_B0_TESTER_ADDR_MSB)
#error DOIP_ROU_ACT_RESP_B0_TESTER_ADDR_MSB is already defined
#endif
#define DOIP_ROU_ACT_RESP_B0_TESTER_ADDR_MSB 0U

#if (defined DOIP_ROU_ACT_RESP_B1_TESTER_ADDR_LSB)
#error DOIP_ROU_ACT_RESP_B1_TESTER_ADDR_LSB is already defined
#endif
#define DOIP_ROU_ACT_RESP_B1_TESTER_ADDR_LSB 1U

#if (defined DOIP_ROU_ACT_RESP_B2_ENTITY_ADDR_MSB)
#error DOIP_ROU_ACT_RESP_B2_ENTITY_ADDR_MSB is already defined
#endif
#define DOIP_ROU_ACT_RESP_B2_ENTITY_ADDR_MSB 2U

#if (defined DOIP_ROU_ACT_RESP_B3_ENTITY_ADDR_LSB)
#error DOIP_ROU_ACT_RESP_B3_ENTITY_ADDR_LSB is already defined
#endif
#define DOIP_ROU_ACT_RESP_B3_ENTITY_ADDR_LSB 3U

#if (defined DOIP_ROU_ACT_RESP_B4_ACT_CODE)
#error DOIP_ROU_ACT_RESP_B4_ACT_CODE is already defined
#endif
#define DOIP_ROU_ACT_RESP_B4_ACT_CODE 4U

#if (defined DOIP_ROU_ACT_RESP_B5_RESERVED)
#error DOIP_ROU_ACT_RESP_B5_RESERVED is already defined
#endif
#define DOIP_ROU_ACT_RESP_B5_RESERVED 5U

#if (defined DOIP_ROU_ACT_RESP_B9_OEM_SPECIFIC)
#error DOIP_ROU_ACT_RESP_B9_OEM_SPECIFIC is already defined
#endif
#define DOIP_ROU_ACT_RESP_B9_OEM_SPECIFIC 9U


/* === Routing activation response: Length  === */
#if (defined DOIP_ROU_ACT_ISO_RESERVED_LEN)
#error DOIP_ROU_ACT_ISO_RESERVED_LEN is already defined
#endif
#define DOIP_ROU_ACT_ISO_RESERVED_LEN 0x04U

#if (defined DOIP_ROU_ACT_OEM_SPECIFIC_LEN)
#error DOIP_ROU_ACT_OEM_SPECIFIC_LEN is already defined
#endif
#define DOIP_ROU_ACT_OEM_SPECIFIC_LEN 0x04U


/* === Routing activation response: Code  === */
/* Routing activation response codes */
#if (defined DOIP_ROU_ACT_RESP_UNKNOWN_SRC)
#error DOIP_ROU_ACT_RESP_UNKNOWN_SRC is already defined
#endif
#define DOIP_ROU_ACT_RESP_UNKNOWN_SRC 0x00U

#if (defined DOIP_ROU_ACT_RESP_NO_FREE_TCP)
#error DOIP_ROU_ACT_RESP_NO_FREE_TCP is already defined
#endif
#define DOIP_ROU_ACT_RESP_NO_FREE_TCP 0x01U

#if (defined DOIP_ROU_ACT_RESP_WRONG_SRC)
#error DOIP_ROU_ACT_RESP_WRONG_SRC is already defined
#endif
#define DOIP_ROU_ACT_RESP_WRONG_SRC 0x02U

#if (defined DOIP_ROU_ACT_RESP_SRC_IN_USE)
#error DOIP_ROU_ACT_RESP_SRC_IN_USE is already defined
#endif
#define DOIP_ROU_ACT_RESP_SRC_IN_USE 0x03U

#if (defined DOIP_ROU_ACT_RESP_NO_AUTHENTICATION)
#error DOIP_ROU_ACT_RESP_NO_AUTHENTICATION is already defined
#endif
#define DOIP_ROU_ACT_RESP_NO_AUTHENTICATION 0x04U

#if (defined DOIP_ROU_ACT_RESP_NO_CONFIRMATION)
#error DOIP_ROU_ACT_RESP_NO_CONFIRMATION is already defined
#endif
#define DOIP_ROU_ACT_RESP_NO_CONFIRMATION 0x05U

#if (defined DOIP_ROU_ACT_RESP_UNKNOWN_ACT_TYPE)
#error DOIP_ROU_ACT_RESP_UNKNOWN_ACT_TYPE is already defined
#endif
#define DOIP_ROU_ACT_RESP_UNKNOWN_ACT_TYPE 0x06U

#if (defined DOIP_ROU_ACT_RESP_TCP_CONN_UNSECURED)
#error DOIP_ROU_ACT_RESP_TCP_CONN_UNSECURED is already defined
#endif
#define DOIP_ROU_ACT_RESP_TCP_CONN_UNSECURED 0x07U

#if (defined DOIP_ROU_ACT_RESP_SUCCESS)
#error DOIP_ROU_ACT_RESP_SUCCESS is already defined
#endif
#define DOIP_ROU_ACT_RESP_SUCCESS 0x10U

#if (defined DOIP_ROU_ACT_RESP_PENDING_CONFIRMATION)
#error DOIP_ROU_ACT_RESP_PENDING_CONFIRMATION is already defined
#endif
#define DOIP_ROU_ACT_RESP_PENDING_CONFIRMATION 0x11U

#if (defined DOIP_ROU_ACT_RESP_SUCCESS_AUTO)
#error DOIP_ROU_ACT_RESP_SUCCESS_AUTO is already defined
#endif
#define DOIP_ROU_ACT_RESP_SUCCESS_AUTO 0xFDU

#if (defined DOIP_DO_NOT_SEND_RESP)
#error DOIP_DO_NOT_SEND_RESP is already defined
#endif
#define DOIP_DO_NOT_SEND_RESP 0xFCU

#if (defined DOIP_INVALID_RESPONSE_CODE)
#error DOIP_INVALID_RESPONSE_CODE is already defined
#endif
#define DOIP_INVALID_RESPONSE_CODE 0xFFU


/* === Byte position: Diagnostic entity status response === */
#if (defined DOIP_ENT_STATUS_RESP_B0_NODE_TYPE)
#error DOIP_ENT_STATUS_RESP_B0_NODE_TYPE is already defined
#endif
#define DOIP_ENT_STATUS_RESP_B0_NODE_TYPE 8U

#if (defined DOIP_ENT_STATUS_RESP_B1_MAX_OPEN_SOCKETS)
#error DOIP_ENT_STATUS_RESP_B1_MAX_OPEN_SOCKETS is already defined
#endif
#define DOIP_ENT_STATUS_RESP_B1_MAX_OPEN_SOCKETS 9U

#if (defined DOIP_ENT_STATUS_RESP_B2_CUR_OPEN_SOCKETS)
#error DOIP_ENT_STATUS_RESP_B2_CUR_OPEN_SOCKETS is already defined
#endif
#define DOIP_ENT_STATUS_RESP_B2_CUR_OPEN_SOCKETS 10U

#if (defined DOIP_ENT_STATUS_RESP_B3_MAX_DATA_SIZE_3_MSB)
#error DOIP_ENT_STATUS_RESP_B3_MAX_DATA_SIZE_3_MSB is already defined
#endif
#define DOIP_ENT_STATUS_RESP_B3_MAX_DATA_SIZE_3_MSB 11U

#if (defined DOIP_ENT_STATUS_RESP_B4_MAX_DATA_SIZE_2)
#error DOIP_ENT_STATUS_RESP_B4_MAX_DATA_SIZE_2 is already defined
#endif
#define DOIP_ENT_STATUS_RESP_B4_MAX_DATA_SIZE_2 12U

#if (defined DOIP_ENT_STATUS_RESP_B5_MAX_DATA_SIZE_1)
#error DOIP_ENT_STATUS_RESP_B5_MAX_DATA_SIZE_1 is already defined
#endif
#define DOIP_ENT_STATUS_RESP_B5_MAX_DATA_SIZE_1 13U

#if (defined DOIP_ENT_STATUS_RESP_B6_MAX_DATA_SIZE_0_LSB)
#error DOIP_ENT_STATUS_RESP_B6_MAX_DATA_SIZE_0_LSB is already defined
#endif
#define DOIP_ENT_STATUS_RESP_B6_MAX_DATA_SIZE_0_LSB 14U

/* === Byte position: Diagnostic power mode information response === */
#if (defined DOIP_ENT_STATUS_RESP_B0_POWER_MODE)
#error DOIP_ENT_STATUS_RESP_B0_POWER_MODE is already defined
#endif
#define DOIP_ENT_STATUS_RESP_B0_POWER_MODE 8U

/* === Byte position: Alive check response === */
#if (defined DOIP_ALIVE_RESP_MSG_B0_SOURCE_ADDR_MSB)
#error DOIP_ALIVE_RESP_MSG_B0_SOURCE_ADDR_MSB is already defined
#endif
#define DOIP_ALIVE_RESP_MSG_B0_SOURCE_ADDR_MSB 0U

#if (defined DOIP_ALIVE_RESP_MSG_B1_SOURCE_ADDR_LSB)
#error DOIP_ALIVE_RESP_MSG_B1_SOURCE_ADDR_LSB is already defined
#endif
#define DOIP_ALIVE_RESP_MSG_B1_SOURCE_ADDR_LSB 1U

/* === Diagnostic message === */
#if (defined DOIP_DIAG_HEADERSIZE)
#error DOIP_DIAG_HEADERSIZE is already defined
#endif
#define DOIP_DIAG_HEADERSIZE ((uint16)4U)

#if (defined DOIP_DIAG_ACK_NACK_HEADER_SIZE)
#error DOIP_DIAG_ACK_NACK_HEADER_SIZE is already defined
#endif
#define DOIP_DIAG_ACK_NACK_HEADER_SIZE ((uint16)5U)

/* Diagnostic message positive acknowledge code */
#if (defined DOIP_DIAG_MSG_CONF_ACK)
#error DOIP_DIAG_MSG_CONF_ACK is already defined
#endif
#define DOIP_DIAG_MSG_CONF_ACK 0x00U

/* Diagnostic message negative acknowledge code */
#if (defined DOIP_DIAG_MSG_NACK_INV_SRC_ADDR)
#error DOIP_DIAG_MSG_NACK_INV_SRC_ADDR is already defined
#endif
#define DOIP_DIAG_MSG_NACK_INV_SRC_ADDR 0x02U

#if (defined DOIP_DIAG_MSG_NACK_INV_TAR_ADDR)
#error DOIP_DIAG_MSG_NACK_INV_TAR_ADDR is already defined
#endif
#define DOIP_DIAG_MSG_NACK_INV_TAR_ADDR 0x03U

#if (defined DOIP_DIAG_MSG_NACK_MSG_TO_LONG)
#error DOIP_DIAG_MSG_NACK_MSG_TO_LONG is already defined
#endif
#define DOIP_DIAG_MSG_NACK_MSG_TO_LONG 0x04U

#if (defined DOIP_DIAG_MSG_NACK_PDUR_BUF_OVFL)
#error DOIP_DIAG_MSG_NACK_PDUR_BUF_OVFL is already defined
#endif
#define DOIP_DIAG_MSG_NACK_PDUR_BUF_OVFL 0x05U

#if (defined DOIP_DIAG_MSG_NACK_ROUTE_INACTIVE)
#error DOIP_DIAG_MSG_NACK_ROUTE_INACTIVE is already defined
#endif
#define DOIP_DIAG_MSG_NACK_ROUTE_INACTIVE 0x06U

#if (defined DOIP_DIAG_MSG_NACK_PDUR_BUF_NOT_OK)
#error DOIP_DIAG_MSG_NACK_PDUR_BUF_NOT_OK is already defined
#endif
#define DOIP_DIAG_MSG_NACK_PDUR_BUF_NOT_OK 0x08U

#if (defined DOIP_DIAG_MSG_HDR_OK)
#error DOIP_DIAG_MSG_HDR_OK is already defined
#endif
#define DOIP_DIAG_MSG_HDR_OK 0xFEU

/* === Byte position: Diagnostic message header === */
#if (defined DOIP_DIAG_MSG_B0_SOURCE_ADDR_MSB)
#error DOIP_DIAG_MSG_B0_SOURCE_ADDR_MSB is already defined
#endif
#define DOIP_DIAG_MSG_B0_SOURCE_ADDR_MSB 0U

#if (defined DOIP_DIAG_MSG_B1_SOURCE_ADDR_LSB)
#error DOIP_DIAG_MSG_B1_SOURCE_ADDR_LSB is already defined
#endif
#define DOIP_DIAG_MSG_B1_SOURCE_ADDR_LSB 1U

#if (defined DOIP_DIAG_MSG_B2_TARGET_ADDR_MSB)
#error DOIP_DIAG_MSG_B2_TARGET_ADDR_MSB is already defined
#endif
#define DOIP_DIAG_MSG_B2_TARGET_ADDR_MSB 2U

#if (defined DOIP_DIAG_MSG_B3_TARGET_ADDR_LSB)
#error DOIP_DIAG_MSG_B3_TARGET_ADDR_LSB is already defined
#endif
#define DOIP_DIAG_MSG_B3_TARGET_ADDR_LSB 3U

#if (defined DOIP_INVALID_BUFFER_POOL_ID)
#error DOIP_INVALID_BUFFER_POOL_ID is already defined
#endif
#define DOIP_INVALID_BUFFER_POOL_ID 255U

#if (defined DOIP_INVALID_PDUID)
#error DOIP_INVALID_PDUID is already defined
#endif
#define DOIP_INVALID_PDUID 0xFFFFU

#if (defined DOIP_INVALID_ROUTE_IDX)
#error DOIP_INVALID_ROUTE_IDX is already defined
#endif
#define DOIP_INVALID_ROUTE_IDX 0xFFFFU

#if (defined DOIP_INVALID_ROUTING_ACTIVATION_IDX)
#error DOIP_INVALID_ROUTING_ACTIVATION_IDX is already defined
#endif
#define DOIP_INVALID_ROUTING_ACTIVATION_IDX 0xFFFFU


#if (defined DOIP_MAX_TX_NON_DIAG_MSG_SIZE)
#error DOIP_MAX_TX_NON_DIAG_MSG_SIZE is already defined
#endif
#define DOIP_MAX_TX_NON_DIAG_MSG_SIZE (DOIP_GEN_HEADER_SIZE + DOIP_PL_LENGTH_ROUTE_ACT_RESP_OEM)

/** \brief Access Post-Build ROM config
 **
 ** This macro accesses an element (struct) in the Post-build config
 ** by using the given references.
 **
 ** \param[in] type     Type of the element (member) to access
 ** \param[in] name     Name of the element (member) to access
 ** \param[in] element  Element (member) in the config to access
 */
#if (defined DOIP_PBCFG_ACCESS)
#error DOIP_PBCFG_ACCESS is already defined
#endif
/* Deviation MISRAC2012-1 */
#define DOIP_PBCFG_ACCESS(type, name, element) \
    (TS_UNCHECKEDGETCFG(DoIP_RootPtr, type, DOIP, (DoIP_RootPtr->name ## Ref))[(element)])

/** \brief Returns element of TcpConRef[] from Post-Build ROM config
 */
#if (defined DOIP_PBCFG_TCPCON)
#error DOIP_PBCFG_TCPCON is already defined
#endif
#define DOIP_PBCFG_TCPCON(Idx) DOIP_PBCFG_ACCESS(DoIP_TcpConnType, TcpCon, Idx)

/** \brief Returns element of UdpAnnConRef[] from Post-Build ROM config
 */
#if (defined DOIP_PBCFG_UDPANNCON)
#error DOIP_PBCFG_UDPANNCON is already defined
#endif
#define DOIP_PBCFG_UDPANNCON(Idx) DOIP_PBCFG_ACCESS(DoIP_UdpConnType, UdpAnnCon, Idx)

/** \brief Returns element of UdpConRef[] from Post-Build ROM config
 */
#if (defined DOIP_PBCFG_UDPCON)
#error DOIP_PBCFG_UDPCON is already defined
#endif
#define DOIP_PBCFG_UDPCON(Idx) DOIP_PBCFG_ACCESS(DoIP_UdpConnType, UdpCon, Idx)

/** \brief Returns element of TesterRef[] from Post-Build ROM config
 */
#if (defined DOIP_PBCFG_TESTER)
#error DOIP_PBCFG_TESTER is already defined
#endif
#define DOIP_PBCFG_TESTER(Idx) DOIP_PBCFG_ACCESS(DoIP_TesterType, Tester, Idx)

/** \brief Returns element of RouteRef[] from Post-Build ROM config
 */
#if (defined DOIP_PBCFG_ROUTE)
#error DOIP_PBCFG_ROUTE is already defined
#endif
#define DOIP_PBCFG_ROUTE(Idx) DOIP_PBCFG_ACCESS(DoIP_RouteType, Route, Idx)

/** \brief Returns element of RoutingActivationRef[] from Post-Build ROM config
 */
#if (defined DOIP_PBCFG_ROUTINGACT)
#error DOIP_PBCFG_ROUTINGACT is already defined
#endif
#define DOIP_PBCFG_ROUTINGACT(Idx) DOIP_PBCFG_ACCESS(DoIP_RoutingActType, RoutingActivation, Idx)

/** \brief Returns element of LOT_TargetRef[] from Post-Build ROM config
 */
#if (defined DOIP_PBCFG_TARGET)
#error DOIP_PBCFG_TARGET is already defined
#endif
#define DOIP_PBCFG_TARGET(Idx) DOIP_PBCFG_ACCESS(uint16, LOT_Target, Idx)

/** \brief Returns element of LOT_PduRTxPduIdRef[] from Post-Build ROM config
 */
#if (defined DOIP_PBCFG_PDURTXPDUID)
#error DOIP_PBCFG_PDURTXPDUID is already defined
#endif
#define DOIP_PBCFG_PDURTXPDUID(Idx) DOIP_PBCFG_ACCESS(PduIdType, LOT_PduRTxPduId, Idx)

/** \brief Returns element of LOT_PduRRxPduIdRef[] from Post-Build ROM config
 */
#if (defined DOIP_PBCFG_PDURRXPDUID)
#error DOIP_PBCFG_PDURRXPDUID is already defined
#endif
#define DOIP_PBCFG_PDURRXPDUID(Idx) DOIP_PBCFG_ACCESS(PduIdType, LOT_PduRRxPduId, Idx)

#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
/** \brief Returns element of CustomChannelRef[] from Post-Build ROM config
 */
#if (defined DOIP_PBCFG_CUSTOMCHANNEL)
#error DOIP_PBCFG_CUSTOMCHANNEL is already defined
#endif
#define DOIP_PBCFG_CUSTOMCHANNEL(Idx) DOIP_PBCFG_ACCESS(DoIP_CustChannelType, CustomChannel, Idx)

#endif /* DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON */

#undef TS_RELOCATABLE_CFG_ENABLE

/*==================[type definitions]======================================*/

/** \brief General timeout type. */
typedef VAR( uint32, TYPEDEF ) DoIP_TimeoutType;

typedef VAR( uint16, TYPEDEF ) DoIP_TimeoutIdxType;

/** \brief Function pointer type for user defined User_GetVin() */
typedef P2FUNC(Std_ReturnType, TYPEDEF, DoIP_User_GetVin_FpType)
(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) Data
);

#if(DOIP_POWER_MODE == STD_ON)
/** \brief Function pointer type for user defined User_PowerMode() */
typedef P2FUNC(Std_ReturnType, TYPEDEF, DoIP_PowerMode_FpType)
(
  P2VAR(DoIP_PowerStateType, AUTOMATIC, DOIP_APPL_DATA) PowerStateReady
);
#endif

/** \brief Function pointer type for user/SVC interface provided Routing activation auth. func. */
typedef P2FUNC(Std_ReturnType, TYPEDEF, DoIP_RoutingActivationAuth_FpType)
(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_DATA) Authentified,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) AuthenticationReqData,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) AuthenticationResData
);
/* !LINKSTO DoIP.ASR41.SWS_DoIP_00049, 1 */

/** \brief Function pointer type for user/SVC interface Routing activation conf. func. */
typedef P2FUNC(Std_ReturnType, TYPEDEF, DoIP_RoutingActivationConf_FpType)
(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_DATA) Confirmed,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) ConfirmationReqData,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) ConfirmationResData
);
/* !LINKSTO DoIP.ASR41.SWS_DoIP_00048, 1 */

#if (DOIP_DYN_GID_SYNC_ENABLED == STD_ON)
typedef P2FUNC(void, TYPEDEF, DoIP_SyncGid_FpType)
(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_DATA) IsGIDMasterPtr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) GroupIdPtr
);

typedef P2FUNC(Std_ReturnType, TYPEDEF, DoIP_GetGid_FpType)
(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) GroupIdPtr
);
#endif /* DOIP_DYN_GID_SYNC_ENABLED == STD_ON */
/* !LINKSTO DoIP.EB.SWS_DoIP_00051, 1 */
/* !LINKSTO DoIP.EB.DoIPDynamicGIDMasterSelection, 1 */

/* !LINKSTO DoIP.dsn.Func0125.UserVIDRequestReceived_Syntax, 1 */
#if (DOIP_USER_VID_REQ_RECEIVED_CALLBACK == STD_ON)
typedef P2FUNC(void, TYPEDEF, DoIP_VIDReqReceived_FpType)
(
  uint8 DoIPInterfaceId
);
#endif /* DOIP_USER_VID_REQ_RECEIVED_CALLBACK == STD_ON */

/** \brief Message pointer type **/
typedef P2VAR(uint8, TYPEDEF, DOIP_APPL_DATA) DoIP_DataPtrType;

/** \brief Internal routing activation state type. **/
typedef uint8 DoIP_RoutingStateType;

#if (defined DOIP_ROUTING_INACTIVE)
#error DOIP_ROUTING_INACTIVE is already defined
#endif
#define DOIP_ROUTING_INACTIVE 0U

#if (defined DOIP_ROUTING_PENDING_AUTHENTICATION)
#error DOIP_ROUTING_PENDING_AUTHENTICATION is already defined
#endif
#define DOIP_ROUTING_PENDING_AUTHENTICATION 1U

#if (defined DOIP_ROUTING_AUTHENTIFIED)
#error DOIP_ROUTING_AUTHENTIFIED is already defined
#endif
#define DOIP_ROUTING_AUTHENTIFIED 2U

#if (defined DOIP_ROUTING_PENDING_CONFIRMATION)
#error DOIP_ROUTING_PENDING_CONFIRMATION is already defined
#endif
#define DOIP_ROUTING_PENDING_CONFIRMATION 3U

#if (defined DOIP_ROUTING_ACTIVE)
#error DOIP_ROUTING_ACTIVE is already defined
#endif
#define DOIP_ROUTING_ACTIVE 4U

/** \brief Internal routing activation index type. **/
typedef uint16 DoIP_RoutingActivationIdxType;

#if (defined DOIP_AUTOMATIC_ROUTING_ACTIVATION_IDX)
#error DOIP_AUTOMATIC_ROUTING_ACTIVATION_IDX is already defined
#endif
#define DOIP_AUTOMATIC_ROUTING_ACTIVATION_IDX 0xFFFFU

typedef struct
{
  PduLengthType TxDiagMsgLength; /* Diagnostic message length of DoIP_TpTransmit() request. */
  PduIdType NextDoIPPduRPduId; /* ID of next DoIP_TpTransmit() pending request. */
  /* Index of routing activation object used to activate the channel. */
  DoIP_RoutingActivationIdxType RoutingActivationIdx;
#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
  uint16 PayloadType; /* Payload type retrieved from Tx Pdu meta data. */
#endif
  DoIP_TcpConIdxType TcpConIdx; /* Tcp connection index to which this channel is linked. */
  boolean PendingRequest; /* Indicates if a Tx request is pending for this channel. */
  /* Indicates if Oem specific field was received in routing activation request message. */
  boolean OemSpecificValid;
  DoIP_RoutingStateType RoutingActivationState;
  /* OEM specific field from the received routing activation message. */
  uint8 RxOemSpecific[DOIP_ROU_ACT_OEM_SPECIFIC_LEN];
  /* !LINKSTO DoIP.ASR41.SWS_DoIP_00267, 1 */
  /* !LINKSTO DoIP.ASR41.SWS_DoIP_00269, 1 */
  /* OEM specific field transmitted in routing activation response message. */
  uint8 TxOemSpecific[DOIP_ROU_ACT_OEM_SPECIFIC_LEN];
  /* !LINKSTO DoIP.ASR41.SWS_DoIP_00268, 1 */
  /* !LINKSTO DoIP.ASR41.SWS_DoIP_00270, 1 */
} DoIP_ChannelType;

/** \brief Type definition of pointer to DoIP_ChannelType. **/
typedef P2VAR(DoIP_ChannelType, AUTOMATIC, DOIP_APPL_DATA) DoIP_ChannelPtrType;

/** \brief Internal TCP Rx connection state type. **/
typedef uint8 DoIP_TcpConRxStateType;
#if (defined DOIP_TCPCON_RX_OFFLINE)
#error DOIP_TCPCON_RX_OFFLINE is already defined
#endif
#define DOIP_TCPCON_RX_OFFLINE 0U

#if (defined DOIP_TCPCON_RX_START)
#error DOIP_TCPCON_RX_START is already defined
#endif
#define DOIP_TCPCON_RX_START 1U

#if (defined DOIP_TCPCON_RX_READY)
#error DOIP_TCPCON_RX_READY is already defined
#endif
#define DOIP_TCPCON_RX_READY 2U

#if (defined DOIP_TCPCON_RX_ROUTE_ACT)
#error DOIP_TCPCON_RX_ROUTE_ACT is already defined
#endif
#define DOIP_TCPCON_RX_ROUTE_ACT 3U

#if (defined DOIP_TCPCON_RX_DIAG_HEADER)
#error DOIP_TCPCON_RX_DIAG_HEADER is already defined
#endif
#define DOIP_TCPCON_RX_DIAG_HEADER 4U

#if (defined DOIP_TCPCON_RX_DIAG_MSG)
#error DOIP_TCPCON_RX_DIAG_MSG is already defined
#endif
#define DOIP_TCPCON_RX_DIAG_MSG 5U

#if (defined DOIP_TCPCON_RX_ACK_CONF_WAIT)
#error DOIP_TCPCON_RX_ACK_CONF_WAIT is already defined
#endif
#define DOIP_TCPCON_RX_ACK_CONF_WAIT 6U

#if (defined DOIP_TCPCON_RX_DISCARD_DIAG_MSG)
#error DOIP_TCPCON_RX_DISCARD_DIAG_MSG is already defined
#endif
#define DOIP_TCPCON_RX_DISCARD_DIAG_MSG 7U

#if (defined DOIP_TCPCON_RX_DISCARD_MSG)
#error DOIP_TCPCON_RX_DISCARD_MSG is already defined
#endif
#define DOIP_TCPCON_RX_DISCARD_MSG 8U

#if (defined DOIP_TCPCON_RX_SINGLE_ALIVE_CHECK_WAIT)
#error DOIP_TCPCON_RX_SINGLE_ALIVE_CHECK_WAIT is already defined
#endif
#define DOIP_TCPCON_RX_SINGLE_ALIVE_CHECK_WAIT 9U

#if (defined DOIP_TCPCON_RX_ALL_CON_ALIVE_CHECK_WAIT)
#error DOIP_TCPCON_RX_ALL_CON_ALIVE_CHECK_WAIT is already defined
#endif
#define DOIP_TCPCON_RX_ALL_CON_ALIVE_CHECK_WAIT 10U

#if (defined DOIP_TCPCON_RX_ALIVE_CHECK_RESP)
#error DOIP_TCPCON_RX_ALIVE_CHECK_RESP is already defined
#endif
#define DOIP_TCPCON_RX_ALIVE_CHECK_RESP 11U

#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
#if (defined DOIP_TCPCON_RX_CUSTOM_MSG_PAYLOAD)
#error DOIP_TCPCON_RX_CUSTOM_MSG_PAYLOAD is already defined
#endif
#define DOIP_TCPCON_RX_CUSTOM_MSG_PAYLOAD 12U
#endif /* DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON */

/** \brief Internal TCP Tx connection state type. **/
typedef uint8 DoIP_TcpConTxStateType;
#if (defined DOIP_TCPCON_TX_OFFLINE)
#error DOIP_TCPCON_TX_OFFLINE is already defined
#endif
#define DOIP_TCPCON_TX_OFFLINE 0U

#if (defined DOIP_TCPCON_TX_IDLE)
#error DOIP_TCPCON_TX_IDLE is already defined
#endif
#define DOIP_TCPCON_TX_IDLE 1U

#if (defined DOIP_TCPCON_TX_ROUTE_ACT)
#error DOIP_TCPCON_TX_ROUTE_ACT is already defined
#endif
#define DOIP_TCPCON_TX_ROUTE_ACT 2U

#if (defined DOIP_TCPCON_TX_DIAG_MSG_START)
#error DOIP_TCPCON_TX_DIAG_MSG_START is already defined
#endif
#define DOIP_TCPCON_TX_DIAG_MSG_START 3U

#if (defined DOIP_TCPCON_TX_DIAG_MSG)
#error DOIP_TCPCON_TX_DIAG_MSG is already defined
#endif
#define DOIP_TCPCON_TX_DIAG_MSG 4U

#if (defined DOIP_TCPCON_TX_DIAG_ACK_NACK_START)
#error DOIP_TCPCON_TX_DIAG_ACK_NACK_START is already defined
#endif
#define DOIP_TCPCON_TX_DIAG_ACK_NACK_START 5U

#if (defined DOIP_TCPCON_TX_DIAG_ACK_NACK)
#error DOIP_TCPCON_TX_DIAG_ACK_NACK is already defined
#endif
#define DOIP_TCPCON_TX_DIAG_ACK_NACK 6U

#if (defined DOIP_TCPCON_TX_GENHEADER_NACK)
#error DOIP_TCPCON_TX_GENHEADER_NACK is already defined
#endif
#define DOIP_TCPCON_TX_GENHEADER_NACK 7U

#if (defined DOIP_TCPCON_TX_ROUTE_ACT_OEM)
#error DOIP_TCPCON_TX_ROUTE_ACT_OEM is already defined
#endif
#define DOIP_TCPCON_TX_ROUTE_ACT_OEM 8U

#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
#if (defined DOIP_TCPCON_TX_CUSTOM_MSG_START)
#error DOIP_TCPCON_TX_CUSTOM_MSG_START is already defined
#endif
#define DOIP_TCPCON_TX_CUSTOM_MSG_START 9U

#if (defined DOIP_TCPCON_TX_CUSTOM_MSG)
#error DOIP_TCPCON_TX_CUSTOM_MSG is already defined
#endif
#define DOIP_TCPCON_TX_CUSTOM_MSG 10U
#endif /* DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON */

#if (defined DOIP_TCPCON_TX_ALIVE_CHECK_REQ)
#error DOIP_TCPCON_TX_ALIVE_CHECK_REQ is already defined
#endif
#define DOIP_TCPCON_TX_ALIVE_CHECK_REQ 11U

/** \brief Internal connection state type. **/
typedef uint8 DoIP_ConnectionStateType;

/* DoIP connection states according to ISO13400 Figure 25 */
/* Connection state socket initialized */
#if (defined DOIP_TCP_CONSTATE_INIT)
#error DOIP_TCP_CONSTATE_INIT is already defined
#endif
#define DOIP_TCP_CONSTATE_INIT 0U

/* Connection state Source address requested */
#if (defined DOIP_TCP_CONSTATE_SA_REQUESTED)
#error DOIP_TCP_CONSTATE_SA_REQUESTED is already defined
#endif
#define DOIP_TCP_CONSTATE_SA_REQUESTED 1U

/* Connection state Registered pending */
#if (defined DOIP_TCP_CONSTATE_SA_REGISTERED_PENDING)
#error DOIP_TCP_CONSTATE_SA_REGISTERED_PENDING is already defined
#endif
#define DOIP_TCP_CONSTATE_SA_REGISTERED_PENDING 2U

/* Connection state Registered routing active */
#if (defined DOIP_TCP_CONSTATE_SA_REGISTERED_ROUTING_ACTIVE)
#error DOIP_TCP_CONSTATE_SA_REGISTERED_ROUTING_ACTIVE is already defined
#endif
#define DOIP_TCP_CONSTATE_SA_REGISTERED_ROUTING_ACTIVE 3U


/* !LINKSTO DoIP.ASR41.SWS_DoIP_00002, 1 */
/* !LINKSTO DoIP.ASR41.SWS_DoIP_00142, 1 */
typedef struct
{
  uint32 AliveCheckTimeout;
  uint32 RxDiagMsgLength;
  uint32 RxBufferSize;
  PduIdType DoIPSoAdPduId;
  PduIdType DoIPPduRRxPduId;
  PduIdType DoIPPduRTxPduId;
  PduIdType RequestedTxState_DoIPPduRTxPduId;
  PduIdType NextDoIPPduRPduId; /* ID to next pending Tx request. */
  PduIdType LastDoIPPduRPduId; /* ID to last pending Tx request. */
  uint16 Source;
  /* Source addresses received in DiagMsg - used when sending NACK. */
  uint16 RxDiagMsgSA;
  uint16 RxDiagMsgTA;
  /* Start index in PbCfg DoIP_Route[] of the fist pending routing auth/conf. */
  uint16 NextPendingRouteIdx;
  /* Source and target addresses for diagnostic negative acknowledge message */
  uint16 TxDiagNackMsgSA;
  uint16 TxDiagNackMsgTA;
  /* Tester Source address received in Routing activation request message.*/
  uint16 RoutActReqSA;
  /* Specifies the number of bytes that are copied from the original Diagnostic msg. on ACK/NACK */
  uint16 PrevDiagMsgSize;
  /* Number of transmitted bytes of generic header, diag. header or non-diagnostic msg payload. */
  PduLengthType TxMsgPos;
  PduLengthType WritePos; /* buffer write pointer */
  PduLengthType ReadPos;  /* buffer read pointer */
  DoIP_TcpConRxStateType RxState;
  DoIP_TcpConTxStateType TxState;
  DoIP_TcpConTxStateType RequestedTxState;
  boolean ResetGeneralInactivityTimer;
  /* Response code: Depending on the Tx state it can be used for routing response generic header
     or diagnostic NACK. */
  uint8 TxRespCode;
  DoIP_ConnectionStateType ConnectionState;
  /* Pointer to PbCfg DoIP_Tester[] assigned to this Tcp connection. */
  P2CONST(DoIP_TesterType, DOIP_VAR, DOIP_APPL_CONST) TesterPtr;
  /* Flag indicating that more then 1 pending routing activations exist for this Tcp connection. */
  boolean MultipleAuthConfPending;
  /* Buffer for storing segmented headers or diagnostic message payload used for ACK/NACK. */
  uint8 Buffer[DOIP_TCP_CONN_BUFF_SIZE];
} DoIP_TcpConType;

/** \brief Function pointer type for filling message headers and non-diagnostic msg. payloads */
typedef P2FUNC(void, DOIP_APPL_CODE, DoIP_FillMsgHdrFpType)
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) SduDataPtr
);

/** \brief Function pointer type for filling diagnostic message payload */
typedef P2FUNC(BufReq_ReturnType, DOIP_APPL_CODE, DoIP_FillDiagMsgPldFpType)
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) AvailableDataPtr
);

/* !LINKSTO DoIP.dsn.Func0129.User_RoutingActivationCallback, 1 */
/** \brief Function pointer type for routing activation user callback function. */
typedef P2FUNC(void, TYPEDEF, DoIP_RoutingActivationCallback_FpType)
(
  SoAd_SoConIdType SoConId,   /* SoConId of Tcp connection that received routing activation msg. */
  uint16 SourceAddr,          /* Source address from routing activation message. */
  uint8 ActivationType,       /* Activation type from routing activation message. */
  /* Pointer to OEM specific field from routing activation message. NULL_PTR if OEM specific field
   * was omitted. */
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) OemSpecificPtr
);

/*==================[external function declarations]========================*/

#define DOIP_START_SEC_CODE
#include <DoIP_MemMap.h>

/**
 * \brief User_GetVin() dummy callback function
 *
 * This function is used if a user does not provide User_GetVin() function.
 *
 * \param[in] DataPtr - Unused parameter
 */
extern FUNC(Std_ReturnType, DOIP_CODE) DoIP_User_GetVinDummy
(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) DataPtr
);

/**
 * \brief Routing Activation Authentication dummy callback function
 *
 * This function is used if a user does not provide a callback function.
 *
 * \param[out] Authentified - Set to TRUE if valid pointer provided indicating auth. was successful
 * \param[in]  AuthenticationReqData - Unused parameter
 * \param[out] AuthenticationResData - Unused parameter
 * \return Std_ReturnType
 * \retval E_OK - Authentified contains valid data.
 * \retval E_NOT_OK - Authentified does not contains valid data.
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DOIP_CODE) DoIP_RoutingActivationAuthDummy
(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_DATA) Authentified,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) AuthenticationReqData,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) AuthenticationResData
);

/**
 * \brief Routing Activation Confirmation dummy callback function
 *
 * This function is used if a user does not provide a callback function.
 *
 * \param[out] Confirmed - Set to TRUE if valid pointer provided indicating conf. was successful
 * \param[in]  ConfirmationReqData - Unused parameter
 * \param[out] ConfirmationResData - Unused parameter
 * \return Std_ReturnType
 * \retval E_OK - Confirmed contains valid data.
 * \retval E_NOT_OK - Confirmed does not contains valid data.
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DOIP_CODE) DoIP_RoutingActivationConfDummy
(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_DATA) Confirmed,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) ConfirmationReqData,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) ConfirmationResData
);

/** \brief Internal service to fill generic DoIP header.
 *
 * \param[out] PduPtr - Pointer to store the generic DoIP message header.
 * \param[in] PayloadLength - Length of the DoIP message payload to be stored in the header.
 * \param[in] PayloadType - Payload type to be stored in the header.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
extern FUNC(void, DOIP_CODE) DoIP_Fill_GenHeader
(
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) PduPtr,
  uint32 PayloadLength,
  uint16 PayloadType
);

/** \brief Internal service to find the TCP connection for a DoIPSoAdPduId.
 *
 * \param[in] DoIPSoAdPduId - ID from a DoIP API called from SoAd.
 *
 * \return The function returns TcpConIdx. If no connection is found, invalid TcpConIdx is returned.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
extern FUNC(DoIP_TcpConIdxType, DOIP_CODE) DoIP_FindTcpCon
(
  PduIdType DoIPSoAdPduId
);

/** \brief Internal service to prepare Tcp connection state machines
 *
 * This internal service initializes Tcp connection state machines and assignes SoAd Pdu id
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] DoIPSoAdPduId - SoAd Pdu Id that is assigned to this Tcp connection
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
extern FUNC(void, DOIP_CODE) DoIP_PrepareTcpCon
(
  DoIP_TcpConIdxType TcpConIdx,
  PduIdType DoIPSoAdPduId
);

#if (DOIP_USER_TCP_SOCON_MODECHG_CALLBACK == STD_ON)
typedef P2FUNC(void, TYPEDEF, DoIP_TcpSoConModeChg_FpType)
(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Status
);
#endif /* DOIP_USER_TCP_SOCON_MODECHG_CALLBACK == STD_ON */

/** \brief Internal service to perform routing activation of a single channel.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] DoIPPduRPduId - DoIPPduRPduID of the channel that needs to be activated.
 * \param[in] PayloadLength - Payload length of a routing activation request message (7 or 11).
 * \param[in] RoutingActIdx - Index of routing activation object
 * \param[out] ChannelProcessedPtrPtr - Adress of a pointer to already processed channel.
 * \param[in] PduPtr - Pointer to routing activation request payload.
 * \param[out] TxStatePtr - Pointer to TxState for sending acknowledge message.
 * \param[out] RespCodePtr - Pointer to response code for acknowledge message.
 *
 * \return BufReq_ReturnType
 * \retval BUFREQ_OK - Function execution was successful.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
extern FUNC(void, DOIP_CODE) DoIP_RoutingActivationChannel
(
  DoIP_TcpConIdxType TcpConIdx,
  PduIdType DoIPPduRPduId,
  PduLengthType PayloadLength,
  DoIP_RoutingActivationIdxType RoutingActIdx,
  P2VAR(DoIP_ChannelPtrType, AUTOMATIC, DOIP_APPL_DATA) ChannelProcessedPtrPtr,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) PduPtr,
  P2VAR(DoIP_TcpConTxStateType, AUTOMATIC, DOIP_APPL_DATA) TxStatePtr,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) RespCodePtr
);

/** \brief Internal service to send a non-diagnostic message (e.g. positive/negative NACK message)
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] TxState - Requested internal TCP Tx connection state
 * \param[in] RespCode - Message acknowledge code (if applicable)
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
extern FUNC(void, DOIP_CODE) DoIP_SendTcpNonDiagMsg
(
  DoIP_TcpConIdxType TcpConIdx,
  DoIP_TcpConTxStateType TxState,
  uint8 RespCode
);

/** \brief Helper function to construct a 16 bit value from bytes.
 *
 * \param[in] Byte1_Msb - Most significant byte of the 16 bit value.
 * \param[in] Byte0_Lsb - Least significant byte of the 16 bit value.
 *
 * \return The function returns the put together 16 bit value.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
extern FUNC(uint16, DOIP_CODE) DoIP_Make_uint16(uint8 Byte1_Msb, uint8 Byte0_Lsb);


/** \brief Internal service to handle segmented reception.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] PduInfoPtr - Pointer to the header.
 * \param[out] RxMsgPtr - Pointer to the received message, NULL_PTR if message
 *                        is not complete.
 * \param[out] MsgLengthPtr - Pointer to the message length.
 *
 * \return BufReq_ReturnType
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
extern FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_HandleSegmentedRx
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr,
  P2VAR(DoIP_DataPtrType, AUTOMATIC, DOIP_APPL_DATA) RxMsgPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) MsgLengthPtr
);

#define DOIP_STOP_SEC_CODE
#include <DoIP_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/
#define DOIP_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <DoIP_MemMap.h>

#include <TSAutosar.h> /* EB specific standard types */

/** \brief User_GetVin() function pointer
 *
 * This is a function pointer to user defined User_GetVin().
 */
extern CONST(DoIP_User_GetVin_FpType, DOIP_APPL_CONST) DoIP_User_GetVin_Fp;

#if(DOIP_POWER_MODE == STD_ON)
/** \brief DoIP_User_PowerMode() function pointer
 *
 * This is a function pointer to user/Rte provided PowerMode() callback function.
 */
extern CONST(DoIP_PowerMode_FpType, DOIP_APPL_CONST) DoIP_PowerMode_Fp;
#endif

/** \brief User_RoutingActivationAuth() function pointer list
 *
 * This array contains user/SVC interface provided RoutingActivationAuth function pointers.
 */
extern CONST(DoIP_RoutingActivationAuth_FpType, DOIP_APPL_CONST) DoIP_RoutingActivationAuth_FpList[];

#if (DOIP_DYN_GID_SYNC_ENABLED == STD_ON)
/** \brief DoIP_User_SyncGid() function pointer
 *
 * This is a function pointer to user defined Dynamic GID Sync callback function.
 */
extern CONST(DoIP_SyncGid_FpType, DOIP_APPL_CONST) DoIP_User_SyncGid_Fp;

/** \brief DoIP_User_GetGid() function pointer
 *
 * This is a function pointer to user defined DoIPGetGID function.
 */
extern CONST(DoIP_GetGid_FpType, DOIP_APPL_CONST) DoIP_User_GetGid_Fp;

#endif

#if(DOIP_USER_VID_REQ_RECEIVED_CALLBACK == STD_ON)
/** \brief DoIP_User_VIDRequestReceived() function pointer
 *
 * This is a function pointer to user defined VIDRequestReceived function.
 */
extern CONST(DoIP_VIDReqReceived_FpType, DOIP_APPL_CONST) DoIP_User_VIDRequestReceived_Fp;
#endif /* DOIP_USER_VID_REQ_RECEIVED_CALLBACK == STD_ON */

/** \brief User_RoutingActivationConf() function pointer list
 *
 * This array contains user/SVC interface provided RoutingActivationConf function pointers.
 */
extern CONST(DoIP_RoutingActivationConf_FpType, DOIP_APPL_CONST) DoIP_RoutingActivationConf_FpList[];

#if (DOIP_USER_TCP_SOCON_MODECHG_CALLBACK == STD_ON)
/** \brief DoIP_User_TcpSoConModeChg_Fp() function pointer
 *
 * This is a function pointer to user defined TcpSoConModeChg function.
 */
extern CONST(DoIP_TcpSoConModeChg_FpType, DOIP_APPL_CONST) DoIP_User_TcpSoConModeChg_Fp;
#endif /* DOIP_USER_TCP_SOCON_MODECHG_CALLBACK == STD_ON */

#if (DOIP_ROUTING_ACTIVATION_CALLBACK_NUM > 0U)
/** \brief List of function pointers to user defined routing activation callback functions.
 */
extern CONST(DoIP_RoutingActivationCallback_FpType, DOIP_APPL_CONST)
  DoIP_RoutingActivationCallback_FpList[];
#endif /* DOIP_ROUTING_ACTIVATION_CALLBACK_NUM > 0U */

#define DOIP_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <DoIP_MemMap.h>

#define DOIP_START_SEC_CONST_32
#include <DoIP_MemMap.h>

/* Value used to validate post build configuration against link time configuration. */
extern CONST(uint32, DOIP_CONST) DoIP_LcfgSignature;

#define DOIP_STOP_SEC_CONST_32
#include <DoIP_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define DOIP_START_SEC_VAR_INIT_UNSPECIFIED
#include <DoIP_MemMap.h>

extern P2CONST(DoIP_ConfigType, DOIP_VAR, DOIP_APPL_CONST) DoIP_RootPtr;

#define DOIP_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <DoIP_MemMap.h>

#define DOIP_START_SEC_VAR_INIT_8
#include <DoIP_MemMap.h>

extern VAR(boolean, DOIP_VAR) DoIP_Initialized;

#define DOIP_STOP_SEC_VAR_INIT_8
#include <DoIP_MemMap.h>

#define DOIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <DoIP_MemMap.h>

/* Data common to standard and custom payload channels. */
extern VAR(DoIP_ChannelType, DOIP_VAR) DoIP_Channel[DOIP_MAX_CHANNELS];

extern VAR(DoIP_TcpConType, DOIP_VAR) DoIP_TcpCon[DOIP_MAX_PARALLEL_TESTER + 1U];

#define DOIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <DoIP_MemMap.h>

#define DOIP_START_SEC_VAR_INIT_32
#include <DoIP_MemMap.h>

#if (DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
/** \brief counter for number of dropped invalid Generic hdr messages */
extern VAR(uint32, DOIP_VAR) DoIP_MeasurementDropData_GenHdr;

/** \brief counter for number of dropped Diag messages */
extern VAR(uint32, DOIP_VAR) DoIP_MeasurementDropData_DiagMsg;
#endif /* DOIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#define DOIP_STOP_SEC_VAR_INIT_32
#include <DoIP_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* DOIP_INT_H */
/*==================[end of file]===========================================*/

