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

/* !LINKSTO Dcm.Dsn.File.EndiannessInterpretation.Impl,1 */
/* This file contains the implementation of the EndiannessInterpretation software unit. */

/*===============================[includes]======================================================*/

#include <Dcm_Dsp_EndiannessInterpretation.h>
#include <TSMem.h>

/*===========================[macro definitions]=================================================*/
#if ((DCM_DSP_USE_DIDSERVICES == STD_ON) && (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON))

/** \brief Offset to calculate the bit index after a big endian copy */
#if (defined DCM_ENDIANNESSINTERPRETATION_2BYTES_OFFSET)
  #error "DCM_ENDIANNESSINTERPRETATION_2BYTES_OFFSET is already defined"
#endif
#define DCM_ENDIANNESSINTERPRETATION_2BYTES_OFFSET   2U

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

/*==================[internal function declarations]=============================================*/

/** \brief Gets the data type size in bits and the size of the array with the same type as the data type
 *         needed to store the data
 *
 *  \param[in] DataType                        Configured data type
 *  \param[in] DataLength                      Configured data length
 *  \param[out] SizeOfDataType                 Size of data type in bits
 *  \param[out] DataArraySize                  Size of array needed to store the data
 *
 *  \return none
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_EndiannessInterpretation_GetSignalTypeAndSizeInformation(
  Dcm_DidDataByteType DataType,
  uint16 DataLength,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SizeOfDataType,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) DataArraySize);

/** \brief Determine the position of the msb of the signal
 *
 *  \param[in] DataLength                      Configured data length
 *  \param[in] Endianness                      Endianness of the signal
 *  \param[in/out] MostSignificantBitOfSignal     Position of the signal's msb
 *
 *  \return none
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_EndiannessInterpretation_GetSignalMostSignificantBitPosition(
  uint16 DataLength,
  Dcm_EndiannessType Endianness,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) MostSignificantBitOfSignal);

/** \brief Determine Copy method, depending on endianness, copy direction and cpu byte order
 *
 *  \param[in] Endianness    Endianness of the signal
 *  \param[in] Direction     Copy from variable to buffer or from buffer to variable
 *
 *  \return Copy method
 */
STATIC FUNC(Dcm_DataCopyMethodType, DCM_CODE) Dcm_Dsp_EndiannessInterpretation_GetDataCopyMethod(
  Dcm_EndiannessType Endianness,
  Dcm_DataCopyDirectionType Direction);

/*==================[external function definitions]==============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO SWS_Dcm_00970,1 */
/* !LINKSTO SWS_Dcm_00971,1 */
/* !LINKSTO Dcm.Dsn.IB.EndiannessInterpretation.CopyData,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_EndiannessInterpretation_CopyData(
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SignalBuffer,
  Dcm_DataCopyDirectionType Direction,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DestinationBuffer,
  Dcm_EndiannessType Endianness,
  Dcm_DidDataByteType DataType,
  uint16 DataLength,
  uint16 DataPosition
  )
{
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SourceBuffer = &SignalBuffer[0U];
  /* Data position from which the copy starts in the source buffer */
  uint16 SourceDataPosition = 0U;
  /* Data position to which the copy starts in the destination buffer */
  uint16 DestinationDataPosition = 0U;
  /* Variable which keeps track of the DataLength remaining to be copied */
  uint16 RemainingDataLength = DataLength;
  /* Stores how many elements the data array has depending on the data type and data size
   * (If the data type is not an array type it is considered an 1 element array) */
  uint16 DataArraySize = 0U;
  /* Size of data type in bits */
  uint8 SizeOfDataType = 0U;
  /* Index for data elements used when copying data in the destination buffer */
  uint16_least DataElementsIndex;
  /* Stores the direction and endianness of data copy */
  Dcm_DataCopyMethodType DataCopyMethod;
  /* Stores the current bit position (before and after data copy) */
  uint16 CurrentBitIndex = DataPosition;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
  Dcm_ByteAdvanceDirectionType ByteAdvanceDirection = DCM_BYTE_DIRECTION_INCREMENT;
#else
  Dcm_ByteAdvanceDirectionType ByteAdvanceDirection = DCM_BYTE_DIRECTION_DECREMENT;
#endif /*(CPU_BYTE_ORDER == LOW_BYTE_FIRST)*/

  DBG_DCM_DSP_ENDIANNESSINTERPRETATION_COPYDATA_ENTRY(
    SignalBuffer,Direction,DestinationBuffer,Endianness,DataType,DataLength,DataPosition);

  /* Determine the direction and endianness of data copy. This determines how the source and destination
   * buffers are went through */
  DataCopyMethod = Dcm_Dsp_EndiannessInterpretation_GetDataCopyMethod(Endianness, Direction);

  /* Get the size of data type and the size of the array needed to store the data
   * depending on the data size and data type */
  Dcm_Dsp_EndiannessInterpretation_GetSignalTypeAndSizeInformation(
    DataType,DataLength,&SizeOfDataType,&DataArraySize);

  /* For data copy from input buffer to variable, before copying the actual data, fill the variable with 0 or 1 depending on the sign */
  if ((Direction == DCM_ENDIANNESSINTERPRETATION_COPY_DATA_FROM_IN_BUFFER) &&
      (Endianness != DCM_OPAQUE_ENDIAN) &&
       ((SizeOfDataType > DataLength) || (((DataLength%SizeOfDataType) != 0) && (DCM_IS_ARRAY_TYPE(DataType)))) &&
       DCM_IS_SIGNED_TYPE(DataType))
  {
    /* Fill value according to the sign */
    uint8 FillValue;
    /* Stores the position of the msb of the signal */
    uint16 MostSignificantBitOfSignal = DataPosition;

    Dcm_Dsp_EndiannessInterpretation_GetSignalMostSignificantBitPosition(DataLength, Endianness, &MostSignificantBitOfSignal);

    /* Determine the signedness */
    if ((SourceBuffer[MostSignificantBitOfSignal/DCM_BYTE_BITS] & (1U<<(MostSignificantBitOfSignal%DCM_BYTE_BITS))) != 0U)
    {
      FillValue = DCM_UINT8_MAX;
    }
    else
    {
      FillValue = 0x00U;
    }

    /* Fill the destination with 0 or 1 depending on the sign */
    /* !LINKSTO Dcm.EB.EndiannessInterpretation.CopyBufferToData.NonArrayType.NegativeSignExtension,1 */
    /* !LINKSTO Dcm.EB.EndiannessInterpretation.CopyBufferToData.NonArrayType.PositiveSignExtension,1 */
    /* !LINKSTO Dcm.EB.EndiannessInterpretation.CopyBufferToData.ArrayType.NegativeSignExtension,1 */
    /* !LINKSTO Dcm.EB.EndiannessInterpretation.CopyBufferToData.ArrayType.PositiveSignExtension,1 */
    if ((DCM_IS_ARRAY_TYPE(DataType)) && (Endianness != DCM_BIG_ENDIAN))
    {
      uint16 DestinationStartPosition = (DataArraySize-1U)*((uint16)DCM_GET_BYTES((uint16)SizeOfDataType));
      TS_MemSet(&DestinationBuffer[DestinationStartPosition], FillValue, (uint32)DCM_GET_BYTES((uint32)SizeOfDataType));
    }
    else
    {
      TS_MemSet(DestinationBuffer, FillValue, (uint32)DCM_GET_BYTES((uint32)SizeOfDataType));
    }
  }

  /* If data has an array type, each element is copied individually
   * If data doesn't have an array type, only one copy step will be made for the whole data size (or data type size) */
  for (DataElementsIndex=0U; DataElementsIndex<DataArraySize; DataElementsIndex++)
  {
    /* How many bits of data are copied at this step */
    uint16 SizeToCopy;

    /* If there is more data to be copied than the size of data type, copy a chunk with the size of the data type size */
    if (RemainingDataLength > SizeOfDataType)
    {
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.StopAtTypeLength.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.StopAtTypeLength.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.StopAtTypeLength.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.StopAtTypeLength.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      SizeToCopy = SizeOfDataType;
    }
    else
    {
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.StopAtSignalLength.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.StopAtSignalLength.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.StopAtSignalLength.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.StopAtSignalLength.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      SizeToCopy = RemainingDataLength;
    }

    /* Copy to out buffer (Read Did operation) */
    /* !LINKSTO Dcm.Dsn.IB.EndiannessInterpretation.CopyDataToOutBuffer,1 */
    if (Direction == DCM_ENDIANNESSINTERPRETATION_COPY_DATA_TO_OUT_BUFFER)
    {
      if ((Endianness == DCM_BIG_ENDIAN) && DCM_IS_ARRAY_TYPE(DataType))
      {
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint8Array.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint16Array.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint32Array.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint8Array.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint16Array.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint32Array.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint8Array.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint16Array.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint32Array.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint8Array.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint16Array.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint32Array.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */

        /* If the data has an array type and big endianness, start from the element with the highest index
         * in the source and go towards the low index elements */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        SourceDataPosition = ((DataArraySize - 1U)*(uint16)SizeOfDataType) - ((uint16)DataElementsIndex*(uint16)SizeOfDataType);
#else
        SourceDataPosition = ((DataArraySize - 1U)*(uint16)SizeOfDataType) +
          (((uint16)DCM_GET_BYTES((uint16)SizeOfDataType) - 1U)*DCM_BYTE_BITS) - ((uint16)DataElementsIndex*(uint16)SizeOfDataType);
#endif /*(CPU_BYTE_ORDER == LOW_BYTE_FIRST)*/

      }
      else
      {
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint8Array.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint16Array.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint32Array.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint8Array.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint16Array.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint32Array.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint8Array.Opaque.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint16Array.Opaque.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint32Array.Opaque.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint8Array.Opaque.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint16Array.Opaque.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint32Array.Opaque.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint8Array.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint16Array.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint32Array.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint8Array.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint16Array.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint32Array.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint8Array.Opaque.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint16Array.Opaque.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.uint32Array.Opaque.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint8Array.Opaque.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint16Array.Opaque.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.sint32Array.Opaque.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */

        /* If the data doesn't have an array type and big endianness, start from the element with the lowest index
         * in the source and at each iteration, advance the copy position in the source buffer by one data type chunk */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.Start.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.Start.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.Start.OpaqueOnLittleEndianCPU.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.Start.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.Start.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.Start.OpaqueOnLittleEndianCPU.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        SourceDataPosition = (uint16)(DataElementsIndex*SizeOfDataType);
#else
        if (Endianness != DCM_OPAQUE_ENDIAN)
        {
          SourceDataPosition = (uint16)(DataElementsIndex*SizeOfDataType + ((uint16)DCM_GET_BYTES((uint16)SizeOfDataType) - 1U)*DCM_BYTE_BITS);
        }
        else
        {
          /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.Start.OpaqueOnBigEndianCPU.USE_DATA_SENDER_RECEIVER,1 */
          /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.Start.OpaqueOnBigEndianCPU.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
          SourceDataPosition = (uint16)(DataElementsIndex*SizeOfDataType);
        }
#endif /*(CPU_BYTE_ORDER == LOW_BYTE_FIRST)*/
      }

      /* The start position in the destination for Read Did operations is the configured DataPosition
       * After each copy it is recalculated */
      DestinationDataPosition = CurrentBitIndex;
    }
    /* Copy from buffer (Write Did operation) */
    /* !LINKSTO Dcm.Dsn.IB.EndiannessInterpretation.CopyDataFromInBuffer,1 */
    else
    {
      /* If the data has an array type and big endianness, start from the element with the highest index
       * in the destination and go towards the low index elements */
      if ((Endianness == DCM_BIG_ENDIAN) && DCM_IS_ARRAY_TYPE(DataType))
      {
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint8Array.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint16Array.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint32Array.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint8Array.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint16Array.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint32Array.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint8Array.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint16Array.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint32Array.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint8Array.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint16Array.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint32Array.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        DestinationDataPosition = ((DataArraySize - 1U)*(uint16)SizeOfDataType) - ((uint16)DataElementsIndex*(uint16)SizeOfDataType);
#else
        DestinationDataPosition = ((DataArraySize - 1U)*(uint16)SizeOfDataType) +
          (((uint16)DCM_GET_BYTES((uint16)SizeOfDataType) - 1U)*DCM_BYTE_BITS) - ((uint16)DataElementsIndex*(uint16)SizeOfDataType);
#endif /*(CPU_BYTE_ORDER == LOW_BYTE_FIRST)*/
      }
      else
      {
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint8Array.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint16Array.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint32Array.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint8Array.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint16Array.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint32Array.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint8Array.Opaque.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint16Array.Opaque.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint32Array.Opaque.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint8Array.Opaque.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint16Array.Opaque.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint32Array.Opaque.USE_DATA_SENDER_RECEIVER,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint8Array.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint16Array.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint32Array.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint8Array.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint16Array.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint32Array.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint8Array.Opaque.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint16Array.Opaque.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.uint32Array.Opaque.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint8Array.Opaque.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint16Array.Opaque.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.sint32Array.Opaque.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */

        /* If the data doesn't have an array type with big endianness, start from the element with the lowest index
         * in the destination and at each iteration, advance the copy position in the destination buffer by one data type chunk */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        DestinationDataPosition = (uint16)(DataElementsIndex*SizeOfDataType);
#else
        if (Endianness != DCM_OPAQUE_ENDIAN)
        {
          DestinationDataPosition = (uint16)(DataElementsIndex*SizeOfDataType + ((uint16)DCM_GET_BYTES((uint16)SizeOfDataType) - 1U)*DCM_BYTE_BITS);
        }
        else
        {
          DestinationDataPosition = (uint16)(DataElementsIndex*SizeOfDataType);
        }
#endif /*(CPU_BYTE_ORDER == LOW_BYTE_FIRST)*/
      }

      /* The position in the source for Write Did operations is the configured DataPosition
       * After each copy it is recalculated */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.Start.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.Start.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      SourceDataPosition = CurrentBitIndex;
    }

    /* Copy data from source to destination at the calculated positions */
    Dcm_DspInternal_CopySignalData(
      SourceBuffer, DestinationBuffer, SourceDataPosition, DestinationDataPosition, SizeToCopy, DataCopyMethod, ByteAdvanceDirection);

    /* Calculate the remaining data length for the next iteration */
    if (RemainingDataLength >= SizeOfDataType)
    {
      RemainingDataLength -= SizeOfDataType;
    }
    else
    {
      RemainingDataLength = SizeOfDataType - RemainingDataLength;
    }

    /* Calculate the current bit index - needed also for keeping track of sign extension */
    if (Endianness == DCM_BIG_ENDIAN)
    {
      /* From the "virtual" position where the bit index would be for a non big endian copy (DataPosition+SizeToCopy),
       * translate the index in the other direction with 2 times the number of bytes that the data copy occupies.
       * 2 times because the index was first translated into one direction and to "mirror" it in the other direction
       * the occupied data needs to be substracted twice */
      CurrentBitIndex = (CurrentBitIndex+SizeToCopy) -
          ((((SizeToCopy+(DataPosition%DCM_BYTE_BITS))/DCM_BYTE_BITS)*DCM_ENDIANNESSINTERPRETATION_2BYTES_OFFSET)*DCM_BYTE_BITS);
    }
    else
    {
      /* For little endian and opaque signals the bit index is incremented with the size which was copied */
      CurrentBitIndex += SizeToCopy;
    }
  }

    /* For data copy from variable to buffer
     * check if the data to be copied to the output buffer has a configured size bigger than the size of data type
     * in which case the S/R interface copies into the buffer a chunk of data with the size of data type size
     * The rest of the bits up to the data size need to be filled out according to the source signedness to preserve the sign
     * in the destination buffer */
  if ((Endianness != DCM_OPAQUE_ENDIAN) &&
       (Direction == DCM_ENDIANNESSINTERPRETATION_COPY_DATA_TO_OUT_BUFFER) &&
       (RemainingDataLength > 0U) && DCM_IS_SIGNED_TYPE(DataType))
  {
    /* 0 or 1 depending on the sign */
    uint8 FillValue;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    uint16 MostSignificantByteOfSignal = (uint16)DCM_GET_BYTES((uint16)SizeOfDataType) - 1U;
#else
    uint16 MostSignificantByteOfSignal = 0U;
#endif /*(CPU_BYTE_ORDER == LOW_BYTE_FIRST)*/

    /* If the msb of the signal is 1 fill with 1, otherwise with 0
     * No need to check msb in case of array types since in that case the application will provide the necessary
     * number of array elements to fill the signal */
    if ((SourceBuffer[MostSignificantByteOfSignal] & DCM_ENDIANNESSINTERPRETATION_MSB_MASK) != 0U)
    {
      FillValue = DCM_UINT8_MAX;
    }
    else
    {
      FillValue = 0x00U;
    }

    /* Fill the source buffer with the desired number of 0 or 1 bits
     * This method is used to easily copy it afterwards into the desired position in the destination
     * using Dcm_DspInternal_CopySignalData */
    TS_MemSet(SourceBuffer, FillValue, (uint32)DCM_GET_BYTES((uint32)RemainingDataLength));

    /* Copy the source buffer (which now contains 0 or 1 depending on the signal signedness into
     * the desired position (where the initial data copy has ended) in the destination */
    /* !LINKSTO Dcm.EB.EndiannessInterpretation.CopyDataToBuffer.NegativeSignExtension,1 */
    /* !LINKSTO Dcm.EB.EndiannessInterpretation.CopyDataToBuffer.PositiveSignExtension,1 */
    Dcm_DspInternal_CopySignalData(
      SourceBuffer, DestinationBuffer, 0U, CurrentBitIndex, RemainingDataLength, DataCopyMethod, ByteAdvanceDirection);
  }

  DBG_DCM_DSP_ENDIANNESSINTERPRETATION_COPYDATA_EXIT(
    SignalBuffer,Direction,DestinationBuffer,Endianness,DataType,DataLength,DataPosition);
}

/*==================[internal function definitions]==============================================*/

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_EndiannessInterpretation_GetSignalTypeAndSizeInformation(
  Dcm_DidDataByteType DataType,
  uint16 DataLength,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SizeOfDataType,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) DataArraySize)
{
  /* Variable for the number of bytes needed to store the data.
   * This is further used for calculating the number of array elements when the data has an array type */
  uint16 DataArrayLengthInBytes = DCM_GET_BYTES(DataLength);

  switch (DataType)
  {
    case DCM_TYPE_BOOLEAN:
    case DCM_TYPE_UINT8:
    case DCM_TYPE_SINT8:
    {
     *SizeOfDataType = DCM_ENDIANNESSINTERPRETATION_TYPESIZE_8BITS;
     /* If the data type is not an array type consider it as an 1 element array */
     *DataArraySize = 1U;
     break;
    }
    case DCM_TYPE_UINT8_N:
    case DCM_TYPE_SINT8_N:
    {
     *SizeOfDataType = DCM_ENDIANNESSINTERPRETATION_TYPESIZE_8BITS;
     *DataArraySize = DataArrayLengthInBytes;
     break;
    }
    case DCM_TYPE_UINT16:
    case DCM_TYPE_SINT16:
    {
     *SizeOfDataType = DCM_ENDIANNESSINTERPRETATION_TYPESIZE_16BITS;
     /* If the data type is not an array type consider it as an 1 element array */
     *DataArraySize = 1U;
     break;
    }
    case DCM_TYPE_UINT16_N:
    case DCM_TYPE_SINT16_N:
    {
      *SizeOfDataType = DCM_ENDIANNESSINTERPRETATION_TYPESIZE_16BITS;
      *DataArraySize = (uint16)(DCM_GET_WORDS((uint32)DataLength));
     break;
    }
    case DCM_TYPE_UINT32:
    case DCM_TYPE_SINT32:
    {
     *SizeOfDataType = DCM_ENDIANNESSINTERPRETATION_TYPESIZE_32BITS;
     /* If the data type is not an array type consider it as an 1 element array */
     *DataArraySize = 1U;
     break;
    }
    case DCM_TYPE_UINT32_N:
    case DCM_TYPE_SINT32_N:
    {
      *SizeOfDataType = DCM_ENDIANNESSINTERPRETATION_TYPESIZE_32BITS;
      *DataArraySize = DCM_GET_DWORDS(DataLength);
     break;
    }
    /* CHECK: NOPARSE */
    default:
    {
     /* Defensive branch. Default case at the end of the switch statement */
     DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_EndiannessInterpretation_GetSignalMostSignificantBitPosition(
  uint16 DataLength,
  Dcm_EndiannessType Endianness,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) MostSignificantBitOfSignal)
{
  if (Endianness == DCM_BIG_ENDIAN)
  {
    /* Stores the remaining data length when calculating the position of the signal's msb
     * For the beginning consider it as being at the start of the signal */
    uint16 RemainingData = DataLength;

    /* Calculate the position of the signal's msb */
    while (RemainingData > 0)
    {
      /* If the current determined position plus the data length does not go over a byte boundary,
       * the msb is in the same byte as the current position */
      if ((((*MostSignificantBitOfSignal)%DCM_BYTE_BITS) + RemainingData) <= DCM_BYTE_BITS)
      {
        *MostSignificantBitOfSignal = ((*MostSignificantBitOfSignal) + RemainingData) - 1U;
        RemainingData = 0U;
      }
      else
      {
        /* How many bits are from the current determined position up to the byte boundary */
        uint8 BitsUntilByteBoundary = DCM_BYTE_BITS - ((*MostSignificantBitOfSignal)%DCM_BYTE_BITS);

        /* From the remaining data length remove how many bits are from the current determined position up to the byte boundary */
        RemainingData = RemainingData - BitsUntilByteBoundary;

        /* If the current determined position plus the data length goes over a byte boundary,
         * the msb is in the previous byte (--) as the current position
         * Calculate the msb position as being the current determined position plus how many bits are up to the byte boundary
         * and go 2 bytes back */
        *MostSignificantBitOfSignal = ((*MostSignificantBitOfSignal) + BitsUntilByteBoundary) - (DCM_ENDIANNESSINTERPRETATION_2BYTES_OFFSET*DCM_BYTE_BITS);
      }
    }
  }
  else
  {
    /* For little endian and opaque endianness the position is to the (++) byte(s) */
    *MostSignificantBitOfSignal = ((*MostSignificantBitOfSignal) + DataLength) - 1U;
  }
}

STATIC FUNC(Dcm_DataCopyMethodType, DCM_CODE) Dcm_Dsp_EndiannessInterpretation_GetDataCopyMethod(
  Dcm_EndiannessType Endianness,
  Dcm_DataCopyDirectionType Direction)
{
  Dcm_DataCopyMethodType DataCopyMethod = DCM_COPY_TO_OUTBUFFER_BIGENDIAN;

  switch (Endianness)
  {
    case DCM_BIG_ENDIAN:
    {
      if (Direction == DCM_ENDIANNESSINTERPRETATION_COPY_DATA_TO_OUT_BUFFER)
      {
        DataCopyMethod = DCM_COPY_TO_OUTBUFFER_BIGENDIAN;
      }
      else
      {
        DataCopyMethod = DCM_COPY_FROM_INBUFFER_BIGENDIAN;
      }
      break;
    }
    case DCM_LITTLE_ENDIAN:
    {
      if (Direction == DCM_ENDIANNESSINTERPRETATION_COPY_DATA_TO_OUT_BUFFER)
      {
        DataCopyMethod = DCM_COPY_TO_OUTBUFFER_LITTLEENDIAN;
      }
      else
      {
        DataCopyMethod = DCM_COPY_FROM_INBUFFER_LITTLEENDIAN;
      }
      break;
    }
    case DCM_OPAQUE_ENDIAN:
    {
      if (Direction == DCM_ENDIANNESSINTERPRETATION_COPY_DATA_TO_OUT_BUFFER)
      {
        DataCopyMethod = DCM_COPY_TO_OUTBUFFER_OPAQUE;
      }
      else
      {
        DataCopyMethod = DCM_COPY_FROM_INBUFFER_OPAQUE;
      }
      break;
    }
    /* CHECK: NOPARSE */
    default:
    {
     /* Defensive branch. Default case at the end of the switch statement */
     DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }

  return DataCopyMethod;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /*#if ((DCM_DSP_USE_DIDSERVICES == STD_ON) && (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON))*/
/*==================[end of file]================================================================*/
