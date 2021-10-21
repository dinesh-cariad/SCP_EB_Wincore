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

#ifndef EB_INTGR_BSWM_USERCALLOUTS_H
#define EB_INTGR_BSWM_USERCALLOUTS_H


/*==================[includes]==============================================*/

#include <TSAutosar.h>     /* EB specific standard types */


/*==================[macros]================================================*/

/* BswM states:
 * The values assigned to the macros must be less than or equal to the value
 * you have configured in parameter "BswMRequestedModeMax" of the corresponding
 * BswM mode request port (the value of parameter "BswMModeRequestSource" of
 * this mode request port has to be "BswMGenericRequest"). */
#define EB_INTGR_BSWM_BSWM_SM        0u  /* the BswMModeRequesterId of the BswM state
                                          * machine. */

#define EB_INTGR_BSWM_STARTUP_TWO_A  0u  /*!< ECU state handled by BswM module. */
#define EB_INTGR_BSWM_STARTUP_TWO_B  1u  /*!< ECU state handled by BswM module. */
#define EB_INTGR_BSWM_RUN_TWO        2u  /*!< ECU state handled by BswM module. */
#define EB_INTGR_BSWM_GO_OFF_ONE_A   3u  /*!< ECU state handled by BswM module. */
#define EB_INTGR_BSWM_GO_OFF_ONE_B   4u  /*!< ECU state handled by BswM module. */
#define EB_INTGR_BSWM_WAKEUP_TWO     5u  /*!< ECU state handled by BswM module. */

/* SWC_CyclicCounter states:
 * The values assigned to the macros must be less than or equal to the value
 * you have configured in parameter "BswMRequestedModeMax" of the corresponding
 * BswM mode request port (the value of parameter "BswMModeRequestSource" of
 * this mode request port has to be "BswMGenericRequest"). */
#define EB_INTGR_BSWM_SWC_CYCLIC             1u  /* the BswMModeRequesterId of the 
                                                  * SWC_CyclicCounter. */

#define EB_INTGR_BSWM_SWC_CYCLIC_UNINIT      0u  /* SWC_CyclicCounter state */
#define EB_INTGR_BSWM_SWC_CYCLIC_RUN         1u  /* SWC_CyclicCounter state */
#define EB_INTGR_BSWM_SWC_CYCLIC_SHUTDOWN    2u  /* SWC_CyclicCounter state */


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/* AUTOSAR Memory Mapping - start section for code */
#define BSWM_START_SEC_CODE
#include <BswM_MemMap.h>


extern FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnStartupTwoA(void);
extern FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnStartupTwoB(void);
extern FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_DriverInitTwo(void);
extern FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_DriverInitThree(void);
extern FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnRunTwo(void);
extern FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnPrpShutdown(void);
extern FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnGoOffOneA(void);
extern FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnGoOffOneB(void);
extern FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnGoOffTwo(void);
extern FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnWakeupTwo(void);


#define BSWM_STOP_SEC_CODE
#include <BswM_MemMap.h>


/*==================[internal function definitions]=========================*/


#endif /* ifndef EB_INTGR_BSWM_USERCALLOUTS_H */

/*==================[end of file]===========================================*/
