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
#ifndef NVM_CALC_CRC_H
#define NVM_CALC_CRC_H
/*==================[inclusions]============================================*/

#include <TSAutosar.h>            /* EB specific standard types             */
                                  /* standard types                         */
#include <NvM_Int_Types.h>        /* NvM specific internal types            */
#include <NvM_Cfg.h>              /* Generated extern definitions of        */
                                  /* configuration data.                    */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

/** \brief Calculates remaining CRC data length.
 **        In case of interrupted CRC calculation this function determines the
 **        number of data bytes which must be included in the CRC calculation
 **        in the next step.
 **
 **
 ** \return Number of bytes to be calulated in the next step,
 **         range: 0 .. (2^32)-1
 **
 ** \remarks
 ** - The function is called by NvM_CalcCrc_AsyncState.
 */
extern FUNC(uint32, NVM_CODE) NvM_CalcCrc_UpdateLength(void);

/** \brief Returns the start address in the internal buffer where the CRC is stored.
 **
 ** \revtal address of the CRC buffer which must currently used by the
 **         NvM_ReadBlock() function.
 **         range: 0 .. size of NVM_APPL_DATA
 **
 **/
extern FUNC_P2VAR(uint8, AUTOMATIC, NVM_CODE) NvM_CalcCrc_GetBlockCrcAddress(void);

/** \brief Returns address of stored RAM CRC.
 **        This function returns the address where the CRC is
 **        stored in RAM for the current NVRAM block.
 **/
extern FUNC_P2VAR(uint8, AUTOMATIC, NVM_CODE) NvM_GetStoredRamCRCAddress(void);

#if (NVM_CRC_COMP_MECH == STD_ON)
/** \brief Returns address of stored NV CRC.
 **        This function returns the address where the CRC is
 **        stored in RAM for the current NVRAM block.
 **
 ** \return address of the NV data CRC buffer corresponding to he BlockId
 **          found in NvM_GlobalBlockNumber
 **/
extern FUNC_P2VAR(uint8, AUTOMATIC, NVM_CODE) NvM_GetStoredNvCRCAddress(void);
#endif /*NVM_CRC_COMP_MECH == STD_ON*/

/** \brief Calculates the CRC for the NVRAM Block.
 **        For the current block defined by NvM_CurrentBlockDescriptorPtr,
 **        CRC is calculated starting from DataAddress.
 **
 ** \param[in]  DataAddress: Start address of the data
 **/
extern FUNC(void, NVM_CODE) NvM_CalculateCrc
(
  NvM_PtrToApplDataType DataAddress
);


/** \brief Copies the CRC for the current NVRAM Block.
 **        For the block defined by BlockDesc,
 **        CRC is copied from NvM_SourceAddress to NvM_TargetAddress.
 **
 ** \param[in]  DataAddress: Start address to load CRC
 ** \param[in]  CrcAddress: CRC Address that shall be used as source.
 ** \param[in]  Block Descriptor
 **/
extern FUNC(void, NVM_CODE) NvM_CopyCrc
(
  NvM_PtrToApplDataType DataAddress,
  NvM_PtrToApplDataType CrcAddress,
  uint32 BlockDesc
);
/** \brief Calculates the CRC for the NVRAM Block by calling the
 **        corresponding CRC library API.
 **
 ** \param[in]  DataAddress: Start address to calculate CRC
 ** \param[in]  CrcAddress: Crc Calculation Buffer address
 ** \param[in]  Length On the data on which the CRC must be done
 ** \param[in]  NVM block descriptor to determine CRC type
 ** \param[in]  TRUE if first call/ FALSE - if consequent calls
 **
 **/
extern FUNC(void, NVM_CODE) NvM_CalcCrc_ProcessCrc
(
  NvM_PtrToApplDataType CrcDataAdress,
  NvM_PtrToApplDataType CalcCrcBuffer,
  uint32 Length,
  uint32 BlockDesc,
  boolean FirstCall
);

/** \brief Verifies calculated CRC with the stored CRC.
 **        For the current block defined by NvM_CurrentBlockDescriptorPtr,
 **        the calculated CRC is compared with the CRC stored at DataAddress.
 **
 ** \param[in]  DataAddress: Start address of the stored CRC
 ** \param[in]  DataAddress: Calculation buffer.
 ** \param[in]  Block Descriptor to determine CRC type.
 **
 ** \return    NvM_RequestResultType
 ** \retval    NVM_REQ_OK     Comparison successful
 ** \retval    NVM_REQ_NOT_OK Comparison failed
 **/
extern FUNC(NvM_RequestResultType, NVM_CODE) NvM_CheckCrc
(
  P2CONST(uint8, AUTOMATIC, NVM_APPL_DATA) DataAddress,
  NvM_PtrToApplDataType CrcCalcBuffer,
  uint32 BlockDesc
);
#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
/** \brief Add an element to the top
 **
 ** \param[in]  uint16 BlockId

 **/
extern FUNC(void, NVM_CODE) NvM_CalcCrc_EnqueueElement( uint16 BlockNumber );

/** \brief Pop the element from the bottom
 **
 ** \return    uint16 BlockId

 **/
extern FUNC(uint16, NVM_CODE) NvM_CalcCrc_PeekElement( void );
/** \brief Removes element NvM_GlobalBlockNumber
 **        Updates the queue
 **
 **/
extern FUNC(void, NVM_CODE) NvM_CalcCrc_RemoveElement ( void );
#endif
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* ifndef NVM_CALC_CRC_H */
/*==================[end of file]===========================================*/
