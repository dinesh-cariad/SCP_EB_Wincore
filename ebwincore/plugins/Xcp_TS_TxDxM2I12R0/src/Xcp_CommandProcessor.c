
/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 19.2 (advisory)
 * The union keyword should not be used.
 *
 * Reason:
 * Pointer is necessary to be used for both const and non-const positions to prevent
 * compiler warnings. Usage is allowed like this according to the C standard as qualified
 * and unqualified types have the same alignment and size restrictions, and the first
 * element within a union starts at the same memory position.
 *
 * MISRAC2012-2) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different object type.
 *
 * Reason:
 * Dynamic DAQ lists are stored into a memory area of uint8 type with fixed size. We
 * need to convert area sections into corresponding DAQ list, ODT and ODT entries types.
 *
 * MISRAC2012-3) Deviated Rule: 11.4 (advisory)
 * A conversion should not be performed between a pointer to object and an integer type.
 *
 * Reason:
 * Cast is unavoidable - we construct the address of an ODT Entry based on input bytes.
 *
 * MISRAC2012-4) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to object
 *
 * Reason:
 * The MTA address for write purposes is needed.
 *
 * MISRAC2012-5) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears in a single function
 *
 * Reason:
 * Using the variable at function scope would require making the variable static, which
 * would prevent allocation to a predefined memory section.
 *
 * MISRAC2012-6) Deviated Rule: 13.5 (required)
 * The right hand operand of a logical && or || operator shall not contain persistent side effects.
 *
 * Reason:
 * Function Xcp_IsMemoryAreaAccessible does not contain any side effect but MISRA generally considers
 * functions have side effects. Condition FALSE == Xcp_MemoryWrite.Pending needs to short-circuit
 * the call to Xcp_IsMemoryAreaAccessible as this should only be done once, when the download command
 * is processed the first time.
 * And also GetApplicationID() doesn't have any side effect.
 */

/*==================[inclusions]============================================*/


#include <Xcp.h>                /* Module public API */
#if ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) )
#include <TSAutosar.h>          /* EB specific standard types */
#include <TSMem.h>              /* EB memory functions */
#include <SchM_Xcp.h>           /* Needed for exclusive area definition */
#include <Xcp_Trace.h>

#include <Xcp_Int.h>            /* Internal defines and declarations */
#include <Xcp_UserCallouts.h>   /* XCP user callout functions */
#include <Xcp_Cbk.h>            /* Callbacks and callouts of Xcp */

#if (XCP_MEMORY_ACCESS_AREAS == STD_ON)
#include <Xcp_MemoryProxy.h>
#include <Os.h>                  /* Os counter is required */
#endif

#if (XCP_TIMESTAMP_TYPE != XCP_NO_TIME_STAMP_TS_MASK)

#if ((XCP_USER_TIMESTAMP_SUPPORTED == STD_OFF) && (XCP_MEMORY_ACCESS_AREAS == STD_OFF))
#include <Os.h>                  /* Os counter is required */
#endif

#endif /* (XCP_TIMESTAMP_TYPE != XCP_NO_TIME_STAMP_TS_MASK) */

#if (XCP_BUILD_CHECKSUM_SUPPORT == STD_ON)
#if ((XCP_BUILD_CHECKSUM_TYPE == XCP_CRC_16_CITT)||(XCP_BUILD_CHECKSUM_TYPE == XCP_CRC_32))
#include <Crc.h>               /* CRC is required for BUILD_CHECKSUM command */
#endif
#endif

#if ((XCP_STORE_DAQ_SUPPORTED == STD_ON) || (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON))
#include <NvM.h>                /* NvM API */
#endif


#if (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)
#include <FrIf.h>
#endif

#include <Xcp_Cfg.h>

/*==================[macros]================================================*/

/** \brief Negative Response
 **
 ** This macro fulfill a negative answer into the response queue
 **
 ** \param[in] err_pid -> the expected negative response, length - > expected length of the response
 */
#if (defined XCP_ERROR_RESPONSE)
#error XCP_ERROR_RESPONSE already defined
#endif
#define XCP_ERROR_RESPONSE(response_buffer, err_pid, length) \
  do \
  { \
    response_buffer.CTO[XCP_RES_PID_INDEX] = XCP_ERR_PID; \
    response_buffer.CTO[XCP_RES_ERRCODE_INDEX] = err_pid; \
    response_buffer.Length = length; \
  } while ( 0 )
/** \brief Increment MTA according to a size in AG
 **
 ** This macro increments the address of the MTA according to the given
 ** parameter Size.
 **
 ** \param[in] Size Size to increment the MTA in AG
 */
#if (defined XCP_INCREMENT_MTA_BY_SIZE_IN_AG)
#error XCP_INCREMENT_MTA_BY_SIZE_IN_AG already defined
#endif
#define XCP_INCREMENT_MTA_BY_SIZE_IN_AG(Size) \
  do \
  { \
    Xcp_MTA.Address.Write = &Xcp_MTA.Address.Write[((Size)* XCP_ADDRESS_GRANULARITY_SIZE)]; \
  } while( 0 )

#if (defined XCP_PGM_ABSOLUTE_ACCESS_MODE)
#error XCP_PGM_ABSOLUTE_ACCESS_MODE already defined
#endif
/** \brief Definition of value to indicate the absolute access mode as defined by ASAM */
#define XCP_PGM_ABSOLUTE_ACCESS_MODE                            0U

#if (defined XCP_PGM_ABS_ACCESS_MODE_MASK)
#error XCP_PGM_ABS_ACCESS_MODE_MASK already defined
#endif
/** \brief Definition of value to indicate that the ABSOLUTE_ACCESS_MODE is available for
 **        programming */
#define XCP_PGM_ABS_ACCESS_MODE_MASK                            0x1U

#if (defined XCP_PGM_PROPERTIES_MASK)
#error XCP_PGM_PROPERTIES_MASK already defined
#endif
/** \brief Definition for PGM_PROPERTIES bit mask for command GET_PGM_PROCESSOR_INFO:
 **        - bit0..1: the programming mode bits, will be masked to only contain those modes,
 **                   supported by the Xcp.
 **        - bit2..7: will remain as given by the Xcp_ApplGetSectorInfo().
 **/
#define XCP_PGM_PROPERTIES_MASK                                 0xFCU

#if (defined XCP_CMD_SYNCH_PENDING_MASK)
#error XCP_CMD_SYNCH_PENDING_MASK already defined
#endif
/** \brief Definition of value to indicate a pending SYNCH command */
#define XCP_CMD_SYNCH_PENDING_MASK                              0x1U

#if ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON))
#if (defined XCP_RECEIVED_GET_SLAVE_ID_ECHO_MASK)
#error XCP_RECEIVED_GET_SLAVE_ID_ECHO_MASK already defined
#endif
/** \brief Definition of value to indicate reception of sub command GET_SLAVE_ID
 ** with mode = "identify by echo" */
#define XCP_RECEIVED_GET_SLAVE_ID_ECHO_MASK                     0x2U
#endif /* ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON)) */

#if (defined XCP_CMD_DISCONNECT_PENDING_MASK)
#error XCP_CMD_DISCONNECT_PENDING_MASK already defined
#endif
/** \brief Definition of value to indicate a pending DISCONNECT command */
#define XCP_CMD_DISCONNECT_PENDING_MASK                         0x4U

#if (defined XCP_CMD_BUSY_ABORTED_MASK)
#error XCP_CMD_BUSY_ABORTED_MASK already defined
#endif
/** \brief Definition of value to indicate an aborted busy command */
#define XCP_CMD_BUSY_ABORTED_MASK                               0x8U

#if (defined XCP_CMD_DOWNLOAD_BLOCK_MODE_MASK)
#error XCP_CMD_DOWNLOAD_BLOCK_MODE_MASK already defined
#endif
/** \brief Definition of value to indicate a pending DOWNLOAD block mode transfer */
#define XCP_CMD_DOWNLOAD_BLOCK_MODE_MASK                        0x10U

#if (defined XCP_CMD_PROGRAMMING_STARTED_MASK)
#error XCP_CMD_PROGRAMMING_STARTED_MASK already defined
#endif
/** \brief Definition of value to indicate that PROGRAM_START command was received */
#define XCP_CMD_PROGRAMMING_STARTED_MASK                        0x20U

#if (defined XCP_CMD_PROGRAMMING_ACTIVE_MASK)
#error defined XCP_CMD_PROGRAMMING_ACTIVE_MASK already defined
#endif
/** \brief Definition of value to indicate that the programming of a segment is active */
#define XCP_CMD_PROGRAMMING_ACTIVE_MASK                         0x40U

#if (defined XCP_CMD_PGM_BLOCK_MODE_MASK)
#error defined XCP_CMD_PGM_BLOCK_MODE_MASK already defined
#endif
/** \brief Definition of value to indicate that the programming is currently in block mode */
#define XCP_CMD_PGM_BLOCK_MODE_MASK                             0x80U

#if (defined XCP_ABORT_PROGRAMMING_MASK)
#error XCP_ABORT_PROGRAMMING_MASK already defined
#endif
/** \brief Definition of value to indicate that the flash programming was marked to be aborted
 * by an asynchronous event (e.g. the RX indication)*/
#define XCP_ABORT_PROGRAMMING_MASK                              0x1U

/** \brief Response length value for commands that are not supported by the current
 * configuration/version */
#if (defined XCP_UNSUPPORTED_CMD_RES_LENGTH)
#error XCP_UNSUPPORTED_CMD_RES_LENGTH already defined
#endif
#define XCP_UNSUPPORTED_CMD_RES_LENGTH                     255U

#if (defined XCP_IS_SYNCH_CMD_PENDING)
#error XCP_IS_SYNCH_CMD_PENDING already defined
#endif
/** \brief Returns the SYNCH command status. */
#define XCP_IS_SYNCH_CMD_PENDING() \
  ((XCP_CMD_SYNCH_PENDING_MASK == (Xcp_CmdReqStatus & XCP_CMD_SYNCH_PENDING_MASK))? TRUE:FALSE)

#if (defined XCP_IS_DISCONNECT_CMD_PENDING)
#error XCP_IS_DISCONNECT_CMD_PENDING already defined
#endif
/** \brief Returns the DISCONNECT command status. */
#define XCP_IS_DISCONNECT_CMD_PENDING() \
  ((XCP_CMD_DISCONNECT_PENDING_MASK == (Xcp_CmdReqStatus & XCP_CMD_DISCONNECT_PENDING_MASK))? TRUE:FALSE)

#if (defined XCP_IS_CMD_BUSY_ABORTED)
#error XCP_IS_CMD_BUSY_ABORTED already defined
#endif
/** \brief Returns whether the busy command was aborted or not */
#define XCP_IS_CMD_BUSY_ABORTED() \
  ((XCP_CMD_BUSY_ABORTED_MASK == (Xcp_CmdReqStatus & XCP_CMD_BUSY_ABORTED_MASK))? TRUE:FALSE)

#if (defined XCP_UNSUPPORTED_CMD_LENGTH)
#error XCP_UNSUPPORTED_CMD_LENGTH already defined
#endif
/** \brief Unsupported commands are handled as length 1. In this way their
 **        PID is evaluated and ERR_CMD_UNKNOWN is replied. */
#define XCP_UNSUPPORTED_CMD_LENGTH        1U

#if (defined XCP_WRITE_DAQ_MULTIPLE_BYTES_PER_ELEMENT)
#error XCP_WRITE_DAQ_MULTIPLE_BYTES_PER_ELEMENT already defined
#endif
/** \brief Number of bytes needed to be received in the command for each element */
#define XCP_WRITE_DAQ_MULTIPLE_BYTES_PER_ELEMENT        8U

/** \brief Macro to initialize the Cyclic Event counter.
 **
 ** This macro is used for a Cyclic Event
 ** to initialize to 1U the TimeCycleCounter variable.
 **
 **/
#if (defined XCP_CYCLIC_EVENTS_CNT_INI)
#error XCP_CYCLIC_EVENTS_CNT_INI already defined
#endif
#define XCP_CYCLIC_EVENTS_CNT_INI()                                      \
  do                                                                     \
  {                                                                      \
    /* Check if the event that has this Daq attached is a cyclic one */  \
    if( Xcp_EventInfo[DaqListPtr->EventId].TimeCycle > 0U )              \
    {                                                                    \
      /* This is a cyclic event */                                       \
      /* Set the cyclic event time cycle to the initial value 1U.        \
         This value allows the event to trigger for the first time when  \
         the Xcp_MainFunction is called */                               \
         Xcp_Event[DaqListPtr->EventId].TimeCycleCounter = 1U;      \
    }                                                                    \
  } while( 0 )

#if (XCP_RESOURCE_CAL_PAG == XCP_RESOURCE_CAL_PAG_MASK)
/** \brief Macro required only if Calibration/Paging Resource is supported */
#if (defined XCP_GET_SEGMENT_INFO_CMD_RES_LENGTH)
#error XCP_GET_SEGMENT_INFO_CMD_RES_LENGTH already defined
#endif
/** \brief GET_SEGMENT_INFO comand response maximum length */
#define XCP_GET_SEGMENT_INFO_CMD_RES_LENGTH  5U

#endif /* (XCP_RESOURCE_CAL_PAG == XCP_RESOURCE_CAL_PAG_MASK) */

#if (XCP_MODIFY_BITS_SUPPORTED == STD_ON)
/** \brief Macros required only if MODIFY_BITS command is supported */
#if (defined XCP_MODIFY_VALUE_SIZE)
#error XCP_MODIFY_VALUE_SIZE already defined
#endif
/** \brief Modify Bits value size (4 bytes) */
#define XCP_MODIFY_VALUE_SIZE                4U

#if (defined XCP_MODIFY_SHIFT_MAX)
#error XCP_MODIFY_SHIFT_MAX already defined
#endif
/** \brief Modify Bits Shift value max */
#define XCP_MODIFY_SHIFT_MAX                31U

#endif /* (XCP_MODIFY_BITS_SUPPORTED == STD_ON) */

/** \brief Identifier for calculating the length of the DOWNLOAD,
 **        DOWNLOAD_NEXT, UNLOCK, TRANSPORT_LAYER_CMD CTO, PROGRAM, PROGRAM_NEXT */
#if (defined XCP_COMMAND_LENGTH_CALCULATED)
#error XCP_COMMAND_LENGTH_CALCULATED already defined
#endif
#define XCP_COMMAND_LENGTH_CALCULATED     240U

#if (defined XCP_RESP_CTO_QUEUE_LENGTH_SIZE)
#error XCP_RESP_CTO_QUEUE_LENGTH_SIZE already defined
#endif
/** \brief Response CTO message length size (number of bytes).
 * The length of a response CTO message is an 8-bit number (one byte) */
#define XCP_RESP_CTO_QUEUE_LENGTH_SIZE    1U

#if (defined XCP_RESP_CTO_QUEUE_MULTICAST_SIZE)
#error XCP_RESP_CTO_QUEUE_MULTICAST_SIZE already defined
#endif
/** \brief Number of bytes used for multicast flag in each CTO.
 *  */
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
#define XCP_RESP_CTO_QUEUE_MULTICAST_SIZE    1U
#else
#define XCP_RESP_CTO_QUEUE_MULTICAST_SIZE    0U
#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */

#if (defined XCP_RESP_CTO_QUEUE_FREE_RESERVED)
#error XCP_RESP_CTO_QUEUE_FREE_RESERVED already defined
#endif
/** \brief Removes a number of bytes that were previously reserved. */
#define XCP_RESP_CTO_QUEUE_FREE_RESERVED(PrevReservedBytes)  \
Xcp_CtoQueue.ReservedBytes -= ( (PrevReservedBytes) + XCP_RESP_CTO_QUEUE_LENGTH_SIZE )

#if (defined XCP_IS_COMMAND_EXECUTABLE)
#error XCP_IS_COMMAND_EXECUTABLE already defined
#endif
/** \brief Verify whether the command is executable.
 *
 * When XCP is connected all available commands can be executed.  When XCP is
 * disconnected only the CONNECT and GET_SLAVE_ID can be executed All commands
 * with ID > 0xC0 will be queued from rx indication function 0x00U - 0xBF are
 * queued as STIM. */
#define XCP_IS_COMMAND_EXECUTABLE(command) \
(((Xcp_ConnectionStatus.State == XCP_STATE_CONNECTED) || \
  (((Xcp_ConnectionStatus.State == XCP_STATE_DISCONNECTED) || \
    (Xcp_ConnectionStatus.State == XCP_STATE_RESUME)) && \
   (((command) == XCP_CMD_CONNECT_PID) || \
    ((command) == XCP_CMD_TRANSPORT_LAYER_PID ))) \
  )? TRUE:FALSE)

#if (defined XCP_CMD_PROCESSOR_STATE_TRANS)
#error XCP_CMD_PROCESSOR_STATE_TRANS already defined
#endif
/** \brief Change command processor state machine to specified state. */
#define XCP_CMD_PROCESSOR_STATE_TRANS(nextState) \
                                             (Xcp_CmdProcessorState = (nextState))

#if (defined XCP_GET_CTOQUEUE_FREESPACE)
#error XCP_GET_CTOQUEUE_FREESPACE already defined
#endif
/** \brief Calculate free space available in CTO Queue.
 ** \param[in] RetType Cast type used to fix MISRA-C 10.1 violation */
#define XCP_GET_CTOQUEUE_FREESPACE(RetType) \
                            ((RetType)XCP_CTO_QUEUE_SIZE - (RetType)Xcp_CtoQueue.NoOfBytes)

#if (defined XCP_READ_CMDPACKET_BYTE)
#error XCP_READ_CMDPACKET_BYTE already defined
#endif
/** \brief Reads specified byte from  first command packet in command queue. */
#define XCP_READ_CMDPACKET_BYTE(ByteIndex) \
    (Xcp_CommandQueue.Queue[Xcp_CommandQueue.Head].CTO[(ByteIndex)])

#if (defined XCP_GET_NO_OF_COMMANDS)
#error XCP_GET_NO_OF_COMMANDS already defined
#endif
/** \brief Returns the number of commands. */
#define XCP_GET_NO_OF_COMMANDS()                          (Xcp_CommandQueue.NoOfCmds)

#if (XCP_PGM_SUPPORTED == STD_ON)

#if (defined XCP_PGM_NO_OF_ELEMENTS_INDEX)
/** \brief The index in the PROGRAM and PROGRAM_NEXT commands parameters where how many elements
 * to flash is found */
#error XCP_PGM_NO_OF_ELEMENTS_INDEX already defined
#endif
#define XCP_PGM_NO_OF_ELEMENTS_INDEX     1U

#if (defined XCP_PGM_CLEAR_RANGE_INDEX)
/** \brief The index in the PROGRAM_CLEAR command input parameters where the clear range is found */
#error XCP_PGM_CLEAR_RANGE_INDEX already defined
#endif
#define XCP_PGM_CLEAR_RANGE_INDEX        4U

#if (defined XCP_IS_PGM_COMMAND_ID)
#error XCP_IS_PGM_COMMAND_ID already defined
#endif
/** \brief Checks if the command id is a PGM id */
#define XCP_IS_PGM_COMMAND_ID(CommandId) \
  ( (((CommandId) >= XCP_CMD_FIRST_PROGRAMMING_PID) && \
     ((CommandId) <= XCP_CMD_LAST_PROGRAMMING_PID) \
    ) ? TRUE : FALSE )

#endif /* (XCP_PGM_SUPPORTED == STD_ON) */

#if (defined XCP_MTATYPE_ADDRESS)
#error XCP_MTATYPE_ADDRESS already defined
#endif
/** \brief  MTA is address of a memory location */
#define XCP_MTATYPE_ADDRESS              0U

#if (defined XCP_MTATYPE_COUNTER)
#error XCP_MTATYPE_COUNTER already defined
#endif
/** \brief  MTA is used as sequence counter */
#define XCP_MTATYPE_COUNTER              1U

#if (XCP_SEED_AND_KEY_ENABLED == STD_ON)

#if (defined XCP_SEC_ACCESS_GETSEED_ZERO)
#error XCP_SEC_ACCESS_GETSEED_ZERO already defined
#endif
/** \brief Security access request is a Get Seed request with first part of seed */
#define XCP_SEC_ACCESS_GETSEED_ZERO      0U

#if (defined XCP_SEC_ACCESS_GETSEED_ONE)
#error XCP_SEC_ACCESS_GETSEED_ONE already defined
#endif
/** \brief Security access request is a Get Seed request with remaining parts of seed */
#define XCP_SEC_ACCESS_GETSEED_ONE       1U

#if (defined XCP_SEC_ACCESS_UNLOCK)
#error XCP_SEC_ACCESS_UNLOCK already defined
#endif
/** \brief Security access request is an Unlock */
#define XCP_SEC_ACCESS_UNLOCK            2U

#if (defined XCP_SEC_ACCESS_INVALID)
#error XCP_SEC_ACCESS_INVALID already defined
#endif
/** \brief Security access request is invalid(initial state) */
#define XCP_SEC_ACCESS_INVALID           3U

#endif /* (XCP_SEED_AND_KEY_ENABLED == STD_ON) */

#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON)

#ifdef XCP_ODT_ENTRY_USED_SPACE
#error XCP_ODT_ENTRY_USED_SPACE already defined
#endif
/** \brief The virtual size of an ODT entry when optimizations are enabled
 ** computed as the sum of its address, length and its bit offset if it is enabled */
#if (XCP_ENABLE_BIT_OFFSET == STD_OFF)
#define XCP_ODT_ENTRY_USED_SPACE ((XCP_GET_ODT_ENTRY_ADDRESS_SIZE + XCP_GET_ODT_ENTRY_LENGTH_SIZE))
#else
#define XCP_ODT_ENTRY_USED_SPACE ((XCP_GET_ODT_ENTRY_ADDRESS_SIZE + XCP_GET_ODT_ENTRY_LENGTH_SIZE + XCP_GET_ODT_ENTRY_BITOFFSET_SIZE))
#endif

#endif

/*==================[type definitions]======================================*/
/** \brief  Definition of MTA type.
 *
 * MTA can be an address or a sequence counter. */
typedef uint8 Xcp_MTAType_t;

/** \brief MTA information structure */
/* Deviation MISRAC2012-1 <START> */
typedef struct {
  union
  {
    P2VAR(uint8, TYPEDEF, XCP_VAR)   Write; /* Memory transfer address */
    P2CONST(uint8, TYPEDEF, XCP_VAR) Read;  /* Memory transfer address */
  } Address;

  Xcp_MTAType_t Type; /* MTA type */
  boolean IsSet;      /* As NULL_PTR can be a valid address to be set, an additional flag indicating
                         whether MTA was set is needed*/
} Xcp_MTA_t;
/* Deviation MISRAC2012-1 <STOP> */
/** \brief Command processor function pointer type */
typedef P2FUNC(Xcp_ErrorType, XCP_CONST, CommandHandlerPtr_t)(void);

/** \brief Structure containing each commands' execution handler and additional information regarding
 * the execution needs */
typedef struct
{
  CommandHandlerPtr_t CommandHandlerPtr;        /* Pointer to command execution handler */
  uint8               BaseLength;               /* Base Length */
  uint8               CommandMaxResponseLength; /* Maximum response length a command can send */
} Xcp_Commands_t;

/* If DAQ or STIM resource is supported */
#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) || \
     (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))
/** \brief DAQ pointer information structure */
typedef struct {
  uint16 DAQList; /* DAQ List number */
  uint8 ODT;      /* Relative ODT number within the DAQ list */
  uint8 ODTEntry;  /* Relative ODT entry number within the ODT */
} Xcp_DAQPtr_t;
#endif

/* If Seed and Key mechanism is required */
#if (XCP_SEED_AND_KEY_ENABLED == STD_ON)
/** \brief  Definition of security access request type.
 *
 * Security request type can be a SEED request or Unlock / Compare KEY
 * request. */
typedef uint8 Xcp_SecAccessType_t;

/** \brief Security Access information structure */
typedef struct {
  Xcp_SecAccessType_t PrevType; /* PreviousSecurity Access type */
  uint8 Resource;           /* Last requested resource */
} Xcp_SecAccess_t;

#endif /* (XCP_SEED_AND_KEY_ENABLED == STD_ON) */

/** \brief Structure needed by PROGRAM, PROGRAM_NEXT, DOWNLOAD & DOWNLOAD_NEXT commands to write
 * in the memory (RAM or non-volatile) */
typedef struct
{
  uint8 ElementsLeftToWrite;                /* The remaining data elements that must be written in
                                               the memory */
  uint8 ElementsWrittenSoFar;               /* The data elements that were processed up to now and put in
                                               the write buffer. Used only for block mode transfer */
  uint8 Buffer[XCP_MAX_MEMORY_WRITE_BYTES]; /* The buffer where we keep the elements received from
                                               the master. When the writing (DOWNLOAD or PROGRAM
                                               sequence) is completed successfully, the buffer
                                               values will be copied at the MTA */
  boolean Pending;                          /* Flag indicating that the current write operation is
                                               pending due to RAM callbacks returning XCP_APPL_BUSY */
} Xcp_MemoryWriteType;

#if (XCP_BUILD_CHECKSUM_SUPPORT == STD_ON)
/** \brief Typedef of the Build Checksum data structure used to exchange data,
 **        between the two tasks used for asynchronous crc calculation. */
typedef struct {
  uint32 BlockSize;           /* store the memory size of the blocked used to calculate crc. */
  uint32 ChecksumResult;      /* store crc operation result. */
  P2CONST( void, AUTOMATIC, XCP_APPL_DATA ) AddressPtr;/*store start address of the memory block.*/
#if (XCP_BUILD_CHECKSUM_TYPE == XCP_CRC_USER_CALLOUT)
  uint8 ApplCalloutResult;    /* store the execution result of the crc user callout. */
#endif /* (XCP_BUILD_CHECKSUM_TYPE == XCP_CRC_USER_CALLOUT) */
  uint8 ChecksumType;         /* store the type used to calculated the crc. */
#if (XCP_ASYNCH_CRC_SUPPORT == STD_ON)
  boolean IsRequestCRCActive; /*semaphore which manages concurrent access between the two tasks.*/
#endif /* (XCP_ASYNCH_CRC_SUPPORT == STD_ON) */
} Xcp_BuildChecksumDescriptor_t;
#endif /* (XCP_BUILD_CHECKSUM_SUPPORT == STD_ON) */

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

/** \brief Function to reset the variables used to monitor a memory write
 **
 ** Use this function if the download/programming sequence finished (positively or with an error)
 ** or SYNCH command is executed or, if it's the download case, DISCONNECT command is executed.
 **  */
STATIC FUNC(void, XCP_CODE) Xcp_ResetMemoryWriteInfo(void);

/** \brief Function to enqueue CTO into CTO queue.
 **
 ** Use this function to enqueue one response CTO into CTO queue.
 ** CTO queue is implemented  as byte array. CTO length will be queued
 ** first(or second if Multicast commands are implemented) and then the
 ** CTO data bytes will be queued.
 **
 ** \param[in] CtoPtr       Address of the CTO to be queued
 **
 */
STATIC FUNC(void, XCP_CODE) Xcp_InsertResponseCTO
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA) CtoPtr
);

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)

/** \brief Function to enqueue CTO for a multicast response into the CTO queue.
 **
 ** Use this function to enqueue one multicast response CTO into CTO queue.
 ** CTO queue is implemented  as byte array. CTO multicast flag will be
 ** queued first, followed by CTO length and then the CTO data bytes
 ** will be queued.
 **
 ** \param[in] CtoPtr       Address of the CTO to be queued
 **
 */
STATIC FUNC(void, XCP_CODE) Xcp_InsertMulticastResponseCTO
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA) CtoPtr
);

#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */

/** \brief Function to determine the number of bytes to be reserved in the CTO response queue.
 **
 **  The function will return the number of bytes to be reserved either statically, as the
 **  maximum response the command can yield, either dynamically, based on the command parameters.
 **
 ** \param[in]      CommandId  The command which awaits execution.
 **
 ** \return The number of bytes to be reserved.
 ** */
STATIC FUNC(uint8, XCP_CODE) Xcp_GetLengthToBeReserved
(
  uint8 CommandId
);

/** \brief Function to reserve bytes to the response CTO queue.
 **
 ** This function reserves a number of bytes (ReserveBytes + 1)
 ** to the response CTO queue.
 ** It does that way because the length of a message goes to
 ** the response CTO queue as well.
 ** This function must be called from an exclusive area (protected).
 **
 ** \param[in]      ReserveBytes  Number of bytes to be reserved.
 **
 ** \return Result of the operation
 ** \retval E_OK Bytes have been reserved in the queue.
 ** \retval E_NOT_OK Not enough space in the queue to reserve that many bytes.
 ** */
STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_ReserveBytesToRespCTOQueue
(
  Xcp_CtoNoOfBytesType ReserveBytes
);

/* If DAQ or STIM resource is supported */
#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) || \
     (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))

/** \brief Function to disassociate DAQ list from an Event channel.
 **
 ** Use this function to disassociate specified DAQ list from specified
 ** Event channel. First the daq list will be removed and then the DAQ Lists
 ** positioned after the specified DAQ list will be moved one position up
 ** the list.
 **
 ** \param[in] DaqListNumber       DAQ List to be disassociated
 ** \param[in] EventChannelNumber  Event channel from which the DAQ List to be disassociated
 */


STATIC FUNC(void, XCP_CODE) Xcp_RemoveDAQListFromEventChannel
(
  Xcp_DaqIdType DaqListNumber,
  uint16 EventChannelNumber
);

/** \brief Function to associate DAQ list to an Event channel.
 **
 ** Use this function to associate specified DAQ list to specified
 ** Event channel. All DAQ List with priority less that the specified
 ** DAQ list will be moved one position down the list and then the new
 ** DAQ List will be inserted.
 **
 ** \param[in] DaqListNumber       DAQ List to be associated with the specified event channel
 ** \param[in] EventChannelNumber  Event channel to which the DAQ List to be associated
 */
STATIC FUNC(void, XCP_CODE) Xcp_AssociateDAQListToEventChannel
(
  Xcp_DaqIdType DaqListNumber,
  uint16 EventChannelNumber
);

#endif /* ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) ||
        *  (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)) */

/** \brief Function to queue CTO bytes into CTO queue.
 **
 ** CTO Queue is a byte array. Use this function to enqueue one byte from the
 ** CTO into CTO queue
 **
 ** \param[in] DataByte  The data byte to be queued
 */
STATIC FUNC(void, XCP_CODE) Xcp_EnqueueByteInCtoQueue(uint8 DataByte);

/** \brief Function to Dequeue one command from Command Queue.
 **
 ** Use this function to flush one command from command queue.  Queue head
 ** will be updated to the next command in queue and rolled over if
 ** necessary. Number of commands in queue will also be updated. */
STATIC FUNC(void, XCP_CODE) Xcp_DeQueueCmdQueue(void);

/** \brief Function to process one single command.
 **
 ** Use this function to execute the first command in command queue. The command will be checked for
 ** its executability. If the command is executable, a request will be made to invoke the
 ** corresponding command handler.
 **
 ** Depending on the command handler response, the following post-execution actions will be made:
 **
 ** - XCP_E_BUSY:
 ** Command processing can't be finished in this cycle. The command will be kept in
 ** the queue and the state of the command processor will change to busy.
 **
 ** - XCP_E_OK, XCP_E_NOT_OK:
 **  Command finished (successfully or not). The command shall be dequeued and state of the command
 **  processor will change to idle.
 **
 ** - XCP_E_INSUFFICIENT_MEMORY:
 ** Command didn't execute as there was not enough resources to do that (e.g. not enough space in
 ** the response queue). Keep the command in queue until needed resources are available.
 **
 **  */
STATIC FUNC(void, XCP_CODE) Xcp_ProcessCmd(void);

/** \brief Invoke processing function for the specified command.
 **
 **  The processing function for the specified command will be called only if the maximum response
 **  the command could yield can be queued in the CTO response queue.
 **
 ** \param[in] Command The command ID
 **
 ** \return The command execution response
 ** \retval XCP_E_OK      Processing done
 ** \retval XCP_E_NOT_OK  Processing can't be done
 ** \retval XCP_E_BUSY    Processing not complete
 ** \retval XCP_E_INSUFFICIENT_MEMORY  Processing failed due to insufficient memory
 **  */
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ExecuteCommand(uint8 Command);

/** \brief Function to Process CONNECT command
 **
 ** Use this function to execute a CONNECT command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdConnect(void);

/** \brief Function to Process DISCONNECT command request.
 **
 ** Use this function to execute a DISCONNECT command request.
 **
 ** \return E_OK if command succeeded and E_NOT_OK otherwise.
 ** */
STATIC FUNC(Std_ReturnType, XCP_CODE)  Xcp_ProcessCmdDisConnect(void);

/** \brief Function to Process GET_ID command request.
 **
 ** Use this function to execute a GET_ID command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetId(void);

/** \brief Function to Process GET_COMM_MODE_INFO command request.
 **
 ** Use this function to execute a GET_COMM_MODE_INFO command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetCommModeInfo(void);

#if (XCP_PGM_SUPPORTED == STD_ON)

/** \brief Function used to handle Xcp_CmdReqStatusExtension requests */
STATIC FUNC(void, XCP_CODE) Xcp_HandleCmdReqExtStatus(void);

/** \brief Function to process PROGRAM_MAX command request.
 **
 ** Use this function to execute a PROGRAM_MAX command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 ** \retval XCP_E_INSUFFICIENT_MEMORY Space not available to send response.
 */
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdProgramMax(void);

#if (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)

/** \brief Function to process PROGRAM_NEXT command request.
 **
 ** Use this function to execute a PROGRAM_NEXT command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 */
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdProgramNext(void);
#endif /* (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON) */

/** \brief Function to process PROGRAM_RESET command request.
 **
 ** Use this function to execute a PROGRAM_RESET command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 */
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdProgramReset(void);

/** \brief Function to process PROGRAM command request.
 **
 ** Use this function to execute a PROGRAM command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 */
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdProgram(void);

/** \brief Function to process PROGRAM_CLEAR command request.
 **
 ** Use this function to execute a PROGRAM_CLEAR command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 */
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdProgramClear(void);

/** \brief Function to process PROGRAM_START command request.
 **
 ** Use this function to execute a PROGRAM_START command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 */
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdProgramStart(void);

/** \brief Function to process GET_SECTOR_INFO command request.
 **
 ** Use this function to execute a GET_SECTOR_INFO command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetSectorInfo(void);

/** \brief Function to process GET_PGM_PROCESSOR_INFO command request.
 **
 ** Use this function to execute a GET_PGM_PROCESSOR_INFO command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetPgmProcInfo(void);

/** \brief Helper function to process flash programming
 **
 ** This function is used by the commands PROGRAM, PROGRAM_MAX and PROGRAM_NEXT
 ** (as far as available) to perform the real programming.
 **
 ** \param[in] DataPtr Pointer to the data, that shall be programmed.
 ** \param[in] CallingCmdPid -  Which programming command called this function (PROGRAM,
 ** PROGRAM_NEXT or PROGRAM_MAX)
 **
 */
STATIC FUNC(void, XCP_CODE) Xcp_ProgramFlash
(
  P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) DataPtr,
  uint8 CallingCmdPid
);
#endif /* (XCP_PGM_SUPPORTED == STD_ON) */

/** \brief Function to Process GET_STATUS command request.
 **
 ** Use this function to execute a GET_STATUS command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetStatus(void);
/* If DAQ or STIM resource is supported */
#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) || \
     (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))
/** \brief Function to Process START_STOP_DAQ_LIST command request.
 **
 ** Use this function to execute a START_STOP_DAQ_LIST command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdStartStopDAQList(void);

/** \brief Function to Process START_STOP_SYNCH command request.
 **
 ** Use this function to execute a START_STOP_SYNCH command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdStartStopSynch(void);

/** \brief Function is sub function to Process the START STOP Sych command request.
 **
 ** Use this function to check error handling and to execute START STOP Sych command.
 **
 ** \param[out] Xcp_CTOType pointer to ResponseCTO data.
 **/
STATIC FUNC(void, XCP_CODE) Xcp_ProcessStartStopSynch
(
  P2VAR(Xcp_CTOType,AUTOMATIC,AUTOMATIC) ResponseCTOPtr
);

/** \brief Function to Process GET_DAQ_PROCESSOR_INFO command request.
 **
 ** Use this function to execute a GET_DAQ_PROCESSOR_INFO command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetDAQProcessorInfo(void);

/** \brief Function to Process GET_DAQ_LIST_MODE command request.
 **
 ** Use this function to execute a GET_DAQ_LIST_MODE command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetDAQListMode(void);

/** \brief Function to Process GET_DAQ_RESOLUTION_INFO command request.
 **
 ** Use this function to execute a GET_DAQ_RESOLUTION_INFO command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetDAQResolutionInfo(void);

/** \brief Function to Process GET_DAQ_EVENT_INFO command request.
 **
 ** Use this function to execute a GET_DAQ_EVENT_INFO command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetDAQEventInfo(void);

/** \brief Function is sub function to Process the Set DAQ list mode command request.
 **
 ** Use this function to check error handling for Set DAQ list mode command.
 **
 ** \param[out] Xcp_CTOType pointer to ResponseCTO data.
 **/
STATIC FUNC(void, XCP_CODE) Xcp_SubSetDAQListMode
(
  P2VAR(Xcp_CTOType,AUTOMATIC,AUTOMATIC) ResponseCTOPtr
);

/** \brief Function is sub function to Process the Set DAQ list mode command request.
 **
 ** Use this function to check error handling for Set DAQ list direction.
 **
 ** \param[in] EventCfgPtr sets a pointer for the configured event list
 ** \param[in] ResponseCTOPtr fulfill the response queue
 ** \param[out] Status which results from daq direction check.
 **/
STATIC FUNC( boolean, XCP_CODE) Xcp_SubSetDaqMode_DaqProcess
(
 P2CONST(Xcp_EventInfoType,AUTOMATIC, XCP_VAR) EventCfgPtr,
 P2VAR(Xcp_CTOType,AUTOMATIC,AUTOMATIC) ResponseCTOPtr
);
/** \brief Function is sub function to Process the set DAQ list mode command request.
 **
 ** Use this function to execute Set DAQ list mode command.
 **/
STATIC FUNC(void, XCP_CODE) Xcp_ProcessSetDAQListMode
(
  void
);

/* This command is available only for static DAQ lists */
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC_MASK)

/** \brief Function to Process GET_DAQ_LIST_INFO command request.
 **
 ** Use this function to execute a GET_DAQ_LIST_INFO command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetDAQListInfo(void);

/** \brief Function to Process CLEAR_DAQ_LIST command request.
 **
 ** Use this function to execute a CLEAR_DAQ_LIST command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdClearDAQList(void);

#if (XCP_MIN_DAQ < XCP_NO_OF_NON_DYNAMIC_DAQ_LISTS)
/** \brief Function to clear ODT Entries associated with STATIC DAQ lists.
 **
 ** This function clears pointer to data element, address extension, length
 ** information and bit offset.
 **
 ** \param[in] OdtEntryPtr     Address of first ODT Entry information to be cleared
 ** \param[in] NoOfODTEntries  Number of ODT Entry informations to be cleared
 */
STATIC FUNC(void, XCP_CODE) Xcp_ClearODTEntries
(
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
  P2VAR(Xcp_OdtEntryType, AUTOMATIC, XCP_APPL_DATA) ODTEntryPtr,
#else
  P2VAR(usize, AUTOMATIC, XCP_APPL_DATA) ODTEntryPtr,
#endif
  uint8 NoOfODTEntries
);

/** \brief Function to clear ODTs associated with STATIC DAQ lists.
 **
 ** This function clears specified ODTs by invoking Xcp_ClearODTEntries().
 ** Clearing of an ODT is in fact is the clearing of all ODT entries
 ** associated with the ODT.
 **
 ** \param[in] OdtPtr     Address of first ODT to be cleared
 ** \param[in] NoOfODTs   Number of ODTs to be cleared */
STATIC FUNC(void, XCP_CODE) Xcp_ClearODTs
(
  P2CONST(Xcp_OdtType, AUTOMATIC, XCP_APPL_DATA) ODTPtr,
  uint8 NoOfODTs
);
#endif /* (XCP_MIN_DAQ < XCP_NO_OF_NON_DYNAMIC_DAQ_LISTS) */
#endif /* (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC_MASK) */

/** \brief Function to Process SET_DAQ_LIST_MODE command request.
 **
 ** Use this function to execute a SET_DAQ_LIST_INFO command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdSetDAQListMode(void);
#endif /* ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) ||
        *  (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)) */

#if (XCP_USE_MEMORY_PROXY == STD_ON)
/** \brief Function to Process UPLOAD command request in a Bsw distributed context
 **
 ** Use this function to execute a UPLOAD command request in a multi-core context.  Present
 ** implementation of the UPLOAD command is to support a prior GET_ID command.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 ** \retval XCP_E_NOT_OK Command encountered an error.
 ** \retval XCP_E_BUSY Command needs more main function cycles in order to complete the task.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdUpload_BswDistribution(void);
#endif

/** \brief Function to Process UPLOAD command request.
 **
 ** Use this function to execute a UPLOAD command request.  Present
 ** implementation of the UPLOAD command is to support a prior GET_ID command.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 ** \retval XCP_E_NOT_OK Command encountered an error.
 ** \retval XCP_E_BUSY Command needs more main function cycles in order to complete the task.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdUpload(void);

/** \brief Function to Process TRANSPORT_LAYER_CMD command request.
 **
 ** Use this function to execute transport layer specific commands.  The
 ** commands to be executed are defined in the Transport Layer specification.
 ** Present implementation supports XCP on CAN specific command GET_SLAVE_ID .
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdTLSpecific(void);

/* If XCP on CAN is Enabled */
#if ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON))

/** \brief Function to Process XCP on CAN specific sub command GET_SLAVE_ID.
 **
 ** Use this function to execute transport layer specific commands
 ** GET_SLAVE_ID.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdGetSlaveIdOnCAN(void);

#endif

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)

/** \brief Function to Process XCP on Ethernet specific sub command GET_SLAVE_ID.
 **
 ** Use this function to execute transport layer specific commands
 ** GET_SLAVE_ID.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdGetSlaveIdOnEthernet(void);

#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */

#if (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)

/** \brief Function to Process XCP on FlexRay specific sub command FLX_ASSIGN.
 **
 ** Use this function to execute transport layer specific commands
 ** FLX_ASSIGN.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdFlxAssign(void);

/** \brief Function to reset the buffer
 ** \param[in]  FlxBuff - the buffer which needs to be reset.
 **
 ** \return Response that must be given by the calling function.
 **/
STATIC FUNC(uint8, XCP_CODE) Xcp_FlxAssignResetBuffer(uint8 FlxBuff);

/** \brief Function to validate the the parameters
 **
 ** \return whether the parameters are validated or not.
 ** \retval TRUE Parameters are OK, command can continue.
 ** \retval FALSE  Parameters are NOT OK, command should return NRC
 **/
STATIC FUNC(boolean, XCP_CODE) Xcp_FlxAssignParametersAreValidated(void);

/** \brief Function to Process the XCP_PACKET received from FLX_ASSIGN.
 **
 ** \param[in]  FlxBuff - the buffer for which this packet is meant
 ** \param[in]  XcpPacket - the packet type
 ** \return Execution status.
 ** \retval E_OK The Packet type was accepted and set.
 ** \retval E_NOT_OK The Packet type was not accepted.
 **/
STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_SetXcpPacket(uint8 FlxBuff, uint8 XcpPacket);

/** \brief Function to set XCP_PACKET received from FLX_ASSIGN with CMD support in case packet is Rx.
 **
 ** \param[in]  FlxBuff - the buffer for which this packet is meant
 ** \param[in]  XcpPacket - the packet type
 ** \return Execution status.
 ** \retval TRUE The Packet type was valid.
 ** \retval FALSE The Packet type was not valid.
 **/
STATIC FUNC(boolean, XCP_CODE) Xcp_AllowTxPacket (uint8 FlxBuff, uint8 XcpPacket);

/** \brief Function to set XCP_PACKET received from FLX_ASSIGN with RESERR support in case packet is Tx.
 **
 ** \param[in]  FlxBuff - the buffer for which this packet is meant
 ** \param[in]  XcpPacket - the packet type
 ** \return Execution status.
 ** \retval TRUE The Packet type was valid.
 ** \retval FALSE The Packet type was not valid.
 **/
STATIC FUNC(boolean, XCP_CODE) Xcp_AllowRxPacket (uint8 FlxBuff, uint8 XcpPacket);

/** \brief Function to validate the XCP_PACKET received from FLX_ASSIGN.
 **
 ** \param[in]  FlxBuff - the buffer for which this packet is meant
 ** \param[in]  XcpPacket - the packet type
 ** \return Execution status.
 ** \retval TRUE The Packet type was valid.
 ** \retval FALSE The Packet type was not valid.
 **/
STATIC FUNC(boolean, XCP_CODE) Xcp_ValidateXcpPacket(uint8 FlxBuff, uint8 XcpPacket);


/** \brief Function to reset the Flexray Buffer
 **
 ** \param[in]  FlxBuff - the buffer which needs to be reset
 ** \return Result of the operation
 ** \retval E_OK The buffer was restored successfully
 ** \retval E_NOT_OK The buffer was not restored successfully
 **/
STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_FlxRestoreBuffer(uint8 FlxBuff);

/** \brief Function to return the length of a buffer
 **
 ** \param[in]  FlxBuff - the buffer ID
 ** \return Buffer length (MAX_FLX_LEN_BUF_x).
 **/
STATIC FUNC(uint8, XCP_CODE) Xcp_GetFlxBufferLength(uint8 FlxBuff);

/** \brief Function to Process XCP on FlexRay specific sub command FLX_ACTIVATE.
 **
 ** Use this function to execute transport layer specific commands
 ** FLX_ACTIVATE.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdFlxActivate(void);

/** \brief Function to Process XCP on FlexRay specific sub command FLX_DEACTIVATE.
 **
 ** Use this function to execute transport layer specific commands
 ** FLX_DEACTIVATE.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdFlxDeActivate(void);

/** \brief Common function for FLX_ACTIVATE and FLX_DEACTIVATE.
 **
 ** This function shall be called by Xcp_ProcessSubCmdFlxActivate() and
 ** Xcp_ProcessSubCmdFlxDeActivate() upon execution
 **
 ** \param[in] Activate    TRUE if the function is called by Xcp_ProcessSubCmdFlxActivate()
 **                        and FALSE, otherwise
 **/
STATIC FUNC(void, XCP_CODE) Xcp_CommonFlxActivateDeactivate(boolean Activate);


/** \brief Function to Process XCP on FlexRay specific sub command SET_DAQ_FLX_BUF.
 **
 ** Use this function to execute transport layer specific commands
 ** SET_DAQ_FLX_BUF.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdFlxSetDaqListBuf(void);

/** \brief Function to Process XCP on FlexRay specific sub command GET_DAQ_FLX_BUF.
 **
 ** Use this function to execute transport layer specific commands
 ** GET_DAQ_FLX_BUF.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdFlxGetDaqListBuf(void);

#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) &&  (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON) */

/* If DAQ or STIM resource is supported */
#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) || \
     (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))

/* If XCP on CAN is Enabled */
#if (STD_ON == XCP_GET_DAQ_ID_SUPPORTED)

/** \brief Function to Process XCP on CAN specific sub command GET_DAQ_ID.
 **
 ** Use this function to execute transport layer specific commands
 ** GET_DAQ_ID.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdGetDaqID(void);

#endif /* STD_ON == XCP_GET_DAQ_ID_SUPPORTED */

/* If DaqList configuration type is Dynamic*/
#if (XCP_DAQ_COUNT > 0U)

/** \brief Function to clear the dynamic info block during initialization or FREE_DAQ command */
STATIC FUNC(void, XCP_CODE) Xcp_ClearDynamicInfoBlock(void);

#if (XCP_MIN_DAQ != XCP_MAX_DAQ)
/** \brief Function used to arrange all dynamic ODTs in the dynamic area.
 ** It will basically set, for each DAQ list, the pointer to the first ODT it has so that
 ** the ODTs will form a contiguous array. It will also set the FIRST_PID field for each DAQ list
 ** if necessary. */
STATIC FUNC(void, XCP_CODE) Xcp_ArrangeDynamicODTs(void);
#endif

/** \brief Function used to arrange all dynamic ODT entries in the dynamic area.
 ** It will basically set, for each ODT, the pointer to the first ODT entry it has so that
 ** the ODT entries will form a contiguous array. */
STATIC FUNC(void, XCP_CODE) Xcp_ArrangeDynamicODTEntries(void);

/** \brief Function to Process ALLOC_ODT_ENTRY command request.
 **
 ** Use this function to execute a ALLOC_ODT_ENTRY command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdAllocODTEntry(void);

/** \brief Function to Process ALLOC_ODT command request.
 **
 ** Use this function to execute a ALLOC_ODT command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdAllocODT(void);

/** \brief Function to Process ALLOC_DAQ command request.
 **
 ** Use this function to execute a ALLOC_DAQ command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdAllocDAQ(void);

/** \brief Function to Process FREE_DAQ command request.
 **
 ** Use this function to execute a FREE_DAQ command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdFreeDAQ(void);

#endif /* (XCP_DAQ_COUNT > 0U) */

/** \brief Function to Process SET_DAQ_PTR command request.
 **
 ** Use this function to execute a SET_DAQ_PTR command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdSetDAQPtr(void);

/** \brief Function to Process WRITE_DAQ command request.
 **
 ** Use this function to execute a WRITE_DAQ command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdWriteDAQ(void);

#if (XCP_WRITE_DAQ_MULTIPLE_SUPPORT == STD_ON)
/** \brief Function to Process WRITE_DAQ_MULTIPLE command request.
 **
 ** Use this function to execute a WRITE_DAQ_MULTIPLE command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdWriteDAQMultiple(void);
#endif


/** \brief Function to validate WRITE_DAQ or WRITE_DAQ_MULTIPLE commands.
 **
 ** \param[out] ResponseCTO    Pointer to the response which may be filled by this function
 **                            in case an error is detected
 **
 ** \return whether the commands passed validation.
 ** \retval TRUE   Command can proceed to be executed.
 ** \retval FALSE  Command must be rejected with an ERR.
 **/
STATIC FUNC(boolean, XCP_CODE)  Xcp_WriteDAQ_AreConditionsCorrect
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA)  ResponseCTO
);


/** \brief Function to write a single DAQ element
 ** To be used by WRITE_DAQ or WRITE_DAQ_MULTIPLE commands.
 **
 ** \param[out] ResponseCTO    Pointer to the response which may be filled by this function
 **                            in case an error is detected
 **
 ** \return whether the DAQ element was written successfully.
 ** \retval TRUE   DAQ element was written successfully
 ** \retval FALSE  DAQ element was not written successfully
 **/
STATIC FUNC(boolean, XCP_CODE)  Xcp_WriteDAQ_WriteOneODT
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA)  ResponseCTO,
  uint8 ReqDAQElementSize,
  uint32 ReqDAQElementAddress,
  uint8 BitPosition
);


#endif /*((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) ||
        * (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))*/

/* If user commands configured */
#if (XCP_NUMBER_OF_USERCOMMANDS > 0U)
/** \brief Function to Process User defined commands.
 **
 ** This is a generic function for executing user commands. After checking
 ** conditions for executing user command it will call a specific user
 ** defined call-out
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdUserDefined(void);
#endif /* (XCP_NUMBER_OF_USERCOMMANDS > 0U) */

#if (XCP_SEED_AND_KEY_ENABLED == STD_ON)

/** \brief Function to Process security access command GET_SEED.
 **
 ** Use this function to execute command GET_SEED.  This function use an
 ** external algorithm to generate seed.  External algorithm can be provided
 ** in file Xcp_Callouts.c.  If the requested resource is already unlocked the
 ** external algorithm wont be invoked.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetSeed(void);

/** \brief Function to Process security access command UNLOCK.
 **
 ** Use this function to execute command UNLOCK.  This function use an
 ** external algorithm to compare the received key.  External algorithm can be
 ** provided in file Xcp_Callouts.c.  If the key is valid the function will
 ** set the resource as unlocked.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdUnlock(void);

#endif /* (XCP_SEED_AND_KEY_ENABLED == STD_ON) */

/* If DAQ or STIM resource is supported */
#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) || \
     (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))

/** \brief Function to Process GET_DAQ_CLOCK command request.
 **
 ** Use this function to execute a GET_DAQ_CLOCK command request.  This
 ** function reads the time stamp counter value using the OS function call
 ** GetCounterValue().
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetDAQClock(void);

#endif /*((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) ||
        * (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))*/

#if (XCP_MAX_CTO > 8)
/** \brief Function to Process SHORT_DOWNLOAD command request.
 **
 ** Use this function to execute a SHORT_DOWNLOAD command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdShortDownload(void);
#endif /* XCP_MAX_CTO > 8 */

/** \brief Function to Process DOWNLOAD_MAX command request.
 **
 ** Use this function to execute a DOWNLOAD_MAX command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdDownloadMax(void);

/** \brief Function to Process DOWNLOAD command request.
 **
 ** Use this function to execute a DOWNLOAD command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdDownload(void);


#if (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)
/** \brief Function to Process DOWNLOAD_NEXT command request.
 **
 ** Use this function to execute a DOWNLOAD_NEXT command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdDownloadNext(void);
#endif

/** \brief Function to Process one download request.
 **
 ** This function is used by DOWNLOAD and DOWNLOAD_NEXT commands and will fill the download buffer
 ** with incoming elements from the master. If the download sequence is completed, it will copy the
 ** download buffer into the slave's memory pointed by the MTA.
 **
 ** \param[out] ResponseCTO    Pointer to the response which may be filled by this function call if
 **                           it is the last download from the sequence
 ** \param[in] IndexInCmdQueueOfFirstElement: Index in the CMD queue of the first element to be downloaded
 ** \param[in] MaxDownloadElements: Maximum elements that can be downloaded to the slave in one command
 */
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessOneDownload
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA)  ResponseCTO,
  uint8 IndexInCmdQueueOfFirstElement,
  uint8 MaxDownloadElements
);

/** \brief Function to Process SHORT_UPLOAD command request.
 **
 ** Use this function to execute a SHORT_UPLOAD command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdShortUpload(void);

/** \brief Function to Process SET_MTA command request.
 **
 ** Use this function to execute a SET_MTA command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdSetMTA(void);

#if (XCP_RESOURCE_CAL_PAG == XCP_RESOURCE_CAL_PAG_MASK)
/** \brief Function to process COPY_CAL_PAGE command request.
 **
 ** Use this function to execute a COPY_CAL_PAGE command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdCopyCalPage(void);

/** \brief Function to process GET_PAG_PROCESSOR_INFO command request.
 **
 ** Use this function to execute a GET_PAG_PROCESSOR_INFO command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetPagProcInfo(void);

/** \brief Function to process GET_CAL_PAGE command request.
 **
 ** Use this function to execute a GET_CAL_PAGE command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetCalPage(void);

/** \brief Function to process SET_CAL_PAGE command request.
 **
 ** Use this function to execute a SET_CAL_PAGE command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdSetCalPage(void);

/** \brief Function to process GET_SEGMENT_MODE command request.
 **
 ** Use this function to execute a GET_SEGMENT_MODE command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetSegmentMode(void);

/** \brief Function to process SET_SEGMENT_MODE command request.
 **
 ** Use this function to execute a SET_SEGMENT_MODE command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdSetSegmentMode(void);

/** \brief Function to process GET_SEGMENT_INFO command request.
 **
 ** Use this function to execute a GET_SEGMENT_INFO command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetSegmentInfo(void);

/** \brief Function to process GET_PAGE_INFO command request.
 **
 ** Use this function to execute a GET_PAGE_INFO command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetPageInfo(void);

#endif /* (XCP_RESOURCE_CAL_PAG == XCP_RESOURCE_CAL_PAG_MASK) */

#if (XCP_BUILD_CHECKSUM_SUPPORT == STD_ON)
/** \brief Function to process BUILD_CHECKSUM command request.
 **
 ** Use this function to execute a BUILD_CHECKSUM command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdBuildChecksum(void);
#endif

#if (XCP_MODIFY_BITS_SUPPORTED == STD_ON)
/** \brief Function to Process MODIFY_BITS command request.
 **
 ** This function shall execute a MODIFY_BITS command request.
 **
 ** \return Command execution status.
 ** \retval E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdModifyBits(void);

#if ((XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON) || (XCP_USE_MEMORY_PROXY == STD_ON))
/** \brief Function to prepare data for MODIFY_BITS command request to MemoryProxy.
 **
 **/
STATIC FUNC(void, XCP_CODE) Xcp_PrepareDataForRequest (Xcp_MemoryProxyDataType * Data, uint8 Shift_Value, uint16 And_Mask, uint16 Xor_Mask, uint8 MemAreaIdx);
#endif
#endif

#if ((XCP_STORE_DAQ_SUPPORTED == STD_ON) || \
     (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON))
/** \brief Function to process SET_REQUEST command request.
 **
 ** Use this function to execute a SET_REQUEST command request.
 **
 ** \return Command execution status.
 ** \retval XCP_E_OK   Command executed successfully.
 **/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdSetRequest(void);
#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) || (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON) */

#if (XCP_STORE_DAQ_SUPPORTED == STD_ON)
/**
 ** \brief Function to handle a request to store the DAQ lists
 **  This function is used by SET_REQUEST command handler to perform the storing task.
 ** \param[in]  RequestedMode - the requested mode of how to store DAQ (Resume or no Resume)
 ** \param[out] ResponseCTO  -  Pointer to the response which will be filled upon the execution of
 **                             this function
 **
 **  */
STATIC FUNC(void, XCP_CODE) Xcp_StartStoreDaq
(
  uint8 RequestedMode,
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA) ResponseCTO
);

/**
 ** \brief Function to handle a request to clear the DAQ lists
 **  This function is used by SET_REQUEST command handler to perform the clearing task.
 **
 ** \param[out] ResponseCTO  -  Pointer to the response which will be filled upon the execution of
 **                             this function
 **
 **  */
STATIC FUNC(void, XCP_CODE) Xcp_StartClearDaq
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA) ResponseCTO
);

#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) */

#if (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON)
/**
 ** \brief Function to handle a request to store the calibration data in the non volatile memory
 **  This function is used by SET_REQUEST command handler to perform the storing task.
 **
 ** \param[out] ResponseCTO  -  Pointer to the response which will be filled upon the execution of
 **                             this function
 **
 **  */
STATIC FUNC(void, XCP_CODE) Xcp_StartStoreCal
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA) ResponseCTO
);

#endif /* (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON) */


/**
 ** \brief Tries to send an error message and will immediately dequeue the command.
 **
 ** \param[in] ErrResponseCTOPtr  -  Pointer to the response which will be filled by this function
 **                               caller
 **
 **  */
STATIC FUNC(void, XCP_CODE) Xcp_AbortCommandWithError
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA)  ErrResponseCTOPtr
);

/**
 ** \brief Function to check if a command is OK to be executed in the current context
 **
 ** \param[in] CommandId - The PID of the command to be validated
 ** \param[out] ErrResponseCTOPtr  -  Pointer to the response which will get filled if the validation
 ** fails. If the validation passes, the response will not be used by the calling function.
 **
 ** \return Whether the command can be executed
 ** \retval TRUE   Command can be executed
 ** \retval FALSE  Command cannot be executed
 **
 **  */
STATIC FUNC(boolean, XCP_CODE) Xcp_CommandCanBeExecuted
(
  uint8 CommandId,
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA)  ErrResponseCTOPtr
);

/**
 ** \brief This function enqueues the XCP_EV_CMD_PENDING event if the
 **        parameter is XCP_E_BUSY.
 ** \param[in] ret - The PID of the command to be validated
 **
 **  */
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF)
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
STATIC FUNC(void, XCP_CODE) Xcp_SendCmdPendingEventCto(Xcp_ErrorType ret);
#else
#define Xcp_SendCmdPendingEventCto(ret)
#endif
#endif

#if (XCP_MODIFY_BITS_SUPPORTED == STD_ON)
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF)
/**
 ** \brief Function used for reading, modifying and than writing the modified value
 **  for the modify bits command.
 **
 ** \param[in] ResponseCTO  -  Pointer to the response which will be filled by this function
 **                               caller
 **
 **  */
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessModifyBits
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA) ResponseCTO
);
#endif
#endif

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

#define XCP_START_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

/** \brief The commands handlers and additional information regarding the command execution needs
 **   - Command function pointer
 **   - Base length (number of bytes)
 **     "Base length" means the packet length without any headers
 **     or tails that are transport layer or configuration specific.
 **     It is used for calculating the lengths of the packets received via FlexRay.
 **   - Command maximum response length
 **
 ** NOTE:
 ** 1) Some commands have common handling or need to calculate their length
 **    based on the message content. In these cases (e.g. DOWNLOAD)
 **    the XCP_COMMAND_LENGTH_CALCULATED ID is used to identify the special case
 **    and calculate the real length out of it.
 ** 2) Array uses magic numbers as they can directly be compared with the ASAM
 **    specification and they are not reused anywhere in the module which makes
 **    the overhead for constants questionable at least.
 */
STATIC CONST(Xcp_Commands_t, XCP_CONST) Xcp_Commands[XCP_NR_OF_XCP_COMMAND_CODES] =
{
  /* 0xC0 */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xC1 */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xC2 */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xC3 */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xC4 */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xC5 */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xC6 */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#if (XCP_WRITE_DAQ_MULTIPLE_SUPPORT == STD_ON)
  /* 0xC7 */
  {
    &Xcp_ProcessCmdWriteDAQMultiple,      /* Command handler pointer */
    0U,                                   /* Base Length - will be calculated */
    2U,                                   /* Command maximum response length */
  },
#else
  /* 0xC7 */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif /* (XCP_WRITE_DAQ_MULTIPLE_SUPPORT == STD_ON)*/
#if (XCP_PGM_SUPPORTED == STD_ON)
  /* 0xC8 */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xC9 - PROGRAM_MAX */
  {
    &Xcp_ProcessCmdProgramMax,            /* Command handler pointer */
    XCP_MAX_CTO_PGM,                      /* Base Length */
    2U,                                   /* Command maximum response length */
  },
#if (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)
  /* 0xCA - PROGRAM_NEXT */
  {
    &Xcp_ProcessCmdProgramNext,           /* Command handler pointer */
    XCP_COMMAND_LENGTH_CALCULATED,        /* Base Length */
    2U,                                   /* Command maximum response length */
  },
#else
  /* 0xCA  */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif /* (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON) */
  /* 0xCB */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xCC */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xCD - GET_SECTOR_INFO */
  {
    &Xcp_ProcessCmdGetSectorInfo,         /* Command handler pointer */
    3U,                                   /* Base Length */
    XCP_CMD_SECTOR_INFO_MAX_RES_LENGTH,   /* Command maximum response length */
  },
  /* 0xCE - GET_PGM_PROCESSOR_INFO */
  {
    &Xcp_ProcessCmdGetPgmProcInfo,        /* Command handler pointer */
    1U,                                   /* Base Length */
    XCP_CMD_PGM_PROC_INFO_RES_LENGTH,     /* Command maximum response length */
  },
  /* 0xCF - PROGRAM_RESET */
  {
    &Xcp_ProcessCmdProgramReset,          /* Command handler pointer */
    1U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
  /* 0xD0 - PROGRAM */
  {
    &Xcp_ProcessCmdProgram,                /* Command handler pointer */
    XCP_COMMAND_LENGTH_CALCULATED,        /* Base Length */
    2U,                                   /* Command maximum response length */
  },
  /* 0xD1 - PROGRAM_CLEAR */
  {
    &Xcp_ProcessCmdProgramClear,          /* Command handler pointer */
    8U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
  /* 0xD2 - PROGRAM_START */
  {
    &Xcp_ProcessCmdProgramStart,          /* Command handler pointer */
    1U,                                   /* Base Length */
    7U,                                   /* Command maximum response length */
  },
  #else /* no programming commands available */
  /* 0xC8 */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xC9 */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xCA */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xCB */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xCC */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xCD */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xCE */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xCF */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xD0 */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xD1 */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xD2 */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif /* (XCP_PGM_SUPPORTED == STD_ON) */

/* If DAQ or STIM resource is supported */
#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) || \
     (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))
#if (XCP_DAQ_COUNT > 0U)
  /* 0xD3 - ALLOC_ODT_ENTRY */
  {
    &Xcp_ProcessCmdAllocODTEntry,         /* Command handler pointer */
    6U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
  /* 0xD4 - ALLOC_ODT */
  {
    &Xcp_ProcessCmdAllocODT,              /* Command handler pointer */
    5U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
  /* 0xD5 - ALLOC_DAQ */
  {
    &Xcp_ProcessCmdAllocDAQ,              /* Command handler pointer */
    4U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
  /* 0xD6 - FREE_DAQ */
  {
    &Xcp_ProcessCmdFreeDAQ,               /* Command handler pointer */
    1U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
#else /* Dynamic configuration is not supported */
  /*  0xD3 - ALLOC_ODT_ENTRY */
  {
    NULL_PTR,                             /* Command handler pointer */
    6U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xD4 - ALLOC_ODT */
  {
    NULL_PTR,                             /* Command handler pointer */
    5U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xD5 - ALLOC_DAQ */
  {
    NULL_PTR,                             /* Command handler pointer */
    4U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xD6 - FREE_DAQ */
  {
    NULL_PTR,                             /* Command handler pointer */
    1U,                                 /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif /* (XCP_DAQ_COUNT > 0U) */
  /* 0xD7 - GET_DAQ_EVENT_INFO */
  {
    &Xcp_ProcessCmdGetDAQEventInfo,        /* Command handler pointer */
    4U,                                   /* Base Length */
    7U,                                   /* Command maximum response length */
  },
/* This command is available only for static DAQ lists */
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC_MASK)
  /* 0xD8 - GET_DAQ_LIST_INFO */
  {
    &Xcp_ProcessCmdGetDAQListInfo,         /* Command handler pointer */
    4U,                                   /* Base Length */
    6U,                                   /* Command maximum response length */
  },
#else /* Static configuration is not supported */
  /* 0xD8 - GET_DAQ_LIST_INFO */
  {
    NULL_PTR,                             /* Command handler pointer */
    4U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif /* (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC_MASK) */
  /* 0xD9 - GET_DAQ_RESOLUTION_INFO */
  {
    &Xcp_ProcessCmdGetDAQResolutionInfo,   /* Command handler pointer */
    1U,                                   /* Base Length */
    8U,                                   /* Command maximum response length */
  },
  /* 0xDA - GET_DAQ_PROCESSOR_INFO */
  {
    &Xcp_ProcessCmdGetDAQProcessorInfo,    /* Command handler pointer */
    1U,                                   /* Base Length */
    8U,                                   /* Command maximum response length */
  },
  /* 0xDB - READ_DAQ  */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xDC - GET_DAQ_CLOCK */
  {
    &Xcp_ProcessCmdGetDAQClock,            /* Command handler pointer */
    1U,                                   /* Base Length */
    8U,                                   /* Command maximum response length */
  },
  /* 0xDD - START_STOP_SYNCH */
  {
    &Xcp_ProcessCmdStartStopSynch,         /* Command handler pointer */
    2U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
  /* 0xDE - START_STOP_DAQ_LIST */
  {
    &Xcp_ProcessCmdStartStopDAQList,       /* Command handler pointer */
    4U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
  /* 0xDF - GET_DAQ_LIST_MODE */
  {
    &Xcp_ProcessCmdGetDAQListMode,         /* Command handler pointer */
    4U,                                   /* Base Length */
    8U,                                   /* Command maximum response length */
  },
  /* 0xE0 - SET_DAQ_LIST_MODE */
  {
    &Xcp_ProcessCmdSetDAQListMode,         /* Command handler pointer */
    8U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
  /* 0xE1 - WRITE_DAQ */
  {
    &Xcp_ProcessCmdWriteDAQ,               /* Command handler pointer */
    8U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
  /* 0xE2 - SET_DAQ_PTR */
  {
    &Xcp_ProcessCmdSetDAQPtr,              /* Command handler pointer */
    6U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC_MASK)
  /* 0xE3 - CLEAR_DAQ_LIST */
  {
    &Xcp_ProcessCmdClearDAQList,           /* Command handler pointer */
    4U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
#else /* Static configuration is not supported */
  /* 0xE3 - CLEAR_DAQ_LIST */
  {
    NULL_PTR,                             /* Command handler pointer */
    4U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif /* (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC_MASK) */
#else /* ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) ||
          (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)) */
  /* 0xD3 - ALLOC_ODT_ENTRY */
  {
    NULL_PTR,                             /* Command handler pointer */
    6U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xD4 - ALLOC_ODT */
  {
    NULL_PTR,                             /* Command handler pointer */
    5U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xD5 - ALLOC_DAQ */
  {
    NULL_PTR,                             /* Command handler pointer */
    4U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xD6 - FREE_DAQ */
  {
    NULL_PTR,                             /* Command handler pointer */
    1U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xD7 - GET_DAQ_EVENT_INFO */
  {
    NULL_PTR,                             /* Command handler pointer */
    4U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xD8 - GET_DAQ_LIST_INFO */
  {
    NULL_PTR,                             /* Command handler pointer */
    4U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xD9 - GET_DAQ_RESOLUTION_INFO */
  {
    NULL_PTR,                             /* Command handler pointer */
    1U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xDA - GET_DAQ_PROCESSOR_INFO */
  {
    NULL_PTR,                             /* Command handler pointer */
    1U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xDB - READ_DAQ */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xDC - GET_DAQ_CLOCK */
  {
    NULL_PTR,                             /* Command handler pointer */
    1U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xDD - START_STOP_SYNCH */
  {
    NULL_PTR,                             /* Command handler pointer */
    2U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xDE - START_STOP_DAQ_LIST */
  {
    NULL_PTR,                             /* Command handler pointer */
    4U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xDF - GET_DAQ_LIST_MODE */
  {
    NULL_PTR,                             /* Command handler pointer */
    4U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xE0 - SET_DAQ_LIST_MODE */
  {
    NULL_PTR,                             /* Command handler pointer */
    8U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xE1 - WRITE_DAQ */
  {
    NULL_PTR,                             /* Command handler pointer */
    8U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xE2 - SET_DAQ_PTR */
  {
    NULL_PTR,                             /* Command handler pointer */
    6U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xE3 - CLEAR_DAQ_LIST */
  {
    NULL_PTR,                             /* Command handler pointer */
    4U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif /* ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) ||
          (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)) */

#if (XCP_RESOURCE_CAL_PAG == XCP_RESOURCE_CAL_PAG_MASK)
  /* 0xE4 - COPY_CAL_PAGE */
  {
    &Xcp_ProcessCmdCopyCalPage,           /* Command handler pointer */
    5U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
  /* 0xE5 - GET_SEGMENT_MODE */
  {
    &Xcp_ProcessCmdGetSegmentMode,        /* Command handler pointer */
    3U,                                   /* Base Length */
    3U,                                   /* Command maximum response length */
  },
  /* 0xE6 - SET_SEGMENT_MODE */
  {
    &Xcp_ProcessCmdSetSegmentMode,        /* Command handler pointer */
    3U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
  /* 0xE7 - GET_PAGE_INFO */
  {
    &Xcp_ProcessCmdGetPageInfo,           /* Command handler pointer */
    4U,                                   /* Base Length */
    3U,                                   /* Command maximum response length */
  },
  /* 0xE8 - GET_SEGMENT_INFO */
  {
    &Xcp_ProcessCmdGetSegmentInfo,        /* Command handler pointer */
    5U,                                   /* Base Length */
    8U,                                   /* Command maximum response length */
  },
  /* 0xE9 - GET_PAG_PROCESSOR_INFO */
  {
    &Xcp_ProcessCmdGetPagProcInfo,        /* Command handler pointer */
    1U,                                   /* Base Length */
    3U,                                   /* Command maximum response length */
  },
  /* 0xEA - GET_CAL_PAGE */
  {
    &Xcp_ProcessCmdGetCalPage,            /* Command handler pointer */
    3U,                                   /* Base Length */
    4U,                                   /* Command maximum response length */
  },
  /* 0xEB - SET_CAL_PAGE */
  {
    &Xcp_ProcessCmdSetCalPage,            /* Command handler pointer */
    4U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
#else /* XCP_RESOURCE_CAL_PAG != XCP_RESOURCE_CAL_PAG_MASK */
  /* 0xE4 - COPY_CAL_PAGE */
  {
    NULL_PTR,                             /* Command handler pointer */
    5U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xE5 - GET_SEGMENT_MODE */
  {
    NULL_PTR,                             /* Command handler pointer */
    3U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xE6 - SET_SEGMENT_MODE */
  {
    NULL_PTR,                             /* Command handler pointer */
    3U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xE7 - GET_PAGE_INFO */
  {
    NULL_PTR,                             /* Command handler pointer */
    4U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xE8 - GET_SEGMENT_INFO */
  {
    NULL_PTR,                             /* Command handler pointer */
    5U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xE9 - GET_PAG_PROCESSOR_INFO */
  {
    NULL_PTR,                             /* Command handler pointer */
    1U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xEA - GET_CAL_PAGE */
  {
    NULL_PTR,                             /* Command handler pointer */
    3U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xEB - SET_CAL_PAGE */
  {
    NULL_PTR,                             /* Command handler pointer */
    4U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif /* (XCP_RESOURCE_CAL_PAG == XCP_RESOURCE_CAL_PAG_MASK) */
#if (XCP_MODIFY_BITS_SUPPORTED == STD_ON)
  /* 0xEC - MODIFY_BITS */
  {
    &Xcp_ProcessCmdModifyBits,            /* Command handler pointer */
    6U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
#else
  /* 0xEC - MODIFY_BITS */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif /* (XCP_MODIFY_BITS_SUPPORTED == STD_ON) */
#if (XCP_MAX_CTO > 8)
  /* 0xED */
  {
    &Xcp_ProcessCmdShortDownload,         /* Command handler pointer */
    9U,                                   /* Base Length */
    3U,                                   /* Command maximum response length */
  },
#else
  /* 0xED */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_UNSUPPORTED_CMD_LENGTH,           /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif /* (XCP_MAX_CTO > 8) */
  /* 0xEE */
  {
    &Xcp_ProcessCmdDownloadMax,            /* Command handler pointer */
    XCP_MIN_OF_ALL_MAX_CTO,               /* Base Length */
    3U,                                   /* Command maximum response length */
  },
#if (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)
  /* 0xEF - DOWNLOAD_NEXT */
  {
    &Xcp_ProcessCmdDownloadNext,          /* Command handler pointer */
    XCP_COMMAND_LENGTH_CALCULATED,        /* Base Length */
    3U,                                   /* Command maximum response length */
  },
#else /* Master block mode is not available */
  /* 0xEF - DOWNLOAD_NEXT */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_COMMAND_LENGTH_CALCULATED,        /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif /* (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) */
  /* 0xF0 - DOWNLOAD */
  {
    &Xcp_ProcessCmdDownload,              /* Command handler pointer */
    XCP_COMMAND_LENGTH_CALCULATED,        /* Base Length */
    2U,                                   /* Command maximum response length */
  },
#if (XCP_NUMBER_OF_USERCOMMANDS > 0U)
  /* 0xF1 - USER_CMD  */
  {
    &Xcp_ProcessCmdUserDefined,           /* Command handler pointer */
    2U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
#else /* XCP_NUMBER_OF_USERCOMMANDS > 0U */
  /* 0xF1 - USER_CMD  */
  {
    NULL_PTR,                             /* Command handler pointer */
    1U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif /* (XCP_NUMBER_OF_USERCOMMANDS > 0U) */
  /* 0xF2 - TRANSPORT_LAYER_CMD */
  {
    &Xcp_ProcessCmdTLSpecific,            /* Command handler pointer */
    XCP_COMMAND_LENGTH_CALCULATED,        /* Base Length */
    8U,                                   /* Command maximum response length */
  },
#if (XCP_BUILD_CHECKSUM_SUPPORT == STD_ON)
  /* 0xF3 - BUILD_CHECKSUM */
  {
    &Xcp_ProcessCmdBuildChecksum,         /* Command handler pointer */
    8U,                                   /* Base Length */
    8U,                                   /* Command maximum response length */
  },
#else /* BUILD_CHECKSUM is not supported */
  /* 0xF3 - BUILD_CHECKSUM  */
  {
    NULL_PTR,                             /* Command handler pointer */
    8U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif /* (XCP_BUILD_CHECKSUM_SUPPORT == STD_ON) */
  /*  0xF4 - SHORT_UPLOAD */
  {
    &Xcp_ProcessCmdShortUpload,           /* Command handler pointer */
    8U,                                   /* Base Length */
    XCP_MAX_CTO                           /* Command maximum response length */
  },
  /* 0xF5 - UPLOAD */
  {
#if (XCP_USE_MEMORY_PROXY == STD_ON)
    &Xcp_ProcessCmdUpload_BswDistribution,/* Command handler pointer */
#else
    &Xcp_ProcessCmdUpload,                /* Command handler pointer */
#endif
    2U,                                   /* Base Length */
    XCP_MAX_CTO,                          /* Command maximum response length */
  },
  /* 0xF6 - SET_MTA */
  {
    &Xcp_ProcessCmdSetMTA,                /* Command handler pointer */
    8U,                                   /* Base Length */
    1U,                                   /* Command maximum response length */
  },
#if (XCP_SEED_AND_KEY_ENABLED == STD_ON)
  /* 0xF7 - UNLOCK */
  {
    &Xcp_ProcessCmdUnlock,                /* Command handler pointer */
    XCP_COMMAND_LENGTH_CALCULATED,        /* Base Length */
    2U,                                   /* Command maximum response length */
  },
  /* 0xF8 - GET_SEED */
  {
    &Xcp_ProcessCmdGetSeed,               /* Command handler pointer */
    3U,                                   /* Base Length */
    XCP_MAX_CTO,                          /* Command maximum response length */
  },
#else /* XCP_SEED_AND_KEY_ENABLED == STD_OFF */
  /* 0xF7 - UNLOCK */
  {
    NULL_PTR,                             /* Command handler pointer */
    XCP_COMMAND_LENGTH_CALCULATED,        /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /* 0xF8 - GET_SEED */
  {
    NULL_PTR,                             /* Command handler pointer */
    3U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif /* (XCP_SEED_AND_KEY_ENABLED == STD_ON) */

#if ((XCP_STORE_DAQ_SUPPORTED == STD_ON) || \
     (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON))
  /*  0xF9 - SET_REQUEST */
  {
    &Xcp_ProcessCmdSetRequest,            /* Command handler pointer */
    4U,                                   /* Base Length */
    2U,                                   /* Command maximum response length */
  },
#else
  /*  0xF9 - SET_REQUEST */
  {
    NULL_PTR,                             /* Command handler pointer */
    4U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
#endif
  /* 0xFA - GET_ID */
  {
    &Xcp_ProcessCmdGetId,                 /* Command handler pointer */
    2U,                                   /* Base Length */
#if (XCP_GET_ID_TYPE1_CALLOUT_ENABLED == STD_ON)
#if ( (XCP_CMD_GETID_RES_LENGTH_MIN + XCP_GET_ID_TYPE1_MAX_LENGTH) < XCP_MAX_CTO )
                                          /* Command maximum response length */
    (XCP_CMD_GETID_RES_LENGTH_MIN + XCP_GET_ID_TYPE1_MAX_LENGTH),
#else
    XCP_MAX_CTO,                          /* Command maximum response length */
#endif
#else /* (XCP_GET_ID_TYPE1_CALLOUT_ENABLED == STD_OFF) */
#if ( (XCP_CMD_GETID_RES_LENGTH_MIN + XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH) < XCP_MAX_CTO )
                                          /* Command maximum response length */
    (XCP_CMD_GETID_RES_LENGTH_MIN + XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH),
#else
    XCP_MAX_CTO,                          /* Command maximum response length */
#endif
#endif /* (XCP_GET_ID_TYPE1_CALLOUT_ENABLED == STD_ON) */
  },
  /* 0xFB - GET_COMM_MODE_INFO */
  {
    &Xcp_ProcessCmdGetCommModeInfo,       /* Command handler pointer */
    1U,                                   /* Base Length */
    8U,                                   /* Command maximum response length */
  },
  /*  0xFC - SYNCH Implemented without handling function */
  {
    NULL_PTR,                             /* Command handler pointer */
    1U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /*  0xFD - GET_STATUS */
  {
    &Xcp_ProcessCmdGetStatus,             /* Command handler pointer */
    1U,                                   /* Base Length */
    6U,                                   /* Command maximum response length */
  },
  /*  0xFE - DISCONNECT : Handled directly, with out inserting in to command queue */
  {
    NULL_PTR,                             /* Command handler pointer */
    1U,                                   /* Base Length */
    XCP_UNSUPPORTED_CMD_RES_LENGTH,       /* Command maximum response length */
  },
  /*  0xFF - CONNECT */
  {
    &Xcp_ProcessCmdConnect,               /* Command handler pointer */
    2U,                                   /* Base Length */
    8U,                                   /* Command maximum response length */
  },
};

#define XCP_STOP_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

#define XCP_START_SEC_CONST_8
#include <Xcp_MemMap.h>

#if (XCP_GET_ID_TYPE1_CALLOUT_ENABLED == STD_OFF)
#if (XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH > 0)
/** \brief ASAM-MC2 filename without path and extension.
 *
 * This is information for identification type 1: ASAM-MC2 filename without
 * path and extension. */
/* Deviation MISRAC2012-5 <START> */
STATIC CONST(uint8, XCP_CONST)
  Xcp_IdentificationTypeOne[XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH]
  = {XCP_GET_ID_TYPE1_ASAM_FILENAME};
/* Deviation MISRAC2012-5 <STOP> */
#endif /* if (XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH > 0) */
#endif /* if (XCP_GET_ID_TYPE1_CALLOUT_ENABLED == STD_OFF) */

#define XCP_STOP_SEC_CONST_8
#include <Xcp_MemMap.h>

/*==================[external data]=========================================*/

#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
VAR(Atomic_t, XCP_VAR) Xcp_RAMCalloutProxyState;
#endif

#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>
/** \brief All current status information of the slave device.
 *
 * This includes the status of the resource protection, pending store requests
 * the general status of data acquisition and stimulation. */
VAR(Xcp_StatusType, XCP_VAR_CLEARED) Xcp_Status;

#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
/** \brief Definition of Event_CTO queue.
 *
 * The Event_CTO queue is implemented as byte array. */
VAR(Xcp_EventCTOQueueType, XCP_VAR_CLEARED) Xcp_EventCTOQueue;
#endif /* (XCP_EVENT_PACKET_ENABLED == STD_ON) */

#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>

/*==================[internal data]=========================================*/

#define XCP_START_SEC_VAR_16
#include <Xcp_MemMap.h>

#if (XCP_SLAVE_BLOCK_MODE == XCP_SLAVE_BLOCK_MODE_MASK)
/** \brief  Variable to hold the number of the data bytes we still need to send in a block mode
 * upload sequence */
/* Deviation MISRAC2012-5 <START> */
STATIC VAR(uint16, XCP_VAR) Xcp_RemainingDataBytesToUpload = 0U;
/* Deviation MISRAC2012-5 <STOP> */
#endif

#define XCP_STOP_SEC_VAR_16
#include <Xcp_MemMap.h>

#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>

/** \brief Memory writing related information */
STATIC VAR(Xcp_MemoryWriteType, XCP_VAR_CLEARED) Xcp_MemoryWrite;

/** \brief Definition of CTO queue.
 *
 * The CTO queue is implemented as byte array. */
STATIC VAR(Xcp_CTOQueueType, XCP_VAR_CLEARED) Xcp_CtoQueue;

#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>


#define XCP_START_SEC_VAR_CLEARED_8
#include <Xcp_MemMap.h>

#if (XCP_USE_MEMORY_PROXY == STD_ON)
static uint8 Xcp_MemoryProxyBufferIndex;
#endif

/** \brief Command processor state machine state */
STATIC VAR(Xcp_ProcessorStateType, XCP_VAR_CLEARED) Xcp_CmdProcessorState;

/** \brief Definition of Command queue.
 *
 * The command queue is implemented as array of CTO structures. */
STATIC VAR(Xcp_CMDQueueType, XCP_VAR_CLEARED) Xcp_CommandQueue;

/** \brief Command status bit flags.
 *
 * Bit position 0:
 *         1 - SYNCH command received
 *         0 - No pending SYNCH command
 * Bit position 1:
 *         1 - Transport layer sub-command GET_SLAVE_ID with mode = "identify by echo" is received.
 *         0 - Transport layer sub-command GET_SLAVE_ID with mode = "identify by echo" not received.
 * Bit position 2:
 *         1 - DISCONNECT command received
 *         0 - No pending DISCONNECT command
 * Bit position 3:
 *         1 - BUSY command was aborted
 *         0 - BUSY command was not aborted
 * Bit position 4:
 *         1 - DOWNLOAD command pending
 *         0 - No pending DOWNLOAD command
 * Bit position 5:
 *         1 - PROGRAM_START command received
 *         0 - No PROGRAM_START command received or PROGRAM_RESET received in the meantime
 * Bit position 6:
 *         1 - First PROGRAM/_MAX command received, thus the programming of a segment is active
 *         0 - A PROGRAM with no. of elements = 0 was received, thus programming is inactive
 * Bit position 7:
 *         1 - Programming is in block mode transfer
 *         0 - Programming is not in block mode
 *
 */
STATIC VAR(uint8, XCP_VAR_CLEARED) Xcp_CmdReqStatus;

#if (XCP_PGM_SUPPORTED == STD_ON)

/** \brief Command status extension bit flags.
 *
 * Bit position 0:
 *         1 - Abort Programming session is set
 *         0 - No action to attend
 * Bit position 1..7: RESERVED
 *
 */
STATIC VAR(uint8, XCP_VAR_CLEARED) Xcp_CmdReqStatusExtension;
#endif

#if (XCP_GET_ID_TYPE1_CALLOUT_ENABLED == STD_ON)
/** \brief Buffer where the Identification information retrieved from user callout
 *  is stored. */
/* Deviation MISRAC2012-5 <START> */
STATIC VAR(uint8, XCP_VAR) Xcp_GetIdBuffer[XCP_GET_ID_TYPE1_MAX_LENGTH];
/* Deviation MISRAC2012-5 <STOP> */
#endif

#define XCP_STOP_SEC_VAR_CLEARED_8
#include <Xcp_MemMap.h>


#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>

#if (XCP_BUILD_CHECKSUM_SUPPORT == STD_ON)
/** \brief Build Checksum data structure used to exchange data,
 **        between the two tasks used for asynchronous crc calculation. */
STATIC VAR(Xcp_BuildChecksumDescriptor_t, XCP_VAR_CLEARED) Xcp_BuildChecksumData;
#endif

/** \brief Memory transfer address information */
STATIC VAR(Xcp_MTA_t, XCP_VAR_CLEARED) Xcp_MTA;

/* If Seed and Key mechanism is required */
#if (XCP_SEED_AND_KEY_ENABLED == STD_ON)
/** \brief Security Access information */
STATIC VAR(Xcp_SecAccess_t, XCP_VAR_CLEARED) Xcp_SecAccess;
#endif


#if (XCP_DAQ_COUNT > 0U)
/** \brief Runtime info regarding the configured Dynamic DAQ lists */
VAR(Xcp_DynamicDaqListsType, XCP_VAR_CLEARED) Xcp_DynamicInfo;
#endif /* (XCP_DAQ_COUNT > 0U) */

#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>

#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

/* If DAQ or STIM resource is supported */
#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) || \
     (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))

/** \brief Global variable to hold DAQ List pointer
 *  This variable holds ODT entry pointer defined by the command SET_DAQ_PTR.
 *  This variable will be used to service commands WRITE_DAQ and READ_DAQ.
 */
STATIC VAR(Xcp_DAQPtr_t, XCP_VAR) Xcp_DAQPointer =
{
  XCP_MAX_DAQ,
  XCP_MAXIMUM_ODT,
  XCP_MAX_ENTRIES_PER_ODT
};


#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
/** \brief Structure holding the DAQ List ID for which each FlexRay Buffer is assigned to */
STATIC VAR(Xcp_FlxBufRuntimeType, XCP_VAR) Xcp_FlxDaqList[XCP_NO_OF_FLX_BUF_CONFIGURED];
#endif


#endif /* ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) ||
           (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)) */

#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

/*==================[external function definitions]=========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

#if (XCP_ASYNCH_CRC_SUPPORT == STD_ON)
/*--------------------[Xcp_GetBuildChecksumReqStatus]----------------------*/
FUNC(Std_ReturnType, XCP_CODE) Xcp_GetBuildChecksumReqStatus(void)
{
  /* Local variable to hold return value */
  Std_ReturnType RetVal;

  DBG_XCP_GETBUILDCHECKSUMREQSTATUS_ENTRY();

  /* Enter critical section.*/
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  RetVal = ((Xcp_BuildChecksumData.IsRequestCRCActive == FALSE) ? E_NOT_OK : E_OK);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_GETBUILDCHECKSUMREQSTATUS_EXIT(RetVal);

  return RetVal;
}
#endif

#if (XCP_PGM_SUPPORTED == STD_ON)
/*-----------------------[Xcp_SetAbortProgrammingSession]--------------------------*/

FUNC(void, XCP_CODE) Xcp_SetAbortProgrammingSession(void)
{
  DBG_XCP_SETABORTPROGRAMMINGSESSION_ENTER();

  if ((Xcp_CmdReqStatus & XCP_CMD_PROGRAMMING_ACTIVE_MASK) != 0U)
  {
    /* There is an error, programming of a segment will not be active anymore. The user can retry
     * the whole sequence. The programming sequence will be aborted in the next main function
     * cycle */
    Xcp_CmdReqStatusExtension |= XCP_ABORT_PROGRAMMING_MASK;
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    /* Because an error has occured and the entire programming sequence shall be restarted, a DET
       notification shall be issued to signal this situation. */
    XCP_DET_REPORT_ERROR(XCP_SID_SET_ABORT_PROGRAMMING_SESSION, XCP_PROGRAMMING_SEGMENT_NOT_ACTIVE);
#endif
  }

  DBG_XCP_SETABORTPROGRAMMINGSESSION_EXIT();
}
#endif

#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
FUNC(boolean, XCP_CODE)  Xcp_IsAtLeastOneEventMainFunctionProcessing(void)
{
  /* Check that there is not an event main function currently running that also has
     DAQ lists associated running */
  uint8 i,j;
  boolean atLeastOneEventIsCurrentlyProcessing = FALSE;

  for (i=0; i<XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS; i++)
  {
    if (XCP_IS_BYTE_FLAG_SET(i, Xcp_IsMFEventCurrentlyProcessing) == TRUE)
    {
      /* Search the appropriate index in all the events structure
       * (those having a main function as well as the others)*/
      for (j=0; j<XCP_MAX_EVENT_CHANNEL; j++)
      {
        /* If the index matches */
        if (Xcp_EventInfo[j].IndexRunningFlag == i)
        {
          /* Check if there is currently a running DAQ list associated with this event */
          if (Xcp_Event[j].NumberOfRunningDaqLists > 0U)
          {
            atLeastOneEventIsCurrentlyProcessing = TRUE;
          }
        }
      }
    }
  }
  return atLeastOneEventIsCurrentlyProcessing;
}

#endif /*XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U*/

/*------------------[Xcp_InitCommandProcessor]------------------------------*/

FUNC(void, XCP_CODE) Xcp_InitCommandProcessor(void)
{
#if (XCP_MAX_DAQ > 0U) && (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)
  uint8 i;
#endif
  DBG_XCP_INITCOMMANDPROCESSOR_ENTRY();

  /* Set Command processor state machine state to IDLE */
  XCP_CMD_PROCESSOR_STATE_TRANS(XCP_CMDPROCESSOR_IDLE);
  /* Initialize command request status flags */
  Xcp_CmdReqStatus     = 0U;
  /* Initialize Memory transfer address information */
  Xcp_MTA.Type         = XCP_MTATYPE_ADDRESS;
  Xcp_MTA.Address.Read = NULL_PTR;
  Xcp_MTA.IsSet        = FALSE;
#if(XCP_ASYNCH_CRC_SUPPORT == STD_ON)
  /* initialize the asynchronous build checksum job request. */
  Xcp_BuildChecksumData.IsRequestCRCActive = FALSE;
#endif
/* If Dynamic DAQ lists are supported */
#if (XCP_DAQ_COUNT > 0U)
  /* Clear the dynamic info block */
  Xcp_ClearDynamicInfoBlock();
#endif /* (XCP_DAQ_COUNT > 0U) */

  /* Initialize All current status information of the slave device. This
   * includes the status of the resource protection, pending store requests
   * and the general status of data acquisition and stimulation. Protection
   * status: Present implementation supports SEED/UNLOCK and thus all
   * resources are initialized as Protected */
#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
  Xcp_Status.NoOfRunningDaq  = 0U;
  Xcp_Status.NoOfSelectedDaq = 0U;
#endif
  Xcp_Status.SessionStatus   = 0U;
#if (XCP_SEED_AND_KEY_ENABLED == STD_ON)
  Xcp_Status.ProtectionStatus = XCP_RESOURCE_PGM_MASK |
  ( XCP_RESOURCE_STIM_MASK | ( XCP_RESOURCE_DAQ_MASK | XCP_RESOURCE_CAL_PAG_MASK ) );

  /* Initialize security access information */
  Xcp_SecAccess.PrevType = XCP_SEC_ACCESS_INVALID;
#else
  /* Seed and Key mechanism is not required and thus all resources are
   * unprotected */
  Xcp_Status.ProtectionStatus = 0U;
#endif

/* If event packet transmission enabled */
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
  /* Initialize event Cto queue */
  Xcp_FlushEventCTOQueue();
#endif /* (XCP_EVENT_PACKET_ENABLED == STD_ON) */

#if (XCP_MAX_DAQ > 0U)
#if (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)
  /* Initially all FlexRay buffers are assigned to all DAQ lists */
  for (i= 0; i<XCP_NO_OF_FLX_BUF_CONFIGURED; i++)
  {
    Xcp_FlxDaqList[i].FlxDaqAcceptAll = TRUE;
  }
#endif
#endif

  /* Reset the globals used by any memory writing routine */
  Xcp_ResetMemoryWriteInfo();

  DBG_XCP_INITCOMMANDPROCESSOR_EXIT();
}

/*---------------------[Xcp_ProcessCommandQueue]----------------------------*/

FUNC(void, XCP_CODE) Xcp_ProcessCommandQueue(void)
{
  /* Local variable to hold number of commands in command queue.
   * Set to 0 to prevent processing commands if none available */
  uint8 NoOfCmds = 0U;
  /* iterator through the queued commands */
  uint8 Iterator;
  /* Local variable to prepare CTO */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCOMMANDQUEUE_ENTRY();

  /* Enter critical section.*/
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* If SYNCH command is pending */
  if (TRUE == XCP_IS_SYNCH_CMD_PENDING())
  {
    /* Initialize command request status flags */
    Xcp_CmdReqStatus = 0U;
    /* Flush command queue */
    Xcp_FlushCMDQueue();
    /* Reset the globals used by any memory writing routine */
    Xcp_ResetMemoryWriteInfo();
    /* Flush CTO Queue */
    Xcp_FlushCTOQueue();
    /* Initialize transmit processor */
    Xcp_InitTransmitProcessor();
#if(XCP_ASYNCH_CRC_SUPPORT == STD_ON)
    /* reset the asynchronous build checksum job request. */
    Xcp_BuildChecksumData.IsRequestCRCActive = FALSE;
#endif
    /* Trigger state transition */
    XCP_CMD_PROCESSOR_STATE_TRANS(XCP_CMDPROCESSOR_IDLE);

    /* Prepare error CTO response */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_SYNCH, XCP_ERR_CMD_SYNCH_LENGTH);
    /* XCP Insert CTO in response CTO queue  */
    Xcp_InsertResponseCTO(&ResponseCTO);
#if (XCP_USE_MEMORY_PROXY == STD_ON)
    /* !LINKSTO Xcp.BswDistribution.Calibration.MemoryProxy.Interrupted.Synch,1 */
    Atomics_Store(&Xcp_MemoryProxyInterrupted, TRUE);
#endif
#if (XCP_SYNCH_RECEIVED_CALLOUT_ENABLED == STD_ON)
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    /* !LINKSTO Xcp.Xcp_ApplSynchReceived.Call,1 */
    Xcp_ApplSynchReceived();
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
#endif
  }
  /* If DISCONNECT command is pending */
  else if (TRUE == XCP_IS_DISCONNECT_CMD_PENDING())
  {
#if (XCP_PGM_SUPPORTED == STD_ON)
    /* Do not accept DISCONNECT command if a programming of a segment is currently active */
    if ((Xcp_CmdReqStatus & XCP_CMD_PROGRAMMING_ACTIVE_MASK) == XCP_CMD_PROGRAMMING_ACTIVE_MASK)
    {

      /* Verify available space in CTO queue */
      if (XCP_GET_CTOQUEUE_FREESPACE(uint32) >= XCP_ERR_PGM_ACTIVE_LENGTH)
      {
        /* Fill in the error ERR_PGM_ACTIVE */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_PGM_ACTIVE, XCP_ERR_PGM_ACTIVE_LENGTH);
        /* XCP Insert CTO */
        Xcp_InsertResponseCTO(&ResponseCTO);
      }

      /* Clear pending status of DISCONNECT command */
      Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_DISCONNECT_PENDING_MASK);

    }
    else
#endif
    {
      /* No meaningful reaction to the return value. The function already queued a message, either
       * error or positive */
      (void)Xcp_ProcessCmdDisConnect();
      /* The disconnect pending flag is implicitly cleared by Xcp_ProcessCmdDisConnect() function */
    }
  }
  else
  {
#if (XCP_PGM_SUPPORTED == STD_ON)
    /* handle cmd extension status requests */
    Xcp_HandleCmdReqExtStatus();
#endif
    /* All commands needs to be executed */
    NoOfCmds = XCP_GET_NO_OF_COMMANDS();
  }

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Process queued commands (if any) */
  for (Iterator = 0U; Iterator < NoOfCmds; Iterator++)
  {
    Xcp_ProcessCmd();
  }

  DBG_XCP_PROCESSCOMMANDQUEUE_EXIT();
}

/*------------------[Xcp_GetCmdLength]-----------------------------------*/

FUNC(void, XCP_CODE) Xcp_GetCmdLength
(
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) XcpRxSduPtr,
  P2VAR(PduLengthType, AUTOMATIC, XCP_APPL_DATA) DataLengthMin,
  P2VAR(PduLengthType, AUTOMATIC, XCP_APPL_DATA) DataLengthMax
)
{
  /* command ID */
  const uint8 CommandId = XcpRxSduPtr[XCP_CMD_BYTE0_INDEX];
  /* result length read from array */
  PduLengthType MinLength;
  PduLengthType MaxLength;

  DBG_XCP_GETCMDLENGTH_ENTRY(XcpRxSduPtr);

  /* get maximum cmd length */
#if (XCP_ON_CANFD_ENABLED == STD_ON)
  if(XCP_IS_FLAG_SET(XCP_MAX_DLC_REQUIRED, Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Properties, uint8) == TRUE)
  {
    MaxLength = Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].MaxDlcValue;
  }
  else
#endif
#if (XCP_PGM_SUPPORTED == STD_ON)
  if(XCP_IS_PGM_COMMAND_ID(CommandId) == TRUE)
  {
    MaxLength = Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto_Pgm;
  }
  else
#endif
  {
    MaxLength = Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto;
  }

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
#if (XCP_NO_OF_CONNECTIONS > 1U)
  if (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].ConnectionType ==
      XCP_CONNECTION_OVER_FLEXRAY)
#endif
  {
    /* There is an exception for FLX_ASSIGN sub-command: 12 bytes must be allowed */
    if ( CommandId == XCP_CMD_TRANSPORT_LAYER_PID )
    {
      const uint8 SubCommandId = XcpRxSduPtr[XCP_CMD_BYTE1_INDEX];
      if ((SubCommandId == XCP_SUBCMD_FLX_ASSIGN) && (MaxLength < XCP_MAX_CTO_FLX_ASSIGN))
      {
        MaxLength = XCP_MAX_CTO_FLX_ASSIGN;
      }
    }
  }
#endif /* ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)) */

  /* get minimum cmd length */
  if ( CommandId == XCP_CMD_UNLOCK_PID )
  {
    /* UNLOCK */
    MinLength = (uint8)XCP_MIN((XCP_UNLOCK_CMD_HEADER_LENGTH + XcpRxSduPtr[XCP_CMD_BYTE1_INDEX]),
                        Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto);
  }
#if (XCP_WRITE_DAQ_MULTIPLE_SUPPORT == STD_ON)
  else if (CommandId == XCP_CMD_WRITE_DAQ_MULTIPLE_PID)
  {
    /* WRITE_DAQ_MULTIPLE */
    const uint8 NoOfDAQ = XcpRxSduPtr[XCP_CMD_BYTE1_INDEX];
    if (NoOfDAQ > 0)
    {
       MinLength = (PduLengthType)(((uint32)NoOfDAQ*XCP_WRITE_DAQ_MULTIPLE_BYTES_PER_ELEMENT) + 2U);
    }
    else
    {
      /* !LINKSTO Xcp.ASAM.WRITEDAQMULTIPLE006,1 */
      /* At least 10 bytes are needed nonetheless, if NoOfDAQ is 0 - an error will be thrown from
       * WRITE_DAQ_MULTIPLE command execution */
      MinLength = 10U;
    }
  }
#endif /* XCP_WRITE_DAQ_MULTIPLE_SUPPORT == STD_ON */
  else if (CommandId == XCP_CMD_TRANSPORT_LAYER_PID)
  {
    /* TRANSPORT_LAYER_CMD */
    /* No sub commands with parameters are supported,
       therefore just evaluate the sub command code;
       Xcp.TPCMD.NegativeResponse001 */
    MinLength = 2U;
  }
  else if ( IS_CMD_MASTER_BLOCK_MODE(CommandId) == TRUE )
  {
    /* Check if additional fill bytes are needed due to alignment */
#if (XCP_DL_PGM_CMD_HEADER_LENGTH > XCP_ADDRESS_GRANULARITY_SIZE)
    MinLength = (uint8)XCP_MIN(((XCP_DL_PGM_CMD_HEADER_LENGTH) +
                        (XcpRxSduPtr[XCP_CMD_BYTE1_INDEX] * XCP_ADDRESS_GRANULARITY_SIZE)),
                        IS_CMD_DOWNLOAD(CommandId) ? XCP_GET_CONNECTION_MAX_CTO : XCP_GET_CONNECTION_MAX_CTO_PGM);
#else
    MinLength = (uint8)XCP_MIN((XCP_ADDRESS_GRANULARITY_SIZE +
                        (XcpRxSduPtr[XCP_CMD_BYTE1_INDEX] * XCP_ADDRESS_GRANULARITY_SIZE)),
                        IS_CMD_DOWNLOAD(CommandId) ? XCP_GET_CONNECTION_MAX_CTO : XCP_GET_CONNECTION_MAX_CTO_PGM);
#endif
  }
  /* If user commands configured */
#if (XCP_NUMBER_OF_USERCOMMANDS > 0U)
  else if (CommandId == XCP_CMD_USERCMD_PID)
  {
    uint8 StatusVal = XCP_E_NOT_OK;
    uint8 SubCommandIndex;
    for (SubCommandIndex = 0; SubCommandIndex < XCP_NUMBER_OF_USERCOMMANDS ; SubCommandIndex++)
    {
      /* retrieve the command */
      if ( XcpRxSduPtr[XCP_CMD_BYTE1_INDEX] ==  Xcp_UserCommandConf[SubCommandIndex].SubCmdCode)
      {
         StatusVal = XCP_E_OK;
         /* exit loop  if the command was found*/
         break;
      }
    }
    if (StatusVal == XCP_E_OK)
    /* if sub-command code within valid range  */
    {
      /* MinLength shall be the sum between baseLength and configured parameter length  */
      MinLength = (uint8)(Xcp_UserCommandConf[SubCommandIndex].SubCmdLength +
                Xcp_Commands[CommandId - XCP_CMD_PID_BASE].BaseLength);
    }
    else
    {
      /* MinLength shall be the baseLength */
      MinLength = Xcp_Commands[CommandId - XCP_CMD_PID_BASE].BaseLength;
    }
  }
#endif /* (XCP_NUMBER_OF_USERCOMMANDS > 0U) */
  else
  {
    /* command default minimum length size */
    MinLength = Xcp_Commands[CommandId - XCP_CMD_PID_BASE].BaseLength;
  }

  /* return the minimum calculated size */
  *DataLengthMin = MinLength;

  /* return the maximum calculated length */
  *DataLengthMax = MaxLength;

  DBG_XCP_GETCMDLENGTH_EXIT(DataLengthMin,DataLengthMax);
}

/*----------------------------[Xcp_IsCmdImplemented]---------------------------*/

FUNC (boolean, XCP_CODE) Xcp_IsCmdImplemented(uint8 Pid)
{
  boolean RetVal;

  DBG_XCP_ISCMDIMPLEMENTED_ENTRY(Pid);

  if((XCP_PID_IS_SYNCH(Pid)) ||
     (XCP_PID_IS_DISCONNECT(Pid)))
  {
    RetVal = TRUE;
  }
  else
  {
    RetVal = ((Xcp_Commands[(Pid) - XCP_CMD_PID_BASE].CommandHandlerPtr != NULL_PTR)
              ? TRUE : FALSE);
  }

  DBG_XCP_ISCMDIMPLEMENTED_EXIT(RetVal);

  return RetVal;
}

/*----------------------------[Xcp_SendCmdErrorPacket]---------------------------*/

FUNC(void, XCP_CODE) Xcp_SendCmdErrorPacket
(
  uint8 Pid,
  uint8 ErrId
)
{
  DBG_XCP_SENDCMDERRORPACKET_ENTRY(Pid, ErrId);

  if((Xcp_ConnectionStatus.State == XCP_STATE_CONNECTED) ||
     (XCP_CMD_CONNECT_PID == Pid))
  {
    /* Check space available in response CTO queue */
    if ( XCP_ERR_CMD_SYNTAX_LENGTH <= XCP_GET_CTOQUEUE_FREESPACE(uint32) )
    {
      /* Local variable to prepare response */
      Xcp_CTOType ResponseCTO;

      /* Prepare error CTO with negative response XCP_ERR_CMD_SYNTAX */
      ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_ERR_PID;
      switch(ErrId)
      {
      case XCP_ERR_CMD_SYNTAX:
        ResponseCTO.CTO[XCP_RES_ERRCODE_INDEX] = XCP_ERR_CMD_SYNTAX;
        ResponseCTO.Length = XCP_ERR_CMD_SYNTAX_LENGTH;
        break;
      case XCP_ERR_CMD_UNKNOWN:
        /* XCP_ERR_CMD_UNKNOWN represent the default response, for this case*/
      default:
        ResponseCTO.CTO[XCP_RES_ERRCODE_INDEX] = XCP_ERR_CMD_UNKNOWN;
        ResponseCTO.Length = XCP_ERR_CMD_UNKNOWN_LENGTH;
        break;
      }

      /* Enqueue response packet */
      Xcp_InsertResponseCTO(&ResponseCTO);
    }
    else
    {
#if(XCP_PROD_ERR_HANDLING_RESP_CTO_QUEUE_FULL!= TS_PROD_ERR_DISABLE)
      /* There is not enough available space in the response CTO queue.
       Therefore the response packet is not queued and a DET notification
       is issued to signal this situation. */

      XCP_REPORT_RESP_CTO_QUEUE_FULL(XCP_SID_SEND_CMD_SYNTAX_PACKET);
#endif
    } /* ( XCP_ERR_CMD_SYNTAX_LENGTH <= XCP_GET_CTOQUEUE_FREESPACE() ) */
  }

  DBG_XCP_SENDCMDERRORPACKET_EXIT();
}

/*----------------------------[Xcp_InsertCommand]---------------------------*/

FUNC(void, XCP_CODE) Xcp_InsertCommand
(
  PduLengthType SduLength,
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) SduDataPtr
)
{
  DBG_XCP_INSERTCOMMAND_ENTRY(SduLength,SduDataPtr);

  /* Enqueue command CTO to the command queue */
  TS_MemCpy(Xcp_CommandQueue.Queue[Xcp_CommandQueue.Tail].CTO, SduDataPtr, SduLength);

  /* Update length of the command packet */
  Xcp_CommandQueue.Queue[Xcp_CommandQueue.Tail].Length = (uint8)SduLength;
  /* Update Queue tail */
  Xcp_CommandQueue.Tail++;
  Xcp_CommandQueue.Tail %= XCP_CMD_PROCESSOR_QUEUE_SIZE;

  /* Update number of commands */
  Xcp_CommandQueue.NoOfCmds++;

  DBG_XCP_INSERTCOMMAND_EXIT(SduLength,SduDataPtr);
}

/*-----------------------[Xcp_SetSynchCmdPending]--------------------------*/

FUNC(void, XCP_CODE) Xcp_SetSynchCmdPending(void)
{
  DBG_XCP_SETSYNCHCMDPENDING_ENTRY();

  /* Set SYNCH command as pending */
  Xcp_CmdReqStatus |= XCP_CMD_SYNCH_PENDING_MASK;

  DBG_XCP_SETSYNCHCMDPENDING_EXIT();
}

/*-----------------------[Xcp_SetDisconnectCmdPending]--------------------------*/

FUNC(void, XCP_CODE) Xcp_SetDisconnectCmdPending(void)
{
  DBG_XCP_SETDISCONNECTCMDPENDING_ENTRY();

  /* Set DISCONNECT command as pending */
  Xcp_CmdReqStatus |= XCP_CMD_DISCONNECT_PENDING_MASK;

  DBG_XCP_SETDISCONNECTCMDPENDING_EXIT();
}

#if (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)

/*-----------------------[Xcp_IsProgrammingBlockMode]--------------------------*/
FUNC(boolean, XCP_CODE) Xcp_IsProgrammingBlockMode(void)
{
  boolean RetValue;

  DBG_XCP_ISPROGRAMMINGBLOCKMODE_ENTRY();

  RetValue = ((Xcp_CmdReqStatus & XCP_CMD_PGM_BLOCK_MODE_MASK) != 0U ) ? TRUE : FALSE;

  DBG_XCP_ISPROGRAMMINGBLOCKMODE_EXIT(RetValue);
  return RetValue;
}

/*-----------------------[Xcp_SetProgrammingBlockMode]--------------------------*/
FUNC(void, XCP_CODE) Xcp_SetProgrammingBlockMode(void)
{
  /* Set the programming transfer in block mode  */
  Xcp_CmdReqStatus |= XCP_CMD_PGM_BLOCK_MODE_MASK;
}

#endif /* (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) */

#if (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)

/*-----------------------[Xcp_IsDownloadInBlockMode]--------------------------*/
FUNC(boolean, XCP_CODE) Xcp_IsDownloadInBlockMode(void)
{
  boolean RetValue;
  DBG_XCP_ISDOWNLOADINBLOCKMODE_ENTRY();
  RetValue = ((Xcp_CmdReqStatus & XCP_CMD_DOWNLOAD_BLOCK_MODE_MASK) != 0U) ? TRUE : FALSE;
  DBG_XCP_ISDOWNLOADINBLOCKMODE_EXIT(RetValue);
  return RetValue;
}

/*-----------------------[Xcp_SetDownloadInBlockMode]--------------------------*/
FUNC(void, XCP_CODE) Xcp_SetDownloadInBlockMode(void)
{
  DBG_XCP_SETDOWNLOADINBLOCKMODE_ENTRY();

  /* Set the DOWNLOAD in block mode */
  Xcp_CmdReqStatus |= XCP_CMD_DOWNLOAD_BLOCK_MODE_MASK;

  DBG_XCP_SETDOWNLOADINBLOCKMODE_EXIT();
}

#endif /* (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) */

/*-----------------------[Xcp_CommandQueueIsEmpty]--------------------------*/

FUNC(boolean, XCP_CODE) Xcp_CommandQueueIsEmpty(void)
{
  boolean RetValue;
  DBG_XCP_COMMANDQUEUEISEMPTY_ENTRY();
  RetValue = (Xcp_CommandQueue.NoOfCmds == 0U) ? TRUE : FALSE;
  DBG_XCP_COMMANDQUEUEISEMPTY_EXIT(RetValue);
  return RetValue;
}


/*-----------------------[Xcp_CommandQueueIsFull]--------------------------*/

FUNC(boolean, XCP_CODE) Xcp_CommandQueueIsFull(void)
{
  boolean RetValue;
  DBG_XCP_COMMANDQUEUEISFULL_ENTRY();
  RetValue = (Xcp_CommandQueue.NoOfCmds == XCP_CMD_PROCESSOR_QUEUE_SIZE ) ? TRUE : FALSE;
  DBG_XCP_COMMANDQUEUEISFULL_EXIT(RetValue);
  return RetValue;
}

/* If event packet transmission enabled */
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)

/*----------------------------[Xcp_IsEventCtoQueueEmpty]----------------------------*/

FUNC(boolean, XCP_CODE) Xcp_IsEventCtoQueueEmpty(void)
{
  boolean RetVal;
  DBG_XCP_ISEVENTCTOQUEUEEMPTY_ENTRY();
  RetVal = (0U == Xcp_EventCTOQueue.NoOfElements)? TRUE : FALSE;
  DBG_XCP_ISEVENTCTOQUEUEEMPTY_EXIT(RetVal);
  return RetVal;
}

/*----------------------------[Xcp_GetEventCtoLength]----------------------------*/

FUNC(PduLengthType, XCP_CODE) Xcp_GetEventCtoLength(void)
{
  PduLengthType RetVal;
  const uint8 Readposition = Xcp_EventCTOQueue.Head;
  DBG_XCP_GETEVENTCTOLENGTH_ENTRY();
  RetVal = (Xcp_EventCTOQueue.EventCTOQueue[Readposition].Length);
  DBG_XCP_GETEVENTCTOLENGTH_EXIT(RetVal);
  return RetVal;
}

/*----------------------------[Xcp_DeQueueEventCtoQueue]----------------------------*/

FUNC(void, XCP_CODE) Xcp_DeQueueEventCtoQueue
(
  PduLengthType DataLength,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) TxBufferPtr
)
{
  const uint8 Readposition = Xcp_EventCTOQueue.Head;

  DBG_XCP_DEQUEUEEVENTCTOQUEUE_ENTRY(DataLength, TxBufferPtr);

  /* Dequeue Event CTO from Event CTO queue to transmit buffer */
  TS_MemCpy(TxBufferPtr,
            &(Xcp_EventCTOQueue.EventCTOQueue[Readposition].EVENT_CTO[0U]),
            DataLength);

  /* select the next read position into queue */
  Xcp_EventCTOQueue.Head = (Xcp_EventCTOQueue.Head + 1U) % XCP_EV_CTO_QUEUE_SIZE;
  Xcp_EventCTOQueue.NoOfElements--;

  DBG_XCP_DEQUEUEEVENTCTOQUEUE_EXIT(TxBufferPtr);
}

/*----------------------------[Xcp_EnqueueCtoEvent]----------------------------*/

FUNC(void, XCP_CODE)  Xcp_EnqueueCtoEvent
(
  P2CONST(Xcp_EventCTOType, AUTOMATIC, XCP_APPL_DATA) EventCtoPtr
)
{
  uint8 EvCtoQueueIndex;
  uint8 EvCtoIndex;
  /* initialize with false: consider that the event queue is empty */
  boolean EvFound = FALSE;
  P2VAR(Xcp_EventCTOType, AUTOMATIC, XCP_APPL_DATA) EvCTOQueuePtr;

  DBG_XCP_ENQUEUECTOEVENT_ENTRY(EventCtoPtr);

  if(Xcp_EventCTOQueue.NoOfElements < XCP_EV_CTO_QUEUE_SIZE)
  {
    /* search for the same event in Event Cto queue */
    for(EvCtoQueueIndex = Xcp_EventCTOQueue.Head; EvCtoQueueIndex != Xcp_EventCTOQueue.Tail;
        EvCtoQueueIndex = (EvCtoQueueIndex + 1U) % XCP_EV_CTO_QUEUE_SIZE)
    {
      /* initialize with true: consider that the selected element is identical with the input one */
      EvFound = TRUE;
      /* select event package from Event CTO */
      EvCTOQueuePtr = &Xcp_EventCTOQueue.EventCTOQueue[EvCtoQueueIndex];
      /* verify if the event package is already in queue
       * we skip the verification for the first element, which is actually
       * the ID of Event responce package, which is the same for all Ev CTOs.*/
      for(EvCtoIndex = 1U; EvCtoIndex < EventCtoPtr->Length; EvCtoIndex++)
      {
        if(EventCtoPtr->EVENT_CTO[EvCtoIndex] != EvCTOQueuePtr->EVENT_CTO[EvCtoIndex])
        {
          /* make it false: the presumption is proved to be wrong */
          EvFound = FALSE;
          break;
        }
      }
      /* check if the event was found */
      if(EvFound == TRUE)
      {
        /* the element is already in queue */
        break;
      }
    }

    /* in case the event is not already into the queue, add the event.
     * in this way we don't store the same package twice which doesn't
     * add any new information for the Master, and we give a chance for
     * other event packages to be prepare for dispatching. */
    if(EvFound == FALSE)
    {
      /* select the next available entry into event cto queue */
      EvCTOQueuePtr = &Xcp_EventCTOQueue.EventCTOQueue[Xcp_EventCTOQueue.Tail];
      /* add the event into event CTO queue */
      TS_MemCpy(EvCTOQueuePtr->EVENT_CTO, EventCtoPtr->EVENT_CTO, EventCtoPtr->Length);
      /* store the length of the enqueued event Cto package */
      EvCTOQueuePtr->Length = EventCtoPtr->Length;
      /* update event CTO queue length */
      Xcp_EventCTOQueue.NoOfElements++;
      /* select the next write position */
      Xcp_EventCTOQueue.Tail = (Xcp_EventCTOQueue.Tail + 1U) % XCP_EV_CTO_QUEUE_SIZE;
    }
  }
#if(XCP_PROD_ERR_HANDLING_RESP_CTO_QUEUE_FULL!= TS_PROD_ERR_DISABLE)
  /* if there is not enough space to enqueue the event CTO*/
  else
  {
    /* There is not enough available space in the response CTO queue.
       Therefore the event CTO is not enqueued and a DET notification
       is issued to signal this situation. */

    XCP_REPORT_RESP_CTO_QUEUE_FULL(XCP_SID_ENQUEUECTOEVENT);

  }
#endif /* if (XCP_PROD_ERR_HANDLING_RESP_CTO_QUEUE_FULL!= TS_PROD_ERR_DISABLE) */

  DBG_XCP_ENQUEUECTOEVENT_EXIT();
}
#endif /* (XCP_EVENT_PACKET_ENABLED == STD_ON) */

/*----------------------------[Xcp_FlushCMDQueue]----------------------------*/

FUNC(void, XCP_CODE) Xcp_FlushCMDQueue(void)
{
  DBG_XCP_FLUSHCMDQUEUE_ENTRY();

  /* Reset queue head */
  Xcp_CommandQueue.Head     =  0U;
  /* Reset queue tail */
  Xcp_CommandQueue.Tail     =  0U;
  /* Reset number of commands in queue */
  Xcp_CommandQueue.NoOfCmds =  0U;

  DBG_XCP_FLUSHCMDQUEUE_EXIT();
}

/*----------------------------[Xcp_FlushCTOQueue]---------------------------*/

FUNC(void, XCP_CODE) Xcp_FlushCTOQueue(void)
{
  DBG_XCP_FLUSHCTOQUEUE_ENTRY();

  /* Reset queue head */
  Xcp_CtoQueue.Head      =  0U;
  /* Reset queue tail */
  Xcp_CtoQueue.Tail      =  0U;
  /* Reset number of bytes in queue */
  Xcp_CtoQueue.NoOfBytes =  0U;

  DBG_XCP_FLUSHCTOQUEUE_EXIT();
}

/*------------------------[Xcp_FlushEventCTOQueue]--------------------------*/
/* If event packet transmission enabled */
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)

FUNC(void, XCP_CODE) Xcp_FlushEventCTOQueue(void)
{
  DBG_XCP_FLUSHEVENTCTOQUEUE_ENTRY();

  /* Clear all pending events to be transmitted by resetting the index */
  Xcp_EventCTOQueue.NoOfElements = 0U;
  Xcp_EventCTOQueue.Head = 0U;
  Xcp_EventCTOQueue.Tail = 0U;

  DBG_XCP_FLUSHEVENTCTOQUEUE_EXIT();
}

#endif /* (XCP_EVENT_PACKET_ENABLED == STD_ON) */

/*--------------------[Xcp_WriteDwordToByteArray]---------------------------*/

FUNC(void, XCP_CODE) Xcp_WriteDwordToByteArray
(
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) DesPtr,
  uint32                                 Dword
)
{
  DBG_XCP_WRITEDWORDTOBYTEARRAY_ENTRY(Dword);

  DesPtr[XCP_UINT32_SB0_IDX] = (uint8)Dword;
  DesPtr[XCP_UINT32_SB1_IDX] = (uint8)(Dword >> 8U);
  DesPtr[XCP_UINT32_SB2_IDX] = (uint8)(Dword >> 16U);
  DesPtr[XCP_UINT32_SB3_IDX] = (uint8)(Dword >> 24U);

  DBG_XCP_WRITEDWORDTOBYTEARRAY_EXIT(DesPtr);
}

/*---------------------[Xcp_WriteWordToByteArray]---------------------------*/

FUNC(void, XCP_CODE) Xcp_WriteWordToByteArray
(
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) DesPtr,
  uint16                                 Word
)
{
  DBG_XCP_WRITEWORDTOBYTEARRAY_ENTRY(Word);

  DesPtr[XCP_UINT16_LSB_IDX] = (uint8)Word;
  DesPtr[XCP_UINT16_MSB_IDX] = (uint8)(Word >> 8U);

  DBG_XCP_WRITEWORDTOBYTEARRAY_EXIT(DesPtr);
}

/*--------------------[Xcp_GetCommandProcessorState]---------------------------*/

FUNC(Xcp_ProcessorStateType, XCP_CODE) Xcp_GetCommandProcessorState(void)
{
  DBG_XCP_GETCOMMANDPROCESSORSTATE_ENTRY();
  DBG_XCP_GETCOMMANDPROCESSORSTATE_EXIT(Xcp_CmdProcessorState);
  return Xcp_CmdProcessorState;
}

/*--------------------[Xcp_AbortBusyCommand]---------------------------*/

FUNC(void, XCP_CODE) Xcp_AbortBusyCommand(void)
{
  DBG_XCP_ABORTBUSYCOMMAND_ENTRY();

  /* Set the busy command as aborted */
  Xcp_CmdReqStatus |= XCP_CMD_BUSY_ABORTED_MASK;

  DBG_XCP_ABORTBUSYCOMMAND_EXIT();
}

#if (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON)

/*--------------------[Xcp_SetReqStoreCalReqCbk]---------------------------*/
FUNC(Std_ReturnType, XCP_APPL_CODE) Xcp_SetReqStoreCalReqCbk
(
  uint8 ServiceId,
  NvM_RequestResultType JobResult
)
{
  Std_ReturnType RetValue;
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
  /* !LINKSTO Xcp.dsn.BSWDistribution.MemProxy.Atomics,1 */
  Atomic_value_t partition = Atomics_Load(&Xcp_PartitionOfActiveConnection);
  /* !LINKSTO Xcp.dsn.BSWDistribution.XcpEvent.StoreCall,1 */
  if (XCP_MASTER_APPLICATION == partition)
  {
    RetValue = Xcp_SetReqStoreCalReqCbk_Internal(ServiceId, JobResult);
  }
  else
  {
#if (XCP_REMOTE_DESTINATIONS == STD_ON)
    Xcp_RemoteDispatchType data;
    data.type = XCP_REMOTE_DISPATCH_SETREQSTORECALREQCBK;
    data.p8[0] = ServiceId;
    data.p8[1] = JobResult;
    (void) Xcp_RemoteDispatch_Handler(data);
#endif
    RetValue = XCP_OK;
  }
#else
  RetValue = Xcp_SetReqStoreCalReqCbk_Internal(ServiceId, JobResult);
#endif
  return RetValue;
}

FUNC(Std_ReturnType, XCP_APPL_CODE) Xcp_SetReqStoreCalReqCbk_Internal
(
  uint8 ServiceId,
  NvM_RequestResultType JobResult
)
{
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
  Xcp_EventCTOType EventCTO;
#endif

  DBG_XCP_SETREQSTORECALREQCBK_ENTRY(ServiceId,JobResult);

  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
    /* Check SessionStatus: Request to store calibration data */
    if( ( (Xcp_Status.SessionStatus & XCP_MASK_STORE_CAL_REQ) != 0U ) &&
        (ServiceId == XCP_NVM_WRITE_BLOCK_API_ID) )
    {
      /* Clear Xcp_Status.SessionStatus - STORE_CAL_REQ bit */
      XCP_CLEAR_FLAG_U8_ATOMIC(XCP_BIT_STORE_CAL_REQ, Xcp_Status.SessionStatus);

#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
      /* Check ServiceId and JobResult */
      if (JobResult == NVM_REQ_OK)
      {
        /* Prepare the Event Packet EV_STORE_CAL package */
        EventCTO.EVENT_CTO[0U] = XCP_EV_PID;
        EventCTO.EVENT_CTO[1U] = XCP_EV_STORE_CAL;
        EventCTO.Length = XCP_CALPAG_EV_STORE_CAL_LENGTH;

        /* Enter critical area */
        SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

        /* enqueue event package into event CTO queue */
        Xcp_EnqueueCtoEvent(&EventCTO);

        /* Exit critical area */
        SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      }
#else
      TS_PARAM_UNUSED(JobResult);
#endif /* (XCP_EVENT_PACKET_ENABLED == STD_ON) */

    }  /* ( ( (Xcp_Status.SessionStatus & XCP_MASK_STORE_CAL_REQ) != 0U ) &&
            ( ServiceId == XCP_NVM_WRITE_BLOCK_API_ID) )*/
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }
  DBG_XCP_SETREQSTORECALREQCBK_EXIT(E_OK,ServiceId,JobResult);
  return E_OK;
}
#endif /* (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON) */

#if (XCP_STORE_DAQ_SUPPORTED == STD_ON)

/*--------------------[Xcp_NvmStoreDaqSingleCbk]---------------------------*/

FUNC(Std_ReturnType, XCP_CODE) Xcp_NvmStoreDaqSingleCbk
(
  uint8 ServiceId,
  NvM_RequestResultType JobResult
)
{
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF)
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
  Xcp_EventCTOType EventCTO;
#endif

  DBG_XCP_NVMSTOREDAQSINGLECBK_ENTRY(ServiceId,JobResult);

  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
    if (ServiceId != XCP_NVM_WRITE_BLOCK_API_ID)
    {
      /* Ignore callback, it was wrongfully sent to us - we expect only the NvM_WriteBlock service */
    }
    else
    {
      /* Find out for which request the callback is received  */
      if ((Xcp_Status.SessionStatus & XCP_MASK_CLEAR_DAQ_REQ) != 0U)
      {
        /* Clear the CLEAR_DAQ_REQ flag regardless of the block request status */
        XCP_CLEAR_FLAG_U8_ATOMIC(XCP_BIT_CLEAR_DAQ_REQ, Xcp_Status.SessionStatus);

#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
        if (JobResult == NVM_REQ_OK)
        {
          /* prepare the Event Packet XCP_EV_CLEAR_DAQ */
          EventCTO.EVENT_CTO[0U] = XCP_EV_PID;
          EventCTO.EVENT_CTO[1U] = XCP_EV_CLEAR_DAQ;
          EventCTO.Length = 0x02U;

          /* Protect concurrent access to Event CTO Queue */
          SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

          /* enqueue event package into event CTo queue */
          Xcp_EnqueueCtoEvent(&EventCTO);

          /* exit critical section */
          SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
        }
#else
        TS_PARAM_UNUSED(JobResult);
#endif
      }
      if ((Xcp_Status.SessionStatus & XCP_MASK_STORE_DAQ_REQ) != 0U)
      {
        /* Clear the STORE_DAQ_REQ flag regardless of the block request status */
        XCP_CLEAR_FLAG_U8_ATOMIC(XCP_BIT_STORE_DAQ_REQ, Xcp_Status.SessionStatus);

#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
        if (JobResult == NVM_REQ_OK)
        {
          /* Queue the Event Packet XCP_EV_STORE_DAQ */
          EventCTO.EVENT_CTO[0U] = XCP_EV_PID;
          EventCTO.EVENT_CTO[1U] = XCP_EV_STORE_DAQ;
          EventCTO.Length = 0x02U;

          /* Protect concurrent access to Event CTO Queue */
          SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

          /* enqueue event package into event CTo queue */
          Xcp_EnqueueCtoEvent(&EventCTO);

          /* exit critical section */
          SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
        }
#else
        TS_PARAM_UNUSED(JobResult);
#endif
      }
    }
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }
  DBG_XCP_NVMSTOREDAQSINGLECBK_EXIT(E_OK,ServiceId,JobResult);
#else
  /* !LINKSTO Xcp.dsn.BSWDistribution.XcpEvent.StoreDaq,1 */
  /* !LINKSTO Xcp.dsn.BSWDistribution.XcpEvent.ClearDaq,1 */
  TS_PARAM_UNUSED(ServiceId);
  TS_PARAM_UNUSED(JobResult);
#endif /* XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF */
  return E_OK;
}
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON)
FUNC(Std_ReturnType, XCP_CODE) Xcp_ReadRamBlockFromNvMNativeCbk(
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvMBuffer
)
{
#if (XCP_NO_OF_NON_DYNAMIC_ODTENTRIES > 0)
  const uint8* nvPtr = ((const uint8*)NvMBuffer) + sizeof(Xcp_DaqLists);
#endif
  TS_MemCpy(&Xcp_DaqLists, NvMBuffer, sizeof(Xcp_AllDaqListsType));
#if (XCP_NO_OF_NON_DYNAMIC_ODTENTRIES > 0)
  TS_MemCpy(&Xcp_OdtAddressTableRAM[0], nvPtr, sizeof(Xcp_OdtAddressTableRAM));
  nvPtr += sizeof(Xcp_OdtAddressTableRAM);
  TS_MemCpy(&Xcp_OdtLengthTableRAM[0], nvPtr, sizeof(Xcp_OdtLengthTableRAM));
  nvPtr += sizeof(Xcp_OdtLengthTableRAM);
#if (XCP_ENABLE_BIT_OFFSET == STD_ON)
  TS_MemCpy(&Xcp_OdtBitOffsetTableRAM[0], nvPtr, sizeof(Xcp_OdtBitOffsetTableRAM));
#endif
#endif
  return E_OK;
}
FUNC(Std_ReturnType, XCP_CODE) Xcp_WriteRamBlockToNvMNativeCbk(
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvMBuffer
)
{
#if (XCP_NO_OF_NON_DYNAMIC_ODTENTRIES > 0)
  uint8* nvPtr = ((uint8*)NvMBuffer) + sizeof(Xcp_DaqLists);
#endif
  TS_MemCpy(NvMBuffer, &Xcp_DaqLists, sizeof(Xcp_DaqLists));
#if (XCP_NO_OF_NON_DYNAMIC_ODTENTRIES > 0)
  TS_MemCpy(nvPtr, &Xcp_OdtAddressTableRAM[0], sizeof(Xcp_OdtAddressTableRAM));
  nvPtr += sizeof(Xcp_OdtAddressTableRAM);
  TS_MemCpy(nvPtr, &Xcp_OdtLengthTableRAM[0], sizeof(Xcp_OdtLengthTableRAM));
  nvPtr += sizeof(Xcp_OdtLengthTableRAM);
#if (XCP_ENABLE_BIT_OFFSET == STD_ON)
  TS_MemCpy(nvPtr, &Xcp_OdtBitOffsetTableRAM[0], sizeof(Xcp_OdtBitOffsetTableRAM));
#endif
#endif
  return E_OK;
}
#endif
/*--------------------------[Xcp_PrepareStoredDaqLists]-----------------------------*/
FUNC(void, XCP_CODE) Xcp_PrepareStoredDaqLists(void)
{
  /* Local variable to hold address of the requested DAQ List's parameters */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr;
  /* Iterator through all DAQ lists */
  Xcp_DaqIdType DaqListNumber;

  DBG_XCP_PREPARESTOREDDAQLISTS_ENTRY();

#if (XCP_DAQ_COUNT > 0U)
  /* Set the number of configured dynamic DAQ lists */
  Xcp_DynamicInfo.NoOfConfiguredDynamicDAQLists = Xcp_DaqLists.Xcp_StoreDaq.NoOfConfiguredDynamicDaqLists;
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON)
  Xcp_DynamicInfo.AllocatedOdtsSoFar = Xcp_DaqLists.Xcp_StoreDaq.NoOfConfiguredOdts;
  Xcp_DynamicInfo.AllocatedOdtEntriesSoFar = Xcp_DaqLists.Xcp_StoreDaq.NoOfConfiguredOdtEntries;
  Xcp_DynamicInfo.FirstDynamicOdtEntryAddr = (usize*)((usize)Xcp_DaqLists.Xcp_DynamicArea + (Xcp_DynamicInfo.NoOfConfiguredDynamicDAQLists * sizeof(Xcp_DaqType)) + (Xcp_DynamicInfo.AllocatedOdtsSoFar * sizeof(Xcp_OdtType)));
#endif
#endif

  /* Loop through all configured DAQ Lists to check which ones are selected and which ones are not */
  for (DaqListNumber = 0U; (DaqListNumber < XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(Xcp_DaqIdType));
       DaqListNumber++)
  {
    /* Retrieve DAQ List parameter */
    DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqListNumber);

    /* If a DAQ List is selected to be stored */
    if (XCP_IS_BYTE_FLAG_SET(DaqListNumber, Xcp_DaqLists.Xcp_StoreDaq.SelectedDAQ) == TRUE)
    {
      /* If selected DAQ lists have events associated queue all their events */
      /* CHECK: NOPARSE */
      if (DaqListPtr->EventId != XCP_INVALID_EVENT_CHANNEL)
      /* CHECK: PARSE */
      {
        /* Non predefined lists lost their associated events during the reset. The connection
         * must be made once again. */
#if (XCP_MIN_DAQ > 0)
        if (DaqListNumber >= XCP_MIN_DAQ)
#endif
        {
          Xcp_AssociateDAQListToEventChannel(DaqListNumber,DaqListPtr->EventId);
        }
        /* If there is a request to start in RESUME mode, also set the DAQ lists as running */
        if ((Xcp_DaqLists.Xcp_StoreDaq.Mode & XCP_MASK_STORE_DAQ_REQ_RESUME) != 0U)
        {
          /* Set DAQ list as running */
          XCP_SET_DAQLIST_RUNNING(DaqListPtr);
          /* Update number of DAQ Lists running */
          Xcp_Status.NoOfRunningDaq++;
        }
      }
      /* CHECK: NOPARSE */
      else
      {
        /* To reach this else the following scenario should occur: A DaqList must be selected
           and its associated event should be XCP_INVALID_EVENT_CHANNEL.
           The current implementation does not allow a DaqList to be selected if its associated
           event is XCP_INVALID_EVENT_CHANNEL. Therefore this branch is unreachable */
        XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
      }
      /* CHECK: PARSE */
    }
    else /* the DAQ list was not selected to be stored*/
    {
      /* Restore it with default values */
      Xcp_ResetDaqListData(DaqListNumber);
    }
  }

  DBG_XCP_PREPARESTOREDDAQLISTS_EXIT();
}

/*--------------------------[Xcp_PrepareStoredDaqListsResume]-----------------------------*/
FUNC(void, XCP_CODE) Xcp_PrepareStoredDaqListsResume(void)
{
  /* We save the state of the XCP so we can restore it if something goes wrong */
  Xcp_StateType SavedState  = Xcp_ConnectionStatus.State;
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
  Xcp_EventCTOType EventCTO;
#endif

  DBG_XCP_PREPARESTOREDDAQLISTSRESUME_ENTRY();

  /* Set the status of XCP as RESUME */
  XCP_SET_FLAG_U8_ATOMIC(XCP_BIT_RESUME_MODE, Xcp_Status.SessionStatus);

  /* Set the state of XCP as RESUME */
  Xcp_ConnectionStatus.State = XCP_STATE_RESUME;
#if(XCP_NO_OF_CONNECTIONS > 1U)

  /* in case multiple connections are used, restore the ConnectionId to be used */
  Xcp_ConnectionStatus.ConnectionId = Xcp_DaqLists.Xcp_StoreDaq.ConnectionId;

  /* Initialize runtime flags from configuration: if the PDU is dynamic and which packet is supported
   If resume mode is not enabled, those flags are initialized from Rx Indication context */
  Xcp_InitRxPduFlags(Xcp_ConnectionStatus.ConnectionId);
  Xcp_InitTxPduFlags(Xcp_ConnectionStatus.ConnectionId);

#endif
  /* Set XCP slave status as DAQ running */
  XCP_SET_FLAG_U8_ATOMIC(XCP_BIT_DAQ_RUNNING, Xcp_Status.SessionStatus);

  /* Prepare the stored DAQ lists which are selected */
  Xcp_PrepareStoredDaqLists();

  /* If any of the DAQ list is running */
  if (Xcp_Status.NoOfRunningDaq > 0U )
  {
    /* Try to send EV_RESUME_MODE event */
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)

#if (XCP_TIMESTAMP_SUPPORTED == XCP_TIMESTAMP_MASK)
#if (XCP_USER_TIMESTAMP_SUPPORTED == STD_OFF)
    /* Local variable to hold the counter value */
    TickType CounterValue;
#endif /* (XCP_USER_TIMESTAMP_SUPPORTED == STD_OFF) */

    /* Local variable to hold the timestamp */
    Xcp_TimestampType TimeStamp = 0U;

#if (XCP_USER_TIMESTAMP_SUPPORTED == STD_OFF)
    /* Retrieve present time stamp from the OS counter */
    (void) GetCounterValue(XCP_OS_COUNTER, &CounterValue);

    /* Convert the CounterVAlue to the Xcp_TimestampType and copy the value to the TimpeStamp variable */
    TimeStamp = (Xcp_TimestampType)CounterValue;
#else
    /* Get the timestamp value from the user provided function */
    TimeStamp = Xcp_ApplGetTimestamp();
#endif /* (XCP_USER_TIMESTAMP_SUPPORTED == STD_OFF) */
#endif /* (XCP_TIMESTAMP_SUPPORTED == XCP_TIMESTAMP_MASK) */

    /* Prepare the event XCP_EV_RESUME_MODE */
    EventCTO.EVENT_CTO[0U] = XCP_EV_PID;
    EventCTO.EVENT_CTO[1U] = XCP_EV_RESUME_MODE;
    Xcp_WriteWordToByteArray(&(EventCTO.EVENT_CTO[2U]),
                             XCP_GET_SESSION_CONFIGID());
#if (XCP_TIMESTAMP_SUPPORTED == XCP_TIMESTAMP_MASK)
    if (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Timestamp_Enabled == TRUE)
    {
      /* Copy time stamp to response packet */
      Xcp_WriteDwordToByteArray( &(EventCTO.EVENT_CTO[4U]), TimeStamp );
      EventCTO.Length = 8U;
    }
    else
#endif
    {
      /* Timestamp is disabled */
      EventCTO.Length = 4U;
    }
    /* Protect concurrent access to Event CTO Queue */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* enqueue event package into event CTo queue */
    Xcp_EnqueueCtoEvent(&EventCTO);

    /* exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
#endif /* (XCP_EVENT_PACKET_ENABLED == STD_ON) */
  }
  else
  {
    /* Clear the status of XCP as it is no longer in RESUME */
    XCP_CLEAR_FLAG_U8_ATOMIC(XCP_BIT_RESUME_MODE, Xcp_Status.SessionStatus);
    /* Restore the state of XCP */
    Xcp_ConnectionStatus.State = SavedState;
    /* Clear the XCP slave status as DAQ running */
    XCP_CLEAR_FLAG_U8_ATOMIC(XCP_BIT_DAQ_RUNNING, Xcp_Status.SessionStatus);
  }

  DBG_XCP_PREPARESTOREDDAQLISTSRESUME_EXIT();
}

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
FUNC(void, XCP_CODE) Xcp_NvMRequestHandler(void)
{
  (void) NvM_WriteBlock(XCP_STORE_DAQ_NVM_BLOCK_ID, NULL_PTR);
}
#endif

#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) */

#if (XCP_DAQ_COUNT > 0U)
FUNC_P2VAR(Xcp_DaqType, XCP_VAR_CLEARED, XCP_CODE) Xcp_GetDaqListPtr(Xcp_DaqIdType DaqListNumber)
{
  /* The queried pointer to a DAQ list */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) RetValue;

  /* Deviation MISRAC2012-2 <START> */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) AddressOfTheFirstDynamicDaq =
      (Xcp_DaqType *)(&(Xcp_DaqLists.Xcp_DynamicArea[0U]));
  /* Deviation MISRAC2012-2 <STOP> */

  DBG_XCP_GETDAQLISTPTR_ENTRY(DaqListNumber);

  XCP_PRECONDITION_ASSERT(DaqListNumber < XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(Xcp_DaqIdType),
                          XCP_SID_MAIN_FUNCTION);
#if (XCP_MIN_DAQ > 0)
  RetValue = (DaqListNumber<XCP_MIN_DAQ)?
             &(Xcp_DaqLists.Xcp_NonDynamicDaqLists.Xcp_Daq[DaqListNumber]):
             &(AddressOfTheFirstDynamicDaq[DaqListNumber - XCP_MIN_DAQ]);
#else
  RetValue = &(AddressOfTheFirstDynamicDaq[DaqListNumber]);
#endif

  DBG_XCP_GETDAQLISTPTR_EXIT(RetValue, DaqListNumber);
  return RetValue;
}

FUNC(Xcp_DaqIdType, XCP_CODE) Xcp_GetNoOfDynamicDaqLists( void )
{
  Xcp_DaqIdType RetValue = Xcp_DynamicInfo.NoOfConfiguredDynamicDAQLists;

  DBG_XCP_GETNOOFDYNAMICDAQLISTS_ENTRY();

  DBG_XCP_GETNOOFDYNAMICDAQLISTS_EXIT(RetValue);
  return RetValue;
}

#endif /* (XCP_DAQ_COUNT > 0U) */

#if (XCP_BUILD_CHECKSUM_SUPPORT == STD_ON)
/*--------------------------[Xcp_CalculateCRC]-----------------------------------*/
FUNC(void, XCP_CODE) Xcp_CalculateCRC(void)
{
  DBG_XCP_CALCULATECRC_ENTRY();

#if (XCP_BUILD_CHECKSUM_TYPE == XCP_CRC_USER_CALLOUT)

    Xcp_BuildChecksumData.ApplCalloutResult =
        Xcp_ApplBuildChecksum( Xcp_BuildChecksumData.AddressPtr ,
                               Xcp_BuildChecksumData.BlockSize,
                               &Xcp_BuildChecksumData.ChecksumResult,
                               &Xcp_BuildChecksumData.ChecksumType
                              );
#elif (XCP_BUILD_CHECKSUM_TYPE == XCP_CRC_16_CITT)
      Xcp_BuildChecksumData.ChecksumResult =
          Crc_CalculateCRC16( Xcp_BuildChecksumData.AddressPtr,
                              (Xcp_BuildChecksumData.BlockSize * XCP_ADDRESS_GRANULARITY_SIZE),
                              XCP_CRC_16_CITT_STARTVALUE,
                              TRUE
                              );
      Xcp_BuildChecksumData.ChecksumType = 0x08U;
#else /* then XCP_BUILD_CHECKSUM_TYPE == XCP_CRC_32 */
      Xcp_BuildChecksumData.ChecksumResult =
          Crc_CalculateCRC32( Xcp_BuildChecksumData.AddressPtr,
                              (Xcp_BuildChecksumData.BlockSize * XCP_ADDRESS_GRANULARITY_SIZE),
                              XCP_CRC_32_STARTVALUE,
                              TRUE
                              );
      Xcp_BuildChecksumData.ChecksumType = 0x09U;
#endif

#if (XCP_ASYNCH_CRC_SUPPORT == STD_ON)
  /* Clear the build checksum pending request to signal that the build checksum job is finished */
  Xcp_BuildChecksumData.IsRequestCRCActive = FALSE;
#endif

  DBG_XCP_CALCULATECRC_EXIT();
}
#endif /* (XCP_BUILD_CHECKSUM_SUPPORT == STD_ON) */

#if (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON)
/*-----------------------------[Xcp_EnableCommunicationChannel]--------------------------*/
FUNC(uint8, XCP_CODE) Xcp_EnableCommunicationChannel(
  P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) ParamPtr,
  uint8 Length,
  P2VAR( uint8, AUTOMATIC, XCP_APPL_DATA ) SubCmdResponse,
  uint8 RespLength)
{
  uint8 RetVal;
  DBG_XCP_ENABLE_COMMUNICATION_CHANNEL_ENTRY(ParamPtr, Length, SubCmdResponse, RespLength);
  /* CHECK: NOPARSE */
  if (Length != XCP_DYNAMIC_PDU_CMD_LENGTH)
  {
    /* check if received frame has the expected length. This condition cannot be covered on TRUE
       case because the current configuration does not allow the length to be set to a value
       different from XCP_DYNAMIC_PDU_CMD_LENGTH */
    XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
    RetVal = E_NOT_OK;
  }
  /* CHECK: PARSE */
  /* check if the direction is within expected range */
  else if (ParamPtr[XCP_DYNAMIC_PDU_CMD_STATE] > XCP_DYNAMIC_PDU_CMD_STATE_ENABLED)
  {
    RetVal = E_NOT_OK;
  }
  /* Check if expected direction for is transmission */
  else if (ParamPtr[XCP_DYNAMIC_PDU_CMD_DIRECTION] == XCP_DYNAMIC_PDU_CMD_DIRECTION_TX )
  {
    /* call function to process request for Tx PDU */
    RetVal = Xcp_EnableTxCommunicationChannel(ParamPtr);
  }
  /* Check if expected direction is reception */
  else if (ParamPtr[XCP_DYNAMIC_PDU_CMD_DIRECTION] == XCP_DYNAMIC_PDU_CMD_DIRECTION_RX )
  {
    /* call function to process request for Rx PDU */
    RetVal = Xcp_EnableRxCommunicationChannel(ParamPtr);
  }
  else
  {
    /* Requested direction is invalid - request shall have no effect */
    RetVal = E_NOT_OK;
  }

  TS_PARAM_UNUSED(SubCmdResponse);
  TS_PARAM_UNUSED(RespLength);

  DBG_XCP_ENABLE_COMMUNICATION_CHANNEL_EXIT(RetVal, ParamPtr, Length, SubCmdResponse, RespLength);
  return RetVal;
}
#endif /* (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON) */

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))

/*-----------------------------[Xcp_IsFlxDaqIdAllowedForTheBuffer]--------------------------*/
FUNC(boolean, XCP_CODE) Xcp_IsFlxDaqIdAllowedForTheBuffer
(
  PduIdType PduId,
  uint8 PacketType,
  Xcp_DaqIdType DaqListId
)
{
  uint8 i;
  boolean retValue = FALSE;
  boolean DaqListAssigned = FALSE;

  for (i = 0; i< XCP_NO_OF_FLX_BUF_CONFIGURED; i++)
  {
    if ((Xcp_FlxDaqList[i].FlxDaqAcceptAll == FALSE) &&  (Xcp_FlxDaqList[i].FlxDaqAssignment == DaqListId))
    {
      DaqListAssigned = TRUE;
      break;
    }
  }
  /* Find the buffer associated with the internal PDU ID */
  for (i = 0; i< XCP_NO_OF_FLX_BUF_CONFIGURED; i++)
  {
    if (Xcp_FlxBufConfig[i].FlxPduIdConfIdx == PduId)
    {
      boolean PacketTypeStatus = FALSE;
      /* Check if the buffer is Tx and packet type is DAQ or buffer is Rx and packet type is STIM*/
      if ((PacketType == XCP_SUPPORT_FOR_DAQ_DTO_PDU) &&
          (XCP_IS_FLAG_SET(XCP_BUFFER_DIRECTION_TX_MASK, Xcp_FlxBufConfig[i].Flags,uint8) == TRUE))
      {
        PacketTypeStatus = TRUE;
      }
      else if ((PacketType == XCP_SUPPORT_FOR_STIM_DTO_PDU) &&
               (XCP_IS_FLAG_SET(XCP_BUFFER_DIRECTION_TX_MASK, Xcp_FlxBufConfig[i].Flags,uint8) == FALSE))
      {
        PacketTypeStatus = TRUE;
      }
      else
      {
        /* nothing to do */
      }
      /* Check if the buffer is Tx and packet type is DAQ or buffer is Rx and packet type is STIM*/
      if (PacketTypeStatus == TRUE)
      {
        if (Xcp_FlxDaqList[i].FlxDaqAcceptAll == TRUE)
        {
          /* This buffer has not DAQ list assigned yet, check if the
           * DAQ List is assigned to another buffer. If yes, then that DAQ
           * list must be processed only by the buffers that accept it */
          if (DaqListAssigned != TRUE)
          {
            /* DAQ list is not assigned to any buffer and this buffer can
             * handle any DAQ list, therefore accept request */
            retValue = TRUE;
          }
        }
        else
        {
          if (Xcp_FlxDaqList[i].FlxDaqAssignment == DaqListId)
          {
            retValue = TRUE;
          }
        }
      }
    }
  }
  return retValue;
}
#endif /*  (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)*/


#if (XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON)

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
FUNC_P2CONST(Xcp_MemoryAreaType, AUTOMATIC, XCP_CODE) Xcp_FindMemoryArea
(
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) MemoryAreaAddress,
  uint32 MemoryAreaLength,
  uint8 MemoryAreaType,
  uint8 MemoryAreaScope,
  P2VAR (uint8, AUTOMATIC, XCP_APPL_DATA) MemoryAreaIdx
)
{
  const Xcp_MemoryAreaType* retValue = NULL_PTR;

#if (XCP_NUMBER_OF_MEMORY_AREAS > 0)
  uint8 i;

  for (i = 0U; i< XCP_NUMBER_OF_MEMORY_AREAS; i++)
  {
    if (
         (
           /* !LINKSTO Xcp.ProtectedMemoryLocations.AccessScope,1 */
           (Xcp_MemoryAreas[i].AccessScope == XCP_MEMORY_AREA_SCOPE_ALL) ||
           (Xcp_MemoryAreas[i].AccessScope == MemoryAreaScope)
         ) &&
         (
          /* !LINKSTO   Xcp.ProtectedMemoryLocations.AccessType,1 */
           (Xcp_MemoryAreas[i].AccessType == XCP_MEMORY_AREA_TYPE_READ_WRITE) ||
           (Xcp_MemoryAreas[i].AccessType == MemoryAreaType)
         ) &&
         (
           /* Deviation MISRAC2012-3 <START> */
           (uint32)MemoryAreaAddress >= (uint32)Xcp_MemoryAreas[i].MemoryRangeStartAddress
         ) &&
         (
           ((uint32)MemoryAreaAddress + MemoryAreaLength) <=
           ((uint32)Xcp_MemoryAreas[i].MemoryRangeStartAddress + Xcp_MemoryAreas[i].MemoryRangeLength)
         )
           /* Deviation MISRAC2012-3 <STOP> */
       )
    {
       /* We found a suitable memory range, no need to continue */
       retValue = &Xcp_MemoryAreas[i];
       *MemoryAreaIdx = i;
       break;
    }
  }
#else
  TS_PARAM_UNUSED(MemoryAreaAddress);
  TS_PARAM_UNUSED(MemoryAreaLength);
  TS_PARAM_UNUSED(MemoryAreaType);
  TS_PARAM_UNUSED(MemoryAreaScope);
  TS_PARAM_UNUSED(MemoryAreaIdx);
#endif
  return retValue;
}
#endif

/*-----------------------------[Xcp_IsMemoryAreaAccessible]--------------------------*/
/* !LINKSTO Xcp.ProtectedMemoryLocations.General,1 */
FUNC(boolean, XCP_CODE) Xcp_IsMemoryAreaAccessible
(
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) MemoryAreaAddress,
  uint32 MemoryAreaLength,
  uint8 MemoryAreaType,
  uint8 MemoryAreaScope
)
{
/* !LINKSTO Xcp.Configuration.ProtectedMemoryLocations.XcpMemoryAccessAreasSupport.False,1 */
  boolean retValue = FALSE;
/* !LINKSTO Xcp.Configuration.ProtectedMemoryLocations.XcpMemoryAccessAreasSupport.True,1 */
#if (XCP_NUMBER_OF_MEMORY_AREAS > 0)
  uint8 i;
  /* Search in the configured areas if the requested one is allowed */
  for (i = 0U; i< XCP_NUMBER_OF_MEMORY_AREAS; i++)
  {
    if (
         (
           /* !LINKSTO Xcp.ProtectedMemoryLocations.AccessScope,1 */
           (Xcp_MemoryAreas[i].AccessScope == XCP_MEMORY_AREA_SCOPE_ALL) ||
           (Xcp_MemoryAreas[i].AccessScope == MemoryAreaScope)
         ) &&
         (
          /* !LINKSTO   Xcp.ProtectedMemoryLocations.AccessType,1 */
           (Xcp_MemoryAreas[i].AccessType == XCP_MEMORY_AREA_TYPE_READ_WRITE) ||
           (Xcp_MemoryAreas[i].AccessType == MemoryAreaType)
         ) &&
         (
           /* Deviation MISRAC2012-3 <START> */
           (uint32)MemoryAreaAddress >= (uint32)Xcp_MemoryAreas[i].MemoryRangeStartAddress
         ) &&
         (
           ((uint32)MemoryAreaAddress + MemoryAreaLength) <=
           ((uint32)Xcp_MemoryAreas[i].MemoryRangeStartAddress + Xcp_MemoryAreas[i].MemoryRangeLength)
         )
           /* Deviation MISRAC2012-3 <STOP> */
       )
    {
       /* We found a suitable memory range, no need to continue */
       retValue = TRUE;
       break;
    }
  }
/* !LINKSTO Xcp.Configuration.ProtectedMemoryLocations.XcpMemoryAccessAreasCalloutSupport.True,1 */
#elif(XCP_MEMORY_AREAS_ACCESS_CALLOUT == STD_ON)
  retValue = Xcp_ApplIsMemoryAreaAccessible(MemoryAreaAddress, MemoryAreaLength, MemoryAreaType,
                                            MemoryAreaScope);
#else
  TS_PARAM_UNUSED(MemoryAreaAddress);
  TS_PARAM_UNUSED(MemoryAreaLength);
  TS_PARAM_UNUSED(MemoryAreaType);
  TS_PARAM_UNUSED(MemoryAreaScope);
#endif /* XCP_NUMBER_OF_MEMORY_AREAS > 0 */

  return retValue;
}
#endif /* XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON */

/*==================[internal function definitions]=========================*/

/*-----------------------------[Xcp_GetLengthToBeReserved]-----------------------------*/
STATIC FUNC(uint8, XCP_CODE) Xcp_GetLengthToBeReserved
(
  uint8 CommandId
)
{
  /* The calculated length to be reserved in the CTO response queue */
  uint8 ReserveLength;

  DBG_XCP_GETLENGTHTOBERESERVED_ENTRY(CommandId);

  /* If command is UPLOAD or SHORT_UPLOAD the length depends on the command parameters */
  if ((CommandId == XCP_CMD_UPLOAD_PID) || (CommandId == XCP_CMD_SHORT_UPLOAD_PID))
  {
    ReserveLength = XCP_MIN(
                    ((XCP_ADDRESS_GRANULARITY_SIZE * XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX))
                     + XCP_ADDRESS_GRANULARITY_SIZE),
                     Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto
                   );
  }
  /* If user commands configured */
#if (XCP_NUMBER_OF_USERCOMMANDS > 0U)
  else if (CommandId == XCP_CMD_USERCMD_PID)
  {
    uint8 SubCommandIndex;
    uint8 StatusVal = XCP_E_NOT_OK;
    for (SubCommandIndex = 0; SubCommandIndex < XCP_NUMBER_OF_USERCOMMANDS ; SubCommandIndex++)
    {
      /* retrieve the command */
      if (XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX) ==  Xcp_UserCommandConf[SubCommandIndex].SubCmdCode)
      {
        StatusVal = XCP_E_OK;
        /* exit loop  if the command was found*/
        break;
      }
    }
    if (StatusVal == XCP_E_OK)
    {
      /* Total reserved length shall be the configured length plus one byte for positive response */
      ReserveLength =
           Xcp_UserCommandConf[SubCommandIndex].SubCmdRespLength +
           Xcp_Commands[CommandId - XCP_CMD_PID_BASE].CommandMaxResponseLength;
    }
    else
    {
      /* Total reserved length shall be CommandMaxResponseLength */
      ReserveLength =
           Xcp_Commands[CommandId - XCP_CMD_PID_BASE].CommandMaxResponseLength;
    }
  }
#endif /* (XCP_NUMBER_OF_USERCOMMANDS > 0U) */
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
  else if ((Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].ConnectionType == XCP_CONNECTION_OVER_UDPIP) &&
           XCP_PID_IS_TRANSPORT_LAYER(CommandId) &&
           XCP_SUBPID_IS_GET_SLAVE_ID(XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX)))
  {
    /* GET_SLAVE_ID response size is different on Ethernet than on CAN */
    ReserveLength = XCP_SUBCMD_GET_SLAVE_ID_ON_ETH_RES_LENGTH;
  }
#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */
  else
  {
    /* Get the length statically */
    ReserveLength = Xcp_Commands[CommandId - XCP_CMD_PID_BASE].CommandMaxResponseLength;
  }
  DBG_XCP_GETLENGTHTOBERESERVED_EXIT(ReserveLength,CommandId);
  return ReserveLength;
}

/*--------------------------[Xcp_Disconnect]--------------------------------*/

FUNC(void, XCP_CODE) Xcp_Disconnect(void)
{
  DBG_XCP_DISCONNECT_ENTRY();

  /* Flush command queue */
  Xcp_FlushCMDQueue();
  /* Terminate session */
  Xcp_TerminateSession();
  /* Initialize receive processor */
  Xcp_InitReceiveProcessor();
  /* Initialize command processor */
  Xcp_InitCommandProcessor();
  /* If DAQ/STIM resource is supported */
#if (XCP_MAX_DAQ != 0U)
  /* Reset DAQList runtime information */
  Xcp_ResetEventProcessor();
#endif
  /* Initialize transmit processor */
  Xcp_InitTransmitProcessor();
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
  /* Reset the Id of active connection partition */
  Atomics_Store(&Xcp_PartitionOfActiveConnection, XCP_NUM_OF_OS_APPS);
#endif

  DBG_XCP_DISCONNECT_EXIT();
}

/*-----------------------------[Xcp_ReserveBytesToRespCTOQueue]-------------*/

STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_ReserveBytesToRespCTOQueue
(
  Xcp_CtoNoOfBytesType ReserveBytes
)
{
  /* Local variable to hold the number of bytes that need to be reserved plus the length (1 byte)
   * plus Multicast flag (0 or 1 byte) */
  Xcp_CtoNoOfBytesType ReserveBytesPlusLength = ReserveBytes + XCP_RESP_CTO_QUEUE_LENGTH_SIZE +
                                                XCP_RESP_CTO_QUEUE_MULTICAST_SIZE;
  /* Local variable to hold return value */
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_XCP_RESERVEBYTESTORESPCTOQUEUE_ENTRY(ReserveBytes);

  /* Check if there is enough free space into the Response CTO queue */
  if ( ReserveBytesPlusLength <= XCP_GET_CTOQUEUE_FREESPACE(Xcp_CtoNoOfBytesType) )
  {
    /* Enough free space: set the ReservedBytes variable */
    Xcp_CtoQueue.ReservedBytes += ReserveBytesPlusLength;
    /* Set the return value to E_OK */
    RetVal = E_OK;
  }

  /* Return E_OK or E_NOT_OK */

  DBG_XCP_RESERVEBYTESTORESPCTOQUEUE_EXIT(RetVal,ReserveBytes);
  return RetVal;
}

#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) ||\
     (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))
#if((XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC_MASK) &&\
    (XCP_MIN_DAQ < XCP_NO_OF_NON_DYNAMIC_DAQ_LISTS))

/*------------------------[Xcp_ClearODTEntries]-----------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_ClearODTEntries
(
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
  P2VAR(Xcp_OdtEntryType, AUTOMATIC, XCP_APPL_DATA) ODTEntryPtr,
#else
  P2VAR(usize, AUTOMATIC, XCP_APPL_DATA) ODTEntryPtr,
#endif
  uint8 NoOfODTEntries
)
{
  /* Local variable to keep track of the cleared ODT entries */
  uint8 NoOfClearedODTEntries;

  DBG_XCP_CLEARODTENTRIES_ENTRY(ODTEntryPtr,NoOfODTEntries);

  /* Clear all ODT Entries */
  for (NoOfClearedODTEntries = 0U;
       NoOfClearedODTEntries < NoOfODTEntries;
       ++NoOfClearedODTEntries)
  {
    /* Clear the pointer to the data element as per ASAM specifications
     *
     * Req ID. Xcp.ASAM.CLEARDAQLIST003  */
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
    ODTEntryPtr[NoOfClearedODTEntries].Address   = NULL_PTR;
    /* Clear bit offset */
    ODTEntryPtr[NoOfClearedODTEntries].BitOffset = 0xFFU;
    /* Clear address extension */
    ODTEntryPtr[NoOfClearedODTEntries].AddrExtn  = 0U;
    /* Reset data element length */
    ODTEntryPtr[NoOfClearedODTEntries].Length    = 0U;
#else
    *(uint32*)Xcp_GetOdtEntryData(ODTEntryPtr, NoOfClearedODTEntries, XCP_ODT_ENTRY_FIELD_ADDRESS) = 0U;
    *(uint8*)Xcp_GetOdtEntryData(ODTEntryPtr, NoOfClearedODTEntries, XCP_ODT_ENTRY_FIELD_LENGTH) = 0U;
#if (XCP_ENABLE_BIT_OFFSET == STD_ON)
    *((uint8*)Xcp_GetOdtEntryData(ODTEntryPtr,NoOfClearedODTEntries,XCP_ODT_ENTRY_FIELD_BITOFFSET)) = 0xFFU;
#endif
#endif
  }

  DBG_XCP_CLEARODTENTRIES_EXIT(ODTEntryPtr,NoOfODTEntries);
}

/*----------------------------[Xcp_ClearODTs]--------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_ClearODTs
(
  P2CONST(Xcp_OdtType, AUTOMATIC, XCP_APPL_DATA) ODTPtr,
  uint8                                        NoOfODTs
)
{
  /* Local variable to keep track of the cleared ODTs */
  uint8 NoOfClearedODTs;

  DBG_XCP_CLEARODTS_ENTRY(ODTPtr,NoOfODTs);

  /* Clear all ODTs */
  for (NoOfClearedODTs = 0; NoOfClearedODTs < NoOfODTs; ++NoOfClearedODTs)
  {
    /* Clear all ODT Entries */
    Xcp_ClearODTEntries(
      ODTPtr[NoOfClearedODTs].OdtEntry,
      ODTPtr[NoOfClearedODTs].NrOfOdtEntries);
  }

  DBG_XCP_CLEARODTS_EXIT(ODTPtr,NoOfODTs);
}

#endif /* (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC_MASK) &&
          XCP_MIN_DAQ < XCP_NO_OF_NON_DYNAMIC_DAQ_LISTS */
#endif /* ( (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) ||
            (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK) ) */

/*-----------------------------[Xcp_InsertResponseCTO]-----------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_InsertResponseCTO
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA) CtoPtr
)
{
  /* Local variable index CTO bytes */
  uint8 CtoByteIndex;

  DBG_XCP_INSERTRESPONSECTO_ENTRY(CtoPtr);

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
  /* Enqueue multicast byte */
  Xcp_EnqueueByteInCtoQueue(XCP_CTO_IS_NOT_MULTICAST);
#endif
  /* Enqueue CTO length */
  Xcp_EnqueueByteInCtoQueue(CtoPtr->Length);
  /* Enqueue each byte in CTO into CTO Queue */
  for (CtoByteIndex = 0U; CtoByteIndex < CtoPtr->Length; CtoByteIndex++)
  {
    Xcp_EnqueueByteInCtoQueue(CtoPtr->CTO[CtoByteIndex]);
  }

  DBG_XCP_INSERTRESPONSECTO_EXIT(CtoPtr);
}

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)

/*-----------------------------[Xcp_InsertMulticastResponseCTO]---------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_InsertMulticastResponseCTO
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA) CtoPtr
)
{
  /* Local variable index CTO bytes */
  uint8 CtoByteIndex;

  DBG_XCP_INSERTMULTICASTRESPONSECTO_ENTRY(CtoPtr);

  /* Enqueue multicast byte */
  Xcp_EnqueueByteInCtoQueue(XCP_CTO_IS_MULTICAST);
  /* Enqueue CTO length */
  Xcp_EnqueueByteInCtoQueue(CtoPtr->Length);
  /* Enqueue each byte in CTO into CTO Queue */
  for (CtoByteIndex = 0U; CtoByteIndex < CtoPtr->Length; CtoByteIndex++)
  {
    Xcp_EnqueueByteInCtoQueue(CtoPtr->CTO[CtoByteIndex]);
  }

  DBG_XCP_INSERTMULTICASTRESPONSECTO_EXIT(CtoPtr);
}

#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */

/*--------------------------[Xcp_TerminateSession]---------------------------*/

FUNC(void, XCP_CODE) Xcp_TerminateSession(void)
{
  DBG_XCP_TERMINATESESSION_ENTRY();

  /* Flush CTO Queue */
  Xcp_FlushCTOQueue();
  /* If event packet transmission enabled */
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
  /* Flush XCP events CTOs */
  Xcp_FlushEventCTOQueue();
#endif

/* If DAQ or STIM resource is supported */
#if (XCP_MAX_DAQ != 0U)

  /* Reset event channels runtime information */
  Xcp_ResetEventChannels();

  /* Flush DTO Queue */
  Xcp_FlushDTORunTimeData();

#endif /* (XCP_MAX_DAQ != 0U) */

  /* release II'ed lock of the connection resources */
  Xcp_ConnectionStatus.State = XCP_STATE_DISCONNECTED;
  Xcp_ConnectionStatus.Flags = 0U;

  DBG_XCP_TERMINATESESSION_EXIT();
}

/*----------------[Xcp_RemoveDAQListFromEventChannel]------------------------*/
/* If DAQ/STIM resource is supported */
#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) || \
     (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))

STATIC FUNC(void, XCP_CODE) Xcp_RemoveDAQListFromEventChannel
(
  Xcp_DaqIdType DaqListNumber,
  uint16 EventChannelNumber
)
{
  /* Local variable to hold daq list index with respect to other DAQ lists
   * associated with the event */
  uint16 DaqListIndex = 0U;
  /* Local variable to hold address of the requested Event channel
   * parameters */
  Xcp_EventType *EventParamPtr = &Xcp_Event[EventChannelNumber];

  DBG_XCP_REMOVEDAQLISTFROMEVENTCHANNEL_ENTRY(DaqListNumber,EventChannelNumber);

  /* Find DAQ List index from DAQ lists associated with the event. */

  while ( (DaqListNumber != EventParamPtr->DaqIdListPtr[DaqListIndex]) &&
          (DaqListIndex != EventParamPtr->DaqIdListCount)
        )
  {
    /* Verify next */
    DaqListIndex++;
  }
  /* Dis-associate DAQList from the event channel */
  /* Decrement number of lists associated with the event channel */
  /* CHECK: NOPARSE */
  if (0U < XCP_GET_EVENT_NO_OF_DAQLIST(EventChannelNumber))
  /* CHECK: PARSE */
  {
    XCP_GET_EVENT_NO_OF_DAQLIST(EventChannelNumber)--;
  }
  /* CHECK: NOPARSE */
  else
  {
    /* Unreachable else branch. This condition can be reached in the following situations:
       * from Xcp_ProcessCmdClearDAQList() context
       * from Xcp_ProcessSetDAQListMode() context
       * from Xcp_ProcessCmdFreeDAQ() context
       In all the situations mentioned above a DAQ list is disassociated from a current event
       channel only after a check is made to see if that current DaqList is found in the Xcp_Event
       structure. Once it is disassociated, it cannot re-enter to check if the number of DAQ Lists
       currently associated with event channel is zero, because the same if-check is performed
       previously in the function calls mentioned above.*/
    XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
  }
  /* CHECK: PARSE */
  /* Move the DAQ list with same or less priority up.  Note: The list is
   * already sorted */
  while (DaqListIndex <
          XCP_GET_EVENT_NO_OF_DAQLIST(EventChannelNumber))
  {
    /* Move the lists up */
    EventParamPtr->DaqIdListPtr[DaqListIndex] =
       EventParamPtr->DaqIdListPtr[DaqListIndex + 0x1U];
    /* Move to  next */
    DaqListIndex++;
  }

  DBG_XCP_REMOVEDAQLISTFROMEVENTCHANNEL_EXIT(DaqListNumber,EventChannelNumber);
}

/*----------------[Xcp_AssociateDAQListToEventChannel]-----------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_AssociateDAQListToEventChannel
(
  Xcp_DaqIdType DaqListNumber,
  uint16 EventChannelNumber
)
{
  /* Local variable to hold insert location index */
  uint8 InsertDaqListIndex = XCP_GET_EVENT_NO_OF_DAQLIST(EventChannelNumber);
  /* Local variable to hold address of the requested Event channel
   * parameters */
  Xcp_EventType *EventParamPtr = &Xcp_Event[EventChannelNumber];

  DBG_XCP_ASSOCIATEDAQLISTTOEVENTCHANNEL_ENTRY(DaqListNumber,EventChannelNumber);

  /* Check whether there is any DAQ List associated with the event */
  if (XCP_GET_EVENT_NO_OF_DAQLIST(EventChannelNumber) > 0U)
  {
    /* Local variable to hold address of the requested DAQ List's parameters */
    P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListToBeAssociatedPtr = XCP_GET_DAQ_LIST_PTR(DaqListNumber);

    /* Insert the DAQ list number in the event's list of associated DAQ lists based on its priority.
     * We assume that the list is already sorted in a decreasing order. */
    for (; InsertDaqListIndex > 0U; InsertDaqListIndex--)
    {
      /* Get the nearest left DAQ list from the current index */
      P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr =
          XCP_GET_DAQ_LIST_PTR(EventParamPtr->DaqIdListPtr[InsertDaqListIndex-1U]);
      if (DaqListToBeAssociatedPtr->Priority <= DaqListPtr->Priority)
      {
        /* DAQ list to be associated is at the right position to be inserted. */
        break;
      }
      else
      {
        /* Move the queried DAQ list to the right as the priority is lower than the one the new
         * DAQ list has */
        EventParamPtr->DaqIdListPtr[InsertDaqListIndex] = EventParamPtr->DaqIdListPtr[InsertDaqListIndex-1U];
      }
    }
  }

  /* Insert the DAQ List  */
  EventParamPtr->DaqIdListPtr[InsertDaqListIndex] = DaqListNumber;
  /* Update the number of lists associated with the event */
  XCP_GET_EVENT_NO_OF_DAQLIST(EventChannelNumber)++;

  DBG_XCP_ASSOCIATEDAQLISTTOEVENTCHANNEL_EXIT(DaqListNumber,EventChannelNumber);
}

#endif /* (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) */
/*-----------------------[Xcp_EnqueueByteInCtoQueue]----------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_EnqueueByteInCtoQueue(uint8 DataByte)
{
  DBG_XCP_ENQUEUEBYTEINCTOQUEUE_ENTRY(DataByte);

  /* Enqueue the byte*/
  Xcp_CtoQueue.Queue[Xcp_CtoQueue.Tail] = DataByte;
  /* Update the tail */
  Xcp_CtoQueue.Tail++;
  Xcp_CtoQueue.Tail %= XCP_CTO_QUEUE_SIZE;
  /* Update number of bytes in CTO Queue */
  Xcp_CtoQueue.NoOfBytes++;

  DBG_XCP_ENQUEUEBYTEINCTOQUEUE_EXIT(DataByte);
}

/*----------------------------[Xcp_DeQueueCmdQueue]--------------------------*/

/* DeQueue first command from command queue. */
STATIC FUNC(void, XCP_CODE) Xcp_DeQueueCmdQueue(void)
{
  DBG_XCP_DEQUEUECMDQUEUE_ENTRY();

  /* If queue is not empty */
  if (0U != XCP_GET_NO_OF_COMMANDS())
  {
    /* Update Queue Head */
    Xcp_CommandQueue.Head++;
    Xcp_CommandQueue.Head %= XCP_CMD_PROCESSOR_QUEUE_SIZE;
    /* Update number of commands */
    Xcp_CommandQueue.NoOfCmds--;
  }

  DBG_XCP_DEQUEUECMDQUEUE_EXIT();
}

/*----------------------------[Xcp_IsCtoQueueEmpty]----------------------------*/

FUNC(boolean, XCP_CODE) Xcp_IsCtoQueueEmpty(void)
{
  boolean RetVal;
  DBG_XCP_ISCTOQUEUEEMPTY_ENTRY();
  RetVal = ((0U == Xcp_CtoQueue.NoOfBytes)? TRUE:FALSE);
  DBG_XCP_ISCTOQUEUEEMPTY_EXIT(RetVal);
  return RetVal;
}

/*----------------------------[Xcp_GetCtoLength]----------------------------*/

FUNC(PduLengthType, XCP_CODE) Xcp_GetCtoLength(void)
{
  PduLengthType RetVal;
  DBG_XCP_GETCTOLENGTH_ENTRY();

  RetVal = (Xcp_CtoQueue.Queue[Xcp_CtoQueue.Head + XCP_RESP_CTO_QUEUE_MULTICAST_SIZE]);

  DBG_XCP_GETCTOLENGTH_EXIT(RetVal);
  return RetVal;
}

/*---------------------------[Xcp_DeQueueCtoQueue]---------------------------*/

/* DeQueue first CTO from CTO queue. */
FUNC(void, XCP_CODE) Xcp_DeQueueCtoQueue
(
  PduLengthType DataLength,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) BufferPtr
)
{
  /* Iterator */
  PduLengthType DataIndex;

  DBG_XCP_DEQUEUECTOQUEUE_ENTRY(DataLength, BufferPtr);

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
  /* Skip over the multicast and length bytes */
  Xcp_CtoQueue.Head += 2U;
  /* Update total number of bytes in CTO queue */
  Xcp_CtoQueue.NoOfBytes -= 2U;
#else
  /* Skip over the length bytes */
  ++Xcp_CtoQueue.Head;
  /* Update total number of bytes in CTO queue */
  Xcp_CtoQueue.NoOfBytes--;
#endif
  /* Check whether queue head needs to be rolled over */
  Xcp_CtoQueue.Head %= XCP_CTO_QUEUE_SIZE;

  /* Copy CTO bytes until all bytes are copied */
  for(DataIndex = 0U; DataIndex < DataLength; DataIndex++)
  {
    /* Copy CTO bytes to user buffer */
    BufferPtr[DataIndex] = Xcp_CtoQueue.Queue[Xcp_CtoQueue.Head];
    /* Update total number of bytes in CTO queue */
    Xcp_CtoQueue.NoOfBytes--;
    /* Check whether queue head needs to be rolled over */
    ++Xcp_CtoQueue.Head;
    Xcp_CtoQueue.Head %= XCP_CTO_QUEUE_SIZE;
  }

  DBG_XCP_DEQUEUECTOQUEUE_EXIT(DataLength, BufferPtr);
}

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)

/*---------------------------[ Xcp_IsMulticastResponse]---------------------------*/

FUNC(boolean, XCP_CODE) Xcp_IsMulticastResponse(void)
{
  DBG_XCP_ISMULTICASTRESPONSE_ENTRY();

  DBG_XCP_ISMULTICASTRESPONSE_EXIT(Xcp_CtoQueue.Queue[Xcp_CtoQueue.Head] == XCP_CTO_IS_MULTICAST);

  return (Xcp_CtoQueue.Queue[Xcp_CtoQueue.Head] == XCP_CTO_IS_MULTICAST);
}

#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */

/*---------------------------[Xcp_ProcessCmd]-------------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_ProcessCmd(void)
{
  /* Local variable to hold command ID */
  const uint8 Command = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE0_INDEX);
  /* Variable to hold the error message, if any */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMD_ENTRY();

  /* Initialize the response */
  ResponseCTO.Length = 0U;
  ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_ERR_PID;

  if (Xcp_CommandCanBeExecuted(Command, &ResponseCTO))
  {
    /* Execute the command */
    const Xcp_ErrorType cmdProcError = Xcp_ExecuteCommand(Command);
    /* Enter critical section.*/
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    /* Check the return value of the command execution */
    switch (cmdProcError)
    {
      case XCP_E_BUSY:
        /* Command processing can't be finished in this cycle, trigger state change */
        XCP_CMD_PROCESSOR_STATE_TRANS(XCP_CMDPROCESSOR_BUSY);
        break;
      case XCP_E_OK:
      case XCP_E_NOT_OK:
        /* Command processing finished, dequeue the command from command queue */
        Xcp_DeQueueCmdQueue();
        /* Trigger state transition */
        XCP_CMD_PROCESSOR_STATE_TRANS(XCP_CMDPROCESSOR_IDLE);
        break;
      case XCP_E_INSUFFICIENT_MEMORY:
        /* Do nothing, keep the command in queue until the CTO response queue has enough space
         * to queue the response. */
        break;
      /* CHECK: NOPARSE */
      default:
        /* all possible cases have been explicitly covered in 'case's above, the 'default'
         * cannot be reached */
         XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
        break;
      /* CHECK: PARSE */
    }
    /* Exit critical section.*/
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  }
  else
  {
    /* If we have a response to send */
    /* CHECK: NOPARSE */
    if (ResponseCTO.Length > 0U)
    /* CHECK: PARSE */
    {
      /* The command didn't passed the validation, abort with error */
      Xcp_AbortCommandWithError(&ResponseCTO);

#if ((XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) || \
     (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON))
      /* Enter critical section.*/
      SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

#if (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)
      if (Command == XCP_CMD_DOWNLOAD_PID)
      {
        /* The block mode was put as active by the receive processor but the DOWNLOAD command
         * is not allowed to execute, therefore we must clear the block mode flag */
        Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_DOWNLOAD_BLOCK_MODE_MASK);
      }
#endif /* (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) */

#if (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)
      if (Command == XCP_CMD_PROGRAM_PID)
      {
        /* The block mode was put as active by the receive processor but the PROGRAM command
         * is not allowed to execute, therefore we must clear the block mode flag */
        Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_PGM_BLOCK_MODE_MASK);
      }
#endif /* (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON) */

      /* Exit critical section.*/
      SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
#endif /* ((XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) || \
        *  (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)) */
    }
    /* CHECK: NOPARSE */
    else
    {
      /* This else branch is unreachable. The following scenario should occur:
         A command should be sent, but is should not be executed and no response
         should be enqueued. This case is not possible with  the current implementation because
         if a command is sent and cannot be executed, a negative response is always enqueued
         into the response CTO. */
      XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
    }
    /* CHECK: PARSE */
  }

  DBG_XCP_PROCESSCMD_EXIT();
}

/*-----------------------[Xcp_ExecuteCommand]--------------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ExecuteCommand(uint8 Command)
{
  /* Function return value */
  Xcp_ErrorType RetValue;
  /* The result of byte reservation function */
  Std_ReturnType ReservationResult;

  /* Determine the number of bytes to be reserved in the CTO response queue for this command */
  uint8 ReserveLength = Xcp_GetLengthToBeReserved(Command);

  DBG_XCP_EXECUTECOMMAND_ENTRY(Command);

  /* Enter critical area */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Reserve bytes to the response CTO Queue */
  ReservationResult = Xcp_ReserveBytesToRespCTOQueue( ReserveLength );

  /* Exit critical area */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  /* Check if the response reservation has been successful */
  if ( ReservationResult == XCP_OK )
  { /* Response CTO bytes have been reserved into the response CTO queue */
    /* Execute the command handler */
    RetValue = (Xcp_Commands[Command - XCP_CMD_PID_BASE].CommandHandlerPtr)();

    /* Enter critical area */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* Remove the reserved bytes from the response CTO queue. The cast is safe as it will be
     * cast to the same type or greater. */
    XCP_RESP_CTO_QUEUE_FREE_RESERVED( (Xcp_CtoNoOfBytesType) ReserveLength );

    /* Exit critical area */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  }
  else
  {
    /* Command was not executed, wait for the CTO response queue to get additional free bytes */
    RetValue = XCP_E_INSUFFICIENT_MEMORY;
  }

  DBG_XCP_EXECUTECOMMAND_EXIT(RetValue,Command);
  return RetValue;
}

/*--------------------------[Xcp_ProcessCmdConnect]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdConnect(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDCONNECT_ENTRY();

  /* Check whether mode parameter is valid */
  switch (XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX))
  {
    case XCP_CONNECT_MODE_NORMAL:
      /* Prepare CTO with positive response */
      ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
      ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_RESOURCE_SUPPORTED;
      ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = XCP_COMM_MODE_BASIC;
      ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto;
      Xcp_WriteWordToByteArray(&ResponseCTO.CTO[XCP_RES_BYTE4_INDEX], Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Dto);
      ResponseCTO.CTO[XCP_RES_BYTE6_INDEX] = XCP_PROTOCOL_LAYER_MAJOR_VN;
      ResponseCTO.CTO[XCP_RES_BYTE7_INDEX] = XCP_TRANSPORT_LAYER_MAJOR_VN;
      ResponseCTO.Length = XCP_CMD_CONNECT_RES_LENGTH;

      /* apply the II'th level lock of the connection resources */
      Xcp_ConnectionStatus.State = XCP_STATE_CONNECTED;
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
      Atomics_Store(&Xcp_PartitionOfActiveConnection, (uint8)GetApplicationID());
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
      /* !LINKSTO Xcp.dsn.BswDistribution.EventProcessing.SharedMirror.NewConnection,1 */
      /* Reset all request data */
      Xcp_ResetRequestData();
#endif

#endif
      /* Clear the flag that the TP configuration was done, since now we are connected */
      XCP_CLEAR_FLAG_U8_ATOMIC(XCP_TP_CONFIGURATION_DONE_BIT, Xcp_ConnectionStatus.Flags);

    break;
    case XCP_CONNECT_MODE_USERDEFINED:
      /* Shall be implemented later */
    default:
      /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_CMD_SYNTAX_LENGTH);
    break;
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDCONNECT_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------------[Xcp_ProcessCmdDisConnect]-----------------------*/

STATIC FUNC(Std_ReturnType, XCP_CODE)  Xcp_ProcessCmdDisConnect(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  /* Standard return value */
  Std_ReturnType RetValue = E_NOT_OK;

#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
  boolean AtLeastOneEventMainFunctionProcessing = Xcp_IsAtLeastOneEventMainFunctionProcessing();
#endif

  DBG_XCP_PROCESSCMDDISCONNECT_ENTRY();

  /* find out if there is a pending storage/clearing of DAQ lists in the NV memory
     or there is  an event main function currently running that also has
     DAQ lists associated running */
 #if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
  if (((Xcp_Status.SessionStatus & XCP_MASK_CLEAR_DAQ_REQ) != 0U ) ||
      ((Xcp_Status.SessionStatus & XCP_MASK_STORE_DAQ_REQ) != 0U ) ||
    /* !LINKSTO Xcp.DISCONNECT.BusyWhenEventMFIsProcessing,1 */
      (AtLeastOneEventMainFunctionProcessing == TRUE))
#else
  if (((Xcp_Status.SessionStatus & XCP_MASK_CLEAR_DAQ_REQ) != 0U ) ||
      ((Xcp_Status.SessionStatus & XCP_MASK_STORE_DAQ_REQ) != 0U ))
#endif

  {
    /* Prepare CTO with negative response ERR_CMD_BUSY */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_BUSY, XCP_ERR_CMD_BUSY_LENGTH);
    /* Clear pending status of DISCONNECT command */
    Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_DISCONNECT_PENDING_MASK);
  }
  else
  {
    /* Perform an XCP disconnect */
    Xcp_Disconnect();

    /* Prepare CTO with positive response */
    ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
    ResponseCTO.Length = XCP_CMD_DISCONNECT_RES_LENGTH;
    RetValue = E_OK;
  }

  /* Queue the response */
  Xcp_InsertResponseCTO(&ResponseCTO);

  DBG_XCP_PROCESSCMDDISCONNECT_EXIT(RetValue);

  return RetValue;
}

/*--------------------------[Xcp_ProcessCmdGetId]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetId(void)
{
  /* Retrieve requested Identification Type  */
  uint8 IdentificationType = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDGETID_ENTRY();

  switch(IdentificationType)
  {
    /* ASAM-MC2 filename without path and extension */
    case XCP_IDENTIFICATION_TYPE1:
    {
#if (XCP_GET_ID_TYPE1_CALLOUT_ENABLED == STD_ON)
      uint8 IdLength;
#endif

      /* Prepare CTO with positive response */
      ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
      ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_IDENTIFICATION_TRANSFER_MODE_1_MASK;
      ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = XCP_RESERVED_BYTE;
      ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = XCP_RESERVED_BYTE;

#if (XCP_GET_ID_TYPE1_CALLOUT_ENABLED == STD_ON)
      /* !LINKSTO Xcp.GET_ID.UserCallout,1 */
      /* !LINKSTO Xcp.GET_ID.UserCallout.ReturnValue.APPL_OK,1 */
      if (Xcp_ApplGetIdentification(Xcp_GetIdBuffer, &IdLength) == XCP_APPL_OK)
      {
        if ((IdLength + XCP_CMD_GETID_RES_LENGTH_MIN) > Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto)
        {
          /* If response length is greater than a single CTO size */
          ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_IDENTIFICATION_TRANSFER_MODE_0_MASK;
          /* Set MTA */
          Xcp_MTA.Type         = XCP_MTATYPE_ADDRESS;
          Xcp_MTA.Address.Read = Xcp_GetIdBuffer;
          Xcp_MTA.IsSet        = TRUE;
          /* Set response data length to MAX_CTO */
          ResponseCTO.Length   = XCP_CMD_GETID_RES_LENGTH_MIN;
        }
        else
        {
          /* If response length is less than a single CTO size */
          /* Copy identification field bytes to response CTO */
          TS_MemCpy(
            &(ResponseCTO.CTO[XCP_RES_BYTE8_INDEX]),
            Xcp_GetIdBuffer,
            IdLength);
          /* Set response length */
          ResponseCTO.Length = (uint8)(XCP_CMD_GETID_RES_LENGTH_MIN + IdLength);
        }
      }
      else
      {
        IdLength = 0U;
        /* !LINKSTO Xcp.GET_ID.UserCallout.ReturnValue.NegativeResponse,1 */
        /* Set response length */
        ResponseCTO.Length = XCP_CMD_GETID_RES_LENGTH_MIN;
      }

      /* Fill the identification type length */
      Xcp_WriteDwordToByteArray(
        &(ResponseCTO.CTO[XCP_RES_BYTE4_INDEX]),
        IdLength);
#else /* (XCP_GET_ID_TYPE1_CALLOUT_ENABLED == STD_OFF) */

#if (XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH > 0)
      /* Fill the identification type length */
      Xcp_WriteDwordToByteArray(
        &(ResponseCTO.CTO[XCP_RES_BYTE4_INDEX]),
        XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH);

      if ((XCP_CMD_GETID_RES_LENGTH_MIN + XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH) >
           Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto)
      {
        /* If response length is greater than a single CTO size */
        ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_IDENTIFICATION_TRANSFER_MODE_0_MASK;
        /* Set MTA */
        Xcp_MTA.Type         = XCP_MTATYPE_ADDRESS;
        Xcp_MTA.Address.Read = Xcp_IdentificationTypeOne;
        Xcp_MTA.IsSet        = TRUE;
        /* Set response data length to MAX_CTO */
        ResponseCTO.Length   = XCP_CMD_GETID_RES_LENGTH_MIN;
      }
      else
      {
        /* If response length is less than a single CTO size */
        /* Copy identification field bytes to response CTO */
        TS_MemCpy(
          &(ResponseCTO.CTO[XCP_RES_BYTE8_INDEX]),
          Xcp_IdentificationTypeOne,
          XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH);
        /* Set response length */
        ResponseCTO.Length = (uint8)(XCP_CMD_GETID_RES_LENGTH_MIN + XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH);
      }
#else
      /* Set response length */
      ResponseCTO.Length = (uint8)(XCP_CMD_GETID_RES_LENGTH_MIN + XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH);
#endif /* (XCP_GET_ID_TYPE1_ASAM_FILENAME_LENGTH > 0) */
#endif /* (XCP_GET_ID_TYPE1_CALLOUT_ENABLED == STD_ON) */
    }
    break;
    /* ASCII text */
    case XCP_IDENTIFICATION_TYPE0:
    /* ASAM-MC2 filename with path and extension */
    case XCP_IDENTIFICATION_TYPE2:
    /* URL where the ASAM-MC2 file can be found */
    case XCP_IDENTIFICATION_TYPE3:
    /* ASAM-MC2 file to upload */
    case XCP_IDENTIFICATION_TYPE4:
    /* User defined */
    default: /* Prepare CTO with positive response */
      ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
      ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_IDENTIFICATION_TRANSFER_MODE_1_MASK;
      ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = XCP_RESERVED_BYTE;
      ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = XCP_RESERVED_BYTE;
      /* Since requested identification type is not supported return length
       * 0(zero)*/
      Xcp_WriteDwordToByteArray(
        &(ResponseCTO.CTO[XCP_RES_BYTE4_INDEX]), 0U);
      /* Set response length */
      ResponseCTO.Length = XCP_CMD_GETID_RES_LENGTH_MIN;
    break;
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETID_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------------[Xcp_ProcessCmdGetCommModeInfo]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetCommModeInfo(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDGETCOMMMODEINFO_ENTRY();

  /* Prepare CTO with positive response */
  ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
  ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_RESERVED_BYTE;
  ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = XCP_COMM_MODE_OPTIONAL;
  ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = XCP_RESERVED_BYTE;
  ResponseCTO.CTO[XCP_RES_BYTE4_INDEX] = XCP_GET_CONNECTION_MAX_BS;
  ResponseCTO.CTO[XCP_RES_BYTE5_INDEX] = XCP_MIN_ST;
  /* Replace the reserved value with XCP_CMD_PROCESSOR_QUEUE_SIZE when interleaved communication
   * is implemented */
  ResponseCTO.CTO[XCP_RES_BYTE6_INDEX] = XCP_RESERVED_BYTE;
  ResponseCTO.CTO[XCP_RES_BYTE7_INDEX] = XCP_DRIVER_VERSION;
  /* Set response length */
  ResponseCTO.Length = XCP_CMD_GET_COMM_INFO_RES_LENGTH;

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETCOMMMODEINFO_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------------[Xcp_ProcessCmdGetStatus]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetStatus(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDGETSTATUS_ENTRY();

  /* Prepare CTO with positive response */
  ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
  ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_GET_SESSION_STATUS();
  ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = XCP_GET_RESOURCE_PROTECTION_STATUS();
  ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = XCP_RESERVED_BYTE;
  Xcp_WriteWordToByteArray(&(ResponseCTO.CTO[XCP_RES_BYTE4_INDEX]),
                           XCP_GET_SESSION_CONFIGID());
  ResponseCTO.Length = XCP_CMD_GETSTATUS_RES_LENGTH;

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETSTATUS_EXIT(XCP_E_OK);
  return XCP_E_OK;
}
#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) || \
     (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))
/*----------------------[Xcp_ProcessCmdStartStopDAQList]---------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdStartStopDAQList(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDSTARTSTOPDAQLIST_ENTRY();
  /* !LINKSTO Xcp.ASAM.PROTECTION.INVALID.RESOURCEACCESS,1 */
  /* Verify protection status for both DAQ and STIM*/
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* BOTH Daq and STIM is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Retrieve requested DAQ List number */
    const uint16 DaqListNumber =
        XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));

    /* If DAQ List number is invalid */
    if (DaqListNumber >= XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(uint16))
    {
      /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
    else /* DAQ List number is valid */
    {
      /* Local variable to hold address of the requested DAQ List's parameters */
      P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqListNumber);
       /* To start/stop a DAQ list, it should not be protected STIM or DAQ or BOTH */
        /* Verify DAQ protection status */
      if((XCP_MASK_DAQLIST_DIRECTION_DAQ ==
          (DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION)) &&
          ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
           (uint8)XCP_RESOURCE_DAQ_MASK))
      {
        /* !LINKSTO Xcp.ASAM.STARTSTOPDAQLIST.AccessLocked.DAQ,1 */
        /* Either Daq or STIM is protected return ERR_ACCESS_LOCKED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
      }
      /* Verify STIM protection status */
      else if ((XCP_MASK_DAQLIST_DIRECTION_STIM ==
                (DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION)) &&
               ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
                (uint8)XCP_RESOURCE_STIM_MASK))
      {
        /* !LINKSTO Xcp.ASAM.STARTSTOPDAQLIST.AccessLocked.STIM,1 */
        /* Either Daq or STIM is protected return ERR_ACCESS_LOCKED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
      }
      /* To check if direction is not set or event is not configured */
      else if (XCP_IS_DAQLIST_FULLY_CONFIGURED(DaqListPtr) == FALSE)
      {
        /* Prepare error CTO with negative response ERR_DAQ_CONFIG when DAQ Direction is not clear
         * and event not configured */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_DAQ_CONFIG, XCP_ERR_DAQ_CONFIG_LENGTH);
      }
      else
      {
        /* Retrieve requested mode */
        uint8 DaqMode = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);

        /* Enter critical section */
        SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

        /* Handle each requested mode */
        switch(DaqMode)
        {
          /* Mode = 00 = Stop */
          case XCP_SSDL_MODE_STOP:
              /* Check whether DAQ List is running */
              if(XCP_IS_DAQ_LIST_RUNNING(DaqListPtr) == TRUE)
              {
                /*Stop the DAQ List*/
                XCP_SET_DAQLIST_STOPPED(DaqListPtr);
                /* Update the Xcp total number of DAQ Lists running */
                XCP_GET_NO_OF_DAQLISTS_RUNNING()--;
                /* If all DAQ lists got stopped */
                if (XCP_GET_NO_OF_DAQLISTS_RUNNING() == 0U)
                {
                  /* Clear Xcp DAQ running status */
                  XCP_CLEAR_XCP_STATUS_DAQ_RUNNING();
                }
#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
                /* Update number of DAQ Lists running for the associated event */
                Xcp_Event[DaqListPtr->EventId].NumberOfRunningDaqLists--;
#endif
              }

              /* Remove stopped DTOs from DTO Queue */
              Xcp_ClearDaqListDtos((Xcp_DaqIdType)DaqListNumber);

              /* Prepare response CTO */
              ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
              /* If DAQ Packet Identification is of type "Absolute PID" */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)
              ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_GET_FIRST_PID(DaqListPtr);
#else
              /* The value is not significant when Packet Identification is of
               * type is not "Absolute PID" */
              ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_RESERVED_BYTE;
#endif
              ResponseCTO.Length =XCP_CMD_SSDL_RES_LENGTH;
          break;
          /* Mode = 01 = Start */
          case XCP_SSDL_MODE_START:
              /* Check whether DAQ List is already running */
              if (XCP_IS_DAQ_LIST_RUNNING(DaqListPtr) == FALSE)
              {
#if(XCP_OVERLOAD_MSB_SUPPORT == STD_ON)
                /* set action to skip the check for event channel overload detection,
                 * for one event channel processing cycle */
                DaqListPtr->Flags |= (uint8)XCP_MASK_DAQLIST_SKIPOVCHECK;
                DaqListPtr->Flags &= (uint8)(~XCP_MASK_DAQLIST_OVCHECKSKIPPED);
#endif /* (XCP_OVERLOAD_MSB_SUPPORT == STD_ON) */
                /* Set DAQ list as running */
                XCP_SET_DAQLIST_RUNNING(DaqListPtr);
                /* Update Xcp total number of DAQ Lists running */
                XCP_GET_NO_OF_DAQLISTS_RUNNING()++;
#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
                /* Update number of DAQ Lists running for the associated event */
                Xcp_Event[DaqListPtr->EventId].NumberOfRunningDaqLists++;
#endif
              }
              /* Set XCP slave status as DAQ running */
              XCP_SET_XCP_STATUS_DAQ_RUNNING();
              /* Prepare response CTO */
              ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
              /* If DAQ Packet Identification is of type "Absolute PID" */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)
              ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_GET_FIRST_PID(DaqListPtr);
#else
              /* The value is not significant when Packet Identification is of
               * type is not "Absolute PID" */
              ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_RESERVED_BYTE;
#endif
              ResponseCTO.Length = XCP_CMD_SSDL_RES_LENGTH;

              /* If this is a cyclic event
                 set the cyclic event time cycle to the initial value 1U.
                 This value allows the event to trigger for the first time when
                 the Xcp_MainFunction is called */
              XCP_CYCLIC_EVENTS_CNT_INI( );

          break;
          /* Mode = 02 = Select */
          case XCP_SSDL_MODE_SELECT:
              /* Set the DAQ List as selected */
              XCP_SET_DAQLIST_SELECTED(DaqListPtr);
              /* Update number of selected DAQ Lists */
              XCP_GET_NO_OF_DAQLISTS_SELECTED()++;
              /* Prepare response CTO */
              ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
              /* If DAQ Packet Identification is of type "Absolute PID" */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)
              ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_GET_FIRST_PID(DaqListPtr);
#else
              /* The value is not significant when Packet Identification is of
               * type is not "Absolute PID" */
              ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_RESERVED_BYTE;
#endif
              ResponseCTO.Length =XCP_CMD_SSDL_RES_LENGTH;
          break;
          /* Undefined mode parameter */
          default:
            /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
            XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
          break;
        }

        /* Exit critical section */
        SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      }
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDSTARTSTOPDAQLIST_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*----------------------[Xcp_ProcessCmdStartStopSynch]---------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdStartStopSynch(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDSTARTSTOPSYNCH_ENTRY();
  /* !LINKSTO Xcp.ASAM.PROTECTION.INVALID.RESOURCEACCESS,1 */
  /* If both DAQ and STIM resources are protected */
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* Both Daq and STIM is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Process the START STOP Synch command request */
    Xcp_ProcessStartStopSynch(&ResponseCTO);
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDSTARTSTOPSYNCH_EXIT(XCP_E_OK);
  return XCP_E_OK;
}
/*----------------------[Xcp_ProcessStartStopSynch]---------------------*/
STATIC FUNC(void, XCP_CODE) Xcp_ProcessStartStopSynch
(
  P2VAR(Xcp_CTOType,AUTOMATIC,AUTOMATIC) ResponseCTOPtr
)
{
  /* Local variable to hold DAQ List number */
  Xcp_DaqIdType DaqListNumber;
  /* Local variable to hold address of the requested DAQ List's parameters */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr;

  DBG_XCP_PROCESSSTARTSTOPSYNCH_ENTRY(ResponseCTOPtr);

  /* Handle each requested mode */
  switch (XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX))
  {
    /* Mode = 00 = Stop All */
    case XCP_SS_SYNCH_MODE_STOP_ALL:
      /* Loop through all DAQ Lists until the last DAQ List */
      for (DaqListNumber = 0U; DaqListNumber < XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(Xcp_DaqIdType);
           ++DaqListNumber)
      {
        /* Retrieve DAQ List parameter */
        DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqListNumber);

        /* Check whether DAQ List is already running */
        if (XCP_IS_DAQ_LIST_RUNNING(DaqListPtr) == TRUE)
        {
          /* Stop the DAQ List */
          XCP_SET_DAQLIST_STOPPED(DaqListPtr);
          /* Update the number of DAQ Lists running */
          XCP_GET_NO_OF_DAQLISTS_RUNNING()--;
#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
          /* Update number of DAQ Lists running for the associated event */
          Xcp_Event[DaqListPtr->EventId].NumberOfRunningDaqLists--;
#endif
        }
      }
      /* Clear Xcp DAQ running status */
      XCP_CLEAR_FLAG_U8_ATOMIC(XCP_BIT_DAQ_RUNNING, Xcp_Status.SessionStatus);

      /* enter critical section */
      SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

      /* Flush DTO Queue to avoid transmission of any pending DTOs */
      Xcp_FlushDTORunTimeData();

      /* enter critical section */
      SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

      /* Prepare CTO with positive response */
      ResponseCTOPtr->CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
      ResponseCTOPtr->Length = XCP_CMD_SS_SYNCH_RES_LENGTH;
    break;
    /* Mode = 01 = Start Selected */
    case XCP_SS_SYNCH_MODE_START_SELECTED:
      /* Loop through all DAQ Lists until the last DAQ List or until all
       * selected DAQ Lists are covered */
      for (DaqListNumber = 0U;
         ((DaqListNumber < XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(Xcp_DaqIdType)) &&
          (0U != XCP_GET_NO_OF_DAQLISTS_SELECTED()));
         ++DaqListNumber)
      {
        /* Retrieve DAQ List parameter */
        DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqListNumber);
        /* If the DAQ List is selected */
        if (XCP_IS_DAQ_LIST_SELECTED(DaqListPtr) == TRUE)
        {
          /* Check whether DAQ List is already running */
          if (FALSE == XCP_IS_DAQ_LIST_RUNNING(DaqListPtr))
          {
            /* Set DAQ list as running */
            XCP_SET_DAQLIST_RUNNING(DaqListPtr);
            /* Update number of DAQ Lists running */
            XCP_GET_NO_OF_DAQLISTS_RUNNING()++;
#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
            /* Update number of DAQ Lists running for the associated event */
            Xcp_Event[DaqListPtr->EventId].NumberOfRunningDaqLists++;
#endif
          }

          /* If this is a cyclic event
             set the cyclic event time cycle to the initial value 1U.
             This value allows the event to trigger for the first time when
             the Xcp_MainFunction is called */
          XCP_CYCLIC_EVENTS_CNT_INI( );

          /* Update number of DAQ Lists selected as the selected DAQ list is
           * running */
          XCP_GET_NO_OF_DAQLISTS_SELECTED()--;
          /* Clear selected status of the DAQ List */
          XCP_CLEAR_DAQLIST_SELECTED(DaqListPtr);
        }
      }
      /* If any of the DAQ list is running */
      if (0U != XCP_GET_NO_OF_DAQLISTS_RUNNING())
      {
        /* Set XCP slave status as DAQ running */
        XCP_SET_FLAG_U8_ATOMIC(XCP_BIT_DAQ_RUNNING, Xcp_Status.SessionStatus);
      }
      /* Prepare CTO with positive response */
      ResponseCTOPtr->CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
      ResponseCTOPtr->Length = XCP_CMD_SS_SYNCH_RES_LENGTH;
    break;
    /* Mode = 02 = Stop Selected*/
    case XCP_SS_SYNCH_MODE_STOP_SELECTED:
      /* Loop through all DAQ Lists until the last DAQ List or until all
       * selected DAQ Lists are covered */
      for (DaqListNumber = 0U;
         ((DaqListNumber < XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(Xcp_DaqIdType))
          && (0U != XCP_GET_NO_OF_DAQLISTS_SELECTED()));
         ++DaqListNumber)
      {
        /* Retrieve DAQ List parameter */
        DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqListNumber);
        /* If the DAQ List is selected */
        if (XCP_IS_DAQ_LIST_SELECTED(DaqListPtr) == TRUE)
        {
          /* Check whether DAQ List is already running */
          if (XCP_IS_DAQ_LIST_RUNNING(DaqListPtr) == TRUE)
          {
            /* Stop the DAQ List */
            XCP_SET_DAQLIST_STOPPED(DaqListPtr);
            /* Update the number of DAQ Lists running */
            XCP_GET_NO_OF_DAQLISTS_RUNNING()--;
#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
            /* Update number of DAQ Lists running for the associated event */
            Xcp_Event[DaqListPtr->EventId].NumberOfRunningDaqLists--;
#endif
          }
          /* Update number of DAQ Lists selected as the selected DAQ list is stopped */
          XCP_GET_NO_OF_DAQLISTS_SELECTED()--;
          /* Clear selected status of the DAQ List */
          XCP_CLEAR_DAQLIST_SELECTED(DaqListPtr);
        }
      }
      /* If all DAQ lists got stopped */
      if (XCP_GET_NO_OF_DAQLISTS_RUNNING() == 0U)
      {
        /* Clear Xcp DAQ running status */
        XCP_CLEAR_FLAG_U8_ATOMIC(XCP_BIT_DAQ_RUNNING, Xcp_Status.SessionStatus);
      }

      /* Remove stopped DTOs from DTO Queue */
      Xcp_ClearStoppedDtos();

      /* Prepare CTO with positive response */
      ResponseCTOPtr->CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
      ResponseCTOPtr->Length = XCP_CMD_SS_SYNCH_RES_LENGTH;
    break;
    /* Undefined mode parameter */
    default:
      /* Prepare error CTO with negative response ERR_MODE_NOT_VALID */
      XCP_ERROR_RESPONSE((*ResponseCTOPtr), XCP_ERR_MODE_NOT_VALID, XCP_ERR_MODE_NOT_VALID_LENGTH);
    break;
  }

  DBG_XCP_PROCESSSTARTSTOPSYNCH_EXIT(ResponseCTOPtr);
}

/*-------------------[Xcp_ProcessCmdGetDAQProcessorInfo]---------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetDAQProcessorInfo(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  uint8 Properties = XCP_DAQ_PROPERTIES;

  DBG_XCP_PROCESSCMDGETDAQPROCESSORINFO_ENTRY();
  /* !LINKSTO Xcp.ASAM.PROTECTION.INVALID.RESOURCEACCESS,1 */
  /* If both DAQ and STIM resources are protected */
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* Daq is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Prepare CTO with positive response */
    ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
#if (XCP_TIMESTAMP_SUPPORTED == XCP_TIMESTAMP_MASK)
    if (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Timestamp_Enabled == TRUE)
    {
      Properties |= XCP_TIMESTAMP_MASK;
    }
    else
    {
      Properties &= ~XCP_TIMESTAMP_MASK;
    }
#endif
    ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = Properties;
    Xcp_WriteWordToByteArray(
      &(ResponseCTO.CTO[XCP_RES_BYTE2_INDEX]),
      XCP_MAX_DAQ);
    Xcp_WriteWordToByteArray(
      &(ResponseCTO.CTO[XCP_RES_BYTE4_INDEX]),
      XCP_MAX_EVENT_CHANNEL);
    ResponseCTO.CTO[XCP_RES_BYTE6_INDEX] = XCP_MIN_DAQ;
    ResponseCTO.CTO[XCP_RES_BYTE7_INDEX] =  XCP_DAQ_KEY_BYTE;
    ResponseCTO.Length = XCP_CMD_GET_DAQPI_RES_LENGTH;
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETDAQPROCESSORINFO_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*----------------------[Xcp_ProcessCmdGetDAQListMode]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetDAQListMode(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDGETDAQLISTMODE_ENTRY();
  /* !LINKSTO Xcp.ASAM.PROTECTION.INVALID.RESOURCEACCESS,1 */
  /* Verify protection status for both DAQ and STIM*/
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* BOTH Daq and STIM is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Retrieve requested DAQ List number */
    const uint16 DaqListNumber =
        XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));

    /* If DAQ List number is invalid (not yet dynamically allocated) */
    if ( DaqListNumber >= XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(uint16) )
    {
      /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }

    else /* DAQ List number is valid */
    {
      /* Local variable to hold address of the requested DAQ List's parameters */
      P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqListNumber);
      /* If the DAQ list was not fully configured via a prior SET_DAQ_LIST_MODE command */
      if ( XCP_IS_DAQLIST_FULLY_CONFIGURED(DaqListPtr) == FALSE )
      {
        /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
      }
      /* Verify DAQ protection status */
      else if ((XCP_MASK_DAQLIST_DIRECTION_DAQ ==
                (DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION)) &&
               ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
                (uint8)XCP_RESOURCE_DAQ_MASK))
      {
        /* !LINKSTO Xcp.ASAM.GETDAQLISTMODE.AccessLocked.DAQ,1 */
        /* Daq  is protected return ERR_ACCESS_LOCKED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
      }
      else if((XCP_MASK_DAQLIST_DIRECTION_STIM ==
               (DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION)) &&
              ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
               (uint8)XCP_RESOURCE_STIM_MASK))
      {
        /* !LINKSTO Xcp.ASAM.GETDAQLISTMODE.AccessLocked.STIM,1 */
        /* STIM is protected return ERR_ACCESS_LOCKED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
      }
      else
      {
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_GET_DAQLIST_MODE(DaqListPtr);
        /* "Selected" flag is conflicting with the "alternating" flag as the "selected" is required
         * to be the bit 0 according to GET_DAQ_LIST_MODE command, while the "alternating" flag is
         * required to be also the bit 0 according to the SET_DAQ_LIST_MODE command. Therefore,
         * we put the "selected" flag to bit 2 position */
        if ((DaqListPtr->Mode & XCP_MASK_DAQLIST_SELECTED) != 0U)
        {
          /* Set the selected bit at the GET_DAQ_LIST_MODE required position*/
          ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] |= XCP_MASK_DAQLIST_SELECTED_GDLM;
          /* Clear the selected bit which was set when the entire mode was copied */
          ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] &= (uint8)(~XCP_MASK_DAQLIST_SELECTED);
        }
        else
        {
          /* Clear the selected bit from the GET_DAQ_LIST_MODE required position */
          ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] &= (uint8)(~XCP_MASK_DAQLIST_SELECTED_GDLM);
        }
        ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = XCP_RESERVED_BYTE;
        ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = XCP_RESERVED_BYTE;
        Xcp_WriteWordToByteArray(
          &(ResponseCTO.CTO[XCP_RES_BYTE4_INDEX]),
          XCP_GET_DAQLIST_EVENT_CHANNEL(DaqListPtr));
        ResponseCTO.CTO[XCP_RES_BYTE6_INDEX] = XCP_GET_DAQLIST_PRESCALER(DaqListPtr);
        ResponseCTO.CTO[XCP_RES_BYTE7_INDEX] = XCP_GET_DAQLIST_PRIORITY(DaqListPtr);
        ResponseCTO.Length = XCP_CMD_GET_DAQLM_RES_LENGTH;
      }
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETDAQLISTMODE_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*-------------------[Xcp_ProcessCmdGetDAQResolutionInfo]--------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetDAQResolutionInfo(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDGETDAQRESOLUTIONINFO_ENTRY();
  /* !LINKSTO Xcp.ASAM.PROTECTION.INVALID.RESOURCEACCESS,1 */
  /* If both DAQ and STIM resources are protected */
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* Both Daq and STIM is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Prepare CTO with positive response */
    ResponseCTO.CTO[XCP_RES_PID_INDEX]   = XCP_RES_PID;
    ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_GRANULARITY_ODT_ENTRY_SIZE_DAQ;
#if (XCP_DAQ_SUPPORTED == STD_ON)
    ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_OdtEntry_Size_Daq;
#else
    ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = 0U;
#endif
    ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = XCP_GRANULARITY_ODT_ENTRY_SIZE_STIM;
#if (XCP_STIM_SUPPORTED == STD_ON)
    ResponseCTO.CTO[XCP_RES_BYTE4_INDEX] = Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_OdtEntry_Size_Stim;
#else
    ResponseCTO.CTO[XCP_RES_BYTE4_INDEX] = 0U;
#endif
    ResponseCTO.CTO[XCP_RES_BYTE5_INDEX] = XCP_TIMESTAMP_MODE;
    Xcp_WriteWordToByteArray(
      &(ResponseCTO.CTO[XCP_RES_BYTE6_INDEX]),
      XCP_TIMESTAMP_TICKS);
    ResponseCTO.Length = XCP_CMD_GET_DAQRI_RES_LENGTH;
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETDAQRESOLUTIONINFO_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdGetDAQEventInfo]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetDAQEventInfo(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDGETDAQEVENTINFO_ENTRY();
  /* !LINKSTO Xcp.ASAM.PROTECTION.INVALID.RESOURCEACCESS,1 */
  /* If both DAQ and STIM resources are protected */
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* Both Daq and STIM is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Retrieve requested Event Channel number */
    const uint16 EventChannelNumber =
        XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));

    /* Check whether event channel is valid */
    if (XCP_MAX_EVENT_CHANNEL <= EventChannelNumber)
    {
      /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
    else
    {
      /* Local variable to hold address of event configuration information */
      const Xcp_EventInfoType *EventCfgPtr = &Xcp_EventInfo[EventChannelNumber];
      /* Prepare CTO with positive response */
      ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
      ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_GET_EVENT_PROPERTIES(EventCfgPtr);
      ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = XCP_GET_EVENT_MAX_DAQ(EventCfgPtr);
      ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = EventCfgPtr->EventNameLength;
      ResponseCTO.CTO[XCP_RES_BYTE4_INDEX] = EventCfgPtr->TimeCycle;
      ResponseCTO.CTO[XCP_RES_BYTE5_INDEX] = EventCfgPtr->TimeUnit;
      ResponseCTO.CTO[XCP_RES_BYTE6_INDEX] = XCP_GET_EVENT_PRIORITY(EventCfgPtr);
      ResponseCTO.Length = XCP_CMD_GET_DAQEI_RES_LENGTH;

      /* Set MTA */
      Xcp_MTA.Type         = XCP_MTATYPE_ADDRESS;
      /* Set the address location from which the name to be uploaded */
      Xcp_MTA.Address.Read = EventCfgPtr->EventNamePtr;
      /* Mark the MTA as being set */
      Xcp_MTA.IsSet        = TRUE;
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETDAQEVENTINFO_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*----------------------[Xcp_ProcessCmdGetDAQListInfo]-----------------------*/
/* This command is available only for static DAQ lists */
#if (XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC_MASK)

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetDAQListInfo(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDGETDAQLISTINFO_ENTRY();
  /* !LINKSTO Xcp.ASAM.PROTECTION.INVALID.RESOURCEACCESS,1 */
  /* Verify protection status for both DAQ and STIM*/
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* BOTH Daq and STIM is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Retrieve requested DAQ List number */
    const uint16 DaqListNumber =
        XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));

    /* If DAQ List number is invalid */
    if (DaqListNumber >= XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(uint16))
    {
      /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
    else /* DAQ List number is valid */
    {
      /* Local variable to hold address of the requested DAQ List's parameters */
      P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqListNumber);

      /* !LINKSTO Xcp.ASAM.GETDAQLISTINFO.AccessLocked.DAQ,1 */
      /* Verify DAQ protection status */
      if ((XCP_MASK_DAQLIST_DIRECTION_DAQ ==
            (DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION)) &&
           ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
               (uint8)XCP_RESOURCE_DAQ_MASK))

      {
        /* Either Daq or STIM is protected return ERR_ACCESS_LOCKED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
      }
      /* !LINKSTO Xcp.ASAM.GETDAQLISTINFO.AccessLocked.STIM,1 */
      /* Verify STIM protection status */
      else if ((XCP_MASK_DAQLIST_DIRECTION_STIM ==
        (DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION)) &&
       ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
           (uint8)XCP_RESOURCE_STIM_MASK))
      {
      /* Either Daq or STIM is protected return ERR_ACCESS_LOCKED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
      }
      else
      {
        /* Local variable to hold address of the requested DAQ List */
        const P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR) DAQListConfigPtr = &Xcp_DaqListsDefault.Xcp_Daq[DaqListNumber];

        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_GET_DAQLIST_PROPERTIES(DAQListConfigPtr);
        ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = XCP_GET_DAQLIST_MAX_ODT(DAQListConfigPtr);
        ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = XCP_GET_DAQLIST_MAX_ODT_ENTRIES(DAQListConfigPtr);
        Xcp_WriteWordToByteArray(
            &(ResponseCTO.CTO[XCP_RES_BYTE4_INDEX]),
          XCP_GET_DAQLIST_FIXED_EVENT_CHANNEL(DAQListConfigPtr));
        ResponseCTO.Length = XCP_CMD_GET_DAQLI_RES_LENGTH;
      }
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETDAQLISTINFO_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdClearDAQList]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdClearDAQList(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  /* Local variable to hold Event Channel number */
  uint16 EventChannel;
  /* Local variable to hold Daq Id Index */
  uint16 Daqindex;

  DBG_XCP_PROCESSCMDCLEARDAQLIST_ENTRY();

  /* !LINKSTO Xcp.ASAM.PROTECTION.INVALID.RESOURCEACCESS,1 */
  /* Verify protection status for both DAQ and STIM*/
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* BOTH Daq and STIM is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Retrieve requested DAQ List number. */
    const uint16 DaqListNumber =
        XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));

    /* If DAQ List number is invalid */
    if (DaqListNumber >= XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(uint16))
    {
      /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
    else /* DAQ List number valid */
    {
      /* Local variable to hold address of the requested DAQ List's parameters */
      P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqListNumber);
      /* !LINKSTO Xcp.ASAM.CLEARDAQLIST.AccessLocked.DAQ,1 */
      /* Verify DAQ protection status */
      if ((XCP_MASK_DAQLIST_DIRECTION_DAQ ==
              (DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION)) &&
             ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
                 (uint8)XCP_RESOURCE_DAQ_MASK))
      {
        /* Either Daq or STIM is protected return ERR_ACCESS_LOCKED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
      }
      /* !LINKSTO Xcp.ASAM.CLEARDAQLIST.AccessLocked.STIM,1 */
      /* Verify STIM protection status */
      else if ((XCP_MASK_DAQLIST_DIRECTION_STIM ==
        (DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION)) &&
       ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
           (uint8)XCP_RESOURCE_STIM_MASK))
      {
        /* Either Daq or STIM is protected return ERR_ACCESS_LOCKED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
      }
      else
      {
#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0)
        /* !LINKSTO Xcp.CLEAR_DAQ_LIST.BusyWhenEventMFIsProcessing,1 */
        /* Check that the event associated with this DAQ list is not currently running */
        if  (
             (DaqListPtr->EventId != XCP_INVALID_EVENT_CHANNEL) &&
             (
               XCP_IS_BYTE_FLAG_SET(Xcp_EventInfo[DaqListPtr->EventId].IndexRunningFlag,
               Xcp_IsMFEventCurrentlyProcessing) == TRUE
             ) &&
             (Xcp_Event[DaqListPtr->EventId].NumberOfRunningDaqLists  > 0U)
            )
        {
          /* There is currently an ongoing Event Main Function processing this DAQ list,
          command must be retried in order to not mess the ongoing event processing */
          XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_BUSY, XCP_ERR_CMD_BUSY_LENGTH);
        }
        else
#endif /* (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0) */
        {
          /* in case there are static Daq list available */
#if (XCP_MIN_DAQ < XCP_NO_OF_NON_DYNAMIC_DAQ_LISTS)
#if (XCP_MIN_DAQ != 0)
          /* Check whether the DAQ List is predefined and configurable */
          if (XCP_MIN_DAQ <= DaqListNumber)
#endif /* (XCP_MIN_DAQ != 0) */
          {
            /* Clear all ODT Entries associated with every ODT in the DAQ List */
            Xcp_ClearODTs(DaqListPtr->OdtList, DaqListPtr->MaxOdt);
          }
#endif /* (XCP_MIN_DAQ < XCP_NO_OF_NON_DYNAMIC_DAQ_LISTS) */

          /* Reset DAQ List states and running Data.
             The cast is OK as, if Xcp_DaqIdType is an 8 bit type and the DAQ ID is
             a 16 bit value, the previous check ( DaqListNumber < XCP_MAX_DAQ )
             ensured that the DAQ list ID is no bigger than 255 at this point. */
          Xcp_ResetDaqListData( (Xcp_DaqIdType)DaqListNumber );

          /* If any event is associated with the DAQList, this needs to be
           * removed */
          for (EventChannel = 0U; EventChannel < XCP_MAX_EVENT_CHANNEL; ++EventChannel)
          {
            for (Daqindex = 0U; Daqindex < Xcp_Event[EventChannel].DaqIdListCount; ++Daqindex)
            {
              if (DaqListNumber == Xcp_Event[EventChannel].DaqIdListPtr[Daqindex])
              {
                SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
                /* Disassociate DAQ list from present event channel.
                   The cast is OK as, if Xcp_DaqIdType is an 8 bit type and the DAQ ID is
                   a 16 bit value, the previous check ( DaqListNumber < XCP_MAX_DAQ )
                   ensured that the DAQ list ID is no bigger than 255 at this point. */
                Xcp_RemoveDAQListFromEventChannel( (Xcp_DaqIdType)DaqListNumber, EventChannel );
                SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
              }
            }
          }

#if (XCP_MIN_DAQ != 0U)
          /* Initialize Preconfigured DaqList with configured values */
          if (DaqListNumber < XCP_MIN_DAQ)
          {
            /* For PREDEFINED DAQ lists, reset the associated events */
            for (EventChannel = 0U; EventChannel < XCP_MAX_EVENT_CHANNEL; ++EventChannel)
            {
              for (Daqindex = 0U; Daqindex < Xcp_EventInfo[EventChannel].DaqIdListCount; ++Daqindex)
              {
                if (DaqListNumber == Xcp_EventInfo[EventChannel].DaqIdListPtr[Daqindex])
                {
                  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
                  /* Associate DAQ list to present event channel.
                     The DaqListNumber variable is casted to Xcp_DaqIdType type
                     a 16 bit value, the previous check ( DaqListNumber < XCP_MAX_DAQ )
                     ensured that the DAQ list ID is no bigger than 255 at this point. */
                  Xcp_AssociateDAQListToEventChannel( (Xcp_DaqIdType)DaqListNumber, EventChannel );
                  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
                }
              }
            }
          }
#endif
          /* Prepare CTO with positive response */
          ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
          ResponseCTO.Length = XCP_CMD_CLEAR_DAQ_RES_LENGTH;
        }
      }
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDCLEARDAQLIST_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

#endif /*(XCP_DAQ_CONFIG_TYPE == XCP_DAQ_STATIC_MASK)*/

/*----------------------[Xcp_ProcessCmdSetDAQListMode]----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdSetDAQListMode(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDSETDAQLISTMODE_ENTRY();
  /* !LINKSTO Xcp.ASAM.PROTECTION.INVALID.RESOURCEACCESS,1 */
  /* Verify protection status for both DAQ and STIM */
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* Both Daq and STIM is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
#if (XCP_PRESCALER_SUPPORTED == XCP_PRESCALER_MASK)
    /* Local variable to hold requested Prescaler */
    uint8 Prescaler;
#endif
    /* Local variable to hold requested Mode */
    uint8 Mode;

    /* Retrieve requested DAQ List number.
       This DaqListNumber is a variable holding the DAQ list number from the command
       sent by the Master.
       The DAQ list number validity is checked below. */
    const uint16 DaqListNumber =
        XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));

    /* Retrieve requested event channel number */
    const uint16 EventChannelNumber =
        XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX));

#if (XCP_PRESCALER_SUPPORTED == XCP_PRESCALER_MASK)
    /* Retrieve requested Prescaler */
    Prescaler = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE6_INDEX);
#endif
    /* Retrieve requested Mode */
    Mode = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);

    /* Verify command parameters, Verify whether DAQ List number, event
     * channel number and prescaler are valid */
#if (XCP_PRESCALER_SUPPORTED == XCP_PRESCALER_MASK)
     /* !LINKSTO Xcp.ASAM.SETDAQLISTMODE002,1 */
    if (((DaqListNumber >= XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(uint16)) ||
      /* !LINKSTO Xcp.SETDAQLISTMODE.InvalidEventChannel,1 */
         (EventChannelNumber >= XCP_MAX_EVENT_CHANNEL)) ||
       /*Xcp.SETDAQLISTMODE.Prescaler001,1 */
        ((XCP_MASK_DAQLIST_DIRECTION_DAQ == (Mode & XCP_MASK_DAQLIST_DIRECTION)) &&
         (XCP_PRESCALER_MIN > Prescaler)))
#else
    /* !LINKSTO Xcp.ASAM.SETDAQLISTMODE002,1 */
    if ((DaqListNumber >= XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(uint16)) ||
    /* !LINKSTO Xcp.SETDAQLISTMODE.InvalidEventChannel,1 */
        (EventChannelNumber >= XCP_MAX_EVENT_CHANNEL))
#endif /*(XCP_PRESCALER_SUPPORTED == XCP_PRESCALER_MASK) */
    {
      /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
    /* !LINKSTO Xcp.SETDAQLISTMODE.AccessLocked.DAQ,1 */
    /* Verify DAQ protection status */
    else if ((XCP_MASK_DAQLIST_DIRECTION_DAQ == (Mode & XCP_MASK_DAQLIST_DIRECTION)) &&
             ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
                 (uint8)XCP_RESOURCE_DAQ_MASK))
    {
      /* Either Daq or STIM is protected return ERR_ACCESS_LOCKED */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
    }
    /* !LINKSTO Xcp.SETDAQLISTMODE.AccessLocked.STIM,1 */
    else if((XCP_MASK_DAQLIST_DIRECTION_STIM == (Mode & XCP_MASK_DAQLIST_DIRECTION)) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
         (uint8)XCP_RESOURCE_STIM_MASK))
    {
      /* Either Daq or STIM is protected return ERR_ACCESS_LOCKED */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
    }
    /* If alternating mode and timestamped mode are requested together or if
     * timestamped mode is requested and the requested event has ODT
     * consistency or if alternating mode is requested along with direction
     * STIM */
    else if ((((XCP_MASK_DAQLIST_ALTERNATING_MODE ==
               (Mode & XCP_MASK_DAQLIST_ALTERNATING_MODE)) ||
              (XCP_EVENT_CHANNEL_CONSISTENCY_ODT ==
               XCP_GET_EVENT_CHANNEL_CONSISTENCY(EventChannelNumber))) &&
             (XCP_MASK_DAQLIST_TIMESTAMPED_MODE ==
              (Mode & XCP_MASK_DAQLIST_TIMESTAMPED_MODE))))
    {
      /* Prepare error CTO with negative response ERR_MODE_NOT_VALID */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_MODE_NOT_VALID, XCP_ERR_MODE_NOT_VALID_LENGTH);
    }
    else if ((XCP_MASK_DAQLIST_ALTERNATING_MODE ==
              (Mode & XCP_MASK_DAQLIST_ALTERNATING_MODE)) &&
              (XCP_MASK_DAQLIST_DIRECTION_STIM ==
              (Mode & XCP_MASK_DAQLIST_DIRECTION)))
    {
      /* Prepare error CTO with negative response ERR_MODE_NOT_VALID */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_MODE_NOT_VALID, XCP_ERR_MODE_NOT_VALID_LENGTH);
    }
    /* If timestamp can't be switched off */
#if (XCP_TIMESTAMP_FIXED == XCP_TIMESTAMP_FIXED_MASK)
    /* If master requested to switched off timestamp*/
    else if ( 0U == (Mode & XCP_MASK_DAQLIST_TIMESTAMPED_MODE))
    {
      /* Prepare error CTO with negative response ERR_CMD_SYNTAX */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_SYNTAX, XCP_ERR_CMD_SYNTAX_LENGTH);
    }
#endif
    /* If PID is not absolute ODT number */
#if (XCP_IDENTIFICATION_FIELD_TYPE != XCP_ABSOLUTE_IF_MASK)
    /* If master requested to switch off PID */
    else if (XCP_MASK_DAQLIST_PID_OFF_MODE == (Mode & XCP_MASK_DAQLIST_PID_OFF_MODE))
    {
      /* Prepare error CTO with negative response ERR_CMD_SYNTAX */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_SYNTAX, XCP_ERR_CMD_SYNTAX_LENGTH);
    }
#endif
#if (XCP_RESOURCE_STIM != XCP_RESOURCE_STIM_MASK)
    /* If the requested direction is STIM but STIM direction is not supported */
    else if ((Mode & XCP_MASK_DAQLIST_DIRECTION) == XCP_MASK_DAQLIST_DIRECTION_STIM )
    {
      /* Prepare error CTO with negative response ERR_CMD_SYNTAX */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_SYNTAX, XCP_ERR_CMD_SYNTAX_LENGTH);
    }
#endif
    else
    {
      /* Process SET_DAQ_LIST_MODE command request */
      Xcp_SubSetDAQListMode(&ResponseCTO);
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDSETDAQLISTMODE_EXIT(XCP_E_OK);
  return XCP_E_OK;
}
/*----------------------[Xcp_SubSetDAQListMode]----------------------*/
STATIC FUNC(void, XCP_CODE) Xcp_SubSetDAQListMode
(
  P2VAR(Xcp_CTOType,AUTOMATIC,AUTOMATIC) ResponseCTOPtr
)
{
    /* Local variable to hold address of the requested DAQ List's parameters */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr;
  /* Retrieve requested DAQ List number */
  const uint16 DaqListNumber =
      XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));
  /* Retrieve requested Event channel number */
  const uint16 EventChannelNumber =
      XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX));

  /* Local Return value status */
  boolean LocalCheckSts = FALSE;
  /* Local variable to hold address of event configuration information */
  const Xcp_EventInfoType *EventCfgPtr = &Xcp_EventInfo[EventChannelNumber];
#if ((XCP_DAQ_COUNT > 0U) && (XCP_MAX_ODT > XCP_MAX_ODT_STIM))
  /* Local variable to hold requested Mode */
  uint8 Mode = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);
#endif

#if (XCP_MIN_DAQ == 0U)
  DBG_XCP_SUBSETDAQLISTMODE_ENTRY(ResponseCTOPtr);
#endif


  /* Get the required DAQ list */
  DaqListPtr = XCP_GET_DAQ_LIST_PTR((Xcp_DaqIdType)DaqListNumber);

  /* If requested event channel or associated event channel to the
   * requested DAQ List is in queue. */
  if (((TRUE == XCP_IS_BYTE_FLAG_SET(XCP_EVENT_IS_SET_FLAG_POS,
                             &(Xcp_Event[EventChannelNumber].EventStatusFlag))) ||
       ((XCP_INVALID_EVENT_CHANNEL != DaqListPtr->EventId) &&
        (TRUE == XCP_IS_BYTE_FLAG_SET(XCP_EVENT_IS_SET_FLAG_POS,
                 &(Xcp_Event[DaqListPtr->EventId].EventStatusFlag))))))
  {
    /* Prepare error CTO with negative response ERR_DAQ_ACTIVE */
    XCP_ERROR_RESPONSE((*ResponseCTOPtr), XCP_ERR_DAQ_ACTIVE, XCP_ERR_DAQ_ACTIVE_LENGTH);
  }
  /* If the event channel is not available or DAQ list is not already
   * associated with the event channel and the specified event channel
   * already has the maximum number of DAQ list associated with it */
  else if ((FALSE == XCP_IS_EVENT_CHANNEL_AVAILABLE(EventCfgPtr))||
           ((EventChannelNumber != XCP_GET_DAQLIST_EVENT_CHANNEL(DaqListPtr)) &&
            (XCP_GET_EVENT_MAX_DAQ(EventCfgPtr) == XCP_GET_EVENT_NO_OF_DAQLIST(EventChannelNumber))))
  {

    /* Prepare error CTO with negative response ERR_CMD_SYNTAX */
    XCP_ERROR_RESPONSE((*ResponseCTOPtr), XCP_ERR_CMD_SYNTAX, XCP_ERR_CMD_SYNTAX_LENGTH);
  }
  else
  {
     LocalCheckSts = Xcp_SubSetDaqMode_DaqProcess(EventCfgPtr,
                                                  ResponseCTOPtr);
  }
  /* Check that the intention is to configure a dynamically configurable DAQ list with the direction
   * STIM and if this is allowed according to the ODT PID limitation for the STIM case
   * (the PID shouldn't be higher than 0xBF)
   * We only perform the check if the configuration is DYNAMIC (as the check for STATIC is performed
   * at configuration time) and the DAQ list will then have an ODT with the PID greater than 0xBF*/
#if ((XCP_DAQ_COUNT > 0U) && (XCP_MAX_ODT > XCP_MAX_ODT_STIM))

  if (LocalCheckSts == TRUE)
  {
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)
    /* If direction is STIM and the number of configured ODTs for the current DAQ list plus the number
     * of allowed ODTs for previous DAQ lists is greater than the maximum allowed */
    if (((Mode & XCP_MASK_DAQLIST_DIRECTION) == XCP_MASK_DAQLIST_DIRECTION_STIM) &&
        ((DaqListPtr->FirstPID + DaqListPtr->MaxOdt) > XCP_MAX_ODT_STIM))
#else /* XCP_IDENTIFICATION_FIELD_TYPE != XCP_ABSOLUTE_IF_MASK */
    /* If direction is STIM and the number of configured ODTs for the current DAQ list is greater than
     * the maximum allowed */
    if (((Mode & XCP_MASK_DAQLIST_DIRECTION) == XCP_MASK_DAQLIST_DIRECTION_STIM) &&
        (DaqListPtr->MaxOdt > XCP_MAX_ODT_STIM))
#endif /* (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK) */
    {

      /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
      XCP_ERROR_RESPONSE((*ResponseCTOPtr), XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
      LocalCheckSts = FALSE;
    }
  }
#endif /*(XCP_DAQ_COUNT > 0U) && (XCP_MAX_ODT > XCP_MAX_ODT_STIM) */
  if (LocalCheckSts == TRUE)
  {
    /* Process the set DAQ list mode command request */
    Xcp_ProcessSetDAQListMode();
    /* Prepare CTO with positive response */
    ResponseCTOPtr->CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
    ResponseCTOPtr->Length = XCP_CMD_SET_DAQLM_RES_LENGTH;
  }
  DBG_XCP_SUBSETDAQLISTMODE_EXIT(ResponseCTOPtr);
}

STATIC FUNC( boolean, XCP_CODE) Xcp_SubSetDaqMode_DaqProcess
(
 P2CONST(Xcp_EventInfoType,AUTOMATIC, XCP_VAR) EventCfgPtr,
 P2VAR(Xcp_CTOType,AUTOMATIC,AUTOMATIC) ResponseCTOPtr
)
{
  /* Local Return value status */
  boolean LocalCheckSts = FALSE;
  /* Get DAQ list mode */
  uint8 Mode = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);
#if (XCP_MIN_DAQ != 0U)
  /* Retrieve requested DAQ List number */
  const uint16 DaqListNumber =
      XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));
  /* Retrieve requested Event channel number */
  const uint16 EventChannelNumber =
      XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX));

  P2CONST(Xcp_DaqType, AUTOMATIC, XCP_CONST) DaqListCfgPtr = NULL_PTR;
  DBG_XCP_SUBSETDAQLISTMODE_ENTRY(ResponseCTOPtr);
  if (DaqListNumber < XCP_MIN_DAQ)
  {
    /* Local variable to hold address of DAQ configuration information */
    DaqListCfgPtr= &(Xcp_DaqListsDefault.Xcp_Daq[DaqListNumber]);
  }
#endif
  DBG_XCP_SUBSETDAQMODE_DAQPROCESS_ENTRY();
#if (XCP_MIN_DAQ != 0U)

  if (DaqListNumber < XCP_MIN_DAQ)
  {
    /* If the requested DAQ List is of direction STIM */
    if (XCP_MASK_DAQLIST_DIRECTION_STIM == (Mode & XCP_MASK_DAQLIST_DIRECTION))
    {
      /* The DAQ is configured to support DAQ Lists with direction STIM and
       *  Event channel supports DAQ Lists with direction STIM */
      if ((TRUE == XCP_IS_EVENT_DIRECTION(EventCfgPtr,XCP_MASK_EVENT_DIRECTION_STIM)) &&
          (XCP_IS_DAQ_DIRECTION(DaqListNumber,XCP_MASK_DAQ_DIRECTION_STIM) == TRUE))
      {
        /* !LINKSTO Xcp.SETDAQLISTMODE.CompatibleDirection.STIM,1 */
        LocalCheckSts = TRUE;
      }
    }
    /* If the requested DAQ List is of direction DAQ */
    /* CHECK: NOPARSE */
    else if (XCP_MASK_DAQLIST_DIRECTION_DAQ == (Mode & XCP_MASK_DAQLIST_DIRECTION))
    /* CHECK: PARSE */
    {

      /* The DAQ is configured to support DAQ Lists with direction DAQ and
       * Event channel supports DAQ Lists with direction DAQ */
      /* !LINKSTO Xcp.SETDAQLISTMODE.CompatibleDirection.DAQ,1 */
      if ((TRUE == XCP_IS_EVENT_DIRECTION(EventCfgPtr,XCP_MASK_EVENT_DIRECTION_DAQ)) &&
          (XCP_IS_DAQ_DIRECTION(DaqListNumber,XCP_MASK_DAQ_DIRECTION_DAQ) == TRUE))
      {
        LocalCheckSts = TRUE;
      }
    }
    /* CHECK: NOPARSE */
    else
    {
      /* Daq List direction is checked before getting in this branch. If Daq list
         direction is different from DAQ direction, then a ERR_MODE_NOT_VALID
         will be sent as a negative response from Xcp slave. */
    }
    /* CHECK: PARSE */

    if (LocalCheckSts == FALSE)
    {
      /* Prepare error CTO with negative response ERR_CMD_SYNTAX */
      XCP_ERROR_RESPONSE((*ResponseCTOPtr), XCP_ERR_CMD_SYNTAX, XCP_ERR_CMD_SYNTAX_LENGTH);
    }
  }
  else
  {
    LocalCheckSts = TRUE;
  }
  if (LocalCheckSts == TRUE)
  {
    /* If event channel is fixed and requested event channel is not the
    * configured fixed event channel  */
    if((DaqListNumber < XCP_MIN_DAQ) &&
       ((XCP_IS_DAQLIST_EVENT_CHANNEL_FIXED(DaqListCfgPtr) == TRUE ) &&
         (XCP_GET_DAQLIST_FIXED_EVENT_CHANNEL(DaqListCfgPtr) != EventChannelNumber)))
    {
      /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
      XCP_ERROR_RESPONSE((*ResponseCTOPtr), XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
      LocalCheckSts = FALSE;
    }
  }
  #else /* (XCP_MIN_DAQ != 0U) */
  /* The DAQ is configured to support DAQ Lists with direction STIM and
  *  Event channel supports DAQ Lists with direction STIM */
  if((XCP_MASK_DAQLIST_DIRECTION_STIM == (Mode & XCP_MASK_DAQLIST_DIRECTION)) &&
   (TRUE== XCP_IS_EVENT_DIRECTION(EventCfgPtr,XCP_MASK_EVENT_DIRECTION_STIM)))
  {
    LocalCheckSts = TRUE;
  }
  /* The DAQ is configured to support DAQ Lists with direction DAQ and
  *  Event channel supports DAQ Lists with direction DAQ */
  else if ((XCP_MASK_DAQLIST_DIRECTION_DAQ == (Mode & XCP_MASK_DAQLIST_DIRECTION)) &&
         (TRUE == XCP_IS_EVENT_DIRECTION(EventCfgPtr,XCP_MASK_EVENT_DIRECTION_DAQ)))
  {
    LocalCheckSts = TRUE;
  }
  else
  {
    /* nothing to do */
  }
  if (LocalCheckSts == FALSE)
  {
    /* Prepare error CTO with negative response ERR_CMD_SYNTAX */
    XCP_ERROR_RESPONSE((*ResponseCTOPtr), XCP_ERR_CMD_SYNTAX, XCP_ERR_CMD_SYNTAX_LENGTH);
  }
#endif /* (XCP_MIN_DAQ != 0U) */

  DBG_XCP_SUBSETDAQMODE_DAQPROCESS_EXIT();
  return LocalCheckSts;
}
/*----------------------[Xcp_ProcessSetDAQListMode]----------------------*/
STATIC FUNC(void, XCP_CODE) Xcp_ProcessSetDAQListMode
(
  void
)
{
  /* Retrieve requested DAQ List number */
  const uint16 DaqListNumber =
      XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));
  /* Retrieve requested Event channel number */
  const uint16 EventChannelNumber =
      XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX));
  /* Local variable to hold address of the requested DAQ List's
   * parameters */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr;
  /* Local variable to hold requested Prescaler */
  uint8 Prescaler;
  /* Local variable to hold requested Mode */
  uint8 Mode;
  /* Local variable to hold requested Mode */
  uint8 Priority;

  DBG_XCP_PROCESSSETDAQLISTMODE_ENTRY();

  /* Get the requested DAQ list */
  DaqListPtr = XCP_GET_DAQ_LIST_PTR((Xcp_DaqIdType)DaqListNumber);
  /* Retrieve requested Prescaler */
  Prescaler = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE6_INDEX);
  /* Retrieve requested Mode */
  Mode = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);
  /* Retrieve requested Priority */
  Priority = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE7_INDEX);

  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  /* clear old dependencies to the selected daq list */
  if (XCP_INVALID_EVENT_CHANNEL != DaqListPtr->EventId)
  {
    /* Disassociate DAQ list from present event channel.
       The cast is OK as, if Xcp_DaqIdType is an 8 bit type and the DAQ ID is
       a 16 bit value, the previous check in Xcp_ProcessCmdSetDAQListMode() function
       ( DaqListNumber < XCP_MAX_DAQ ) ensured that the DAQ list ID is no bigger
       than 255 at this point. */
    Xcp_RemoveDAQListFromEventChannel( (Xcp_DaqIdType)DaqListNumber, DaqListPtr->EventId );

    if((DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION) != (Mode & XCP_MASK_DAQLIST_DIRECTION))
    {
      if((Mode & XCP_MASK_DAQLIST_DIRECTION) == XCP_MASK_DAQLIST_DIRECTION_STIM)
      {
        /* remove DTO information from daq list priority queue to prevent sending
         * wrong DTO packages */
        Xcp_ClearDaqListDtos((Xcp_DaqIdType)DaqListNumber);
      }
#if (XCP_OVERLOAD_MSB_SUPPORT == STD_ON)
      else /* we have DAQ direction */
      {
        /* set action to skip the check for event channel overload detection,
         * for one event channel processing cycle */
        DaqListPtr->Flags |= (uint8)XCP_MASK_DAQLIST_SKIPOVCHECK;
        DaqListPtr->Flags &= (uint8)(~XCP_MASK_DAQLIST_OVCHECKSKIPPED);
      }
#endif /* (XCP_OVERLOAD_MSB_SUPPORT == STD_ON) */
    }
  }
  /* Set PID_OFF mode */
  if (0U == ((uint8)(Mode & XCP_MASK_DAQLIST_PID_OFF_MODE)))
  {
    DaqListPtr->Mode &= (uint8)(~XCP_MASK_DAQLIST_PID_OFF_MODE);
  }
  else
  {
    DaqListPtr->Mode |= (uint8)(XCP_MASK_DAQLIST_PID_OFF_MODE);
  }
  /* Set Timestamp mode  */
  if (0U == ((uint8)(Mode & XCP_MASK_DAQLIST_TIMESTAMPED_MODE)))
  {
    DaqListPtr->Mode &= (uint8)(~XCP_MASK_DAQLIST_TIMESTAMPED_MODE);
  }
  else
  {
    DaqListPtr->Mode |= (uint8)(XCP_MASK_DAQLIST_TIMESTAMPED_MODE);
  }
  /* Set DAQ List direction  */
  if (0U == ((uint8)(Mode & XCP_MASK_DAQLIST_DIRECTION)))
  {
    DaqListPtr->Mode &= (uint8)(~XCP_MASK_DAQLIST_DIRECTION);
  }
  else
  {
    DaqListPtr->Mode |= (uint8)(XCP_MASK_DAQLIST_DIRECTION);
  }
  /* Set DAQ List alternating mode status  */
  if (0U == ((uint8)(Mode & XCP_MASK_DAQLIST_ALTERNATING_MODE)))
  {
    DaqListPtr->Mode &= (uint8)(~XCP_MASK_DAQLIST_ALTERNATING_MODE);
  }
  else
  {
    DaqListPtr->Mode |= (uint8)(XCP_MASK_DAQLIST_ALTERNATING_MODE);
  }
  /* Set DAQ List Prescaler */
  XCP_SET_DAQLIST_PRESCALER(DaqListPtr, Prescaler);
  /* Set DAQ List Prescaler count */
  XCP_SET_DAQLIST_PRESCALERCNT(DaqListPtr, Prescaler);
  /* Set DAQ List priority */
  XCP_SET_DAQLIST_PRIORITY(DaqListPtr, Priority);
  /* Associate DAQ List to the specified event channel */
  XCP_SET_DAQLIST_EVENT_CHANNEL(DaqListPtr, EventChannelNumber);
  /* Sort DAQ Lists associated with the event channel
     This cast is safe because for the configuration with Identification Field Type
     Absolute ODT Number there are maximum 255 DAQ Lists only. */
  Xcp_AssociateDAQListToEventChannel( (Xcp_DaqIdType)DaqListNumber, EventChannelNumber );
  /* Set DAQ List as configured */
  XCP_SET_DAQLIST_CONFIGURED(DaqListPtr);

  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_PROCESSSETDAQLISTMODE_EXIT();
}
#endif /*((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) ||
        * (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)) */

/*-------------------------[Xcp_ProcessCmdSetMTA]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdSetMTA(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

#if(XCP_SET_MTA_ENDIANNESS == STD_ON)
  /* Invert the bytes */
  const uint32 MTAAddress =
    ((uint32)(Xcp_CommandQueue.Queue[Xcp_CommandQueue.Head].CTO[4U])       ) |
    ((uint32)(Xcp_CommandQueue.Queue[Xcp_CommandQueue.Head].CTO[5U]) <<  8U) |
    ((uint32)(Xcp_CommandQueue.Queue[Xcp_CommandQueue.Head].CTO[6U]) << 16U) |
    ((uint32)(Xcp_CommandQueue.Queue[Xcp_CommandQueue.Head].CTO[7U]) << 24U);
#endif

  DBG_XCP_PROCESSCMDSETMTA_ENTRY();

  Xcp_MTA.Type   = XCP_MTATYPE_ADDRESS;

#if(XCP_SET_MTA_ENDIANNESS == STD_OFF)
  TS_MemCpy(&Xcp_MTA.Address.Write,
            &(Xcp_CommandQueue.Queue[Xcp_CommandQueue.Head].CTO[4U]),
            XCP_MTA_ADDRESS_LENGTH);
#else
  TS_MemCpy(&Xcp_MTA.Address.Write, &MTAAddress, XCP_MTA_ADDRESS_LENGTH);
#endif

  /* Prepare CTO with positive response */
  ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
  ResponseCTO.Length                 = XCP_CMD_SET_MTA_RES_LENGTH;
  /* Mark the MTA as being set */
  Xcp_MTA.IsSet = TRUE;
  /* Enter critical section */

  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDSETMTA_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*-------------------------[Xcp_ProcessCmdShortUpload]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdShortUpload(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  Xcp_ErrorType RetValue = XCP_E_OK;
  /* Callout function return value */
#if ((XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF) && (XCP_WRITE_READ_MEMORY_RAM == STD_ON))
  Xcp_ApplReturnType CalloutResForRead;
#endif
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
  Atomic_value_t localRAMCalloutProxyState;
#endif
  /* Local variable to hold requested number of data bytes */
  const uint8 NoOfDataBytes
    = (uint8)( XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX) *
               XCP_ADDRESS_GRANULARITY_SIZE );

  DBG_XCP_PROCESSCMDSHORTUPLOAD_ENTRY();

  ResponseCTO.Length = 0U;
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
  /* !LINKSTO Xcp.dsn.BSWDistribution.MemProxy.Atomics,1 */
  localRAMCalloutProxyState = Atomics_Load(&Xcp_RAMCalloutProxyState);
  /* !LINKSTO Xcp.BswDistribution.CalibrationCommands.UserDefinedCallouts.ProvideResponse,1 */
  if (localRAMCalloutProxyState == XCP_RESULT_ALREADY_PROCESSED)
  {
    TS_MemCpy(&(ResponseCTO.CTO[XCP_ADDRESS_GRANULARITY_SIZE]), &Xcp_MemoryProxyBuffer, NoOfDataBytes);
    /* Prepare CTO with positive response */
    ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
    ResponseCTO.Length = (uint8)(NoOfDataBytes + XCP_ADDRESS_GRANULARITY_SIZE);
    Xcp_MTA.Address.Write = &Xcp_MTA.Address.Write[NoOfDataBytes];
    RetValue = XCP_E_OK;
    Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_NO_USER_CALLOUT_REQUESTED);
    Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_AVAILABLE);
  }
  else if (localRAMCalloutProxyState == XCP_NO_USER_CALLOUT_REQUESTED)
#endif
  {
    /* Verify whether requested length is valid */
    if (NoOfDataBytes > (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto - XCP_ADDRESS_GRANULARITY_SIZE))
    {
      /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
    else
    {
#if (XCP_USE_MEMORY_PROXY == STD_ON)
      const Xcp_MemoryAreaType* MemoryArea;
      Xcp_MemoryProxyDataType Xcp_MemoryProxyData;
        uint8 MemAreaIdx;
#endif
      Xcp_MTA.Type   = XCP_MTATYPE_ADDRESS;
      TS_MemCpy(&Xcp_MTA.Address.Write,
                &(Xcp_CommandQueue.Queue[Xcp_CommandQueue.Head].CTO[4U]),
                XCP_MTA_ADDRESS_LENGTH);

      /* Mark the MTA as being set */
      Xcp_MTA.IsSet = TRUE;
#if (XCP_USE_MEMORY_PROXY == STD_ON)
      /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.ProtectedMemoryLocations,1 */
      MemoryArea = Xcp_FindMemoryArea(XCP_APPL_GET_ADDRESS(0, Xcp_MTA.Address.Write), NoOfDataBytes, XCP_MEMORY_AREA_TYPE_READ, XCP_MEMORY_AREA_SCOPE_CALIBRATION, &MemAreaIdx);
      if(MemoryArea == NULL_PTR)
      {
        /* !LINKSTO Xcp.BswDistribution.Calibration.SHORT_UPLOAD.NoCalibrationMemoryAccessArea.Error,1 */
        /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.ProtectedMemoryLocations.NegativeResponse,1 */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
      }
      else if ((uint8)GetApplicationID() != MemoryArea->ApplicationId)
      {
        /* !LINKSTO Xcp.BswDistribution.Calibration.UPLOAD.MemoryProxy.Usage,1 */
        /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyAvailable,1 */
        /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyLocked,1 */
        /* !LINKSTO Xcp.dsn.BSWDistribution.MemProxy.ProtectedAccess,1 */
        if (Atomics_Load(&Xcp_MemoryProxyState) == XCP_MEMORY_PROXY_AVAILABLE)
        {
          Std_ReturnType result;
          const uint8 ApplicationId = (uint8)GetApplicationID();
          Xcp_MemoryProxyData.type = XCP_MEMORYPROXYDATA_UPLOAD;
          Xcp_MemoryProxyData.size = NoOfDataBytes;
          Xcp_MemoryProxyData.address = XCP_APPL_GET_ADDRESS(0, Xcp_MTA.Address.Write);
          Xcp_MemoryProxyData.EventId = XCP_E_INVALID_EVENT;
          /* !LINKSTO Xcp.BswDistribution.Communication.UnitInteraction.CoreLocal,1 */
          /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyLocked,1 */
          Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_LOCKED);
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
          Xcp_MemoryProxyData.MemoryAreaIdx = MemAreaIdx;
          Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_REQUESTED_TO_MEMORY_PROXY);
#endif
          result = Xcp_SendMemoryProxyData[ApplicationId][MemoryArea->ApplicationId](&Xcp_MemoryProxyData);
          if ( result == SCHM_E_OK)
          {
            /* !LINKSTO Xcp.BswDistribution.Calibration.SHORT_UPLOAD.MemoryProxy.Busy,1 */
            RetValue = XCP_E_BUSY;
          }
          else
          {
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
            Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_NO_USER_CALLOUT_REQUESTED);
#endif
            /* intentionally no response */
            /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.SchMError,1 */
            XCP_DET_REPORT_RUNTIME_ERROR(XCP_SID_MAIN_FUNCTION, XCP_E_SCHM_ERROR);
          }
        }
        /* check the state again, it might be that the memory proxy task already executed */
        if (Atomics_Load(&Xcp_MemoryProxyState) == XCP_MEMORY_PROXY_READY)
        {
          TS_MemCpy(&(ResponseCTO.CTO[XCP_ADDRESS_GRANULARITY_SIZE]), &Xcp_MemoryProxyBuffer, NoOfDataBytes);
          /* Prepare CTO with positive response */
          /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyReady.PosResponse,1 */
          ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
          ResponseCTO.Length = (uint8)(NoOfDataBytes + XCP_ADDRESS_GRANULARITY_SIZE);
          Xcp_MTA.Address.Write = &Xcp_MTA.Address.Write[NoOfDataBytes];
          /* !LINKSTO Xcp.BswDistribution.Calibration.UPLOAD.MemoryProxy.Finished.State,1 */
          /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyReady.ResetState,1 */
          Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_AVAILABLE);
          RetValue = XCP_E_OK;
        }
        else
        {
          /* keep waiting for the memory proxy */
          RetValue = XCP_E_BUSY;
        }
      }
      else
#endif
      {
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
        Xcp_MemoryProxyData.type = XCP_MEMORYPROXYDATA_UPLOAD;
        Xcp_MemoryProxyData.size = NoOfDataBytes;
        Xcp_MemoryProxyData.address = XCP_APPL_GET_ADDRESS(0, Xcp_MTA.Address.Write);
        Xcp_MemoryProxyData.EventId = XCP_E_INVALID_EVENT;
        /* !LINKSTO Xcp.BswDistribution.Communication.UnitInteraction.CoreLocal,1 */
        Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_LOCKED);
        Xcp_MemoryProxyData.MemoryAreaIdx = MemAreaIdx;
        Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_REQUESTED_TO_MEMORY_PROXY);
        Xcp_MemoryProxyHandler(&Xcp_MemoryProxyData);
        /* keep waiting for the memory proxy */
        RetValue = XCP_E_BUSY;
#else
/* !LINKSTO Xcp.ProtectedMemoryLocations.General,1 */
#if ((XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON) && (XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF))
        if (Xcp_IsMemoryAreaAccessible
               (
                   XCP_APPL_GET_ADDRESS(0U, Xcp_MTA.Address.Write),
                   NoOfDataBytes,
                   XCP_MEMORY_AREA_TYPE_READ,
                   XCP_MEMORY_AREA_SCOPE_CALIBRATION
               ) == FALSE
           )
        {
          /* !LINKSTO Xcp.ProtectedMemoryLocations.NegativeResponse.Commands,1 */
          /* Prepare error CTO with negative response ERR_ACCESS_DENIED */
          XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
        }
        else
#endif
        {
      /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyReady.ContinueProcessing,1 */
#if(XCP_WRITE_READ_MEMORY_RAM == STD_ON)
      /* Callout function return value */
      /* Deviation MISRAC2012-4 <START> */
      CalloutResForRead = Xcp_DescriptorCallbackTypePtr.readFromRAMCallback(
                  &(ResponseCTO.CTO[XCP_ADDRESS_GRANULARITY_SIZE]),
                  XCP_APPL_GET_ADDRESS( 0U, (P2VAR(void, AUTOMATIC, XCP_APPL_DATA))Xcp_MTA.Address.Write ),
                  NoOfDataBytes);
      /* Deviation MISRAC2012-4 <STOP> */
      /* Check the callout function return */
      switch( CalloutResForRead )
      {
        /* !LINKSTO Xcp.CalibrationCommands.ApplBusy, 1 */
        case XCP_APPL_BUSY:
          RetValue = XCP_E_BUSY;
          break;
        case XCP_APPL_OK:
          /* Prepare CTO with positive response */
          ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
          /* Safe to set the cast to uint8 here as NoOfDataBytes + XCP_ADDRESS_GRANULARITY_SIZE is lower
          * than XCP_MAX_CTO, which can be maximum 255 */
          ResponseCTO.Length = (uint8)(NoOfDataBytes + XCP_ADDRESS_GRANULARITY_SIZE);
          /* Reset MTA */
          Xcp_MTA.Address.Write = &Xcp_MTA.Address.Write[NoOfDataBytes];
          break;
        case XCP_APPL_ERR_OUT_OF_RANGE:
         /* Prepare error CTO with negative response XCP_APPL_ERR_OUT_OF_RANGE */
          XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
          break;
        case XCP_APPL_ERR_ACCESS_DENIED:
        /* Prepare error CTO with negative response ERR_ACCESS_DENIED */
          XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
          break;
        default:
          /* EB specific handling :*/
          /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
          XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
          break;
      } /* switch( CalloutResForRead ) */

#else
          TS_MemCpy(&(ResponseCTO.CTO[XCP_ADDRESS_GRANULARITY_SIZE]),
                  XCP_APPL_GET_ADDRESS( 0U, (P2VAR(void, AUTOMATIC, XCP_APPL_DATA)) Xcp_MTA.Address.Write ),
                    NoOfDataBytes);
          /* Prepare CTO with positive response */
          ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
          /* Safe to set the cast to uint8 here as NoOfDataBytes + XCP_ADDRESS_GRANULARITY_SIZE is lower
          * than XCP_MAX_CTO, which can be maximum 255 */
          ResponseCTO.Length = (uint8)(NoOfDataBytes + XCP_ADDRESS_GRANULARITY_SIZE);
                  /* Reset MTA */
          Xcp_MTA.Address.Write = &Xcp_MTA.Address.Write[NoOfDataBytes];
#endif /* XCP_WRITE_READ_MEMORY_RAM == STD_ON */
        }
#endif
      }
    }
  }
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
  else
  {
    /* keep waiting for the memory proxy */
    RetValue = XCP_E_BUSY;
  }
#endif
  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  if (ResponseCTO.Length > 0U)
  {
    /* Enqueue response packet */
    Xcp_InsertResponseCTO(&ResponseCTO);
  }

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF)
  Xcp_SendCmdPendingEventCto(RetValue);
#endif
  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDSHORTUPLOAD_EXIT(RetValue);
  return RetValue;
}

#if (XCP_USE_MEMORY_PROXY == STD_ON)
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdUpload_BswDistribution(void)
{
  Xcp_ErrorType RetVal = XCP_E_NOT_OK;
  Xcp_CTOType ResponseCTO;
  boolean ProvideResponse = FALSE;
  Std_ReturnType ret;
  const Xcp_MemoryAreaType* MemoryArea;
  uint8 MemAreaIdx;
  const uint16 NoOfDataBytesRequested = (uint16)( XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX) ) * XCP_ADDRESS_GRANULARITY_SIZE;

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
  Atomic_value_t localRAMCalloutProxyState = Atomics_Load(&Xcp_RAMCalloutProxyState);
  /* !LINKSTO Xcp.BswDistribution.CalibrationCommands.UserDefinedCallouts.ProvideResponse,1 */
  if (localRAMCalloutProxyState == XCP_RESULT_ALREADY_PROCESSED)
  {
    RetVal = Xcp_ProcessCmdUpload();
    Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_NO_USER_CALLOUT_REQUESTED);
    Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_AVAILABLE);
  }
  else if (localRAMCalloutProxyState == XCP_NO_USER_CALLOUT_REQUESTED)
#endif
  {
    if (Xcp_CmdProcessorState == XCP_CMDPROCESSOR_IDLE)
    {
      if (( Xcp_MTA.Type == XCP_MTATYPE_COUNTER) ||
          ( Xcp_MTA.IsSet == FALSE))
      {
        /* Prepare error CTO with negative response XCP_ERR_ACCESS_DENIED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
        ProvideResponse = TRUE;
      }
#if (XCP_SLAVE_BLOCK_MODE == XCP_SLAVE_BLOCK_MODE_MASK)
      /* Verify whether requested length is valid. */
      else if (NoOfDataBytesRequested == 0U)
#else /* Block mode is not supported */
      /* Verify whether requested length is valid. */
      else if (
                (NoOfDataBytesRequested > (XCP_MAX_CTO - XCP_ADDRESS_GRANULARITY_SIZE)) ||
                (NoOfDataBytesRequested == 0U)
              )
#endif /* (XCP_SLAVE_BLOCK_MODE == XCP_SLAVE_BLOCK_MODE_MASK) */
      {
        /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
        ProvideResponse = TRUE;
      }
      else
      {
        const uint8 ApplicationId = (uint8)GetApplicationID();
        Xcp_MemoryProxyDataType Xcp_MemoryProxyData;
        Xcp_MemoryProxyData.type = XCP_MEMORYPROXYDATA_UPLOAD;
        Xcp_MemoryProxyData.size = NoOfDataBytesRequested;
        Xcp_MemoryProxyData.address = XCP_APPL_GET_ADDRESS(0, Xcp_MTA.Address.Write);
        Xcp_MemoryProxyData.EventId = XCP_E_INVALID_EVENT;
        /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.ProtectedMemoryLocations,1 */
        MemoryArea = Xcp_FindMemoryArea(XCP_APPL_GET_ADDRESS(0, Xcp_MTA.Address.Write), NoOfDataBytesRequested, XCP_MEMORY_AREA_TYPE_READ, XCP_MEMORY_AREA_SCOPE_CALIBRATION, &MemAreaIdx);
        if(MemoryArea == NULL_PTR)
        {
          /* !LINKSTO Xcp.BswDistribution.Calibration.UPLOAD.NoCalibrationMemoryAccessArea.Error,1 */
          /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.ProtectedMemoryLocations.NegativeResponse,1 */
          XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
          Xcp_InsertResponseCTO(&ResponseCTO);
          RetVal = XCP_E_NOT_OK;
        }
        else if ((uint8)GetApplicationID() != MemoryArea->ApplicationId)
        {
          /* !LINKSTO Xcp.BswDistribution.Calibration.UPLOAD.MemoryProxy.Usage,1 */
          /* !LINKSTO Xcp.BswDistribution.Communication.UnitInteraction.CoreLocal,1 */
          /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyAvailable,1 */
          /* !LINKSTO Xcp.dsn.BSWDistribution.MemProxy.ProtectedAccess,1 */
          if (Atomics_Load(&Xcp_MemoryProxyState) == XCP_MEMORY_PROXY_AVAILABLE)
          {
            /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyLocked,1 */
            Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_LOCKED);
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
            Xcp_MemoryProxyData.MemoryAreaIdx = MemAreaIdx;
            Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_REQUESTED_TO_MEMORY_PROXY);
#endif
            ret = Xcp_SendMemoryProxyData[ApplicationId][MemoryArea->ApplicationId](&Xcp_MemoryProxyData);
            if ( ret == SCHM_E_OK)
            {
              /* !LINKSTO Xcp.BswDistribution.Calibration.UPLOAD.MemoryProxy.Busy,1 */
              RetVal = XCP_E_BUSY;
            }
            else
            {
              /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.SchMError.MemProxyResetState,1 */
              Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_AVAILABLE);
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
              Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_NO_USER_CALLOUT_REQUESTED);
#endif
              RetVal = XCP_E_NOT_OK;
              /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.SchMError,1 */
              XCP_DET_REPORT_RUNTIME_ERROR(XCP_SID_MAIN_FUNCTION, XCP_E_SCHM_ERROR);
            }
          }
          if (Atomics_Load(&Xcp_MemoryProxyState) == XCP_MEMORY_PROXY_READY)
          {
            RetVal = Xcp_ProcessCmdUpload();
          }
        }
        else
        {
          /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyReady.ContinueProcessing,1 */
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
          Xcp_MemoryProxyData.MemoryAreaIdx = MemAreaIdx;
          Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_REQUESTED_TO_MEMORY_PROXY);
          Xcp_MemoryProxyHandler(&Xcp_MemoryProxyData);
          RetVal = XCP_E_BUSY;
#else
          /* bypass the memory proxy entirely */
          Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_READY);
          TS_MemCpy(&Xcp_MemoryProxyBuffer, Xcp_MemoryProxyData.address, Xcp_MemoryProxyData.size);
          RetVal = Xcp_ProcessCmdUpload();
#endif
        }
      }
    }
    else
    {
      if (Atomics_Load(&Xcp_MemoryProxyState) == XCP_MEMORY_PROXY_LOCKED)
      {
        RetVal = XCP_E_BUSY;
      }
      else
      {
        RetVal = Xcp_ProcessCmdUpload();
      }
    }
  }
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
  else
  {
    /* keep waiting for the memory proxy */
    RetVal = XCP_E_BUSY;
  }
#endif

  if(ProvideResponse == TRUE)
  {
    Xcp_InsertResponseCTO(&ResponseCTO);
  }

  return RetVal;
}
#endif

/*-------------------------[Xcp_ProcessCmdUpload]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdUpload(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  /* Local variable to hold return value */
  Xcp_ErrorType RetVal = XCP_E_NOT_OK;
  /* Callout function return value */
  Xcp_ApplReturnType CalloutResForRead;
  /* Flag signaling whether this command execution must output a response to the master */
  boolean ProvideResponse = FALSE;
  const uint8 NoOfDataBytesRequested = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX) * XCP_ADDRESS_GRANULARITY_SIZE;
  /* Variable holding the number of bytes this command execution should upload */
  uint8 NoOfDataBytesToUploadNow = NoOfDataBytesRequested;

  DBG_XCP_PROCESSCMDUPLOAD_ENTRY();

  /* Perform initial checks and assignments */
#if (XCP_USE_MEMORY_PROXY == STD_ON)
  if (Atomics_Load(&Xcp_MemoryProxyState) == XCP_MEMORY_PROXY_READY)
  {
#else
  if (Xcp_CmdProcessorState == XCP_CMDPROCESSOR_IDLE)
  {
#endif

#if (XCP_USE_MEMORY_PROXY == STD_ON)
    /* !LINKSTO Xcp.BswDistribution.Calibration.UPLOAD.MemoryProxy.Finished.State,1 */
    /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyReady.ResetState,1 */
    Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_AVAILABLE);
    Xcp_MemoryProxyBufferIndex = 0;
#endif

    /* Check whether MTA indicates address */
    if (( Xcp_MTA.Type == XCP_MTATYPE_COUNTER) ||
        ( Xcp_MTA.IsSet == FALSE))
    {
      /* Prepare error CTO with negative response XCP_ERR_ACCESS_DENIED */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
      ProvideResponse = TRUE;
    }
#if (XCP_SLAVE_BLOCK_MODE == XCP_SLAVE_BLOCK_MODE_MASK)
    /* Verify whether requested length is valid. */
    else if (NoOfDataBytesRequested == 0U)
#else /* Block mode is not supported */
    /* Verify whether requested length is valid. */
    else if (
              (NoOfDataBytesRequested > (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto - XCP_ADDRESS_GRANULARITY_SIZE)) ||
              (NoOfDataBytesRequested == 0U)
            )
#endif /* (XCP_SLAVE_BLOCK_MODE == XCP_SLAVE_BLOCK_MODE_MASK) */
    {
      /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
      ProvideResponse = TRUE;
    }
    else
    {
#if (XCP_SLAVE_BLOCK_MODE != XCP_SLAVE_BLOCK_MODE_MASK)
      /* Only one upload will be made. Directly initialize the number of bytes to upload now */
      NoOfDataBytesToUploadNow = NoOfDataBytesRequested;
#else
      /* Initialize the remaining data bytes to upload. */
      Xcp_RemainingDataBytesToUpload = NoOfDataBytesRequested;
#endif /* (XCP_SLAVE_BLOCK_MODE != XCP_SLAVE_BLOCK_MODE_MASK) */
      RetVal = XCP_E_OK;
    }
  }
  else
  {
     RetVal = XCP_E_OK;
  }

  /* If checks were OK, or there was no need for checking (in the busy case)*/
  if (RetVal == XCP_E_OK)
  {
#if (XCP_SLAVE_BLOCK_MODE == XCP_SLAVE_BLOCK_MODE_MASK)
    /* if the number of remaining data bytes to upload are higher than the maximum allowed data
     * bytes to be uploaded in one message  */
    if (Xcp_RemainingDataBytesToUpload > (uint8)XCP_MAX_UPLOAD_BYTES_ONE_CTO)
    {
      /* Upload maximum bytes allowed */
      NoOfDataBytesToUploadNow = XCP_MAX_UPLOAD_BYTES_ONE_CTO;
    }
    else
    {
      /* Upload the remaining bytes. Safe to cast to uint8. */
      NoOfDataBytesToUploadNow = (uint8)Xcp_RemainingDataBytesToUpload;
    }
#endif /* (XCP_SLAVE_BLOCK_MODE != XCP_SLAVE_BLOCK_MODE_MASK) */

    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    /* It might be that the busy command was aborted in the meantime
     * (SYNCH or another command during BUSY state). If so, we exit the command processing */
    if (XCP_IS_CMD_BUSY_ABORTED() == TRUE)
    {
      /* Processing will not continue */
      RetVal = XCP_E_NOT_OK;
      /* Clear the aborted flag */
      Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_BUSY_ABORTED_MASK);
      SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      /* Do not provide a response */
      ProvideResponse = FALSE;
    }
    else
    {
      SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      ProvideResponse = TRUE;
/* !LINKSTO Xcp.ProtectedMemoryLocations.General,1 */
#if ((XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON) && (XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF))
      if (Xcp_IsMemoryAreaAccessible
             (
                 XCP_APPL_GET_ADDRESS(0U, Xcp_MTA.Address.Write),
                 NoOfDataBytesToUploadNow,
                 XCP_MEMORY_AREA_TYPE_READ,
                 XCP_MEMORY_AREA_SCOPE_CALIBRATION
             ) == FALSE
         )
      {
        /* !LINKSTO Xcp.ProtectedMemoryLocations.NegativeResponse.Commands,1 */
        /* Prepare error CTO with negative response ERR_ACCESS_DENIED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
      }
      else
#endif
      {
        /* Deviation MISRAC2012-4 <START> */
#if ((XCP_WRITE_READ_MEMORY_RAM == STD_ON) && (XCP_USE_MEMORY_PROXY == STD_OFF))
        CalloutResForRead = Xcp_DescriptorCallbackTypePtr.readFromRAMCallback(
                    &(ResponseCTO.CTO[XCP_ADDRESS_GRANULARITY_SIZE]),
                    XCP_APPL_GET_ADDRESS( 0U, (P2VAR(void, AUTOMATIC, XCP_APPL_DATA))Xcp_MTA.Address.Write ),
                    NoOfDataBytesToUploadNow);

#else
        TS_MemCpy(&(ResponseCTO.CTO[XCP_ADDRESS_GRANULARITY_SIZE]),
                  XCP_APPL_GET_ADDRESS( 0U, (P2VAR(void, AUTOMATIC, XCP_APPL_DATA))Xcp_MTA.Address.Write ),
                  NoOfDataBytesToUploadNow);
        CalloutResForRead = XCP_APPL_OK;
#endif /* (XCP_WRITE_READ_MEMORY_RAM == STD_ON) */
        /* Deviation MISRAC2012-4 <STOP> */
        /* Check the callout function return */
        switch( CalloutResForRead )
        {
          case XCP_APPL_OK:
            /* Prepare CTO with positive response */
            ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
            /* Safe to set the cast to uint8 here as NoOfDataBytes + XCP_ADDRESS_GRANULARITY_SIZE is lower
            * than XCP_MAX_CTO, which can be maximum 255 */
            ResponseCTO.Length = NoOfDataBytesToUploadNow + XCP_ADDRESS_GRANULARITY_SIZE;
            /* Post-increment MTA */
            Xcp_MTA.Address.Write = &Xcp_MTA.Address.Write[NoOfDataBytesToUploadNow];
#if (XCP_USE_MEMORY_PROXY == STD_ON)
            Xcp_MemoryProxyBufferIndex += NoOfDataBytesToUploadNow;
#endif
#if (XCP_SLAVE_BLOCK_MODE == XCP_SLAVE_BLOCK_MODE_MASK)
            Xcp_RemainingDataBytesToUpload -= NoOfDataBytesToUploadNow;
#endif
            break;
          /* !LINKSTO Xcp.CalibrationCommands.ApplBusy, 1 */
          case XCP_APPL_BUSY:
            RetVal = XCP_E_BUSY;
            ProvideResponse = FALSE;
            break;
          case XCP_APPL_ERR_OUT_OF_RANGE:
           /* Prepare error CTO with negative response XCP_APPL_ERR_OUT_OF_RANGE */
            XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
            RetVal = XCP_E_NOT_OK;
            break;
          case XCP_APPL_ERR_ACCESS_DENIED:
          /* Prepare error CTO with negative response ERR_ACCESS_DENIED */
            XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
            RetVal = XCP_E_NOT_OK;
            break;
          default:
            /* EB specific handling :*/
            /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
            XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
            RetVal = XCP_E_NOT_OK;
            break;
        } /* switch( CalloutResForRead ) */
#if (XCP_SLAVE_BLOCK_MODE == XCP_SLAVE_BLOCK_MODE_MASK)
        /* if we still need to upload */
        if (Xcp_RemainingDataBytesToUpload > 0U)
        {
          /* the command has not finished, emit busy state. It will be continued next time the
           * command processing is called */
          RetVal = XCP_E_BUSY;
        }
#endif
      }
    }
  }

  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  /* If a response is needed to be queued */
  if (ProvideResponse == TRUE)
  {
    /* Enqueue response packet. */
    Xcp_InsertResponseCTO(&ResponseCTO);
  }
  else
  {
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF)
    Xcp_SendCmdPendingEventCto(RetVal);
#endif
  }

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDUPLOAD_EXIT(RetVal);
  return RetVal;
}

/* !LINKSTO Xcp.ASAM.SHORTDOWNLOAD007,1 */
#if (XCP_MAX_CTO > 8)
/*-------------------------[Xcp_ProcessCmdShortDownload]-----------------------*/

/* !LINKSTO Xcp.ASAM.SHORTDOWNLOAD001,1 */
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdShortDownload(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  Xcp_ErrorType RetVal = XCP_E_OK;
    /* Local variable to hold requested number of elements */
  const uint8 NoOfDataElementsRequested = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);

  /* Initialize the ResponseCTO length */
  ResponseCTO.Length = 0U;

  /* Check if number of elements requested is valid */
  if(((NoOfDataElementsRequested == 0U) || (NoOfDataElementsRequested > XCP_MAX_SHORT_DOWNLOAD_ELEMENTS))
#if (XCP_USE_MEMORY_PROXY == STD_ON)
     /* the command processor state can be in busy state meaning
        that the memory proxy has not yet finished
      */
     && (Xcp_CmdProcessorState == XCP_CMDPROCESSOR_IDLE)
#endif
  )
  {
    /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
  }
  else
  {
    if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_CAL_PAG_MASK) ==
                   XCP_RESOURCE_CAL_PAG_MASK)
    {
      /* Calibration commands are protected; return ERR_ACCESS_LOCKED */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
    }
    else
    {
      /* !LINKSTO Xcp.ASAM.SHORTDOWNLOAD005,1 */
      /* Set the MTA */
      Xcp_MTA.Type   = XCP_MTATYPE_ADDRESS;
      TS_MemCpy(&Xcp_MTA.Address.Write,
                &(Xcp_CommandQueue.Queue[Xcp_CommandQueue.Head].CTO[4U]),
                XCP_MTA_ADDRESS_LENGTH);
      /* Mark the MTA as being set */
      Xcp_MTA.IsSet = TRUE;

      /* Set the variable holding the number of elements to download with the requested number found
       * in the command */
      if (Xcp_MemoryWrite.Pending == FALSE)
      {
        Xcp_MemoryWrite.ElementsLeftToWrite = NoOfDataElementsRequested;
      }
      /* Process the elements for download */
      RetVal = Xcp_ProcessOneDownload(&ResponseCTO, XCP_SHORT_DOWNLOAD_FIRST_ELEMENT_IDX, NoOfDataElementsRequested);
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Check if we need to send a response */
  /* CHECK: NOPARSE */
  if (ResponseCTO.Length > 0U)
  /* CHECK: PARSE */
  {
    /* Queue the response */
    Xcp_InsertResponseCTO(&ResponseCTO);

    /* Reset the globals used by any memory writing routine */
    Xcp_ResetMemoryWriteInfo();
  }
  /* CHECK: NOPARSE */
  else
  {
    /* Unreachable else branch. A response in the CTO shall always be enqueued
    no matter which situation occurs in cases above */
    XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
  }
  /* CHECK: PARSE */

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF)
  Xcp_SendCmdPendingEventCto(RetVal);
#endif
  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  return RetVal;
}

#endif /* XCP_MAX_CTO > 8 */

/*-------------------------[Xcp_ProcessCmdDownloadMax]-----------------------*/

/* !LINKSTO Xcp.ASAM.DOWNLOADMAX001,1 */
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdDownloadMax(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  Xcp_ErrorType RetVal = XCP_E_OK;

  /* Initialize the ResponseCTO length */
  ResponseCTO.Length = 0U;
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_CAL_PAG_MASK) ==
                 XCP_RESOURCE_CAL_PAG_MASK)
  {
    /* Calibration commands are protected; return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    if (( Xcp_MTA.Type == XCP_MTATYPE_COUNTER) || ( Xcp_MTA.IsSet == FALSE))
    {
      /* Prepare error CTO with negative response XCP_ERR_ACCESS_DENIED */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
    }
    else
    {
      /* !LINKSTO Xcp.ASAM.DOWNLOADMAX005,1 */
      /* Set the variable holding the number of elements to download with the requested number found
       * in the command */
      if (Xcp_MemoryWrite.Pending == FALSE)
      {
        Xcp_MemoryWrite.ElementsLeftToWrite = XCP_MAX_DOWNLOAD_MAX_ELEMENTS;
      }
      /* Process the elements for download */
      RetVal = Xcp_ProcessOneDownload(&ResponseCTO, XCP_DOWNLOAD_MAX_FIRST_ELEMENT_IDX, XCP_MAX_DOWNLOAD_MAX_ELEMENTS);
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Check if we need to send a response  */
  /* CHECK: NOPARSE */
  if (ResponseCTO.Length > 0U)
  /* CHECK: PARSE */
  {
    /* Queue the response */
    Xcp_InsertResponseCTO(&ResponseCTO);

    /* Reset the globals used by any memory writing routine */
    Xcp_ResetMemoryWriteInfo();
  }
  /* CHECK: NOPARSE */
  else
  {
    /* Unreachable else branch. A response in the CTO shall always be enqueued
    no matter which situation occurs in cases above */
    XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
  }
  /* CHECK: PARSE */

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF)
  Xcp_SendCmdPendingEventCto(RetVal);
#endif
  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  return RetVal;
}

/*-------------------------[Xcp_ProcessCmdDownload]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdDownload(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
    /* Local variable to hold requested number of data bytes */
  const uint8 NoOfDataElementsRequested = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);
  uint8 Xcp_Max_Download_Elements = XCP_MAX_DOWNLOAD_ELEMNTS_ONE_CMD;
  Xcp_ErrorType RetVal = XCP_E_OK;

  DBG_XCP_PROCESSCMDDOWNLOAD_ENTRY();
  /* Initialize the ResponseCTO length */
  ResponseCTO.Length = 0U;
#if (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)
  if (XCP_MAX_DOWNLOAD_ELEMNTS_ONE_CMD * Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_BS < XCP_ABS_MAX_DOWNLOAD_ELEMENTS)
  {
    Xcp_Max_Download_Elements = XCP_MAX_DOWNLOAD_ELEMNTS_ONE_CMD * Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_BS;
  }
  else
  {
    Xcp_Max_Download_Elements = XCP_ABS_MAX_DOWNLOAD_ELEMENTS;
  }
#endif

  if ((NoOfDataElementsRequested == 0U) ||
      (NoOfDataElementsRequested > Xcp_Max_Download_Elements))
  {
    /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
  }
  else
  {
    if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_CAL_PAG_MASK) ==
                   XCP_RESOURCE_CAL_PAG_MASK)
    {
      /* Calibration commands are protected; return ERR_ACCESS_LOCKED */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
    }
    else
    {
      if (( Xcp_MTA.Type == XCP_MTATYPE_COUNTER) || ( Xcp_MTA.IsSet == FALSE))
      {
        /* Prepare error CTO with negative response XCP_ERR_ACCESS_DENIED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
      }
      else
      {
        /* Set the variable holding the number of elements to download with the requested number found
         * in the command */
        if (Xcp_MemoryWrite.Pending == FALSE)
        {
          Xcp_MemoryWrite.ElementsLeftToWrite = NoOfDataElementsRequested;
        }
        /* Process the elements for download */
        RetVal = Xcp_ProcessOneDownload(&ResponseCTO, XCP_GET_START_DATA_INDEX_FROM_CTO(), XCP_MAX_DOWNLOAD_ELEMNTS_ONE_CMD);
      }
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Check if we need to send a response  */
  if (ResponseCTO.Length > 0U)
  {
#if (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)
    /* Download has finished - clear the flag */
    Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_DOWNLOAD_BLOCK_MODE_MASK);
#endif

    /* Queue the response */
    Xcp_InsertResponseCTO(&ResponseCTO);

    /* Reset the globals used by any memory writing routine */
    Xcp_ResetMemoryWriteInfo();
  }
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF)
  Xcp_SendCmdPendingEventCto(RetVal);
#endif
  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_PROCESSCMDDOWNLOAD_EXIT(RetVal);
  return RetVal;
}

/*-------------------------[Xcp_ProcessCmdDownloadNext]-----------------------*/
#if (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdDownloadNext(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  Xcp_ErrorType RetVal = XCP_E_OK;
  DBG_XCP_PROCESSCMDDOWNLOADNEXT_ENTRY();
  /* Initialize the ResponseCTO length */
  ResponseCTO.Length = 0U;

  /* Check that the number of elements left to download match the expected */
  if ((Xcp_MemoryWrite.ElementsLeftToWrite != XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX)) &&
      (FALSE == Xcp_MemoryWrite.Pending)
#if (XCP_USE_MEMORY_PROXY == STD_ON)
     /* the command processor state can be in busy state meaning
        that the memory proxy has not yet finished
      */
     && (Xcp_CmdProcessorState == XCP_CMDPROCESSOR_IDLE)
#endif
     )
  {
    /* If the number of data elements does not match the expected value, the error code
     * ERR_SEQUENCE will be returned.
     * The negative response will contain the expected number of data elements. */
    ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_ERR_PID;
    ResponseCTO.CTO[XCP_RES_ERRCODE_INDEX] = XCP_ERR_SEQUENCE;
    ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = Xcp_MemoryWrite.ElementsLeftToWrite;
    ResponseCTO.Length = XCP_ERR_WRITE_SEQUENCE_LENGTH;
  }
  else
  {
    /* Process the elements for download */
    RetVal = Xcp_ProcessOneDownload(&ResponseCTO, XCP_GET_START_DATA_INDEX_FROM_CTO(), XCP_MAX_DOWNLOAD_ELEMNTS_ONE_CMD);
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Check if we need to send a response  */
  if (ResponseCTO.Length > 0U)
  {
    /* Download has finished - clear the flag */
    Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_DOWNLOAD_BLOCK_MODE_MASK);

    /* Queue the response */
    Xcp_InsertResponseCTO(&ResponseCTO);

    /* Reset the globals used by any memory writing routine */
    Xcp_ResetMemoryWriteInfo();
  }
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF)
  Xcp_SendCmdPendingEventCto(RetVal);
#endif
  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_PROCESSCMDDOWNLOADNEXT_EXIT(RetVal);
  return RetVal;
}
#endif /* (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) */

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessOneDownload
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA)  ResponseCTO,
  uint8 IndexInCmdQueueOfFirstElement,
  uint8 MaxDownloadElements
)
{
  /* Callout function return value */
  Xcp_ApplReturnType CalloutResForWrite;
  /* the number of elements that will be processed */
  uint8 NumberOfElementsToProcessNow = 0U;
  Xcp_ErrorType RetValue = XCP_E_OK;
#if (XCP_USE_MEMORY_PROXY == STD_ON)
  const Xcp_MemoryAreaType* MemoryArea;
  boolean OptimizedBswDistribution = FALSE;
  uint8 MemAreaIdx;
#endif

  DBG_XCP_PROCESSONEDOWNLOAD_ENTRY(ResponseCTO, IndexInCmdQueueOfFirstElement, MaxDownloadElements);

  if (FALSE == Xcp_MemoryWrite.Pending)
  {
    /* calculate number of elements to be processed now */
    if (Xcp_MemoryWrite.ElementsLeftToWrite >= MaxDownloadElements)
    {
      /* process the maximum number of elements possible in this step */
      NumberOfElementsToProcessNow = MaxDownloadElements;
    }
    else
    {
      /* process the last remaining elements or a single download (no DOWNLOAD_NEXT afterwards)
       * with less then maximum elements that can be processed in one cycle */
      NumberOfElementsToProcessNow = Xcp_MemoryWrite.ElementsLeftToWrite;
    }

#if (XCP_USE_MEMORY_PROXY == STD_ON)
  /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.ProtectedMemoryLocations,1 */
  MemoryArea = Xcp_FindMemoryArea(XCP_APPL_GET_ADDRESS(0, Xcp_MTA.Address.Write), NumberOfElementsToProcessNow * (uint32)XCP_ADDRESS_GRANULARITY_SIZE, XCP_MEMORY_AREA_TYPE_WRITE, XCP_MEMORY_AREA_SCOPE_CALIBRATION, &MemAreaIdx);
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF)
    /* Deviation MISRAC2012-6 */
    if ((MemoryArea != NULL_PTR) && ((uint8)GetApplicationID() == MemoryArea->ApplicationId))
    {
      OptimizedBswDistribution = TRUE;
    }
#endif
#endif
    /* copy the elements in the internal buffer */
    TS_MemCpy
    (
      (&Xcp_MemoryWrite.Buffer[Xcp_MemoryWrite.ElementsWrittenSoFar * XCP_ADDRESS_GRANULARITY_SIZE]),
      &(Xcp_CommandQueue.Queue[Xcp_CommandQueue.Head].CTO[IndexInCmdQueueOfFirstElement]),
      ( (uint32)NumberOfElementsToProcessNow * XCP_ADDRESS_GRANULARITY_SIZE )
    );

    /* increment the number of elements processed so far */
    Xcp_MemoryWrite.ElementsWrittenSoFar += (NumberOfElementsToProcessNow * XCP_ADDRESS_GRANULARITY_SIZE);

    /* decrement the number of elements remaining to be processed with the number of elements
       * processed in this step */
    Xcp_MemoryWrite.ElementsLeftToWrite -= NumberOfElementsToProcessNow;
  }
/* !LINKSTO Xcp.ProtectedMemoryLocations.General,1 */
#if ((XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON) && (XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF))
  /* Deviation MISRAC2012-6 */
  if ( (FALSE == Xcp_MemoryWrite.Pending) &&
       (Xcp_IsMemoryAreaAccessible
       (
           XCP_APPL_GET_ADDRESS(0U, Xcp_MTA.Address.Write),
           ((uint32)NumberOfElementsToProcessNow * XCP_ADDRESS_GRANULARITY_SIZE),
           XCP_MEMORY_AREA_TYPE_WRITE,
           XCP_MEMORY_AREA_SCOPE_CALIBRATION
       ) == FALSE)
     )
  {
    /* !LINKSTO Xcp.ProtectedMemoryLocations.NegativeResponse.Commands,1 */
    /* Prepare error CTO with negative response ERR_ACCESS_DENIED */
    XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
  }
  else
#endif /* XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON */
  {
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
    Atomic_value_t localRAMCalloutProxyState = Atomics_Load(&Xcp_RAMCalloutProxyState);
    /* !LINKSTO Xcp.BswDistribution.CalibrationCommands.UserDefinedCallouts.ProvideResponse,1 */
    if (localRAMCalloutProxyState == XCP_RESULT_ALREADY_PROCESSED)
    {
      /* increment MTA */
      Xcp_MTA.Address.Write = &Xcp_MTA.Address.Write[(uint8)(Xcp_MemoryWrite.ElementsWrittenSoFar * XCP_ADDRESS_GRANULARITY_SIZE)];
      /* send a positive response, since the download has finished */
      ResponseCTO->CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
      ResponseCTO->Length = XCP_DOWNLOAD_POSITIVE_LENGTH;
      Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_NO_USER_CALLOUT_REQUESTED);
      RetValue = XCP_E_OK;
      Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_AVAILABLE);
    }
    else if (localRAMCalloutProxyState == XCP_NO_USER_CALLOUT_REQUESTED)
#endif
    {
      /* if download has finished */
      if (Xcp_MemoryWrite.ElementsLeftToWrite == 0U)
      {
        /* calculate the number of bytes to copy from the buffer to the intended memory location */
        uint8 NumberOfBytesToDownload = (uint8)(Xcp_MemoryWrite.ElementsWrittenSoFar *
                                                XCP_ADDRESS_GRANULARITY_SIZE);
        /*  we execute the download if the CTO response queue has room to enqueue the response */
        /* CHECK: NOPARSE */
        if (XCP_GET_CTOQUEUE_FREESPACE(uint32) >= XCP_DOWNLOAD_POSITIVE_LENGTH)
        /* CHECK: PARSE */
        {
#if (XCP_USE_MEMORY_PROXY == STD_ON)
          if (OptimizedBswDistribution == FALSE)
          {
            /* !LINKSTO Xcp.BswDistribution.Calibration.DOWNLOAD.MemoryProxy.Usage,1 */
            Xcp_MemoryWrite.ElementsWrittenSoFar = 0U;
            /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyAvailable,1 */
            /* !LINKSTO Xcp.dsn.BSWDistribution.MemProxy.ProtectedAccess,1 */
            if (Atomics_Load(&Xcp_MemoryProxyState) == XCP_MEMORY_PROXY_AVAILABLE)
            {
              Xcp_MemoryProxyDataType Xcp_MemoryProxyData;
              const uint8 ApplicationId = (uint8)GetApplicationID();
              Xcp_MemoryProxyData.type = XCP_MEMORYPROXYDATA_DOWNLOAD;
              Xcp_MemoryProxyData.address = XCP_APPL_GET_ADDRESS(0, Xcp_MTA.Address.Write);
              Xcp_MemoryProxyData.size = NumberOfBytesToDownload;
              Xcp_MemoryProxyData.EventId = XCP_E_INVALID_EVENT;
              MemoryArea = Xcp_FindMemoryArea(Xcp_MemoryProxyData.address, NumberOfBytesToDownload, XCP_MEMORY_AREA_TYPE_WRITE, XCP_MEMORY_AREA_SCOPE_CALIBRATION, &MemAreaIdx);
              if (MemoryArea != NULL_PTR)
              {
                Std_ReturnType result;
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
                Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_REQUESTED_TO_MEMORY_PROXY);
#endif
                /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyLocked,1 */
                Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_LOCKED);
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
                Xcp_MemoryProxyData.MemoryAreaIdx = MemAreaIdx;
#endif
                TS_MemCpy(&Xcp_MemoryProxyBuffer, &Xcp_MemoryWrite.Buffer, NumberOfBytesToDownload);
                if (ApplicationId == MemoryArea->ApplicationId)
                {
                  Xcp_MemoryProxyHandler(&Xcp_MemoryProxyData);
                  result = SCHM_E_OK;
                }
                else
                {
                  result = Xcp_SendMemoryProxyData[ApplicationId][MemoryArea->ApplicationId](&Xcp_MemoryProxyData);
                }
                if ( result == SCHM_E_OK)
                {
                  /* !LINKSTO Xcp.BswDistribution.Calibration.DOWNLOAD.MemoryProxy.Busy,1 */
                  /* !LINKSTO Xcp.BswDistribution.Calibration.DOWNLOAD_MAX.MemoryProxy.Busy,1 */
                  /* !LINKSTO Xcp.BswDistribution.Calibration.SHORT_DOWNLOAD.MemoryProxy.Busy,1 */
                  /* !LINKSTO Xcp.BswDistribution.Calibration.DOWNLOAD_NEXT.MemoryProxy.Busy,1 */
                  RetValue = XCP_E_BUSY;
                }
                else
                {
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
                  Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_NO_USER_CALLOUT_REQUESTED);
#endif
                  /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.SchMError.MemProxyResetState,1 */
                  Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_AVAILABLE);
                  /* intentionally no response */
                  /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.SchMError,1 */
                  XCP_DET_REPORT_RUNTIME_ERROR(XCP_SID_MAIN_FUNCTION, XCP_E_SCHM_ERROR);
                }
              }
              else
              {
                /* !LINKSTO Xcp.BswDistribution.Calibration.DOWNLOAD.NoCalibrationMemoryAccessArea.Error,1 */
                /* !LINKSTO Xcp.BswDistribution.Calibration.DOWNLOAD_MAX.NoCalibrationMemoryAccessArea.Error,1 */
                /* !LINKSTO Xcp.BswDistribution.Calibration.SHORT_DOWNLOAD.NoCalibrationMemoryAccessArea.Error,1 */
                /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.ProtectedMemoryLocations.NegativeResponse,1 */
                XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
              }
            }
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF)
            /* check the state again, it might be that the memory proxy task already executed */
            if (Atomics_Load(&Xcp_MemoryProxyState) == XCP_MEMORY_PROXY_READY)
            {
              /* increment MTA */
              Xcp_MTA.Address.Write = &Xcp_MTA.Address.Write[NumberOfBytesToDownload];
              /* send a positive response, since the download has finished */
              ResponseCTO->CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
              ResponseCTO->Length = XCP_DOWNLOAD_POSITIVE_LENGTH;
              /* !LINKSTO Xcp.BswDistribution.Calibration.DOWNLOAD.MemoryProxy.Finished.State,1 */
              /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyReady.ResetState,1 */
              Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_AVAILABLE);
              RetValue = XCP_E_OK;
            }
            else
            {
              /* keep waiting for the memory proxy */
              RetValue = XCP_E_BUSY;
            }
#endif
          }
          else
#endif
          {
            /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyReady.ContinueProcessing,1 */
            /* !LINKSTO Xcp.BswDistribution.Calibration.DOWNLOAD.Optimization,1 */
            /* reset the counter holding the number of elements downloaded in this download sequence */
            /*Xcp_MemoryWrite.ElementsWrittenSoFar = 0U;*/

            /* !LINKSTO Xcp.ASAM.DOWNLOADMAX002,1 */
            /* !LINKSTO Xcp.ASAM.SHORTDOWNLOAD002,1 */
            /* copy all the data from the buffer to the intended memory location pointed by the MTA */
            /* write to memory*/
#if(XCP_WRITE_READ_MEMORY_RAM == STD_ON)
            CalloutResForWrite = Xcp_DescriptorCallbackTypePtr.writeToRAMCallback(
                        XCP_APPL_GET_ADDRESS( 0U, Xcp_MTA.Address.Write ),
                        &(Xcp_MemoryWrite.Buffer[0U]),
                        NumberOfBytesToDownload);
#else
            SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
            TS_MemCpy(XCP_APPL_GET_ADDRESS( 0U, Xcp_MTA.Address.Write ),
                      &(Xcp_MemoryWrite.Buffer[0U]),
                      NumberOfBytesToDownload);
            SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
            CalloutResForWrite = XCP_APPL_OK;
#endif /* (XCP_WRITE_READ_MEMORY_RAM == STD_ON) */
            switch( CalloutResForWrite )
            {
              case XCP_APPL_OK:
                /* post-increment MTA address by the number of elements processed in this download instance */
              /* !LINKSTO Xcp.ASAM.DOWNLOADMAX003,1 */
              /* !LINKSTO Xcp.ASAM.SHORTDOWNLOAD003,1 */
                Xcp_MTA.Address.Write = &Xcp_MTA.Address.Write[NumberOfBytesToDownload];
                /* send a positive response, since the download has finished */
                ResponseCTO->CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
                ResponseCTO->Length                 = XCP_DOWNLOAD_POSITIVE_LENGTH;
                break;
              /* !LINKSTO Xcp.CalibrationCommands.ApplBusy, 1 */
              case XCP_APPL_BUSY:
                RetValue = XCP_E_BUSY;
                Xcp_MemoryWrite.Pending = TRUE;
                break;
              case XCP_APPL_ERR_OUT_OF_RANGE:
               /* Prepare error CTO with negative response XCP_APPL_ERR_OUT_OF_RANGE */
                XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
                break;
              default:
                /* EB specific handling :*/
                /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
                XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
                break;
            } /* switch( CalloutResForWrite ) */
          }
        }
        /* CHECK: NOPARSE */
        else
        {
          /* Unreachable else branch.
             The else branch can be covered only if the CTO queue is full (the value of
             XCP_DOWNLOAD_POSITIVE_LENGTH is 0x01U, so we need the CTO to be full so
             there is no byte free). If the CTO is full, the command ONE_DOWNLOAD
             cannot be sent and this if condition is not reached.
           */
          XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
        }
        /* CHECK: PARSE */
      }
    }
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
    else
    {
      /* keep waiting for the memory proxy */
      RetValue = XCP_E_BUSY;
    }
#endif
  }

  DBG_XCP_PROCESSONEDOWNLOAD_EXIT(RetValue);
  return RetValue;
}

/*-------------------------[Xcp_ProcessCmdTLSpecific]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdTLSpecific(void)
{
  /* Local variable to hold return value */
  Xcp_ErrorType RetVal = XCP_E_NOT_OK;
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  /* Local variable to store if check if the TTL CMD is implemented */
  boolean IsSubCmdSupported = TRUE;
  /* Local variable that identifies the subcommand ID */
  uint8 SubCommandId = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);

  DBG_XCP_PROCESSCMDTLSPECIFIC_ENTRY();

  switch (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].ConnectionType)
  {
#if ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON))
    case XCP_CONNECTION_OVER_CAN:
    case XCP_CONNECTION_OVER_CANFD:
    {
      switch (SubCommandId)
      {
        case XCP_SUBCMD_GET_SLAVE_ID_PID:
        {
          RetVal = Xcp_ProcessSubCmdGetSlaveIdOnCAN();
          break;
        }
#if (XCP_GET_DAQ_ID_SUPPORTED == STD_ON)
        case XCP_SUBCMD_GET_DAQ_ID_PID:
        {
          RetVal = Xcp_ProcessSubCmdGetDaqID();
          break;
        }
#endif
        default:
        {
           /* no TTL CMD is supported for the provided subcommandID */
           IsSubCmdSupported = FALSE;
           break;
        }
      }
      break;
    }
#endif /* XCP_ON_CAN_ENABLED == STD_ON || XCP_ON_CANFD_ENABLED == STD_ON */
#if (XCP_ON_FLEXRAY_ENABLED == STD_ON)
    case XCP_CONNECTION_OVER_FLEXRAY:
    {
#if (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)
      switch (SubCommandId)
      {
        case XCP_SUBCMD_FLX_ASSIGN:
        {
          RetVal = Xcp_ProcessSubCmdFlxAssign();
          break;
        }
        case XCP_SUBCMD_FLX_ACTIVATE:
        {
          RetVal = Xcp_ProcessSubCmdFlxActivate();
          break;
        }
        case XCP_SUBCMD_FLX_DEACTIVATE:
        {
          RetVal = Xcp_ProcessSubCmdFlxDeActivate();
          break;
        }
        case XCP_SUBCMD_GET_DAQ_FLX_BUF:
        {
          RetVal = Xcp_ProcessSubCmdFlxGetDaqListBuf();
          break;
        }
        case XCP_SUBCMD_SET_DAQ_FLX_BUF:
        {
          RetVal = Xcp_ProcessSubCmdFlxSetDaqListBuf();
          break;
        }
        default:
        {
           /* no TTL CMD is supported for the provided subcommandID */
           IsSubCmdSupported = FALSE;
           break;
        }
      }
#else
      TS_PARAM_UNUSED(SubCommandId);
      /* no subcommand is supported */
      IsSubCmdSupported = FALSE;
#endif /* XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON */
       break;
    }
#endif /* XCP_ON_FLEXRAY_ENABLED == STD_ON */
#if (XCP_ON_ETHERNET_ENABLED == STD_ON)
    case XCP_CONNECTION_OVER_TCPIP:
    {
      TS_PARAM_UNUSED(SubCommandId);
      /* no TTL CMD is supported for Ethernet TCPIP */
      IsSubCmdSupported = FALSE;
      break;
    }
    case XCP_CONNECTION_OVER_UDPIP:
    {
      switch (SubCommandId)
      {
/* !LINKSTO Xcp.Ethernet.Udp.GETSLAVEID,1 */

        case XCP_SUBCMD_GET_SLAVE_ID_PID:
        {
#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
          /* !LINKSTO Xcp.Ethernet.Udp.GETSLAVEID.OnlyOverIPv4,1 */
          /* !LINKSTO Xcp.ASAM.Ethernet.Udp.GETSLAVEID008,1 */
          if(XCP_IS_BYTE_FLAG_SET(Xcp_ConnectionStatus.ConnectionId,
                &Xcp_Connections_IPv6[Xcp_ConnectionStatus.ConnectionId / 8U]))
          {
            /* command not supported if connection type is IPv6 */
            IsSubCmdSupported = FALSE;
          }
          else
          {
            /* execute the command */
            RetVal = Xcp_ProcessSubCmdGetSlaveIdOnEthernet();
          }
#else
          TS_PARAM_UNUSED(SubCommandId);
          /* no subcommand is supported */
          IsSubCmdSupported = FALSE;
#endif /* XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON */
          break;
        }
        default:
          /* no TTL CMD is supported for the provided subcommandID */
          IsSubCmdSupported = FALSE;
          break;
      }
      break;
    }
#endif /* (XCP_ON_ETHERNET_ENABLED == STD_ON) */
#if (XCP_ON_CDD_ENABLED == STD_ON)
    case XCP_CONNECTION_OVER_CDD:
    {
      TS_PARAM_UNUSED(SubCommandId);
      /* no TTL CMD is supported for CDD */
      IsSubCmdSupported = FALSE;
      break;
    }
#endif /* XCP_ON_CDD_ENABLED == STD_ON */
/* CHECK: NOPARSE */
    default:
    {
       /* Unknown situation. Condition cannot be reached but a default is necessary for MISRA */
       XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
       break;
    }
/* CHECK: PARSE */
  }

  if((IsSubCmdSupported == FALSE) &&
     (XCP_STATE_CONNECTED == Xcp_ConnectionStatus.State))
  {
    /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    /* Enqueue response packet */
    Xcp_InsertResponseCTO(&ResponseCTO);
    /* Exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  }
  else
  {
    /* The TP command executed without CONNECT, mark the flag so that the PDUs related information
     *  (e.g. DAQ packet type is supported by that PDU) is not re-initialized with pre-configured values
     *  when a new receive indication is processed. */
    XCP_SET_FLAG_U8_ATOMIC(XCP_TP_CONFIGURATION_DONE_BIT, Xcp_ConnectionStatus.Flags);
  }

  DBG_XCP_PROCESSCMDTLSPECIFIC_EXIT(RetVal);

  return RetVal;
}

/*----------------------[Xcp_ProcessSubCmdGetSlaveIdOnCAN]-----------------------*/

#if ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON))

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdGetSlaveIdOnCAN(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  /* Local variable to hold command parameter start address */
  uint8 *ParamterPtr = &XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX);
  /* Local variable to hold requested mode */
  uint8 Mode = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE5_INDEX);
  /* Local constant holding response CTO CAN ID */
  const uint32 CanId = (uint32)Xcp_RxLPduIdConf[Xcp_ConnectionCfg[
                       Xcp_ConnectionStatus.ConnectionId].CtoSlavePduId].PduCanId;

  DBG_XCP_PROCESSSUBCMDGETSLAVEIDONCAN_ENTRY();

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Verify whether command parameter bytes 2,3 and 4 reads "XCP" */
  if ( ((XCP_ASCII_X != ParamterPtr[XCP_GET_SLAVE_ID_OFFSET_X])  ||
        (XCP_ASCII_C != ParamterPtr[XCP_GET_SLAVE_ID_OFFSET_C])) ||
        (XCP_ASCII_P != ParamterPtr[XCP_GET_SLAVE_ID_OFFSET_P])
     )
  {
    /* Prepare error CTO with negative response ERR_CMD_SYNTAX */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_SYNTAX, XCP_ERR_CMD_SYNTAX_LENGTH);
  }
  /* Verify requested mode parameter */
  else if (XCP_GET_SLAVE_ID_BY_INVERSE_ECHO < Mode)
  {
    /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
  }
  else
  {
    /* If mode = identify by echo */
    if (XCP_GET_SLAVE_ID_BY_ECHO == Mode)
    {
      /* Prepare CTO with positive response */
      ResponseCTO.CTO[XCP_RES_PID_INDEX]   = XCP_RES_PID;
      ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_ASCII_X;
      ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = XCP_ASCII_C;
      ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = XCP_ASCII_P;
      ResponseCTO.CTO[XCP_RES_BYTE4_INDEX] = (uint8)CanId;
      ResponseCTO.CTO[XCP_RES_BYTE5_INDEX] = (uint8)(CanId >> 8U);
      ResponseCTO.CTO[XCP_RES_BYTE6_INDEX] = (uint8)(CanId >> 16U);
      ResponseCTO.CTO[XCP_RES_BYTE7_INDEX] = (uint8)(CanId >> 24U);
      ResponseCTO.Length = XCP_SUBCMD_GET_SLAVE_ID_ON_CAN_RES_LENGTH;

      /* Set GET_SLAVE_ID request receive status */
      Xcp_CmdReqStatus |= XCP_RECEIVED_GET_SLAVE_ID_ECHO_MASK;
    }
    /* If mode = identify by inverse echo */
    else
    {
      /* If there was a prior request with mode = identify by echo */
      if (XCP_RECEIVED_GET_SLAVE_ID_ECHO_MASK
          == (Xcp_CmdReqStatus & XCP_RECEIVED_GET_SLAVE_ID_ECHO_MASK))
      {
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_ASCII_INVERSE_X;
        ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = XCP_ASCII_INVERSE_C;
        ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = XCP_ASCII_INVERSE_P;
        ResponseCTO.CTO[XCP_RES_BYTE4_INDEX] = (uint8)CanId;
        ResponseCTO.CTO[XCP_RES_BYTE5_INDEX] = (uint8)(CanId >> 8U);
        ResponseCTO.CTO[XCP_RES_BYTE6_INDEX] = (uint8)(CanId >> 16U);
        ResponseCTO.CTO[XCP_RES_BYTE7_INDEX] = (uint8)(CanId >> 24U);
        ResponseCTO.Length = XCP_SUBCMD_GET_SLAVE_ID_ON_CAN_RES_LENGTH;

        /* Clear GET_SLAVE_ID request receive status */
        Xcp_CmdReqStatus &= (uint8)(~XCP_RECEIVED_GET_SLAVE_ID_ECHO_MASK);
      }
      else
      {
        /* Ignore request silently */
        ResponseCTO.Length = 0U;
      }
    }
  }
  /* If there is a response to send */
  if (0U != ResponseCTO.Length)
  {
    /* Enqueue response packet */
    Xcp_InsertResponseCTO(&ResponseCTO);
  }

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_PROCESSSUBCMDGETSLAVEIDONCAN_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

#endif

/*----------------------[Xcp_ProcessSubCmdGetSlaveIdOnEthernet]-----------------------*/

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
/* !LINKSTO Xcp.ASAM.Ethernet.Udp.GETSLAVEID001,1 */
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdGetSlaveIdOnEthernet(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  /* Variable to hold the local IPv4 address of the Rx socket */
  TcpIp_SockAddrInetType LocalAddress;

  DBG_XCP_PROCESSSUBCMDGETSLAVEIDONETHERNET_ENTRY();

  if (Xcp_SoAdGetLocalAddress(&LocalAddress) != E_OK)
  {
    /* !LINKSTO Xcp.ASAM.Ethernet.Udp.GETSLAVEID009,1 */
    /* SoAd_GetLocalAddr function failed, but connection is IPv4 */
    LocalAddress.port = 0U;
    LocalAddress.addr[0] = 0U;
  }

  if(XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE20_INDEX) != XCP_GET_SLAVE_ID_ON_ETH_IP_V4)
  {
    /* !LINKSTO Xcp.Ethernet.Udp.GETSLAVEID.CMDOnlyIPv4,1 */
    /* GET_SLAVE_ID is only supported on IPv4 */
    /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
  }
  else
  {
    /* Local variable to store STATUS byte */
    uint8 Status;
    /* Port, Intel byte order */
    /* !LINKSTO Xcp.ASAM.Ethernet.Udp.GETSLAVEID011,1 */
    uint16 ResponsePort = (uint16) \
        ( ((uint16)(XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX)) ) |
          ((uint16)(XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE3_INDEX)) << 8U) );

    /* IPv4 Address, Network byte order */
    uint32 ResponseAddress = (uint32) \
        ( ((uint32)(XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX)) << 24U) |
          ((uint32)(XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE5_INDEX)) << 16U) |
          ((uint32)(XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE6_INDEX)) << 8U) |
          ((uint32)(XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE7_INDEX)) ) );

    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    /* !LINKSTO Xcp.ASAM.Ethernet.Udp.GETSLAVEID007,1 */
    Xcp_SetMulticastAddress(&ResponsePort, &ResponseAddress);

    /* Exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    /* !LINKSTO Xcp.ASAM.Ethernet.Udp.GETSLAVEID002,1 */
    /* Positive response */
    ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;

    /* Local slave address, Network byte order */
    ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = (uint8)(LocalAddress.addr[0] >> 24U);
    ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = (uint8)(LocalAddress.addr[0] >> 16U);
    ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = (uint8)(LocalAddress.addr[0] >> 8U);
    ResponseCTO.CTO[XCP_RES_BYTE4_INDEX] = (uint8)(LocalAddress.addr[0]);

    /* Mark the remaining bytes to reserved */
    TS_MemSet(&ResponseCTO.CTO[XCP_RES_BYTE5_INDEX], XCP_RESERVED_BYTE, 12U);
    /* !LINKSTO Xcp.ASAM.Ethernet.Udp.GETSLAVEID011,1 */
    /* Local slave port, Intel byte order */
    ResponseCTO.CTO[XCP_RES_BYTE17_INDEX] = (uint8)(LocalAddress.port);
    ResponseCTO.CTO[XCP_RES_BYTE18_INDEX] = (uint8)(LocalAddress.port >> 8U);

    /* Get the current status */
    Status = 0U;
    /* !LINKSTO Xcp.ASAM.Ethernet.Udp.GETSLAVEID003,1 */
    /* TRANSPORT_PROTOCOL*/
#if (XCP_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON)
    /* support both TCP and UDP */
    XCP_SET_FLAG(XCP_GET_SLAVE_ID_IP_TRANSPORT_PROTOCOL_II_FLAG, Status, uint8);
#else
    /* support only UDP */
    XCP_SET_FLAG(XCP_GET_SLAVE_ID_IP_TRANSPORT_PROTOCOL_I_FLAG, Status, uint8);
#endif

    /* IP_VERSION, only IPv4 is supported, nothing to set */

    /* SLV_AVAILABILITY */
    if(Xcp_ConnectionStatus.State == XCP_STATE_CONNECTED)
    {
      XCP_SET_FLAG(XCP_GET_SLAVE_ID_SLV_AVAILABILITY_FLAG, Status, uint8);
    }

    /* Fill in STATUS byte */
    ResponseCTO.CTO[XCP_RES_BYTE19_INDEX] = Status;

    /* RESOURCE */
    ResponseCTO.CTO[XCP_RES_BYTE20_INDEX] = XCP_RESOURCE_SUPPORTED;

    /* Set LENGTH(DWORD) to 0 */
    TS_MemBZero(&ResponseCTO.CTO[XCP_RES_BYTE21_INDEX], 4U);

    ResponseCTO.Length = XCP_SUBCMD_GET_SLAVE_ID_ON_ETH_RES_LENGTH;
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertMulticastResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_PROCESSSUBCMDGETSLAVEIDONETHERNET_EXIT(XCP_E_OK);

  return XCP_E_OK;
}

#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */

#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) || \
     (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))

/*----------------------[Xcp_ProcessSubCmdGetDaqID]-----------------------*/

#if (STD_ON == XCP_GET_DAQ_ID_SUPPORTED)

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdGetDaqID(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  /* Local variable to hold CAN ID */
  uint32 CanId;
  /* Retrieve requested DAQ List number */
  const uint16 DaqListNumber =
      XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));

  DBG_XCP_PROCESSSUBCMDGETDAQID_ENTRY();

  /* If DAQ List number is invalid */
  if (DaqListNumber >= XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(uint16))
  {
    /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
  }
  else
  {
    /* Get the DAQ list based on the DAQ list number */
    P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqListNumber);

    /* If DAQ lists are configured at least on of the resources are available (DAQ and/or STIM)*/

    /* If requested DAQ list's direction is STIM  */
    if (XCP_MASK_DAQLIST_DIRECTION_STIM ==
       (DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION))
    {
#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
    /* store STIM can Id */
      CanId = Xcp_RxLPduIdConf[Xcp_ConnectionCfg[
              Xcp_ConnectionStatus.ConnectionId].StimDtoPduId].PduCanId;
      ResponseCTO.CTO[XCP_RES_PID_INDEX]   = XCP_RES_PID;
      ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_CAN_ID_FIXED;
      ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = XCP_RESERVED_BYTE;
      ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = XCP_RESERVED_BYTE;
      ResponseCTO.CTO[XCP_RES_BYTE4_INDEX] = (uint8)CanId;
      ResponseCTO.CTO[XCP_RES_BYTE5_INDEX] = (uint8)(CanId >> 8U);
      ResponseCTO.CTO[XCP_RES_BYTE6_INDEX] = (uint8)(CanId >> 16U);
      ResponseCTO.CTO[XCP_RES_BYTE7_INDEX] = (uint8)(CanId >> 24U);
      ResponseCTO.Length = XCP_SUBCMD_GET_DAQ_ID_RES_LENGTH;
#endif
    }
    /* Requested DAQ list's direction is DAQ */
    else
    {
#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
    /* store DAQ can Id */
      CanId = Xcp_TxLPduIdConf[Xcp_ConnectionCfg[
              Xcp_ConnectionStatus.ConnectionId].DaqDtoPduId].PduCanId;
      ResponseCTO.CTO[XCP_RES_PID_INDEX]   = XCP_RES_PID;
      ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_CAN_ID_FIXED;
      ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = XCP_RESERVED_BYTE;
      ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = XCP_RESERVED_BYTE;
      ResponseCTO.CTO[XCP_RES_BYTE4_INDEX] = (uint8)CanId;
      ResponseCTO.CTO[XCP_RES_BYTE5_INDEX] = (uint8)(CanId >> 8U);
      ResponseCTO.CTO[XCP_RES_BYTE6_INDEX] = (uint8)(CanId >> 16U);
      ResponseCTO.CTO[XCP_RES_BYTE7_INDEX] = (uint8)(CanId >> 24U);
      ResponseCTO.Length = XCP_SUBCMD_GET_DAQ_ID_RES_LENGTH;
#endif
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_PROCESSSUBCMDGETDAQID_EXIT(XCP_OK);
  return XCP_OK;
}

#endif /* (STD_ON == XCP_GET_DAQ_ID_SUPPORTED) */

#if (XCP_DAQ_COUNT > 0U)

/*--------------------[Xcp_ProcessCmdFreeDAQ]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdFreeDAQ(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDFREEDAQ_ENTRY();
  /* !LINKSTO Xcp.ASAM.PROTECTION.INVALID.RESOURCEACCESS,1 */
  /* If both DAQ and STIM resources are protected */
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* Daq and STIM is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
    /* !LINKSTO Xcp.FREE_DAQ.BusyWhenEventMFIsProcessing,1 */
    /* Check that there is not an event main function currently running that also has
       DAQ lists associated running */
    if (Xcp_IsAtLeastOneEventMainFunctionProcessing())
    {
      /* There is currently an ongoing Event Main Function processing,
       command must be retried in order to not mess the ongoing event processing */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_BUSY, XCP_ERR_CMD_BUSY_LENGTH);
    }
    else
#endif /* (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0) */
    {
      /* Local variable to hold Dynamic DAQ List  */
      Xcp_DaqIdType DAQListNumber;

      /* Enter the critical section as the DAQ list can be read during Xcp_SetEvent(), if
       * associated to the event */
      SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

      /* Clear dynamically allocated DAQ Lists */
      for ( DAQListNumber = XCP_MIN_DAQ;
            DAQListNumber < XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(Xcp_DaqIdType);
            DAQListNumber++
          )
      {
        /* Get the DAQ list based on the DAQ list number */
        P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(DAQListNumber);

        /* If any event is associated with the DAQList, this needs to be removed */
        if (DaqListPtr->EventId < XCP_MAX_EVENT_CHANNEL)
        {
          Xcp_RemoveDAQListFromEventChannel(DAQListNumber, DaqListPtr->EventId);
        }
      }

      /* Exit the critical section */
      SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

      /* Clear the dynamic info block */
      Xcp_ClearDynamicInfoBlock();

      /* Update last serviced dynamic DAQ request */
      Xcp_DynamicInfo.LastDynamicDAQReq = XCP_CMD_FREE_DAQ_PID;

      /* Reset the entire dynamic memory area to 0 */
      TS_MemBZero(Xcp_DaqLists.Xcp_DynamicArea, XCP_DYNAMIC_AREA_SIZE);

      /* Prepare CTO with positive response */
      ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
      ResponseCTO.Length = XCP_CMD_FREE_DAQ_RES_LENGTH;
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDFREEDAQ_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdAllocDAQ]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdAllocDAQ(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDALLOCDAQ_ENTRY();
  /* !LINKSTO Xcp.ASAM.PROTECTION.INVALID.RESOURCEACCESS,1 */
  /* If both DAQ and STIM resources are protected */
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* Both Daq and STIM is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Local variable to hold number of available DAQ Lists to be configured */
    uint16 NumberOfConfigurableDaqLists;

    /* Get requested number of DAQ Lists */
    const uint16 RequestedNoOfDAQs =
        XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));

    /* How many bytes are being consumed already by dynamically allocated DAQ lists (via a
     * previous ALLOC_DAQ command) */
    uint32 SpaceConsumedByDaqLists =  Xcp_DynamicInfo.NoOfConfiguredDynamicDAQLists * sizeof(Xcp_DaqType);

    /* Identify number of available DAQ Lists to be configured in the pre-allocated area */
    NumberOfConfigurableDaqLists =
        (uint16)((XCP_DYNAMIC_AREA_SIZE - SpaceConsumedByDaqLists)/(sizeof(Xcp_DaqType)));

    /* Check if the calling sequence is correct: ALLOC_DAQ must be called after a FREE_DAQ and
     * before any ALLOC_ODT and ALLOC_ODT_ENTRY to be called */
    if (Xcp_DynamicInfo.LastDynamicDAQReq < XCP_CMD_ALLOC_DAQ_PID)
    {
      /* Sequence is not correct, send error */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEQUENCE, XCP_ERR_SEQUENCE_LENGTH);
    }
    /* Verify whether the requested number of DAQ Lists is valid and within range */
    else if ((RequestedNoOfDAQs == 0U) ||
             (RequestedNoOfDAQs > (XCP_DAQ_COUNT - (uint16)Xcp_DynamicInfo.NoOfConfiguredDynamicDAQLists))
            )

    {
      /* Invalid number of DAQ Lists  */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
    /* Verify whether there is enough space available to allocate requested number of DAQ Lists */
    else if (RequestedNoOfDAQs > NumberOfConfigurableDaqLists)
    {
      /* Reset the entire configuration */
      Xcp_ClearDynamicInfoBlock();

      /* Not enough space available to allocate new DAQ lists - send error */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_MEMORY_OVERFLOW, XCP_ERR_MEMORY_OVERFLOW_LENGTH);
    }
    else
    {
      /* Iterator */
      Xcp_DaqIdType i;
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)
      /* FIRST_PID */
      uint8 FirstPid = 0U;
#if (XCP_MIN_DAQ > 0)
      /* Get the FIRST_PID of the last predefined DAQ list */
      FirstPid = Xcp_DaqListsDefault.Xcp_Daq[XCP_MIN_DAQ-1U].FirstPID +
                 Xcp_DaqListsDefault.Xcp_Daq[XCP_MIN_DAQ-1U].MaxOdt;
#endif
#endif /* (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK) */

      /* Save the number of dynamically configured DAQ Lists. Cast is safe due to previous condition
       * that checks that the requested DAQ number is within range. */
      Xcp_DynamicInfo.NoOfConfiguredDynamicDAQLists += (Xcp_DaqIdType)RequestedNoOfDAQs;

      /* For all dynamically allocated DAQ lists */
      for (i=XCP_MIN_DAQ; i< (XCP_MIN_DAQ + Xcp_DynamicInfo.NoOfConfiguredDynamicDAQLists); i++)
      {
        /* Get the DAQ list */
        P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(i);

        /* Set the associated event channel as invalid */
        DaqListPtr->EventId = XCP_INVALID_EVENT_CHANNEL;

#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)
        /* Set a dummy FIRST_PID for the dynamic DAQ lists. This value should be overwritten by
         * follow up ALLOC_ODT commands.
         * The cast to uint8 is safe because the VSMD checks make sure
         * the right-hand number cannot be greater than 255 */
        DaqListPtr->FirstPID = (uint8)(FirstPid + ((i-XCP_MIN_DAQ) * XCP_ODT_COUNT));
#endif /* (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK) */
      }

      /* Update last serviced dynamic DAQ request */
      Xcp_DynamicInfo.LastDynamicDAQReq = XCP_CMD_ALLOC_DAQ_PID;
      /* Prepare CTO with positive response */
      ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
      ResponseCTO.Length = XCP_CMD_ALLOC_DAQ_RES_LENGTH;
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDALLOCDAQ_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdAllocODT]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdAllocODT(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDALLOCODT_ENTRY();

  /* This is needed to prevent MISRA violations when the only DAQ lists allowed are predefined ones */
#if (XCP_MIN_DAQ == XCP_MAX_DAQ)
  /* There is no available dynamic DAQ list to be configured */
  XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
#else

  /* Verify protection status */
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* Both Daq and STIM is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Calculate next DAQ List to be allocated */
    const uint16 DAQListTobeAllocated = XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(uint16);
    /* Retrieve the requested DAQ List number */
    const uint16 RequestedDAQNumber =
        XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));
    /* Retrieve the requested number of ODTs */
    const uint8 RequestedNoOfODTs = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX);
    /* Get the DAQ list based on the DAQ list number */

    /* ALLOC_ODT command must be called only after an ALLOC_DAQ or ALLOC_ODT command */
    if ((Xcp_DynamicInfo.LastDynamicDAQReq != XCP_CMD_ALLOC_DAQ_PID) &&
        (Xcp_DynamicInfo.LastDynamicDAQReq != XCP_CMD_ALLOC_ODT_PID)
       )
    {
      /* Violated dynamic allocation sequence:
         ALLOC_ODT shall not follow a FREE_DAQ or an ALLOC_ODT_ENTRY command */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEQUENCE, XCP_ERR_SEQUENCE_LENGTH);
    }
    /* Check whether the requested DAQ list is available:
       Check whether the DAQ List is a dynamic DAQ list and is already allocated.
       Check whether number of ODTs requested is valid and within range */
    else if ((RequestedDAQNumber >= XCP_MAX_DAQ) ||
             (RequestedNoOfODTs == 0U))
    {
      /* The DAQ List is not allocated or it is not dynamic */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
    else if (RequestedDAQNumber >= DAQListTobeAllocated )
    {
      /* The DAQ List is not allocated or is not dynamic or the number of ODTs
       * requested is invalid */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
#if (XCP_MIN_DAQ != 0)
    else if (RequestedDAQNumber < XCP_MIN_DAQ )
    {
      /* The DAQ List is not allocated or is not dynamic or the number of ODTs
       * requested is invalid */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
#endif
    else /* DAQ List number is valid */
    {
      P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) RequestedDaqListPtr =
        XCP_GET_DAQ_LIST_PTR(RequestedDAQNumber);
      if(RequestedNoOfODTs > (XCP_ODT_COUNT - RequestedDaqListPtr->MaxOdt))
      {
        /* The number of requested ODTs is invalid */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
      }
      /* Do not allow allocation to an already configured (using SET_DAQ_LIST_MODE) DAQ List */
      else if (XCP_IS_DAQLIST_FULLY_CONFIGURED(RequestedDaqListPtr) == TRUE)
      {
        /* DAQ List is not available for further allocation of associated ODTS */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_RES_TEMP_NOT_A, XCP_ERR_RES_TEMP_NOT_A_LENGTH);
      }
      else
      {
        /* Local variable to hold number of available free space for ODT */
        uint16 FreeODTSpace;

        /* How many bytes are being consumed by the DAQ lists */
        uint32 SpaceConsumedByDaqLists =  Xcp_DynamicInfo.NoOfConfiguredDynamicDAQLists * sizeof(Xcp_DaqType);

        /* Identify available free space in the allocated memory area */
        FreeODTSpace = (uint16)(((XCP_DYNAMIC_AREA_SIZE - SpaceConsumedByDaqLists)/
                                 ((uint32)sizeof(Xcp_OdtType))
                                ) -
                                Xcp_DynamicInfo.AllocatedOdtsSoFar
                               );

        /* Check whether there is enough space available to allocate the
         * requested ODTs */
        if (RequestedNoOfODTs > FreeODTSpace)
        {
          /* Reset the entire configuration */
          Xcp_ClearDynamicInfoBlock();

          /* Not enough space available to allocate new ODTs */
          XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_MEMORY_OVERFLOW, XCP_ERR_MEMORY_OVERFLOW_LENGTH);
        }
        else
        {
          /* Address of the very first DAQ list - the beginning of the dynamic area */
          /* Deviation MISRAC2012-2 <START> */
          P2VAR(Xcp_DaqType, TYPEDEF, XCP_VAR) AddressOfTheFirstDynamicDaq =
          (Xcp_DaqType *)(&(Xcp_DaqLists.Xcp_DynamicArea[0U]));


          /* If this is the first ALLOC_ODT command, we must set the address of the first ODT */
          if (Xcp_DynamicInfo.LastDynamicDAQReq == XCP_CMD_ALLOC_DAQ_PID)
          {
            /* Save the address of the very first ODT */
            Xcp_DynamicInfo.FirstDynamicOdtPtr =
            (Xcp_OdtType *)(&AddressOfTheFirstDynamicDaq[Xcp_DynamicInfo.NoOfConfiguredDynamicDAQLists]);
          }
          /* Deviation MISRAC2012-2 <STOP> */
          /* Allocate ODTs in the space set aside for ODTs associated with the specified DAQ */
          RequestedDaqListPtr->MaxOdt += RequestedNoOfODTs;

          /* Update how many ODTs have been allocated so far */
          Xcp_DynamicInfo.AllocatedOdtsSoFar+=RequestedNoOfODTs;

          /* Update last serviced dynamic DAQ request */
          Xcp_DynamicInfo.LastDynamicDAQReq = XCP_CMD_ALLOC_ODT_PID;
          /* Prepare CTO with positive response */
          ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
          ResponseCTO.Length = XCP_CMD_ALLOC_DAQ_RES_LENGTH;
        }
      }
    }
  }
#endif /* (XCP_MIN_DAQ == XCP_MAX_DAQ) */

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_PROCESSCMDALLOCODT_EXIT(XCP_E_OK);

  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdAllocODTEntry]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdAllocODTEntry(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDALLOCODTENTRY_ENTRY();

  /* This is needed to prevent MISRA violations when the only DAQ lists allowed are predefined ones */
#if (XCP_MIN_DAQ == XCP_MAX_DAQ)
  /* There is no available dynamic DAQ list to be configured */
  ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_ERR_PID;
  ResponseCTO.CTO[XCP_RES_ERRCODE_INDEX] = XCP_ERR_OUT_OF_RANGE;
  ResponseCTO.Length = XCP_ERR_OUT_OF_RANGE_LENGTH;
#else

  /* Verify protection status */
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* Both Daq and STIM is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  /* ALLOC_ODT_ENTRY command must be called only after an ALLOC_ODT or ALLOC_ODT_ENTRY command */
  else if ((Xcp_DynamicInfo.LastDynamicDAQReq != XCP_CMD_ALLOC_ODT_PID) &&
      (Xcp_DynamicInfo.LastDynamicDAQReq != XCP_CMD_ALLOC_ODT_ENTRY_PID)
     )
  {
    /* Violated dynamic allocation sequence:
       ALLOC_ODT_ENTRY shall not follow a FREE_DAQ or an ALLOC_DAQ command */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEQUENCE, XCP_ERR_SEQUENCE_LENGTH);
  }
  else
  {
    /* Calculate next DAQ List to be allocated */
    const uint16 DAQListTobeAllocated = XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(uint16);
    /* Retrieve the requested DAQ List number */
    const uint16 RequestedDAQNumber =
        XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));

    /* Check whether the requested DAQ list is available */
#if (XCP_MIN_DAQ != 0)
    if ((RequestedDAQNumber < XCP_MIN_DAQ) ||
        ((RequestedDAQNumber >= XCP_MAX_DAQ) ||
         (RequestedDAQNumber >= DAQListTobeAllocated)))
#else
    if ((RequestedDAQNumber >= XCP_MAX_DAQ) ||
        (RequestedDAQNumber >= DAQListTobeAllocated ))
#endif
    {
      /* The DAQ List is not allocated or is not dynamic */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
    else
    {
      /* Retrieve the requested ODT number */
      const uint8 RequestedODTNumber = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX);
      /* Get the DAQ list based on the number */
      P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) RequestedDaqListPtr =
        XCP_GET_DAQ_LIST_PTR(RequestedDAQNumber);

      /* If this is the first ALLOC_ODT_ENTRY command we must arrange all allocated ODTs in the
       * dynamic memory area */
      if (Xcp_DynamicInfo.LastDynamicDAQReq == XCP_CMD_ALLOC_ODT_PID)
      {
        /* We need to re-assign the start position of where the first ODT is for each DAQ list */
        Xcp_ArrangeDynamicODTs();
      }

      /* Check that requested ODT number is valid */
      if (RequestedODTNumber >= RequestedDaqListPtr->MaxOdt)
      {
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
      }
      else
      {
        /* Retrieve the requested number of ODT Entries */
        const uint8 ReqNoOfODTEntries = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE5_INDEX);

        /* Get the number of ODT entries which can be allocated in the ODT based on the ASAM limits
         * (ODT_ENTRY_COUNT) */
        uint8 OdtEntriesThatCanBeAllocated =
            Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Entries_Per_Odt_Dyn - RequestedDaqListPtr->OdtList[RequestedODTNumber].NrOfOdtEntries;

#if (XCP_TIMESTAMP_SUPPORTED == XCP_TIMESTAMP_MASK)
        if (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Timestamp_Enabled == TRUE)
        {
          /* If timestamp is enabled and the requested ODT is zero calculate the free ODT entry space
          * taking into consideration the timestamp limitation */
          if( RequestedODTNumber == 0U )
          {
            OdtEntriesThatCanBeAllocated =
                Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Entries_Per_Odt_Dyn_TS - RequestedDaqListPtr->OdtList[RequestedODTNumber].NrOfOdtEntries;
          }
        }
#endif
        /* Check whether number of ODT Entries requested is valid or the requested ODT is valid */
        if ((ReqNoOfODTEntries == 0U) ||
            (ReqNoOfODTEntries > OdtEntriesThatCanBeAllocated)
           )
        {
          /* The DAQ List is not allocated or is not dynamic */
          XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
        }
        /* Do not allow allocation to an already configured (using SET_DAQ_LIST_MODE) DAQ List */
        else if (XCP_IS_DAQLIST_FULLY_CONFIGURED(RequestedDaqListPtr) == TRUE)
        {
          /* DAQ List is not available for further allocation of associated ODTs */
          XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_RES_TEMP_NOT_A, XCP_ERR_RES_TEMP_NOT_A_LENGTH);
        }
        else
        {
          /* Local variable to hold number of available free space for ODT Entries */
          uint16 FreeODTEntrySpace;
          /* Local variable to hold available free space in dynamic mirror */
          Xcp_MirrorSizeType FreeDynamicMirrorSpace;
          /* Size required in Dynamic Mirror for Odt Entries */
          Xcp_OdtSizeType OdtDataSize;

          /* How many bytes are being consumed by DAQ lists and ODTs */
          uint32 SpaceConsumedByDaqListsAndOdts =
              ((uint32)Xcp_DynamicInfo.NoOfConfiguredDynamicDAQLists * (uint32)sizeof(Xcp_DaqType)) +
              (Xcp_DynamicInfo.AllocatedOdtsSoFar * (uint32)sizeof(Xcp_OdtType));

          /* Identify available free space for ODT entries in the allocated memory */
          FreeODTEntrySpace = (uint16)(((XCP_DYNAMIC_AREA_SIZE - SpaceConsumedByDaqListsAndOdts)
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
                                        /(sizeof(Xcp_OdtEntryType))
#else
                                        / XCP_ODT_ENTRY_USED_SPACE
#endif
                                       ) -
                                       Xcp_DynamicInfo.AllocatedOdtEntriesSoFar
                                      );
          /* Identify available free space for ODT entries in dynamic mirror */
          FreeDynamicMirrorSpace = (Xcp_MirrorSizeType)
            (XCP_DYNAMIC_MIRROR_SIZE - Xcp_DTOEntryMirror.DynamicMirrorReservedBytes);
          /* Size required in Dynamic Mirror for Odt Entries */
          OdtDataSize = ReqNoOfODTEntries * (Xcp_OdtSizeType)XCP_MAX_ODTENTRY_SIZE;

          /* Check whether there is enough space available to allocate the requested ODT Entries */
          if ((ReqNoOfODTEntries > FreeODTEntrySpace) ||
              (OdtDataSize > FreeDynamicMirrorSpace))
          {
            /* Reset the entire configuration */
            Xcp_ClearDynamicInfoBlock();

            /* Not enough space available to allocate new ODT entries - send error */
            XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_MEMORY_OVERFLOW, XCP_ERR_MEMORY_OVERFLOW_LENGTH);
          }
          else
          {
            /* If this is the first ALLOC_ODT_ENTRY command, we must set the address of the first ODT
             * entry */
            if (Xcp_DynamicInfo.LastDynamicDAQReq == XCP_CMD_ALLOC_ODT_PID)
            {
              /* Save the address of the absolute first ODT Entry */
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
              /* Deviation MISRAC2012-2 <START> */
              Xcp_DynamicInfo.FirstDynamicOdtEntryPtr =
                  (Xcp_OdtEntryType *)(&(Xcp_DynamicInfo.FirstDynamicOdtPtr[Xcp_DynamicInfo.AllocatedOdtsSoFar]));
#else
              Xcp_DynamicInfo.FirstDynamicOdtEntryAddr =
                  (usize *)(&(Xcp_DynamicInfo.FirstDynamicOdtPtr[Xcp_DynamicInfo.AllocatedOdtsSoFar]));
#endif
              /* Deviation MISRAC2012-2 <STOP> */
            }
            /* Initialize with 0. It will be updated later when daq is sampled */
            RequestedDaqListPtr->OdtList[RequestedODTNumber].DTODataSize = 0U;
            /* Store the position in mirror where will be located data from this ODT  */
            RequestedDaqListPtr->OdtList[RequestedODTNumber].MirrorOffsetPos =
                Xcp_DTOEntryMirror.DynamicMirrorReservedBytes;
            /* Update number of bytes reserved in dynamic mirror */
            Xcp_DTOEntryMirror.DynamicMirrorReservedBytes += OdtDataSize;

            /* Allocate ODTEntries in the space set aside for ODT Entries
             * associated with the specified ODT */
            RequestedDaqListPtr->OdtList[RequestedODTNumber].NrOfOdtEntries += ReqNoOfODTEntries;

            /* Update how many ODT entries have been allocated so far */
            Xcp_DynamicInfo.AllocatedOdtEntriesSoFar+=ReqNoOfODTEntries;

            /* Update last serviced dynamic DAQ request */
            Xcp_DynamicInfo.LastDynamicDAQReq = XCP_CMD_ALLOC_ODT_ENTRY_PID;
            /* Prepare CTO with positive response */
            ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
            ResponseCTO.Length = XCP_CMD_ALLOC_DAQ_RES_LENGTH;
          }
        }
      }
    }
  }
#endif /* (XCP_MIN_DAQ == XCP_MAX_DAQ) */

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDALLOCODTENTRY_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

STATIC FUNC(void, XCP_CODE) Xcp_ClearDynamicInfoBlock(void)
{
  DBG_XCP_CLEARDYNAMICINFOBLOCK_ENTRY();

  /* Clear number of dynamically allocated DAQ lists */
  Xcp_DynamicInfo.NoOfConfiguredDynamicDAQLists = 0U;
  /* Clear number of allocated ODTs */
  Xcp_DynamicInfo.AllocatedOdtsSoFar = 0U;
  /* Clear number of allocated ODT entries */
  Xcp_DynamicInfo.AllocatedOdtEntriesSoFar = 0U;
  /* Reset the address of the first dynamic ODT */
  Xcp_DynamicInfo.FirstDynamicOdtPtr = NULL_PTR;
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
  /* Reset the address of the first dynamic ODT Entry */
  Xcp_DynamicInfo.FirstDynamicOdtEntryPtr = NULL_PTR;
#else
  Xcp_DynamicInfo.FirstDynamicOdtEntryAddr = NULL_PTR;
#endif
  /* Reset the last dynamic configuration command */
  Xcp_DynamicInfo.LastDynamicDAQReq = XCP_CMD_FREE_DAQ_NOT_RECEIVED;
  /* Clear number of reserved bytes in Dynamic ODT Mirror */
  Xcp_DTOEntryMirror.DynamicMirrorReservedBytes = 0U;

  DBG_XCP_CLEARDYNAMICINFOBLOCK_EXIT();
}

#if (XCP_MIN_DAQ != XCP_MAX_DAQ) /* this condition is put to avoid compiler warnings when there are
no dynamic DAQ lists configurable as, then, this function would not be called by anybody */
STATIC FUNC(void, XCP_CODE) Xcp_ArrangeDynamicODTs(void)
{
  /* Iterator through dynamic DAQ lists */
  Xcp_DaqIdType DaqListNumber;

  /* The index in the dynamic area where the first ODT for the required DAQ list resides */
  uint32 IndexOfTheFirstOdtInDAQ = 0U;

  for (DaqListNumber=XCP_MIN_DAQ;
         DaqListNumber < XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(Xcp_DaqIdType);
         DaqListNumber++)
  {
    /* Get the DAQ list */
    P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DynamicDaqListPtr =
      XCP_GET_DAQ_LIST_PTR(DaqListNumber);

    if (DynamicDaqListPtr->MaxOdt > 0U)
    {
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)
      /* Variable to calculate FIRST_PID */
      uint8 FirstPid = 0U;
#endif
      /* Assign the address of the first ODT */
      DynamicDaqListPtr->OdtList = &(Xcp_DynamicInfo.FirstDynamicOdtPtr[IndexOfTheFirstOdtInDAQ]);
      /* Move the position to the next first ODT */
      IndexOfTheFirstOdtInDAQ += DynamicDaqListPtr->MaxOdt;

/* We must re-calculate the FIRST_PID */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)
      if (DaqListNumber > 0U)
      {
        CONSTP2CONST(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) PreviousDaqListPtr =
            XCP_GET_DAQ_LIST_PTR(DaqListNumber - 1U);
        FirstPid = PreviousDaqListPtr->FirstPID + PreviousDaqListPtr->MaxOdt;
      }
#if (XCP_MIN_DAQ > 0)
      else
      {
        /* Get the FIRST_PID of the last predefined DAQ list */
        FirstPid = Xcp_DaqListsDefault.Xcp_Daq[XCP_MIN_DAQ-1U].FirstPID +
                   Xcp_DaqListsDefault.Xcp_Daq[XCP_MIN_DAQ-1U].MaxOdt;
      }
#endif
      /* Set the FIRST_PID of the containing DAQ list */
      DynamicDaqListPtr->FirstPID = FirstPid;
#endif /* (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK) */
    }
  }
}
#endif /* (XCP_MIN_DAQ != XCP_MAX_DAQ) */

STATIC FUNC(void, XCP_CODE) Xcp_ArrangeDynamicODTEntries(void)
{
  /* Iterator through dynamic DAQ lists */
  Xcp_DaqIdType DaqListNumber;
  /* Iterator through dynamic ODTs */
  uint8 ODTNumber;
  /* The index in the dynamic area where the first ODT entry for the required ODT is */
  uint32 IndexOfTheFirstOdtEntryInOdt = 0U;

  for (DaqListNumber=XCP_MIN_DAQ;
         DaqListNumber < XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(Xcp_DaqIdType);
         DaqListNumber++)
  {
    /* Get the DAQ list */
    P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DynamicDaqListPtr =
      XCP_GET_DAQ_LIST_PTR(DaqListNumber);

    for (ODTNumber=0U; ODTNumber < DynamicDaqListPtr->MaxOdt; ODTNumber++)
    {
      /* Get the ODT */
      P2VAR(Xcp_OdtType, TYPEDEF, XCP_VAR) DynamicOdt = &DynamicDaqListPtr->OdtList[ODTNumber];

      if (DynamicOdt->NrOfOdtEntries > 0U)
      {
        /* Set the pointer to the first ODT Entry in the ODT */
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
        DynamicOdt->OdtEntry = &(Xcp_DynamicInfo.FirstDynamicOdtEntryPtr[IndexOfTheFirstOdtEntryInOdt]);
#else
        DynamicOdt->OdtEntry = &(Xcp_DynamicInfo.FirstDynamicOdtEntryAddr[IndexOfTheFirstOdtEntryInOdt]);
#endif
        /* Move the position to the next first ODT */
        IndexOfTheFirstOdtEntryInOdt += DynamicOdt->NrOfOdtEntries;
      }
    }
  }
}

#endif /* (XCP_DAQ_COUNT > 0U) */

/*--------------------[Xcp_ProcessCmdSetDAQPtr]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdSetDAQPtr(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDSETDAQPTR_ENTRY();
  /* !LINKSTO Xcp.ASAM.PROTECTION.INVALID.RESOURCEACCESS,1 */
  /* Verify protection status for both DAQ and STIM*/
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* BOTH Daq and STIM is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Retrieve the requested DAQ List number */
    const uint16 RequestedDAQListNumber =
        XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));

    /* Verify requested DAQ List */
    if (RequestedDAQListNumber >= XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(uint16))
    {
      /* The requested DAQ List is invalid */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
    else
    {
      /* Local variable to hold requested ODT number */
      uint8 RequestedODTNumber;
      /* Get the DAQ list based on the DAQ list number */
      P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) RequestedDaqList =
        XCP_GET_DAQ_LIST_PTR(RequestedDAQListNumber);

      /* Retrieve the requested ODT number */
      RequestedODTNumber = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX);

#if (XCP_DAQ_COUNT > 0U)
      /* We need to re-assign the start position of where the first ODT entry is for each ODT,
       * if the last command was ALLOC_ODT_ENTRY */
      if (Xcp_DynamicInfo.LastDynamicDAQReq == XCP_CMD_ALLOC_ODT_ENTRY_PID)
      {
        Xcp_ArrangeDynamicODTEntries();
        /* Update last serviced dynamic DAQ request */
        Xcp_DynamicInfo.LastDynamicDAQReq = XCP_CMD_SET_DAQ_PTR_PID;
      }
#endif

      /* Verify requested ODT */
      if (RequestedDaqList->MaxOdt <= RequestedODTNumber)
      {
        /* The requested ODT is invalid */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
      }
      else
      {
        /* Retrieve the requested ODT Entry number */
        uint8 RequestedODTEntryNumber = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE5_INDEX);

        /* Verify requested ODT Entry */
        if (RequestedDaqList->OdtList[RequestedODTNumber].NrOfOdtEntries <= RequestedODTEntryNumber)
        {
          /* The requested ODT Entry is invalid */
          XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
        }
        else
        {
          /* Set DAQ pointer */
          Xcp_DAQPointer.DAQList = RequestedDAQListNumber;
          Xcp_DAQPointer.ODT = RequestedODTNumber;
          Xcp_DAQPointer.ODTEntry = RequestedODTEntryNumber;
          /* Prepare CTO with positive response */
          ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
          ResponseCTO.Length = XCP_CMD_SET_DAQ_PTR_RES_LENGTH;
        }
      }
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDSETDAQPTR_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

#if (XCP_WRITE_DAQ_MULTIPLE_SUPPORT == STD_ON)
/*--------------------[Xcp_ProcessCmdWriteDAQMultiple]-----------------------*/

/* !LINKSTO Xcp.ASAM.WRITEDAQMULTIPLE001,1 */
STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdWriteDAQMultiple(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  boolean ConfigurationIsValid = FALSE;
  /* Save the ODT Entry pointer to rollback in case of failure*/
  uint8 SavedOdtEntryPointer = Xcp_DAQPointer.ODTEntry;

  if (Xcp_WriteDAQ_AreConditionsCorrect(&ResponseCTO) == TRUE)
  {
    uint8 i;
    /* Retrieve number of elements */
    const uint8 NoOfDAQElements = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);

    /* Each element needs 8 bytes so check if the no of requested elements have enough data */
    /* !LINKSTO Xcp.ASAM.WRITEDAQMULTIPLE002,1 */
    if (NoOfDAQElements == 0U)
    {
      /* There must be at least one element to write */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
    else
    {
      /* Get the DAQ list based on the information provided by previous SET_DAQ_PTR command */
      P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) PointedDaqListPtr =
        XCP_GET_DAQ_LIST_PTR(Xcp_DAQPointer.DAQList);

      /* Check if the requested number of elements is within range */
      /* !LINKSTO Xcp.ASAM.WRITEDAQMULTIPLE004,1 */
      /* !LINKSTO Xcp.ASAM.WRITEDAQMULTIPLE.ErrorCheck003,1 */
      if ((NoOfDAQElements + Xcp_DAQPointer.ODTEntry) > PointedDaqListPtr->OdtList[Xcp_DAQPointer.ODT].NrOfOdtEntries)
      {
        /* There must be at least one element to write */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
      }
      else
      {
        for (i=1U; i<=NoOfDAQElements; i++)
        {
          /* Retrieve requested size of DAQ Element */
          const uint8 ReqDAQElementSize =
              XCP_READ_CMDPACKET_BYTE(((i - 1U)*XCP_WRITE_DAQ_MULTIPLE_BYTES_PER_ELEMENT)+XCP_CMD_BYTE3_INDEX);

          /* Retrieve address of element.*/
          const uint8 ByteIndex = ((i - 1U)*XCP_WRITE_DAQ_MULTIPLE_BYTES_PER_ELEMENT)+XCP_CMD_BYTE4_INDEX;
          const uint32 ReqDAQElementAddress = XCP_UINT32_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(ByteIndex));
          const uint8 BitPosition =
                XCP_READ_CMDPACKET_BYTE(((i - 1U)*XCP_WRITE_DAQ_MULTIPLE_BYTES_PER_ELEMENT)+XCP_CMD_BYTE2_INDEX);

          if (Xcp_WriteDAQ_WriteOneODT(&ResponseCTO, ReqDAQElementSize, ReqDAQElementAddress,BitPosition) == FALSE)
          {
            ConfigurationIsValid = FALSE;
            break;
          }
          else
          {
            ConfigurationIsValid = TRUE;
          }
        }
      }
    }
  }

  /* !LINKSTO Xcp.ASAM.WRITEDAQMULTIPLE005,1 */
  if (ConfigurationIsValid == FALSE)
  {
    /* We need to set the pointer back to the original ODT Entry */
    Xcp_DAQPointer.ODTEntry = SavedOdtEntryPointer;
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  return XCP_E_OK;
}
#endif /* XCP_WRITE_DAQ_MULTIPLE_SUPPORT == STD_ON */

/*--------------------[Xcp_ProcessCmdWriteDAQ]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdWriteDAQ(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  ResponseCTO.Length = 0U;

  DBG_XCP_PROCESSCMDWRITEDAQ_ENTRY();

  if (Xcp_WriteDAQ_AreConditionsCorrect(&ResponseCTO) == TRUE)
  {
   /* Retrieve address of element.*/
    const uint32 ReqDAQElementAddress =
          XCP_UINT32_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX));
    /* Retrieve requested size of DAQ Element */
    const uint8 ReqDAQElementSize = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX);
    /* Retrieve requested Bit Position  */
    const uint8 BitPosition = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);

    (void)Xcp_WriteDAQ_WriteOneODT(&ResponseCTO, ReqDAQElementSize, ReqDAQElementAddress,BitPosition);
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDWRITEDAQ_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------[Xcp_WriteDAQ_AreConditionsCorrect]-----------------------*/
STATIC FUNC(boolean, XCP_CODE)  Xcp_WriteDAQ_AreConditionsCorrect
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA)  ResponseCTO
)
{
  boolean retValue = FALSE;
  /* !LINKSTO Xcp.ASAM.PROTECTION.INVALID.RESOURCEACCESS,1 */
  /* Verify protection status */
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* Daq is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Check whether the DAQ pointer points to a valid DAQ */
    if (Xcp_DAQPointer.DAQList >= XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(uint16))
    {
      /* Memory location is not accessible / invalid */
      XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
    }
    else
    {
#if ((XCP_MIN_DAQ == XCP_NO_OF_NON_DYNAMIC_DAQ_LISTS) && (XCP_DAQ_COUNT == 0U))
      /* we have only predefined daq lists configured, so, the DAQ List is not writable */
      XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_WRITE_PROTECTED, XCP_ERR_WRITE_PROTECTED_LENGTH);
#else /* we have static/dynamic daq lists configured:
       * ((XCP_MIN_DAQ < XCP_NO_OF_NON_DYNAMIC_DAQ_LISTS) || (XCP_DAQ_COUNT > 0U)) */
#if (XCP_MIN_DAQ != 0)
      /* Check whether the DAQ List is writable */
      /* !LINKSTO Xcp.ASAM.WRITEDAQMULTIPLE.ErrorCheck001,1 */
      if (Xcp_DAQPointer.DAQList < XCP_MIN_DAQ)
      {
        /* DAQ List is not writable */
        XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_WRITE_PROTECTED, XCP_ERR_WRITE_PROTECTED_LENGTH);
      }
      else
#endif /* (XCP_MIN_DAQ != 0) */
      {
        /* Get the DAQ list based on the information provided by previous SET_DAQ_PTR command */
        P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) PointedDaqListPtr =
          XCP_GET_DAQ_LIST_PTR(Xcp_DAQPointer.DAQList);
        /* CHECK: NOPARSE */
        if (PointedDaqListPtr->MaxOdt <= Xcp_DAQPointer.ODT)
        {
          /* This condition cannot be reached because of configuration restriction.
          MaxOdt associated to the current set DaqList is always greater than the ODT
          of Xcp_DAQPointer */
          /* Check whether the DAQ pointer points to a valid ODT. This check is
          performed previously in Xcp_ProcessCmdSetDAQPtr. If the DAQList points
          to an invalid ODT a negative error shall be enqueued */
          /* Memory location is not accessible / invalid */
          XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
        }
        /* CHECK: PARSE */
        /* Check whether the DAQ pointer points to a valid ODTEntry */
        else if (PointedDaqListPtr->OdtList[Xcp_DAQPointer.ODT].NrOfOdtEntries <= Xcp_DAQPointer.ODTEntry)
        {
          /* Memory location is not accessible / invalid */
          XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
        }
        /* If the DAQ already started */
        /* !LINKSTO Xcp.ASAM.WRITEDAQMULTIPLE.ErrorCheck002,1 */
        else if (XCP_IS_DAQ_LIST_RUNNING(PointedDaqListPtr) == TRUE)
        {
          /* Prepare error CTO with negative response ERR_DAQ_ACTIVE */
          XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_DAQ_ACTIVE, XCP_ERR_DAQ_ACTIVE_LENGTH);
        }
        else
        {
          retValue = TRUE;
        }
      }
#endif /* ((XCP_MIN_DAQ == XCP_NO_OF_NON_DYNAMIC_DAQ_LISTS) && (XCP_DAQ_COUNT == 0U)) */
    }
  }
  return retValue;
}

/*--------------------[Xcp_WriteDAQ_WriteOneODT]-----------------------*/
STATIC FUNC(boolean, XCP_CODE)  Xcp_WriteDAQ_WriteOneODT
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA)  ResponseCTO,
  uint8 ReqDAQElementSize,
  uint32 ReqDAQElementAddress,
  uint8 BitPosition
)
{
  boolean RetValue = FALSE;
  /* Get the DAQ list based on the information provided by previous SET_DAQ_PTR command */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) PointedDaqListPtr =
    XCP_GET_DAQ_LIST_PTR(Xcp_DAQPointer.DAQList);
  /* Check whether the the bit position (BIT_OFFSET) is valid */
  /* !LINKSTO Xcp.WRITEDAQ.InvalidLength003,1 */
#if ((XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON) && (XCP_ENABLE_BIT_OFFSET == STD_OFF))
  /* !LINKSTO Xcp.WriteDaq.BitOffset.RamOptimizations,1 */
  if(BitPosition != XCP_BIT_OFFSET_IGNORE_VALUE)
#elif   (XCP_ADDRESS_GRANULARITY_SIZE == XCP_BYTE_AG_SIZE)
  if((BitPosition != XCP_BIT_OFFSET_IGNORE_VALUE) &&
     (BitPosition > XCP_BIT_OFFSET_MAX_VALUE_AG_BYTE))
#elif (XCP_ADDRESS_GRANULARITY_SIZE == XCP_WORD_AG_SIZE)
  /* CHECK: NOPARSE */
  if((BitPosition != XCP_BIT_OFFSET_IGNORE_VALUE) &&
     (BitPosition > XCP_BIT_OFFSET_MAX_VALUE_AG_WORD))
  /* The current implementation does not support ADDRESS_GRANULARITY > BYTE */
  /* CHECK: PARSE */
#elif (XCP_ADDRESS_GRANULARITY_SIZE == XCP_DWORD_AG_SIZE)
  /* CHECK: NOPARSE */
  if((BitPosition != XCP_BIT_OFFSET_IGNORE_VALUE) &&
     (BitPosition > XCP_BIT_OFFSET_MAX_VALUE_AG_DWORD))
  /* The current implementation does not support ADDRESS_GRANULARITY > BYTE */
  /* CHECK: PARSE */
#endif
  {
    /* BIT_OFFSET value is invalid */
    XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
  }
  else if ( /* !LINKSTO Xcp.ASAM.BITWISE_STIMULATION.007,1 */
            (BitPosition != XCP_BIT_OFFSET_IGNORE_VALUE) &&
            (((PointedDaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION) == XCP_MASK_DAQLIST_DIRECTION_DAQ) &&
              (ReqDAQElementSize != XCP_GRANULARITY_ODT_ENTRY_SIZE_DAQ)
            )
          )
  {
    /* !LINKSTO Xcp.ASAM.WRITEDAQMULTIPLE.ErrorCheck005,1 */
    XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
  }
  else if( /* !LINKSTO Xcp.ASAM.BITWISE_STIMULATION.007,1 */
           (BitPosition != XCP_BIT_OFFSET_IGNORE_VALUE) &&
           /* Length must match GRANULARITY_ODT_ENTRY_SIZE_x  */
           (((PointedDaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION) == XCP_MASK_DAQLIST_DIRECTION_STIM) &&
            (ReqDAQElementSize != XCP_GRANULARITY_ODT_ENTRY_SIZE_STIM)
           )
        )
  {
    /* !LINKSTO Xcp.ASAM.WRITEDAQMULTIPLE.ErrorCheck005,1 */
    XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
  }
  else
  {
#if (XCP_DAQ_SUPPORTED == STD_ON)
    uint8 maxOdtEntrySizeDaq = Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_OdtEntry_Size_Daq;
#else
    uint8 maxOdtEntrySizeDaq = 0;
#endif
#if (XCP_STIM_SUPPORTED == STD_ON)
    uint8 maxOdtEntrySizeStim = Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_OdtEntry_Size_Stim;
#else
    uint8 maxOdtEntrySizeStim = 0;
#endif
    uint8 maxOdtEntrySizeDaqStim = maxOdtEntrySizeDaq;
    if (maxOdtEntrySizeDaqStim < maxOdtEntrySizeStim)
    {
      maxOdtEntrySizeDaqStim = maxOdtEntrySizeStim;
    }
    /* !LINKSTO Xcp.ASAM.WRITEDAQMULTIPLE.ErrorCheck004,1 */
    if (ReqDAQElementSize > maxOdtEntrySizeDaqStim )
    {
      /* Invalid ODT entry length */
      XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
    else
    {
      /* Local variable to hold ODT size */
      uint16 ODTSize = 0U;
      /* Local variable to hold number of ODT entries in the ODT */
      uint8 NoOfODTEntries;
      /* Local variable to hold header size of Xcp message */
      uint8 HeaderSize;
      /* Local variable to hold ODT pointer */
      P2VAR(Xcp_OdtType, AUTOMATIC, XCP_APPL_DATA) OdtPtr;

      /* Get ODT pointer */
      OdtPtr = &(PointedDaqListPtr->OdtList[Xcp_DAQPointer.ODT]);
      /* Get number of entries in the target ODT */
      NoOfODTEntries = OdtPtr->NrOfOdtEntries;
      /* Calculate size of elements represented by the ODT */
      while (NoOfODTEntries > 0U)
      {
        NoOfODTEntries--;
        /* Get the ODT entry size in AG */
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
        ODTSize += OdtPtr->OdtEntry[NoOfODTEntries].Length;
#else
        ODTSize += *(uint8*)Xcp_GetOdtEntryData(OdtPtr->OdtEntry,NoOfODTEntries,XCP_ODT_ENTRY_FIELD_LENGTH);
#endif
      }
      /* Calculate resultant size of ODT after update with the requested
       * data */
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
      ODTSize -= OdtPtr->OdtEntry[Xcp_DAQPointer.ODTEntry].Length;
#else
      ODTSize -= *(uint8*)Xcp_GetOdtEntryData(OdtPtr->OdtEntry,Xcp_DAQPointer.ODTEntry,XCP_ODT_ENTRY_FIELD_LENGTH);
#endif
      ODTSize += ReqDAQElementSize;

      /* Calculate the header size (Length of Identifier field and Timestamp field) */
      /* The time stamp field will be present only for the first ODT of the DAQ list */
#if (XCP_TIMESTAMP_SUPPORTED == XCP_TIMESTAMP_MASK)
      if (Xcp_DAQPointer.ODT == 0U && (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Timestamp_Enabled == TRUE))
      {
        HeaderSize = XCP_PID_LENGTH + XCP_TS_LENGTH;
      }
      else
#endif
      {
        HeaderSize = XCP_PID_LENGTH;
      }

      /* Check whether the resultant size is within limits */
      if ((Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Dto) < (ODTSize + HeaderSize))
      {
        /* Resultant size of ODT is invalid or size of ODT entry is invalid */
        XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
      }
      else
      {
        /* Write ODT entry */

#if (XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON)
#if ((XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF) || (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON))
        uint8 accessType = XCP_MEMORY_AREA_TYPE_READ;
        if (XCP_MASK_DAQLIST_DIRECTION_STIM == (PointedDaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION))
        {
          accessType = XCP_MEMORY_AREA_TYPE_WRITE;
        }
        /* Deviation MISRAC2012-3 <START> */
        if (Xcp_IsMemoryAreaAccessible
               (
                   XCP_APPL_GET_ADDRESS(0U, (P2VAR(uint8, XCP_VAR, XCP_APPL_DATA))ReqDAQElementAddress),
                   ReqDAQElementSize,
                   accessType,
                   XCP_MEMORY_AREA_SCOPE_DAQ_STIM
               ) == FALSE
           )
        /* Deviation MISRAC2012-3 <STOP> */
        {
          /* !LINKSTO Xcp.ProtectedMemoryLocations.NegativeResponse.Commands,1 */
          /* Prepare error CTO with negative response ERR_ACCESS_DENIED */
          XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
        }
        else
#endif /* (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON) || (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON) */
#endif /* XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON */
        {
          /* Write ODT entry BitOfsset */
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
          OdtPtr->OdtEntry[Xcp_DAQPointer.ODTEntry].BitOffset = BitPosition;
#elif (XCP_ENABLE_BIT_OFFSET == STD_ON)
          *(uint8*)Xcp_GetOdtEntryData(OdtPtr->OdtEntry,Xcp_DAQPointer.ODTEntry,XCP_ODT_ENTRY_FIELD_BITOFFSET) = BitPosition;
#endif
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
          /* Deviation MISRAC2012-3 <START> */
          OdtPtr->OdtEntry[Xcp_DAQPointer.ODTEntry].Address = (P2VAR(uint8, XCP_VAR, XCP_APPL_DATA)) ReqDAQElementAddress;
          /* Deviation MISRAC2012-3 <STOP> */
          OdtPtr->OdtEntry[Xcp_DAQPointer.ODTEntry].Length = ReqDAQElementSize;
#else
          *(uint32*)Xcp_GetOdtEntryData(OdtPtr->OdtEntry,Xcp_DAQPointer.ODTEntry,XCP_ODT_ENTRY_FIELD_ADDRESS) = ReqDAQElementAddress;
          *(uint8*)Xcp_GetOdtEntryData(OdtPtr->OdtEntry,Xcp_DAQPointer.ODTEntry,XCP_ODT_ENTRY_FIELD_LENGTH) = ReqDAQElementSize;
#endif

          /* Increment DAQ pointer to next ODT entry */
          Xcp_DAQPointer.ODTEntry++;
          /* Prepare CTO with positive response */
          ResponseCTO->CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
          ResponseCTO->Length = XCP_CMD_WRITE_DAQ_RES_LENGTH;

          RetValue = TRUE;
        }
      }
    }
  }
  return RetValue;
}


#endif /*((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) ||
        * (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))*/

#if (XCP_SEED_AND_KEY_ENABLED == STD_ON)

/*--------------------------[Xcp_ProcessCmdGetSeed]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetSeed(void)
{
  /* Local variable to hold requested resource type */
  uint8 ResourceIdentifier;
  /* Local variable to hold requested mode */
  uint8 Mode;
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  /* Local variable to hold generated seed length */
  uint8 SeedLength = Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto - 1;
  /* Local variable to hold Seed request return value */
  Std_ReturnType errorCode = E_NOT_OK;

  DBG_XCP_PROCESSCMDGETSEED_ENTRY();

  /* Retrieve requested mode */
  Mode = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);
  /* Retrieve requested Resource Type */
  ResourceIdentifier = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX);

  /* Verify requested resource type
     Present implementation doesn't support Mode = 1 */
  if (Mode != XCP_SEC_ACCESS_GETSEED_ZERO)
  {
    /* Requested mode is invalid or
       Requested resource type is out of range */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
  }
  else if((ResourceIdentifier != (uint8)XCP_RESOURCE_PGM_MASK)  &&
         (ResourceIdentifier != (uint8)XCP_RESOURCE_STIM_MASK) &&
         (ResourceIdentifier != (uint8)XCP_RESOURCE_DAQ_MASK)  &&
         (ResourceIdentifier != (uint8)XCP_RESOURCE_CAL_PAG_MASK))
  {
    /* Requested mode is invalid or
       Requested resource type is out of range */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
  }
  /* Verify whether the request follows the sequence rules.
     1) There shall be no previous request and the current request shall be XCP_SEC_ACCESS_GETSEED_ONE
     2) The previous request shall be XCP_SEC_ACCESS_GETSEED_ZERO and the current request
        shall be XCP_SEC_ACCESS_GETSEED_ONE
     3) The previous request shall be XCP_SEC_ACCESS_GETSEED_ONE and the current request
        shall be XCP_SEC_ACCESS_GETSEED_ONE
     4) The previous request shall be XCP_SEC_ACCESS_UNLOCK and the current request shall be
        XCP_SEC_ACCESS_GETSEED_ZERO
   */
  else if ((Mode != XCP_SEC_ACCESS_GETSEED_ONE) &&
         ((XCP_SEC_ACCESS_GETSEED_ZERO == Xcp_SecAccess.PrevType) ||
          (XCP_SEC_ACCESS_GETSEED_ONE == Xcp_SecAccess.PrevType)))
  {
    /* Requested sequence is invalid
     * Previous request was GET_SEED mode zero(0)
     * Previous request was GET_SEED mode One(1) */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEQUENCE, XCP_ERR_SEQUENCE_LENGTH);
  }
  /* CHECK: NOPARSE */
  else if ((Mode != XCP_SEC_ACCESS_GETSEED_ZERO) &&
           (Xcp_SecAccess.PrevType > XCP_SEC_ACCESS_GETSEED_ONE))
  {
    /* Implementation does not support Mode != XCP_SEC_ACCESS_GETSEED_ZERO.
     * Xcp_SecAccess.PrevType is loaded with the value of Mode. Further on,
     * its value will never be equal to XCP_SEC_ACCESS_GETSEED_ONE */

    /* Requested sequence is invalid
     * There was no previous request or previous request was UNLOCK*/
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEQUENCE, XCP_ERR_SEQUENCE_LENGTH);
  }
  /* CHECK: PARSE */
  else
  {
    /* Local variable to hold protection status of the requested resource */
    uint8  ProtectionStatus =  (uint8)(Xcp_Status.ProtectionStatus & ResourceIdentifier);
    /* Check whether the resource is already unlocked */
    if (XCP_RESOURCE_UNLOCKED == ProtectionStatus)
    {
      /* Resource is already unlocked.
         Send response with seed length information = zero(0) */
      ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
      /* Set seed length information */
      ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_RESOURCE_UNLOCKED;
      /* Set response length */
      ResponseCTO.Length = XCP_CMD_GETSEED_RES_LENGTH_MIN;
      /* Set security request status */
      Xcp_SecAccess.PrevType = (Xcp_SecAccessType_t)Mode;
      Xcp_SecAccess.Resource = ResourceIdentifier;
    }
    else
    {
      /* Calculate seed */
      errorCode = Xcp_ApplGetSeed(ResourceIdentifier, &(ResponseCTO.CTO[XCP_RES_BYTE2_INDEX]), &SeedLength);
      /* Check the validity of seed
       * Current implementation doesn't support Seed length greater than MAX_CTO - 2
       */
      if ((E_OK != errorCode) ||
         (SeedLength > Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto)
        )
      {
        /* !LINKSTO Xcp.GetSEED.ErrOutOfRange.Response,1 */
        /* Algorithm failed or generated seed cannot be sent in one CTO */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
      }
      else
      {
        /* Set positive response ID */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        /* Set seed length */
        ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = SeedLength;
        /* Set length of the response with seed */
        ResponseCTO.Length = SeedLength + XCP_CMD_GETSEED_RES_LENGTH_MIN;
        /* Set security request status */
        Xcp_SecAccess.PrevType = (Xcp_SecAccessType_t)Mode;
        Xcp_SecAccess.Resource = ResourceIdentifier;
      }
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETSEED_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------------[Xcp_ProcessCmdUnlock]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdUnlock(void)
{
  /* Local variable to hold key length information */
  uint8 KeyLength;
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDUNLOCK_ENTRY();

  /* Retrieve key length information */
  KeyLength = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);

  /* Verify whether the request follows the sequence rules.
     UNLOCK request shall be accepted only if there is a prior GET_SEED request. */
  if (XCP_SEC_ACCESS_GETSEED_ZERO != Xcp_SecAccess.PrevType)
  {
    /* Requested sequence is invalid */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEQUENCE, XCP_ERR_SEQUENCE_LENGTH);
  }
  else
  {
    /* Local variable to hold compare key request return value*/
    Std_ReturnType errorCode;

    /* Compare key */
    errorCode = Xcp_ApplCompareKey(Xcp_SecAccess.Resource,
                                     &(XCP_READ_CMDPACKET_BYTE(XCP_RES_BYTE2_INDEX)),
                                     KeyLength
                                    );
    /* Check the validity of key */
    if (E_OK != errorCode)
    {
      /* Algorithm failed */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);

      /* Enter critical section */
      SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

      /* Flush queues:
       * 1. Command Queue is of size one(1) and thus there won't be any new commands to flush
       * 2. Flush XCP Event CTO Queue
       * 3. Reset DAQ event channels runtime information
       * 4. Flush DTO queue
       * 5. Do not flush CTO response queue as we need to send response to this command
       */
/* If event packet transmission enabled */
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
      /* Flush XCP events CTOs */
      Xcp_FlushEventCTOQueue();
#endif

/* If DAQ or STIM resource is supported */
#if (XCP_MAX_DAQ != 0U)

      /* Reset event channels runtime information */
      Xcp_ResetEventChannels();

      /* Flush DTO Queue */
      Xcp_FlushDTORunTimeData();

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
      Xcp_ResetRequestData();
#endif
#endif /* (XCP_MAX_DAQ != 0U) */

      /* Go to disconnected state */
      Xcp_ConnectionStatus.State = XCP_STATE_DISCONNECTED;

      /* Exit critical section.*/
      SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    }
    else
    {
      /* Unlock the resource */
      Xcp_Status.ProtectionStatus &= (uint8)(0xFFU ^ Xcp_SecAccess.Resource);
      /* Prepare CTO with positive response */
      ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
      ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_GET_RESOURCE_PROTECTION_STATUS();
      ResponseCTO.Length = XCP_CMD_UNLOCK_RES_LENGTH;
      /* Set security request status */
      Xcp_SecAccess.PrevType = XCP_SEC_ACCESS_UNLOCK;
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDUNLOCK_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

#endif /* (XCP_SEED_AND_KEY_ENABLED == STD_ON) */

#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) || \
     (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))

/*--------------------------[Xcp_ProcessCmdGetDAQClock]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE)  Xcp_ProcessCmdGetDAQClock(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDGETDAQCLOCK_ENTRY();
  /* !LINKSTO Xcp.ASAM.PROTECTION.INVALID.RESOURCEACCESS,1 */
  /* If both DAQ and STIM resources are protected */
  if(((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_DAQ_MASK) ==
              (uint8)XCP_RESOURCE_DAQ_MASK) &&
     ((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_STIM_MASK) ==
              (uint8)XCP_RESOURCE_STIM_MASK))
  {
    /* Daq is protected return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
#if (XCP_TIMESTAMP_SUPPORTED == XCP_TIMESTAMP_MASK)
  else if (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Timestamp_Enabled == TRUE)
  {

/* Check if time stamp is supported */
#if (XCP_USER_TIMESTAMP_SUPPORTED == STD_OFF)
      /* Local variable to hold the counter value */
    TickType CounterValue;
#endif /* (XCP_USER_TIMESTAMP_SUPPORTED == STD_OFF) */

    /* Local variable to hold the timestamp */
    Xcp_TimestampType TimeStamp = 0U;

    /* Prepare CTO with positive response */
    ResponseCTO.CTO[XCP_RES_PID_INDEX]   = XCP_RES_PID;
    ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_RESERVED_BYTE;
    ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = XCP_RESERVED_BYTE;
    ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = XCP_RESERVED_BYTE;

#if (XCP_USER_TIMESTAMP_SUPPORTED == STD_OFF)
    /* Retrieve present time stamp from the OS counter */
    (void) GetCounterValue(XCP_OS_COUNTER, &CounterValue);

    /* Convert the CounterVAlue to the Xcp_TimestampType and copy the value to the TimpeStamp variable */
    TimeStamp = (Xcp_TimestampType)CounterValue;
#else
    /* Get the Timestamp value from the user provided function */
    TimeStamp = Xcp_ApplGetTimestamp();
#endif /* (XCP_USER_TIMESTAMP_SUPPORTED == STD_OFF) */

    /* Copy time stamp to response packet */
    Xcp_WriteDwordToByteArray( &(ResponseCTO.CTO[XCP_RES_BYTE4_INDEX]), TimeStamp );
    /* Set the response length */
    ResponseCTO.Length = XCP_CMD_GET_DAQ_CLOCK_RES_LENGTH;
  }
#endif
  else
  {
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETDAQCLOCK_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

#endif /* (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) ||
           (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)   */

#if (XCP_RESOURCE_CAL_PAG == XCP_RESOURCE_CAL_PAG_MASK)
/*--------------------[Xcp_ProcessCmdCopyCalPage]------------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdCopyCalPage(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDCOPYCALPAGE_ENTRY();

  /* Check if the Calibration/Paging Resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_CAL_PAG_MASK) ==
                 XCP_RESOURCE_CAL_PAG_MASK)
  {
    /* Calibration commands are protected; return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Callout function return value */
    Xcp_ApplReturnType CalloutRes;
    /* Local variable to hold the Source Segment */
    const uint8 SrcSegment = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);
    /* Local variable to hold the Source Page */
    const uint8 SrcPage = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX);
    /* Local variable to hold the Destination Segment */
    const uint8 DestSegment = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE3_INDEX);
    /* Local variable to hold the Destination Page */
    const uint8 DestPage = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX);

    /* Call Xcp_ApplGetPagProcessorInfo() callout function */
    CalloutRes = Xcp_ApplCopyCalPage( SrcSegment, SrcPage, DestSegment, DestPage );

    /* Check the callout function return */
    switch( CalloutRes )
    {
      case XCP_APPL_OK:
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        ResponseCTO.Length = XCP_CMD_COPY_CAL_PAGE_RES_LENGTH;
        break;
      case XCP_APPL_ERR_WRITE_PROTECTED:
        /* Prepare error CTO with negative response ERR_WRITE_PROTECTED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_WRITE_PROTECTED, XCP_ERR_WRITE_PROTECTED_LENGTH);
        break;
      case XCP_APPL_ERR_PAGE_NOT_VALID:
        /* Prepare error CTO with negative response ERR_PAGE_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_PAGE_NOT_VALID, XCP_ERR_PAGE_NOT_VALID_LENGTH);
        break;
      case XCP_APPL_ERR_SEGMENT_NOT_VALID:
        /* Prepare error CTO with negative response ERR_SEGMENT_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEGMENT_NOT_VALID, XCP_ERR_SEGMENT_NOT_VALID_LENGTH);
        break;
      default:
        /* EB specific handling: */
        /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
        break;
    }

  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDCOPYCALPAGE_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdGetPagProcInfo]----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetPagProcInfo(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  /* Callout function return value */
  Xcp_ApplReturnType CalloutRes;
  /* Total number of available segments */
  uint8 MaxSegment = 0U;
  /* General properties for paging (Freeze Supported) */
  uint8 PagProperties = 0U;

  DBG_XCP_PROCESSCMDGETPAGPROCINFO_ENTRY();

  /* Check if the Calibration/Paging Resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_CAL_PAG_MASK) ==
                 XCP_RESOURCE_CAL_PAG_MASK)
  {
    /* Calibration commands are protected; return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Call Xcp_ApplGetPagProcessorInfo() callout function */
    CalloutRes = Xcp_ApplGetPagProcessorInfo( &MaxSegment, &PagProperties );

    /* Check the callout function return */
    switch( CalloutRes )
    {
      case XCP_APPL_OK:
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        ResponseCTO.CTO[1U] = MaxSegment; /* maximum number of segments */
        ResponseCTO.CTO[2U] = PagProperties; /* freeze mode support */
        ResponseCTO.Length = XCP_CMD_GET_PAG_PROC_INFO_RES_LENGTH;
        break;
      default:
        /* EB specific handling: */
        /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
        break;
    } /* switch( CalloutRes ) */
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETPAGPROCINFO_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdGetCalPage]-------------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetCalPage(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDGETCALPAGE_ENTRY();

  /* Check if the Calibration/Paging Resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_CAL_PAG_MASK) ==
                 XCP_RESOURCE_CAL_PAG_MASK)
  {
    /* Calibration commands are protected; return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Callout function return value */
    Xcp_ApplReturnType CalloutRes;
    /* Local variable where the Page number returned by the callout function is stored */
    uint8 Page = 0U;
    /* GET_CAL_PAGE command: Mode parameter */
    const uint8 Mode = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);
    /* Segment number */
    const uint8 Segment = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX);

    /* Call Xcp_ApplGetCalPage() callout function */
    CalloutRes = Xcp_ApplGetCalPage( Segment, Mode, &Page );

    switch( CalloutRes )
    {
      case XCP_APPL_OK:
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        ResponseCTO.CTO[1U] = XCP_RESERVED_BYTE;
        ResponseCTO.CTO[2U] = XCP_RESERVED_BYTE;
        ResponseCTO.CTO[3U] = Page;
        ResponseCTO.Length = XCP_CMD_GET_CAL_PAGE_RES_LENGTH;
        break;
      case XCP_APPL_ERR_MODE_NOT_VALID:
        /* Prepare error CTO with negative response ERR_MODE_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_MODE_NOT_VALID, XCP_ERR_MODE_NOT_VALID_LENGTH);
        break;
      case XCP_APPL_ERR_SEGMENT_NOT_VALID:
        /* Prepare error CTO with negative response ERR_SEGMENT_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEGMENT_NOT_VALID, XCP_ERR_SEGMENT_NOT_VALID_LENGTH);
        break;
      case XCP_APPL_ERR_PAGE_NOT_VALID:
        /* Prepare error CTO with negative response ERR_PAGE_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_PAGE_NOT_VALID, XCP_ERR_PAGE_NOT_VALID_LENGTH);
        break;
      default:
        /* EB specific handling: */
        /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
        break;
    } /* switch( CalloutRes ) */
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETCALPAGE_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdSetCalPage]-------------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdSetCalPage(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDSETCALPAGE_ENTRY();

  /* Check if the Calibration/Paging Resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_CAL_PAG_MASK) ==
                 XCP_RESOURCE_CAL_PAG_MASK)
  {
    /* Calibration commands are protected; return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Callout function return value */
    Xcp_ApplReturnType CalloutRes;
    /* SET_CAL_PAGE command: Mode parameter */
    const uint8 Mode = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);
    /* Segment Number */
    const uint8 Segment = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX);
    /* Page Number */
    const uint8 Page = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE3_INDEX);

    /* Call Xcp_ApplSetCalPage() callout function */
    CalloutRes = Xcp_ApplSetCalPage( Segment, Page, Mode );

    switch( CalloutRes )
    {
      case XCP_APPL_OK:
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        ResponseCTO.Length = XCP_CMD_SET_CAL_PAGE_RES_LENGTH;
        break;
      case XCP_APPL_ERR_PAGE_NOT_VALID:
        /* Prepare error CTO with negative response ERR_PAGE_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_PAGE_NOT_VALID, XCP_ERR_PAGE_NOT_VALID_LENGTH);
        break;
      case XCP_APPL_ERR_MODE_NOT_VALID:
        /* Prepare error CTO with negative response ERR_MODE_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_MODE_NOT_VALID, XCP_ERR_MODE_NOT_VALID_LENGTH);
        break;
      case XCP_APPL_ERR_SEGMENT_NOT_VALID:
        /* Prepare error CTO with negative response ERR_SEGMENT_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEGMENT_NOT_VALID, XCP_ERR_SEGMENT_NOT_VALID_LENGTH);
        break;
      default:
        /* EB specific handling: */
        /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
        break;
    } /* switch( CalloutRes ) */
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDSETCALPAGE_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdGetSegmentMode]-------------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetSegmentMode(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDGETSEGMENTMODE_ENTRY();

  /* Check if the Calibration/Paging Resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_CAL_PAG_MASK) ==
                 XCP_RESOURCE_CAL_PAG_MASK)
  {
    /* Calibration commands are protected; return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Callout function return value */
    Xcp_ApplReturnType CalloutRes;
    /* Local variable where the Mode that is returned by the callout function is stored */
    uint8 Mode = 0U;
    /* Segment number */
    const uint8 Segment = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX);

    /* Call Xcp_ApplGetSegmentMode() callout function */
    CalloutRes = Xcp_ApplGetSegmentMode( Segment, &Mode );

    switch( CalloutRes )
    {
      case XCP_APPL_OK:
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        ResponseCTO.CTO[1U] = XCP_RESERVED_BYTE;
        ResponseCTO.CTO[2U] = Mode;
        ResponseCTO.Length = XCP_CMD_GET_SEGMENT_MODE_RES_LENGTH;
        break;
      case XCP_APPL_ERR_OUT_OF_RANGE:
        /* Prepare error CTO with negative response XCP_APPL_ERR_OUT_OF_RANGE */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_APPL_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
        break;
      case XCP_APPL_ERR_SEGMENT_NOT_VALID:
        /* Prepare error CTO with negative response XCP_APPL_ERR_SEGMENT_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_APPL_ERR_SEGMENT_NOT_VALID, XCP_ERR_SEGMENT_NOT_VALID_LENGTH);
        break;
      default:
        /* EB specific handling: */
        /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
        break;
    } /* switch( CalloutRes ) */
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETSEGMENTMODE_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdSetSegmentMode]-------------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdSetSegmentMode(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDSETSEGMENTMODE_ENTRY();

  /* Check if the Calibration/Paging Resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_CAL_PAG_MASK) ==
                 XCP_RESOURCE_CAL_PAG_MASK)
  {
    /* Calibration commands are protected; return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Callout function return value */
    Xcp_ApplReturnType CalloutRes;
    /* SET_SEGMENT_MODE command: Mode parameter */
    const uint8 Mode = ( XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX) & /* Take Bit 0 only */
                         XCP_CMD_SET_GET_SEGMENT_MODE_FREEZE_MASK );
    /* Segment number */
    const uint8 Segment = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX);

    /* Call Xcp_ApplSetSegmentMode() callout function */
    CalloutRes = Xcp_ApplSetSegmentMode( Segment, Mode );

    switch( CalloutRes )
    {
      case XCP_APPL_OK:
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        ResponseCTO.Length = XCP_CMD_SET_SEGMENT_MODE_RES_LENGTH;
        break;
      case XCP_APPL_ERR_OUT_OF_RANGE:
        /* Prepare error CTO with negative response XCP_APPL_ERR_OUT_OF_RANGE */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_APPL_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
        break;
      case XCP_APPL_ERR_SEGMENT_NOT_VALID:
        /* Prepare error CTO with negative response XCP_APPL_ERR_SEGMENT_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_APPL_ERR_SEGMENT_NOT_VALID, XCP_ERR_SEGMENT_NOT_VALID_LENGTH);
        break;
      case XCP_APPL_ERR_MODE_NOT_VALID:
        /* Prepare error CTO with negative response XCP_APPL_ERR_MODE_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_APPL_ERR_MODE_NOT_VALID, XCP_ERR_MODE_NOT_VALID_LENGTH);
        break;
      default:
        /* EB specific handling: */
        /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
        break;
    } /* switch( CalloutRes ) */
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDSETSEGMENTMODE_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdGetSegmentInfo]-------------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetSegmentInfo(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDGETSEGMENTINFO_ENTRY();

  /* Check if the Calibration/Paging Resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_CAL_PAG_MASK) ==
                 XCP_RESOURCE_CAL_PAG_MASK)
  {
    /* Calibration commands are protected; return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Callout function return value */
    Xcp_ApplReturnType CalloutRes;
    /* GET_SEGMENT_INFO: Mode */
    const uint8 Mode = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);
    /* Segment Number */
    const uint8 Segment = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX);
    /* GET_SEGMENT_INFO: Segment Info command parameter */
    const uint8 SegmentInfo = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE3_INDEX);
    /* GET_SEGMENT_INFO: Mapping Index command parameter */
    const uint8 MappingIndex = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX);
    /* Array where the callout function return information is stored */
    uint8 GetSegmentInfoRespData[XCP_GET_SEGMENT_INFO_CMD_RES_LENGTH];

    /* Call Xcp_ApplGetSegmentInfo() callout function */
    CalloutRes = Xcp_ApplGetSegmentInfo( Mode, Segment, SegmentInfo,
                                         MappingIndex, &GetSegmentInfoRespData[0] );

    switch( CalloutRes )
    {
      case XCP_APPL_OK:
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        /* Check Mode parameter */
        if( ( Mode == 0U ) || ( Mode == 2U ) )
        { /* Mode = 0 or Mode = 2 */
          ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = XCP_RESERVED_BYTE; /* Reserved */
          ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = XCP_RESERVED_BYTE; /* Reserved */
          ResponseCTO.CTO[XCP_RES_BYTE3_INDEX] = XCP_RESERVED_BYTE; /* Reserved */
          /* Copy 4-byte source/destination address or length parameter */
          TS_MemCpy( &ResponseCTO.CTO[XCP_RES_BYTE4_INDEX],
                     GetSegmentInfoRespData,
                     XCP_CALPAG_ADDRESS_LENGTH_SIZE );
          ResponseCTO.Length = XCP_CMD_GET_SEGMENT_INFO_RES_LENGTH_0;
        }
        else
        { /* Mode = 1 */
          /* Copy standard info for this segment */
          TS_MemCpy( &ResponseCTO.CTO[XCP_RES_BYTE1_INDEX],
                     GetSegmentInfoRespData,
                     XCP_CALPAG_GETSEGMENTINFO_STANDARD_SIZE );
          ResponseCTO.Length = XCP_CMD_GET_SEGMENT_INFO_RES_LENGTH_1;
        }
        break;
      case XCP_APPL_ERR_MODE_NOT_VALID:
        /* Prepare error CTO with negative response ERR_MODE_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_MODE_NOT_VALID, XCP_ERR_MODE_NOT_VALID_LENGTH);
        break;
      case XCP_APPL_ERR_SEGMENT_NOT_VALID:
        /* Prepare error CTO with negative response ERR_SEGMENT_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEGMENT_NOT_VALID, XCP_ERR_SEGMENT_NOT_VALID_LENGTH);
        break;
      case XCP_APPL_ERR_OUT_OF_RANGE:
        /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
        break;
      default:
        /* EB specific handling: */
        /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
        break;
    } /* switch( CalloutRes ) */
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETSEGMENTINFO_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdGetPageInfo]-------------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetPageInfo(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDGETPAGEINFO_ENTRY();

  /* Check if the Calibration/Paging Resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_CAL_PAG_MASK) ==
                 XCP_RESOURCE_CAL_PAG_MASK)
  {
    /* Calibration commands are protected; return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Callout function return value */
    Xcp_ApplReturnType CalloutRes;
    /* Segment Number */
    const uint8 Segment = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX);
    /* Page Number*/
    const uint8 Page = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE3_INDEX);
    /* Local variable where PageProperties is stored */
    uint8 PageProperties = 0U;
    /* Local variable where the INIT_SEGMENT is stored */
    uint8 InitSegment = 0U;

    /* Call Xcp_ApplGetPageInfo() callout function */
    CalloutRes = Xcp_ApplGetPageInfo( Segment, Page, &PageProperties, &InitSegment );

    switch( CalloutRes )
    {
      case XCP_APPL_OK:
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        ResponseCTO.CTO[1U] = PageProperties;
        ResponseCTO.CTO[2U] = InitSegment;
        ResponseCTO.Length = XCP_CMD_GET_PAGE_INFO_RES_LENGTH;
        break;
      case XCP_APPL_ERR_SEGMENT_NOT_VALID:
        /* Prepare error CTO with negative response ERR_SEGMENT_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEGMENT_NOT_VALID, XCP_ERR_SEGMENT_NOT_VALID_LENGTH);
        break;
      case XCP_APPL_ERR_PAGE_NOT_VALID:
        /* Prepare error CTO with negative response ERR_PAGE_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_PAGE_NOT_VALID, XCP_ERR_PAGE_NOT_VALID_LENGTH);
        break;
      case XCP_APPL_ERR_OUT_OF_RANGE:
        /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
        break;
      default:
        /* EB specific handling: */
        /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
        break;
    } /* switch( CalloutRes ) */
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETPAGEINFO_EXIT(XCP_E_OK);
  return XCP_E_OK;
}

#endif /* (XCP_RESOURCE_CAL_PAG == XCP_RESOURCE_CAL_PAG_MASK) */

#if (XCP_BUILD_CHECKSUM_SUPPORT == STD_ON)
/*--------------------[Xcp_ProcessCmdBuildChecksum]----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdBuildChecksum(void)
{
#if (XCP_ASYNCH_CRC_SUPPORT == STD_ON)
  /* Flag signaling whether this command execution must output a response to the master */
  boolean ProvideResponse = FALSE;
#endif
  /* Local variable to prepare the response */
  Xcp_CTOType ResponseCTO;
  /* Local variable to hold return value */
  Xcp_ErrorType RetVal;
  DBG_XCP_PROCESSCMDBUILDCHECKSUM_ENTRY();

#if (XCP_ASYNCH_CRC_SUPPORT == STD_ON)
  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  /* First step is to check state machine state and prepare the necessary actions */
  /* perform calculation of the busy state machine */
  if (Xcp_CmdProcessorState == XCP_CMDPROCESSOR_BUSY)
  {
    /* It might be that the busy command was aborted in the meantime
    * (SYNCH or another command during BUSY state). If so, we exit the command processing */
    if (XCP_IS_CMD_BUSY_ABORTED() == TRUE)
    {
      /* Processing will not continue */
      RetVal = XCP_E_NOT_OK;
      /* Clear the aborted flag */
      Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_BUSY_ABORTED_MASK);
      /* clear the pending asynchronous build checksum request */
      Xcp_BuildChecksumData.IsRequestCRCActive = FALSE;
      /* no response message is sent towards the Master */
      ProvideResponse = FALSE;
    }
    else if(Xcp_BuildChecksumData.IsRequestCRCActive == TRUE)
    {
      /* The job is not finished so remain in BUSY state until the build checksum job is finished */
      RetVal = XCP_E_BUSY;
      /* no response message is sent towards the Master */
      ProvideResponse = FALSE;
    }
    else
    {
      /* The build checksum job has finished and therefore the XCP state has changed. */
      RetVal = XCP_E_OK;
      /* A positive response will be prepared later on */
      ProvideResponse = TRUE;
    }
  }
  else
#endif /* (XCP_ASYNCH_CRC_SUPPORT == STD_ON) */
  {
    if (( Xcp_MTA.Type == XCP_MTATYPE_COUNTER) || ( Xcp_MTA.IsSet == FALSE))
    {
      /* Prepare error CTO with negative response XCP_ERR_ACCESS_DENIED */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
      RetVal = XCP_E_NOT_OK;

#if(XCP_ASYNCH_CRC_SUPPORT == STD_ON)
      /* prevent setting of a pending asynchronous build checksum request */
      Xcp_BuildChecksumData.IsRequestCRCActive = FALSE;
      /* A negative response is ready to be send towards the Master. */
      ProvideResponse = TRUE;
#endif /* (XCP_ASYNCH_CRC_SUPPORT == STD_ON) */
    }
    else
    {
/* !LINKSTO Xcp.ProtectedMemoryLocations.General,1 */
#if ((XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON) && (XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF))
      if (Xcp_IsMemoryAreaAccessible
             (
                 XCP_APPL_GET_ADDRESS(0U, Xcp_MTA.Address.Write),
                 XCP_UINT32_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX)),
                 XCP_MEMORY_AREA_TYPE_READ,
                 XCP_MEMORY_AREA_SCOPE_CALIBRATION
             ) == FALSE
         )
      {
        /* !LINKSTO Xcp.ProtectedMemoryLocations.NegativeResponse.Commands,1 */
        /* Prepare error CTO with negative response ERR_ACCESS_DENIED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
        RetVal = XCP_E_NOT_OK;
      }
      else
#endif /* XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON */
      {
        /* prepare build checksum input data*/
        Xcp_BuildChecksumData.AddressPtr = XCP_APPL_GET_ADDRESS( 0U, Xcp_MTA.Address.Write );
        Xcp_BuildChecksumData.BlockSize =
            XCP_UINT32_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX));
#if (XCP_ASYNCH_CRC_SUPPORT == STD_ON)
        /* set build checksum request */
        Xcp_BuildChecksumData.IsRequestCRCActive = TRUE;
        /* Job will be triggered and the state machine enters into Busy state until
         * the build checksum job will be finished. */
        RetVal = XCP_E_BUSY;
#else
        /* Build checksum job is synchronously executed within this context and a
         * response, either positive or negative, will be prepared later on. */
        RetVal = XCP_E_OK;
#endif
      }
    }
  }
#if (XCP_ASYNCH_CRC_SUPPORT == STD_ON)
  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
#endif /* (XCP_ASYNCH_CRC_SUPPORT == STD_ON) */

  if( RetVal == XCP_E_OK )
  {
#if (XCP_ASYNCH_CRC_SUPPORT == STD_OFF)
/* calculate the build checksum job in the synchronous context*/
      Xcp_CalculateCRC();
#endif /* (XCP_ASYNCH_CRC_SUPPORT == STD_OFF) */
/* Prepare CTO with response depending on the selected CRC method. */
#if (XCP_BUILD_CHECKSUM_TYPE == XCP_CRC_USER_CALLOUT)
        switch( Xcp_BuildChecksumData.ApplCalloutResult )
        {
          case XCP_APPL_OK:
            /* Prepare CTO with positive response */
            ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
            ResponseCTO.CTO[1U] = Xcp_BuildChecksumData.ChecksumType;
            ResponseCTO.CTO[2U] = XCP_RESERVED_BYTE;
            ResponseCTO.CTO[3U] = XCP_RESERVED_BYTE;
            Xcp_WriteDwordToByteArray( &(ResponseCTO.CTO[4U]), Xcp_BuildChecksumData.ChecksumResult );
            ResponseCTO.Length = XCP_CMD_BUILD_CHECKSUM_RES_LENGTH;
            /* Post-increment MTA according to command specification */
            XCP_INCREMENT_MTA_BY_SIZE_IN_AG( Xcp_BuildChecksumData.BlockSize );
            break;
          case XCP_APPL_ERR_OUT_OF_RANGE:
            /* Prepare error CTO with negative response ERR_OUT_OF_RANGE (special
            format for this command). */
            ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_ERR_PID;
            ResponseCTO.CTO[XCP_RES_ERRCODE_INDEX] = XCP_ERR_OUT_OF_RANGE;
            ResponseCTO.CTO[2U] = XCP_RESERVED_BYTE;
            ResponseCTO.CTO[3U] = XCP_RESERVED_BYTE;
            Xcp_WriteDwordToByteArray( &(ResponseCTO.CTO[4U]), Xcp_BuildChecksumData.ChecksumResult );
            ResponseCTO.Length = 8U;
            RetVal = XCP_E_NOT_OK;
            break;
          case XCP_APPL_ERR_ACCESS_DENIED:
            /* Prepare error CTO with negative response ERR_ACCESS_DENIED */
            XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
            RetVal = XCP_E_NOT_OK;
            break;
          default:
            /* EB specific handling: */
            /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
            XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
            RetVal = XCP_E_NOT_OK;
            break;
        }
#else /* XCP_BUILD_CHECKSUM_TYPE == XCP_CRC_USER_CALLOUT */
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        ResponseCTO.CTO[1U] = Xcp_BuildChecksumData.ChecksumType;
        ResponseCTO.CTO[2U] = XCP_RESERVED_BYTE;
        ResponseCTO.CTO[3U] = XCP_RESERVED_BYTE;
        Xcp_WriteDwordToByteArray( &(ResponseCTO.CTO[4U]), Xcp_BuildChecksumData.ChecksumResult );
        ResponseCTO.Length = XCP_CMD_BUILD_CHECKSUM_RES_LENGTH;
        /* Post-increment MTA according to command specification */
        XCP_INCREMENT_MTA_BY_SIZE_IN_AG( Xcp_BuildChecksumData.BlockSize );
#endif /* (XCP_BUILD_CHECKSUM_TYPE == XCP_CRC_USER_CALLOUT) */
  }
  else
  {
    /* Nothing to do. */
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  /* If the CRC request is not active anymore (either because the MTA is not set properly
     or the CTO is prepared with the response depending on the chosen CRC method) the response
     needs to be queued in the CTO */
#if (XCP_ASYNCH_CRC_SUPPORT == STD_ON)
  if(ProvideResponse == TRUE)
#endif
  {
    /* Enqueue response packet. */
    Xcp_InsertResponseCTO(&ResponseCTO);
  }
  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_PROCESSCMDBUILDCHECKSUM_EXIT(RetVal);
  return RetVal;
}
#endif /*(XCP_BUILD_CHECKSUM_SUPPORT == STD_ON)*/

#if (XCP_MODIFY_BITS_SUPPORTED == STD_ON)
/*-------------------------[Xcp_ProcessCmdModifyBits]------------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdModifyBits(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  /* Local variable to hold request parameter Shift Value (S) */
  const uint8 Shift_Value = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);
#if ((XCP_USE_MEMORY_PROXY == STD_ON) || (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON))
  /* Local variable to hold request parameter AND Mask (MA) */
  const uint16 And_Mask =
      XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));
  /* Local variable to hold request parameter XOR Mask (MX) */
  const uint16 Xor_Mask =
      XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX));
#endif
  Xcp_ErrorType RetValue = XCP_E_OK;
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
   Atomic_value_t localRAMCalloutProxyState = Atomics_Load(&Xcp_RAMCalloutProxyState);
#endif
#if (XCP_USE_MEMORY_PROXY == STD_ON)
  Xcp_MemoryProxyDataType Xcp_MemoryProxyData;
  const Xcp_MemoryAreaType* MemoryArea;
  uint8 MemAreaIdx;
#endif

  DBG_XCP_PROCESSCMDMODIFYBITS_ENTRY();

  ResponseCTO.Length = 0U;

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
  /* !LINKSTO Xcp.BswDistribution.CalibrationCommands.UserDefinedCallouts.ProvideResponse,1 */
  if (localRAMCalloutProxyState == XCP_RESULT_ALREADY_PROCESSED)
  {
    /* positive response */
    ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
    ResponseCTO.Length = XCP_CMD_MODIFY_BITS_RES_LENGTH;
    RetValue = XCP_E_OK;
    Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_NO_USER_CALLOUT_REQUESTED);
    Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_AVAILABLE);
  }
  /* check if we did not sent the request in previous MF cycles and we are
     waiting for a response from MemoryProxy */
  else if (localRAMCalloutProxyState == XCP_NO_USER_CALLOUT_REQUESTED)
#endif
  {
    /* Verify protection status for calibration commands */
    if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_CAL_PAG_MASK) ==
               XCP_RESOURCE_CAL_PAG_MASK)
    {
      /* Calibration commands are protected; return ERR_ACCESS_LOCKED */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
    }
    /* Check whether MTA address is set and MTA address has the correct type */
    else if ( ( Xcp_MTA.IsSet == FALSE) || ( Xcp_MTA.Type == XCP_MTATYPE_COUNTER) )
    {
      /* Prepare error CTO with negative response XCP_ERR_ACCESS_DENIED */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
    }
    /* Verify whether command parameter Shift Value (S) is smaller than 32 */
    else if (Shift_Value > XCP_MODIFY_SHIFT_MAX)
    {
      /* Prepare error CTO with negative response ERR_CMD_OUT_OF_RANGE */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
    else
    {
#if (XCP_USE_MEMORY_PROXY == STD_ON)
      /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.ProtectedMemoryLocations,1 */
      MemoryArea = Xcp_FindMemoryArea(XCP_APPL_GET_ADDRESS(0, Xcp_MTA.Address.Write), XCP_MODIFY_VALUE_SIZE, XCP_MEMORY_AREA_TYPE_READ_WRITE, XCP_MEMORY_AREA_SCOPE_CALIBRATION, &MemAreaIdx);
      if(MemoryArea == NULL_PTR)
      {
        /* !LINKSTO Xcp.BswDistribution.Calibration.MODIFY_BITS.NoCalibrationMemoryAccessArea.Error,1 */
        /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.ProtectedMemoryLocations.NegativeResponse,1 */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
      }
      else if ((uint8)GetApplicationID() != MemoryArea->ApplicationId)
      {
        /* !LINKSTO Xcp.BswDistribution.Calibration.MODIFY_BITS.MemoryProxy.Usage,1 */
        /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyAvailable,1 */
        /* !LINKSTO Xcp.dsn.BSWDistribution.MemProxy.ProtectedAccess,1 */
        if (Atomics_Load(&Xcp_MemoryProxyState) == XCP_MEMORY_PROXY_AVAILABLE)
        {
          Std_ReturnType result;
          const uint8 ApplicationId = (uint8)GetApplicationID();

          Xcp_PrepareDataForRequest(&Xcp_MemoryProxyData, Shift_Value, And_Mask, Xor_Mask, MemAreaIdx);
          result = Xcp_SendMemoryProxyData[ApplicationId][MemoryArea->ApplicationId](&Xcp_MemoryProxyData);
          if ( result == SCHM_E_OK)
          {
            /* !LINKSTO Xcp.BswDistribution.Calibration.MODIFY_BITS.MemoryProxy.Busy,1 */
            RetValue = XCP_E_BUSY;
          }
          else
          {
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
            Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_NO_USER_CALLOUT_REQUESTED);
#endif
            /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.SchMError.MemProxyResetState,1 */
            Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_AVAILABLE);
            /* intentionally no response */
            /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.SchMError,1 */
            XCP_DET_REPORT_RUNTIME_ERROR(XCP_SID_MAIN_FUNCTION, XCP_E_SCHM_ERROR);
          }
        }
        /* check the state again, it might be that the memory proxy task already executed */
        if (Atomics_Load(&Xcp_MemoryProxyState) == XCP_MEMORY_PROXY_READY)
        {
          /* positive response */
          /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyReady.PosResponse,1 */
          ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
          ResponseCTO.Length = XCP_CMD_MODIFY_BITS_RES_LENGTH;
          /* !LINKSTO Xcp.BswDistribution.Calibration.MODIFY_BITS.MemoryProxy.Finished.State,1 */
          /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyReady.ResetState,1 */
          Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_AVAILABLE);
          RetValue = XCP_E_OK;
        }
        else
        {
          /* keep waiting for the memory proxy */
          RetValue = XCP_E_BUSY;
        }
      }
      else
#endif /* XCP_USE_MEMORY_PROXY == STD_ON */
      {
        /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyReady.ContinueProcessing,1 */
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
        Xcp_PrepareDataForRequest(&Xcp_MemoryProxyData, Shift_Value, And_Mask, Xor_Mask, MemAreaIdx);
        Xcp_MemoryProxyHandler(&Xcp_MemoryProxyData);
        /* keep waiting for the memory proxy */
        RetValue = XCP_E_BUSY;
#else
        RetValue = Xcp_ProcessModifyBits(&ResponseCTO);
#endif
      }
    }
  }
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
  else
  {
    /* MemoryProxy did not provide a response for finishing the command execution */
    RetValue = XCP_E_BUSY;
  }
#endif

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  if (ResponseCTO.Length > 0U)
  {
    /* Enqueue response packet. */
    Xcp_InsertResponseCTO(&ResponseCTO);
  }

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF)
  Xcp_SendCmdPendingEventCto(RetValue);
#endif
  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDMODIFYBITS_EXIT(RetValue);
  return RetValue;
}

#if ((XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON) || (XCP_USE_MEMORY_PROXY == STD_ON))
STATIC FUNC(void, XCP_CODE) Xcp_PrepareDataForRequest (Xcp_MemoryProxyDataType * Data, uint8 Shift_Value, uint16 And_Mask, uint16 Xor_Mask, uint8 MemAreaIdx)
{
  Data->type = XCP_MEMORYPROXYDATA_MODIFY_BITS;
  Data->address = XCP_APPL_GET_ADDRESS(0, Xcp_MTA.Address.Write);
  Data->EventId = XCP_E_INVALID_EVENT;
  /* !LINKSTO Xcp.BswDistribution.Communication.UnitInteraction.CoreLocal,1 */
  /* !LINKSTO Xcp.dsn.BSWDistribution.CalibrationCommands.MemProxyLocked,1 */
  Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_LOCKED);
  Xcp_MemoryProxyBuffer[0] = Shift_Value;
  Xcp_MemoryProxyBuffer[1] = (uint8)((And_Mask & 0xFF00U) >> 8U);
  Xcp_MemoryProxyBuffer[2] = (uint8) And_Mask;
  Xcp_MemoryProxyBuffer[3] = (uint8)((Xor_Mask & 0xFF00U) >> 8U);
  Xcp_MemoryProxyBuffer[4] = (uint8) Xor_Mask;
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
  Data->MemoryAreaIdx = MemAreaIdx;
  Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_REQUESTED_TO_MEMORY_PROXY);
#else
  TS_PARAM_UNUSED(MemAreaIdx);
#endif
}
#endif

#endif /* (XCP_MODIFY_BITS_SUPPORTED == STD_ON) */

#if (XCP_PGM_SUPPORTED == STD_ON)

/*-----------------------------[Xcp_HandleCmdReqExtStatus]-----------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_HandleCmdReqExtStatus(void)
{
  DBG_XCP_HANDLECMDREQEXTSTATUS_ENTER();

  /* If the flash programming was marked to be aborted */
  if ((Xcp_CmdReqStatusExtension & XCP_ABORT_PROGRAMMING_MASK) == XCP_ABORT_PROGRAMMING_MASK)
  {
    /* There is an error, programming of a segment is not active anymore. The user can retry
     * the whole sequence. The programming sequence is aborted immediately */

    /* Clear the active programming flag */
    Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_PROGRAMMING_ACTIVE_MASK);

#if (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)
    /* Clear the programming block mode flag */
    Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_PGM_BLOCK_MODE_MASK);
#endif

    /* Reset the globals used by any memory writing routine */
    Xcp_ResetMemoryWriteInfo();

    /* Clear the stored flag */
    Xcp_CmdReqStatusExtension &= (uint8)(~XCP_ABORT_PROGRAMMING_MASK);
  }

  DBG_XCP_HANDLECMDREQEXTSTATUS_EXIT();
}

/*--------------------[Xcp_ProcessCmdProgramMax]-------------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdProgramMax(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDPROGRAMMAX_ENTRY();

  /* Initialize the ResponseCTO length */
  ResponseCTO.Length = 0U;

  /* If PGM resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_PGM_MASK) ==
             (uint8)XCP_RESOURCE_PGM_MASK
    )
  {
    /* PGM is protected, return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* If PROGRAM_START wasn't called before */
    if ((Xcp_CmdReqStatus & XCP_CMD_PROGRAMMING_STARTED_MASK) == 0U)
    {
      /* Fill in ERR_SEQUENCE message */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEQUENCE, XCP_ERR_SEQUENCE_LENGTH);
    }
    else
    {
      /* Check whether MTA address is set and MTA address has the correct type */
      if ( ( Xcp_MTA.IsSet == FALSE) || ( Xcp_MTA.Type == XCP_MTATYPE_COUNTER) )
      {
       /* Prepare error CTO with negative response XCP_ERR_ACCESS_DENIED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
      }
      else
      {
        /*  Get the data to be programmed  */
        P2VAR( uint8, AUTOMATIC, XCP_APPL_DATA ) DataPtr = &XCP_READ_CMDPACKET_BYTE(XCP_ADDRESS_GRANULARITY_SIZE);

        /* How many elements to program, i.e. maximum possible */
        Xcp_MemoryWrite.ElementsLeftToWrite = Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto_Pgm - XCP_ADDRESS_GRANULARITY_SIZE;

        /* If the active programming flag is not yet set */
        if ((Xcp_CmdReqStatus & XCP_CMD_PROGRAMMING_ACTIVE_MASK) == 0U)
        {
          /* Enter critical section */
          SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

          /* Set the active programming of a segment */
          Xcp_CmdReqStatus |= XCP_CMD_PROGRAMMING_ACTIVE_MASK;

          /* Exit critical section */
          SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
        }

        /* Call the common programming function */
        Xcp_ProgramFlash( DataPtr, XCP_CMD_PROGRAM_MAX_PID );
      }
    }
  }

  /* Check if we need to send a response  */
  if (ResponseCTO.Length > 0U)
  {
    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* We got an error, programming of a segment is not active anymore. The user can retry
     * the whole sequence. */
    Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_PROGRAMMING_ACTIVE_MASK);

    /* Queue the response */
    Xcp_InsertResponseCTO(&ResponseCTO);

    /* Exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* Reset the globals used by any memory writing routine */
    Xcp_ResetMemoryWriteInfo();
  }

  DBG_XCP_PROCESSCMDPROGRAMMAX_EXIT(XCP_E_OK);

  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdProgramReset]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdProgramReset(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  /* Initialize response length */
  ResponseCTO.Length = 0U;

  DBG_XCP_PROCESSCMDPROGRAMRESET_ENTRY();

  /* If PGM resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_PGM_MASK) ==
             (uint8)XCP_RESOURCE_PGM_MASK
    )
  {
    /* PGM is protected, return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* If PROGRAM_START wasn't called before */
    if ((Xcp_CmdReqStatus & XCP_CMD_PROGRAMMING_STARTED_MASK) == 0U)
    {
      /* Fill in ERR_SEQUENCE message */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEQUENCE, XCP_ERR_SEQUENCE_LENGTH);
    }
    else
    {
      Std_ReturnType ReturnValue = Xcp_ProcessCmdDisConnect();
      /* PROGRAM_RESET behaves more or less equal to DISCONNECT */
      /* CHECK: NOPARSE */
      if (ReturnValue == E_OK)
      /* CHECK: PARSE */
      {
        /* Call callout function to trigger any application specific effects */
        Xcp_ApplProgramReset();
      }
      /* CHECK: NOPARSE */
      else
      {
        /* Unreachable else branch. For this branch to be reached one of the following two
           situations should occur:
         * A storage/clearing of DAQ lists in the NV memory should be pending and after a
           programming session should be activated via PROGRAM_START command. A PROGRAM_START
           command cannot be executed if a storage/clearing of DAQ lists in the NV memory is
           pending and therefore PROGRAM_RESET command returns ERR_SEQUENCE response.
         * If a programming session is active, a request for storage/clearing of DAQ lists
           in the NV memory cannot be triggered, because this command is not accepted in a
           programming session. */
        XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
      }
      /* CHECK: PARSE */
    }
  }

  if (ResponseCTO.Length > 0U)
  {
    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* Enqueue response packet */
    Xcp_InsertResponseCTO(&ResponseCTO);

    /* Exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  }

  DBG_XCP_PROCESSCMDPROGRAMRESET_EXIT(XCP_E_OK);

  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdProgram]----------------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdProgram(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDPROGRAM_ENTRY();

  /* Initialize the ResponseCTO length */
  ResponseCTO.Length = 0U;

  /* If PGM resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_PGM_MASK) ==
             (uint8)XCP_RESOURCE_PGM_MASK
    )
  {
    /* PGM is protected, return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* If PROGRAM_START wasn't called before */
    if ((Xcp_CmdReqStatus & XCP_CMD_PROGRAMMING_STARTED_MASK) == 0U)
    {
      /* Fill in ERR_SEQUENCE message */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEQUENCE, XCP_ERR_SEQUENCE_LENGTH);
    }
    else
    {
      /* Check whether MTA address is set and MTA address has the correct type */
      if ( ( Xcp_MTA.IsSet == FALSE) || ( Xcp_MTA.Type == XCP_MTATYPE_COUNTER) )
      {
       /* Prepare error CTO with negative response XCP_ERR_ACCESS_DENIED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
      }
      else
      {
        /* Fetch the number of elements to be programmed */
        const uint8 NoOfDataElementsRequested = XCP_READ_CMDPACKET_BYTE(XCP_PGM_NO_OF_ELEMENTS_INDEX);
        uint8 Xcp_Max_Pgm_Elements_One_Cmd;
        uint8 Xcp_Max_Program_Elements;

#if (XCP_ADDRESS_GRANULARITY_SIZE == XCP_BYTE_AG_SIZE)
        Xcp_Max_Pgm_Elements_One_Cmd = (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto_Pgm - 2U) / XCP_ADDRESS_GRANULARITY_SIZE;
#else
        Xcp_Max_Pgm_Elements_One_Cmd = (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto_Pgm - XCP_ADDRESS_GRANULARITY_SIZE) / XCP_ADDRESS_GRANULARITY_SIZE;
#endif
#if (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)
        if (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_BS_Pgm * Xcp_Max_Pgm_Elements_One_Cmd < XCP_ABS_MAX_PROGRAM_ELEMENTS)
        {
          Xcp_Max_Program_Elements = Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_BS_Pgm * Xcp_Max_Pgm_Elements_One_Cmd;
        }
        else
        {
          Xcp_Max_Program_Elements = XCP_ABS_MAX_PROGRAM_ELEMENTS;
        }
#else
        Xcp_Max_Program_Elements = Xcp_Max_Pgm_Elements_One_Cmd;
#endif
        /* Check that the requested number of data elements is valid */
        if (NoOfDataElementsRequested > Xcp_Max_Program_Elements)
        {
          /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
          XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
        }
        else
        {
          /* Set the pointer to where the data to be programmed is located */
          CONSTP2VAR( uint8, AUTOMATIC, XCP_APPL_DATA ) DataPtr =
              &XCP_READ_CMDPACKET_BYTE(XCP_DL_PGM_CMD_HEADER_LENGTH);

          /* Set the variable holding the number of elements to program with the requested number
           * found in the command */
          Xcp_MemoryWrite.ElementsLeftToWrite = NoOfDataElementsRequested;

          /* Enter critical section */
          SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

          /* Check that this is the PROGRAM with 0 as number of data elements */
          if (NoOfDataElementsRequested == 0U)
          {
            /* This marks the end of an active programming segment */
            Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_PROGRAMMING_ACTIVE_MASK);
          }
          else
          {
            /* If the active programming flag is not yet set */
            if ((Xcp_CmdReqStatus & XCP_CMD_PROGRAMMING_ACTIVE_MASK) == 0U)
            {
              /* Set the beginning of an active programming of a segment */
              Xcp_CmdReqStatus |= XCP_CMD_PROGRAMMING_ACTIVE_MASK;
            }
          }

          /* Exit critical section */
          SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

          /* Call the common programming function */
          Xcp_ProgramFlash( DataPtr, XCP_CMD_PROGRAM_PID );
        }
      }
    }
  }

  /* Check if we need to send a response  */
  if (ResponseCTO.Length > 0U)
  {
    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* Queue the response */
    Xcp_InsertResponseCTO(&ResponseCTO);

    /* We got an error, programming of a segment is not active anymore. The user can retry
     * the whole sequence. */
    Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_PROGRAMMING_ACTIVE_MASK);

#if (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)
    /* This is the end of the block mode transfer */
    Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_PGM_BLOCK_MODE_MASK);
#endif

    /* Exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  }

  DBG_XCP_PROCESSCMDPROGRAM_EXIT(XCP_E_OK);

  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdProgramNext]------------------------*/

#if (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdProgramNext(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDPROGRAMNEXT_ENTRY();

  /* Initialize the length */
  ResponseCTO.Length = 0U;

  /* Check that a PROGRAM was issued beforehand */
  if ((Xcp_CmdReqStatus & XCP_CMD_PROGRAMMING_ACTIVE_MASK) == 0U)
  {
    /* PROGRAM_NEXT needs a PROGRAM to be called before it, so ERR_SEQUENCE will be returned. */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEQUENCE, XCP_ERR_SEQUENCE_LENGTH);
  }
  else
  {
    /* Fetch the number of elements to be programmed */
    const uint8 NoOfDataElementsRequested = XCP_READ_CMDPACKET_BYTE(XCP_PGM_NO_OF_ELEMENTS_INDEX);

    /* Check that the number of elements left to program match the expected */
    if (Xcp_MemoryWrite.ElementsLeftToWrite != NoOfDataElementsRequested)
    {
      /* If the number of data elements does not match the expected value, the error code
       * ERR_SEQUENCE will be returned.
       * The negative response will contain the expected number of data elements. */
      ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_ERR_PID;
      ResponseCTO.CTO[XCP_RES_ERRCODE_INDEX] = XCP_ERR_SEQUENCE;
      ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = Xcp_MemoryWrite.ElementsLeftToWrite;
      ResponseCTO.Length = XCP_ERR_WRITE_SEQUENCE_LENGTH;
    }
    else
    {
      /* Set the pointer to where the data to be programmed is located */
      CONSTP2VAR( uint8, AUTOMATIC, XCP_APPL_DATA ) DataPtr =
          &XCP_READ_CMDPACKET_BYTE(XCP_DL_PGM_CMD_HEADER_LENGTH);

      /* Call the common programming function */
      Xcp_ProgramFlash(DataPtr , XCP_CMD_PROGRAM_NEXT_PID);
    }
  }

  /* If we have an error response registered */
  if (ResponseCTO.Length > 0U)
  {
    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* We got an error, programming of a segment is not active anymore. The user can retry
     * the whole sequence. */
    Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_PROGRAMMING_ACTIVE_MASK);

    /* This is the end of the block mode transfer */
    Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_PGM_BLOCK_MODE_MASK);

    /* Queue the response */
    Xcp_InsertResponseCTO(&ResponseCTO);

    /* Exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* Reset the globals used by any memory writing routine */
    Xcp_ResetMemoryWriteInfo();
  }

  DBG_XCP_PROCESSCMDPROGRAMNEXT_EXIT(XCP_E_OK);
  return XCP_E_OK;
}
#endif /* (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON) */

/*--------------------[Xcp_ProgramFlash]---------------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_ProgramFlash
(
  P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) DataPtr,
  uint8 CallingCmdPid
)
{
  /* The response the command which calls this function will send to the master */
  Xcp_CTOType ResponseCTO;

  /* the number of elements that will be processed */
  uint8 NumberOfElementsToProcessNow;

  /* The message returned by the callout function */
  Xcp_ApplReturnType CalloutRes = XCP_APPL_ERR_CMD_UNKNOWN;

  DBG_XCP_PROGRAMFLASH_ENTRY(DataPtr, CallingCmdPid);

  /* Initialize the length of our response with 0 as, in block mode, we musn't reply positively
   * unless this is the last programming command. */
  ResponseCTO.Length = 0U;

  if (CallingCmdPid == XCP_CMD_PROGRAM_MAX_PID)
  {
    /* With PROGRAM_MAX, the maximum possible number of elements are programmed. The maximum
     * for this command differs from the maximum possible of PROGRAM/_NEXT commands. */
    NumberOfElementsToProcessNow =  Xcp_MemoryWrite.ElementsLeftToWrite;
  }
  else
  {
    uint8 Xcp_Max_Pgm_Elements_One_Cmd;
#if (XCP_ADDRESS_GRANULARITY_SIZE == XCP_BYTE_AG_SIZE)
    Xcp_Max_Pgm_Elements_One_Cmd = (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto_Pgm - 2U) / XCP_ADDRESS_GRANULARITY_SIZE;
#else
    Xcp_Max_Pgm_Elements_One_Cmd = (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto_Pgm - XCP_ADDRESS_GRANULARITY_SIZE) / XCP_ADDRESS_GRANULARITY_SIZE;
#endif
    /* calculate number of elements to be processed now */
    if (Xcp_MemoryWrite.ElementsLeftToWrite >= Xcp_Max_Pgm_Elements_One_Cmd)
    {
      /* process the maximum number of elements possible in this step */
      NumberOfElementsToProcessNow = Xcp_Max_Pgm_Elements_One_Cmd;
    }
    else
    {
      /* process the last remaining elements or a single PROGRAM (no PROGRAM_NEXT afterwards)
       * with less then maximum elements that can be processed in one cycle */
      NumberOfElementsToProcessNow = Xcp_MemoryWrite.ElementsLeftToWrite;
    }
  }

  /* copy the elements in the internal buffer */
  TS_MemCpy
  (
    (&Xcp_MemoryWrite.Buffer[Xcp_MemoryWrite.ElementsWrittenSoFar * XCP_ADDRESS_GRANULARITY_SIZE]),
    DataPtr,
    ( (uint32)NumberOfElementsToProcessNow * XCP_ADDRESS_GRANULARITY_SIZE )
  );

  /* increment the number of elements processed so far */
  Xcp_MemoryWrite.ElementsWrittenSoFar += NumberOfElementsToProcessNow;

  /* decrement the number of elements remaining to be processed with the number of elements
   * processed in this step */
  Xcp_MemoryWrite.ElementsLeftToWrite -= NumberOfElementsToProcessNow;

  if (Xcp_MemoryWrite.ElementsLeftToWrite == 0U)
  {
    /* calculate the number of bytes to copy from the buffer to the intended memory location */
    uint16 NumberOfBytesToProgram = ( (uint16)Xcp_MemoryWrite.ElementsWrittenSoFar *
                                       XCP_ADDRESS_GRANULARITY_SIZE );

    /* reset the counter holding the number of elements programmed in this sequence */
    Xcp_MemoryWrite.ElementsWrittenSoFar = 0U;

    /* Call the callout function to handle the actual programming */
    CalloutRes = Xcp_ApplProgram( XCP_APPL_GET_ADDRESS(0U, Xcp_MTA.Address.Write),
                                  &(Xcp_MemoryWrite.Buffer[0U]),
                                  NumberOfBytesToProgram
                                );
    /* Determine our response based on the callout response */
    switch( CalloutRes )
    {
      case XCP_APPL_OK:
        /* post-increment MTA address by the number of elements processed in this programming instance */
        Xcp_MTA.Address.Write = &Xcp_MTA.Address.Write[NumberOfBytesToProgram];
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        ResponseCTO.Length = XCP_CMD_PGM_RES_LENGTH;
        break;
      case XCP_APPL_ERR_OUT_OF_RANGE:
        /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
        break;
      case XCP_APPL_ERR_ACCESS_DENIED:
        /* Prepare error CTO with negative response ERR_ACCESS_DENIED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
        break;
      case XCP_APPL_ERR_MEMORY_OVERFLOW:
        /* Prepare error CTO with negative response ERR_MEMORY_OVERFLOW */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_MEMORY_OVERFLOW, XCP_ERR_MEMORY_OVERFLOW_LENGTH);
        break;
      default:
        /* EB specific handling: */
        /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
        break;
    }
  }

   /* Check if we need to send a response */
  if (ResponseCTO.Length > 0U)
  {
    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* Enqueue response packet */
    Xcp_InsertResponseCTO(&ResponseCTO);

    if (CalloutRes != XCP_APPL_OK)
    {
      /* We got an error, programming of a segment is not active anymore. The user can retry
       * the whole sequence. */
      Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_PROGRAMMING_ACTIVE_MASK);
    }

#if (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)
    /* This is the end of the block mode transfer */
    Xcp_CmdReqStatus &= (uint8)(~XCP_CMD_PGM_BLOCK_MODE_MASK);
#endif

    /* Exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* Reset the globals used by any memory writing routine */
    Xcp_ResetMemoryWriteInfo();
  }

  DBG_XCP_PROGRAMFLASH_EXIT();
}

/*--------------------[Xcp_ProcessCmdProgramClear]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdProgramClear(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDPROGRAMCLEAR_ENTRY();

  /* If PGM resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_PGM_MASK) ==
             (uint8)XCP_RESOURCE_PGM_MASK
    )
  {
    /* PGM is protected, return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* If PROGRAM_START wasn't called before */
    if ((Xcp_CmdReqStatus & XCP_CMD_PROGRAMMING_STARTED_MASK) == 0U)
    {
      /* Fill in ERR_SEQUENCE message */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEQUENCE, XCP_ERR_SEQUENCE_LENGTH);
    }
    else
    {
      /* Check whether MTA address is set and MTA address has the correct type */
      if ( ( Xcp_MTA.IsSet == FALSE) || ( Xcp_MTA.Type == XCP_MTATYPE_COUNTER) )
      {
        /* Prepare error CTO with negative response XCP_ERR_ACCESS_DENIED */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
      }
      else
      {
        const uint8 Mode = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);

        if (Mode != XCP_PGM_ABSOLUTE_ACCESS_MODE)
        {
          /* We support only absolute access mode, prepare error CTO with negative response
           * ERR_OUT_OF_RANGE */
          XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
        }
        else
        {

          /* The message returned by the callout function */
          Xcp_ApplReturnType CalloutRes;

          /* The length of the memory part to be cleared */
          const uint32 ClearRange =
              XCP_UINT32_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_PGM_CLEAR_RANGE_INDEX));

          /* Call the callout function to do the actual work */
          CalloutRes = Xcp_ApplProgramClear( XCP_APPL_GET_ADDRESS( 0U, Xcp_MTA.Address.Write),
                                             ClearRange
                                           );

          /* Check the callout function's response and construct our message upon it */
          switch( CalloutRes )
          {
            case XCP_APPL_OK:
              /* Prepare CTO with positive response */
              ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
              ResponseCTO.Length = XCP_CMD_PGM_CLEAR_RES_LENGTH;
              break;
            case XCP_APPL_ERR_OUT_OF_RANGE:
              /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
              XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
              break;
            case XCP_APPL_ERR_ACCESS_DENIED:
              /* Prepare error CTO with negative response ERR_ACCESS_DENIED */
              XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
              break;
            default:
              /* EB specific handling: */
              /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
              XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
              break;
          }
        }
      }
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_PROCESSCMDPROGRAMCLEAR_EXIT(XCP_E_OK);

  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdProgramStart]-----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdProgramStart(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDPROGRAMSTART_ENTRY();

  /* If PGM resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & (uint8)XCP_RESOURCE_PGM_MASK) ==
             (uint8)XCP_RESOURCE_PGM_MASK
    )
  {
    /* PGM is protected, return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* If we have at least one DAQ list running, we must reject the programming start */
    if ((Xcp_Status.SessionStatus & XCP_MASK_DAQ_RUNNING ) != 0U )
    {
      /* Fill in ERR_DAQ_ACTIVE message */
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_DAQ_ACTIVE, XCP_ERR_DAQ_ACTIVE_LENGTH);
    }
    else
    {
      /* The message returned by the callout function */
      Xcp_ApplReturnType CalloutRes;

      /* Variable to pass on the callout function to write the error code */
      uint16 ErrorCode = 0U;

      /* Call the callout function to do the actual work */
      CalloutRes = Xcp_ApplProgramStart( &ErrorCode );

      /* Check the callout function's response and construct our message upon it */
      switch( CalloutRes )
      {
        case XCP_APPL_OK:
          /* Prepare CTO with positive response */
          ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
          ResponseCTO.CTO[1U] = XCP_RESERVED_BYTE;
          ResponseCTO.CTO[2U] = XCP_COMM_MODE_PGM;
          ResponseCTO.CTO[3U] = Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto_Pgm;
          ResponseCTO.CTO[4U] = XCP_GET_CONNECTION_MAX_BS_PGM;
          ResponseCTO.CTO[5U] = XCP_MIN_ST_PGM;
          ResponseCTO.CTO[6U] = 0U; /* No interleaved mode support */
          ResponseCTO.Length = XCP_CMD_PGM_START_RES_LENGTH;

          /* Enter critical section */
          SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

          /* Mark the programming in progress */
          Xcp_CmdReqStatus |= XCP_CMD_PROGRAMMING_STARTED_MASK;

          /* Exit critical section */
          SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

          break;
        case XCP_APPL_ERR_GENERIC:
          /* Prepare error CTO with negative response ERR_GENERIC */
          XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_GENERIC, XCP_ERR_GENERIC_LENGTH);
          Xcp_WriteWordToByteArray(&ResponseCTO.CTO[XCP_RES_ERRGENERIC_ADDERR_INDEX], ErrorCode);
          break;
        default:
          /* EB specific handling: */
          /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
          XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
          break;
      }
    }
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_PROCESSCMDPROGRAMSTART_EXIT(XCP_E_OK);

  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdGetSectorInfo]-------------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetSectorInfo(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDGETSECTORINFO_ENTRY();

  /* Check if the Programming Resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_PGM_MASK) ==
                 XCP_RESOURCE_PGM_MASK)
  {
    /* Programming commands are protected; return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Callout function return value */
    Xcp_ApplReturnType CalloutRes;
    /* GET_SECTOR_INFO: Mode */
    const uint8 Mode = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);
    /* GET_SECTOR_INFO: Sector Number */
    const uint8 Sector = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX);
    /* Array where the callout function return sector's name address */
    uint8 GetSectorInfoRespMta[XCP_MTA_ADDRESS_LENGTH];
    /* Array where the callout function return sector's information */
    uint8 GetSectorInfoRespData[XCP_CMD_SECTOR_INFO_MAX_RES_LENGTH - 1U];

    /* Call Xcp_ApplGetSectorInfo() callout function */
    CalloutRes = Xcp_ApplGetSectorInfo( Mode, Sector,
                                        GetSectorInfoRespMta,
                                        GetSectorInfoRespData );

    switch( CalloutRes )
    {
      case XCP_APPL_OK:
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        /* Check Mode parameter */
        if( ( Mode == 0U ) || ( Mode == 1U ) )
        { /* Mode = 0 or Mode = 1 */
          /* Copy 7-byte data containing sector information */
          TS_MemCpy( &ResponseCTO.CTO[XCP_RES_BYTE1_INDEX],
                     GetSectorInfoRespData,
                     (XCP_CMD_SECTOR_INFO_MAX_RES_LENGTH - 1U) );
          ResponseCTO.Length = XCP_CMD_SECTOR_INFO_MAX_RES_LENGTH;
        }
        else
        { /* Mode = 2 */
          /* Copy the sector name length information */
          ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] =
              GetSectorInfoRespData[XCP_SECTOR_INFO_NAME_BYTE_POS];
          ResponseCTO.Length = XCP_CMD_SECTOR_INFO_MIN_RES_LENGTH;

          /* set MTA to the Sector's name address, if available */
          if(GetSectorInfoRespData[XCP_SECTOR_INFO_NAME_BYTE_POS] > 0U)
          {
            /* If address granularity is 4 bytes */
            Xcp_MTA.Type = XCP_MTATYPE_ADDRESS;

            /* set MTA to the Sector's name address */
            TS_MemCpy( &Xcp_MTA.Address.Read,
                       GetSectorInfoRespMta,
                       XCP_MTA_ADDRESS_LENGTH);

            /* Mark the MTA as being set */
            Xcp_MTA.IsSet = TRUE;
          }
        }
        break;
      case XCP_APPL_ERR_MODE_NOT_VALID:
        /* Prepare error CTO with negative response ERR_MODE_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_MODE_NOT_VALID, XCP_ERR_MODE_NOT_VALID_LENGTH);
        break;
      case XCP_APPL_ERR_SEGMENT_NOT_VALID:
        /* Prepare error CTO with negative response XCP_ERR_SEGMENT_NOT_VALID */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_SEGMENT_NOT_VALID, XCP_ERR_SEGMENT_NOT_VALID_LENGTH);
        break;
      default:
        /* EB specific handling: */
        /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
        break;
    } /* switch( CalloutRes ) */
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETSECTORINFO_EXIT(XCP_E_OK);

  return XCP_E_OK;
}

/*--------------------[Xcp_ProcessCmdGetPgmProcInfo]----------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdGetPgmProcInfo(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;

  DBG_XCP_PROCESSCMDGETPGMPROCINFO_ENTRY();

  /* Check if the Programming Resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_PGM_MASK) ==
                 XCP_RESOURCE_PGM_MASK)
  {
    /* Programming commands are protected; return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Callout function return value */
    Xcp_ApplReturnType CalloutRes;
    /* Total number of available sectors */
    uint8 MaxSector;
    /* General properties for programming */
    uint8 PgmProperties;

    /* Call Xcp_ApplGetPgmProcessorInfo() callout function */
    CalloutRes = Xcp_ApplGetPgmProcessorInfo( &MaxSector, &PgmProperties );

    /* Check the callout function return */
    switch( CalloutRes )
    {
      case XCP_APPL_OK:
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        PgmProperties &= ( XCP_PGM_PROPERTIES_MASK | XCP_PGM_ABS_ACCESS_MODE_MASK );
        ResponseCTO.CTO[XCP_RES_BYTE1_INDEX] = PgmProperties;
        ResponseCTO.CTO[XCP_RES_BYTE2_INDEX] = MaxSector;
        ResponseCTO.Length = XCP_CMD_PGM_PROC_INFO_RES_LENGTH;
        break;
      default:
        /* EB specific handling: */
        /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
        break;
    } /* switch( CalloutRes ) */
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();


  DBG_XCP_PROCESSCMDGETPGMPROCINFO_EXIT(XCP_E_OK);

  return XCP_E_OK;
}


#endif /* (XCP_PGM_SUPPORTED == STD_ON) */

/*--------------------[Xcp_ProcessCmdSetRequest]----------------------*/
#if ((XCP_STORE_DAQ_SUPPORTED == STD_ON) || \
     (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON))

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdSetRequest(void)
{
  /* Local variable to prepare the response */
  Xcp_CTOType ResponseCTO;
  /* Local variable to hold requested mode */
  uint8 RequestedMode;

  DBG_XCP_PROCESSCMDSETREQUEST_ENTRY();
  /* Initialize response CTO length */
  ResponseCTO.Length = 0U;

  /* Retrieve the requested mode */
  RequestedMode = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);

  /* Check if a NvM storage is currently pending */
  if ((Xcp_Status.SessionStatus &
      ((XCP_MASK_CLEAR_DAQ_REQ | (XCP_MASK_STORE_DAQ_REQ | XCP_MASK_STORE_CAL_REQ)))) != 0U)
  {
    /* The master must wait for the current request to finish, issue ERR_RES_TEMP_NOT_A. */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_RES_TEMP_NOT_A, XCP_ERR_RES_TEMP_NOT_A_LENGTH);
  }
  else
  {
    switch (RequestedMode)
    {
#if (XCP_STORE_DAQ_SUPPORTED == STD_ON)
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF)
      /* !LINKSTO Xcp.BswDistribution.Daq.SetRequest.NoResume,1 */
      case XCP_MASK_STORE_DAQ_REQ_RESUME:
#endif
      case XCP_MASK_STORE_DAQ_REQ_NORESUME:
      {
        /* Request for write the DAQ lists */
        Xcp_StartStoreDaq(RequestedMode, &ResponseCTO);
        break;
      }
      case XCP_MASK_CLEAR_DAQ_REQ:
      {
        /* Request for write/clear the DAQ lists */
        Xcp_StartClearDaq(&ResponseCTO);
        break;
      }
#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) */
#if ( XCP_CAL_PAG_STORE_SUPPORTED == STD_ON)
      case XCP_MASK_STORE_CAL_REQ:
      {
        /* Request for write the calibration/paging data */
        Xcp_StartStoreCal(&ResponseCTO);
        break;
      }
#endif /* (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON) */
      default:
      {
        /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
        XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
        break;
      }
    }
  }
  /* If we have an answer to send */
  /* CHECK: NOPARSE */
  if (ResponseCTO.Length > 0U)
  /* CHECK: PARSE */
  {
    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* Enqueue response packet. */
    Xcp_InsertResponseCTO(&ResponseCTO);

    /* Exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  }
  else
  /* CHECK: NOPARSE */
  {
    /* The else branch cannot be reached in the current implementation because
     * a response shall always be enqueued into the CTO. There is no situation
     * in which a response does not exist and therefore not enqueued.
     */
    XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
  }
  /* CHECK: PARSE */
  DBG_XCP_PROCESSCMDSETREQUEST_EXIT(XCP_E_OK);
  return XCP_E_OK;
}
#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON || XCP_CAL_PAG_STORE_SUPPORTED == STD_ON) */
  /* If user commands configured */
#if (XCP_NUMBER_OF_USERCOMMANDS > 0U)

/*---------------User defined commands--------------------------------------*/
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessCmdUserDefined(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  uint8 SubCommandIndex;
  uint8 Xcp_SubCmdResponse[XCP_MAX_CTO - 1];

  boolean Xcp_ExecuteCallout = TRUE;
  Xcp_ErrorType StatusVal = XCP_E_NOT_OK;

  DBG_XCP_PROCESSCMDUSERDEFINED_ENTRY();

  for (SubCommandIndex = 0; SubCommandIndex < XCP_NUMBER_OF_USERCOMMANDS ; SubCommandIndex++)
  {
      /* !LINKSTO Xcp.Configuration.XcpUserCommand.XcpSubCommandCode,1 */
    /* retrieve the command */
    if (XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX) ==  Xcp_UserCommandConf[SubCommandIndex].SubCmdCode)
    {
      StatusVal = XCP_E_OK;
     /* exit loop  if the command was found*/
     break;
    }
  }
  /* sub-command code shall be within valid range */
  if (StatusVal == XCP_E_OK)
  {
    if (Xcp_UserCommandConf[SubCommandIndex].SubCmdRespLength >
        (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto - 1))
    {
      XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
      Xcp_ExecuteCallout = FALSE;
    }
    /* Check if the Programming Resource is protected SubCmdSeedKey*/
    if(Xcp_UserCommandConf[SubCommandIndex].SubCmdSeedKey != XCP_RESOURCE_UNLOCKED)
    {
     /* check protection status */
     if((XCP_GET_RESOURCE_PROTECTION_STATUS() & Xcp_UserCommandConf[SubCommandIndex].SubCmdSeedKey) ==
                    Xcp_UserCommandConf[SubCommandIndex].SubCmdSeedKey)
     {
       /* Programming commands are protected; return ERR_ACCESS_LOCKED */
       XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
       Xcp_ExecuteCallout = FALSE;
     }
    }
   /* call user callout function */
    if(Xcp_ExecuteCallout == TRUE)
    {
      /* !LINKSTO Xcp.Configuration.XcpUserCommand.XcpSubCommandCallout.PositiveResponse,1 */
      if(E_OK == Xcp_UserCommandConf[SubCommandIndex].SubCmdCallout(
                 &(XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX)),
                 Xcp_UserCommandConf[SubCommandIndex].SubCmdLength,
                 Xcp_SubCmdResponse,
                 Xcp_UserCommandConf[SubCommandIndex].SubCmdRespLength))
      {
        /* Prepare CTO with positive response */
        ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        /* If response user call-out shall add data to the response,
          copy response to the response queue */
        if (Xcp_UserCommandConf[SubCommandIndex].SubCmdRespLength > 0U)
        {
          TS_MemCpy(&ResponseCTO.CTO[XCP_CMD_BYTE1_INDEX],
                   &Xcp_SubCmdResponse,
                   Xcp_UserCommandConf[SubCommandIndex].SubCmdRespLength);
        }
        ResponseCTO.Length = Xcp_UserCommandConf[SubCommandIndex].SubCmdRespLength + 1U;
      }
      else
      {
        /* !LINKSTO Xcp.Configuration.XcpUserCommand.XcpSubCommandCallout.NegativeResponse,1 */
       /* load the first 2 bytes for the generic error response */
       ResponseCTO.CTO[XCP_RES_PID_INDEX] = XCP_ERR_PID;
       ResponseCTO.CTO[XCP_CMD_BYTE1_INDEX] = XCP_ERR_GENERIC;
       /* dummy value for the response in case user response length is 0 */
       ResponseCTO.CTO[XCP_CMD_BYTE2_INDEX] = 0xFFU;
       ResponseCTO.CTO[XCP_CMD_BYTE3_INDEX] = 0xFFU;
       ResponseCTO.Length = XCP_ERR_GENERIC_LENGTH;

       if (Xcp_UserCommandConf[SubCommandIndex].SubCmdRespLength > 0U)
        {
          TS_MemCpy(&ResponseCTO.CTO[XCP_CMD_BYTE2_INDEX],
                  &Xcp_SubCmdResponse,
                  Xcp_UserCommandConf[SubCommandIndex].SubCmdRespLength);
        }
      }
    }
  }
  else
  {
    /* User command Sub-Command code not configured - error  XCP_ERR_CMD_UNKNOWN*/
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Enqueue response packet */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_PROCESSCMDUSERDEFINED_EXIT(XCP_E_OK);
  return E_OK;
}
#endif /* (XCP_NUMBER_OF_USERCOMMANDS > 0U) */


/*--------------------------[Xcp_StartStoreDaq]-----------------------------*/

#if (XCP_STORE_DAQ_SUPPORTED == STD_ON)
STATIC FUNC(void, XCP_CODE) Xcp_StartStoreDaq
(
  uint8 RequestedMode,
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA) ResponseCTO
)
{
  /* The Session Configuration Id from the SET_REQUEST command */
  uint16 SessionConfigId =
      XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));

  DBG_XCP_STARTSTOREDAQ_ENTRY(RequestedMode,ResponseCTO);
  /* If the Session Configuration ID is 0, reject the store request */
  if (SessionConfigId == 0U)
  {
    /* Fill in ERR_CMD_SYNTAX message */
    XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_CMD_SYNTAX, XCP_ERR_CMD_SYNTAX_LENGTH);
  }
  else
  {
    /* If we have at least a DAQ list running, we must reject the store request */
    if ((Xcp_Status.SessionStatus & XCP_MASK_DAQ_RUNNING ) != 0U )
    {
      /* Fill in ERR_DAQ_ACTIVE message */
      XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_DAQ_ACTIVE, XCP_ERR_DAQ_ACTIVE_LENGTH);
    }
    else
    {
      /* Iterator */
      Xcp_DaqIdType i;
      /* Check that at least one DAQ list was selected */
      boolean AtLeastOneDaqListIsSelected = FALSE;

      /* Clear any previous selected DAQ lists from the ADMIN array */
      for (i = 0U; i< XCP_NUMBER_OF_BYTES_SELECTED_DAQ; i++)
      {
        Xcp_DaqLists.Xcp_StoreDaq.SelectedDAQ[i] = 0U;
      }

      /* The mode must be also saved in the NV memory in order to determine if this is the RESUME
       * mode or not upon slave restart. */
      Xcp_DaqLists.Xcp_StoreDaq.Mode = RequestedMode;

#if (XCP_DAQ_COUNT > 0U)
      /* Save the number of dynamically configured DAQ lists */
      Xcp_DaqLists.Xcp_StoreDaq.NoOfConfiguredDynamicDaqLists = Xcp_DynamicInfo.NoOfConfiguredDynamicDAQLists;
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON)
      Xcp_DaqLists.Xcp_StoreDaq.NoOfConfiguredOdts = Xcp_DynamicInfo.AllocatedOdtsSoFar;
      Xcp_DaqLists.Xcp_StoreDaq.NoOfConfiguredOdtEntries = Xcp_DynamicInfo.AllocatedOdtEntriesSoFar;
#endif
#endif

      /* For all selected DAQ lists - set the appropriate selected flag in the store DAQ admin
       * structure while clearing the one in the DAQ list's mode (retrieved by GET_DAQ_LIST_MODE) */
      for (i = 0U; i< XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(Xcp_DaqIdType); i++)
      {
        /* Get the DAQ list */
        P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(i);
        /* If the DAQ List is selected */
        if ((DaqListPtr->Mode & XCP_MASK_DAQLIST_SELECTED) != 0U)
        {

          /* Save the selected flag in the NV memory via the store DAQ admin block */
          XCP_SET_BYTE_FLAG(i, Xcp_DaqLists.Xcp_StoreDaq.SelectedDAQ);

          /* Clear the selected flag from the DAQ's mode */
          DaqListPtr->Mode &= (uint8)(~XCP_MASK_DAQLIST_SELECTED);

          if (RequestedMode == XCP_MASK_STORE_DAQ_REQ_RESUME)
          {
            /* Set the resume flag for the DAQ list */
            DaqListPtr->Mode |= XCP_MASK_DAQLIST_RESUME;
          }
          AtLeastOneDaqListIsSelected = TRUE;
        }
      }
      /* If not a single DAQ list was selected, reject with ERR_SEQUENCE message */
      if (AtLeastOneDaqListIsSelected == FALSE)
      {
        /* Fill in ERR_SEQUENCE message */
        XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_SEQUENCE, XCP_ERR_SEQUENCE_LENGTH);
      }
      else
      {
        /* Proceed if the write request is accepted by the NvM. We give a NULL_PTR as the source
         * pointer as the block should have a permanent RAM address assigned to it */
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF)
        if (NvM_WriteBlock(XCP_STORE_DAQ_NVM_BLOCK_ID, NULL_PTR) == E_OK)
#endif
        {
#if (XCP_REMOTE_DESTINATIONS == STD_ON)
          uint8 applicationId = (uint8)GetApplicationID();

          if (applicationId != XCP_MASTER_APPLICATION)
          {
            Xcp_SendNvMRequest[applicationId](applicationId);
          }
          else
          {
            (void)NvM_WriteBlock(XCP_STORE_DAQ_NVM_BLOCK_ID, NULL_PTR);
          }
#endif
          /* Set the Store DAQ pending flag in the session status*/
          XCP_SET_FLAG_U8_ATOMIC(XCP_BIT_STORE_DAQ_REQ, Xcp_Status.SessionStatus);

          /* Save the Configuration Session ID */
          Xcp_DaqLists.Xcp_StoreDaq.ConfigurationSessionID = SessionConfigId;
#if(XCP_NO_OF_CONNECTIONS > 1U)
          /* Save the Configuration Session ID */
          Xcp_DaqLists.Xcp_StoreDaq.ConnectionId = Xcp_ConnectionStatus.ConnectionId;
#endif
          /* Fill in positive response */
          ResponseCTO->CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
          ResponseCTO->Length = XCP_SET_REQUEST_POSITIVE_LENGTH;
        }
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF)
        else
        {
          /* NvM has rejected the request : send error message RESOURCE_TEMPORARY_NOT_ACCESSIBLE */
          XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_RES_TEMP_NOT_A, XCP_ERR_RES_TEMP_NOT_A_LENGTH);

          /* We must restore the selected status and the resume mode (if needed) for the selected
           * DAQ lists */
          for (i = 0U; i< XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(Xcp_DaqIdType); i++)
          {
            /* Get the DAQ list */
            P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(i);

            /* If a DAQ List is selected to be stored mode */
            if (XCP_IS_BYTE_FLAG_SET(i, Xcp_DaqLists.Xcp_StoreDaq.SelectedDAQ) == TRUE)
            {
              /* Set the selected flag from the DAQ's mode */
              DaqListPtr->Mode |= XCP_MASK_DAQLIST_SELECTED;

              if (RequestedMode == XCP_MASK_STORE_DAQ_REQ_RESUME)
              {
                /* Clear the resume flag for the DAQ list */
                DaqListPtr->Mode &= (uint8)(~XCP_MASK_DAQLIST_RESUME);
              }
            }
          }
        }
#endif
      }
    }
  }

  DBG_XCP_STARTSTOREDAQ_EXIT(RequestedMode,ResponseCTO);
}

/*--------------------------[Xcp_StartClearDaq]-----------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_StartClearDaq
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA) ResponseCTO
)
{
  DBG_XCP_STARTCLEARDAQ_ENTRY(ResponseCTO);

  /* If we have at least a DAQ list running, we must reject the clear request */
  if ((Xcp_Status.SessionStatus & XCP_MASK_DAQ_RUNNING ) != 0U )
  {
    /* Fill in ERR_DAQ_ACTIVE message */
    XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_DAQ_ACTIVE, XCP_ERR_DAQ_ACTIVE_LENGTH);
  }
  else
  {
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF)
    /* The Session Configuration Id is saved if restore is needed later on */
    uint16 SavedSessionConfigId = Xcp_DaqLists.Xcp_StoreDaq.ConfigurationSessionID;
#endif
    /* Clear the Session Configuration ID */
    Xcp_DaqLists.Xcp_StoreDaq.ConfigurationSessionID = 0U;
#if(XCP_NO_OF_CONNECTIONS > 1U)
    /* Set the connectionId to INVALID */
    Xcp_DaqLists.Xcp_StoreDaq.ConnectionId = XCP_INVALID_CONNECTION_ID;
#endif
    /* The mode must be also saved in the NV memory */
    Xcp_DaqLists.Xcp_StoreDaq.Mode = XCP_MASK_CLEAR_DAQ_REQ;

    /* Register the write request */
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF)
    if (NvM_WriteBlock(XCP_STORE_DAQ_NVM_BLOCK_ID, NULL_PTR) == E_OK)
#endif
    {
#if (XCP_REMOTE_DESTINATIONS == STD_ON)
      uint8 applicationId = (uint8)GetApplicationID();

      if (applicationId != XCP_MASTER_APPLICATION)
      {
        Xcp_SendNvMRequest[applicationId](applicationId);
      }
      else
      {
        (void)NvM_WriteBlock(XCP_STORE_DAQ_NVM_BLOCK_ID, NULL_PTR);
      }
#endif
      /* Set the clear DAQ pending flag in the session status bit field as this */
      XCP_SET_FLAG_U8_ATOMIC(XCP_BIT_CLEAR_DAQ_REQ, Xcp_Status.SessionStatus);

      /* Fill in positive response */
      ResponseCTO->CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
      ResponseCTO->Length = XCP_SET_REQUEST_POSITIVE_LENGTH;
    }
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF)
    else
    {
      /* Restore the Session Configuration Id */
      Xcp_DaqLists.Xcp_StoreDaq.ConfigurationSessionID = SavedSessionConfigId;
#if(XCP_NO_OF_CONNECTIONS > 1U)
      /* Restore ConnectionId */
      Xcp_DaqLists.Xcp_StoreDaq.ConnectionId = Xcp_ConnectionStatus.ConnectionId;
#endif
      /* Restore the Clear mode */
      Xcp_DaqLists.Xcp_StoreDaq.Mode &= (uint8)(~XCP_MASK_CLEAR_DAQ_REQ);

      /* NvM has rejected the request : send error message */
      XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_RES_TEMP_NOT_A, XCP_ERR_RES_TEMP_NOT_A_LENGTH);
    }
#endif
  }

  DBG_XCP_STARTCLEARDAQ_EXIT(ResponseCTO);
}

#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) */

#if ( XCP_CAL_PAG_STORE_SUPPORTED == STD_ON)
/*--------------------------[Xcp_StartStoreCal]-----------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_StartStoreCal
(
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA) ResponseCTO
)
{
  DBG_XCP_STARTSTORECAL_ENTRY(ResponseCTO);

  /* Check if the Calibration/Paging Resource is protected */
  if((XCP_GET_RESOURCE_PROTECTION_STATUS() & XCP_RESOURCE_CAL_PAG_MASK) ==
                 XCP_RESOURCE_CAL_PAG_MASK)
  {
    /* Calibration commands are protected; return ERR_ACCESS_LOCKED */
    XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_ACCESS_LOCKED, XCP_ERR_ACCESS_LOCKED_LENGTH);
  }
  else
  {
    /* Callout function return value */
    Xcp_ApplReturnType CalloutRes;
    /* Call Xcp_ApplSetReqStoreCalReq() callout function */
    CalloutRes = Xcp_ApplSetReqStoreCalReq( );

    switch( CalloutRes )
    {
      case XCP_APPL_OK:
        /* Set STORE_CAL_REQ mode parameter */
        XCP_SET_FLAG_U8_ATOMIC(XCP_BIT_STORE_CAL_REQ, Xcp_Status.SessionStatus);
        /* Prepare CTO with positive response */
        ResponseCTO->CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
        ResponseCTO->Length = XCP_CMD_SET_REQUEST_RES_LENGTH;
        break;
      case XCP_APPL_ERR_OUT_OF_RANGE:
        /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
        XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
        break;
      default:
        /* EB specific handling: */
        /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
        XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
        break;
    } /* switch( CalloutRes ) */
  }

  DBG_XCP_STARTSTORECAL_EXIT(ResponseCTO);
}

#endif /* (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON) */


#if (XCP_ON_FLEXRAY_ENABLED == STD_ON) &&  (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)

/*--------------------[Xcp_ProcessSubCmdFlxAssign]----------------------------*/

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdFlxAssign(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  /* Local flag that holds the command response value */
  uint8 commandResponse = XCP_RES_PID;
  /* Local variable to hold the FLX_BUF parameter */
  const uint8 CmdParamFlxBuf = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX);
  /* Local variable to hold the XCP_PACKET parameter */
  const uint8 CmdParamXcpPacketType = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE3_INDEX);
  /* Initialize the ResponseCTO length */
  ResponseCTO.Length = 0U;

  if (CmdParamXcpPacketType == 0U)
  {
    commandResponse = Xcp_FlxAssignResetBuffer(CmdParamFlxBuf);
  }
  else
  {
    if (Xcp_FlxAssignParametersAreValidated() == TRUE)
    {
      /* Local variable to hold the SLOT_ID parameter */
      const uint16 CmdParamFlxSlotId =
        XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX));
      /* Local variable to hold the OFFSET parameter */
      const uint8 CmdParamOffset = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE6_INDEX);
      /* Local variable to hold the CYCLE_REPETITION parameter */
      const uint8 CmdParamCycleRepetition = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE7_INDEX);
      /* Local variable to hold the CHANNEL parameter */
      const uint8 CmdParamFlxChannel = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE8_INDEX);
      /* Local variable to hold the MAX_FLX_LEN_BUF_x parameter */
      const uint8 CmdParamMaxFlxLenBuf = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE9_INDEX);
      /* Local variable to hold the HEADER_CRC parameter */
      const uint16 CmdParamHeaderCrc =
        XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE10_INDEX));

      /* (Re)configure the Xcp Packets internally */
      if (Xcp_SetXcpPacket(CmdParamFlxBuf, CmdParamXcpPacketType) == E_OK)
      {
        if(XCP_IS_FLAG_SET(XCP_BUFFER_FIXED_MASK, Xcp_FlxBufConfig[CmdParamFlxBuf].Flags,uint8) == FALSE)
        {
          /* Adjust buffer lengths in Xcp */
          if (Xcp_FlxBufConfig[CmdParamFlxBuf].FlxBufLengthProperties != XCP_FLX_BUF_VALUE_FIXED)
          {
            /* Enter critical section */
            SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
            if(XCP_IS_FLAG_SET(XCP_BUFFER_DIRECTION_TX_MASK, Xcp_FlxBufConfig[CmdParamFlxBuf].Flags,uint8) == TRUE)
            {
              Xcp_SetTxFlxBufferLength(CmdParamFlxBuf, CmdParamMaxFlxLenBuf);
            }
            else
            {
              Xcp_SetRxFlxBufferLength(CmdParamFlxBuf, CmdParamMaxFlxLenBuf);
            }
            /* Exit critical section */
            SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
          }

          /* (Re)configure the Flexray Hw Buffer */
          /* !LINKSTO Xcp833.1,1 */
          if (FrIf_ReconfigLPdu(
                                        Xcp_FlxBufConfig[CmdParamFlxBuf].FlxCtrlIdx,  /* FrIf_CtrlIdx */
                                        Xcp_FlxBufConfig[CmdParamFlxBuf].FlxLPDU_ID,  /* FrIf_LPduIdx */
                                        CmdParamFlxSlotId,                            /* FrIf_FrameId */
                                        (Fr_ChannelType)CmdParamFlxChannel,           /* FrIf_ChnlIdx */
                                        CmdParamCycleRepetition,                      /* FrIf_CycleRepetition */
                                        CmdParamOffset,                               /* FrIf_CycleOffset */
                                        CmdParamMaxFlxLenBuf,                         /* FrIf_PayloadLength */
                                        CmdParamHeaderCrc                             /* FrIf_HeaderCRC */
                                ) != E_OK
             )
          {
            commandResponse = XCP_ERR_CMD_SYNTAX;
          }
        }
      }
      else
      {
        commandResponse = XCP_ERR_OUT_OF_RANGE;
      }
    }
    else
    {
      commandResponse = XCP_ERR_OUT_OF_RANGE;
    }
  }

  if (commandResponse == XCP_RES_PID)
  {
    /* Prepare CTO with positive response */
    ResponseCTO.CTO[XCP_RES_PID_INDEX]   = XCP_RES_PID;
    ResponseCTO.Length = XCP_SUBCMD_FLX_ASSIGN_RES_LENGTH;
  }
  else if (commandResponse == XCP_ERR_OUT_OF_RANGE)
  {
    /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
  }
  else
  {
    /* Prepare error CTO with negative response ERR_CMD_SYNTAX. */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_CMD_SYNTAX, XCP_ERR_CMD_SYNTAX_LENGTH);
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Queue the response */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  return XCP_E_OK;
}

STATIC FUNC(uint8, XCP_CODE) Xcp_FlxAssignResetBuffer(uint8 FlxBuff)
{
  /* Local flag that holds the command response value */
  uint8 commandResponse = XCP_RES_PID;

  if (FlxBuff == XCP_FLX_BUF_RESET_ALL)
  {
    uint8 i;
    /* restore packet assignment and LPDU for all buffers */
    for (i = 0; i< XCP_NO_OF_FLX_BUF_CONFIGURED; i++)
    {
      if (Xcp_FlxRestoreBuffer(i) == E_NOT_OK)
      {
        commandResponse = XCP_ERR_CMD_SYNTAX;
        break;
      }
    }
  }
  else
  {
    if (FlxBuff < XCP_NO_OF_FLX_BUF_CONFIGURED)
    {
      /* restore packet assignment and LPDU  only for this buffer */
      if (Xcp_FlxRestoreBuffer(FlxBuff) == E_NOT_OK)
      {
        commandResponse = XCP_ERR_CMD_SYNTAX;
      }
    }
    else
    {
      commandResponse = XCP_ERR_OUT_OF_RANGE;
    }
  }
  return commandResponse;
}

STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_FlxRestoreBuffer(uint8 FlxBuff)
{
  Std_ReturnType retValue = E_NOT_OK;
  Xcp_FlxBufType buffer = Xcp_FlxBufConfig[FlxBuff];
  uint8 bufferLength = Xcp_GetFlxBufferLength(FlxBuff);

  /* Set assignment */
#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) || \
   (XCP_EVENT_PACKET_ENABLED == STD_ON))
  if(XCP_IS_FLAG_SET(XCP_BUFFER_DIRECTION_TX_MASK, buffer.Flags,uint8) == TRUE)
  {
#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
    /* enable/disable the DAQ packet type */
    Xcp_SetPacketEnableStatusDaq(Xcp_TxLPduIdConf[buffer.FlxPduIdConfIdx].IntPduBufId,
        (XCP_IS_FLAG_SET(XCP_SUPPORT_FOR_DAQ_DTO_PDU, Xcp_TxLPduIdConf[buffer.FlxPduIdConfIdx].Properties, uint8)));
#endif
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
    /* enable/disable the EVSERV packet type */
    Xcp_SetPacketEnableStatusEvServ(Xcp_TxLPduIdConf[buffer.FlxPduIdConfIdx].IntPduBufId,
        (XCP_IS_FLAG_SET(XCP_SUPPORT_FOR_EVSERV_CTO_PDU, Xcp_TxLPduIdConf[buffer.FlxPduIdConfIdx].Properties, uint8)));
#endif
  }
#endif /* XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK || XCP_EVENT_PACKET_ENABLED == STD_ON */


#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
  if(XCP_IS_FLAG_SET(XCP_BUFFER_DIRECTION_TX_MASK, buffer.Flags,uint8) == FALSE)
  {
    /* enable/disable the STIM packet type */
    Xcp_SetPacketEnableStatusStim(Xcp_RxLPduIdConf[buffer.FlxPduIdConfIdx].IntPduBufId,
        (XCP_IS_FLAG_SET(XCP_SUPPORT_FOR_STIM_DTO_PDU, Xcp_RxLPduIdConf[buffer.FlxPduIdConfIdx].Properties, uint8)));
  }
#endif /* XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK */

  /* Reset the DAQ lists assignment */
  Xcp_FlxDaqList[FlxBuff].FlxDaqAcceptAll = TRUE;

  /* try to restore LPDU_ID if it is preconfigured */
  retValue = Xcp_SetDefaultLpduToBuffer(FlxBuff, bufferLength);

  return retValue;
}


STATIC FUNC(boolean, XCP_CODE) Xcp_FlxAssignParametersAreValidated(void)
{
  /* Local variable to hold the FLX_BUF parameter */
  const uint8 CmdParamFlxBuf = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX);
  /* Local variable to hold the SLOT_ID parameter */
  const uint16 CmdParamFlxSlotId =
    XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX));
  /* Local variable to hold the OFFSET parameter */
  const uint8 CmdParamOffset = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE6_INDEX);
  /* Local variable to hold the CYCLE_REPETITION parameter */
  const uint8 CmdParamCycleRepetition = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE7_INDEX);
  /* Local variable to hold the CHANNEL parameter */
  const uint8 CmdParamFlxChannel = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE8_INDEX);
  /* Local variable to hold the MAX_FLX_LEN_BUF_x parameter */
  const uint8 CmdParamMaxFlxLenBuf = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE9_INDEX);

  boolean retValue = TRUE;
  /* Initial quick parameter validation */
  /* !LINKSTO Xcp.ASAM.TPCMD.FLEXRAY.FLXAssign.InvalidBuffConf,1 */
  if ((CmdParamFlxBuf >= XCP_NO_OF_FLX_BUF_CONFIGURED)||
      ((CmdParamFlxChannel != XCP_FLX_BUF_VALUE_CHANNEL_A) &&
       (CmdParamFlxChannel != XCP_FLX_BUF_VALUE_CHANNEL_B)))
  {
    retValue = FALSE;
  }
  else if ((CmdParamMaxFlxLenBuf > Xcp_FlxBufConfig[CmdParamFlxBuf].FlxBufLengthInitValue) ||
           (CmdParamFlxSlotId > MAX_FLX_SLOT_ID) ||
           (CmdParamFlxSlotId == INVALID_FLX_SLOT_ID))
  {
    retValue = FALSE;
  }
  else
  {
    /* !LINKSTO Xcp.ASAM.FLEXRAY.BufferConfiguration002,1 */
    if(XCP_IS_FLAG_SET(XCP_BUFFER_FIXED_MASK, Xcp_FlxBufConfig[CmdParamFlxBuf].Flags,uint8) == FALSE)
    {
      /* !LINKSTO Xcp.ASAM.TPCMD.FLEXRAY.FLXAssign.InvalidBuffParameters,1 */
      if ((Xcp_FlxBufConfig[CmdParamFlxBuf].FlxBufLengthProperties == XCP_FLX_BUF_VALUE_FIXED) &&
          (Xcp_FlxBufConfig[CmdParamFlxBuf].FlxBufLengthInitValue != CmdParamMaxFlxLenBuf))
      {
        retValue = FALSE;
      }
      /* !LINKSTO Xcp.ASAM.TPCMD.FLEXRAY.FLXAssign.InvalidBuffParameters,1 */
      else if((Xcp_FlxBufConfig[CmdParamFlxBuf].FlxBufSlotIDProperties == XCP_FLX_BUF_VALUE_FIXED) &&
              (Xcp_FlxBufConfig[CmdParamFlxBuf].FlxBufSlotIDInitValue != CmdParamFlxSlotId))
      {
        retValue = FALSE;
      }
      /* !LINKSTO Xcp.ASAM.TPCMD.FLEXRAY.FLXAssign.InvalidBuffParameters,1 */
      else if ((Xcp_FlxBufConfig[CmdParamFlxBuf].FlxBufOffsetProperties == XCP_FLX_BUF_VALUE_FIXED) &&
              (Xcp_FlxBufConfig[CmdParamFlxBuf].FlxBufOffsetInitValue != CmdParamOffset))
      {
        retValue = FALSE;
      }
      /* !LINKSTO Xcp.ASAM.TPCMD.FLEXRAY.FLXAssign.InvalidBuffParameters,1 */
      else if ((Xcp_FlxBufConfig[CmdParamFlxBuf].FlxBufCycleRepetitionProperties == XCP_FLX_BUF_VALUE_FIXED) &&
          (Xcp_FlxBufConfig[CmdParamFlxBuf].FlxBufCycleRepetitionInitValue != CmdParamCycleRepetition))
      {
        retValue = FALSE;
      }
      /* !LINKSTO Xcp.ASAM.TPCMD.FLEXRAY.FLXAssign.InvalidBuffParameters,1 */
      else if((Xcp_FlxBufConfig[CmdParamFlxBuf].FlxBufChannelProperties == XCP_FLX_BUF_VALUE_FIXED) &&
          (Xcp_FlxBufConfig[CmdParamFlxBuf].FlxBufChannelInitValue != CmdParamFlxChannel))
      {
        retValue = FALSE;
      }
      else
      {
        /* nothing to do */
      }
    }
  }
  return retValue;
}

STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_SetXcpPacket(uint8 FlxBuff, uint8 XcpPacket)
{
  boolean retValue = E_NOT_OK;
  Xcp_FlxBufType buffer = Xcp_FlxBufConfig[FlxBuff];
  boolean shouldBeEnabled;


  if (Xcp_ValidateXcpPacket(FlxBuff, XcpPacket) == TRUE)
  {
    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* Set assignment */
#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) || \
     (XCP_EVENT_PACKET_ENABLED == STD_ON))
    if(XCP_IS_FLAG_SET(XCP_BUFFER_DIRECTION_TX_MASK, buffer.Flags,uint8) == TRUE)
    {
#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
      shouldBeEnabled = ((XcpPacket & XCP_FLX_BUF_PACKET_DAQ_MASK) == XCP_FLX_BUF_PACKET_DAQ_MASK) ? TRUE : FALSE;
      /* enable/disable the DAQ packet type */
      Xcp_SetPacketEnableStatusDaq(Xcp_TxLPduIdConf[buffer.FlxPduIdConfIdx].IntPduBufId, shouldBeEnabled);
#endif
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
      shouldBeEnabled = ((XcpPacket & XCP_FLX_BUF_PACKET_EV_SERV_MASK) == XCP_FLX_BUF_PACKET_EV_SERV_MASK) ? TRUE : FALSE;
      /* enable/disable the EVSERV packet type */
      Xcp_SetPacketEnableStatusEvServ(Xcp_TxLPduIdConf[buffer.FlxPduIdConfIdx].IntPduBufId, shouldBeEnabled);
#endif
    }
#endif /* XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK || XCP_EVENT_PACKET_ENABLED == STD_ON */

#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
    if(XCP_IS_FLAG_SET(XCP_BUFFER_DIRECTION_TX_MASK, buffer.Flags,uint8) == FALSE)
    {
      shouldBeEnabled = ((XcpPacket & XCP_FLX_BUF_PACKET_STIM_MASK) == XCP_FLX_BUF_PACKET_STIM_MASK) ? TRUE : FALSE;
      /* enable/disable the STIM packet type */
      Xcp_SetPacketEnableStatusStim(Xcp_RxLPduIdConf[buffer.FlxPduIdConfIdx].IntPduBufId, shouldBeEnabled);
    }
#endif /* XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK */

    /* Exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    retValue = E_OK;
  }

  return retValue;
}
STATIC FUNC(boolean, XCP_CODE) Xcp_AllowTxPacket (uint8 FlxBuff, uint8 XcpPacket)
{
  boolean retValue = TRUE;
  Xcp_FlxBufType buffer = Xcp_FlxBufConfig[FlxBuff];

  /* Do not allow RESERR packet type to be cleared if the buffer does support already RESERR */
  if (XCP_IS_FLAG_SET(XCP_SUPPORT_FOR_RESERR_CTO_PDU, Xcp_TxLPduIdConf[buffer.FlxPduIdConfIdx].Properties, uint8) == TRUE)
  {
    if (XCP_IS_FLAG_SET(XCP_FLX_BUF_PACKET_RES_ERR_MASK, XcpPacket, uint8) == FALSE)
    {
      retValue = FALSE;
    }
  }
  /* Do not allow RESERR packet type to be set if the buffer doesn't already support already RESERR */
  else
  {
    if (XCP_IS_FLAG_SET(XCP_FLX_BUF_PACKET_RES_ERR_MASK, XcpPacket, uint8) == TRUE)
    {
      retValue = FALSE;
    }
  }
  return retValue;
}

STATIC FUNC(boolean, XCP_CODE) Xcp_AllowRxPacket (uint8 FlxBuff, uint8 XcpPacket)
{

  boolean retValue = FALSE;
  Xcp_FlxBufType buffer = Xcp_FlxBufConfig[FlxBuff];

  /* !LINKSTO Xcp.ASAM.TPCMD.FLEXRAY.FLXAssign.AllowOnlyRxPacketTypeSupportCMD,1 */
  /* Do not allow CMD packet type to be cleared if the buffer does support already CMD */
  if (XCP_IS_FLAG_SET(XCP_SUPPORT_FOR_CMD_CTO_PDU, Xcp_RxLPduIdConf[buffer.FlxPduIdConfIdx].Properties, uint8) == TRUE)
  {
    if (XCP_IS_FLAG_SET(XCP_FLX_BUF_PACKET_CMD_MASK, XcpPacket, uint8) == TRUE)
    {
      retValue = TRUE;
    }
  }
  /* !LINKSTO Xcp.ASAM.TPCMD.FLEXRAY.FLXAssign.AllowOnlyRxPacketTypeNotSupportCMD,1 */
  /* Do not allow CMD packet type to be set if the buffer doesn't already support already CMD */
  else
  {
    if (XCP_IS_FLAG_SET(XCP_FLX_BUF_PACKET_CMD_MASK, XcpPacket, uint8) == FALSE)
    {
      retValue = TRUE;
    }
  }
  return retValue;
}


STATIC FUNC(boolean, XCP_CODE) Xcp_ValidateXcpPacket(uint8 FlxBuff, uint8 XcpPacket)
{
  boolean retValue = FALSE;
  Xcp_FlxBufType buffer = Xcp_FlxBufConfig[FlxBuff];

  /* i: We don't allow multiple PDUs having CMD or RESERR packet types */
  /* !LINKSTO Xcp.ASAM.TPCMD.FLEXRAY.FLXAssign.AllowOnlyTxPacketType,1 */
  /* Check if the package is allowed according to the initially configured direction  */
  if(XCP_IS_FLAG_SET(XCP_BUFFER_DIRECTION_TX_MASK, buffer.Flags,uint8) == TRUE)
  {
    /* Allow only Tx packet types */
    switch(XcpPacket)
    {
      case XCP_FLX_BUF_PACKET_RES_ERR_MASK:
        retValue = TRUE;
        break;
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
      case XCP_FLX_BUF_PACKET_EV_SERV_MASK:
      case (XCP_FLX_BUF_PACKET_RES_ERR_MASK | XCP_FLX_BUF_PACKET_EV_SERV_MASK):
        retValue = TRUE;
        break;
#endif
#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
      case XCP_FLX_BUF_PACKET_DAQ_MASK:
      case(XCP_FLX_BUF_PACKET_RES_ERR_MASK | XCP_FLX_BUF_PACKET_DAQ_MASK):
        retValue = TRUE;
        break;
#endif
#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) &&  (XCP_EVENT_PACKET_ENABLED == STD_ON)
      case (XCP_FLX_BUF_PACKET_RES_ERR_MASK | XCP_FLX_BUF_PACKET_DAQ_MASK | XCP_FLX_BUF_PACKET_EV_SERV_MASK):
        retValue = TRUE;
        break;
#endif
      default:
        /* nothing to do */
        break;
    }
    /* Set RESERR packet support for allowing Tx buffers */
    if(retValue == TRUE)
    {
      retValue = Xcp_AllowTxPacket(FlxBuff, XcpPacket);
    }
  }
  else
  {
    /* Allow only Rx packet types */
#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
    if ((XcpPacket == (XCP_FLX_BUF_PACKET_CMD_MASK)) ||
        (XcpPacket == (XCP_FLX_BUF_PACKET_STIM_MASK)) ||
        (XcpPacket == (XCP_FLX_BUF_PACKET_CMD_MASK | XCP_FLX_BUF_PACKET_STIM_MASK)))
#else
    if (XcpPacket == (XCP_FLX_BUF_PACKET_CMD_MASK))
#endif
    {
      retValue = Xcp_AllowRxPacket(FlxBuff, XcpPacket);
    }
  }
  return retValue;
}

STATIC FUNC(uint8, XCP_CODE) Xcp_GetFlxBufferLength(uint8 FlxBuff)
{
  uint8 retValue;
  if(XCP_IS_FLAG_SET(XCP_BUFFER_DIRECTION_TX_MASK, Xcp_FlxBufConfig[FlxBuff].Flags,uint8) == TRUE)
  {
    retValue = Xcp_GetTxFlxBufferLength(FlxBuff);
  }
  else
  {
    retValue = Xcp_GetRxFlxBufferLength(FlxBuff);
  }
  return retValue;
}

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdFlxActivate(void)
{
  Xcp_CommonFlxActivateDeactivate(TRUE);
  return XCP_E_OK;
}

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdFlxDeActivate(void)
{
  Xcp_CommonFlxActivateDeactivate(FALSE);
  return XCP_E_OK;
}

STATIC FUNC(void, XCP_CODE) Xcp_CommonFlxActivateDeactivate(boolean Activate)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  /* Local flag that holds the command response value */
  uint8 commandResponse = XCP_ERR_OUT_OF_RANGE;

#if(XCP_EVENT_PACKET_ENABLED == STD_ON)
  /* Local variable to hold the FLX_BUF parameter */
  const uint8 CmdParamFlxBuf = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX);
  if (CmdParamFlxBuf < XCP_NO_OF_FLX_BUF_CONFIGURED)
  {
    /* Check if the buffer is Tx and if it has EV_SERV packet type enabled */
    if(XCP_IS_FLAG_SET(XCP_BUFFER_DIRECTION_TX_MASK, Xcp_FlxBufConfig[CmdParamFlxBuf].Flags,uint8) == TRUE)
    {
      if (Xcp_GetPacketStatusEvServEnabled(Xcp_FlxBufConfig[CmdParamFlxBuf].FlxPduIdConfIdx) == TRUE)
      {
        /* activate/deactivate the EVSERV packet type */
        Xcp_SetPacketActivateStatusEvServ(Xcp_TxLPduIdConf[Xcp_FlxBufConfig[CmdParamFlxBuf].FlxPduIdConfIdx].IntPduBufId, Activate);
        commandResponse = XCP_RES_PID;
      }
    }
  }
#else
  TS_PARAM_UNUSED(Activate);
#endif

  /* Check what response should we give  */
  if (commandResponse == XCP_RES_PID)
  {
    /* Prepare CTO with positive response */
    ResponseCTO.CTO[XCP_RES_PID_INDEX]   = XCP_RES_PID;
    ResponseCTO.Length = XCP_SUBCMD_FLX_ACTIVATE_RES_LENGTH;
  }
  else
  {
    /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Queue the response */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
}


STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdFlxSetDaqListBuf(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  uint8 i;
  /* Local flag that holds the command response value */
  uint8 commandResponse = XCP_RES_PID;

  /* Local variable to hold the DAQ_LIST_NUMBER parameter */
  const uint16 CmdParamDaqListNumber =
    XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));

  /* Local variable to hold the number of buffers to be assigned parameter */
  const uint8 CmdParamNumberOfBuffers = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX);

  /* Initialize the ResponseCTO length */
  ResponseCTO.Length = 0U;

#if (XCP_MAX_DAQ > 0U)
  if(CmdParamDaqListNumber < XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(Xcp_DaqIdType))
  {
    /* first validate the buffer IDs */
    if (CmdParamNumberOfBuffers <= XCP_NO_OF_FLX_BUF_CONFIGURED)
    {
      for (i=0; i<CmdParamNumberOfBuffers; i++)
      {
        const uint8 CmdParamBufferId =  XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE5_INDEX + i);
        if (CmdParamBufferId >= XCP_NO_OF_FLX_BUF_CONFIGURED)
        {
          commandResponse = XCP_ERR_OUT_OF_RANGE;
          break;
        }
      }
    }
    else
    {
      commandResponse = XCP_ERR_OUT_OF_RANGE;
    }

    if (commandResponse == XCP_RES_PID)
    {
      /* Next, clear the buffers which already have this DAQ List ID */
      for (i=0; i<XCP_NO_OF_FLX_BUF_CONFIGURED; i++)
      {
        if ((Xcp_FlxDaqList[i].FlxDaqAcceptAll == FALSE) &&
            (Xcp_FlxDaqList[i].FlxDaqAssignment == (Xcp_DaqIdType)CmdParamDaqListNumber))
        {
          Xcp_FlxDaqList[i].FlxDaqAcceptAll = TRUE;
        }
      }

      /* Ultimately, set the buffers for which this DAQ list ID is requested */
      for (i=0; i<CmdParamNumberOfBuffers; i++)
      {
        const uint8 CmdParamBufferId =  XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE5_INDEX + i);
        /* Set the DAQ IDs assignment */
        Xcp_FlxDaqList[CmdParamBufferId].FlxDaqAcceptAll = FALSE;
        Xcp_FlxDaqList[CmdParamBufferId].FlxDaqAssignment = (Xcp_DaqIdType)CmdParamDaqListNumber;
      }
    }
  }
  else
  {
    commandResponse = XCP_ERR_OUT_OF_RANGE;
  }
#else
  commandResponse = XCP_ERR_OUT_OF_RANGE;
#endif /* XCP_MAX_DAQ > 0U */

  /* Check what response should we give  */
  if (commandResponse == XCP_RES_PID)
  {
    /* Prepare CTO with positive response */
    ResponseCTO.CTO[XCP_RES_PID_INDEX]   = XCP_RES_PID;
    ResponseCTO.Length = XCP_SUBCMD_SET_DAQ_FLX_BUF_RES_LENGTH;
  }
  else
  {
    /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
  }

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Queue the response */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  return XCP_E_OK;
}

STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessSubCmdFlxGetDaqListBuf(void)
{
  /* Local variable to prepare response */
  Xcp_CTOType ResponseCTO;
  uint8 i;

  /* Local variable to hold the DAQ_LIST_NUMBER parameter */
  const uint16 CmdParamDaqListNumber =
    XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));

  /* Initialize the ResponseCTO length */
  ResponseCTO.Length = 0U;

#if (XCP_MAX_DAQ > 0U)
  if(CmdParamDaqListNumber < XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(Xcp_DaqIdType))
  {
    uint8 ResLength = XCP_SUBCMD_GET_DAQ_FLX_BUF_RES_MIN_LENGTH;
    /* Prepare CTO with positive response */
    ResponseCTO.CTO[XCP_RES_PID_INDEX]   = XCP_RES_PID;
    /* Implementation decision: buffers are always configurable
       since there is little information in the ASAM spec regarding this parameter */
    ResponseCTO.CTO[XCP_RES_BYTE1_INDEX]   = 1;

    /* Search for buffers which are assigned to the DAQ list */
    for (i=0; i<XCP_NO_OF_FLX_BUF_CONFIGURED; i++)
    {
      if ((Xcp_FlxDaqList[i].FlxDaqAcceptAll == FALSE) &&
          (Xcp_FlxDaqList[i].FlxDaqAssignment == (Xcp_DaqIdType)CmdParamDaqListNumber))
      {
        ResponseCTO.CTO[ResLength]   = i;
        ResLength++;
      }
    }
    /* Set the number of buffers */
    ResponseCTO.CTO[XCP_RES_BYTE2_INDEX]   = ResLength - XCP_SUBCMD_GET_DAQ_FLX_BUF_RES_MIN_LENGTH;
    ResponseCTO.Length = ResLength;
  }
  else
  {
    /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
    XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
  }
#else
  /* Prepare error CTO with negative response ERR_OUT_OF_RANGE */
  XCP_ERROR_RESPONSE(ResponseCTO, XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
#endif /* XCP_MAX_DAQ > 0U */

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Queue the response */
  Xcp_InsertResponseCTO(&ResponseCTO);

  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  return XCP_E_OK;
}


#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) &&  (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON) */

/*--------------------------[Xcp_AbortCommandWithError]-----------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_AbortCommandWithError
(
   P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA)  ErrResponseCTOPtr
)
{
  DBG_XCP_ABORTCOMMANDWITHERROR_ENTRY(ErrResponseCTOPtr);

  /* Enter critical section.*/
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  if (Xcp_ConnectionStatus.State == XCP_STATE_CONNECTED)
  {
    /* Local variable read available space in CTO Queue */
    uint16 SpaceAvailable = XCP_GET_CTOQUEUE_FREESPACE(uint16);

    /* verify available space in CTO queue */
    if (SpaceAvailable >= ErrResponseCTOPtr->Length)
    {
      /* XCP Insert CTO */
      Xcp_InsertResponseCTO(ErrResponseCTOPtr);
    }
  }
  else
  {
    /* Nothing to do */
  }

  /* Dequeue the command */
  Xcp_DeQueueCmdQueue();
  /* Trigger state transition */
  XCP_CMD_PROCESSOR_STATE_TRANS(XCP_CMDPROCESSOR_IDLE);

  /* Exit critical section.*/
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_ABORTCOMMANDWITHERROR_EXIT();
}

/*--------------------------[Xcp_CommandCanBeExecuted]-----------------------------*/

STATIC FUNC(boolean, XCP_CODE) Xcp_CommandCanBeExecuted
(
  uint8 CommandId,
  P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA)  ErrResponseCTOPtr
)
{
  boolean CommandIsExecutable = FALSE;

  DBG_XCP_COMMANDCANBEEXECUTED_ENTRY(CommandId);

  /* Check whether command is available */
  if (XCP_IS_COMMAND_EXECUTABLE(CommandId) == FALSE)
  {
    /* Set the error message */
    ErrResponseCTOPtr->CTO[XCP_RES_ERRCODE_INDEX] = XCP_ERR_CMD_UNKNOWN;
    ErrResponseCTOPtr->Length = XCP_ERR_CMD_UNKNOWN_LENGTH;
  }
  else
  {
    /* The command must be executed */
    CommandIsExecutable = TRUE;
#if (XCP_PGM_SUPPORTED == STD_ON)
    /* During an active programming session or the programming of a segment is active, execute only
     * allowed commands */
    if ((COMMAND_CAN_EXECUTE_PGM_START(CommandId) == FALSE) &&
         ((Xcp_CmdReqStatus & XCP_CMD_PROGRAMMING_STARTED_MASK) == XCP_CMD_PROGRAMMING_STARTED_MASK))
    {
      /* Set the error message */
      ErrResponseCTOPtr->CTO[XCP_RES_ERRCODE_INDEX] = XCP_ERR_PGM_ACTIVE;
      ErrResponseCTOPtr->Length = XCP_ERR_PGM_ACTIVE_LENGTH;
      /* The command must not be executed */
      CommandIsExecutable = FALSE;
    }
    else if ((COMMAND_CAN_EXECUTE_PGM_ACTIVE(CommandId) == FALSE) &&
        ((Xcp_CmdReqStatus & XCP_CMD_PROGRAMMING_ACTIVE_MASK) == XCP_CMD_PROGRAMMING_ACTIVE_MASK))
    {
      /* Set the error message */
       ErrResponseCTOPtr->CTO[XCP_RES_ERRCODE_INDEX] = XCP_ERR_PGM_ACTIVE;
       ErrResponseCTOPtr->Length = XCP_ERR_PGM_ACTIVE_LENGTH;
       /* The command must not be executed */
       CommandIsExecutable = FALSE;
    }
    else
 #endif
    {
#if (XCP_STORE_DAQ_SUPPORTED == STD_ON)
      /* If the command can modify a DAQ list runtime information (anything from Xcp_DaqLists) and
       *  currently a request to STORE/CLEAR the DAQ lists in NV memory is pending, the commands must
       *  be replied with XCP_ERR_CMD_BUSY message and the command shall not be executed. */
      boolean CommandCanModifyDaq = COMMAND_CAN_MODIFY_DAQ_LIST(CommandId);
      if ((CommandCanModifyDaq == TRUE) &&
          (((Xcp_Status.SessionStatus & XCP_MASK_CLEAR_DAQ_REQ) != 0U ) ||
           ((Xcp_Status.SessionStatus & XCP_MASK_STORE_DAQ_REQ) != 0U )))
      {
        /* Set the error message */
        ErrResponseCTOPtr->CTO[XCP_RES_ERRCODE_INDEX] = XCP_ERR_CMD_BUSY;
        ErrResponseCTOPtr->Length = XCP_ERR_CMD_BUSY_LENGTH;
        /* The command must not be executed */
        CommandIsExecutable = FALSE;
      }
      else
      {
        /* Reject the flash programming start if we are currently writing in the NV memory. */
        if ( (CommandId == XCP_CMD_PROGRAM_START_PID) &&
             (((Xcp_Status.SessionStatus & XCP_MASK_CLEAR_DAQ_REQ) != 0U ) ||
             ((Xcp_Status.SessionStatus & XCP_MASK_STORE_DAQ_REQ) != 0U )))
        {
          /* Set the error message */
          ErrResponseCTOPtr->CTO[XCP_RES_ERRCODE_INDEX] = XCP_ERR_CMD_BUSY;
          ErrResponseCTOPtr->Length = XCP_ERR_CMD_BUSY_LENGTH;
          /* The command must not be executed */
          CommandIsExecutable = FALSE;
        }
      }
#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) */
    }
  }

  DBG_XCP_COMMANDCANBEEXECUTED_EXIT(CommandIsExecutable, ErrorMessage);

  return CommandIsExecutable;
}

/*-----------------------[Xcp_ResetMemoryWriteInfo]--------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_ResetMemoryWriteInfo(void)
{
  DBG_XCP_RESETMEMORYWRITEINFO_ENTRY();

  Xcp_MemoryWrite.ElementsLeftToWrite = 0U;
  Xcp_MemoryWrite.ElementsWrittenSoFar = 0U;
  Xcp_MemoryWrite.Pending = FALSE;

  DBG_XCP_RESETMEMORYWRITEINFO_EXIT();
}

#if ((XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF) && (XCP_EVENT_PACKET_ENABLED == STD_ON))
/* !LINKSTO Xcp.CalibrationCommands.EvCmdPending,1 */
STATIC FUNC(void, XCP_CODE) Xcp_SendCmdPendingEventCto(Xcp_ErrorType ret)
{
  if (ret == XCP_E_BUSY)
  {
    Xcp_EventCTOType EventCTO;
    EventCTO.EVENT_CTO[0U] = XCP_EV_PID;
    EventCTO.EVENT_CTO[1U] = XCP_EV_CMD_PENDING;
    EventCTO.Length = XCP_EV_CMD_PENDING_LENGTH;

    Xcp_EnqueueCtoEvent(&EventCTO);
  }
}
#endif

#if (XCP_MODIFY_BITS_SUPPORTED == STD_ON)
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF)
STATIC FUNC(Xcp_ErrorType, XCP_CODE) Xcp_ProcessModifyBits( P2VAR(Xcp_CTOType, AUTOMATIC, XCP_APPL_DATA) ResponseCTO)
{
  Xcp_ErrorType RetValue = XCP_E_OK;
    /* Callout function return value */
  Xcp_ApplReturnType CalloutResForRead, CalloutResForWrite;
  /* Local variable holding the value to be modified */
  uint32 A_Value;
  const uint8 Shift_Value = XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE1_INDEX);
  /* Local variable to hold request parameter AND Mask (MA) */
  const uint16 And_Mask =
      XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE2_INDEX));
  /* Local variable to hold request parameter XOR Mask (MX) */
  const uint16 Xor_Mask =
      XCP_UINT16_FROM_BYTE_ARRAY(&XCP_READ_CMDPACKET_BYTE(XCP_CMD_BYTE4_INDEX));
  /* Local temporary variables used to calculate the result of the MODIFY_BITS command.
     This is because of Misra rule 10.3 and 10.5 contradiction. See below. */
  uint32 Temp_Value_1, Temp_Value_2;
/* !LINKSTO Xcp.ProtectedMemoryLocations.General,1 */
#if ((XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON) && (XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF))
  if (Xcp_IsMemoryAreaAccessible
       (
           XCP_APPL_GET_ADDRESS(0U, Xcp_MTA.Address.Write),
           XCP_MODIFY_VALUE_SIZE,
           XCP_MEMORY_AREA_TYPE_READ_WRITE,
           XCP_MEMORY_AREA_SCOPE_CALIBRATION
       ) == FALSE
     )
  {
    /* !LINKSTO Xcp.ProtectedMemoryLocations.NegativeResponse.Commands,1 */
    /* Prepare error CTO with negative response ERR_ACCESS_DENIED */
    XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
  }
  else
#endif
  {
   /* Read 4 bytes from memory location pointed to by MTA */
   /* read from memory location*/
   /* Deviation MISRAC2012-4 <START> */
#if(XCP_WRITE_READ_MEMORY_RAM == STD_ON)
   CalloutResForRead = Xcp_DescriptorCallbackTypePtr.readFromRAMCallback
   (
     &A_Value,
     XCP_APPL_GET_ADDRESS( 0U, (P2VAR(void, AUTOMATIC, XCP_APPL_DATA)) Xcp_MTA.Address.Write ),
     XCP_MODIFY_VALUE_SIZE
   );
#else
    TS_MemCpy(&A_Value,
              XCP_APPL_GET_ADDRESS( 0U, (P2VAR(void, AUTOMATIC, XCP_APPL_DATA)) Xcp_MTA.Address.Write ),
              XCP_MODIFY_VALUE_SIZE
             );
    CalloutResForRead = XCP_APPL_OK;
#endif /* (XCP_WRITE_READ_MEMORY_RAM == STD_ON) */
    /* Deviation MISRAC2012-4 <STOP> */
    if ( CalloutResForRead == XCP_APPL_OK )
    {
      /* Modify bits */
      /* This line has been split due to Misra rules 10.3 and 10.5 contradiction:
         A = ( ( A ) & ( (~((uint32)(((uint16)~And_Mask) << Shift_Value))) ) ^
                       ( (uint32)(Xor_Mask << Shift_Value) ) )
         Two temporary variables have been added for this purpose. */

      Temp_Value_1 = (uint32)( (uint16)(~And_Mask) ) << Shift_Value;
      Temp_Value_2 = (uint32)Xor_Mask << Shift_Value;

      /* Calculate A_Value */
      A_Value = ( A_Value & (uint32)( ~Temp_Value_1 ) ) ^ Temp_Value_2;

      /* Write DWORD back to memory location pointed to by MTA */
      /*write to memory location*/
#if(XCP_WRITE_READ_MEMORY_RAM == STD_ON)
      CalloutResForWrite = Xcp_DescriptorCallbackTypePtr.writeToRAMCallback
      (
        XCP_APPL_GET_ADDRESS( 0U, (P2VAR(void, AUTOMATIC, XCP_APPL_DATA)) Xcp_MTA.Address.Write ),
        (P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ))&A_Value,
        XCP_MODIFY_VALUE_SIZE
      );
#else
      SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      TS_MemCpy( XCP_APPL_GET_ADDRESS( 0U, (P2VAR(void, AUTOMATIC, XCP_APPL_DATA)) Xcp_MTA.Address.Write ),
                 &A_Value,
                 XCP_MODIFY_VALUE_SIZE
               );
      SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      CalloutResForWrite = XCP_APPL_OK;
#endif /* (XCP_WRITE_READ_MEMORY_RAM == STD_ON) */
      switch( CalloutResForWrite )
      {
        case XCP_APPL_OK:
          /* Prepare CTO with positive response */
          ResponseCTO->CTO[XCP_RES_PID_INDEX] = XCP_RES_PID;
          ResponseCTO->Length = XCP_CMD_MODIFY_BITS_RES_LENGTH;
          break;
        /* !LINKSTO Xcp.CalibrationCommands.ApplBusy, 1 */
        case XCP_APPL_BUSY:
          RetValue = XCP_E_BUSY;
          break;
        case XCP_APPL_ERR_OUT_OF_RANGE:
          /* Prepare error CTO with negative response XCP_APPL_ERR_OUT_OF_RANGE */
          XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
          break;
        default:
          /* EB specific handling :*/
          /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
          XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
          break;
      } /* switch(CalloutResForWrite) */
    }
    /* !LINKSTO Xcp.CalibrationCommands.ApplBusy, 1 */
    else if( CalloutResForRead == XCP_APPL_BUSY)
    {
      RetValue = XCP_E_BUSY;
    }
    else if( CalloutResForRead == XCP_APPL_ERR_OUT_OF_RANGE )
    {
      /* Prepare error CTO with negative response XCP_APPL_ERR_OUT_OF_RANGE */
      XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_OUT_OF_RANGE, XCP_ERR_OUT_OF_RANGE_LENGTH);
    }
    else if( CalloutResForRead == XCP_APPL_ERR_ACCESS_DENIED )
    {
      /* Prepare error CTO with negative response ERR_ACCESS_DENIED */
      XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_ACCESS_DENIED, XCP_ERR_ACCESS_DENIED_LENGTH);
    }
    else
    {
      /* Prepare error CTO with negative response ERR_CMD_UNKNOWN */
      XCP_ERROR_RESPONSE((*ResponseCTO), XCP_ERR_CMD_UNKNOWN, XCP_ERR_CMD_UNKNOWN_LENGTH);
    }
  }
  return RetValue;
}
#endif
#endif

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>
#endif /* ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) ) */

/*==================[end of file]===========================================*/
