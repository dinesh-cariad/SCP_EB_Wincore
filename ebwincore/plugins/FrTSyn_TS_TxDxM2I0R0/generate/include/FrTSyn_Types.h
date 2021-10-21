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

#ifndef FRTSYN_TYPES_H
#define FRTSYN_TYPES_H

[!INCLUDE "../include/FrTSyn_PrecompileMacros.m"!][!//
[!CODE!]
[!AUTOSPACING!]
/*==================[inclusions]============================================*/

#include <TSAutosar.h> /* Specific AUTOSAR standard types */
#include <ComStack_Types.h>
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00062,1 */
#if (defined FRTSYN_TX_OFF)
#error FRTSYN_TX_OFF already defined!
#endif /* #if (defined FRTSYN_TX_OFF)*/
#define FRTSYN_TX_OFF 0U

#if (defined FRTSYN_TX_ON)
#error FRTSYN_TX_ON already defined!
#endif /* #if (defined FRTSYN_TX_ON)*/
#define FRTSYN_TX_ON 1U

/** \brief CRC modes
 **
 ** These defines specify whether or not CRC is supported on a Time Domain
 */

#if (defined FRTSYN_SYNC_CRC_NOT_VALIDATED)
#error FRTSYN_SYNC_CRC_NOT_VALIDATED already defined!
#endif /* #if (defined FRTSYN_SYNC_CRC_NOT_VALIDATED)*/
#define FRTSYN_SYNC_CRC_NOT_VALIDATED 0x10U

#if (defined FRTSYN_OFS_CRC_NOT_VALIDATED)
#error FRTSYN_OFS_CRC_NOT_VALIDATED already defined!
#endif /* #if (defined FRTSYN_OFS_CRC_NOT_VALIDATED)*/
#define FRTSYN_OFS_CRC_NOT_VALIDATED 0x34U

#if (defined FRTSYN_SYNC_CRC_VALIDATED)
#error FRTSYN_SYNC_CRC_VALIDATED already defined!
#endif /* #if (defined FRTSYN_SYNC_CRC_VALIDATED)*/
#define FRTSYN_SYNC_CRC_VALIDATED 0x20U

#if (defined FRTSYN_OFS_CRC_VALIDATED)
#error FRTSYN_OFS_CRC_VALIDATED already defined!
#endif /* #if (defined FRTSYN_OFS_CRC_VALIDATED)*/
#define FRTSYN_OFS_CRC_VALIDATED 0x44U

#if (defined FRTSYN_CRC_IGNORED)
#error FRTSYN_CRC_IGNORED already defined!
#endif /* #if (defined FRTSYN_CRC_IGNORED)*/
#define FRTSYN_CRC_IGNORED 0x01U

#if (defined FRTSYN_SYNC_CRC_IGNORED)
#error FRTSYN_SYNC_CRC_IGNORED already defined!
#endif /* #if (defined FRTSYN_SYNC_CRC_IGNORED)*/
#define FRTSYN_SYNC_CRC_IGNORED FRTSYN_CRC_IGNORED

#if (defined FRTSYN_OFS_CRC_IGNORED)
#error FRTSYN_OFS_CRC_IGNORED already defined!
#endif /* #if (defined FRTSYN_OFS_CRC_IGNORED)*/
#define FRTSYN_OFS_CRC_IGNORED FRTSYN_CRC_IGNORED

#if (defined FRTSYN_CRC_OPTIONAL)
#error FRTSYN_CRC_OPTIONAL already defined!
#endif /* #if (defined FRTSYN_CRC_OPTIONAL)*/
#define FRTSYN_CRC_OPTIONAL 0x03U

#if (defined FRTSYN_SYNC_CRC_OPTIONAL)
#error FRTSYN_SYNC_CRC_OPTIONAL already defined!
#endif /* #if (defined FRTSYN_SYNC_CRC_OPTIONAL)*/
#define FRTSYN_SYNC_CRC_OPTIONAL FRTSYN_CRC_OPTIONAL

#if (defined FRTSYN_OFS_CRC_OPTIONAL)
#error FRTSYN_OFS_CRC_OPTIONAL already defined!
#endif /* #if (defined FRTSYN_OFS_CRC_OPTIONAL)*/
#define FRTSYN_OFS_CRC_OPTIONAL FRTSYN_CRC_OPTIONAL

#if (defined FRTSYN_SYNC_CRC_NOT_SUPPORTED)
#error FRTSYN_SYNC_CRC_NOT_SUPPORTED already defined!
#endif /* #if (defined FRTSYN_SYNC_CRC_NOT_SUPPORTED)*/
#define FRTSYN_SYNC_CRC_NOT_SUPPORTED 0x10U

#if (defined FRTSYN_OFS_CRC_NOT_SUPPORTED)
#error FRTSYN_OFS_CRC_NOT_SUPPORTED already defined!
#endif /* #if (defined FRTSYN_OFS_CRC_NOT_SUPPORTED)*/
#define FRTSYN_OFS_CRC_NOT_SUPPORTED 0x34U

#if (defined FRTSYN_SYNC_CRC_SUPPORTED)
#error FRTSYN_SYNC_CRC_SUPPORTED already defined!
#endif /* #if (defined FRTSYN_SYNC_CRC_SUPPORTED)*/
#define FRTSYN_SYNC_CRC_SUPPORTED 0x20U

#if (defined FRTSYN_OFS_CRC_SUPPORTED)
#error FRTSYN_OFS_CRC_SUPPORTED already defined!
#endif /* #if (defined FRTSYN_OFS_CRC_SUPPORTED)*/
#define FRTSYN_OFS_CRC_SUPPORTED 0x44U

/** \brief Reserved value for associated Time Domains
 **
 ** The value is used for round-robin selection of
 ** the Time Domain for upcoming transmission (multiplexed),
 ** also prohibits multiple transmissions.
 */
#if (defined FRTSYN_RESERVED_TT_DONE)
#error FRTSYN_RESERVED_TT_DONE already defined!
#endif /* #if (defined FRTSYN_RESERVED_TT_DONE)*/
#define FRTSYN_RESERVED_TT_DONE 0xFFU

/** \brief Transmission mode type
 **
 ** This type defines the values that are for "read Tx mode".
 */
typedef uint8 FrTSyn_TransmissionModeType;

/** \brief Type of FrTSynGlobalTimeSyncDataIDListValue */
typedef uint8 FrTSyn_DataIDType;

/** \brief Defines the type of FrTSyn Slave shared variables. */
typedef struct
{
  CONST(uint16, TYPEDEF) syncTimeBaseId; /* Synchronized Time Base from StbM. */
  CONST(uint8, TYPEDEF) rxCrcValidated; /*  This parameter controls whether or not CRC validation shall be supported.*/
  CONST(uint8, TYPEDEF) scjumpWidth; /* The SequenceCounterJumpWidth specifies the maximum allowed gap of the Sequence Counter between two SYNC*/
  CONST(uint8, TYPEDEF) timeDomainId; /* Global Time Domain Handle Id. */
[!IF "$FrTSyn_TimeValidationEnabled = 'true'"!]
  CONST(uint8, TYPEDEF) segmentId; /* Global Time Network Segment Id. */
[!ENDIF!][!//
  CONST(uint8, TYPEDEF) assocIdx; /* Idx of OFS or self */
  CONST(uint8, TYPEDEF) ctrlId; /* Controller ID for FrIf_GetState(). */
  CONST(uint8, TYPEDEF) clstId; /* Cluster ID for FrIf_GetGlobalTime(). */
[!IF "$FrTSyn_TimeValidationEnabled = 'true'"!]
  CONST(boolean, TYPEDEF) timeValidation; /* Time Validation */
[!ENDIF!][!//
[!IF "($FrTSyn_NumGTDILEs > 0)"!][!//
  /*!LINKSTO FrTSyn.EB.DataType.DataID,1 */
  [!WS "2"!]CONST([!"$DIDIdxType"!], TYPEDEF) DIDIdx; /* Lookup IDX for DataIdList */
[!ENDIF!][!//
} FrTSyn_SlaveTimeDomainConfigType;


/** \brief Defines the type of FrTSyn Master shared variables. */
typedef struct
{
  CONST(uint16, TYPEDEF) syncTimeBaseId; /* Synchronized Time Base from StbM. */
  CONST(uint16, TYPEDEF) txPeriod; /* Time period of the SYNC sequence in MainFunction invocations. */
[!IF "$immediateTS = 'true'"!][!//
  CONST(uint16, TYPEDEF) cdbncCntr; /* Debounce counter reload value */
  [!IF "bit:and($usedTxTypes, $TX_BOTH) > 0"!][!// Only needed when both cyclic and immediate tx is used
  CONST(uint16, TYPEDEF) cresumeCntr; /* Cyclic message resume counter reload value */
  [!ENDIF!][!//
[!ENDIF!][!//
  CONST(PduIdType, TYPEDEF) txPduId; /* Handle Id of the FrIf PDU to transmit the global time information. */
  CONST(uint8, TYPEDEF) txCrcSecured; /*  This parameter controls whether or not CRC shall be supported.*/
  CONST(uint8, TYPEDEF) timeDomainId; /* Global Time Domain Handle Id. */
[!IF "$FrTSyn_TimeValidationEnabled = 'true'"!]
  CONST(uint8, TYPEDEF) segmentId; /* Global Time Network Segment Id. */
[!ENDIF!][!//
  CONST(uint8, TYPEDEF) assocIdx; /* Idx of OFS, self or FRTSYN_RESERVED_TT_DONE */
  CONST(uint8, TYPEDEF) ctrlId; /* Controller ID for FrIf_GetGlobalTime(). */
  CONST(uint8, TYPEDEF) clstId; /* Cluster ID for FrIf_GetState(). */
  CONST(uint8, TYPEDEF) txCondFuncIdx; /* Lookup ID FrTSyn_TxCondFuncs */
[!IF "$FrTSyn_TimeValidationEnabled = 'true'"!]
  CONST(uint8, TYPEDEF) transmissionMode; /* Immediate or Decoupled transmission in FrIf */
  CONST(boolean, TYPEDEF) timeValidation; /* Time Validation */
[!ENDIF!][!//
[!IF "($FrTSyn_NumGTDILEs > 0)"!][!//
  /* !LINKSTO FrTSyn.EB.DataType.DataID,1 */
  [!WS "2"!]CONST([!"$DIDIdxType"!], TYPEDEF) DIDIdx; /* Lookup IDX for DataIdList */
[!ENDIF!][!//
} FrTSyn_MasterTimeDomainConfigType;


/** \brief Defines the type of FrTSyn configuration structure. */
/* !LINKSTO FrTSyn.ASR44.SWS_FrTSyn_00061,1 */
typedef struct
{
  CONST(FrTSyn_SlaveTimeDomainConfigType, TYPEDEF) FrTSyn_TimeSlaveConfig;
  CONST(FrTSyn_MasterTimeDomainConfigType, TYPEDEF) FrTSyn_TimeMasterConfig;
} FrTSyn_ConfigType;


/** \brief Defines the type of FrTSyn shared variables. */
typedef struct
{
  VAR(uint32, TYPEDEF) cycleLength; /* Cycle Length in nanoseconds from FrIf */
  VAR(uint32, TYPEDEF) maxCycleLength; /* Maximum cycle length */
  VAR(uint16, TYPEDEF) macrotickDuration; /* MacrotickDuration in nanoseconds from FrIf */
  VAR(uint8, TYPEDEF) seqCount; /* Received SC */
  VAR(uint8, TYPEDEF) assocIdx; /* Idx of OFS or self */
  VAR(uint8, TYPEDEF) receivedMsg[16]; /* Received Message */
  VAR(boolean, TYPEDEF) isReceptionOngoing; /* Message received Flag */
} FrTSyn_SlaveType;


/** \brief Defines the type of FrTSyn shared variables. */
typedef struct
{
  VAR(uint32, TYPEDEF) cycleLength; /* Cycle Length in nanoseconds from FrIf */
  VAR(uint16, TYPEDEF) macrotickDuration; /* MacrotickDuration in nanoseconds from FrIf */
  VAR(uint16, TYPEDEF) txPeriod;/* Time until next transmission */
[!IF "$immediateTS = 'true'"!][!//
  VAR(uint16, TYPEDEF) dbncCntr; /* Debounce counter */
[!ENDIF!][!//
  VAR(FrTSyn_TransmissionModeType, TYPEDEF) txMode; /* Transmission Mode for this Master */
  VAR(uint8, TYPEDEF) txProperties; /* bit 0: Is timeBase set; bit 1: Is immediate transmission */
[!IF "$immediateTS = 'true'"!][!//
  VAR(uint8, TYPEDEF) timebaseUpdCntr; /* timeBaseUpdateCounter */
[!ENDIF!]
  VAR(uint8, TYPEDEF) assocIdx; /* Idx of OFS, self or FRTSYN_RESERVED_TT_DONE */
  VAR(uint8, TYPEDEF) seqCount; /* Current SC */
  VAR(uint8, TYPEDEF) msgToTransmit[16]; /* Message to be transmitted */
} FrTSyn_MasterType;

/** \brief Type for transmission condition evaluation functions */
typedef P2FUNC(boolean, FRTSYN_CODE, FrTSyn_TxCondFuncType)
(
  P2VAR(FrTSyn_MasterType, AUTOMATIC, AUTOMATIC) pMasterData,
  P2CONST(FrTSyn_MasterTimeDomainConfigType, AUTOMATIC, AUTOMATIC) pMasterCfg
);

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* ifndef FRTSYN_TYPES_H */
/*==================[end of file]===========================================*/
[!ENDCODE!]
