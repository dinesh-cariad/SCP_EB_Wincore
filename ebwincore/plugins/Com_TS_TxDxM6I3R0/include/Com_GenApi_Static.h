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
#ifndef COM_GENAPI_STATIC_H_
#define COM_GENAPI_STATIC_H_

/*==================[inclusions]============================================*/

#include <Com_Rules.h>

/*==================[macros]================================================*/

#if (defined COM_GENAPI_RXPDU_RXDMTIMEOUT)     /* To prevent double declaration */
#error COM_GENAPI_RXPDU_RXDMTIMEOUT already defined
#endif /* if (defined COM_GENAPI_RXPDU_RXDMTIMEOUT) */

/** \brief Define COM_GENAPI_RXPDU_RXDMTIMEOUT */
#define COM_GENAPI_RXPDU_RXDMTIMEOUT 2U

#if (defined COM_RXPDU_RXDMTIMEOUT)
#if (COM_GENAPI_RXPDU_RXDMTIMEOUT != COM_RXPDU_RXDMTIMEOUT)
#error COM_GENAPI_RXPDU_RXDMTIMEOUT != COM_RXPDU_RXDMTIMEOUT
#endif /* (COM_GENAPI_RXPDU_RXDMTIMEOUT != COM_RXPDU_RXDMTIMEOUT) */
#endif /* (defined COM_RXPDU_RXDMTIMEOUT) */

#if (defined COM_EB_EXTRACT_BE_1)        /* To prevent double declaration */
#error COM_EB_EXTRACT_BE_1 is already defined
#endif /* (defined COM_EB_EXTRACT_BE_1) */
/*
 * extract a signal from a 1-byte buffer
 *    shift .. right-shift of last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_BE_1(offset, shift, pnData ) \
        ( ((pnData)[0] & (uint8)(0xffU >> (offset))) >> (shift)  \
        )

#if (defined COM_EB_EXTRACT_BE_2)        /* To prevent double declaration */
#error COM_EB_EXTRACT_BE_2 is already defined
#endif /* (defined COM_EB_EXTRACT_BE_2) */
/*
 * extract a signal from a 2-byte buffer
 *    shift .. right-shift of last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_BE_2(offset, shift, pnData ) \
        ( ((uint16)(pnData)[1] >> (shift))                                         \
        | (uint16)((((uint16)(pnData)[0] & (0xffU >> (offset))) << (8U-(shift))))  \
        )

#if (defined COM_EB_EXTRACT_BE_3)        /* To prevent double declaration */
#error COM_EB_EXTRACT_BE_3 is already defined
#endif /* (defined COM_EB_EXTRACT_BE_3) */
/*
 * extract a signal from a 3-byte buffer
 *    shift .. right-shift of last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_BE_3(offset, shift, pnData ) \
        ( ((uint32)(pnData)[2] >> (shift))                                     \
        | ((uint32)(pnData)[1] << (8U-(shift)))                                \
        | (((uint32)(pnData)[0] & ((uint32)0xffU>>(offset))) << (16U-(shift))) \
        )

#if (defined COM_EB_EXTRACT_BE_4)        /* To prevent double declaration */
#error COM_EB_EXTRACT_BE_4 is already defined
#endif /* (defined COM_EB_EXTRACT_BE_4) */
/*
 * extract a signal from a 4-byte buffer
 *    shift .. right-shift of last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_BE_4(offset, shift, pnData ) \
        ( ((uint32)(pnData)[3] >> (shift))                                      \
        | ((uint32)(pnData)[2] << (8U-(shift)))                                 \
        | ((uint32)(pnData)[1] << (16U-(shift)))                                \
        | (((uint32)(pnData)[0] &((uint32)0xffU >> (offset))) << (24U-(shift))) \
        )

#if (defined COM_EB_EXTRACT_BE_5)        /* To prevent double declaration */
#error COM_EB_EXTRACT_BE_5 is already defined
#endif /* (defined COM_EB_EXTRACT_BE_5) */
/*
 * extract a signal from a 5-byte buffer
 *    shift .. right-shift of last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_BE_5(offset, shift, pnData ) \
        ( ((uint32)(pnData)[4] >> (shift))                                       \
        | ((uint32)(pnData)[3] << (8U-(shift)))                                  \
        | ((uint32)(pnData)[2] << (16U-(shift)))                                 \
        | ((uint32)(pnData)[1] << (24U-(shift)))                                 \
        | (((uint32)(pnData)[0] & ((uint32)0xffU >> (offset))) << (32U-(shift))) \
        )

#if (defined COM_EB_EXTRACT_BE_6)        /* To prevent double declaration */
#error COM_EB_EXTRACT_BE_6 is already defined
#endif /* (defined COM_EB_EXTRACT_BE_6) */
/*
 * extract a signal from a 6-byte buffer
 *    shift .. right-shift of last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_BE_6(offset, shift, pnData ) \
        ( ((uint64)(pnData)[5] >> (shift))                                       \
        | ((uint64)(pnData)[4] << (8U-(shift)))                                  \
        | ((uint64)(pnData)[3] << (16U-(shift)))                                 \
        | ((uint64)(pnData)[2] << (24U-(shift)))                                 \
        | ((uint64)(pnData)[1] << (32U-(shift)))                                 \
        | (((uint64)(pnData)[0] & ((uint64)0xffU >> (offset))) << (40U-(shift))) \
        )

#if (defined COM_EB_EXTRACT_BE_7)        /* To prevent double declaration */
#error COM_EB_EXTRACT_BE_7 is already defined
#endif /* (defined COM_EB_EXTRACT_BE_7) */
/*
 * extract a signal from a 7-byte buffer
 *    shift .. right-shift of last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_BE_7(offset, shift, pnData ) \
        ( ((uint64)(pnData)[6] >> (shift))                                       \
        | ((uint64)(pnData)[5] << (8U-(shift)))                                  \
        | ((uint64)(pnData)[4] << (16U-(shift)))                                 \
        | ((uint64)(pnData)[3] << (24U-(shift)))                                 \
        | ((uint64)(pnData)[2] << (32U-(shift)))                                 \
        | ((uint64)(pnData)[1] << (40U-(shift)))                                 \
        | (((uint64)(pnData)[0] & ((uint64)0xffU >> (offset))) << (48U-(shift))) \
        )

#if (defined COM_EB_EXTRACT_BE_8)        /* To prevent double declaration */
#error COM_EB_EXTRACT_BE_8 is already defined
#endif /* (defined COM_EB_EXTRACT_BE_8) */
/*
 * extract a signal from a 8-byte buffer
 *    shift .. right-shift of last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_BE_8(offset, shift, pnData ) \
        ( ((uint64)(pnData)[7] >> (shift))                                       \
        | ((uint64)(pnData)[6] << (8U-(shift)))                                  \
        | ((uint64)(pnData)[5] << (16U-(shift)))                                 \
        | ((uint64)(pnData)[4] << (24U-(shift)))                                 \
        | ((uint64)(pnData)[3] << (32U-(shift)))                                 \
        | ((uint64)(pnData)[2] << (40U-(shift)))                                 \
        | ((uint64)(pnData)[1] << (48U-(shift)))                                 \
        | (((uint64)(pnData)[0] & ((uint64)0xffU >> (offset))) << (56U-(shift))) \
        )

#if (defined COM_EB_EXTRACT_BE_INT_8)        /* To prevent double declaration */
#error COM_EB_EXTRACT_BE_INT_8 is already defined
#endif /* (defined COM_EB_EXTRACT_BE_INT_8) */
/*
 * extract a 8 bit signal from a 1-byte buffer
 *    shift .. right-shift of last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_BE_INT_8(offset, shift, pnData ) \
   COM_EB_EXTRACT_BE_1((offset), (shift), (pnData) )

#if (defined COM_EB_EXTRACT_BE_INT_16)        /* To prevent double declaration */
#error COM_EB_EXTRACT_BE_INT_16 is already defined
#endif /* (defined COM_EB_EXTRACT_BE_INT_16) */
/*
 * extract a 16 bit signal from a 2-byte buffer
 * NOTE: buffer has to be 2-byte aligned.
 *    shift .. right-shift of last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 15)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_BE_INT_16(offset, shift, pnData ) \
        ( (uint16)((*(P2VAR(uint16, AUTOMATIC, COM_VAR_CLEARED))(pnData)) \
              &(uint16)(0xffffU>>(offset)))>>(shift) \
        )

#if (defined COM_EB_EXTRACT_BE_INT_32)        /* To prevent double declaration */
#error COM_EB_EXTRACT_BE_INT_32 is already defined
#endif /* (defined COM_EB_EXTRACT_BE_INT_32) */
/*
 * extract a 16 bit signal from a 4-byte buffer
 * NOTE: buffer has to be 4-byte aligned.
 *    shift .. right-shift of last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 31)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_BE_INT_32(offset, shift, pnData )                       \
        ( (uint32)((*(P2VAR(uint32, AUTOMATIC, COM_VAR_CLEARED))(pnData))       \
              & (uint32)(0xffffffffU >> (offset))) >> (shift)                  \
        )

#if (defined COM_EB_EXTRACT_LE_1)        /* To prevent double declaration */
#error COM_EB_EXTRACT_LE_1 is already defined
#endif /* (defined COM_EB_EXTRACT_LE_1) */
/*
 * extract a signal from a 1-byte buffer
 *    mask .. mask for last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_LE_1(mask, offset, pnData )     \
        ( (((pnData)[0] & (uint8)(mask)) >> (offset))  \
        )

#if (defined COM_EB_EXTRACT_LE_2)        /* To prevent double declaration */
#error COM_EB_EXTRACT_LE_2 is already defined
#endif /* (defined COM_EB_EXTRACT_LE_2) */
/*
 * extract a signal from a 2-byte buffer
 *    mask .. mask for last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_LE_2(mask, offset, pnData )                             \
        ( ((uint16)(pnData)[0] >> (offset))                                    \
        | (uint16)((((uint16)(pnData)[1] & (uint32)(mask)) << (8U-(offset))))  \
        )

#if (defined COM_EB_EXTRACT_LE_3)        /* To prevent double declaration */
#error COM_EB_EXTRACT_LE_3 is already defined
#endif /* (defined COM_EB_EXTRACT_LE_3) */
/*
 * extract a signal from a 3-byte buffer
 *    mask .. mask for last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_LE_3(mask, offset, pnData )                \
        ( ((uint32)(pnData)[0]>>(offset))                         \
        | ((uint32)(pnData)[1]<<(8U-(offset)))                    \
        | (((uint32)(pnData)[2]&(uint32)(mask))<<(16U-(offset)))  \
        )

#if (defined COM_EB_EXTRACT_LE_4)        /* To prevent double declaration */
#error COM_EB_EXTRACT_LE_4 is already defined
#endif /* (defined COM_EB_EXTRACT_LE_4) */
/*
 * extract a signal from a 4-byte buffer
 *    mask .. mask for last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_LE_4(mask, offset, pnData )                    \
        ( ((uint32)(pnData)[0] >> (offset))                           \
        | ((uint32)(pnData)[1] << (8U-(offset)))                      \
        | ((uint32)(pnData)[2] << (16U-(offset)))                     \
        | (((uint32)(pnData)[3] & (uint32)(mask)) << (24U-(offset)))  \
        )

#if (defined COM_EB_EXTRACT_LE_5)        /* To prevent double declaration */
#error COM_EB_EXTRACT_LE_5 is already defined
#endif /* (defined COM_EB_EXTRACT_LE_5) */
/*
 * extract a signal from a 5-byte buffer
 *    mask .. mask for last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_LE_5(mask, offset, pnData )                    \
        ( ((uint32)(pnData)[0] >> (offset))                           \
        | ((uint32)(pnData)[1] << (8U-(offset)))                      \
        | ((uint32)(pnData)[2] << (16U-(offset)))                     \
        | ((uint32)(pnData)[3] << (24U-(offset)))                     \
        | (((uint32)(pnData)[4] & (uint32)(mask)) << (32U-(offset)))  \
        )

#if (defined COM_EB_EXTRACT_LE_6)        /* To prevent double declaration */
#error COM_EB_EXTRACT_LE_6 is already defined
#endif /* (defined COM_EB_EXTRACT_LE_6) */
/*
 * extract a signal from a 6-byte buffer
 *    mask .. mask for last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_LE_6(mask, offset, pnData )                    \
        ( ((uint64)(pnData)[0] >> (offset))                           \
        | ((uint64)(pnData)[1] << (8U-(offset)))                      \
        | ((uint64)(pnData)[2] << (16U-(offset)))                     \
        | ((uint64)(pnData)[3] << (24U-(offset)))                     \
        | ((uint64)(pnData)[4] << (32U-(offset)))                     \
        | (((uint64)(pnData)[5] & (uint64)(mask)) << (40U-(offset)))  \
        )

#if (defined COM_EB_EXTRACT_LE_7)        /* To prevent double declaration */
#error COM_EB_EXTRACT_LE_7 is already defined
#endif /* (defined COM_EB_EXTRACT_LE_7) */
/*
 * extract a signal from a 7-byte buffer
 *    mask .. mask for last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_LE_7(mask, offset, pnData )                    \
        ( ((uint64)(pnData)[0] >> (offset))                           \
        | ((uint64)(pnData)[1] << (8U-(offset)))                      \
        | ((uint64)(pnData)[2] << (16U-(offset)))                     \
        | ((uint64)(pnData)[3] << (24U-(offset)))                     \
        | ((uint64)(pnData)[4] << (32U-(offset)))                     \
        | ((uint64)(pnData)[5] << (40U-(offset)))                     \
        | (((uint64)(pnData)[6] & (uint64)(mask)) << (48U-(offset)))  \
        )

#if (defined COM_EB_EXTRACT_LE_8)        /* To prevent double declaration */
#error COM_EB_EXTRACT_LE_8 is already defined
#endif /* (defined COM_EB_EXTRACT_LE_8) */
/*
 * extract a signal from a 8-byte buffer
 *    mask .. mask for last byte
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_LE_8(mask, offset, pnData )                    \
        ( ((uint64)(pnData)[0] >> (offset))                           \
        | ((uint64)(pnData)[1] << (8U-(offset)))                      \
        | ((uint64)(pnData)[2] << (16U-(offset)))                     \
        | ((uint64)(pnData)[3] << (24U-(offset)))                     \
        | ((uint64)(pnData)[4] << (32U-(offset)))                     \
        | ((uint64)(pnData)[5] << (40U-(offset)))                     \
        | ((uint64)(pnData)[6] << (48U-(offset)))                     \
        | (((uint64)(pnData)[7] & (uint64)(mask)) << (56U-(offset)))  \
        )

#if (defined COM_EB_EXTRACT_LE_INT_8)        /* To prevent double declaration */
#error COM_EB_EXTRACT_LE_INT_8 is already defined
#endif /* (defined COM_EB_EXTRACT_LE_INT_8) */
/*
 * extract a signal from a 1-byte buffer
 *    mask .. mask for all bytes
 *    offset .. offset of first bit of signal in buffer (0 .. 7)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_LE_INT_8(mask, offset, pnData )  \
   COM_EB_EXTRACT_LE_1(mask, offset, pnData )

#if (defined COM_EB_EXTRACT_LE_INT_16)        /* To prevent double declaration */
#error COM_EB_EXTRACT_LE_INT_16 is already defined
#endif /* (defined COM_EB_EXTRACT_LE_INT_16) */
/*
 * extract a signal from a 2-byte buffer
 * NOTE: buffer has to be 2-byte aligned.
 *    mask .. mask for all bytes
 *    offset .. offset of first bit of signal in buffer (0 .. 15)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_LE_INT_16(mask, offset, pnData )                        \
        ( (uint16)((*(P2VAR(uint16, AUTOMATIC, COM_VAR_CLEARED))(pnData))       \
              & (uint16)(mask)) >> (offset)                                    \
        )

#if (defined COM_EB_EXTRACT_LE_INT_32)        /* To prevent double declaration */
#error COM_EB_EXTRACT_LE_INT_32 is already defined
#endif /* (defined COM_EB_EXTRACT_LE_INT_32) */
/*
 * extract a signal from a 4-byte buffer
 * NOTE: buffer has to be 4-byte aligned.
 *    mask .. mask for all bytes
 *    offset .. offset of first bit of signal in buffer (0 .. 31)
 *    pnData .. pointer to buffer
 */
#define COM_EB_EXTRACT_LE_INT_32(mask, offset, pnData )                        \
        ( (uint32)((*(P2VAR(uint32, AUTOMATIC, COM_VAR_CLEARED))(pnData))       \
              & (uint32)(mask)) >> (offset)                                    \
        )

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* COM_GENAPI_STATIC_H_ */

/*==================[end of file]===========================================*/

