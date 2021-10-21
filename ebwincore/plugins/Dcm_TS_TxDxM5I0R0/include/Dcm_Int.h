/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DCM_INT_H
#define DCM_INT_H

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 19.10 (required)
 *     Parameter instance shall be enclosed in parentheses.
 *
 *     Reason:
 *     The parameter is used in function parameter declarations, definitions
 *     or as structure members and enclosing it in parentheses results
 *     in compiler syntax error.
 *
 *  MISRA-2) Deviated Rule: 18.4 (required)
 *     Unions shall not be used.
 *
 *     Reason:
 *     Union is used to implement variant records and each variant shares common field and
 *     additional fields that are specific to the variant. This union is used and controlled
 *     internally only.
 *     By using union instead of structure, memory usage is reduced.
 *
 *  MISRA-3) Deviated Rule: 8.9 (required)
 *     An identifier with external linkage shall have exactly one external definition.
 *
 *     Reason:
 *     This include file is a central include file containing declarations and definitions
 *     used by all software units.
 *     During unit testing, the untested software units of the Dcm are stubbed. These stubs do
 *     provide the definition for some of the global data structures.
 *     By deviating from this rule it is possible to detect unspecified communication in legacy
 *     code using global variables. This is a temporary deviation.
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 20.7 (required)
  *     "Expressions resulting from the expansion of macro parameters shall
  *     be enclosed in parentheses."
  *
  *     Reason:
  *     The parameter is used in function parameter declarations, definitions
  *     or as structure members and enclosing it in parentheses results
  *     in compiler syntax error.
  *
  *  MISRAC2012-2) Deviated Rule: 8.6 (required)
  *     "An identifier with external linkage shall have exactly one external definition."
  *
  *     Reason:
  *     This include file is a central include file containing declarations and definitions
  *     used by all software units.
  *     During unit testing, the untested software units of the Dcm are stubbed. These stubs do
  *     provide the definition for some of the global data structures.
  *     By deviating from this rule it is possible to detect unspecified communication in legacy
  *     code using global variables. This is a temporary deviation.
  *
  *  MISRAC2012-3) Deviated Rule: 19.2 (required)
  *     "The union keyword should not be used."
  *
  *     Reason:
  *     Union is used to implement variant records and each variant shares common field and
  *     additional fields that are specific to the variant. This union is used and controlled
  *     internally only.
  *     By using union instead of structure, memory usage is reduced.
  *
  */

/*==================[inclusions]=================================================================*/

#include <Dcm.h>
#include <Dcm_Dsl_CommunicationServices.h>
#include <Dcm_Dsl_CommunicationServices_RxConnections.h>
#include <Dcm_Dsl_CommunicationServices_TxConnections.h>
#include <Dcm_Dsl_CommunicationServices_BufferManager.h>

#include <Dcm_Dsl_Supervisor.h>

#include <Dcm_Cfg.h>                                            /* Dcm configuration header file */

#include <ComStack_Types.h>                                            /* AUTOSAR standard types */
#if (DCM_NVM_USAGE_REQUIRED == STD_ON)
#include <NvM.h>
#endif /* if (DCM_NVM_USAGE_REQUIRED == STD_ON) */

#include <Dcm_DefProg_Cfg.h>


/*==================[macros]=====================================================================*/

#if (defined DCM_E_RESOURCE_LOCKED)
#error DCM_E_RESOURCE_LOCKED already defined
#endif /* if (defined DCM_E_RESOURCE_LOCKED) */

/* !LINKSTO Dcm.EB.Std_ReturnTypeExtended.DCM_E_RESOURCE_LOCKED,1 */
/* !LINKSTO Dcm.Dsn.Types.Std_ReturnType,1 */
#define DCM_E_RESOURCE_LOCKED 0x1FU

#ifndef DCM_DEFAULT_SESSION

/** \brief  Definition of default session if same name is not configured in the DcmDspSessionRow.
 **
 ** If the same name is not configured this macro will be used for checking for default session in
 ** static code. */
#define DCM_DEFAULT_SESSION 1U
#endif /* ifndef DCM_DEFAULT_SESSION */

#if (defined DCM_SID_TESTER_PRESENT)
#error DCM_SID_TESTER_PRESENT already defined
#endif /* if (defined DCM_SID_TESTER_PRESENT) */

/** \brief Definition of service ID of diagnostic service 'Tester Present' */
#define DCM_SID_TESTER_PRESENT 0x3EU

#if (defined DCM_TESTER_PRESENT_REQ_LEN)
#error DCM_TESTER_PRESENT_REQ_LEN already defined
#endif /* if (defined DCM_TESTER_PRESENT_REQ_LEN) */

/** \brief Length of diagnostic service 'Tester Present' request */
#define DCM_TESTER_PRESENT_REQ_LEN 0x02U

#if (defined DCM_SID_SESSION_CONTROL)
#error DCM_SID_SESSION_CONTROL already defined
#endif /* if (defined DCM_SID_SESSION_CONTROL) */

/** \brief Definition of service ID of diagnostic service 'Diagnostic Session Control' */
#define DCM_SID_SESSION_CONTROL (0x10U)

#if ((DCM_DSP_USE_SERVICE_0X04 == STD_ON) || (DCM_DSP_USE_SERVICE_0X09 == STD_ON))
#if (defined DCM_SID_CLEAR_DTC)
#error DCM_SID_CLEAR_DTC already defined
#endif /* if (defined DCM_SID_CLEAR_DTC) */

/** \brief Definition of service ID of
 ** diagnostic service 'Clear/Reset emission-related diagnostic information' */
#define DCM_SID_CLEAR_DTC (0x04U)

#if (defined DCM_SID_REQUEST_VEHICLE_INFORMATION)
#error DCM_SID_REQUEST_VEHICLE_INFORMATION already defined
#endif /* if (defined DCM_SID_REQUEST_VEHICLE_INFORMATION) */

/** \brief Definition of service ID of
 ** diagnostic service 'Request vehicle information'
 */
#define DCM_SID_REQUEST_VEHICLE_INFORMATION 0x09U
#endif /* ((DCM_DSP_USE_SERVICE_0X04 == STD_ON) || (DCM_DSP_USE_SERVICE_0X09 == STD_ON))) */

/** \brief Definition of service ID of diagnostic service 'Security Access' */
#if (defined DCM_SID_SECURITYACCESS)
  #error DCM_SID_SECURITYACCESS is already defined
#endif
#define DCM_SID_SECURITYACCESS        (0x27U)

/** \brief Definition of service ID of diagnostic service 'Communication Control' */
#if (defined DCM_SID_COMMUNICATIONCONTROL)
  #error DCM_SID_COMMUNICATIONCONTROL is already defined
#endif
#define DCM_SID_COMMUNICATIONCONTROL        (0x28U)

#if (defined DCM_SID_INVALID)
#error DCM_SID_INVALID already defined
#endif /* if (defined DCM_SID_INVALID) */

/** \brief Invalid value for service ID */
#define DCM_SID_INVALID 0x00U

#if (defined DCM_ZERO_SUBFUNCTION)
#error DCM_ZERO_SUBFUNCTION already defined
#endif /* if (defined DCM_ZERO_SUBFUNCTION) */

/** \brief Definition of a subfunction equal to 0x00 */
#define DCM_ZERO_SUBFUNCTION 0x00U

#if (defined DCM_SUPPOSRES_BITMASK)
#error DCM_SUPPOSRES_BITMASK already defined
#endif /* if (defined DCM_SUPPOSRES_BITMASK) */

/** \brief Mask to check 'suppress positive response bit' is set in dianostic request */
#define DCM_SUPPOSRES_BITMASK 0x80U

#if (defined DCM_CLEAR_SUPPOSRES_BIT)
#error DCM_CLEAR_SUPPOSRES_BIT already defined
#endif /* if (defined DCM_CLEAR_SUPPOSRES_BIT) */

/** \brief Definition of mask removing the 'positive response bit' */
#define DCM_CLEAR_SUPPOSRES_BIT 0x7FU

#if (defined DCM_SID_RESPONSE_BIT)
#error DCM_SID_RESPONSE_BIT already defined
#endif /* if (defined DCM_SID_RESPONSE_BIT) */

/** \brief Definition of bit mask of a response ID */
#define DCM_SID_RESPONSE_BIT 0x40U

#if (defined DCM_DIAG_REQ_SID_IND)
#error DCM_DIAG_REQ_SID_IND already defined
#endif /* if (defined DCM_DIAG_REQ_SID_IND) */

/** \brief Index corresponding to service id in diagnostic request */
#define DCM_DIAG_REQ_SID_IND 0x00U

#if (defined DCM_DIAG_REQ_SUBFUNC_IND)
#error DCM_DIAG_REQ_SUBFUNC_IND already defined
#endif /* if (defined DCM_DIAG_REQ_SUBFUNC_IND) */

/** \brief Index corresponding to service subfunction in diagnostic request */
#define DCM_DIAG_REQ_SUBFUNC_IND 0x01U

/** \brief Definition of DCM_INSTANCE_ID */
#if (defined DCM_INSTANCE_ID)
  #error "DCM_INSTANCE_ID is already defined"
#endif
#define DCM_INSTANCE_ID                    0x00U

/** \brief Error code for interface timeout */
#if (defined DCM_E_INTERFACE_TIMEOUT)
  #error "DCM_E_INTERFACE_TIMEOUT is already defined"
#endif
#define DCM_E_INTERFACE_TIMEOUT            0x01U

/** \brief Error code for interface value out of range */
#if (defined DCM_E_INTERFACE_RETURN_VALUE)
  #error "DCM_E_INTERFACE_RETURN_VALUE is already defined"
#endif
#define DCM_E_INTERFACE_RETURN_VALUE       0x02U

/** \brief Error code for interface buffer over flow */
#if (defined DCM_E_INTERFACE_BUFFER_OVERFLOW)
  #error "DCM_E_INTERFACE_BUFFER_OVERFLOW is already defined"
#endif
#define DCM_E_INTERFACE_BUFFER_OVERFLOW    0x03U

/** \brief Error code for module uninitialization */
#if (defined DCM_E_UNINIT)
  #error "DCM_E_UNINIT is already defined"
#endif
#define DCM_E_UNINIT                       0x05U

/** \brief Error code for invalid input parameter */
#if (defined DCM_E_PARAM)
  #error "DCM_E_PARAM is already defined"
#endif
#define DCM_E_PARAM                        0x06U

/** \brief Error code for DCM API service invoked with NULL POINTER as parameter */
#if (defined DCM_E_PARAM_POINTER)
  #error "DCM_E_PARAM_POINTER is already defined"
#endif
#define DCM_E_PARAM_POINTER                0x07U

/** \brief Error code for NvM Read operation failure */
#if (defined DCM_E_NVM_READ_FAIL)
  #error "DCM_E_NVM_READ_FAIL is already defined"
#endif
#define DCM_E_NVM_READ_FAIL                0x08U

/** \brief Error code for NvM Write operation failure */
#if (defined DCM_E_NVM_WRITE_FAIL)
  #error "DCM_E_NVM_WRITE_FAIL is already defined"
#endif
#define DCM_E_NVM_WRITE_FAIL               0x0FU

/** \brief Error code for failed write of Prog conditions */
#if (defined DCM_E_SET_PROG_CONDITIONS_FAIL)
  #error "DCM_E_SET_PROG_CONDITIONS_FAIL is already defined"
#endif
#define DCM_E_SET_PROG_CONDITIONS_FAIL     0x09U

/* This is defined as a runtime error in SWS with value 0x01. Here we report it as a DET error */
/** \brief Error code for failed write of Prog conditions */
#if (defined DCM_E_INVALID_VALUE)
  #error "DCM_E_INVALID_VALUE is already defined"
#endif
#define DCM_E_INVALID_VALUE                0x13U

/** \brief Error code for invalid Diagnostic Request Length */
#if (defined DCM_E_INVALID_REQLEN)
  #error "DCM_E_INVALID_REQLEN is already defined"
#endif
#define DCM_E_INVALID_REQLEN               0x0AU

/** \brief Error code for ReturnControlToECU when session transition to default occurs */
#if (defined DCM_E_RETURNCONTROLTOECU_FAIL)
  #error "DCM_E_RETURNCONTROLTOECU_FAIL is already defined"
#endif
#define DCM_E_RETURNCONTROLTOECU_FAIL      0x0BU

/** \brief Error code for ReturnControlToECU when processing Timeout occurs */
#if (defined DCM_E_RETURNCONTROLTOECU_TIMEOUT)
  #error "DCM_E_RETURNCONTROLTOECU_TIMEOUT is already defined"
#endif
#define DCM_E_RETURNCONTROLTOECU_TIMEOUT   0x0CU

/** \brief Error code for failed setting of ROE event activation status */
#if (defined DCM_E_ROE_ACTIVATE_EVENT_FAIL)
  #error "DCM_E_ROE_ACTIVATE_EVENT_FAIL is already defined"
#endif
#define DCM_E_ROE_ACTIVATE_EVENT_FAIL      0x10U

/** \brief Error code for protocol information requested with no protocol running */
#if (defined DCM_E_NO_CURRENT_PROTOCOL)
  #error "DCM_E_NO_CURRENT_PROTOCOL is already defined"
#endif
#define DCM_E_NO_CURRENT_PROTOCOL          0x11U

/** \brief Error code for wrong configuration of DcmDspRoeBufSize */
#if (defined DCM_E_ROE_BUFFER_CONFIG_FAIL)
  #error "DCM_E_ROE_BUFFER_CONFIG_FAIL is already defined"
#endif
#define DCM_E_ROE_BUFFER_CONFIG_FAIL       0x0DU

/** \brief Error code for failed NvM_CancelJobs() operation */
#if (defined DCM_E_NVM_CANCEL_JOBS_FAILED)
  #error "DCM_E_NVM_CANCEL_JOBS_FAILED is already defined"
#endif
#define DCM_E_NVM_CANCEL_JOBS_FAILED       0x0EU

/** \brief Error code for memory overlap */
#if (defined DCM_E_MEMORY_OVERLAP)
  #error "DCM_E_MEMORY_OVERLAP is already defined"
#endif
#define DCM_E_MEMORY_OVERLAP    0x0FU

/** \brief Error code for ranges with wrong limit definitions */
#if (defined DCM_E_RANGE_WITH_WRONG_LIMITS)
  #error "DCM_E_RANGE_WITH_WRONG_LIMITS is already defined"
#endif
#define DCM_E_RANGE_WITH_WRONG_LIMITS    0x10U

/** \brief Error code for failed ReleaseLock() operation */
#if (defined DCM_E_RELEASELOCK_FAILED)
  #error "DCM_E_RELEASELOCK_FAILED is already defined"
#endif
#define DCM_E_RELEASELOCK_FAILED           0x12U

/** \brief Error code for failed SchM_Call_ComM_DCM_ActiveDiagnostic */
#if (defined DCM_E_SCHM_CALL_ACTIVEDIAGNOSTIC_FAILED)
  #error "DCM_E_SCHM_CALL_ACTIVEDIAGNOSTIC_FAILED is already defined"
#endif
#define DCM_E_SCHM_CALL_ACTIVEDIAGNOSTIC_FAILED        0x14U

/** \brief Error code for failed SchM_Call_ComM_DCM_InactiveDiagnostic */
#if (defined DCM_E_SCHM_CALL_INACTIVEDIAGNOSTIC_FAILED)
  #error "DCM_E_SCHM_CALL_INACTIVEDIAGNOSTIC_FAILED is already defined"
#endif
#define DCM_E_SCHM_CALL_INACTIVEDIAGNOSTIC_FAILED      0x15U

/** \brief Invalid value for NotifResultType */
#if (defined DCM_INVALID_NTFRSLT_RANGE_START)
  #error "DCM_INVALID_NTFRSLT_RANGE_START is already defined"
#endif
#define DCM_INVALID_NTFRSLT_RANGE_START                0x79U

/** \brief P2StarMaxTime parameter Resolution */
#if (defined DCM_P2STARTIME_RESOLUTION)
  #error "DCM_P2STARTIME_RESOLUTION is already defined"
#endif
#define DCM_P2STARTIME_RESOLUTION          10U

/** \brief Byte shifting in a word */
#if (defined DCM_BYTE_SWAPPING)
  #error "DCM_BYTE_SWAPPING is already defined"
#endif
#define DCM_BYTE_SWAPPING                   8U

/** \brief Do not send response to an OBD message */
#if (defined DCM_E_INHIBIT)
  #error "DCM_E_INHIBIT is already defined"
#endif
#define DCM_E_INHIBIT                       0xAAU

/* Macro for OBD availability ranges */
#if (defined DCM_OBD_AVAILABILITY_RANGE)
  #error "DCM_OBD_AVAILABILITY_RANGE is already defined"
#endif
#define DCM_OBD_AVAILABILITY_RANGE         0x20U

/** \brief Determine if a given PID/MID/... is of availability type */
#if (defined DCM_IS_AVAILABILITY_OBDSUBID)
  #error "DCM_IS_AVAILABILITY_OBDSUBID is already defined"
#endif
#define DCM_IS_AVAILABILITY_OBDSUBID(ObdSubId) \
          ((((ObdSubId) % DCM_OBD_AVAILABILITY_RANGE) == 0U) ? TRUE : FALSE)

#if (defined DCM_S3SERVER_TICK_DEFAULT)
#error DCM_S3SERVER_TICK_DEFAULT already defined
#endif /* if (defined DCM_S3SERVER_TICK_DEFAULT) */

/** \brief MainFunction Tick for S3Server Timer */
#define DCM_S3SERVER_TICK_DEFAULT          (5000U/DCM_TASK_TIME)

#if (defined DCM_P2STARMIN_TICK_DEFAULT)
#error DCM_P2STARMIN_TICK_DEFAULT already defined
#endif /* if (defined DCM_P2STARMIN_TICK_DEFAULT) */

/** \brief MainFunction Tick for P2*ServerMin Timer */
#define DCM_P2STARMIN_TICK_DEFAULT         0x00U

#if (defined DCM_P2MIN_TICK_DEFAULT)
#error DCM_P2MIN_TICK_DEFAULT already defined
#endif /* if (defined DCM_P2MIN_TICK_DEFAULT) */

/** \brief MainFunction Tick for P2ServerMin Timer */
#define DCM_P2MIN_TICK_DEFAULT             0x00U

#if (defined DCM_PROCESS_PAGE_ID_INVALID)
#error DCM_PROCESS_PAGE_ID_INVALID already defined
#endif /* if (defined DCM_PROCESS_PAGE_ID_INVALID) */

/** \brief Invalid value for process page id */
#define DCM_PROCESS_PAGE_ID_INVALID 0xFFU

#if (defined DCM_SEC_TAB_ENTRY_IDX_LOCKED)
#error DCM_SEC_TAB_ENTRY_IDX_LOCKED already defined
#endif /* if (defined DCM_SEC_TAB_ENTRY_IDX_LOCKED) */

/** \brief Invalid value for security table id */
#define DCM_SEC_TAB_ENTRY_IDX_LOCKED 0xFFU

#if (defined DCM_PROCESS_PAGE_FILTERED_DTC)
#error DCM_PROCESS_PAGE_FILTERED_DTC already defined
#endif /* if (defined DCM_PROCESS_PAGE_FILTERED_DTC) */

/** \brief Invalid value for process page id */
#define DCM_PROCESS_PAGE_FILTERED_DTC 0x00U

#if (defined DCM_PROCESS_PAGE_DTC_EXT_DATA_RECORD)
#error DCM_PROCESS_PAGE_DTC_EXT_DATA_RECORD already defined
#endif /* if (defined DCM_PROCESS_PAGE_DTC_EXT_DATA_RECORD) */

/** \brief Invalid value for process page id */
#define DCM_PROCESS_PAGE_DTC_EXT_DATA_RECORD 0x01U

#if (defined DCM_PROCESS_PAGE_DTC_SNAPSHOT_IDENTIFICATION)
#error DCM_PROCESS_PAGE_DTC_SNAPSHOT_IDENTIFICATION already defined
#endif /* if (defined DCM_PROCESS_PAGE_DTC_SNAPSHOT_IDENTIFICATION) */

/** \brief Invalid value for process page id */
#define DCM_PROCESS_PAGE_DTC_SNAPSHOT_IDENTIFICATION 0x02U

#if (defined DCM_DSPINTERNAL_DCMCONFIRMATION)
#error DCM_DSPINTERNAL_DCMCONFIRMATION already defined
#endif /* if (defined DCM_DSPINTERNAL_DCMCONFIRMATION) */

/** \brief Invalid value for process page id */
#define DCM_DSPINTERNAL_DCMCONFIRMATION 0x00U

#if (defined DCM_HSM_DCM_EV_INVALID)
#error DCM_HSM_DCM_EV_INVALID already defined
#endif /* if (defined DCM_HSM_DCM_EV_INVALID) */

/** \brief Invalid value for state machine event */
#define DCM_HSM_DCM_EV_INVALID 0xFFU

/* Getting the number of bytes occupied by bits */
#if (defined DCM_GET_BYTES)
  #error "DCM_GET_BYTES is already defined"
#endif
#define DCM_GET_BYTES(Bits)                 (((Bits) + 7U)/8U)

/* Getting the number of bits occupied by given bytes */
#if (defined DCM_GET_BITS)
  #error "DCM_GET_BITS is already defined"
#endif
#define DCM_GET_BITS(Bytes)                 ((Bytes) * 8U)

/* Getting the number of uint16s occupied by bits */
#if (defined DCM_GET_WORDS)
  #error "DCM_GET_WORDS is already defined"
#endif
#define DCM_GET_WORDS(Bits)                 (((Bits) + 15U)/16U)

/* Getting the number of uint32s occupied by bits */
#if (defined DCM_GET_DWORDS)
  #error "DCM_GET_DWORDS is already defined"
#endif
#define DCM_GET_DWORDS(Bits)                 (((Bits) + 31U)/32U)

#if (defined DCM_UINT8_MAX)
  #error "DCM_UINT8_MAX is already defined"
#endif
#define DCM_UINT8_MAX           (0xFFU)

#if (defined DCM_UINT16_MAX)
  #error "DCM_UINT16_MAX is already defined"
#endif
#define DCM_UINT16_MAX          (0xFFFFU)

#if (defined DCM_UINT32_MAX)
  #error "DCM_UINT32_MAX is already defined"
#endif
#define DCM_UINT32_MAX          (0xFFFFFFFFUL)

/* Getting the higher byte form uint16 */
#if (defined DCM_HIBYTE)
  #error "DCM_HIBYTE is already defined"
#endif
#define DCM_HIBYTE(word)                    ((uint8)(((word)>> 8U)& DCM_UINT8_MAX))

/* Getting the lower byte form uint16 */
#if (defined DCM_LOBYTE)
  #error "DCM_LOBYTE is already defined"
#endif
#define DCM_LOBYTE(word)                    ((uint8)((word)& DCM_UINT8_MAX))

/* Getting the higher word form uint32 */
#if (defined DCM_HIWORD)
  #error "DCM_HIWORD is already defined"
#endif
#define DCM_HIWORD(dword)                   ((uint16)(((dword)>> 16U)& DCM_UINT16_MAX))

/* Getting the lower word form uint32 */
#if (defined DCM_LOWORD)
  #error "DCM_LOWORD is already defined"
#endif
#define DCM_LOWORD(dword)                   ((uint16)((dword)& DCM_UINT16_MAX))

/* Getting the higher dword form uint64 */
#if (defined DCM_HIDWORD)
  #error "DCM_HIDWORD is already defined"
#endif
#define DCM_HIDWORD(qword)                  ((uint32)(((qword)>> 32U)& DCM_UINT32_MAX))

/* Getting the lower dword form uint64 */
#if (defined DCM_LODWORD)
  #error "DCM_LODWORD is already defined"
#endif
#define DCM_LODWORD(qword)                  ((uint32)((qword)& DCM_UINT32_MAX))

/* Checks whether a bit in a uint32 is set */
#if (defined DCM_UINT32_IS_BIT_SET)
  #error "DCM_UINT32_IS_BIT_SET is already defined"
#endif
#define DCM_UINT32_IS_BIT_SET(dword, BitPosition)   (((dword) & (uint32)((uint32)0x01U << (BitPosition))) != 0U)

/* Sets the value of a bit in byte */
#if (defined DCM_UINT32_SET_BIT)
  #error "DCM_UINT32_SET_BIT is already defined"
#endif
#define DCM_UINT32_SET_BIT(dword, BitPosition)      ((dword) |= (uint32)((uint32)0x01U <<(BitPosition)))

/* Clears the value of a bit in byte */
#if (defined DCM_UINT32_CLEAR_BIT)
  #error "DCM_UINT32_CLEAR_BIT is already defined"
#endif
#define DCM_UINT32_CLEAR_BIT(dword, BitPosition)    ((dword) &= (uint32)(~(uint32)((uint32)0x01U << (BitPosition))))

/* Checks whether a bit in a byte is set */
#if (defined DCM_UINT8_IS_BIT_SET)
  #error "DCM_UINT8_IS_BIT_SET is already defined"
#endif
#define DCM_UINT8_IS_BIT_SET(Byte, BitPosition)   (((Byte) & (uint8)(0x01U << (BitPosition))) != 0U)

/* Sets the value of a bit in byte */
#if (defined DCM_UINT8_SET_BIT)
  #error "DCM_UINT8_SET_BIT is already defined"
#endif
#define DCM_UINT8_SET_BIT(Byte, BitPosition)      ((Byte) |= (uint8)(0x01U <<(BitPosition)))

/* Clears the value of a bit in byte */
#if (defined DCM_UINT8_CLEAR_BIT)
  #error "DCM_UINT8_CLEAR_BIT is already defined"
#endif
#define DCM_UINT8_CLEAR_BIT(Byte, BitPosition)    ((Byte) &= (uint8)(~(uint8)(0x01U << (BitPosition))))

/* Checks whether a bit is set in an array of bytes */
#if (defined DCM_UINT8_IS_BIT_IN_ARRAY_SET)
  #error "DCM_UINT8_IS_BIT_IN_ARRAY_SET is already defined"
#endif
#define DCM_UINT8_IS_BIT_IN_ARRAY_SET(bitsArray, indexBit) \
        ((((bitsArray)[(indexBit)/8U]>>((indexBit)%8U))&1U)?(TRUE):(FALSE))

/** \brief Macro to create a 16bit word from two bytes */
#if (defined DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB)
  #error "DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB is already defined"
#endif
#define DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(HB, LB) \
                                          ((uint16)((uint16)(HB) << 8U) | (uint16)((uint16)(LB)))

/** \brief Extracts the given byte from an uint64 */
#if (defined DCM_BYTE_FROM_QWORD)
  #error "DCM_BYTE_FROM_QWORD is already defined"
#endif
#define DCM_BYTE_FROM_QWORD(QWord, BytePosition) \
  (uint8)(((QWord) >> (8U*((BytePosition) - 1U))) & 0xFFU)

/* \brief Macro for constructing a 24 bits DTC from octets */
#if (defined DCM_CONSTRUCT_DTC)
  #error "DCM_CONSTRUCT_DTC is already defined"
#endif
#define DCM_CONSTRUCT_DTC(HB,MB,LB) ((uint32)(HB) << 16U)|((uint32)(MB)<< 8U)|((uint32)(LB))

#if (defined DCM_DSP_MIDSERVICES_USE_LOCKING)
  #error "DCM_DSP_MIDSERVICES_USE_LOCKING is already defined"
#endif
/* \brief Macro to indicate the necessity of exclusive access to the MidServices */
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ONCHANGEOFDATAIDENTIFIER_EVENTS > 0U)\
    && ((DCM_DSP_SERVICE_0X06_ASYNC == STD_ON) || (DCM_DSP_SERVICE_0X22_ASYNC == STD_ON))\
    && (DCM_OBDMID_SUPPORT_SWC == STD_ON))
#define DCM_DSP_MIDSERVICES_USE_LOCKING STD_ON
#else
#define DCM_DSP_MIDSERVICES_USE_LOCKING STD_OFF
#endif

/* ------------------[ list of AUTOSAR API service IDs ]-----------------------------------------*/
#if (defined DCM_SVCID_STARTOFRECEPTION)
#error DCM_SVCID_STARTOFRECEPTION already defined
#endif /* if (defined DCM_SVCID_STARTOFRECEPTION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_STARTOFRECEPTION.
 */
#define DCM_SVCID_STARTOFRECEPTION 0x00U

#if (defined DCM_SERVID_INIT)
#error DCM_SERVID_INIT already defined
#endif /* if (defined DCM_SERVID_INIT) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_INIT.
 */
#define DCM_SERVID_INIT 0x01U

#if (defined DCM_SVCID_COPYRXDATA)
#error DCM_SVCID_COPYRXDATA already defined
#endif /* if (defined DCM_SVCID_COPYRXDATA) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_COPYRXDATA.
 */
#define DCM_SVCID_COPYRXDATA 0x02U

#if (defined DCM_SVCID_TPRXINDICATION)
#error DCM_SVCID_TPRXINDICATION already defined
#endif /* if (defined DCM_SVCID_TPRXINDICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_TPRXINDICATION.
 */
#define DCM_SVCID_TPRXINDICATION 0x03U

#if (defined DCM_SVCID_COPYTXDATA)
#error DCM_SVCID_COPYTXDATA already defined
#endif /* if (defined DCM_SVCID_COPYTXDATA) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_COPYTXDATA.
 */
#define DCM_SVCID_COPYTXDATA 0x04U

#if (defined DCM_SVCID_TPTXCONFIRMATION)
#error DCM_SVCID_TPTXCONFIRMATION already defined
#endif /* if (defined DCM_SVCID_TPTXCONFIRMATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_TPTXCONFIRMATION.
 */
#define DCM_SVCID_TPTXCONFIRMATION 0x05U

#if (defined DCM_SERVID_GETSESCTRLTYPE)
#error DCM_SERVID_GETSESCTRLTYPE already defined
#endif /* if (defined DCM_SERVID_GETSESCTRLTYPE) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_GETSESCTRLTYPE.
 */
#define DCM_SERVID_GETSESCTRLTYPE 0x06U

#if (defined DCM_SERVID_GETSECURITYLEVEL)
#error DCM_SERVID_GETSECURITYLEVEL already defined
#endif /* if (defined DCM_SERVID_GETSECURITYLEVEL) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_GETSECURITYLEVEL.
 */
#define DCM_SERVID_GETSECURITYLEVEL 0x0DU

#if (defined DCM_SERVID_GETACTIVEPROTOCOL)
#error DCM_SERVID_GETACTIVEPROTOCOL already defined
#endif /* if (defined DCM_SERVID_GETACTIVEPROTOCOL) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_GETACTIVEPROTOCOL.
 */
#define DCM_SERVID_GETACTIVEPROTOCOL 0x0FU

#if (defined DCM_SERVID_COMMNOCOMMODEENTERED)
#error DCM_SERVID_COMMNOCOMMODEENTERED already defined
#endif /* if (defined DCM_SERVID_COMMNOCOMMODEENTERED) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_COMMNOCOMMODEENTERED.
 */
#define DCM_SERVID_COMMNOCOMMODEENTERED 0x21U

#if (defined DCM_SERVID_COMMSILENTCOMMODEENTERED)
#error DCM_SERVID_COMMSILENTCOMMODEENTERED already defined
#endif /* if (defined DCM_SERVID_COMMSILENTCOMMODEENTERED) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_COMMSILENTCOMMODEENTERED.
 */
#define DCM_SERVID_COMMSILENTCOMMODEENTERED 0x22U

#if (defined DCM_SERVID_COMMFULLCOMMODEENTERED)
#error DCM_SERVID_COMMFULLCOMMODEENTERED already defined
#endif /* if (defined DCM_SERVID_COMMFULLCOMMODEENTERED) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_COMMFULLCOMMODEENTERED.
 */
#define DCM_SERVID_COMMFULLCOMMODEENTERED 0x23U

#if (defined DCM_SERVID_GETVERSIONINFO)
#error DCM_SERVID_GETVERSIONINFO already defined
#endif /* if (defined DCM_SERVID_GETVERSIONINFO) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_GETVERSIONINFO.
 */
#define DCM_SERVID_GETVERSIONINFO 0x24U

#if (defined DCM_SERVID_MAINFUNCTION)
#error DCM_SERVID_MAINFUNCTION already defined
#endif /* if (defined DCM_SERVID_MAINFUNCTION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_MAINFUNCTION.
 */
#define DCM_SERVID_MAINFUNCTION 0x25U

#if (defined DCM_SVCID_READMEMORY)
#error DCM_SVCID_READMEMORY already defined
#endif /* if (defined DCM_SVCID_READMEMORY) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_READMEMORY.
 */
#define DCM_SVCID_READMEMORY 0x26U

#if (defined DCM_SVCID_WRITEMEMORY)
#error DCM_SVCID_WRITEMEMORY already defined
#endif /* if (defined DCM_SVCID_WRITEMEMORY) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_WRITEMEMORY.
 */
#define DCM_SVCID_WRITEMEMORY 0x27U

#if (defined DCM_SERVID_DSL_RESPONSEONONEEVENT)
#error DCM_SERVID_DSL_RESPONSEONONEEVENT already defined
#endif /* if (defined DCM_SERVID_DSL_RESPONSEONONEEVENT) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_DSL_RESPONSEONONEEVENT.
 */
#define DCM_SERVID_DSL_RESPONSEONONEEVENT 0x28U

#if (defined DCM_SERVID_RESETTODEFAULTSESSION)
#error DCM_SERVID_RESETTODEFAULTSESSION already defined
#endif /* if (defined DCM_SERVID_RESETTODEFAULTSESSION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_RESETTODEFAULTSESSION.
 */
#define DCM_SERVID_RESETTODEFAULTSESSION 0x2AU

#if (defined DCM_SERVID_SETACTIVEDIAGNOSTIC)
#error DCM_SERVID_SETACTIVEDIAGNOSTIC already defined
#endif /* if (defined DCM_SERVID_SETACTIVEDIAGNOSTIC) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_SETACTIVEDIAGNOSTIC.
 */
#define DCM_SERVID_SETACTIVEDIAGNOSTIC 0x56U

#if (defined DCM_SERVID_DEMTRIGGERONDTC)
#error DCM_SERVID_DEMTRIGGERONDTC already defined
#endif /* if (defined DCM_SERVID_DEMTRIGGERONDTC) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_DEMTRIGGERONDTC.
 */
#define DCM_SERVID_DEMTRIGGERONDTC 0x2BU

#if (defined DCM_SERVID_TRIGGERONEVENT)
#error DCM_SERVID_TRIGGERONEVENT already defined
#endif /* if (defined DCM_SERVID_TRIGGERONEVENT) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_TRIGGERONEVENT.
 */
#define DCM_SERVID_TRIGGERONEVENT 0x2DU

#if (defined DCM_SVCID_REQUESTDOWNLOAD)
#error DCM_SVCID_REQUESTDOWNLOAD already defined
#endif /* if (defined DCM_SVCID_REQUESTDOWNLOAD) */
/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_REQUESTDOWNLOAD.
 */
#define DCM_SVCID_REQUESTDOWNLOAD 0x30U

#if (defined DCM_SVCID_REQUESTUPLOAD)
#error DCM_SVCID_REQUESTUPLOAD already defined
#endif /* if (defined DCM_SVCID_REQUESTUPLOAD) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_REQUESTUPLOAD.
 */
#define DCM_SVCID_REQUESTUPLOAD 0x31U

#if (defined DCM_SVCID_REQUESTTRANSFEREXIT)
#error DCM_SVCID_REQUESTTRANSFEREXIT already defined
#endif /* if (defined DCM_SVCID_REQUESTTRANSFEREXIT) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_REQUESTTRANSFEREXIT.
 */
#define DCM_SVCID_REQUESTTRANSFEREXIT 0x32U

#if (defined DCM_SVCID_READDATALENGTH_SYNC)
#error DCM_SVCID_READDATALENGTH_SYNC already defined
#endif /* if (defined DCM_SVCID_READDATALENGTH_SYNC) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_READDATALENGTH_SYNC.
 */
#define DCM_SVCID_READDATALENGTH_SYNC 0x36U

#if (defined DCM_SVCID_READDATALENGTH_ASYNC)
#error DCM_SVCID_READDATALENGTH_ASYNC already defined
#endif /* if (defined DCM_SVCID_READDATALENGTH_ASYNC) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_READDATALENGTH_ASYNC.
 */
#define DCM_SVCID_READDATALENGTH_ASYNC 0x4CU

#if (defined DCM_SVCID_CONDITIONCHECKREAD_ASYNC)
#error DCM_SVCID_CONDITIONCHECKREAD_ASYNC already defined
#endif /* if (defined DCM_SVCID_CONDITIONCHECKREAD_ASYNC) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_CONDITIONCHECKREAD_ASYNC.
 */
#define DCM_SVCID_CONDITIONCHECKREAD_ASYNC 0x37U

#if (defined DCM_SVCID_READDATA_SYNC)
#error DCM_SVCID_READDATA_SYNC already defined
#endif /* if (defined DCM_SVCID_READDATA_SYNC) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_READDATA_SYNC.
 */
#define DCM_SVCID_READDATA_SYNC 0x34U

#if (defined DCM_SVCID_READDATA_ASYNC)
#error DCM_SVCID_READDATA_ASYNC already defined
#endif /* if (defined DCM_SVCID_READDATA_ASYNC) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_READDATA_ASYNC.
 */
#define DCM_SVCID_READDATA_ASYNC 0x3BU

#if (defined DCM_SVCID_READSCALINGDATA_SYNC)
#error DCM_SVCID_READSCALINGDATA_SYNC already defined
#endif /* if (defined DCM_SVCID_READSCALINGDATA_SYNC) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_READSCALINGDATA_SYNC.
 */
#define DCM_SVCID_READSCALINGDATA_SYNC 0x4BU

#if (defined DCM_SVCID_READSCALINGDATA_ASYNC)
#error DCM_SVCID_READSCALINGDATA_ASYNC already defined
#endif /* if (defined DCM_SVCID_READSCALINGDATA_ASYNC) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_READSCALINGDATA_ASYNC.
 */
#define DCM_SVCID_READSCALINGDATA_ASYNC 0x38U

#if (defined DCM_SVCID_WRITEDATA_SYNC_FIXLENGTH)
#error DCM_SVCID_WRITEDATA_SYNC_FIXLENGTH already defined
#endif /* if (defined DCM_SVCID_WRITEDATA_SYNC_FIXLENGTH) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_WRITEDATA_SYNC_FIXLENGTH.
 */
#define DCM_SVCID_WRITEDATA_SYNC_FIXLENGTH 0x51U

#if (defined DCM_SVCID_WRITEDATA_SYNC_VARLENGTH)
#error DCM_SVCID_WRITEDATA_SYNC_VARLENGTH already defined
#endif /* if (defined DCM_SVCID_WRITEDATA_SYNC_VARLENGTH) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_WRITEDATA_SYNC_VARLENGTH.
 */
#define DCM_SVCID_WRITEDATA_SYNC_VARLENGTH 0x52U

#if (defined DCM_SVCID_WRITEDATA_ASYNC_FIXLENGTH)
#error DCM_SVCID_WRITEDATA_ASYNC_FIXLENGTH already defined
#endif /* if (defined DCM_SVCID_WRITEDATA_ASYNC_FIXLENGTH) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_WRITEDATA_ASYNC_FIXLENGTH.
 */
#define DCM_SVCID_WRITEDATA_ASYNC_FIXLENGTH 0x35U

#if (defined DCM_SVCID_WRITEDATA_ASYNC_VARLENGTH)
#error DCM_SVCID_WRITEDATA_ASYNC_VARLENGTH already defined
#endif /* if (defined DCM_SVCID_WRITEDATA_ASYNC_VARLENGTH) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_WRITEDATA_ASYNC_VARLENGTH.
 */
#define DCM_SVCID_WRITEDATA_ASYNC_VARLENGTH 0x3EU

#if (defined DCM_SVCID_CONDITIONCHECKREAD_SYNC)
#error DCM_SVCID_CONDITIONCHECKREAD_SYNC already defined
#endif /* if (defined DCM_SVCID_CONDITIONCHECKREAD_SYNC) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_CONDITIONCHECKREAD_SYNC.
 */
#define DCM_SVCID_CONDITIONCHECKREAD_SYNC 0x49U

#if (defined DCM_SVCID_TXCONFIRMATION)
#error DCM_SVCID_TXCONFIRMATION already defined
#endif /* if (defined DCM_SVCID_TXCONFIRMATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_TXCONFIRMATION.
 */
#define DCM_SVCID_TXCONFIRMATION 0x40U

#if (defined DCM_SERVID_DSP_DCMCONFIRMATION)
#error DCM_SERVID_DSP_DCMCONFIRMATION already defined
#endif /* if (defined DCM_SERVID_DSP_DCMCONFIRMATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_DSP_DCMCONFIRMATION.
 */
#define DCM_SERVID_DSP_DCMCONFIRMATION 0x42U

#if (defined DCM_SERVID_PROCESSINGDONE)
#error DCM_SERVID_PROCESSINGDONE already defined
#endif /* if (defined DCM_SERVID_PROCESSINGDONE) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_PROCESSINGDONE.
 */
#define DCM_SERVID_PROCESSINGDONE 0x43U

#if (defined DCM_SERVID_SETNEGRESPONSE)
#error DCM_SERVID_SETNEGRESPONSE already defined
#endif /* if (defined DCM_SERVID_SETNEGRESPONSE) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_SETNEGRESPONSE.
 */
#define DCM_SERVID_SETNEGRESPONSE 0x44U

#if (defined DCM_SERVID_DSD_STARTPAGEDPROCESSING)
#error DCM_SERVID_DSD_STARTPAGEDPROCESSING already defined
#endif /* if (defined DCM_SERVID_DSD_STARTPAGEDPROCESSING) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_DSD_STARTPAGEDPROCESSING.
 */
#define DCM_SERVID_DSD_STARTPAGEDPROCESSING 0x45U

#if (defined DCM_SERVID_DSD_MANUFACTURERNOTIFICATION)
#error DCM_SERVID_DSD_MANUFACTURERNOTIFICATION already defined
#endif /* if (defined DCM_SERVID_DSD_MANUFACTURERNOTIFICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_DSD_MANUFACTURERNOTIFICATION.
 */
#define DCM_SERVID_DSD_MANUFACTURERNOTIFICATION 0x46U

#if (defined DCM_SERVID_DSD_SUPPLIERNOTIFICATION)
#error DCM_SERVID_DSD_SUPPLIERNOTIFICATION already defined
#endif /* if (defined DCM_SERVID_DSD_SUPPLIERNOTIFICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_DSD_SUPPLIERNOTIFICATION.
 */
#define DCM_SERVID_DSD_SUPPLIERNOTIFICATION 0x47U

#if (defined DCM_SVCID_DTCSETTINGON)
#error DCM_SVCID_DTCSETTINGON already defined
#endif /* if (defined DCM_SVCID_DTCSETTINGON) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_DTCSETTINGON.
 */
#define DCM_SVCID_DTCSETTINGON 0x48U

#if (defined DCM_SVCID_DTCSETTINGOFF)
#error DCM_SVCID_DTCSETTINGOFF already defined
#endif /* if (defined DCM_SVCID_DTCSETTINGOFF) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_DTCSETTINGOFF.
 */
#define DCM_SVCID_DTCSETTINGOFF 0x49U

#if (defined DCM_SVCID_SETPROGCONDITIONS)
#error DCM_SVCID_SETPROGCONDITIONS already defined
#endif /* if (defined DCM_SVCID_SETPROGCONDITIONS) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_SETPROGCONDITIONS.
 */
#define DCM_SVCID_SETPROGCONDITIONS 0x4AU

#if (defined DCM_SVCID_GETPROGCONDITIONS)
#error DCM_SVCID_GETPROGCONDITIONS already defined
#endif /* if (defined DCM_SVCID_GETPROGCONDITIONS) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_GETPROGCONDITIONS.
 */
#define DCM_SVCID_GETPROGCONDITIONS 0x4BU

#if (defined DCM_SVCID_CONTROLDTCSETTING)
#error DCM_SVCID_CONTROLDTCSETTING already defined
#endif /* if (defined DCM_SVCID_CONTROLDTCSETTING) */
/** \brief API ID of service handler ControlDTCSetting. */
#define DCM_SVCID_CONTROLDTCSETTING 0x4DU

#if (defined DCM_SSVCID_HANDLEREXTRACTOR)
#error DCM_SSVCID_HANDLEREXTRACTOR already defined
#endif /* if (defined DCM_SSVCID_HANDLEREXTRACTOR) */
/** \brief API ID for extractor of subservice handler. */
#define DCM_SSVCID_HANDLEREXTRACTOR 0x55U

#if (defined DCM_SVCID_CLEARDTC)
#error DCM_SVCID_CLEARDTC already defined
#endif /* if (defined DCM_SVCID_CLEARDTC) */

/** \brief ClearDTC Rte interface service ID.
 **
 ** Definition of DCM_SVCID_CLEARDTC.
 */
#define DCM_SVCID_CLEARDTC 0x61U

#if (defined DCM_SVCID_GETFREEZEFRAMEDATABYDTC)
#error DCM_SVCID_GETFREEZEFRAMEDATABYDTC already defined
#endif /* if (defined DCM_SVCID_GETFREEZEFRAMEDATABYDTC) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_GETFREEZEFRAMEDATABYDTC.
 */
#define DCM_SVCID_GETFREEZEFRAMEDATABYDTC 0x70U

#if (defined DCM_SVCID_GETEXTENDEDDATABYDTC)
#error DCM_SVCID_GETEXTENDEDDATABYDTC already defined
#endif /* if (defined DCM_SVCID_GETEXTENDEDDATABYDTC) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_GETEXTENDEDDATABYDTC.
 */
#define DCM_SVCID_GETEXTENDEDDATABYDTC 0x71U

#if (defined DCM_SVCID_ENABLEDTCRECORDUPDATE)
#error DCM_SVCID_ENABLEDTCRECORDUPDATE already defined
#endif /* if (defined DCM_SVCID_ENABLEDTCRECORDUPDATE) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_ENABLEDTCRECORDUPDATE.
 */
#define DCM_SVCID_ENABLEDTCRECORDUPDATE 0x72U

#if (defined DCM_SVCID_DISABLEDTCRECORDUPDATE)
#error DCM_SVCID_DISABLEDTCRECORDUPDATE already defined
#endif /* if (defined DCM_SVCID_DISABLEDTCRECORDUPDATE) */

/** \brief Dem_DisableDTCRecordUpdate() DEM interface service ID.
 **
 ** Definition of DCM_SVCID_DISABLEDTCRECORDUPDATE.
 */
#define DCM_SVCID_DISABLEDTCRECORDUPDATE 0x73U

#if (defined DCM_SVCID_ACTIVATEEVENT)
#error DCM_SVCID_ACTIVATEEVENT already defined
#endif /* if (defined DCM_SVCID_ACTIVATEEVENT) */

/** \brief ROE ActivateEvent Rte interface service ID.
 **
 ** Definition of DCM_SVCID_ACTIVATEEVENT.
 */
#define DCM_SVCID_ACTIVATEEVENT 0x74U

#if (defined DCM_SERVID_DSD_PROCESSPAGE)
#error DCM_SERVID_DSD_PROCESSPAGE already defined
#endif /* if (defined DCM_SERVID_DSD_PROCESSPAGE) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_DSD_PROCESSPAGE.
 */
#define DCM_SERVID_DSD_PROCESSPAGE 0x69U

#if (defined DCM_SERVID_DSD_UPDATEPAGE)
#error DCM_SERVID_DSD_UPDATEPAGE already defined
#endif /* if (defined DCM_SERVID_DSD_UPDATEPAGE) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_DSD_UPDATEPAGE.
 */
#define DCM_SERVID_DSD_UPDATEPAGE 0x6AU

#if (defined DCM_SERVID_COMMON_COMM_MODE_ENTRY)
#error DCM_SERVID_COMMON_COMM_MODE_ENTRY already defined
#endif /* if (defined DCM_SERVID_COMMON_COMM_MODE_ENTRY) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SERVID_COMMON_COMM_MODE_ENTRY.
 */
#define DCM_SERVID_COMMON_COMM_MODE_ENTRY 0x2CU

#if (defined DCM_SVCID_PROCESSSERVICEASYNC)
#error DCM_SVCID_PROCESSSERVICEASYNC already defined
#endif /* if (defined DCM_SVCID_PROCESSSERVICEASYNC) */

/** \brief Internal API service ID.
 **
 ** Definition of DCM_SVCID_PROCESSSERVICEASYNC.
 */
#define DCM_SVCID_PROCESSSERVICEASYNC 0x73U

#if (defined DCM_SVCID_ROEONCHANGEOFDID)
#error DCM_SVCID_ROEONCHANGEOFDID already defined
#endif /* if (defined DCM_SVCID_ROEONCHANGEOFDID) */

/** \brief Internal API service ID.
 **
 ** Definition of DCM_SVCID_ROEONCHANGEOFDID.
 */
#define DCM_SVCID_ROEONCHANGEOFDID 0x75U

#if (defined DCM_SVCID_NVMWRITEDATA)
#error DCM_SVCID_NVMWRITEDATA already defined
#endif /* if (defined DCM_SVCID_NVMWRITEDATA) */

/** \brief Internal API service ID.
 **
 ** Definition of DCM_SVCID_NVMWRITEDATA.
 */
#define DCM_SVCID_NVMWRITEDATA 0x76U

#if (defined DCM_SVCID_NVMREADDATA)
#error DCM_SVCID_NVMREADDATA already defined
#endif /* if (defined DCM_SVCID_NVMREADDATA) */

/** \brief Internal API service ID.
 **
 ** Definition of DCM_SVCID_NVMREADDATA.
 */
#define DCM_SVCID_NVMREADDATA 0x77U

#if (defined DCM_SVCID_NVMREADBLOCK)
#error DCM_SVCID_NVMREADBLOCK already defined
#endif /* if (defined DCM_SVCID_NVMREADBLOCK) */

/** \brief Internal API service ID.
 **
 ** Definition of DCM_SVCID_NVMREADBLOCK.
 */
#define DCM_SVCID_NVMREADBLOCK 0x78U

#if (defined DCM_SVCID_NVMWRITEBLOCK)
#error DCM_SVCID_NVMWRITEBLOCK already defined
#endif /* if (defined DCM_SVCID_NVMWRITEBLOCK) */

/** \brief Internal API service ID.
 **
 ** Definition of DCM_SVCID_NVMWRITEBLOCK.
 */
#define DCM_SVCID_NVMWRITEBLOCK 0x79U

#if (defined DCM_SVCID_NVMCANCELJOBS)
#error DCM_SVCID_NVMCANCELJOBS already defined
#endif /* if (defined DCM_SVCID_NVMCANCELJOBS) */

/** \brief Internal API service ID.
 **
 ** Definition of DCM_SVCID_NVMCANCELJOBS.
 */
#define DCM_SVCID_NVMCANCELJOBS 0x7AU

#if (defined DCM_SVCID_NVMGETERRORSTATUS)
#error DCM_SVCID_NVMGETERRORSTATUS already defined
#endif /* if (defined DCM_SVCID_NVMGETERRORSTATUS) */

/** \brief Internal API service ID.
 **
 ** Definition of DCM_SVCID_NVMGETERRORSTATUS.
 */
#define DCM_SVCID_NVMGETERRORSTATUS 0x7BU

#if (defined DCM_SVCID_SECURITYACCESS)
#error DCM_SVCID_SECURITYACCESS already defined
#endif /* if (defined DCM_SVCID_SECURITYACCESS) */

/** \brief Internal API service ID for SecurityAccess.
 **
 ** Definition of DCM_SVCID_SECURITYACCESS.
 */
#define DCM_SVCID_SECURITYACCESS 0x90U

#if (defined DCM_SVCID_SECURITYGETSECURITYATTEMPTCOUNTER)
#error DCM_SVCID_SECURITYGETSECURITYATTEMPTCOUNTER already defined
#endif /* if (defined DCM_SVCID_SECURITYGETSECURITYATTEMPTCOUNTER) */

/** \brief Internal API service ID for GetSecurityAttemptCounter.
 **
 ** Definition of DCM_SVCID_SECURITYGETSECURITYATTEMPTCOUNTER.
 */
#define DCM_SVCID_SECURITYGETSECURITYATTEMPTCOUNTER 0x59U

#if (defined DCM_SVCID_SECURITYSETSECURITYATTEMPTCOUNTER)
#error DCM_SVCID_SECURITYSETSECURITYATTEMPTCOUNTER already defined
#endif /* if (defined DCM_SVCID_SECURITYSETSECURITYATTEMPTCOUNTER) */

/** \brief Internal API service ID for SetSecurityAttemptCounter.
 **
 ** Definition of DCM_SVCID_SECURITYSETSECURITYATTEMPTCOUNTER.
 */
#define DCM_SVCID_SECURITYSETSECURITYATTEMPTCOUNTER 0x5AU

#if (defined DCM_SVCID_SECURITYGETSEEDFNCADRASYN)
#error DCM_SVCID_SECURITYGETSEEDFNCADRASYN already defined
#endif /* if (defined DCM_SVCID_SECURITYGETSEEDFNCADRASYN) */

/** \brief Internal API service ID for GetSeed async with ADR.
 **
 ** Definition of DCM_SVCID_SECURITYGETSEEDFNCADRASYN.
 */
#define DCM_SVCID_SECURITYGETSEEDFNCADRASYN 0x91U

#if (defined DCM_SVCID_SECURITYGETSEEDFNCADRSYNC)
#error DCM_SVCID_SECURITYGETSEEDFNCADRSYNC already defined
#endif /* if (defined DCM_SVCID_SECURITYGETSEEDFNCADRSYNC) */

/** \brief Internal API service ID for GetSeed sync with ADR.
 **
 ** Definition of DCM_SVCID_SECURITYGETSEEDFNCADRSYNC.
 */
#define DCM_SVCID_SECURITYGETSEEDFNCADRSYNC 0x92U

#if (defined DCM_SVCID_SECURITYGETSEEDFNCASYNC)
#error DCM_SVCID_SECURITYGETSEEDFNCASYNC already defined
#endif /* if (defined DCM_SVCID_SECURITYGETSEEDFNCASYNC) */

/** \brief Internal API service ID for GetSeed async.
 **
 ** Definition of DCM_SVCID_SECURITYGETSEEDFNCASYNC.
 */
#define DCM_SVCID_SECURITYGETSEEDFNCASYNC 0x93U

#if (defined DCM_SVCID_SECURITYGETSEEDFNCSYNC)
#error DCM_SVCID_SECURITYGETSEEDFNCSYNC already defined
#endif /* if (defined DCM_SVCID_SECURITYGETSEEDFNCSYNC) */

/** \brief Internal API service ID for GetSeed sync.
 **
 ** Definition of DCM_SVCID_SECURITYGETSEEDFNCSYNC.
 */
#define DCM_SVCID_SECURITYGETSEEDFNCSYNC 0x94U

#if (defined DCM_API_CALLER_GENERIC_SVCH_IMPLEMENTATION_ID)
#error DCM_API_CALLER_GENERIC_SVCH_IMPLEMENTATION_ID already defined
#endif /* if (defined DCM_API_CALLER_GENERIC_SVCH_IMPLEMENTATION_ID) */
/** \brief External API service ID for Dcm_APICaller_GenericSvcHImplementation.
 **
 ** Definition of DCM_API_CALLER_GENERIC_SVCH_IMPLEMENTATION_ID.
 */
#define DCM_API_CALLER_GENERIC_SVCH_IMPLEMENTATION_ID 0x7FU

#if (defined DCM_SVCID_JUMPTOBL)
#error DCM_SVCID_JUMPTOBL already defined
#endif /* if (defined DCM_SVCID_JUMPTOBL) */

/** \brief Internal API service ID for Dcm_DspInternal_JumpToBL.
 **
 ** Definition of DCM_SVCID_JUMPTOBL.
 */
#define DCM_SVCID_JUMPTOBL 0x95U

#if (defined DCM_SVCID_DEMREPORTSTATUSBYTE)
#error  already defined DCM_SVCID_DEMREPORTSTATUSBYTE
#endif /* if (defined DCM_SVCID_DEMREPORTSTATUSBYTE) */

/** \brief Internal API service ID for Dcm_DspInternal_DemReportStatusByte.
 **
 ** Definition of DCM_SVCID_DEMREPORTSTATUSBYTE.
 */
#define DCM_SVCID_DEMREPORTSTATUSBYTE 0x96U

#if (defined DCM_SVCID_READDATABYIDENTIFIER)
#error DCM_SVCID_READDATABYIDENTIFIER already defined
#endif /* if (defined DCM_SVCID_READDATABYIDENTIFIER) */

/** \brief Internal API service ID.
 **
 ** Definition of DCM_SVCID_READDATABYIDENTIFIER.
 */
#define DCM_SVCID_READDATABYIDENTIFIER 0x97U

#if (defined DCM_SVCID_READSCALINGDATABYIDENTIFIER)
#error DCM_SVCID_READSCALINGDATABYIDENTIFIER already defined
#endif /* if (defined DCM_SVCID_READSCALINGDATABYIDENTIFIER) */

/** \brief Internal API service ID.
 **
 ** Definition of DCM_SVCID_READSCALINGDATABYIDENTIFIER.
 */
#define DCM_SVCID_READSCALINGDATABYIDENTIFIER 0xB3U

#if (defined DCM_SVCID_GETFILTEREDDTC)
#error DCM_SVCID_GETFILTEREDDTC already defined
#endif /* if (defined DCM_SVCID_GETFILTEREDDTC) */

/** \brief Internal API service ID.
 **
 ** Definition of DCM_SVCID_GETFILTEREDDTC.
 */
#define DCM_SVCID_GETFILTEREDDTC 0x98U

#if (defined DCM_IS_OBD_SVCID)
#error DCM_IS_OBD_SVCID already defined
#endif
#define DCM_IS_OBD_SVCID(x) (((x) <= 0x0FU))

#if (defined DCM_SVCID_OBD_DEFENSIVE_BRANCH)
#error DCM_SVCID_OBD_DEFENSIVE_BRANCH already defined
#endif /* if (defined DCM_SVCID_OBD_DEFENSIVE_BRANCH) */

#define DCM_SVCID_OBD_DEFENSIVE_BRANCH 0xA0U

#if (defined DCM_SVCID_RDBPI)
#error DCM_SVCID_RDBPI already defined
#endif /* if (defined DCM_SVCID_RDBPI) */

/** \brief Internal API service ID for Dcm_DspInternal_ReadDataByPeriodicIdentifier.
 **
 ** Definition of DCM_SVCID_RDBPI.
 */
#define DCM_SVCID_RDBPI 0x99U

#if (defined DCM_SVCID_DYNAMICALLYDEFINEDDID)
#error DCM_SVCID_DYNAMICALLYDEFINEDDID already defined
#endif /* if (defined DCM_SVCID_DYNAMICALLYDEFINEDDID) */

/** \brief Internal API service ID.
 **
 ** Definition of DCM_SVCID_DYNAMICALLYDEFINEDDID.
 */
#define DCM_SVCID_DYNAMICALLYDEFINEDDID 0x9AU

#if (defined DCM_SVCID_ASYNCSERVICEDISPATCHER)
#error DCM_SVCID_ASYNCSERVICEDISPATCHER already defined
#endif /* if (defined DCM_SVCID_ASYNCSERVICEDISPATCHER) */
/** \brief Internal API service ID for the asynchronous service dispatcher.
 **
 ** Definition of DCM_SVCID_ASYNCSERVICEDISPATCHER.
 */
#define DCM_SVCID_ASYNCSERVICEDISPATCHER 0xA0U

#if ((DCM_DSP_USE_SERVICE_0X19 == STD_ON) || (DCM_DSP_USE_SERVICE_0XAF == STD_ON))
#if (defined DCM_SVCID_READDTCINFORMATION)
#error DCM_SVCID_READDTCINFORMATION already defined
#endif /* if (defined DCM_SVCID_READDTCINFORMATION) */
/** \brief Internal API service ID for service ReadDTC.
 **
 ** Definition of DCM_SVCID_READDTCINFORMATION.
 */
#define DCM_SVCID_READDTCINFORMATION 0xA1U
#endif /* #if ((DCM_DSP_USE_SERVICE_0X19 == STD_ON) || (DCM_DSP_USE_SERVICE_0XAF == STD_ON)) */

#if (defined DCM_SVCID_CLEAREMISSIONDIAGINFO)
#error DCM_SVCID_CLEAREMISSIONDIAGINFO already defined
#endif /* if (defined DCM_SVCID_CLEAREMISSIONDIAGINFO) */

/** \brief Internal API service ID for Dcm_DspInternal_ClearEmissionDiagInfo_SvcH.
 **
 ** Definition of DCM_SVCID_CLEAREMISSIONDIAGINFO.
 */
#define DCM_SVCID_CLEAREMISSIONDIAGINFO 0xA4U

/** \brief Internal API service ID for PduR_DcmTransmit.
 **
 ** Definition of DCM_SVCID_PDUR_DCMTRANSMIT.
 */
#if (defined DCM_SVCID_PDUR_DCMTRANSMIT)
  #error "DCM_SVCID_PDUR_DCMTRANSMIT is already defined"
#endif
#define DCM_SVCID_PDUR_DCMTRANSMIT 0xA5U

#if (defined DCM_SVCID_PROCESSMODERULES)
#error DCM_SVCID_PROCESSMODERULES already defined
#endif /* if (defined DCM_SVCID_PROCESSMODERULES) */

/** \brief Internal API service ID for Dcm_Dsp_CommunicationControl_ProcessModeRules.
 **
 ** Definition of DCM_SVCID_PROCESSMODERULES.
 */
#define DCM_SVCID_PROCESSMODERULES 0xA6U

#if (defined DCM_SVCID_RESPONSEONEVENT)
#error DCM_SVCID_RESPONSEONEVENT already defined
#endif /* if (defined DCM_SVCID_RESPONSEONEVENT) */

/** \brief Internal API service ID for Dcm_Dsp_ResponseOnEvent_SvcH.
 **
 ** Definition of DCM_SVCID_RESPONSEONEVENT.
 */
#define DCM_SVCID_RESPONSEONEVENT 0xA7U

#if (defined DCM_SVCID_READDIDDATA)
#error DCM_SVCID_READDIDDATA already defined
#endif /* if (defined DCM_SVCID_READDIDDATA) */

/** \brief Internal API service ID.
 **
 ** Definition of DCM_SVCID_READDIDDATA.
 */
#define DCM_SVCID_READDIDDATA 0x40U


#if (defined DCM_SVCID_WRITEDIDDATA)
#error DCM_SVCID_WRITEDIDDATA already defined
#endif /* if (defined DCM_SVCID_WRITEDIDDATA) */

/** \brief Internal API service ID.
 **
 ** Definition of DCM_SVCID_WRITEDIDDATA.
 */
#define DCM_SVCID_WRITEDIDDATA 0x41U

#if (defined DCM_SVCID_WRITEDATABYIDENTIFIER)
#error DCM_SVCID_WRITEDATABYIDENTIFIER already defined
#endif /* if (defined DCM_SVCID_WRITEDATABYIDENTIFIER) */

/** \brief Internal API service ID for Dcm_Dsp_WriteDataByIdentifier_SvcH.
 **
 ** Definition of DCM_SVCID_WRITEDATABYIDENTIFIER.
 */
#define DCM_SVCID_WRITEDATABYIDENTIFIER 0xA8U

#if (defined DCM_SVCID_ROUTINECONTROL)
#error DCM_SVCID_ROUTINECONTROL already defined
#endif /* if (defined DCM_SVCID_ROUTINECONTROL) */

/** \brief Internal API service ID for Dcm_Dsp_RoutineControl_SvcH.
 **
 ** Definition of DCM_SVCID_ROUTINECONTROL.
 */
#define DCM_SVCID_ROUTINECONTROL 0xA9U

#if (defined DCM_SVCID_ROUTINECONTROLSTART)
#error DCM_SVCID_ROUTINECONTROLSTART already defined
#endif /* if (defined DCM_SVCID_ROUTINECONTROLSTART) */

/** \brief Internal API service ID for Dcm_Dsp_RoutineControl_StartRoutine_SSvcH.
 **
 ** Definition of DCM_SVCID_ROUTINECONTROLSTART.
 */
#define DCM_SVCID_ROUTINECONTROLSTART 0xAAU

#if (defined DCM_SVCID_ROUTINECONTROLSTOP)
#error DCM_SVCID_ROUTINECONTROLSTOP already defined
#endif /* if (defined DCM_SVCID_ROUTINECONTROLSTOP) */

/** \brief Internal API service ID for Dcm_Dsp_RoutineControl_StopRoutine_SSvcH.
 **
 ** Definition of DCM_SVCID_ROUTINECONTROLSTOP.
 */
#define DCM_SVCID_ROUTINECONTROLSTOP 0xABU

#if (defined DCM_SVCID_ROUTINECONTROLROUTINERESULTS)
#error DCM_SVCID_ROUTINECONTROLROUTINERESULTS already defined
#endif /* if (defined DCM_SVCID_ROUTINECONTROLROUTINERESULTS) */

/** \brief Internal API service ID for Dcm_Dsp_RoutineControl_RequestResults_SSvcH.
 **
 ** Definition of DCM_SVCID_ROUTINECONTROLROUTINERESULTS.
 */
#define DCM_SVCID_ROUTINECONTROLROUTINERESULTS 0xACU

#if (defined DCM_SVCID_INJECTOR)
#error DCM_SVCID_INJECTOR already defined
#endif /* if (defined DCM_SVCID_INJECTOR) */

/** \brief Internal API service ID for Dcm_InjectDiagnosticRequest.
 **
 ** Definition of DCM_SVCID_INJECTOR.
 */
#define DCM_SVCID_INJECTOR 0xADU

#if (defined DCM_SVCID_ROUTINECONTROLSTART_DEFAULTINTERFACE)
#error DCM_SVCID_ROUTINECONTROLSTART_DEFAULTINTERFACE already defined
#endif /* if (defined DCM_SVCID_ROUTINECONTROLSTART_DEFAULTINTERFACE) */

/** \brief Internal API service ID for Dcm_DefaultRoutineStart.
 **
 ** Definition of DCM_SVCID_ROUTINECONTROLSTART_DEFAULTINTERFACE.
 */
#define DCM_SVCID_ROUTINECONTROLSTART_DEFAULTINTERFACE 0xAEU

#if (defined DCM_SVCID_ROUTINECONTROLSTOP_DEFAULTINTERFACE)
#error DCM_SVCID_ROUTINECONTROLSTOP_DEFAULTINTERFACE already defined
#endif /* if (defined DCM_SVCID_ROUTINECONTROLSTOP_DEFAULTINTERFACE) */

/** \brief Internal API service ID for Dcm_DefaultRoutineStop.
 **
 ** Definition of DCM_SVCID_ROUTINECONTROLSTOP_DEFAULTINTERFACE.
 */
#define DCM_SVCID_ROUTINECONTROLSTOP_DEFAULTINTERFACE 0xAFU

#if (defined DCM_SVCID_ROUTINECONTROLROUTINERESULTS_DEFAULTINTERFACE)
#error DCM_SVCID_ROUTINECONTROLROUTINERESULTS_DEFAULTINTERFACE already defined
#endif /* if (defined DCM_SVCID_ROUTINECONTROLROUTINERESULTS_DEFAULTINTERFACE) */

/** \brief Internal API service ID for Dcm_DefaultRoutineRequestRoutineResults.
 **
 ** Definition of DCM_SVCID_ROUTINECONTROLROUTINERESULTS_DEFAULTINTERFACE.
 */
#define DCM_SVCID_ROUTINECONTROLROUTINERESULTS_DEFAULTINTERFACE 0xB0U

#if (defined DCM_SERVID_DEFAULTDIDREAD_DEFAULTINTERFACE)
#error DCM_SERVID_DEFAULTDIDREAD_DEFAULTINTERFACE already defined
#endif /* if (defined DCM_SERVID_DEFAULTDIDREAD_DEFAULTINTERFACE) */

/** \brief Internal API service ID for reading unconfigured DIDs.
 **
 ** Definition of DCM_SERVID_DEFAULTDIDREAD_DEFAULTINTERFACE.
 */
#define DCM_SERVID_DEFAULTDIDREAD_DEFAULTINTERFACE 0xB1U

#if (defined DCM_SERVID_DEFAULTDIDWRITE_DEFAULTINTERFACE)
#error DCM_SERVID_DEFAULTDIDWRITE_DEFAULTINTERFACE already defined
#endif /* if (defined DCM_SERVID_DEFAULTDIDWRITE_DEFAULTINTERFACE) */

/** \brief Internal API service ID for writing unconfigured DIDs.
 **
 ** Definition of DCM_SERVID_DEFAULTDIDWRITE_DEFAULTINTERFACE.
 */
#define DCM_SERVID_DEFAULTDIDWRITE_DEFAULTINTERFACE 0xB2U

#if (defined DCM_SERVID_DEFAULTDIDRESETTODEFAULT_DEFAULTINTERFACE)
#error DCM_SERVID_DEFAULTDIDRESETTODEFAULT_DEFAULTINTERFACE already defined
#endif /* if (defined DCM_SERVID_DEFAULTDIDRESETTODEFAULT_DEFAULTINTERFACE) */

/** \brief Internal API service ID for resetting unconfigured DIDs to default control.
 **
 ** Definition of DCM_SERVID_DEFAULTDIDRESETTODEFAULT_DEFAULTINTERFACE.
 */
#define DCM_SERVID_DEFAULTDIDRESETTODEFAULT_DEFAULTINTERFACE 0xB3U

#if (defined DCM_SERVID_DEFAULTDIDRETURNCONTROLTOECU_DEFAULTINTERFACE)
#error DCM_SERVID_DEFAULTDIDRETURNCONTROLTOECU_DEFAULTINTERFACE already defined
#endif /* if (defined DCM_SERVID_DEFAULTDIDRETURNCONTROLTOECU_DEFAULTINTERFACE) */

/** \brief Internal API service ID for returning unconfigured DIDs to ECU control.
 **
 ** Definition of DCM_SERVID_DEFAULTDIDRETURNCONTROLTOECU_DEFAULTINTERFACE.
 */
#define DCM_SERVID_DEFAULTDIDRETURNCONTROLTOECU_DEFAULTINTERFACE 0xB3U

#if (defined DCM_SERVID_DEFAULTDIDRETURNCONTROLTOECUFORALL_DEFAULTINTERFACE)
#error DCM_SERVID_DEFAULTDIDRETURNCONTROLTOECUFORALL_DEFAULTINTERFACE already defined
#endif /* if (defined DCM_SERVID_DEFAULTDIDRETURNCONTROLTOECUFORALL_DEFAULTINTERFACE) */

/** \brief Internal API service ID for returning all unconfigured DIDs to ECU control.
 **
 ** Definition of DCM_SERVID_DEFAULTDIDRETURNCONTROLTOECUFORALL_DEFAULTINTERFACE.
 */
#define DCM_SERVID_DEFAULTDIDRETURNCONTROLTOECUFORALL_DEFAULTINTERFACE 0xB4U

#if (defined DCM_SERVID_DEFAULTDIDFREEZECURRENTSTATE_DEFAULTINTERFACE)
#error DCM_SERVID_DEFAULTDIDFREEZECURRENTSTATE_DEFAULTINTERFACE already defined
#endif /* if (defined DCM_SERVID_DEFAULTDIDFREEZECURRENTSTATE_DEFAULTINTERFACE) */

/** \brief Internal API service ID for freezing unconfigured DIDs to current control state.
 **
 ** Definition of DCM_SERVID_DEFAULTDIDFREEZECURRENTSTATE_DEFAULTINTERFACE.
 */
#define DCM_SERVID_DEFAULTDIDFREEZECURRENTSTATE_DEFAULTINTERFACE 0xB5U

#if (defined DCM_SERVID_DEFAULTDIDSHORTTERMADJUSTMENT_DEFAULTINTERFACE)
#error DCM_SERVID_DEFAULTDIDSHORTTERMADJUSTMENT_DEFAULTINTERFACE already defined
#endif /* if (defined DCM_SERVID_DEFAULTDIDSHORTTERMADJUSTMENT_DEFAULTINTERFACE) */

/** \brief Internal API service ID for short term adjustment of control state of unconfigured DIDs.
 **
 ** Definition of DCM_SERVID_DEFAULTDIDSHORTTERMADJUSTMENT_DEFAULTINTERFACE.
 */
#define DCM_SERVID_DEFAULTDIDSHORTTERMADJUSTMENT_DEFAULTINTERFACE 0xB5U

#if (defined DCM_SVCID_ROUTINECONTROLADDINFOBYTE)
#error DCM_SVCID_ROUTINECONTROLADDINFOBYTE already defined
#endif /* if (defined DCM_SVCID_INJECTOR) */

/** \brief API to add one more byte to the successful response of a requested routine.
 **
 ** Definition of DCM_SVCID_ROUTINECONTROLADDINFOBYTE.
 */
#define DCM_SVCID_ROUTINECONTROLADDINFOBYTE 0xAEU

/** \brief Internal API service ID for Dcm_Dsp_DemFacade_ClearDTCInfoSetup.
 **
 ** Definition of DCM_SVCID_CLEARDTCINFOSETUP.
 */
#if (defined DCM_SVCID_CLEARDTCINFOSETUP)
  #error "DCM_SVCID_CLEARDTCINFOSETUP is already defined"
#endif
#define DCM_SVCID_CLEARDTCINFOSETUP 0xB1U

/** \brief Internal API service ID for  Dcm_Dsp_DemFacade_ClearDTCInfo_Process().
 **
 ** Definition of DCM_SVCID_CLEARDTCINFOPROCESS.
 */
#if (defined DCM_SVCID_CLEARDTCINFOPROCESS)
  #error "DCM_SVCID_CLEARDTCINFOPROCESS is already defined"
#endif
#define DCM_SVCID_CLEARDTCINFOPROCESS 0xB2U

#if (defined DCM_SVCID_GETINFOTYPEVALUEDATA)
#error DCM_SVCID_GETINFOTYPEVALUEDATA already defined
#endif /* if (defined DCM_SVCID_GETINFOTYPEVALUEDATA) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of DCM_SVCID_GETINFOTYPEVALUEDATA.
 */
#define DCM_SVCID_GETINFOTYPEVALUEDATA 0x60U

#if (defined DCM_DET_REPORT_ERROR)
  #error "DCM_DET_REPORT_ERROR is already defined"
#endif
/** \brief DCM_DET_REPORT_ERROR() determines whether errors are detected
 **
 ** In development mode, this macro evaluates to its parameter, which is a boolean value with
 ** TRUE indicating an error.
 **
 ** In production mode this macro always evaluates to 0, thus disabling error detection.
 */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
#define DCM_DET_REPORT_ERROR(ServiceId,ErrorCode)                       \
  ((void)Det_ReportError(DCM_MODULE_ID, DCM_INSTANCE_ID, (ServiceId), (ErrorCode)))
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */

#if (defined DCM_DTC_AND_STATUS_SNGL_RCD_LEN)
#error DCM_DTC_AND_STATUS_SNGL_RCD_LEN already defined
#endif /* if (defined DCM_DTC_AND_STATUS_SNGL_RCD_LEN) */

/** \brief Definition of length of single DTC and Status record in response
     message of Service 0x19 - Subfunction 0x02(Report DTC by status mask) */
#define DCM_DTC_AND_STATUS_SNGL_RCD_LEN (0x04U)

#if (defined DCM_DTC_SNAPSHOT_SNGL_RCD_LEN)
#error DCM_DTC_SNAPSHOT_RECORD_SNGL_RCD_LEN already defined
#endif /* if (defined DCM_DTC_SNAPSHOT_SNGL_RCD_LEN) */

/** \brief Definition of length of single DTC Snapshot record in response
     message of Service 0x19 - Subfunction 0x03(Report DTC Snapshot Record Identification) */
#define DCM_DTC_SNAPSHOT_SNGL_RCD_LEN (0x04U)

#if (defined DCM_DTC_EXT_RECORD_NUM_LEN)
#error DCM_DTC_EXT_RECORD_NUM_LEN already defined
#endif /* if (defined DCM_DTC_EXT_RECORD_NUM_LEN) */

/** \brief Definition of length of DTC Extended record number */
#define DCM_DTC_EXT_RECORD_NUM_LEN (0x01U)

#if (defined DCM_NUM_STD_SERVICES)
#error DCM_NUM_STD_SERVICES already defined
#endif /* if (defined DCM_NUM_STD_SERVICES) */

/** \brief Definition of number of DCM services which do not
     access RTE interface  */
#define DCM_NUM_STD_SERVICES (0x02U)

/* Macro for Read Event Data services ID */
#if (defined DCM_DSP_OBD_SERVICE_0X01)
  #error "DCM_DSP_OBD_SERVICE_0X01 is already defined"
#endif
#define DCM_DSP_OBD_SERVICE_0X01             (0x01U)

#if (defined DCM_DSP_OBD_SERVICE_0X02)
  #error "DCM_DSP_OBD_SERVICE_0X02 is already defined"
#endif
#define DCM_DSP_OBD_SERVICE_0X02             (0x02U)

#if (defined DCM_DSP_OBD_SERVICE_0X03)
  #error "DCM_DSP_OBD_SERVICE_0X03 is already defined"
#endif
#define DCM_DSP_OBD_SERVICE_0X03             (0x03U)

#if (defined DCM_DSP_OBD_SERVICE_0X06)
  #error "DCM_DSP_OBD_SERVICE_0X06 is already defined"
#endif
#define DCM_DSP_OBD_SERVICE_0X06             (0x06U)

#if (defined DCM_DSP_OBD_SERVICE_0X07)
  #error "DCM_DSP_OBD_SERVICE_0X07 is already defined"
#endif
#define DCM_DSP_OBD_SERVICE_0X07             (0x07U)

#if (defined DCM_DSP_OBD_SERVICE_0X08)
  #error "DCM_DSP_OBD_SERVICE_0X08 is already defined"
#endif
#define DCM_DSP_OBD_SERVICE_0X08             (0x08U)

#if (defined DCM_DSP_OBD_SERVICE_0X0A)
  #error "DCM_DSP_OBD_SERVICE_0X0A is already defined"
#endif
#define DCM_DSP_OBD_SERVICE_0X0A             (0x0AU)

#if (defined DCM_SID_ECU_RESET)
#error DCM_SID_ECU_RESET already defined
#endif /* if (defined DCM_SID_ECU_RESET) */

/** \brief Definition of service id of Ecu Reset service */
#define DCM_SID_ECU_RESET (0x11U)

#if (defined DCM_SID_READ_DTC_INFORMATION)
#error DCM_SID_READ_DTC_INFORMATION already defined
#endif /* if (defined DCM_SID_READ_DTC_INFORMATION) */

/** \brief Definition of service id of ReadDTCInformation */
#define DCM_SID_READ_DTC_INFORMATION (0x19U)

#if (defined DCM_SID_CLR_DIAG_INFORMATION)
#error DCM_SID_CLR_DIAG_INFORMATION already defined
#endif /* if (defined DCM_SID_CLR_DIAG_INFORMATION) */

/** \brief Definition of service id of ClearDiagnosticInformation */
#define DCM_SID_CLR_DIAG_INFORMATION (0x14U)

#if (defined DCM_SID_DCM_CONFIRMATION)
#error DCM_SID_DCM_CONFIRMATION already defined
#endif /* if (defined DCM_SID_CLR_DIAG_INFORMATION) */

/** \brief Definition of service id of Dcm_Confirmation callout */
#define DCM_SID_DCM_CONFIRMATION (0x29U)

#if (defined DCM_SID_CONTROLDTCSETTING)
#error DCM_SID_CONTROLDTCSETTING already defined
#endif /* if (defined DCM_SID_CONTROLDTCSETTING) */
/** \brief Definition of service id of UDS service ControlDTCSetting */
#define DCM_SID_CONTROLDTCSETTING (0x85U)

#if (defined DCM_SSID_CONTROLDTCSETTING_ON)
#error DCM_SSID_CONTROLDTCSETTING_ON already defined
#endif /* if (defined DCM_SSID_CONTROLDTCSETTING_ON) */
/** \brief Definition of subservice id of UDS service ControlDTCSetting ON */
#define DCM_SSID_CONTROLDTCSETTING_ON (0x01U)

#if (defined DCM_SSID_CONTROLDTCSETTING_OFF)
#error DCM_SSID_CONTROLDTCSETTING_OFF already defined
#endif /* if (defined DCM_SSID_CONTROLDTCSETTING_OFF) */
/** \brief Definition of subservice id of UDS service ControlDTCSetting OFF */
#define DCM_SSID_CONTROLDTCSETTING_OFF (0x02U)

#if (defined DCM_SID_ROUTINE_CONTROL)
#error DCM_SID_ROUTINE_CONTROL already defined
#endif /* if (defined DCM_SID_ROUTINE_CONTROL) */
/** \brief Definition of service id of UDS service RoutineControl */
#define DCM_SID_ROUTINE_CONTROL (0x31U)

#if (defined DCM_INVALID_SESSION)
#error DCM_INVALID_SESSION already defined
#endif/* #if (defined DCM_INVALID_SESSION) */

/** \brief Invalid value for Diagnostics Session entered */
#define DCM_INVALID_SESSION 0U

#if (defined DCM_FUNCTIONAL_TYPE)
#error DCM_FUNCTIONAL_TYPE already defined
#endif /* if (defined DCM_FUNCTIONAL_TYPE) */

/** \brief Definition of value for functional request */
#define DCM_FUNCTIONAL_TYPE 1U

#if (defined DCM_PHYSICAL_TYPE)
#error DCM_PHYSICAL_TYPE already defined
#endif /* if (defined DCM_PHYSICAL_TYPE) */

 /** \brief Definition of value for physical request */
#define DCM_PHYSICAL_TYPE   0U

#if (defined DCM_SES_LVL_NON_DEF)
#error DCM_SES_LVL_NON_DEF already defined
#endif /* if (defined DCM_SES_LVL_NON_DEF) */

/** \brief Value for a non-default session level */
#define DCM_SES_LVL_NON_DEF 0x00U

#if (defined DCM_MESSAGE_TYPE_MASK)
#error DCM_MESSAGE_TYPE_MASK already defined
#endif/* #if (defined DCM_MESSAGE_TYPE_MASK) */

/** \brief Mask to get the Message type from 'CommunicationType' parameter */
#define DCM_MESSAGE_TYPE_MASK 0x03U

#if (defined DCM_SUBNET_NUMBER_MASK)
#error DCM_SUBNET_NUMBER_MASK already defined
#endif/* #if (defined DCM_SUBNET_NUMBER_MASK) */

/** \brief Mask to get the subnet number from 'CommunicationType' parameter */
#define DCM_SUBNET_NUMBER_MASK 0xF0U

#if (DCM_NUM_PROTOCOL > 1U)
#if (defined DCM_HSM_INFO)
  #error "DCM_HSM_INFO is already defined"
#endif
#define DCM_HSM_INFO(a)                   Dcm_HsmInfo[(a)]
#if (defined DCM_PROTOCOL_CONFIG)
  #error "DCM_PROTOCOL_CONFIG is already defined"
#endif
#define DCM_PROTOCOL_CONFIG(a)                 Dcm_ProtocolConfig[(a)]
/* Deviation MISRAC2012-1 <START> */
/* Deviation MISRA-1 */
#if (defined DCM_PL_START_TIMER)
  #error "DCM_PL_START_TIMER is already defined"
#endif
#define DCM_PL_START_TIMER(a, b)          a, b
/* Deviation MISRA-1 */
#if (defined DCM_PDL_DISPATCH_SERVICE_REQ)
  #error "DCM_PDL_DISPATCH_SERVICE_REQ is already defined"
#endif
#define DCM_PDL_DISPATCH_SERVICE_REQ(a)   a
/* Deviation MISRA-1 */
#if (defined DCM_CUR_INST_ID)
  #error "DCM_CUR_INST_ID is already defined"
#endif
#define DCM_CUR_INST_ID(a)                a
/* Deviation MISRAC2012-1 <STOP> */
#if (defined DCM_SES_TAB_CONFIG)
  #error "DCM_SES_TAB_CONFIG is already defined"
#endif
#define DCM_SES_TAB_CONFIG(a)             Dcm_SesTabConfig[(a)]
#else
#if (defined DCM_HSM_INFO)
  #error "DCM_HSM_INFO is already defined"
#endif
#define DCM_HSM_INFO(a)                   Dcm_HsmInfo[0]
#if (defined DCM_PROTOCOL_CONFIG)
  #error "DCM_PROTOCOL_CONFIG is already defined"
#endif
#define DCM_PROTOCOL_CONFIG(a)                 Dcm_ProtocolConfig[0]
/* Deviation MISRAC2012-1 */
/* Deviation MISRA-1 */
#if (defined DCM_PL_START_TIMER)
  #error "DCM_PL_START_TIMER is already defined"
#endif
#define DCM_PL_START_TIMER(a, b)          b
#if (defined DCM_PDL_DISPATCH_SERVICE_REQ)
  #error "DCM_PDL_DISPATCH_SERVICE_REQ is already defined"
#endif
#define DCM_PDL_DISPATCH_SERVICE_REQ(a)   void
#if (defined DCM_CUR_INST_ID)
  #error "DCM_CUR_INST_ID is already defined"
#endif
#define DCM_CUR_INST_ID(a)                0
#if (defined DCM_SES_TAB_CONFIG)
  #error "DCM_SES_TAB_CONFIG is already defined"
#endif
#define DCM_SES_TAB_CONFIG(a)             Dcm_SesTabConfig[0]
#endif

/* Wrapper function used for parameter typecasting
 * while accessing DCM_HSMEMITINST. */
#if (defined DCM_HSMEMITINST_WRAPPER)
  #error "DCM_HSMEMITINST_WRAPPER is already defined"
#endif
#define DCM_HSMEMITINST_WRAPPER(a,b,c)\
  DCM_HSMEMITINST((a),((uint8)(b)),(c))

#if (defined DCM_HSMEMITTOSELFINST_WRAPPER)
  #error "DCM_HSMEMITTOSELFINST_WRAPPER is already defined"
#endif
#define DCM_HSMEMITTOSELFINST_WRAPPER(a,b,c)\
  DCM_HSMEMITTOSELFINST((a),((uint8)(b)),(c))

/* Macro to extract lowest nibble from a byte data */
#if (defined DCM_MASK_NIBBLE)
  #error "DCM_MASK_NIBBLE is already defined"
#endif
#define DCM_MASK_NIBBLE                     (0x0FU)

/* Macro to extract lowest byte from a data */
#if (defined DCM_MASK_BYTE)
  #error "DCM_MASK_BYTE is already defined"
#endif
#define DCM_MASK_BYTE                       (0xFFU)

/* Macro for number of bits in a nibble */
#if (defined DCM_NIBBLE_BITS)
  #error "DCM_NIBBLE_BITS is already defined"
#endif
#define DCM_NIBBLE_BITS                     (0x04U)

/* Macro for number of bits in a byte */
#if (defined DCM_BYTE_BITS)
  #error "DCM_BYTE_BITS is already defined"
#endif
#define DCM_BYTE_BITS                       (0x08U)

/* Macro for maximum number of bytes in an address */
#if (defined DCM_MAX_ADDRESS_BYTES)
  #error "DCM_MAX_ADDRESS_BYTES is already defined"
#endif
#define DCM_MAX_ADDRESS_BYTES               (0x04U)

/* Macro for maximum number of bytes in an address */
#if (defined DCM_MAX_MEMSIZE_BYTES)
  #error "DCM_MAX_MEMSIZE_BYTES is already defined"
#endif
#define DCM_MAX_MEMSIZE_BYTES               (0x04U)

/* Macro for not configured EcuC PduID */
#if (defined DCM_NOT_CONFIGURED_ECUC_PDUID)
  #error "DCM_NOT_CONFIGURED_ECUC_PDUID is already defined"
#endif
#define DCM_NOT_CONFIGURED_ECUC_PDUID               (0x0U)

/*==================[type definitions]===========================================================*/

/** \brief Type definition for time base of the Dcm Module */
typedef uint16 Dcm_TimeBaseConfigType;

/** \brief Different types of timing parameters
 **
 ** This type is used for the INTERNAL Dcm_GetSesTimingValues() to get values of configured default
 ** timing parameters, values of current active timing parameters or values of configured timing
 ** parameter limits. */

#if (defined DCM_DEFAULT)
  #error "DCM_DEFAULT is already defined"
#endif
#define DCM_DEFAULT 0U

#if (defined DCM_CURRENT)
  #error "DCM_CURRENT is already defined"
#endif
#define DCM_CURRENT 1U

#if (defined DCM_LIMITS)
  #error "DCM_LIMITS is already defined"
#endif
#define DCM_LIMITS  2U

typedef uint8 Dcm_TimerModeType;

/* !LINKSTO Dcm549,1 */
/** \brief Type to indicate whether diagnostic session allows to jump to Bootloader */

/** \brief Diagnostic session doesn't allow to jump to Bootloader */
#if (defined DCM_NO_BOOT)
  #error "DCM_NO_BOOT is already defined"
#endif
#define DCM_NO_BOOT 0U

/** \brief Diagnostic session allows to jump to OEM Bootloader */
#if (defined DCM_OEM_BOOT)
  #error "DCM_OEM_BOOT is already defined"
#endif
#define DCM_OEM_BOOT 1U

/** \brief Diagnostic session allows to jump to System Supplier Bootloader */
#if (defined DCM_SYS_BOOT)
  #error "DCM_SYS_BOOT is already defined"
#endif
#define DCM_SYS_BOOT 2U

typedef uint8 Dcm_SessionForBootType;

/* !LINKSTO Dcm549,1 */
/** \brief Type of messages handled by Dcm */

/** \brief ROE */
#if (defined DCM_ROE)
  #error "DCM_ROE is already defined"
#endif
#define DCM_ROE       0U

/** \brief ROE final response*/
#if (defined DCM_ROE_FINAL)
  #error "DCM_ROE_FINAL is already defined"
#endif
#define DCM_ROE_FINAL 1U

/** \brief Periodic Responses */
#if (defined  DCM_PERIODIC_RESPONSE)
  #error "DCM_PERIODIC_RESPONSE is already defined"
#endif
#define  DCM_PERIODIC_RESPONSE        2U

/** \brief Other Normal messages */
#if (defined DCM_NORMAL)
  #error "DCM_NORMAL is already defined"
#endif
#define DCM_NORMAL    3U

/** \brief No kind of message */
#if (defined DCM_NONE)
  #error "DCM_NONE is already defined"
#endif
#define DCM_NONE      4U

typedef uint8 Dcm_MessageType;

/* !LINKSTO Dcm549,1 */
/** \brief Entry of a session table */
typedef struct
{
  /** \brief Extended Timeout for sending response pending message in this session
   * (P2*ServerMax) */
  uint32 P2StarMaxTime;
  /** \brief Timeout for sending response pending message in this session (P2ServerMax) */
  uint16 P2MaxTime;
#if (DCM_DIAG_SES_RESP_PEND_ENABLED == STD_ON)
  /** \brief Maximum number of ResponsesPending NRC (0x78) possible in this session */
  uint16 NumResponsePending;
#endif
  /** \brief Parameter indicating whether diagnostic session allows to jump to Bootloader */
  Dcm_SessionForBootType SessionForBoot;
  /** \brief Diagnostic session type */
  Dcm_SesCtrlType SesCtrl;
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
  /** \brief Diagnostic session type generated by Rte */
  uint8 RteSesMode;
#endif
}
Dcm_SesTabEntryConfigType;

/** \brief Table of available session configurations */
typedef struct
{
  /** \brief Pointer to array of session level configuration information */
  P2CONST(Dcm_SesTabEntryConfigType, TYPEDEF, DCM_CONST) SessionEntries;
  /** \brief Number of configured sessions */
  uint8 NumEntries;
  /** \brief Index to default session */
  uint8 DefSessionIndex;
}
Dcm_SesTabConfigType;

/** \brief Type of external service functions for diagnostic requests */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ExternalServiceFunctionType)(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

typedef P2FUNC(boolean, DCM_APPL_CODE, Dcm_ModeRuleFunctionType)
  (P2VAR(uint8, AUTOMATIC, DCM_VAR) Nrc);

/** \brief Type of external Subservice functions  */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ExternalSubServiceFunctionType)(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Type of callback function used to confirm a transmission
 ** to the diagnostic application */
typedef P2FUNC(void, DCM_APPL_CODE, Dcm_DcmConfirmationType)(
  Dcm_IdContextType idContext, PduIdType dcmRxPduId, Dcm_ConfirmationStatusType status);

/** \brief Type of process page function */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ProcessPageFuncType)(
  Dcm_DemClientIdType DemClientId,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Type of internal service functions for diagnostic requests */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_InternalServiceFunctionType)(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Type of service handler functions for diagnostic requests */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_GenericServiceHandlerType)(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Type of internal Subservice functions */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_InternalSubServiceFunctionType)(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief This type contains configuration for a single Subservice */
typedef struct
{
  /** \brief Pointer to array of allowed sub service sessions */
  P2CONST(Dcm_SesCtrlType, TYPEDEF, DCM_CONST) SubSesCtrls;

  /** \brief Pointer to array of allowed sub service security levels */
  P2CONST(Dcm_SecLevelType, TYPEDEF, DCM_CONST) SubSecLevels;

  /** \brief Pointer to External Subservice function */
  Dcm_ExternalSubServiceFunctionType ExtSrvHandler;

  /** \brief Pointer to Internal Subservice function */
  Dcm_InternalSubServiceFunctionType IntSrvHandler;

  /** \brief Pointer to Mode rule function */
  Dcm_ModeRuleFunctionType ModeRuleSubSrvFunction;

  /** \brief Subservice id */
  uint8 SubServiceId;

  /** \brief Number of sessions types for which this subservice may be executed */
  uint8 NumSubSesCtrls;

  /** \brief Number of security levels for which this subservice is allowed */
  uint8 NumSubSecLevels;
}
Dcm_SubSidTabEntryConfigType;

/** \brief This type contains configuration for a single service ID */
typedef struct
{
  /** \brief Pointer to array of allowed sessions */
  P2CONST(Dcm_SesCtrlType, TYPEDEF, DCM_CONST) SesCtrls;

  /** \brief Pointer to array of allowed security levels */
  P2CONST(Dcm_SecLevelType, TYPEDEF, DCM_CONST) SecLevels;

  /** \brief Pointer to array of Subservices configuration */
  P2CONST(Dcm_SubSidTabEntryConfigType, TYPEDEF, DCM_CONST) SubServices;

  /** \brief Pointer to External service function */
  Dcm_ExternalServiceFunctionType ExtSrvHandler;

  /** \brief Pointer to Internal service function */
  Dcm_InternalServiceFunctionType IntSrvHandler;

  /** \brief Pointer to Mode rule function */
  Dcm_ModeRuleFunctionType ModeRuleSrvFunction;

  /** \brief Service id of this entry */
  uint8 ServiceId;

  /** \brief Number of sessions types for which this service may be executed */
  uint8 NumSesCtrls;

  /** \brief Number of security levels for which this service is allowed */
  uint8 NumSecLevels;

  /** \brief Number of subservices of this service */
  uint8 NumSubServices;

  /** \brief subfunction is available */
  boolean SubfuncAvail;

  /** \brief Asynchronous execution of Service is available */
  boolean AsyncServiceExecution;
}
Dcm_SidTabEntryConfigType;

/** \brief 'Response Pending' transmission status */

/* No 'Response Pending' transmission is active */
#if (defined DCM_RP_NO_DATA)
  #error "DCM_RP_NO_DATA is already defined"
#endif
#define DCM_RP_NO_DATA 0U

/* 'Response Pending' Tx Confirmation is pending */
#if (defined DCM_RP_PENDING_CONF)
  #error "DCM_RP_PENDING_CONF is already defined"
#endif
#define DCM_RP_PENDING_CONF 1U

/* Tx Confirmation received for 'Response Pending' */
#if (defined DCM_RP_CONFIRMED_TX)
  #error "DCM_RP_CONFIRMED_TX is already defined"
#endif
#define DCM_RP_CONFIRMED_TX 2U

typedef uint8 Dcm_RpTxStatusType;

/** \brief State of the jump to bootloader process */

/* There is no jump to the bootloader in progress or a jump is in preparation. */
#if (defined DCM_PREPARE_JUMP)
  #error "DCM_PREPARE_JUMP is already defined"
#endif
#define DCM_PREPARE_JUMP 0U

/* An immediate RequestCorrectlyReceivedResponsePending NRC needs to be transmitted and
   the Dcm is waiting for the confirmation of this transmission from the PduR. */
#if (defined DCM_FIRST_RCRRP_PENDING)
  #error "DCM_FIRST_RCRRP_PENDING is already defined"
#endif
#define DCM_FIRST_RCRRP_PENDING 1U

/* All prerequisites for a bootloader jump are fulfilled, the Programming Conditions are being
   set and in case this setting is successful, a reboot will follow. */
#if (defined DCM_JUMP_IN_PROGRESS)
  #error "DCM_JUMP_IN_PROGRESS is already defined"
#endif
#define DCM_JUMP_IN_PROGRESS 2U

/** \brief This type defines the type of cancellation that a Protocol is undergoing.
 *         This also defines which actions are necessary for the cancellation operation to be
 *         considered complete.
 */

/** \brief Generic invalid cancellation operation type. */
#if (defined DCM_CANCEL_INVALID)
  #error "DCM_CANCEL_INVALID is already defined"
#endif
#define DCM_CANCEL_INVALID 0U

/** \brief Normal (non-Page-Buffered) transmission is being cancelled. */
#if (defined DCM_CANCEL_NORMAL_SENDING)
  #error "DCM_CANCEL_NORMAL_SENDING is already defined"
#endif
#define DCM_CANCEL_NORMAL_SENDING 1U

/** \brief Page-Buffered transmission is being cancelled. */
#if (defined DCM_CANCEL_PG_BUF_SENDING)
  #error "DCM_CANCEL_PG_BUF_SENDING is already defined"
#endif
#define DCM_CANCEL_PG_BUF_SENDING 2U

/** \brief The processing of an ROE event-triggered response is being cancelled. */
#if (defined DCM_CANCEL_ROE_PROCESSING)
  #error "DCM_CANCEL_ROE_PROCESSING is already defined"
#endif
#define DCM_CANCEL_ROE_PROCESSING 3U

/** \brief The processing of a normal response is being cancelled and no current
 **        transmission needs to end as well. */
#if (defined DCM_CANCEL_NORMAL_PROCESSING_NO_TX)
  #error "DCM_CANCEL_NORMAL_PROCESSING_NO_TX is already defined"
#endif
#define DCM_CANCEL_NORMAL_PROCESSING_NO_TX 4U

/** \brief The processing of a normal response is being cancelled and an ongoing
 **        transmission needs to end as well. */
#if (defined DCM_CANCEL_NORMAL_PROCESSING_TX)
  #error "DCM_CANCEL_NORMAL_PROCESSING_TX is already defined"
#endif
#define DCM_CANCEL_NORMAL_PROCESSING_TX 5U

typedef uint8 Dcm_CancellationType;

/** \brief This type expresses the type of operation that has been successfully performed
 *         as a part of the cancellation of a protocol.
 */

/** \brief Generic invalid cancellation type. */
#if (defined DCM_INVALID_CANCELLATION)
  #error "DCM_INVALID_CANCELLATION is already defined"
#endif
#define DCM_INVALID_CANCELLATION 0U

/** \brief A service processing has been cancelled successfully (the pending interfaces have
 **        been called with a DCM_CANCEL OpStatus) */
#if (defined DCM_SERVICE_CANCELLATION)
  #error "DCM_SERVICE_CANCELLATION is already defined"
#endif
#define DCM_SERVICE_CANCELLATION 1U

/** \brief An ongoing transmission has been ended successfully. */
#if (defined DCM_TRANSMISSION_CANCELLATION)
  #error "DCM_TRANSMISSION_CANCELLATION is already defined"
#endif
#define DCM_TRANSMISSION_CANCELLATION 2U

typedef uint8 Dcm_CancellationConfirmationType;

/** \brief This type defines the state of the cancellation of a running Asynchronous
 *         Service Handler.
 */

/** \brief Generic invalid cancellation type. */
#if (defined DCM_ASYNC_SERVICE_HANDLER_INVALID_CANCELLATION)
  #error "DCM_ASYNC_SERVICE_HANDLER_INVALID_CANCELLATION is already defined"
#endif
#define DCM_ASYNC_SERVICE_HANDLER_INVALID_CANCELLATION 0U

/** \brief Reattempt to initiate the cancellation of the Asynchronous Service Handler
 **        in the next Dcm_MainFunction() cycle. */
#if (defined DCM_ASYNC_SERVICE_HANDLER_CANCEL_IN_NEXT_CYCLE)
  #error "DCM_ASYNC_SERVICE_HANDLER_CANCEL_IN_NEXT_CYCLE is already defined"
#endif
#define DCM_ASYNC_SERVICE_HANDLER_CANCEL_IN_NEXT_CYCLE 1U

/** \brief The cancellation of the Asynchronous Service Handler has been initiated and
 **        is pending a result from the Asynchronous Service Handler */
#if (defined DCM_ASYNC_SERVICE_HANDLER_CANCELLATION_PENDING)
  #error "DCM_ASYNC_SERVICE_HANDLER_CANCELLATION_PENDING is already defined"
#endif
#define DCM_ASYNC_SERVICE_HANDLER_CANCELLATION_PENDING 2U

typedef uint8 Dcm_RetryAsyncServiceCancellationType;

/** \brief This type contains the context of a Protocol's cancellation operations.
 */
typedef struct
{
  /** \brief the type of cancellation that a Protocol is undergoing. */
  Dcm_CancellationType CancellationType;

  /** \brief Whether a final pending transmission has been completed. */
  boolean FinalTransmissionDone;

  /** \brief Whether a pending service processing has been completed and therefore
             no further cancellation call would be necessary.
             Please note that if this is an internal service handler, there may come one more
             confirmation call with an OpStatus of DCM_CONFIRMED_OK/NOT_OK. */
  boolean ServiceProcessingDone;

#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
  /** \brief The state of the cancellation of a running Asynchronous
   **        Service Handler. */
  Dcm_RetryAsyncServiceCancellationType RetryAsyncServiceCancellation;
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */
}
Dcm_CancellationConditionsType;

/** \brief This type contains the necessary context information for DCM state
 *         machine to dispatch a new request to a protocol.
 */
typedef struct
{
  /** \brief The information regarding the RxBuffer currently processed by this Protocol. */
  Dcm_BufferType JobRxBuffer;

  /** \brief RxConnectionId from whence the currently processed request was
             received. This is the same as the RxPduId.  */
  Dcm_BufferUserType JobRxConnectionId;

  /** \brief TxConnectionId to where the result of this processing should go.
             This is translatable into the TxPduId and ConfirmationTxPduId.  */
  Dcm_BufferUserType JobTxConnectionId;

  /** \brief ComMChannel which controls the communication session for this processing */
  NetworkHandleType JobComMChannelId;

  Dcm_TesterSourceAddressType JobTesterSourceAddress;

  /** \brief The type of the request that this Protocol is currently processing. */
  Dcm_RequestTypeType JobRequestType;

  /** \brief Total number of RCRRP */
  uint16 JobRpCntr;

  /** \brief Total number of confirmed RCRRP */
  uint16 JobRpCntrConfirmed;

  /** \brief Status of the transmission */
  Dcm_RpTxStatusType JobResponsePendingTxStatus;

} Dcm_ProtocolJobType;

/** \brief This type contains the the most frequently used context information for DCM state
 * machine */
typedef struct
{
  /** \brief Basic information to dispatch a new request to a protocol.
   *         The old state needs to be preserved until the previous request is finished completely.
   *         (OosNotified state is reached)
   */
  Dcm_ProtocolJobType NextJob;

  /** \brief message Context for requests */
  Dcm_MsgContextType msgContext;

  /** \brief Counter of resp. data sent via paged buffer processing */
  Dcm_MsgLenType sentLength;

  /** \brief Timer value of Dsp retry timer */
  uint16 retryTimer;

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
  /** \brief Timer used to trigger retry of read data by identifier */
  uint16 retryTimerRDBPI;
#endif /* (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */

  /** \brief RxConnectionId from whence the currently processed request was
             received. This is the same as the RxPduId.  */
  Dcm_BufferUserType RxConnectionId;

  Dcm_TesterSourceAddressType TesterSourceAddress;

  /** \brief TxConnectionId to where the result of this processing should go.
             This is translatable into the TxPduId and ConfirmationTxPduId.  */
  Dcm_BufferUserType TxConnectionId;

  /** \brief Response Pending counter */
  uint16 rpCntr;

  /** \brief Confirmed Response Pending counter */
  uint16 rpCntrConfirmed;

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
  /** \brief Forced Response Pending counter */
  uint16 rpForcedCntr;

  /** \brief Forced Response Pending Confirmation counter */
  uint16 rpForcedCntrConfirmed;
#endif

  /** \brief The information regarding the RxBuffer currently processed by this Protocol. */
  Dcm_BufferType RxBuffer;

  /** \brief The information regarding the TxBuffer currently processed by this Protocol. */
  Dcm_BufferType TxBuffer;

  /** \brief The information regarding the NRCBuffer currently processed by this Protocol. */
  Dcm_BufferType NRCBuffer;

 /** \brief ComMChannel which controls the communication session for this processing */
  NetworkHandleType ComMChannelId;

  /** \brief Index of the currently active serviceId; Indexes the array Dcm_SidTabEntryConfig[] */
  uint8 sidTabEntryInd;

#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
  /** \brief Currently entered Diagnostics Session */
  Dcm_SesCtrlType confirmedDiagSession;
#endif
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
  /** \brief Currently requested reset mode */
  Dcm_ResetModeType requestedResetMode;

  /** \brief Currently confirmed reset mode */
  Dcm_ResetModeType confirmedResetMode;
#endif
  /** \brief Id of service request handled by the state machine */
  uint8 serviceId;

  /** \brief Id of subservice request handled by the state machine */
  uint8 subServiceId;

  /** \brief Index of the currently active sub-service; Indexes the array
   **  Dcm_SubSidTabEntryConfig[] */
  uint8 subServiceIdIdx;

  /** \brief Negative response code occurred during processing of the request */
  Dcm_NegativeResponseCodeType negativeResponseCode;

  /** \brief Id of ProcessPage() function to be called during paged processing
   **
   ** Indexes the array of paged buffer processing functions.
   ** Set to DCM_PROCESS_PAGE_ID_INVALID (=0xFF) if paged processing is not active.
   */
  uint8 processPageFuncId;

  /** \brief flag used to avoid innecesary calls to SetProgConditions() */
  boolean SetProgConditionsFinished;

  /** \brief Indicates if Jump to bootloader mode request is accepted */
  boolean jumpToBLSupport;

  /** \brief Indicates if Jump to bootloader request is pending */
  uint8 jumpToBLExecution;

  /** \brief Indicates if Jump to bootloader request has been inhibited
   ** due to a pending previously started SetProgConditions() call */
  boolean jumpToBLInhibited;

  /** \brief Type of service request handled by the state machine
   **
   ** - TRUE  : If service processed is DSP service
   ** - FALSE : If service processed is RTE/CentralDiag Service */
  boolean dspService;

  /** \brief Status of failures before dispatching a service request
   **
   ** - TRUE  : Initial request validations successful
   ** - FALSE : Initial request validations failed */
  boolean requestValidationStatus;

  /** \brief The context of any cancellation operation pending for this Protocol. */
  Dcm_CancellationConditionsType CancellationConditions;

  /** \brief 'Response Pending' transmission status */
  Dcm_RpTxStatusType responsePendingTxStatus;

  /** \brief The type of the request that this Protocol is currently processing. */
  Dcm_RequestTypeType RequestType;
}
Dcm_HsmInfoType;

/** \brief This type contains configuration of the service ID tables. */
typedef struct
{
  /** \brief Pointer to array of service functions included in this table. */
  P2CONST(Dcm_SidTabEntryConfigType, TYPEDEF, DCM_CONST) Entries;

  /** \brief number of service entries in this table */
  uint8                                                  NumEntries;
}
Dcm_SidTabConfigType;

/** \brief Transmission type selected for protocol */

/** \brief No transmission type is selected */
#if (defined DCM_NOTYPE)
  #error "DCM_NOTYPE is already defined"
#endif
#define DCM_NOTYPE 0U

/** \brief TYPE1 is selected for transmission */
#if (defined DCM_TYPE1)
  #error "DCM_TYPE1 is already defined"
#endif
#define DCM_TYPE1 1U

/** \brief TYPE2 is selected for transmission */
#if (defined DCM_TYPE2)
  #error "DCM_TYPE2 is already defined"
#endif
#define DCM_TYPE2 2U

typedef uint8 Dcm_ProtocolTransType;

/* Tells Protocol whether to attempt a RCRRP transmission on entry,
   whether a transmission is already ongoing or whether to just start
   processing the request. */
#define DCM_PROTOCOL_RCRRP_INVALID 0x00U
#define DCM_PROTOCOL_NO_RCRRP_ON_ENTRY 0x01U
#define DCM_PROTOCOL_BEGIN_TO_TRANSMIT_RCRRP_ON_ENTRY 0x02U
#define DCM_PROTOCOL_TRANSMITTING_RCRRP_ON_ENTRY 0x03U
typedef uint8 Dcm_ProtocolRCRRPTransmissionType;

/** \brief Type of Xxx_StartProtocol function of the CallbackDCMRequestServices interface */
typedef P2FUNC(Std_ReturnType, RTE_CODE, Dcm_DslStartProtocolFncType) (
  Dcm_ProtocolType ProtocolID);

/** \brief Type of Xxx_StopProtocol function of the CallbackDCMRequestServices interface */
typedef P2FUNC(Std_ReturnType, RTE_CODE, Dcm_DslStopProtocolFncType) (
  Dcm_ProtocolType ProtocolID);

typedef struct
{
  /** \brief function pointer for StartProtocol function */
  Dcm_DslStartProtocolFncType StartProtocol;
  /** \brief function pointer for StopProtocol function */
  Dcm_DslStopProtocolFncType  StopProtocol;
} Dcm_CallbackRequestServicesType;

/** \brief Pointer to SidTabEntryConfigType */
typedef P2CONST(Dcm_SidTabEntryConfigType, TYPEDEF, DCM_CONST) Dcm_SidTabEntryConfigPtrType;

/** \brief The type of service request on a Data Identifier(DID) */

/** \brief ReadDID Service request */
#if (defined DCM_DID_OP_READ)
  #error "DCM_DID_OP_READ is already defined"
#endif
#define DCM_DID_OP_READ 0U

/** \brief WriteDID Service request */
#if (defined DCM_DID_OP_WRITE)
  #error "DCM_DID_OP_WRITE is already defined"
#endif
#define DCM_DID_OP_WRITE 1U

/** \brief IOControl Service request */
#if (defined DCM_DID_OP_IOCONTROL)
  #error "DCM_DID_OP_IOCONTROL is already defined"
#endif
#define DCM_DID_OP_IOCONTROL 2U

/** \brief Dynamically Defined DID Service request */
#if (defined DCM_DID_OP_DYNAMIC)
  #error "DCM_DID_OP_DYNAMIC is already defined"
#endif
#define DCM_DID_OP_DYNAMIC 3U

/** \brief ReadDataByperiodicIdentifier Service request */
#if (defined DCM_DID_OP_PERIODIC)
  #error "DCM_DID_OP_PERIODIC is already defined"
#endif
#define DCM_DID_OP_PERIODIC 4U

#if (defined DCM_DID_OP_READSCALING)
  #error "DCM_DID_OP_READSCALING is already defined"
#endif
#define DCM_DID_OP_READSCALING 5U

typedef uint8 Dcm_DidOpType;

/* !LINKSTO Dcm.Dsn.Type.Dcm_DidConfigIdxType,1 */
/** \brief Type of an index into a DcmDspDid or DcmDspDidRange array */
#if ( (DCM_DSP_DIDSERVICES_NUM_DIDS > 0xFFU)         || \
      (DCM_DSP_DIDSERVICES_NUM_DID_RANGES > 0xFFU) )
typedef uint16_least Dcm_DidConfigIdxType;
#else
typedef uint8 Dcm_DidConfigIdxType;
#endif

/* Different data types access of data belonging to DcmDspDataType for a Did */
/** \brief DcmDspDataType: type of the signal is boolean */
#if (defined DCM_TYPE_BOOLEAN)
  #error "DCM_TYPE_BOOLEAN is already defined"
#endif
#define DCM_TYPE_BOOLEAN 0U

/** \brief DcmDspDataType: type of the signal is sint16 */
#if (defined DCM_TYPE_SINT16)
  #error "DCM_TYPE_SINT16 is already defined"
#endif
#define DCM_TYPE_SINT16  1U

/** \brief DcmDspDataType: type of the signal is sint16_N */
#if (defined DCM_TYPE_SINT16_N)
  #error "DCM_TYPE_SINT16_N is already defined"
#endif
#define DCM_TYPE_SINT16_N  2U

/** \brief DcmDspDataType: type of the signal is sint32 */
#if (defined DCM_TYPE_SINT32)
  #error "DCM_TYPE_SINT32 is already defined"
#endif
#define DCM_TYPE_SINT32  3U

/** \brief DcmDspDataType: type of the signal is sint32_N */
#if (defined DCM_TYPE_SINT32_N)
  #error "DCM_TYPE_SINT32_N is already defined"
#endif
#define DCM_TYPE_SINT32_N  4U

/** \brief DcmDspDataType: type of the signal is sint8 */
#if (defined DCM_TYPE_SINT8)
  #error "DCM_TYPE_SINT8 is already defined"
#endif
#define DCM_TYPE_SINT8   5U

/** \brief DcmDspDataType: type of the signal is sint8_N */
#if (defined DCM_TYPE_SINT8_N)
  #error "DCM_TYPE_SINT8_N is already defined"
#endif
#define DCM_TYPE_SINT8_N   6U

/** \brief DcmDspDataType: type of the signal is uint16 */
#if (defined DCM_TYPE_UINT16)
  #error "DCM_TYPE_UINT16 is already defined"
#endif
#define DCM_TYPE_UINT16  7U

/** \brief DcmDspDataType: type of the signal is uint16_N */
#if (defined DCM_TYPE_UINT16_N)
  #error "DCM_TYPE_UINT16_N is already defined"
#endif
#define DCM_TYPE_UINT16_N  8U

/** \brief DcmDspDataType: type of the signal is uint32 */
#if (defined DCM_TYPE_UINT32)
  #error "DCM_TYPE_UINT32 is already defined"
#endif
#define DCM_TYPE_UINT32  9U

/** \brief DcmDspDataType: type of the signal is uint32_N */
#if (defined DCM_TYPE_UINT32_N)
  #error "DCM_TYPE_UINT32_N is already defined"
#endif
#define DCM_TYPE_UINT32_N  10U

/** \brief DcmDspDataType: type of the signal is uint8 */
#if (defined DCM_TYPE_UINT8)
  #error "DCM_TYPE_UINT8 is already defined"
#endif
#define DCM_TYPE_UINT8   11U

/** \brief DcmDspDataType: type of the signal is uint8_N */
#if (defined DCM_TYPE_UINT8_N)
  #error "DCM_TYPE_UINT8_N is already defined"
#endif
#define DCM_TYPE_UINT8_N   12U

/** \brief Dcm_EndiannessType: type of endianness is big endian */
#if (defined DCM_BIG_ENDIAN)
#error DCM_BIG_ENDIAN already defined
#endif /* if (defined DCM_BIG_ENDIAN) */

#define DCM_BIG_ENDIAN 0x01U

/** \brief Dcm_EndiannessType: type of endianness is little endian */
#if (defined DCM_LITTLE_ENDIAN)
#error DCM_LITTLE_ENDIAN already defined
#endif /* if (defined DCM_LITTLE_ENDIAN) */

#define DCM_LITTLE_ENDIAN 0x02U

/** \brief Dcm_EndiannessType: type of endianness is opaque */
#if (defined DCM_OPAQUE_ENDIAN)
#error DCM_OPAQUE_ENDIAN already defined
#endif /* if (defined DCM_OPAQUE_ENDIAN) */

#define DCM_OPAQUE_ENDIAN 0x03U

/** \brief Dcm_ByteAdvanceDirectionType: type byte advancement in a data copy is ++ */
#if (defined DCM_BYTE_DIRECTION_INCREMENT)
#error DCM_BYTE_DIRECTION_INCREMENT already defined
#endif /* if (defined DCM_BYTE_DIRECTION_INCREMENT) */

#define DCM_BYTE_DIRECTION_INCREMENT 1U

/** \brief Dcm_ByteAdvanceDirectionType: type byte advancement in a data copy is -- */
#if (defined DCM_BYTE_DIRECTION_DECREMENT)
#error DCM_BYTE_DIRECTION_DECREMENT already defined
#endif /* if (defined DCM_BYTE_DIRECTION_DECREMENT) */

#define DCM_BYTE_DIRECTION_DECREMENT 2U

/** \brief Dcm_DataCopyMethodType: data copy method is to out buffer big endian */
#if (defined DCM_COPY_TO_OUTBUFFER_BIGENDIAN)
#error DCM_COPY_TO_OUTBUFFER_BIGENDIAN already defined
#endif /* if (defined DCM_COPY_TO_OUTBUFFER_BIGENDIAN) */

#define DCM_COPY_TO_OUTBUFFER_BIGENDIAN 1U

/** \brief Dcm_DataCopyMethodType: data copy method is to out buffer little endian */
#if (defined DCM_COPY_TO_OUTBUFFER_LITTLEENDIAN)
#error DCM_COPY_TO_OUTBUFFER_LITTLEENDIAN already defined
#endif /* if (defined DCM_COPY_TO_OUTBUFFER_LITTLEENDIAN) */

#define DCM_COPY_TO_OUTBUFFER_LITTLEENDIAN 2U

/** \brief Dcm_DataCopyMethodType: data copy method is to out buffer opaque */
#if (defined DCM_COPY_TO_OUTBUFFER_OPAQUE)
#error DCM_COPY_TO_OUTBUFFER_OPAQUE already defined
#endif /* if (defined DCM_COPY_TO_OUTBUFFER_OPAQUE) */

#define DCM_COPY_TO_OUTBUFFER_OPAQUE 3U

/** \brief Dcm_DataCopyMethodType: data copy method is from in buffer big endian */
#if (defined DCM_COPY_FROM_INBUFFER_BIGENDIAN)
#error DCM_COPY_FROM_INBUFFER_BIGENDIAN already defined
#endif /* if (defined DCM_COPY_FROM_INBUFFER_BIGENDIAN) */

#define DCM_COPY_FROM_INBUFFER_BIGENDIAN 4U

/** \brief Dcm_DataCopyMethodType: data copy method is from in buffer little endian */
#if (defined DCM_COPY_FROM_INBUFFER_LITTLEENDIAN)
#error DCM_COPY_FROM_INBUFFER_LITTLEENDIAN already defined
#endif /* if (defined DCM_COPY_FROM_INBUFFER_LITTLEENDIAN) */

#define DCM_COPY_FROM_INBUFFER_LITTLEENDIAN 5U

/** \brief Dcm_DataCopyMethodType: data copy method is from in buffer opaque */
#if (defined DCM_COPY_FROM_INBUFFER_OPAQUE)
#error DCM_COPY_FROM_INBUFFER_OPAQUE already defined
#endif /* if (defined DCM_COPY_FROM_INBUFFER_OPAQUE) */

#define DCM_COPY_FROM_INBUFFER_OPAQUE 6U


/** \brief Different types of AUTOSAR access interfaces */

/** \brief  This type contains the configuration information for each signals configured in a Did
 */
typedef struct
{
  /** \brief Position in bits of the DID data */
  uint16 DataPos;
  /** \brief Index to the Data array */
  uint16 DspDataIndex;
}
Dcm_DidSignalType;

/** \brief Type for DidDataType */
typedef uint8 Dcm_DidDataByteType;

#if(DCM_DSP_USE_SERVICE_0X2C == STD_ON)
/** \brief Invalid DDDid source */
#if (defined DCM_DDDID_SRC_TYPE_NONE)
  #error "DCM_DDDID_SRC_TYPE_NONE is already defined"
#endif
#define DCM_DDDID_SRC_TYPE_NONE 0U

#if(DCM_0X2C_0X02_SSVC_ENABLED == STD_ON)
/** \brief Memory type DDDid source */
#if (defined DCM_DDDID_SRC_TYPE_MEM)
  #error "DCM_DDDID_SRC_TYPE_MEM is already defined"
#endif
#define DCM_DDDID_SRC_TYPE_MEM 1U
#endif /* #if(DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) */

/** \brief Did type DDDid source */
#if (defined DCM_DDDID_SRC_TYPE_DID)
  #error "DCM_DDDID_SRC_TYPE_DID is already defined"
#endif
#define DCM_DDDID_SRC_TYPE_DID 2U

typedef uint8 Dcm_DDDidSrcType;

#if (defined DCM_DDDID_CLEAR_ALL)
  #error "DCM_DDDID_CLEAR_ALL is already defined"
#endif
#define DCM_DDDID_CLEAR_ALL 0xFFFFU

/** \brief Dynamic DID defined by Data Identifier
           Dcm_DDDidSrcType must be the first member, since it is used in a union to identify
           the type of the array element */
typedef struct
{
  /** \brief Type of DDDid source  */
  Dcm_DDDidSrcType srcType;
  /** \brief The referenced source DID
   **        for DDDid sources of type DID */
  uint16 srcDid;
  /** \brief Position of the data in the source DID record */
  uint8 srcDidPos;
  /** \brief Size of the data in the source DID record */
  uint8 srcDidSize;
} Dcm_DDDidDefByDataIdentifierType;

/** \brief Dynamic DID defined by Memory Address
           Dcm_DDDidSrcType must be the first member, since it is used in a union to identify
           the type of the array element */
typedef struct
{
  /** \brief Type of DDDid source  */
  Dcm_DDDidSrcType srcType;
  /** \brief Address of the data for DDDid sources of type memory.  */
  uint32 srcMemAddr;
  /** \brief Size of the data for DDDid sources of type memory.  */
  uint32 srcMemSize;
} Dcm_DDDidDefByMemAddressType;

/** \brief Entry in the source table of a dynamically defined DID */
/* Deviation MISRAC2012-3 */
/* Deviation MISRA-2 */
typedef union
{
  /** \brief Type of DDDid source */
  Dcm_DDDidSrcType srcType;
  /** \brief Dynamic DID defined by Memory Address */
  Dcm_DDDidDefByMemAddressType memAddrDDDid;
  /** \brief Dynamic DID defined by Identifier */
  Dcm_DDDidDefByDataIdentifierType dataIdDDDid;
} Dcm_DDDidSrcTblEntryType;
#endif /* (DCM_DSP_USE_SERVICE_0X2C == STD_ON) */

/* The state for checking the request data */
#if (defined DCM_DID_REQUEST_CHECK)
  #error "DCM_DID_REQUEST_CHECK is already defined"
#endif
#define DCM_DID_REQUEST_CHECK 0U

/* The state for processing the request data */
#if (defined DCM_DID_REQUEST_PROCESSING)
  #error "DCM_DID_REQUEST_PROCESSING is already defined"
#endif
#define DCM_DID_REQUEST_PROCESSING 1U

/* The state for PENDING read operation */
#if (defined DCM_DID_READ_PENDING)
  #error "DCM_DID_READ_PENDING is already defined"
#endif
#define DCM_DID_READ_PENDING 2U

/* Invalid state */
#if (defined DCM_DID_REQUEST_INVALID)
  #error "DCM_DID_REQUEST_INVALID is already defined"
#endif
#define DCM_DID_REQUEST_INVALID 3U

/** \brief  This type contains the different stages of processing a Did request */
typedef uint8 Dcm_DidRequestProcessingStType;

/* State for checking the conditions for reading DID*/
/* Read Primary DID */
#if (defined DCM_DID_READ_PRIMARY)
  #error "DCM_DID_READ_PRIMARY is already defined"
#endif
#define DCM_DID_READ_PRIMARY 0U

/* Read Referenced DID */
#if (defined DCM_DID_READ_REFERENCED)
  #error "DCM_DID_READ_REFERENCED is already defined"
#endif
#define DCM_DID_READ_REFERENCED 1U

/** \brief Different Contexts while reading a Did using the service 0x22
 */
typedef uint8 Dcm_DidReadProcessingStType;

/* State for checking the conditions for reading referenced DID*/
#if (defined DCM_CURRENT_DID_READ_CONDITION)
  #error "DCM_CURRENT_DID_READ_CONDITION is already defined"
#endif
#define DCM_CURRENT_DID_READ_CONDITION 0U

/* State for reading referenced DIDs ID to the buffer*/
#if (defined DCM_CURRENT_DID_READ_ID)
  #error "DCM_CURRENT_DID_READ_ID is already defined"
#endif
#define DCM_CURRENT_DID_READ_ID 1U

/* State for reading referenced DID*/
#if (defined DCM_CURRENT_DID_READ_DATA)
  #error "DCM_CURRENT_DID_READ_DATA is already defined"
#endif
#define DCM_CURRENT_DID_READ_DATA 2U

/** \brief This type contains the different stages of processing a referenced Did read
 */
typedef uint8 Dcm_CurrentDidProcessingStType;

/* Invalid case */
#if (defined DCM_RET_CTRL_INACTIVE)
  #error "DCM_RET_CTRL_INACTIVE is already defined"
#endif
#define DCM_RET_CTRL_INACTIVE 0U

/* Returning control to Ecu for a signal is finished */
#if (defined DCM_RET_CTRL_FINISHED)
  #error "DCM_RET_CTRL_FINISHED is already defined"
#endif
#define DCM_RET_CTRL_FINISHED 1U

/* Returning control to Ecu for a signal is Pending */
#if (defined DCM_RET_CTRL_PENDING)
  #error "DCM_RET_CTRL_PENDING is already defined"
#endif
#define DCM_RET_CTRL_PENDING 2U

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
/* Invalid case. For unconfigured DIDs. */
#if (defined DCM_RET_CTRL_DEFAULT_INACTIVE)
  #error "DCM_RET_CTRL_DEFAULT_INACTIVE is already defined"
#endif
#define DCM_RET_CTRL_DEFAULT_INACTIVE 3U

/* Returning control to Ecu for a signal is finished. For unconfigured DIDs. */
#if (defined DCM_RET_CTRL_DEFAULT_FINISHED)
  #error "DCM_RET_CTRL_DEFAULT_FINISHED is already defined"
#endif
#define DCM_RET_CTRL_DEFAULT_FINISHED 4U

/* Returning control to Ecu for a signal  is Pending. For unconfigured DIDs. */
#if (defined DCM_RET_CTRL_DEFAULT_PENDING)
  #error "DCM_RET_CTRL_DEFAULT_PENDING is already defined"
#endif
#define DCM_RET_CTRL_DEFAULT_PENDING 5U
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

/* Continue processing */
#if (defined DCM_DID_READ_CYCLE_CONTINUE)
  #error "DCM_DID_READ_CYCLE_CONTINUE is already defined"
#endif
#define DCM_DID_READ_CYCLE_CONTINUE 0U

/* Interrupt processing because of a PENDING return from a lower interface */
#if (defined DCM_DID_READ_CYCLE_PENDING)
  #error "DCM_DID_READ_CYCLE_PENDING is already defined"
#endif
#define DCM_DID_READ_CYCLE_PENDING 1U

/* Interrupt processing because of service handler cancellation */
#if (defined DCM_DID_READ_CYCLE_CANCEL)
  #error "DCM_DID_READ_CYCLE_CANCEL is already defined"
#endif
#define DCM_DID_READ_CYCLE_CANCEL 2U

/* Interrupt processing because of an NRC being set */
#if (defined DCM_DID_READ_CYCLE_NRC)
  #error "DCM_DID_READ_CYCLE_NRC is already defined"
#endif
#define DCM_DID_READ_CYCLE_NRC 3U

/* Interrupt processing because of all the DIDs in the list having been read */
#if (defined DCM_DID_READ_CYCLE_COMPLETED)
  #error "DCM_DID_READ_CYCLE_COMPLETED is already defined"
#endif
#define DCM_DID_READ_CYCLE_COMPLETED 4U

/** \brief  This type contains the different reasons for interrupting a DID read processing
            cycle */
typedef uint8 Dcm_DidReadCycleInterruptType;

/* Continue processing */
#define DCM_DID_READ_CYCLE_CONTINUE 0U
/* Interrupt processing because of a PENDING return from a lower interface */
#define DCM_DID_READ_CYCLE_PENDING 1U

/** \brief Type contains the State of currently processed DID in the list
           of referenced DIDs for the current primary DID */
typedef uint8 Dcm_DidInListProcessingStType;

/* Check availability of DID */
#if (defined DCM_DID_IN_LIST_READ_CYCLE_AVAILABILITY)
  #error "DCM_DID_IN_LIST_READ_CYCLE_AVAILABILITY is already defined"
#endif
#define DCM_DID_IN_LIST_READ_CYCLE_AVAILABILITY 0U

/* Execute the actual read operation */
#if (defined DCM_DID_IN_LIST_READ_CYCLE_READ)
  #error "DCM_DID_IN_LIST_READ_CYCLE_READ is already defined"
#endif
#define DCM_DID_IN_LIST_READ_CYCLE_READ 1U

/** \brief Type contains State of currently processed Dynamic DID */
typedef uint8 Dcm_DidInRefListProcessingStType;

/* Check availability of DID */
#if (defined DCM_DID_IN_REF_LIST_READ_CYCLE_AVAILABILITY)
  #error "DCM_DID_IN_REF_LIST_READ_CYCLE_AVAILABILITY is already defined"
#endif
#define DCM_DID_IN_REF_LIST_READ_CYCLE_AVAILABILITY 0U

/* Execute the actual read operation */
#if (defined DCM_DID_IN_REF_LIST_READ_CYCLE_READ)
  #error "DCM_DID_IN_REF_LIST_READ_CYCLE_READ is already defined"
#endif
#define DCM_DID_IN_REF_LIST_READ_CYCLE_READ 1U

/** \brief Type contains State of current read operation for Dynamic DID */
typedef uint8 Dcm_DynamicDidProcessingStType;

#if (defined DCM_DYNAMIC_DID_INITIAL_CALL)
  #error "DCM_DYNAMIC_DID_INITIAL_CALL is already defined"
#endif
#define  DCM_DYNAMIC_DID_INITIAL_CALL 0U

#if (defined DCM_DYNAMIC_DID_CONFIGURE_SOURCE)
  #error "DCM_DYNAMIC_DID_CONFIGURE_SOURCE is already defined"
#endif
#define DCM_DYNAMIC_DID_CONFIGURE_SOURCE 1U

#if (defined DCM_DYNAMIC_DID_READ_FROM_SOURCE)
  #error "DCM_DYNAMIC_DID_READ_FROM_SOURCE is already defined"
#endif
#define DCM_DYNAMIC_DID_READ_FROM_SOURCE 2U

#if (defined DCM_DYNAMIC_DID_UPDATE_RESULT)
  #error "DCM_DYNAMIC_DID_UPDATE_RESULT is already defined"
#endif
#define DCM_DYNAMIC_DID_UPDATE_RESULT 3U

#if (defined DCM_EXECUTE_SESSION_SECURITY_MODERULE_CHECKS)
  #error "DCM_EXECUTE_SESSION_SECURITY_MODERULE_CHECKS is already defined"
#endif
#define  DCM_EXECUTE_SESSION_SECURITY_MODERULE_CHECKS 1U

#if (defined DCM_DO_NOT_EXECUTE_SESSION_SECURITY_MODERULE_CHECKS)
  #error "DCM_DO_NOT_EXECUTE_SESSION_SECURITY_MODERULE_CHECKS is already defined"
#endif
#define  DCM_DO_NOT_EXECUTE_SESSION_SECURITY_MODERULE_CHECKS 0U

typedef boolean Dcm_PerformEndiannessConversionType;

#if (defined DCM_PERFORM_ENDIANNESS_CONVERSION)
  #error "DCM_PERFORM_ENDIANNESS_CONVERSION is already defined"
#endif
#define  DCM_PERFORM_ENDIANNESS_CONVERSION 1U

#if (defined DCM_DO_NOT_PERFORM_ENDIANNESS_CONVERSION)
  #error "DCM_DO_NOT_PERFORM_ENDIANNESS_CONVERSION is already defined"
#endif
#define  DCM_DO_NOT_PERFORM_ENDIANNESS_CONVERSION 0U

/** \brief State for read operation on Primary DID */
#if (defined DCM_PRIMARY_DID)
  #error "DCM_PRIMARY_DID is already defined"
#endif
#define DCM_PRIMARY_DID 0U

/** \brief State for read operation on Referenced DID */
#if (defined DCM_REFERENCED_DID)
  #error "DCM_REFERENCED_DID is already defined"
#endif
#define DCM_REFERENCED_DID 1U

/** \brief Pointer to const DataId array */
typedef P2CONST(uint16, TYPEDEF, DCM_APPL_CONST) Dcm_DataIdPtrConstType;


/** \brief Type of service function for request indication
 ** Two types of indicatiin functionality
 ** 1) Request Notification for Manufacturer
 ** 2) Request Notification for Supplier */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteIndicationType) (
  uint8 SID,
  P2CONST(uint8, AUTOMATIC, RTE_APPL_CONST) RequestData,
  uint16 DataSize,
  uint8 ReqType,
  uint16 SourceAddress,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_APPL_DATA) ErrorCode);

/** \brief Type of service function for Confirmation */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteConfirmationType) (
  uint8 SID,
  uint8 ReqType,
  uint16 SourceAddress,
  Dcm_ConfirmationStatusType ConfirmationStatus);

/** \brief This type contains the configuration information for Notification functionality */
typedef struct
{
  Dcm_RteIndicationType Indication; /*< ptr to Indication() */
  Dcm_RteConfirmationType Confirmation; /*< ptr to Confirmation() */
}
Dcm_RteRequestNotificationType;

/** \brief Type definition for OBD PidIdentifier */
typedef uint8 Dcm_DspPidIdentifierType;

#if(DCM_DSP_USE_SERVICE_0X08 == STD_ON)

/** \brief Type of the size of the IN buffer passed to the Xxx_RequestControl() function */
typedef uint8 Dcm_DspReqCtrlInBufferSizeType;

/** \brief Type of the size of the OUT buffer passed to the Xxx_RequestControl() function */
typedef uint8 Dcm_DspReqCtrlOutBufferSizeType;

/** \brief Type of the Id of the On-Board System, Test or Component */
typedef uint8 Dcm_DspReqCtrlTestIdType;

/** \brief Type of the indicator of whether the RequestControl instance is used */
typedef boolean Dcm_DspRequestControlUsedType;

/** \brief Type of function to send comapre key value (Sync and User operations) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_DspRequestControlFncType) (
  P2VAR(uint8, AUTOMATIC, DCM_VAR) OutBuffer,
  P2CONST(uint8, AUTOMATIC, DCM_VAR) InBuffer);

/** \brief RequestControl configuration structure  */
typedef struct
{
  /** \brief Pointer to the function that handles this RequestControl instance */
  Dcm_DspRequestControlFncType RequestControlFnc;

  /** \brief Size of the IN buffer passed to the Xxx_RequestControl() function */
  Dcm_DspReqCtrlInBufferSizeType InBufferSize;

  /** \brief Size of the OUT buffer passed to the Xxx_RequestControl() function */
  Dcm_DspReqCtrlOutBufferSizeType OutBufferSize;

  /** \brief Id of the On-Board System, Test or Component */
  Dcm_DspReqCtrlTestIdType TestId;
} Dcm_DspRequestControlType;

#endif /* #if(DCM_DSP_USE_SERVICE_0X08 == STD_ON) */

typedef uint8 Dcm_ServiceHandlerType;

#if (defined DCM_INTERNAL_SERVICE_HANDLER)
  #error "DCM_INTERNAL_SERVICE_HANDLER is already defined"
#endif
#define DCM_INTERNAL_SERVICE_HANDLER 0

#if (defined DCM_EXTERNAL_SERVICE_HANDLER)
  #error "DCM_EXTERNAL_SERVICE_HANDLER is already defined"
#endif
#define DCM_EXTERNAL_SERVICE_HANDLER 1

typedef uint8 Dcm_ServiceExecutionType;

#if (defined DCM_SYNC_EXECUTION)
  #error "DCM_SYNC_EXECUTION is already defined"
#endif
#define DCM_SYNC_EXECUTION 0

#if (defined DCM_ASYNC_EXECUTION)
  #error "DCM_ASYNC_EXECUTION is already defined"
#endif
#define DCM_ASYNC_EXECUTION 1

#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
typedef uint8 Dcm_AsynchronousProcessingStateType;

#if (defined DCM_ASYNC_PROCESSING_IDLE)
  #error "DCM_ASYNC_PROCESSING_IDLE is already defined"
#endif
#define DCM_ASYNC_PROCESSING_IDLE 0U

#if (defined DCM_ASYNC_PROCESSING_RUNNING)
  #error "DCM_ASYNC_PROCESSING_RUNNING is already defined"
#endif
#define DCM_ASYNC_PROCESSING_RUNNING 1U

#if (defined DCM_ASYNC_PROCESSING_DATA_AVAILABLE)
  #error "DCM_ASYNC_PROCESSING_DATA_AVAILABLE is already defined"
#endif
#define DCM_ASYNC_PROCESSING_DATA_AVAILABLE 2U

#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */

typedef struct {
  Dcm_GenericServiceHandlerType ServiceFunction;
  P2VAR(Dcm_MsgContextType, TYPEDEF, DCM_VAR) curMsgContext;
  Dcm_OpStatusType OpStatus;
  Std_ReturnType RetVal;
  Dcm_ServiceHandlerType ServiceHandlerType;
  uint8 DcmInstanceIndex;
  boolean ServiceProcessingInProgress;
#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
  Dcm_ServiceExecutionType ServiceExecutionType;
  Dcm_AsynchronousProcessingStateType AsynchronousProcessingState;
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */
}
Dcm_ServiceProcessingContext_Type;

typedef P2VAR(Dcm_ServiceProcessingContext_Type, TYPEDEF, DCM_VAR)
  Dcm_ServiceProcessingContext_PtrType;

/** \brief Generic type for uint8 based IDs (PID/OBDMID/TID/INFOTYPE) */
typedef uint8 Dcm_GenericUint8IdType;

/* Service context/execution types */
/** \brief Structure to store current configuration for an out buffer */
typedef struct{
  /** \brief Size of the buffer in bytes */
  uint32 Size;
  /** \brief Count of already used bytes in the buffer */
  uint32 Occupied;
  /** \brief Pointer to the buffer */
  P2VAR(uint8, TYPEDEF, DCM_VAR) BufferPtr;
} Dcm_DspMsgBufferCfgType;

/** \brief Values of a mutex in locked/unlocked state */
#if (defined DCM_MUTEX_LOCKED)
  #error "DCM_MUTEX_LOCKED is already defined"
#endif
#define DCM_MUTEX_LOCKED   1U

#if (defined DCM_MUTEX_UNLOCKED)
  #error "DCM_MUTEX_UNLOCKED is already defined"
#endif
#define DCM_MUTEX_UNLOCKED 0U

/** \brief A binary semaphore (mutex) type that can only be locked or unlocked */
typedef uint8 Dcm_MutexType;

/** \brief Copy method, depending on endianness and copy direction */
typedef uint8 Dcm_DataCopyMethodType;

/** \brief Byte advancement in a variable depending on cpu byte order */
typedef uint8 Dcm_ByteAdvanceDirectionType;

/** \brief Type for DidData endianness */
typedef uint8 Dcm_EndiannessType;

/** \brief Copy direction, either from variable to buffer or from buffer to variable */
typedef uint8 Dcm_DataCopyDirectionType;


/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Try to lock a mutex
 ** This function will try to lock a mutex passed by reference. If it succeeds in locking
 ** a positive response code will be returned, otherwise a negative response code will be returned
 **
 ** \param[in] MutexPtr    Pointer to the mutex to be locked
 **
 ** \return
 **         - E_OK - Mutex could be successfully locked
 **         - DCM_E_RESOURCE_LOCKED - Mutex already locked by someone else
 */
extern FUNC(Std_ReturnType,DCM_CODE) Dcm_Internal_GetLock(
  P2VAR(Dcm_MutexType, AUTOMATIC, DCM_VAR) MutexPtr
);

/** \brief Try to unlock a mutex
 ** This function will try to unlock a mutex passed by reference. If the mutex is already unlocked
 ** and the Det functionality is enabled a Det entry will be generated.
 **
 ** \param[in] MutexPtr    Pointer to the mutex to be locked
 */
extern FUNC(void, DCM_CODE) Dcm_Internal_ReleaseLock(
  P2VAR(Dcm_MutexType, AUTOMATIC, DCM_VAR) MutexPtr
);

/** \brief Initializes the Dsp's service processor
 *
 *  \return none
 */
extern FUNC (void, DCM_CODE) Dcm_Dsp_ServiceProcessorInit(void);

/** \brief Initializes the Dsp's service processor context for the provided protocol
 *
 *  \param ProtocolIdx[in]  Index of the protocol currently under processing.
 *
 *  \return none
 */
extern FUNC (void, DCM_CODE) Dcm_Dsp_ServiceProcessorReset(Dcm_ProtocolIndexType ProtocolIdx);

/** \brief This function returns the subservice handler. Strips the service/subservice part from
 **        the message if OpStatus is DCM_INITIAL state.
 **
 ** \param[in]    OpStatus     Operation state of the function
 ** \param[inout] pMsgContext  Message related information for one diagnostic protocol identifier.
 */
extern FUNC(Dcm_GenericServiceHandlerType, DCM_CODE) Dcm_Dsp_ExtractSubServiceHandler(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Notifies a Protocol that the transmission initiated on a TxConnection has timed out
 *         waiting for the next page of data in a Page-Buffered transmission context.
 *
 *  \param ProtocolId [in]       Unique ID of the Protocol BufferUser entity to be notified.
 *  \param TxConnectionId [in]   Unique ID of the TxConnection BufferUser entity which timed out.
 *
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_ProtocolTransmissionTimedOutWaitingForData(
  Dcm_BufferUserType ProtocolId,
  Dcm_BufferUserType TxConnectionId
);

/** \brief Sets up the cancellation operation for the processing of a Protocol.
 *
 *  \param CancellationType [in]  The type of the cancellation, representing what actions need to
 *                                be fulfilled in order for the cancellation to be considered
 *                                completed.
 *  \param ProtocolIndex [in]     The index of the Protocol; Indexes the arrays Dcm_HsmInfo[] and
 *                                Dcm_ProtocolConfig[].
 *
 *  \return none
 */
extern FUNC (void, DCM_CODE) Dcm_Dsp_ProtocolSetUpCancellation(
  Dcm_CancellationType CancellationType,
  Dcm_ProtocolIndexType ProtocolIndex
);

/** \brief Cancels the processing of a Protocol, causing any interface which is waiting for
 *         further calls to be called one last time with a DCM_CANCEL OpStatus.
 *
 *  \param ProtocolIndex [in]     The index of the Protocol; Indexes the arrays Dcm_HsmInfo[] and
 *                                Dcm_ProtocolConfig[].
 *
 *  \return none
 */
extern FUNC(void, DCM_APPL_CODE) Dcm_Dsp_ProtocolCancelProcessing(
  Dcm_ProtocolIndexType ProtocolIndex
);

/** \brief Dispatches a request to a protocol.
 *
 *  \param ProtocolId [in]         The ID of the Protocol;
 *  \param RxConnectionId [in]     The ID of the RxConnection on which the request was received;
 *  \param TxConnectionId [in]     The ID of the TxConnection on which the request will be
 *                                   responded;
 *  \param RequestBuffer [in]      The Buffer element containing the request data;
 *  \param RequestType [in]        The type of the request which is to be processed;
 *  \param rpCntr [in]             Number of RCRRP transmission;
 *  \param rpCntrConfirmed [in]    Number of confirmed RCRRP transmission;
 *
 *  \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_ProtocolStart(
  Dcm_BufferUserType ProtocolId,
  Dcm_BufferUserType RxConnectionId,
  Dcm_BufferUserType TxConnectionId,
  Dcm_BufferType RequestBuffer,
  Dcm_RequestTypeType RequestType,
  Dcm_ProtocolRCRRPTransmissionType ProtocolRCRRPTransmission,
  uint16 rpCntr,
  uint16 rpCntrConfirmed
);

/** \brief Called by the Supervisor in the context of a Protocol preemption in order to emit the
 *         CANCEL_PROCESSING event in the state machine of the Protocol with the ProtocolId ID.
 *         This will cause that protocol to cancel its pending operations and then notify
 *         the Supervisor of the operations having finished with a call to the
 *         Dcm_Dsl_SupervisorProtocolFree() API. After this, the process of protocol preemption may
 *         continue.
 *
 *  \param ProtocolId [in]     The BufferUser ID used in order to identify the protocol to be
 *                             cancelled.
 *
 *  \return none
 */
extern FUNC (void, DCM_CODE) Dcm_Dsp_ProtocolCancel(
  Dcm_BufferUserType ProtocolId
);

/** \brief Confirms that a certain action which needs to be fulfilled for a cancellation, such
 *         as completing a transmission or receiving a confirmation that the final pending
 *         interface has been called with a DCM_CANCEL OpStatus, .
 *
 *  \param CancellationConfirmation [in]    The type of action that has been completed.
 *  \param ProtocolIndex [in]               The index of the Protocol; Indexes the arrays
 *                                          Dcm_HsmInfo[] and Dcm_ProtocolConfig[].
 *
 *  \return none
 */
extern FUNC (void, DCM_CODE) Dcm_Dsp_CancellationDone(
  Dcm_CancellationConfirmationType CancellationConfirmation,
  Dcm_ProtocolIndexType ProtocolIndex
);


/** \brief Returns a pointer to the HSM info structure of the current HSM / protocol
 **
 ** This function must not be used outside of service processing since it requires a
 ** protocol being started or active.   If it is called before any service request has
 ** ever been received it triggers a Det error and returns a pointer to the first HSM
 ** info entry.
 **
 ** \return Pointer to element of Dcm_HsmInfo.
 */
/* Deviation MISRAC2012-1 <+2> */
extern FUNC_P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_CODE)
  Dcm_Dsp_GetCurrentHsmInfo(void);

/** \brief Validate and dispatch service request dependent on the service Id to the respective
 *         service function.
 *
 *  This function has to be called from within the Hsm!
 */
/* Deviation MISRAC2012-1 <+2> */
extern FUNC(void, DCM_CODE) Dcm_DsdInternal_DispatchSvcReq(
  DCM_PDL_DISPATCH_SERVICE_REQ(uint8 instIdx));

/** \brief Service Processing function for service handlers
 **
 ** This function is called from within Dcm_DsdInternal_DispatchSvcReq()
 */
extern FUNC(void, DCM_CODE) Dcm_DspInternal_SvcFunc(
  uint8 instIdx,
  Dcm_OpStatusType  OpStatus);

/* EB extension */

/** \brief This function confirms the successful transmission or a transmission error of a
 **        diagnostic service.
 **
 ** \param ProtocolIndex Index of the protocol on which the request was processed.
 **
 ** \param status Status indication about confirmation (differentiate failure indication and normal
 **        confirmation) (::DCM_RES_POS_OK, ::DCM_RES_POS_NOT_OK, ::DCM_RES_NEG_OK,
 **        ::DCM_RES_NEG_NOT_OK)
 */

extern FUNC(void, DCM_CODE) Dcm_DspInternal_DcmConfirmation(
  Dcm_ProtocolIndexType ProtocolIndex, Dcm_ConfirmationStatusType Status);

#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
/** \brief Start a segmented transmission of a response
 ** With this API, the application gives the complete response length to DCM nd starts PagedBuffer
 ** handling.
 ** Complete response length information (in bytes) is given in pMsgContext->resDataLen
 **
 ** \param[in] pMsgContext Message-related information for one diagnostic protocol identifier.
 */
 extern FUNC(void, DCM_CODE) Dcm_DsdInternal_StartPagedProcessing(
  P2CONST(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Request transmission of filled page
 **
 ** \param[in] FilledPageLen By application filled size (in Bytes) of Page
 */
extern FUNC(void, DCM_CODE) Dcm_DsdInternal_ProcessPage(Dcm_MsgLenType FilledPageLen);

 /** \brief Function for updating the buffer used in Paged buffering mechanism
 **
 ** \param[in] PagedBufPtr Pointer to the buffer in which data is to be filled
 ** \param[in] PageLen Length of the buffer
 */
extern FUNC(void, DCM_CODE) Dcm_DspInternal_UpdatePage(
    Dcm_MsgType PagedBufPtr,
    Dcm_MsgLenType PageLen);

/** \brief DCM informs application, that processing of paged buffer was cancelled due to errors
 */
extern FUNC(void, DCM_APPL_CODE) Dcm_DspInternal_CancelPagedBufferProcessing(void);
#endif

#if(DCM_DSP_USE_SERVICE_0X14 == STD_ON)
/** \brief Implement DEM access service 0x14
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ClearDiagnosticInformation_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* (DCM_DSP_USE_SERVICE_0X14 == STD_ON) */

#if(DCM_DSP_USE_SERVICE_0X10 == STD_ON)
/** \brief Enable different diagnostic sessions on the Dcm
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_DiagnosticSessionControl_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Generic subservice handler for UDS service DiagnosticSessionControl (0x10)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_DiagnosticSessionControlCommon_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* (DCM_DSP_USE_SERVICE_0X10 == STD_ON) */

#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
/** \brief Generic interface which shall be used by SessionModeEntry functions to confirm
 **        session to DiagnosticSessionControl service handler.
 **
 ** \param[in] Session which shall be confirmed.
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_DiagnosticSessionControl_ConfirmSession(
  Dcm_SesCtrlType ConfirmedSession);
#endif /* (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

#if(DCM_DSP_USE_SERVICE_0X11 == STD_ON)
/** \brief Provides a means to allow an external tester to request a server reset
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ECUReset_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if(DCM_DSP_SUBSERVICE_HARDRESET_0x01 == STD_ON)
/** \brief UDS service 0x11-0x01
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_HardReset_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if(DCM_DSP_SUBSERVICE_HARDRESET_0x01 == STD_ON)*/

#if(DCM_DSP_SUBSERVICE_KEYOFFONRESET_0x02 == STD_ON)
/** \brief UDS service 0x11-0x02
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_KeyOffOnReset_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if(DCM_DSP_SUBSERVICE_KEYOFFONRESET_0x02 == STD_ON)*/

#if(DCM_DSP_SUBSERVICE_SOFTRESET_0x03 == STD_ON)
/** \brief UDS service 0x11-0x03
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_SoftReset_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if(DCM_DSP_SUBSERVICE_SOFTRESET_0x03 == STD_ON)*/

#if(DCM_DSP_SUBSERVICE_ENABLERAPIDPOWERSHUTDOWN_0x04 == STD_ON)
/** \brief UDS service 0x11-0x04
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_EnableRapidPowerShutDown_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if(DCM_DSP_SUBSERVICE_ENABLERAPIDPOWERSHUTDOWN_0x04 == STD_ON)*/

#if(DCM_DSP_SUBSERVICE_DISABLERAPIDPOWERSHUTDOWN_0x05 == STD_ON)
/** \brief UDS service 0x11-0x05
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_DisableRapidPowerShutDown_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if(DCM_DSP_SUBSERVICE_DISABLERAPIDPOWERSHUTDOWN_0x05 == STD_ON)*/
#endif /* (DCM_DSP_USE_SERVICE_0X11 == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
/** \brief Sample and schedule periodic data identifiers for transmission
 **
 ** \param None
 **
 ** \retval None
 **
 ** \Reentrancy{Non-reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_Schedule(void);

/** \brief Attempt to start the transmission of one or more periodic data identifiers
 ** \param None
 **
 ** \retval None
 **
 ** \Reentrancy{Non-reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_Transmit(void);
#endif /* (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */

#if ((DCM_DSP_USE_SERVICE_0X28 == STD_ON) || (DCM_DSP_USE_SERVICE_0X85 == STD_ON))
/** \brief Returns the service configuration entry for a given service id
 **
 ** \param [in]  instIdx                The index of the protocol in the context of which
 **                                     service execution shall take place.
 **
 ** \param [in]  serviceId              Indicate service id that will be searched.
 **
 ** \param [out] pServiceIdCfg          Pointer to the configuration that corresponds in the
 **                                     configuration table for the given service id.
 **
 ** \retval Indicate if configuration for service id was found or not.
 */
extern FUNC(boolean, DCM_CODE) Dcm_DspInternal_GetSidConfig(
  uint8 instIdx,
  uint8 serviceId,
  P2VAR(Dcm_SidTabEntryConfigPtrType, AUTOMATIC, DCM_CONST) pServiceIdCfg);
#endif /* ((DCM_DSP_USE_SERVICE_0X28 == STD_ON) || (DCM_DSP_USE_SERVICE_0X85 == STD_ON)) */

#if ((DCM_DSP_USE_SERVICE_0X2E == STD_ON) || (DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X31 == STD_ON) || (DCM_DSP_USE_SERVICE_0X86 == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || (DCM_DSP_USE_SERVICE_0X2A == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_DSP_USE_SERVICE_0X02 == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X2C == STD_ON) || (DCM_DSP_USE_SERVICE_0X24 == STD_ON))
/** \brief The function for copying any bits of data from a valid source buffer to valid
    destination buffer
 **
 ** \param[in] SourceBuffer Pointer to source buffer
 ** \param[in] SourceStartPosition Start bit position of data in source buffer
 ** \param[in] DestStartPosition Start bit position of data in destination buffer
 ** \param[in] SizeToCopy Size of data in bits
 ** \param[out] DestBuffer Pointer to destination buffer
 ** \param[in] CopyMethod Copy method, depending on endianness and copy direction
 ** \param[in] ByteAdvanceDirection Byte advancement in a variable depending on cpu byte order
 */
extern FUNC(void, DCM_CODE) Dcm_DspInternal_CopySignalData(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) SourceBuffer,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DestBuffer,
  Dcm_MsgLenType SourceStartPosition,
  Dcm_MsgLenType DestStartPosition,
  Dcm_MsgLenType SizeToCopy,
  Dcm_DataCopyMethodType CopyMethod,
  Dcm_ByteAdvanceDirectionType ByteAdvanceDirection
  );

#endif /* #if ((DCM_DSP_USE_SERVICE_0X2E == STD_ON) || (DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X31 == STD_ON) || (DCM_DSP_USE_SERVICE_0X86 == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || (DCM_DSP_USE_SERVICE_0X2A == STD_ON) ||
     (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_DSP_USE_SERVICE_0X02 == STD_ON)) ||
     (DCM_DSP_USE_SERVICE_0X2C == STD_ON)*/

#if (DCM_DSP_USE_SERVICE_0X31 == STD_ON)
extern FUNC(void, DCM_CODE) Dcm_DspInternal_ShiftBitsInBuffer(
  P2VAR(Dcm_MsgItemType, AUTOMATIC, DCM_VAR) buffer,
  Dcm_MsgLenType fromPosition,
  Dcm_MsgLenType toPosition,
  Dcm_MsgLenType noOfBits,
  boolean clearJunkBits);
#endif /* DCM_DSP_USE_SERVICE_0X31 == STD_ON  */

#if(DCM_DSP_USE_SERVICE_0X03 == STD_ON)

/** \brief The generic service handler for OBD service 0x03
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnsotic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_RequestConfirmedDTC_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#endif /* #if(DCM_DSP_USE_SERVICE_0X03 == STD_ON) */

#if(DCM_DSP_USE_SERVICE_0X04 == STD_ON)

/** \brief The generic service handler for OBD service 0x04
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnsotic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ClearEmissionDiagInfo_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#endif /* #if(DCM_DSP_USE_SERVICE_0X04 == STD_ON) */

#if(DCM_DSP_USE_SERVICE_0X07 == STD_ON)

/** \brief The generic service handler for OBD service 0x07
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnsotic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_RequestPendingDTC_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#endif /* #if(DCM_DSP_USE_SERVICE_0X07 == STD_ON) */
#if(DCM_DSP_USE_SERVICE_0X0A == STD_ON)

/** \brief The generic service handler for OBD service 0x0A
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnsotic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_RequestPermanentDTC_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#endif /* #if(DCM_DSP_USE_SERVICE_0X0A == STD_ON) */

#if (DCM_DSP_USE_OBDDATASERVICES == STD_ON)
/** \brief Helper function to set the corresponding bit in availabilityData
 **        if 'id' is in range .'availabilityId+1'..'availabilityId'+0x1F,
 **        beginning from hihgest bit in availabilityData[0].to lowest bit in availabilityData[3].
 **        if 'id' is < 'availabilityId'+0x1F set bit 0 of availabilityData[3]
 **
 ** \param[in] id PID/OBDMID/TID/INFOTYPE to be checked if in range.
               Invalid values {0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0, 0xFF}.
 **
 ** \param[in] availabilityId Special range ID describes range
 ** 'availabilityId+1'..'availabilityId'+0x1F.
 **            Valid values: {0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0}
 **
 ** \param[out] availabilityData Pointer to bitmask array of four uin8.
 **
 ** \retval DCM_E_OK      One bit in availabilityData is set.
 ** \retval DCM_E_NOT_OK  No bit in availabilityData is set.
 ** \retval DCM_E_PARAM   Invalid values for parameters.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_OBDSupport_UpdateAvailableInfoWithID(
  Dcm_GenericUint8IdType id,
  Dcm_GenericUint8IdType availabilityId,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) availabilityData);

/** \brief Helper function to check if given reqData buffer contains mixed type of IDs:
 **        - availabilityIDs: {0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0}
 **        - nonAvailabilityIDs: other in range 0x01 to 0xFF
 **
 ** \param[in] reqData Buffer to check
 ** \param[in] reqDataLen Size of the given buffer
 **
 ** \retval DCM_E_OK      The given buffer don't contain mixed type of IDs.
 ** \retval DCM_E_NOT_OK  The given buffer contains mixed type of IDs
 **                       or the given buffer size is 0.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_OBDSupport_CheckRequestCombinedIds(
    P2VAR(uint8, AUTOMATIC, DCM_VAR)  reqData,
    Dcm_MsgLenType  reqDataLen);

#endif /* (DCM_DSP_USE_OBDDATASERVICES == STD_ON) */

/*==================[internal function declarations]=============================================*/

/* Legacy code end */

/** \brief Used by a TxConnection in order to Request a new page of data from the Protocol
 *         in the context of a Page-Buffered transmission.
 *
 *  \param ProtocolId [in]      Unique BufferUser ID of the Protocol from which a new page of
 *                              data is requested.
 *  \param TxConnectionId [in]  Unique BufferUser ID of the TxConnection which requests a new
 *                              page of data.
 *
 *  \return Std_ReturnType
 *  \retval DCM_E_OK         A new page of data can be provided at a later time.
 *  \retval DCM_E_NOT_OK     A new page of data will not be available (as a result of an error).
 */
extern FUNC (void, DCM_CODE) Dcm_Dsp_ProtocolProvideNextTxBuffer(
  Dcm_BufferUserType ProtocolId,
  Dcm_BufferUserType TxConnectionId
);

/** \brief Used by a TxConnection in order to notify a Protocol that a trasmission has ended.
 **
 *  \param ProtocolId [in]           Unique BufferUser ID of the Protocol which requested the
 *                                  transmission.
 *  \param TransmissionResult [in]  The result of the transmission.
 *  \param TxConnectionId [in]      Unique BufferUser ID of the TxConnection which notifies the
 *                                  end of the transmission session.
 *
 *  \return none
 */
extern FUNC (void, DCM_CODE) Dcm_Dsp_ProtocolTransmissionFinished(
  Dcm_BufferUserType ProtocolId,
  Dcm_TxResultType TransmissionResult,
  Dcm_BufferUserType TxConnectionId
);

/** \brief Notification function upon protocol timer timeout
 **
 ** \param[in] ProtocolIndex Index of protocol which has to be notified
 */
extern FUNC (void, DCM_CODE) Dcm_Dsp_Protocol_P2TimerTimeout(
  Dcm_ProtocolIndexType ProtocolIndex);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[external constants]=========================================================*/

#define DCM_START_SEC_CONST_8
#include <Dcm_MemMap.h>

#if (DCM_DSP_PRECONF_ADDRESS_LENGTH_FORMATIDENTIFIER == STD_ON)
/** \brief Global array holding SupportedAddressAndLengthFormatIdentifier configuration
 *
 * Holds configuration information of all Supported AddressAndLengthFormatIdentifiers. */
extern CONST(uint8, DCM_CONST)
  Dcm_SupportedAddAndLenFormatIds[DCM_NUM_ADDR_LENGTH_FORMAT_IDENTIFIER];
#endif

#define DCM_STOP_SEC_CONST_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Table of diagnostic sessions
 *
 * Configuration information of session states. */
extern CONST(Dcm_SesTabConfigType, DCM_CONST) Dcm_SesTabConfig[DCM_NUM_PROTOCOL];

#if (DCM_CALLBACK_REQUEST_SIZE > 0U)
/** \brief Global array for Callback Request Services function pointers */
extern CONST(Dcm_CallbackRequestServicesType, DCM_CONST)
  Dcm_CallbackRequests[DCM_CALLBACK_REQUEST_SIZE];
#endif

#if (DCM_NUM_SID_TABLE > 0U)
/** \brief Global array holding the configuration for the service Id table */
extern CONST(Dcm_SidTabConfigType, DCM_CONST) Dcm_SidTabConfig[DCM_NUM_SID_TABLE];
#endif

#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
/** \brief Global constant holding the pointer to the mode rule function that controls
 *         re-enabling of DTCs.
 */
extern CONST(Dcm_ModeRuleFunctionType, DCM_CONST) Dcm_DspModeRuleDTCFunction;
#endif /* DCM_MODEDECLARATION_SUPPORT */

/*-----------------[RequestControl Configuration]------------------------------------*/

#if(DCM_DSP_USE_SERVICE_0X08 == STD_ON)

#if (DCM_DSP_NUM_REQUESTCONTROL_CFG > 0)
/* Global array holding the configuration for an OBD Service $08 Id */
extern CONST(Dcm_DspRequestControlType, DCM_CONST)
  Dcm_DspRequestControlConfig[DCM_DSP_NUM_REQUESTCONTROL_CFG];
#endif

#endif /* #if(DCM_DSP_USE_SERVICE_0X08 == STD_ON) */

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/*-----------------[Calibration]------------------------------------*/

#define DCM_START_SEC_CALIB_8
#include <Dcm_MemMap.h>

/** \brief Global array holding the availability configuration for the configured services */
extern CONST(uint8, DCM_CONST)
  Dcm_DsdServiceUsed[DCM_DSP_SRV_TABLES_USED_BUF_SIZE][DCM_DSP_SERVICE_USED_BUF_SIZE];

#if(DCM_DSP_USE_SERVICE_0X08 == STD_ON)

#if (DCM_DSP_NUM_REQUESTCONTROL_CFG > 0)
/* Global array holding the information on the used entries in the Dcm_DspRequestControlConfig
 * array. This information is kept in a separate array to better suit calibration requirements. */
extern CONST(Dcm_DspRequestControlUsedType, DCM_CONST)
  Dcm_DspRequestControlUsedConfig[DCM_DSP_NUM_REQUESTCONTROL_CFG];
#endif

#endif /* #if(DCM_DSP_USE_SERVICE_0X08 == STD_ON) */

#define DCM_STOP_SEC_CALIB_8
#include <Dcm_MemMap.h>

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/** \brief Variable to hold DCM initialization state */
extern VAR(boolean, DCM_VAR) Dcm_Initialized;

#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
/** \brief Variable to hold Paged Buffer cancellation flag */
extern VAR(boolean, DCM_VAR) Dcm_CancelPagedProcessing;
#endif /* #if (DCM_PAGEDBUFFER_ENABLED == STD_ON) */

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

extern VAR(uint8, DCM_VAR) Dcm_PeriodicRequest[2];

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Global variable holding the value of RxPduId on which the request was received */
extern VAR(PduIdType, DCM_VAR) Dcm_RxPduIdBL;

/** \brief Array to hold the HSM information for each protocol. */
/* Deviation MISRAC2012-2 */
/* Deviation MISRA-3 */
extern VAR(Dcm_HsmInfoType, DCM_VAR) Dcm_HsmInfo[DCM_NUM_PROTOCOL];

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (DCM_RTE_TABLE_REQUEST_MANUFACTURER_SIZE != 0U)
/** \brief RTE-Callback function pointer table for Manufacturer Notification */
extern CONST (Dcm_RteRequestNotificationType, DCM_CONST)
  Dcm_RteManufacturerNotificationTable[DCM_RTE_TABLE_REQUEST_MANUFACTURER_SIZE];
#endif

#if (DCM_RTE_TABLE_REQUEST_SUPPLIER_SIZE != 0U)
/** \brief RTE-Callback function pointer table for Supplier Notification */
extern CONST (Dcm_RteRequestNotificationType, DCM_CONST)
  Dcm_RteSupplierNotificationTable[DCM_RTE_TABLE_REQUEST_SUPPLIER_SIZE];
#endif

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Allows the integrator to store relevant information prior to jumping to bootloader */
extern VAR(Dcm_ProgConditionsType, DCM_VAR) Dcm_ProgConditions;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (DCM_DSP_USE_SERVICE_0X2C == STD_ON)
#if (DCM_DDDID_DO_NOT_INIT == STD_ON)
#define DCM_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#else
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#endif /* DCM_DDDID_DO_NOT_INIT == STD_ON */
#include <Dcm_MemMap.h>

/** \brief Global array holding the source configuration of DDDIDs */
/* Deviation MISRA-3 */
extern VAR(Dcm_DDDidSrcTblEntryType, DCM_VAR) Dcm_DDDidSrcTable[DCM_NUM_DDDID_SRCTAB_ELEMENTS];

#if (DCM_DDDID_DO_NOT_INIT == STD_ON)
#define DCM_STOP_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#else
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#endif /* DCM_DDDID_DO_NOT_INIT == STD_ON */
#include <Dcm_MemMap.h>

#endif /* DCM_DSP_USE_SERVICE_0X2C == STD_ON */
/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_INT_H */
/*==================[end of file]================================================================*/
