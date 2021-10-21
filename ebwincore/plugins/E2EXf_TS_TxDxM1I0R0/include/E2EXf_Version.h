#ifndef E2EXF_VERSION_H
#define E2EXF_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR E2EXf
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EXf.
 *
 * \version 1.0.36
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

#if (defined E2EXF_VENDOR_ID)
#error E2EXF_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define E2EXF_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined E2EXF_MODULE_ID)
#error E2EXF_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define E2EXF_MODULE_ID         176U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined E2EXF_AR_RELEASE_MAJOR_VERSION)
#error E2EXF_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define E2EXF_AR_RELEASE_MAJOR_VERSION     4U

#if (defined E2EXF_AR_RELEASE_MINOR_VERSION)
#error E2EXF_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define E2EXF_AR_RELEASE_MINOR_VERSION     2U

#if (defined E2EXF_AR_RELEASE_REVISION_VERSION)
#error E2EXF_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define E2EXF_AR_RELEASE_REVISION_VERSION  1U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined E2EXF_SW_MAJOR_VERSION)
#error E2EXF_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define E2EXF_SW_MAJOR_VERSION             1U

#if (defined E2EXF_SW_MINOR_VERSION)
#error E2EXF_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define E2EXF_SW_MINOR_VERSION             0U

#if (defined E2EXF_SW_PATCH_VERSION)
#error E2EXF_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define E2EXF_SW_PATCH_VERSION             36U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef E2EXF_VERSION_H */
/*==================[end of file]============================================*/
