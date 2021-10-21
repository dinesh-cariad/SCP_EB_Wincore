/**
 * \file
 *
 * \brief AUTOSAR Fee
 *
 * This file contains the implementation of the AUTOSAR
 * module Fee.
 *
 * \version 6.14.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/
/* !LINKSTO FEE084,1 */
#include <Std_Types.h>    /* Autosar standard type definitions */
#include <Fee_Internal.h>      /* Fee internal header file */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

#define FEE_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <MemMap.h>

[!INCLUDE "../include/Fee_Checks.m"!][!//
 [!VAR "SectorIndex" = "0"!][!//
 [!VAR "SectionIndex" = "0"!][!//
 [!VAR "NbOfSections" = "FeeGeneral/FeeNumberOfSections"!][!//
 [!VAR "SectionSize" = "0"!][!//
 [!VAR "SmallestSectionSize" = "$TotalFlashSize"!][!//
 [!VAR "SectionStartAddress" = "$Section0StartAddress"!][!//

 /* !LINKSTO Fee.Sections,1 */
 /* !LINKSTO FEE071,1, FEE103,1 */
 /* !LINKSTO Fee.Sections.Generation,1 */
 /* Sections configuration array */
CONST(Fee_SectionConfiguration_t, FEE_CONST) Fee_SectionCfg[FEE_NUMBER_OF_SECTIONS] =
{[!//
 [!IF "(as:modconf('Fee')[1]/FeeGeneral/FeeNumberOfSections = 2) and as:modconf('Fee')[1]/FeeGeneral/EnableAutoSectionGeneration = 'true'"!][!//
    [!VAR "SmallestSectionSize" = "$Section0size"!][!//
    [!IF "($Section1size < $SmallestSectionSize)"!][!//
        [!VAR "SmallestSectionSize" = "$Section1size"!][!//
    [!ENDIF!][!//
  {
    [!"num:integer($Section0StartAddress)"!]U,
    [!"num:integer($Section0size)"!]U
  },
  {
    [!"num:integer($Section1StartAddress)"!]U,
    [!"num:integer($Section1size)"!]U
  }
  [!ELSEIF "as:modconf('Fee')[1]/FeeGeneral/EnableAutoSectionGeneration = 'false'"!][!//
    [!VAR "TotalSectCount" = "count(FeeGeneral/FeeSectionConfiguration/*)"!][!//
    [!VAR "Index" = "0"!][!//
    [!LOOP "node:order(FeeGeneral/FeeSectionConfiguration/*,'node:value(FeeSectionNumber)')"!][!//
    [!IF "(FeeSectionSize < $SmallestSectionSize)"!][!//
        [!VAR "SmallestSectionSize" = "FeeSectionSize"!][!//
    [!ENDIF!][!//
    [!VAR "Index" = "($Index + 1)"!][!//
  {
    [!"FeeSectionStartAddress"!]U,
    [!"FeeSectionSize"!]U
  }[!IF "$Index = $TotalSectCount"!][!ELSE!],[!ENDIF!][!//
    [!ENDLOOP!][!//
  [!ELSE!][!//
    [!IF "node:exists(as:modconf('Fls')[1]/FlsConfigSet/*[1]/FlsSectorList/FlsSector)"!][!//
     [!LOOP "as:modconf('Fls')[1]/FlsConfigSet/*[1]/FlsSectorList/FlsSector/*"!][!// !LINKSTO Fee.Sections.Generation,1 
       [!/* Check if FlsSectorOption parameter exists and if the current sector is a data flash sector OR use all configured flash sectors */!][!//
       [!IF "((($NumberOfDataSectors > 0) and (node:exists(FlsSectorOption)) and (FlsSectorOption = 'FLS_DF_SECTOR')) or ((not(node:exists(FlsSectorOption))) and ($NumberOfDataSectors = 0)))"!][!//
         [!FOR "i" = "1" TO "FlsNumberOfSectors"!][!//
             [!VAR "SectionSize" = "$SectionSize + FlsSectorSize"!][!//
             [!IF "($SectionSize > $GenericSectionSize)"!][!//
               [!ERROR "Available FLS Flash could not be split into equal sections. Please disable EnableAutoSectionGeneration and configure the FEE sections manually!"!][!//
               [!BREAK!]
             [!ELSEIF "$SectionSize = $GenericSectionSize"!][!//
                [!IF "($SectionSize < $SmallestSectionSize)"!][!//
                  [!VAR "SmallestSectionSize" = "$SectionSize"!][!//
                [!ENDIF!]
  {
    [!"num:integer($SectionStartAddress)"!]U,
    [!"num:integer($SectionSize)"!]U
  },[!//
             [!VAR "SectionIndex" = "$SectionIndex + 1"!][!//
             [!VAR "SectionStartAddress" = "$SectionStartAddress + $SectionSize"!][!//
             [!VAR "SectionSize" = "0"!][!//
             [!ELSE!][!//
             [!ENDIF!][!//
         [!ENDFOR!][!//
       [!ENDIF!][!//
     [!ENDLOOP!][!//
    [!ELSEIF "node:exists(as:modconf('Fls')[1]/FlsConfigSet/FlsSectorList/FlsSector)"!][!//
     [!LOOP "as:modconf('Fls')[1]/FlsConfigSet/FlsSectorList/FlsSector/*"!][!//
       [!/* Check if FlsSectorOption parameter exists and if the current sector is a data flash sector OR use all configured flash sectors */!][!//
       [!IF "((($NumberOfDataSectors > 0) and (node:exists(FlsSectorOption)) and (FlsSectorOption = 'FLS_DF_SECTOR')) or ((not(node:exists(FlsSectorOption))) and ($NumberOfDataSectors = 0)))"!][!//
         [!FOR "i" = "1" TO "FlsNumberOfSectors"!][!//
             [!VAR "SectionSize" = "$SectionSize + FlsSectorSize"!][!//
             [!IF "($SectionSize > $GenericSectionSize)"!][!//
               [!ERROR "Available FLS Flash could not be split into equal sections. Please disable EnableAutoSectionGeneration and configure the FEE sections manually!"!][!//
               [!BREAK!]
             [!ELSEIF "$SectionSize = $GenericSectionSize"!][!//
                [!IF "($SectionSize < $SmallestSectionSize)"!][!//
                  [!VAR "SmallestSectionSize" = "$SectionSize"!][!//
                [!ENDIF!]
  {
    [!"num:integer($SectionStartAddress)"!]U,
    [!"num:integer($SectionSize)"!]U
  },[!//
             [!VAR "SectionIndex" = "$SectionIndex + 1"!][!//
             [!VAR "SectionStartAddress" = "$SectionStartAddress + $SectionSize"!][!//
             [!VAR "SectionSize" = "0"!][!//
             [!ELSE!][!//
             [!ENDIF!][!//
         [!ENDFOR!][!//
       [!ENDIF!][!//
     [!ENDLOOP!][!//
    [!ELSE!][!//
    [!ERROR!]
    /* FlsSector not configured or incompatible Fee/Fls implementation */
    [!ENDERROR!]
    [!ENDIF!][!//
     [!IF "($SectionIndex != $NbOfSections - 1)"!][!//
      [!// [!ERROR "Available FLS Flash could not be split into equal sections. Please disable EnableAutoSectionGeneration and configure the FEE sections manually! 2"!][!//
     [!ENDIF!][!//
  [!ENDIF!][!//
};[!//
[!VAR "SectionManagementReservedSize" = "2 * $SectionManagementOverhead + $MarginSize"!][!//
[!IF "(FeeGeneral/FeeEnableSmallSectionSize = 'true')"!]
  [!IF "((num:integer(num:integer(FeeGeneral/FeeNumberOfSections) div 2)*$SmallestSectionSize) < ($BlocksTotalSizeAl + $SectionManagementReservedSize))"!]
    [!/* !LINKSTO Fee.FeeEnableSmallSectionSize.Size,1 */!][!//
    [!ERROR "In case FeeEnableSmallSectionSize is enabled. The total size of the blocks have to fit in the (total size of the sections / 2)!"!][!//
  [!ELSE!][!//
    [!WARNING "All the blocks can not be written in the smallest section."!]
  [!ENDIF!][!//
[!ELSEIF "(FeeGeneral/FeeEnableSmallSectionSize = 'false') and (($BlocksTotalSizeAl + $SectionManagementReservedSize) > $SmallestSectionSize)"!][!//
    [!ERROR "All the blocks can not be written in the smallest section. Lower the number of sections or lower the blocks size, or add more data flash!"!][!//
[!ELSE!][!//
    [!IF "(FeeGeneral/FeeEnableSmallSectionSize = 'false') and ($BlocksTotalSizeAl + $LargestBlockSizeInSection + $SectionManagementReservedSize) > $SmallestSectionSize"!][!//
        [!ERROR "All the blocks plus one can not be written in the smallest section. Lower the number of sections or lower the blocks size, or add more data flash!"!][!//
    [!ENDIF!][!//
    [!IF "node:exists(FeeGeneral/FeeNumberOfNotConfigBlocks) and node:exists(FeeGeneral/FeeWriteCustomApi) and (FeeGeneral/FeeWriteCustomApi = 'true')"!][!//
      [!/*We might have only one not-configured block bigger than the largest config block */!][!//
      [!VAR "LargestPossibleNotConfigBlock" = "num:integer(FeeGeneral/FeeDataSizeNotConfiguredBlocks)"!][!//
      [!VAR "LargestPossibleNotConfigBlockAlInSection"!][!INDENT "0"!][!//
        [!CALL "GetBlockTotalSizeInSection", "BlockDataSize" = "$LargestPossibleNotConfigBlock"!][!//
      [!ENDINDENT!][!ENDVAR!][!//
      [!IF "($BlocksTotalSizeAl + LargestPossibleNotConfigBlockAlInSection + $SectionManagementReservedSize) > $SmallestSectionSize"!][!//
        [!/* SmallestSectionSize is bigger than FEE block management data,
             because we have at least 1 config block and we checked that largest cfg block fits.
             There is no need for the following check here:
             IF "$SmallestSectionSize > ($BlocksTotalSizeAl + (StartPatternSize + WrittenMarkerSize + $BlockInfoTotalSizeAl) + $SectionManagementReservedSize)"!]
           */!][!//
          [!VAR "AvailableSizeInSmallestSection" = "$SmallestSectionSize - ($BlocksTotalSizeAl + (StartPatternSize + WrittenMarkerSize + $BlockInfoTotalSizeAl) + $SectionManagementReservedSize)"!][!//
          [!VAR "AvailableSizeInSmallestSection" = "num:integer(num:integer($AvailableSizeInSmallestSection div $VirPageSize) * $VirPageSize)"!][!//
          [!WARNING!][!INDENT "0"!][!//
            ATTENTION: All the blocks plus largest possible not configured block - [!"num:integer($LargestPossibleNotConfigBlock)"!] bytes - [!//
            can not be written into the smallest section. [!//
            If a not configured block will be bigger than [!"num:integer($AvailableSizeInSmallestSection)"!] bytes, [!//
            data will be lost.[!//
            [!ENDINDENT!][!//
          [!ENDWARNING!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
[!ENDIF!]

[!VAR "Index" = "0"!][!//
/* Blocks configuration array */
CONST(Fee_BlockConfiguration_t, FEE_CONST)Fee_BlockCfg[FEE_NO_OF_CONFIGURED_BLOCKS] =
{
[!VAR "TotalCount" = "count(FeeBlockConfiguration/*)"!][!//
[!LOOP "node:order(FeeBlockConfiguration/*,'node:value(FeeBlockNumber)')"!][!//
[!VAR "Index" = "($Index + 1)"!][!//
  {
    [!"FeeBlockSize"!]U,
[!/* !LINKSTO FEE005,1 */!][!//
    [!CALL "GetAlignedSize", "DataSize" = "FeeBlockSize", "Alignment"= "$VirPageSize"!]U,
    [!"FeeBlockNumber"!]U,
    [!IF "FeeImmediateData='true'"!]TRUE[!ELSE!]FALSE[!ENDIF!]
  }[!IF "$Index = $TotalCount"!][!ELSE!],[!ENDIF!]
[!ENDLOOP!][!//
};

#define FEE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <MemMap.h>

#if FEE_FLS_ALIGNED_32 == STD_OFF
#define FEE_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <MemMap.h>
#else
#define FEE_START_SEC_CONFIG_DATA_32
#include <MemMap.h>
#endif
[!IF "((node:exists(FeeGeneral/FeeConsistencyPattern)) and (FeeGeneral/FeeConsistencyPattern = 'true'))"!]
/** \brief Array containing consistency pattern value */
CONST(uint8, FEE_CONST) Fee_ConsistencyPattern[FEE_CONSISTENCY_PATTERN_SIZE] =
{
 [!FOR "i" = "1" TO "$VirPageSize"!][!//
 FEE_CONSISTENCY_MARKER_PATTERN,
 [!ENDFOR!][!//
};
[!ENDIF!]

 /** \brief Array containing consistency pattern value */
 CONST(uint8, FEE_CONST) Fee_WrittenMarkerPattern[FEE_WRITTEN_MARKER_SIZE] =
 {
  [!FOR "i" = "1" TO "$VirPageSize"!][!//
  FEE_WRITTEN_MARKER_PATTERN,
  [!ENDFOR!][!//
 };
#if FEE_FLS_ALIGNED_32 == STD_OFF
#define FEE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <MemMap.h>
#else
#define FEE_STOP_SEC_CONFIG_DATA_32
#include <MemMap.h>
#endif
/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*===========================================================================*/

/*==================[end of file Fee_Cfg.c]===========================================*/

