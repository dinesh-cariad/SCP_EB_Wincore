#ifndef TRANSFORMER_TYPES_H
#define TRANSFORMER_TYPES_H

/**
 * \file
 *
 * \brief AUTOSAR Base
 *
 * This file contains the implementation of the AUTOSAR
 * module Base.
 *
 * \version 5.0.31
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/
#include <Std_Types.h>    /* AUTOSAR standard type declarations */

/*==================[macros]=================================================*/

/* -------------------- error codes (SWS_Xfrm_00031) -----------------------*/

#if (defined E_NO_DATA)
#error E_NO_DATA already defined
#endif
/** \brief No data available which can be deserialized. */
#define E_NO_DATA                  0x01U

/* Error code 0x80: This is reserved to avoid number clashes for autonomous error reactions */

#if (defined E_SER_GENERIC_ERROR)
#error E_SER_GENERIC_ERROR already defined
#endif
/** \brief A generic not precisely detailed error occured. */
#define E_SER_GENERIC_ERROR        0x81U

/* Error code 0x82-0x86: These are reserved to be compliant with SOME/IP which defines errors with
   these values that don't relate to serialization and thus can't be created by a transformer. */

#if (defined E_SER_WRONG_PROTOCOL_VERSION)
#error E_SER_WRONG_PROTOCOL_VERSION already defined
#endif
/** \brief The version of the receiving transformer did not match the sending transformer. */
#define E_SER_WRONG_PROTOCOL_VERSION      0x87U

#if (defined E_SER_WRONG_INTERFACE_VERSION)
#error E_SER_WRONG_INTERFACE_VERSION already defined
#endif
/** \brief Interface version of serialized data is not supported. */
#define E_SER_WRONG_INTERFACE_VERSION      0x88U

#if (defined E_SER_MALFORMED_MESSAGE)
#error E_SER_MALFORMED_MESSAGE already defined
#endif
/** \brief The received message is malformed. The transformer is not able to produce an output. */
#define E_SER_MALFORMED_MESSAGE    0x89U

#if (defined E_SER_WRONG_MESSAGE_TYPE)
#error E_SER_WRONG_MESSAGE_TYPE already defined
#endif
/** \brief The received message type was not expected. */
#define E_SER_WRONG_MESSAGE_TYPE        0x8aU


/* -------------------- error codes (SWS_Xfrm_00032) -----------------------*/

#if (defined E_SAFETY_VALID_REP)
#error E_SAFETY_VALID_REP already defined
#endif
/** \brief The data are valid according to safety, although data with a repeated counter
 *         were received. */
#define E_SAFETY_VALID_REP         0x01U

#if (defined E_SAFETY_VALID_SEQ)
#error E_SAFETY_VALID_SEQ already defined
#endif
/** \brief The data are valid according to safety, although a counter jump occurred. */
#define E_SAFETY_VALID_SEQ         0x02U

#if (defined E_SAFETY_VALID_ERR)
#error E_SAFETY_VALID_ERR already defined
#endif
/** \brief The data are valid according to safety, although the check itself failed. */
#define E_SAFETY_VALID_ERR         0x03U

#if (defined E_SAFETY_VALID_NND)
#error E_SAFETY_VALID_NND already defined
#endif
/** \brief Communication is valid according to safety, but no new data received. */
#define E_SAFETY_VALID_NND         0x05U

#if (defined E_SAFETY_NODATA_OK)
#error E_SAFETY_NODATA_OK already defined
#endif
/** \brief No data are available since initialization of transformer. */
#define E_SAFETY_NODATA_OK         0x20U

#if (defined E_SAFETY_NODATA_REP)
#error E_SAFETY_NODATA_REP already defined
#endif
/** \brief No data are available since initialization of transformer because a repeated counter
 *         was received. */
#define E_SAFETY_NODATA_REP        0x21U

#if (defined E_SAFETY_NODATA_SEQ)
#error E_SAFETY_NODATA_SEQ already defined
#endif
/** \brief No data are available since initialization of transformer and a counter jump
 *         occurred. */
#define E_SAFETY_NODATA_SEQ        0x22U

#if (defined E_SAFETY_NODATA_ERR)
#error E_SAFETY_NODATA_ERR already defined
#endif
/** \brief No data are available since initialization of transformer. Therefore the check
 *         failed. */
#define E_SAFETY_NODATA_ERR        0x23U

#if (defined E_SAFETY_NODATA_NND)
#error E_SAFETY_NODATA_NND already defined
#endif
/** \brief No data are available since initialization of transformer. */
#define E_SAFETY_NODATA_NND        0x25U

#if (defined E_SAFETY_INIT_OK)
#error E_SAFETY_INIT_OK already defined
#endif
/** \brief Not enough data were received to use them. */
#define E_SAFETY_INIT_OK           0x30U

#if (defined E_SAFETY_INIT_REP)
#error E_SAFETY_INIT_REP already defined
#endif
/** \brief Not enough data were received to use them but some with a repeated counter were
 *         received. */
#define E_SAFETY_INIT_REP          0x31U

#if (defined E_SAFETY_INIT_SEQ)
#error E_SAFETY_INIT_SEQ already defined
#endif
/** \brief Not enough data were received to use them, additionally a counter jump occurred. */
#define E_SAFETY_INIT_SEQ          0x32U

#if (defined E_SAFETY_INIT_ERR)
#error E_SAFETY_INIT_ERR already defined
#endif
/** \brief Not enough data were received to use them, additionally a check failed. */
#define E_SAFETY_INIT_ERR          0x33U

#if (defined E_SAFETY_INIT_NND)
#error E_SAFETY_INIT_NND already defined
#endif
/** \brief Not enough data were received to use them, additionally no new data received. */
#define E_SAFETY_INIT_NND          0x35U

#if (defined E_SAFETY_INVALID_OK)
#error E_SAFETY_INVALID_OK already defined
#endif
/** \brief The data are invalid and cannot be used. */
#define E_SAFETY_INVALID_OK        0x40U

#if (defined E_SAFETY_INVALID_REP)
#error E_SAFETY_INVALID_REP already defined
#endif
/** \brief The data are invalid and cannot be used because a repeated counter was received. */
#define E_SAFETY_INVALID_REP       0x41U

#if (defined E_SAFETY_INVALID_SEQ)
#error E_SAFETY_INVALID_SEQ already defined
#endif
/** \brief The data are invalid and cannot be used due to a counter jump. */
#define E_SAFETY_INVALID_SEQ       0x42U

#if (defined E_SAFETY_INVALID_ERR)
#error E_SAFETY_INVALID_ERR already defined
#endif
/** \brief The data are invalid and cannot be used because a check failed. */
#define E_SAFETY_INVALID_ERR       0x43U

#if (defined E_SAFETY_INVALID_NND)
#error E_SAFETY_INVALID_NND already defined
#endif
/** \brief Communication is invalid according to safety and no new data received. */
#define E_SAFETY_INVALID_NND       0x45U

#if (defined E_SAFETY_SOFT_RUNTIMEERROR)
#error E_SAFETY_SOFT_RUNTIMEERROR already defined
#endif
/** \brief A runtime error occurred, safety properties could not be checked (state or status cannot
 *         be determined) but non-protected output data could be produced nonetheless. */
#define E_SAFETY_SOFT_RUNTIMEERROR 0x77U

#if (defined E_SAFETY_HARD_RUNTIMEERROR)
#error E_SAFETY_HARD_RUNTIMEERROR already defined
#endif
/** \brief A runtime error occurred, safety properties could not be checked and NO output data
 *         could be produced. */
#define E_SAFETY_HARD_RUNTIMEERROR 0xFFU


/* -------------------- error codes (SWS_Xfrm_00033) -----------------------*/

#if (defined E_SEC_NOT_AUTH)
#error E_SEC_NOT_AUTH already defined
#endif
/** \brief The data was not authenticated correctly. */
#define E_SEC_NOT_AUTH             0x01U

#if (defined E_SEC_NOT_FRESH)
#error E_SEC_NOT_FRESH already defined
#endif
/** \brief The data was not fresh.*/
#define E_SEC_NOT_FRESH            0x02U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
#endif /* ifndef TRANSFORMER_TYPES_H */
