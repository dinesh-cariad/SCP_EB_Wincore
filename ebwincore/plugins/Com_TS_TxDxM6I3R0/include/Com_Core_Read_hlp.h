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

#ifndef COM_CORE_READ_HLP_H_
#define COM_CORE_READ_HLP_H_

#include <TSAutosar.h>

/*==================[macros]================================================*/

#if (defined COM_EB_SWAP_U16)        /* To prevent double declaration */
#error COM_EB_SWAP_U16 is already defined
#endif /* (defined COM_EB_SWAP_U16) */

#define COM_EB_SWAP_U16(val) (((uint16)(((uint16)(val) & (uint16)0xFF00U) >> 8U)) | \
         ((uint16)(((uint16)(val) & (uint16)0x00FFU) << 8U)))

#if (defined COM_EB_SWAP_U32)        /* To prevent double declaration */
#error COM_EB_SWAP_U32 is already defined
#endif /* (defined COM_EB_SWAP_U32) */

#define COM_EB_SWAP_U32(val) ((((uint32)(val) & 0xFF000000U) >> 24U) | \
         (((uint32)(val) & 0x00FF0000U) >> 8U) | \
         (((uint32)(val) & 0x0000FF00U) << 8U) | \
         (((uint32)(val) & 0x000000FFU) << 24U))

#if (defined COM_EB_CONVERT_ENDIAN_RX_POS_NET_E1)        /* To prevent double declaration */
#error COM_EB_CONVERT_ENDIAN_RX_POS_NET_E1 is already defined
#endif /* (defined COM_EB_CONVERT_ENDIAN_RX_POS_NET_E1) */

#if (defined COM_EB_CONVERT_ENDIAN_RX_POS_NET_E2)        /* To prevent double declaration */
#error COM_EB_CONVERT_ENDIAN_RX_POS_NET_E2 is already defined
#endif /* (defined COM_EB_CONVERT_ENDIAN_RX_POS_NET_E2) */

#if (defined COM_EB_CONVERT_TO_E1_U16)        /* To prevent double declaration */
#error COM_EB_CONVERT_TO_E1_U16 is already defined
#endif /* (defined COM_EB_CONVERT_TO_E1_U16) */

#if (defined COM_EB_CONVERT_TO_E1_U32)        /* To prevent double declaration */
#error COM_EB_CONVERT_TO_E1_U32 is already defined
#endif /* (defined COM_EB_CONVERT_TO_E1_U32) */

#if (defined COM_EB_CONVERT_TO_E2_U16)        /* To prevent double declaration */
#error COM_EB_CONVERT_TO_E2_U16 is already defined
#endif /* (defined COM_EB_CONVERT_TO_E2_U16) */

#if (defined COM_EB_CONVERT_TO_E2_U32)        /* To prevent double declaration */
#error COM_EB_CONVERT_TO_E2_U32 is already defined
#endif /* (defined COM_EB_CONVERT_TO_E2_U32) */

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#define COM_EB_CONVERT_ENDIAN_RX_POS_NET_E1(bytePos, size, offset) ((bytePos) + (offset))
#define COM_EB_CONVERT_ENDIAN_RX_POS_NET_E2(bytePos, size, offset) (((bytePos) + ((size) - 1U)) - (offset))
#define COM_EB_CONVERT_TO_E1_U16(val) (val)
#define COM_EB_CONVERT_TO_E1_U32(val) (val)
#define COM_EB_CONVERT_TO_E2_U16(val) COM_EB_SWAP_U16(val)
#define COM_EB_CONVERT_TO_E2_U32(val) COM_EB_SWAP_U32(val)
#else
#define COM_EB_CONVERT_TO_E1_U16(val) COM_EB_SWAP_U16(val)
#define COM_EB_CONVERT_TO_E1_U32(val) COM_EB_SWAP_U32(val)
#define COM_EB_CONVERT_TO_E2_U16(val) (val)
#define COM_EB_CONVERT_TO_E2_U32(val) (val)
#define COM_EB_CONVERT_ENDIAN_RX_POS_NET_E1(bytePos, size, offset) (((bytePos) + ((size) - 1U)) - (offset))
#define COM_EB_CONVERT_ENDIAN_RX_POS_NET_E2(bytePos, size, offset) ((bytePos) + (offset))
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* COM_CORE_READ_HLP_H_ */
