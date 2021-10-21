/**
 * \file
 *
 * \brief AUTOSAR E2EPW
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EPW.
 *
 * \version 2.3.20
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to
 * object.
 *
 * Reason:
 * The E2E Protection Wrapper uses generic interfaces to (de-)serialize
 * complex data of different types.
 *
 */

/*==[Includes]================================================================*/

#include <Std_Types.h>          /* AUTOSAR Std types */
#include <E2EPW.h>              /* Module public API and configuration */
#include <E2EPW_Int_Cfg.h>      /* E2EPW PreCompile parameters */
#include <E2E.h>                /* AUTOSAR E2E types */
#include <Rte.h>                /* AUTOSAR Rte types */
#include <E2EPW_Int_Stc.h>      /* E2EPW static internals */

#if (E2EPW_ENABLE_PROFILE_P01 == STD_ON)
#include <E2E_P01.h>           /* E2E Profile 01 */
#endif /* (E2EPW_ENABLE_PROFILE_P01 == STD_ON) */

#if (E2EPW_ENABLE_PROFILE_P02 == STD_ON)
#include <E2E_P02.h>            /* E2E Profile 02 */
#endif /* (E2EPW_ENABLE_PROFILE_P02 == STD_ON) */

#if (E2EPW_ENABLE_PROFILE_PDAI1 == STD_ON)
#include <E2E_PDAI1.h>          /* E2E Profile DAI1 */
#endif /* (E2EPW_ENABLE_PROFILE_PDAI1 == STD_ON) */

#if (E2EPW_ENABLE_PROFILE_PDAI2 == STD_ON)
#include <E2E_PDAI2.h>          /* E2E Profile DAI2 */
#endif /* (E2EPW_ENABLE_PROFILE_PDAI2 == STD_ON) */

/*==[Macros]==================================================================*/

/* pre-compile time sanity checks to ensure that 8-bit return values are used */
#if (E2E_E_OK >= 256U)
#error E2E_E_OK must have a value smaller than 256
#endif

#if (E2E_E_INTERR >= 256U)
#error E2E_E_INTERR must have a value smaller than 256
#endif

#if (E2E_E_INPUTERR_WRONG >= 256U)
#error E2E_E_INPUTERR_WRONG must have a value smaller than 256
#endif

#if (E2E_E_INPUTERR_NULL >= 256U)
#error E2E_E_INPUTERR_NULL must have a value smaller than 256
#endif

#if (E2EPW_E_REDUNDANCY >= 256U)
#error E2EPW_E_REDUNDANCY must have a value smaller than 256
#endif

#if (E2EPW_E_DESERIALIZATION >= 256U)
#error E2EPW_E_DESERIALIZATION must have a value smaller than 256
#endif

#if (E2EPW_STATUS_OK >= 256U)
#error E2EPW_STATUS_OK must have a value smaller than 256
#endif

#if (E2EPW_STATUS_OKSOMELOST >= 256U)
#error E2EPW_STATUS_OKSOMELOST must have a value smaller than 256
#endif

#if (E2EPW_STATUS_NONEWDATA >= 256U)
#error E2EPW_STATUS_NONEWDATA must have a value smaller than 256
#endif

#if (E2EPW_STATUS_WRONGCRC >= 256U)
#error E2EPW_STATUS_WRONGCRC must have a value smaller than 256
#endif

#if (E2EPW_STATUS_INITIAL >= 256U)
#error E2EPW_STATUS_INITIAL must have a value smaller than 256
#endif

#if (E2EPW_STATUS_REPEATED >= 256U)
#error E2EPW_STATUS_REPEATED must have a value smaller than 256
#endif

#if (E2EPW_STATUS_WRONGSEQUENCE >= 256U)
#error E2EPW_STATUS_WRONGSEQUENCE must have a value smaller than 256
#endif

#if (E2EPW_STATUS_DATAINVALID >= 256U)
#error E2EPW_STATUS_DATAINVALID must have a value smaller than 256
#endif

#if (E2EPW_STATUS_SYNC >= 256U)
#error E2EPW_STATUS_SYNC must have a value smaller than 256
#endif


/* Deviation MISRAC2012-1 <START> */

/*==[Types]===================================================================*/

/*==[Declaration of functions with internal linkage]==========================*/

#if ((E2EPW_ENABLE_PROFILE_P01 == STD_ON) || \
     (E2EPW_ENABLE_PROFILE_P02 == STD_ON) ||  \
     (E2EPW_ENABLE_PROFILE_PDAI1 == STD_ON) ||  \
     (E2EPW_ENABLE_PROFILE_PDAI2 == STD_ON))

#define E2EPW_START_SEC_CODE
#include <MemMap.h>

/**
 * \brief Initialize a Data Element including padding bytes with a given Pattern
 *
 * \pre The Data Element \a DePtr must be of size \a NBytes
 *
 * \param[inou] DePtr Pointer to Data Element
 * \param[in] NBytes The number of bytes which shall be copied
 * \param[in] Pattern The Pattern which is used to fill the Data Element
 */
static FUNC(void, E2EPW_CODE) E2EPW_Init_De
(
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) DePtr,
   uint16 NBytes,
   uint8 Pattern
);

/*==[Definition of Constants with internal linkage]===========================*/

/*==[Definition of Variables with internal linkage]===========================*/

/*==[Definition of Constants with external linkage]===========================*/

/*==[Definition of Variables with external linkage]===========================*/

/*==[Definition of functions with external linkage]===========================*/

FUNC(uint32, E2EPW_CODE) E2EPW_GenericRead
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) data,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   uint32 RetVal;

   if (NULL_PTR == (data))
   {
      /* Byte0=retRead, Byte1=retVerify, Byte2=RetCheck, Byte3=RetBitExt */
      RetVal = (uint32)((uint32)(RTE_E_OK) | ((uint32)((uint32)E2E_E_INPUTERR_NULL)<< ((uint8)8)) |
            ((uint32)((uint32)E2E_E_OK)<< ((uint8)16)) | ((uint32)((uint32)E2E_E_OK)<< ((uint8)24)));
   }
   else
   {
      /* Step R2.1: The E2EPW calls Rte_IsUpdated() (optionally) and Rte_Read_<p>_<o>() */
 #if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
      /* Note: 2nd and 4d arguments are unused, but required due to generic function type */
      RetVal = (uint32)(Config->RteReadWriteFunc(data, NULL_PTR, 0U));
 #else
      RetVal = (uint32)(Config->RteReadWriteFunc(data));
 #endif
      /* call profile specific generic E2EPW code which does the rest */
      RetVal |= Config->E2EPW_InnerFunc(Config, data, SerDataDEData);
   }

   return RetVal;
}

FUNC(uint32, E2EPW_CODE) E2EPW_GenericWrite
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) data,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
   , uint16 SerDataLength
#endif
)
{
   uint32 RetVal;

   if (NULL_PTR == data)
   {
      /* Byte0=retWrite, Byte1=retVerify, Byte2=RetProtect, Byte3=RetBitExt */
      RetVal = (uint32)((uint32)(RTE_E_OK) | ((uint32)((uint32)E2E_E_INPUTERR_NULL)<< ((uint8)8)) |
            ((uint32)((uint32)E2E_E_OK)<< ((uint8)16)) | ((uint32)((uint32)E2E_E_OK)<< ((uint8)24)));
   }
   else
   {
      /* call profile specific generic E2EPW code which does the rest */
      RetVal = Config->E2EPW_InnerFunc(Config, data, SerDataDEData);
      /* Only in case of successful execution, the data is transmitted through RTE */
      if (0U == RetVal)
      {
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
         RetVal |= ((uint32)(Config->RteReadWriteFunc(data, SerDataDEData, SerDataLength)));
#else
         RetVal |= ((uint32)(Config->RteReadWriteFunc(data)));
#endif
      }
   }

   return RetVal;
}

FUNC(uint32, E2EPW_CODE) E2EPW_GenericWrite1
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) data,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   uint32 RetVal;

   if (NULL_PTR == data)
   {
      /* Byte0=retWrite, Byte1=retVerify, Byte2=RetProtect, Byte3=RetBitExt */
      RetVal = (uint32)((uint32)(RTE_E_OK) | ((uint32)((uint32)E2E_E_INPUTERR_NULL)<< ((uint8)8)) |
            ((uint32)((uint32)E2E_E_OK)<< ((uint8)16)) | ((uint32)((uint32)E2E_E_OK)<< ((uint8)24)));
   }
   else
   {
      /* call profile specific generic E2EPW code which does the rest */
      RetVal = Config->E2EPW_InnerFunc(Config, data, SerDataDEData);
   }

   return RetVal;
}

#if (E2EPW_ENABLE_PROFILE_P01 == STD_ON)

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerRead_P01
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_P01ConfigType, AUTOMATIC, E2EPW_CONST) E2E_P01Config =
            (P2CONST(E2E_P01ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_P01ReceiverStateType, AUTOMATIC, E2EPW_VAR) E2E_P01ReceiverState =
            (P2VAR(E2E_P01ReceiverStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   uint32 RetCheck, RetBitExt, RetStatus;
   uint8 RangeCheckResult;

#if (E2EPW_ENABLE_PROFILE_PVCC == STD_ON)
   /* Initialize data memory with 0 according to EB_E2EPW020413 */
   E2EPW_Init_De(SerData, (uint16)(E2E_P01Config->DataLength / 8U), 0x00U);
#else
   /* Initialize data memory with 0xFF,
   * thus all bits in unused bit areas are set to one (E2EUSE173) */
   E2EPW_Init_De(SerData, (uint16)(E2E_P01Config->DataLength / 8U), 0xFFU);
#endif
   /* Step R3A/R13A: Serialize Data Element into uint8 array */
   /* Perform range checks on Data Element members */
   RangeCheckResult = Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Step R4/R14: Call the E2E library E2E_PXXCheck() to detect possible communication errors */
   RetCheck = E2E_P01Check(E2E_P01Config, E2E_P01ReceiverState, SerData);
   /* Map profile specific status values to E2EPW return values */
#if (E2EPW_ENABLE_PROFILE_PVCC == STD_ON)
   if (E2E_P01ReceiverState->Status == E2E_P01STATUS_DATAINVALID)
   {
      RetStatus = (uint32) E2EPW_STATUS_WRONGCRC;
      RetCheck = E2E_E_INPUTERR_WRONG;
   }
   else
#endif
   {
      RetStatus = (uint32) E2E_P01ReceiverState->Status;
   }
   /* Step R7A/R17A: If original and temporary Data Element differ,
   * a de-serialization error occurred */
   if (E2EPW_RANGECHK_VALID == RangeCheckResult)
   {
      /* Defensive behavior check: Only status values smaller than 256 can be returned. */
      if (RetStatus >= 256UL)
      {
         RetBitExt = (uint32)((uint32) E2E_E_INTERR) << ((uint8)8);
      }
      else
      {
         RetBitExt = ((uint32) E2E_E_OK) << ((uint8)8);
      }
   }
   else
   {
      RetBitExt = (uint32)((uint32) E2EPW_E_DESERIALIZATION) << ((uint8)8);
   }
   return (uint32)((uint32)(RetBitExt | (RetCheck << 16U)) | (uint32)(RetStatus << 24U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerRead_P01
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   return E2EPW_CX_IR_GenericInnerRead_P01(Config, AppData, SerDataDEData);
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite_P01
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_P01ConfigType, AUTOMATIC, E2EPW_CONST) E2E_P01Config =
         (P2CONST(E2E_P01ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_P01SenderStateType, AUTOMATIC, E2EPW_VAR) E2E_P01SenderState =
         (P2VAR(E2E_P01SenderStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 DeCrcOffs = Config->DE_CRCoffs;
   const uint16 DeCntOffs = Config->DE_CNToffs;
   const uint16 DeDIdNibbleOffs = Config->DE_DIDNibbleOffs;
   const uint8 ProfileCrcByteOffs = (uint8)(((uint16)(E2E_P01Config->CRCOffset / 8U)) & 0xFFU);
   const uint8 ProfileCntByteOffs = (uint8)(((uint16)(E2E_P01Config->CounterOffset / 8U)) & 0xFFU);
   const uint8 ProfileCntBitShift = ((uint16)(E2E_P01Config->CounterOffset % 8U) == 0U) ? 0U : 4U;
   uint32 RetProtect, retVerify;
   uint8 RangeCheckResult;

#if (E2EPW_ENABLE_PROFILE_PVCC == STD_ON)
   /* Initialize data memory with 0 according to EB_E2EPW020413 */
   E2EPW_Init_De(SerData, (uint16)(E2E_P01Config->DataLength / 8U), 0x00U);
#else
   /* Initialize data memory with 0xFF,
   * thus all bits in unused bit areas are set to one (E2EUSE173) */
   E2EPW_Init_De(SerData, (uint16)(E2E_P01Config->DataLength / 8U), 0xFFU);
#endif
   /* Step S3A: Serialize Data Element into uint8 array */
   /* Perform range checks on Data Element members except for Crc, Counter Member, and
    * DataID Nibble Member (if used)
    * which can be ensured by setting the byte allocated to the Counter and DataID Nibble to 0.
    * Note: If DataID Nibble member does not exist, then the offset value equals that of
    * the CRC struct member. */
   AppDataUint8[DeCntOffs] = 0U;
   AppDataUint8[DeDIdNibbleOffs] = 0U;
   RangeCheckResult = Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Step S4: Call the E2E library E2E_PXXProtect() to protect the data element */
   RetProtect = E2E_P01Protect(E2E_P01Config, E2E_P01SenderState, SerData);
   /* Step S5: Write Crc and Counter calculated by the E2E library back to the Data Element */
   AppDataUint8[DeCrcOffs] = SerData[ProfileCrcByteOffs];
   AppDataUint8[DeCntOffs] =
       (uint8)((uint8)(SerData[ProfileCntByteOffs] >> ProfileCntBitShift)& 0x0FU);
   /* If DataID nibble mode is used, additionally write back DataID Nibble to the Data Element */
   if (E2E_P01Config->DataIDMode == E2E_P01_DATAID_NIBBLE)
   {
      const uint8 ProfileDataIDNibbleByteOffs =
          (uint8)(((uint16)(E2E_P01Config->DataIDNibbleOffset / 8U)) & 0xFFU);
      const uint8 ProfileDataIDNibbleBitShift =
          ((uint16)(E2E_P01Config->DataIDNibbleOffset % 8U) == 0U) ? 0U : 4U;
      AppDataUint8[DeDIdNibbleOffs] = (uint8)
          ((uint8)(SerData[ProfileDataIDNibbleByteOffs] >> ProfileDataIDNibbleBitShift)& 0x0FU);
   }
   /* Step S7A: If original and temporary Data Element differ, incorrect input data is provided */
   retVerify = (E2EPW_RANGECHK_VALID == RangeCheckResult) ? (((uint32)((uint32) E2E_E_OK) << ((uint8)8)))
         : (((uint32)((uint32) E2E_E_INPUTERR_WRONG) << ((uint8)8)));
   /* Note: Always return 0 (E2E_E_OK) in Byte 3 */
   return (uint32)(retVerify | (uint32)(RetProtect << 16U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite_P01
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   return E2EPW_CX_IR_GenericInnerWrite_P01(Config, AppData, SerDataDEData);
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite2_P01
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_P01ConfigType, AUTOMATIC, E2EPW_CONST) E2E_P01Config =
         (P2CONST(E2E_P01ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_P01SenderStateType, AUTOMATIC, E2EPW_VAR) E2E_P01SenderState =
         (P2VAR(E2E_P01SenderStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 DeCrcOffs = Config->DE_CRCoffs;
   const uint16 DeCntOffs = Config->DE_CNToffs;
   const uint16 DeDIdNibbleOffs = Config->DE_DIDNibbleOffs;
   const uint8 CrcVal = AppDataUint8[DeCrcOffs];
   const uint8 CntVal = AppDataUint8[DeCntOffs];
   const uint8 DIdNibbleVal = AppDataUint8[DeDIdNibbleOffs];
   const uint8 ProfileCrcByteOffs = (uint8)(((uint16)(E2E_P01Config->CRCOffset / 8U))&0xFFU);
   const uint8 ProfileCntByteOffs = (uint8)(((uint16)(E2E_P01Config->CounterOffset / 8U))&0xFFU);
   const uint8 ProfileCntBitShift = ((uint16)(E2E_P01Config->CounterOffset % 8U) == 0U) ? 0U : 4U;
   const uint8 ProfileDataIDNibbleByteOffs =
       (uint8)(((uint16)(E2E_P01Config->DataIDNibbleOffset / 8U)) & 0xFFU);
   const uint8 ProfileDataIDNibbleBitShift =
       ((uint16)(E2E_P01Config->DataIDNibbleOffset % 8U) == 0U) ? 0U : 4U;
   uint32 RetProtect, RetVote;
   uint8 RangeCheckResult;

#if (E2EPW_ENABLE_PROFILE_PVCC == STD_ON)
   /* Initialize data memory with 0 according to EB_E2EPW020413 */
   E2EPW_Init_De(SerData, (uint16)(E2E_P01Config->DataLength / 8U), 0x00U);
#else
   /* Initialize data memory with 0xFF,
   * thus all bits in unused bit areas are set to one (E2EUSE173) */
   E2EPW_Init_De(SerData, (uint16)(E2E_P01Config->DataLength / 8U), 0xFFU);
#endif
   /* Step S13A: Serialize Data Element into uint8 array */
   /* Perform range checks on Data Element members */
   RangeCheckResult = Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Step S14: Call the E2E library E2E_PXXProtect() to protect the data element */
   RetProtect = E2E_P01Protect(E2E_P01Config, E2E_P01SenderState, SerData);
   /* Step S17: Votes on the equality of the control and data fields */
   RangeCheckResult |= ((CrcVal == SerData[ProfileCrcByteOffs]) &&
       (CntVal == (uint8)((uint8)(SerData[ProfileCntByteOffs] >> ProfileCntBitShift) & 0x0FU))) ?
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;
   /* If DataID nibble mode is used, additionally vote on equality of DataId Nibble value */
   if (E2E_P01Config->DataIDMode == E2E_P01_DATAID_NIBBLE)
   {
      RangeCheckResult |= (DIdNibbleVal == (uint8)
          ((uint8)(SerData[ProfileDataIDNibbleByteOffs] >> ProfileDataIDNibbleBitShift) & 0x0FU)) ?
              E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;
   }
   /* If original and temporary Data Element differ, redundancy check failed */
   if (E2EPW_RANGECHK_VALID == RangeCheckResult)
   {
      RetVote = (uint32)((uint32) E2E_E_OK) << ((uint8)8);
   }
   else
   {
      RetVote = (uint32)((uint32) E2EPW_E_REDUNDANCY) << ((uint8)8);
   }
   /* Note: Always return 0 (E2E_E_OK) in Byte 3 */
   return (uint32)(RetVote | (RetProtect << 16U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite2_P01
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   return E2EPW_CX_IR_GenericInnerWrite2_P01(Config, AppData, SerDataDEData);
}

#endif /* (E2EPW_ENABLE_PROFILE_P01 == STD_ON) */

#if (E2EPW_ENABLE_PROFILE_P02 == STD_ON)

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerRead_P02
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_P02ConfigType, AUTOMATIC, E2EPW_CONST) E2E_P02Config =
         (P2CONST(E2E_P02ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_P02ReceiverStateType, AUTOMATIC, E2EPW_VAR) E2E_P02ReceiverState =
         (P2VAR(E2E_P02ReceiverStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   uint32 RetCheck, RetBitExt, RetStatus;
   uint8 RangeCheckResult;

   /* Initialize data memory with 0xFF,
    * thus all bits in unused bit areas are set to one (E2EUSE173) */
   E2EPW_Init_De(SerData, (uint16)(E2E_P02Config->DataLength / 8U), 0xFFU);
   /* Step R3A/R13A: Serialize Data Element into uint8 array */
   /* Perform range checks on Data Element members */
   RangeCheckResult = Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Step R4/R14: Call the E2E library E2E_PXXCheck() to detect possible communication errors */
   RetCheck = E2E_P02Check(E2E_P02Config, E2E_P02ReceiverState, SerData);
   /* Map profile specific status values to E2EPW return values */
   RetStatus = (uint32) E2E_P02ReceiverState->Status;
   /* Step R7A/R17A: If original and temporary Data Element differ,
    * a de-serialization error occurred */
   if (E2EPW_RANGECHK_VALID == RangeCheckResult)
   {
      /* Defensive behavior check: Only status values smaller than 256 can be returned. */
      if (RetStatus >= 256UL)
      {
         RetBitExt = (uint32)((uint32) E2E_E_INTERR) << ((uint8)8);
      }
      else
      {
         RetBitExt = ((uint32) E2E_E_OK) << ((uint8)8);
      }
   }
   else
   {
      RetBitExt = (uint32)((uint32) E2EPW_E_DESERIALIZATION) << ((uint8)8);
   }
   return (uint32)((uint32)(RetBitExt | (RetCheck << 16U)) | (uint32)(RetStatus << 24U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerRead_P02
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_P02ConfigType, AUTOMATIC, E2EPW_CONST) E2E_P02Config =
         (P2CONST(E2E_P02ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_P02ReceiverStateType, AUTOMATIC, E2EPW_VAR) E2E_P02ReceiverState =
         (P2VAR(E2E_P02ReceiverStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerApplData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 DeCrcOffs = Config->DE_CRCoffs;
   const uint16 DeCntOffs = Config->DE_CNToffs;
   const uint8 CrcVal = AppDataUint8[DeCrcOffs];
   const uint8 CntVal = AppDataUint8[DeCntOffs];
   uint32 RetCheck, RetStatus, RetE2EPW;

   /* Initialize data memory, thus all padding bytes are set to 0 */
   E2EPW_Init_De(SerApplData, Config->DE_Size, 0U);
   /* Copy application Data Element member-wise to temporary Data Element */
   /* Range check result can be ignored here */
   (void)Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Swap the memory of the Crc/Counter struct member
    * with that of the profile-specific position */
   SerApplData[DeCrcOffs] = SerApplData[0];
   SerApplData[DeCntOffs] = SerApplData[1];
   SerApplData[0] = CrcVal;
   SerApplData[1] = CntVal;
   /* Step R4/R14: Call the E2E library E2E_PXXCheck() to detect possible communication errors */
   RetCheck = E2E_P02Check(E2E_P02Config, E2E_P02ReceiverState, SerApplData);
   /* Map profile specific status values to E2EPW return values */
   RetStatus = (uint32) E2E_P02ReceiverState->Status;
   /* Defensive behavior check: Only status values smaller than 256 can be returned. */
   if (RetStatus >= 256UL)
   {
      RetE2EPW = (uint32)((uint32) E2E_E_INTERR) << ((uint8)8);
   }
   else
   {
      RetE2EPW = (uint32)((uint32) E2E_E_OK) << ((uint8)8);
   }
   return (uint32)((uint32)(RetE2EPW | (RetCheck << 16U)) | (uint32)(RetStatus << 24U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite_P02
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_P02ConfigType, AUTOMATIC, E2EPW_CONST) E2E_P02Config =
         (P2CONST(E2E_P02ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_P02SenderStateType, AUTOMATIC, E2EPW_VAR) E2E_P02SenderState =
         (P2VAR(E2E_P02SenderStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   uint32 RetProtect, retVerify;
   uint8 RangeCheckResult;

   /* Initialize data memory with 0xFF,
    * thus all bits in unused bit areas are set to one (E2EUSE173) */
   E2EPW_Init_De(SerData, (uint16)(E2E_P02Config->DataLength / 8U), 0xFFU);
   /* Step S3A: Serialize Data Element into uint8 array */
   /* Perform range checks on Data Element members except for Crc and Counter Member
    * which can be ensured by setting the byte allocated to the Counter Member to 0. */
   AppDataUint8[Config->DE_CNToffs] = 0U;
   RangeCheckResult = Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Step S4: Call the E2E library E2E_PXXProtect() to protect the data element */
   RetProtect = E2E_P02Protect(E2E_P02Config, E2E_P02SenderState, SerData);
   /* Step S5: Write Crc and Counter calculated by the E2E library back to the Data Element */
   AppDataUint8[Config->DE_CRCoffs] = SerData[0];
   AppDataUint8[Config->DE_CNToffs] = SerData[1] & 0x0FU;
   /* Step S7A: If original and temporary Data Element differ, incorrect input data is provided */
   retVerify = (E2EPW_RANGECHK_VALID == RangeCheckResult) ? ((uint32)((uint32) E2E_E_OK) << ((uint8)8))
         : ((uint32)((uint32) E2E_E_INPUTERR_WRONG) << ((uint8)8));
   /* Note: Always return 0 (E2E_E_OK) in Byte 3 */
   return (uint32)(retVerify | (uint32)(RetProtect << 16U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite_P02
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_P02ConfigType, AUTOMATIC, E2EPW_CONST) E2E_P02Config =
         (P2CONST(E2E_P02ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_P02SenderStateType, AUTOMATIC, E2EPW_VAR) E2E_P02SenderState =
         (P2VAR(E2E_P02SenderStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerApplData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 DeCrcOffs = Config->DE_CRCoffs;
   const uint16 DeCntOffs = Config->DE_CNToffs;
   uint32 RetProtect;

   /* Initialize data memory, thus all padding bytes are set to 0 */
   E2EPW_Init_De(SerApplData, Config->DE_Size, 0U);
   /* Copy application Data Element member-wise to temporary Data Element */
   /* Range check result can be ignored here */
   (void)Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Swap the memory of the Crc/Counter struct member with that of the profile-specific position */
   SerApplData[DeCrcOffs] = SerApplData[0];
   SerApplData[DeCntOffs] = SerApplData[1];
   /* Crc/Counter Byte can be set to 0 (nothing is read) */
   SerApplData[0] = 0x00U;
   SerApplData[1] = 0x00U;
   /* Step S4: Call the E2E library E2E_PXXProtect() to protect the data element */
   RetProtect = E2E_P02Protect(E2E_P02Config, E2E_P02SenderState, SerApplData);
   /* Step S5: Write Crc and Counter calculated by the E2E library back to the Data Element */
   AppDataUint8[DeCrcOffs] = SerApplData[0];
   AppDataUint8[DeCntOffs] = SerApplData[1] & 0x0FU;
   /* Note: Always return 0 (E2E_E_OK) in Bytes 1 and 3 */
   return (uint32)(RetProtect << 16U);
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite2_P02
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_P02ConfigType, AUTOMATIC, E2EPW_CONST) E2E_P02Config =
         (P2CONST(E2E_P02ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_P02SenderStateType, AUTOMATIC, E2EPW_VAR) E2E_P02SenderState =
         (P2VAR(E2E_P02SenderStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 DeCrcOffs = Config->DE_CRCoffs;
   const uint16 DeCntOffs = Config->DE_CNToffs;
   const uint8 CrcVal = AppDataUint8[DeCrcOffs];
   const uint8 CntVal = AppDataUint8[DeCntOffs];
   uint32 RetProtect, RetVote;
   uint8 RangeCheckResult;

   /* Initialize data memory with 0xFF,
    * thus all bits in unused bit areas are set to one (E2EUSE173) */
   E2EPW_Init_De(SerData, (uint16)(E2E_P02Config->DataLength / 8U), 0xFFU);
   /* Step S13A: Serialize Data Element into uint8 array */
   /* Perform range checks on Data Element members */
   RangeCheckResult = Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Step S14: Call the E2E library E2E_PXXProtect() to protect the data element */
   RetProtect = E2E_P02Protect(E2E_P02Config, E2E_P02SenderState, SerData);
   /* Step S17: Votes on the equality of the control and data fields */
   RangeCheckResult |= ((CrcVal == SerData[0]) &&
       (CntVal == (uint8)(SerData[1] & 0x0FU))) ?
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;
   /* If original and temporary Data Element differ, redundancy check failed */
   if (E2EPW_RANGECHK_VALID == RangeCheckResult)
   {
      RetVote = (uint32)((uint32) E2E_E_OK) << ((uint8)8);
   }
   else
   {
      RetVote = (uint32)((uint32) E2EPW_E_REDUNDANCY) << ((uint8)8);
   }
   /* Note: Always return 0 (E2E_E_OK) in Byte 3 */
   return (uint32)(RetVote | (RetProtect << 16U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite2_P02
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_P02ConfigType, AUTOMATIC, E2EPW_CONST) E2E_P02Config =
         (P2CONST(E2E_P02ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_P02SenderStateType, AUTOMATIC, E2EPW_VAR) E2E_P02SenderState =
         (P2VAR(E2E_P02SenderStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerApplData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 DeCrcOffs = Config->DE_CRCoffs;
   const uint16 DeCntOffs = Config->DE_CNToffs;
   const uint8 CrcVal = AppDataUint8[DeCrcOffs];
   const uint8 CntVal = AppDataUint8[DeCntOffs];
   uint32 RetProtect, RetVote;

   /* Initialize data memory, thus all padding bytes are set to 0 */
   E2EPW_Init_De(SerApplData, Config->DE_Size, 0U);
   /* Copy application Data Element member-wise to temporary Data Element */
   /* Range check result can be ignored here */
   (void)Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Swap the memory of the Crc/Counter struct member with that of the profile-specific position */
   SerApplData[DeCrcOffs] = SerApplData[0];
   SerApplData[DeCntOffs] = SerApplData[1];
   /* Crc/Counter Byte can be set to 0 (nothing is read) */
   SerApplData[0] = 0x00U;
   SerApplData[1] = 0x00U;
   /* Step S14: Call the E2E library E2E_PXXProtect() to protect the data element */
   RetProtect = E2E_P02Protect(E2E_P02Config, E2E_P02SenderState, SerApplData);
   /* If control Data of original and serialized Data Element differ, redundancy check failed */
   if ((CrcVal != SerApplData[0]) || (CntVal != (uint8)(SerApplData[1] & 0x0FU)))
   {
      RetVote = (uint32)((uint32) E2EPW_E_REDUNDANCY) << ((uint8)8);
   }
   else
   {
      RetVote = (uint32)((uint32) E2E_E_OK) << ((uint8)8);
   }
   /* Note: Always return 0 (E2E_E_OK) in Byte 3 */
   return (uint32)(RetVote | (RetProtect << 16U));
}

#endif /* (E2EPW_ENABLE_PROFILE_P02 == STD_ON) */

#if (E2EPW_ENABLE_PROFILE_PDAI1 == STD_ON)

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerRead_PDAI1
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_PDAI1ConfigType, AUTOMATIC, E2EPW_CONST) E2E_PDAI1Config =
         (P2CONST(E2E_PDAI1ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_PDAI1ReceiverStateType, AUTOMATIC, E2EPW_VAR) E2E_PDAI1ReceiverState =
         (P2VAR(E2E_PDAI1ReceiverStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   uint32 RetCheck, RetBitExt, RetStatus;
   uint8 RangeCheckResult;

   /* Initialize data memory with 0xFF,
    * thus all bits in unused bit areas are set to one (E2EUSE173) */
   E2EPW_Init_De(SerData, (uint16)(E2E_PDAI1Config->DataLength / 8U), 0xFFU);
   /* Step R3A/R13A: Serialize Data Element into uint8 array */
   /* Perform range checks on Data Element members */
   RangeCheckResult = Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Step R4/R14: Call the E2E library E2E_PXXCheck() to detect possible communication errors */
   RetCheck = E2E_PDAI1Check(E2E_PDAI1Config, E2E_PDAI1ReceiverState, SerData);
   /* Map profile specific status values to E2EPW return values */
   RetStatus = (uint32) E2E_PDAI1ReceiverState->Status;
   /* Step R7A/R17A: If original and temporary Data Element differ,
    * a de-serialization error occurred */
   if (E2EPW_RANGECHK_VALID == RangeCheckResult)
   {
      /* Defensive behavior check: Only status values smaller than 256 can be returned. */
      if (RetStatus >= 256UL)
      {
         RetBitExt = (uint32)((uint32) E2E_E_INTERR) << ((uint8)8);
      }
      else
      {
         RetBitExt = (uint32)((uint32) E2E_E_OK) << ((uint8)8);
      }
   }
   else
   {
      RetBitExt = (uint32)((uint32) E2EPW_E_DESERIALIZATION) << ((uint8)8);
   }
   return (uint32)((uint32)(RetBitExt | (RetCheck << 16U)) | (uint32)(RetStatus << 24U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerRead_PDAI1
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_PDAI1ConfigType, AUTOMATIC, E2EPW_CONST) E2E_PDAI1Config =
         (P2CONST(E2E_PDAI1ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_PDAI1ReceiverStateType, AUTOMATIC, E2EPW_VAR) E2E_PDAI1ReceiverState =
         (P2VAR(E2E_PDAI1ReceiverStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerApplData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 DeCrcOffs = Config->DE_CRCoffs;
   const uint16 DeCntOffs = Config->DE_CNToffs;
   const uint8 CrcVal = AppDataUint8[DeCrcOffs];
   const uint8 CntVal = AppDataUint8[DeCntOffs];
   uint32 RetCheck, RetStatus, RetE2EPW;

   /* Initialize data memory, thus all padding bytes are set to 0 */
   E2EPW_Init_De(SerApplData, Config->DE_Size, 0U);
   /* Copy application Data Element member-wise to temporary Data Element */
   /* Range check result can be ignored here */
   (void)Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Swap the memory of the Crc/Counter struct member
    * with that of the profile-specific position */
   SerApplData[DeCrcOffs] = SerApplData[(Config->DE_Size) - 1U];
   SerApplData[DeCntOffs] = SerApplData[(Config->DE_Size) - 2U];
   SerApplData[(Config->DE_Size) - 1U] = CrcVal;
   SerApplData[(Config->DE_Size) - 2U] = (uint8)(CntVal << 4);
   /* Step R4/R14: Call the E2E library E2E_PXXCheck() to detect possible communication errors */
   RetCheck = E2E_PDAI1Check(E2E_PDAI1Config, E2E_PDAI1ReceiverState, SerApplData);
   /* Map profile specific status values to E2EPW return values */
   RetStatus = (uint32) E2E_PDAI1ReceiverState->Status;
   /* Defensive behavior check: Only status values smaller than 256 can be returned. */
   if (RetStatus >= 256UL)
   {
      RetE2EPW = (uint32)((uint32) E2E_E_INTERR) << ((uint8)8);
   }
   else
   {
      RetE2EPW = (uint32)((uint32) E2E_E_OK) << ((uint8)8);
   }
   return (uint32)((uint32)(RetE2EPW | (RetCheck << 16U)) | (uint32)(RetStatus << 24U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite_PDAI1
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_PDAI1ConfigType, AUTOMATIC, E2EPW_CONST) E2E_PDAI1Config =
         (P2CONST(E2E_PDAI1ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_PDAI1SenderStateType, AUTOMATIC, E2EPW_VAR) E2E_PDAI1SenderState =
         (P2VAR(E2E_PDAI1SenderStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 ProfileDataLength = E2E_PDAI1Config->DataLength / 8U;
   uint32 RetProtect, retVerify;
   uint8 RangeCheckResult;

   /* Initialize data memory with 0xFF,
    * thus all bits in unused bit areas are set to one (E2EUSE173) */
   E2EPW_Init_De(SerData, (uint16)(E2E_PDAI1Config->DataLength / 8U), 0xFFU);
   /* Step S3A: Serialize Data Element into uint8 array */
   /* Perform range checks on Data Element members except for Crc and Counter Member
    * which can be ensured by setting the byte allocated to the Counter Member to 0. */
   AppDataUint8[Config->DE_CNToffs] = 0U;
   RangeCheckResult = Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Step S4: Call the E2E library E2E_PXXProtect() to protect the data element */
   RetProtect = E2E_PDAI1Protect(E2E_PDAI1Config, E2E_PDAI1SenderState, SerData);
   /* Step S5: Write Crc and Counter calculated by the E2E library back to the Data Element */
   AppDataUint8[Config->DE_CRCoffs] = SerData[ProfileDataLength - 1U];
   AppDataUint8[Config->DE_CNToffs] = (uint8)((uint8)(SerData[ProfileDataLength - 2U] >> 4)&0x0FU);
   /* Step S7A: If original and temporary Data Element differ, incorrect input data is provided */
   retVerify = (E2EPW_RANGECHK_VALID == RangeCheckResult) ? (uint32)((uint32)((uint32) E2E_E_OK) << ((uint8)8))
         : (uint32)((uint32)((uint32) E2E_E_INPUTERR_WRONG) << ((uint8)8));
   /* Note: Always return 0 (E2E_E_OK) in Byte 3 */
   return (uint32)(retVerify | (uint32)(RetProtect << 16U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite_PDAI1
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_PDAI1ConfigType, AUTOMATIC, E2EPW_CONST) E2E_PDAI1Config =
         (P2CONST(E2E_PDAI1ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_PDAI1SenderStateType, AUTOMATIC, E2EPW_VAR) E2E_PDAI1SenderState =
         (P2VAR(E2E_PDAI1SenderStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerApplData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 DeCrcOffs = Config->DE_CRCoffs;
   const uint16 DeCntOffs = Config->DE_CNToffs;
   uint32 RetProtect;

   /* Initialize data memory, thus all padding bytes are set to 0 */
   E2EPW_Init_De(SerApplData, Config->DE_Size, 0U);
   /* Copy application Data Element member-wise to temporary Data Element */
   /* Range check result can be ignored here */
   (void)Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Swap the memory of the Crc/Counter struct member with that of the profile-specific position */
   SerApplData[DeCrcOffs] = SerApplData[(Config->DE_Size) - 1U];
   SerApplData[DeCntOffs] = SerApplData[(Config->DE_Size) - 2U];
   /* Crc/Counter Byte can be set to 0 (nothing is read) */
   SerApplData[(Config->DE_Size) - 1U] = 0x00U;
   SerApplData[(Config->DE_Size) - 2U] = 0x00U;
   /* Step S4: Call the E2E library E2E_PXXProtect() to protect the data element */
   RetProtect = E2E_PDAI1Protect(E2E_PDAI1Config, E2E_PDAI1SenderState, SerApplData);
   /* Step S5: Write Crc and Counter calculated by the E2E library back to the Data Element */
   AppDataUint8[DeCrcOffs] = SerApplData[(Config->DE_Size) - 1U];
   AppDataUint8[DeCntOffs] = (uint8)((uint8)(SerApplData[(Config->DE_Size) - 2U] >> 4U) & 0x0FU);
   /* Note: Always return 0 (E2E_E_OK) in Bytes 1 and 3 */
   return (uint32)(RetProtect << 16U);
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite2_PDAI1
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_PDAI1ConfigType, AUTOMATIC, E2EPW_CONST) E2E_PDAI1Config =
         (P2CONST(E2E_PDAI1ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_PDAI1SenderStateType, AUTOMATIC, E2EPW_VAR) E2E_PDAI1SenderState =
         (P2VAR(E2E_PDAI1SenderStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 DeCrcOffs = Config->DE_CRCoffs;
   const uint16 DeCntOffs = Config->DE_CNToffs;
   const uint8 CrcVal = AppDataUint8[DeCrcOffs];
   const uint8 CntVal = AppDataUint8[DeCntOffs];
   const uint16 ProfileDataLength = E2E_PDAI1Config->DataLength / 8U;
   uint32 RetProtect, RetVote;
   uint8 RangeCheckResult;

   /* Initialize data memory with 0xFF,
    * thus all bits in unused bit areas are set to one (E2EUSE173) */
   E2EPW_Init_De(SerData, (uint16)(E2E_PDAI1Config->DataLength / 8U), 0xFFU);
   /* Step S13A: Serialize Data Element into uint8 array */
   /* Perform range checks on Data Element members */
   RangeCheckResult = Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Step S14: Call the E2E library E2E_PXXProtect() to protect the data element */
   RetProtect = E2E_PDAI1Protect(E2E_PDAI1Config, E2E_PDAI1SenderState, SerData);
   /* Step S17: Votes on the equality of the control and data fields */
   RangeCheckResult |= ((CrcVal == SerData[ProfileDataLength - 1U]) &&
       (CntVal == (uint8)((uint8)(SerData[ProfileDataLength - 2U] >> 4) & 0x0FU))) ?
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;
   /* If original and temporary Data Element differ, redundancy check failed */
   if (E2EPW_RANGECHK_VALID == RangeCheckResult)
   {
      RetVote = (uint32)((uint32) E2E_E_OK) << ((uint8)8);
   }
   else
   {
      RetVote = (uint32)((uint32) E2EPW_E_REDUNDANCY) << ((uint8)8);
   }
   /* Note: Always return 0 (E2E_E_OK) in Byte 3 */
   return (uint32)(RetVote | (RetProtect << 16U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite2_PDAI1
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_PDAI1ConfigType, AUTOMATIC, E2EPW_CONST) E2E_PDAI1Config =
         (P2CONST(E2E_PDAI1ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_PDAI1SenderStateType, AUTOMATIC, E2EPW_VAR) E2E_PDAI1SenderState =
         (P2VAR(E2E_PDAI1SenderStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerApplData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 DeCrcOffs = Config->DE_CRCoffs;
   const uint16 DeCntOffs = Config->DE_CNToffs;
   const uint8 CrcVal = AppDataUint8[DeCrcOffs];
   const uint8 CntVal = AppDataUint8[DeCntOffs];
   uint32 RetProtect, RetVote;

   /* Initialize data memory, thus all padding bytes are set to 0 */
   E2EPW_Init_De(SerApplData, Config->DE_Size, 0U);
   /* Copy application Data Element member-wise to temporary Data Element */
   /* Range check result can be ignored here */
   (void)Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Swap the memory of the Crc/Counter struct member with that of the profile-specific position */
   SerApplData[DeCrcOffs] = SerApplData[(Config->DE_Size) - 1U];
   SerApplData[DeCntOffs] = SerApplData[(Config->DE_Size) - 2U];
   /* Crc/Counter Byte can be set to 0 (nothing is read) */
   SerApplData[(Config->DE_Size) - 1U] = 0x00U;
   SerApplData[(Config->DE_Size) - 2U] = 0x00U;
   /* Step S14: Call the E2E library E2E_PXXProtect() to protect the data element */
   RetProtect = E2E_PDAI1Protect(E2E_PDAI1Config, E2E_PDAI1SenderState, SerApplData);
   /* If control Data of original and serialized Data Element differ, redundancy check failed */
   if ((CrcVal != SerApplData[(Config->DE_Size) - 1U]) ||
       (CntVal != (uint8)((uint8)(SerApplData[(Config->DE_Size) - 2U] >> 4) & 0x0FU)))
   {
      RetVote = (uint32)((uint32) E2EPW_E_REDUNDANCY) << ((uint8)8);
   }
   else
   {
      RetVote = (uint32)((uint32) E2E_E_OK) << ((uint8)8);
   }
   /* Note: Always return 0 (E2E_E_OK) in Byte 3 */
   return (uint32)(RetVote | (RetProtect << 16U));
}

#endif /* (E2EPW_ENABLE_PROFILE_PDAI1 == STD_ON) */

#if (E2EPW_ENABLE_PROFILE_PDAI2 == STD_ON)

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerRead_PDAI2
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_PDAI2ConfigType, AUTOMATIC, E2EPW_CONST) E2E_PDAI2Config =
         (P2CONST(E2E_PDAI2ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_PDAI2ReceiverStateType, AUTOMATIC, E2EPW_VAR) E2E_PDAI2ReceiverState =
         (P2VAR(E2E_PDAI2ReceiverStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   uint32 RetCheck, RetBitExt, RetStatus;
   uint8 RangeCheckResult;

   /* Initialize data memory with 0xFF,
    * thus all bits in unused bit areas are set to one (E2EUSE173) */
   E2EPW_Init_De(SerData, (uint16)(E2E_PDAI2Config->DataLength / 8U), 0xFFU);
   /* Step R3A/R13A: Serialize Data Element into uint8 array */
   /* Perform range checks on Data Element members */
   RangeCheckResult = Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Step R4/R14: Call the E2E library E2E_PXXCheck() to detect possible communication errors */
   RetCheck = E2E_PDAI2Check(E2E_PDAI2Config, E2E_PDAI2ReceiverState, SerData);
   /* Map profile specific status values to E2EPW return values */
   RetStatus = (uint32) E2E_PDAI2ReceiverState->Status;
   /* Step R7A/R17A: If original and temporary Data Element differ,
    * a de-serialization error occurred */
   if (E2EPW_RANGECHK_VALID == RangeCheckResult)
   {
      /* Defensive behavior check: Only status values smaller than 256 can be returned. */
      if (RetStatus >= 256UL)
      {
         RetBitExt = (uint32)((uint32) E2E_E_INTERR) << ((uint8)8);
      }
      else
      {
         RetBitExt = (uint32)((uint32) E2E_E_OK) << ((uint8)8);
      }
   }
   else
   {
      RetBitExt = (uint32)((uint32) E2EPW_E_DESERIALIZATION) << ((uint8)8);
   }
   return (uint32)((uint32)(RetBitExt | (RetCheck << 16U)) | (uint32)(RetStatus << 24U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerRead_PDAI2
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_PDAI2ConfigType, AUTOMATIC, E2EPW_CONST) E2E_PDAI2Config =
         (P2CONST(E2E_PDAI2ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_PDAI2ReceiverStateType, AUTOMATIC, E2EPW_VAR) E2E_PDAI2ReceiverState =
         (P2VAR(E2E_PDAI2ReceiverStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerApplData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 DeCrcOffs = Config->DE_CRCoffs;
   const uint16 DeCntOffs = Config->DE_CNToffs;
   const uint8 CrcVal = AppDataUint8[DeCrcOffs];
   const uint8 CntVal = AppDataUint8[DeCntOffs];
   uint32 RetCheck, RetStatus, RetE2EPW;

   /* Initialize data memory, thus all padding bytes are set to 0 */
   E2EPW_Init_De(SerApplData, Config->DE_Size, 0U);
   /* Copy application Data Element member-wise to temporary Data Element */
   /* Range check result can be ignored here */
   (void)Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Swap the memory of the Crc/Counter struct member
    * with that of the profile-specific position */
   SerApplData[DeCrcOffs] = SerApplData[0];
   SerApplData[DeCntOffs] = SerApplData[1];
   SerApplData[0] = CrcVal;
   SerApplData[1] = CntVal;
   /* Step R4/R14: Call the E2E library E2E_PXXCheck() to detect possible communication errors */
   RetCheck = E2E_PDAI2Check(E2E_PDAI2Config, E2E_PDAI2ReceiverState, SerApplData);
   /* Map profile specific status values to E2EPW return values */
   RetStatus = (uint32) E2E_PDAI2ReceiverState->Status;
   /* Defensive behavior check: Only status values smaller than 256 can be returned. */
   if (RetStatus >= 256UL)
   {
      RetE2EPW = (uint32)((uint32) E2E_E_INTERR) << ((uint8)8);
   }
   else
   {
      RetE2EPW = (uint32)((uint32) E2E_E_OK) << ((uint8)8);
   }
   return (uint32)((uint32)(RetE2EPW | (RetCheck << 16U)) | (uint32)(RetStatus << 24U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite_PDAI2
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_PDAI2ConfigType, AUTOMATIC, E2EPW_CONST) E2E_PDAI2Config =
         (P2CONST(E2E_PDAI2ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_PDAI2SenderStateType, AUTOMATIC, E2EPW_VAR) E2E_PDAI2SenderState =
         (P2VAR(E2E_PDAI2SenderStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   uint32 RetProtect, retVerify;
   uint8 RangeCheckResult;

   /* Initialize data memory with 0xFF,
    * thus all bits in unused bit areas are set to one (E2EUSE173) */
   E2EPW_Init_De(SerData, (uint16)(E2E_PDAI2Config->DataLength / 8U), 0xFFU);
   /* Step S3A: Serialize Data Element into uint8 array */
   /* Perform range checks on Data Element members except for Crc and Counter Member
    * which can be ensured by setting the byte allocated to the Counter Member to 0. */
   AppDataUint8[Config->DE_CNToffs] = 0U;
   RangeCheckResult = Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Step S4: Call the E2E library E2E_PXXProtect() to protect the data element */
   RetProtect = E2E_PDAI2Protect(E2E_PDAI2Config, E2E_PDAI2SenderState, SerData);
   /* Step S5: Write Crc and Counter calculated by the E2E library back to the Data Element */
   AppDataUint8[Config->DE_CRCoffs] = SerData[0];
   AppDataUint8[Config->DE_CNToffs] = SerData[1] & 0x0FU;
   /* Step S7A: If original and temporary Data Element differ, incorrect input data is provided */
   retVerify = (E2EPW_RANGECHK_VALID == RangeCheckResult) ? (uint32)((uint32)((uint32) E2E_E_OK) << ((uint8)8))
         : (uint32)((uint32)((uint32) E2E_E_INPUTERR_WRONG) << ((uint8)8));
   /* Note: Always return 0 (E2E_E_OK) in Byte 3 */
   return (uint32)(retVerify | (uint32)(RetProtect << 16U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite_PDAI2
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_PDAI2ConfigType, AUTOMATIC, E2EPW_CONST) E2E_PDAI2Config =
         (P2CONST(E2E_PDAI2ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_PDAI2SenderStateType, AUTOMATIC, E2EPW_VAR) E2E_PDAI2SenderState =
         (P2VAR(E2E_PDAI2SenderStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerApplData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 DeCrcOffs = Config->DE_CRCoffs;
   const uint16 DeCntOffs = Config->DE_CNToffs;
   uint32 RetProtect;

   /* Initialize data memory, thus all padding bytes are set to 0 */
   E2EPW_Init_De(SerApplData, Config->DE_Size, 0U);
   /* Copy application Data Element member-wise to temporary Data Element */
   /* Range check result can be ignored here */
   (void)Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Swap the memory of the Crc/Counter struct member with that of the profile-specific position */
   SerApplData[DeCrcOffs] = SerApplData[0];
   SerApplData[DeCntOffs] = SerApplData[1];
   /* Crc/Counter Byte can be set to 0 (nothing is read) */
   SerApplData[0] = 0x00U;
   SerApplData[1] = 0x00U;
   /* Step S4: Call the E2E library E2E_PXXProtect() to protect the data element */
   RetProtect = E2E_PDAI2Protect(E2E_PDAI2Config, E2E_PDAI2SenderState, SerApplData);
   /* Step S5: Write Crc and Counter calculated by the E2E library back to the Data Element */
   AppDataUint8[DeCrcOffs] = SerApplData[0];
   AppDataUint8[DeCntOffs] = SerApplData[1] & 0x0FU;
   /* Note: Always return 0 (E2E_E_OK) in Bytes 1 and 3 */
   return (uint32)(RetProtect << 16U);
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite2_PDAI2
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_PDAI2ConfigType, AUTOMATIC, E2EPW_CONST) E2E_PDAI2Config =
         (P2CONST(E2E_PDAI2ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_PDAI2SenderStateType, AUTOMATIC, E2EPW_VAR) E2E_PDAI2SenderState =
         (P2VAR(E2E_PDAI2SenderStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 DeCrcOffs = Config->DE_CRCoffs;
   const uint16 DeCntOffs = Config->DE_CNToffs;
   const uint8 CrcVal = AppDataUint8[DeCrcOffs];
   const uint8 CntVal = AppDataUint8[DeCntOffs];
   uint32 RetProtect, RetVote;
   uint8 RangeCheckResult;

   /* Initialize data memory with 0xFF,
    * thus all bits in unused bit areas are set to one (E2EUSE173) */
   E2EPW_Init_De(SerData, (uint16)(E2E_PDAI2Config->DataLength / 8U), 0xFFU);
   /* Step S13A: Serialize Data Element into uint8 array */
   /* Perform range checks on Data Element members */
   RangeCheckResult = Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Step S14: Call the E2E library E2E_PXXProtect() to protect the data element */
   RetProtect = E2E_PDAI2Protect(E2E_PDAI2Config, E2E_PDAI2SenderState, SerData);
   /* Step S17: Votes on the equality of the control and data fields */
   RangeCheckResult |= ((CrcVal == SerData[0]) &&
       (CntVal == (uint8)(SerData[1] & 0x0FU))) ?
       E2EPW_RANGECHK_VALID : E2EPW_RANGECHK_INVALID;
   /* If original and temporary Data Element differ, redundancy check failed */
   if (E2EPW_RANGECHK_VALID == RangeCheckResult)
   {
      RetVote = (uint32)((uint32) E2E_E_OK) << ((uint8)8);
   }
   else
   {
      RetVote = (uint32)((uint32) E2EPW_E_REDUNDANCY) << ((uint8)8);
   }
   /* Note: Always return 0 (E2E_E_OK) in Byte 3 */
   return (uint32)(RetVote | (RetProtect << 16U));
}

FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite2_PDAI2
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
)
{
   CONSTP2CONST(E2E_PDAI2ConfigType, AUTOMATIC, E2EPW_CONST) E2E_PDAI2Config =
         (P2CONST(E2E_PDAI2ConfigType, AUTOMATIC, E2EPW_CONST)) Config->E2E_PXXConfig;
   CONSTP2VAR(E2E_PDAI2SenderStateType, AUTOMATIC, E2EPW_VAR) E2E_PDAI2SenderState =
         (P2VAR(E2E_PDAI2SenderStateType, AUTOMATIC, E2EPW_VAR)) Config->E2E_PXXState;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerApplData =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) SerDataDEData;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_VAR) AppDataUint8 =
       (P2VAR(uint8, AUTOMATIC, E2EPW_VAR)) AppData;
   const uint16 DeCrcOffs = Config->DE_CRCoffs;
   const uint16 DeCntOffs = Config->DE_CNToffs;
   const uint8 CrcVal = AppDataUint8[DeCrcOffs];
   const uint8 CntVal = AppDataUint8[DeCntOffs];
   uint32 RetProtect, RetVote;

   /* Initialize data memory, thus all padding bytes are set to 0 */
   E2EPW_Init_De(SerApplData, Config->DE_Size, 0U);
   /* Copy application Data Element member-wise to temporary Data Element */
   /* Range check result can be ignored here */
   (void)Config->E2EPW_PackCopyDeFunc(AppData, SerDataDEData);
   /* Swap the memory of the Crc/Counter struct member with that of the profile-specific position */
   SerApplData[DeCrcOffs] = SerApplData[0];
   SerApplData[DeCntOffs] = SerApplData[1];
   /* Crc/Counter Byte can be set to 0 (nothing is read) */
   SerApplData[0] = 0x00U;
   SerApplData[1] = 0x00U;
   /* Step S14: Call the E2E library E2E_PXXProtect() to protect the data element */
   RetProtect = E2E_PDAI2Protect(E2E_PDAI2Config, E2E_PDAI2SenderState, SerApplData);
   /* If control Data of original and serialized Data Element differ, redundancy check failed */
   if ((CrcVal != SerApplData[0]) || (CntVal != (uint8)(SerApplData[1] & 0x0FU)))
   {
      RetVote = (uint32)((uint32) E2EPW_E_REDUNDANCY) << ((uint8)8);
   }
   else
   {
      RetVote = (uint32)((uint32) E2E_E_OK) << ((uint8)8);
   }
   /* Note: Always return 0 (E2E_E_OK) in Byte 3 */
   return (uint32)(RetVote | (RetProtect << 16U));
}

#endif /* (E2EPW_ENABLE_PROFILE_PDAI2 == STD_ON) */

/*==[Definition of functions with internal linkage]===========================*/

static FUNC(void, E2EPW_CODE) E2EPW_Init_De
(
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) DePtr,
   uint16 NBytes,
   uint8 Pattern
)
{
   /* define pointer for access */
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_APPL_DATA) DePtr2 =
         (P2VAR(uint8, AUTOMATIC, E2EPW_APPL_DATA)) DePtr;
   uint16_least i;

   for (i = 0U; i < NBytes; ++i)
   {
      DePtr2[i] = Pattern;
   }
}

#define E2EPW_STOP_SEC_CODE
#include <MemMap.h>

#else

/** \brief Dummy type definition in case E2EPW.c contains no source code */
typedef void E2EPWPreventEmptyTranslationUnitType;

#endif /* ((E2EPW_ENABLE_PROFILE_P01 == STD_ON) || \
      (E2EPW_ENABLE_PROFILE_P02 == STD_ON) ||  \
      (E2EPW_ENABLE_PROFILE_PDAI1 == STD_ON) ||  \
      (E2EPW_ENABLE_PROFILE_PDAI2 == STD_ON)) */

/* Deviation MISRAC2012-1 <STOP> */

/*==================[end of file]============================================*/
