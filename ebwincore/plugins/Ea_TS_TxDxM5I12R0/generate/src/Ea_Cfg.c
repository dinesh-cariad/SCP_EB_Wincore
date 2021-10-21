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
#include <Ea.h>                 /* Module's public API */
#include <Ea_Int.h>             /* Module's internal API */

/*==================[macros]================================================*/
[!NOCODE!][!//
[!// Find the number of containers of the given size necessary to store a given amount of stuff.
[!MACRO "FindNecessaryContainerNum", "contentSize", "containerSize"!][!//
[!VAR "result" = "num:i($contentSize div $containerSize)"!][!//
[!IF "($result * $containerSize) < $contentSize"!][!VAR "result" = "num:i($result + 1)"!][!ENDIF!][!//
[!"$result"!][!//
[!ENDMACRO!][!//
[!// ---- Get and check general configuration parameters ----------------------------
[!VAR "virtualPageSize" = "EaGeneral/EaVirtualPageSize"!]
[!VAR "eepAllowedWriteCycles" = "as:modconf('Eep')[1]/EepPublishedInformation/EepAllowedWriteCycles"!]
[!VAR "BlockDataOverhead" = "2"!][!// one byte for VLEC Start Marker and one byte for VLEC End Marker
[!ENDNOCODE!][!//

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

#define EA_START_SEC_CONST_16

#include <Ea_MemMap.h>

/** \brief Array used to validate given block numbers and find their
 * corresponding index in Ea_BlockConfig and Ea_VLEBlockInfo. */
CONST(uint16, EA_CONST) Ea_BlockNumberList[EA_NUMBEROFBLOCKS] =
{
[!LOOP "node:order(EaBlockConfiguration/*, 'EaBlockNumber')"!][!//
  [!"EaBlockNumber"!]U,
[!ENDLOOP!][!//
};

#define EA_STOP_SEC_CONST_16
#include <Ea_MemMap.h>


#define EA_START_SEC_CONST_UNSPECIFIED
#include <Ea_MemMap.h>

/** \brief Configuration of each block.
 *
 * Each entry consists of: Block start address, User data size, Total size of
 * one copy, VLE copies number, Immediate block */
[!/* Value of EA_NUMBEROFBLOCKS is always greater than 0, checked in XDM file */!][!//
CONST(Ea_BlockConfigType, EA_CONST) Ea_BlockConfig[EA_NUMBEROFBLOCKS] =
{
[!VAR "thisBlockStartAddress" = "0"!][!//
[!LOOP "node:order(EaBlockConfiguration/*, 'EaBlockNumber')"!][!//
[!/* Find the number of necessary pages for this block */!][!//
[!VAR "thisBlockPageNum"!][!CALL "FindNecessaryContainerNum", "contentSize" = "EaBlockSize + $BlockDataOverhead", "containerSize" = "$virtualPageSize"!][!ENDVAR!][!//
[!/* Find the number of necessary copies for this block */!][!//
[!VAR "thisBlockCopiesNum"!][!CALL "FindNecessaryContainerNum", "contentSize" = "EaNumberOfWriteCycles", "containerSize" = "$eepAllowedWriteCycles"!][!ENDVAR!][!//
[!/* one additional page is required for the Invalid Flag IF. */!][!//
[!/* !LINKSTO EA005,1 */!][!//
[!VAR "thisCopyTotalSize" = "num:i(($thisBlockPageNum * $virtualPageSize) + $virtualPageSize)"!][!//
  { [!"num:inttohex($thisBlockStartAddress)"!]U, [!"num:i($thisCopyTotalSize)"!]U, [!"EaBlockSize"!]U,[!//
 [!"num:i($thisBlockCopiesNum)"!]U, [!IF "EaImmediateData = 'true'"!]TRUE[!ELSE!]FALSE[!ENDIF!] }, /* Block #[!"EaBlockNumber"!] */
[!VAR "thisBlockStartAddress" = "$thisBlockStartAddress + ($thisCopyTotalSize * $thisBlockCopiesNum)"!][!//
[!ENDLOOP!][!//
};

[!NOCODE!][!//
[!IF "node:exists(as:modconf('Eep')[1]/EepInitConfiguration/*/EepSize)"!][!//
[!// Check to ensure that memory available is sufficient for all blocks and their copies
[!IF "sum(as:modconf('Eep')[1]/EepInitConfiguration/*/EepSize) < $thisBlockStartAddress"!]
[!ERROR!] The EEPROM memory available (Configured memory = [!"sum(as:modconf('Eep')[1]/EepInitConfiguration/*/EepSize)"!] bytes) is not sufficient for all blocks and their copies in Ea (Required memory = [!"$thisBlockStartAddress"!] bytes).
Correct the EEPROM size at Eep/EepInitConfiguration/EepInitConfiguration/EepSize[!ENDERROR!][!//
[!ELSE!]
[!INFO!] Memory used by Ea = [!"$thisBlockStartAddress"!] bytes and the configured memory in EEPROM = [!"sum(as:modconf('Eep')[1]/EepInitConfiguration/*/EepSize)"!] bytes [!ENDINFO!]
[!ENDIF!]
[!ELSEIF "node:exists(as:modconf('Eep')[1]/EepInitConfiguration/EepSize)"!][!//
[!// Check to ensure that memory available is sufficient for all blocks and their copies
[!IF "sum(as:modconf('Eep')[1]/EepInitConfiguration/EepSize) < $thisBlockStartAddress"!]
[!ERROR!] The EEPROM memory available (Configured memory = [!"sum(as:modconf('Eep')[1]/EepInitConfiguration/EepSize)"!] bytes) is not sufficient for all blocks and their copies in Ea (Required memory = [!"$thisBlockStartAddress"!] bytes).
Correct the EEPROM size at Eep/EepInitConfiguration/EepInitConfiguration/EepSize[!ENDERROR!][!//
[!ELSE!]
[!INFO!] Memory used by Ea = [!"$thisBlockStartAddress"!] bytes and the configured memory in EEPROM = [!"sum(as:modconf('Eep')[1]/EepInitConfiguration/EepSize)"!] bytes [!ENDINFO!]
[!ENDIF!]
[!ELSE!][!//
[!ERROR!]
/* EepSize not configured or incompatible Ea/Eep implementation */
[!ENDERROR!]
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!//
#define EA_STOP_SEC_CONST_UNSPECIFIED
#include <Ea_MemMap.h>

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
