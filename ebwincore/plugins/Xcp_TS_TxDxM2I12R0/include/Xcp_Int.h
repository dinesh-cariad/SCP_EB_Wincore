#ifndef XCP_INT_H
#define XCP_INT_H

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
/*
 *  MISRA-C:2012 Deviation List
 *
 */
/*==================[inclusions]============================================*/
#include <Xcp_Cfg.h>            /* XCP configuration header */
#include <Xcp_Int_Cfg.h>        /* Internal configuration defines and declarations */
#include <Xcp_DefProg_Cfg.h>    /* Configuration defines related to defensive programming */
#include <Xcp_Cbk.h>            /* Callback function */
#if ((XCP_BSW_DISTRIBUTION_ENABLED == STD_ON) && ((XCP_MEMORY_ACCESS_AREAS == STD_ON) || (XCP_REMOTE_DESTINATIONS == STD_ON)))
#include <Rte_Type.h>
#endif
#if (XCP_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON)
#include <TcpIp.h>        /* TcpIp types */
#endif

#if((XCP_PROD_ERR_HANDLING_PDU_LOST == TS_PROD_ERR_REP_TO_DEM)||\
    (XCP_PROD_ERR_HANDLING_PDU_BUFFER_FULL ==TS_PROD_ERR_REP_TO_DEM)||\
    (XCP_PROD_ERR_HANDLING_PDU_BUFFER_LOCKED ==TS_PROD_ERR_REP_TO_DEM)||\
    (XCP_PROD_ERR_HANDLING_RESP_CTO_QUEUE_FULL == TS_PROD_ERR_REP_TO_DEM)||\
    (XCP_PROD_ERR_HANDLING_STIMULATION_DATA_LOST == TS_PROD_ERR_REP_TO_DEM)||\
    (XCP_PROD_ERR_HANDLING_ILLEGAL_MEMORY_ACCESS == TS_PROD_ERR_REP_TO_DEM)||\
    (XCP_PROD_ERR_HANDLING_ACQUISITION_DATA_LOST == TS_PROD_ERR_REP_TO_DEM))
#include <Dem.h>                 /* Dem API*/
#endif
#if((XCP_PROD_ERR_HANDLING_PDU_LOST == TS_PROD_ERR_REP_TO_DET)||\
    (XCP_PROD_ERR_HANDLING_PDU_BUFFER_FULL ==TS_PROD_ERR_REP_TO_DET)||\
    (XCP_PROD_ERR_HANDLING_PDU_BUFFER_LOCKED ==TS_PROD_ERR_REP_TO_DET)||\
    (XCP_PROD_ERR_HANDLING_RESP_CTO_QUEUE_FULL == TS_PROD_ERR_REP_TO_DET)||\
    (XCP_PROD_ERR_HANDLING_STIMULATION_DATA_LOST ==TS_PROD_ERR_REP_TO_DET)||\
    (XCP_PROD_ERR_HANDLING_ILLEGAL_MEMORY_ACCESS ==TS_PROD_ERR_REP_TO_DET)||\
    (XCP_PROD_ERR_HANDLING_ACQUISITION_DATA_LOST == TS_PROD_ERR_REP_TO_DET)||\
    (XCP_DEV_ERROR_DETECT == STD_ON) ||\
    (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON))
 #include <Det.h>                 /* Det API*/
#endif

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
#include <Atomics.h>
#endif

/*==================[macros]================================================*/





/*------------------------[Defensive programming]----------------------------*/

#if (defined XCP_PRECONDITION_ASSERT)
#error XCP_PRECONDITION_ASSERT is already defined
#endif

#if (defined XCP_PRECONDITION_ASSERT_NO_EVAL)
#error XCP_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (XCP_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define XCP_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), XCP_MODULE_ID, XCP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define XCP_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), XCP_MODULE_ID, XCP_INSTANCE_ID, (ApiId))
#else
#define XCP_PRECONDITION_ASSERT(Condition, ApiId)
#define XCP_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined XCP_POSTCONDITION_ASSERT)
#error XCP_POSTCONDITION_ASSERT is already defined
#endif

#if (defined XCP_POSTCONDITION_ASSERT_NO_EVAL)
#error XCP_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (XCP_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define XCP_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), XCP_MODULE_ID, XCP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define XCP_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), XCP_MODULE_ID, XCP_INSTANCE_ID, (ApiId))
#else
#define XCP_POSTCONDITION_ASSERT(Condition, ApiId)
#define XCP_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined XCP_INVARIANT_ASSERT)
#error XCP_INVARIANT_ASSERT is already defined
#endif

#if (defined XCP_INVARIANT_ASSERT_NO_EVAL)
#error XCP_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (XCP_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define XCP_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), XCP_MODULE_ID, XCP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define XCP_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), XCP_MODULE_ID, XCP_INSTANCE_ID, (ApiId))
#else
#define XCP_INVARIANT_ASSERT(Condition, ApiId)
#define XCP_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined XCP_STATIC_ASSERT)
# error XCP_STATIC_ASSERT is already defined
#endif
#if (XCP_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define XCP_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define XCP_STATIC_ASSERT(expr)
#endif

#if (defined XCP_UNREACHABLE_CODE_ASSERT)
#error XCP_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (XCP_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define XCP_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(XCP_MODULE_ID, XCP_INSTANCE_ID, (ApiId))
#else
#define XCP_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined XCP_INTERNAL_API_ID)
#error XCP_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define XCP_INTERNAL_API_ID DET_INTERNAL_API_ID



/*------------------[Bit Masks]----------------------------------------------*/

#if (defined XCP_BYTE_ORDER)
#error XCP_BYTE_ORDER already defined
#endif
/** \brief Definition of value indicating the byte order used for transferring
 ** multi-byte parameters in an XCP Packet. */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#define XCP_BYTE_ORDER               XCP_LITTLE_ENDIAN_MASK
#else
#define XCP_BYTE_ORDER               XCP_BIG_ENDIAN_MASK
#endif

#if (XCP_MAX_DAQ > 0)

#if (defined XCP_MASK_DAQ_DIRECTION_STIM)
#error XCP_MASK_DAQ_DIRECTION_STIM already defined
#endif
/** \brief Mask to check whether DAQ direction is STIM */
#define XCP_MASK_DAQ_DIRECTION_STIM                      0x08U

#if (defined XCP_MASK_DAQ_DIRECTION_DAQ)
#error XCP_MASK_DAQ_DIRECTION_DAQ already defined
#endif
/** \brief Mask to check whether DAQ direction is DAQ */
#define XCP_MASK_DAQ_DIRECTION_DAQ                       0x04U

#endif /* (XCP_MAX_DAQ > 0) */

#if (defined XCP_RESOURCE_SUPPORTED)
#error XCP_RESOURCE_SUPPORTED already defined
#endif
/** \brief Definition of supported resources by XCP module. */
#define XCP_RESOURCE_SUPPORTED     \
( XCP_RESOURCE_CAL_PAG | ( XCP_RESOURCE_DAQ | (XCP_RESOURCE_STIM | XCP_RESOURCE_PGM) ) )

#if (defined XCP_COMM_MODE_BASIC)
#error XCP_COMM_MODE_BASIC already defined
#endif
/** \brief Definition of value indicating Basic communication mode parameters of Xcp. */
#define XCP_COMM_MODE_BASIC            \
( XCP_BYTE_ORDER | ( XCP_ADDRESS_GRANULARITY | XCP_SLAVE_BLOCK_MODE ) )

#if (defined XCP_COMM_MODE_OPTIONAL)
#error XCP_COMM_MODE_OPTIONAL already defined
#endif
/** \brief Definition of value indicating communication mode information. */
#define XCP_COMM_MODE_OPTIONAL      XCP_MASTER_BLOCK_MODE

#if (defined XCP_COMM_MODE_PGM)
#error XCP_COMM_MODE_PGM already defined
#endif
/** \brief COMM_MODE_PGM for command PROGRAM_START. */
#define XCP_COMM_MODE_PGM           (XCP_MASTER_BLOCK_MODE_PGM | XCP_SLAVE_BLOCK_MODE)

#if (defined XCP_TIMESTAMP_MODE)
#error XCP_TIMESTAMP_MODE already defined
#endif
/** \brief Definition of value indicating Timestamp unit and size. */
#define XCP_TIMESTAMP_MODE          \
( XCP_TIMESTAMP_TYPE | ( XCP_TIMESTAMP_FIXED | XCP_TIMESTAMP_UNIT ) )

#if (defined XCP_RESUME_SUPPORTED)
#error XCP_RESUME_SUPPORTED already defined
#endif
/** \brief Definition of value indicating Resume Mode is supported. */
#if (XCP_STORE_DAQ_SUPPORTED == STD_ON && XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF)
#define XCP_RESUME_SUPPORTED           XCP_RESUME_SUPPORTED_MASK
#else
#define XCP_RESUME_SUPPORTED           0x00U
#endif

#if (defined XCP_DAQ_PROPERTIES)
#error XCP_DAQ_PROPERTIES already defined
#endif
/* !LINKSTO Xcp.ASAM.BITWISE_STIMULATION.001,1 */
/** \brief Definition of value indicating the general properties of DAQ
 ** processor. */
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF)
#define XCP_DAQ_PROPERTIES                     \
( XCP_DAQ_CONFIG_TYPE | ( XCP_PRESCALER_SUPPORTED | ( XCP_RESUME_SUPPORTED | \
  ( XCP_TIMESTAMP_SUPPORTED | ( XCP_OVERLOAD_INDICATION_TYPE | XCP_BIT_STIM_SUPPORTED ) ) ) ) )
#else
/* !LINKSTO Xcp.BswDistribution.Daq.GetDaqProcessorInfo.NoResume,1 */
#define XCP_DAQ_PROPERTIES                     \
( XCP_DAQ_CONFIG_TYPE | ( XCP_PRESCALER_SUPPORTED | ( XCP_TIMESTAMP_SUPPORTED | \
  ( XCP_OVERLOAD_INDICATION_TYPE | XCP_BIT_STIM_SUPPORTED ) ) ) )
#endif

#if (defined XCP_DAQ_KEY_BYTE)
#error XCP_DAQ_KEY_BYTE already defined
#endif
/** \brief Definition of value indicating the Daq List general properties . */
#define XCP_DAQ_KEY_BYTE                    \
( XCP_OPTIMISATION_TYPE | ( XCP_ADDRESS_EXTENSION_TYPE | XCP_IDENTIFICATION_FIELD_TYPE ) )

#if (defined XCP_MAX_CTO_FLX_ASSIGN)
#error XCP_MAX_CTO_FLX_ASSIGN already defined
#endif
/** \brief Maximum CTO length for FLX_ASSIGN command */
#define XCP_MAX_CTO_FLX_ASSIGN              12U


#if (defined XCP_MAX_CTO_PGM_ABS)
#error XCP_MAX_CTO_PGM_ABS already defined
#endif
/** \brief The absolute maximum CTO size defined by the maximum between MAX_CTO and MAX_CTO_PGM
  * If MAX_CTO_ABS is lower than 12 and FLX_ASSIGN subcommand is supported, adjust it to 12
  * to be able to accomodate FLX_ASSIGN reception
  */
#if (XCP_PGM_SUPPORTED == STD_ON)
#if (XCP_MAX_CTO >= XCP_MAX_CTO_PGM)
#define XCP_MAX_CTO_PGM_ABS          XCP_MAX_CTO
#else
#define XCP_MAX_CTO_PGM_ABS          XCP_MAX_CTO_PGM
#endif
#else /* programming not supported */
#define XCP_MAX_CTO_PGM_ABS            XCP_MAX_CTO
#endif /* XCP_PGM_SUPPORTED == STD_ON */

#if (defined XCP_MAX_CTO_ABS)
#error XCP_MAX_CTO_ABS already defined
#endif

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
#if (XCP_MAX_CTO_PGM_ABS > 12)
#define XCP_MAX_CTO_ABS        XCP_MAX_CTO_PGM_ABS
#else
#define XCP_MAX_CTO_ABS        XCP_MAX_CTO_FLX_ASSIGN
#endif
#elif (XCP_GET_MAX_OF_MAX_DLC > XCP_MAX_CTO_PGM_ABS)
#define XCP_MAX_CTO_ABS        XCP_GET_MAX_OF_MAX_DLC
#else
#define XCP_MAX_CTO_ABS        XCP_MAX_CTO_PGM_ABS
#endif

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
#if (defined XCP_MAX_SIZE_OF_REQUEST_BUFFER)
#error XCP_MAX_SIZE_OF_REQUEST_BUFFER already defined
#endif
/** \brief Maximum number of DTOs in all events */
#define XCP_MAX_SIZE_OF_REQUEST_BUFFER     (XCP_MAX_DAQ*XCP_MAX_ODT_IN_DAQ*XCP_MAX_ENTRIES_PER_ODT*(XCP_NUMBER_OF_QUEUED_EVENTS + 1U))

#if (defined XCP_MAX_SIZE_OF_DAQ_PRIO_LIST)
#error XCP_MAX_SIZE_OF_DAQ_PRIO_LIST already defined
#endif
/** \brief Maximum number of ODTs in all events */
#define XCP_MAX_SIZE_OF_DAQ_PRIO_LIST     (XCP_MAX_DAQ*XCP_MAX_ODT_IN_DAQ*(XCP_NUMBER_OF_QUEUED_EVENTS + 1U))
#endif /* XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON */

#if (defined XCP_CMD_UPLOAD_DATA_SOURCE)
#error XCP_CMD_UPLOAD_DATA_SOURCE already defined
#endif
#if (XCP_CALIBRATION_MEMORY_ACCESS_AREAS == STD_ON)
#define XCP_CMD_UPLOAD_DATA_SOURCE &(Xcp_MemoryProxyBuffer[Xcp_MemoryProxyBufferIndex])
#else
#define XCP_CMD_UPLOAD_DATA_SOURCE (XCP_APPL_GET_ADDRESS(0, Xcp_MTA.Address.Write))
#endif

/*------------------[Constants]----------------------------------------------*/

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)

/** \brief byte index of least significant byte in a uint16 value for little
 * endian architectures */
#if (defined XCP_UINT16_LSB_IDX)
#error XCP_UINT16_LSB_IDX already defined
#endif

#define XCP_UINT16_LSB_IDX 0U
/** \brief byte index of most significant byte in a uint16 value for little
 * endian architectures */
#if (defined XCP_UINT16_MSB_IDX)
#error XCP_UINT16_MSB_IDX already defined
#endif

#define XCP_UINT16_MSB_IDX 1U

/** \brief byte index of least significant byte in a uint32 value for little
 * endian architectures */
#if (defined XCP_UINT32_SB0_IDX)
#error XCP_UINT32_SB0_IDX already defined
#endif

#define XCP_UINT32_SB0_IDX 0U
/** \brief byte index of 2nd least significant byte in a uint32 value for
 * little endian architectures */
#if (defined XCP_UINT32_SB1_IDX)
#error XCP_UINT32_SB1_IDX already defined
#endif

#define XCP_UINT32_SB1_IDX 1U
/** \brief byte index of 3rd least significant byte in a uint32 value for
 * little endian architectures */
#if (defined XCP_UINT32_SB2_IDX)
#error XCP_UINT32_SB2_IDX already defined
#endif

#define XCP_UINT32_SB2_IDX 2U
/** \brief byte index of 4th least significant byte in a uint32 value for
 * little endian architectures */
#if (defined XCP_UINT32_SB3_IDX)
#error XCP_UINT32_SB3_IDX already defined
#endif

#define XCP_UINT32_SB3_IDX 3U

#else /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */

/** \brief byte index of least significant byte in a uint16 value for big
 * endian architectures */
#if (defined XCP_UINT16_LSB_IDX)
#error XCP_UINT16_LSB_IDX already defined
#endif

#define XCP_UINT16_LSB_IDX 1U
/** \brief byte index of most significant byte in a uint16 value for big
 * endian architectures */
#if (defined XCP_UINT16_MSB_IDX)
#error XCP_UINT16_MSB_IDX already defined
#endif

#define XCP_UINT16_MSB_IDX 0U

/** \brief byte index of least significant byte in a uint32 value for big
 * endian architectures */
#if (defined XCP_UINT32_SB0_IDX)
#error XCP_UINT32_SB0_IDX already defined
#endif

#define XCP_UINT32_SB0_IDX 3U
/** \brief byte index of 2nd least significant byte in a uint32 value for big
 * endian architectures */
#if (defined XCP_UINT32_SB1_IDX)
#error XCP_UINT32_SB1_IDX already defined
#endif

#define XCP_UINT32_SB1_IDX 2U
/** \brief byte index of 3rd least significant byte in a uint32 value for big
 * endian architectures */
#if (defined XCP_UINT32_SB2_IDX)
#error XCP_UINT32_SB2_IDX already defined
#endif

#define XCP_UINT32_SB2_IDX 1U
/** \brief byte index of 4th least significant byte in a uint32 value for big
 * endian architectures */
#if (defined XCP_UINT32_SB3_IDX)
#error XCP_UINT32_SB3_IDX already defined
#endif

#define XCP_UINT32_SB3_IDX 0U

#endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */

#if (defined XCP_CMD_PROCESSOR_QUEUE_SIZE)
#error XCP_CMD_PROCESSOR_QUEUE_SIZE already defined
#endif
/** \brief Size of the Command Queue.
 *
 * In Standard communication mode the size of command queue shall be 0x01U.
 * In Block communication mode the size of command queue shall be
 * MAX_BS/MAX_BS_PGM which ever is larger.  In Interleaved communication mode
 * the size of command queue shall be QUEUE_SIZE/QUEUE_SIZE_PGM which ever is
 * larger. */
#if((XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) || \
    (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON))
/** \brief In Block communication mode the size of command queue shall be MAX_BS */
#if (XCP_MAX_BS > XCP_MAX_BS_PGM)
#define XCP_CMD_PROCESSOR_QUEUE_SIZE      XCP_MAX_BS
#else
#define XCP_CMD_PROCESSOR_QUEUE_SIZE      XCP_MAX_BS_PGM
#endif
#else /* no block mode */
/** \brief In Standard communication mode the size of command queue shall be 1. */
#define XCP_CMD_PROCESSOR_QUEUE_SIZE      1U
#endif /* XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON || XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON */

#if ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON))

#if (defined XCP_ASCII_X)
#error XCP_ASCII_X already defined
#endif
/** \brief Definition of value indicating ASCII value for character X */
#define XCP_ASCII_X                                      0x58U

#if (defined XCP_ASCII_C)
#error XCP_ASCII_C already defined
#endif
/** \brief Definition of value indicating ASCII value for character C */
#define XCP_ASCII_C                                      0x43U

#if (defined XCP_ASCII_P)
#error XCP_ASCII_P already defined
#endif
/** \brief Definition of value indicating ASCII value for character P */
#define XCP_ASCII_P                                      0x50U

#if (defined XCP_ASCII_INVERSE_X)
#error XCP_ASCII_INVERSE_X already defined
#endif
/** \brief Definition of value indicating inverse of ASCII value for character X */
#define XCP_ASCII_INVERSE_X                              0xA7U

#if (defined XCP_ASCII_INVERSE_C)
#error XCP_ASCII_INVERSE_C already defined
#endif
/** \brief Definition of value indicating inverse of ASCII value for character C */
#define XCP_ASCII_INVERSE_C                              0xBCU

#if (defined XCP_ASCII_INVERSE_P)
#error XCP_ASCII_INVERSE_P already defined
#endif
/** \brief Definition of value indicating inverse of ASCII value for character P */
#define XCP_ASCII_INVERSE_P                              0xAFU

#if (defined XCP_GET_SLAVE_ID_OFFSET_X)
#error XCP_GET_SLAVE_ID_OFFSET_X already defined
#endif
/** \brief Definition of value indicating offset to ASCII value for character X */
#define XCP_GET_SLAVE_ID_OFFSET_X                        0x00U

#if (defined XCP_GET_SLAVE_ID_OFFSET_C)
#error XCP_GET_SLAVE_ID_OFFSET_C already defined
#endif
/** \brief Definition of value indicating offset to ASCII value for character C */
#define XCP_GET_SLAVE_ID_OFFSET_C                        0x01U

#if (defined XCP_GET_SLAVE_ID_OFFSET_P)
#error XCP_GET_SLAVE_ID_OFFSET_P already defined
#endif
/** \brief Definition of value indicating offset to ASCII value for character P */
#define XCP_GET_SLAVE_ID_OFFSET_P                        0x02U

#if (defined XCP_GET_SLAVE_ID_BY_ECHO)
#error XCP_GET_SLAVE_ID_BY_ECHO already defined
#endif
/** \brief Definition of command GET_SLAVE_ID mode parameter value indicating mode
 ** "confirm by echo". */
#define XCP_GET_SLAVE_ID_BY_ECHO                         0x00U

#if (defined XCP_GET_SLAVE_ID_BY_INVERSE_ECHO)
#error XCP_GET_SLAVE_ID_BY_INVERSE_ECHO already defined
#endif
/** \brief Definition of command GET_SLAVE_ID mode parameter value indicating mode
 ** "confirm by inverse echo". */
#define XCP_GET_SLAVE_ID_BY_INVERSE_ECHO                 0x01U

#if (defined XCP_SUBCMD_GET_SLAVE_ID_ON_CAN_RES_LENGTH)
#error XCP_SUBCMD_GET_SLAVE_ID_ON_CAN_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command GET_SLAVE_ID on CAN. */
#define XCP_SUBCMD_GET_SLAVE_ID_ON_CAN_RES_LENGTH               0x08U

#if (defined XCP_SUBCMD_GET_DAQ_ID_RES_LENGTH)
#error XCP_SUBCMD_GET_DAQ_ID_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command GET_DAQ_ID. */
#define XCP_SUBCMD_GET_DAQ_ID_RES_LENGTH                 0x08U

#if (defined XCP_CAN_ID_FIXED )
#error XCP_CAN_ID_FIXED  already defined
#endif
/** \brief Definition of value to indicate if CAN_ID is FIXED */
#define XCP_CAN_ID_FIXED                                 0x01U

#endif /* ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON)) */

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)
#if (defined XCP_SUBCMD_GET_SLAVE_ID_ON_ETH_RES_LENGTH)
#error XCP_SUBCMD_GET_SLAVE_ID_ON_ETH_RES_LENGTH already defined
#endif
/** \brief Length for positive response to the command GET_SLAVE_ID on Ethernet. */
#define XCP_SUBCMD_GET_SLAVE_ID_ON_ETH_RES_LENGTH       25U

#if (defined XCP_CTO_IS_NOT_MULTICAST)
#error XCP_CTO_IS_NOT_MULTICAST already defined
#endif
/** \brief Value indicating that CTO is not of type Multicast. */
#define XCP_CTO_IS_NOT_MULTICAST                        0U

#if (defined XCP_CTO_IS__MULTICAST)
#error XCP_CTO_IS_MULTICAST already defined
#endif
/** \brief Value indicating that CTO is of type Multicast. */
#define XCP_CTO_IS_MULTICAST                            1U

#if (defined XCP_GET_SLAVE_ID_ON_ETH_IP_V4)
#error XCP_GET_SLAVE_ID_ON_ETH_IP_V4 already defined
#endif
/** \brief Value for IPv4 version from IP_VERSION field of GET_SLAVE_ID on Ethernet. */
#define XCP_GET_SLAVE_ID_ON_ETH_IP_V4                   0U

#endif /* (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON) */

#if (defined XCP_CMDPROCESSOR_IDLE)
#error XCP_CMDPROCESSOR_IDLE already defined
#endif
/** \brief Value indicating that processor is currently idle */
#define XCP_CMDPROCESSOR_IDLE                           0U

#if (defined XCP_CMDPROCESSOR_BUSY)
#error XCP_CMDPROCESSOR_BUSY already defined
#endif
/** \brief Value indicating that processor is currently busy */
#define XCP_CMDPROCESSOR_BUSY                           1U

#if (defined XCP_ABSOLUTE_IF_MASK)
#error XCP_ABSOLUTE_IF_MASK already defined
#endif
/** \brief Mask indicating Identification field type with Absolute ODT number. */
#define XCP_ABSOLUTE_IF_MASK                               0x00U

#if (defined XCP_RELATIVE_BYTE_IF_MASK)
#error XCP_RELATIVE_BYTE_IF_MASK already defined
#endif
/** \brief Mask indicating Identification field type with Relative ODT number
 ** and 1 byte absolute DAQ list number. */
#define XCP_RELATIVE_BYTE_IF_MASK                          0x40U

#if (defined XCP_RELATIVE_WORD_IF_MASK)
#error XCP_RELATIVE_WORD_IF_MASK already defined
#endif
/** \brief Mask indicating Identification field type with Relative ODT number
 ** and 2 byte absolute DAQ list number. */
#define XCP_RELATIVE_WORD_IF_MASK                          0x80U

#if (defined XCP_RELATIVE_WORD_ALIGNED_IF_MASK)
#error XCP_RELATIVE_WORD_ALIGNED_IF_MASK already defined
#endif
/** \brief Mask indicating Identification field type with Relative ODT number,
 ** and word aligned absolute DAQ list number. */
#define XCP_RELATIVE_WORD_ALIGNED_IF_MASK                  0xC0U

#if (XCP_MAX_DAQ > 0)

#if (defined XCP_PID_LENGTH)
#error XCP_PID_LENGTH already defined
#endif
/** \brief Length of the Identification field in an incoming DTO
 */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)
#define XCP_PID_LENGTH    1U
#elif (XCP_IDENTIFICATION_FIELD_TYPE == XCP_RELATIVE_BYTE_IF_MASK)
#define XCP_PID_LENGTH    2U
#elif (XCP_IDENTIFICATION_FIELD_TYPE == XCP_RELATIVE_WORD_IF_MASK)
#define XCP_PID_LENGTH    3U
#else /* (XCP_IDENTIFICATION_FIELD_TYPE == XCP_RELATIVE_WORD_ALIGNED_IF_MASK) */
#define XCP_PID_LENGTH    4U
#endif

#endif /* (XCP_MAX_DAQ > 0) */

#if (defined XCP_TS_LENGTH)
#error XCP_TS_LENGTH already defined
#endif
/** \brief Length of the Timestamp field in an incoming DTO */
#if (XCP_TIMESTAMP_TYPE == XCP_NO_TIME_STAMP_TS_MASK)
#define XCP_TS_LENGTH 0U
#elif (XCP_TIMESTAMP_TYPE == XCP_ONE_BYTE_TS_MASK)
#define XCP_TS_LENGTH 1U
#elif (XCP_TIMESTAMP_TYPE == XCP_TWO_BYTE_TS_MASK)
#define XCP_TS_LENGTH 2U
#elif (XCP_TIMESTAMP_TYPE == XCP_FOUR_BYTE_TS_MASK)
#define XCP_TS_LENGTH 4U
#endif

#if (defined XCP_CTO_QUEUE_SIZE)
#error XCP_CTO_QUEUE_SIZE already defined
#endif
/** \brief Size of CTO Queue. Size of CTO Queue is taken as size required for
 * responses to at least two commands. */
#define XCP_CTO_QUEUE_SIZE ((XCP_MAX_CTO + 1U) * 2U)

#if (defined XCP_MAX_UPLOAD_BYTES_ONE_CTO)
  #error XCP_MAX_UPLOAD_BYTES_ONE_CTO already defined
#endif
/** \brief  Maximum bytes that can be uploaded from the slave in one CTO response */
#define XCP_MAX_UPLOAD_BYTES_ONE_CTO    (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto - XCP_ADDRESS_GRANULARITY_SIZE)

#if (defined XCP_MAX_DOWNLOAD_MAX_ELEMENTS)
  #error XCP_MAX_DOWNLOAD_MAX_ELEMENTS already defined
#endif
/** \brief  Maximum elements that can be downloaded with DOWNLOAD_MAX */
#define XCP_MAX_DOWNLOAD_MAX_ELEMENTS      ((Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto/XCP_ADDRESS_GRANULARITY_SIZE) - 1U)

#if (defined XCP_MAX_SHORT_DOWNLOAD_ELEMENTS)
  #error XCP_MAX_SHORT_DOWNLOAD_ELEMENTS already defined
#endif
/** \brief  Maximum elements that can be downloaded with a SHORT_DOWNLOAD command */
#define XCP_MAX_SHORT_DOWNLOAD_ELEMENTS    (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto - 8U)

/** \brief  Maximum elements that can be downloaded to the slave in one command
 * (DOWNLOAD or DOWNLOAD_NEXT)*/
#if (defined XCP_MAX_DOWNLOAD_ELEMNTS_ONE_CMD)
#error XCP_MAX_DOWNLOAD_ELEMNTS_ONE_CMD already defined
#endif
#if (XCP_ADDRESS_GRANULARITY_SIZE == XCP_BYTE_AG_SIZE)
#define XCP_MAX_DOWNLOAD_ELEMNTS_ONE_CMD   ((Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto - 2U) / XCP_ADDRESS_GRANULARITY_SIZE)
#else /* XCP_ADDRESS_GRANULARITY_SIZE = 2 or 4 bytes */
#define XCP_MAX_DOWNLOAD_ELEMNTS_ONE_CMD   ((Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Max_Cto - XCP_ADDRESS_GRANULARITY_SIZE) / \
                                             XCP_ADDRESS_GRANULARITY_SIZE)
#endif /* XCP_ADDRESS_GRANULARITY_SIZE == XCP_BYTE_AG_SIZE */

#if (XCP_ADDRESS_GRANULARITY_SIZE == XCP_BYTE_AG_SIZE)
#define XCP_MAX_OF_MAX_DOWNLOAD_ELEMENTS_ONE_CMD ((XCP_MAX_CTO - 2U) / XCP_ADDRESS_GRANULARITY_SIZE)
#else
#define XCP_MAX_OF_MAX_DOWNLOAD_ELEMENTS_ONE_CMD   ((XCP_MAX_CTO - XCP_ADDRESS_GRANULARITY_SIZE) / \
                                             XCP_ADDRESS_GRANULARITY_SIZE)
#endif

#if (defined XCP_MAX_DOWNLOAD_ELEMENTS)
#error XCP_MAX_DOWNLOAD_ELEMENTS already defined
#endif

#if (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)
/** \brief  Maximum elements that can be downloaded to the slave in one download sequence */
  #if ((XCP_MAX_BS * XCP_MAX_OF_MAX_DOWNLOAD_ELEMENTS_ONE_CMD) < XCP_ABS_MAX_DOWNLOAD_ELEMENTS)
  #define XCP_MAX_DOWNLOAD_ELEMENTS        (XCP_MAX_BS * XCP_MAX_OF_MAX_DOWNLOAD_ELEMENTS_ONE_CMD)
  #else /* we can't have more than 255 elements downloaded in one download sequence */
  #define XCP_MAX_DOWNLOAD_ELEMENTS        XCP_ABS_MAX_DOWNLOAD_ELEMENTS
  #endif /* (XCP_MAX_BS * XCP_MAX_OF_MAX_DOWNLOAD_ELEMENTS_ONE_CMD) < 255U */
#else /* (XCP_MASTER_BLOCK_MODE_SUPPORTED != STD_ON) */
  #define XCP_MAX_DOWNLOAD_ELEMENTS        XCP_MAX_OF_MAX_DOWNLOAD_ELEMENTS_ONE_CMD
#endif /* XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON */

#if (defined XCP_MAX_DOWNLOAD_BYTES)
#error XCP_MAX_DOWNLOAD_BYTES already defined
#endif
/** \brief  Maximum bytes that can be downloaded to the slave in one download sequence */
#define XCP_MAX_DOWNLOAD_BYTES             (XCP_MAX_DOWNLOAD_ELEMENTS * XCP_ADDRESS_GRANULARITY_SIZE)


#if (defined XCP_MAX_DOWNLOAD_BYTES_ONE_CMD)
#error XCP_MAX_DOWNLOAD_BYTES_ONE_CMD already defined
#endif
/** \brief  Maximum bytes that can be downloaded to the slave in one download sequence
 ** This maximum is determined by DOWNLOAD_MAX download capacity */
#if (XCP_ADDRESS_GRANULARITY_SIZE == XCP_BYTE_AG_SIZE)
#define XCP_MAX_DOWNLOAD_BYTES_ONE_CMD             (XCP_MAX_CTO - 1U)
#else /* XCP_ADDRESS_GRANULARITY_SIZE = 2 or 4 bytes */
#define XCP_MAX_DOWNLOAD_BYTES_ONE_CMD             (XCP_MAX_CTO - 2U)
#endif /* XCP_ADDRESS_GRANULARITY_SIZE == XCP_BYTE_AG_SIZE */

#if (defined XCP_MAX_DOWNLOAD_MEM_SIZE)
#error XCP_MAX_DOWNLOAD_MEM_SIZE already defined
#endif
/** \brief  Maximum bytes that we must reserve in the memory buffer for DOWNLOAD commands */
#if (XCP_MAX_DOWNLOAD_BYTES_ONE_CMD  > XCP_MAX_DOWNLOAD_BYTES)
#define  XCP_MAX_DOWNLOAD_MEM_SIZE XCP_MAX_DOWNLOAD_BYTES_ONE_CMD
#else
#define  XCP_MAX_DOWNLOAD_MEM_SIZE XCP_MAX_DOWNLOAD_BYTES
#endif

#if (XCP_PGM_SUPPORTED == STD_ON)
/** \brief  Maximum elements that can be programmed to the slave in one command
 * (PROGRAM or PROGRAM_NEXT) - but not PROGRAM_MAX */
#if (defined XCP_MAX_PROGRAM_ELEMNTS_ONE_CMD)
#error XCP_MAX_PROGRAM_ELEMNTS_ONE_CMD already defined
#endif
#if (XCP_ADDRESS_GRANULARITY_SIZE == XCP_BYTE_AG_SIZE)
#define XCP_MAX_PROGRAM_ELEMNTS_ONE_CMD   ((XCP_MAX_CTO_PGM - 2U) / XCP_ADDRESS_GRANULARITY_SIZE)
#else /* XCP_ADDRESS_GRANULARITY_SIZE = 2 or 4 bytes */
#define XCP_MAX_PROGRAM_ELEMNTS_ONE_CMD   ((XCP_MAX_CTO_PGM - XCP_ADDRESS_GRANULARITY_SIZE) / \
                                             XCP_ADDRESS_GRANULARITY_SIZE)
#endif /* XCP_ADDRESS_GRANULARITY_SIZE == XCP_BYTE_AG_SIZE */

#if (defined XCP_MAX_PROGRAM_ELEMENTS)
#error XCP_MAX_PROGRAM_ELEMENTS already defined
#endif
#if (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)
/** \brief  Maximum elements that can be programmed to the slave in one programming sequence */
  #if ((XCP_MAX_BS_PGM * XCP_MAX_PROGRAM_ELEMNTS_ONE_CMD) < XCP_ABS_MAX_PROGRAM_ELEMENTS)
  #define XCP_MAX_PROGRAM_ELEMENTS        (XCP_MAX_BS_PGM * XCP_MAX_PROGRAM_ELEMNTS_ONE_CMD)
  #else /* we can't have more than 255 elements programmed in one programming sequence */
  #define XCP_MAX_PROGRAM_ELEMENTS        XCP_ABS_MAX_PROGRAM_ELEMENTS
  #endif /* (XCP_MAX_BS_PGM * XCP_MAX_PROGRAM_ELEMNTS_ONE_CMD) < 255U */
#else /* (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED != STD_ON) */
  #define XCP_MAX_PROGRAM_ELEMENTS        XCP_MAX_PROGRAM_ELEMNTS_ONE_CMD
#endif /* XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON */

#if (defined XCP_MAX_PROGRAM_BYTES)
#error XCP_MAX_PROGRAM_BYTES already defined
#endif
/** \brief  Maximum bytes that can be programmed to the slave in one programming sequence */
#define XCP_MAX_PROGRAM_BYTES             (XCP_MAX_PROGRAM_ELEMENTS * XCP_ADDRESS_GRANULARITY_SIZE)
#else /* PGM resource is not available */
#if (defined XCP_MAX_PROGRAM_BYTES)
#error XCP_MAX_PROGRAM_BYTES already defined
#endif
/** \brief  Maximum bytes that can be programmed to the slave in one programming sequence */
#define XCP_MAX_PROGRAM_BYTES             0U
#endif /* XCP_PGM_SUPPORTED == STD_ON */

#if (defined XCP_MAX_MEMORY_WRITE_BYTES)
#error XCP_MAX_MEMORY_WRITE_BYTES already defined
#endif
/** \brief  The maximum between XCP_MAX_PROGRAM_BYTES and XCP_MAX_DOWNLOAD_MEM_SIZE */
#if (XCP_MAX_PROGRAM_BYTES > XCP_MAX_DOWNLOAD_MEM_SIZE)
#define XCP_MAX_MEMORY_WRITE_BYTES   XCP_MAX_PROGRAM_BYTES
#else
#define XCP_MAX_MEMORY_WRITE_BYTES   XCP_MAX_DOWNLOAD_MEM_SIZE
#endif

/** \brief Will return the start index of data to be downloaded from the CTO command.
 * Definition is according to the ASAM specs and applies to DOWNLOAD AND DOWNLOAD_NEXT
 * commands
 */
#if (defined XCP_GET_START_DATA_INDEX_FROM_CTO)
#error XCP_GET_START_DATA_INDEX_FROM_CTO already defined
#endif
#if (XCP_ADDRESS_GRANULARITY_SIZE == XCP_BYTE_AG_SIZE)
#define XCP_GET_START_DATA_INDEX_FROM_CTO()      2U
#else /* AG = 2 or 4 bytes */
#define XCP_GET_START_DATA_INDEX_FROM_CTO()      XCP_ADDRESS_GRANULARITY_SIZE
#endif

#if (defined XCP_SHORT_DOWNLOAD_FIRST_ELEMENT_IDX)
#error XCP_SHORT_DOWNLOAD_FIRST_ELEMENT_IDX already defined
#endif
/** \brief  Index in the CMD queue of the first element to be downloaded with SHORT_DOWNLOAD */
#define XCP_SHORT_DOWNLOAD_FIRST_ELEMENT_IDX     8U


 #if (defined XCP_DOWNLOAD_MAX_FIRST_ELEMENT_IDX)
#error XCP_DOWNLOAD_MAX_FIRST_ELEMENT_IDX already defined
#endif
/** \brief  Index in the CMD queue of the first element to be downloaded  with DOWNLOAD_MAX*/
#define XCP_DOWNLOAD_MAX_FIRST_ELEMENT_IDX       XCP_ADDRESS_GRANULARITY_SIZE

#if (defined XCP_DRIVER_VERSION)
#error XCP_DRIVER_VERSION already defined
#endif
/** \brief The version that must be returned to the XCP master upon GET_COMM_MODE_INFO command
 * Constructed according to the ASAM specs */
#define XCP_DRIVER_VERSION  (((uint8)(XCP_SW_MAJOR_VERSION << 4U)) | (XCP_SW_MINOR_VERSION & 0x0FU));

#if (XCP_EVENT_PACKET_ENABLED == STD_ON)

#if (defined XCP_EV_STIM_TIMEOUT_INFO_DAQLIST)
#error XCP_EV_STIM_TIMEOUT_INFO_DAQLIST already defined
#endif
/** \brief EV_STIM_TIMEOUT event reports with a DAQList info. */
#define XCP_EV_STIM_TIMEOUT_INFO_DAQLIST                  0x01U

#if (defined XCP_EV_STIM_TIMEOUT_INFO_EVENT)
#error XCP_EV_STIM_TIMEOUT_INFO_EVENT already defined
#endif
/** \brief EV_STIM_TIMEOUT event reports with a Event channel number as info. */
#define XCP_EV_STIM_TIMEOUT_INFO_EVENT                    0x00U

#if (defined XCP_EV_PID)
#error XCP_EV_PID already defined
#endif
/** \brief PID for Event packet CTO. */
#define XCP_EV_PID                                        0xFDU

#endif /* XCP_EVENT_PACKET_ENABLED == STD_ON */

#if (XCP_MAX_DAQ > 0)

#if (defined XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS)
#error XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS already defined
#endif
/** \brief The number of configured DAQ lists.
 ** \param[in] RetType Cast type used to fix MISRA-C 10.1 violation */
#if (XCP_DAQ_COUNT == 0U)
#define XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(RetType)    ((RetType)XCP_MAX_DAQ)
#else
#define XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(RetType)    ((RetType)XCP_MIN_DAQ + (RetType)Xcp_GetNoOfDynamicDaqLists())
#endif

#endif /* (XCP_MAX_DAQ > 0) */

#if (XCP_EVENT_PACKET_ENABLED == STD_ON)

#if (defined XCP_EV_RESUME_MODE)
#error XCP_EV_RESUME_MODE already defined
#endif
/** \brief Event code for Slave starting in RESUME mode */
#define XCP_EV_RESUME_MODE              0x00U

#if (defined XCP_EV_CLEAR_DAQ)
#error XCP_EV_CLEAR_DAQ already defined
#endif
/** \brief Event code for clearing the DAQ configuration in non-volatile memory. */
#define XCP_EV_CLEAR_DAQ                0x01U

#if (defined XCP_EV_STORE_DAQ)
#error XCP_EV_STORE_DAQ already defined
#endif
/** \brief Event code for storing the DAQ configuration in non-volatile memory. */
#define XCP_EV_STORE_DAQ                0x02U

#if (defined XCP_EV_STORE_CAL)
#error XCP_EV_STORE_CAL already defined
#endif
/** \brief Event code for storing the calibration data in non-volatile memory. */
#define XCP_EV_STORE_CAL                0x03U

#if (defined XCP_EV_CMD_PENDING)
#error XCP_EV_CMD_PENDING already defined
#endif
/** \brief Event code for Slave requesting to restart time-out */
#define XCP_EV_CMD_PENDING              0x05U

#if (defined XCP_EV_DAQ_OVERLOAD)
#error XCP_EV_DAQ_OVERLOAD already defined
#endif
/** \brief Event code for DAQ processor overload */
#define XCP_EV_DAQ_OVERLOAD             0x06U

#if (defined XCP_EV_SESSION_TERMINATED)
#error XCP_EV_SESSION_TERMINATED already defined
#endif
/** \brief Event code for Session terminated by slave device */
#define XCP_EV_SESSION_TERMINATED       0x07U

#if (defined XCP_EV_TIME_SYNC)
#error XCP_EV_TIME_SYNC already defined
#endif
/** \brief Event code for Transfer of externally triggered timestamp */
#define XCP_EV_TIME_SYNC                0x08U

#if (defined XCP_EV_STIM_TIMEOUT)
#error XCP_EV_STIM_TIMEOUT already defined
#endif
/** \brief Event code for Indication of a STIM timeout */
#define XCP_EV_STIM_TIMEOUT             0x09U

#if (defined XCP_EV_SLEEP)
#error XCP_EV_SLEEP already defined
#endif
/** \brief Event code for Slave entering SLEEP mode */
#define XCP_EV_SLEEP                    0x0AU

#if (defined XCP_EV_WAKE_UP)
#error XCP_EV_WAKE_UP already defined
#endif
/** \brief Event code for Slave leaving SLEEP mode */
#define XCP_EV_WAKE_UP                  0x0BU

#if (defined XCP_EV_USER)
#error XCP_EV_USER already defined
#endif
/** \brief Event code for User-defined event */
#define XCP_EV_USER                     0x0EU

#if (defined XCP_EV_TRANSPORT)
#error XCP_EV_TRANSPORT already defined
#endif
/** \brief Event code for Transport layer specific event */
#define XCP_EV_TRANSPORT                0x0FU

#endif /* XCP_EVENT_PACKET_ENABLED == STD_ON */

#if (XCP_OVERLOAD_MSB_SUPPORT == STD_ON)
#if (defined XCP_EV_DAQ_OVERLOAD_EV_MASK)
#error XCP_EV_DAQ_OVERLOAD_EV_MASK already defined
#endif
/** \brief Mask used to extract the flag indicating that the
 **        Ev OVERLOAD has been detected for the current DTO */
#define XCP_EV_DAQ_OVERLOAD_EV_MASK   0x01U
#endif /* (XCP_OVERLOAD_MSB_SUPPORT == STD_ON) */

#if (XCP_MAX_EVENT_CHANNEL > 0U)

#if (defined XCP_EVENT_IS_SET_FLAG_POS)
#error XCP_EVENT_IS_SET_FLAG_POS already defined
#endif
/** \brief Mask used to extract the flag indicating that the
 **        Event has been set */
#define XCP_EVENT_IS_SET_FLAG_POS     0U

#if (defined XCP_EVENT_OVELOAD_FLAG_POS)
#error XCP_EVENT_OVELOAD_FLAG_POS already defined
#endif
/** \brief Mask used to extract the flag indicating that the
 **        Event overload has been set for the selected event channel */
#define XCP_EVENT_OVELOAD_FLAG_POS    1U

#endif /* (XCP_MAX_EVENT_CHANNEL > 0U) */

#if (defined XCP_TX_PDU_FLAG_SIZE)
#error XCP_TX_PDU_FLAG_SIZE already defined
#endif
/** \brief defines the number of bytes necessary to allocate for the Tx Pdu buffers,
 *         to accommodate the busy status */
#if ((XCP_MAX_TX_PDU_PER_CON % 8U) == 0U)
#define XCP_TX_PDU_FLAG_SIZE  \
  ((XCP_MAX_TX_PDU_PER_CON) / 8U)
#else
#define XCP_TX_PDU_FLAG_SIZE  \
  ((XCP_MAX_TX_PDU_PER_CON / 8U) + 1U)
#endif

#if (defined XCP_RX_PDU_FLAG_SIZE)
#error XCP_RX_PDU_FLAG_SIZE already defined
#endif
/** \brief defines the number of bytes necessary to allocate for the Tx Pdu buffers,
 *         to accommodate the busy status */
#if ((XCP_MAX_RX_PDU_PER_CON % 8U) == 0U)
#define XCP_RX_PDU_FLAG_SIZE  \
  ((XCP_MAX_RX_PDU_PER_CON) / 8U)
#else
#define XCP_RX_PDU_FLAG_SIZE  \
  ((XCP_MAX_RX_PDU_PER_CON / 8U) + 1U)
#endif

#if (defined XCP_EVENT_PROCESSING_FLAG_SIZE)
#error XCP_EVENT_PROCESSING_FLAG_SIZE already defined
#endif
/** \brief defines the number of bytes necessary to allocate for the events that have main functions,
 *   to accommodate the busy status */
#if ((XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS % 8U) == 0U)
#define XCP_EVENT_PROCESSING_FLAG_SIZE  \
  ((XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS) / 8U)
#else
#define XCP_EVENT_PROCESSING_FLAG_SIZE  \
  ((XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS / 8U) + 1U)
#endif

#if (defined XCP_CONNECTION_FLAG_SIZE)
#error XCP_CONNECTION_FLAG_SIZE already defined
#endif
/** \brief defines the number of bytes necessary to allocate for the Tx Pdu buffers,
 *         to accommodate the busy status */
#if ((XCP_NO_OF_CONNECTIONS % 8U) == 0U)
#define XCP_CONNECTION_FLAG_SIZE  \
  ((XCP_NO_OF_CONNECTIONS) / 8U)
#else
#define XCP_CONNECTION_FLAG_SIZE  \
  ((XCP_NO_OF_CONNECTIONS / 8U) + 1U)
#endif


#if (defined XCP_NO_EVENT_MAIN_FUNCTION_IDX)
#error XCP_NO_EVENT_MAIN_FUNCTION_IDX already defined
#endif
/** \brief Value indicating that the index is not to be considered (no event main function) */
#define XCP_NO_EVENT_MAIN_FUNCTION_IDX                   0xFFU

#if (defined XCP_MAX_ODT_MASK_SIZE)
#error XCP_MAX_ODT_MASK_SIZE already defined
#endif
#if (XCP_MAX_DAQ > 0U)
/** \brief The maximum size of ODT mask.
 * This value is used to declare the maximum size of ODT Data Mask */
#if ((XCP_MAX_ODT_IN_DAQ % 8U) == 0U)
#define XCP_MAX_ODT_MASK_SIZE \
((XCP_MAX_ODT_IN_DAQ) / 8U)
#else
#define XCP_MAX_ODT_MASK_SIZE \
((XCP_MAX_ODT_IN_DAQ / 8U) + 1U)
#endif
#endif /* (XCP_MAX_DAQ > 0U) */

#if (defined XCP_INIT_CONNECTION_ID)
#error XCP_INIT_CONNECTION_ID already defined
#endif
/** \brief The initialization value for connection Id status. */
#if (XCP_NO_OF_CONNECTIONS > 1U)
#define XCP_INIT_CONNECTION_ID                             XCP_INVALID_CONNECTION_ID
#else
#define XCP_INIT_CONNECTION_ID                             0U
#endif /* XCP_NO_OF_CONNECTIONS > 1U */

#if (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON)
#if (defined XCP_DYNAMIC_PDU_CMD_LENGTH)
#error XCP_DYNAMIC_PDU_CMD_LENGTH already defined
#endif
/** \brief Expected length of the received buffer */
#define XCP_DYNAMIC_PDU_CMD_LENGTH                         3U

#if (defined XCP_DYNAMIC_PDU_CMD_DIRECTION)
#error XCP_DYNAMIC_PDU_CMD_DIRECTION already defined
#endif
/** \brief Index of data indicating direction in the received buffer */
#define XCP_DYNAMIC_PDU_CMD_DIRECTION                      0U

#if (defined XCP_DYNAMIC_PDU_CMD_PDUID)
#error XCP_DYNAMIC_PDU_CMD_PDUID already defined
#endif
/** \brief Index of data indicating PduId in the received buffer */
#define XCP_DYNAMIC_PDU_CMD_PDUID                          1U

#if (defined XCP_DYNAMIC_PDU_CMD_STATE)
#error XCP_DYNAMIC_PDU_CMD_STATE already defined
#endif
/** \brief Index of data indicating requested state in the received buffer */
#define XCP_DYNAMIC_PDU_CMD_STATE                          2U

#if (defined XCP_DYNAMIC_PDU_CMD_STATE_ENABLED)
#error XCP_DYNAMIC_PDU_CMD_STATE_ENABLED already defined
#endif
/** \brief Defines the enabled state of a PDU */
#define XCP_DYNAMIC_PDU_CMD_STATE_ENABLED                  1U

#if (defined XCP_DYNAMIC_PDU_CMD_STATE_DISABLED)
#error XCP_DYNAMIC_PDU_CMD_STATE_DISABLED already defined
#endif
/** \brief Defines the disabled state of a PDU */
#define XCP_DYNAMIC_PDU_CMD_STATE_DISABLED                 0U

#if (defined XCP_DYNAMIC_PDU_CMD_DIRECTION_TX)
#error XCP_DYNAMIC_PDU_CMD_DIRECTION_TX already defined
#endif
/** \brief Defines the disabled state of a PDU */
#define XCP_DYNAMIC_PDU_CMD_DIRECTION_TX                   0U

#if (defined XCP_DYNAMIC_PDU_CMD_DIRECTION_RX)
#error XCP_DYNAMIC_PDU_CMD_DIRECTION_RX already defined
#endif
/** \brief Defines the disabled state of a PDU */
#define XCP_DYNAMIC_PDU_CMD_DIRECTION_RX                   1U

#endif

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))

#if (defined XCP_FLX_BUF_NO_INIT_VALUE)
#error XCP_FLX_BUF_NO_INIT_VALUE already defined
#endif
/** \brief Defines the symbolic name for not initialized values
  */
#define XCP_FLX_BUF_NO_INIT_VALUE                          0U

#if (defined XCP_FLX_BUF_VALUE_FIXED)
#error XCP_FLX_BUF_VALUE_FIXED already defined
#endif
/** \brief Defines wether the value of the FLX_BUF parameter is fixed or not
  * Fixed means that the value cannot be changed via FLX_ASSIGN subcommand
  */
#define XCP_FLX_BUF_VALUE_FIXED                            0U

#if (defined XCP_FLX_BUF_VALUE_VARIABLE)
#error XCP_FLX_BUF_VALUE_VARIABLE already defined
#endif
/** \brief Defines wether the value of the FLX_BUF parameter is variable or not
  * Variable means that the parameter does not have an initial value and it
  * must be changed via FLX_ASSIGN subcommand
  */
#define XCP_FLX_BUF_VALUE_VARIABLE                         1U

#if (defined XCP_FLX_BUF_VALUE_VARIABLE_INITIALIZED)
#error XCP_FLX_BUF_VALUE_VARIABLE_INITIALIZED already defined
#endif
/** \brief Defines wether the value of the FLX_BUF parameter is variable and initialized or not
  * Variable initialized means that the parameter has have an initial value and it
  * can be changed via FLX_ASSIGN subcommand
  */
#define XCP_FLX_BUF_VALUE_VARIABLE_INITIALIZED             2U


#if (defined XCP_FLX_BUF_RESET_ALL)
#error XCP_FLX_BUF_RESET_ALL already defined
#endif
/** \brief Value for FLX_BUF signaling that all buffers shall be reset
  */
#define XCP_FLX_BUF_RESET_ALL                             0xFFU

#if (defined XCP_FLX_BUF_VALUE_CHANNEL_A)
#error XCP_FLX_BUF_VALUE_CHANNEL_A already defined
#endif
/** \brief Symbolic value for CHANNEL_A
  */
#define XCP_FLX_BUF_VALUE_CHANNEL_A                        0U

#if (defined XCP_FLX_BUF_VALUE_CHANNEL_B)
#error XCP_FLX_BUF_VALUE_CHANNEL_B already defined
#endif
/** \brief Symbolic value for CHANNEL_B
  */
#define XCP_FLX_BUF_VALUE_CHANNEL_B                        1U


#if (defined XCP_FLX_NO_BIT_SET_MASK)
#error XCP_FLX_NO_BIT_SET_MASK already defined
#endif
/** \brief Gets the mask for no bit set
  */
#define XCP_FLX_NO_BIT_SET_MASK                          (0U)

#if (defined XCP_FLX_BUF_PACKET_CMD_MASK)
#error XCP_FLX_BUF_PACKET_CMD_MASK already defined
#endif
/** \brief Gets the mask for the CMD bit in the XCP_PACKET
  */
#define XCP_FLX_BUF_PACKET_CMD_MASK                      (1U)

#if (defined XCP_FLX_BUF_PACKET_STIM_MASK)
#error XCP_FLX_BUF_PACKET_STIM_MASK already defined
#endif
/** \brief Gets the mask for the STIM bit in the XCP_PACKET
  */
#define XCP_FLX_BUF_PACKET_STIM_MASK                     (2U)

#if (defined XCP_FLX_BUF_PACKET_RES_ERR_MASK)
#error XCP_FLX_BUF_PACKET_RES_ERR_MASK already defined
#endif
/** \brief Gets the mask for the RES_ERR bit in the XCP_PACKET
  */
#define XCP_FLX_BUF_PACKET_RES_ERR_MASK                  (4U)

#if (defined XCP_FLX_BUF_PACKET_EV_SERV_MASK)
#error XCP_FLX_BUF_PACKET_EV_SERV_MASK already defined
#endif
/** \brief Gets the mask for the EV_SERV bit in the XCP_PACKET
  */
#define XCP_FLX_BUF_PACKET_EV_SERV_MASK                  (8U)

#if (defined XCP_FLX_BUF_PACKET_DAQ_MASK)
#error XCP_FLX_BUF_PACKET_DAQ_MASK already defined
#endif
/** \brief Gets the mask for the DAQ bit in the XCP_PACKET
  */
#define XCP_FLX_BUF_PACKET_DAQ_MASK                      (16U)


#if (defined XCP_BUFFER_DIRECTION_TX_MASK)
#error XCP_BUFFER_DIRECTION_TX_MASK already defined
#endif
/** \brief Bit set means that the buffer direction is Tx, else Rx
  */
#define XCP_BUFFER_DIRECTION_TX_MASK                     (1U)

#if (defined XCP_BUFFER_PRECONFIGURED_MASK)
#error XCP_BUFFER_PRECONFIGURED_MASK already defined
#endif
/** \brief Bit set means that the buffer is preconfigured
  */
#define XCP_BUFFER_PRECONFIGURED_MASK                    (2U)

#if (defined XCP_BUFFER_FIXED_MASK)
#error XCP_BUFFER_FIXED_MASK already defined
#endif
/** \brief Bit set means that the buffer is fixed and cannot be changed at runtime
  */
#define XCP_BUFFER_FIXED_MASK                            (4U)


#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON) */

#if (XCP_MEMORY_ACCESS_AREAS == STD_ON)
#if (defined XCP_NO_MEMPROXY_REQUESTED)
#error XCP_NO_MEMPROXY_REQUESTED already defined
#endif
/** \brief State for event not waiting for process
  */
#define XCP_NO_MEMPROXY_REQUESTED                    (0U)

#if (defined XCP_PROCESS_ON_MEMPROXY_REQUESTED)
#error XCP_PROCESS_ON_MEMPROXY_REQUESTED already defined
#endif
/** \brief State for memory proxy request triggered
  */
#define XCP_PROCESS_ON_MEMPROXY_REQUESTED            (1U)
#endif

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
#if (defined XCP_NO_USER_CALLOUT_REQUESTED)
#error XCP_NO_USER_CALLOUT_REQUESTED already defined
#endif
/** \brief State for no request */
#define XCP_NO_USER_CALLOUT_REQUESTED       (0U)

#if (defined XCP_USERCALLOUT_REQUEST_IN_PROGRESS)
#error XCP_USERCALLOUT_REQUEST_IN_PROGRESS already defined
#endif
/** \brief State for request processing in progress */
#define XCP_USERCALLOUT_REQUEST_IN_PROGRESS       (1U)

#if (defined XCP_RESULT_ALREADY_PROCESSED)
#error XCP_RESULT_ALREADY_PROCESSED already defined
#endif
/** \brief State for request was processed and response can be sent */
#define XCP_RESULT_ALREADY_PROCESSED        (2U)

#if (defined XCP_REQUESTED_TO_MEMORY_PROXY)
#error XCP_REQUESTED_TO_MEMORY_PROXY already defined
#endif
/** \brief State for request to MemoryProxy in progress */
#define XCP_REQUESTED_TO_MEMORY_PROXY        (3U)
#endif

/*------------------[Function like macros]----------------------------------------------*/
#if (XCP_MAX_DAQ > 0)
#if (defined XCP_GET_DAQ_LIST_PTR)
#error XCP_GET_DAQ_LIST_PTR already defined
#endif
#if (XCP_DAQ_COUNT == 0U)
/** \brief Gets the DAQ list pointer from the corresponding static structure */
#define XCP_GET_DAQ_LIST_PTR(DaqListNumber)  &(Xcp_DaqLists.Xcp_NonDynamicDaqLists.Xcp_Daq[(DaqListNumber)])

#else /* DAQ lists are dynamically configured */
#define XCP_GET_DAQ_LIST_PTR(DaqListNumber) Xcp_GetDaqListPtr((Xcp_DaqIdType)(DaqListNumber))
#endif /* (XCP_DAQ_COUNT == 0U) */

#if (defined XCP_GET_MIRROR_PTR)
#error XCP_GET_MIRROR_PTR already defined
#endif
#if (XCP_DAQ_COUNT == 0U)
/** \brief Gets the DAQ list pointer from the corresponding static structure */
#define XCP_GET_MIRROR_PTR(DaqID, MirrorOffsetPosition) \
  (&(Xcp_DTOEntryMirror.NonDynamicOdtMirrorDataArray[(MirrorOffsetPosition)]))

#else /* DAQ lists are dynamically configured */
#define XCP_GET_MIRROR_PTR(DaqID, MirrorOffsetPosition) \
  Xcp_GetMirrorPtr((DaqID), (MirrorOffsetPosition))
#endif /* (XCP_DAQ_COUNT == 0U) */

#if (defined XCP_GET_DAQ_ID_FROM_PRIO)
#error XCP_GET_DAQ_ID_FROM_PRIO already defined
#endif
/** \brief Gets the DAQ ID from priority queue */
#define XCP_GET_DAQ_ID_FROM_PRIO(PrioId) \
  (Xcp_DaqPrioQueue.DaqRuntimeDataArray[(PrioId)].DaqId)

/** \brief Gets the ODT ID from priority queue */
#define XCP_GET_ODT_ID_FROM_PRIO(PrioId) \
  (Xcp_DaqPrioQueue.DaqRuntimeDataArray[(PrioId)].OdtId)

/** \brief Returns the FIRST_PID in case the DTO Packets have an
 * Identification Field Type "absolute ODT number". */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)
#if (defined XCP_GET_FIRST_PID)
#error XCP_GET_FIRST_PID already defined
#endif
#define XCP_GET_FIRST_PID(ListParamPtr) ((ListParamPtr)->FirstPID)
#endif

#if (defined XCP_GET_DAQLIST_PROPERTIES)
#error XCP_GET_DAQLIST_PROPERTIES already defined
#endif
/** \brief Returns the properties of the specified DAQ List.   */
#define XCP_GET_DAQLIST_PROPERTIES(DaqCfgPtr) ((DaqCfgPtr)->Properties)

#if (defined XCP_GET_DAQLIST_MAX_ODT)
#error XCP_GET_DAQLIST_MAX_ODT already defined
#endif
/** \brief Returns the number of ODTs in the specified DAQ List.   */
#define XCP_GET_DAQLIST_MAX_ODT(DaqCfgPtr) ((DaqCfgPtr)->MaxOdt)

#if (defined XCP_GET_DAQLIST_MAX_ODT_ENTRIES)
#error XCP_GET_DAQLIST_MAX_ODT_ENTRIES already defined
#endif
/** \brief Returns the maximum number ODT entries in the specified DAQ List. */
#define XCP_GET_DAQLIST_MAX_ODT_ENTRIES(DaqCfgPtr) ((DaqCfgPtr)->MaxOdtEntries)

#if (defined XCP_IS_DAQ_DIRECTION)
#error XCP_IS_DAQ_DIRECTION already defined
#endif
/** \brief Checks whether the DAQ configuration supports specified direction */
#define XCP_IS_DAQ_DIRECTION(ListNumber, Direction) \
  (((Xcp_DaqListsDefault.Xcp_Daq[(ListNumber)].Properties & (Direction)) == (Direction))? TRUE:FALSE)

#if (defined XCP_IS_DAQLIST_EVENT_CHANNEL_FIXED)
#error XCP_IS_DAQLIST_EVENT_CHANNEL_FIXED already defined
#endif
/** \brief Checks whether the DAQ List event channel number is fixed */
#define XCP_IS_DAQLIST_EVENT_CHANNEL_FIXED(DaqCfgPtr) \
      ((XCP_MASK_DAQLIST_EVENT_FIXED == \
             ((DaqCfgPtr)->Properties & XCP_MASK_DAQLIST_EVENT_FIXED))?TRUE:FALSE)

#if (defined XCP_GET_DAQLIST_FIXED_EVENT_CHANNEL)
#error XCP_GET_DAQLIST_FIXED_EVENT_CHANNEL already defined
#endif
/** \brief Returns number of the fixed event channel to be used for the
 *   specified DAQ list.
 */
#define XCP_GET_DAQLIST_FIXED_EVENT_CHANNEL(DaqCfgPtr) \
  (((XCP_IS_DAQLIST_EVENT_CHANNEL_FIXED(DaqCfgPtr))== TRUE) ? \
  ((DaqCfgPtr))->EventId : XCP_INVALID_EVENT_CHANNEL)

#endif /* (XCP_MAX_DAQ > 0) */

#if (defined XCP_GET_SESSION_CONFIGID)
#error XCP_GET_SESSION_CONFIGID already defined
#endif
/** \brief Returns the configuration ID. */
#if (XCP_STORE_DAQ_SUPPORTED == STD_ON)
#define XCP_GET_SESSION_CONFIGID()             Xcp_DaqLists.Xcp_StoreDaq.ConfigurationSessionID
#else
#define XCP_GET_SESSION_CONFIGID()             0U
#endif /* XCP_STORE_DAQ_SUPPORTED == STD_ON */

#if (XCP_DEV_ERROR_DETECT == STD_ON)

#if (defined XCP_DET_REPORT_ERROR)
#error XCP_DET_REPORT_ERROR already defined
#endif
/** \brief Report Xcp development error */
#define XCP_DET_REPORT_ERROR(ApiId,ErrorId)                            \
        ((void)Det_ReportError(XCP_MODULE_ID, XCP_INSTANCE_ID, (ApiId), (ErrorId)))

#endif /* if (XCP_DEV_ERROR_DETECT == STD_ON) */

#if (defined XCP_DET_REPORT_RUNTIME_ERROR)
#error XCP_DET_REPORT_RUNTIME_ERROR already defined
#endif
/** \brief Report Xcp runtime error */
#define XCP_DET_REPORT_RUNTIME_ERROR(ApiId,ErrorId) ((void)Det_ReportRuntimeError(XCP_MODULE_ID, XCP_INSTANCE_ID, (ApiId), (ErrorId)))

#if (defined XCP_UINT16_FROM_BYTE_ARRAY)
  #error XCP_UINT16_FROM_BYTE_ARRAY already defined
#endif
/** \brief Create a uint16 value from the first two elements of uint8 array */
#define XCP_UINT16_FROM_BYTE_ARRAY(ptr) ( (uint16) \
          ( (uint16)((uint16)((ptr)[XCP_UINT16_MSB_IDX]) << 8U) \
          | ((uint16)((ptr)[XCP_UINT16_LSB_IDX]) ) ) )

#if (defined XCP_UINT32_FROM_BYTE_ARRAY)
  #error XCP_UINT32_FROM_BYTE_ARRAY already defined
#endif
/** \brief Create a uint32 value from the first four elements of uint8 array */
#define XCP_UINT32_FROM_BYTE_ARRAY(ptr) ( (uint32) \
          ( ((uint32)((ptr)[XCP_UINT32_SB3_IDX]) << 24U) \
          | ((uint32)((ptr)[XCP_UINT32_SB2_IDX]) << 16U) \
          | ((uint32)((ptr)[XCP_UINT32_SB1_IDX]) << 8U) \
          | ((uint32)((ptr)[XCP_UINT32_SB0_IDX]) ) ) )

#if (XCP_MAX_EVENT_CHANNEL != 0U)

#if (defined XCP_GET_EVENT_NO_OF_DAQLIST)
#error XCP_GET_EVENT_NO_OF_DAQLIST already defined
#endif
/** \brief Returns number of DAQ Lists currently associated with event channel.
*/
#define XCP_GET_EVENT_NO_OF_DAQLIST(ChannelNumber) \
                                    (Xcp_Event[(ChannelNumber)].DaqIdListCount)

#if (defined XCP_GET_EVENT_CHANNEL_CONSISTENCY)
#error XCP_GET_EVENT_CHANNEL_CONSISTENCY already defined
#endif
/** \brief Returns the consistency of the specified event channel. */
#define XCP_GET_EVENT_CHANNEL_CONSISTENCY(ChannelNumber) \
  (Xcp_EventInfo[(ChannelNumber)].Flag & XCP_EVENT_CHANNEL_CONSISTENCY_MASK)

#endif /* XCP_MAX_EVENT_CHANNEL != 0U */

#if (XCP_MAX_DAQ != 0U)

#if (defined XCP_IS_DAQLIST_FULLY_CONFIGURED)
#error XCP_IS_DAQLIST_FULLY_CONFIGURED already defined
#endif
/** \brief Check whether DAQ List is already configured with SET_DAQ_LIST_MODE
 **   command.
 */
#define XCP_IS_DAQLIST_FULLY_CONFIGURED(DaqListPtr) \
 (((uint8)((DaqListPtr)->Flags & (uint8)(XCP_MASK_DAQLIST_CONFIGURED)) == 0U)?\
        FALSE:TRUE )

#endif /* XCP_MAX_DAQ != 0U */

#if (defined XCP_GET_FLAG_BYTE_POS)
#error XCP_GET_FLAG_BYTE_POS aleready defined
#endif
/** \brief macro definition used to calculate the byte position in the mask array */
#define XCP_GET_FLAG_BYTE_POS(FlagId)  ((FlagId) / 8U)

#if (defined XCP_GET_FLAG_BYTE_MASK)
#error XCP_GET_FLAG_BYTE_MASK already defined
#endif
/** \brief macro definition used to calculate the flag mask inside the selected
 *         byte of the mask array */
#define XCP_GET_FLAG_BYTE_MASK(FlagId) ((uint8)(1U << ((FlagId) % 8U)))


#if (defined XCP_GET_FLAG_BIT)
#error XCP_GET_FLAG_BIT already defined
#endif
/** \brief macro definition used to calculate the bit inside the selected
 *         byte of the mask array */
#define XCP_GET_FLAG_BIT(FlagId) ((uint8)((FlagId) % 8U))

#if (defined XCP_IS_FLAG_SET)
#error XCP_IS_FLAG_SET already defined
#endif
/** \brief Function to verify if a flag is set inside a bit array variable
 **
 ** \param[in]  FlagMask     The Mask used to retrieve flag value
 ** \param[in]  BitArray     Variable containing the flag
 ** \param[in]  BitArrayType The type of the variable containing the flag
 **
 ** \retval     TRUE,        if the byte flag is set
 ** \retval     FALSE,       if the byte flag is cleared */
#define XCP_IS_FLAG_SET(FlagMask, BitArray, BitArrayType) \
  ((((BitArray) & (BitArrayType)(FlagMask)) != 0U ) ? TRUE : FALSE)

#if (defined XCP_IS_BYTE_FLAG_SET)
#error XCP_IS_BYTE_FLAG_SET already defined
#endif
/** \brief Function to verify if a flag is set inside a byte array
 **
 ** \param[in]  FlagId       The Id of the flag
 ** \param[in]  BitArrayPtr  Pointer to the byte array containing the flag
 **
 ** \retval     TRUE,        if the byte flag is set
 ** \retval     FALSE,       if the byte flag is cleared */
#define XCP_IS_BYTE_FLAG_SET(FlagId, BitArrayPtr) \
  XCP_IS_FLAG_SET(XCP_GET_FLAG_BYTE_MASK((FlagId)), \
                  ((P2CONST(uint8,AUTOMATIC,AUTOMATIC))(BitArrayPtr))[XCP_GET_FLAG_BYTE_POS((FlagId))], \
                  uint8)

#if (defined XCP_CLEAR_FLAG)
#error XCP_CLEAR_FLAG already defined
#endif
/** \brief Function to clear a flag inside a bit array variable
 **
 ** \param[in]   FlagMask     The Mask used to retrieve flag value
 ** \param[out]  BitArray     Variable containing the flag
 ** \param[in]  BitArrayType  The type of the variable containing the flag */
#define XCP_CLEAR_FLAG(FlagMask, BitArray, BitArrayType) \
  ((BitArray) &= (BitArrayType)(~(FlagMask)))

#if (defined XCP_CLEAR_BYTE_FLAG)
#error XCP_CLEAR_BYTE_FLAG already defined
#endif
/** \brief Function to clear a flag inside a byte array
 **
 ** \param[in]  FlagId       The Id of the flag
 ** \param[out] BitArrayPtr  Pointer to the byte array containing the flag */
#define XCP_CLEAR_BYTE_FLAG(FlagId, BitArrayPtr) \
  XCP_CLEAR_FLAG(XCP_GET_FLAG_BYTE_MASK((FlagId)), \
                 ((P2VAR(uint8,AUTOMATIC,AUTOMATIC))(BitArrayPtr))[XCP_GET_FLAG_BYTE_POS((FlagId))], \
                 uint8)


#if (defined XCP_CLEAR_FLAG_U8_ATOMIC)
#error XCP_CLEAR_FLAG_U8_ATOMIC already defined
#endif
/** \brief Function to clear a flag inside a bit array variable in an atomic fashion
 **
 ** \param[in]   BitPosition  The bit position used to clear bit
 ** \param[out]  BitField     Variable containing the bit */
#define XCP_CLEAR_FLAG_U8_ATOMIC(BitPosition, BitField) \
    TS_AtomicClearBit_8(&(BitField), \
                       (BitPosition) \
                     )

#if (defined XCP_SET_FLAG_U8_ATOMIC)
#error XCP_SET_FLAG_U8_ATOMIC already defined
#endif
/** \brief Function to set a flag inside a bit array variable in an atomic fashion
 **
 ** \param[in]   BitPosition  The bit position used to set bit value
 ** \param[out]  BitField     Variable containing the bit */
#define XCP_SET_FLAG_U8_ATOMIC(BitPosition, BitField) \
    TS_AtomicSetBit_8(&(BitField), \
                       (BitPosition) \
                     )

#if (defined XCP_CLEAR_BYTE_FLAG_ATOMIC)
#error XCP_CLEAR_BYTE_FLAG_ATOMIC already defined
#endif
/** \brief Function to clear a flag inside a byte array in an atomic fashion
 ** Use this only if it's outside an Xcp exclusive area
 ** \param[in]  FlagId       The Id of the flag
 ** \param[out] BitArrayPtr  Pointer to the byte array containing the flag */
#define XCP_CLEAR_BYTE_FLAG_ATOMIC(FlagId, BitArrayPtr) \
  TS_AtomicClearBit_8(&((P2VAR(uint8,AUTOMATIC,AUTOMATIC))(BitArrayPtr))[XCP_GET_FLAG_BYTE_POS((FlagId))], \
                       XCP_GET_FLAG_BIT((FlagId)) \
                     )


#if (defined XCP_SET_FLAG)
#error XCP_SET_FLAG already defined
#endif
/** \brief Function to set a flag inside a bit array variable
 **
 ** \param[in]   FlagMask     The Mask used to retrieve flag value
 ** \param[out]  BitArray     Variable containing the flag
 ** \param[in]  BitArrayType  The type of the variable containing the flag */
#define XCP_SET_FLAG(FlagMask, BitArray, BitArrayType) \
  ((BitArray) |= (BitArrayType)(FlagMask))

#if (defined XCP_SET_BYTE_FLAG)
#error XCP_SET_BYTE_FLAG already defined
#endif
/** \brief Function to set a flag inside a byte array
 **
 ** \param[in]  FlagId       The Id of the flag
 ** \param[out] BitArrayPtr  Pointer to the byte array containing the flag */
#define XCP_SET_BYTE_FLAG(FlagId, BitArrayPtr) \
  XCP_SET_FLAG(XCP_GET_FLAG_BYTE_MASK((FlagId)), \
               ((P2VAR(uint8,AUTOMATIC,AUTOMATIC))(BitArrayPtr))[XCP_GET_FLAG_BYTE_POS((FlagId))], \
               uint8)


#if (defined XCP_SET_BYTE_FLAG_ATOMIC)
#error XCP_SET_BYTE_FLAG_ATOMIC already defined
#endif
/** \brief Function to set a flag inside a byte array in an atomic fashion
 **
 ** \param[in]  FlagId       The Id of the flag
 ** \param[out] BitArrayPtr  Pointer to the byte array containing the flag */
#define XCP_SET_BYTE_FLAG_ATOMIC(FlagId, BitArrayPtr) \
  TS_AtomicSetBit_8(&((P2VAR(uint8,AUTOMATIC,AUTOMATIC))(BitArrayPtr))[XCP_GET_FLAG_BYTE_POS((FlagId))], \
                       XCP_GET_FLAG_BIT((FlagId)) \
                     )

#if (defined XCP_RELEASE_FIRST_LEVEL_LOCK)
#error XCP_RELEASE_FIRST_LEVEL_LOCK already defined
#endif
/** \brief The initialization value for connection Id status needed for releasing the first level lock. */
#if (XCP_NO_OF_CONNECTIONS > 1U)
#define XCP_RELEASE_FIRST_LEVEL_LOCK()    (Xcp_ConnectionStatus.ConnectionId = XCP_INVALID_CONNECTION_ID)
#else
#define XCP_RELEASE_FIRST_LEVEL_LOCK()    (Xcp_ConnectionStatus.ConnectionId = 0U)
#endif /* XCP_NO_OF_CONNECTIONS > 1U */

#if (defined XCP_RELEASE_SECOND_LEVEL_LOCK)
#error XCP_RELEASE_SECOND_LEVEL_LOCK already defined
#endif
/** \brief The initialization value of connection state used for releasing the second level lock */
#define XCP_RELEASE_SECOND_LEVEL_LOCK()    (Xcp_ConnectionStatus.State = XCP_STATE_DISCONNECTED)

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)

#if (defined SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS)
#error SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS already defined
#endif
/** \brief Definition of the Exclusive Area Enter */
#define SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS() Xcp_SchMExclusiveAreas[GetCoreID()].enter()

#if (defined SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS)
#error SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS already defined
#endif
/** \brief Definition of the Exclusive Area Exit */
#define SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS() Xcp_SchMExclusiveAreas[GetCoreID()].exit()
#endif

#if (defined MAX_FLX_LEN_ABS)
#error MAX_FLX_LEN_ABS already defined
#endif
/** \brief Maximum possible length of a FlexRay buffer */
#define MAX_FLX_LEN_ABS             254U


#if (defined MIN_FLX_LEN_ABS)
#error MIN_FLX_LEN_ABS already defined
#endif
/** \brief Minimum possible length of a FlexRay buffer */
#define MIN_FLX_LEN_ABS             8U

#if (defined MAX_FLX_SLOT_ID)
#error MAX_FLX_SLOT_ID already defined
#endif
/** \brief Maximum possible value for a FlexRay Slot ID  */
#define MAX_FLX_SLOT_ID             2047U

#if (defined INVALID_FLX_SLOT_ID)
#error INVALID_FLX_SLOT_ID already defined
#endif
/** \brief Invalid value for a slot ID */
#define INVALID_FLX_SLOT_ID         0U

#ifndef XCP_REPORT_RETRY_FAILURE
/** \brief definition of the retry error reporting interface */
#if (XCP_PROD_ERR_HANDLING_RETRY_FAILED == TS_PROD_ERR_REP_TO_DEM)
    /* Report DEM */
#define XCP_REPORT_RETRY_FAILURE(SidId) \
    Dem_ReportErrorStatus(XCP_E_RETRY_FAILED_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED)
#elif (XCP_PROD_ERR_HANDLING_RETRY_FAILED == TS_PROD_ERR_REP_TO_DET)
    /* Report DET */
#define XCP_REPORT_RETRY_FAILURE(SidId) \
    XCP_DET_REPORT_ERROR(SidId, XCP_E_DEMTODET_RETRY_FAILED)
#endif
#endif

#ifndef XCP_REPORT_ILLEGAL_MEMORY_ACCESS
/** \brief definition of the illegal memory access reporting interface */
#if (XCP_PROD_ERR_HANDLING_ILLEGAL_MEMORY_ACCESS == TS_PROD_ERR_REP_TO_DEM)
    /* Report DEM */
#define XCP_REPORT_ILLEGAL_MEMORY_ACCESS(SidId) \
    Dem_ReportErrorStatus(XCP_E_ILLEGAL_MEMORY_ACCESS_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED)
#elif (XCP_PROD_ERR_HANDLING_ILLEGAL_MEMORY_ACCESS == TS_PROD_ERR_REP_TO_DET)
    /* Report DET */
#define XCP_REPORT_ILLEGAL_MEMORY_ACCESS(SidId) \
    XCP_DET_REPORT_ERROR(SidId, XCP_E_DEMTODET_ILLEGAL_MEMORY_ACCESS)
#endif
#endif

#ifndef XCP_REPORT_PDU_BUFFER_LOCKED
/** \brief definition of the pdu buffer locked reporting interface */
#if (XCP_PROD_ERR_HANDLING_PDU_BUFFER_LOCKED == TS_PROD_ERR_REP_TO_DEM)
    /* Report DEM */
#define XCP_REPORT_PDU_BUFFER_LOCKED(SidId) \
    Dem_ReportErrorStatus(XCP_E_PDU_BUFFER_LOCKED_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED)
#elif (XCP_PROD_ERR_HANDLING_PDU_BUFFER_LOCKED == TS_PROD_ERR_REP_TO_DET)
    /* Report DET */
#define XCP_REPORT_PDU_BUFFER_LOCKED(SidId) \
    XCP_DET_REPORT_ERROR(SidId, XCP_E_DEMTODET_PDU_BUFFER_LOCKED)
#endif
#endif

#ifndef XCP_REPORT_RESP_CTO_QUEUE_FULL
/** \brief definition of the response cto queue full reporting interface */
#if (XCP_PROD_ERR_HANDLING_RESP_CTO_QUEUE_FULL == TS_PROD_ERR_REP_TO_DEM)
    /* Report DEM */
#define XCP_REPORT_RESP_CTO_QUEUE_FULL(SidId) \
    Dem_ReportErrorStatus(XCP_E_RESP_CTO_QUEUE_FULL_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED)
#elif (XCP_PROD_ERR_HANDLING_RESP_CTO_QUEUE_FULL == TS_PROD_ERR_REP_TO_DET)
    /* Report DET */
#define XCP_REPORT_RESP_CTO_QUEUE_FULL(SidId) \
    XCP_DET_REPORT_ERROR(SidId, XCP_E_DEMTODET_RESP_CTO_QUEUE_FULL)
#endif
#endif

#ifndef XCP_REPORT_PDU_BUFFER_FULL
/** \brief definition of the pdu buffer full reporting interface */
#if (XCP_PROD_ERR_HANDLING_PDU_BUFFER_FULL == TS_PROD_ERR_REP_TO_DEM)
    /* Report DEM */
#define XCP_REPORT_PDU_BUFFER_FULL(SidId) \
    Dem_ReportErrorStatus(XCP_E_PDU_BUFFER_FULL_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED)
#elif (XCP_PROD_ERR_HANDLING_PDU_BUFFER_FULL == TS_PROD_ERR_REP_TO_DET)
    /* Report DET */
#define XCP_REPORT_PDU_BUFFER_FULL(SidId) \
    XCP_DET_REPORT_ERROR(SidId, XCP_E_DEMTODET_PDU_BUFFER_FULL)
#endif
#endif

#ifndef XCP_REPORT_PDU_LOST
/** \brief definition of the pdu lost reporting interface */
#if (XCP_PROD_ERR_HANDLING_PDU_LOST == TS_PROD_ERR_REP_TO_DEM)
    /* Report DEM */
#define XCP_REPORT_PDU_LOST(SidId) \
    Dem_ReportErrorStatus(XCP_E_PDU_LOST_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED)
#elif (XCP_PROD_ERR_HANDLING_PDU_LOST == TS_PROD_ERR_REP_TO_DET)
    /* Report DET */
#define XCP_REPORT_PDU_LOST(SidId) \
    XCP_DET_REPORT_ERROR(SidId, XCP_E_DEMTODET_PDU_LOST)
#endif
#endif

#ifndef XCP_REPORT_STIMULATION_DATA_LOST
/** \brief definition of the stimulation data lost reporting interface */
#if (XCP_PROD_ERR_HANDLING_STIMULATION_DATA_LOST == TS_PROD_ERR_REP_TO_DEM)
    /* Report DEM */
#define XCP_REPORT_STIMULATION_DATA_LOST(SidId) \
    Dem_ReportErrorStatus(XCP_E_STIMULATION_DATA_LOST_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED)
#elif (XCP_PROD_ERR_HANDLING_STIMULATION_DATA_LOST == TS_PROD_ERR_REP_TO_DET)
    /* Report DET */
#define XCP_REPORT_STIMULATION_DATA_LOST(SidId) \
    XCP_DET_REPORT_ERROR(SidId, XCP_E_DEMTODET_STIMULATION_DATA_LOST)
#endif
#endif

#ifndef XCP_REPORT_ACQUISITION_DATA_LOST
/** \brief definition of the acquisition data lost reporting interface */
#if (XCP_PROD_ERR_HANDLING_ACQUISITION_DATA_LOST == TS_PROD_ERR_REP_TO_DEM)
    /* Report DEM */
#define XCP_REPORT_ACQUISITION_DATA_LOST(SidId) \
    Dem_ReportErrorStatus(XCP_E_ACQUISITION_DATA_LOST_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED)
#elif (XCP_PROD_ERR_HANDLING_ACQUISITION_DATA_LOST == TS_PROD_ERR_REP_TO_DET)
    /* Report DET */
#define XCP_REPORT_ACQUISITION_DATA_LOST(SidId) \
    XCP_DET_REPORT_ERROR(SidId, XCP_E_DEMTODET_ACQUISITION_DATA_LOST)
#endif
#endif

#if (defined IS_CMD_MASTER_BLOCK_MODE)
#error IS_CMD_MASTER_BLOCK_MODE already defined
#endif
/** \brief Checks if the command id is a Master block mode command */
#if((XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) && \
    (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON))
#define IS_CMD_MASTER_BLOCK_MODE(CommandId)         \
    (((((CommandId) == XCP_CMD_DOWNLOAD_NEXT_PID)  || \
       ((CommandId) == XCP_CMD_DOWNLOAD_PID     )) || \
      (((CommandId) == XCP_CMD_PROGRAM_NEXT_PID )  || \
       ((CommandId) == XCP_CMD_PROGRAM_PID      ))    \
      ) ? TRUE : FALSE)
#elif(XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)
#if (XCP_PGM_SUPPORTED == STD_ON)
#define IS_CMD_MASTER_BLOCK_MODE(CommandId)          \
    ((((((CommandId) == XCP_CMD_DOWNLOAD_NEXT_PID)  || \
        ((CommandId) == XCP_CMD_DOWNLOAD_PID     )) || \
       ( (CommandId) == XCP_CMD_PROGRAM_PID       ))   \
      ) ? TRUE : FALSE)
#else
#define IS_CMD_MASTER_BLOCK_MODE(CommandId)        \
    (((((CommandId) == XCP_CMD_DOWNLOAD_NEXT_PID) || \
       ((CommandId) == XCP_CMD_DOWNLOAD_PID     ))   \
      ) ? TRUE : FALSE)
#endif /* (XCP_PGM_SUPPORTED == STD_ON) */
#elif(XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)
#define IS_CMD_MASTER_BLOCK_MODE(CommandId)        \
    (((( (CommandId) == XCP_CMD_DOWNLOAD_PID    ) || \
       (((CommandId) == XCP_CMD_PROGRAM_NEXT_PID) || \
        ((CommandId) == XCP_CMD_PROGRAM_PID     )))  \
      ) ? TRUE : FALSE)
#elif(XCP_PGM_SUPPORTED == STD_ON)
#define IS_CMD_MASTER_BLOCK_MODE(CommandId)       \
    (((((CommandId) == XCP_CMD_DOWNLOAD_PID    ) || \
       ((CommandId) == XCP_CMD_PROGRAM_PID     ))   \
      ) ? TRUE : FALSE)
#else
#define IS_CMD_MASTER_BLOCK_MODE(CommandId)   \
    ((((CommandId) == XCP_CMD_DOWNLOAD_PID    ) \
      ) ? TRUE : FALSE)
#endif

#if (defined IS_CMD_DOWNLOAD)
#error IS_CMD_DOWNLOAD already defined
#endif
#define IS_CMD_DOWNLOAD(CommandId) (((CommandId) == XCP_CMD_DOWNLOAD_PID) || ((CommandId) == XCP_CMD_DOWNLOAD_NEXT_PID))

#if (defined XCP_NUMBER_OF_FRAME_COUNTERS)
#error XCP_NUMBER_OF_FRAME_COUNTERS already defined
#endif
/** \brief The number of frame counters */
#define XCP_NUMBER_OF_FRAME_COUNTERS   (XCP_NO_OF_FLEXRAY_CONN_WITH_SEQ_CORR + XCP_NO_OF_ETHERNET_CONN)

#if (defined XCP_REMOTE_DISPATCH_SETEVENT)
#error XCP_REMOTE_DISPATCH_SETEVENT already defined
#endif
/** \brief The type for Xcp_SetEvent request through dispatcher API */
#define XCP_REMOTE_DISPATCH_SETEVENT 0U

#if (defined XCP_REMOTE_DISPATCH_SETTRANSMISSIONMODE)
#error XCP_REMOTE_DISPATCH_SETTRANSMISSIONMODE already defined
#endif
/** \brief The type for Xcp_SetTransmissionMode request through dispatcher API */
#define XCP_REMOTE_DISPATCH_SETTRANSMISSIONMODE 1U

#if (defined XCP_REMOTE_DISPATCH_CONTROL)
#error XCP_REMOTE_DISPATCH_CONTROL already defined
#endif
/** \brief The type for Xcp_Control request through dispatcher API */
#define XCP_REMOTE_DISPATCH_CONTROL 2U

#if (defined XCP_REMOTE_DISPATCH_SETREQSTORECALREQCBK)
#error XCP_REMOTE_DISPATCH_SETREQSTORECALREQCBK already defined
#endif
/** \brief The type for Xcp_SetReqStoreCalReqCbk request through dispatcher API */
#define XCP_REMOTE_DISPATCH_SETREQSTORECALREQCBK 3U

#if (defined XCP_REMOTE_DISPATCH_INIT)
#error XCP_REMOTE_DISPATCH_INIT already defined
#endif
/** \brief The type for Xcp_Init request through dispatcher API */
#define XCP_REMOTE_DISPATCH_INIT 4U

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
#if (defined XCP_NO_ACTIVE_CONNECTION)
#error XCP_NO_ACTIVE_CONNECTION already defined
#endif
/** \brief The if value used for no active connection */
#define XCP_NO_ACTIVE_CONNECTION 0xFFU
#endif

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)

#define SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS() Xcp_SchMExclusiveAreas[GetCoreID()].enter()
#define SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS() Xcp_SchMExclusiveAreas[GetCoreID()].exit()

#endif

#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON)

#ifdef XCP_GET_ODT_ENTRY_ADDRESS_SIZE
#error XCP_GET_ODT_ENTRY_ADDRESS_SIZE already defined
#endif
/** \brief The size of an ODT entry address, 4 bytes as required by ASAM */
#define XCP_GET_ODT_ENTRY_ADDRESS_SIZE ((usize)4U)

#ifdef XCP_GET_ODT_ENTRY_LENGTH_SIZE
#error XCP_GET_ODT_ENTRY_LENGTH_SIZE already defined
#endif
/** \brief The size of an ODT entry length */
#define XCP_GET_ODT_ENTRY_LENGTH_SIZE ((usize)1U)

#ifdef XCP_GET_ODT_ENTRY_BITOFFSET_SIZE
#error XCP_GET_ODT_ENTRY_BITOFFSET_SIZE already defined
#endif
/** \brief The size of an ODT entry bit offset */
#define XCP_GET_ODT_ENTRY_BITOFFSET_SIZE ((usize)1U)


#ifdef XCP_ODT_ENTRY_FIELD_ADDRESS
#error XCP_ODT_ENTRY_FIELD_ADDRESS already defined
#endif

#ifdef XCP_ODT_ENTRY_FIELD_LENGTH
#error XCP_ODT_ENTRY_FIELD_LENGTH already defined
#endif

#ifdef XCP_ODT_ENTRY_FIELD_BITOFFSET
#error XCP_ODT_ENTRY_FIELD_BITOFFSET already defined
#endif

/** \brief Identifier used by Xcp_GetOdtEntryData for the ODT entry address */
#define XCP_ODT_ENTRY_FIELD_ADDRESS 0U
/** \brief Identifier used by Xcp_GetOdtEntryData for the ODT entry length */
#define XCP_ODT_ENTRY_FIELD_LENGTH 1U
/** \brief Identifier used by Xcp_GetOdtEntryData for the ODT entry bit offset */
#define XCP_ODT_ENTRY_FIELD_BITOFFSET 2U

#endif

/*==================[type definitions]======================================*/

/* XCP Odt Id Type */
typedef uint8 Xcp_OdtIdType;

#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
/* XCP event packet codes */
typedef uint8 Xcp_EventPacketType;

/* EVENT_CTO information used while updating the EVENT_CTO queue. */
typedef struct
{
  uint8 Length;                    /* Length of Event_CTO  */
  uint8 EVENT_CTO[XCP_MAX_EV_CTO]; /* Array of EVENT_CTO  */
} Xcp_EventCTOType;

/* EVENT CTO queue */
typedef struct
{
  uint8 Head;     /* read position in queue */
  uint8 Tail;     /* write position in queue*/
  uint8 NoOfElements; /* no of elements available in queue */
  Xcp_EventCTOType EventCTOQueue[XCP_EV_CTO_QUEUE_SIZE]; /* Event Cto queue */
}Xcp_EventCTOQueueType;
#endif /* XCP_EVENT_PACKET_ENABLED == STD_ON */

#if (XCP_MAX_DAQ != 0U)

/* DAQ Runtime Data Type.*/
typedef struct
{
  Xcp_DaqIdType DaqId;            /* The Id of DAQ */
  Xcp_OdtIdType OdtId;            /* The Id of ODT */
#if (XCP_OVERLOAD_MSB_SUPPORT == STD_ON)
  uint8 OverloadOdt;           /* The Overload status for current ODT */
#endif /* (XCP_OVERLOAD_MSB_SUPPORT == STD_ON) */
} Xcp_DaqRuntimeDataType;

/* DAQ Priority queue Type. */
typedef struct
{
  /* DaqRuntimeDataArray */
  Xcp_DaqRuntimeDataType DaqRuntimeDataArray[XCP_NO_DAQ_DTOS];
  /* Number of entries in DaqRuntimeDataArray */
  Xcp_DaqIdType NoOfEntries;
} Xcp_DaqPrioQueueType;

/* DAQ Run Time Info Type. */
typedef struct
{
#if (XCP_TIMESTAMP_SUPPORTED == XCP_TIMESTAMP_MASK)
  /* Timestamp value written when the event is processed. */
  Xcp_TimestampType TimeStamp;
#endif
  /* Bit field containing signaling if the ODT data in the mirror is:
   * - New = 1,
   * - Old = 0  */
  uint8 IsNewOdtDataMask[XCP_MAX_ODT_MASK_SIZE];
} Xcp_DaqRunTimeInfoType;

/* Xcp Event. */
typedef struct
{
  uint32 TimeCycleCounter;     /* Needed for Cyclic Event Channels ( TimeCycle > 0U ) */
                               /* Event Sampling Period / Cycle Counter * Xcp Main Function Period */
  P2VAR(Xcp_DaqIdType, TYPEDEF, XCP_VAR) DaqIdListPtr; /* Pointer to the list of DAQ IDs
                                                                of this event */
  uint8 NextDaqIndex;         /* Next DAQ index to be sampled (applicable for DAQ/ODT consistency) */
  uint8 NextOdtIndex;          /* Next ODT to be sampled (applicable for ODT consistency) */
  uint8 DaqIdListCount;        /* The number of DAQ IDs of this event */
  uint8 EventStatusFlag;       /* bitmap used to indicate the Event Status:
                                * - XCP_EVENT_IS_SET_FLAG_POS
                                * - XCP_EVENT_OVELOAD_FLAG_POS */
#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
  Xcp_DaqIdType NumberOfRunningDaqLists; /* Number of DAQ lists associated with this event that are currently running */
#endif
} Xcp_EventType;

/* Event information in ROM. */
typedef struct
{
  uint32 CycleCounterMax; /* Event Channel Time Cycle Counter Max */
                          /* For cyclic events it is calculated:
                              ( TimeCycle * TimeUnit_in seconds ) / XcpMainFuncPeriod ). */
                          /* For normal events it is initialized to 1U */
  P2CONST(uint8, TYPEDEF, XCP_CONST) EventNamePtr; /* The name of the event */
  P2CONST(Xcp_DaqIdType, TYPEDEF, XCP_CONST) DaqIdListPtr; /* Pointer to the list of DAQ IDs
                                                                of this event */
  uint8 DaqIdListCount;   /* The number of DAQ IDs of this event */
  uint8 EventNameLength;  /* The length of the event name */
  uint8 Flag;             /* Consistency and STIM/DAQ, both direction combine
                                                    configured for this event */
  uint8 MaxDaqList;       /* Maximum number of DAQ supported for this event */
  uint8 Priority;         /* The priority of this event */
  uint8 TimeCycle;        /* Event Sampling Period / Cycle */
  uint8 TimeUnit;         /* Event Channel Time Cycle Unit */
#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
  uint8 IndexRunningFlag; /* Index in the flags holding whether the event MF is currently processing */
#endif

} Xcp_EventInfoType;

/* Information related to each event in the queue
*/
typedef struct
{
  uint16 ID; /* The ID of the event */
  uint16 NextDaqIndex; /* Next DAQ index to be sampled (applicable for DAQ,
                          ODT consistency) */
  uint8 NextOdtIndex; /* Next ODT to be sampled
                         (applicable for ODT consistency) */
  uint8 Priority; /* The priority of the event */
} Xcp_EventProcessingInfoType;

#endif /* XCP_MAX_DAQ != 0U */

/* CTO information used while updating the CTO queue. */
typedef struct
{
  uint8 Length;              /* The Length of the CTO */
  uint8 CTO[XCP_MAX_CTO_ABS]; /* The CTO */
} Xcp_CTOType;

/* Command queue Type. */
typedef struct
{
  uint8 NoOfCmds; /* Number of commands in queue */
  uint8 Head;     /* Command queue header */
  uint8 Tail;     /* Command queue tail */
  Xcp_CTOType Queue[XCP_CMD_PROCESSOR_QUEUE_SIZE]; /* Command queue */
} Xcp_CMDQueueType;

/* Number of bytes used in CTO Queue. Can hold MAX_CTO + the length information */
#if (XCP_CTO_QUEUE_SIZE > 0xFF)
  typedef uint16 Xcp_CtoNoOfBytesType;
#else
  typedef uint8 Xcp_CtoNoOfBytesType;
#endif

/* CTO queue Type. */
typedef struct
{
  Xcp_CtoNoOfBytesType NoOfBytes;     /* Number of bytes in CTO Queue */
  Xcp_CtoNoOfBytesType ReservedBytes; /* Number of reserved bytes in CTO Queue */
  Xcp_CtoNoOfBytesType Head;          /* CTO queue header */
  Xcp_CtoNoOfBytesType Tail;          /* CTO queue tail */
  uint8 Queue[XCP_CTO_QUEUE_SIZE];    /* CTO queue */
} Xcp_CTOQueueType;

/** \brief Command processor's states */
typedef uint8 Xcp_ProcessorStateType;

/** \brief Status information of the slave.
 * All current status information of the slave device: this includes the status of the resource
 * protection, pending store requests, general status of data acquisition and stimulation */
typedef struct
{
#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
  uint16 NoOfSelectedDaq; /* Number of DAQ lists in selected mode */
  uint16 NoOfRunningDaq;  /* Number of DAQ lists in running mode */
#endif
  uint8 SessionStatus;    /* Bit7: Slave is in RESUME mode or not.
                           * Bit6: Whether a DAQ list is currently running
                           * Bit5: RESERVED
                           * Bit3: Request to clear DAQ configuration pending.
                           * Bit2: Request to store DAQ list pending.
                           * Bit0: Request to store CALibration data pending.
                           */
  uint8 ProtectionStatus; /* Bit4: CAL/PAG commands are protected or not.
                           * Bit3: DAQ list commands are protected or not.
                           * Bit2: STIM list commands are protected or not.
                           * Bit0: Programming commands are protected or not.
                           */
} Xcp_StatusType;

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))

/* XCP Flexray HW Buffers configuration */
typedef struct
{
  uint8 FlxBufID;                         /* ID of the FLX_BUF */
  uint16 FlxLPDU_ID;                       /* LPDU_ID of the FLX_BUF */
  uint8 FlxPduIdConfIdx;                  /* Index in either  Xcp_TxLPduIdConf or Xcp_RxLPduIdConf
                                             used to link the packet type assignment and internal PDU ID */
  uint8 FlxCtrlIdx;                       /* Flexray controler ID */

  uint8 FlxBufLengthProperties;           /* MAX_FLX_LEN_BUF properties
                                             (fixed or variable init) */
  uint8 FlxBufLengthInitValue;             /* MAX_FLX_LEN_BUF_x_init value */

  uint8 FlxBufSlotIDProperties;           /* SLOT_ID properties
                                            (fixed, variable or variable init) */
  uint16 FlxBufSlotIDInitValue;            /* SLOT_ID initial value if any */

  uint8 FlxBufOffsetProperties;           /* OFFSET properties
                                          (fixed, variable or variable init) */
  uint8 FlxBufOffsetInitValue;            /* OFFSET initial value if any */

  uint8 FlxBufCycleRepetitionProperties;  /* CYCLE_REPETITION properties
                                             (fixed, variable or variable init) */
  uint8 FlxBufCycleRepetitionInitValue;   /* CYCLE_REPETITION initial value if any */

  uint8 FlxBufChannelProperties;          /* CHANNEL properties
                                             (fixed, variable or variable init) */
  uint8 FlxBufChannelInitValue;           /* CHANNEL initial value if any */

  uint8 Flags;                            /* BIT 0 - Direction of this buffer, taken from the LPDU */
                                          /* BIT 1 - Wether the buffer is completely initialized or not */
                                          /* BIT 2 - Wether the buffer is fixed or not
                                                     (if it's fixed it cannot be changed at runtime) */

} Xcp_FlxBufType;

#if (XCP_MAX_DAQ > 0U)
/* XCP Flexray HW Buffers runtime info */
typedef struct
{
  /* Variable holding the DAQ List ID for which this FlexRay Buffer is assigned to */
  Xcp_DaqIdType FlxDaqAssignment;
  /* Flag indicating whether the buffer should accept any DAQ list since SET_DAQ_FLX_BUF was
  not yet called for this buffer */
  boolean FlxDaqAcceptAll;
}Xcp_FlxBufRuntimeType;

#endif /* (XCP_MAX_DAQ > 0U) */

#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON) */

#if (XCP_NUMBER_OF_MEMORY_AREAS > 0 || XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
/* Generic measurement type  */
typedef struct
{
  P2VAR(uint8, XCP_CONST, XCP_APPL_DATA) MemoryRangeStartAddress;
  uint32 MemoryRangeLength;
  uint8  AccessType;
  uint8  AccessScope;
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
  uint8  ApplicationId;
#endif
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
  Xcp_DescriptorCallbackType Callback;
#endif
}Xcp_MemoryAreaType;
#endif /* XCP_NUMBER_OF_MEMORY_AREAS > 0 */

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
/* queue for events which are ready for calling MemProxy  */
typedef struct
{
  uint16 EventId[XCP_NUMBER_OF_QUEUED_EVENTS];
  uint8 ReadIndex;
  uint8 WriteIdx;
} Xcp_RequestQueueType;

/* Information about request queue  */
typedef struct
{
  uint16 IndexOfRequest;
  uint8 RequestStatus;
  uint8 ProcessingCtr;
  uint8 NoOfMemProxyRequests;
  boolean EntriesForAppIdAvailable[XCP_NUM_OF_OS_APPS];
} Xcp_RequestForEventType;

typedef struct
{
  Xcp_OdtSizeType DataLength;
  Xcp_DaqIdType DaqId;
  uint8 OdtIndex;
} Xcp_RequestForDaqPrioType;

/* Structure for processing RequestBuffer */
typedef struct
{
  uint16 WriteIdx;
  uint8 NoOfEntries[XCP_MAX_EVENT_CHANNEL];
} Xcp_RequestToMemProxyStructType;

typedef struct
{
  Xcp_RequestForDaqPrioType Queue[XCP_MAX_SIZE_OF_DAQ_PRIO_LIST];
  uint16 ReadIdx;
  uint16 WriteIdx;
  uint8 NoOfEntries[XCP_MAX_EVENT_CHANNEL];
} Xcp_DaqPrioQueueOfRequestType;
#endif

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)

typedef P2FUNC(void, XCP_CONST, XcpSchMExclusiveAreaFuncPtr)(void);

typedef struct
{
  XcpSchMExclusiveAreaFuncPtr enter;
  XcpSchMExclusiveAreaFuncPtr exit;
} XcpSchMExclusiveAreaStruct;

#endif

#if (XCP_DAQ_COUNT > 0U)
/** \brief Runtime info regarding the configured Dynamic DAQ lists */
typedef struct
{
  uint32 AllocatedOdtsSoFar; /* Total number of ODTs allocated up to now */
  uint32 AllocatedOdtEntriesSoFar; /* Total number of ODT entries allocated up to now */
  P2VAR(Xcp_OdtType, TYPEDEF, XCP_VAR) FirstDynamicOdtPtr;           /* Address of the very first
                                                                        configured dynamic ODT */
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
  P2VAR(Xcp_OdtEntryType, TYPEDEF, XCP_VAR) FirstDynamicOdtEntryPtr; /* Address of the very
                                                                        first configured
                                                                        dynamic ODT entry*/
#else
  P2VAR(usize, TYPEDEF, XCP_VAR) FirstDynamicOdtEntryAddr;
#endif
  Xcp_DaqIdType NoOfConfiguredDynamicDAQLists; /* Total number of dynamically configured DAQ Lists */
  uint8 LastDynamicDAQReq; /* Last serviced dynamic DAQ request */
} Xcp_DynamicDaqListsType;
#endif /* (XCP_DAQ_COUNT > 0U) */

/*==================[external function declarations]========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

#if ((XCP_MAX_DAQ != 0U) && \
     (XCP_EVENT_PACKET_ENABLED == STD_ON) && \
     (XCP_OVERLOAD_MSB_SUPPORT == STD_OFF))
/** \brief Function to prepare the EV_DAQ_OVERLOAD event package notification. */
extern FUNC(void, XCP_CODE)  Xcp_EmitDAQOverloadEvent(void);
#endif

/** \brief  Function to set a DAQ list sampling event.
 **
 ** This function is called by Xcp_SetEvent to set an event for the given event channel
 ** to trigger the sampling of the associated DAQ list(s).
 **
 ** \param[in]  EventId  XCP event channel ID.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Xcp_ReturnType, XCP_CODE) Xcp_SetEvent_Internal
(
  uint16 EventId
);

#if (XCP_SUPPRESS_TX_SUPPORT == STD_ON)
/**
 ** \brief This API is used to turn on and off of the TX capabilities of used communication
 ** bus channel in XCP module.
 **
 **  It is called by Xcp_SetTransmissionMode API.
 **
 ** \param[in] Channel The Network channel for the used bus communication
 ** \param[in] Mode Enabled or disabled Transmission mode Parameters
 **
 ** \ServiceID{::XCP_SID_SET_TRANSMISSION_MODE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
FUNC(void, XCP_CODE)  Xcp_SetTransmissionMode_Internal
(
  NetworkHandleType Channel,
  Xcp_TransmissionModeType Mode
);
#endif

#if (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON)
/** \brief Callback for STORE_CAL_REQ in SET_REQUEST command
 **
 ** This function is called by Xcp_SetReqStoreCalReqCbk
 **
 ** \param[in]   uint8 ServiceId - Unique Service ID of NVRAM manager service
 ** \param[in]   NvM_RequestResultType JobResult - Covers the job result of the previous
 **                                                processed single block job
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, XCP_APPL_CODE) Xcp_SetReqStoreCalReqCbk_Internal
(
  uint8 ServiceId,
  NvM_RequestResultType JobResult
);
#endif

#if (XCP_ENABLE_XCP_CONTROL_API == STD_ON)
/**
 ** \brief Internal function to perform an XCP disable or enable depending on on the
 **  command passed.
 **
 ** It is called from Xcp_Control function.
 **
 ** \param[in] command gives the command to enable or disable the Xcp module.
 **
 ** \ServiceID{::XCP_SID_STATE_CONTROL}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_CODE) Xcp_Control_Internal( uint8 command );
#endif

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON && XCP_REMOTE_DESTINATIONS == STD_ON)
/** \brief Function to send requests from Master partition to the partion that has
 ** the active connection.
 **
 ** \param[in]  data  Specific data for the requested API.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, XCP_CODE) Xcp_RemoteDispatch_Handler(Xcp_RemoteDispatchType data);
#endif

/** \brief Perform the initialization of the XCP module.
 **
 ** This function is invoked by Xcp_Init.
 **
 ** \param[in]  Xcp_ConfigPtr  Pointer to a selected configuration structure.
 **
 ** \ServiceID{::XCP_SID_INIT}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_CODE) Xcp_Init_Internal( P2CONST(Xcp_ConfigType, AUTOMATIC, XCP_APPL_CONST) Xcp_ConfigPtr );

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
/** \brief Perform the initialization of specific data from the Slave partition.
 **
 ** \param[in]  Xcp_ConfigPtr  Pointer to a selected configuration structure.
 **
 ** \ServiceID{::XCP_SID_INIT}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_CODE) Xcp_Init_OnSlave( void );
#endif

#if ((XCP_BSW_DISTRIBUTION_ENABLED == STD_ON) && (XCP_STORE_DAQ_SUPPORTED == STD_ON))
/** \brief Function to invoke NvM write block on the Master Partition.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, XCP_CODE) Xcp_NvMRequestHandler(void);
#endif

#if (XCP_MAX_DAQ != 0U)

/** \brief Function to Reset the DAQ Lists and Events
 **
 ** This function resets Running, Selected and Configured
 ** status of all DAQ Lists in XCP slave after a disconnect. */
extern FUNC(void, XCP_CODE) Xcp_ResetEventProcessor(void);

/** \brief Function to Reset a DAQ List
 **
 ** This function resets the configured status, running status
 ** and selected status of a DAQ List in XCP Slave
 **
 ** \param[in] DaqListNumber  The DAQ List number to be reset
 */
extern FUNC(void, XCP_CODE) Xcp_ResetDaqListData(Xcp_DaqIdType DaqListNumber);

/** \brief Function to process the one Event.
 **
 ** param[in] EventId Id of the event which has to be processed
 **/
extern FUNC(void, XCP_CODE) Xcp_ProcessEvents(uint16 EventId);

#if (XCP_MAX_MF_PROCESSED_EVENTS > 0)
/** \brief Function to process events from Xcp_MainFunction context */
extern FUNC(void, XCP_CODE) Xcp_ProcessMFEvents(void);
#endif

/** \brief Function to return whether the Priority Queue has entry
 ** \retval  TRUE if Daq Priority queue has entry
 ** \retval  FALSE if Daq Priority queue has no entry
 ** \return whether the Daq Priority queue has entry or not
 **  */
extern FUNC(boolean, XCP_CODE) Xcp_IsDaqPrioQueueEmpty(void);

/** \brief Function to get the size of the next CTO to send.
 **
 ** \return The size of next command response CTO in Event CTO queue */
extern FUNC(PduLengthType, XCP_CODE) Xcp_GetDtoLength(void);

/** \brief Function to Get DTO Data.
 **
 ** Use this function to Get DTO Data from Mirror.
 ** DTO data bytes will be copied from Mirror considering information from first location from Priority queue.
 ** Header information regarding PID will be copied from DaqRuntimeDataArray and
 ** time stamp will be copied from Xcp_DaqRunTimeInfo.
 ** Number of entry from priority queue will be updated.
 **
 ** \param[in]  DataLength    DTO length
 ** \param[out] BufferPtr     Buffer to which DTO shall be stored.
 */
extern FUNC(void, XCP_CODE) Xcp_GetDtoData
(
  PduLengthType DataLength,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) BufferPtr
);

/** \brief Function to unqueue a DTO entry from Xcp_DaqPrioQueue */
extern FUNC(void, XCP_CODE) Xcp_ClearDTOEntryFromDaqPrioQueue(void);

/** \brief Function to flush DTO runtime data */
extern FUNC(void, XCP_CODE) Xcp_FlushDTORunTimeData(void);

/** \brief Function to find the DaqId associated with a DTO in DTO Queue
 **
 ** This function returns the DaqId associated with a DTO in DTO Queue
 ** when the pointer to the DTO entry is passed as the parameter.
 ** The Daq Id is found based on the identification type used.
 **
 ** \param[inout] DtoPtr  The pointer to the DTO entry in DTO Queue
 ** \param[out] DaqID     The Daq Id associated with the DTO
 ** \param[out] OdtIndex  The ODT Index associated with the DTO
 */
extern FUNC(void, XCP_CODE) Xcp_GetDaqIDFromDto
(
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) DtoPtr,
  P2VAR(Xcp_DaqIdType, AUTOMATIC, AUTOMATIC) DaqID,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) OdtIndex
);

/** \brief Function to clear all stopped DTOs from Queue.
 **
 ** Use this function to clear all pending DTOs associated with
 ** inactive DAQ Lists so that the trasmission of the DTOs
 ** is stopped.
 **/
extern FUNC(void, XCP_CODE) Xcp_ClearStoppedDtos(void);

/** \brief Function to clear all DTOs from Queue for
 **        the given DaqList Id.
 **
 ** \param[in] DaqId Id of the daq list for which the DTOs
 **            will be eliminated from daq list prio list
 **/
extern FUNC(void, XCP_CODE) Xcp_ClearDaqListDtos(Xcp_DaqIdType DaqId);

/** \brief Function to clear an entry from DAQ Priority Queue.
 **
 ** Use this function to clear an entry from Xcp_DaqPrioQueue
 **
 ** \param[in] Position  Position which will be removed from Queue
 **/
extern FUNC(void, XCP_CODE) Xcp_ClearDaqPrioQueueEntry
(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) Position
);

/** \brief Function to reset all event channels runtime information
 **
 ** Use this function to reset all event channels: event channel is not set, next DAQ and ODT index
 ** to be processed is 0  */
extern FUNC(void, XCP_CODE) Xcp_ResetEventChannels(void);

#endif /* #if (XCP_MAX_DAQ != 0U) */

#if (XCP_PGM_SUPPORTED == STD_ON)

/** \brief Function used to set abort programming session request */
extern FUNC(void, XCP_CODE) Xcp_SetAbortProgrammingSession(void);
#endif

#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
/** \brief Function check if at least one event main function is currently being processed
 **
 ** Use this function to prevent interfering with an ongoing event main function.
 **
 ** \return TRUE, if at least one event main function is currently being processed and FALSE
 ** otherwise
 **  */
extern FUNC(boolean, XCP_CODE)  Xcp_IsAtLeastOneEventMainFunctionProcessing(void);
#endif

#if (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)

/** \brief Function to return whether a programming is currently in block mode
 **
 ** \return TRUE if programming is in block mode, FALSE otherwise
 **  */
extern FUNC(boolean, XCP_CODE) Xcp_IsProgrammingBlockMode(void);

/** \brief Function to mark the beginning of an block mode programming
 **
 ** This means, that from now on, only PROGRAM_NEXT commands are accepted.
 **  */
extern FUNC(void, XCP_CODE) Xcp_SetProgrammingBlockMode(void);

#endif /* XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON */


#if (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)

/** \brief Function to return whether a download in block mode is currently active
 **
 ** Use this function to check if the block mode transfer for DOWNLOAD is currently active. Use this
 ** to validate incoming commands.
 **
 ** \retval TRUE   Download is in block mode
 ** \retval FALSE  Download is not in block mode
 ** \return TRUE if download is in block mode transfer, FALSE otherwise
 **  */
extern FUNC(boolean, XCP_CODE) Xcp_IsDownloadInBlockMode(void);

/** \brief Function to set the download in block mode flag
 **
 ** The function will set the download in block mode flag - meaning that we just received a DOWNLOAD
 ** command and we can accept further DOWNLOAD_NEXT commands
 **  */
extern FUNC(void, XCP_CODE) Xcp_SetDownloadInBlockMode(void);

#endif /* XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON */

#if (XCP_ASYNCH_CRC_SUPPORT == STD_ON)
/** \brief Function used to get the status of asynchronous checksum job request.
 **
 ** \retval E_OK     an asynchronous checksum job request is active.
 ** \retval E_NOT_OK no asynchronous checksum job request is active.
 **/
extern FUNC(Std_ReturnType, XCP_CODE) Xcp_GetBuildChecksumReqStatus(void);
#endif /* (XCP_ASYNCH_CRC_SUPPORT == STD_ON) */

#if (XCP_BUILD_CHECKSUM_SUPPORT == STD_ON)
/** \brief Function used to Calculate BUILD_CHECKSUM request.
 **
 ** Use this function to execute a BUILD_CHECKSUM command request.
 **
 **/
extern FUNC(void, XCP_CODE) Xcp_CalculateCRC(void);
#endif /* (XCP_BUILD_CHECKSUM_SUPPORT == STD_ON) */

#if (XCP_EVENT_PACKET_ENABLED == STD_ON)

/** \brief Function to Enqueue one CTO from event package into event Cto queue.
 **
 ** Use this function to enqueue one CTO from the response CTO queue.  The
 ** Event Cto package will be enqueued only if it is not already into
 ** Event Cto queue.
 **
 ** \param[in] EventCtoPtr The Event Cto package which has to be enqueued
 **/
extern FUNC(void, XCP_CODE)  Xcp_EnqueueCtoEvent
(
  P2CONST(Xcp_EventCTOType, AUTOMATIC, XCP_APPL_DATA) EventCtoPtr
);

/** \brief Function to Dequeue one CTO from the response CTO Queue.
 **
 ** Use this function to dequeue one CTO from the response CTO queue.  CTO
 ** data bytes will be copied byte by byte from queue head.  Queue head will
 ** be updated to the next CTO length information in the queue and rolled over
 ** if necessary.  Number of bytes in queue will also be updated.
 **
 ** \param[in] DataLength How much data shall be dequeued
 ** \param[in] BufferPtr  Buffer to which EventCTO shall be dequeued */
extern FUNC(void, XCP_CODE) Xcp_DeQueueEventCtoQueue
(
  PduLengthType DataLength,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) TxBufferPtr
);

/** \brief Function to check whether command response CTO Queue is empty.
 **
 ** \retval TRUE   The event queue is empty
 ** \retval FALSE  The event queue is not empty
 ** \return The status of event queue */
extern FUNC(boolean, XCP_CODE) Xcp_IsEventCtoQueueEmpty(void);

/** \brief Function to get the size of the next Event CTO to send.
 **
 ** \return The size of next command response Event CTO in Event CTO queue */
extern FUNC(PduLengthType, XCP_CODE) Xcp_GetEventCtoLength(void);

/** \brief Function to flush all pending event CTOs
 **
 ** Use this function flush all pending event CTOs.
 ** Flags for all event codes will be cleared. */
extern FUNC(void, XCP_CODE) Xcp_FlushEventCTOQueue(void);

#endif /* XCP_EVENT_PACKET_ENABLED == STD_ON */

/** \brief Function to start the autonomous disconnect process
 **
 ** \param[in] EmitSessionTerminatedEvent whether to send the EV_SESSION_TERMINATED event
 **            before disconnecting
 ** */
extern FUNC(void, XCP_CODE) Xcp_PrepareForAutonomousDisconnect
(
  boolean EmitSessionTerminatedEvent
);

#if ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON))

/** \brief Function to get the data to be passed through TriggerTransmit
 ** function
 **
 ** Use this function to get the data to be passed through TriggerTransmit
 ** function.
 **
 ** \param[in] TxPduId id used to link to the buffer
 ** \param[out] PduInfoPtr The address of the buffer in which data needs to be
 ** \return  E_OK, if the output buffer was successfully written
 **          E_NOT_OK, if no data is available to be written into the output buffer
 ** copied */
extern FUNC(Std_ReturnType, XCP_CODE) Xcp_ProvideTxData
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr
);

/** \brief Function to determine whether XCP transmit processor is waiting for
 ** a transmit trigger.
 **
 ** This functions checks whether the transmit processor state machine is
 ** expecting transmit trigger invocation for a previous transmit request. Use
 ** this function to ward off any unexpected trigger requests from the lower
 ** layer.
 **
 ** \return State machine status
 ** \retval TRUE    Transmit processor is expecting a transmit trigger.
 ** \retval FALSE   Transmit processor is not expecting a transmit trigger. */
extern FUNC(boolean, XCP_CODE) Xcp_IsWaitingForTrigger(void);

#endif /* ((XCP_TRIGGER_TX_API_ENABLED == STD_ON) || (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON)) */

/** \brief Function to indicate that the Pdu transmission was successfully
 **
 ** Use this function to indicate confirmation via IndicateConfirmation function
 **
 ** \param[in] TxPduId internal Id of the Tx Pdu
 * */
extern FUNC(void, XCP_CODE) Xcp_IndicateTxConfirmation
(
  PduIdType TxPduId
);

#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
/** \brief Function to check the length of a received Stim package.
 **
 ** This function is called by Xcp_ProcessRxPdu() function only,
 ** the function call being critical section protected.
 ** \param[in]  XcpRxSduPtr Pointer to the DTO data
 ** \param[out] DataLengthMin Pointer to the calculated minimum data length
 ** \param[out] DataLengthMax Pointer to the calculated maximum data length
 ** */
extern FUNC(void, XCP_CODE) Xcp_GetStimLength
(
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) XcpRxSduPtr,
  P2VAR(PduLengthType, AUTOMATIC, XCP_APPL_DATA) DataLengthMin,
  P2VAR(PduLengthType, AUTOMATIC, XCP_APPL_DATA) DataLengthMax
);

/** \brief Function to insert an incoming DTO packet into the STIM Buffer
 **
 ** This function is used to check an incoming DTO packet and insert
 ** it into the STIM Buffer.
 **
 ** \param[in] SduDataPtr  DTO packet data to be processed. */
extern FUNC(void, XCP_CODE) Xcp_InsertSTIM
(
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) SduDataPtr
);

/** \brief Function to process the stimulation of data
 **
 ** This function is used to stimulate the data stored in the STIM Buffer
 ** for one or more ODTs of one or more DAQ Lists based on consistency
 **
 ** \param[in] EventID      Event ID to process
 */
extern FUNC(void, XCP_CODE) Xcp_StimulateData
(
  uint16 EventID
);

#endif /* XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK */

#if (XCP_STORE_DAQ_SUPPORTED == STD_ON)
/**
 ** \brief Function to prepare the selected stored DAQ lists
 **  The function will determine which DAQ lists were selected to be stored in NV memory and will
 **  make them ready to be sampled.
 **  */
extern FUNC(void, XCP_CODE) Xcp_PrepareStoredDaqLists(void);

/**
 ** \brief Function to trigger the resume mode.
 **  The function will determine which DAQ lists need to be started in resume mode, prepare them for
 **  immediate transmission and queue their associated events.
 **  */
extern FUNC(void, XCP_CODE) Xcp_PrepareStoredDaqListsResume(void);

#endif /* XCP_STORE_DAQ_SUPPORTED == STD_ON */

#if (XCP_DAQ_COUNT > 0U)

/**
 ** \brief Function to return a pointer to a DAQ list from the corresponding area where it is stored.
 **
 **  This function will return a pointer to the DAQ list corresponding to the received DAQ list number
 **  from two possible areas, depending on the configuration type:
 **   - if it is predefined, from the Xcp_DaqLists.Xcp_NonDynamicDaqLists structure
 **   - if it is dynamic, from the area allocated for dynamic DAQ lists
 **
 **  If the configuration type is static, this function will be available as a macro, as then there
 **  would be only possible area from where to fetch the DAQ list.
 **
 **  \param[in] DaqListNumber   The DAQ list number based upon the DAQ list is retrieved.
 **
 ** \return A pointer to the DAQ list found.
 **  */
extern FUNC_P2VAR(Xcp_DaqType, XCP_VAR_CLEARED, XCP_CODE) Xcp_GetDaqListPtr
(
  Xcp_DaqIdType DaqListNumber
);
/**
 ** \brief Function to return a pointer to a Dto Mirror Data from the corresponding area where it is stored.
 **
 **  This function will return a pointer to the Mirror corresponding to the received DAQ list number and OdtIndex.
 **  from two possible areas, depending on the configuration type:
 **   - if it is predefined, from the Xcp_DTOEntryMirror.Xcp_NonDynamicOdtMirrorDataArray
 **   - if it is dynamic, from the Xcp_DTOEntryMirror.Xcp_DynamicOdtMirrorDataArray
 **
 **  \param[in] DaqID   The DAQ list number.
 **  \param[in] OdtIndex   The ODT number.
 **
 ** \return A pointer to the Mirror found.
 **  */
extern FUNC_P2VAR(uint8, XCP_VAR_CLEARED, XCP_CODE) Xcp_GetMirrorPtr
(
  Xcp_DaqIdType DaqID,
  Xcp_MirrorSizeType MirrorOffsetPosition
);
/**
 ** \brief Function to return the number of dynamically configured DAQ lists.
 **
 ** If the dynamic DAQ list configuration is invalid (e.g. ERR_MEMORY_OVERFLOW was thrown during
 ** the configuration), this function will return 0.
 **
 ** \return The number of dynamically configured DAQ lists.
 **  */
extern FUNC(Xcp_DaqIdType, XCP_CODE) Xcp_GetNoOfDynamicDaqLists( void );

#endif /* (XCP_DAQ_COUNT > 0U) */

#if (XCP_MAX_CYCLIC_EVENT_CHANNEL > 0U)
/** \brief Sets all the cyclic events
 **/
extern FUNC(void, XCP_CODE) Xcp_SetCyclicEvents( void );

#endif /* XCP_MAX_CYCLIC_EVENT_CHANNEL > 0U */

#if (XCP_NUMBER_OF_FRAME_COUNTERS > 0U)
/** \brief Initializes the frame counter to 0. */
extern FUNC(void, XCP_CODE) Xcp_InitFrameCtr(void);
#endif /* XCP_NUMBER_OF_FRAME_COUNTERS > 0U */

#if (XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON)

/** \brief Function which copies the remote address from the socket having the SrcSoConId to
 **        the socket having the DstSoConId.
 **
 ** The copy of the remote address is done with/without reseting the port value.
 **
 ** \param[in]   SrcSoConId The socket ID which is the source of the remote address.
 ** \param[in]   DstSoConId The socket ID which is the destination of the remote address.
 ** \param[in]   ConnectCmd: - TRUE when CONNECT command was called before any other information received on the bus;
 **                           - FALSE when the CONNECT command wasn't called before any other received information.
 */
extern FUNC(void, XCP_CODE) Xcp_CopySoAdRemoteAddress
(
  SoAd_SoConIdType SrcSoConId,
  SoAd_SoConIdType DstSoConId,
  boolean ConnectCmd
);

#if (XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON)

/** \brief Function which stores the multicast IPv4 address and port in order to be used later
 **        when the response to the multicast command is sent.
 **
 ** \param[in]   PortPtr    Pointer to an uint16 variable representing the multicast port.
 ** \param[in]   AddressPtr Pointer to an uint32 variable representing the multicast IPv4 address.
 */
extern FUNC(void, XCP_CODE) Xcp_SetMulticastAddress
(
  P2VAR(uint16, AUTOMATIC, XCP_CODE) PortPtr,
  P2VAR(uint32, AUTOMATIC, XCP_CODE) AddressPtr
);

/** \brief Function which changes the address and port of the Tx Socket to the multicast address and
 **        port which were previously stored using Xcp_SetMulticastAddress. It also stores the current
           Tx Socket address and port in order to be restored after multicast response is sent.
 **
 ** \return Whether the socket address and port were successfully changed.
 */
extern FUNC(Std_ReturnType, XCP_CODE) Xcp_SoAdChangeTxToMulticast(void);

/** \brief Function which changes the address and port of the Tx Socket to previously stored
 **        unicast address and port
 **
 ** \return Whether the socket address and port were successfully changed.
 */
extern FUNC(Std_ReturnType, XCP_CODE) Xcp_SoAdChangeTxToUnicast(void);

/** \brief Function which retrieves the local IPv4 address and port of the Rx Socket.
 **
 ** \param[in]   IsIPv6         Pointer to boolean variable which is set to true if the
 **                             current connection is of type IPv6.
 ** \param[in]   AddressPtr     Pointer to a TcpIp_SockAddrInetType struct where the IPv4 address
 **                             and port of local Rx Socket are retrieved.
 **
 ** \return Whether the IPv4 address and port were successfully retrieved.
 */
extern FUNC(Std_ReturnType, XCP_CODE) Xcp_SoAdGetLocalAddress
(
    P2VAR(TcpIp_SockAddrInetType, AUTOMATIC, XCP_CODE) AddressPtr
);

/** \brief Function to determine if the next CTO to be retrieved is a multicast response.
 **
 ** \return whether the next CTO to be retrieved is a multicast response or not */
extern FUNC(boolean, XCP_CODE) Xcp_IsMulticastResponse(void);

#endif /* XCP_IMPLEMENT_MULTICAST_COMMANDS == STD_ON */
#endif /* XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON */

/**
 ** \brief Function to perform an XCP disconnect.
 **  The function will terminate the current XCP session and
 **  reset internal data structures and sub processors.
 **  */
extern FUNC(void, XCP_CODE) Xcp_Disconnect(void);

/** \brief Function to get the command processor current state
 **
 ** \return The state of the command processor
 ** \retval XCP_CMDPROCESSOR_IDLE      The command processor's state is IDLE
 ** \retval XCP_CMDPROCESSOR_BUSY      The command processor's state is BUSY
 **
 */
extern FUNC(Xcp_ProcessorStateType, XCP_CODE) Xcp_GetCommandProcessorState(void);

/** \brief Function to get the transmit processor current state
 **
 ** \return The state of the command processor
 ** \retval XCP_CMDPROCESSOR_IDLE      The command processor's state is IDLE
 ** \retval XCP_CMDPROCESSOR_BUSY      The command processor's state is BUSY
 **
 */
extern FUNC(Xcp_ProcessorStateType, XCP_CODE) Xcp_GetTransmitProcessorState(void);

/** \brief Function to send next XCP packet
 **
 ** Use this function to transmit any pending XCP packet.
 **
 ** \param[in] TxStateContext The context from which the function is called:
 **            XCP_CMDPROCESSOR_IDLE -> from Xcp_MainFunction() context;
 **            XCP_CMDPROCESSOR_BUSY -> from Xcp_TxConfirmation() context.
 ** */
extern FUNC(void, XCP_CODE) Xcp_SendNextPacket
(
  Xcp_ProcessorStateType TxStateContext
);

/** \brief Function to Process commands
 **
 ** This function implements the command processor state machine.
 ** This function shall be invoked cyclically from the main function.
 **/
extern FUNC(void, XCP_CODE) Xcp_ProcessCommandQueue(void);

/** \brief Function to copy Rx Pdu information to Rx Pdu buffer
 **
 ** \param[in]     XcpRxPduId  The Rx PDU Id
 ** \param[in]     XcpRxPduPtr Pdu information associated to the Rx Pdu Id
 */
extern FUNC(void, XCP_CODE) Xcp_ReceiveRxData
(
  PduIdType XcpRxPduId,
  P2CONST(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
);

/** \brief Function to Process reception
 **
 ** This function implements the reception of incoming PDUs.
 ** This function shall be invoked cyclically.
 **
 ** \param[in]  SidId The Function Id from where the function is called
 **/
extern FUNC(void, XCP_CODE) Xcp_ProcessReception
(
  uint8 SidId
);

/** \brief Function to terminate the current session
 **
 ** This function will flush command queue, CTO Queue, DTO Queue and all
 ** pending event packets.
 **
 ** This function must only be executed within a critical section.
 **  */
extern FUNC(void, XCP_CODE) Xcp_TerminateSession(void);

#if (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON)
/** \brief Function called when a request to enable/disable
 ** an XCP Pdu is received.
 **
 ** This function only checks the parameters received by a command.
 **
 ** \param[in]  ParamPtr       Pointer to received data buffer
 ** \param[in]  Length         Length of the received data buffer
 ** \param[in]  RespLength     Length of the response data buffer
 ** \param[out] SubCmdResponse Pointer to the response buffer
 **
 ** \return whether function executed correctly or not
 **  */
extern FUNC(uint8, XCP_CODE) Xcp_EnableCommunicationChannel
(
  P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) ParamPtr,
  uint8 Length,
  P2VAR( uint8, AUTOMATIC, XCP_APPL_DATA ) SubCmdResponse,
  uint8 RespLength
);

/** \brief Function called when to enable/disable
 ** a transmission XCP Pdu is received.
 **
 ** This function only checks the parameters received by a command.
 **
 ** \param[in]  ParamPtr       Pointer to received data buffer
 **
 ** \return whether function executed correctly or not
 **  */
extern FUNC(uint8, XCP_CODE) Xcp_EnableTxCommunicationChannel
(
  P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) ParamPtr
);

/** \brief Function called to enable/disable
 ** a reception  XCP Pdu is received.
 **
 ** This function only checks the parameters received by a command.
 **
 ** \param[in]  ParamPtr       Pointer to received data buffer
 **
 ** \return whether function executed correctly or not
 **  */
extern FUNC(uint8, XCP_CODE) Xcp_EnableRxCommunicationChannel
(
  P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) ParamPtr
);
#endif /* XCP_DYNAMIC_PDU_SUPPORTED == STD_ON */

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))


/** \brief Function to determine if the buffer associated with the PDU Id can accept the DAQ
 ** List that is currently being transmitted
 **
 ** \param[in] PduId - the PDU Id
 ** \param[in] PacketType - the PacketType - STIM or DAQ
 ** \param[in] DaqListId - the DAQ Id which is transmitted/received
 ** \return whether the PDU can accept the DAQ List
 ** \retval TRUE the PDU can accept the DAQ List
 ** \retval FALSE the PDU cannot accept the DAQ List
 */
extern FUNC(boolean, XCP_CODE) Xcp_IsFlxDaqIdAllowedForTheBuffer
(
  PduIdType PduId,
  uint8 PacketType,
  Xcp_DaqIdType DaqListId
);

#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
/** \brief Function called to set whether the PDU
 ** should accept DAQ packet types or not
 **
 **
 ** \param[in]  FlagID                Index in the bitfield
 ** \param[in]  ShouldBeEnabled       TRUE if DAQ should be enabled, FALSE otherwise
 **
 **  */
extern FUNC(void, XCP_CODE) Xcp_SetPacketEnableStatusDaq
(
  PduIdType FlagID,
  boolean ShouldBeEnabled
);
#endif

#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
/** \brief Function called to set whether the PDU
 ** should accept STIM packet types or not
 **
 **
 ** \param[in]  FlagID                Index in the bitfield
 ** \param[in]  ShouldBeEnabled       TRUE if STIM should be enabled, FALSE otherwise
 **
 **  */
extern FUNC(void, XCP_CODE) Xcp_SetPacketEnableStatusStim
(
  PduIdType FlagID,
  boolean ShouldBeEnabled
);
#endif

#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
/** \brief Function called to set whether the PDU
 ** should accept EV_SERV packet types or not
 **
 **
 ** \param[in]  FlagID                Index in the bitfield
 ** \param[in]  ShouldBeEnabled       TRUE if EV_SERV should be enabled, FALSE otherwise
 **
 **  */
extern FUNC(void, XCP_CODE) Xcp_SetPacketEnableStatusEvServ
(
  PduIdType FlagID,
  boolean ShouldBeEnabled
);

/** \brief Function called to set whether the
 ** EV_SERV packet types are activated or not
 ** for the current buffer
 **
 ** \param[in]  FlagID                Index in the bitfield
 ** \param[in]  ShouldBeEnabled       TRUE if EV_SERV should be activated, FALSE otherwise
 **
 **  */
extern FUNC(void, XCP_CODE) Xcp_SetPacketActivateStatusEvServ
(
  PduIdType FlagID,
  boolean ShouldBeActivated
);

/** \brief Function called to get whether the
 ** EV_SERV packet types are enabled or not
 ** for the current buffer
 **
 ** \param[in]  TxPduIdx        Index in Xcp_TxLPduIdConf
 ** \return TRUE if EV_SERV packet is enabled, FALSE otherwise
 **  */
FUNC(boolean, XCP_CODE) Xcp_GetPacketStatusEvServEnabled
(
  uint8 TxPduIdx
);
#endif

/** \brief Function to adjust the Rx buffer lengths based on the new value for a buffer.
 **
 ** \param[in] bufferId - the buffer which has now a new length
 ** \param[in] newLength - new value for the length (MAX_FLX_LEN_BUF_x)
 **/
extern FUNC(void, XCP_CODE) Xcp_SetRxFlxBufferLength(uint8 bufferId, uint8 newLength);

/** \brief Function to adjust the Tx buffer lengths based on the new value for a buffer.
 **
 ** \param[in] bufferId - the buffer which has now a new length
 ** \param[in] newLength - new value for the length (MAX_FLX_LEN_BUF_x)
 **/
extern FUNC(void, XCP_CODE) Xcp_SetTxFlxBufferLength(uint8 bufferId, uint8 newLength);

/** \brief Function to return the maximum buffer length (MAX_FLX_LEN_BUF_x) for a TX buffer
 **
 ** \param[in] bufferId - the buffer ID for which the value should be returned
 ** \return The maximum buffer length
 **/
extern FUNC(uint8, XCP_CODE) Xcp_GetTxFlxBufferLength(uint8 bufferId);

/** \brief Function to return the maximum buffer length (MAX_FLX_LEN_BUF_x) for a RX buffer
 **
 ** \param[in] pduId - the pdu for which the value should be returned
 ** \return The maximum buffer length of this PDU
 **/
extern FUNC(uint8, XCP_CODE) Xcp_GetRxFlxBufferLength(uint8 bufferId);


/** \brief Function to re(set) the LPDU to a hw buffer if the buffer has the
 **  necessary preconfigured values
 **
 ** \param[in] bufferId - buffer which needs to be resets
 ** \param[in] bufferLength - buffer length (MAX_FLX_LEN_BUF_x)
 ** \return Whether the reset was successful or not
 **/
FUNC(Std_ReturnType, XCP_CODE) Xcp_SetDefaultLpduToBuffer(uint8 bufferId, uint8 bufferLength);

#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON) */

#if (XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON)
/** \brief Function used to check if the requested memory area access is allowed
 **
 ** \param[in] MemoryAreaAddress - Start address of the memory area that needs to be validated
 ** \param[in] MemoryAreaLength - Length of the memory area that needs to be validated
 ** \param[in] MemoryAreaType - Type (read/write) of the memory area that needs to be validated
 ** \param[in] MemoryAreaScope - Scope (commands/DAQ) of the memory area that needs to be validated
 ** \return Whether the  requested memory area access is allowed
 **/
extern FUNC(boolean, XCP_CODE) Xcp_IsMemoryAreaAccessible
(
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) MemoryAreaAddress,
  uint32 MemoryAreaLength,
  uint8 MemoryAreaType,
  uint8 MemoryAreaScope
);

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
extern FUNC_P2CONST(Xcp_MemoryAreaType, AUTOMATIC, XCP_CODE) Xcp_FindMemoryArea
(
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) MemoryAreaAddress,
  uint32 MemoryAreaLength,
  uint8 MemoryAreaType,
  uint8 MemoryAreaScope,
  P2VAR (uint8, AUTOMATIC, XCP_APPL_DATA) MemoryAreaIdx
);
#endif

#endif /* XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON */

/** \brief Function to return the maximum allowed length of a PDU
 **
 ** \param[in] pduId - the pdu for which the value should be returned
 ** \return The  maximum length of this PDU
 **/
extern FUNC(PduLengthType, XCP_CODE) Xcp_GetPduLength(PduIdType pduId);

/** \brief Function called to (re)initialize the TxPDU flags.
 ** Will get the information from the configuration depending on
 ** the current active connection
 ** \param[in]   ConnectionId - The current connection ID
 **
 **  */
extern FUNC(void, XCP_CODE) Xcp_InitTxPduFlags(uint8 ConnectionId);


/** \brief Function called to (re)initialize the RxPDU flags.
 ** Will get the information from the configuration depending on
 ** the current active connection
 **
 ** \param[in]   ConnectionId - The current connection ID
 **  */
extern FUNC(void, XCP_CODE) Xcp_InitRxPduFlags(uint8 ConnectionId);

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
/** \brief Function called to initialize the data of Request Buffer
 **  */
extern FUNC(void, XCP_CODE) Xcp_ResetRequestData(void);

/** \brief Function used to enqueue the requests for MemoryProxy
 **
 ** \param[in] DaqId  The Id of the Daq
 ** \param[in] OdtEntryLength  The size of the OdtEntry
 ** \param[in] DTOMirrorWritePos  The position from Mirror where the data will be writen/read
 ** \param[in] AppId  The ApplicationId where the memmory access shall be done
 ** \param[in] Type  The type of request DAQ or STIM
 ** \param[in] MemoryAreaAddress  The address of the memory area
 **
 ** */
extern FUNC(void, XCP_CODE) Xcp_MemoryProxyEnqueueRequest
(
  Xcp_DaqIdType DaqId,
  uint8 OdtEntryLength,
  Xcp_MirrorSizeType DTOMirrorWritePos,
  uint8 AppId,
  uint8 Type,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) MemoryAreaAddress,
  uint8 bitMask
);
#endif

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#if ((XCP_MAX_DAQ != 0U) && (XCP_MAX_DAQ > XCP_DAQ_COUNT) && (XCP_NO_OF_NON_DYNAMIC_ODTENTRIES > 0) && (XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON))

#define XCP_START_SEC_CONST_8
#include <Xcp_MemMap.h>

/** \brief The lengths of all ODT entries */
extern CONST(uint8, XCP_CONST) Xcp_OdtLengthTableROM[XCP_NO_OF_NON_DYNAMIC_ODTENTRIES];

#if (XCP_ENABLE_BIT_OFFSET == STD_ON)
/** \brief The bit offsets of all ODT entries */
extern CONST(uint8, XCP_CONST) Xcp_OdtBitOffsetTableROM[XCP_NO_OF_NON_DYNAMIC_ODTENTRIES];
#endif

#define XCP_STOP_SEC_CONST_8
#include <Xcp_MemMap.h>

#endif
#define XCP_START_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

#if (XCP_MAX_EVENT_CHANNEL != 0U)
/** \brief Event Information */
extern CONST(Xcp_EventInfoType, XCP_CONST) Xcp_EventInfo[XCP_MAX_EVENT_CHANNEL];

#if (XCP_MAX_MF_PROCESSED_EVENTS > 0)
/** \brief Configuration of Event Ids which  processed by the MainFunction,
 **        in the priority order */
extern CONST(uint16, XCP_CONST) Xcp_MFEventCfg[XCP_MAX_MF_PROCESSED_EVENTS];
#endif /* (XCP_MAX_MF_PROCESSED_EVENTS > 0) */

#if (XCP_MAX_CYCLIC_EVENT_CHANNEL > 0) && (XCP_MAX_DAQ > 0)
/** \brief Cyclic Events Ids which are set by MainFunction */
extern CONST(uint16, XCP_CONST) Xcp_MFCyclicEventCfg[XCP_MAX_CYCLIC_EVENT_CHANNEL];
#endif /* (XCP_MAX_CYCLIC_EVENT_CHANNEL > 0) */
#endif /* (XCP_MAX_EVENT_CHANNEL != 0U) */

#if ((XCP_MAX_DAQ != 0U) && (XCP_MAX_DAQ > XCP_DAQ_COUNT)) /* If we have static or predefined DAQ lists */
extern CONST(Xcp_NonDynamicDaqListsType, XCP_CONST) Xcp_DaqListsDefault;

#if ((XCP_NO_OF_NON_DYNAMIC_ODTENTRIES > 0) && (XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON))
/** \brief The addresses of all ODT entries */
extern CONSTP2VAR(uint8, XCP_CONST, XCP_APPL_DATA) Xcp_OdtAddressTableROM[XCP_NO_OF_NON_DYNAMIC_ODTENTRIES];
#endif

#endif
#if (XCP_PDU_ROUTING_TABLE == STD_ON)
#if ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON))
/** \brief Routing map for the outgoing CanIf Pdus. */
extern CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_CanIfRxLPduIdRouteConf[XCP_NO_OF_CANIF_RX_PDU_IDS];

/** \brief Routing map for the incoming CanIf Pdus. */
extern CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_CanIfTxLPduIdRouteConf[XCP_NO_OF_CANIF_TX_PDU_IDS];
#endif /* ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON)) */

#if (XCP_ON_FLEXRAY_ENABLED == STD_ON)
/** \brief Routing map for the outgoing FrIf Pdus. */
extern CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_FrIfRxLPduIdRouteConf[XCP_NO_OF_FRIF_RX_PDU_IDS];

/** \brief Routing map for the incoming FrIf Pdus. */
extern CONST(Xcp_LPduIdRouteConfType, XCP_CONST ) Xcp_FrIfTxLPduIdRouteConf[XCP_NO_OF_FRIF_TX_PDU_IDS];
#endif /* XCP_ON_FLEXRAY_ENABLED == STD_ON */

#if (XCP_ON_ETHERNET_ENABLED == STD_ON)
/** \brief Routing map for the outgoing SoAd Pdus. */
extern CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_SoAdRxLPduIdRouteConf[XCP_NO_OF_SOAD_RX_PDU_IDS];

/** \brief Routing map for the incoming SoAd Pdus. */
extern  CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_SoAdTxLPduIdRouteConf[XCP_NO_OF_SOAD_TX_PDU_IDS];
#endif /* XCP_ON_ETHERNET_ENABLED == STD_ON */

#if (XCP_ON_CDD_ENABLED == STD_ON)
/** \brief Routing map for the outgoing Cdd Pdus. */
extern CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_CddRxLPduIdRouteConf[XCP_NO_OF_CDD_RX_PDU_IDS];

/** \brief Routing map for the incoming Cdd Pdus. */
extern CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_CddTxLPduIdRouteConf[XCP_NO_OF_CDD_TX_PDU_IDS];
#endif /* (XCP_ON_CDD_ENABLED == STD_ON) */
#endif /* XCP_PDU_ROUTING_TABLE == STD_ON */

/** \brief Tx Pdu Ids configured by the transport layer.
 **
 ** This Ids are used store information regarding:
 ** * mapping PDU toward connection
 ** * destination PDU id into transport layer configuration
 ** * start position of the PDU data inside the Tx PDU buffer area
 ** * supported Xcp communication transfer object types. */
extern CONST(Xcp_TxLPduIdConfType, XCP_CONST )  Xcp_TxLPduIdConf[XCP_NO_OF_TX_PDU_IDS];

/** \brief Rx Pdu Ids configured by the transport layer.
 **
 ** This Ids are used store information regarding:
 ** * mapping PDU toward connection
 ** * start position of the PDU data inside the Rx PDU buffer area
 ** * supported Xcp communication transfer object types. */
extern CONST(Xcp_RxLPduIdConfType, XCP_CONST )  Xcp_RxLPduIdConf[XCP_NO_OF_RX_PDU_IDS];

#if (XCP_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON)
/** \brief List used to map TCP/IP socket connections towards Xcp connection Id. */
extern CONST(Xcp_TcpIpSocketConMapType, XCP_CONST)
       Xcp_TcpIpSocketConMapCfg[XCP_NO_OF_TCPIP_SOCKET_CON];
#endif

/** \brief Connection configuration info */
extern CONST(Xcp_ConnectionCfgType, XCP_CONST) Xcp_ConnectionCfg[XCP_NO_OF_CONNECTIONS];

#if (XCP_NUMBER_OF_USERCOMMANDS > 0U)
/** \brief User defined commands configuration */
extern CONST(Xcp_UserCommandConfType, XCP_CONST) Xcp_UserCommandConf[XCP_NUMBER_OF_USERCOMMANDS];
#endif /* XCP_NUMBER_OF_USERCOMMANDS > 0U */

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
/** \brief Xcp Flexray Buffer configuration */
extern CONST(Xcp_FlxBufType, XCP_CONST) Xcp_FlxBufConfig[XCP_NO_OF_FLX_BUF_CONFIGURED];
#endif

#if (XCP_NUMBER_OF_MEMORY_AREAS > 0U)
/** \brief Array holding the configured memory areas */
extern CONST(Xcp_MemoryAreaType, XCP_CONST) Xcp_MemoryAreas[XCP_NUMBER_OF_MEMORY_AREAS];
#endif

#define XCP_STOP_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

#define XCP_START_SEC_CONST_8
#include <Xcp_MemMap.h>

#if (XCP_ON_ETHERNET_ENABLED == STD_ON)

/** \brief Array that holds information about which connections are on IPv6 */
extern CONST(uint8, XCP_CONST) Xcp_Connections_IPv6[XCP_CONNECTION_FLAG_SIZE];

#endif

#define XCP_STOP_SEC_CONST_8
#include <Xcp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define XCP_START_SEC_VAR_8
#include <Xcp_MemMap.h>

#if (XCP_SUPPRESS_TX_SUPPORT == STD_ON)
/** \brief Flags to check wether the connections have TX enabled or not.
 ** Can be changed by Xcp_SetTransmissionMode() API */
extern VAR(uint8, XCP_VAR) Xcp_TransmissionDisabled[XCP_CONNECTION_FLAG_SIZE];
#endif

#define XCP_STOP_SEC_VAR_8
#include <Xcp_MemMap.h>



#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

#if (XCP_MAX_EVENT_CHANNEL != 0U)
/** \brief Event Details */
extern VAR(Xcp_EventType, XCP_VAR)
  Xcp_Event[XCP_MAX_EVENT_CHANNEL];
#endif


#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

#define XCP_START_SEC_VAR_8
#include <Xcp_MemMap.h>

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
/** \brief Flags signaling if a Main Function started and Memory Proxy did not
 ** finished the processing
 **/
extern VAR(uint8, XCP_VAR) Xcp_MemoryProxyErrorPending;

/** \brief Flags signaling if a Cyclic Event has been set while the response from
 ** Memory Proxy was not received
 **/
extern VAR(uint8, XCP_VAR) Xcp_CyclicEventOverloaded;
#endif

#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0)
/** \brief Flags signaling if an Event Main Function is currently executing or not */
extern VAR(uint8, XCP_VAR) Xcp_IsMFEventCurrentlyProcessing[XCP_EVENT_PROCESSING_FLAG_SIZE];

/** \brief Flag to disable the execution of any event main function: triggered when
 ** an autonomous disconnect is going to happen
 **/
extern VAR(boolean, XCP_VAR) Xcp_EventMainFunctionsExecutionIsAllowed;
#endif

#define XCP_STOP_SEC_VAR_8
#include <Xcp_MemMap.h>

#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
/** \brief Variable holding the Id of the Application on which the Connection is active */
extern VAR(Atomic_t, XCP_VAR) Xcp_PartitionOfActiveConnection;
extern VAR(Atomic_t, XCP_VAR) Xcp_ActiveConnectionId;
#endif

#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>

#if (XCP_DAQ_COUNT > 0U)
/** \brief Runtime info regarding the configured Dynamic DAQ lists */
extern VAR(Xcp_DynamicDaqListsType, XCP_VAR_CLEARED) Xcp_DynamicInfo;
#endif /* (XCP_DAQ_COUNT > 0U) */

#if (XCP_MAX_DAQ != 0U)

/* Queue for holding the DAQ Priority */
extern VAR(Xcp_DaqPrioQueueType, XCP_VAR_CLEARED) Xcp_DaqPrioQueue;

/* Mirror for holding the ODT data */
extern VAR(Xcp_AllDtoDataMirrorType, XCP_VAR_CLEARED) Xcp_DTOEntryMirror;

/* Buffer for holding Time stamp information and information for stimulation  */
extern VAR(Xcp_DaqRunTimeInfoType, XCP_VAR_CLEARED) Xcp_DaqRunTimeInfo[XCP_MAX_DAQ];
#endif

/** \brief All current status information of the slave device. */
extern VAR(Xcp_StatusType,  XCP_VAR_CLEARED) Xcp_Status;

#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
/** \brief Definition of Event_CTO queue.
 *
 * The Event_CTO queue is implemented as byte array. */
extern VAR(Xcp_EventCTOQueueType, XCP_VAR_CLEARED) Xcp_EventCTOQueue;
#endif

#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>

#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
/** \brief Variable used for holding request data for MemoryProxy
 ** */
extern VAR(Xcp_RequestToMemProxyType, XCP_VAR) Xcp_RequestBuffer[XCP_MAX_SIZE_OF_REQUEST_BUFFER];
/** \brief Variable used for holding the status of request for every event
 ** */
extern VAR(Xcp_RequestForEventType, XCP_VAR) Xcp_RequestForEvent[XCP_MAX_EVENT_CHANNEL];
/** \brief Variable used for holding information about the Daq Prio information of the
 ** requested event
 ** */
extern VAR(Xcp_DaqPrioQueueOfRequestType, XCP_VAR) Xcp_DaqPrioQueueOfRequest;
#endif

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
/** \brief Variable holding data used for processing of the RequestBuffer
 ** */
extern VAR(Xcp_RequestToMemProxyStructType, XCP_VAR) Xcp_RequestToMemProxy;
#endif
#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

#define XCP_START_SEC_VAR_16
#include <Xcp_MemMap.h>

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
/** \brief The Id of the event that is being processed and for which the memory
 ** proxy request is ongoing
 ** */
extern VAR(uint16, XCP_VAR) Xcp_ProcessedEventId;
#endif

#define XCP_STOP_SEC_VAR_16
#include <Xcp_MemMap.h>

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)

#define XCP_START_SEC_CONST_8
#include <Xcp_MemMap.h>

extern CONST(uint8, XCP_CONST) Xcp_ConnectionToPartitionMap[XCP_NO_OF_CONNECTIONS];

#define XCP_STOP_SEC_CONST_8
#include <Xcp_MemMap.h>

#define XCP_START_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

extern CONST(XcpSchMExclusiveAreaStruct, XCP_CONST) Xcp_SchMExclusiveAreas[XCP_NUM_OF_OS_CORES];

#define XCP_STOP_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

#endif

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)

#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

extern VAR(Atomic_t, XCP_VAR) Xcp_RAMCalloutProxyState;

#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

#endif

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

#endif /* ifndef XCP_INT_H */
