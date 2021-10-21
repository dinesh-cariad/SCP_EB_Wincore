/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef COM_CORE_READ_H_
#define COM_CORE_READ_H_

/*==================[inclusions]============================================*/

#include <TSAutosar.h>
#include <Com_Priv.h>

/*==================[macros]================================================*/

#ifndef COM_READFUNCS
#define COM_READFUNCS Com_ReadFuncs
#endif

#if (defined COM_CORE_READ_CORE_SIZE) /* To prevent double declaration */
#error COM_CORE_READ_CORE_SIZE already defined
#endif

/** \brief Define COM_CORE_READ_CORE_SIZE
 * Number of different implementations in function pointer array
 */
#define COM_CORE_READ_CORE_SIZE  (uint16)64U


#if (defined COM_CORE_READ_ARRAY_SIZE) /* To prevent double declaration */
#error COM_CORE_READ_ARRAY_SIZE already defined
#endif

/** \brief Define COM_CORE_READ_ARRAY_SIZE
 * Total number of entries in function pointer array
 */
#define COM_CORE_READ_ARRAY_SIZE (COM_CORE_READ_CORE_SIZE * 2U)


#if (defined COM_CORE_READ_CORE_MASK) /* To prevent double declaration */
#error COM_CORE_READ_CORE_MASK already defined
#endif

/** \brief Define COM_CORE_READ_CORE_MASK
 * Mask to access the core function (independent if an extension is configured)
 */
#define COM_CORE_READ_CORE_MASK (COM_CORE_READ_CORE_SIZE - 1U)


#if (defined COM_CORE_READ_EXTENDED_MASK) /* To prevent double declaration */
#error COM_CORE_READ_EXTENDED_MASK already defined
#endif

/** \brief Define COM_CORE_READ_EXTENDED_MASK
 * Mask to access the extended function (masks out flags which are not relevant for library)
 */
#define COM_CORE_READ_EXTENDED_MASK (COM_CORE_READ_ARRAY_SIZE - 1U)

/** \brief Define COM_CORE_READ_DUMMY_MASK
 * Mask to access the extended function (masks out flags which are not relevant for library)
 */
#define COM_CORE_READ_DUMMY_MASK ((uint16)(31U))

#if (defined COM_CORE_READ_CLEAR_LOCK_FLAG) /* To prevent double declaration */
#error COM_CORE_READ_CLEAR_LOCK_FLAG already defined
#endif

/** \brief Define COM_CORE_READ_CLEAR_LOCK_FLAG
 * Mask clears flag which defines if buffer shall be locked (3rd bit position)
 */
#define COM_CORE_READ_CLEAR_LOCK_FLAG ((uint16)0xFFF7U)


#if (defined COM_CORE_READ_CLEAR_SIGN_FLAG) /* To prevent double declaration */
#error COM_CORE_READ_CLEAR_SIGN_FLAG already defined
#endif

/** \brief Define COM_CORE_READ_CLEAR_SIGN_FLAG
 * Mask clears flag which defines if sign extension is done (4th bit position)
 */
#define COM_CORE_READ_CLEAR_SIGN_FLAG ((uint16)0xFFEFU)


#if (defined COM_CORE_READ_CORE_MASK_NOLOCK) /* To prevent double declaration */
#error COM_CORE_READ_CORE_MASK_NOLOCK already defined
#endif

/** \brief Define COM_CORE_READ_EXTENDED_MASK
 * Mask to access (independent if an extension is configured)
 * - core function
 * - no buffer lock
 */
#define COM_CORE_READ_CORE_MASK_NOLOCK \
   (COM_CORE_READ_CORE_MASK \
            & COM_CORE_READ_CLEAR_LOCK_FLAG)


#if (defined COM_CORE_READ_CORE_MASK_NOSIGN) /* To prevent double declaration */
#error COM_CORE_READ_CORE_MASK_NOSIGN already defined
#endif

/** \brief Define COM_CORE_READ_CORE_MASK_NOSIGN
 * Mask to access (independent if an extension is configured)
 * - core function
 * - no sign extension
 */
#define COM_CORE_READ_CORE_MASK_NOSIGN \
      (COM_CORE_READ_CORE_MASK \
            & COM_CORE_READ_CLEAR_SIGN_FLAG)


#if (defined COM_CORE_READ_CORE_MASK_NOLOCK_NOSIGN) /* To prevent double declaration */
#error COM_CORE_READ_CORE_MASK_NOLOCK_NOSIGN already defined
#endif

/** \brief Define COM_CORE_READ_CORE_MASK_NOLOCK_NOSIGN
 * Mask to access (independent if an extension is configured)
 * - core function
 * - no buffer lock
 * - no sign extension
 */
#define COM_CORE_READ_CORE_MASK_NOLOCK_NOSIGN \
      (COM_CORE_READ_CORE_MASK_NOLOCK \
            & COM_CORE_READ_CLEAR_SIGN_FLAG)


#if (defined Com_EB_Read_Get_Fptr) /* To prevent double declaration */
#error Com_EB_Read_Get_Fptr already defined
#endif

/** \brief Com_EB_Read_Get_Fptr
 * Provides a function pointer of from the function pointer array
 * \param[in] Com_EB_ReadPropPtr - configuration of a signal
 * \param[in] mask - mask which shall be applied for function pointer extraction
 * \param[out] fptr - calculated function pointer
 */
#define Com_EB_Read_Get_Fptr(Com_EB_ReadPropPtr, mask, fptr) \
   do{ \
      uint16 const index = ((Com_EB_ReadPropPtr)->ComRxSignalFlags & (mask)); \
      (fptr) =  COM_READFUNCS[index].Read; \
   }while(0)


#if (defined Com_EB_Read_Call_HLP) /* To prevent double declaration */
#error Com_EB_Read_Call_HLP already defined
#endif

/** \brief Com_EB_Read_Call_HLP
 * Invokes a library function
 * \param[in] SignalDataPtr - pointer to signal value
 * \param[in] dataPtr - buffer to which the data shall be read
 * \param[in] Com_EB_ReadPropPtr - configuration of a signal
 * \param[in] mask - mask which shall be applied for function pointer extraction
 */
#define Com_EB_Read_Call_HLP(SignalDataPtr, dataPtr, Com_EB_ReadPropPtr, mask, retval) \
   do{ \
      P2VAR(Com_ReadFuncType, AUTOMATIC, COM_CODE) fptr;  \
      Com_EB_Read_Get_Fptr(Com_EB_ReadPropPtr, mask, fptr); \
      fptr((SignalDataPtr), (dataPtr), (Com_EB_ReadPropPtr), (retval)); \
   }while(0)


#if (defined Com_EB_Read_Core) /* To prevent double declaration */
#error Com_EB_Read_Core already defined
#endif

/** \brief Com_EB_Read_Core
 * Invokes a library function (core part, independent from if extensions is configured)
 * \param[in] SignalDataPtr - pointer to signal value
 * \param[in] dataPtr - buffer to which the data shall be read
 * \param[in] Com_EB_ReadPropPtr - configuration of a signal
 */
#define Com_EB_Read_Core(SignalDataPtr, dataPtr, Com_EB_ReadPropPtr, retval) \
   Com_EB_Read_Call_HLP((SignalDataPtr), (dataPtr), (Com_EB_ReadPropPtr), COM_CORE_READ_CORE_MASK, (retval))


#if (defined Com_EB_Read_Ext) /* To prevent double declaration */
#error Com_EB_Read_Ext already defined
#endif

/** \brief Com_EB_Read_Ext
 * Invokes a library function
 * \param[in] SignalDataPtr - pointer to signal value
 * \param[in] dataPtr - buffer to which the data shall be read
 * \param[in] Com_EB_ReadPropPtr - configuration of a signal
 */
#define Com_EB_Read_Ext(SignalDataPtr, dataPtr, Com_EB_ReadPropPtr, retval) \
   Com_EB_Read_Call_HLP((SignalDataPtr), (dataPtr), (Com_EB_ReadPropPtr), COM_CORE_READ_EXTENDED_MASK, (retval))


#if (defined Com_EB_Read_Core_NoLock) /* To prevent double declaration */
#error Com_EB_Read_Core_NoLock already defined
#endif

/** \brief Com_EB_Read_Core_NoLock
 * Invokes a library function, independent from if extensions is configured:
 * - core part,
 * - no buffer lock,
 * \param[in] SignalDataPtr - pointer to signal value
 * \param[in] dataPtr - buffer to which the data shall be read
 * \param[in] Com_EB_ReadPropPtr - configuration of a signal
 */
#define Com_EB_Read_Core_NoLock(SignalDataPtr, dataPtr, Com_EB_ReadPropPtr, retval) \
   Com_EB_Read_Call_HLP((SignalDataPtr), (dataPtr), (Com_EB_ReadPropPtr), \
            COM_CORE_READ_CORE_MASK_NOLOCK, (retval))


#if (defined Com_EB_Read_Core_NoSign) /* To prevent double declaration */
#error Com_EB_Read_Core_NoSign already defined
#endif

/** \brief Com_EB_Read_Core_NoSign
 * Invokes a library function, independent from if extensions is configured:
 * - core part,
 * - no sign extension
 * \param[in] SignalDataPtr - pointer to signal value
 * \param[in] dataPtr - buffer to which the data shall be read
 * \param[in] Com_EB_ReadPropPtr - configuration of a signal
 */
#define Com_EB_Read_Core_NoSign(SignalDataPtr, dataPtr, Com_EB_ReadPropPtr, retval) \
   Com_EB_Read_Call_HLP((SignalDataPtr), (dataPtr), (Com_EB_ReadPropPtr), \
            COM_CORE_READ_CORE_MASK_NOSIGN, (retval))

#if (defined Com_EB_Read_Core_NoLock_NoSign) /* To prevent double declaration */
#error Com_EB_Read_Core_NoLock_NoSign already defined
#endif

/** \brief Com_EB_Read_Core_NoLock_NoSign
 * Invokes a library function, independent from if extensions is configured:
 * - core part,
 * - no buffer lock,
 * - no sign extension
 * \param[in] SignalDataPtr - pointer to signal value
 * \param[in] dataPtr - buffer to which the data shall be read
 * \param[in] Com_EB_ReadPropPtr - configuration of a signal
 */
#define Com_EB_Read_Core_NoLock_NoSign(SignalDataPtr, dataPtr, Com_EB_ReadPropPtr, retval) \
   Com_EB_Read_Call_HLP((SignalDataPtr), (dataPtr), (Com_EB_ReadPropPtr), \
            COM_CORE_READ_CORE_MASK_NOLOCK_NOSIGN, (retval))


#if ((COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))

/** \brief Com_EB_ReadIsReceived
 * Invokes a library function which checks if a signal is received
 * \param[in] rxSignalPtr - configuration of the Rx signal
 * \param[in] pduLength - length of the Pdu
 * \retval TRUE if signal is received, else FALSE
 */
#define Com_EB_ReadIsReceived(rxSignalPtr, pduLength) \
COM_READFUNCS[(rxSignalPtr)->ComSignalBase.ComRxSignalFlags & COM_CORE_READ_CORE_MASK].IsReceived((rxSignalPtr), (pduLength))

#endif /* ((COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON)) */


#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)

/** \brief Com_EB_ReadCopySig
 * Invokes a library function which copies the value and update bit from an external buffer to the Rx I-PDU buffer of the signal
 * \param[in] rxSignalPtr - configuration of the Rx signal
 * \param[in] iPduPtr - configuration of the Rx I-PDU
 * \param[in] pduInfoPtr - Pdu info of the external buffer
 */
#define Com_EB_ReadCopySig(rxSignalPtr, iPduPtr, pduInfoPtr) \
do{ \
  P2VAR(Com_ReadCopyFuncType, AUTOMATIC, COM_CODE) fptr;  \
  uint16 const index = ((rxSignalPtr)->ComSignalBase.ComRxSignalFlags & COM_CORE_READ_CORE_MASK); \
  fptr =  COM_READFUNCS[index].Copy; \
  fptr((rxSignalPtr), (iPduPtr), (pduInfoPtr), 0U); \
  }while(0)

/** \brief Com_EB_ReadCopyGSig
 * Invokes a library function which copies the value from an external buffer to the Rx I-PDU buffer of the group signal
 * \param[in] rxSignalPtr - configuration of the Rx group signal
 * \param[in] iPduPtr - configuration of the Rx I-PDU
 * \param[in] pduInfoPtr - Pdu info of the external buffer
 */
#define Com_EB_ReadCopyGSig(rxSignalPtr, iPduPtr, pduInfoPtr, offset) \
do{ \
  P2VAR(Com_ReadCopyFuncType, AUTOMATIC, COM_CODE) fptr;  \
  uint16 const index = (((rxSignalPtr)->ComSignalBase.ComRxSignalFlags & COM_CORE_READ_CORE_MASK) \
          | COM_CORE_READ_CORE_SIZE); \
  fptr =  COM_READFUNCS[index].Copy; \
  fptr((rxSignalPtr), (iPduPtr), (pduInfoPtr), (offset)); \
  }while(0)

#endif /* (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) */

/*==================[type definitions]======================================*/

/** \brief Define Com_ReadFuncType
 * signature of a library function for de-serialization
 */
typedef FUNC(void, COM_CODE) (Com_ReadFuncType)
(
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
);

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)

/** \brief Define Com_ReadCopyFuncType
 * signature of a library function for copy signal
 */
typedef FUNC(void, COM_CODE) (Com_ReadCopyFuncType)
(
  P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
  P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
  P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
  uint32 BufferOffset
);

#endif /* (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) */

#if ((COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))

/** \brief Define Com_EB_ReadReceivedFuncType
 * signature of a library function checking of signal is received
 */
typedef FUNC(uint8, COM_CODE) (Com_EB_ReadReceivedFuncType)
(
  P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) RxSignalPtr,
  uint32 Pdulength
);

#endif /* ((COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON)) */

/** \brief Define Com_ReadExtFuncTableType
 * type for the function pointer arrays
 */
typedef struct
{
  /* Function reading a signal */
  P2VAR(Com_ReadFuncType, TYPEDEF, COM_CODE) Read;

#if ((COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))
  /* Function checking if signal is received */
  P2VAR(Com_EB_ReadReceivedFuncType, TYPEDEF, COM_CODE) IsReceived;
#endif /* ((COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON)) */

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)
  /* Function checking if signal is received */
  P2VAR(Com_ReadCopyFuncType, TYPEDEF, COM_CODE) Copy;
#endif /* (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) */

} Com_ReadExtFuncTableType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#define COM_START_SEC_CONST_UNSPECIFIED
#include <Com_MemMap.h>

extern CONST(Com_ReadExtFuncTableType, AUTOMATIC) COM_READFUNCS[COM_CORE_READ_ARRAY_SIZE];

#define COM_STOP_SEC_CONST_UNSPECIFIED
#include <Com_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* COM_CORE_READ_H_ */

