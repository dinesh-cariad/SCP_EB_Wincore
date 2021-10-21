/**
 * \file
 *
 * \brief AUTOSAR ComXf
 *
 * This file contains the implementation of the AUTOSAR
 * module ComXf.
 *
 * \version 1.0.37
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef COMXF_INT_H
#define COMXF_INT_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types, e.g. TS_PARAM_UNUSED */
#include <Com_ComXf.h>          /* header file of EB Com module for ComXf */
#include <TransformerTypes.h>   /* General transformer data types */
#include <ComXf_Cfg.h>          /* Pre-compile time configuration */
#include <ComXf_Api_Stc.h>      /* Static API of ComXf */
#include <ComXf_Trace.h>        /* Trace macros */

#if (COMXF_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                /* Det API */
#endif /* COMXF_DEV_ERROR_DETECT == STD_ON */

/*==================[macros]================================================*/

/* !LINKSTO ComXf.Design.PreCondition.E_OK,1 */
#if (E_OK != 0U)
#error precondition for common usage of serialization library not fulfilled
#error error codes are aggregated with or operations. Therefore the E_OK has to be zero.
#endif

#if (defined COMXF_EB_DET_REPORT_ERROR) /* to prevent double declaration */
#error COMXF_EB_DET_REPORT_ERROR already defined
#endif /* if (defined COMXF_EB_DET_REPORT_ERROR) */

#if (defined ComXf_EB_Write_Signal) /* to prevent double declaration */
#error ComXf_EB_Write_Signal already defined
#endif /* if (defined ComXf_EB_Write_Signal) */

#if (defined ComXf_EB_Read_Signal) /* to prevent double declaration */
#error ComXf_EB_Read_Signal already defined
#endif /* if (defined ComXf_EB_Read_Signal) */

#if (defined COMXF_COM_SET_WRITE_RETVAL)        /* To prevent double declaration */
#error COMXF_COM_SET_WRITE_RETVAL is already defined
#endif /* (defined COMXF_COM_SET_WRITE_RETVAL) */

/* !LINKSTO ComXf.Design.Com_ComXf_Write.E_SER_GENERIC_ERROR,1 */
#if (defined (COM_CHECK_VALUE_SIZE))
#if (COM_CHECK_VALUE_SIZE == STD_ON)
#define COMXF_COM_SET_WRITE_RETVAL(retvalPtr) \
 do { \
 if(*(retvalPtr) != E_OK)\
 { \
 *(retvalPtr) = E_SER_GENERIC_ERROR; \
 }\
 } while(0)
 #else
 #define COMXF_COM_SET_WRITE_RETVAL(retvalPtr) TS_PARAM_UNUSED((retvalPtr))
 #endif /* (COM_CHECK_VALUE_SIZE == STD_ON) */
#else
#define COMXF_COM_SET_WRITE_RETVAL(retvalPtr) TS_PARAM_UNUSED((retvalPtr))
#endif /* (defined (COM_CHECK_VALUE_SIZE)) */

#if (COMXF_DEV_ERROR_DETECT == STD_ON)

/** \brief Macro for reporting an error to Det
 **
 ** \param[in] SID Service ID of the API function
 ** \param[in] ERROR_CODE Error code reported to Det module
 **/
#define COMXF_EB_DET_REPORT_ERROR(SID, ERROR_CODE)\
  do{                                             \
     (void)Det_ReportError(                       \
              (uint16) (COMXF_MODULE_ID),         \
              (uint8) (COMXF_INSTANCE_ID),        \
              (uint8) (SID),                      \
              (uint8) (ERROR_CODE)                \
     );                                           \
  }while((0))


#define ComXf_EB_Write_Signal(buffer, SignalDataPtr, retvalPtr, GSignalId, SGId) \
  do {                                                                           \
     if (Com_ComXf_GetTxSGId(GSignalId) != (SGId))                               \
     {                                                                           \
        COMXF_EB_DET_REPORT_ERROR(COMXF_SID_SR_TRANSFORMER, COMXF_E_PARAM);      \
        (*(retvalPtr)) = E_SER_GENERIC_ERROR;                                    \
     }                                                                           \
     else                                                                        \
     {                                                                           \
        Com_ComXf_Write((buffer), (P2CONST(void, AUTOMATIC, COMXF_APPL_DATA))    \
                 (SignalDataPtr), (retvalPtr), (GSignalId));                     \
        COMXF_COM_SET_WRITE_RETVAL(retvalPtr);                                   \
     }                                                                           \
  } while(0)

#define ComXf_EB_Read_Signal(buffer, SignalDataPtr, retvalPtr, GSignalId, SGId)  \
  do {                                                                           \
     if (Com_ComXf_GetRxSGId(GSignalId) != (SGId))                               \
     {                                                                           \
        COMXF_EB_DET_REPORT_ERROR(COMXF_SID_SR_INV_TRANSFORMER, COMXF_E_PARAM);  \
        (*(retvalPtr)) = E_SER_GENERIC_ERROR;                                    \
     }                                                                           \
     else                                                                        \
     {                                                                           \
        Com_ComXf_Read((buffer), (P2VAR(void, AUTOMATIC, COMXF_APPL_DATA))       \
                 (SignalDataPtr), (retvalPtr), (GSignalId));                     \
     }                                                                           \
  } while(0)

#else

#define ComXf_EB_Write_Signal(buffer, SignalDataPtr, retvalPtr, GSignalId, SGId)         \
  Com_ComXf_Write((buffer), (P2CONST(void, AUTOMATIC, COMXF_APPL_DATA)) (SignalDataPtr), \
           (retvalPtr), (GSignalId))

#define ComXf_EB_Read_Signal(buffer, SignalDataPtr, retvalPtr, GSignalId, SGId)          \
  Com_ComXf_Read((buffer), (P2VAR(void, AUTOMATIC, COMXF_APPL_DATA)) (SignalDataPtr),    \
           (retvalPtr), (GSignalId))

#endif /* (COMXF_DEV_ERROR_DETECT == STD_ON) */

/*==================[Error codes]===========================================*/

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define COMXF_START_SEC_CODE
#include <ComXf_MemMap.h>

#if (COMXF_DEV_ERROR_DETECT == STD_ON)

/** \brief Does DET checks for serialization APIs.
 **
 ** This service checks the input parameter. If an error occurred DET is called.
 **
 ** \param[in]  buffer         Buffer allocated by the RTE, where the transformed data has to
 **                            be stored by the transformer.
 ** \param[in]  bufferLength   Used length of the buffer.
 ** \param[in]  dataElement    Data element which shall be transformed.
 ** \param[out] retvalPtr      reference to storage of return value
 **                            E_OK if no error occurred
 **                            E_SER_GENERIC_ERROR otherwise
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, COMXF_CODE) ComXf_EB_CheckInput
(
  P2CONST(uint8, AUTOMATIC, COMXF_APPL_DATA) buffer,
  P2VAR(uint16, AUTOMATIC, COMXF_APPL_DATA) bufferLength,
  P2CONST(void, AUTOMATIC, COMXF_APPL_DATA) dataElement,
  P2VAR(uint8, AUTOMATIC, COMXF_APPL_DATA) retvalPtr
);

/** \brief Does DET checks for deserialization APIs.
 **
 ** This service checks the input parameter. If an error occurred DET is called.
 **
 ** \param[in]  buffer         Buffer allocated by the RTE, where the transformed data has to
 **                            be read by the transformer.
 ** \param[in]  bufferLength   Used length of the buffer.
 ** \param[in]  dataElement    Data element which shall be transformed.
 ** \param[out] retvalPtr      reference to storage of return value
 **                            E_OK if no error occurred
 **                            E_SER_GENERIC_ERROR otherwise
 ** \param[in]  SignalGroupId  Data element which shall be transformed.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, COMXF_CODE) ComXf_EB_Inv_CheckInput
(
  P2CONST(uint8, AUTOMATIC, COMXF_APPL_DATA) buffer,
  uint16 bufferLength,
  P2CONST(void, AUTOMATIC, COMXF_APPL_DATA) dataElement,
  P2VAR(uint8, AUTOMATIC, COMXF_APPL_DATA) retvalPtr,
  Com_SignalIdType SignalGroupId
);

#endif /* (COMXF_DEV_ERROR_DETECT == STD_ON) */


#define COMXF_STOP_SEC_CODE
#include <ComXf_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef COMXF_INT_H */
/*==================[end of file]===========================================*/
