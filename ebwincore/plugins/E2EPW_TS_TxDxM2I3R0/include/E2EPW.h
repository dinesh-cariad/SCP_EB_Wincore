/**
 * \file
 *
 * \brief AUTOSAR E2EPW
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EPW.
 *
 * \version 2.3.20
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#if (!defined E2EPW_H)
#define E2EPW_H

/*==[Includes]================================================================*/

#include <Std_Types.h>              /* AUTOSAR Std types */

/*==[Macros]==================================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined E2EPW_VENDOR_ID) /* to prevent double declaration */
#error E2EPW_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define E2EPW_VENDOR_ID         1U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined E2EPW_AR_RELEASE_MAJOR_VERSION) /* to prevent double declaration */
#error E2EPW_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR specification major version */
#define E2EPW_AR_RELEASE_MAJOR_VERSION  4U

#if (defined E2EPW_AR_RELEASE_MINOR_VERSION) /* to prevent double declaration */
#error E2EPW_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR specification minor version */
#define E2EPW_AR_RELEASE_MINOR_VERSION  0U

#if (defined E2EPW_AR_RELEASE_REVISION_VERSION) /* to prevent double declaration */
#error E2EPW_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR specification patch version */
#define E2EPW_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR specification version identification]----------*/

#if (defined E2EPW_AR_MAJOR_VERSION) /* to prevent double declaration */
#error E2EPW_AR_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release  major version */
#define E2EPW_AR_MAJOR_VERSION  2U

#if (defined E2EPW_AR_MINOR_VERSION) /* to prevent double declaration */
#error E2EPW_AR_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define E2EPW_AR_MINOR_VERSION  0U

#if (defined E2EPW_AR_PATCH_VERSION) /* to prevent double declaration */
#error E2EPW_AR_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define E2EPW_AR_PATCH_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined E2EPW_SW_MAJOR_VERSION) /* to prevent double declaration */
#error E2EPW_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define E2EPW_SW_MAJOR_VERSION  2U

#if (defined E2EPW_SW_MINOR_VERSION) /* to prevent double declaration */
#error E2EPW_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define E2EPW_SW_MINOR_VERSION  3U

#if (defined E2EPW_SW_PATCH_VERSION) /* to prevent double declaration */
#error E2EPW_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define E2EPW_SW_PATCH_VERSION  20U

/*------------------[E2EPW Static Interface version]-------------------------*/

#if (defined E2EPW_STATICIF_VERSION) /* to prevent double declaration */
#error E2EPW_STATICIF_VERSION already defined
#endif
/** \brief E2EPW Static interface version */
#define E2EPW_STATICIF_VERSION          6

#if (defined E2EPW_GENIF_VERSION_REQ) /* to prevent double declaration */
#error E2EPW_GENIF_VERSION_REQ already defined
#endif
/** \brief E2EPW Generated interface version required by component E2EPW Static */
#define E2EPW_GENIF_VERSION_REQ         6

#if (defined E2EPW_GENIF_VERSION_CHECK) /* to prevent double declaration */
#error E2EPW_GENIF_VERSION_CHECK already defined
#endif

/** \brief Interface compatibility check macro for component E2EPW Generated
 *
 * \param[in] staticif_req the required interface version of E2EPW Static
 * \param[in] generatedif the interface version of E2EPW Generated
 *
 * \retval TRUE The two components E2EPW Static and E2EPW Generated are compatible
 * \retval FALSE The two components E2EPW Static and E2EPW Generated are not compatible
 */
#define E2EPW_GENIF_VERSION_CHECK(staticif_req, generatedif)   \
    ((E2EPW_STATICIF_VERSION >= (staticif_req)) &&             \
     (E2EPW_GENIF_VERSION_REQ <= (generatedif)))

/*------------------[E2E Error Classification]-------------------------------*/

#if (defined E2EPW_STATUS_OK) /* To prevent double declaration */
#error E2EPW_STATUS_OK already defined
#endif /* if (defined E2EPW_STATUS_OK) */

#if (defined E2EPW_STATUS_NONEWDATA) /* To prevent double declaration */
#error E2EPW_STATUS_NONEWDATA already defined
#endif /* if (defined E2EPW_STATUS_NONEWDATA) */

#if (defined E2EPW_STATUS_WRONGCRC) /* To prevent double declaration */
#error E2EPW_STATUS_WRONGCRC already defined
#endif /* if (defined E2EPW_STATUS_WRONGCRC) */

#if (defined E2EPW_STATUS_SYNC) /* To prevent double declaration */
#error E2EPW_STATUS_SYNC already defined
#endif /* if (defined E2EPW_STATUS_SYNC) */

#if (defined E2EPW_STATUS_INITIAL) /* To prevent double declaration */
#error E2EPW_STATUS_INITIAL already defined
#endif /* if (defined E2EPW_STATUS_INITIAL) */

#if (defined E2EPW_STATUS_REPEATED) /* To prevent double declaration */
#error E2EPW_STATUS_REPEATED already defined
#endif /* if (defined E2EPW_STATUS_REPEATED) */

#if (defined E2EPW_STATUS_OKSOMELOST) /* To prevent double declaration */
#error E2EPW_STATUS_OKSOMELOST already defined
#endif /* if (defined E2EPW_STATUS_OKSOMELOST) */

#if (defined E2EPW_STATUS_WRONGSEQUENCE) /* To prevent double declaration */
#error E2EPW_STATUS_WRONGSEQUENCE already defined
#endif /* if (defined E2EPW_STATUS_WRONGSEQUENCE) */

#if (defined E2EPW_STATUS_DATAINVALID) /* To prevent double declaration */
#error E2EPW_STATUS_DATAINVALID already defined
#endif /* if (defined E2EPW_STATUS_DATAINVALID) */

#if (defined E2EPW_E_DESERIALIZATION) /* To prevent double declaration */
#error E2EPW_E_DESERIALIZATION already defined
#endif /* if (defined E2EPW_E_DESERIALIZATION) */

#if (defined E2EPW_E_REDUNDANCY) /* To prevent double declaration */
#error E2EPW_E_REDUNDANCY already defined
#endif /* if (defined E2EPW_E_REDUNDANCY) */

/** \brief The E2E library has been invoked and new Data is available. */
#define E2EPW_STATUS_OK            0x00U

/** \brief The E2E library has been invoked but new Data is not available since the last call. */
#define E2EPW_STATUS_NONEWDATA     0x01U

/** \brief The data has been received, but the CRC is incorrect. */
#define E2EPW_STATUS_WRONGCRC      0x02U

/** \brief New data has been received after detection of an unexpected behavior of counter. */
#define E2EPW_STATUS_SYNC          0x03U

/** \brief The E2E library E2E_PXXCheck() was called the very first time */
#define E2EPW_STATUS_INITIAL       0x04U

/** \brief New data has been correctly received
 * but is identical to most recent correctly received data. */
#define E2EPW_STATUS_REPEATED      0x08U

/** \brief The E2E library has been invoked and new Data is available, but
 * some Data in the sequence have been probably lost since the last correct/initial reception. */
#define E2EPW_STATUS_OKSOMELOST    0x20U

/** \brief New data has been correctly received, but the Counter Delta is too big. */
#define E2EPW_STATUS_WRONGSEQUENCE 0x40U

/** \brief The E2E library (only E2E Profile 01) recognized
 * that all bits in the application data (except CRC) are set to one. */
#define E2EPW_STATUS_DATAINVALID   0x80U

/** \brief COM/RTE did not correctly expand I-PDU into data elements */
#define E2EPW_E_DESERIALIZATION    0x03U

/** \brief voting error between Write1 and Write2 */
#define E2EPW_E_REDUNDANCY         0x05U

/*==[Types]===================================================================*/

/*==[Declaration of Constants with external linkage]==========================*/

/*==[Declaration of Variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/


#endif /* if !defined( E2EPW_H ) */
/*==================[end of file]============================================*/
