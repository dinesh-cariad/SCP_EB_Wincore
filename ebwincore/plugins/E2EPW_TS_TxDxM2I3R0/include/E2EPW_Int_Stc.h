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
 *  MISRA-C:2012 Deviation List
 *
 *   MISRAC2012-1) Deviated Rule: 20.7 (Required)
 *   Expression resulting from the expansion of macro parameters shall be enclosed in parentheses.
 *
 *   Reason:
 *   Macros are used for type independent casts. Such a parameter cannot be
 *   enclosed in parentheses as this would lead to a syntax error.
 */

#if (!defined E2EPW_INT_STC_H)
#define E2EPW_INT_STC_H

/*==================[inclusions]=============================================*/

#include <E2EPW_Int_Cfg.h>          /* E2EPW PreCompile parameters */
#include <Std_Types.h>              /* AUTOSAR Std types */

/*==[Macros]==================================================================*/

#if (defined E2EPW_RANGECHK_VALID) /* To prevent double declaration */
#error E2EPW_RANGECHK_VALID already defined
#endif /* if (defined E2EPW_RANGECHK_VALID) */

/** \brief Define E2EPW_RANGECHK_VALID with 0 for a successful range check result */
#define E2EPW_RANGECHK_VALID 0U

#if (defined E2EPW_RANGECHK_INVALID) /* To prevent double declaration */
#error E2EPW_RANGECHK_INVALID already defined
#endif /* if (defined E2EPW_RANGECHK_INVALID) */

/** \brief Define E2EPW_RANGECHK_INVALID with 255 for a successful range check result */
#define E2EPW_RANGECHK_INVALID 255U

#if ((E2EPW_ENABLE_PROFILE_P01 == STD_ON) ||  \
     (E2EPW_ENABLE_PROFILE_P02 == STD_ON) ||  \
     (E2EPW_ENABLE_PROFILE_PDAI1 == STD_ON) ||  \
     (E2EPW_ENABLE_PROFILE_PDAI2 == STD_ON))

#if (defined E2EPW_GenericWrite2) /* To prevent double declaration */
#error E2EPW_GenericWrite2 already defined
#endif /* if (defined E2EPW_GenericWrite2) */

/** \brief Define Function mapping from E2EPW_GenericWrite2 to E2EPW_GenericWrite
 * E2EPW_GenericWrite implements same function as required for E2EPW_GenericWrite2
 */
#define E2EPW_GenericWrite2 E2EPW_GenericWrite

#if (defined E2EPW_GenericRead2) /* To prevent double declaration */
#error E2EPW_GenericRead2 already defined
#endif /* if (defined E2EPW_GenericRead2) */

/** \brief Define Function mapping from E2EPW_GenericRead2 to E2EPW_GenericRead2
 * E2EPW_GenericWrite1 implements same function as required for E2EPW_GenericRead2
 */
#define E2EPW_GenericRead2 E2EPW_GenericWrite1

/*==[Types]===================================================================*/

/** \brief Struct prototype for the generic configuration data */
struct E2EPW_GenericConfigStruct;

/** \brief Generic function pointer for signal packing */
typedef FUNC(uint8, E2EPW_CODE) (*E2EPW_PackCopyDeFuncPtrType)
(
   CONSTP2CONST(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_SrcPtr,
   CONSTP2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DstPtr
);

/** \brief Generic function pointer for functions called inside a generic Read/Write function */
typedef FUNC(uint32, E2EPW_CODE) (*E2EPW_GenericInnerFuncPtrType)
(
   /* Deviation MISRAC2012-1 */
   P2CONST(struct E2EPW_GenericConfigStruct, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/** \brief Generic function pointer for functions called inside a generic Read/Write function */
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
typedef FUNC(Std_ReturnType, E2EPW_CODE) (*E2EPW_GenericRteReadWriteFuncPtrType)
(
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement,
   P2VAR(uint8, AUTOMATIC, E2EPW_VAR) SerData,
   uint16 SerDataLength
);
#else
typedef FUNC(Std_ReturnType, E2EPW_CODE) (*E2EPW_GenericRteReadWriteFuncPtrType)
(
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) genericDataElement
);
#endif

/** \brief Configuration data for generic wrapper routine */
typedef struct E2EPW_GenericConfigStruct{
   E2EPW_GenericRteReadWriteFuncPtrType RteReadWriteFunc;
   /**<  Generic Rte Read/Write function called inside a generic E2EPW Read/Write function */
   E2EPW_PackCopyDeFuncPtrType E2EPW_PackCopyDeFunc;
   /**<  The signal pack resp. Copy Data Element function associated to this configuration */
   E2EPW_GenericInnerFuncPtrType E2EPW_InnerFunc;
   /**<  Profile specific function called inside a generic Read or Write function */
   P2CONST(void, TYPEDEF, E2EPW_CONST) E2E_PXXConfig;
   /**<  Pointer to configuration data of transmitted Data of a specific E2E Profile */
   P2VAR(void, TYPEDEF, E2EPW_VAR) E2E_PXXState;
   /**<  Pointer to sender or receiver state of a specific E2E Profile */
   CONST(uint16, TYPEDEF) DE_Size;
   /**<  Size of the DataElement in bytes */
   CONST(uint16, TYPEDEF) DE_CRCoffs;
   /**<  Byte offset of the Crc signal within the DataElement */
   CONST(uint16, TYPEDEF) DE_CNToffs;
   /**<  Byte offset of the alive/sequence counter signal within the DataElement */
#if (E2EPW_ENABLE_PROFILE_P01 == STD_ON)
   CONST(uint16, TYPEDEF) DE_DIDNibbleOffs;
   /**<  Byte offset of the DataID Nibble signal within the DataElement in case Profile 01
    * is used with inclusion mode E2E_P01_DATAID_NIBBLE. (Otherwise, this parameter
    * is unused and equals the byte offset of the CRC signal)*/
#endif
} E2EPW_GenericConfigType;

/*==[Declaration of Constants with external linkage]==========================*/

/*==[Declaration of Variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/

#define E2EPW_START_SEC_CODE
#include <MemMap.h>

/**
 * \brief Generic Single Channel Read Protection Wrapper
 *
 * \param[in] config Pointer to configuration data
 * \param[in] data Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_GenericRead
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) data,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Single Channel Write Protection Wrapper
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] data Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 * \param[in] SerDataLength Data length in bytes of parameter \a SerDataDEData
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_GenericWrite
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) data,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
#if (E2EPW_ENABLE_TX_SIGNAL_GROUP_ARRAY_ACCESS == STD_ON)
   , uint16 SerDataLength
#endif
);

/**
 * \brief Generic Redundant Channel Write1 Protection Wrapper
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] data Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_GenericWrite1
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) data,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

#if (E2EPW_ENABLE_PROFILE_P01 == STD_ON)

/**
 * \brief Generic Inner Read Implementation
 *
 * Implementation is done for intra-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerRead_P01
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Read Implementation
 *
 * Implementation is done for inter-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerRead_P01
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write Implementation
 *
 * Implementation is done for inter-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 *         and result of internal runtime checks in Byte 2
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite_P01
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write Implementation
 *
 * Implementation is done for intra-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 *         and result of internal runtime checks in Byte 2
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite_P01
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write2 Implementation
 *
 * Implementation is done for inter-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite2_P01
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write2 Implementation
 *
 * Implementation is done for intra-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 *         and result of redundancy check in Byte 3
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite2_P01
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

#endif /* (E2EPW_ENABLE_PROFILE_P01 == STD_ON) */

#if (E2EPW_ENABLE_PROFILE_P02 == STD_ON)

/**
 * \brief Generic Inner Read Implementation
 *
 * Implementation is done for inter-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerRead_P02
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Read Implementation
 *
 * Implementation is done for intra-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerRead_P02
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write Implementation
 *
 * Implementation is done for inter-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 *         and result of internal runtime checks in Byte 2
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite_P02
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write Implementation
 *
 * Implementation is done for intra-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 *         and result of internal runtime checks in Byte 2
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite_P02
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write2 Implementation
 *
 * Implementation is done for inter-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite2_P02
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write2 Implementation
 *
 * Implementation is done for intra-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 *         and result of redundancy check in Byte 3
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite2_P02
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

#endif /* (E2EPW_ENABLE_PROFILE_P2 == STD_ON) */

#if (E2EPW_ENABLE_PROFILE_PDAI1 == STD_ON)

/**
 * \brief Generic Inner Read Implementation
 *
 * Implementation is done for inter-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerRead_PDAI1
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Read Implementation
 *
 * Implementation is done for intra-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerRead_PDAI1
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write Implementation
 *
 * Implementation is done for inter-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 *         and result of internal runtime checks in Byte 2
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite_PDAI1
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write Implementation
 *
 * Implementation is done for intra-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 *         and result of internal runtime checks in Byte 2
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite_PDAI1
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write2 Implementation
 *
 * Implementation is done for inter-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite2_PDAI1
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write2 Implementation
 *
 * Implementation is done for intra-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 *         and result of redundancy check in Byte 3
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite2_PDAI1
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

#endif /* (E2EPW_ENABLE_PROFILE_PDAI1 == STD_ON) */

#if (E2EPW_ENABLE_PROFILE_PDAI2 == STD_ON)

/**
 * \brief Generic Inner Read Implementation
 *
 * Implementation is done for inter-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerRead_PDAI2
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Read Implementation
 *
 * Implementation is done for intra-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerRead_PDAI2
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write Implementation
 *
 * Implementation is done for inter-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 *         and result of internal runtime checks in Byte 2
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite_PDAI2
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write Implementation
 *
 * Implementation is done for intra-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 *         and result of internal runtime checks in Byte 2
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite_PDAI2
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write2 Implementation
 *
 * Implementation is done for inter-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to Configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IR_GenericInnerWrite2_PDAI2
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

/**
 * \brief Generic Inner Write2 Implementation
 *
 * Implementation is done for intra-Ecu communication with DataElement
 * of complex datatype.
 *
 * \param[in] Config Pointer to configuration data
 * \param[in] AppData Pointer to application DataElement
 * \param[in] SerDataDEData Pointer to a pre-allocated uint8 array or DataElement (used for serialization)
 *
 * \return uint32 value containing return value of E2E Library function in Byte 0
 *         and result of redundancy check in Byte 3
 */
extern FUNC(uint32, E2EPW_CODE) E2EPW_CX_IA_GenericInnerWrite2_PDAI2
(
   P2CONST(E2EPW_GenericConfigType, AUTOMATIC, E2EPW_CONST) Config,
   P2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) AppData,
   P2VAR(void, AUTOMATIC, E2EPW_VAR) SerDataDEData
);

#endif /* (E2EPW_ENABLE_PROFILE_PDAI2 == STD_ON) */

#define E2EPW_STOP_SEC_CODE
#include <MemMap.h>

#endif /* ((E2EPW_ENABLE_PROFILE_P01 == STD_ON) || \
      (E2EPW_ENABLE_PROFILE_P02 == STD_ON) ||  \
      (E2EPW_ENABLE_PROFILE_PDAI1 == STD_ON) ||  \
      (E2EPW_ENABLE_PROFILE_PDAI2 == STD_ON)) */

#endif /* if !defined( E2EPW_INT_STC_H ) */
/*==================[end of file]============================================*/
