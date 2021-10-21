#ifndef SOMEIPXF_VERSION_H
#define SOMEIPXF_VERSION_H
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

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined SOMEIPXF_VENDOR_ID)
#error SOMEIPXF_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define SOMEIPXF_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined SOMEIPXF_MODULE_ID)
#error SOMEIPXF_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define SOMEIPXF_MODULE_ID         174U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined SOMEIPXF_AR_RELEASE_MAJOR_VERSION)
#error SOMEIPXF_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define SOMEIPXF_AR_RELEASE_MAJOR_VERSION     4U

#if (defined SOMEIPXF_AR_RELEASE_MINOR_VERSION)
#error SOMEIPXF_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define SOMEIPXF_AR_RELEASE_MINOR_VERSION     2U

#if (defined SOMEIPXF_AR_RELEASE_REVISION_VERSION)
#error SOMEIPXF_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define SOMEIPXF_AR_RELEASE_REVISION_VERSION  1U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined SOMEIPXF_SW_MAJOR_VERSION)
#error SOMEIPXF_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define SOMEIPXF_SW_MAJOR_VERSION             1U

#if (defined SOMEIPXF_SW_MINOR_VERSION)
#error SOMEIPXF_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define SOMEIPXF_SW_MINOR_VERSION             0U

#if (defined SOMEIPXF_SW_PATCH_VERSION)
#error SOMEIPXF_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define SOMEIPXF_SW_PATCH_VERSION             47U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef SOMEIPXF_VERSION_H */
/*==================[end of file]============================================*/
