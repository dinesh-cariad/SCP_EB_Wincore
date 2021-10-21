/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef COM_H
#define COM_H
/*==================[inclusions]============================================*/


#include <TSAutosar.h>          /* EB specific standard types */
#include <Com_Api.h>            /* Module public API */
#include <Com_Version.h>        /* Module version declarations */

#include <Com_GenApi_Static.h>

#if ((COM_TX_TPAPI_ENABLE == STD_ON) || (COM_RX_TPAPI_ENABLE == STD_ON))
#include <EcuC.h>
#endif /* ((COM_TX_TPAPI_ENABLE == STD_ON) || (COM_RX_TPAPI_ENABLE == STD_ON)) */

/* Exclude post-build-time config include file if requested to do so */
#if (!defined COM_NO_PBCFG_REQUIRED) && (!defined COM_NO_CFG_REQUIRED)
#if (COM_PBCFGM_SUPPORT_ENABLED == STD_OFF)
/* Exclude post-build-time config include file if PbcfgM support is enabled */
#include <Com_PBcfg.h>
#endif /* (COM_PBCFGM_SUPPORT_ENABLED == STD_OFF) */
#include <Com_GenApi.h>
#endif /* (!defined COM_NO_PBCFG_REQUIRED) && (!defined COM_NO_CFG_REQUIRED) */


#undef COM_NO_PBCFG_REQUIRED
#undef COM_NO_CFG_REQUIRED


/*==================[version check]=========================================*/

/*==================[macros]================================================*/

/*==================[macro checks]==========================================*/

/* check for defined STD_ON macro value */
#ifndef STD_ON
#error STD_ON not defined
#endif /* STD_ON */

/* check for defined STD_OFF macro value */
#ifndef STD_OFF
#error STD_OFF not defined
#endif /* STD_OFF */

/* check for correctly defined switch COM_VERSION_INFO_API */
#ifndef COM_VERSION_INFO_API
#error macro COM_VERSION_INFO_API not defined
#endif /* COM_VERSION_INFO_API */

/* test for valid value of COM_VERSION_INFO_API */
#if (COM_VERSION_INFO_API != STD_ON) && \
    (COM_VERSION_INFO_API != STD_OFF)
#error macro COM_VERSION_INFO_API has an invalid value
#endif /* COM_VERSION_INFO_API */

/* check for correctly defined switch COM_DEV_ERROR_DETECT */
#ifndef COM_DEV_ERROR_DETECT
#error macro COM_DEV_ERROR_DETECT not defined
#endif /* COM_DEV_ERROR_DETECT */

/* test for valid value of COM_DEV_ERROR_DETECT */
#if (COM_DEV_ERROR_DETECT != STD_ON) && \
    (COM_DEV_ERROR_DETECT != STD_OFF)
#error macro COM_DEV_ERROR_DETECT has an invalid value
#endif /* COM_DEV_ERROR_DETECT */

#if ((COM_TX_TPAPI_ENABLE == STD_ON) || (COM_RX_TPAPI_ENABLE == STD_ON))

#if (defined COM_ECUC_SW_MAJOR_VERSION) /* to prevent double declaration */
#error COM_ECUC_SW_MAJOR_VERSION already defined
#endif
/** \brief Com module largest compatible EcuC AUTOSAR module major version  */
#define COM_ECUC_SW_MAJOR_VERSION          5U

#if (defined COM_ECUC_SW_MINOR_VERSION) /* to prevent double declaration */
#error COM_ECUC_SW_MINOR_VERSION already defined
#endif
/** \brief Com module largest compatible EcuC AUTOSAR module minor version  */
#define COM_ECUC_SW_MINOR_VERSION          0U

#if (defined COM_ECUC_SW_PATCH_VERSION) /* to prevent double declaration */
#error COM_ECUC_SW_PATCH_VERSION already defined
#endif
/** \brief Com module largest compatible EcuC AUTOSAR module patch version  */
#define COM_ECUC_SW_PATCH_VERSION          11U

/** \brief EcuC library compatibility check */
#if !ECUC_COMPATIBILITY_VERSION_CHECK(COM_ECUC_SW_MAJOR_VERSION, COM_ECUC_SW_MINOR_VERSION, COM_ECUC_SW_PATCH_VERSION)
#error Version check between Com module and EcuC library failed
#endif

#endif /* ((COM_TX_TPAPI_ENABLE == STD_ON) || (COM_RX_TPAPI_ENABLE == STD_ON)) */
/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef COM_H */
/*==================[end of file]===========================================*/
