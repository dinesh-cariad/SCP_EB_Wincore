/**
 * \file
 *
 * \brief AUTOSAR Dlt
 *
 * This file contains the implementation of the AUTOSAR
 * module Dlt.
 *
 * \version 1.8.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* This file contains definitions for interface functions provided by Dlt to
 * PduR for reception and process the unpacked receive data. */


/*==================[inclusions]============================================*/

#include <Std_Types.h>                            /* AUTOSAR standard types */
#include <TSAutosar.h>                            /* EB specific standard types */
#include <TSMem.h>                                /* EB specific memory functions */

#include <Dlt.h>                                  /* Module public and internal declarations */
#include <Dlt_Int.h>
#include <Dlt_Trace.h>

#include <PduR.h>
#include <PduR_Dlt.h>
#include <Dlt_UserCallouts.h>
#include <ComStack_Cfg.h>

/*==================[macros]================================================*/

/*   --- BEGIN --- Update these values whenever a new control message is implemented --- BEGIN --- */

/** \brief Maximum size a control message response can have (in bytes)
 * This is the maximum of all implemented so far control messages and does not
 * considers the ServiceId and Status response
 */

#if(defined DLT_MAX_REQUIRED_PAYLOAD_LENGTH)
#error DLT_MAX_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Maximum required payload length out of all implemented Control Messages    */
#define DLT_MAX_REQUIRED_PAYLOAD_LENGTH          (DLT_CM_0x01_REQUIRED_PAYLOAD_LENGTH)

#if(defined DLT_NUMBER_OF_CONTROL_HANDLERS)
#error DLT_NUMBER_OF_CONTROL_HANDLERS already defined
#endif
/** \brief  Number of control messages + 1 (reserved) */
#if(DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
#define DLT_NUMBER_OF_CONTROL_HANDLERS            (0x25U + 1U)
#else
#define DLT_NUMBER_OF_CONTROL_HANDLERS            (0x22U + 1U)
#endif
/*   --- END --- Update these values whenever a new control message is implemented --- END --- */

#if(defined DLT_HTYP_IDX)
#error DLT_HTYP_IDX already defined
#endif
/** \brief HTYP field index in the standard header. */
#define DLT_HTYP_IDX                              0x0U

#if(defined DLT_ECUID_IDX)
#error DLT_ECUID_IDX already defined
#endif
/** \brief ECU field index in the standard header. */
#define DLT_ECUID_IDX                             0x4U

#if(defined DLT_RESERVED_SERVICE_ID)
#error DLT_RESERVED_SERVICE_ID already defined
#endif
/** \brief ServiceId 0 is reserved. */
#define DLT_RESERVED_SERVICE_ID                   0x0U

#if(defined DLT_RESPONSE_SERVICE_ID_IDX)
#error DLT_RESPONSE_SERVICE_ID_IDX already defined
#endif
/** \brief Index of the ServiceId in the response message payload */
#define DLT_RESPONSE_SERVICE_ID_IDX               0x0U

#if(defined DLT_RESPONSE_STATUS_IDX)
#error DLT_RESPONSE_STATUS_IDX already defined
#endif
/** \brief Index of the status in the response message payload */
#define DLT_RESPONSE_STATUS_IDX                   0x4U

#if(defined DLT_REQUEST_SERVICE_ID_IDX)
#error DLT_REQUEST_SERVICE_ID_IDX already defined
#endif
/** \brief Index of the ServiceId in the request message */
#define DLT_REQUEST_SERVICE_ID_IDX                0x0U

#if(defined DLT_SERVICE_ID_SIZE)
#error DLT_SERVICE_ID_SIZE already defined
#endif
/** \brief Length of the ServiceId in the request message */
#define DLT_SERVICE_ID_SIZE                       0x4U

#if(defined DLT_TYPE_CONTROL_BITS_IN_MSIN)
#error DLT_TYPE_CONTROL_BITS_IN_MSIN already defined
#endif
/** \brief TYPE_CONTROL bits to be written in MSIN field. */
#define DLT_TYPE_CONTROL_BITS_IN_MSIN             ((DLT_TYPE_CONTROL) << 1U)

#if(defined DLT_CONTROL_REQUEST_BITS_IN_MSIN)
#error DLT_CONTROL_REQUEST_BITS_IN_MSIN already defined
#endif
/** \brief CONTROL_REQUEST bits to be written in MSIN field. */
#define DLT_CONTROL_REQUEST_BITS_IN_MSIN          ((DLT_CONTROL_REQUEST) << 4U)

#if(defined DLT_MIN_RESPONSE_SIZE)
#error DLT_MIN_RESPONSE_SIZE already defined
#endif
/** \brief Minimum payload response size.
 *
 * 4 bytes ServiceId + 1 Byte Status
 */
#define DLT_MIN_RESPONSE_SIZE                     0x5U

#if(defined DLT_MAX_RESPONSE_PAYLOAD_SIZE)
#error DLT_MAX_RESPONSE_PAYLOAD_SIZE already defined
#endif
/** \brief Maximum size a payload can have (in bytes)
 * This is the maximum of all implemented so far control messages
 *
 * Maximum size must be at least 6 (4 byte ServiceId, 1 byte Status, and 4 byte response)
 *
 */
#define DLT_MAX_RESPONSE_PAYLOAD_SIZE             (DLT_MIN_RESPONSE_SIZE + DLT_MAX_CONTROL_MESSAGE_RESPONSE_SIZE)

#if(defined DLT_RX_MIN_NUMBER_OF_BYTES)
#error DLT_RX_MIN_NUMBER_OF_BYTES already defined
#endif
/** \brief The minimum number of bytes to be received in order to have a valid reception */
#define DLT_RX_MIN_NUMBER_OF_BYTES                (DLT_MIN_HEADER_LENGTH + DLT_EXT_HEADER_LENGTH_STATIC + DLT_SERVICE_ID_SIZE)

#if(defined DLT_RECEPTION_IDLE)
#error DLT_RECEPTION_IDLE already defined
#endif
/** \brief Idle state of the reception state machine. */
#define DLT_RECEPTION_IDLE                        0x0U

#if(defined DLT_RECEPTION_STARTED)
#error DLT_RECEPTION_STARTED already defined
#endif
/** \brief Started state of the reception state machine. */
#define DLT_RECEPTION_STARTED                     0x1U

#if(defined DLT_RECEPTION_CANCELLED)
#error DLT_RECEPTION_CANCELLED already defined
#endif
/** \brief Canceled state of the reception state machine. */
#define DLT_RECEPTION_CANCELLED                   0x2U

#if(defined DLT_RECEPTION_COMPLETED)
#error DLT_RECEPTION_COMPLETED already defined
#endif
/** \brief Completed state of the reception state machine. */
#define DLT_RECEPTION_COMPLETED                   0x3U

#if(defined DLT_UNSUPPORTED_MSG_LENGTH)
#error DLT_UNSUPPORTED_MSG_LENGTH already defined
#endif
/** \brief Placeholder for an unsupported control message length. */
#define DLT_UNSUPPORTED_MSG_LENGTH                0xFFU

#if(defined DLT_UINT32_FROM_BYTE_ARRAY_MSB)
#error DLT_UINT32_FROM_BYTE_ARRAY_MSB already defined
#endif
/** \brief Create a uint32 value from the first four elements of uint8 array. MSB first (big endian) */
#define DLT_UINT32_FROM_BYTE_ARRAY_MSB(ptr)        ( (uint32) \
                                                    ( ((uint32)((ptr)[0]) << 24U) \
                                                    | ((uint32)((ptr)[1]) << 16U) \
                                                    | ((uint32)((ptr)[2]) << 8U) \
                                                    | ((uint32)((ptr)[3]) ) ) )

#if(defined DLT_UINT32_FROM_BYTE_ARRAY_LSB)
#error DLT_UINT32_FROM_BYTE_ARRAY_LSB already defined
#endif
/** \brief Create a uint32 value from the first four elements of uint8 array. LSB first (little endian) */
#define DLT_UINT32_FROM_BYTE_ARRAY_LSB(ptr)        ( (uint32) \
                                                    ( ((uint32)((ptr)[3]) << 24U) \
                                                    | ((uint32)((ptr)[2]) << 16U) \
                                                    | ((uint32)((ptr)[1]) << 8U) \
                                                    | ((uint32)((ptr)[0]) ) ) )

#if(defined DLT_UINT16_FROM_BYTE_ARRAY_MSB)
#error DLT_UINT16_FROM_BYTE_ARRAY_MSB already defined
#endif
/** \brief Create an uint16 value from the first two elements of an uint8 array. MSB first (big endian) */
#define DLT_UINT16_FROM_BYTE_ARRAY_MSB(ptr) ( (uint16) ( ((uint16)((ptr)[0U]) << 8U) \
                                                       | ((uint16)((ptr)[1U])) ) )

#if(defined DLT_MSB_TO_PLATFORM_ENDIANNESS)
#error DLT_MSB_TO_PLATFORM_ENDIANNESS already defined
#endif
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)

/** \brief Copy the MSB-first content of Array_2 to Array_1, LSB-first */
#define DLT_MSB_TO_PLATFORM_ENDIANNESS(Array_1, Array_2, Offset) \
  Array_1[0U] = Array_2[Offset + 3U]; \
  Array_1[1U] = Array_2[Offset + 2U]; \
  Array_1[2U] = Array_2[Offset + 1U]; \
  Array_1[3U] = Array_2[Offset + 0U];

#else /* #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST) */

/** \brief Copy the MSB-first content of Array_2 to Array_1, MSB-first */
#define DLT_MSB_TO_PLATFORM_ENDIANNESS(Array_1, Array_2, Offset) \
  Array_1[0U] = Array_2[Offset + 0U]; \
  Array_1[1U] = Array_2[Offset + 1U]; \
  Array_1[2U] = Array_2[Offset + 2U]; \
  Array_1[3U] = Array_2[Offset + 3U];

#endif /* #if (CPU_BYTE_ORDER == LOW_BYTE_FIRST) */

#if(defined DLT_CM_0x01_APP_ID_PAYLOAD_IDX)
#error DLT_CM_0x01_APP_ID_PAYLOAD_IDX already defined
#endif
/** \brief Index of the applicationId parameter in the reception payload for control message with ServiceId 0x01 */
#define DLT_CM_0x01_APP_ID_PAYLOAD_IDX              (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x01_CONTEXT_ID_PAYLOAD_IDX)
#error DLT_CM_0x01_CONTEXT_ID_PAYLOAD_IDX already defined
#endif
/** \brief Index of the contextId parameter in the reception payload for control message with ServiceId 0x01 */
#define DLT_CM_0x01_CONTEXT_ID_PAYLOAD_IDX          (DLT_SERVICE_ID_SIZE + 0x04U)

#if(defined DLT_CM_0x01_LOG_LEVEL_PAYLOAD_IDX)
#error DLT_CM_0x01_LOG_LEVEL_PAYLOAD_IDX already defined
#endif
/** \brief Index of the newLogLevel parameter in the reception payload for control message with ServiceId 0x01 */
#define DLT_CM_0x01_LOG_LEVEL_PAYLOAD_IDX           (DLT_SERVICE_ID_SIZE + 0x08U)

#if(defined DLT_CM_0x02_APP_ID_PAYLOAD_IDX)
#error DLT_CM_0x02_APP_ID_PAYLOAD_IDX already defined
#endif
/** \brief Index of the applicationId parameter in the reception payload for control message with ServiceId 0x02 */
#define DLT_CM_0x02_APP_ID_PAYLOAD_IDX              (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x02_CONTEXT_ID_PAYLOAD_IDX)
#error DLT_CM_0x02_CONTEXT_ID_PAYLOAD_IDX already defined
#endif
/** \brief Index of the contextId parameter in the reception payload for control message with ServiceId 0x02 */
#define DLT_CM_0x02_CONTEXT_ID_PAYLOAD_IDX          (DLT_SERVICE_ID_SIZE + 0x04U)

#if(defined DLT_CM_0x02_TRACE_STATUS_PAYLOAD_IDX)
#error DLT_CM_0x02_TRACE_STATUS_PAYLOAD_IDX already defined
#endif
/** \brief Index of the newTraceStatus parameter in the reception payload for control message with ServiceId 0x02 */
#define DLT_CM_0x02_TRACE_STATUS_PAYLOAD_IDX        (DLT_SERVICE_ID_SIZE + 0x08U)

#if(defined DLT_CM_0x03_OPTIONS_PAYLOAD_IDX)
#error DLT_CM_0x03_OPTIONS_PAYLOAD_IDX already defined
#endif
/** \brief Index of the options parameter in the reception payload for control message with ServiceId 0x03 */
#define DLT_CM_0x03_OPTIONS_PAYLOAD_IDX             (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x03_APP_ID_PAYLOAD_IDX)
#error DLT_CM_0x03_APP_ID_PAYLOAD_IDX already defined
#endif
/** \brief Index of the applicationId parameter in the reception payload for control message with ServiceId 0x03 */
#define DLT_CM_0x03_APP_ID_PAYLOAD_IDX              (DLT_SERVICE_ID_SIZE + 0x01U)

#if(defined DLT_CM_0x03_CTX_ID_PAYLOAD_IDX)
#error DLT_CM_0x03_CTX_ID_PAYLOAD_IDX already defined
#endif
/** \brief Index of the contextId parameter in the reception payload for control message with ServiceId 0x03 */
#define DLT_CM_0x03_CTX_ID_PAYLOAD_IDX              (DLT_SERVICE_ID_SIZE + 0x05U)

#if(defined DLT_CM_0x08_MAX_BANDWIDTH_PAYLOAD_IDX)
#error DLT_CM_0x08_MAX_BANDWIDTH_PAYLOAD_IDX already defined
#endif
/** \brief Index of the max_bandwidth parameter in the reception payload for control message with ServiceId 0x08 */
#define DLT_CM_0x08_MAX_BANDWIDTH_PAYLOAD_IDX       (DLT_SERVICE_ID_SIZE + 0x04U)

#if(defined DLT_CM_0x09_NEW_STATUS_PAYLOAD_IDX)
#error DLT_CM_0x09_NEW_STATUS_PAYLOAD_IDX already defined
#endif
/** \brief Index of the new_status parameter in the reception payload for control message with ServiceId 0x09 */
#define DLT_CM_0x09_NEW_STATUS_PAYLOAD_IDX          (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x10_NEW_STATUS_PAYLOAD_IDX)
#error DLT_CM_0x10_NEW_STATUS_PAYLOAD_IDX already defined
#endif
/** \brief Index of the new_status parameter in the reception payload for control message with ServiceId 0x10 */
#define DLT_CM_0x10_NEW_STATUS_PAYLOAD_IDX          (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x0A_NEW_STATUS_PAYLOAD_IDX)
#error DLT_CM_0x0A_NEW_STATUS_PAYLOAD_IDX already defined
#endif
/** \brief Index of the newstatus parameter in the reception payload for control message with ServiceId 0x0A */
#define DLT_CM_0x0A_NEW_STATUS_PAYLOAD_IDX          (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x11_LOG_LEVEL_PAYLOAD_IDX)
#error DLT_CM_0x11_LOG_LEVEL_PAYLOAD_IDX already defined
#endif
/** \brief Index of the newLogLevel parameter in the reception payload for control message with ServiceId 0x11 */
#define DLT_CM_0x11_LOG_LEVEL_PAYLOAD_IDX           (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x12_TRACE_STATUS_PAYLOAD_IDX)
#error DLT_CM_0x12_TRACE_STATUS_PAYLOAD_IDX already defined
#endif
/** \brief Index of the newTraceStatus parameter in the reception payload for control message with ServiceId 0x12 */
#define DLT_CM_0x12_TRACE_STATUS_PAYLOAD_IDX        (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x0D_NEW_STATUS_PAYLOAD_IDX)
#error DLT_CM_0x0D_NEW_STATUS_PAYLOAD_IDX already defined
#endif
/** \brief Index of the new_status parameter in the reception payload for control message with ServiceId 0x0D */
#define DLT_CM_0x0D_NEW_STATUS_PAYLOAD_IDX          (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x0E_NEW_STATUS_PAYLOAD_IDX)
#error DLT_CM_0x0E_NEW_STATUS_PAYLOAD_IDX already defined
#endif
/** \brief Index of the new_status parameter in the reception payload for control message with ServiceId 0x0E */
#define DLT_CM_0x0E_NEW_STATUS_PAYLOAD_IDX          (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x0F_NEW_STATUS_PAYLOAD_IDX)
#error DLT_CM_0x0F_NEW_STATUS_PAYLOAD_IDX already defined
#endif
/** \brief Index of the new_status parameter in the reception payload for control message with ServiceId 0x0F */
#define DLT_CM_0x0F_NEW_STATUS_PAYLOAD_IDX          (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x1F_APP_ID_PAYLOAD_IDX)
#error DLT_CM_0x1F_APP_ID_PAYLOAD_IDX already defined
#endif
/** \brief Index of the applicationId parameter in the reception payload for control message with ServiceId 0x1F */
#define DLT_CM_0x1F_APP_ID_PAYLOAD_IDX              (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x1F_CONTEXT_ID_PAYLOAD_IDX)
#error DLT_CM_0x1F_CONTEXT_ID_PAYLOAD_IDX already defined
#endif
/** \brief Index of the contextId parameter in the reception payload for control message with ServiceId 0x1F */
#define DLT_CM_0x1F_CONTEXT_ID_PAYLOAD_IDX          (DLT_SERVICE_ID_SIZE + 0x04U)

#if(defined DLT_CM_0x20_APP_ID_PAYLOAD_IDX)
#error DLT_CM_0x20_APP_ID_PAYLOAD_IDX already defined
#endif
/** \brief Index of the Application ID parameter in the reception payload for control message with ServiceId 0x20 */
#define DLT_CM_0x20_APP_ID_PAYLOAD_IDX              (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x20_CONTEXT_ID_PAYLOAD_IDX)
#error DLT_CM_0x20_CONTEXT_ID_PAYLOAD_IDX already defined
#endif
/** \brief Index of the Context_ID parameter in the reception payload for control message with ServiceId 0x20 */
#define DLT_CM_0x20_CONTEXT_ID_PAYLOAD_IDX          (DLT_SERVICE_ID_SIZE + 0x04U)

#if(defined DLT_CM_0x20_LOG_CHN_PAYLOAD_IDX)
#error DLT_CM_0x20_LOG_CHN_PAYLOAD_IDX already defined
#endif
/** \brief Index of the Log Channel parameter in the reception payload for control message with ServiceId 0x20 */
#define DLT_CM_0x20_LOG_CHN_PAYLOAD_IDX             (DLT_SERVICE_ID_SIZE + 0x08U)

#if(defined DLT_CM_0x20_ADDREMOVEOP_PAYLOAD_IDX)
#error DLT_CM_0x20_ADDREMOVEOP_PAYLOAD_IDX already defined
#endif
/** \brief Index of the Add/Remove operation parameter in the reception payload for control message with ServiceId 0x20 */
#define DLT_CM_0x20_ADDREMOVEOP_PAYLOAD_IDX         (DLT_SERVICE_ID_SIZE + 0x0CU)

#if(defined DLT_CM_0x21_LOG_CHN_PAYLOAD_IDX)
#error DLT_CM_0x21_LOG_CHN_PAYLOAD_IDX already defined
#endif
/** \brief Index of the LogChannelName parameter in the reception payload for control message with ServiceId 0x21 */
#define DLT_CM_0x21_LOG_CHN_PAYLOAD_IDX             (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x21_LOG_NEW_THRESHOLD_PAYLOAD_IDX)
#error DLT_CM_0x21_LOG_NEW_THRESHOLD_PAYLOAD_IDX already defined
#endif
/** \brief Index of the NewThreshold parameter in the reception payload for control message with ServiceId 0x21 */
#define DLT_CM_0x21_LOG_NEW_THRESHOLD_PAYLOAD_IDX   (DLT_SERVICE_ID_SIZE + 0x04U)

#if(defined DLT_CM_0x21_LOG_NEW_TRACESTS_IDX)
#error DLT_CM_0x21_LOG_NEW_TRACESTS_IDX already defined
#endif
/** \brief Index of the New Trace Status parameter in the reception payload for control message with ServiceId 0x21 */
#define DLT_CM_0x21_LOG_NEW_TRACESTS_IDX            (DLT_SERVICE_ID_SIZE + 0x05U)

#if(defined DLT_CM_0x22_LOG_CHN_PAYLOAD_IDX)
#error DLT_CM_0x22_LOG_CHN_PAYLOAD_IDX already defined
#endif
/** \brief Index of the LogChannelName parameter in the reception payload or control message with ServiceId 0x22 */
#define DLT_CM_0x22_LOG_CHN_PAYLOAD_IDX             (DLT_SERVICE_ID_SIZE + 0x00U)

#if(defined DLT_CM_0x01_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x01_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x01  */
#define DLT_CM_0x01_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 13U)

#if(defined DLT_CM_0x02_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x02_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x02  */
#define DLT_CM_0x02_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 13U)

#if(defined DLT_CM_0x03_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x03_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x03 */
#define DLT_CM_0x03_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 13U)

#if(defined DLT_CM_0x04_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x04_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x04  */
#define DLT_CM_0x04_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE)

#if(defined DLT_CM_0x05_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x05_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x05  */
#define DLT_CM_0x05_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE)

#if(defined DLT_CM_0x06_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x06_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x06  */
#define DLT_CM_0x06_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE)

#if(defined DLT_CM_0x08_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x08_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x08  */
#define DLT_CM_0x08_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 8U)

#if(defined DLT_CM_0x09_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x09_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x09  */
#define DLT_CM_0x09_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 1U)

#if(defined DLT_CM_0x0A_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x0A_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x0A  */
#define DLT_CM_0x0A_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 1U)

#if(defined DLT_CM_0x0D_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x0D_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x0D  */
#define DLT_CM_0x0D_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 1U)

#if(defined DLT_CM_0x0E_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x0E_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x0E  */
#define DLT_CM_0x0E_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 1U)

#if(defined DLT_CM_0x0F_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x0F_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x0F  */
#define DLT_CM_0x0F_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 1U)

#if(defined DLT_CM_0x10_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x10_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x10  */
#define DLT_CM_0x10_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 1U)

#if(defined DLT_CM_0x11_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x11_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x11  */
#define DLT_CM_0x11_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 5U)

#if(defined DLT_CM_0x12_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x12_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x12  */
#define DLT_CM_0x12_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 5U)

#if(defined DLT_CM_0x13_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x13_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x13  */
#define DLT_CM_0x13_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE)

#if(defined DLT_CM_0x15_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x15_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x15  */
#define DLT_CM_0x15_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE)

#if(defined DLT_CM_0x17_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x17_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x17  */
#define DLT_CM_0x17_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE)

#if(defined DLT_CM_0x18_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x18_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x18  */
#define DLT_CM_0x18_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 4U)

#if(defined DLT_CM_0x19_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x19_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x19  */
#define DLT_CM_0x19_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE)

#if(defined DLT_CM_0x1A_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x1A_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x1A  */
#define DLT_CM_0x1A_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE)

#if(defined DLT_CM_0x1B_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x1B_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x1B  */
#define DLT_CM_0x1B_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE)

#if(defined DLT_CM_0x1C_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x1C_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x1C  */
#define DLT_CM_0x1C_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE)

#if(defined DLT_CM_0x1D_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x1D_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x1D  */
#define DLT_CM_0x1D_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE)

#if(defined DLT_CM_0x1E_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x1E_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x1E  */
#define DLT_CM_0x1E_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE)

#if(defined DLT_CM_0x1F_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x1F_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x1F  */
#define DLT_CM_0x1F_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 8U)

#if(defined DLT_CM_0x20_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x20_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x20  */
#define DLT_CM_0x20_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 5U)

#if(defined DLT_CM_0x21_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x21_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x21  */
#define DLT_CM_0x21_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 6U)

#if(defined DLT_CM_0x22_REQUIRED_PAYLOAD_LENGTH)
#error DLT_CM_0x22_REQUIRED_PAYLOAD_LENGTH already defined
#endif
/** \brief Required length for the request payload of service 0x22  */
#define DLT_CM_0x22_REQUIRED_PAYLOAD_LENGTH             (DLT_SERVICE_ID_SIZE + 4U)

#if(defined DLT_HEADER_EXTENDED_HEADER_BIT_MASK)
#error DLT_HEADER_EXTENDED_HEADER_BIT_MASK already defined
#endif
/** \brief Bit mask for EXTENDED_HEADER bit in the header  */
#define DLT_HEADER_EXTENDED_HEADER_BIT_MASK    0x01U

#if(defined DLT_HEADER_ENDIANES_BIT_MASK)
#error DLT_HEADER_ENDIANES_BIT_MASK already defined
#endif
/** \brief Bit mask for ENDIANES bit in the header  */
#define DLT_HEADER_ENDIANES_BIT_MASK           0x02U

#if(defined DLT_HEADER_ECU_ID_BIT_MASK)
#error DLT_HEADER_ECU_ID_BIT_MASK already defined
#endif
/** \brief Bit mask for ECU_ID bit in the header  */
#define DLT_HEADER_ECU_ID_BIT_MASK             0x04U

#if(defined DLT_HEADER_SESSION_ID_BIT_MASK)
#error DLT_HEADER_SESSION_ID_BIT_MASK already defined
#endif
/** \brief Bit mask for SESSION_ID bit in the header  */
#define DLT_HEADER_SESSION_ID_BIT_MASK         0x08U

#if(defined DLT_HEADER_TIMESTAMP_BIT_MASK)
#error DLT_HEADER_TIMESTAMP_BIT_MASK already defined
#endif
/** \brief Bit mask for TIMESTAMP bit in the header  */
#define DLT_HEADER_TIMESTAMP_BIT_MASK          0x10U

#if (DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON)

#if(defined DLT_RESPONSE_RESERVED_BYTES)
#error DLT_RESPONSE_RESERVED_BYTES already defined
#endif
/** \brief The space (in bytes) that is taken by the reserved portion of the response */
#define DLT_RESPONSE_RESERVED_BYTES 0x04U

#if(defined DLT_APP_ID_COUNT_SIZE)
#error DLT_APP_ID_COUNT_SIZE already defined
#endif
/** \brief The space (in bytes) that is taken by the appIdCount
 * in Dlt_GetLogInfoWrapper()'s response */
#define DLT_APP_ID_COUNT_SIZE 0x02U

#if(defined DLT_RESPONSE_APP_ID_SIZE_WITH_DESC)
#error DLT_RESPONSE_APP_ID_SIZE_WITH_DESC already defined
#endif
/** \brief The space (in bytes) that is taken by the information about each AppId */
#define DLT_RESPONSE_APP_ID_SIZE_WITH_DESC 0x07U

#if(defined DLT_RESPONSE_APP_ID_SIZE_NO_DESC)
#error DLT_RESPONSE_APP_ID_SIZE_NO_DESC already defined
#endif
/** \brief The space (in bytes) that is taken by the information about each AppId */
#define DLT_RESPONSE_APP_ID_SIZE_NO_DESC   0x06U

#if(defined DLT_RESPONSE_CTX_ID_SIZE_WITH_DESC)
#error DLT_RESPONSE_CTX_ID_SIZE_WITH_DESC already defined
#endif
/** \brief The space (in bytes) that is taken by the information about each ContextId */
#define DLT_RESPONSE_CTX_ID_SIZE_WITH_DESC 0x07U

#if(defined DLT_RESPONSE_CTX_ID_SIZE_NO_DESC)
#error DLT_RESPONSE_CTX_ID_SIZE_NO_DESC already defined
#endif
/** \brief The space (in bytes) that is taken by the information about each ContextId */
#define DLT_RESPONSE_CTX_ID_SIZE_NO_DESC   0x06U

#if(defined DLT_RESPONSE_UINT8_OFFSET)
#error DLT_RESPONSE_UINT8_OFFSET already defined
#endif
/** \brief Offset in bytes for uint8 values written in the uint8 response buffer */
#define DLT_RESPONSE_UINT8_OFFSET 0x01U

#if(defined DLT_RESPONSE_UINT16_OFFSET)
#error DLT_RESPONSE_UINT16_OFFSET already defined
#endif
/** \brief Offset in bytes for uint16 values written in the uint8 response buffer */
#define DLT_RESPONSE_UINT16_OFFSET 0x02U

#if(defined DLT_RESPONSE_UINT32_OFFSET)
#error DLT_RESPONSE_UINT32_OFFSET already defined
#endif
/** \brief Offset in bytes for uint32 values written in the uint8 response buffer */
#define DLT_RESPONSE_UINT32_OFFSET 0x04U

#endif /* DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON */

/** \brief The Dlt_RxBufferType contains 3 members of type PduLengthType,
 *         which can be either an uint16 or an uint32.
 *         This macro represents the number of these members. */
#define DLT_RX_BUFFER_TYPE_STRUCT 3U

/*==================[type definitions]======================================*/

#if (DLT_RECEPTION_CONTROL_MESSAGES_ENABLED == STD_ON)
/** \brief Command processor function pointer type */
typedef P2FUNC(Dlt_ReturnType, DLT_CONST, MessageHandlerPtr_Type)
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/** \brief Structure containing each control message' execution handler and
    additional information regarding the execution requirements */
typedef struct
{
  MessageHandlerPtr_Type MessageHandlerPtr; /* Pointer to control message execution handler */
  uint8 BaseLength; /* Required Payload Length to be received */
} Dlt_ControlMessages_Type;

/** \brief  Structure containing the reception buffer type */
typedef struct
{
  PduLengthType NumberOfBytesFilled;
  PduLengthType ExpectedNumberOfBytesToBeReceived;
  PduLengthType RemainingBytes;
  uint8 Buffer[DLT_RX_MESSAGE_BUFFER_SIZE];
} Dlt_RxBufferType;

#endif /*DLT_RECEPTION_CONTROL_MESSAGES_ENABLED == STD_ON */

/*==================[external function declarations]========================*/

#define DLT_START_SEC_CODE
#include <Dlt_MemMap.h>
/*-------------------Dlt_WriteDwordToByteArray-------------------------*/
extern FUNC(void, DLT_CODE) Dlt_WriteDwordToByteArray
(
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) DesPtr,
  uint32                                 Dword
)
{
  DBG_DLT_WRITEDWORDTOBYTEARRAY_ENTRY(Dword);

  DesPtr[DLT_UINT32_SB0_IDX] = (uint8)Dword;
  DesPtr[DLT_UINT32_SB1_IDX] = (uint8)(Dword >> 8U);
  DesPtr[DLT_UINT32_SB2_IDX] = (uint8)(Dword >> 16U);
  DesPtr[DLT_UINT32_SB3_IDX] = (uint8)(Dword >> 24U);

  DBG_DLT_WRITEDWORDTOBYTEARRAY_EXIT(DesPtr);
}

#define DLT_STOP_SEC_CODE
#include <Dlt_MemMap.h>

/*==================[internal function declarations]========================*/

#if (DLT_RECEPTION_CONTROL_MESSAGES_ENABLED == STD_ON)

#define DLT_START_SEC_CODE
#include <Dlt_MemMap.h>


/** \brief Get the received message length
 **
 ** \param[in] OffsetInBuffer The offset in the reception buffer where to read the
 **                           length fields
 **
 ** \return The received message length.
 **/
STATIC FUNC(Dlt_MsgLengthType, DLT_CODE) Dlt_GetMessageLength(Dlt_MsgLengthType OffsetInBuffer);

/** \brief Validate that the request is well formed
 **
 ** \param[in] OffsetInBuffer The offset in the reception buffer where to validate
 **                           the current request
 ** \param[out] ServiceId The received service Id
 **
 ** \return The success state of the validation
 ** \retval DLT_CTRL_OK Validation is successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Validation is not successful, status is DLT_CTRL_NOT_SUPPORTED.
 ** \retval DLT_CTRL_ERROR  Validation is not successful, status is DLT_CTRL_ERROR.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_ValidateRequest
(
  Dlt_MsgLengthType OffsetInBuffer,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_DATA) ServiceId
);

/** \brief Computes the length of the header according to
 **  the received buffer
 **
 ** This function calculates the length of the DLT message header
 ** according to received bits (WSID, WSTMS, WEID).
 **
 ** \return The length of the header for the current message
 **/
STATIC FUNC(uint8, DLT_CODE) Dlt_CalculateReceivedHeaderLength(void);

/** \brief Execute the Control Message by calling the wrapper to the C-API handler
 **
 ** \param[in] ServiceId Service Identifier
 ** \param[in] OffsetInBuffer The offset in the reception buffer where to validate
 **                           the current request
 ** \param[out] PayloadToBeSent Response payload that the control Message
                                writes plus protocol bytes: ServiceId and Status
 ** \param[out] PayloadToBeSentLength Length of the response payload
 **/
STATIC FUNC(void, DLT_CODE) Dlt_ExecuteRequest
(
  uint32 ServiceId,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) PayloadToBeSent,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) PayloadToBeSentLength,
  Dlt_MsgLengthType OffsetInBuffer
);

/** \brief Triggers the response to be sent back to the requester
 **
 ** \param[in] PayloadToBeSent Response payload for the requester
 ** \param[in] PayloadToBeSentLength Length of the response payload
 **/
STATIC FUNC(void, DLT_CODE) Dlt_RespondToRequest
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) PayloadToBeSent,
  Dlt_MsgLengthType PayloadToBeSentLength
);

#if (DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON)

/* !LINKSTO Dlt.ASR431.Command.GetLogInfo, 1 */
/** \brief Wrapper function for Dlt_GetLogInfo() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetLogInfoWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
/* !LINKSTO Dlt.ASR431.Command.SetLogChannelAssignment, 1 */
/** \brief Wrapper function for Dlt_SetLogChannelAssignment() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetLogChannelAssignmentWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/* !LINKSTO Dlt.ASR431.Command.GetLogChannelNames, 1 */
/** \brief Wrapper function for Dlt_GetLogChannelNames() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetLogChannelNamesWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/* !LINKSTO Dlt.ASR431.Command.SetLogChannelThreshold, 1 */
/** \brief Wrapper function for Dlt_SetLogChannelThreshold() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetLogChannelThresholdWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/* !LINKSTO Dlt.ASR431.Command.GetLogChannelThreshold, 1 */
/** \brief Wrapper function for Dlt_GetLogChannelThreshold() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetLogChannelThresholdWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

#endif /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */

/* !LINKSTO Dlt.ASR431.Command.SetLogLevel, 1 */
/** \brief Wrapper function for Dlt_SetLogLevel() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetLogLevelWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/* !LINKSTO Dlt.ASR431.Command.SetTraceStatus, 1 */
/** \brief Wrapper function for Dlt_SetTraceStatus() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetTraceStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/* !LINKSTO Dlt.ASR431.Command.GetTraceStatus, 1 */
/** \brief Wrapper function for Dlt_GetTraceStatus() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetTraceStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

#endif /* DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON */

/* !LINKSTO Dlt.ASR431.Command.SetDefaultLogLevel, 1 */
/** \brief Wrapper function for Dlt_SetDefaultLogLevel() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetDefaultLogLevelWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/* !LINKSTO Dlt.ASR431.Command.GetDefaultLogLevel, 1 */
/** \brief Wrapper function for Dlt_GetDefaultLogLevel() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetDefaultLogLevelWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

#if (DLT_TRAFFIC_SHAPER_ENABLED == STD_ON)
/** \brief Wrapper function for Dlt_SetComInterfaceMaxBandwidth() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetComInterfaceMaxBandwidthWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/** \brief Wrapper function for Dlt_GetComInterfaceMaxBandwidth() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetComInterfaceMaxBandwidthWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

#endif /* DLT_TRAFFIC_SHAPER_ENABLED == STD_ON */

/* !LINKSTO Dlt.ASR431.Command.SetDefaultTraceStatus, 1 */
/** \brief Wrapper function for Dlt_SetDefaultTraceStatus() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetDefaultTraceStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/* !LINKSTO Dlt.ASR431.Command.GetDefaultTraceStatus, 1 */
/** \brief Wrapper function for Dlt_GetDefaultTraceStatus() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetDefaultTraceStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/* !LINKSTO Dlt.ASR431.Command.SetMessageFiltering, 1 */
/** \brief Wrapper function for Dlt_SetMessageFiltering() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetMessageFilteringWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/** \brief Wrapper function for Dlt_GetMessageFilteringStatus() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetMessageFilteringStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
/* !LINKSTO Dlt.ASR431.Command.ResetToFactoryDefault, 1 */
/** \brief Wrapper function for Dlt_ResetToFactoryDefault() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_ResetToFactoryDefaultWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);
#endif /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */

/** \brief Wrapper function for Dlt_SetUseECUID() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetUseECUIDWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/** \brief Wrapper function for Dlt_GetUseECUIDStatus() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetUseECUIDStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/** \brief Wrapper function for Dlt_SetUseSessionID() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetUseSessionIDWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/** \brief Wrapper function for Dlt_GetUseSessionIDStatus() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetUseSessionIDStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

#if (DLT_IMPLEMENT_TIMESTAMP != STD_OFF)
/** \brief Wrapper function for Dlt_SetUseTimestamp() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetUseTimestampWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/** \brief Wrapper function for Dlt_GetUseTimestampStatus() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetUseTimestampStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);
#endif /* DLT_IMPLEMENT_TIMESTAMP != STD_OFF */

/** \brief Wrapper function for Dlt_SetUseExtendedHeader() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetUseExtendedHeaderWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/** \brief Wrapper function for Dlt_GetUseExtendedHeaderStatus() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetUseExtendedHeaderStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/* !LINKSTO Dlt.ASR431.Command.StoreConfiguration, 1 */
/** \brief Wrapper function for Dlt_StorePersistent/Configuration() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_StoreConfigWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

#if (DLT_IMPLEMENT_VERBOSE == STD_ON)
/** \brief Wrapper function for Dlt_SetVerboseMode() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetVerboseModeWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

/** \brief Wrapper function for Dlt_GetVerboseModeStatus() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload length of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetVerboseModeStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);
#endif /* DLT_IMPLEMENT_VERBOSE */

/** \brief Wrapper function for Dlt_GetSoftwareVersion() API
 **
 ** \param[in] RequestBuffer Request buffer out of which the control message gets the parameters
 ** \param[in] ReceivedHeaderLength Size of the received header
 ** \param[out] Response Response payload, if any, of the control message
 ** \param[out] ResponseLength Response payload lenght of the control message
 ** \return the control message status
 ** \retval DLT_CTRL_OK Control message request was successful.
 ** \retval DLT_CTRL_NOT_SUPPORTED Control message request is not supported.
 ** \retval DLT_CTRL_ERROR  Validation Control message request was not successful.
 **/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetSoftwareVersionWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
);

#define DLT_STOP_SEC_CODE
#include <Dlt_MemMap.h>

#endif /* DLT_RECEPTION_CONTROL_MESSAGES_ENABLED == STD_ON */

/*==================[internal data]=========================================*/

#if (DLT_RECEPTION_CONTROL_MESSAGES_ENABLED == STD_ON)

#define DLT_START_SEC_VAR_UNSPECIFIED
#include <Dlt_MemMap.h>

/** \brief Reception buffer and corresponding admin fields */
STATIC VAR(Dlt_RxBufferType, DLT_VAR) Dlt_RxBuffer;

#define DLT_STOP_SEC_VAR_UNSPECIFIED
#include <Dlt_MemMap.h>

#define DLT_START_SEC_VAR_CLEARED_8
#include <Dlt_MemMap.h>

/** \brief Status for the reception connection */
STATIC VAR(uint8, DLT_VAR) Dlt_RxConnectionState;


STATIC VAR(uint8, DLT_VAR) ResponseBuffer[DLT_MAX_CONTROL_MESSAGE_RESPONSE_SIZE];

#define DLT_STOP_SEC_VAR_CLEARED_8
#include <Dlt_MemMap.h>

#endif /* DLT_RECEPTION_CONTROL_MESSAGES_ENABLED == STD_ON */

/*==================[external data]=========================================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

#if (DLT_RECEPTION_CONTROL_MESSAGES_ENABLED == STD_ON)

#define DLT_START_SEC_CONST_UNSPECIFIED
#include <Dlt_MemMap.h>

/** \brief Control messages handlers */
STATIC CONST(Dlt_ControlMessages_Type, DLT_CONST) Dlt_ControlMessages[DLT_NUMBER_OF_CONTROL_HANDLERS] =
{
  /* 0x0 is reserved */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
#if (DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON)
  /* 0x1 Set_LogLevel */
  {
    Dlt_SetLogLevelWrapper,               /* Control message pointer */
    DLT_CM_0x01_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
  /* 0x2 Dlt_SetTraceStatus*/
  {
    Dlt_SetTraceStatusWrapper,            /* Control message handler pointer */
    DLT_CM_0x02_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
#else
  /* 0x1 Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
  /* 0x2 Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
#endif /* DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON */
  /* 0x3 Dlt_GetLogInfo */
  {
    Dlt_GetLogInfoWrapper,                /* Control message handler pointer */
    DLT_CM_0x03_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
  /* 0x4 Dlt_GetDefaultLogLevel */
  {
    Dlt_GetDefaultLogLevelWrapper,        /* Control message handler pointer */
    DLT_CM_0x04_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
  /* 0x5 Dlt_StorePersistent/Dlt_StoreConfiguration */
  {
    Dlt_StoreConfigWrapper,               /* Control message handler pointer */
    DLT_CM_0x05_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  /* 0x6 Dlt_ResetToFactoryDefault */
  {
    Dlt_ResetToFactoryDefaultWrapper,     /* Control message handler pointer */
    DLT_CM_0x06_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
#else
  /* 0x6 Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
#endif
  /* 0x7 Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
#if (DLT_TRAFFIC_SHAPER_ENABLED == STD_ON)
  /* 0x8 Dlt_SetComInterfaceMaxBandwidth */
  {
    Dlt_SetComInterfaceMaxBandwidthWrapper,   /* Control message handler pointer */
    DLT_CM_0x08_REQUIRED_PAYLOAD_LENGTH,      /* Base Length */
  },
#else
  /* 0x8 Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
#endif /* DLT_TRAFFIC_SHAPER_ENABLED == STD_ON */
#if (DLT_IMPLEMENT_VERBOSE == STD_ON)
  /* 0x9 SetVerboseMode */
  {
    Dlt_SetVerboseModeWrapper,            /* Control message handler pointer */
    DLT_CM_0x09_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
#else
  /* 0x9 Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
#endif /* DLT_IMPLEMENT_VERBOSE == STD_ON */
#if (DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON)
  /* 0xA SetMessageFiltering */
  {
    Dlt_SetMessageFilteringWrapper,       /* Control message handler pointer */
    DLT_CM_0x0A_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
#else
  /* 0xA Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
#endif /* DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON */
  /* 0xB Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
  /* 0xC Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
  /* 0xD SetUseECUID */
  {
    Dlt_SetUseECUIDWrapper,               /* Control message handler pointer */
    DLT_CM_0x0D_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
  /* 0xE Dlt_SetUseSessionID */
  {
    Dlt_SetUseSessionIDWrapper,           /* Control message handler pointer */
    DLT_CM_0x0E_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
#if (DLT_IMPLEMENT_TIMESTAMP != STD_OFF)
  /* 0xF Dlt_SetUseTimestamp */
  {
    Dlt_SetUseTimestampWrapper,           /* Control message handler pointer */
    DLT_CM_0x0F_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
#else /* #if (DLT_IMPLEMENT_TIMESTAMP == STD_OFF) */
  /* 0xF Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
#endif /* DLT_IMPLEMENT_TIMESTAMP != STD_OFF */
#if (DLT_IMPLEMENT_EXTENDED_HEADER == STD_ON)
  /* 0x10 Dlt_SetUseExtendedHeader */
  {
    Dlt_SetUseExtendedHeaderWrapper,      /* Control message handler pointer */
    DLT_CM_0x10_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
#else
  /* 0x10 Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
#endif /* DLT_IMPLEMENT_EXTENDED_HEADER == STD_ON */
  /* 0x11 Dlt_SetDefaultLogLevel */
  {
    Dlt_SetDefaultLogLevelWrapper,         /* Control message handler pointer */
    DLT_CM_0x11_REQUIRED_PAYLOAD_LENGTH,   /* Base Length */
  },
  /* 0x12 Dlt_SetDefaultTraceStatus */
  {
    Dlt_SetDefaultTraceStatusWrapper,     /* Control message handler pointer */
    DLT_CM_0x12_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
  /* 0x13 Get ECU Software Version */
  {
    Dlt_GetSoftwareVersionWrapper,        /* Control message handler pointer */
    DLT_CM_0x13_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
  /* 0x14 Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
  /* 0x15 Dlt_GetDefaultTraceStatus */
  {
    Dlt_GetDefaultTraceStatusWrapper,      /* Control message handler pointer */
    DLT_CM_0x15_REQUIRED_PAYLOAD_LENGTH,   /* Base Length */
  },
  /* 0x16 Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  /* 0x17 GetLogChannelNames */
  {
    Dlt_GetLogChannelNamesWrapper,        /* Control message handler pointer */
    DLT_CM_0x17_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
#else
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
#endif
#if (DLT_TRAFFIC_SHAPER_ENABLED == STD_ON)
  /* 0x18 Dlt_GetComInterfaceMaxBandwidth */
  {
    Dlt_GetComInterfaceMaxBandwidthWrapper,   /* Control message handler pointer */
    DLT_CM_0x18_REQUIRED_PAYLOAD_LENGTH,      /* Base Length */
  },
#else
  /* 0x18 Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
#endif
#if (DLT_IMPLEMENT_VERBOSE == STD_ON)
  /* 0x19 Dlt_GetVerboseModeStatus */
  {
    Dlt_GetVerboseModeStatusWrapper,      /* Control message handler pointer */
    DLT_CM_0x19_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
#else
    /* 0x19 Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
#endif /* DLT_IMPLEMENT_VERBOSE == STD_ON */
  /* 0x1A Dlt_GetMessageFilteringStatus */
  {
    Dlt_GetMessageFilteringStatusWrapper, /* Control message handler pointer */
    DLT_CM_0x1A_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
  /* 0x1B Dlt_GetUseECUID*/
  {
    Dlt_GetUseECUIDStatusWrapper,         /* Control message handler pointer */
    DLT_CM_0x1B_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
  /* 0x1C Dlt_GetUseSessionID */
  {
    Dlt_GetUseSessionIDStatusWrapper,     /* Control message handler pointer */
    DLT_CM_0x1C_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
#if (DLT_IMPLEMENT_TIMESTAMP != STD_OFF)
  /* 0x1D Dlt_GetUseTimestamp */
  {
    Dlt_GetUseTimestampStatusWrapper,     /* Control message handler pointer */
    DLT_CM_0x1D_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
#else /* #if (DLT_IMPLEMENT_TIMESTAMP == STD_OFF) */
  /* 0x1D Not implemented */
  {
    NULL_PTR,                             /* Control message handler pointer */
    DLT_UNSUPPORTED_MSG_LENGTH,           /* Base Length */
  },
#endif /* DLT_IMPLEMENT_TIMESTAMP != STD_OFF */
  /* 0x1E Dlt_GetUseExtendedHeader */
  {
    Dlt_GetUseExtendedHeaderStatusWrapper,/* Control message handler pointer */
    DLT_CM_0x1E_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
  /* 0x1F Dlt_GetTraceStatus */
  {
    Dlt_GetTraceStatusWrapper,            /* Control message handler pointer */
    DLT_CM_0x1F_REQUIRED_PAYLOAD_LENGTH,  /* Base Length */
  },
#if  (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  /* 0x20 Dlt_SetLogChannelAssignment */
  {
    Dlt_SetLogChannelAssignmentWrapper,        /* Control message handler pointer */
    DLT_CM_0x20_REQUIRED_PAYLOAD_LENGTH,       /* Base Length */
  },
  /* 0x21 Dlt_SetLogChannelThreshold */
  {
    Dlt_SetLogChannelThresholdWrapper,          /* Control message handler pointer */
    DLT_CM_0x21_REQUIRED_PAYLOAD_LENGTH,        /* Base Length */
  },
  /* 0x22 Dlt_GetLogChannelThreshold */
  {
    Dlt_GetLogChannelThresholdWrapper,          /* Control message handler pointer */
    DLT_CM_0x22_REQUIRED_PAYLOAD_LENGTH,        /* Base Length */
  }
#endif
};

#define DLT_STOP_SEC_CONST_UNSPECIFIED
#include <Dlt_MemMap.h>

#define DLT_START_SEC_CONST_8
#include <Dlt_MemMap.h>

/** \brief Payload buffer for response */

STATIC VAR(uint8, DLT_APPL_DATA) Dlt_PayloadToRespond[DLT_MAX_RESPONSE_PAYLOAD_SIZE];

#define DLT_STOP_SEC_CONST_8
#include <Dlt_MemMap.h>

#endif /* DLT_RECEPTION_CONTROL_MESSAGES_ENABLED == STD_ON */

/*==================[external function definitions]=========================*/

#define DLT_START_SEC_CODE
#include <Dlt_MemMap.h>

#if (DLT_RECEPTION_CONTROL_MESSAGES_ENABLED == STD_ON)


#if (DLT_ON_ETHERNET == STD_ON)

/*------------------[Dlt_ComStartOfReception]-------------------------------*/
/* !LINKSTO SWS_Dlt_00517,1, Dlt.swdd.Communication.Interface,1 */
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComStartOfReception
(
  PduIdType id,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) bufferSizePtr)
{
  DBG_DLT_COMSTARTOFRECEPTION_ENTRY(id, TpSduLength, bufferSizePtr);

  TS_PARAM_UNUSED(id);
  TS_PARAM_UNUSED(TpSduLength);
  TS_PARAM_UNUSED(bufferSizePtr);

  DLT_UNREACHABLE_CODE_ASSERT(DLT_INSTANCE_ID, DLT_SID_ComStartOfReception);

  DBG_DLT_COMSTARTOFRECEPTION_EXIT(BUFREQ_OK, id, TpSduLength, bufferSizePtr);

  return BUFREQ_OK;
}

/*------------------[Dlt_ComCopyRxData]-------------------------------------*/
/* !LINKSTO SWS_Dlt_00515,1, Dlt.swdd.Communication.Interface,1 */
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComCopyRxData
(
  PduIdType id,
  P2CONST(PduInfoType, AUTOMATIC, DLT_APPL_CONST) info,
  P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) bufferSizePtr
)
{
  BufReq_ReturnType RetValue = BUFREQ_E_NOT_OK;

  DBG_DLT_COMCOPYRXDATA_ENTRY(id, info, bufferSizePtr);

#if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_InitStateMaster == DLT_UNINITIALIZED)
  {
    DLT_DET_REPORT_ERROR(DLT_SID_ComCopyRxData, DLT_E_NOT_INITIALIZED);
  }
  else if ((NULL_PTR == bufferSizePtr) || (NULL_PTR == info))
  {
    DLT_DET_REPORT_ERROR(DLT_SID_ComCopyRxData, DLT_E_PARAM_POINTER);
  }
  else if (id != DLT_RX_PDU_ID)
  {
    /* inform user if input parameter pduId is invalid */
    DLT_DET_REPORT_ERROR(DLT_SID_ComCopyRxData, DLT_E_WRONG_PARAMETERS);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
    if ((info->SduLength == 0U) || (info->SduDataPtr == NULL_PTR))
    {
      /* This means that the lower layer queries for our available buffer */
      /* !LINKSTO Dlt.swdd.DltCore.API.ComCopyRxDataRemaining,1 */
      *bufferSizePtr = DLT_RX_MESSAGE_BUFFER_SIZE;
      Dlt_RxConnectionState = DLT_RECEPTION_STARTED;

      RetValue = BUFREQ_OK;
    }
    else
    {
      /* Enter exclusive area to prevent concurrent accesses to RxBuffer */
      Dlt_BSWMasterContextPtr->EnterExclusiveArea();

      if (Dlt_RxConnectionState == DLT_RECEPTION_STARTED)
      {

        /* no of bytes filled and the requested amount of bytes to be copied should not be
         * greater than the maximum buffer size */
        if ((info->SduLength  + Dlt_RxBuffer.NumberOfBytesFilled) <= DLT_RX_MESSAGE_BUFFER_SIZE)
        {
          RetValue = BUFREQ_OK;

          /* copy data into the buffer */
          TS_MemCpy(&(Dlt_RxBuffer.Buffer[Dlt_RxBuffer.NumberOfBytesFilled]), info->SduDataPtr, info->SduLength);

          /* update number of filled bytes for the next call to continue to copy the amount
           * from the address where the last copy was made */
          Dlt_RxBuffer.NumberOfBytesFilled += info->SduLength;

          /* update the remaining size for the buffer */
          *bufferSizePtr -= info->SduLength;

          /* Since on SoAd, there is no Dlt_ComRxIndication being called to mark the end of reception,
          and no Dlt_ComStartOfReception to mark how much bytes to expect, as the PduHeader is false,
          state goes into completed directly */

          Dlt_RxConnectionState = DLT_RECEPTION_COMPLETED;
        }
        else
        {
          RetValue = BUFREQ_E_OVFL;
        }

      }

      Dlt_BSWMasterContextPtr->ExitExclusiveArea();

    }
    if (RetValue != BUFREQ_OK)
    {
      Dlt_RxConnectionState = DLT_RECEPTION_CANCELLED;
    }

    DBG_DLT_COMCOPYRXDATA_EXIT(BUFREQ_OK, id, info, bufferSizePtr);
  }
  return RetValue;
}

/*------------------[Dlt_ComRxIndication]-----------------------------------*/
/* !LINKSTO SWS_Dlt_00272,1, Dlt.swdd.Communication.Interface,1 */
FUNC(void, DLT_CODE) Dlt_ComRxIndication
(
  PduIdType DltRxPduId,
  NotifResultType Result
)
{
  DBG_DLT_COMRXINDICATION_ENTRY(DltRxPduId);
#if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_InitStateMaster == DLT_UNINITIALIZED)
  {
    DLT_DET_REPORT_ERROR(DLT_SID_ComRxIndication, DLT_E_NOT_INITIALIZED);
  }
  else if (DltRxPduId != DLT_RX_PDU_ID)
  {
    /* inform user if input parameter pduId is invalid */
    DLT_DET_REPORT_ERROR(DLT_SID_ComRxIndication, DLT_E_WRONG_PARAMETERS);
  }
  else
#endif /* DLT_DEV_ERROR_DETECT == STD_ON */
  {
    /* Reset state and clear buffer */
    Dlt_InitReceiveProcessor();
  }

  TS_PARAM_UNUSED(Result);
  DBG_DLT_COMRXINDICATION_EXIT(DltRxPduId);
}

#else /* No Ethernet */

/*------------------[Dlt_ComStartOfReception]-------------------------------*/
/* !LINKSTO SWS_Dlt_00517,1, Dlt.swdd.Communication.Interface,1 */
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComStartOfReception
(
  PduIdType id,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) bufferSizePtr)
{
  BufReq_ReturnType RetValue = BUFREQ_E_NOT_OK;

  DBG_DLT_COMSTARTOFRECEPTION_ENTRY(id, TpSduLength, bufferSizePtr);

#if (DLT_DEV_ERROR_DETECT == STD_OFF)
  TS_PARAM_UNUSED(id);
#else
  if (Dlt_InitStateMaster == DLT_UNINITIALIZED)
  {
    /* !LINKSTO EB_Req_Dlt_ComStartOfReception.DltNotInit.DET,1 */
    DLT_DET_REPORT_ERROR(DLT_SID_ComStartOfReception, DLT_E_NOT_INITIALIZED);
  }
  else if (NULL_PTR == bufferSizePtr)
  {
    /* !LINKSTO Dlt.swdd.ComStartOfReception.NullPtr,1 */
    DLT_DET_REPORT_ERROR(DLT_SID_ComStartOfReception, DLT_E_PARAM_POINTER);
  }
  else if (id != DLT_RX_PDU_ID)
  {
    /* !LINKSTO Dlt.swdd.ComStartOfReception.InvalidRxPdu,1 */
    /* inform user if input parameter pduId is invalid */
    DLT_DET_REPORT_ERROR(DLT_SID_ComStartOfReception, DLT_E_WRONG_PARAMETERS);
  }
  else
#endif /* (DLT_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Idle state of the Dlt shows it is ready to receive requests */
    if (Dlt_RxConnectionState == DLT_RECEPTION_IDLE)
    {
      /* if TPSduLength is different to 0, reject request */
      if (TpSduLength != 0U)
      {
        /* check that  length fits our buffer */
        if (TpSduLength <= DLT_RX_MESSAGE_BUFFER_SIZE)
        {
          /* connection should be started and no other request shall
           * interrupt till the current buffer is processed */
          Dlt_RxConnectionState = DLT_RECEPTION_STARTED;

          /* Enter exclusive area to prevent concurrent accesses to RxBuffer */
          Dlt_BSWMasterContextPtr->EnterExclusiveArea();

          /* expected no of bytes to be received - should not be bigger than message buffer size */
          Dlt_RxBuffer.ExpectedNumberOfBytesToBeReceived = TpSduLength;
          /* store in the variable the init amount of bytes to be stored */
          Dlt_RxBuffer.RemainingBytes = Dlt_RxBuffer.ExpectedNumberOfBytesToBeReceived;

          Dlt_BSWMasterContextPtr->ExitExclusiveArea();

          /* buffer size initial remaining space */
          *bufferSizePtr = (PduLengthType)(DLT_RX_MESSAGE_BUFFER_SIZE);
          RetValue = BUFREQ_OK;
        }
        else
        {
          RetValue = BUFREQ_E_OVFL;
        }
      }
    }
  }

  DBG_DLT_COMSTARTOFRECEPTION_EXIT(BUFREQ_OK, id, TpSduLength, bufferSizePtr);

  return RetValue;
}

/*------------------[Dlt_ComCopyRxData]-------------------------------------*/
/* !LINKSTO SWS_Dlt_00515, 1, Dlt.swdd.Communication.Interface, 1 */
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComCopyRxData
(
  PduIdType id,
  P2CONST(PduInfoType, AUTOMATIC, DLT_APPL_CONST) info,
  P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) bufferSizePtr
)
{
  BufReq_ReturnType RetValue = BUFREQ_E_NOT_OK;

  DBG_DLT_COMCOPYRXDATA_ENTRY(id, info, bufferSizePtr);

#if (DLT_DEV_ERROR_DETECT == STD_OFF)
  TS_PARAM_UNUSED(id);
#else
  if (Dlt_InitStateMaster == DLT_UNINITIALIZED)
  {
    /* !LINKSTO Dlt.swdd.ComCopyRxData.NoInit,1 */
    DLT_DET_REPORT_ERROR(DLT_SID_ComCopyRxData, DLT_E_NOT_INITIALIZED);
  }
  else if ((NULL_PTR == bufferSizePtr) || (NULL_PTR == info))
  {
    /* !LINKSTO Dlt.swdd.ComCopyRxData.NullPtr.DET,1 */
    DLT_DET_REPORT_ERROR(DLT_SID_ComCopyRxData, DLT_E_PARAM_POINTER);
  }
  else if (id != DLT_RX_PDU_ID)
  {
    /* !LINKSTO Dlt.swdd.ComCopyRxData.InvalidRxPdu,1 */
    /* inform user if input parameter pduId is invalid */
    DLT_DET_REPORT_ERROR(DLT_SID_ComCopyRxData, DLT_E_WRONG_PARAMETERS);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
    if ((info->SduLength == 0U) || (info->SduDataPtr == NULL_PTR))
    {
      /* This means that the lower layer queries for our available buffer */
      *bufferSizePtr = DLT_RX_MESSAGE_BUFFER_SIZE;
      RetValue = BUFREQ_OK;
    }
    else
    {
      if (Dlt_RxConnectionState == DLT_RECEPTION_STARTED)
      {
        /* Enter exclusive area to prevent concurrent accesses to RxBuffer */
        Dlt_BSWMasterContextPtr->EnterExclusiveArea();

        /* no of bytes filled and the requested amount of bytes to be copied should not be
         * greater than the maximum expected bytes received */
        if (info->SduLength <= Dlt_RxBuffer.RemainingBytes)
        {
          RetValue = BUFREQ_OK;
          /* copy data into the buffer */
          /* !LINKSTO Dlt.Receive.Message.MultipleFrames,1 */
          TS_MemCpy(&(Dlt_RxBuffer.Buffer[Dlt_RxBuffer.NumberOfBytesFilled]),
          info->SduDataPtr, info->SduLength);

          /* update number of filled bytes for the next call to continue to copy the amount
           * from the address where the last copy was made */
          Dlt_RxBuffer.NumberOfBytesFilled += info->SduLength;

          /* the expected amount to be received from the start of reception has to be
           * updated at each call of CopyRx */
          Dlt_RxBuffer.RemainingBytes -= info->SduLength;

          /* update the remaining size for the buffer */
          *bufferSizePtr -= info->SduLength;
        }
        else
        {
          RetValue = BUFREQ_E_OVFL;
        }
        Dlt_BSWMasterContextPtr->ExitExclusiveArea();

      }
    }
    if (RetValue != BUFREQ_OK)
    {
      Dlt_RxConnectionState = DLT_RECEPTION_CANCELLED;
    }

    DBG_DLT_COMCOPYRXDATA_EXIT(BUFREQ_OK, id, info, bufferSizePtr);
  }
  return RetValue;
}

/*------------------[Dlt_ComRxIndication]-----------------------------------*/
/* !LINKSTO SWS_Dlt_00272,1, Dlt.swdd.Communication.Interface,1 */
FUNC(void, DLT_CODE) Dlt_ComRxIndication
(
  PduIdType DltRxPduId,
  NotifResultType Result
)
{
  Std_ReturnType reqStatus = E_NOT_OK;

  DBG_DLT_COMRXINDICATION_ENTRY(DltRxPduId);
#if (DLT_DEV_ERROR_DETECT == STD_OFF)
  TS_PARAM_UNUSED(DltRxPduId);
#else
  if (Dlt_InitStateMaster == DLT_UNINITIALIZED)
  {
    /* !LINKSTO Dlt.swdd.ComRxIndication.NoInit,1 */
    DLT_DET_REPORT_ERROR(DLT_SID_ComRxIndication, DLT_E_NOT_INITIALIZED);
  }
  else if (DltRxPduId != DLT_RX_PDU_ID)
  {
    /* !LINKSTO Dlt.swdd.ComRxIndication.InvalidRxPdu,1 */
    /* inform user if input parameter pduId is invalid */
    DLT_DET_REPORT_ERROR(DLT_SID_ComRxIndication, DLT_E_WRONG_PARAMETERS);
  }
  else
#endif /* DLT_DEV_ERROR_DETECT == STD_ON */
  {
    if ((Result == E_OK)
        && (Dlt_RxBuffer.NumberOfBytesFilled
            == Dlt_RxBuffer.ExpectedNumberOfBytesToBeReceived)
        && (Dlt_RxConnectionState == DLT_RECEPTION_STARTED))
    {
      reqStatus = E_OK;
      Dlt_RxConnectionState = DLT_RECEPTION_COMPLETED;
    }
    if (reqStatus == E_NOT_OK)
    {
      /* Reset state and clear buffer */
      Dlt_InitReceiveProcessor();
    }
  }

  DBG_DLT_COMRXINDICATION_EXIT(DltRxPduId);
}

#endif /* DLT_ON_ETHERNET == STD_ON */

FUNC(void, DLT_CODE) Dlt_UnpackRxData(void)
{
  /* !LINKSTO Dlt.swdd.DltCore.API.MainFunction.ProcessControlMessages,1 */
  if (Dlt_RxConnectionState == DLT_RECEPTION_COMPLETED)
  {
    /* !LINKSTO Dlt.swdd.ControlMessages.Validation.MinReceivedBytes.BareStdHeader,1 */
    if (Dlt_RxBuffer.NumberOfBytesFilled >= (DLT_RX_MIN_NUMBER_OF_BYTES))
    {
      PduLengthType MessageLength;
      PduLengthType AccumulatedLength = 0U;
      do
      {
        Dlt_MsgLengthType PayloadLength = 0U;
        /* !LINKSTO Dlt.swdd.ControlMessages.Unpacking,1 */
        MessageLength = Dlt_GetMessageLength(AccumulatedLength);

        if (((MessageLength + AccumulatedLength) <= Dlt_RxBuffer.NumberOfBytesFilled) &&
           (MessageLength != 0U))
        {
          uint32 ServiceId = DLT_RESERVED_SERVICE_ID;
          Dlt_CtrlReturnType validationResult = Dlt_ValidateRequest(AccumulatedLength, &ServiceId);

          if (validationResult == DLT_CTRL_OK)
          {
            /* Execute, and get response */
            Dlt_ExecuteRequest(ServiceId, Dlt_PayloadToRespond, &PayloadLength, AccumulatedLength);
          }
          else
          {
            /* !LINKSTO Dlt.swdd.ControlMessages.Validation.NotPassed,1,
                        Dlt.swdd.ControlMessages.ECUID.NotValid,1,
                        Dlt.swdd.ControlMessages.WEID.NotValid,1,
                        Dlt.swdd.ControlMessages.Validation.ServiceId.NotValid,1,
                        Dlt.swdd.ControlMessages.Validation.HeaderNOK,1,
                        Dlt.swdd.ControlMessages.HandlingPointer.Null,1 */
            /* Respond with error */
            Dlt_WriteDwordToByteArrayPayload(&Dlt_PayloadToRespond[DLT_RESPONSE_SERVICE_ID_IDX], ServiceId);
            PayloadLength += DLT_SERVICE_ID_SIZE;

            /* Fifth byte is always the Status */
            Dlt_PayloadToRespond[DLT_RESPONSE_STATUS_IDX] = validationResult;
            PayloadLength++;
          }

          AccumulatedLength += MessageLength;
          /* Payload length shall not be 0*/
          DLT_PRECONDITION_ASSERT(DLT_INSTANCE_ID, (PayloadLength > 0U), DLT_INTERNAL_API_ID);

          /* !LINKSTO Dlt.swdd.ControlMessages.TransmissionOfResponse,1 */
          Dlt_RespondToRequest(Dlt_PayloadToRespond, PayloadLength);
        }
        else
        {
         /* Abort - LEN field was misleading */

         AccumulatedLength = Dlt_RxBuffer.NumberOfBytesFilled;
        }
      }while (AccumulatedLength < Dlt_RxBuffer.NumberOfBytesFilled);
    }

    /* Reset buffer and clear state */
    Dlt_InitReceiveProcessor();
  }
  else if (Dlt_RxConnectionState == DLT_RECEPTION_CANCELLED)
  {
    /* Reset buffer and clear state */
    Dlt_InitReceiveProcessor();
  }
  else
  {
    /* Do nothing */
  }
}

/*-----------------------------[Dlt_InitReceiveProcessor]------------------*/
FUNC(void, DLT_CODE) Dlt_InitReceiveProcessor(void)
{
  /* Enter exclusive area to prevent concurrent accesses to RxBuffer */
  Dlt_BSWMasterContextPtr->EnterExclusiveArea();

  Dlt_RxConnectionState = DLT_RECEPTION_IDLE;
  Dlt_RxBuffer.ExpectedNumberOfBytesToBeReceived = 0U;
  Dlt_RxBuffer.NumberOfBytesFilled = 0U;
  Dlt_RxBuffer.RemainingBytes = 0U;

  Dlt_BSWMasterContextPtr->ExitExclusiveArea();
}

#else /* DLT_RECEPTION_CONTROL_MESSAGES_ENABLED == STD_OFF */

/* CHECK: NOPARSE */
/*
   These functions only exist because the PduR module needs them, but that they have
   no functionality. For coverage reasons the following functions have been excluded
   from measurements so that the overall code coverage report is not affected:
   * Dlt_ComStartOfReception
   * Dlt_ComCopyRxData
   * Dlt_ComRxIndication
 */
/*------------------[Dlt_ComStartOfReception]-------------------------------*/
/* Dummy function, required by PduR */
/* !LINKSTO SWS_Dlt_00517,1, Dlt.swdd.Communication.Interface,1 */
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComStartOfReception(
    PduIdType id,
    PduLengthType TpSduLength,
    P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) bufferSizePtr)
{
  DBG_DLT_COMSTARTOFRECEPTION_ENTRY(id, TpSduLength, bufferSizePtr);

  TS_PARAM_UNUSED(id);
  TS_PARAM_UNUSED(TpSduLength);
  TS_PARAM_UNUSED(bufferSizePtr);

  DLT_UNREACHABLE_CODE_ASSERT(DLT_INSTANCE_ID, DLT_SID_ComStartOfReception);

  DBG_DLT_COMSTARTOFRECEPTION_EXIT(BUFREQ_OK, id, TpSduLength, bufferSizePtr);

  return BUFREQ_OK;
}

/*------------------[Dlt_ComCopyRxData]-------------------------------------*/
/* Dummy function, required by PduR */
/* !LINKSTO SWS_Dlt_00515,1, Dlt.swdd.Communication.Interface,1 */
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_ComCopyRxData(
    PduIdType id,
    P2CONST(PduInfoType, AUTOMATIC, DLT_APPL_CONST) info,
    P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_DATA) bufferSizePtr)
{
  DBG_DLT_COMCOPYRXDATA_ENTRY(id, info, bufferSizePtr);

  TS_PARAM_UNUSED(id);
  TS_PARAM_UNUSED(info);
  TS_PARAM_UNUSED(bufferSizePtr);

  DLT_UNREACHABLE_CODE_ASSERT(DLT_INSTANCE_ID, DLT_SID_ComCopyRxData);

  DBG_DLT_COMCOPYRXDATA_EXIT(BUFREQ_OK, id, info, bufferSizePtr);

  return BUFREQ_OK;
}

/*------------------[Dlt_ComRxIndication]-----------------------------------*/
/* Dummy function, required by PduR */
/* !LINKSTO SWS_Dlt_00272,1, Dlt.swdd.Communication.Interface,1 */
FUNC(void, DLT_CODE) Dlt_ComRxIndication(
    PduIdType DltRxPduId,
    NotifResultType Result)
{
  DBG_DLT_COMRXINDICATION_ENTRY(DltRxPduId);

  TS_PARAM_UNUSED(DltRxPduId);
  TS_PARAM_UNUSED(Result);

  DLT_UNREACHABLE_CODE_ASSERT(DLT_INSTANCE_ID, DLT_SID_ComRxIndication);

  DBG_DLT_COMRXINDICATION_EXIT(DltRxPduId);
}
/* CHECK: PARSE */

#endif /* DLT_RECEPTION_CONTROL_MESSAGES_ENABLED == STD_ON */


#define DLT_STOP_SEC_CODE
#include <Dlt_MemMap.h>

/*==================[internal function definitions]=========================*/

#if (DLT_RECEPTION_CONTROL_MESSAGES_ENABLED == STD_ON)

#define DLT_START_SEC_CODE
#include <Dlt_MemMap.h>


/*-----------------------------[Dlt_GetMessageLength]------------------*/
STATIC FUNC(Dlt_MsgLengthType, DLT_CODE) Dlt_GetMessageLength(Dlt_MsgLengthType OffsetInBuffer)
{
  /* select the Rx buffer */
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) RxBufferPtr = NULL_PTR;

  Dlt_BSWMasterContextPtr->EnterExclusiveArea();
  RxBufferPtr = &Dlt_RxBuffer.Buffer[OffsetInBuffer];
  Dlt_BSWMasterContextPtr->ExitExclusiveArea();

  return  (((Dlt_MsgLengthType)((Dlt_MsgLengthType)(RxBufferPtr[DLT_LEN_HEADER_OFFSET]) << 8U)) |
                               ((Dlt_MsgLengthType)(RxBufferPtr[DLT_LEN_HEADER_OFFSET+1U]) & 0x00ffU));

}


/*-----------------------------[Dlt_ValidateRequest]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_ValidateRequest
(
  Dlt_MsgLengthType OffsetInBuffer,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_DATA) ServiceId
)
{
  Dlt_CtrlReturnType RetValue = DLT_CTRL_ERROR;
  uint32 LocalServiceId = DLT_RESERVED_SERVICE_ID;
  PduLengthType ReceivedHeaderLength;
  PduLengthType Dlt_LocalNumberOfBytesFilled;

  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) RxBufferPtr = NULL_PTR;
  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) PayloadReceived = NULL_PTR;
  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) MessageInfo = NULL_PTR;
  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) HeaderType = NULL_PTR;

  ReceivedHeaderLength = Dlt_CalculateReceivedHeaderLength();

  if ( (OffsetInBuffer + (DLT_RX_BUFFER_TYPE_STRUCT * (sizeof(PduLengthType))) + ReceivedHeaderLength) <= DLT_RX_MESSAGE_BUFFER_SIZE)
  {
    Dlt_BSWMasterContextPtr->EnterExclusiveArea();
    /* select the Rx buffer */
    RxBufferPtr = &Dlt_RxBuffer.Buffer[OffsetInBuffer];
    Dlt_LocalNumberOfBytesFilled = Dlt_RxBuffer.NumberOfBytesFilled;
    Dlt_BSWMasterContextPtr->ExitExclusiveArea();

    PayloadReceived = &RxBufferPtr[ReceivedHeaderLength];
    MessageInfo = &RxBufferPtr[ReceivedHeaderLength - DLT_EXT_HEADER_LENGTH_STATIC]; /* MSIN field */
    HeaderType = &RxBufferPtr[DLT_HTYP_IDX]; /* HTYP field */

    /* Validate that we have received the correct number of bytes.
     * We should have at least the headers and the service ID
     * Buffer access is protected by reception state */
    /* !LINKSTO Dlt.swdd.ControlMessages.Validation.MinReceivedBytes,1 */
    if (Dlt_LocalNumberOfBytesFilled >= (ReceivedHeaderLength + DLT_SERVICE_ID_SIZE))
    {
      /* !LINKSTO Dlt.swdd.ControlMessages.Validation.MSCI,1 */
      if (((*MessageInfo & DLT_MSTP_MASK) == (DLT_TYPE_CONTROL_BITS_IN_MSIN)) && ((*MessageInfo & DLT_MTIN_MASK) == (DLT_CONTROL_REQUEST_BITS_IN_MSIN)))
      {
        /* ECU ID must be sent */
        /* !LINKSTO Dlt.swdd.ControlMessages.Validation.WEID,1 */
        if((*HeaderType & DLT_HEADER_ECU_ID_BIT_MASK) != 0U)
        {
          /* local array to hold the ECU ID field */
#if (STD_ON == DLT_GETECUID_AT_RUNTIME)
            uint8 ExpectedEcuId[DLT_ECU_HEADER_LENGTH_STATIC] = {0};
            /* !LINKSTO EB_Req_EcuIdChoice.Callout.ValidateHeader, 1 */
            P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA)pEcuId = (uint8 *)Dlt_AppGetEcuIdAddress();
            if (NULL_PTR != pEcuId)
            {
              TS_MemCpy(ExpectedEcuId,pEcuId,DLT_ECU_HEADER_LENGTH_STATIC);
            }
#if (DLT_DEV_ERROR_DETECT == STD_ON)
            else
            {
              DLT_DET_REPORT_ERROR(DLT_SID_GetEcuIdAddress, DLT_E_PARAM_POINTER);
            }
#endif /* #if (DLT_DEV_ERROR_DETECT == STD_ON) */
#else
          const uint8 ExpectedEcuId[DLT_ECU_HEADER_LENGTH_STATIC] = {DLT_ECU_ID};
#endif

          /* !LINKSTO Dlt.swdd.ControlMessages.Validation.ECUID,1 */
          if (TS_MemCmp(ExpectedEcuId, &RxBufferPtr[DLT_ECUID_IDX], DLT_ECU_HEADER_LENGTH_STATIC) == E_OK)
          {
            if((*HeaderType & DLT_HEADER_ENDIANES_BIT_MASK) == 0U)
            {
              /* !LINKSTO SWS_Dlt_00304, 1 */
              LocalServiceId = DLT_UINT32_FROM_BYTE_ARRAY_LSB(&(PayloadReceived[DLT_REQUEST_SERVICE_ID_IDX]));
            }
            else
            {
              /* !LINKSTO SWS_Dlt_00097, 1 */
              /* !LINKSTO Dlt.swdd.ControlMessages.HTYP.HeaderNotValid,1 */
              LocalServiceId = DLT_UINT32_FROM_BYTE_ARRAY_MSB(&(PayloadReceived[DLT_REQUEST_SERVICE_ID_IDX]));
            }
            /* !LINKSTO Dlt.swdd.ControlMessages.Validation.ValidServiceId,1 */
            /* !LINKSTO EB_Req_CheckForImplementation,1 */
            if ((LocalServiceId < DLT_NUMBER_OF_CONTROL_HANDLERS) &&
                (LocalServiceId != DLT_RESERVED_SERVICE_ID) &&
                (Dlt_ControlMessages[LocalServiceId].MessageHandlerPtr != NULL_PTR))
            {
              /* Check if we have enough received bytes */
              /* !LINKSTO Dlt.swdd.ControlMessages.Validation.FullReceivedNumberOfBytes,1 */
              if (Dlt_LocalNumberOfBytesFilled >= ((PduLengthType)(Dlt_ControlMessages[LocalServiceId].BaseLength) + ReceivedHeaderLength))
              {
                RetValue = DLT_CTRL_OK;
              }
            }
            else
            {
              RetValue = DLT_CTRL_NOT_SUPPORTED;
            }
          }
        }
      }
    }
  }
  /* Provide ServiceId */
  *ServiceId = LocalServiceId;
  return RetValue;
}

/*-----------------------------[Dlt_ExecuteRequest]------------------*/
STATIC FUNC(void, DLT_CODE) Dlt_ExecuteRequest
(
  uint32 ServiceId,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) PayloadToBeSent,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) PayloadToBeSentLength,
  Dlt_MsgLengthType OffsetInBuffer
)
{
  Dlt_CtrlReturnType Status;
  uint8 ReceivedHeaderLength;
  Dlt_MsgLengthType responseLength = 0U;
  Dlt_MsgLengthType PayloadLength = 0;
  /* select the Rx buffer */
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) RxBufferPtr = NULL_PTR;

  Dlt_BSWMasterContextPtr->EnterExclusiveArea();
  RxBufferPtr = &Dlt_RxBuffer.Buffer[OffsetInBuffer];
  Dlt_BSWMasterContextPtr->ExitExclusiveArea();

  ReceivedHeaderLength = Dlt_CalculateReceivedHeaderLength();

  /* Execute the request*/
  /* !LINKSTO Dlt.swdd.ControlMessages.Execution,1 */
  Status = Dlt_ControlMessages[ServiceId].MessageHandlerPtr(RxBufferPtr, ReceivedHeaderLength, ResponseBuffer, &responseLength);

  /* Fill in the payload */
  /* First 4 bytes are always the service Id */
  Dlt_WriteDwordToByteArrayPayload(&PayloadToBeSent[DLT_RESPONSE_SERVICE_ID_IDX], ServiceId);
  PayloadLength += DLT_SERVICE_ID_SIZE;

  /* 5th byte is always the Status */
  /* !LINKSTO Dlt.ASR431.SWS_Dlt_00700, 1 */
  PayloadToBeSent[DLT_RESPONSE_STATUS_IDX] = Status;
  PayloadLength++;

  if (responseLength > 0U)
  {
    /* Copy the response into the payload */
    TS_MemCpy(&PayloadToBeSent[PayloadLength], ResponseBuffer, responseLength);
    PayloadLength += responseLength;
  }
  *PayloadToBeSentLength = PayloadLength;
}

/*-----------------------------[Dlt_RespondToRequest]------------------*/
STATIC FUNC(void, DLT_CODE) Dlt_RespondToRequest
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) PayloadToBeSent,
  Dlt_MsgLengthType PayloadToBeSentLength
)
{
  /* Header must always contain extended header */
  uint8 Header[DLT_MAX_STD_HEADER_LENGTH + DLT_EXT_HEADER_LENGTH_STATIC];
  uint8 NextHeaderFieldIdx = DLT_MIN_HEADER_LENGTH; /* Minimum length of the header */
  uint8 LocalHeaderLength;
  Dlt_MsgLengthType FullMessageLength;
  Dlt_MessageCommonInfoType MsgInfo;
  uint32 timestamp;
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  uint8 ChannelIdx = DEFAULT_LOG_CHN_IDX;
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
  uint8 ChannelIdx = 0U;
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  timestamp = Dlt_GetTimestamp();

  /* Calculate length for the response in header of a control message*/
  LocalHeaderLength = Dlt_CalculateRespHdrLength();

  FullMessageLength  = (Dlt_MsgLengthType) LocalHeaderLength + PayloadToBeSentLength;

  Dlt_BSWMasterContextPtr->EnterExclusiveArea();
  /* WEID and UEH are always being set, all other fields are dependent on the configuration */
  Header[DLT_HTYP_HEADER_OFFSET] =                  DLT_HEADER_EXTENDED_HEADER_BIT
                                                  | DLT_HEADER_ECU_ID_BIT
                                                  | Dlt_HeaderUseSessionID
#if (DLT_IMPLEMENT_TIMESTAMP != STD_OFF)
                                                  | Dlt_HeaderUseTimestamp
#endif /* DLT_IMPLEMENT_TIMESTAMP != STD_OFF */
                                                  ;
  Dlt_BSWMasterContextPtr->ExitExclusiveArea();
  /* !LINKSTO Dlt.ASR431.SWS_Dlt_00674,1 */
  /* Create the standard header fields in Dlt_LocalHeaderBuffer */
  Dlt_CreateStandardHeader(ChannelIdx, 0U, Header, &NextHeaderFieldIdx, FullMessageLength, timestamp);

  MsgInfo.arg_count = 1U;
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  TS_MemBZero(&MsgInfo.app_id, sizeof(MsgInfo.app_id));
  TS_MemBZero(&MsgInfo.context_id, sizeof(MsgInfo.context_id));
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
  MsgInfo.app_id = 0U;
  MsgInfo.context_id = 0U;
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */
  MsgInfo.log_level_trace_info = 0U;

  /* !LINKSTO EB.Req.DltMessageOptionsBitField, 1 */
#if (DLT_MESSAGE_OPTIONS_BITFIELD == AUTOSAR_421)
  MsgInfo.options = 0x03U; /* 0000 0011 - bits 0, 1, 2 for message_type, which is 0x03U (DLT_TYPE_CONTROL)
                            *           - bit 3 for verbose_mode, which is 0x00U (DLT_VERBOSE_MODE_OFF) */
#else /* #if (DLT_MESSAGE_OPTIONS_BITFIELD == AUTOSAR_431) */
  MsgInfo.options = 0x06U; /* 0000 0110 - bits 1, 2, 3 for message_type, which is 0x03U (DLT_TYPE_CONTROL)
                            *           - bit 0 for verbose_mode, which is 0x00U (DLT_VERBOSE_MODE_OFF) */
#endif /* #if (DLT_MESSAGE_OPTIONS_BITFIELD == AUTOSAR_421) */

  /* Create the extended header fields in header */
  Dlt_CreateExtendedHeader(&MsgInfo, Header, &NextHeaderFieldIdx);

  Dlt_SendResponse(Header, PayloadToBeSent, LocalHeaderLength, PayloadToBeSentLength);
}

/*-----------------------------[Dlt_CalculateReceivedHeaderLength]------------------*/
STATIC FUNC(uint8, DLT_CODE) Dlt_CalculateReceivedHeaderLength(void)
{
  uint8 LocalHeaderLength = DLT_MIN_HEADER_LENGTH;
  uint8 HeaderType;

  Dlt_BSWMasterContextPtr->EnterExclusiveArea();
  HeaderType = Dlt_RxBuffer.Buffer[DLT_HTYP_IDX]; /* HTYP field */
  Dlt_BSWMasterContextPtr->ExitExclusiveArea();

  if((HeaderType & DLT_HEADER_ECU_ID_BIT_MASK) != 0U)
  {
    LocalHeaderLength += DLT_ECU_HEADER_LENGTH_STATIC;
  }
  if((HeaderType & DLT_HEADER_SESSION_ID_BIT_MASK) != 0U)
  {
    LocalHeaderLength += DLT_SEID_HEADER_LENGTH_STATIC;
  }
  if((HeaderType & DLT_HEADER_TIMESTAMP_BIT_MASK) != 0U)
  {
    LocalHeaderLength += DLT_TMSP_HEADER_LENGTH_STATIC;
  }
  /* Extended header must be always received */
  LocalHeaderLength += DLT_EXT_HEADER_LENGTH_STATIC;

  return LocalHeaderLength;
}

#if (DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON)

/*-----------------------------[Dlt_GetLogInfoWrapper]--------------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetLogInfoWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  /* !LINKSTO Dlt.ASR431.Ref_SWS_Dlt_00706, 1 */
  Dlt_CtrlReturnType retValWrapper = DLT_CTRL_ERROR;
  Std_ReturnType retValFunction = E_NOT_OK;

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  Dlt_ApplicationIDType appId = {0U};
  Dlt_ContextIDType contextId = {0U};
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  uint8 status = 0U;

  P2CONST(uint8, AUTOMATIC, DLT_CONST) RequestPayload = &RequestBuffer[ReceivedHeaderLength];

  /* Get data from payload */
  uint8 options = RequestPayload[DLT_CM_0x03_OPTIONS_PAYLOAD_IDX];

  /* Get the AppId/ContextId tuple from the request payload */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  DLT_MSB_TO_PLATFORM_ENDIANNESS(appId, RequestPayload, DLT_CM_0x03_APP_ID_PAYLOAD_IDX);
  DLT_MSB_TO_PLATFORM_ENDIANNESS(contextId, RequestPayload, DLT_CM_0x03_CTX_ID_PAYLOAD_IDX);
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
  Dlt_ApplicationIDType appId = DLT_UINT32_FROM_BYTE_ARRAY_MSB(&(RequestPayload[DLT_CM_0x03_APP_ID_PAYLOAD_IDX]));
  Dlt_ContextIDType contextId = DLT_UINT32_FROM_BYTE_ARRAY_MSB(&(RequestPayload[DLT_CM_0x03_CTX_ID_PAYLOAD_IDX]));
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_GetLogInfo, 1 */
  /* !LINKSTO Dlt.ASR431.SWS_Dlt_00705, 1 */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  retValFunction = Dlt_GetLogInfo(options,
                                  ((const Dlt_ApplicationIDType*)(void*)&appId),
                                  ((const Dlt_ContextIDType*)(void*)&contextId),
                                  &status,
                                  Response);
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
  retValFunction = Dlt_GetLogInfo(options, appId, contextId, &status, Response);
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  if ((retValFunction == E_OK) && (status == DLT_CTRL_OK))
  {
    uint16 AppIdx;
    uint16 NoOfContextIds = 0U;
    Dlt_AppToContextIdTableIdxType AppToCtxIdx;

    for (AppIdx = 0U; AppIdx < DLT_UINT16_FROM_BYTE_ARRAY_MSB(Response); AppIdx++)
    {
      /* Calculate the total number of ContextIds across all the requested AppIds */
      NoOfContextIds += (uint16)Dlt_AppToContextIdTableIdx[AppIdx];

      if (DLT_GETLOGINFO_OPTIONS_WITH_DESC == options)
      {
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
        /* Add to the response length each AppId's description length */
        *ResponseLength += Dlt_AppIdTable[AppIdx].DltAppDescription.AppDescLen;
#else /* DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431 */
        /* When textual descriptions are requested and DltServiceAPI != AUTOSAR_431,
         * increase the response length by one byte (description length) */
        *ResponseLength += 1U;
#endif /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */
      }

      for (AppToCtxIdx = 0U; AppToCtxIdx < Dlt_AppToContextIdTableIdx[AppIdx]; AppToCtxIdx++)
      {
        if (DLT_GETLOGINFO_OPTIONS_WITH_DESC == options)
        {
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
          Dlt_ContextIdTablePtrType ContextIdTable = &Dlt_BSWMasterContextPtr->CoreContext.ContextIdTable[AppToCtxIdx];
          /* Add to the response length each ContextId's description length */
          *ResponseLength += ContextIdTable->ContextDescription.CtxtDescLen;
#else /* DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431 */
          /* When textual descriptions are requested and DltServiceAPI != AUTOSAR_431,
           * increase the response length by one byte (description length) */
          *ResponseLength += 1U;
#endif /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */
        }
      }
    }

    if (DLT_GETLOGINFO_OPTIONS_NO_DESC == options)
    {
      *ResponseLength = (Dlt_MsgLengthType)(DLT_APP_ID_COUNT_SIZE                                     +
                        (DLT_RESPONSE_APP_ID_SIZE_NO_DESC * DLT_UINT16_FROM_BYTE_ARRAY_MSB(Response)) +
                        (DLT_RESPONSE_CTX_ID_SIZE_NO_DESC * NoOfContextIds));
    }
    else
    {
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
      *ResponseLength += (Dlt_MsgLengthType)(DLT_APP_ID_COUNT_SIZE                                       +
                         (DLT_RESPONSE_APP_ID_SIZE_WITH_DESC * DLT_UINT16_FROM_BYTE_ARRAY_MSB(Response)) +
                         (DLT_RESPONSE_CTX_ID_SIZE_WITH_DESC * NoOfContextIds));
#else
      *ResponseLength += (Dlt_MsgLengthType)(DLT_APP_ID_COUNT_SIZE                                     +
                         (DLT_RESPONSE_APP_ID_SIZE_NO_DESC * DLT_UINT16_FROM_BYTE_ARRAY_MSB(Response)) +
                         (DLT_RESPONSE_APP_ID_SIZE_NO_DESC * NoOfContextIds));
#endif
    }

    /* Make sure the last 4 bytes of the response are the reserved ones
     * that shall be filled with zero */
    Response[*ResponseLength + 0U] = 0U;
    Response[*ResponseLength + 1U] = 0U;
    Response[*ResponseLength + 2U] = 0U;
    Response[*ResponseLength + 3U] = 0U;

    /* Increase the response length in order to contain the reserved bytes */
    *ResponseLength += DLT_RESPONSE_RESERVED_BYTES;

    retValWrapper = DLT_CTRL_OK;
  }
  else
  {
    /* !LINKSTO EB_Req_Dlt_GetLogInfo.InvalidRetValue, 1 */
    *ResponseLength = 0U;
  }

  return retValWrapper;
}

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
/*-----------------------------[Dlt_SetLogChannelAssignmentWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetLogChannelAssignmentWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue = DLT_CTRL_ERROR;
  Std_ReturnType RetValueFunction = E_NOT_OK;
  P2CONST(uint8, AUTOMATIC, DLT_VAR) RequestPayload = &RequestBuffer[ReceivedHeaderLength];
  Dlt_ApplicationIDType appId = {0U};
  Dlt_ContextIDType contextId = {0U};
  Dlt_LogChannelNameType LogChannelName = {0U};
  Dlt_AssignmentOperationType AddRemoveOp = 0U;

  /* Get the AppId/ContextId tuple from the request payload */
  DLT_MSB_TO_PLATFORM_ENDIANNESS(appId, RequestPayload, DLT_CM_0x20_APP_ID_PAYLOAD_IDX);
  DLT_MSB_TO_PLATFORM_ENDIANNESS(contextId, RequestPayload, DLT_CM_0x20_CONTEXT_ID_PAYLOAD_IDX);
  /* Get the log channel name from the request payload */
  DLT_MSB_TO_PLATFORM_ENDIANNESS(LogChannelName, RequestPayload, DLT_CM_0x20_LOG_CHN_PAYLOAD_IDX);

  /* Get the add/remove operation from the request payload */
  AddRemoveOp = RequestPayload[DLT_CM_0x20_ADDREMOVEOP_PAYLOAD_IDX];

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_SetLogChannelAssignment,1 */
  RetValueFunction = Dlt_SetLogChannelAssignment( ((const Dlt_ApplicationIDType*)(void*)&appId),
                                                  ((const Dlt_ContextIDType*)(void*)&contextId),
                                                  (const Dlt_LogChannelNameType*)(void*)LogChannelName,
                                                  AddRemoveOp );

  if (RetValueFunction == E_OK)
  {
    RetValue = DLT_CTRL_OK;
  }

  *ResponseLength = 0U;

  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);
  return RetValue;
}
/*-----------------------------[Dlt_GetLogChannelNamesWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetLogChannelNamesWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue = DLT_CTRL_ERROR;

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_GetLogChannelNames,1 */
  if (Dlt_GetLogChannelNames(&Response[0U], (Dlt_LogChannelNameArrayType*)(void*)&Response[DLT_CHANNEL_IDX_SIZE]) == E_OK)
  {
    *ResponseLength = DLT_CHANNEL_IDX_SIZE + (((Dlt_MsgLengthType)(Response[0U])) * DLT_CHANNEL_NAME_SIZE);
    RetValue = DLT_CTRL_OK;
  }
  else
  {
    *ResponseLength = 0U;
  }

  /* No requested data besides the Service Id*/
  TS_PARAM_UNUSED(RequestBuffer);
  TS_PARAM_UNUSED(ReceivedHeaderLength);

  return RetValue;
}
/*-----------------------------[Dlt_SetLogChannelThresholdWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetLogChannelThresholdWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue = DLT_CTRL_ERROR;
  Std_ReturnType RetValueFunction = E_NOT_OK;
  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) RequestPayload = &RequestBuffer[ReceivedHeaderLength];
  Dlt_LogChannelNameType logChannelName = {0U};
  Dlt_MessageLogLevelType newThreshold = 0U;
  boolean newTraceStatus = 0U;

  /* copy data into the buffer */
  DLT_MSB_TO_PLATFORM_ENDIANNESS(logChannelName, RequestPayload, DLT_CM_0x21_LOG_CHN_PAYLOAD_IDX);
  newThreshold = RequestPayload[DLT_CM_0x21_LOG_NEW_THRESHOLD_PAYLOAD_IDX];
  newTraceStatus = RequestPayload[DLT_CM_0x21_LOG_NEW_TRACESTS_IDX];

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_SetLogChannelThreshold,1 */
  RetValueFunction = Dlt_SetLogChannelThreshold((const Dlt_LogChannelNameType*)(void*)logChannelName, newThreshold, newTraceStatus);

  if (RetValueFunction == E_OK)
  {
    RetValue = DLT_CTRL_OK;
  }

  *ResponseLength = 0U;

  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);
  return RetValue;
}
/*-----------------------------[Dlt_GetLogChannelThresholdWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetLogChannelThresholdWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue = DLT_CTRL_ERROR;
  Std_ReturnType RetValueFunction = E_NOT_OK;
  Dlt_LogChannelNameType logChannelName = {0U};

  DLT_MSB_TO_PLATFORM_ENDIANNESS(logChannelName, RequestBuffer, (ReceivedHeaderLength+DLT_CM_0x22_LOG_CHN_PAYLOAD_IDX));

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_GetLogChannelThreshold,1 */
  RetValueFunction = Dlt_GetLogChannelThreshold((const Dlt_LogChannelNameType*)(void*)logChannelName, &Response[0U], &Response[1U]);

  if (RetValueFunction == E_OK)
  {
    RetValue = DLT_CTRL_OK;
  }

  *ResponseLength = 3U;

  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);
  return RetValue;
}

#endif /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */

/*-----------------------------[Dlt_SetLogLevelWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetLogLevelWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  /* !LINKSTO Dlt.ASR431.Ref_SWS_Dlt_00702, 1 */
  Dlt_CtrlReturnType retValWrapper = DLT_CTRL_ERROR;
  Std_ReturnType     retValASR43   = E_NOT_OK;

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  Dlt_ApplicationIDType appId = {0U};
  Dlt_ContextIDType contextId = {0U};
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) RequestPayload = &RequestBuffer[ReceivedHeaderLength];

  /* Get data from payload */
  Dlt_MessageLogLevelType NewLogLevel = RequestPayload[DLT_CM_0x01_LOG_LEVEL_PAYLOAD_IDX];

  /* Get the AppId/ContextId tuple from the request payload */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  DLT_MSB_TO_PLATFORM_ENDIANNESS(appId, RequestPayload, DLT_CM_0x01_APP_ID_PAYLOAD_IDX);
  DLT_MSB_TO_PLATFORM_ENDIANNESS(contextId, RequestPayload, DLT_CM_0x01_CONTEXT_ID_PAYLOAD_IDX);
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
  Dlt_ApplicationIDType appId = DLT_UINT32_FROM_BYTE_ARRAY_MSB(&(RequestPayload[DLT_CM_0x01_APP_ID_PAYLOAD_IDX]));
  Dlt_ContextIDType contextId = DLT_UINT32_FROM_BYTE_ARRAY_MSB(&(RequestPayload[DLT_CM_0x01_CONTEXT_ID_PAYLOAD_IDX]));
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_SetLogLevel, 1 */
#if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431)
  TS_PARAM_UNUSED(retValASR43);

  retValWrapper = Dlt_SetLogLevel(appId, contextId, NewLogLevel);
#else /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */
  /* !LINKSTO Dlt.ASR431.SWS_Dlt_00701, 1 */
  retValASR43 = Dlt_SetLogLevel(((const Dlt_ApplicationIDType*)(void*)&appId),
                                ((const Dlt_ContextIDType*)(void*)contextId),
                                NewLogLevel);

  if (retValASR43 == E_OK)
  {
    retValWrapper = DLT_CTRL_OK;
  }
#endif /* DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431 */

  *ResponseLength = 0U;
  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);

  return retValWrapper;
}

/*-----------------------------[Dlt_SetTraceStatusWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetTraceStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  /* !LINKSTO Dlt.ASR431.Ref_SWS_Dlt_00704, 1 */
  Dlt_CtrlReturnType retValWrapper = DLT_CTRL_ERROR;
  Std_ReturnType     retValASR43   = E_NOT_OK;

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  Dlt_ApplicationIDType appId = {0U};
  Dlt_ContextIDType contextId = {0U};
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) RequestPayload = &RequestBuffer[ReceivedHeaderLength];

  /* Get data from payload */
  Dlt_MessageTraceStatusType NewTraceStatus = RequestPayload[DLT_CM_0x02_TRACE_STATUS_PAYLOAD_IDX];

  /* Get the AppId/ContextId tuple from the request payload */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  DLT_MSB_TO_PLATFORM_ENDIANNESS(appId, RequestPayload, DLT_CM_0x02_APP_ID_PAYLOAD_IDX);
  DLT_MSB_TO_PLATFORM_ENDIANNESS(contextId, RequestPayload, DLT_CM_0x02_CONTEXT_ID_PAYLOAD_IDX);
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
  Dlt_ApplicationIDType appId = DLT_UINT32_FROM_BYTE_ARRAY_MSB(&(RequestPayload[DLT_CM_0x02_APP_ID_PAYLOAD_IDX]));
  Dlt_ContextIDType contextId = DLT_UINT32_FROM_BYTE_ARRAY_MSB(&(RequestPayload[DLT_CM_0x02_CONTEXT_ID_PAYLOAD_IDX]));
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_SetTraceStatus, 1 */
#if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431)
  TS_PARAM_UNUSED(retValASR43);

  retValWrapper = Dlt_SetTraceStatus(appId, contextId, (boolean)NewTraceStatus);
#else /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */
  /* !LINKSTO Dlt.ASR431.SWS_Dlt_00703, 1 */
  retValASR43 = Dlt_SetTraceStatus(((const Dlt_ApplicationIDType*)(void*)&appId),
                                   ((const Dlt_ContextIDType*)(void*)&contextId),
                                   NewTraceStatus);
  if (retValASR43 == E_OK)
  {
    retValWrapper = DLT_CTRL_OK;
  }
#endif /* DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431 */
  *ResponseLength = 0U;

  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);

  return retValWrapper;
}

/*-----------------------------[Dlt_GetTraceStatusWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetTraceStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType retValWrapper = DLT_CTRL_ERROR;
  Std_ReturnType retValASR43 = E_NOT_OK;

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  Dlt_ApplicationIDType appId = {0U};
  Dlt_ContextIDType contextId = {0U};
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) RequestPayload = &RequestBuffer[ReceivedHeaderLength];

  Dlt_MessageTraceStatusType TraceStatusASR42 = DLT_TRACE_STATUS_OFF;
  boolean TraceStatusASR43 = DLT_TRACE_STATUS_OFF;

  /* Get the AppId/ContextId tuple from the request payload */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  DLT_MSB_TO_PLATFORM_ENDIANNESS(appId, RequestPayload, DLT_CM_0x1F_APP_ID_PAYLOAD_IDX);
  DLT_MSB_TO_PLATFORM_ENDIANNESS(contextId, RequestPayload, DLT_CM_0x1F_CONTEXT_ID_PAYLOAD_IDX);
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
  Dlt_ApplicationIDType appId = DLT_UINT32_FROM_BYTE_ARRAY_MSB(&(RequestPayload[DLT_CM_0x1F_APP_ID_PAYLOAD_IDX]));
  Dlt_ContextIDType contextId = DLT_UINT32_FROM_BYTE_ARRAY_MSB(&(RequestPayload[DLT_CM_0x1F_CONTEXT_ID_PAYLOAD_IDX]));
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

/* !LINKSTO EB_Req_Dlt_GetTraceStatus, 1 */
#if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431)
  TS_PARAM_UNUSED(retValASR43);
  TS_PARAM_UNUSED(TraceStatusASR43);

  retValWrapper = Dlt_ASR42_GetTraceStatus(appId, contextId, &TraceStatusASR42);

  if (retValWrapper == DLT_CTRL_OK)
  {
    *ResponseLength = 1U;
    Response[0] = TraceStatusASR42;
  }
#else /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */
  TS_PARAM_UNUSED(TraceStatusASR42);

  retValASR43 = Dlt_ASR43_GetTraceStatus(((const Dlt_ApplicationIDType*)(void*)&appId),
                                         ((const Dlt_ContextIDType*)(void*)&contextId),
                                         &TraceStatusASR43);

  if (retValASR43 == E_OK)
  {
    retValWrapper = DLT_CTRL_OK;
    *ResponseLength = 1U;
    /* !LINKSTO Dlt.ASR431.SWS_Dlt_00719, 1 */
    Response[0] = TraceStatusASR43;
  }
#endif /* DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431 */
  else
  {
    /* !LINKSTO EB_Req_Dlt_GetTraceStatus.InvalidRetValue, 1 */
    *ResponseLength = 0U;
  }

  TS_PARAM_UNUSED(ReceivedHeaderLength);

  return retValWrapper;
}

#endif /* DLT_IMPLEMENT_FILTER_MESSAGES == STD_ON */

/*-----------------------------[Dlt_SetDefaultLogLevelWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetDefaultLogLevelWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType retValWrapper  = DLT_CTRL_ERROR;
  Std_ReturnType     retValASR43    = E_NOT_OK;

  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) RequestPayload = &RequestBuffer[ReceivedHeaderLength];

  /* Get data from payload */
  Dlt_MessageLogLevelType NewLogLevel = RequestPayload[DLT_CM_0x11_LOG_LEVEL_PAYLOAD_IDX];

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_SetDefaultLogLevel,1 */
#if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431)
  TS_PARAM_UNUSED(retValASR43);

  retValWrapper = Dlt_SetDefaultLogLevel(NewLogLevel);
#else /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */
  /* !LINKSTO Dlt.ASR431.SWS_Dlt_00715, 1 */
  retValASR43 = Dlt_SetDefaultLogLevel(NewLogLevel);

  if (retValASR43 == E_OK)
  {
    retValWrapper = DLT_CTRL_OK;
  }
#endif /* DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431 */

  *ResponseLength = 0U;

  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);
  TS_PARAM_UNUSED(ReceivedHeaderLength);

  return retValWrapper;
}

/*-----------------------------[Dlt_GetDefaultLogLevelWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetDefaultLogLevelWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType      RetValWrapper = DLT_CTRL_ERROR;
  Dlt_MessageLogLevelType RetValFunction_ASR42;
  Std_ReturnType          RetValFunction_ASR43;

  Dlt_MessageLogLevelType defaultLogLevel = DLT_LOG_OFF;

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_GetDefaultLogLevel,1 */
#if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431)
  TS_PARAM_UNUSED(RetValFunction_ASR43);
  TS_PARAM_UNUSED(defaultLogLevel);

  RetValFunction_ASR42 = Dlt_GetDefaultLogLevel();

  if (RetValFunction_ASR42 != DLT_LOG_OFF)
  {
    RetValWrapper = DLT_CTRL_OK;
    *ResponseLength = 1U;
    /* return the default log level */
    Response[0] = RetValFunction_ASR42;
  }
#else /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */
  TS_PARAM_UNUSED(RetValFunction_ASR42);

  /* !LINKSTO Dlt.ASR431.SWS_Dlt_00708, 1 */
  RetValFunction_ASR43 = Dlt_GetDefaultLogLevel(&defaultLogLevel);

  if (RetValFunction_ASR43 == E_OK)
  {
    RetValWrapper = DLT_CTRL_OK;
    *ResponseLength = 1U;
    /* return the default log level */
    Response[0] = defaultLogLevel;
  }
#endif /* DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431 */

  else
  {
    /* !LINKSTO EB_Req_Dlt_GetDefaultLogLevel.InvalidRetValue,1 */
    *ResponseLength = 0U;
  }

  /* No request parameters */
  TS_PARAM_UNUSED(RequestBuffer);
  TS_PARAM_UNUSED(ReceivedHeaderLength);

  return RetValWrapper;
}

#if (DLT_TRAFFIC_SHAPER_ENABLED == STD_ON)
/*-----------------------------[Dlt_SetComInterfaceMaxBandwidth]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetComInterfaceMaxBandwidthWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue;
  uint32 MaxBandwidth;
  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) RequestPayload = &RequestBuffer[ReceivedHeaderLength];

  const uint8 HeaderType = RequestBuffer[DLT_HTYP_IDX]; /* HTYP field */

  if ((HeaderType & DLT_HEADER_ENDIANES_BIT_MASK) == 0U)
  {
    /* Get data from payload */
    MaxBandwidth = DLT_UINT32_FROM_BYTE_ARRAY_LSB(&(RequestPayload[DLT_CM_0x08_MAX_BANDWIDTH_PAYLOAD_IDX]));
  }
  else
  {
    /* Get data from payload */
    MaxBandwidth = DLT_UINT32_FROM_BYTE_ARRAY_MSB(&(RequestPayload[DLT_CM_0x08_MAX_BANDWIDTH_PAYLOAD_IDX]));
  }


  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_SetComInterfaceMaxBandwidth,1 */
  RetValue = Dlt_SetComInterfaceMaxBandwidth(MaxBandwidth);
  *ResponseLength = 0U;

  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);
  return RetValue;
}

/*-----------------------------[Dlt_GetComInterfaceMaxBandwidthWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetComInterfaceMaxBandwidthWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue = DLT_CTRL_ERROR;
  uint32 RetBWValue;

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_GetComInterfaceMaxBandwidth,1 */
  RetBWValue = Dlt_GetComInterfaceMaxBandwidth();

  if (RetBWValue != 0U)
  {
    RetValue = DLT_CTRL_OK;
    *ResponseLength = 4U;
    /* return the the maximum bandwidth allowed for this interface in bit per second */
    Dlt_WriteDwordToByteArrayPayload(Response, RetBWValue);
  }
  else
  {
    /* !LINKSTO EB_Req_Dlt_GetComInterfaceMaxBandwidth.InvalidRetValue,1 */
    *ResponseLength = 0U;
  }

  /* No request parameters */
  TS_PARAM_UNUSED(RequestBuffer);
  TS_PARAM_UNUSED(ReceivedHeaderLength);
  return RetValue;
}
#endif /* DLT_TRAFFIC_SHAPER_ENABLED == STD_ON */

/*-----------------------------[Dlt_SetDefaultTraceStatusWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetDefaultTraceStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType retValWrapper = DLT_CTRL_ERROR;

  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) RequestPayload = &RequestBuffer[ReceivedHeaderLength];

  /* Get data from payload */
  boolean NewTraceStatus = RequestPayload[DLT_CM_0x12_TRACE_STATUS_PAYLOAD_IDX];

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_SetDefaultTraceStatus,1 */
#if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431)

  /* !LINKSTO Dlt.ASR431.SWS_Dlt_00716, 1 */
  retValWrapper = Dlt_SetDefaultTraceStatus(NewTraceStatus);

#else /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */

  /* !LINKSTO Dlt.ASR431.SWS_Dlt_00716, 1 */
  if (Dlt_SetDefaultTraceStatus(NewTraceStatus) == E_OK)
  {
    retValWrapper = DLT_CTRL_OK;
  }

#endif /* DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431 */

  *ResponseLength = 0U;

  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);

  return retValWrapper;
}

/*-----------------------------[Dlt_GetDefaultTraceStatusWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetDefaultTraceStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValWrapper = DLT_CTRL_ERROR;

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  boolean traceStatus;
#else /* if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
  Dlt_MessageTraceStatusType traceStatus;
#endif /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_GetDefaultTraceStatus,1 */
#if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431)
#if (DLT_ENABLE_BSW_DISTRIBUTION == STD_ON)
  /* !LINKSTO EB_Req_Dlt_GetDefaultTraceStatus.onSlaveCore,1 */
  if(GetCoreID() != DLT_MASTER_CORE)
  {
    traceStatus = DLT_DFT_INVALID_TRACESTATUS;
  }
  else
  {
    traceStatus = Dlt_GetDefaultTraceStatus();
  }
#else
  traceStatus = Dlt_GetDefaultTraceStatus();
#endif /* DLT_ENABLE_BSW_DISTRIBUTION == STD_ON */

  if (traceStatus != DLT_DFT_INVALID_TRACESTATUS)
  {
    RetValWrapper = DLT_CTRL_OK;
    *ResponseLength = 1U;
    /* return the default trace status */
    Response[0] = traceStatus;
  }

#else /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */

  if (Dlt_GetDefaultTraceStatus(&traceStatus) == E_OK)
  {
    RetValWrapper = DLT_CTRL_OK;
    *ResponseLength = 1U;
    /* !LINKSTO Dlt.ASR431.SWS_Dlt_00717, 1 */
    Response[0] = traceStatus;
  }
#endif /* DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431 */
  else
  {
    /* !LINKSTO EB_Req_Dlt_GetDefaultTraceStatus.InvalidRetValue,1 */
    *ResponseLength = 0U;
  }
  TS_PARAM_UNUSED(RequestBuffer);
  TS_PARAM_UNUSED(ReceivedHeaderLength);

  return RetValWrapper;
}

/*-----------------------------[Dlt_SetMessageFilteringWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetMessageFilteringWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType retValWrapper        = DLT_CTRL_ERROR;
  Std_ReturnType     retValFunction_ASR43 = E_NOT_OK;

  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) RequestPayload = &RequestBuffer[ReceivedHeaderLength];

  /* Get data from payload */
  Dlt_FilterMessagesType NewStatus = RequestPayload[DLT_CM_0x0A_NEW_STATUS_PAYLOAD_IDX];
  boolean                status    = RequestPayload[DLT_CM_0x0A_NEW_STATUS_PAYLOAD_IDX];

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_SetMessageFiltering,1 */
#if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431)
  TS_PARAM_UNUSED(status);
  TS_PARAM_UNUSED(retValFunction_ASR43);

  retValWrapper = Dlt_SetMessageFiltering(NewStatus);
#else /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */
  TS_PARAM_UNUSED(NewStatus);
  retValFunction_ASR43 = Dlt_SetMessageFiltering(status);

  if (retValFunction_ASR43 == E_OK)
  {
    /* !LINKSTO Dlt.ASR431.Ref_SWS_Dlt_00775, 1 */
    retValWrapper = DLT_CTRL_OK;
  }
#endif /* DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431 */
  *ResponseLength = 0U;

  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);
  TS_PARAM_UNUSED(ReceivedHeaderLength);

  return retValWrapper;
}

/*-----------------------------[Dlt_GetMessageFilteringStatusWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetMessageFilteringStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue = DLT_CTRL_ERROR;
  Dlt_FilterMessagesType RetMFValue;

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_GetMessageFilteringStatus,1 */
  RetMFValue = Dlt_GetMessageFilteringStatus();

  if (RetMFValue != DLT_INVALID_MESSAGEFILTERINGSTS)
  {
    RetValue = DLT_CTRL_OK;
    *ResponseLength = 1U;
    /* return the message filtering status */
    Response[0] = RetMFValue;
  }
  else
  {
    /* !LINKSTO EB_Req_Dlt_GetMessageFilteringStatus.InvalidRetValue,1 */
    *ResponseLength = 0U;
  }
  /* No request parameters */
  TS_PARAM_UNUSED(RequestBuffer);
  TS_PARAM_UNUSED(ReceivedHeaderLength);
  return RetValue;
}

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
/*-----------------------------[Dlt_ResetToFactoryDefaultWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_ResetToFactoryDefaultWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  /* !LINKSTO Dlt.ControlCommand.ResetToFactoryDefault.RetVal.NOK.Delete.NvM, 1 */
  /* !LINKSTO EB_Req_Dlt_ResetToFactoryDefault.InvalidRetValue, 1 */
  Dlt_CtrlReturnType RetValue = DLT_CTRL_ERROR;

  /* Call the handler */
  /* !LINKSTO Dlt.ControlCommand.ResetToFactoryDefault.Reset.NvM, 1 */
  /* !LINKSTO Dlt.ControlCommand.ResetToFactoryDefault.Reset.NoNvM, 1 */
  /* !LINKSTO EB_Req_Dlt_ResetToFactoryDefault, 1 */
  if (Dlt_ResetToFactoryDefault() == E_OK)
  {
    /* !LINKSTO Dlt.ControlCommand.ResetToFactoryDefault.Delete.NvM, 1 */
    /* !LINKSTO Dlt.ControlCommand.ResetToFactoryDefault.RetVal.OK.Delete.NvM, 1 */
    /* !LINKSTO Dlt.ControlCommand.ResetToFactoryDefault.RetVal.OK.Reset.NoNvM, 1 */
    RetValue = DLT_CTRL_OK;
  }

  *ResponseLength = 0U;

  TS_PARAM_UNUSED(RequestBuffer);
  TS_PARAM_UNUSED(ReceivedHeaderLength);
  TS_PARAM_UNUSED(Response);

  return RetValue;
}
#endif /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */

/*-----------------------------[Dlt_SetUseECUIDWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetUseECUIDWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue;
  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) RequestPayload = &RequestBuffer[ReceivedHeaderLength];

  /* Get data from payload */
  uint8 NewStatus = RequestPayload[DLT_CM_0x0D_NEW_STATUS_PAYLOAD_IDX];

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_SetUseECUID,1 */
  RetValue = Dlt_SetUseECUID(NewStatus);
  *ResponseLength = 0U;

  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);
  TS_PARAM_UNUSED(ReceivedHeaderLength);
  return RetValue;
}

/*-----------------------------[Dlt_GetUseECUIDStatusWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetUseECUIDStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue = DLT_CTRL_ERROR;
  uint8 retUEValue;

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_GetUseECUID,1 */
  retUEValue = Dlt_GetUseECUID();

  if (DLT_CTRL_ERROR == retUEValue)
  {
    *ResponseLength = 0U;
    RetValue = DLT_CTRL_ERROR;
  }
  else
  {
    RetValue = DLT_CTRL_OK;
    *ResponseLength = 1U;
    /* return the ECUID usage status */
    Response[0] = retUEValue;
  }
  /* No request parameters */
  TS_PARAM_UNUSED(RequestBuffer);
  TS_PARAM_UNUSED(ReceivedHeaderLength);
  return RetValue;
}

/*-----------------------------[Dlt_SetUseSessionIDWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetUseSessionIDWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue;
  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) RequestPayload = &RequestBuffer[ReceivedHeaderLength];

  /* Get data from payload */
  uint8 NewStatus = RequestPayload[DLT_CM_0x0E_NEW_STATUS_PAYLOAD_IDX];

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_SetUseSessionID,1 */
  RetValue = Dlt_SetUseSessionID(NewStatus);
  *ResponseLength = 0U;

  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);
  TS_PARAM_UNUSED(ReceivedHeaderLength);
  return RetValue;
}

/*-----------------------------[Dlt_GetUseSessionIDStatusWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetUseSessionIDStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue = DLT_CTRL_ERROR;
  uint8 retUSValue;

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_GetUseSessionID,1 */
  retUSValue = Dlt_GetUseSessionID();


  if (DLT_CTRL_ERROR == retUSValue)
  {
    *ResponseLength = 0U;
    RetValue = DLT_CTRL_ERROR;
  }
  else
  {
    RetValue = DLT_CTRL_OK;
    *ResponseLength = 1U;
    /* return the use EcuId status */
    Response[0] = retUSValue;
  }
  /* No request parameters */
  TS_PARAM_UNUSED(RequestBuffer);
  TS_PARAM_UNUSED(ReceivedHeaderLength);
  return RetValue;
}

#if (DLT_IMPLEMENT_TIMESTAMP != STD_OFF)
/*-----------------------------[Dlt_SetUseTimestampWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetUseTimestampWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue;
  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) RequestPayload = &RequestBuffer[ReceivedHeaderLength];

  /* Get data from payload */
  uint8 NewStatus = RequestPayload[DLT_CM_0x0F_NEW_STATUS_PAYLOAD_IDX];

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_SetUseTimestamp,1 */
  RetValue = Dlt_SetUseTimestamp(NewStatus);
  *ResponseLength = 0U;

  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);
  TS_PARAM_UNUSED(ReceivedHeaderLength);
  return RetValue;
}

/*-----------------------------[Dlt_GetUseTimestampStatusWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetUseTimestampStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue = DLT_CTRL_ERROR;
  uint8 RetUTValue;

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_GetUseTimestamp,1 */
  RetUTValue = Dlt_GetUseTimestamp();


  if (DLT_CTRL_ERROR == RetUTValue)
  {
    *ResponseLength = 0U;
    RetValue = DLT_CTRL_ERROR;
  }
  else
  {
    RetValue = DLT_CTRL_OK;
    *ResponseLength = 1U;
    /* return the use timestamp status */
    Response[0] = RetUTValue;
  }

  /* No request parameters */
  TS_PARAM_UNUSED(RequestBuffer);
  TS_PARAM_UNUSED(ReceivedHeaderLength);
  return RetValue;
}
#endif /* DLT_IMPLEMENT_TIMESTAMP != STD_OFF */

/*-----------------------------[Dlt_SetUseExtendedHeaderWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetUseExtendedHeaderWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue;
  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) RequestPayload = &RequestBuffer[ReceivedHeaderLength];

  /* Get data from payload */
  uint8 NewStatus = RequestPayload[DLT_CM_0x10_NEW_STATUS_PAYLOAD_IDX];

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_SetUseExtendedHeader,1 */
  RetValue = Dlt_SetUseExtendedHeader(NewStatus);
  *ResponseLength = 0U;

  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);
  TS_PARAM_UNUSED(ReceivedHeaderLength);
  return RetValue;
}

/*-----------------------------[Dlt_GetUseExtendedHeaderStatusWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetUseExtendedHeaderStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue = DLT_CTRL_ERROR;
  uint8 RetUEHValue;

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_GetUseExtendedHeader,1 */
  RetUEHValue = Dlt_GetUseExtendedHeader();


  if (DLT_CTRL_ERROR == RetUEHValue)
  {
    *ResponseLength = 0U;
    RetValue = DLT_CTRL_ERROR;
  }
  else
  {
    RetValue = DLT_CTRL_OK;
    *ResponseLength = 1U;
    /* return the use extended header status */
    Response[0] = RetUEHValue;
  }
  /* No request parameters */
  TS_PARAM_UNUSED(RequestBuffer);
  TS_PARAM_UNUSED(ReceivedHeaderLength);
  return RetValue;
}

/* !LINKSTO Dlt.ASR431.Ref_SWS_Dlt_00078, 1 */
/*-----------------------------[Dlt_StoreConfigWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_StoreConfigWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  /* !LINKSTO Dlt.ASR431.Ref_SWS_Dlt_00709_NOK, 1 */
  Dlt_CtrlReturnType RetValueWrapper = DLT_CTRL_ERROR;
  Dlt_ReturnType retValStorePers;
  Std_ReturnType retValStoreConfig;

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_StorePersistent,1 */
  /* !LINKSTO EB_Req_Dlt_StoreConfiguration,1 */
#if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431)
  TS_PARAM_UNUSED(retValStoreConfig);
#if (DLT_IMPLEMENT_NV_RAM_STORAGE == STD_ON)

  retValStorePers = Dlt_StorePersistent(DLT_NO_SESSION_ID);

  if (retValStorePers == DLT_E_OK)
  {
    RetValueWrapper = DLT_CTRL_OK;
  }

#else /* DLT_IMPLEMENT_NV_RAM_STORAGE == STD_OFF */

  TS_PARAM_UNUSED(retValStorePers);

  RetValueWrapper = DLT_CTRL_NOT_SUPPORTED;

#endif /* DLT_IMPLEMENT_NV_RAM_STORAGE == STD_ON */

#else /* DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431 */
  TS_PARAM_UNUSED(retValStorePers);

  /* !LINKSTO Dlt.ASR431.Ref_SWS_Dlt_00709, 1 */
  retValStoreConfig = Dlt_StoreConfiguration();

  if (retValStoreConfig == E_OK)
  {
    /* !LINKSTO Dlt.ASR431.Ref_SWS_Dlt_00709_OK, 1 */
    RetValueWrapper = DLT_CTRL_OK;
  }
  else if (retValStoreConfig == DLT_E_NOT_SUPPORTED)
  {
    /* !LINKSTO Dlt.ASR431.Ref_SWS_Dlt_00710, 1 */
    RetValueWrapper = DLT_CTRL_NOT_SUPPORTED;
  }
  else
  {
    /* RetValueWrapper == DLT_CTRL_ERROR */
  }
#endif /* DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431 */

   *ResponseLength = 0U;

  /* No request parameters */
  TS_PARAM_UNUSED(RequestBuffer);
  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);
  TS_PARAM_UNUSED(ReceivedHeaderLength);

  return RetValueWrapper;
}

#if (DLT_IMPLEMENT_VERBOSE == STD_ON)
/*-----------------------------[Dlt_SetVerboseModeWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_SetVerboseModeWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue;
  P2CONST(uint8, AUTOMATIC, DLT_APPL_DATA) RequestPayload = &RequestBuffer[ReceivedHeaderLength];

  /* Get data from payload */
  uint8 NewStatus = RequestPayload[DLT_CM_0x09_NEW_STATUS_PAYLOAD_IDX];

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  Dlt_ApplicationIDType AppId = {0U};
  Dlt_ContextIDType ContextId = {0U};
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  DLT_MSB_TO_PLATFORM_ENDIANNESS(AppId, RequestBuffer,
    (ReceivedHeaderLength - DLT_HEADER_APPID_LENGTH_STATIC - DLT_HEADER_CTXID_LENGTH_STATIC));
  DLT_MSB_TO_PLATFORM_ENDIANNESS(ContextId, RequestBuffer,
    (ReceivedHeaderLength - DLT_HEADER_CTXID_LENGTH_STATIC));
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
  /* Application Id field are the last 8 bytes of the extended header */
  Dlt_ApplicationIDType AppId =
    DLT_UINT32_FROM_BYTE_ARRAY_MSB(&(RequestBuffer[ReceivedHeaderLength - DLT_HEADER_APPID_LENGTH_STATIC - DLT_HEADER_CTXID_LENGTH_STATIC]));
  /* Context Id field are the last 4 bytes of the extended header */
  Dlt_ContextIDType ContextId =
    DLT_UINT32_FROM_BYTE_ARRAY_MSB(&(RequestBuffer[ReceivedHeaderLength - DLT_HEADER_CTXID_LENGTH_STATIC]));
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_SetVerboseMode,1 */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  RetValue = Dlt_SetVerboseMode(((const Dlt_ApplicationIDType*)(void*)&AppId),
                                ((const Dlt_ContextIDType*)(void*)&ContextId),
                                NewStatus);
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
  RetValue = Dlt_SetVerboseMode(AppId, ContextId, NewStatus);
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)*/

  *ResponseLength = 0U;

  /* No other response for this Control Message, besides the status and the ServiceId */
  TS_PARAM_UNUSED(Response);

  return RetValue;
}

/*-----------------------------[Dlt_GetVerboseModeStatusWrapper]------------------*/
STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetVerboseModeStatusWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue = DLT_CTRL_ERROR;
  uint8 VerboseMode = 0U;

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  Dlt_ApplicationIDType AppId = {0U};
  Dlt_ContextIDType ContextId = {0U};
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  DLT_MSB_TO_PLATFORM_ENDIANNESS(AppId, RequestBuffer,
    (ReceivedHeaderLength - DLT_HEADER_APPID_LENGTH_STATIC - DLT_HEADER_CTXID_LENGTH_STATIC));
  DLT_MSB_TO_PLATFORM_ENDIANNESS(ContextId, RequestBuffer,
    (ReceivedHeaderLength - DLT_HEADER_APPID_LENGTH_STATIC));
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
  /* Context Id field are the last 4 bytes of the extended header */
  Dlt_ApplicationIDType AppId =
    DLT_UINT32_FROM_BYTE_ARRAY_MSB(&(RequestBuffer[ReceivedHeaderLength - DLT_HEADER_APPID_LENGTH_STATIC - DLT_HEADER_CTXID_LENGTH_STATIC]));
  /* Application Id field are the last 8 bytes of the extended header */
  Dlt_ContextIDType ContextId =
    DLT_UINT32_FROM_BYTE_ARRAY_MSB(&(RequestBuffer[ReceivedHeaderLength - DLT_HEADER_CTXID_LENGTH_STATIC]));
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431) */

  /* Call the handler */
  /* !LINKSTO EB_Req_Dlt_GetVerboseModeStatus,1 */
#if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)
  RetValue = Dlt_GetVerboseModeStatus(((const Dlt_ApplicationIDType*)(void*)&AppId),
                                      ((const Dlt_ContextIDType*)(void*)&ContextId),
                                      &VerboseMode);
#else /* #if (DLT_DEFAULT_ASR_RETVAL != AUTOSAR_431) */
  RetValue = Dlt_GetVerboseModeStatus(AppId, ContextId, &VerboseMode);
#endif /* #if (DLT_DEFAULT_ASR_RETVAL == AUTOSAR_431)*/

  *ResponseLength = 1U;

  TS_PARAM_UNUSED(ReceivedHeaderLength);
  /* return the used verbose mode */
  Response[0] = VerboseMode;

  return RetValue;
}
#endif /* #if (DLT_IMPLEMENT_VERBOSE == STD_ON) */

/*-----------------------------[Dlt_GetSoftwareVersionWrapper]------------------*/
/* !LINKSTO PRS_Dlt_00393, 1*/
/* !LINKSTO SWS_Dlt_00393, 1 */
 STATIC FUNC(Dlt_CtrlReturnType, DLT_CODE) Dlt_GetSoftwareVersionWrapper
(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_CONST) RequestBuffer,
  uint8 ReceivedHeaderLength,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) Response,
  P2VAR(Dlt_MsgLengthType, AUTOMATIC, DLT_APPL_DATA) ResponseLength
)
{
  Dlt_CtrlReturnType RetValue = DLT_CTRL_ERROR;
#if (DLT_GETSOFTWAREVERSION_CONTROL_MESSAGE == STD_ON)
  const uint32 swVersionSize = DLT_GETSOFTWAREVERSION_LENGTH;
  Std_ReturnType RetSwVersion = E_NOT_OK;
  /* !LINKSTO Dlt.swdd.DltControlMessage.GetSoftwareVersion.Processing, 1 */
  RetSwVersion = Dlt_GetSoftwareVersion(&Response[sizeof(swVersionSize)], swVersionSize);
  if (E_OK == RetSwVersion)
  {
    RetValue = DLT_CTRL_OK;
    *ResponseLength = sizeof(swVersionSize) + DLT_GETSOFTWAREVERSION_LENGTH;
    DLT_FILL_BUFFER_32_MSB(Response,0U,swVersionSize);
  }
  else
  {
    *ResponseLength = 0U;
  }
#else /* DLT_GETSOFTWAREVERSION_CONTROL_MESSAGE == STD_OFF */

  RetValue = DLT_CTRL_NOT_SUPPORTED;
  *ResponseLength = 0U;
  TS_PARAM_UNUSED(Response);
#endif /* DLT_GETSOFTWAREVERSION_CONTROL_MESSAGE == STD_ON */
  /* No request parameters */
  TS_PARAM_UNUSED(RequestBuffer);
  TS_PARAM_UNUSED(ReceivedHeaderLength);
  return RetValue;
}


#define DLT_STOP_SEC_CODE
#include <Dlt_MemMap.h>

#endif /* DLT_RECEPTION_CONTROL_MESSAGES_ENABLED == STD_ON */
/*==================[end of file]===========================================*/
