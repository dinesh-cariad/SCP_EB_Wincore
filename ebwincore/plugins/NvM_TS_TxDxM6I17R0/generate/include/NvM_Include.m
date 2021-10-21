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
[!/* !LINKSTO NVM544,1 */!]
[!/*
=== define maximum space needed for CRC ===
*/!][!IF "count(NvMBlockDescriptor/*[(NvMBlockUseCrc = 'true') and (NvMBlockCrcType='NVM_CRC32')]) > 0"!][!/*
   */!][!VAR "maxCrcSize"="4"!][!/*
*/!][!ELSEIF "count(NvMBlockDescriptor/*[(NvMBlockUseCrc = 'true') and (NvMBlockCrcType='NVM_CRC16')]) > 0"!][!/*
   */!][!VAR "maxCrcSize"="2"!][!/*
*/!][!ELSE!][!/*
   */!][!VAR "maxCrcSize"="1"!][!/*   
*/!][!ENDIF!][!/*

=== calculate the maximum space needed for internal buffer ===
  */!][!VAR "crcSize" = "0"!][!/*
  */!][!VAR "ExtraCryptoSize" = "0"!][!/*
  */!][!VAR "blockIdSize" = "0"!][!/*
  */!][!VAR "maxBlockSize"="0"!][!/* 
  */!][!LOOP "NvMBlockDescriptor/*[((NvMBlockUseCrc = 'true') and (NvMUserProvidesSpaceForBlockAndCrc != 'true')) or 
                                    (NvMWriteVerification = 'true') or 
                                    (NvMPreWriteDataComp = 'true') or 
                                    (NvMEnBlockCheck = 'true') or 
                                    (NvMStaticBlockIDCheck = 'true') or 
                                    ((NvMEnableBlockCryptoSecurityHandling = 'true') and
                                    (../../NvMCommon/NvMCommonCryptoSecurityParameters/NvMEnableCryptoSecurityHooks = 'true')) or 
                                    (NvMBlockUseSyncMechanism = 'true') or 
                                    (NvMBlockManagementType = 'NVM_BLOCK_REDUNDANT') or 
                                    (NvMBlockManagementType = 'NVM_BLOCK_DATASET')]"!][!/*
  */!][!IF "NvMBlockUseCrc = 'true'"!][!/*
    */!][!IF "NvMBlockCrcType = 'NVM_CRC32'"!][!/*
      */!][!VAR "crcSize"="4"!][!/* 
      */!][!ELSEIF "NvMBlockCrcType = 'NVM_CRC16'"!][!/*
      */!][!VAR "crcSize"="2"!][!/* 
    */!][!ELSE!][!/*
      */!][!VAR "crcSize"="1"!][!/* 
    */!][!ENDIF!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "../../NvMCommon/NvMCommonCryptoSecurityParameters/NvMEnableCryptoSecurityHooks = 'true'"!][!/*
  */!][!IF "NvMEnableBlockCryptoSecurityHandling = 'true'"!][!VAR "ExtraCryptoSize"="NvMCryptoExtraInfoSize"!][!ENDIF!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "NvMStaticBlockIDCheck = 'true'"!][!VAR "blockIdSize"="2"!][!ENDIF!][!/*
  */!][!VAR "blockSize"="NvMNvBlockLength + $blockIdSize + $crcSize + $ExtraCryptoSize"!][!/*
  */!][!IF "$blockSize > $maxBlockSize"!][!/*
     */!][!VAR "maxBlockSize"="$blockSize"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

=== calculate the space needed for RAM CRC buffer ===
*/!][!VAR "CrcBufSize" = "0"!][!/*
*/!][!LOOP "NvMBlockDescriptor/*[(NvMBlockUseCrc = 'true') and (NvMCalcRamBlockCrc = 'true')]"!][!/*
   */!][!IF "NvMBlockCrcType = 'NVM_CRC8'"!][!/*
      */!][!VAR "CrcBufSize" = "$CrcBufSize + 1"!][!/*
      */!][!ELSEIF "NvMBlockCrcType = 'NVM_CRC16'"!][!/*
      */!][!VAR "CrcBufSize" = "$CrcBufSize + 2"!][!/*
   */!][!ELSE!][!/*
      */!][!VAR "CrcBufSize" = "$CrcBufSize + 4"!][!/*
   */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

=== calculate the space needed for NV CRC buffer ===
*/!][!VAR "NvCrcBufSize" = "0"!][!/*
*/!][!LOOP "NvMBlockDescriptor/*[(NvMBlockUseCRCCompMechanism = 'true') and (NvMBlockUseCrc = 'true')]"!][!/*
   */!][!IF "NvMBlockCrcType = 'NVM_CRC8'"!][!/*
      */!][!VAR "NvCrcBufSize" = "$NvCrcBufSize + 1"!][!/*
      */!][!ELSEIF "NvMBlockCrcType = 'NVM_CRC16'"!][!/*
      */!][!VAR "NvCrcBufSize" = "$NvCrcBufSize + 2"!][!/*
   */!][!ELSE!][!/*
      */!][!VAR "NvCrcBufSize" = "$NvCrcBufSize + 4"!][!/*
   */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

=== calculate the space needed for RAM Write verification buffer ===
*/!][!VAR "writeVerificationBufferSize"="0"!][!/*
*/!][!VAR "preWriteDataCompBufferSize"="0"!][!/*
*/!][!VAR "maxVerificationSize"="0"!][!/*
*/!][!IF "count(NvMBlockDescriptor/*[(NvMWriteVerification = 'true')])>0"!][!/* 
  */!][!LOOP "NvMBlockDescriptor/*[(NvMWriteVerification = 'true')]"!][!/*
  */!][!VAR "writeVerificationBufferSize"="NvMWriteVerificationDataSize"!][!/*
  */!][!IF "$writeVerificationBufferSize > $maxVerificationSize"!][!/*
     */!][!VAR "maxVerificationSize"="$writeVerificationBufferSize"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!ENDIF!]
[!VAR "NVM_DeviceIdList"="''"!][!/*
*/!][!VAR "NVM_NrOfDevices"="0"!][!/*
*/!][!LOOP "as:modconf('NvM')[1]/NvMBlockDescriptor/*"!][!/*
  */!][!VAR "Found"="'false'"!][!/*
  */!][!VAR "DeviceId"="node:value(NvMNvramDeviceId)"!][!/*
  */!][!LOOP "text:split($NVM_DeviceIdList,';')"!][!/*
    */!][!IF "((.) = $DeviceId)"!][!/*
     */!][!VAR "Found"="'true'"!][!/*
  */!][!ENDIF!][!/*
  */!][!ENDLOOP!][!/*
  */!][!IF "($Found = 'false')"!][!/*
    */!][!VAR "NVM_DeviceIdList"!][!"$NVM_DeviceIdList"!][!"NvMNvramDeviceId"!];[!ENDVAR!][!/*
    */!][!VAR "NVM_NrOfDevices"="$NVM_NrOfDevices + 1"!][!/*
    */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!IF "count(NvMBlockDescriptor/*[(NvMPreWriteDataComp = 'true')])>0"!][!/* 
  */!][!LOOP "NvMBlockDescriptor/*[(NvMPreWriteDataComp = 'true')]"!][!/*
  */!][!VAR "preWriteDataCompBufferSize"="NvMPreWriteDataCompDataSize"!][!/*
  */!][!IF "$preWriteDataCompBufferSize > $maxVerificationSize"!][!/*
     */!][!VAR "maxVerificationSize"="$preWriteDataCompBufferSize"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!ENDIF!][!/*

=== calculate the maximum space needed for intermediate buffer ===
  */!][!VAR "intcrcSize" = "0"!][!/*
  */!][!VAR "intblockIdSize" = "0"!][!/*
  */!][!LOOP "NvMBlockDescriptor/*[(NvMEnBlockCheck = 'true')]"!][!/*
  */!][!IF "NvMBlockUseCrc = 'true'"!][!/*
    */!][!IF "NvMBlockCrcType = 'NVM_CRC32'"!][!/*
      */!][!VAR "intcrcSize"="4"!][!/* 
      */!][!ELSEIF "NvMBlockCrcType = 'NVM_CRC16'"!][!/*
      */!][!VAR "intcrcSize"="2"!][!/* 
    */!][!ELSE!][!/*
      */!][!VAR "intcrcSize"="1"!][!/* 
    */!][!ENDIF!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "NvMStaticBlockIDCheck = 'true'"!][!VAR "intblockIdSize"="2"!][!ENDIF!][!/*
  */!][!VAR "intblockSize"="NvMNvBlockLength + $intblockIdSize + $intcrcSize"!][!/*
  */!][!IF "$intblockSize > $maxVerificationSize"!][!/*
     */!][!VAR "maxVerificationSize"="$intblockSize"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!]