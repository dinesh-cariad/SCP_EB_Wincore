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

#ifndef COM_CORE_WRITE_H_
#define COM_CORE_WRITE_H_


/*==================[inclusions]============================================*/

#include <TSAutosar.h>
#include <Com_Priv.h>

/*==================[macros]================================================*/

#ifndef COM_WRITEFUNCS
#define COM_WRITEFUNCS Com_WriteFuncs
#endif

#if (defined COM_CORE_WRITE_CORE_SIZE) /* To prevent double declaration */
#error COM_CORE_WRITE_CORE_SIZE already defined
#endif

/** \brief Define COM_CORE_WRITE_CORE_SIZE
 * Number of different implementations in function pointer array
 */
#define COM_CORE_WRITE_CORE_SIZE (uint16)256U


#if (defined COM_CORE_WRITE_ARRAY_SIZE) /* To prevent double declaration */
#error COM_CORE_WRITE_ARRAY_SIZE already defined
#endif

/** \brief Define COM_CORE_WRITE_ARRAY_SIZE
 * Total number of entries in function pointer array
 */
#define COM_CORE_WRITE_ARRAY_SIZE ((uint16)(COM_CORE_WRITE_CORE_SIZE * 2U))


#if (defined COM_CORE_WRITE_CORE_MASK) /* To prevent double declaration */
#error COM_CORE_WRITE_CORE_MASK already defined
#endif

/** \brief Define COM_CORE_WRITE_CORE_MASK
 * Mask to access the core function (independent if an extension is configured)
 */
#define COM_CORE_WRITE_CORE_MASK ((uint16)(COM_CORE_WRITE_CORE_SIZE - 1U))


#if (defined COM_CORE_WRITE_EXTENDED_MASK) /* To prevent double declaration */
#error COM_CORE_WRITE_EXTENDED_MASK already defined
#endif

/** \brief Define COM_CORE_WRITE_EXTENDED_MASK
 * Mask to access the extended function (masks out flags which are not relevant for library)
 */
#define COM_CORE_WRITE_EXTENDED_MASK ((uint16)(COM_CORE_WRITE_ARRAY_SIZE - 1U))

#if (defined COM_CORE_WRITE_DUMMY_MASK) /* To prevent double declaration */
#error COM_CORE_WRITE_DUMMY_MASK already defined
#endif

/** \brief Define COM_CORE_WRITE_EXTENDED_MASK
 * Mask to access the extended function (masks out flags which are not relevant for library)
 */
#define COM_CORE_WRITE_DUMMY_MASK ((uint16)(63U))

#if (defined COM_CORE_WRITE_CLEAR_LOCK_FLAG) /* To prevent double declaration */
#error COM_CORE_WRITE_CLEAR_LOCK_FLAG already defined
#endif

/** \brief Define COM_CORE_WRITE_CLEAR_LOCK_FLAG
 * Mask clears flag which defines if buffer shall be locked (6th bit position)
 */
#define COM_CORE_WRITE_CLEAR_LOCK_FLAG ((uint16)0xFFBFU)


#if (defined Com_EB_Write_Get_Fptr) /* To prevent double declaration */
#error Com_EB_Write_Get_Fptr already defined
#endif

/** \brief Com_EB_Write_Get_Fptr
 * Provides a function pointer of from the function pointer array
 * \param[in] Com_EB_WritePropPtr - configuration of a signal
 * \param[in] mask - mask which shall be applied for function pointer extraction
 * \param[out] fptr - calculated function pointer
 */
#define Com_EB_Write_Get_Fptr(Com_EB_WritePropPtr, mask, fptr) \
   do{ \
      uint16 const index = ((Com_EB_WritePropPtr)->ComTxSignalFlags & (mask)); \
      (fptr) =  COM_WRITEFUNCS[index]; \
   }while(0)


#if (defined Com_EB_Write_Call_HLP) /* To prevent double declaration */
#error Com_EB_Write_Call_HLP already defined
#endif

/** \brief Com_EB_Write_Call_HLP
 * Invokes a library function
 * \param[in] SignalDataPtr - pointer to signal value
 * \param[in] dataPtr - buffer to which the data shall be written
 * \param[in] Com_EB_WritePropPtr - configuration of a signal
 * \param[in] IntDataPtr - pointer to internal data structures
 * \param[in] mask - mask which shall be applied for function pointer extraction
 */
#define Com_EB_Write_Call_HLP(SignalDataPtr, dataPtr, Com_EB_WritePropPtr, IntDataPtr, mask) \
   do{ \
      P2VAR(Com_WriteFuncType, AUTOMATIC, COM_CODE) fptr; \
      Com_EB_Write_Get_Fptr(Com_EB_WritePropPtr, mask, fptr); \
      fptr((SignalDataPtr), (dataPtr), (Com_EB_WritePropPtr), (IntDataPtr)); \
   }while(0)


#if (defined Com_EB_Write_Core) /* To prevent double declaration */
#error Com_EB_Write_Core already defined
#endif

/** \brief Com_EB_Write_Core
 * Invokes a library function (core part, independent from if extensions is configured)
 * \param[in] SignalDataPtr - pointer to signal value
 * \param[in] dataPtr - buffer to which the data shall be written
 * \param[in] Com_EB_WritePropPtr - configuration of a signal
 * \param[in] IntDataPtr - pointer to internal data structures
 */
#define Com_EB_Write_Core(SignalDataPtr, dataPtr, Com_EB_WritePropPtr, IntDataPtr) \
   Com_EB_Write_Call_HLP((SignalDataPtr), (dataPtr), (Com_EB_WritePropPtr), (IntDataPtr), COM_CORE_WRITE_CORE_MASK)


#if (defined Com_EB_Write_Ext) /* To prevent double declaration */
#error Com_EB_Write_Ext already defined
#endif

/** \brief Com_EB_Write_Ext
 * Invokes a library function
 * \param[in] SignalDataPtr - pointer to signal value
 * \param[in] dataPtr - buffer to which the data shall be written
 * \param[in] Com_EB_WritePropPtr - configuration of a signal
 * \param[in] IntDataPtr - pointer to internal data structures
 */
#define Com_EB_Write_Ext(SignalDataPtr, dataPtr, Com_EB_WritePropPtr, IntDataPtr) \
   Com_EB_Write_Call_HLP((SignalDataPtr), (dataPtr), (Com_EB_WritePropPtr), (IntDataPtr), COM_CORE_WRITE_EXTENDED_MASK)


#if (defined Com_EB_Write_Core_NoLock) /* To prevent double declaration */
#error Com_EB_Write_Core_NoLock already defined
#endif

/** \brief Com_EB_Write_Core_NoLock
 * Invokes a library function (core part, no buffer lock, independent from if extensions is configured)
 * \param[in] SignalDataPtr - pointer to signal value
 * \param[in] dataPtr - buffer to which the data shall be written
 * \param[in] Com_EB_WritePropPtr - configuration of a signal
 * \param[in] IntDataPtr - pointer to internal data structures
 */
#define Com_EB_Write_Core_NoLock(SignalDataPtr, dataPtr, Com_EB_WritePropPtr, IntDataPtr) \
   Com_EB_Write_Call_HLP((SignalDataPtr), (dataPtr), (Com_EB_WritePropPtr), (IntDataPtr), \
            (COM_CORE_WRITE_CORE_MASK & COM_CORE_WRITE_CLEAR_LOCK_FLAG))

/*==================[type definitions]======================================*/

/** \brief Define Com_WriteFuncType
 * signature of a library function for serialization
 */
typedef FUNC(void, COM_CODE) (Com_WriteFuncType)
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, TYPEDEF, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#define COM_START_SEC_CONST_UNSPECIFIED
#include <Com_MemMap.h>

extern CONSTP2VAR(Com_WriteFuncType, AUTOMATIC, COM_CODE)
        COM_WRITEFUNCS[COM_CORE_WRITE_ARRAY_SIZE];

#define COM_STOP_SEC_CONST_UNSPECIFIED
#include <Com_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* _COM_CORE_WRITE_H_ */
