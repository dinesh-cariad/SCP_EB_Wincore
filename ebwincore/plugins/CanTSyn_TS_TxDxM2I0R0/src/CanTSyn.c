/**
 * \file
 *
 * \brief AUTOSAR CanTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTSyn.
 *
 * \version 2.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason:
 * False-positive from the Tasking v5.0r2 compiler (TCVX-41885)
 */

/*==================[inclusions]============================================*/

#include <TSAutosar.h> /* Specific standard types */
#include <TSMem.h> /* EB memcopy */
#include <CanTSyn.h> /* Module public API */
#include <CanTSyn_Trace.h> /* Debug and trace */
#include <CanIf.h> /* Module public API */

#include <SchM_CanTSyn.h> /* SchM interface header */

#if(CANTSYN_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* DET support */
#endif /* CANTSYN_DEV_ERROR_DETECT == STD_ON */
#if((CANTSYN_TX_CRC_USED == STD_ON) || (CANTSYN_RX_CRC_USED == STD_ON))
#include <Crc.h> /* CRC support */
#endif/* (CANTSYN_TX_CRC_USED == STD_ON) || (CANTSYN_RX_CRC_USED == STD_ON) */

/*==================[version check]=========================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef CANTSYN_VENDOR_ID /* configuration check */
#error CANTSYN_VENDOR_ID must be defined
#endif

#if (CANTSYN_VENDOR_ID != 1U) /* vendor check */
#error CANTSYN_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef CANTSYN_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error CANTSYN_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (CANTSYN_AR_RELEASE_MAJOR_VERSION != 4U)
#error CANTSYN_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef CANTSYN_AR_RELEASE_MINOR_VERSION /* configuration check */
#error CANTSYN_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (CANTSYN_AR_RELEASE_MINOR_VERSION != 4U )
#error CANTSYN_AR_RELEASE_MINOR_VERSION wrong (!= 4U)
#endif

#ifndef CANTSYN_AR_RELEASE_REVISION_VERSION /* configuration check */
#error CANTSYN_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (CANTSYN_AR_RELEASE_REVISION_VERSION != 0U )
#error CANTSYN_AR_RELEASE_REVISION_VERSION wrong (!= 0U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef CANTSYN_SW_MAJOR_VERSION /* configuration check */
#error CANTSYN_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (CANTSYN_SW_MAJOR_VERSION != 2U)
#error CANTSYN_SW_MAJOR_VERSION wrong (!= 2U)
#endif

#ifndef CANTSYN_SW_MINOR_VERSION /* configuration check */
#error CANTSYN_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (CANTSYN_SW_MINOR_VERSION < 0U)
#error CANTSYN_SW_MINOR_VERSION wrong (< 0U)
#endif

#ifndef CANTSYN_SW_PATCH_VERSION /* configuration check */
#error CANTSYN_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (CANTSYN_SW_PATCH_VERSION < 8U)
#error CANTSYN_SW_PATCH_VERSION wrong (< 8U)
#endif

/*==================[macros]================================================*/

#if(CANTSYN_DEV_ERROR_DETECT == STD_ON)

#if (defined CANTSYN_DET_REPORT_ERROR)
#error CANTSYN_DET_REPORT_ERROR is already defined
#endif /* if (defined CANTSYN_DET_REPORT_ERROR) */

#define CANTSYN_DET_REPORT_ERROR(ApiId, ErrorId) \
  (void) Det_ReportError(CANTSYN_MODULE_ID, CANTSYN_INSTANCE_ID, (ApiId), (ErrorId))
#endif /* CANTSYN_DEV_ERROR_DETECT == STD_ON */

#if (defined CANTSYN_MSG_SIZE) /* to prevent double declaration */
#error CANTSYN_MSG_SIZE is already defined
#endif /* if (defined CANTSYN_MSG_SIZE) */
#define CANTSYN_MSG_SIZE 8U /* Size of the CanTSyn messages */

#if (defined CANTSYN_EXTENDED_MSG_SIZE) /* to prevent double declaration */
#error CANTSYN_EXTENDED_MSG_SIZE is already defined
#endif /* if (defined CANTSYN_EXTENDED_MSG_SIZE) */
#define CANTSYN_EXTENDED_MSG_SIZE 16U /* Size of the CanTSyn extended messages */

#if (defined CANTSYN_NO_USERDATA) /* to prevent double declaration */
#error CANTSYN_NO_USERDATA is already defined
#endif /* if (defined CANTSYN_NO_USERDATA) */
#define CANTSYN_NO_USERDATA 0U /* No User Data support */

/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00031,1 */
#if (defined CANTSYN_SYNC_WITHOUT_CRC) /* to prevent double declaration */
#error CANTSYN_SYNC_WITHOUT_CRC is already defined
#endif /* if (defined CANTSYN_SYNC_WITHOUT_CRC) */
#define CANTSYN_SYNC_WITHOUT_CRC 0x10U /* Sync Message with no CRC Type */

#if (defined CANTSYN_FUP_WITHOUT_CRC) /* to prevent double declaration */
#error CANTSYN_FUP_WITHOUT_CRC is already defined
#endif /* if (defined CANTSYN_FUP_WITHOUT_CRC) */
#define CANTSYN_FUP_WITHOUT_CRC  0x18U /* Fup Message with no CRC Type */

/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00041,1 */

#if (defined CANTSYN_OFS_WITHOUT_CRC) /* to prevent double declaration */
#error CANTSYN_OFS_WITHOUT_CRC is already defined
#endif /* if (defined CANTSYN_OFS_WITHOUT_CRC) */
#define CANTSYN_OFS_WITHOUT_CRC  0x34U /* OFS Message with no CRC Type */

#if (defined CANTSYN_OFS_EXT_MSG_WITHOUT_CRC) /* to prevent double declaration */
#error CANTSYN_OFS_EXT_MSG_WITHOUT_CRC is already defined
#endif /* if (defined CANTSYN_OFS_EXT_MSG_WITHOUT_CRC) */
#define CANTSYN_OFS_EXT_MSG_WITHOUT_CRC  0x54U /* Extended OFS Message with no CRC Type */

#if (defined CANTSYN_OFNS_WITHOUT_CRC) /* to prevent double declaration */
#error CANTSYN_OFNS_WITHOUT_CRC is already defined
#endif /* if (defined CANTSYN_OFNS_WITHOUT_CRC) */
#define CANTSYN_OFNS_WITHOUT_CRC 0x3CU /* OFNS Message with no CRC Type */

#if (defined CANTSYN_SYNC_WITH_CRC) /* to prevent double declaration */
#error CANTSYN_SYNC_WITH_CRC is already defined
#endif /* if (defined CANTSYN_SYNC_WITH_CRC) */
#define CANTSYN_SYNC_WITH_CRC 0x20U /* Sync Message with CRC Type */

#if (defined CANTSYN_FUP_WITH_CRC) /* to prevent double declaration */
#error CANTSYN_FUP_WITH_CRC is already defined
#endif /* if (defined CANTSYN_FUP_WITH_CRC) */
#define CANTSYN_FUP_WITH_CRC  0x28U /* Fup Message with no CRC Type */

#if (defined CANTSYN_OFS_WITH_CRC) /* to prevent double declaration */
#error CANTSYN_OFS_WITH_CRC is already defined
#endif /* if (defined CANTSYN_OFS_WITH_CRC) */
#define CANTSYN_OFS_WITH_CRC  0x44U /* Ofs Message with CRC Type */

#if (defined CANTSYN_OFS_EXT_MSG_WITH_CRC) /* to prevent double declaration */
#error CANTSYN_OFS_EXT_MSG_WITH_CRC is already defined
#endif /* if (defined CANTSYN_OFS_EXT_MSG_WIT_CRC) */
#define CANTSYN_OFS_EXT_MSG_WITH_CRC  0x64U /* Extended OFS Message with CRC Type */

#if (defined CANTSYN_OFNS_WITH_CRC) /* to prevent double declaration */
#error CANTSYN_OFNS_WITH_CRC is already defined
#endif /* if (defined CANTSYN_OFNS_WITH_CRC) */
#define CANTSYN_OFNS_WITH_CRC 0x4CU /* Ofns Message with no CRC Type */

#if (defined CANTSYN_SYNCMSG_SEND_WAIT) /* to prevent double declaration */
#error CANTSYN_SYNCMSG_SEND_WAIT is already defined
#endif /* if (defined CANTSYN_SYNCMSG_SEND_WAIT) */
#define CANTSYN_SYNCMSG_SEND_WAIT   0U /* The SYNC message is awaited to be sent */

#if (defined CANTSYN_SYNCMSG_WAIT_TXCONF) /* to prevent double declaration */
#error CANTSYN_SYNCMSG_WAIT_TXCONF is already defined
#endif /* if (defined CANTSYN_SYNCMSG_WAIT_TXCONF) */
#define CANTSYN_SYNCMSG_WAIT_TXCONF 1U /* Tx Confirmation for SYNC Message is awaited */

#if (defined CANTSYN_FUPMSG_SEND_WAIT) /* to prevent double declaration */
#error CANTSYN_FUPMSG_SEND_WAIT is already defined
#endif /* if (defined CANTSYN_FUPMSG_SEND_WAIT) */
#define CANTSYN_FUPMSG_SEND_WAIT    2U /* The FUP message is awaited to be sent */

#if (defined CANTSYN_FUPMSG_WAIT_TXCONF) /* to prevent double declaration */
#error CANTSYN_FUPMSG_WAIT_TXCONF is already defined
#endif /* if (defined CANTSYN_FUPMSG_WAIT_TXCONF) */
#define CANTSYN_FUPMSG_WAIT_TXCONF  3U /* Tx Confirmation for FUP Message is awaited */

#if (defined CANTSYN_OFSMSG_SEND_WAIT) /* to prevent double declaration */
#error CANTSYN_OFSMSG_SEND_WAIT is already defined
#endif /* if (defined CANTSYN_OFSMSG_SEND_WAIT) */
#define CANTSYN_OFSMSG_SEND_WAIT    4U /* The OFS message is awaited to be sent */

#if (defined CANTSYN_OFSMSG_WAIT_TXCONF) /* to prevent double declaration */
#error CANTSYN_OFSMSG_WAIT_TXCONF is already defined
#endif /* if (defined CANTSYN_OFSMSG_WAIT_TXCONF) */
#define CANTSYN_OFSMSG_WAIT_TXCONF  5U /* Tx Confirmation for OFS Message is awaited */

#if (defined CANTSYN_OFNSMSG_SEND_WAIT) /* to prevent double declaration */
#error CANTSYN_OFNSMSG_SEND_WAIT is already defined
#endif /* if (defined CANTSYN_OFNSMSG_SEND_WAIT) */
#define CANTSYN_OFNSMSG_SEND_WAIT   6U /* The OFNS message is awaited to be sent */

#if (defined CANTSYN_OFNSMSG_WAIT_TXCONF) /* to prevent double declaration */
#error CANTSYN_OFNSMSG_WAIT_TXCONF is already defined
#endif /* if (defined CANTSYN_OFNSMSG_WAIT_TXCONF) */
#define CANTSYN_OFNSMSG_WAIT_TXCONF 7U /* Tx Confirmation for OFNS Message is awaited */

#if (defined CANTSYN_SYNCMSG_RX_WAIT) /* to prevent double declaration */
#error CANTSYN_SYNCMSG_RX_WAIT is already defined
#endif /* if (defined CANTSYN_SYNCMSG_RX_WAIT) */
#define CANTSYN_SYNCMSG_RX_WAIT         0U /* The SYNC message is awaited to be received */

#if (defined CANTSYN_FUPMSG_RX_WAIT) /* to prevent double declaration */
#error CANTSYN_FUPMSG_RX_WAIT is already defined
#endif /* if (defined CANTSYN_FUPMSG_RX_WAIT) */
#define CANTSYN_FUPMSG_RX_WAIT          1U /* The FUP message is awaited to be received */

#if(CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON)
#if (defined CANTSYN_FUPMSG_RX_WAIT_TIMEOUT) /* to prevent double declaration */
#error CANTSYN_FUPMSG_RX_WAIT_TIMEOUT is already defined
#endif /* if (defined CANTSYN_FUPMSG_RX_WAIT_TIMEOUT) */
#define CANTSYN_FUPMSG_RX_WAIT_TIMEOUT  2U /* The FUP message is awaited to be received, FollowUpTimeout loaded */
#endif /* (CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON) */

#if (defined CANTSYN_OFSMSG_RX_WAIT) /* to prevent double declaration */
#error CANTSYN_OFSMSG_RX_WAIT is already defined
#endif /* if (defined CANTSYN_OFSMSG_RX_WAIT) */
#define CANTSYN_OFSMSG_RX_WAIT          3U /* The OFS message is awaited to be received */

#if (defined CANTSYN_OFNSMSG_RX_WAIT) /* to prevent double declaration */
#error CANTSYN_OFNSMSG_RX_WAIT is already defined
#endif /* if (defined CANTSYN_OFNSMSG_RX_WAIT) */
#define CANTSYN_OFNSMSG_RX_WAIT         4U /* The OFNS message is awaited to be received */

#if(CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON)
#if (defined CANTSYN_OFNSMSG_RX_WAIT_TIMEOUT) /* to prevent double declaration */
#error CANTSYN_OFNSMSG_RX_WAIT_TIMEOUT is already defined
#endif /* if (defined CANTSYN_OFNSMSG_RX_WAIT_TIMEOUT) */
#define CANTSYN_OFNSMSG_RX_WAIT_TIMEOUT 5U /* The OFNS message is awaited to be received, FollowUpTimeout loaded */
#endif /* (CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON) */

#if (defined CANTSYN_NANOSEC_MAX) /* to prevent double declaration */
#error CANTSYN_NANOSEC_MAX is already defined
#endif /* if (defined CANTSYN_NANOSEC_MAX) */
#define CANTSYN_NANOSEC_MAX (uint32)999999999U /* Maximum nanosecond value used to compute OVS (shall be 10^9ns-1) */

#if (defined CANTSYN_SC_MAXLIMIT) /* to prevent double declaration */
#error CANTSYN_SC_MAXLIMIT is already defined
#endif /* if (defined CANTSYN_SC_MAXLIMIT) */
#define CANTSYN_SC_MAXLIMIT 15U /* Limit of Sequence Counter */

#if (defined CANTSYN_SC_STARTUP_TIMEOUT) /* to prevent double declaration */
#error CANTSYN_SC_STARTUP_TIMEOUT is already defined
#endif /* if (defined CANTSYN_SC_STARTUP_TIMEOUT) */
#define CANTSYN_SC_STARTUP_TIMEOUT  (255U)

#if (defined CANTSYN_STBM_TIMEOUT) /* to prevent double declaration */
#error CANTSYN_STBM_TIMEOUT is already defined
#endif /* if (defined CANTSYN_STBM_TIMEOUT) */
#define CANTSYN_STBM_TIMEOUT  (1U)

/* Time Gateway synchronization status values */
#if (defined CANTSYN_SYNCTOGTM) /* to prevent double declaration */
#error CANTSYN_SYNCTOGTM is already defined
#endif /* if (defined CANTSYN_SYNCTOGTM) */
#define CANTSYN_SYNCTOGTM       0U

#if (defined CANTSYN_SYNCTOSUBDOMAIN) /* to prevent double declaration */
#error CANTSYN_SYNCTOSUBDOMAIN is already defined
#endif /* if (defined CANTSYN_SYNCTOSUBDOMAIN) */
#define CANTSYN_SYNCTOSUBDOMAIN 1U

#if (defined CANTSYN_SYNC_TO_GATEWAY_MASK) /* to prevent double declaration */
#error CANTSYN_SYNC_TO_GATEWAY_MASK is already defined
#endif /* if (defined CANTSYN_SYNC_TO_GATEWAY_MASK) */
#define CANTSYN_SYNC_TO_GATEWAY_MASK  4U

#if (defined CANTSYN_GLOBAL_TIME_BASE_MASK) /* to prevent double declaration */
#error CANTSYN_GLOBAL_TIME_BASE_MASK is already defined
#endif /* if (defined CANTSYN_GLOBAL_TIME_BASE_MASK) */
#define CANTSYN_GLOBAL_TIME_BASE_MASK 8U

#if (defined CANTSYN_TIMEOUTMASK) /* to prevent double declaration */
#error CANTSYN_TIMEOUTMASK is already defined
#endif /* if (defined CANTSYN_TIMEOUTMASK) */
#define CANTSYN_TIMEOUTMASK           1U

#if (defined CANTSYN_SC_MASK) /* to prevent double declaration */
#error CANTSYN_SC_MASK is already defined
#endif /* if (defined CANTSYN_SC_MASK) */
#define CANTSYN_SC_MASK               15U

#if (defined CANTSYN_OVS_MASK) /* to prevent double declaration */
#error CANTSYN_OVS_MASK is already defined
#endif /* if (defined CANTSYN_OVS_MASK) */
#define CANTSYN_OVS_MASK              3U

#if (defined CANTSYN_SGW_MASK) /* to prevent double declaration */
#error CANTSYN_SGW_MASK is already defined
#endif /* if (defined CANTSYN_SGW_MASK) */
#define CANTSYN_SGW_MASK              4U

#if (defined CANTSYN_START_VALUE_8H2F) /* to prevent double declaration */
#error CANTSYN_START_VALUE_8H2F is already defined
#endif /* if (defined CANTSYN_START_VALUE_8H2F) */
#define CANTSYN_START_VALUE_8H2F (0xFFU)

#if (defined CANTSYN_CRC_LENGTH) /* to prevent double declaration */
#error CANTSYN_CRC_LENGTH is already defined
#endif /* if (defined CANTSYN_CRC_LENGTH) */
#define CANTSYN_CRC_LENGTH     (7U)

#if (defined CANTSYN_CRC_EXT_LENGTH) /* to prevent double declaration */
#error CANTSYN_CRC_EXT_LENGTH is already defined
#endif /* if (defined CANTSYN_CRC_EXT_LENGTH) */
#define CANTSYN_CRC_EXT_LENGTH     (15U)

/* Byte numbers */

#if (defined CANTSYN_B0_MSG_TYPE) /* to prevent double declaration */
#error CANTSYN_B0_MSG_TYPE is already defined
#endif /* if (defined CANTSYN_B0_MSG_TYPE) */
#define CANTSYN_B0_MSG_TYPE     0U

#if (defined CANTSYN_B1_MSG) /* to prevent double declaration */
#error CANTSYN_B1_MSG is already defined
#endif /* if (defined CANTSYN_B1_MSG) */
#define CANTSYN_B1_MSG          1U

#if (defined CANTSYN_B2_MSG_DOMAIN) /* to prevent double declaration */
#error CANTSYN_B2_MSG_DOMAIN is already defined
#endif /* if (defined CANTSYN_B2_MSG_DOMAIN) */
#define CANTSYN_B2_MSG_DOMAIN   2U

#if (defined CANTSYN_B3_MSG_RESERVED) /* to prevent double declaration */
#error CANTSYN_B3_MSG_RESERVED is already defined
#endif /* if (defined CANTSYN_B3_MSG_RESERVED) */
#define CANTSYN_B3_MSG_RESERVED 3U

#if (defined CANTSYN_B3_MSG_GATEWAY) /* to prevent double declaration */
#error CANTSYN_B3_MSG_GATEWAY is already defined
#endif /* if (defined CANTSYN_B3_MSG_GATEWAY) */
#define CANTSYN_B3_MSG_GATEWAY  3U

#if (defined CANTSYN_B3_MSG) /* to prevent double declaration */
#error CANTSYN_B3_MSG is already defined
#endif /* if (defined CANTSYN_B3_MSG) */
#define CANTSYN_B3_MSG          3U

#if (defined CANTSYN_B4_MSG) /* to prevent double declaration */
#error CANTSYN_B4_MSG is already defined
#endif /* if (defined CANTSYN_B4_MSG) */
#define CANTSYN_B4_MSG          4U

#if (defined CANTSYN_B5_MSG) /* to prevent double declaration */
#error CANTSYN_B5_MSG is already defined
#endif /* if (defined CANTSYN_B5_MSG) */
#define CANTSYN_B5_MSG          5U

#if (defined CANTSYN_B6_MSG) /* to prevent double declaration */
#error CANTSYN_B6_MSG is already defined
#endif /* if (defined CANTSYN_B6_MSG) */
#define CANTSYN_B6_MSG          6U

#if (defined CANTSYN_B6_MSG_RESERVED) /* to prevent double declaration */
#error CANTSYN_B6_MSG_RESERVED is already defined
#endif /* if (defined CANTSYN_B6_MSG_RESERVED) */
#define CANTSYN_B6_MSG_RESERVED      6U

#if (defined CANTSYN_B7_MSG) /* to prevent double declaration */
#error CANTSYN_B7_MSG is already defined
#endif /* if (defined CANTSYN_B7_MSG) */
#define CANTSYN_B7_MSG          7U

#if (defined CANTSYN_B7_MSG_RESERVED) /* to prevent double declaration */
#error CANTSYN_B7_MSG_RESERVED is already defined
#endif /* if (defined CANTSYN_B7_MSG_RESERVED) */
#define CANTSYN_B7_MSG_RESERVED      7U

#if (defined CANTSYN_B8_RESERVED) /* to prevent double declaration */
#error CANTSYN_B8_RESERVED is already defined
#endif /* if (defined CANTSYN_B8_RESERVED) */
#define CANTSYN_B8_RESERVED          8U

#if (defined CANTSYN_B8_MSG) /* to prevent double declaration */
#error CANTSYN_B8_MSG is already defined
#endif /* if (defined CANTSYN_B8_MSG) */
#define CANTSYN_B8_MSG          8U

#if (defined CANTSYN_B9_MSG) /* to prevent double declaration */
#error CANTSYN_B9_MSG is already defined
#endif /* if (defined CANTSYN_B9_MSG) */
#define CANTSYN_B9_MSG          9U

#if (defined CANTSYN_B10_MSG) /* to prevent double declaration */
#error CANTSYN_B10_MSG is already defined
#endif /* if (defined CANTSYN_B10_MSG) */
#define CANTSYN_B10_MSG          10U

#if (defined CANTSYN_B11_MSG) /* to prevent double declaration */
#error CANTSYN_B11_MSG is already defined
#endif /* if (defined CANTSYN_B11_MSG) */
#define CANTSYN_B11_MSG          11U

#if (defined CANTSYN_B12_MSG) /* to prevent double declaration */
#error CANTSYN_B12_MSG is already defined
#endif /* if (defined CANTSYN_B12_MSG) */
#define CANTSYN_B12_MSG          12U

#if (defined CANTSYN_B13_MSG) /* to prevent double declaration */
#error CANTSYN_B13_MSG is already defined
#endif /* if (defined CANTSYN_B13_MSG) */
#define CANTSYN_B13_MSG          13U

#if (defined CANTSYN_B14_MSG) /* to prevent double declaration */
#error CANTSYN_B14_MSG is already defined
#endif /* if (defined CANTSYN_B14_MSG) */
#define CANTSYN_B14_MSG          14U

#if (defined CANTSYN_B15_MSG) /* to prevent double declaration */
#error CANTSYN_B15_MSG is already defined
#endif /* if (defined CANTSYN_B15_MSG) */
#define CANTSYN_B15_MSG          15U

#if (defined CANTSYN_GET_B3) /* to prevent double declaration */
#error CANTSYN_GET_B3 is already defined
#endif /* if (defined CANTSYN_GET_B3) */
#define CANTSYN_GET_B3 24U

#if (defined CANTSYN_GET_B2) /* to prevent double declaration */
#error CANTSYN_GET_B2 is already defined
#endif /* if (defined CANTSYN_GET_B2) */
#define CANTSYN_GET_B2 16U

#if (defined CANTSYN_GET_B1) /* to prevent double declaration */
#error CANTSYN_GET_B1 is already defined
#endif /* if (defined CANTSYN_GET_B1) */
#define CANTSYN_GET_B1 8U

#if (defined CANTSYN_SEC_PART_SIZE) /* to prevent double declaration */
#error CANTSYN_SEC_PART_SIZE is already defined
#endif /* if (defined CANTSYN_SEC_PART_SIZE) */
#define CANTSYN_SEC_PART_SIZE 4U

#if (defined CANTSYN_NSEC_PART_SIZE) /* to prevent double declaration */
#error CANTSYN_NSEC_PART_SIZE is already defined
#endif /* if (defined CANTSYN_NSEC_PART_SIZE) */
#define CANTSYN_NSEC_PART_SIZE 4U

#if (defined CANTSYN_STOP_IDX_OF_SYNC_MSGS) /* to prevent double declaration */
#error CANTSYN_STOP_IDX_OF_SYNC_MSGS is already defined
#endif /* if (defined CANTSYN_STOP_IDX_OF_SYNC_MSGS) */
#define CANTSYN_STOP_IDX_OF_SYNC_MSGS 15U

#if (defined CANTSYN_START_IDX_OF_OFS_MSGS) /* to prevent double declaration */
#error CANTSYN_START_IDX_OF_OFS_MSGS is already defined
#endif /* if (defined CANTSYN_START_IDX_OF_OFS_MSGS) */
#define CANTSYN_START_IDX_OF_OFS_MSGS 16U

#if (defined CANTSYN_NO_IMMEDIATE_TRANSMISSION) /* to prevent double declaration */
#error CANTSYN_NO_IMMEDIATE_TRANSMISSION is already defined
#endif /* if (defined CANTSYN_NO_IMMEDIATE_TRANSMISSION) */
#define CANTSYN_NO_IMMEDIATE_TRANSMISSION 0U

#if (defined CANTSYN_GET_B2_DOMAIN_SC) /* to prevent double declaration */
#error CANTSYN_GET_B2_DOMAIN_SC is already defined
#endif /* if (defined CANTSYN_GET_B2_DOMAIN_SC) */
#define CANTSYN_GET_B2_DOMAIN_SC(DomainId, SeqCount) (((uint8)((DomainId) << 4U)) | (SeqCount))

#if (defined CANTSYN_B3_SGW_OVS) /* to prevent double declaration */
#error CANTSYN_B3_SGW_OVS is already defined
#endif /* if (defined CANTSYN_B3_SGW_OVS) */
#define CANTSYN_B3_SGW_OVS(Index, Ovs)               ((uint8)(CanTSyn_SyncMaster[(Index)].SyncSGWBit << 2U) | (Ovs))


#if (defined CANTSYN_B3_SGW) /* to prevent double declaration */
#error CANTSYN_B3_SGW is already defined
#endif /* if (defined CANTSYN_B3_SGW) */
#define CANTSYN_B3_SGW(Index)               ((uint8)(CanTSyn_OffsetMaster[(Index)].OfsSGWBit))

#if (defined CANTSYN_GET_SEQ_COUNTER) /* to prevent double declaration */
#error CANTSYN_GET_SEQ_COUNTER is already defined
#endif /* if (defined CANTSYN_GET_SEQ_COUNTER) */
#define CANTSYN_GET_SEQ_COUNTER(SeqCounter)          (((SeqCounter) != 0U)?((SeqCounter)-1U):(CANTSYN_SC_MAXLIMIT))

#if (defined CANTSYN_SET_SEQ_COUNTER) /* to prevent double declaration */
#error CANTSYN_SET_SEQ_COUNTER is already defined
#endif /* if (defined CANTSYN_SET_SEQ_COUNTER) */
#define CANTSYN_SET_SEQ_COUNTER(SeqCounter)          (((SeqCounter) < CANTSYN_SC_MAXLIMIT)?((SeqCounter)+1U):(0U))

#if (defined CANTSYN_GET_USERBYTE0) /* to prevent double declaration */
#error CANTSYN_GET_USERBYTE0 is already defined
#endif /* if (defined CANTSYN_GET_USERBYTE0) */
#define CANTSYN_GET_USERBYTE0(UserData) (((UserData).userDataLength > 0U)?((UserData).userByte0):(0U))

#if (defined CANTSYN_GET_USERBYTE1) /* to prevent double declaration */
#error CANTSYN_GET_USERBYTE1 is already defined
#endif /* if (defined CANTSYN_GET_USERBYTE1) */
#define CANTSYN_GET_USERBYTE1(UserData) (((UserData).userDataLength > 1U)?((UserData).userByte1):(0U))

#if (defined CANTSYN_GET_USERBYTE2) /* to prevent double declaration */
#error CANTSYN_GET_USERBYTE2 is already defined
#endif /* if (defined CANTSYN_GET_USERBYTE2) */
#define CANTSYN_GET_USERBYTE2(UserData) (((UserData).userDataLength > 2U)?((UserData).userByte2):(0U))

#if (defined CANTSYN_GET_SC_SLAVE) /* to prevent double declaration */
#error CANTSYN_GET_SC_SLAVE is already defined
#endif /* if (defined CANTSYN_GET_SC_SLAVE) */
#define CANTSYN_GET_SC_SLAVE(Byte2)       ((Byte2) & 0x0FU)

#if (defined CANTSYN_GET_DOMAINID_SLAVE) /* to prevent double declaration */
#error CANTSYN_GET_DOMAINID_SLAVE is already defined
#endif /* if (defined CANTSYN_GET_DOMAINID_SLAVE) */
#define CANTSYN_GET_DOMAINID_SLAVE(Byte2) (uint8)((Byte2) >> 4U)

#if(CANTSYN_RX_SYNC_USED == STD_ON)

#if (defined CANTSYN_IS_CRC_VAL_SYNC) /* to prevent double declaration */
#error CANTSYN_IS_CRC_VAL_SYNC is already defined
#endif /* if (defined CANTSYN_IS_CRC_VAL_SYNC) */
#define CANTSYN_IS_CRC_VAL_SYNC(PduId, Type)     (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_VALIDATED) && \
                                                      ((Type) == 0x20U))?TRUE:FALSE)

#if (defined CANTSYN_IS_CRC_NOT_VAL_SYNC) /* to prevent double declaration */
#error CANTSYN_IS_CRC_NOT_VAL_SYNC is already defined
#endif /* if (defined CANTSYN_IS_CRC_NOT_VAL_SYNC) */
#define CANTSYN_IS_CRC_NOT_VAL_SYNC(PduId, Type) (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_NOT_VALIDATED) && \
                                                      ((Type) == 0x10U))?TRUE:FALSE)

#if (defined CANTSYN_IS_CRC_IGNORED_SYNC) /* to prevent double declaration */
#error CANTSYN_IS_CRC_IGNORED_SYNC is already defined
#endif /* if (defined CANTSYN_IS_CRC_IGNORED_SYNC) */
#define CANTSYN_IS_CRC_IGNORED_SYNC(PduId, Type) (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_IGNORED) && \
                                                     (((Type) == 0x10U) || ((Type) == 0x20U)))?TRUE:FALSE)

#if (defined CANTSYN_IS_CRC_OPTIONAL_SYNC) /* to prevent double declaration */
#error CANTSYN_IS_CRC_OPTIONAL_SYNC is already defined
#endif /* if (defined CANTSYN_IS_CRC_OPTIONAL_SYNC) */
#define CANTSYN_IS_CRC_OPTIONAL_SYNC(PduId, Type) (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_OPTIONAL) && \
                                                     (((Type) == 0x10U) || ((Type) == 0x20U)))?TRUE:FALSE)

#if (defined CANTSYN_IS_SYNC_CRC_TYPE) /* to prevent double declaration */
#error CANTSYN_IS_SYNC_CRC_TYPE is already defined
#endif /* if (defined CANTSYN_IS_SYNC_CRC_TYPE) */
#define CANTSYN_IS_SYNC_CRC_TYPE(Type) (((Type) == 0x20U)?TRUE:FALSE)

#if (defined CANTSYN_IS_CRC_VAL_FUP) /* to prevent double declaration */
#error CANTSYN_IS_CRC_VAL_FUP is already defined
#endif /* if (defined CANTSYN_IS_CRC_VAL_FUP) */
#define CANTSYN_IS_CRC_VAL_FUP(PduId, Type)      (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_VALIDATED) && \
                                                      ((Type) == 0x28U))?TRUE:FALSE)

#if (defined CANTSYN_IS_CRC_NOT_VAL_FUP) /* to prevent double declaration */
#error CANTSYN_IS_CRC_NOT_VAL_FUP is already defined
#endif /* if (defined CANTSYN_IS_CRC_NOT_VAL_FUP) */
#define CANTSYN_IS_CRC_NOT_VAL_FUP(PduId, Type)  (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_NOT_VALIDATED) && \
                                                      ((Type) == 0x18U))?TRUE:FALSE)

#if (defined CANTSYN_IS_CRC_IGNORED_FUP) /* to prevent double declaration */
#error CANTSYN_IS_CRC_IGNORED_FUP is already defined
#endif /* if (defined CANTSYN_IS_CRC_IGNORED_FUP) */
#define CANTSYN_IS_CRC_IGNORED_FUP(PduId, Type)  (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_IGNORED) && \
                                                     (((Type) == 0x18U) || ((Type) == 0x28U)))?TRUE:FALSE)

#if (defined CANTSYN_IS_CRC_OPTIONAL_FUP) /* to prevent double declaration */
#error CANTSYN_IS_CRC_OPTIONAL_FUP is already defined
#endif /* if (defined CANTSYN_IS_CRC_OPTIONAL_FUP) */
#define CANTSYN_IS_CRC_OPTIONAL_FUP(PduId, Type)  (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_OPTIONAL) && \
                                                     (((Type) == 0x18U) || ((Type) == 0x28U)))?TRUE:FALSE)

#if (defined CANTSYN_IS_FUP_CRC_TYPE) /* to prevent double declaration */
#error CANTSYN_IS_FUP_CRC_TYPE is already defined
#endif /* if (defined CANTSYN_IS_FUP_CRC_TYPE) */
#define CANTSYN_IS_FUP_CRC_TYPE(Type) (((Type) == 0x28U)?TRUE:FALSE)

#endif /* (CANTSYN_RX_SYNC_USED == STD_ON) */


#if(CANTSYN_RX_OFFSET_USED == STD_ON)

#if (defined CANTSYN_IS_CRC_VAL_OFS) /* to prevent double declaration */
#error CANTSYN_IS_CRC_VAL_OFS is already defined
#endif /* if (defined CANTSYN_IS_CRC_VAL_OFS) */
#define CANTSYN_IS_CRC_VAL_OFS(PduId, Type)      (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_VALIDATED) && \
                                                      ((Type) == 0x44U))?TRUE:FALSE)

#if (defined CANTSYN_IS_EXT_CRC_VAL_OFS) /* to prevent double declaration */
#error CANTSYN_IS_EXT_CRC_VAL_OFS is already defined
#endif /* if (defined CANTSYN_IS_EXT_CRC_VAL_OFS) */
#define CANTSYN_IS_EXT_CRC_VAL_OFS(PduId, Type)      (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_VALIDATED) && \
                                                      ((Type) == 0x64U))?TRUE:FALSE)

#if (defined CANTSYN_IS_CRC_NOT_VAL_OFS) /* to prevent double declaration */
#error CANTSYN_IS_CRC_NOT_VAL_OFS is already defined
#endif /* if (defined CANTSYN_IS_CRC_NOT_VAL_OFS) */
#define CANTSYN_IS_CRC_NOT_VAL_OFS(PduId, Type)  (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_NOT_VALIDATED) && \
                                                      ((Type) == 0x34U))?TRUE:FALSE)

#if (defined CANTSYN_IS_EXT_CRC_NOT_VAL_OFS) /* to prevent double declaration */
#error CANTSYN_IS_EXT_CRC_NOT_VAL_OFS is already defined
#endif /* if (defined CANTSYN_IS_EXT_CRC_NOT_VAL_OFS) */
#define CANTSYN_IS_EXT_CRC_NOT_VAL_OFS(PduId, Type)  (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_NOT_VALIDATED) && \
                                                      ((Type) == 0x54U))?TRUE:FALSE)

#if (defined CANTSYN_IS_CRC_IGNORED_OFS) /* to prevent double declaration */
#error CANTSYN_IS_CRC_IGNORED_OFS is already defined
#endif /* if (defined CANTSYN_IS_CRC_IGNORED_OFS) */
#define CANTSYN_IS_CRC_IGNORED_OFS(PduId, Type)  (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_IGNORED) && \
                                                      (((Type) == 0x34U) || ((Type) == 0x44U)))?TRUE:FALSE)

#if (defined CANTSYN_IS_EXT_CRC_IGNORED_OFS) /* to prevent double declaration */
#error CANTSYN_IS_EXT_CRC_IGNORED_OFS is already defined
#endif /* if (defined CANTSYN_IS_EXT_CRC_IGNORED_OFS) */
#define CANTSYN_IS_EXT_CRC_IGNORED_OFS(PduId, Type)  (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_IGNORED) && \
                                                      (((Type) == 0x54U) || ((Type) == 0x64U)))?TRUE:FALSE)

#if (defined CANTSYN_IS_CRC_OPTIONAL_OFS) /* to prevent double declaration */
#error CANTSYN_IS_CRC_OPTIONAL_OFS is already defined
#endif /* if (defined CANTSYN_IS_CRC_OPTIONAL_OFS) */
#define CANTSYN_IS_CRC_OPTIONAL_OFS(PduId, Type)  (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_OPTIONAL) && \
                                                      (((Type) == 0x34U) || ((Type) == 0x44U)))?TRUE:FALSE)

#if (defined CANTSYN_IS_OFS_CRC_TYPE) /* to prevent double declaration */
#error CANTSYN_IS_OFS_CRC_TYPE is already defined
#endif /* if (defined CANTSYN_IS_OFS_CRC_TYPE) */
#define CANTSYN_IS_OFS_CRC_TYPE(Type) (((Type) == 0x44U)?TRUE:FALSE)

#if (defined CANTSYN_IS_EXT_CRC_OPTIONAL_OFS) /* to prevent double declaration */
#error CANTSYN_IS_EXT_CRC_OPTIONAL_OFS is already defined
#endif /* if (defined CANTSYN_IS_EXT_CRC_OPTIONAL_OFS) */
#define CANTSYN_IS_EXT_CRC_OPTIONAL_OFS(PduId, Type)  (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_OPTIONAL) && \
                                                      (((Type) == 0x54U) || ((Type) == 0x64U)))?TRUE:FALSE)

#if (defined CANTSYN_IS_OFS_EXT_CRC_TYPE) /* to prevent double declaration */
#error CANTSYN_IS_OFS_EXT_CRC_TYPE is already defined
#endif /* if (defined CANTSYN_IS_OFS_EXT_CRC_TYPE) */
#define CANTSYN_IS_OFS_EXT_CRC_TYPE(Type) (((Type) == 0x64U)?TRUE:FALSE)

#if (defined CANTSYN_IS_CRC_VAL_OFNS) /* to prevent double declaration */
#error CANTSYN_IS_CRC_VAL_OFNS is already defined
#endif /* if (defined CANTSYN_IS_CRC_VAL_OFNS) */
#define CANTSYN_IS_CRC_VAL_OFNS(PduId, Type)     (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_VALIDATED) && \
                                                      ((Type) == 0x4CU))?TRUE:FALSE)

#if (defined CANTSYN_IS_CRC_NOT_VAL_OFNS) /* to prevent double declaration */
#error CANTSYN_IS_CRC_NOT_VAL_OFNS is already defined
#endif /* if (defined CANTSYN_IS_CRC_NOT_VAL_OFNS) */
#define CANTSYN_IS_CRC_NOT_VAL_OFNS(PduId, Type) (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_NOT_VALIDATED) && \
                                                      ((Type) == 0x3CU))?TRUE:FALSE)

#if (defined CANTSYN_IS_CRC_IGNORED_OFNS) /* to prevent double declaration */
#error CANTSYN_IS_CRC_IGNORED_OFNS is already defined
#endif /* if (defined CANTSYN_IS_CRC_IGNORED_OFNS) */
#define CANTSYN_IS_CRC_IGNORED_OFNS(PduId, Type) (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_IGNORED) && \
                                                      (((Type) == 0x3CU) || ((Type) == 0x4CU)))?TRUE:FALSE)

#if (defined CANTSYN_IS_CRC_OPTIONAL_OFNS) /* to prevent double declaration */
#error CANTSYN_IS_CRC_OPTIONAL_OFNS is already defined
#endif /* if (defined CANTSYN_IS_CRC_OPTIONAL_OFNS) */
#define CANTSYN_IS_CRC_OPTIONAL_OFNS(PduId, Type) (((CanTSyn_TimeSlaveConfig[(PduId)].RxCrcValidated == CANTSYN_CRC_OPTIONAL) && \
                                                      (((Type) == 0x3CU) || ((Type) == 0x4CU)))?TRUE:FALSE)

#if (defined CANTSYN_IS_OFNS_CRC_TYPE) /* to prevent double declaration */
#error CANTSYN_IS_OFNS_CRC_TYPE is already defined
#endif /* if (defined CANTSYN_IS_OFNS_CRC_TYPE) */
#define CANTSYN_IS_OFNS_CRC_TYPE(Type) (((Type) == 0x4CU)?TRUE:FALSE)

#endif /* (CANTSYN_RX_OFFSET_USED == STD_ON) */


#if((CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON))

#if (defined CANTSYN_DELTA_SC) /* to prevent double declaration */
#error CANTSYN_DELTA_SC is already defined
#endif /* if (defined CANTSYN_DELTA_SC) */
#define CANTSYN_DELTA_SC(receivedSeqCount, seqCount)  (((receivedSeqCount) >= (seqCount))?(uint8)((receivedSeqCount) - (seqCount)) : \
                                                             (uint8)(((receivedSeqCount) + CANTSYN_SC_MAXLIMIT + 1U) - (seqCount)))
#endif/* (CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON) */

#if (defined CANTSYN_NS_PER_SEC) /* to prevent double declaration */
#error CANTSYN_NS_PER_SEC is already defined
#endif /* if (defined CANTSYN_NS_PER_SEC) */
#define CANTSYN_NS_PER_SEC 1000000000U /* Nanoseconds in one second */

#if (defined CANTSYN_MAX_32BIT_VALUE) /* to prevent double declaration */
#error CANTSYN_MAX_32BIT_VALUE is already defined
#endif /* if (defined CANTSYN_MAX_32BIT_VALUE) */
#define CANTSYN_MAX_32BIT_VALUE 0xFFFFFFFFU /* Maximum value of seconds portion */

#if (defined CANTSYN_GET_TIME_VAL) /* to prevent double declaration */
#error CANTSYN_GET_TIME_VAL is already defined
#endif /* if (defined CANTSYN_GET_TIME_VAL) */
#define CANTSYN_GET_TIME_VAL(DataPtr) (((((uint32) ((uint32) DataPtr[CANTSYN_B4_MSG]) << 24U)  | \
                                         ((uint32) ((uint32) DataPtr[CANTSYN_B5_MSG]) << 16U)) | \
                                         ((uint32) ((uint32) DataPtr[CANTSYN_B6_MSG]) << 8U))  | \
                                                   ((uint32) DataPtr[CANTSYN_B7_MSG]))

#define CANTSYN_GET_EXT_TIME_VAL(DataPtr) (((((uint32) ((uint32) DataPtr[CANTSYN_B8_MSG]) << 24U)  | \
                                             ((uint32) ((uint32) DataPtr[CANTSYN_B9_MSG]) << 16U)) | \
                                             ((uint32) ((uint32) DataPtr[CANTSYN_B10_MSG]) << 8U))  | \
                                                       ((uint32) DataPtr[CANTSYN_B11_MSG]))

#define CANTSYN_GET_EXT_NANO_TIME_VAL(DataPtr) (((((uint32) ((uint32) DataPtr[CANTSYN_B12_MSG]) << 24U)  | \
                                                  ((uint32) ((uint32) DataPtr[CANTSYN_B13_MSG]) << 16U)) | \
                                                  ((uint32) ((uint32) DataPtr[CANTSYN_B14_MSG]) << 8U))  | \
                                                            ((uint32) DataPtr[CANTSYN_B15_MSG]))

/*==================[internal function declarations]=======================*/

#define CANTSYN_START_SEC_CODE
#include <CanTSyn_MemMap.h>


#if(CANTSYN_TX_SYNC_USED == STD_ON)
/**
 * \brief Service to assemble the SYNC Frame.
 * \param[in] CanTSynPduIdx - Index of the active TxPdu.
 * \param[out] TxDataPtr - Pointer containing the corresponding data.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_FillTxSyncFrame
(
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_DATA) TxDataPtr,
  uint8 CanTSynPduIdx
);

/**
 * \brief Service to assemble the FUP Frame.
 * \param[in] CanTSynPduIdx - Index of the active TxPdu.
 *            OvsBit - The OVS bit
 * \param[out] TxDataPtr - Pointer containing the corresponding data.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_FillTxFupFrame
(
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_DATA) TxDataPtr,
  uint8 CanTSynPduIdx,
  uint8 CanTSynOvsBit
);

/**
 * \brief Service to clear the sync state machine of the CanTSyn module.
 * \param[in] ConfirmationHandleId - Index of the active TxPdu.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_TxSyncReset
(
  uint8 ConfirmationHandleId
);


/**
 * \brief Service to send the SYNC message to CanIf.
 * \param[in] ConfirmationHandleId        - Index of the TxPdu.
 *            IsImmediateSYNCTransmission - Indication if an immediate transmission takes place or not.
 * \return E_OK: CanTSyn message sent.
 *         E_NOT_OK: No message sent or CanIf returned E_NOT_OK.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_SendSyncMessage
(
  uint8 ConfirmationHandleId,
  boolean IsImmediateSYNCTransmission
);


/**
 * \brief Service to sum two StbM_TimeStamps.
 * \param[in] timeStampAugendPtr        - pointer to first constant StbM_TimeStamp.
 *            timeStampAddendPtr        - pointer to second constant StbM_TimeStamp.
 * \return StbM_TimeStamp: resulted (sum of the two parameters) StbM_TimeStamp
 */
STATIC FUNC(StbM_TimeStampType, CANTSYN_CODE) CanTSyn_AddTimestamp
(
    P2CONST(StbM_TimeStampType, AUTOMATIC, CANTSYN_VAR) timeStampAugendPtr,
    P2CONST(StbM_TimeStampType, AUTOMATIC, CANTSYN_VAR) timeStampAddendPtr
);

#if (CANTSYN_MASTER_TIME_DOMAIN_VALIDATION_SUPPORT == STD_ON)
/**
 * \brief Service to sum two StbM_TimeStamps.
 * \param[in] timeStampAugendPtr        - pointer to first constant StbM_TimeStamp.
 *            timeStampAddendPtr        - pointer to second constant StbM_TimeStamp.
 * \return StbM_TimeStamp: resulted (sum of the two parameters) StbM_TimeStamp
 */
STATIC FUNC(StbM_TimeStampType, CANTSYN_CODE) CanTSyn_AddTimestamp_TimeValidation
(
    P2CONST(StbM_TimeStampType, AUTOMATIC, CANTSYN_VAR) timeStampAugendPtr,
    P2CONST(StbM_TimeStampType, AUTOMATIC, CANTSYN_VAR) timeStampAddendPtr
);
#endif /* (CANTSYN_MASTER_TIME_DOMAIN_VALIDATION_SUPPORT == STD_ON) */

/**
 * \brief Service to send the FUP message to CanIf.
 * \param[in] ConfirmationHandleId - Index of the TxPdu.
 *
 * \return E_OK: CanTSyn message sent.
 *         E_NOT_OK: No message sent or CanIf returned E_NOT_OK.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_SendFupMessage
(
  uint8 ConfirmationHandleId
);

/**
 * \brief Service to enable the transmission of the SYNC message.
 * \param[in] NewTimeBaseCounter         - UpdateCounter received from StbM
 *            isImmediateTransmissionPtr - Indication if an immediate
 *                                         transmission takes place or not.
 *            PduId                      - Index of the TxPdu.
 * \return E_OK:     CanTSyn message shall be sent.
 *         E_NOT_OK: No message shall be sent until timeout or immediate transmission flag set.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsTxSYNCMessageAwaited
(
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_DATA) NewTimeBaseCounter,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) IsImmediateTransmissionPtr,
  uint8 PduId
);

/**
 * \brief Service to process the Tx SYNC messages of the CanTSyn module.
 * \param[in] ConfirmationHandleId - Index of the active TxPdu.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_TxProcessSyncMsg
(
  uint8 ConfirmationHandleId
);

/**
 * \brief Service to process the confirmation for Tx SYNC messages of the CanTSyn module.
 * \param[in] CanTSyn_PduActiveIndex - Index of the active TxPdu.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ConfirmSyncMsg
(
  uint8 CanTSyn_PduActiveIndex
);
#endif /* CANTSYN_TX_SYNC_USED == STD_ON */


#if(CANTSYN_TX_OFFSET_USED == STD_ON)
/**
 * \brief Service to process the confirmation for Tx OFS messages of the CanTSyn module.
 * \param[in] CanTSyn_PduActiveIndex - Index of the active TxPdu.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ConfirmOfsMsg
(
  uint8 CanTSyn_PduActiveIndex
);

/**
 * \brief Service to assemble the OFS Frame.
 * \param[in] CanTSynPduIdx - Index of the active TxPdu.
 *            CanTSynTimeStampPtr - The Seconds part(Lo and Hi) of the Offset.
 * \param[out] CanTSynTxDataPtr - Pointer containing the corresponding data.
 */

STATIC FUNC(void, CANTSYN_CODE) CanTSyn_FillTxOfsFrame
(
  P2CONST(StbM_TimeStampType, AUTOMATIC, CANTSYN_APPL_CONST) CanTSynTimeStampPtr,
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_DATA) CanTSynTxDataPtr,
  uint8 CanTSynPduIdx
);

/**
 * \brief Service to assemble the OFNS Frame.
 * \param[in] CanTSynPduIdx - Index of the active TxPdu.
 *
 * \param[out] CanTSynTxDataPtr - Pointer containing the corresponding data.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_FillTxOfnsFrame
(
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_DATA) CanTSynTxDataPtr,
  uint8 CanTSynPduIdx
);

/**
 * \brief Service to clear the offset state machine of the CanTSyn module.
 * \param[in] ConfirmationHandleId - Index of the active PduId.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_TxOffsetReset
(
  uint8 ConfirmationHandleId
);

/**
 * \brief Service to send the OFS and OFNS message to CanIf.
 * \param[in] CanTSynPduIndex - Index of the TxPdu.
 *            CanTSynTxStatus - Current Tx status.
 *            CanTSynIsImmediateOFSTransmission - Indication if an immediate transmission takes place or not
 *
 * \return E_OK: CanTSyn message sent.
 *         E_NOT_OK: No message sent or CanIf returned E_NOT_OK.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_SendOfsMessage
(
  uint8 CanTSynPduIndex,
  uint8 CanTSynTxStatus,
  boolean CanTSynIsImmediateOFSTransmission
);

/**
 * \brief Service to enable the transmission of the OFS message.
 * \param[in] NewTimeBaseCounter             - UpdateCounter received from StbM.
 *            isImmediateSYNCTransmissionPtr - Indication if an immediate
 *                                             transmission takes place or not.
 *            PduId                          - Index of the TxPdu.
 * \return E_OK:     CanTSyn message shall be sent.
 *         E_NOT_OK: No message shall be sent until timeout or immediate transmission flag set.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsTxOFSMessageAwaited
(
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_DATA) NewTimeBaseCounter,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) IsImmediateTransmissionPtr,
  uint8 PduId
);

/**
 * \brief Service to process the Tx OFS messages of the CanTSyn module.
 * \param[in] ConfirmationHandleId - Index of the active PduId.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_TxProcessOfsMsg
(
  uint8 ConfirmationHandleId
);

#endif /* CANTSYN_TX_OFFSET_USED == STD_ON */

#if ((CANTSYN_TX_SYNC_USED == STD_ON) || (CANTSYN_RX_SYNC_USED == STD_ON))
/**
* \brief Service to subtract two StbM_VirtualLocalTimes.
* \param[in] virtualLocalTimeMinuendPtr     - pointer to constant StbM_VirtualLocalTime minuend.
*            virtualLocalTimeSubtrahendPtr  - pointer to constant StbM_VirtualLocalTime subtrahend.
* \return StbM_TimeStamp:          resulted StbM_VirtualLocalTime after subtraction and conversion
*/
STATIC FUNC(StbM_TimeStampType, CANTSYN_CODE) CanTSyn_SubtractVLT
(
    P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, CANTSYN_VAR) virtualLocalTimeMinuendPtr,
    P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, CANTSYN_VAR) virtualLocalTimeSubtrahendPtr
);
#endif /* ((CANTSYN_TX_SYNC_USED == STD_ON) || (CANTSYN_RX_SYNC_USED == STD_ON)) */

#if(CANTSYN_TX_CRC_USED == STD_ON)
/**
 * \brief Service to calculate the normal message CRC value.
 * \param[in] DataId    - Data Id value.
 *            TxDataPtr - The Seconds part(Lo and Hi) of the Offset.
 *            CrcLength - CRC Length 7 (Normal message) or 15 (Extended message)
 * \param[out] The CRC value.
 */
STATIC FUNC(uint8, CANTSYN_CODE) CanTSyn_CalculateCRC
(
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) TxDataPtr,
  CanTSyn_DataIDListType DataId,
  uint8 CrcLength
);
#endif /* CANTSYN_TX_CRC_USED == STD_ON */


#if((CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON))
/**
 * \brief Service to validate the arguments of CanTSyn_RxIndication.
 * \param[in] RxPduId - The Slave Pdu ID.
 *
 * \param[in] PduInfoPtr  - Contains the length (SduLength) of the received PDU, a pointer to a buffer (SduDataPtr) containing the PDU.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_RxIndication_Validation
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, CANTSYN_APPL_DATA) PduInfoPtr
);

/**
 * \brief Service to get the status of the current Domain.
 * \param[in] SlavePduId - The Slave Pdu ID.
 *
 * \param[out] RxStatusPtr  - The Seconds part(Lo and Hi) of the Offset.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_GetRxStatus
(
  uint8 SlavePduId,
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_DATA) RxStatusPtr
);

#if(CANTSYN_RX_SYNC_USED == STD_ON)
/**
 * \brief Service to check if received message is FUP type.
 * \param[in] SlavePduId - The Slave Pdu ID.
 *            Type       - Type of the message (read from SduDataPtr)
 *
 * \param[out] CrcValidation - Indication if the Crc validation is to be performed.
 *
 * \return E_OK: Message Type accepted.
 *         E_NOT_OK: Message Type not accepted for this SlaveDomainId
 *                   and RxStatus.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsFUPType
(
  uint8 SlavePduId,
  uint8 Type,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) CrcValidation
);

/**
 * \brief Service to check if received message is SYNC type.
 * \param[in] SlavePduId - The Slave Pdu ID.
 *            Type       - Type of the message (read from SduDataPtr)
 *
 * \param[out] CrcValidation - Indication if the Crc validation is to be performed.
 *
 * \return E_OK: Message Type accepted.
 *         E_NOT_OK: Message Type not accepted for this SlaveDomainId
 *                   and RxStatus.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsSYNCType
(
  uint8 SlavePduId,
  uint8 Type,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) CrcValidation
);
#endif /* CANTSYN_RX_SYNC_USED == STD_ON */

#if(CANTSYN_RX_OFFSET_USED == STD_ON)
/**
 * \brief Service to check if received message is OFNS type.
 * \param[in] SlavePduId - The Slave Pdu ID.
 *            Type       - Type of the message (read from SduDataPtr)
 *
 * \param[out] CrcValidation - Indication if the Crc validation is to be performed.
 *
 * \return E_OK: Message Type accepted.
 *         E_NOT_OK: Message Type not accepted for this SlaveDomainId
 *                   and RxStatus.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsOFNSType
(
  uint8 SlavePduId,
  uint8 Type,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) CrcValidation
);

/**
 * \brief Service to check if received message is OFS type.
 * \param[in] SlavePduId - The Slave Pdu ID.
 *            Type       - Type of the message (read from SduDataPtr)
 *
 * \param[out] CrcValidation - Indication if the Crc validation is to be performed.
 *
 * \return E_OK: Message Type accepted.
 *         E_NOT_OK: Message Type not accepted for this SlaveDomainId
 *                   and RxStatus.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsOFSType
(
  uint8 SlavePduId,
  uint8 Type,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) CrcValidation
);

#if(CANTSYN_RX_USE_EXTENDED_MSG == STD_ON)
/**
 * \brief Service to check if received message is an Extended OFS type.
 * \param[in] SlavePduId - The Slave Pdu ID.
 *            Type       - Type of the message (read from SduDataPtr)
 *
 * \param[out] CrcValidation - Indication if the Crc validation is to be performed.
 *
 * \return E_OK: Message Type accepted.
 *         E_NOT_OK: Message Type not accepted for this SlaveDomainId
 *                   and RxStatus.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsOFSExtenedType
(
  uint8 SlavePduId,
  uint8 Type,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) CrcValidation
);
#endif /* CANTSYN_RX_USE_EXTENDED_MSG == STD_ON */

/**
 * \brief Service to check if received message is a Normal OFS type.
 * \param[in] SlavePduId - The Slave Pdu ID.
 *            Type       - Type of the message (read from SduDataPtr)
 *
 * \param[out] CrcValidation - Indication if the Crc validation is to be performed.
 *
 * \return E_OK: Message Type accepted.
 *         E_NOT_OK: Message Type not accepted for this SlaveDomainId
 *                   and RxStatus.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsOFSNormalType
(
  uint8 SlavePduId,
  uint8 Type,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) CrcValidation
);
#endif /* CANTSYN_RX_OFFSET_USED == STD_ON */

/**
 * \brief Service to check the Message CRC of the received message.
 * \param[in] SlavePduId    - Slave Time Domain Id.
 *            RxStatus         - Status of the current Slave Time Domain.
 *            DataPtr          - Pointer containing the data needed for message
 *                               type and CRC computation.
 *
 * \param[out] CrcValidation - Indication if the CRC validation is to be performed.
 *
 * \return E_OK: Message Type accepted.
 *         E_NOT_OK: Message Type not accepted for this SlaveDomainId
 *                   and RxStatus.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_CheckMsgType
(
  uint8 SlavePduId,
  uint8 RxStatus,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) CrcValidation
);

#if(CANTSYN_RX_CRC_USED == STD_ON)
/**
 * \brief Service to check the Message CRC of the received message.
 * \param[in] SlavePduId    - Slave Time Domain Id.
 *            RxStatus         - Status of the current Slave Time Domain.
 *            DataPtr          - Pointer containing the data needed for message
 *                               type and CRC computation.
 *
 * \return E_OK: Message CRC accepted.
 *         E_NOT_OK: Message CRC not accepted for this SlaveDomainId
 *                   and RxStatus.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_CheckMsgCRC
(
  uint8 SlavePduId,
  uint8 RxStatus,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr
);
#endif /* CANTSYN_RX_CRC_USED == STD_ON */

/**
 * \brief Service to check if the Sequence Counter of the received message shall be accepted.
 * \param[in] RxPduId          - Slave Pdu Id.
 *            ReceivedSC       - The Sequence Counter received in the .
 *            IsSync           - Indication if the received message is a SYNC or a OFS message.
 *
 * \return E_OK: Message with received sequence counter accepted.
 *         E_NOT_OK: Message with received sequence counter not accepted for this RxPduId.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_CheckSequenceCounter
(
  uint8 RxPduId,
  uint8 ReceivedSC,
  boolean IsSync
);
#endif /* (CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON) */

#if((CANTSYN_RX_SYNC_USED == STD_ON) && (CANTSYN_RX_CRC_USED == STD_ON))
/**
 * \brief Function used for time slave configuration in the case of a RX wait for a SYNC message.
 * \param[in]  SlavePduId      - Slave Time Domain Id.
 * \param[in]  DataPtr         - Pointer containing the data needed for message
 *                               CRC computation.
 *
 * \return E_OK: Message Type and CRC accepted.
 *         E_NOT_OK: CRC not accepted for this SlaveDomainId
 *                   and RxStatus.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_SyncMsg_Rx_CRC_Check
(
  uint8 SlavePduId,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr
);

/**
 * \brief Function used for time slave configuration in the case of a RX wait (wait timeout) for
 * a FUP message.
 * \param[in]  SlavePduId      - Slave Time Domain Id.
 * \param[in]  DataPtr         - Pointer containing the data needed for message
 *                               CRC computation.
 *
 * \return E_OK: Message Type and CRC accepted.
 *         E_NOT_OK: CRC not accepted for this SlaveDomainId
 *                   and RxStatus.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_FupMsg_Rx_CRC_Check
(
  uint8 SlavePduId,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr
);
#endif /* (CANTSYN_RX_SYNC_USED == STD_ON) && (CANTSYN_RX_CRC_USED == STD_ON) */

#if((CANTSYN_RX_OFFSET_USED == STD_ON) && (CANTSYN_RX_CRC_USED == STD_ON))
/**
 * \brief Function used for time slave configuration in the case of a RX wait for an OFS message.
 * \param[in]  SlavePduId      - Slave Time Domain Id.
 * \param[in]  DataPtr         - Pointer containing the data needed for message
 *                               CRC computation.
 *
 * \return E_OK: Message Type and CRC accepted.
 *         E_NOT_OK: CRC not accepted for this SlaveDomainId
 *                   and RxStatus.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_OfsMsg_Rx_CRC_Check
(
  uint8 SlavePduId,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr
);

/**
 * \brief Function used for time slave configuration in the case of a RX wait (wait timeout) for
 * an OFS message.
 * \param[in]  SlavePduId      - Slave Time Domain Id.
 * \param[in]  DataPtr         - Pointer containing the data needed for message
 *                               CRC computation.
 *
 * \return E_OK: Message Type and CRC accepted.
 *         E_NOT_OK: CRC not accepted for this SlaveDomainId
 *                   and RxStatus.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_OfnsMsg_Rx_CRC_Check
(
  uint8 SlavePduId,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr
);
#endif /* (CANTSYN_RX_OFFSET_USED == STD_ON) && (CANTSYN_RX_CRC_USED == STD_ON) */

#if(CANTSYN_RX_SYNC_USED == STD_ON)
/**
 * \brief Service to process the received SYNC message.
 * \param[in] T2VLT      - T2 virtual local time.
 *            DataPtr    - Pointer containing the received data.
 *            PduId       - Slave Pdu Id.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ProcessRxSyncFrame
(
  StbM_VirtualLocalTimeType T2VLT,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  uint8 PduId
);

/**
 * \brief Service to process the received FUP message.
 * \param[in] T5VLT       - T5 virtual local time.
 *            PduId       - Slave Pdu Id.
 *            DataPtr     - Pointer containing the received data.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ProcessRxFupFrame
(
  StbM_VirtualLocalTimeType T5VLT,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  uint8 PduId
);

/**
 * \brief Service to clear the sync state machine of the CanTSyn module.
 * \param[in] CanTSyn_SlavePduActiveIndex - Index of the active RxPduId.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_RxSyncReset
(
  uint8 CanTSyn_SlavePduActiveIndex
);
#endif /* (CANTSYN_RX_SYNC_USED == STD_ON) */


#if((CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON))
/**
 * \brief Service to clear the state machine of the CanTSyn module.
 * \param[in] SlavePduId - pduId
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_RxReset
(
  uint8 SlavePduId
);

/**
 * \brief Service to process the received frame
 * \param[in] SlavePduId        - Slave Time Domain Id.
 *            RxStatus          - Status of the current Slave Time Domain.
 *            DataPtr           - Pointer containing the data needed for message
 *                                type and CRC computation.
 *            T_VLT             - Virtual local time
 *            messageLength     - rx frame length
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ProcessRxFrame
(
  uint8 SlavePduId,
  uint8 RxStatus,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  StbM_VirtualLocalTimeType T_VLT,
  PduLengthType messageLength
);
#endif /* ((CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON)) */



#if(CANTSYN_RX_CRC_USED == STD_ON)

/**
 * \brief Service to check the CRC of the received message (either normal or extended).
 * \param[in] DataID     - Data Id taken from configuration based on the
 *                         message type.
 *            DataPtr    - Pointer containing the data needed for message
 *                         CRC computation.
 *            CrcLength  - CRC Length 7 (Normal message) or 15 (Extended message)
 *
 * \return E_OK: Message Type and CRC accepted.
 *         E_NOT_OK: Message Type and CRC not accepted for this SlaveDomainId
 *                   and RxStatus.
 */
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_CheckCRC
(
  uint8 DataID,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  uint8 CrcLength
);
#endif /* (CANTSYN_RX_CRC_USED == STD_ON) */


#if(CANTSYN_RX_OFFSET_USED == STD_ON)
/**
 * \brief Service to clear the sync state machine of the CanTSyn module.
 * \param[in] CanTSyn_SlavePduActiveIndex - Index of the active RxPduId.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_RxOffsetReset
(
  uint8 CanTSyn_SlavePduActiveIndex
);

#if(CANTSYN_RX_USE_EXTENDED_MSG == STD_ON)
/**
 * \brief Service to process the extended length received OFS message.
 * \param[in] T_VLT       - Virtual local time.
 *            DataPtr     - Pointer containing the received data.
 *            PduId       - Slave PduId.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ProcessExtRxOfsFrame
(
  StbM_VirtualLocalTimeType T_VLT,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  uint8 PduId
);
#endif

/**
 * \brief Service to process the received OFS message.
 * \param[in] DataPtr     - Pointer containing the received data.
 *            PduId       - Slave PduId.
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ProcessRxOfsFrame
(
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  uint8 PduId
);

/**
 * \brief Service to process the received OFNS message.
 * \param[in] T_VLT       - Virtual local time.
 *            DataPtr     - Pointer containing the received data.
 *            PduId       - Slave Pdu Id.
 *
 */
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ProcessRxOfnsFrame
(
  StbM_VirtualLocalTimeType T_VLT,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  uint8 PduId
);
#endif /* (CANTSYN_RX_OFFSET_USED == STD_ON) */

#define CANTSYN_STOP_SEC_CODE
#include <CanTSyn_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define CANTSYN_START_SEC_VAR_INIT_8
#include <CanTSyn_MemMap.h>

/** \brief Variable to indicate that module was initialized. */
STATIC VAR(boolean, CANTSYN_VAR) CanTSyn_Initialized = FALSE;

#define CANTSYN_STOP_SEC_VAR_INIT_8
#include <CanTSyn_MemMap.h>

#if((CANTSYN_TX_SYNC_USED == STD_ON) || (CANTSYN_TX_OFFSET_USED == STD_ON))
#define CANTSYN_START_SEC_VAR_CLEARED_8
#include <CanTSyn_MemMap.h>
STATIC VAR(CanTSyn_TransmissionModeType, CANTSYN_VAR) CanTSyn_TxMode[CANTSYN_NUMBER_OF_CONTROLLERS];
#define CANTSYN_STOP_SEC_VAR_CLEARED_8
#include <CanTSyn_MemMap.h>
#endif /* (CANTSYN_TX_SYNC_USED == STD_ON) || (CANTSYN_TX_OFFSET_USED == STD_ON) */

#define CANTSYN_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanTSyn_MemMap.h>
/* !LINKSTO CanTSyn.dsn.PrecompileTimeConfig,1 */
#if(CANTSYN_TX_SYNC_USED == STD_ON)
/** \brief Sync Messages Master variable. */
STATIC VAR(CanTSyn_SyncMasterType, CANTSYN_VAR) CanTSyn_SyncMaster[CANTSYN_NUMBER_OF_SYNC_MASTERS];
#endif /* CANTSYN_TX_SYNC_USED == STD_ON */

#if(CANTSYN_TX_OFFSET_USED == STD_ON)
/** \brief Offset Messages Master variable. */
STATIC VAR(CanTSyn_OffsetMasterType, CANTSYN_VAR) CanTSyn_OffsetMaster[CANTSYN_NUMBER_OF_OFFSET_MASTERS];
#endif /* CANTSYN_TX_OFFSET_USED == STD_ON */

#if(CANTSYN_RX_SYNC_USED == STD_ON)
/** \brief Sync Messages Slave variable. */
STATIC VAR(CanTSyn_SyncSlaveType, CANTSYN_VAR) CanTSyn_SyncSlave[CANTSYN_NUMBER_OF_SYNC_SLAVES];
#endif /* CANTSYN_RX_SYNC_USED == STD_ON */

#if(CANTSYN_RX_OFFSET_USED == STD_ON)
/** \brief Sync Messages Slave variable. */
STATIC VAR(CanTSyn_OffsetSlaveType, CANTSYN_VAR) CanTSyn_OffsetSlave[CANTSYN_NUMBER_OF_OFFSET_SLAVES];
#endif /* CANTSYN_RX_OFFSET_USED == STD_ON */

#define CANTSYN_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanTSyn_MemMap.h>

/*==================[external function definitions]=========================*/
#define CANTSYN_START_SEC_CODE
#include <CanTSyn_MemMap.h>


#if(CANTSYN_VERSION_INFO_API == STD_ON)
/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00094,1 */
FUNC(void, CANTSYN_CODE) CanTSyn_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CANTSYN_APPL_DATA) versioninfo
)
{
  DBG_CANTSYN_GETVERSIONINFO_ENTRY(versioninfo);

#if(CANTSYN_DEV_ERROR_DETECT == STD_ON)
  /* check if the versioninfo pointer is a Null Pointer */
  /* !LINKSTO CanTSyn.EB.GetVersionInfo.NullPtr,1 */
  if(NULL_PTR == versioninfo)
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00088,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00089,1 */
    CANTSYN_DET_REPORT_ERROR(CANTSYN_SID_GETVERSIONINFO, CANTSYN_E_NULL_POINTER);
  }
  else
#endif /* CANTSYN_DEV_ERROR_DETECT == STD_ON */
  {
    /* Return version information */
    versioninfo->vendorID = CANTSYN_VENDOR_ID;
    versioninfo->moduleID = CANTSYN_MODULE_ID;
    versioninfo->sw_major_version = CANTSYN_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = CANTSYN_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = CANTSYN_SW_PATCH_VERSION;
  }

  DBG_CANTSYN_GETVERSIONINFO_EXIT(versioninfo);
}
#endif /* CANTSYN_VERSION_INFO_API */


/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00093,1 */
FUNC(void, CANTSYN_CODE) CanTSyn_Init
(
  P2CONST(CanTSyn_ConfigType, AUTOMATIC, CANTSYN_APPL_CONST) configPtr
)
{
  uint8 PduIdx;

  DBG_CANTSYN_INIT_ENTRY(configPtr);

#if(CANTSYN_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO CanTSyn.EB.Init.ConfigPtr,1 */
  /* current implementation supports only Precompile variant */
  if(NULL_PTR != configPtr)
  {
    CANTSYN_DET_REPORT_ERROR(CANTSYN_SID_INIT, CANTSYN_E_INIT_FAILED);
  }
  else
#else
  TS_PARAM_UNUSED(configPtr);
#endif
  {

#if((CANTSYN_TX_SYNC_USED == STD_ON) || (CANTSYN_TX_OFFSET_USED == STD_ON))
    {
      uint8 CtrlIdx;
      /* !LINKSTO CanTSyn.dsn.Mode.Initialization,1 */
      /* Accept all transmit requests on all channels */
      /* Deviation TASKING-1 */
      for(CtrlIdx = 0U; CtrlIdx < CANTSYN_NUMBER_OF_CONTROLLERS; CtrlIdx++)
      {
        CanTSyn_TxMode[CtrlIdx] = CANTSYN_TX_ON;
      }
    }
#endif /* (CANTSYN_TX_SYNC_USED == STD_ON) || (CANTSYN_TX_OFFSET_USED == STD_ON) */

#if(CANTSYN_TX_SYNC_USED == STD_ON)
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00003,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00006,1 */
    /* Deviation TASKING-1 */
    for(PduIdx = 0U; PduIdx < CANTSYN_NUMBER_OF_SYNC_MASTERS; PduIdx++)
    {
      P2VAR(CanTSyn_SyncMasterType, AUTOMATIC, CANTSYN_APPL_DATA)
            CanTSyn_MasterPtr = &CanTSyn_SyncMaster[PduIdx];

      /* initialize module variables */
      CanTSyn_TxSyncReset(PduIdx);

      /* Clear the Sequence Counter */ /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00007,1 */
      CanTSyn_MasterPtr->SeqCount = 0U;

      /* Clear the SGW Bit */
      CanTSyn_MasterPtr->SyncSGWBit = 0U;

      /* clear the Tx Period Time */
      CanTSyn_MasterPtr->TimeTxPeriod = 0U;

      /* initialize syncTimeBaseCounter */
      CanTSyn_MasterPtr->syncTimeBaseCounter = 0U;

#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON)
      /* initialize SYNC CyclicMsgResumeTime timer */
      CanTSyn_MasterPtr->CyclicMsgResumeTime = 0U;
#endif /* (CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON) */
    }
#endif /* CANTSYN_TX_SYNC_USED == STD_ON */

#if(CANTSYN_TX_OFFSET_USED == STD_ON)
    /* Deviation TASKING-1 */
    for(PduIdx = 0U; PduIdx < CANTSYN_NUMBER_OF_OFFSET_MASTERS; PduIdx++)
    {
      P2VAR(CanTSyn_OffsetMasterType, AUTOMATIC, CANTSYN_APPL_DATA)
        CanTSyn_OffsetMasterPtr = &CanTSyn_OffsetMaster[PduIdx];

      /* initialize module variables */
      CanTSyn_TxOffsetReset(PduIdx);

      /* Clear the Sequence Counter */ /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00007,1 */
      CanTSyn_OffsetMasterPtr->SeqCount = 0U;

      /* Clear the Offset SGW Bit */
      CanTSyn_OffsetMasterPtr->OfsSGWBit = 0U;

      /* clear the Tx Period Time */
      CanTSyn_OffsetMasterPtr->TimeTxPeriod = 0U;

      /* initialize syncTimeBaseCounter */
      CanTSyn_OffsetMasterPtr->OfsTimeBaseCounter = 0U;

#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON)
      /* initialize OFS CyclicMsgResumeTime timer */
      CanTSyn_OffsetMasterPtr->CyclicMsgResumeTime = 0U;
#endif /* (CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON) */
    }
#endif /* CANTSYN_TX_OFFSET_USED == STD_ON */

#if(CANTSYN_RX_SYNC_USED == STD_ON)
    for(PduIdx = 0U;
        /* Deviation TASKING-1 */
        PduIdx < CANTSYN_NUMBER_OF_SYNC_SLAVES;
        PduIdx++)
    {
      /* initialize module variables */
      CanTSyn_SyncSlave[PduIdx].SeqCount = CANTSYN_SC_STARTUP_TIMEOUT;

      /* initialize SYNC RxStatus and all the variables for a new SYNC reception */
      /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_Init,1 */
      CanTSyn_RxSyncReset(PduIdx);
    }
#endif /* CANTSYN_RX_SYNC_USED == STD_ON */

#if(CANTSYN_RX_OFFSET_USED == STD_ON)
    for(PduIdx = 0U;
        /* Deviation TASKING-1 */
        PduIdx < CANTSYN_NUMBER_OF_OFFSET_SLAVES;
        PduIdx++)
    {
      /* initialize module variables */
      CanTSyn_OffsetSlave[PduIdx].SeqCount = CANTSYN_SC_STARTUP_TIMEOUT;

      /* initialize OFS RxStatus and all the variables for a new OFS reception */
      /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_Init,1 */
      CanTSyn_RxOffsetReset(PduIdx);
    }
#endif /* (CANTSYN_RX_OFFSET_USED == STD_ON) */

    /* module initialized */
    CanTSyn_Initialized = TRUE;
  }

  DBG_CANTSYN_INIT_EXIT(configPtr);
}

#if((CANTSYN_TX_SYNC_USED == STD_ON) || (CANTSYN_TX_OFFSET_USED == STD_ON))
/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00095,1 */
FUNC(void, CANTSYN_CODE) CanTSyn_SetTransmissionMode
(
  uint8 CtrlIdx,
  CanTSyn_TransmissionModeType Mode
)
{
  DBG_CANTSYN_SETTRANSMISSIONMODE_ENTRY(CtrlIdx, Mode);

#if(CANTSYN_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO CanTSyn.EB.SetTransmissionMode.Uninit,1 */
  if(FALSE == CanTSyn_Initialized)
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00088,1 */ /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00089,1 */
    CANTSYN_DET_REPORT_ERROR(CANTSYN_SID_SETTRANSMISSIONMODE, CANTSYN_E_UNINIT);
  }
  else if((CANTSYN_TX_ON != Mode) && (CANTSYN_TX_OFF != Mode))
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00134,1 */ /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00089,1 */
    CANTSYN_DET_REPORT_ERROR(CANTSYN_SID_SETTRANSMISSIONMODE, CANTSYN_E_PARAM);
  }
  else
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
  {
    uint8 PduIndex;
#if(CANTSYN_DEV_ERROR_DETECT == STD_ON)
    boolean ControllerFound = FALSE;
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
    for(PduIndex = 0U;
        /* Deviation TASKING-1 */
        PduIndex < (CANTSYN_NUMBER_OF_SYNC_MASTERS + CANTSYN_NUMBER_OF_OFFSET_MASTERS);
        PduIndex++)
    {
      if(CtrlIdx == CanTSyn_TimeMasterConfig[PduIndex].ControllerId)
      {
        /* set the Time Master mode to the requested one */
        CanTSyn_TxMode[CanTSyn_TimeMasterConfig[PduIndex].CtrlIndex] = Mode;
#if(CANTSYN_DEV_ERROR_DETECT == STD_ON)
        ControllerFound = TRUE;
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
        break;
      }
    }
#if(CANTSYN_DEV_ERROR_DETECT == STD_ON)
    if(FALSE == ControllerFound)
    {
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00134,1 */ /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00089,1 */
      CANTSYN_DET_REPORT_ERROR(CANTSYN_SID_SETTRANSMISSIONMODE, CANTSYN_E_INV_CTRL_IDX);
    }
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
  }

  DBG_CANTSYN_SETTRANSMISSIONMODE_EXIT(CtrlIdx, Mode);
}
#endif /* (CANTSYN_TX_SYNC_USED == STD_ON) || (CANTSYN_TX_OFFSET_USED == STD_ON) */


#if ( (CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON))
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_RxIndication_Validation
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, CANTSYN_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType RetVal = E_OK ;
#if(CANTSYN_DEV_ERROR_DETECT == STD_ON)
  /* check if the module was initialized */
  if(FALSE == CanTSyn_Initialized)
  {
    /* !LINKSTO CanTSyn.EB.RxIndication.Uninit,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00088,1 */ /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00089,1 */
    CANTSYN_DET_REPORT_ERROR(CANTSYN_SID_RXINDICATION, CANTSYN_E_UNINIT);
    RetVal = E_NOT_OK ;
  }
  else if(NULL_PTR == PduInfoPtr)
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00097.PduInfoPtr.E_NULL_POINTER.RxIndication,1 */
    CANTSYN_DET_REPORT_ERROR(CANTSYN_SID_RXINDICATION, CANTSYN_E_NULL_POINTER);
    RetVal = E_NOT_OK ;
  }
  else if(NULL_PTR == PduInfoPtr->SduDataPtr)
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00097.SduDataPtr.E_NULL_POINTER.RxIndication,1 */
    CANTSYN_DET_REPORT_ERROR(CANTSYN_SID_RXINDICATION, CANTSYN_E_NULL_POINTER);
    RetVal = E_NOT_OK ;
  }
  /* check if received PduId exists */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00086,1 */
  else if( ( (uint8)CANTSYN_NUMBER_OF_SLAVES - (uint8)1U ) < (uint8)RxPduId )
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00097.INVALID_PDUID.RxIndication,1 */
    CANTSYN_DET_REPORT_ERROR(CANTSYN_SID_RXINDICATION, CANTSYN_E_INVALID_PDUID);
    RetVal = E_NOT_OK ;
  }
  else
#endif /* CANTSYN_DEV_ERROR_DETECT == STD_ON */
  {
  /* check the received Pdu Length */
#if(CANTSYN_RX_USE_EXTENDED_MSG == STD_ON)
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00010.DLC.ExtendedMessage,1 */
    if( (CanTSyn_TimeSlaveConfig[RxPduId].IsExtendedMessage == 1U ) && ( (uint8)(PduInfoPtr->SduLength) != CANTSYN_EXTENDED_MSG_SIZE)  )
    {
      /* !LINKSTO CanTSyn.EB.DLC.ExtendedMessage.INVALID_PDU_LENGTH.RxIndication,1 */
      RetVal = E_NOT_OK ;
      /* reset RxStatus due to wrong length */
      CanTSyn_RxReset((uint8)RxPduId);
    }
    else if( ( CanTSyn_TimeSlaveConfig[RxPduId].IsExtendedMessage == 0U ) && ( (uint8)(PduInfoPtr->SduLength) != CANTSYN_MSG_SIZE)  )
    {
      /* !LINKSTO CanTSyn.EB.DLC.NormalMessage.INVALID_PDU_LENGTH.RxIndication,1 */
      RetVal = E_NOT_OK ;
      /* reset RxStatus due to wrong length */
      CanTSyn_RxReset((uint8)RxPduId);
    }
#else /* CANTSYN_RX_USE_EXTENDED_MSG == STD_ON */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00010.DLC.NormalMessage,1 */
    if(  (uint8)(PduInfoPtr->SduLength) != CANTSYN_MSG_SIZE  )
    {
      /* !LINKSTO CanTSyn.EB.DLC.NormalMessage.INVALID_PDU_LENGTH.RxIndication,1 */
      RetVal = E_NOT_OK ;
      /* reset RxStatus due to wrong length */
      CanTSyn_RxReset((uint8)RxPduId);
    }
#endif /* CANTSYN_RX_USE_EXTENDED_MSG == STD_ON */
    /* check if received PduId exists */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00086,1 */
    else if( (CANTSYN_GET_DOMAINID_SLAVE(PduInfoPtr->SduDataPtr[2U]) != CanTSyn_TimeSlaveConfig[RxPduId].TimeDomainId) )
    {
      RetVal = E_NOT_OK ;
      /* reset RxStatus due to wrong time domain id */
      CanTSyn_RxReset((uint8)RxPduId);
    }
    else
    {
    }
  }
  return RetVal;
}
#endif /* (CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON) */


/* !LINKSTO CanTSyn.EB.CanTSyn_RxIndication_PduInfoPtr,1 */
FUNC(void, CANTSYN_CODE) CanTSyn_RxIndication
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, CANTSYN_APPL_DATA) PduInfoPtr
)
{
#if((CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON))
  DBG_CANTSYN_RXINDICATION_ENTRY(RxPduId, PduInfoPtr);

  if( CanTSyn_RxIndication_Validation(RxPduId, PduInfoPtr) == E_OK)
  {
    const uint8 rxPduId = (uint8)RxPduId;
    uint8 RxStatus;
    P2CONST(CanTSyn_TimeSlaveDomainConfigType, AUTOMATIC, CANTSYN_APPL_CONST) SlaveConfigPtr = &CanTSyn_TimeSlaveConfig[rxPduId];
    P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) sduDataPtr = PduInfoPtr->SduDataPtr;
    boolean ProcessFrame = FALSE;
    boolean CrcValidation = FALSE;
    StbM_VirtualLocalTimeType T_VLT = {0U, 0U};
    StbM_SynchronizedTimeBaseType TimeBaseId;

    /* get the status */
    CanTSyn_GetRxStatus(rxPduId, &RxStatus);

    TimeBaseId = SlaveConfigPtr->SyncTimeBaseId;

    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00073,1 */
    SchM_Enter_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_1();

    /* The CrcValidation will be TRUE if the time domain is configured to CRC_VALIDATION
       or CRC_OPTIONAL with message type indicating that CRC validation should be performed. */
    if(E_OK == CanTSyn_CheckMsgType(rxPduId, RxStatus, sduDataPtr, &CrcValidation))
    {
      /* !LINKSTO CanTSyn.EB.Rx_StbM_GetCurrentVirtualLocalTime_E_NOT_OK,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00135,1 */
      if(E_OK == StbM_GetCurrentVirtualLocalTime(TimeBaseId, &T_VLT))
      {
        ProcessFrame = TRUE;
      }
    }

    SchM_Exit_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_1();

    if(ProcessFrame)
    {
#if(CANTSYN_RX_CRC_USED == STD_ON)
    /* check if the received type shall be accepted */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00086,1 */
      Std_ReturnType CrcRetVal;
      if(TRUE == CrcValidation)
      {
        CrcRetVal = CanTSyn_CheckMsgCRC(rxPduId, RxStatus, sduDataPtr);
      }
      else
      {
        CrcRetVal = E_OK;
      }

      if(E_OK == CrcRetVal)
#endif /* CANTSYN_RX_CRC_USED == STD_ON */
      {
        /* process Rx frame */
        CanTSyn_ProcessRxFrame(rxPduId, RxStatus, sduDataPtr, T_VLT, PduInfoPtr->SduLength);
      }
#if(CANTSYN_RX_CRC_USED == STD_ON)
      else
      {
        /* reset RxStatus due to wrong CRC */
        CanTSyn_RxReset(rxPduId);
      }
#endif /* CANTSYN_RX_CRC_USED == STD_ON */
    }
    else
    {
      /* reset RxStatus due to wrong type */
      CanTSyn_RxReset(rxPduId);
    }
  }

  DBG_CANTSYN_RXINDICATION_EXIT(RxPduId, PduInfoPtr);
#else
  /* No support for Time Slave */
  TS_PARAM_UNUSED(RxPduId);
  TS_PARAM_UNUSED(PduInfoPtr);
#endif /* (CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON) */
}


/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00099,1 */
FUNC(void, CANTSYN_CODE) CanTSyn_TxConfirmation
(
  PduIdType TxPduId
)
{
#if((CANTSYN_TX_SYNC_USED == STD_ON) || (CANTSYN_TX_OFFSET_USED == STD_ON))
  DBG_CANTSYN_TXCONFIRMATION_ENTRY(TxPduId);

#if(CANTSYN_DEV_ERROR_DETECT == STD_ON)
  /* check if the module was initialized */
  if(FALSE == CanTSyn_Initialized)
  {
    /* !LINKSTO CanTSyn.EB.TxConfirmation.Uninit,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00088,1 */ /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00089,1 */
    CANTSYN_DET_REPORT_ERROR(CANTSYN_SID_TXCONFIRMATION, CANTSYN_E_UNINIT);
  }
  /* check if the function was called with valid PduId */
  else
#endif /* CANTSYN_DEV_ERROR_DETECT == STD_ON */
  {
    /* check PduId validity */
    if((CANTSYN_NUMBER_OF_MASTERS - 1U) < TxPduId)
    {
#if(CANTSYN_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00100,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00088,1 */ /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00089,1 */
      CANTSYN_DET_REPORT_ERROR(CANTSYN_SID_TXCONFIRMATION, CANTSYN_E_INVALID_PDUID);
#endif /* CANTSYN_DEV_ERROR_DETECT == STD_ON */
    }
    else
    {
#if(CANTSYN_TX_OFFSET_USED == STD_ON)
      /* check if the Confirmation is for a SYNC/FUP or OFS/OFNS received */
      if(TxPduId >= CANTSYN_NUMBER_OF_SYNC_MASTERS)
      {
        CanTSyn_ConfirmOfsMsg((uint8)TxPduId - CANTSYN_NUMBER_OF_SYNC_MASTERS);
      }
      else
#endif /* (CANTSYN_TX_OFFSET_USED == STD_ON) */
      {
        CanTSyn_ConfirmSyncMsg((uint8)TxPduId);
      }
    }
  }

  DBG_CANTSYN_TXCONFIRMATION_EXIT(TxPduId);
#else

  /* No support for Time Master */
  TS_PARAM_UNUSED(TxPduId);

#endif /* (CANTSYN_TX_SYNC_USED == STD_ON) || (CANTSYN_TX_OFFSET_USED == STD_ON) */
}


/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00102,1 */
FUNC(void, CANTSYN_CODE) CanTSyn_MainFunction(void)
{
#if(CANTSYN_IS_PDUINDEX_USED == STD_ON)
  uint8 PduIndex;
#endif /* (CANTSYN_IS_PDUINDEX_USED == STD_ON) */

  DBG_CANTSYN_MAINFUNCTION_ENTRY();

  /* check if the module was initialized */
  if(TRUE == CanTSyn_Initialized)
  {
#if(CANTSYN_TX_SYNC_USED == STD_ON)
      for(PduIndex = 0U;
          /* Deviation TASKING-1 */
          PduIndex < CANTSYN_NUMBER_OF_SYNC_MASTERS;
          PduIndex++)
      {
        /* Send frame only if TxMode is TX_ON */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00044,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00043,1 */
        if(CANTSYN_TX_ON == CanTSyn_TxMode[CanTSyn_TimeMasterConfig[PduIndex].CtrlIndex])
        {
          /* SYNC Pdu */
          CanTSyn_TxProcessSyncMsg(PduIndex);
        }
      }
#endif /* CANTSYN_TX_SYNC_USED == STD_ON */

#if(CANTSYN_TX_OFFSET_USED == STD_ON)
      for(PduIndex = 0U;
          /* Deviation TASKING-1 */
          PduIndex < CANTSYN_NUMBER_OF_OFFSET_MASTERS;
          PduIndex++)
      {
        /* Send frame only if TxMode is TX_ON */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00044,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00043,1 */
        if(CANTSYN_TX_ON == CanTSyn_TxMode[CanTSyn_TimeMasterConfig[PduIndex + CANTSYN_NUMBER_OF_SYNC_MASTERS].CtrlIndex])
        {
          /* OFS Pdu */
          CanTSyn_TxProcessOfsMsg(PduIndex);
        }
      }
#endif /* CANTSYN_TX_OFFSET_USED == STD_ON */


#if((CANTSYN_RX_SYNC_USED == STD_ON) && (CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON))
    for(PduIndex = 0U;
        /* Deviation TASKING-1 */
        PduIndex < CANTSYN_NUMBER_OF_SYNC_SLAVES;
        PduIndex++)
    {
      P2VAR(CanTSyn_SyncSlaveType, AUTOMATIC, CANTSYN_APPL_DATA)
              CanTSyn_SyncSlavePtr = &CanTSyn_SyncSlave[PduIndex];

      switch(CanTSyn_SyncSlavePtr->RxStatus)
      {
        case CANTSYN_FUPMSG_RX_WAIT:
        {
          /* Protection needed for RxStatus, because a reception of a FUP message can interrupt
             CanTSyn_MainFunction and RxStatus will be set to CANTSYN_SYNCMSG_RX_WAIT
             and after that it will be set here to CANTSYN_FUPMSG_RX_WAIT_TIMEOUT
             which causes a new reception of a SYNC message to be ignored.
          */
          SchM_Enter_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();

          if(CanTSyn_TimeSlaveConfig[PduIndex].RxFollowUpOffsetTimeout > 0U)
          {
            /* Load the Follow Up Offset Timeout */
            CanTSyn_SyncSlavePtr->TimeRxFollowUpOffset = CanTSyn_TimeSlaveConfig[PduIndex].RxFollowUpOffsetTimeout;

            /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_SyncRXWAIT-FupRXWAITTIMEOUT,1 */
            /* change RxStatus, FollowUpOffsetTimeout loaded */
            CanTSyn_SyncSlavePtr->RxStatus = CANTSYN_FUPMSG_RX_WAIT_TIMEOUT;

          }
          else /* RxFollowUpOffsetTimeout is configured to a value smaller or equal to 0 */
          {
            /* if CanTSynGlobalTimeFollowUpTimeout is zero,
             * CanTSyn waits in CANTSYN_FUPMSG_RX_WAIT state until the awaited FUP arrives
             * => nothing to be done
             */
            /* !LINKSTO CanTSyn.EB.Zero.CanTSynGlobalTimeFollowUpTimeout.Awaited.FUP,1 */
            /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAITTIMEOUT_SyncRXWAIT_NoFupReceived,1 */
          }

          SchM_Exit_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();
        }
        break;

        case CANTSYN_FUPMSG_RX_WAIT_TIMEOUT:
        {
          /* Protection needed because a new reception can reset the TimeRxFollowUpOffset
             before decrementation and when TimeRxFollowUpOffset will be decremented
             it will cause an undefined behaviour */
          SchM_Enter_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();

          if(CanTSyn_SyncSlavePtr->TimeRxFollowUpOffset == 0U)
          {
            /* reset due to timeout */
            /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00063,1 */
            /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAITTIMEOUT_SyncRXWAIT,1 */
            CanTSyn_RxSyncReset(PduIndex);
          }
          else
          {
            /* decrease the time to wait for FUP message */
            CanTSyn_SyncSlavePtr->TimeRxFollowUpOffset--;
          }

          SchM_Exit_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();
        }
        break;

        default:
          /* nothing to be done */
        break;
      }
    }
#else
    /* if all CanTSyn Time Domains have CanTSynGlobalTimeFollowUpTimeout set to zero,
     * RxFollowUpTimeout shall not be handled and CanTSyn shall wait in
     * CANTSYN_FUPMSG_RX_WAIT state, until the awaited FUP arrives
     */
    /* !LINKSTO CanTSyn.EB.Zero.CanTSynGlobalTimeFollowUpTimeout.Awaited.FUP,1 */
#endif /* ((CANTSYN_RX_SYNC_USED == STD_ON) && (CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON)) */


#if((CANTSYN_RX_OFFSET_USED == STD_ON) && (CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON))
    for(PduIndex = 0U;
        /* Deviation TASKING-1 */
        PduIndex < CANTSYN_NUMBER_OF_OFFSET_SLAVES;
        PduIndex++)
    {
      P2VAR(CanTSyn_OffsetSlaveType, AUTOMATIC, CANTSYN_APPL_DATA)
              CanTSyn_OffsetSlavePtr = &CanTSyn_OffsetSlave[PduIndex];

      switch(CanTSyn_OffsetSlavePtr->RxStatus)
      {
        case CANTSYN_OFNSMSG_RX_WAIT:
        {
          /* Protection needed, because a reception of a OFNS message can interrupt
             CanTSyn_MainFunction and RxStatus will be set to CANTSYN_OFSMSG_RX_WAIT
             and after that it will be set here to CANTSYN_OFNSMSG_RX_WAIT_TIMEOUT
             which causes a new reception of a OFS message to be ignored */
          SchM_Enter_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();

          if(CanTSyn_TimeSlaveConfig[PduIndex + CANTSYN_NUMBER_OF_SYNC_SLAVES].RxFollowUpOffsetTimeout > 0U)
          {
            /* Load the Follow Up Offset Timeout */
            CanTSyn_OffsetSlavePtr->TimeRxFollowUpOffset = CanTSyn_TimeSlaveConfig[PduIndex + CANTSYN_NUMBER_OF_SYNC_SLAVES].RxFollowUpOffsetTimeout;

            /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfsRXWAIT-OfnsRXWAITTIMEOUT,1 */
            /* change RxStatus, FollowUpOffsetTimeout loaded */
            CanTSyn_OffsetSlavePtr->RxStatus = CANTSYN_OFNSMSG_RX_WAIT_TIMEOUT;
          }
          else
          {
            /* if CanTSynGlobalTimeFollowUpTimeout is zero,
             * CanTSyn waits in CANTSYN_OFNSMSG_RX_WAIT state until the awaited OFNS arrives
             * => nothing to be done
             */
            /* !LINKSTO CanTSyn.EB.Zero.CanTSynGlobalTimeFollowUpTimeout.Awaited.OFNS,1 */
            /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAITTIMEOUT_OfsRXWAIT_NoOfnsReceived,1 */
          }

          SchM_Exit_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();
        }
        break;

        case CANTSYN_OFNSMSG_RX_WAIT_TIMEOUT:
        {
          /* Protection needed because a new reception can reset the TimeRxFollowUpOffset
             before decrementation and when TimeRxFollowUpOffset will be decremented
             it will cause an undefined behaviour */
          SchM_Enter_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();

          if(CanTSyn_OffsetSlavePtr->TimeRxFollowUpOffset == 0U)
          {
            SchM_Enter_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();

            /* reset due to timeout */
            /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00071,1 */
            /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAITTIMEOUT_OfsRXWAIT,1 */
            CanTSyn_RxOffsetReset(PduIndex);

            SchM_Exit_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();
          }
          else
          {
            /* decrease the time to wait for OFNS message */
            CanTSyn_OffsetSlavePtr->TimeRxFollowUpOffset--;
          }

          SchM_Exit_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();
        }
        break;

        default:
          /* nothing to be done */
        break;
      }
    }
#else
    /* if all CanTSyn Time Domains have CanTSynGlobalTimeFollowUpTimeout set to zero,
     * RxFollowUpTimeout shall not be handled and CanTSyn shall wait in
     * CANTSYN_OFNSMSG_RX_WAIT state, until the awaited OFNS arrives
     */
    /* !LINKSTO CanTSyn.EB.Zero.CanTSynGlobalTimeFollowUpTimeout.Awaited.OFNS,1 */
#endif /* ((CANTSYN_RX_OFFSET_USED == STD_ON) && (CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON)) */
  }

  DBG_CANTSYN_MAINFUNCTION_EXIT();

}


#if(CANTSYN_TX_SYNC_USED == STD_ON)
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_FillTxSyncFrame
(
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_DATA) TxDataPtr,
  uint8 CanTSynPduIdx
)
{
  uint32 T0Seconds = CanTSyn_SyncMaster[CanTSynPduIdx].SyncTimeT0.seconds;
  uint8 seqCounter = CanTSyn_SyncMaster[CanTSynPduIdx].SeqCount;
  P2CONST(CanTSyn_TimeMasterDomainConfigType, AUTOMATIC, AUTOMATIC) MasterConfigPtr = &CanTSyn_TimeMasterConfig[CanTSynPduIdx];

#if(CANTSYN_TX_CRC_USED == STD_ON)
  CanTSyn_DataIDListType DataId;
#endif /* (CANTSYN_TX_CRC_USED == STD_ON) */

  DBG_CANTSYN_FILLTXSYNCFRAME_ENTRY(TxDataPtr, CanTSynPduIdx);

  TxDataPtr[CANTSYN_B2_MSG_DOMAIN] = CANTSYN_GET_B2_DOMAIN_SC(MasterConfigPtr->TimeDomainId, seqCounter);
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00012,1 */ /* User Byte 0 */
  TxDataPtr[CANTSYN_B3_MSG_RESERVED] = CanTSyn_SyncMaster[CanTSynPduIdx].UserByte0;

  /* assemble Bytes 4-7 in Big Endian format */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00008,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00015.NormalMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00017.NormalMessage,1 */
  TxDataPtr[CANTSYN_B4_MSG] = ((uint8)(T0Seconds >> CANTSYN_GET_B3));
  TxDataPtr[CANTSYN_B5_MSG] = ((uint8)(T0Seconds >> CANTSYN_GET_B2));
  TxDataPtr[CANTSYN_B6_MSG] = ((uint8)(T0Seconds >> CANTSYN_GET_B1));
  TxDataPtr[CANTSYN_B7_MSG] = (uint8)T0Seconds;

#if (CANTSYN_TX_USE_EXTENDED_MSG == STD_ON)
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00015.ExtendedMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00017.ExtendedMessage,1 */
  if(MasterConfigPtr->IsExtendedMessage)
  {
    TS_MemBZero(&TxDataPtr[CANTSYN_B8_RESERVED], 8U);
  }
#endif

#if(CANTSYN_TX_CRC_USED == STD_ON)
  /* check if CRC supported */
  if(MasterConfigPtr->IsTxCrcSecuredUsed == CANTSYN_CRC_SUPPORTED)
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00017.NormalMessage,1 */ /* Type with CRC support */
    TxDataPtr[CANTSYN_B0_MSG_TYPE] = CANTSYN_SYNC_WITH_CRC;

    /* add the DataID Value based on the current sequence counter */ /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00054,1 */
    DataId = MasterConfigPtr->SyncOfsDataIdListPtr[seqCounter];

#if (CANTSYN_TX_USE_EXTENDED_MSG == STD_ON)
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00017.ExtendedMessage,1 */
    if(MasterConfigPtr->IsExtendedMessage)
    {
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00085.CRC.ExtendedMessage,1 */
      TxDataPtr[CANTSYN_B1_MSG] = CanTSyn_CalculateCRC(&TxDataPtr[CANTSYN_B2_MSG_DOMAIN], DataId, CANTSYN_CRC_EXT_LENGTH);
    }
    else
#endif
    {
      /* get the CRC value */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00050,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00085.CRC.NormalMessage,1 */
      TxDataPtr[CANTSYN_B1_MSG] = CanTSyn_CalculateCRC(&TxDataPtr[CANTSYN_B2_MSG_DOMAIN], DataId, CANTSYN_CRC_LENGTH);
    }

  }
  else
#endif /* CANTSYN_TX_CRC_USED == STD_ON */
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00015.NormalMessage,1 */
    TxDataPtr[CANTSYN_B0_MSG_TYPE] = CANTSYN_SYNC_WITHOUT_CRC; /* Type */ /* No CRC support */
    TxDataPtr[CANTSYN_B1_MSG] = CanTSyn_SyncMaster[CanTSynPduIdx].UserByte1; /* User Byte 1 */
  }

  DBG_CANTSYN_FILLTXSYNCFRAME_EXIT(TxDataPtr, CanTSynPduIdx);
}


STATIC FUNC(void, CANTSYN_CODE) CanTSyn_FillTxFupFrame
(
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_DATA) TxDataPtr,
  uint8 CanTSynPduIdx,
  uint8 CanTSynOvsBit
)
{
  uint32 T0Nanoseconds = CanTSyn_SyncMaster[CanTSynPduIdx].FupTimeT4.nanoseconds;
  uint8 FupSeqCount = CANTSYN_SC_MAXLIMIT;

#if(CANTSYN_TX_CRC_USED == STD_ON)
  CanTSyn_DataIDListType DataId;
#endif /* (CANTSYN_TX_CRC_USED == STD_ON) */

  DBG_CANTSYN_FILLTXFUPFRAME_ENTRY(TxDataPtr, CanTSynPduIdx, CanTSynOvsBit);

  /* Sequence Counter increased at SYNC */
  /* Use the same */
  FupSeqCount = CANTSYN_GET_SEQ_COUNTER(CanTSyn_SyncMaster[CanTSynPduIdx].SeqCount);

  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00049_OFNS_SCUnchanged,1 */
  TxDataPtr[CANTSYN_B2_MSG_DOMAIN] = CANTSYN_GET_B2_DOMAIN_SC(CanTSyn_TimeMasterConfig[CanTSynPduIdx].TimeDomainId, FupSeqCount);
  TxDataPtr[CANTSYN_B3_MSG_GATEWAY] = CANTSYN_B3_SGW_OVS(CanTSynPduIdx, CanTSynOvsBit);

  /* assemble Bytes 4-7 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00008,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00016.NormalMessage,1 */
  TxDataPtr[CANTSYN_B4_MSG] = (((uint8)(T0Nanoseconds >> CANTSYN_GET_B3)));
  TxDataPtr[CANTSYN_B5_MSG] = ((uint8)(T0Nanoseconds >> CANTSYN_GET_B2));
  TxDataPtr[CANTSYN_B6_MSG] = ((uint8)(T0Nanoseconds >> CANTSYN_GET_B1));
  TxDataPtr[CANTSYN_B7_MSG] = (uint8)T0Nanoseconds;

#if (CANTSYN_TX_USE_EXTENDED_MSG == STD_ON)
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00016.ExtendedMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00018.ExtendedMessage,1 */
  if(CanTSyn_TimeMasterConfig[CanTSynPduIdx].IsExtendedMessage)
  {
    TS_MemBZero(&TxDataPtr[CANTSYN_B8_RESERVED], 8U);
  }
#endif

#if(CANTSYN_TX_CRC_USED == STD_ON)
  /* check if CRC supported */
  if(CanTSyn_TimeMasterConfig[CanTSynPduIdx].IsTxCrcSecuredUsed == CANTSYN_CRC_SUPPORTED)
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00018.NormalMessage,1 */ /* Type with CRC support */
    TxDataPtr[CANTSYN_B0_MSG_TYPE] = CANTSYN_FUP_WITH_CRC;

    /* get DataId value based on the current sequence counter */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00054,1 */
    DataId = CanTSyn_TimeMasterConfig[CanTSynPduIdx].FupOfnsDataIdListPtr[FupSeqCount];

#if (CANTSYN_TX_USE_EXTENDED_MSG == STD_ON)
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00017.ExtendedMessage,1 */
    if(CanTSyn_TimeMasterConfig[CanTSynPduIdx].IsExtendedMessage)
    {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00085.CRC.ExtendedMessage,1 */
      TxDataPtr[CANTSYN_B1_MSG] = CanTSyn_CalculateCRC(&TxDataPtr[CANTSYN_B2_MSG_DOMAIN], DataId, CANTSYN_CRC_EXT_LENGTH);
    }
    else
#endif
    {
      /* get the CRC value */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00050,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00085.CRC.NormalMessage,1 */
      TxDataPtr[CANTSYN_B1_MSG] = CanTSyn_CalculateCRC(&TxDataPtr[CANTSYN_B2_MSG_DOMAIN], DataId, CANTSYN_CRC_LENGTH);
    }

  }
  else
#endif /* CANTSYN_TX_CRC_USED == STD_ON */
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00016.NormalMessage,1 */
    TxDataPtr[CANTSYN_B0_MSG_TYPE] = CANTSYN_FUP_WITHOUT_CRC; /* Type */ /* No CRC support */
    TxDataPtr[CANTSYN_B1_MSG] = CanTSyn_SyncMaster[CanTSynPduIdx].UserByte2; /* User Byte 2 */
  }

  DBG_CANTSYN_FILLTXFUPFRAME_EXIT(TxDataPtr, CanTSynPduIdx, CanTSynOvsBit);
}


STATIC FUNC(void, CANTSYN_CODE) CanTSyn_TxSyncReset
(
  uint8 ConfirmationHandleId
)
{
  P2VAR(CanTSyn_SyncMasterType, AUTOMATIC, CANTSYN_APPL_DATA)
          CanTSyn_MasterPtr = &CanTSyn_SyncMaster[ConfirmationHandleId];

  DBG_CANTSYN_TXSYNCRESET_ENTRY(ConfirmationHandleId);

  /* Protection needed because CanTSyn_TxSyncReset can be interrupted
     by CanTSyn_TxConfirmation where SyncTimeT0 will be read.
     If protection is not used, garbage data will be read for SyncTimeT0 variable
     Protection needed because CanTSyn_TxSyncReset can be interrupted
     by CanTSyn_TxConfirmation which also writes FupTimeT4 variable */
  SchM_Enter_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();

  /* Clear the Sync Time seconds portion */
  CanTSyn_MasterPtr->SyncTimeT0.seconds = 0U;

  /* Clear the Sync Time nanoseconds portion */
  CanTSyn_MasterPtr->SyncTimeT0.nanoseconds = 0U;

  /* Clear TxConfirmation Timeout */
  CanTSyn_MasterPtr->ConfirmationTimeout = 0U;

  /* Clear seconds portion of T4 needed for FUP message */
  CanTSyn_MasterPtr->FupTimeT4.seconds = 0U;

  /* Clear nanoseconds portion of T4 needed for FUP message */
  CanTSyn_MasterPtr->FupTimeT4.nanoseconds = 0U;

  /* Clear the debounceCounter */
  CanTSyn_MasterPtr->DebounceTime = 0U;

  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynTxStatus_Init,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00025,1 */
  /* each time synchronization Sync-Fup sequence starts with a SYNC message */
  CanTSyn_MasterPtr->TxStatus = CANTSYN_SYNCMSG_SEND_WAIT;

  /* clear User Data */
  CanTSyn_MasterPtr->UserByte0 = 0U;
  CanTSyn_MasterPtr->UserByte1 = 0U;
  CanTSyn_MasterPtr->UserByte2 = 0U;

  SchM_Exit_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();

  DBG_CANTSYN_TXSYNCRESET_EXIT(ConfirmationHandleId);
}
#endif /* CANTSYN_TX_SYNC_USED == STD_ON */


#if(CANTSYN_TX_OFFSET_USED == STD_ON)
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_TxOffsetReset
(
  uint8 ConfirmationHandleId
)
{
  P2VAR(CanTSyn_OffsetMasterType, AUTOMATIC, CANTSYN_APPL_DATA)
         CanTSyn_OffsetMasterPtr = &CanTSyn_OffsetMaster[ConfirmationHandleId];

  DBG_CANTSYN_TXOFFSETRESET_ENTRY(ConfirmationHandleId);

  /* Clear TxConfirmation Timeout */
  CanTSyn_OffsetMasterPtr->ConfirmationTimeout = 0U;

  SchM_Enter_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();

  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynTxStatus_Init,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00035,1 */
  /* each time synchronization Ofs-Ofns sequence starts with an OFS message */
  CanTSyn_OffsetMasterPtr->TxStatus = CANTSYN_OFSMSG_SEND_WAIT;

  /* Clear the debounceCounter */
  CanTSyn_OffsetMasterPtr->DebounceTime = 0U;

  SchM_Exit_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();

  DBG_CANTSYN_TXOFFSETRESET_EXIT(ConfirmationHandleId);
}


STATIC FUNC(void, CANTSYN_CODE) CanTSyn_FillTxOfsFrame
(
  P2CONST(StbM_TimeStampType, AUTOMATIC, CANTSYN_APPL_CONST) CanTSynTimeStampPtr,
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_DATA) CanTSynTxDataPtr,
  uint8 CanTSynPduIdx
)
{
  uint8 seqCounter = CanTSyn_OffsetMaster[CanTSynPduIdx].SeqCount;
  uint32 seconds = CanTSynTimeStampPtr->seconds;
  P2CONST(CanTSyn_TimeMasterDomainConfigType, AUTOMATIC, CANTSYN_APPL_CONST) CanTSyn_TimeMasterDomainPtr = &CanTSyn_TimeMasterConfig[CanTSynPduIdx+CANTSYN_NUMBER_OF_SYNC_MASTERS];

#if(CANTSYN_TX_CRC_USED == STD_ON)
  CanTSyn_DataIDListType DataId;
#endif /* (CANTSYN_TX_CRC_USED == STD_ON) */

  DBG_CANTSYN_FILLTXOFSFRAME_ENTRY(CanTSynTimeStampPtr, CanTSynTxDataPtr, CanTSynPduIdx);

#if (CANTSYN_TX_USE_EXTENDED_MSG == STD_ON)
  if(CanTSyn_TimeMasterDomainPtr->IsExtendedMessage)
  {
    uint32 nanoseconds = CanTSyn_OffsetMaster[CanTSynPduIdx].OfsTimeNSec;

    CanTSynTxDataPtr[CANTSYN_B2_MSG_DOMAIN] = CANTSYN_GET_B2_DOMAIN_SC(CanTSyn_TimeMasterDomainPtr->TimeDomainId, seqCounter);
    CanTSynTxDataPtr[CANTSYN_B3_MSG] = CANTSYN_B3_SGW(CanTSynPduIdx); /* reserved (Bit 7 to Bit 1), default: 0
                                                          * SGW (Bit 0)
                                                          */

    CanTSynTxDataPtr[CANTSYN_B4_MSG] = CanTSyn_OffsetMaster[CanTSynPduIdx].UserByte0; /* User Byte 0, default: 0 */
    CanTSynTxDataPtr[CANTSYN_B5_MSG] = CanTSyn_OffsetMaster[CanTSynPduIdx].UserByte1; /* User Byte 1, default: 0 */

    CanTSynTxDataPtr[CANTSYN_B6_MSG_RESERVED] = 0x00U; /* reserved, default: 0 */
    CanTSynTxDataPtr[CANTSYN_B7_MSG_RESERVED] = 0x00U; /* reserved, default: 0 */

    /* Byte 8-11: OfsTimeSec = 32 Bit offset time value in seconds */
    CanTSynTxDataPtr[CANTSYN_B8_MSG] = (((uint8)(seconds >> CANTSYN_GET_B3)));
    CanTSynTxDataPtr[CANTSYN_B9_MSG] = ((uint8)(seconds >> CANTSYN_GET_B2));
    CanTSynTxDataPtr[CANTSYN_B10_MSG] = ((uint8)(seconds >> CANTSYN_GET_B1));
    CanTSynTxDataPtr[CANTSYN_B11_MSG] = (uint8)seconds;

    /* Byte 12-15: OfsTimeNSec = 32 Bit offset time value in nanoseconds */
    CanTSynTxDataPtr[CANTSYN_B12_MSG] = (((uint8)(nanoseconds >> CANTSYN_GET_B3)));
    CanTSynTxDataPtr[CANTSYN_B13_MSG] = ((uint8)(nanoseconds >> CANTSYN_GET_B2));
    CanTSynTxDataPtr[CANTSYN_B14_MSG] = ((uint8)(nanoseconds >> CANTSYN_GET_B1));
    CanTSynTxDataPtr[CANTSYN_B15_MSG] = (uint8)nanoseconds;

#if(CANTSYN_TX_CRC_USED == STD_ON)
    /* check if CRC supported */
    if(CanTSyn_TimeMasterDomainPtr->IsTxCrcSecuredUsed == CANTSYN_CRC_SUPPORTED)
    {
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00112,1 */
      DataId = CanTSyn_TimeMasterConfig[CanTSynPduIdx+CANTSYN_NUMBER_OF_SYNC_MASTERS].SyncOfsDataIdListPtr[seqCounter];

      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00128,1 */ /* Type with CRC support */
      CanTSynTxDataPtr[CANTSYN_B0_MSG_TYPE] = CANTSYN_OFS_EXT_MSG_WITH_CRC;

      /*  CanTSynTxDataPtr[CANTSYN_B1_MSG] calculated last, CRC calculation necessary */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00085.CRC.ExtendedMessage,1 */
      CanTSynTxDataPtr[CANTSYN_B1_MSG] = CanTSyn_CalculateCRC(&CanTSynTxDataPtr[CANTSYN_B2_MSG_DOMAIN], DataId, CANTSYN_CRC_EXT_LENGTH);
    }
    else
#endif /* CANTSYN_TX_CRC_USED == STD_ON */
    {
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00111,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00126,1 */
      CanTSynTxDataPtr[CANTSYN_B0_MSG_TYPE] = CANTSYN_OFS_EXT_MSG_WITHOUT_CRC; /* Type */ /* No CRC support */
      CanTSynTxDataPtr[CANTSYN_B1_MSG] = CanTSyn_OffsetMaster[CanTSynPduIdx].UserByte2; /* User Byte 2, default: 0 */
    }
  }
  else
#endif /* CANTSYN_TX_USE_EXTENDED_MSG == STD_ON */
  {
    CanTSynTxDataPtr[CANTSYN_B2_MSG_DOMAIN] = CANTSYN_GET_B2_DOMAIN_SC(CanTSyn_TimeMasterDomainPtr->TimeDomainId, seqCounter);
    CanTSynTxDataPtr[CANTSYN_B3_MSG_RESERVED] = CanTSyn_OffsetMaster[CanTSynPduIdx].UserByte0; /* User Byte 0, default: 0 */

    /* assemble Bytes 4-7 in Big Endian format */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00008,1 */
    CanTSynTxDataPtr[CANTSYN_B4_MSG] = (((uint8)(seconds >> CANTSYN_GET_B3)));
    CanTSynTxDataPtr[CANTSYN_B5_MSG] = ((uint8)(seconds >> CANTSYN_GET_B2));
    CanTSynTxDataPtr[CANTSYN_B6_MSG] = ((uint8)(seconds >> CANTSYN_GET_B1));
    CanTSynTxDataPtr[CANTSYN_B7_MSG] = (uint8)seconds;

#if(CANTSYN_TX_CRC_USED == STD_ON)
    /* check if CRC supported */
    if(CanTSyn_TimeMasterDomainPtr->IsTxCrcSecuredUsed == CANTSYN_CRC_SUPPORTED)
    {
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00128,1 */ /* Type with CRC support */
      CanTSynTxDataPtr[CANTSYN_B0_MSG_TYPE] = CANTSYN_OFS_WITH_CRC;

      /* get DataId value based on the current sequence counter */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00054,1 */
      DataId = CanTSyn_TimeMasterConfig[CanTSynPduIdx+CANTSYN_NUMBER_OF_SYNC_MASTERS].SyncOfsDataIdListPtr[seqCounter];

      /* get the CRC value */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00050,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00085.CRC.NormalMessage,1 */
      CanTSynTxDataPtr[CANTSYN_B1_MSG] = CanTSyn_CalculateCRC(&CanTSynTxDataPtr[CANTSYN_B2_MSG_DOMAIN], DataId, CANTSYN_CRC_LENGTH);
    }
    else
#endif /* CANTSYN_TX_CRC_USED == STD_ON */
    {
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00126,1 */
      CanTSynTxDataPtr[CANTSYN_B0_MSG_TYPE] = CANTSYN_OFS_WITHOUT_CRC; /* Type */ /* No CRC support */
      CanTSynTxDataPtr[CANTSYN_B1_MSG] = CanTSyn_OffsetMaster[CanTSynPduIdx].UserByte1; /* User Byte 1, default: 0 */
    }
  }

  DBG_CANTSYN_FILLTXOFSFRAME_EXIT(CanTSynTimeStampPtr, CanTSynTxDataPtr, CanTSynPduIdx);
}


STATIC FUNC(void, CANTSYN_CODE) CanTSyn_FillTxOfnsFrame
(
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_DATA) CanTSynTxDataPtr,
  uint8 CanTSynPduIdx
)
{
  uint8 seqCounter = CANTSYN_SC_MAXLIMIT;
  uint32 nanoseconds = CanTSyn_OffsetMaster[CanTSynPduIdx].OfsTimeNSec;

#if(CANTSYN_TX_CRC_USED == STD_ON)
  CanTSyn_DataIDListType DataId;
#endif /* (CANTSYN_TX_CRC_USED == STD_ON) */

  DBG_CANTSYN_FILLTXOFNSFRAME_ENTRY(CanTSynTxDataPtr, CanTSynPduIdx);

  /* Sequence Counter increased at OFS */
  /* Use the same */
  seqCounter = CANTSYN_GET_SEQ_COUNTER(CanTSyn_OffsetMaster[CanTSynPduIdx].SeqCount);

  CanTSynTxDataPtr[CANTSYN_B2_MSG_DOMAIN] = CANTSYN_GET_B2_DOMAIN_SC(CanTSyn_TimeMasterConfig[CanTSynPduIdx+CANTSYN_NUMBER_OF_SYNC_MASTERS].TimeDomainId, seqCounter);
  CanTSynTxDataPtr[CANTSYN_B3_MSG_RESERVED] = CANTSYN_B3_SGW(CanTSynPduIdx); /* reserved (Bit 7 to Bit 1), default: 0
                                                                              * SGW (Bit 0)
                                                                              */

  /* assemble Bytes 4-7 in Big Endian format */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00008,1 */
  CanTSynTxDataPtr[CANTSYN_B4_MSG] = (((uint8)(nanoseconds >> CANTSYN_GET_B3)));
  CanTSynTxDataPtr[CANTSYN_B5_MSG] = ((uint8)(nanoseconds >> CANTSYN_GET_B2));
  CanTSynTxDataPtr[CANTSYN_B6_MSG] = ((uint8)(nanoseconds >> CANTSYN_GET_B1));
  CanTSynTxDataPtr[CANTSYN_B7_MSG] = (uint8)nanoseconds;

#if(CANTSYN_TX_CRC_USED == STD_ON)
  /* check if CRC supported */
  if(CanTSyn_TimeMasterConfig[CanTSynPduIdx+CANTSYN_NUMBER_OF_SYNC_MASTERS].IsTxCrcSecuredUsed == CANTSYN_CRC_SUPPORTED)
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00129,1 */ /* Type with CRC support */
    CanTSynTxDataPtr[CANTSYN_B0_MSG_TYPE] = CANTSYN_OFNS_WITH_CRC;

    /* get DataId value based on the current sequence counter */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00054,1 */
    DataId = CanTSyn_TimeMasterConfig[CanTSynPduIdx+CANTSYN_NUMBER_OF_SYNC_MASTERS].FupOfnsDataIdListPtr[seqCounter];

    /* get the CRC value */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00050,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00085.CRC.NormalMessage,1 */
    CanTSynTxDataPtr[CANTSYN_B1_MSG] = CanTSyn_CalculateCRC(&CanTSynTxDataPtr[CANTSYN_B2_MSG_DOMAIN], DataId, CANTSYN_CRC_LENGTH);
  }
  else
#endif /* CANTSYN_TX_CRC_USED == STD_ON */
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00127,1 */
    CanTSynTxDataPtr[CANTSYN_B0_MSG_TYPE] = CANTSYN_OFNS_WITHOUT_CRC; /* Type */ /* No CRC support */
    CanTSynTxDataPtr[CANTSYN_B1_MSG] = CanTSyn_OffsetMaster[CanTSynPduIdx].UserByte2; /* User Byte 2, default: 0 */
  }

  DBG_CANTSYN_FILLTXOFNSFRAME_EXIT(CanTSynTxDataPtr, CanTSynPduIdx);
}


STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_SendOfsMessage
(
  uint8 CanTSynPduIndex,
  uint8 CanTSynTxStatus,
  boolean CanTSynIsImmediateOFSTransmission
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  boolean MessageAccepted = FALSE;
  StbM_TimeStampType StbMTimeStamp;
  StbM_UserDataType UserData;
  uint8 SduData[CANTSYN_TX_MAX_MSG_LEN];
  P2CONST(CanTSyn_TimeMasterDomainConfigType, AUTOMATIC, CANTSYN_APPL_CONST)
          CanTSyn_TimeMasterDomainPtr = &CanTSyn_TimeMasterConfig[CanTSynPduIndex + CANTSYN_NUMBER_OF_SYNC_MASTERS];
  P2VAR(CanTSyn_OffsetMasterType, AUTOMATIC, CANTSYN_APPL_DATA)
        CanTSyn_OffsetMasterPtr = &CanTSyn_OffsetMaster[CanTSynPduIndex];

  DBG_CANTSYN_SENDOFSMESSAGE_ENTRY(CanTSynPduIndex, CanTSynTxStatus, CanTSynIsImmediateOFSTransmission);

  /* assemble the message */
  if(CANTSYN_OFNSMSG_SEND_WAIT == CanTSynTxStatus)
  {
    /* fill the OFNS message */
    CanTSyn_FillTxOfnsFrame(&SduData[0], CanTSynPduIndex);

    MessageAccepted = TRUE;
  }
  else
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00046,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00056,1 */
    /* get the the second portion and nanosecond of offset time base */
    /* !LINKSTO CanTSyn.EB.StbM_GetOffset_E_NOT_OK,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00135,1 */
    if(E_OK == StbM_GetOffset(CanTSyn_TimeMasterDomainPtr->SyncTimeBaseId, &StbMTimeStamp, &UserData))
    {
      /* save the nanosecond portion for the OFNS message */
      CanTSyn_OffsetMasterPtr->OfsTimeNSec = StbMTimeStamp.nanoseconds;

      /* check the Time Base Status for offset frames */
      if((StbMTimeStamp.timeBaseStatus & CANTSYN_SYNC_TO_GATEWAY_MASK) != 0U)
      {
        CanTSyn_OffsetMasterPtr->OfsSGWBit = CANTSYN_SYNCTOSUBDOMAIN;
      }
      else
      {
        CanTSyn_OffsetMasterPtr->OfsSGWBit = CANTSYN_SYNCTOGTM;
      }

      /* save the user Data */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00011,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00013,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00014,1 */
      CanTSyn_OffsetMasterPtr->UserByte0 = CANTSYN_GET_USERBYTE0(UserData);
      CanTSyn_OffsetMasterPtr->UserByte1 = CANTSYN_GET_USERBYTE1(UserData);
      CanTSyn_OffsetMasterPtr->UserByte2 = CANTSYN_GET_USERBYTE2(UserData);

      /* fill the OFS message */
      CanTSyn_FillTxOfsFrame(&StbMTimeStamp, &SduData[0], CanTSynPduIndex);

      /* restart the Sequence Counter when reached the limit */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00048,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00049_OFNS_SCUnchanged,1 */
      CanTSyn_OffsetMasterPtr->SeqCount = CANTSYN_SET_SEQ_COUNTER(CanTSyn_OffsetMasterPtr->SeqCount);

      MessageAccepted = TRUE;
    }
  }

  if(MessageAccepted == TRUE)
  {
    PduInfoType CanIfTxInfo;

#if (CANTSYN_TX_USE_EXTENDED_MSG == STD_ON)
    if(CanTSyn_TimeMasterDomainPtr->IsExtendedMessage)
    {
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00010.DLC.ExtendedMessage,1 */
      CanIfTxInfo.SduLength = CANTSYN_EXTENDED_MSG_SIZE;
    }
    else
#endif
    {
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00010.DLC.NormalMessage,1 */
      /* size of the message is 8 Bytes */
      CanIfTxInfo.SduLength = CANTSYN_MSG_SIZE;
    }

    CanIfTxInfo.SduDataPtr = SduData;

    if(CanTSynTxStatus == CANTSYN_OFSMSG_SEND_WAIT)
    {
      /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynTxStatus_OfsSENDWAIT-OfsWAITTXCONF,1 */
      /* wait for the OFS Confirmation */
      CanTSyn_OffsetMasterPtr->TxStatus = CANTSYN_OFSMSG_WAIT_TXCONF;
    }
    else
    {
      /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynTxStatus_OfnsSENDWAIT-OfnsWAITTXCONF,1 */
      /* wait for the OFNS Confirmation */
      CanTSyn_OffsetMasterPtr->TxStatus = CANTSYN_OFNSMSG_WAIT_TXCONF;
    }


    /* Transmit SYNC Message */
    /* !LINKSTO CanTSyn.EB.CanIf_Transmit_E_NOT_OK,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00136,1 */
    if(E_OK == CanIf_Transmit(CanTSyn_TimeMasterDomainPtr->TxPduId, &CanIfTxInfo))
    {
      if(CanTSynTxStatus == CANTSYN_OFSMSG_SEND_WAIT)
      {
#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON)
        if(FALSE != CanTSynIsImmediateOFSTransmission)
        {
          /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00119,1 */
          CanTSyn_OffsetMasterPtr->CyclicMsgResumeTime = CanTSyn_TimeMasterDomainPtr->CyclicMsgResumeTime;
        }
        else
#endif /* (CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON)*/
        {
          /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00038,1 */
          CanTSyn_OffsetMasterPtr->TimeTxPeriod = CanTSyn_TimeMasterDomainPtr->TxPeriodTimeout;
        }
      }

      RetVal = E_OK;
    }
  }

#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_OFF)
  TS_PARAM_UNUSED(CanTSynIsImmediateOFSTransmission);
#endif /* (CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_OFF) */

  DBG_CANTSYN_SENDOFSMESSAGE_EXIT(CanTSynPduIndex, CanTSynTxStatus, CanTSynIsImmediateOFSTransmission);

  return RetVal;
}


STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsTxOFSMessageAwaited
(
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_DATA) NewTimeBaseCounter,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) IsImmediateTransmissionPtr,
  uint8 PduId
)
{
  P2VAR(CanTSyn_OffsetMasterType, AUTOMATIC, CANTSYN_APPL_DATA)
    CanTSyn_OffsetMasterPtr = &CanTSyn_OffsetMaster[PduId];
  P2CONST(CanTSyn_TimeMasterDomainConfigType, AUTOMATIC, CANTSYN_APPL_CONST)
    CanTSyn_TimeMasterDomainPtr =
      &CanTSyn_TimeMasterConfig[PduId + CANTSYN_NUMBER_OF_SYNC_MASTERS];
  Std_ReturnType retVal = E_NOT_OK;
  boolean isImmediateOFSTransmission = FALSE;

  DBG_CANTSYN_ISTXOFSMESSAGEAWAITED_ENTRY(isImmediateTransmissionPtr, PduId);

  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00123,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00032,1 */
  if(CanTSyn_OffsetMasterPtr->DebounceTime != 0U)
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00124,1 */
    /* wait until timer will expire */
    CanTSyn_OffsetMasterPtr->DebounceTime--;
  }

  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00125,1 */
#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_OFF)
  if((CanTSyn_OffsetMasterPtr->TimeTxPeriod == 0U) &&
     (CanTSyn_OffsetMasterPtr->DebounceTime == 0U))
#else
  if(((CanTSyn_TimeMasterDomainPtr->IsImmediateTimeSync == TRUE) ||
      (CanTSyn_OffsetMasterPtr->TimeTxPeriod == 0U)) &&
     (CanTSyn_OffsetMasterPtr->DebounceTime == 0U))
#endif
  {
    StbM_TimeBaseStatusType syncTimeBaseStatus = 0U;
    StbM_TimeBaseStatusType offsetTimeBaseStatus = 0U;

    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00135,1 */
    (void)StbM_GetTimeBaseStatus(CanTSyn_TimeMasterDomainPtr->SyncTimeBaseId,
        &syncTimeBaseStatus, &offsetTimeBaseStatus
    );

    if((offsetTimeBaseStatus & CANTSYN_GLOBAL_TIME_BASE_MASK) != 0U)
    {
      /* If CanTSynImmediateTimeSync is set to TRUE, check if immediate transmission is possible */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00118,1 */
#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON)
      uint8 newOfsTimeBaseCounter = 0U;

      if(CanTSyn_TimeMasterDomainPtr->IsImmediateTimeSync == TRUE)
      {
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00135,1 */
        newOfsTimeBaseCounter =
            StbM_GetTimeBaseUpdateCounter(CanTSyn_TimeMasterDomainPtr->SyncTimeBaseId);

        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00117,1 */
        if(newOfsTimeBaseCounter != CanTSyn_OffsetMasterPtr->OfsTimeBaseCounter)
        {
          CanTSyn_OffsetMasterPtr->TimeTxPeriod = 0U;
          *NewTimeBaseCounter = newOfsTimeBaseCounter;
          /* If all the preconditions are fulfilled the immediate transmission is validated */
          isImmediateOFSTransmission = TRUE;
        }
      }

      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00038,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00119,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00121.OFS,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00120.OFS,1 */
      if((CanTSyn_OffsetMasterPtr->CyclicMsgResumeTime == 0U) ||
         (TRUE == isImmediateOFSTransmission))
#else
        TS_PARAM_UNUSED(NewTimeBaseCounter);
#endif /* (CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON) */
      {
        /* Transmit immediate OFS message if the immediate transmission is enabled and validated,
         * transmit cyclic OFS message if the immediate transmission is not enabled in configuration
         * and if the immediate transmission is enabled in configuration but not validated because
         * not all the preconditions are fulfilled */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00038,1 */
#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON)
        if((isImmediateOFSTransmission == TRUE) ||
           ((CanTSyn_TimeMasterDomainPtr->TxPeriodTimeout != 0U) && (CanTSyn_OffsetMasterPtr->TimeTxPeriod == 0U)))
#endif /* (CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON) */
        {
          *IsImmediateTransmissionPtr = isImmediateOFSTransmission;
          retVal = E_OK;
        }
      }
    }
  }

  DBG_CANTSYN_ISTXOFSMESSAGEAWAITED_EXIT(isImmediateTransmissionPtr, PduId);

  return retVal;
}


STATIC FUNC(void, CANTSYN_CODE) CanTSyn_TxProcessOfsMsg
(
  uint8 ConfirmationHandleId
)
{
  P2VAR(CanTSyn_OffsetMasterType, AUTOMATIC, CANTSYN_APPL_DATA)
    CanTSyn_OffsetMasterPtr = &CanTSyn_OffsetMaster[ConfirmationHandleId];
  P2CONST(CanTSyn_TimeMasterDomainConfigType, AUTOMATIC, CANTSYN_APPL_CONST)
    CanTSyn_TimeMasterDomainPtr =
      &CanTSyn_TimeMasterConfig[ConfirmationHandleId + CANTSYN_NUMBER_OF_SYNC_MASTERS];

  DBG_CANTSYN_TXPROCESSOFSMSG_ENTRY(ConfirmationHandleId);

#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON)
  if(CanTSyn_OffsetMasterPtr->CyclicMsgResumeTime != 0U)
  {
    /* Wait until CyclicMsgResumeTime will expire */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00120.OFS,1 */
    CanTSyn_OffsetMasterPtr->CyclicMsgResumeTime--;
  }
#endif /* (CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON) */

  if(CanTSyn_OffsetMasterPtr->ConfirmationTimeout != 0U)
  {
    /* wait until timer will expire */
    CanTSyn_OffsetMasterPtr->ConfirmationTimeout--;
  }

  if(CanTSyn_OffsetMasterPtr->TimeTxPeriod != 0U)
  {
    /* wait until timer will expire */
    CanTSyn_OffsetMasterPtr->TimeTxPeriod--;
  }

  /* check the status of the Master Domain */
  switch(CanTSyn_OffsetMasterPtr->TxStatus)
  {
  case CANTSYN_OFSMSG_SEND_WAIT:
  {
    boolean isImmediateOFSTransmission = FALSE;
    uint8 newOfsTimeBaseCounter = 0U;

    if(E_OK == CanTSyn_IsTxOFSMessageAwaited(&newOfsTimeBaseCounter, &isImmediateOFSTransmission, ConfirmationHandleId))
    {

      /* Transmit OFS Message */
      if(E_OK == CanTSyn_SendOfsMessage(ConfirmationHandleId, CANTSYN_OFSMSG_SEND_WAIT, isImmediateOFSTransmission))
      {
#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON)
        if(isImmediateOFSTransmission == TRUE)
        {
          /* save the new OffsetTimeBaseCounter */
          CanTSyn_OffsetMasterPtr->OfsTimeBaseCounter = newOfsTimeBaseCounter;
        }
#else
        TS_PARAM_UNUSED(newOfsTimeBaseCounter);
#endif /* (CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON) */
        /* load the Confirmation Timeout */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00042,1 */
        CanTSyn_OffsetMasterPtr->ConfirmationTimeout = CanTSyn_TimeMasterDomainPtr->ConfirmationTimeout;
      }
      else
      {
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00036,1 */
        /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynTxStatus_OfsSENDWAIT-OfsSENDWAIT,1 */
        /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynTxStatus_OfnsSENDWAIT_OfsSENDWAIT,1 */
        /* reset state machine due to timeout */
        CanTSyn_TxOffsetReset(ConfirmationHandleId);
      }
    }
  }
  break;

  case CANTSYN_OFNSMSG_SEND_WAIT:
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00124,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00040,1 */
    /* wait until timer will expire */
    CanTSyn_OffsetMasterPtr->DebounceTime--;

    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00123,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00125,1 */
    if(CanTSyn_OffsetMasterPtr->DebounceTime == 0U)
    {
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00039.SameTimeDomain,1 */
      (void)CanTSyn_SendOfsMessage(ConfirmationHandleId, CANTSYN_OFNSMSG_SEND_WAIT, FALSE);

      /* load the Confirmation Timeout */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00042,1 */
      CanTSyn_OffsetMasterPtr->ConfirmationTimeout = CanTSyn_TimeMasterDomainPtr->ConfirmationTimeout;
    }
  }
  break;

  /* CANTSYN_OFSMSG_WAIT_TXCONF */
  /* CANTSYN_OFNSMSG_WAIT_TXCONF */
  default:
  {
    if(CanTSyn_OffsetMasterPtr->ConfirmationTimeout == 0U)
    {
      /* reset state machine due to timeout */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00037,1 */
      /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynTxStatus_OfsWAITTXCONF_OfsSENDWAIT,1 */
      CanTSyn_TxOffsetReset(ConfirmationHandleId);
    }
  }
  break;
  }

  DBG_CANTSYN_TXPROCESSOFSMSG_EXIT(ConfirmationHandleId);
}
#endif /* CANTSYN_TX_OFFSET_USED == STD_ON */

#if ((CANTSYN_TX_SYNC_USED == STD_ON) || (CANTSYN_RX_SYNC_USED == STD_ON))
STATIC FUNC(StbM_TimeStampType, CANTSYN_CODE) CanTSyn_SubtractVLT
(
    P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, CANTSYN_VAR) virtualLocalTimeMinuendPtr,
    P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, CANTSYN_VAR) virtualLocalTimeSubtrahendPtr
)
{
  StbM_TimeStampType CalcTimeStamp = {0U, 0U, 0U, 0U};

  /* when virtualLocalTimeSubtrahendPtr->nanosecondsLo is greater than
   * virtualLocalTimeMinuendPtr->nanosecondsLo then
   * virtualLocalTimeMinuendPtr->nanosecondsHi is greater than 0.
   */
  if(virtualLocalTimeSubtrahendPtr->nanosecondsLo > virtualLocalTimeMinuendPtr->nanosecondsLo)
  {
    /* do not consider nanosecondsHi, a difference greater than 0xFFFFFFFF is too much
     * to be considered */
    CalcTimeStamp.nanoseconds = 0xFFFFFFFFU - virtualLocalTimeSubtrahendPtr->nanosecondsLo + virtualLocalTimeMinuendPtr->nanosecondsLo;
    /* + 1U because {0x01 0x00} - {0x00 0xFFFFFFFF} = 1 */
    CalcTimeStamp.nanoseconds += 1U;
  }
  else
  {
    CalcTimeStamp.nanoseconds = virtualLocalTimeMinuendPtr->nanosecondsLo - virtualLocalTimeSubtrahendPtr->nanosecondsLo;
  }

  return CalcTimeStamp;
}
#endif /* ((CANTSYN_TX_SYNC_USED == STD_ON) || (CANTSYN_RX_SYNC_USED == STD_ON)) */

#if(CANTSYN_TX_SYNC_USED == STD_ON)
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_SendSyncMessage
(
  uint8 ConfirmationHandleId,
  boolean IsImmediateSYNCTransmission
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  StbM_TimeStampType StbMTimeStamp;
  StbM_VirtualLocalTimeType T0VLT;
  StbM_UserDataType UserData;
  P2CONST(CanTSyn_TimeMasterDomainConfigType, AUTOMATIC, CANTSYN_APPL_CONST) CanTSyn_TimeMasterDomainPtr = &CanTSyn_TimeMasterConfig[ConfirmationHandleId];

  DBG_CANTSYN_SENDSYNCMESSAGE_ENTRY(ConfirmationHandleId, IsImmediateSYNCTransmission);

  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00045,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00056,1 */
  /* get the current time stamp that is valid at this time and */
  /* the User Data of the Time Base */

  /* !LINKSTO CanTSyn.EB.StbM_BusGetCurrentTime_E_NOT_OK,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00135,1 */
  if(E_OK == StbM_BusGetCurrentTime(CanTSyn_TimeMasterDomainPtr->SyncTimeBaseId, &StbMTimeStamp, &T0VLT, &UserData))
  {
    PduInfoType CanIfTxInfo;
    uint8 SduData[CANTSYN_TX_MAX_MSG_LEN];
    P2VAR(CanTSyn_SyncMasterType, AUTOMATIC, CANTSYN_APPL_DATA) CanTSyn_MasterPtr = &CanTSyn_SyncMaster[ConfirmationHandleId];

    /* save the seconds portion of T0 */
    CanTSyn_MasterPtr->SyncTimeT0.seconds = StbMTimeStamp.seconds;

    /* save the nanoseconds portion of T0 */
    CanTSyn_MasterPtr->SyncTimeT0.nanoseconds = StbMTimeStamp.nanoseconds;

    /* Save virtual local time */
    CanTSyn_MasterPtr->T0VLT = T0VLT;

    /* check the Time Base Status */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00030.SyncToSubDomain.SGW,1 */
    if((StbMTimeStamp.timeBaseStatus & CANTSYN_SYNC_TO_GATEWAY_MASK) != 0U)
    {
      CanTSyn_MasterPtr->SyncSGWBit = CANTSYN_SYNCTOSUBDOMAIN;
    }
    else
    {
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00030.SyncToGTM.SGW,1 */
      CanTSyn_MasterPtr->SyncSGWBit = CANTSYN_SYNCTOGTM;
    }

    /* save the user Data */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00011,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00013,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00014,1 */
    CanTSyn_MasterPtr->UserByte0 = CANTSYN_GET_USERBYTE0(UserData);
    CanTSyn_MasterPtr->UserByte1 = CANTSYN_GET_USERBYTE1(UserData);
    CanTSyn_MasterPtr->UserByte2 = CANTSYN_GET_USERBYTE2(UserData);

#if (CANTSYN_TX_USE_EXTENDED_MSG == STD_ON)
    if(CanTSyn_TimeMasterDomainPtr->IsExtendedMessage)
    {
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00010.DLC.ExtendedMessage,1 */
      CanIfTxInfo.SduLength = CANTSYN_EXTENDED_MSG_SIZE;
    }
    else
#endif
    {
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00010.DLC.NormalMessage,1 */
      /* size of the message is 8 Bytes */
      CanIfTxInfo.SduLength = CANTSYN_MSG_SIZE;
    }

    CanIfTxInfo.SduDataPtr = SduData;

    /* assemble the message */
    CanTSyn_FillTxSyncFrame(CanIfTxInfo.SduDataPtr, ConfirmationHandleId);

    /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynTxStatus_SyncSENDWAIT-SyncWAITTXCONF,1 */
    /* wait for Confirmation */
    CanTSyn_MasterPtr->TxStatus = CANTSYN_SYNCMSG_WAIT_TXCONF;

    /* restart the Sequence Counter when reached the limit */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00048,1 */
    CanTSyn_MasterPtr->SeqCount = CANTSYN_SET_SEQ_COUNTER(CanTSyn_MasterPtr->SeqCount);

    /* Transmit SYNC Message */
    /* !LINKSTO CanTSyn.EB.CanIf_Transmit_E_NOT_OK,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00136,1 */
    if(E_OK == CanIf_Transmit(CanTSyn_TimeMasterDomainPtr->TxPduId, &CanIfTxInfo))
    {
#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON)
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00119,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00120.SYNC,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00122,1 */
      if(IsImmediateSYNCTransmission == TRUE)
      {
        CanTSyn_MasterPtr->CyclicMsgResumeTime = CanTSyn_TimeMasterDomainPtr->CyclicMsgResumeTime;
      }
      else
#endif /* (CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON) */
      {
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00028,1 */
        CanTSyn_MasterPtr->TimeTxPeriod = CanTSyn_TimeMasterDomainPtr->TxPeriodTimeout;
      }

      RetVal = E_OK;
    }

  }

#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_OFF)
  TS_PARAM_UNUSED(IsImmediateSYNCTransmission);
#endif /* (CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_OFF)*/

  DBG_CANTSYN_SENDSYNCMESSAGE_EXIT(ConfirmationHandleId, IsImmediateSYNCTransmission);

  return RetVal;
}

#if (CANTSYN_MASTER_TIME_DOMAIN_VALIDATION_SUPPORT == STD_ON)
STATIC FUNC(StbM_TimeStampType, CANTSYN_CODE) CanTSyn_AddTimestamp_TimeValidation
(
    P2CONST(StbM_TimeStampType, AUTOMATIC, CANTSYN_VAR) timeStampAugendPtr,
    P2CONST(StbM_TimeStampType, AUTOMATIC, CANTSYN_VAR) timeStampAddendPtr
)
{
  StbM_TimeStampType CalcTimeStamp = {0U, 0U, 0U, 0U};

  /* ignore overflow, since each nanosecond element should not reach 10^9 */
  uint32 NsSum = timeStampAugendPtr->nanoseconds + timeStampAddendPtr->nanoseconds;
  /* calculate seconds sum, overflow detection happens later */
  uint32 SecSum = timeStampAugendPtr->seconds + timeStampAddendPtr->seconds;
  /* store carry to add it to the seconds later */
  uint8 NsCarry = (NsSum / CANTSYN_NS_PER_SEC);

  /* write back nanosecond part */
  CalcTimeStamp.nanoseconds = (NsSum % CANTSYN_NS_PER_SEC);
  /* write back (low-)second part considering the Nanosecond carry */
  CalcTimeStamp.seconds = SecSum + NsCarry;

  return CalcTimeStamp;
}
#endif /* (CANTSYN_MASTER_TIME_DOMAIN_VALIDATION_SUPPORT == STD_ON) */

STATIC FUNC(StbM_TimeStampType, CANTSYN_CODE) CanTSyn_AddTimestamp
(
    P2CONST(StbM_TimeStampType, AUTOMATIC, CANTSYN_VAR) timeStampAugendPtr,
    P2CONST(StbM_TimeStampType, AUTOMATIC, CANTSYN_VAR) timeStampAddendPtr
)
{
  StbM_TimeStampType CalcTimeStamp = {0U, 0U, 0U, 0U};

  /* ignore overflow, since each nanosecond element should not reach 10^9 */
  uint32 NsSum = timeStampAugendPtr->nanoseconds + timeStampAddendPtr->nanoseconds;
  /* calculate seconds sum, overflow detection happens later */
  uint32 SecSum = timeStampAugendPtr->seconds + timeStampAddendPtr->seconds;
  /* store carry to add it to the seconds later */
  uint8 NsCarry = ((NsSum / CANTSYN_NS_PER_SEC) > 0U) ? 1U : 0U;

  /* write back nanosecond part */
  CalcTimeStamp.nanoseconds = NsSum;
  /* write back (low-)second part considering the Nanosecond carry */
  CalcTimeStamp.seconds = SecSum + NsCarry;

  return CalcTimeStamp;
}

STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_SendFupMessage
(
  uint8 ConfirmationHandleId
)
{
  PduInfoType CanIfTxInfo;
  uint8 SduData[CANTSYN_TX_MAX_MSG_LEN];
  Std_ReturnType retVal = E_NOT_OK;
  uint8 OvsBit;
  P2CONST(CanTSyn_TimeMasterDomainConfigType, AUTOMATIC, CANTSYN_APPL_CONST) CanTSyn_TimeMasterDomainPtr = &CanTSyn_TimeMasterConfig[ConfirmationHandleId];
  P2VAR(CanTSyn_SyncMasterType, AUTOMATIC, CANTSYN_APPL_DATA) CanTSyn_MasterPtr = &CanTSyn_SyncMaster[ConfirmationHandleId];

  DBG_CANTSYN_SENDFUPMESSAGE_ENTRY(ConfirmationHandleId);

  /* if T4 >= 1s */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00045,1 */ /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00056,1 */
  /* save the seconds portion of Fup T4 into OVS */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00047,1 */
  OvsBit = (uint8)(CanTSyn_MasterPtr->FupTimeT4.nanoseconds/(CANTSYN_NANOSEC_MAX + 1U));

  /* save the nanoseconds portion from Fup T4 */
  CanTSyn_MasterPtr->FupTimeT4.nanoseconds = CanTSyn_MasterPtr->FupTimeT4.nanoseconds%(CANTSYN_NANOSEC_MAX + 1U);

#if (CANTSYN_TX_USE_EXTENDED_MSG == STD_ON)
  if(CanTSyn_TimeMasterDomainPtr->IsExtendedMessage)
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00010.DLC.ExtendedMessage,1 */
    CanIfTxInfo.SduLength = CANTSYN_EXTENDED_MSG_SIZE;
  }
  else
#endif
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00010.DLC.NormalMessage,1 */
    /* size of the message is 8 Bytes */
    CanIfTxInfo.SduLength = CANTSYN_MSG_SIZE;
  }

  CanIfTxInfo.SduDataPtr = SduData;

  /* assemble the message */
  CanTSyn_FillTxFupFrame(CanIfTxInfo.SduDataPtr, ConfirmationHandleId, OvsBit);

  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynTxStatus_FupSENDWAIT-FupWAITTXCONF,1 */
  /* wait for Confirmation */
  CanTSyn_MasterPtr->TxStatus = CANTSYN_FUPMSG_WAIT_TXCONF;

  /* Transmit FUP Message */
  /* !LINKSTO CanTSyn.EB.CanIf_Transmit_E_NOT_OK,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00136,1 */
  retVal = CanIf_Transmit(CanTSyn_TimeMasterDomainPtr->TxPduId, &CanIfTxInfo);

  DBG_CANTSYN_SENDFUPMESSAGE_EXIT(ConfirmationHandleId);

  return retVal;
}

STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsTxSYNCMessageAwaited
(
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_DATA) NewTimeBaseCounter,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) IsImmediateTransmissionPtr,
  uint8 PduId
)
{
  P2VAR(CanTSyn_SyncMasterType, AUTOMATIC, CANTSYN_APPL_DATA)
    CanTSyn_SyncMasterPtr = &CanTSyn_SyncMaster[PduId];
  P2CONST(CanTSyn_TimeMasterDomainConfigType, AUTOMATIC, CANTSYN_APPL_CONST)
    CanTSyn_TimeMasterDomainPtr = &CanTSyn_TimeMasterConfig[PduId];
  Std_ReturnType retVal = E_NOT_OK;
  boolean isImmediateSYNCTransmission = FALSE;
  StbM_TimeBaseStatusType syncTimeBaseStatus = 0U;
  StbM_TimeBaseStatusType offsetTimeBaseStatus = 0U;

  DBG_CANTSYN_ISTXSYNCMESSAGEAWAITED_ENTRY(isImmediateTransmissionPtr, PduId);

  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00123,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00032,1 */
  if(CanTSyn_SyncMasterPtr->DebounceTime != 0U)
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00124,1 */
    /* wait until timer will expire */
    CanTSyn_SyncMasterPtr->DebounceTime--;
  }

#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_OFF)
  if((CanTSyn_SyncMasterPtr->TimeTxPeriod == 0U) &&
     (CanTSyn_SyncMasterPtr->DebounceTime == 0U))
#else
  if(((CanTSyn_TimeMasterDomainPtr->IsImmediateTimeSync == TRUE) ||
      (CanTSyn_SyncMasterPtr->TimeTxPeriod == 0U)) &&
     (CanTSyn_SyncMasterPtr->DebounceTime == 0U))
#endif
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00135,1 */
    (void)StbM_GetTimeBaseStatus(CanTSyn_TimeMasterDomainPtr->SyncTimeBaseId,
        &syncTimeBaseStatus, &offsetTimeBaseStatus
    );

    if((syncTimeBaseStatus & CANTSYN_GLOBAL_TIME_BASE_MASK) != 0U)
    {
      /* If CanTSynImmediateTimeSync is set to TRUE, check if immediate transmission
       * is possible */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00118,1 */
#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON)
      uint8 newSyncTimeBaseCounter = 0U;

      if(CanTSyn_TimeMasterDomainPtr->IsImmediateTimeSync == TRUE)
      {
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00135,1 */
        newSyncTimeBaseCounter =
            StbM_GetTimeBaseUpdateCounter(CanTSyn_TimeMasterDomainPtr->SyncTimeBaseId);

        if(newSyncTimeBaseCounter != CanTSyn_SyncMasterPtr->syncTimeBaseCounter)
        {
          CanTSyn_SyncMasterPtr->TimeTxPeriod = 0U;
          *NewTimeBaseCounter = newSyncTimeBaseCounter;
          /* If all the preconditions are fulfilled the immediate transmission is validated */
          isImmediateSYNCTransmission = TRUE;
        }
      }

      /* Resume cyclic message transmission if CyclicMsgResumeTime has reached a value equal or
       * less than zero */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00121.SYNC,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00122,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00028,1 */
      if((CanTSyn_SyncMasterPtr->CyclicMsgResumeTime == 0U) ||
         (TRUE == isImmediateSYNCTransmission))
#else
      TS_PARAM_UNUSED(NewTimeBaseCounter);
#endif /* (CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON) */
      {
        /* Transmit immediate SYNC message if the immediate transmission is enabled and validated,
         * transmit cyclic SYNC message if the immediate transmission is not enabled in
         * configuration and if the immediate transmission is enabled in configuration but not
         * validated because not all the preconditions are fulfilled */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00028,1 */
#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON)
        if((isImmediateSYNCTransmission == TRUE) ||
            ((CanTSyn_TimeMasterDomainPtr->TxPeriodTimeout != 0U) && (CanTSyn_SyncMasterPtr->TimeTxPeriod == 0U)))
#endif
        {
          *IsImmediateTransmissionPtr = isImmediateSYNCTransmission;
          retVal = E_OK;
        }
      }
    }
  }

  DBG_CANTSYN_ISTXSYNCMESSAGEAWAITED_EXIT(isImmediateTransmissionPtr, PduId);

  return retVal;
}


STATIC FUNC(void, CANTSYN_CODE) CanTSyn_TxProcessSyncMsg
(
  uint8 ConfirmationHandleId
)
{
  P2VAR(CanTSyn_SyncMasterType, AUTOMATIC, CANTSYN_APPL_DATA)
    CanTSyn_SyncMasterPtr = &CanTSyn_SyncMaster[ConfirmationHandleId];
  P2CONST(CanTSyn_TimeMasterDomainConfigType, AUTOMATIC, CANTSYN_APPL_CONST)
    CanTSyn_TimeMasterDomainPtr = &CanTSyn_TimeMasterConfig[ConfirmationHandleId];

  DBG_CANTSYN_TXPROCESSSYNCMSG_ENTRY(ConfirmationHandleId);

#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON)
  if(CanTSyn_SyncMasterPtr->CyclicMsgResumeTime != 0U)
  {
    /* Wait until CyclicMsgResumeTime will expire */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00120.SYNC,1 */
    CanTSyn_SyncMasterPtr->CyclicMsgResumeTime--;
  }
#endif /* (CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON) */

  if(CanTSyn_SyncMasterPtr->TimeTxPeriod != 0U)
  {
      /* wait until timer will expire */
      CanTSyn_SyncMasterPtr->TimeTxPeriod--;
  }

  if(CanTSyn_SyncMasterPtr->ConfirmationTimeout != 0U)
  {
    /* wait until timer will expire */
    CanTSyn_SyncMasterPtr->ConfirmationTimeout--;
  }

  /* check the status of the Master Domain */
  switch(CanTSyn_SyncMasterPtr->TxStatus)
  {
  case CANTSYN_SYNCMSG_SEND_WAIT:
  {
    boolean isImmediateSYNCTransmission = FALSE;
    uint8 newSyncTimeBaseCounter = 0U;

    if(E_OK == CanTSyn_IsTxSYNCMessageAwaited(&newSyncTimeBaseCounter, &isImmediateSYNCTransmission, ConfirmationHandleId))
    {
      /* Transmit immediate or synchronous SYNC Message */
      if(E_OK == CanTSyn_SendSyncMessage(ConfirmationHandleId, isImmediateSYNCTransmission))
      {
#if(CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON)
        if(isImmediateSYNCTransmission == TRUE)
        {
          /* save the new SyncTimeBaseCounter */
          CanTSyn_SyncMasterPtr->syncTimeBaseCounter = newSyncTimeBaseCounter;
        }
#else
        TS_PARAM_UNUSED(newSyncTimeBaseCounter);
#endif /* (CANTSYN_TX_IMMEDIATE_TRANSMISSION == STD_ON) */
        /* load the Confirmation Timeout */
        CanTSyn_SyncMasterPtr->ConfirmationTimeout =
            CanTSyn_TimeMasterDomainPtr->ConfirmationTimeout;
      }
      else
      {
        /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynTxStatus_SyncSENDWAIT-SyncSENDWAIT,1 */
        /* reset state machine due to timeout */
        CanTSyn_TxSyncReset(ConfirmationHandleId);
      }
    }
  }
  break;

  case CANTSYN_FUPMSG_SEND_WAIT:
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00124,1 */
    /* wait until timer will expire */
    CanTSyn_SyncMasterPtr->DebounceTime--;

    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00123,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00125,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00032,1 */
    if(CanTSyn_SyncMasterPtr->DebounceTime == 0U)
    {
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00029.SameTimeDomain,1 */
      if(E_OK != CanTSyn_SendFupMessage(ConfirmationHandleId))
      {
        /* reset state machine due to timeout */
        /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynTxStatus_FupSENDWAIT-SyncSENDWAIT,1 */
        CanTSyn_TxSyncReset(ConfirmationHandleId);
      }
      else
      {
        /* load the Confirmation Timeout */
        CanTSyn_SyncMasterPtr->ConfirmationTimeout =
            CanTSyn_TimeMasterDomainPtr->ConfirmationTimeout;
      }

      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00049.FUP.SCUnchanged,1 */
      /* Sequence counter remains unchanged on every transmission request of a FUP message.*/
    }
  }
  break;

  /* CANTSYN_FUPMSG_WAIT_TXCONF */
  /* CANTSYN_SYNCMSG_WAIT_TXCONF */
  default:
  {
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00033,1 */
    /* observe Confirmation Timeout */
    if(CanTSyn_SyncMasterPtr->ConfirmationTimeout == 0U)
    {
      /* reset state machine due to timeout */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00027,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00037,1 */
      /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynTxStatus_SyncWAITTXCONF_SyncSENDWAIT,1 */
      /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynTxStatus_FupWAITTXCONF_SyncSENDWAIT,1 */
      CanTSyn_TxSyncReset(ConfirmationHandleId);
    }
  }
  break;
  }

  DBG_CANTSYN_TXPROCESSSYNCMSG_EXIT(ConfirmationHandleId);
}


STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ConfirmSyncMsg
(
  uint8 CanTSyn_PduActiveIndex
)
{
  P2VAR(CanTSyn_SyncMasterType, AUTOMATIC, CANTSYN_VAR) CanTSyn_MasterPtr = &CanTSyn_SyncMaster[CanTSyn_PduActiveIndex];
  P2CONST(CanTSyn_TimeMasterDomainConfigType, AUTOMATIC, CANTSYN_APPL_CONST)
          CanTSyn_TimeMasterDomainPtr = &CanTSyn_TimeMasterConfig[CanTSyn_PduActiveIndex];

  DBG_CANTSYN_CONFIRMSYNCMSG_ENTRY(CanTSyn_PduActiveIndex);

  switch(CanTSyn_MasterPtr->TxStatus)
  {
    case CANTSYN_SYNCMSG_WAIT_TXCONF:
    {
      boolean ProcessConfirmation = FALSE;
      StbM_VirtualLocalTimeType T1VLT;

      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00045,1 */

      SchM_Enter_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_1();

      /* !LINKSTO CanTSyn.EB.StbM_GetCurrentVirtualLocalTime_E_NOT_OK,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00135,1 */
      if(E_OK == StbM_GetCurrentVirtualLocalTime(CanTSyn_TimeMasterDomainPtr->SyncTimeBaseId, &T1VLT))
      {
        ProcessConfirmation = TRUE;
      }

      SchM_Exit_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_1();

      if(ProcessConfirmation)
      {
         StbM_TimeStampType T1_T0VLT;
         StbM_VirtualLocalTimeType T0VLT = CanTSyn_MasterPtr->T0VLT;
         StbM_TimeStampType T0SyncNs = {0U, 0U, 0U, 0U};

         T0SyncNs.nanoseconds = CanTSyn_MasterPtr->SyncTimeT0.nanoseconds;

         T1_T0VLT = CanTSyn_SubtractVLT(&T1VLT, &T0VLT);
         CanTSyn_MasterPtr->FupTimeT4 = CanTSyn_AddTimestamp(&T0SyncNs, &T1_T0VLT);

        /* load Tx Follow Up Offset Timeout */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00032,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00124,1 */
        CanTSyn_MasterPtr->DebounceTime = CanTSyn_TimeMasterDomainPtr->DebounceTime;

        /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynTxStatus_SyncWAITTXCONF-FupSENDWAIT,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00026,1 */
        /* FUP message will be sent next */
        CanTSyn_MasterPtr->TxStatus = CANTSYN_FUPMSG_SEND_WAIT;
        /* !LINKSTO CanTSyn.ASR19-11.SWS_CanTSyn_00137,1 */
        /* !LINKSTO CanTSyn.ASR19-11.SWS_CanTSyn_00138,1 */
#if (CANTSYN_MASTER_TIME_DOMAIN_VALIDATION_SUPPORT == STD_ON)
        /* !LINKSTO CanTSyn.ASR19-11.SWS_CanTSyn_00141,1 */
        if(TRUE == CanTSyn_TimeMasterDomainPtr->IsTimeValidationUsed)
        {
          StbM_CanTimeMasterMeasurementType MasterMeasureData = {0U, {0U, 0U}, {0U, 0U}, 0U};
          uint8 sequenceCounterVal = CANTSYN_GET_SEQ_COUNTER(CanTSyn_SyncMaster[CanTSyn_PduActiveIndex].SeqCount);
          StbM_TimeStampType FullPreciseOriginTimestamp = CanTSyn_AddTimestamp_TimeValidation(&(CanTSyn_MasterPtr->SyncTimeT0), &T1_T0VLT);
          /* !LINKSTO CanTSyn.ASR20-11.SWS_CanTSyn_00142.SequenceCounter,1 */
          MasterMeasureData.sequenceCounter = (uint16)sequenceCounterVal;
          /* !LINKSTO CanTSyn.ASR20-11.SWS_CanTSyn_00142.syncEgressTimestamp,1 */
          MasterMeasureData.syncEgressTimestamp = T1VLT;
          /* !LINKSTO CanTSyn.ASR20-11.SWS_CanTSyn_00142.preciseOriginTimestamp,1 */
          MasterMeasureData.preciseOriginTimestamp.nanoseconds = FullPreciseOriginTimestamp.nanoseconds;
          MasterMeasureData.preciseOriginTimestamp.seconds = FullPreciseOriginTimestamp.seconds;
          /* !LINKSTO CanTSyn.ASR20-11.SWS_CanTSyn_00142.segmentId,1 */
          MasterMeasureData.segmentId=CanTSyn_TimeMasterDomainPtr->NetworkSegmentId;

         /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00135,1 */
          (void)StbM_CanSetMasterTimingData(CanTSyn_TimeMasterDomainPtr->SyncTimeBaseId, &MasterMeasureData);
        }
#endif /* CANTSYN_MASTER_TIME_DOMAIN_VALIDATION_SUPPORT == STD_ON */
      }
    }
    break;

    case CANTSYN_FUPMSG_WAIT_TXCONF:
    {
      /* load the Debounce Timeout */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00124,1 */
      CanTSyn_MasterPtr->DebounceTime = CanTSyn_TimeMasterDomainPtr->DebounceTime;
      /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynTxStatus_FupWAITTXCONF_SyncSENDWAIT,1 */
      /* SYNC message will be sent next */
      CanTSyn_MasterPtr->TxStatus = CANTSYN_SYNCMSG_SEND_WAIT;
    }
    break;

    default:
       /* Tx Confirmation not awaited */
    break;
  }
  DBG_CANTSYN_CONFIRMSYNCMSG_EXIT(CanTSyn_PduActiveIndex);
}
#endif /* CANTSYN_TX_SYNC_USED == STD_ON */


#if(CANTSYN_TX_OFFSET_USED == STD_ON)
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ConfirmOfsMsg
(
  uint8 CanTSyn_PduActiveIndex
)
{
  P2VAR(CanTSyn_OffsetMasterType, AUTOMATIC, CANTSYN_VAR) CanTSyn_MasterPtr = &CanTSyn_OffsetMaster[CanTSyn_PduActiveIndex];
  P2CONST(CanTSyn_TimeMasterDomainConfigType, AUTOMATIC, CANTSYN_APPL_CONST) CanTSyn_TimeMasterDomainPtr =
      &CanTSyn_TimeMasterConfig[CanTSyn_PduActiveIndex + CANTSYN_NUMBER_OF_SYNC_MASTERS];

  DBG_CANTSYN_CONFIRMOFSMSG_ENTRY(CanTSyn_PduActiveIndex);

  switch(CanTSyn_MasterPtr->TxStatus)
  {
    case CANTSYN_OFSMSG_WAIT_TXCONF:
    {
      /* load Tx Follow Up Offset Timeout */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00040,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00124,1 */
      CanTSyn_MasterPtr->DebounceTime = CanTSyn_TimeMasterDomainPtr->DebounceTime;
#if (CANTSYN_TX_USE_EXTENDED_MSG == STD_ON)
      if(CanTSyn_TimeMasterDomainPtr->IsExtendedMessage)
      {
        /* !LINKSTO CanTSyn.Dsn.OfsExtended.CanTSynTxStatus_OfsWAITTXCONF_OfsSENDWAIT,1 */
        /* !LINKSTO CanTSyn.EB.ExtendedOFS,1 */
        /* OFS message will be sent next */
        CanTSyn_MasterPtr->TxStatus = CANTSYN_OFSMSG_SEND_WAIT;
      }
      else
#endif
      {
        /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynTxStatus_OfsWAITTXCONF-OfnsSENDWAIT,1 */
        /* OFNS message will be sent next */
        CanTSyn_MasterPtr->TxStatus = CANTSYN_OFNSMSG_SEND_WAIT;
      }

    }
    break;

    case CANTSYN_OFNSMSG_WAIT_TXCONF:
    {
      /* load the Debounce Timeout */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00124,1 */
      CanTSyn_MasterPtr->DebounceTime = CanTSyn_TimeMasterDomainPtr->DebounceTime;
      /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynTxStatus_OfnsWAITTXCONF_OfsSENDWAIT,1 */
      /* OFS message will be sent next */
      CanTSyn_MasterPtr->TxStatus = CANTSYN_OFSMSG_SEND_WAIT;
    }
    break;

    default:
       /* Tx Confirmation not awaited */
    break;
  }

  DBG_CANTSYN_CONFIRMOFSMSG_EXIT(CanTSyn_PduActiveIndex);
}
#endif /* CANTSYN_TX_OFFSET_USED == STD_ON */


#if(CANTSYN_TX_CRC_USED == STD_ON)

STATIC FUNC(uint8, CANTSYN_CODE) CanTSyn_CalculateCRC
(
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) TxDataPtr,
  CanTSyn_DataIDListType DataId,
  uint8 CrcLength
)
{
  uint8 computedCRC;
  uint8 CRCData[CANTSYN_TX_MAX_MSG_LEN - 1U];
  uint8 DataIdPos = CrcLength - 1U;

  DBG_CANTSYN_CALCULATECRC_ENTRY(TxDataPtr, DataId, CrcLength);

  SchM_Enter_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();

  /* copy bytes 2 to CrcLength - 1U */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00055.CRC.ExtendedMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00055.CRC.NormalMessage,1 */
  TS_MemCpy(&CRCData[0], &TxDataPtr[0], (CrcLength - 1U));

  /* add the DataID Value based on the current sequence counter */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00054,1 */
  CRCData[DataIdPos] = DataId;

  SchM_Exit_CanTSyn_SCHM_CANTSYN_EXCLUSIVE_AREA_0();

  /* get the CRC value */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00050,1 */
  computedCRC = Crc_CalculateCRC8H2F(&CRCData[0], CrcLength, CANTSYN_START_VALUE_8H2F, TRUE);

  DBG_CANTSYN_CALCULATECRC_EXIT(TxDataPtr, DataId, CrcLength);

  return computedCRC;
}
#endif /* CANTSYN_TX_CRC_USED == STD_ON */

#if((CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON))
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_GetRxStatus
(
  uint8 SlavePduId,
  P2VAR(uint8, AUTOMATIC, CANTSYN_APPL_DATA) RxStatusPtr
)
{
  DBG_CANTSYN_GETRXSTATUS_ENTRY(SlavePduId, RxStatusPtr);

#if((CANTSYN_RX_SYNC_USED == STD_ON) && (CANTSYN_RX_OFFSET_USED == STD_ON))
  if(SlavePduId < CANTSYN_NUMBER_OF_SYNC_SLAVES)
  {
    *RxStatusPtr = CanTSyn_SyncSlave[SlavePduId].RxStatus;
  }
  else
  {
    *RxStatusPtr = CanTSyn_OffsetSlave[SlavePduId - CANTSYN_NUMBER_OF_SYNC_SLAVES].RxStatus;
  }
#elif(CANTSYN_RX_SYNC_USED == STD_ON)
  *RxStatusPtr = CanTSyn_SyncSlave[SlavePduId].RxStatus;
#elif(CANTSYN_RX_OFFSET_USED == STD_ON)
  *RxStatusPtr = CanTSyn_OffsetSlave[SlavePduId].RxStatus
#endif /* ((CANTSYN_RX_SYNC_USED == STD_ON) && (CANTSYN_RX_OFFSET_USED == STD_ON)) */

  DBG_CANTSYN_GETRXSTATUS_EXIT(SlavePduId, RxStatusPtr);
}

#if(CANTSYN_RX_SYNC_USED == STD_ON)
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsFUPType
(
  uint8 SlavePduId,
  uint8 Type,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) CrcValidation
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00110.ValidFUPMessage.NoCRC,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00110.ValidFUPMessage.CRC,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00110.InvalidType.FUPMessage,1 */
#if(CANTSYN_RX_CRC_USED == STD_ON)
  boolean isCrcType = CANTSYN_IS_CRC_VAL_FUP(SlavePduId, Type);
#endif

#if (CANTSYN_RX_CRC_USED == STD_OFF)
  if(
      (CANTSYN_IS_CRC_NOT_VAL_FUP(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_CRC_IGNORED_FUP(SlavePduId, Type) == TRUE)
    )
#else /*  (CANTSYN_RX_CRC_USED == STD_ON) */
  if(
      (CANTSYN_IS_CRC_NOT_VAL_FUP(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_CRC_IGNORED_FUP(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_CRC_OPTIONAL_FUP(SlavePduId, Type) == TRUE) ||
      (isCrcType)
    )
#endif /* (CANTSYN_RX_CRC_USED == STD_OFF) */
  {
    RetVal = E_OK;
  }
#if (CANTSYN_RX_CRC_USED == STD_ON)
  if(
      ((CANTSYN_IS_CRC_OPTIONAL_FUP(SlavePduId, Type) == TRUE) &&
       (CANTSYN_IS_FUP_CRC_TYPE(Type) == TRUE)) ||
      (isCrcType)
    )
  {
    *CrcValidation = TRUE;
  }
  else
#endif /* (CANTSYN_RX_CRC_USED == STD_ON) */
  {
    *CrcValidation = FALSE;
  }
  return RetVal;
}


STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsSYNCType
(
  uint8 SlavePduId,
  uint8 Type,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) CrcValidation
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  /* check the received message type */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00058.SYNC.ValidType,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00059.SYNC.ValidType,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00109.ValidSYNCMessage.NoCRC,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00109.ValidSYNCMessage.CRC,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00109.InvalidType.SYNCMessage,1 */
#if(CANTSYN_RX_CRC_USED == STD_ON)
  boolean isCrcType = CANTSYN_IS_CRC_VAL_SYNC(SlavePduId, Type);
#endif

#if (CANTSYN_RX_CRC_USED == STD_OFF)
  if(
      (CANTSYN_IS_CRC_NOT_VAL_SYNC(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_CRC_IGNORED_SYNC(SlavePduId, Type) == TRUE)
    )
#else /* (CANTSYN_RX_CRC_USED == STD_ON) */
  if(
      (CANTSYN_IS_CRC_NOT_VAL_SYNC(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_CRC_IGNORED_SYNC(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_CRC_OPTIONAL_SYNC(SlavePduId, Type) == TRUE) ||
      (isCrcType)
    )
#endif /* (CANTSYN_RX_CRC_USED == STD_OFF) */
  {
    RetVal = E_OK;
  }
#if (CANTSYN_RX_CRC_USED == STD_ON)
  if(
      ((CANTSYN_IS_CRC_OPTIONAL_SYNC(SlavePduId, Type) == TRUE) &&
       (CANTSYN_IS_SYNC_CRC_TYPE(Type) == TRUE)) ||
      (isCrcType)
    )
  {
    *CrcValidation = TRUE;
  }
  else
#endif /* (CANTSYN_RX_CRC_USED == STD_ON) */
  {
    *CrcValidation = FALSE;
  }
  return RetVal;
}
#endif /* CANTSYN_RX_SYNC_USED == STD_ON */

#if(CANTSYN_RX_OFFSET_USED == STD_ON)
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsOFNSType
(
  uint8 SlavePduId,
  uint8 Type,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) CrcValidation
)
{
  Std_ReturnType RetVal = E_NOT_OK;

    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00114.ValidOFNSMessage.NoCRC,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00114.ValidOFNSMessage.CRC,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00114.InvalidType.OFNSMessage,1 */
#if(CANTSYN_RX_CRC_USED == STD_ON)
  boolean isCrcType = CANTSYN_IS_CRC_VAL_OFNS(SlavePduId, Type);
#endif

/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00070.ValidOFNSMessage,1 */
#if (CANTSYN_RX_CRC_USED == STD_OFF)
  if(
      (CANTSYN_IS_CRC_NOT_VAL_OFNS(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_CRC_IGNORED_OFNS(SlavePduId, Type) == TRUE)
    )
#else /* (CANTSYN_RX_CRC_USED == STD_ON) */
  if(
      (CANTSYN_IS_CRC_NOT_VAL_OFNS(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_CRC_IGNORED_OFNS(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_CRC_OPTIONAL_OFNS(SlavePduId, Type) == TRUE) ||
      (isCrcType)
    )
#endif /* (CANTSYN_RX_CRC_USED == STD_OFF) */
  {
    RetVal = E_OK;
  }
#if (CANTSYN_RX_CRC_USED == STD_ON)
  if(
      ((CANTSYN_IS_CRC_OPTIONAL_OFNS(SlavePduId, Type) == TRUE) &&
       (CANTSYN_IS_OFNS_CRC_TYPE(Type) == TRUE)) ||
      (isCrcType)
    )
  {
    *CrcValidation = TRUE;
  }
  else
#endif /* (CANTSYN_RX_CRC_USED == STD_ON) */
  {
    *CrcValidation = FALSE;
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsOFSType
(
  uint8 SlavePduId,
  uint8 Type,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) CrcValidation
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if(CANTSYN_RX_USE_EXTENDED_MSG == STD_ON)
  if(CanTSyn_TimeSlaveConfig[SlavePduId].IsExtendedMessage)
  {
    RetVal = CanTSyn_IsOFSExtenedType(SlavePduId, Type, CrcValidation);
  }
  else
#endif /* CANTSYN_RX_USE_EXTENDED_MSG == STD_ON */
  {
    RetVal = CanTSyn_IsOFSNormalType(SlavePduId, Type, CrcValidation);
  }
  return RetVal;
}

#if(CANTSYN_RX_USE_EXTENDED_MSG == STD_ON)
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsOFSExtenedType
(
  uint8 SlavePduId,
  uint8 Type,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) CrcValidation
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if(CANTSYN_RX_CRC_USED == STD_ON)
  boolean isCrcType = CANTSYN_IS_EXT_CRC_VAL_OFS(SlavePduId, Type);
#endif

  /* check the received message type */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00066.Valid.NormalOFSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00067.Valid.OFSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00065.Valid.NormalOFSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00113.ValidOFSMessage.NoCRC,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00113.ValidOFSMessage.CRC,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00113.InvalidType.OFSMessage,1 */
#if (CANTSYN_RX_CRC_USED == STD_OFF)
  if(
      (CANTSYN_IS_EXT_CRC_NOT_VAL_OFS(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_EXT_CRC_IGNORED_OFS(SlavePduId, Type) == TRUE)
    )
#else /* (CANTSYN_RX_CRC_USED == STD_ON) */
  if(
      (CANTSYN_IS_EXT_CRC_NOT_VAL_OFS(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_EXT_CRC_IGNORED_OFS(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_EXT_CRC_OPTIONAL_OFS(SlavePduId, Type) == TRUE) ||
      (isCrcType)
    )
#endif /* (CANTSYN_RX_CRC_USED == STD_OFF) */
  {
    RetVal = E_OK;
  }
#if (CANTSYN_RX_CRC_USED == STD_ON)
  if(
    ((CANTSYN_IS_EXT_CRC_OPTIONAL_OFS(SlavePduId, Type) == TRUE) &&
     (CANTSYN_IS_OFS_EXT_CRC_TYPE(Type) == TRUE)) ||
    (isCrcType)
  )
  {
    *CrcValidation = TRUE;
  }
  else
#endif /* (CANTSYN_RX_CRC_USED == STD_ON) */
  {
    *CrcValidation = FALSE;
  }
  return RetVal;
}
#endif /* CANTSYN_RX_USE_EXTENDED_MSG == STD_ON */

STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_IsOFSNormalType
(
  uint8 SlavePduId,
  uint8 Type,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) CrcValidation
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if(CANTSYN_RX_CRC_USED == STD_ON)
  boolean isCrcType = CANTSYN_IS_CRC_VAL_OFS(SlavePduId, Type);
#endif

  /* check the received message type */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00066.Valid.NormalOFSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00067.Valid.OFSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00065.Valid.NormalOFSMessage,1 */
#if (CANTSYN_RX_CRC_USED == STD_OFF)
  if(
      (CANTSYN_IS_CRC_NOT_VAL_OFS(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_CRC_IGNORED_OFS(SlavePduId, Type) == TRUE)
    )
#else /* (CANTSYN_RX_CRC_USED == STD_ON) */
 if(
      (CANTSYN_IS_CRC_NOT_VAL_OFS(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_CRC_IGNORED_OFS(SlavePduId, Type) == TRUE) ||
      (CANTSYN_IS_CRC_OPTIONAL_OFS(SlavePduId, Type) == TRUE) ||
      (isCrcType)
    )
#endif /* (CANTSYN_RX_CRC_USED == STD_OFF) */
  {
    RetVal = E_OK;
  }
#if (CANTSYN_RX_CRC_USED == STD_ON)
  if(
      ((CANTSYN_IS_CRC_OPTIONAL_OFS(SlavePduId, Type) == TRUE) &&
       (CANTSYN_IS_OFS_CRC_TYPE(Type) == TRUE)) ||
      (isCrcType)
    )
  {
    *CrcValidation = TRUE;
  }
  else
#endif /* (CANTSYN_RX_CRC_USED == STD_ON) */
  {
    *CrcValidation = FALSE;
  }
  return RetVal;
}
#endif /* CANTSYN_RX_OFFSET_USED == STD_ON */

STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_CheckMsgType
(
  uint8 SlavePduId,
  uint8 RxStatus,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  P2VAR(boolean, AUTOMATIC, CANTSYN_APPL_DATA) CrcValidation
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  const uint8 Type = DataPtr[0];

  DBG_CANTSYN_CHECKMSGTYPE_ENTRY(SlavePduId, RxStatus, DataPtr);

  switch(RxStatus)
  {
#if(CANTSYN_RX_SYNC_USED == STD_ON)
    /* Fall through */
    case CANTSYN_FUPMSG_RX_WAIT:
#if(CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON)
    case CANTSYN_FUPMSG_RX_WAIT_TIMEOUT:
#endif /* (CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON) */
    {
      RetVal = CanTSyn_IsFUPType(SlavePduId, Type, CrcValidation);
      break;
    }

#if(CANTSYN_RX_OFFSET_USED == STD_ON)
    case CANTSYN_SYNCMSG_RX_WAIT:
#else
    default:
    /* CANTSYN_SYNCMSG_RX_WAIT */
#endif
    {
      RetVal = CanTSyn_IsSYNCType(SlavePduId, Type, CrcValidation);
      break;
    }
#endif /* CANTSYN_RX_SYNC_USED == STD_ON */

#if(CANTSYN_RX_OFFSET_USED == STD_ON)

    /* Fall through */
    case CANTSYN_OFNSMSG_RX_WAIT:
#if(CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON)
    case CANTSYN_OFNSMSG_RX_WAIT_TIMEOUT:
#endif /* (CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON) */
    {
      RetVal = CanTSyn_IsOFNSType(SlavePduId, Type, CrcValidation);
      break;
    }

    default:
    /* CANTSYN_OFSMSG_RX_WAIT */
    {
      RetVal = CanTSyn_IsOFSType(SlavePduId, Type, CrcValidation);
      break;
    }
#endif /* (CANTSYN_RX_OFFSET_USED == STD_ON) */
  }

  DBG_CANTSYN_CHECKMSGTYPE_EXIT(SlavePduId, RxStatus, DataPtr);

  return RetVal;
}

#if(CANTSYN_RX_CRC_USED == STD_ON)
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_CheckMsgCRC
(
  uint8 SlavePduId,
  uint8 RxStatus,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_CANTSYN_CHECKMSGCRC_ENTRY(SlavePduId, RxStatus, DataPtr);

  switch(RxStatus)
  {
#if(CANTSYN_RX_SYNC_USED == STD_ON)
    /* Fall through */
    case CANTSYN_FUPMSG_RX_WAIT:
#if(CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON)
    case CANTSYN_FUPMSG_RX_WAIT_TIMEOUT:
#endif /* (CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON) */
    {
      RetVal = CanTSyn_FupMsg_Rx_CRC_Check(SlavePduId, DataPtr);
      break;
    }

#if(CANTSYN_RX_OFFSET_USED == STD_ON)
    case CANTSYN_SYNCMSG_RX_WAIT:
#else
    default:
    /* CANTSYN_SYNCMSG_RX_WAIT */
#endif
    {
      RetVal = CanTSyn_SyncMsg_Rx_CRC_Check(SlavePduId, DataPtr);
      break;
    }
#endif /* CANTSYN_RX_SYNC_USED == STD_ON */

#if(CANTSYN_RX_OFFSET_USED == STD_ON)

    /* Fall through */
    case CANTSYN_OFNSMSG_RX_WAIT:
#if(CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON)
    case CANTSYN_OFNSMSG_RX_WAIT_TIMEOUT:
#endif /* (CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON) */
    {
      RetVal = CanTSyn_OfnsMsg_Rx_CRC_Check(SlavePduId, DataPtr);
      break;
    }

    default:
    /* CANTSYN_OFSMSG_RX_WAIT */
    {
      RetVal = CanTSyn_OfsMsg_Rx_CRC_Check(SlavePduId, DataPtr);
      break;
    }
#endif /* (CANTSYN_RX_OFFSET_USED == STD_ON) */
  }

  DBG_CANTSYN_CHECKMSGCRC_EXIT(SlavePduId, RxStatus, DataPtr);

  return RetVal;
}
#endif /* CANTSYN_RX_CRC_USED == STD_ON */

STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_CheckSequenceCounter
(
  uint8 RxPduId,
  uint8 ReceivedSC,
  boolean IsSync
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(CanTSyn_TimeSlaveDomainConfigType, AUTOMATIC, CANTSYN_APPL_CONST)
          CanTSyn_TimeSlaveConfigPtr = &CanTSyn_TimeSlaveConfig[RxPduId];
  uint8 SeqCount = 0U;
  uint8 DeltaSeqCount = 0U;

  /* get the sequence counter of the Time Domain */
  if(TRUE == IsSync)
  {
#if(CANTSYN_RX_SYNC_USED == STD_ON)
    SeqCount = CanTSyn_SyncSlave[RxPduId].SeqCount;
#endif /* CANTSYN_RX_SYNC_USED == STD_ON */
  }
  else
  {
#if(CANTSYN_RX_OFFSET_USED == STD_ON)
    SeqCount = CanTSyn_OffsetSlave[RxPduId - CANTSYN_NUMBER_OF_SYNC_SLAVES].SeqCount;
#endif /* CANTSYN_RX_OFFSET_USED == STD_ON */
  }



  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00079.Startup.NoSCCheck.SYNC,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00079.Startup.NoSCCheck.OFS,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00078.TS.IgnoreMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00078.TS.IgnoreMessage0DeltaSC,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00078.SYNC.TimeSequenceCounterJumpWidthLimit,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00078.OFS.TimeSequenceCounterJumpWidthLimit,1 */
  /* Omit SC check for the 1'st received message (SeqCount == CANTSYN_SC_STARTUP_TIMEOUT)
     Check if the SC of the 2'nd received message is a valid one (less or equal than Sequence Counter Jump Width)*/
  if(SeqCount == CANTSYN_SC_STARTUP_TIMEOUT)
  {
    RetVal = E_OK;
  }
  else
  {
     DeltaSeqCount = CANTSYN_DELTA_SC(ReceivedSC, SeqCount);
     if((DeltaSeqCount > 0U) && (DeltaSeqCount <= CanTSyn_TimeSlaveConfigPtr->ScJumpWidth))
     {
       RetVal = E_OK;
     }
     else
     {
        uint8 timeBaseStatus;
        StbM_TimeBaseStatusType syncTimeBaseStatus = 0U;
        StbM_TimeBaseStatusType offsetTimeBaseStatus = 0U;

        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00135,1 */
        (void)StbM_GetTimeBaseStatus(CanTSyn_TimeSlaveConfigPtr->SyncTimeBaseId,
                                     &syncTimeBaseStatus, &offsetTimeBaseStatus);

        if(TRUE == IsSync)
        {
          timeBaseStatus = syncTimeBaseStatus;
        }
        else
        {
          timeBaseStatus = offsetTimeBaseStatus;
        }

        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00079.DeltaSC.GreaterThan0.TBUpdate.NoSCCheck.SYNC,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00079.DeltaSC.GreaterThan0.TBUpdate.NoSCCheck.OFS,1 */
        /* if a timeout was reported at StbM level and the difference between the SCs of
           two consecutive SYNC messages is greater than 1, omit SC check */
        if(((timeBaseStatus & CANTSYN_STBM_TIMEOUT) != 0U) && (DeltaSeqCount > 0U))
        {
           RetVal = E_OK;
        }
     }
  }

  return RetVal;
}

#if(CANTSYN_RX_SYNC_USED == STD_ON)

#if(CANTSYN_RX_CRC_USED == STD_ON)
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_SyncMsg_Rx_CRC_Check
(
  uint8 SlavePduId,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 seqCounter = DataPtr[2] & CANTSYN_SC_MASK;

  DBG_CANTSYN_SYNCMSGRXCRCCHECK_ENTRY(SlavePduId, DataPtr);

/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00130,1 */
/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00131,1 */
#if(CANTSYN_RX_USE_EXTENDED_MSG == STD_ON)
  if(CanTSyn_TimeSlaveConfig[SlavePduId].IsExtendedMessage)
  {
    RetVal = CanTSyn_CheckCRC
        (CanTSyn_TimeSlaveConfig[SlavePduId].SyncOfsDataIdListPtr[seqCounter], DataPtr, CANTSYN_CRC_EXT_LENGTH);
  }
  else
#endif
  {
    /* call CanTSyn_CheckCRC() with a DataID Value based on the current sequence counter (SC) */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00084,1 */
    RetVal = CanTSyn_CheckCRC
        (CanTSyn_TimeSlaveConfig[SlavePduId].SyncOfsDataIdListPtr[seqCounter], DataPtr, CANTSYN_CRC_LENGTH);
  }

  DBG_CANTSYN_SYNCMSGRXCRCCHECK_EXIT(SlavePduId, DataPtr);

  return RetVal;
}

STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_FupMsg_Rx_CRC_Check
(
  uint8 SlavePduId,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 seqCounter = DataPtr[2] & CANTSYN_SC_MASK;

  DBG_CANTSYN_FUPMSGRXCRCCHECK_ENTRY(SlavePduId, DataPtr);

  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00130,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00131,1 */
#if(CANTSYN_RX_USE_EXTENDED_MSG == STD_ON)
  if(CanTSyn_TimeSlaveConfig[SlavePduId].IsExtendedMessage)
  {
    RetVal = CanTSyn_CheckCRC
        (CanTSyn_TimeSlaveConfig[SlavePduId].FupOfnsDataIdListPtr[seqCounter], DataPtr, CANTSYN_CRC_EXT_LENGTH);
  }
  else
#endif
  {
    /* call CanTSyn_CheckCRC() with a DataID Value based on the current sequence counter (SC) */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00084,1 */
    RetVal = CanTSyn_CheckCRC
        (CanTSyn_TimeSlaveConfig[SlavePduId].FupOfnsDataIdListPtr[seqCounter], DataPtr, CANTSYN_CRC_LENGTH);
  }

  DBG_CANTSYN_FUPMSGRXCRCCHECK_EXIT(SlavePduId, DataPtr);

  return RetVal;
}
#endif /* (CANTSYN_RX_CRC_USED == STD_ON) */

#endif /* CANTSYN_RX_SYNC_USED == STD_ON */


#if(CANTSYN_RX_OFFSET_USED == STD_ON)

#if(CANTSYN_RX_CRC_USED == STD_ON)
STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_OfsMsg_Rx_CRC_Check
(
  uint8 SlavePduId,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 seqCounter = DataPtr[2] & CANTSYN_SC_MASK;

  DBG_CANTSYN_OFSMSGRXCRCCHECK_ENTRY(SlavePduId, DataPtr);

/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00130,1 */
/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00131,1 */
/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00065.Valid.NormalOFSMessage,1 */
#if(CANTSYN_RX_USE_EXTENDED_MSG == STD_ON)
  if(CanTSyn_TimeSlaveConfig[SlavePduId].IsExtendedMessage)
  {
    RetVal = CanTSyn_CheckCRC
        (CanTSyn_TimeSlaveConfig[SlavePduId].SyncOfsDataIdListPtr[seqCounter], DataPtr, CANTSYN_CRC_EXT_LENGTH);
  }
  else
#endif /* (CANTSYN_RX_CRC_USED == STD_ON) */
  {
    /* call CanTSyn_CheckCRC() with a DataID Value based on the current sequence counter (SC) */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00084,1 */
    RetVal = CanTSyn_CheckCRC
        (CanTSyn_TimeSlaveConfig[SlavePduId].SyncOfsDataIdListPtr[seqCounter], DataPtr, CANTSYN_CRC_LENGTH);
  }

  DBG_CANTSYN_OFSMSGRXCRCCHECK_EXIT(SlavePduId, DataPtr);

  return RetVal;
}


STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_OfnsMsg_Rx_CRC_Check
(
  uint8 SlavePduId,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 seqCounter = DataPtr[2] & CANTSYN_SC_MASK;

  DBG_CANTSYN_OFSMSGRXCRCCHECK_ENTRY(SlavePduId, DataPtr);

  /* call CanTSyn_CheckCRC() with a DataID Value based on the current sequence counter (SC) */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00084,1 */
  RetVal = CanTSyn_CheckCRC
      (CanTSyn_TimeSlaveConfig[SlavePduId].FupOfnsDataIdListPtr[seqCounter], DataPtr, CANTSYN_CRC_LENGTH);

  DBG_CANTSYN_OFSMSGRXCRCCHECK_EXIT(SlavePduId, DataPtr);

  return RetVal;
}
#endif /* (CANTSYN_RX_CRC_USED == STD_ON) */

#endif /* CANTSYN_RX_OFFSET_USED == STD_ON */


#if(CANTSYN_RX_CRC_USED == STD_ON)

STATIC FUNC(Std_ReturnType, CANTSYN_CODE) CanTSyn_CheckCRC
(
  uint8 DataID,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  uint8 CrcLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  uint8 CRCData[CANTSYN_RX_MAX_MSG_LEN - 1U];
  uint8 DataIdPos = CrcLength - 1U;

  DBG_CANTSYN_CHECKCRC_ENTRY(DataID, DataPtr, CrcLength);

  /* copy bytes*/

  TS_MemCpy(&CRCData[0], &DataPtr[2], CrcLength - 1U);

  /* add the DataID Value based on the current sequence counter */
  CRCData[DataIdPos] = DataID;

  /* check if the CRC value is valid */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00080,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00057.SYNC.ValidTypeAndCRC,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00060.ValidFUPMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00068.ValidOFNSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00069.ValidOFNSMessage,1 */
  if(DataPtr[1] == Crc_CalculateCRC8H2F(&CRCData[0], CrcLength, CANTSYN_START_VALUE_8H2F, TRUE))
  {
    RetVal = E_OK;
  }

  DBG_CANTSYN_CHECKCRC_EXIT(DataID, DataPtr, CrcLength);

  return RetVal;
}
#endif /* (CANTSYN_RX_CRC_USED == STD_ON) */

#endif /* (CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON) */


#if((CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON))
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_RxReset
(
  uint8 SlavePduId
)
{
  DBG_CANTSYN_RXRESET_ENTRY(SlavePduId);

#if((CANTSYN_RX_SYNC_USED == STD_ON) && (CANTSYN_RX_OFFSET_USED == STD_ON))
  if(SlavePduId < CANTSYN_NUMBER_OF_SYNC_SLAVES)
  {
    /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAIT_InvalidCRC_SyncRXWAIT,1 */
    /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAITTIMEOUT_InvalidCRC_SyncRXWAIT,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00109.InvalidCRC.SYNCMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00057.SYNC.InvalidCRC,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00110.InvalidCRC.FUPMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00060.InvalidCRC.FUPMessage,1 */
    /* ignore the invalid FUP message (wrong CRC) and reset the SYNC RxStatus and clear all the variables for a new SYNC-FUP reception */

    /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAIT_InvalidLENGTH_SyncRXWAIT,1 */
    /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAIT_InvalidTYPE_SyncRXWAIT,1 */
    /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAITTIMEOUT_InvalidLENGTH_SyncRXWAIT,1 */
    /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAITTIMEOUT_InvalidTYPE_SyncRXWAIT,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00057.SYNC.InvalidType,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00058.SYNC.InvalidType,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00059.SYNC.InvalidType,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00060.InvalidType.FUPMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00061.InvalidType.FUPMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00062.InvalidType.FUPMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00061.InvalidType.FUPMessage,1 */
    /* ignore the invalid FUP message (wrong type or length) and reset the SYNC RxStatus and clear all the variables for a new SYNC-FUP reception */

    /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAIT_InvalidDOMAINID_SyncRXWAIT,1 */
    /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAITTIMEOUT_InvalidDOMAINID_SyncRXWAIT,1 */
    /* ignore the invalid FUP message (wrong time domain id) and reset the SYNC RxStatus and clear all the variables for a new SYNC-FUP reception */

    CanTSyn_RxSyncReset(SlavePduId);
  }
  else
  {
    /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAIT_InvalidCRC_OfsRXWAIT,1 */
    /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAITTIMEOUT_InvalidCRC_OfsRXWAIT,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00113.InvalidCRC.OFSMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00114.InvalidCRC.OFNSMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00065.InvalidCRC.NormalOFSMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00068.InvalidCRC.OFNSMessage,1 */
    /* ignore the invalid OFNS message (wrong CRC) and reset the OFS RxStatus and clear all the variables for a new OFS-OFNS reception */

    /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAIT_InvalidLENGTH_OfsRXWAIT,1 */
    /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAIT_InvalidTYPE_OfsRXWAIT,1 */
    /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAITTIMEOUT_InvalidLENGTH_OfsRXWAIT,1 */
    /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAITTIMEOUT_InvalidTYPE_OfsRXWAIT,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00065.InvalidType.NormalOFSMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00066.InvalidType.NormalOFSMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00067.InvalidType.OFSMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00068.InvalidType.OFNSMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00069.InvalidType.OFNSMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00067.InvalidType.OFSMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00070.InvalidType.OFNSMessage,1 */
    /* ignore the invalid OFNS message (wrong type or length) and reset the OFS RxStatus and clear all the variables for a new OFS-OFNS reception */

    /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAIT_InvalidDOMAINID_OfsRXWAIT,1 */
    /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAITTIMEOUT_InvalidDOMAINID_OfsRXWAIT,1 */
    /* ignore the invalid OFNS message (wrong time domain id) and reset the OFS RxStatus and clear all the variables for a new OFS-OFNS reception */

    CanTSyn_RxOffsetReset(SlavePduId - CANTSYN_NUMBER_OF_SYNC_SLAVES);
  }
#elif(CANTSYN_RX_SYNC_USED == STD_ON)
  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAIT_InvalidCRC_SyncRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAITTIMEOUT_InvalidCRC_SyncRXWAIT,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00109.InvalidCRC.SYNCMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00057.SYNC.InvalidCRC,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00110.InvalidCRC.FUPMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00060.InvalidCRC.FUPMessage,1 */
  /* ignore the invalid FUP message (wrong CRC) and reset the SYNC RxStatus and clear all the variables for a new SYNC-FUP reception */

  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAIT_InvalidLENGTH_SyncRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAIT_InvalidTYPE_SyncRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAITTIMEOUT_InvalidLENGTH_SyncRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAITTIMEOUT_InvalidTYPE_SyncRXWAIT,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00057.SYNC.InvalidType,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00058.SYNC.InvalidType,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00059.SYNC.InvalidType,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00060.InvalidType.FUPMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00061.InvalidType.FUPMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00062.InvalidType.FUPMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00061.InvalidType.FUPMessage,1 */
  /* ignore the invalid FUP message (wrong type or length) and reset the SYNC RxStatus and clear all the variables for a new SYNC-FUP reception */

  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAIT_InvalidDOMAINID_SyncRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAITTIMEOUT_InvalidDOMAINID_SyncRXWAIT,1 */
  /* ignore the invalid FUP message (wrong time domain id) and reset the SYNC RxStatus and clear all the variables for a new SYNC-FUP reception */

  CanTSyn_RxSyncReset(SlavePduId);
#elif(CANTSYN_RX_OFFSET_USED == STD_ON)
  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAIT_InvalidCRC_OfsRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAITTIMEOUT_InvalidCRC_OfsRXWAIT,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00113.InvalidCRC.OFSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00114.InvalidCRC.OFNSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00065.InvalidCRC.NormalOFSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00068.InvalidCRC.OFNSMessage,1 */
  /* ignore the invalid OFNS message (wrong CRC) and reset the OFS RxStatus and clear all the variables for a new OFS-OFNS reception */

  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAIT_InvalidLENGTH_OfsRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAIT_InvalidTYPE_OfsRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAITTIMEOUT_InvalidLENGTH_OfsRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAITTIMEOUT_InvalidTYPE_OfsRXWAIT,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00065.InvalidType.NormalOFSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00066.InvalidType.NormalOFSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00067.InvalidType.OFSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00068.InvalidType.OFNSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00069.InvalidType.OFNSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00067.InvalidType.OFSMessage,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00070.InvalidType.OFNSMessage,1 */
  /* ignore the invalid OFNS message (wrong type or length) and reset the OFS RxStatus and clear all the variables for a new OFS-OFNS reception */

  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAIT_InvalidDOMAINID_OfsRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAITTIMEOUT_InvalidDOMAINID_OfsRXWAIT,1 */
  /* ignore the invalid OFNS message (wrong time domain id) and reset the OFS RxStatus and clear all the variables for a new OFS-OFNS reception */

  CanTSyn_RxOffsetReset(SlavePduId - CANTSYN_NUMBER_OF_SYNC_SLAVES);
#endif /* (CANTSYN_RX_OFFSET_USED == STD_ON) */

  DBG_CANTSYN_RXRESET_EXIT(SlavePduId);
}
#endif /* ((CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON)) */



#if(CANTSYN_RX_SYNC_USED == STD_ON)
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ProcessRxSyncFrame
(
  StbM_VirtualLocalTimeType T2VLT,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  uint8 PduId
)
{
  P2VAR(CanTSyn_SyncSlaveType, AUTOMATIC, CANTSYN_APPL_DATA)
  CanTSyn_SyncSlavePtr = &CanTSyn_SyncSlave[PduId];

  DBG_CANTSYN_PROCESSRXSYNCFRAME_ENTRY(DataPtr, TimeBaseId, PduId, T2VLT);

  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00073,1 */

  /* save T2VLT */
  CanTSyn_SyncSlavePtr->T2VLT = T2VLT;

  /* save the Sequence counter for the FUP message */
  CanTSyn_SyncSlavePtr->SeqCount = CANTSYN_GET_SC_SLAVE(DataPtr[CANTSYN_B2_MSG_DOMAIN]);

  /* save the seconds part of T0 */
  /* disassemble Bytes 4-7 (Big Endian format) */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00008,1 */
  CanTSyn_SyncSlavePtr->T0Sec = CANTSYN_GET_TIME_VAL(DataPtr);

    /* save User Data Byte 0 */
    CanTSyn_SyncSlavePtr->UserByte0 = DataPtr[CANTSYN_B3_MSG];

  /* if no CRC used */
  if(DataPtr[CANTSYN_B0_MSG_TYPE] == CANTSYN_SYNC_WITHOUT_CRC)
  {
    /* save User Data Byte 1 */
    CanTSyn_SyncSlavePtr->UserByte1 = DataPtr[CANTSYN_B1_MSG];
  }

  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_SyncRXWAIT-FupRXWAIT,1 */
  /* next message received shall be the FUP message */
  CanTSyn_SyncSlavePtr->RxStatus = CANTSYN_FUPMSG_RX_WAIT;

  DBG_CANTSYN_PROCESSRXSYNCFRAME_EXIT(DataPtr, TimeBaseId, PduId, T2VLT);
}

STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ProcessRxFupFrame
(
  StbM_VirtualLocalTimeType T5VLT,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  uint8 PduId
)
{
  StbM_UserDataType userData;
  StbM_MeasurementType measureData;
  const StbM_SynchronizedTimeBaseType TimeBaseId = CanTSyn_TimeSlaveConfig[PduId].SyncTimeBaseId;
  StbM_TimeStampType globalTime = {0U, 0U, 0U, 0U};
  uint32 T4Nanoseconds = 0U;

  DBG_CANTSYN_PROCESSRXFUPFRAME_ENTRY(PduId, DataPtr, T5VLT);

  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00073,1 */

  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00115,1 */
  measureData.pathDelay = 0U;

  /* get the nanoseconds portion from the message */
  /* disassemble Bytes 4-7 (Big Endian format) */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00008,1 */
  T4Nanoseconds = CANTSYN_GET_TIME_VAL(DataPtr);

  /* SWS_CanTSyn_00086 - 5 : check if the nanoseconds part matches the defined range.
   * See the bug fix ticket [ASCCANTSYN-86]. */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00086,1 */
  if(T4Nanoseconds <= CANTSYN_NANOSEC_MAX)
  {
    uint8 SecondsOVS = DataPtr[CANTSYN_B3_MSG] & CANTSYN_OVS_MASK;
    StbM_VirtualLocalTimeType T2VLT = CanTSyn_SyncSlave[PduId].T2VLT;
    /* T3diff + SyncTimeNSec */
    StbM_TimeStampType T5_T2VLT = CanTSyn_SubtractVLT(&T5VLT, &T2VLT);

    globalTime.nanoseconds = T5_T2VLT.nanoseconds + T4Nanoseconds;
    /* set the user data */
    userData.userByte0 = CanTSyn_SyncSlave[PduId].UserByte0;

    /* if no CRC used */
    if(DataPtr[CANTSYN_B0_MSG_TYPE] == CANTSYN_FUP_WITHOUT_CRC)
    {
      /* save User Data Byte 1 and User Data Byte 2 */
      userData.userByte1 = CanTSyn_SyncSlave[PduId].UserByte1;
      userData.userByte2 = DataPtr[CANTSYN_B1_MSG];
      /* no CRC configured for SYNC and FUP messages */
      userData.userDataLength = 3U;
    }
    else
    {
      /* only userByte0 available */
      userData.userDataLength = 1U;
    }

    /* store the received T0 seconds value */
    globalTime.seconds = CanTSyn_SyncSlave[PduId].T0Sec;

    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00075,1 */
    /* T0 + OVS + SyncTimeSec */
    /* Check if the received seconds portion is in range */
    if((CANTSYN_MAX_32BIT_VALUE - globalTime.seconds) < SecondsOVS)
    {
      /* Adjust seconds to prevent an overflow and increment secondsHi in order to mark
       * the adjustment */
      globalTime.seconds = globalTime.seconds - (CANTSYN_MAX_32BIT_VALUE - SecondsOVS) - 1U;
      globalTime.secondsHi++;
    }
    else
    {
      globalTime.seconds = globalTime.seconds + SecondsOVS;
    }

    /* Adjust the seconds portion when the calculated nanoseconds portion that represents the
     * new global time exceeds the defined range. See the bug fix ticket [ASCCANTSYN-86]. */
    if(globalTime.nanoseconds > CANTSYN_NANOSEC_MAX)
    {
      /* Adjust nanoseconds with 1 second if the value of nanoseconds is not in range */
      globalTime.nanoseconds = globalTime.nanoseconds - CANTSYN_NS_PER_SEC;

      /* If the value of seconds portion is too high */
      if(globalTime.seconds == CANTSYN_MAX_32BIT_VALUE)
      {
        /* Adjust seconds to prevent an overflow and increment secondsHi in order to mark
         * the adjustment */
        globalTime.seconds = 0U;
        globalTime.secondsHi++;
      }
      else
      {
        /* Increment seconds if it is not equal with the maximum value */
        globalTime.seconds++;
      }
    }

    /* check if the SGW bit */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00133.SyncToSubDomain.SGW,1 */
      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00133.SyncToGTM.SGW,1 */
    if((DataPtr[CANTSYN_B3_MSG] & CANTSYN_SGW_MASK) != 0U)
    {
      globalTime.timeBaseStatus |= CANTSYN_SYNC_TO_GATEWAY_MASK;
    }

    /* set the global time */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00064,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00135,1 */
    (void)StbM_BusSetGlobalTime(TimeBaseId, &globalTime, &userData, &measureData, &T5VLT);
    /* !LINKSTO CanTSyn.ASR19-11.SWS_CanTSyn_00137,1 */
    /* !LINKSTO CanTSyn.ASR19-11.SWS_CanTSyn_00138,1 */
#if (CANTSYN_SLAVE_TIME_DOMAIN_VALIDATION_SUPPORT == STD_ON)
    /* !LINKSTO CanTSyn.ASR19-11.SWS_CanTSyn_00139,1 */
    if(TRUE == CanTSyn_TimeSlaveConfig[PduId].IsTimeValidationUsed)
    {
      /* !LINKSTO CanTSyn.ASR20-11.SWS_CanTSyn_00140.UnusedData,1 */
      StbM_CanTimeSlaveMeasurementType SlaveMeasureData = {0U, {0U ,0U}, {0U, 0U}, {0U, 0U}, {0U, 0U}, 0U};
      /* !LINKSTO CanTSyn.ASR20-11.SWS_CanTSyn_00140.SequenceCounter,1 */
      SlaveMeasureData.sequenceCounter = (uint16)CanTSyn_SyncSlave[PduId].SeqCount;
      /* !LINKSTO CanTSyn.ASR20-11.SWS_CanTSyn_00140.syncIngressTimestamp,1 */
      SlaveMeasureData.syncIngressTimestamp = T2VLT;
      /* !LINKSTO CanTSyn.ASR20-11.SWS_CanTSyn_00140.preciseOriginTimestamp,1 */
      SlaveMeasureData.preciseOriginTimestamp.nanoseconds = T4Nanoseconds;
      SlaveMeasureData.preciseOriginTimestamp.seconds = (CanTSyn_SyncSlave[PduId].T0Sec) + SecondsOVS;
      /* !LINKSTO CanTSyn.ASR20-11.SWS_CanTSyn_00140.segmentId,1 */
      SlaveMeasureData.segmentId=CanTSyn_TimeSlaveConfig[PduId].NetworkSegmentId;

      /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00135,1 */
      (void)StbM_CanSetSlaveTimingData(TimeBaseId, &SlaveMeasureData);
    }
#endif /* CANTSYN_SLAVE_TIME_DOMAIN_VALIDATION_SUPPORT */
  }

  /* ignore the invalid FUP message (nanoseconds above the defined range) and reset the SYNC RxStatus and clear all variables for a new SYNC-FUP reception
     or reset the SYNC RxStatus and clear all the variables for a new SYNC-FUP reception, after a successful reception of a SYNC-FUP sequence */
  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAIT_SyncRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAIT_NsAboveDefinedRange_SyncRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAITTIMEOUT_SyncRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAITTIMEOUT_NsAboveDefinedRange_SyncRXWAIT,1 */
  CanTSyn_RxSyncReset(PduId);

  DBG_CANTSYN_PROCESSRXFUPFRAME_EXIT(PduId, DataPtr, T5VLT);
}


STATIC FUNC(void, CANTSYN_CODE) CanTSyn_RxSyncReset
(
  uint8 CanTSyn_SlavePduActiveIndex
)
{
  P2VAR(CanTSyn_SyncSlaveType, AUTOMATIC, CANTSYN_APPL_DATA)
          CanTSyn_SyncSlavePtr = &CanTSyn_SyncSlave[CanTSyn_SlavePduActiveIndex];

  DBG_CANTSYN_RXSYNCRESET_ENTRY(CanTSyn_SlavePduActiveIndex);

  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_Init,1 */
  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAIT_SyncRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAITTIMEOUT_SyncRXWAIT,1 */
  /* first message awaited to be received is the SYNC message */
  CanTSyn_SyncSlavePtr->RxStatus = CANTSYN_SYNCMSG_RX_WAIT;

  /* Clear the Follow Up Offset Timeout */
  CanTSyn_SyncSlavePtr->TimeRxFollowUpOffset = 0U;

  CanTSyn_SyncSlavePtr->T0Sec = 0U;

  /* clear user data */
  CanTSyn_SyncSlavePtr->UserByte0 = 0U;
  CanTSyn_SyncSlavePtr->UserByte1 = 0U;

  DBG_CANTSYN_RXSYNCRESET_EXIT(CanTSyn_SlavePduActiveIndex);
}
#endif /* (CANTSYN_RX_SYNC_USED == STD_ON) */


#if(CANTSYN_RX_OFFSET_USED == STD_ON)
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_RxOffsetReset
(
  uint8 CanTSyn_SlavePduActiveIndex
)
{
  P2VAR(CanTSyn_OffsetSlaveType, AUTOMATIC, CANTSYN_APPL_DATA)
          CanTSyn_OffsetSlavePtr = &CanTSyn_OffsetSlave[CanTSyn_SlavePduActiveIndex];

  DBG_CANTSYN_RXOFFSETRESET_ENTRY(CanTSyn_SlavePduActiveIndex);

  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_Init,1 */
  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAIT_OfsRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAITTIMEOUT_OfsRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.OfsExtended.CanTSynRxStatus_OfsRXWAIT_OfsRXWAIT,1 */
  /* first message awaited to be received is the SYNC message */
  CanTSyn_OffsetSlavePtr->RxStatus = CANTSYN_OFSMSG_RX_WAIT;

  /* Clear the Follow Up Offset Timeout */
  CanTSyn_OffsetSlavePtr->TimeRxFollowUpOffset = 0U;

  /* Clear the Seconds portion */
  CanTSyn_OffsetSlavePtr->OfsSecLo = 0U;

  DBG_CANTSYN_RXOFFSETRESET_EXIT(CanTSyn_SlavePduActiveIndex);
}

#if(CANTSYN_RX_USE_EXTENDED_MSG == STD_ON)
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ProcessExtRxOfsFrame
(
  StbM_VirtualLocalTimeType T_VLT,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  uint8 PduId
)
{
  StbM_TimeStampType timeStamp = {0U, 0U, 0U, 0U};
  StbM_UserDataType userData;
  StbM_MeasurementType measureData;

  P2VAR(CanTSyn_OffsetSlaveType, AUTOMATIC, CANTSYN_APPL_DATA)
          CanTSyn_OffsetSlavePtr = &CanTSyn_OffsetSlave[PduId];

  DBG_CANTSYN_PROCESSEXTRXOFSFRAME_ENTRY(PduId, DataPtr);

  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00116,1 */
  measureData.pathDelay = 0U;

  /* get the nanoseconds portion from the message */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00074,1 */
  /* disassemble Bytes 4-7 (Big Endian format) */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00008,1 */
  timeStamp.nanoseconds = CANTSYN_GET_EXT_NANO_TIME_VAL(DataPtr);

  /* save the Sequence counter for the FUP message */
  CanTSyn_OffsetSlavePtr->SeqCount = CANTSYN_GET_SC_SLAVE(DataPtr[CANTSYN_B2_MSG_DOMAIN]);

  /* SWS_CanTSyn_00086 - 5 : check if the nanoseconds part matches the defined range */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00086,1 */
  if(timeStamp.nanoseconds <= CANTSYN_NANOSEC_MAX)
  {
    timeStamp.seconds = CANTSYN_GET_EXT_TIME_VAL(DataPtr);

    /* if no CRC used */
    if(DataPtr[CANTSYN_B0_MSG_TYPE] == CANTSYN_OFS_EXT_MSG_WITHOUT_CRC)
    {
      /* save userByte2 from Byte1 */
      userData.userByte2 = DataPtr[1];
      userData.userDataLength = 3U;
    }
    else
    {
      userData.userDataLength = 2U;
    }

    userData.userByte0 = DataPtr[4];
    userData.userByte1 = DataPtr[5];

    if((DataPtr[CANTSYN_B3_MSG] & CANTSYN_SGW_MASK) != 0U)
    {
      timeStamp.timeBaseStatus |= CANTSYN_SYNC_TO_GATEWAY_MASK;
    }

    /* set the global time */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00072.StbM.ExtendedMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00116,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00135,1 */
    (void)StbM_BusSetGlobalTime(CanTSyn_TimeSlaveConfig[PduId+CANTSYN_NUMBER_OF_SYNC_SLAVES].SyncTimeBaseId, &timeStamp, &userData, &measureData, &T_VLT);
  }

  /* ignore the invalid Extended OFS message (nanoseconds above the defined range) and reset the OFS RxStatus and clear all variables for a new Extended OFS reception
     or reset the OFS RxStatus and clear all the variables for a new Extended OFS reception, after a successful reception of an Extended OFS */
  /* !LINKSTO CanTSyn.Dsn.OfsExtended.CanTSynRxStatus_OfsRXWAIT_OfsRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.OfsExtended.CanTSynRxStatus_OfsRXWAIT_NsAboveDefinedRange_OfsRXWAIT,1 */
  CanTSyn_RxOffsetReset(PduId);

  DBG_CANTSYN_PROCESSEXTRXOFSFRAME_EXIT(PduId, DataPtr);
}
#endif /* (CANTSYN_RX_USE_EXTENDED_MSG == STD_ON) */


STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ProcessRxOfsFrame
(
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  uint8 PduId
)
{
  P2VAR(CanTSyn_OffsetSlaveType, AUTOMATIC, CANTSYN_APPL_DATA)
          CanTSyn_OffsetSlavePtr = &CanTSyn_OffsetSlave[PduId];

  DBG_CANTSYN_PROCESSRXOFSFRAME_ENTRY(PduId, DataPtr);

  /* save the Sequence counter for the FUP message */
  CanTSyn_OffsetSlavePtr->SeqCount = CANTSYN_GET_SC_SLAVE(DataPtr[CANTSYN_B2_MSG_DOMAIN]);

  /* save the seconds part */
  /* disassemble Bytes 4-7 (Big Endian format) */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00074,1 */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00008,1 */
  CanTSyn_OffsetSlavePtr->OfsSecLo = CANTSYN_GET_TIME_VAL(DataPtr);

  /* if no CRC used */
  if(DataPtr[CANTSYN_B0_MSG_TYPE] == CANTSYN_OFS_WITHOUT_CRC)
  {
    /* save userByte1 from Byte1 and userByte0 from Byte3 */
    CanTSyn_OffsetSlavePtr->UserByte1 = DataPtr[1];
    CanTSyn_OffsetSlavePtr->UserByte0 = DataPtr[3];
  }
  else
  {
    /* save userByte0 from Byte3 */
    CanTSyn_OffsetSlavePtr->UserByte0 = DataPtr[3];
  }

  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfsRXWAIT-OfnsRXWAIT,1 */
  /* next message received shall be the OFNS message */
  CanTSyn_OffsetSlavePtr->RxStatus = CANTSYN_OFNSMSG_RX_WAIT;

  DBG_CANTSYN_PROCESSRXOFSFRAME_EXIT(PduId, DataPtr);
}


STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ProcessRxOfnsFrame
(
  StbM_VirtualLocalTimeType T_VLT,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  uint8 PduId
)
{
  StbM_TimeStampType timeStamp = {0U, 0U, 0U, 0U};
  StbM_UserDataType userData;
  StbM_MeasurementType measureData;

  DBG_CANTSYN_PROCESSRXOFNSFRAME_ENTRY(PduId, DataPtr, T_VLT);

  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00116,1 */
  measureData.pathDelay = 0U;

  /* get the nanoseconds portion from the message */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00074,1 */
  /* disassemble Bytes 4-7 (Big Endian format) */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00008,1 */
  timeStamp.nanoseconds = CANTSYN_GET_TIME_VAL(DataPtr);

  /* SWS_CanTSyn_00086 - 5 : check if the nanoseconds part matches the defined range */
  /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00086,1 */
  if(timeStamp.nanoseconds <= CANTSYN_NANOSEC_MAX)
  {
    timeStamp.seconds = CanTSyn_OffsetSlave[PduId].OfsSecLo;

    /* set the User Data Byte 0 from received OFS frame */
    userData.userByte0 = CanTSyn_OffsetSlave[PduId].UserByte0;

    /* if no CRC used */
    if(DataPtr[CANTSYN_B0_MSG_TYPE] == CANTSYN_OFNS_WITHOUT_CRC)
    {
      /* save User Data Byte 1 and User Data Byte 2 from OFNS frame */
      userData.userByte1 = CanTSyn_OffsetSlave[PduId].UserByte1;
      userData.userByte2 = DataPtr[CANTSYN_B1_MSG];
      /* no CRC configured for SYNC and FUP messages */
      userData.userDataLength = 3U;
    }
    else
    {
      /* only userByte0 is available */
      userData.userDataLength = 1U;
    }

    /* check if the SGW bit is set */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00133.SyncToSubDomain.SGW,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00133.SyncToGTM.SGW,1 */
    if((DataPtr[CANTSYN_B3_MSG] & CANTSYN_SGW_MASK) != 0U)
    {
      timeStamp.timeBaseStatus |= CANTSYN_SYNC_TO_GATEWAY_MASK;
    }

    /* set the global time */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00072.StbM.NormalMessage,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00116,1 */
    /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00135,1 */
    (void)StbM_BusSetGlobalTime(CanTSyn_TimeSlaveConfig[PduId+CANTSYN_NUMBER_OF_SYNC_SLAVES].SyncTimeBaseId, &timeStamp, &userData, &measureData, &T_VLT);
  }

  /* ignore the invalid OFNS message (nanoseconds above the defined range) and reset the OFS RxStatus and clear all variables for a new OFS-OFNS reception
     or reset the OFS RxStatus and clear all the variables for a new OFS-OFNS reception, after a successful reception of a OFS-OFNS sequence */
  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAIT_OfsRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAITTIMEOUT_OfsRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAIT_NsAboveDefinedRange_OfsRXWAIT,1 */
  /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAITTIMEOUT_NsAboveDefinedRange_OfsRXWAIT,1 */
  CanTSyn_RxOffsetReset(PduId);

  DBG_CANTSYN_PROCESSRXOFNSFRAME_EXIT(PduId, DataPtr, T_VLT);
}
#endif /* (CANTSYN_RX_OFFSET_USED == STD_ON) */



#if((CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON))
STATIC FUNC(void, CANTSYN_CODE) CanTSyn_ProcessRxFrame
(
  uint8 SlavePduId,
  uint8 RxStatus,
  P2CONST(uint8, AUTOMATIC, CANTSYN_APPL_CONST) DataPtr,
  StbM_VirtualLocalTimeType T_VLT,
  PduLengthType messageLength
)
{
  const uint8 receivedSequenceCounter = DataPtr[2U] & CANTSYN_SC_MASK;
#if(CANTSYN_RX_OFFSET_USED == STD_ON)
  uint8 OffsetRxPduId = SlavePduId - CANTSYN_NUMBER_OF_SYNC_SLAVES;
#endif /* (CANTSYN_RX_OFFSET_USED == STD_ON) */

#if(CANTSYN_RX_USE_EXTENDED_MSG == STD_OFF)
   TS_PARAM_UNUSED(messageLength);
#endif

  DBG_CANTSYN_PROCESSRXFRAME_ENTRY(SlavePduId, RxStatus, DataPtr, T_VLT, messageLength);

  switch(RxStatus)
  {
#if(CANTSYN_RX_SYNC_USED == STD_ON)
    case CANTSYN_FUPMSG_RX_WAIT:
#if(CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON)
    case CANTSYN_FUPMSG_RX_WAIT_TIMEOUT:
#endif /* (CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON) */
    {
      /* check if the Sequence Counter is received with the same value like for the SYNC message */
      if(receivedSequenceCounter == CanTSyn_SyncSlave[SlavePduId].SeqCount)
      {
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00061.ValidFUPMessage,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00076.SYNCMatchFUP.SC,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00062.ValidFUPMessage,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00087,1 */
        CanTSyn_ProcessRxFupFrame(T_VLT,
                                 DataPtr,
                                 SlavePduId);
      }
      else
      {
        /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAIT_InvalidSC_SyncRXWAIT,1 */
        /* !LINKSTO CanTSyn.Dsn.SyncFup.CanTSynRxStatus_FupRXWAITTIMEOUT_InvalidSC_SyncRXWAIT,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00110.InvalidSC.FUPMessage,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00060.InvalidSC.FUPMessage,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00061.InvalidSC.FUPMessage,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00062.InvalidSC.FUPMessage,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00076.NoMatchSYNC_FUP.SC,1 */
        /* ignore the invalid FUP message (wrong SC) and reset the SYNC RxStatus and clear all the variables for a new SYNC-FUP reception */
        CanTSyn_RxSyncReset(SlavePduId);
      }
    }
    break;

#if(CANTSYN_RX_OFFSET_USED == STD_ON)
    case CANTSYN_SYNCMSG_RX_WAIT:
#else
    default:
     /* CANTSYN_SYNCMSG_RX_WAIT */
#endif /* CANTSYN_RX_OFFSET_USED == STD_ON */
    {
      /* check if the received Sequence Counter shall be accepted */
      if (E_OK == CanTSyn_CheckSequenceCounter(SlavePduId, receivedSequenceCounter, TRUE))
      {
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00086,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00087,1 */
        CanTSyn_ProcessRxSyncFrame(T_VLT, DataPtr, SlavePduId);
      }
    }
    break;
#endif /* CANTSYN_RX_SYNC_USED == STD_ON */



#if(CANTSYN_RX_OFFSET_USED == STD_ON)
    case CANTSYN_OFNSMSG_RX_WAIT:
#if(CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON)
    case CANTSYN_OFNSMSG_RX_WAIT_TIMEOUT:
#endif /* (CANTSYN_RX_FOLLOWUP_TIMEOUT_USED == STD_ON) */
    {
      /* check if the Sequence Counter is received with the same value like for the OFS message */
      if(receivedSequenceCounter == CanTSyn_OffsetSlave[OffsetRxPduId].SeqCount)
      {
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00077.OFSMatchOFNS.SC,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00087,1 */
        CanTSyn_ProcessRxOfnsFrame(T_VLT, DataPtr, OffsetRxPduId);
      }
      else
      {
        /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAIT_InvalidSC_OfsRXWAIT,1 */
        /* !LINKSTO CanTSyn.Dsn.OfsOfns.CanTSynRxStatus_OfnsRXWAITTIMEOUT_InvalidSC_OfsRXWAIT,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00077.NoMatchOFS_OFNS.SC,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00114.InvalidSC.OFNSMessage,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00068.InvalidSC.OFNSMessage,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00069.InvalidSC.OFNSMessage,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00070.InvalidSC.OFNSMessage,1 */
        /* ignore the invalid OFNS message (wrong SC) and reset the OFS RxStatus and clear all the variables for a new OFS-OFNS reception */
        CanTSyn_RxOffsetReset(SlavePduId - CANTSYN_NUMBER_OF_SYNC_SLAVES);

      }
    }
    break;

    default:
      /* CANTSYN_OFSMSG_RX_WAIT */
    {
      /* check if the received Sequence Counter shall be accepted */
      if(E_OK == CanTSyn_CheckSequenceCounter(SlavePduId, receivedSequenceCounter, FALSE))
      {
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00130,1 */
        /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00131,1 */
#if(CANTSYN_RX_USE_EXTENDED_MSG == STD_ON)
        if(messageLength == CANTSYN_EXTENDED_MSG_SIZE)
        {
          CanTSyn_ProcessExtRxOfsFrame(T_VLT, DataPtr, OffsetRxPduId);
        }
        else
#endif
        {
          /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00086,1 */
          /* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00087,1 */
          CanTSyn_ProcessRxOfsFrame(DataPtr, OffsetRxPduId);
        }
      }
    }
    break;
#endif /* (CANTSYN_RX_OFFSET_USED == STD_ON) */
  }


  DBG_CANTSYN_PROCESSRXFRAME_EXIT(SlavePduId, RxStatus, DataPtr, T_VLT, messageLength);
}
#endif /* ((CANTSYN_RX_SYNC_USED == STD_ON) || (CANTSYN_RX_OFFSET_USED == STD_ON)) */




#define CANTSYN_STOP_SEC_CODE
#include <CanTSyn_MemMap.h>


