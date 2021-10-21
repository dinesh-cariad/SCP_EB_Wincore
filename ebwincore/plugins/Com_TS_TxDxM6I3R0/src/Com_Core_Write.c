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

/*==================[inclusions]============================================*/

#include <TSAutosar.h>
#include <Com_Core_Write.h>
#include <Com_Cfg.h>
#include <TSMem.h>

/*==================[macros]================================================*/

#if (defined COM_EB_CHECKRANGE08)        /* To prevent double declaration */
#error COM_EB_CHECKRANGE08 is already defined
#endif /* (defined COM_EB_CHECKRANGE08) */

#if (defined COM_EB_CHECKRANGE16)        /* To prevent double declaration */
#error COM_EB_CHECKRANGE16 is already defined
#endif /* (defined COM_EB_CHECKRANGE16) */

#if (defined COM_EB_CHECKRANGE32)        /* To prevent double declaration */
#error COM_EB_CHECKRANGE32 is already defined
#endif /* (defined COM_EB_CHECKRANGE32) */

#if (COM_CHECK_VALUE_SIZE == STD_ON)
#define COM_EB_CHECKRANGE08(val,WritePropPtr,retval) \
   do{ \
      Com_EB_CheckValueSize8Bit((uint8)(val),(WritePropPtr));\
      (retval) = E_OK; \
   }while(0)
#define COM_EB_CHECKRANGE16(val,WritePropPtr,retval) \
   do{ \
      Com_EB_CheckValueSize16Bit((uint16)(val),(WritePropPtr));\
      (retval) = E_OK; \
   }while(0)
#define COM_EB_CHECKRANGE32(val,WritePropPtr,retval) \
   do{ \
      Com_EB_CheckValueSize32Bit((uint32)(val),(WritePropPtr));\
      (retval) = E_OK; \
   }while(0)
#else
#define COM_EB_CHECKRANGE08(val,WritePropPtr,retval)
#define COM_EB_CHECKRANGE16(val,WritePropPtr,retval)
#define COM_EB_CHECKRANGE32(val,WritePropPtr,retval)
#endif

#if (defined COM_EB_CONVERT_ENDIAN_POS_NET_E1)        /* To prevent double declaration */
#error COM_EB_CONVERT_ENDIAN_POS_NET_E1 is already defined
#endif /* (defined COM_EB_CONVERT_ENDIAN_POS_NET_E1) */

#if (defined COM_EB_CONVERT_ENDIAN_POS_NET_E2)        /* To prevent double declaration */
#error COM_EB_CONVERT_ENDIAN_POS_NET_E2 is already defined
#endif /* (defined COM_EB_CONVERT_ENDIAN_POS_NET_E2) */

#if (defined COM_EB_CONVERT_ENDIAN_POS_NET_E1_2BYTE)        /* To prevent double declaration */
#error COM_EB_CONVERT_ENDIAN_POS_NET_E1_2BYTE is already defined
#endif /* (defined COM_EB_CONVERT_ENDIAN_POS_NET_E1_2BYTE) */

#if (defined COM_EB_CONVERT_ENDIAN_POS_NET_E2_2BYTE)        /* To prevent double declaration */
#error COM_EB_CONVERT_ENDIAN_POS_NET_E2_2BYTE is already defined
#endif /* (defined COM_EB_CONVERT_ENDIAN_POS_NET_E2_2BYTE) */

#if (defined COM_EB_CONVERT_ENDIAN_POS_NET_E1_4BYTE)        /* To prevent double declaration */
#error COM_EB_CONVERT_ENDIAN_POS_NET_E1_4BYTE is already defined
#endif /* (defined COM_EB_CONVERT_ENDIAN_POS_NET_E1_4BYTE) */

#if (defined COM_EB_CONVERT_ENDIAN_POS_NET_E2_4BYTE)        /* To prevent double declaration */
#error COM_EB_CONVERT_ENDIAN_POS_NET_E2_4BYTE is already defined
#endif /* (defined COM_EB_CONVERT_ENDIAN_POS_NET_E2_4BYTE) */

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#define COM_EB_CONVERT_ENDIAN_POS_NET_E1(bytePos, size, offset) ((bytePos) + (offset))
#define COM_EB_CONVERT_ENDIAN_POS_NET_E2(bytePos, size, offset) (((bytePos) + ((size) - 1U)) - (offset))
#define COM_EB_CONVERT_ENDIAN_POS_NET_E1_2BYTE(x) (x)
#define COM_EB_CONVERT_ENDIAN_POS_NET_E2_2BYTE(x) (uint16)( (uint16)((x) >> 8U) | (uint16)((x) << 8U))
#define COM_EB_CONVERT_ENDIAN_POS_NET_E1_4BYTE(x) (x)
#define COM_EB_CONVERT_ENDIAN_POS_NET_E2_4BYTE(x) (uint32)((((uint32)(x) & 0x000000FFUL) << 24) \
        | (uint32)((((uint32)(x) & 0x0000FF00UL) << 8) | \
                (uint32)((((uint32)(x) & 0x00FF0000UL) >> 8) | \
                        (uint32)(((uint32)(x) & 0xFF000000UL) >> 24))))
#else
#define COM_EB_CONVERT_ENDIAN_POS_NET_E1(bytePos, size, offset) (((bytePos) + ((size) - 1U)) - (offset))
#define COM_EB_CONVERT_ENDIAN_POS_NET_E2(bytePos, size, offset) ((bytePos) + (offset))
#define COM_EB_CONVERT_ENDIAN_POS_NET_E1_2BYTE(x) (uint16)(((x) >> 8U) | ((x) << 8U))
#define COM_EB_CONVERT_ENDIAN_POS_NET_E2_2BYTE(x) (x)
#define COM_EB_CONVERT_ENDIAN_POS_NET_E1_4BYTE(x) (uint32)((((uint32)(x) & 0x000000FFUL) << 24) | \
        (uint32)((((uint32)(x) & 0x0000FF00UL) << 8) | \
                (uint32)((((uint32)(x) & 0x00FF0000UL) >> 8) | \
                        (uint32)(((uint32)(x) & 0xFF000000UL) >> 24))))
#define COM_EB_CONVERT_ENDIAN_POS_NET_E2_4BYTE(x) (x)
#define COM_EB_CONVERT_ENDIAN_POS_NET_E2_8BYTE(x) (x)
#endif

#if (defined Com_EB_SetUB)        /* To prevent double declaration */
#error Com_EB_SetUB is already defined
#endif /* (defined Com_EB_SetUB) */

#if (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)
#define Com_EB_SetUB(Address, Bit) Com_SetBitUInt8((Address), (Bit))
#else
#define Com_EB_SetUB(Address, Bit)
#endif /* (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define COM_START_SEC_CODE
#include <Com_MemMap.h>

#if (COM_CHECK_VALUE_SIZE == STD_ON)

/** \brief Com_EB_CheckValueSize8Bit - checks if the value fits into the
 * signal and if the size of the signal fits to its type.
 * If not, DET is called.
 * \param[in] Val - value which shall be checked
 * \param[in] Com_EB_WritePropPtr - configuration of a signal
 * \return
 * \retval None
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckValueSize8Bit
(
   uint8 Val,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr
);


/** \brief Com_EB_CheckValueSize16Bit - checks if the value fits into the
 * signal and if the size of the signal fits to its type.
 * If not, DET is called.
 * \param[in] Val - value which shall be checked
 * \param[in] Com_EB_WritePropPtr - configuration of a signal
 * \return
 * \retval None
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckValueSize16Bit
(
   uint16 Val,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr
);


/** \brief Com_EB_CheckValueSize32Bit - checks if the value fits into the
 * signal and if the size of the signal fits to its type.
 * If not, DET is called.
 * \param[in] Val - value which shall be checked
 * \param[in] Com_EB_WritePropPtr - configuration of a signal
 * \return
 * \retval None
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckValueSize32Bit
(
   uint32 Val,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr
);

#endif /* (COM_CHECK_VALUE_SIZE == STD_ON) */


#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_DL_DUB_DOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

);
#else
#define Com_EB_Write_16_SX_DL_DUB_DOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

);
#else
#define Com_EB_Write_32_SX_DL_DUB_DOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

);
#else
#define Com_EB_Write_64_SX_DL_DUB_DOC_E1 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_DL_DUB_DOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_DL_DUB_DOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_DL_DUB_DOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_DL_DUB_DOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_DL_DUB_DOC_E2 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_DL_DUB_EOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_DL_DUB_EOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_DL_DUB_EOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_DL_DUB_EOC_E1 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_DL_DUB_EOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_DL_DUB_EOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_DL_DUB_EOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_DL_DUB_EOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_DL_DUB_EOC_E2 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_DL_EUB_DOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_DL_EUB_DOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_DL_EUB_DOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_DL_EUB_DOC_E1 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_DL_EUB_DOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_DL_EUB_DOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_DL_EUB_DOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_DL_EUB_DOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_DL_EUB_DOC_E2 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_DL_EUB_EOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_DL_EUB_EOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_DL_EUB_EOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_DL_EUB_EOC_E1 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_DL_EUB_EOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_DL_EUB_EOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_DL_EUB_EOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_DL_EUB_EOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_DL_EUB_EOC_E2 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if (COM_TX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_EL_DUB_DOC_E1 Com_EB_Write_DET
#endif

#if (COM_TX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_EL_DUB_DOC_E1 Com_EB_Write_DET
#endif

#if (COM_TX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_EL_DUB_DOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_EL_DUB_DOC_E1 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_EL_DUB_DOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if (COM_TX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_EL_DUB_DOC_E2 Com_EB_Write_DET
#endif

#if (COM_TX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_EL_DUB_DOC_E2 Com_EB_Write_DET
#endif

#if (COM_TX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_EL_DUB_DOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_EL_DUB_DOC_E2 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_EL_DUB_EOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_EL_DUB_EOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_EL_DUB_EOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_EL_DUB_EOC_E1 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_EL_DUB_EOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_EL_DUB_EOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_EL_DUB_EOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_EL_DUB_EOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_EL_DUB_EOC_E2 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_EL_EUB_DOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_EL_EUB_DOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_EL_EUB_DOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_EL_EUB_DOC_E1 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_EL_EUB_DOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_EL_EUB_DOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_EL_EUB_DOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_EL_EUB_DOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_EL_EUB_DOC_E2 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_EL_EUB_EOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_EL_EUB_EOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_EL_EUB_EOC_E1 Com_EB_Write_DET
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_EL_EUB_EOC_E1 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_EL_EUB_EOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_08_SX_EL_EUB_EOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_16_SX_EL_EUB_EOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_32_SX_EL_EUB_EOC_E2 Com_EB_Write_DET
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);
#else
#define Com_EB_Write_64_SX_EL_EUB_EOC_E2 Com_EB_Write_DET
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

static FUNC(void, COM_CODE) Com_EB_Write_BO_DL_DUB_DOC
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

);

static FUNC(void, COM_CODE) Com_EB_Write_A1_DL_DUB_DOC
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

);

static FUNC(void, COM_CODE) Com_EB_Write_BO_EL_DUB_DOC
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

);

static FUNC(void, COM_CODE) Com_EB_Write_A1_EL_DUB_DOC
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

);

static FUNC(void, COM_CODE) Com_EB_Write_DET
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>


/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#define COM_START_SEC_CONST_UNSPECIFIED
#include <Com_MemMap.h>

CONSTP2VAR(Com_WriteFuncType, AUTOMATIC, COM_CODE)
        COM_WRITEFUNCS[COM_CORE_WRITE_ARRAY_SIZE] =
{
    /*   0 */  Com_EB_Write_08_SX_DL_DUB_DOC_E1,
    /*   1 */  Com_EB_Write_16_SX_DL_DUB_DOC_E1,
    /*   2 */  Com_EB_Write_32_SX_DL_DUB_DOC_E1,
    /*   3 */  Com_EB_Write_AY_S0_DL_DUB_DOC_E1,
    /*   4 */  Com_EB_Write_08_SX_DL_DUB_DOC_E1,
    /*   5 */  Com_EB_Write_16_SX_DL_DUB_DOC_E1,
    /*   6 */  Com_EB_Write_32_SX_DL_DUB_DOC_E1,
    /*   7 */  Com_EB_Write_AY_S1_DL_DUB_DOC_E3, /* UINT8/DYN, noLock, noUB, noTOC */
    /*   8 */  Com_EB_Write_08_SX_DL_DUB_DOC_E2,
    /*   9 */  Com_EB_Write_16_SX_DL_DUB_DOC_E2,
    /*  10 */  Com_EB_Write_32_SX_DL_DUB_DOC_E2,
    /*  11 */  Com_EB_Write_AY_S0_DL_DUB_DOC_E2,
    /*  12 */  Com_EB_Write_08_SX_DL_DUB_DOC_E2,
    /*  13 */  Com_EB_Write_16_SX_DL_DUB_DOC_E2,
    /*  14 */  Com_EB_Write_32_SX_DL_DUB_DOC_E2,
    /*  15 */                  Com_EB_Write_DET,
    /*  16 */  Com_EB_Write_08_SX_DL_DUB_EOC_E1,
    /*  17 */  Com_EB_Write_16_SX_DL_DUB_EOC_E1,
    /*  18 */  Com_EB_Write_32_SX_DL_DUB_EOC_E1,
    /*  19 */  Com_EB_Write_AY_S0_DL_DUB_EOC_E1,
    /*  20 */  Com_EB_Write_08_SX_DL_DUB_EOC_E1,
    /*  21 */  Com_EB_Write_16_SX_DL_DUB_EOC_E1,
    /*  22 */  Com_EB_Write_32_SX_DL_DUB_EOC_E1,
    /*  23 */  Com_EB_Write_AY_S1_DL_DUB_EOC_E3,  /* UINT8/DYN, noLock, noUB, TOC */
    /*  24 */  Com_EB_Write_08_SX_DL_DUB_EOC_E2,
    /*  25 */  Com_EB_Write_16_SX_DL_DUB_EOC_E2,
    /*  26 */  Com_EB_Write_32_SX_DL_DUB_EOC_E2,
    /*  27 */  Com_EB_Write_AY_S0_DL_DUB_EOC_E2,
    /*  28 */  Com_EB_Write_08_SX_DL_DUB_EOC_E2,
    /*  29 */  Com_EB_Write_16_SX_DL_DUB_EOC_E2,
    /*  30 */  Com_EB_Write_32_SX_DL_DUB_EOC_E2,
    /*  31 */                  Com_EB_Write_DET,
    /*  32 */  Com_EB_Write_08_SX_DL_EUB_DOC_E1,
    /*  33 */  Com_EB_Write_16_SX_DL_EUB_DOC_E1,
    /*  34 */  Com_EB_Write_32_SX_DL_EUB_DOC_E1,
    /*  35 */  Com_EB_Write_AY_S0_DL_EUB_DOC_E1,
    /*  36 */  Com_EB_Write_08_SX_DL_EUB_DOC_E1,
    /*  37 */  Com_EB_Write_16_SX_DL_EUB_DOC_E1,
    /*  38 */  Com_EB_Write_32_SX_DL_EUB_DOC_E1,
    /*  39 */  Com_EB_Write_AY_S1_DL_EUB_DOC_E3,  /* UINT8/DYN, noLock, UB, noTOC */
    /*  40 */  Com_EB_Write_08_SX_DL_EUB_DOC_E2,
    /*  41 */  Com_EB_Write_16_SX_DL_EUB_DOC_E2,
    /*  42 */  Com_EB_Write_32_SX_DL_EUB_DOC_E2,
    /*  43 */  Com_EB_Write_AY_S0_DL_EUB_DOC_E2,
    /*  44 */  Com_EB_Write_08_SX_DL_EUB_DOC_E2,
    /*  45 */  Com_EB_Write_16_SX_DL_EUB_DOC_E2,
    /*  46 */  Com_EB_Write_32_SX_DL_EUB_DOC_E2,
    /*  47 */                  Com_EB_Write_DET,
    /*  48 */  Com_EB_Write_08_SX_DL_EUB_EOC_E1,
    /*  49 */  Com_EB_Write_16_SX_DL_EUB_EOC_E1,
    /*  50 */  Com_EB_Write_32_SX_DL_EUB_EOC_E1,
    /*  51 */  Com_EB_Write_AY_S0_DL_EUB_EOC_E1,
    /*  52 */  Com_EB_Write_08_SX_DL_EUB_EOC_E1,
    /*  53 */  Com_EB_Write_16_SX_DL_EUB_EOC_E1,
    /*  54 */  Com_EB_Write_32_SX_DL_EUB_EOC_E1,
    /*  55 */  Com_EB_Write_AY_S1_DL_EUB_EOC_E3,  /* UINT8/DYN, noLock, UB, TOC */
    /*  56 */  Com_EB_Write_08_SX_DL_EUB_EOC_E2,
    /*  57 */  Com_EB_Write_16_SX_DL_EUB_EOC_E2,
    /*  58 */  Com_EB_Write_32_SX_DL_EUB_EOC_E2,
    /*  59 */  Com_EB_Write_AY_S0_DL_EUB_EOC_E2,
    /*  60 */  Com_EB_Write_08_SX_DL_EUB_EOC_E2,
    /*  61 */  Com_EB_Write_16_SX_DL_EUB_EOC_E2,
    /*  62 */  Com_EB_Write_32_SX_DL_EUB_EOC_E2,
    /*  63 */                  Com_EB_Write_DET,
    /*  64 */  Com_EB_Write_08_SX_EL_DUB_DOC_E1,
    /*  65 */  Com_EB_Write_16_SX_EL_DUB_DOC_E1,
    /*  66 */  Com_EB_Write_32_SX_EL_DUB_DOC_E1,
    /*  67 */  Com_EB_Write_AY_S0_EL_DUB_DOC_E1,
    /*  68 */  Com_EB_Write_08_SX_EL_DUB_DOC_E1,
    /*  69 */  Com_EB_Write_16_SX_EL_DUB_DOC_E1,
    /*  70 */  Com_EB_Write_32_SX_EL_DUB_DOC_E1,
    /*  71 */  Com_EB_Write_AY_S1_EL_DUB_DOC_E3,  /* UINT8/DYN, Lock, noUB, noTOC */
    /*  72 */  Com_EB_Write_08_SX_EL_DUB_DOC_E2,
    /*  73 */  Com_EB_Write_16_SX_EL_DUB_DOC_E2,
    /*  74 */  Com_EB_Write_32_SX_EL_DUB_DOC_E2,
    /*  75 */  Com_EB_Write_AY_S0_EL_DUB_DOC_E2,
    /*  76 */  Com_EB_Write_08_SX_EL_DUB_DOC_E2,
    /*  77 */  Com_EB_Write_16_SX_EL_DUB_DOC_E2,
    /*  78 */  Com_EB_Write_32_SX_EL_DUB_DOC_E2,
    /*  79 */                  Com_EB_Write_DET,
    /*  80 */  Com_EB_Write_08_SX_EL_DUB_EOC_E1,
    /*  81 */  Com_EB_Write_16_SX_EL_DUB_EOC_E1,
    /*  82 */  Com_EB_Write_32_SX_EL_DUB_EOC_E1,
    /*  83 */  Com_EB_Write_AY_S0_EL_DUB_EOC_E1,
    /*  84 */  Com_EB_Write_08_SX_EL_DUB_EOC_E1,
    /*  85 */  Com_EB_Write_16_SX_EL_DUB_EOC_E1,
    /*  86 */  Com_EB_Write_32_SX_EL_DUB_EOC_E1,
    /*  87 */  Com_EB_Write_AY_S1_EL_DUB_EOC_E3,  /* UINT8/DYN, Lock, noUB, TOC */
    /*  88 */  Com_EB_Write_08_SX_EL_DUB_EOC_E2,
    /*  89 */  Com_EB_Write_16_SX_EL_DUB_EOC_E2,
    /*  90 */  Com_EB_Write_32_SX_EL_DUB_EOC_E2,
    /*  91 */  Com_EB_Write_AY_S0_EL_DUB_EOC_E2,
    /*  92 */  Com_EB_Write_08_SX_EL_DUB_EOC_E2,
    /*  93 */  Com_EB_Write_16_SX_EL_DUB_EOC_E2,
    /*  94 */  Com_EB_Write_32_SX_EL_DUB_EOC_E2,
    /*  95 */                  Com_EB_Write_DET,
    /*  96 */  Com_EB_Write_08_SX_EL_EUB_DOC_E1,
    /*  97 */  Com_EB_Write_16_SX_EL_EUB_DOC_E1,
    /*  98 */  Com_EB_Write_32_SX_EL_EUB_DOC_E1,
    /*  99 */  Com_EB_Write_AY_S0_EL_EUB_DOC_E1,
    /* 100 */  Com_EB_Write_08_SX_EL_EUB_DOC_E1,
    /* 101 */  Com_EB_Write_16_SX_EL_EUB_DOC_E1,
    /* 102 */  Com_EB_Write_32_SX_EL_EUB_DOC_E1,
    /* 103 */  Com_EB_Write_AY_S1_EL_EUB_DOC_E3,   /* UINT8/DYN, Lock, UB, noTOC */
    /* 104 */  Com_EB_Write_08_SX_EL_EUB_DOC_E2,
    /* 105 */  Com_EB_Write_16_SX_EL_EUB_DOC_E2,
    /* 106 */  Com_EB_Write_32_SX_EL_EUB_DOC_E2,
    /* 107 */  Com_EB_Write_AY_S0_EL_EUB_DOC_E2,
    /* 108 */  Com_EB_Write_08_SX_EL_EUB_DOC_E2,
    /* 109 */  Com_EB_Write_16_SX_EL_EUB_DOC_E2,
    /* 110 */  Com_EB_Write_32_SX_EL_EUB_DOC_E2,
    /* 111 */                  Com_EB_Write_DET,
    /* 112 */  Com_EB_Write_08_SX_EL_EUB_EOC_E1,
    /* 113 */  Com_EB_Write_16_SX_EL_EUB_EOC_E1,
    /* 114 */  Com_EB_Write_32_SX_EL_EUB_EOC_E1,
    /* 115 */  Com_EB_Write_AY_S0_EL_EUB_EOC_E1,
    /* 116 */  Com_EB_Write_08_SX_EL_EUB_EOC_E1,
    /* 117 */  Com_EB_Write_16_SX_EL_EUB_EOC_E1,
    /* 118 */  Com_EB_Write_32_SX_EL_EUB_EOC_E1,
    /* 119 */  Com_EB_Write_AY_S1_EL_EUB_EOC_E3,   /* UINT8/DYN, Lock, UB, noTOC */
    /* 120 */  Com_EB_Write_08_SX_EL_EUB_EOC_E2,
    /* 121 */  Com_EB_Write_16_SX_EL_EUB_EOC_E2,
    /* 122 */  Com_EB_Write_32_SX_EL_EUB_EOC_E2,
    /* 123 */  Com_EB_Write_AY_S0_EL_EUB_EOC_E2,
    /* 124 */  Com_EB_Write_08_SX_EL_EUB_EOC_E2,
    /* 125 */  Com_EB_Write_16_SX_EL_EUB_EOC_E2,
    /* 126 */  Com_EB_Write_32_SX_EL_EUB_EOC_E2,
    /* 127 */                  Com_EB_Write_DET,

    /* 128 */  Com_EB_Write_BO_DL_DUB_DOC,            /* BOOLEAN                    NoLock, noUb, noTOC, XE */
    /* 129 */  Com_EB_Write_A1_DL_DUB_DOC,            /* S/UINT8 aligned size 8;    NoLock, noUb, noTOC, XE */
    /* 130 */  Com_EB_Write_DET,
    /* 131 */  Com_EB_Write_64_SX_DL_DUB_DOC_E1,
    /* 132 */  Com_EB_Write_DET,
    /* 133 */  Com_EB_Write_DET,
    /* 134 */  Com_EB_Write_DET,
    /* 135 */  Com_EB_Write_64_SX_DL_DUB_DOC_E1,
    /* 136 */  Com_EB_Write_DET,
    /* 137 */  Com_EB_Write_DET,
    /* 138 */  Com_EB_Write_DET,
    /* 139 */  Com_EB_Write_64_SX_DL_DUB_DOC_E2,
    /* 140 */  Com_EB_Write_DET,
    /* 141 */  Com_EB_Write_DET,
    /* 142 */  Com_EB_Write_DET,
    /* 143 */  Com_EB_Write_64_SX_DL_DUB_DOC_E2,
    /* 144 */  Com_EB_Write_DET,
    /* 145 */  Com_EB_Write_DET,
    /* 146 */  Com_EB_Write_DET,
    /* 147 */  Com_EB_Write_64_SX_DL_DUB_EOC_E1,
    /* 148 */  Com_EB_Write_DET,
    /* 149 */  Com_EB_Write_DET,
    /* 150 */  Com_EB_Write_DET,
    /* 151 */  Com_EB_Write_64_SX_DL_DUB_EOC_E1,
    /* 152 */  Com_EB_Write_DET,
    /* 153 */  Com_EB_Write_DET,
    /* 154 */  Com_EB_Write_DET,
    /* 155 */  Com_EB_Write_64_SX_DL_DUB_EOC_E2,
    /* 156 */  Com_EB_Write_DET,
    /* 157 */  Com_EB_Write_DET,
    /* 158 */  Com_EB_Write_DET,
    /* 159 */  Com_EB_Write_64_SX_DL_DUB_EOC_E2,
    /* 160 */  Com_EB_Write_DET,
    /* 161 */  Com_EB_Write_DET,
    /* 162 */  Com_EB_Write_DET,
    /* 163 */  Com_EB_Write_64_SX_DL_EUB_DOC_E1,
    /* 164 */  Com_EB_Write_DET,
    /* 165 */  Com_EB_Write_DET,
    /* 166 */  Com_EB_Write_DET,
    /* 167 */  Com_EB_Write_64_SX_DL_EUB_DOC_E1,
    /* 168 */  Com_EB_Write_DET,
    /* 169 */  Com_EB_Write_DET,
    /* 170 */  Com_EB_Write_DET,
    /* 171 */  Com_EB_Write_64_SX_DL_EUB_DOC_E2,
    /* 172 */  Com_EB_Write_DET,
    /* 173 */  Com_EB_Write_DET,
    /* 174 */  Com_EB_Write_DET,
    /* 175 */  Com_EB_Write_64_SX_DL_EUB_DOC_E2,
    /* 176 */  Com_EB_Write_DET,
    /* 177 */  Com_EB_Write_DET,
    /* 178 */  Com_EB_Write_DET,
    /* 179 */  Com_EB_Write_64_SX_DL_EUB_EOC_E1,
    /* 180 */  Com_EB_Write_DET,
    /* 181 */  Com_EB_Write_DET,
    /* 182 */  Com_EB_Write_DET,
    /* 183 */  Com_EB_Write_64_SX_DL_EUB_EOC_E1,
    /* 184 */  Com_EB_Write_DET,
    /* 185 */  Com_EB_Write_DET,
    /* 186 */  Com_EB_Write_DET,
    /* 187 */  Com_EB_Write_64_SX_DL_EUB_EOC_E2,
    /* 188 */  Com_EB_Write_DET,
    /* 189 */  Com_EB_Write_DET,
    /* 190 */  Com_EB_Write_DET,
    /* 191 */  Com_EB_Write_64_SX_DL_EUB_EOC_E2,
    /* 192 */  Com_EB_Write_BO_EL_DUB_DOC,            /* BOOLEAN                      Lock, noUb, noTOC, XE */
    /* 193 */  Com_EB_Write_A1_EL_DUB_DOC,            /* S/UINT8 aligned size 8;      Lock, noUb, noTOC, XE */
    /* 194 */  Com_EB_Write_DET,
    /* 195 */  Com_EB_Write_64_SX_EL_DUB_DOC_E1,
    /* 196 */  Com_EB_Write_DET,
    /* 197 */  Com_EB_Write_DET,
    /* 198 */  Com_EB_Write_DET,
    /* 199 */  Com_EB_Write_64_SX_EL_DUB_DOC_E1,
    /* 200 */  Com_EB_Write_DET,
    /* 201 */  Com_EB_Write_DET,
    /* 202 */  Com_EB_Write_DET,
    /* 203 */  Com_EB_Write_64_SX_EL_DUB_DOC_E2,
    /* 204 */  Com_EB_Write_DET,
    /* 205 */  Com_EB_Write_DET,
    /* 206 */  Com_EB_Write_DET,
    /* 207 */  Com_EB_Write_64_SX_EL_DUB_DOC_E2,
    /* 208 */  Com_EB_Write_DET,
    /* 209 */  Com_EB_Write_DET,
    /* 210 */  Com_EB_Write_DET,
    /* 211 */  Com_EB_Write_64_SX_EL_DUB_EOC_E1,
    /* 212 */  Com_EB_Write_DET,
    /* 213 */  Com_EB_Write_DET,
    /* 214 */  Com_EB_Write_DET,
    /* 215 */  Com_EB_Write_64_SX_EL_DUB_EOC_E1,
    /* 216 */  Com_EB_Write_DET,
    /* 217 */  Com_EB_Write_DET,
    /* 218 */  Com_EB_Write_DET,
    /* 219 */  Com_EB_Write_64_SX_EL_DUB_EOC_E2,
    /* 220 */  Com_EB_Write_DET,
    /* 221 */  Com_EB_Write_DET,
    /* 222 */  Com_EB_Write_DET,
    /* 223 */  Com_EB_Write_64_SX_EL_DUB_EOC_E2,
    /* 224 */  Com_EB_Write_DET,
    /* 225 */  Com_EB_Write_DET,
    /* 226 */  Com_EB_Write_DET,
    /* 227 */  Com_EB_Write_64_SX_EL_EUB_DOC_E1,
    /* 228 */  Com_EB_Write_DET,
    /* 229 */  Com_EB_Write_DET,
    /* 230 */  Com_EB_Write_DET,
    /* 231 */  Com_EB_Write_64_SX_EL_EUB_DOC_E1,
    /* 232 */  Com_EB_Write_DET,
    /* 233 */  Com_EB_Write_DET,
    /* 234 */  Com_EB_Write_DET,
    /* 235 */  Com_EB_Write_64_SX_EL_EUB_DOC_E2,
    /* 236 */  Com_EB_Write_DET,
    /* 237 */  Com_EB_Write_DET,
    /* 238 */  Com_EB_Write_DET,
    /* 239 */  Com_EB_Write_64_SX_EL_EUB_DOC_E2,
    /* 240 */  Com_EB_Write_DET,
    /* 241 */  Com_EB_Write_DET,
    /* 242 */  Com_EB_Write_DET,
    /* 243 */  Com_EB_Write_64_SX_EL_EUB_EOC_E1,
    /* 244 */  Com_EB_Write_DET,
    /* 245 */  Com_EB_Write_DET,
    /* 246 */  Com_EB_Write_DET,
    /* 247 */  Com_EB_Write_64_SX_EL_EUB_EOC_E1,
    /* 248 */  Com_EB_Write_DET,
    /* 249 */  Com_EB_Write_DET,
    /* 250 */  Com_EB_Write_DET,
    /* 251 */  Com_EB_Write_64_SX_EL_EUB_EOC_E2,
    /* 252 */  Com_EB_Write_DET,
    /* 253 */  Com_EB_Write_DET,
    /* 254 */  Com_EB_Write_DET,
    /* 255 */  Com_EB_Write_64_SX_EL_EUB_EOC_E2,


    /* 256 */  Com_EB_Write_SignalExt,   /*   0 */
    /* 257 */  Com_EB_Write_SignalExt,   /*   1 */
    /* 258 */  Com_EB_Write_SignalExt,   /*   2 */
    /* 259 */  Com_EB_Write_SignalExt,   /*   3 */
    /* 260 */  Com_EB_Write_SignalExt,   /*   4 */
    /* 261 */  Com_EB_Write_SignalExt,   /*   5 */
    /* 262 */  Com_EB_Write_SignalExt,   /*   6 */
    /* 263 */  Com_EB_Write_SignalExt,   /*   7 */
    /* 264 */  Com_EB_Write_SignalExt,   /*   8 */
    /* 265 */  Com_EB_Write_SignalExt,   /*   9 */
    /* 266 */  Com_EB_Write_SignalExt,   /*  10 */
    /* 267 */  Com_EB_Write_SignalExt,   /*  11 */
    /* 268 */  Com_EB_Write_SignalExt,   /*  12 */
    /* 269 */  Com_EB_Write_SignalExt,   /*  13 */
    /* 270 */  Com_EB_Write_SignalExt,   /*  14 */
    /* 271 */        Com_EB_Write_DET,   /*  15 */
    /* 272 */  Com_EB_Write_SignalExt,   /*  16 */
    /* 273 */  Com_EB_Write_SignalExt,   /*  17 */
    /* 274 */  Com_EB_Write_SignalExt,   /*  18 */
    /* 275 */  Com_EB_Write_SignalExt,   /*  19 */
    /* 276 */  Com_EB_Write_SignalExt,   /*  20 */
    /* 277 */  Com_EB_Write_SignalExt,   /*  21 */
    /* 278 */  Com_EB_Write_SignalExt,   /*  22 */
    /* 279 */  Com_EB_Write_SignalExt,   /*  23 */
    /* 280 */  Com_EB_Write_SignalExt,   /*  24 */
    /* 281 */  Com_EB_Write_SignalExt,   /*  25 */
    /* 282 */  Com_EB_Write_SignalExt,   /*  26 */
    /* 283 */  Com_EB_Write_SignalExt,   /*  27 */
    /* 284 */  Com_EB_Write_SignalExt,   /*  28 */
    /* 285 */  Com_EB_Write_SignalExt,   /*  29 */
    /* 286 */  Com_EB_Write_SignalExt,   /*  30 */
    /* 287 */        Com_EB_Write_DET,   /*  31 */
    /* 288 */  Com_EB_Write_SignalExt,   /*  32 */
    /* 289 */  Com_EB_Write_SignalExt,   /*  33 */
    /* 290 */  Com_EB_Write_SignalExt,   /*  34 */
    /* 291 */  Com_EB_Write_SignalExt,   /*  35 */
    /* 292 */  Com_EB_Write_SignalExt,   /*  36 */
    /* 293 */  Com_EB_Write_SignalExt,   /*  37 */
    /* 294 */  Com_EB_Write_SignalExt,   /*  38 */
    /* 295 */  Com_EB_Write_SignalExt,   /*  39 */
    /* 296 */  Com_EB_Write_SignalExt,   /*  40 */
    /* 297 */  Com_EB_Write_SignalExt,   /*  41 */
    /* 298 */  Com_EB_Write_SignalExt,   /*  42 */
    /* 299 */  Com_EB_Write_SignalExt,   /*  43 */
    /* 300 */  Com_EB_Write_SignalExt,   /*  44 */
    /* 301 */  Com_EB_Write_SignalExt,   /*  45 */
    /* 302 */  Com_EB_Write_SignalExt,   /*  46 */
    /* 303 */        Com_EB_Write_DET,   /*  47 */
    /* 304 */  Com_EB_Write_SignalExt,   /*  48 */
    /* 305 */  Com_EB_Write_SignalExt,   /*  49 */
    /* 306 */  Com_EB_Write_SignalExt,   /*  50 */
    /* 307 */  Com_EB_Write_SignalExt,   /*  51 */
    /* 308 */  Com_EB_Write_SignalExt,   /*  52 */
    /* 309 */  Com_EB_Write_SignalExt,   /*  53 */
    /* 310 */  Com_EB_Write_SignalExt,   /*  54 */
    /* 311 */  Com_EB_Write_SignalExt,   /*  55 */
    /* 312 */  Com_EB_Write_SignalExt,   /*  56 */
    /* 313 */  Com_EB_Write_SignalExt,   /*  57 */
    /* 314 */  Com_EB_Write_SignalExt,   /*  58 */
    /* 315 */  Com_EB_Write_SignalExt,   /*  59 */
    /* 316 */  Com_EB_Write_SignalExt,   /*  60 */
    /* 317 */  Com_EB_Write_SignalExt,   /*  61 */
    /* 318 */  Com_EB_Write_SignalExt,   /*  62 */
    /* 319 */        Com_EB_Write_DET,   /*  63 */
    /* 320 */  Com_EB_Write_SignalExt,   /*  64 */
    /* 321 */  Com_EB_Write_SignalExt,   /*  65 */
    /* 322 */  Com_EB_Write_SignalExt,   /*  66 */
    /* 323 */  Com_EB_Write_SignalExt,   /*  67 */
    /* 324 */  Com_EB_Write_SignalExt,   /*  68 */
    /* 325 */  Com_EB_Write_SignalExt,   /*  69 */
    /* 326 */  Com_EB_Write_SignalExt,   /*  70 */
    /* 327 */  Com_EB_Write_SignalExt,   /*  71 */
    /* 328 */  Com_EB_Write_SignalExt,   /*  72 */
    /* 329 */  Com_EB_Write_SignalExt,   /*  73 */
    /* 330 */  Com_EB_Write_SignalExt,   /*  74 */
    /* 331 */  Com_EB_Write_SignalExt,   /*  75 */
    /* 332 */  Com_EB_Write_SignalExt,   /*  76 */
    /* 333 */  Com_EB_Write_SignalExt,   /*  77 */
    /* 334 */  Com_EB_Write_SignalExt,   /*  78 */
    /* 335 */        Com_EB_Write_DET,   /*  79 */
    /* 336 */  Com_EB_Write_SignalExt,   /*  80 */
    /* 337 */  Com_EB_Write_SignalExt,   /*  81 */
    /* 338 */  Com_EB_Write_SignalExt,   /*  82 */
    /* 339 */  Com_EB_Write_SignalExt,   /*  83 */
    /* 340 */  Com_EB_Write_SignalExt,   /*  84 */
    /* 341 */  Com_EB_Write_SignalExt,   /*  85 */
    /* 342 */  Com_EB_Write_SignalExt,   /*  86 */
    /* 343 */  Com_EB_Write_SignalExt,   /*  87 */
    /* 344 */  Com_EB_Write_SignalExt,   /*  88 */
    /* 345 */  Com_EB_Write_SignalExt,   /*  89 */
    /* 346 */  Com_EB_Write_SignalExt,   /*  90 */
    /* 347 */  Com_EB_Write_SignalExt,   /*  91 */
    /* 348 */  Com_EB_Write_SignalExt,   /*  92 */
    /* 349 */  Com_EB_Write_SignalExt,   /*  93 */
    /* 350 */  Com_EB_Write_SignalExt,   /*  94 */
    /* 351 */        Com_EB_Write_DET,   /*  95 */
    /* 352 */  Com_EB_Write_SignalExt,   /*  96 */
    /* 353 */  Com_EB_Write_SignalExt,   /*  97 */
    /* 354 */  Com_EB_Write_SignalExt,   /*  98 */
    /* 355 */  Com_EB_Write_SignalExt,   /*  99 */
    /* 356 */  Com_EB_Write_SignalExt,   /* 100 */
    /* 357 */  Com_EB_Write_SignalExt,   /* 101 */
    /* 358 */  Com_EB_Write_SignalExt,   /* 102 */
    /* 359 */  Com_EB_Write_SignalExt,   /* 103 */
    /* 360 */  Com_EB_Write_SignalExt,   /* 104 */
    /* 361 */  Com_EB_Write_SignalExt,   /* 105 */
    /* 362 */  Com_EB_Write_SignalExt,   /* 106 */
    /* 363 */  Com_EB_Write_SignalExt,   /* 107 */
    /* 364 */  Com_EB_Write_SignalExt,   /* 108 */
    /* 365 */  Com_EB_Write_SignalExt,   /* 109 */
    /* 366 */  Com_EB_Write_SignalExt,   /* 110 */
    /* 367 */        Com_EB_Write_DET,   /* 111 */
    /* 368 */  Com_EB_Write_SignalExt,   /* 112 */
    /* 369 */  Com_EB_Write_SignalExt,   /* 113 */
    /* 370 */  Com_EB_Write_SignalExt,   /* 114 */
    /* 371 */  Com_EB_Write_SignalExt,   /* 115 */
    /* 372 */  Com_EB_Write_SignalExt,   /* 116 */
    /* 373 */  Com_EB_Write_SignalExt,   /* 117 */
    /* 374 */  Com_EB_Write_SignalExt,   /* 118 */
    /* 375 */  Com_EB_Write_SignalExt,   /* 119 */
    /* 376 */  Com_EB_Write_SignalExt,   /* 120 */
    /* 377 */  Com_EB_Write_SignalExt,   /* 121 */
    /* 378 */  Com_EB_Write_SignalExt,   /* 122 */
    /* 379 */  Com_EB_Write_SignalExt,   /* 123 */
    /* 380 */  Com_EB_Write_SignalExt,   /* 124 */
    /* 381 */  Com_EB_Write_SignalExt,   /* 125 */
    /* 382 */  Com_EB_Write_SignalExt,   /* 126 */
    /* 383 */        Com_EB_Write_DET,   /* 127 */

    /* 384 */  Com_EB_Write_SignalExt,   /* 128 */
    /* 385 */  Com_EB_Write_SignalExt,   /* 129 */
    /* 386 */  Com_EB_Write_SignalExt,   /* 130 */
    /* 387 */  Com_EB_Write_SignalExt,   /* 131 */
    /* 388 */  Com_EB_Write_SignalExt,   /* 132 */
    /* 389 */  Com_EB_Write_SignalExt,   /* 133 */
    /* 390 */        Com_EB_Write_DET,   /* 134 */
    /* 391 */  Com_EB_Write_SignalExt,   /* 135 */
    /* 392 */        Com_EB_Write_DET,   /* 136 */
    /* 393 */        Com_EB_Write_DET,   /* 137 */
    /* 394 */        Com_EB_Write_DET,   /* 138 */
    /* 395 */  Com_EB_Write_SignalExt,   /* 139 */
    /* 396 */        Com_EB_Write_DET,   /* 140 */
    /* 397 */        Com_EB_Write_DET,   /* 141 */
    /* 398 */        Com_EB_Write_DET,   /* 142 */
    /* 399 */  Com_EB_Write_SignalExt,   /* 143 */
    /* 400 */        Com_EB_Write_DET,   /* 144 */
    /* 401 */        Com_EB_Write_DET,   /* 145 */
    /* 402 */        Com_EB_Write_DET,   /* 146 */
    /* 403 */  Com_EB_Write_SignalExt,   /* 147 */
    /* 404 */        Com_EB_Write_DET,   /* 148 */
    /* 405 */        Com_EB_Write_DET,   /* 149 */
    /* 406 */        Com_EB_Write_DET,   /* 150 */
    /* 407 */  Com_EB_Write_SignalExt,   /* 151 */
    /* 408 */        Com_EB_Write_DET,   /* 152 */
    /* 409 */        Com_EB_Write_DET,   /* 153 */
    /* 410 */        Com_EB_Write_DET,   /* 154 */
    /* 411 */  Com_EB_Write_SignalExt,   /* 155 */
    /* 412 */        Com_EB_Write_DET,   /* 156 */
    /* 413 */        Com_EB_Write_DET,   /* 157 */
    /* 414 */        Com_EB_Write_DET,   /* 158 */
    /* 415 */  Com_EB_Write_SignalExt,   /* 159 */
    /* 416 */        Com_EB_Write_DET,   /* 160 */
    /* 417 */        Com_EB_Write_DET,   /* 161 */
    /* 418 */        Com_EB_Write_DET,   /* 162 */
    /* 419 */  Com_EB_Write_SignalExt,   /* 163 */
    /* 420 */        Com_EB_Write_DET,   /* 164 */
    /* 421 */        Com_EB_Write_DET,   /* 165 */
    /* 422 */        Com_EB_Write_DET,   /* 166 */
    /* 423 */  Com_EB_Write_SignalExt,   /* 167 */
    /* 424 */        Com_EB_Write_DET,   /* 168 */
    /* 425 */        Com_EB_Write_DET,   /* 169 */
    /* 426 */        Com_EB_Write_DET,   /* 170 */
    /* 427 */  Com_EB_Write_SignalExt,   /* 171 */
    /* 428 */        Com_EB_Write_DET,   /* 172 */
    /* 429 */        Com_EB_Write_DET,   /* 173 */
    /* 430 */        Com_EB_Write_DET,   /* 174 */
    /* 431 */  Com_EB_Write_SignalExt,   /* 175 */
    /* 432 */        Com_EB_Write_DET,   /* 176 */
    /* 433 */        Com_EB_Write_DET,   /* 177 */
    /* 434 */        Com_EB_Write_DET,   /* 178 */
    /* 435 */  Com_EB_Write_SignalExt,   /* 179 */
    /* 436 */        Com_EB_Write_DET,   /* 180 */
    /* 437 */        Com_EB_Write_DET,   /* 181 */
    /* 438 */        Com_EB_Write_DET,   /* 182 */
    /* 439 */  Com_EB_Write_SignalExt,   /* 183 */
    /* 440 */        Com_EB_Write_DET,   /* 184 */
    /* 441 */        Com_EB_Write_DET,   /* 185 */
    /* 442 */        Com_EB_Write_DET,   /* 186 */
    /* 443 */  Com_EB_Write_SignalExt,   /* 187 */
    /* 444 */        Com_EB_Write_DET,   /* 188 */
    /* 445 */        Com_EB_Write_DET,   /* 189 */
    /* 446 */        Com_EB_Write_DET,   /* 190 */
    /* 447 */  Com_EB_Write_SignalExt,   /* 191 */
    /* 448 */  Com_EB_Write_SignalExt,   /* 192 */
    /* 449 */  Com_EB_Write_SignalExt,   /* 193 */
    /* 450 */  Com_EB_Write_SignalExt,   /* 194 */
    /* 451 */  Com_EB_Write_SignalExt,   /* 195 */
    /* 452 */  Com_EB_Write_SignalExt,   /* 196 */
    /* 453 */  Com_EB_Write_SignalExt,   /* 197 */
    /* 454 */        Com_EB_Write_DET,   /* 198 */
    /* 455 */  Com_EB_Write_SignalExt,   /* 199 */
    /* 456 */        Com_EB_Write_DET,   /* 200 */
    /* 457 */        Com_EB_Write_DET,   /* 201 */
    /* 458 */        Com_EB_Write_DET,   /* 202 */
    /* 459 */  Com_EB_Write_SignalExt,   /* 203 */
    /* 460 */        Com_EB_Write_DET,   /* 204 */
    /* 461 */        Com_EB_Write_DET,   /* 205 */
    /* 462 */        Com_EB_Write_DET,   /* 206 */
    /* 463 */  Com_EB_Write_SignalExt,   /* 207 */
    /* 464 */        Com_EB_Write_DET,   /* 208 */
    /* 465 */        Com_EB_Write_DET,   /* 209 */
    /* 466 */        Com_EB_Write_DET,   /* 210 */
    /* 467 */  Com_EB_Write_SignalExt,   /* 211 */
    /* 468 */        Com_EB_Write_DET,   /* 212 */
    /* 469 */        Com_EB_Write_DET,   /* 213 */
    /* 470 */        Com_EB_Write_DET,   /* 214 */
    /* 471 */  Com_EB_Write_SignalExt,   /* 215 */
    /* 472 */        Com_EB_Write_DET,   /* 216 */
    /* 473 */        Com_EB_Write_DET,   /* 217 */
    /* 474 */        Com_EB_Write_DET,   /* 218 */
    /* 475 */  Com_EB_Write_SignalExt,   /* 219 */
    /* 476 */        Com_EB_Write_DET,   /* 220 */
    /* 477 */        Com_EB_Write_DET,   /* 221 */
    /* 478 */        Com_EB_Write_DET,   /* 222 */
    /* 479 */  Com_EB_Write_SignalExt,   /* 223 */
    /* 480 */        Com_EB_Write_DET,   /* 224 */
    /* 481 */        Com_EB_Write_DET,   /* 225 */
    /* 482 */        Com_EB_Write_DET,   /* 226 */
    /* 483 */  Com_EB_Write_SignalExt,   /* 227 */
    /* 484 */        Com_EB_Write_DET,   /* 228 */
    /* 485 */        Com_EB_Write_DET,   /* 229 */
    /* 486 */        Com_EB_Write_DET,   /* 230 */
    /* 487 */  Com_EB_Write_SignalExt,   /* 231 */
    /* 488 */        Com_EB_Write_DET,   /* 232 */
    /* 489 */        Com_EB_Write_DET,   /* 233 */
    /* 490 */        Com_EB_Write_DET,   /* 234 */
    /* 491 */  Com_EB_Write_SignalExt,   /* 235 */
    /* 492 */        Com_EB_Write_DET,   /* 236 */
    /* 493 */        Com_EB_Write_DET,   /* 237 */
    /* 494 */        Com_EB_Write_DET,   /* 238 */
    /* 495 */  Com_EB_Write_SignalExt,   /* 239 */
    /* 496 */        Com_EB_Write_DET,   /* 240 */
    /* 497 */        Com_EB_Write_DET,   /* 241 */
    /* 498 */        Com_EB_Write_DET,   /* 242 */
    /* 499 */  Com_EB_Write_SignalExt,   /* 243 */
    /* 500 */        Com_EB_Write_DET,   /* 244 */
    /* 501 */        Com_EB_Write_DET,   /* 245 */
    /* 502 */        Com_EB_Write_DET,   /* 246 */
    /* 503 */  Com_EB_Write_SignalExt,   /* 247 */
    /* 504 */        Com_EB_Write_DET,   /* 248 */
    /* 505 */        Com_EB_Write_DET,   /* 249 */
    /* 506 */        Com_EB_Write_DET,   /* 250 */
    /* 507 */  Com_EB_Write_SignalExt,   /* 251 */
    /* 508 */        Com_EB_Write_DET,   /* 252 */
    /* 509 */        Com_EB_Write_DET,   /* 253 */
    /* 510 */        Com_EB_Write_DET,   /* 254 */
    /* 511 */  Com_EB_Write_SignalExt    /* 255 */
};

#define COM_STOP_SEC_CONST_UNSPECIFIED
#include <Com_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#define COM_START_SEC_CODE
#include <Com_MemMap.h>

#if (COM_CHECK_VALUE_SIZE == STD_ON)


/** \brief Com_EB_CheckValueSize8Bit - checks if the value fits into the
 * signal and if the size of the signal fits to its type.
 * If not, DET is called.
 * \param[in] Val - value which shall be checked
 * \param[in] size - number of bits reserved for the signal
 * \return
 * \retval None
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckValueSize8Bit
(
   uint8 Val,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr
)
{
   ComSignalBitSizeType size = Com_EB_WritePropPtr->ComSignalBase.ComBitSize;

#if (COM_PRECONDITION_ASSERT_ENABLED == STD_ON)
   /* CHECK: NOPARSE */
   if (!(size <= 8U))
   {
       COM_PRECONDITION_ASSERT((size <= 8U), COM_INTERNAL_API_ID);
   }
   /* Only do this check if the size isn't 8, as in that case
    * - shifting doesn't work (undefined behavior)
    * - there are no more significant bits left to test */
   else
   /* CHECK: PARSE */
#endif
   {
      if (COM_GET_TX_SIGNAL_SIGNAL_TYPE_SIGN(Com_EB_WritePropPtr) == COM_UNSIGNED) /* unsigned */
      {
         /* Unsigned; there shall be no more significant bits set unlike the
          * most significant which gets into the Pdu. */

         if ((uint8) (Val >> size) != 0U)
         {
            COM_DET_REPORTERROR(COM_E_SIGNAL_TOO_WIDE, COM_INTERNAL_API_ID);
         }
      }
      else                   /* unsigned / signed */
      {
         /* Signed; all more significant bits in value shall be equal to the
          * most significant bit that will make it into the Pdu */

         /* Make a copy of value */
         uint8 valcopy = Val;

         /* Get the most significant bit of the value which will make
          * it into the PDU */
         uint8 MSB = (uint8)((uint8)(1U << (size - 1U)) & Val);

         /* Prepare for the check */
         if (MSB != 0U)
         {
            /* The MSB in the NWSig is 1 */
            /* Fill all lower bits, that is, all other bits which are in the
             * NWSig, too, with 1, too */
            valcopy |= (uint8) ((uint8) (~0U) >> (8U - size));
         }
         else
         {
            /* The MSB in the NWSig is 0 */
            /* Fill all lower bits, that is, all other bits which are in the
             * NWSig, too, with 0, too */
            valcopy = (uint8)(valcopy & ((uint8)(0xFFU << size)));
         }

         /* Do magic.
          * If the "MSB" is 1, the number resulting from un-packing the
          * network signal on the receiver side is negative.
          * If the number fits into the network signal, all more significant
          * bits must also be 1. As we made the less significant bits 1,
          * too, the inverted valcopy must be 0.
          * If the "MSB" is 0, the number resulting from un-packing the
          * network signal on the receiver side is positive.
          * If the number fits into the network signal, all more significant
          * bits must also be 0. As we made the less significant bits 0,
          * too, valcopy must be zero. */

         {
            /* calculate the complement of Valcopy */
            uint8 const compValcopy = (uint8)(~valcopy);

            if (((MSB != 0U) && (compValcopy != 0U)) || ((MSB == 0U) && (valcopy != 0U)))
            {
               COM_DET_REPORTERROR(COM_E_SIGNAL_TOO_WIDE, COM_INTERNAL_API_ID);
            }
         }

      }                      /* end if (signed) */
   }

}                               /* Com_EB_CheckValueSize8Bit */


/** \brief Com_EB_CheckValueSize16Bit - checks if the value fits into the
 * signal and if the size of the signal fits to its type.
 * If not, DET is called.
 * \param[in] Val - value which shall be checked
 * \param[in] size - number of bits reserved for the signal
 * \return
 * \retval None
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckValueSize16Bit
(
   uint16 Val,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr
)
{
   ComSignalBitSizeType size = Com_EB_WritePropPtr->ComSignalBase.ComBitSize;

#if (COM_PRECONDITION_ASSERT_ENABLED == STD_ON)
   /* CHECK: NOPARSE */
   if (!(size <= 16U))
   {
       COM_PRECONDITION_ASSERT((size <= 16U), COM_INTERNAL_API_ID);
   }
   /* Only do this check if the size isn't 16, as in that case
    * - shifting doesn't work (undefined behavior)
    * - there are no more significant bits left to test */
   else
   /* CHECK: PARSE */
#endif
   {
      if (COM_GET_TX_SIGNAL_SIGNAL_TYPE_SIGN(Com_EB_WritePropPtr) == COM_UNSIGNED) /* unsigned */
      {
         /* Unsigned; there shall be no more significant bits set unlike the
          * most significant which gets into the Pdu. */

         if ((uint16) (Val >> size) != 0U)
         {
            COM_DET_REPORTERROR(COM_E_SIGNAL_TOO_WIDE, COM_INTERNAL_API_ID);
         }
      }
      else                   /* unsigned / signed */
      {
         /* Signed; all more significant bits in value shall be equal to the
          * most significant bit that will make it into the Pdu */

         /* Make a copy of value */
         uint16 valcopy = Val;

         /* Get the most significant bit of the value which will make
          * it into the PDU */
         uint16 MSB = (uint16)(((uint16)((uint16)1U << (size - 1U))) & Val);

         /* Prepare for the check */
         if (MSB != 0U)
         {
            /* The MSB in the NWSig is 1 */
            /* Fill all lower bits, that is, all other bits which are in the
             * NWSig, too, with 1, too */
            valcopy |= (uint16) ( (uint16)(~((uint16)0U)) >> (16U - size));
         }
         else
         {
            /* The MSB in the NWSig is 0 */
            /* Fill all lower bits, that is, all other bits which are in the
             * NWSig, too, with 0, too */
            valcopy = (uint16)(valcopy & ((uint16)(0xFFFFU << size)));
         }

         /* Do magic.
          * If the "MSB" is 1, the number resulting from un-packing the
          * network signal on the receiver side is negative.
          * If the number fits into the network signal, all more significant
          * bits must also be 1. As we made the less significant bits 1,
          * too, the inverted valcopy must be 0.
          * If the "MSB" is 0, the number resulting from un-packing the
          * network signal on the receiver side is positive.
          * If the number fits into the network signal, all more significant
          * bits must also be 0. As we made the less significant bits 0,
          * too, valcopy must be zero. */

         {
            /* calculate the complement of Valcopy */
            uint16 const compValcopy = (uint16)(~valcopy);

            if (((MSB != 0U) && (compValcopy != 0U)) || ((MSB == 0U) && (valcopy != 0U)))
            {
               COM_DET_REPORTERROR(COM_E_SIGNAL_TOO_WIDE, COM_INTERNAL_API_ID);
            }
         }

      }                      /* end if (signed) */
   }

}                               /* Com_EB_CheckValueSize16Bit */


/** \brief Com_EB_CheckValueSize32Bit - checks if the value fits into the
 * signal and if the size of the signal fits to its type.
 * If not, DET is called.
 * \param[in] Val - value which shall be checked
 * \param[in] size - number of bits reserved for the signal
 * \return
 * \retval None
 */
_COM_STATIC_ FUNC(void, COM_CODE) Com_EB_CheckValueSize32Bit
(
   uint32 Val,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr
)
{
   ComSignalBitSizeType size = Com_EB_WritePropPtr->ComSignalBase.ComBitSize;

#if (COM_PRECONDITION_ASSERT_ENABLED == STD_ON)
   /* CHECK: NOPARSE */
   if (!(size <= 32U))
   {
       COM_PRECONDITION_ASSERT((size <= 32U), COM_INTERNAL_API_ID);
   }
   /* Only do this check if the size isn't 32, as in that case
    * - shifting doesn't work (undefined behavior)
    * - there are no more significant bits left to test */
   else
   /* CHECK: PARSE */
#endif
   {
      if (COM_GET_TX_SIGNAL_SIGNAL_TYPE_SIGN(Com_EB_WritePropPtr) == COM_UNSIGNED) /* unsigned */
      {
         /* Unsigned; there shall be no more significant bits set unlike the
          * most significant which gets into the Pdu. */

         if ((uint32) (Val >> size) != 0UL)
         {
            COM_DET_REPORTERROR(COM_E_SIGNAL_TOO_WIDE, COM_INTERNAL_API_ID);
         }
      }
      else                   /* unsigned / signed */
      {
         /* Signed; all more significant bits in value shall be equal to the
          * most significant bit that will make it into the Pdu */

         /* Make a copy of value */
         uint32 valcopy = Val;

         /* Get the most significant bit of the value which will make
          * it into the PDU */
         uint32 MSB = (uint32)((1UL << (size - 1U)) & Val);

         /* Prepare for the check */
         if (MSB != 0U)
         {
            /* The MSB in the NWSig is 1 */
            /* Fill all lower bits, that is, all other bits which are in the
             * NWSig, too, with 1, too */
            valcopy |= (uint32) ((uint32) (~0UL) >> (32U - size));
         }
         else
         {
            /* The MSB in the NWSig is 0 */
            /* Fill all lower bits, that is, all other bits which are in the
             * NWSig, too, with 0, too */
            valcopy &= (uint32)((~0UL) << (size));
         }

         /* Do magic.
          * If the "MSB" is 1, the number resulting from un-packing the
          * network signal on the receiver side is negative.
          * If the number fits into the network signal, all more significant
          * bits must also be 1. As we made the less significant bits 1,
          * too, the inverted valcopy must be 0.
          * If the "MSB" is 0, the number resulting from un-packing the
          * network signal on the receiver side is positive.
          * If the number fits into the network signal, all more significant
          * bits must also be 0. As we made the less significant bits 0,
          * too, valcopy must be zero. */

         {
            /* calculate the complement of Valcopy */
            uint32 const compValcopy = (uint32)(~valcopy);

            if (((MSB != 0U) && (compValcopy != 0U)) || ((MSB == 0U) && (valcopy != 0U)))
            {
               COM_DET_REPORTERROR(COM_E_SIGNAL_TOO_WIDE, COM_INTERNAL_API_ID);
            }
         }

      }                      /* end if (signed) */
   }                         /* end if (size < 32U) */

}                               /* Com_EB_CheckValueSize32Bit */

#endif /* (COM_CHECK_VALUE_SIZE == STD_ON) */


#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;





   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));
   }



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;





   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);





   /* use all bits available in the first byte */
   ((dataPtr[pos])) = (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask)));

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos + 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      ((dataPtr[pos])) = (((uint8) (0xFFU & Val)));

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos + 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_Write_32_SX_DL_DUB_DOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit + 32U;

    Com_EB_Write_32_SX_DL_DUB_DOC_E1(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );
  }
  else
  {
    Com_EB_Write_32_SX_DL_DUB_DOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

  TS_PARAM_UNUSED(IntDataPtr);

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;





   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E1(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      ((dataPtr[pos])) = ((U8Ptr[ii]));
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_DL_DUB_DOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint32 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;


   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) DataPtr_Start =
       &dataPtr[Com_EB_WritePropPtr->ComSignalPositionInBit];


   while (ii < IntDataPtr->arraySize)
   {
      ((DataPtr_Start[ii])) = ((U8Ptr[ii]));
      ii++;
   }

} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;





   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));
   }



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;





   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);





   /* use all bits available in the first byte */
   ((dataPtr[pos])) = (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask)));

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos - 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      ((dataPtr[pos])) = (((uint8) (0xFFU & Val)));

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos - 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_Write_32_SX_DL_DUB_DOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit - 32U;

    Com_EB_Write_32_SX_DL_DUB_DOC_E2(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );
  }
  else
  {
    Com_EB_Write_32_SX_DL_DUB_DOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

  TS_PARAM_UNUSED(IntDataPtr);

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;





   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E2(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      ((dataPtr[pos])) = ((U8Ptr[ii]));
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;





   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);
   }



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;





   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);





   /* use all bits available in the first byte */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask))), valueChanged);

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos + 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (0xFFU & Val))), valueChanged);

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos + 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_Write_32_SX_DL_DUB_EOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit + 32U;

    Com_EB_Write_32_SX_DL_DUB_EOC_E1(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );
  }
  else
  {
    Com_EB_Write_32_SX_DL_DUB_EOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;





   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E1(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((U8Ptr[ii])), valueChanged);
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_DL_DUB_EOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint32 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) DataPtr_Start =
       &dataPtr[Com_EB_WritePropPtr->ComSignalPositionInBit];


   while (ii < IntDataPtr->arraySize)
   {
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((DataPtr_Start[ii])), ((U8Ptr[ii])), valueChanged);
      ii++;
   }


   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;

} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;





   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);
   }



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;





   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);





   /* use all bits available in the first byte */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask))), valueChanged);

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos - 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (0xFFU & Val))), valueChanged);

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos - 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_Write_32_SX_DL_DUB_EOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit - 32U;

    Com_EB_Write_32_SX_DL_DUB_EOC_E2(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );
  }
  else
  {
    Com_EB_Write_32_SX_DL_DUB_EOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;





   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E2(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((U8Ptr[ii])), valueChanged);
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));
   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));
   }



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* use all bits available in the first byte */
   ((dataPtr[pos])) = (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask)));

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos + 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      ((dataPtr[pos])) = (((uint8) (0xFFU & Val)));

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos + 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif


#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_Write_32_SX_DL_EUB_DOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit + 32U;

    Com_EB_Write_32_SX_DL_EUB_DOC_E1(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );
  }
  else
  {
    Com_EB_Write_32_SX_DL_EUB_DOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E1(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      ((dataPtr[pos])) = ((U8Ptr[ii]));
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_DL_EUB_DOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{

   uint32 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) DataPtr_Start =
       &dataPtr[Com_EB_WritePropPtr->ComSignalPositionInBit];

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   while (ii < IntDataPtr->arraySize)
   {
      ((DataPtr_Start[ii])) = ((U8Ptr[ii]));
      ii++;
   }

} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));
   }



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* use all bits available in the first byte */
   ((dataPtr[pos])) = (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask)));

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos - 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      ((dataPtr[pos])) = (((uint8) (0xFFU & Val)));

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos - 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif


#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_Write_32_SX_DL_EUB_DOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit - 32U;

    Com_EB_Write_32_SX_DL_EUB_DOC_E2(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );
  }
  else
  {
    Com_EB_Write_32_SX_DL_EUB_DOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E2(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      ((dataPtr[pos])) = ((U8Ptr[ii]));
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));



   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);
   }



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* use all bits available in the first byte */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask))), valueChanged);

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos + 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (0xFFU & Val))), valueChanged);

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos + 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_Write_32_SX_DL_EUB_EOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit + 32U;

    Com_EB_Write_32_SX_DL_EUB_EOC_E1(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );
  }
  else
  {
    Com_EB_Write_32_SX_DL_EUB_EOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E1(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((U8Ptr[ii])), valueChanged);
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_DL_EUB_EOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{

   uint8 valueChanged = FALSE;

   uint32 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) DataPtr_Start =
       &dataPtr[Com_EB_WritePropPtr->ComSignalPositionInBit];


   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   while (ii < IntDataPtr->arraySize)
   {
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((DataPtr_Start[ii])), ((U8Ptr[ii])), valueChanged);
      ii++;
   }


   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;

} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_DL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);
   }



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_DL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_DL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* use all bits available in the first byte */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask))), valueChanged);

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos - 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (0xFFU & Val))), valueChanged);

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos - 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_DL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_Write_32_SX_DL_EUB_EOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit - 32U;

    Com_EB_Write_32_SX_DL_EUB_EOC_E2(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );
  }
  else
  {
    Com_EB_Write_32_SX_DL_EUB_EOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_DL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;



   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E2(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((U8Ptr[ii])), valueChanged);
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));



   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if (COM_TX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if (COM_TX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if (COM_TX_LITTLEENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   /* use all bits available in the first byte */
   ((dataPtr[pos])) = (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask)));

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos + 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      ((dataPtr[pos])) = (((uint8) (0xFFU & Val)));

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos + 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

    Com_EB_Write_32_SX_DL_DUB_DOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit + 32U;

    Com_EB_Write_32_SX_DL_DUB_DOC_E1(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
  }
  else
  {
    Com_EB_Write_32_SX_EL_DUB_DOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

  TS_PARAM_UNUSED(IntDataPtr);

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_DUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E1(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      ((dataPtr[pos])) = ((U8Ptr[ii]));
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_EL_DUB_DOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{

   uint32 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) DataPtr_Start =
       &dataPtr[Com_EB_WritePropPtr->ComSignalPositionInBit];

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   while (ii < IntDataPtr->arraySize)
   {
      ((DataPtr_Start[ii])) = ((U8Ptr[ii]));
      ii++;
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);

} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if (COM_TX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if (COM_TX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if (COM_TX_BIGENDIAN_ENABLE == STD_ON)
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   /* use all bits available in the first byte */
   ((dataPtr[pos])) = (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask)));

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos - 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      ((dataPtr[pos])) = (((uint8) (0xFFU & Val)));

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos - 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

    Com_EB_Write_32_SX_DL_DUB_DOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit - 32U;

    Com_EB_Write_32_SX_DL_DUB_DOC_E2(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
  }
  else
  {
    Com_EB_Write_32_SX_EL_DUB_DOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

  TS_PARAM_UNUSED(IntDataPtr);

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_DUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E2(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      ((dataPtr[pos])) = ((U8Ptr[ii]));
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   /* use all bits available in the first byte */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask))), valueChanged);

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos + 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (0xFFU & Val))), valueChanged);

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos + 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif


#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

    Com_EB_Write_32_SX_DL_DUB_EOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit + 32U;

    Com_EB_Write_32_SX_DL_DUB_EOC_E1(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
  }
  else
  {
    Com_EB_Write_32_SX_EL_DUB_EOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_DUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E1(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((U8Ptr[ii])), valueChanged);
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_EL_DUB_EOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{

   uint8 valueChanged = FALSE;

   uint32 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) DataPtr_Start =
       &dataPtr[Com_EB_WritePropPtr->ComSignalPositionInBit];

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   while (ii < IntDataPtr->arraySize)
   {
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((DataPtr_Start[ii])), ((U8Ptr[ii])), valueChanged);
      ii++;
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;

} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   /* use all bits available in the first byte */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask))), valueChanged);

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos - 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (0xFFU & Val))), valueChanged);

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos - 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif


#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

    Com_EB_Write_32_SX_DL_DUB_EOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit - 32U;

    Com_EB_Write_32_SX_DL_DUB_EOC_E2(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
  }
  else
  {
    Com_EB_Write_32_SX_EL_DUB_EOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_DUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();



   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E2(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((U8Ptr[ii])), valueChanged);
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* use all bits available in the first byte */
   ((dataPtr[pos])) = (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask)));

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos + 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      ((dataPtr[pos])) = (((uint8) (0xFFU & Val)));

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos + 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif


#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WritePropPtr for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

    Com_EB_Write_32_SX_DL_EUB_DOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WritePropPtr for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit + 32U;

    Com_EB_Write_32_SX_DL_EUB_DOC_E1(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
  }
  else
  {
    Com_EB_Write_32_SX_EL_EUB_DOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

  TS_PARAM_UNUSED(IntDataPtr);

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_EUB_DOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E1(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      ((dataPtr[pos])) = ((U8Ptr[ii]));
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_EL_EUB_DOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{

   uint32 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) DataPtr_Start =
       &dataPtr[Com_EB_WritePropPtr->ComSignalPositionInBit];

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   while (ii < IntDataPtr->arraySize)
   {
      ((DataPtr_Start[ii])) = ((U8Ptr[ii]));
      ii++;
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);

} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask))));
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{


   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* use all bits available in the first byte */
   ((dataPtr[pos])) = (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask)));

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos - 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      ((dataPtr[pos])) = (((uint8) (0xFFU & Val)));

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos - 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      ((dataPtr[pos])) = ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask))));
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

    Com_EB_Write_32_SX_DL_EUB_DOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit - 32U;

    Com_EB_Write_32_SX_DL_EUB_DOC_E2(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
  }
  else
  {
    Com_EB_Write_32_SX_EL_EUB_DOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

  TS_PARAM_UNUSED(IntDataPtr);

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_EUB_DOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{


   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E2(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      ((dataPtr[pos])) = ((U8Ptr[ii]));
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos + 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* use all bits available in the first byte */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask))), valueChanged);

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos + 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (0xFFU & Val))), valueChanged);

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos + 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_LITTLEENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

    Com_EB_Write_32_SX_DL_EUB_EOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit + 32U;

    Com_EB_Write_32_SX_DL_EUB_EOC_E1(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
  }
  else
  {
    Com_EB_Write_32_SX_EL_EUB_EOC_E1(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_EUB_EOC_E1
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E1(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((U8Ptr[ii])), valueChanged);
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

static FUNC(void, COM_CODE) Com_EB_Write_AY_S1_EL_EUB_EOC_E3
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{

   uint8 valueChanged = FALSE;

   uint32 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) DataPtr_Start =
       &dataPtr[Com_EB_WritePropPtr->ComSignalPositionInBit];

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   while (ii < IntDataPtr->arraySize)
   {
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((DataPtr_Start[ii])), ((U8Ptr[ii])), valueChanged);
      ii++;
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;

} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_08_SX_EL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 Val = (uint16)((uint8) * (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint16 mask = 0xFFU;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (8U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);

   if (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8)(uint16)(~mask)) | (uint8) (Val & mask)))), valueChanged);
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_08_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_16_SX_EL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32)((uint16) * (P2CONST(uint16, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr);

   uint32 mask = (uint32)0xFFFFUL;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE16(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* reduce mask to the size of the signal */
   mask = mask >> (16U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;
   Val = Val << col;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* mask out the bits not to be overwritten and write them */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);

   while (mask > 255U)
   {
      Val = Val >> 8U;
      mask = mask >> 8U;
      pos = pos - 1;

      /* mask out the bits not to be overwritten and write them */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_16_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_32_SX_EL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
   uint8 valueChanged = FALSE;

   uint32 Val = (uint32) * (P2CONST(uint32, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   uint8 putvalue = (uint8) Val;
   uint32 mask = (uint32) 0xFFFFFFFFUL;
   uint8 space = 8U;
   sint8 ssize;

   /* Starting position of the signals in bits divided by 8 => starting position in byte */
   ComSignalDescBitType row = (Com_EB_WritePropPtr->ComSignalPositionInBit / 8U);
   uint8 col = (uint8)Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   /* Signal position in bit has a maximum value of 2031. */
   sint16 pos = (sint16) row;



   COM_EB_CHECKRANGE32(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   /* in the first byte, this much space can be used */
   space = space - col;

   /* reduce mask to the size of the signal */
   mask = mask >> (32U - Com_EB_WritePropPtr->ComSignalBase.ComBitSize);

   /* shift LSB of mask to LSB position of the signal in the Pdu */
   mask = mask << col;

   /* prepare an uint8 which contains the least significant bits

    * of value */
   putvalue = (uint8)(((uint8) putvalue) << col);

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   /* use all bits available in the first byte */
   COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (putvalue & mask))), valueChanged);

   {
      uint8 tmp_val = (uint8)Com_EB_WritePropPtr->ComSignalBase.ComBitSize - space;
      ssize = (sint8) tmp_val;
   }
   pos = pos - 1;
   Val = Val >> space;

   while (ssize > 7)
   {
      /* if there are whole bytes of data, copy the byte-wise */
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), (((uint8) (0xFFU & Val))), valueChanged);

      Val = Val >> 8U;
      ssize = ssize - (sint8)8;
      pos = pos - 1;
   }

   if (ssize > 0)
   {
      uint8 ssize_hlp = (uint8) ssize;
      /* mask out the bits not to be overwritten and write them */
      uint8 mask_u8 = 0xFFU >> ((uint8)(8U - ssize_hlp));
      mask = (uint32) mask_u8;
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((((uint8) (dataPtr[pos] & (uint8) (~mask)) | (uint8) (Val & mask)))), valueChanged);
   }

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_32_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

#if ( (COM_TX_BIGENDIAN_ENABLE == STD_ON) && \
      (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) )
static FUNC(void, COM_CODE) Com_EB_Write_64_SX_EL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr

)
{
  P2CONST(ComEmu64Type, AUTOMATIC, AUTOMATIC) SignalDataPtr_loc =
      (P2CONST(ComEmu64Type, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

  if(Com_EB_WritePropPtr->ComSignalBase.ComBitSize > 32U)
  {
    ComTxSignalType Com_EB_WriteProp_loc = *Com_EB_WritePropPtr;

    /* adapt Com_EB_WriteProp_loc for low 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = 32U;

    Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

    Com_EB_Write_32_SX_DL_EUB_EOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    /* adapt Com_EB_WriteProp_loc for high 32 bits */
    Com_EB_WriteProp_loc.ComSignalBase.ComBitSize = Com_EB_WritePropPtr->ComSignalBase.ComBitSize - 32U;
    Com_EB_WriteProp_loc.ComSignalPositionInBit = Com_EB_WritePropPtr->ComSignalPositionInBit - 32U;

    Com_EB_Write_32_SX_DL_EUB_EOC_E2(
        &SignalDataPtr_loc->HiWord,
        dataPtr,
        &Com_EB_WriteProp_loc,
        IntDataPtr
        );

    Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();
  }
  else
  {
    Com_EB_Write_32_SX_EL_EUB_EOC_E2(
        &SignalDataPtr_loc->LoWord,
        dataPtr,
        Com_EB_WritePropPtr,
        IntDataPtr
        );
  }

} /* Com_EB_Write_64_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */
#endif

static FUNC(void, COM_CODE) Com_EB_Write_AY_S0_EL_EUB_EOC_E2
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{
   uint8 valueChanged = FALSE;

   uint16 ii = 0U;

   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) U8Ptr =
         (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

   Com_EB_SetUB((&dataPtr[Com_EB_WritePropPtr->ComUpdateBitPositionInBit / 8U]),((Com_EB_WritePropPtr->ComUpdateBitPositionInBit % 8U)));

   do
   {
      uint16 pos = COM_EB_CONVERT_ENDIAN_POS_NET_E2(Com_EB_WritePropPtr->ComSignalPositionInBit, Com_EB_WritePropPtr->ComSignalBase.ComBitSize, ii);
      COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(((dataPtr[pos])), ((U8Ptr[ii])), valueChanged);
      ii++;
   }while (ii < (Com_EB_WritePropPtr->ComSignalBase.ComBitSize));

   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

   (IntDataPtr->trigger) = (IntDataPtr->trigger) | valueChanged;


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */

static FUNC(void, COM_CODE) Com_EB_Write_BO_DL_DUB_DOC
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{

   const uint8 Val = *(P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   const uint16 pos = Com_EB_WritePropPtr->ComSignalPositionInBit / 8U;

   const uint16 bit = Com_EB_WritePropPtr->ComSignalPositionInBit % 8U;

   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   if (Val == 0U)
   {
      /* clear the bit */
      dataPtr[pos] &= (uint8) ~((uint8)(UINT8_C(1) << (bit)));
   }
   else
   {
      /* set the bit */
      dataPtr[pos] |= (uint8)(UINT8_C(1) << (bit));
   }

   TS_PARAM_UNUSED(IntDataPtr);

}



static FUNC(void, COM_CODE) Com_EB_Write_A1_DL_DUB_DOC
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{

   const uint8 Val = *(P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   const uint16 pos = Com_EB_WritePropPtr->ComSignalPositionInBit;

   ((dataPtr[pos]) = (Val));

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */



static FUNC(void, COM_CODE) Com_EB_Write_BO_EL_DUB_DOC
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{

   const uint8 Val = *(P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   const uint16 pos = Com_EB_WritePropPtr->ComSignalPositionInBit / 8U;

   const uint16 bit = (uint16) ( (uint32)((uint32)(Com_EB_WritePropPtr->ComSignalPositionInBit) % 8U) );

   COM_EB_CHECKRANGE08(Val,Com_EB_WritePropPtr,IntDataPtr->RangeCheckPassed);

   if (Val == 0U)
   {
      /* clear the bit */
      COM_AtomicClearBit_8(&dataPtr[pos], bit);
   }
   else
   {
      /* set the bit */
      COM_AtomicSetBit_8(&dataPtr[pos], bit);
   }

   TS_PARAM_UNUSED(IntDataPtr);

}



static FUNC(void, COM_CODE) Com_EB_Write_A1_EL_DUB_DOC
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{

   const uint8 Val = *(P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)) SignalDataPtr;

   const uint16 pos = Com_EB_WritePropPtr->ComSignalPositionInBit;

   TS_AtomicAssign8(dataPtr[pos], Val);

   TS_PARAM_UNUSED(IntDataPtr);


} /* Com_EB_Write_AY_FUNC(COM_EB_SIGN,COM_EB_LOCK,COM_EB_UB,COM_EB_OC,COM_EB_ENDIAN) */



static FUNC(void, COM_CODE) Com_EB_Write_DET
(
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) dataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
)
{

#if (COM_DEV_ERROR_DETECT == STD_ON)

     COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COM_INTERNAL_API_ID);

#endif /* COM_DEV_ERROR_DETECT */

   IntDataPtr->SendSigRetval = COM_SERVICE_NOT_AVAILABLE;

   TS_PARAM_UNUSED(SignalDataPtr);
   TS_PARAM_UNUSED(dataPtr);
   TS_PARAM_UNUSED(Com_EB_WritePropPtr);
}


#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>

/*==================[end of file]===========================================*/
