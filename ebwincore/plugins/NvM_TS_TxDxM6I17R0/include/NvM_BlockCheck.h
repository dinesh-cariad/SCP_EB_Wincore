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
#ifndef NVM_BLOCKCHECK_H
#define NVM_BLOCKCHECK_H
/*==================[inclusions]============================================*/
#include <TSAutosar.h>            /* EB specific standard types             */
                                  /* standard types                         */
#include <NvM_Int_Types.h>        /* NvM specific internal types            */
#include <NvM_Cfg.h>                  /*configuration header*/


#if (NVM_BLOCK_CHECK_ENABLED == STD_ON)
/*==================[internal data]==========================================*/

#define NVM_START_SEC_VAR_INIT_8
#include <NvM_MemMap.h>
/** \brief  Internal flag to enable/disable the block check mechanism.
 **/
extern VAR( uint8,NVM_APPL_DATA ) NvM_BcGlobalDisableFlag;

#define NVM_STOP_SEC_VAR_INIT_8
#include <NvM_MemMap.h>

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
/*==================[functions]================================================*/

/** \brief  Initialize the BlockCheck's internal variables
 **
 ** \retval :  void
 **
 ** \param[in] void
 **/
extern FUNC(void,NVM_CODE) NvM_BcInit(void);

/** \brief  The main function for the BlockCheck mechanism. In this function,
 **         similar to normal NvM handling, the state machine functions are
 **         called in a while loop.
 **
 ** \retval :  void
 **
 ** \param[in] void
 **/
extern FUNC(void,NVM_CODE) NvM_BcMainFuncProcess(void);

/** \brief  First cancel the Current actions of the underlying devices and
 **         then resets the BC state machine.
 **
 ** \retval :  void
 **
 ** \param[in] void
 **/
extern FUNC(void,NVM_CODE) NvM_BcResetCurrentProcess(void);

/** \brief  Loops trough all the possible BC blocks configured and does the
 **         corresponding queue handling : Increments counter/ Inserts in queue
 **         when counter elapsed / Removes from queue and shuts OFF an BC element
 **         when it was disabled / Removes from queue and resets the count when a
 **         BC element was cancelled.
 **
 ** \retval :  void
 **
 ** \param[in] void
 **/
extern FUNC(void,NVM_CODE) NvM_BcProcessQueue(void);



/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* NVM_BLOCK_CHECK_ENABLED == STD_ON */
#endif /* ifndef NVM_BLOCKCHECK_H */

/*=================[end of file NvM_Wrapper.h]=======================*/
