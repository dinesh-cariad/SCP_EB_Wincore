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

#ifndef CANTSYN_TYPES_H
#define CANTSYN_TYPES_H

/*==================[inclusions]============================================*/

[!INCLUDE "../include/CanTSyn_PrecompileMacros.m"!][!//
[!CALL "IsMasterTimeValidationSupported"!][!//
[!CALL "IsSlaveTimeValidationSupported"!][!//
#include <TSAutosar.h> /* Specific AUTOSAR standard types */
#include <ComStack_Types.h> /* typedefs for AUTOSAR com stack */
#include <StbM.h>
[!IF "($isMasterTimeValidationSupported = 'true') or ($isSlaveTimeValidationSupported = 'true')"!][!//
#include <StbM_CanTSyn.h>
[!ENDIF!][!//
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00092,1 */

#if (defined CANTSYN_TX_OFF)
#error CANTSYN_TX_OFF is already defined
#endif /* if (defined CANTSYN_TX_OFF) */

#define CANTSYN_TX_OFF 0U

#if (defined CANTSYN_TX_ON)
#error CANTSYN_TX_ON is already defined
#endif /* if (defined CANTSYN_TX_ON) */

#define CANTSYN_TX_ON 1U

/** \brief Transmission mode type
 **
 ** This type defines the values that the "read Tx mode".
 */
typedef uint8 CanTSyn_TransmissionModeType;


/** \brief Defines the type of Data ID Lists used for CRC calculation. */
typedef uint8 CanTSyn_DataIDListType;

/** \brief Defines the type of CanTSyn configuration structure. */
/* !LINKSTO CanTSyn.ASR44.SWS_CanTSyn_00091,1 */
typedef uint8 CanTSyn_ConfigType;

[!CALL "IsTxImmediateTransmissionUsed"!]

/** \brief Defines the type of CanTSyn Master shared variables. */
typedef struct
{
[!IF "$CanTSyn_CRCMasterDomains != 0"!]
  P2CONST(CanTSyn_DataIDListType, TYPEDEF, CANTSYN_APPL_CONST) SyncOfsDataIdListPtr;
  P2CONST(CanTSyn_DataIDListType, TYPEDEF, CANTSYN_APPL_CONST) FupOfnsDataIdListPtr;
[!ENDIF!]
  CONST(uint16, TYPEDEF) DebounceTime; /* Debounce time between SYNC and related FUP/ OFS and related OFNS messages */
[!IF "$IsTxImmediateTransmissionUsed = 'true'"!]
  CONST(uint16, TYPEDEF) CyclicMsgResumeTime; /* Time used to observe the time between an immediate transmission and the 1st cycle transmission */
[!ENDIF!][!//
  CONST(uint16, TYPEDEF) TxPeriodTimeout; /* Time period of the SYNC-FUP sequence in MainFunction invocations (indicates if there is a cyclic transmission of a
  SYNC or OFS message) */
  CONST(uint16, TYPEDEF) ConfirmationTimeout; /* Time used to observe the confirmation of a message */
  CONST(uint16, TYPEDEF) SyncTimeBaseId; /* StbMSynchronizedTimeBaseIdentifier needed for the call of StbM_GetCurrentTime() */
  CONST(PduIdType, TYPEDEF) TxPduId; /* Handle Id used to transmit the global time information */
  CONST(uint8, TYPEDEF) TimeDomainId; /* Global Time Domain Handle Id */
  CONST(uint8, TYPEDEF) ControllerId; /* Controller handle Id associated to this Pdu */
  CONST(uint8, TYPEDEF) CtrlIndex; /* Index of the controller associated to this Pdu */
  CONST(uint8, TYPEDEF) IsTxCrcSecuredUsed; /* Configuration parameter that activates/deactivates the CRC support */
  CONST(boolean, TYPEDEF) IsImmediateTimeSync; /* Configuration parameter that enables/disables the cyclic polling of StbM_GetTimeBaseUpdateCounter() */
[!IF "$CanTSyn_NumTxExtendedMsgFormat > 0"!]
  CONST(boolean, TYPEDEF) IsExtendedMessage; /* Configuration parameter that enables/disables the extended message format */
[!ENDIF!]
[!IF "$isMasterTimeValidationSupported = 'true'"!]
  CONST(boolean, TYPEDEF) IsTimeValidationUsed; /* Configuration parameter that enables/disables the use of Time validation */
  CONST(uint8, TYPEDEF) NetworkSegmentId; /* Global Time Network Segment ID */
[!ENDIF!]
} CanTSyn_TimeMasterDomainConfigType;

/** \brief Defines the type of CanTSyn Slave shared variables. */
typedef struct
{
[!IF "$CanTSyn_CRCSlaveDomains != 0"!]
  P2CONST(CanTSyn_DataIDListType, TYPEDEF, CANTSYN_APPL_CONST) SyncOfsDataIdListPtr;
  P2CONST(CanTSyn_DataIDListType, TYPEDEF, CANTSYN_APPL_CONST) FupOfnsDataIdListPtr;
[!ENDIF!]
  CONST(uint16, TYPEDEF) RxFollowUpOffsetTimeout; /* Time offset between SYN and related FUP messages in MainFunction invocations */
  CONST(uint16, TYPEDEF) SyncTimeBaseId; /* StbMSynchronizedTimeBaseIdentifier needed for StbM */
  CONST(uint8, TYPEDEF) TimeDomainId; /* Global Time Domain Handle Id */
  CONST(uint8, TYPEDEF) RxCrcValidated; /* Configuration parameter that activates/deactivates the CRC support */
  CONST(uint8, TYPEDEF) ScJumpWidth; /* Maximum allowed gap between two SYNC/ two OFS messages */
[!IF "$CanTSyn_NumRxExtendedMsgFormat > 0"!]
  CONST(boolean, TYPEDEF) IsExtendedMessage; /* Configuration parameter that enables/disables the extended message format*/
[!ENDIF!]
[!IF "$isSlaveTimeValidationSupported = 'true'"!]
  CONST(boolean, TYPEDEF) IsTimeValidationUsed; /* Configuration parameter that enables/disables the use of Time validation*/
  CONST(uint8, TYPEDEF) NetworkSegmentId; /* Global Time Network Segment ID */
[!ENDIF!]
} CanTSyn_TimeSlaveDomainConfigType;


/** \brief Defines the type of CanTSyn master shared variables used for the SYNC/FUP messages. */
typedef struct
{
  VAR(StbM_TimeStampType, TYPEDEF) SyncTimeT0; /* The T0 time containing seconds and nanoseconds */
  VAR(StbM_TimeStampType, TYPEDEF) FupTimeT4; /* T4 time needed for FUP message */
  VAR(StbM_VirtualLocalTimeType, TYPEDEF) T0VLT; /* T0 virtual local time */
  VAR(uint16, TYPEDEF) ConfirmationTimeout; /* Confirmation timeout after transmission of a SYNC message or a OFS message. */
  VAR(uint16, TYPEDEF) DebounceTime; /* Debounce Time between the SYNC and the related FUP or between OFS and OFNS */
[!IF "$IsTxImmediateTransmissionUsed = 'true'"!]
  VAR(uint16, TYPEDEF) CyclicMsgResumeTime; /* Time between an immediate transmission and the first cyclic transmission of a SYNC message */
[!ENDIF!][!//
  VAR(uint16, TYPEDEF) TimeTxPeriod; /* Time used for cyclic transmission of the SYNC messages */
  VAR(uint8, TYPEDEF) SeqCount; /* The sequence counter for the Master */
  VAR(uint8, TYPEDEF) TxStatus; /* Status of message transmission of the Master */
  VAR(uint8, TYPEDEF) SyncSGWBit; /* Time Gateway synchronization value */
  VAR(uint8, TYPEDEF) UserByte0; /* user data contained in Byte 0 */
  VAR(uint8, TYPEDEF) UserByte1; /* user data contained in Byte 1 */
  VAR(uint8, TYPEDEF) UserByte2; /* user data contained in Byte 2 */
  VAR(uint8, TYPEDEF) syncTimeBaseCounter; /* value indicating a Time Base update */
} CanTSyn_SyncMasterType;


/** \brief Defines the type of CanTSyn master shared variables used for the OFS/OFNS messages. */
typedef struct
{
  VAR(uint32, TYPEDEF) OfsTimeNSec; /* nanosecond portion of the Offset time */
  VAR(uint16, TYPEDEF) ConfirmationTimeout; /* Confirmation timeout after transmission of a SYNC message or a OFS message. */
  VAR(uint16, TYPEDEF) DebounceTime; /* Debounce Time between the SYNC and the related FUP or between OFS and OFNS */
[!IF "$IsTxImmediateTransmissionUsed = 'true'"!]
  VAR(uint16, TYPEDEF) CyclicMsgResumeTime; /* Time between an immediate transmission and the first cyclic transmission of an OFS message */
[!ENDIF!]
  VAR(uint16, TYPEDEF) TimeTxPeriod; /* Time used for cyclic transmission of the SYNC messages */
  VAR(uint8, TYPEDEF) SeqCount; /* The sequence counter for the Master */
  VAR(uint8, TYPEDEF) TxStatus; /* Status of message transmission of the Master */
  VAR(uint8, TYPEDEF) OfsSGWBit; /* Time Gateway synchronization value */
  VAR(uint8, TYPEDEF) UserByte0; /* user data contained in Byte 0 */
  VAR(uint8, TYPEDEF) UserByte1; /* user data contained in Byte 1 */
  VAR(uint8, TYPEDEF) UserByte2; /* user data contained in Byte 2 */
  VAR(uint8, TYPEDEF) OfsTimeBaseCounter; /* value indicating a Time Base update */
} CanTSyn_OffsetMasterType;


/** \brief Defines the type of CanTSyn slave shared variables used for the SYNC/FUP messages. */
typedef struct
{
  VAR(StbM_VirtualLocalTimeType, TYPEDEF) T2VLT; /* T2 virtual local time */
  VAR(uint32, TYPEDEF) T0Sec; /* Seconds part of T0 from SYNC message */
  VAR(uint16, TYPEDEF) TimeRxFollowUpOffset; /* Time between the SYNC and the related FUP or between OFS and OFNS */
  VAR(uint8, TYPEDEF) SeqCount; /* The sequence counter for the Slave */
  VAR(uint8, TYPEDEF) RxStatus; /* Status of message reception of the Slave */
  VAR(uint8, TYPEDEF) UserByte0; /* User Byte 0 from SYNC message */
  VAR(uint8, TYPEDEF) UserByte1; /* User Byte 1 from SYNC message, if no CRC configured */
}CanTSyn_SyncSlaveType;


/** \brief Defines the type of CanTSynTSyn slave shared variables used for the OFS/OFNS messages. */
typedef struct
{
  VAR(uint32, TYPEDEF) OfsSecLo; /* Seconds part from OFS message */
  VAR(uint16, TYPEDEF) TimeRxFollowUpOffset; /* Time between the between OFS and OFNS */
  VAR(uint8, TYPEDEF) SeqCount; /* The sequence counter for the Slave */
  VAR(uint8, TYPEDEF) RxStatus; /* Status of message reception of the Slave */
  VAR(uint8, TYPEDEF) UserByte0; /* User Byte 0 from OFS message */
  VAR(uint8, TYPEDEF) UserByte1; /* User Byte 1 from OFS message, if no CRC configured */
}CanTSyn_OffsetSlaveType;


/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* ifndef CANTSYN_TYPES_H */
/*==================[end of file]===========================================*/


