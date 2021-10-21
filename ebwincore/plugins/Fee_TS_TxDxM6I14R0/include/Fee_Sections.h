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

#ifndef FEE_SECTIONS_H
#define FEE_SECTIONS_H

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


/** \brief State entry function to initiate the flash write to mark the section header
 *         as Active.
 */
extern FUNC(void, FEE_CODE) Fee_SfOnEntryMarkHeaderActive(void);

/** \brief State entry function to initiate the flash write to mark the section footer
 *         as Active.
 */
extern FUNC(void, FEE_CODE) Fee_SfOnEntryMarkFooterActive(void);

/** \brief State function to check if the flash write to mark the section header or footer
 **        as Active was successful.
 **
 **        If the usage of consistency patterns is disabled, the state function initializes
 **        the switch section operation if the header write is not during startup. Otherwise it
 **        starts the queued job which triggered the header write.
 **
 **/
extern FUNC(void, FEE_CODE) Fee_SfMarkHeaderFooterActive(void);

/** \brief State entry function to initiate the flash write to mark the section header
 *         as Erasable.
 */
extern FUNC(void, FEE_CODE) Fee_SfOnEntryMarkHeaderErasable(void);

/** \brief State function to check if the flash write to mark the section header
 **        as Erasable was successful.
 **
 **        If the usage of consistency patterns is disabled, the state function initializes
 **        the switch section operation if the header write is not during startup. Otherwise it
 **        starts the queued job which triggered the header write.
 **/
extern FUNC(void, FEE_CODE) Fee_SfMarkHeaderErasable(void);

/** \brief State entry function to initiate the flash write to mark the section footer
 *         as Erasable.
 */
extern FUNC(void, FEE_CODE) Fee_SfOnEntryMarkFooterErasable(void);

/** \brief State function to check if the flash write to mark the section footer
 **        as Erasable was successful.
 **
 **        If the usage of consistency patterns is disabled, the state function initializes
 **        the switch section operation if the header write is not during startup. Otherwise it
 **        starts the queued job which triggered the header write.
 **/
extern FUNC(void, FEE_CODE) Fee_SfMarkFooterErasable(void);


/** \brief State entry function to initiate the flash write to store in the header
 *         the number of erase cycles of the section, together with a Crc
 *         computed over the counter and ConfigId.
 */
extern FUNC(void, FEE_CODE) Fee_SfOnEntryWriteSectionHeader(void);

/** \brief State function to check if the flash write to store
 **        the number of erase cycles of the section was successful.
 **
 **        If the usage of consistency patterns is enabled, the transition to write the section
 **        Erase Counter end pattern is triggered.
 **/
extern FUNC(void, FEE_CODE) Fee_SfWriteSectionHeader(void);

/** \brief State entry function to initiate the flash write to store in the footer
 *         the number of erase cycles of the section, together with a Crc
 *         computed over the counter and ConfigId.
 */
extern FUNC(void, FEE_CODE) Fee_SfOnEntryWriteSectionFooter(void);

/** \brief State function to check if the flash write to store
 **        the number of erase cycles of the section was successful.
 **
 **        If the usage of consistency patterns is enabled, the transition to write the section
 **        Erase Counter end pattern is triggered.
 **/
extern FUNC(void, FEE_CODE) Fee_SfWriteSectionFooter(void);

/** \brief State entry function to initiate flash erase */
extern FUNC(void, FEE_CODE) Fee_SfOnEntryEraseSection(void);

/** \brief State function to check if the flash erase was successful.
 *         If an immediate write is in queue the erase job is cancelled and
 *         write job is started. Otherwise, if the flash erase was successful,
 *         any pending job is executed or Fee goes to idle state.
 */
extern FUNC(void, FEE_CODE) Fee_SfEraseSection(void);

#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)

/** \brief State entry function to initiate writing the Erase Counter field end pattern.
 **/
extern FUNC(void, FEE_CODE) Fee_SfOnEntryWriteHeaderSectionManagementEndPattern(void);

/** \brief State function to check if writing the Erase Counter field end pattern was successful.
 **/
extern FUNC(void, FEE_CODE) Fee_SfWriteHeaderSectionManagementEndPattern(void);

/** \brief State entry function to initiate writing the Erase Counter field end pattern.
 **/
extern FUNC(void, FEE_CODE) Fee_SfOnEntryWriteFooterSectionManagementEndPattern(void);

/** \brief State function to check if writing the Erase Counter field end pattern was successful.
 **/
extern FUNC(void, FEE_CODE) Fee_SfWriteFooterSectionManagementEndPattern(void);
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


#endif /* ifndef FEE_SECTIONS_H */
