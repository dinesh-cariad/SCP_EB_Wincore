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

#ifndef SOMEIPTP_TYPES_H
#define SOMEIPTP_TYPES_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>            /* Specific standard types */
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/** \brief defines to define SomeIpTp states of TxNSdu. */

/** \brief Macro to define that Channel is free */
#if (defined SOMEIPTP_IDLE)
#error SOMEIPTP_IDLE already defined!
#endif /* #if (defined SOMEIPTP_IDLE)*/
#define SOMEIPTP_IDLE                          1U

/** \brief Macro to define that Transmit request accepted and Tp first meesage is about to be processed */
#if (defined SOMEIPTP_FIRST_TO_BE_TRANSMITTED)
#error SOMEIPTP_FIRST_TO_BE_TRANSMITTED already defined!
#endif /* #if (defined SOMEIPTP_FIRST_TO_BE_TRANSMITTED)*/
#define SOMEIPTP_FIRST_TO_BE_TRANSMITTED       2U

/** \brief Macro to define that Channel awaits the First Segment */
#if (defined SOMEIPTP_FIRST)
#error SOMEIPTP_FIRST already defined!
#endif /* #if (defined SOMEIPTP_FIRST)*/
#define SOMEIPTP_FIRST                         3U

/** \brief Macro to define that Channel awaits the First Segment Confirmation */
#if (defined SOMEIPTP_FIRST_CONF)
#error SOMEIPTP_FIRST_CONF already defined!
#endif /* #if (defined SOMEIPTP_FIRST_CONF)*/
#define SOMEIPTP_FIRST_CONF                    4U

/** \brief Macro to define that previous segment is confirmed but awaiting the end of Tx separation time */
#if (defined SOMEIPTP_NEXT_TO_BE_CONSECUTIVE)
#error SOMEIPTP_NEXT_TO_BE_CONSECUTIVE already defined!
#endif /* #if (defined SOMEIPTP_NEXT_TO_BE_CONSECUTIVE)*/
#define SOMEIPTP_NEXT_TO_BE_CONSECUTIVE        5U

/** \brief Macro to define that Channel awaits a consecutive segment */
#if (defined SOMEIPTP_CONSECUTIVE)
#error SOMEIPTP_CONSECUTIVE already defined!
#endif /* #if (defined SOMEIPTP_CONSECUTIVE)*/
#define SOMEIPTP_CONSECUTIVE                   6U

/** \brief Macro to define that Channel awaits a consecutive segment Confirmation */
#if (defined SOMEIPTP_CONSECUTIVE_CONF)
#error SOMEIPTP_CONSECUTIVE_CONF already defined!
#endif /* #if (defined SOMEIPTP_CONSECUTIVE_CONF)*/
#define SOMEIPTP_CONSECUTIVE_CONF              7U

/** \brief Macro to define that previous segment is confirmed but awaiting the end of Tx separation time */
#if (defined SOMEIPTP_NEXT_TO_BE_LAST)
#error SOMEIPTP_NEXT_TO_BE_LAST already defined!
#endif /* #if (defined SOMEIPTP_NEXT_TO_BE_LAST)*/
#define SOMEIPTP_NEXT_TO_BE_LAST               8U

/** \brief Macro to define that Channel awaits the last segment */
#if (defined SOMEIPTP_LAST)
#error SOMEIPTP_LAST already defined!
#endif /* #if (defined SOMEIPTP_LAST)*/
#define SOMEIPTP_LAST                          9U

/** \brief Macro to define that Channel awaits the last segment Confirmation */
#if (defined SOMEIPTP_LAST_CONF)
#error SOMEIPTP_LAST_CONF already defined!
#endif /* #if (defined SOMEIPTP_LAST_CONF)*/
#define SOMEIPTP_LAST_CONF                     10U

/** \brief Macro to define that Transmit request accepted and Single frame meesage is about to be processed */
#if (defined SOMEIPTP_SINGLE_TO_BE_TRANSMITTED)
#error SOMEIPTP_SINGLE_TO_BE_TRANSMITTED already defined!
#endif /* #if (defined SOMEIPTP_SINGLE_TO_BE_TRANSMITTED)*/
#define SOMEIPTP_SINGLE_TO_BE_TRANSMITTED      11U

/** \brief Macro to define that the data can be transmitted directly */
#if (defined SOMEIPTP_SINGLE)
#error SOMEIPTP_SINGLE already defined!
#endif /* #if (defined SOMEIPTP_SINGLE)*/
#define SOMEIPTP_SINGLE                        12U

/** \brief Macro to define that the channel awaits the Single segment confirmation */
#if (defined SOMEIPTP_SINGLE_CONF)
#error SOMEIPTP_SINGLE_CONF already defined!
#endif /* #if (defined SOMEIPTP_SINGLE_CONF)*/
#define SOMEIPTP_SINGLE_CONF                   13U

/** \brief Macro to define that Channel awaits the First Segment */
#if (defined SOMEIPTP_RX_STARTED)
#error SOMEIPTP_RX_STARTED already defined!
#endif /* #if (defined SOMEIPTP_RX_STARTED)*/
#define SOMEIPTP_RX_STARTED                    2U

/** \brief Macro to define that there an error in the chunk received */
#if (defined SOMEIPTP_NO_RX_ERROR)
#error SOMEIPTP_NO_RX_ERROR already defined!
#endif /* #if (defined SOMEIPTP_NO_RX_ERROR)*/
#define SOMEIPTP_NO_RX_ERROR           0x01U


/** \brief Type to define NSDU states */
typedef VAR(uint8, TYPEDEF) SomeIpTp_StateType;

/** \brief Type to define Runtime Errors at Rx */
typedef VAR(uint8, TYPEDEF) SomeIpTp_ErrorType;

/** \brief Type to define Retry states for CopyTxData */
typedef VAR(TpDataStateType, TYPEDEF) SomeIpTp_RetryStateType;

/** \brief Type to define Channel configured parameters. */
/* !LINKSTO SomeIpTp.dsn.PrecompileTimeConfig,1 */
typedef struct
{
  CONST(uint16, TYPEDEF) NPduSeparationTimeMFC;
  CONST(uint16, TYPEDEF) RxTimeoutTimeMFC;
  CONST(uint16, TYPEDEF) TxConfTimeoutTimeMFC;
}SomeIpTp_ChannelConfigType;

/** \brief Type to define TxNSdu configured parameters. */
/* !LINKSTO SomeIpTp.dsn.PrecompileTimeConfig,1 */
typedef struct
{
#if(TS_SIZE_PduLengthType == 2U) /* PduLengthType = UINT16 */
  CONSTP2VAR(uint8, AUTOMATIC, TYPEDEF) MetaData; /* pointer to a local buffer for Metadata */
  CONST(PduLengthType, TYPEDEF) MaxNpduLength; /* Configured Npdu length */
#elif(TS_SIZE_PduLengthType == 4U) /* PduLengthType = UINT32 */
  CONST(PduLengthType, TYPEDEF) MaxNpduLength; /* Configured Npdu length */
  CONSTP2VAR(uint8, AUTOMATIC, TYPEDEF) MetaData; /* pointer to a local buffer for Metadata */
#else
#error TS_SIZE_PduLengthType is not correctly defined!
#endif /* TS_SIZE_PduLengthType */
  CONST(uint16, TYPEDEF) ChannelId; /* Channel Id */
  CONST(PduIdType, TYPEDEF) NSduTxId; /* for TxConfirmation and CopyTx pathes = PduRDestPduId */
  CONST(PduIdType, TYPEDEF) NPduTxId; /* for Transmission path = PduRSrcPduId */
}SomeIpTp_TpTxNSduConfigType;

/** \brief Type to define TxNSdu State machine parameters. */
typedef struct
{
#if(TS_SIZE_PduLengthType == 2U) /* PduLengthType = UINT16 */
  VAR(SomeIpTp_RetryStateType, TYPEDEF) retryState; /* retry state of NSdu */
  VAR(PduLengthType, TYPEDEF) offset;    /* already copied bytes (multiple of 16). */
  VAR(PduLengthType, TYPEDEF) remSegments; /* Remaining Segments */
  VAR(PduLengthType, TYPEDEF) lastSegLength; /* length of last segment in bytes */
#elif(TS_SIZE_PduLengthType == 4U) /* PduLengthType = UINT32 */
  VAR(PduLengthType, TYPEDEF) offset;    /* already copied bytes (multiple of 16). */
  VAR(PduLengthType, TYPEDEF) remSegments; /* Remaining Segments */
  VAR(PduLengthType, TYPEDEF) lastSegLength; /* length of last segment in bytes */
  VAR(SomeIpTp_RetryStateType, TYPEDEF) retryState; /* retry state of NSdu */
#else
#error TS_SIZE_PduLengthType is not correctly defined!
#endif /* TS_SIZE_PduLengthType */
  VAR(uint16, TYPEDEF)  txSeparationMFC; /* TxSeparationTime in unit of Main funcion cycles */
  VAR(uint16, TYPEDEF)  txConfTimeoutMFC; /* TxConfirmationTimeout in unit of Main funcion cycles */
  VAR(SomeIpTp_StateType, TYPEDEF) ongoingState; /* state of NSdu */
}SomeIpTp_TpTxNSduType;

/** \brief Type to define RxNSdu configured parameters. */
/* !LINKSTO SomeIpTp.dsn.PrecompileTimeConfig,1 */
typedef struct
{
  CONSTP2VAR(uint8, AUTOMATIC, TYPEDEF) MetaData; /* pointer to a local buffer for Metadata */
  CONST(uint16, TYPEDEF) ChannelId; /* Channel Id */
  CONST(PduIdType, TYPEDEF) NSduRxIndId; /* for RxIndication path = PduRDestPduId */
}SomeIpTp_TpRxNSduConfigType;

/** \brief Type to define RxNSdu State machine parameters. */
typedef struct
{
  VAR(PduLengthType, TYPEDEF) expOffest; /* expected offset value in the next segment */
  VAR(PduLengthType, TYPEDEF) AvailBufLength; /* Provided from the last call of copyRxData */
  VAR(uint16, TYPEDEF)  rxTimeoutMFC; /* RxTimeout in units of Main funcion cycles */
  VAR(SomeIpTp_StateType, TYPEDEF) ongoingState; /* state of NSdu */
}SomeIpTp_TpRxNSduType;

/** \brief Defines the type of SomeIpTp configuration structure. */
/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_91002,1 */
typedef struct
{
  uint8 dummy;
}SomeIpTp_ConfigType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef SOMEIPTP_TYPES_H */
/*==================[end of file]===========================================*/
