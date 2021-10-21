#ifndef SOMEIPXF_S_API_STC_H
#define SOMEIPXF_S_API_STC_H

/**
 * \file
 *
 * \brief AUTOSAR SomeIpXf
 *
 * This file contains the implementation of the AUTOSAR
 * module SomeIpXf.
 *
 * \version 1.0.47
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * ## operator is used for code generation via pre-processor.
 *
 */

/*==================[inclusions]============================================*/
#include <SomeIpXf_Cfg.h>              /* Generated configuration */

/*==================[macros]================================================*/

#if (defined SOMEIPXF_XFRMBUFFERLENGTH_TYPE) /* to prevent double declaration */
#error SOMEIPXF_XFRMBUFFERLENGTH_TYPE already defined
#endif

#if (SOMEIPXF_XFRMBUFFERLENGTHTYPE_ISUINT32 == STD_ON)
#define SOMEIPXF_XFRMBUFFERLENGTH_TYPE uint32
#else
#define SOMEIPXF_XFRMBUFFERLENGTH_TYPE uint16
#endif

/* -------------------- SOMEIPXF message types (SWS_SomeIpXf_00031) ----------*/

#if (defined SOMEIPXF_MESSAGE_TYPE_REQUEST)
#error SOMEIPXF_MESSAGE_TYPE_REQUEST already defined
#endif
/** \brief A request expecting a response */
#define SOMEIPXF_MESSAGE_TYPE_REQUEST                             0x00U

#if (defined SOMEIPXF_MESSAGE_TYPE_REQUEST_NO_RETURN)
#error SOMEIPXF_MESSAGE_TYPE_REQUEST_NO_RETURN already defined
#endif
/** \brief A fire&forget request */
#define SOMEIPXF_MESSAGE_TYPE_REQUEST_NO_RETURN                   0x01U

#if (defined SOMEIPXF_MESSAGE_TYPE_NOTIFICATION)
#error SOMEIPXF_MESSAGE_TYPE_NOTIFICATION already defined
#endif
/** \brief A request of a notification expecting no response */
#define SOMEIPXF_MESSAGE_TYPE_NOTIFICATION                        0x02U

#if (defined SOMEIPXF_MESSAGE_TYPE_RESPONSE)
#error SOMEIPXF_MESSAGE_TYPE_RESPONSE already defined
#endif
/** \brief The response message */
#define SOMEIPXF_MESSAGE_TYPE_RESPONSE                            0x80U

#if (defined SOMEIPXF_MESSAGE_TYPE_ERROR)
#error SOMEIPXF_MESSAGE_TYPE_ERROR already defined
#endif
/** \brief The response containing an error */
#define SOMEIPXF_MESSAGE_TYPE_ERROR                               0x81U

/* -------------------- END OF SOMEIPXF message types (SWS_SomeIpXf_00031) ----------*/

/*==================[Error codes]===========================================*/

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef SOMEIPXF_S_API_STC_H */
/*==================[end of file]===========================================*/
