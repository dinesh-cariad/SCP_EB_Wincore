#ifndef COMXF_VERSION_H
#define COMXF_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR ComXf
 *
 * This file contains the implementation of the AUTOSAR
 * module ComXf.
 *
 * \version 1.0.37
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

#if (defined COMXF_VENDOR_ID)
#error COMXF_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define COMXF_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined COMXF_MODULE_ID)
#error COMXF_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define COMXF_MODULE_ID         175U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined COMXF_AR_RELEASE_MAJOR_VERSION)
#error COMXF_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define COMXF_AR_RELEASE_MAJOR_VERSION     4U

#if (defined COMXF_AR_RELEASE_MINOR_VERSION)
#error COMXF_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define COMXF_AR_RELEASE_MINOR_VERSION     2U

#if (defined COMXF_AR_RELEASE_REVISION_VERSION)
#error COMXF_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define COMXF_AR_RELEASE_REVISION_VERSION  1U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined COMXF_SW_MAJOR_VERSION)
#error COMXF_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define COMXF_SW_MAJOR_VERSION             1U

#if (defined COMXF_SW_MINOR_VERSION)
#error COMXF_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define COMXF_SW_MINOR_VERSION             0U

#if (defined COMXF_SW_PATCH_VERSION)
#error COMXF_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define COMXF_SW_PATCH_VERSION             37U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef COMXF_VERSION_H */
/*==================[end of file]============================================*/
