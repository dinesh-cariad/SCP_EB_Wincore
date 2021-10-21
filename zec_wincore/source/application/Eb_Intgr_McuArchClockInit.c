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

/*==================[inclusions]============================================*/
#include <TSAutosar.h>
#include <Mcu.h>
#include <Eb_Intgr_McuArchClockInit.h>

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define MCU_START_SEC_APPL_CODE
#include <MemMap.h>


/**
 * \brief  Function "MyMcuArchClockInit_Init" additionally initializes the
 *         clocks.
 *
 * The procedure steps of this function are relevant for all platforms except
 * from WIN32X86. The function must be available for all platforms in order to
 * unify the EcuM configuration.
 */

FUNC(void,MCU_APPL_CODE) Eb_Intgr_McuArchClockInit_Init(void)
{
#ifndef _X86_

  /*
   * If MCU_INIT_CLOCK/MCU_INIT_CLOCK_API is set to STD_OFF, the clock initialization is disabled
   * in the MCU driver.
   */
#if (( MCU_INIT_CLOCK_API == STD_ON )||( MCU_INIT_CLOCK == STD_ON ))

  MCU_CLOCK_INIT_FUNC(MCU_CLOCK_CFG_PARAM);

  /* Wait until the PLL is locked */
  while (MCU_PLL_LOCKED != Mcu_GetPllStatus())
  {
    ;
  }

  /* Activate the PLL Clock */
  Mcu_DistributePllClock();

#endif /* #if ( MCU_INIT_CLOCK == STD_ON ) */

#endif /* #ifndef _X86_ */
}


#define MCU_STOP_SEC_APPL_CODE
#include <MemMap.h>


/*==================[end of file]===========================================*/
