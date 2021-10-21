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
/* This file contains function prototypes for features that are additional to the AUTOSAR_SWS_FlashEEPROMEmulation.pdf specification */

#ifndef FEE_EXTRA_H
#define FEE_EXTRA_H

/*==================[inclusions]============================================*/
#include <Std_Types.h>        /* AUTOSAR Standard type header*/
#include <Fee_Cfg.h>          /* FEE configuration header */

/*==================[macros]================================================*/

/*------------------------[Autosar Vendor Specific API service IDs]------------------------------*/

#if (defined FEE_FREEZEACTIVITIES_API_ID)
#error FEE_FREEZEACTIVITIES_API_ID already defined
#endif
/** \brief Service Id of Fee_FreezeActivities */
#define FEE_FREEZEACTIVITIES_API_ID                         0x13U

#if (defined FEE_WRITE_CUSTOM_API_ID)
#error FEE_WRITE_CUSTOM_API_ID already defined
#endif
/** \brief Service Id of Fee_WriteCustom */
#define FEE_WRITE_CUSTOM_API_ID                             0x14U

#if (defined FEE_CANCELSECTIONERASE_API_ID)
#error FEE_CANCELSECTIONERASE_API_ID already defined
#endif
/** \brief Service Id of Fee_CancelSectionErase */
#define FEE_CANCELSECTIONERASE_API_ID                       0x15U

#if (defined FEE_READ_CUSTOM_API_ID)
#error FEE_READ_CUSTOM_API_ID already defined
#endif
/** \brief Service Id of Fee_ReadCustom */
#define FEE_READ_CUSTOM_API_ID                              0x16U

#if (defined FEE_GET_ERASE_COUNTER_API_ID)
#error FEE_GET_ERASE_COUNTER_API_ID already defined
#endif
/** \brief Service Id of Fee_GetEraseCounterValue */
#define FEE_GET_ERASE_COUNTER_API_ID                        0x17U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define FEE_START_SEC_CODE
#include <MemMap.h>

#if (FEE_ERASE_COUNTER_API == STD_ON)
/** \brief Vendor specific service that returns the value of erase counter.
 **
 ** The Fee_GetEraseCounterValue() API returns the number of erase cycles for one section.
 ** This function provides means of estimating the wear of the flash memory device.
 ** Under unexpected conditions,  if data cannot be recovered from flash,
 ** the value of the erase counter will also be lost.
 **
 ** \return uint32
 ** \retval CounterValue     Number of times Fee sections have been erased
 **
 ** \ServiceID{0x17}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(uint32, FEE_CODE) Fee_GetEraseCounterValue(void);
#endif

#if (FEE_FREEZE_ACTIVITIES_API == STD_ON)
 /** \brief Vendor specific service to "freeze" or "unfreeze" the Fee module in terms of performing any activity.
 **
 ** This function causes Fee to allow the current operation at Fls level to
 ** complete but perform no other until the module is "unfrozen". Usage of
 ** this function to freeze the Fee module MUST BE DONE WITH CARE.
 **
 ** \param[in]      Freeze : TRUE - Fee will not perform any activity
 **                          FALSE - Fee will behave normally
 **
 ** \return Std_ReturnType
 ** \retval E_OK     request was accepted
 ** \retval E_NOT_OK request was not accepted
 **
 ** \ServiceID{0x13}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC(Std_ReturnType, FEE_CODE) Fee_FreezeActivities(uint8  Freeze);
#endif

#if (FEE_WRITE_CUSTOM_API == STD_ON)
/** \brief Vendor specific service for writing a not configured logical block
 **
 ** This API starts a write operation for a not configured block.
 ** This operation writes the contents of the user buffer to a
 ** logical block in flash. The actual write job is
 ** executed asynchronously within Fee_MainFunction.
 **
 ** \param[in]      BlockNumber Number of not configured block
 ** \param[out]     DataBufferPtr Pointer to data buffer
 ** \param[in]      BlockSize Size of not configured block
 **
 ** \return Std_ReturnType
 ** \retval E_OK     Fee accepted the write job
 ** \retval E_NOT_OK Fee rejected the write job
 **
 ** \ServiceID{0x14}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC(Std_ReturnType, FEE_CODE) Fee_WriteCustom
(
  uint16                                   BlockNumber,
  P2CONST(uint8, AUTOMATIC, FEE_APPL_DATA) DataBufferPtr,
  uint16                                   BlockSize
);
#endif

#if (FEE_READ_CUSTOM_API == STD_ON)
/** \brief Vendor specific service for reading a not configured logical block
 **
 ** This API starts a read operation for a not configured block.
 ** The read operation involves reading the contents of a logical block
 ** stored in flash into a user buffer. The actual read job is
 ** executed asynchronously within Fee_MainFunction.
 **
 ** \param[in]      BlockNumber Number of not configured block
 ** \param[in]      BlockOffset Read address offset inside the block
 ** \param[in]      DataBufferPtr Pointer to data buffer
 ** \param[in]      Length  Number of bytes to read
 **
 ** \return Std_ReturnType
 ** \retval E_OK     Fee accepted the read job
 ** \retval E_NOT_OK Fee rejected the read job
 **
 ** \ServiceID{0x16}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC(Std_ReturnType, FEE_CODE) Fee_ReadCustom
(
  uint16                                 BlockNumber,
  uint16                                 BlockOffset,
  P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA) DataBufferPtr,
  uint16                                 Length
);
#endif

#if (FEE_CANCEL_SECTION_ERASE == STD_ON)
/** \brief Vendor specific service to cancel a section erase operation
 **
 ** This API aborts an ongoing section erase at request synchronously.
 ** Semantics : Should not be called while Fee main function is ongoing.
 **
 **
 ** \return Std_ReturnType
 ** \retval E_OK if an erase was ongoing and was canceled.
 **         E_NOT_OK no erase cancelation was done because either no erase is ongoing or
 **                  the function call interrupted the Fee main function.
 **
 ** \ServiceID{0x15}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, FEE_CODE) Fee_CancelSectionErase(void);
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

#endif /* ifndef FEE_EXTRA_H */
