#ifndef STD_TYPES_H
/* !LINKSTO Base.ASR403.STD003,1, Base.ASR403.STD014,1 */
#define STD_TYPES_H

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


/* !LINKSTO Base.ASR403.STD004,1 */

/*==================[inclusions]=============================================*/

/* !LINKSTO Base.ASR403.STD001,1 */
#include <Platform_Types.h> /* Autosar platform specific type declarations */
#include <Compiler.h>       /* Autosar compiler specific declarations */

/*==================[macros]=================================================*/

/* !LINKSTO Base.ASR403.STD007,1 */
#if (defined STD_HIGH)
#error STD_HIGH already defined
#endif

/** \brief physical state 5V or 3.3V  */
#define STD_HIGH 1U

#if (defined STD_LOW)
#error STD_LOW already defined
#endif

/** \brief physical state 0V */
#define STD_LOW 0U

/* !LINKSTO Base.ASR403.STD013,1 */
#if (defined STD_ACTIVE)
#error STD_ACTIVE already defined
#endif

/** \brief Autosar logical state 'active' */
#define STD_ACTIVE 1U

#if (defined STD_IDLE)
#error STD_IDLE already defined
#endif

/** \brief Autosar logical state 'idle' */
#define STD_IDLE 0U

/* !LINKSTO Base.ASR403.STD010,1 */
#if (defined STD_ON)
#error STD_ON already defined
#endif

/** \brief Autosar definition for 'on' */
#define STD_ON 1U

#if (defined STD_OFF)
#error STD_OFF already defined
#endif

/** \brief Autosar definition for 'off' */
#define STD_OFF 0U

#if (defined STD_MESSAGETYPE_REQUEST)
#error STD_MESSAGETYPE_REQUEST already defined
#endif

/** \brief Autosar definition for message Type 'Request' */
#define STD_MESSAGETYPE_REQUEST 0U

#if (defined STD_MESSAGETYPE_RESPONSE)
#error STD_MESSAGETYPE_RESPONSE already defined
#endif

/** \brief Autosar definition for message Type 'Response' */
#define STD_MESSAGETYPE_RESPONSE 1U

#if (defined STD_MESSAGERESULT_OK)
#error STD_MESSAGERESULT_OK already defined
#endif

/** \brief Autosar definition for message Result Type 'Ok' */
#define STD_MESSAGERESULT_OK 0U

#if (defined STD_MESSAGERESULT_ERROR)
#error STD_MESSAGERESULT_ERROR already defined
#endif

/** \brief Autosar definition for message Result Type 'Error' */
#define STD_MESSAGERESULT_ERROR 1U 

#ifndef STATUSTYPEDEFINED
/** \brief Guard macro for type definition of StatusType */
#define STATUSTYPEDEFINED
/** \brief Definition StatusType (OSEK compliance) */
typedef unsigned char StatusType;
#endif

/* !LINKSTO Base.ASR403.STD006,1 */
#ifndef E_OK
/** \brief Constant for value OK of StatusType */
/* !LINKSTO Base.ASR403.STD011,1 */
#define E_OK 0U
#endif

#ifndef E_NOT_OK
/** \brief Constant for value NOT_OK of StatusType */
/* !LINKSTO Base.ASR403.STD011,1 */
#define E_NOT_OK 1U
#endif

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined STD_VENDOR_ID)
#error STD_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define STD_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined STD_MODULE_ID)
#error STD_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification
 *
 * Left empty as this header does not belong to any module
 */
#define STD_MODULE_ID         197U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined STD_AR_RELEASE_MAJOR_VERSION)
#error STD_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define STD_AR_RELEASE_MAJOR_VERSION     4U

#if (defined STD_AR_RELEASE_MINOR_VERSION)
#error STD_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define STD_AR_RELEASE_MINOR_VERSION     0U

#if (defined STD_AR_RELEASE_REVISION_VERSION)
#error STD_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define STD_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined STD_SW_MAJOR_VERSION)
#error STD_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define STD_SW_MAJOR_VERSION     1U

#if (defined STD_SW_MINOR_VERSION)
#error STD_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define STD_SW_MINOR_VERSION     3U

#if (defined STD_SW_PATCH_VERSION)
#error STD_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define STD_SW_PATCH_VERSION     0U

/*------------------[AUTOSAR module vendor API infix]------------------------*/

#if (defined STD_VENDOR_API_INFIX)
#error STD_VENDOR_API_INFIX already defined
#endif
/** \brief Vendor API infix
 *
 * Left empty as this header does not belong to any module
 */
#define STD_VENDOR_API_INFIX

/*==================[type definitions]=======================================*/

/** \brief Autosar standard API return type */
/* !LINKSTO Base.ASR403.STD005,1 */
typedef uint8 Std_ReturnType;

/** \brief Autosar standard Message Type type */
/* Currently is limited to the distinction between requests and responses in C/S communication. */
/* !LINKSTO Base.ASR20-11.SWS_Std_91001,1 */
typedef uint8 Std_MessageTypeType;

/** \brief Autosar standard Message Result type */
/* Currently is limited to the distinction between requests and responses in C/S communication. */
/* !LINKSTO Base.ASR20-11.SWS_Std_91002,1 */
typedef uint8 Std_MessageResultType;

#ifndef STD_VERSION_INFO_TYPE_DEFINED
/** \brief Guard for definition of Std_VersionInfoType */
#define STD_VERSION_INFO_TYPE_DEFINED STD_OFF
#endif

/* !LINKSTO Base.CustomStdVersionInfoType.ConfigMacro,1 */
#if (STD_VERSION_INFO_TYPE_DEFINED != STD_ON)

/** \brief return type for xxx_GetVersionInfo() functions of each BSW module
 *
 * This type is used to request the version of BSW module using the
 * xxx_GetVersionInfo() function.
 */
/* !LINKSTO Base.ASR403.STD015,1 */
typedef struct
{
   uint16 vendorID;
   uint16 moduleID;
   uint8  sw_major_version;
   uint8  sw_minor_version;
   uint8  sw_patch_version;
} Std_VersionInfoType;

#endif /* #if (STD_VERSION_INFO_TYPE_DEFINED != STD_ON) */

/** \brief Autosar Standard ExtractProtocolHeaderFields API ptr type */
/* !LINKSTO Base.ASR20-11.SWS_Std_91003,1 */
typedef P2FUNC(Std_ReturnType, SOMEIPXF_APPL_DATA, Std_ExtractProtocolHeaderFieldsType)
(
   P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) buffer,
   uint32 bufferLength,
   P2VAR(Std_MessageTypeType, AUTOMATIC, SOMEIPXF_APPL_DATA) messageType,
   P2VAR(Std_MessageResultType, AUTOMATIC, SOMEIPXF_APPL_DATA) messageResult
);

/*==================[end of file]============================================*/
#endif /* ifndef STD_TYPES_H */
