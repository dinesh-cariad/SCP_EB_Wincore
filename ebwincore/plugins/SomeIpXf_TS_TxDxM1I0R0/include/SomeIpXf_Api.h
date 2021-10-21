#ifndef SOMEIPXF_API_H
#define SOMEIPXF_API_H

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

/*==================[inclusions]============================================*/

#include <SomeIpXf_Version.h>   /* Module Version Info */
#include <SomeIpXf_Cfg.h>       /* Module Configuration */

/*==================[macros]================================================*/

/* -------------------- SOMEIPXF return codes (SWS_SomeIpXf_00115) -----------*/

#if (defined SOMEIPXF_E_UNKNOWN_SERVICE)
#error SOMEIPXF_E_UNKNOWN_SERVICE already defined
#endif
/** \brief The requested Service ID is unknown. */
#define SOMEIPXF_E_UNKNOWN_SERVICE            0x02U

#if (defined SOMEIPXF_E_UNKNOWN_METHOD)
#error SOMEIPXF_E_UNKNOWN_METHOD already defined
#endif
/** \brief The requested Method ID is unknown. Service ID is known. */
#define SOMEIPXF_E_UNKNOWN_METHOD             0x03U

#if (defined SOMEIPXF_E_NOT_READY)
#error SOMEIPXF_E_NOT_READY already defined
#endif
/** \brief Service ID and Method ID are known. Application not running. */
#define SOMEIPXF_E_NOT_READY                  0x04U

#if (defined SOMEIPXF_E_NOT_REACHABLE)
#error SOMEIPXF_E_NOT_REACHABLE already defined
#endif
/** \brief System running the service is not reachable (internal error code only). */
#define SOMEIPXF_E_NOT_REACHABLE              0x05U

#if (defined SOMEIPXF_E_TIMEOUT)
#error SOMEIPXF_E_TIMEOUT already defined
#endif
/** \brief A timeout occurred (internal error code only). */
#define SOMEIPXF_E_TIMEOUT                    0x06U

#if (defined SOMEIPXF_E_WRONG_PROTOCOL_VERSION)
#error SOMEIPXF_E_WRONG_PROTOCOL_VERSION already defined
#endif
/** \brief Version of SOME/IP protocol not supported. */
#define SOMEIPXF_E_WRONG_PROTOCOL_VERSION     0x07U

#if (defined SOMEIPXF_E_WRONG_INTERFACE_VERSION)
#error SOMEIPXF_E_WRONG_INTERFACE_VERSION already defined
#endif
/** \brief Interface version mismatch. */
#define SOMEIPXF_E_WRONG_INTERFACE_VERSION    0x08U

#if (defined SOMEIPXF_E_MALFORMED_MESSAGE)
#error SOMEIPXF_E_MALFORMED_MESSAGE already defined
#endif
/** \brief Deserialization error, so that payload cannot be deserialized. */
#define SOMEIPXF_E_MALFORMED_MESSAGE          0x09U

#if (defined SOMEIPXF_E_WRONG_MESSAGE_TYPE)
#error SOMEIPXF_E_WRONG_MESSAGE_TYPE already defined
#endif
/** \brief An unexpected message type was received (e.g. SOMEIPXF_MESSAGE_TYPE_REQUEST_NO_RETURN for
 **  a method defined as SOMEIPXF_MESSAGE_TYPE_REQUEST.) */
#define SOMEIPXF_E_WRONG_MESSAGE_TYPE         0x0aU

/*==================[Error codes]===========================================*/

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/
/** \brief This is the type of the data structure used for initialization as specified by AUTOSAR.
 ** Since the SomeIpXf module is not postbuild capable, the type remains for conformance reasons.
 */
typedef VAR( uint8, TYPEDEF ) SomeIpXf_ConfigType;

/*==================[external function declarations]========================*/
#define SOMEIPXF_START_SEC_CODE
#include <SomeIpXf_MemMap.h>

#if (SOMEIPXF_VERSION_INFO_API == STD_ON)

/** \brief Get version information of the SomeIpXf module.
**
** This service returns the version information of this module. The version
** information includes:
**  - Module Id
**  - Vendor Id
**  - Vendor specific version numbers
**
** \param[out] versioninfo Pointer to where to store the version
**                         information of this module.
**
** \ServiceID{0x00}
** \Reentrancy{Non-Reentrant}
** \Synchronicity{Synchronous}
*/
extern FUNC(void, SOMEIPXF_CODE) SomeIpXf_GetVersionInfo
(
P2VAR(Std_VersionInfoType, AUTOMATIC, SOMEIPXF_APPL_DATA) versioninfo
);
#endif /* SOMEIPXF_VERSION_INFO_API == STD_ON */


#define SOMEIPXF_STOP_SEC_CODE
#include <SomeIpXf_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef SOMEIPXF_API_H */
/*==================[end of file]===========================================*/
