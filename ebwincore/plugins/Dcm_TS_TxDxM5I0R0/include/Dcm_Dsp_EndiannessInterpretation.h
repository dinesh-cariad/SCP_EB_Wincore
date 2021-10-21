/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO Dcm.Dsn.File.EndiannessInterpretation.PublicApi,1 */

#ifndef DCM_DSP_ENDIANNESSINTERPRETATION_H
#define DCM_DSP_ENDIANNESSINTERPRETATION_H

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>         /* AUTOSAR standard types */
#include <Dcm_Int.h>
#include <Dcm_Trace.h>
#include <Dcm_Types.h>

/*==================[macros]=====================================================================*/
#if (defined DCM_ENDIANNESSINTERPRETATION_COPY_DATA_TO_OUT_BUFFER)
#error DCM_ENDIANNESSINTERPRETATION_COPY_DATA_TO_OUT_BUFFER already defined
#endif /* if (defined DCM_ENDIANNESSINTERPRETATION_COPY_DATA_TO_OUT_BUFFER) */

#define DCM_ENDIANNESSINTERPRETATION_COPY_DATA_TO_OUT_BUFFER 0x00U

#if (defined DCM_ENDIANNESSINTERPRETATION_COPY_DATA_FROM_IN_BUFFER)
#error DCM_ENDIANNESSINTERPRETATION_COPY_DATA_FROM_IN_BUFFER already defined
#endif /* if (defined DCM_ENDIANNESSINTERPRETATION_COPY_DATA_FROM_IN_BUFFER) */

#define DCM_ENDIANNESSINTERPRETATION_COPY_DATA_FROM_IN_BUFFER 0x01U

#if (defined DCM_ENDIANNESSINTERPRETATION_TYPESIZE_8BITS)
#error DCM_ENDIANNESSINTERPRETATION_TYPESIZE_8BITS already defined
#endif /* if (defined DCM_ENDIANNESSINTERPRETATION_TYPESIZE_8BITS) */

#define DCM_ENDIANNESSINTERPRETATION_TYPESIZE_8BITS 8U

#if (defined DCM_ENDIANNESSINTERPRETATION_TYPESIZE_16BITS)
#error DCM_ENDIANNESSINTERPRETATION_TYPESIZE_16BITS already defined
#endif /* if (defined DCM_ENDIANNESSINTERPRETATION_TYPESIZE_16BITS) */

#define DCM_ENDIANNESSINTERPRETATION_TYPESIZE_16BITS 16U

#if (defined DCM_ENDIANNESSINTERPRETATION_TYPESIZE_32BITS)
#error DCM_ENDIANNESSINTERPRETATION_TYPESIZE_32BITS already defined
#endif /* if (defined DCM_ENDIANNESSINTERPRETATION_TYPESIZE_32BITS) */

#define DCM_ENDIANNESSINTERPRETATION_TYPESIZE_32BITS 32U

#if (defined DCM_ENDIANNESSINTERPRETATION_TYPESIZE_1BYTE)
#error DCM_ENDIANNESSINTERPRETATION_TYPESIZE_1BYTE already defined
#endif /* if (defined DCM_ENDIANNESSINTERPRETATION_TYPESIZE_1BYTE) */

#define DCM_ENDIANNESSINTERPRETATION_TYPESIZE_1BYTE 1U

#if (defined DCM_ENDIANNESSINTERPRETATION_TYPESIZE_2BYTE)
#error DCM_ENDIANNESSINTERPRETATION_TYPESIZE_2BYTE already defined
#endif /* if (defined DCM_ENDIANNESSINTERPRETATION_TYPESIZE_2BYTE) */

#define DCM_ENDIANNESSINTERPRETATION_TYPESIZE_2BYTE 2U

#if (defined DCM_ENDIANNESSINTERPRETATION_TYPESIZE_4BYTE)
#error DCM_ENDIANNESSINTERPRETATION_TYPESIZE_4BYTE already defined
#endif /* if (defined DCM_ENDIANNESSINTERPRETATION_TYPESIZE_4BYTE) */

#define DCM_ENDIANNESSINTERPRETATION_TYPESIZE_4BYTE 4U

#if (defined DCM_ENDIANNESSINTERPRETATION_MSB_MASK)
#error DCM_ENDIANNESSINTERPRETATION_MSB_MASK already defined
#endif /* if (defined DCM_ENDIANNESSINTERPRETATION_MSB_MASK) */

#define DCM_ENDIANNESSINTERPRETATION_MSB_MASK 0x80U

/** \brief Macro which determines if a did data type is an array type or not */
#if (defined DCM_IS_ARRAY_TYPE)
  #error "DCM_IS_ARRAY_TYPE is already defined"
#endif
#define DCM_IS_ARRAY_TYPE(DataType) \
  (((DataType) == DCM_TYPE_SINT8_N) || \
   ((DataType) == DCM_TYPE_SINT16_N) || \
   ((DataType) == DCM_TYPE_SINT32_N) || \
   ((DataType) == DCM_TYPE_UINT8_N) || \
   ((DataType) == DCM_TYPE_UINT16_N) || \
   ((DataType) == DCM_TYPE_UINT32_N))

/** \brief Macro which determines if a did data type is signed type or not */
#if (defined DCM_IS_SIGNED_TYPE)
  #error "DCM_IS_SIGNED_TYPE is already defined"
#endif
#define DCM_IS_SIGNED_TYPE(DataType) \
  (((DataType) == DCM_TYPE_SINT8) || \
   ((DataType) == DCM_TYPE_SINT8_N) || \
   ((DataType) == DCM_TYPE_SINT16) || \
   ((DataType) == DCM_TYPE_SINT16_N) || \
   ((DataType) == DCM_TYPE_SINT32) || \
   ((DataType) == DCM_TYPE_SINT32_N))

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

#if ((DCM_DSP_USE_DIDSERVICES == STD_ON) && (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON))

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Function which copies data from a source buffer to a certain position in a destination buffer
 * with a certain endianness
 *
 *  \param[in] SignalBuffer                    Signal buffer from which data is copied
 *  \param[in] Direction                       Copy from an input stream into a variable or from a variable to an output stream
 *  \param[out] DestinationBuffer              Pointer to the location where data is copied
 *  \param[in] Endianness                      Endianness considered when copying data
 *  \param[in] DataType                        Type of data to be copied
 *  \param[in] DataLength                      Size of data to be copied
 *  \param[in] DataPosition                    Position of data to be copied (either in the source or in the destination buffer depending on the copy direction)
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_EndiannessInterpretation_CopyData(
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SignalBuffer,
  Dcm_DataCopyDirectionType Direction,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DestinationBuffer,
  Dcm_EndiannessType Endianness,
  Dcm_DidDataByteType DataType,
  uint16 DataLength,
  uint16 DataPosition
);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /*#if ((DCM_DSP_USE_DIDSERVICES == STD_ON) && (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON))*/
#endif /* ifndef DCM_DSP_ENDIANNESSINTERPRETATION_H */
/*==================[end of file]================================================================*/
