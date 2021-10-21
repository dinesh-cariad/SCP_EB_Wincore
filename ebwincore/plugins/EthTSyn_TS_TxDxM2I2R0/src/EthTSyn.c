/**
 * \file
 *
 * \brief AUTOSAR EthTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module EthTSyn.
 *
 * \version 2.2.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 *  Misra-C:2012 Deviations:
 *
 *  MISRAC2012-1) Deviated Rule: 8.13 (Advisory)
 *    A pointer should point to a const-qualified type whenever possible.
 *
 *    Reason:
 *    - The API parameter is specified to be pointer to variable by AUTOSAR specification.
 *    - The API parameter is required to be pointer to variable for at least one possible
 *      configuration.
 *
 *  MISRAC2012-2) Deviated Rule: 8.9 (Advisory)
 *    An object should be defined at block scope if its identifier only appears in a single
 *    function.
 *
 *    Reason:
 *    - The AUTOSAR memory mapping requires that functions are mapped in SEC_CODE memory sections.
 *    - Objects at block scope require a different memory mapping, e.g. to a SEC_VAR section,
 *    which leads to nested memory sections, which is not supported by some compilers.
 *
 *  MISRAC2012-3) Deviated Rule 8.4 (Required)
 *    A compatible declaration shall be visible when an object or function
 *    with external linkage is defined.
 *
 *    Reason:
 *    - The symbol isn't meant to be normally exposed. It's used only during
 *      testing.
 */


/*==================[inclusions]============================================*/

#include <TSAutosar.h> /* Specific standard types */
#include <ComStack_Types.h> /* Communication stack types */
#include <EthTSyn.h> /* Module public API */
#include <EthTSyn_Cfg.h> /* Module configuration */
#include <EthTSyn_Int.h> /* Module internal header file */
#include <EthTSyn_Int01_Lib.h> /* Module internal header file */
#if(ETHTSYN_SWITCH_USED == STD_ON)
#include <EthTSyn_Int02_Swt.h> /* Module internal header file used for switch/bridge */
#endif
#include <EthTSyn_Types.h> /* Module specific types */
#include <EthTSyn_Trace.h> /* Debug and trace */
#include <EthTSyn_Version.h> /* Module version declarations */
#include <EthIf.h> /* Module public API */
#include <StbM.h> /* Module public API */
#include <TSMem.h> /* TS_MemCpy, ... */
#include <TSAtomic_Assign.h> /* Atomic assignment macros */

/* !LINKSTO EthTSyn.EB.SwitchConfigured, 1 */
#if(ETHTSYN_SWITCH_USED == STD_ON)
#include <EthSwt.h> /* Module public API */
#endif

#include <SchM_EthTSyn.h> /* SchM interface header */

/* if any DEM event is switched on, include Dem.h */
#if ((ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED == TS_PROD_ERR_REP_TO_DEM) || \
     (ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED == TS_PROD_ERR_REP_TO_DEM) || \
     (ETHTSYN_PROD_ERR_HANDLING_UNEXPECTED_SYNC == TS_PROD_ERR_REP_TO_DEM))
#include <Dem.h>
#endif

#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* DET support */
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */

#if((ETHTSYN_TX_CRC_USED == STD_ON) || (ETHTSYN_RX_CRC_USED == STD_ON))
#include <Crc.h> /* CRC support */
#endif/* (ETHTSYN_TX_CRC_USED == STD_ON) || (ETHTSYN_RX_CRC_USED == STD_ON) */

#if(ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT == STD_ON)
#include <DevAuth.h>
#endif

#if(ETHTSYN_TIME_RECORDING == STD_ON)
#include <StbM_EthTSyn.h> /* Module public API */
#endif
/*==================[macros]================================================*/

#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
#if (defined ETHTSYN_DET_REPORT_ERROR)
#error ETHTSYN_DET_REPORT_ERROR is already defined
#endif
#define ETHTSYN_DET_REPORT_ERROR(ApiId, ErrorId) \
  (void) Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, (ApiId), (ErrorId))
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */

#if (defined ETHTSYN_PTP_FRAMETYPE)
#error ETHTSYN_PTP_FRAMETYPE is already defined
#endif
/* All EthTSyn frames use the PTP frame type (See 802.1AS-2011 Ch 11.3.5). */
#define ETHTSYN_PTP_FRAMETYPE (Eth_FrameType) 0x88F7U

#if (defined ETHTSYN_MAC_ADD_SIZE)
#error ETHTSYN_MAC_ADD_SIZE is already defined
#endif
#define ETHTSYN_MAC_ADD_SIZE 6U /* Size of MAC address in bytes. */

#if (defined ETHTSYN_TIME_STAMP_SIZE)
#error ETHTSYN_TIME_STAMP_SIZE is already defined
#endif
#define ETHTSYN_TIME_STAMP_SIZE 10U /* Size of time stamp in bytes. */

#if (defined ETHTSYN_SEQ_NUM_MAX)
#error ETHTSYN_SEQ_NUM_MAX is already defined
#endif
#define ETHTSYN_SEQ_NUM_MAX 0xFFFFU /* Maximum of the 16 bit sequence number. */

#if (defined ETHTSYN_INV_BUFIDX)
#error ETHTSYN_INV_BUFIDX is already defined
#endif
/* Invalid buffer index */
/* BufIdx is Eth_BufIdxType -> used uint32 with maximum for invalid value. */
#define ETHTSYN_INV_BUFIDX 0xFFFFFFFFU

#if (defined ETHTSYN_UNINIT)
#error ETHTSYN_UNINIT is already defined
#endif
/* Check if module is initialized properly */
#define ETHTSYN_UNINIT (EthTSyn_RootCfgPtr == NULL_PTR)

#if (defined ETHTSYN_INV_DEBOUNCE_VALUE)
#error ETHTSYN_INV_DEBOUNCE_VALUE is already defined
#endif
/* Invalid debounce value */
/* DebounceTime is uint32 with maximum for invalid value. */
#define ETHTSYN_INV_DEBOUNCE_VALUE 0xFFFFFFFFU

#if (defined ETHTSYN_SC_MAXLIMIT)
#error ETHTSYN_SC_MAXLIMIT is already defined
#endif
/* GlobalTimeSequenceCounterJumpWidth is limited by uint16 max value. */
#define ETHTSYN_SC_MAXLIMIT 0xFFFFU

#if (defined ETHTSYN_STBM_TIMEOUT) /* to prevent double declaration */
#error ETHTSYN_STBM_TIMEOUT is already defined
#endif /* if (defined ETHTSYN_STBM_TIMEOUT) */
#define ETHTSYN_STBM_TIMEOUT  (1U)

/*====== Pdelay calculation ======*/

#if (defined ETHTSYN_PTP_HEADER_SIZE)
#error ETHTSYN_PTP_HEADER_SIZE is already defined
#endif
#define ETHTSYN_PTP_HEADER_SIZE 34U /* PTP header size is 34 bytes. */

#if (defined ETHTSYN_FUP_TLV_SIZE)
#error ETHTSYN_FUP_TLV_SIZE is already defined
#endif
#define ETHTSYN_FUP_TLV_SIZE 32U /* Follow up TLV size is 32 bytes. */

#if (defined ETHTSYN_RESERVED_10BYTES)
#error ETHTSYN_RESERVED_10BYTES is already defined
#endif
#define ETHTSYN_RESERVED_10BYTES 10U /* 10 bytes reserved space. */

/* ====== PTP header values ====== */
#if (defined ETHTSYN_PTP_HEADER_TRANS_SPEC_UN)
#error ETHTSYN_PTP_HEADER_TRANS_SPEC_UN is already defined
#endif
/* Transport specific upper nibble value (for or conjunction) (See 802.1AS-2011 Ch 10.5.2.2.1) */
#define ETHTSYN_PTP_HEADER_TRANS_SPEC_UN 0x10U

/* Message type (See 802.1AS-2011 Ch 11.4.2.1) */
/* !LINKSTO EthTSyn.ASR151.PRS_TS_00028, 1 */
#if (defined ETHTSYN_PTP_HEADER_MSGTYPE_SYNC)
#error ETHTSYN_PTP_HEADER_MSGTYPE_SYNC is already defined
#endif
#define ETHTSYN_PTP_HEADER_MSGTYPE_SYNC 0x00U

#if (defined ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_REQ)
#error ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_REQ is already defined
#endif
#define ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_REQ 0x02U

#if (defined ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_RESP)
#error ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_RESP is already defined
#endif
#define ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_RESP 0x03U

#if (defined ETHTSYN_PTP_HEADER_MSGTYPE_FUP)
#error ETHTSYN_PTP_HEADER_MSGTYPE_FUP is already defined
#endif
#define ETHTSYN_PTP_HEADER_MSGTYPE_FUP 0x08U

#if (defined ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_FUP)
#error ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_FUP is already defined
#endif
#define ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_FUP 0x0AU

#if (defined ETHTSYN_PTP_HEADER_MSGTYPE_ANNOUNCE)
#error ETHTSYN_PTP_HEADER_MSGTYPE_ANNOUNCE is already defined
#endif
#define ETHTSYN_PTP_HEADER_MSGTYPE_ANNOUNCE 0x0BU

#if (defined ETHTSYN_PTP_HEADER_VERSION)
#error ETHTSYN_PTP_HEADER_VERSION is already defined
#endif
#define ETHTSYN_PTP_HEADER_VERSION 0x02U /* PTP version (See 802.1AS-2011 Ch 10.5.2.2.3) */

#if (defined ETHTSYN_PTP_HEADER_DOMAIN)
#error ETHTSYN_PTP_HEADER_DOMAIN is already defined
#endif
#define ETHTSYN_PTP_HEADER_DOMAIN 0x00U /* Domain number (See 802.1AS-2011 Ch 8.1) */

#if (defined ETHTSYN_PTP_HEADER_RESERVED)
#error ETHTSYN_PTP_HEADER_RESERVED is already defined
#endif
#define ETHTSYN_PTP_HEADER_RESERVED 0x00U

#if (defined ETHTSYN_PTP_HEADER_FLAG_TWOSTEP)
#error ETHTSYN_PTP_HEADER_FLAG_TWOSTEP is already defined
#endif
#define ETHTSYN_PTP_HEADER_FLAG_TWOSTEP 0x0002U /* Set bit 1 (See 802.1AS-2011 Ch 11.4.2.3) */

#if (defined ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE)
#error ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE is already defined
#endif
#define ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE 0x0800U /* Set bit (See 802.1AS-2011 Ch 10.5.2.2.6) */

#if (defined ETHTSYN_PTP_HEADER_NOFLAGS)
#error ETHTSYN_PTP_HEADER_NOFLAGS is already defined
#endif
#define ETHTSYN_PTP_HEADER_NOFLAGS 0x0000U

/* EUI-48 defined bytes of the source clock identity. */
#if (defined ETHTSYN_PTP_HEADER_EUI48_IDX3_SRCPORTIDENTITY)
#error ETHTSYN_PTP_HEADER_EUI48_IDX3_SRCPORTIDENTITY is already defined
#endif
#define ETHTSYN_PTP_HEADER_EUI48_IDX3_SRCPORTIDENTITY 0xFFU

#if (defined ETHTSYN_PTP_HEADER_EUI48_IDX4_SRCPORTIDENTITY)
#error ETHTSYN_PTP_HEADER_EUI48_IDX4_SRCPORTIDENTITY is already defined
#endif
#define ETHTSYN_PTP_HEADER_EUI48_IDX4_SRCPORTIDENTITY 0xFEU

#if (defined ETHTSYN_PTP_HEADER_LOGMSGINTERVAL_RESERVED)
#error ETHTSYN_PTP_HEADER_LOGMSGINTERVAL_RESERVED is already defined
#endif
/* Log message interval reserved value for Pdelay_Resp and Pdelay_Resp_Follow_Up
   (See 802.1AS-2011 Ch 11.4.2.8) */
#define ETHTSYN_PTP_HEADER_LOGMSGINTERVAL_RESERVED 0x7FU

/* Control field (See 802.1AS-2011 Ch 11.4.2.7) */
#if (defined ETHTSYN_PTP_HEADER_CONTROL_SYNC)
#error ETHTSYN_PTP_HEADER_CONTROL_SYNC is already defined
#endif
#define ETHTSYN_PTP_HEADER_CONTROL_SYNC 0x00U

#if (defined ETHTSYN_PTP_HEADER_CONTROL_FUP)
#error ETHTSYN_PTP_HEADER_CONTROL_FUP is already defined
#endif
#define ETHTSYN_PTP_HEADER_CONTROL_FUP 0x02U

#if (defined ETHTSYN_PTP_HEADER_CONTROL_PDELAY)
#error ETHTSYN_PTP_HEADER_CONTROL_PDELAY is already defined
#endif
#define ETHTSYN_PTP_HEADER_CONTROL_PDELAY 0x05U /* All Pdealy frames */

#if (defined ETHTSYN_PTP_HEADER_CONTROL_ANNOUNCE)
#error ETHTSYN_PTP_HEADER_CONTROL_ANNOUNCE is already defined
#endif
#define ETHTSYN_PTP_HEADER_CONTROL_ANNOUNCE 0x05U /* (See 802.1AS-2011 Ch 10.5.2.2.10) */


/* ====== Follow Up TLV values ====== */

#if (defined ETHTSYN_FUP_TLV_TYPE_1)
#error ETHTSYN_FUP_TLV_TYPE_1 is already defined
#endif
#define ETHTSYN_FUP_TLV_TYPE_1 0x00U /* TLV type (See 802.1AS-2011 Ch 11.4.4.3.2) */

#if (defined ETHTSYN_FUP_TLV_TYPE_0)
#error ETHTSYN_FUP_TLV_TYPE_0 is already defined
#endif
#define ETHTSYN_FUP_TLV_TYPE_0 0x03U

#if (defined ETHTSYN_FUP_TLV_LENGTH_1)
#error ETHTSYN_FUP_TLV_LENGTH_1 is already defined
#endif
#define ETHTSYN_FUP_TLV_LENGTH_1 0U /* Length (See 802.1AS-2011 Ch 11.4.4.3.3) */

#if (defined ETHTSYN_FUP_TLV_LENGTH_0)
#error ETHTSYN_FUP_TLV_LENGTH_0 is already defined
#endif
/* Value is strange since TLV is 32 bytes. Maybe the bytes after length field are counted. */
#define ETHTSYN_FUP_TLV_LENGTH_0 28U

#if (defined ETHTSYN_FUP_TLV_ORGID_2)
#error ETHTSYN_FUP_TLV_ORGID_2 is already defined
#endif
#define ETHTSYN_FUP_TLV_ORGID_2 0x00U /* Organization ID (See 802.1AS-2011 Ch 11.4.4.3.4) */

#if (defined ETHTSYN_FUP_TLV_ORGID_1)
#error ETHTSYN_FUP_TLV_ORGID_1 is already defined
#endif
#define ETHTSYN_FUP_TLV_ORGID_1 0x80U

#if (defined ETHTSYN_FUP_TLV_ORGID_0)
#error ETHTSYN_FUP_TLV_ORGID_0 is already defined
#endif
#define ETHTSYN_FUP_TLV_ORGID_0 0xC2U

/* Organization sub type (See 802.1AS-2011 Ch 11.4.4.3.5) */
#if (defined ETHTSYN_FUP_TLV_ORG_SUBTYPE_2)
#error ETHTSYN_FUP_TLV_ORG_SUBTYPE_2 is already defined
#endif
#define ETHTSYN_FUP_TLV_ORG_SUBTYPE_2 0U

#if (defined ETHTSYN_FUP_TLV_ORG_SUBTYPE_1)
#error ETHTSYN_FUP_TLV_ORG_SUBTYPE_1 is already defined
#endif
#define ETHTSYN_FUP_TLV_ORG_SUBTYPE_1 0U

#if (defined ETHTSYN_FUP_TLV_ORG_SUBTYPE_0)
#error ETHTSYN_FUP_TLV_ORG_SUBTYPE_0 is already defined
#endif
#define ETHTSYN_FUP_TLV_ORG_SUBTYPE_0 1U

/* Grand master time base indicator */
#if (defined ETHTSYN_FUP_TLV_GM_TIMEBASE_IND_1)
#error ETHTSYN_FUP_TLV_GM_TIMEBASE_IND_1 is already defined
#endif
#define ETHTSYN_FUP_TLV_GM_TIMEBASE_IND_1 0U

#if (defined ETHTSYN_FUP_TLV_GM_TIMEBASE_IND_0)
#error ETHTSYN_FUP_TLV_GM_TIMEBASE_IND_0 is already defined
#endif
#define ETHTSYN_FUP_TLV_GM_TIMEBASE_IND_0 0U /* Wireshark from Extreme Switch */

/* Byte masks */
#if (defined ETHTSYN_32BIT_MASK0_LSB)
#error ETHTSYN_32BIT_MASK0_LSB is already defined
#endif
#define ETHTSYN_32BIT_MASK0_LSB (uint32) 0x000000FFU

#if (defined ETHTSYN_32BIT_MASK1)
#error ETHTSYN_32BIT_MASK1 is already defined
#endif
#define ETHTSYN_32BIT_MASK1 (uint32) 0x0000FF00U

#if (defined ETHTSYN_32BIT_MASK2)
#error ETHTSYN_32BIT_MASK2 is already defined
#endif
#define ETHTSYN_32BIT_MASK2 (uint32) 0x00FF0000U

#if (defined ETHTSYN_32BIT_MASK3_MSB)
#error ETHTSYN_32BIT_MASK3_MSB is already defined
#endif
#define ETHTSYN_32BIT_MASK3_MSB (uint32) 0xFF000000U

#if (defined ETHTSYN_16BIT_MASK_LSB)
#error ETHTSYN_16BIT_MASK_LSB is already defined
#endif
#define ETHTSYN_16BIT_MASK_LSB (uint16) 0x00FFU

#if (defined ETHTSYN_16BIT_MASK_MSB)
#error ETHTSYN_16BIT_MASK_MSB is already defined
#endif
#define ETHTSYN_16BIT_MASK_MSB (uint16) 0xFF00U

#if (defined ETHTSYN_8BIT_MASK_LN)
#error ETHTSYN_8BIT_MASK_LN is already defined
#endif
#define ETHTSYN_8BIT_MASK_LN (uint8) 0x0FU /* Mask for lower nibble. */

#if (defined ETHTSYN_8BIT_MASK_UN)
#error ETHTSYN_8BIT_MASK_UN is already defined
#endif
#define ETHTSYN_8BIT_MASK_UN (uint8) 0xF0U /* Mask for upper nibble. */

/* Bit masks */
#if (defined ETHTSYN_32BIT_MASK_BIT31)
#error ETHTSYN_32BIT_MASK_BIT31 is already defined
#endif
#define ETHTSYN_32BIT_MASK_BIT31 (uint32) 0x80000000U /* Mask to get bit 31 (MSB) of 32 bit var.*/

#if (defined ETHTSYN_16BIT_MASK_BIT15)
#error ETHTSYN_16BIT_MASK_BIT15 is already defined
#endif
#define ETHTSYN_16BIT_MASK_BIT15 (uint16) 0x8000U /* Mask to get bit 15 (MSB) of 16 bit var.*/

/* ====== PTP frame payload sizes ====== */
#if (defined ETHTSYN_PTP_SYNC_PAYLOADSIZE)
#error ETHTSYN_PTP_SYNC_PAYLOADSIZE is already defined
#endif
#define ETHTSYN_PTP_SYNC_PAYLOADSIZE 10U /* (See 802.1AS-2011 Ch 11.4.3) */

#if (defined ETHTSYN_PTP_SYNC_FUP_PAYLOADSIZE)
#error ETHTSYN_PTP_SYNC_FUP_PAYLOADSIZE is already defined
#endif
#define ETHTSYN_PTP_SYNC_FUP_PAYLOADSIZE 42U /* (See 802.1AS-2011 Ch 11.4.4.1) */

#if (defined ETHTSYN_PTP_PDELAY_REQ_PAYLOADSIZE)
#error ETHTSYN_PTP_PDELAY_REQ_PAYLOADSIZE is already defined
#endif
#define ETHTSYN_PTP_PDELAY_REQ_PAYLOADSIZE 20U /* (See 802.1AS-2011 Ch 11.4.5) */

#if (defined ETHTSYN_PTP_PDELAY_RESP_PAYLOADSIZE)
#error ETHTSYN_PTP_PDELAY_RESP_PAYLOADSIZE is already defined
#endif
#define ETHTSYN_PTP_PDELAY_RESP_PAYLOADSIZE 20U /* (See 802.1AS-2011 Ch 11.4.6.1) */

#if (defined ETHTSYN_PTP_PDELAY_RESP_FUP_PAYLOADSIZE)
#error ETHTSYN_PTP_PDELAY_RESP_FUP_PAYLOADSIZE is already defined
#endif
#define ETHTSYN_PTP_PDELAY_RESP_FUP_PAYLOADSIZE 20U /* (See 802.1AS-2011 Ch 11.4.7.1) */

#if (defined ETHTSYN_PTP_ANNOUNCE_PAYLOADSIZE)
#error ETHTSYN_PTP_ANNOUNCE_PAYLOADSIZE is already defined
#endif
#define ETHTSYN_PTP_ANNOUNCE_PAYLOADSIZE 42U /* (See 802.1AS-2011 Ch 10.5.3.1) (N = 1) */

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_SIZE)
#error ETHTSYN_AUTH_CHALLENGE_TLV_SIZE is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_SIZE 14U

/* ====== AUTH_CHALLENGE Tlv byte numbers ====== */
#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_B0_TYPE)
#error ETHTSYN_AUTH_CHALLENGE_TLV_B0_TYPE is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_B0_TYPE 0U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_B1_TYPE)
#error ETHTSYN_AUTH_CHALLENGE_TLV_B1_TYPE is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_B1_TYPE 1U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_B2_LENGTH)
#error ETHTSYN_AUTH_CHALLENGE_TLV_B2_LENGTH is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_B2_LENGTH 2U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_B3_LENGTH)
#error ETHTSYN_AUTH_CHALLENGE_TLV_B3_LENGTH is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_B3_LENGTH 3U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_B4_CHLTYPE)
#error ETHTSYN_AUTH_CHALLENGE_TLV_B4_CHLTYPE is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_B4_CHLTYPE 4U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_B5_RESERVED)
#error ETHTSYN_AUTH_CHALLENGE_TLV_B5_RESERVED is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_B5_RESERVED 5U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_B6_REQNONCE)
#error ETHTSYN_AUTH_CHALLENGE_TLV_B6_REQNONCE is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_B6_REQNONCE 6U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_B7_REQNONCE)
#error ETHTSYN_AUTH_CHALLENGE_TLV_B7_REQNONCE is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_B7_REQNONCE 7U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_B8_REQNONCE)
#error ETHTSYN_AUTH_CHALLENGE_TLV_B8_REQNONCE is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_B8_REQNONCE 8U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_B9_REQNONCE)
#error ETHTSYN_AUTH_CHALLENGE_TLV_B9_REQNONCE is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_B9_REQNONCE 9U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_B10_RESPNONCE)
#error ETHTSYN_AUTH_CHALLENGE_TLV_B10_RESPNONCE is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_B10_RESPNONCE 10U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_B11_RESPNONCE)
#error ETHTSYN_AUTH_CHALLENGE_TLV_B11_RESPNONCE is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_B11_RESPNONCE 11U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_B12_RESPNONCE)
#error ETHTSYN_AUTH_CHALLENGE_TLV_B12_RESPNONCE is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_B12_RESPNONCE 12U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_B13_RESPNONCE)
#error ETHTSYN_AUTH_CHALLENGE_TLV_B9_REQNONCE is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_B13_RESPNONCE 13U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_TYPE_0)
#error ETHTSYN_AUTH_CHALLENGE_TLV_TYPE_0 is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_TYPE_0 0x20U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_TYPE_1)
#error ETHTSYN_AUTH_CHALLENGE_TLV_TYPE_1 is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_TYPE_1 0x01U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_LENGTH_0)
#error ETHTSYN_AUTH_CHALLENGE_TLV_LENGTH_0 is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_LENGTH_0 0x00U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_LENGTH_1)
#error ETHTSYN_AUTH_CHALLENGE_TLV_LENGTH_1 is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_LENGTH_1 0x0EU

#if (defined ETHTSYN_AUTH_CHALLENGE_RESPONSE_TYPE)
#error ETHTSYN_AUTH_CHALLENGE_RESPONSE_TYPE is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_RESPONSE_TYPE 0x02U

#if (defined ETHTSYN_AUTH_CHALLENGE_TLV_RESERVED)
#error ETHTSYN_AUTH_CHALLENGE_TLV_RESERVED is already defined
#endif
#define ETHTSYN_AUTH_CHALLENGE_TLV_RESERVED 0x00U

/* ====== PTP header byte numbers ====== */
#if (defined ETHTSYN_PTP_HEADER_B0_LN_MSGTYPE)
#error ETHTSYN_PTP_HEADER_B0_LN_MSGTYPE is already defined
#endif
#define ETHTSYN_PTP_HEADER_B0_LN_MSGTYPE 0U /* Message type (lower nibble) */

#if (defined ETHTSYN_PTP_HEADER_B0_UN_TRANS_SPEC)
#error ETHTSYN_PTP_HEADER_B0_UN_TRANS_SPEC is already defined
#endif
#define ETHTSYN_PTP_HEADER_B0_UN_TRANS_SPEC 0U /* Transport specific (upper nibble) */

#if (defined ETHTSYN_PTP_HEADER_B1_PTPVERSION)
#error ETHTSYN_PTP_HEADER_B1_PTPVERSION is already defined
#endif
#define ETHTSYN_PTP_HEADER_B1_PTPVERSION 1U /* PTP version */

#if (defined ETHTSYN_PTP_HEADER_B2_MSGLENGTH_1)
#error ETHTSYN_PTP_HEADER_B2_MSGLENGTH_1 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B2_MSGLENGTH_1 2U /* 16 bit message length */

#if (defined ETHTSYN_PTP_HEADER_B3_MSGLENGTH_0)
#error ETHTSYN_PTP_HEADER_B3_MSGLENGTH_0 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B3_MSGLENGTH_0 3U

#if (defined ETHTSYN_PTP_HEADER_B4_DOMAIN)
#error ETHTSYN_PTP_HEADER_B4_DOMAIN is already defined
#endif
#define ETHTSYN_PTP_HEADER_B4_DOMAIN 4U /* Domain number */

#if (defined ETHTSYN_PTP_HEADER_B5_RESERVED)
#error ETHTSYN_PTP_HEADER_B5_RESERVED is already defined
#endif
#define ETHTSYN_PTP_HEADER_B5_RESERVED 5U

#if (defined ETHTSYN_PTP_HEADER_B6_FLAGS_0)
#error ETHTSYN_PTP_HEADER_B6_FLAGS_0 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B6_FLAGS_0 6U /* 16 bit flags */

#if (defined ETHTSYN_PTP_HEADER_B7_FLAGS_1)
#error ETHTSYN_PTP_HEADER_B7_FLAGS_1 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B7_FLAGS_1 7U

#if (defined ETHTSYN_PTP_HEADER_B8_CORRFIELD_0)
#error ETHTSYN_PTP_HEADER_B8_CORRFIELD_0 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B8_CORRFIELD_0 8U /* 8 byte correction field */

#if (defined ETHTSYN_PTP_HEADER_B9_CORRFIELD_1)
#error ETHTSYN_PTP_HEADER_B9_CORRFIELD_1 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B9_CORRFIELD_1 9U

#if (defined ETHTSYN_PTP_HEADER_B10_CORRFIELD_2)
#error ETHTSYN_PTP_HEADER_B10_CORRFIELD_2 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B10_CORRFIELD_2 10U

#if (defined ETHTSYN_PTP_HEADER_B11_CORRFIELD_3)
#error ETHTSYN_PTP_HEADER_B11_CORRFIELD_3 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B11_CORRFIELD_3 11U

#if (defined ETHTSYN_PTP_HEADER_B12_CORRFIELD_4)
#error ETHTSYN_PTP_HEADER_B12_CORRFIELD_4 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B12_CORRFIELD_4 12U

#if (defined ETHTSYN_PTP_HEADER_B13_CORRFIELD_5)
#error ETHTSYN_PTP_HEADER_B13_CORRFIELD_5 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B13_CORRFIELD_5 13U

#if (defined ETHTSYN_PTP_HEADER_B14_CORRFIELD_6)
#error ETHTSYN_PTP_HEADER_B14_CORRFIELD_6 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B14_CORRFIELD_6 14U

#if (defined ETHTSYN_PTP_HEADER_B15_CORRFIELD_7)
#error ETHTSYN_PTP_HEADER_B15_CORRFIELD_7 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B15_CORRFIELD_7 15U

#if (defined ETHTSYN_PTP_HEADER_B16_RESERVED_0)
#error ETHTSYN_PTP_HEADER_B16_RESERVED_0 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B16_RESERVED_0 16U

#if (defined ETHTSYN_PTP_HEADER_B17_RESERVED_1)
#error ETHTSYN_PTP_HEADER_B17_RESERVED_1 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B17_RESERVED_1 17U

#if (defined ETHTSYN_PTP_HEADER_B18_RESERVED_2)
#error ETHTSYN_PTP_HEADER_B18_RESERVED_2 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B18_RESERVED_2 18U

#if (defined ETHTSYN_PTP_HEADER_B19_RESERVED_3)
#error ETHTSYN_PTP_HEADER_B19_RESERVED_3 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B19_RESERVED_3 19U

#if (defined ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0)
#error ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0 20U /* 10 byte source port identity */

#if (defined ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1)
#error ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1 21U

#if (defined ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2)
#error ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2 22U

#if (defined ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3)
#error ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3 23U

#if (defined ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4)
#error ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4 24U

#if (defined ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5)
#error ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5 25U

#if (defined ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6)
#error ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6 26U

#if (defined ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7)
#error ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7 27U

#if (defined ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8)
#error ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8 28U

#if (defined ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9)
#error ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9 29U

#if (defined ETHTSYN_PTP_HEADER_B30_SEQUNUMBER_1)
#error ETHTSYN_PTP_HEADER_B30_SEQUNUMBER_1 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B30_SEQUNUMBER_1 30U /* 16 bit sequence number */

#if (defined ETHTSYN_PTP_HEADER_B31_SEQUNUMBER_0)
#error ETHTSYN_PTP_HEADER_B31_SEQUNUMBER_0 is already defined
#endif
#define ETHTSYN_PTP_HEADER_B31_SEQUNUMBER_0 31U

#if (defined ETHTSYN_PTP_HEADER_B32_CONTROL)
#error ETHTSYN_PTP_HEADER_B32_CONTROL is already defined
#endif
#define ETHTSYN_PTP_HEADER_B32_CONTROL 32U /* Control field. */

#if (defined ETHTSYN_PTP_HEADER_B33_LOGMSGINTERVAL)
#error ETHTSYN_PTP_HEADER_B33_LOGMSGINTERVAL is already defined
#endif
#define ETHTSYN_PTP_HEADER_B33_LOGMSGINTERVAL 33U /* Log message interval (till next Tx frame) */

/* ====== Follow up TLV ====== */
#if (defined ETHTSYN_FUP_TLV_B0_TYPE_1)
#error ETHTSYN_FUP_TLV_B0_TYPE_1 is already defined
#endif
#define ETHTSYN_FUP_TLV_B0_TYPE_1 0U /* 16 bit TLV Type */

#if (defined ETHTSYN_FUP_TLV_B1_TYPE_0)
#error ETHTSYN_FUP_TLV_B1_TYPE_0 is already defined
#endif
#define ETHTSYN_FUP_TLV_B1_TYPE_0 1U /* PTP version */

#if (defined ETHTSYN_FUP_TLV_B2_LENGTH_1)
#error ETHTSYN_FUP_TLV_B2_LENGTH_1 is already defined
#endif
#define ETHTSYN_FUP_TLV_B2_LENGTH_1 2U /* 16 bit TLV length */

#if (defined ETHTSYN_FUP_TLV_B3_LENGTH_0)
#error ETHTSYN_FUP_TLV_B3_LENGTH_0 is already defined
#endif
#define ETHTSYN_FUP_TLV_B3_LENGTH_0 3U

#if (defined ETHTSYN_FUP_TLV_B4_ORGID_2)
#error ETHTSYN_FUP_TLV_B4_ORGID_2 is already defined
#endif
#define ETHTSYN_FUP_TLV_B4_ORGID_2 4U /* 3 bytes organization ID */

#if (defined ETHTSYN_FUP_TLV_B5_ORGID_1)
#error ETHTSYN_FUP_TLV_B5_ORGID_1 is already defined
#endif
#define ETHTSYN_FUP_TLV_B5_ORGID_1 5U

#if (defined ETHTSYN_FUP_TLV_B6_ORGID_0)
#error ETHTSYN_FUP_TLV_B6_ORGID_0 is already defined
#endif
#define ETHTSYN_FUP_TLV_B6_ORGID_0 6U

#if (defined ETHTSYN_FUP_TLV_B7_ORG_SUBTYPE_2)
#error ETHTSYN_FUP_TLV_B7_ORG_SUBTYPE_2 is already defined
#endif
#define ETHTSYN_FUP_TLV_B7_ORG_SUBTYPE_2 7U /* 24 bit organization sub type. */

#if (defined ETHTSYN_FUP_TLV_B8_ORG_SUBTYPE_1)
#error ETHTSYN_FUP_TLV_B8_ORG_SUBTYPE_1 is already defined
#endif
#define ETHTSYN_FUP_TLV_B8_ORG_SUBTYPE_1 8U

#if (defined ETHTSYN_FUP_TLV_B9_ORG_SUBTYPE_0)
#error ETHTSYN_FUP_TLV_B9_ORG_SUBTYPE_0 is already defined
#endif
#define ETHTSYN_FUP_TLV_B9_ORG_SUBTYPE_0 9U

#if (defined ETHTSYN_FUP_TLV_B10_CSRO_3)
#error ETHTSYN_FUP_TLV_B10_CSRO_3 is already defined
#endif
#define ETHTSYN_FUP_TLV_B10_CSRO_3 10U /* 32 bit cumulativeScaledRateOffset */

#if (defined ETHTSYN_FUP_TLV_B11_CSRO_2)
#error ETHTSYN_FUP_TLV_B11_CSRO_2 is already defined
#endif
#define ETHTSYN_FUP_TLV_B11_CSRO_2 11U

#if (defined ETHTSYN_FUP_TLV_B12_CSRO_1)
#error ETHTSYN_FUP_TLV_B12_CSRO_1 is already defined
#endif
#define ETHTSYN_FUP_TLV_B12_CSRO_1 12U

#if (defined ETHTSYN_FUP_TLV_B13_CSRO_0)
#error ETHTSYN_FUP_TLV_B13_CSRO_0 is already defined
#endif
#define ETHTSYN_FUP_TLV_B13_CSRO_0 13U

#if (defined ETHTSYN_FUP_TLV_B14_GM_TIMEBASEIND_1)
#error ETHTSYN_FUP_TLV_B14_GM_TIMEBASEIND_1 is already defined
#endif
#define ETHTSYN_FUP_TLV_B14_GM_TIMEBASEIND_1 14U /* 16 bit grand master time base indicator */

#if (defined ETHTSYN_FUP_TLV_B15_GM_TIMEBASEIND_0)
#error ETHTSYN_FUP_TLV_B15_GM_TIMEBASEIND_0 is already defined
#endif
#define ETHTSYN_FUP_TLV_B15_GM_TIMEBASEIND_0 15U

#if (defined ETHTSYN_FUP_TLV_B16_GM_PH_CHG_0)
#error ETHTSYN_FUP_TLV_B16_GM_PH_CHG_0 is already defined
#endif
#define ETHTSYN_FUP_TLV_B16_GM_PH_CHG_0 16U /* 12 bytes  last grand master phase change */

#if (defined ETHTSYN_FUP_TLV_B17_GM_PH_CHG_1)
#error ETHTSYN_FUP_TLV_B17_GM_PH_CHG_1 is already defined
#endif
#define ETHTSYN_FUP_TLV_B17_GM_PH_CHG_1 17U

#if (defined ETHTSYN_FUP_TLV_B18_GM_PH_CHG_2)
#error ETHTSYN_FUP_TLV_B18_GM_PH_CHG_2 is already defined
#endif
#define ETHTSYN_FUP_TLV_B18_GM_PH_CHG_2 18U

#if (defined ETHTSYN_FUP_TLV_B19_GM_PH_CHG_3)
#error ETHTSYN_FUP_TLV_B19_GM_PH_CHG_3 is already defined
#endif
#define ETHTSYN_FUP_TLV_B19_GM_PH_CHG_3 19U

#if (defined ETHTSYN_FUP_TLV_B20_GM_PH_CHG_4)
#error ETHTSYN_FUP_TLV_B20_GM_PH_CHG_4 is already defined
#endif
#define ETHTSYN_FUP_TLV_B20_GM_PH_CHG_4 20U

#if (defined ETHTSYN_FUP_TLV_B21_GM_PH_CHG_5)
#error ETHTSYN_FUP_TLV_B21_GM_PH_CHG_5 is already defined
#endif
#define ETHTSYN_FUP_TLV_B21_GM_PH_CHG_5 21U

#if (defined ETHTSYN_FUP_TLV_B22_GM_PH_CHG_6)
#error ETHTSYN_FUP_TLV_B22_GM_PH_CHG_6 is already defined
#endif
#define ETHTSYN_FUP_TLV_B22_GM_PH_CHG_6 22U

#if (defined ETHTSYN_FUP_TLV_B23_GM_PH_CHG_7)
#error ETHTSYN_FUP_TLV_B23_GM_PH_CHG_7 is already defined
#endif
#define ETHTSYN_FUP_TLV_B23_GM_PH_CHG_7 23U

#if (defined ETHTSYN_FUP_TLV_B24_GM_PH_CHG_8)
#error ETHTSYN_FUP_TLV_B24_GM_PH_CHG_8 is already defined
#endif
#define ETHTSYN_FUP_TLV_B24_GM_PH_CHG_8 24U

#if (defined ETHTSYN_FUP_TLV_B25_GM_PH_CHG_9)
#error ETHTSYN_FUP_TLV_B25_GM_PH_CHG_9 is already defined
#endif
#define ETHTSYN_FUP_TLV_B25_GM_PH_CHG_9 25U

#if (defined ETHTSYN_FUP_TLV_B26_GM_PH_CHG_10)
#error ETHTSYN_FUP_TLV_B26_GM_PH_CHG_10 is already defined
#endif
#define ETHTSYN_FUP_TLV_B26_GM_PH_CHG_10 26U

#if (defined ETHTSYN_FUP_TLV_B27_GM_PH_CHG_11)
#error ETHTSYN_FUP_TLV_B27_GM_PH_CHG_11 is already defined
#endif
#define ETHTSYN_FUP_TLV_B27_GM_PH_CHG_11 27U

#if (defined ETHTSYN_FUP_TLV_B28_GM_FREQ_CHG_3)
#error ETHTSYN_FUP_TLV_B28_GM_FREQ_CHG_3 is already defined
#endif
#define ETHTSYN_FUP_TLV_B28_GM_FREQ_CHG_3 28U /* 32 scaled last grand master frequency change */

#if (defined ETHTSYN_FUP_TLV_B29_GM_FREQ_CHG_2)
#error ETHTSYN_FUP_TLV_B29_GM_FREQ_CHG_2 is already defined
#endif
#define ETHTSYN_FUP_TLV_B29_GM_FREQ_CHG_2 29U

#if (defined ETHTSYN_FUP_TLV_B30_GM_FREQ_CHG_1)
#error ETHTSYN_FUP_TLV_B30_GM_FREQ_CHG_1 is already defined
#endif
#define ETHTSYN_FUP_TLV_B30_GM_FREQ_CHG_1 30U

#if (defined ETHTSYN_FUP_TLV_B31_GM_FREQ_CHG_0)
#error ETHTSYN_FUP_TLV_B31_GM_FREQ_CHG_0 is already defined
#endif
#define ETHTSYN_FUP_TLV_B31_GM_FREQ_CHG_0 31U

/* ====== AUTOSAR TLV header data ====== */
#if (defined ETHTSYN_FUP_EXT_TLV_LENGTH_0)
#error ETHTSYN_FUP_EXT_TLV_LENGTH_0 is already defined
#endif
#define ETHTSYN_FUP_EXT_TLV_LENGTH_0 6U

#if (defined ETHTSYN_FUP_EXT_TLV_LENGTH_1)
#error ETHTSYN_FUP_EXT_TLV_LENGTH_1 is already defined
#endif
#define ETHTSYN_FUP_EXT_TLV_LENGTH_1 0U

#if (defined ETHTSYN_FUP_EXT_TLV_ORGID_2)
#error ETHTSYN_FUP_EXT_TLV_ORGID_2 is already defined
#endif
#define ETHTSYN_FUP_EXT_TLV_ORGID_2 0x1AU

#if (defined ETHTSYN_FUP_EXT_TLV_ORGID_1)
#error ETHTSYN_FUP_EXT_TLV_ORGID_1 is already defined
#endif
#define ETHTSYN_FUP_EXT_TLV_ORGID_1 0x75U

#if (defined ETHTSYN_FUP_EXT_TLV_ORGID_0)
#error ETHTSYN_FUP_EXT_TLV_ORGID_0 is already defined
#endif
#define ETHTSYN_FUP_EXT_TLV_ORGID_0 0xFBU

#if (defined ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_2)
#error ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_2 is already defined
#endif
#define ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_2 0x60U

#if (defined ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_1)
#error ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_1 is already defined
#endif
#define ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_1 0x56U

#if (defined ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_0)
#error ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_0 is already defined
#endif
#define ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_0 0x76U

/* ====== AUTOSAR follow up sub TLV ====== */
#if (defined ETHTSYN_FUP_SUBTLV_TIME_SIZE)
#error ETHTSYN_FUP_SUBTLV_TIME_SIZE is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_TIME_SIZE 5U

#if (defined ETHTSYN_FUP_SUBTLV_TIME_B0_TYPE)
#error ETHTSYN_FUP_SUBTLV_TIME_B0_TYPE is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_TIME_B0_TYPE 0U

#if (defined ETHTSYN_FUP_SUBTLV_TIME_B1_LENGTH)
#error ETHTSYN_FUP_SUBTLV_TIME_B1_LENGTH] is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_TIME_B1_LENGTH 1U

#if (defined ETHTSYN_FUP_SUBTLV_TIME_B2_TIME_FLAGS)
#error ETHTSYN_FUP_SUBTLV_TIME_B2_TIME_FLAGS is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_TIME_B2_TIME_FLAGS 2U

#if (defined ETHTSYN_FUP_SUBTLV_TIME_B3_CRC_TIME_0)
#error ETHTSYN_FUP_SUBTLV_TIME_B3_CRC_TIME_0 is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_TIME_B3_CRC_TIME_0 3U

#if (defined ETHTSYN_FUP_SUBTLV_TIME_B4_CRC_TIME_1)
#error ETHTSYN_FUP_SUBTLV_TIME_B4_CRC_TIME_1 is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_TIME_B4_CRC_TIME_1 4U

#if (defined ETHTSYN_FUP_SUBTLV_TIME_TYPE)
#error ETHTSYN_FUP_SUBTLV_TIME_TYPE is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_TIME_TYPE 0x28U

#if (defined ETHTSYN_FUP_SUBTLV_TIME_LENGTH)
#error ETHTSYN_FUP_SUBTLV_TIME_LENGTH is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_TIME_LENGTH 3U

#if (defined ETHTSYN_FUP_AUTOSAR_TLV_HEADER_LENGTH)
#error ETHTSYN_FUP_AUTOSAR_TLV_HEADER_LENGTH is already defined
#endif
#define ETHTSYN_FUP_AUTOSAR_TLV_HEADER_LENGTH 10U

#if (defined ETHTSYN_FUP_SUBTLV_HEADER_SIZE)
#error ETHTSYN_FUP_SUBTLV_HEADER_SIZE is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_HEADER_SIZE 2U

#if (defined ETHTSYN_FUP_AUTOSAR_TLV_HEADER_START_ADDR)
#error ETHTSYN_FUP_AUTOSAR_TLV_HEADER_START_ADDR is already defined
#endif
#define ETHTSYN_FUP_AUTOSAR_TLV_HEADER_START_ADDR 76U

#if (defined ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR)
#error ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR is already defined
#endif
#define ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR 86U

#if (defined ETHTSYN_MESSAGE_LENGTH_BIT_MASK)
#error ETHTSYN_MESSAGE_LENGTH_BIT_MASK is already defined
#endif
#define ETHTSYN_MESSAGE_LENGTH_BIT_MASK 0x01U

#if (defined ETHTSYN_DOMAIN_NUMBER_BIT_MASK)
#error ETHTSYN_DOMAIN_NUMBER_BIT_MASK is already defined
#endif
#define ETHTSYN_DOMAIN_NUMBER_BIT_MASK 0x02U

#if (defined ETHTSYN_CORRECTION_FIELD_BIT_MASK)
#error ETHTSYN_CORRECTION_FIELD_BIT_MASK is already defined
#endif
#define ETHTSYN_CORRECTION_FIELD_BIT_MASK 0x04U

#if (defined ETHTSYN_SOURCE_PORT_IDENTITY_BIT_MASK)
#error ETHTSYN_SOURCE_PORT_IDENTITY_BIT_MASK is already defined
#endif
#define ETHTSYN_SOURCE_PORT_IDENTITY_BIT_MASK 0x08U

#if (defined ETHTSYN_SEQUENCE_ID_BIT_MASK)
#error ETHTSYN_SEQUENCE_ID_BIT_MASK is already defined
#endif
#define ETHTSYN_SEQUENCE_ID_BIT_MASK 0x10U

#if (defined ETHTSYN_PRECISE_ORIGIN_TIMESTAMP_BIT_MASK)
#error ETHTSYN_PRECISE_ORIGIN_TIMESTAMP_BIT_MASK is already defined
#endif
#define ETHTSYN_PRECISE_ORIGIN_TIMESTAMP_BIT_MASK 0x20U

/* ====== AUTOSAR follow-up STATUS sub TLV ====== */
#if (defined ETHTSYN_FUP_SUBTLV_STATUS_SIZE)
#error ETHTSYN_FUP_SUBTLV_STATUS_SIZE] is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_STATUS_SIZE 4U

#if (defined ETHTSYN_FUP_SUBTLV_STATUS_B0_TYPE)
#error ETHTSYN_FUP_SUBTLV_STATUS_B0_TYPE] is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_STATUS_B0_TYPE 0U

#if (defined ETHTSYN_FUP_SUBTLV_STATUS_B1_LENGTH)
#error ETHTSYN_FUP_SUBTLV_STATUS_B1_LENGTH] is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_STATUS_B1_LENGTH 1U

#if (defined ETHTSYN_FUP_SUBTLV_STATUS_B2_STATUS)
#error ETHTSYN_FUP_SUBTLV_STATUS_B2_STATUS is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_STATUS_B2_STATUS 2U

#if (defined ETHTSYN_FUP_SUBTLV_STATUS_B3_CRC_STATUS)
#error ETHTSYN_FUP_SUBTLV_STATUS_B3_CRC_STATUS is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_STATUS_B3_CRC_STATUS 3U

#if (defined ETHTSYN_FUP_SUBTLV_STATUS_NOT_SECURED_TYPE)
#error ETHTSYN_FUP_SUBTLV_STATUS_NOT_SECURED_TYPE is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_STATUS_NOT_SECURED_TYPE 0x51U

#if (defined ETHTSYN_FUP_SUBTLV_STATUS_SECURED_TYPE)
#error ETHTSYN_FUP_SUBTLV_STATUS_SECURED_TYPE is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_STATUS_SECURED_TYPE 0x50U

#if (defined ETHTSYN_FUP_SUBTLV_STATUS_LENGTH)
#error ETHTSYN_FUP_SUBTLV_STATUS_LENGTH is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_STATUS_LENGTH 2U

#if (defined ETHTSYN_FUP_SUBTLV_STATUS_CRC_STATUS)
#error ETHTSYN_FUP_SUBTLV_STATUS_CRC_STATUS is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_STATUS_CRC_STATUS 0U

#if (defined ETHTSYN_SYNC_TO_SUBDOMAIN)
#error ETHTSYN_SYNC_TO_SUBDOMAIN is already defined
#endif
#define ETHTSYN_SYNC_TO_SUBDOMAIN 1U

#if (defined ETHTSYN_SYNC_TO_GTM)
#error ETHTSYN_SYNC_TO_GTM is already defined
#endif
#define ETHTSYN_SYNC_TO_GTM 0U

#if (defined ETHTSYN_GLOBAL_TIME_BASE_MASK)
#error ETHTSYN_GLOBAL_TIME_BASE_MASK is already defined
#endif
#define ETHTSYN_GLOBAL_TIME_BASE_MASK  8U /* Global time base mask */

#if (defined ETHTSYN_SYNC_TO_GATEWAY_MASK_STBM)
#error ETHTSYN_SYNC_TO_GATEWAY_MASK_STBM is already defined
#endif
#define ETHTSYN_SYNC_TO_GATEWAY_MASK_STBM  0x04U /* Sync to gateway mask */

#if (defined ETHTSYN_SYNC_TO_GATEWAY_MASK_SUBTLV)
#error ETHTSYN_SYNC_TO_GATEWAY_MASK_SUBTLV is already defined
#endif
#define ETHTSYN_SYNC_TO_GATEWAY_MASK_SUBTLV  0x01U /* Sync to gateway mask */

/* ====== AUTOSAR follow-up UserData sub TLV ====== */
#if (defined ETHTSYN_FUP_SUBTLV_USERDATA_SIZE)
#error ETHTSYN_FUP_SUBTLV_USERDATA_SIZE is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_USERDATA_SIZE 7U

#if (defined ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH)
#error ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH 5U

#if (defined ETHTSYN_FUP_SUBTLV_USERDATA_SECURED_TYPE)
#error ETHTSYN_FUP_SUBTLV_USERDATA_SECURED_TYPE is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_USERDATA_SECURED_TYPE 0x60U

#if (defined ETHTSYN_FUP_SUBTLV_USERDATA_NOT_SECURED_TYPE)
#error ETHTSYN_FUP_SUBTLV_USERDATA_NOT_SECURED_TYPE is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_USERDATA_NOT_SECURED_TYPE 0x61U

#if (defined ETHTSYN_FUP_SUBTLV_USERDATA_B0_TYPE)
#error ETHTSYN_FUP_SUBTLV_USERDATA_B0_TYPE is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_USERDATA_B0_TYPE 0U

#if (defined ETHTSYN_FUP_SUBTLV_USERDATA_B1_LENGTH)
#error ETHTSYN_FUP_SUBTLV_USERDATA_B1_LENGTH is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_USERDATA_B1_LENGTH 1U

#if (defined ETHTSYN_FUP_SUBTLV_USERDATA_B2_USER_DATA_LENGTH)
#error ETHTSYN_FUP_SUBTLV_USERDATA_B2_USER_DATA_LENGTH is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_USERDATA_B2_USER_DATA_LENGTH 2U

#if (defined ETHTSYN_FUP_SUBTLV_USERDATA_B3_USER_BYTE_0)
#error ETHTSYN_FUP_SUBTLV_USERDATA_B3_USER_BYTE_0 is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_USERDATA_B3_USER_BYTE_0 3U

#if (defined ETHTSYN_FUP_SUBTLV_USERDATA_B4_USER_BYTE_1)
#error ETHTSYN_FUP_SUBTLV_USERDATA_B4_USER_BYTE_1 is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_USERDATA_B4_USER_BYTE_1 4U

#if (defined ETHTSYN_FUP_SUBTLV_USERDATA_B5_USER_BYTE_2)
#error ETHTSYN_FUP_SUBTLV_USERDATA_B5_USER_BYTE_2 is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_USERDATA_B5_USER_BYTE_2 5U

#if (defined ETHTSYN_FUP_SUBTLV_USERDATA_B6_CRC_USERDATA)
#error ETHTSYN_FUP_SUBTLV_USERDATA_B6_CRC_USERDATA is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_USERDATA_B6_CRC_USERDATA 6U

/* ====== AUTOSAR follow-up OFS sub TLV ====== */
#if (defined ETHTSYN_FUP_SUBTLV_OFS_SIZE)
#error ETHTSYN_FUP_SUBTLV_OFS_SIZE] is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_SIZE 19U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B0_TYPE)
#error ETHTSYN_FUP_SUBTLV_OFS_B0_TYPE] is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B0_TYPE 0U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B1_LENGTH)
#error ETHTSYN_FUP_SUBTLV_OFS_B1_LENGTH] is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B1_LENGTH 1U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B2_TIMEDOMAIN)
#error ETHTSYN_FUP_SUBTLV_OFS_B2_TIMEDOMAIN is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B2_TIMEDOMAIN 2U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B3_TIMESEC)
#error ETHTSYN_FUP_SUBTLV_OFS_B3_TIMESEC is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B3_TIMESEC 3U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B4_TIMESEC)
#error ETHTSYN_FUP_SUBTLV_OFS_B4_TIMESEC is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B4_TIMESEC 4U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B5_TIMESEC)
#error ETHTSYN_FUP_SUBTLV_OFS_B5_TIMESEC is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B5_TIMESEC 5U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B6_TIMESEC)
#error ETHTSYN_FUP_SUBTLV_OFS_B6_TIMESEC is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B6_TIMESEC 6U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B7_TIMESEC)
#error ETHTSYN_FUP_SUBTLV_OFS_B7_TIMESEC is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B7_TIMESEC 7U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B8_TIMESEC)
#error ETHTSYN_FUP_SUBTLV_OFS_B8_TIMESEC is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B8_TIMESEC 8U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B9_TIMENSEC)
#error ETHTSYN_FUP_SUBTLV_OFS_B9_TIMENSEC is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B9_TIMENSEC 9U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B10_TIMENSEC)
#error ETHTSYN_FUP_SUBTLV_OFS_B10_TIMENSEC is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B10_TIMENSEC 10U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B11_TIMENSEC)
#error ETHTSYN_FUP_SUBTLV_OFS_B11_TIMENSEC is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B11_TIMENSEC 11U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B12_TIMENSEC)
#error ETHTSYN_FUP_SUBTLV_OFS_B12_TIMENSEC is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B12_TIMENSEC 12U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B13_STATUS)
#error ETHTSYN_FUP_SUBTLV_OFS_B13_STATUS is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B13_STATUS 13U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B14_USER_DATA_LENGTH)
#error ETHTSYN_FUP_SUBTLV_OFS_B14_USER_DATA_LENGTH is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B14_USER_DATA_LENGTH 14U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B15_USER_DATA_BYTE_0)
#error ETHTSYN_FUP_SUBTLV_OFS_B15_USER_DATA_BYTE_0 is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B15_USER_DATA_BYTE_0 15U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B16_USER_DATA_BYTE_1)
#error ETHTSYN_FUP_SUBTLV_OFS_B16_USER_DATA_BYTE_1 is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B16_USER_DATA_BYTE_1 16U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B17_USER_DATA_BYTE_2)
#error ETHTSYN_FUP_SUBTLV_OFS_B17_USER_DATA_BYTE_2 is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B17_USER_DATA_BYTE_2 17U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_B18_CRC)
#error ETHTSYN_FUP_SUBTLV_OFS_B18_CRC is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_B18_CRC 18U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_NOT_SECURED_TYPE)
#error ETHTSYN_FUP_SUBTLV_OFS_NOT_SECURED_TYPE is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_NOT_SECURED_TYPE 0x34U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_SECURED_TYPE)
#error ETHTSYN_FUP_SUBTLV_OFS_SECURED_TYPE is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_SECURED_TYPE 0x44U

#if (defined ETHTSYN_FUP_SUBTLV_OFS_LENGTH)
#error ETHTSYN_FUP_SUBTLV_OFS_LENGTH is already defined
#endif
#define ETHTSYN_FUP_SUBTLV_OFS_LENGTH 17U

/* ====== Announce frame values ====== */

/* Time sync grand master priority (See 802.1AS-2011 Ch 10.5.3.2.2) */
#if (defined ETHTSYN_GM_PRIORITY1)
#error ETHTSYN_GM_PRIORITY1 is already defined
#endif
#define ETHTSYN_GM_PRIORITY1 246U

#if (defined ETHTSYN_GM_PRIORITY2)
#error ETHTSYN_GM_PRIORITY2 is already defined
#endif
#define ETHTSYN_GM_PRIORITY2 246U

#if (defined ETHTSYN_GM_CLOCK_CLASS)
#error ETHTSYN_GM_CLOCK_CLASS is already defined
#endif
/* Time sync grand master clockClass (See 802.1AS-2011 Ch 8.6.2.2) */
#define ETHTSYN_GM_CLOCK_CLASS 248U

#if (defined ETHTSYN_GM_CLOCK_ACCURANCY)
#error ETHTSYN_GM_CLOCK_ACCURANCY is already defined
#endif
/* Time sync grand master clockClass (See 802.1AS-2011 Ch 8.6.2.3) */
#define ETHTSYN_GM_CLOCK_ACCURANCY 254U

/* Time sync grand master offset scaled log variance (See 802.1AS-2011 Ch 8.6.2.4) */
#if (defined ETHTSYN_GM_OFFSET_SCALEDLOG_VARIANCE_1)
#error ETHTSYN_GM_OFFSET_SCALEDLOG_VARIANCE_1 is already defined
#endif
#define ETHTSYN_GM_OFFSET_SCALEDLOG_VARIANCE_1 0x41U /* 16 bit */

#if (defined ETHTSYN_GM_OFFSET_SCALEDLOG_VARIANCE_0)
#error ETHTSYN_GM_OFFSET_SCALEDLOG_VARIANCE_0 is already defined
#endif
#define ETHTSYN_GM_OFFSET_SCALEDLOG_VARIANCE_0 0x00U

#if(ETHTSYN_SEND_ANNOUNCEFRAMES == STD_ON)
#if (defined ETHTSYN_ANNOUNCE_TLV_TYPE_1)
#error ETHTSYN_ANNOUNCE_TLV_TYPE_1 is already defined
#endif
#define ETHTSYN_ANNOUNCE_TLV_TYPE_1 0x00U /* TLV type (See 802.1AS-2011 Ch 10.5.3.3.2) */

#if (defined ETHTSYN_ANNOUNCE_TLV_TYPE_0)
#error ETHTSYN_ANNOUNCE_TLV_TYPE_0 is already defined
#endif
#define ETHTSYN_ANNOUNCE_TLV_TYPE_0 0x08U

#if (defined ETHTSYN_ANNOUNCE_TLV_LENGTH_1)
#error ETHTSYN_ANNOUNCE_TLV_LENGTH_1 is already defined
#endif
#define ETHTSYN_ANNOUNCE_TLV_LENGTH_1 0U /* Length (See 802.1AS-2011 Ch 10.5.3.3.3) */

#if (defined ETHTSYN_ANNOUNCE_TLV_LENGTH_0)
#error ETHTSYN_ANNOUNCE_TLV_LENGTH_0 is already defined
#endif
#define ETHTSYN_ANNOUNCE_TLV_LENGTH_0 8U

/* ====== Announce frame Payload ====== */
#if (defined ETHTSYN_ANNOUNCE_PL_B09_RESERVED)
#error ETHTSYN_ANNOUNCE_PL_B09_RESERVED is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B09_RESERVED 9U

#if (defined ETHTSYN_ANNOUNCE_PL_B10_CUR_UTC_OFFSET_1)
#error ETHTSYN_ANNOUNCE_PL_B10_CUR_UTC_OFFSET_1 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B10_CUR_UTC_OFFSET_1 10U /* 16 bit */

#if (defined ETHTSYN_ANNOUNCE_PL_B11_CUR_UTC_OFFSET_0)
#error ETHTSYN_ANNOUNCE_PL_B11_CUR_UTC_OFFSET_0 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B11_CUR_UTC_OFFSET_0 11U

#if (defined ETHTSYN_ANNOUNCE_PL_B12_RESERVED)
#error ETHTSYN_ANNOUNCE_PL_B12_RESERVED is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B12_RESERVED 12U

#if (defined ETHTSYN_ANNOUNCE_PL_B13_GM_PRIO1)
#error ETHTSYN_ANNOUNCE_PL_B13_GM_PRIO1 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B13_GM_PRIO1 13U

#if (defined ETHTSYN_ANNOUNCE_PL_B14_GM_CLK_QUAL_3)
#error ETHTSYN_ANNOUNCE_PL_B14_GM_CLK_QUAL_3 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B14_GM_CLK_QUAL_3 14U /* Type ClockQuality 4 byte */

#if (defined ETHTSYN_ANNOUNCE_PL_B15_GM_CLK_QUAL_2)
#error ETHTSYN_ANNOUNCE_PL_B15_GM_CLK_QUAL_2 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B15_GM_CLK_QUAL_2 15U

#if (defined ETHTSYN_ANNOUNCE_PL_B16_GM_CLK_QUAL_1)
#error ETHTSYN_ANNOUNCE_PL_B16_GM_CLK_QUAL_1 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B16_GM_CLK_QUAL_1 16U

#if (defined ETHTSYN_ANNOUNCE_PL_B17_GM_CLK_QUAL_0)
#error ETHTSYN_ANNOUNCE_PL_B17_GM_CLK_QUAL_0 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B17_GM_CLK_QUAL_0 17U

#if (defined ETHTSYN_ANNOUNCE_PL_B18_GM_PRIO2)
#error ETHTSYN_ANNOUNCE_PL_B18_GM_PRIO2 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B18_GM_PRIO2 18U

#if (defined ETHTSYN_ANNOUNCE_PL_B19_GM_IDENTITY_0)
#error ETHTSYN_ANNOUNCE_PL_B19_GM_IDENTITY_0 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B19_GM_IDENTITY_0 19U

#if (defined ETHTSYN_ANNOUNCE_PL_B20_GM_IDENTITY_1)
#error ETHTSYN_ANNOUNCE_PL_B20_GM_IDENTITY_1 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B20_GM_IDENTITY_1 20U

#if (defined ETHTSYN_ANNOUNCE_PL_B21_GM_IDENTITY_2)
#error ETHTSYN_ANNOUNCE_PL_B21_GM_IDENTITY_2 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B21_GM_IDENTITY_2 21U

#if (defined ETHTSYN_ANNOUNCE_PL_B22_GM_IDENTITY_3)
#error ETHTSYN_ANNOUNCE_PL_B22_GM_IDENTITY_3 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B22_GM_IDENTITY_3 22U

#if (defined ETHTSYN_ANNOUNCE_PL_B23_GM_IDENTITY_4)
#error ETHTSYN_ANNOUNCE_PL_B23_GM_IDENTITY_4 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B23_GM_IDENTITY_4 23U

#if (defined ETHTSYN_ANNOUNCE_PL_B24_GM_IDENTITY_5)
#error ETHTSYN_ANNOUNCE_PL_B24_GM_IDENTITY_5 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B24_GM_IDENTITY_5 24U

#if (defined ETHTSYN_ANNOUNCE_PL_B25_GM_IDENTITY_6)
#error ETHTSYN_ANNOUNCE_PL_B25_GM_IDENTITY_6 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B25_GM_IDENTITY_6 25U

#if (defined ETHTSYN_ANNOUNCE_PL_B26_GM_IDENTITY_7)
#error ETHTSYN_ANNOUNCE_PL_B26_GM_IDENTITY_7 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B26_GM_IDENTITY_7 26U

#if (defined ETHTSYN_ANNOUNCE_PL_B27_STEP_REM_1)
#error ETHTSYN_ANNOUNCE_PL_B27_STEP_REM_1 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B27_STEP_REM_1 27U /* 16 bit */

#if (defined ETHTSYN_ANNOUNCE_PL_B28_STEP_REM_0)
#error ETHTSYN_ANNOUNCE_PL_B28_STEP_REM_0 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B28_STEP_REM_0 28U

#if (defined ETHTSYN_ANNOUNCE_PL_B29_TIME_SOURCE)
#error ETHTSYN_ANNOUNCE_PL_B29_TIME_SOURCE is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B29_TIME_SOURCE 29U

#if (defined ETHTSYN_ANNOUNCE_PL_B30_PATH_TRACE_TLV_TYPE_1)
#error ETHTSYN_ANNOUNCE_PL_B30_PATH_TRACE_TLV_TYPE_1 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B30_PATH_TRACE_TLV_TYPE_1 30U

#if (defined ETHTSYN_ANNOUNCE_PL_B31_PATH_TRACE_TLV_TYPE_0)
#error ETHTSYN_ANNOUNCE_PL_B31_PATH_TRACE_TLV_TYPE_0 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B31_PATH_TRACE_TLV_TYPE_0 31U

#if (defined ETHTSYN_ANNOUNCE_PL_B32_PATH_TRACE_TLV_LEN_0)
#error ETHTSYN_ANNOUNCE_PL_B32_PATH_TRACE_TLV_LEN_0 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B32_PATH_TRACE_TLV_LEN_0 32U

#if (defined ETHTSYN_ANNOUNCE_PL_B33_PATH_TRACE_TLV_LEN_1)
#error ETHTSYN_ANNOUNCE_PL_B33_PATH_TRACE_TLV_LEN_1 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B33_PATH_TRACE_TLV_LEN_1 33U

#if (defined ETHTSYN_ANNOUNCE_PL_B34_PATH_TRACE_TLV_SEQ_0)
#error ETHTSYN_ANNOUNCE_PL_B34_PATH_TRACE_TLV_SEQ_0 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B34_PATH_TRACE_TLV_SEQ_0 34U

#if (defined ETHTSYN_ANNOUNCE_PL_B35_PATH_TRACE_TLV_SEQ_1)
#error ETHTSYN_ANNOUNCE_PL_B35_PATH_TRACE_TLV_SEQ_1 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B35_PATH_TRACE_TLV_SEQ_1 35U

#if (defined ETHTSYN_ANNOUNCE_PL_B36_PATH_TRACE_TLV_SEQ_2)
#error ETHTSYN_ANNOUNCE_PL_B36_PATH_TRACE_TLV_SEQ_2 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B36_PATH_TRACE_TLV_SEQ_2 36U

#if (defined ETHTSYN_ANNOUNCE_PL_B37_PATH_TRACE_TLV_SEQ_3)
#error ETHTSYN_ANNOUNCE_PL_B37_PATH_TRACE_TLV_SEQ_3 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B37_PATH_TRACE_TLV_SEQ_3 37U

#if (defined ETHTSYN_ANNOUNCE_PL_B38_PATH_TRACE_TLV_SEQ_4)
#error ETHTSYN_ANNOUNCE_PL_B38_PATH_TRACE_TLV_SEQ_4 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B38_PATH_TRACE_TLV_SEQ_4 38U

#if (defined ETHTSYN_ANNOUNCE_PL_B39_PATH_TRACE_TLV_SEQ_5)
#error ETHTSYN_ANNOUNCE_PL_B39_PATH_TRACE_TLV_SEQ_5 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B39_PATH_TRACE_TLV_SEQ_5 39U

#if (defined ETHTSYN_ANNOUNCE_PL_B40_PATH_TRACE_TLV_SEQ_6)
#error ETHTSYN_ANNOUNCE_PL_B40_PATH_TRACE_TLV_SEQ_6 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B40_PATH_TRACE_TLV_SEQ_6 40U

#if (defined ETHTSYN_ANNOUNCE_PL_B41_PATH_TRACE_TLV_SEQ_7)
#error ETHTSYN_ANNOUNCE_PL_B41_PATH_TRACE_TLV_SEQ_7 is already defined
#endif
#define ETHTSYN_ANNOUNCE_PL_B41_PATH_TRACE_TLV_SEQ_7 41U
#endif /* ETHTSYN_SEND_ANNOUNCEFRAMES == STD_ON) */

#define ETHTSYN_GET16(headerPtr, constByteOffset) \
  (((uint16)(((uint16)(((headerPtr))[(constByteOffset)]))<<8u)) | \
   ((uint16) ((uint8) (((headerPtr))[(constByteOffset)+1]))) )

#define ETHTSYN_SET_TXFLAG(Flag, BitNr, StateOfGlobalTxFlag)      \
 do{                                                              \
   TS_AtomicSetBit_8(Flag, BitNr);                                \
   TS_AtomicAssign8(EthTSyn_Frame2Transmit, StateOfGlobalTxFlag); \
 }while(0)

#if (defined ETHTSYN_DELTA_SC)
#error ETHTSYN_DELTA_SC is already defined
#endif /* if (defined ETHTSYN_DELTA_SC) */
#define ETHTSYN_DELTA_SC(receivedSeqCount, seqCount)  (((receivedSeqCount) >= (seqCount))?(uint16)((receivedSeqCount) - (seqCount)) : \
                                                             (uint16)(((receivedSeqCount) + ETHTSYN_SC_MAXLIMIT + 1U) - (seqCount)))

/*==================[type definitions]======================================*/

/** \brief Type definition of PTP header fields which are not common for all frames. */
typedef struct
{
  uint16 MsgLength;
  uint16 Flags;
  uint16 SequNumber;
  uint8 MsgType;
  uint8 Control;
  uint8 LogMsgInterval;
}EthTSyn_PTPHeaderType;

/*==================[internal function declarations]========================*/
#define ETHTSYN_START_SEC_CODE
#include <EthTSyn_MemMap.h>

/**
 * \brief Service to translate EthIfCtrlIdx to EthTSynCtrlIdx.
 *
 * \param[in] EthIfCtrlIdx - Index of the EthIf controller.
 * \param[out] EthTSynCtrlIdxPtr - Pointer containing the corresponding EthTSyn controller index.
 *
 * \return E_OK: EthTSyn controller index found.
 *         E_NOT_OK: No EthTSyn controller index found for this EthIfCtrlIdx.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_TranslateEthIfCtrlIdx
(
  uint8 EthIfCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) EthTSynCtrlIdxPtr
);

/**
 * \brief Service to transmit a Sync frame.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TransmitSyncFrame
(
  uint8 EthTSynCtrlIdx
);

/**
 * \brief Service to transmit a Sync follow up frame.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TransmitSyncFUpFrame
(
  uint8 EthTSynCtrlIdx
);

/**
 * \brief Service to calculate origin time stamp with HW timestamping.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \return E_OK: Success
 *         E_NOT_OK: Calculation failed.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_CalculateOriginTimeStamp
(
  uint8 EthTSynCtrlIdx,
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr
#else
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr
#endif
);
#if((ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED != TS_PROD_ERR_DISABLE) && (ETHTSYN_MAX_SLAVE != 0U))
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_SyncFupDropped
(
  uint8 EthTSynCtrlIdx
);
#endif
#if((ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED != TS_PROD_ERR_DISABLE) && (ETHTSYN_MAX_SLAVE != 0U))
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_PDelayFrameDropped
(
  uint8 EthTSynCtrlIdx
);
#endif

#if (ETHTSYN_MAX_SLAVE != 0U)
/**
 * \brief Initialize all slave variables.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Init_SlaveVar
(
  uint8 EthTSynCtrlIdx
);

/**
 * \brief Initialize all slave variables used fo calculation of the pdelay.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ResetSlavePdelayData
(
  uint8 EthTSynCtrlIdx
);

#endif

/**
 * \brief Initialize all Master and common variables.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Init_CtrlVar
(
  uint8 EthTSynCtrlIdx
);

#if((ETHTSYN_TIME_RECORDING == STD_ON) && (ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE))
/**
 * \brief Initialize all slave variables.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Init_TimeRecordingVar
(
  uint8 EthTSynCtrlIdx
);
#endif

/**
 * \brief Find the next minimum of all ports of a controller.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] PortIdx - Index of the EthTSyn ports.
 *
 */
STATIC FUNC(boolean, ETHTSYN_CODE) EthTSyn_NextMinDebounceOffset
(
  P2VAR(uint32, AUTOMATIC, ETHTSYN_APPL_DATA) CommonOffset,
  uint32 SpecificOffset
);



#if(ETHTSYN_SEND_ANNOUNCEFRAMES == STD_ON)
/**
 * \brief Service to transmit an announce frame.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TransmitAnnounceFrame
(
  uint8 EthTSynCtrlIdx
);
#endif /* ETHTSYN_SEND_ANNOUNCEFRAMES == STD_ON) */

/**
 * \brief Service to transmit a Pdelay request frame.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TransmitPdelayReqFrame
(
  uint8 EthTSynCtrlIdx
);

/**
 * \brief Service to transmit a Pdelay response frame.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] RxDataPtr - Pointer to the received Pdelay request payload.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TransmitPdelayRespFrame
(
  uint8 EthTSynCtrlIdx
);

/**
 * \brief Service to transmit a Pdelay response follow up frame.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TransmitPdelayRespFUpFrame
(
  uint8 EthTSynCtrlIdx
);

/**
 * \brief Service to get Tx buffer and fill the PTP header
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] PayloadLength - Length of the frame payload.
 * \param[out] BufIdxPtr - Pointer to the buffer index of the requested Tx frame buffer.
 * \param[out] BufPtr - Pointer to return the pointer of the Ethernet Tx frame buffer.
 *
 * \return E_OK: Tx buffer available and big enough.
 *         E_NOT_OK: No suitable Tx buffer available.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_GetTxBuffer
(
  uint8 EthTSynCtrlIdx,
  uint16 PayloadLength,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETHTSYN_APPL_DATA) BufIdxPtr,
  P2VAR(EthTSyn_PtrUint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
);

/**
 * \brief Service to fill the PTP header for all outgoing EthTSyn frames.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] PTPHeader - PTP header containing all bytes which do not have common values.
 * \param[out] BufPtr - Pointer to store the created 34 byte PTP header.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillPTPHeader
(
  uint8 EthTSynCtrlIdx,
  P2CONST(EthTSyn_PTPHeaderType, AUTOMATIC, ETHTSYN_APPL_DATA) PTPHeader,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
);

/**
 * \brief Service to fill the TLV information of the follow up frame.
 *
 * \param[out] BufPtr - Pointer to store the TLV information.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillFUpTLV
(
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
);

#if(ETHTSYN_SEND_ANNOUNCEFRAMES == STD_ON)
/**
 * \brief Service to fill announce frame payload.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[out] BufPtr - Pointer to store the created announce frame payload.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillAnnouncePayload
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
);
#endif /* ETHTSYN_SEND_ANNOUNCEFRAMES == STD_ON) */

/**
 * \brief Service to send the PTP frame.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] BufIdx - Buffer index of used Tx frame buffer.
 * \param[in] PayloadLength - Length of the frame payload.
 * \param[in] ActivateTimeStamp - Activates egress time stamping.
 *
 * \return E_OK: Transmit performed.
 *         E_NOT_OK: Error detected.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_TransmitFrame
(
  uint8 EthTSynCtrlIdx,
  Eth_BufIdxType BufIdx,
  uint16 PayloadLength,
  boolean ActivateTimeStamp
);

/**
 * \brief Service to convert the time stamp to a byte array ready to send via frame.
 *
 * \param[in] TimeStampPtr - Pointer to time stamp which requires conversion.
 * \param[out] BufPtr - Pointer to store the converted time stamp.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TimeStampToArray
(
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
);

#if (ETHTSYN_MAX_SLAVE != 0U)
/**
 * \brief Service to convert a byte array to a time stamp.
 *
 * \param[out] TimeStampPtr - Pointer to store the converted time stamp.
 * \param[in] BufPtr - Pointer provide the time stamp in byte array.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ArrayToTimeStamp
(
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
);

/**
 * \brief Service to process incoming Sync frames.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to the payload of the frame.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ProcessRxSyncFrame
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr
);

/**
 * \brief Service to process incoming Sync follow up frames.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to the payload of the frame.
 * \param[in] Length - Length of the FUp frame.
 *
 * \return Sequence number which is stored in the PTP header.
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ProcessRxSynFUpFrame
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length
);

/**
 * \brief Service to process incoming Pdelay response frames.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to the payload of the frame.
 *
 * \return Sequence number which is stored in the PTP header.
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ProcessRxPdelayRespFrame
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr
);

/**
 * \brief Service to process incoming Pdelay response follow up frames.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to the payload of the frame.
 *
 * \return Sequence number which is stored in the PTP header.
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ProcessRxPdelayRespFUpFrame
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr
);

/**
 * \brief Service to check whether an EthIf controller is a slave.
 *
 * This service checks whether an EthIf controller is a slave.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthIf controller
 *
 * \return TRUE if EthTSynCtrlIdx is a slave
 *         FALSE if EthTSynCtrlIdx is not a slave
 */
STATIC FUNC(boolean, ETHTSYN_CODE) EthTSyn_IsSlave
(
  uint8 EthTSynCtrlIdx
);

/**
 * \brief Service to convert and add correction field array to time stamp.
 *
 * This converts the 8 byte correction field array into a time stamp value and adds it to
 * the given time stamp.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] CorrectionFieldArray - 8 byte correction field array in ns.
 * \param[in/out] TimeStampPtr - Pointer returning the sum of this time stamp and the converted
 *                               time stamp.
 *
 * \return E_OK: Correction field is valid.
 *         E_NOT_OK: Correction field is invalid.
 *
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_AddCorrField
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) CorrectionFieldArray,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr
);

/**
 * \brief Service to calculate the Pdelay.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 *
 */

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_CalculatePdelay
(
  uint8 EthTSynCtrlIdx
);

#endif /* ETHTSYN_MAX_SLAVE != 0U */

#if((ETHTSYN_MAX_SLAVE != 0U) || (ETHTSYN_SWITCH_USED == STD_ON))
/**
 * \brief Service to subtract two time stamps.
 *
 * This service subtracts two time stamps and returns the result in the first time stamp
 * parameter if successful. In case that subtraction was not successful (negative result) the
 * parameters are not changed.
 *
 * Note: A result can either be positive or negative. If service returns E_NOT_OK the call
 *       with swapped parameters will always return E_OK.
 *
 * \param[in] TimeStamp_MinuendPtr - Minuend pointer to a time stamp for subtraction.
 * \param[in] TimeStamp_SubtrahendPtr - Subtrahend pointer to a time stamp for subtraction.
 * \param[in/out] TimeStamp_ResultPtr - Result pointer provides a variable for the result of the
 *                                      subtraction.
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_SubTimeStamps
(
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStamp_MinuendPtr,
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStamp_SubtrahendPtr,
  P2VAR(Eth_TimeIntDiffType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStamp_ResultPtr
);

/**
 * \brief Service to compare two time stamps.
 *
 * This service compares two time stamps to detect which one is bigger.
 *
 * \param[in] TimeStamp1Ptr - First pointer to a time stamp.
 * \param[in] TimeStamp2Ptr - Second pointer to a time stamp.
 *
 * \return TRUE if TimeStamp1Ptr >= TimeStamp2Ptr
 *         FALSE if TimeStamp1Ptr < TimeStamp2Ptr
 */

STATIC FUNC(boolean, ETHTSYN_CODE) EthTSyn_TimeStamp_IsGreaterOrEqual
(
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStamp1Ptr,
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStamp2Ptr
);
#endif
/**
 * \brief Service to get the ingress time stamp.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] RxDataPtr - Pointer to the received frame.
 * \param[out] TimeStampPtr - Pointer to return the actual time stamp.
 *
 * \return E_OK: Transmit performed.
 *         E_NOT_OK: Error detected.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_GetIngressTimeStamp
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) RxDataPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr
);

/**
 * \brief Service to get the egress time stamp.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] BufIdx - Buffer index of used Tx frame buffer.
 * \param[out] TimeStampPtr - Pointer to return the actual time stamp.
 *
 * \return E_OK: Transmit performed.
 *         E_NOT_OK: Error detected.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_GetEgressTimeStamp
(
  uint8 EthTSynCtrlIdx,
  Eth_BufIdxType BufIdx,
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr
#else
  P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr
#endif
);

/**
 * \brief Service to return serial number from PTP header.
 *
 * \param[in] PTPHeaderPtr - Pointer PTP header.
 *
 * \return Sequence number which is stored in the PTP header.
 */
STATIC FUNC(uint16, ETHTSYN_CODE) EthTSyn_GetPTPHeaderSequNumber
(
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PTPHeaderPtr
);

#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
/**
 * \brief Service to fill the AUTOSAR TLV information of the follow up frame.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[out] BufPtr - Pointer to store the TLV information.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillFUpAutosarTLV
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
);

/**
 * \brief Service to fill the not secured AUTOSAR TLV information of the follow up frame.
 *
 * \param[out] BufPtr - Pointer to store the TLV information.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillFUpSubTLVStatus_NotSecured
(
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint8 SubTlvOffset
);

/**
 * \brief Service to fill the secured AUTOSAR TLV information of the follow up frame.
 *
 * \param[out] BufPtr - Pointer to store the TLV information.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillFUpSubTLV_UserData_NotSecured
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint16 SubTlvOffset
);

/**
 * \brief Service to fill the OFS AUTOSAR TLV information of the follow up frame.
 *
 * \param[out] BufPtr - Pointer to store the TLV information.
 *
 * * \return E_OK: Received data from StbM_GetOffset() was valid.
 *           E_NOT_OK: Received data from StbM_GetOffset() was invalid.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_FillFUpSubTLVOfs
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint8 SubTlvOffset
);

/**
 * \brief Service to fill in the the status field of the SubTLV.
 *
 * \param[in] BufPtr - Pointer to store the TLV information.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FUpSubTLVStatus
(
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
);


#if(ETHTSYN_TX_CRC_USED == STD_ON)

/**
 * \brief Service to fill the secured AUTOSAR TLV information of the follow up frame.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[out] BufPtr - Pointer to store the TLV information.
 * \param[in] SubTlvOffset - Offset where the Time Sub Tlv is found in the frame.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillFUpSubTLVTime_Secured
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint8 SubTlvOffset
);


/**
 * \brief Service to fill the secured AUTOSAR TLV information of the follow up frame.
 *
 * \param[out] BufPtr - Pointer to store the TLV information.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillFUpSubTLVStatus_Secured
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint8 SubTlvOffset
);

/**
 * \brief Service to fill the secured AUTOSAR TLV information of the follow up frame.
 *
 * \param[out] BufPtr - Pointer to store the TLV information.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillFUpSubTLV_UserData_Secured
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint16 SubTlvOffset
);

#endif /* (ETHTSYN_TX_CRC_USED == STD_ON) */

/**
 * \brief Service to calculate the slave CRC.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] Crc_0 - Pointer to store the calculated Crc_0.
 * \param[in] Crc_1 - Pointer to store the calculated Crc_1.
 *
 * \return E_OK: Header data is valid.
 *         E_NOT_OK: Header data is invalid.
 *
 */
#if((ETHTSYN_RX_CRC_USED == STD_ON) || (ETHTSYN_TX_CRC_USED == STD_ON))
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_CalculateCrc
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) Crc_0,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) Crc_1
);

/**
 * \brief Service to calculate the CRC userdata in case of master, and to calculate and compare
 *        in case of slave.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] SubTlvOffset - Offset where we find the respective Sub Tlv.
 * \param[out]Crc_UserData - Pointer to store the calculated Crc (only for master, set to NULL_PTR
 *            otherwise).
 */

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_CalcComp_UserDataCrc
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint16 SubTlvOffset,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) Crc_UserData
);

/**
 * \brief Service to calculate the OFS CRC in case of master, and to calculate and compare
 *        in case of slave.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] SubTlvOffset - Offset where we find the respective Sub Tlv.
 * \param[out]Crc_UserData - Pointer to store the calculated Crc (only for master, set to NULL_PTR
 *            otherwise).
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_CalcComp_OfsCrc
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint16 SubTlvOffset,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) Crc_Ofs
);

#endif

/**
 * \brief Service to calculate the slave CRC for Time Secured Sub-Tlv.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] SubTlvOffset - Offset where we find the respective Sub Tlv.
 *
 */
#if(ETHTSYN_RX_CRC_USED == STD_ON)
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF)
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_CompareCrc
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 SubTlvOffset
);

/**
 * \brief Service to calculate the slave CRC for Status Secured Sub-Tlv
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] SubTlvOffset - Offset where we find the respective Sub Tlv.
 *
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_CompareCrc_Status
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 SubTlvOffset
);
#endif
#endif

#if (ETHTSYN_MAX_SLAVE != 0U)
/**
 * \brief Service to verify the AUTOSAR Sub-TLV.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] Length - Length of the received frame
 *
 * \return E_OK: AUTOSAR TLV is valid.
 *         E_NOT_OK: AUTOSAR SubTLV is invalid.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyAutosarTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length
);

/**
 * \brief Service to verify the TLV header data.
 *
 * \param[in] DataPtr - Pointer to store the received FUp information.
 *
 * \return E_OK: Header data is valid.
 *         E_NOT_OK: Header data is invalid.
 *
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyTLVHeader
(
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr
);

/**
 * \brief Service to verify the SubTLV data.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] Length - Length of the received frame
 *
 * \return E_OK: SubTLV data is valid.
 *         E_NOT_OK: SubTLV data is invalid.
 */
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF)
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifySubTLVData
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length
);

/**
 * \brief Service to verify the TimeSecured SubTLV.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] Length - Length of the received frame.
 * \param[in] Offset - Offset to where the respective SubTlv starts.
 *
 * \return E_OK: SubTLV data is valid.
 *         E_NOT_OK: Received and calculated Crc are not equal.
                     Slave is configured to CRC_NOT_VALIDATED.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyTimeSecuredSubTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length,
  uint16 Offset
);
/**
 * \brief Service to verify the StatusSecured SubTLV.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] Length - Length of the received frame.
 * \param[in] Offset - Offset to where the respective SubTlv starts.
 *
 * \return E_OK: SubTLV data is valid.
 *         E_NOT_OK: Received and calculated Crc are not equal.
                     Slave is configured to CRC_NOT_VALIDATED.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyStatusSecuredSubTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length,
  uint16 Offset
);
/**
 * \brief Service to verify the Status NotSecured SubTLV.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] Length - Length of the received frame.
 * \param[in] Offset - Offset to where the respective SubTlv starts.
 *
 * \return E_OK: SubTLV data is valid.
 *         E_NOT_OK: Slave is configured to CRC_VALIDATED.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyStatusNotSecuredSubTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length,
  uint16 Offset
);

/**
 * \brief Service to verify the UserDataSecured SubTLV.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] Length - Length of the received frame.
 * \param[in] Offset - Offset to where the respective SubTlv starts.
 *
 * \return E_OK: SubTLV data is valid.
 *         E_NOT_OK: Received and calculated Crc are not equal.
                     Slave is configured to CRC_NOT_VALIDATED.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyUserDataSecuredSubTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length,
  uint16 Offset
);

/**
 * \brief Service to verify the UserData NotSecured SubTLV.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] Length - Length of the received frame.
 * \param[in] Offset - Offset to where the respective SubTlv starts.
 *
 * \return E_OK: SubTLV data is valid.
 *         E_NOT_OK: Slave is configured to CRC_VALIDATED.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyUserDataNotSecuredSubTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length,
  uint16 Offset
);

/**
 * \brief Service to verify the Ofs NotSecured SubTLV.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] Length - Length of the received frame.
 * \param[in] Offset - Offset to where the respective SubTlv starts.
 *
 * \return E_OK: SubTLV data is valid.
 *         E_NOT_OK: Slave is configured to CRC_VALIDATED.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyOfsNotSecuredSubTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length,
  uint16 Offset
);

/**
 * \brief Service to verify the OfsSecured SubTLV.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] Length - Length of the received frame.
 * \param[in] Offset - Offset to where the respective SubTlv starts.
 *
 * \return E_OK: SubTLV data is valid.
 *         E_NOT_OK: Received and calculated Crc are not equal.
                     Slave is configured to CRC_NOT_VALIDATED.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyOfsSecuredSubTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length,
  uint16 Offset
);

/**
 * \brief Service to save the received OfsSubTLV data .
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to store the received FUp information.
 * \param[in] Offset - Offset to where the respective SubTlv starts.
 *
 * \return E_OK: SubTLV data is valid.
 *         E_NOT_OK: SubTLV data is invalid.
 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SaveOfsData
(
 uint8 EthTSynCtrlIdx,
 P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
 uint16 Offset
);
#endif /* ETHTSYN_MAX_SLAVE != 0U */
#endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */
#endif /* (ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF) */
/**
 * \brief Service to reset sync state machine.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ResetSynTxStateMachine
(
  uint8 EthTSynCtrlIdx
);

/**
 * \brief Service to perform TSyn master main function tasks
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_MainFunctionTimeSynMaster
(
  uint8 EthTSynCtrlIdx
);

#if(ETHTSYN_MAX_SLAVE != 0U)
/**
 * \brief Service to perform TSyn slave main function tasks
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_MainFunctionTimeSynSlave
(
  uint8 EthTSynCtrlIdx
);
#endif

/**
 * \brief Service of the mainfunction
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ProcessFrame2Transmit(void);

/**
 * \brief Service of the mainfunction
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TriggerPeriodicFrames(void);

/**
 * \brief Service to perform transmit frame after debounce was perfomed
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TransmitFrameWithDebounce
(
  uint8 EthTSynCtrlIdx
);

/**
 * \brief Function used to process the received frame.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] DataPtr - Address of the received payload.
 * \param[in] PhysAddrPtr - Pointer to physical source address.
 * \param[in] LenByte - Length of the payload contained in the received Rx buffer.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ProcessRxMsg
(
 uint8 EthTSynCtrlIdx,
 P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
 P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PhysAddrPtr,
 uint16 LenByte
);

/**
 * \brief Service to process incoming Pdelay request frames.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] DataPtr - Pointer to the payload of the frame.
 * \param[in] LenByte - Length of the payload contained in the received Rx buffer.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ProcessRxPdelayReqFrame
(
 uint8 EthTSynCtrlIdx,
 P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
 uint16 LenByte
);

#if(ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT == STD_ON)
/**
 * \brief Service to verify the Authentication Challenge TLV for Device Authentication.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] DataPtr - Address of the received payload.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_PdelayDevAuth
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr
);

/**
 * \brief  Service to fill the Authentication Challenge TLV information
 *         of the pdelayresp/pdelayresp_fup frames.
 *
 * \param[in] EthTSynCtrlIdx - Index of the EthTSyn controller.
 * \param[in] DataPtr - Address of the buffer to be filled.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillAuthChallengeTLV
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
);
#endif

#define ETHTSYN_STOP_SEC_CODE
#include <EthTSyn_MemMap.h>
/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/
#define ETHTSYN_START_SEC_CONST_UNSPECIFIED
#include <EthTSyn_MemMap.h>

#if (ETHTSYN_MAX_SLAVE != 0U)
/** \brief Ns Array to time stamp conversion table
 *
 *  This table contains the converted value in seconds and nanoseconds (< 1s) of:
 *  Index 0: 2^32ns
 *  Index 1: 2^33ns
 *  ...
 *  Index 15: 2^47ns
 *
 *  Type sorted by nanoseconds, seconds, secondsHi.
 */
 /* Deviation MISRAC2012-2 */
STATIC CONST(Eth_TimeStampType, ETHTSYN_CONST) EthTSyn_NsArrayConversionTable[16U] =
{
  {294967296U, 4U, 0U},
  {589934592U, 8U, 0U},
  {179869184U, 17U, 0U},
  {359738368U, 34U, 0U},
  {719476736U, 68U, 0U},
  {438953472U, 137U, 0U},
  {877906944U, 274U, 0U},
  {755813888U, 549U, 0U},
  {511627776U, 1099U, 0U},
  {23255552U, 2199U, 0U},
  {46511104U, 4398U, 0U},
  {93022208U, 8796U, 0U},
  {186044416U, 17592U, 0U},
  {372088832U, 35184U, 0U},
  {744177664U, 70368U, 0U},
  {488355328U, 140737U, 0U}
};
#endif

#define ETHTSYN_STOP_SEC_CONST_UNSPECIFIED
#include <EthTSyn_MemMap.h>
/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define ETHTSYN_START_SEC_VAR_INIT_8
#include <EthTSyn_MemMap.h>

#define ETHTSYN_STOP_SEC_VAR_INIT_8
#include <EthTSyn_MemMap.h>

#define ETHTSYN_START_SEC_VAR_INIT_UNSPECIFIED
#include <EthTSyn_MemMap.h>

/** \brief Pointer to access the configuration structure. */
STATIC P2CONST(EthTSyn_ConfigType, ETHTSYN_VAR, ETHTSYN_APPL_CONST) EthTSyn_RootCfgPtr = NULL_PTR;

#define ETHTSYN_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <EthTSyn_MemMap.h>

#define ETHTSYN_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <EthTSyn_MemMap.h>

/** \brief Master/slave common controller variables. */
STATIC VAR(EthTSyn_ControllerType, ETHTSYN_VAR) EthTSyn_Ctrl[ETHTSYN_MAX_CTRL];

#if (ETHTSYN_MAX_SLAVE != 0U)
/** \brief Slave controller variables. */
STATIC VAR(EthTSyn_ControllerSlaveType, ETHTSYN_VAR) EthTSyn_Slave[ETHTSYN_MAX_SLAVE];

/** \brief Stores the extracted SYNC_TO_GATEWAY  bit */
/* Deviation MISRAC2012-2 */
STATIC VAR(uint8, ETHTSYN_VAR) EthTSyn_SyncToGatewayBit;

#endif /* ETHTSYN_MAX_SLAVE != 0U */

#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)

/** \brief Global time base status */
STATIC VAR(EthTSyn_ControllerType, ETHTSYN_VAR) EthTSyn_MasterTimeBaseStatus;

#endif

STATIC VAR(boolean, ETHTSYN_VAR) EthTSyn_Frame2Transmit;

/** \brief Stores the number of mainfunction() calls */
VAR(uint32, ETHTSYN_VAR) EthTSyn_GeneralCounter;

/** \brief Stores the minimum debounceoffset */
STATIC VAR(uint32, ETHTSYN_VAR) GlobalMinDebounceTimeOffset;

#if(ETHTSYN_SWITCH_USED == STD_ON)
STATIC EthTSyn_ControllerSwtPortType EthTSyn_SwitchPorts[ETHTSYN_SWITCH_PORTS_CONFIGURED];
#endif

/* Deviation MISRAC2012-3 <+10>*/
#if (defined ETHTSYN_TST_RUNTIME_DATA_EXPOSE_PTR)
#if (ETHTSYN_TST_RUNTIME_DATA_EXPOSE_PTR != 0)
#if(ETHTSYN_SWITCH_USED == STD_ON)
CONSTP2VAR(EthTSyn_ControllerSwtPortType, ETHTSYN_CONST, ETHTSYN_APPL_DATA) EthTSyn_TS_SwitchPorts = EthTSyn_SwitchPorts;
#endif
CONSTP2VAR(EthTSyn_ControllerType, ETHTSYN_CONST, ETHTSYN_APPL_DATA) EthTSyn_TS_Ctrl = EthTSyn_Ctrl;
#if (ETHTSYN_MAX_SLAVE != 0U)
CONSTP2VAR(EthTSyn_ControllerSlaveType, ETHTSYN_CONST, ETHTSYN_APPL_DATA) EthTSyn_TS_Slave = EthTSyn_Slave;
#endif
#endif /* TS_ETHTSYN_RUNTIME_DATA_EXPOSE_PTR != 0 */
#endif /* ifdef TS_ETHTSYN_RUNTIME_DATA_EXPOSE_PTR */

#define ETHTSYN_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <EthTSyn_MemMap.h>

/*==================[external function definitions]=========================*/
#define ETHTSYN_START_SEC_CODE
#include <EthTSyn_MemMap.h>

#if(ETHTSYN_VERSION_INFO_API == STD_ON)
/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00036, 1 */
FUNC(void, ETHTSYN_CODE) EthTSyn_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETHTSYN_APPL_DATA) VersionInfo
)
{
  DBG_ETHTSYN_GETVERSIONINFO_ENTRY(VersionInfo);

#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  if(VersionInfo == NULL_PTR)
  {
    /* !LINKSTO EthTSyn.EB.GetVersionInfo.NullPtr, 1 */
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_GETVERSIONINFO, ETHTSYN_E_PARAM_POINTER);
  }
  else
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
  {
    /* Return version information */
    /* !LINKSTO EthTSyn.EB.GetVersionInfo, 1 */
    VersionInfo->vendorID = ETHTSYN_VENDOR_ID;
    VersionInfo->moduleID = ETHTSYN_MODULE_ID;
    VersionInfo->sw_major_version = ETHTSYN_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = ETHTSYN_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = ETHTSYN_SW_PATCH_VERSION;
  }

  DBG_ETHTSYN_GETVERSIONINFO_EXIT(VersionInfo);
}
#endif /* ETHTSYN_VERSION_INFO_API */

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00035, 1 */
FUNC(void, ETHTSYN_CODE) EthTSyn_Init
(
  P2CONST(EthTSyn_ConfigType, AUTOMATIC, ETHTSYN_APPL_CONST) CfgPtr
)
{
  uint8 EthTSynCtrlIdx;

  DBG_ETHTSYN_INIT_ENTRY(CfgPtr);

  SchM_Enter_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_0();

  EthTSyn_RootCfgPtr = CfgPtr;

  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00006, 1 */
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00008, 1 */

  if(CfgPtr == NULL_PTR)
  {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00030_ValueCheck_INIT_FAILED, 1 */
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_INIT, ETHTSYN_E_INIT_FAILED);
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
  }
  else
  {
#if (ETHTSYN_MAX_SLAVE != 0U)
    for(
         EthTSynCtrlIdx = 0U;
         EthTSynCtrlIdx < EthTSyn_RootCfgPtr->MaxEthTSynSlaveIdx;
         EthTSynCtrlIdx++
       )
    {
      EthTSyn_Init_SlaveVar(EthTSynCtrlIdx);
    }
#endif /* ETHTSYN_MAX_SLAVE != 0U */
    for(
         EthTSynCtrlIdx = 0U;
         EthTSynCtrlIdx < EthTSyn_RootCfgPtr->MaxEthTSynCtrlIdx;
         EthTSynCtrlIdx++
       )
    {
      EthTSyn_Init_CtrlVar(EthTSynCtrlIdx);
      /* !LINKSTO EthTSyn.Dsn.CtrlSynTxState_Init, 1 */
      /* !LINKSTO EthTSyn.Dsn.CtrlSynBridge_INIT-IDLE, 1 */
      EthTSyn_ResetSynTxStateMachine(EthTSynCtrlIdx);
    }

    /* Init global counter */
    EthTSyn_GeneralCounter = 0U;

    /* Maximum */
    GlobalMinDebounceTimeOffset = ETHTSYN_INV_DEBOUNCE_VALUE;
  }

  SchM_Exit_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_0();

  DBG_ETHTSYN_INIT_EXIT(CfgPtr);
}

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00044, 1 */
FUNC(void, ETHTSYN_CODE) EthTSyn_MainFunction(void)
{
  DBG_ETHTSYN_MAINFUNCTION_ENTRY();

  /* !LINKSTO EthTSyn.EB.MainFunction.Uninit, 1 */
  if(!ETHTSYN_UNINIT)
  {
    uint8 EthTSynCtrlIdx;
    EthTSyn_GeneralCounter++;
    /*check for smallest */
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00130, 1 */
    if(GlobalMinDebounceTimeOffset == EthTSyn_GeneralCounter)
    {
      GlobalMinDebounceTimeOffset = ETHTSYN_INV_DEBOUNCE_VALUE;

      for(
         EthTSynCtrlIdx = 0U;
         EthTSynCtrlIdx < EthTSyn_RootCfgPtr->MaxEthTSynCtrlIdx;
         EthTSynCtrlIdx++
       )
      {
        /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00133, 1 */
        /* !LINKSTO EthTSyn.EB.TransparentClock_DebounceValueElapse, 1 */
        if(EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset == EthTSyn_GeneralCounter)
        {
          EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset = ETHTSYN_INV_DEBOUNCE_VALUE;
        }
        else
        {
          (void)EthTSyn_NextMinDebounceOffset
                (&GlobalMinDebounceTimeOffset,
                 EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset
                );
        }
      }
    }

    EthTSyn_TriggerPeriodicFrames();

    if(EthTSyn_Frame2Transmit)
    {
      TS_AtomicAssign8(EthTSyn_Frame2Transmit, FALSE);
      EthTSyn_ProcessFrame2Transmit();
    }
  }
  DBG_ETHTSYN_MAINFUNCTION_EXIT();
}

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00040, 1 */
FUNC(void, ETHTSYN_CODE) EthTSyn_RxIndication
(
  uint8 CtrlIdx,
  Eth_FrameType FrameType,
  boolean IsBroadcast,
  /* Deviation MISRAC2012-1 */
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PhysAddrPtr,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 LenByte
)
{
  DBG_ETHTSYN_RXINDICATION_ENTRY(CtrlIdx, FrameType, IsBroadcast, PhysAddrPtr, DataPtr, LenByte);

#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO EthTSyn.EB.RxIndication.Uninit, 1 */
  if(ETHTSYN_UNINIT)
  {
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_RXINDICATION, ETHTSYN_E_UNINIT);
  }
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00041_PARAMPTR, 1 */
  else if(PhysAddrPtr == NULL_PTR)
  {
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_RXINDICATION, ETHTSYN_E_PARAM_POINTER);
  }
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00041_PARAMPTR, 1 */
  else if(DataPtr == NULL_PTR)
  {
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_RXINDICATION, ETHTSYN_E_PARAM_POINTER);
  }
  /* !LINKSTO EthTSyn.EB.RxIndication.InvalidHeaderSize, 1 */
  else if(ETHTSYN_PTP_HEADER_SIZE > LenByte)
  {
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_RXINDICATION, ETHTSYN_E_PARAM);
  }
  else
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
  {
    /* !LINKSTO EthTSyn.EB.RxIndication.InvalidFrameType_DetOFF, 1 */
    if(ETHTSYN_PTP_FRAMETYPE == FrameType)
    {
      uint8 EthTSynCtrlIdx;
      Std_ReturnType RetVal;

      TS_PARAM_UNUSED(IsBroadcast);
      TS_PARAM_UNUSED(FrameType);

      EthTSynCtrlIdx = 0U; /* Initialize to get rid of warning. */
      RetVal = EthTSyn_TranslateEthIfCtrlIdx(CtrlIdx, &EthTSynCtrlIdx);

      if(E_OK == RetVal)
      {
#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
        /* !LINKSTO EthTSyn.EB.RxIndication.InvalidDomainNumber, 1 */
        if(DataPtr[ETHTSYN_PTP_HEADER_B4_DOMAIN] == EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SyncGlobalTimeDomainId)
#else
        /* CHECK: NOPARSE */
        if(DataPtr[ETHTSYN_PTP_HEADER_B4_DOMAIN] == ETHTSYN_PTP_HEADER_DOMAIN)
        /* CHECK: PARSE */
#endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */
        {
          EthTSyn_ProcessRxMsg(EthTSynCtrlIdx, DataPtr, PhysAddrPtr, LenByte);
        }
      }
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
      else
      {
        /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00041_CTRLIDX, 1 */
        /* Throw a DET error if the input parameter "CtrlIdx" is not valid */
        ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_RXINDICATION, ETHTSYN_E_CTRL_IDX);
      }
#endif
    }
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    else
    {
      /* !LINKSTO EthTSyn.EB.RxIndication.InvalidFrameType_DetON, 1 */
      ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_RXINDICATION, ETHTSYN_E_PARAM);
    }
#endif
  }

  DBG_ETHTSYN_RXINDICATION_EXIT(CtrlIdx, FrameType, IsBroadcast, PhysAddrPtr, DataPtr, LenByte);
}

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00042, 1 */
FUNC(void, ETHTSYN_CODE) EthTSyn_TxConfirmation
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx
)
{
  Std_ReturnType TmpRet;
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  Eth_TimeStampType TmpTimeStamp;

  TmpTimeStamp.nanoseconds = 0U;
  TmpTimeStamp.seconds = 0U;
  TmpTimeStamp.secondsHi = 0U;
#else
  StbM_VirtualLocalTimeType TmpTimeStamp;

  TmpTimeStamp.nanosecondsLo = 0U;
  TmpTimeStamp.nanosecondsHi = 0U;
#endif
  DBG_ETHTSYN_TXCONFIRMATION_ENTRY(CtrlIdx, BufIdx);

#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO EthTSyn.EB.TxConfirmation.Uninit, 1 */
  if(ETHTSYN_UNINIT)
  {
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_TXCONFIRMATION, ETHTSYN_E_UNINIT);
  }
  else
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00175, 1 */
  /* No DET shall be reported if EthTSyn_TxConfirmation() is called with invalid controller ID */
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
  {
    uint8 EthTSynCtrlIdx;

    for(
         EthTSynCtrlIdx = 0U;
         EthTSynCtrlIdx < EthTSyn_RootCfgPtr->MaxEthTSynCtrlIdx;
         EthTSynCtrlIdx++
       )
    {
      if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx == CtrlIdx)
      {
        if(EthTSyn_Ctrl[EthTSynCtrlIdx].Sync_BufIdx == BufIdx)
        {
          EthTSyn_CtrlSyncTxStateType SyncTxState;

          /* Get sync frame egress time stamp. */
          /* !LINKSTO EthTSyn.EB.Master.SendSyncFU, 1 */
          /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00127, 1 */
          TmpRet = EthTSyn_GetEgressTimeStamp
          (
            EthTSynCtrlIdx,
            (Eth_BufIdxType)EthTSyn_Ctrl[EthTSynCtrlIdx].Sync_BufIdx, /* BufIdx */
            &TmpTimeStamp /* TimeStampPtr */
          );
          TS_AtomicAssign8
          (
            SyncTxState,
            EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState
          );

          if(TmpRet == E_OK)
          {
            /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00127, 1 */
            TmpRet = EthTSyn_CalculateOriginTimeStamp(EthTSynCtrlIdx, &TmpTimeStamp);
          }

          if((TmpRet == E_OK) && (ETHTSYN_CTRL_SYN_TX_STATE_WAIT_FOR_ETH_EGRESS_TS == SyncTxState))
          {
            if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount == 0U)
            {
              TS_AtomicAssign8
              (
                EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState,
                ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_FUP
              );
            }
#if(ETHTSYN_SWITCH_USED == STD_ON)
            else
            {
              /* !LINKSTO EthTSyn.EB.BoundaryClock_TransmitSyncOnMasterPorts, 1 */
#if(ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF)
              /* !LINKSTO EthTSyn.Dsn.CtrlSynTxState_WAIT_EGRESS-READY_SYN_TO_PORT, 1 */
              TS_AtomicAssign8
              (
                EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState,
                ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_SYN_TO_PORT
              );
#else
              /* If EthSwt will handle the distribution of sync frames to non host ports
                 (EthTSynSendSyncFrameOnlyOnHostPort set to true) EthTSyn will take no action until
                 a valid ingress timestamp for the host port. */
              /* !LINKSTO EthTSyn.EB.EthTSynSendSyncFrameOnlyOnHostPort, 1 */
              TS_AtomicAssign8
              (
                EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState,
                ETHTSYN_CTRL_SYN_TX_STATE_WAIT_FOR_SWT_INGRESS_TS
              );
#endif /* ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF */
            }
#endif /* ETHTSYN_SWITCH_USED == STD_ON */
          }
          else
          {
            EthTSyn_ResetSynTxStateMachine(EthTSynCtrlIdx);
          }
          /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if((ETHTSYN_TIME_RECORDING == STD_ON) && (ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE))
          /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00214, 1 */
          /* If time measurement with switches is not used, we can inform StbM, else EthTSyn will pass the master
             information on the transmission of each fup frame, because only at that moment the correctionField
             will be available. */
          if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
          {
            (void)StbM_EthSetMasterTimingData
              (
                  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId,
                  /* !LINKSTO EthTSyn.ASR20-11.SWS_EthTSyn_00215, 1 */
                  &EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording
              );
          }
#endif
          EthTSyn_Ctrl[EthTSynCtrlIdx].Sync_BufIdx = ETHTSYN_INV_BUFIDX;
        }
        else if(EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResp_BufIdx == BufIdx)
        {
          /* !LINKSTO EthTSyn.EB.Master.SendPdelayResponseFU, 1 */
          /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00159, 1 */
          /* !LINKSTO EthTSyn.EB.InvTimeQuality.PdelayReqEgress, 1 */
          TmpRet = EthTSyn_GetEgressTimeStamp
          (
            EthTSynCtrlIdx,
            (Eth_BufIdxType)EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResp_BufIdx, /* BufIdx */
            &TmpTimeStamp /* TimeStampPtr */
          );

           /* Set the flag */
           if(E_OK == TmpRet)
           {
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
              EthTSyn_Ctrl[EthTSynCtrlIdx].EgressTimeStamp_PdelayT3 = TmpTimeStamp;
#else /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
              /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00122, 1 */
              EthTSyn_Int01_ConvertVirtualTimeToEthTime(&TmpTimeStamp,
                                           &EthTSyn_Ctrl[EthTSynCtrlIdx].EgressTimeStamp_PdelayT3);
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
#if(ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT == STD_ON)
              if(EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqReceivedWithAuthChallTlv == TRUE)
              {
                /* !LINKSTO EthTSyn.EB.DevAuth_PdelayRespTxConf, 1 */
                TmpRet = DevAuth_PDelayResIndication
                    (
                        EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx,
                        &EthTSyn_Ctrl[EthTSynCtrlIdx].EgressTimeStamp_PdelayT3,
                        EthTSyn_Ctrl[EthTSynCtrlIdx].RequestNonce
                    );
                if(E_OK != TmpRet)
                {
                  /* !LINKSTO EthTSyn.EB.DevAuth_PDelayResIndication_NOK, 1 */
                  /* The authentication challenge TLV will not be sent in the pdelay_resp_fup frame */
                  TS_AtomicAssign8(
                                    EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqReceivedWithAuthChallTlv,
                                    FALSE
                                  );
                }
              }
#endif/* ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT == STD_ON */
                ETHTSYN_SET_TXFLAG
                 (&EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, ETHTSYN_BITPOS_PDELAYRESP_FUP, TRUE);
           }
        }
        else if(EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReq_BufIdx == BufIdx)
        {
#if (ETHTSYN_MAX_SLAVE != 0U)
          if(TRUE == EthTSyn_IsSlave(EthTSynCtrlIdx)) /* Slave */
          {
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF)
            /* Store t1 (See 802.1AS-2011 Ch 11.1.2). */
            /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00013, 1 */
            /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00123, 1 */
            TmpRet = EthTSyn_GetEgressTimeStamp
            (
              EthTSynCtrlIdx,
              (Eth_BufIdxType)EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReq_BufIdx, /* BufIdx */
              &TmpTimeStamp /* TimeStampPtr */
            );

            /* !LINKSTO EthTSyn.EB.Slave.StbMSync.GetCurrentTime.NotOk, 1 */
            if(E_OK == TmpRet)
            {
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
              EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t1 = TmpTimeStamp;
#else /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
              EthTSyn_Int01_ConvertVirtualTimeToEthTime(&TmpTimeStamp,
                                              &EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t1);
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
              /* TxConfirmation was received */
              TS_AtomicSetBit_8
              (
                &EthTSyn_Slave[EthTSynCtrlIdx].CalculatePdelay,
                ETHTSYN_BITPOS_PDELAYREQ_EGRESSCONF
              );
            }
            else
            {
              /* Something is wrong, abort the pdelay calculation */
              /* !LINKSTO EthTSyn.EB.Slave.NoConfirmationForPdelayReq, 1 */
              EthTSyn_ResetSlavePdelayData(EthTSynCtrlIdx);
            }
          /* The Pdelay_Req was transmitted, wait for the egress timestamp */
#endif /* ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF */
            EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReq_BufIdx = ETHTSYN_INV_BUFIDX;
          }
#endif /* ETHTSYN_MAX_SLAVE != 0U */
        }
#if((ETHTSYN_TIME_RECORDING == STD_ON) && (ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE))
        else if(EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayRespFup_BufIdx == BufIdx)
        {
          /* The confirmation for a pdelay_resp_fup frame is expected only when the
             timevalidation is enabled for the timedomain. */
          /* Set the recorded timing measurement data for a pdelay of the
           * corresponding Synchronized Time Base on Time Master */
          /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00219, 1 */
          (void)StbM_EthSetPdelayResponderData
              (
                  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId,
                  /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00220, 1 */
                  &EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording
              );
           EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayRespFup_BufIdx = ETHTSYN_INV_BUFIDX;
        }
#endif
        else
        {
          /* Sync_Follow_Up, Pdelay_Resp_Follow_Up */
          /* Nothing to do for these frames. */
        }
      }
    }
  }

  DBG_ETHTSYN_TXCONFIRMATION_EXIT(CtrlIdx, BufIdx);
}

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00043, 1 */
FUNC(void, ETHTSYN_CODE) EthTSyn_TrcvLinkStateChg
(
  uint8 CtrlIdx,
  EthTrcv_LinkStateType LinkState
)
{
  DBG_ETHTSYN_TRCVLINKSTATECHG_ENTRY(CtrlIdx, LinkState);

#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO EthTSyn.EB.TrcvLinkStateChg.Uninit, 1 */
  if(ETHTSYN_UNINIT)
  {
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_TRCVLINKSTATECHG, ETHTSYN_E_UNINIT);
  }
  else
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00174, 1 */
  /* No DET shall be reported if EthTSyn_TrcvLinkStateChg() is called with invalid controller ID */
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
  {
    uint8 EthTSynCtrlIdx;
    boolean UpdatePhysAddr = FALSE;
    uint8 DestMacAddr[ETHTSYN_MAC_ADD_SIZE];

    for(
         EthTSynCtrlIdx = 0U;
         EthTSynCtrlIdx < EthTSyn_RootCfgPtr->MaxEthTSynCtrlIdx;
         EthTSynCtrlIdx++
       )
    {
      if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx == CtrlIdx)
      {
        TS_AtomicAssign8(EthTSyn_Ctrl[EthTSynCtrlIdx].LinkState, LinkState);

        TS_MemCpy(DestMacAddr, EthTSyn_DestMacAddr, ETHTSYN_MAC_ADD_SIZE);

        UpdatePhysAddr = TRUE;
        if(ETHTRCV_LINK_STATE_ACTIVE != LinkState)
        {
#if(ETHTSYN_SWITCH_USED == STD_ON)
          EthTSyn_IntPortIdxType Idx;
#endif
          EthTSyn_ResetSynTxStateMachine(EthTSynCtrlIdx);
          /* Set timeout to 0 to trigger sync frame as soon as link is up again. */
          EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextSyncFrame_Timeout = 0U;
          TS_AtomicAssign8(EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, 0U);
#if(ETHTSYN_SWITCH_USED == STD_ON)
          for
          (
            Idx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart;
            Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
            EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
            Idx++
          )
          {
            TS_AtomicAssign8(EthTSyn_SwitchPorts[Idx].Frame2Transmit, 0U);
          }
#endif
        }
      }
    }

    if(UpdatePhysAddr == TRUE)
    {
      if(ETHTRCV_LINK_STATE_ACTIVE == LinkState)
      {
        /* !LINKSTO EthTSyn.EB.UpdatePhysAddrFilter_AddToFilter, 1 */
        (void)EthIf_UpdatePhysAddrFilter
        (
           CtrlIdx, /* EthIf CtrlIdx */
           DestMacAddr,
           ETH_ADD_TO_FILTER
        );
      }
      else
      {
        /* !LINKSTO EthTSyn.EB.UpdatePhysAddrFilter_RemoveFromFilter, 1 */
        (void)EthIf_UpdatePhysAddrFilter
        (
           CtrlIdx, /* EthIf CtrlIdx */
           DestMacAddr,
           ETH_REMOVE_FROM_FILTER
        );
      }
    }
  }

  DBG_ETHTSYN_TRCVLINKSTATECHG_EXIT(CtrlIdx, LinkState);
}

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00039, 1 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SetTransmissionMode
(
  uint8 CtrlIdx,
  EthTSyn_TransmissionModeType Mode
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHTSYN_SETTRANSMISSIONMODE_ENTRY(CtrlIdx, Mode);

#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO EthTSyn.EB.SetTransmissionMode.Uninit, 1 */
  if(ETHTSYN_UNINIT)
  {
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_SETTRANSMISSIONMODE, ETHTSYN_E_UNINIT);
  }
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00172_Mode, 1 */
  else if((Mode != ETHTSYN_TX_ON) && (Mode != ETHTSYN_TX_OFF))
  {
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_SETTRANSMISSIONMODE, ETHTSYN_E_PARAM);
  }
  else
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
  {
    uint8 EthTSynCtrlIdx;

    for(
         EthTSynCtrlIdx = 0U;
         EthTSynCtrlIdx < EthTSyn_RootCfgPtr->MaxEthTSynCtrlIdx;
         EthTSynCtrlIdx++
       )
    {
      if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx == CtrlIdx)
      {
        if(EthTSyn_Ctrl[EthTSynCtrlIdx].TxMode != Mode)
        {
#if(ETHTSYN_SWITCH_USED == STD_ON)
          EthTSyn_IntPortIdxType Idx;
#endif
          TS_AtomicAssign8(EthTSyn_Ctrl[EthTSynCtrlIdx].TxMode, Mode);
          TS_AtomicAssign8(EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, 0U);
#if(ETHTSYN_SWITCH_USED == STD_ON)
          for(
           Idx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart;
           Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
           EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
           Idx++
         )
         {
            TS_AtomicAssign8(EthTSyn_SwitchPorts[Idx].Frame2Transmit, 0U);
         }
#endif
        }
        RetVal = E_OK;
      }
    }
    if(RetVal == E_NOT_OK)
    {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
       /* Throw a DET error if the input parameter "CtrlIdx" is not valid */
       /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00172_CtrlIdx, 1 */
       ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_SETTRANSMISSIONMODE, ETHTSYN_E_CTRL_IDX);
#endif
    }
  }

  DBG_ETHTSYN_SETTRANSMISSIONMODE_EXIT(CtrlIdx, Mode);
}

#if(ETHTSYN_SWITCH_USED == STD_ON)
FUNC(void, ETHTSYN_CODE) EthTSyn_EthSwtPtpTimeStampIndication
(
  uint8 EthIfCtrlIdx,
  EthSwt_MgmtInfoType MgmtInfo,
  uint32 UniqueId,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) PortTimeStampPtr
)
{
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  if(ETHTSYN_UNINIT)
  {
    /* !LINKSTO EthTSyn.EB.EthTSyn_EthSwtPtpTimeStampIndication.Uninit, 1 */
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_ETHSWTPTPTIMESTAMPINDICATION, ETHTSYN_E_UNINIT);
  }
  else if(PortTimeStampPtr == NULL_PTR)
  {
    /* !LINKSTO EthTSyn.EB.EthTSyn_EthSwtPtpTimeStampIndication.NULL_PTR, 1 */
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_ETHSWTPTPTIMESTAMPINDICATION, ETHTSYN_E_PARAM_POINTER);
  }
  else
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
  {
    uint8 EthTSynCtrlIdx = 0U;
    EthTSyn_IntPortIdxType Idx;
    Std_ReturnType RetVal;

    RetVal = EthTSyn_TranslateEthIfCtrlIdx(EthIfCtrlIdx, &EthTSynCtrlIdx);

    if(E_OK == RetVal)
    {
      boolean Found = FALSE;
      for
      (
         Idx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart;
        ((Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
                EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart))
            && (FALSE == Found));
         Idx++
      )
      {
        EthTSyn_PortSyncTxStateType PortSynTxState;
        EthTSyn_PortSyncTxStateType PortPdelayTxState;
        TS_AtomicAssign8
        (
          PortSynTxState,
          EthTSyn_SwitchPorts[Idx].SyncTxState
        );

        TS_AtomicAssign8
        (
          PortPdelayTxState,
          EthTSyn_SwitchPorts[Idx].PdelayState
        );

        if(MgmtInfo.SwitchIdx == EthTSyn_RootCfgPtr->SwitchConfig[Idx].EthSwtIdx)
        {
          if(MgmtInfo.SwitchPortIdx == EthTSyn_RootCfgPtr->SwitchConfig[Idx].EthSwtPortIdx)
          {
            if(EthTSyn_SwitchPorts[Idx].UniqueId == UniqueId)
            {
              /* !LINKSTO EthTSyn.EB.EthSwt_GetTimeStamps, 1 */
              switch(PortSynTxState)
              {
                case ETHTSYN_PORT_SYN_TX_STATE_WAIT_EGRESS_TS:
                {
                  EthTSyn_SwitchPorts[Idx].SyncTimeStamp.nanoseconds = PortTimeStampPtr->nanoseconds;
                  EthTSyn_SwitchPorts[Idx].SyncTimeStamp.seconds = PortTimeStampPtr->seconds;
                  EthTSyn_SwitchPorts[Idx].SyncTimeStamp.secondsHi = PortTimeStampPtr->secondsHi;
#if(ETHTSYN_TIME_RECORDING == STD_ON) 
                  if(EthTSyn_RootCfgPtr->SwitchConfig[Idx].TimeValidationEnabled == TRUE)
                  {
                    /* Save the egress timestamp to be passed to the StbM. */
                    /* Convert the egress timestamp. */
                    (void)EthTSyn_Int01_ConvertEthTimeToVirtualTime(&EthTSyn_SwitchPorts[Idx].SyncTimeStamp,
                       &EthTSyn_SwitchPorts[Idx].SyncMasterTimeRecording.syncEgressTimestamp);
                  }
#endif /* ETHTSYN_TIME_RECORDING == STD_ON */
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
                  if(Idx == EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt)
                  {
                    /* For the host port, the only time when an egress timestamp is expected, is when the
                       GTM is not the MGM cpu. */
                    /* T5 - T4 */
                    /* !LINKSTO EthTSyn.EB.TransparentClock_SynchronizeT5T4_Invalid, 1 */
                    EthTSyn_SubTimeStamps(
                                   &EthTSyn_SwitchPorts[Idx].SyncTimeStamp, /* Egress on the host port */
                                   &EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].SyncTimeStamp, /* Ingress timestamp in the switch */
                                   &EthTSyn_Slave[EthTSynCtrlIdx].TimeDiffHostEgressSlaveIngress /* (T5 - T4 ) */
                                 );
                    if(EthTSyn_Slave[EthTSynCtrlIdx].TimeDiffHostEgressSlaveIngress.sign == TRUE)
                    {
                      TS_AtomicAssign8
                      (
                          EthTSyn_SwitchPorts[Idx].SyncTxState,
                          ETHTSYN_PORT_READY_FOR_TIME_SYNC
                      );
                    }
                  }
                  else
                  {
#endif
                    /* !LINKSTO EthTSyn.Dsn.CtrlSynTxState_CTRL_READY_FUP-PORT_READY_FUP, 1 */
                    /* !LINKSTO EthTSyn.Dsn.CtrlSynBridge_CTRL_READY-PORT_READY, 1 */
                    /* !LINKSTO EthTSyn.Dsn.PortSynTxState_WAIT_EGRESS-READY_FUP, 1 */
                    TS_AtomicAssign8
                    (
                        EthTSyn_SwitchPorts[Idx].SyncTxState,
                        ETHTSYN_PORT_SYN_TX_STATE_READY_FOR_FUP
                    );
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
                  }
#endif
                  Found = TRUE;
                  break;
                }
                case ETHTSYN_PORT_SYN_TX_STATE_WAIT_INGRESS_TS:
                {

                  EthTSyn_SwitchPorts[Idx].SyncTimeStamp.nanoseconds = PortTimeStampPtr->nanoseconds;
                  EthTSyn_SwitchPorts[Idx].SyncTimeStamp.seconds = PortTimeStampPtr->seconds;
                  EthTSyn_SwitchPorts[Idx].SyncTimeStamp.secondsHi = PortTimeStampPtr->secondsHi;
                  /* !LINKSTO EthTSyn.Dsn.CtrlSynTxState_WAIT_SWT_INGRESS-VALID_INGRESS, 1 */
                  /* !LINKSTO EthTSyn.Dsn.HostPortSynTxState_WAIT_INGRESS-VALID_INGRESS, 1 */
                  /* !LINKSTO EthTSyn.Dsn.CtrlSynBridge_WAIT_INGRESS-VALID_INGRESS, 1 */
                  TS_AtomicAssign8
                  (
                      EthTSyn_SwitchPorts[Idx].SyncTxState,
                      ETHTSYN_PORT_SYN_TX_STATE_VALID_INGRESS_TS
                  );
#if(ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_ON)
                  {
                    uint8 PortIndex;
                    for(
                        PortIndex = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart + 1U;
                        PortIndex < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
                            EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
                       PortIndex++
                     )
                    {
                     /* If EthTSynSendSyncFrameOnlyOnHostPort set to true and a valid ingress
                        timestamp was received set all the non host ports to
                        ETHTSYN_PORT_SYN_TX_STATE_WAIT_EGRESS_TS. */
                      TS_AtomicAssign8
                      (
                          EthTSyn_SwitchPorts[PortIndex].SyncTxState,
                          ETHTSYN_PORT_SYN_TX_STATE_WAIT_EGRESS_TS
                      );
                    }
                  }
#endif /* (ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_ON) */
                  Found = TRUE;
                  break;
                }
                /* CHECK: NOPARSE */
                default:
                {
                  /* This branch can not be reached since UniqueId will not match for other states. */
  #if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
                  ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
  #endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
                  break; /* Caution: this 'break' is required by MISRA-C:2012 */
                }
                /* CHECK: PARSE */
              }
              EthTSyn_SwitchPorts[Idx].UniqueId = ETHTSYN_UNIQUE_ID_INVALID;
            }
            else
            {
              if(EthTSyn_SwitchPorts[Idx].PdelayUniqueId == UniqueId)
              {
                /* !LINKSTO EthTSyn.EB.EthSwt_GetTimeStamps, 1 */
                switch(PortPdelayTxState)
                {
                  case ETHTSYN_PORT_PDELAY_WAIT_EGRESS_TS:
                  {
                    /* !LINKSTO EthTSyn.EB.SwitchPdelay.PdelayResp_SaveEgressTS, 1 */
                    EthTSyn_SwitchPorts[Idx].EgressPdelayFrameTimeStamp.nanoseconds = PortTimeStampPtr->nanoseconds;
                    EthTSyn_SwitchPorts[Idx].EgressPdelayFrameTimeStamp.seconds = PortTimeStampPtr->seconds;
                    EthTSyn_SwitchPorts[Idx].EgressPdelayFrameTimeStamp.secondsHi = PortTimeStampPtr->secondsHi;
                    /* Make sure that the transmit flag is set to true
                     * (this can be already set if a sync cycle is in progress) */
                    /* !LINKSTO EthTSyn.EB.SwitchPdelay.PdelayRespFup_Transmit, 1 */
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
                    if(Idx == EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave)
                    {
                      /* Save T1 */
                      EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t1 = EthTSyn_SwitchPorts[Idx].EgressPdelayFrameTimeStamp;
                      /* If the egress timestamp was received on the slave port, it means that a pdelay calculation was started */
                      TS_AtomicSetBit_8
                      (
                        &EthTSyn_Slave[EthTSynCtrlIdx].CalculatePdelay,
                        ETHTSYN_BITPOS_PDELAYREQ_EGRESSCONF
                      );
                      /* If a slave started the calculaiton of the pdelay(by sending the Pdelay_Req) there is no need to transmit
                       * any other message */
                      TS_AtomicAssign8
                      (
                        EthTSyn_SwitchPorts[Idx].PdelayState,
                        ETHTSYN_PORT_PDELAY_STATE_IDLE
                      );
#if(ETHTSYN_TIME_RECORDING == STD_ON)
                      if(EthTSyn_RootCfgPtr->SwitchConfig[Idx].TimeValidationEnabled == TRUE)
                      {
                        /* Converted value of the responseReceiptTimestamp */
                        (void)EthTSyn_Int01_ConvertEthTimeToVirtualTime
                            (
                                &EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t1,
                                &EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestOriginTimestamp
                            );
                      }
#endif /* ETHTSYN_TIME_RECORDING == STD_ON */

                    }
                    else
#endif
                    {
                      ETHTSYN_SET_TXFLAG
                        (&EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, ETHTSYN_BITPOS_TX_SWT, TRUE);
                      /* In the next mainfunction the pdelayrespFUp frame can be transmitted. */
                      /* !LINKSTO EthTSyn.EB.TimeAwareBridge_MasterTransmitPdelayRespFUp, 1 */
                      TS_AtomicSetBit_8
                      (
                        &EthTSyn_SwitchPorts[Idx].Frame2Transmit,
                        ETHTSYN_BITPOS_PDELAYRESP_FUP
                      );
                    }
                    Found = TRUE;
                    break;
                  }
                  /* CHECK: NOPARSE */
                  case ETHTSYN_PORT_PDELAY_WAIT_INGRESS_TS:
                  {
                    /* Not used */
                    break;
                  }
                  default:
                  {
                    /* This branch can not be reached since UniqueId will not match for other states. */
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
                    ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
                    break; /* Caution: this 'break' is required by MISRA-C:2012 */
                  }
                  /* CHECK: PARSE */
                }
                EthTSyn_SwitchPorts[Idx].PdelayUniqueId = ETHTSYN_UNIQUE_ID_INVALID;
              }
            }
          }
          /* !LINKSTO EthTSyn.EB.Master.InvalidUniqueId, 1 */
          /* else ignore */
        }
        /* !LINKSTO EthTSyn.EB.Master.InvalidSwitchId, 1 */
        /* else ignore */
      }
    }
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    else
    {
      /* !LINKSTO EthTSyn.EB.EthSwtPtpTimeStampIndication_InvalidControllerId, 1 */
      /* Throw a DET error if the input parameter "CtrlIdx" is not valid */
      ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_ETHSWTPTPTIMESTAMPINDICATION,
                               ETHTSYN_E_PARAM);
    }
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
  }
}

FUNC(boolean, ETHTSYN_CODE) EthTSyn_EthSwtPtpRxIndication
(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHTSYN_APPL_DATA) MgmtInfoPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) IngressTimeStampPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint8 EthIfCtrlIdx,
  uint16 Len,
  uint32 UniqueId
)
{
  boolean TimeStampRequired = FALSE;
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  if(ETHTSYN_UNINIT)
  {
    /* !LINKSTO EthTSyn.EB.EthTSyn_EthSwtPtpRxIndication.Uninit, 1 */
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_ETHSWTPTPRXINDICATION, ETHTSYN_E_UNINIT);
  }
  else if(MgmtInfoPtr == NULL_PTR)
  {
    /* !LINKSTO EthTSyn.EB.EthTSyn_EthSwtPtpRxIndication.NULL_PTR, 1 */
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_ETHSWTPTPRXINDICATION, ETHTSYN_E_PARAM_POINTER);
  }
  else if(DataPtr == NULL_PTR)
  {
    /* !LINKSTO EthTSyn.EB.EthTSyn_EthSwtPtpRxIndication.NULL_PTR, 1 */
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_ETHSWTPTPRXINDICATION, ETHTSYN_E_PARAM_POINTER);
  }
  else if(IngressTimeStampPtr == NULL_PTR)
  {
    /* !LINKSTO EthTSyn.EB.EthTSyn_EthSwtPtpRxIndication.NULL_PTR, 1 */
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_ETHSWTPTPRXINDICATION, ETHTSYN_E_PARAM_POINTER);
  }
  /* !LINKSTO EthTSyn.EB.EthTSyn_EthSwtPtpRxIndication.InvalidHeaderSize, 1 */
  else if(ETHTSYN_PTP_HEADER_SIZE > Len)
  {
    ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_ETHSWTPTPRXINDICATION, ETHTSYN_E_PARAM);
  }
  else
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
  {
    uint8 EthTSynCtrlIdx = 0U;
    Std_ReturnType RetVal;

    RetVal = EthTSyn_TranslateEthIfCtrlIdx(EthIfCtrlIdx, &EthTSynCtrlIdx);
    if(E_OK == RetVal)
    {
#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
      /* !LINKSTO EthTSyn.EB.EthTSyn_EthSwtPtpRxIndicationCRC.InvalidDomainNumber, 1 */
      if(DataPtr[ETHTSYN_PTP_HEADER_B4_DOMAIN] == EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SyncGlobalTimeDomainId)
#else
      /* !LINKSTO EthTSyn.EB.EthTSyn_EthSwtPtpRxIndication.InvalidDomainNumber, 1 */
      if(DataPtr[ETHTSYN_PTP_HEADER_B4_DOMAIN] == ETHTSYN_PTP_HEADER_DOMAIN)
#endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */
      {
        /* Process received message */
        TimeStampRequired = EthTSyn_Inc02_ProcessRxSwtMsg(MgmtInfoPtr, IngressTimeStampPtr, DataPtr, EthTSynCtrlIdx, Len, UniqueId);
      }
    }
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    else
    {
      /* !LINKSTO EthTSyn.EB.EthSwtPtpRxIndication_InvalidControllerId, 1 */
      /* Throw a DET error if the input parameter "CtrlIdx" is not valid */
      ETHTSYN_DET_REPORT_ERROR(ETHTSYN_SID_ETHSWTPTPRXINDICATION, ETHTSYN_E_PARAM);
    }
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
  }

  return TimeStampRequired;
}

#endif /* ETHTSYN_SWITCH_USED == STD_ON */

/*==================[internal function definitions]=========================*/

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_TranslateEthIfCtrlIdx
(
  uint8 EthIfCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) EthTSynCtrlIdxPtr
)
{
  uint8 EthTSynCtrlIdx;
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHTSYN_TRANSLATEETHIFCTRLIDX_ENTRY(EthIfCtrlIdx, EthTSynCtrlIdxPtr);

  for(
       EthTSynCtrlIdx = 0U;
       EthTSynCtrlIdx < EthTSyn_RootCfgPtr->MaxEthTSynCtrlIdx;
       EthTSynCtrlIdx++
     )
  {
    if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx == EthIfCtrlIdx)
    {
      *EthTSynCtrlIdxPtr = EthTSynCtrlIdx;
      RetVal = E_OK;
      break;
    }
  }

  DBG_ETHTSYN_TRANSLATEETHIFCTRLIDX_EXIT(RetVal, EthIfCtrlIdx, EthTSynCtrlIdxPtr);

  return RetVal;
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TransmitSyncFrame
(
  uint8 EthTSynCtrlIdx
)
{
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00061, 1 */
  Std_ReturnType RetVal;
  Eth_BufIdxType BufIdx = 0U;
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PTPPayloadPtr;
  EthTSyn_PTPHeaderType PTPHeader;

  DBG_ETHTSYN_TRANSMITSYNCFRAME_ENTRY(EthTSynCtrlIdx);

  /* Prepare time sync frame PTP header. */
  PTPHeader.Flags = ETHTSYN_PTP_HEADER_FLAG_TWOSTEP;
  PTPHeader.Flags |= ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE;
  PTPHeader.MsgType = ETHTSYN_PTP_HEADER_MSGTYPE_SYNC;
  PTPHeader.Control = ETHTSYN_PTP_HEADER_CONTROL_SYNC;

  /* Set sequence number. */
  /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00189, 1 */
  TS_AtomicAssign16(PTPHeader.SequNumber, EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber);

  PTPHeader.LogMsgInterval = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SyncTxMsgLogInterval;

  PTPHeader.MsgLength = ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_SYNC_PAYLOADSIZE;

  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00202, 1 */
  RetVal = EthTSyn_GetTxBuffer(EthTSynCtrlIdx, PTPHeader.MsgLength, &BufIdx, &PTPPayloadPtr);

  /* !LINKSTO EthTSyn.EB.EthIf_Return_NotOk, 2 */
  if(E_OK == RetVal)
  {
    /* Fill PTP header */
    EthTSyn_FillPTPHeader(EthTSynCtrlIdx, &PTPHeader, &PTPPayloadPtr[0U]);
    /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if((ETHTSYN_TIME_RECORDING == STD_ON) && (ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE))
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
  {
    /* Save SyncSequNumber to be reported to StbM for time recording */
    TS_AtomicAssign16(EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sequenceId,
                                                      EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber);
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity =
         (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0]) << 56U);
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
         (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1]) << 48U);
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
         (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2]) << 40U);
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
         (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3]) << 32U);
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
         (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4]) << 24U);
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
         (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5]) << 16U);
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
         (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6]) << 8U);
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
         (uint64)((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7]);
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.portNumber =
         (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8]);
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.portNumber |=
         (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9]);
  }
#endif
    /* Set PTP payload */
    /* All bytes are reserved. */
    TS_MemBZero(&PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE], ETHTSYN_PTP_SYNC_PAYLOADSIZE);

    EthTSyn_Ctrl[EthTSynCtrlIdx].Sync_BufIdx = BufIdx;

    /* Send frame */
    RetVal = EthTSyn_TransmitFrame(EthTSynCtrlIdx, BufIdx, PTPHeader.MsgLength, TRUE);

    /* !LINKSTO EthTSyn.EB.EthIf_Transmit_NotOk, 1 */
    if(RetVal == E_OK)
    {
      TS_AtomicAssign8
      (
        EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState,
        ETHTSYN_CTRL_SYN_TX_STATE_WAIT_FOR_ETH_EGRESS_TS
      );
    }
  }

  DBG_ETHTSYN_TRANSMITSYNCFRAME_EXIT(EthTSynCtrlIdx);
}


STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TransmitSyncFUpFrame
(
  uint8 EthTSynCtrlIdx
)
{
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00061, 1 */
  Std_ReturnType TmpRet;
  Eth_BufIdxType BufIdx = 0U;
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PTPPayloadPtr;
  EthTSyn_PTPHeaderType PTPHeader;
  uint16 SubTlvSize = 0U;

  DBG_ETHTSYN_TRANSMITSYNCFUPFRAME_ENTRY(EthTSynCtrlIdx);

  /* Prepare PTP header. */
  PTPHeader.Flags = ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE;
  PTPHeader.MsgType = ETHTSYN_PTP_HEADER_MSGTYPE_FUP;
  PTPHeader.Control = ETHTSYN_PTP_HEADER_CONTROL_FUP;

  /* Set sequence number. */
  /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00190, 1 */
  TS_AtomicAssign16(PTPHeader.SequNumber, EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber);

  /* Same value as sync frame (See 802.1AS-2011 Ch 11.4.2.8) */
  PTPHeader.LogMsgInterval = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SyncTxMsgLogInterval;

#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)

  /* Increment the frame size with 10 bytes */
  SubTlvSize += ETHTSYN_FUP_AUTOSAR_TLV_HEADER_LENGTH;

  /* If Time SubTLV is enabled, increment the frame size with 5 bytes */
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TLVFollowUpTimeSubTLV == TRUE)
  {
    SubTlvSize += ETHTSYN_FUP_SUBTLV_TIME_SIZE;
  }
  /* If Status SubTLV is enabled, increment the frame size with 4 bytes */
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TLVFollowUpStatusSubTLV == TRUE)
  {
    SubTlvSize += ETHTSYN_FUP_SUBTLV_STATUS_SIZE;
  }
  /* If UserData SubTLV is enabled, increment the frame size with 7 bytes */
  if(
      (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TLVFollowUpUserDataSubTLV == TRUE) &&
      (EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userDataLength  != 0U)
    )
  {
    SubTlvSize += ETHTSYN_FUP_SUBTLV_USERDATA_SIZE;
  }

  /* If OFS SubTLV is enabled, increment the frame size with 19 bytes */
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TLVFollowUpOFSSubTLV == TRUE)
  {
    SubTlvSize += ETHTSYN_FUP_SUBTLV_OFS_SIZE;
  }

  if(SubTlvSize == ETHTSYN_FUP_AUTOSAR_TLV_HEADER_LENGTH)
  {
    SubTlvSize = 0U;
  }
#endif

  PTPHeader.MsgLength = ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_SYNC_FUP_PAYLOADSIZE + SubTlvSize;

  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00203, 1 */
  TmpRet = EthTSyn_GetTxBuffer(EthTSynCtrlIdx, PTPHeader.MsgLength, &BufIdx, &PTPPayloadPtr);

  /* !LINKSTO EthTSyn.EB.EthIf_Return_NotOk, 2 */
  if(E_OK == TmpRet)
  {
    uint8 PayloadIdx = 0U;

    /* Fill PTP header */
    EthTSyn_FillPTPHeader(EthTSynCtrlIdx, &PTPHeader, &PTPPayloadPtr[PayloadIdx]);

    PayloadIdx += ETHTSYN_PTP_HEADER_SIZE; /* Set payload index to next free space. */

    /* Set PTP payload */
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00018, 1 */
    EthTSyn_TimeStampToArray
      (
        &EthTSyn_Ctrl[EthTSynCtrlIdx].OriginTimeStamp, &PTPPayloadPtr[PayloadIdx]
      );
    PayloadIdx += ETHTSYN_TIME_STAMP_SIZE; /* Set payload index to next free space. */

    /* Add IEEE TLV information. */
    EthTSyn_FillFUpTLV(&PTPPayloadPtr[PayloadIdx]);

#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
    if(SubTlvSize > 0U)
    {
      /* The AUTOSAR TLV is used */
      EthTSyn_FillFUpAutosarTLV(EthTSynCtrlIdx, &PTPPayloadPtr[0U]);
    }
#endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_ON) */

    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00203, 1 */
    /* Transmit frame: Return value is ignored since no repetition possible in case of error. */
    (void)EthTSyn_TransmitFrame(EthTSynCtrlIdx, BufIdx, PTPHeader.MsgLength, FALSE);
  }


  DBG_ETHTSYN_TRANSMITSYNCFUPFRAME_EXIT(EthTSynCtrlIdx);
}

#if(ETHTSYN_SEND_ANNOUNCEFRAMES == STD_ON)
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TransmitAnnounceFrame
(
  uint8 EthTSynCtrlIdx
)
{
  Std_ReturnType RetVal;
  Eth_BufIdxType BufIdx = 0U;
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PTPPayloadPtr;
  EthTSyn_PTPHeaderType PTPHeader;

  DBG_ETHTSYN_TRASNMITANNOUNCEFRAME_ENTRY(EthTSynCtrlIdx);

  /* Prepare time sync frame PTP header. */
  PTPHeader.Flags = ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE;
  PTPHeader.MsgType = ETHTSYN_PTP_HEADER_MSGTYPE_ANNOUNCE;
  PTPHeader.Control = ETHTSYN_PTP_HEADER_CONTROL_ANNOUNCE;

  /* Set sequence number. */
  TS_AtomicAssign16(PTPHeader.SequNumber, EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber);

  PTPHeader.LogMsgInterval = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SyncTxMsgLogInterval;

  PTPHeader.MsgLength = ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_ANNOUNCE_PAYLOADSIZE;

  RetVal = EthTSyn_GetTxBuffer(EthTSynCtrlIdx, PTPHeader.MsgLength, &BufIdx, &PTPPayloadPtr);

  /* !LINKSTO EthTSyn.EB.EthIf_Return_NotOk, 2 */
  if(E_OK == RetVal)
  {
    /* Fill PTP header */
    EthTSyn_FillPTPHeader(EthTSynCtrlIdx, &PTPHeader, &PTPPayloadPtr[0U]);

    /* Set PTP payload */
    EthTSyn_FillAnnouncePayload(EthTSynCtrlIdx, &PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE]);

    /* Send frame: Ignore return value. No error handling will be done. */
    (void) EthTSyn_TransmitFrame(EthTSynCtrlIdx, BufIdx, PTPHeader.MsgLength, FALSE);
  }
  DBG_ETHTSYN_TRASNMITANNOUNCEFRAME_EXIT(EthTSynCtrlIdx);
}
#endif /* ETHTSYN_SEND_ANNOUNCEFRAMES == STD_ON) */

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TransmitPdelayReqFrame
(
  uint8 EthTSynCtrlIdx
)
{
  Std_ReturnType TmpRet = E_NOT_OK;
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PTPPayloadPtr;
  EthTSyn_PTPHeaderType PTPHeader;
  uint16 PdelayReqSequNumber;
  Eth_BufIdxType BufIdx = 0U;

  DBG_ETHTSYN_TRANSMITPDELAYREQFRAME_ENTRY(EthTSynCtrlIdx);

  TS_AtomicAssign16(PdelayReqSequNumber, EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqSequNumber);
  /* Increment sequence number. The Sequence Counter shall wrap around at 65535 to 0 again. */
  /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00188, 1 */
  if(ETHTSYN_SEQ_NUM_MAX == PdelayReqSequNumber)
  {
    /* The first transmission of the Pdelay_Req frame shall contain seqId 0.*/
    /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00187_PdelayReq, 1 */
    PdelayReqSequNumber = 0U;
  }
  else
  {
    PdelayReqSequNumber++;
  }

  TS_AtomicAssign16(EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqSequNumber, PdelayReqSequNumber);

  /* Prepare PTP header. */
  PTPHeader.Flags = ETHTSYN_PTP_HEADER_NOFLAGS;
  PTPHeader.Flags |= ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE;
  PTPHeader.MsgType = ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_REQ;
  PTPHeader.Control = ETHTSYN_PTP_HEADER_CONTROL_PDELAY;

  /* Set sequence number. */
  PTPHeader.SequNumber = PdelayReqSequNumber;

  PTPHeader.LogMsgInterval
    = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayTxMsgLogInterval;

  PTPHeader.MsgLength = ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_REQ_PAYLOADSIZE;

  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00200, 1 */
  TmpRet = EthTSyn_GetTxBuffer(EthTSynCtrlIdx, PTPHeader.MsgLength, &BufIdx, &PTPPayloadPtr);

  /* !LINKSTO EthTSyn.EB.EthIf_Return_NotOk, 2 */
  if(E_OK == TmpRet)
  {
    /* Fill PTP header */
    EthTSyn_FillPTPHeader(EthTSynCtrlIdx, &PTPHeader, &PTPPayloadPtr[0U]);

    /* Set PTP payload */
    /* All bytes are reserved. */
    TS_MemBZero(&PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE], ETHTSYN_PTP_PDELAY_REQ_PAYLOADSIZE);

#if(ETHTSYN_MAX_SLAVE != 0U)
    if(TRUE == EthTSyn_IsSlave(EthTSynCtrlIdx))
    {
      /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if(ETHTSYN_TIME_RECORDING == STD_ON)
      if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
      {
        StbM_TimeStampType StbMTimeStamp;
        StbM_UserDataType UserData;
        /* Save the value of the Virtual Local Time of the reference Global Time Tuple
         *  - referenceLocalTimestamp*/
        /* Save the value of the local instance of the Global Time of the reference
         *  Global Time Tuple referenceGlobalTimestamp */
        /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00223, 1 */
        (void)StbM_BusGetCurrentTime
        (
            EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId, /* timeBaseId */
            &StbMTimeStamp,
            &EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.referenceLocalTimestamp,
            &UserData
        );
        EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.referenceGlobalTimestamp.
          nanoseconds = StbMTimeStamp.nanoseconds;
        EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.referenceGlobalTimestamp.
          seconds     = StbMTimeStamp.seconds;
        /* Save the Sequence Id of sent Pdelay_Req frame */
       TS_AtomicAssign8
        (
        EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.sequenceId,
        PdelayReqSequNumber
        );

       /* Save the sourcePortId of sent Pdelay_Req frame*/
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity =
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0]) << 56U);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1]) << 48U);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2]) << 40U);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3]) << 32U);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4]) << 24U);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5]) << 16U);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6]) << 8U);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity |=
           (uint64)((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7]);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.portNumber =
           (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8]);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.portNumber |=
           (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9]);
      }
#endif/* ETHTSYN_TIME_RECORDING == STD_ON */
    }
#endif/* ETHTSYN_MAX_SLAVE != 0U */
    /* Send frame */
    TmpRet = EthTSyn_TransmitFrame(EthTSynCtrlIdx, BufIdx, PTPHeader.MsgLength, TRUE);

    if(E_OK == TmpRet)
    {
      /* Set source port identity to find corresponding Pdelay response and Pdelay response follow
         up frames. */
      SchM_Enter_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_0();
      TS_MemCpy(
                 &EthTSyn_Ctrl[EthTSynCtrlIdx].Pdelay_Tx_SrcPortIdentity[0U],
                 &PTPPayloadPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0],
                 ETHTSYN_SOURCEPORTIDENTITY_SIZE
               );
      SchM_Exit_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_0();

      EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReq_BufIdx = BufIdx;

      /* Set wait timeout for frame to transmit. */
      EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextPdelayReqFrame_Timeout
        = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayReqTxInterval;
#if(ETHTSYN_MAX_SLAVE != 0U)
      if(TRUE == EthTSyn_IsSlave(EthTSynCtrlIdx))
      {
        /* A pdelay calculation is started */
        TS_AtomicSetBit_8
        (
          &EthTSyn_Slave[EthTSynCtrlIdx].CalculatePdelay,
          ETHTSYN_BITPOS_PDELAYREQ_SENT
        );
      }
#endif /* (ETHTSYN_MAX_SLAVE != 0U) */
#if (ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED != TS_PROD_ERR_DISABLE)
      /* Set wait timeout for receiving PdelayResp/PdelayResp_Fup frames. */
      EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayRespFupPairTimeoutInit = TRUE;
      EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayRespPairTimeout
        = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayRespFupPairsPeriod;
#endif
    }
  }

  DBG_ETHTSYN_TRANSMITPDELAYREQFRAME_EXIT(EthTSynCtrlIdx);

}

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00201, 1 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TransmitPdelayRespFrame
(
  uint8 EthTSynCtrlIdx
)
{
  Std_ReturnType TmpRet;
  Eth_BufIdxType BufIdx = 0U;
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PTPPayloadPtr;
  EthTSyn_PTPHeaderType PTPHeader;

  DBG_ETHTSYN_TRANSMITPDELAYRESPFRAME_ENTRY(EthTSynCtrlIdx, RxDataPtr);

  /* Prepare PTP header. */
  PTPHeader.Flags = ETHTSYN_PTP_HEADER_FLAG_TWOSTEP;
  PTPHeader.Flags |= ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE;
  PTPHeader.MsgType = ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_RESP;
  PTPHeader.Control = ETHTSYN_PTP_HEADER_CONTROL_PDELAY;

  /* Set sequence number. */
  /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00191_PdelayResp, 1 */
  TS_AtomicAssign16(
                     PTPHeader.SequNumber,
                     EthTSyn_Ctrl[EthTSynCtrlIdx].Ingress_PdelayReqSequNumber
                   );


  /* Reserved value (See 802.1AS-2011 Ch 11.4.2.8) */
  PTPHeader.LogMsgInterval = ETHTSYN_PTP_HEADER_LOGMSGINTERVAL_RESERVED;

  PTPHeader.MsgLength = ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_RESP_PAYLOADSIZE;

#if(ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT == STD_ON)
  if(EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqReceivedWithAuthChallTlv == TRUE)
  {
    /* !LINKSTO EthTSyn.EB.DevAuth_PdelayResp, 1 */
    PTPHeader.MsgLength += ETHTSYN_AUTH_CHALLENGE_TLV_SIZE;
  }
#endif

  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00012, 1 */
  TmpRet = EthTSyn_GetTxBuffer(EthTSynCtrlIdx, PTPHeader.MsgLength, &BufIdx, &PTPPayloadPtr);

  /* !LINKSTO EthTSyn.EB.EthIf_Return_NotOk, 2 */
  if(E_OK == TmpRet)
  {
    /* Fill PTP header */
    EthTSyn_FillPTPHeader(EthTSynCtrlIdx, &PTPHeader, &PTPPayloadPtr[0U]);

    TS_MemCpy(
               &PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_TIME_STAMP_SIZE],
               &EthTSyn_Ctrl[EthTSynCtrlIdx].Pdelay_Rx_SrcPortIdentity[0U],
               ETHTSYN_SOURCEPORTIDENTITY_SIZE
             );
      /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if((ETHTSYN_TIME_RECORDING == STD_ON) && (ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE))
      if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
      {
        StbM_UserDataType UserData;
        StbM_TimeStampType StbMTimeStamp;
        StbM_VirtualLocalTimeType StbMVirtualLocalTime;

        StbMVirtualLocalTime.nanosecondsLo = 0U;
        StbMVirtualLocalTime.nanosecondsHi = 0U;
        /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00218, 1 */
        (void)StbM_BusGetCurrentTime
          (
            EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId, /* timeBaseId */
            &StbMTimeStamp, /* timeStampPtr */
            &StbMVirtualLocalTime, /* localTimePtr */
            &UserData /* userDataPtr */
          );

        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.referenceLocalTimestamp.
            nanosecondsLo = StbMVirtualLocalTime.nanosecondsLo;
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.referenceLocalTimestamp.
            nanosecondsHi = StbMVirtualLocalTime.nanosecondsHi;
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.referenceGlobalTimestamp.
            nanoseconds = StbMTimeStamp.nanoseconds;
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.referenceGlobalTimestamp.
            seconds = StbMTimeStamp.seconds;

        /* Save the sourcePortId of received Pdelay_Req frame*/
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity =
             (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0]) << 56U);
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity |=
             (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1]) << 48U);
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity |=
             (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2]) << 40U);
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity |=
             (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3]) << 32U);
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity |=
             (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4]) << 24U);
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity |=
             (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5]) << 16U);
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity |=
             (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6]) << 8U);
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity |=
             (uint64)((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7]);
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responsePortId.portNumber =
             (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8]);
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responsePortId.portNumber |=
             (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9]);
      }
#endif

    /* Set PTP payload */
    EthTSyn_TimeStampToArray(&EthTSyn_Ctrl[EthTSynCtrlIdx].IngressTimeStamp,
                             &PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE]);


    EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResp_BufIdx = BufIdx;

#if(ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT == STD_ON)
    if(EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqReceivedWithAuthChallTlv == TRUE)
    {
      /* !LINKSTO EthTSyn.EB.DevAuth_TLV, 1 */
      EthTSyn_FillAuthChallengeTLV(EthTSynCtrlIdx,
        &PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_RESP_PAYLOADSIZE]);
    }
#endif

    /* Send frame: Return value ignored: No retransmission performed in error case. */
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00012, 1 */
    (void) EthTSyn_TransmitFrame(EthTSynCtrlIdx, BufIdx, PTPHeader.MsgLength, TRUE);
  }

  DBG_ETHTSYN_TRANSMITPDELAYRESPFRAME_EXIT(EthTSynCtrlIdx, RxDataPtr);
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TransmitPdelayRespFUpFrame
(
  uint8 EthTSynCtrlIdx
)
{
  Std_ReturnType TmpRet;
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PTPPayloadPtr;
  EthTSyn_PTPHeaderType PTPHeader;
  Eth_BufIdxType BufIdx = 0U;

  DBG_ETHTSYN_TRANSMITPDELAYRESPFUPFRAME_ENTRY(EthTSynCtrlIdx);

  /* Prepare PTP header. */
  PTPHeader.Flags = ETHTSYN_PTP_HEADER_NOFLAGS;
  PTPHeader.Flags |= ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE;
  PTPHeader.MsgType = ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_FUP;
  PTPHeader.Control = ETHTSYN_PTP_HEADER_CONTROL_PDELAY;

  /* Set sequence number. */
  /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00191_PdelayRespFup, 1 */
  TS_AtomicAssign16(
                     PTPHeader.SequNumber,
                     EthTSyn_Ctrl[EthTSynCtrlIdx].Ingress_PdelayReqSequNumber
                   );

  /* Reserved value (See 802.1AS-2011 Ch 11.4.2.8) */
  PTPHeader.LogMsgInterval = ETHTSYN_PTP_HEADER_LOGMSGINTERVAL_RESERVED;

  PTPHeader.MsgLength = ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_RESP_FUP_PAYLOADSIZE;

#if(ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT == STD_ON)
  if(EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqReceivedWithAuthChallTlv == TRUE)
  {
    /* !LINKSTO EthTSyn.EB.DevAuth_PdelayRespFUp, 1 */
    PTPHeader.MsgLength += ETHTSYN_AUTH_CHALLENGE_TLV_SIZE;
  }
#endif

  /* !LINKSTO EthTSyn.EB.Master.SendPdelayResponseFU, 1 */
  TmpRet = EthTSyn_GetTxBuffer(EthTSynCtrlIdx, PTPHeader.MsgLength, &BufIdx, &PTPPayloadPtr);

  /* !LINKSTO EthTSyn.EB.EthIf_Return_NotOk, 2 */
  if(E_OK == TmpRet)
  {
    /* Fill PTP header */
    EthTSyn_FillPTPHeader(EthTSynCtrlIdx, &PTPHeader, &PTPPayloadPtr[0U]);

    TS_MemCpy(
               &PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_TIME_STAMP_SIZE],
               &EthTSyn_Ctrl[EthTSynCtrlIdx].Pdelay_Rx_SrcPortIdentity[0U],
                ETHTSYN_SOURCEPORTIDENTITY_SIZE
             );
      /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if((ETHTSYN_TIME_RECORDING == STD_ON) && (ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE))
      if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
      {
        /* Save the buffer id */
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayRespFup_BufIdx = BufIdx;
      }
#endif
      /* Set PTP payload */
      EthTSyn_TimeStampToArray(&EthTSyn_Ctrl[EthTSynCtrlIdx].EgressTimeStamp_PdelayT3, &PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE]);
#if(ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT == STD_ON)
      if(EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqReceivedWithAuthChallTlv == TRUE)
      {
        /* !LINKSTO EthTSyn.EB.DevAuth_TLV, 1 */
        EthTSyn_FillAuthChallengeTLV(EthTSynCtrlIdx,
            &PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_RESP_FUP_PAYLOADSIZE]);
        /* Clear the flag */
        TS_AtomicAssign8(EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqReceivedWithAuthChallTlv, FALSE);
      }
#endif
    /* Send frame: Ignore return value since no retry possible. */
    /* !LINKSTO EthTSyn.EB.Master.SendPdelayResponseFU, 1 */
    (void) EthTSyn_TransmitFrame(EthTSynCtrlIdx, BufIdx, PTPHeader.MsgLength, FALSE);
  }
  /* Reset variables */
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResp_BufIdx = ETHTSYN_INV_BUFIDX;

  DBG_ETHTSYN_TRANSMITPDELAYRESPFUPFRAME_EXIT(EthTSynCtrlIdx);
}

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_GetTxBuffer
(
  uint8 EthTSynCtrlIdx,
  uint16 PayloadLength,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETHTSYN_APPL_DATA) BufIdxPtr,
  P2VAR(EthTSyn_PtrUint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
)
{
  uint16 BufferSize = PayloadLength;
  Std_ReturnType RetVal = E_NOT_OK;
  BufReq_ReturnType RetVal_BufReq;

  DBG_ETHTSYN_GETTXBUFFER_ENTRY(EthTSynCtrlIdx, PayloadLength, BufIdxPtr, BufPtr);

  /* Get Tx buffer. */
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00202, 1 */
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00162, 1 */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00163, 1 */
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00148, 1 */
  /* !LINKSTO EthTSyn.EB.Master.FrameType, 1 */
  RetVal_BufReq = EthIf_ProvideTxBuffer
  (
    EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx, /* EthIf CtrlIdx */
    ETHTSYN_PTP_FRAMETYPE, /* FrameType */
    EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].FramePriority, /* Priority */
    BufIdxPtr, /* BufIdxPtr */
    BufPtr, /* BufPtr */
    &BufferSize /* LenBytePtr */
  );

  if((PayloadLength <= BufferSize) && (BUFREQ_OK == RetVal_BufReq))
  {
    RetVal = E_OK;
  }

  DBG_ETHTSYN_GETTXBUFFER_EXIT(RetVal, EthTSynCtrlIdx, PayloadLength, BufIdxPtr, BufPtr);

  return RetVal;
}

/* !LINKSTO EthTSyn.ASR151.PRS_TS_00064, 1 */
/* !LINKSTO EthTSyn.ASR151.PRS_TS_00065_FUp, 1 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillPTPHeader
(
  uint8 EthTSynCtrlIdx,
  P2CONST(EthTSyn_PTPHeaderType, AUTOMATIC, ETHTSYN_APPL_DATA) PTPHeader,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
)
{
  uint8 SrcMacAddr[ETHTSYN_MAC_ADD_SIZE];

  DBG_ETHTSYN_FILLPTPHEADER_ENTRY(EthTSynCtrlIdx, PTPHeader, BufPtr);

  BufPtr[ETHTSYN_PTP_HEADER_B0_LN_MSGTYPE] = PTPHeader->MsgType & ETHTSYN_8BIT_MASK_LN;
  BufPtr[ETHTSYN_PTP_HEADER_B0_UN_TRANS_SPEC] |= ETHTSYN_PTP_HEADER_TRANS_SPEC_UN;
  BufPtr[ETHTSYN_PTP_HEADER_B1_PTPVERSION] = ETHTSYN_PTP_HEADER_VERSION;

  BufPtr[ETHTSYN_PTP_HEADER_B2_MSGLENGTH_1]
    = (uint8)((uint16)(PTPHeader->MsgLength & ETHTSYN_16BIT_MASK_MSB) >> 8U);
  BufPtr[ETHTSYN_PTP_HEADER_B3_MSGLENGTH_0]
   = (uint8)(PTPHeader->MsgLength & ETHTSYN_16BIT_MASK_LSB);

     /* Select the domain number for IEEE or AUTOSAR */
  switch(PTPHeader->MsgType)
  {
    case ETHTSYN_PTP_HEADER_MSGTYPE_SYNC: /* Fall through */
    case ETHTSYN_PTP_HEADER_MSGTYPE_FUP:
    {
#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_ON)
      BufPtr[ETHTSYN_PTP_HEADER_B4_DOMAIN] = ETHTSYN_PTP_HEADER_DOMAIN;
#else
      BufPtr[ETHTSYN_PTP_HEADER_B4_DOMAIN] =
      EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SyncGlobalTimeDomainId;
#endif
      break;
    }
    default:
    {
      /* Use IEEE as default */
      BufPtr[ETHTSYN_PTP_HEADER_B4_DOMAIN] = ETHTSYN_PTP_HEADER_DOMAIN;
      break;
    }
  }

  BufPtr[ETHTSYN_PTP_HEADER_B5_RESERVED] = ETHTSYN_PTP_HEADER_RESERVED;

  BufPtr[ETHTSYN_PTP_HEADER_B6_FLAGS_0] = (uint8)(PTPHeader->Flags & ETHTSYN_16BIT_MASK_LSB);
  BufPtr[ETHTSYN_PTP_HEADER_B7_FLAGS_1]
    = (uint8)((uint16)(PTPHeader->Flags & ETHTSYN_16BIT_MASK_MSB) >> 8U);

  BufPtr[ETHTSYN_PTP_HEADER_B8_CORRFIELD_0] = 0x00U;
  BufPtr[ETHTSYN_PTP_HEADER_B9_CORRFIELD_1] = 0x00U;
  BufPtr[ETHTSYN_PTP_HEADER_B10_CORRFIELD_2] = 0x00U;
  BufPtr[ETHTSYN_PTP_HEADER_B11_CORRFIELD_3] = 0x00U;
  BufPtr[ETHTSYN_PTP_HEADER_B12_CORRFIELD_4] = 0x00U;
  BufPtr[ETHTSYN_PTP_HEADER_B13_CORRFIELD_5] = 0x00U;
  BufPtr[ETHTSYN_PTP_HEADER_B14_CORRFIELD_6] = 0x00U;
  BufPtr[ETHTSYN_PTP_HEADER_B15_CORRFIELD_7] = 0x00U;

  BufPtr[ETHTSYN_PTP_HEADER_B16_RESERVED_0] = ETHTSYN_PTP_HEADER_RESERVED;
  BufPtr[ETHTSYN_PTP_HEADER_B17_RESERVED_1] = ETHTSYN_PTP_HEADER_RESERVED;
  BufPtr[ETHTSYN_PTP_HEADER_B18_RESERVED_2] = ETHTSYN_PTP_HEADER_RESERVED;
  BufPtr[ETHTSYN_PTP_HEADER_B19_RESERVED_3] = ETHTSYN_PTP_HEADER_RESERVED;

  /* Get source MAC address */
  EthIf_GetPhysAddr(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx, &SrcMacAddr[0]);

  BufPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0] = SrcMacAddr[0U]; /* Source MAC upper 3 bytes */
  BufPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1] = SrcMacAddr[1U];
  BufPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2] = SrcMacAddr[2U];
  BufPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3] = ETHTSYN_PTP_HEADER_EUI48_IDX3_SRCPORTIDENTITY;
  BufPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4] = ETHTSYN_PTP_HEADER_EUI48_IDX4_SRCPORTIDENTITY;
  BufPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5] = SrcMacAddr[3U]; /* Source MAC lower 3 bytes */
  BufPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6] = SrcMacAddr[4U];
  BufPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7] = SrcMacAddr[5U];
  BufPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8] = 0x00U; /* 16 bit port number */
  BufPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9] = 0x01U;

  BufPtr[ETHTSYN_PTP_HEADER_B30_SEQUNUMBER_1]
    = (uint8)((uint16)(PTPHeader->SequNumber & ETHTSYN_16BIT_MASK_MSB) >> 8U);
  BufPtr[ETHTSYN_PTP_HEADER_B31_SEQUNUMBER_0]
    = (uint8)(PTPHeader->SequNumber & ETHTSYN_16BIT_MASK_LSB);
  BufPtr[ETHTSYN_PTP_HEADER_B32_CONTROL] = PTPHeader->Control;
  BufPtr[ETHTSYN_PTP_HEADER_B33_LOGMSGINTERVAL] = PTPHeader->LogMsgInterval;

  DBG_ETHTSYN_FILLPTPHEADER_EXIT(EthTSynCtrlIdx, PTPHeader, BufPtr);
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillFUpTLV
(
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
)
{
  DBG_ETHTSYN_FILLFUPTLV_ENTRY(BufPtr);

  BufPtr[ETHTSYN_FUP_TLV_B0_TYPE_1] = ETHTSYN_FUP_TLV_TYPE_1;
  BufPtr[ETHTSYN_FUP_TLV_B1_TYPE_0] = ETHTSYN_FUP_TLV_TYPE_0;
  BufPtr[ETHTSYN_FUP_TLV_B2_LENGTH_1] = ETHTSYN_FUP_TLV_LENGTH_1;
  BufPtr[ETHTSYN_FUP_TLV_B3_LENGTH_0] = ETHTSYN_FUP_TLV_LENGTH_0;
  BufPtr[ETHTSYN_FUP_TLV_B4_ORGID_2] = ETHTSYN_FUP_TLV_ORGID_2;
  BufPtr[ETHTSYN_FUP_TLV_B5_ORGID_1] = ETHTSYN_FUP_TLV_ORGID_1;
  BufPtr[ETHTSYN_FUP_TLV_B6_ORGID_0] = ETHTSYN_FUP_TLV_ORGID_0;
  BufPtr[ETHTSYN_FUP_TLV_B7_ORG_SUBTYPE_2] = ETHTSYN_FUP_TLV_ORG_SUBTYPE_2;
  BufPtr[ETHTSYN_FUP_TLV_B8_ORG_SUBTYPE_1] = ETHTSYN_FUP_TLV_ORG_SUBTYPE_1;
  BufPtr[ETHTSYN_FUP_TLV_B9_ORG_SUBTYPE_0] = ETHTSYN_FUP_TLV_ORG_SUBTYPE_0;

  BufPtr[ETHTSYN_FUP_TLV_B10_CSRO_3] = 0U;
  BufPtr[ETHTSYN_FUP_TLV_B11_CSRO_2] = 0U;
  BufPtr[ETHTSYN_FUP_TLV_B12_CSRO_1] = 0U;
  BufPtr[ETHTSYN_FUP_TLV_B13_CSRO_0] = 0U;
  BufPtr[ETHTSYN_FUP_TLV_B14_GM_TIMEBASEIND_1] = ETHTSYN_FUP_TLV_GM_TIMEBASE_IND_1;
  BufPtr[ETHTSYN_FUP_TLV_B15_GM_TIMEBASEIND_0] = ETHTSYN_FUP_TLV_GM_TIMEBASE_IND_0;

  BufPtr[ETHTSYN_FUP_TLV_B16_GM_PH_CHG_0] = 0x00U;
  BufPtr[ETHTSYN_FUP_TLV_B17_GM_PH_CHG_1] = 0x00U;
  BufPtr[ETHTSYN_FUP_TLV_B18_GM_PH_CHG_2] = 0x00U;
  BufPtr[ETHTSYN_FUP_TLV_B19_GM_PH_CHG_3] = 0x00U;
  BufPtr[ETHTSYN_FUP_TLV_B20_GM_PH_CHG_4] = 0x00U;
  BufPtr[ETHTSYN_FUP_TLV_B21_GM_PH_CHG_5] = 0x00U;
  BufPtr[ETHTSYN_FUP_TLV_B22_GM_PH_CHG_6] = 0x00U;
  BufPtr[ETHTSYN_FUP_TLV_B23_GM_PH_CHG_7] = 0x00U;
  BufPtr[ETHTSYN_FUP_TLV_B24_GM_PH_CHG_8] = 0x00U;
  BufPtr[ETHTSYN_FUP_TLV_B25_GM_PH_CHG_9] = 0x00U;
  BufPtr[ETHTSYN_FUP_TLV_B26_GM_PH_CHG_10] = 0x00U;
  BufPtr[ETHTSYN_FUP_TLV_B27_GM_PH_CHG_11] = 0x00U;

  BufPtr[ETHTSYN_FUP_TLV_B28_GM_FREQ_CHG_3] = 0x00U;
  BufPtr[ETHTSYN_FUP_TLV_B29_GM_FREQ_CHG_2] = 0x00U;
  BufPtr[ETHTSYN_FUP_TLV_B30_GM_FREQ_CHG_1] = 0x00U;
  BufPtr[ETHTSYN_FUP_TLV_B31_GM_FREQ_CHG_0] = 0x00U;

  DBG_ETHTSYN_FILLFUPTLV_EXIT(BufPtr);
}

#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
/* !LINKSTO EthTSyn.ASR151.PRS_TS_00065_AutosarTLVHeader, 1 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillFUpAutosarTLV
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
)
{
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00093_TimeNotSecured, 1 */
  /* A time secured SubTlv will not be supported if
     EthTSynGlobalTimeTxCrcSecured is CRC_NOT_SUPPORTED */
  uint8 Length;
  uint8 AutosarTlvOffset = ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_SYNC_FUP_PAYLOADSIZE;

  uint8 SubTlvOffset = ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR;

  /* AUTOSAR TLV Header */
  BufPtr[AutosarTlvOffset + ETHTSYN_FUP_TLV_B0_TYPE_1] = ETHTSYN_FUP_TLV_TYPE_1;
  BufPtr[AutosarTlvOffset + ETHTSYN_FUP_TLV_B1_TYPE_0] = ETHTSYN_FUP_TLV_TYPE_0;

  /* Length without Sub-TLVs is 6U */
  Length = ETHTSYN_FUP_EXT_TLV_LENGTH_0;

  BufPtr[AutosarTlvOffset + ETHTSYN_FUP_TLV_B4_ORGID_2] = ETHTSYN_FUP_EXT_TLV_ORGID_2;
  BufPtr[AutosarTlvOffset + ETHTSYN_FUP_TLV_B5_ORGID_1] = ETHTSYN_FUP_EXT_TLV_ORGID_1;
  BufPtr[AutosarTlvOffset + ETHTSYN_FUP_TLV_B6_ORGID_0] = ETHTSYN_FUP_EXT_TLV_ORGID_0;
  BufPtr[AutosarTlvOffset + ETHTSYN_FUP_TLV_B7_ORG_SUBTYPE_2] = ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_2;
  BufPtr[AutosarTlvOffset + ETHTSYN_FUP_TLV_B8_ORG_SUBTYPE_1] = ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_1;
  BufPtr[AutosarTlvOffset + ETHTSYN_FUP_TLV_B9_ORG_SUBTYPE_0] = ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_0;

  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00070, 1 */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00068, 1 */

#if(ETHTSYN_TX_CRC_USED == STD_ON)
  /* If the Master -> EthTSynTLVFollowUpTimeSubTLV is set */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00065_TimeSecured, 1 */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00074, 1 */
  if(
      (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TLVFollowUpTimeSubTLV == TRUE) &&
      (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].IsTxCrcSecuredUsed == ETHTSYN_CRC_SUPPORTED)
    )
  {
    /* Length is length plus the size in bytes of FUpSubTLVTime secured */
    Length += ETHTSYN_FUP_SUBTLV_TIME_SIZE;
    EthTSyn_FillFUpSubTLVTime_Secured
    (
      EthTSynCtrlIdx,
      &BufPtr[0U],
      SubTlvOffset
    );

    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00071, 1 */
    SubTlvOffset += ETHTSYN_FUP_SUBTLV_TIME_SIZE;
  }

  /* If the Master -> EthTSynTLVFollowUpStatusSubTLV is set and CRC_SUPPORTED */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00065_StatusSecured, 1 */
  if(
      (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TLVFollowUpStatusSubTLV == TRUE) &&
      (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].IsTxCrcSecuredUsed == ETHTSYN_CRC_SUPPORTED)
    )
  {
    /* Length is length plus the size in bytes of FUpSubTLVStatus secured */
    Length += ETHTSYN_FUP_SUBTLV_STATUS_SIZE;
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00077, 1 */
    EthTSyn_FillFUpSubTLVStatus_Secured
    (
      EthTSynCtrlIdx,
      &BufPtr[0U],
      SubTlvOffset
    );

    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00071, 1 */
    SubTlvOffset += ETHTSYN_FUP_SUBTLV_STATUS_SIZE;
  }

  /* If the Master -> TLVFollowUpUserDataSubTLV is set and CRC_SUPPORTED */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00065_UserDataSecured, 1 */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00093_UserDataSecured, 1 */
  if(
      (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TLVFollowUpUserDataSubTLV == TRUE) &&
      (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].IsTxCrcSecuredUsed == ETHTSYN_CRC_SUPPORTED)
    )
  {
    /* SWS_EthTSyn_00153: If UserDataLength equals 0, UserDataSubTlv is excluded */
    if(EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userDataLength != 0U)
    {
      /* Length is length plus the size in bytes of FUpSubTLVUserData secured */
      Length += ETHTSYN_FUP_SUBTLV_USERDATA_SIZE;

      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00078, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00079, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00082, 1 */
      EthTSyn_FillFUpSubTLV_UserData_Secured
      (
        EthTSynCtrlIdx,
        &BufPtr[0U],
        SubTlvOffset
      );

      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00071, 1 */
      SubTlvOffset += ETHTSYN_FUP_SUBTLV_USERDATA_SIZE;
    }
  }
#else
  TS_PARAM_UNUSED(EthTSynCtrlIdx);
#endif /* (ETHTSYN_TX_CRC_USED == STD_ON) */

  /* If the Master -> EthTSynTLVFollowUpStatusSubTLV is set and CRC_NOT_SUPPORTED */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00065_StatusNotSecured, 1 */
  if(
      (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TLVFollowUpStatusSubTLV == TRUE) &&
      (
        EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].IsTxCrcSecuredUsed ==
        ETHTSYN_CRC_NOT_SUPPORTED
      )
    )
  {
    /* Length is length plus the size in bytes of FUpSubTLVStatus not secured */
    Length += ETHTSYN_FUP_SUBTLV_STATUS_SIZE;
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00071, 1 */
    EthTSyn_FillFUpSubTLVStatus_NotSecured
    (
      &BufPtr[0U],
      SubTlvOffset
    );

    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00071, 1 */
    SubTlvOffset += ETHTSYN_FUP_SUBTLV_STATUS_SIZE;
  }

  /* If the Master -> TLVFollowUpUserDataSubTLV is set and CRC_NOT_SUPPORTED */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00065_UserDataNotSecured, 1 */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00093_UserDataNotSecured, 1 */
  if(
      (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TLVFollowUpUserDataSubTLV == TRUE) &&
      (
        EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].IsTxCrcSecuredUsed ==
        ETHTSYN_CRC_NOT_SUPPORTED
      )
    )
  {
    /* If UserDataLength equals 0, UserDataSubTlv is excluded */
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00153, 1 */
    if(EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userDataLength != 0U)
    {
      /* Length is length plus the size in bytes of FUpSubTLVUserData not secured */
      Length += ETHTSYN_FUP_SUBTLV_USERDATA_SIZE;

      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00078, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00079, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00082, 1 */
      EthTSyn_FillFUpSubTLV_UserData_NotSecured
      (
        EthTSynCtrlIdx,
        &BufPtr[0U],
        SubTlvOffset
      );

      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00071, 1 */
      SubTlvOffset += ETHTSYN_FUP_SUBTLV_USERDATA_SIZE;
    }
  }

  /* If the Master -> EthTSynTLVFollowUpOFSSubTLV is set */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00086, 1 */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00093_OFSSecured, 1 */
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TLVFollowUpOFSSubTLV == TRUE)
  {
    Std_ReturnType RetVal = E_NOT_OK;
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00071, 1 */
    RetVal = EthTSyn_FillFUpSubTLVOfs
    (
      EthTSynCtrlIdx,
      &BufPtr[0U],
      SubTlvOffset
    );

    if(E_OK == RetVal)
    {
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00071, 1 */
      /* Length is length plus the size in bytes of FUpSubTLVOfs not secured */
      Length += ETHTSYN_FUP_SUBTLV_OFS_SIZE;
    }
  }

  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00067, 1 */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00069, 1 */
  BufPtr[AutosarTlvOffset + ETHTSYN_FUP_TLV_B2_LENGTH_1] = ETHTSYN_FUP_EXT_TLV_LENGTH_1;
  BufPtr[AutosarTlvOffset + ETHTSYN_FUP_TLV_B3_LENGTH_0] = Length;

}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillFUpSubTLVStatus_NotSecured
(
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint8 SubTlvOffset
)
{
  /* AUTOSAR TLV Sub-TLV: Status Not Secured */
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_STATUS_B0_TYPE] =
    ETHTSYN_FUP_SUBTLV_STATUS_NOT_SECURED_TYPE;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_STATUS_B1_LENGTH] =
    ETHTSYN_FUP_SUBTLV_STATUS_LENGTH;
  EthTSyn_FUpSubTLVStatus(&BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_STATUS_B2_STATUS]);
  /* Not used */
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_STATUS_B3_CRC_STATUS] =
    ETHTSYN_FUP_SUBTLV_STATUS_CRC_STATUS;
}


STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillFUpSubTLV_UserData_NotSecured
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint16 SubTlvOffset
)
{
  /* AUTOSAR TLV Sub-TLV: UserData Secured */
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B0_TYPE] =
    ETHTSYN_FUP_SUBTLV_USERDATA_NOT_SECURED_TYPE;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B1_LENGTH] =
    ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B2_USER_DATA_LENGTH] =
      EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userDataLength;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B3_USER_BYTE_0] =
      EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte0;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B4_USER_BYTE_1] =
      EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte1;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B5_USER_BYTE_2] =
      EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte2;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B6_CRC_USERDATA] = 0U;
}


STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_FillFUpSubTLVOfs
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint8 SubTlvOffset
)
{
  uint8 Crc_Ofs = 0U;
  StbM_TimeStampType timeStamp;
  StbM_UserDataType userData;
  Std_ReturnType Retval = E_NOT_OK;

  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00095, 1 */
  /* !LINKSTO EthTSyn.EB.OffsetSubTlv_GetOffset_NOK, 2 */
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00199, 1 */
  Retval = StbM_GetOffset
  (
    EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].OffsetTimeBaseId, &timeStamp, &userData
  );

  if(E_OK == Retval)
  {
    /* Length */
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B1_LENGTH] = ETHTSYN_FUP_SUBTLV_OFS_LENGTH;

    /* OfsTimeDomain */
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00085, 1 */
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B2_TIMEDOMAIN] =
                                 EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].OfsGlobalTimeDomainId;

    /* OfsTimeSec - SecondsHi */
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B3_TIMESEC] =
                                (uint8)((uint16)(timeStamp.secondsHi & ETHTSYN_32BIT_MASK1) >> 8U);
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B4_TIMESEC] =
                                (uint8)(timeStamp.secondsHi  & ETHTSYN_32BIT_MASK0_LSB);
    /* OfsTimeSec - Seconds */
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B5_TIMESEC] =
                             (uint8)((uint32)(timeStamp.seconds & ETHTSYN_32BIT_MASK3_MSB) >> 24U);
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B6_TIMESEC] =
                             (uint8)((uint32)(timeStamp.seconds & ETHTSYN_32BIT_MASK2) >> 16U);
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B7_TIMESEC] =
                             (uint8)((uint32)(timeStamp.seconds & ETHTSYN_32BIT_MASK1) >> 8U);
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B8_TIMESEC] =
                             (uint8)(timeStamp.seconds  & ETHTSYN_32BIT_MASK0_LSB);

    /* OfsTimeNSec */
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B9_TIMENSEC] =
                         (uint8)((uint32)(timeStamp.nanoseconds & ETHTSYN_32BIT_MASK3_MSB) >> 24U);
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B10_TIMENSEC] =
                         (uint8)((uint32)(timeStamp.nanoseconds & ETHTSYN_32BIT_MASK2) >> 16U);
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B11_TIMENSEC] =
                         (uint8)((uint32)(timeStamp.nanoseconds & ETHTSYN_32BIT_MASK1) >> 8U);
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B12_TIMENSEC] =
                         (uint8)(timeStamp.nanoseconds & ETHTSYN_32BIT_MASK0_LSB);

    /* Status */
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00094, 1 */
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B13_STATUS] = timeStamp.timeBaseStatus;

    /* userDataLength */
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00087, 1 */
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B14_USER_DATA_LENGTH] =  userData.userDataLength;

    /* userBytes */
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00089, 1 */
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B15_USER_DATA_BYTE_0] =  userData.userByte0;
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B16_USER_DATA_BYTE_1] =  userData.userByte1;
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B17_USER_DATA_BYTE_2] =  userData.userByte2;

#if(ETHTSYN_TX_CRC_USED == STD_ON)
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00084, 1 */
    if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].IsTxCrcSecuredUsed == ETHTSYN_CRC_SUPPORTED)
    {
      /* AUTOSAR TLV Sub-TLV: OFS Secured */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00065_OFSSecured, 1 */
      BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B0_TYPE] = ETHTSYN_FUP_SUBTLV_OFS_SECURED_TYPE;
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00103, 1 */
      (void)EthTSyn_CalcComp_OfsCrc(EthTSynCtrlIdx, &BufPtr[0U], SubTlvOffset, &Crc_Ofs);
    }
    else
    {
#endif
      /* AUTOSAR TLV Sub-TLV: OFS Not Secured */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00065_OFSNotSecured, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00093_OFSnotSecured, 1 */
      BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B0_TYPE] =
                                                            ETHTSYN_FUP_SUBTLV_OFS_NOT_SECURED_TYPE;
#if(ETHTSYN_TX_CRC_USED == STD_ON)
    }
#endif

    /* CRC_OFS */
    BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B18_CRC] = Crc_Ofs;
  }

  return Retval;
}
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FUpSubTLVStatus
(
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
)
{
  BufPtr[0U] = 0U;

  /* Set status from master SYNC_TO_GATEWAY bit */
  if((EthTSyn_MasterTimeBaseStatus.GlobalTimebaseStatus & ETHTSYN_SYNC_TO_GATEWAY_MASK_STBM) > 0U)
  {
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00094_SyncToGatewaySet, 1 */
    BufPtr[0U] |= ETHTSYN_SYNC_TO_SUBDOMAIN; /* the offset is 0 */
  }
  else
  {
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00094_SyncToGatewayNOTSet, 1 */
    BufPtr[0U] |= ETHTSYN_SYNC_TO_GTM;
  }
}

#if(ETHTSYN_TX_CRC_USED == STD_ON)

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillFUpSubTLVTime_Secured
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint8 SubTlvOffset
)
{
  uint8 Crc_0;
  uint8 Crc_1;

  /* AUTOSAR TLV Sub-TLV: Time Secured */
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_TIME_B0_TYPE]  = ETHTSYN_FUP_SUBTLV_TIME_TYPE;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_TIME_B1_LENGTH] = ETHTSYN_FUP_SUBTLV_TIME_LENGTH;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_TIME_B2_TIME_FLAGS] =
      EthTSyn_Ctrl[EthTSynCtrlIdx].Crc_Time_Flags;

  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00182, 1 */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00184, 1 */
  EthTSyn_CalculateCrc(EthTSynCtrlIdx, &BufPtr[0U], &Crc_0, &Crc_1);

  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_TIME_B3_CRC_TIME_0] = Crc_0;

  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_TIME_B4_CRC_TIME_1] = Crc_1;
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillFUpSubTLVStatus_Secured
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint8 SubTlvOffset
)
{
  uint8 Crc_Status;
  uint8 SeqNr = (BufPtr[ETHTSYN_PTP_HEADER_B31_SEQUNUMBER_0]) % 16U;

  /* AUTOSAR TLV Sub-TLV: Status Secured */
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_STATUS_B0_TYPE] =
    ETHTSYN_FUP_SUBTLV_STATUS_SECURED_TYPE;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_STATUS_B1_LENGTH] = ETHTSYN_FUP_SUBTLV_STATUS_LENGTH;
  EthTSyn_FUpSubTLVStatus(&BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_STATUS_B2_STATUS]);

  Crc_Status = Crc_CalculateCRC8H2F
      (&BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_STATUS_B2_STATUS], 1U, 0U, TRUE);

  Crc_Status = Crc_CalculateCRC8H2F
      (&EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].DataIdList[SeqNr], 1U, Crc_Status, FALSE);

  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_STATUS_B3_CRC_STATUS] = Crc_Status;
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillFUpSubTLV_UserData_Secured
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint16 SubTlvOffset
)
{
  uint8 Crc_UserData = 0U;

  /* AUTOSAR TLV Sub-TLV: UserData Secured */
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B0_TYPE] =
    ETHTSYN_FUP_SUBTLV_USERDATA_SECURED_TYPE;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B1_LENGTH] =
    ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B2_USER_DATA_LENGTH] =
      EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userDataLength ;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B3_USER_BYTE_0] =
      EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte0;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B4_USER_BYTE_1] =
      EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte1;
  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B5_USER_BYTE_2] =
      EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte2;

  (void)EthTSyn_CalcComp_UserDataCrc(EthTSynCtrlIdx, &BufPtr[0U], SubTlvOffset, &Crc_UserData);

  BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B6_CRC_USERDATA] = Crc_UserData;
}

#endif /*(ETHTSYN_TX_CRC_USED == STD_ON) */

#endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

#if(ETHTSYN_SEND_ANNOUNCEFRAMES == STD_ON)
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillAnnouncePayload
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
)
{
  uint8 Idx;
  uint8 SrcMacAddr[ETHTSYN_MAC_ADD_SIZE];

  DBG_ETHTSYN_FILLANNOUNCEPAYLOAD_ENTRY(EthTSynCtrlIdx, BufPtr);

  for(Idx = 0U; Idx <ETHTSYN_RESERVED_10BYTES; Idx++)
  {
    BufPtr[Idx] = 0U;
  }

  BufPtr[ETHTSYN_ANNOUNCE_PL_B10_CUR_UTC_OFFSET_1] = 0U;
  BufPtr[ETHTSYN_ANNOUNCE_PL_B11_CUR_UTC_OFFSET_0] = 0U;

  BufPtr[ETHTSYN_ANNOUNCE_PL_B12_RESERVED] = 0U;

  BufPtr[ETHTSYN_ANNOUNCE_PL_B13_GM_PRIO1] = ETHTSYN_GM_PRIORITY1;

  BufPtr[ETHTSYN_ANNOUNCE_PL_B14_GM_CLK_QUAL_3] = ETHTSYN_GM_CLOCK_CLASS;
  BufPtr[ETHTSYN_ANNOUNCE_PL_B15_GM_CLK_QUAL_2] = ETHTSYN_GM_CLOCK_ACCURANCY;
  BufPtr[ETHTSYN_ANNOUNCE_PL_B16_GM_CLK_QUAL_1] = ETHTSYN_GM_OFFSET_SCALEDLOG_VARIANCE_1;
  BufPtr[ETHTSYN_ANNOUNCE_PL_B17_GM_CLK_QUAL_0] = ETHTSYN_GM_OFFSET_SCALEDLOG_VARIANCE_0;

  BufPtr[ETHTSYN_ANNOUNCE_PL_B18_GM_PRIO2] = ETHTSYN_GM_PRIORITY2;

  /* Get source MAC address */
  EthIf_GetPhysAddr(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx, &SrcMacAddr[0]);

  BufPtr[ETHTSYN_ANNOUNCE_PL_B19_GM_IDENTITY_0] = SrcMacAddr[0U]; /* Source MAC upper 3 bytes */
  BufPtr[ETHTSYN_ANNOUNCE_PL_B20_GM_IDENTITY_1] = SrcMacAddr[1U];
  BufPtr[ETHTSYN_ANNOUNCE_PL_B21_GM_IDENTITY_2] = SrcMacAddr[2U];
  BufPtr[ETHTSYN_ANNOUNCE_PL_B22_GM_IDENTITY_3] = ETHTSYN_PTP_HEADER_EUI48_IDX3_SRCPORTIDENTITY;
  BufPtr[ETHTSYN_ANNOUNCE_PL_B23_GM_IDENTITY_4] = ETHTSYN_PTP_HEADER_EUI48_IDX4_SRCPORTIDENTITY;
  BufPtr[ETHTSYN_ANNOUNCE_PL_B24_GM_IDENTITY_5] = SrcMacAddr[3U]; /* Source MAC lower 3 bytes */
  BufPtr[ETHTSYN_ANNOUNCE_PL_B25_GM_IDENTITY_6] = SrcMacAddr[4U];
  BufPtr[ETHTSYN_ANNOUNCE_PL_B26_GM_IDENTITY_7] = SrcMacAddr[5U];

  BufPtr[ETHTSYN_ANNOUNCE_PL_B27_STEP_REM_1] = 0U;
  BufPtr[ETHTSYN_ANNOUNCE_PL_B28_STEP_REM_0] = 0U;

  BufPtr[ETHTSYN_ANNOUNCE_PL_B29_TIME_SOURCE] = 0xA0U; /* internal oscillator */

  BufPtr[ETHTSYN_ANNOUNCE_PL_B30_PATH_TRACE_TLV_TYPE_1] = ETHTSYN_ANNOUNCE_TLV_TYPE_1;
  BufPtr[ETHTSYN_ANNOUNCE_PL_B31_PATH_TRACE_TLV_TYPE_0] = ETHTSYN_ANNOUNCE_TLV_TYPE_0;

  BufPtr[ETHTSYN_ANNOUNCE_PL_B32_PATH_TRACE_TLV_LEN_0] = ETHTSYN_ANNOUNCE_TLV_LENGTH_1;
  BufPtr[ETHTSYN_ANNOUNCE_PL_B33_PATH_TRACE_TLV_LEN_1] = ETHTSYN_ANNOUNCE_TLV_LENGTH_0;

  /* Source MAC upper 3 bytes */
  BufPtr[ETHTSYN_ANNOUNCE_PL_B34_PATH_TRACE_TLV_SEQ_0] = SrcMacAddr[0U];
  BufPtr[ETHTSYN_ANNOUNCE_PL_B35_PATH_TRACE_TLV_SEQ_1] = SrcMacAddr[1U];
  BufPtr[ETHTSYN_ANNOUNCE_PL_B36_PATH_TRACE_TLV_SEQ_2] = SrcMacAddr[2U];
  BufPtr[ETHTSYN_ANNOUNCE_PL_B37_PATH_TRACE_TLV_SEQ_3]
    = ETHTSYN_PTP_HEADER_EUI48_IDX3_SRCPORTIDENTITY;
  BufPtr[ETHTSYN_ANNOUNCE_PL_B38_PATH_TRACE_TLV_SEQ_4]
    = ETHTSYN_PTP_HEADER_EUI48_IDX4_SRCPORTIDENTITY;
  /* Source MAC lower 3 bytes */
  BufPtr[ETHTSYN_ANNOUNCE_PL_B39_PATH_TRACE_TLV_SEQ_5] = SrcMacAddr[3U];
  BufPtr[ETHTSYN_ANNOUNCE_PL_B40_PATH_TRACE_TLV_SEQ_6] = SrcMacAddr[4U];
  BufPtr[ETHTSYN_ANNOUNCE_PL_B41_PATH_TRACE_TLV_SEQ_7] = SrcMacAddr[5U];

  DBG_ETHTSYN_FILLANNOUNCEPAYLOAD_EXIT(EthTSynCtrlIdx, BufPtr);
}
#endif /* ETHTSYN_SEND_ANNOUNCEFRAMES == STD_ON) */

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_TransmitFrame
(
  uint8 EthTSynCtrlIdx,
  Eth_BufIdxType BufIdx,
  uint16 PayloadLength,
  boolean ActivateTimeStamp
)
{
  uint8 DestMacAddr[ETHTSYN_MAC_ADD_SIZE];
  Std_ReturnType RetVal;

  DBG_ETHTSYN_TRANSMITFRAME_ENTRY(EthTSynCtrlIdx, BufIdx, PayloadLength);

  /* !LINKSTO EthTSyn.ASR44.ECUC_EthTSyn_00058_Conf, 1 */
  TS_MemCpy(DestMacAddr, EthTSyn_DestMacAddr, ETHTSYN_MAC_ADD_SIZE);

#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  if(ActivateTimeStamp == TRUE)
  {
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00200, 1 */
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00202, 1 */
    EthIf_EnableEgressTimeStamp
      (
        EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx,
        BufIdx
      );
  }
#else
  TS_PARAM_UNUSED(ActivateTimeStamp);
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */

  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00200, 1 */
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00202, 1 */
  /* !LINKSTO EthTSyn.EB.Master.FrameType, 1 */
  RetVal = EthIf_Transmit
  (
    EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx, /* EthIf CtrlIdx */
    BufIdx, /* BufIdx */
    ETHTSYN_PTP_FRAMETYPE, /* FrameType */
    TRUE, /* TxConfirmation */
    PayloadLength, /* LenByte */
    &DestMacAddr[0U] /* PhysAddrPtr */
  );

  DBG_ETHTSYN_TRANSMITFRAME_EXIT(RetVal, EthTSynCtrlIdx, BufIdx, PayloadLength);

  return RetVal;
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TimeStampToArray
(
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
)
{
  DBG_ETHTSYN_TIMESTAMPTOARRAY_ENTRY(TimeStampPtr, BufPtr);
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00181, 1 */
  BufPtr[0U] = (uint8)((uint16)(TimeStampPtr->secondsHi & ETHTSYN_16BIT_MASK_MSB) >> 8U);
  BufPtr[1U] = (uint8)(TimeStampPtr->secondsHi & ETHTSYN_16BIT_MASK_LSB);
  BufPtr[2U] = (uint8)((uint32)(TimeStampPtr->seconds & ETHTSYN_32BIT_MASK3_MSB) >> 24U);
  BufPtr[3U] = (uint8)((uint32)(TimeStampPtr->seconds & ETHTSYN_32BIT_MASK2) >> 16U);
  BufPtr[4U] = (uint8)((uint32)(TimeStampPtr->seconds & ETHTSYN_32BIT_MASK1) >> 8U);
  BufPtr[5U] = (uint8)(TimeStampPtr->seconds & ETHTSYN_32BIT_MASK0_LSB);
  BufPtr[6U] = (uint8)((uint32)(TimeStampPtr->nanoseconds & ETHTSYN_32BIT_MASK3_MSB) >> 24U);
  BufPtr[7U] = (uint8)((uint32)(TimeStampPtr->nanoseconds & ETHTSYN_32BIT_MASK2) >> 16U);
  BufPtr[8U] = (uint8)((uint32)(TimeStampPtr->nanoseconds & ETHTSYN_32BIT_MASK1) >> 8U);
  BufPtr[9U] = (uint8)(TimeStampPtr->nanoseconds & ETHTSYN_32BIT_MASK0_LSB);

  DBG_ETHTSYN_TIMESTAMPTOARRAY_EXIT(TimeStampPtr, BufPtr);
}
#if (ETHTSYN_MAX_SLAVE != 0U)
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ArrayToTimeStamp
(
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
)
{
  DBG_ETHTSYN_ARRAYTOTIMESTAMP_ENTRY(TimeStampPtr, BufPtr);

  TimeStampPtr->secondsHi = (uint16)((uint16)BufPtr[0U] << 8U)
                            + BufPtr[1U];
  TimeStampPtr->seconds = ((uint32)BufPtr[2U] << 24U)
                          + (uint32)((uint32)BufPtr[3U] << 16U)
                          + (uint32)((uint32)BufPtr[4U] << 8U)
                          + BufPtr[5U];
  TimeStampPtr->nanoseconds = ((uint32)BufPtr[6U] << 24U)
                              + (uint32)((uint32)BufPtr[7U] << 16U)
                              + (uint32)((uint32)BufPtr[8U] << 8U)
                              + BufPtr[9U];

  DBG_ETHTSYN_ARRAYTOTIMESTAMP_EXIT(TimeStampPtr, BufPtr);
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ProcessRxSyncFrame
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr
)
{
  Std_ReturnType TmpRet = E_NOT_OK;
  Eth_TimeStampType Sync_IngressTimeStamp;

  Sync_IngressTimeStamp.nanoseconds = 0U;
  Sync_IngressTimeStamp.seconds = 0U;
  Sync_IngressTimeStamp.secondsHi = 0U;

  DBG_ETHTSYN_PROCESSRXSYNCFRAME_ENTRY(EthTSynCtrlIdx, DataPtr);

  /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00199_CheckSC, 1 */
  if(E_OK == EthTSyn_CheckSC(EthTSynCtrlIdx, DataPtr))
  {
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00128, 1 */
    TmpRet = EthTSyn_GetIngressTimeStamp(EthTSynCtrlIdx, DataPtr, &Sync_IngressTimeStamp);
  }
#if (ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED != TS_PROD_ERR_DISABLE)
  if((EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived == TRUE) || (E_OK != TmpRet))
  {
    /* Fup frame is missing or there where an issue on processing the sync frame */
    EthTSyn_SyncFupDropped(EthTSynCtrlIdx);
  }
#endif

  /* !LINKSTO EthTSyn.EB.Slave.LostSyncFUPFrame, 1 */
  if(E_OK == TmpRet)
  {
    EthTSyn_Slave[EthTSynCtrlIdx].Sync_ActualIngressTimeStamp = Sync_IngressTimeStamp;
    TS_AtomicAssign8(EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived, TRUE);
    /* Timeout value of the Follow_Up message (of the subsequent Sync message).
       A value of 0 deactivates this timeout observation.*/
    EthTSyn_Slave[EthTSynCtrlIdx].GlobalTimeFollowUpTimeout =
            EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].GlobalTimeFollowUpTimeout;
    /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if(ETHTSYN_TIME_RECORDING == STD_ON)
    if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
    {

      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity =
           (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0]) << 56U);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1]) << 48U);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2]) << 40U);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3]) << 32U);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4]) << 24U);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5]) << 16U);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6]) << 8U);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity |=
           (uint64)((uint64) DataPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7]);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.portNumber =
           (uint16)((uint16) DataPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8]);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.portNumber |=
           (uint16)((uint16) DataPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9]);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sequenceId =
                                                EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber;

    }
#endif
  }
  else
  {
    /* Reset the state of the received sync frame*/
    TS_AtomicAssign8(EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived, FALSE);
  }

  DBG_ETHTSYN_PROCESSRXSYNCFRAME_EXIT(EthTSynCtrlIdx, DataPtr);
}

FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_CheckSC
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr
)
{
  Std_ReturnType TmpRet = E_NOT_OK;
  uint16 DeltaSeqCount = 0U;
  uint16 ReceivedSeqCounter = 0U;

  DBG_ETHTSYN_CHECKSC_ENTRY(EthTSynCtrlIdx, DataPtr);

  /* Store serial number to find corresponding follow up frame. */
  ReceivedSeqCounter = EthTSyn_GetPTPHeaderSequNumber(DataPtr);
  /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00199_SkipCheckSCounterAfterInit, 1 */
  /* !LINKSTO EthTSyn.EB.NoSCCheck_SyncTimeout, 1 */
  if(EthTSyn_Slave[EthTSynCtrlIdx].CheckSeqCounter == FALSE)
  {
    /* On the next received sync frame the sequence counter can be checked against the jump width. */
    TS_AtomicAssign8(EthTSyn_Slave[EthTSynCtrlIdx].CheckSeqCounter, TRUE);
    TmpRet = E_OK;
  }
  else
  {
    DeltaSeqCount = ETHTSYN_DELTA_SC(ReceivedSeqCounter, EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber);

    if((DeltaSeqCount > 0U) && (DeltaSeqCount <= EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SequenceCounterJumpWidth))
    {
      /* Valid Sequence counter*/
      /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00198_JumpWidthMaxValue, 1 */
      TmpRet = E_OK;
    }
    else
    {
      StbM_TimeBaseStatusType syncTimeBaseStatus = 0U;
      StbM_TimeBaseStatusType offsetTimeBaseStatus = 0U;

      (void)StbM_GetTimeBaseStatus(
                                    EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId,
                                    &syncTimeBaseStatus, &offsetTimeBaseStatus
                                  );
      TS_PARAM_UNUSED(offsetTimeBaseStatus);

      if(((syncTimeBaseStatus & ETHTSYN_STBM_TIMEOUT) != 0U) && (DeltaSeqCount > 0U))
      {
        /* !LINKSTO EthTSyn.EB.NoSCCheck_SyncTimeout, 1 */
        /* Timeout occured - No need to check SC for the next received Sync frame. */
        TmpRet = E_OK;
      }

      /* Invalid Sequence counter*/
      /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00198_JumpWidthGreaterThanConfiguredCounterJumpWidth, 1 */
      /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00198_JumpWidthIsZero, 1 */
    }

  }
  /* Save the seqId for the next received sync frame. */
  TS_AtomicAssign16(EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber, ReceivedSeqCounter);

  DBG_ETHTSYN_CHECKSC_EXIT(TmpRet, EthTSynCtrlIdx, DataPtr);

  return TmpRet;
}
#endif /* ETHTSYN_MAX_SLAVE != 0U */

#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
#if (ETHTSYN_MAX_SLAVE != 0U)

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyAutosarTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length
)
{
  Std_ReturnType RetVal = E_NOT_OK;
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  uint8 Crc_Time_1;
#endif
  /* The length of the IEEE TLV plus the length of the AUTOSAR TLV header: 76 + 10.
   * If the Length is higher than this size there is at least 1 SubTLV. */
  /* !LINKSTO EthTSyn.EB.RxIndication.InvalidFrameLength, 1 */
  if(Length > (ETHTSYN_FUP_AUTOSAR_TLV_HEADER_START_ADDR + ETHTSYN_FUP_AUTOSAR_TLV_HEADER_LENGTH))
  {
    RetVal = EthTSyn_VerifyTLVHeader(&DataPtr[ETHTSYN_FUP_AUTOSAR_TLV_HEADER_START_ADDR]);

    if(RetVal != E_NOT_OK)
    {
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF)
      /* !LINKSTO EthTSyn.EB.RxIndication.ProcessSubTlvLength, 1 */
      RetVal = EthTSyn_VerifySubTLVData(EthTSynCtrlIdx, &DataPtr[0U], Length);
#else
      if((DataPtr[ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR] == ETHTSYN_FUP_SUBTLV_TIME_TYPE) &&
         ((DataPtr[ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR + ETHTSYN_FUP_SUBTLV_TIME_B2_TIME_FLAGS] &
           ETHTSYN_CORRECTION_FIELD_BIT_MASK) != 0U
         )
        )
        {
          /* !LINKSTO EthTSyn.ASR151.PRS_TS_00207, 1 */
          EthTSyn_Inc02_BridgeCalculateCrcTime1(EthTSynCtrlIdx, &DataPtr[0U], &Crc_Time_1);
          if(Crc_Time_1 == DataPtr[ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR + ETHTSYN_FUP_SUBTLV_TIME_B4_CRC_TIME_1])
          {
            RetVal = E_OK;
          }
          else
          {
            RetVal = E_NOT_OK;
          }
        }
        else
        {
          /* SyncFUp doesn't contain a TimeSecured Sub-TLV, or the TimeSecured Sub-TLV doesn't need validation */
          RetVal = E_OK;
        }
#endif
    }
  }

  return RetVal;
}

/* !LINKSTO EthTSyn.ASR151.PRS_TS_00106, 1 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyTLVHeader
(
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint16 Tlv_Length;

  Tlv_Length = ETHTSYN_GET16(DataPtr, 2);

  /* Verify header: */
  if(
      /* TLV type */
      (DataPtr[ETHTSYN_FUP_TLV_B0_TYPE_1] == 0U) &&
      (DataPtr[ETHTSYN_FUP_TLV_B1_TYPE_0] == 3U) &&
      /* length field */
      (Tlv_Length > ETHTSYN_FUP_EXT_TLV_LENGTH_0) &&

      /* organizationId */
      (DataPtr[ETHTSYN_FUP_TLV_B4_ORGID_2] == ETHTSYN_FUP_EXT_TLV_ORGID_2) &&
      (DataPtr[ETHTSYN_FUP_TLV_B5_ORGID_1] == ETHTSYN_FUP_EXT_TLV_ORGID_1) &&
      (DataPtr[ETHTSYN_FUP_TLV_B6_ORGID_0] == ETHTSYN_FUP_EXT_TLV_ORGID_0) &&
      /* organizationSubType */
      (DataPtr[ETHTSYN_FUP_TLV_B7_ORG_SUBTYPE_2] == ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_2) &&
      (DataPtr[ETHTSYN_FUP_TLV_B8_ORG_SUBTYPE_1] == ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_1) &&
      (DataPtr[ETHTSYN_FUP_TLV_B9_ORG_SUBTYPE_0] == ETHTSYN_FUP_EXT_TLV_ORG_SUBTYPE_0)
    )
  {
    RetVal = E_OK;
  }

  return RetVal;
}

#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF)
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifySubTLVData
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint16 Idx = ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR;
  uint16 Tlv_Received_Length = 0U;
  uint16 Tlv_Processed_Length = ETHTSYN_FUP_EXT_TLV_LENGTH_0;
  boolean ExitLoop = FALSE;

  Tlv_Received_Length = ETHTSYN_GET16(DataPtr, ETHTSYN_FUP_AUTOSAR_TLV_HEADER_START_ADDR + 2U);

  while
  (
    ((Idx + ETHTSYN_FUP_SUBTLV_HEADER_SIZE) <= Length) &&
    (ExitLoop != TRUE) &&
    (Tlv_Processed_Length < Tlv_Received_Length)
  )
  {
    /* Verify the type of the existing SubTLV */
    switch(DataPtr[Idx])
    {
      case ETHTSYN_FUP_SUBTLV_TIME_TYPE:
      {
        /* If Sub-TLV length is incorrect, following data shall be corrupt - drop whole message */
        /* !LINKSTO EthTSyn.EB.AutosarSubTLV.IncorrectLength, 1 */
        /* DataPtr[Idx + 1U] holds the second byte in a SubTLV - Length Byte */
        if(DataPtr[Idx + 1U] == ETHTSYN_FUP_SUBTLV_TIME_LENGTH)
        {
          RetVal = EthTSyn_VerifyTimeSecuredSubTlv(EthTSynCtrlIdx, &DataPtr[0U], Length, Idx);
        }
        else
        {
          RetVal = E_NOT_OK;
        }
        break;
      }
      case ETHTSYN_FUP_SUBTLV_STATUS_NOT_SECURED_TYPE:
      {
        /* If Sub-TLV length is incorrect, following data shall be corrupt - drop whole message */
        /* !LINKSTO EthTSyn.EB.AutosarSubTLV.IncorrectLength, 1 */
        /* DataPtr[Idx + 1U] holds the second byte in a SubTLV - Length Byte */
        if(DataPtr[Idx + 1U] == ETHTSYN_FUP_SUBTLV_STATUS_LENGTH)
        {
          RetVal = EthTSyn_VerifyStatusNotSecuredSubTlv(EthTSynCtrlIdx, &DataPtr[0U], Length, Idx);
        }
        else
        {
          RetVal = E_NOT_OK;
        }
        break;
      }
      case ETHTSYN_FUP_SUBTLV_STATUS_SECURED_TYPE:
      {

        /* If Sub-TLV length is incorrect, following data shall be corrupt - drop whole message */
        /* !LINKSTO EthTSyn.EB.AutosarSubTLV.IncorrectLength, 1 */
        /* DataPtr[Idx + 1U] holds the second byte in a SubTLV - Length Byte */
        if(DataPtr[Idx + 1U] == ETHTSYN_FUP_SUBTLV_STATUS_LENGTH)
        {
          RetVal = EthTSyn_VerifyStatusSecuredSubTlv(EthTSynCtrlIdx, &DataPtr[0U], Length, Idx);
        }
        else
        {
          RetVal = E_NOT_OK;
        }

        break;
      }
      case ETHTSYN_FUP_SUBTLV_USERDATA_NOT_SECURED_TYPE:
      {
        /* If Sub-TLV length is incorrect, following data shall be corrupt - drop whole message */
        /* !LINKSTO EthTSyn.EB.AutosarSubTLV.IncorrectLength, 1 */
        /* DataPtr[Idx + 1U] holds the second byte in a SubTLV - Length Byte */
        if(DataPtr[Idx + 1U] == ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH)
        {
          /* !LINKSTO EthTSyn.ASR151.PRS_TS_00081, 1 */
          RetVal = EthTSyn_VerifyUserDataNotSecuredSubTlv
                      (EthTSynCtrlIdx, &DataPtr[0U], Length, Idx);
        }
        else
        {
          RetVal = E_NOT_OK;
        }

        break;
      }
      case ETHTSYN_FUP_SUBTLV_USERDATA_SECURED_TYPE:
      {
        /* If Sub-TLV length is incorrect, following data shall be corrupt - drop whole message */
        /* !LINKSTO EthTSyn.EB.AutosarSubTLV.IncorrectLength, 1 */
        /* DataPtr[Idx + 1U] holds the second byte in a SubTLV - Length Byte */
        if(DataPtr[Idx + 1U] == ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH)
        {
          /* !LINKSTO EthTSyn.ASR151.PRS_TS_00081, 1 */
          RetVal = EthTSyn_VerifyUserDataSecuredSubTlv(EthTSynCtrlIdx, &DataPtr[0U], Length, Idx);
        }
        else
        {
          RetVal = E_NOT_OK;
        }

        break;
      }
      case ETHTSYN_FUP_SUBTLV_OFS_NOT_SECURED_TYPE:
      {
        /* If Sub-TLV length is incorrect, following data shall be corrupt - drop whole message */
        /* !LINKSTO EthTSyn.EB.AutosarSubTLV.IncorrectLength, 1 */
        /* DataPtr[Idx + 1U] holds the second byte in a SubTLV - Length Byte */
        if(DataPtr[Idx + 1U] == ETHTSYN_FUP_SUBTLV_OFS_LENGTH)
        {
          /* !LINKSTO EthTSyn.ASR151.PRS_TS_00119_Sub_TlvType, 1 */
          RetVal = EthTSyn_VerifyOfsNotSecuredSubTlv
                      (EthTSynCtrlIdx, &DataPtr[0U], Length, Idx);
        }
        else
        {
          RetVal = E_NOT_OK;
        }

        break;
      }
      case ETHTSYN_FUP_SUBTLV_OFS_SECURED_TYPE:
      {
        /* If Sub-TLV length is incorrect, following data shall be corrupt - drop whole message */
        /* !LINKSTO EthTSyn.EB.AutosarSubTLV.IncorrectLength, 1 */
        /* DataPtr[Idx + 1U] holds the second byte in a SubTLV - Length Byte */
        if(DataPtr[Idx + 1U] == ETHTSYN_FUP_SUBTLV_OFS_LENGTH)
        {
          /* !LINKSTO EthTSyn.ASR151.PRS_TS_00081, 1 */
          /* !LINKSTO EthTSyn.ASR151.PRS_TS_00119_Sub_TlvType, 1 */
          RetVal = EthTSyn_VerifyOfsSecuredSubTlv(EthTSynCtrlIdx, &DataPtr[0U], Length, Idx);
        }
        else
        {
          RetVal = E_NOT_OK;
        }

        break;
      }
      /* Sub-TLV with unknown Sub-TLV type shall be ignored and the next
        Sub-TLV shall be processed. */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00118, 1 */
      /* !LINKSTO EthTSyn.EB.AutosarSubTLV.UnknownSubTLVType, 1 */
      default:
      {
        RetVal = E_OK;
        break;
      }
    }
    if(RetVal == E_NOT_OK)
    {
      ExitLoop = TRUE;
    }
    else
    {
      Tlv_Processed_Length += (uint16)(DataPtr[Idx + 1U]) + ETHTSYN_FUP_SUBTLV_HEADER_SIZE;
    }

    /* Move DataPtr offset to hold the SubTLV type byte  */
    Idx += (uint16)(DataPtr[Idx + 1U]) + ETHTSYN_FUP_SUBTLV_HEADER_SIZE;
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyTimeSecuredSubTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length,
  uint16 Offset
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  if((Offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_TIME_LENGTH) <= Length)
  {
    switch(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].RxCrcValidated)
    {
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00107_TimeSecuredValidated, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00109_TimeSecured_Optional, 1 */
      case ETHTSYN_CRC_VALIDATED: /* Fall through */
      case ETHTSYN_CRC_OPTIONAL:
      {
#if(ETHTSYN_RX_CRC_USED == STD_ON)
        RetVal = EthTSyn_CompareCrc
        (
          EthTSynCtrlIdx,
          &DataPtr[0U],
          Offset
        );
#endif
        break;
      }
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00108_TimeSecured_Ignored, 1 */
      case ETHTSYN_CRC_IGNORED:
      {
        RetVal = E_OK;
        break;
      }
      case ETHTSYN_CRC_NOT_VALIDATED:
      {
        /* !LINKSTO EthTSyn.ASR151.PRS_TS_00107_TimeNotValidated, 1 */
        /* !LINKSTO EthTSyn.EB.AutosarSubTLV.CRC_NOT_VALIDATED, 1 */
        RetVal = E_NOT_OK;
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
        ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
        RetVal = E_NOT_OK;
        break; /* Caution: this 'break' is required by MISRA-C:2012 */
      }
      /* CHECK: PARSE */
    }
  }
#if(ETHTSYN_RX_CRC_USED == STD_OFF)
  TS_PARAM_UNUSED(DataPtr);
#endif
  return RetVal;
}

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyStatusSecuredSubTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length,
  uint16 Offset
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  if((Offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_STATUS_LENGTH) <= Length)
  {
    switch(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].RxCrcValidated)
    {
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00107_StatusValidated, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00109_StatusSecured_Optional, 1 */
      case ETHTSYN_CRC_VALIDATED: /* Fall through */
      case ETHTSYN_CRC_OPTIONAL:
      {
#if(ETHTSYN_RX_CRC_USED == STD_ON)
        RetVal = EthTSyn_CompareCrc_Status
        (
          EthTSynCtrlIdx,
          &DataPtr[0U],
          Offset
        );
        if(RetVal == E_OK)
#endif /* ETHTSYN_RX_CRC_USED == STD_ON */
        {
          /* Verify that SubTLVPtr byte offset 2 is the status */
          /* !LINKSTO EthTSyn.ASR151.PRS_TS_00156_SgwIsNOTSyncToSubDomain, 1 */
          if(
              (
                DataPtr[Offset + ETHTSYN_FUP_SUBTLV_STATUS_B2_STATUS] &
                ETHTSYN_SYNC_TO_GATEWAY_MASK_SUBTLV
              ) > 0U
            )
          {
            /* !LINKSTO EthTSyn.ASR151.PRS_TS_00156_SgwIsSyncToSubDomain, 1 */
            /* Extract the SYNC_TO_GATEWAY bit if it is set */
            EthTSyn_SyncToGatewayBit = ETHTSYN_SYNC_TO_GATEWAY_MASK_STBM;
          }
        }
        break;
      }
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00108_StatusSecured_Ignored, 1 */
      case ETHTSYN_CRC_IGNORED:
      {
        /* Verify that SubTLVPtr byte offset 2 is the status */
        if(
            (
              DataPtr[Offset + ETHTSYN_FUP_SUBTLV_STATUS_B2_STATUS] &
              ETHTSYN_SYNC_TO_GATEWAY_MASK_SUBTLV
            ) > 0U
          )
        {
          /* Extract the SYNC_TO_GATEWAY bit if it is set */
          EthTSyn_SyncToGatewayBit = ETHTSYN_SYNC_TO_GATEWAY_MASK_STBM;
        }
        RetVal = E_OK;
        break;
      }
      case ETHTSYN_CRC_NOT_VALIDATED:
      {
        /* !LINKSTO EthTSyn.EB.AutosarSubTLV.CRC_NOT_VALIDATED, 1 */
        RetVal = E_NOT_OK;
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
        ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
        RetVal = E_NOT_OK;
        break; /* Caution: this 'break' is required by MISRA-C:2012 */
      }
      /* CHECK: PARSE */
    }
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyStatusNotSecuredSubTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length,
  uint16 Offset
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  if((Offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_STATUS_LENGTH) <= Length)
  {
    switch(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].RxCrcValidated)
    {
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00107_StatusNotValidated, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00108_StatusNotSecured_Ignored, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00109_StatusNotSecured_Optional, 1 */
      case ETHTSYN_CRC_IGNORED: /* Fall through */
      case ETHTSYN_CRC_NOT_VALIDATED: /* Fall through */
      case ETHTSYN_CRC_OPTIONAL:
      {
        {
          /* Verify that SubTLVPtr byte offset 2 is the status */
          /* !LINKSTO EthTSyn.ASR151.PRS_TS_00156_SgwIsNOTSyncToSubDomain, 1 */
          if(
              (
                DataPtr[Offset + ETHTSYN_FUP_SUBTLV_STATUS_B2_STATUS] &
                ETHTSYN_SYNC_TO_GATEWAY_MASK_SUBTLV
              ) > 0U
            )
          {
            /* Extract the SYNC_TO_GATEWAY bit if it is set */
            /* !LINKSTO EthTSyn.ASR151.PRS_TS_00156_SgwIsSyncToSubDomain, 1 */
            EthTSyn_SyncToGatewayBit = ETHTSYN_SYNC_TO_GATEWAY_MASK_STBM;
          }
          /* All the SubTLV data is set */
          RetVal = E_OK;
        }
        break;
      }
      case ETHTSYN_CRC_VALIDATED:
      {
        /* !LINKSTO EthTSyn.EB.AutosarSubTLV.CRC_VALIDATED, 1 */
        RetVal = E_NOT_OK;
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
        ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
        RetVal = E_NOT_OK;
        break; /* Caution: this 'break' is required by MISRA-C:2012 */
      }
      /* CHECK: PARSE */
    }
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyUserDataSecuredSubTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length,
  uint16 Offset
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  if((Offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH) <= Length)
  {
    switch(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].RxCrcValidated)
    {
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00107_UsrDataValidated, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00109_UserDataSecured_Optional, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00116_Validated, 1 */
      case ETHTSYN_CRC_VALIDATED: /* Fall through */
      case ETHTSYN_CRC_OPTIONAL:
      {
#if(ETHTSYN_RX_CRC_USED == STD_ON)
        RetVal = EthTSyn_CalcComp_UserDataCrc
        (
          EthTSynCtrlIdx,
          &DataPtr[0U],
          Offset,
          NULL_PTR
        );

        EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userDataLength  =
                      DataPtr[Offset + ETHTSYN_FUP_SUBTLV_USERDATA_B2_USER_DATA_LENGTH];
        EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte0 =
                      DataPtr[Offset + ETHTSYN_FUP_SUBTLV_USERDATA_B3_USER_BYTE_0];
        EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte1 =
                      DataPtr[Offset + ETHTSYN_FUP_SUBTLV_USERDATA_B4_USER_BYTE_1];
        EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte2 =
                      DataPtr[Offset + ETHTSYN_FUP_SUBTLV_USERDATA_B5_USER_BYTE_2];

#endif /* ETHTSYN_RX_CRC_USED == STD_ON */
        break;
      }
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00108_UserDataSecured_Ignored, 1 */
      case ETHTSYN_CRC_IGNORED:
      {
        EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userDataLength  =
                      DataPtr[Offset + ETHTSYN_FUP_SUBTLV_USERDATA_B2_USER_DATA_LENGTH];
        EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte0 =
                      DataPtr[Offset + ETHTSYN_FUP_SUBTLV_USERDATA_B3_USER_BYTE_0];
        EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte1 =
                      DataPtr[Offset + ETHTSYN_FUP_SUBTLV_USERDATA_B4_USER_BYTE_1];
        EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte2 =
                      DataPtr[Offset + ETHTSYN_FUP_SUBTLV_USERDATA_B5_USER_BYTE_2];

        RetVal = E_OK;
        break;
      }
      case ETHTSYN_CRC_NOT_VALIDATED:
      {
        /* !LINKSTO EthTSyn.EB.AutosarSubTLV.CRC_NOT_VALIDATED, 1 */
        RetVal = E_NOT_OK;
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
        ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
        RetVal = E_NOT_OK;
        break; /* Caution: this 'break' is required by MISRA-C:2012 */
      }
      /* CHECK: PARSE */
    }
  }
#if(ETHTSYN_RX_CRC_USED == STD_OFF)
  TS_PARAM_UNUSED(DataPtr);
#endif
  return RetVal;
}

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyUserDataNotSecuredSubTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length,
  uint16 Offset
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  if((Offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_USERDATA_LENGTH) <= Length)
  {
    switch(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].RxCrcValidated)
    {
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00107_UsrDataNotValidated, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00108_UserDataNotSecured_Ignored, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00109_UserDataNotSecured_Optional, 1 */
      case ETHTSYN_CRC_IGNORED: /* Fall through */
      case ETHTSYN_CRC_NOT_VALIDATED: /* Fall through */
      case ETHTSYN_CRC_OPTIONAL:
      {
        EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userDataLength  =
                      DataPtr[Offset + ETHTSYN_FUP_SUBTLV_USERDATA_B2_USER_DATA_LENGTH];
        EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte0 =
                      DataPtr[Offset + ETHTSYN_FUP_SUBTLV_USERDATA_B3_USER_BYTE_0];
        EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte1 =
                      DataPtr[Offset + ETHTSYN_FUP_SUBTLV_USERDATA_B4_USER_BYTE_1];
        EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte2 =
                      DataPtr[Offset + ETHTSYN_FUP_SUBTLV_USERDATA_B5_USER_BYTE_2];

        RetVal = E_OK;
        break;
      }
      case ETHTSYN_CRC_VALIDATED:
      {
        /* !LINKSTO EthTSyn.EB.AutosarSubTLV.CRC_VALIDATED, 1 */
        RetVal = E_NOT_OK;
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
        ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
        RetVal = E_NOT_OK;
        break; /* Caution: this 'break' is required by MISRA-C:2012 */
      }
      /* CHECK: PARSE */
    }
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyOfsNotSecuredSubTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length,
  uint16 Offset
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  if((Offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_OFS_LENGTH) <= Length)
  {
    switch(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].RxCrcValidated)
    {
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00107_OFSNotValidated, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00108_OFSNotSecured_Ignored, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00109_OFSNotSecured_Optional, 1 */
      case ETHTSYN_CRC_IGNORED: /* Fall through */
      case ETHTSYN_CRC_NOT_VALIDATED: /* Fall through */
      case ETHTSYN_CRC_OPTIONAL:
      {
        RetVal = EthTSyn_SaveOfsData(EthTSynCtrlIdx, &DataPtr[0U], Offset);
        break;
      }
      case ETHTSYN_CRC_VALIDATED:
      {
        /* !LINKSTO EthTSyn.EB.AutosarSubTLV.CRC_VALIDATED, 1 */
        RetVal = E_NOT_OK;
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
        ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
        RetVal = E_NOT_OK;
        break; /* Caution: this 'break' is required by MISRA-C:2012 */
      }
      /* CHECK: PARSE */
    }
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_VerifyOfsSecuredSubTlv
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length,
  uint16 Offset
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  if((Offset + ETHTSYN_FUP_SUBTLV_HEADER_SIZE + ETHTSYN_FUP_SUBTLV_OFS_LENGTH) <= Length)
  {
    switch(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].RxCrcValidated)
    {
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00107_OFSValidated, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00109_OFSSecured_Optional, 1 */
      case ETHTSYN_CRC_VALIDATED: /* Fall through */
      case ETHTSYN_CRC_OPTIONAL:
      {
#if(ETHTSYN_RX_CRC_USED == STD_ON)
        /* !LINKSTO EthTSyn.ASR151.PRS_TS_00117_Validated, 1 */
        /* !LINKSTO EthTSyn.ASR151.PRS_TS_00117_Optional, 1 */
        RetVal = EthTSyn_CalcComp_OfsCrc
        (
          EthTSynCtrlIdx,
          &DataPtr[0U],
          Offset,
          NULL_PTR
        );

        if(RetVal == E_OK)
        {
          RetVal = EthTSyn_SaveOfsData(EthTSynCtrlIdx, &DataPtr[0U], Offset);
        }

#endif /* ETHTSYN_RX_CRC_USED == STD_ON */
        break;
      }
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00108_OFSSecured_Ignored, 1 */
      case ETHTSYN_CRC_IGNORED:
      {
        RetVal = EthTSyn_SaveOfsData(EthTSynCtrlIdx, &DataPtr[0U], Offset);
        break;
      }
      case ETHTSYN_CRC_NOT_VALIDATED:
      {
        /* !LINKSTO EthTSyn.EB.AutosarSubTLV.CRC_NOT_VALIDATED, 1 */
        RetVal = E_NOT_OK;
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
        ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
        RetVal = E_NOT_OK;
        break; /* Caution: this 'break' is required by MISRA-C:2012 */
      }
      /* CHECK: PARSE */
    }
  }
#if(ETHTSYN_RX_CRC_USED == STD_OFF)
  TS_PARAM_UNUSED(DataPtr);
#endif
  return RetVal;
}

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SaveOfsData
(
 uint8 EthTSynCtrlIdx,
 P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
 uint16 Offset
)
{
 Std_ReturnType RetVal = E_NOT_OK;

  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00110, 1 */
  /* OfsTimeDomain */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00119_OfsTimeDomain, 1 */
  EthTSyn_Slave[EthTSynCtrlIdx].ReceivedOffsetTimeDomId =
                                DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B2_TIMEDOMAIN];

  /* The received time domain matches to the defined OfsTimeDomain */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00119_TimeDomain, 1 */
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].OfsGlobalTimeDomainId ==
                                                  EthTSyn_Slave[EthTSynCtrlIdx].ReceivedOffsetTimeDomId)
  {
    /* Calculate and save received TimeSecHi */
    EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetTimeStamp.secondsHi =
                        (uint16)((uint16)DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B3_TIMESEC] << 8U)
                                        + DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B4_TIMESEC];

    /* Calculate and save received TimeSec */
    EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetTimeStamp.seconds =
                       ((uint32)DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B5_TIMESEC] << 24U)
                     + (uint32)((uint32)DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B6_TIMESEC] << 16U)
                     + (uint32)((uint32)DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B7_TIMESEC] << 8U)
                     + DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B8_TIMESEC];

    /* Calculate and save received TimeNSec */
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00119_OfsTimeNSec, 1 */
    EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetTimeStamp.nanoseconds =
                     ((uint32)DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B9_TIMENSEC] << 24U)
                   + (uint32)((uint32)DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B10_TIMENSEC] << 16U)
                   + (uint32)((uint32)DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B11_TIMENSEC] << 8U)
                   + DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B12_TIMENSEC];

    /* Status */
    EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetTimeStamp.timeBaseStatus =
                                DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B13_STATUS];

    /* userDataLength */
    EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetUserData.userDataLength =
                                DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B14_USER_DATA_LENGTH];
    /* userBytes */
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00088, 1 */
    EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetUserData.userByte0 =
                                DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B15_USER_DATA_BYTE_0];
    EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetUserData.userByte1 =
                                DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B16_USER_DATA_BYTE_1];
    EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetUserData.userByte2 =
                                DataPtr[Offset + ETHTSYN_FUP_SUBTLV_OFS_B17_USER_DATA_BYTE_2];
    EthTSyn_Slave[EthTSynCtrlIdx].OffsetTlvReceived = TRUE;

    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00119_OfsTimeNSec, 1 */
    if(ETHTSYN_NANOSEC_MAX < EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetTimeStamp.nanoseconds)
    {
      RetVal = E_NOT_OK;
    }
    else
    {
      /* !LINKSTO EthTSyn.EB.NanosecondsLimit, 1 */
      RetVal = E_OK;
    }
  }

  return RetVal;
}

#endif /* (ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF) */
#endif /* ETHTSYN_MAX_SLAVE != 0U */

#if((ETHTSYN_RX_CRC_USED == STD_ON) || (ETHTSYN_TX_CRC_USED == STD_ON))
/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00111, 1 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_CalculateCrc
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) Crc_0,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) Crc_1
)
{
  uint8 Crc_Temp_0;
  uint8 Crc_Temp_1;
  uint8 SeqNr = (BufPtr[ETHTSYN_PTP_HEADER_B31_SEQUNUMBER_0]) % 16U;

  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00096, 1 */
  Crc_Temp_0 = Crc_CalculateCRC8H2F(&EthTSyn_Ctrl[EthTSynCtrlIdx].Crc_Time_Flags, 1U, 0U, TRUE);
  Crc_Temp_1 = Crc_Temp_0;

  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00099, 1 */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00113, 1 */
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].CrcDomainNumber == TRUE)
  {
    Crc_Temp_0 = Crc_CalculateCRC8H2F
            (&BufPtr[ETHTSYN_PTP_HEADER_B4_DOMAIN], 1U, Crc_Temp_0, FALSE);
  }
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].CrcSourcePortIdentity == TRUE)
  {
    Crc_Temp_0 = Crc_CalculateCRC8H2F
            (&BufPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0], 10U, Crc_Temp_0, FALSE);
  }
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].CrcPreciseOriginTimestamp == TRUE)
  {
    Crc_Temp_0 = Crc_CalculateCRC8H2F
            (
              &BufPtr[ETHTSYN_PTP_HEADER_SIZE], ETHTSYN_TIME_STAMP_SIZE, Crc_Temp_0, FALSE
            );
  }
  Crc_Temp_0 = Crc_CalculateCRC8H2F
            (
              &EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].DataIdList[SeqNr],
              1U, Crc_Temp_0, FALSE
            );

  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00100, 1 */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00114, 1 */
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].CrcMessageLength == TRUE)
  {
    Crc_Temp_1 =
        Crc_CalculateCRC8H2F(&BufPtr[ETHTSYN_PTP_HEADER_B2_MSGLENGTH_1], 2U, Crc_Temp_1, FALSE);
  }

  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].CrcCorrectionField == TRUE)
  {
    Crc_Temp_1 = Crc_CalculateCRC8H2F
            (&BufPtr[ETHTSYN_PTP_HEADER_B8_CORRFIELD_0], 8U, Crc_Temp_1, FALSE);
  }

  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].CrcSequenceId == TRUE)
  {
    Crc_Temp_1 =
        Crc_CalculateCRC8H2F(&BufPtr[ETHTSYN_PTP_HEADER_B30_SEQUNUMBER_1], 2U, Crc_Temp_1, FALSE);
  }
  Crc_Temp_1 = Crc_CalculateCRC8H2F
            (
              &EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].DataIdList[SeqNr],
              1U, Crc_Temp_1, FALSE
            );

  *Crc_0 = Crc_Temp_0;
  *Crc_1 = Crc_Temp_1;
}

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_CalcComp_UserDataCrc
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint16 SubTlvOffset,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) Crc_UserData
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 Crc_UserDataTemp = 0U;
  uint8 SeqNr = (BufPtr[ETHTSYN_PTP_HEADER_B31_SEQUNUMBER_0]) % 16U;

  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00102, 1 */
  /* Calculate Crc_UserData by considering: UserDataLength, UserByte_0, UserByte_1, UserByte_2 and
   * DataID */
  Crc_UserDataTemp = Crc_CalculateCRC8H2F
      (&BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B2_USER_DATA_LENGTH], 4U, 0U, TRUE);

  Crc_UserDataTemp = Crc_CalculateCRC8H2F
      (&EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].DataIdList[SeqNr], 1U, Crc_UserDataTemp,
                                                                                            FALSE);

#if(ETHTSYN_MAX_SLAVE != 0U)
  if(TRUE == EthTSyn_IsSlave(EthTSynCtrlIdx)) /* Slave */
  {
    /* Compare calculated Crc with received Crc */
    if(
        Crc_UserDataTemp == BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_USERDATA_B6_CRC_USERDATA]
      )
    {
      RetVal = E_OK;
    }
  }
  else
#endif/* ETHTSYN_MAX_SLAVE != 0U */
  {
    *Crc_UserData = Crc_UserDataTemp;
    RetVal = E_OK;
  }

  return RetVal;
}


STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_CalcComp_OfsCrc
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint16 SubTlvOffset,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) Crc_Ofs
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 Crc_OfsTemp = 0U;
  uint8 SeqNr = (BufPtr[ETHTSYN_PTP_HEADER_B31_SEQUNUMBER_0]) % 16U;

  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00119_DataId, 1 */
  /* calculate the CRC for CRC_OFS by considering the contents
   * of OfsTimeDomain, OfsTimeSec, OfsTimeNSec, Status, UserDataLength,
   * UserByte_0, UserByte_1, UserByte_2 and DataID (in this order).*/

  Crc_OfsTemp = Crc_CalculateCRC8H2F
      (&BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B2_TIMEDOMAIN], 16U, 0U, TRUE);

  Crc_OfsTemp = Crc_CalculateCRC8H2F
      (&EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].DataIdList[SeqNr], 1U, Crc_OfsTemp, FALSE);

#if(ETHTSYN_MAX_SLAVE != 0U)
  if(TRUE == EthTSyn_IsSlave(EthTSynCtrlIdx)) /* Slave */
  {
    /* Compare calculated Crc with received Crc */
    if(
        Crc_OfsTemp == BufPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_OFS_B18_CRC]
      )
    {
      RetVal = E_OK;
    }
  }
  else
#endif/* ETHTSYN_MAX_SLAVE != 0U */
  {
    *Crc_Ofs = Crc_OfsTemp;
    RetVal = E_OK;
  }

  return RetVal;
}

#endif /* ((ETHTSYN_RX_CRC_USED == STD_ON) || (ETHTSYN_TX_CRC_USED == STD_ON)) */

#if(ETHTSYN_RX_CRC_USED == STD_ON)
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF)
/* !LINKSTO EthTSyn.ASR151.PRS_TS_00113, 1 */
/* !LINKSTO EthTSyn.ASR151.PRS_TS_00114, 1 */
STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_CompareCrc
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 SubTlvOffset
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 Crc_0;
  uint8 Crc_1;

  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00183, 1 */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00185, 1 */
  EthTSyn_CalculateCrc(EthTSynCtrlIdx, &DataPtr[0U], &Crc_0, &Crc_1);

  if(
      (Crc_0 == DataPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_TIME_B3_CRC_TIME_0]) &&
      (Crc_1 == DataPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_TIME_B4_CRC_TIME_1])
    )
  {
    RetVal = E_OK;
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_CompareCrc_Status
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 SubTlvOffset
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 Crc_Status;

  uint8 SeqNr = (DataPtr[ETHTSYN_PTP_HEADER_B31_SEQUNUMBER_0]) % 16U;

  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00101, 1 */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00115, 1 */
  Crc_Status = Crc_CalculateCRC8H2F
      (&DataPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_STATUS_B2_STATUS], 1U, 0U, TRUE);

  Crc_Status = Crc_CalculateCRC8H2F
      (&EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].DataIdList[SeqNr], 1U, Crc_Status, FALSE);

  /* Compare calculated Crc with received Crc */
  if(
      Crc_Status == DataPtr[SubTlvOffset + ETHTSYN_FUP_SUBTLV_STATUS_B3_CRC_STATUS]
    )
  {
    RetVal = E_OK;
  }
  return RetVal;
}
#endif

#endif/* (ETHTSYN_RX_CRC_USED == STD_ON) */
#endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

#if (ETHTSYN_MAX_SLAVE != 0U)
/* !LINKSTO EthTSyn.EB.Slave.LostPdelayRespFUP, 1 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ProcessRxSynFUpFrame
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 Length
)
{
  const uint16 RxSequNumber = EthTSyn_GetPTPHeaderSequNumber(DataPtr);
  Std_ReturnType RetVal = E_NOT_OK;

  /* Default value for the SYNC_TO_GATEWAY bit in timeBaseStatus */
  EthTSyn_SyncToGatewayBit = 0U;

  DBG_ETHTSYN_PROCESSRXSYNCFUPFRAME_ENTRY(EthTSynCtrlIdx, DataPtr);

  /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00196, 1 */
  if(RxSequNumber == EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber)
  {
    /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00119_SeqId, 1 */
#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
    Std_ReturnType Check = E_NOT_OK;

    Check = EthTSyn_VerifyAutosarTlv(EthTSynCtrlIdx, &DataPtr[0U], Length);
#else
    {
      TS_PARAM_UNUSED(Length);
    }
#endif

#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
    if(Check == E_OK)
    {
#endif
      if(EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived)
      {
        Eth_TimeStampType OriginTimeStamp;
        Eth_TimeStampType TmpTimeStamp;
        Eth_TimeStampType TmpTimeStamp_T2;
        
        OriginTimeStamp.nanoseconds = 0U;
        OriginTimeStamp.seconds = 0U;
        OriginTimeStamp.secondsHi = 0U;

        TmpTimeStamp_T2.nanoseconds = 0U;
        TmpTimeStamp_T2.seconds = 0U;
        TmpTimeStamp_T2.secondsHi = 0U;

        /* !LINKSTO EthTSyn.EB.Slave.LostSyncFUPFrame, 1 */
        TS_AtomicAssign8(EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived, FALSE);

        /* Get origin time stamp of the master from follow up frame payload. */
        EthTSyn_ArrayToTimeStamp(&OriginTimeStamp, &DataPtr[ETHTSYN_PTP_HEADER_SIZE]);

        if(ETHTSYN_NANOSEC_MAX < OriginTimeStamp.nanoseconds)
        {
          /* !LINKSTO EthTSyn.EB.Nanoseconds, 1 */
          RetVal = E_NOT_OK;
        }
        else
        {
          /* !LINKSTO EthTSyn.EB.NanosecondsLimit, 1 */
          RetVal = E_OK;
        }

        if(E_OK == RetVal)
        {
          /* Measure Data */
          StbM_MeasurementType MeasureData;
#if(ETHTSYN_TIME_RECORDING == STD_ON)
          StbM_EthTimeSlaveMeasurementType RecordData;
#endif /* ETHTSYN_TIME_RECORDING == STD_ON */
          /* Calculate time offset between master and slave. */
          /* Pdelay + OriginTimeStampSync[FUP] */
          TmpTimeStamp.secondsHi = 0U;
          TmpTimeStamp.seconds = 0U;
          TmpTimeStamp.nanoseconds = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay;

          /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00150,1 */
          MeasureData.pathDelay = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay;

          RetVal = EthTSyn_AddCorrField
          (
            EthTSynCtrlIdx,
            &DataPtr[ETHTSYN_PTP_HEADER_B8_CORRFIELD_0],
            &TmpTimeStamp
          );

          if(E_OK == RetVal) /* Only perform a correction if no wrong time stamp detected. */
          {
            /* Follow up frame correspond to latest received sync frame. */
            const Eth_TimeStampType Sync_ActualIngressTimeStamp
              = EthTSyn_Slave[EthTSynCtrlIdx].Sync_ActualIngressTimeStamp;
            StbM_VirtualLocalTimeType VirtualLocalTimeT2;
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
            Eth_TimeStampQualType TimeQuality;
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */

            VirtualLocalTimeT2.nanosecondsLo = 0U;
            VirtualLocalTimeT2.nanosecondsHi = 0U;

            EthTSyn_AddTimeStamps(&TmpTimeStamp, &OriginTimeStamp);

#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
            if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].VltStbMIsEthFreerunningCounter
                                                                                           == TRUE)
            {
              SchM_Enter_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_0();
              /* !LINKSTO EthTSyn.EB.Slave.HWTimpStamp.GatewaySync, 1 */
              /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00024, 1 */
              RetVal = EthIf_GetCurrentTime
                (
                  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx,
                  &TimeQuality,
                  &TmpTimeStamp_T2
                );
              SchM_Exit_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_0();
              RetVal |=
                EthTSyn_Int01_ConvertEthTimeToVirtualTime(&TmpTimeStamp_T2, &VirtualLocalTimeT2);
            }
            else
            {
              SchM_Enter_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_1();
              /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00024, 1 */
              RetVal = EthIf_GetCurrentTime
                (
                  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx,
                  &TimeQuality,
                  &TmpTimeStamp_T2
                );
              RetVal |= StbM_GetCurrentVirtualLocalTime
                (
                  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId,
                  &VirtualLocalTimeT2
                );
              SchM_Exit_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_1();
            }

            /* !LINKSTO EthTSyn.EB.Slave.InvHWTimpStamp.GatewaySync, 1 */
            if(TimeQuality != ETH_VALID)
            {
              RetVal = E_NOT_OK;
            }

#else /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
            SchM_Enter_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_1();
            RetVal = StbM_GetCurrentVirtualLocalTime
              (
                 EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId,
                 &VirtualLocalTimeT2
              );
            SchM_Exit_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_1();
            /* Convert virtual time to ethtime */
            EthTSyn_Int01_ConvertVirtualTimeToEthTime(&VirtualLocalTimeT2, &TmpTimeStamp_T2);
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */

            /* !LINKSTO EthTSyn.EB.Slave.StbMSync.GetCurrentTime.NotOk, 1 */
            if(E_OK == RetVal)
            {
              StbM_UserDataType UserData;
              StbM_TimeStampType StbMTimeStamp;
              Eth_TimeIntDiffType TimeStampDiff;

              /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00080, 1 */
              UserData.userDataLength = EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userDataLength;
              UserData.userByte0 = EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte0;
              UserData.userByte1 = EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte1;
              UserData.userByte2 = EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte2;
              /* T2 - T1 */
              EthTSyn_SubTimeStamps(
                             &TmpTimeStamp_T2, /* T2 */
                             &Sync_ActualIngressTimeStamp, /* T1 */
                             &TimeStampDiff /* (T2 - T1 ) */
                           );
              /* OriginTimeStamp + correctionField + Pdelay + (T2 - T1) */
              EthTSyn_AddTimeStamps(
                           &TmpTimeStamp, /* OriginTimeStamp + correctionField + Pdelay */
                           &TimeStampDiff.diff /* T2 - T1  */
                         );
              StbMTimeStamp.nanoseconds = TmpTimeStamp.nanoseconds;
              StbMTimeStamp.seconds = TmpTimeStamp.seconds;
              StbMTimeStamp.secondsHi = TmpTimeStamp.secondsHi;
              StbMTimeStamp.timeBaseStatus = EthTSyn_SyncToGatewayBit;
              /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if(ETHTSYN_TIME_RECORDING == STD_ON)
              if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
              {
                /* Save PreciseOriginTimestamp as received in the Followup Message from master */
                EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.preciseOriginTimestamp.
                                                         nanoseconds = OriginTimeStamp.nanoseconds;
                EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.preciseOriginTimestamp.
                                                         seconds = OriginTimeStamp.seconds;
                EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.pDelay =
                                                              EthTSyn_Slave[EthTSynCtrlIdx].Pdelay;
                RecordData = EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording;
              }
#endif
              /* Return value ignored: No further action possible if E_NOT_OK returned. */
              /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00129, 1 */
              /* !LINKSTO EthTSyn.EB.Slave.HWTimpStamp.GatewaySync, 1 */
              /* !LINKSTO EthTSyn.EB.Slave.StbMSync, 1 */
              /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00052, 1 */
              (void) StbM_BusSetGlobalTime
                (
                  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId, /* timeBaseId */
                  &StbMTimeStamp, /* timeStampPtr */
                  &UserData, /* userDataPtr */
                  &MeasureData, /* measureDataPtr */
                  &VirtualLocalTimeT2 /* localTimePtr */
                );
              if(EthTSyn_Slave[EthTSynCtrlIdx].OffsetTlvReceived == TRUE)
              {
                 /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00198, 1 */
                (void) StbM_BusSetGlobalTime
                  (
                    EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].OffsetTimeBaseId , /* offsetTimeBaseId */
                    &EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetTimeStamp, /* timeStampPtr */
                    &EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetUserData, /* userDataPtr */
                    &MeasureData, /* measureDataPtr */
                    &VirtualLocalTimeT2 /* localTimePtr */
                  );
                EthTSyn_Slave[EthTSynCtrlIdx].OffsetTlvReceived = FALSE;
              }
              /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if(ETHTSYN_TIME_RECORDING == STD_ON)
              if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
              {
                /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00216, 1 */
                (void) StbM_EthSetSlaveTimingData
                  (
                    EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId, /* timeBaseId */
                    /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00217, 1 */
                    &RecordData /* measureDataPtr */
                  );
              }
#endif /* ETHTSYN_TIME_RECORDING == STD_ON */

#if(ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED == TS_PROD_ERR_REP_TO_DEM)
              /* Heal dem event */
              /* !LINKSTO EthTSyn.EB.Diagnostic_SyncOK_DEM, 1 */
              Dem_ReportErrorStatus
              (
                  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EventIdSyncFailed,
                  DEM_EVENT_STATUS_PREPASSED
              );
#elif (ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED == TS_PROD_ERR_REP_TO_DET)
              /* Sync/Fup pair was received, reset the counter */
              EthTSyn_Slave[EthTSynCtrlIdx].SyncFupDroppedPairsCt = 0U;
#endif
#if(ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED != TS_PROD_ERR_DISABLE)
              /* Reload the value of timeout */
              EthTSyn_Slave[EthTSynCtrlIdx].SyncFupPairTimeout =
                EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].ReceiveSyncFupPairTimeout;
#endif
            }
          }
        }
      }
#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
    }
#endif
  }
#if (ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED != TS_PROD_ERR_DISABLE)
  if(E_OK != RetVal)
  {
    /* Sync frame is missing or there where an issue on processing the Fup frame */
    EthTSyn_SyncFupDropped(EthTSynCtrlIdx);
  }
#endif
  DBG_ETHTSYN_PROCESSRXSYNCFUPFRAME_EXIT(EthTSynCtrlIdx, DataPtr);
}

/* !LINKSTO EthTSyn.EB.Slave.LostPdelayResp, 1 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ProcessRxPdelayRespFrame
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr
)
{
  Std_ReturnType TmpRet;
  uint16 PdelayReqSequNumber;
  EthTSyn_PdelayCalculationType PdelayCalculationStarted;
  const uint16 RxSequNumber = EthTSyn_GetPTPHeaderSequNumber(DataPtr);

  DBG_ETHTSYN_PROCESSPDELAYRESPFRAME_ENTRY(EthTSynCtrlIdx, DataPtr);

  TS_AtomicAssign16(PdelayReqSequNumber, EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqSequNumber);
  TS_AtomicAssign8(PdelayCalculationStarted, EthTSyn_Slave[EthTSynCtrlIdx].CalculatePdelay);

  SchM_Enter_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_0();
  /* Check source port identity */
  TmpRet = TS_MemCmp(
                      &EthTSyn_Ctrl[EthTSynCtrlIdx].Pdelay_Tx_SrcPortIdentity[0U],
                      &DataPtr[ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_TIME_STAMP_SIZE],
                      ETHTSYN_SOURCEPORTIDENTITY_SIZE
                    );
  SchM_Exit_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_0();

  if(RxSequNumber != PdelayReqSequNumber)
  {
    /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00192, 1 */
    TmpRet = E_NOT_OK;
  }
  else if(!(ETHTSYN_PDELAYCALC_PDELAYREQ_SENT_MASK & PdelayCalculationStarted))
  {
    TmpRet = E_NOT_OK;
  }
  else
  {
    /* Keep TmpRet from TS_MemCmp(). */
  }

  if(E_OK == TmpRet)
  { /* Received Pdelay response correspond to previously sent Pdelay request. */

  /* See 802.1AS-2011 Ch 11.1.2: Propagation delay calculation:
     D = ((t4 - t1) - (t3 - t2)) / 2 */
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF)
    /* Get t4 */
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00049, 1 */
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00161, 1 */
    TmpRet = EthTSyn_GetIngressTimeStamp(EthTSynCtrlIdx, DataPtr, &EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t4);

    if(E_OK == TmpRet)
    {
#endif /* ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF */
      /* Get t2 */
      EthTSyn_ArrayToTimeStamp(
                                &EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t2,
                                &DataPtr[ETHTSYN_PTP_HEADER_SIZE]
                              );

      if(ETHTSYN_NANOSEC_MAX >= EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t2.nanoseconds)
      {
        /* !LINKSTO EthTSyn.EB.NanosecondsLimit, 1 */
        /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if(ETHTSYN_TIME_RECORDING == STD_ON)
        if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
        {
          /* requestReceiptTimeStamp as received in pdelay_Response message */
          EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestReceiptTimestamp.
              seconds = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t2.seconds;
          EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestReceiptTimestamp.
              nanoseconds = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t2.nanoseconds;

          /* Save the sourcePortId of the received Pdelay_Resp frame*/
          EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                 = (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0]) << 56U);
          EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                |= (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1]) << 48U);
          EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                |= (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2]) << 40U);
          EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                |= (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3]) << 32U);
          EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                |= (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4]) << 24U);
          EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                |= (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5]) << 16U);
          EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                |= (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6]) << 8U);
          EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                |= (uint64)((uint64) DataPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7]);
          EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.portNumber =
               (uint16)((uint16) DataPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8]);
          EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.portNumber |=
               (uint16)((uint16) DataPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9]);

        }
#endif
        TS_AtomicSetBit_8
        (
          &EthTSyn_Slave[EthTSynCtrlIdx].CalculatePdelay,
          ETHTSYN_BITPOS_PDELAYRESP_RECEIVED
        );
      }
      else
      {
        /* Something is wrong, abort the pdelay calculation */
        TmpRet = E_NOT_OK;
      }
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF)
    }
#endif/* ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF */
  }
  if(TmpRet == E_NOT_OK)
  {
#if (ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED != TS_PROD_ERR_DISABLE)
    EthTSyn_PDelayFrameDropped(EthTSynCtrlIdx);
#endif
    /* Something is wrong, abort the pdelay calculation */
    EthTSyn_ResetSlavePdelayData(EthTSynCtrlIdx);
  }

  DBG_ETHTSYN_PROCESSPDELAYRESPFRAME_EXIT(EthTSynCtrlIdx, DataPtr);
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ProcessRxPdelayRespFUpFrame
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr
)
{
  Std_ReturnType TmpRet;
  uint16 PdelayReqSequNumber;
  EthTSyn_PdelayCalculationType PdelayCalculationStarted;
  const uint16 RxSequNumber = EthTSyn_GetPTPHeaderSequNumber(DataPtr);

  DBG_ETHTSYN_PROCESSPDELAYRESPFUPFRAME_ENTRY(EthTSynCtrlIdx, DataPtr);

  TS_AtomicAssign16(PdelayReqSequNumber, EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqSequNumber);
  TS_AtomicAssign8(PdelayCalculationStarted, EthTSyn_Slave[EthTSynCtrlIdx].CalculatePdelay);

  SchM_Enter_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_0();
  /* Check source port identity */
  TmpRet = TS_MemCmp(
                      &EthTSyn_Ctrl[EthTSynCtrlIdx].Pdelay_Tx_SrcPortIdentity[0U],
                      &DataPtr[ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_TIME_STAMP_SIZE],
                      ETHTSYN_SOURCEPORTIDENTITY_SIZE
                    );
  SchM_Exit_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_0();

  if(RxSequNumber != PdelayReqSequNumber)
  {
    /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00194, 1 */
    TmpRet = E_NOT_OK;
  }
  else if(!(ETHTSYN_PDELAYCALC_PDELAYRESP_RECEIVED_MASK & PdelayCalculationStarted))
  {
    TmpRet = E_NOT_OK;
  }
  else
  {
    /* Keep TmpRet from TS_MemCmp(). */
  }

  if(E_OK == TmpRet)
  {
    /* Received Pdelay response follow up correspond to previously sent Pdelay request. */
    /* Get t3 */
    EthTSyn_ArrayToTimeStamp(&EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t3, &DataPtr[ETHTSYN_PTP_HEADER_SIZE]);

    if(ETHTSYN_NANOSEC_MAX >= EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t3.nanoseconds)
    {
      /* !LINKSTO EthTSyn.EB.NanosecondsLimit, 1 */
      /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if(ETHTSYN_TIME_RECORDING == STD_ON)
      if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
      {
         /* responseOriginTimeStamp as received in pdelay_Response_Follow_Up */
         EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responseOriginTimestamp.seconds
              =  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t3.seconds;
         EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responseOriginTimestamp.
              nanoseconds = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t3.nanoseconds;
      }
#endif

      TS_AtomicSetBit_8
      (
        &EthTSyn_Slave[EthTSynCtrlIdx].CalculatePdelay,
        ETHTSYN_BITPOS_PDELAYRESP_FUP_RECEIVED
      );

#if(ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED == TS_PROD_ERR_REP_TO_DEM)
      /* Heal dem event */
      /* !LINKSTO EthTSyn.EB.Diagnostic_PdelayOK_DEM, 1 */
      Dem_ReportErrorStatus
      (
         EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EventIdPdelayFailed,
         DEM_EVENT_STATUS_PREPASSED
      );
#elif(ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED == TS_PROD_ERR_REP_TO_DET)
      /* PdelayResp/Fup pair was received, reset the counter */
      EthTSyn_Slave[EthTSynCtrlIdx].PdelayRespFupDroppedPairsCt = 0U;
#endif
#if (ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED != TS_PROD_ERR_DISABLE)
      /* Reset the supervision of timeout */
      EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayRespFupPairTimeoutInit = FALSE;
#endif
    }
    else
    {
      /* Something is wrong, abort the pdelay calculation */
      TmpRet = E_NOT_OK;
    }
  }

  if(E_OK != TmpRet)
  {
#if (ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED != TS_PROD_ERR_DISABLE)
    EthTSyn_PDelayFrameDropped(EthTSynCtrlIdx);
#endif
    /* Something is wrong, abort the pdelay calculation */
    EthTSyn_ResetSlavePdelayData(EthTSynCtrlIdx);
  }

  DBG_ETHTSYN_PROCESSPDELAYRESPFUPFRAME_EXIT(EthTSynCtrlIdx, DataPtr);
}
#endif /* ETHTSYN_MAX_SLAVE != 0U */

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ProcessRxPdelayReqFrame
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 LenByte
)
{
  Std_ReturnType TmpRet;

  DBG_ETHTSYN_PROCESSRXPDELAYREQFRAME_ENTRY(EthTSynCtrlIdx, DataPtr, LenByte);

  /* Store serial number to be used for Pdelay_resp and Pdelay_resp_FUp. */
  TS_AtomicAssign16(
                     EthTSyn_Ctrl[EthTSynCtrlIdx].Ingress_PdelayReqSequNumber,
                     EthTSyn_GetPTPHeaderSequNumber(DataPtr)
                   );
  TS_MemCpy(
       &EthTSyn_Ctrl[EthTSynCtrlIdx].Pdelay_Rx_SrcPortIdentity[0U],
       &DataPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0],
       ETHTSYN_SOURCEPORTIDENTITY_SIZE
     );
  /* Get ingress time stamp of Pdelay request. */
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00160, 1 */
  TmpRet = EthTSyn_GetIngressTimeStamp
          (EthTSynCtrlIdx, DataPtr, &EthTSyn_Ctrl[EthTSynCtrlIdx].IngressTimeStamp);
  /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if((ETHTSYN_TIME_RECORDING == STD_ON) && (ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE))
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
  {
    /* Store serial number as received from slave. */
    TS_AtomicAssign16(
                   EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.sequenceId,
                   EthTSyn_Ctrl[EthTSynCtrlIdx].Ingress_PdelayReqSequNumber
                 );

    /* Save the sourcePortId of received Pdelay_Req frame*/
    EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestPortId.clockIdentity =
     (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0]) << 56U);
    EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestPortId.clockIdentity |=
     (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1]) << 48U);
    EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestPortId.clockIdentity |=
     (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2]) << 40U);
    EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestPortId.clockIdentity |=
     (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3]) << 32U);
    EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestPortId.clockIdentity |=
     (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4]) << 24U);
    EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestPortId.clockIdentity |=
     (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5]) << 16U);
    EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestPortId.clockIdentity |=
     (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6]) << 8U);
    EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestPortId.clockIdentity |=
     (uint64)((uint64) DataPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7]);
    EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestPortId.portNumber =
     (uint16)((uint16) DataPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8]);
    EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestPortId.portNumber |=
     (uint16)((uint16) DataPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9]);
  }
#endif
  if(TmpRet == E_OK)
  {
#if(ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT == STD_ON)
    /* !LINKSTO EthTSyn.EB.DevAuth_RxPdelayReqWithTLV, 1 */
    if(ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_REQ_PAYLOADSIZE  < LenByte)
    {
       EthTSyn_PdelayDevAuth(EthTSynCtrlIdx,
          &DataPtr[ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_REQ_PAYLOADSIZE]);
    }
#else
    TS_PARAM_UNUSED(LenByte);
#endif
     /* !LINKSTO EthTSyn.ASR151.PRS_TS_00120, 1 */
     /* set the transmission flag for pdelayresp, to be sent in the next mainfunction */
     ETHTSYN_SET_TXFLAG
      (&EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, ETHTSYN_BITPOS_PDELAYRESP, TRUE);
  }

  DBG_ETHTSYN_PROCESSRXPDELAYREQFRAME_EXIT(EthTSynCtrlIdx, DataPtr, LenByte);
}

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_GetIngressTimeStamp
(
  uint8 EthTSynCtrlIdx,
  /* Deviation MISRAC2012-1 */
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) RxDataPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr
)
{
  Std_ReturnType RetVal;

#if(ETHTSYN_TIME_RECORDING == STD_ON)
  /* Extract message type from PTP header. */
  const uint8 MsgType = RxDataPtr[ETHTSYN_PTP_HEADER_B0_LN_MSGTYPE] & ETHTSYN_8BIT_MASK_LN;
#endif

  DBG_ETHTSYN_GETINGRESSTIMESTAMP_ENTRY(EthTSynCtrlIdx, RxDataPtr, TimeStampPtr);

#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  {
    Eth_TimeStampQualType TimeQuality;

    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00128, 1 */
    (void)EthIf_GetIngressTimeStamp
      (
        EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx, /* EthIf CtrlIdx */
        RxDataPtr,
        &TimeQuality,
        TimeStampPtr /* TimeStampPtr */
      );

    /* !LINKSTO EthTSyn.EB.InvTimeQuality.PdelayRespIngress, 1 */
    /* !LINKSTO EthTSyn.EB.InvTimeQuality.Slave.SyncIngress, 1 */
    /* !LINKSTO EthTSyn.EB.InvTimeQuality.PdelayReqIngress, 1 */
    if(ETH_VALID == TimeQuality)
    {
      RetVal = E_OK;
    }
    else
    {
      RetVal = E_NOT_OK;
    }

    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00124, 1 */
    /* !LINKSTO EthTSyn.EB.InvTimeStamp.PdelayReqIngress, 1 */
    if(ETHTSYN_NANOSEC_MAX < TimeStampPtr->nanoseconds)
    {
      /* !LINKSTO EthTSyn.EB.Nanoseconds, 1 */
      RetVal = E_NOT_OK; /* Nanoseconds >= 1s is not allowed. */
    }
    /* !LINKSTO EthTSyn.EB.NanosecondsLimit, 1 */
  }
#else /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */

#if(ETHTSYN_TIME_RECORDING == STD_OFF)
  TS_PARAM_UNUSED(RxDataPtr); /* Not needed for StbM time stamp. */
#endif/* ETHTSYN_TIME_RECORDING == STD_OFF */

  RetVal = EthTSyn_Ctrl[EthTSynCtrlIdx].GetCurrentVirtualLocalTimeRetVal;
  EthTSyn_Int01_ConvertVirtualTimeToEthTime(&EthTSyn_Ctrl[EthTSynCtrlIdx].RxIngressVirtualTime, TimeStampPtr);

#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */

  /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if(ETHTSYN_TIME_RECORDING == STD_ON)
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
  {
    switch(MsgType)
    {
      case ETHTSYN_PTP_HEADER_MSGTYPE_SYNC:
      {
#if(ETHTSYN_MAX_SLAVE != 0U)
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
        (void)EthTSyn_Int01_ConvertEthTimeToVirtualTime
            (
                TimeStampPtr,
                &EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.syncIngressTimestamp
            );
#else
          EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.syncIngressTimestamp =
                                                EthTSyn_Ctrl[EthTSynCtrlIdx].RxIngressVirtualTime;
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
#endif /* ETHTSYN_MAX_SLAVE != 0U */
        break;
      }
      case ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_REQ:
      {
#if(ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE)
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
        (void)EthTSyn_Int01_ConvertEthTimeToVirtualTime
            (
                TimeStampPtr,
                &EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestReceiptTimestamp
            );
#else
          /* Get Ingress timestamp for pdelay_req on master side */
        /* !LINKSTO EthTSyn.ASR20-11.SWS_EthTSyn_00224, 1 */
        EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestReceiptTimestamp =
                                                EthTSyn_Ctrl[EthTSynCtrlIdx].RxIngressVirtualTime;
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
#endif /* ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE*/
        break;
      }
      case ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_RESP:
      {
#if(ETHTSYN_MAX_SLAVE != 0U)
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
        (void)EthTSyn_Int01_ConvertEthTimeToVirtualTime
            (
               TimeStampPtr,
               &EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responseReceiptTimestamp
            );
#else
          EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responseReceiptTimestamp =
                                                EthTSyn_Ctrl[EthTSynCtrlIdx].RxIngressVirtualTime;
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
#endif /* ETHTSYN_MAX_SLAVE != 0U */
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
        /* Unknown message type: Ignore */
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
        ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
        break; /* Caution: this 'break' is required by MISRA-C:2012 */
      }
      /* CHECK: PARSE */
    }
  }
#endif /* ETHTSYN_TIME_RECORDING == STD_ON */

  DBG_ETHTSYN_GETINGRESSTIMESTAMP_EXIT(RetVal, EthTSynCtrlIdx, RxDataPtr, TimeStampPtr);

  return RetVal;
}


STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_GetEgressTimeStamp
(
  uint8 EthTSynCtrlIdx,
  Eth_BufIdxType BufIdx,
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr
#else
  P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr
#endif
)
{
  Std_ReturnType RetVal;

  DBG_ETHTSYN_GETEGRESSTIMESTAMP_ENTRY(EthTSynCtrlIdx, BufIdx, TimeStampPtr);

#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  {
    Eth_TimeStampQualType TimeQuality;

    RetVal = EthIf_GetEgressTimeStamp
    (
      EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx, /* EthIf CtrlIdx */
      BufIdx, /* BufIdx */
      &TimeQuality,
      TimeStampPtr /* TimeStampPtr */
    );

    /* !LINKSTO EthTSyn.EB.InvTimeQuality.Master.SyncEgress, 1 */
    /* !LINKSTO EthTSyn.EB.InvTimeQuality.PdelayRespEgress, 1 */
    if((RetVal == E_OK) && (ETH_VALID == TimeQuality))
    {
      RetVal = E_OK;
    }
    else
    {
      RetVal = E_NOT_OK;
    }

    if(ETHTSYN_NANOSEC_MAX < TimeStampPtr->nanoseconds)
    {
      /* !LINKSTO EthTSyn.EB.Nanoseconds, 1 */
      RetVal = E_NOT_OK; /* Nanoseconds >= 1s is not allowed. */
    }
    /* !LINKSTO EthTSyn.EB.NanosecondsLimit, 1 */
  }
#else /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
  SchM_Enter_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_1();
  RetVal = StbM_GetCurrentVirtualLocalTime
      (
          EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId,
          TimeStampPtr
      );
  SchM_Exit_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_1();
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
  /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if(ETHTSYN_TIME_RECORDING == STD_ON)
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
  {
    if(BufIdx == EthTSyn_Ctrl[EthTSynCtrlIdx].Sync_BufIdx)
    {
#if(ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE)
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
      (void)EthTSyn_Int01_ConvertEthTimeToVirtualTime
        (
            TimeStampPtr,
            &EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.syncEgressTimestamp
        );
#else
      /* Save T2Vlt to be reported to StbM for time recording */
      EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.syncEgressTimestamp = *TimeStampPtr;
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
#endif /* ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE */
    }
    else if(BufIdx == EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResp_BufIdx)
    {
      /* Save Pdelay_Resp egress timestamp for slave */
#if(ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE)
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
      (void)EthTSyn_Int01_ConvertEthTimeToVirtualTime
        (
            TimeStampPtr,
            &EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responseOriginTimestamp
        );
#else
      EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responseOriginTimestamp =
                                                                                    *TimeStampPtr;
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
#endif /* ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE */
    }
    /* EthTSyn_GetEgressTimeStamp() is called only for Sync, Pdelay_fup and Pdelay_req frames,
       and false condition will never be reached, therefore check no parse is needed. */
    /* CHECK: NOPARSE */
    else if(BufIdx == EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReq_BufIdx)
    {
#if(ETHTSYN_MAX_SLAVE != 0U)
      /* Save Pdelay_Req egress timestamp for slave */
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
      (void)EthTSyn_Int01_ConvertEthTimeToVirtualTime
        (
            TimeStampPtr,
            &EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestOriginTimestamp
        );
#else
      EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestOriginTimestamp =
                                                                                   *TimeStampPtr;
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
#endif/* ETHTSYN_MAX_SLAVE != 0U */
    }
    /* CHECK: PARSE */
    /* CHECK: NOPARSE */
    else
    {
      /* This branch can not be reached since BufIdx will not match for other frames. */
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
      ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
    }
    /* CHECK: PARSE */
  }
#else
  TS_PARAM_UNUSED(BufIdx);
#endif/* ETHTSYN_TIME_RECORDING == STD_ON */

  DBG_ETHTSYN_GETEGRESSTIMESTAMP_EXIT(RetVal, EthTSynCtrlIdx, BufIdx, TimeStampPtr);

  return RetVal;
}

STATIC FUNC(uint16, ETHTSYN_CODE) EthTSyn_GetPTPHeaderSequNumber
(
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PTPHeaderPtr
)
{
  const uint16 SequNumber
    = (uint16)(((uint16)PTPHeaderPtr[ETHTSYN_PTP_HEADER_B30_SEQUNUMBER_1]) << 8U)
      + (uint16)(PTPHeaderPtr[ETHTSYN_PTP_HEADER_B31_SEQUNUMBER_0]);
  DBG_ETHTSYN_GETHEADERSEQUNUMBER_ENTRY(PTPHeaderPtr);

  DBG_ETHTSYN_GETHEADERSEQUNUMBER_EXIT(SequNumber, PTPHeaderPtr);

  return SequNumber;
}

#if((ETHTSYN_MAX_SLAVE != 0U) || (ETHTSYN_SWITCH_USED == STD_ON))
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_SubTimeStamps
(
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStamp_MinuendPtr,
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStamp_SubtrahendPtr,
  P2VAR(Eth_TimeIntDiffType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStamp_ResultPtr
)
{
  boolean Borrow;
  boolean Sign;

  Eth_TimeStampType TimeStamp_Minuend = *TimeStamp_MinuendPtr;
  Eth_TimeStampType TimeStamp_Subtrahend = *TimeStamp_SubtrahendPtr;

  DBG_ETHTSYN_SUBTIMESTAMPS_ENTRY(
                                   TimeStamp_MinuendPtr,
                                   TimeStamp_SubtrahendPtr,
                                   TimeStamp_ResultPtr
                                 );

  Sign = EthTSyn_TimeStamp_IsGreaterOrEqual(&TimeStamp_Minuend, &TimeStamp_Subtrahend);

  if(Sign == FALSE)
  {
    /* Negative */
    Eth_TimeStampType TmpTimeStamp = TimeStamp_Minuend;
    TimeStamp_Minuend = TimeStamp_Subtrahend;
    TimeStamp_Subtrahend = TmpTimeStamp;
  }

  if(TimeStamp_Minuend.nanoseconds >= TimeStamp_Subtrahend.nanoseconds)
  {
    TimeStamp_Minuend.nanoseconds -= TimeStamp_Subtrahend.nanoseconds;
    Borrow = FALSE;
  }
  else
  {
    uint32 DiffNanoSeconds = TimeStamp_Subtrahend.nanoseconds - TimeStamp_Minuend.nanoseconds;
    TimeStamp_Minuend.nanoseconds = (ETHTSYN_NANOSEC_MAX + 1U) - DiffNanoSeconds;
    Borrow = TRUE;
  }

  if(Borrow)
  {
    if(TimeStamp_Minuend.seconds > 0U)
    {
      TimeStamp_Minuend.seconds--;
      Borrow = FALSE;
    }
    else
    {
      TimeStamp_Minuend.seconds = ETHTSYN_32BIT_MAX; /* Set to maximum of 32 bit */
      Borrow = TRUE;
    }
  }

  if(TimeStamp_Minuend.seconds >= TimeStamp_Subtrahend.seconds)
  {
    TimeStamp_Minuend.seconds -= TimeStamp_Subtrahend.seconds;
  }
  else
  {
    uint32 DiffSeconds = TimeStamp_Subtrahend.seconds - TimeStamp_Minuend.seconds;
    TimeStamp_Minuend.seconds = ETHTSYN_32BIT_MAX - DiffSeconds;
    /* Borrow 1 secondHi which equals ETHTSYN_32BIT_MAX + 1.
     * It was necessary to separate +1 in order to avoid type overflow. */
    TimeStamp_Minuend.seconds++;
    Borrow = TRUE;
  }

  if(Borrow)
  {
    TimeStamp_Minuend.secondsHi--;
  }

  /* TimeStamp_Minuend.secondsHi is always >= TimeStamp_Subtrahend.secondsHi */
  TimeStamp_Minuend.secondsHi -= TimeStamp_Subtrahend.secondsHi;

  /* Write result. */
  TimeStamp_ResultPtr->sign = Sign;
  TimeStamp_ResultPtr->diff.secondsHi = TimeStamp_Minuend.secondsHi;
  TimeStamp_ResultPtr->diff.seconds = TimeStamp_Minuend.seconds;
  TimeStamp_ResultPtr->diff.nanoseconds = TimeStamp_Minuend.nanoseconds;

  DBG_ETHTSYN_SUBTIMESTAMPS_EXIT(
                                  TimeStamp_MinuendPtr,
                                  TimeStamp_SubtrahendPtr,
                                  TimeStamp_ResultPtr
                                );
}

STATIC FUNC(boolean, ETHTSYN_CODE) EthTSyn_TimeStamp_IsGreaterOrEqual
(
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStamp1Ptr,
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStamp2Ptr
)
{
  boolean Sign;

  DBG_ETHTSYN_TIMESTAMP_ISGREATEROREQUAL_ENTRY(TimeStamp1Ptr, TimeStamp2Ptr);

  if(TimeStamp1Ptr->secondsHi > TimeStamp2Ptr->secondsHi)
  {
    Sign = TRUE;
  }
  else if(TimeStamp1Ptr->secondsHi < TimeStamp2Ptr->secondsHi)
  {
    Sign = FALSE;
  }
  /* TimeStamp1Ptr->secondsHi == TimeStamp2Ptr->secondsHi */
  else if(TimeStamp1Ptr->seconds > TimeStamp2Ptr->seconds)
  {
    Sign = TRUE;
  }
  else if(TimeStamp1Ptr->seconds < TimeStamp2Ptr->seconds)
  {
    Sign = FALSE;
  }
  /* TimeStamp1Ptr->secondsHi == TimeStamp2Ptr->secondsHi */
  /* TimeStamp1Ptr->seconds == TimeStamp2Ptr->seconds */
  else if(TimeStamp1Ptr->nanoseconds >= TimeStamp2Ptr->nanoseconds)
  {
    Sign = TRUE; /* TimeStamp1Ptr >= TimeStamp2Ptr */
  }
  else
  {
    Sign = FALSE; /* TimeStamp1Ptr < TimeStamp2Ptr */
  }

  DBG_ETHTSYN_TIMESTAMP_ISGREATEROREQUAL_EXIT(Sign, TimeStamp1Ptr, TimeStamp2Ptr);

  return Sign;
}
#endif

FUNC(void, ETHTSYN_CODE) EthTSyn_AddTimeStamps
(
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStamp1Ptr,
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStamp2Ptr
)
{
  uint8 Idx;
  uint32 Overflow = 0U;
  uint32 LocalNanosec = TimeStamp2Ptr->nanoseconds;
  DBG_ETHTSYN_ADDTIMESTAMPS_ENTRY(TimeStamp1Ptr, TimeStamp2Ptr);

  for(Idx = 0U; Idx < 4U; Idx++)
  {
    if(TimeStamp1Ptr->nanoseconds > ETHTSYN_NANOSEC_MAX)
    {
      TimeStamp1Ptr->nanoseconds -= ETHTSYN_NANOSEC_MAX + 1U; /* - 1s */
      Overflow++;
    }
    if(LocalNanosec > ETHTSYN_NANOSEC_MAX)
    {
      LocalNanosec -= ETHTSYN_NANOSEC_MAX + 1U; /* - 1s */
      Overflow++;
    }
  }
  /* ANSI C90: A computation involving unsigned operands can
               never overflow, because a result that cannot be represented by the
               resulting unsigned integer type is reduced modulo the number that is
               one greater than the largest value that can be represented by the
               resulting unsigned integer type. */
  TimeStamp1Ptr->nanoseconds += LocalNanosec;

  if(TimeStamp1Ptr->nanoseconds > ETHTSYN_NANOSEC_MAX)
  {
    TimeStamp1Ptr->nanoseconds -= ETHTSYN_NANOSEC_MAX + 1U; /* - 1s */
    TimeStamp1Ptr->seconds++;
  }

  TimeStamp1Ptr->seconds += TimeStamp2Ptr->seconds;

  /* !LINKSTO EthTSyn.EB.Slave_OverflowDetection, 1 */
  if(TimeStamp1Ptr->seconds < TimeStamp2Ptr->seconds) /* Overflow occured */
  {
    TimeStamp1Ptr->secondsHi++;
  }

  TimeStamp1Ptr->seconds += Overflow;
    /* !LINKSTO EthTSyn.EB.Slave_OverflowDetection, 1 */
  if(TimeStamp1Ptr->seconds < Overflow) /* Overflow occured */
  {
    TimeStamp1Ptr->secondsHi++;
  }

  TimeStamp1Ptr->secondsHi += TimeStamp2Ptr->secondsHi;

  DBG_ETHTSYN_ADDTIMESTAMPS_EXIT(TimeStamp1Ptr, TimeStamp2Ptr);
}

#if (ETHTSYN_MAX_SLAVE != 0U)
STATIC FUNC(boolean, ETHTSYN_CODE) EthTSyn_IsSlave
(
  uint8 EthTSynCtrlIdx
)
{
  boolean RetVal;

  if(EthTSynCtrlIdx < ETHTSYN_MAX_SLAVE)
  {
    RetVal = TRUE;
  }
  else
  {
    RetVal = FALSE;
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_AddCorrField
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) CorrectionFieldArray,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr
)
{
  uint16 Idx;
  uint16 CorrFieldHi_ns;
  Eth_TimeStampType NsTimeStamp;
  uint16 RemainingCorrFieldHi_ns;
  Eth_TimeStampType CorrFieldHi_TS;
  Std_ReturnType RetVal = E_NOT_OK;

  CorrFieldHi_ns = (uint16)(((uint16) CorrectionFieldArray[0U]) << 8U);
  CorrFieldHi_ns += CorrectionFieldArray[1U];

  NsTimeStamp.nanoseconds = (uint32)(((uint32) CorrectionFieldArray[2U]) << 24U);
  NsTimeStamp.nanoseconds |= (uint32)(((uint32) CorrectionFieldArray[3U]) << 16U);
  NsTimeStamp.nanoseconds |= (uint32)(((uint32) CorrectionFieldArray[4U]) << 8U);
  NsTimeStamp.nanoseconds |= CorrectionFieldArray[5U];
  NsTimeStamp.seconds = 0U;
  NsTimeStamp.secondsHi = 0U;
  /* Save CorrectionField to be reported to StbM for time recording */
  /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if(ETHTSYN_TIME_RECORDING == STD_ON)  
  {
    uint64 CorrValue = 0U;

    CorrValue =  (uint64)(((uint64) CorrectionFieldArray[0U]) << 56U);
    CorrValue |= (uint64)(((uint64) CorrectionFieldArray[1U]) << 48U);
    CorrValue |= (uint64)(((uint64) CorrectionFieldArray[2U]) << 40U);
    CorrValue |= (uint64)(((uint64) CorrectionFieldArray[3U]) << 32U);
    CorrValue |= (uint64)(((uint64) CorrectionFieldArray[4U]) << 24U);
    CorrValue |= (uint64)(((uint64) CorrectionFieldArray[5U]) << 16U);
    CorrValue |= (uint64)(((uint64) CorrectionFieldArray[6U]) << 8U);
    CorrValue |= (uint64)((uint64) CorrectionFieldArray[7U]);

    EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.correctionField = (sint64)CorrValue;
  }
#endif /* ETHTSYN_TIME_RECORDING == STD_ON */
  /* !LINKSTO EthTSyn.EB.Slave.AddCorrField_NegativeCorrectionField, 1 */
  if((CorrFieldHi_ns > ETHTSYN_16BIT_MASK_BIT15) || /* Check if Correction Field is negative. */
      (
        /* !LINKSTO EthTSyn.EB.Slave.AddCorrField_MaxValues_NsTimeStamp_CorrFieldHi, 1 */
        /* Invalid value detected (See 802.1AS-2011 Ch 11.4.2.4) */
        (ETHTSYN_16BIT_SIGNED_MAX == CorrFieldHi_ns) &&
        (ETHTSYN_32BIT_MAX == NsTimeStamp.nanoseconds)
      )
    )
  {
    RetVal = E_NOT_OK;
  }
  else
  {
    if(CorrFieldHi_ns >= EthTSyn_Slave[EthTSynCtrlIdx].LastCorrFieldHi_ns)
    {
      /* Speed optimization: Add last converted values and only convert delta. */
      CorrFieldHi_TS = EthTSyn_Slave[EthTSynCtrlIdx].LastCorrFieldHi_TS;

      RemainingCorrFieldHi_ns = CorrFieldHi_ns - EthTSyn_Slave[EthTSynCtrlIdx].LastCorrFieldHi_ns;
    }
    else
    {
      RemainingCorrFieldHi_ns = CorrFieldHi_ns;

      CorrFieldHi_TS.nanoseconds = 0U;
      CorrFieldHi_TS.seconds = 0U;
      CorrFieldHi_TS.secondsHi = 0U;
    }

    /* !LINKSTO EthTSyn.EB.Slave.AddCorrField_RemainingCorrFieldHi_Ns_NotZero, 1 */
    /* Convert the remaining higher 15 bits (signed) of 48bit ns into  time stamp. */
    for(Idx = 0U; Idx < 15U; Idx++)
    {
      if(RemainingCorrFieldHi_ns == 0U)
      {
        /* Calculation finished for this CorrFieldHi_ns */
        break;
      }
      if((RemainingCorrFieldHi_ns & 0x0001U) == 1U)
      {
        EthTSyn_AddTimeStamps(&CorrFieldHi_TS, &EthTSyn_NsArrayConversionTable[Idx]);
      }

      RemainingCorrFieldHi_ns >>= 1U;
    }

    EthTSyn_AddTimeStamps(TimeStampPtr, &CorrFieldHi_TS);

    /* Store CorrFieldHi_ns for next function call. */
    EthTSyn_Slave[EthTSynCtrlIdx].LastCorrFieldHi_ns = CorrFieldHi_ns;
    /* Store calculated time stamp value corresponding to CorrFieldHi_ns. */
    EthTSyn_Slave[EthTSynCtrlIdx].LastCorrFieldHi_TS = CorrFieldHi_TS;

    /* Add ns part of correction field */
    /* !LINKSTO EthTSyn.EB.Slave.AddCorrField_NsPart_Of_CorrField, 1 */
    /* !LINKSTO EthTSyn.EB.Nanoseconds.CorrectionTime, 1 */
    if(NsTimeStamp.nanoseconds > ETHTSYN_NANOSEC_MAX)
    {
      NsTimeStamp.seconds += NsTimeStamp.nanoseconds / (ETHTSYN_NANOSEC_MAX + 1U);
      NsTimeStamp.nanoseconds %= (ETHTSYN_NANOSEC_MAX + 1U);
    }

    EthTSyn_AddTimeStamps(TimeStampPtr, &NsTimeStamp);

    RetVal = E_OK;
  }

  return RetVal;
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_CalculatePdelay
(
  uint8 EthTSynCtrlIdx
)
{
  uint32 ActualPdelay = 0U;
  boolean UpdatePdelay = FALSE;
  Eth_TimeStampType TimeStamp_t1 = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t1;
  Eth_TimeStampType TimeStamp_t2 = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t2;
  Eth_TimeStampType TimeStamp_t3 = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t3;
  Eth_TimeStampType TimeStamp_t4 = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t4;
  Eth_TimeIntDiffType TimeStampDiffT4T1;
  Eth_TimeIntDiffType TimeStampDiffT3T2;
  Eth_TimeIntDiffType PdelayDiff;

  EthTSyn_SubTimeStamps(
                         &TimeStamp_t4,
                         &TimeStamp_t1,
                         &TimeStampDiffT4T1 /* (t4 - t1) */
                       );

  EthTSyn_SubTimeStamps(
                         &TimeStamp_t3,
                         &TimeStamp_t2,
                         &TimeStampDiffT3T2 /* (t3 - t2) */
                       );

  /* !LINKSTO EthTSyn.EB.PdelayResp_NegativeTimeOffset, 1 */
  if((FALSE == TimeStampDiffT4T1.sign) || (FALSE == TimeStampDiffT3T2.sign))
  {
    UpdatePdelay = FALSE;
  }
  /* !LINKSTO EthTSyn.EB.TimeDiff_InvalidTime, 1 */
  else if((0U != TimeStampDiffT4T1.diff.secondsHi) || (0U != TimeStampDiffT3T2.diff.secondsHi))
  {
    UpdatePdelay = FALSE;
  }
  /* !LINKSTO EthTSyn.EB.TimeDiff_InvalidTime, 1 */
  else if((0U != TimeStampDiffT4T1.diff.seconds) || (0U != TimeStampDiffT3T2.diff.seconds))
  {
    UpdatePdelay = FALSE;
  }
  else if((ETHTSYN_NANOSEC_MAX < TimeStampDiffT4T1.diff.nanoseconds) ||
          (ETHTSYN_NANOSEC_MAX < TimeStampDiffT3T2.diff.nanoseconds))
  {
    /* !LINKSTO EthTSyn.EB.Nanoseconds, 1 */
    UpdatePdelay = FALSE; /* A time difference > 1s is suspicious. */
  }
  /* !LINKSTO EthTSyn.EB.NanosecondsLimit, 1 */
  else
  {
    EthTSyn_SubTimeStamps(
                           &TimeStampDiffT4T1.diff, /* T4 - T1 */
                           &TimeStampDiffT3T2.diff, /* T3 - T2 */
                           &PdelayDiff /* (T4 - T1) - (T3 - T2) */
                         );

    /* !LINKSTO EthTSyn.EB.TransparentClock_PdelayCalculation, 1 */
    ActualPdelay = PdelayDiff.diff.nanoseconds / 2U;
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00154, 1 */
    if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayLatencyThreshold >= ActualPdelay)
    {
      /* Check if Pdelay measurement is within expected range and valid. */
      /* !LINKSTO EthTSyn.EB.PdelayRespFU_NegativeTimeOffset, 1 */
      if(FALSE == PdelayDiff.sign) /* Pdelay must be positive. */
      {
        UpdatePdelay = FALSE;
      }
      else if(0U == EthTSyn_Slave[EthTSynCtrlIdx].Pdelay)
      {
        EthTSyn_Slave[EthTSynCtrlIdx].Pdelay = ActualPdelay; /* Init Pdelay */
        UpdatePdelay = FALSE; /* No need to update */
      }
      else
      {
        UpdatePdelay = TRUE;
      }
    }
  }

  /* Pdelay calculation: Add a fraction of delta to actual Pdelay. */
  if(UpdatePdelay)
  {
    uint32 Delta;
    const uint8 DeltaShift = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].FilterDeltaShift;

    if(ActualPdelay > EthTSyn_Slave[EthTSynCtrlIdx].Pdelay)
    {
      /* !LINKSTO EthTSyn.EB.Slave.PdelayFilter.Disabled, 1 */
      Delta = ActualPdelay - EthTSyn_Slave[EthTSynCtrlIdx].Pdelay;
      EthTSyn_Slave[EthTSynCtrlIdx].Pdelay += (uint32) (Delta >> DeltaShift);
    }
    else
    {
      /* !LINKSTO EthTSyn.EB.Slave.PdelayFilter.Enabled, 1 */
      Delta = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay - ActualPdelay;
      EthTSyn_Slave[EthTSynCtrlIdx].Pdelay -= (uint32) (Delta >> DeltaShift);
    }
  }

#if(ETHTSYN_TIME_RECORDING == STD_ON)
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
  {
    EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.pdelay = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay;
    /* Set the recorded timing measurement data for a pdelay of the
     *  corresponding Synchronized Time Base on Time Master */
    /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00221, 1 */
    (void)StbM_EthSetPdelayInitiatorData
        (
            EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId,
            /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00222, 1 */
            &EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording
        );
  }
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  /* If a TAB is configured, check if the timevalidation is enabled for the slave port. */
  else if (EthTSyn_RootCfgPtr->SwitchConfig[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].TimeValidationEnabled == TRUE)
  {
    EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.pdelay = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay;
    /* requestReceiptTimeStamp as received in pdelay_Response message */
    EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestReceiptTimestamp.
        seconds = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t2.seconds;
    EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestReceiptTimestamp.
        nanoseconds = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t2.nanoseconds;
    /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */

    /* responseOriginTimeStamp as received in pdelay_Response_Follow_Up */
    EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responseOriginTimestamp.seconds
        =  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t3.seconds;
    EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responseOriginTimestamp.
        nanoseconds = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t3.nanoseconds;

    /* Set the recorded timing measurement data for a pdelay of the
     *  corresponding Synchronized Time Base on Time Master */
    /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00221, 1 */
    (void)StbM_EthSetPdelayInitiatorData
        (
            EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId,
            /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00222, 1 */
            &EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording
        );
  }
  else
  {
    /* Nothing to do */
  }
#endif /* ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON */
#endif
}
#endif /* ETHTSYN_MAX_SLAVE != 0U */

#if (ETHTSYN_MAX_SLAVE != 0U)
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Init_SlaveVar
(
  uint8 EthTSynCtrlIdx
)
{
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00006, 1 */
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00008, 1 */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00140, 1 */
  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00010, 1 */
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00142, 1 */
  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay =
    EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].DefaultPdelayNs;
  /* LastCorrFieldHi_ns = maximum will be treated as invalid in the calculation. */
  EthTSyn_Slave[EthTSynCtrlIdx].LastCorrFieldHi_ns = ETHTSYN_16BIT_MAX;
  EthTSyn_Slave[EthTSynCtrlIdx].LastCorrFieldHi_TS.nanoseconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].LastCorrFieldHi_TS.seconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].LastCorrFieldHi_TS.secondsHi = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived = FALSE;
  EthTSyn_Slave[EthTSynCtrlIdx].OffsetTlvReceived = FALSE;
  /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00199_SkipCheckSCounterAfterInit, 1 */
  EthTSyn_Slave[EthTSynCtrlIdx].CheckSeqCounter = FALSE;
  EthTSyn_Slave[EthTSynCtrlIdx].ReceivedOffsetTimeDomId = 0xFFU;
  EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetTimeStamp.timeBaseStatus = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetTimeStamp.secondsHi = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetTimeStamp.seconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetTimeStamp.nanoseconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetUserData.userDataLength = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetUserData.userByte0 = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetUserData.userByte1 = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].StbMOffsetUserData.userByte2 = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].GlobalTimeFollowUpTimeout = 0U;
  EthTSyn_ResetSlavePdelayData(EthTSynCtrlIdx);
#if (ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED == TS_PROD_ERR_REP_TO_DET)
  EthTSyn_Slave[EthTSynCtrlIdx].SyncFupDroppedPairsCt = 0U;
#endif
#if (ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED == TS_PROD_ERR_REP_TO_DET)
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayRespFupDroppedPairsCt = 0U;
#endif
#if (ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED != TS_PROD_ERR_DISABLE)
  /* !LINKSTO EthTSyn.EB.Diagnostic_EthTSynReceiveSyncFupPairsPeriod, 1 */
  EthTSyn_Slave[EthTSynCtrlIdx].SyncFupPairTimeout =
        EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].ReceiveSyncFupPairTimeout;
#endif
#if (ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  /* !LINKSTO EthTSyn.EB.Diagnostic_EthTSynReceiveSyncFupPairsPeriod, 1 */
  EthTSyn_Slave[EthTSynCtrlIdx].BridgeLastSyncSentTime = 0U;
#endif
  /* If EthTSynTimeRecording is set to True */
#if(ETHTSYN_TIME_RECORDING == STD_ON)
  /* Detailed data for time validation of the Time Slave on Ethernet */
  /* Sequence Id of received Sync frame */
  EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sequenceId = 0U;
  /* SourcePortId from received Sync frame */
  EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.portNumber = 0U;
  /* Ingress timestamp of Sync frame converted to Virtual Local Time */
  EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.syncIngressTimestamp.nanosecondsLo = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.syncIngressTimestamp.nanosecondsHi = 0U;
  /* PreciseOriginTimestamp taken from the received Follow_Up frame */
  EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.preciseOriginTimestamp.nanoseconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.preciseOriginTimestamp.seconds = 0U;
  /* CorrectionField taken from the received Follow_Up frame */
  EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.correctionField = 0;
  /* Currently valid pDelay value */
  EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.pDelay = 0U;
  /* SyncLocal Time Tuple (Virtual Local Time part) */
  EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.referenceLocalTimestamp.nanosecondsLo
                                                                                            = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.referenceLocalTimestamp.nanosecondsHi
                                                                                            = 0U;
  /* SyncLocal Time Tuple (Global Time part) */
  EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.referenceGlobalTimestamp.nanoseconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.referenceGlobalTimestamp.seconds = 0U;

  /* Detailed timing data for the pDelay Initiator */
  /* Sequence Id of sent Pdelay_Req frame */
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.sequenceId = 0U;
  /* SourcePortId of sent Pdelay_Req frame */
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.portNumber = 0U;
  /* SourcePortId of sent Pdelay_Resp frame */
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.portNumber = 0U;
  /* Egress timestamp of Pdelay_Req in Virtual Local Time */
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestOriginTimestamp.
                                                                              nanosecondsLo = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestOriginTimestamp.
                                                                              nanosecondsHi = 0U;
  /* Ingress timestamp of Pdelay_Resp in Virtual Local Time */
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responseReceiptTimestamp.
                                                                              nanosecondsLo = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responseReceiptTimestamp.
                                                                              nanosecondsHi = 0U;

  /* Ingress timestamp of Pdelay_Req in Global Time taken from the received Pdelay_Resp */
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestReceiptTimestamp.
                                                                               nanoseconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestReceiptTimestamp.
                                                                               seconds = 0U;

  /* Egress timestamp of Pdelay_Resp in Global Time taken from the
     received Pdelay_Resp_Follow_Up */
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responseOriginTimestamp.
                                                                               nanoseconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responseOriginTimestamp.
                                                                               seconds = 0U;
  /* Value of the Virtual Local Time of the reference Global Time Tuple */
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.referenceLocalTimestamp.
                                                                              nanosecondsLo = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.referenceLocalTimestamp.
                                                                              nanosecondsHi = 0U;
  /* Value of the local instance of the Global Time of the reference Global Time Tuple */
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.referenceGlobalTimestamp.
                                                                               nanoseconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.referenceGlobalTimestamp.
                                                                               seconds = 0U;
#endif

#if((ETHTSYN_DEV_ERROR_DETECT == STD_ON) && (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON))
  TS_MemBZero(
               &EthTSyn_Slave[EthTSynCtrlIdx].Sync_Rx_SrcPortIdentity[0U],
               ETHTSYN_SOURCEPORTIDENTITY_SIZE
             );
  EthTSyn_Slave[EthTSynCtrlIdx].SaveSrcPortId = TRUE;
#endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) && (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON) */

#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  EthTSyn_Slave[EthTSynCtrlIdx].TimeDiffHostEgressSlaveIngress.diff.nanoseconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].TimeDiffHostEgressSlaveIngress.diff.seconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].TimeDiffHostEgressSlaveIngress.diff.secondsHi = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].TimeDiffHostEgressSlaveIngress.sign = FALSE;
  EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.diff.nanoseconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.diff.seconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.diff.secondsHi = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.sign = FALSE;
  /* Boundary clock transmit sync frames . */
  EthTSyn_Slave[EthTSynCtrlIdx].TriggerTxOnBridge = FALSE;
#endif
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ResetSlavePdelayData
(
  uint8 EthTSynCtrlIdx
)
{
  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t1.nanoseconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t1.seconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t1.secondsHi = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t2.nanoseconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t2.seconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t2.secondsHi = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t3.nanoseconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t3.seconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t3.secondsHi = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t4.nanoseconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t4.seconds = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t4.secondsHi = 0U;
  EthTSyn_Slave[EthTSynCtrlIdx].PdelayFollowUpTimeout = 0xFFFFFFFFU;
  TS_AtomicAssign8
  (
      EthTSyn_Slave[EthTSynCtrlIdx].CalculatePdelay,
      ETHTSYN_PDELAY_CALCULATION_IDLE
  );
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  EthTSyn_Inc02_ResetPortPdelayVars(EthTSynCtrlIdx);
#endif
}
#endif

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Init_CtrlVar
(
  uint8 EthTSynCtrlIdx
)
{
  EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber = 0xFFFFU; /* Maximum */
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqSequNumber = 0xFFFFU; /* Maximum */
  EthTSyn_Ctrl[EthTSynCtrlIdx].Ingress_PdelayReqSequNumber = 0U;
  /* This must be aligned to StbM, timeBaseUpdateCounter initial value:
   * requirement SWS_StbM_00344 , initial value must be 0U */
  EthTSyn_Ctrl[EthTSynCtrlIdx].LastTimeBaseUpdateCounter = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].LinkState = ETHTRCV_LINK_STATE_DOWN;
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResp_BufIdx = ETHTSYN_INV_BUFIDX;
  EthTSyn_Ctrl[EthTSynCtrlIdx].Sync_BufIdx = ETHTSYN_INV_BUFIDX;
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReq_BufIdx = ETHTSYN_INV_BUFIDX;
  EthTSyn_Ctrl[EthTSynCtrlIdx].TxMode = ETHTSYN_TX_OFF;
  EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset = ETHTSYN_INV_DEBOUNCE_VALUE;
  EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].StbMTimeStamp.nanoseconds = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].StbMTimeStamp.seconds = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].StbMTimeStamp.secondsHi = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].StbMTimeStamp.timeBaseStatus = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userDataLength = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte0 = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte1 = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte2 = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].StbMVirtualLocalTime.nanosecondsLo = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].StbMVirtualLocalTime.nanosecondsHi = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].ImmediateTransIsLocked = FALSE;
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  TS_MemBZero(&EthTSyn_Ctrl[EthTSynCtrlIdx].ReceivedFupFrame[0], ETHTSYN_SYNCFUP_FRAME_MAX_SIZE);
  EthTSyn_Ctrl[EthTSynCtrlIdx].ReceivedFupFrameLength = 0U;
#endif

#if (ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED != TS_PROD_ERR_DISABLE)
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayRespFupPairTimeoutInit = FALSE;
#endif
#if((ETHTSYN_TIME_RECORDING == STD_ON) && (ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE))
  EthTSyn_Init_TimeRecordingVar(EthTSynCtrlIdx);
#endif
#if(ETHTSYN_SWITCH_USED == STD_ON)
  {
    EthTSyn_IntPortIdxType Idx = 0U;
    for(
         Idx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart;
         Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
         EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
         Idx++
       )
    {
      EthTSyn_SwitchPorts[Idx].EgressPdelayFrameTimeStamp.nanoseconds = 0U;
      EthTSyn_SwitchPorts[Idx].EgressPdelayFrameTimeStamp.seconds = 0U;
      EthTSyn_SwitchPorts[Idx].EgressPdelayFrameTimeStamp.secondsHi = 0U;
      EthTSyn_SwitchPorts[Idx].Frame2Transmit = 0U;
      EthTSyn_SwitchPorts[Idx].IngPdelayFrameTimeStamp.nanoseconds = 0U;
      EthTSyn_SwitchPorts[Idx].IngPdelayFrameTimeStamp.seconds = 0U;
      EthTSyn_SwitchPorts[Idx].IngPdelayFrameTimeStamp.secondsHi = 0U;
      EthTSyn_SwitchPorts[Idx].Ingress_PdelayReqSequNumber = 0U;
      /* !LINKSTO EthTSyn.Dsn.PortPdelayState_Init, 1 */
      EthTSyn_SwitchPorts[Idx].PdelayState = ETHTSYN_PORT_PDELAY_STATE_IDLE;
      EthTSyn_SwitchPorts[Idx].PdelayUniqueId = ETHTSYN_UNIQUE_ID_INVALID;
      EthTSyn_SwitchPorts[Idx].SyncTimeStamp.nanoseconds = 0U;
      EthTSyn_SwitchPorts[Idx].SyncTimeStamp.seconds = 0U;
      EthTSyn_SwitchPorts[Idx].SyncTimeStamp.secondsHi = 0U;
      /* !LINKSTO EthTSyn.Dsn.HostPortSynTxState_INIT, 1 */
      /* !LINKSTO EthTSyn.Dsn.PortSynTxState_INIT, 1 */
      EthTSyn_SwitchPorts[Idx].SyncTxState = ETHTSYN_PORT_SYN_TX_STATE_IDLE;
      EthTSyn_SwitchPorts[Idx].UniqueId = ETHTSYN_UNIQUE_ID_INVALID;

      TS_MemBZero(
                   &EthTSyn_SwitchPorts[Idx].Pdelay_Rx_SrcPortIdentity[0U],
                   ETHTSYN_SOURCEPORTIDENTITY_SIZE
                 );
    }
  }
#endif

  TS_MemBZero(
               &EthTSyn_Ctrl[EthTSynCtrlIdx].Pdelay_Tx_SrcPortIdentity[0U],
               ETHTSYN_SOURCEPORTIDENTITY_SIZE
             );
  TS_MemBZero(
               &EthTSyn_Ctrl[EthTSynCtrlIdx].Pdelay_Rx_SrcPortIdentity[0U],
               ETHTSYN_SOURCEPORTIDENTITY_SIZE
             );
  /* Send next frame immediately. */
  EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextSyncFrame_Timeout = 0U;

  /* Set Pdelay send timeout initial value different to the sync send timeout to reduce
     traffic on bus. */
  EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextPdelayReqFrame_Timeout
    = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayReqTxInterval;

  /* Initialize the time flags */
  EthTSyn_Ctrl[EthTSynCtrlIdx].Crc_Time_Flags = 0U;

  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].CrcMessageLength == TRUE)
  {
    EthTSyn_Ctrl[EthTSynCtrlIdx].Crc_Time_Flags |= ETHTSYN_MESSAGE_LENGTH_BIT_MASK;
  }
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].CrcDomainNumber == TRUE)
  {
    EthTSyn_Ctrl[EthTSynCtrlIdx].Crc_Time_Flags |= ETHTSYN_DOMAIN_NUMBER_BIT_MASK;
  }
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].CrcCorrectionField == TRUE)
  {
    EthTSyn_Ctrl[EthTSynCtrlIdx].Crc_Time_Flags |= ETHTSYN_CORRECTION_FIELD_BIT_MASK;
  }
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].CrcSourcePortIdentity == TRUE)
  {
    EthTSyn_Ctrl[EthTSynCtrlIdx].Crc_Time_Flags |= ETHTSYN_SOURCE_PORT_IDENTITY_BIT_MASK;
  }
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].CrcSequenceId == TRUE)
  {
    EthTSyn_Ctrl[EthTSynCtrlIdx].Crc_Time_Flags |= ETHTSYN_SEQUENCE_ID_BIT_MASK;
  }
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].CrcPreciseOriginTimestamp == TRUE)
  {
    EthTSyn_Ctrl[EthTSynCtrlIdx].Crc_Time_Flags |= ETHTSYN_PRECISE_ORIGIN_TIMESTAMP_BIT_MASK;
  }
}

#if((ETHTSYN_TIME_RECORDING == STD_ON) && (ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE))
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Init_TimeRecordingVar
(
  uint8 EthTSynCtrlIdx
)
{
  /* Detailed data for time validation of the Time Master on Ethernet */
  /* SequenceId of sent Ethernet frame */
  EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sequenceId = 0U;
  /* SourcePortId of sending Ethernet port */
  EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.portNumber = 0U;
  /* Egress timestamp of Sync frame */
  EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.syncEgressTimestamp.nanosecondsLo = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.syncEgressTimestamp.nanosecondsHi = 0U;
  /* Egress timestamp of Sync frame in Global Time */
  EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.preciseOriginTimestamp.seconds = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.preciseOriginTimestamp.nanoseconds = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.correctionField = 0U;

  /* Timing data for the pDelay Responder */
  /* SequenceId of received Pdelay_Req frame */
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.sequenceId = 0U;
  /* SourcePortId of received Pdelay_Req frame */
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestPortId.clockIdentity = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestPortId.portNumber = 0U;
  /* SourcePortId of sent Pdelay_Resp frame */
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responsePortId.portNumber = 0U;
  /* Ingress timestamp of Pdelay_Req converted to Virtual Local Time */
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestReceiptTimestamp.
                                                                              nanosecondsLo = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.requestReceiptTimestamp.
                                                                              nanosecondsHi = 0U;
  /* Egress timestamp of Pdelay_Resp converted to Virtual Local Time */
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responseOriginTimestamp.
                                                                              nanosecondsLo = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.responseOriginTimestamp.
                                                                              nanosecondsHi = 0U;
  /* Value of the Virtual Local Time of the reference Global Time Tuple used to convert
     requestReceiptTimestamp and responseOriginTimestamp into Global Time*/
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.referenceLocalTimestamp.
                                                                              nanosecondsLo = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.referenceLocalTimestamp.
                                                                              nanosecondsHi = 0U;
  /* Value of the local instance of the Global Time of the reference Global Time Tuple used to
     convert requestReceiptTimestamp and responseOriginTimestamp into Global Time*/
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.referenceGlobalTimestamp.
                                                                              nanoseconds = 0U;
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayResponderTimeRecording.referenceGlobalTimestamp.
                                                                              seconds = 0U;

  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayRespFup_BufIdx = ETHTSYN_INV_BUFIDX;

}
#endif

STATIC FUNC(boolean, ETHTSYN_CODE) EthTSyn_NextMinDebounceOffset
(
  P2VAR(uint32, AUTOMATIC, ETHTSYN_APPL_DATA) CommonOffset,
  uint32 SpecificOffset
)
{
  boolean SpecificOffsetUpdated = FALSE;

  if(*CommonOffset == ETHTSYN_INV_DEBOUNCE_VALUE)
  {
    *CommonOffset = SpecificOffset;
    SpecificOffsetUpdated = TRUE;
  }
  else if((EthTSyn_GeneralCounter <= *CommonOffset) && (EthTSyn_GeneralCounter <= SpecificOffset))
  {
    /* both GlobalMinDebounceTimeOffset and EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset
       have no wrap around */
    if(*CommonOffset > SpecificOffset)
    {
      *CommonOffset = SpecificOffset;
      SpecificOffsetUpdated = TRUE;
    }
    /* else do nothing */
  }
  else if((EthTSyn_GeneralCounter <= *CommonOffset) && (EthTSyn_GeneralCounter > SpecificOffset))
  {
    /* nothing to do GlobalMinDebounceTimeOffset has no wrap around while EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset has. */
  }
  else if((EthTSyn_GeneralCounter > *CommonOffset) && (EthTSyn_GeneralCounter <= SpecificOffset))
  {
    *CommonOffset = SpecificOffset;
    SpecificOffsetUpdated = TRUE;
  }
  /* CHECK: NOPARSE */
  /* False condition is unreachable, but NOPARSE needs to be added before "else if" branch.
   * Remaining branches inside "else if" are already covered by tests. */
  else if((EthTSyn_GeneralCounter > *CommonOffset) && (EthTSyn_GeneralCounter > SpecificOffset))
  {
    if(*CommonOffset > SpecificOffset)
    {
      *CommonOffset = SpecificOffset;
      SpecificOffsetUpdated = TRUE;
    }
  }
  else
  {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif
  }
  /* CHECK: PARSE */

  return SpecificOffsetUpdated;
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ResetSynTxStateMachine
(
  uint8 EthTSynCtrlIdx
)
{
#if(ETHTSYN_SWITCH_USED == STD_ON)
  EthTSyn_IntPortIdxType Idx;

  /* cleanup */
  for(
        Idx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart;
        Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
              EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
        Idx++
     )
  {
    TS_AtomicAssign8
    (
      EthTSyn_SwitchPorts[Idx].SyncTxState,
      ETHTSYN_PORT_SYN_TX_STATE_IDLE
    );
    EthTSyn_SwitchPorts[Idx].SyncTimeStamp.nanoseconds = 0U;
    EthTSyn_SwitchPorts[Idx].SyncTimeStamp.seconds = 0U;
    EthTSyn_SwitchPorts[Idx].SyncTimeStamp.secondsHi = 0U;
    EthTSyn_SwitchPorts[Idx].UniqueId = ETHTSYN_UNIQUE_ID_INVALID;
  }
#endif /* ETHTSYN_SWITCH_USED == STD_ON */

  TS_AtomicAssign8
  (
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState,
    ETHTSYN_CTRL_SYN_TX_STATE_IDLE
  );
}
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_MainFunctionTimeSynMaster
(
  uint8 EthTSynCtrlIdx
)
{
  const StbM_SynchronizedTimeBaseType TimeBaseId
    = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId;
  uint8 TimeBaseUpdateCounter;
  Std_ReturnType RetVal;

  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00135, 1 */
  if(
      (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].ImmediateTimeSync) &&
      (!EthTSyn_Ctrl[EthTSynCtrlIdx].ImmediateTransIsLocked)
    )
  {
    TimeBaseUpdateCounter = StbM_GetTimeBaseUpdateCounter(TimeBaseId);
    if (TimeBaseUpdateCounter != EthTSyn_Ctrl[EthTSynCtrlIdx].LastTimeBaseUpdateCounter)
    {
      StbM_TimeBaseStatusType SyncTimeBaseStatus;
      StbM_TimeBaseStatusType OffsetTimeBaseStatus;
      EthTSyn_Ctrl[EthTSynCtrlIdx].LastTimeBaseUpdateCounter = TimeBaseUpdateCounter;

      RetVal = StbM_GetTimeBaseStatus(TimeBaseId, &SyncTimeBaseStatus,
                                      &OffsetTimeBaseStatus
                                     );
      if(
          ((SyncTimeBaseStatus & ETHTSYN_GLOBAL_TIME_BASE_MASK) != 0U) &&
          (RetVal == E_OK)
        )
      {
        uint16 SyncSequNumberImm;
        StbM_UserDataType UserDataImm;
        StbM_TimeStampType StbMTimeStampImm;
        StbM_VirtualLocalTimeType StbMVirtualLocalTimeImm;
        StbMVirtualLocalTimeImm.nanosecondsLo = 0U;
        StbMVirtualLocalTimeImm.nanosecondsHi = 0U;

        TS_AtomicAssign16(SyncSequNumberImm, EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber);
        /* Increment sequence number. The Sequence Counter shall wrap around at 65535 to 0 again. */
        /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00189, 1 */
        if(ETHTSYN_SEQ_NUM_MAX == SyncSequNumberImm)
        {
          /* The first transmission of the Sync frame shall contain seqId 0.*/
          /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00187_Sync, 1 */
          SyncSequNumberImm = 0U;
        }
        else
        {
          SyncSequNumberImm++;
        }

        TS_AtomicAssign16(EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber, SyncSequNumberImm);
        /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00202, 1 */
        RetVal = StbM_BusGetCurrentTime
          (
            EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId, /* timeBaseId */
            &StbMTimeStampImm, /* timeStampPtr */
            &StbMVirtualLocalTimeImm, /* localTimePtr */
            &UserDataImm /* userDataPtr */
          );

        /* !LINKSTO EthTSyn.EB.BusGetCurrentTime_NOK, 1 */
        if(RetVal == E_OK)
        {
          EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userDataLength = UserDataImm.userDataLength;
          EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte0 = UserDataImm.userByte0;
          EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte1 = UserDataImm.userByte1;
          EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte2 = UserDataImm.userByte2;

          EthTSyn_Ctrl[EthTSynCtrlIdx].StbMTimeStamp = StbMTimeStampImm;
          EthTSyn_Ctrl[EthTSynCtrlIdx].StbMVirtualLocalTime = StbMVirtualLocalTimeImm;

          /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00135, 1 */
          if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount == 0U)
          {
             ETHTSYN_SET_TXFLAG
              (&EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, ETHTSYN_BITPOS_TX_SYNC, TRUE);
          }
#if(ETHTSYN_SWITCH_USED == STD_ON)
          else
          {
            ETHTSYN_SET_TXFLAG
              (&EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, ETHTSYN_BITPOS_TX_SWT, TRUE);
            TS_AtomicSetBit_8
            (
              &EthTSyn_SwitchPorts
                 [EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].Frame2Transmit,
              ETHTSYN_BITPOS_TX_SYNC_ON_HOST
            );
          }
#endif
          EthTSyn_Ctrl[EthTSynCtrlIdx].ImmediateTransIsLocked = TRUE;
          /* Set timeout for next Sync frame. */
          /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00136, 1 */
          /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00137.1, 1 */
          /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00137.2, 1 */
          EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextSyncFrame_Timeout
            = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].CyclicMsgResumeTime;
        }
      }
    }
  }

  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SyncTxInterval != 0U)
  {
    if(0U < EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextSyncFrame_Timeout)
    {
      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00137.2, 1 */
      EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextSyncFrame_Timeout--;
    }

    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00139, 1 */
    if(0U == EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextSyncFrame_Timeout)
    {
      StbM_TimeBaseStatusType syncTimeBaseStatus;
      StbM_TimeBaseStatusType offsetTimeBaseStatus;

      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00202, 1 */
      EthTSyn_Ctrl[EthTSynCtrlIdx].ImmediateTransIsLocked = FALSE;
      RetVal = StbM_GetTimeBaseStatus(TimeBaseId, &syncTimeBaseStatus, &offsetTimeBaseStatus);
      TS_PARAM_UNUSED(offsetTimeBaseStatus);
      /* !LINKSTO EthTSyn.EB.Slave.GetTimeBaseStatus.NotOk, 1 */
      if(RetVal == E_OK)
      {
#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
        /* Store the time base status */
        EthTSyn_MasterTimeBaseStatus.GlobalTimebaseStatus = syncTimeBaseStatus;
#endif
        /* !LINKSTO EthTSyn.ASR151.PRS_TS_00016_TxPeriodZero, 1 */
        /* !LINKSTO EthTSyn.ASR151.PRS_TS_00016, 1 */
        if((syncTimeBaseStatus & ETHTSYN_GLOBAL_TIME_BASE_MASK) != 0U)
        {
          uint16 SyncSequNumber;
          StbM_UserDataType UserData;
          StbM_TimeStampType StbMTimeStamp;
          StbM_VirtualLocalTimeType StbMVirtualLocalTime;

          StbMVirtualLocalTime.nanosecondsLo = 0U;
          StbMVirtualLocalTime.nanosecondsHi = 0U;

          TS_AtomicAssign16(SyncSequNumber, EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber);
          /* Increment sequence number. The Sequence Counter shall wrap around at 65535 to 0 again. */
          /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00189, 1 */
          if(ETHTSYN_SEQ_NUM_MAX == SyncSequNumber)
          {
            /* The first transmission of the Sync frame shall contain seqId 0.*/
            /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00187_Sync, 1 */
            SyncSequNumber = 0U;
          }
          else
          {
            SyncSequNumber++;
          }

          TS_AtomicAssign16(EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber, SyncSequNumber);
          /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00202, 1 */
          RetVal = StbM_BusGetCurrentTime
            (
              EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId, /* timeBaseId */
              &StbMTimeStamp, /* timeStampPtr */
              &StbMVirtualLocalTime, /* localTimePtr */
              &UserData /* userDataPtr */
            );

          /* !LINKSTO EthTSyn.EB.BusGetCurrentTime_NOK, 1 */
          if(RetVal == E_OK)
          {
            EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userDataLength = UserData.userDataLength;
            EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte0 = UserData.userByte0;
            EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte1 = UserData.userByte1;
            EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte2 = UserData.userByte2;

            EthTSyn_Ctrl[EthTSynCtrlIdx].StbMTimeStamp = StbMTimeStamp;
            EthTSyn_Ctrl[EthTSynCtrlIdx].StbMVirtualLocalTime = StbMVirtualLocalTime;

            /* !LINKSTO EthTSyn.EB.Master.DisableAnnounce, 1 */
#if(ETHTSYN_SEND_ANNOUNCEFRAMES == STD_ON)
            /* !LINKSTO EthTSyn.EB.Master.SendPeriodicAnnounce, 1 */
            EthTSyn_TransmitAnnounceFrame(EthTSynCtrlIdx);
#endif /* ETHTSYN_SEND_ANNOUNCEFRAMES == STD_ON) */
            /* Master: Send sync frame. */
            /* !LINKSTO EthTSyn.EB.Master.SendPeriodicSync, 1 */
            if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount == 0U)
            {
              ETHTSYN_SET_TXFLAG
                (&EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, ETHTSYN_BITPOS_TX_SYNC, TRUE);
            }
#if(ETHTSYN_SWITCH_USED == STD_ON)
            else
            {
              ETHTSYN_SET_TXFLAG
                (&EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, ETHTSYN_BITPOS_TX_SWT, TRUE);
              TS_AtomicSetBit_8
              (
                &EthTSyn_SwitchPorts
                   [EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].Frame2Transmit,
                ETHTSYN_BITPOS_TX_SYNC_ON_HOST
              );

            }
#endif
            /* Set timeout for next Sync frame. */
            EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextSyncFrame_Timeout
              = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SyncTxInterval;
          }
        }
      }
    }
  }

  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount == 0U)
  {
    if(
        EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState
        == ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_FUP
      )
    {
      ETHTSYN_SET_TXFLAG
        (&EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, ETHTSYN_BITPOS_TX_FUP, TRUE);
      /* cleanup */
      EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState = ETHTSYN_CTRL_SYN_TX_STATE_IDLE;
    }
  }
}

#if(ETHTSYN_MAX_SLAVE != 0U)
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_MainFunctionTimeSynSlave
(
  uint8 EthTSynCtrlIdx
)
{
#if (ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED != TS_PROD_ERR_DISABLE)
  /* CHECK: NOPARSE */
  if(0U < EthTSyn_Slave[EthTSynCtrlIdx].SyncFupPairTimeout)
  {
     EthTSyn_Slave[EthTSynCtrlIdx].SyncFupPairTimeout--;
  }
  else
  {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif
  }
  /* CHECK: PARSE */

  if(0U == EthTSyn_Slave[EthTSynCtrlIdx].SyncFupPairTimeout)
  {
    /* !LINKSTO EthTSyn.EB.Diagnostic_EthTSynReceiveSyncFupPairsPeriod_Expire, 1 */
    /* Sync/Fup frames are missing , increase the value of SyncFupDroppedPairsCt and
     * eventually report to Dem\Det */
    EthTSyn_SyncFupDropped(EthTSynCtrlIdx);
  }

#endif

#if (ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED != TS_PROD_ERR_DISABLE)
  if(EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayRespFupPairTimeoutInit == TRUE)
  {
    /* CHECK: NOPARSE */
    if(0U < EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayRespPairTimeout)
    {
      EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayRespPairTimeout--;
    }
    else
    {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
      ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif
    }
    /* CHECK: PARSE */
    /* !LINKSTO EthTSyn.EB.Diagnostic_EthTSynReceivePdelayRespFupPairsPeriod, 1 */
    /* !LINKSTO EthTSyn.EB.Diagnostic_EthTSynReceivePdelayRespFupPairsPeriod_Expire, 1 */
    if(0U == EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayRespPairTimeout)
    {
      /* PdelayResp/PdelayRespFup frames are missing , increase the value of
         PdelayRespFupDroppedPairsCt and eventually report to Dem\Det */
      EthTSyn_PDelayFrameDropped(EthTSynCtrlIdx);
      /* Abort the pdelay calculation */
      EthTSyn_ResetSlavePdelayData(EthTSynCtrlIdx);
    }
  }
#endif
  /* If the followup timeout is configured and the sync frame was received, begin the timeout observation. */
  if(
      (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].GlobalTimeFollowUpTimeout > 0U) &&
      (EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived)
    )
  {
    if(EthTSyn_Slave[EthTSynCtrlIdx].GlobalTimeFollowUpTimeout == 0U)
    {
      /* Timeout expired, reset the state */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00025, 1 */
      /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00197, 1 */
      TS_AtomicAssign8(EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived, FALSE);
    }
    else
    {
      EthTSyn_Slave[EthTSynCtrlIdx].GlobalTimeFollowUpTimeout--;
    }
  }

  if((EthTSyn_Slave[EthTSynCtrlIdx].CalculatePdelay) == ETHTSYN_READY_FOR_PDELAY_CALCULATION)
  {
    EthTSyn_CalculatePdelay(EthTSynCtrlIdx);

    /* Pdelay calculation done, reset the variables */
    EthTSyn_ResetSlavePdelayData(EthTSynCtrlIdx);
  }

  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00164.TimeoutDeactivated, 1 */
  if(
      (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayRespAndRespFollowUpTimeout > 0U) &&
      /* Handle the timeout only if a pdelay calculation was started */
      (EthTSyn_Slave[EthTSynCtrlIdx].CalculatePdelay != ETHTSYN_PDELAY_CALCULATION_IDLE)
    )
  {
    if(0U == EthTSyn_Slave[EthTSynCtrlIdx].PdelayFollowUpTimeout)
    {
      /* If a reception timeout occurs (refer to [PRS_TS_00164]), any received
         Pdelay_Resp resp Pdelay_Resp_Follow_Up shall be ignored, until a new
         Pdelay_Req has been sent. */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00210, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00004, 1 */
      /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00193, 1 */
      /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00195, 1 */
      EthTSyn_ResetSlavePdelayData(EthTSynCtrlIdx);
    }
    else
    {
      EthTSyn_Slave[EthTSynCtrlIdx].PdelayFollowUpTimeout--;
    }
  }
}
#endif

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TriggerPeriodicFrames(void)
{

  uint8 EthTSynCtrlIdx;

  for(
       EthTSynCtrlIdx = 0U;
       EthTSynCtrlIdx < EthTSyn_RootCfgPtr->MaxEthTSynCtrlIdx;
       EthTSynCtrlIdx++
     )
  {
    uint8 TxMode;
    TS_AtomicAssign8(TxMode, EthTSyn_Ctrl[EthTSynCtrlIdx].TxMode);

    /* Send frame only if active link available. */
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00021, 1 */
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00022, 1 */
    if(ETHTSYN_TX_ON == TxMode)
    {
      uint8 LinkState;

      TS_AtomicAssign8(LinkState, EthTSyn_Ctrl[EthTSynCtrlIdx].LinkState);

      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00020, 1 */
      if(ETHTRCV_LINK_STATE_ACTIVE == LinkState)
      {
          /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00134, 1 */
          if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeSyncMaster)
          {
            EthTSyn_MainFunctionTimeSynMaster(EthTSynCtrlIdx);
          }
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
          else if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == FALSE)
          {
            /* Check if the sync period expired and no sync was received.*/
            /* Save the time of the last transmitted frame */
            if((EthTSyn_GeneralCounter - EthTSyn_Slave[EthTSynCtrlIdx].BridgeLastSyncSentTime) >=
                                    EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].BridgeTxPeriod)
            {
              /* ready to transmit, on the next mainfunction, the sync to the connected slaves */
              /* Boundary clock transmit sync frames . */
              EthTSyn_Slave[EthTSynCtrlIdx].TriggerTxOnBridge = TRUE;
            }

            if(EthTSyn_Slave[EthTSynCtrlIdx].TriggerTxOnBridge == TRUE)
            {
              /* Begin transmission */
              EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextSyncFrame_Timeout = 0U;
              EthTSyn_MainFunctionTimeSynMaster(EthTSynCtrlIdx);
              /* Save the time of the last transmitted frame */
              EthTSyn_Slave[EthTSynCtrlIdx].BridgeLastSyncSentTime = EthTSyn_GeneralCounter;
              /* Reset relevant boundary variables */
              EthTSyn_Slave[EthTSynCtrlIdx].TriggerTxOnBridge = FALSE;
              EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextSyncFrame_Timeout = 0xFFFFFFFFU;
            }
          }
          else
          {
            /* Nothing to do */
          }
#endif
          /* !LINKSTO EthTSyn.ASR151.PRS_TS_00011, 1 */
          /* !LINKSTO EthTSyn.ASR151.PRS_TS_00141, 1 */
          if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayReqTxInterval != 0U)
          {
            /* Master/Slave: Send Pdelay_Req frame. */
            /* !LINKSTO EthTSyn.EB.Master_TransmitPdelayRequestFrame, 1 */
            if(0U < EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextPdelayReqFrame_Timeout)
            {
              EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextPdelayReqFrame_Timeout--;
            }
            /* Else branch - do nothing - retry the transmission in the next EthTSyn_MainFunction() */
            /* !LINKSTO EthTSyn.EB.PdelayReqTransmit_Retry, 1 */
            if(0U == EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextPdelayReqFrame_Timeout)
            {
              /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00135, 1 */
              if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount == 0U)
              {
                 ETHTSYN_SET_TXFLAG
               (&EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, ETHTSYN_BITPOS_PDELAYREQ, TRUE);
              }
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
              else
              {
                /* !LINKSTO EthTSyn.EB.TransparentClock_PdelayReqTransmission, 1 */
                ETHTSYN_SET_TXFLAG
                  (&EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, ETHTSYN_BITPOS_TX_SWT, TRUE);
                TS_AtomicSetBit_8
                (
                  &EthTSyn_SwitchPorts
                     [EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].Frame2Transmit,
                  ETHTSYN_BITPOS_PDELAYREQ
                );
              }
#endif
#if (ETHTSYN_MAX_SLAVE != 0U)
              if(TRUE == EthTSyn_IsSlave(EthTSynCtrlIdx)) /* Slave */
              {
                 /* !LINKSTO EthTSyn.ASR151.PRS_TS_00005, 1 */
                 EthTSyn_ResetSlavePdelayData(EthTSynCtrlIdx);
                 /* Load the configured timeout: Time Master and Time Slave shall observe the Pdelay timeout
                 as given by PdelayRespAndRespFollowUpTimeout , if a Pdelay_Req has been
                 transmitted (waiting for Pdelay_Resp). */
                 /* !LINKSTO EthTSyn.ASR151.PRS_TS_00164.PdelayReqTimeout, 1 */
                 EthTSyn_Slave[EthTSynCtrlIdx].PdelayFollowUpTimeout
                   = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayRespAndRespFollowUpTimeout;
               }
#endif/* (ETHTSYN_MAX_SLAVE != 0U) */
            }
          }

      }
      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00019, 1 */
      else
      {
        /* Set Pdelay send timeout initial value different to the sync send timeout to reduce
           traffic on bus. */
        EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextPdelayReqFrame_Timeout
          = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayReqTxInterval;
      }
    }
#if(ETHTSYN_MAX_SLAVE != 0U)
    if(TRUE == EthTSyn_IsSlave(EthTSynCtrlIdx))
    {
      EthTSyn_MainFunctionTimeSynSlave(EthTSynCtrlIdx);
    }
#endif/* (ETHTSYN_MAX_SLAVE != 0U) */
#if(ETHTSYN_SWITCH_USED == STD_ON)
    EthTSyn_Inc02_SwtSynStateHandling(EthTSynCtrlIdx);
#endif /* (ETHTSYN_SWITCH_USED == STD_ON) */
  }
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ProcessFrame2Transmit(void)
{

  uint8 EthTSynCtrlIdx;

  for(
       EthTSynCtrlIdx = 0U;
       EthTSynCtrlIdx < EthTSyn_RootCfgPtr->MaxEthTSynCtrlIdx;
       EthTSynCtrlIdx++
     )
  {
    uint8 TxMode;
    TS_AtomicAssign8(TxMode, EthTSyn_Ctrl[EthTSynCtrlIdx].TxMode);

    /* Send frame only if active link available. */
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00021, 1 */
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00022, 1 */
    if(ETHTSYN_TX_ON == TxMode)
    {
      if(EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit != 0U)
      {
        if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount == 0U)
        {
          /* if offset reached transmit frame and load the debounceoffset */
          /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00133, 1 */
          if(EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset == ETHTSYN_INV_DEBOUNCE_VALUE)
          {
            /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00131, 1 */
            /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00133, 1 */
            EthTSyn_TransmitFrameWithDebounce(EthTSynCtrlIdx);
            /* set the debounce time */
            /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00130, 1 */
            if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].GlobalTimeDebounceTime != 0U)
            {
              EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset =
                  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].GlobalTimeDebounceTime +
                  EthTSyn_GeneralCounter;
              if(EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset == ETHTSYN_INV_DEBOUNCE_VALUE)
              {
                /* !LINKSTO EthTSyn.EB.DebounceCalculation_MaxValue, 1 */
                /* If calculated DebounceTimeOffset = ETHTSYN_INV_DEBOUNCE_VALUE the frame transmission
                  shall be lost. Decreasing the DebounceTimeOffset with 1 shall transmit the
                  frame 1 EthTSyn_MainFunction() earlier, but the frame won't be lost. */
                EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset--;
              }
              /* needs to be set always when debounce is set to a new value != invalid */
              /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00132, 1 */
              (void)EthTSyn_NextMinDebounceOffset
                    (&GlobalMinDebounceTimeOffset,
                     EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset
                    );
            }

          }
        }
#if(ETHTSYN_SWITCH_USED == STD_ON)
        else
        {
          EthTSyn_IntPortIdxType Idx = 0U;
          /* if swt used */
          if(EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset ==
                                                        ETHTSYN_INV_DEBOUNCE_VALUE)
          {
            uint8 ClearSwtFlag = 0U;
            for(
                 Idx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart;
                 Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
                 EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
                 Idx++
               )
            {
              if(EthTSyn_SwitchPorts[Idx].Frame2Transmit != 0U)
              {
                /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00131, 1 */
                EthTSyn_Inc02_TransmitSwtFrameWithDebounce(EthTSynCtrlIdx,Idx);
                if(EthTSyn_SwitchPorts[Idx].Frame2Transmit != 0U)
                {
                  /* If still another frame must be transmitted don't clear the transmission flag */
                  ClearSwtFlag++;
                }
                /* Debounce value is not calculated for the HostPort since it would add a
                 * debounce counter between Sync frames on different ports. */
                if(Idx != EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt)
                {
                  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].GlobalTimeDebounceTime != 0U)
                  {
                    /* !LINKSTO EthTSyn.EB.TransparentClock_DebounceValueSet, 1 */
                    EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset =
                        EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].GlobalTimeDebounceTime +
                        EthTSyn_GeneralCounter;

                    if(EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset == ETHTSYN_INV_DEBOUNCE_VALUE)
                    {
                      /* !LINKSTO EthTSyn.EB.DebounceCalculation_MaxValue, 1 */
                      /* If calculated DebounceTimeOffset = ETHTSYN_INV_DEBOUNCE_VALUE the frame transmission
                        shall be lost. Decreasing the DebounceTimeOffset with 1 shall transmit the
                        frame 1 EthTSyn_MainFunction() earlier, but the frame won't be lost. */
                      EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset--;
                    }

                    /* needs to be set always when debounce is set to a new value != invalid */
                    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00132, 1 */
                    (void)EthTSyn_NextMinDebounceOffset
                          (&GlobalMinDebounceTimeOffset,
                           EthTSyn_Ctrl[EthTSynCtrlIdx].DebounceTimeOffset
                          );
                  }
                }
              }
            }
            if(ClearSwtFlag == 0U)
            {
              /* clear the flag */
              TS_AtomicClearBit_8
              (
                &EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit,
                ETHTSYN_BITPOS_TX_SWT
              );
            }
          }
        }
#endif
      }
    }
    if(EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit != 0U)
    {
      TS_AtomicAssign8(EthTSyn_Frame2Transmit, TRUE);
    }
  }
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_TransmitFrameWithDebounce
(
  uint8 EthTSynCtrlIdx
)
{
  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00186, 1 */
  if((EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit & ETHTSYN_DEBOUNCE_TX_SYNC_MASK) != 0U)
  {
    EthTSyn_TransmitSyncFrame(EthTSynCtrlIdx);

    /* clear the flag */
    TS_AtomicClearBit_8
    (
      &EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit,
      ETHTSYN_BITPOS_TX_SYNC
    );

  }
  else if((EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit & ETHTSYN_DEBOUNCE_TX_FUP_MASK) != 0U)
  {
    EthTSyn_TransmitSyncFUpFrame(EthTSynCtrlIdx);

    /* clear the flag */
    TS_AtomicClearBit_8
    (
      &EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit,
      ETHTSYN_BITPOS_TX_FUP
    );

  }
  else if((EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit & ETHTSYN_DEBOUNCE_PDELAYREQ_MASK) != 0U)
  {
    EthTSyn_TransmitPdelayReqFrame(EthTSynCtrlIdx);
    /* clear the flag */
    TS_AtomicClearBit_8
    (
      &EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit,
      ETHTSYN_BITPOS_PDELAYREQ
    );

  }
  else if((EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit & ETHTSYN_DEBOUNCE_PDELAYRESP_MASK) != 0U)
  {
    EthTSyn_TransmitPdelayRespFrame(EthTSynCtrlIdx);

    /* clear the flag */
    TS_AtomicClearBit_8
    (
      &EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit,
      ETHTSYN_BITPOS_PDELAYRESP
    );

  }
  /* CHECK: NOPARSE */
  else if((EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit & ETHTSYN_DEBOUNCE_PDELAYRESP_FUP_MASK) !=
                                                                                                0U)
  {
    EthTSyn_TransmitPdelayRespFUpFrame(EthTSynCtrlIdx);

    /* clear the flag */
    TS_AtomicClearBit_8
    (
      &EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit,
      ETHTSYN_BITPOS_PDELAYRESP_FUP
    );

  }
  else
  {
    /* nothing to do */
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif
  }
  /* CHECK: PARSE */
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_ProcessRxMsg
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  /* Deviation MISRAC2012-1 */
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PhysAddrPtr,
  uint16 LenByte
)
{
  /* Extract message type from PTP header. */
  const uint8 MsgType = DataPtr[ETHTSYN_PTP_HEADER_B0_LN_MSGTYPE] & ETHTSYN_8BIT_MASK_LN;

  TS_PARAM_UNUSED(PhysAddrPtr);

#if(ETHTSYN_HW_TIMESTAMP_SUPPORT != STD_ON)

  SchM_Enter_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_1();
  if((MsgType == ETHTSYN_PTP_HEADER_MSGTYPE_SYNC) ||
     (MsgType == ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_REQ) ||
     (MsgType == ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_RESP)
    )
  {
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00180, 1 */
    /* !LINKSTO EthTSyn.EB.GetVirtualProtection_PdelayReq, 1 */
    /* !LINKSTO EthTSyn.EB.GetVirtualProtection_PdelayResp, 1 */
    EthTSyn_Ctrl[EthTSynCtrlIdx].GetCurrentVirtualLocalTimeRetVal =
    StbM_GetCurrentVirtualLocalTime
    (
      EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId,
      &EthTSyn_Ctrl[EthTSynCtrlIdx].RxIngressVirtualTime
    );
  }

  SchM_Exit_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_1();

#endif

  /* !LINKSTO EthTSyn.ASR151.PRS_TS_00120, 1 */
  switch(MsgType)
  {
    case ETHTSYN_PTP_HEADER_MSGTYPE_SYNC:
    {
      /* !LINKSTO EthTSyn.EB.Sync_IncorrectLength, 1 */
      if(LenByte == ETHTSYN_SYNC_FRAME_SIZE)
      {
#if(ETHTSYN_MAX_SLAVE != 0U)
        if(TRUE == EthTSyn_IsSlave(EthTSynCtrlIdx)) /* Slave */
        {
#if((ETHTSYN_DEV_ERROR_DETECT == STD_ON) && (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON))
          Std_ReturnType TmpRet = E_NOT_OK;
          if(EthTSyn_Slave[EthTSynCtrlIdx].SaveSrcPortId == TRUE)
          {
            /* When the first sync frame is received, save the sourceportid, in order to be able to detect
               parallel masters. */
            TS_MemCpy(
               &EthTSyn_Slave[EthTSynCtrlIdx].Sync_Rx_SrcPortIdentity[0U],
               &DataPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0],
               ETHTSYN_SOURCEPORTIDENTITY_SIZE
             );
             EthTSyn_Slave[EthTSynCtrlIdx].SaveSrcPortId = FALSE;
          }

          /* Check source port identity */
          TmpRet = TS_MemCmp(
                    &EthTSyn_Slave[EthTSynCtrlIdx].Sync_Rx_SrcPortIdentity[0U],
                    &DataPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0],
                    ETHTSYN_SOURCEPORTIDENTITY_SIZE
                  );
          if(TmpRet == E_OK)
          {
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
            /* !LINKSTO EthTSyn.EB.Master.RxIndication, 1 */
            /* !LINKSTO EthTSyn.ASR151.PRS_TS_00120, 1 */
            EthTSyn_ProcessRxSyncFrame(EthTSynCtrlIdx, DataPtr);
#if((ETHTSYN_DEV_ERROR_DETECT == STD_ON) && (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON))
          }
          else
          {
            /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00146, 1 */
            ETHTSYN_DET_REPORT_ERROR
            (
              ETHTSYN_SID_RXINDICATION,
              ETHTSYN_E_TSCONFLICT
            );
          }
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
        }
        else
#endif /* ETHTSYN_MAX_SLAVE != 0U */
        {
#if(ETHTSYN_PROD_ERR_HANDLING_UNEXPECTED_SYNC == TS_PROD_ERR_REP_TO_DEM)
          /* !LINKSTO EthTSyn.EB.Diagnostic_UnexpectedSync_DEM, 1 */
          Dem_ReportErrorStatus
          (
            EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EventIdUnexpectedSync,
            DEM_EVENT_STATUS_FAILED
          );
#endif/* ETHTSYN_PROD_ERR_HANDLING_UNEXPECTED_SYNC == TS_PROD_ERR_REP_TO_DEM */

#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)

#if(ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
          /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00145, 1 */
          ETHTSYN_DET_REPORT_ERROR
          (
            ETHTSYN_SID_RXINDICATION,
            ETHTSYN_E_TMCONFLICT
          );
#endif

#if(ETHTSYN_PROD_ERR_HANDLING_UNEXPECTED_SYNC == TS_PROD_ERR_REP_TO_DET)
          /* !LINKSTO EthTSyn.EB.Diagnostic_UnexpectedSync_DET, 1 */
          ETHTSYN_DET_REPORT_ERROR
          (
            ETHTSYN_SID_RXINDICATION,
            ETHTSYN_E_TMCONFLICT_CONFIGURED
          );
#endif

#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
          }
      }
       break;
    }
    case ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_REQ:
    {
      /* !LINKSTO EthTSyn.EB.PdelayReq_IncorrectLength, 1 */
      if((ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_REQ_PAYLOADSIZE) <= LenByte)
      {
        /* !LINKSTO EthTSyn.ASR151.PRS_TS_00143, 1 */
        if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayRespEnable == TRUE)
        {
          EthTSyn_ProcessRxPdelayReqFrame(EthTSynCtrlIdx, DataPtr, LenByte);
        }
      }
      break;
    }
    case ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_RESP:
    {
      if((ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_RESP_PAYLOADSIZE) <= LenByte)
      {
#if (ETHTSYN_MAX_SLAVE != 0U)
        if(
            (TRUE == EthTSyn_IsSlave(EthTSynCtrlIdx)) &&
            (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayReqTxInterval != 0U)
          ) /* Slave */
          {
            /* !LINKSTO EthTSyn.EB.Master.RxIndication, 1 */
            /* !LINKSTO EthTSyn.ASR151.PRS_TS_00120, 1 */
            /* !LINKSTO EthTSyn.EB.Slave.LostSyncFUPFrame, 1 */
            EthTSyn_ProcessRxPdelayRespFrame(EthTSynCtrlIdx, DataPtr);
            /* Update PdelayRespAndRespFollowUpTimeout(waiting for Pdelay_Resp) */
            /* !LINKSTO EthTSyn.ASR151.PRS_TS_00164.PdelayRespTimeout, 1 */
            EthTSyn_Slave[EthTSynCtrlIdx].PdelayFollowUpTimeout
              = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayRespAndRespFollowUpTimeout;
          }
#endif /* ETHTSYN_MAX_SLAVE != 0U */
      }
      break;
    }
    case ETHTSYN_PTP_HEADER_MSGTYPE_FUP:
    {
      if((ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_SYNC_FUP_PAYLOADSIZE) <= LenByte)
      {
#if (ETHTSYN_MAX_SLAVE != 0U)
        if(TRUE == EthTSyn_IsSlave(EthTSynCtrlIdx)) /* Slave */
        {
          /* !LINKSTO EthTSyn.EB.Master.RxIndication, 1 */
          /* !LINKSTO EthTSyn.ASR151.PRS_TS_00120, 1 */
          /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00179, 1 */
          /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00052, 1 */
          /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00190, 1 */
          EthTSyn_ProcessRxSynFUpFrame(EthTSynCtrlIdx, DataPtr, LenByte);
        }
#endif /* ETHTSYN_MAX_SLAVE != 0U */
      }
      break;
    }
    case ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_FUP:
    {
      if((ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_RESP_FUP_PAYLOADSIZE) <= LenByte)
      {
#if (ETHTSYN_MAX_SLAVE != 0U)
        if(
            (TRUE == EthTSyn_IsSlave(EthTSynCtrlIdx)) &&
            (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayReqTxInterval != 0U)
          ) /* Slave */
        {
          /* !LINKSTO EthTSyn.EB.Master.RxIndication, 1 */
          EthTSyn_ProcessRxPdelayRespFUpFrame(EthTSynCtrlIdx, DataPtr);
        }
#endif /* ETHTSYN_MAX_SLAVE != 0U */
      }
      break;
    }
    default:
    {
      /* !LINKSTO EthTSyn.EB.UnexpectedFrames, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00005, 1 */
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00206, 1 */
      /* Unknown message type: Ignore */
      break;
    }
  }
}


STATIC FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_CalculateOriginTimeStamp
(
  uint8 EthTSynCtrlIdx,
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr
#else
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStampPtr
#endif
)
{
  StbM_VirtualLocalTimeType VirtualLocalTime;
  Std_ReturnType TmpRet;
  Eth_TimeStampType StbMTimeStamp2EthTimeStamp;
  Eth_TimeStampType EthTimeValueDiffT4Vlt_T1Vlt;

  DBG_ETHTSYN_CALCULATEORIGINTIMESTAMP_ENTRY(EthTSynCtrlIdx);

  EthTimeValueDiffT4Vlt_T1Vlt.nanoseconds = 0U;
  EthTimeValueDiffT4Vlt_T1Vlt.seconds = 0U;
  EthTimeValueDiffT4Vlt_T1Vlt.secondsHi = 0U;

  VirtualLocalTime.nanosecondsLo = 0U;
  VirtualLocalTime.nanosecondsHi = 0U;

  /* convert StbM_TimeStamp to Eth_TimeStamp */
  StbMTimeStamp2EthTimeStamp.nanoseconds = EthTSyn_Ctrl[EthTSynCtrlIdx].StbMTimeStamp.nanoseconds;
  StbMTimeStamp2EthTimeStamp.seconds = EthTSyn_Ctrl[EthTSynCtrlIdx].StbMTimeStamp.seconds;
  StbMTimeStamp2EthTimeStamp.secondsHi = EthTSyn_Ctrl[EthTSynCtrlIdx].StbMTimeStamp.secondsHi;

#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)

  TS_PARAM_UNUSED(EthTimeValueDiffT4Vlt_T1Vlt);

  TmpRet = EthTSyn_Int01_ConvertEthTimeToVirtualTime
  (
      TimeStampPtr,
      &VirtualLocalTime
  );

  if(E_OK == TmpRet)
  {
    if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].VltStbMIsEthFreerunningCounter == TRUE)
    {
      StbM_VirtualLocalTimeType VirtualLocalTimeDiff_t2_t0;
      VirtualLocalTimeDiff_t2_t0.nanosecondsLo = 0U;
      VirtualLocalTimeDiff_t2_t0.nanosecondsHi = 0U;

      TmpRet = EthTSyn_Int01_SubVirtualLocalTimes
                  (
                     &VirtualLocalTime,
                     &EthTSyn_Ctrl[EthTSynCtrlIdx].StbMVirtualLocalTime,
                     &VirtualLocalTimeDiff_t2_t0
                  );
      if(TmpRet == E_OK)
      {
        Eth_TimeStampType EthTimeStampDiffT2_T0;
        EthTimeStampDiffT2_T0.nanoseconds = 0U;
        EthTimeStampDiffT2_T0.seconds = 0U;
        EthTimeStampDiffT2_T0.secondsHi = 0U;

        EthTSyn_Int01_ConvertVirtualTimeToEthTime(&VirtualLocalTimeDiff_t2_t0,
                                                                           &EthTimeStampDiffT2_T0);

        /* Torigin = T0 + (T2vlt-T0vlt) */
        /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00188, 1 */
        EthTSyn_AddTimeStamps(&StbMTimeStamp2EthTimeStamp, &EthTimeStampDiffT2_T0);
      }
    }
    else
    {
      Eth_TimeStampQualType TimeQuality;
      Eth_TimeStampType EthTimeStamp;
      StbM_VirtualLocalTimeType VirtualLocalTimeT3;
      StbM_VirtualLocalTimeType VirtualLocalTimeT4;
      StbM_VirtualLocalTimeType VirtualLocalTimeDiff_t4_t0;
      StbM_VirtualLocalTimeType VirtualLocalTimeDiff_t3_t2;
      StbM_VirtualLocalTimeType VirtualLocalTimeDiff;

      EthTimeStamp.nanoseconds = 0U;
      EthTimeStamp.seconds = 0U;
      EthTimeStamp.secondsHi = 0U;

      VirtualLocalTimeDiff.nanosecondsLo = 0U;
      VirtualLocalTimeDiff.nanosecondsHi = 0U;

      VirtualLocalTimeDiff_t4_t0.nanosecondsLo = 0U;
      VirtualLocalTimeDiff_t4_t0.nanosecondsHi = 0U;
      
      VirtualLocalTimeDiff_t3_t2.nanosecondsLo = 0U;
      VirtualLocalTimeDiff_t3_t2.nanosecondsHi = 0U;

      SchM_Enter_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_1();
      /* !LINKSTO EthTSyn.EB.InvTimeQuality.Master.GetCurrentTime, 1 */
      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00017, 1 */
      TmpRet = EthIf_GetCurrentTime(
                                     EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx,
                                     &TimeQuality,
                                     &EthTimeStamp
                                   );
      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00017, 1 */
      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00180, 1 */
      TmpRet |= StbM_GetCurrentVirtualLocalTime
      (
        EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId,
        &VirtualLocalTimeT4
      );
      SchM_Exit_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_1();
      if(TimeQuality != ETH_VALID)
      {
        TmpRet = E_NOT_OK;
      }

      if(E_OK == TmpRet)
      {
        /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00017, 1 */
        TmpRet = EthTSyn_Int01_ConvertEthTimeToVirtualTime
        (
            &EthTimeStamp,
            &VirtualLocalTimeT3
        );
      }

      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00017, 1 */
      if(TmpRet == E_OK)
      {
        TmpRet = EthTSyn_Int01_SubVirtualLocalTimes
                    (
                       &VirtualLocalTimeT4,
                       &EthTSyn_Ctrl[EthTSynCtrlIdx].StbMVirtualLocalTime,
                       &VirtualLocalTimeDiff_t4_t0
                    );
        TmpRet |= EthTSyn_Int01_SubVirtualLocalTimes
                    (
                       &VirtualLocalTimeT3,
                       &VirtualLocalTime,
                       &VirtualLocalTimeDiff_t3_t2
                    );
        TmpRet |= EthTSyn_Int01_SubVirtualLocalTimes
                     (
                       &VirtualLocalTimeDiff_t4_t0,
                       &VirtualLocalTimeDiff_t3_t2,
                       &VirtualLocalTimeDiff
                     );
      }
      if(TmpRet == E_OK)
      {
        Eth_TimeStampType EthTimeStampDiff;

        EthTimeStampDiff.nanoseconds = 0U;
        EthTimeStampDiff.seconds = 0U;
        EthTimeStampDiff.secondsHi = 0U;

        EthTSyn_Int01_ConvertVirtualTimeToEthTime(&VirtualLocalTimeDiff, &EthTimeStampDiff);

        /* Torigin= T0 + (T4vlt-T0vlt) -(T3vlt-T2vlt); */
        EthTSyn_AddTimeStamps(&StbMTimeStamp2EthTimeStamp, &EthTimeStampDiff);

      }
    }
  }
#else

  TmpRet = EthTSyn_Int01_SubVirtualLocalTimes
      (
         TimeStampPtr,
         &EthTSyn_Ctrl[EthTSynCtrlIdx].StbMVirtualLocalTime,
         &VirtualLocalTime
      );

  EthTSyn_Int01_ConvertVirtualTimeToEthTime(&VirtualLocalTime, &EthTimeValueDiffT4Vlt_T1Vlt);
  /* T0 + t0(virtuallocaltime) */
  EthTSyn_AddTimeStamps(&StbMTimeStamp2EthTimeStamp, &EthTimeValueDiffT4Vlt_T1Vlt);

#endif

  if(TmpRet == E_OK)
  {
    EthTSyn_Ctrl[EthTSynCtrlIdx].OriginTimeStamp = StbMTimeStamp2EthTimeStamp;
  }

  /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if((ETHTSYN_TIME_RECORDING == STD_ON) && (ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE))
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeValidationEnabled == TRUE)
  {
    /* Save preciseOriginTimeStamp to be reported to StbM for time recording */
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.preciseOriginTimestamp.nanoseconds =
                                                            StbMTimeStamp2EthTimeStamp.nanoseconds;
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.preciseOriginTimestamp.seconds =
                                                            StbMTimeStamp2EthTimeStamp.seconds;
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.correctionField  = 0U;
  }
#endif

  DBG_ETHTSYN_CALCULATEORIGINTIMESTAMP_EXIT(TmpRet, EthTSynCtrlIdx);

  return TmpRet;
}
#if((ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED != TS_PROD_ERR_DISABLE) && (ETHTSYN_MAX_SLAVE != 0U))
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_SyncFupDropped
(
  uint8 EthTSynCtrlIdx
)
{
  /* Reload the value of accepted waiting timeout */
  EthTSyn_Slave[EthTSynCtrlIdx].SyncFupPairTimeout =
    EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].ReceiveSyncFupPairTimeout;

   /* Clear the flag, a sync/fup was dropped */
  TS_AtomicAssign8(EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived, FALSE);
#if(ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED == TS_PROD_ERR_REP_TO_DEM)
       /* !LINKSTO EthTSyn.EB.Diagnostic_SyncFailed_DEM, 1 */
       /* !LINKSTO EthTSyn.EB.TimeAwareBridge_DEM_FailedSync, 1 */
       /* !LINKSTO EthTSyn.EB.TimeAwareBridge_DEM_FailedSync_Ingress, 1 */
        Dem_ReportErrorStatus
        (
           EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EventIdSyncFailed,
           DEM_EVENT_STATUS_PREFAILED
         );
#elif((ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED == TS_PROD_ERR_REP_TO_DET) && \
      (ETHTSYN_DEV_ERROR_DETECT == STD_ON) \
     )
        EthTSyn_Slave[EthTSynCtrlIdx].SyncFupDroppedPairsCt++;
        if(EthTSyn_Slave[EthTSynCtrlIdx].SyncFupDroppedPairsCt >=
                                                                 ETHTSYN_MAX_DROPPED_SYNCFUP_PAIRS)
        {
          EthTSyn_Slave[EthTSynCtrlIdx].SyncFupDroppedPairsCt = ETHTSYN_MAX_DROPPED_SYNCFUP_PAIRS;
          /* !LINKSTO EthTSyn.EB.Diagnostic_SyncFailed_DET, 1 */
          ETHTSYN_DET_REPORT_ERROR
          (
              ETHTSYN_SID_RXINDICATION,
              ETHTSYN_E_DET_SYNC_FAILED
          );
        }
#endif
}
#endif

#if((ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED != TS_PROD_ERR_DISABLE) && (ETHTSYN_MAX_SLAVE != 0U))
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_PDelayFrameDropped
(
  uint8 EthTSynCtrlIdx
)
{

  /* Reload the value of accepted waiting timeout */
  EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayRespPairTimeout =
    EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayRespFupPairsPeriod;

#if(ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED == TS_PROD_ERR_REP_TO_DEM)
        /* !LINKSTO EthTSyn.EB.Diagnostic_PdelayFailed_DEM, 1 */
        Dem_ReportErrorStatus
        (
           EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EventIdPdelayFailed,
           DEM_EVENT_STATUS_PREFAILED
         );
#elif((ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED == TS_PROD_ERR_REP_TO_DET) && \
      (ETHTSYN_DEV_ERROR_DETECT == STD_ON))
        EthTSyn_Slave[EthTSynCtrlIdx].PdelayRespFupDroppedPairsCt++;
        if(EthTSyn_Slave[EthTSynCtrlIdx].PdelayRespFupDroppedPairsCt >=
                                                     ETHTSYN_MAX_DROPPED_PDELAYRESPPDELAYFUP_PAIRS)
        {
          EthTSyn_Slave[EthTSynCtrlIdx].PdelayRespFupDroppedPairsCt =
                                                     ETHTSYN_MAX_DROPPED_PDELAYRESPPDELAYFUP_PAIRS;
          /* !LINKSTO EthTSyn.EB.Diagnostic_PdelayFailed_DET, 1 */
          ETHTSYN_DET_REPORT_ERROR
          (
              ETHTSYN_SID_RXINDICATION,
              ETHTSYN_E_DET_PDELAY_FAILED
          );
        }
#endif
}
#endif
#if(ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT == STD_ON)
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_PdelayDevAuth
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint16 Tlv_Length;
  Tlv_Length = ETHTSYN_GET16(DataPtr, 2U);
  /* !LINKSTO EthTSyn.EB.DevAuth_PdelayReqInvalidTLV, 1 */
  if(
      /* TLV type */
      (DataPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B0_TYPE] == ETHTSYN_AUTH_CHALLENGE_TLV_TYPE_0) &&
      (DataPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B1_TYPE] == ETHTSYN_AUTH_CHALLENGE_TLV_TYPE_1) &&
      /* length field */
      (Tlv_Length == ETHTSYN_AUTH_CHALLENGE_TLV_SIZE)
    )
  {
    /* !LINKSTO EthTSyn.EB.DevAuth_PdelayReqValidAuthTLV, 1 */
    /* Save RequestNonce from received PdelayReq frame */
    EthTSyn_Ctrl[EthTSynCtrlIdx].RequestNonce =
                       ((uint32)DataPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B6_REQNONCE] << 24U)
                        + (uint32)((uint32)DataPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B7_REQNONCE] << 16U)
                        + (uint32)((uint32)DataPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B8_REQNONCE] << 8U)
                        + DataPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B9_REQNONCE];
    /* !LINKSTO EthTSyn.EB.DevAuth_PdelayReqIndication_NOK, 1 */
    RetVal = DevAuth_PDelayReqIndication
    (
        EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx,
        &EthTSyn_Ctrl[EthTSynCtrlIdx].IngressTimeStamp,
        EthTSyn_Ctrl[EthTSynCtrlIdx].RequestNonce,
        &EthTSyn_Ctrl[EthTSynCtrlIdx].ResponseNonce
     );

    if(RetVal == E_OK)
    {
      TS_AtomicAssign8(EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqReceivedWithAuthChallTlv, TRUE);
    }

  }
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_FillAuthChallengeTLV
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr
)
{
  /* tlvType = AUTHENTICATION_CHALLENGE */
  BufPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B0_TYPE] = ETHTSYN_AUTH_CHALLENGE_TLV_TYPE_0;
  BufPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B1_TYPE] = ETHTSYN_AUTH_CHALLENGE_TLV_TYPE_1;

  /* lengthField */
  BufPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B2_LENGTH] = ETHTSYN_AUTH_CHALLENGE_TLV_LENGTH_0;
  BufPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B3_LENGTH] = ETHTSYN_AUTH_CHALLENGE_TLV_LENGTH_1;

  /* challengeType */
  BufPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B4_CHLTYPE] = ETHTSYN_AUTH_CHALLENGE_RESPONSE_TYPE;
  /* reserved */
  BufPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B5_RESERVED] = ETHTSYN_AUTH_CHALLENGE_TLV_RESERVED;

  /* RequestNonce */
  BufPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B6_REQNONCE] =
      (uint8)((uint32)(EthTSyn_Ctrl[EthTSynCtrlIdx].RequestNonce & ETHTSYN_32BIT_MASK3_MSB) >> 24U);
  BufPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B7_REQNONCE] =
      (uint8)((uint32)(EthTSyn_Ctrl[EthTSynCtrlIdx].RequestNonce & ETHTSYN_32BIT_MASK2) >> 16U);
  BufPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B8_REQNONCE] =
      (uint8)((uint32)(EthTSyn_Ctrl[EthTSynCtrlIdx].RequestNonce & ETHTSYN_32BIT_MASK1) >> 8U);
  BufPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B9_REQNONCE] =
      (uint8)(EthTSyn_Ctrl[EthTSynCtrlIdx].RequestNonce & ETHTSYN_32BIT_MASK0_LSB);

  /* ResponseNonce */
  BufPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B10_RESPNONCE] =
      (uint8)((uint32)(EthTSyn_Ctrl[EthTSynCtrlIdx].ResponseNonce & ETHTSYN_32BIT_MASK3_MSB) >> 24U);
  BufPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B11_RESPNONCE] =
      (uint8)((uint32)(EthTSyn_Ctrl[EthTSynCtrlIdx].ResponseNonce & ETHTSYN_32BIT_MASK2) >> 16U);
  BufPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B12_RESPNONCE] =
      (uint8)((uint32)(EthTSyn_Ctrl[EthTSynCtrlIdx].ResponseNonce & ETHTSYN_32BIT_MASK1) >> 8U);
  BufPtr[ETHTSYN_AUTH_CHALLENGE_TLV_B13_RESPNONCE] =
      (uint8)(EthTSyn_Ctrl[EthTSynCtrlIdx].ResponseNonce & ETHTSYN_32BIT_MASK0_LSB);

}
#endif /* ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT == STD_ON */
#define ETHTSYN_STOP_SEC_CODE
#include <EthTSyn_MemMap.h>

/*==================[version check]=========================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef ETHTSYN_VENDOR_ID /* configuration check */
#error ETHTSYN_VENDOR_ID must be defined
#endif

#if (ETHTSYN_VENDOR_ID != 1U) /* vendor check */
#error ETHTSYN_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef ETHTSYN_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error ETHTSYN_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (ETHTSYN_AR_RELEASE_MAJOR_VERSION != 4U)
#error ETHTSYN_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef ETHTSYN_AR_RELEASE_MINOR_VERSION /* configuration check */
#error ETHTSYN_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (ETHTSYN_AR_RELEASE_MINOR_VERSION != 4U )
#error ETHTSYN_AR_RELEASE_MINOR_VERSION wrong (!= 4U)
#endif

#ifndef ETHTSYN_AR_RELEASE_REVISION_VERSION /* configuration check */
#error ETHTSYN_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (ETHTSYN_AR_RELEASE_REVISION_VERSION != 0U )
#error ETHTSYN_AR_RELEASE_REVISION_VERSION wrong (!= 0U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef ETHTSYN_SW_MAJOR_VERSION /* configuration check */
#error ETHTSYN_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (ETHTSYN_SW_MAJOR_VERSION != 2U)
#error ETHTSYN_SW_MAJOR_VERSION wrong (!= 2U)
#endif

#ifndef ETHTSYN_SW_MINOR_VERSION /* configuration check */
#error ETHTSYN_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (ETHTSYN_SW_MINOR_VERSION < 2U)
#error ETHTSYN_SW_MINOR_VERSION wrong (< 2U)
#endif

#ifndef ETHTSYN_SW_PATCH_VERSION /* configuration check */
#error ETHTSYN_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (ETHTSYN_SW_PATCH_VERSION < 4U)
#error ETHTSYN_SW_PATCH_VERSION wrong (< 4U)
#endif

/*==================[end of file]===========================================*/
