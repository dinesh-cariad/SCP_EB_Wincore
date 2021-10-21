[!/**
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
 */!][!//
[!NOCODE!] 
[!/********************** Checking of Fee/Ea-Blocksizes vs. NvM-Blocksizes **********************************************/!][!//
  [!LOOP "NvMBlockDescriptor/*"!]
    [!VAR "crcSize" = "0"!]
    [!VAR "blockIdSize" = "0"!]
    [!VAR "ExtraCryptoInfoSize" = "0"!]
    [!IF "NvMBlockUseCrc = 'true' and node:exists(NvMBlockCrcType)"!]
       [!IF "NvMBlockCrcType = 'NVM_CRC32'"!]
        [!VAR "crcSize"="4"!]
       [!ELSEIF "NvMBlockCrcType = 'NVM_CRC16'"!]
        [!VAR "crcSize"="2"!]
       [!ELSE!]
        [!VAR "crcSize"="1"!]
       [!ENDIF!]
    [!ENDIF!]
    [!IF "NvMStaticBlockIDCheck = 'true'"!][!VAR "blockIdSize"="2"!][!ENDIF!]
    [!IF "../../NvMCommon/NvMCommonCryptoSecurityParameters/NvMEnableCryptoSecurityHooks = 'true'"!]
    [!IF "NvMEnableBlockCryptoSecurityHandling = 'true'"!][!VAR "ExtraCryptoInfoSize"="NvMCryptoExtraInfoSize"!][!ENDIF!]
    [!ENDIF!]
    [!VAR "reqBlkSize"="num:i(NvMNvBlockLength + $blockIdSize + $crcSize + $ExtraCryptoInfoSize)"!]

    [!IF "node:refvalid(NvMTargetBlockReference/NvMNameOfFeeBlock) and (NvMTargetBlockReference/NvMNameOfFeeBlock)!="""!]
      [!VAR "BlockNumber"="as:ref(./NvMTargetBlockReference/NvMNameOfFeeBlock)/FeeBlockNumber"!]
      [!VAR "NumberOfCopies"="./NvMNvBlockNum"!]
      [!LOOP "as:modconf('Fee')"!]
        [!LOOP "FeeBlockConfiguration/*"!]
          [!IF "((FeeBlockNumber >= $BlockNumber) and (FeeBlockNumber < ($BlockNumber + $NumberOfCopies)))"!]
             [!IF "FeeBlockSize < $reqBlkSize"!]
                [!ERROR!] Configured Block size of Fee Block Number:[!"FeeBlockNumber"!] is not valid. It should be [!"$reqBlkSize"!].[!ENDERROR!]
             [!ENDIF!]
          [!ENDIF!]
        [!ENDLOOP!]
      [!ENDLOOP!]
    [!ELSEIF "node:refvalid(NvMTargetBlockReference/NvMNameOfEaBlock) and (NvMTargetBlockReference/NvMNameOfEaBlock)!="""!]
      [!VAR "BlockNumber"="as:ref(NvMTargetBlockReference/NvMNameOfEaBlock)/EaBlockNumber"!]
      [!VAR "NumberOfCopies"="./NvMNvBlockNum"!]
      [!LOOP "as:modconf('Ea')"!]
        [!LOOP "EaBlockConfiguration/*"!]
            [!IF "((EaBlockNumber >= $BlockNumber) and (EaBlockNumber < ($BlockNumber + $NumberOfCopies)))"!]
               [!IF "EaBlockSize < $reqBlkSize"!]
                  [!ERROR!] Configured Block size of Ea Block Number:[!"EaBlockNumber"!] is not valid. It should be [!"$reqBlkSize"!].[!ENDERROR!]
               [!ENDIF!]
            [!ENDIF!]
        [!ENDLOOP!]
      [!ENDLOOP!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDNOCODE!]