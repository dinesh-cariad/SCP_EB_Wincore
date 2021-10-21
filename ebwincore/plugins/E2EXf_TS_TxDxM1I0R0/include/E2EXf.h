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
#ifndef E2EXF_H
#define E2EXF_H

/*==================[inclusions]============================================*/

#include <E2EXf_Cfg.h>       /* Generated configuration */
#include <E2EXf_Api.h>       /* Generated APIs of partitions */
#include <E2EXf_Version.h>   /* Module version info */

/*==================[macros]================================================*/

#if (defined E2EXF_SID_GETVERSIONINFO)          /* To prevent double declaration */
#error E2EXF_SID_GETVERSIONINFO already defined
#endif /* if (defined E2EXF_SID_GETVERSIONINFO) */

#if (defined E2EXF_SID_TRANSFORMER)          /* To prevent double declaration */
#error E2EXF_SID_TRANSFORMER already defined
#endif /* if (defined E2EXF_SID_TRANSFORMER) */

#if (defined E2EXF_SID_INV_TRANSFORMER)          /* To prevent double declaration */
#error E2EXF_SID_INV_TRANSFORMER already defined
#endif /* if (defined E2EXF_SID_INV_TRANSFORMER) */

/* API Service ID's */
/** \brief API Service ID for E2EXf_GetVersioninfo() */
#define E2EXF_SID_GETVERSIONINFO               ((uint8)0x00U)
/** \brief API Service ID for E2EXf_transformerId()
 ** communication */
#define E2EXF_SID_TRANSFORMER                  ((uint8)0x03U)
/** \brief API Service ID for E2EXf_Inv_transformerId()
 ** communication */
#define E2EXF_SID_INV_TRANSFORMER              ((uint8)0x04U)

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define E2EXF_START_SEC_CODE
#include <E2EXf_MemMap.h>

/*--------------------- E2EXf_GetVersionInfo ----------------------------*/
#if (E2EXF_VERSION_INFO_API == STD_ON)
/** \brief Get version information of the E2EXf module.
**
** This service returns the version information of this module. The version
** information includes:
**  - Module Id
**  - Vendor Id
**  - Vendor specific version numbers
**
** \param[out] VersionInfo Pointer to where to store the version
**                         information of this module.
**
** \ServiceID{0x00}
** \Reentrancy{Reentrant}
** \Synchronicity{Synchronous}
*/
extern FUNC(void, E2EXF_CODE) E2EXf_GetVersionInfo
(
    P2VAR(Std_VersionInfoType, AUTOMATIC, E2EXF_APPL_DATA) VersionInfo
);
#endif /* (E2EXF_VERSION_INFO_API == STD_ON) */


#define E2EXF_STOP_SEC_CODE
#include <E2EXf_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef E2EXF_H */
/*==================[end of file]===========================================*/
