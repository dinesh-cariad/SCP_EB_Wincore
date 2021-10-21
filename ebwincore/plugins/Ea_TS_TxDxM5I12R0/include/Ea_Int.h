#ifndef EA_INT_H
#define EA_INT_H

/**
 * \file
 *
 * \brief AUTOSAR Ea
 *
 * This file contains the implementation of the AUTOSAR
 * module Ea.
 *
 * \version 5.12.14
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/
#include <Ea.h>

#if (EA_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>          /* Det API */
#endif

/*==================[macros]================================================*/

#if (defined EA_VLE_CURRENT_COPY_UNKNOWN)
#error EA_VLE_CURRENT_COPY_UNKNOWN is already defined
#endif
/** \brief Used to indicate that the current copy in unknown in
 **        Ea_VLEBlockInfoType. */
/* The macro is needed for test purpose, hence it is defined here */
#define EA_VLE_CURRENT_COPY_UNKNOWN 0U

/*--------------------------------------------------------------------------*/

#if (defined EA_INVALID_PATTERN)
#error EA_INVALID_PATTERN is already defined
#endif
/** \brief invalid pattern */
/* The macro is needed for test purpose, hence it is defined here */
#define EA_INVALID_PATTERN       0xAAU

#if (defined EA_INCONSISTENT_PATTERN)
#error EA_INCONSISTENT_PATTERN is already defined
#endif
/** \brief inconsistent pattern */
/* The macro is needed for test purpose, hence it is defined here */
#define EA_INCONSISTENT_PATTERN  0x55U

/*--------------------------------------------------------------------------*/
#if (defined EA_S_WRITE_START_MARKER)
#error EA_S_WRITE_START_MARKER is already defined
#endif
/** \brief ID of Ea state WriteStartMarker */
#define EA_S_WRITE_START_MARKER 0U

#if (defined EA_S_WRITE_DATA)
#error EA_S_WRITE_DATA is already defined
#endif
/** \brief ID of Ea state WriteData */
#define EA_S_WRITE_DATA         1U

#if (defined EA_S_WRITE_END_MARKER)
#error EA_S_WRITE_END_MARKER is already defined
#endif
/** \brief ID of Ea state WriteEndMarker */
#define EA_S_WRITE_END_MARKER   2U

#if (defined EA_S_WRITE_FINAL)
#error EA_S_WRITE_FINAL is already defined
#endif
/** \brief ID of Ea state WriteFinal */
#define EA_S_WRITE_FINAL        3U

#if (defined EA_S_READ_START_MARKER)
#error EA_S_READ_START_MARKER is already defined
#endif
/** \brief ID of Ea state ReadStartMarker */
#define EA_S_READ_START_MARKER  4U

#if (defined EA_S_READ_END_MARKER)
#error EA_S_READ_END_MARKER is already defined
#endif
/** \brief ID of Ea state ReadEndMarker */
#define EA_S_READ_END_MARKER    5U

#if (defined EA_S_READ_DATA)
#error EA_S_READ_DATA is already defined
#endif
/** \brief ID of Ea state ReadData */
#define EA_S_READ_DATA          6U

#if (defined EA_S_READ_CHECK_MARKER)
#error EA_S_READ_CHECK_MARKER is already defined
#endif
/** \brief ID of Ea state ReadCheckMarker */
#define EA_S_READ_CHECK_MARKER  7U

#if (defined EA_S_ERASE_IMMEDIATE)
#error EA_S_ERASE_IMMEDIATE is already defined
#endif
/** \brief ID of Ea state EraseImmediate */
#define EA_S_ERASE_IMMEDIATE    8U

#if (defined EA_S_INVALIDATE_ERASE)
#error EA_S_INVALIDATE_ERASE is already defined
#endif
/** \brief ID of Ea state InvalidateErase */
#define EA_S_INVALIDATE_ERASE   9U

#if (defined EA_S_INVALIDATE)
#error EA_S_INVALIDATE is already defined
#endif
/** \brief ID of Ea state Invalidate */
#define EA_S_INVALIDATE         10U

#if (defined EA_S_FINAL)
#error EA_S_FINAL is already defined
#endif
/** \brief ID of Ea state Final */
#define EA_S_FINAL              11U

#if (defined EA_S_VLE_START_MARKER)
#error EA_S_VLE_START_MARKER is already defined
#endif
/** \brief ID of Ea state VleStartMarker */
#define EA_S_VLE_START_MARKER   12U

#if (defined EA_S_VLE_END_MARKER)
#error EA_S_VLE_END_MARKER is already defined
#endif
/** \brief ID of Ea state VleEndMarker */
#define EA_S_VLE_END_MARKER     13U

#if (defined EA_S_VLE_FINAL)
#error EA_S_VLE_FINAL is already defined
#endif
/** \brief ID of Ea state VleFinal */
#define EA_S_VLE_FINAL          14U

#if (defined EA_S_IDLE)
#error EA_S_IDLE is already defined
#endif
/** \brief ID of Ea state Idle (does not correspond to a state function) */
#define EA_S_IDLE               254U

#if (defined EA_S_UNINIT)
#error EA_S_UNINIT is already defined
#endif
/** \brief ID of Ea state Uninit (does not correspond to a state function) */
#define EA_S_UNINIT             255U

#if (defined EA_NUM_STATE_FUNCTIONS)
#error EA_NUM_STATE_FUNCTIONS is already defined
#endif
/** \brief number of real state functions
 *
 * Equals number of states minus 2 for the state IDLE and UNINIT which have no
 * corresponding state processing functions */
#define EA_NUM_STATE_FUNCTIONS  15U

/*==================[type definitions]======================================*/

/** \brief internal states of Ea
 *
 * The defines EA_S_* shall be used to denote the values for this data type */
typedef uint8 Ea_MainStateType;

/** \brief Structure used to hold the configuration for a block (ROM). */
typedef struct
{
  Eep_AddressType StartAddress; /**< Start address */
  Eep_LengthType BlockPageSize; /**< Block size with full pages (bytes) */
  uint16 UserDataSize;
    /**< Size of user data (bytes).
         Data-type uint16 is used inplace of Eep_LengthType as value
         of configuration parameter EaBlockSize ranges from 1..65535 */
  uint8 VLECopiesNum;           /**< Number of data copy in this block */
  boolean ImmediateBlock;       /**< Indicates if this block can be erased */
} Ea_BlockConfigType;

/** \brief Structure used to hold VLE information for a block (RAM). */
typedef struct
{
  uint8 CurrentCopy;                  /**< Current copy [1 254] */
  uint8 CurrentVLECounterValue;       /**< Current VLEC [0 255] */
  boolean BlockConsistent;            /**< Consistency of the block */
} Ea_VLEBlockInfoType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define EA_START_SEC_VAR_INIT_8
#include <Ea_MemMap.h>

/** \brief internal temporary buffer used for copying the block */
extern VAR(uint8, EA_VAR) Ea_TempBuffer[EA_TEMPBUFFER_LENGTH];

#define EA_STOP_SEC_VAR_INIT_8
#include <Ea_MemMap.h>

#define EA_START_SEC_VAR_INIT_UNSPECIFIED
#include <Ea_MemMap.h>

/** \brief internal state variable */
extern VAR(Ea_MainStateType, EA_VAR) Ea_MainState;

/** \brief Current Status of EA Module */
extern VAR(MemIf_StatusType, EA_VAR) Ea_Status;

#define EA_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Ea_MemMap.h>


#define EA_START_SEC_CONST_16
#include <Ea_MemMap.h>

/** \brief Array used to validate given block numbers and find their
 **        corresponding index in Ea_BlockConfig and Ea_VLEBlockInfo. */
extern CONST(uint16, EA_CONST) Ea_BlockNumberList[EA_NUMBEROFBLOCKS];

#define EA_STOP_SEC_CONST_16
#include <Ea_MemMap.h>

#define EA_START_SEC_VAR_INIT_UNSPECIFIED
#include <Ea_MemMap.h>

/** \brief Current State: current job result. */
extern VAR(MemIf_JobResultType, EA_VAR) Ea_JobResult;

/** \brief VLE Information, modified during runtime.
 **        Defined in configuration. */
extern VAR(Ea_VLEBlockInfoType, EA_VAR) Ea_VLEBlockInfo[EA_NUMBEROFBLOCKS];

#define EA_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Ea_MemMap.h>

#define EA_START_SEC_CONST_UNSPECIFIED
#include <Ea_MemMap.h>

/** \brief Configuration of each block. */
extern CONST(Ea_BlockConfigType, EA_CONST) Ea_BlockConfig[EA_NUMBEROFBLOCKS];

#define EA_STOP_SEC_CONST_UNSPECIFIED
#include <Ea_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef EA_INT_H */
/*==================[end of file]===========================================*/
