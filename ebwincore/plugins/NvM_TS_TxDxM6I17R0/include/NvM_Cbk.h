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
#ifndef NVM_CBK_H
#define NVM_CBK_H

/*==================[inclusions]============================================*/

#include <Std_Types.h>        /* AUTOSAR standard types              */
#include <NvM_Version.h>      /* the module's version information    */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

/** \brief Function to be used by the underlying memory abstraction to signal
 ** end of job without error
 **
 ** This callback function can be used by an underlying module to signal
 ** the NVRAM Manager that it finished a job successfully. The NVRAM
 ** Manager additionally calls MemIf_GetJobResult() to evaluate the job
 ** result of the underlying module and only if MEMIF_JOB_OK is returned
 ** it continues with normal operation. Therefore all underlying
 ** modules must also set the appropriate job result before calling
 ** NvM_JobEndNotification(). The NvM_MainFunction() must be called
 ** cyclically although NvM_JobEndNotification() is called by an underlying
 ** module because if the configuration parameter NvMCrcNumOf-
 ** Bytes is less than the size of the currently processed NVRAM Block
 ** then only the cyclic NvM_MainFunction() can finish the CRC calculation.
 ** This function might be called from interrupt level. The execution
 ** time depends mainly on the execution time of the CRC calculation
 ** of the current NVRAM Block. Therefore if it is called from interrupt
 ** level the configuration parameter NvMCrcNumOfBytes shall be set
 ** appropriately so that the CRC calculation is interrupted and finished
 ** by the NvM_MainFunction() which is called from Task level.
 **
 ** \ServiceID{17}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC( void, NVM_CODE ) NvM_JobEndNotification( void );

/** \brief Function to be used by the underlying memory abstraction to signal
 ** end of job with error
 **
 ** This callback function can be used by an underlying module to signal
 ** the NVRAM Manager that it finished a job with an error. The NVRAM
 ** Manager additionally calls MemIf_GetJobResult() to evaluate the job
 ** result of the underlying module. This is necessary because depending
 ** on the kind of error different error recovery is done. Therefore all
 ** underlying modules must also set the appropriate job result before
 ** calling NvM_JobErrorNotification(). The NvM_MainFunction() must
 ** be called cyclically although NvM_JobEndNotification() is called by
 ** an underlying module because if the configuration parameter NvM-
 ** CrcNumOfBytes is less than the size of the currently processed
 ** NVRAM Block then only the cyclic NvM_MainFunction() can finish
 ** the CRC calculation. This function might be called from interrupt
 ** level. The execution time depends mainly on the execution time of
 ** the CRC calculation of the current NVRAM Block. Therefore if it is
 ** called from interrupt level the configuration parameter NvMCrcNumOfBytes
 ** shall be set appropriately so that the CRC calculation is
 ** interrupted and finished by the NvM_MainFunction() which is called
 ** from Task level.
 **
 ** \ServiceID{18}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC( void, NVM_CODE ) NvM_JobErrorNotification( void );

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

#endif /* ifndef NVM_CBK_H */
/*==================[end of file NvM_Cbk.h]=================================*/
