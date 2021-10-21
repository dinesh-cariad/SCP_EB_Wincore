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

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * ## operator is used for code generation via pre-processor.
 *
 * MISRAC2012-2) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to
 * void into pointer to object.
 *
 * Reason:
 * - The memory routines are optimized for dealing with aligned
 *   memory sections.
 * - The SomeIpXf module use generic interfaces to (de-)serialize
 *   complex data of different types.
 *
 * MISRAC2012-3) Deviated Rule: 2.2 (required)
 * There shall be no dead code.
 *
 * Reason:
 * The macro E2E_PARAM_UNUSED causes a False Positive Misra warning.
 * The macro is needed to avoid a compiler warning and can't
 * be written differently.
 * It expands to a void cast which is an exception of the rule 2.2
 * according to the MISRA-C:2012 Guidelines March 2013.
 *
 * MISRAC2012-4) Deviated Rule: 20.9 (required)
 * All identifiers used in the controlling expression of #if or #elif
 * preprocessing directives shall be #define'd before evaluation.
 *
 * Reason:
 * Conditional configuration macro checks operate with concatenated
 * identifiers via preprocessor operator ##. The concatenated macros
 * are defined and verified for uniqueness. However, the macro tokens
 * _END which are merged during the expansion are not defined since those
 * tokens are only used for concatenation. Nonetheless, a violation of Rule: 20.9
 * gets reported according to the MISRA-C:2012 Guidelines March 2013.
 *
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason:
 * False-positive from the Tasking v5.0r2 compiler (TCVX-41885)
 */

/* Code-Metric Deviation List
 *
 * CODEMETRIC-1) Deviated Rule: HIS_CALLS
 * not 17 <= 12
 *
 * Reason:
 * depends on which data types are configured
 *
 * CODEMETRIC-2) Deviated Rule: HIS_VOCF
 * not 12.1 <= 12
 *
 * Reason:
 * SOMEIPXF_READ macros depend on CPU_BYTE_ORDER
*/

/*==================[inclusions]============================================*/

#include <SomeIpXf_S_Int.h>          /* internal functions and types */

#include <TransformerTypes.h>

/*==================[macros]================================================*/

#if (defined SOMIPXF_OUTER_MOST_DIM) /* to prevent double declaration */
#error SOMIPXF_OUTER_MOST_DIM already defined
#endif /* if (defined SOMIPXF_OUTER_MOST_DIM) */

#define SOMIPXF_OUTER_MOST_DIM 0U

#if (defined SOMIPXF_INTERMEDIATE_DIM) /* to prevent double declaration */
#error SOMIPXF_INTERMEDIATE_DIM already defined
#endif /* if (defined SOMIPXF_INTERMEDIATE_DIM) */

#define SOMIPXF_INTERMEDIATE_DIM 1U

#if (defined SOMIPXF_INNER_MOST_DIM) /* to prevent double declaration */
#error SOMIPXF_INNER_MOST_DIM already defined
#endif /* if (defined SOMIPXF_INNER_MOST_DIM) */

#define SOMIPXF_INNER_MOST_DIM 2U

#if (defined SOMEIPXF_SET_StructWithLfContext_StructWithLf_InfoType) /* to prevent double declaration */
#error SOMEIPXF_SET_StructWithLfContext_StructWithLf_InfoType already defined
#endif /* if (defined SOMEIPXF_SET_StructWithLfContext_StructWithLf_InfoType) */

#define SOMEIPXF_SET_StructWithLfContext_StructWithLf_InfoType(memb,val) \
  StructWithLfContextPtr->StructWithLf_Info_LEVEL[StructWithLfContextPtr->StructWithLf_current_NESTED_LEVEL].memb = (val)

#if (defined SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType) /* to prevent double declaration */
#error SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType already defined
#endif /* if (defined SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType) */

#define SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(memb) \
  StructWithLfContextPtr->StructWithLf_Info_LEVEL[StructWithLfContextPtr->StructWithLf_current_NESTED_LEVEL].memb

#if (defined SOMEIPXF_CLEAR_StructWithLfContext_StructWithLf_InfoType) /* to prevent double declaration */
#error SOMEIPXF_CLEAR_StructWithLfContext_StructWithLf_InfoType already defined
#endif /* if (defined SOMEIPXF_CLEAR_StructWithLfContext_StructWithLf_InfoType) */

#define SOMEIPXF_CLEAR_StructWithLfContext_StructWithLf_InfoType(nested_level) \
  do{ \
    StructWithLfContextPtr->StructWithLf_Info_LEVEL[(nested_level)].LFsize = 0U; \
    StructWithLfContextPtr->StructWithLf_Info_LEVEL[(nested_level)].LF_Value = 0U; \
    StructWithLfContextPtr->StructWithLf_Info_LEVEL[(nested_level)].LF_BufPos = 0U; \
    StructWithLfContextPtr->StructWithLf_Info_LEVEL[(nested_level)].ConfigPosNWEndianness = 0U; \
    } while(0)

#if (defined SOMEIPXF_GET_ADDR_StructWithLf_InfoType) /* to prevent double declaration */
#error SOMEIPXF_GET_ADDR_StructWithLf_InfoType already defined
#endif /* if (defined SOMEIPXF_GET_ADDR_StructWithLf_InfoType) */

#define SOMEIPXF_GET_ADDR_StructWithLf_InfoType(memb) \
  &StructWithLfContextPtr->StructWithLf_Info_LEVEL[StructWithLfContextPtr->StructWithLf_current_NESTED_LEVEL].memb

#if (defined SOMEIPXF_SET_FSAContext_FSA_InfoType) /* to prevent double declaration */
#error SOMEIPXF_SET_FSAContext_FSA_InfoType already defined
#endif /* if (defined SOMEIPXF_SET_FSAContext_FSA_InfoType) */

#define SOMEIPXF_SET_FSAContext_FSA_InfoType(memb,val) \
  FSAContextPtr->FSA_Info_LEVEL[FSAContextPtr->FSA_current_NESTED_LEVEL].memb = (val)

#if (defined SOMEIPXF_GET_FSAContext_FSA_InfoType) /* to prevent double declaration */
#error SOMEIPXF_GET_FSAContext_FSA_InfoType already defined
#endif /* if (defined SOMEIPXF_GET_FSAContext_FSA_InfoType) */

#define SOMEIPXF_GET_FSAContext_FSA_InfoType(memb) \
  FSAContextPtr->FSA_Info_LEVEL[FSAContextPtr->FSA_current_NESTED_LEVEL].memb

#if (defined SOMEIPXF_GET_ADDR_FSAContext_FSA_InfoType) /* to prevent double declaration */
#error SOMEIPXF_GET_ADDR_FSAContext_FSA_InfoType already defined
#endif /* if (defined SOMEIPXF_GET_ADDR_FSAContext_FSA_InfoType) */

#define SOMEIPXF_GET_ADDR_FSAContext_FSA_InfoType(memb) \
  &FSAContextPtr->FSA_Info_LEVEL[FSAContextPtr->FSA_current_NESTED_LEVEL].memb

#if (defined SOMEIPXF_CLEAR_FSAContext_FSA_InfoType) /* to prevent double declaration */
#error SOMEIPXF_CLEAR_FSAContext_FSA_InfoType already defined
#endif /* if (defined SOMEIPXF_CLEAR_FSAContext_FSA_InfoType) */

#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)

#define SOMEIPXF_CLEAR_FSAContext_FSA_InfoType(nested_level) \
  do{ \
    uint32 dim_level_idx = 0U; \
    for(dim_level_idx=0U;dim_level_idx<SOMEIPXF_FSA_MAX_DIM;dim_level_idx++) \
    { \
        FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_DIM_LEVEL[dim_level_idx].FSA_LF_Value = 0U; \
        FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_DIM_LEVEL[dim_level_idx].FSA_LF_BufPos = 0U; \
        FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_DIM_LEVEL[dim_level_idx].FSA_curr_element = 0U; \
        FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_DIM_LEVEL[dim_level_idx].FSA_max_element = 0U; \
        FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_DIM_LEVEL[dim_level_idx].FSA_element_sizeof = 0U; \
        FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_DIM_LEVEL[dim_level_idx].FSA_ArrayDataByteOffset = 0U; \
    } \
    FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_ConfigPosRemElement = 0U; \
    FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_LFsize = 0U; \
    FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_ConfigPosNWEndianness = 0U; \
    FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSAWithLf = FALSE; \
    FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_current_DIM_LEVEL = 0U; \
    FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_max_config_DIM = 0U; \
    } while(0)

#else

#define SOMEIPXF_CLEAR_FSAContext_FSA_InfoType(nested_level) \
  do{ \
    uint32 dim_level_idx = 0U; \
    for(dim_level_idx=0U;dim_level_idx<SOMEIPXF_FSA_MAX_DIM;dim_level_idx++) \
    { \
        FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_DIM_LEVEL[dim_level_idx].FSA_curr_element = 0U; \
        FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_DIM_LEVEL[dim_level_idx].FSA_max_element = 0U; \
        FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_DIM_LEVEL[dim_level_idx].FSA_element_sizeof = 0U; \
        FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_DIM_LEVEL[dim_level_idx].FSA_ArrayDataByteOffset = 0U; \
    } \
    FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_ConfigPosRemElement = 0U; \
    FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSAWithLf = FALSE; \
    FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_current_DIM_LEVEL = 0U; \
    FSAContextPtr->FSA_Info_LEVEL[(nested_level)].FSA_max_config_DIM = 0U; \
    } while(0)

#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */

#if (defined SOMEIPXF_SET_FSAContext_DIM_LEVEL) /* to prevent double declaration */
#error SOMEIPXF_SET_FSAContext_DIM_LEVEL already defined
#endif /* if (defined SOMEIPXF_SET_FSAContext_DIM_LEVEL) */

#define SOMEIPXF_SET_FSAContext_DIM_LEVEL(memb,val) \
  FSAContextPtr->FSA_Info_LEVEL[FSAContextPtr->FSA_current_NESTED_LEVEL].\
  FSA_DIM_LEVEL[FSAContextPtr->FSA_Info_LEVEL[FSAContextPtr->FSA_current_NESTED_LEVEL].FSA_current_DIM_LEVEL].\
  memb = (val)

#if (defined SOMEIPXF_GET_FSAContext_DIM_LEVEL) /* to prevent double declaration */
#error SOMEIPXF_GET_FSAContext_DIM_LEVEL already defined
#endif /* if (defined SOMEIPXF_GET_FSAContext_DIM_LEVEL) */

#define SOMEIPXF_GET_FSAContext_DIM_LEVEL(memb) \
  FSAContextPtr->FSA_Info_LEVEL[FSAContextPtr->FSA_current_NESTED_LEVEL].\
  FSA_DIM_LEVEL[FSAContextPtr->FSA_Info_LEVEL[FSAContextPtr->FSA_current_NESTED_LEVEL].FSA_current_DIM_LEVEL].memb

#if (defined SOMEIPXF_GET_ADDR_FSAContext_DIM_LEVEL) /* to prevent double declaration */
#error SOMEIPXF_GET_ADDR_FSAContext_DIM_LEVEL already defined
#endif /* if (defined SOMEIPXF_GET_ADDR_FSAContext_DIM_LEVEL) */

#define SOMEIPXF_GET_ADDR_FSAContext_DIM_LEVEL(memb) \
  &FSAContextPtr->FSA_Info_LEVEL[FSAContextPtr->FSA_current_NESTED_LEVEL].\
  FSA_DIM_LEVEL[FSAContextPtr->FSA_Info_LEVEL[FSAContextPtr->FSA_current_NESTED_LEVEL].FSA_current_DIM_LEVEL].memb

#if (defined SOMEIPXF_SET_VSAContext_VSA_Info_LEVEL) /* to prevent double declaration */
#error SOMEIPXF_SET_VSAContext_VSA_Info_LEVEL already defined
#endif /* if (defined SOMEIPXF_SET_VSAContext_VSA_Info_LEVEL) */

#define SOMEIPXF_SET_VSAContext_VSA_Info_LEVEL(memb,val) \
  VSAContextPtr->VSA_Info_LEVEL[VSAContextPtr->VSA_current_NESTED_LEVEL].memb = (val)

#if (defined SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL) /* to prevent double declaration */
#error SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL already defined
#endif /* if (defined SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL) */

#define SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(memb) \
  VSAContextPtr->VSA_Info_LEVEL[VSAContextPtr->VSA_current_NESTED_LEVEL].memb

#if (defined SOMEIPXF_GET_ADDR_VSAContext_VSA_Info_LEVEL) /* to prevent double declaration */
#error SOMEIPXF_GET_ADDR_VSAContext_VSA_Info_LEVEL already defined
#endif /* if (defined SOMEIPXF_GET_ADDR_VSAContext_VSA_Info_LEVEL) */

#define SOMEIPXF_GET_ADDR_VSAContext_VSA_Info_LEVEL(memb) \
  &VSAContextPtr->VSA_Info_LEVEL[VSAContextPtr->VSA_current_NESTED_LEVEL].memb

#if (defined SOMEIPXF_SET_VSAContext_DIM_LEVEL) /* to prevent double declaration */
#error SOMEIPXF_SET_VSAContext_DIM_LEVEL already defined
#endif /* if (defined SOMEIPXF_SET_VSAContext_DIM_LEVEL) */

#define SOMEIPXF_SET_VSAContext_DIM_LEVEL(memb,val) \
  VSAContextPtr->VSA_Info_LEVEL[VSAContextPtr->VSA_current_NESTED_LEVEL].\
  VSA_Context_DIM_LEVEL[VSAContextPtr->VSA_Info_LEVEL[VSAContextPtr->VSA_current_NESTED_LEVEL].VSA_current_DIM_LEVEL].\
  memb = (val)

#if (defined SOMEIPXF_GET_VSAContext_DIM_LEVEL) /* to prevent double declaration */
#error SOMEIPXF_GET_VSAContext_DIM_LEVEL already defined
#endif /* if (defined SOMEIPXF_GET_VSAContext_DIM_LEVEL) */

#define SOMEIPXF_GET_VSAContext_DIM_LEVEL(memb) \
  VSAContextPtr->VSA_Info_LEVEL[VSAContextPtr->VSA_current_NESTED_LEVEL].\
  VSA_Context_DIM_LEVEL[VSAContextPtr->VSA_Info_LEVEL[VSAContextPtr->VSA_current_NESTED_LEVEL].VSA_current_DIM_LEVEL].memb

#if (defined SOMEIPXF_GET_ADDR_VSAContext_DIM_LEVEL) /* to prevent double declaration */
#error SOMEIPXF_GET_ADDR_VSAContext_DIM_LEVEL already defined
#endif /* if (defined SOMEIPXF_GET_ADDR_VSAContext_DIM_LEVEL) */

#define SOMEIPXF_GET_ADDR_VSAContext_DIM_LEVEL(memb) \
  &VSAContextPtr->VSA_Info_LEVEL[VSAContextPtr->VSA_current_NESTED_LEVEL].\
  VSA_Context_DIM_LEVEL[VSAContextPtr->VSA_Info_LEVEL[VSAContextPtr->VSA_current_NESTED_LEVEL].VSA_current_DIM_LEVEL].memb

#if (defined SOMEIPXF_CLEAR_VSAContext_VSA_Info_LEVEL) /* to prevent double declaration */
#error SOMEIPXF_CLEAR_VSAContext_VSA_Info_LEVEL already defined
#endif /* if (defined SOMEIPXF_CLEAR_VSAContext_VSA_Info_LEVEL) */

#define SOMEIPXF_CLEAR_VSAContext_VSA_Info_LEVEL(nested_level) \
  do{ \
    uint32 dim_level_idx = 0U; \
    for(dim_level_idx=0U;dim_level_idx<SOMEIPXF_VSA_MAX_DIM;dim_level_idx++) \
    { \
      VSAContext->VSA_Info_LEVEL[nested_level].VSA_Context_DIM_LEVEL[dim_level_idx].VSA_LF_Value = 0U; \
      VSAContext->VSA_Info_LEVEL[nested_level].VSA_Context_DIM_LEVEL[dim_level_idx].VSA_LF_BufPos = 0U; \
      VSAContext->VSA_Info_LEVEL[nested_level].VSA_Context_DIM_LEVEL[dim_level_idx].VSA_SI_offsetof = 0U; \
      VSAContext->VSA_Info_LEVEL[nested_level].VSA_Context_DIM_LEVEL[dim_level_idx].VSA_element_sizeof = 0U; \
      VSAContext->VSA_Info_LEVEL[nested_level].VSA_Context_DIM_LEVEL[dim_level_idx].VSA_elements = 0U; \
      VSAContext->VSA_Info_LEVEL[nested_level].VSA_Context_DIM_LEVEL[dim_level_idx].VSA_curr_element = 0U; \
      VSAContext->VSA_Info_LEVEL[nested_level].VSA_Context_DIM_LEVEL[dim_level_idx].VSA_max_element = 0U; \
      VSAContext->VSA_Info_LEVEL[nested_level].VSA_Context_DIM_LEVEL[dim_level_idx].VSA_ArrayDataByteOffset = 0U; \
    } \
    VSAContext->VSA_Info_LEVEL[nested_level].VSA_LFsize = 0U; \
    VSAContext->VSA_Info_LEVEL[nested_level].VSA_SI_sizeof = 0U; \
    VSAContext->VSA_Info_LEVEL[nested_level].VSA_ConfigPosRemElement = 0U; \
    VSAContext->VSA_Info_LEVEL[nested_level].VSA_ConfigPosZeroElement = 0U; \
    VSAContext->VSA_Info_LEVEL[nested_level].VSA_ConfigPosVSAStop = 0U; \
    VSAContext->VSA_Info_LEVEL[nested_level].VSA_ConfigPosNWEndianness = 0U; \
    VSAContext->VSA_Info_LEVEL[nested_level].VSA_IsOfSize0 = FALSE; \
    VSAContext->VSA_Info_LEVEL[nested_level].VSA_current_DIM_LEVEL = 0U; \
    VSAContext->VSA_Info_LEVEL[nested_level].VSA_max_config_DIM = 0U; \
    } while(0)

#if (defined SOMEIPXF_SET_CONFIGPOS) /* to prevent double declaration */
#error SOMEIPXF_SET_CONFIGPOS already defined
#endif /* if (defined SOMEIPXF_SET_CONFIGPOS) */

#define SOMEIPXF_SET_CONFIGPOS(RetVal, CfgElemHandled, ConfigTAG, ConfigPos)  \
do { \
  if(ConfigTAG == ConfigPtr[ConfigPos]) \
  { \
    CfgElemHandled = ConfigPos; \
  } \
  else \
  { \
    RetVal = E_SER_GENERIC_ERROR; \
  } \
} while(0)

#if (defined SOMEIPXF_VSA_INFO_LEVEL_STATIC_CONFIG_OFFSET) /* To prevent double declaration */
#error SOMEIPXF_VSA_INFO_LEVEL_STATIC_CONFIG_OFFSET already defined
#endif /* (defined SOMEIPXF_VSA_INFO_LEVEL_STATIC_CONFIG_OFFSET) */

#define SOMEIPXF_VSA_INFO_LEVEL_STATIC_CONFIG_OFFSET 4U

#if (defined SOMEIPXF_FSA_INFO_LEVEL_STATIC_CONFIG_OFFSET) /* To prevent double declaration */
#error SOMEIPXF_FSA_INFO_LEVEL_STATIC_CONFIG_OFFSET already defined
#endif /* (defined SOMEIPXF_FSA_INFO_LEVEL_STATIC_CONFIG_OFFSET) */

#define SOMEIPXF_FSA_INFO_LEVEL_STATIC_CONFIG_OFFSET 3U

#if (defined SOMEIPXF_ARG_TLV_CONTEXT_TAG_SIZE) /* to prevent double declaration */
#error SOMEIPXF_ARG_TLV_CONTEXT_TAG_SIZE already defined
#endif /* if (defined SOMEIPXF_ARG_TLV_CONTEXT_TAG_SIZE) */
#define SOMEIPXF_ARG_TLV_CONTEXT_TAG_SIZE 3U

#if (defined SOMEIPXF_STRUCT_TLV_CONTEXT_TAG_SIZE) /* to prevent double declaration */
#error SOMEIPXF_STRUCT_TLV_CONTEXT_TAG_SIZE already defined
#endif /* if (defined SOMEIPXF_STRUCT_TLV_CONTEXT_TAG_SIZE) */
#define SOMEIPXF_STRUCT_TLV_CONTEXT_TAG_SIZE 5U

#if (defined SOMEIPXF_STRUCT_TLV_CONTEXT_TAG_AND_LF_SIZE) /* to prevent double declaration */
#error SOMEIPXF_STRUCT_TLV_CONTEXT_TAG_AND_LF_SIZE already defined
#endif /* if (defined SOMEIPXF_STRUCT_TLV_CONTEXT_TAG_AND_LF_SIZE) */
#define SOMEIPXF_STRUCT_TLV_CONTEXT_TAG_AND_LF_SIZE 6U

#if (defined SOMEIPXF_STRUCT_TLV_FIXED_MEMBER_COUNT_OFFSET) /* to prevent double declaration */
#error SOMEIPXF_STRUCT_TLV_FIXED_MEMBER_COUNT_OFFSET already defined
#endif /* if (defined SOMEIPXF_STRUCT_TLV_FIXED_MEMBER_COUNT_OFFSET) */
#define SOMEIPXF_STRUCT_TLV_FIXED_MEMBER_COUNT_OFFSET 4U

#if (defined SOMEIPXF_STRUCT_TLV_OPT_MEMBER_CONTEXT_TAG_SIZE) /* to prevent double declaration */
#error SOMEIPXF_STRUCT_TLV_OPT_MEMBER_CONTEXT_TAG_SIZE already defined
#endif /* if (defined SOMEIPXF_STRUCT_TLV_OPT_MEMBER_CONTEXT_TAG_SIZE) */
#define SOMEIPXF_STRUCT_TLV_OPT_MEMBER_CONTEXT_TAG_SIZE 4U

#if (defined SOMEIPXF_STRUCT_TLV_MEMBER_CONTEXT_TAG_SIZE) /* to prevent double declaration */
#error SOMEIPXF_STRUCT_TLV_MEMBER_CONTEXT_TAG_SIZE already defined
#endif /* if (defined SOMEIPXF_STRUCT_TLV_MEMBER_CONTEXT_TAG_SIZE) */
#define SOMEIPXF_STRUCT_TLV_MEMBER_CONTEXT_TAG_SIZE 1U

#if (defined SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE) /* to prevent double declaration */
#error SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE already defined
#endif /* if (defined SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE) */
#define SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE 2U

#if (defined SOMEIPXF_ARG_TLV_STATIC_LF_BYTE_SIZE_CONFIG_OFFSET) /* To prevent double declaration */
#error SOMEIPXF_ARG_TLV_STATIC_LF_BYTE_SIZE_CONFIG_OFFSET already defined
#endif /* (defined SOMEIPXF_ARG_TLV_STATIC_LF_BYTE_SIZE_CONFIG_OFFSET) */

#define SOMEIPXF_ARG_TLV_STATIC_LF_BYTE_SIZE_CONFIG_OFFSET 20U

#if (defined SomeIpXf_IsAvailable_TLV) /* to prevent double declaration */
#error SomeIpXf_IsAvailable_TLV already defined
#endif /* if (defined SomeIpXf_IsAvailable_TLV) */
/* SWS_SomeIpXf_00290 */
#define SomeIpXf_IsAvailable_TLV(data, byte_pos, bit_pos) ((data[byte_pos] & (1U << bit_pos)) != 0U)

#if (defined SomeIpXf_SetAvailable_TLV) /* to prevent double declaration */
#error SomeIpXf_SetAvailable_TLV already defined
#endif /* if (defined SomeIpXf_SetAvailable_TLV) */
#define SomeIpXf_SetAvailable_TLV(data, byte_pos, bit_pos) \
            data[byte_pos] = data[byte_pos] | (1U << bit_pos)

#if (defined SOMEIPXF_WRITE_TLV_TAG) /* to prevent double declaration */
#error SOMEIPXF_WRITE_TLV_TAG already defined
#endif /* if (defined SOMEIPXF_WRITE_TLV_TAG) */
#define SOMEIPXF_WRITE_TLV_TAG(BytesCopied, BufPtr, Wiretype, DataId)                                  \
    do{                                                                                                \
        /* set TLV_tag_header (WireType + DataID) within Buffer (BE format within byte stream) */      \
        (BufPtr[(BytesCopied)]) = (uint8) (((Wiretype << 0x04U) & 0x70U) | ((DataId & 0xF00U) >> 8U)); \
        (BytesCopied) = (BytesCopied) + 1U;                                                            \
        (BufPtr[(BytesCopied)]) = (uint8) (DataId & 0xFFU);                                            \
        (BytesCopied) = (BytesCopied) + 1U;                                                            \
    }while(0)

#if (defined SOMEIPXF_GET_TLV_TAG_VALUE) /* to prevent double declaration */
#error SOMEIPXF_GET_TLV_TAG_VALUE already defined
#endif /* if (defined SOMEIPXF_GET_TLV_TAG_VALUE) */
#define SOMEIPXF_GET_TLV_TAG_VALUE(BufPtr, TLV_tag_header, TLV_tag_header_byte_offset, BytesCopied)         \
    do {                                                                                                    \
      (TLV_tag_header) = (uint16)(((uint16)((BufPtr)[(BytesCopied) + (TLV_tag_header_byte_offset) + 1U])) | \
          ((uint16)(((uint16)((BufPtr)[(BytesCopied) + (TLV_tag_header_byte_offset)])) << 8U)));            \
    } while(0)

#if (defined SOMEIPXF_GET_TLV_TAG_DATAID) /* to prevent double declaration */
#error SOMEIPXF_GET_TLV_TAG_DATAID already defined
#endif /* if (defined SOMEIPXF_GET_TLV_TAG_DATAID) */
#define SOMEIPXF_GET_TLV_TAG_DATAID(TLV_tag_dataID, TLV_tag_header) \
do {                                                                \
      (TLV_tag_dataID) = (uint16) ((TLV_tag_header) & 0x0FFFU);     \
    } while(0)

#if (defined SOMEIPXF_GET_TLV_DATAMEM_SIZE_WIRETYPE0123) /* to prevent double declaration */
#error SOMEIPXF_GET_TLV_DATAMEM_SIZE_WIRETYPE0123 already defined
#endif /* if (defined SOMEIPXF_GET_TLV_DATAMEM_SIZE_WIRETYPE0123) */
#define SOMEIPXF_GET_TLV_DATAMEM_SIZE_WIRETYPE0123(TLV_tag_WireType, DataMemSize)   \
  do {                                                                              \
      switch((TLV_tag_WireType))                                                    \
      {                                                                             \
        case 0U:{ (DataMemSize) = 1U; } break;                                      \
        case 1U:{ (DataMemSize) = 2U; } break;                                      \
        case 2U:{ (DataMemSize) = 4U; } break;                                      \
        case 3U:{ (DataMemSize) = 8U; } break;                                      \
        default:                                                                    \
        {                                                                           \
          /* this should never be reached */                                        \
          (DataMemSize) = 1U;                                                       \
          /* defensive programming mechanism (no Det interface) */                  \
          RetVal = E_SER_GENERIC_ERROR;                                             \
        }                                                                           \
        break;                                                                      \
      }                                                                             \
    } while(0)

#if (defined SOMEIPXF_GET_TLV_TAG_WIRETYPE) /* to prevent double declaration */
#error SOMEIPXF_GET_TLV_TAG_WIRETYPE already defined
#endif /* if (defined SOMEIPXF_GET_TLV_TAG_WIRETYPE) */
#define SOMEIPXF_GET_TLV_TAG_WIRETYPE(TLV_tag_header, TLV_tag_WireType)  \
  do{                                                                    \
    (TLV_tag_WireType) = (uint16) (((TLV_tag_header) & 0x7000U) >> 12U); \
    }while(0)

#if (defined SOMEIPXF_CLEAR_TLV_STRUCT_CONTEXT) /* to prevent double declaration */
#error SOMEIPXF_CLEAR_TLV_STRUCT_CONTEXT already defined
#endif /* if (defined SOMEIPXF_CLEAR_TLV_STRUCT_CONTEXT) */
#define SOMEIPXF_CLEAR_TLV_STRUCT_CONTEXT(TLV_Struct_NestedLevel, RowMax)  \
  do{                                                                      \
      uint32 ii_NestedLevel = 0U;                                          \
      for(ii_NestedLevel = 0U; ii_NestedLevel < RowMax; ii_NestedLevel++)  \
      {                                                                    \
        NestedTLVStruct[ii_NestedLevel][(TLV_Struct_NestedLevel) + 1] = 0U;\
      }                                                                    \
    }while(0)

#if (defined SOMEIPXF_CLEAR_AVAILABLITY_BIT_FIELD) /* to prevent double declaration */
#error SOMEIPXF_CLEAR_AVAILABLITY_BIT_FIELD already defined
#endif /* if (defined SOMEIPXF_CLEAR_AVAILABLITY_BIT_FIELD) */
#define SOMEIPXF_CLEAR_AVAILABLITY_BIT_FIELD(DataBytePtr)                                                               \
  do {                                                                                                       \
      const SOMEIPXF_XFRMBUFFERLENGTH_TYPE FixMemCount = ConfigPtr[CfgElemHandled + 5U];                     \
      const SOMEIPXF_XFRMBUFFERLENGTH_TYPE DataIdCount = ConfigPtr[CfgElemHandled + 6U];                     \
      /* Check if any optional member is present */                                                          \
      if(FixMemCount != DataIdCount)                                                                         \
      {                                                                                                      \
        /*clear availability bit field*/                                                                     \
        uint32 ii = 0U;                                                                                      \
        const SOMEIPXF_XFRMBUFFERLENGTH_TYPE SizeofavailabilityBitfield = ConfigPtr[CfgElemHandled + 4U];    \
        const SOMEIPXF_XFRMBUFFERLENGTH_TYPE OffsetofavailabilityBitfield = ConfigPtr[CfgElemHandled + 3U];  \
        for(ii = 0U; ii < SizeofavailabilityBitfield; ii++)                                                  \
        {                                                                                                    \
          (DataBytePtr[OffsetofavailabilityBitfield + ii +  DeSerContextPtr->DataByteOffset]) = 0U;          \
        }                                                                                                    \
      }                                                                                                      \
  } while(0)

#if (defined SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE) /* to prevent double declaration */
#error SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE already defined
#endif /* if (defined SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE) */
#define SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(memb) \
    TLVContextPtr->TLV_Info_LEVEL[TLVContextPtr->TLV_Current_NestedLevel].memb

#if (defined SOMEIPXF_GETWIRETYPE) /* to prevent double declaration */
#error SOMEIPXF_GETWIRETYPE already defined
#endif /* if (defined SOMEIPXF_GETWIRETYPE) */
#define SOMEIPXF_GETWIRETYPE(Tag_WireType, BytesCopied, BufPtr) \
    Tag_WireType = (uint8)(((BufPtr)[BytesCopied] >> 4U) & 0x07U)

#if (defined SOMEIPXF_CLEAR_StructWithTLVContext_StructWithTLV_InfoType) /* to prevent double declaration */
#error SOMEIPXF_CLEAR_StructWithTLVContext_StructWithTLV_InfoType already defined
#endif /* if (defined SOMEIPXF_CLEAR_StructWithTLVContext_StructWithTLV_InfoType) */

#if(SOMEIPXF_TLV_STRUCT_MEMBER_UNIQUE_DATAID_CHECK_ENABLE == STD_ON)
#define SOMEIPXF_CLEAR_StructWithTLVContext_StructWithTLV_InfoType(nested_level) \
  do{ \
      uint32 IndexCount = 0U; \
      for(IndexCount = 0U; IndexCount < (SOMEIPXF_MAX_TLV_IDS_BYTES); IndexCount++)  \
      { \
        TLVContextPtr->TLV_Info_LEVEL[(nested_level)].DataID_Processed[IndexCount] = 0U;\
      } \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVDataIDCount = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVDataIDStartCfgHandleId = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].FixMemCount = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].EndinessPosition = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].PositionofLF = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLV_static_lenghtfield_bytesize = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].MembIdx = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVStruct_LFsize = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level) + 1U].ExtElement = FALSE; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].PositionofStopTlvTag = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVDataIDProcessedCount = 0U; \
      TLVContextPtr->TLV_Struct_Deserial_FirstElement = FALSE; \
    } while(0)
#else
#define SOMEIPXF_CLEAR_StructWithTLVContext_StructWithTLV_InfoType(nested_level) \
  do{ \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVDataIDCount = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVDataIDStartCfgHandleId = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].FixMemCount = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].EndinessPosition = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].PositionofLF = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLV_static_lenghtfield_bytesize = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].MembIdx = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVStruct_LFsize = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level) + 1U].ExtElement = FALSE; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].PositionofStopTlvTag = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVDataIDProcessedCount = 0U; \
      TLVContextPtr->TLV_Struct_Deserial_FirstElement = FALSE; \
    } while(0)
#endif /* if(SOMEIPXF_TLV_STRUCT_MEMBER_UNIQUE_DATAID_CHECK_ENABLE == STD_ON) */

#if (defined SOMEIPXF_CLEAR_TLVContext_TLV_InfoType) /* to prevent double declaration */
#error SOMEIPXF_CLEAR_TLVContext_TLV_InfoType already defined
#endif /* if (defined SOMEIPXF_CLEAR_TLVContext_TLV_InfoType) */


#if(SOMEIPXF_TLV_STRUCT_MEMBER_UNIQUE_DATAID_CHECK_ENABLE == STD_ON)
#define SOMEIPXF_CLEAR_TLVContext_TLV_InfoType(nested_level) \
  do{ \
      uint32 IndexCount = 0U; \
      for(IndexCount = 0U; IndexCount < (SOMEIPXF_MAX_TLV_IDS_BYTES); IndexCount++)  \
      { \
        TLVContextPtr->TLV_Info_LEVEL[(nested_level)].DataID_Processed[IndexCount] = 0U;\
      } \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVDataIDCount = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVDataIDStartCfgHandleId = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].FixMemCount = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].EndinessPosition = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].PositionofLF = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLV_static_lenghtfield_bytesize = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].MembIdx = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVStruct_LFsize = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].ExtElement = FALSE; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].PositionofStopTlvTag = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVDataIDProcessedCount = 0U; \
      TLVContextPtr->TLV_Current_NestedLevel = 0U; \
      TLVContextPtr->TLV_Root = TRUE; \
      TLVContextPtr->TLV_Struct_Deserial_FirstElement = FALSE; \
    } while(0)
#else
#define SOMEIPXF_CLEAR_TLVContext_TLV_InfoType(nested_level) \
  do{ \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVDataIDCount = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVDataIDStartCfgHandleId = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].FixMemCount = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].EndinessPosition = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].PositionofLF = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLV_static_lenghtfield_bytesize = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].MembIdx = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVStruct_LFsize = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].ExtElement = FALSE; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].PositionofStopTlvTag = 0U; \
      TLVContextPtr->TLV_Info_LEVEL[(nested_level)].TLVDataIDProcessedCount = 0U; \
      TLVContextPtr->TLV_Current_NestedLevel = 0U; \
      TLVContextPtr->TLV_Root = TRUE; \
      TLVContextPtr->TLV_Struct_Deserial_FirstElement = FALSE; \
    } while(0)
#endif/* if(SOMEIPXF_TLV_STRUCT_MEMBER_UNIQUE_DATAID_CHECK_ENABLE == STD_ON) */

/*==================[type definitions]======================================*/

/** \brief definition of the SomIpXf_DimensionType */
typedef uint8 SomIpXf_DimensionType;

/*==================[internal function declarations]========================*/
#define SOMEIPXF_START_SEC_CODE
#include <SomeIpXf_MemMap.h>

/* Serialize the basic data types */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerializeBasicType
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)

/* set up structure with length field context */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SetUpStructWithLfContext
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(StructWithLf_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) StructWithLfContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength
);

/* handling structure with length field context */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerHandlingStructWithLfContext
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(StructWithLf_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) StructWithLfContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

/* handling structure with length field context */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserHandlingStructWithLfContext
(
   P2VAR(StructWithLf_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) StructWithLfContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */

#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)

/* serialize an array of basic data type with fix size */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerializeFSAOfBasicType
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

/* set up FSA context */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SetUpFSAContext
(
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)
/* set up extensible FSA context */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SetUpExtFSAContext
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

/* serialize an array of basic data type with fix size */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerializeExtFSAOfBasicType
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr
);

/* Deserialize array of fixed size with basic data types (not of a structure) */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserializeExtFSAOfBasicType
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength
);
#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */

/* handling FSA of struct context */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerHandlingFSAContext
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerHandlingFSADim
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType FSA_Dim
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_FSA_elements_ser
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType FSA_Dim
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_FSA_elements_rem
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType FSA_Dim
);

/* handling FSA of struct context */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserHandlingFSAContext
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserHandlingFSADim
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType FSA_Dim
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_FSA_elements_deser
(
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType FSA_Dim
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_FSA_elements_deser_rem
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserFSADimSkippingBytes
(
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr
);
#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */

#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */

#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SetUpVSAContext_DimLevel
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) CfgElemHandledPtr,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) VSA_LF_BuffPos,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SetUpVSAContext
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_VSAGetSizeIndicatorValues
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) ElementNum,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_SI_offsetof,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_SI_sizeof
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerHandlingVSADim
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType VSA_Dim
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerializeVSA_element_check_point
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Set_SI
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE CurrElement,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_SI_offsetof,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_SI_sizeof
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_VSA_elements_deser
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType VSA_Dim
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_VSA_elements_deser_rem
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   uint32 Element_ByteLength
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserHandlingVSADim
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType VSA_Dim
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserializeVSA_element_check_point
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */

/* Deserialize basic data type */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserializeBasicType
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength
);

#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
/* Deserialize array of fixed size with basic data types (not of a structure) */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserializeFSAOfBasicType
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength
);
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */

#if ( (SOMEIPXF_VSA_AVAILABLE == STD_ON) || \
      (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) || \
      (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) )

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Set_LF
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE LFBufferPos,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE LFValue,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE LFSize,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfigPosNWEndianness
);

#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) ||
          (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) ||
          (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */

#if ( (SOMEIPXF_VSA_AVAILABLE == STD_ON) || \
      (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) || \
      (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) || \
      (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || \
      (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) )

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Set_ConfigPosNWEndianness
(
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) ConfigPosNWEndianness,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE LFSize
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_GetLFValues
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BuffPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(uint32, AUTOMATIC, SOMEIPXF_APPL_DATA) LF_ValuePtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE LF_BuffPos,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfigPosNWEndianness,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE LFsize
);

#endif /* ( (SOMEIPXF_VSA_AVAILABLE == STD_ON) || \
            (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) || \
            (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) || \
            (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || \
            (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) ) */

#if (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON)
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_GetNextArg (
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(uint32, AUTOMATIC, SOMEIPXF_APPL_DATA) TLV_ARG_BufPosPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLV_static_LF_bytesize_Index,
  uint16 TLV_tag_WireType
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_TLV_Arg_Find (
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(uint32, AUTOMATIC, SOMEIPXF_APPL_DATA) TLV_ARG_BufPosPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength
);

STATIC FUNC(void, SOMEIPXF_CODE) SomeIpXf_TLV_Arg_Set (
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);
#endif /* (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) */

#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_TLV_SetUpContext (
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

/* setting of TLV tag of TLV optional member  */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_TLV_Opt_Member_SetTLVTag (
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

/* setting of TLV tag of TLV fixed member  */
STATIC FUNC(void, SOMEIPXF_CODE) SomeIpXf_TLV_Fix_Member_SetTLVTag (
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

/* handling of TLV stop  */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_TlvStop (
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied
);

/* Setting of TLV LF*/
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Set_TLV_lenghtfield (
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied
);

/* Setting up the context for TLV inv */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_InvTLVSetContext (
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) CfgElemHandledPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) TLV_LFSize_WireType567
);

STATIC FUNC(uint8, SOMEIPXF_CODE)  SomeIpXf_TlvSkipCurrentDataId (
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BytesCopiedPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) CfgElemHandledPtr
);

/* handling of TLV inverese start */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_TlvStart (
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

/* handling of TLV inverese optional member */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_TLV_Opt_Member_Start (
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

/* handling of TLV inverese member stop */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_TLV_Member_Stop (
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_Get_NextDataIdPos (
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BytesCopiedPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr
);


STATIC FUNC(void, SOMEIPXF_CODE) SomeIpXf_ValidateReceivedDataId(
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(boolean, AUTOMATIC, SOMEIPXF_APPL_DATA)  DataID_validPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) ValidDataIdIndexPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE StartIndexInConfigDataId,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE StopIndexInConfigDataId,
  uint16 TLV_tag_dataID
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_CheckValidWiretypeAndDuplicateKnownMember (
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) CfgElemHandledPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE ValidDataIdIndex,
  uint16 TLV_tag_header
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Int_DataID_WireType_Valid (
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(boolean, AUTOMATIC, SOMEIPXF_APPL_DATA)  DataID_validPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) CfgElemHandledPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr
);

/* handling of TLV inverese fixed member */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_TLV_Fix_Member_Start (
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

/* handling of TLV inverese tlv stop */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_TlvStop (
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr
);

#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */

#if ((SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON))

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Get_TLV_LFSize_And_EndianessPos (
  uint16 Tag_WireType,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) TLV_static_lenghtfield_bytesizeptr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) ConfigPosNWEndiannessptr
);

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_GetLFByteSize (
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) TLV_static_lf_bytesizeptr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE WireType
);

#endif /* ((SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)) */

#if (SOMEIPXF_ET_AVAILABLE == STD_ON)
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_ET_GetHeader
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   uint8 InterfaceVersion
);
#endif /* (SOMEIPXF_ET_AVAILABLE == STD_ON) */

/* pre-condition checks on SomeIpXf config array */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_ConfigArray
(
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
);

#define SOMEIPXF_STOP_SEC_CODE
#include <SomeIpXf_MemMap.h>
/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define SOMEIPXF_START_SEC_CODE
#include <SomeIpXf_MemMap.h>

/*-------------------- SomeIpXf_S_Int_GenericTransformer ----------------------*/
/* Deviation CODEMETRIC-1 */
FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_S_Int_GenericTransformer
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(void, AUTOMATIC, SOMEIPXF_APPL_DATA) DataPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_AdminDataType, AUTOMATIC, SOMEIPXF_APPL_DATA) AdminData
)
{
/* force coverage instrumentation to start after variable init to prevent compiler warning */
/* CHECK: NOPARSE */
   /* used for defensive programming only (no Det interface) */
   uint8 RetVal = E_OK;
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr =
      (P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA)) DataPtr;

   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr =
          AdminData->DeSerContextPtr;
#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr =
       AdminData->FSAContextPtr;
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)
   P2VAR(StructWithLf_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) StructWithLfContextPtr =
       AdminData->StructWithLfContextPtr;
#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */
#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr =
       AdminData->VSAContextPtr;
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)
   P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr = AdminData->TLVContextPtr;
#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */

   SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied = DeSerContextPtr->BytesCopied;
   boolean ElemFound = TRUE;

/* CHECK: PARSE */

/* CHECK: NOPARSE */
   /* check if CfgElemHandled is within config array size and all case tags are larger then stop */
   if( (CfgElemHandled >= ConfigPtr[1U]) || (ConfigPtr[CfgElemHandled] < SOMEIPXF_DATA_ELEMENT_STOP) )
   {
     RetVal = E_SER_GENERIC_ERROR;
   }
   else
/* CHECK: PARSE */
   {
      switch (ConfigPtr[CfgElemHandled])
      {

/*=================================================================*/
/*               B A S I C   T Y P E S                             */
/*=================================================================*/

         case SOMEIPXF_BASIC_DATA_TYPE_START:
         {
            /* serialize basic data type */
            RetVal = SomeIpXf_SerializeBasicType(
               &BufPtr[BytesCopied], /* out: points to current output buffer position */
               &DataBytePtr[0U],  /* in: uint8 representation of the data element */
               &ConfigPtr[CfgElemHandled], /* in: points to current configuration element */
               DeSerContextPtr /* in/out: general de-/serialization context */
            );
         }
         break;

/*=================================================================*/
/*                      S T R U C T U R E S                        */
/*=================================================================*/
#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)

         case SOMEIPXF_STRUCT_WITH_LF_START:
         {
            /* set up structure with length field context */
            RetVal = SomeIpXf_SetUpStructWithLfContext(
                &BufPtr[0U], /* in: points to current output buffer position */
                &ConfigPtr[0U], /* in: points to current configuration element */
                StructWithLfContextPtr, /* in: StructWithLf context */
                DeSerContextPtr, /* in/out: general de-/serialization context */
                0U /* in: unused BufferLength for ser */
             );
         }
         break;

         case SOMEIPXF_STRUCT_WITH_LF_STOP:
         {
            /* handling structure with length field context */
            RetVal = SomeIpXf_SerHandlingStructWithLfContext(
                BufPtr, /* in: output buffer */
                &ConfigPtr[0U], /* in: points to current configuration element */
                StructWithLfContextPtr, /* in: StructWithLf context */
                DeSerContextPtr /* in/out: general de-/serialization context */
            );
         }
         break;

#endif /* #if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */
/*=================================================================*/
/*           A R R A Y S   O F   F I X E D   S I Z E               */
/*=================================================================*/
#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)

         case SOMEIPXF_FSA_OF_BASIC_TYPE_START:
         {
            /* serialize an array of basic data type with fix size */
            RetVal = SomeIpXf_SerializeFSAOfBasicType(
               &BufPtr[BytesCopied], /* out: points to current output buffer position */
               &DataBytePtr[0U],  /* in: uint8 representation of the data element */
               &ConfigPtr[CfgElemHandled], /* in: points to current configuration element */
               DeSerContextPtr /* in/out: general de-/serialization context */
            );
         }
         break;

         case SOMEIPXF_FSA_OF_FSA_START:
         case SOMEIPXF_FSA_OF_STRUCT_START:
         {
           /* set up FSA of struct/FSA context */
           RetVal = SomeIpXf_SetUpFSAContext(
               &ConfigPtr[CfgElemHandled], /* in: points to current configuration element */
               FSAContextPtr, /* in: FSA context info */
               DeSerContextPtr /* in/out: general de-/serialization context */
           );
         }
         break;

         case SOMEIPXF_FSA_OF_FSA_STOP:
         case SOMEIPXF_FSA_OF_STRUCT_STOP:
         {
           /* handling FSA of struct/FSA context */
           RetVal = SomeIpXf_SerHandlingFSAContext(
               &BufPtr[0U], /* in: points to current output buffer position */
               &ConfigPtr[0U], /* in: points to current configuration element */
               FSAContextPtr, /* in: FSA context info */
               DeSerContextPtr /* in/out: general de-/serialization context */
           );
         }
         break;

#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)
         case SOMEIPXF_FSA_WITH_LF_START:
         {
           RetVal = SomeIpXf_SetUpExtFSAContext(
               &BufPtr[BytesCopied], /* in: points to current output buffer position */
               0U, /* in: unused BufferLength for ser */
               &ConfigPtr[0U], /* in: points to current configuration element */
               FSAContextPtr, /* in: FSA context info */
               DeSerContextPtr /* in/out: general de-/serialization context */
           );
         }
         break;

         case SOMEIPXF_FSA_OF_BASIC_DATA_TYPE_WITH_LF_START:
         {
           /* serialize an array of basic data type with fix size */
           RetVal = SomeIpXf_SerializeExtFSAOfBasicType(
              &BufPtr[BytesCopied], /* out: points to current output buffer position */
              &DataBytePtr[0U],  /* in: uint8 representation of the data element */
              &ConfigPtr[CfgElemHandled], /* in: points to current configuration element */
              DeSerContextPtr, /* in/out: general de-/serialization context */
              FSAContextPtr /* in: FSA context info */
           );
         }
         break;

         case SOMEIPXF_FSA_WITH_LF_STOP:
         {
           RetVal = SomeIpXf_SerHandlingFSAContext(
               &BufPtr[0U], /* in: points to current output buffer position */
               &ConfigPtr[0U], /* in: points to current configuration element */
               FSAContextPtr, /* in: FSA context info */
               DeSerContextPtr /* in/out: general de-/serialization context */
           );
         }
         break;
#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */

#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */

/*=================================================================*/
/*                         S T R I N G S                           */
/*=================================================================*/

/*=================================================================*/
/*        A R R A Y S   O F   V A R I A B L E   S I Z E            */
/*=================================================================*/

#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
         case SOMEIPXF_VSA_START:
         {
            /* set up VSA context */
            RetVal = SomeIpXf_SetUpVSAContext(
                &BufPtr[BytesCopied], /* in: points to current output buffer position */
                0U, /* in: unused BufferLength for ser */
                &DataBytePtr[0U], /* in: uint8 representation of the data element */
                &ConfigPtr[0U], /* in: points to current configuration element */
                VSAContextPtr, /* in: overall VSA context */
                DeSerContextPtr /* in/out: general de-/serialization context */
            );
         }
         break;

         case SOMEIPXF_VSA_ELEMENT_CHECK_POINT:
         {
            /* VSA element handling */
            RetVal = SomeIpXf_SerializeVSA_element_check_point(
                BufPtr, /* in: output buffer */
                &ConfigPtr[0U], /* in: points to current configuration element */
                VSAContextPtr, /* in: overall VSA context */
                DeSerContextPtr /* in/out: general de-/serialization context */
            );
         }
         break;
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */

/*=================================================================*/
/*        E X T E N S I B L E S T R U C T S - T L V                */
/*=================================================================*/
#if(SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON)

         case SOMEIPXF_ARG_TLV_START:
         {
            SomeIpXf_TLV_Arg_Set(
                BufPtr, /* out: points to current output buffer position */
                &ConfigPtr[0U], /* in: points to current configuration element */
                DeSerContextPtr /* in/out: general de-/serialization context */
              );
         }
         break;

#endif /* (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) */

#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)

         case SOMEIPXF_STRUCT_TLV_START:
         {
           /*Context for TLV element */
           RetVal = SomeIpXf_TLV_SetUpContext(
                   &ConfigPtr[0U], /* in: points to current configuration element */
                   TLVContextPtr,  /* in: TLV context info */
                   DeSerContextPtr /* in/out: general de-/serialization context */
                 );
         }
         break;

         case SOMEIPXF_STRUCT_TLV_MEMBER_OPT_START:
         {
           /*Context for optional member */
           RetVal = SomeIpXf_TLV_Opt_Member_SetTLVTag(
               BufPtr,  /* output buffer */
               &DataBytePtr[0U],  /* in: uint8 representation of the data element */
               &ConfigPtr[0U], /* in: points to current configuration element */
               DeSerContextPtr /* in/out: general de-/serialization context */
           );
         }
         break;

         case SOMEIPXF_STRUCT_TLV_MEMBER_FIX_START:
         {
           /*Context for fix member */
           SomeIpXf_TLV_Fix_Member_SetTLVTag(
             BufPtr, /* output buffer */
             &ConfigPtr[0U], /* in: points to current configuration element */
             TLVContextPtr, /* in/out: TLV context info */
             DeSerContextPtr /* in/out: general de-/serialization context */
           );
         }
         break;

         case SOMEIPXF_STRUCT_TLV_STOP:
         {
           /* TLV stop set the length field */
           RetVal = SomeIpXf_TlvStop(
               BufPtr,  /* output buffer */
               &ConfigPtr[0U], /* in: points to current configuration element */
               TLVContextPtr, /* in/out: TLV context info */
               BytesCopied  /* in: overall number of bytes copied to the buffer */
           );
         }
         break;

         case SOMEIPXF_STRUCT_TLV_MEMBER_OPT_STOP:
         case SOMEIPXF_STRUCT_TLV_MEMBER_FIX_STOP:
         break;

#endif  /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */
/*=================================================================*/
/*                         O T H E R S                             */
/*=================================================================*/

         case SOMEIPXF_DATA_ELEMENT_START:
         {
           /* pre-condition checks on SomeIpXf config array */
           RetVal = SomeIpXf_ConfigArray(
               &ConfigPtr[0U], /* in: points to current configuration element */
               DeSerContextPtr /* in/out: general de-/serialization context */
            );
         }
         break;

         /* CHECK: NOPARSE */
         default:
         {
            /* this should never be reached */
            /* defensive programming mechanism (no Det interface) */
            RetVal = E_SER_GENERIC_ERROR;
            ElemFound = FALSE;
         }
         break;
         /* CHECK: PARSE */

      } /* end of switch (ConfigPtr[CfgElemHandled] */

   /* end of serialized data type reached in configuration */
   /* RetVal not considered in while condition since only used for defensive programming reason */
   }

   AdminData->ElemFound = ElemFound;

   /* used for defensive programming only (no Det interface) */
   return RetVal;
}


/*------------------- SomeIpXf_S_Int_Inv_GenericTransformer --------------------*/
/* Deviation CODEMETRIC-1 */
FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_S_Int_Inv_GenericTransformer
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA) DataPtr,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BytesCopiedPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_AdminDataType, AUTOMATIC, SOMEIPXF_APPL_DATA) AdminData
)
{
/* force coverage instrumentation to start after variable init to prevent compiler warning */
/* CHECK: NOPARSE */
   uint8 RetVal = E_OK;
   /* Deviation MISRAC2012-2 <+2> */
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr =
      (P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA)) DataPtr;

   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr =
          AdminData->DeSerContextPtr;
#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr =
       AdminData->FSAContextPtr;
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)
   P2VAR(StructWithLf_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) StructWithLfContextPtr =
       AdminData->StructWithLfContextPtr;
#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */
#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr =
       AdminData->VSAContextPtr;
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE DataByteOffset = DeSerContextPtr->DataByteOffset;
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)
   P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr = AdminData->TLVContextPtr;
#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */
#if (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON)
  uint32 TLV_ARG_BufPos = AdminData->TLV_ARG_BufPos;
#endif /* (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) */
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied = DeSerContextPtr->BytesCopied;
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;

   boolean ElemFound = TRUE;
/* CHECK: PARSE */

#if (SOMEIPXF_VSA_AVAILABLE == STD_OFF)
   /* Deviation MISRAC2012-3 */
   SOMEIPXF_PARAM_UNUSED(BufferLength);
#endif
   /* Deviation MISRAC2012-3 */
   SOMEIPXF_PARAM_UNUSED(BytesCopiedPtr);

/* CHECK: NOPARSE */
   /* check if CfgElemHandled is within config array size and all case tags are larger then stop */
   if( (CfgElemHandled >= ConfigPtr[1U]) || (ConfigPtr[CfgElemHandled] < SOMEIPXF_DATA_ELEMENT_STOP) )
   {
     RetVal = E_SER_GENERIC_ERROR;
   }
   else
/* CHECK: PARSE */
   {
      switch (ConfigPtr[CfgElemHandled])
      {
/*=================================================================*/
/*               B A S I C   T Y P E S                             */
/*=================================================================*/

         case SOMEIPXF_BASIC_DATA_TYPE_START:
         {
            /* deserialize basic data type, returns E_OK */
            RetVal = SomeIpXf_DeserializeBasicType(
               &BufPtr[BytesCopied], /* in: points to next free buffer position */
               &DataBytePtr[0U],  /* out: uint8 representation of the data element */
               &ConfigPtr[CfgElemHandled], /* in: points to current configuration element */
               DeSerContextPtr, /* in/out: general de-/serialization context */
               BufferLength /* in: BufferLength for deser */
            );
         }
         break;

/*=================================================================*/
/*                      S T R U C T U R E S                        */
/*=================================================================*/
#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)

         case SOMEIPXF_STRUCT_WITH_LF_START:
         {
           /* set up structure with length field context */
           RetVal = SomeIpXf_SetUpStructWithLfContext(
               &BufPtr[0U], /* in: points to current output buffer position */
               &ConfigPtr[0U], /* in: points to current configuration element */
               StructWithLfContextPtr, /* in: StructWithLf context */
               DeSerContextPtr, /* in/out: general de-/serialization context */
               BufferLength /* in: BufferLength for deser */
           );
         }
         break;

         case SOMEIPXF_STRUCT_WITH_LF_STOP:
         {
           /* handling structure with length field context */
           RetVal = SomeIpXf_DeserHandlingStructWithLfContext(
               StructWithLfContextPtr, /* in: StructWithLf context */
               DeSerContextPtr  /* in/out: overall number of bytes copied to the buffer */
           );
         }
         break;

#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */
/*=================================================================*/
/*           A R R A Y S   O F   F I X E D   S I Z E               */
/*=================================================================*/
#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)

         case SOMEIPXF_FSA_OF_BASIC_TYPE_START:
         {
            /* deserialize an array of basic data type with fix size, returns E_OK */
            RetVal = SomeIpXf_DeserializeFSAOfBasicType(
               &BufPtr[BytesCopied], /* in: points to next free buffer position */
               &DataBytePtr[0U], /* out: uint8 representation of the data element */
               &ConfigPtr[CfgElemHandled], /* in: points to current configuration element */
               DeSerContextPtr, /* in/out: overall number of bytes copied to the buffer */
               BufferLength /* in: BufferLength for deser */
            );
         }
         break;

         case SOMEIPXF_FSA_OF_FSA_START:
         case SOMEIPXF_FSA_OF_STRUCT_START:
         {
           RetVal = SomeIpXf_SetUpFSAContext(
               &ConfigPtr[CfgElemHandled], /* in: points to current configuration element */
               FSAContextPtr, /* in: FSA context info */
               DeSerContextPtr /* in/out: general de-/serialization context */
           );
         }
         break;

         case SOMEIPXF_FSA_OF_FSA_STOP:
         case SOMEIPXF_FSA_OF_STRUCT_STOP:
         {
           /* handling FSA of struct context */
           RetVal = SomeIpXf_DeserHandlingFSAContext(
               &BufPtr[0U], /* in: points to current output buffer position */
               BufferLength, /* in: BufferLength */
               &ConfigPtr[0U], /* in: points to current configuration element */
               FSAContextPtr, /* in: FSA context info */
               DeSerContextPtr /* in/out: general de-/serialization context */
           );
         }
         break;

#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)
         case SOMEIPXF_FSA_WITH_LF_START:
         {
           RetVal = SomeIpXf_SetUpExtFSAContext(
               &BufPtr[0U], /* in: points to current output buffer position */
               BufferLength, /* in: BufferLength */
               &ConfigPtr[0U], /* in: points to current configuration element */
               FSAContextPtr, /* in: FSA context info */
               DeSerContextPtr /* in/out: general de-/serialization context */
           );
         }
         break;

         case SOMEIPXF_FSA_OF_BASIC_DATA_TYPE_WITH_LF_START:
         {
           /* deserialize an array of basic data type with fix size, returns E_OK */
           RetVal = SomeIpXf_DeserializeExtFSAOfBasicType(
              &BufPtr[BytesCopied], /* in: points to next free buffer position */
              &DataBytePtr[0U],  /* out: uint8 representation of the data element */
              &ConfigPtr[CfgElemHandled], /* in: points to current configuration element */
              DeSerContextPtr,  /* in/out: overall number of bytes copied to the buffer */
              FSAContextPtr, /* in: FSA context info */
              BufferLength /* in: BufferLength for deser */
           );
         }
         break;

         case SOMEIPXF_FSA_WITH_LF_STOP:
         {
           RetVal = SomeIpXf_DeserHandlingFSAContext(
               &BufPtr[0U], /* in: points to current output buffer position */
               BufferLength, /* in: BufferLength */
               &ConfigPtr[0U], /* in: points to current configuration element */
               FSAContextPtr, /* in: FSA context info */
               DeSerContextPtr /* in/out: general de-/serialization context */
           );
         }
         break;
#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */

#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */
/*=================================================================*/
/*                         S T R I N G S                           */
/*=================================================================*/

/*=================================================================*/
/*        A R R A Y S   O F   V A R I A B L E   S I Z E            */
/*=================================================================*/

#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
         case SOMEIPXF_VSA_START:
         {
            /* set up VSA context */
            RetVal = SomeIpXf_SetUpVSAContext(
                &BufPtr[BytesCopied], /* in: points to current output buffer position */
                BufferLength, /* in: BufferLength */
                &DataBytePtr[DataByteOffset], /* in: uint8 representation of the data element */
                &ConfigPtr[0U], /* in: points to current configuration element */
                VSAContextPtr, /* in: overall VSA context */
                DeSerContextPtr /* in/out: general de-/serialization context */
            );
         }
         break;

         case SOMEIPXF_VSA_ELEMENT_CHECK_POINT:
         {
            /* VSA element handling */
            RetVal = SomeIpXf_DeserializeVSA_element_check_point(
                &BufPtr[0U], /* in: points to output buffer */
                BufferLength, /* in: BufferLength */
                &DataBytePtr[0U], /* out: uint8 representation of the data element */
                &ConfigPtr[0U], /* in: points to current configuration element */
                VSAContextPtr, /* in: overall VSA context */
                DeSerContextPtr /* in/out: general de-/serialization context */
            );
         }
         break;
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */

/*=================================================================*/
/*        E X T E N S I B L E S T R U C T S - T L V                */
/*=================================================================*/
#if(SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON)

         case SOMEIPXF_ARG_TLV_START:
         {
            /* Find the TLV argument to be processed in byte stream received */
            RetVal = SomeIpXf_TLV_Arg_Find(
              BufPtr, /* in: points to current output buffer position */
              &ConfigPtr[0U], /* in: points to current configuration element */
              &TLV_ARG_BufPos, /* in/out: overall number of bytes handled already */
              DeSerContextPtr, /* in/out: general de-/serialization context */
              BufferLength /* in: used length of the buffer */
              );
         }
         break;

#endif /* (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) */

#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)

         case SOMEIPXF_STRUCT_TLV_START:
         {
            /* Find the first member in config */
            RetVal = SomeIpXf_Inv_TlvStart(
              BufPtr, /* in: points to current output buffer position */
              BufferLength, /* in: BufferLength */
              DataBytePtr, /* out: uint8 representation of the data element */
              &ConfigPtr[0U],/* in: points to current configuration element */
              TLVContextPtr, /* in: TLV context info */
              DeSerContextPtr /* in/out: general de-/serialization context */
              );
         }
         break;

         case SOMEIPXF_STRUCT_TLV_STOP:
         {
            /* Decrement the nested level
             * Return Malformed message in case all fixed member are not processed
             */
            RetVal = SomeIpXf_Inv_TlvStop(
                TLVContextPtr /* in: TLV context info */
            );
         }
         break;

         case SOMEIPXF_STRUCT_TLV_MEMBER_OPT_START:
         {
            /* Process optional member received */
            RetVal = SomeIpXf_Inv_TLV_Opt_Member_Start(
                BufPtr, /* in: points to current output buffer position */
                DataBytePtr, /* out: uint8 representation of the data element  */
                &ConfigPtr[0U], /* in: points to current configuration element */
                TLVContextPtr, /* in: TLV context info */
                DeSerContextPtr /* in/out: general de-/serialization context */
            );
        }
        break;

        case SOMEIPXF_STRUCT_TLV_MEMBER_FIX_START:
        {
            /*De-serilize the fixed member */
            RetVal = SomeIpXf_Inv_TLV_Fix_Member_Start(
                BufPtr, /* in: points to current output buffer position */
                TLVContextPtr, /* in: TLV context info */
                DeSerContextPtr /* in/out: general de-/serialization context */
         );
        }
        break;

        case SOMEIPXF_STRUCT_TLV_MEMBER_FIX_STOP:
        case SOMEIPXF_STRUCT_TLV_MEMBER_OPT_STOP:
        {
            /* Check all members are processed and
             * check the new element in case all are not processed
             */
            RetVal = SomeIpXf_Inv_TLV_Member_Stop(
                &ConfigPtr[0U],/* in: points to current configuration element */
                BufferLength, /* in: BufferLength */
                BufPtr, /* in: points to current output buffer position */
                TLVContextPtr, /* in: TLV context info */
                DeSerContextPtr /* in/out: general de-/serialization context */
            );
        }
        break;

#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */
/*=================================================================*/
/*                         O T H E R S                             */
/*=================================================================*/

        case SOMEIPXF_DATA_ELEMENT_START:
        {
          /* pre-condition checks on SomeIpXf config array */
          RetVal = SomeIpXf_ConfigArray(
              &ConfigPtr[0U], /* in: points to current configuration element */
              DeSerContextPtr /* in/out: general de-/serialization context */
           );
        }
        break;

        /* CHECK: NOPARSE */
        default:
        {
           /* this should never be reached */
           /* defensive programming mechanism (no Det interface) */
           RetVal = E_SER_GENERIC_ERROR;
           ElemFound = FALSE;
        }
        break;
       /* CHECK: PARSE */

      } /* end of switch (ConfigPtr[CfgElemHandled] */
   }

   AdminData->ElemFound = ElemFound;

#if(SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON)
  /* Assign the processed data to global structure */
  AdminData->TLV_ARG_BufPos = TLV_ARG_BufPos;
#endif /* (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) */

   return RetVal;
}

FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SR_GetHeader
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   uint8 InterfaceVersion
)
{
  uint8 RetVal;

  if (BufPtr[4] != SOMEIPXF_PROTOCOL_VERSION)
  {
    RetVal = E_SER_WRONG_PROTOCOL_VERSION;
  }
  else if (BufPtr[5] != InterfaceVersion)
  {
    RetVal = E_SER_WRONG_INTERFACE_VERSION;
  }
  else if((BufPtr[6] != SOMEIPXF_MESSAGE_TYPE_REQUEST_NO_RETURN) &&
          (BufPtr[6] != SOMEIPXF_MESSAGE_TYPE_NOTIFICATION))
  {
    RetVal = E_SER_WRONG_MESSAGE_TYPE;
  }
  else if (BufPtr[7] != 0x00U)
  {
    RetVal = E_SER_GENERIC_ERROR;
  }
  else
  {
    RetVal = E_OK;
  }

  return RetVal;
}

FUNC(void, SOMEIPXF_CODE) SomeIpXf_S_Init_AdminData
(
   P2VAR(SomeIpXf_AdminDataType, AUTOMATIC, SOMEIPXF_APPL_DATA) AdminData,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BytesCopiedPtr,
   boolean TLV_Arg_De_Seral
)
{
#if (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_OFF)
  SOMEIPXF_PARAM_UNUSED(BytesCopiedPtr);
  SOMEIPXF_PARAM_UNUSED(TLV_Arg_De_Seral);
#endif /* (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_OFF) */

  AdminData->DeSerContextPtr = NULL_PTR;
#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
  AdminData->FSAContextPtr = NULL_PTR;
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */
#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)
  AdminData->StructWithLfContextPtr = NULL_PTR;
#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */
#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
  AdminData->VSAContextPtr = NULL_PTR;
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */

#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)
  AdminData->TLVContextPtr = NULL_PTR;
#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */

#if (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON)
  if(SOMEIPXF_DE_SER == TLV_Arg_De_Seral)
  {
    /* Start position (TLV_ARG_BufPos) in the received stream for the
     * search algorithm of TLV Arg Data IDs gets forward via BytesCopiedFromBuffer
     * through the Inv_GenericTransformer calls.
     * BytesCopiedFromBuffer value is always 8U in case of reordering feature
     * for arguments is enabled otherwise the value holds current buffer position.
     */
    AdminData->TLV_ARG_BufPos = *BytesCopiedPtr;
  }
#endif /* (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) */

  AdminData->ElemFound = FALSE;
}

FUNC(void, SOMEIPXF_CODE) SomeIpXf_S_Init_DeSerContext
(
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  DeSerContextPtr->BytesCopied = 0U;
  DeSerContextPtr->CfgElemHandled = 0U;
  DeSerContextPtr->DataByteOffset = 0U;
#if ((SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON))
  /* set LF size to invalid value */
  DeSerContextPtr->TLV_LFSize_WireType567 = 0U;
#endif /* ((SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)) */
}

#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)
FUNC(void, SOMEIPXF_CODE) SomeIpXf_S_Init_StructWithLfContext
(
   P2VAR(StructWithLf_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) StructWithLfContextPtr,
   boolean StructWithLf_De_Seral
)
{
  uint32 info_nested_level_idx = 0U;

  /* Deviation TASKING-1 */
  for(info_nested_level_idx=0U; info_nested_level_idx<SOMEIPXF_MAX_NESTED_STRUCT_WITH_LF_DEPTH; info_nested_level_idx++)
  {
    SOMEIPXF_CLEAR_StructWithLfContext_StructWithLf_InfoType(info_nested_level_idx);
  }
  StructWithLfContextPtr->StructWithLf_current_NESTED_LEVEL = 0U;
  /* initialize Root context member */
  StructWithLfContextPtr->StructWithLf_Root = TRUE;
  StructWithLfContextPtr->StructWithLf_De_Seral = StructWithLf_De_Seral;
}
#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */

#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
FUNC(void, SOMEIPXF_CODE) SomeIpXf_S_Init_FSAContext
(
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   boolean FSA_De_Seral
)
{
  uint32 info_level_idx = 0U;

  /* Deviation TASKING-1 */
  for(info_level_idx=0U; info_level_idx<SOMEIPXF_MAX_NESTED_ARRAY_DEPTH; info_level_idx++)
  {
    /* Deviation TASKING-1 */
    SOMEIPXF_CLEAR_FSAContext_FSA_InfoType(info_level_idx);
  }
  FSAContextPtr->FSA_current_NESTED_LEVEL = 0U;
  /* initialize Root context member */
  FSAContextPtr->FSA_Root = TRUE;
  FSAContextPtr->FSA_De_Seral = FSA_De_Seral;
}
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */

#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
FUNC(void, SOMEIPXF_CODE) SomeIpXf_S_Init_VSAContext
(
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContext,
   boolean VSA_De_Seral
)
{
  uint32 info_level_idx = 0U;

  /* Deviation TASKING-1 */
  for(info_level_idx=0U;info_level_idx<SOMEIPXF_MAX_NESTED_VSA_DEPTH;info_level_idx++)
  {
    /* Deviation TASKING-1 */
    SOMEIPXF_CLEAR_VSAContext_VSA_Info_LEVEL(info_level_idx);
  }
  VSAContext->VSA_current_NESTED_LEVEL = 0U;
  /* initialize Root context member */
  VSAContext->VSA_Root = TRUE;
  VSAContext->VSA_De_Seral = VSA_De_Seral;
}
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */

#if (SOMEIPXF_CS_AVAILABLE == STD_ON)
FUNC(void, SOMEIPXF_CODE) SomeIpXf_Request_SetHeader
(
   P2CONST(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle,
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   uint8 InterfaceVersion
)
{
   /* header of client/server communication serialized with
      RequestId,        - TransactionHandle (input)
      ProtocolVersion   - SOMEIPXF_PROTOCOL_VERSION, (generated macro)
      InterfaceVersion  - InterfaceVersion (input, generated to API)
      MessageType       - SOMEIPXF_MESSAGE_TYPE_REQUEST (SWS_SomeIpXf_00031)
      ReturnCode        - 0x00 (SWS_SomeIpXf_00033) */

   /* RequestId */
   SOMEIPXF_SET_REQUEST_ID(TransactionHandle->clientId,
                           TransactionHandle->sequenceCounter,
                           BufPtr);

   /* ProtocolVersion (generated) */
   BufPtr[4] = (uint8)SOMEIPXF_PROTOCOL_VERSION;

   /* InterfaceVersion */
   BufPtr[5] = InterfaceVersion;

   /* MessageType is always SOMEIPXF_MESSAGE_TYPE_REQUEST (SWS_SomeIpXf_00033) */
   BufPtr[6] = SOMEIPXF_MESSAGE_TYPE_REQUEST;

   /* ReturnCode is always 0x00 (SWS_SomeIpXf_00033) */
   BufPtr[7] = 0x00U;
}


FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Request_GetHeader
(
   P2VAR(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle,
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   uint8 InterfaceVersion
)
{
   uint8 RetVal;

   /* header of client/server communication serialized with
      RequestId,        - TransactionHandle (output)
      ProtocolVersion   - SOMEIPXF_PROTOCOL_VERSION, (generated macro)
      InterfaceVersion  - InterfaceVersion (input, generated to API)
      MessageType       - SOMEIPXF_MESSAGE_TYPE_REQUEST (SWS_SomeIpXf_00031) -> no check necessary
      ReturnCode        - 0x00 (SWS_SomeIpXf_00033) -> no check necessary */

   if (BufPtr[4] != SOMEIPXF_PROTOCOL_VERSION)
   {
     RetVal = E_SER_WRONG_PROTOCOL_VERSION;
   }
   else if (BufPtr[5] != InterfaceVersion)
   {
     RetVal = E_SER_WRONG_INTERFACE_VERSION;
   }
   else if (BufPtr[6] != SOMEIPXF_MESSAGE_TYPE_REQUEST)
   {
     RetVal = E_SER_WRONG_MESSAGE_TYPE;
   }
   else if (BufPtr[7] != 0x00U)
   {
     RetVal = E_SER_GENERIC_ERROR;
   }
   else
   {
     RetVal = E_OK;

     /* RequestId */
     SOMEIPXF_GET_REQUEST_ID(TransactionHandle->clientId,
                             TransactionHandle->sequenceCounter,
                             BufPtr);
   }

   return RetVal;
}


FUNC(void, SOMEIPXF_CODE) SomeIpXf_Response_SetHeader
(
   P2CONST(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle,
   uint8 InterfaceVersion,
   Std_ReturnType ReturnValue,
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr
)
{
   /* header of client/server communication serialized with
      RequestId,        - TransactionHandle (input)
      ProtocolVersion   - SOMEIPXF_PROTOCOL_VERSION, (generated macro)
      InterfaceVersion  - InterfaceVersion (input, generated to API)
      MessageType       - SOMEIPXF_MESSAGE_TYPE_RESPONSE (SWS_SomeIpXf_00031, Bugzilla RfC 67584)
      ReturnCode        - by ReturnValue initialized,
                          updated if error by SomeIpXf (SWS_SomeIpXf_00115) */

   /* RequestId */
   SOMEIPXF_SET_REQUEST_ID(TransactionHandle->clientId,
                           TransactionHandle->sequenceCounter,
                           BufPtr);

   /* ProtocolVersion (generated) */
   BufPtr[4] = (uint8)SOMEIPXF_PROTOCOL_VERSION;

   /* InterfaceVersion */
   BufPtr[5] = InterfaceVersion;

   /* MessageType is always SOMEIPXF_MESSAGE_TYPE_RESPONSE (SWS_SomeIpXf_00031, Bugzilla RfC 67584) */
   BufPtr[6] = SOMEIPXF_MESSAGE_TYPE_RESPONSE;

   if(ReturnValue >= SOMEIPXF_ERROR_REACTION_THRESHOLD)
   {
     /* set configured MessageType for autonomous error response */
     BufPtr[6] = SOMEIPXF_ERROR_MESSAGE_TYPE;

     /* ReturnCode (SWS_SomeIpXf_00201) */
     BufPtr[7] = (uint8)(ReturnValue - SOMEIPXF_ERROR_REACTION_THRESHOLD);
   }
   else if(ReturnValue != E_OK)
   {
     /* ReturnCode (SWS_SomeIpXf_00107) */
     BufPtr[7] = (uint8)(ReturnValue + SOMEIPXF_ADAPT_APPERROR_RANGE);
   }
   else
   {
     /* ReturnCode (SWS_SomeIpXf_00107) - no application error is configured
      * or set to 0 */
     BufPtr[7] = ReturnValue;
   }

}


FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Response_GetHeader
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   uint8 InterfaceVersion,
   P2VAR(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle,
   P2VAR(Std_ReturnType, AUTOMATIC, SOMEIPXF_APPL_DATA) ReturnValue
)
{
   uint8 RetVal;

   /* header of client/server communication serialized with (check with 0x80 diff)
      RequestId,        - TransactionHandle (output)
      ProtocolVersion   - SOMEIPXF_PROTOCOL_VERSION, (generated macro)
      InterfaceVersion  - InterfaceVersion (input, generated to API)
      MessageType       - SOMEIPXF_MESSAGE_TYPE_RESPONSE (SWS_SomeIpXf_00031, Bugzilla RfC 67584)
      ReturnCode        - by ReturnValue initialized,
                          updated if error by SomeIpXf (SWS_SomeIpXf_00115) */

   if (BufPtr[4] != SOMEIPXF_PROTOCOL_VERSION)
   {
     RetVal = E_SER_WRONG_PROTOCOL_VERSION;
   }
   else if (BufPtr[5] != InterfaceVersion)
   {
     RetVal = E_SER_WRONG_INTERFACE_VERSION;
   }
   else if ( (BufPtr[6] != SOMEIPXF_MESSAGE_TYPE_RESPONSE) &&
             (BufPtr[6] != SOMEIPXF_ERROR_MESSAGE_TYPE) )
   {
     RetVal = E_SER_WRONG_MESSAGE_TYPE;
   }
   else
   {
     RetVal = E_OK;

     /* RequestId */
     SOMEIPXF_GET_REQUEST_ID(TransactionHandle->clientId,
                             TransactionHandle->sequenceCounter,
                             BufPtr);

     /* ReturnCode (passed if ClientServerOperation references an application error) */
     if (ReturnValue != NULL_PTR)
     {
       *ReturnValue = BufPtr[7];
     }
   }

   return RetVal;
}
#endif /* (SOMEIPXF_CS_AVAILABLE == STD_ON) */


#if (SOMEIPXF_ET_AVAILABLE == STD_ON)

FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_ET_Generic
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   uint8 InterfaceVersion
)
{
   uint8 RetVal;

   /* check for executeDespiteDataUnavailability */
   if((Buffer == NULL_PTR) && (BufferLength == 0U))
   {
      RetVal = E_NO_DATA;
   }
   else if ((Buffer == NULL_PTR) ||
            (BufferLength != SOMEIPXF_HEADER_LENGTH))
   {
      /* input parameter check, for defensive programming (no Det interface) */
      RetVal = E_SER_GENERIC_ERROR;
   }
   else
   {
      RetVal = SomeIpXf_ET_GetHeader(Buffer, InterfaceVersion);
   }

   return RetVal;
}
#endif /* (SOMEIPXF_ET_AVAILABLE == STD_ON) */

#if ( (SOMEIPXF_VSA_AVAILABLE == STD_ON) || \
      (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) || \
      (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) )

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Set_LF
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE LFBufferPos,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE LFValue,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE LFSize,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfigPosNWEndianness
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  const uint16 NumOfBytes_u16 = (uint16) LFValue;
  const uint32 NumOfBytes_u32 = (uint32) LFValue;
#if (defined HAS_64BIT_TYPES)
  const uint64 NumOfBytes_u64 = (uint64) LFValue;
#endif /* (defined HAS_64BIT_TYPES) */

  /* set LFValue based on LFSize */
  switch(LFSize)
  {
    case 1U:
    {
      BufPtr[LFBufferPos] = (uint8) ((LFValue) & 0xFFU);
    }
    break;

    case 2U:
    {
      SOMEIPXF_COPY_U16_BUF(ConfigPtr, ConfigPosNWEndianness, BufPtr, LFBufferPos, NumOfBytes_u16);
    }
    break;

    case 4U:
    {
      SOMEIPXF_COPY_U32_BUF(ConfigPtr, ConfigPosNWEndianness, BufPtr, LFBufferPos, NumOfBytes_u32);
    }
    break;

#if (defined HAS_64BIT_TYPES)
    case 8U:
    {
      SOMEIPXF_COPY_U64_BUF(ConfigPtr, ConfigPosNWEndianness, BufPtr, LFBufferPos, NumOfBytes_u64);
    }
    break;
#endif

    /* CHECK: NOPARSE */
    default:
    {
      /* should never been reached */
      RetVal = E_SER_GENERIC_ERROR;
    }
    break;
    /* CHECK: PARSE */
  }

  return RetVal;
}

#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) ||
          (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) ||
          (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */

#if ( (SOMEIPXF_VSA_AVAILABLE == STD_ON) || \
      (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) || \
      (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) || \
      (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || \
      (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) )

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Set_ConfigPosNWEndianness
(
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) ConfigPosNWEndianness,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE LFSize
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  /* set config pos of NW Endianness for LF based on LFSize */
  switch(LFSize)
  {
    case 1U:
    {
      *ConfigPosNWEndianness = SOMEIPXF_CONFIG_START_OFFEST_NW_1B;
    }
    break;

    case 2U:
    {
      *ConfigPosNWEndianness = SOMEIPXF_CONFIG_START_OFFEST_NW_2B;
    }
    break;

    case 4U:
    {
      *ConfigPosNWEndianness = SOMEIPXF_CONFIG_START_OFFEST_NW_4B;
    }
    break;

#if (defined HAS_64BIT_TYPES)
    case 8U:
    {
      *ConfigPosNWEndianness = SOMEIPXF_CONFIG_START_OFFEST_NW_8B;
    }
    break;
#endif /* (defined HAS_64BIT_TYPES) */

    /* CHECK: NOPARSE */
    default:
    {
      /* this should never be reached */
      *ConfigPosNWEndianness = SOMEIPXF_CONFIG_START_OFFEST_NW_1B;
      /* defensive programming mechanism (no Det interface) */
      RetVal = E_SER_GENERIC_ERROR;
    }
    break;
    /* CHECK: PARSE */
  }

  return RetVal;
}

/* Deviation CODEMETRIC-2 */
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_GetLFValues
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BuffPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(uint32, AUTOMATIC, SOMEIPXF_APPL_DATA) LF_ValuePtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE LF_BuffPos,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfigPosNWEndianness,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE LFsize
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  uint16 NumOfBytes_u16 = 0U;
  uint32 NumOfBytes_u32 = 0U;
#if (defined HAS_64BIT_TYPES)
  uint64 NumOfBytes_u64 = 0U;
#endif /* (defined HAS_64BIT_TYPES) */

  /* get value of size independent on the data type */
  switch(LFsize)
  {
     case 1U:
     {
         *LF_ValuePtr = (uint32) BuffPtr[LF_BuffPos];
     }
     break;

     case 2U:
     {
         SOMEIPXF_READ_U16_BUF(ConfigPtr, ConfigPosNWEndianness,
             BuffPtr, LF_BuffPos, NumOfBytes_u16);
         *LF_ValuePtr = (uint32) NumOfBytes_u16;
     }
     break;

     case 4U:
     {
         SOMEIPXF_READ_U32_BUF(ConfigPtr, ConfigPosNWEndianness,
             BuffPtr, LF_BuffPos, NumOfBytes_u32);
         *LF_ValuePtr = NumOfBytes_u32;
     }
     break;

#if (defined HAS_64BIT_TYPES)
     case 8U:
     {
         SOMEIPXF_READ_U64_BUF(ConfigPtr, ConfigPosNWEndianness,
             BuffPtr, LF_BuffPos, NumOfBytes_u64);
         *LF_ValuePtr = (uint32) NumOfBytes_u64;
     }
     break;
#endif /* (defined HAS_64BIT_TYPES) */

     /* CHECK: NOPARSE */
     default:
     {
        /* this should never be reached */
        /* defensive programming mechanism (no Det interface) */
        *LF_ValuePtr = 0U;
        RetVal = E_SER_GENERIC_ERROR;
     }
     break;
     /* CHECK: PARSE */
  }

  return RetVal;
}

#endif /* ( (SOMEIPXF_VSA_AVAILABLE == STD_ON) || \
            (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) || \
            (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) || \
            (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || \
            (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) ) */

/*==================[internal function definitions]=========================*/

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_ConfigArray
(
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  uint8 RetVal = E_SER_GENERIC_ERROR;

  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfigArraySize = ConfigPtr[1U];

  /* config array size is smaller then SOMEIPXF_DATA_ELEMENT_STOP value */
  boolean invalid_ConfigArraySize = ConfigArraySize >= SOMEIPXF_DATA_ELEMENT_STOP;
  /* SOMEIPXF_DATA_ELEMENT_STOP is not on ConfigArraySize pos */
  boolean invalid_ConfigArrayValue = ConfigPtr[(ConfigArraySize - 1U)] != SOMEIPXF_DATA_ELEMENT_STOP;

/* CHECK: NOPARSE */
  if( (TRUE == invalid_ConfigArraySize) || (TRUE == invalid_ConfigArrayValue) )
  {
    RetVal = E_SER_GENERIC_ERROR;
  }
  else
/* CHECK: PARSE */
  {
    RetVal = E_OK;
  }

  /* set CfgElemHandled over config array size tag */
  DeSerContextPtr->CfgElemHandled = DeSerContextPtr->CfgElemHandled + SOMEIPXF_CONFIG_START_OFFEST;

  return RetVal;
}

/*----------- S e r i a l i z a t i o n   o f   S O M E / I P --------------*/

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerializeBasicType
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufPos = 0U; /* indicates position of serialized data */
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfPos = 2U; /* neglects initial SOMEIPXF_BASIC_DATA_TYPE_START and
   SOMEIPXF_BASIC_DATA_TYPE_CONFIG */

   do{
      BufPtr[BufPos] = DataBytePtr[ConfigPtr[ConfPos] + DeSerContextPtr->DataByteOffset];
      BufPos++;
      ConfPos++;

   /* end of basis data type serialization reached */
   }while (ConfigPtr[ConfPos] != SOMEIPXF_BASIC_DATA_TYPE_STOP);

   /* updates the number of bytes serialized and the number of configuation elements handled */
   DeSerContextPtr->BytesCopied += BufPos;
   DeSerContextPtr->CfgElemHandled += ConfPos;

   /* returns defensive programming problems */
   return E_OK;
}

#if (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON)

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SetUpStructWithLfContext
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(StructWithLf_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) StructWithLfContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  /* check if nested level should be increased */
  if(TRUE == StructWithLfContextPtr->StructWithLf_Root)
  {
    /* for root keeps nested level 0 and
     * set StructWithLf_Root to FALSE for further nested structs */
    StructWithLfContextPtr->StructWithLf_Root = FALSE;
  }
  else
  {
    /* CHECK: NOPARSE */
    if( ( (StructWithLfContextPtr->StructWithLf_current_NESTED_LEVEL + 1U) >= SOMEIPXF_MAX_NESTED_STRUCT_WITH_LF_DEPTH ) )
    {
      RetVal = E_SER_GENERIC_ERROR;
    }
    else
    /* CHECK: PARSE */
    {
      StructWithLfContextPtr->StructWithLf_current_NESTED_LEVEL++;
    }
  }

  /* CHECK: NOPARSE */
  if(RetVal == E_OK)
  /* CHECK: PARSE */
  {
    boolean con_valid_StructWithLf_De_Seral = FALSE;

    /* set StructWithLf context within StructWithLf_InfoType */
    SOMEIPXF_SET_StructWithLfContext_StructWithLf_InfoType(LF_BufPos, DeSerContextPtr->BytesCopied);

#if ((SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON))
    if( (SOMEIPXF_DE_SER == StructWithLfContextPtr->StructWithLf_De_Seral) &&
        /* different LFSize based different wire type */
        (DeSerContextPtr->TLV_LFSize_WireType567 != 0U) )
    {
      /* set diffrent LFsize based on diffrent wire type then configured */
      SOMEIPXF_SET_StructWithLfContext_StructWithLf_InfoType(LFsize, DeSerContextPtr->TLV_LFSize_WireType567);
      /* Reset the LF size */
      DeSerContextPtr->TLV_LFSize_WireType567 = 0U;
    }
    else
#endif /* ((SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)) */
    {
      /* set LFsize since only a received TLV member with a different wire type then configured could change
       * the LFSize for the de-serialization */
      SOMEIPXF_SET_StructWithLfContext_StructWithLf_InfoType(LFsize, ConfigPtr[1U + DeSerContextPtr->CfgElemHandled]);
    }

    /* set config pos of NW Endianness for LF based on LFSize */
    RetVal = SomeIpXf_Set_ConfigPosNWEndianness(SOMEIPXF_GET_ADDR_StructWithLf_InfoType(ConfigPosNWEndianness),
                                                SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(LFsize));

    con_valid_StructWithLf_De_Seral = ((RetVal == E_OK) && (SOMEIPXF_DE_SER == StructWithLfContextPtr->StructWithLf_De_Seral));

    if(TRUE == con_valid_StructWithLf_De_Seral)
    {
      /* RetVal is valid and de-seralization is enabled */

      if( BufferLength <= ((uint32) SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(LF_BufPos) +
          SOMEIPXF_HEADER_LENGTH + (uint32) SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(LFsize)) )
      {
        /* bufferlength is less then expected (LF_BufPos + LFsize) */
        RetVal = E_SER_MALFORMED_MESSAGE;
      }
      else
      {
        /* get LF value */
        RetVal = SomeIpXf_GetLFValues(
            BufPtr,
            ConfigPtr,
            SOMEIPXF_GET_ADDR_StructWithLf_InfoType(LF_Value),
            SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(LF_BufPos),
            SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(ConfigPosNWEndianness),
            SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(LFsize)
            );

        /* CHECK: NOPARSE */
        if(RetVal == E_OK)
        /* CHECK: PARSE */
        {
          if( SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(LF_Value) >
              (BufferLength - (uint32) SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(LF_BufPos) -
              SOMEIPXF_HEADER_LENGTH - (uint32) SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(LFsize)) )
          {
            /* LF value is larger then expected (bufferlength) */
            RetVal = E_SER_MALFORMED_MESSAGE;
          }
        }
      }

    }

    /* shift buffer pos for LF */
    DeSerContextPtr->BytesCopied = DeSerContextPtr->BytesCopied
        + SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(LFsize);

    /* set config pos to member entry */
    DeSerContextPtr->CfgElemHandled = DeSerContextPtr->CfgElemHandled + 1U;
  }

  /* returns defensive programming problems */
  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerHandlingStructWithLfContext
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(StructWithLf_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) StructWithLfContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE LF_BufPos = SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(LF_BufPos);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE LFsize = SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(LFsize);

  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfigPosNWEndianness =
      SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(ConfigPosNWEndianness);

  /* calculate the current element byte length based from the LF position */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE Struct_ByteLength = (DeSerContextPtr->BytesCopied) - (LF_BufPos + LFsize);

  /* set LF value */
  RetVal = SomeIpXf_Set_LF(BufPtr, ConfigPtr, LF_BufPos, Struct_ByteLength,
      LFsize, ConfigPosNWEndianness);

  /* clear StructWithLf_Info_LEVEL on current nested level */
  SOMEIPXF_CLEAR_StructWithLfContext_StructWithLf_InfoType(StructWithLfContextPtr->StructWithLf_current_NESTED_LEVEL);

  /* check if nested level needs to be decreased */
  if(StructWithLfContextPtr->StructWithLf_current_NESTED_LEVEL > 0U)
  {
    StructWithLfContextPtr->StructWithLf_current_NESTED_LEVEL--;
  }
  else
  {
    StructWithLfContextPtr->StructWithLf_Root = TRUE;
  }

  /* returns defensive programming problems */
  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserHandlingStructWithLfContext
(
   P2VAR(StructWithLf_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) StructWithLfContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;
  const uint32 LF_BufPos = (uint32) SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(LF_BufPos);
  const uint32 LFsize = (uint32) SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(LFsize);
  const uint32 LF_Value = SOMEIPXF_GET_StructWithLfContext_StructWithLf_InfoType(LF_Value);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied = DeSerContextPtr->BytesCopied;

  const uint32 NextElementBufferPos = LF_Value + LF_BufPos + LFsize;

  if(NextElementBufferPos > BytesCopied)
  {
    const uint32 NumSkipBytes = (uint32) (NextElementBufferPos - BytesCopied);

    /* skipping behavior */
    DeSerContextPtr->BytesCopied = (SOMEIPXF_XFRMBUFFERLENGTH_TYPE) (BytesCopied + NumSkipBytes);
  }
  /* check if less data than expected are handed over for deserialization */
  else if(NextElementBufferPos < BytesCopied)
  {
    RetVal = E_SER_MALFORMED_MESSAGE;
  }
  else
  {
    /* empty else */
  }

  /* clear StructWithLf_Info_LEVEL on current nested level */
  SOMEIPXF_CLEAR_StructWithLfContext_StructWithLf_InfoType(StructWithLfContextPtr->StructWithLf_current_NESTED_LEVEL);

  /* check if VSA nested level needs to be decreased */
  if(StructWithLfContextPtr->StructWithLf_current_NESTED_LEVEL > 0U)
  {
    StructWithLfContextPtr->StructWithLf_current_NESTED_LEVEL--;
  }
  else
  {
    StructWithLfContextPtr->StructWithLf_Root = TRUE;
  }

  /* returns defensive programming problems */
  return RetVal;
}

#endif /* (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_ON) */

#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerializeFSAOfBasicType
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufPos = 0U; /* position of serialized data */
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfPos = 0U; /* position wthin array element */
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfPosOffset = 0U; /* offset position of array element within array */
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE NumOfArrayElem = ConfigPtr[1]; /* overall number of array elements */
   const SOMEIPXF_XFRMBUFFERLENGTH_TYPE SizeOfElem = ConfigPtr[2]; /* Size of an array element */

   do{
      ConfPos = 5U; /* reset for array handling, the first data element position */

      do{
         BufPtr[BufPos] = DataBytePtr[ConfigPtr[ConfPos] + ConfPosOffset + DeSerContextPtr->DataByteOffset];
         BufPos++;
         ConfPos++;

      /* end of array element serialization reached */
      }while (ConfigPtr[ConfPos] != SOMEIPXF_BASIC_DATA_TYPE_STOP);

      ConfPosOffset += SizeOfElem;
      NumOfArrayElem--;

   /* end of array serialization reached */
   }while (NumOfArrayElem > 0U);


   /* update number of bytes serialized and number of configuation elements handled */
   /* handle end tag SOMEIPXF_FSA_OF_BASIC_TYPE_STOP */
   ConfPos += 1U;  /* add SOMEIPXF_FSA_OF_BASIC_TYPE_STOP */
   DeSerContextPtr->BytesCopied += BufPos;
   DeSerContextPtr->CfgElemHandled += ConfPos;

   /* returns defensive programming problems */
   return E_OK;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SetUpFSAContext
(
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = 0U;

  /* check if nested level should be increased */
  if( (TRUE == FSAContextPtr->FSA_Root) )
  {
    /* for root keeps nested level 0 and
     * set FSA_Root to FALSE for further nested FSAs */
    FSAContextPtr->FSA_Root = FALSE;
  }
  else
  {
    /* CHECK: NOPARSE */
    if( ( (FSAContextPtr->FSA_current_NESTED_LEVEL + 1U) >= SOMEIPXF_MAX_NESTED_ARRAY_DEPTH ) )
    {
      RetVal = E_SER_GENERIC_ERROR;
    }
    else
    /* CHECK: PARSE */
    {
      FSAContextPtr->FSA_current_NESTED_LEVEL++;
    }
  }

  /* CHECK: NOPARSE */
  if(RetVal == E_OK)
  /* CHECK: PARSE */
  {
    /* set FSA context within FSA_InfoType */
    SOMEIPXF_SET_FSAContext_FSA_InfoType(FSAWithLf, FALSE);
    SOMEIPXF_SET_FSAContext_FSA_InfoType(FSA_ConfigPosRemElement, (DeSerContextPtr->CfgElemHandled + 3U));
    /* FSA with NO LF gets optimized by MCG to one dimension only */
    SOMEIPXF_SET_FSAContext_FSA_InfoType(FSA_current_DIM_LEVEL, 0U);
    SOMEIPXF_SET_FSAContext_FSA_InfoType(FSA_max_config_DIM, 1U);
    SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_curr_element, 0U);
    SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_max_element, ConfigPtr[CfgElemHandled + 1U]);
    SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_element_sizeof, ConfigPtr[CfgElemHandled + 2U]);
    SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_ArrayDataByteOffset, DeSerContextPtr->DataByteOffset);

    /* set config pos for FSA element */
    DeSerContextPtr->CfgElemHandled = DeSerContextPtr->CfgElemHandled + 3U;
  }

  /* returns defensive programming problems */
  return RetVal;
}

#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SetUpExtFSAContext
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;

  /* check if nested level should be increased */
  if( (TRUE == FSAContextPtr->FSA_Root) )
  {
    /* for root keeps nested level 0 and
     * set FSA_Root to FALSE for further nested FSAs */
    FSAContextPtr->FSA_Root = FALSE;
  }
  else
  {
    /* CHECK: NOPARSE */
    if( ( (FSAContextPtr->FSA_current_NESTED_LEVEL + 1U) >= SOMEIPXF_MAX_NESTED_ARRAY_DEPTH ) )
    {
      RetVal = E_SER_GENERIC_ERROR;
    }
    else
    /* CHECK: PARSE */
    {
      FSAContextPtr->FSA_current_NESTED_LEVEL++;
    }
  }

  /* CHECK: NOPARSE */
  if(RetVal == E_OK)
  /* CHECK: PARSE */
  {
    uint8 dim_idx;
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE FSA_LF_BuffPosOffset = DeSerContextPtr->BytesCopied + SOMEIPXF_HEADER_LENGTH;
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE FSA_LF_BuffPos = 0U;

    /* set FSA context within FSA_InfoType */
    SOMEIPXF_SET_FSAContext_FSA_InfoType(FSAWithLf, TRUE);
    SOMEIPXF_SET_FSAContext_FSA_InfoType(FSA_max_config_DIM, (uint8) ConfigPtr[CfgElemHandled + 1U]);

#if ((SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON))
    if( (SOMEIPXF_DE_SER == FSAContextPtr->FSA_De_Seral) &&
        /* different LFSize based different wire type */
        (DeSerContextPtr->TLV_LFSize_WireType567 != 0U) )
    {
      /* set diffrent LFsize based on diffrent wire type then configured */
      SOMEIPXF_SET_FSAContext_FSA_InfoType(FSA_LFsize, DeSerContextPtr->TLV_LFSize_WireType567);
      /* Reset the LF size */
      DeSerContextPtr->TLV_LFSize_WireType567 = 0U;
    }
    else
#endif /* ((SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)) */
    {
      /* set LFsize since only a received TLV member with a different wire type then configured could change
       * the LFSize for the de-serialization */
      SOMEIPXF_SET_FSAContext_FSA_InfoType(FSA_LFsize, ConfigPtr[CfgElemHandled + 2U]);
    }

    /* set config pos of NW Endianness for LF based on LFSize */
    RetVal = SomeIpXf_Set_ConfigPosNWEndianness(SOMEIPXF_GET_ADDR_FSAContext_FSA_InfoType(FSA_ConfigPosNWEndianness),
                                                SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_LFsize));

    /* set CfgElemHandled to Dim Config offset pos */
    CfgElemHandled = CfgElemHandled + SOMEIPXF_FSA_INFO_LEVEL_STATIC_CONFIG_OFFSET;

    /* ensure that configured dimensions level can be hold by FSA context */
    /* CHECK: NOPARSE */
    if(SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_max_config_DIM) <= SOMEIPXF_FSA_MAX_DIM)
    /* CHECK: PARSE */
    {
      /* set VSA DIM context within VSA_Context_DIM_LEVEL */
      for(dim_idx=0U; dim_idx<SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_max_config_DIM); dim_idx++)
      {
        boolean con_valid_FSA_De_Seral = FALSE;
        /* set current dimension level */
        SOMEIPXF_SET_FSAContext_FSA_InfoType(FSA_current_DIM_LEVEL, dim_idx);
        /* set initiated VSA_ArrayDataByteOffset for new VSA context */
        SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_ArrayDataByteOffset, DeSerContextPtr->DataByteOffset);
        /* set max elments values of data element per dim */
        SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_max_element, ConfigPtr[CfgElemHandled]);
        /* set sizeof element per dim */
        SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_element_sizeof, ConfigPtr[CfgElemHandled + 1U]);
        /* increase config pos offset per dim due to static config pos */
        CfgElemHandled = CfgElemHandled + 2U;

        /* set byte buffer pos of LF per dim */
        SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_LF_BufPos, FSA_LF_BuffPos + FSA_LF_BuffPosOffset);
        /* increase byte buffer pos of LF per dim due to static LF size */
        FSA_LF_BuffPos = FSA_LF_BuffPos + SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_LFsize);
        /* set initial element index to be handled */
        SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_curr_element, 0U);

        con_valid_FSA_De_Seral = ((RetVal == E_OK) && (SOMEIPXF_DE_SER == FSAContextPtr->FSA_De_Seral));

        if(TRUE == con_valid_FSA_De_Seral)
        {
          if( BufferLength <= ((uint32) SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_LF_BufPos) +
              (uint32) SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_LFsize)) )
          {
            /* bufferlength is less then expected (LF_BufPos + LFsize) */
            RetVal |= E_SER_MALFORMED_MESSAGE;
          }
          else
          {
            /* get LF value */
            RetVal |= SomeIpXf_GetLFValues(
                BufPtr,
                ConfigPtr,
                SOMEIPXF_GET_ADDR_FSAContext_DIM_LEVEL(FSA_LF_Value),
                (SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_LF_BufPos) - SOMEIPXF_HEADER_LENGTH),
                SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_ConfigPosNWEndianness),
                SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_LFsize)
            );

            /* CHECK: NOPARSE */
            if(RetVal == E_OK)
            /* CHECK: PARSE */
            {
              if( SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_LF_Value) >
                  (BufferLength - (uint32) SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_LF_BufPos) -
                  (uint32) SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_LFsize)) )
              {
                /* LF value is larger then expected (bufferlength) */
                RetVal |= E_SER_MALFORMED_MESSAGE;
              }
            }
          }

        }

        /* shift buffer pos for LF */
        DeSerContextPtr->BytesCopied = DeSerContextPtr->BytesCopied + SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_LFsize);
      }

      /* set config pos for FSA element */
      DeSerContextPtr->CfgElemHandled = CfgElemHandled;

      SOMEIPXF_SET_FSAContext_FSA_InfoType(FSA_ConfigPosRemElement, DeSerContextPtr->CfgElemHandled);
    }

  } /* if(RetVal == E_OK) */

  /* returns defensive programming problems */
  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerializeExtFSAOfBasicType
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr
)
{
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufPos = 0U; /* position of serialized data */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfPos = 0U; /* position wthin array element */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfPosOffset = 0U; /* offset position of array element within array */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE NumOfArrayElem = SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_max_element); /* overall number of array elements */
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE SizeOfElem = SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_element_sizeof); /* Size of an array element */

  /* set NumOfArrayElem reduced by one to current elements due to increase in element check point */
  SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_curr_element,(NumOfArrayElem - 1U));

  do{
     ConfPos = 1U; /* reset for array handling, the first data element position */

     do{
        BufPtr[BufPos] = DataBytePtr[ConfigPtr[ConfPos] + ConfPosOffset + DeSerContextPtr->DataByteOffset];
        BufPos++;
        ConfPos++;

     /* end of array element serialization reached */
     }while (ConfigPtr[ConfPos] != SOMEIPXF_FSA_OF_BASIC_DATA_TYPE_WITH_LF_STOP);

     ConfPosOffset += SizeOfElem;
     NumOfArrayElem--;

  /* end of array serialization reached */
  }while (NumOfArrayElem > 0U);

  /* update number of bytes serialized and number of configuation elements handled */
  DeSerContextPtr->BytesCopied += BufPos;
  DeSerContextPtr->CfgElemHandled += ConfPos;

  /* returns defensive programming problems */
  return E_OK;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserializeExtFSAOfBasicType
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufPos = 0U; /* position of serialized data */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfPos = 0U; /* position of data byte within array element */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfPosOffset = 0U; /* position of array element within array */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE NumOfArrayElem = SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_max_element); /* number of elements the array provides */
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE SizeOfElem = SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_element_sizeof); /* size of a single array element */

  /* FSA of BDT size check required due to previous possible relocation of the buffer pos by skipping behviour of an
   * ext. DT, VSA or TLV */
  if((DeSerContextPtr->BytesCopied + SOMEIPXF_HEADER_LENGTH + (NumOfArrayElem * SizeOfElem)) > BufferLength)
  {
    /* LF skipping value was larger then expected (bufferlength) */
    RetVal = E_SER_MALFORMED_MESSAGE;
  }
  else
  {
    /* set NumOfArrayElem reduced by one to current elements due to increase in element check point */
    SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_curr_element,(NumOfArrayElem - 1U));

    do{
       ConfPos = 1U; /* reset for array handling, the first data element position */

       do{
          DataBytePtr[ConfigPtr[ConfPos] + ConfPosOffset + DeSerContextPtr->DataByteOffset] = BufPtr[BufPos];
          BufPos++;
          ConfPos++;

       /* end of array element serialization reached */
       }while (ConfigPtr[ConfPos] != SOMEIPXF_FSA_OF_BASIC_DATA_TYPE_WITH_LF_STOP);

       ConfPosOffset += SizeOfElem;
       NumOfArrayElem--;

    /* end of array serialization reached */
    } while (NumOfArrayElem > 0U);

    /* update number of bytes serialized and number of configuation elements handled */
    DeSerContextPtr->BytesCopied += BufPos;
    DeSerContextPtr->CfgElemHandled += ConfPos;
  }

  return RetVal;
}

#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_FSA_elements_ser
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType FSA_Dim
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;

#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE FSA_LF_BufPos = SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_LF_BufPos);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE FSA_LFsize = SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_LFsize);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE FSA_ConfigPosNWEndianness = SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_ConfigPosNWEndianness);

  /* calculate the current element byte length based from the LF position */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE Element_ByteLength = (DeSerContextPtr->BytesCopied) -
      ((FSA_LF_BufPos - SOMEIPXF_HEADER_LENGTH) + FSA_LFsize);
#else
  SOMEIPXF_PARAM_UNUSED(ConfigPtr);
  SOMEIPXF_PARAM_UNUSED(BufPtr);
#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */
#if (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_OFF)
  SOMEIPXF_PARAM_UNUSED(FSA_Dim);
#endif /* SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_OFF) */

#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)
    /* check if FSA with LF */
    if(SOMEIPXF_GET_FSAContext_FSA_InfoType(FSAWithLf) == TRUE)
    {
      RetVal = SomeIpXf_Set_LF(BufPtr, ConfigPtr, (FSA_LF_BufPos - SOMEIPXF_HEADER_LENGTH), Element_ByteLength,
          FSA_LFsize, FSA_ConfigPosNWEndianness);
    }
#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */

    /* clear ArrayDataByteOffset of the current nested level */
    SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_ArrayDataByteOffset, 0U);

#if (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON)
    if(FSA_Dim != SOMIPXF_OUTER_MOST_DIM)
    {
      /* set data byte offset to array byte offset */
      DeSerContextPtr->DataByteOffset = 0U;

      /* decrease dimension */
      SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_current_DIM_LEVEL)--;

      /* set CfgElemHandled to check point again */
      CfgElemHandled = DeSerContextPtr->CfgElemHandled - 1U;
    }
    else
#endif /* (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON) */
    {
      /* clear FSA_Info_LEVEL entry on current nested level */
      /* Deviation TASKING-1 */
      SOMEIPXF_CLEAR_FSAContext_FSA_InfoType(FSAContextPtr->FSA_current_NESTED_LEVEL);

      /* all FSA outer most elements serialized, decrease nested level */
      if(FSAContextPtr->FSA_current_NESTED_LEVEL > 0U)
      {
        FSAContextPtr->FSA_current_NESTED_LEVEL--;
        DeSerContextPtr->DataByteOffset = SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_ArrayDataByteOffset);
      }
      else
      {
        /* set data byte offset to array byte offset */
        DeSerContextPtr->DataByteOffset = 0U;
        /* enable FSA root entry */
        FSAContextPtr->FSA_Root = TRUE;
      }
    }

    DeSerContextPtr->CfgElemHandled = CfgElemHandled;

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_FSA_elements_rem
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType FSA_Dim
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE FSA_ArrayDataByteOffset =
      SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_ArrayDataByteOffset);

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE ArrayDataByteOffsetofNextElem = FSA_ArrayDataByteOffset +
      SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_element_sizeof);

#if ( (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_OFF) || (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_OFF) )
  SOMEIPXF_PARAM_UNUSED(ConfigPtr);
  SOMEIPXF_PARAM_UNUSED(BufPtr);
  SOMEIPXF_PARAM_UNUSED(FSA_Dim);
#endif /* ( (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_OFF) || (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_OFF) ) */

  /* set ArrayDataByteOffset to next element */
  SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_ArrayDataByteOffset, ArrayDataByteOffsetofNextElem);
  /* set data byte offset to array byte offset */
  DeSerContextPtr->DataByteOffset = ArrayDataByteOffsetofNextElem;

#if (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON)
  {
    const uint8 Current_DIM_LEVEL = SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_current_DIM_LEVEL);
    const uint8 Max_DIM_LEVEL = SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_max_config_DIM) - 1U;
    uint32 dim_idx = 0U;

#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)
    const SOMEIPXF_XFRMBUFFERLENGTH_TYPE FSA_LF_BufPos = SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_LF_BufPos);
    const SOMEIPXF_XFRMBUFFERLENGTH_TYPE FSA_LFsize = SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_LFsize);

    /* set LF for intermediate dimension (pre-condition in SomeIpXf_SerHandlingFSAContext ensures
     * multi dim FSA only with FSAWithLf set to TRUE) */
    if(FSA_Dim == SOMIPXF_INTERMEDIATE_DIM)
    {
      const SOMEIPXF_XFRMBUFFERLENGTH_TYPE FSA_ConfigPosNWEndianness = SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_ConfigPosNWEndianness);

      /* calculate the current element byte length based from the LF position */
      SOMEIPXF_XFRMBUFFERLENGTH_TYPE Element_ByteLength = (DeSerContextPtr->BytesCopied) -
          ((FSA_LF_BufPos - SOMEIPXF_HEADER_LENGTH) + FSA_LFsize);

      RetVal = SomeIpXf_Set_LF(BufPtr, ConfigPtr, (FSA_LF_BufPos - SOMEIPXF_HEADER_LENGTH), Element_ByteLength,
          FSA_LFsize, FSA_ConfigPosNWEndianness);
    }
#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */

    /* loop over inner dimensions */
    for(dim_idx = Current_DIM_LEVEL;dim_idx < Max_DIM_LEVEL; dim_idx++)
    {
      /* increase dimension and reset FSA_curr_element */
      SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_current_DIM_LEVEL)++;
      SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_curr_element, 0U);

      /* set array byte offset to next inner dim elements */
      SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_ArrayDataByteOffset, ArrayDataByteOffsetofNextElem);
      /* set data byte offset to array byte offset */
      DeSerContextPtr->DataByteOffset = ArrayDataByteOffsetofNextElem;

#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)
      /* no check if FSA with LF required since FSA with NO LF gets optimized
       * by MCG to one dimension only */

      /* save LF buffer pos of next dim */
      SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_LF_BufPos, DeSerContextPtr->BytesCopied + SOMEIPXF_HEADER_LENGTH);
      /* set buffer pos to next DIM element */
      DeSerContextPtr->BytesCopied = DeSerContextPtr->BytesCopied + FSA_LFsize;
#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */
    }

  }
#endif /* (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON) */

  /* set CfgElemHandled back to FSA element */
  SOMEIPXF_SET_CONFIGPOS(RetVal, CfgElemHandled,
      SOMEIPXF_FSA_ELEMENT_ENTER, (SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_ConfigPosRemElement)));

  DeSerContextPtr->CfgElemHandled = CfgElemHandled;

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerHandlingFSADim
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType FSA_Dim
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE FSA_curr_element = SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_curr_element);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE FSA_max_element = SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_max_element);

  /* FSA element got serialized */
  FSA_curr_element++;
  SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_curr_element, FSA_curr_element);

  /* CHECK: NOPARSE */
  if(FSA_curr_element > FSA_max_element)
  {
    RetVal = E_SER_GENERIC_ERROR;
  }
  /* CHECK: PARSE */
  else if(FSA_curr_element == FSA_max_element)
  {
    /* all elements serialized */
    RetVal = SomeIpXf_FSA_elements_ser(
        BufPtr,
        ConfigPtr,
        FSAContextPtr,
        DeSerContextPtr,
        FSA_Dim);
  }
  else /* (FSA_curr_element < FSA_max_element) */
  {
    /* remaining elements to be serialized */
    RetVal = SomeIpXf_FSA_elements_rem(
        BufPtr,
        ConfigPtr,
        FSAContextPtr,
        DeSerContextPtr,
        FSA_Dim);
  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerHandlingFSAContext
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  const uint8 Current_DIM_LEVEL = SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_current_DIM_LEVEL);
#if (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON)
  const uint8 Max_DIM_LEVEL = SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_max_config_DIM) - 1U;
  boolean con_invalid_dim_value = (Current_DIM_LEVEL > Max_DIM_LEVEL);
  /* hint: multi dim FSA without LFs optimized via config (only outer most dim calls used) */
  boolean con_invalid_dim_FSAWithLf = (
      /* current dim is not the outer most (multi dim FSA) */
      (Current_DIM_LEVEL != 0U)  &&
      /* no FSA LF value is enabled for the multi dim FSA */
      (SOMEIPXF_GET_FSAContext_FSA_InfoType(FSAWithLf) == FALSE) );
#else
  boolean con_invalid_dim_value = (Current_DIM_LEVEL != 0U);
  boolean con_invalid_dim_FSAWithLf = FALSE;
#endif /* (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON) */

  /* CHECK: NOPARSE */
  if( (TRUE == con_invalid_dim_value) || (TRUE == con_invalid_dim_FSAWithLf))
  {
    /* invalid dim values */

    /* clear FSA_Info_LEVEL entry on current nested level */
    /* Deviation TASKING-1 */
    SOMEIPXF_CLEAR_FSAContext_FSA_InfoType(FSAContextPtr->FSA_current_NESTED_LEVEL);

    if(FSAContextPtr->FSA_current_NESTED_LEVEL > 0U)
    {
      FSAContextPtr->FSA_current_NESTED_LEVEL--;
    }
    else
    {
      /* enable FSA root entry */
      FSAContextPtr->FSA_Root = TRUE;
    }

    RetVal = E_SER_GENERIC_ERROR;
  }
  else
  /* CHECK: PARSE */
  {
    /* handled elements of outer most dimension */
    if(Current_DIM_LEVEL == 0U)
    {
      RetVal = SomeIpXf_SerHandlingFSADim(
          BufPtr,
          ConfigPtr,
          FSAContextPtr,
          DeSerContextPtr,
          SOMIPXF_OUTER_MOST_DIM);
    }
#if (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON)
    /* handled elements of intermediate dimension */
    else if(Current_DIM_LEVEL < Max_DIM_LEVEL)
    {
      RetVal = SomeIpXf_SerHandlingFSADim(
          BufPtr,
          ConfigPtr,
          FSAContextPtr,
          DeSerContextPtr,
          SOMIPXF_INTERMEDIATE_DIM);
    }
    /* handled elements of inner most dimension */
    else /* ( (Current_DIM_LEVEL == Max_DIM_LEVEL) && (0U != Max_DIM_LEVEL) ) */
    {
      RetVal = SomeIpXf_SerHandlingFSADim(
          BufPtr,
          ConfigPtr,
          FSAContextPtr,
          DeSerContextPtr,
          SOMIPXF_INNER_MOST_DIM);
    }
#endif /* (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON) */

  }

  return RetVal;
}

#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserFSADimSkippingBytes
(
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;
  const uint32 LF_BufPos = (uint32) SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_LF_BufPos);
  const uint32 LFsize = (uint32) SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_LFsize);
  const uint32 LF_Value = SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_LF_Value);

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied = DeSerContextPtr->BytesCopied + SOMEIPXF_HEADER_LENGTH;

  const uint32 NextElementBufferPos = LF_Value + LF_BufPos + LFsize;

  if((NextElementBufferPos) > (BytesCopied))
  {
    /* LF_Value is greater than de-seralized bytes, apply skipping
     * of excess bytes */
    const uint32 NumSkipBytes = (uint32) ((NextElementBufferPos) - BytesCopied);
    /* skipping behavior */
    DeSerContextPtr->BytesCopied = (SOMEIPXF_XFRMBUFFERLENGTH_TYPE) (DeSerContextPtr->BytesCopied + NumSkipBytes);
  }
  /* check if less data than expected are handed over for deserialization */
  else if((NextElementBufferPos) < (BytesCopied))
  {
    RetVal = E_SER_MALFORMED_MESSAGE;
  }
  else
  {
    /* empty else */
  }

  /* returns defensive programming problems */
  return RetVal;
}
#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_FSA_elements_deser
(
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType FSA_Dim
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;

#if (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_OFF)
  SOMEIPXF_PARAM_UNUSED(FSA_Dim);
#endif /* (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_OFF) */

#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)
    if(SOMEIPXF_GET_FSAContext_FSA_InfoType(FSAWithLf) == TRUE)
    {
      /* possible skipping of excess bytes */
      RetVal = SomeIpXf_DeserFSADimSkippingBytes(
          DeSerContextPtr,
          FSAContextPtr
          );
    }
#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */

    /* clear ArrayDataByteOffset of the current nested level */
    SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_ArrayDataByteOffset, 0U);

#if (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON)
    if(FSA_Dim != SOMIPXF_OUTER_MOST_DIM)
    {
      /* set data byte offset to array byte offset */
      DeSerContextPtr->DataByteOffset = 0U;

      /* decrease dimension */
      SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_current_DIM_LEVEL)--;

      /* set CfgElemHandled to check point again */
      CfgElemHandled = DeSerContextPtr->CfgElemHandled - 1U;
    }
    else
#endif /* (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON) */
    {
      /* clear FSA_Info_LEVEL entry on current nested level */
      /* Deviation TASKING-1 */
      SOMEIPXF_CLEAR_FSAContext_FSA_InfoType(FSAContextPtr->FSA_current_NESTED_LEVEL);

      /* all FSA outer most elements serialized, decrease nested level */
      if(FSAContextPtr->FSA_current_NESTED_LEVEL > 0U)
      {
        FSAContextPtr->FSA_current_NESTED_LEVEL--;
        DeSerContextPtr->DataByteOffset = SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_ArrayDataByteOffset);
      }
      else
      {
        /* set data byte offset to array byte offset */
        DeSerContextPtr->DataByteOffset = 0U;
        /* enable FSA root entry */
        FSAContextPtr->FSA_Root = TRUE;
      }
    }

    DeSerContextPtr->CfgElemHandled = CfgElemHandled;

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_FSA_elements_deser_rem
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE FSA_ArrayDataByteOffset =
      SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_ArrayDataByteOffset);

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE ArrayDataByteOffsetofNextElem = FSA_ArrayDataByteOffset +
      SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_element_sizeof);

#if ( (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_OFF) || (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_OFF) )
  SOMEIPXF_PARAM_UNUSED(ConfigPtr);
  SOMEIPXF_PARAM_UNUSED(BufPtr);
  SOMEIPXF_PARAM_UNUSED(BufferLength);
#endif /* ( (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_OFF) || (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_OFF) ) */

  /* set ArrayDataByteOffset to next element */
  SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_ArrayDataByteOffset, ArrayDataByteOffsetofNextElem);
  /* set data byte offset to array byte offset */
  DeSerContextPtr->DataByteOffset = ArrayDataByteOffsetofNextElem;

#if (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON)
  {
    const uint8 Current_DIM_LEVEL = SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_current_DIM_LEVEL);
    const uint8 Max_DIM_LEVEL = SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_max_config_DIM) - 1U;
    uint32 dim_idx = 0U;

    /* loop over inner dimensions */
    for(dim_idx = Current_DIM_LEVEL;dim_idx < Max_DIM_LEVEL; dim_idx++)
    {
      /* increase dimension and reset FSA_curr_element */
      SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_current_DIM_LEVEL)++;
      SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_curr_element, 0U);

      /* set array byte offset to next inner dim elements */
      SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_ArrayDataByteOffset, ArrayDataByteOffsetofNextElem);
      /* set data byte offset to array byte offset */
      DeSerContextPtr->DataByteOffset = ArrayDataByteOffsetofNextElem;

#if (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON)
      /* no check if FSA with LF required since FSA with NO LF gets optimized
       * by MCG to one dimension only */

      /* save LF buffer pos of next dim */
      SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_LF_BufPos, DeSerContextPtr->BytesCopied + SOMEIPXF_HEADER_LENGTH);

      if( BufferLength <= ((uint32) SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_LF_BufPos) +
          (uint32) SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_LFsize)) )
      {
        /* bufferlength is less then expected (LF_BufPos + LFsize) */
        RetVal |= E_SER_MALFORMED_MESSAGE;
      }
      else
      {
        /* get LF value and check if valid based on not larger then expected */
        RetVal |= SomeIpXf_GetLFValues(
            BufPtr,
            ConfigPtr,
            SOMEIPXF_GET_ADDR_FSAContext_DIM_LEVEL(FSA_LF_Value),
            (SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_LF_BufPos) - SOMEIPXF_HEADER_LENGTH),
            SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_ConfigPosNWEndianness),
            SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_LFsize)
        );

        /* CHECK: NOPARSE */
        if(RetVal == E_OK)
        /* CHECK: PARSE */
        {
          if( SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_LF_Value) >
              (BufferLength - (uint32) SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_LF_BufPos) -
              (uint32) SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_LFsize)) )
          {
            /* LF value is larger then expected (bufferlength) */
            RetVal |= E_SER_MALFORMED_MESSAGE;
          }
        }
      }

      /* set buffer pos to next DIM element */
      DeSerContextPtr->BytesCopied = DeSerContextPtr->BytesCopied + SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_LFsize);

#endif /* (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_ON) */
    }
  }
#endif /* (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON) */

  /* set CfgElemHandled back to FSA element */
  SOMEIPXF_SET_CONFIGPOS(RetVal, CfgElemHandled,
      SOMEIPXF_FSA_ELEMENT_ENTER, (SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_ConfigPosRemElement)));

  DeSerContextPtr->CfgElemHandled = CfgElemHandled;

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserHandlingFSADim
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType FSA_Dim
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE FSA_curr_element = SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_curr_element);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE FSA_max_element = SOMEIPXF_GET_FSAContext_DIM_LEVEL(FSA_max_element);

  /* FSA element got serialized */
  FSA_curr_element++;
  SOMEIPXF_SET_FSAContext_DIM_LEVEL(FSA_curr_element, FSA_curr_element);

  /* CHECK: NOPARSE */
  if(FSA_curr_element > FSA_max_element)
  {
    RetVal = E_SER_GENERIC_ERROR;
  }
  /* CHECK: PARSE */
  else if(FSA_curr_element == FSA_max_element)
  {
    /* all elements de-serialized */
    RetVal = SomeIpXf_FSA_elements_deser(
        FSAContextPtr,
        DeSerContextPtr,
        FSA_Dim);
  }
  else /* (FSA_curr_element < FSA_max_element) */
  {
    /* remaining elements to be de-serialized */
    RetVal = SomeIpXf_FSA_elements_deser_rem(
        BufPtr,
        BufferLength,
        ConfigPtr,
        FSAContextPtr,
        DeSerContextPtr);
  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserHandlingFSAContext
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(FSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) FSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  const uint8 Current_DIM_LEVEL = SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_current_DIM_LEVEL);
#if (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON)
  const uint8 Max_DIM_LEVEL = SOMEIPXF_GET_FSAContext_FSA_InfoType(FSA_max_config_DIM) - 1U;
  boolean con_invalid_dim_value = (Current_DIM_LEVEL > Max_DIM_LEVEL);
#else
  boolean con_invalid_dim_value = (Current_DIM_LEVEL != 0U);
#endif /* (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON) */

  /* CHECK: NOPARSE */
  if(TRUE == con_invalid_dim_value)
  {
    /* invalid dim values */

    /* clear FSA_Info_LEVEL entry on current nested level */
    /* Deviation TASKING-1 */
    SOMEIPXF_CLEAR_FSAContext_FSA_InfoType(FSAContextPtr->FSA_current_NESTED_LEVEL);

    if(FSAContextPtr->FSA_current_NESTED_LEVEL > 0U)
    {
      FSAContextPtr->FSA_current_NESTED_LEVEL--;
    }
    else
    {
      /* enable FSA root entry */
      FSAContextPtr->FSA_Root = TRUE;
    }

    RetVal = E_SER_GENERIC_ERROR;
  }
  else
  /* CHECK: PARSE */
  {
    /* handled elements of outer most dimension */
    if(Current_DIM_LEVEL == 0U)
    {
      RetVal = SomeIpXf_DeserHandlingFSADim(
          BufPtr,
          BufferLength,
          ConfigPtr,
          FSAContextPtr,
          DeSerContextPtr,
          SOMIPXF_OUTER_MOST_DIM
      );
    }
#if (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON)
    /* handled elements of current dimension */
    else if(Current_DIM_LEVEL < Max_DIM_LEVEL)
    {
      RetVal = SomeIpXf_DeserHandlingFSADim(
          BufPtr,
          BufferLength,
          ConfigPtr,
          FSAContextPtr,
          DeSerContextPtr,
          SOMIPXF_INTERMEDIATE_DIM
      );
    }
    /* handled elements of inner most dimension */
    else /* ( (Current_DIM_LEVEL == Max_DIM_LEVEL) && (0U != Max_DIM_LEVEL) ) */
    {
      RetVal = SomeIpXf_DeserHandlingFSADim(
          BufPtr,
          BufferLength,
          ConfigPtr,
          FSAContextPtr,
          DeSerContextPtr,
          SOMIPXF_INNER_MOST_DIM
      );
    }
#endif /* (SOMEIPXF_FSA_MULTI_DIM_ENABLED == STD_ON) */

  }

  return RetVal;
}

#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */

#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SetUpVSAContext_DimLevel
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) CfgElemHandledPtr,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) VSA_LF_BuffPos,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;
  uint8 dim_idx = 0U;
  boolean VSA_IsOfSize0 = FALSE;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = *CfgElemHandledPtr;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_LF_BuffPos_loc = *VSA_LF_BuffPos;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_LF_BuffPosOffset = DeSerContextPtr->BytesCopied + SOMEIPXF_HEADER_LENGTH;
  /* save array byte offset for SI of next VSA via data byte offset */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_ArrayDataByteOffset = DeSerContextPtr->DataByteOffset;

  /* set VSA DIM context within VSA_Context_DIM_LEVEL */
  for(dim_idx=0U;dim_idx<SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_max_config_DIM);dim_idx++)
  {
    /* set current dimension level */
    SOMEIPXF_SET_VSAContext_VSA_Info_LEVEL(VSA_current_DIM_LEVEL, dim_idx);
    /* set initiated VSA_ArrayDataByteOffset for new VSA context */
    SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_ArrayDataByteOffset, VSA_ArrayDataByteOffset);
    /* set size indicator offsetof values per dim */
    SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_SI_offsetof, ConfigPtr[CfgElemHandled] + VSA_ArrayDataByteOffset);
    /* set sizeof element per dim */
    SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_element_sizeof, ConfigPtr[CfgElemHandled + 1U]);
    /* set max elments values of data element per dim */
    SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_max_element, ConfigPtr[CfgElemHandled + 2U]);

    /* increase config pos offset per dim due to static config pos */
    CfgElemHandled = CfgElemHandled + 3U;
    /* set byte buffer pos of LF per dim */
    SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_LF_BufPos, VSA_LF_BuffPos_loc + VSA_LF_BuffPosOffset);

    if(SOMEIPXF_SER == VSAContextPtr->VSA_De_Seral)
    {
      /* get size indicator values and set elements to be serialized per dim */
      RetVal |= SomeIpXf_VSAGetSizeIndicatorValues(
          DataBytePtr,
          SOMEIPXF_GET_ADDR_VSAContext_DIM_LEVEL(VSA_elements),
          SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_SI_offsetof),
          SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_SI_sizeof)
      );

      if(SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_elements) > SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_max_element))
      {
        /* size indicator value is larger then expected (configured) */
        SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_elements, 0U);
        RetVal |= E_SER_GENERIC_ERROR;
      }
      else if( (SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_elements) != 0U) && (TRUE == VSA_IsOfSize0) )
      {
        /* outer dim SI was already set to 0U, remaining SI get set to 0U */
        SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_elements, 0U);
      }
      else if(SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_elements) == 0U)
      {
        VSA_IsOfSize0 = TRUE;
      }
      else
      {
        /* empty else */
      }

      SOMEIPXF_PARAM_UNUSED(BufferLength);
    }
    else
    {

      if( BufferLength < ((uint32) SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_LF_BufPos) +
          (uint32) SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_LFsize)) )
      {
        /* bufferlength is less then expected (LF_BufPos + LFsize) */
        RetVal |= E_SER_MALFORMED_MESSAGE;
      }
      else
      {
        /* get first length filed values serialized and set LF_Value to be de-serialized per dim */
        RetVal |= SomeIpXf_GetLFValues(
            BufPtr,
            ConfigPtr,
            SOMEIPXF_GET_ADDR_VSAContext_DIM_LEVEL(VSA_LF_Value),
            VSA_LF_BuffPos_loc,
            SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosNWEndianness),
            SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_LFsize)
        );

        /* CHECK: NOPARSE */
        if(RetVal == E_OK)
        /* CHECK: PARSE */
        {
          if( SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_LF_Value) >
              (BufferLength - (uint32) SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_LF_BufPos) -
              (uint32) SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_LFsize)) )
          {
            /* LF value is larger then expected (bufferlength) */
            RetVal |= E_SER_MALFORMED_MESSAGE;
          }
          else if(SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_LF_Value) == 0U)
          {
            VSA_IsOfSize0 = TRUE;
          }
          else
          {
            /* empty else */
          }
        }
      }

    }

    /* increase byte buffer pos of LF per dim due to static LF size */
    VSA_LF_BuffPos_loc = VSA_LF_BuffPos_loc + SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_LFsize);

    /* set initial element index to be handled */
    SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_curr_element, 0U);

  } /* for(dim_idx<SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_max_config_DIM)) */

  /* save if VSA is set with size indicator zero */
  SOMEIPXF_SET_VSAContext_VSA_Info_LEVEL(VSA_IsOfSize0,VSA_IsOfSize0);
  /* return current buffer pos where to continue processing */
  *VSA_LF_BuffPos = VSA_LF_BuffPos_loc;
  *CfgElemHandledPtr = CfgElemHandled;

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SetUpVSAContext
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_LF_BuffPos = 0U;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfigArraySize = ConfigPtr[1U];

  /* check if nested level should be increased */
  if( (TRUE == VSAContextPtr->VSA_Root) )
  {
    /* for root VSA keep nested level 0 and
     * set VSA_Root to FALSE for further nested VSAs */
    VSAContextPtr->VSA_Root = FALSE;
  }
  else
  {
    /* CHECK: NOPARSE */
    if( ( (VSAContextPtr->VSA_current_NESTED_LEVEL + 1U) >= SOMEIPXF_MAX_NESTED_VSA_DEPTH ) )
    {
      RetVal = E_SER_GENERIC_ERROR;
    }
    else
    /* CHECK: PARSE */
    {
      VSAContextPtr->VSA_current_NESTED_LEVEL++;
    }

  }

  /* CHECK: NOPARSE */
  if(RetVal == E_OK)
  /* CHECK: PARSE */
  {
    /* set VSA context within VSA_Info_LEVEL */
    SOMEIPXF_SET_VSAContext_VSA_Info_LEVEL(VSA_max_config_DIM, (uint8) ConfigPtr[CfgElemHandled + 1U]);

#if ((SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON))
    if( (SOMEIPXF_DE_SER == VSAContextPtr->VSA_De_Seral) &&
        /* different LFSize based different wire type */
        (DeSerContextPtr->TLV_LFSize_WireType567 != 0U) )
    {
      /* set diffrent LFsize based on diffrent wire type then configured */
      SOMEIPXF_SET_VSAContext_VSA_Info_LEVEL(VSA_LFsize, DeSerContextPtr->TLV_LFSize_WireType567);
      /* Reset the LF size */
      DeSerContextPtr->TLV_LFSize_WireType567 = 0U;
    }
    else
#endif /* ((SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)) */
    {
      /* set LFsize since only a received TLV member with a different wire type then configured could change
       * the LFSize for the de-serialization */
      SOMEIPXF_SET_VSAContext_VSA_Info_LEVEL(VSA_LFsize, ConfigPtr[CfgElemHandled + 2U]);
    }

    /* set config pos of NW Endianness for LF based on LFSize */
    RetVal = SomeIpXf_Set_ConfigPosNWEndianness(SOMEIPXF_GET_ADDR_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosNWEndianness),
                                                SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_LFsize));

    /* set VSA_SI_sizeof per dim */
    SOMEIPXF_SET_VSAContext_VSA_Info_LEVEL(VSA_SI_sizeof, ConfigPtr[CfgElemHandled + 3U]);

    /* set CfgElemHandled to Dim Config offset pos */
    CfgElemHandled = CfgElemHandled + SOMEIPXF_VSA_INFO_LEVEL_STATIC_CONFIG_OFFSET + 1U;

    /* CHECK: NOPARSE */
    /* ensure that configured dimensions level can be hold by VSA context */
    if(SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_max_config_DIM) <= SOMEIPXF_VSA_MAX_DIM)
    /* CHECK: PARSE */
    {
      /* set VSA DIM context */
        RetVal |= SomeIpXf_SetUpVSAContext_DimLevel(
            BufPtr,
            BufferLength,
            DataBytePtr,
            ConfigPtr,
            VSAContextPtr,
            &CfgElemHandled,
            &VSA_LF_BuffPos,
            DeSerContextPtr
            );
    }
    else
    {
      /* invalid config */
      RetVal = E_SER_GENERIC_ERROR;
    }

    /* save config index of zero element entry */
    SOMEIPXF_SET_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosZeroElement, CfgElemHandled);
    /* save config index of VSA stop */
    SOMEIPXF_SET_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosVSAStop, ConfigPtr[CfgElemHandled + 1U]);

    /* CHECK: NOPARSE */
    /* check if VSA stop index pos is within config array size */
    if(SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosVSAStop) > (ConfigArraySize - 2U))
    {
      /* VSA stop index pos is invalid */
      RetVal = E_SER_GENERIC_ERROR;
    }
    /* CHECK: PARSE */

    /* save config index of element entry */
    SOMEIPXF_SET_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosRemElement, CfgElemHandled + 2U);

    if(E_OK == RetVal)
    {
      /* set buffer pos to inner most DIM payload */
      DeSerContextPtr->BytesCopied = DeSerContextPtr->BytesCopied + VSA_LF_BuffPos;

      if(TRUE == SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_IsOfSize0))
      {
        /* VSA with SI set to zero */

        /* set config pos to zero element check point tag to hanled zero SIs */
        DeSerContextPtr->CfgElemHandled = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosZeroElement) - 1U;
      }
      else
      {
        /* set config pos to payload element tag (represents config arry entries) */
        DeSerContextPtr->CfgElemHandled = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosRemElement);
      }
    }

  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_VSAGetSizeIndicatorValues
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) ElementNum,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_SI_offsetof,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_SI_sizeof
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  P2CONST(uint16, AUTOMATIC, SOMEIPXF_VAR) SizePtr2 =
     (P2CONST(uint16, AUTOMATIC, SOMEIPXF_VAR))
     (P2CONST(void, AUTOMATIC, SOMEIPXF_VAR)) &DataBytePtr[VSA_SI_offsetof];
  P2CONST(uint32, AUTOMATIC, SOMEIPXF_VAR) SizePtr4 =
     (P2CONST(uint32, AUTOMATIC, SOMEIPXF_VAR))
     (P2CONST(void, AUTOMATIC, SOMEIPXF_VAR)) &DataBytePtr[VSA_SI_offsetof];
#if (defined HAS_64BIT_TYPES)
  P2CONST(uint64, AUTOMATIC, SOMEIPXF_VAR) SizePtr8 =
     (P2CONST(uint64, AUTOMATIC, SOMEIPXF_VAR))
     (P2CONST(void, AUTOMATIC, SOMEIPXF_VAR)) &DataBytePtr[VSA_SI_offsetof];
#endif

  /* get value of size in dependence on data type */
  switch(VSA_SI_sizeof)
  {
     case 1U:
     {
       *ElementNum = (SOMEIPXF_XFRMBUFFERLENGTH_TYPE) DataBytePtr[VSA_SI_offsetof];
     }
     break;

     case 2U:
     {
       *ElementNum = (SOMEIPXF_XFRMBUFFERLENGTH_TYPE) *SizePtr2;
     }
     break;

     case 4U:
     {
       SOMEIPXF_ASSIGN_UINT_32_64_TO_XFRMBUFFERLENGTH_TYPE((*ElementNum), (*SizePtr4));
     }
     break;

#if (defined HAS_64BIT_TYPES)
     case 8U:
     {
       SOMEIPXF_ASSIGN_UINT_32_64_TO_XFRMBUFFERLENGTH_TYPE((*ElementNum), (*SizePtr8));
     }
     break;
#endif

     /* CHECK: NOPARSE */
     default:
     {
        /* this should never be reached */
        /* defensive programming mechanism (no Det interface) */
        *ElementNum = 0U;
        RetVal = E_SER_GENERIC_ERROR;
     }
     break;
     /* CHECK: PARSE */
  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerHandlingVSADim
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType VSA_Dim
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON)
  const uint8 Current_DIM_LEVEL = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_current_DIM_LEVEL);
  /* Max_DIM_LEVEL reduced by one due to zero based dim handling */
  const uint8 Max_DIM_LEVEL = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_max_config_DIM) - 1U;
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON) */
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_LF_BufPos = SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_LF_BufPos);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_LFsize = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_LFsize);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_ConfigPosVSAStop = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosVSAStop);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_ArrayDataByteOffset = SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_ArrayDataByteOffset);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE Elements = SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_elements);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_ConfigPosNWEndianness = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosNWEndianness);
  const boolean VSA_IsOfSize0 = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_IsOfSize0);

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_ConfigPosRemElement = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosRemElement);
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CurrElement = SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_curr_element);
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = VSA_ConfigPosVSAStop;

  /* calculate the current element byte length based from the LF position */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE Element_ByteLength = (DeSerContextPtr->BytesCopied) -
      ((VSA_LF_BufPos - SOMEIPXF_HEADER_LENGTH) + VSA_LFsize);

#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_OFF)
  SOMEIPXF_PARAM_UNUSED(VSA_Dim);
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_OFF) */

  if(0U != Elements)
  {
    /* element only serialized in case of non zero SI */
    SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_curr_element)++;
    CurrElement++;
  }

  /* CHECK: NOPARSE */
  if(CurrElement > Elements)
  {
    RetVal = E_SER_GENERIC_ERROR;
  }
  /* CHECK: PARSE */
  /* all elements serialized */
  else if(CurrElement == Elements)
  {
    RetVal = SomeIpXf_Set_LF(BufPtr, ConfigPtr, (VSA_LF_BufPos - SOMEIPXF_HEADER_LENGTH), Element_ByteLength,
        VSA_LFsize, VSA_ConfigPosNWEndianness);

    /* clear ArrayDataByteOffset of the current nested level */
    SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_ArrayDataByteOffset, 0U);

#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON)
    if(VSA_Dim != SOMIPXF_OUTER_MOST_DIM)
    {
      /* set data byte offset to array byte offset */
      DeSerContextPtr->DataByteOffset = 0U;

      /* decrease dimension */
      SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_current_DIM_LEVEL)--;

      /* set CfgElemHandled to check point again */
      CfgElemHandled = DeSerContextPtr->CfgElemHandled - 1U;
    }
    else
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON) */
    {
      /* set CfgElemHandled to next tag after SOMEIPXF_VSA_STOP */
      SOMEIPXF_SET_CONFIGPOS(RetVal, CfgElemHandled,
          SOMEIPXF_VSA_STOP, VSA_ConfigPosVSAStop);

      /* check if VSA nested level needs to be decreased */
      if(VSAContextPtr->VSA_current_NESTED_LEVEL > 0U)
      {
        VSAContextPtr->VSA_current_NESTED_LEVEL--;
        DeSerContextPtr->DataByteOffset = SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_ArrayDataByteOffset);
      }
      else /* VSA_current_NESTED_LEVEL == 0U */
      {
        /* set data byte offset to array byte offset */
        DeSerContextPtr->DataByteOffset = 0U;
        /* enable VSA root entry */
        VSAContextPtr->VSA_Root = TRUE;
      }
    }

  }
  /* remaining elements to be serialized */
  else /* CurrElement < Elements */
  {
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE ArrayDataByteOffsetofNextElem = VSA_ArrayDataByteOffset +
        SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_element_sizeof);

    /* set ArrayDataByteOffset to next outer most element */
    SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_ArrayDataByteOffset, ArrayDataByteOffsetofNextElem);
    /* set data byte offset to array byte offset */
    DeSerContextPtr->DataByteOffset = ArrayDataByteOffsetofNextElem;

#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON)
    {
      uint32 dim_idx = 0U;

      if(VSA_Dim == SOMIPXF_INTERMEDIATE_DIM)
      {
        RetVal = SomeIpXf_Set_LF(BufPtr, ConfigPtr, (VSA_LF_BufPos - SOMEIPXF_HEADER_LENGTH), Element_ByteLength,
                  VSA_LFsize, VSA_ConfigPosNWEndianness);
      }

      /* loop over inner dimensions */
      for(dim_idx = Current_DIM_LEVEL;dim_idx < Max_DIM_LEVEL; dim_idx++)
      {
        /* increase dimension and reset VSA_curr_element */
        SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_current_DIM_LEVEL)++;
        SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_curr_element, 0U);

        /* set array byte offset to next inner dim elements */
        SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_ArrayDataByteOffset, ArrayDataByteOffsetofNextElem);
        /* set data byte offset to array byte offset */
        DeSerContextPtr->DataByteOffset = ArrayDataByteOffsetofNextElem;

        /* save LF buffer pos of next dim */
        SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_LF_BufPos, DeSerContextPtr->BytesCopied + SOMEIPXF_HEADER_LENGTH);

        /* set buffer pos to next DIM payload element */
        DeSerContextPtr->BytesCopied = DeSerContextPtr->BytesCopied + VSA_LFsize;
      }

    }
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON) */

    if(TRUE == VSA_IsOfSize0)
    {
      /* in case of VSA with SI set to zero, set config pos to enter check point tag again */
      CfgElemHandled = DeSerContextPtr->CfgElemHandled - 1U;
    }
    else
    {
      /* set config pos back to payload element tag */
      SOMEIPXF_SET_CONFIGPOS(RetVal, CfgElemHandled,
          SOMEIPXF_VSA_ELEMENT_ENTER, VSA_ConfigPosRemElement);
    }

  }

  DeSerContextPtr->CfgElemHandled = CfgElemHandled;

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SerializeVSA_element_check_point
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  const uint8 Current_DIM_LEVEL = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_current_DIM_LEVEL);
#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON)
  /* Max_DIM_LEVEL reduced by one due to zero based dim handling */
  const uint8 Max_DIM_LEVEL = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_max_config_DIM) - 1U;
  boolean con_invalid_dim_value = (Current_DIM_LEVEL > Max_DIM_LEVEL);
#else
  boolean con_invalid_dim_value = (Current_DIM_LEVEL != 0U);
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON) */

  /* CHECK: NOPARSE */
  if(TRUE == con_invalid_dim_value)
  {
    /* invalid dim values */
    RetVal = E_SER_GENERIC_ERROR;
  }
  else
  /* CHECK: PARSE */
  {
    /* handled elements of outer most dimension */
    if(Current_DIM_LEVEL == 0U)
    {
      RetVal = SomeIpXf_SerHandlingVSADim(
          BufPtr,
          ConfigPtr,
          VSAContextPtr,
          DeSerContextPtr,
          SOMIPXF_OUTER_MOST_DIM
       );
    }
#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON)
    /* handled elements of current dimension */
    else if(Current_DIM_LEVEL < Max_DIM_LEVEL)
    {
      RetVal = SomeIpXf_SerHandlingVSADim(
          BufPtr,
          ConfigPtr,
          VSAContextPtr,
          DeSerContextPtr,
          SOMIPXF_INTERMEDIATE_DIM
       );
    }
    /* handled elements of inner most dimension */
    else /* ( (Current_DIM_LEVEL == Max_DIM_LEVEL) && (0U != Max_DIM_LEVEL) ) */
    {
      RetVal = SomeIpXf_SerHandlingVSADim(
          BufPtr,
          ConfigPtr,
          VSAContextPtr,
          DeSerContextPtr,
          SOMIPXF_INNER_MOST_DIM
       );
    }
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON) */

  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_VSA_elements_deser
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType VSA_Dim
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON)
  const uint8 Current_DIM_LEVEL = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_current_DIM_LEVEL);
  /* Max_DIM_LEVEL reduced by one due to zero based dim handling */
  const uint8 Max_DIM_LEVEL = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_max_config_DIM) - 1U;
  const boolean VSA_IsOfSize0 = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_IsOfSize0);
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON) */

  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_ConfigPosVSAStop = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosVSAStop);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_SI_offsetof = SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_SI_offsetof);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_SI_sizeof = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_SI_sizeof);

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CurrElement = SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_curr_element);
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = VSA_ConfigPosVSAStop;

#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_OFF)
  SOMEIPXF_PARAM_UNUSED(VSA_Dim);
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON) */

#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON)
  if(VSA_Dim == SOMIPXF_OUTER_MOST_DIM)
  {
    uint32 dim_idx = 0U;

    /* Iterate through all DIMs and check if VSA_SI_offsetof is the same.
     * If this is the case && VSA_IsOfSize0 is true then set 0U SI */
    if(TRUE == VSA_IsOfSize0)
    {
      boolean VSA_all_SI_offsetof_equal = TRUE;
      SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_SI_offsetof_hlp = VSA_SI_offsetof;

      /* interate through all DIMs and check if VSA_SI_offsetof is equal */
      for(dim_idx = Current_DIM_LEVEL;dim_idx < Max_DIM_LEVEL; dim_idx++)
      {
        /* increase dim */
        SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_current_DIM_LEVEL)++;
        /* get inner SI offset and check if the same as the initial one */
        VSA_SI_offsetof_hlp = SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_SI_offsetof);
        if(VSA_SI_offsetof_hlp != VSA_SI_offsetof)
        {
          VSA_all_SI_offsetof_equal = FALSE;
          break;
        }
      }

      if(TRUE == VSA_all_SI_offsetof_equal)
      {
        CurrElement = 0U;
      }
    }

  }
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON) */

  /* set size indicator with current element value */
  RetVal = SomeIpXf_Set_SI(DataBytePtr, CurrElement, VSA_SI_offsetof,
      VSA_SI_sizeof);

  /* clear ArrayDataByteOffset of the current nested level */
  SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_ArrayDataByteOffset, 0U);

  /* set CfgElemHandled to next tag after SOMEIPXF_VSA_STOP */
  SOMEIPXF_SET_CONFIGPOS(RetVal, CfgElemHandled,
      SOMEIPXF_VSA_STOP, VSA_ConfigPosVSAStop);

#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON)
  if(VSA_Dim != SOMIPXF_OUTER_MOST_DIM)
  {
    /* set data byte offset to array byte offset */
    DeSerContextPtr->DataByteOffset = 0U;

    /* decrease dimension */
    SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_current_DIM_LEVEL)--;

    /* set CfgElemHandled to check point again */
    CfgElemHandled = DeSerContextPtr->CfgElemHandled - 1U;
  }
  else
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON) */
  {
    /* set CfgElemHandled to next tag after SOMEIPXF_VSA_STOP */
    SOMEIPXF_SET_CONFIGPOS(RetVal, CfgElemHandled,
        SOMEIPXF_VSA_STOP, VSA_ConfigPosVSAStop);

    /* check if VSA nested level needs to be decreased */
    if(VSAContextPtr->VSA_current_NESTED_LEVEL > 0U)
    {
      VSAContextPtr->VSA_current_NESTED_LEVEL--;
      DeSerContextPtr->DataByteOffset = SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_ArrayDataByteOffset);
    }
    else /* VSA_current_NESTED_LEVEL == 0U */
    {
      /* set data byte offset to array byte offset */
      DeSerContextPtr->DataByteOffset = 0U;
      /* enable VSA root entry */
      VSAContextPtr->VSA_Root = TRUE;
    }
  }

  DeSerContextPtr->CfgElemHandled = CfgElemHandled;

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_VSA_elements_deser_rem
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   uint32 Element_ByteLength
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON)
  const uint8 Current_DIM_LEVEL = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_current_DIM_LEVEL);
  /* Max_DIM_LEVEL reduced by one due to zero based dim handling */
  const uint8 Max_DIM_LEVEL = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_max_config_DIM) - 1U;
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_ConfigPosNWEndianness =
      SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosNWEndianness);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_LFsize = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_LFsize);
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON) */

  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_ConfigPosVSAStop = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosVSAStop);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_ArrayDataByteOffset = SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_ArrayDataByteOffset);
  const boolean VSA_IsOfSize0 = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_IsOfSize0);

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_ConfigPosRemElement = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_ConfigPosRemElement);
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CurrElement = SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_curr_element);
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = VSA_ConfigPosVSAStop;

  /* LF value is greater then expected (max possible dim length reached)
   * if the next element is out of VSA_max_element boundary */
  boolean con_skipping_LF_larger_max_elem = ( (CurrElement + 1U) >
  SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_max_element) );

#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_OFF)
  SOMEIPXF_PARAM_UNUSED(BufPtr);
  SOMEIPXF_PARAM_UNUSED(BufferLength);
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON) */

  if(TRUE == con_skipping_LF_larger_max_elem)
  {
    /* length of a variable length array is greater than expected */
    const uint32 NumSkipBytes = (uint32) (SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_LF_Value) - (Element_ByteLength));
    /* skipping behavior */
    DeSerContextPtr->BytesCopied = (SOMEIPXF_XFRMBUFFERLENGTH_TYPE) (DeSerContextPtr->BytesCopied + NumSkipBytes);

    /* in case of skipping, set config pos to enter check point tag again */
    CfgElemHandled = DeSerContextPtr->CfgElemHandled - 1U;

    /* decrease VSA_curr_element once for skipping due to increase via reenter of check point tag */
    SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_curr_element)--;
  }
  else
  {
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE ArrayDataByteOffsetofNextElem = VSA_ArrayDataByteOffset +
        SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_element_sizeof);

    /* set ArrayDataByteOffset to next outer most element */
    SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_ArrayDataByteOffset, ArrayDataByteOffsetofNextElem);
    /* set data byte offset to array byte offset */
    DeSerContextPtr->DataByteOffset = ArrayDataByteOffsetofNextElem;

#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON)
    {
      uint32 dim_idx = 0U;

      /* loop over inner dimensions */
      for(dim_idx = Current_DIM_LEVEL;dim_idx < Max_DIM_LEVEL; dim_idx++)
      {
        /* increase dimension and reset VSA_curr_element */
        SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_current_DIM_LEVEL)++;
        SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_curr_element, 0U);

        /* set array byte offset to next inner dim elements */
        SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_ArrayDataByteOffset, ArrayDataByteOffsetofNextElem);
        /* set data byte offset to array byte offset */
        DeSerContextPtr->DataByteOffset = ArrayDataByteOffsetofNextElem;

        /* save LF buffer pos of next dim */
        SOMEIPXF_SET_VSAContext_DIM_LEVEL(VSA_LF_BufPos, DeSerContextPtr->BytesCopied + SOMEIPXF_HEADER_LENGTH);

        if( BufferLength < ((uint32) SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_LF_BufPos) +
            (uint32) SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_LFsize)) )
        {
          /* bufferlength is less then expected (LF_BufPos + LFsize) */
          RetVal |= E_SER_MALFORMED_MESSAGE;
        }
        else
        {
          /* get new LF value and check if valid based on not larger then expected */
          RetVal |= SomeIpXf_GetLFValues(
              BufPtr,
              &ConfigPtr[0U],
              SOMEIPXF_GET_ADDR_VSAContext_DIM_LEVEL(VSA_LF_Value),
              SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_LF_BufPos) - SOMEIPXF_HEADER_LENGTH,
              VSA_ConfigPosNWEndianness,
              SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_LFsize)
          );

          /* CHECK: NOPARSE */
          if(RetVal == E_OK)
          /* CHECK: PARSE */
          {
            if( SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_LF_Value) >
                (BufferLength - (uint32) SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_LF_BufPos) -
                (uint32) SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_LFsize)) )
            {
              /* LF value is larger then expected (bufferlength) */
              RetVal |= E_SER_MALFORMED_MESSAGE;
            }
          }
        }

        /* set buffer pos to next DIM payload element */
        DeSerContextPtr->BytesCopied = DeSerContextPtr->BytesCopied + VSA_LFsize;
      }

    }
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON) */

    if(TRUE == VSA_IsOfSize0)
    {
      /* in case of VSA with SI set to zero, set config pos to enter check point tag again */
      CfgElemHandled = DeSerContextPtr->CfgElemHandled - 1U;
    }
    else
    {
      /* set config pos back to payload element tag */
      SOMEIPXF_SET_CONFIGPOS(RetVal, CfgElemHandled,
          SOMEIPXF_VSA_ELEMENT_ENTER, VSA_ConfigPosRemElement);
    }
  }

  DeSerContextPtr->CfgElemHandled = CfgElemHandled;

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserHandlingVSADim
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SomIpXf_DimensionType VSA_Dim
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  const uint32 VSA_LF_BufPos = (uint32) SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_LF_BufPos);
  const uint32 VSA_LFsize = (uint32) SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_LFsize);
  const uint32 VSA_LF_Value = SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_LF_Value);

  /* calculate element size after de-seralization based on LF position */
  uint32 Element_ByteLength = (uint32) (DeSerContextPtr->BytesCopied) -
      ((VSA_LF_BufPos - SOMEIPXF_HEADER_LENGTH) + VSA_LFsize);

  if(0U != VSA_LF_Value)
  {
    /* element only de-serialized in case of non zero SI */
    SOMEIPXF_GET_VSAContext_DIM_LEVEL(VSA_curr_element)++;
  }

  if(Element_ByteLength == VSA_LF_Value)
  {
    /* all elements de-serialized */
    RetVal = SomeIpXf_VSA_elements_deser(
        DataBytePtr,
        ConfigPtr,
        VSAContextPtr,
        DeSerContextPtr,
        VSA_Dim);
  }
  else if(Element_ByteLength < VSA_LF_Value)
  {
    /* remaining elements to be de-serialized */
    RetVal = SomeIpXf_VSA_elements_deser_rem(
        BufPtr,
        BufferLength,
        ConfigPtr,
        VSAContextPtr,
        DeSerContextPtr,
        Element_ByteLength);
  }
  /* VSA_ElementSize is larger then VSA_LF_Value -> VSA_LF_Value is invalid */
  else
  {
    RetVal = E_SER_MALFORMED_MESSAGE;
  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserializeVSA_element_check_point
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(VSA_NESTED_LEVEL_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) VSAContextPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  const uint8 Current_DIM_LEVEL = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_current_DIM_LEVEL);
#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON)
  /* Max_DIM_LEVEL reduced by one due to zero based dim handling */
  const uint8 Max_DIM_LEVEL = SOMEIPXF_GET_VSAContext_VSA_Info_LEVEL(VSA_max_config_DIM) - 1U;
  boolean con_invalid_dim_value = (Current_DIM_LEVEL > Max_DIM_LEVEL);
#else
  boolean con_invalid_dim_value = (Current_DIM_LEVEL != 0U);
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON) */

  /* CHECK: NOPARSE */
  if(TRUE == con_invalid_dim_value)
  {
    /* invalid dim values */
    RetVal = E_SER_GENERIC_ERROR;
  }
  else
  /* CHECK: PARSE */
  {
    /* handled elements of outer most dimension */
    if(Current_DIM_LEVEL == 0U)
    {
      RetVal = SomeIpXf_DeserHandlingVSADim(
          BufPtr,
          BufferLength,
          DataBytePtr,
          ConfigPtr,
          VSAContextPtr,
          DeSerContextPtr,
          SOMIPXF_OUTER_MOST_DIM
          );
    }
#if (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON)
    /* handled elements of current dimension */
    else if(Current_DIM_LEVEL < Max_DIM_LEVEL)
    {
      RetVal = SomeIpXf_DeserHandlingVSADim(
          BufPtr,
          BufferLength,
          DataBytePtr,
          ConfigPtr,
          VSAContextPtr,
          DeSerContextPtr,
          SOMIPXF_INTERMEDIATE_DIM
          );
    }
    /* handled elements of inner most dimension */
    else /* ( (Current_DIM_LEVEL == Max_DIM_LEVEL) && (0U != Max_DIM_LEVEL) ) */
    {
      RetVal = SomeIpXf_DeserHandlingVSADim(
          BufPtr,
          BufferLength,
          DataBytePtr,
          ConfigPtr,
          VSAContextPtr,
          DeSerContextPtr,
          SOMIPXF_INNER_MOST_DIM
          );
    }
#endif /* (SOMEIPXF_VSA_MULTI_DIM_ENABLED == STD_ON) */

  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Set_SI
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE CurrElement,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_SI_offsetof,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE VSA_SI_sizeof
)
{
  uint8 RetVal = E_OK;

  const uint16 NumOfElements_u16 = (uint16) CurrElement;
  const uint32 NumOfElements_u32 = (uint32) CurrElement;
#if (defined HAS_64BIT_TYPES)
  const uint64 NumOfElements_u64 = (uint64) CurrElement;
#endif /* (defined HAS_64BIT_TYPES) */

  switch(VSA_SI_sizeof)
  {
    case 1U:
    {
      DataBytePtr[VSA_SI_offsetof] = (uint8) ((CurrElement) & 0xFFU);
    }
    break;

    case 2U:
    {
      SOMEIPXF_COPY_U16_DATA_ELM(DataBytePtr, VSA_SI_offsetof, NumOfElements_u16);
    }
    break;

    case 4U:
    {
      SOMEIPXF_COPY_U32_DATA_ELM(DataBytePtr, VSA_SI_offsetof, NumOfElements_u32);
    }
    break;

#if (defined HAS_64BIT_TYPES)
    case 8U:
    {
      SOMEIPXF_COPY_U64_DATA_ELM(DataBytePtr, VSA_SI_offsetof, NumOfElements_u64);
    }
    break;
#endif

    /* CHECK: NOPARSE */
    default:
    {
      /* should never been reached */
      RetVal = E_SER_GENERIC_ERROR;
    }
    break;
    /* CHECK: PARSE */
  }

  return RetVal;
}
#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */


#if ((SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON))

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Get_TLV_LFSize_And_EndianessPos (
  uint16 Tag_WireType,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) TLV_static_lenghtfield_bytesizeptr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) ConfigPosNWEndiannessptr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  switch(Tag_WireType)
  {
    case 5U:
    {
      *TLV_static_lenghtfield_bytesizeptr = 1U;
      *ConfigPosNWEndiannessptr = SOMEIPXF_CONFIG_START_OFFEST_NW_1B;
    }
    break;
    case 6U:
    {
      *TLV_static_lenghtfield_bytesizeptr = 2U;
      *ConfigPosNWEndiannessptr = SOMEIPXF_CONFIG_START_OFFEST_NW_2B;
    }
    break;
    case 7U:
    {
      *TLV_static_lenghtfield_bytesizeptr = 4U;
      *ConfigPosNWEndiannessptr = SOMEIPXF_CONFIG_START_OFFEST_NW_4B;
    }
    break;
    /* CHECK: NOPARSE */
    default:
    {
      /* this should never be reached */
      *TLV_static_lenghtfield_bytesizeptr = 1U;
      *ConfigPosNWEndiannessptr = SOMEIPXF_CONFIG_START_OFFEST_NW_1B;
      /* defensive programming mechanism (no Det interface) */
      RetVal = E_SER_GENERIC_ERROR;
    }
    break;
    /* CHECK: PARSE */
  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_GetLFByteSize
(
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) TLV_static_lf_bytesizeptr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE WireType
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  switch(WireType)
  {
    case 5U:
    {
      *TLV_static_lf_bytesizeptr = 1U;
    }
    break;
    case 6U:
    {
      *TLV_static_lf_bytesizeptr = 2U;
    }
    break;
    case 7U:
    {
      *TLV_static_lf_bytesizeptr = 4U;
    }
    break;
    /* CHECK: NOPARSE */
    default:
    /* CHECK: PARSE */
    {
      /* this should never be reached */
      *TLV_static_lf_bytesizeptr = 1U;
      /* defensive programming mechanism (no Det interface) */
      RetVal = E_SER_GENERIC_ERROR;
    }
    break;
  }

  return RetVal;

}

#endif /* ((SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) || (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)) */

#if (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON)

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Set_TLV_lenghtfield (
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied
)
{
  /* used for defensive programming only (no Det interface) */
   uint8 RetVal = E_OK;

  /* get position of LF for current TLV_Struct_NestedLevel */
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLV_lenghtfield_pos =
      SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(PositionofLF);
  /* get size of LF for current TLV_Struct_NestedLevel */
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLV_lenghtfield_bytesize =
      SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLV_static_lenghtfield_bytesize);
  /* get Endianness position for current TLV_Struct_NestedLevel */
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE EndinessPosition =
      SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(EndinessPosition);
   /* calculate serialized length based on BytesCopied - position */
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE DataMemSize =
    (SOMEIPXF_XFRMBUFFERLENGTH_TYPE)(BytesCopied - (TLV_lenghtfield_pos + TLV_lenghtfield_bytesize));

  /* set calculated serialized DataMemSize based on TLV_lenghtfield_bytesize */
  switch(TLV_lenghtfield_bytesize)
  {
    /* Length field value is set */
    case 1U:
    {
      BufPtr[TLV_lenghtfield_pos] = (uint8)((DataMemSize) & 0xFFU);
    }
    break;
    case 2U:
    {
      SOMEIPXF_COPY_U16_BUF(ConfigPtr, EndinessPosition, BufPtr, TLV_lenghtfield_pos,
          (uint16)DataMemSize);
    }
    break;
    case 4U:
    {
      SOMEIPXF_COPY_U32_BUF(ConfigPtr, EndinessPosition, BufPtr, TLV_lenghtfield_pos,
          (uint32)DataMemSize);
    }
    break;
    /* CHECK: NOPARSE */
    default:
    {
      /* should never been reached */
      RetVal = E_SER_MALFORMED_MESSAGE;
    }
    break;
    /* CHECK: PARSE */
  }
  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_CheckValidWiretypeAndDuplicateKnownMember (
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) CfgElemHandledPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE ValidDataIdIndex,
  uint16 TLV_tag_header
)
{
  /* used for defensive programming only (no Det interface) */
  uint8  RetVal = E_OK;

#if(SOMEIPXF_TLV_STRUCT_MEMBER_UNIQUE_DATAID_CHECK_ENABLE == STD_ON)
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE Byteposition;
    uint8 bitposition;
    boolean DuplicateDataIdfound = FALSE;
#endif /* (SOMEIPXF_TLV_STRUCT_MEMBER_UNIQUE_DATAID_CHECK_ENABLE == STD_ON) */

  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE DataID_CfgElemHandled_loc = SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLVDataIDStartCfgHandleId);

  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE PositionofMemberstartIndex =
          ConfigPtr[DataID_CfgElemHandled_loc + (ValidDataIdIndex * 2U) + 1U];

  uint16 TLV_tag_WireType = 0xFFFFU;
  uint8 TLV_tag_WireType_configured;

  /* Increment the index of processed dataid's */
  SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLVDataIDProcessedCount) = ValidDataIdIndex + 1U;

#if(SOMEIPXF_TLV_STRUCT_MEMBER_UNIQUE_DATAID_CHECK_ENABLE == STD_ON)
  /* Get the byte position of the TLVID */
  Byteposition = ValidDataIdIndex / 8U;
  /* Get the bit position of the TLVID in the found byte */
  bitposition = ValidDataIdIndex % 8U;

  /* Find duplicate dataid in receiver configuration */
  DuplicateDataIdfound =
      (boolean)((SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(DataID_Processed[Byteposition]) &
      (1U << bitposition)) != 0U);

  if(DuplicateDataIdfound == TRUE)
  {
    /* Duplicateid found */
    RetVal = E_SER_MALFORMED_MESSAGE;
  }
  else
#endif /* (SOMEIPXF_TLV_STRUCT_MEMBER_UNIQUE_DATAID_CHECK_ENABLE == STD_ON) */
  {

#if(SOMEIPXF_TLV_STRUCT_MEMBER_UNIQUE_DATAID_CHECK_ENABLE == STD_ON)
    /* Set the position of the TLVID to 1 */
    SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(DataID_Processed[Byteposition]) =
        (uint8)(SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(DataID_Processed[Byteposition]) |
        (1U << bitposition));
#endif /* (SOMEIPXF_TLV_STRUCT_MEMBER_UNIQUE_DATAID_CHECK_ENABLE == STD_ON) */

    /* Check member is optional */
    if(ConfigPtr[PositionofMemberstartIndex] == SOMEIPXF_STRUCT_TLV_MEMBER_OPT_START)
    {
      /* Get the configured wiretype */
      TLV_tag_WireType_configured = (uint8)ConfigPtr[PositionofMemberstartIndex + 2U];
    }
    else /* member is fixed */
    {
      /* Get the configured wiretype */
      TLV_tag_WireType_configured = (uint8)ConfigPtr[PositionofMemberstartIndex + 1U];
    }

    SOMEIPXF_GET_TLV_TAG_WIRETYPE(TLV_tag_header, TLV_tag_WireType);

    /* Check configured wiretype is complex data type */
    if(TLV_tag_WireType_configured >= 4U)
    {
      /* If wire type is equal to or greater than 4 but less than 7 found valid id
       * Defensive check as wiretype can never be greater than 7
       */
      /* CHECK: NOPARSE */
      if((TLV_tag_WireType >= 4U) && (TLV_tag_WireType <= 7U))
      /* CHECK: PARSE*/
      {
        /* Valid wiretype */
        RetVal = E_OK;

        *CfgElemHandledPtr = ConfigPtr[DataID_CfgElemHandled_loc + (ValidDataIdIndex * 2U) + 1U] - 1U;
      }
      else
      {
        /* Invalid wiretype */
        RetVal = E_SER_MALFORMED_MESSAGE;
      }
    }
    else
    {
      /* If member is not complex data type then TAG shall be matched */
      if(TLV_tag_WireType == TLV_tag_WireType_configured)
      {
         /* Valid wiretype */
         RetVal = E_OK;

         /* According to the current DataID config pos set CfgElemHandled to the corresponding TLV member */
         *CfgElemHandledPtr = ConfigPtr[DataID_CfgElemHandled_loc + (ValidDataIdIndex * 2U) + 1U] - 1U;
      }
      else
      {
        /* Invalid wiretype */
        RetVal = E_SER_MALFORMED_MESSAGE;
      }
    }
  }

  return RetVal;
}

STATIC FUNC(void, SOMEIPXF_CODE) SomeIpXf_ValidateReceivedDataId(
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(boolean, AUTOMATIC, SOMEIPXF_APPL_DATA)  DataID_validPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) ValidDataIdIndexPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE StartIndexInConfigDataId,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE StopIndexInConfigDataId,
  uint16 TLV_tag_dataID
)
{
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE ii_NumDataIDs;
  boolean DataID_valid_cond = FALSE;

  /* Config index of the dataid's list */
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE DataID_CfgElemHandled_loc =
      SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLVDataIDStartCfgHandleId);

  /* Find the received member dataid in the config list */
  for(ii_NumDataIDs = StartIndexInConfigDataId; ii_NumDataIDs < StopIndexInConfigDataId; ii_NumDataIDs++)
  {
    /* Check received member dataid in same as config  */
    DataID_valid_cond =
        ((SOMEIPXF_XFRMBUFFERLENGTH_TYPE) ConfigPtr[DataID_CfgElemHandled_loc + (ii_NumDataIDs * 2U)] ==
        TLV_tag_dataID);

    /* If received valid dataid found in the config*/
    if(DataID_valid_cond == TRUE)
    {
      /* Position of dataid in the list */
      *ValidDataIdIndexPtr = ii_NumDataIDs;

      /* Valid dataid found in the config */
      *DataID_validPtr = TRUE;

      /* break if valid data id found */
      break;
    }
  }
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Int_DataID_WireType_Valid (
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(boolean, AUTOMATIC, SOMEIPXF_APPL_DATA)  DataID_validPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) CfgElemHandledPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  /* valid dataid index in config */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE ValidDataIdIndex;
  /* Get the TLV dataid's count */
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLVDataIDCount =
      SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLVDataIDCount);
  /* Get the TLV dataid's processed count */
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLVDataIDProcessedCount =
      SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLVDataIDProcessedCount);

  uint16 TLV_tag_header;
  uint16 TLV_tag_dataID;

  /* Retrive the tlv tag from the received stream */
  SOMEIPXF_GET_TLV_TAG_VALUE(BufPtr, TLV_tag_header, 0U, BytesCopied);
  /* Get the dataId */
  SOMEIPXF_GET_TLV_TAG_DATAID(TLV_tag_dataID, TLV_tag_header);

  /* Find the received data id from the stream
   * Start index: Last valid dataid found Index (TLVDataIDProcessedCount)
   * End Index: Till the end of the dataid list (TLVDataIDCount)
   */
  SomeIpXf_ValidateReceivedDataId(ConfigPtr, DataID_validPtr, TLVContextPtr, &ValidDataIdIndex,
      TLVDataIDProcessedCount, TLVDataIDCount, TLV_tag_dataID);

  /* Check if received member is found in the config */
  if(*DataID_validPtr == TRUE)
  {
    /* Validate the wiretype of received member and check it is not duplicated */
    RetVal = SomeIpXf_CheckValidWiretypeAndDuplicateKnownMember(ConfigPtr, CfgElemHandledPtr,
        TLVContextPtr, ValidDataIdIndex, TLV_tag_header);
  }
#if(SOMEIPXF_TLV_STRUCT_MEMBER_REORDERING_ENABLE == STD_ON)
  /* Check that valid dataid is already found */
  else
  {
    /* Find the received data id from the stream
     * Start index: From the start again (0U)
     * End Index: Last valid dataid found Index (TLVDataIDProcessedCount)
     */
    SomeIpXf_ValidateReceivedDataId(ConfigPtr, DataID_validPtr, TLVContextPtr, &ValidDataIdIndex,
        0U, TLVDataIDProcessedCount, TLV_tag_dataID);

    /* Check if received member is found in the config */
    if(*DataID_validPtr == TRUE)
    {
      /* Validate the wiretype of received member and check it is not duplicated */
      RetVal = SomeIpXf_CheckValidWiretypeAndDuplicateKnownMember(ConfigPtr, CfgElemHandledPtr,
          TLVContextPtr, ValidDataIdIndex, TLV_tag_header);
    }
  }
#endif /* (SOMEIPXF_TLV_STRUCT_MEMBER_REORDERING_ENABLE == STD_ON) */

 return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_TLV_SetUpContext (
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
   uint8 RetVal = E_OK;

  /* Check it is first or inital level level */
  if(TRUE == TLVContextPtr->TLV_Root)
  {
    TLVContextPtr->TLV_Root = FALSE;
  }
  else
  {
    /* Check next level is within the specified range */
    /* CHECK: NOPARSE */
    if((TLVContextPtr->TLV_Current_NestedLevel + 1U) >= SOMEIPXF_MAX_NESTED_TLV_DEPTH)
    {
      RetVal = E_SER_GENERIC_ERROR;
    }
    else
    /* CHECK: PARSE */
    {
      /* Increment the nested level */
      TLVContextPtr->TLV_Current_NestedLevel++;
    }
  }

  /* CHECK: NOPARSE */
  if(E_OK == RetVal)
  /* CHECK: PARSE */
  {

    SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied = DeSerContextPtr->BytesCopied;
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;

    /* mark position of LF in serialized stream based on current nested level*/
    SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(PositionofLF) = BytesCopied;

    /* set static LF byte size */
    SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLV_static_lenghtfield_bytesize) = ConfigPtr[CfgElemHandled + 2U];

    /* set config pos of NW Endianness for LF based on LFSize */
    RetVal = SomeIpXf_Set_ConfigPosNWEndianness(&SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(EndinessPosition),
        SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLV_static_lenghtfield_bytesize));

    /* shift BytesCopied according to static LF byte size */
    BytesCopied = (SOMEIPXF_XFRMBUFFERLENGTH_TYPE)(BytesCopied +
            SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLV_static_lenghtfield_bytesize));

    /* get amount of DataIDs for current TLV struct */
    SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLVDataIDCount) = ConfigPtr[CfgElemHandled +
                                                        SOMEIPXF_STRUCT_TLV_CONTEXT_TAG_SIZE + 1U];

    /* get amount of fixed member for current TLV struct */
    SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(FixMemCount) = ConfigPtr[CfgElemHandled +
                                               SOMEIPXF_STRUCT_TLV_FIXED_MEMBER_COUNT_OFFSET + 1U];

    /* jump over TLV struct context and set CfgElemHandled to next TLV member */
    CfgElemHandled = CfgElemHandled + SOMEIPXF_STRUCT_TLV_CONTEXT_TAG_SIZE +
        (SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLVDataIDCount)*2U) + 1U;

    DeSerContextPtr->BytesCopied = BytesCopied;
    DeSerContextPtr->CfgElemHandled = CfgElemHandled;
  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_TLV_Opt_Member_SetTLVTag (
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
   uint8 RetVal = E_OK;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE OffsetofavailabilityBitfield = ConfigPtr[CfgElemHandled + 4U];
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE OptionalMemberIndex_AVB = ConfigPtr[CfgElemHandled + 5U];

  /* Byte position of availability field
   * DataByteoffset is added to get position of availability byte in array of struct
   */
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE byte_pos = (SOMEIPXF_XFRMBUFFERLENGTH_TYPE)
      ((OptionalMemberIndex_AVB / 8U) + OffsetofavailabilityBitfield + DeSerContextPtr->DataByteOffset);

  /* Bit position in the byte field */
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE bit_pos = (SOMEIPXF_XFRMBUFFERLENGTH_TYPE)(OptionalMemberIndex_AVB % 8U);

  /* check if member shall be serialized  */
  if(SomeIpXf_IsAvailable_TLV(DataBytePtr, byte_pos, bit_pos) == TRUE)
  {
    uint16 Wiretype = (uint16)ConfigPtr[CfgElemHandled + 2U];
    uint16 DataId = (uint16)ConfigPtr[CfgElemHandled + 3U];
    /* set TLV_tag_header (WireType + DataID) within Buffer */
    SOMEIPXF_WRITE_TLV_TAG(DeSerContextPtr->BytesCopied, (BufPtr), Wiretype, DataId);

    /* jump over and set CfgElemHandled to serialize TLV data member */
    CfgElemHandled = CfgElemHandled + SOMEIPXF_STRUCT_TLV_OPT_MEMBER_CONTEXT_TAG_SIZE + 1U;
  }
  else
  {
    const SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfigArraySize = ConfigPtr[1U];
    const SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfigTLVOptMemStopIdx = ConfigPtr[CfgElemHandled + 1U];

    /* Check the member stop index less than array size and it also points to
     * SOMEIPXF_STRUCT_TLV_MEMBER_OPT_STOP tag
     */
    /* CHECK: NOPARSE */
    if(((ConfigPtr[ConfigTLVOptMemStopIdx]) != SOMEIPXF_STRUCT_TLV_MEMBER_OPT_STOP) ||
      (ConfigTLVOptMemStopIdx >= ConfigArraySize))
    {
       RetVal = E_SER_GENERIC_ERROR;
    }
    else
    /* CHECK: PARSE */
    {
      /* Skip the current element and set config to the member stop */
      CfgElemHandled = ConfigPtr[CfgElemHandled + 1U];
    }
  }

  DeSerContextPtr->CfgElemHandled = CfgElemHandled;
  return RetVal;
}

STATIC FUNC(void, SOMEIPXF_CODE) SomeIpXf_TLV_Fix_Member_SetTLVTag (
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;

  const uint16 Wiretype = (uint16)ConfigPtr[CfgElemHandled + 1U];
  const uint16 DataId = (uint16)ConfigPtr[CfgElemHandled + 2U];

  SOMEIPXF_WRITE_TLV_TAG(DeSerContextPtr->BytesCopied, (BufPtr), Wiretype, DataId);

  /* Decrement the fixed member count and it shall be zero before tlv stop for current nested level */
  SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(FixMemCount)--;

  /* jump over  and set CfgElemHandled to serialize TLV data member */
  CfgElemHandled = CfgElemHandled + SOMEIPXF_STRUCT_TLV_MEMBER_CONTEXT_TAG_SIZE + 1U;

  DeSerContextPtr->CfgElemHandled = CfgElemHandled;
}



STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_TlvStop (
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE MembIdx_NonOptDataIDs = SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(MembIdx);
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE NumNonOptDataIDs = SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(FixMemCount);

  /* valid DataID found, fixed member is missing within TLV struct */
  if(NumNonOptDataIDs != MembIdx_NonOptDataIDs)
  {
    RetVal = E_SER_MALFORMED_MESSAGE;
  }
  else
  {
    /* Reset the values for current nested level */
    /* Ext member of current nested level will be used again
     * Ext member is set once by optional or fixed member
     * And it will be used by the TLV start again in case of array of struct
     * Reset the next nested level as it will be set by the optional or fixed member
     */
    /* Deviation TASKING-1 */
    SOMEIPXF_CLEAR_StructWithTLVContext_StructWithTLV_InfoType(TLVContextPtr->TLV_Current_NestedLevel);

    if(TLVContextPtr->TLV_Current_NestedLevel > 0U)
    {
      /* decrease NestedLevel */
      TLVContextPtr->TLV_Current_NestedLevel--;
    }
    else
    {
      TLVContextPtr->TLV_Root = TRUE;
    }
  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_TlvStop (
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  /* Check all fixed members are processed */
  /* CHECK: NOPARSE */
  if(SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(FixMemCount) != 0U)
  {
    /* Return the malformed message as all fix member are not processed */
    RetVal = E_SER_MALFORMED_MESSAGE;
  }
  /*All fixed members always be processed */
  else
  /* CHECK: PARSE */
  {
    RetVal = SomeIpXf_Set_TLV_lenghtfield(BufPtr, ConfigPtr, TLVContextPtr, BytesCopied);

    if(TLVContextPtr->TLV_Current_NestedLevel > 0U)
    {
      /* decrease NestedLevel */
      TLVContextPtr->TLV_Current_NestedLevel--;
    }
    else
    {
      TLVContextPtr->TLV_Root = TRUE;
    }
  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE)  SomeIpXf_TlvSkipCurrentDataId(
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BytesCopiedPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) CfgElemHandledPtr
)
{
  /* DataID not found */
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = *CfgElemHandledPtr;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied = *BytesCopiedPtr;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLV_static_lenghtfield_bytesize = 0U;
  uint32 DataMemSize = 0U;
  uint16 TLV_tag_header;
  uint16 TLV_tag_WireType;

  /* Get the tlv tag value */
  SOMEIPXF_GET_TLV_TAG_VALUE(BufPtr, TLV_tag_header, 0U, BytesCopied);
  /* Get the wiretype from value */
  SOMEIPXF_GET_TLV_TAG_WIRETYPE(TLV_tag_header, TLV_tag_WireType);

  if(TLV_tag_WireType < 4U)
  {
    /* Get the size of member depending on wire type */
    SOMEIPXF_GET_TLV_DATAMEM_SIZE_WIRETYPE0123(TLV_tag_WireType, (DataMemSize));
    TLV_static_lenghtfield_bytesize = 0U;
  }
  else
  {
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfigPosNWEndianness = 0U;

    if(TLV_tag_WireType == 0x04U)
    {
      TLV_static_lenghtfield_bytesize = SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLV_static_lenghtfield_bytesize);
      ConfigPosNWEndianness = SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(EndinessPosition);
    }
    else
    {
      /* Get the Endianness position and LF byte size */
     RetVal = SomeIpXf_Get_TLV_LFSize_And_EndianessPos(TLV_tag_WireType, &TLV_static_lenghtfield_bytesize,
          &ConfigPosNWEndianness);
    }

    /* CHECK: NOPARSE */
    if(E_OK == RetVal)
    /* CHECK: PARSE */
    {

      if( BufferLength < ((uint32)BytesCopied + SOMEIPXF_HEADER_LENGTH +
          (uint32)TLV_static_lenghtfield_bytesize + SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE) )
      {
        /* bufferlength is less then expected (LF_BufPos + LFsize + TLV tag size) */
        RetVal |= E_SER_MALFORMED_MESSAGE;
      }
      else
      {
        /* extract value out of LF based on size */
        RetVal |= SomeIpXf_GetLFValues(
            BufPtr,
            &ConfigPtr[0U],
            &DataMemSize,
            (BytesCopied + SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE),
            ConfigPosNWEndianness,
            TLV_static_lenghtfield_bytesize
            );
      }
    }
  }

  if(E_OK == RetVal)
  {
    if(DataMemSize > ((uint32)BufferLength - (uint32)BytesCopied - SOMEIPXF_HEADER_LENGTH -
             (uint32)TLV_static_lenghtfield_bytesize - SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE))
    {
      /* LF value is larger then expected (bufferlength) */
      RetVal |= E_SER_MALFORMED_MESSAGE;
    }
    else
    {
      /* overall TLV struct top level LF value */
      uint32 TLVStruct_LFsize = SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLVStruct_LFsize);

      /* shift BytesCopied according to wireType to next TLV tag of member */
      BytesCopied = (BytesCopied + SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE + TLV_static_lenghtfield_bytesize +
          (SOMEIPXF_XFRMBUFFERLENGTH_TYPE)DataMemSize);

      /* First element of tlv srtuct is processed
       * Set the flag to true so, that next time LF is not calculated for the current struct
       */
      TLVContextPtr->TLV_Struct_Deserial_FirstElement = TRUE;
      /* First element of nested tlv srtuct is processed
       * Set the flag to false so, that next time LF is not calculated for the current struct
       */
      SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(ExtElement) = FALSE;

      /* Check all elements are checked and reached the LFValue*/
      if(BytesCopied == TLVStruct_LFsize)
      {
        /* set CfgElemHandled to next TLV stop tag
         * all bytes are skipped as specified in LF the current tlv struct
         */
        CfgElemHandled = SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(PositionofStopTlvTag);
      }
      else if(BytesCopied > TLVStruct_LFsize)
      {
        RetVal |= E_SER_MALFORMED_MESSAGE;
      }
      else if( BufferLength < ((uint32)BytesCopied + SOMEIPXF_HEADER_LENGTH +
          SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE) )
      {
        /* Bufferlength is less than BytesCopied + headerlength + wiretype size
         * In this case dataid cannot be read correctly as there will be overflow
         */
        RetVal |= E_SER_MALFORMED_MESSAGE;
      }
      else /*BytesCopied < LFValue*/
      {
          /* empty else */
      }
      /* set CfgElemHandled for current tag -> CfgElemHandled - 1U; in order to enter again and check for new
       * DataID
       * CfgElemHandled handle will be incremented by 1 by general transformer
       */
      CfgElemHandled = CfgElemHandled - 1U;

      *CfgElemHandledPtr = CfgElemHandled;
      *BytesCopiedPtr  = BytesCopied;
    }
  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_InvTLVSetContext(
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) CfgElemHandledPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) TLV_LFSize_WireType567
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = *CfgElemHandledPtr;

  /* for root keeps nested level 0 */
  if(TRUE == TLVContextPtr->TLV_Root)
  {
    TLVContextPtr->TLV_Root = FALSE;
  }
  else
  {
    /*Check for the max nested level supported */
    /* CHECK: NOPARSE */
    if((TLVContextPtr->TLV_Current_NestedLevel + 1U) >= SOMEIPXF_MAX_NESTED_TLV_DEPTH)
    {
      RetVal = E_SER_GENERIC_ERROR;
    }
    else
    /* CHECK: PARSE */
    {
      /*Increment the nested level */
      TLVContextPtr->TLV_Current_NestedLevel++;
    }
  }

  /* first valid TLV member found in current nested level-> save TLV struct context info */
  /* CHECK: NOPARSE */
  if(E_OK == RetVal)
  /* CHECK: PARSE */
  {
    const SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfigArraySize = ConfigPtr[1U];
    /* Position of TLV stop */
    SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(PositionofStopTlvTag) = ConfigPtr[CfgElemHandled + 1U];

    /* CHECK: NOPARSE */
    /* check if TLV stop index pos is within config array size
     * Also stop index points to SOMEIPXF_STRUCT_TLV_STOP tag
     */
    if((SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(PositionofStopTlvTag) > ConfigArraySize) ||
        (ConfigPtr[SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(PositionofStopTlvTag)] != SOMEIPXF_STRUCT_TLV_STOP))
    {
      /* TLV stop index pos is invalid */
      RetVal = E_SER_GENERIC_ERROR;
    }
    else
    /* CHECK: PARSE */
    {
      if(0U == *TLV_LFSize_WireType567)
      {
        /* set static LF byte size */
        SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLV_static_lenghtfield_bytesize) = ConfigPtr[CfgElemHandled + 2U];
      }
      else
      {
        SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLV_static_lenghtfield_bytesize) = *TLV_LFSize_WireType567;
        *TLV_LFSize_WireType567 = 0U;
      }

      /* set endianness position based on Lf size */
      RetVal = SomeIpXf_Set_ConfigPosNWEndianness(&SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(EndinessPosition),
                  SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLV_static_lenghtfield_bytesize));

      /*Reset the MembIdx of given level based on TLV_Current_NestedLevel*/
      SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(MembIdx) = 0U;

      /* save amount of non optional members for the current nested level */
      SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(FixMemCount) = ConfigPtr[CfgElemHandled + 5U];

      /* save over all amount of members for the current nested level */
      SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLVDataIDCount) = ConfigPtr[CfgElemHandled +
                                                        SOMEIPXF_STRUCT_TLV_CONTEXT_TAG_SIZE + 1U];

      /* save SomeIpXf cfg pos of DataID list start for the current nested level */
      SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLVDataIDStartCfgHandleId) =
          CfgElemHandled + 1U + SOMEIPXF_STRUCT_TLV_CONTEXT_TAG_AND_LF_SIZE;
    }
  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_TlvStart (
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied = DeSerContextPtr->BytesCopied;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLV_LFSize_WireType567 = DeSerContextPtr->TLV_LFSize_WireType567;
  uint32 LFValue = 0U;
  boolean DataID_valid = FALSE;

  /* Clear availability bit */
  SOMEIPXF_CLEAR_AVAILABLITY_BIT_FIELD(DataBytePtr);

  /* Retrieve the LF value only once
   * During 2nd iteration LF shall not be calculated only Data ID's checked
   */
  if( /* SetUpContext if ExtElement holds TLV struct (TLV_Current_NestedLevel not increased yet) */
      (TRUE == TLVContextPtr->TLV_Info_LEVEL[TLVContextPtr->TLV_Current_NestedLevel + 1U].ExtElement) ||
      /* SetUpContext for the first element only once (first element not handled yet) */
      (FALSE == TLVContextPtr->TLV_Struct_Deserial_FirstElement))
  {
    /* Set the config values for current nested level  */
    RetVal = SomeIpXf_InvTLVSetContext(
        &ConfigPtr[0U],
        TLVContextPtr,
        &CfgElemHandled,
        &TLV_LFSize_WireType567
        );

    /* CHECK: NOPARSE */
    if(E_OK == RetVal)
    /* CHECK: PARSE */
    {

      if( BufferLength < ((uint32)BytesCopied + SOMEIPXF_HEADER_LENGTH +
          (uint32)SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLV_static_lenghtfield_bytesize)) )
      {
        /* bufferlength is less then expected (LF_BufPos + LFsize) */
        RetVal = E_SER_MALFORMED_MESSAGE;
      }
      else
      {
        /* Get the length field from of the tlv struct starting at the BytesCopied position */
        RetVal |= SomeIpXf_GetLFValues(
            BufPtr,
            &ConfigPtr[0U],
            &LFValue,
            BytesCopied,
            SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(EndinessPosition),
            SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLV_static_lenghtfield_bytesize)
            );

        /* CHECK: NOPARSE */
        if(E_OK == RetVal)
        /* CHECK: PARSE */
        {
          if( LFValue > ((uint32)BufferLength - (uint32)BytesCopied - SOMEIPXF_HEADER_LENGTH -
              (uint32)SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLV_static_lenghtfield_bytesize)) )
          {
            /* LF value is larger then expected (bufferlength) */
            RetVal = E_SER_MALFORMED_MESSAGE;
          }
          else
          {
            /* Total number of bytes to be de-serialized */
            SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLVStruct_LFsize) =
                LFValue + SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLV_static_lenghtfield_bytesize) + BytesCopied;

            /* Increment the bytecopied by the LF size */
            BytesCopied = BytesCopied + SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLV_static_lenghtfield_bytesize);
          }
        }
      }

    }
  }

  if(E_OK == RetVal)
  {
    if(  /* LFValue is not zero else set TLV stop idx */
         (0U != LFValue) ||
         /* first element still handled (DataID not found) */
         (TRUE == TLVContextPtr->TLV_Struct_Deserial_FirstElement))
    {

      /* check if DataID and wire type is valid and member is within SomeIpXf config list */
      RetVal = SomeIpXf_Int_DataID_WireType_Valid(
          BufPtr,
          BytesCopied,
          &ConfigPtr[0U],
          &DataID_valid,
          &CfgElemHandled,
          TLVContextPtr
          );

        if(E_OK == RetVal)
        {
          /* Data id not found in stream */
          if(FALSE == DataID_valid)
          {
            /* DataID not found */
            RetVal = SomeIpXf_TlvSkipCurrentDataId(
                BufPtr,
                BufferLength,
                &BytesCopied,
                &ConfigPtr[0U],
                TLVContextPtr,
                &CfgElemHandled
                );
        }
      }
    }
    else
    {
      /* set CfgElemHandled to next nested TLV stop tag
       * CfgElemHandled will be incremented by 1 by general transformer
       */
      CfgElemHandled = SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(PositionofStopTlvTag) - 1U;
    }
  }
  else
  {
    /* set CfgElemHandled to next nested TLV stop tag
     * CfgElemHandled will be incremented by 1 by general transformer
     */
    CfgElemHandled = SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(PositionofStopTlvTag) - 1U;
  }

  DeSerContextPtr->CfgElemHandled = CfgElemHandled;
  DeSerContextPtr->BytesCopied = BytesCopied;
  DeSerContextPtr->TLV_LFSize_WireType567 = TLV_LFSize_WireType567;

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_TLV_Opt_Member_Start (
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  /* Initalize to invalid wiretype */
  uint8 Tag_WireType = 8U;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied = DeSerContextPtr->BytesCopied;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLV_LFSize_WireType567 = DeSerContextPtr->TLV_LFSize_WireType567;

  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE OffsetofavailabilityBitfield = ConfigPtr[CfgElemHandled + 4U];
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE OptionalMemberIndex_AVB = ConfigPtr[CfgElemHandled + 5U];

  /*Get the byte position in current availabilityBitfield*/
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE byte_pos = (SOMEIPXF_XFRMBUFFERLENGTH_TYPE)
      ((OptionalMemberIndex_AVB / 8U) + OffsetofavailabilityBitfield + DeSerContextPtr->DataByteOffset);
  /* Bit position in the byte field */
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE bit_pos = (SOMEIPXF_XFRMBUFFERLENGTH_TYPE)(OptionalMemberIndex_AVB % 8U);

  /* Wire type in the tag */
  SOMEIPXF_GETWIRETYPE(Tag_WireType, BytesCopied, BufPtr);

  /* shift BytesCopied according to TLV tag size */
  BytesCopied = BytesCopied + SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE;

  /*check if member is not of wire type 0,1,2
   * If member is not complex data type then directly go for deserialization
   */
  if(Tag_WireType >= 0x04U)
  {
    /* update the lenghtfield_bytesize for wiretype is 5,6,7 */
    if(Tag_WireType > 0x04U)
    {
      RetVal = SomeIpXf_GetLFByteSize(&TLV_LFSize_WireType567, Tag_WireType);
    }

    /* CHECK: NOPARSE */
    if((TLVContextPtr->TLV_Current_NestedLevel + 1U) >= SOMEIPXF_MAX_NESTED_TLV_DEPTH)
    {
      RetVal = E_SER_GENERIC_ERROR;
    }
    else
    /* CHECK: PARSE */
    {
      /*Set the extensible element as true */
      TLVContextPtr->TLV_Info_LEVEL[TLVContextPtr->TLV_Current_NestedLevel + 1U].ExtElement = TRUE;
    }
  }

  /* set bit in availabilityBitfield */
  SomeIpXf_SetAvailable_TLV(DataBytePtr, byte_pos, bit_pos);

  /* jump over and set CfgElemHandled to deserialize optional TLV data member */
  CfgElemHandled =
    (SOMEIPXF_XFRMBUFFERLENGTH_TYPE)(CfgElemHandled +
     SOMEIPXF_STRUCT_TLV_OPT_MEMBER_CONTEXT_TAG_SIZE + 1U);

  DeSerContextPtr->BytesCopied = BytesCopied;
  DeSerContextPtr->CfgElemHandled = CfgElemHandled;
  DeSerContextPtr->TLV_LFSize_WireType567 = TLV_LFSize_WireType567;

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_TLV_Member_Stop (
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied = DeSerContextPtr->BytesCopied;
  uint32 TLVStruct_LFsize;

  /*Get the total length field of nested structure */
  TLVStruct_LFsize = SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLVStruct_LFsize);

  /* If byte stream already reached end of struct for current nested level
   * check all data is de-serialzed received with respect to LF
   */
  if(BytesCopied >= TLVStruct_LFsize)
  {
    /* set CfgElemHandled to next TLV stop tag */
    CfgElemHandled = SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(PositionofStopTlvTag);
    /* LF was less than the expected data */
    if(BytesCopied > TLVStruct_LFsize)
    {
      RetVal = E_SER_MALFORMED_MESSAGE;
    }
    /* decrease once since general transformer will increase again */
    CfgElemHandled = CfgElemHandled - 1U;
  }
  else if( BufferLength < ((uint32)BytesCopied + SOMEIPXF_HEADER_LENGTH +
      SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE) )
  {
    /* bufferlength is less then expected (BytesCopied + SOMEIPXF_HEADER_LENGTH + TLV tag size)
     * Bytes processed has reached the maximum buffer received
     */
      RetVal = E_SER_MALFORMED_MESSAGE;
  }
  else
  {
    /* get next DataID and continue with deserializing */
    boolean DataID_valid = FALSE;

    /* read DataID of stream and check if this is within list */
    RetVal = SomeIpXf_Int_DataID_WireType_Valid(
        BufPtr,
        BytesCopied,
        ConfigPtr,
        &DataID_valid,
        &CfgElemHandled,
        TLVContextPtr
        );

    if(E_OK == RetVal)
    {
      /* Data id not found in stream */
      if(FALSE == DataID_valid)
      {
        RetVal = SomeIpXf_Inv_Get_NextDataIdPos(
            &ConfigPtr[0U],
            BufferLength,
            BufPtr,
            &BytesCopied,
            TLVContextPtr
            );

       /* set CfgElemHandled for current tag -> CfgElemHandled - 1U; in order to enter again and check for new
        * DataID */
       CfgElemHandled--;
      }
    }
  }

  DeSerContextPtr->BytesCopied = BytesCopied;
  DeSerContextPtr->CfgElemHandled = CfgElemHandled;

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_Get_NextDataIdPos (
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BytesCopiedPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied = *BytesCopiedPtr;
  uint32 DataMemSize = 0U;
  uint16 TLV_tag_header;
  uint16 TLV_tag_WireType;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLV_static_lenghtfield_bytesize = 0U;

  SOMEIPXF_GET_TLV_TAG_VALUE(BufPtr, TLV_tag_header, 0U, BytesCopied);
  SOMEIPXF_GET_TLV_TAG_WIRETYPE(TLV_tag_header, TLV_tag_WireType);

  if(TLV_tag_WireType < 4U)
  {
    /* Basic data type */
    SOMEIPXF_GET_TLV_DATAMEM_SIZE_WIRETYPE0123(TLV_tag_WireType, (DataMemSize));
    TLV_static_lenghtfield_bytesize = 0U;
  }
  else
  {
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfigPosNWEndianness = 0U;

    if(TLV_tag_WireType == 0x04U)
    {
      TLV_static_lenghtfield_bytesize = SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(TLV_static_lenghtfield_bytesize);
      ConfigPosNWEndianness = SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(EndinessPosition);
    }
    else
    {
      /* Get the Endianness position and LF byte size */
     RetVal = SomeIpXf_Get_TLV_LFSize_And_EndianessPos(TLV_tag_WireType, &TLV_static_lenghtfield_bytesize, &ConfigPosNWEndianness);
    }

    /* CHECK: NOPARSE */
    if(E_OK == RetVal)
    /* CHECK: PARSE */
    {

      if( BufferLength < ((uint32)BytesCopied + SOMEIPXF_HEADER_LENGTH +
          (uint32)TLV_static_lenghtfield_bytesize + SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE) )
      {
        /* bufferlength is less then expected (LF_BufPos + LFsize + TLV tag size) */
        RetVal = E_SER_MALFORMED_MESSAGE;
      }
      else
      {
        /* extract value out of LF based on size */
        RetVal |= SomeIpXf_GetLFValues(
            BufPtr,
            &ConfigPtr[0U],
            &DataMemSize,
            (BytesCopied + SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE),
            ConfigPosNWEndianness,
            TLV_static_lenghtfield_bytesize
            );
      }

    }
  }

  /* Sanity check */
  /* CHECK: NOPARSE */
  if(E_OK == RetVal)
  /* CHECK: PARSE */
  {
    if( DataMemSize > ((uint32)BufferLength - (uint32)BytesCopied - SOMEIPXF_HEADER_LENGTH -
        (uint32)TLV_static_lenghtfield_bytesize - SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE) )
    {
      /* LF value is larger then expected (bufferlength) */
      RetVal = E_SER_MALFORMED_MESSAGE;
    }
    else
    {
      /* shift BytesCopied according to wireType to next TLV tag of member */
      BytesCopied = (SOMEIPXF_XFRMBUFFERLENGTH_TYPE)(BytesCopied + SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE
          + TLV_static_lenghtfield_bytesize + (SOMEIPXF_XFRMBUFFERLENGTH_TYPE)DataMemSize);
    }

    *BytesCopiedPtr = BytesCopied;
  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_TLV_Fix_Member_Start (
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopied = DeSerContextPtr->BytesCopied;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLV_LFSize_WireType567 = DeSerContextPtr->TLV_LFSize_WireType567;

  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  uint8 Tag_WireType;

  /* Extract the wire type */
  SOMEIPXF_GETWIRETYPE(Tag_WireType, BytesCopied, BufPtr);

  /* shift BytesCopied according to TLV tag size */
  BytesCopied = BytesCopied + SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE;

  /* Check member is primitive data type or complex */
  if(Tag_WireType >= 0x04U)
  {
    /* update the lenghtfield_bytesize for wiretype is 5,6,7 */
    if(Tag_WireType > 0x04U )
    {
      /* Set the LF size depending on received wire type */
      RetVal = SomeIpXf_GetLFByteSize(&TLV_LFSize_WireType567, Tag_WireType);
    }
    /* CHECK: NOPARSE */
    if((TLVContextPtr->TLV_Current_NestedLevel + 1U) >= SOMEIPXF_MAX_NESTED_TLV_DEPTH)
    {
      RetVal = E_SER_GENERIC_ERROR;
    }
    else
    /* CHECK: PARSE */
    {
      /*Set the extensible element as true */
      TLVContextPtr->TLV_Info_LEVEL[TLVContextPtr->TLV_Current_NestedLevel + 1U].ExtElement = TRUE;
    }
  }

  /* jump over and set CfgElemHandled to deserialize TLV data member */
  CfgElemHandled = CfgElemHandled + SOMEIPXF_STRUCT_TLV_MEMBER_CONTEXT_TAG_SIZE + 1U;

  /* increase MembIdx of given level based on TLV_Struct_NestedLevel */
  SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(MembIdx) = SOMEIPXF_TLV_STRUCT_WITH_INFO_TYPE(MembIdx) + 1U;

  DeSerContextPtr->CfgElemHandled = CfgElemHandled;
  DeSerContextPtr->BytesCopied = BytesCopied;
  DeSerContextPtr->TLV_LFSize_WireType567 = TLV_LFSize_WireType567;

  return RetVal;
}

FUNC(void, SOMEIPXF_CODE) SomeIpXf_Init_TLVContext
(
   P2VAR(TLV_Nested_Type, AUTOMATIC, SOMEIPXF_APPL_DATA) TLVContextPtr
)
{
  uint32 info_level_idx = 0U;
  for(info_level_idx = 0U; info_level_idx < SOMEIPXF_MAX_NESTED_TLV_DEPTH; info_level_idx++)
  {
    /* Deviation TASKING-1 */
    SOMEIPXF_CLEAR_TLVContext_TLV_InfoType(info_level_idx);
  }
}
#endif /* (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_ON) */

#if (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON)
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_GetNextArg(
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(uint32, AUTOMATIC, SOMEIPXF_APPL_DATA) TLV_ARG_BufPosPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLV_static_LF_bytesize_Index,
  uint16 TLV_tag_WireType
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLV_static_lenghtfield_bytesize = 0U;
  uint32 DataArgSize = 0U;

  if(TLV_tag_WireType < 0x04U)
  {
    SOMEIPXF_GET_TLV_DATAMEM_SIZE_WIRETYPE0123(TLV_tag_WireType, (DataArgSize));
    TLV_static_lenghtfield_bytesize = 0U;
  }
  else
  {
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfigPosNWEndianness = 0U;

    if(TLV_tag_WireType == 0x04U)
    {
      /* Set the LF byte size */
      TLV_static_lenghtfield_bytesize = (SOMEIPXF_XFRMBUFFERLENGTH_TYPE) ConfigPtr[TLV_static_LF_bytesize_Index];
      /* set config pos of NW Endianness for LF based on LFSize */
      RetVal = SomeIpXf_Set_ConfigPosNWEndianness(&ConfigPosNWEndianness, TLV_static_lenghtfield_bytesize);
    }
    else
    {
      RetVal = SomeIpXf_Get_TLV_LFSize_And_EndianessPos(TLV_tag_WireType, &TLV_static_lenghtfield_bytesize, &ConfigPosNWEndianness);
    }

    /* CHECK: NOPARSE */
    if(E_OK == RetVal)
    /* CHECK: PARSE */
    {
      if( BufferLength < (*TLV_ARG_BufPosPtr +
          SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE + (uint32)TLV_static_lenghtfield_bytesize) )
      {
        /* bufferlength is less then expected (LF_BufPos + LFsize) */
        RetVal = E_SER_MALFORMED_MESSAGE;
      }
      else
      {
        /* extract value out of LF based on size */
        RetVal |= SomeIpXf_GetLFValues(
            BufPtr,
            &ConfigPtr[0U],
            &DataArgSize,
            (DeSerContextPtr->BytesCopied + SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE),
            ConfigPosNWEndianness,
            TLV_static_lenghtfield_bytesize
        );
      }
    }
  }

  /* CHECK: NOPARSE */
  if(RetVal == E_OK)
    /* CHECK: PARSE */
  {

    /* Check bytes processed is not more than received buffer length */
    if( DataArgSize >= ((uint32)BufferLength - *TLV_ARG_BufPosPtr -
        SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE - (uint32)TLV_static_lenghtfield_bytesize) )
    {
      /* Bytes processed hits or exceeds buffer length -> malformed */
      RetVal = E_SER_MALFORMED_MESSAGE;
    }
    else
    {
      /* Total number of bytes processed */
      *TLV_ARG_BufPosPtr = *TLV_ARG_BufPosPtr + SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE +
          DataArgSize + (uint32)TLV_static_lenghtfield_bytesize;

      /* Store the position of next argument */
      DeSerContextPtr->BytesCopied = (SOMEIPXF_XFRMBUFFERLENGTH_TYPE)(DeSerContextPtr->BytesCopied +
          SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE +
          (SOMEIPXF_XFRMBUFFERLENGTH_TYPE)DataArgSize + TLV_static_lenghtfield_bytesize);

      /* SOMEIP_00218: check next DataID -> set back CfgElemHandled to
       * SOMEIPXF_ARG_TLV_START for current argument
       */
      DeSerContextPtr->CfgElemHandled = DeSerContextPtr->CfgElemHandled - 1U;
    }

  }

  return RetVal;
}

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_TLV_Arg_Find (
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(uint32, AUTOMATIC, SOMEIPXF_APPL_DATA) TLV_ARG_BufPosPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  uint16 TLV_tag_header_buf;
  uint16 TLV_tag_WireType_Buf;
  uint16 TLV_tag_WireType_conf;
  uint16 TLV_tag_dataID_buf;
  uint16 TLV_tag_dataID_conf;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLV_LFSize_WireType567 = DeSerContextPtr->TLV_LFSize_WireType567;

  /* Get wiretype of member from config */
  TLV_tag_WireType_conf = (uint16) ConfigPtr[DeSerContextPtr->CfgElemHandled + 1U];
  /* Get dataid of member from config */
  TLV_tag_dataID_conf = (uint16) ConfigPtr[DeSerContextPtr->CfgElemHandled + 2U];

  /* This check ensures that data id will not bre read from invalid address */
  if(BufferLength <= DeSerContextPtr->BytesCopied + SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE
      + SOMEIPXF_HEADER_LENGTH)
  {
    /* Bytescopied has reached the maximum length */
    RetVal = E_SER_MALFORMED_MESSAGE;
  }
  else
  {
    /* Get tag of member in byte stream */
      SOMEIPXF_GET_TLV_TAG_VALUE(BufPtr, TLV_tag_header_buf, 0U, DeSerContextPtr->BytesCopied);

      TLV_tag_header_buf = (uint16)(TLV_tag_header_buf & 0x7FFFU);
      /* Get wiretype of byte stream */
      SOMEIPXF_GET_TLV_TAG_WIRETYPE(TLV_tag_header_buf, TLV_tag_WireType_Buf);
      /* Get TLV DataID of byte stream */
      SOMEIPXF_GET_TLV_TAG_DATAID(TLV_tag_dataID_buf, TLV_tag_header_buf);

      /* Check received argument is same as of configured argument  */
      if(TLV_tag_dataID_conf == TLV_tag_dataID_buf)
      {
        /* Check configured wiretype is complex data type */
        if(TLV_tag_WireType_conf >= 4U)
        {
          /* Invalid wire type if it is less than 4 */
          if(TLV_tag_WireType_Buf < 4U)
          {
            RetVal = E_SER_MALFORMED_MESSAGE;
          }
          else
          {
            /* update the lenghtfield_bytesize for wiretype is 5,6,7 */
            if(TLV_tag_WireType_Buf > 0x04U)
            {
              RetVal = SomeIpXf_GetLFByteSize(&TLV_LFSize_WireType567, TLV_tag_WireType_Buf);

              DeSerContextPtr->TLV_LFSize_WireType567 = TLV_LFSize_WireType567;
            }

            /* PRS_SOMEIP_00217: DataID known */
            /* shift BytesCopied according to TLV tag size */
            DeSerContextPtr->BytesCopied = DeSerContextPtr->BytesCopied + SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE;
            /* set CfgElemHandled to de-serialize TLV record data type or basic data type */
            DeSerContextPtr->CfgElemHandled = DeSerContextPtr->CfgElemHandled + SOMEIPXF_ARG_TLV_CONTEXT_TAG_SIZE;
          }
        }
        else
        {
          /* If member is not complex data type then wire type shall match */
          if(TLV_tag_WireType_Buf == TLV_tag_WireType_conf)
          {
            /* PRS_SOMEIP_00217: DataID known */
            /* shift BytesCopied according to TLV tag size */
            DeSerContextPtr->BytesCopied = DeSerContextPtr->BytesCopied + SOMEIPXF_TLV_WIRETYPE_DATAID_SIZE;
            /* set CfgElemHandled to de-serialize TLV record data type or basic data type */
            DeSerContextPtr->CfgElemHandled = DeSerContextPtr->CfgElemHandled + SOMEIPXF_ARG_TLV_CONTEXT_TAG_SIZE;
          }
          else
          {
            /* Invalid wire type */
            RetVal = E_SER_MALFORMED_MESSAGE;
          }
        }
      }
      else
      {
        /* PRS_SOMEIP_00217: DataID unknown -> skip argument */

        const SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLV_static_LF_bytesize_Index = DeSerContextPtr->CfgElemHandled + 3U;

        RetVal = SomeIpXf_Inv_GetNextArg(
            BufPtr,
            &ConfigPtr[0U],
            TLV_ARG_BufPosPtr,
            DeSerContextPtr,
            BufferLength,
            TLV_static_LF_bytesize_Index,
            TLV_tag_WireType_Buf
            );

      }
  }


  return RetVal;
}

STATIC FUNC(void, SOMEIPXF_CODE) SomeIpXf_TLV_Arg_Set (
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr
)
{
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE CfgElemHandled = DeSerContextPtr->CfgElemHandled;

  const uint16 Wiretype = (uint16)ConfigPtr[CfgElemHandled + 1U];
  const uint16 DataId = (uint16)ConfigPtr[CfgElemHandled + 2U];

  /* Update the TLV header of arguments */
  SOMEIPXF_WRITE_TLV_TAG((DeSerContextPtr->BytesCopied), (BufPtr), Wiretype, DataId);
  /* Set CfgElemHandled to next Tag
   * Increment by TLV tag size and LF size
   */
  CfgElemHandled = CfgElemHandled + SOMEIPXF_ARG_TLV_CONTEXT_TAG_SIZE;

  DeSerContextPtr->CfgElemHandled = CfgElemHandled;
}

#if (SOMEIPXF_TLV_ARGUMENT_UNIQUE_DATAID_CHECK_ENABLE == STD_ON)
FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_CheckMultipleKnownDataIDs
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLVArgIDListSize,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) TLVArgID,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  SomeIpXf_DeSerInfoType DeSerContext;
  uint32 BytesProcessed = 0U;
  uint16 TLV_tag_header_buf;
  uint16 TLV_tag_WireType_Buf;
  uint16 TLV_tag_dataID_buf;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLVArgCount = 0U;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE Byteposition;
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLVArgProcessedCount;
  uint8 bitposition;
  uint8 Arguments_Processed[SOMEIPXF_MAX_ARGUMENT_TLV_IDS_BYTES];
  boolean DuplicateDataIdfound = FALSE;

  /* Initalize the arguments processed to zero  */
  for(TLVArgProcessedCount = 0U; (TLVArgProcessedCount < SOMEIPXF_MAX_ARGUMENT_TLV_IDS_BYTES);
      TLVArgProcessedCount++)
  {
    Arguments_Processed[TLVArgProcessedCount] = 0x00U;
  }

  /* Loop over all received TLV arguments and add to DataId_In_ReceivedFrame list. */
  /* CHECK: NOPARSE */
  while((BytesProcessed  < (SOMEIPXF_XFRMBUFFERLENGTH_TYPE)(BufferLength - SOMEIPXF_HEADER_LENGTH))
      && (RetVal == E_OK))
  /* CHECK: PARSE */
  {
    DeSerContext.BytesCopied = (SOMEIPXF_XFRMBUFFERLENGTH_TYPE) BytesProcessed;

    /* Get tag of member in byte stream */
    SOMEIPXF_GET_TLV_TAG_VALUE(BufPtr, TLV_tag_header_buf, 0U, BytesProcessed);
    TLV_tag_header_buf = (uint16)(TLV_tag_header_buf & 0x7FFFU);

    /* Get wiretype of byte stream */
    SOMEIPXF_GET_TLV_TAG_WIRETYPE(TLV_tag_header_buf, TLV_tag_WireType_Buf);
    /* Get TLV DataID of byte stream */
    SOMEIPXF_GET_TLV_TAG_DATAID(TLV_tag_dataID_buf, TLV_tag_header_buf);

    for(TLVArgCount = 0U; (TLVArgCount < TLVArgIDListSize); TLVArgCount++)
    {
      if(TLVArgID[TLVArgCount] == TLV_tag_dataID_buf)
      {
        /* Get the byte position of the TLVID */
        Byteposition = TLVArgCount / 8U;
         /* Get the bit position of the TLVID in the found byte */
        bitposition = TLVArgCount % 8U;

        /* Find duplicate dataid in receiver configuration */
        DuplicateDataIdfound = (boolean)((Arguments_Processed[Byteposition] &
             (1U << bitposition)) != 0U);

        if(DuplicateDataIdfound == TRUE)
        {
          /* Return malformed message for MultipleKnownDataIDs */
          RetVal = E_SER_MALFORMED_MESSAGE;
        }
        else
        {
          /* Set the position of the TLVID to 1 */
          Arguments_Processed[Byteposition] =
              (uint8)(Arguments_Processed[Byteposition] | (1U << bitposition));
        }

        /* Argument found */
        break;
      }
    }

    if(RetVal == E_OK)
    {
      /* PRS_SOMEIP_00217: DataID unknown -> skip argument */

      RetVal = SomeIpXf_Inv_GetNextArg(
          BufPtr,
          &ConfigPtr[0U],
          &BytesProcessed,
          &DeSerContext,
          BufferLength,
          SOMEIPXF_ARG_TLV_STATIC_LF_BYTE_SIZE_CONFIG_OFFSET,
          TLV_tag_WireType_Buf
          );
    }
  }

  return RetVal;
}
#endif /* (SOMEIPXF_TLV_ARGUMENT_UNIQUE_DATAID_CHECK_ENABLE == STD_ON) */
#endif /* (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_ON) */

/*--------- D e s e r i a l i z a t i o n   o f   S O M E / I P --------------*/

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserializeBasicType
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength
)
{
  uint8 RetVal = E_OK;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufPos = 0U; /* indicates position of serialized data */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfPos = 2U; /* neglects initial SOMEIPXF_BASIC_DATA_TYPE_START and
   SOMEIPXF_BASIC_DATA_TYPE_CONFIG */

#if ( (SOMEIPXF_VSA_AVAILABLE == STD_OFF) && \
    (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_OFF) && \
    (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_OFF) && \
    (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_OFF) && \
    (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_OFF) )
  /* Deviation MISRAC2012-3 */
  SOMEIPXF_PARAM_UNUSED(BufferLength);
#else
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BasicTypeSize = ConfigPtr[1U];
  /* BDT size check required due to previous possible relocation of the buffer pos by skipping behviour of an
   * ext. DT,VSA or TLV */
  if((DeSerContextPtr->BytesCopied + SOMEIPXF_HEADER_LENGTH + BasicTypeSize) > BufferLength)
  {
    /* LF skipping value was larger then expected (bufferlength) */
    RetVal = E_SER_MALFORMED_MESSAGE;
  }
  else
#endif /* ( (SOMEIPXF_VSA_AVAILABLE == STD_OFF) && \
    (SOMEIPXF_STRUCT_WITH_LF_AVAILABLE == STD_OFF) && \
    (SOMEIPXF_FSA_WITH_LF_AVAILABLE == STD_OFF) && \
    (SOMEIPXF_TLV_ARGUMENT_AVAILABLE == STD_OFF) && \
    (SOMEIPXF_TLV_STRUCT_AVAILABLE == STD_OFF) ) */
  {
    do{
      DataBytePtr[ConfigPtr[ConfPos] + DeSerContextPtr->DataByteOffset] = BufPtr[BufPos];
      BufPos++;
      ConfPos++;

      /* end of basis data type serialization reached */
    }while (ConfigPtr[ConfPos] != SOMEIPXF_BASIC_DATA_TYPE_STOP);

    /* updates the number of bytes serialized and the number of configuation elements handled */
    DeSerContextPtr->BytesCopied += BufPos;
    DeSerContextPtr->CfgElemHandled += ConfPos;
  }

  return RetVal;
}

#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)
STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_DeserializeFSAOfBasicType
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) DataBytePtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
   P2VAR(SomeIpXf_DeSerInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) DeSerContextPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength
)
{
  /* used for defensive programming only (no Det interface) */
  uint8 RetVal = E_OK;

  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufPos = 0U; /* position of serialized data */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfPos = 0U; /* position of data byte within array element */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE ConfPosOffset = 0U; /* position of array element within array */
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE NumOfArrayElem = ConfigPtr[1]; /* number of elements the array provides */
  const SOMEIPXF_XFRMBUFFERLENGTH_TYPE SizeOfElem = ConfigPtr[2]; /* size of a single array element */

  /* FSA of BDT size check required due to previous possible relocation of the buffer pos by skipping behviour of an
   * ext. DT, VSA or TLV */
  if((DeSerContextPtr->BytesCopied + SOMEIPXF_HEADER_LENGTH + (NumOfArrayElem * SizeOfElem)) > BufferLength)
  {
    /* LF skipping value was larger then expected (bufferlength) */
    RetVal = E_SER_MALFORMED_MESSAGE;
  }
  else
  {
    do{
      ConfPos = 5U; /* reset for array handling, the first data element position */

      do{
        DataBytePtr[ConfigPtr[ConfPos] + ConfPosOffset + DeSerContextPtr->DataByteOffset] = BufPtr[BufPos];
        BufPos++;
        ConfPos++;

        /* end of array element serialization reached */
      }while (ConfigPtr[ConfPos] != SOMEIPXF_BASIC_DATA_TYPE_STOP);

      ConfPosOffset += SizeOfElem;
      NumOfArrayElem--;

      /* end of array serialization reached */
    } while (NumOfArrayElem > 0U);

    /* update number of bytes serialized and number of configuation elements handled */
    /* handle end tag SOMEIPXF_FSA_OF_BASIC_TYPE_STOP */
    ConfPos += 1U;  /* add SOMEIPXF_FSA_OF_BASIC_TYPE_STOP */
    DeSerContextPtr->BytesCopied += BufPos;
    DeSerContextPtr->CfgElemHandled += ConfPos;
  }

  return RetVal;
}
#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */

#if (SOMEIPXF_ET_AVAILABLE == STD_ON)

STATIC FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_ET_GetHeader
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   uint8 InterfaceVersion
)
{
   uint8 RetVal;

   /* header of external trigger communication serialized with
      ProtocolVersion   - SOMEIPXF_PROTOCOL_VERSION, (generated macro)
      InterfaceVersion  - InterfaceVersion (input, generated to API)
      MessageType       - SOMEIPXF_MESSAGE_TYPE_REQUEST_NO_RETURN (SWS_SomeIpXf_00031,
                                                                    Bugzilla RfC 67584)
      ReturnCode        - by ReturnValue initialized,
                          updated if error by SomeIpXf (SWS_SomeIpXf_00115) */
   if (BufPtr[4] != SOMEIPXF_PROTOCOL_VERSION)
   {
     RetVal = E_SER_WRONG_PROTOCOL_VERSION;
   }
   else if (BufPtr[5] != InterfaceVersion)
   {
     RetVal = E_SER_WRONG_INTERFACE_VERSION;
   }
   else if (BufPtr[6] != SOMEIPXF_MESSAGE_TYPE_REQUEST_NO_RETURN)
   {
     RetVal = E_SER_WRONG_MESSAGE_TYPE;
   }
   else if (BufPtr[7] != 0x00U)
   {
     RetVal = E_SER_GENERIC_ERROR;
   }
   else
   {
     RetVal = E_OK;
   }

   return RetVal;
}
#endif /* (SOMEIPXF_ET_AVAILABLE == STD_ON) */


#define SOMEIPXF_STOP_SEC_CODE
#include <SomeIpXf_MemMap.h>

/*==================[end of file]===========================================*/
