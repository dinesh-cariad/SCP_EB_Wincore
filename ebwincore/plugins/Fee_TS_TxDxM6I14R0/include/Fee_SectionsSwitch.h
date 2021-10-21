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
/* This file contains function prototypes for section operations */

#ifndef FEE_SECTIONSSWITCH_H
#define FEE_SECTIONSSWITCH_H

/*==================[inclusions]============================================*/
/* !LINKSTO FEE084,1, FEE002,1 */
#include <Std_Types.h>        /* AUTOSAR Standard type header*/
/* !LINKSTO FEE084,1, FEE002,1 */
#include <Fee_Cfg.h>          /* FEE configuration header */
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define FEE_START_SEC_CODE
#include <MemMap.h>

/** \brief State entry function to initiate writing the written marker
 *         after BlockInfo.
 */
extern FUNC(void, FEE_CODE) Fee_SfOnEntrySSWriteWrittenMarker(void);

/** \brief State function to check if writing of written marker was successful.
 **
 **        If the usage of consistency patterns is disabled, the state function resumes
 **        a pending switch section or erase operation. If no operation is pending,
 **        state is set to idle.
 **
 **        If the usage of consistency patterns is enabled, the transition to write the block
 **        data end pattern is triggered.
 */
extern FUNC(void, FEE_CODE) Fee_SfSSWriteWrittenMarker(void);

/** \brief State entry function to initiate reading block data from flash
 *         during section switch operation.
 */
extern FUNC(void, FEE_CODE) Fee_SfOnEntrySSReadData(void);

/** \brief State function to check the status of reading block data from flash
 *         during section switch operation.
 *         If an immediate job has been requested, the ongoing flash read is canceled
 *         and the write job is initiated. Otherwise, the block id at the end of data
 *         is checked for consistency and if the block is inconsistent, cache is update
 *         with inconsistent address, and next state is set to FEE_SS_COPY_INFO.
 */
extern FUNC(void, FEE_CODE) Fee_SfSSReadData(void);

/** \brief State entry function to prepare the buffer and write the block info to flash
 *         during section switch operation. If the flash write is accepted,
 *         the Info Address is updated.
 */
extern FUNC(void, FEE_CODE) Fee_SfOnEntrySSCopyInfo(void);

/** \brief State function to check the status of writing the block info to flash.
 *         If the block is marked as inconsistent in cache, the data block need
 *         not be copied. Otherwise the data block is copied by setting
 *         the next state to FEE_SS_COPY_DATA.
 */
extern FUNC(void, FEE_CODE) Fee_SfSSCopyInfo(void);

/** \brief State entry function to prepare the buffer and write the block data to flash
 *         during section switch operation. If the flash write is accepted,
 *         the Data Address is updated.
 */
extern FUNC(void, FEE_CODE) Fee_SfOnEntrySSCopyData(void);

/** \brief State function to check the status of writing the block data to flash.
 *         If copy of block data was successful, cache is updated and next block
 *         is taken for copying by calling Fee_SectionSwitching().
 */
extern FUNC(void, FEE_CODE) Fee_SfSSCopyData(void);

#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)
#if (FEE_CONSISTENCY_START_PATTERN_ENABLED == STD_ON)
/** \brief State entry function to initiate writing the block info start pattern at the next
 **        location in flash memory.
 **/
extern FUNC(void, FEE_CODE) Fee_SfOnEntrySSWriteBlockInfoStartPattern(void);

/** \brief State function to check if writing the block info start pattern was successful
 **        and trigger the transition to write block info.
 **/
extern FUNC(void, FEE_CODE) Fee_SfSSWriteBlockInfoStartPattern(void);

/** \brief State entry function to initiate writing the block data start pattern at the next
 **        location in flash memory.
 **/
extern FUNC(void, FEE_CODE) Fee_SfOnEntrySSWriteBlockDataStartPattern(void);

/** \brief State function to check if writing of block data start pattern was succesful
 **        and trigger the transition to write block data.
 **/
extern FUNC(void, FEE_CODE) Fee_SfSSWriteBlockDataStartPattern(void);
#endif/*FEE_CONSISTENCY_START_PATTERN_ENABLED*/
/** \brief State entry function to initiate writing the block info end pattern at the next
 **        location in flash memory.
 **/
extern FUNC(void, FEE_CODE) Fee_SfOnEntrySSWriteBlockInfoEndPattern(void);

/** \brief State function to check if writing the block info end pattern was successful
 **        and trigger the transition to write block data or complete the current request.
 **/
extern FUNC(void, FEE_CODE) Fee_SfSSWriteBlockInfoEndPattern(void);
#endif


#define FEE_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* ifndef FEE_SECTIONSSWITCH_H */
