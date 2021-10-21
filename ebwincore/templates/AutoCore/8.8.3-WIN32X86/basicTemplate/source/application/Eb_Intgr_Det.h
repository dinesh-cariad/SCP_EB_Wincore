/**
 * \file
 *
 * \brief AUTOSAR ApplTemplates
 *
 * This file contains the implementation of the AUTOSAR
 * module ApplTemplates.
 *
 * \version 6.8.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef EB_INTGR_DET_H
#define EB_INTGR_DET_H

/*==================[includes]==============================================*/

#include <Base_Modules.h>

/*==================[macros]================================================*/

#if (defined EB_INTGR_DEV_ERROR_DETECT)
#error EB_INTGR_DEV_ERROR_DETECT already defined
#endif /* if (defined EB_INTGR_DEV_ERROR_DETECT) */

/** \brief Definition to enable EB integration code Det calls **/
#if defined(BASE_DET_ENABLED) && (BASE_DET_ENABLED==STD_ON)
#define EB_INTGR_DEV_ERROR_DETECT               STD_ON
#else
#define EB_INTGR_DEV_ERROR_DETECT               STD_OFF
#endif


#if (defined EB_INTGR_MODULE_ID)
#error EB_INTGR_MODULE_ID already defined
#endif /* if (defined EB_INTGR_MODULE_ID) */

/** \brief Definition of module id for EB integration code **/
#define EB_INTGR_MODULE_ID                      0xEBEBU


#if (defined EB_INTGR_INSTANCE_ID)
#error EB_INTGR_INSTANCE_ID already defined
#endif /* if (defined EB_INTGR_INSTANCE_ID) */

/** \brief Definition of instance id for EB integration code **/
#define EB_INTGR_INSTANCE_ID                    1U


#if (defined EB_INTGR_API_ECUM_CB_NFYNVMJOBEND)
#error EB_INTGR_API_ECUM_CB_NFYNVMJOBEND already defined
#endif /* if (defined EB_INTGR_API_ECUM_CB_NFYNVMJOBEND) */

/** \brief Definition of API id for EB integration code **/
#define EB_INTGR_API_ECUM_CB_NFYNVMJOBEND       1U


#if (defined EB_INTGR_E_WRONG_SERVICEID)
#error EB_INTGR_E_WRONG_SERVICEID already defined
#endif /* if (defined EB_INTGR_E_WRONG_SERVICEID) */

/** \brief Definition of module id for EB integration code **/
#define EB_INTGR_E_WRONG_SERVICEID              1U


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef EB_INTGR_DET_H */
