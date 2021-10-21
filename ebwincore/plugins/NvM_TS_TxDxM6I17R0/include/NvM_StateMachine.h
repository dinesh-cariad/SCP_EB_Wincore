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
#ifndef NVM_STATE_MACHINE_LIB_H
#define NVM_STATE_MACHINE_LIB_H

/*==================[inclusions]============================================*/

#include <Std_Types.h>            /* AUTOSAR standard types                 */
#include <NvM_Int_Types.h>        /* NvM specific internal types            */

/*==================[external function declarations]========================*/
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

/** \brief Checks if a new service is available in the Immediate Queue or in
 **        the Standard Queue. If a service is available, it initializes
 **        NvM_GlobalBlockNumber and the first state of the state machine
 **        appropriately.
 **/
extern FUNC( void,NVM_CODE ) NvM_StateMachine_SetNextRequest( void );

#if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ))

/** \brief Starts the write job for an immediate block.
 **        In case a standard priority job is canceled by
 **        an immediate priority job this function is called
 **        to start the immediate job.
 **/
extern FUNC( void,NVM_CODE ) NvM_StateMachine_WriteImmediateBlock( void );

#endif /* #if ( ( NVM_JOB_PRIORITIZATION == STD_ON ) && ( NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS > 0U ))*/

/** \brief Evaluates the job result reported by the underlying drivers.
 **        It is used by NvM_InvalidateNvBlock() and NvM_EraseNvBlock().
 **/
extern FUNC( void,NVM_CODE ) NvM_StateMachine_ProcessJobResult( void );

/** \brief Prepares the state machine function pointers for processing
 **        redandant blocks. Used by NvM_WriteBlock() and NvM_InvalidateNvBlock().
 **/
extern FUNC( void,NVM_CODE ) NvM_StateMachine_ProcessRedundantBlock
(
  NvM_CurrentFunctionPtrType NextState
);

/** \brief Performs error handling by reporting the error to Dem.
 **        It is used by NvM_EraseNvBlock() and NvM_InvalidateNvBlock().
 **/
extern FUNC( void,NVM_CODE ) NvM_StateMachine_RedundantBlockError( void );

/** \brief Terminates the current call level of the asynchronous state machines.
 **        The currently processed sub state machine will be terminated.
 **/
extern FUNC( void,NVM_CODE ) NvM_StateMachine_TerminateCurrentLevel( void );

/** \brief Terminates the current two top call levels of the state machine.
 **        The two sub state machine on top of the call stack are terminated.
 **/
extern FUNC( void,NVM_CODE ) NvM_StateMachine_TerminateSubLevel( void );

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef NvM_StateMachine_Lib.h */

/*==================[end of file NvM_StateMachine_Lib.h]====================*/
