/* !LINKSTO SWS_Crc_00050,1, SWS_Crc_00048,1 */
#ifndef CRC_VERSION_H
#define CRC_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Crc
 *
 * This file contains the implementation of the AUTOSAR
 * module Crc.
 *
 * \version 6.11.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined CRC_VENDOR_ID)
#error CRC_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define CRC_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined CRC_MODULE_ID)
#error CRC_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define CRC_MODULE_ID         201U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined CRC_AR_RELEASE_MAJOR_VERSION)
#error CRC_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define CRC_AR_RELEASE_MAJOR_VERSION     4U

#if (defined CRC_AR_RELEASE_MINOR_VERSION)
#error CRC_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define CRC_AR_RELEASE_MINOR_VERSION     0U

#if (defined CRC_AR_RELEASE_REVISION_VERSION)
#error CRC_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define CRC_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined CRC_SW_MAJOR_VERSION)
#error CRC_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define CRC_SW_MAJOR_VERSION             6U

#if (defined CRC_SW_MINOR_VERSION)
#error CRC_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define CRC_SW_MINOR_VERSION             11U

#if (defined CRC_SW_PATCH_VERSION)
#error CRC_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define CRC_SW_PATCH_VERSION             13U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef CRC_VERSION_H */
/*==================[end of file]============================================*/
