/**
 * \file
 *
 * \brief AUTOSAR Dem
 *
 * This file contains the implementation of the AUTOSAR
 * module Dem.
 *
 * \version 6.4.1
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef DEM_NVMCBK_H
#define DEM_NVMCBK_H

/*==================[inclusions]============================================*/
/* !LINKSTO dsn.Dem.IncludeStr,1 */

#include <Std_Types.h>                            /* AUTOSAR standard types */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define DEM_START_SEC_CODE
#include <Dem_MemMap.h>

#if ( (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH) || \
      (DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_TRIGGER) )

/** \brief Callback-Function from NvM for default data block to request
 ** the data to be copied from the NvM RAM mirror during NvM_ReadBlock()
 ** or NvM_ReadAll()
 **
 ** \param[in] NvMBuffer  Pointer to NvM RAM mirror
 **
 ** \return returns always E_OK
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_NvMReadCopyEventStatusBlock(
  P2CONST(void, NVM_APPL_DATA, NVM_VAR_CLEARED) NvMBuffer);

/** \brief Callback-Function from NvM for default data block to request
 ** the data to be copied into the NvM RAM mirror during NvM_WriteBlock()
 ** or NvM_WriteAll()
 **
 ** \param[in] NvMBuffer  Pointer to NvM RAM mirror
 **
 ** \return returns always E_OK
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_NvMWriteCopyEventStatusBlock(
  P2VAR(void, NVM_APPL_DATA, NVM_VAR_CLEARED) NvMBuffer);

/** \brief Callback-Function from NvM for default data block
 ** notifying successful completion of single block request
 **
 ** \param[in] ServiceId
 ** \param[in] JobResult
 **
 ** \return returns always E_OK
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_NvMEventStatusBlockCallback(
  uint8                 ServiceId,
  NvM_RequestResultType JobResult);

#endif /* DEM_CLEAR_DTC_BEHAVIOR == DEM_CLRRESP_NONVOLATILE_FINISH/TRIGGER */

#if (DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON)

/** \brief Callback-Function from NvM for primary NV gate-entry data block
 ** notifying successful completion of single block request
 **
 ** \param[in] ServiceId
 ** \param[in] JobResult
 **
 ** \return returns always E_OK
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_NvMGateEntryPrimaryBlockCallback(
  uint8                 ServiceId,
  NvM_RequestResultType JobResult);

#if (DEM_MAX_NUMBER_EVENT_ENTRY_SEC != 0U)
/** \brief Callback-Function from NvM for secondary NV gate-entry data block
 ** notifying successful completion of single block request
 **
 ** \param[in] ServiceId
 ** \param[in] JobResult
 **
 ** \return returns always E_OK
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_NvMGateEntrySecondaryBlockCallback(
  uint8                 ServiceId,
  NvM_RequestResultType JobResult);
#endif

#if (DEM_MAX_NUMBER_EVENT_ENTRY_MIR != 0U)
/** \brief Callback-Function from NvM for mirror NV gate-entry data block
 ** notifying successful completion of single block request
 **
 ** \param[in] ServiceId
 ** \param[in] JobResult
 **
 ** \return returns always E_OK
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DEM_CODE) Dem_NvMGateEntryMirrorBlockCallback(
  uint8                 ServiceId,
  NvM_RequestResultType JobResult);
#endif
#endif /* DEM_USE_IMMEDIATE_NV_STORAGE == STD_ON */

/* !LINKSTO Dem_OBD_0005,1 */
#if (DEM_MAX_NUMBER_EVENT_ENTRY_PER != 0U)
/** \brief Callback-Function from NvM for permanent NV data block
 ** notifying successful completion of single block request.
 ** Function is provided only if permanent event memory is enabled.
 ** If JobResult is NVM_REQ_OK, the Dem_EventMemoryPermanentNonVolatileStored[]
 ** array is updated considering the content of Dem_EventMemoryPermanent[]
 ** array.
 **
 ** \param[in] ServiceId
 ** \param[in] JobResult
 **
 ** \return returns E_OK according to NvM requirement NVM368
 ** \return return E_NOT_OK, in case DET reporting and BSW Distribution are
 ** enabled and the callback function is called from another context apart
 ** from the master core
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
/* !LINKSTO Dem_OBD_0054,1, Dem_OBD_0009,2 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_NvMWriteFinishedPermanentMemory(
  uint8                 ServiceId,
  NvM_RequestResultType JobResult);

/** \brief Block specific callback routine which is called in order to let the
 ** Dem copy Dem_EventMemoryPermanent[] array data from RAM block to NvM
 ** permanent memory block.
 **
 ** \param[out] NvMBuffer
 **
 ** \return returns E_OK on success otherwise E_NOT_OK
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
/* !LINKSTO Dem_OBD_0050,2, Dem_OBD_0009,2 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_NvMWriteCopyPermanentMemory(
  P2VAR(void, NVM_APPL_DATA, NVM_VAR_CLEARED) NvMBuffer);

/** \brief Block specific callback routine which is called in order to let the
 ** Dem copy Dem_EventMemoryPermanent[] array data from NvM block to RAM
 ** array.
 **
 ** \param[in] NvMBuffer
 **
 ** \return returns E_OK on success otherwise E_NOT_OK
 **
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous}
 **/
/* !LINKSTO Dem_OBD_0053,2 */
extern FUNC(Std_ReturnType, DEM_CODE) Dem_NvMReadCopyPermanentMemory(
  P2CONST(void, NVM_APPL_DATA, NVM_VAR_CLEARED) NvMBuffer);
#endif

#define DEM_STOP_SEC_CODE
#include <Dem_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef DEM_NVMCBK_H */
/*==================[end of file]===========================================*/
