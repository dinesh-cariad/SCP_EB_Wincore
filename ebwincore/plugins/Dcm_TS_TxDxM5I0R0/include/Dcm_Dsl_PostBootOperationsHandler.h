/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* !LINKSTO Dcm.Dsn.File.PostBootOperationsHandler.PublicApi,1 */
/* This file contains the dummy public API of the PostBootOperationsHandler */

#ifndef DCM_DSL_POSTBOOTOPERATIONSHANDLER_H
#define DCM_DSL_POSTBOOTOPERATIONSHANDLER_H

/*==================[type definitions]===========================================================*/
/** \brief States of PostBootOperationsHandler */

/** \brief Initial state. Immediately after boot-up the Dcm checks the ProgConditions and sees
           whether a PostBoot Operation is required */
#if (defined DCM_POSTBOOT_INITIALIZE_WARM_START)
  #error "DCM_POSTBOOT_INITIALIZE_WARM_START is already defined"
#endif
#define DCM_POSTBOOT_INITIALIZE_WARM_START 0U

/** \brief A PostBoot Operation was required and the necessary request has been given to the
           relevant RxConnection. This might or might not be processed, but until this time
           the state machine of the PostBootOperationsHandler must wait. */
#if (defined DCM_POSTBOOT_WAIT_FOR_PROCESSING)
  #error "DCM_POSTBOOT_WAIT_FOR_PROCESSING is already defined"
#endif
#define DCM_POSTBOOT_WAIT_FOR_PROCESSING 1U

/** \brief The PostBoot Operation has been processed (either successfully or unsuccessfully).
           the PostBootOperationsHandler must now decide if the ProgConditions need updating. */
#if (defined DCM_POSTBOOT_ACTIONS_AFTER_PROCESSING)
  #error "DCM_POSTBOOT_ACTIONS_AFTER_PROCESSING is already defined"
#endif
#define DCM_POSTBOOT_ACTIONS_AFTER_PROCESSING 2U

/** \brief The ProgConditions need updating and Dcm_SetProgConditions will be called successfully
           for this. */
#if (defined DCM_POSTBOOT_SET_PROGRAMMING_CONDITIONS)
  #error "DCM_POSTBOOT_SET_PROGRAMMING_CONDITIONS is already defined"
#endif
#define DCM_POSTBOOT_SET_PROGRAMMING_CONDITIONS 3U

/** \brief Processing of all PostBoot Operations is finished. */
#if (defined DCM_POSTBOOT_OPERATIONS_FINISHED)
  #error "DCM_POSTBOOT_OPERATIONS_FINISHED is already defined"
#endif
#define DCM_POSTBOOT_OPERATIONS_FINISHED 4U

typedef uint8 Dcm_PostBootOperationsStateType;

/*==================[external function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>
/** \brief Initializes the PostBootOperationsHandler. Within this function, the
 **        PostBootOperationsHandler reads the ProgConditions and decides whether any
 **        PostBootOperations are in order.
 **
 ** \return none
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_PostBootOperationsHandler_Init(
  void
);

/** \brief Executes the actions of the PostBootOperationsHandler. Within this function, the
 **        PostBootOperationsHandler state machine coordinates its actions.
 **
 ** \return none
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_PostBootOperationsHandler_MainFunction(
  void
);

/** \brief Interrogates the PostBootOperationsHandler whether a jump to the Bootloader (in
 **        the context of, for example, the DiagnosticSessionControl (0x10) service is allowed.
 **
 ** \retval TRUE         Jump to the Bootloader is inhibited.
 ** \retval FALSE        Jump to the Bootloader is allowed.
 */
extern FUNC (boolean, DCM_CODE) Dcm_Dsl_PostBootOperationsHandler_JumpInhibited(
  void
);

/** \brief Called by the Protocol or the Supervisor in order to notify the
 **        PostBootOperationsHandler that the requested actions have been completed and to
 **        allow it to move to the next phase where the ProgConditions are updated. The Supervisor
 **        calls this in an error condition and the Dsp calls this when the PostBoot answer has
 **        been processed (either successfully or unsuccessfully).
 **
 ** \retval TRUE         Jump to the Bootloader is inhibited.
 ** \retval FALSE        Jump to the Bootloader is allowed.
 */
extern FUNC (void, DCM_CODE) Dcm_Dsl_PostBootOperationsHandler_ActionsCompleted(
  void
);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* ifndef DCM_DSL_POSTBOOTOPERATIONSHANDLER_H */
/*==================[end of file]================================================================*/
