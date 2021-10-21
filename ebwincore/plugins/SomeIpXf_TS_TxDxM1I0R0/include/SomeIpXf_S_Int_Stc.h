#ifndef SOMEIPXF_S_INT_STC_H
#define SOMEIPXF_S_INT_STC_H

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
 */

/*==================[inclusions]============================================*/

#include <SomeIpXf_S_Api_Stc.h>

/*==================[macros]================================================*/

#if (defined SOMEIPXF_SER) /* to prevent double declaration */
#error SOMEIPXF_SER already defined
#endif /* if (defined SOMEIPXF_SER) */

#define SOMEIPXF_SER 0U

#if (defined SOMEIPXF_DE_SER) /* to prevent double declaration */
#error SOMEIPXF_DE_SER already defined
#endif /* if (defined SOMEIPXF_DE_SER) */

#define SOMEIPXF_DE_SER 1U

#if (defined SOMEIPXF_PROTOCOL_VERSION)
#error SOMEIPXF_PROTOCOL_VERSION already defined
#endif

/* Implemented protocol version */
#define SOMEIPXF_PROTOCOL_VERSION   0x01U

/* size of the complete SOME/IP Header according to Figure 7.3. in bytes */
#if (defined SOMEIPXF_HEADER_LENGTH)
#error SOMEIPXF_HEADER_LENGTH is already defined
#endif
#define SOMEIPXF_HEADER_LENGTH  8U

/** \brief Session Handling id counter maximum value  */
#if (defined SESSION_HANDLING_ID_COUNTER_MAX)
#error SESSION_HANDLING_ID_COUNTER_MAX is already defined
#endif
#define SESSION_HANDLING_ID_COUNTER_MAX  0xFFFFU

#if (defined SOMEIPXF_PARAM_UNUSED)
#error SOMEIPXF_PARAM_UNUSED already defined
#endif

/** \brief This macro can be used to avoid compiler warnings */
#define SOMEIPXF_PARAM_UNUSED(x) ((void) (x))

#if (defined SOMEIPXF_HLP_UNIQUETYPEDEF)
#error SOMEIPXF_HLP_UNIQUETYPEDEF already defined
#endif

#if (defined SOMEIPXF_HLP_UNIQUE)
#error SOMEIPXF_HLP_UNIQUE already defined
#endif

/* Helper macros for string concatenation */
/* Deviation MISRAC2012-1 */
#define SOMEIPXF_HLP_UNIQUETYPEDEF(x, y) x ## y
#define SOMEIPXF_HLP_UNIQUE(x, y) SOMEIPXF_HLP_UNIQUETYPEDEF(x, y)

#if (defined SOMEIPXF_PREVENTEMPTYTRANSLATIONUNIT)
#error SOMEIPXF_PREVENTEMPTYTRANSLATIONUNIT already defined
#endif

/** \brief Prevent empty translation unit
 *
 * This macro shall be used to prevent that a translation unit is empty within the SomeIpXf module.
 */
/* disable CheckC warning about two consecutive underscores */
/* CHECK: RULE 501 OFF */
#define SOMEIPXF_PREVENTEMPTYTRANSLATIONUNIT \
    typedef void SOMEIPXF_HLP_UNIQUE(SomeIpXfPreventEmptyTranslationUnit, __LINE__);
/* CHECK: RULE 501 ON */

#if (defined SOMEIPXF_STATIC_ASSERT_XFRMBUFFERLENGTH_TYPE_CAT)
#error SOMEIPXF_STATIC_ASSERT_XFRMBUFFERLENGTH_TYPE_CAT already defined
#endif

#define SOMEIPXF_STATIC_ASSERT_XFRMBUFFERLENGTH_TYPE_CAT(data_type_name,someipxf_data_type_config_id) \
        SOMEIPXF_STATIC_ASSERT_xfrmbufferlength_type_too_small_for_ ## data_type_name ## _ConfigID_ ## someipxf_data_type_config_id

#if (defined SOMEIPXF_STATIC_ASSERT_XFRMBUFFERLENGTH_TYPE)
#error SOMEIPXF_STATIC_ASSERT_XFRMBUFFERLENGTH_TYPE already defined
#endif

/** \brief Report static Xfrm buffer length assertion
 *
 * This macro shall be used to verify if the size of the data element can be hold
 * by the configured Xfrm buffer length. This check is done by a static compile time
 * assertion as part of defensive programming within the SomeIpXf module.
 */
#define SOMEIPXF_STATIC_ASSERT_XFRMBUFFERLENGTH_TYPE(data_type_name,someipxf_data_type_config_id) \
    typedef uint8 SOMEIPXF_STATIC_ASSERT_XFRMBUFFERLENGTH_TYPE_CAT(data_type_name,someipxf_data_type_config_id) \
            [(sizeof(data_type_name) <= SOMEIPXF_XFRMBUFFERLENGTH_TYPE_SIZE_MAX) ? 1 : -1]

/* Control macros for configuration arrays */
#if (defined SOMEIPXF_DATATYPE_MAX)
#error SOMEIPXF_DATATYPE_MAX is already defined
#endif

#if (defined SOMEIPXF_DATA_ELEMENT_START)
#error SOMEIPXF_DATA_ELEMENT_START is already defined
#endif

#if (defined SOMEIPXF_DATA_ELEMENT_STOP)
#error SOMEIPXF_DATA_ELEMENT_STOP is already defined
#endif

#if (defined SOMEIPXF_BASIC_DATA_TYPE_START)
#error SOMEIPXF_BASIC_DATA_TYPE_START is already defined
#endif

#if (defined SOMEIPXF_BASIC_DATA_TYPE_STOP)
#error SOMEIPXF_BASIC_DATA_TYPE_STOP is already defined
#endif

#if (defined SOMEIPXF_FSA_OF_BASIC_TYPE_START)
#error SOMEIPXF_FSA_OF_BASIC_TYPE_START is already defined
#endif

#if (defined SOMEIPXF_FSA_OF_BASIC_TYPE_STOP)
#error SOMEIPXF_FSA_OF_BASIC_TYPE_STOP is already defined
#endif

#if (defined SOMEIPXF_FSA_OF_STRUCT_START)
#error SOMEIPXF_FSA_OF_STRUCT_START is already defined
#endif

#if (defined SOMEIPXF_FSA_OF_STRUCT_STOP)
#error SOMEIPXF_FSA_OF_STRUCT_STOP is already defined
#endif

#if (defined SOMEIPXF_VSA_START)
#error SOMEIPXF_VSA_START is already defined
#endif

#if (defined SOMEIPXF_VSA_STOP)
#error SOMEIPXF_VSA_STOP is already defined
#endif

#if (defined SOMEIPXF_STRUCT_WITH_LF_START)
#error SOMEIPXF_STRUCT_WITH_LF_START is already defined
#endif

#if (defined SOMEIPXF_STRUCT_WITH_LF_STOP)
#error SOMEIPXF_STRUCT_WITH_LF_STOP is already defined
#endif

#if (defined SOMEIPXF_BASIC_DATA_TYPE_WITH_LF_STOP)
#error SOMEIPXF_BASIC_DATA_TYPE_WITH_LF_STOP already defined
#endif

#if (defined SOMEIPXF_FSA_WITH_LF_START)
#error SOMEIPXF_FSA_WITH_LF_START is already defined
#endif

#if (defined SOMEIPXF_FSA_WITH_LF_STOP)
#error SOMEIPXF_FSA_WITH_LF_STOP is already defined
#endif

#if (defined SOMEIPXF_VSA_ELEMENT_CHECK_POINT)
#error SOMEIPXF_VSA_ELEMENT_CHECK_POINT is already defined
#endif

#if (defined SOMEIPXF_VSA_ELEMENT_ENTER)
#error SOMEIPXF_VSA_ELEMENT_ENTER is already defined
#endif

#if (defined SOMEIPXF_FSA_OF_VSA_START)
#error SOMEIPXF_FSA_OF_VSA_START is already defined
#endif

#if (defined SOMEIPXF_FSA_OF_VSA_STOP)
#error SOMEIPXF_FSA_OF_VSA_STOP is already defined
#endif

/* Data type (uint16 or uint32) defines element config data size */
#if (SOMEIPXF_XFRMBUFFERLENGTHTYPE_ISUINT32 == STD_ON)
#define SOMEIPXF_DATATYPE_MAX                    UINT32_C(0xFFFFFFFF)
#else
#define SOMEIPXF_DATATYPE_MAX                    UINT16_C(0xFFFF)
#endif

#define SOMEIPXF_DATA_ELEMENT_START                   (SOMEIPXF_DATATYPE_MAX - 1U)
#define SOMEIPXF_BASIC_DATA_TYPE_START                (SOMEIPXF_DATATYPE_MAX - 2U)
#define SOMEIPXF_BASIC_DATA_TYPE_STOP                 (SOMEIPXF_DATATYPE_MAX - 3U)
#define SOMEIPXF_FSA_OF_BASIC_TYPE_START              (SOMEIPXF_DATATYPE_MAX - 4U)
#define SOMEIPXF_FSA_OF_BASIC_TYPE_STOP               (SOMEIPXF_DATATYPE_MAX - 5U)
#define SOMEIPXF_FSA_OF_STRUCT_START                  (SOMEIPXF_DATATYPE_MAX - 6U)
#define SOMEIPXF_FSA_OF_STRUCT_STOP                   (SOMEIPXF_DATATYPE_MAX - 7U)
#define SOMEIPXF_FSA_OF_FSA_START                     (SOMEIPXF_DATATYPE_MAX - 8U)
#define SOMEIPXF_FSA_OF_FSA_STOP                      (SOMEIPXF_DATATYPE_MAX - 9U)
#define SOMEIPXF_STRUCT_WITH_LF_START                 (SOMEIPXF_DATATYPE_MAX - 10U)
#define SOMEIPXF_STRUCT_WITH_LF_STOP                  (SOMEIPXF_DATATYPE_MAX - 11U)
#define SOMEIPXF_FSA_WITH_LF_START                    (SOMEIPXF_DATATYPE_MAX - 12U)
#define SOMEIPXF_FSA_WITH_LF_STOP                     (SOMEIPXF_DATATYPE_MAX - 13U)
#define SOMEIPXF_FSA_ELEMENT_ENTER                    (SOMEIPXF_DATATYPE_MAX - 14U)
#define SOMEIPXF_FSA_OF_BASIC_DATA_TYPE_WITH_LF_START (SOMEIPXF_DATATYPE_MAX - 15U)
#define SOMEIPXF_FSA_OF_BASIC_DATA_TYPE_WITH_LF_STOP  (SOMEIPXF_DATATYPE_MAX - 16U)
#define SOMEIPXF_VSA_START                            (SOMEIPXF_DATATYPE_MAX - 17U)
#define SOMEIPXF_VSA_STOP                             (SOMEIPXF_DATATYPE_MAX - 18U)
#define SOMEIPXF_VSA_ELEMENT_CHECK_POINT              (SOMEIPXF_DATATYPE_MAX - 19U)
#define SOMEIPXF_VSA_ELEMENT_ENTER                    (SOMEIPXF_DATATYPE_MAX - 20U)
#define SOMEIPXF_STRUCT_TLV_START                     (SOMEIPXF_DATATYPE_MAX - 21U)
#define SOMEIPXF_STRUCT_TLV_STOP                      (SOMEIPXF_DATATYPE_MAX - 22U)
#define SOMEIPXF_STRUCT_TLV_MEMBER_OPT_START          (SOMEIPXF_DATATYPE_MAX - 23U)
#define SOMEIPXF_STRUCT_TLV_MEMBER_OPT_STOP           (SOMEIPXF_DATATYPE_MAX - 24U)
#define SOMEIPXF_STRUCT_TLV_MEMBER_FIX_START          (SOMEIPXF_DATATYPE_MAX - 25U)
#define SOMEIPXF_STRUCT_TLV_MEMBER_FIX_STOP           (SOMEIPXF_DATATYPE_MAX - 26U)
#define SOMEIPXF_ARG_TLV_START                        (SOMEIPXF_DATATYPE_MAX - 27U)
#define SOMEIPXF_DATA_ELEMENT_STOP                    (SOMEIPXF_DATATYPE_MAX - 28U)

#if (defined SOMEIPXF_DATA_ELEMENT_CONFIG_START)
#error SOMEIPXF_DATA_ELEMENT_CONFIG_START is already defined
#endif

#define SOMEIPXF_DATA_ELEMENT_CONFIG_START \
        SOMEIPXF_DATA_ELEMENT_START, \
        SOMEIPXF_CONFIG_ARRAY_SIZE,\
        SOMEIPXF_SET_INTEGER_CONFIG_NW_1B,\
        SOMEIPXF_SET_INTEGER_CONFIG_NW_2B /* LFByteSize */,\
        SOMEIPXF_SET_INTEGER_CONFIG_NW_4B /* LFByteSize */,\
        SOMEIPXF_SET_INTEGER_CONFIG_NW_8B /* LFByteSize */

#if (defined SOMEIPXF_CONFIG_START_OFFEST)
#error SOMEIPXF_CONFIG_START_OFFEST is already defined
#endif
#define SOMEIPXF_CONFIG_START_OFFEST 16U

#if (defined SOMEIPXF_CONFIG_START_OFFEST_NW_1B)
#error SOMEIPXF_CONFIG_START_OFFEST_NW_1B is already defined
#endif
#define SOMEIPXF_CONFIG_START_OFFEST_NW_1B 2U

#if (defined SOMEIPXF_CONFIG_START_OFFEST_NW_2B)
#error SOMEIPXF_CONFIG_START_OFFEST_NW_2B is already defined
#endif
#define SOMEIPXF_CONFIG_START_OFFEST_NW_2B 3U

#if (defined SOMEIPXF_CONFIG_START_OFFEST_NW_4B)
#error SOMEIPXF_CONFIG_START_OFFEST_NW_4B is already defined
#endif
#define SOMEIPXF_CONFIG_START_OFFEST_NW_4B 5U

#if (defined SOMEIPXF_CONFIG_START_OFFEST_NW_8B)
#error SOMEIPXF_CONFIG_START_OFFEST_NW_8B is already defined
#endif
#define SOMEIPXF_CONFIG_START_OFFEST_NW_8B 9U

#if (defined SOMEIPXF_BASIC_DATA_TYPE_CONFIG)
#error SOMEIPXF_BASIC_DATA_TYPE_CONFIG is already defined
#endif

#define SOMEIPXF_BASIC_DATA_TYPE_CONFIG(size) size

#if (defined SOMEIPXF_VSA_STOP_INDEX)
#error SOMEIPXF_VSA_STOP_INDEX is already defined
#endif

#define SOMEIPXF_VSA_STOP_INDEX(idx) idx

#if (defined SOMEIPXF_CONFIG_ARRAY_CAT)
#error SOMEIPXF_CONFIG_ARRAY_CAT is already defined
#endif

#define SOMEIPXF_CONFIG_ARRAY_CAT(someipxf_data_type_config_id) \
    SomeIpXf_Config_ ## someipxf_data_type_config_id \
    [SOMEIPXF_CONFIG_ARRAY_SIZE_ ## SomeIpXf_Config_ ## someipxf_data_type_config_id]

#if (defined SOMEIPXF_CONFIG_ARRAY_HLP)
#error SOMEIPXF_CONFIG_ARRAY_HLP is already defined
#endif

#define SOMEIPXF_CONFIG_ARRAY_HLP(someipxf_data_type_config_id) \
  SOMEIPXF_CONFIG_ARRAY_CAT(someipxf_data_type_config_id)

#if (defined SOMEIPXF_CONFIG_ARRAY)
#error SOMEIPXF_CONFIG_ARRAY is already defined
#endif

#define SOMEIPXF_CONFIG_ARRAY \
  SOMEIPXF_STATIC_ASSERT_XFRMBUFFERLENGTH_TYPE(SOMEIPXF_DATA_TYPE_NAME, SOMEIPXF_DATA_TYPE_CONFIG_ID); \
  CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, SOMEIPXF_CONST) SOMEIPXF_CONFIG_ARRAY_HLP(SOMEIPXF_DATA_TYPE_CONFIG_ID)

#if (defined SOMEIPXF_SET_FSA_CONFIG)
#error SOMEIPXF_SET_FSA_CONFIG is already defined
#endif
/* This macro defines an array of fixed size by the number of elements and the size of a single
 * element */
#define SOMEIPXF_SET_FSA_CONFIG(NumArrayElements, ArrayElementType) \
   (NumArrayElements),\
   sizeof(ArrayElementType)

#if (defined SOMEIPXF_SET_CONFIG_CAT)
#error SOMEIPXF_SET_CONFIG_CAT is already defined
#endif

#define SOMEIPXF_SET_CONFIG_CAT(someipxf_data_type_config_endian, set_integer_config) \
  SomeIpXf_Config_ ## someipxf_data_type_config_endian ## set_integer_config

#if (defined SOMEIPXF_SET_CONFIG_HLP)
#error SOMEIPXF_SET_CONFIG_HLP is already defined
#endif

#define SOMEIPXF_SET_CONFIG_HLP(someipxf_data_type_config_endian, set_integer_config) \
  SOMEIPXF_SET_CONFIG_CAT(someipxf_data_type_config_endian, set_integer_config)

#if (defined SOMEIPXF_SET_INTEGER_CONFIG_NW_1B)
#error SOMEIPXF_SET_INTEGER_CONFIG_NW_1B is already defined
#endif

#define SOMEIPXF_SET_INTEGER_CONFIG_NW_1B \
    SOMEIPXF_SET_CONFIG_HLP(SOMEIPXF_DATA_TYPE_CONFIG_ENDIAN, _SET_INTEGER_CONFIG_NW_1B)

#if (defined SOMEIPXF_SET_INTEGER_CONFIG_NW_2B)
#error SOMEIPXF_SET_INTEGER_CONFIG_NW_2B is already defined
#endif

#define SOMEIPXF_SET_INTEGER_CONFIG_NW_2B \
    SOMEIPXF_SET_CONFIG_HLP(SOMEIPXF_DATA_TYPE_CONFIG_ENDIAN, _SET_INTEGER_CONFIG_NW_2B)

#if (defined SOMEIPXF_SET_INTEGER_CONFIG_NW_4B)
#error SOMEIPXF_SET_INTEGER_CONFIG_NW_4B is already defined
#endif

#define SOMEIPXF_SET_INTEGER_CONFIG_NW_4B \
    SOMEIPXF_SET_CONFIG_HLP(SOMEIPXF_DATA_TYPE_CONFIG_ENDIAN, _SET_INTEGER_CONFIG_NW_4B)

#if (defined SOMEIPXF_SET_INTEGER_CONFIG_NW_8B)
#error SOMEIPXF_SET_INTEGER_CONFIG_NW_8B is already defined
#endif

#define SOMEIPXF_SET_INTEGER_CONFIG_NW_8B \
    SOMEIPXF_SET_CONFIG_HLP(SOMEIPXF_DATA_TYPE_CONFIG_ENDIAN, _SET_INTEGER_CONFIG_NW_8B)

#if (defined SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_1B) /* to prevent double declaration */
#error SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_1B already defined
#endif /* if (defined SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_1B) */
#define SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_1B        \
  0

#if (defined SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_2B) /* to prevent double declaration */
#error SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_2B already defined
#endif /* if (defined SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_2B) */
#define SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_2B        \
  0, \
  1

#if (defined SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_4B) /* to prevent double declaration */
#error SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_4B already defined
#endif /* if (defined SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_4B) */
#define SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_4B        \
  0, \
  1, \
  2, \
  3

#if (defined SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_8B) /* to prevent double declaration */
#error SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_8B already defined
#endif /* if (defined SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_8B) */
#define SomeIpXf_Config_OP_SET_INTEGER_CONFIG_NW_8B        \
  0, \
  1, \
  2, \
  3, \
  4, \
  5, \
  6, \
  7

#if (defined SOMEIPXF_MEMBER_SIZE)
#error SOMEIPXF_MEMBER_SIZE is already defined
#endif
/* This macro extracts the size of a single struct member */
#define SOMEIPXF_MEMBER_SIZE(datatype, member) sizeof(((datatype *)0)->member)

#if (defined SOMEIPXF_CONFIG_ARRAY_SIZE_CAT)
#error SOMEIPXF_CONFIG_ARRAY_SIZE_CAT is already defined
#endif
#define SOMEIPXF_CONFIG_ARRAY_SIZE_CAT(someipxf_data_type_config_id) \
  SOMEIPXF_CONFIG_ARRAY_SIZE_ ## SomeIpXf_Config_ ## someipxf_data_type_config_id

#if (defined SOMEIPXF_CONFIG_ARRAY_SIZE_HLP)
#error SOMEIPXF_CONFIG_ARRAY_SIZE_HLP is already defined
#endif
#define SOMEIPXF_CONFIG_ARRAY_SIZE_HLP(someipxf_data_type_config_id) \
  SOMEIPXF_CONFIG_ARRAY_SIZE_CAT(someipxf_data_type_config_id)

#if (defined SOMEIPXF_CONFIG_ARRAY_SIZE)
#error SOMEIPXF_CONFIG_ARRAY_SIZE is already defined
#endif
/* This macro defines the config array size */
#define SOMEIPXF_CONFIG_ARRAY_SIZE \
  SOMEIPXF_CONFIG_ARRAY_SIZE_HLP(SOMEIPXF_DATA_TYPE_CONFIG_ID)

#if (defined SOMEIPXF_SET_VSA_CONFIG)
#error SOMEIPXF_SET_VSA_CONFIG is already defined
#endif
/* This macro defines an array of variable size configurations */
#define SOMEIPXF_SET_VSA_CONFIG(DimOfArray, LFsize, SizeIndicatorType, VSAPayload) \
   (DimOfArray), \
   (LFsize), \
   sizeof(SizeIndicatorType), \
   offsetof(SOMEIPXF_DATA_TYPE_NAME, VSAPayload)

#if (defined SOMEIPXF_SET_VSA_DIM_CONFIG)
#error SOMEIPXF_SET_VSA_DIM_CONFIG is already defined
#endif
/* This macro defines a single dimension of an array of variable size by
 * the maximum number of its elements and the offset to its size member in the structure */
#define SOMEIPXF_SET_VSA_DIM_CONFIG(SizeIndicator_Dim, PayloadElementSize, DimElements , MaxNumArrayElem) \
   offsetof(SOMEIPXF_DATA_TYPE_NAME, SizeIndicator_Dim), \
   (SOMEIPXF_MEMBER_SIZE(SOMEIPXF_DATA_TYPE_NAME, PayloadElementSize)*DimElements), \
   (MaxNumArrayElem)

#if (defined SOMEIPXF_SET_STRUCT_LENGTH_FIELD_CONFIG)
#error SOMEIPXF_SET_STRUCT_LENGTH_FIELD_CONFIG is already defined
#endif
/* This macro defines the length field of a structure by
 * the SizeOfStructLengthFields */
#define SOMEIPXF_SET_STRUCT_LENGTH_FIELD_CONFIG(SizeLengthField) (SizeLengthField)

#if (defined SOMEIPXF_SET_FSA_WITH_LF_CONFIG)
#error SOMEIPXF_SET_FSA_WITH_LF_CONFIG is already defined
#endif
/* This macro defines the fix size array with length field configuration by
 * the number of dimensions, the basic data type of an element and SizeOfArrayLengthFields */
#define SOMEIPXF_SET_FSA_WITH_LF_CONFIG(NumDimArray, SizeLengthField) \
   (NumDimArray),         \
   (SizeLengthField)

#if (defined SOMEIPXF_SET_FSA_WITH_LF_DIM_CONFIG)
#error SOMEIPXF_SET_FSA_WITH_LF_DIM_CONFIG is already defined
#endif
/* This macro defines a single dimension of the fix size array with length field by
 * the number of array elements and the overall minimum number of bytes (without optional elements) */
#define SOMEIPXF_SET_FSA_WITH_LF_DIM_CONFIG(NumArrayElements, ElementSize, DimElements) \
   (NumArrayElements), \
   (sizeof(ElementSize)*DimElements)

#if (defined SOMEIPXF_ARG_TLV_CONFIG)/* to prevent double declaration */
#error SOMEIPXF_ARG_TLV_CONFIG is already defined
#endif
/* This macro defines an array of global config context values for a TLV argument:
 * - WireTpye
 * - Data ID
 * - static size of length fields
 *  */
#define SOMEIPXF_ARG_TLV_CONFIG(WireType, DatatId, LFSize) \
  (WireType),                                          \
  (DatatId),                                           \
  (LFSize)

#if (defined SOMEIPXF_SET_TLV_STRUCT_CONFIG)/* to prevent double declaration */
#error SOMEIPXF_SET_TLV_STRUCT_CONFIG is already defined
#endif
/* This macro defines an array of global config context values for a TLV struct:
 * - static size of length fields
 * - position of availability field
 * - length of availability bitfield
 * - number of nonOptional members
 * - the overall number of Data IDs
 *  */
#define SOMEIPXF_SET_TLV_STRUCT_CONFIG(LFSize, availabilityBitfield_type, FixMemNum, DataIDNum) \
   (LFSize),                                                                                    \
   offsetof(SOMEIPXF_DATA_TYPE_NAME, availabilityBitfield_type),                                \
   sizeof(((SOMEIPXF_DATA_TYPE_NAME *)0)->availabilityBitfield_type),                           \
   (FixMemNum),                                                                                 \
   (DataIDNum)

#if (defined SOMEIPXF_SET_TLV_STRUCT_FIX_MEMBER_CONFIG)/* to prevent double declaration */
#error SOMEIPXF_SET_TLV_STRUCT_FIX_MEMBER_CONFIG is already defined
#endif
/* This macro defines an array of global config context values for a TLV struct
 * with only fixed members:
 * - static size of length fields
 * - number of nonOptional (fixed) members
 * - the overall number of Data IDs
 *  */
#define SOMEIPXF_SET_TLV_STRUCT_FIX_MEMBER_CONFIG(LFSize, FixMemNum, DataIDNum) \
   (LFSize),                                                                    \
   SOMEIPXF_DATATYPE_MAX,                                                       \
   SOMEIPXF_DATATYPE_MAX,                                                       \
   (FixMemNum),                                                                 \
   (DataIDNum)

#if (defined SOMEIPXF_SET_TLV_STRUCT_OPT_MEMBER_CONFIG)/* to prevent double declaration */
#error SOMEIPXF_SET_TLV_STRUCT_OPT_MEMBER_CONFIG is already defined
#endif
/* This macro defines an array of config context values for a optional TLV member:
 * - WireTpye
 * - Data ID
 * - offset of availabilityBitfield
 * - optional member index
 *  */
#define SOMEIPXF_SET_TLV_STRUCT_OPT_MEMBER_CONFIG(WireType, DataId, OptionalMemberIndex, availabilityBitfield_type) \
   (WireType),                                                                                              \
   (DataId),                                                                                              \
   offsetof(SOMEIPXF_DATA_TYPE_NAME, availabilityBitfield_type),                                          \
   (OptionalMemberIndex)

#if (defined SOMEIPXF_SET_TLV_STRUCT_MEMBER_CONFIG)/* to prevent double declaration */
#error SOMEIPXF_SET_TLV_STRUCT_MEMBER_CONFIG is already defined
#endif
/* This macro defines an array of config context values for a non optional TLV member:
 * - WireTpye
 * - Data ID
 *  */
#define SOMEIPXF_SET_TLV_STRUCT_MEMBER_CONFIG(WireType, DataId) \
  (WireType), \
  (DataId)

#if (defined SOMEIPXF_TLV_DATA_ID)/* to prevent double declaration */
#error SOMEIPXF_TLV_DATA_ID is already defined
#endif
/* This macro defines the tlv dataid  */
#define SOMEIPXF_TLV_DATA_ID(TlvDataId) TlvDataId

#if (defined SOMEIPXF_STRUCT_TLV_MEMBER_START_INDEX)/* to prevent double declaration */
#error SOMEIPXF_STRUCT_TLV_MEMBER_START_INDEX is already defined
#endif
/* This macro defines the member start position index */
#define SOMEIPXF_STRUCT_TLV_MEMBER_START_INDEX(MemberStartPositionIdx) MemberStartPositionIdx

#if (defined SOMEIPXF_TLV_STRUCT_STOP_INDEX)/* to prevent double declaration */
#error SOMEIPXF_TLV_STRUCT_STOP_INDEX is already defined
#endif
/* This macro defines the Tlv stop index  */
#define SOMEIPXF_TLV_STRUCT_STOP_INDEX(TlvStopIndex) TlvStopIndex

#if (defined SOMEIPXF_STRUCT_TLV_MEMBER_OPT_STOP_INDEX)/* to prevent double declaration */
#error SOMEIPXF_STRUCT_TLV_MEMBER_OPT_STOP_INDEX is already defined
#endif
/* This macro defines an the Tlv member stop index  */
#define SOMEIPXF_STRUCT_TLV_MEMBER_OPT_STOP_INDEX(TlvOptMemberStopIndex) TlvOptMemberStopIndex

#if (defined SOMEIPXF_STRUCT_TLV_MEMBER_FIX_STOP_INDEX)/* to prevent double declaration */
#error SOMEIPXF_STRUCT_TLV_MEMBER_FIX_STOP_INDEX is already defined
#endif
/* This macro defines an the Tlv member stop index  */
#define SOMEIPXF_STRUCT_TLV_MEMBER_FIX_STOP_INDEX(TlvOptMemberStopIndex) TlvOptMemberStopIndex

#if (defined SOMEIPXF_ASSIGN_UINT_32_64_TO_XFRMBUFFERLENGTH_TYPE)
#error SOMEIPXF_ASSIGN_UINT_32_64_TO_XFRMBUFFERLENGTH_TYPE is already defined
#endif
/* This macro covers the assignment of an uint32/uint64 right hand operand
 * to an xfrmbufferlength_type dependable left hand operand. In case of
 * xfrmbufferlength_type configured to uint16 an explicit truncation
 * of the uint32 value applies. */
#if (SOMEIPXF_XFRMBUFFERLENGTHTYPE_ISUINT32 == STD_ON)
#define SOMEIPXF_ASSIGN_UINT_32_64_TO_XFRMBUFFERLENGTH_TYPE(left_hand_xfrmbufferlength_type, right_hand_uint32) \
do{ \
    left_hand_xfrmbufferlength_type = (uint32)((right_hand_uint32)&0xFFFFFFFFU); \
  } while(0)
#else
#define SOMEIPXF_ASSIGN_UINT_32_64_TO_XFRMBUFFERLENGTH_TYPE(left_hand_xfrmbufferlength_type, right_hand_uint32) \
do{ \
    left_hand_xfrmbufferlength_type = (uint16)((right_hand_uint32)&0xFFFFU); \
  } while(0)
#endif

#if (defined SOMEIPXF_XFRMBUFFERLENGTH_TYPE_SIZE_MAX) /* to prevent double declaration */
#error SOMEIPXF_XFRMBUFFERLENGTH_TYPE_SIZE_MAX already defined
#endif

#if (SOMEIPXF_XFRMBUFFERLENGTHTYPE_ISUINT32 == STD_ON)
#define SOMEIPXF_XFRMBUFFERLENGTH_TYPE_SIZE_MAX 0xFFFFFFFFUL
#else
#define SOMEIPXF_XFRMBUFFERLENGTH_TYPE_SIZE_MAX 0xFFFFU
#endif

#if (SOMEIPXF_VSA_AVAILABLE == STD_ON)

#if (defined SOMEIPXF_VSA_MULTI_DIM_ENABLED)
#error SOMEIPXF_VSA_MULTI_DIM_ENABLED is already defined
#endif
/* This macro enable code which is required to handle VSAs with multi dimensions only */
#if (SOMEIPXF_VSA_MAX_DIM > 1U)
#define SOMEIPXF_VSA_MULTI_DIM_ENABLED STD_ON
#else
#define SOMEIPXF_VSA_MULTI_DIM_ENABLED STD_OFF
#endif

#endif /* (SOMEIPXF_VSA_AVAILABLE == STD_ON) */

#if (SOMEIPXF_FSA_AVAILABLE == STD_ON)

#if (defined SOMEIPXF_FSA_MULTI_DIM_ENABLED)
#error SOMEIPXF_FSA_MULTI_DIM_ENABLED is already defined
#endif
/* This macro enable code which is required to handle VSAs with multi dimensions only */
#if (SOMEIPXF_FSA_MAX_DIM > 1U)
#define SOMEIPXF_FSA_MULTI_DIM_ENABLED STD_ON
#else
#define SOMEIPXF_FSA_MULTI_DIM_ENABLED STD_OFF
#endif

#endif /* (SOMEIPXF_FSA_AVAILABLE == STD_ON) */

#if (defined SOMEIPXF_SET_STRUCT_CONFIG_NW_1B)
#error SOMEIPXF_SET_STRUCT_CONFIG_NW_1B is already defined
#endif

#define SOMEIPXF_SET_STRUCT_CONFIG_NW_1B \
    SOMEIPXF_SET_CONFIG_HLP(SOMEIPXF_DATA_TYPE_CONFIG_ENDIAN, _SET_STRUCT_CONFIG_NW_1B)

#if (defined SOMEIPXF_SET_STRUCT_CONFIG_NW_2B)
#error SOMEIPXF_SET_STRUCT_CONFIG_NW_2B is already defined
#endif

#define SOMEIPXF_SET_STRUCT_CONFIG_NW_2B \
    SOMEIPXF_SET_CONFIG_HLP(SOMEIPXF_DATA_TYPE_CONFIG_ENDIAN, _SET_STRUCT_CONFIG_NW_2B)

#if (defined SOMEIPXF_SET_STRUCT_CONFIG_NW_4B)
#error SOMEIPXF_SET_STRUCT_CONFIG_NW_4B is already defined
#endif

#define SOMEIPXF_SET_STRUCT_CONFIG_NW_4B \
    SOMEIPXF_SET_CONFIG_HLP(SOMEIPXF_DATA_TYPE_CONFIG_ENDIAN, _SET_STRUCT_CONFIG_NW_4B)

#if (defined SOMEIPXF_SET_STRUCT_CONFIG_NW_8B)
#error SOMEIPXF_SET_STRUCT_CONFIG_NW_8B is already defined
#endif

#define SOMEIPXF_SET_STRUCT_CONFIG_NW_8B \
    SOMEIPXF_SET_CONFIG_HLP(SOMEIPXF_DATA_TYPE_CONFIG_ENDIAN, _SET_STRUCT_CONFIG_NW_8B)

/* configuration macros */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)

/* Little Endian on platform to Little Endian on network */
#if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_1B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_1B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_1B) */
#define SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_1B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member)

#if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_2B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_2B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_2B) */
#define SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_2B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member),                   \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1

#if (defined SOMEIPXF_SET_STRUCT_CONFIG_MEM) /* to prevent double declaration */
#error SOMEIPXF_SET_STRUCT_CONFIG_MEM already defined
#endif /* if (defined SOMEIPXF_SET_STRUCT_CONFIG_MEM) */
#define SOMEIPXF_SET_STRUCT_CONFIG_MEM(Member, Next_Member, ByteSize) \
  ( offsetof(SOMEIPXF_DATA_TYPE_NAME, Next_Member) - offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) ), \
  ByteSize

#if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_4B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_4B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_4B) */
#define SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_4B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member),                   \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 2,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 3

#if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_8B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_8B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_8B) */
#define SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_8B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member),                   \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 2,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 3,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 4,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 5,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 6,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 7

#if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_1B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_1B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_1B) */
#define SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_1B        \
  0

#if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_2B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_2B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_2B) */
#define SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_2B        \
  0, \
  1

#if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_4B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_4B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_4B) */
#define SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_4B        \
  0, \
  1, \
  2, \
  3

#if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_8B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_8B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_8B) */
#define SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_8B        \
  0, \
  1, \
  2, \
  3, \
  4, \
  5, \
  6, \
  7

/* Little Endian on platform to Big Endian on network */
#if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_1B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_1B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_1B) */
#define SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_1B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member)

#if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_2B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_2B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_2B) */
#define SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_2B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member)

#if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_4B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_4B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_4B) */
#define SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_4B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 3,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 2,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member)

#if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_8B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_8B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_8B) */
#define SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_8B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 7,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 6,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 5,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 4,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 3,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 2,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member)

#if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_1B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_1B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_1B) */
#define SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_1B        \
  0

#if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_2B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_2B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_2B) */
#define SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_2B        \
  1, \
  0

#if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_4B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_4B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_4B) */
#define SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_4B        \
  3, \
  2, \
  1, \
  0

#if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_8B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_8B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_8B) */
#define SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_8B        \
  7, \
  6, \
  5, \
  4, \
  3, \
  2, \
  1, \
  0

#if (defined SOMEIPXF_COPY_U16_BUF) /* to prevent double declaration */
#error SOMEIPXF_COPY_U16_BUF already defined
#endif /* if (defined SOMEIPXF_COPY_U16_BUF) */
#define SOMEIPXF_COPY_U16_BUF(ConfigPtr, ConfigPtr_StartPos, BufPtr, BufPos, Val_U16) \
  do{ \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 0U]] =  (uint8) ((Val_U16)); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 1U]] =  (uint8) ((Val_U16) >> 8U); \
  } while(0)

#if (defined SOMEIPXF_COPY_U16_DATA_ELM) /* to prevent double declaration */
#error SOMEIPXF_COPY_U16_DATA_ELM already defined
#endif /* if (defined SOMEIPXF_COPY_U16_DATA_ELM) */
#define SOMEIPXF_COPY_U16_DATA_ELM(DataBytePtr, DataBytePos, Val_U16) \
  do{ \
  (DataBytePtr)[(DataBytePos)] =  (uint8) ((Val_U16)); \
  (DataBytePtr)[(DataBytePos) + 1U] =  (uint8) ((Val_U16) >> 8U); \
  } while(0)

#if (defined SOMEIPXF_READ_U16_BUF) /* to prevent double declaration */
#error SOMEIPXF_READ_U16_BUF already defined
#endif /* if (defined SOMEIPXF_READ_U16_BUF) */
#define SOMEIPXF_READ_U16_BUF(ConfigPtr, ConfigPtr_StartPos, BufPtr, BufPos, Val_U16) \
  do{ \
  (Val_U16) = (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 1U]]; \
  (Val_U16) = (((Val_U16) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 0U]]); \
  } while(0)

#if (defined SOMEIPXF_COPY_U32_BUF) /* to prevent double declaration */
#error SOMEIPXF_COPY_U32_BUF already defined
#endif /* if (defined SOMEIPXF_COPY_U32_BUF) */
#define SOMEIPXF_COPY_U32_BUF(ConfigPtr, ConfigPtr_StartPos, BufPtr, BufPos, Val_U32) \
  do{ \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 0U]] = (uint8) ((Val_U32)); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 1U]] = (uint8) ((Val_U32) >> 8U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 2U]] = (uint8) ((Val_U32) >> 16U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 3U]] = (uint8) ((Val_U32) >> 24U); \
  } while(0)

#if (defined SOMEIPXF_COPY_U32_DATA_ELM) /* to prevent double declaration */
#error SOMEIPXF_COPY_U32_DATA_ELM already defined
#endif /* if (defined SOMEIPXF_COPY_U32_DATA_ELM) */
#define SOMEIPXF_COPY_U32_DATA_ELM(DataBytePtr, DataBytePos, Val_U32) \
  do{ \
  (DataBytePtr)[(DataBytePos)] = (uint8) ((Val_U32)); \
  (DataBytePtr)[(DataBytePos) + 1U] = (uint8) ((Val_U32) >> 8U); \
  (DataBytePtr)[(DataBytePos) + 2U] = (uint8) ((Val_U32) >> 16U); \
  (DataBytePtr)[(DataBytePos) + 3U] = (uint8) ((Val_U32) >> 24U); \
  } while(0)

#if (defined SOMEIPXF_READ_U32_BUF) /* to prevent double declaration */
#error SOMEIPXF_READ_U32_BUF already defined
#endif /* if (defined SOMEIPXF_READ_U32_BUF) */
#define SOMEIPXF_READ_U32_BUF(ConfigPtr, ConfigPtr_StartPos, BufPtr, BufPos, Val_U32) \
  do{ \
  (Val_U32) = (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 3U]]; \
  (Val_U32) = (((Val_U32) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 2U]]); \
  (Val_U32) = (((Val_U32) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 1U]]); \
  (Val_U32) = (((Val_U32) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 0U]]); \
  } while(0)

#if (defined SOMEIPXF_COPY_U64_BUF) /* to prevent double declaration */
#error SOMEIPXF_COPY_U64_BUF already defined
#endif /* if (defined SOMEIPXF_COPY_U64_BUF) */
#define SOMEIPXF_COPY_U64_BUF(ConfigPtr, ConfigPtr_StartPos, BufPtr, BufPos, Val_U64) \
  do{ \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 0U]] = (uint8) ((Val_U64)); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 1U]] = (uint8) ((Val_U64) >> 8U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 2U]] = (uint8) ((Val_U64) >> 16U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 3U]] = (uint8) ((Val_U64) >> 24U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 4U]] = (uint8) ((Val_U64) >> 32U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 5U]] = (uint8) ((Val_U64) >> 40U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 6U]] = (uint8) ((Val_U64) >> 48U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 7U]] = (uint8) ((Val_U64) >> 56U); \
  } while(0)

#if (defined SOMEIPXF_COPY_U64_DATA_ELM) /* to prevent double declaration */
#error SOMEIPXF_COPY_U64_DATA_ELM already defined
#endif /* if (defined SOMEIPXF_COPY_U64_DATA_ELM) */
#define SOMEIPXF_COPY_U64_DATA_ELM(DataBytePtr, DataBytePos, Val_U64) \
  do{ \
  (DataBytePtr)[(DataBytePos)] = (uint8) ((Val_U64)); \
  (DataBytePtr)[(DataBytePos) + 1U] = (uint8) ((Val_U64) >> 8U); \
  (DataBytePtr)[(DataBytePos) + 2U] = (uint8) ((Val_U64) >> 16U); \
  (DataBytePtr)[(DataBytePos) + 3U] = (uint8) ((Val_U64) >> 24U); \
  (DataBytePtr)[(DataBytePos) + 4U] = (uint8) ((Val_U64) >> 32U); \
  (DataBytePtr)[(DataBytePos) + 5U] = (uint8) ((Val_U64) >> 40U); \
  (DataBytePtr)[(DataBytePos) + 6U] = (uint8) ((Val_U64) >> 48U); \
  (DataBytePtr)[(DataBytePos) + 7U] = (uint8) ((Val_U64) >> 56U); \
  } while(0)

#if (defined SOMEIPXF_READ_U64_BUF) /* to prevent double declaration */
#error SOMEIPXF_READ_U64_BUF already defined
#endif /* if (defined SOMEIPXF_READ_U64_BUF) */
#define SOMEIPXF_READ_U64_BUF(ConfigPtr, ConfigPtr_StartPos, BufPtr, BufPos, Val_U64) \
  do{ \
  (Val_U64) = (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 7U]]; \
  (Val_U64) = (((Val_U64) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 6U]]); \
  (Val_U64) = (((Val_U64) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 5U]]); \
  (Val_U64) = (((Val_U64) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 4U]]); \
  (Val_U64) = (((Val_U64) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 3U]]); \
  (Val_U64) = (((Val_U64) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 2U]]); \
  (Val_U64) = (((Val_U64) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 1U]]); \
  (Val_U64) = (((Val_U64) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 0U]]); \
  } while(0)

#else  /* CPU_BYTE_ORDER is HIGH_BYTE_FIRST */

#if (defined SOMEIPXF_COPY_U16_BUF) /* to prevent double declaration */
#error SOMEIPXF_COPY_U16_BUF already defined
#endif /* if (defined SOMEIPXF_COPY_U16_BUF) */
#define SOMEIPXF_COPY_U16_BUF(ConfigPtr, ConfigPtr_StartPos, BufPtr, BufPos, Val_U16) \
  do{ \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 0U]] =  (uint8) ((Val_U16) >> 8U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 1U]] =  (uint8) ((Val_U16)); \
  } while(0)

#if (defined SOMEIPXF_COPY_U16_DATA_ELM) /* to prevent double declaration */
#error SOMEIPXF_COPY_U16_DATA_ELM already defined
#endif /* if (defined SOMEIPXF_COPY_U16_DATA_ELM) */
#define SOMEIPXF_COPY_U16_DATA_ELM(DataBytePtr, DataBytePos, Val_U16) \
  do{ \
  (DataBytePtr)[(DataBytePos)] =  (uint8) ((Val_U16) >> 8U); \
  (DataBytePtr)[(DataBytePos) + 1U] =  (uint8) ((Val_U16)); \
  } while(0)

#if (defined SOMEIPXF_READ_U16_BUF) /* to prevent double declaration */
#error SOMEIPXF_READ_U16_BUF already defined
#endif /* if (defined SOMEIPXF_READ_U16_BUF) */
#define SOMEIPXF_READ_U16_BUF(ConfigPtr, ConfigPtr_StartPos, BufPtr, BufPos, Val_U16) \
  do{ \
  (Val_U16) = (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 0U]]; \
  (Val_U16) = (((Val_U16) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 1U]]); \
  } while(0)

#if (defined SOMEIPXF_COPY_U32_BUF) /* to prevent double declaration */
#error SOMEIPXF_COPY_U32_BUF already defined
#endif /* if (defined SOMEIPXF_COPY_U32_BUF) */
#define SOMEIPXF_COPY_U32_BUF(ConfigPtr, ConfigPtr_StartPos, BufPtr, BufPos, Val_U32) \
  do{ \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 0U]] = (uint8) ((Val_U32) >> 24U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 1U]] = (uint8) ((Val_U32) >> 16U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 2U]] = (uint8) ((Val_U32) >> 8U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 3U]] = (uint8) ((Val_U32)); \
  } while(0)

#if (defined SOMEIPXF_COPY_U32_DATA_ELM) /* to prevent double declaration */
#error SOMEIPXF_COPY_U32_DATA_ELM already defined
#endif /* if (defined SOMEIPXF_COPY_U32_DATA_ELM) */
#define SOMEIPXF_COPY_U32_DATA_ELM(DataBytePtr, DataBytePos, Val_U32) \
  do{ \
  (DataBytePtr)[(DataBytePos)] = (uint8) ((Val_U32) >> 24U); \
  (DataBytePtr)[(DataBytePos) + 1U] = (uint8) ((Val_U32) >> 16U); \
  (DataBytePtr)[(DataBytePos) + 2U] = (uint8) ((Val_U32) >> 8U); \
  (DataBytePtr)[(DataBytePos) + 3U] = (uint8) ((Val_U32)); \
  } while(0)

#if (defined SOMEIPXF_READ_U32_BUF) /* to prevent double declaration */
#error SOMEIPXF_READ_U32_BUF already defined
#endif /* if (defined SOMEIPXF_READ_U32_BUF) */
#define SOMEIPXF_READ_U32_BUF(ConfigPtr, ConfigPtr_StartPos, BufPtr, BufPos, Val_U32) \
  do{ \
  (Val_U32) = (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 0U]]; \
  (Val_U32) = (((Val_U32) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 1U]]); \
  (Val_U32) = (((Val_U32) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 2U]]); \
  (Val_U32) = (((Val_U32) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 3U]]); \
  } while(0)

#if (defined SOMEIPXF_COPY_U64_BUF) /* to prevent double declaration */
#error SOMEIPXF_COPY_U64_BUF already defined
#endif /* if (defined SOMEIPXF_COPY_U64_BUF) */
#define SOMEIPXF_COPY_U64_BUF(ConfigPtr, ConfigPtr_StartPos, BufPtr, BufPos, Val_U64) \
  do{ \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 0U]] = (uint8) ((Val_U64) >> 56U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 1U]] = (uint8) ((Val_U64) >> 48U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 2U]] = (uint8) ((Val_U64) >> 40U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 3U]] = (uint8) ((Val_U64) >> 32U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 4U]] = (uint8) ((Val_U64) >> 24U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 5U]] = (uint8) ((Val_U64) >> 16U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 6U]] = (uint8) ((Val_U64) >> 8U); \
  (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 7U]] = (uint8) ((Val_U64)); \
  } while(0)

#if (defined SOMEIPXF_COPY_U64_DATA_ELM) /* to prevent double declaration */
#error SOMEIPXF_COPY_U64_DATA_ELM already defined
#endif /* if (defined SOMEIPXF_COPY_U64_DATA_ELM) */
#define SOMEIPXF_COPY_U64_DATA_ELM(DataBytePtr, DataBytePos, Val_U64) \
  do{ \
  (DataBytePtr)[(DataBytePos)] = (uint8) ((Val_U64) >> 56U); \
  (DataBytePtr)[(DataBytePos) + 1U] = (uint8) ((Val_U64) >> 48U); \
  (DataBytePtr)[(DataBytePos) + 2U] = (uint8) ((Val_U64) >> 40U); \
  (DataBytePtr)[(DataBytePos) + 3U] = (uint8) ((Val_U64) >> 32U); \
  (DataBytePtr)[(DataBytePos) + 4U] = (uint8) ((Val_U64) >> 24U); \
  (DataBytePtr)[(DataBytePos) + 5U] = (uint8) ((Val_U64) >> 16U); \
  (DataBytePtr)[(DataBytePos) + 6U] = (uint8) ((Val_U64) >> 8U); \
  (DataBytePtr)[(DataBytePos) + 7U] = (uint8) ((Val_U64)); \
  } while(0)

#if (defined SOMEIPXF_READ_U64_BUF) /* to prevent double declaration */
#error SOMEIPXF_READ_U64_BUF already defined
#endif /* if (defined SOMEIPXF_READ_U64_BUF) */
#define SOMEIPXF_READ_U64_BUF(ConfigPtr, ConfigPtr_StartPos, BufPtr, BufPos, Val_U64) \
  do{ \
  (Val_U64) = (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 0U]]; \
  (Val_U64) = (((Val_U64) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 1U]]); \
  (Val_U64) = (((Val_U64) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 2U]]); \
  (Val_U64) = (((Val_U64) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 3U]]); \
  (Val_U64) = (((Val_U64) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 4U]]); \
  (Val_U64) = (((Val_U64) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 5U]]); \
  (Val_U64) = (((Val_U64) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 6U]]); \
  (Val_U64) = (((Val_U64) << 8) | (BufPtr)[(BufPos) + (ConfigPtr)[(ConfigPtr_StartPos) + 7U]]); \
  } while(0)

/* Big Endian on platform to Little Endian on network */
#if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_1B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_1B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_1B) */
#define SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_1B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member)

#if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_2B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_2B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_2B) */
#define SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_2B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member)

#if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_4B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_4B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_4B) */
#define SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_4B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 3,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 2,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member)

#if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_8B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_8B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_8B) */
#define SomeIpXf_Config_LE_SET_STRUCT_CONFIG_NW_8B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 7,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 6,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 5,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 4,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 3,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 2,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member)

#if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_1B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_1B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_1B) */
#define SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_1B        \
  0

#if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_2B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_2B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_2B) */
#define SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_2B        \
  1, \
  0

#if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_4B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_4B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_4B) */
#define SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_4B        \
  3, \
  2, \
  1, \
  0

#if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_8B) /* to prevent double declaration */
#error SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_8B already defined
#endif /* if (defined SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_8B) */
#define SomeIpXf_Config_LE_SET_INTEGER_CONFIG_NW_8B        \
  7, \
  6, \
  5, \
  4, \
  3, \
  2, \
  1, \
  0

/* Big Endian on platform to Big Endian on network */
#if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_1B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_1B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_1B) */
#define SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_1B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member)

#if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_2B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_2B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_2B) */
#define SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_2B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member),                   \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1

#if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_4B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_4B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_4B) */
#define SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_4B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member),                   \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 2,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 3

#if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_8B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_8B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_8B) */
#define SomeIpXf_Config_BE_SET_STRUCT_CONFIG_NW_8B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member),                   \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 2,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 3,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 4,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 5,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 6,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 7

#if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_1B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_1B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_1B) */
#define SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_1B        \
  0

#if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_2B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_2B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_2B) */
#define SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_2B        \
  0, \
  1

#if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_4B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_4B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_4B) */
#define SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_4B        \
  0, \
  1, \
  2, \
  3

#if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_8B) /* to prevent double declaration */
#error SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_8B already defined
#endif /* if (defined SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_8B) */
#define SomeIpXf_Config_BE_SET_INTEGER_CONFIG_NW_8B        \
  0, \
  1, \
  2, \
  3, \
  4, \
  5, \
  6, \
  7

#endif


/* Little/Big Endian on platform to Opaque on network */
#if (defined SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_1B) /* to prevent double declaration */
#error SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_1B already defined
#endif /* if (defined SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_1B) */
#define SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_1B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member)

#if (defined SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_2B) /* to prevent double declaration */
#error SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_2B already defined
#endif /* if (defined SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_2B) */
#define SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_2B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member),                   \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1

#if (defined SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_4B) /* to prevent double declaration */
#error SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_4B already defined
#endif /* if (defined SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_4B) */
#define SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_4B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member),                   \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 2,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 3

#if (defined SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_8B) /* to prevent double declaration */
#error SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_8B already defined
#endif /* if (defined SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_8B) */
#define SomeIpXf_Config_OP_SET_STRUCT_CONFIG_NW_8B(Member) \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member),                   \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 1,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 2,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 3,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 4,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 5,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 6,               \
  offsetof(SOMEIPXF_DATA_TYPE_NAME, Member) + 7

#if (defined SOMEIPXF_ERROR_REACTION_THRESHOLD) /* to prevent double declaration */
#error SOMEIPXF_ERROR_REACTION_THRESHOLD already defined
#endif /* if (defined SOMEIPXF_ERROR_REACTION_THRESHOLD) */
/** \brief The autonomous error reaction occurs if parameter returnValue
 ** is greater or equal to 0x80U */
#define SOMEIPXF_ERROR_REACTION_THRESHOLD 0x80U

#if (defined SOMEIPXF_ADAPT_APPERROR_RANGE) /* to prevent double declaration */
#error SOMEIPXF_ADAPT_APPERROR_RANGE already defined
#endif /* if (defined SOMEIPXF_ADAPT_APPERROR_RANGE) */
/** \brief The number ranges for the return code field is adapted by adding
 ** 0x1F if at least one possibleError is defined */
#define SOMEIPXF_ADAPT_APPERROR_RANGE 0x1FU

#if (defined SOMEIPXF_SET_REQUEST_ID) /* to prevent double declaration */
#error SOMEIPXF_SET_REQUEST_ID already defined
#endif /* if (defined SOMEIPXF_SET_REQUEST_ID) */

#if (defined SOMEIPXF_GET_REQUEST_ID) /* to prevent double declaration */
#error SOMEIPXF_GET_REQUEST_ID already defined
#endif /* if (defined SOMEIPXF_GET_REQUEST_ID) */

/* macro to set request ID in header of client/server communication
 * IN: ClientId, SequenceCounter
 * OUT: BufPtr (pointer to the beginning of the header in the buffer) */
#define SOMEIPXF_SET_REQUEST_ID(ClientId, SequenceCounter, BufPtr)                   \
        (BufPtr)[0] = (uint8)((uint16)((ClientId) >> 8U) & (uint8)0xFFU);            \
        (BufPtr)[1] = (uint8)((ClientId) & (uint8)0xFFU);                            \
        (BufPtr)[2] = (uint8)((uint16)((SequenceCounter) >> 8U) & (uint8)0xFFU);     \
        (BufPtr)[3] = (uint8)((SequenceCounter) & (uint8)0xFFU)

/* macro to get request ID of client/server communication
 * IN: BufPtr (pointer to the beginning of the header in the buffer)
 * OUT: ClientId, SequenceCounter */
#define SOMEIPXF_GET_REQUEST_ID(ClientId, SequenceCounter, BufPtr)    \
        (ClientId)        = (uint16)(((uint16)((BufPtr)[1])) |        \
                            ((uint16)(((uint16)((BufPtr)[0]))<<8U))); \
        (SequenceCounter) = (uint16)(((uint16)((BufPtr)[3])) |        \
                            ((uint16)(((uint16)((BufPtr)[2]))<<8U)))

#if (defined SOMEIPXF_S_DECL_FUNC_INVXF_MESSAGE_ERROR_CALLOUT) /* to prevent double declaration */
#error SOMEIPXF_S_DECL_FUNC_INVXF_MESSAGE_ERROR_CALLOUT already defined
#endif /* if (defined SOMEIPXF_S_DECL_FUNC_INVXF_MESSAGE_ERROR_CALLOUT) */

#if (defined SOMEIPXF_MESSAGE_ERROR_CALLOUT) /* to prevent double declaration */
#error SOMEIPXF_MESSAGE_ERROR_CALLOUT already defined
#endif /* if (defined SOMEIPXF_MESSAGE_ERROR_CALLOUT) */

#if (SOMEIPXF_MSGERRORCALLOUT_AVAILABLE == STD_ON)

/* macro to generically declare Message Error Callouts */
#define SOMEIPXF_S_DECL_FUNC_INVXF_MESSAGE_ERROR_CALLOUT(XfrmName)               \
;                                                                                \
                                                                                 \
extern FUNC(void, SOMEIPXF_CODE) XfrmName ##_MsgErrorCallout                     \
(                                                                                \
    uint8 RetVal                                                                 \
)

/* Message Error Callout section for assembling SR, Client, Server and ET
 * inverse transformer functions */
#define SOMEIPXF_MESSAGE_ERROR_CALLOUT(XfrmName)                                \
   if(RetVal != E_OK)                                                           \
   {                                                                            \
      XfrmName ##_MsgErrorCallout(RetVal);                                      \
   }
#else
/* macros expand to empty */
#define SOMEIPXF_S_DECL_FUNC_INVXF_MESSAGE_ERROR_CALLOUT(XfrmName)

#define SOMEIPXF_MESSAGE_ERROR_CALLOUT(XfrmName)

#endif /* (SOMEIPXF_MSGERRORCALLOUT_AVAILABLE == STD_ON) */

/* -------------  Macros for transforming functions -----------------*/

#if (defined SOMEIPXF_ARG_PASSBYREF) /* to prevent double declaration */
#error SOMEIPXF_ARG_PASSBYREF already defined
#endif /* if (defined SOMEIPXF_ARG_PASSBYREF) */
#define SOMEIPXF_ARG_PASSBYREF(DataType) P2CONST(DataType, AUTOMATIC, SOMEIPXF_APPL_DATA)

#if (defined SOMEIPXF_ARG_PASSBYVAL) /* to prevent double declaration */
#error SOMEIPXF_ARG_PASSBYVAL already defined
#endif /* if (defined SOMEIPXF_ARG_PASSBYVAL) */
/* Primitive data types and enums are passed by value to transformer API by RTE */
#define SOMEIPXF_ARG_PASSBYVAL(DataType) DataType

#if (defined SOMEIPXF_PARAM_PASSBYREF) /* to prevent double declaration */
#error SOMEIPXF_PARAM_PASSBYREF already defined
#endif /* if (defined SOMEIPXF_PARAM_PASSBYREF) */
#define SOMEIPXF_PARAM_PASSBYREF

#if (defined SOMEIPXF_PARAM_PASSBYVAL) /* to prevent double declaration */
#error SOMEIPXF_PARAM_PASSBYVAL already defined
#endif /* if (defined SOMEIPXF_PARAM_PASSBYVAL) */
#define SOMEIPXF_PARAM_PASSBYVAL &

#if (defined SOMEIPXF_CHECKELEMENT_PASSBYREF) /* to prevent double declaration */
#error SOMEIPXF_CHECKELEMENT_PASSBYREF already defined
#endif /* if (defined SOMEIPXF_CHECKELEMENT_PASSBYREF) */
#define SOMEIPXF_CHECKELEMENT_PASSBYREF || (DataElement == NULL_PTR)

#if (defined SOMEIPXF_CHECKELEMENT_PASSBYVAL) /* to prevent double declaration */
#error SOMEIPXF_CHECKELEMENT_PASSBYVAL already defined
#endif /* if (defined SOMEIPXF_CHECKELEMENT_PASSBYVAL) */
#define SOMEIPXF_CHECKELEMENT_PASSBYVAL


#if (defined SOMEIPXF_CONFIGNAME) /* to prevent double declaration */
#error SOMEIPXF_CONFIGNAME already defined
#endif /* if (defined SOMEIPXF_CONFIGNAME) */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_CONFIGNAME(TypeConfigName, ConfigID)                           \
                            SomeIpXf_Config_ ## ConfigID

#if (defined SOMEIPXF_DEF_SESSIONHANDLINGVAR) /* to prevent double declaration */
#error SOMEIPXF_DEF_SESSIONHANDLINGVAR already defined
#endif /* if (defined SOMEIPXF_DEF_SESSIONHANDLINGVAR) */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_DEF_SESSIONHANDLINGVAR(XfrmName)                                                \
static VAR(uint16, SOMEIPXF_VAR) XfrmName ## _SessionHandlingCntVal = 0U

#if (defined SOMEIPXF_SESSION_HANDLING_SR) /* to prevent double declaration */
#error SOMEIPXF_SESSION_HANDLING_SR already defined
#endif /* if (defined SOMEIPXF_SESSION_HANDLING_SR) */
/* \brief Increment the session handling counter
 *        Reset the counter to 1 after roll over
 */
#define SOMEIPXF_SESSION_HANDLING_SR(x) ((x == SESSION_HANDLING_ID_COUNTER_MAX) ? x = 1U : x++ )

#if (defined SOMEIPXF_SR_SETHEADER_ACTIVE_SESSIONHANDLING) /* to prevent double declaration */
#error SOMEIPXF_SR_SETHEADER_ACTIVE_SESSIONHANDLING already defined
#endif /* if (defined SOMEIPXF_SR_SETHEADER_ACTIVE_SESSIONHANDLING) */
/* \brief Set the header for SR communication if session handling status is active */
#define SOMEIPXF_SR_SETHEADER_ACTIVE_SESSIONHANDLING(BufPtr, InterfaceVersion, MessageType,      \
         SessionId_Current_Value)                                                                \
    BufPtr[0] = 0U;                                                                              \
    BufPtr[1] = 0U;                                                                              \
    SOMEIPXF_SESSION_HANDLING_SR(SessionId_Current_Value);                                       \
    BufPtr[2] = (SessionId_Current_Value >> 8U) & 0xFFU;                                         \
    BufPtr[3] = SessionId_Current_Value & 0xFFU;                                                 \
    BufPtr[4] = (uint8)SOMEIPXF_PROTOCOL_VERSION;                                                \
    BufPtr[5] = InterfaceVersion;                                                                \
    BufPtr[6] = MessageType;                                                                     \
    BufPtr[7] = 0x00U;                                                                           \

#if (defined SOMEIPXF_SR_SETHEADER_INACTIVE_SESSIONHANDLING) /* to prevent double declaration */
#error SOMEIPXF_SR_SETHEADER_INACTIVE_SESSIONHANDLING already defined
#endif /* if (defined SOMEIPXF_SR_SETHEADER_INACTIVE_SESSIONHANDLING) */
/* \brief Set the header for SR communication if session handling status is inactive */
#define SOMEIPXF_SR_SETHEADER_INACTIVE_SESSIONHANDLING(BufPtr, InterfaceVersion, MessageType,    \
        Unused)                                                                                  \
    BufPtr[0] = 0U;                                                                              \
    BufPtr[1] = 0U;                                                                              \
    BufPtr[2] = 0U;                                                                              \
    BufPtr[3] = 0U;                                                                              \
    BufPtr[4] = (uint8)SOMEIPXF_PROTOCOL_VERSION;                                                \
    BufPtr[5] = InterfaceVersion;                                                                \
    BufPtr[6] = MessageType;                                                                     \
    BufPtr[7] = 0x00U;                                                                           \

#if (defined SOMEIPXF_SESSIONHANDLING_COUNTERVALUE) /* to prevent double declaration */
#error SOMEIPXF_SESSIONHANDLING_COUNTERVALUE already defined
#endif /* if (defined SOMEIPXF_SESSIONHANDLING_COUNTERVALUE) */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_SESSIONHANDLING_COUNTERVALUE(XfrmName)                                          \
                            XfrmName ## _SessionHandlingCntVal

#if (defined SOMEIPXF_SETHEADER_SESSIONHANDLING) /* to prevent double declaration */
#error SOMEIPXF_SETHEADER_SESSIONHANDLING already defined
#endif /* if (defined SOMEIPXF_SETHEADER_SESSIONHANDLING) */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_SETHEADER_SESSIONHANDLING(SessionHandling_Status)                               \
                            SOMEIPXF_SR_SETHEADER_ ## SessionHandling_Status

#if (defined SOMEIPXF_S_SR_DEF_FUNC_XF_HLP) /* to prevent double declaration */
#error SOMEIPXF_S_SR_DEF_FUNC_XF_HLP already defined
#endif /* if (defined SOMEIPXF_S_SR_DEF_FUNC_XF_HLP) */

#define SOMEIPXF_S_SR_DEF_FUNC_XF_HLP(XfrmName, DataType, PassMethodType, PassMethodCheck,       \
    PassMethod, InterfaceVersion, MessageType, TypeConfigName, Endianess, fctName,               \
    ConfigID, SessionHandling_Status)                                                            \
FUNC(uint8, SOMEIPXF_CODE) XfrmName                                                              \
(                                                                                                \
    P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,                                          \
    P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BufferLength,           \
    PassMethodType(DataType) DataElement                                                         \
)                                                                                                \
{                                                                                                \
    uint8 RetVal;                                                                                \
                                                                                                 \
    if((Buffer == NULL_PTR) || (BufferLength == NULL_PTR) PassMethodCheck)                       \
    {                                                                                            \
        RetVal = E_SER_GENERIC_ERROR;                                                            \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
         SOMEIPXF_SETHEADER_SESSIONHANDLING(SessionHandling_Status)(Buffer, InterfaceVersion,    \
             MessageType, SOMEIPXF_SESSIONHANDLING_COUNTERVALUE(XfrmName))                       \
                                                                                                 \
        *BufferLength = SOMEIPXF_HEADER_LENGTH;                                                  \
                                                                                                 \
        RetVal = fctName(                                                                        \
            &Buffer[*BufferLength],                                                              \
            BufferLength,                                                                        \
            (P2CONST(void, AUTOMATIC, SOMEIPXF_APPL_DATA))PassMethod DataElement,                \
            &SOMEIPXF_CONFIGNAME(TypeConfigName, ConfigID)[0]                                    \
        );                                                                                       \
    }                                                                                            \
    return RetVal;                                                                               \
}


#if (defined SOMEIPXF_S_SR_DEF_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_S_SR_DEF_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_S_SR_DEF_FUNC_XF) */
/* Macro to define safe transformer API function for sender/receiver communication
 * Parameters:
 * XfrmName   Name of the safe transformer
 * DataType   Data type of data element handed over for serialization
 * PassMethod   Method for passing data element to method (PASSBYREF or PASSBYVAL)
 * InterfaceVersion   The interface version the transformer shall use
 * MessageType   The Message Type which shall be placed into the SOME/IP header
 * TypeConfigName   Type of the data element which is part of configuration name
 * Endianess   Endianess of transformation which is part of the configuration name (LE, BE or OP)
 * SessionHandling_Status SessionHandling of transformation which is part of the configuration
 *                       (INACTIVE_SESSIONHANDLING, ACTIVE_SESSIONHANDLING)
 */
/* Deviation MISRAC2012-1 <+4> */
#define SOMEIPXF_S_SR_DEF_FUNC_XF(XfrmName, DataType, PassMethod, InterfaceVersion, MessageType,  \
        TypeConfigName, Endianess, ConfigID, SessionHandling_Status)                              \
SOMEIPXF_S_SR_DEF_FUNC_XF_HLP(XfrmName, DataType, SOMEIPXF_ARG_ ## PassMethod,                    \
    SOMEIPXF_CHECKELEMENT_ ## PassMethod, SOMEIPXF_PARAM_ ## PassMethod, InterfaceVersion,        \
    MessageType, TypeConfigName, Endianess, SomeIpXf_S_GenericTransformer, ConfigID, SessionHandling_Status)

#if (defined SOMEIPXF_S_SR_DECL_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_S_SR_DECL_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_S_SR_DECL_FUNC_XF) */
/* Deviation MISRAC2012-1 <+6> */
#define SOMEIPXF_S_SR_DECL_FUNC_XF(XfrmName, DataType, PassMethod)  \
extern FUNC(uint8, SOMEIPXF_CODE) XfrmName                          \
(                                                                   \
    P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,             \
    P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BufferLength,   \
    SOMEIPXF_ARG_ ## PassMethod(DataType) DataElement               \
)

#if (defined SOMEIPXF_S_SR_DEF_FUNC_INVXF_HLP) /* to prevent double declaration */
#error SOMEIPXF_S_SR_DEF_FUNC_INVXF_HLP already defined
#endif /* if (defined SOMEIPXF_S_SR_DEF_FUNC_INVXF_HLP) */

#define SOMEIPXF_S_SR_DEF_FUNC_INVXF_HLP(XfrmName, DataType, InterfaceVersion, TypeConfigName, \
    Endianess, fctName, ConfigID)                                                              \
FUNC(uint8, SOMEIPXF_CODE) XfrmName                                                            \
(                                                                                              \
    P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,                                      \
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,                                               \
    P2VAR(DataType, AUTOMATIC, SOMEIPXF_APPL_DATA) DataElement                                 \
)                                                                                              \
{                                                                                              \
    uint8 RetVal;                                                                              \
                                                                                               \
    /* check for executeDespiteDataUnavailability */                                           \
    if((Buffer == NULL_PTR) && (BufferLength == 0U))                                           \
    {                                                                                          \
        RetVal = E_NO_DATA;                                                                    \
    }                                                                                          \
    else if((Buffer == NULL_PTR) || (DataElement == NULL_PTR))                                 \
    {                                                                                          \
        RetVal = E_SER_GENERIC_ERROR;                                                          \
    }                                                                                          \
    else if (BufferLength < SOMEIPXF_HEADER_LENGTH)                                            \
    {                                                                                          \
        RetVal = E_SER_GENERIC_ERROR;                                                          \
    }                                                                                          \
    else                                                                                       \
    {                                                                                          \
        SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopiedFromBuffer = SOMEIPXF_HEADER_LENGTH;         \
        /* header of sender/receiver communication */                                          \
        RetVal = SomeIpXf_SR_GetHeader(Buffer, InterfaceVersion);                              \
        if(RetVal == E_OK)                                                                     \
        {                                                                                      \
            if(BufferLength > BytesCopiedFromBuffer)                                           \
            {                                                                                  \
                RetVal = fctName(                                                              \
                    &Buffer[BytesCopiedFromBuffer],                                            \
                    BufferLength,                                                              \
                    (P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA))DataElement,                   \
                    &BytesCopiedFromBuffer,                                                    \
                    &SOMEIPXF_CONFIGNAME(TypeConfigName, ConfigID)[0]                          \
                );                                                                             \
                                                                                               \
                if( (RetVal == E_OK) && (BytesCopiedFromBuffer > BufferLength) )               \
                {                                                                              \
                  RetVal = E_SER_MALFORMED_MESSAGE;                                            \
                }                                                                              \
            }                                                                                  \
            else                                                                               \
            {                                                                                  \
                RetVal = E_SER_MALFORMED_MESSAGE;                                              \
            }                                                                                  \
        }                                                                                      \
    }

#if (defined SOMEIPXF_S_SR_DEF_FUNC_INVXF_HLP_END) /* to prevent double declaration */
#error SOMEIPXF_S_SR_DEF_FUNC_INVXF_HLP_END already defined
#endif /* if (defined SOMEIPXF_S_SR_DEF_FUNC_INVXF_HLP_END) */

#define SOMEIPXF_S_SR_DEF_FUNC_INVXF_HLP_END               \
   return RetVal;                                          \
}

#if (defined SOMEIPXF_S_SR_DEF_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_S_SR_DEF_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_S_SR_DEF_FUNC_INVXF) */
/* Macro to define safe inverse-transformer API function for sender/receiver communication
 * Parameters:
 * XfrmName   Name of the safe inverse transformer
 * DataType   Data type of element where data is de-serialized into
 * InterfaceVersion   The interface version the inverse transformer shall use
 * TypeConfigName   Type of the data element which is part of configuration name
 * Endianess   Endianess of transformation which is part of the configuration name (LE, BE or OP)
 */
#define SOMEIPXF_S_SR_DEF_FUNC_INVXF(XfrmName, DataType, InterfaceVersion, TypeConfigName,        \
    Endianess, ConfigID)                                                                          \
SOMEIPXF_S_SR_DEF_FUNC_INVXF_HLP(XfrmName, DataType, InterfaceVersion, TypeConfigName, Endianess, \
    SomeIpXf_S_Inv_GenericTransformer, ConfigID)                                                  \
SOMEIPXF_MESSAGE_ERROR_CALLOUT(XfrmName)                                                          \
SOMEIPXF_S_SR_DEF_FUNC_INVXF_HLP_END

#if (defined SOMEIPXF_S_SR_DECL_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_S_SR_DECL_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_S_SR_DECL_FUNC_INVXF) */
#define SOMEIPXF_S_SR_DECL_FUNC_INVXF(XfrmName, DataType)         \
extern FUNC(uint8, SOMEIPXF_CODE) XfrmName                        \
(                                                                 \
    P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,         \
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,                  \
    P2VAR(DataType, AUTOMATIC, SOMEIPXF_APPL_DATA) DataElement    \
)SOMEIPXF_S_DECL_FUNC_INVXF_MESSAGE_ERROR_CALLOUT(XfrmName)

#if (defined SOMEIPXF_CAT) /* to prevent double declaration */
#error SOMEIPXF_CAT already defined
#endif /* if (defined SOMEIPXF_CAT) */

#if (defined SOMEIPXF_CAT_HLP) /* to prevent double declaration */
#error SOMEIPXF_CAT_HLP already defined
#endif /* if (defined SOMEIPXF_CAT_HLP) */

#define SOMEIPXF_CAT(x, y) SOMEIPXF_CAT_HLP(x, y)
/* Deviation MISRAC2012-1 */
#define SOMEIPXF_CAT_HLP(x, y) x ## y


/* ---------------------------- Client MACROS --------------------------------- */
/* Macro guards */
#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_START) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_START already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_START) */

#if (defined SOMEIPXF_S_CLIENT_XF_PARAM_GEN) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_PARAM_GEN already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_PARAM_GEN) */

#if (defined SOMEIPXF_S_CLIENT_XF_PARAM) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_PARAM already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_PARAM) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_PARAMLIST) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_PARAMLIST already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_PARAMLIST) */

#if (defined SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_1) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_1 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_1) */

#if (defined SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_2) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_2 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_2) */

#if (defined SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_1_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_1_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_1_END) */

#if (defined SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_2_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_2_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_2_END) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_MID_1) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_MID_1 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_MID_1) */

#if (defined SOMEIPXF_S_CLIENT_XF_BUFCHECK_PASSBYREF) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_BUFCHECK_PASSBYREF already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_BUFCHECK_PASSBYREF) */

#if (defined SOMEIPXF_S_CLIENT_XF_BUFCHECK_PASSBYVAL) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_BUFCHECK_PASSBYVAL already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_BUFCHECK_PASSBYVAL) */

#if (defined SOMEIPXF_S_CLIENT_XF_BUFCHECK_GEN) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_BUFCHECK_GEN already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_BUFCHECK_GEN) */

#if (defined SOMEIPXF_S_CLIENT_XF_BUFCHECK) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_BUFCHECK already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_BUFCHECK) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_BUFFCHECK) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_BUFFCHECK already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_BUFFCHECK) */

#if (defined SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_1) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_1 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_1) */

#if (defined SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_2) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_2 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_2) */

#if (defined SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_1_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_1_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_1_END) */

#if (defined SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_2_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_2_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_2_END) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_MID_2) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_MID_2 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_MID_2) */

#if (defined SOMEIPXF_S_CLIENT_XF_CALL_GEN) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_CALL_GEN already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_CALL_GEN) */

#if (defined SOMEIPXF_S_CLIENT_XF_CALL) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_XF_CALL already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_XF_CALL) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF_CALLS) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_XF_CALLS already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF_CALLS) */

#if (defined SOMEIPXF_S_CLIENT_LOOP_XF_CALL_1) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_LOOP_XF_CALL_1 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_LOOP_XF_CALL_1) */

#if (defined SOMEIPXF_S_CLIENT_LOOP_XF_CALL_2) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_LOOP_XF_CALL_2 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_LOOP_XF_CALL_2) */

#if (defined SOMEIPXF_S_CLIENT_LOOP_XF_CALL_1_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_LOOP_XF_CALL_1_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_LOOP_XF_CALL_1_END) */

#if (defined SOMEIPXF_S_CLIENT_LOOP_XF_CALL_2_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_LOOP_XF_CALL_2_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_LOOP_XF_CALL_2_END) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_END) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_XF) */
/* End of Macro guards */


/* Static Start-section */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_START(XfrmName)                                  \
FUNC(uint8, SOMEIPXF_CODE) XfrmName                                                         \
(                                                                                           \
    P2CONST(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle, \
    P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,                                     \
    P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BufferLength


/* Create Parameter List for function call */
/* Deviation MISRAC2012-1 <+3> */
#define SOMEIPXF_S_CLIENT_XF_PARAM_GEN(DataType, Nr, PassMethod, config)  \
,\
    SOMEIPXF_ARG_ ## PassMethod(DataType) Data_ ## Nr

/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_CLIENT_XF_PARAM(ARG_ID)                \
SOMEIPXF_S_CLIENT_XF_PARAM_GEN SOMEIPXF_CLIENT_ ## ARG_ID

/* Macro loop */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_PARAMLIST(SEQ) SOMEIPXF_CAT(SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_1 SEQ, _END)
#define SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_1(x) SOMEIPXF_S_CLIENT_XF_PARAM(x) SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_2
#define SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_2(x) SOMEIPXF_S_CLIENT_XF_PARAM(x) SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_1
#define SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_1_END
#define SOMEIPXF_S_CLIENT_XF_LOOP_PARAM_2_END
/* End of Create Parameter List for function call */


/* Static Mid-section 1 */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_MID_1                                                  \
)                                                                                                 \
{                                                                                                 \
    /* init. with E_OK to avoid compiler warning "condition always true" in else branch below */  \
    uint8 RetVal = E_OK;                                                                          \
                                                                                                  \
    if((Buffer == NULL_PTR) || (BufferLength == NULL_PTR) || (TransactionHandle == NULL_PTR)


/* Create Buffer check */
/* Nullptr check needed */
/* Deviation MISRAC2012-1 <+3> */
#define SOMEIPXF_S_CLIENT_XF_BUFCHECK_PASSBYREF(Nr) \
||                                                  \
       (Data_ ## Nr == NULL_PTR)

/* No Nullptr check needed, expands to nothing */
#define SOMEIPXF_S_CLIENT_XF_BUFCHECK_PASSBYVAL(Nr)

/* Insert Nullptr check if needed with helper macros above */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_CLIENT_XF_BUFCHECK_GEN(DataType, Nr, PassMethod, config)    \
SOMEIPXF_S_CLIENT_XF_BUFCHECK_ ## PassMethod(Nr)

/* Create NullPointer Check List for function call */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_CLIENT_XF_BUFCHECK(ARG_ID)                \
SOMEIPXF_S_CLIENT_XF_BUFCHECK_GEN SOMEIPXF_CLIENT_ ## ARG_ID

/* Macro loop */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_BUFFCHECK(SEQ) SOMEIPXF_CAT(SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_1 SEQ, _END)
#define SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_1(x) SOMEIPXF_S_CLIENT_XF_BUFCHECK(x) SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_2
#define SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_2(x) SOMEIPXF_S_CLIENT_XF_BUFCHECK(x) SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_1
#define SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_1_END
#define SOMEIPXF_S_CLIENT_XF_LOOP_BUFCHECK_2_END
/* End of Create Buffer check */


/* Mid-section 2 */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_MID_2(InterfaceVersion)                  \
)                                                                                   \
    {                                                                               \
        /* defensive programming mechanism (no Det interface) */                    \
        RetVal = E_SER_GENERIC_ERROR;                                               \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        /* header of client/server communication */                                 \
        SomeIpXf_Request_SetHeader(TransactionHandle, Buffer, InterfaceVersion);    \
                                                                                    \
        *BufferLength = SOMEIPXF_HEADER_LENGTH;


/* Create Function call */

/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+7> */
#define SOMEIPXF_S_CLIENT_XF_CALL_GEN(DataType, Nr, PassMethod, config)              \
        if(RetVal == E_OK)                                                           \
        {                                                                            \
              RetVal = SomeIpXf_S_GenericTransformer(                                \
                  &Buffer[*BufferLength],                                            \
                  BufferLength,                                                      \
                  (P2CONST(void, AUTOMATIC, SOMEIPXF_APPL_DATA)) SOMEIPXF_PARAM_ ## PassMethod(Data_ ## Nr),   \
                  &config[0]                                                         \
              );                                                                     \
        }

/* List of Transformer calls */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_CLIENT_XF_CALL(ARG_ID)                \
SOMEIPXF_S_CLIENT_XF_CALL_GEN SOMEIPXF_CLIENT_ ## ARG_ID

/* Macro loop */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_XF_CALLS(SEQ) SOMEIPXF_CAT(SOMEIPXF_S_CLIENT_LOOP_XF_CALL_1 SEQ, _END)
#define SOMEIPXF_S_CLIENT_LOOP_XF_CALL_1(x) SOMEIPXF_S_CLIENT_XF_CALL(x) SOMEIPXF_S_CLIENT_LOOP_XF_CALL_2
#define SOMEIPXF_S_CLIENT_LOOP_XF_CALL_2(x) SOMEIPXF_S_CLIENT_XF_CALL(x) SOMEIPXF_S_CLIENT_LOOP_XF_CALL_1
#define SOMEIPXF_S_CLIENT_LOOP_XF_CALL_1_END
#define SOMEIPXF_S_CLIENT_LOOP_XF_CALL_2_END
/* End of Create Function call */


/* static End section */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_END     \
    }                                              \
                                                   \
    return RetVal;                                 \
}


/* Macro to define safe transformer API function for Client communication
 * Parameters:
 * XfrmName   Name of the inverse transformer
 * InterfaceVersion   The interface version the inverse transformer shall use
 */
/* Deviation MISRAC2012-1 <+4> */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_XF(XfrmName, InterfaceVersion)                                  \
SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_START(XfrmName) SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_PARAMLIST( SOMEIPXF_CLIENT_SEQ_XF_ ## XfrmName )       \
SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_MID_1 SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_BUFFCHECK( SOMEIPXF_CLIENT_SEQ_XF_ ## XfrmName ) SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_MID_2( InterfaceVersion )  \
        SOMEIPXF_S_CLIENT_DEF_FUNC_XF_CALLS( SOMEIPXF_CLIENT_SEQ_XF_ ## XfrmName )                 \
    SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_END


/* ----- Function declaration ----- */

#if (defined SOMEIPXF_S_CLIENT_DECL_FUNC_XF_SECT_START) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DECL_FUNC_XF_SECT_START already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DECL_FUNC_XF_SECT_START) */

#if (defined SOMEIPXF_S_CLIENT_DECL_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DECL_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DECL_FUNC_XF) */

/* Static Start-section */
#define SOMEIPXF_S_CLIENT_DECL_FUNC_XF_SECT_START(XfrmName)                                 \
extern FUNC(uint8, SOMEIPXF_CODE) XfrmName                                                  \
(                                                                                           \
    P2CONST(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle, \
    P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,                                     \
    P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BufferLength

/* Assemble parts for declaration */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_CLIENT_DECL_FUNC_XF(XfrmName)                                             \
SOMEIPXF_S_CLIENT_DECL_FUNC_XF_SECT_START(XfrmName) SOMEIPXF_S_CLIENT_DEF_FUNC_XF_SECT_PARAMLIST( SOMEIPXF_CLIENT_SEQ_XF_ ## XfrmName )    \
)                                                                                            \

/* ----- End of Function declaration ----- */
/* --------------------- End of Client MACROS --------------------------------- */


/* ---------------------------- INV-Client MACROS --------------------------------- */
/* Macro guards */
#if (defined SOMEIPXF_S_CLIENT_INVXF_START_NORETVAL) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_START_NORETVAL already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_START_NORETVAL) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_START_RETVAL) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_START_RETVAL already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_START_RETVAL) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_START) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_START already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_START) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_PARAM_GEN) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_PARAM_GEN already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_PARAM_GEN) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_PARAM) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_PARAM already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_PARAM) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_PARAMLIST_NOARG) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_PARAMLIST_NOARG already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_PARAMLIST_NOARG) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_PARAMLIST_ARG) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_PARAMLIST_ARG already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_PARAMLIST_ARG) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_1) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_1 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_1) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_2) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_2 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_2) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_1_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_1_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_1_END) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_2_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_2_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_2_END) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_1) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_1 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_1) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_BUFCHECK_GEN) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_BUFCHECK_GEN already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_BUFCHECK_GEN) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_BUFCHECK) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_BUFCHECK already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_BUFCHECK) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_BUFFCHECK_ARG) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_BUFFCHECK_ARG already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_BUFFCHECK_ARG) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_BUFFCHECK_NOARG) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_BUFFCHECK_NOARG already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_BUFFCHECK_NOARG) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_1) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_1 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_1) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_2) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_2 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_2) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_1_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_1_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_1_END) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_2_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_2_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_2_END) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2_RETVAL) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2_RETVAL already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2_RETVAL) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2_NORETVAL) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2_NORETVAL already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2_NORETVAL) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_CALL_GEN) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_CALL_GEN already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_CALL_GEN) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_CALL_GEN_TLV) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_CALL_GEN_TLV already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_CALL_GEN_TLV) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_CALL) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_CALL already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_CALL) */

#if (defined SOMEIPXF_S_CLIENT_INVXF_CALL_TLV) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_INVXF_CALL_TLV already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_INVXF_CALL_TLV) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_CALLS_TLV_ARG) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_CALLS_TLV_ARG already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_CALLS_TLV_ARG) */

#if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_1) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_1 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_1) */

#if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_2) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_2 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_2) */

#if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_1_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_1_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_1_END) */

#if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_2_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_2_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_2_END) */

#if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_11) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_11 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_11) */

#if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_22) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_22 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_22) */

#if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_11_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_11_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_11_END) */

#if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_22_END) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_22_END already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_22_END) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END1_TLV) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END1_TLV already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END1_TLV) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END1_NON_TLV) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END1_NON_TLV already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END1_NON_TLV) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END2) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END2 already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END2) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_FIND_DUPLICATEID_TLV) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_FIND_DUPLICATEID_TLV already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_FIND_DUPLICATEID_TLV) */

#if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_FIND_DUPLICATEID_NON_TLV) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_FIND_DUPLICATEID_NON_TLV already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_FIND_DUPLICATEID_NON_TLV) */
/* Macro guards */


/* Start-section */
#define SOMEIPXF_S_CLIENT_INVXF_START_NORETVAL

#define SOMEIPXF_S_CLIENT_INVXF_START_RETVAL  ,\
    P2VAR(Std_ReturnType, AUTOMATIC, SOMEIPXF_APPL_DATA) ReturnValue

/* Deviation MISRAC2012-1 <+6> */
#define SOMEIPXF_S_CLIENT_INVXF_START(XfrmName, HasReturnValue)                                 \
FUNC(uint8, SOMEIPXF_CODE) XfrmName                                                             \
(                                                                                               \
    P2VAR(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle,       \
    P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,                                       \
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength SOMEIPXF_S_CLIENT_INVXF_START_ ## HasReturnValue
/* End of Start-section */


/* Create Parameter List for inverse function call */
/* Deviation MISRAC2012-1 <+3> */
#define SOMEIPXF_S_CLIENT_INVXF_PARAM_GEN(DataType, Nr, PassMethod, config)    \
    ,\
    P2VAR(DataType, AUTOMATIC, SOMEIPXF_APPL_DATA) Data_ ## Nr

/* Create Parameter List for inverse function call */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_CLIENT_INVXF_PARAM(ARG_ID)    \
    SOMEIPXF_S_CLIENT_INVXF_PARAM_GEN SOMEIPXF_CLIENT_ ## ARG_ID

/* No Arguments */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_PARAMLIST_NOARG(UNUSED) \

#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_PARAMLIST_ARG(SEQ) SOMEIPXF_CAT(SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_1 SEQ, _END)
#define SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_1(x) SOMEIPXF_S_CLIENT_INVXF_PARAM(x) SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_2
#define SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_2(x) SOMEIPXF_S_CLIENT_INVXF_PARAM(x) SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_1
#define SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_1_END
#define SOMEIPXF_S_CLIENT_INVXF_LOOP_PARAM_2_END
/* End of Create Parameter List for inverse function call */

/* Mid-section 1 */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_1         \
)                                                           \
{                                                           \
    uint8 RetVal;                                           \
                                                            \
    /* check for executeDespiteDataUnavailability */        \
    if((Buffer == NULL_PTR) && (BufferLength == 0U))        \
    {                                                       \
        RetVal = E_NO_DATA;                                 \
    }                                                       \
    else                                                    \
    {                                                       \
        if((Buffer == NULL_PTR) || (TransactionHandle == NULL_PTR)


/* Creation of Nullptr checks */
/* Deviation MISRAC2012-1 <+3> */
#define SOMEIPXF_S_CLIENT_INVXF_BUFCHECK_GEN(DataType, Nr, PassMethod, config)    \
||                                                                                \
           (Data_ ## Nr == NULL_PTR)

/* Create NullPointer Check List for function call */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_CLIENT_INVXF_BUFCHECK(ARG_ID)                 \
SOMEIPXF_S_CLIENT_INVXF_BUFCHECK_GEN SOMEIPXF_CLIENT_ ## ARG_ID

/* No Arguments */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_BUFFCHECK_NOARG(UNUSED)

#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_BUFFCHECK_ARG(SEQ) SOMEIPXF_CAT(SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_1 SEQ, _END)
#define SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_1(x) SOMEIPXF_S_CLIENT_INVXF_BUFCHECK(x) SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_2
#define SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_2(x) SOMEIPXF_S_CLIENT_INVXF_BUFCHECK(x) SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_1
#define SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_1_END
#define SOMEIPXF_S_CLIENT_INVXF_LOOP_BUFCHECK_2_END
/* End of Creation of Nullptr checks */


/* Mid-section 2 */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2_RETVAL ReturnValue
#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2_NORETVAL NULL_PTR

/* Deviation MISRAC2012-1 <+17> */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2(InterfaceVersion, HasReturnValue)            \
)                                                                                                \
        {                                                                                        \
            /* defensive programming mechanism (no Det interface) */                             \
            RetVal = E_SER_GENERIC_ERROR;                                                        \
        }                                                                                        \
        else if (BufferLength < SOMEIPXF_HEADER_LENGTH)                                          \
        {                                                                                        \
            /* defensive programming mechanism (no Det interface) */                             \
            RetVal = E_SER_GENERIC_ERROR;                                                        \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopiedFromBuffer = SOMEIPXF_HEADER_LENGTH;       \
                                                                                                 \
            /* header of client/server communication */                                          \
            RetVal = SomeIpXf_Response_GetHeader(Buffer, InterfaceVersion, TransactionHandle, SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2_ ## HasReturnValue); \
                                                                                                 \

/* End of Mid-section 2 */

#define SOMEIPXF_CLIENT_HELP_END

/* Get the config of first argument of i-signal */
#define SOMEIPXF_S_CLIENT_INVXF_GET_CONFIGID_HELPER(DataType, Nr, PassMethod, config) config

#define SOMEIPXF_S_CLIENT_INVXF_GET_CONFIGID(XfrmName)                  \
SOMEIPXF_CAT(SOMEIPXF_S_CLIENT_INVXF_GET_CONFIGID_HELPER SOMEIPXF_CLIENT_INVXF_ ## XfrmName ## _ARG_1 SOMEIPXF_CLIENT_HELP, _END)

#if (SOMEIPXF_TLV_ARGUMENT_UNIQUE_DATAID_CHECK_ENABLE == STD_ON)
/* Deviation MISRAC2012-1 <+11> */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_FIND_DUPLICATEID_TLV(XfrmName)                        \
if(RetVal == E_OK)                                                                             \
{                                                                                              \
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLVArgIDListSize = SOMEIPXF_TLV_ARG_IDS_COUNT_ ## XfrmName;   \
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLVArgID[SOMEIPXF_TLV_ARG_IDS_COUNT_ ## XfrmName] =           \
            {SOMEIPXF_TLV_ARG_IDS_ ## XfrmName};                                               \
             BytesCopiedFromBuffer = 8U;                                                       \
           /* Check multiple known data id's */                                                \
            RetVal = SomeIpXf_CheckMultipleKnownDataIDs(                                       \
                &Buffer[BytesCopiedFromBuffer],                                                \
                BufferLength,                                                                  \
                TLVArgIDListSize,                                                              \
                &TLVArgID[0U], /* TLV Arg ID list of OUT/IN-OUT Parameters */                  \
      &SOMEIPXF_S_CLIENT_INVXF_GET_CONFIGID(XfrmName)[0U]);                                    \
}                                                                                              \

#else
/* macros expand to empty */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_FIND_DUPLICATEID_TLV(XfrmName) /* Optimization enabled. Duplicate dataid's shall not be found*/
#endif /* (SOMEIPXF_TLV_ARGUMENT_UNIQUE_DATAID_CHECK_ENABLE == STD_ON) */

/* macros expand to empty */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_FIND_DUPLICATEID_NON_TLV(XfrmName) /* No TLV DataId's */

/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+10> */
#define SOMEIPXF_S_CLIENT_INVXF_CALL_GEN(DataType, Nr, PassMethod, config)            \
                                                                                      \
            if(RetVal == E_OK)                                                        \
            {                                                                         \
                if(BufferLength > BytesCopiedFromBuffer)                              \
                {                                                                     \
                    RetVal = SomeIpXf_S_Inv_GenericTransformer(                       \
                        &Buffer[BytesCopiedFromBuffer],                               \
                        BufferLength,                                                 \
                        (P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA))Data_ ## Nr,      \
                        &BytesCopiedFromBuffer,                                       \
                        &config[0]                                                    \
                    );                                                                \
                }                                                                     \
                else                                                                  \
                {                                                                     \
                    RetVal = E_SER_MALFORMED_MESSAGE;                                 \
                }                                                                     \
            }                                                                         \
                                                                                      \

#if (SOMEIPXF_TLV_ARGUMENT_REORDERING_ENABLE == STD_ON)
/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+10> */
#define SOMEIPXF_S_CLIENT_INVXF_CALL_GEN_TLV(DataType, Nr, PassMethod, config)          \
                                                                                        \
        if(RetVal == E_OK)                                                              \
        {                                                                               \
            /* Start again */                                                           \
            BytesCopiedFromBuffer = 8U;                                                 \
            {                                                                           \
                RetVal = SomeIpXf_S_Inv_GenericTransformer(                             \
                    &Buffer[BytesCopiedFromBuffer],                                     \
                    BufferLength,                                                       \
                    (P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA))Data_ ## Nr,            \
                    &BytesCopiedFromBuffer,                                             \
                    &config[0]                                                          \
                );                                                                      \
            }                                                                           \
            if( (RetVal == E_OK) && (BufferLength < BytesCopiedFromBuffer))             \
            {                                                                           \
                RetVal = E_SER_MALFORMED_MESSAGE;                                       \
            }                                                                           \
        }                                                                               \

#else
/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+10> */
#define SOMEIPXF_S_CLIENT_INVXF_CALL_GEN_TLV(DataType, Nr, PassMethod, config)        \
                                                                                      \
            if(RetVal == E_OK)                                                        \
            {                                                                         \
                if(BufferLength > BytesCopiedFromBuffer)                              \
                {                                                                     \
                    RetVal = SomeIpXf_S_Inv_GenericTransformer(                       \
                        &Buffer[BytesCopiedFromBuffer],                               \
                        BufferLength,                                                 \
                        (P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA))Data_ ## Nr,      \
                        &BytesCopiedFromBuffer,                                       \
                        &config[0]                                                    \
                    );                                                                \
                }                                                                     \
                else                                                                  \
                {                                                                     \
                    RetVal = E_SER_MALFORMED_MESSAGE;                                 \
                }                                                                     \
            }                                                                         \
                                                                                      \

#endif /* (SOMEIPXF_TLV_ARGUMENT_REORDERING_ENABLE == STD_ON) */

/* List of SAFE Transformer calls */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_CLIENT_INVXF_CALL(ARG_ID)                  \
SOMEIPXF_S_CLIENT_INVXF_CALL_GEN SOMEIPXF_CLIENT_ ## ARG_ID

#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG(SEQ) SOMEIPXF_CAT(SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_1 SEQ, _END)
#define SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_1(x) SOMEIPXF_S_CLIENT_INVXF_CALL(x) SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_2
#define SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_2(x) SOMEIPXF_S_CLIENT_INVXF_CALL(x) SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_1
#define SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_1_END
#define SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_2_END

/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_CLIENT_INVXF_CALL_TLV(ARG_ID)                  \
SOMEIPXF_S_CLIENT_INVXF_CALL_GEN_TLV SOMEIPXF_CLIENT_ ## ARG_ID

#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_CALLS_TLV_ARG(SEQ) SOMEIPXF_CAT(SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_11 SEQ, _END)
#define SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_11(x) SOMEIPXF_S_CLIENT_INVXF_CALL_TLV(x) SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_22
#define SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_22(x) SOMEIPXF_S_CLIENT_INVXF_CALL_TLV(x) SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_11
#define SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_11_END
#define SOMEIPXF_S_CLIENT_LOOP_INVXF_CALL_22_END

/* No Arguments */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG(UNUSED) \
    SOMEIPXF_PARAM_UNUSED(BytesCopiedFromBuffer);
/* End of Create transformer function calls */


/* static End section 1 */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END1_NON_TLV                   \
            if( (RetVal == E_OK) && (BufferLength < BytesCopiedFromBuffer))  \
            {                                                                \
                RetVal = E_SER_MALFORMED_MESSAGE;                            \
            }                                                                \
        }                                                                    \
    }

#if(SOMEIPXF_TLV_ARGUMENT_REORDERING_ENABLE == STD_ON)
/* static End section 1 */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END1_TLV                       \
        }                                                                    \
    }
#else
/* static End section 1 */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END1_TLV                       \
            if( (RetVal == E_OK) && (BufferLength < BytesCopiedFromBuffer))  \
            {                                                                \
                RetVal = E_SER_MALFORMED_MESSAGE;                            \
            }                                                                \
        }                                                                    \
    }
#endif /* (SOMEIPXF_TLV_ARGUMENT_REORDERING_ENABLE == STD_ON) */

#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END2                           \
    return RetVal;                                                           \
}


/* Macro to define safe inverse transformer API function for Client communication
 * Parameters:
 * XfrmName   Name of the inverse transformer
 * InterfaceVersion   The interface version the inverse transformer shall use
 * HasReturnValue   Function has return value (RETVAL, NORETVAL)
 * HasArguments   Function has one ore more Arguments, e.g. Data_1 (ARG, NOARG)
 * HasAutonomousErrorReact   Function has autonomous Error reaction enabled/disabled
 *                           (AUTONERR, NOAUTONERR)
 */
/* Deviation MISRAC2012-1 <+8> */
#define SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF(XfrmName, InterfaceVersion, HasReturnValue, HasArguments, IsTlv )  \
SOMEIPXF_S_CLIENT_INVXF_START(XfrmName, HasReturnValue)SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_PARAMLIST_ ## HasArguments( SOMEIPXF_CLIENT_SEQ_INVXF_ ## XfrmName )    \
SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_1 SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_BUFFCHECK_ ## HasArguments( SOMEIPXF_CLIENT_SEQ_INVXF_ ## XfrmName ) SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_MID_2( InterfaceVersion, HasReturnValue ) \
SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_CALLS_ ## IsTlv ##_ ## HasArguments( SOMEIPXF_CLIENT_SEQ_INVXF_ ## XfrmName )   \
SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_FIND_DUPLICATEID_ ## IsTlv(XfrmName)                   \
SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END1_ ## IsTlv                                    \
SOMEIPXF_MESSAGE_ERROR_CALLOUT(XfrmName)                                                \
SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_END2

/* ----- INV Function declaration ----- */
#if (defined SOMEIPXF_S_CLIENT_DECL_FUNC_INVXF_START) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DECL_FUNC_INVXF_START already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DECL_FUNC_INVXF_START) */

#if (defined SOMEIPXF_S_CLIENT_DECL_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_S_CLIENT_DECL_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_S_CLIENT_DECL_FUNC_INVXF) */

/* Static Start-section */
/* Deviation MISRAC2012-1 <+6> */
#define SOMEIPXF_S_CLIENT_DECL_FUNC_INVXF_START(XfrmName, HasReturnValue)                    \
extern FUNC(uint8, SOMEIPXF_CODE) XfrmName                                                   \
(                                                                                            \
    P2VAR(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle,    \
    P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,                                    \
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength SOMEIPXF_S_CLIENT_INVXF_START_ ## HasReturnValue

/* Assemble parts for declaration */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_CLIENT_DECL_FUNC_INVXF(XfrmName, InterfaceVersion, HasReturnValue, HasArguments )  \
SOMEIPXF_S_CLIENT_DECL_FUNC_INVXF_START(XfrmName, HasReturnValue) SOMEIPXF_S_CLIENT_DEF_FUNC_INVXF_SECT_PARAMLIST_ ## HasArguments( SOMEIPXF_CLIENT_SEQ_INVXF_ ## XfrmName )    \
)SOMEIPXF_S_DECL_FUNC_INVXF_MESSAGE_ERROR_CALLOUT(XfrmName)

/* ----- INV Function declaration ----- */
/* --------------------- End of INV-Client MACROS --------------------------------- */


/* --------------------- Server MACROS --------------------------------- */
/* Macro guards */
#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START_NORETVAL) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START_NORETVAL already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START_NORETVAL) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START_RETVAL) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START_RETVAL already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START_RETVAL) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START) */

#if (defined SOMEIPXF_S_SERVER_XF_PARAM_GEN) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_PARAM_GEN already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_PARAM_GEN) */

#if (defined SOMEIPXF_S_SERVER_XF_PARAM) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_PARAM already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_PARAM) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_PARAMLIST) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_PARAMLIST already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_PARAMLIST) */

#if (defined SOMEIPXF_S_SERVER_XF_LOOP_PARAM_1) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_LOOP_PARAM_1 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_LOOP_PARAM_1) */

#if (defined SOMEIPXF_S_SERVER_XF_LOOP_PARAM_2) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_LOOP_PARAM_2 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_LOOP_PARAM_2) */

#if (defined SOMEIPXF_S_SERVER_XF_LOOP_PARAM_1_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_LOOP_PARAM_1_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_LOOP_PARAM_1_END) */

#if (defined SOMEIPXF_S_SERVER_XF_LOOP_PARAM_2_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_LOOP_PARAM_2_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_LOOP_PARAM_2_END) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_NOARG_NOAUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_NOARG_NOAUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_NOARG_NOAUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_NORETVAL_NOARG_NOAUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_NORETVAL_NOARG_NOAUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_NORETVAL_NOARG_NOAUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_NOARG_AUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_NOARG_AUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_NOARG_AUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_ARG_NOAUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_ARG_NOAUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_ARG_NOAUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_NORETVAL_ARG_NOAUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_NORETVAL_ARG_NOAUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_NORETVAL_ARG_NOAUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_ARG_AUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_ARG_AUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_ARG_AUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1) */

#if (defined SOMEIPXF_S_SERVER_XF_BUFCHECK_PASSBYREF) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_BUFCHECK_PASSBYREF already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_BUFCHECK_PASSBYREF) */

#if (defined SOMEIPXF_S_SERVER_XF_BUFCHECK_PASSBYVAL) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_BUFCHECK_PASSBYVAL already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_BUFCHECK_PASSBYVAL) */

#if (defined SOMEIPXF_S_SERVER_XF_BUFCHECK_GEN) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_BUFCHECK_GEN already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_BUFCHECK_GEN) */

#if (defined SOMEIPXF_S_SERVER_XF_BUFCHECK) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_BUFCHECK already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_BUFCHECK) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_BUFFCHECK) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_BUFFCHECK already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_BUFFCHECK) */

#if (defined SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_1) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_1 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_1) */

#if (defined SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_2) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_2 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_2) */

#if (defined SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_1_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_1_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_1_END) */

#if (defined SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_2_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_2_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_2_END) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_ARG_NOAUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_ARG_NOAUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_ARG_NOAUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_NOARG_NOAUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_NOARG_NOAUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_NOARG_NOAUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_ARG_AUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_ARG_AUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_ARG_AUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_NOARG_AUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_NOARG_AUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_NOARG_AUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_BRACKET_NOAUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_BRACKET_NOAUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_BRACKET_NOAUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_BRACKET_AUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_BRACKET_AUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_BRACKET_AUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_IF_NORETVAL_NOAUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_IF_NORETVAL_NOAUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_IF_NORETVAL_NOAUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_IF_RETVAL_NOAUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_IF_RETVAL_NOAUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_IF_RETVAL_NOAUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_IF_RETVAL_AUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_IF_RETVAL_AUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_IF_RETVAL_AUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_RETVAL) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_RETVAL already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_RETVAL) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_NORETVAL) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_NORETVAL already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_NORETVAL) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_2_ARG) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_2_ARG already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_2_ARG) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_2_NOARG) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_2_NOARG already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_2_NOARG) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2) */

#if (defined SOMEIPXF_S_SERVER_XF_CALL_GEN) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_CALL_GEN already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_CALL_GEN) */

#if (defined SOMEIPXF_S_SERVER_XF_CALL) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_XF_CALL already defined
#endif /* if (defined SOMEIPXF_S_SERVER_XF_CALL) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_CALLS) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_CALLS) already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_CALLS) */

#if (defined SOMEIPXF_S_SERVER_LOOP_XF_CALL_1) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_LOOP_XF_CALL_1 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_LOOP_XF_CALL_1) */

#if (defined SOMEIPXF_S_SERVER_LOOP_XF_CALL_2) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_LOOP_XF_CALL_2 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_LOOP_XF_CALL_2) */

#if (defined SOMEIPXF_S_SERVER_LOOP_XF_CALL_1_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_LOOP_XF_CALL_1_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_LOOP_XF_CALL_1_END) */

#if (defined SOMEIPXF_S_SERVER_LOOP_XF_CALL_2_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_LOOP_XF_CALL_2_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_LOOP_XF_CALL_2_END) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_NORETVAL_ARG_NOAUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_NORETVAL_ARG_NOAUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_NORETVAL_ARG_NOAUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_NORETVAL_NOARG_NOAUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_NORETVAL_NOARG_NOAUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_NORETVAL_NOARG_NOAUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_ARG_NOAUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_ARG_NOAUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_ARG_NOAUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_NOARG_AUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_NOARG_AUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_NOARG_AUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_NOARG_NOAUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_NOARG_NOAUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_NOARG_NOAUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_ARG_AUTONERR) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_ARG_AUTONERR already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_ARG_AUTONERR) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_XF) */
/* Macro guards */

/* Start-section */
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START_NORETVAL

#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START_RETVAL  ,\
    Std_ReturnType ReturnValue

/* Deviation MISRAC2012-1 <+6> */
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START(XfrmName, HasReturnValue)                       \
FUNC(uint8, SOMEIPXF_CODE) XfrmName                                                              \
(                                                                                                \
    P2CONST(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle,      \
    P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,                                          \
    P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BufferLength SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START_ ## HasReturnValue
/* End of Start-section */


/* Create Parameter List for function call */
/* Deviation MISRAC2012-1 <+3> */
#define SOMEIPXF_S_SERVER_XF_PARAM_GEN(DataType, Nr, PassMethod, config)\
,\
    SOMEIPXF_ARG_ ## PassMethod(DataType) Data_ ## Nr

/* Create Parameter List for function call */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_SERVER_XF_PARAM(ARG_ID)    \
SOMEIPXF_S_SERVER_XF_PARAM_GEN SOMEIPXF_SERVER_ ## ARG_ID

#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_PARAMLIST(SEQ) SOMEIPXF_CAT(SOMEIPXF_S_SERVER_XF_LOOP_PARAM_1 SEQ, _END)
#define SOMEIPXF_S_SERVER_XF_LOOP_PARAM_1(x) SOMEIPXF_S_SERVER_XF_PARAM(x) SOMEIPXF_S_SERVER_XF_LOOP_PARAM_2
#define SOMEIPXF_S_SERVER_XF_LOOP_PARAM_2(x) SOMEIPXF_S_SERVER_XF_PARAM(x) SOMEIPXF_S_SERVER_XF_LOOP_PARAM_1
#define SOMEIPXF_S_SERVER_XF_LOOP_PARAM_1_END
#define SOMEIPXF_S_SERVER_XF_LOOP_PARAM_2_END
/* End of Create Parameter List for function call */


/* Mid-section 1 */
/* No parameter */
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_NOARG_NOAUTONERR
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_NORETVAL_NOARG_NOAUTONERR
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_NOARG_AUTONERR    \
      ||                                                                  \
      (((ReturnValue < SOMEIPXF_ERROR_REACTION_THRESHOLD)

/* No autonomous error reaction */
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_ARG_NOAUTONERR    \
  ||                                                                      \

/* No autonomous error reaction */
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_NORETVAL_ARG_NOAUTONERR  \
  ||                                                                      \

/* With autonomous error reaction */
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_RETVAL_ARG_AUTONERR      \
      ||                                                                  \
      ((ReturnValue < SOMEIPXF_ERROR_REACTION_THRESHOLD) && (
/* Deviation MISRAC2012-1 <+7> */

#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1( HasReturnValue, HasArguments, AutonErrReact )    \
)                                                                                                  \
{                                                                                                  \
    /* init. with E_OK to avoid compiler warning "condition always true" in else branch below */   \
    uint8 RetVal = E_OK;                                                                           \
                                                                                                   \
    if((Buffer == NULL_PTR) || (BufferLength == NULL_PTR) || (TransactionHandle == NULL_PTR) SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1_ ## HasReturnValue ## _ ## HasArguments ## _ ## AutonErrReact    \
                                                                                                   \

/* End of Mid-section 1 */


/* Create Nullptr checks */
/* Nullptr check needed */
/* Deviation MISRAC2012-1 <+3> */
#define SOMEIPXF_S_SERVER_XF_BUFCHECK_PASSBYREF(Nr) \
                                                    \
       (Data_ ## Nr == NULL_PTR)  ||                \
                                                    \

/* No Nullptr check needed */
#define SOMEIPXF_S_SERVER_XF_BUFCHECK_PASSBYVAL(Nr)

/* Insert Nullptr check if needed with helper macros above */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_SERVER_XF_BUFCHECK_GEN(DataType, Nr, PassMethod, config)   \
SOMEIPXF_S_SERVER_XF_BUFCHECK_ ## PassMethod(Nr)

/* Create NullPointer Check List */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_SERVER_XF_BUFCHECK(ARG_ID)                   \
SOMEIPXF_S_SERVER_XF_BUFCHECK_GEN SOMEIPXF_SERVER_ ## ARG_ID    \

#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_BUFFCHECK(SEQ) SOMEIPXF_CAT(SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_1 SEQ, _END)
#define SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_1(x) SOMEIPXF_S_SERVER_XF_BUFCHECK(x) SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_2
#define SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_2(x) SOMEIPXF_S_SERVER_XF_BUFCHECK(x) SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_1
#define SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_1_END
#define SOMEIPXF_S_SERVER_XF_LOOP_BUFCHECK_2_END
/* End of Create Nullptr checks */


/* MidSection 2 */
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_ARG_NOAUTONERR (0U)
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_NOARG_NOAUTONERR
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_ARG_AUTONERR (0U)
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_NOARG_AUTONERR

#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_BRACKET_NOAUTONERR
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_BRACKET_AUTONERR ) )

#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_IF_NORETVAL_NOAUTONERR
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_IF_RETVAL_NOAUTONERR
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_IF_RETVAL_AUTONERR   \
      if(ReturnValue < SOMEIPXF_ERROR_REACTION_THRESHOLD)             \
        {

/* Retval or no Retval*/
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_RETVAL ReturnValue
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_NORETVAL E_OK

/* With Arguments */
/* Deviation MISRAC2012-1 <+14> */
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_2_ARG(InterfaceVersion, HasReturnValue, HasArguments, AutonErrReact )  \
SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_ ## HasArguments ## _ ## AutonErrReact  ) SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_BRACKET ## _ ## AutonErrReact    \
    {                                                                                           \
        /* defensive programming mechanism (no Det interface) */                                \
        RetVal = E_SER_GENERIC_ERROR;                                                           \
    }                                                                                           \
    else                                                                                        \
    {                                                                                           \
        /* header of client/server communication */                                             \
        SomeIpXf_Response_SetHeader(TransactionHandle, InterfaceVersion, SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_ ## HasReturnValue , Buffer); \
                                                                                                \
        *BufferLength = SOMEIPXF_HEADER_LENGTH;                                                 \
                                                                                                \
        SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_IF_ ## HasReturnValue ## _ ## AutonErrReact

/* Without Arguments */
/* Deviation MISRAC2012-1 <+10> */
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_2_NOARG(InterfaceVersion, HasReturnValue, HasArguments, AutonErrReact ) \
SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_ ## HasArguments ## _ ## AutonErrReact ) SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_BRACKET ## _ ## AutonErrReact                  \
            {                                                                               \
                /* defensive programming mechanism (no Det interface) */                    \
                RetVal = E_SER_GENERIC_ERROR;                                               \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                /* header of client/server communication */                                 \
                SomeIpXf_Response_SetHeader(TransactionHandle, InterfaceVersion, SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_TOP_ ## HasReturnValue , Buffer); \
                                                                                            \
                *BufferLength = SOMEIPXF_HEADER_LENGTH;                                     \

/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2(InterfaceVersion, HasReturnValue, HasArguments, AutonErrReact )    \
SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2_2_ ## HasArguments(InterfaceVersion, HasReturnValue, HasArguments, AutonErrReact )
/* End of Mid-section 2 */


/* Create function calls */

/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+7> */
#define SOMEIPXF_S_SERVER_XF_CALL_GEN(DataType, Nr, PassMethod, config)                           \
          if(RetVal == E_OK)                                                                      \
          {                                                                                       \
                RetVal = SomeIpXf_S_GenericTransformer(                                           \
                    &Buffer[*BufferLength],                                                       \
                    BufferLength,                                                                 \
                    (P2CONST(void, AUTOMATIC, SOMEIPXF_APPL_DATA)) SOMEIPXF_PARAM_ ## PassMethod(Data_ ## Nr),   \
                    &(config)[0]                                                                  \
                );                                                                                \
          }

/* List of Transformer calls */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_SERVER_XF_CALL(ARG_ID)                 \
SOMEIPXF_S_SERVER_XF_CALL_GEN SOMEIPXF_SERVER_ ## ARG_ID

#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_CALLS(SEQ) SOMEIPXF_CAT(SOMEIPXF_S_SERVER_LOOP_XF_CALL_1 SEQ, _END)
#define SOMEIPXF_S_SERVER_LOOP_XF_CALL_1(x) SOMEIPXF_S_SERVER_XF_CALL(x) SOMEIPXF_S_SERVER_LOOP_XF_CALL_2
#define SOMEIPXF_S_SERVER_LOOP_XF_CALL_2(x) SOMEIPXF_S_SERVER_XF_CALL(x) SOMEIPXF_S_SERVER_LOOP_XF_CALL_1
#define SOMEIPXF_S_SERVER_LOOP_XF_CALL_1_END
#define SOMEIPXF_S_SERVER_LOOP_XF_CALL_2_END
/* End of Create function calls */


/* End-section */
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_NORETVAL_ARG_NOAUTONERR
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_NORETVAL_NOARG_NOAUTONERR
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_ARG_NOAUTONERR
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_NOARG_AUTONERR
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_NOARG_NOAUTONERR

#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_RETVAL_ARG_AUTONERR   \
        }                                                            \
        else                                                         \
        {                                                            \
            RetVal = E_OK;                                           \
        }

/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END( HasReturnValue, HasArguments, HasAutonomousErrorReact)      \
    SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END_ ## HasReturnValue ## _ ## HasArguments ## _ ## HasAutonomousErrorReact  \
    }                                                                                          \
                                                                                               \
    return RetVal;                                                                             \
}
/* End of End-section */


/* Macro to define safe transformer API function for Server communication
 * Parameters:
 * XfrmName   Name of the inverse transformer
 * InterfaceVersion   The interface version the inverse transformer shall use
 * HasReturnValue   Function has return value (RETVAL, NORETVAL)
 * HasArguments   Function has one ore more Arguments, e.g. Data_1 (ARG, NOARG)
 * HasAutonomousErrorReact   Function has autonomous Error reaction enabled/disabled
 *                           (AUTONERR, NOAUTONERR)
 */
/* Deviation MISRAC2012-1 <+6> */
#define SOMEIPXF_S_SERVER_DEF_FUNC_XF(XfrmName, InterfaceVersion, HasReturnValue, HasArguments, HasAutonomousErrorReact)                                 \
SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START(XfrmName, HasReturnValue)SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_PARAMLIST( SOMEIPXF_SERVER_SEQ_XF_ ## XfrmName )    \
    SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_1( HasReturnValue, HasArguments, HasAutonomousErrorReact )                                                    \
     SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_BUFFCHECK( SOMEIPXF_SERVER_SEQ_XF_ ## XfrmName ) SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_MID_2( InterfaceVersion, HasReturnValue, HasArguments, HasAutonomousErrorReact )     \
                                                                                            \
          SOMEIPXF_S_SERVER_DEF_FUNC_XF_CALLS( SOMEIPXF_SERVER_SEQ_XF_ ## XfrmName )        \
        SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_END( HasReturnValue, HasArguments, HasAutonomousErrorReact)

/* ----- Function declaration ----- */
#if (defined SOMEIPXF_S_SERVER_DECL_FUNC_XF_SECT_START) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DECL_FUNC_XF_SECT_START already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DECL_FUNC_XF_SECT_START) */

#if (defined SOMEIPXF_S_SERVER_DECL_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DECL_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DECL_FUNC_XF) */

/* Start-section */
/* Deviation MISRAC2012-1 <+6> */
#define SOMEIPXF_S_SERVER_DECL_FUNC_XF_SECT_START(XfrmName, HasReturnValue)                  \
extern FUNC(uint8, SOMEIPXF_CODE) XfrmName                                                   \
(                                                                                            \
    P2CONST(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle,  \
    P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,                                      \
    P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BufferLength SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_START_ ## HasReturnValue

/* Assemble parts for declaration */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_SERVER_DECL_FUNC_XF(XfrmName, InterfaceVersion, HasReturnValue, HasArguments, HasAutonomousErrorReact)                                 \
SOMEIPXF_S_SERVER_DECL_FUNC_XF_SECT_START(XfrmName, HasReturnValue)SOMEIPXF_S_SERVER_DEF_FUNC_XF_SECT_PARAMLIST( SOMEIPXF_SERVER_SEQ_XF_ ## XfrmName )    \
)
/* ----- End of Function declaration ----- */
/* --------------------- End of Server MACROS --------------------------------- */


/* --------------------- INV-Server MACROS --------------------------------- */
/* Macro guards */
#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_START) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_START already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_START) */

#if (defined SOMEIPXF_SERFER_INVXF_ARG_PASSBYVAL) /* to prevent double declaration */
#error SOMEIPXF_SERFER_INVXF_ARG_PASSBYVAL already defined
#endif /* if (defined SOMEIPXF_SERFER_INVXF_ARG_PASSBYVAL) */

#if (defined SOMEIPXF_S_SERVER_INVXF_PARAM_GEN) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_PARAM_GEN already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_PARAM_GEN) */

#if (defined SOMEIPXF_S_SERVER_INVXF_PARAM) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_PARAM already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_PARAM) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_PARAMLIST) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_PARAMLIST already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_PARAMLIST) */

#if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_1) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_1 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_1) */

#if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_2) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_2 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_2) */

#if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_1_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_1_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_1_END) */

#if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_2_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_2_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_2_END) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_MID_1) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_MID_1 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_MID_1) */

#if (defined SOMEIPXF_S_SERVER_INVXF_BUFCHECK_GEN) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_BUFCHECK_GEN already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_BUFCHECK_GEN) */

#if (defined SOMEIPXF_S_SERVER_INVXF_BUFCHECK) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_BUFCHECK already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_BUFCHECK) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_BUFFCHECK) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_BUFFCHECK already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_BUFFCHECK) */

#if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_1) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_1 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_1) */

#if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_2) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_2 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_2) */

#if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_1_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_1_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_1_END) */

#if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_2_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_2_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_2_END) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_MID_2) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_MID_2 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_MID_2) */

#if (defined SOMEIPXF_S_SERVER_INVXF_CALL_GEN) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_CALL_GEN already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_CALL_GEN) */

#if (defined SOMEIPXF_S_SERVER_INVXF_CALL) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_CALL already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_CALL) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG) */

#if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_1) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_1 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_1) */

#if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_2) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_2 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_2) */

#if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_1_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_1_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_1_END) */

#if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_2_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_2_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_2_END) */

#if (defined SOMEIPXF_S_SERVER_INVXF_CALL_GEN_TLV) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_INVXF_CALL_GEN_TLV already defined
#endif /* if (defined SOMEIPXF_S_SERVER_INVXF_CALL_GEN_TLV) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_CALLS_TLV_ARG) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_CALLS_TLV_ARG already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_CALLS_TLV_ARG) */

#if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_11) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_11 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_11) */

#if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_22) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_22 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_22) */

#if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_11_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_11_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_11_END) */

#if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_22_END) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_22_END already defined
#endif /* if (defined SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_22_END) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END1_TLV) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END1_TLV already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END1_TLV) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END1_NON_TLV) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END1_NON_TLV already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END1_NON_TLV) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END2) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END2 already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END2) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_FIND_DUPLICATEID_TLV) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_FIND_DUPLICATEID_TLV already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_FIND_DUPLICATEID_TLV) */

#if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_FIND_DUPLICATEID_NON_TLV) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_FIND_DUPLICATEID_NON_TLV already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_FIND_DUPLICATEID_NON_TLV) */
/* Macro guards */


/* Start-section */
#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_START(XfrmName)                               \
FUNC(uint8, SOMEIPXF_CODE) XfrmName                                                         \
(                                                                                           \
    P2VAR(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle,   \
    P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,                                   \
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength


/* Create Parameter List for function call */
/* Inverse server call is always P2VAR: */
#define SOMEIPXF_SERFER_INVXF_ARG_PASSBYVAL(DataType) P2VAR(DataType, AUTOMATIC, SOMEIPXF_APPL_DATA)

/* Deviation MISRAC2012-1 <+3> */
#define SOMEIPXF_S_SERVER_INVXF_PARAM_GEN(DataType, Nr, config) \
,                                                               \
    SOMEIPXF_SERFER_INVXF_ARG_PASSBYVAL(DataType) Data_ ## Nr

/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_SERVER_INVXF_PARAM(ARG_ID)                \
SOMEIPXF_S_SERVER_INVXF_PARAM_GEN SOMEIPXF_SERVER_ ## ARG_ID

#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_PARAMLIST(SEQ) SOMEIPXF_CAT(SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_1 SEQ, _END)
#define SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_1(x) SOMEIPXF_S_SERVER_INVXF_PARAM(x) SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_2
#define SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_2(x) SOMEIPXF_S_SERVER_INVXF_PARAM(x) SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_1
#define SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_1_END
#define SOMEIPXF_S_SERVER_INVXF_LOOP_PARAM_2_END
/* End of Create Parameter List for function call */


/* Static Mid-section 1 */
#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_MID_1         \
)                                                           \
{                                                           \
    uint8 RetVal;                                           \
                                                            \
    /* check for executeDespiteDataUnavailability */        \
    if((Buffer == NULL_PTR) && (BufferLength == 0U))        \
    {                                                       \
        RetVal = E_NO_DATA;                                 \
    }                                                       \
    else                                                    \
    {                                                       \
       if((Buffer == NULL_PTR) || (TransactionHandle == NULL_PTR)


/* Create Nullptr check */
/* Deviation MISRAC2012-1 <+3> */
#define SOMEIPXF_S_SERVER_INVXF_BUFCHECK_GEN(DataType, Nr, config)  \
||                                                                  \
          (Data_ ## Nr == NULL_PTR)

/* Create NullPointer Check List for function call */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_SERVER_INVXF_BUFCHECK(ARG_ID)                \
SOMEIPXF_S_SERVER_INVXF_BUFCHECK_GEN SOMEIPXF_SERVER_ ## ARG_ID

#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_BUFFCHECK(SEQ) SOMEIPXF_CAT(SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_1 SEQ, _END)
#define SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_1(x) SOMEIPXF_S_SERVER_INVXF_BUFCHECK(x) SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_2
#define SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_2(x) SOMEIPXF_S_SERVER_INVXF_BUFCHECK(x) SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_1
#define SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_1_END
#define SOMEIPXF_S_SERVER_INVXF_LOOP_BUFCHECK_2_END
/* End of Create Nullptr check */


/* Mid-section 2 */
#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_MID_2(InterfaceVersion)                          \
)                                                                                              \
        {                                                                                      \
            /* defensive programming mechanism (no Det interface) */                           \
            RetVal = E_SER_GENERIC_ERROR;                                                      \
        }                                                                                      \
        else if (BufferLength < SOMEIPXF_HEADER_LENGTH)                                        \
        {                                                                                      \
            /* defensive programming mechanism (no Det interface) */                           \
            RetVal = E_SER_GENERIC_ERROR;                                                      \
        }                                                                                      \
        else                                                                                   \
        {                                                                                      \
            SOMEIPXF_XFRMBUFFERLENGTH_TYPE BytesCopiedFromBuffer = SOMEIPXF_HEADER_LENGTH;     \
                                                                                               \
            /* header of client/server communication */                                        \
            RetVal = SomeIpXf_Request_GetHeader(TransactionHandle, Buffer, InterfaceVersion);


/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+10> */
#define SOMEIPXF_S_SERVER_INVXF_CALL_GEN(DataType, Nr, config)                     \
                                                                                   \
                    if(RetVal == E_OK)                                             \
            {                                                                      \
                if(BufferLength > BytesCopiedFromBuffer)                           \
                {                                                                  \
                    RetVal = SomeIpXf_S_Inv_GenericTransformer(                    \
                        &Buffer[BytesCopiedFromBuffer],                            \
                        BufferLength,                                              \
                        (P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA))Data_ ## Nr,   \
                        &BytesCopiedFromBuffer,                                    \
                        &config[0]                                                 \
                    );                                                             \
                }                                                                  \
                else                                                               \
                {                                                                  \
                    RetVal = E_SER_MALFORMED_MESSAGE;                              \
                }                                                                  \
            }                                                                      \
                                                                                   \
                                                                                   \

#if (SOMEIPXF_TLV_ARGUMENT_REORDERING_ENABLE == STD_ON)
/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+10> */
#define SOMEIPXF_S_SERVER_INVXF_CALL_GEN_TLV(DataType, Nr, config)                      \
                                                                                        \
        if(RetVal == E_OK)                                                              \
        {                                                                               \
            /* Start again */                                                           \
            BytesCopiedFromBuffer = 8U;                                                 \
            {                                                                           \
                RetVal = SomeIpXf_S_Inv_GenericTransformer(                             \
                    &Buffer[BytesCopiedFromBuffer],                                     \
                    BufferLength,                                                       \
                    (P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA))Data_ ## Nr,            \
                    &BytesCopiedFromBuffer,                                             \
                    &config[0]                                                          \
                );                                                                      \
            }                                                                           \
            if( (RetVal == E_OK) && (BufferLength < BytesCopiedFromBuffer))             \
            {                                                                           \
                RetVal = E_SER_MALFORMED_MESSAGE;                                       \
            }                                                                           \
        }                                                                               \
                                                                                        \
                                                                                        \

#else

/* Create transformer function calls */
/* Deviation MISRAC2012-1 <+10> */
#define SOMEIPXF_S_SERVER_INVXF_CALL_GEN_TLV(DataType, Nr, config)               \
                                                                                 \
          if(RetVal == E_OK)                                                     \
          {                                                                      \
              if(BufferLength > BytesCopiedFromBuffer)                           \
              {                                                                  \
                  RetVal = SomeIpXf_S_Inv_GenericTransformer(                    \
                      &Buffer[BytesCopiedFromBuffer],                            \
                      BufferLength,                                              \
                      (P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA))Data_ ## Nr,   \
                      &BytesCopiedFromBuffer,                                    \
                      &config[0]                                                 \
                  );                                                             \
              }                                                                  \
              else                                                               \
              {                                                                  \
                  RetVal = E_SER_MALFORMED_MESSAGE;                              \
              }                                                                  \
          }                                                                      \
                                                                                 \

#endif /* (SOMEIPXF_TLV_ARGUMENT_REORDERING_ENABLE == STD_ON) */

#define SOMEIPXF_SERVER_HELP_END

/* Get the config of first argument of i-signal */
#define SOMEIPXF_S_SERVER_INVXF_GET_CONFIGID_HELPER(DataType, Nr, config) config

#define SOMEIPXF_S_SERVER_INVXF_GET_CONFIGID(XfrmName)                  \
SOMEIPXF_CAT(SOMEIPXF_S_SERVER_INVXF_GET_CONFIGID_HELPER SOMEIPXF_SERVER_INVXF_ ## XfrmName ## _ARG_1 SOMEIPXF_SERVER_HELP, _END)

#if (SOMEIPXF_TLV_ARGUMENT_UNIQUE_DATAID_CHECK_ENABLE == STD_ON)
/* Deviation MISRAC2012-1 <+11> */
#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_FIND_DUPLICATEID_TLV(XfrmName)                        \
if(RetVal == E_OK)                                                                             \
{                                                                                              \
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLVArgIDListSize = SOMEIPXF_TLV_ARG_IDS_COUNT_ ## XfrmName;   \
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE TLVArgID[SOMEIPXF_TLV_ARG_IDS_COUNT_ ## XfrmName] =           \
            {SOMEIPXF_TLV_ARG_IDS_ ## XfrmName};                                               \
             BytesCopiedFromBuffer = 8U;                                                       \
           /* Check multiple known data id's */                                                \
            RetVal = SomeIpXf_CheckMultipleKnownDataIDs(                                       \
                &Buffer[BytesCopiedFromBuffer],                                                \
                BufferLength,                                                                  \
                TLVArgIDListSize,                                                              \
                &TLVArgID[0U], /* TLV Arg ID list of OUT/IN-OUT Parameters */                  \
      &SOMEIPXF_S_SERVER_INVXF_GET_CONFIGID(XfrmName)[0U]);                                    \
}                                                                                              \

#else

#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_FIND_DUPLICATEID_TLV(XfrmName) /* Optimization enabled. Duplicate dataid's shall not be found*/

#endif /* (SOMEIPXF_TLV_ARGUMENT_UNIQUE_DATAID_CHECK_ENABLE == STD_ON) */

/* macros expand to empty */
#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_FIND_DUPLICATEID_NON_TLV(XfrmName) /* No TLV DataId's */\

/* static End section 1 */
#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END1_NON_TLV                   \
            if( (RetVal == E_OK) && (BufferLength < BytesCopiedFromBuffer))  \
            {                                                                \
                RetVal = E_SER_MALFORMED_MESSAGE;                            \
            }                                                                \
        }                                                                    \
    }

#if(SOMEIPXF_TLV_ARGUMENT_REORDERING_ENABLE == STD_ON)
/* static End section 1 */
#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END1_TLV                       \
        }                                                                    \
    }
#else
/* static End section 1 */
#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END1_TLV                           \
            if( (RetVal == E_OK) && (BufferLength < BytesCopiedFromBuffer))  \
            {                                                                \
                RetVal = E_SER_MALFORMED_MESSAGE;                            \
            }                                                                \
        }                                                                    \
    }
#endif /* (SOMEIPXF_TLV_ARGUMENT_REORDERING_ENABLE == STD_ON) */

/* List of Transformer calls */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_SERVER_INVXF_CALL(ARG_ID)                 \
SOMEIPXF_S_SERVER_INVXF_CALL_GEN SOMEIPXF_SERVER_ ## ARG_ID

/* With Arguments */
#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_ARG(SEQ) SOMEIPXF_CAT(SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_1 SEQ, _END)
#define SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_1(x) SOMEIPXF_S_SERVER_INVXF_CALL(x) SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_2
#define SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_2(x) SOMEIPXF_S_SERVER_INVXF_CALL(x) SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_1
#define SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_1_END
#define SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_2_END

/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_SERVER_INVXF_CALL_TLV(ARG_ID)                  \
SOMEIPXF_S_SERVER_INVXF_CALL_GEN_TLV SOMEIPXF_SERVER_ ## ARG_ID

#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_CALLS_TLV_ARG(SEQ) SOMEIPXF_CAT(SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_11 SEQ, _END)
#define SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_11(x) SOMEIPXF_S_SERVER_INVXF_CALL_TLV(x) SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_22
#define SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_22(x) SOMEIPXF_S_SERVER_INVXF_CALL_TLV(x) SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_11
#define SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_11_END
#define SOMEIPXF_S_SERVER_LOOP_INVXF_CALL_22_END

/* No Arguments */
#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_CALLS_NON_TLV_NOARG(UNUSED) \
    SOMEIPXF_PARAM_UNUSED(BytesCopiedFromBuffer);
/* End of Create transformer function calls */

/* static End section 2 */
#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END2                              \
    return RetVal;                                                              \
}

/* Macro to define safe inverse transformer API function for Server communication
 * Parameters:
 * XfrmName   Name of the inverse transformer
 * InterfaceVersion   The interface version the inverse transformer shall use
 * HasArguments   Function has one ore more Arguments, e.g. Data_1 (ARG, NOARG)
 */
/* Deviation MISRAC2012-1 <+8> */
#define SOMEIPXF_S_SERVER_DEF_FUNC_INVXF(XfrmName, InterfaceVersion, HasArguments, IsTlv)                                                           \
SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_START(XfrmName)SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_PARAMLIST( SOMEIPXF_SERVER_SEQ_INVXF_ ## XfrmName )                      \
SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_MID_1 SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_BUFFCHECK( SOMEIPXF_SERVER_SEQ_INVXF_ ## XfrmName ) SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_MID_2( InterfaceVersion )  \
SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_CALLS_ ## IsTlv ##_ ## HasArguments( SOMEIPXF_SERVER_SEQ_INVXF_ ## XfrmName )                                                    \
SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_FIND_DUPLICATEID_ ## IsTlv(XfrmName)                   \
SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END1_ ## IsTlv                                    \
SOMEIPXF_MESSAGE_ERROR_CALLOUT(XfrmName)                                                \
SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_END2

/* ----- INV Function declaration ----- */
#if (defined SOMEIPXF_S_SERVER_DECL_FUNC_INVXF_SECT_START) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DECL_FUNC_INVXF_SECT_START already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DECL_FUNC_INVXF_SECT_START) */

#if (defined SOMEIPXF_S_SERVER_DECL_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_S_SERVER_DECL_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_S_SERVER_DECL_FUNC_INVXF) */

/* Decl-Start-section */
#define SOMEIPXF_S_SERVER_DECL_FUNC_INVXF_SECT_START(XfrmName)                              \
extern FUNC(uint8, SOMEIPXF_CODE) XfrmName                                                  \
(                                                                                           \
    P2VAR(Rte_Cs_TransactionHandleType, AUTOMATIC, SOMEIPXF_APPL_DATA) TransactionHandle,   \
    P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,                                   \
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength

/* Assemble parts for declaration */
/* Deviation MISRAC2012-1 <+2> */
#define SOMEIPXF_S_SERVER_DECL_FUNC_INVXF(XfrmName, InterfaceVersion, HasArguments)                \
SOMEIPXF_S_SERVER_DECL_FUNC_INVXF_SECT_START(XfrmName) SOMEIPXF_S_SERVER_DEF_FUNC_INVXF_SECT_PARAMLIST( SOMEIPXF_SERVER_SEQ_INVXF_ ## XfrmName )  \
)SOMEIPXF_S_DECL_FUNC_INVXF_MESSAGE_ERROR_CALLOUT(XfrmName)

/* ----- End of INV Function declaration ----- */
/* --------------------- End of INV-Server MACROS --------------------------------- */



/* ---------------------- External Trigger MACROS -------------------------------------- */
/* Macro guards */
#if (defined SOMEIPXF_S_ET_DEF_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_S_ET_DEF_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_S_ET_DEF_FUNC_XF) */

#if (defined SOMEIPXF_S_ET_DECL_FUNC_XF) /* to prevent double declaration */
#error SOMEIPXF_S_ET_DECL_FUNC_XF already defined
#endif /* if (defined SOMEIPXF_S_ET_DECL_FUNC_XF) */

#if (defined SOMEIPXF_ET_SETHEADER) /* to prevent double declaration */
#error SOMEIPXF_ET_SETHEADER already defined
#endif /* if (defined SOMEIPXF_ET_SETHEADER) */

#if (defined SOMEIPXF_ET_SESSIONHANDLINGVAR) /* to prevent double declaration */
#error SOMEIPXF_ET_SESSIONHANDLINGVAR already defined
#endif /* if (defined SOMEIPXF_ET_SESSIONHANDLINGVAR) */
/* Macro guards */

/* ET macro */
/* header of external trigger communication is identical to that of sender/receiver */
/* Deviation MISRAC2012-1 */
#define SOMEIPXF_ET_SETHEADER(SessionHandlingStatus) SOMEIPXF_SR_SETHEADER_ ## SessionHandlingStatus

/* Deviation MISRAC2012-1 */
#define SOMEIPXF_ET_SESSIONHANDLINGVAR(XfrmName) XfrmName ## _SessionHandlingCntVal

#define SOMEIPXF_S_ET_DEF_FUNC_XF(XfrmName, InterfaceVersion, SessionHandlingStatus)            \
FUNC(uint8, SOMEIPXF_CODE) XfrmName                                                             \
(                                                                                               \
    P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,                                         \
    P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BufferLength           \
)                                                                                               \
{                                                                                               \
    uint8 RetVal = E_SER_GENERIC_ERROR;                                                         \
                                                                                                \
    if ((Buffer != NULL_PTR) &&                                                                 \
        (BufferLength != NULL_PTR)                                                              \
       )                                                                                        \
    {                                                                                           \
       SOMEIPXF_ET_SETHEADER(SessionHandlingStatus)(Buffer, InterfaceVersion,                   \
       SOMEIPXF_MESSAGE_TYPE_REQUEST_NO_RETURN, SOMEIPXF_ET_SESSIONHANDLINGVAR(XfrmName));      \
      *BufferLength = SOMEIPXF_HEADER_LENGTH;                                                   \
       RetVal = E_OK;                                                                           \
    }                                                                                           \
    return RetVal;                                                                              \
}

/* Declaration */
#define SOMEIPXF_S_ET_DECL_FUNC_XF(XfrmName)                                            \
extern FUNC(uint8, SOMEIPXF_CODE) XfrmName                                              \
(                                                                                       \
    P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,                                 \
    P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BufferLength   \
)

/* ---------------------- End of External Trigger MACROS ------------------------- */


/* ---------------------- External Trigger INV MACROS ---------------------------- */
/* Macro guards */
#if (defined SOMEIPXF_S_ET_DEF_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_S_ET_DEF_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_S_ET_DEF_FUNC_INVXF) */

#if (defined SOMEIPXF_S_ET_DECL_FUNC_INVXF) /* to prevent double declaration */
#error SOMEIPXF_S_ET_DECL_FUNC_INVXF already defined
#endif /* if (defined SOMEIPXF_S_ET_DECL_FUNC_INVXF) */
/* Macro guards */


/* INV ET macro */
#define SOMEIPXF_S_ET_DEF_FUNC_INVXF(XfrmName, InterfaceVersion)                \
FUNC(uint8, SOMEIPXF_CODE) XfrmName                                             \
(                                                                               \
    P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,                       \
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength                                 \
)                                                                               \
{                                                                               \
    uint8 RetVal;                                                               \
                                                                                \
    RetVal = SomeIpXf_Inv_ET_Generic(Buffer, BufferLength, InterfaceVersion);   \
                                                                                \
    SOMEIPXF_MESSAGE_ERROR_CALLOUT(XfrmName)                                    \
                                                                                \
    return RetVal;                                                              \
}


/* INV Declaration */
#define SOMEIPXF_S_ET_DECL_FUNC_INVXF(XfrmName)              \
extern FUNC(uint8, SOMEIPXF_CODE) XfrmName                   \
(                                                            \
    P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,    \
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength              \
)SOMEIPXF_S_DECL_FUNC_INVXF_MESSAGE_ERROR_CALLOUT(XfrmName)

/* ----------------End of External Trigger INV MACROS ---------------------------- */



/*==================[Error codes]===========================================*/

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define SOMEIPXF_START_SEC_CODE
#include <SomeIpXf_MemMap.h>

extern FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_S_GenericTransformer
(
   P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BufferLengthPtr,
   P2CONST(void, AUTOMATIC, SOMEIPXF_APPL_DATA) DataPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr
);

extern FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_S_Inv_GenericTransformer
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
   P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA) DataPtr,
   P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BytesCopiedPtr,
   P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr
);

/* Check versions in header received by deserializer */
extern FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_SR_GetHeader
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
   uint8 InterfaceVersion
);

#if (SOMEIPXF_ET_AVAILABLE == STD_ON)
extern FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Inv_ET_Generic
(
    P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) Buffer,
    SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
    uint8 InterfaceVersion
);
#endif /* (SOMEIPXF_ET_AVAILABLE == STD_ON) */

#define SOMEIPXF_STOP_SEC_CODE
#include <SomeIpXf_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef SOMEIPXF_S_INT_STC_H */
/*==================[end of file]===========================================*/
