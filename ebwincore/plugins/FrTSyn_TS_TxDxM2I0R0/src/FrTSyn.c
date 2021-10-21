/**
 * \file
 *
 * \brief AUTOSAR FrTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTSyn.
 *
 * \version 2.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/*
 *  MISRA-C:2012 Deviations:
 *
 *  MISRAC2012-1) Deviated Rule: 13.5 (Required)
 *    The right hand operand of a logical && or || operator shall not contain persistent
 *    side effects.
 *
 *    Reason:
 *    - The order of evaluation is considered and the side effect does not need to
 *      occur unless the preceding conditions are evaluated positively.
 *
 *  MISRAC2012-2) Deviated Rule: 8.9 (Advisory)
 *    An object should be defined at block scope if its identifier only
 *    appears in a single function.
 *
 *    Reason:
 *    - The AUTOSAR memory mapping requires that functions are mapped in
 *      SEC_CODE memory sections. Objects at block scope require a different
 *      memory mapping, e.g. to a SEC_VAR section, which leads to nested memory
 *      sections, which is not supported by some compilers.
 *
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
#include <FrTSyn.h> /* Module public API */
#include <FrTSyn_Trace.h> /* Debug and trace */

#include <FrTSyn_Version.h> /* Module version declarations */
#include <FrIf.h> /* Module public API */
#include <StbM.h> /* Module public API */
#include <BswM.h> /* Module public API */
#if(FRTSYN_CRC_CHECK_USED == STD_ON)
#include <Crc.h> /* Module public API */
#endif
#include <TSMem.h> /* TS standard memory function files */

#include <SchM_FrTSyn.h> /* SchM interface header */

#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* DET support */
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */

#if(FRTSYN_TIME_VALIDATION_ENABLED == STD_ON)
#include <StbM_FrTSyn.h> /* Time Validation support */
#endif /* FRTSYN_TIME_VALIDATION_ENABLED == STD_ON */

/*==================[version check]=========================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef FRTSYN_VENDOR_ID /* configuration check */
#error FRTSYN_VENDOR_ID must be defined
#endif

#if (FRTSYN_VENDOR_ID != 1U) /* vendor check */
#error FRTSYN_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef FRTSYN_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error FRTSYN_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (FRTSYN_AR_RELEASE_MAJOR_VERSION != 4U)
#error FRTSYN_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef FRTSYN_AR_RELEASE_MINOR_VERSION /* configuration check */
#error FRTSYN_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (FRTSYN_AR_RELEASE_MINOR_VERSION != 4U )
#error FRTSYN_AR_RELEASE_MINOR_VERSION wrong (!= 4U)
#endif

#ifndef FRTSYN_AR_RELEASE_REVISION_VERSION /* configuration check */
#error FRTSYN_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (FRTSYN_AR_RELEASE_REVISION_VERSION != 0U )
#error FRTSYN_AR_RELEASE_REVISION_VERSION wrong (!= 0U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef FRTSYN_SW_MAJOR_VERSION /* configuration check */
#error FRTSYN_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (FRTSYN_SW_MAJOR_VERSION != 2U)
#error FRTSYN_SW_MAJOR_VERSION wrong (!= 2U)
#endif

#ifndef FRTSYN_SW_MINOR_VERSION /* configuration check */
#error FRTSYN_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (FRTSYN_SW_MINOR_VERSION < 0U)
#error FRTSYN_SW_MINOR_VERSION wrong (< 0U)
#endif

#ifndef FRTSYN_SW_PATCH_VERSION /* configuration check */
#error FRTSYN_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (FRTSYN_SW_PATCH_VERSION < 9U)
#error FRTSYN_SW_PATCH_VERSION wrong (< 9U)
#endif

/*==================[macros]================================================*/

#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
#if (defined FRTSYN_DET_REPORT_ERROR)
#error FRTSYN_DET_REPORT_ERROR already defined!
#endif /* #if (defined FRTSYN_DET_REPORT_ERROR)*/
#define FRTSYN_DET_REPORT_ERROR(ApiId, ErrorId) \
  (void) Det_ReportError(FRTSYN_MODULE_ID, FRTSYN_INSTANCE_ID, (ApiId), (ErrorId))
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */

#if (defined FRTSYN_MESSAGE_PAYLOAD)
#error FRTSYN_MESSAGE_PAYLOAD already defined!
#endif /* #if (defined FRTSYN_MESSAGE_PAYLOAD)*/
#define FRTSYN_MESSAGE_PAYLOAD 16U

#if (defined FRTSYN_NANOSECONDS_LENGTH_IN_BYTES)
#error FRTSYN_NANOSECONDS_LENGTH_IN_BYTES already defined!
#endif /* #if (defined FRTSYN_NANOSECONDS_LENGTH_IN_BYTES)*/
#define FRTSYN_NANOSECONDS_LENGTH_IN_BYTES 4U

#if (defined FRTSYN_SECONDS_LENGTH_IN_BYTES)
#error FRTSYN_SECONDS_LENGTH_IN_BYTES already defined!
#endif /* #if (defined FRTSYN_SECONDS_LENGTH_IN_BYTES)*/
#define FRTSYN_SECONDS_LENGTH_IN_BYTES 4U

#if (defined FRTSYN_SECONDSHI_LENGTH_IN_BYTES)
#error FRTSYN_SECONDSHI_LENGTH_IN_BYTES already defined!
#endif /* #if (defined FRTSYN_SECONDSHI_LENGTH_IN_BYTES)*/
#define FRTSYN_SECONDSHI_LENGTH_IN_BYTES 2U

#if (defined FRTSYN_BYTES_NEEDED_FROM_MSG_LENGTH)
#error FRTSYN_BYTES_NEEDED_FROM_MSG_LENGTH already defined!
#endif /* #if (defined FRTSYN_BYTES_NEEDED_FROM_MSG_LENGTH)*/
#define FRTSYN_BYTES_NEEDED_FROM_MSG_LENGTH 14U

#if (defined FRTSYN_USER_DATA_PAYLOAD)
#error FRTSYN_USER_DATA_PAYLOAD already defined!
#endif /* #if (defined FRTSYN_USER_DATA_PAYLOAD)*/
#define FRTSYN_USER_DATA_PAYLOAD 3U

#if (defined FRTSYN_MESSAGE_TYPE_BYTE)
#error FRTSYN_MESSAGE_TYPE_BYTE already defined!
#endif /* #if (defined FRTSYN_MESSAGE_TYPE_BYTE)*/
#define FRTSYN_MESSAGE_TYPE_BYTE 0

#if(FRTSYN_CRC_CHECK_USED == STD_ON)

#if (defined FRTSYN_CRC_BYTE)
#error FRTSYN_CRC_BYTE already defined!
#endif /* #if (defined FRTSYN_CRC_BYTE)*/
#define FRTSYN_CRC_BYTE 1

#if (defined FRTSYN_CRC_DATA_LENGTH)
#error FRTSYN_CRC_DATA_LENGTH already defined!
#endif /* #if (defined FRTSYN_CRC_DATA_LENGTH)*/
#define FRTSYN_CRC_DATA_LENGTH 15U

/** \brief Macro used to retrieve FrTSynGlobalTime DataIDListValues */
#if (defined FRTSYN_GET_DIDLISTVAL)
#error FRTSYN_GET_DIDLISTVAL already defined!
#endif /* #if (defined FRTSYN_GET_DIDLISTVAL)*/
#define FRTSYN_GET_DIDLISTVAL(idx, seqCounter) FrTSyn_GlobalTimeDataIDListElements[((idx) + (seqCounter))]

#endif /* FRTSYN_CRC_CHECK_USED == STD_ON */

#if(FRTSYN_IS_MASTER_FUNC_USED == STD_ON)

#if (defined FRTSYN_TXTYPE_PERIODIC)
#error FRTSYN_TXTYPE_PERIODIC already defined!
#endif /* #if (defined FRTSYN_TXTYPE_PERIODIC) */
#define FRTSYN_TXTYPE_PERIODIC 0U

#if (defined FRTSYN_TXTYPE_IMMEDIATE)
#error FRTSYN_TXTYPE_IMMEDIATE already defined!
#endif /* #if (defined FRTSYN_TXTYPE_IMMEDIATE) */
#define FRTSYN_TXTYPE_IMMEDIATE 1U

#if (defined FRTSYN_TXTYPE_BOTH)
#error FRTSYN_TXTYPE_BOTH already defined!
#endif /* #if (defined FRTSYN_TXTYPE_BOTH) */
#define FRTSYN_TXTYPE_BOTH 2U

/** \brief Macro used to evaluate TX conditions */
#if (defined FRTSYN_TX_COND)
#error FRTSYN_TX_COND already defined!
#endif /* #if (defined FRTSYN_TX_COND)*/
#define FRTSYN_TX_COND(pMasterData, pMasterCfg) FrTSyn_TxCondFuncs[(*(pMasterCfg)).txCondFuncIdx]((pMasterData), (pMasterCfg))

#if (defined FRTSYN_TIMEBASE_SET_MASK)
#error FRTSYN_TIMEBASE_SET_MASK already defined !
#endif /* #if (defined FRTSYN_TIMEBASE_SET_MASK) */
#define FRTSYN_TIMEBASE_SET_MASK  1U

#if (defined FRTSYN_TX_IMMEDIATE_MASK)
#error FRTSYN_TX_IMMEDIATE_MASK already defined !
#endif /* #if (defined FRTSYN_TX_IMMEDIATE_MASK) */
#define FRTSYN_TX_IMMEDIATE_MASK  2U

/** \brief Macro checks the transmission properties of the master
 *
 *  \param[in] pMasterData  Pointer to the master runtime data
 *  \param[in] property     Mask to verify with the txProperties
 *                          of the master's runtime data
 * \return TRUE             Corresponding bit is set
 *         FALSE            Corresponding bit is not set
 */
#if (defined FRTSYN_TX_PROPERTY_CHK)
#error FRTSYN_TX_PROPERTY_CHK already defined !
#endif /* #if (defined FRTSYN_TX_PROPERTY_CHK) */
#define FRTSYN_TX_PROPERTY_CHK(pMasterData, property) ((property) == ((*(pMasterData)).txProperties & (property)))

/** \brief Macro sets a transmission property of the master
 *
 *  \param[in] pMasterData  Pointer to the master runtime data
 *  \param[in] property     Mask to use to set the corresponding bit
 *                          in the txProperties member of the master's
 *                          runtime data structure
 */
#if (defined FRTSYN_TX_PROPERTY_SET)
#error FRTSYN_TX_PROPERTY_SET already defined !
#endif /* #if (defined FRTSYN_TX_PROPERTY_SET) */
#define FRTSYN_TX_PROPERTY_SET(pMasterData, property) ((*(pMasterData)).txProperties |= (property))

/** \brief Macro clears a transmission property of the master
 *
 *  \param[in] pMasterData  Pointer to the master runtime data
 *  \param[in] property     Mask to use to clear the corresponding bit
 *                          in the txProperties member of the master's
 *                          runtime data structure
 */
#if (defined FRTSYN_TX_PROPERTY_CLR)
#error FRTSYN_TX_PROPERTY_CLR already defined !
#endif /* #if (defined FRTSYN_TX_PROPERTY_CLR) */
#define FRTSYN_TX_PROPERTY_CLR(pMasterData, property) ((*(pMasterData)).txProperties &= ~(property))

#if (defined FRTSYN_IS_IMMEDIATE_TX)
#error FRTSYN_IS_IMMEDIATE_TX already defined !
#endif /* #if (defined FRTSYN_IS_IMMEDIATE_TX) */
#define FRTSYN_IS_IMMEDIATE_TX(pMasterData) (FRTSYN_TX_PROPERTY_CHK(pMasterData, FRTSYN_TX_IMMEDIATE_MASK))

#if (defined FRTSYN_SET_IMMEDIATE_TX)
#error FRTSYN_SET_IMMEDIATE_TX already defined !
#endif /* #if (defined FRTSYN_SET_IMMEDIATE_TX) */
#define FRTSYN_SET_IMMEDIATE_TX(pMasterData) (FRTSYN_TX_PROPERTY_SET(pMasterData, FRTSYN_TX_IMMEDIATE_MASK))

#if (defined FRTSYN_CLEAR_ONGOING_TXTYPE)
#error FRTSYN_CLEAR_ONGOING_TXTYPE already defined !
#endif /* #if (defined FRTSYN_CLEAR_ONGOING_TXTYPE) */
#define FRTSYN_CLEAR_ONGOING_TXTYPE(pMasterData) (FRTSYN_TX_PROPERTY_CLR(pMasterData, FRTSYN_TX_IMMEDIATE_MASK))

#if (defined FRTSYN_IS_TIMEBASE_SET)
#error FRTSYN_IS_TIMEBASE_SET already defined !
#endif
#define FRTSYN_IS_TIMEBASE_SET(pMasterData) (FRTSYN_TX_PROPERTY_CHK(pMasterData, FRTSYN_TIMEBASE_SET_MASK))

#if (defined FRTSYN_STORE_TIMEBASE_SET)
#error FRTSYN_STORE_TIMEBASE_SET already defined !
#endif /* #if (defined FRTSYN_STORE_TIMEBASE_SET) */
#define FRTSYN_STORE_TIMEBASE_SET(pMasterData) (FRTSYN_TX_PROPERTY_SET(pMasterData, FRTSYN_TIMEBASE_SET_MASK))

#endif /* FRTSYN_IS_MASTER_FUNC_USED == STD_ON */

#if (FRTSYN_TIME_VALIDATION_ENABLED == STD_ON)

#if (defined FRTSYN_TIME_VALIDATION_ENABLED_ON_DOMAIN)
#error FRTSYN_TIME_VALIDATION_ENABLED_ON_DOMAIN is already defined.
#endif /* #if (defined FRTSYN_TIME_VALIDATION_ENABLED_ON_DOMAIN) */
/** \brief Macro checks if time validation is enabled on a time domain
 *
 *  \param[in] pCfg         Pointer to the time domain cfg data
 */
#define FRTSYN_TIME_VALIDATION_ENABLED_ON_DOMAIN(pCfg) ((pCfg)->timeValidation == FRTSYN_TIME_VALIDATION_IS_ENABLED)

#if (defined FRTSYN_IS_FRIF_IMMEDIATE)
#error FRTSYN_IS_FRIF_IMMEDIATE is already defined.
#endif /* #if (defined FRTSYN_IS_FRIF_IMMEDIATE) */
/** \brief Macro checks if FrIfImmediate is set to 'true' in FrIf
 *         for our Pdu
 *
 *  \param[in] pCfg         Pointer to the time domain cfg data
 */
#define FRTSYN_IS_FRIF_IMMEDIATE(pCfg) ((pCfg)->transmissionMode == FRTSYN_FRIF_TX_MODE_IMMEDIATE)

#if (defined FRTSYN_ASSIGN_TIME_DATA)
#error FRTSYN_ASSIGN_TIME_DATA is already defined.
#endif
/** \brief Copies time validation data of the StbM_FrTimeMasterMeasurementType
 **
 ** This macro copies the time members (sequenceCounter, referenceTimestamp and preciseOriginTimestamp)
 ** of a structure of type StbM_FrTimeMasterMeasurementType.
 **
 ** \param[in] dst - Destination struct for assignment
 ** \param[in] src - Source struct for assignment
 */
#define FRTSYN_ASSIGN_TIME_DATA(dst,src)                                                          \
        do                                                                                        \
        {                                                                                         \
          (dst).sequenceCounter = (src).sequenceCounter;                                          \
          (dst).referenceTimestamp.nanosecondsLo = (src).referenceTimestamp.nanosecondsLo;        \
          (dst).referenceTimestamp.nanosecondsHi = (src).referenceTimestamp.nanosecondsHi;        \
          (dst).preciseOriginTimestamp.seconds = (src).preciseOriginTimestamp.seconds;            \
          (dst).preciseOriginTimestamp.nanoseconds = (src).preciseOriginTimestamp.nanoseconds;    \
          (dst).segmentId = (src).segmentId;    \
        } while (0)

#endif /* FRTSYN_TIME_VALIDATION_ENABLED == STD_ON */

#if (defined FRTSYN_TIME_DOMAIN_SEQ_COUNTER_BYTE)
#error FRTSYN_TIME_DOMAIN_SEQ_COUNTER_BYTE already defined!
#endif /* #if (defined FRTSYN_TIME_DOMAIN_SEQ_COUNTER_BYTE)*/
#define FRTSYN_TIME_DOMAIN_SEQ_COUNTER_BYTE 2

#if (defined FRTSYN_FCNT_AND_SGW_BYTE)
#error FRTSYN_FCNT_AND_SGW_BYTE already defined!
#endif /* #if (defined FRTSYN_FCNT_AND_SGW_BYTE)*/
#define FRTSYN_FCNT_AND_SGW_BYTE 3

#if (defined FRTSYN_DATA_ID_LIST_BYTE)
#error FRTSYN_DATA_ID_LIST_BYTE already defined!
#endif /* #if (defined FRTSYN_DATA_ID_LIST_BYTE)*/
#define FRTSYN_DATA_ID_LIST_BYTE 14U

#if (defined FRTSYN_SEQ_COUNTER_NOCHECK)
#error FRTSYN_SEQ_COUNTER_NOCHECK already defined!
#endif /* #if (defined FRTSYN_SEQ_COUNTER_NOCHECK)*/
#define FRTSYN_SEQ_COUNTER_NOCHECK (FRTSYN_MAX_SEQ_COUNTER + 1U)

#if (defined FRTSYN_SEQ_COUNTER_MASK)
#error FRTSYN_SEQ_COUNTER_MASK already defined!
#endif /* #if (defined FRTSYN_SEQ_COUNTER_MASK)*/
#define FRTSYN_SEQ_COUNTER_MASK 0x0FU

#if (defined FRTSYN_TIME_DOMAIN_SHIFT)
#error FRTSYN_TIME_DOMAIN_SHIFT already defined!
#endif /* #if (defined FRTSYN_TIME_DOMAIN_SHIFT)*/
#define FRTSYN_TIME_DOMAIN_SHIFT 4U

#if (defined FRTSYN_CYCLE_COUNTER_MASK)
#error FRTSYN_CYCLE_COUNTER_MASK already defined!
#endif /* #if (defined FRTSYN_CYCLE_COUNTER_MASK)*/
#define FRTSYN_CYCLE_COUNTER_MASK 0xFCU

#if (defined FRTSYN_CYCLE_COUNTER_SHIFT)
#error FRTSYN_CYCLE_COUNTER_SHIFT already defined!
#endif /* #if (defined FRTSYN_CYCLE_COUNTER_SHIFT)*/
#define FRTSYN_CYCLE_COUNTER_SHIFT 2

#if (defined FRTSYN_FR_CYCLE_COUNTER_MAX)
#error FRTSYN_FR_CYCLE_COUNTER_MAX already defined!
#endif /* #if (defined FRTSYN_FR_CYCLE_COUNTER_MAX)*/
#define FRTSYN_FR_CYCLE_COUNTER_MAX 64U

#if (defined FRTSYN_SGW_MASK)
#error FRTSYN_SGW_MASK already defined!
#endif /* #if (defined FRTSYN_SGW_MASK)*/
#define FRTSYN_SGW_MASK 0x2U

#if (defined FRTSYN_SGW_TX_MASK)
#error FRTSYN_SGW_TX_MASK already defined!
#endif /* #if (defined FRTSYN_SGW_TX_MASK)*/
#define FRTSYN_SGW_TX_MASK 0x04U

#if (defined FRTSYN_SECONDS_BYTE_0)
#error FRTSYN_SECONDS_BYTE_0 already defined!
#endif /* #if (defined FRTSYN_SECONDS_BYTE_0)*/
#define FRTSYN_SECONDS_BYTE_0 6

#if (defined FRTSYN_SECONDS_BYTE_1)
#error FRTSYN_SECONDS_BYTE_1 already defined!
#endif /* #if (defined FRTSYN_SECONDS_BYTE_1)*/
#define FRTSYN_SECONDS_BYTE_1 (FRTSYN_SECONDS_BYTE_0 + 1)

#if (defined FRTSYN_SECONDS_BYTE_2)
#error FRTSYN_SECONDS_BYTE_2 already defined!
#endif /* #if (defined FRTSYN_SECONDS_BYTE_2)*/
#define FRTSYN_SECONDS_BYTE_2 (FRTSYN_SECONDS_BYTE_0 + 2)

#if (defined FRTSYN_SECONDS_BYTE_3)
#error FRTSYN_SECONDS_BYTE_3 already defined!
#endif /* #if (defined FRTSYN_SECONDS_BYTE_3)*/
#define FRTSYN_SECONDS_BYTE_3 (FRTSYN_SECONDS_BYTE_0 + 3)

#if (defined FRTSYN_SECONDS_BYTE_4)
#error FRTSYN_SECONDS_BYTE_4 already defined!
#endif /* #if (defined FRTSYN_SECONDS_BYTE_4)*/
#define FRTSYN_SECONDS_BYTE_4 (FRTSYN_SECONDS_BYTE_0 + 4)

#if (defined FRTSYN_SECONDS_BYTE_5)
#error FRTSYN_SECONDS_BYTE_5 already defined!
#endif /* #if (defined FRTSYN_SECONDS_BYTE_5)*/
#define FRTSYN_SECONDS_BYTE_5 (FRTSYN_SECONDS_BYTE_0 + 5)

#if (defined FRTSYN_NSECONDS_BYTE_0)
#error FRTSYN_NSECONDS_BYTE_0 already defined!
#endif /* #if (defined FRTSYN_NSECONDS_BYTE_0)*/
#define FRTSYN_NSECONDS_BYTE_0 12

#if (defined FRTSYN_NSECONDS_BYTE_1)
#error FRTSYN_NSECONDS_BYTE_1 already defined!
#endif /* #if (defined FRTSYN_NSECONDS_BYTE_1)*/
#define FRTSYN_NSECONDS_BYTE_1 (FRTSYN_NSECONDS_BYTE_0 + 1)

#if (defined FRTSYN_NSECONDS_BYTE_2)
#error FRTSYN_NSECONDS_BYTE_2 already defined!
#endif /* #if (defined FRTSYN_NSECONDS_BYTE_2)*/
#define FRTSYN_NSECONDS_BYTE_2 (FRTSYN_NSECONDS_BYTE_0 + 2)

#if (defined FRTSYN_NSECONDS_BYTE_3)
#error FRTSYN_NSECONDS_BYTE_3 already defined!
#endif /* #if (defined FRTSYN_NSECONDS_BYTE_3)*/
#define FRTSYN_NSECONDS_BYTE_3 (FRTSYN_NSECONDS_BYTE_0 + 3)

#if (defined FRTSYN_USER_DATA_BYTE_0)
#error FRTSYN_USER_DATA_BYTE_0 already defined!
#endif /* #if (defined FRTSYN_USER_DATA_BYTE_0)*/
#define FRTSYN_USER_DATA_BYTE_0 4

#if (defined FRTSYN_USER_DATA_BYTE_1)
#error FRTSYN_USER_DATA_BYTE_1 already defined!
#endif /* #if (defined FRTSYN_USER_DATA_BYTE_1)*/
#define FRTSYN_USER_DATA_BYTE_1 (FRTSYN_USER_DATA_BYTE_0 + 1)

#if (defined FRTSYN_USER_DATA_BYTE_2)
#error FRTSYN_USER_DATA_BYTE_2 already defined!
#endif /* #if (defined FRTSYN_USER_DATA_BYTE_2)*/
#define FRTSYN_USER_DATA_BYTE_2 1

#if (defined FRTSYN_NANOSECONDS_MAX_VALUE)
#error FRTSYN_NANOSECONDS_MAX_VALUE already defined!
#endif /* #if (defined FRTSYN_NANOSECONDS_MAX_VALUE)*/
#define FRTSYN_NANOSECONDS_MAX_VALUE 0x3B9ACA00U

#if (defined FRTSYN_NS_PER_SEC)
#error FRTSYN_NS_PER_SEC already defined!
#endif /* #if (defined FRTSYN_NS_PER_SEC)*/
#define FRTSYN_NS_PER_SEC 1000000000U

#if (defined FRTSYN_US_PER_SEC)
#error FRTSYN_US_PER_SEC already defined!
#endif /* #if (defined FRTSYN_US_PER_SEC)*/
#define FRTSYN_US_PER_SEC 1000000U

#if (defined FRTSYN_US_TO_NS)
#error FRTSYN_US_TO_NS already defined!
#endif /* #if (defined FRTSYN_US_TO_NS)*/
#define FRTSYN_US_TO_NS 1000U

#if (defined FRTSYN_NS_TO_US)
#error FRTSYN_NS_TO_US already defined!
#endif /* #if (defined FRTSYN_NS_TO_US)*/
#define FRTSYN_NS_TO_US 1000U

/*==================[internal function declarations]=======================*/

#define FRTSYN_START_SEC_CODE
#include <FrTSyn_MemMap.h>

/**
 * \brief Service add nanoseconds to StbM_TimeStampType
 * \param[inout] timeStampPtr Pointer to out current Time Stamp.
 * \param[in] nanosecondsToAdd How many nanoseconds to add.
 */
STATIC FUNC(void, FRTSYN_CODE) FrTSyn_AddToTimeStamp
(
  P2VAR(StbM_TimeStampType, AUTOMATIC, AUTOMATIC) timeStampPtr,
  uint32 nanosecondsToAdd
);

/**
 * \brief Service subtract from current Time Stamp
 * \param[inout] timeStampPtr to out current Time Stamp.
 * \param[in] nanosecondsToSubtract How many nanoseconds to subtract
 */
#if(FRTSYN_IS_SLAVE_FUNC_USED == STD_ON)
STATIC FUNC(void, FRTSYN_CODE) FrTSyn_SubtractFromTimeStamp
(
  P2VAR(StbM_TimeStampType, AUTOMATIC, AUTOMATIC) timeStampPtr,
  uint32 nanosecondsToSubtract
);

/**
 * \brief Processes incoming messages
 *
 * \param[in] TimeDomainIdx Idx of the Time Domain in the
 *            generated config structure array
 *
 * \return void
 */
STATIC FUNC(void, FRTSYN_CODE) FrTSyn_ProcessSlave
(
  uint8 TimeDomainIdx
);

/**
 * \brief Prepare and send received data to StbM
 *
 * \param[in] receivedTimeStamp -  received time stamp data
 *            userData -  received time user data
 *            TimeDomainIdx - Idx of the Time Domain in the
 *            generated config structure array
 *            receivedCycle - received FlexRay cycle
 *            isSync - time domain is sync or not
 *
 * \return void
 */
STATIC FUNC(void, FRTSYN_CODE) FrTSyn_SendToStbM
(
  P2VAR(StbM_TimeStampType, AUTOMATIC, AUTOMATIC) receivedTimeStampPtr,
  P2VAR(StbM_UserDataType, AUTOMATIC, AUTOMATIC) userDataPtr,
  uint8 TimeDomainIdx,
  uint8 receivedCycle,
  boolean isSync
);
#endif /* (FRTSYN_IS_SLAVE_FUNC_USED == STD_ON) */

#if(FRTSYN_IS_MASTER_FUNC_USED == STD_ON)
/**
 * \brief Transmits timesync message
 *
 * \param[in] TimeDomainIdx Idx of the Time Domain in the
 *            generated config structure array
 *
 * \return void
 */
STATIC FUNC(void, FRTSYN_CODE) FrTSyn_SendTSMsg
(
  uint8 TimeDomainIdx
);

/**
 * \brief Service to disassemble a variable to Array, and add it
 * to the destination.
 *
 * \param[in] value - Value to be disassembled.
 * \param[in] length - Lenghth in bytes of the variable.
 *
 * \param[out] destination - Pointer containing the destiantion data.
 *
 */
LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_AddDataToArray
(
  uint8* destination,
  uint32 value,
  uint8 length
);

/**
 * \brief Evaluates whether the time base has been set previously
 *
 * \param[in, out] pMasterData Pointer to runtime data
 * \param[in] pMasterCfg Config to configuration data.
 *
 * \return TRUE TimeBase set previously
 *         FALSE No TimeBase NOT set previously
 */

STATIC FUNC(boolean, FRTSYN_CODE) FrTSyn_IsTimeBaseSet
(
  P2VAR(FrTSyn_MasterType, AUTOMATIC, AUTOMATIC) pMasterData,
  P2CONST(FrTSyn_MasterTimeDomainConfigType, AUTOMATIC, AUTOMATIC) pMasterCfg
);
#endif /* (FRTSYN_IS_MASTER_FUNC_USED == STD_ON) */

#if (FRTSYN_TXTYPE_PERIODIC_USED == STD_ON)
/**
 * \brief Evaluates periodic transmission conditions
 *
 * \param[in, out] pMasterData Pointer to runtime data
 * \param[in] pMasterCfg Config to configuration data
 *
 * \return TRUE Transmission has to occur
 *         FALSE No transmission must occur
 */
STATIC FUNC(boolean, FRTSYN_CODE) FrTSyn_TxCondFunc_Periodic
(
  P2VAR(FrTSyn_MasterType, AUTOMATIC, AUTOMATIC) pMasterData,
  P2CONST(FrTSyn_MasterTimeDomainConfigType, AUTOMATIC, AUTOMATIC) pMasterCfg
);
#endif

#if (FRTSYN_TXTYPE_IMMEDIATE_USED == STD_ON)
/**
 * \brief Evaluates immediate transmission conditions
 *
 * \param[in, out] pMasterData Pointer to runtime data
 * \param[in] pMasterCfg Config to configuration data
 *
 * \return TRUE Transmission has to occur
 *         FALSE No transmission must occur
 */
STATIC FUNC(boolean, FRTSYN_CODE) FrTSyn_TxCondFunc_Immediate
(
  P2VAR(FrTSyn_MasterType, AUTOMATIC, AUTOMATIC) pMasterData,
  P2CONST(FrTSyn_MasterTimeDomainConfigType, AUTOMATIC, AUTOMATIC) pMasterCfg
);
#endif

#if (FRTSYN_TXTYPE_BOTH_USED == STD_ON)
/**
 * \brief Evaluates transmission conditions
 *        for timedomains which are configured
 *        for both periodic and immediate
 *        transmission
 *
 * \param[in, out] pMasterData Pointer to runtime data
 * \param[in] pMasterCfg Config to configuration data
 *
 * \return TRUE Transmission has to occur
 *         FALSE No transmission must occur
 */
STATIC FUNC(boolean, FRTSYN_CODE) FrTSyn_TxCondFunc_Both
(
  P2VAR(FrTSyn_MasterType, AUTOMATIC, AUTOMATIC) pMasterData,
  P2CONST(FrTSyn_MasterTimeDomainConfigType, AUTOMATIC, AUTOMATIC) pMasterCfg
);
#endif

#define FRTSYN_STOP_SEC_CODE
#include <FrTSyn_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define FRTSYN_START_SEC_VAR_INIT_8
#include <FrTSyn_MemMap.h>

#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
/** \brief Variable to indicate that module was initialized. */
STATIC VAR(boolean, FRTSYN_VAR) FrTSyn_Initialized = FALSE;
#endif

#define FRTSYN_STOP_SEC_VAR_INIT_8
#include <FrTSyn_MemMap.h>

#if(FRTSYN_CRC_CHECK_USED == STD_ON)
#define FRTSYN_START_SEC_VAR_CLEARED_8
#include <FrTSyn_MemMap.h>
/* Deviation MISRAC2012-2 */
/* Where CRC data will be stored (input for Crc_CalculateCRC8H2F() ) */
STATIC VAR(uint8, FRTSYN_VAR) CRCData[FRTSYN_CRC_DATA_LENGTH];

#define FRTSYN_STOP_SEC_VAR_CLEARED_8
#include <FrTSyn_MemMap.h>
#endif /* (FRTSYN_CRC_CHECK_USED == STD_ON) */

#define FRTSYN_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrTSyn_MemMap.h>

#if(FRTSYN_IS_SLAVE_FUNC_USED == STD_ON)
/** \brief FrTSyn shared variables. */
STATIC VAR(FrTSyn_SlaveType, FRTSYN_VAR) FrTSyn_Slave[FRTSYN_NUMBER_OF_TIME_SLAVES];
#endif /* (FRTSYN_IS_SLAVE_FUNC_USED == STD_ON) */

#if(FRTSYN_IS_MASTER_FUNC_USED == STD_ON)
STATIC VAR(FrTSyn_MasterType, FRTSYN_VAR) FrTSyn_Master[FRTSYN_NUMBER_OF_TIME_MASTERS];

#if(FRTSYN_TIME_VALIDATION_ENABLED == STD_ON)
STATIC VAR(StbM_FrTimeMasterMeasurementType, FRTSYN_VAR) FrTSyn_TimeValidationData[FRTSYN_NUMBER_OF_TIME_MASTERS];
#endif /* (FRTSYN_TIME_VALIDATION_ENABLED == STD_ON) */

#endif /* (FRTSYN_IS_MASTER_FUNC_USED == STD_ON) */

#define FRTSYN_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrTSyn_MemMap.h>

#if(FRTSYN_IS_MASTER_FUNC_USED == STD_ON)
#define FRTSYN_START_SEC_CONST_UNSPECIFIED
#include <FrTSyn_MemMap.h>
/* Deviation MISRAC2012-2 */
STATIC CONST(FrTSyn_TxCondFuncType, FRTSYN_APPL_CONST) FrTSyn_TxCondFuncs[3U] =
{
  FRTSYN_TX_PERIODIC_LUT_NAME,
  FRTSYN_TX_IMMEDIATE_LUT_NAME,
  FRTSYN_TX_BOTH_LUT_NAME
};
#define FRTSYN_STOP_SEC_CONST_UNSPECIFIED
#include <FrTSyn_MemMap.h>
#endif /* FRTSYN_IS_MASTER_FUNC_USED == STD_ON */

/*==================[external function definitions]=========================*/
#define FRTSYN_START_SEC_CODE
#include <FrTSyn_MemMap.h>

/* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00064,1 */
#if(FRTSYN_VERSION_INFO_API == STD_ON)
FUNC(void, FRTSYN_CODE) FrTSyn_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, FRTSYN_APPL_DATA) versioninfo
)
{
  DBG_FRTSYN_GETVERSIONINFO_ENTRY(versioninfo);

#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* check if the versioninfo pointer is a Null Pointer */
  if(NULL_PTR == versioninfo)
  {
    /* !LINKSTO FrTSyn.EB.GetVersionInfo.NullPtr,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00058,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00059,1 */
    FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_GETVERSIONINFO, FRTSYN_E_NULL_POINTER);
  }
  else
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
  {
    /* Return version information */
    versioninfo->vendorID = FRTSYN_VENDOR_ID;
    versioninfo->moduleID = FRTSYN_MODULE_ID;
    versioninfo->sw_major_version = FRTSYN_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = FRTSYN_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = FRTSYN_SW_PATCH_VERSION;
  }

  DBG_FRTSYN_GETVERSIONINFO_EXIT(versioninfo);
}
#endif /* FRTSYN_VERSION_INFO_API */

/* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00063,1 */
FUNC(void, FRTSYN_CODE) FrTSyn_Init
(
  P2CONST(FrTSyn_ConfigType, AUTOMATIC, FRTSYN_APPL_CONST) configPtr
)
{

  VAR(uint8, AUTOMATIC) FrTSyn_DomainActiveIndex;

  DBG_FRTSYN_INIT_ENTRY(CfgPtr);

  /* current implementation supports only Precompile variant */
  TS_PARAM_UNUSED(configPtr);

  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00003,1 */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00005,1 */
#if(FRTSYN_IS_SLAVE_FUNC_USED == STD_ON)
  for(FrTSyn_DomainActiveIndex = 0U;
      /* Deviation TASKING-1 */
      FrTSyn_DomainActiveIndex < FRTSYN_NUMBER_OF_TIME_SLAVES;
      FrTSyn_DomainActiveIndex++)
  {
    CONSTP2CONST(FrTSyn_SlaveTimeDomainConfigType, AUTOMATIC, FRTSYN_APPL_CONST) pSlaveCfg = &FrTSyn_SlaveConfig[FrTSyn_DomainActiveIndex];
    CONSTP2VAR(FrTSyn_SlaveType, AUTOMATIC, FRTSYN_APPL_DATA) pSlaveData = &FrTSyn_Slave[FrTSyn_DomainActiveIndex];

    /* Set Cycle Length for every Slave */
    pSlaveData->cycleLength = FrIf_GetCycleLength(pSlaveCfg->ctrlId);
    /* Set Maximum Cycle Length to avoid calculating it in each MF */
    pSlaveData->maxCycleLength = FRTSYN_FR_CYCLE_COUNTER_MAX * pSlaveData->cycleLength;
    /* Set Macrotick Duration for every Slave */
    pSlaveData->macrotickDuration = FrIf_GetMacrotickDuration(pSlaveCfg->ctrlId);
    /* Clear the received Sequence counter */
    pSlaveData->seqCount = FRTSYN_SEQ_COUNTER_NOCHECK;
    /* Clear the reception Flag */
    pSlaveData->isReceptionOngoing = FALSE;
    /* The value will point to self initially */
    pSlaveData->assocIdx = FrTSyn_DomainActiveIndex;
    /* Clear received message data */
    TS_MemBZero(pSlaveData->receivedMsg, FRTSYN_MESSAGE_PAYLOAD);
  }
#endif /* (FRTSYN_IS_SLAVE_FUNC_USED == STD_ON) */

#if(FRTSYN_IS_MASTER_FUNC_USED == STD_ON)
  for(FrTSyn_DomainActiveIndex = 0U;
      /* Deviation TASKING-1 */
      FrTSyn_DomainActiveIndex < FRTSYN_NUMBER_OF_TIME_MASTERS;
      FrTSyn_DomainActiveIndex++)
  {
    CONSTP2CONST(FrTSyn_MasterTimeDomainConfigType, AUTOMATIC, AUTOMATIC) pMasterCfg = &FrTSyn_MasterConfig[FrTSyn_DomainActiveIndex];
    CONSTP2VAR(FrTSyn_MasterType, AUTOMATIC, AUTOMATIC) pMasterData = &FrTSyn_Master[FrTSyn_DomainActiveIndex];
#if (FRTSYN_TIME_VALIDATION_ENABLED == STD_ON)
    CONSTP2VAR(StbM_FrTimeMasterMeasurementType, AUTOMATIC, AUTOMATIC) pTimeValidationData = &FrTSyn_TimeValidationData[FrTSyn_DomainActiveIndex];
#endif /* FRTSYN_TIME_VALIDATION_ENABLED == STD_ON */

    /* Set Cycle Length for every Slave */
    pMasterData->cycleLength = FrIf_GetCycleLength(pMasterCfg->ctrlId);
    /* Set Macrotick Duration for every Slave */
    pMasterData->macrotickDuration = FrIf_GetMacrotickDuration(pMasterCfg->ctrlId);
    /* Initialize Tx Period to 0 */
    pMasterData->txPeriod = 0U;
#if(FRTSYN_IS_ITS_MASTER_FUNC_USED == STD_ON)
    /* Clear the Debounce Counter */
    pMasterData->dbncCntr = 0U;
    /* Clear the TimeBaseUpdateCounter */
    /* Could fetch from StbM but if it has changed it'll sync soon */
    pMasterData->timebaseUpdCntr = 0U;
#endif
    /* Set Tx Mode to ON for now */
    pMasterData->txMode = FRTSYN_TX_ON;
    /* Tx Properties */
    pMasterData->txProperties = 0U;
    /* Set assocIdx to enable transmission */
    pMasterData->assocIdx = FRTSYN_RESERVED_TT_DONE;
    /* Clear the Sequence Counter */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00006,1 */
    pMasterData->seqCount = 0U;
    /* Clear message data */
    TS_MemBZero(pMasterData->msgToTransmit, FRTSYN_MESSAGE_PAYLOAD);

#if(FRTSYN_TIME_VALIDATION_ENABLED == STD_ON)
    pTimeValidationData->sequenceCounter = 0U;
    pTimeValidationData->referenceTimestamp.nanosecondsLo = 0U;
    pTimeValidationData->referenceTimestamp.nanosecondsHi = 0U;
    pTimeValidationData->preciseOriginTimestamp.seconds = 0U;
    pTimeValidationData->preciseOriginTimestamp.nanoseconds = 0U;
    pTimeValidationData->segmentId = 0U;
#endif /* (FRTSYN_TIME_VALIDATION_ENABLED == STD_ON) */

  }
#endif /* (FRTSYN_IS_MASTER_FUNC_USED == STD_ON) */

#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
  FrTSyn_Initialized = TRUE;
#endif

  DBG_FRTSYN_INIT_EXIT(CfgPtr);
}

/* !LINKSTO FrTSyn.EB.TxConfirmationDummy,1 */
FUNC(void, FRTSYN_CODE) FrTSyn_TxConfirmation
(
  PduIdType TxPduId
)
{
  TS_PARAM_UNUSED(TxPduId);
}

/* !LINKSTO FrTSyn.EB.FrTSyn_RxIndication_PduInfoPtr,1 */
FUNC(void, FRTSYN_CODE) FrTSyn_RxIndication
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, FRTSYN_APPL_DATA) PduInfoPtr
)
{
#if(FRTSYN_IS_SLAVE_FUNC_USED == STD_OFF)
  TS_PARAM_UNUSED(RxPduId);
  TS_PARAM_UNUSED(PduInfoPtr);
#endif /* (FRTSYN_IS_SLAVE_FUNC_USED == STD_ON) */

  DBG_FRTSYN_RXINDICATION_ENTRY(RxPduId, PduInfoPtr);

#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* check if the module was initialized */
  if(FALSE == FrTSyn_Initialized)
  {
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00067.UNINIT.RxIndication,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00058,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00059,1 */
    FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_RXINDICATION, FRTSYN_E_UNINIT);
  }
  /* check if the function was called with valid PduId */
  else
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
  {
#if(FRTSYN_IS_SLAVE_FUNC_USED == STD_ON)
    if(NULL_PTR != PduInfoPtr)
    {
      /* ignore messages of incorrect length */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00009,1 */
      if (FRTSYN_MESSAGE_PAYLOAD == PduInfoPtr->SduLength)
      {
        if(NULL_PTR != PduInfoPtr->SduDataPtr)
        {
          /* check PduId validity */
          const uint8 receivedTimeDomain = ((uint8) PduInfoPtr->SduDataPtr[FRTSYN_TIME_DOMAIN_SEQ_COUNTER_BYTE]) >> FRTSYN_TIME_DOMAIN_SHIFT;

          if((RxPduId < FRTSYN_NUMBER_OF_TIME_SLAVES) && ((receivedTimeDomain == FrTSyn_SlaveConfig[RxPduId].timeDomainId) || ((receivedTimeDomain + 16U) == FrTSyn_SlaveConfig[FrTSyn_SlaveConfig[RxPduId].assocIdx].timeDomainId)))
          {
            const uint8 assocIdx = FrTSyn_Slave[RxPduId].assocIdx;

            CONSTP2CONST(FrTSyn_SlaveTimeDomainConfigType, AUTOMATIC, FRTSYN_APPL_CONST) pSlaveCfg = &FrTSyn_SlaveConfig[assocIdx];
            CONSTP2VAR(FrTSyn_SlaveType, AUTOMATIC, FRTSYN_APPL_DATA) pSlaveData = &FrTSyn_Slave[assocIdx];

            if(FALSE == pSlaveData->isReceptionOngoing)
            {
              SchM_Enter_FrTSyn_SCHM_FRTSYN_EXCLUSIVE_AREA_0();

              /* Store received message */
              TS_MemCpy(pSlaveData->receivedMsg, PduInfoPtr->SduDataPtr, FRTSYN_MESSAGE_PAYLOAD);
              /* Update reference */
              FrTSyn_Slave[RxPduId].assocIdx = pSlaveCfg->assocIdx;
              pSlaveData->isReceptionOngoing = TRUE;

              SchM_Exit_FrTSyn_SCHM_FRTSYN_EXCLUSIVE_AREA_0();
            }
          }
          else
          {
#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
            /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00067.INVALID_PDUID.RxIndication,1 */
            /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00058,1 */
            /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00059,1 */
            FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_RXINDICATION, FRTSYN_E_INVALID_PDUID);
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
          }
        }
        else
        {
#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
          /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00067.SduDataPtr.E_NULL_POINTER.RxIndication,1 */
          /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00058,1 */
          /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00059,1 */
          FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_RXINDICATION, FRTSYN_E_NULL_POINTER);
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
        }
      }
      else
      {
#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00067.PduInfoPtr.INVALID_SDULENGTH.RxIndication,1 */
        FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_RXINDICATION, FRTSYN_E_PARAM);
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
      }
    }
    else
    {
#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00067.PduInfoPtr.E_NULL_POINTER.RxIndication,1 */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00058,1 */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00059,1 */
      FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_RXINDICATION, FRTSYN_E_NULL_POINTER);
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
    }
#endif /* (FRTSYN_IS_SLAVE_FUNC_USED == STD_ON) */
  }

  DBG_FRTSYN_RXINDICATION_EXIT(RxPduId, PduInfoPtr);
}

/* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00071,1 */
FUNC(void, FRTSYN_CODE) FrTSyn_MainFunction(void)
{
  /* Current index of the Time Domain */
  uint8 FrTSyn_TimeDomainActiveIndex;

  DBG_FRTSYN_MAINFUNCTION_ENTRY();

#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* check if the module was initialized */
  if(FALSE == FrTSyn_Initialized)
  {
    /* !LINKSTO FrTSyn.EB.MainFunction.Uninit,1 */
    FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_MAINFUNCTION, FRTSYN_E_UNINIT);
  }
  /* check if the function was called with valid PduId */
  else
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
  {
#if(FRTSYN_IS_SLAVE_FUNC_USED == STD_ON)
    for(FrTSyn_TimeDomainActiveIndex = 0U;
        /* Deviation TASKING-1 */
        FrTSyn_TimeDomainActiveIndex < FRTSYN_NUMBER_OF_TIME_SLAVES;
        FrTSyn_TimeDomainActiveIndex++)
    {
      /* Check if we have received a message on the current Time Domain, if yes, process the message */
      boolean receptionOngoing;
      TS_AtomicAssign8(receptionOngoing, FrTSyn_Slave[FrTSyn_TimeDomainActiveIndex].isReceptionOngoing);

      if(TRUE == receptionOngoing)
      {
        FrTSyn_ProcessSlave(FrTSyn_TimeDomainActiveIndex);

        /* Clear the Flag for ongoing reception */
        TS_AtomicAssign8(FrTSyn_Slave[FrTSyn_TimeDomainActiveIndex].isReceptionOngoing, FALSE);
      }
    }
#endif /* (FRTSYN_IS_SLAVE_FUNC_USED == STD_ON) */

#if(FRTSYN_IS_MASTER_FUNC_USED == STD_ON)
  for(FrTSyn_TimeDomainActiveIndex = 0U;
      /* Deviation TASKING-1 */
      FrTSyn_TimeDomainActiveIndex < FRTSYN_NUMBER_OF_TIME_MASTERS;
      FrTSyn_TimeDomainActiveIndex++)
  {
    CONSTP2VAR(FrTSyn_MasterType, AUTOMATIC, AUTOMATIC) pMasterData = &FrTSyn_Master[FrTSyn_TimeDomainActiveIndex];
    CONSTP2CONST(FrTSyn_MasterTimeDomainConfigType, AUTOMATIC, AUTOMATIC) pMasterCfg = &FrTSyn_MasterConfig[FrTSyn_TimeDomainActiveIndex];

    /* Check that transmission is enabled for this channel */
    /* Evaluate condition for transmission and handle timers */
    if((TRUE == FRTSYN_TX_COND(pMasterData, pMasterCfg)) && (FRTSYN_TX_OFF != pMasterData->txMode))
    {
      /* FrIf returned state */
      FrIf_StateType state;
      const Std_ReturnType ret = FrIf_GetState(pMasterCfg->clstId, &state);
      if((ret == E_OK) && (state == FRIF_STATE_ONLINE))
      {
        /* Only call FrIf_GetGlobalTime() if FrIf_GetState() returns FRIF_STATE_ONLINE */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00078,1 */
        FrTSyn_SendTSMsg(FrTSyn_TimeDomainActiveIndex);
      }
    }
  }
#endif /* (FRTSYN_IS_MASTER_FUNC_USED == STD_ON) */
  }
  DBG_FRTSYN_MAINFUNCTION_EXIT();
}

/* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00069,1 */
/* Deviation MISRAC2012-1 <+4> */
FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_TriggerTransmit
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, FRTSYN_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

#if(FRTSYN_IS_MASTER_FUNC_USED == STD_OFF)
  TS_PARAM_UNUSED(TxPduId);
  TS_PARAM_UNUSED(PduInfoPtr);
#endif /* (FRTSYN_IS_MASTER_FUNC_USED == STD_ON) */

  DBG_FRTSYN_TRIGGERTRANSMIT_ENTRY(TxPduId, PduInfoPtr);

#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* check if the module was initialized */
  if(FALSE == FrTSyn_Initialized)
  {
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00070.UNINIT.TriggerTransmit,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00058,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00059,1 */
    FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_TRIGGERTRANSMIT, FRTSYN_E_UNINIT);
  }
  /* check if the function was called with valid PduId */
  else
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
  {
#if(FRTSYN_IS_MASTER_FUNC_USED == STD_ON)
      if(NULL_PTR != PduInfoPtr)
      {
        if(NULL_PTR != PduInfoPtr->SduDataPtr)
        {
          /* check PduId validity */
          if(TxPduId < FRTSYN_NUMBER_OF_TIME_MASTERS)
          {
            uint8 assocIdx;
            CONSTP2CONST(FrTSyn_MasterTimeDomainConfigType, AUTOMATIC, AUTOMATIC) pMasterCfg = &FrTSyn_MasterConfig[TxPduId];

            TS_AtomicAssign8(assocIdx, FrTSyn_Master[TxPduId].assocIdx);

            if(FRTSYN_RESERVED_TT_DONE != assocIdx)
            {
              SchM_Enter_FrTSyn_SCHM_FRTSYN_EXCLUSIVE_AREA_0();

              /* Copy data to the received ptr */
              TS_MemCpy(&PduInfoPtr->SduDataPtr[0], &FrTSyn_Master[assocIdx].msgToTransmit[0], FRTSYN_MESSAGE_PAYLOAD);
              /* Copy the length of the data to the ptr */
              PduInfoPtr->SduLength = FRTSYN_MESSAGE_PAYLOAD;

              /* This is used because of multiplexed PDUs */
              FrTSyn_Master[TxPduId].assocIdx = pMasterCfg->assocIdx;

              SchM_Exit_FrTSyn_SCHM_FRTSYN_EXCLUSIVE_AREA_0();

#if(FRTSYN_TIME_VALIDATION_ENABLED == STD_ON)
              /* !LINKSTO FrTSyn.ASR19-11.SWS_FrTSyn_00100.Decoupled,1 */
              /* !LINKSTO FrTSyn.dsn.TimeValidationTriggerTransmit,1 */
              if(FRTSYN_TIME_VALIDATION_ENABLED_ON_DOMAIN(pMasterCfg))
              {
                StbM_FrTimeMasterMeasurementType measurementData;

                SchM_Enter_FrTSyn_SCHM_FRTSYN_EXCLUSIVE_AREA_0();

                FRTSYN_ASSIGN_TIME_DATA(measurementData, FrTSyn_TimeValidationData[TxPduId]);

                SchM_Exit_FrTSyn_SCHM_FRTSYN_EXCLUSIVE_AREA_0();

                (void)StbM_FrSetMasterTimingData(pMasterCfg->syncTimeBaseId, &measurementData);
              }
#endif /* FRTSYN_TIME_VALIDATION_ENABLED == STD_ON */

              RetVal = E_OK;
            }
            else
            {
#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
            /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00070.INVALID_PDUID.TriggerTransmit,1 */
            /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00058,1 */
            /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00059,1 */
            FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_TRIGGERTRANSMIT, FRTSYN_E_INVALID_PDUID);
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
            }
          }
          else
          {
#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
            /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00070.INVALID_PDUID.TriggerTransmit,1 */
            /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00058,1 */
            /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00059,1 */
            FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_TRIGGERTRANSMIT, FRTSYN_E_INVALID_PDUID);
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
          }
        }
        else
        {
#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
          /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00070.SduDataPtr.E_NULL_POINTER.TriggerTransmit,1 */
          /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00058,1 */
          /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00059,1 */
          FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_TRIGGERTRANSMIT, FRTSYN_E_NULL_POINTER);
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
        }
      }
      else
      {
#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00070.PduInfoPtr.E_NULL_POINTER.TriggerTransmit,1 */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00058,1 */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00059,1 */
        FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_TRIGGERTRANSMIT, FRTSYN_E_NULL_POINTER);
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
      }
#endif /* FRTSYN_IS_MASTER_FUNC_USED == STD_ON */
  }

  DBG_FRTSYN_TRIGGERTRANSMIT_EXIT(TxPduId, PduInfoPtr);

  return RetVal;
}

/* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00065,1 */
FUNC(void, FRTSYN_CODE) FrTSyn_SetTransmissionMode
(
  uint8 CtrlIdx,
  FrTSyn_TransmissionModeType Mode
)
{

#if(FRTSYN_IS_MASTER_FUNC_USED == STD_ON)
  VAR(uint8, AUTOMATIC) FrTSyn_TimeMasterActiveIndex;
#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
  VAR(uint8, AUTOMATIC) FrTSyn_ReportInvalidCtrlIdx = TRUE;
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
#else
  TS_PARAM_UNUSED(CtrlIdx);
  TS_PARAM_UNUSED(Mode);
#endif /* (FRTSYN_IS_MASTER_FUNC_USED == STD_ON) */

  DBG_FRTSYN_SETTRANSMISSIONMODE_ENTRY(CtrlIdx, Mode);

/* !LINKSTO FrTSyn.EB.SetTransmissionMode.Uninit,1 */
#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* check if the module was initialized */
  if(FALSE == FrTSyn_Initialized)
  {
    FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_SETTRANSMISSIONMODE, FRTSYN_E_UNINIT);
  }
#if(FRTSYN_IS_MASTER_FUNC_USED == STD_ON)
  else if((FRTSYN_TX_ON != Mode) && (FRTSYN_TX_OFF != Mode))
  {
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00095.Mode.E_PARAM.SetTransmissionMode,1 */
    FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_SETTRANSMISSIONMODE, FRTSYN_E_PARAM);
  }
#endif /* (FRTSYN_IS_MASTER_FUNC_USED == STD_ON) */
  /* change the mode of the received controller */
  else
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
  {
#if(FRTSYN_IS_MASTER_FUNC_USED == STD_ON)
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00026,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00027,1 */
    for(FrTSyn_TimeMasterActiveIndex = 0U;
        /* Deviation TASKING-1 */
        FrTSyn_TimeMasterActiveIndex < FRTSYN_NUMBER_OF_TIME_MASTERS;
        FrTSyn_TimeMasterActiveIndex++)
    {
      if(CtrlIdx == FrTSyn_MasterConfig[FrTSyn_TimeMasterActiveIndex].ctrlId)
      {
        FrTSyn_Master[FrTSyn_TimeMasterActiveIndex].txMode = Mode;
#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
        FrTSyn_ReportInvalidCtrlIdx = FALSE;
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
      }
    }
#if(FRTSYN_DEV_ERROR_DETECT == STD_ON)
    /* check if CtrlIdx is invalid */
    if(TRUE == FrTSyn_ReportInvalidCtrlIdx)
    {
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00095.CtrlIdx.E_INV_CTRL_IDX.SetTransmissionMode,1 */
      FRTSYN_DET_REPORT_ERROR(FRTSYN_SID_SETTRANSMISSIONMODE, FRTSYN_E_INV_CTRL_IDX);
    }
#endif /* FRTSYN_DEV_ERROR_DETECT == STD_ON */
#endif /* (FRTSYN_IS_MASTER_FUNC_USED == STD_ON) */
  }
  DBG_FRTSYN_SETTRANSMISSIONMODE_EXIT(CtrlIdx, Mode);
}

#if(FRTSYN_IS_MASTER_FUNC_USED == STD_ON)
LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_AddDataToArray
(
  uint8* destination,
  uint32 value,
  uint8 length
)
{
  uint8 shiftNr = (length < (uint8)4U)?8U:24U;
  uint8 byteIdx;

  DBG_FRTSYN_ADDDATATOARRAY_ENTRY(destination, value, length);

  for(byteIdx = 0U;
      byteIdx < length;
      byteIdx++)
  {
    destination[byteIdx] = (uint8) (((uint32)value) >> shiftNr);
    shiftNr -= 8;
  }

  DBG_FRTSYN_ADDDATATOARRAY_EXIT(destination, value, length);
}

STATIC FUNC(void, FRTSYN_CODE) FrTSyn_SendTSMsg
(
  uint8 TimeDomainIdx
)
{
  CONSTP2VAR(FrTSyn_MasterType, AUTOMATIC, AUTOMATIC) pMasterData = &FrTSyn_Master[TimeDomainIdx];
  CONSTP2CONST(FrTSyn_MasterTimeDomainConfigType, AUTOMATIC, AUTOMATIC) pMasterCfg = &FrTSyn_MasterConfig[TimeDomainIdx];

#if (FRTSYN_TIME_VALIDATION_ENABLED == STD_ON)
  CONSTP2VAR(StbM_FrTimeMasterMeasurementType, AUTOMATIC, AUTOMATIC) pTimeValidationData = &FrTSyn_TimeValidationData[TimeDomainIdx];
  StbM_FrTimeMasterMeasurementType measurementData;
#endif /* FRTSYN_TIME_VALIDATION_ENABLED == STD_ON */

  /* Where User Data will be stored from StbM */
  StbM_UserDataType userData;
  /* Where Time Stamp will be sotred from StbM */
  StbM_TimeStampType globalTime;
  /* Where the Tx data sent to FrIf will be stored */
  PduInfoType TxDataPtr;
  /* Either Sync or Ofs processing is ok, message may be assembled */
  boolean eitherOK = FALSE;
  uint8 PtrData[FRTSYN_MESSAGE_PAYLOAD] = {0U};
  TxDataPtr.SduDataPtr = PtrData;

  if(pMasterCfg->timeDomainId < 16U)
  {
    StbM_VirtualLocalTimeType virtLocalT0;

    /* SWS_FrTSyn_00028 (2) - Retrieve current time as [Tsync;T0vlt] */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00028,1 */
    if (StbM_BusGetCurrentTime(pMasterCfg->syncTimeBaseId, &globalTime, &virtLocalT0, &userData) == E_OK)
    {
      StbM_VirtualLocalTimeType virtLocalT1;
      uint16 currentMacroticks;
      uint8 currentCycle;
      Std_ReturnType frifRet;
      Std_ReturnType stbmRet;

      SchM_Enter_FrTSyn_SCHM_FRTSYN_EXCLUSIVE_AREA_1();

      frifRet = FrIf_GetGlobalTime(pMasterCfg->ctrlId, &currentCycle, &currentMacroticks);
      stbmRet = StbM_GetCurrentVirtualLocalTime(pMasterCfg->syncTimeBaseId, &virtLocalT1);

      SchM_Exit_FrTSyn_SCHM_FRTSYN_EXCLUSIVE_AREA_1();

      if((E_OK == frifRet) && (E_OK == stbmRet))
      {
        /* Get SGW from Time Stamp */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00020,1 */
        const uint8 receivedSGW = ((globalTime.timeBaseStatus & (uint8)FRTSYN_SGW_TX_MASK) > 0U) ? 1U : 0U;

        /* The difference between T0VLT and T1VLT has to fit in an uint32 (ns),
         * the calls are right after another
         */
        uint32 addToTime = ((uint32)((uint32)FRTSYN_FR_CYCLE_COUNTER_MAX - (uint32)currentCycle) * pMasterData->cycleLength) - ((uint32)currentMacroticks * pMasterData->macrotickDuration);
        if(virtLocalT0.nanosecondsLo > virtLocalT1.nanosecondsLo)
        {
          /* Debatable which is preferred, the delay cause by + 1 or the error by omitting it */
          addToTime += (0xFFFFFFFFU - virtLocalT0.nanosecondsLo) + (virtLocalT1.nanosecondsLo + 1U);
        }
        else
        {
          addToTime += virtLocalT1.nanosecondsLo - virtLocalT0.nanosecondsLo;
        }

        FrTSyn_AddToTimeStamp(&globalTime, addToTime);

        /* SWS_FrTSyn_00037 (2) and (3) and (4) - Calculate FCNT and SGW */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00037,1 */
        pMasterData->msgToTransmit[FRTSYN_TIME_DOMAIN_SEQ_COUNTER_BYTE] =  ((pMasterCfg->timeDomainId << 4) | pMasterData->seqCount);
        pMasterData->msgToTransmit[FRTSYN_FCNT_AND_SGW_BYTE] = ((uint8)(currentCycle << 2)) | (uint8)(receivedSGW << 1);

        /* SWS_FrTSyn_00037 (4) - Add secondsHi part to message */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00037,1 */
        FrTSyn_AddDataToArray(&pMasterData->msgToTransmit[FRTSYN_SECONDS_BYTE_0], (uint32)globalTime.secondsHi, (uint8)FRTSYN_SECONDSHI_LENGTH_IN_BYTES);

#if (FRTSYN_TIME_VALIDATION_ENABLED == STD_ON)
        /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00101.seqCounter,1 */
        measurementData.sequenceCounter = pMasterData->seqCount;
        /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00101.T1VLT,1 */
        measurementData.referenceTimestamp.nanosecondsLo = virtLocalT1.nanosecondsLo;
        measurementData.referenceTimestamp.nanosecondsHi = virtLocalT1.nanosecondsHi;
        /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00101.T0,1 */
        measurementData.preciseOriginTimestamp.seconds = globalTime.seconds;
        measurementData.preciseOriginTimestamp.nanoseconds = globalTime.nanoseconds;
        /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00101.segID,1 */
        measurementData.segmentId = pMasterCfg->segmentId;
#endif /* FRTSYN_TIME_VALIDATION_ENABLED == STD_ON */

        eitherOK = TRUE;
      }
    }
    /* SWS_FrTSyn_00028 (1) - get currentCycle and currentMacroticks from FlexRay */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00028,1 */
  }
  else
  {
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00022,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00029,1 */
    if(StbM_GetOffset(pMasterCfg->syncTimeBaseId, &globalTime, &userData) == E_OK)
    {
      /* Get SGW from Time Stamp */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00020,1 */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00079,1 */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00080,1 */
      const uint8 receivedSGW = ((globalTime.timeBaseStatus & (uint8)FRTSYN_SGW_TX_MASK) > 0U) ? 1U : 0U;

      pMasterData->msgToTransmit[FRTSYN_TIME_DOMAIN_SEQ_COUNTER_BYTE] =  (((pMasterCfg->timeDomainId - 16U) << 4) | pMasterData->seqCount);
      pMasterData->msgToTransmit[FRTSYN_FCNT_AND_SGW_BYTE] = (uint8)(receivedSGW << 1);
      pMasterData->msgToTransmit[FRTSYN_SECONDS_BYTE_0] = 0U;
      pMasterData->msgToTransmit[FRTSYN_SECONDS_BYTE_1] = 0U;

      eitherOK = TRUE;
    }
  }

  if(TRUE == eitherOK)
  {
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00010,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00012,1 */
    pMasterData->msgToTransmit[FRTSYN_USER_DATA_BYTE_0] = userData.userByte0;
    pMasterData->msgToTransmit[FRTSYN_USER_DATA_BYTE_1] = userData.userByte1;

    /* SWS_FrTSyn_00037 (4) - Add seconds part to message */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00037,1 */
    FrTSyn_AddDataToArray(&pMasterData->msgToTransmit[FRTSYN_SECONDS_BYTE_2], globalTime.seconds, (uint8)FRTSYN_SECONDS_LENGTH_IN_BYTES);

    /* SWS_FrTSyn_00037 (4) - Add nanoseconds part to message */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00037,1 */
    FrTSyn_AddDataToArray(&pMasterData->msgToTransmit[FRTSYN_NSECONDS_BYTE_0], globalTime.nanoseconds, (uint8)FRTSYN_NANOSECONDS_LENGTH_IN_BYTES);

#if (FRTSYN_CRC_CHECK_USED == STD_ON)
    /* Check if CRC is supported on this Master Domain */
    if((FRTSYN_SYNC_CRC_SUPPORTED == pMasterCfg->txCrcSecured)
        || (FRTSYN_OFS_CRC_SUPPORTED == pMasterCfg->txCrcSecured))
    {
      /* Prepare data for CRC calculation, copy needed parts from the Message to be transmitted */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00035,1 */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00036,1 */
      TS_MemCpy(&CRCData[0], &pMasterData->msgToTransmit[2], FRTSYN_BYTES_NEEDED_FROM_MSG_LENGTH);
      /* Add DataIDLList*/
      CRCData[FRTSYN_DATA_ID_LIST_BYTE] = FRTSYN_GET_DIDLISTVAL(pMasterCfg->DIDIdx, pMasterData->seqCount);
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00031,1 */
      pMasterData->msgToTransmit[FRTSYN_CRC_BYTE] = Crc_CalculateCRC8H2F(&CRCData[0], FRTSYN_CRC_DATA_LENGTH, FRTSYN_CRC_STARTVAL, (boolean)TRUE);
    }
    else /* CRC is not supported on this Time Domain */
#endif /* FRTSYN_CRC_CHECK_USED == STD_ON */
    {
      pMasterData->msgToTransmit[FRTSYN_USER_DATA_BYTE_2] = userData.userByte2;
    }
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00021,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00025,1 */
    pMasterData->msgToTransmit[FRTSYN_MESSAGE_TYPE_BYTE] = pMasterCfg->txCrcSecured;

    /* SWS_FrTSyn_00037 (1) and (4) - Calculate SC */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00030,1 */
    pMasterData->seqCount = FRTSYN_GET_NEW_SEQ_COUNTER(pMasterData->seqCount);

    /* Set the length as the message payload */
    TxDataPtr.SduLength = FRTSYN_MESSAGE_PAYLOAD;
    /* Copy data from the message to the ptr */
    TS_MemCpy(&TxDataPtr.SduDataPtr[0], &pMasterData->msgToTransmit[0], FRTSYN_MESSAGE_PAYLOAD);

    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00070.INTERRUPTION.TriggerTransmit,1 */
    TS_AtomicAssign8(pMasterData->assocIdx, TimeDomainIdx);
    /* Transmit Data to FrIf module */
    if(E_OK == FrIf_Transmit(pMasterCfg->txPduId, &TxDataPtr))
    {

#if ((FRTSYN_TXTYPE_IMMEDIATE_USED == STD_ON) || (FRTSYN_TXTYPE_BOTH_USED == STD_ON))
      /* Reload the timers */
      switch(pMasterCfg->txCondFuncIdx)
      {
#if (FRTSYN_TXTYPE_IMMEDIATE_USED == STD_ON)
        case FRTSYN_TXTYPE_IMMEDIATE:
          /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00085,1 */
          pMasterData->dbncCntr = pMasterCfg->cdbncCntr;
          break;
#endif /* FRTSYN_TXTYPE_IMMEDIATE_USED == STD_ON */
#if (FRTSYN_TXTYPE_BOTH_USED == STD_ON)
        case FRTSYN_TXTYPE_BOTH:
          /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00085,1 */
          pMasterData->dbncCntr = pMasterCfg->cdbncCntr;
          if(FRTSYN_IS_IMMEDIATE_TX(pMasterData))
          {
            /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00089,1 */
            /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00090,1 */
            pMasterData->txPeriod = pMasterCfg->cresumeCntr;
          }
          break;
#endif /* FRTSYN_TXTYPE_BOTH_USED == STD_ON */
        /* CHECK: NOPARSE */
        default:
          /* No other possible transmission type */
          break;
        /* CHECK: PARSE */
      }
#endif /* ((FRTSYN_TXTYPE_IMMEDIATE_USED == STD_ON) || (FRTSYN_TXTYPE_BOTH_USED == STD_ON)) */

#if(FRTSYN_TIME_VALIDATION_ENABLED == STD_ON)
      /* !LINKSTO FrTSyn.ASR19-11.SWS_FrTSyn_00100.Immediate,1 */
      if(FRTSYN_TIME_VALIDATION_ENABLED_ON_DOMAIN(pMasterCfg))
      {
        if(FRTSYN_IS_FRIF_IMMEDIATE(pMasterCfg))
        {
          (void)StbM_FrSetMasterTimingData(pMasterCfg->syncTimeBaseId, &measurementData);
        }
        else
        {
          SchM_Enter_FrTSyn_SCHM_FRTSYN_EXCLUSIVE_AREA_0();

          FRTSYN_ASSIGN_TIME_DATA(*pTimeValidationData, measurementData);

          SchM_Exit_FrTSyn_SCHM_FRTSYN_EXCLUSIVE_AREA_0();
        }
      }
#endif /* FRTSYN_TIME_VALIDATION_ENABLED == STD_ON */

    }
    else
    {
      TS_AtomicAssign8(pMasterData->assocIdx, FRTSYN_RESERVED_TT_DONE);
    }
  }

#if (FRTSYN_TXTYPE_BOTH_USED == STD_ON)
    /* Cyclic message resume time expired or normal periodic TX, reload for periodic transmission */
  if (!FRTSYN_IS_IMMEDIATE_TX(pMasterData))
#endif
  {
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00019.Periodic.TxPeriodReload,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00023.Periodic.TxPeriodReload,1 */
    pMasterData->txPeriod = pMasterCfg->txPeriod;
  }


#if (FRTSYN_TXTYPE_BOTH_USED == STD_ON)
  /* Clear the current transmission type */
  FRTSYN_CLEAR_ONGOING_TXTYPE(pMasterData);
#endif
}

#if (FRTSYN_TXTYPE_PERIODIC_USED == STD_ON)
/* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00019.Periodic.TxPeriodEnabled,1 */
/* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00023.Periodic.TxPeriodEnabled,1 */
STATIC FUNC(boolean, FRTSYN_CODE) FrTSyn_TxCondFunc_Periodic
(
  P2VAR(FrTSyn_MasterType, AUTOMATIC, AUTOMATIC) pMasterData,
  P2CONST(FrTSyn_MasterTimeDomainConfigType, AUTOMATIC, AUTOMATIC) pMasterCfg
)
{
  boolean retVal = FALSE;

  /* Decrease Main Function counter */
  if(0U < pMasterData->txPeriod)
  {
    pMasterData->txPeriod--;
  }

  /* It could be 0 after init, without it being decremented */
  if(0U == pMasterData->txPeriod)
  {
    if(TRUE == FrTSyn_IsTimeBaseSet(pMasterData, pMasterCfg))
    {
      retVal = TRUE;
    }
  }

  return retVal;
}
#endif /* FRTSYN_TXTYPE_PERIODIC_USED == STD_ON */

#if (FRTSYN_TXTYPE_IMMEDIATE_USED == STD_ON)
/* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00019.Periodic.TxPeriodDisabled,1 */
/* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00023.Periodic.TxPeriodDisabled,1 */
STATIC FUNC(boolean, FRTSYN_CODE) FrTSyn_TxCondFunc_Immediate
(
  P2VAR(FrTSyn_MasterType, AUTOMATIC, AUTOMATIC) pMasterData,
  P2CONST(FrTSyn_MasterTimeDomainConfigType, AUTOMATIC, AUTOMATIC) pMasterCfg
)
{
  boolean retVal = FALSE;

  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00084,1 */
  if(0U < pMasterData->dbncCntr)
  {
    /* Decrease debounce counter */
    pMasterData->dbncCntr--;
  }

  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00086,1 */
  if(0U == pMasterData->dbncCntr)
  {
    const uint8 timeBaseUpdCntrPrev = pMasterData->timebaseUpdCntr;

    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00087,1 */
    /* Has to be checked in each cycle while considering the debounce timer  */
    TS_AtomicAssign8(pMasterData->timebaseUpdCntr, StbM_GetTimeBaseUpdateCounter(pMasterCfg->syncTimeBaseId));

    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00088,1 */
    /* Deviation MISRAC2012-1 */
    if((timeBaseUpdCntrPrev != pMasterData->timebaseUpdCntr)
       && (TRUE == FrTSyn_IsTimeBaseSet(pMasterData, pMasterCfg)))
    {
      retVal = TRUE;
    }
  }

  return retVal;
}
#endif /* FRTSYN_TXTYPE_IMMEDIATE_USED == STD_ON */

#if (FRTSYN_TXTYPE_BOTH_USED == STD_ON)
/* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00019.Periodic.TxPeriodEnabled,1 */
/* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00023.Periodic.TxPeriodEnabled,1 */
STATIC FUNC(boolean, FRTSYN_CODE) FrTSyn_TxCondFunc_Both
(
  P2VAR(FrTSyn_MasterType, AUTOMATIC, AUTOMATIC) pMasterData,
  P2CONST(FrTSyn_MasterTimeDomainConfigType, AUTOMATIC, AUTOMATIC) pMasterCfg
)
{
  boolean retVal = FALSE;

  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00084,1 */
  if(0U < pMasterData->dbncCntr)
  {
    /* Decrease debounce counter */
    pMasterData->dbncCntr--;
  }

  /* Decrease counter for next periodic transmission */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00090,1 */
  if(0U < pMasterData->txPeriod)
  {
    pMasterData->txPeriod--;
  }

  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00086,1 */
  if(0U == pMasterData->dbncCntr)
  {
    const uint8 timeBaseUpdCntrPrev = pMasterData->timebaseUpdCntr;

    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00087,1 */
    /* Has to be checked in each cycle while considering the debounce timer  */
    TS_AtomicAssign16(pMasterData->timebaseUpdCntr, StbM_GetTimeBaseUpdateCounter(pMasterCfg->syncTimeBaseId));

    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00088,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00019.Periodic.GlobalTimeBaseBit,1 */
    /* Deviation MISRAC2012-1 */
    if((timeBaseUpdCntrPrev != pMasterData->timebaseUpdCntr)
       && (TRUE == FrTSyn_IsTimeBaseSet(pMasterData, pMasterCfg)))
    {
      retVal = TRUE;
      FRTSYN_SET_IMMEDIATE_TX(pMasterData);
    }

    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00089,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00091,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00093,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00019.Periodic.CyclicMsgResumeCounter,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00023.Periodic.CyclicMsgResumeCounter,1 */
    if(0U == pMasterData->txPeriod)
    {
      /* Check if the message has to be sent this Main Function */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00019.Periodic.TxPeriodEnabled,1 */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00023.Periodic.TxPeriodEnabled,1 */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00019.Periodic.GlobalTimeBaseBit,1 */
      if(TRUE == FrTSyn_IsTimeBaseSet(pMasterData, pMasterCfg))
      {
        retVal = TRUE;
        /* The txProperties member is not modified in this case,
         * bit 1 remains unset, meaning periodic transmission
         */
      }
    }
  }

  return retVal;
}
#endif /* FRTSYN_TXTYPE_BOTH_USED == STD_ON */

STATIC FUNC(boolean, FRTSYN_CODE) FrTSyn_IsTimeBaseSet
(
  P2VAR(FrTSyn_MasterType, AUTOMATIC, AUTOMATIC) pMasterData,
  P2CONST(FrTSyn_MasterTimeDomainConfigType, AUTOMATIC, AUTOMATIC) pMasterCfg
)
{
  boolean retVal = FALSE;

  if(FRTSYN_IS_TIMEBASE_SET(pMasterData))
  {
    retVal = TRUE;
  }
  else
  {
    StbM_TimeBaseStatusType timeBaseStatus;
    StbM_TimeBaseStatusType timeBaseOfsStatus;

    if(E_OK == StbM_GetTimeBaseStatus(pMasterCfg->syncTimeBaseId, &timeBaseStatus, &timeBaseOfsStatus))
    {
      const StbM_TimeBaseStatusType status = (pMasterCfg->timeDomainId < 16U) ? timeBaseStatus : timeBaseOfsStatus;

      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00023.Periodic.GlobalTimeBaseBit,1 */
      if(STBM_GLOBAL_TIME_BASE_FLAG == (status & STBM_GLOBAL_TIME_BASE_FLAG))
      {
        FRTSYN_STORE_TIMEBASE_SET(pMasterData);
        retVal = TRUE;
      }
    }
  }

  return retVal;
}

#endif /* (FRTSYN_IS_MASTER_FUNC_USED == STD_ON) */

STATIC FUNC(void, FRTSYN_CODE) FrTSyn_AddToTimeStamp
(
  P2VAR(StbM_TimeStampType, AUTOMATIC, AUTOMATIC) timeStampPtr,
  uint32 nanosecondsToAdd
)
{
  uint32 nanosecondsSum;
  /* compute the seconds to add */
  const uint32 secondsFromNs = (nanosecondsToAdd / FRTSYN_NS_PER_SEC);
  /* carry to add it to the high parts of seconds */
  uint8 secondsCarry = 0U;
  uint8 nanosecondsCarry;

  DBG_FRTSYN_ADDTOTIMESTAMP_ENTRY(timeStampPtr, nanosecondsToAdd);

  /* ignore overflow */
  nanosecondsSum = timeStampPtr->nanoseconds + (nanosecondsToAdd - (secondsFromNs * FRTSYN_NS_PER_SEC));

  /* store carry to add it to the seconds later */
  nanosecondsCarry = ((uint8) (nanosecondsSum / FRTSYN_NS_PER_SEC));

  /* detect if sum of seconds causes an overflow */
  if ((0xffffffffU - timeStampPtr->seconds) < secondsFromNs)
  {
    secondsCarry = 1U;
  }

  /* detect if nanoseconds overflow causes an overflow of the seconds */
  if (((timeStampPtr->seconds + secondsFromNs) == 0xffffffffU) && (nanosecondsCarry > 0U))
  {
    secondsCarry = 1U;
  }

  /* Don't consider overflow of secondsHi, this might happen in thousand of years. */
  /* write back nanosecond part */
  timeStampPtr->nanoseconds = nanosecondsSum - ((uint32) nanosecondsCarry * FRTSYN_NS_PER_SEC);
  /* write back (low-)second part considering the Nanosecond carry */
  timeStampPtr->seconds += (secondsFromNs + nanosecondsCarry);
  /* write back (high-)second part */
  timeStampPtr->secondsHi += secondsCarry;

  DBG_FRTSYN_ADDTOTIMESTAMP_EXIT(timeStampPtr, nanosecondsToAdd);
}

#if(FRTSYN_IS_SLAVE_FUNC_USED == STD_ON)

STATIC FUNC(void, FRTSYN_CODE) FrTSyn_ProcessSlave
(
  uint8 TimeDomainIdx
)
{
  CONSTP2CONST(FrTSyn_SlaveTimeDomainConfigType, AUTOMATIC, FRTSYN_APPL_CONST) pSlaveCfg = &FrTSyn_SlaveConfig[TimeDomainIdx];
  CONSTP2VAR(FrTSyn_SlaveType, AUTOMATIC, FRTSYN_APPL_DATA) pSlaveData = &FrTSyn_Slave[TimeDomainIdx];
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC) rcvdMsg = pSlaveData->receivedMsg;

  /* See if message can be processed */
  boolean messageCanBeProcessed = FALSE;

  /*  SWS_FrTSyn_00056 (1) -  Check that the Message Type is a Valid one corresponding to the configured value */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00038,1 */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00039,1 */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00040,1 */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00081,1 */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00042,1 */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00043,1 */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00044,1 */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00082.OFS_CRC_SECURED,1 */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00082.OFS_NOT_CRC_SECURED,1 */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00056,1 */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00014,1 */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00015,1 */
  if (FRTSYN_IS_MESSAGE_TYPE_VALID(rcvdMsg[FRTSYN_MESSAGE_TYPE_BYTE], TimeDomainIdx) != 0U)
  {
    const uint8 receivedSequenceCounter = rcvdMsg[FRTSYN_TIME_DOMAIN_SEQ_COUNTER_BYTE] & FRTSYN_SEQ_COUNTER_MASK;
    /* This represents the difference between the current seqCounter and the one before, so it can be checked later(has to be less than the scjumpWidth) */
    const uint8 deltaSequenceCounter = (receivedSequenceCounter >= pSlaveData->seqCount) ?
      (uint8)(receivedSequenceCounter - pSlaveData->seqCount) :
      (uint8)((receivedSequenceCounter + FRTSYN_MAX_SEQ_COUNTER + 1U)
       - pSlaveData->seqCount);
    const boolean isSync = (pSlaveCfg->timeDomainId < 16U) ? TRUE : FALSE;

    uint8 userDataLength = 0U;
    /* Where User Data will be stored */
    StbM_TimeStampType receivedTimeStamp = {0U, 0U, 0U, 0U};
    StbM_UserDataType userData;

    /*  SWS_FrTSyn_00056 (2) -  Validate sequence counter - discard message otherwise (see RFC 71057) */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00048,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00056,1 */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00049.Startup,1 */
    if ((FRTSYN_SEQ_COUNTER_NOCHECK == pSlaveData->seqCount) || ((deltaSequenceCounter > 0U) && (deltaSequenceCounter <= pSlaveCfg->scjumpWidth)))
    {
      messageCanBeProcessed = TRUE;
    }
    else
    {
      StbM_TimeBaseStatusType timeBaseStatus;
      StbM_TimeBaseStatusType timeBaseOfsStatus;
      P2VAR(StbM_TimeBaseStatusType, AUTOMATIC, AUTOMATIC) status = isSync ? &timeBaseStatus : &timeBaseOfsStatus;

      /* Always returns E_OK */
      (void)StbM_GetTimeBaseStatus(pSlaveCfg->syncTimeBaseId, &timeBaseStatus, &timeBaseOfsStatus);

      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00049.Timeout.DeltaSC.GreaterThan0.NoSCCheck,1 */
      /* Check if TIMEOUT bit was set in StbM and the difference between the previous SC and the current one is greater than 0 => no SC check */
      if((STBM_TIMEOUT_FLAG == (*status & STBM_TIMEOUT_FLAG)) && (deltaSequenceCounter > 0U))
      {
        messageCanBeProcessed = TRUE;
      }
      else
      {
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00049.Timeout.DeltaSC.EqualsZero.MessageNotAccepted,1 */
        /* If TIMEOUT bit was set in StbM and the difference between the previous SC and the current one is equal to 0
            => the frame will be ignored and FrTSyn_SendToStbM() will not be called */
      }
    }

    /* Store the sequence counter either way */
    TS_AtomicAssign8(pSlaveData->seqCount, receivedSequenceCounter);

    if(messageCanBeProcessed)
    {
      /* SWS_FrTSyn_00056 (6) - Check that CRC(including DataID) matches if we have in our configuration CRC_VALIDATED */
      const uint8 crcValidatedType = isSync ? FRTSYN_SYNC_CRC_VALIDATED : FRTSYN_OFS_CRC_VALIDATED;

#if(FRTSYN_CRC_CHECK_USED == STD_ON)
      /* Check if the Time Domain uses CRC_IGNORED */
      boolean crcIgnored = (FRTSYN_CRC_IGNORED == FrTSyn_SlaveConfig[TimeDomainIdx].rxCrcValidated) ? TRUE : FALSE;
      /* Store the userDataLength since it can vary */

      /* Received message */
      if((crcValidatedType == rcvdMsg[FRTSYN_MESSAGE_TYPE_BYTE]) && (FALSE == crcIgnored))
      {
        /* Store received CRC for later comparison */
        const uint8 receivedCRC = rcvdMsg[FRTSYN_CRC_BYTE];
        /* Prepare data for CRC check, copy needed parts from Received Message */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00054,1 */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00055,1 */
        TS_MemCpy(&CRCData[0], &rcvdMsg[2], FRTSYN_BYTES_NEEDED_FROM_MSG_LENGTH);
        /* Add DataIDLList*/
        CRCData[FRTSYN_DATA_ID_LIST_BYTE] = FRTSYN_GET_DIDLISTVAL(pSlaveCfg->DIDIdx, pSlaveData->seqCount);
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00050,1 */
        if(receivedCRC != Crc_CalculateCRC8H2F(&CRCData[0], FRTSYN_CRC_DATA_LENGTH, FRTSYN_CRC_STARTVAL, (boolean)TRUE))
        {
          messageCanBeProcessed = FALSE;
        }
      }
      else
#endif /* FRTSYN_CRC_CHECK_USED == STD_ON */
      {
        /* Store the User Data Byte2, increase userData length */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00011,1 */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00013,1 */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00010,1 */
        if((crcValidatedType != rcvdMsg[FRTSYN_MESSAGE_TYPE_BYTE]))
        {
          userData.userByte2 = rcvdMsg[FRTSYN_USER_DATA_BYTE_2];
          userDataLength++;
        }
      }
    }

    /* Same check as above, see if it's still valid */
    if(messageCanBeProcessed)
    {
      /* SWS_FrTSyn_00046 (1) - Extract time stamp - Byte Order is Big Endian*/
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00046,1 */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00007,1 */
      receivedTimeStamp.nanoseconds =
        ((uint32) ((uint32) rcvdMsg[FRTSYN_NSECONDS_BYTE_0]) << 24) |
        ((uint32) ((uint32) rcvdMsg[FRTSYN_NSECONDS_BYTE_1]) << 16) |
        ((uint32) ((uint32) rcvdMsg[FRTSYN_NSECONDS_BYTE_2]) << 8) |
                   (uint32) rcvdMsg[FRTSYN_NSECONDS_BYTE_3];

      /* SWS_FrTSyn_00056 (5) - Time stamp matches the defined range of StbM_TimeStampType.nanoseconds */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00056,1 */
      if(FRTSYN_NANOSECONDS_MAX_VALUE > receivedTimeStamp.nanoseconds)
      {
        /* Get received SGW */
        const uint8 receivedSGW = ((rcvdMsg[FRTSYN_FCNT_AND_SGW_BYTE] & (uint8)FRTSYN_SGW_MASK) > 0U) ? 1U : 0U;
        /* Get received FCNT */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00028,1 */
        const uint8 receivedCycle = ((uint8) (rcvdMsg[FRTSYN_FCNT_AND_SGW_BYTE] & FRTSYN_CYCLE_COUNTER_MASK)) >> FRTSYN_CYCLE_COUNTER_SHIFT;

        /* SWS_FrTSyn_00046 (1) - Extract time stamp - Byte Order is Big Endian*/
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00046,1 */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00007,1 */
        receivedTimeStamp.secondsHi = (uint16)
          ((uint32) ((uint32) (rcvdMsg[FRTSYN_SECONDS_BYTE_0]) << 8U) |
                      (uint32) rcvdMsg[FRTSYN_SECONDS_BYTE_1]);

        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00047,1 */
        receivedTimeStamp.seconds =
            ((uint32) ((uint32) rcvdMsg[FRTSYN_SECONDS_BYTE_2]) << 24U) |
            ((uint32) ((uint32) rcvdMsg[FRTSYN_SECONDS_BYTE_3]) << 16U) |
            ((uint32) ((uint32) rcvdMsg[FRTSYN_SECONDS_BYTE_4]) << 8U) |
                       (uint32) rcvdMsg[FRTSYN_SECONDS_BYTE_5];

        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00094,1 */
        if (0U != receivedSGW)
        {
          receivedTimeStamp.timeBaseStatus = STBM_SYNC_TO_GATEWAY_FLAG;
        }

        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00057,1 */

        /* Store User Data Byte0 and Byte1 */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00011,1 */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00013,1 */
        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00010,1 */
        userDataLength += 2U;
        userData.userByte0 = rcvdMsg[FRTSYN_USER_DATA_BYTE_0];
        userData.userByte1 = rcvdMsg[FRTSYN_USER_DATA_BYTE_1];
        userData.userDataLength = userDataLength;


        FrTSyn_SendToStbM(&receivedTimeStamp, &userData, TimeDomainIdx, receivedCycle, isSync);

      } /* nanoseconds within range */
      else
      {
        ; /* Nothing to do, invalid message */
      }
    }
  } /* message type is valid */
}

STATIC FUNC(void, FRTSYN_CODE) FrTSyn_SendToStbM
(
  P2VAR(StbM_TimeStampType, AUTOMATIC, AUTOMATIC) receivedTimeStampPtr,
  P2VAR(StbM_UserDataType, AUTOMATIC, AUTOMATIC) userDataPtr,
  uint8 TimeDomainIdx,
  uint8 receivedCycle,
  boolean isSync
)
{
  CONSTP2CONST(FrTSyn_SlaveTimeDomainConfigType, AUTOMATIC, FRTSYN_APPL_CONST) pSlaveCfg = &FrTSyn_SlaveConfig[TimeDomainIdx];
  CONSTP2VAR(FrTSyn_SlaveType, AUTOMATIC, FRTSYN_APPL_DATA) pSlaveData = &FrTSyn_Slave[TimeDomainIdx];
  StbM_VirtualLocalTimeType virtLocalTime = {0U, 0U};
  /* Measure Data */
  /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00092,1 */
  const StbM_MeasurementType measureData = {0U};

  DBG_FRTSYN_SENDTOSTBM_ENTRY(receivedTimeStampPtr, userDataPtr, TimeDomainIdx, receivedCycle, isSync);

  /* check if we are dealing with SYNC or OFFSET time domain */
  if(isSync)
  {
    /* FrIf returned state */
    FrIf_StateType state;
    /* Only call FrIf_GetGlobalTime() if FrIf_GetState() returns FRIF_STATE_ONLINE */
    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00078,1 */
    if ((E_OK == FrIf_GetState(pSlaveCfg->clstId, &state)) && (FRIF_STATE_ONLINE == state))
    {
      Std_ReturnType frifRet;
      Std_ReturnType stbmRet;
      uint16 currentMacroTicks;
      uint8 currentCycle;

      SchM_Enter_FrTSyn_SCHM_FRTSYN_EXCLUSIVE_AREA_1();

      /* SWS_FrTSyn_00046 (3) - get currentCycle and currentMacroticks from FlexRay */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00046,1 */
      frifRet = FrIf_GetGlobalTime(pSlaveCfg->ctrlId, &currentCycle, &currentMacroTicks);
      stbmRet = StbM_GetCurrentVirtualLocalTime(pSlaveCfg->syncTimeBaseId, &virtLocalTime);

      SchM_Exit_FrTSyn_SCHM_FRTSYN_EXCLUSIVE_AREA_1();

      if((E_OK == frifRet) && (E_OK == stbmRet))
      {
        const uint32 timeDiff =  (pSlaveData->cycleLength * currentCycle) + (pSlaveData->macrotickDuration * (uint32)currentMacroTicks);

#if (FRTSYN_TIME_VALIDATION_ENABLED == STD_ON)
        StbM_TimeStampShortType receivedTimeStamp;

        receivedTimeStamp.nanoseconds = receivedTimeStampPtr->nanoseconds;
        receivedTimeStamp.seconds = receivedTimeStampPtr->seconds;
#endif /* FRTSYN_TIME_VALIDATION_ENABLED == STD_ON */

        if(receivedCycle <= currentCycle)
        {
          /* Subtract maximum cycle length (calculated in init) and difference */
          FrTSyn_SubtractFromTimeStamp(receivedTimeStampPtr, pSlaveData->maxCycleLength - timeDiff);
        }
        else
        {
          FrTSyn_AddToTimeStamp(receivedTimeStampPtr, timeDiff);
        }

        /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00041,1 */
        (void) StbM_BusSetGlobalTime(pSlaveCfg->syncTimeBaseId, receivedTimeStampPtr, userDataPtr, &measureData, &virtLocalTime);

#if (FRTSYN_TIME_VALIDATION_ENABLED == STD_ON)
        if(FRTSYN_TIME_VALIDATION_ENABLED_ON_DOMAIN(pSlaveCfg))
        {
          StbM_FrTimeSlaveMeasurementType slaveMeasurement;

          /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00099.seqCounter,1 */
          slaveMeasurement.sequenceCounter = pSlaveData->seqCount;
          /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00099.currentCycle,1 */
          slaveMeasurement.currentCycle = currentCycle;
          /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00099.currentMacroticks,1 */
          slaveMeasurement.CurrentMacroticks = currentMacroTicks;
          /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00099.FCNT,1 */
          slaveMeasurement.FCNT = receivedCycle;
          /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00099.CycleLength,1 */
          slaveMeasurement.cycleLength = pSlaveData->cycleLength;
          /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00099.MacrotickDuration,1 */
          slaveMeasurement.macrotickDuration = pSlaveData->macrotickDuration;
          /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00099.T1VLT,1 */
          slaveMeasurement.syncIngressTimestamp.nanosecondsLo = virtLocalTime.nanosecondsLo;
          slaveMeasurement.syncIngressTimestamp.nanosecondsHi = virtLocalTime.nanosecondsHi;
          /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00099.T0,1 */
          slaveMeasurement.preciseOriginTimestampSec.seconds = receivedTimeStamp.seconds;
          slaveMeasurement.preciseOriginTimestampSec.nanoseconds = receivedTimeStamp.nanoseconds;
          /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00099.referenceLocalTimestamp,1 */
          slaveMeasurement.referenceLocalTimestamp.nanosecondsLo = 0U;
          slaveMeasurement.referenceLocalTimestamp.nanosecondsHi = 0U;
          /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00099.referenceGlobalTimestamp,1 */
          slaveMeasurement.referenceGlobalTimestampSec.seconds = 0U;
          slaveMeasurement.referenceGlobalTimestampSec.nanoseconds = 0U;
          /* !LINKSTO FrTSyn.ASR20-11.SWS_FrTSyn_00099.segID,1 */
          slaveMeasurement.segmentId = pSlaveCfg->segmentId;

          /* !LINKSTO FrTSyn.ASR19-11.SWS_FrTSyn_00098.Reception,1 */
          /* !LINKSTO FrTSyn.ASR19-11.SWS_FrTSyn_00098.BusSetGlobalTime,1 */
          (void)StbM_FrSetSlaveTimingData(pSlaveCfg->syncTimeBaseId, &slaveMeasurement);
        }
#endif /* FRTSYN_TIME_VALIDATION_ENABLED == STD_ON */
      }
    } /* FrIf_GetState() ==  E_OK */
  } /* SYNC */
  else
  {
    Std_ReturnType stbmRet;

    SchM_Enter_FrTSyn_SCHM_FRTSYN_EXCLUSIVE_AREA_1();

    stbmRet = StbM_GetCurrentVirtualLocalTime(pSlaveCfg->syncTimeBaseId, &virtLocalTime);

    SchM_Exit_FrTSyn_SCHM_FRTSYN_EXCLUSIVE_AREA_1();

    /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00047,1 */
    if(E_OK == stbmRet)
    {
      /* Forward global time to StbM */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00045,1 */
      /* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00041,1 */
      (void) StbM_BusSetGlobalTime(pSlaveCfg->syncTimeBaseId, receivedTimeStampPtr, userDataPtr, &measureData, &virtLocalTime);
    }
  } /* OFS */

  DBG_FRTSYN_SENDTOSTBM_EXIT(receivedTimeStampPtr, userDataPtr, TimeDomainIdx, receivedCycle, isSync);
}


STATIC FUNC(void, FRTSYN_CODE) FrTSyn_SubtractFromTimeStamp
(
  P2VAR(StbM_TimeStampType, AUTOMATIC, AUTOMATIC) timeStampPtr,
  uint32 nanosecondsToSubtract
)
{
  /* Difference between the nanoseconds part of minuend and Subtrahend */
  uint32 nanosecondsDiff;
  /* Difference between the seconds part of minuend and Subtrahend */
  uint32 secondsDiff;
  /* store seconds carry to subtract it from the seconds (high) later */
  uint8 secondsCarry;
  /* store nanoseconds carry to subtract it from the seconds later */
  uint8 nanosecondsCarry = ((uint8) (nanosecondsToSubtract / FRTSYN_NS_PER_SEC));

  DBG_FRTSYN_SUBSTRACTFROMTIMESTAMP_ENTRY(timeStampPtr, nanosecondsToSubtract);

  /* compute nanoseconds to subtract */
  nanosecondsToSubtract = nanosecondsToSubtract - ((uint32)nanosecondsCarry * FRTSYN_NS_PER_SEC);

  /* Part 1: calculation for nanoseconds part of timestamp */

  /* We need some special arithmetic. Normal underflow is not sufficient.
   * If the subtrahend is greater than the minuend, calculate from a base value of 10^9 */
  if (nanosecondsToSubtract > timeStampPtr->nanoseconds)
  {
    /* Subtrahend greater - calculate with underflow */
    nanosecondsDiff = (FRTSYN_NS_PER_SEC + timeStampPtr->nanoseconds)
      - nanosecondsToSubtract;
    nanosecondsCarry += 1U;
  }
  else
  {
    nanosecondsDiff = timeStampPtr->nanoseconds - nanosecondsToSubtract;
  }

  /* Part 2: calculation for seconds (low) part of timestamp */

  /* underflow of unsigned variables is defined in C */
  secondsDiff = timeStampPtr->seconds - nanosecondsCarry;
  /* check if we had an underflow */
  secondsCarry = (nanosecondsCarry > timeStampPtr->seconds) ? 1U : 0U;

  /* Part 3: calculation for seconds (high) part of timestamp (including error check) */

  /* write back nanosecond part */
  timeStampPtr->nanoseconds = nanosecondsDiff;
  /* write back (low-)second part */
  timeStampPtr->seconds = secondsDiff;
  /* write back (high-)second part */
  timeStampPtr->secondsHi -= secondsCarry;

  DBG_FRTSYN_SUBSTRACTFROMTIMESTAMP_EXIT(timeStampPtr, nanosecondsToSubtract);
}
#endif /* (FRTSYN_IS_SLAVE_FUNC_USED == STD_ON) */

#define FRTSYN_STOP_SEC_CODE
#include <FrTSyn_MemMap.h>
