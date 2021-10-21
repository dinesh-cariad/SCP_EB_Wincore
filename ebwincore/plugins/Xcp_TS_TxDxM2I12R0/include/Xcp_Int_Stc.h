#ifndef XCP_INT_STC_H
#define XCP_INT_STC_H
/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/
#include <ComStack_Types.h>
/*==================[macros]================================================*/

/*------------------[Bit Masks]----------------------------------------------*/

#if (defined XCP_RESOURCE_CAL_PAG_MASK)
#error XCP_RESOURCE_CAL_PAG_MASK already defined
#endif
/** \brief Definition of value to indicate availability of calibration and
 ** paging functionality.
 **/
#define XCP_RESOURCE_CAL_PAG_MASK                          0x01U

#if (defined XCP_RESOURCE_DAQ_MASK)
#error XCP_RESOURCE_DAQ_MASK already defined
#endif
/** \brief Definition of value to indicate availability of data acquisition
 ** functionality.
 **/
#define XCP_RESOURCE_DAQ_MASK                              0x04U

#if (defined XCP_RESOURCE_STIM_MASK)
#error XCP_RESOURCE_STIM_MASK already defined
#endif
/** \brief Definition of value to indicate availability of data stimulation
 ** functionality.
 **/
#define XCP_RESOURCE_STIM_MASK                             0x08U

#if (defined XCP_RESOURCE_PGM_MASK)
#error XCP_RESOURCE_PGM_MASK already defined
#endif
/** \brief Definition of value to indicate availability of programming
 ** functionality.
 **/
#define XCP_RESOURCE_PGM_MASK                              0x10U

#if (defined XCP_LITTLE_ENDIAN_MASK)
#error XCP_LITTLE_ENDIAN_MASK already defined
#endif
/** \brief Definition of value to indicate little endian byte order.
 **/
#define XCP_LITTLE_ENDIAN_MASK                             0x00U

#if (defined XCP_BIG_ENDIAN_MASK)
#error XCP_BIG_ENDIAN_MASK already defined
#endif
/** \brief Definition of value to indicate big endian byte order.
 **/
#define XCP_BIG_ENDIAN_MASK                                0x01U

#if (defined XCP_BYTE_AG_SIZE )
#error XCP_BYTE_AG_SIZE  already defined
#endif
/** \brief Definition of value to indicate one byte granularity size.
 **/
#define XCP_BYTE_AG_SIZE                                   1U

#if (defined XCP_WORD_AG_SIZE )
#error XCP_WORD_AG_SIZE  already defined
#endif
/** \brief Definition of value to indicate two byte granularity size.
 **/
#define XCP_WORD_AG_SIZE                                   2U


#if (defined XCP_DWORD_AG_SIZE )
#error XCP_DWORD_AG_SIZE  already defined
#endif
/** \brief Definition of value to indicate four byte granularity size.
 **/
#define XCP_DWORD_AG_SIZE                                  4U

#if (defined XCP_DLONG_AG_SIZE )
#error XCP_DLONG_AG_SIZE  already defined
#endif
/** \brief Definition of value to indicate eight byte granularity size.
 **/
#define XCP_DLONG_AG_SIZE                                  8U


#if (defined XCP_BYTE_AG_MASK )
#error XCP_BYTE_AG_MASK  already defined
#endif
/** \brief Definition of value to indicate Address Granularity of one byte.
 **/
#define XCP_BYTE_AG_MASK                                   0x00U

#if (defined XCP_MASTER_BLOCK_MODE_MASK)
#error XCP_MASTER_BLOCK_MODE_MASK already defined
#endif
/** \brief Definition of value to mask support for master block mode. */
#define XCP_MASTER_BLOCK_MODE_MASK                         0x01U

#if (defined XCP_NO_TIME_STAMP_TS_MASK)
#error XCP_NO_TIME_STAMP_TS_MASK already defined
#endif
/** \brief Definition of value to indicate no timeStamp.
 **/
#define XCP_NO_TIME_STAMP_TS_MASK                          0x00U

#if (defined XCP_ONE_BYTE_TS_MASK)
#error XCP_ONE_BYTE_TS_MASK already defined
#endif
/** \brief Definition of value to indicate timeStamp with size one byte.
 **/
#define XCP_ONE_BYTE_TS_MASK                               0x01U

#if (defined XCP_TWO_BYTE_TS_MASK)
#error XCP_TWO_BYTE_TS_MASK already defined
#endif
/** \brief Definition of value to indicate timeStamp with size two byte.
 **/
#define XCP_TWO_BYTE_TS_MASK                               0x02U

#if (defined XCP_FOUR_BYTE_TS_MASK)
#error XCP_FOUR_BYTE_TS_MASK already defined
#endif
/** \brief Definition of value to indicate timeStamp with size four byte.
 **/
#define XCP_FOUR_BYTE_TS_MASK                              0x04U

#if (defined XCP_TIMESTAMP_TYPE_MASK)
#error XCP_TIMESTAMP_TYPE_MASK already defined
#endif
/** \brief Definition of value to mask timestamp type informationin timestamp
 ** mode parameter of DAQ Processor.
 **/
#define XCP_TIMESTAMP_TYPE_MASK                            0x07U

#if (defined XCP_TIMESTAMP_FIXED_MASK)
#error XCP_TIMESTAMP_FIXED_MASK already defined
#endif
/** \brief Definition of value to mask information on fixed timestamp in
 ** timestamp mode parameter of DAQ Processor.
 **/
#define XCP_TIMESTAMP_FIXED_MASK                           0x08U

#if (defined XCP_TIMESTAMP_UNIT_1NS_MASK)
#error XCP_TIMESTAMP_UNIT_1NS_MASK already defined
#endif
/** \brief Definition of value to indicate timestamp unit ONE nanosecond.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIMESTAMP_UNIT_1NS_MASK                        0U

#if (defined XCP_TIMESTAMP_UNIT_10NS_MASK)
#error XCP_TIMESTAMP_UNIT_10NS_MASK already defined
#endif
/** \brief Definition of value to indicate timestamp unit TEN nanosecond.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIMESTAMP_UNIT_10NS_MASK                       1U

#if (defined XCP_TIMESTAMP_UNIT_100NS_MASK)
#error XCP_TIMESTAMP_UNIT_100NS_MASK already defined
#endif
/** \brief Definition of value to indicate timestamp unit HUNDRED nanosecond.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIMESTAMP_UNIT_100NS_MASK                      2U

#if (defined XCP_TIMESTAMP_UNIT_1US_MASK)
#error XCP_TIMESTAMP_UNIT_1US_MASK already defined
#endif
/** \brief Definition of value to indicate timestamp unit ONE microsecond.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIMESTAMP_UNIT_1US_MASK                        3U

#if (defined XCP_TIMESTAMP_UNIT_10US_MASK)
#error XCP_TIMESTAMP_UNIT_10US_MASK already defined
#endif
/** \brief Definition of value to indicate timestamp unit TEN microsecond.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIMESTAMP_UNIT_10US_MASK                       4U

#if (defined XCP_TIMESTAMP_UNIT_100US_MASK)
#error XCP_TIMESTAMP_UNIT_100US_MASK already defined
#endif
/** \brief Definition of value to indicate timestamp unit HUNDRED microsecond.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIMESTAMP_UNIT_100US_MASK                      5U

#if (defined XCP_TIMESTAMP_UNIT_1MS_MASK)
#error XCP_TIMESTAMP_UNIT_1MS_MASK already defined
#endif
/** \brief Definition of value to indicate timestamp unit ONE millisecond.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIMESTAMP_UNIT_1MS_MASK                        6U

#if (defined XCP_TIMESTAMP_UNIT_10MS_MASK)
#error XCP_TIMESTAMP_UNIT_10MS_MASK already defined
#endif
/** \brief Definition of value to indicate timestamp unit TEN millisecond.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIMESTAMP_UNIT_10MS_MASK                       7U

#if (defined XCP_TIMESTAMP_UNIT_100MS_MASK)
#error XCP_TIMESTAMP_UNIT_100MS_MASK already defined
#endif
/** \brief Definition of value to indicate timestamp unit HUNDRED millisecond.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIMESTAMP_UNIT_100MS_MASK                      8U

#if (defined XCP_TIMESTAMP_UNIT_1S_MASK)
#error XCP_TIMESTAMP_UNIT_1S_MASK already defined
#endif
/** \brief Definition of value to indicate timestamp unit ONE second.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIMESTAMP_UNIT_1S_MASK                         9U

#if (defined XCP_TIMESTAMP_UNIT_1PS_MASK)
#error XCP_TIMESTAMP_UNIT_1PS_MASK already defined
#endif
/** \brief Definition of value to indicate timestamp unit ONE picosecond.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIMESTAMP_UNIT_1PS_MASK                        10U

#if (defined XCP_TIMESTAMP_UNIT_10PS_MASK)
#error XCP_TIMESTAMP_UNIT_10PS_MASK already defined
#endif
/** \brief Definition of value to indicate timestamp unit TEN picosecond.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIMESTAMP_UNIT_10PS_MASK                       11U

#if (defined XCP_TIMESTAMP_UNIT_100PS_MASK)
#error XCP_TIMESTAMP_UNIT_100PS_MASK already defined
#endif
/** \brief Definition of value to indicate timestamp unit HUNDRED picosecond.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIMESTAMP_UNIT_100PS_MASK                      12U

#if (defined XCP_TIME_UNIT_DONT_CARE_MASK)
#error XCP_TIME_UNIT_DONT_CARE_MASK already defined
#endif
/** \brief Definition of value to indicate event channel time unit
 **        if Event ChannelTime Cycle = 0 (don't care value)
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIME_UNIT_DONT_CARE_MASK                       0U

#if (defined XCP_TIMESTAMP_UNIT_POS)
#error XCP_TIMESTAMP_UNIT_POS already defined
#endif
/** \brief Definition of value indicating the start bit position of the time
 ** stamp unit information in the DAQ processor timestamp mode parameter.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_TIMESTAMP_UNIT_POS                             4U

#if (defined XCP_DAQ_STATIC_MASK)
#error XCP_DAQ_STATIC_MASK already defined
#endif
/** \brief Definition of value indicating static configuration of non
 ** predefined DAQ Lists.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_DAQ_STATIC_MASK                                0U

#if (defined XCP_DAQ_DYNAMIC_MASK)
#error XCP_DAQ_DYNAMIC_MASK already defined
#endif
/** \brief Definition of value indicating dynamic configuration of non
 ** predefined DAQ Lists.
 **
 ** \note Macro definition is in conformance with ASAM Specification Part 2.
 **/
#define XCP_DAQ_DYNAMIC_MASK                                1U

#if (defined XCP_PRESCALER_MASK)
#error XCP_PRESCALER_MASK already defined
#endif
/** \brief Definition of value to mask support for Prescaler.
 **/
#define XCP_PRESCALER_MASK                                 0x02U


#if (defined XCP_TIMESTAMP_MASK)
#error XCP_TIMESTAMP_MASK already defined
#endif
/** \brief Definition of value to mask support for Timestamp.
 **/
#define XCP_TIMESTAMP_MASK                                 0x10U


#if (defined XCP_BIT_STIM_SUPPORTED_MASK)
#error XCP_BIT_STIM_SUPPORTED_MASK already defined
#endif
/** \brief Definition of value to mask support for BIT_STIM_SUPPORTED.
 **/
#define XCP_BIT_STIM_SUPPORTED_MASK                        0x08U

#if (defined XCP_BIT_STIM_SUPPORTED)
#error XCP_BIT_STIM_SUPPORTED already defined
#endif
/** \brief Switch, indicating whether the slave supports bitwise stimulation. */
#define XCP_BIT_STIM_SUPPORTED                XCP_BIT_STIM_SUPPORTED_MASK

#if (defined XCP_NO_OVERLOAD_INDICATION_OL_MASK)
#error XCP_NO_OVERLOAD_INDICATION_OL_MASK already defined
#endif
/** \brief Mask indicating no overload indication to the master.
 **/
#define XCP_NO_OVERLOAD_INDICATION_OL_MASK                 0x00U

#if (defined XCP_PID_OL_MASK)
#error XCP_PID_OL_MASK already defined
#endif
/** \brief Mask indicating overload indication type which uses most
 ** significant bit of the DTO packet identifier for overload indication to
 ** the master.
 **/
#define XCP_PID_OL_MASK                                    0x40U

#if (defined XCP_EVENT_OL_MASK)
#error XCP_EVENT_OL_MASK already defined
#endif
/** \brief Mask indicating overload indication type which uses event packet
 ** for overload indication to the master.
 **/
#define XCP_EVENT_OL_MASK                                  0x80U


#if (defined XCP_DEFAULT_OM_MASK)
#error XCP_DEFAULT_OM_MASK already defined
#endif
/** \brief Mask indicating optimization type which has no limit on ODT entry
 ** type */
#define XCP_DEFAULT_OM_MASK                                0U

#if (defined XCP_ODT_TYPE_16_OM_MASK)
#error XCP_ODT_TYPE_16_OM_MASK already defined
#endif
/** \brief Mask indicating optimization type which supports only ODT entries
 ** of same type within one ODT with the largest type being 16 bit. */
#define XCP_ODT_TYPE_16_OM_MASK                            1U

#if (defined XCP_ODT_TYPE_32_OM_MASK)
#error XCP_ODT_TYPE_32_OM_MASK already defined
#endif
/** \brief Mask indicating optimization type which supports only ODT entries
 ** of same type within one ODT with the largest type being 32 bit. */
#define XCP_ODT_TYPE_32_OM_MASK                            2U

#if (defined XCP_ODT_TYPE_64_OM_MASK)
#error XCP_ODT_TYPE_64_OM_MASK already defined
#endif
/** \brief Mask indicating optimization type which supports only ODT entries
 ** of same type within one ODT with the largest type being 64 bit. */
#define XCP_ODT_TYPE_64_OM_MASK                            3U

#if (defined XCP_ODT_TYPE_ALIGNMENT_OM_MASK)
#error XCP_ODT_TYPE_ALIGNMENT_OM_MASK already defined
#endif
/** \brief Mask indicating optimization type which supports all kind of data
 **  types arranged in alignment order within one ODT. */
#define XCP_ODT_TYPE_ALIGNMENT_OM_MASK                     4U

#if (defined XCP_MAX_ENTRY_SIZE_OM_MASK)
#error XCP_MAX_ENTRY_SIZE_OM_MASK already defined
#endif
/** \brief Mask indicating optimization type which supports only ODT entries
 **  of a fixed length defined by MAX_ODT_ENTRY_SIZE_DAQ and
 **  MAX_ODT_ENTRY_SIZE_STIM. */
#define XCP_MAX_ENTRY_SIZE_OM_MASK                         5U

#if (defined XCP_FREE_AE_MASK)
#error XCP_FREE_AE_MASK already defined
#endif
/** \brief Mask indicating address extension type which supports different
 ** address extension within one and the same ODT.
 **/
#define XCP_FREE_AE_MASK                                   0x00U

#if (defined XCP_ODT_AE_MASK)
#error XCP_ODT_AE_MASK already defined
#endif
/** \brief Mask indicating address extension type which supports same address
 ** extension for all entries within one ODT.
 **/
#define XCP_ODT_AE_MASK                                    0x10U

#if (defined XCP_DAQ_AE_MASK)
#error XCP_DAQ_AE_MASK already defined
#endif
/** \brief Mask indicating address extension type which supports same address
 ** extension for all entries within one DAQ.
 **/
#define XCP_DAQ_AE_MASK                                    0x30U

#if (defined XCP_ADDRESS_EXTENSION_MASK)
#error XCP_ADDRESS_EXTENSION_MASK already defined
#endif
/** \brief Address extension type mask.
 **/
#define XCP_ADDRESS_EXTENSION_MASK                         0x30U

#if (defined XCP_IDENTIFICATION_FIELD_MASK)
#error XCP_IDENTIFICATION_FIELD_MASK already defined
#endif
/** \brief Identification field type mask.
 **/
#define XCP_IDENTIFICATION_FIELD_MASK                      0xC0U

#if (defined XCP_SLAVE_BLOCK_MODE_MASK)
#error XCP_SLAVE_BLOCK_MODE_MASK already defined
#endif
/** \brief Definition of value to mask support for slave block mode.
 **
 ** \note Macro definition is in conformity with ASAM Specification Part 2.
 **/
#define XCP_SLAVE_BLOCK_MODE_MASK                          0x40U

#if (defined XCP_BIT_STORE_CAL_REQ)
#error XCP_BIT_STORE_CAL_REQ already defined
#endif
/** \brief Bit position to store request to save the calibration data. */
#define XCP_BIT_STORE_CAL_REQ                              0x00U

#if (defined XCP_BIT_STORE_DAQ_REQ)
#error XCP_BIT_STORE_DAQ_REQ already defined
#endif
/** \brief Bit position to set request to save DAQ list setup into non-volatile
 * memory regardless of the RESUME mode. The bit is used for the GET_STATUS'
 * current status parameter */
#define XCP_BIT_STORE_DAQ_REQ                             0x02U

#if (defined XCP_BIT_CLEAR_DAQ_REQ)
#error XCP_BIT_CLEAR_DAQ_REQ already defined
#endif
/** \brief Bit position to store request to clear all DAQ lists in non-volatile
 * memory. */
#define XCP_BIT_CLEAR_DAQ_REQ                             0x03U

#if (defined XCP_MASK_STORE_CAL_REQ)
#error XCP_MASK_STORE_CAL_REQ already defined
#endif
/** \brief Mask to set request to save the calibration data. */
#define XCP_MASK_STORE_CAL_REQ                             0x01U

#if (defined XCP_MASK_STORE_DAQ_REQ_NORESUME)
#error XCP_MASK_STORE_DAQ_REQ_NORESUME already defined
#endif
/** \brief Mask to set request to save DAQ list setup into non-volatile
 * memory (without RESUME)*/
#define XCP_MASK_STORE_DAQ_REQ_NORESUME                    0x02U

#if (defined XCP_MASK_STORE_DAQ_REQ_RESUME)
#error XCP_MASK_STORE_DAQ_REQ_RESUME already defined
#endif
/** \brief Mask to set request to save DAQ list setup into non-volatile
 * memory with RESUME enabled */
#define XCP_MASK_STORE_DAQ_REQ_RESUME                      0x04U

#if (defined XCP_MASK_CLEAR_DAQ_REQ)
#error XCP_MASK_CLEAR_DAQ_REQ already defined
#endif
/** \brief Mask to set request to clear all DAQ lists in non-volatile
 * memory. */
#define XCP_MASK_CLEAR_DAQ_REQ                             0x08U

#if (defined XCP_MASK_STORE_DAQ_REQ)
#error XCP_MASK_STORE_DAQ_REQ already defined
#endif
/** \brief Mask to set request to save DAQ list setup into non-volatile
 * memory regardless of the RESUME mode. The bit is used for the GET_STATUS'
 * current status parameter */
#define XCP_MASK_STORE_DAQ_REQ                             0x04U
#if (defined XCP_EVENT_CHANNEL_DIRECTION_DAQ)
#error XCP_EVENT_CHANNEL_DIRECTION_DAQ already defined
#endif
/** \brief Event channel supports only DAQ lists with DIRECTION = DAQ. */
#define XCP_EVENT_CHANNEL_DIRECTION_DAQ                    0x04U

#if (defined XCP_EVENT_CHANNEL_DIRECTION_STIM)
#error XCP_EVENT_CHANNEL_DIRECTION_STIM already defined
#endif
/** \brief Event channel supports only DAQ lists with DIRECTION = STIM. */
#define XCP_EVENT_CHANNEL_DIRECTION_STIM                   0x08U

#if (defined XCP_EVENT_CHANNEL_DIRECTION_DAQ_STIM)
#error XCP_EVENT_CHANNEL_DIRECTION_DAQ_STIM already defined
#endif
/** \brief Event channel supports only DAQ lists with DIRECTION = DAQ and STIM.
 */
#define XCP_EVENT_CHANNEL_DIRECTION_DAQ_STIM               0x0CU

#if (defined XCP_EVENT_CHANNEL_CONSISTENCY_ODT)
#error XCP_EVENT_CHANNEL_CONSISTENCY_ODT already defined
#endif
/** \brief ODT consistency. */
#define XCP_EVENT_CHANNEL_CONSISTENCY_ODT                  0x00U

#if (defined XCP_EVENT_CHANNEL_CONSISTENCY_DAQ)
#error XCP_EVENT_CHANNEL_CONSISTENCY_DAQ already defined
#endif
/** \brief DAQ consistency. */
#define XCP_EVENT_CHANNEL_CONSISTENCY_DAQ                  0x40U

#if (defined XCP_EVENT_CHANNEL_CONSISTENCY_EVENT)
#error XCP_EVENT_CHANNEL_CONSISTENCY_EVENT already defined
#endif
/** \brief Event consistency. */
#define XCP_EVENT_CHANNEL_CONSISTENCY_EVENT                0x80U

#if (defined XCP_EVENT_CHANNEL_CONSISTENCY_MASK)
#error XCP_EVENT_CHANNEL_CONSISTENCY_MASK already defined
#endif
/** \brief consistency Mask. */
#define XCP_EVENT_CHANNEL_CONSISTENCY_MASK                 0xC0U

#if (defined XCP_DAQ_LIST_DIRECTION_DAQ)
#error XCP_DAQ_LIST_DIRECTION_DAQ already defined
#endif
/** \brief DAQ List supports only DAQ lists with DIRECTION = DAQ. */
#define XCP_DAQ_LIST_DIRECTION_DAQ                         0x04U

#if (defined XCP_DAQ_LIST_DIRECTION_STIM)
#error XCP_DAQ_LIST_DIRECTION_STIM already defined
#endif
/** \brief DAQ List supports only DAQ lists with DIRECTION = STIM. */
#define XCP_DAQ_LIST_DIRECTION_STIM                        0x08U

#if (defined XCP_DAQ_LIST_DIRECTION_DAQ_STIM)
#error XCP_DAQ_LIST_DIRECTION_DAQ_STIM already defined
#endif
/** \brief DAQ List supports only DAQ lists with DIRECTION = DAQ and STIM. */
#define XCP_DAQ_LIST_DIRECTION_DAQ_STIM                    0x0CU

#if (defined XCP_DAQ_LIST_DIRECTION_MASK)
#error XCP_DAQ_LIST_DIRECTION_MASK already defined
#endif
/** \brief DAQ List property Mask. */
#define XCP_DAQ_LIST_DIRECTION_MASK                        0x0CU

#if (defined XCP_DAQ_LIST_PREDEFINED)
#error XCP_DAQ_LIST_PREDEFINED already defined
#endif
/** \brief Daq list is predefined. */
#define XCP_DAQ_LIST_PREDEFINED                            0x01U

#if (defined XCP_MASK_DAQLIST_ALTERNATING_MODE)
#error XCP_MASK_DAQLIST_ALTERNATING_MODE already defined
#endif
/** \brief Mast to set DAQ List alternating mode */
 #define XCP_MASK_DAQLIST_ALTERNATING_MODE                 0x01U

#if (defined XCP_MASK_DAQLIST_TIMESTAMPED_MODE)
#error XCP_MASK_DAQLIST_TIMESTAMPED_MODE already defined
#endif
/** \brief Mask to set DAQ List timestamped mode */
#define XCP_MASK_DAQLIST_TIMESTAMPED_MODE                  0x10U

#if (defined XCP_MASK_DAQLIST_PID_OFF_MODE)
#error XCP_MASK_DAQLIST_PID_OFF_MODE already defined
#endif
/** \brief Mask set DAQ List PID OFF mode */
#define XCP_MASK_DAQLIST_PID_OFF_MODE                      0x20U

#if (defined XCP_MASK_DAQLIST_DIRECTION)
#error XCP_MASK_DAQLIST_DIRECTION already defined
#endif
/** \brief Mask to check DAQ List direction */
#define XCP_MASK_DAQLIST_DIRECTION                         0x02U

#if (defined XCP_MASK_DAQLIST_DIRECTION_STIM)
#error XCP_MASK_DAQLIST_DIRECTION_STIM already defined
#endif
/** \brief Mask to set DAQ List direction to STIM */
#define XCP_MASK_DAQLIST_DIRECTION_STIM                    0x02U

#if (defined XCP_MASK_DAQLIST_DIRECTION_DAQ)
#error XCP_MASK_DAQLIST_DIRECTION_DAQ already defined
#endif
/** \brief Mask to set DAQ List direction to DAQ */
#define XCP_MASK_DAQLIST_DIRECTION_DAQ                     0x00U

#if (defined XCP_MASK_DAQLIST_SELECTED)
#error XCP_MASK_DAQLIST_SELECTED already defined
#endif
/** \brief Mask to Set DAQ List as selected */
#define XCP_MASK_DAQLIST_SELECTED                          0x04U

#if (defined XCP_MASK_DAQLIST_SELECTED_GDLM)
#error XCP_MASK_DAQLIST_SELECTED_GDLM already defined
#endif
/** \brief Mask to Set DAQ List as selected. The position is required by the
 * GET_DAQ_LIST_MODE command  */
#define XCP_MASK_DAQLIST_SELECTED_GDLM                     0x01U

#if (defined XCP_MASK_DAQLIST_RUNNING)
#error XCP_MASK_DAQLIST_RUNNING already defined
#endif
/** \brief Mask to Set DAQ List as running */
#define XCP_MASK_DAQLIST_RUNNING                           0x40U

#if (defined XCP_MASK_DAQLIST_RESUME)
#error XCP_MASK_DAQLIST_RESUME already defined
#endif
/** \brief Mask to Set DAQ List in resume mode. */
#define XCP_MASK_DAQLIST_RESUME                            0x80U

#if (defined XCP_MASK_DAQLIST_CONFIGURED)
#error XCP_MASK_DAQLIST_CONFIGURED already defined
#endif
/** \brief Mask to Set DAQ List as configured. */
#define XCP_MASK_DAQLIST_CONFIGURED                        0x02U

#if (defined XCP_MASK_DAQLIST_EVENT_FIXED)
#error XCP_MASK_DAQLIST_EVENT_FIXED already defined
#endif
/** \brief Mask for configuration EVENT_FIXED(Event Channel is fixed) */
#define XCP_MASK_DAQLIST_EVENT_FIXED                       0x02U

#if (defined XCP_MASK_EVENT_DIRECTION_STIM)
#error XCP_MASK_EVENT_DIRECTION_STIM already defined
#endif
/** \brief Mask to check whether Event channel direction is STIM */
#define XCP_MASK_EVENT_DIRECTION_STIM                      0x08U

#if (defined XCP_MASK_EVENT_DIRECTION_DAQ)
#error XCP_MASK_EVENT_DIRECTION_DAQ already defined
#endif
/** \brief Mask to check whether Event channel direction is DAQ */
#define XCP_MASK_EVENT_DIRECTION_DAQ                       0x04U

#if (defined XCP_MASK_DAQ_RUNNING)
#error XCP_MASK_DAQ_RUNNING already defined
#endif
/** \brief Mask to set XCP Slave status to indicate that at least one DAQ list
 *   has been started . */
#define XCP_MASK_DAQ_RUNNING                               0x40U

#if (defined XCP_MASK_RESUME_MODE)
#error XCP_MASK_RESUME_MODE already defined
#endif
/** \brief Mask to set XCP slave status to indicate that the slave is in RESUME
 *   mode. */
#define XCP_MASK_RESUME_MODE                               0x80U

#if (defined XCP_BIT_DAQ_RUNNING)
#error XCP_BIT_DAQ_RUNNING already defined
#endif
/** \brief Bit position to set XCP Slave status to indicate that at least one DAQ list
 *   has been started . */
#define XCP_BIT_DAQ_RUNNING                               0x6U

#if (defined XCP_BIT_RESUME_MODE)
#error XCP_BIT_RESUME_MODE already defined
#endif
/** \brief Bit position to set XCP slave status to indicate that the slave is in RESUME
 *   mode. */
#define XCP_BIT_RESUME_MODE                               0x7U

#if (defined XCP_RESUME_SUPPORTED_MASK)
#error XCP_RESUME_SUPPORTED_MASK already defined
#endif
/** \brief Definition of mask to check whether DAQ Resume mode is ON. */
#define XCP_RESUME_SUPPORTED_MASK                          0x04U

#if (defined XCP_MASK_DAQLIST_PID_MODE)
#error XCP_MASK_DAQLIST_PID_MODE already defined
#endif
/** \brief Mask to get the DAQ List PID mode */
#define XCP_MASK_DAQLIST_PID_MODE                          0x20U

#if (defined XCP_MASK_DAQLIST_PID_ON_MODE)
#error XCP_MASK_DAQLIST_PID_ON_MODE already defined
#endif
/** \brief Mask to check whether the DAQ List PID mode is ON */
#define XCP_MASK_DAQLIST_PID_ON_MODE                       0x00U

#if (defined XCP_IDENTIFICATION_TRANSFER_MODE_0_MASK)
#error XCP_IDENTIFICATION_TRANSFER_MODE_0_MASK already defined
#endif
/** \brief Identification transfer mode
 *
 * - TRANSFER_MODE = 0: The master device may upload the requested
 * identification using one or more UPLOAD commands.
 * - COMPRESSED_ENCRYPTED = 0, transferred data are not compressed or
 * encrypted.
 *
 * \note Present implementation supports identification type 1 */
#define XCP_IDENTIFICATION_TRANSFER_MODE_0_MASK            0x00U

#if (defined XCP_IDENTIFICATION_TRANSFER_MODE_1_MASK)
#error XCP_IDENTIFICATION_TRANSFER_MODE_1_MASK already defined
#endif
/** \brief Identification transfer mode
 *
 * - TRANSFER_MODE = 1: The identification is part of this response.
 * - COMPRESSED_ENCRYPTED = 0, transferred data are not compressed or
 * encrypted.
 *
 * \note Present implementation supports identification type 1 */
#define XCP_IDENTIFICATION_TRANSFER_MODE_1_MASK            0x01U

#if (defined XCP_MASK_DAQLIST_READY)
#error XCP_MASK_DAQLIST_READY already defined
#endif
/** \brief Mask to check DAQ List is ready to be processed (sampled/stimulated) */
#define XCP_MASK_DAQLIST_READY                             0x01U

#if (defined XCP_MASK_DAQLIST_SKIPOVCHECK)
#error XCP_MASK_DAQLIST_SKIPOVCHECK already defined
#endif
/** \brief Mask to decide if we check for event channel overload detected */
#define XCP_MASK_DAQLIST_SKIPOVCHECK                       0x04U

#if (defined XCP_MASK_DAQLIST_OVCHECKSKIPPED)
#error XCP_MASK_DAQLIST_OVCHECKSKIPPED already defined
#endif
/** \brief Mask to decide when to clear XCP_MASK_DAQLIST_SKIPOVCHECK flag */
#define XCP_MASK_DAQLIST_OVCHECKSKIPPED                    0x08U

#if (defined XCP_CMD_SET_GET_SEGMENT_MODE_FREEZE_MASK)
#error XCP_CMD_SET_GET_SEGMENT_MODE_FREEZE_MASK already defined
#endif
/** \brief SET_SEGMENT_MODE, GET_SEGMENT_MODE commands Mode parameter Freeze Mask. */
#define XCP_CMD_SET_GET_SEGMENT_MODE_FREEZE_MASK           0x01U

#if (defined XCP_RESOURCE_UNLOCKED)
#error XCP_RESOURCE_UNLOCKED already defined
#endif
/** \brief Mask to check whether a resource is unlocked */
#define XCP_RESOURCE_UNLOCKED                              0x00U

#if (defined XCP_GET_SLAVE_ID_IP_TRANSPORT_PROTOCOL_I_FLAG)
#error XCP_GET_SLAVE_ID_IP_TRANSPORT_PROTOCOL_I_FLAG already defined
#endif
/** \brief Flag for IP_TRANSPORT_PROTOCOL part 1 of STATUS response for
 **        GET_SLAVE_ID (on Ethernet) command. */
#define XCP_GET_SLAVE_ID_IP_TRANSPORT_PROTOCOL_I_FLAG      0x1U

#if (defined XCP_GET_SLAVE_ID_IP_TRANSPORT_PROTOCOL_II_FLAG)
#error XCP_GET_SLAVE_ID_IP_TRANSPORT_PROTOCOL_II_FLAG already defined
#endif
/** \brief Flag for IP_TRANSPORT_PROTOCOL part 2 of STATUS response for
 **        GET_SLAVE_ID (on Ethernet) command. */
#define XCP_GET_SLAVE_ID_IP_TRANSPORT_PROTOCOL_II_FLAG     0x2U

#if (defined XCP_GET_SLAVE_ID_SLV_AVAILABILITY_FLAG)
#error XCP_GET_SLAVE_ID_SLV_AVAILABILITY_FLAG already defined
#endif
/** \brief Flag for IP_VERSION of SLV_AVAILABILITY response for
 **        GET_SLAVE_ID (on Ethernet) command. */
#define XCP_GET_SLAVE_ID_SLV_AVAILABILITY_FLAG             0x8U

/*------------------[Constants]----------------------------------------------*/

#if (defined XCP_EV_CMD_PENDING_LENGTH)
#error XCP_EV_CMD_PENDING_LENGTH already defined
#endif

#define XCP_EV_CMD_PENDING_LENGTH 2U

#if (defined XCP_DL_PGM_CMD_HEADER_LENGTH)
#error XCP_DL_PGM_CMD_HEADER_LENGTH already defined
#endif
/** \brief Length of the DOWNLOAD, DOWNLOAD_NEXT, PROGRAM, PROGRAM_NEXT headers (i.e. command code
 **        and length byte) without fill bytes. */
#define XCP_DL_PGM_CMD_HEADER_LENGTH                       2U

#if (defined XCP_UNLOCK_CMD_HEADER_LENGTH)
#error XCP_UNLOCK_CMD_HEADER_LENGTH already defined
#endif
/** \brief Length of the UNLOCK header (i.e. command code and length byte). */
#define XCP_UNLOCK_CMD_HEADER_LENGTH                       2U

#if (defined XCP_INVALID_EVENT_CHANNEL)
#error XCP_INVALID_EVENT_CHANNEL already defined
#endif
/** \brief Definition of value indicating invalid event channel number */
#define XCP_INVALID_EVENT_CHANNEL                          0xFFFFU

#if (defined XCP_RES_PID)
#error XCP_RES_PID already defined
#endif
/** \brief Packet identifier for positive response CTO packets. */
#define XCP_RES_PID                                        0xFFU

#if (defined XCP_ERR_PID)
#error XCP_ERR_PID already defined
#endif
/** \brief Packet identifier for error response CTO packets. */
#define XCP_ERR_PID                                        0xFEU

#if (defined XCP_RESERVED_BYTE)
#error XCP_RESERVED_BYTE already defined
#endif
/** \brief Byte to fill the command response reserved bytes. */
#define XCP_RESERVED_BYTE                                  0x00U

#if (defined XCP_CMD_PID_BASE)
#error XCP_CMD_PID_BASE already defined
#endif
/** \brief Base value of command PID. Commands PIDs are in range 0xC0..0xFF */
#define XCP_CMD_PID_BASE                                   0xC0U

#if (defined XCP_CMD_SYNCH_PID)
#error XCP_CMD_SYNCH_PID already defined
#endif
/** \brief PID for SYNCH command */
#define XCP_CMD_SYNCH_PID                                  0xFCU

#if (defined XCP_CMD_UNLOCK_PID)
#error XCP_CMD_UNLOCK_PID already defined
#endif
/** \brief PID for UNLOCK command */
#define XCP_CMD_UNLOCK_PID                                 0xF7U

#if (defined XCP_CMD_DISCONNECT_PID)
#error XCP_CMD_DISCONNECT_PID already defined
#endif
/** \brief PID for DISCONNECT command */
#define XCP_CMD_DISCONNECT_PID                             0xFEU

#if (defined XCP_CMD_CONNECT_PID)
#error XCP_CMD_CONNECT_PID already defined
#endif
/** \brief PID for CONNECT command */
#define XCP_CMD_CONNECT_PID                                0xFFU

#if (defined XCP_CMD_USERCMD_PID)
#error XCP_CMD_USERCMD_PID already defined
#endif
/** \brief PID for USER_CMD command */
#define XCP_CMD_USERCMD_PID                                0xF1U

#if (defined XCP_CMD_GETID_PID)
#error XCP_CMD_GETID_PID already defined
#endif
/** \brief PID for GET_ID command */
#define XCP_CMD_GETID_PID                                  0xFAU

#if (defined XCP_CMD_TRANSPORT_LAYER_PID)
#error XCP_CMD_TRANSPORT_LAYER_PID already defined
#endif
/** \brief PID for TRANSPORT_LAYER_CMD command */
#define XCP_CMD_TRANSPORT_LAYER_PID                        0xF2U

 #if (defined XCP_CMD_SHORT_DOWNLOAD_PID)
#error XCP_CMD_SHORT_DOWNLOAD_PID already defined
#endif
/** \brief PID for the SHORT_DOWNLOAD command */
#define XCP_CMD_SHORT_DOWNLOAD_PID                         0xEDU

#if (defined XCP_CMD_DOWNLOAD_PID)
#error XCP_CMD_DOWNLOAD_PID already defined
#endif
/** \brief PID for the DOWNLOAD command */
#define XCP_CMD_DOWNLOAD_PID                               0xF0U

#if (defined XCP_CMD_DOWNLOAD_NEXT_PID)
#error XCP_CMD_DOWNLOAD_NEXT_PID already defined
#endif
/** \brief PID for the DOWNLOAD_NEXT command */
#define XCP_CMD_DOWNLOAD_NEXT_PID                          0xEFU

#if (defined XCP_CMD_BUILD_CHECKSUM_PID)
#error XCP_CMD_BUILD_CHECKSUM_PID already defined
#endif
/** \brief PID for the BUILD_CHECKSUM command */
#define XCP_CMD_BUILD_CHECKSUM_PID                         0xF3U

#if (defined XCP_CMD_SHORT_UPLOAD_PID)
#error XCP_CMD_SHORT_UPLOAD_PID already defined
#endif
/** \brief PID for the SHORT_UPLOAD command */
#define XCP_CMD_SHORT_UPLOAD_PID                           0xF4U

#if (defined XCP_CMD_UPLOAD_PID)
#error XCP_CMD_UPLOAD_PID already defined
#endif
/** \brief PID for the UPLOAD command */
#define XCP_CMD_UPLOAD_PID                                 0xF5U

#if (defined XCP_SUBCMD_GET_SLAVE_ID_PID)
#error XCP_SUBCMD_GET_SLAVE_ID_PID already defined
#endif
/** \brief PID for GET_SLAVE_ID sub command */
#define XCP_SUBCMD_GET_SLAVE_ID_PID                        0xFFU

#if (defined XCP_SUBCMD_GET_DAQ_ID_PID)
#error XCP_SUBCMD_GET_DAQ_ID_PID already defined
#endif
/** \brief PID for GET_DAQ_ID sub command */
#define XCP_SUBCMD_GET_DAQ_ID_PID                          0xFEU

#if (defined XCP_CMD_GET_COMM_MODE_INFO_PID)
#error XCP_CMD_GET_COMM_MODE_INFO_PID already defined
#endif
/** \brief PID for GET_COMM_MODE_INFO command */
#define XCP_CMD_GET_COMM_MODE_INFO_PID                     0xFBU

#if (defined XCP_CMD_GET_STATUS_PID)
#error XCP_CMD_GET_STATUS_PID already defined
#endif
/** \brief PID for GET_STATUS command */
#define XCP_CMD_GET_STATUS_PID                             0xFDU

#if (defined XCP_CMD_SET_MTA_PID)
#error XCP_CMD_SET_MTA_PID already defined
#endif
/** \brief PID for SET_MTA command */
#define XCP_CMD_SET_MTA_PID                                0xF6U

#if (defined XCP_CMD_FREE_DAQ_NOT_RECEIVED)
#error XCP_CMD_FREE_DAQ_NOT_RECEIVED already defined
#endif
/** \brief Value for checking whether FREE_DAQ is received */
#define XCP_CMD_FREE_DAQ_NOT_RECEIVED                      0x00U

#if (defined XCP_CMD_FIRST_PROGRAMMING_PID)
#error XCP_CMD_FIRST_PROGRAMMING_PID already defined
#endif
/** \brief PID for the first programming command */
#define XCP_CMD_FIRST_PROGRAMMING_PID                      0xC8U

#if (defined XCP_CMD_PROGRAM_MAX_PID)
#error XCP_CMD_PROGRAM_MAX_PID already defined
#endif
/** \brief PID for PROGRAM_MAX command */
#define XCP_CMD_PROGRAM_MAX_PID                            0xC9U

#if (defined XCP_CMD_PROGRAM_NEXT_PID)
#error XCP_CMD_PROGRAM_NEXT_PID already defined
#endif
/** \brief PID for PROGRAM_NEXT command */
#define XCP_CMD_PROGRAM_NEXT_PID                           0xCAU

#if (defined XCP_CMD_PROGRAM_RESET_PID)
#error XCP_CMD_PROGRAM_RESET_PID already defined
#endif
/** \brief PID for PROGRAM_RESET command */
#define XCP_CMD_PROGRAM_RESET_PID                          0xCFU

#if (defined XCP_CMD_PROGRAM_PID)
#error XCP_CMD_PROGRAM_PID already defined
#endif
/** \brief PID for PROGRAM command */
#define XCP_CMD_PROGRAM_PID                                0xD0U

#if (defined XCP_CMD_PROGRAM_CLEAR_PID)
#error XCP_CMD_PROGRAM_CLEAR_PID already defined
#endif
/** \brief PID for PROGRAM_CLEAR command */
#define XCP_CMD_PROGRAM_CLEAR_PID                          0xD1U

#if (defined XCP_CMD_PROGRAM_START_PID)
#error XCP_CMD_PROGRAM_START_PID already defined
#endif
/** \brief PID for PROGRAM_START command */
#define XCP_CMD_PROGRAM_START_PID                          0xD2U

#if (defined XCP_CMD_LAST_PROGRAMMING_PID)
#error XCP_CMD_LAST_PROGRAMMING_PID already defined
#endif
/** \brief PID for the last programming command */
#define XCP_CMD_LAST_PROGRAMMING_PID                       0xD2U

#if (defined XCP_CMD_GET_PGM_PROC_INFO_PID)
#error XCP_CMD_GET_PGM_PROC_INFO_PID already defined
#endif
/** \brief PID for the GET_PGM_PROCESSOR_INFO command */
#define XCP_CMD_GET_PGM_PROC_INFO_PID                      0xCEU

#if (defined XCP_CMD_GET_SECTOR_INFO_PID)
#error XCP_CMD_GET_SECTOR_INFO_PID already defined
#endif
/** \brief PID for the GET_SECTOR_INFO command */
#define XCP_CMD_GET_SECTOR_INFO_PID                        0xCDU

#if (defined XCP_CMD_FREE_DAQ_PID)
#error XCP_CMD_FREE_DAQ_PID already defined
#endif
/** \brief PID for FREE_DAQ command */
#define XCP_CMD_FREE_DAQ_PID                               0xD6U

#if (defined XCP_CMD_ALLOC_DAQ_PID)
#error XCP_CMD_ALLOC_DAQ_PID already defined
#endif
/** \brief PID for ALLOC_DAQ command */
#define XCP_CMD_ALLOC_DAQ_PID                              0xD5U

#if (defined XCP_CMD_ALLOC_ODT_PID)
#error XCP_CMD_ALLOC_ODT_PID already defined
#endif
/** \brief PID for ALLOC_ODT command */
#define XCP_CMD_ALLOC_ODT_PID                              0xD4U

#if (defined XCP_CMD_ALLOC_ODT_ENTRY_PID)
#error XCP_CMD_ALLOC_ODT_ENTRY_PID already defined
#endif
/** \brief PID for ALLOC_ODT_ENTRY command */
#define XCP_CMD_ALLOC_ODT_ENTRY_PID                        0xD3U

#if (defined XCP_CMD_SET_DAQ_PTR_PID)
#error XCP_CMD_SET_DAQ_PTR_PID already defined
#endif
/** \brief PID for SET_DAQ_PTR command */
#define XCP_CMD_SET_DAQ_PTR_PID                            0xE2U

#if (defined XCP_CMD_WRITE_DAQ_PID)
#error XCP_CMD_WRITE_DAQ_PID already defined
#endif
/** \brief PID for WRITE_DAQ command */
#define XCP_CMD_WRITE_DAQ_PID                              0xE1U

#if (defined XCP_CMD_SET_DAQ_LIST_MODE_PID)
#error XCP_CMD_SET_DAQ_LIST_MODE_PID already defined
#endif
/** \brief PID for SET_DAQ_LIST_MODE command */
#define XCP_CMD_SET_DAQ_LIST_MODE_PID                      0xE0U

#if (defined XCP_CMD_START_STOP_DAQ_LIST_PID)
#error XCP_CMD_START_STOP_DAQ_LIST_PID already defined
#endif
/** \brief PID for START_STOP_DAQ_LIST command */
#define XCP_CMD_START_STOP_DAQ_LIST_PID                    0xDEU

#if (defined XCP_CMD_START_STOP_SYNCH_PID)
#error XCP_CMD_START_STOP_SYNCH_PID already defined
#endif
/** \brief PID for START_STOP_SYNCH command */
#define XCP_CMD_START_STOP_SYNCH_PID                       0xDDU

#if (defined XCP_CMD_WRITE_DAQ_MULTIPLE_PID)
#error XCP_CMD_WRITE_DAQ_MULTIPLE_PID already defined
#endif
/** \brief PID for WRITE_DAQ_MULTIPLE command */
#define XCP_CMD_WRITE_DAQ_MULTIPLE_PID                     0xC7U

#if (defined XCP_CMD_READ_DAQ_PID)
#error XCP_CMD_READ_DAQ_PID already defined
#endif
/** \brief PID for READ_DAQ command */
#define XCP_CMD_READ_DAQ_PID                               0xDBU

#if (defined XCP_CMD_GET_DAQ_CLOCK_PID)
#error XCP_CMD_GET_DAQ_CLOCK_PID already defined
#endif
/** \brief PID for GET_DAQ_CLOCK command */
#define XCP_CMD_GET_DAQ_CLOCK_PID                          0xDCU

#if (defined XCP_CMD_GET_DAQ_PROCESSOR_INFO_PID)
#error XCP_CMD_GET_DAQ_PROCESSOR_INFO_PID already defined
#endif
/** \brief PID for GET_DAQ_PROCESSOR_INFO command */
#define XCP_CMD_GET_DAQ_PROCESSOR_INFO_PID                 0xDAU

#if (defined XCP_CMD_GET_DAQ_RESOLUTION_INFO_PID)
#error XCP_CMD_GET_DAQ_RESOLUTION_INFO_PID already defined
#endif
/** \brief PID for GET_DAQ_RESOLUTION_INFO command */
#define XCP_CMD_GET_DAQ_RESOLUTION_INFO_PID                0xD9U

#if (defined XCP_CMD_GET_DAQ_LIST_MODE_PID)
#error XCP_CMD_GET_DAQ_LIST_MODE_PID already defined
#endif
/** \brief PID for GET_DAQ_LIST_MODE command */
#define XCP_CMD_GET_DAQ_LIST_MODE_PID                      0xDFU

#if (defined XCP_CMD_GET_DAQ_EVENT_INFO_PID)
#error XCP_CMD_GET_DAQ_EVENT_INFO_PID already defined
#endif
/** \brief PID for GET_DAQ_EVENT_INFO command */
#define XCP_CMD_GET_DAQ_EVENT_INFO_PID                     0xD7U

#if (defined XCP_CMD_CLEAR_DAQ_LIST_PID)
#error XCP_CMD_CLEAR_DAQ_LIST_PID already defined
#endif
/** \brief PID for CLEAR_DAQ_LIST command */
#define XCP_CMD_CLEAR_DAQ_LIST_PID                         0xE3U

#if (defined XCP_CMD_GET_DAQ_LIST_INFO_PID)
#error XCP_CMD_GET_DAQ_LIST_INFO_PID already defined
#endif
/** \brief PID for GET_DAQ_LIST_INFO command */
#define XCP_CMD_GET_DAQ_LIST_INFO_PID                      0xD8U

#if (defined XCP_SUBCMD_FLX_ASSIGN)
#error XCP_SUBCMD_FLX_ASSIGN already defined
#endif
/** \brief PID for FLX_ASSIGN subcommand */
#define XCP_SUBCMD_FLX_ASSIGN                              0xFFU

#if (defined XCP_SUBCMD_FLX_ACTIVATE)
#error XCP_SUBCMD_FLX_ACTIVATE already defined
#endif
/** \brief PID for FLX_ACTIVATE subcommand */
#define XCP_SUBCMD_FLX_ACTIVATE                            0xFEU

#if (defined XCP_SUBCMD_FLX_DEACTIVATE)
#error XCP_SUBCMD_FLX_DEACTIVATE already defined
#endif
/** \brief PID for FLX_DEACTIVATE subcommand */
#define XCP_SUBCMD_FLX_DEACTIVATE                          0xFDU

#if (defined XCP_SUBCMD_GET_DAQ_FLX_BUF)
#error XCP_SUBCMD_GET_DAQ_FLX_BUF already defined
#endif
/** \brief PID for GET_DAQ_FLX_BUF subcommand */
#define XCP_SUBCMD_GET_DAQ_FLX_BUF                         0xFCU

#if (defined XCP_SUBCMD_SET_DAQ_FLX_BUF)
#error XCP_SUBCMD_SET_DAQ_FLX_BUF already defined
#endif
/** \brief PID for SET_DAQ_FLX_BUF subcommand */
#define XCP_SUBCMD_SET_DAQ_FLX_BUF                         0xFBU

#if (defined XCP_CMD_DISCONNECT_RES_LENGTH)
#error XCP_CMD_DISCONNECT_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command DISCONNECT. */
#define XCP_CMD_DISCONNECT_RES_LENGTH                      0x01U

#if (defined XCP_CMD_CONNECT_RES_LENGTH)
#error XCP_CMD_CONNECT_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command CONNECT. */
#define XCP_CMD_CONNECT_RES_LENGTH                         0x08U

#if (defined XCP_CMD_GETID_RES_LENGTH_MIN)
#error XCP_CMD_GETID_RES_LENGTH_MIN already defined
#endif
/** \brief Minimum length of positive response to the command GET_ID. */
#define XCP_CMD_GETID_RES_LENGTH_MIN                       0x08U

#if (defined XCP_CMD_GET_COMM_INFO_RES_LENGTH)
#error XCP_CMD_GET_COMM_INFO_RES_LENGTH already defined
#endif
/** \brief The length of positive response to the command GET_COMM_MODE_INFO. */
#define XCP_CMD_GET_COMM_INFO_RES_LENGTH                   0x08U

#if (defined XCP_CMD_COPY_CAL_PAGE_RES_LENGTH)
#error XCP_CMD_COPY_CAL_PAGE_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command COPY_CAL_PAGE. */
#define XCP_CMD_COPY_CAL_PAGE_RES_LENGTH                   0x01U

#if (defined XCP_CMD_GET_PAG_PROC_INFO_RES_LENGTH)
#error XCP_CMD_GET_PAG_PROC_INFO_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command GET_PAG_PROCESSOR_INFO. */
#define XCP_CMD_GET_PAG_PROC_INFO_RES_LENGTH               0x03U

#if (defined XCP_CMD_GET_CAL_PAGE_RES_LENGTH)
#error XCP_CMD_GET_CAL_PAGE_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command GET_CAL_PAGE. */
#define XCP_CMD_GET_CAL_PAGE_RES_LENGTH                    0x04U

#if (defined XCP_CMD_SET_CAL_PAGE_RES_LENGTH)
#error XCP_CMD_SET_CAL_PAGE_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command SET_CAL_PAGE. */
#define XCP_CMD_SET_CAL_PAGE_RES_LENGTH                    0x01U

#if (defined XCP_CMD_GET_SEGMENT_MODE_RES_LENGTH)
#error XCP_CMD_GET_SEGMENT_MODE_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command GET_SEGMENT_MODE. */
#define XCP_CMD_GET_SEGMENT_MODE_RES_LENGTH                0x03U

#if (defined XCP_CMD_SET_SEGMENT_MODE_RES_LENGTH)
#error XCP_CMD_SET_SEGMENT_MODE_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command SET_SEGMENT_MODE. */
#define XCP_CMD_SET_SEGMENT_MODE_RES_LENGTH                0x01U

#if (defined XCP_CMD_GET_SEGMENT_INFO_RES_LENGTH_0)
#error XCP_CMD_GET_SEGMENT_INFO_RES_LENGTH_0 already defined
#endif
/** \brief Length for positive response to the command GET_SEGMENT_INFO
 ** for Mode 0 or Mode 2. */
#define XCP_CMD_GET_SEGMENT_INFO_RES_LENGTH_0              0x08U

#if (defined XCP_CMD_GET_SEGMENT_INFO_RES_LENGTH_1)
#error XCP_CMD_GET_SEGMENT_INFO_RES_LENGTH_1 already defined
#endif
/** \brief Length for positive response to the command GET_SEGMENT_INFO
 ** for Mode 1. */
#define XCP_CMD_GET_SEGMENT_INFO_RES_LENGTH_1              0x06U

#if (defined XCP_CMD_GET_PAGE_INFO_RES_LENGTH)
#error XCP_CMD_GET_PAGE_INFO_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command GET_PAGE_INFO. */
#define XCP_CMD_GET_PAGE_INFO_RES_LENGTH                    0x03U

#if (defined XCP_CMD_SSDL_RES_LENGTH)
#error XCP_CMD_SSDL_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command START_STOP_DAQ_LIST. */
#define XCP_CMD_SSDL_RES_LENGTH                            0x02U

#if (defined XCP_CMD_SS_SYNCH_RES_LENGTH)
#error XCP_CMD_SS_SYNCH_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command START_STOP_SYNCH. */
#define XCP_CMD_SS_SYNCH_RES_LENGTH                        0x01U

#if (defined XCP_CMD_GET_DAQLI_RES_LENGTH)
#error XCP_CMD_GET_DAQLI_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command GET_DAQ_LIST_INFO. */
#define XCP_CMD_GET_DAQLI_RES_LENGTH                       0x06U

#if (defined XCP_CMD_GET_DAQEI_RES_LENGTH)
#error XCP_CMD_GET_DAQEI_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command GET_DAQ_EVENT_INFO. */
#define XCP_CMD_GET_DAQEI_RES_LENGTH                       0x07U

#if (defined XCP_CMD_GET_DAQLM_RES_LENGTH)
#error XCP_CMD_GET_DAQLM_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command GET_DAQ_LIST_MODE. */
#define XCP_CMD_GET_DAQLM_RES_LENGTH                       0x08U

#if (defined XCP_CMD_GET_DAQPI_RES_LENGTH)
#error XCP_CMD_GET_DAQPI_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command GET_DAQ_PROCESSOR_INFO.*/
#define XCP_CMD_GET_DAQPI_RES_LENGTH                       0x08U

#if (defined XCP_CMD_GETSTATUS_RES_LENGTH)
#error XCP_CMD_GETSTATUS_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command GET_STATUS. */
#define XCP_CMD_GETSTATUS_RES_LENGTH                       0x06U

#if (defined XCP_CMD_GET_DAQRI_RES_LENGTH)
#error XCP_CMD_GET_DAQRI_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command GET_DAQ_RESOLUTION_INFO. */
#define XCP_CMD_GET_DAQRI_RES_LENGTH                       0x08U

#if (defined XCP_CMD_SET_DAQLM_RES_LENGTH)
#error XCP_CMD_SET_DAQLM_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command SET_DAQ_LIST_MODE. */
#define XCP_CMD_SET_DAQLM_RES_LENGTH                       0x01U

#if (defined XCP_CMD_FREE_DAQ_RES_LENGTH)
#error XCP_CMD_FREE_DAQ_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command FREE_DAQ. */
#define XCP_CMD_FREE_DAQ_RES_LENGTH                        0x01U

#if (defined XCP_CMD_ALLOC_DAQ_RES_LENGTH)
#error XCP_CMD_ALLOC_DAQ_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command ALLOC_DAQ. */
#define XCP_CMD_ALLOC_DAQ_RES_LENGTH                       0x01U

#if (defined XCP_CMD_CLEAR_DAQ_RES_LENGTH)
#error XCP_CMD_CLEAR_DAQ_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command CLEAR_DAQ_LIST. */
#define XCP_CMD_CLEAR_DAQ_RES_LENGTH                       0x01U;

#if (defined XCP_CMD_GETSEED_RES_LENGTH_MIN)
#error XCP_CMD_GETSEED_RES_LENGTH_MIN already defined
#endif
/** \brief Length for positive response to the command GET_SEED when the
 * resource is already unlocked. */
#define XCP_CMD_GETSEED_RES_LENGTH_MIN                     0x02U

#if (defined XCP_CMD_UNLOCK_RES_LENGTH)
#error XCP_CMD_UNLOCK_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command UNLOCK. */
#define XCP_CMD_UNLOCK_RES_LENGTH                          0x02U

#if (defined XCP_CMD_GET_DAQ_CLOCK_RES_LENGTH)
#error XCP_CMD_GET_DAQ_CLOCK_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command GET_DAQ_CLOCK. */
#define XCP_CMD_GET_DAQ_CLOCK_RES_LENGTH                   0x08U

#if (defined XCP_CMD_SET_MTA_RES_LENGTH)
#error XCP_CMD_SET_MTA_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command SET_MTA. */
#define XCP_CMD_SET_MTA_RES_LENGTH                         0x01U

#if (defined XCP_MTA_ADDRESS_LENGTH)
#error XCP_MTA_ADDRESS_LENGTH already defined
#endif
/** \brief Length for the MTA address. */
#define XCP_MTA_ADDRESS_LENGTH                             0x04U

#if (defined XCP_CMD_PGM_START_RES_LENGTH)
#error XCP_CMD_PGM_START_RES_LENGTH already defined
#endif
/** \brief Length of positive response to the command PROGRAM_START. */
#define XCP_CMD_PGM_START_RES_LENGTH                       0x07U

#if (defined XCP_CMD_PGM_CLEAR_RES_LENGTH)
#error XCP_CMD_PGM_CLEAR_RES_LENGTH already defined
#endif
/** \brief Length of positive response to the command PROGRAM_CLEAR. */
#define XCP_CMD_PGM_CLEAR_RES_LENGTH                       0x01U

#if (defined XCP_CMD_PGM_RES_LENGTH)
#error XCP_CMD_PGM_RES_LENGTH already defined
#endif
/** \brief Length of positive response to the command PROGRAM. */
#define XCP_CMD_PGM_RES_LENGTH                             0x01U

#if (defined XCP_CMD_PGM_PROC_INFO_RES_LENGTH)
#error XCP_CMD_PGM_PROC_INFO_RES_LENGTH already defined
#endif
/** \brief Length of positive response for command GET_PGM_PROCESSOR_INFO. */
#define XCP_CMD_PGM_PROC_INFO_RES_LENGTH                   0x03U

#if (defined XCP_CMD_SECTOR_INFO_MAX_RES_LENGTH)
#error XCP_CMD_SECTOR_INFO_MAX_RES_LENGTH already defined
#endif
/** \brief Length of positive response for command GET_SECTOR_INFO, in case
 *         the mode is 0x00 or 0x01. */
#define XCP_CMD_SECTOR_INFO_MAX_RES_LENGTH                 0x08U

#if (defined XCP_CMD_SECTOR_INFO_MIN_RES_LENGTH)
#error XCP_CMD_SECTOR_INFO_MIN_RES_LENGTH already defined
#endif
/** \brief Length of positive response for command GET_SECTOR_INFO, in case
 *         the mode is 0x02. */
#define XCP_CMD_SECTOR_INFO_MIN_RES_LENGTH                 0x02U

#if (defined XCP_SECTOR_INFO_NAME_BYTE_POS)
#error XCP_SECTOR_INFO_NAME_BYTE_POS already defined
#endif
/** \brief Byte position for SECTOR_NAME_LENGTH information inside the positive response
 *         for command GET_SECTOR_INFO(used when the requested mode is 0x02). */
#define XCP_SECTOR_INFO_NAME_BYTE_POS                      0x00U

#if (defined XCP_RES_PID_INDEX)
#error XCP_RES_PID_INDEX already defined
#endif
/** \brief Packet Identifier byte position in response packet. */
#define XCP_RES_PID_INDEX                                  0x00U

#if (defined XCP_RES_ERRCODE_INDEX)
#error XCP_RES_ERRCODE_INDEX already defined
#endif
/** \brief Error Code byte position in response packet. */
#define XCP_RES_ERRCODE_INDEX                              0x01U

#if (defined XCP_RES_ERRGENERIC_ADDERR_INDEX)
#error XCP_RES_ERRGENERIC_ADDERR_INDEX already defined
#endif
/** \brief The additional slave device error code byte position in ERR_GENERIC error packet. */
#define XCP_RES_ERRGENERIC_ADDERR_INDEX                    0x02U

#if (defined XCP_BYTE0_INDEX)
#error XCP_BYTE0_INDEX already defined
#endif
/** \brief Zeroth Byte position for generic use purpose. */
#define XCP_BYTE0_INDEX                                   0x00U

#if (defined XCP_BYTE1_INDEX)
#error XCP_BYTE1_INDEX already defined
#endif
/** \brief First Byte position for generic use purpose. */
#define XCP_BYTE1_INDEX                                   0x01U

#if (defined XCP_BYTE2_INDEX)
#error XCP_BYTE2_INDEX already defined
#endif
/** \brief Second Byte position for generic use purpose. */
#define XCP_BYTE2_INDEX                                   0x02U

#if (defined XCP_BYTE3_INDEX)
#error XCP_BYTE3_INDEX already defined
#endif
/** \brief Third Byte position for generic use purpose. */
#define XCP_BYTE3_INDEX                                   0x03U

#if (defined XCP_RES_BYTE0_INDEX)
#error XCP_RES_BYTE0_INDEX already defined
#endif
/** \brief Zeroth Byte position in response packet. */
#define XCP_RES_BYTE0_INDEX                                0x00U

#if (defined XCP_RES_BYTE1_INDEX)
#error XCP_RES_BYTE1_INDEX already defined
#endif
/** \brief First Byte position in response packet. */
#define XCP_RES_BYTE1_INDEX                                0x01U

#if (defined XCP_RES_BYTE2_INDEX)
#error XCP_RES_BYTE2_INDEX already defined
#endif
/** \brief Second Byte position in response packet. */
#define XCP_RES_BYTE2_INDEX                                0x02U

#if (defined XCP_RES_BYTE3_INDEX)
#error XCP_RES_BYTE3_INDEX already defined
#endif
/** \brief Third Byte position in response packet. */
#define XCP_RES_BYTE3_INDEX                                0x03U

#if (defined XCP_RES_BYTE4_INDEX)
#error XCP_RES_BYTE4_INDEX already defined
#endif
/** \brief Fourth Byte position in response packet. */
#define XCP_RES_BYTE4_INDEX                                0x04U

#if (defined XCP_RES_BYTE5_INDEX)
#error XCP_RES_BYTE5_INDEX already defined
#endif
/** \brief Fifth Byte position in response packet. */
#define XCP_RES_BYTE5_INDEX                                0x05U

#if (defined XCP_RES_BYTE6_INDEX)
#error XCP_RES_BYTE6_INDEX already defined
#endif
/** \brief Sixth Byte position in response packet. */
#define XCP_RES_BYTE6_INDEX                                0x06U

#if (defined XCP_RES_BYTE7_INDEX)
#error XCP_RES_BYTE7_INDEX already defined
#endif
/** \brief Seventh Byte position in response packet. */
#define XCP_RES_BYTE7_INDEX                                0x07U

#if (defined XCP_RES_BYTE8_INDEX)
#error XCP_RES_BYTE8_INDEX already defined
#endif
/** \brief Eighth Byte position in response packet. */
#define XCP_RES_BYTE8_INDEX                                0x08U

#if (defined XCP_RES_BYTE9_INDEX)
#error XCP_RES_BYTE9_INDEX already defined
#endif
/** \brief Ninth Byte position in response packet. */
#define XCP_RES_BYTE9_INDEX                                0x09U

#if (defined XCP_RES_BYTE10_INDEX)
#error XCP_RES_BYTE10_INDEX already defined
#endif
/** \brief Tenth Byte position in response packet. */
#define XCP_RES_BYTE10_INDEX                               0x0AU

#if (defined XCP_RES_BYTE11_INDEX)
#error XCP_RES_BYTE11_INDEX already defined
#endif
/** \brief Eleventh Byte position in response packet. */
#define XCP_RES_BYTE11_INDEX                               0x0BU

#if (defined XCP_RES_BYTE12_INDEX)
#error XCP_RES_BYTE12_INDEX already defined
#endif
/** \brief Twelfth Byte position in response packet. */
#define XCP_RES_BYTE12_INDEX                               0x0CU

#if (defined XCP_RES_BYTE13_INDEX)
#error XCP_RES_BYTE13_INDEX already defined
#endif
/** \brief Thirteenth Byte position in response packet. */
#define XCP_RES_BYTE13_INDEX                               0x0DU

#if (defined XCP_RES_BYTE14_INDEX)
#error XCP_RES_BYTE14_INDEX already defined
#endif
/** \brief Fourteenth Byte position in response packet. */
#define XCP_RES_BYTE14_INDEX                               0x0EU

#if (defined XCP_RES_BYTE15_INDEX)
#error XCP_RES_BYTE15_INDEX already defined
#endif
/** \brief Fifteenth Byte position in response packet. */
#define XCP_RES_BYTE15_INDEX                               0x0FU

#if (defined XCP_RES_BYTE16_INDEX)
#error XCP_RES_BYTE16_INDEX already defined
#endif
/** \brief Sixteenth Byte position in response packet. */
#define XCP_RES_BYTE16_INDEX                               0x10U

#if (defined XCP_RES_BYTE17_INDEX)
#error XCP_RES_BYTE17_INDEX already defined
#endif
/** \brief Seventeenth Byte position in response packet. */
#define XCP_RES_BYTE17_INDEX                               0x11U

#if (defined XCP_RES_BYTE18_INDEX)
#error XCP_RES_BYTE18_INDEX already defined
#endif
/** \brief Eighteenth Byte position in response packet. */
#define XCP_RES_BYTE18_INDEX                               0x12U

#if (defined XCP_RES_BYTE19_INDEX)
#error XCP_RES_BYTE19_INDEX already defined
#endif
/** \brief Nineteenth Byte position in response packet. */
#define XCP_RES_BYTE19_INDEX                               0x13U

#if (defined XCP_RES_BYTE20_INDEX)
#error XCP_RES_BYTE20_INDEX already defined
#endif
/** \brief twentieth Byte position in response packet. */
#define XCP_RES_BYTE20_INDEX                               0x14U

#if (defined XCP_RES_BYTE21_INDEX)
#error XCP_RES_BYTE21_INDEX already defined
#endif
/** \brief twenty-first Byte position in response packet. */
#define XCP_RES_BYTE21_INDEX                               0x15U

#if (defined XCP_CMD_BYTE0_INDEX)
#error XCP_CMD_BYTE0_INDEX already defined
#endif
/** \brief Zeroth Byte position in command packet. */
#define XCP_CMD_BYTE0_INDEX                                0x00U

#if (defined XCP_CMD_BYTE1_INDEX)
#error XCP_CMD_BYTE1_INDEX already defined
#endif
/** \brief First Byte position in command packet. */
#define XCP_CMD_BYTE1_INDEX                                0x01U

#if (defined XCP_CMD_BYTE2_INDEX)
#error XCP_CMD_BYTE2_INDEX already defined
#endif
/** \brief Second Byte position in command packet. */
#define XCP_CMD_BYTE2_INDEX                                0x02U

#if (defined XCP_CMD_BYTE3_INDEX)
#error XCP_CMD_BYTE3_INDEX already defined
#endif
/** \brief Third Byte position in command packet. */
#define XCP_CMD_BYTE3_INDEX                                0x03U

#if (defined XCP_CMD_BYTE4_INDEX)
#error XCP_CMD_BYTE4_INDEX already defined
#endif
/** \brief Fourth Byte position in command packet. */
#define XCP_CMD_BYTE4_INDEX                                0x04U

#if (defined XCP_CMD_BYTE5_INDEX)
#error XCP_CMD_BYTE5_INDEX already defined
#endif
/** \brief Fifth Byte position in command packet. */
#define XCP_CMD_BYTE5_INDEX                                0x05U

#if (defined XCP_CMD_BYTE6_INDEX)
#error XCP_CMD_BYTE6_INDEX already defined
#endif
/** \brief Sixth Byte position in command packet. */
#define XCP_CMD_BYTE6_INDEX                                0x06U

#if (defined XCP_CMD_BYTE7_INDEX)
#error XCP_CMD_BYTE7_INDEX already defined
#endif
/** \brief Seventh Byte position in command packet. */
#define XCP_CMD_BYTE7_INDEX                                0x07U

#if (defined XCP_CMD_BYTE8_INDEX)
#error XCP_CMD_BYTE8_INDEX already defined
#endif
/** \brief Eighth Byte position in command packet. */
#define XCP_CMD_BYTE8_INDEX                                0x08U

#if (defined XCP_CMD_BYTE9_INDEX)
#error XCP_CMD_BYTE9_INDEX already defined
#endif
/** \brief Ninth Byte position in command packet. */
#define XCP_CMD_BYTE9_INDEX                                0x09U

#if (defined XCP_CMD_BYTE10_INDEX)
#error XCP_CMD_BYTE10_INDEX already defined
#endif
/** \brief Tenth Byte position in command packet. */
#define XCP_CMD_BYTE10_INDEX                               0x0AU

#if (defined XCP_CMD_BYTE11_INDEX)
#error XCP_CMD_BYTE11_INDEX already defined
#endif
/** \brief Eleventh Byte position in command packet. */
#define XCP_CMD_BYTE11_INDEX                               0x0BU

#if (defined XCP_CMD_BYTE12_INDEX)
#error XCP_CMD_BYTE12_INDEX already defined
#endif
/** \brief Twelfth Byte position in command packet. */
#define XCP_CMD_BYTE12_INDEX                               0x0CU

#if (defined XCP_CMD_BYTE13_INDEX)
#error XCP_CMD_BYTE13_INDEX already defined
#endif
/** \brief Thirteenth Byte position in command packet. */
#define XCP_CMD_BYTE13_INDEX                               0x0DU

#if (defined XCP_CMD_BYTE14_INDEX)
#error XCP_CMD_BYTE14_INDEX already defined
#endif
/** \brief Fourteenth Byte position in command packet. */
#define XCP_CMD_BYTE14_INDEX                               0x0EU

#if (defined XCP_CMD_BYTE15_INDEX)
#error XCP_CMD_BYTE15_INDEX already defined
#endif
/** \brief Fifteenth Byte position in command packet. */
#define XCP_CMD_BYTE15_INDEX                               0x0FU

#if (defined XCP_CMD_BYTE16_INDEX)
#error XCP_CMD_BYTE16_INDEX already defined
#endif
/** \brief Sixteenth Byte position in command packet. */
#define XCP_CMD_BYTE16_INDEX                               0x10U

#if (defined XCP_CMD_BYTE17_INDEX)
#error XCP_CMD_BYTE17_INDEX already defined
#endif
/** \brief Seventeenth Byte position in command packet. */
#define XCP_CMD_BYTE17_INDEX                               0x11U

#if (defined XCP_CMD_BYTE18_INDEX)
#error XCP_CMD_BYTE18_INDEX already defined
#endif
/** \brief Eighteenth Byte position in command packet. */
#define XCP_CMD_BYTE18_INDEX                               0x12U

#if (defined XCP_CMD_BYTE19_INDEX)
#error XCP_CMD_BYTE19_INDEX already defined
#endif
/** \brief Nineteenth Byte position in command packet. */
#define XCP_CMD_BYTE19_INDEX                               0x13U

#if (defined XCP_CMD_BYTE20_INDEX)
#error XCP_CMD_BYTE20_INDEX already defined
#endif
/** \brief twentieth Byte position in command packet. */
#define XCP_CMD_BYTE20_INDEX                               0x14U

#if (defined XCP_TRANSPORT_LAYER_CMD_MIN_LENGTH)
#error XCP_TRANSPORT_LAYER_CMD_MIN_LENGTH already defined
#endif
/** \brief Mandatory minimum length of the TRANSPORT_LAYER_CMD command. */
#define XCP_TRANSPORT_LAYER_CMD_MIN_LENGTH                 0x02U

#if (defined XCP_UNLOCK_CMD_MIN_LENGTH)
#error XCP_UNLOCK_CMD_MIN_LENGTH already defined
#endif
/** \brief Mandatory minimum length of the UNLOCK command. */
#define XCP_UNLOCK_CMD_MIN_LENGTH                          0x03U

#if (defined XCP_BLOCK_MODE_CMD_MIN_LENGTH)
#error XCP_BLOCK_MODE_CMD_MIN_LENGTH already defined
#endif
/** \brief Mandatory minimum length of the command that can be executed in block mode . */
#define XCP_BLOCK_MODE_CMD_MIN_LENGTH                      0x02U

#if (defined XCP_SUBCMD_GET_SLAVE_ID_ON_CAN_LENGTH)
#error XCP_SUBCMD_GET_SLAVE_ID_ON_CAN_LENGTH already defined
#endif
/** \brief Mandatory length of the GET_SLAVE_ID on CAN subcommand. */
#define XCP_SUBCMD_GET_SLAVE_ID_ON_CAN_LENGTH              6U

#if (defined XCP_SUBCMD_GET_SLAVE_ID_ON_ETH_LENGTH)
#error XCP_SUBCMD_GET_SLAVE_ID_ON_ETH_LENGTH already defined
#endif
/** \brief Mandatory length of the GET_SLAVE_ID on ETHERNET subcommand. */
#define XCP_SUBCMD_GET_SLAVE_ID_ON_ETH_LENGTH              21U

#if (defined XCP_ERR_CMD_BUSY)
#error XCP_ERR_CMD_BUSY already defined
#endif
/** \brief The code for error "Command was not executed". */
#define XCP_ERR_CMD_BUSY                                   0x10U

#if (defined XCP_ERR_DAQ_ACTIVE)
#error XCP_ERR_DAQ_ACTIVE already defined
#endif
/** \brief The code for error "Command rejected because DAQ is running". */
#define XCP_ERR_DAQ_ACTIVE                                 0x11U

#if (defined XCP_ERR_PGM_ACTIVE)
#error XCP_ERR_PGM_ACTIVE already defined
#endif
/** \brief The code for error "Command rejected because PGM is running". */
#define XCP_ERR_PGM_ACTIVE                                 0x12U

#if (defined XCP_ERR_CMD_UNKNOWN)
#error XCP_ERR_CMD_UNKNOWN already defined
#endif
/** \brief The code for error "Unknown command or not implemented optional
 * command". */
#define XCP_ERR_CMD_UNKNOWN                                0x20U

#if (defined XCP_ERR_CMD_SYNTAX)
#error XCP_ERR_CMD_SYNTAX already defined
#endif
/** \brief The code for error "Command syntax invalid". */
#define XCP_ERR_CMD_SYNTAX                                 0x21U

#if (defined XCP_ERR_OUT_OF_RANGE)
#error XCP_ERR_OUT_OF_RANGE already defined
#endif
/** \brief The code for error "Command syntax valid but command parameter(s)
 * out of range". */
#define XCP_ERR_OUT_OF_RANGE                               0x22U

#if (defined XCP_ERR_WRITE_PROTECTED)
#error XCP_ERR_WRITE_PROTECTED already defined
#endif
/** \brief The code for error "The memory location is write protected". */
#define XCP_ERR_WRITE_PROTECTED                            0x23U

#if (defined XCP_ERR_ACCESS_DENIED)
#error XCP_ERR_ACCESS_DENIED already defined
#endif
/** \brief The code for error "The memory location is not accessible". */
#define XCP_ERR_ACCESS_DENIED                              0x24U

#if (defined XCP_ERR_ACCESS_LOCKED)
#error XCP_ERR_ACCESS_LOCKED already defined
#endif
/** \brief The code for error "Access denied, Seed & Key is required". */
#define XCP_ERR_ACCESS_LOCKED                              0x25U

#if (defined XCP_ERR_PAGE_NOT_VALID)
#error XCP_ERR_PAGE_NOT_VALID already defined
#endif
/** \brief The code for error "Selected page not available". */
#define XCP_ERR_PAGE_NOT_VALID                             0x26U

#if (defined XCP_ERR_MODE_NOT_VALID)
#error XCP_ERR_MODE_NOT_VALID already defined
#endif
/** \brief The code for error "Selected page mode not available". */
#define XCP_ERR_MODE_NOT_VALID                             0x27U

#if (defined XCP_ERR_SEGMENT_NOT_VALID)
#error XCP_ERR_SEGMENT_NOT_VALID already defined
#endif
/** \brief The code for error "Selected segment not valid". */
#define XCP_ERR_SEGMENT_NOT_VALID                          0x28U

#if (defined XCP_ERR_SEQUENCE)
#error XCP_ERR_SEQUENCE already defined
#endif
/** \brief The code for error "Sequence error". */
#define XCP_ERR_SEQUENCE                                   0x29U

#if (defined XCP_ERR_DAQ_CONFIG)
#error XCP_ERR_DAQ_CONFIG already defined
#endif
/** \brief The code for error "DAQ configuration not valid". */
#define XCP_ERR_DAQ_CONFIG                                 0x2AU

#if (defined XCP_ERR_MEMORY_OVERFLOW)
#error XCP_ERR_MEMORY_OVERFLOW already defined
#endif
/** \brief The code for error "Memory overflow error". */
#define XCP_ERR_MEMORY_OVERFLOW                            0x30U

#if (defined XCP_ERR_GENERIC)
#error XCP_ERR_GENERIC already defined
#endif
/** \brief The code for error "Generic error". */
#define XCP_ERR_GENERIC                                    0x31U

#if (defined XCP_ERR_RES_TEMP_NOT_A)
#error XCP_ERR_RES_TEMP_NOT_A already defined
#endif
/** \brief The code for error "Access to requested temporary not possible". */
#define XCP_ERR_RES_TEMP_NOT_A                             0x33U

#if (defined XCP_ERR_CMD_SYNCH)
#error XCP_ERR_CMD_SYNCH already defined
#endif
/** \brief The code for error "Command processor synchronization". */
#define XCP_ERR_CMD_SYNCH                                  0x00U

#if (defined XCP_ERR_CMD_BUSY_LENGTH)
#error XCP_ERR_CMD_BUSY_LENGTH already defined
#endif
/** \brief Length of error ERR_CMD_BUSY response packet. */
#define XCP_ERR_CMD_BUSY_LENGTH                            0x02U

#if (defined XCP_ERR_PGM_ACTIVE_LENGTH)
#error XCP_ERR_PGM_ACTIVE_LENGTH already defined
#endif
/** \brief Length of error ERR_PGM_ACTIVE response packet. */
#define XCP_ERR_PGM_ACTIVE_LENGTH                          0x02U

#if (defined XCP_ERR_DAQ_ACTIVE_LENGTH)
#error XCP_ERR_DAQ_ACTIVE_LENGTH already defined
#endif
/** \brief Length of error ERR_DAQ_ACTIVE response packet. */
#define XCP_ERR_DAQ_ACTIVE_LENGTH                          0x02U

#if (defined XCP_ERR_CMD_UNKNOWN_LENGTH)
#error XCP_ERR_CMD_UNKNOWN_LENGTH already defined
#endif
/** \brief Length of error ERR_CMD_UNKNOWN response packet. */
#define XCP_ERR_CMD_UNKNOWN_LENGTH                         0x02U

#if (defined XCP_ERR_CMD_SYNTAX_LENGTH)
#error XCP_ERR_CMD_SYNTAX_LENGTH already defined
#endif
/** \brief Length of error ERR_CMD_SYNTAX response packet. */
#define XCP_ERR_CMD_SYNTAX_LENGTH                          0x02U

#if (defined XCP_ERR_OUT_OF_RANGE_LENGTH)
#error XCP_ERR_OUT_OF_RANGE_LENGTH already defined
#endif
/** \brief Length of error ERR_OUT_OF_RANGE response packet. */
#define XCP_ERR_OUT_OF_RANGE_LENGTH                        0x02U

#if (defined XCP_ERR_WRITE_PROTECTED_LENGTH)
#error XCP_ERR_WRITE_PROTECTED_LENGTH already defined
#endif
/** \brief Length of error ERR_WRITE_PROTECTED response packet. */
#define XCP_ERR_WRITE_PROTECTED_LENGTH                     0x02U

#if (defined XCP_ERR_ACCESS_DENIED_LENGTH)
#error XCP_ERR_ACCESS_DENIED_LENGTH already defined
#endif
/** \brief Length of error ERR_ACCESS_DENIED response packet. */
#define XCP_ERR_ACCESS_DENIED_LENGTH                       0x02U

#if (defined XCP_ERR_ACCESS_LOCKED_LENGTH)
#error XCP_ERR_ACCESS_LOCKED_LENGTH already defined
#endif
/** \brief Length of error ERR_ACCESS_LOCKED response packet. */
#define XCP_ERR_ACCESS_LOCKED_LENGTH                       0x02U

#if (defined XCP_ERR_PAGE_NOT_VALID_LENGTH)
#error XCP_ERR_PAGE_NOT_VALID_LENGTH already defined
#endif
/** \brief Length of error ERR_PAGE_NOT_VALID response packet. */
#define XCP_ERR_PAGE_NOT_VALID_LENGTH                      0x02U

#if (defined XCP_ERR_MODE_NOT_VALID_LENGTH)
#error XCP_ERR_MODE_NOT_VALID_LENGTH already defined
#endif
/** \brief Length of error ERR_MODE_NOT_VALID response packet. */
#define XCP_ERR_MODE_NOT_VALID_LENGTH                      0x02U

#if (defined XCP_ERR_SEGMENT_NOT_VALID_LENGTH)
#error XCP_ERR_SEGMENT_NOT_VALID_LENGTH already defined
#endif
/** \brief Length of error ERR_SEGMENT_NOT_VALID response packet. */
#define XCP_ERR_SEGMENT_NOT_VALID_LENGTH                   0x02U

#if (defined XCP_ERR_SEQUENCE_LENGTH)
#error XCP_ERR_SEQUENCE_LENGTH already defined
#endif
/** \brief Length of error ERR_SEQUENCE response packet. */
#define XCP_ERR_SEQUENCE_LENGTH                            0x02U

#if (defined XCP_ERR_WRITE_SEQUENCE_LENGTH)
#error XCP_ERR_WRITE_SEQUENCE_LENGTH already defined
#endif
/** \brief Length of error ERR_SEQUENCE response packet for the DOWNLOAD_NEXT and PROGRAM_NEXT commands
 * If the number of data elements does not match the expected value, the negative response
 * will contain the expected number of data elements. */
#define XCP_ERR_WRITE_SEQUENCE_LENGTH                      0x03U

#if (defined XCP_DOWNLOAD_POSITIVE_LENGTH)
#error XCP_DOWNLOAD_POSITIVE_LENGTH already defined
#endif
/** \brief The number of response bytes a DOWNLOAD/DOWNLOAD_NEXT/SHORT_DOWNLOAD/DOWNLOAD_MAX
 * command execution will return to the master in case a positive response. */
#define XCP_DOWNLOAD_POSITIVE_LENGTH                       0x01U

#if (defined XCP_SET_REQUEST_POSITIVE_LENGTH)
#error XCP_SET_REQUEST_POSITIVE_LENGTH already defined
#endif
/** \brief The number of response bytes a SET_REQUEST command execution
 *  will return to the master in case a positive response. */
#define XCP_SET_REQUEST_POSITIVE_LENGTH                    0x01U

#if (defined XCP_ERR_DAQ_CONFIG_LENGTH)
#error XCP_ERR_DAQ_CONFIG_LENGTH already defined
#endif
/** \brief Length of error ERR_DAQ_CONFIG response packet. */
#define XCP_ERR_DAQ_CONFIG_LENGTH                          0x02U

#if (defined XCP_ERR_MEMORY_OVERFLOW_LENGTH)
#error XCP_ERR_MEMORY_OVERFLOW_LENGTH already defined
#endif
/** \brief Length of error ERR_MEMORY_OVERFLOW response packet. */
#define XCP_ERR_MEMORY_OVERFLOW_LENGTH                     0x02U

#if (defined XCP_ERR_GENERIC_LENGTH)
#error XCP_ERR_GENERIC_LENGTH already defined
#endif
/** \brief Length of error ERR_GENERIC response packet. */
#define XCP_ERR_GENERIC_LENGTH                             0x04U

#if (defined XCP_ERR_RES_TEMP_NOT_A_LENGTH)
#error XCP_ERR_RES_TEMP_NOT_A_LENGTH already defined
#endif
/** \brief Length of error ERR_RESOURCE_TEMPORARY_NOT_ACCESSIBLE response
 * packet. */
#define XCP_ERR_RES_TEMP_NOT_A_LENGTH                      0x02U

#if (defined XCP_ERR_CMD_SYNCH_LENGTH)
#error XCP_ERR_CMD_SYNCH_LENGTH already defined
#endif
/** \brief Length of error ERR_CMD_SYNCH response packet. */
#define XCP_ERR_CMD_SYNCH_LENGTH                           0x02U

#if (defined XCP_CONNECT_MODE_NORMAL)
#error XCP_CONNECT_MODE_NORMAL already defined
#endif
/** \brief CONNECT(Mode = Normal), the master can start an XCP communication
 * with the slave. */
#define XCP_CONNECT_MODE_NORMAL                            0x00U

#if (defined XCP_CONNECT_MODE_USERDEFINED)
#error XCP_CONNECT_MODE_USERDEFINED already defined
#endif
/** \brief CONNECT(Mode = user defined), the master can start an XCP
 * communication with the slave and at the same time tell the slave that it
 * should go into a special(user defined) mode. */
#define XCP_CONNECT_MODE_USERDEFINED                       0x01U

#if (defined XCP_SSDL_MODE_STOP)
#error XCP_SSDL_MODE_STOP already defined
#endif
/** \brief START_STOP_DAQ_LIST command paramater "mode = stop". */
#define XCP_SSDL_MODE_STOP                                 0x00U

#if (defined XCP_SSDL_MODE_START)
#error XCP_SSDL_MODE_START already defined
#endif
/** \brief START_STOP_DAQ_LIST command paramater "mode = start". */
#define XCP_SSDL_MODE_START                                0x01U

#if (defined XCP_SSDL_MODE_SELECT)
#error XCP_SSDL_MODE_SELECT already defined
#endif
/** \brief START_STOP_DAQ_LIST command paramater "mode = select". */
#define XCP_SSDL_MODE_SELECT                               0x02U

#if (defined XCP_SS_SYNCH_MODE_STOP_ALL)
#error XCP_SS_SYNCH_MODE_STOP_ALL already defined
#endif
/** \brief START_STOP_SYNCH command paramater "mode = stop all". */
#define XCP_SS_SYNCH_MODE_STOP_ALL                         0x00U

#if (defined XCP_SS_SYNCH_MODE_START_SELECTED)
#error XCP_SS_SYNCH_MODE_START_SELECTED already defined
#endif
/** \brief START_STOP_SYNCH command paramater "mode = start selected". */
#define XCP_SS_SYNCH_MODE_START_SELECTED                   0x01U

#if (defined XCP_SS_SYNCH_MODE_STOP_SELECTED)
#error XCP_SS_SYNCH_MODE_STOP_SELECTED already defined
#endif
/** \brief START_STOP_SYNCH command paramater "mode = stop select". */
#define XCP_SS_SYNCH_MODE_STOP_SELECTED                    0x02U

#if (defined XCP_PRESCALER_MIN)
#error XCP_PRESCALER_MIN already defined
#endif
/** \brief The minimum possible value for prescaler . */
#define XCP_PRESCALER_MIN                                  0x01U

#if (defined XCP_NR_OF_XCP_COMMAND_CODES)
#error XCP_NR_OF_XCP_COMMAND_CODES already defined
#endif
/** \brief Number of XCP command codes for mapping tables */
#define XCP_NR_OF_XCP_COMMAND_CODES                        0x40U

#if (defined XCP_CRC_16_CITT)
#error XCP_CRC_16_CITT already defined
#endif
/** \brief Symbolic name indicating that the command BUILD_CHECKSUM will use the
    AUTOSAR standard CRC 16 CITT algorithm.  */
#define XCP_CRC_16_CITT                                    0U

#if (defined XCP_CRC_32)
#error XCP_CRC_32 already defined
#endif
/** \brief Symbolic name indicating that the command BUILD_CHECKSUM will use the
     AUTOSAR standard CRC 32 algorithm.  */
#define XCP_CRC_32                                         1U

#if (defined XCP_CRC_USER_CALLOUT)
#error XCP_CRC_USER_CALLOUT already defined
#endif
/** \brief Symbolic name indicating that the command BUILD_CHECKSUM  will trigger a call
     to the user specific callout function Xcp_ApplBuildChecksum(). */
#define XCP_CRC_USER_CALLOUT                               2U

#if (defined XCP_CRC_16_CITT_TYPE)
#error XCP_CRC_16_CITT_TYPE already defined
#endif
/** \brief The type value that must be returned to the master if the XCP_CRC_16_CITT was used
    when building the checksum */
#define XCP_CRC_16_CITT_TYPE                               8U

#if (defined XCP_CRC_32_TYPE)
#error XCP_CRC_32_TYPE already defined
#endif
/** \brief The type value that must be returned to the master if the XCP_CRC_16_CITT was used
    when building the checksum */
#define XCP_CRC_32_TYPE                                    9U

#if (defined XCP_CRC_16_CITT_STARTVALUE)
#error XCP_CRC_16_CITT_STARTVALUE already defined
#endif
/** \brief The start value for the CRC_16_CITT algorithm defined by ASAM specs */
#define XCP_CRC_16_CITT_STARTVALUE                         0xFFFFU

#if (defined XCP_CRC_32_STARTVALUE)
#error XCP_CRC_32_STARTVALUE already defined
#endif
/** \brief The start value for the CRC_32 algorithm defined by ASAM specs */
#define XCP_CRC_32_STARTVALUE                              0xFFFFFFFFU

#if (defined XCP_CMD_BUILD_CHECKSUM_RES_LENGTH)
#error XCP_CMD_BUILD_CHECKSUM_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command BUILD_CHECKSUM. */
#define XCP_CMD_BUILD_CHECKSUM_RES_LENGTH                  8U

#if (defined XCP_CMD_MODIFY_BITS_RES_LENGTH)
#error XCP_CMD_MODIFY_BITS_RES_LENGTH already defined
#endif
/** \brief Length of positive response to the command MODIFY_BITS. */
#define XCP_CMD_MODIFY_BITS_RES_LENGTH                     0x01U

#if (defined XCP_CMD_WRITE_DAQ_RES_LENGTH)
#error XCP_CMD_WRITE_DAQ_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command WRITE_DAQ. */
#define XCP_CMD_WRITE_DAQ_RES_LENGTH                       0x01U

#if (defined XCP_BIT_OFFSET_IGNORE_VALUE)
#error XCP_BIT_OFFSET_IGNORE_VALUE already defined
#endif
/** \brief Value for BIT_OFFSET that must be ignored. */
#define XCP_BIT_OFFSET_IGNORE_VALUE                        0xFFU

#if (defined XCP_BIT_OFFSET_MAX_VALUE_AG_BYTE)
#error XCP_BIT_OFFSET_MAX_VALUE_AG_BYTE already defined
#endif
/** \brief Maximum value for BIT_OFFSET when Address Granularity is BYTE. */
#define XCP_BIT_OFFSET_MAX_VALUE_AG_BYTE                   7U


#if (defined XCP_BIT_OFFSET_MAX_VALUE_AG_WORD)
#error XCP_BIT_OFFSET_MAX_VALUE_AG_WORD already defined
#endif
/** \brief Maximum value for BIT_OFFSET when Address Granularity is WORD. */
#define XCP_BIT_OFFSET_MAX_VALUE_AG_WORD                   15U


#if (defined XCP_BIT_OFFSET_MAX_VALUE_AG_DWORD)
#error XCP_BIT_OFFSET_MAX_VALUE_AG_DWORD already defined
#endif
/** \brief Maximum value for BIT_OFFSET when Address Granularity is DWORD. */
#define XCP_BIT_OFFSET_MAX_VALUE_AG_DWORD                  31U


#if (defined XCP_CMD_SET_DAQ_PTR_RES_LENGTH)
#error XCP_CMD_SET_DAQ_PTR_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command SET_DAQ_PTR. */
#define XCP_CMD_SET_DAQ_PTR_RES_LENGTH                     0x01U


#if (defined XCP_CMD_SET_REQUEST_RES_LENGTH)
#error XCP_CMD_SET_REQUEST_RES_LENGTH already defined
#endif
/** \brief Length for a response to the command SET_REQUEST. */
#define XCP_CMD_SET_REQUEST_RES_LENGTH                     1U

#if (defined XCP_SUBCMD_FLX_ASSIGN_RES_LENGTH)
#error XCP_SUBCMD_FLX_ASSIGN_RES_LENGTH already defined
#endif
/** \brief Length for a response to the command FLX_ASSIGN. */
#define XCP_SUBCMD_FLX_ASSIGN_RES_LENGTH                   1U

#if (defined XCP_SUBCMD_FLX_ACTIVATE_RES_LENGTH)
#error XCP_SUBCMD_FLX_ACTIVATE_RES_LENGTH already defined
#endif
/** \brief Length for a response to the commands FLX_ACTIVATE/FLX_DEACTIVATE. */
#define XCP_SUBCMD_FLX_ACTIVATE_RES_LENGTH                 1U

 #if (defined XCP_SUBCMD_SET_DAQ_FLX_BUF_RES_LENGTH)
#error XCP_SUBCMD_SET_DAQ_FLX_BUF_RES_LENGTH already defined
#endif
/** \brief Length for a response to the command SET_DAQ_FLX_BUF. */
#define XCP_SUBCMD_SET_DAQ_FLX_BUF_RES_LENGTH              1U

 #if (defined XCP_SUBCMD_GET_DAQ_FLX_BUF_RES_MIN_LENGTH)
#error XCP_SUBCMD_GET_DAQ_FLX_BUF_RES_MIN_LENGTH already defined
#endif
/** \brief Minimum length for a response to the command GET_DAQ_FLX_BUF. */
#define XCP_SUBCMD_GET_DAQ_FLX_BUF_RES_MIN_LENGTH          3U


#if (defined XCP_MAX_ODT_STIM)
#error XCP_MAX_ODT_STIM already defined
#endif
/** \brief The maximum allowed number of ODTs when STIM is enabled
 * (It is the maximum PID for a STIM ODT (0xBF) + 1) */
#define XCP_MAX_ODT_STIM                                   192U

#if (defined XCP_PROTOCOL_LAYER_MAJOR_VN)
#error XCP_PROTOCOL_LAYER_MAJOR_VN already defined
#endif
/** \brief The XCP Protocol Layer Version Number indicates the major version
 * of the Protocol Layer Specification. */
#define XCP_PROTOCOL_LAYER_MAJOR_VN                        0x01U

#if (defined XCP_TRANSPORT_LAYER_MAJOR_VN)
#error XCP_TRANSPORT_LAYER_MAJOR_VN already defined
#endif
/** \brief The XCP Transport Layer Version Number indicates the major version
 * of the Specification of the current Transport Layer. */
#define XCP_TRANSPORT_LAYER_MAJOR_VN                       0x01U

#if (defined XCP_IDENTIFICATION_TYPE0)
#error XCP_IDENTIFICATION_TYPE0 already defined
#endif
/** \brief Slave device identification type 0.  ASCII Text */
#define XCP_IDENTIFICATION_TYPE0                           0x00U

#if (defined XCP_IDENTIFICATION_TYPE1)
#error XCP_IDENTIFICATION_TYPE1 already defined
#endif
/** \brief Slave device identification type 1.  ASAM-MC2 filename without path
 * and extension. */
#define XCP_IDENTIFICATION_TYPE1                           0x01U

#if (defined XCP_IDENTIFICATION_TYPE2)
#error XCP_IDENTIFICATION_TYPE2 already defined
#endif
/** \brief Slave device identification type 2. ASAM-MC2 filename with path and
 * extension. */
#define XCP_IDENTIFICATION_TYPE2                           0x02U

#if (defined XCP_IDENTIFICATION_TYPE3)
#error XCP_IDENTIFICATION_TYPE3 already defined
#endif
/** \brief Slave device identification type 3. URL where the ASAM-MC2 file can
 * be found. */
#define XCP_IDENTIFICATION_TYPE3                           0x03U

#if (defined XCP_IDENTIFICATION_TYPE4)
#error XCP_IDENTIFICATION_TYPE4 already defined
#endif
/** \brief Slave device identification type 4. ASAM-MC2 file to upload. */
#define XCP_IDENTIFICATION_TYPE4                           0x04U

#if (defined XCP_SIZE_OF_WORD)
#error XCP_SIZE_OF_WORD already defined
#endif
/** \brief Size of word in bytes */
#define XCP_SIZE_OF_WORD                                   2U

#if (defined XCP_SIZE_OF_DWORD)
#error XCP_SIZE_OF_DWORD already defined
#endif
/** \brief Size of DWord in bytes */
#define XCP_SIZE_OF_DWORD                                  4U

#if (defined XCP_SIZE_OF_PID_RELATIVE_WORD)
#error XCP_SIZE_OF_PID_RELATIVE_WORD already defined
#endif
/** \brief Size of PID with Relative word in bytes */
#define XCP_SIZE_OF_PID_RELATIVE_WORD                      3U

#if (defined XCP_CALPAG_ADDRESS_LENGTH_SIZE)
#error XCP_CALPAG_ADDRESS_LENGTH_SIZE already defined
#endif
/** \brief Calibration Paging: Address / Length Size. */
#define XCP_CALPAG_ADDRESS_LENGTH_SIZE                     4U

#if (defined XCP_CALPAG_GETSEGMENTINFO_STANDARD_SIZE)
#error XCP_CALPAG_GETSEGMENTINFO_STANDARD_SIZE already defined
#endif
/** \brief Calibration Paging: GetSegment Info Command.
 * Standard Info Response size (without Packet ID) */
#define XCP_CALPAG_GETSEGMENTINFO_STANDARD_SIZE            5U

#if (defined XCP_CALPAG_EV_STORE_CAL_LENGTH)
#error XCP_CALPAG_EV_STORE_CAL_LENGTH already defined
#endif
/** \brief Calibration Paging: EV_STORE_CAL event.
 * End of CAL Storing event size */
#define XCP_CALPAG_EV_STORE_CAL_LENGTH                     2U

#if (defined XCP_NVM_WRITE_BLOCK_API_ID)
#error XCP_NVM_WRITE_BLOCK_API_ID already defined
#endif
/** \brief Defines API id of function NvM_WriteBlock(). */
#define XCP_NVM_WRITE_BLOCK_API_ID                         0x7U

#if (defined XCP_ABS_MAX_DOWNLOAD_ELEMENTS)
#error XCP_ABS_MAX_DOWNLOAD_ELEMENTS already defined
#endif
/* The absolute maximum of elements that can be downloaded in one download sequence */
#define XCP_ABS_MAX_DOWNLOAD_ELEMENTS                     255U

#if (defined XCP_ABS_MAX_PROGRAM_ELEMENTS)
#error XCP_ABS_MAX_PROGRAM_ELEMENTS already defined
#endif
/* The absolute maximum of elements that can be programmed in one programming sequence */
#define XCP_ABS_MAX_PROGRAM_ELEMENTS                      255U
#if (defined XCP_DUMMY_DAQ_LIST)
#error XCP_DUMMY_DAQ_LIST already defined
#endif
/** \brief Value to indicate the DAQ List default number.
 * DaqIdListCount specifies the validity of a DAQ list ID
 * inside the event's list and not the value of XCP_DUMMY_DAQ_LIST.
 * XCP_DUMMY_DAQ_LIST is filled only to better read the code. */
#define XCP_DUMMY_DAQ_LIST                                  0U

#if (defined XCP_E_OK)
#error XCP_E_OK already defined
#endif
/** \brief XCP internal error for processing was done successful. */
#define XCP_E_OK                                            0U

#if (defined XCP_E_NOT_OK)
#error XCP_E_NOT_OK already defined
#endif
/** \brief XCP internal error for processing can't be done. */
#define XCP_E_NOT_OK                                        1U

#if (defined XCP_E_BUSY)
#error XCP_E_BUSY already defined
#endif
/** \brief XCP internal error for processing isn't complete. */
#define XCP_E_BUSY                                          2U

#if (defined XCP_E_INSUFFICIENT_MEMORY)
#error XCP_E_INSUFFICIENT_MEMORY already defined
#endif
/** \brief XCP internal error for processing failed due to insufficient memory. */
#define XCP_E_INSUFFICIENT_MEMORY                           3U

#if (defined XCP_STATE_CONNECTED)
#error XCP_STATE_CONNECTED already defined
#endif
/** \brief Defines connection status of XCP module for connected. */
#define XCP_STATE_CONNECTED                                 0U

#if (defined XCP_STATE_DISCONNECTED)
#error XCP_STATE_DISCONNECTED already defined
#endif
/** \brief Defines connection status of XCP module for disconnected. */
#define XCP_STATE_DISCONNECTED                              1U

#if (defined XCP_STATE_RESUME)
#error XCP_STATE_RESUME already defined
#endif
/** \brief Defines connection status of XCP module for resume mode. */
#define XCP_STATE_RESUME                                    2U

#if (defined XCP_SUPPORT_FOR_CMD_CTO_PDU)
#error XCP_SUPPORT_FOR_CMD_CTO_PDU already defined
#endif
/** \brief Flag indicating that the support for CMD CTO packages. */
#define XCP_SUPPORT_FOR_CMD_CTO_PDU                         0x1U

#if (defined XCP_SUPPORT_FOR_STIM_DTO_PDU)
#error XCP_SUPPORT_FOR_STIM_DTO_PDU already defined
#endif
/** \brief Flag indicating that the support for STIM DTO packages. */
#define XCP_SUPPORT_FOR_STIM_DTO_PDU                        0x2U

#if (defined XCP_ENABLE_STATE_DYNAMIC_RX_PDU)
#error XCP_ENABLE_STATE_DYNAMIC_RX_PDU already defined
#endif
/** \brief Flag indicating that the dynamic reception Pdu is
    in state ENABLE*/
#define XCP_ENABLE_STATE_DYNAMIC_RX_PDU                   0x10U

#if (defined XCP_DYNAMIC_RX_PDU)
#error XCP_DYNAMIC_RX_PDU already defined
#endif
/** \brief Flag indicating that the reception Pdu can be enabled/disabled
           at runtime */
#define XCP_DYNAMIC_RX_PDU                                  0x20U

#if (defined XCP_SUPPORT_FOR_RESERR_CTO_PDU)
#error XCP_SUPPORT_FOR_RESERR_CTO_PDU already defined
#endif
/** \brief Flag indicating that the support for RES/ERR CTO packages. */
#define XCP_SUPPORT_FOR_RESERR_CTO_PDU                      0x01U

#if (defined XCP_SUPPORT_FOR_EVSERV_CTO_PDU)
#error XCP_SUPPORT_FOR_EVSERV_CTO_PDU already defined
#endif
/** \brief Flag indicating that the support for EV/SERV CTO packages. */
#define XCP_SUPPORT_FOR_EVSERV_CTO_PDU                      0x02U

#if (defined XCP_SUPPORT_FOR_DAQ_DTO_PDU)
#error XCP_SUPPORT_FOR_DAQ_DTO_PDU already defined
#endif
/** \brief Flag indicating that the support for DAQ DTO packages. */
#define XCP_SUPPORT_FOR_DAQ_DTO_PDU                         0x04U

#if (defined XCP_ENABLE_STATE_DYNAMIC_TX_PDU)
#error XCP_ENABLE_STATE_DYNAMIC_TX_PDU already defined
#endif
/** \brief Flag indicating that the transmission Pdu can be enabled/disabled
           at runtime */
#define XCP_ENABLE_STATE_DYNAMIC_TX_PDU                    0x10U

#if (defined XCP_DYNAMIC_TX_PDU)
#error XCP_DYNAMIC_TX_PDU already defined
#endif
/** \brief Flag indicating that the dynamic transmission Pdu is
    in state ENABLE */
#define XCP_DYNAMIC_TX_PDU                                  0x20U

#if (defined XCP_SUPPORT_TX_FROM_CONFIRMATION)
#error XCP_SUPPORT_TX_FROM_CONFIRMATION already defined
#endif
/** \brief Flag indicating that the transmission can be done from
           the context of TxConfirmation. */
#define XCP_SUPPORT_TX_FROM_CONFIRMATION                    0x40U

#if (defined XCP_SUPPORT_RX_FROM_INDICATION)
#error XCP_SUPPORT_RX_FROM_INDICATION already defined
#endif
/** \brief Flag indicating that the reception can be done from
           the context of RxIndication. */
#define XCP_SUPPORT_RX_FROM_INDICATION                    0x40U

#if (defined XCP_SUPPORT_FOR_TRIGGER_TRANSMIT)
#error XCP_SUPPORT_FOR_TRIGGER_TRANSMIT already defined
#endif
/** \brief Flag indicating that the trigger transmit function is supported
 **        by the PDU channel. */
#define XCP_SUPPORT_FOR_TRIGGER_TRANSMIT                    0x80U

#if (defined XCP_MULTI_PDUS_IN_FRAME_FLAG)
#error XCP_MULTI_PDUS_IN_FRAME_FLAG already defined
#endif
/** \brief Flag indicating that the support for multiple PDU in one Frame is supported. */
#define XCP_MULTI_PDUS_IN_FRAME_FLAG                        0x1U

#if (defined XCP_AUTO_OPEN_SOCON_FLAG)
#error XCP_AUTO_OPEN_SOCON_FLAG already defined
#endif
/** \brief Flag indicating that the support for Automatically
 *         opening Socket connection is supported. */
#define XCP_AUTO_OPEN_SOCON_FLAG                            0x2U

#if (defined XCP_SEQUENCE_CORRECTION_FLAG)
#error XCP_SEQUENCE_CORRECTION_FLAG already defined
#endif
/** \brief Flag indicating that the support for FlexRay sequence correction is enabled. */
#define XCP_SEQUENCE_CORRECTION_FLAG                        0x4U


#if (defined XCP_FLEXRAY_SEQUENCE_CORRECTION_BYTE_IDX)
#error XCP_FLEXRAY_SEQUENCE_CORRECTION_BYTE_IDX already defined
#endif
/** \brief The index in the Xcp Frame where the sequence correction byte is. */
#define XCP_FLEXRAY_SEQUENCE_CORRECTION_BYTE_IDX            0x1U

#if (defined XCP_MAX_DLC_REQUIRED)
#error XCP_MAX_DLC_REQUIRED already defined
#endif
/** \brief Flag indicating that the MAX_DLC_REQUIRED parameter is enabled */
#define XCP_MAX_DLC_REQUIRED                                0x8U

#if (defined XCP_AUTONOMOUS_DISCONNECT_FLAG)
#error XCP_AUTONOMOUS_DISCONNECT_FLAG already defined
#endif
/** \brief Flag indicating that the Xcp slave decided to autonomously disconnect. */
#define XCP_AUTONOMOUS_DISCONNECT_FLAG                      0x1U

#if (defined XCP_FLEXRAY_BUFFERS_READY_FLAG)
#error XCP_FLEXRAY_BUFFERS_READY_FLAG already defined
#endif
/** \brief Flag indicating that the Xcp connection over Flexray has the hw buffers initialized. */
#define XCP_FLEXRAY_BUFFERS_READY_FLAG                      0x2U

#if (defined XCP_TP_CONFIGURATION_DONE_FLAG)
#error XCP_TP_CONFIGURATION_DONE_FLAG already defined
#endif
/** \brief Flag indicating that the a TP command already configured PDU related information
 * (e.g. DAQ list accepted by this PUD). */
#define XCP_TP_CONFIGURATION_DONE_FLAG                      0x4U

#if (defined XCP_MULTICAST_TRANSMIT_MODE_FLAG)
#error XCP_MULTICAST_TRANSMIT_MODE_FLAG already defined
#endif
/** \brief FLAG indicating that the transmission is in multicast mode. */
#define XCP_MULTICAST_TRANSMIT_MODE_FLAG                    0x8U

#if (defined XCP_ETHERNET_DISCONNECT_BIT)
#error XCP_ETHERNET_DISCONNECT_BIT already defined
#endif
/** \brief Bit position indicating that the Xcp connection over Ethernet has to be terminated. */
#define XCP_ETHERNET_DISCONNECT_BIT                         0x0U

#if (defined XCP_FLEXRAY_BUFFERS_READY_BIT)
#error XCP_FLEXRAY_BUFFERS_READY_BIT already defined
#endif
/** \brief Bit position indicating that the Xcp connection over Flexray has the hw buffers initialized. */
#define XCP_FLEXRAY_BUFFERS_READY_BIT                       0x1U

#if (defined XCP_TP_CONFIGURATION_DONE_BIT)
#error XCP_TP_CONFIGURATION_DONE_BIT already defined
#endif
/** \brief Bit position indicating that the a TP command already configured PDU related information
 * (e.g. DAQ list accepted by this PUD). */
#define XCP_TP_CONFIGURATION_DONE_BIT                       0x2U

#if (defined XCP_MULTICAST_TRANSMIT_MODE_BIT)
#error XCP_MULTICAST_TRANSMIT_MODE_BIT already defined
#endif
/** \brief Bit position indicating that the transmission is in multicast mode. */
#define XCP_MULTICAST_TRANSMIT_MODE_BIT                     0x3U

#if (defined XCP_INVALID_CONNECTION_ID)
#error XCP_INVALID_CONNECTION_ID already defined
#endif
/** \brief Invalid connection ID. */
#define XCP_INVALID_CONNECTION_ID                           0xFFU


#if (defined XCP_IGNORED_PDU_ID)
#error XCP_IGNORED_PDU_ID already defined
#endif
/** \brief Ignored PDU ID. */
#define XCP_IGNORED_PDU_ID                                  0xFFFFU

#if (defined XCP_IGNORED_CAN_ID)
#error XCP_IGNORED_CAN_ID already defined
#endif
/** \brief Ignored CAN ID. */
#define XCP_IGNORED_CAN_ID                                  0xFFFFU

#if (defined XCP_IGNORED_NAX_ID)
#error XCP_IGNORED_NAX_ID already defined
#endif
/** \brief Ignored FlexRay network address ID. */
#define XCP_IGNORED_NAX_ID                                  0xFFU

#if (defined XCP_IGNORED_PACKAGE_ALIGNMENT)
#error XCP_IGNORED_PACKAGE_ALIGNMENT already defined
#endif
/** \brief Ignored FlexRay network address ID. */
#define XCP_IGNORED_PACKAGE_ALIGNMENT                       0xFFU

#if (defined XCP_IGNORED_CNT_ID)
#error XCP_IGNORED_CNT_ID already defined
#endif
/** \brief Ignored counter ID. */
#define XCP_IGNORED_CNT_ID                              0xFFU

#if (defined XCP_IGNORED_SOCON_ID)
#error XCP_IGNORED_SOCON_ID already defined
#endif
/** \brief Ignored Socket connection ID. */
#define XCP_IGNORED_SOCON_ID                                0xFFFFU

#if (defined XCP_CONNECTION_OVER_CAN)
#error XCP_CONNECTION_OVER_CAN already defined
#endif
/** \brief The type of connection is over CAN. */
#define XCP_CONNECTION_OVER_CAN                             0U

#if (defined XCP_CONNECTION_OVER_FLEXRAY)
#error XCP_CONNECTION_OVER_FLEXRAY already defined
#endif
/** \brief The type of connection is over FlexRay. */
#define XCP_CONNECTION_OVER_FLEXRAY                         1U

#if (defined XCP_CONNECTION_OVER_TCPIP)
#error XCP_CONNECTION_OVER_TCPIP already defined
#endif
/** \brief The type of connection is over TcpIp. */
#define XCP_CONNECTION_OVER_TCPIP                           2U

#if (defined XCP_CONNECTION_OVER_UDPIP)
#error XCP_CONNECTION_OVER_UDPIP already defined
#endif
/** \brief The type of connection is over UdpIp. */
#define XCP_CONNECTION_OVER_UDPIP                           3U

#if (defined XCP_CONNECTION_OVER_CANFD)
#error XCP_CONNECTION_OVER_CANFD already defined
#endif
/** \brief The type of connection is over CANFD. */
#define XCP_CONNECTION_OVER_CANFD                           4U

#if (defined XCP_CONNECTION_OVER_CDD)
#error XCP_CONNECTION_OVER_CDD already defined
#endif
/** \brief The type of connection is over a CDD. */
#define XCP_CONNECTION_OVER_CDD                             5U

#if (defined XCP_MEMORY_AREA_TYPE_READ_WRITE)
#error XCP_MEMORY_AREA_TYPE_READ_WRITE already defined
#endif
/** \brief The memory area can accept both read and write accesses. */
#define XCP_MEMORY_AREA_TYPE_READ_WRITE                     0U

#if (defined XCP_INFINITE_RETRIES)
#error XCP_INFINITE_RETRIES already defined
#endif
/** \brief Magic number for infinite number of Tx retries. */
#define XCP_INFINITE_RETRIES                                255U


#if (defined XCP_MEMORY_AREA_TYPE_READ)
#error XCP_MEMORY_AREA_TYPE_READ already defined
#endif
/** \brief The memory area can accept only read accesses. */
#define XCP_MEMORY_AREA_TYPE_READ                           1U

#if (defined XCP_MEMORY_AREA_TYPE_WRITE)
#error XCP_MEMORY_AREA_TYPE_WRITE already defined
#endif
/** \brief The memory area can accept only write accesses. */
#define XCP_MEMORY_AREA_TYPE_WRITE                          2U

#if (defined XCP_MEMORY_AREA_SCOPE_ALL)
#error XCP_MEMORY_AREA_SCOPE_ALL already defined
#endif
/** \brief The memory area can accept all requests. */
#define XCP_MEMORY_AREA_SCOPE_ALL                           0U

#if (defined XCP_MEMORY_AREA_SCOPE_CALIBRATION)
#error XCP_MEMORY_AREA_SCOPE_CALIBRATION already defined
#endif
/** \brief The memory area can accept only calibration requests. */
#define XCP_MEMORY_AREA_SCOPE_CALIBRATION                   1U

#if (defined XCP_MEMORY_AREA_SCOPE_DAQ)
#error XCP_MEMORY_AREA_SCOPE_DAQ already defined
#endif
/** \brief The memory area can accept only DAQ or STIM requests. */
#define XCP_MEMORY_AREA_SCOPE_DAQ_STIM                      2U


/*------------------[Function like macros]----------------------------------------------*/

#if (defined XCP_GET_DAQLIST_MODE)
#error XCP_GET_DAQLIST_MODE already defined
#endif
/** \brief Returns current mode of the specified DAQ List. */
#define XCP_GET_DAQLIST_MODE(ListParamPtr) ((ListParamPtr)->Mode)

#if (defined XCP_GET_DAQLIST_EVENT_CHANNEL)
#error XCP_GET_DAQLIST_EVENT_CHANNEL already defined
#endif
/** \brief Returns event channel to which specified DAQ List is associated. */
#define XCP_GET_DAQLIST_EVENT_CHANNEL(ListParamPtr) ((ListParamPtr)->EventId)

#if (defined XCP_SET_DAQLIST_EVENT_CHANNEL)
#error XCP_SET_DAQLIST_EVENT_CHANNEL already defined
#endif
/** \brief Sets the event channel to which specified DAQ List is associated. */
#define XCP_SET_DAQLIST_EVENT_CHANNEL(ListParamPtr, EventChannel) \
                                      (ListParamPtr)->EventId = (EventChannel)

#if (defined XCP_GET_DAQLIST_PRESCALER)
#error XCP_GET_DAQLIST_PRESCALER already defined
#endif
/** \brief Returns the prescaler of the specified DAQ List. */
#define XCP_GET_DAQLIST_PRESCALER(ListParamPtr) ((ListParamPtr)->Prescaler)

#if (defined XCP_SET_DAQLIST_PRESCALER)
#error XCP_SET_DAQLIST_PRESCALER already defined
#endif
/** \brief Sets the prescaler of the specified DAQ List. */
#define XCP_SET_DAQLIST_PRESCALER(ListParamPtr, NewPrescaler)\
                                           (ListParamPtr)->Prescaler = (NewPrescaler)

#if (defined XCP_SET_DAQLIST_PRESCALERCNT)
#error XCP_SET_DAQLIST_PRESCALERCNT already defined
#endif
/** \brief Sets the prescaler count of the specified DAQ List. */
#define XCP_SET_DAQLIST_PRESCALERCNT(ListParamPtr, Prescaler)\
                                        ((ListParamPtr)->PrescalerCnt = (Prescaler))

#if (defined XCP_GET_DAQLIST_PRIORITY)
#error XCP_GET_DAQLIST_PRIORITY already defined
#endif
/** \brief Returns the priority of the specified DAQ List. */
#define XCP_GET_DAQLIST_PRIORITY(ListParamPtr) ((ListParamPtr)->Priority)

#if (defined XCP_SET_DAQLIST_PRIORITY)
#error XCP_SET_DAQLIST_PRIORITY already defined
#endif
/** \brief Sets the priority of the specified DAQ List. */
#define XCP_SET_DAQLIST_PRIORITY(ListParamPtr, NewPriority) \
                                             ((ListParamPtr)->Priority = (NewPriority))

#if (defined XCP_GET_EVENT_PROPERTIES)
#error XCP_GET_EVENT_PROPERTIES already defined
#endif
/** \brief Returns specified event channel properties. */
#define XCP_GET_EVENT_PROPERTIES(EventCfgPtr) ((EventCfgPtr)->Flag)

#if (defined XCP_IS_EVENT_CHANNEL_AVAILABLE)
#error XCP_IS_EVENT_CHANNEL_AVAILABLE already defined
#endif
/** \brief Returns whether event channel is available. */
#define XCP_IS_EVENT_CHANNEL_AVAILABLE(EventCfgPtr) \
                  (((EventCfgPtr)->MaxDaqList == 0U)?FALSE:TRUE)

#if (defined XCP_IS_EVENT_DIRECTION)
#error XCP_IS_EVENT_DIRECTION already defined
#endif
/** \brief Checks whether the event channel supports specified direction.
*/
#define XCP_IS_EVENT_DIRECTION(EventCfgPtr, Direction) \
            ((((EventCfgPtr)->Flag & (Direction)) == (Direction))? TRUE:FALSE)


#if (defined XCP_GET_EVENT_MAX_DAQ)
#error XCP_GET_EVENT_MAX_DAQ already defined
#endif
/** \brief Returns maximum number of DAQ lists associated with the specified
 *   event channel.
 */
#define XCP_GET_EVENT_MAX_DAQ(EventCfgPtr) ((EventCfgPtr)->MaxDaqList)

#if (defined XCP_GET_EVENT_PRIORITY)
#error XCP_GET_EVENT_PRIORITY already defined
#endif
/** \brief Returns priority of the specified event channel. */
#define XCP_GET_EVENT_PRIORITY(EventCfgPtr) ((EventCfgPtr)->Priority)

#if (defined COMMAND_CAN_MODIFY_DAQ_LIST)
#error COMMAND_CAN_MODIFY_DAQ_LIST already defined
#endif
/** \brief Return TRUE if the command can modify a DAQ list */
#define COMMAND_CAN_MODIFY_DAQ_LIST(command) \
((((command) == XCP_CMD_WRITE_DAQ_PID) || \
  ((command) == XCP_CMD_SET_DAQ_LIST_MODE_PID) || \
  ((command) == XCP_CMD_START_STOP_DAQ_LIST_PID) || \
  ((command) == XCP_CMD_START_STOP_SYNCH_PID) || \
  ((command) == XCP_CMD_WRITE_DAQ_MULTIPLE_PID) || \
  ((command) == XCP_CMD_CLEAR_DAQ_LIST_PID) || \
  ((command) == XCP_CMD_FREE_DAQ_PID) || \
  ((command) == XCP_CMD_ALLOC_DAQ_PID) || \
  ((command) == XCP_CMD_ALLOC_ODT_PID) || \
  ((command) == XCP_CMD_ALLOC_ODT_ENTRY_PID) || \
  ((command) == XCP_CMD_DISCONNECT_PID) \
  ) ? TRUE:FALSE)

#if (defined COMMAND_CAN_EXECUTE_PGM_START)
#error COMMAND_CAN_EXECUTE_PGM_START already defined
#endif
/** \brief Return TRUE if the command is allowed during a programming session. Basically all commands
 * which are accepted during the programming of a section + PROGRAM_RESET, PROGRAM_CLEAR, SET_MTA,
 * UPLOAD and BUILD_CHECKSUM.
 * SYNCH is also allowed but handled differently. */
#define COMMAND_CAN_EXECUTE_PGM_START(command) \
((((command) == XCP_CMD_PROGRAM_PID) || \
  ((command) == XCP_CMD_PROGRAM_NEXT_PID) || \
  ((command) == XCP_CMD_PROGRAM_MAX_PID) || \
  ((command) == XCP_CMD_PROGRAM_CLEAR_PID) || \
  ((command) == XCP_CMD_CONNECT_PID) || \
  ((command) == XCP_CMD_GET_COMM_MODE_INFO_PID) || \
  ((command) == XCP_CMD_GETID_PID) || \
  ((command) == XCP_CMD_GET_STATUS_PID) || \
  ((command) == XCP_CMD_SET_MTA_PID) || \
  ((command) == XCP_CMD_UPLOAD_PID) || \
  ((command) == XCP_CMD_BUILD_CHECKSUM_PID) || \
  ((command) == XCP_CMD_PROGRAM_RESET_PID) || \
  ((command) == XCP_CMD_GET_PGM_PROC_INFO_PID) || \
  ((command) == XCP_CMD_GET_SECTOR_INFO_PID) \
  ) ? TRUE:FALSE)

#if (defined COMMAND_CAN_EXECUTE_PGM_ACTIVE)
#error COMMAND_CAN_EXECUTE_PGM_ACTIVE already defined
#endif
/** \brief Return TRUE if the command is allowed during the programming of a section */
#define COMMAND_CAN_EXECUTE_PGM_ACTIVE(command) \
((((command) == XCP_CMD_PROGRAM_PID) || \
  ((command) == XCP_CMD_PROGRAM_NEXT_PID) || \
  ((command) == XCP_CMD_PROGRAM_MAX_PID) || \
  ((command) == XCP_CMD_CONNECT_PID) || \
  ((command) == XCP_CMD_GET_COMM_MODE_INFO_PID) || \
  ((command) == XCP_CMD_GETID_PID) || \
  ((command) == XCP_CMD_GET_STATUS_PID) || \
  ((command) == XCP_CMD_GET_PGM_PROC_INFO_PID) \
  ) ? TRUE:FALSE)

#if (defined XCP_SET_DAQLIST_STOPPED)
#error XCP_SET_DAQLIST_STOPPED already defined
#endif
/** \brief Stop specified DAQ list. */
#define XCP_SET_DAQLIST_STOPPED(ListParamPtr)  \
                  ((ListParamPtr)->Mode &= (uint8)(~XCP_MASK_DAQLIST_RUNNING))

#if (defined XCP_SET_DAQLIST_RUNNING)
#error XCP_SET_DAQLIST_RUNNING already defined
#endif
/** \brief Start specified DAQ list. */
#define XCP_SET_DAQLIST_RUNNING(ListParamPtr) \
                    ((ListParamPtr)->Mode |= (uint8)XCP_MASK_DAQLIST_RUNNING)

#if (defined XCP_SET_DAQLIST_SELECTED)
#error XCP_SET_DAQLIST_SELECTED already defined
#endif
/** \brief Select specified DAQ list. */
#define XCP_SET_DAQLIST_SELECTED(ListParamPtr) \
                    ((ListParamPtr)->Mode |= (uint8)XCP_MASK_DAQLIST_SELECTED)

#if (defined XCP_SET_DAQLIST_CONFIGURED)
#error XCP_SET_DAQLIST_CONFIGURED already defined
#endif
/** \brief Set specified DAQ list as configured. */
#define XCP_SET_DAQLIST_CONFIGURED(ListParamPtr) \
                     ((ListParamPtr)->Flags |= (uint8)XCP_MASK_DAQLIST_CONFIGURED)

#if (defined XCP_IS_DAQ_LIST_RUNNING)
#error XCP_IS_DAQ_LIST_RUNNING already defined
#endif
/** \brief Check whether DAQ List is Running. */
#define XCP_IS_DAQ_LIST_RUNNING(ListParamPtr) \
          (((uint8)((ListParamPtr)->Mode & XCP_MASK_DAQLIST_RUNNING) == 0U)?\
            FALSE:TRUE \
          )

#if (defined XCP_IS_DAQ_LIST_SELECTED)
#error XCP_IS_DAQ_LIST_SELECTED already defined
#endif
/** \brief Check whether DAQ List is Selected. */
#define XCP_IS_DAQ_LIST_SELECTED(ListParamPtr) \
          ((((uint8)(ListParamPtr)->Mode & XCP_MASK_DAQLIST_SELECTED) == 0U)?\
            FALSE:TRUE \
          )

#if (defined XCP_GET_SESSION_STATUS)
#error XCP_GET_SESSION_STATUS already defined
#endif
/** \brief Returns the current status of the Xcp slave. */
#define XCP_GET_SESSION_STATUS()       (Xcp_Status.SessionStatus)

#if (defined XCP_GET_RESOURCE_PROTECTION_STATUS)
#error XCP_GET_RESOURCE_PROTECTION_STATUS already defined
#endif
/** \brief Returns the current status of resource protection. */
#define XCP_GET_RESOURCE_PROTECTION_STATUS() (Xcp_Status.ProtectionStatus)

#if (defined XCP_SET_STORE_CAL_REQ)
#error XCP_SET_STORE_CAL_REQ already defined
#endif
/** \brief Set request to save the calibration data into non-volatile memory. */
#define XCP_SET_STORE_CAL_REQ() \
              (Xcp_Status.SessionStatus |= (uint8)XCP_MASK_STORE_CAL_REQ)

#if (defined XCP_CLEAR_STORE_CAL_REQ)
#error XCP_CLEAR_STORE_CAL_REQ already defined
#endif
/** \brief Clear request to save the calibration data into non-volatile memory. */
#define XCP_CLEAR_STORE_CAL_REQ() \
              (Xcp_Status.SessionStatus &= (uint8)(~XCP_MASK_STORE_CAL_REQ))

#if (defined XCP_SET_STORE_DAQ_REQ)
#error XCP_SET_STORE_DAQ_REQ already defined
#endif
/** \brief Set request to save the DAQ list setup into non-volatile memory. */
#define XCP_SET_STORE_DAQ_REQ() \
              (Xcp_Status.SessionStatus |= (uint8)XCP_MASK_STORE_DAQ_REQ)

#if (defined XCP_SET_XCP_STATUS_DAQ_RUNNING)
#error XCP_SET_XCP_STATUS_DAQ_RUNNING already defined
#endif
/** \brief Update slave status to indicate that at least one DAQ list has been
 *  started and is in RUNNING mode. */
#define XCP_SET_XCP_STATUS_DAQ_RUNNING() \
              (Xcp_Status.SessionStatus |= (uint8)XCP_MASK_DAQ_RUNNING)

#if (defined XCP_CLEAR_XCP_STATUS_DAQ_RUNNING)
#error XCP_CLEAR_XCP_STATUS_DAQ_RUNNING already defined
#endif
/** \brief Update slave status to indicate that all DAQ List has been
 * stopped */
#define XCP_CLEAR_XCP_STATUS_DAQ_RUNNING() \
              (Xcp_Status.SessionStatus &= (uint8)(~XCP_MASK_DAQ_RUNNING))


#if (defined XCP_CLEAR_DAQLIST_SELECTED)
#error XCP_CLEAR_DAQLIST_SELECTED already defined
#endif
/** \brief clear select status of specified DAQ list. */
#define XCP_CLEAR_DAQLIST_SELECTED(ListParamPtr) \
                    ((ListParamPtr)->Mode &= (uint8)(~XCP_MASK_DAQLIST_SELECTED))

#if (defined XCP_GET_NO_OF_DAQLISTS_RUNNING)
#error XCP_GET_NO_OF_DAQLISTS_RUNNING already defined
#endif
/** \brief Returns number of DAQ Lists running. */
#define XCP_GET_NO_OF_DAQLISTS_RUNNING() (Xcp_Status.NoOfRunningDaq)

#if (defined XCP_GET_NO_OF_DAQLISTS_SELECTED)
#error XCP_GET_NO_OF_DAQLISTS_SELECTED already defined
#endif
/** \brief Returns number of DAQ Lists selected. */
#define XCP_GET_NO_OF_DAQLISTS_SELECTED() (Xcp_Status.NoOfSelectedDaq)

#if (defined XCP_PID_IS_CMD)
#error XCP_PID_IS_CMD already defined
#endif
/** \brief Check whether the PID indicates CMD packet */
#define XCP_PID_IS_CMD(Pid) ((Pid) >= XCP_CMD_PID_BASE)

#if (defined XCP_PID_IS_SYNCH)
#error XCP_PID_IS_SYNCH already defined
#endif
/** \brief Check whether the PID indicates DOWNLOAD command packet */
#define XCP_PID_IS_SYNCH(Pid) ((Pid) == XCP_CMD_SYNCH_PID)

#if (defined XCP_PID_IS_DISCONNECT)
#error XCP_PID_IS_DISCONNECT already defined
#endif
/** \brief Check whether the PID indicates DISCONNECT command packet */
#define XCP_PID_IS_DISCONNECT(Pid) ((Pid) == XCP_CMD_DISCONNECT_PID)

#if (defined XCP_PID_IS_CONNECT)
#error XCP_PID_IS_CONNECT already defined
#endif
/** \brief Check whether the PID indicates CONNECT command packet */
#define XCP_PID_IS_CONNECT(Pid) ((Pid) == XCP_CMD_CONNECT_PID)

#if (defined XCP_PID_IS_TRANSPORT_LAYER)
#error XCP_PID_IS_TRANSPORT_LAYER already defined
#endif
/** \brief Check whether the PID indicates XCP_PID_IS_TRANSPORT_LAYER command packet */
#define XCP_PID_IS_TRANSPORT_LAYER(Pid) ((Pid) == XCP_CMD_TRANSPORT_LAYER_PID)

#if (defined XCP_SUBPID_IS_GET_SLAVE_ID)
#error XCP_SUBPID_IS_GET_SLAVE_ID already defined
#endif
/** \brief Check whether the Sub PID represents the ID of GET_SLAVE_ID */
#define XCP_SUBPID_IS_GET_SLAVE_ID(SubPid) ((SubPid) == XCP_SUBCMD_GET_SLAVE_ID_PID)

/** \brief Calculate maximum of two values
 **
 ** \param[in] a First value
 ** \param[in] b Second value
 **
 ** \return Maximum of a and b.
 */
#if (defined XCP_MAX)
#error XCP_MAX already defined
#endif
#define XCP_MAX( a, b )   ( ((a) >= (b)) ? (a) : (b) )

/** \brief Calculate minimum of two values
 **
 ** \param[in] a First value
 ** \param[in] b Second value
 **
 ** \return Minimum of a and b.
 */
#if (defined XCP_MIN)
#error XCP_MIN already defined
#endif
#define XCP_MIN( a, b )   ( ((a) <= (b)) ? (a) : (b) )

/*==================[type definitions]======================================*/

/** \brief Xcp internal error type */
typedef uint8 Xcp_ErrorType;

/** \brief Xcp Connection State type definition */
typedef uint8 Xcp_StateType;

/** \brief Xcp Connection Status type definition */
typedef struct
{
  Xcp_StateType State;            /* connection state */
  uint8 ConnectionId;             /* the ID of the selected connection */
  uint8 Flags;                    /* Connection related flags:
                                   * BIT7: RESERVED,
                                   * BIT6: RESERVED,
                                   * BIT5: RESERVED,
                                   * BIT4: RESERVED,
                                   * BIT3: TX is in Multicast mode,
                                   * BIT2: TP related information in the PDUs was configured (e.g. STIM packet is allowed),
                                   * BIT1: Flexray Hardware buffers are initialized,
                                   * BIT0: Close Ethernet connection */
}Xcp_ConnectionStatusType;

/*==================[external function declarations]=========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

/** \brief Function to initialize command processor
 **
 ** This function Initializes all variables used by
 ** command processor. */
extern FUNC(void, XCP_CODE) Xcp_InitCommandProcessor(void);

/** \brief Function to initialize transmit processor
 **
 ** This function Initializes all variables used by
 ** transmit processor. */
extern FUNC(void, XCP_CODE) Xcp_InitTransmitProcessor(void);

/** \brief Function to initialize receive processor
 **
 ** This function Initializes all variables used by
 ** receive processor. */
extern FUNC(void, XCP_CODE) Xcp_InitReceiveProcessor(void);

/** \brief Function to queue a received command into command queue
 **
 ** Use this function from Rx indication function up on reception
 ** of a command other than SYNCH.
 ** \param[in]      SduLength Command length to be queued.
 ** \param[in]      SduDataPtr Command CTO data to be queued. */
extern FUNC(void, XCP_CODE) Xcp_InsertCommand
(
  PduLengthType SduLength,
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) SduDataPtr
);

/** \brief Function to queue a received SYNCH command
 **
 ** Use this function from Rx indication function up on reception
 ** of a SYNCH command. */
extern FUNC(void, XCP_CODE) Xcp_SetSynchCmdPending(void);

/** \brief Function to queue a received DISCONNECT command
 **
 ** Use this function from Rx indication function up on reception
 ** of a DISCONNECT command. */
extern FUNC(void, XCP_CODE) Xcp_SetDisconnectCmdPending(void);

/** \brief Function to return whether the command queue is empty or not
 ** \retval  TRUE if command queue is empty
 ** \retval  FALSE if command queue is not empty
 ** \return whether the command queue is empty or not
 **  */
extern FUNC(boolean, XCP_CODE) Xcp_CommandQueueIsEmpty(void);

/** \brief Function to return whether the command queue is full or not
 ** \retval  TRUE if command queue is full
 ** \retval  FALSE if command queue is not full
 ** \return whether the command queue is full or not
 **  */
extern FUNC(boolean, XCP_CODE) Xcp_CommandQueueIsFull(void);

/** \brief Function to return whether the command queue is full or not
 ** \retval  TRUE if CTO queue is full
 ** \retval  FALSE if CTO queue is not full
 ** \return whether the CTO queue is full or not
 **  */
extern FUNC(boolean, XCP_CODE) Xcp_IsCtoQueueEmpty(void);

/** \brief Function to get the size of the next CTO to send.
 **
 ** \return The size of next command response CTO in Event CTO queue */
extern FUNC(PduLengthType, XCP_CODE) Xcp_GetCtoLength(void);

/** \brief Function to Dequeue one CTO from the response CTO Queue.
 **
 ** Use this function to dequeue one CTO from the response CTO queue.  CTO
 ** data bytes will be copied byte by byte from queue head.  Queue head will
 ** be updated to the next CTO length information in the queue and rolled over
 ** if necessary.  Number of bytes in queue will also be updated.
 **
 ** \param[in] DataLength    How much data shall be dequeued
 ** \param[in] BufferPtr     Buffer to which CTO shall be dequeued */
extern FUNC(void, XCP_CODE) Xcp_DeQueueCtoQueue
(
  PduLengthType DataLength,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) BufferPtr
);

/** \brief Function to get the CTO length corresponding to a Command.
 **
 ** Use this function to get the base length of the CTO that was received.
 **
 ** \param[in]  XcpRxSduPtr Pointer to the CTO data
 ** \param[out] DataLengthMin Pointer to the calculated minimum data length
 ** \param[out] DataLengthMax Pointer to the calculated maximum data length
 **/
extern FUNC(void, XCP_CODE) Xcp_GetCmdLength
(
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) XcpRxSduPtr,
  P2VAR(PduLengthType, AUTOMATIC, XCP_APPL_DATA) DataLengthMin,
  P2VAR(PduLengthType, AUTOMATIC, XCP_APPL_DATA) DataLengthMax
);

/** \brief Function to send ERR_CMD_SYNTAX response packet.
 **
 ** This function is called by Xcp_ProcessRxPdu() function only,
 ** the function call being critical section protected.
 **
 ** \param[in] Pid Package Id
 ** \param[in] ErrId the Id of the reported error
 ** */
extern FUNC(void, XCP_CODE) Xcp_SendCmdErrorPacket
(
  uint8 Pid,
  uint8 ErrId
);

/** \brief Function to check if the command is implemented.
 **
 ** This function is called by Xcp_ProcessRxPdu() function only,
 ** the function call being critical section protected.
 **
 ** \param[in] Pid the Id of the reported error
 ** */
extern FUNC (boolean, XCP_CODE) Xcp_IsCmdImplemented(uint8 Pid);

/** \brief Function to Process transmission
 **
 ** This function implements the transmission of outgoing PDUs.
 ** This function shall be invoked cyclically.
 **
 ** \param[in]  SidId The Function Id from where the function is called
 **/
extern FUNC(void, XCP_CODE) Xcp_ProcessTransmit
(
  uint8 SidId
);

/** \brief Process the Bus Timeout monitor which releases the I'th level of
 **        connection resources, in case multiple connection are configured.
 **        This function has to be called at the beginning of the Xcp_MainFunction execution
 **
 ** \param[out] IsDisconnectTriggered value indicating whether a disconnect has been triggered
 **/
extern FUNC(void, XCP_CODE) Xcp_ProcessBusMonitor
(
  P2VAR(boolean, AUTOMATIC, XCP_APPL_DATA) IsDisconnectTriggered
);

/** \brief Function to copy a 32 bit data into byte array
 **
 ** Use this function to copy specified 32 bit data to
 ** specified destination.
 **
 ** \param[in]      Dword       Data source
 ** \param[out]     DesPtr      Address of the data destination */
extern FUNC(void, XCP_CODE) Xcp_WriteDwordToByteArray
(
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) DesPtr,
  uint32 Dword
);

/** \brief Function to copy a 16 bit data into byte array
 **
 ** Use this function to copy specified 16 bit data to
 ** specified destination.
 **
 ** \param[in]      Word       Data source
 ** \param[out]     DesPtr     Address of the data destination */
extern FUNC(void, XCP_CODE) Xcp_WriteWordToByteArray
(
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) DesPtr,
  uint16 Word
);

/** \brief Function to abort the current busy command
 ** Use this function in case the master sends an unexpected command
 ** during a slave block mode transfer.
 */
extern FUNC(void, XCP_CODE) Xcp_AbortBusyCommand(void);

/** \brief Function to flush all pending commands
 **
 ** Use this function to flush all pending commands.
 ** Queue head, tail and number of elements will be reset
 ** to 0x00U.
 ** The uploaded bytes counter is also reseted.
 ** */
extern FUNC(void, XCP_CODE) Xcp_FlushCMDQueue(void);

/** \brief Function to flush all pending response CTOs
 **
 ** Use this function to flush all pending response CTOs.
 ** Queue head, tail and number of elements will be reset
 ** to 0x00U. */
extern FUNC(void, XCP_CODE) Xcp_FlushCTOQueue(void);

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

/*==================[external data]=========================================*/

#define XCP_START_SEC_VAR_8
#include <Xcp_MemMap.h>

/** \brief Initialization status of the XCP module */
extern VAR(boolean, XCP_VAR) Xcp_Initialized;

#define XCP_STOP_SEC_VAR_8
#include <Xcp_MemMap.h>

#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

/** \brief Connection status of XCP module. */
extern VAR(Xcp_ConnectionStatusType, XCP_VAR) Xcp_ConnectionStatus;

#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

/*==================[end of file]===========================================*/

#endif /* ifndef XCP_INT_STC_H */
