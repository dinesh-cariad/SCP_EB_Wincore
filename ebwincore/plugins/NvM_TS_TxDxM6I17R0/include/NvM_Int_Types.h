/**
 * \file
 *
 * \brief AUTOSAR NvM
 *
 * This file contains the implementation of the AUTOSAR
 * module NvM.
 *
 * \version 6.17.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef NVM_INT_TYPES_H
#define NVM_INT_TYPES_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>            /* EB specific standard types     */
                                  /* standard types                 */
#include <Std_Types.h>            /* AUTOSAR standard types. NVM550 */

/*==================[type definitions]============================================*/
/**  \brief Defines pointer to unint8 variable
 **
 **/
typedef P2VAR(uint8, TYPEDEF, NVM_APPL_DATA) NvM_PtrToApplDataType;

typedef P2VAR(uint8, TYPEDEF, NVM_VAR) NvM_PtrToVarType;

typedef P2CONST(uint8, TYPEDEF, NVM_CONST) NvM_PtrToConstType;

/**  \brief Defines the type for state functions of
 **         asynchronous state machine.
 **/
typedef P2FUNC( void,NVM_CODE,NvM_CurrentFunctionPtrType )( void );

/**  \brief Defines the bit mask of various user configuration information.
 **/
typedef uint16 NvM_ConstantStatusType;

/* !LINKSTO NVM508,1 */
/**  \brief Defines a bitmask which contains status
 **         information generated during block processing.
 **/
typedef uint16 NvM_DynamicStatusType;

/**  \brief Administrative information for each block which
 **         is changed at run time.
 **/
/* !LINKSTO NVM134,1 */
typedef struct
{
  /** \brief Result of the last API request concerning this block. */
  uint8 NvMResult;
  /** \brief Current request for the block */
  uint8 NvMCurrentServiceId;
  /** \brief Data set block index */
  /* !LINKSTO NVM144,1 */
  uint8 NvMDatasetBlockIndex;
  /** \brief Inidcates if a block is valid or not */
  uint8 NvMValid;
  /** \brief Additional error status information of the last API request
   **        concerning this block. */
  uint16 NvMDynStatus;
  /** \brief Defines extended status information */
  uint8 NvMExtendedStatus;
}NvM_AdminBlockType;

/**  \brief Defines the elements of the standard or immediate queue.
 **/
typedef struct
{
  /** \brief Pointer to the first state of the
   **        asynchronous state machine of an API request. */
  NvM_CurrentFunctionPtrType NvMRequestAsyncPtr;
  /** \brief Pointer to a temporary user data RAM block passed by an API request.  */
  P2VAR( uint8, TYPEDEF, NVM_APPL_DATA ) NvMTempRamBlockDataAddress;
  /** \brief Identifier of the block to be processed. */
  uint16 NvMBlockDescriptorIndex;
}NvM_QueueItemType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* NVM_INT_TYPES_H */

/*=========================[end of file NvM_Int_Types.h]========================*/
