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
#ifndef COM_COMXF_H
#define COM_COMXF_H
/*==================[includes]==============================================*/

#include <TSAutosar.h>               /* EB specific standard types */
#include <Com_Priv.h>                /* Module private API */
#include <Com_Core_Read.h>           /* de-serialization library */
#include <Com_Core_Write.h>          /* serialization library */
#include <Com_SymbolicNames_PBcfg.h> /* symbolic name values */

/*==================[macros]================================================*/

#if (defined COM_COMXF_INIT)          /* To prevent double declaration */
#error COM_COMXF_INIT already defined
#endif

/** \brief Define COM_COMXF_INIT
 * literal for initialized state
 */
#define COM_COMXF_INIT COM_EB_INIT

#if (defined COM_COMXF_UNINIT)          /* To prevent double declaration */
#error COM_COMXF_UNINIT already defined
#endif

/** \brief Define COM_COMXF_UNINIT
 * literal for un-initialized state
 */
#define COM_COMXF_UNINIT COM_EB_UNINIT

#if (defined COM_COMXF_SET_WRITE_RANGE_CHECK_RETVAL)        /* To prevent double declaration */
#error COM_COMXF_SET_WRITE_RANGE_CHECK_RETVAL is already defined
#endif /* (defined COM_COMXF_SET_WRITE_RANGE_CHECK_RETVAL) */

#if (COM_CHECK_VALUE_SIZE == STD_ON)
#define COM_COMXF_SET_WRITE_RANGE_CHECK_RETVAL(IntData, retvalPtr) \
   do { \
      if((IntData).RangeCheckPassed != E_OK) \
      { \
         *(retvalPtr) = (IntData).RangeCheckPassed; \
      }\
   } while(0);
#else
#define COM_COMXF_SET_WRITE_RANGE_CHECK_RETVAL(IntData, retvalPtr)
#endif /* (COM_CHECK_VALUE_SIZE == STD_ON) */

#if (defined COM_COMXF_SET_WRITE_DET_RETVAL)        /* To prevent double declaration */
#error COM_COMXF_SET_WRITE_DET_RETVAL is already defined
#endif /* (defined COM_COMXF_SET_WRITE_DET_RETVAL) */

#if (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON)
#define COM_COMXF_SET_WRITE_DET_RETVAL(IntData, retvalPtr) \
   do { \
      if((IntData).SendSigRetval != E_OK)\
      { \
         *(retvalPtr) = (IntData).SendSigRetval; \
      }\
   } while(0);
#else
#define COM_COMXF_SET_WRITE_DET_RETVAL(IntData, retvalPtr)
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */

#if (defined COM_COMXF_SET_WRITE_RETVAL)        /* To prevent double declaration */
#error COM_COMXF_SET_WRITE_RETVAL is already defined
#endif /* (defined COM_COMXF_SET_WRITE_RETVAL) */

#if ( (COM_CHECK_VALUE_SIZE == STD_ON) || (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) )
#define COM_COMXF_SET_WRITE_RETVAL(IntData, retvalPtr) \
   do { \
      COM_COMXF_SET_WRITE_RANGE_CHECK_RETVAL(IntData, retvalPtr)    \
      COM_COMXF_SET_WRITE_DET_RETVAL(IntData, retvalPtr)            \
   } while(0)
#else
#define COM_COMXF_SET_WRITE_RETVAL(IntData, retvalPtr) TS_PARAM_UNUSED((retvalPtr))
#endif /* ( (COM_CHECK_VALUE_SIZE == STD_ON) || (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) ) */

/*==================[type definitions]======================================*/

typedef Com_EB_StatusType Com_ComXf_StatusType;

/*==================[external function declarations]========================*/

#define COM_START_SEC_CODE
#include <Com_MemMap.h>

#if (defined Com_ComXf_Read)        /* To prevent double declaration */
#error Com_ComXf_Read is already defined
#endif /* (defined Com_ComXf_Read) */

/**
 ** \brief Com_ComXf_Read de-serializes a group signal
 **
 ** This function de-serializes a group signal according to the given
 ** configuration from a buffer. If an error occurs, the error is stored in retvalPtr,
 ** otherwise the value is not changed.
 **
 ** NOTE: May be implemented as function like macro.
 **
 ** \param[in] buffer           Buffer from which value shall be extracted
 ** \param[out]  SignalDataPtr  reference to store value
 ** \param[in,out] retvalPtr    reference where error is reported in case of an error
 ** \param[in]  GroupSignalId   handle Id of the group signal
 **
 **/
#define Com_ComXf_Read(buffer, SignalDataPtr, retvalPtr, GroupSignalId)                         \
   do{                                                                                          \
      CONSTP2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =                      \
      COM_GET_CONFIG_ELEMENT(ComRxSignalType, Com_gConfigPtr->ComRxSignalRef, (GroupSignalId)); \
      CONSTP2CONST(ComSignalBaseType, AUTOMATIC, AUTOMATIC) Com_EB_ReadPropPtr =                \
      &SignalPtr->ComSignalBase;                                                                \
      Com_EB_Read_Core_NoLock((SignalDataPtr), (buffer), Com_EB_ReadPropPtr, retvalPtr) ;       \
   }while(0)


/**
 ** \brief Com_ComXf_Get_RxSignalGroupSize returns length of a Rx signal group
 **
 ** This function returns length of a Rx signal group.
 **
 ** NOTE: May be implemented as function like macro.
 **
 ** \param[in]  SignalGroupId   handle Id of the signal group
 **
 ** \return success: length of the signal group according to the Com configuration
 ** \return failure: 0 (e.g. Com module is not initialized)
 **/
extern FUNC(uint16, COM_CODE) Com_ComXf_Get_RxSignalGroupSize
(
   Com_SignalGroupIdType SignalGroupId
);


/**
 ** \brief Com_ComXf_GetRxSGId returns signal group Id of a group signal
 **
 ** This function returns associated signal group Id of a Rx group signal.
 ** Preconditions:
 ** - COM must be initialized
 **
 ** NOTE: May be implemented as function like macro.
 **
 ** \param[in]  GroupSignalId   handle Id of the group signal
 **
 ** \return handle Id of the signal group, or
 **         0xFFFF (invalid handle id) if passed GroupSignalId does not exist or
 **                COM is not initialized
 **/
extern FUNC(Com_SignalGroupIdType, COM_CODE) Com_ComXf_GetRxSGId
(
   Com_SignalIdType GroupSignalId
);

#if (defined Com_ComXf_Write)        /* To prevent double declaration */
#error Com_ComXf_Write is already defined
#endif /* (defined Com_ComXf_Write) */

/**
 ** \brief Com_ComXf_Write serializes a group signal
 **
 ** This function serializes a group signal according to the given
 ** configuration into buffer. If an error occurs, the error is stored in retvalPtr,
 ** otherwise the value is not changed.
 **
 ** NOTE: May be implemented as function like macro.
 **
 ** \param[out] buffer          Buffer to store the serialized value
 ** \param[in]  SignalDataPtr   Pointer to the value which shall be serialized
 ** \param[in,out] retvalPtr    reference where error is reported in case of an error
 ** \param[in]  GroupSignalId  handle Id of the group signal
 **
 **/
#define Com_ComXf_Write(buffer, SignalDataPtr, retvalPtr, GroupSignalId)                        \
   do{                                                                                          \
      CONSTP2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr =                      \
      COM_GET_CONFIG_ELEMENT(ComTxSignalType, Com_gConfigPtr->ComTxSignalRef, (GroupSignalId)); \
      Com_EB_WriteIntDataType IntData;                                                          \
      COM_INIT_Com_EB_WriteIntDataType(IntData, SignalPtr->ComSignalBase.ComBitSize, FALSE,     \
      E_OK, FALSE, FALSE, E_OK);                                                                \
      Com_EB_Write_Core_NoLock((SignalDataPtr), (buffer), SignalPtr, &IntData);                 \
      COM_COMXF_SET_WRITE_RETVAL(IntData, retvalPtr);                                           \
   }while(0)


/**
 ** \brief Com_ComXf_TxPrepareBuffer prepares a buffer
 ** for serialization of a signal group.
 **
 ** This function checks if the buffer has to be initialized.
 ** That is the case if \param Initialize is TRUE or
 ** the signal group has unused areas
 **
 ** \param[in]  SignalGroupId   handle Id of the signal group
 ** \param[in]  Initialize      If TRUE, the buffer shall always be initialized.
 **                             If FALSE the buffer is only initialized if there are unused areas.
 ** \param[out] buffer          Buffer which shall be prepared
 **
 ** \return success: length of the buffer according to the Com configuration
 ** \return failure: 0 (e.g. Com module is not initialized)
 **/
extern FUNC(uint16, COM_CODE) Com_ComXf_TxPrepareBuffer
(
   P2VAR(uint8, AUTOMATIC, COMXF_APPL_DATA) buffer,
   Com_SignalGroupIdType SignalGroupId,
   boolean Initialize
);


/**
 ** \brief Com_ComXf_GetTxSGId returns signal group Id of a group signal
 **
 ** This function returns associated signal group Id of a Tx group signal.
 ** Preconditions:
 ** - COM must be initialized
 **
 ** NOTE: May be implemented as function like macro.
 **
 ** \param[in]  GroupSignalId   handle Id of the group signal
 **
 ** \return handle Id of the signal group
 **/
extern FUNC(Com_SignalGroupIdType, COM_CODE) Com_ComXf_GetTxSGId
(
   Com_SignalIdType GroupSignalId
);

#if (defined Com_ComXf_GetStatus)        /* To prevent double declaration */
#error Com_ComXf_GetStatus is already defined
#endif /* (defined Com_ComXf_GetStatus) */

/**
 ** \brief Com_ComXf_GetStatus returns the initialization status of the Com module.
 **
 ** This function returns the initialization status of the Com module.
 **
 ** NOTE: May be implemented as function like macro.
 **
 ** \return COM_COMXF_INIT if the Com module is initialized.
 ** \return COM_COMXF_UNINIT if the Com module is not initialized.
 **/
#define Com_ComXf_GetStatus() Com_InitStatus


#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef COM_COMXF_H */
/*==================[end of file]===========================================*/
