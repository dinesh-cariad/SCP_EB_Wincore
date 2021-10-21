#ifndef DCCM_VALIDATION
#define DCCM_VALIDATION

 /***************************************************************************
 *
 * \file Dccm_Validation.h
 *
 * \brief Dccm Validation
 *
 * Copyright 2020 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#include "Dccm_Internal.h"

/*==================[macros]=================================================*/
/**
 *
 * \brief Mask used for subfunction to ignore the suppressPosRspMsgIndicationBit.
 *
 */
#if (defined DCCM_SUBFUNCTION_MASK)
  #error DCCM_SUBFUNCTION_MASK is already defined
#endif
#define DCCM_SUBFUNCTION_MASK 0x7FU

/**
 *
 * \brief Mask used to extract only the suppressPosRspMsgIndicationBit.
 *
 */
#if (defined DCCM_SUPPRESSBIT_MASK)
  #error DCCM_SUPPRESSBIT_MASK is already defined
#endif
#define DCCM_SUPPRESSBIT_MASK 0x80U

/**
 *
 * \brief Mask used to extract the low nibble from a specific parameter.
 *
 */
#if (defined BITS3210_BIT_MASK)
  #error BITS3210_BIT_MASK is already defined
#endif
#define BITS3210_BIT_MASK 0xFU

/**
 *
 * \brief Mask used to shift an parameter with 4 bits,used especially to extract the high nibble.
 *
 */
#if (defined BITS_TO_SHIFT_4)
  #error BITS_TO_SHIFT_4 is already defined
#endif
#define BITS_TO_SHIFT_4 4U

/**
 *
 * \brief Mask used to shift an parameter with 8 bits.
 *
 */
#if (defined BITS_TO_SHIFT_8)
  #error BITS_TO_SHIFT_8 is already defined
#endif
#define BITS_TO_SHIFT_8 8U

/**
 *
 * \brief Macro for ReadScalingDataByIdentifier service which represents the bitMappedReportedWithOutMask encoding
 *        from scalingByte (High Nibble) parameter.
 */
#if (defined DCCM_BIT_MAPPED_REPORTED_WITH_OUT_MASK)
  #error DCCM_BIT_MAPPED_REPORTED_WITH_OUT_MASK is already defined
#endif
#define DCCM_BIT_MAPPED_REPORTED_WITH_OUT_MASK 0x2U

/**
 *
 * \brief Macro for ReadScalingDataByIdentifier service which represents the formula encoding
 *        from scalingByte (High Nibble) parameter.
 *
 */
#if (defined DCCM_SCALING_BYTE_FORMULA)
  #error DCCM_SCALING_BYTE_FORMULA is already defined
#endif
#define DCCM_SCALING_BYTE_FORMULA 0x9U

/**
 *
 * \brief Macro for ReadScalingDataByIdentifier service which represents the unit/format encoding
 *        from scalingByte (High Nibble) parameter.
 *
 */
#if (defined DCCM_SCALING_BYTE_UNIT_FORMAT)
  #error DCCM_SCALING_BYTE_UNIT_FORMAT is already defined
#endif
#define DCCM_SCALING_BYTE_UNIT_FORMAT 0xAU

/**
 *
 * \brief Macro used for FileTransfer service which represents
 *        the modeOfOperation parameter when equals to 0x02 (DeleteFile).
 *
 */
#if (defined DCCM_MODE_OF_OPERATION_DELETE_FILE)
  #error DCCM_MODE_OF_OPERATION_DELETE_FILE is already defined
#endif
#define DCCM_MODE_OF_OPERATION_DELETE_FILE 0x02U

/**
 *
 * \brief Macro used for FileTransfer service which represents
 *        the modeOfOperation parameter when equals to 0x05 (ReadDir).
 *
 */
#if (defined DCCM_MODE_OF_OPERATION_READ_DIR)
  #error DCCM_MODE_OF_OPERATION_READ_DIR is already defined
#endif
#define DCCM_MODE_OF_OPERATION_READ_DIR 0x05U

/**
 *
 * \brief Macro used for AccessTimingParameter service which represents
 *        the sub-function parameter when equals to 0x01 (readExtendedTimingParameterSet).
 *
 */
#if (defined DCCM_READ_EXTEND_TIMING_PARAMETER_SET)
  #error DCCM_READ_EXTEND_TIMING_PARAMETER_SET is already defined
#endif
#define DCCM_READ_EXTEND_TIMING_PARAMETER_SET 0x01U

/**
 *
 * \brief Macro used for AccessTimingParameter service which represents
 *        the sub-function parameter when equals to 0x03 (readCurrentlyActiveTimingParameters).
 *
 */
#if (defined DCCM_READ_CURRENTLY_ACTIVE_TIMING_PARAMETERS)
  #error DCCM_READ_CURRENTLY_ACTIVE_TIMING_PARAMETERS is already defined
#endif
#define DCCM_READ_CURRENTLY_ACTIVE_TIMING_PARAMETERS 0x03U

/**
 *
 * \brief Macro used for ReadDTCInformation service which represents
 *        the maximum value for DTCExtDataRecordNumber parameter for 0x06 and 0x10 subfunctions.
 *
 */
#if (defined DCCM_MAX_DTC_EXT_DATA_RECORD_NR_6_10)
  #error DCCM_MAX_DTC_EXT_DATA_RECORD_NR_6_10 is already defined
#endif
#define DCCM_MAX_DTC_EXT_DATA_RECORD_NR_6_10 0xFDU

/**
 *
 * \brief Macro used for ReadDTCInformation service which represents
 *        the maximum value for DTCExtDataRecordNumber parameter for 0x16 subfunction.
 *
 */
#if (defined DCCM_MAX_DTC_EXT_DATA_RECORD_NR_16)
  #error DCCM_MAX_DTC_EXT_DATA_RECORD_NR_16 is already defined
#endif
#define DCCM_MAX_DTC_EXT_DATA_RECORD_NR_16 0xEFU

/**
 *
 * \brief Macro used for ReadDTCInformation service which represents
 *        the maximum value for DTCExtDataRecordNumber parameter for 0x19 subfunction.
 *
 */
#if (defined DCCM_MAX_DTC_EXT_DATA_RECORD_NR_19)
  #error DCCM_MAX_DTC_EXT_DATA_RECORD_NR_19 is already defined
#endif
#define DCCM_MAX_DTC_EXT_DATA_RECORD_NR_19 0xFEU

/**
 *
 * \brief Macro used for DynamicallyDefineDataIdentifier service which represents
 *        the sub-function parameter when equals to 0x01 (defineByIdentifier).
 *
 */
#if (defined DCCM_DEFINE_BY_IDENTIFYER)
  #error DCCM_DEFINE_BY_IDENTIFYER is already defined
#endif
#define DCCM_DEFINE_BY_IDENTIFYER 0x01U

/**
 *
 * \brief Macro used for DynamicallyDefineDataIdentifier service which represents
 *        the sub-function parameter when equals to 0x02 (defineByMemoryAddress).
 *
 */
#if (defined DCCM_DEFINE_BY_MEMORY_ADDRESS)
  #error DCCM_DEFINE_BY_MEMORY_ADDRESS is already defined
#endif
#define DCCM_DEFINE_BY_MEMORY_ADDRESS 0x02U

/**
 *
 * \brief Macro used for DynamicallyDefineDataIdentifier service which represents
 *        the sub-function parameter when equals to 0x03 (clearDynamicallyDefinedDataIdentifier).
 *
 */
#if (defined DCCM_CLEAR_DYNAMICALLY_DEFINED_DATA_IDENTIFIER)
  #error DCCM_CLEAR_DYNAMICALLY_DEFINED_DATA_IDENTIFIER is already defined
#endif
#define DCCM_CLEAR_DYNAMICALLY_DEFINED_DATA_IDENTIFIER 0x03U

/**
 *
 * \brief Macro which represents the sub-function parameter when equals to 0x00.
 *
 */
#if (defined DCCM_ZERO_SUBFUNCTION)
  #error DCCM_ZERO_SUBFUNCTION is already defined
#endif
#define DCCM_ZERO_SUBFUNCTION 0x00U

/**
 *
 * \brief Macro used for EcuReset service which represents
 *        the sub-function parameter when equals to 0x04 (enableRapidPowerShutDown).
 *
 */
#if (defined DCCM_SERVICE_ECU_RESET_SUBFUNCTION_ENABLE_RAPID_POWER_SHUTDOWN)
  #error DCCM_SERVICE_ECU_RESET_SUBFUNCTION_ENABLE_RAPID_POWER_SHUTDOWN is already defined
#endif
#define DCCM_SERVICE_ECU_RESET_SUBFUNCTION_ENABLE_RAPID_POWER_SHUTDOWN 0x04U

/**
 *
 * \brief Macro used for ReadDTCInformation service which represents
 *        the DTCFormatIdentifier parameter when equals to 0x2 (SAE_J1939-73_DTCFormat).
 *
 */
#if (defined DCCM_DTCFORMAT_2)
  #error DCCM_DTCFORMAT_2 is already defined
#endif
#define DCCM_DTCFORMAT_2 0x2U

/**
 *
 * \brief Macro used for ReadDTCInformation service which represents
 *        the DTCFormatIdentifier parameter when equals to 0x4 (SAE_J2012-DA_DTCFormat_04).
 *
 */
#if (defined DCCM_DTCFORMAT_4)
  #error DCCM_DTCFORMAT_4 is already defined
#endif
#define DCCM_DTCFORMAT_4 0x4U

/**
 *
 * \brief Macro representing the length of 0.
 *
 */
#if (defined DCCM_LENGTH_0)
  #error DCCM_LENGTH_0 is already defined
#endif
#define DCCM_LENGTH_0 0U

/**
 *
 * \brief Macro representing the length of 1.
 *
 */
#if (defined DCCM_LENGTH_1)
  #error DCCM_LENGTH_1 is already defined
#endif
#define DCCM_LENGTH_1 1U

/**
 *
 * \brief Macro representing the length of 2.
 *
 */
#if (defined DCCM_LENGTH_2)
  #error DCCM_LENGTH_2 is already defined
#endif
#define DCCM_LENGTH_2 2U

/**
 *
 * \brief Macro representing the length of 3.
 *
 */
#if (defined DCCM_LENGTH_3)
  #error DCCM_LENGTH_3 is already defined
#endif
#define DCCM_LENGTH_3 3U

/**
 *
 * \brief Macro representing the length of 4.
 *
 */
#if (defined DCCM_LENGTH_4)
  #error DCCM_LENGTH_4 is already defined
#endif
#define DCCM_LENGTH_4 4U

/**
 *
 * \brief Macro representing the length of 5.
 *
 */
#if (defined DCCM_LENGTH_5)
  #error DCCM_LENGTH_5 is already defined
#endif
#define DCCM_LENGTH_5 5U

/**
 *
 * \brief Macro representing the length of 6.
 *
 */
#if (defined DCCM_LENGTH_6)
  #error DCCM_LENGTH_6 is already defined
#endif
#define DCCM_LENGTH_6 6U

/**
 *
 * \brief Macro representing the length of 7.
 *
 */
#if (defined DCCM_LENGTH_7)
  #error DCCM_LENGTH_7 is already defined
#endif
#define DCCM_LENGTH_7 7U

/**
 *
 * \brief Macro representing the length of 8.
 *
 */
#if (defined DCCM_LENGTH_8)
  #error DCCM_LENGTH_8 is already defined
#endif
#define DCCM_LENGTH_8 8U

/* Request SID for each service */

/**
 *
 * \brief Service Id for diagnostic session control request.
 *
 */
#if (defined SID_DIAGNOSTIC_SESSION_CONTROL)
  #error SID_DIAGNOSTIC_SESSION_CONTROL is already defined
#endif
#define SID_DIAGNOSTIC_SESSION_CONTROL           0x10U

/**
 *
 * \brief Service Id for ecu reset request.
 *
 */
#if (defined SID_ECU_RESET)
  #error SID_ECU_RESET is already defined
#endif
#define SID_ECU_RESET                            0x11U

/**
 *
 * \brief Service Id for security acccess request.
 *
 */
#if (defined SID_SECURITY_ACCESS)
  #error SID_SECURITY_ACCESS is already defined
#endif
#define SID_SECURITY_ACCESS                      0x27U

/**
 *
 * \brief Service Id for communication control request.
 *
 */
#if (defined SID_COMMUNICATION_CONTROL)
  #error SID_COMMUNICATION_CONTROL is already defined
#endif
#define SID_COMMUNICATION_CONTROL                0x28U

 /**
 *
 * \brief Service Id for tester present request.
 *
 */
#if (defined SID_TESTER_PRESENT)
  #error SID_TESTER_PRESENT is already defined
#endif
#define SID_TESTER_PRESENT                       0x3EU

/**
 *
 * \brief Service Id for access timining parameter request.
 *
 */
#if (defined SID_ACCESS_TIMING_PARAMETER)
  #error SID_ACCESS_TIMING_PARAMETER is already defined
#endif
#define SID_ACCESS_TIMING_PARAMETER              0x83U

/**
 *
 * \brief Service Id for secured data transmission request.
 *
 */
#if (defined SID_SECURED_DATA_TRANSMISSION)
  #error SID_SECURED_DATA_TRANSMISSION is already defined
#endif
#define SID_SECURED_DATA_TRANSMISSION            0x84U

/**
 *
 * \brief Service Id for control dtc setting request.
 *
 */
#if (defined SID_CONTROL_DTC_SETTING)
  #error SID_CONTROL_DTC_SETTING is already defined
#endif
#define SID_CONTROL_DTC_SETTING                  0x85U

/**
 *
 * \brief Service Id for response on event request.
 *
 */
#if (defined SID_RESPONSE_ON_EVENT)
  #error SID_RESPONSE_ON_EVENT is already defined
#endif
#define SID_RESPONSE_ON_EVENT                    0x86U

/**
 *
 * \brief Service Id for link control request.
 *
 */
#if (defined SID_LINK_CONTROL)
  #error SID_LINK_CONTROL is already defined
#endif
#define SID_LINK_CONTROL                         0x87U

/**
 *
 * \brief Service Id for read data by identifier request.
 *
 */
#if (defined SID_READ_DATA_BY_IDENTIFIER)
  #error SID_READ_DATA_BY_IDENTIFIER is already defined
#endif
#define SID_READ_DATA_BY_IDENTIFIER              0x22U

/**
 *
 * \brief Service Id for read memory by address request.
 *
 */
#if (defined SID_READ_MEMORY_BY_ADDRESS)
  #error SID_READ_MEMORY_BY_ADDRESS is already defined
#endif
#define SID_READ_MEMORY_BY_ADDRESS               0x23U

/**
 *
 * \brief Service Id for read scaling data by identifier request.
 *
 */
#if (defined SID_READ_SCALING_DATA_BY_IDENTIFIER)
  #error SID_READ_SCALING_DATA_BY_IDENTIFIER is already defined
#endif
#define SID_READ_SCALING_DATA_BY_IDENTIFIER      0x24U

/**
 *
 * \brief Service Id for read data by periodic identifier request.
 *
 */
#if (defined SID_READ_DATA_BY_PERIODIC_IDENTIFIER)
  #error SID_READ_DATA_BY_PERIODIC_IDENTIFIER is already defined
#endif
#define SID_READ_DATA_BY_PERIODIC_IDENTIFIER     0x2AU

/**
 *
 * \brief Service Id for dynamically define data identifier request.
 *
 */
#if (defined SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER)
  #error SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER is already defined
#endif
#define SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER   0x2CU

/**
 *
 * \brief Service Id for write data by identifier request.
 *
 */
#if (defined SID_WRITE_DATA_BY_IDENTIFIER)
  #error SID_WRITE_DATA_BY_IDENTIFIER is already defined
#endif
#define SID_WRITE_DATA_BY_IDENTIFIER             0x2EU

/**
 *
 * \brief Service Id for write memory by address request.
 *
 */
#if (defined SID_WRITE_MEMORY_BY_ADDRESS)
  #error SID_WRITE_MEMORY_BY_ADDRESS is already defined
#endif
#define SID_WRITE_MEMORY_BY_ADDRESS              0x3DU

/**
 *
 * \brief Service Id for clear diagnostic information request.
 *
 */
#if (defined SID_CLEAR_DIAGNOSTIC_INFORMATION)
  #error SID_CLEAR_DIAGNOSTIC_INFORMATION is already defined
#endif
#define SID_CLEAR_DIAGNOSTIC_INFORMATION         0x14U

/**
 *
 * \brief Service Id for read dtc information request.
 *
 */
#if (defined SID_READ_DTC_INFORMATION)
  #error SID_READ_DTC_INFORMATION is already defined
#endif
#define SID_READ_DTC_INFORMATION                 0x19U

/**
 *
 * \brief Service Id for input output control by identidier request.
 *
 */
#if (defined SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER)
  #error SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER is already defined
#endif
#define SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER   0x2FU

/**
 *
 * \brief Service Id for routine control request.
 *
 */
#if (defined SID_ROUTINE_CONTROL)
  #error SID_ROUTINE_CONTROL is already defined
#endif
#define SID_ROUTINE_CONTROL                      0x31U

/**
 *
 * \brief Service Id for request download request.
 *
 */
#if (defined SID_REQUEST_DOWNLOAD)
  #error SID_REQUEST_DOWNLOAD is already defined
#endif
#define SID_REQUEST_DOWNLOAD                     0x34U

/**
 *
 * \brief Service Id for request upload request.
 *
 */
#if (defined SID_REQUEST_UPLOAD)
  #error SID_REQUEST_UPLOAD is already defined
#endif
#define SID_REQUEST_UPLOAD                       0x35U

/**
 *
 * \brief Service Id for transfer data request.
 *
 */
#if (defined SID_TRANSFER_DATA)
  #error SID_TRANSFER_DATA is already defined
#endif
#define SID_TRANSFER_DATA                        0x36U

/**
 *
 * \brief Service Id for transfer exit request.
 *
 */
#if (defined SID_REQUEST_TRANSFER_EXIT)
  #error SID_REQUEST_TRANSFER_EXIT is already defined
#endif
#define SID_REQUEST_TRANSFER_EXIT                0x37U

/**
 *
 * \brief Service Id for file transfer request.
 *
 */
#if (defined SID_FILE_TRANSFER)
  #error SID_FILE_TRANSFER is already defined
#endif
#define SID_FILE_TRANSFER                        0x38U

/**
 *
 * \brief Service Id for an negative response message.
 *
 */
#if (defined NEGATIVE_RESPONSE_SID)
  #error NEGATIVE_RESPONSE_SID is already defined
#endif
#define NEGATIVE_RESPONSE_SID                    0x7FU

/* Response SID for each service */

/**
 *
 * \brief Service Id for diagnostic session response.
 *
 */
#if (defined SID_DIAGNOSTIC_SESSION_CONTROL_RSP)
  #error SID_DIAGNOSTIC_SESSION_CONTROL_RSP is already defined
#endif
#define SID_DIAGNOSTIC_SESSION_CONTROL_RSP           0x50U
/**
 *
 * \brief Service Id for ecu reset response.
 *
 */
#if (defined SID_ECU_RESET_RSP)
  #error SID_ECU_RESET_RSP is already defined
#endif
#define SID_ECU_RESET_RSP                            0x51U

/**
 *
 * \brief Service Id for security acccess response.
 *
 */
#if (defined SID_SECURITY_ACCESS_RSP)
  #error SID_SECURITY_ACCESS_RSP is already defined
#endif
#define SID_SECURITY_ACCESS_RSP                      0x67U

/**
 *
 * \brief Service Id for communication control response.
 *
 */
#if (defined SID_COMMUNICATION_CONTROL_RSP)
  #error SID_COMMUNICATION_CONTROL_RSP is already defined
#endif
#define SID_COMMUNICATION_CONTROL_RSP                0x68U

/**
 *
 * \brief Service Id for tester present response.
 *
 */
#if (defined SID_TESTER_PRESENT_RSP)
  #error SID_TESTER_PRESENT_RSP is already defined
#endif
#define SID_TESTER_PRESENT_RSP                       0x7EU

/**
 *
 * \brief Service Id for access timining response.
 *
 */
#if (defined SID_ACCESS_TIMING_PARAMETER_RSP)
  #error SID_ACCESS_TIMING_PARAMETER_RSP is already defined
#endif
#define SID_ACCESS_TIMING_PARAMETER_RSP              0xC3U

/**
 *
 * \brief Service Id for secured data transmission response.
 *
 */
#if (defined SID_SECURED_DATA_TRANSMISSION_RSP)
  #error SID_SECURED_DATA_TRANSMISSION_RSP is already defined
#endif
#define SID_SECURED_DATA_TRANSMISSION_RSP            0xC4U

/**
 *
 * \brief Service Id for control dtc setting response.
 *
 */
#if (defined SID_CONTROL_DTC_SETTING_RSP)
  #error SID_CONTROL_DTC_SETTING_RSP is already defined
#endif
#define SID_CONTROL_DTC_SETTING_RSP                  0xC5U

/**
 *
 * \brief Service Id for response on event response.
 *
 */
#if (defined SID_RESPONSE_ON_EVENT_RSP)
  #error SID_RESPONSE_ON_EVENT_RSP is already defined
#endif
#define SID_RESPONSE_ON_EVENT_RSP                    0xC6U

/**
 *
 * \brief Service Id for link control response.
 *
 */
#if (defined SID_LINK_CONTROL_RSP)
  #error SID_LINK_CONTROL_RSP is already defined
#endif
#define SID_LINK_CONTROL_RSP                         0xC7U

/**
 *
 * \brief Service Id for read data by identidier response.
 *
 */
#if (defined SID_READ_DATA_BY_IDENTIFIER_RSP)
  #error SID_READ_DATA_BY_IDENTIFIER_RSP is already defined
#endif
#define SID_READ_DATA_BY_IDENTIFIER_RSP              0x62U

/**
 *
 * \brief Service Id for read memory by address response.
 *
 */
#if (defined SID_READ_MEMORY_BY_ADDRESS_RSP)
  #error SID_READ_MEMORY_BY_ADDRESS_RSP is already defined
#endif
#define SID_READ_MEMORY_BY_ADDRESS_RSP               0x63U

/**
 *
 * \brief Service Id for read scaling data by identidier response.
 *
 */
#if (defined SID_READ_SCALING_DATA_BY_IDENTIFIER_RSP)
  #error SID_READ_SCALING_DATA_BY_IDENTIFIER_RSP is already defined
#endif
#define SID_READ_SCALING_DATA_BY_IDENTIFIER_RSP      0x64U

/**
 *
 * \brief Service Id for read data by periodic identidier response.
 *
 */
#if (defined SID_READ_DATA_BY_PERIODIC_IDENTIFIER_RSP)
  #error SID_READ_DATA_BY_PERIODIC_IDENTIFIER_RSP is already defined
#endif
#define SID_READ_DATA_BY_PERIODIC_IDENTIFIER_RSP     0x6AU

/**
 *
 * \brief Service Id for dynamically define data identidier response.
 *
 */
#if (defined SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER_RSP)
  #error SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER_RSP is already defined
#endif
#define SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER_RSP   0x6CU

/**
 *
 * \brief Service Id for write data by identidier response.
 *
 */
#if (defined SID_WRITE_DATA_BY_IDENTIFIER_RSP)
  #error SID_WRITE_DATA_BY_IDENTIFIER_RSP is already defined
#endif
#define SID_WRITE_DATA_BY_IDENTIFIER_RSP             0x6EU

/**
 *
 * \brief Service Id for write memory by address response.
 *
 */
#if (defined SID_WRITE_MEMORY_BY_ADDRESS_RSP)
  #error SID_WRITE_MEMORY_BY_ADDRESS_RSP is already defined
#endif
#define SID_WRITE_MEMORY_BY_ADDRESS_RSP              0x7DU

/**
 *
 * \brief Service Id for clear diagnostic information response.
 *
 */
#if (defined SID_CLEAR_DIAGNOSTIC_INFORMATION_RSP)
  #error SID_CLEAR_DIAGNOSTIC_INFORMATION_RSP is already defined
#endif
#define SID_CLEAR_DIAGNOSTIC_INFORMATION_RSP         0x54U

/**
 *
 * \brief Service Id for read dtc information response.
 *
 */
#if (defined SID_READ_DTC_INFORMATION_RSP)
  #error SID_READ_DTC_INFORMATION_RSP is already defined
#endif
#define SID_READ_DTC_INFORMATION_RSP                 0x59U

/**
 *
 * \brief Service Id for input output control by identidier response.
 *
 */
#if (defined SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER_RSP)
  #error SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER_RSP is already defined
#endif
#define SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER_RSP   0x6FU

/**
 *
 * \brief Service Id for routine control response.
 *
 */
#if (defined SID_ROUTINE_CONTROL_RSP)
  #error SID_ROUTINE_CONTROL_RSP is already defined
#endif
#define SID_ROUTINE_CONTROL_RSP                      0x71U

/**
 *
 * \brief Service Id for request download response.
 *
 */
#if (defined SID_REQUEST_DOWNLOAD_RSP)
  #error SID_REQUEST_DOWNLOAD_RSP is already defined
#endif
#define SID_REQUEST_DOWNLOAD_RSP                     0x74U

/**
 *
 * \brief Service Id for request upload response.
 *
 */
#if (defined SID_REQUEST_UPLOAD_RSP)
  #error SID_REQUEST_UPLOAD_RSP is already defined
#endif
#define SID_REQUEST_UPLOAD_RSP                       0x75U

/**
 *
 * \brief Service Id for transfer data response.
 *
 */
#if (defined SID_TRANSFER_DATA_RSP)
  #error SID_TRANSFER_DATA_RSP is already defined
#endif
#define SID_TRANSFER_DATA_RSP                        0x76U

/**
 *
 * \brief Service Id for request transfer exit response.
 *
 */
#if (defined SID_REQUEST_TRANSFER_EXIT_RSP)
  #error SID_REQUEST_TRANSFER_EXIT_RSP is already defined
#endif
#define SID_REQUEST_TRANSFER_EXIT_RSP                0x77U

/**
 *
 * \brief Service Id for file transfer response.
 *
 */
#if (defined SID_FILE_TRANSFER_RSP)
  #error SID_FILE_TRANSFER_RSP is already defined
#endif
#define SID_FILE_TRANSFER_RSP                        0x78U

/**
 *
 * \brief Maximum number of services
 *
 */
#if (defined MAX_NO_OF_SERVICES)
  #error MAX_NO_OF_SERVICES is already defined
#endif
#define MAX_NO_OF_SERVICES         26U

/**
 *
 * \brief The length of a negative response
 *
 */

#if (defined NEGATIVE_RESPONSE_LENGTH)
  #error NEGATIVE_RESPONSE_LENGTH is already defined
#endif
#define NEGATIVE_RESPONSE_LENGTH   3U

/* response codes */
/**
 *
 * \brief Response code when the response length is wrong.
 *
 */
#if (defined DCCM_E_INVALID_RESPONSE_LENGTH)
  #error DCCM_E_INVALID_RESPONSE_LENGTH is already defined
#endif
#define DCCM_E_INVALID_RESPONSE_LENGTH 0x02U

/**
 *
 * \brief Response code when the response format is wrong.
 *
 */
#if (defined DCCM_E_INVALID_RESPONSE_FORMAT)
  #error DCCM_E_INVALID_RESPONSE_FORMAT is already defined
#endif
#define DCCM_E_INVALID_RESPONSE_FORMAT 0x03U

/*==================[external function declarations]=========================*/

#define DCCM_START_SEC_CODE
#include <MemMap.h>

/** \brief This function validates a response based on request.
 **
 ** The function is used to validate a response based on the request
 ** in terms of length and format.
 **
 ** \param[in] RequestBuffer The request buffer used for validation.
 **
 ** \param[in] RequestDataLength The length of the request.
 **
 ** \param[in] ResponseBuffer The response buffer to be validated.
 **
 ** \param[in] ResponseDataLength The length of the response.
 **
 ** \return Std_ReturnType
 ** \retval E_OK The response buffer is correct.
 ** \retval E_NOT_OK The parameters that the function was called up are invalid or
 **                   the minimum length of request required to perform the checks is not met.
 **\retval DCCM_E_INVALID_RESPONSE_LENGTH The positive response may have a fixed length or may be a changeable length.
 **                                       If the length varies the function will only check the minimum length.
 **                                       For negative response length should be 3 bytes. If the length does not meet the
 **                                       requirements stated above, this error will be returned.
 **\retval DCCM_E_INVALID_RESPONSE_FORMAT The SID from the positive response does not match the SID that should follow the request;
 **                                       the 2nd byte of the negative response is not the SID in the request;
 **                                       if the service has a DID, sub-function or a byte that must be echo, those that come in response do not match those in the request.
 **/
extern FUNC(Std_ReturnType, DCCM_CODE) Dccm_ValidateRespBasedOnRequest
(
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) RequestBuffer,
  uint32 RequestDataLength,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) ResponseBuffer,
  uint32 ResponseDataLength
);

#define DCCM_STOP_SEC_CODE
#include <MemMap.h>

#endif
