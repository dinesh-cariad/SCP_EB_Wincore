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
#ifndef NVM_WRAPPER_H
#define NVM_WRAPPER_H
/*==================[inclusions]============================================*/
#include <TSAutosar.h>            /* EB specific standard types             */
                                  /* standard types                         */
#include <NvM_Int_Types.h>        /* NvM specific internal types            */
#include <NvM_Cfg.h>                  /*configuration header*/

#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
/*==================[functions]================================================*/

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

#if (defined NVM_READBLOCK_CALLOUT)
/** \brief If all conditions to execute the NvM_ReadBlock API are met, it will call the
 **        configured ReadBlock callout.
 **
 ** \retval :  Std_ReturnType
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_DstPtr Pointer to a temporary RAM block to where the
 **            NvM data shall be copied.
 **/
extern FUNC(Std_ReturnType,NVM_CODE) NvM_Wrapper_ReadBlock
(
  NvM_BlockIdType BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DstPtr
);
#endif /* defined NVM_READBLOCK_CALLOUT */

#if (defined NVM_WRITEBLOCK_CALLOUT)
/** \brief If all conditions to execute the NvM_WriteBlock API are met, it will call the
 **        configured WriteBlock callout.
 **
 ** \retval :  Std_ReturnType
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_DstPtr Pointer to a temporary RAM block to where the
 **            NvM data shall be copied.
 **/
extern FUNC(Std_ReturnType,NVM_CODE) NvM_Wrapper_WriteBlock
(
  NvM_BlockIdType                         BlockId,
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvM_SrcPtr
);
#endif /* defined NVM_WRITEBLOCK_CALLOUT */

#if (defined NVM_RESTOREBLOCKDEFAULTS_CALLOUT)
/** \brief If all conditions to execute the NvM_RestoreBlockDefaults API are met, it will call the
 **        configured RestoreBlockDefaults callout.
 **
 ** \retval :  Std_ReturnType
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_DstPtr Pointer to a temporary RAM block to where the
 **            NvM data shall be copied.
 **/
extern FUNC(Std_ReturnType,NVM_CODE) NvM_Wrapper_RestoreBlockDefaults
(
  NvM_BlockIdType BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DstPtr
);
#endif /* defined NVM_RESTOREBLOCKDEFAULTS_CALLOUT */

#if (defined NVM_READPRAMBLOCK_CALLOUT)
/** \brief If all conditions to execute the NvM_ReadPRAMBlock API are met, it will call the
 **        configured ReadPRAMBlock callout.
 **
 ** \retval :  Std_ReturnType
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 **/
extern FUNC(Std_ReturnType,NVM_CODE) NvM_Wrapper_ReadPRAMBlock
(
  NvM_BlockIdType BlockId
);
#endif /* defined NVM_READPRAMBLOCK_CALLOUT */

#if (defined NVM_WRITEPRAMBLOCK_CALLOUT)
/** \brief If all conditions to execute the NvM_WritePRAMBlock API are met, it will call the
 **        configured WritePRAMBlock callout.
 **
 ** \retval :  Std_ReturnType
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 **/
extern FUNC(Std_ReturnType,NVM_CODE) NvM_Wrapper_WritePRAMBlock
(
  NvM_BlockIdType BlockId
);
#endif /* defined NVM_WRITEPRAMBLOCK_CALLOUT */

#if (defined NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT)
/** \brief If all conditions to execute the NvM_RestorePRAMBlockDefaults API are met, it will call the
 **        configured RestorePRAMBlockDefaults callout.
 **
 ** \retval :  Std_ReturnType
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 **/
extern FUNC(Std_ReturnType,NVM_CODE) NvM_Wrapper_RestorePRAMBlockDefaults
(
  NvM_BlockIdType BlockId
);
#endif /* defined NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT */

#if (defined NVM_CANCELJOBS_CALLOUT)
/** \brief If all conditions to execute the NvM_CancelJobs API are met, it will call the
 **        configured CancelJobs callout.
 **
 ** \retval :  Std_ReturnType
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 **/
extern FUNC(Std_ReturnType,NVM_CODE) NvM_Wrapper_CancelJobs
(
  NvM_BlockIdType BlockId
);
#endif /* defined NVM_CANCELJOBS_CALLOUT */

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)
#if (defined NVM_ERASENVBLOCK_CALLOUT)
/** \brief If all conditions to execute the NvM_EraseNvBlock API are met, it will call the
 **        configured EraseNvBlock callout.
 **
 ** \retval :  Std_ReturnType
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 **/
extern FUNC(Std_ReturnType,NVM_CODE) NvM_Wrapper_EraseNvBlock
(
  NvM_BlockIdType BlockId
);
#endif /* defined NVM_ERASENVBLOCK_CALLOUT */

#if (defined NVM_INVALIDATENVBLOCK_CALLOUT)
/** \brief If all conditions to execute the NvM_InvalidateNvBlock API are met, it will call the
 **        configured InvalidateNvBlock callout.
 **
 ** \retval :  Std_ReturnType
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 **/
extern FUNC(Std_ReturnType,NVM_CODE) NvM_Wrapper_InvalidateNvBlock
(
  NvM_BlockIdType BlockId
);
#endif /* defined NVM_INVALIDATENVBLOCK_CALLOUT */

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */

#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef NVM_WRAPPER_H */

/*=================[end of file NvM_Wrapper.h]=======================*/
