#ifndef SOMEIPXF_S_INT_H
#define SOMEIPXF_S_INT_H

/**
 * \file
 *
 * \brief AUTOSAR SomeIpXf
 *
 * This file contains the implementation of the AUTOSAR
 * module SomeIpXf.
 *
 * \version 1.0.47
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/
#include <SomeIpXf_S_Int_Stc.h>        /* Internal static declarations (Safe version) */
#include <SomeIpXf_Cfg.h>              /* Generated configuration */
#include <SomeIpXf_S_Api_Stc.h>

/*==================[macros]================================================*/

/*==================[Error codes]===========================================*/

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
typedef struct
{
  /* first LF byte value of byte stream */
  VAR(uint32, TYPEDEF) VSA_LF_Value;
  /* LF buffer position in byte stream */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) VSA_LF_BufPos;
  /* size indicator offsetof value */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) VSA_SI_offsetof;
  /* VSA element sizeof value */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) VSA_element_sizeof;
  /* number of valid elements set by the size indicator */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) VSA_elements;
  /* number of valid element which is currently handled */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) VSA_curr_element;
  /* max number of valid elements determine by static VSA array size */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) VSA_max_element;
  /* relative VSA element offsetof value in case of array of VSA */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) VSA_ArrayDataByteOffset;
} VSA_ContextType;

typedef struct
{
   VAR(VSA_ContextType, TYPEDEF) VSA_Context_DIM_LEVEL[SOMEIPXF_VSA_MAX_DIM];
   /* LF size within byte stream */
   VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) VSA_LFsize;
   /* size indicator sizeof value */
   VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) VSA_SI_sizeof;
   /* config position for handling/reentering remaining element */
   VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) VSA_ConfigPosRemElement;
   /* config position for handling zero elements */
   VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) VSA_ConfigPosZeroElement;
   /* config position for a VSA stop enter */
   VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) VSA_ConfigPosVSAStop;
   /* config position of related INTEGER_CONFIG_NW */
   VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) VSA_ConfigPosNWEndianness;
   VAR(uint8, TYPEDEF) VSA_current_DIM_LEVEL;
   VAR(uint8, TYPEDEF) VSA_max_config_DIM;
   /* indicates a size indicator set to zero */
   VAR(boolean, TYPEDEF) VSA_IsOfSize0;
} VSA_InfoType;

typedef struct
{
   VAR(VSA_InfoType, TYPEDEF) VSA_Info_LEVEL[SOMEIPXF_MAX_NESTED_VSA_DEPTH];
   VAR(uint8, TYPEDEF) VSA_current_NESTED_LEVEL;
   VAR(boolean, TYPEDEF) VSA_Root;
   VAR(boolean, TYPEDEF) VSA_De_Seral;
} VSA_NESTED_LEVEL_Type;
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */

#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)
typedef struct
{
  /* first LF byte value of byte stream */
  VAR(uint32, TYPEDEF) LF_Value;
  /* LF size within byte stream */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) LFsize;
  /* LF buffer position in byte stream */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) LF_BufPos;
  /* config position of related INTEGER_CONFIG_NW */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) ConfigPosNWEndianness;
} StructWithLf_InfoType;

typedef struct
{
   VAR(StructWithLf_InfoType, TYPEDEF) StructWithLf_Info_LEVEL[SOMEIPXF_MAX_NESTED_STRUCT_WITH_LF_DEPTH];
   VAR(uint8, TYPEDEF)   StructWithLf_current_NESTED_LEVEL;
   VAR(boolean, TYPEDEF) StructWithLf_Root;
   VAR(boolean, TYPEDEF) StructWithLf_De_Seral;
} StructWithLf_NESTED_LEVEL_Type;
#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */

#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
typedef struct
{
#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)
  /* first LF byte value of byte stream */
  VAR(uint32, TYPEDEF) FSA_LF_Value;
  /* LF buffer position in byte stream */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) FSA_LF_BufPos;
#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */
  /* number of element which is currently handled */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) FSA_curr_element;
  /* max number of elements determine by static array size */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) FSA_max_element;
  /* FSA element sizeof value */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) FSA_element_sizeof;
  /* relative FSA element offsetof value in case of array of FSA */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) FSA_ArrayDataByteOffset;
} FSA_DimType;

typedef struct
{
  VAR(FSA_DimType, TYPEDEF) FSA_DIM_LEVEL[SOMEIPXF_FSA_MAX_DIM];
  /* config position for handling/reentering remaining element */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) FSA_ConfigPosRemElement;
#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)
  /* LF size within byte stream */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) FSA_LFsize;
  /* config position of related INTEGER_CONFIG_NW */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) FSA_ConfigPosNWEndianness;
#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */
  VAR(uint8, TYPEDEF) FSA_current_DIM_LEVEL;
  VAR(uint8, TYPEDEF) FSA_max_config_DIM;
  /* indicates a extensible FSA on current nested level */
  VAR(boolean, TYPEDEF) FSAWithLf;
} FSA_InfoType;

typedef struct
{
   VAR(FSA_InfoType, TYPEDEF) FSA_Info_LEVEL[SOMEIPXF_MAX_NESTED_ARRAY_DEPTH];
   VAR(uint8, TYPEDEF)   FSA_current_NESTED_LEVEL;
   VAR(boolean, TYPEDEF) FSA_Root;
   VAR(boolean, TYPEDEF) FSA_De_Seral;
} FSA_NESTED_LEVEL_Type;
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */

#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)
typedef struct
{
  /* Length field size for current nested level */
  VAR(uint32, TYPEDEF) TLVStruct_LFsize;
  /* Total number of TLV element in current nested level */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) TLVDataIDCount;
  /* SomeIpXf cfg pos of DataID list start for the current nested level */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) TLVDataIDStartCfgHandleId;
  /* Total number of fixed element in current nested level */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) FixMemCount;
  /* Position of the Endianness in current nested level */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) EndinessPosition;
  /* Position of the LF in current nested level */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) PositionofLF;
  /* Set static LF byte size */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) TLV_static_lenghtfield_bytesize;
  /* MembIdx of given level based in current nested level */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) MembIdx;
  /* TLV stop position  */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) PositionofStopTlvTag;
#if(SOMEIPXF_TLV_STRUCT_MEMBER_UNIQUE_DATAID_CHECK_ENABLE == STD_ON)
  /* Number of Data ID's processed */
  VAR(uint8, TYPEDEF) DataID_Processed[SOMEIPXF_MAX_TLV_IDS_BYTES];
#endif
  /* Total number of DataId processed */
  VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) TLVDataIDProcessedCount;
  /* Is this extensible element */
  VAR(boolean, TYPEDEF) ExtElement;
}TLV_InfoType;

typedef struct
{
  VAR(TLV_InfoType, TYPEDEF) TLV_Info_LEVEL[SOMEIPXF_MAX_NESTED_TLV_DEPTH];
  VAR(uint8, TYPEDEF) TLV_Current_NestedLevel;
  VAR(boolean, TYPEDEF) TLV_Root;
  VAR(boolean, TYPEDEF) TLV_Struct_Deserial_FirstElement;
}TLV_Nested_Type;
#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */

typedef struct
{
   VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE,  TYPEDEF) BytesCopied;
   VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE,  TYPEDEF) CfgElemHandled;
   VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE,  TYPEDEF) DataByteOffset;
#if( (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) || (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) )
   VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, TYPEDEF) TLV_LFSize_WireType567;
#endif /* ( (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) || (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) ) */
} SomeIpXf_DeSerInfoType;

typedef struct
{
#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
   P2VAR(VSA_NESTED_LEVEL_Type, TYPEDEF, SOMEIPXF_APPL_DATA) VSAContextPtr;
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)
   P2VAR(TLV_Nested_Type, TYPEDEF, SOMEIPXF_APPL_DATA) TLVContextPtr;
#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */
#if (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON)
   VAR(uint32,  TYPEDEF) TLV_ARG_BufPos;
#endif /* #if (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) */
#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
   P2VAR(FSA_NESTED_LEVEL_Type, TYPEDEF, SOMEIPXF_APPL_DATA) FSAContextPtr;
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)
   P2VAR(StructWithLf_NESTED_LEVEL_Type, TYPEDEF, SOMEIPXF_APPL_DATA) StructWithLfContextPtr;
#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */
   P2VAR(SomeIpXf_DeSerInfoType, TYPEDEF, SOMEIPXF_APPL_DATA) DeSerContextPtr;
   VAR(boolean, TYPEDEF) ElemFound;
} SomeIpXf_AdminDataType;

/*==================[external function declarations]========================*/
#define SOMEIPXF_START_SEC_CODE
#include <SomeIpXf_MemMap.h>

extern FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_S_Int_GenericTransformer
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(void, AUTOMATIC, SOMEIPXF_APPL_DATA) DataPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_AdminDataType, AUTOMATIC, SOMEIPXF_APPL_DATA) AdminData
);

extern FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_S_Int_Inv_GenericTransformer
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA) DataPtr,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BytesCopiedPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_AdminDataType, AUTOMATIC, SOMEIPXF_APPL_DATA) AdminData
);

extern FUNC(void, SOMEIPXF_CODE) SomeIpXf_S_Init_AdminData
(
   P2VAR(SomeIpXf_AdminDataType, AUTOMATIC, SOMEIPXF_APPL_DATA) AdminData,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BytesCopiedPtr,
   boolean TLV_Arg_De_Seral
);

extern FUNC(void, SOMEIPXF_CODE) SomeIpXf_S_Init_DeSerContext
(
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)
extern FUNC(void, SOMEIPXF_CODE) SomeIpXf_S_Init_StructWithLfContext
(
   P2VAR(StructWithLf_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) StructWithLfContextPtr,
   boolean StructWithLf_De_Seral
);
#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */

#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
FUNC(void, SOMEIPXF_CODE) SomeIpXf_S_Init_FSAContext
(
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   boolean FSA_De_Seral
);
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */

#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
FUNC(void, SOMEIPXF_CODE) SomeIpXf_S_Init_VSAContext
(
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContext,
   boolean VSA_De_Seral
);
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */

#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)
FUNC(void, SOMEIPXF_CODE) SomeIpXf_Init_TLVContext
(
    P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr
);
#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */

/* disable functionality since Rte_Cs_TransactionHandleType might be undefined just for
   SR use (Rte_Type.h might miss the definition) */
#if (SOMEIPXF_CS_AVAILABLE == STD_ON)
extern FUNC(void, SOMEIPXF_CODE) SomeIpXf_Request_SetHeader
(
   P2CONST(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle,
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   uint8 InterfaceVersion
);

extern FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Request_GetHeader
(
   P2VAR(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle,
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   uint8 InterfaceVersion
);

extern FUNC(void, SOMEIPXF_CODE) SomeIpXf_Response_SetHeader
(
   P2CONST(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle,
   uint8 InterfaceVersion,
   Std_ReturnType ReturnValue,
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr
);

extern FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Response_GetHeader
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   uint8 InterfaceVersion,
   P2VAR(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle,
   P2VAR(Std_ReturnType, AUTOMATIC, SOMEIPXF_APPL_DATA) ReturnValue
);
#endif /* (SOMEIPXF_CS_AVAILABLE == STD_ON) */

#if ((SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) && (SOMEIPXF_TLV_ARGUMENT_UNIQUE_DATAID_CHECK_ENABLE == STD_ON))
extern FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_CheckMultipleKnownDataIDs
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLVArgIDListSize,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) TLVArgID,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr
);
#endif /* ((SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) && (SOMEIPXF_TLV_ARGUMENT_UNIQUE_DATAID_CHECK_ENABLE == STD_ON)) */

#define SOMEIPXF_STOP_SEC_CODE
#include <SomeIpXf_MemMap.h>
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef SOMEIPXF_S_INT_H */
/*==================[end of file]===========================================*/
