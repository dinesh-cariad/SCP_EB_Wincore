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

#ifndef EB_INTGR_MCUARCHCLOCKINIT_H
#define EB_INTGR_MCUARCHCLOCKINIT_H

/* This header file is included by EcuM_Cfg.c. Through this header
 * files, EcuM_Cfg.c gets the declaration of the function
 * MyMcuArchClockInit_Init() and also gets the defines for the platform-
 * specific configuration pointer names/init string via the includes
 * of Platforms_Modules.h and Base_Modules.h.
 */

/*==================[inclusions]============================================*/
#include <TSAutosar.h>
#include <Platforms_Modules.h>          /* platform-specific config pointer
                                           names */
#include <Base_Modules.h>               /* Module enable defines and
                                           standard config pointer names */
#include <Eb_Intgr_BswM_Modules.h>      /* Init function call defines    */

/*==================[macros]================================================*/

/* Default parameter for the MCU init clock API. Platform-specific values
 * may be defined in header file "Platforms_Modules.h" located in the Platforms
 * module.
 */
#ifndef MCU_CLOCK_CFG_PARAM
#define MCU_CLOCK_CFG_PARAM 0U
#endif /* #ifndef MCU_CLOCK_CFG_PARAM */

/*==================[external data]=========================================*/

/*==================[external function declarations]========================*/

#define MCU_START_SEC_APPL_CODE
#include <MemMap.h>

extern FUNC(void,MCU_APPL_CODE) Eb_Intgr_McuArchClockInit_Init(void);

#define MCU_STOP_SEC_APPL_CODE
#include <MemMap.h>


#endif  /* #ifndef EB_INTGR_MCUARCHCLOCKINIT_H */

/*==================[end of file]===========================================*/
