/**
 * \file
 *
 * \brief AUTOSAR Fee
 *
 * This file contains the implementation of the AUTOSAR
 * module Fee.
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2017 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* This file contains function prototypes for Fee initialization */

#ifndef FEE_INITIALIZATION_H
#define FEE_INITIALIZATION_H

/*==================[inclusions]============================================*/
/* !LINKSTO FEE084,1, FEE002,1 */
#include <Std_Types.h>        /* AUTOSAR Standard type header*/
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define FEE_START_SEC_CODE
#include <MemMap.h>

/** \brief State entry function to initiate the read of section header from flash */
extern FUNC(void, FEE_CODE) Fee_SfOnEntryInitReadHeader(void);

/** \brief State function to set the section status after the
 *         section header is read from flash.
 */
extern FUNC(void, FEE_CODE) Fee_SfInitReadHeader(void);

/** \brief State entry function to initiate the read of section footer from flash */
extern FUNC(void, FEE_CODE) Fee_SfOnEntryInitReadFooter(void);

/** \brief State function to set the section status after the
 *         section footer is read from flash.
 */
extern FUNC(void, FEE_CODE) Fee_SfInitReadFooter(void);

/** \brief State entry function to initiate the read of block infos to fill the cache.
 */
extern FUNC(void, FEE_CODE) Fee_SfOnEntryInitFillCache(void);

/** \brief State function to check the status of reading block infos to fill the cache.
 *         It checks whether more block infos are present in the current section
 *         and initiate the read operation.
 *         It also checks if the other section also need to be read.
 */
extern FUNC(void, FEE_CODE) Fee_SfInitFillCache(void);

/** \brief State entry function to initiate the read of one block info at a time to fill the cache.
 */
extern FUNC(void, FEE_CODE) Fee_SfOnEntryInitBlockByBlock(void);

/** \brief State function to check the status of reading one block info at a time to fill the cache.
 *         If the read fail go to the next block info and attempt to cache it.
 *         After all block infos in a Fee buffer are read go back to FEE_INIT_FILL_CACHE.
 */
extern FUNC(void, FEE_CODE) Fee_SfInitBlockByBlock(void);

#define FEE_STOP_SEC_CODE
#include <MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* ifndef FEE_INITIALIZATION_H */
